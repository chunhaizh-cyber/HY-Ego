#pragma once

#include "基础数据集.快照格式.h"

#include <atomic>

namespace 海中鱼巣 {

class 基础持久视图 final {
public:
    const 基础数据集& 结构() const noexcept { return *结构_; }
    const std::array<std::uint8_t, 16>& 数据集身份() const noexcept { return 身份_; }
    std::uint64_t 下一个编码() const noexcept { return 下一个编码_; }

private:
    friend class 基础持久运行包;
    基础持久视图() = default;

    std::unique_ptr<const 基础数据集> 结构_;
    std::array<std::uint8_t, 16> 身份_{};
    std::uint64_t 下一个编码_ = 0;
};

// 仅底座运行包使用。现有全局写入和普通宿主尚未接入。
class 基础持久运行包 final {
public:
    using 视图 = std::shared_ptr<const 基础持久视图>;
    static std::pair<基础持久状态, std::unique_ptr<基础持久运行包>> 打开(
        const std::filesystem::path& 目录);
    视图 读取() const noexcept { return 当前_.load(std::memory_order_acquire); }
    基础持久状态 提交(const 视图& 预期, const 基础快照映像& 候选);

private:
    基础持久运行包(std::unique_ptr<基础持久存储> 存储, 视图 初始) noexcept;
    static std::pair<基础持久状态, 视图> 准备(std::span<const std::uint8_t> 字节);
    static 基础持久状态 校验变更(const 基础持久视图& 前, const 基础持久视图& 后);

    std::unique_ptr<基础持久存储> 存储_;
    std::atomic<视图> 当前_;
    std::atomic_flag 提交中_{};
    bool 发布未决_ = false; // 仅取得提交资格后使用
};

} // namespace 海中鱼巣
