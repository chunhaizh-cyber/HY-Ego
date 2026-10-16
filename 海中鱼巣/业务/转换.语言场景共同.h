#pragma once

#include "转换.语言场景归类.h"
#include "../领域/新_词类.h"
#include "../领域/概念_名称关系类.h"
#include "../领域/概念_存在类.h"
#include "../领域/概念_特征类.h"

namespace 海中鱼巣::语言场景转换内部 {
enum class 读取状态 : std::uint8_t {
    已完成, 输入拒绝, 格式失败, 未知词, 缺名称, 缺规则, 额度不足,
    引用不可读, 读取漂移, 内部不一致, 工程未完成
};
struct 原文映射 final { std::string 原文; std::vector<std::size_t> 标量字节边界; };
struct 普通名称材料 final {
    新词条信息 词条;
    概念名称关系信息 名称关系;
    std::variant<存在概念信息, 特征概念信息> 目标概念;
};
struct 名称读取结果 final { 读取状态 状态; std::vector<普通名称材料> 完整普通名称材料组; };
struct 知识读取结果 final { 读取状态 状态; std::vector<新语言知识信息> 完整知识组; };
struct 命名来源项 final {
    解释原文范围 范围;
    稳定编码 词条节点, 名称关系节点, 概念节点;
};
std::optional<原文映射> 解码语言场景原文(const std::string&);
std::string 读取语言场景原文范围(const 原文映射&, 解释原文范围);
名称读取结果 读取词条普通名称完整(稳定编码, 稳定编码,
    const 新_词类&, const 概念_名称关系类&, const 概念_存在类&, const 概念_特征类&);
名称读取结果 读取概念当前名称完整(稳定编码, 稳定编码,
    const 新_词类&, const 概念_名称关系类&, const 概念_存在类&, const 概念_特征类&);
知识读取结果 读取语言场景基础知识完整(稳定编码, std::size_t,
    const 新_语言类&, const 新_词类&, const 概念_存在类&);
读取状态 核验语言场景命名来源(const 原文映射&, const std::vector<命名来源项>&,
    稳定编码, const 新_词类&, const 概念_名称关系类&);
bool 是S01知识(const 新语言知识信息&, 稳定编码);
struct S01只读结果 final {
    语言场景归类输入状态 状态;
    std::vector<语言场景归类候选> 完整候选组;
};
S01只读结果 只读识别语言场景S01(const 语言场景归类输入请求&,
    const 新_语言类&, const 新_词类&, const 概念_名称关系类&,
    const 概念_存在类&, const 概念_特征类&);
} // namespace 海中鱼巣::语言场景转换内部
