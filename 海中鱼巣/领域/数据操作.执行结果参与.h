#pragma once

#include <memory>
#include <optional>
#include <variant>

#include "../核心/基础数据集.h"
#include "新_状态类.h"
#include "本能词典.数据.h"

namespace 海中鱼巣 {

struct 新同组材料端点 final {
    std::variant<稳定编码, 基础组内节点引用> 材料定位;
    稳定编码 内部组织, 正式材料范围;
    std::size_t 后续新节点起点 = 0;
};
struct 新同组状态端点 final {
    std::variant<稳定编码, 基础组内节点引用> 状态定位;
    稳定编码 被描述节点, 来源特征节点, 特征概念节点;
    std::optional<新特征准确值> 固定准确值;
    新状态强时间 强时间;
    std::size_t 后续新节点起点 = 0;
};
struct 新材料结果追加构造 final { 基础节点组追加请求 追加请求; };
struct 新状态结果追加构造 final {
    基础节点组追加请求 追加请求;
    std::uint32_t 新增状态保持数 = 0;
};
class 新共同发表回执接收接口 {
public:
    virtual ~新共同发表回执接收接口() = default;
    virtual void 登记共同发表回执(const 基础片段发布回执&) noexcept = 0;
};
class 新材料结果引用参与接口 : public virtual 新共同发表回执接收接口 {
public:
    virtual ~新材料结果引用参与接口() = default;
    virtual 新材料结果追加构造 构造同组材料引用(const 新同组材料端点&) const = 0;
};
class 新状态结果引用参与接口 : public virtual 新共同发表回执接收接口 {
public:
    virtual ~新状态结果引用参与接口() = default;
    virtual 新状态结果追加构造 构造同组状态引用(const 新同组状态端点&) const = 0;
};
class 新词典结果参与准备 : public 新材料结果引用参与接口,
                         public 新状态结果引用参与接口 {
public:
    virtual 新材料结果追加构造 构造无新状态采用() const = 0;
    virtual void 登记共同发表回执(const 基础片段发布回执&) noexcept = 0;
    virtual void 完成共同发表() noexcept = 0;
    virtual void 取消未发表准备() noexcept = 0;
};
struct 新隔离材料组端点 final {
    std::vector<新同组材料端点> 材料组;
    std::size_t 后续新节点起点 = 0;
};
class 新隔离材料初值参与接口 : public 新共同发表回执接收接口 {
public:
    virtual 新材料结果追加构造 构造同组初始引用(const 新隔离材料组端点&) const = 0;
};
struct 新词典结果参与准备结果 final {
    新词典材料操作状态 状态 = 新词典材料操作状态::输入拒绝;
    std::unique_ptr<新词典结果参与准备> 准备;
    std::optional<新本能词典读取回执> 已有回执;
};

} // namespace 海中鱼巣
