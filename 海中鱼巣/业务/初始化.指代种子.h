#pragma once

#include "解释.初始化最小结构.h"
#include "../领域/概念_名称关系类.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <optional>
#include <string>

namespace 海中鱼巣 {

struct 初始化指代种子请求 final {
    std::string 标准文本;
    稳定编码 语言节点, 规范化合同节点;
    初始化最小资源额度 解释额度{64 * 1024, 16384, 64, 18, 256, 64, 256};
};
enum class 初始化指代种子状态 : std::uint8_t {
    已完成 = 1, 文本未完成, 首批范围不符, 前置材料不符,
    已发布材料缺失, 已发布材料不符
};
enum class 初始化指代种子工程控制 : std::uint8_t {
    已完成 = 1, 提供者未完成, 异常中止
};
enum class 初始化指代种子阶段 : std::uint8_t {
    完整解释 = 1, 首批核验, 前置读取, 定义结构, 名称结构,
    来源处理, 定义处理, 词条处理, 名称处理, 逐项完整读回,
    全部复核, 完成
};
struct 初始化指代调用事实 final {
    bool 已调用写入口 = false, 写入口已返回 = false;
    std::optional<稳定编码> 提供者返回身份, 后续确认当前身份;
    bool 调用后事实未能确认 = false;
    std::exception_ptr 确认读取异常;
};
struct 初始化指代种子项 final {
    std::optional<std::size_t> 声明局部编号;
    std::optional<初始化源范围> 词面来源;
    std::optional<指代概念定义> 定稿;
    初始化指代调用事实 定义事实, 词条事实, 名称事实;
    std::optional<指代概念写入状态> 定义写入状态;
    std::optional<指代概念发布阶段> 定义发布阶段;
    std::optional<新词条建立状态> 词条写入状态;
    std::optional<指代名称变更状态> 名称写入状态;
    std::optional<指代名称本次处理> 名称本次处理;
    std::optional<指代名称核验阶段> 名称核验阶段;
    std::optional<指代概念写入结果> 定义发布;
    std::optional<指代概念查询结果> 结构前定义查询, 定义查询, 定义读回;
    std::optional<新词条建立结果> 词条发布;
    std::optional<新词条查询结果> 词条查询, 词条读回;
    std::optional<指代名称变更结果> 名称发布;
    std::optional<指代名称读取结果> 关系读回;
    std::optional<指代名称列表读取结果> 概念名称读回, 词条候选读回;
    bool 逐项核验完成 = false, 全部复核完成 = false;
};
struct 初始化指代种子结果 final {
    初始化指代种子工程控制 工程控制 = 初始化指代种子工程控制::已完成;
    std::optional<初始化指代种子状态> 状态;
    初始化指代种子阶段 阶段 = 初始化指代种子阶段::完整解释;
    std::optional<std::size_t> 当前项;
    std::optional<初始化最小解释结果> 解释结果;
    std::optional<新语言查询结果> 语言读回;
    std::optional<指代定义结构状态> 定义结构处理;
    std::optional<指代名称读取状态> 名称结构处理;
    初始化指代调用事实 来源事实;
    std::optional<新语言知识发布种类> 来源发布种类;
    std::optional<新语言知识来源结果> 来源发布, 来源读回;
    std::array<初始化指代种子项, 6> 项组;
    std::exception_ptr 工程异常;
    std::string 诊断说明;
};

初始化指代种子结果 初始化指代概念名称(
    const 初始化指代种子请求&, 新_语言类&, 新_词类&,
    概念_存在类&, 概念_名称关系类&);
初始化指代种子结果 读取初始化指代概念名称(
    const 初始化指代种子请求&, const 新_语言类&, const 新_词类&,
    const 概念_存在类&, const 概念_名称关系类&);

} // namespace 海中鱼巣
