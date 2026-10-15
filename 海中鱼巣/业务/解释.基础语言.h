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

} // namespace 海中鱼巣
