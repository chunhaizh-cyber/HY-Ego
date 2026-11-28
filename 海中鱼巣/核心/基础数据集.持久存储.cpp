#include "基础数据集.持久存储.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <bcrypt.h>

#include <algorithm>
#include <array>
#include <limits>
#include <new>
#include <optional>
#include <stdexcept>
#include <string>

namespace 海中鱼巣 {
namespace {

using 状态 = 基础持久状态;
using 字节 = std::uint8_t;
using 摘要 = std::array<字节, 32>;

constexpr wchar_t 清单文件[] = L"current.manifest";
constexpr wchar_t 会话文件[] = L"session.lock";
constexpr const wchar_t* 槽文件[] = {L"snapshot-a.bin", L"snapshot-b.bin"};
constexpr std::uint32_t 当前清单格式 = 1;
constexpr std::uint32_t 当前载荷格式 = 1;
constexpr std::size_t 清单字节数 = 57;
constexpr std::size_t 分块字节数 = 4 * 1024 * 1024;

struct 文件句柄 final {
    HANDLE 句柄 = INVALID_HANDLE_VALUE;
    ~文件句柄() { 关闭(); }
    文件句柄() = default;
    文件句柄(const 文件句柄&) = delete;
    文件句柄& operator=(const 文件句柄&) = delete;

    bool 关闭() noexcept {
        if (句柄 == INVALID_HANDLE_VALUE) return true;
        const HANDLE 原句柄 = std::exchange(句柄, INVALID_HANDLE_VALUE);
        return CloseHandle(原句柄) != FALSE;
    }
};

bool 是普通文件(HANDLE 句柄, bool& 结果) noexcept {
    BY_HANDLE_FILE_INFORMATION 信息{};
    if (!GetFileInformationByHandle(句柄, &信息)) return false;
    结果 = GetFileType(句柄) == FILE_TYPE_DISK &&
        (信息.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_REPARSE_POINT)) == 0;
    return true;
}

struct 清单 final {
    std::uint64_t 序号 = 0;
    字节 活动槽 = 0;
    std::uint64_t 载荷长度 = 0;
    摘要 载荷摘要{};
    friend bool operator==(const 清单&, const 清单&) = default;
};

template<class 数值>
void 写小端(std::span<字节> 输出, std::size_t 起点, 数值 值) noexcept {
    for (std::size_t i = 0; i < sizeof(数值); ++i) {
        输出[起点 + i] = static_cast<字节>(值 >> (i * 8));
    }
}

template<class 数值>
数值 读小端(std::span<const 字节> 输入, std::size_t 起点) noexcept {
    数值 值 = 0;
    for (std::size_t i = 0; i < sizeof(数值); ++i) {
        值 |= static_cast<数值>(输入[起点 + i]) << (i * 8);
    }
    return 值;
}

std::array<字节, 清单字节数> 编码(const 清单& 内容) noexcept {
    std::array<字节, 清单字节数> 输出{};
    写小端(输出, 0, 当前清单格式);
    写小端(输出, 4, 内容.序号);
    输出[12] = 内容.活动槽;
    写小端(输出, 13, 当前载荷格式);
    写小端(输出, 17, 内容.载荷长度);
    std::copy(内容.载荷摘要.begin(), 内容.载荷摘要.end(), 输出.begin() + 25);
    return 输出;
}

bool 合法载荷大小(std::uint64_t 大小) noexcept {
    return 大小 != 0 && 大小 <= std::vector<字节>{}.max_size() &&
        大小 <= static_cast<std::uint64_t>(std::numeric_limits<LONGLONG>::max());
}

状态 解码(std::span<const 字节> 输入, 清单& 输出) noexcept {
    if (输入.size() != 清单字节数) return 状态::材料不完整;
    if (读小端<std::uint32_t>(输入, 0) != 当前清单格式 ||
        读小端<std::uint32_t>(输入, 13) != 当前载荷格式) {
        return 状态::格式不支持;
    }
    输出.序号 = 读小端<std::uint64_t>(输入, 4);
    输出.活动槽 = 输入[12];
    输出.载荷长度 = 读小端<std::uint64_t>(输入, 17);
    if (输出.序号 == 0 || 输出.活动槽 > 1 || !合法载荷大小(输出.载荷长度)) {
        return 状态::结构冲突;
    }
    std::copy(输入.begin() + 25, 输入.end(), 输出.载荷摘要.begin());
    return 状态::已读取;
}

struct 哈希算法 final {
    BCRYPT_ALG_HANDLE 句柄 = nullptr;
    ~哈希算法() { if (句柄) BCryptCloseAlgorithmProvider(句柄, 0); }
};

struct 哈希对象 final {
    BCRYPT_HASH_HANDLE 句柄 = nullptr;
    ~哈希对象() { if (句柄) BCryptDestroyHash(句柄); }
};

bool 计算摘要(std::span<const 字节> 载荷, 摘要& 输出) noexcept {
    哈希算法 算法;
    if (!BCRYPT_SUCCESS(BCryptOpenAlgorithmProvider(&算法.句柄, BCRYPT_SHA256_ALGORITHM, nullptr, 0))) {
        return false;
    }
    哈希对象 哈希;
    if (!BCRYPT_SUCCESS(BCryptCreateHash(算法.句柄, &哈希.句柄, nullptr, 0, nullptr, 0, 0))) {
        return false;
    }
    while (!载荷.empty()) {
        const auto 长度 = std::min(载荷.size(), 分块字节数);
        if (!BCRYPT_SUCCESS(BCryptHashData(哈希.句柄, const_cast<字节*>(载荷.data()),
            static_cast<ULONG>(长度), 0))) {
            return false;
        }
        载荷 = 载荷.subspan(长度);
    }
    return BCRYPT_SUCCESS(BCryptFinishHash(哈希.句柄, 输出.data(), static_cast<ULONG>(输出.size()), 0));
}

状态 读材料(const std::filesystem::path& 路径, std::uint64_t 预期长度,
    std::vector<字节>& 输出) {
    文件句柄 文件;
    文件.句柄 = CreateFileW(路径.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
        FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
    if (文件.句柄 == INVALID_HANDLE_VALUE) {
        return GetLastError() == ERROR_FILE_NOT_FOUND ? 状态::材料不完整 : 状态::资源失败;
    }
    bool 普通 = false;
    if (!是普通文件(文件.句柄, 普通)) return 状态::资源失败;
    if (!普通) return 状态::结构冲突;
    LARGE_INTEGER 长度{};
    if (!GetFileSizeEx(文件.句柄, &长度)) return 状态::资源失败;
    if (长度.QuadPart < 0 || static_cast<std::uint64_t>(长度.QuadPart) != 预期长度) {
        return 状态::材料不完整;
    }
    输出.resize(static_cast<std::size_t>(预期长度));
    std::span<字节> 剩余{输出};
    while (!剩余.empty()) {
        const auto 请求 = static_cast<DWORD>(std::min(剩余.size(), 分块字节数));
        DWORD 实读 = 0;
        if (!ReadFile(文件.句柄, 剩余.data(), 请求, &实读, nullptr)) return 状态::资源失败;
        if (实读 == 0) return 状态::材料不完整;
        剩余 = 剩余.subspan(实读);
    }
    return 状态::已读取;
}

// 只用固定路径判空。遗留槽与失踪的清单不能被解释为新数据集。
状态 探测材料(const std::filesystem::path& 路径, bool& 存在) noexcept {
    const DWORD 属性 = GetFileAttributesW(路径.c_str());
    存在 = 属性 != INVALID_FILE_ATTRIBUTES;
    if (!存在 && GetLastError() != ERROR_FILE_NOT_FOUND) return 状态::资源失败;
    return 状态::已读取;
}

struct 当前材料 final {
    状态 结果 = 状态::资源失败;
    std::optional<清单> 元数据;
    std::vector<字节> 载荷;
};

当前材料 读当前材料(const std::filesystem::path& 目录) {
    const auto 清单路径 = 目录 / 清单文件;
    bool 清单存在 = false;
    auto 结果 = 探测材料(清单路径, 清单存在);
    if (结果 != 状态::已读取) return {结果};
    if (!清单存在) {
        for (const auto* 文件 : 槽文件) {
            bool 存在 = false;
            结果 = 探测材料(目录 / 文件, 存在);
            if (结果 != 状态::已读取) return {结果};
            if (存在) return {状态::材料不完整};
        }
        return {状态::空仓};
    }
    std::vector<字节> 清单内容;
    结果 = 读材料(清单路径, 清单字节数, 清单内容);
    if (结果 != 状态::已读取) return {结果};
    清单 当前;
    结果 = 解码(清单内容, 当前);
    if (结果 != 状态::已读取) return {结果};
    std::vector<字节> 载荷;
    结果 = 读材料(目录 / 槽文件[当前.活动槽], 当前.载荷长度, 载荷);
    if (结果 != 状态::已读取) return {结果};
    摘要 实际摘要{};
    if (!计算摘要(载荷, 实际摘要)) return {状态::资源失败};
    if (实际摘要 != 当前.载荷摘要) return {状态::摘要不一致};
    return {状态::已读取, 当前, std::move(载荷)};
}

struct 临时文件 final {
    std::filesystem::path 路径;
    bool 自有 = false;
    ~临时文件() { if (自有) DeleteFileW(路径.c_str()); }
};

bool 写临时文件(const std::filesystem::path& 目录, std::span<const 字节> 数据, 临时文件& 输出) {
    std::array<字节, 16> 随机{};
    if (!BCRYPT_SUCCESS(BCryptGenRandom(nullptr, 随机.data(), static_cast<ULONG>(随机.size()),
        BCRYPT_USE_SYSTEM_PREFERRED_RNG))) {
        return false;
    }
    constexpr wchar_t 十六进制[] = L"0123456789abcdef";
    std::wstring 文件名 = L"current-snapshot-tmp-";
    for (const auto 字节值 : 随机) {
        文件名 += 十六进制[字节值 >> 4];
        文件名 += 十六进制[字节值 & 15];
    }
    输出.路径 = 目录 / 文件名;
    文件句柄 文件;
    文件.句柄 = CreateFileW(输出.路径.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL, nullptr);
    if (文件.句柄 == INVALID_HANDLE_VALUE) return false;
    输出.自有 = true;
    while (!数据.empty()) {
        const auto 请求 = static_cast<DWORD>(std::min(数据.size(), 分块字节数));
        DWORD 实写 = 0;
        if (!WriteFile(文件.句柄, 数据.data(), 请求, &实写, nullptr) || 实写 == 0) return false;
        数据 = 数据.subspan(实写);
    }
    return FlushFileBuffers(文件.句柄) && 文件.关闭();
}

bool 原子替换(临时文件& 文件, const std::filesystem::path& 目标) noexcept {
    if (!MoveFileExW(文件.路径.c_str(), 目标.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        return false;
    }
    文件.自有 = false;
    return true;
}

} // namespace

struct 基础持久存储::实现 final {
    std::filesystem::path 目录;
    文件句柄 独占锁;
    std::optional<清单> 已知清单;
    bool 发布未决 = false;
};

基础持久存储::基础持久存储(std::unique_ptr<实现> 会话) : 会话_(std::move(会话)) {}
基础持久存储::~基础持久存储() = default;

std::pair<状态, std::unique_ptr<基础持久存储>> 基础持久存储::打开(
    const std::filesystem::path& 目录) {
    try {
        if (!目录.is_absolute() || 目录 != 目录.lexically_normal()) return {状态::入口拒绝, nullptr};
        std::error_code 错误;
        const auto 目录状态 = std::filesystem::status(目录, 错误);
        if (错误 && 错误 != std::errc::no_such_file_or_directory) return {状态::资源失败, nullptr};
        if (!std::filesystem::is_directory(目录状态)) return {状态::入口拒绝, nullptr};
        auto 会话 = std::make_unique<实现>();
        会话->目录 = std::filesystem::canonical(目录, 错误);
        if (错误) return {状态::资源失败, nullptr};
        const auto 锁路径 = 会话->目录 / 会话文件;
        会话->独占锁.句柄 = CreateFileW(锁路径.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr,
            OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OPEN_REPARSE_POINT, nullptr);
        if (会话->独占锁.句柄 == INVALID_HANDLE_VALUE) {
            const auto 原因 = GetLastError();
            return {原因 == ERROR_SHARING_VIOLATION ? 状态::存储占用 : 状态::资源失败, nullptr};
        }
        bool 普通 = false;
        if (!是普通文件(会话->独占锁.句柄, 普通)) return {状态::资源失败, nullptr};
        if (!普通) return {状态::结构冲突, nullptr};
        const auto 当前 = 读当前材料(会话->目录);
        if (当前.结果 != 状态::空仓 && 当前.结果 != 状态::已读取) return {当前.结果, nullptr};
        会话->已知清单 = 当前.元数据;
        return {当前.结果, std::unique_ptr<基础持久存储>(new 基础持久存储(std::move(会话)))};
    } catch (const std::bad_alloc&) {
        return {状态::资源失败, nullptr};
    } catch (const std::length_error&) {
        return {状态::资源失败, nullptr};
    } catch (const std::filesystem::filesystem_error&) {
        return {状态::资源失败, nullptr};
    }
}

std::pair<状态, std::vector<字节>> 基础持久存储::读取() {
    if (会话_->发布未决) return {状态::发布结果未知, {}};
    try {
        auto 当前 = 读当前材料(会话_->目录);
        if ((当前.结果 == 状态::空仓 || 当前.结果 == 状态::已读取) &&
            当前.元数据 != 会话_->已知清单) {
            return {状态::结构冲突, {}};
        }
        return {当前.结果, std::move(当前.载荷)};
    } catch (const std::bad_alloc&) {
        return {状态::资源失败, {}};
    } catch (const std::length_error&) {
        return {状态::资源失败, {}};
    } catch (const std::filesystem::filesystem_error&) {
        return {状态::资源失败, {}};
    }
}

状态 基础持久存储::提交(std::span<const 字节> 载荷) {
    if (会话_->发布未决) return 状态::发布结果未知;
    if (!合法载荷大小(载荷.size())) return 状态::入口拒绝;
    try {
        const auto 结果 = 读取().first;
        if (结果 != 状态::空仓 && 结果 != 状态::已读取) return 结果;
        清单 新清单;
        if (const auto& 旧清单 = 会话_->已知清单) {
            if (旧清单->序号 == std::numeric_limits<std::uint64_t>::max()) return 状态::结构冲突;
            新清单.序号 = 旧清单->序号 + 1;
            新清单.活动槽 = static_cast<字节>(1 - 旧清单->活动槽);
        } else {
            新清单.序号 = 1;
        }
        新清单.载荷长度 = 载荷.size();
        if (!计算摘要(载荷, 新清单.载荷摘要)) return 状态::资源失败;

        const auto 槽目标 = 会话_->目录 / 槽文件[新清单.活动槽];
        const auto 清单目标 = 会话_->目录 / 清单文件;
        const auto 清单字节 = 编码(新清单);
        临时文件 槽临时;
        if (!写临时文件(会话_->目录, 载荷, 槽临时) || !原子替换(槽临时, 槽目标)) {
            return 状态::资源失败;
        }
        // 清单成功前，已有的活动槽仍是唯一见证。首次发布若在这里中止，
        // 下次启动会把留下的无清单槽拒为不完整，而不会静默开新仓。
        临时文件 清单临时;
        if (!写临时文件(会话_->目录, 清单字节, 清单临时)) return 状态::资源失败;
        if (!原子替换(清单临时, 清单目标)) {
            会话_->发布未决 = true;
            return 状态::发布结果未知;
        }
        会话_->已知清单 = 新清单;
        return 状态::已提交;
    } catch (const std::bad_alloc&) {
        return 状态::资源失败;
    } catch (const std::length_error&) {
        return 状态::资源失败;
    } catch (const std::filesystem::filesystem_error&) {
        return 状态::资源失败;
    }
}

} // namespace 海中鱼巣
