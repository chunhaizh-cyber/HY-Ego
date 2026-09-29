#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "../核心/基础数据集.h"
#include "概念_特征类.h"

namespace 海中鱼巣 {

enum class 存在概念种类 : std::uint8_t {
    专属 = 1,
    抽象 = 2
};

struct 存在概念特征项 final {
    稳定编码 特征类型概念;
    稳定编码 具体特征概念;
    friend bool operator==(const 存在概念特征项&,
        const 存在概念特征项&) = default;
};

struct 存在概念信息 final {
    稳定编码 节点;
    存在概念种类 种类 = 存在概念种类::专属;
    std::optional<稳定编码> 对应存在节点;
    std::vector<存在概念特征项> 特征组;
    std::vector<稳定编码> 上位概念组;
    std::vector<稳定编码> 下位概念组;
    friend bool operator==(const 存在概念信息&,
        const 存在概念信息&) = default;
};

enum class 存在概念操作状态 : std::uint8_t {
    已建立 = 1,
    已复用 = 2,
    已更新 = 3,
    无变化 = 4,
    无共同点 = 5,
    入口拒绝 = 6,
    来源不足 = 7,
    概念不存在 = 8,
    特征概念不一致 = 9,
    层级不成立 = 10,
    会形成环 = 11,
    结构不一致 = 12,
    资源失败 = 13
};

struct 存在概念建立结果 final {
    存在概念操作状态 状态 = 存在概念操作状态::入口拒绝;
    std::optional<稳定编码> 概念节点;
};

struct 存在概念抽象结果 final {
    存在概念操作状态 状态 = 存在概念操作状态::入口拒绝;
    std::optional<稳定编码> 抽象概念节点;
    std::vector<存在概念特征项> 共同特征组;
};

// 全局基础数据集中的物理组织根。它不是存在概念，不参与抽象和匹配。
extern 稳定编码 存在概念图根节点;

class 概念_存在类 final {
public:
    explicit 概念_存在类(概念_特征类& 特征概念服务) noexcept;

    bool 初始化() noexcept;

    // 每个实际存在总是建立一个新的专属概念，不按特征相同复用。
    存在概念建立结果 建立专属存在概念(
        稳定编码 实际存在节点,
        const std::vector<稳定编码>& 具体特征概念组) noexcept;

    // 只允许更新专属概念的完整特征投影；不自动产生抽象概念。
    存在概念操作状态 更新专属存在概念(
        稳定编码 专属概念节点,
        const std::vector<稳定编码>& 具体特征概念组) noexcept;

    // 仅供存在建立失败回滚：概念尚未进入上下位图、且存在节点尚未写入
    // 专属概念字段时才允许撤销。
    bool 撤销未绑定专属存在概念(
        稳定编码 专属概念节点,
        稳定编码 实际存在节点) noexcept;

    // 至少两个不同来源概念才可执行。默认提取全部来源的最大共同特征集；
    // 没有共同点时正常返回“无共同点”，不会建立空抽象概念。
    存在概念抽象结果 抽象共同存在概念(
        const std::vector<稳定编码>& 来源概念组) noexcept;

    std::optional<存在概念信息> 获取存在概念(
        稳定编码 概念节点) const noexcept;

    bool 是存在概念节点(稳定编码 节点) const noexcept;
    bool 是专属存在概念节点(稳定编码 节点) const noexcept;
    bool 是抽象存在概念节点(稳定编码 节点) const noexcept;

    std::vector<稳定编码> 查询上位存在概念(
        稳定编码 概念节点) const noexcept;
    std::vector<稳定编码> 查询下位存在概念(
        稳定编码 概念节点) const noexcept;

private:
    概念_特征类& 特征概念服务_;
};

} // namespace 海中鱼巣
