#pragma once

#include "接收认知来源.数据.h"
#include "语言含义网.数据.h"
#include <cstdint>
#include <optional>
#include <span>
#include <stop_token>
#include <string>
#include <variant>
#include <vector>

namespace 海中鱼巣 {
struct 新前置语言输入定位 final {
    新接收认知来源键 来源;
    稳定编码 前置来源节点, 固定文本材料, 语言节点;
    稳定编码 来源人类节点, 自我存在节点, 现实根场景;
    std::string 公开宿主会话;
    std::optional<稳定编码> 正式原文范围{};
    friend bool operator==(const 新前置语言输入定位&,const 新前置语言输入定位&)=default;
};
enum class 新前置语言缺口种类 : std::uint8_t {
    词面未登记=1, 名称材料不足, 对象未唯一绑定, 语言规则不足, 解释多义, 非本入口构式, 角色原文待确认
};
struct 新前置语言缺口 final {
    新前置语言缺口种类 种类;
    std::uint64_t 起始标量=0, 结束标量=0;
    std::vector<稳定编码> 已取得相关节点;
    friend bool operator==(const 新前置语言缺口&,const 新前置语言缺口&)=default;
};
struct 新前置有效分界目标 final {
    稳定编码 范围, 特征类型, 特征;
    friend bool operator==(const 新前置有效分界目标&,const 新前置有效分界目标&)=default;
};
enum class 新语言知识角色类别 : std::uint8_t;
// 来源范围内的构式类别依据，不表示已经建立概念或名称关系。
struct 新语言词条类型依据 final {
    std::uint64_t 起始标量, 结束标量;
    稳定编码 词条;
    新语言知识角色类别 所需类别;
    std::vector<稳定编码> 构式依据组;
    friend bool operator==(const 新语言词条类型依据&,const 新语言词条类型依据&)=default;
};
enum class 新语言知识含义操作 : std::uint8_t;
enum class 新语言知识参数角色 : std::uint8_t;

// 本次语境从正式场景声明及固定原文取得的称名。
struct 语言场景待完善概念词用 final {
    稳定编码 概念节点, 词条, 构式;
    std::int64_t 角色编号;
    friend bool operator==(const 语言场景待完善概念词用&,
        const 语言场景待完善概念词用&) = default;
};

struct 语言场景局部称名 final {
    稳定编码 存在节点;
    std::string 词面;
};
struct 语言图引用值 final {
    新语言知识角色类别 类别=static_cast<新语言知识角色类别>(1);
    稳定编码 节点{};
    std::optional<稳定编码> 来源概念,选用名称关系;
    friend bool operator==(const 语言图引用值&,const 语言图引用值&)=default;
};
struct 语言图整数值 final {
    std::string 规范十进制;
    std::optional<稳定编码> 单位;
    friend bool operator==(const 语言图整数值&,const 语言图整数值&)=default;
};
struct 接收语言已解释角色 final {
    std::int64_t 角色编号;
    新语言知识参数角色 参数;
    std::uint64_t 起始标量, 结束标量;
    std::optional<std::variant<语言图引用值, 语言图整数值>> 值;
    friend bool operator==(const 接收语言已解释角色&,const 接收语言已解释角色&)=default;
};
struct 接收语言静态结构 final {
    稳定编码 所用构式;
    新语言知识含义操作 操作;
    std::vector<接收语言已解释角色> 角色组;
    friend bool operator==(const 接收语言静态结构&,const 接收语言静态结构&)=default;
};

enum class 语言场景基础输入状态 : std::uint8_t;
enum class 基础语言输入状态 : std::uint8_t;
enum class 语言基础统一构式 : std::uint8_t;
struct 接收语言主体来源 final {
    std::optional<稳定编码> 词条, 名称关系, 已绑定存在;
    std::vector<稳定编码> 已证冲突候选{};
    friend bool operator==(const 接收语言主体来源&,const 接收语言主体来源&)=default;
};
// 在同一来源场景内用首次称名范围标识对象。
struct 接收语言主体关系 final {
    std::uint64_t 起始标量, 结束标量, 对象起始标量, 对象结束标量;
    稳定编码 所用构式;
    std::optional<稳定编码> 指代概念;
    bool 已确定;
    std::optional<接收语言主体来源> 来源{};
    friend bool operator==(const 接收语言主体关系&,const 接收语言主体关系&)=default;
};
struct 接收语言句段解释 final {
    std::uint64_t 起始标量, 结束标量;
    语言场景基础输入状态 状态;
    std::optional<基础语言输入状态> 图状态;
    std::vector<新前置语言缺口> 缺口组;
    std::optional<语言基础统一构式> 构式{};
    std::optional<接收语言主体关系> 主体关系{};
    std::vector<新语言词条类型依据> 词条类型依据组{};
    std::optional<接收语言静态结构> 静态结构{};
    friend bool operator==(const 接收语言句段解释&,const 接收语言句段解释&)=default;
};

struct 新接收语言解释内容 final {
    稳定编码 来源场景, 前置来源, 语言节点;
    std::uint64_t 原文起点, 原文终点;
    std::vector<接收语言句段解释> 句段组;
    std::optional<新语言含义网> 含义候选{};
    friend bool operator==(const 新接收语言解释内容&,const 新接收语言解释内容&)=default;
};
struct 新接收篇章指代目标 final {
    std::int64_t 内容项;
    稳定编码 实例;
    新绑定指称 采用时目标;
};
struct 新接收篇章采用信息 final {
    稳定编码 节点, 说话者;
    新接收语言解释内容 内容;
    std::vector<新接收篇章指代目标> 指代目标组{};
};
struct 新接收语言解释信息 final {
    稳定编码 记录节点;
    新接收语言解释内容 内容;
    std::optional<新接收篇章采用信息> 篇章采用{};
};
struct 新接收指代实例信息 final {
    稳定编码 节点, 来源场景, 前置来源, 解释定义, 消解结果特征, 特征类型;
    接收语言主体关系 出现;
    std::optional<稳定编码> 当前目标;
    bool 当前适用 = false;
    std::optional<稳定编码> 当前依据;
};
struct 新前置普通词条名称缺口 final {
    稳定编码 词条, 所用知识;
    新语言知识角色类别 所需类别;
    std::uint64_t 起始标量, 结束标量;
    friend bool operator==(const 新前置普通词条名称缺口&,const 新前置普通词条名称缺口&)=default;
};
struct 新前置语言输入材料 final {
    新前置语言输入定位 定位;
    std::vector<新前置语言缺口> 缺口组;
    bool 可重入原认知=false;
    std::optional<新前置有效分界目标> 有效分界缺口{};
    std::optional<稳定编码> 正式词条准入{};
    std::vector<新前置普通词条名称缺口> 普通词条名称缺口组{};
    friend bool operator==(const 新前置语言输入材料&,const 新前置语言输入材料&)=default;
};
enum class 新前置语言读取状态 : std::uint8_t {
    已读取=1, 来源停用, 输入不适用, 前提已变化, 停止取消, 资源失败
};
struct 新前置语言输入读取结果 final {
    新前置语言读取状态 状态=新前置语言读取状态::输入不适用;
    std::optional<新前置语言输入材料> 材料;
};
class 新前置语言输入读取接口 {
public:
    virtual ~新前置语言输入读取接口()=default;
    virtual 新前置语言输入读取结果 读取前置语言输入材料(
        const 新前置语言输入定位&,std::stop_token) const=0;
    virtual 新前置语言输入读取结果 复读已筹办分界缺口(
        const 新前置语言输入材料&,std::stop_token) const=0;
};
}
