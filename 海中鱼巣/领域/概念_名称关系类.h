#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "../核心/基础数据集.h"

namespace 海中鱼巣 {

class 新_词类;
class 概念_特征类;
class 概念_存在类;

// 名称关系属于全局基础数据集中的自然语言组织材料，不进入世界树。
extern 稳定编码 概念名称关系根节点;

struct 概念名称关系信息 final {
    稳定编码 节点;
    稳定编码 概念节点;
    稳定编码 词条节点;
    friend bool operator==(const 概念名称关系信息&,
        const 概念名称关系信息&) = default;
};

enum class 概念名称关系建立状态 : std::uint8_t {
    已建立 = 1,
    已复用 = 2,
    输入不合法 = 3,
    依赖不存在 = 4,
    结构不一致 = 5,
    资源失败 = 6
};

enum class 概念名称关系查询状态 : std::uint8_t {
    已找到 = 1,
    未找到 = 2,
    输入不合法 = 3,
    结构不一致 = 4,
    资源失败 = 5
};

enum class 概念名称关系删除状态 : std::uint8_t {
    已删除 = 1,
    不存在 = 2,
    输入不合法 = 3,
    结构不一致 = 4,
    资源失败 = 5
};

struct 概念名称关系建立结果 final {
    概念名称关系建立状态 状态 = 概念名称关系建立状态::输入不合法;
    std::optional<稳定编码> 名称关系节点;
};

struct 概念名称关系查询结果 final {
    概念名称关系查询状态 状态 = 概念名称关系查询状态::输入不合法;
    std::optional<概念名称关系信息> 信息;
};

struct 概念名称关系列表查询结果 final {
    概念名称关系查询状态 状态 = 概念名称关系查询状态::输入不合法;
    std::vector<稳定编码> 名称关系节点组;
};

// 一条关系只绑定一个概念和一个词条；同一对端点全局唯一。
// 当前只接纳特征概念与存在概念，不把其它普通节点冒充概念。
class 概念_名称关系类 final {
public:
    概念_名称关系类(
        新_词类& 词服务,
        概念_特征类& 特征概念服务,
        概念_存在类& 存在概念服务) noexcept;

    bool 初始化() noexcept;

    概念名称关系建立结果 建立或取得名称关系(
        稳定编码 概念节点,
        稳定编码 词条节点) noexcept;
    概念名称关系删除状态 删除名称关系(
        稳定编码 名称关系节点) noexcept;

    概念名称关系查询结果 获取名称关系(
        稳定编码 名称关系节点) const noexcept;
    概念名称关系列表查询结果 查询概念名称(
        稳定编码 概念节点) const noexcept;
    概念名称关系列表查询结果 查询词条候选概念(
        稳定编码 词条节点) const noexcept;
    bool 是概念名称关系节点(稳定编码 节点) const noexcept;

private:
    新_词类& 词服务_;
    概念_特征类& 特征概念服务_;
    概念_存在类& 存在概念服务_;
};

} // namespace 海中鱼巣
