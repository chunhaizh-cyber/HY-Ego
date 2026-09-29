#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "新_存在类.h"

namespace 海中鱼巣 {

// 全局基础数据集中的世界树根场景。节点稳定编码本身就是根场景身份。
extern 稳定编码 场景树根节点;

struct 新场景信息 final {
    稳定编码 节点;
    std::optional<稳定编码> 父场景节点;
    稳定编码 专属存在概念节点;
    std::vector<稳定编码> 特征节点组;
    std::vector<稳定编码> 直接成员存在组;
    std::vector<稳定编码> 直接子场景组;
    friend bool operator==(const 新场景信息&, const 新场景信息&) = default;
};

enum class 新场景操作状态 : std::uint8_t {
    已完成 = 1,
    无变化 = 2,
    入口拒绝 = 3,
    场景不存在 = 4,
    父场景不合法 = 5,
    存在不存在 = 6,
    存在已属于其它场景 = 7,
    存在建立失败 = 8,
    存在仍有子存在 = 9,
    会形成环 = 10,
    根场景不可操作 = 11,
    场景非空 = 12,
    结构不一致 = 13,
    资源失败 = 14
};

struct 新场景建立结果 final {
    新场景操作状态 状态 = 新场景操作状态::入口拒绝;
    std::optional<稳定编码> 场景节点;
    std::optional<稳定编码> 专属存在概念节点;
};

struct 新场景成员建立结果 final {
    新场景操作状态 状态 = 新场景操作状态::入口拒绝;
    std::optional<稳定编码> 存在节点;
    std::optional<稳定编码> 专属存在概念节点;
};

class 新_场景类 final {
public:
    explicit 新_场景类(新_存在类& 存在服务) noexcept;

    bool 初始化() noexcept;

    // 根场景没有父场景；重复调用返回同一个根场景。
    新场景建立结果 建立根场景() noexcept;
    // 普通场景只需确认直接父场景即可建立为空场景。
    新场景建立结果 建立场景(稳定编码 父场景节点) noexcept;
    新场景操作状态 迁移子场景(
        稳定编码 场景节点,
        稳定编码 新父场景节点) noexcept;
    // 只删除没有直接子场景和成员存在的非根场景；场景自身特征和
    // 专属存在概念由本调用同步清理。
    新场景操作状态 删除空场景(
        稳定编码 场景节点) noexcept;

    std::optional<新场景信息> 获取场景(
        稳定编码 场景节点) const noexcept;
    bool 是场景节点(稳定编码 节点) const noexcept;

    // 普通存在只能通过场景类建立并立即写入一个最小场景的成员字段。
    新场景成员建立结果 建立成员存在(
        稳定编码 场景节点,
        const 新存在建立请求& 请求) noexcept;
    新场景操作状态 迁移成员存在(
        稳定编码 存在节点,
        稳定编码 目标场景节点) noexcept;
    新场景操作状态 删除成员存在(
        稳定编码 场景节点,
        稳定编码 存在节点) noexcept;

    std::optional<稳定编码> 查询所属场景(
        稳定编码 存在节点) const noexcept;
    std::optional<稳定编码> 查询父场景(
        稳定编码 场景节点) const noexcept;
    std::vector<稳定编码> 查询直接子场景(
        稳定编码 场景节点) const noexcept;
    std::vector<稳定编码> 查询直接成员存在(
        稳定编码 场景节点) const noexcept;

    // 场景本身具有存在身份；特征操作复用存在类，并持续更新同一个
    // 专属存在概念节点。相对坐标矩阵不进入这里。
    新存在特征添加结果 添加场景特征(
        稳定编码 场景节点,
        稳定编码 特征概念节点,
        const 新特征准确值& 初始值) noexcept;
    新存在特征值更新结果 更新场景特征值(
        稳定编码 场景节点,
        稳定编码 特征节点,
        const 新特征准确值& 新值) noexcept;
    std::vector<稳定编码> 查询场景全部特征(
        稳定编码 场景节点) const noexcept;

private:
    新_存在类& 存在服务_;
};

} // namespace 海中鱼巣
