#pragma once
#include "../领域/接收认知来源.数据.h"
#include "../领域/前置语言输入.数据.h"
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
class 新_场景类; class 新_需求类;
struct 新任务前置语言输入结果;
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
    非本次人类需求,D映射拒绝,D映射受阻,前提变化,资源延后,停止未完成,已读回行动请求=13,
    前置输入待治理,前置输入治理等待,
    // 已退役（ASK-READ-SCENE v0.4 §3.3 第5项不做问答匹配写回）；保留编号，不再产生。
    已回答写回,
    // ASK-READ-SCENE v0.4 §3.3：值类构式或招呼已在接收来源场景中保存为场景信息；不建立需求，消息到此结束。
    已保存场景信息
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
    std::optional<新前置语言输入定位> 待交接前置输入{};
    std::optional<新前置语言输入材料> 前置输入等待材料{};
};
// 普通应用持有；推进/ACK仍由self唯一消费者串行调用，worker可并发只读。
// 服务与依赖必须存活到全部相关self/manager/worker最终join。
class 接收需求认知服务 final : public 新前置语言输入读取接口 {
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
    接收行动请求交接确认状态 确认前置输入已接纳(const 新前置语言输入定位&) noexcept;
    接收行动请求交接确认状态 确认前置输入已筹办(const 新任务前置语言输入结果&) noexcept;
    新前置语言输入读取结果 读取前置语言输入材料(
        const 新前置语言输入定位&,std::stop_token) const override;
    std::optional<接收需求认知观察> 读取最近观察() const;
    // ASK-READ-SCENE v0.4 §3.3 第4项：说话者取自我的交互者特征当前值；须在首次推进前由装配设置一次。
    void 设置交互者特征类型(稳定编码) noexcept;
private:
    struct 实现;
    std::unique_ptr<实现> 实现_;
};
} // namespace 海中鱼巣
