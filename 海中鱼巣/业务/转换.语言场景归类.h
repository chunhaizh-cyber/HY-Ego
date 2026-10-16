#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "解释.基础语言.h"

namespace 海中鱼巣 {

class 新_自然语言世界树类;

enum class 语言场景归类角色 : std::uint8_t { 主体 = 1, 归类概念 = 2 };

struct 语言场景归类出现 final {
    std::int64_t 局部出现编号;
    语言场景归类角色 角色;
    解释原文范围 原文范围;
    稳定编码 词条节点, 名称关系节点, 概念节点;
};

struct 语言场景归类含义 final {
    稳定编码 场景节点, 主体实例节点, 主体来源概念节点;
    稳定编码 归类概念节点, 所用知识节点;
    新语言知识作用域 作用域;
    std::string 固定原文;
    std::optional<稳定编码> 未核验来源语言记录节点;
    std::vector<语言场景归类出现> 出现组;
};

struct 语言场景归类候选 final {
    稳定编码 主体概念节点, 归类概念节点, 所用知识节点;
    std::vector<语言场景归类出现> 出现组;
};

struct 语言场景归类输入请求 final {
    稳定编码 语言节点;
    std::string 固定原文;
    std::optional<稳定编码> 未核验来源语言记录节点;
    std::size_t 最大知识条目数;
};

enum class 语言场景归类输入状态 : std::uint8_t {
    已形成且读回, 输入拒绝, 格式失败, 结构未支持, 未知词, 缺名称,
    角色不相容, 缺规则, 概念多义待解, 多规则待解, 额度不足,
    场景建立未完成, 实例建立未完成, 写后读回未完成,
    引用不可读, 读取漂移, 内部不一致, 工程未完成
};

struct 语言场景归类输入结果 final {
    语言场景归类输入状态 状态 = 语言场景归类输入状态::输入拒绝;
    std::vector<语言场景归类候选> 完整候选组;
    std::optional<稳定编码> 已取得场景节点, 已取得实例节点;
    std::optional<语言场景归类含义> 含义;
};

语言场景归类输入结果 转换单句归类到空白语言场景(
    const 语言场景归类输入请求&, 新_自然语言世界树类&,
    const 新_语言类&, const 新_词类&, const 概念_名称关系类&,
    const 概念_存在类&, const 概念_特征类&);

enum class 语言场景归类输出状态 : std::uint8_t {
    已生成且唯一回读, 输入拒绝, 缺名称, 名称待选, 缺规则,
    回读多义, 回读含义不一致, 引用不可读, 读取漂移,
    额度不足, 内部不一致, 工程未完成
};

struct 语言场景归类输出请求 final {
    稳定编码 语言节点;
    语言场景归类含义 含义;
    std::size_t 最大知识条目数;
};

struct 语言场景归类输出结果 final {
    语言场景归类输出状态 状态 = 语言场景归类输出状态::输入拒绝;
    std::string 文本;
    std::vector<语言场景归类出现> 输出对齐组;
    std::optional<稳定编码> 所用知识节点;
};

语言场景归类输出结果 从语言场景归类含义生成可回读短句(
    const 语言场景归类输出请求&, const 新_自然语言世界树类&,
    const 新_语言类&, const 新_词类&, const 概念_名称关系类&,
    const 概念_存在类&, const 概念_特征类&);

} // namespace 海中鱼巣
