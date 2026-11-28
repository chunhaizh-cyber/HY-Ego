#include "自检.当前快照存储.h"
#include "../核心/基础数据集.快照格式.h"

#include <bit>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <limits>
#include <string_view>
#include <thread>

namespace 海中鱼巣 {
namespace {
using S = 基础持久状态;

基础快照映像 示例(bool 第二版) {
    基础快照映像 r;
    r.数据集身份[0] = 42;
    r.下一个编码 = 第二版 ? 12 : 11;
    r.节点组 = {{{1}}, {{2}}, {{3}}};
    r.外部关系组 = {{{4}, {1}, {2}, 基础外部关系类型::父子, -1}};
    r.字段关系组 = {
        {{5}, {1}, {3}, 稳定编码{2}},
        {{6}, {1}, {3}, 基础值{第二版 ? std::int64_t{30} : (std::numeric_limits<std::int64_t>::min)()}},
        {{7}, {1}, {3}, 基础值{std::vector<std::int64_t>{-1, 0, 1}}},
        {{8}, {1}, {3}, 基础值{std::vector<std::uint64_t>{0, (std::numeric_limits<std::uint64_t>::max)()}}},
        {{9}, {2}, {3}, 基础值{std::string{"A\0B", 3}}}
    };
    return r;
}

bool 验证编解码() {
    const auto 原文 = 示例(false);
    const auto [编码状态, 字节] = 编码基础快照(原文);
    if (编码状态 != S::已读取 || 字节.size() < 4 || 字节[0] != 1 || 字节[1] || 字节[2] || 字节[3])
        return false;
    const auto [状态, 映像] = 解码基础快照(字节);
    if (状态 != S::已读取 || 映像 != 原文) return false;
    auto 破坏 = 字节;
    破坏[0] = 2;
    if (解码基础快照(破坏).first != S::格式不支持) return false;
    破坏 = 字节;
    破坏.pop_back();
    if (解码基础快照(破坏).first != S::材料不完整) return false;
    auto 不合法 = 原文;
    不合法.节点组[1] = 不合法.节点组[0];
    if (编码基础快照(不合法).first != S::结构冲突) return false;
    不合法 = 原文;
    不合法.外部关系组[0].目标节点 = {10};
    if (编码基础快照(不合法).first != S::结构冲突) return false;
    不合法 = 原文;
    不合法.字段关系组[0].内容 = 稳定编码{10};
    if (编码基础快照(不合法).first != S::结构冲突) return false;
    return true;
}

bool 验证载体(const std::filesystem::path& 根, std::string_view 操作, std::string_view 案例) {
    const auto [状态, 会话] = 基础持久存储::打开(根);
    if (操作 == "reject") {
        if (会话) return false;
        if (案例 == "occupied") return 状态 == S::存储占用;
        if (案例 == "missing") return 状态 == S::材料不完整;
        if (案例 == "digest") return 状态 == S::摘要不一致;
        if (案例 == "format") return 状态 == S::格式不支持;
        return false;
    }
    if (!会话) return false;
    if ((操作 == "hold" || 操作 == "empty") && 案例 == "-") {
        if (状态 != S::空仓 || 会话->读取().first != S::空仓) return false;
        if (操作 == "hold") {
            std::puts("snapshot session held");
            std::fflush(stdout);
            std::this_thread::sleep_for(std::chrono::seconds(8));
        }
        return true;
    }
    if (案例 != "1" && 案例 != "2") return false;
    const bool 第二版 = 案例 == "2";
    const auto 映像 = 示例(第二版);
    const auto 读回 = [&](const 基础快照映像& 预期) {
        const auto [存储状态, 字节] = 会话->读取();
        if (存储状态 != S::已读取) return false;
        const auto [解码状态, 实际] = 解码基础快照(字节);
        return 解码状态 == S::已读取 && 实际 == 预期;
    };
    if (操作 == "read") return 状态 == S::已读取 && 读回(映像);
    if (操作 != "write") return false;
    if (第二版 ? (状态 != S::已读取 || !读回(示例(false))) : (状态 != S::空仓)) return false;
    const auto [编码状态, 字节] = 编码基础快照(映像);
    return 编码状态 == S::已读取 && 会话->提交(字节) == S::已提交 && 读回(映像);
}
}

bool 运行当前快照存储工程验证(const std::array<std::string, 3>& 参数) noexcept {
    try {
        const auto& [操作, 路径, 案例] = 参数;
        const std::filesystem::path 输入{std::u8string(路径.begin(), 路径.end())};
        if (!输入.is_absolute() || 输入 != 输入.lexically_normal()) return false;
        const auto 根 = std::filesystem::canonical(输入);
        const auto 临时 = std::filesystem::canonical(std::filesystem::temp_directory_path());
        if (根.parent_path() != 临时 || !根.filename().wstring().starts_with(L"HY-EGO-CURRENT-SNAPSHOT-VERIFY-"))
            return false;
        const bool 通过 = 操作 == "codec" && 案例 == "-" ? 验证编解码() : 验证载体(根, 操作, 案例);
        std::puts(通过 ? "current snapshot verification passed" : "current snapshot verification failed");
        return 通过;
    } catch (...) {
        return false;
    }
}
}
