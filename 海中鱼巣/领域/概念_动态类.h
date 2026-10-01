#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "新_状态类.h"
#include "新_特征值类.h"
#include "概念_特征类.h"
#include "概念_二次特征类.h"

namespace 海中鱼巣 {

enum class 动态概念种类 : std::uint8_t {
    实例 = 1,
    聚合 = 2
};

// 前、后值域由特征概念节点承载，避免在动态概念中复制第二份值域事实。
struct 动态概念状态项 final {
    稳定编码 特征类型概念节点;
    稳定编码 前值域概念节点;
    稳定编码 后值域概念节点;
    std::int64_t 相对纳秒 = 0;
    friend bool operator==(const 动态概念状态项&,
        const 动态概念状态项&) = default;
};

// 动态概念自身保存的可复用变化特征。实例概念保留来源状态；后续由
// 多个实例抽象出的概念可以省略具体来源，但类型、结果和值域仍须完整。
struct 动态概念特征项 final {
    std::int64_t 状态间位置 = 0;
    稳定编码 二次特征类型概念节点;
    std::int64_t 准确结果 = 0;
    稳定编码 结果值域概念节点;
    std::optional<稳定编码> 前来源状态节点;
    std::optional<稳定编码> 后来源状态节点;
    friend bool operator==(const 动态概念特征项&,
        const 动态概念特征项&) = default;
};

struct 动态概念信息 final {
    稳定编码 节点;
    动态概念种类 种类 = 动态概念种类::实例;
    std::optional<稳定编码> 对应动态节点;
    std::vector<动态概念状态项> 状态变化组;
    std::vector<动态概念特征项> 动态特征组;
    std::vector<稳定编码> 上位概念组;
    std::vector<稳定编码> 下位概念组;
    friend bool operator==(const 动态概念信息&,
        const 动态概念信息&) = default;
};

enum class 动态概念操作状态 : std::uint8_t {
    已建立 = 1,
    已复用 = 2,
    已更新 = 3,
    无变化 = 4,
    无共同点 = 5,
    入口拒绝 = 6,
    状态不足 = 7,
    状态不一致 = 8,
    来源不足 = 9,
    特征概念不存在 = 10,
    值域概念不存在 = 11,
    概念不存在 = 12,
    层级不成立 = 13,
    会形成环 = 14,
    结构不一致 = 15,
    资源失败 = 16,
    状态数量不合法 = 17
};

struct 动态概念建立结果 final {
    动态概念操作状态 状态 = 动态概念操作状态::入口拒绝;
    std::optional<稳定编码> 概念节点;
};

struct 动态概念聚合结果 final {
    动态概念操作状态 状态 = 动态概念操作状态::入口拒绝;
    std::optional<稳定编码> 聚合概念节点;
    std::vector<动态概念状态项> 共同状态变化组;
};

// 全局基础数据集中的物理组织根。它不是动态概念。
extern 稳定编码 动态概念图根节点;

class 概念_动态类 final {
public:
    概念_动态类(
        概念_特征类& 特征概念服务,
        const 新_特征值类& 特征值服务,
        概念_二次特征类& 二次特征概念服务) noexcept;

    bool 初始化() noexcept;

    // 每个双状态原子动态先建立一个新的实例概念，不按内容相同复用。
    动态概念建立结果 建立实例动态概念(
        稳定编码 实际动态节点,
        const std::vector<新状态信息>& 有序状态组) noexcept;

    // 复合动态的专属实例概念复制成员概念已经发布的状态变化材料，
    // 不持久引用成员实例概念，避免成员改用聚合概念后形成悬空关系。
    动态概念建立结果 建立复合实例动态概念(
        稳定编码 实际动态节点,
        const std::vector<稳定编码>& 有序成员动态概念组,
        const std::vector<动态概念特征项>& 跨特征时间特征组) noexcept;

    // 仅供动态建立失败或动态删除使用；调用前必须解除动态节点中的概念字段。
    bool 删除实例动态概念(
        稳定编码 实例概念节点,
        稳定编码 实际动态节点) noexcept;

    // 至少两个不同来源概念才可执行。只有状态变化项顺序、特征类型和
    // 相对时间相容时，才逐项提取前后值域的共同上位概念。
    动态概念聚合结果 聚合共同动态概念(
        const std::vector<稳定编码>& 来源概念组) noexcept;

    // 保存调用方已经通过新版二次特征计算确认的完整特征组。同一内容
    // 重复保存无写入；概念已有不同特征组时拒绝静默覆盖。
    动态概念操作状态 保存动态特征(
        稳定编码 概念节点,
        const std::vector<动态概念特征项>& 动态特征组) noexcept;

    std::optional<动态概念信息> 获取动态概念(
        稳定编码 概念节点) const noexcept;
    bool 是动态概念节点(稳定编码 节点) const noexcept;
    bool 是实例动态概念节点(稳定编码 节点) const noexcept;
    bool 是聚合动态概念节点(稳定编码 节点) const noexcept;
    std::vector<稳定编码> 查询上位动态概念(
        稳定编码 概念节点) const noexcept;
    std::vector<稳定编码> 查询下位动态概念(
        稳定编码 概念节点) const noexcept;

private:
    概念_特征类& 特征概念服务_;
    const 新_特征值类& 特征值服务_;
    概念_二次特征类& 二次特征概念服务_;
};

} // namespace 海中鱼巣
