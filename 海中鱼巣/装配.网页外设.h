#pragma once

#include "业务/新_外设世界树类.h"
#include "界面/宿主.网页对话.h"
#include <memory>

namespace 海中鱼巣 {

// 组合根保证外设world存活到关闭返回；回调不借用普通应用上下文。
class 网页外设运行模块 final {
public:
    explicit 网页外设运行模块(新_外设世界树类&);
    ~网页外设运行模块();
    新世界初始化结果 初始化() noexcept;
    网页外设运行回调组 取得运行回调();
    void 关闭() noexcept;
    网页外设运行模块(const 网页外设运行模块&) = delete;
    网页外设运行模块& operator=(const 网页外设运行模块&) = delete;
private:
    struct 实现;
    std::shared_ptr<实现> 实现_;
};

} // namespace 海中鱼巣
