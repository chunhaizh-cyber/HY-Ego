#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <bcrypt.h>

#include "../海中鱼巣/核心/服务.L1事实基座.h"

#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

namespace {
using namespace 海中鱼巣;

std::uint64_t 通过数量 = 0;

void 检查(bool 条件, const char* 名称) {
    if (!条件) throw std::runtime_error(名称);
    ++通过数量;
    std::cout << "PASS " << 名称 << '\n';
}

void 写U32(std::vector<std::uint8_t>& 输出, std::uint32_t 值) {
    for (unsigned i = 0; i != 4; ++i)
        输出.push_back(static_cast<std::uint8_t>(值 >> (i * 8)));
}

void 写U64(std::vector<std::uint8_t>& 输出, std::uint64_t 值) {
    for (unsigned i = 0; i != 8; ++i)
        输出.push_back(static_cast<std::uint8_t>(值 >> (i * 8)));
}

std::array<std::uint8_t, 32> 摘要(const std::vector<std::uint8_t>& 数据) {
    BCRYPT_ALG_HANDLE 算法 = nullptr;
    BCRYPT_HASH_HANDLE 哈希 = nullptr;
    DWORD 对象长度 = 0, 已写 = 0;
    std::vector<std::uint8_t> 对象;
    std::array<std::uint8_t, 32> 输出{};
    if (BCryptOpenAlgorithmProvider(&算法, BCRYPT_SHA256_ALGORITHM,
            nullptr, 0) < 0
        || BCryptGetProperty(算法, BCRYPT_OBJECT_LENGTH,
            reinterpret_cast<PUCHAR>(&对象长度), sizeof(对象长度), &已写, 0) < 0) {
        if (算法) BCryptCloseAlgorithmProvider(算法, 0);
        throw std::runtime_error("sha256-provider");
    }
    对象.resize(对象长度);
    PUCHAR 输入 = 数据.empty() ? nullptr
        : const_cast<PUCHAR>(reinterpret_cast<const UCHAR*>(数据.data()));
    const bool 成功 = BCryptCreateHash(算法, &哈希, 对象.data(), 对象长度,
            nullptr, 0, 0) >= 0
        && BCryptHashData(哈希, 输入, static_cast<ULONG>(数据.size()), 0) >= 0
        && BCryptFinishHash(哈希, 输出.data(), static_cast<ULONG>(输出.size()), 0) >= 0;
    if (哈希) BCryptDestroyHash(哈希);
    BCryptCloseAlgorithmProvider(算法, 0);
    if (!成功) throw std::runtime_error("sha256-failed");
    return 输出;
}

void 写文件(const std::filesystem::path& 路径,
    const std::vector<std::uint8_t>& 数据) {
    std::ofstream 输出(路径, std::ios::binary | std::ios::trunc);
    if (!输出) throw std::runtime_error("write-open");
    输出.write(reinterpret_cast<const char*>(数据.data()),
        static_cast<std::streamsize>(数据.size()));
    if (!输出) throw std::runtime_error("write-data");
}

std::vector<std::uint8_t> 读文件(const std::filesystem::path& 路径) {
    std::ifstream 输入(路径, std::ios::binary);
    if (!输入) throw std::runtime_error("read-open");
    return {std::istreambuf_iterator<char>(输入), std::istreambuf_iterator<char>()};
}

std::vector<std::uint8_t> 旧格式四载荷() {
    std::vector<std::uint8_t> 输出;
    写U64(输出, 0x3150414E534C3148ULL);
    写U32(输出, 4);
    写U64(输出, 1); // 旧事实代次材料，只用于确认新实现拒绝旧格式。
    return 输出;
}

std::vector<std::uint8_t> 形成清单(const std::vector<std::uint8_t>& 载荷) {
    const auto 哈希 = 摘要(载荷);
    std::vector<std::uint8_t> 输出;
    写U64(输出, 0x31464E414D314C48ULL);
    写U32(输出, 2);
    写U64(输出, 1);
    输出.push_back(1);
    写U64(输出, static_cast<std::uint64_t>(载荷.size()));
    输出.insert(输出.end(), 哈希.begin(), 哈希.end());
    return 输出;
}
} // namespace

int wmain(int 参数数量, wchar_t** 参数) {
    try {
        std::cout << std::unitbuf;
        if (参数数量 != 2) throw std::runtime_error("controlled-root-required");
        const std::filesystem::path 根 = 参数[1];
        if (!根.is_absolute()) throw std::runtime_error("controlled-root-not-absolute");
        std::error_code 错误;
        std::filesystem::create_directories(根, 错误);
        if (错误) throw std::runtime_error("controlled-root-create");

        const auto 载荷 = 旧格式四载荷();
        const auto 清单材料 = 形成清单(载荷);
        const auto 清单路径 = 根 / L"manifest.bin";
        const auto 槽路径 = 根 / L"snapshot-a.bin";
        写文件(槽路径, 载荷);
        写文件(清单路径, 清单材料);

        const auto 建立 = 建立L1事实基座持久运行包({根});
        检查(建立.恢复.状态 == L1事实基座持久恢复状态::格式不支持,
            "legacy-format4-rejected");
        检查(!建立.运行包, "legacy-format4-no-runtime-package");
        检查(读文件(槽路径) == 载荷, "legacy-payload-preserved");
        检查(读文件(清单路径) == 清单材料, "legacy-manifest-preserved");

        std::cout << "PASS total=" << 通过数量 << '\n';
        return 0;
    } catch (const std::exception& 错误) {
        std::cerr << "FAIL " << 错误.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "FAIL unexpected exception\n";
        return 2;
    }
}
