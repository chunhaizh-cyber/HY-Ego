#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "../领域/新_语言类.h"
#include "../领域/新_词类.h"
#include "../领域/概念_名称关系类.h"
#include "../领域/概念_存在类.h"
#include "../领域/概念_特征类.h"
#include "表达.基础语言.h"

namespace 海中鱼巣 {

class 新_世界树类;
class 新_场景类;
class 新_存在类;

// 世界根只限定解释的检索范围，不是词条或概念名称。
struct 解释目标世界根 final { 稳定编码 场景根; };
struct 解释原文范围 final { std::size_t 起始标量, 结束标量; };
struct 解释已绑定主体 final {
    解释原文范围 原文范围;
    稳定编码 存在节点;
};
struct 基础语言输入请求 final {
    std::string 固定原文;
    稳定编码 语言节点;
    std::optional<稳定编码> 未核验来源语言记录节点;
    std::optional<解释目标世界根> 显式所述世界根;
    std::optional<解释已绑定主体> 主体绑定;
    std::size_t 最大原文标量数, 最大知识条目数, 最大名称候选数;
};
enum class 基础语言输入状态 : std::uint8_t {
    已形成唯一含义, 主体待绑定, 概念多义待解, 多规则待解, 未知词,
    缺名称, 缺规则, 角色不相容, 结构未支持, 世界能力未支持,
    世界根不可读, 主体不属于世界, 输入拒绝, 格式失败,
    引用不可读, 额度不足, 内部不一致, 工程未完成
};
struct 基础语言归类候选 final {
    稳定编码 归类概念节点, 名称关系节点, 所用知识节点;
    解释原文范围 主体范围, 概念范围;
    std::optional<稳定编码> 主体存在节点;
};
struct 普通名称候选读回 final {
    概念名称关系信息 名称关系;
    std::variant<存在概念信息, 特征概念信息> 目标概念;
};
using 语言名称候选读回 = std::variant<普通名称候选读回, 指代名称关系信息>;
struct 语言候选路由请求 final {
    std::string 固定原文;
    std::optional<稳定编码> 未核验来源语言记录节点;
    解释目标世界根 所述世界根;
    解释原文范围 全句范围;
    解释原文范围 主体范围, 概念范围;
    std::optional<解释已绑定主体> 主体绑定;
    新词条信息 概念词条;
    新语言知识信息 所用知识;
    std::vector<语言名称候选读回> 未过滤名称候选组;
    std::size_t 最大名称候选数;
};
struct 语言候选路由结果 final {
    基础语言输入状态 状态;
    语言候选路由请求 回显请求;
    std::vector<语言名称候选读回> 未过滤名称候选组;
    std::vector<基础语言归类候选> 相容候选组;
};
struct 基础语言输入结果 final {
    基础语言输入状态 状态;
    std::string 固定原文;
    std::optional<稳定编码> 未核验来源语言记录节点;
    std::optional<解释目标世界根> 实际所述世界根;
    std::vector<语言名称候选读回> 未过滤名称候选组;
    std::vector<基础语言归类候选> 候选组;
    std::vector<解释原文范围> 未覆盖范围组;
};
class 语言候选路由业务 final {
public:
    语言候选路由结果 形成候选(
        const 语言候选路由请求&, const 新_世界树类&,
        const 新_场景类&,
        const 新_存在类&, const 新_语言类&, const 新_词类&,
        const 概念_名称关系类&, const 概念_存在类&,
        const 概念_特征类&) const;
};
基础语言输入结果 解释单句基础归类(
    const 基础语言输入请求&, const 新_世界树类&,
    const 新_场景类&, const 新_存在类&, const 新_语言类&,
    const 新_词类&, const 概念_名称关系类&,
    const 概念_存在类&, const 概念_特征类&);

// S02 只形成有名称来源的概念上下位陈述提议，不发表概念关系。
struct 基础概念上下位输入请求 final {
    std::string 固定原文;
    稳定编码 语言节点;
    std::optional<稳定编码> 未核验来源语言记录节点;
    std::size_t 最大原文标量数, 最大知识条目数, 最大名称候选数;
    std::size_t 最大组合候选数;
};
enum class 基础概念上下位输入状态 : std::uint8_t {
    已形成唯一含义, 概念多义待解, 多规则待解, 未知词, 缺名称,
    缺规则, 角色不相容, 结构未支持, 能力未支持, 输入拒绝, 格式失败,
    引用不可读, 额度不足, 内部不一致, 工程未完成
};
struct 基础概念上下位候选 final {
    新语言知识角色类别 类别;
    稳定编码 主体概念节点, 归类概念节点;
    稳定编码 主体名称关系节点, 归类名称关系节点, 所用知识节点;
    解释原文范围 主体范围, 归类范围;
};
struct 基础概念上下位输入结果 final {
    基础概念上下位输入状态 状态;
    std::string 固定原文;
    std::optional<稳定编码> 未核验来源语言记录节点;
    std::vector<基础概念上下位候选> 候选组;
    std::vector<解释原文范围> 未覆盖范围组;
};
基础概念上下位输入结果 解释单句概念上下位(
    const 基础概念上下位输入请求&, const 新_语言类&,
    const 新_词类&, const 概念_名称关系类&,
    const 概念_存在类&, const 概念_特征类&);

struct 基础概念上下位可逆表达请求 final {
    基础表达请求 表达;
    std::size_t 最大回读原文标量数, 最大回读知识条目数;
    std::size_t 最大回读名称候选数, 最大回读组合候选数;
};
enum class 基础概念上下位可逆表达状态 : std::uint8_t {
    已生成且可唯一回读, 输入拒绝, 表达未生成,
    回读语义未闭合, 回读技术未完成, 含义不一致
};
struct 基础概念上下位可逆表达结果 final {
    基础概念上下位可逆表达状态 状态;
    std::optional<基础表达状态> 原始表达状态;
    std::optional<基础概念上下位输入状态> 回读状态;
    std::optional<基础表达结果> 可用表达;
};
基础概念上下位可逆表达结果 生成可唯一回读的概念上下位表达(
    const 基础概念上下位可逆表达请求&, const 新_语言类&,
    const 新_词类&, const 概念_名称关系类&,
    const 概念_存在类&, const 概念_特征类&,
    const 新_存在类&);

// S09：外层确认询问与 S02 被询归类分别保留知识身份和范围。
struct 基础概念归类确认输入请求 final {
    std::string 固定原文;
    稳定编码 语言节点;
    std::optional<稳定编码> 未核验来源语言记录节点;
    std::optional<稳定编码> 受询者存在节点;
    std::size_t 最大原文标量数, 最大知识条目数;
    std::size_t 最大名称候选数, 最大组合候选数, 最大确认候选数;
};
enum class 基础概念归类确认输入状态 : std::uint8_t {
    已形成唯一信息请求, 受询者待绑定, 受询者不可读,
    概念多义待解, 多规则待解, 未知词, 缺名称, 缺规则,
    角色不相容, 结构未支持, 能力未支持, 输入拒绝, 格式失败,
    引用不可读, 额度不足, 内部不一致, 工程未完成
};
struct 基础概念归类确认候选 final {
    稳定编码 外层知识节点, 子知识节点;
    基础概念上下位候选 被询归类;
    std::optional<稳定编码> 受询者存在节点;
    解释原文范围 子句范围, 确认词范围;
};
struct 基础概念归类确认输入结果 final {
    基础概念归类确认输入状态 状态;
    std::string 固定原文;
    std::optional<稳定编码> 未核验来源语言记录节点;
    std::vector<基础概念归类确认候选> 候选组;
    std::vector<解释原文范围> 未覆盖范围组;
};
基础概念归类确认输入结果 解释单句概念归类确认(
    const 基础概念归类确认输入请求&, const 新_语言类&,
    const 新_词类&, const 概念_名称关系类&,
    const 概念_存在类&, const 概念_特征类&,
    const 新_存在类&);

struct 基础概念归类确认可逆表达请求 final {
    基础表达请求 表达;
    std::size_t 最大回读原文标量数, 最大回读知识条目数;
    std::size_t 最大回读名称候选数, 最大回读组合候选数;
    std::size_t 最大回读确认候选数;
};
enum class 基础概念归类确认可逆表达状态 : std::uint8_t {
    已生成且可唯一回读, 输入拒绝, 表达未生成,
    回读语义未闭合, 回读技术未完成, 含义不一致
};
struct 基础概念归类确认可逆表达结果 final {
    基础概念归类确认可逆表达状态 状态;
    std::optional<基础表达状态> 原始表达状态;
    std::optional<基础概念归类确认输入状态> 回读状态;
    std::optional<基础表达结果> 可用表达;
};
基础概念归类确认可逆表达结果 生成可唯一回读的概念归类确认表达(
    const 基础概念归类确认可逆表达请求&, const 新_语言类&,
    const 新_词类&, const 概念_名称关系类&,
    const 概念_存在类&, const 概念_特征类&,
    const 新_存在类&);

} // namespace 海中鱼巣
