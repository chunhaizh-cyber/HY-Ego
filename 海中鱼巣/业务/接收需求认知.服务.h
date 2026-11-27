#pragma once
#include "../领域/接收认知来源.数据.h"
#include "../领域/前置语言输入.数据.h"
#include "../领域/语言分界.数据.h"
#include <memory>
#include <optional>
#include <stop_token>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace 海中鱼巣 {
class 新_外设世界树类; class 新_自然语言世界树类;
class 关系陈述语言服务;
class 新_语言类; class 新_词类; class 概念_名称关系类;
class 概念_存在类; class 概念_特征类; class 概念_二次特征类;
class 新_存在类; class 新_特征值类; class 新_特征类;
class 新_场景类; class 新_需求类;
struct 新任务前置语言输入结果;
class 字段询问语言服务; class 新_世界树类; class 正式信息参数读取接口; class 新_自我线程类;
struct 接收分界依据状态投递 final {
    新_自我线程类* 自我{};
    bool (*提交)(新_自我线程类&,稳定编码){};
    friend bool operator==(const 接收分界依据状态投递&,const 接收分界依据状态投递&)=default;
};
struct 接收分界询问配置 final {
    const 新_世界树类* 世界;
    std::shared_ptr<const 字段询问语言服务> 表达;
    const 正式信息参数读取接口* 取材;
    稳定编码 外设场景标识类型, 材料范围概念, 候选类型, 依据类型, 有效分界类型;
    稳定编码 第三方指代概念, 第三方别名关系;
    接收分界依据状态投递 依据状态投递;
    friend bool operator==(const 接收分界询问配置&,const 接收分界询问配置&)=default;
};
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
    已保存场景信息, 已采用分界答复, 分界答复不适用, 语言解释待完善
};
enum class 接收行动请求交接确认状态 : std::uint8_t {
    已确认=1,精确重复,来源冲突,输入拒绝,资源失败
};
struct 接收分界答复证据 final {
    稳定编码 问题, 范围, 依据;
};
struct 接收词条准入证据 final {
    新接收认知来源键 来源;
    稳定编码 前置来源, 范围, 准入材料;
};
enum class 接收篇章成文状态 : std::uint8_t {
    形状未支持=1, 成文受阻, 回读句界不一致, 回读未唯一, 回读含义不一致, 已成文且独立回读, 待成文
};
enum class 基础表达状态 : std::uint8_t;
struct 接收篇章成文观察 final {
    稳定编码 采用;
    接收篇章成文状态 状态=接收篇章成文状态::形状未支持;
    std::optional<基础表达状态> 成文状态{};
    std::string UTF8正文;
    std::vector<接收语言句段解释> 独立回读句段;
};
struct 接收部分成文句段 final {
    std::uint64_t 原文起点, 原文终点;
    接收篇章成文状态 状态=接收篇章成文状态::形状未支持;
    std::optional<基础表达状态> 成文状态{};
    std::string UTF8正文;
    std::vector<接收语言句段解释> 独立回读;
    friend bool operator==(const 接收部分成文句段&, const 接收部分成文句段&) = default;
};
struct 接收部分成文观察 final {
    稳定编码 解释记录;
    std::vector<接收部分成文句段> 句段组;
    friend bool operator==(const 接收部分成文观察&, const 接收部分成文观察&) = default;
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
    std::optional<接收分界答复证据> 分界答复{};
    std::optional<接收词条准入证据> 词条准入{};
    std::vector<接收语言句段解释> 句段解释{};
    std::optional<新接收认知来源键> 被动解释来源{};
    std::optional<接收篇章成文观察> 篇章成文{};
    std::optional<接收部分成文观察> 部分成文{};
    std::optional<稳定编码> 局部具名特征对象{};
    std::optional<稳定编码> 局部组成子{};
};
struct 现实交互指代项 final {
    稳定编码 实例;
    std::optional<新绑定指称> 目标;
    std::optional<std::pair<std::uint64_t, std::uint64_t>> 原文范围{};
    std::optional<稳定编码> 解释依据{};
    std::optional<std::int64_t> 采用内容项{};
    bool 已确认现实目标 = false;
    std::optional<新接收指代关系值> 类型化值{};
    std::vector<新接收指代作用域段> 作用域{};
    std::string 原词面{};
};
struct 现实交互指代材料 final {
    稳定编码 现实交互场景, 接收消息, 语言场景;
    std::optional<稳定编码> 篇章采用;
    std::vector<现实交互指代项> 指代组;
    std::optional<新接收指代调用状态> 类型化读回状态{};
};
// 普通应用持有；推进/ACK仍由self唯一消费者串行调用，worker可并发只读。
// 服务与依赖必须存活到全部相关self/manager/worker最终join。
class 离线词典数据源;

class 接收需求认知服务 final : public 新前置语言输入读取接口 {
public:
    接收需求认知服务(新_外设世界树类&,新_自然语言世界树类&,
        新_语言类&,新_词类&,概念_名称关系类&,概念_存在类&,
        概念_特征类&,概念_二次特征类&,新_存在类&,
        新_特征值类&,新_特征类&,新_场景类&,新_需求类&);
    ~接收需求认知服务();
    void 装配关系陈述(const 关系陈述语言服务&) noexcept;
    bool 装配被动分界询问(接收分界询问配置);
    bool 装配可信词头来源(std::shared_ptr<const 离线词典数据源>);
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
    新前置语言输入读取结果 复读已筹办分界缺口(
        const 新前置语言输入材料&,std::stop_token) const override;
    std::optional<接收需求认知观察> 读取最近观察() const;
    // 留空的篇章采用表示读取当前解释；指定后只读该采用固定的目标。
    新语言分界调用结果<现实交互指代材料> 读取现实交互场景指代(
        const 新接收认知来源键&, const 新_世界树类&, 稳定编码 外设场景标识类型,
        std::optional<稳定编码> 预期现实交互场景, std::optional<稳定编码> 篇章采用) const;
    // ASK-READ-SCENE v0.4 §3.3 第4项：说话者取自我的交互者特征当前值；须在首次推进前由装配设置一次。
    void 设置交互者特征类型(稳定编码) noexcept;
private:
    struct 实现;
    std::unique_ptr<实现> 实现_;
};
} // namespace 海中鱼巣
