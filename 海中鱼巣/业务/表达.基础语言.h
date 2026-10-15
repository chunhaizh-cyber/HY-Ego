#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "../领域/新_语言类.h"

namespace 海中鱼巣 {

class 新_词类;
class 概念_名称关系类;
class 概念_存在类;

enum class 基础表达参数种类 : std::uint8_t { 概念 = 1, 领域对象, 特征类型, 准确值 };
struct 基础表达参数 final {
    新语言知识参数角色 参数;
    新语言知识角色类别 类别;
    基础表达参数种类 种类;
    std::variant<稳定编码, std::int64_t> 内容;
    std::optional<稳定编码> 选用名称关系;
};
struct 基础表达请求 final {
    稳定编码 语言节点;
    新语言知识表达目的 目的;
    新语言知识含义操作 操作;
    新语言知识作用域 作用域;
    std::vector<基础表达参数> 参数组;
    std::size_t 最大知识条目数;
    std::size_t 最大名称条目数;
    std::size_t 最大输出标量数;
};
enum class 基础表达状态 : std::uint8_t {
    已生成, 输入拒绝, 缺规则, 缺名称, 名称待选, 缺指称, 多规则待选,
    能力未支持, 引用不可读, 读取漂移, 额度不足, 工程未完成, 内部不一致
};
struct 基础表达对齐项 final {
    std::size_t 起始标量, 结束标量;
    std::int64_t 结构项编号;
    std::optional<新语言知识参数角色> 参数;
    std::optional<稳定编码> 词条节点;
    std::optional<稳定编码> 名称关系节点;
};
struct 基础表达结果 final {
    基础表达状态 状态 = 基础表达状态::输入拒绝;
    std::string 文本;
    std::optional<稳定编码> 知识节点;
    std::vector<基础表达参数> 已用绑定;
    std::vector<基础表达对齐项> 对齐组;
};

基础表达结果 生成基础语言表达(
    const 基础表达请求& 请求,
    const 新_语言类& 语言服务, const 新_词类& 词服务,
    const 概念_名称关系类& 名称服务,
    const 概念_存在类& 存在概念服务);

} // namespace 海中鱼巣
