module;

#include <cstdint>
#include <memory>

export module 海中鱼巣.装配.普通应用;

import 海中鱼巣.核心.服务.L1事实基座;
import 海中鱼巣.领域.服务.L2存在结构;

export namespace 海中鱼巣 {

struct 普通应用配置 {
    bool 有效() const noexcept {
        return true;
    }
};

struct 普通应用装配结果;

class 普通应用上下文 {
public:
    普通应用上下文() = delete;
    普通应用上下文(const 普通应用上下文&) = delete;
    普通应用上下文& operator=(const 普通应用上下文&) = delete;
    普通应用上下文(普通应用上下文&&) = delete;
    普通应用上下文& operator=(普通应用上下文&&) = delete;

    L2存在结构服务& 取得L2存在结构服务() noexcept {
        return *存在服务_;
    }

    const L2存在结构服务& 取得L2存在结构服务() const noexcept {
        return *存在服务_;
    }

private:
    普通应用上下文(L1事实基座运行包&& 运行包,
        std::unique_ptr<L2存在结构服务>&& 存在服务) noexcept
        : 运行包_(std::move(运行包)), 存在服务_(std::move(存在服务)) {}

    friend struct 普通应用装配结果;
    friend 普通应用装配结果 构造普通应用上下文(const 普通应用配置& 配置);
    L1事实基座运行包 运行包_;
    std::unique_ptr<L2存在结构服务> 存在服务_;
};

enum class 普通应用装配状态 : std::uint8_t {
    已装配 = 0,
    配置无效 = 1,
    构造失败 = 2,
    所有者范围建立失败 = 3,
    存在服务构造失败 = 4
};

struct 普通应用装配结果 {
    普通应用装配状态 状态 = 普通应用装配状态::配置无效;
    std::unique_ptr<普通应用上下文> 上下文;

    bool 成功() const noexcept {
        return 状态 == 普通应用装配状态::已装配 && 上下文 != nullptr;
    }
};

普通应用装配结果 构造普通应用上下文(const 普通应用配置& 配置) {
    if (!配置.有效()) {
        return {};
    }
    try {
        auto 运行包 = 建立L1事实基座运行包();
        constexpr L1所有者范围建立幂等身份 存在所有者建立身份{
            0x4C31'4F57'4E45'5231ULL};
        auto 交付 = 运行包.所有者范围签发器().建立所有者范围(
            {L1所有者范围CRUD合同版本, 存在所有者建立身份,
                L1所有者范围种类::独占结构范围});
        if ((交付.建立结果.状态 != L1所有者范围管理状态::成功
                && 交付.建立结果.状态 != L1所有者范围管理状态::精确重复)
            || !交付.建立结果.所有者事实 || !交付.写入端口)
            return {普通应用装配状态::所有者范围建立失败, nullptr};
        std::unique_ptr<L2存在结构服务> 存在服务;
        try {
            存在服务 = std::make_unique<L2存在结构服务>(
                运行包.读取服务(), std::move(*交付.写入端口));
        } catch (...) {
            return {普通应用装配状态::存在服务构造失败, nullptr};
        }
        return {普通应用装配状态::已装配,
            std::unique_ptr<普通应用上下文>(new 普通应用上下文(
                std::move(运行包), std::move(存在服务)))};
    } catch (...) {
        return {普通应用装配状态::构造失败, nullptr};
    }
}

}
