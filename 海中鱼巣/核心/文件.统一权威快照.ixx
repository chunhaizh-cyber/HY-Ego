// 文件规则：只把已编码值式材料写入同目录临时文件，刷新、复读核对后原子替换，不改变运行期事实。
module;

#include "统一权威快照.数据.h"

#include <fstream>
#include <new>
#include <system_error>

#ifdef _WIN32
#include <windows.h>
#endif

export module 海中鱼巣.核心.文件.统一权威快照;

export namespace 海中鱼巣 {

namespace {

bool 读取文件字节(const std::filesystem::path& 路径, std::vector<std::uint8_t>& 字节) {
    std::ifstream 输入(路径, std::ios::binary | std::ios::ate);
    if (!输入) return false;
    const auto 大小 = 输入.tellg();
    if (大小 < 0 || static_cast<std::uint64_t>(大小) > 统一权威文件最大字节数) return false;
    字节.resize(static_cast<std::size_t>(大小));
    输入.seekg(0, std::ios::beg);
    return 字节.empty() || static_cast<bool>(输入.read(reinterpret_cast<char*>(字节.data()), 大小));
}

bool 原子替换(const std::filesystem::path& 临时路径, const std::filesystem::path& 目标路径) {
#ifdef _WIN32
    return MoveFileExW(临时路径.c_str(), 目标路径.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != FALSE;
#else
    std::error_code 错误;
    std::filesystem::rename(临时路径, 目标路径, 错误);
    return !错误;
#endif
}

} // namespace

快照文件保存结果 原子保存快照材料(const 快照文件保存请求& 请求) {
    快照文件保存结果 结果;
    if (请求.目标路径.empty() || 请求.材料.字节.empty() || 请求.材料.字节.size() > 统一权威文件最大字节数) {
        结果.状态 = 快照文件保存结果状态::路径拒绝;
        return 结果;
    }
    const auto 父目录 = 请求.目标路径.parent_path();
    std::error_code 目录错误;
    if (父目录.empty() || !std::filesystem::is_directory(父目录, 目录错误) || 目录错误) {
        结果.状态 = 快照文件保存结果状态::路径拒绝;
        return 结果;
    }
    auto 临时路径 = 请求.目标路径;
    临时路径 += L".临时.";
    临时路径 += std::to_wstring(请求.材料.字节.size());
    try {
        {
            std::ofstream 输出(临时路径, std::ios::binary | std::ios::trunc);
            if (!输出) { 结果.状态 = 快照文件保存结果状态::资源失败; return 结果; }
            输出.write(reinterpret_cast<const char*>(请求.材料.字节.data()), static_cast<std::streamsize>(请求.材料.字节.size()));
            输出.flush();
            if (!输出) { std::error_code 忽略; std::filesystem::remove(临时路径, 忽略); 结果.状态 = 快照文件保存结果状态::资源失败; return 结果; }
        }
        std::vector<std::uint8_t> 复读字节;
        if (!读取文件字节(临时路径, 复读字节) || 复读字节 != 请求.材料.字节) {
            std::error_code 忽略; std::filesystem::remove(临时路径, 忽略); 结果.状态 = 快照文件保存结果状态::内部不一致; return 结果;
        }
        if (!原子替换(临时路径, 请求.目标路径)) {
            std::error_code 忽略; std::filesystem::remove(临时路径, 忽略); 结果.状态 = 快照文件保存结果状态::资源失败; return 结果;
        }
        结果.状态 = 快照文件保存结果状态::已保存;
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = 快照文件保存结果状态::资源失败;
        return 结果;
    } catch (...) {
        结果.状态 = 快照文件保存结果状态::内部不一致;
        return 结果;
    }
}

} // namespace 海中鱼巣
