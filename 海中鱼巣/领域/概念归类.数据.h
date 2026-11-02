#pragma once
#include "../核心/基础数据集.h"
#include <optional>
#include <vector>
namespace 海中鱼巣 {
enum class 归类概念读取状态 : std::uint8_t { 已读取=1, 非本提供者, 前提已变化, 依赖未就绪, 结构不一致, 资源失败 };
struct 归类概念身份信息 final {
    稳定编码 节点, 所属根;
    std::optional<稳定编码> 直接上位, 目标词条, 归类需求, 回答消息;
    bool 定义未知=false;
    std::vector<稳定编码> 名称关系;
};
struct 归类概念身份读取结果 final {
    归类概念读取状态 状态=归类概念读取状态::非本提供者;
    std::optional<归类概念身份信息> 信息;
    std::vector<基础联合节点快照> 完整保护;
};
struct 归类概念建立请求 final {
    稳定编码 目标词条, 上位概念, 归类需求, 回答消息;
    std::vector<基础联合节点快照> 来源保护;
};
struct 归类概念建立结果 final {
    归类概念读取状态 状态=归类概念读取状态::非本提供者;
    std::optional<稳定编码> 概念节点;
    bool 已复用=false;
};
} // namespace 海中鱼巣
