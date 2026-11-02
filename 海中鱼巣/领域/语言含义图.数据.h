#pragma once

#include "新_语言类.h"
#include "信息内容.数据.h"
#include "新_词类.h"
#include "概念_名称关系类.h"
#include "概念_特征类.h"
#include "接收认知来源.数据.h"
#include <utility>
#include <variant>

namespace 海中鱼巣 {

struct 新语言指代配置 final {
    稳定编码 状态类型, 已消解值域, 比较关系;
};
struct 新语言指代请求 final {
    稳定编码 自然语言场景, 来源消息, 原文材料, 作用域, 代词概念;
    std::uint64_t 原文起点=0, 原文终点=0;
    std::optional<稳定编码> 存在目标;
};
struct 新语言指代信息 final {
    稳定编码 实例, 状态特征;
    新语言指代请求 来源;
    std::optional<稳定编码> 当前存在目标;
};
struct 新语言指代结果 final {
    bool 已读回=false, 已消解=false;
    std::optional<新语言指代信息> 信息;
    std::vector<基础联合节点快照> 完整保护;
};

struct 语言图引用值 final {
    新语言知识角色类别 类别=新语言知识角色类别::存在;
    稳定编码 节点{};
    std::optional<稳定编码> 来源概念,选用名称关系;
    friend bool operator==(const 语言图引用值&,const 语言图引用值&)=default;
};
struct 语言图整数值 final {
    std::string 规范十进制;
    std::optional<稳定编码> 单位;
    friend bool operator==(const 语言图整数值&,const 语言图整数值&)=default;
};
struct 语言图子出现引用 final {
    std::int64_t 出现编号=0;
    friend bool operator==(const 语言图子出现引用&,const 语言图子出现引用&)=default;
};
struct 语言图待求值 final {
    std::int64_t 局部变量编号=0;
    稳定编码 内容范围概念{};
    friend bool operator==(const 语言图待求值&,const 语言图待求值&)=default;
};
struct 语言图材料值 final {
    std::string 原文;
    friend bool operator==(const 语言图材料值&,const 语言图材料值&)=default;
};
using 语言图绑定值=std::variant<语言图引用值,语言图整数值,语言图子出现引用,语言图待求值,语言图材料值>;
struct 语言图角色绑定 final {
    std::int64_t 角色编号=0;
    语言图绑定值 值;
    friend bool operator==(const 语言图角色绑定&,const 语言图角色绑定&)=default;
};
struct 语言图含义出现 final {
    std::int64_t 出现编号=0;
    稳定编码 所用知识节点{};
    std::int64_t 定义含义编号=0;
    新语言知识作用域 有效作用域=新语言知识作用域::普通陈述;
    std::vector<std::int64_t> 结构项路径;
    std::vector<语言图角色绑定> 角色组;
    std::optional<std::pair<std::size_t,std::size_t>> 原文范围;
    friend bool operator==(const 语言图含义出现&,const 语言图含义出现&)=default;
};
struct 语言图含义材料 final {
    稳定编码 语言节点{};
    新语言知识含义图 定义图;
    std::int64_t 根出现编号=0;
    std::vector<语言图含义出现> 出现组;
    std::optional<std::string> 固定原文;
    std::optional<稳定编码> 未核验来源语言记录节点;
    std::optional<正式信息参数投影> 正式需求来源;
    std::optional<正式内容引用定位> 被询正式内容;
    std::optional<稳定编码> 正式指称作用域;
};
struct 语言图角色原文定位 final {
    std::int64_t 出现编号=0, 角色编号=0;
    std::uint64_t 原文起点=0, 原文终点=0;
};
struct 语言特征概念根链项 final {
    特征概念定义保护信息 信息;
    稳定编码 直接上位概念节点;
};
struct 语言特征概念读取依据 final {
    稳定编码 概念节点;
    特征概念定义保护信息 信息;
    std::vector<基础联合节点快照> 完整保护;
    稳定编码 分类根节点{};
    std::vector<语言特征概念根链项> 根链组{};
};
struct 语言词面查询依据 final {
    稳定编码 语言节点, 规范化合同节点;
    std::string 规范化词面;
    std::optional<新词条信息> 命中词条;
};
struct 语言名称候选读取依据 final {
    稳定编码 词条节点;
    std::vector<概念名称候选正式信息> 完整候选组;
};
enum class 新语言含义采用状态 : std::uint8_t {
    已采用=1, 精确重复, 未找到, 输入拒绝, 来源冲突, 前提已变化, 资源失败
};
struct 新语言含义采用请求 final {
    新接收认知来源键 来源;
    稳定编码 场景节点, 说话者节点, 现实范围根;
    稳定编码 原文材料节点;
    语言图含义材料 含义;
    std::vector<语言图角色原文定位> 角色原文定位组;
};
struct 新语言含义采用信息 final {
    稳定编码 节点;
    新接收认知来源键 来源;
    稳定编码 场景节点, 说话者节点, 现实范围根, 原文材料节点;
    语言图含义材料 含义;
    std::vector<语言图角色原文定位> 角色原文定位组;
};
struct 新语言含义采用结果 final {
    新语言含义采用状态 状态 = 新语言含义采用状态::输入拒绝;
    std::optional<新语言含义采用信息> 信息;
    std::vector<基础联合节点快照> 完整保护;
};
struct 新语言需求目标投影 final {
    稳定编码 采用含义节点, 需求主体, 目标存在, 目标特征类型;
    std::optional<std::string> 准确整数, 区间下界, 区间上界;
    std::optional<新语言知识特征方向> 方向;
    std::optional<稳定编码> 单位;
    新接收认知来源键 需求来源{};
    // 仅保存请求原文，不含可执行动作或猜测的目标存在/特征。
    bool 未解请求目标原文=false;
};
struct 新语言需求目标投影结果 final {
    新语言含义采用状态 状态 = 新语言含义采用状态::输入拒绝;
    std::optional<新语言需求目标投影> 信息;
    std::vector<基础联合节点快照> 完整保护;
};
struct 新语言行动请求投影 final {
    稳定编码 采用含义节点, 说话者H, 受请求者E;
    新接收认知来源键 请求来源, 需求来源;
    新语言需求目标投影 期望需求;
};
struct 新语言行动请求投影结果 final {
    新语言含义采用状态 状态 = 新语言含义采用状态::输入拒绝;
    std::optional<新语言行动请求投影> 信息;
    std::vector<基础联合节点快照> 完整保护;
};
struct 新语言采用来源保护 final {
    新语言含义采用状态 状态 = 新语言含义采用状态::输入拒绝;
    新接收认知来源键 来源;
    稳定编码 场景节点, 说话者节点, 现实范围根, 原文材料节点;
    std::string 公开宿主会话, 对话框标识, 固定原文;
    std::uint64_t 上行来源序号=0;
    语言图含义材料 唯一解释;
    std::vector<语言图角色原文定位> 角色原文定位组;
    std::vector<语言特征概念读取依据> 特征概念依据组;
    std::vector<基础联合节点快照> 完整保护;
    std::optional<稳定编码> 受话者节点{};
};
class 新语言采用来源读取接口 {
public:
    virtual ~新语言采用来源读取接口()=default;
    virtual 新语言采用来源保护 读取采用来源保护(
        const 新接收认知来源键&, 稳定编码 已建立来源场景) const=0;
};

} // namespace 海中鱼巣
