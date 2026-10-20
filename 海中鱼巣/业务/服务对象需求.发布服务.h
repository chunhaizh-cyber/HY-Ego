#pragma once

#include "新_世界树类.h"
#include "../领域/新_需求类.h"

namespace 海中鱼巣 {

enum class 服务对象需求发布状态 : std::uint8_t {
    已发布 = 1, 已复用, 需求不存在, 主体不符, 非被认知需求,
    需重新确认, 来源不符, 前提已变化, 已关闭, 已读取, 已发布但当前读回变化
};
struct 服务对象需求发布请求 final {
    稳定编码 场景节点, 服务对象节点, 需求节点;
    稳定编码 动作定义节点, 动作来源节点, 执行主体节点;
};
struct 服务对象需求发布结果 final {
    服务对象需求发布状态 状态 = 服务对象需求发布状态::来源不符;
    std::optional<稳定编码> 需求特征节点, 需求节点, 动作节点;
    std::optional<新世界特征变化结果> 变化;
};
struct 服务对象需求集合读取结果 final {
    服务对象需求发布状态 状态 = 服务对象需求发布状态::来源不符;
    std::optional<稳定编码> 需求特征节点;
    std::vector<稳定编码> 需求节点组, 需重新确认节点组;
};

// 编排正式owner；不拥有需求、特征或动作事实，不建立第二份需求账。
class 服务对象需求发布服务 final {
public:
    服务对象需求发布服务(新_世界树类&, 新_需求类&) noexcept;
    ~服务对象需求发布服务();
    bool 初始化(稳定编码 需求集合类型根) noexcept;
    void 关闭() noexcept;
    服务对象需求发布结果 发布已确认需求(const 服务对象需求发布请求&) noexcept;
    服务对象需求集合读取结果 读取主体需求集合(稳定编码 场景节点, 稳定编码 服务对象节点) const noexcept;
private:
    struct 实现;
    std::unique_ptr<实现> 实现_;
};

}
