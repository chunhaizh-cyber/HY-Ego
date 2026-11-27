#pragma once

#include "业务/新_外设世界树类.h"
#include "界面/宿主.网页对话.h"
#include <memory>
#include <optional>
#include <string_view>

namespace 海中鱼巣 {

struct 网页接收会话来源 final {
    std::optional<稳定编码> 接纳会话绑定人;
};
using 网页接收会话来源读取 = std::optional<网页接收会话来源> (*)(std::string_view);

// 组合根保证外设world存活到关闭返回；回调不借用普通应用上下文。
class 网页外设运行模块 final {
public:
    explicit 网页外设运行模块(新_外设世界树类&);
    ~网页外设运行模块();
    新世界初始化结果 初始化() noexcept;
    网页外设运行回调组 取得运行回调(网页接收会话来源读取 = nullptr);
    // 新建外设交互场景后在模块锁外调用一次（6120 §11.1）；启动宿主前设置。
    void 设置交互场景建立通知(void (*通知)(稳定编码) noexcept) noexcept;
    void 设置明确交互确认通知(void (*通知)(std::string_view, std::string_view) noexcept) noexcept;
    void 关闭() noexcept;
    网页外设运行模块(const 网页外设运行模块&) = delete;
    网页外设运行模块& operator=(const 网页外设运行模块&) = delete;
private:
    struct 实现;
    std::shared_ptr<实现> 实现_;
};

} // namespace 海中鱼巣
