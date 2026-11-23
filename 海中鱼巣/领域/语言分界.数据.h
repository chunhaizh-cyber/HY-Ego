#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include "../核心/基础数据集.h"
#include "接收认知来源.数据.h"
#include "消息交互.数据.h"

namespace 海中鱼巣 {

struct 新语言前置来源请求 final { 新接收认知来源键 来源; };
struct 新语言前置来源信息 final {
    稳定编码 节点, 固定文本材料;
    新接收认知来源键 来源;
    std::string 固定UTF8原文;
};
enum class 新语言分界工程状态 : std::uint8_t {
    已完成=1, 未找到, 未初始化, 输入拒绝, 未支持, 前提已变化, 资源失败, 材料未就绪
};
template<class T> struct 新语言分界调用结果 final {
    新语言分界工程状态 状态=新语言分界工程状态::输入拒绝;
    std::optional<T> 信息;
    std::optional<稳定编码> 本次已发表身份;
};
struct 新分界消息读取结果 final {
    新语言分界工程状态 状态=新语言分界工程状态::输入拒绝;
    std::optional<新外设消息信息> 消息;
    基础只读预期 发布前提;
};
class 新语言分界消息读取接口 {
public:
    virtual ~新语言分界消息读取接口()=default;
    virtual 新分界消息读取结果 读取分界消息正文(稳定编码) const=0;
};

enum class 新语言范围特征种类 : std::uint8_t {
    分词候选材料, 分界依据, 有效分界结果, 词条准入结果
};
struct 新语言分界特征配置 final {
    稳定编码 信息身份范围;
    std::array<稳定编码, 4> 特征类型组;
    稳定编码 参数结构范围{};
    friend bool operator==(const 新语言分界特征配置&,
        const 新语言分界特征配置&) = default;
};
struct 新语言材料范围请求 final {
    稳定编码 前置来源节点;
    std::uint64_t 起始标量 = 0, 结束标量 = 0;
    稳定编码 语言节点, 规范化合同节点, 所属语言场景;
    friend bool operator==(const 新语言材料范围请求&,
        const 新语言材料范围请求&) = default;
};
struct 新语言材料范围信息 final {
    稳定编码 节点;
    新语言材料范围请求 定位;
    std::array<稳定编码, 4> 特征节点组;
    friend bool operator==(const 新语言材料范围信息&,
        const 新语言材料范围信息&) = default;
};

struct 新语言范围特征信息 final {
    新语言材料范围信息 范围;
    std::array<稳定编码, 4> 特征类型组;
    基础联合节点快照 范围快照;
};


struct 新语言范围原文信息 final {
    新语言材料范围信息 范围;
    稳定编码 固定正文材料;
    std::string 根UTF8原文, 范围UTF8原文;
};

struct 新语言分界词段 final {
    std::uint64_t 起始标量 = 0, 结束标量 = 0;
    std::optional<稳定编码> 词条;
    friend bool operator==(const 新语言分界词段&, const 新语言分界词段&) = default;
};
struct 新语言分界候选路径 final {
    std::vector<新语言分界词段> 词段组;
    std::vector<稳定编码> 规则应用记录组;
    friend bool operator==(const 新语言分界候选路径&, const 新语言分界候选路径&) = default;
};
struct 新语言分界候选定义 final {
    std::vector<新语言分界候选路径> 路径组;
    friend bool operator==(const 新语言分界候选定义&, const 新语言分界候选定义&) = default;
};
struct 新语言规则分界来源 final {
    稳定编码 正式匹配记录;
    friend bool operator==(const 新语言规则分界来源&, const 新语言规则分界来源&) = default;
};
struct 新语言回答分界来源 final {
    稳定编码 正式问题, 正式回答, 分段对齐材料;
    friend bool operator==(const 新语言回答分界来源&, const 新语言回答分界来源&) = default;
};
using 新语言分界取材来源 = std::variant<新语言规则分界来源, 新语言回答分界来源>;
struct 新语言正式分界取材 final {
    std::vector<新语言分界词段> 已证明词段;
    新语言分界取材来源 来源;
    friend bool operator==(const 新语言正式分界取材&, const 新语言正式分界取材&) = default;
};

class 新语言正式分界取材接口 {
public:
    virtual ~新语言正式分界取材接口() = default;
    virtual 新语言分界调用结果<新语言正式分界取材> 读取已确认分界(
        稳定编码 范围, 稳定编码 候选) const = 0;
};

struct 新语言分界依据定义 final {
    稳定编码 候选材料;
    新语言正式分界取材 取材;
    friend bool operator==(const 新语言分界依据定义&, const 新语言分界依据定义&) = default;
};
struct 新语言有效分界定义 final {
    稳定编码 正式依据;
    friend bool operator==(const 新语言有效分界定义&, const 新语言有效分界定义&) = default;
};
struct 新语言词条准入定义 final {
    稳定编码 有效结果;
    std::vector<新语言分界词段> 词段组;
    friend bool operator==(const 新语言词条准入定义&, const 新语言词条准入定义&) = default;
};
struct 新语言词条准入词段材料 final {
    新语言分界词段 词段;
    std::optional<std::string> 未登记原文词面;
};
struct 新语言词条准入取材 final {
    稳定编码 语言节点, 规范化合同节点;
    std::vector<新语言词条准入词段材料> 词段材料组;
};
using 新语言分界材料定义 = std::variant<新语言分界候选定义, 新语言分界依据定义,
    新语言有效分界定义, 新语言词条准入定义>;
struct 新语言分界材料信息 final {
    稳定编码 节点, 所属范围;
    新语言分界材料定义 定义;
};
struct 新语言分界采用请求 final {
    稳定编码 范围;
    新语言范围特征种类 种类 = 新语言范围特征种类::分词候选材料;
    稳定编码 材料, 执行;
};
struct 新语言分界采用回执 final {
    稳定编码 范围, 实际材料, 特征类型, 特征, 执行;
    std::optional<稳定编码> 新状态;
    std::optional<std::int64_t> 新状态UTC纳秒;
};
struct 新语言分界依据采用请求 final {
    稳定编码 范围, 依据;
    std::optional<稳定编码> 预期当前依据;
};
struct 新语言分界依据采用回执 final {
    稳定编码 范围, 实际依据, 特征类型, 特征;
    std::optional<稳定编码> 新状态;
    std::optional<std::int64_t> 新状态UTC纳秒;
};

} // namespace 海中鱼巣
