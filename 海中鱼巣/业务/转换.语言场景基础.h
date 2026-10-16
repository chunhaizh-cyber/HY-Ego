#pragma once

#include "转换.语言场景归类.h"
#include <variant>

namespace 海中鱼巣 {

enum class 语言场景基础构式 : std::uint8_t {
    S03特征概念条件 = 1, S03整数, S05组成
};
enum class 语言场景基础角色 : std::uint8_t {
    主体 = 1, 特征类型, 特征概念, 整数值, 部分, 整体
};
struct 语言场景基础命名出现 final {
    std::int64_t 局部出现编号;
    语言场景基础角色 角色;
    解释原文范围 原文范围;
    稳定编码 词条节点, 名称关系节点, 概念节点;
};
struct 语言场景基础整数出现 final {
    std::int64_t 局部出现编号;
    解释原文范围 原文范围;
    std::string 原始数字词面, 规范十进制;
};
using 语言场景基础出现 = std::variant<语言场景基础命名出现,
    语言场景基础整数出现>;
struct 语言场景基础固定词出现 final {
    std::int64_t 结构项局部编号;
    解释原文范围 原文范围;
    稳定编码 词条节点;
};
struct 语言场景基础对象绑定 final {
    std::int64_t 局部出现编号;
    稳定编码 实例节点, 来源概念节点;
};
struct 语言场景特征概念主张 final {
    std::int64_t 主体出现编号, 特征类型出现编号, 特征概念出现编号;
};
struct 语言场景整数特征主张 final {
    std::int64_t 主体出现编号, 特征类型出现编号, 整数出现编号;
};
struct 语言场景组成主张 final {
    std::int64_t 部分出现编号, 整体出现编号;
};
using 语言场景基础主张 = std::variant<语言场景特征概念主张,
    语言场景整数特征主张, 语言场景组成主张>;
struct 语言场景基础候选 final {
    语言场景基础构式 构式;
    稳定编码 所用知识节点;
    std::vector<语言场景基础出现> 出现组;
    std::vector<语言场景基础固定词出现> 固定词组;
    语言场景基础主张 主张;
};
struct 语言场景基础含义 final {
    稳定编码 场景节点;
    新语言知识作用域 作用域;
    std::string 固定原文;
    std::optional<稳定编码> 未核验来源语言记录节点;
    语言场景基础候选 解释;
    std::vector<语言场景基础对象绑定> 对象绑定组;
};
struct 语言场景基础输入请求 final {
    稳定编码 语言节点;
    std::string 固定原文;
    std::optional<稳定编码> 未核验来源语言记录节点;
    std::size_t 最大知识条目数;
};
enum class 语言场景基础输入状态 : std::uint8_t {
    已形成且读回, 输入拒绝, 格式失败, 结构未支持, 未知词, 缺名称,
    角色不相容, 缺规则, 概念多义待解, 多规则待解, 构式多义待解,
    额度不足, 场景建立未完成, 实例建立未完成, 写后读回未完成,
    引用不可读, 读取漂移, 内部不一致, 工程未完成
};
struct 语言场景基础输入结果 final {
    语言场景基础输入状态 状态 = 语言场景基础输入状态::输入拒绝;
    std::vector<语言场景基础候选> 完整候选组;
    std::optional<稳定编码> 已取得场景节点;
    std::vector<语言场景基础对象绑定> 已取得对象绑定组;
    std::optional<语言场景基础含义> 含义;
};
语言场景基础输入结果 转换单句基础结构到空白语言场景(
    const 语言场景基础输入请求&, 新_自然语言世界树类&,
    const 新_语言类&, const 新_词类&, const 概念_名称关系类&,
    const 概念_存在类&, const 概念_特征类&);
enum class 语言场景基础输出状态 : std::uint8_t {
    已生成且唯一回读, 输入拒绝, 缺名称, 名称待选, 缺规则,
    回读多义, 回读含义不一致, 引用不可读, 读取漂移,
    额度不足, 内部不一致, 工程未完成
};
struct 语言场景基础输出请求 final {
    稳定编码 语言节点;
    语言场景基础含义 含义;
    std::size_t 最大知识条目数;
};
struct 语言场景基础输出结果 final {
    语言场景基础输出状态 状态 = 语言场景基础输出状态::输入拒绝;
    std::string 文本;
    std::vector<语言场景基础出现> 输出对齐组;
    std::vector<语言场景基础固定词出现> 输出固定词组;
    std::optional<稳定编码> 所用知识节点;
};
语言场景基础输出结果 从语言场景基础含义生成可回读短句(
    const 语言场景基础输出请求&, const 新_自然语言世界树类&,
    const 新_语言类&, const 新_词类&, const 概念_名称关系类&,
    const 概念_存在类&, const 概念_特征类&);

enum class 语言基础统一构式 : std::uint8_t {
    S01归类 = 1, S02上下位, S03整数, S03特征概念条件, S05组成, S09归类确认
};
struct 语言基础统一对齐 final {
    解释原文范围 原文范围;
    std::vector<std::int64_t> 结构项路径;
    std::optional<稳定编码> 词条节点, 名称关系节点, 概念节点;
    std::optional<std::int64_t> 局部出现编号;
    std::optional<std::string> 规范十进制;
};
struct 语言基础纯概念含义 final {
    std::string 固定原文;
    std::optional<稳定编码> 未核验来源语言记录节点;
    std::variant<基础概念上下位候选, 基础概念归类确认候选> 解释;
    std::vector<语言基础统一对齐> 来源对齐组;
};
using 语言基础统一候选 = std::variant<语言场景归类候选,
    基础概念上下位候选, 语言场景基础候选, 基础概念归类确认候选>;
using 语言基础统一含义 = std::variant<语言场景归类含义,
    语言基础纯概念含义, 语言场景基础含义>;
struct 语言基础统一输入请求 final {
    稳定编码 语言节点;
    std::string 固定原文;
    std::optional<稳定编码> 未核验来源语言记录节点;
    std::size_t 最大原文标量数, 最大知识条目数, 最大名称候选数;
    std::size_t 最大组合候选数, 最大确认候选数;
};
struct 语言基础统一输入结果 final {
    语言场景基础输入状态 状态 = 语言场景基础输入状态::输入拒绝;
    std::vector<语言基础统一候选> 完整候选组;
    std::optional<语言基础统一构式> 构式;
    std::optional<稳定编码> 已取得场景节点;
    std::vector<语言场景基础对象绑定> 已取得对象绑定组;
    std::optional<语言基础统一含义> 含义;
};
语言基础统一输入结果 转换单句基础语言到信息(
    const 语言基础统一输入请求&, 新_自然语言世界树类&,
    const 新_语言类&, const 新_词类&, const 概念_名称关系类&,
    const 概念_存在类&, const 概念_特征类&, const 新_存在类&);
struct 语言基础统一输出请求 final {
    稳定编码 语言节点;
    语言基础统一含义 含义;
    std::size_t 最大知识条目数, 最大名称候选数, 最大输出标量数;
    std::size_t 最大回读原文标量数, 最大组合候选数, 最大确认候选数;
};
struct 语言基础统一输出结果 final {
    语言场景基础输出状态 状态 = 语言场景基础输出状态::输入拒绝;
    std::string 文本;
    std::optional<语言基础统一构式> 构式;
    std::vector<语言基础统一对齐> 输出对齐组;
    std::vector<稳定编码> 所用知识节点组;
};
语言基础统一输出结果 从基础信息生成可回读单句(
    const 语言基础统一输出请求&, const 新_自然语言世界树类&,
    const 新_语言类&, const 新_词类&, const 概念_名称关系类&,
    const 概念_存在类&, const 概念_特征类&, const 新_存在类&);

} // namespace 海中鱼巣
