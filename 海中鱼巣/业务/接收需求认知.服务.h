#pragma once
#include "../领域/接收认知来源.数据.h"
#include "../领域/新_需求类.h"
#include <memory>
#include <optional>
#include <stop_token>
#include <string>
#include <string_view>

namespace 海中鱼巣 {
class 新_外设世界树类; class 新_自然语言世界树类;
class 新_语言类; class 新_词类; class 概念_名称关系类;
class 概念_存在类; class 概念_特征类; class 概念_二次特征类;
class 新_存在类; class 新_特征值类; class 新_特征类;
class 新_场景类; class 新_需求类; class 新_任务类; class 新_方法类; class 新_动态类;
struct 新任务字段追问记录; struct 新本能函数本次条件结果对; class 方法参数场景;
struct 接收需求认知会话语境 final {
    std::string 公开宿主会话;
    std::optional<稳定编码> 已绑定人类;
    稳定编码 现实根,语言;
    std::optional<稳定编码> 已绑定自我{};
    friend bool operator==(const 接收需求认知会话语境&,const 接收需求认知会话语境&)=default;
};
enum class 接收认知语境接纳状态 : std::uint8_t {
    已登记=1,精确重复,来源冲突,输入拒绝,资源失败,停止接收
};
enum class 接收需求认知阶段 : std::uint8_t {
    未处理=1,已采用U,已建立D,已读回,非支持输入,对象未决,
    非本次人类需求,D映射拒绝,D映射受阻,前提变化,资源延后,停止未完成,已读回行动请求=13,已读回待完善请求=14
};
enum class 接收行动请求交接确认状态 : std::uint8_t {
    已确认=1,精确重复,来源冲突,输入拒绝,资源失败
};
struct 接收需求认知观察 final {
    稳定编码 M;
    std::optional<稳定编码> H,S,U,D;
    接收需求认知阶段 阶段=接收需求认知阶段::未处理;
    std::string 原因;
    std::optional<稳定编码> 受请求者{};
    std::optional<新接收认知来源键> 需求来源{};
    std::optional<新行动请求承接来源> 待交接请求{};
    std::optional<新需求字段追问来源> 待完善交接{};
};
// 普通应用持有，self单消费者推进；依赖必须存活到self最终join。
class 接收需求认知服务 final {
public:
    接收需求认知服务(新_外设世界树类&,新_自然语言世界树类&,
        新_语言类&,新_词类&,概念_名称关系类&,概念_存在类&,
        概念_特征类&,概念_二次特征类&,新_存在类&,
        新_特征值类&,新_特征类&,新_场景类&,新_需求类&);
    ~接收需求认知服务();
    接收认知语境接纳状态 登记会话语境(const 接收需求认知会话语境&);
    void 封存会话(std::string_view) noexcept;
    std::optional<接收需求认知观察> 推进一次(std::stop_token);
    // 与推进一次相同，由self唯一消费者串行调用；只确认邮箱接纳。
    接收行动请求交接确认状态 确认行动请求已交接(
        const 新行动请求承接来源&) noexcept;
    std::optional<接收需求认知观察> 读取最近观察() const;
    void 装配任务读取接口(新_任务类&);
    void 装配字段答案执行(新_方法类&,新_动态类&,稳定编码 通用存在概念);
    std::optional<新需求字段答案写回请求> 读取字段答案候选(const 新任务字段追问记录&) const;
    新本能函数本次条件结果对 执行字段答案写回本能(
        const 新需求字段答案写回请求&,新_需求类&,const 方法参数场景&) const;
    // 仅从原M及仍活跃的正式会话投影，不回退到当前窗口或猜测对端。
    std::optional<新需求字段追问来源> 读取字段追问来源(
        稳定编码 被认知需求,稳定编码 自我,稳定编码 自我场景,稳定编码 父需求) const;

private:
    struct 实现;
    std::unique_ptr<实现> 实现_;
};
} // namespace 海中鱼巣
