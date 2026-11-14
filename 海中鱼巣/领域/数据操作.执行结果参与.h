#pragma once

#include <memory>
#include <iterator>
#include <optional>
#include <variant>

#include "../核心/基础数据集.h"
#include "新_状态类.h"
#include "本能词典.数据.h"

namespace 海中鱼巣 {

// 同组端点使用绝对槽位，机械拼接不再平移；不读取或提交数据。
inline void 合并同组结果构造(基础节点组追加请求& 目标, 基础节点组追加请求 来源) {
    const auto 追加 = [](auto& a, auto& b) {
        a.insert(a.end(), std::make_move_iterator(b.begin()), std::make_move_iterator(b.end()));
    };
    追加(目标.新节点组, 来源.新节点组);
    追加(目标.追加字段组, 来源.追加字段组);
    追加(目标.预期既有节点组, 来源.预期既有节点组);
    // 保留各参与者的真实依据；重复宿主由L1规范化，冲突时零发表重读。
    追加(目标.预期联合节点组, 来源.预期联合节点组);
    追加(目标.容许新增入向预期组, 来源.容许新增入向预期组);
    追加(目标.替换字段组, 来源.替换字段组);
    追加(目标.追加外部关系组, 来源.追加外部关系组);
    追加(目标.删除字段关系组, 来源.删除字段关系组);
    追加(目标.只读预期.节点组, 来源.只读预期.节点组);
    追加(目标.只读预期.字段集合组, 来源.只读预期.字段集合组);
    追加(目标.只读预期.外部关系集合组, 来源.只读预期.外部关系集合组);
    追加(目标.删除节点组, 来源.删除节点组);
    追加(目标.删除外部关系组, 来源.删除外部关系组);
}

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
