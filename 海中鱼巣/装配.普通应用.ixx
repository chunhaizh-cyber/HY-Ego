module;

#include <cstdint>
#include <memory>

export module 海中鱼巣.装配.普通应用;

import 海中鱼巣.核心.服务.L1事实基座;

export namespace 海中鱼巣 {

struct 普通应用配置 {
    bool 有效() const noexcept {
        return true;
    }
};

class 普通应用上下文 {
public:
    普通应用上下文() = default;
    普通应用上下文(const 普通应用上下文&) = delete;
    普通应用上下文& operator=(const 普通应用上下文&) = delete;
    普通应用上下文(普通应用上下文&&) = delete;
    普通应用上下文& operator=(普通应用上下文&&) = delete;

    L1事实基座服务 L1;
};

enum class 普通应用装配状态 : std::uint8_t {
    已装配 = 0,
    配置无效 = 1,
    构造失败 = 2
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
        return {普通应用装配状态::已装配, std::make_unique<普通应用上下文>()};
    } catch (...) {
        return {普通应用装配状态::构造失败, nullptr};
    }
}

}
