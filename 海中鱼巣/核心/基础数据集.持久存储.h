#pragma once

#include <cstdint>
#include <filesystem>
#include <memory>
#include <span>
#include <utility>
#include <vector>

namespace 海中鱼巣 {

enum class 基础持久状态 {
    空仓,
    已读取,
    已提交,
    入口拒绝,
    存储占用,
    材料不完整,
    格式不支持,
    摘要不一致,
    结构冲突,
    资源失败,
    发布结果未知,
    内部不一致,
};

// 在同一已存在的存储目录上持有进程间独占会话。调用方串行使用本对象。
class 基础持久存储 final {
public:
    ~基础持久存储();
    基础持久存储(const 基础持久存储&) = delete;
    基础持久存储& operator=(const 基础持久存储&) = delete;

    static std::pair<基础持久状态, std::unique_ptr<基础持久存储>> 打开(
        const std::filesystem::path& 目录);
    std::pair<基础持久状态, std::vector<std::uint8_t>> 读取();
    基础持久状态 提交(std::span<const std::uint8_t> 载荷);

private:
    struct 实现;
    explicit 基础持久存储(std::unique_ptr<实现> 会话);
    std::unique_ptr<实现> 会话_;
};

} // namespace 海中鱼巣
