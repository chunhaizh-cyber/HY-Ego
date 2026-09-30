#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "../核心/基础数据集.h"
#include "新_特征类.h"
#include "新_状态类.h"
#include "概念_存在类.h"

namespace 海中鱼巣 {

class 新_场景类;

struct 新存在初始特征 final {
    稳定编码 特征概念节点;
    新特征准确值 初始值;
    friend bool operator==(const 新存在初始特征&,
        const 新存在初始特征&) = default;
};

struct 新存在建立请求 final {
    std::optional<稳定编码> 父存在节点;
    std::vector<新存在初始特征> 初始特征组;
};

struct 新存在信息 final {
    稳定编码 节点;
    std::optional<稳定编码> 父存在节点;
    稳定编码 专属存在概念节点;
    std::vector<稳定编码> 特征节点组;
    std::vector<稳定编码> 子存在节点组;
    friend bool operator==(const 新存在信息&, const 新存在信息&) = default;
};

enum class 新存在操作状态 : std::uint8_t {
    已完成 = 1,
    无变化 = 2,
    入口拒绝 = 3,
    存在不存在 = 4,
    父存在不合法 = 5,
    初始特征不足 = 6,
    特征概念不合法 = 7,
    特征类型重复 = 8,
    专属概念失败 = 9,
    会形成环 = 10,
    结构不一致 = 11,
    资源失败 = 12,
    特征已更新但专属概念未完成 = 13,
    仍有子存在 = 14,
    状态时间不合法 = 15,
    特征已更新但状态未完成 = 16,
    仍被状态引用 = 17
};

struct 新存在建立结果 final {
    新存在操作状态 状态 = 新存在操作状态::入口拒绝;
    std::optional<稳定编码> 存在节点;
    std::optional<稳定编码> 专属存在概念节点;
    std::vector<稳定编码> 初始状态节点组;
};

struct 新存在特征添加结果 final {
    新存在操作状态 状态 = 新存在操作状态::入口拒绝;
    std::optional<稳定编码> 特征节点;
    std::optional<新状态建立结果> 状态结果;
};

struct 新存在特征值更新结果 final {
    新存在操作状态 状态 = 新存在操作状态::入口拒绝;
    std::optional<新特征值添加结果> 特征结果;
    std::optional<存在概念操作状态> 专属概念结果;
    std::optional<新状态建立结果> 状态结果;
};

class 新_存在类 final {
public:
    新_存在类(
        新_特征类& 特征服务,
        概念_存在类& 存在概念服务,
        新_状态类& 状态服务) noexcept;

    bool 初始化() noexcept;

    std::optional<新存在信息> 获取存在(
        稳定编码 存在节点) const noexcept;

    bool 是存在节点(稳定编码 节点) const noexcept;

    // 特征节点由新_特征类建立；持有关系只在这里作为存在内部字段写入。
    新存在特征添加结果 添加特征(
        稳定编码 存在节点,
        稳定编码 特征概念节点,
        const 新特征准确值& 初始值,
        const 新状态强时间& 强时间) noexcept;

    // 存在实例中的特征值应通过本入口更新，使专属存在概念同步读取
    // 该存在的完整特征概念集合。特征事实已经更新而概念同步失败时，
    // 不回滚实际值，明确返回“特征已更新但专属概念未完成”。
    新存在特征值更新结果 更新特征值(
        稳定编码 存在节点,
        稳定编码 特征节点,
        const 新特征准确值& 新值,
        const 新状态强时间& 强时间) noexcept;

    std::vector<稳定编码> 查询全部特征(
        稳定编码 存在节点) const noexcept;

    std::optional<稳定编码> 查询专属存在概念(
        稳定编码 存在节点) const noexcept;

    新存在操作状态 添加子存在(
        稳定编码 父存在节点,
        稳定编码 子存在节点) noexcept;

    新存在操作状态 移除子存在(
        稳定编码 父存在节点,
        稳定编码 子存在节点) noexcept;

    std::vector<稳定编码> 查询直接子存在(
        稳定编码 存在节点) const noexcept;

private:
    friend class 新_场景类;

    // 场景类先建立节点及场景成员字段，再在同一编排中初始化存在本体。
    // 失败时只回滚本类写入，节点与场景字段仍由场景类清理。
    新存在建立结果 初始化存在节点(
        稳定编码 存在节点,
        const 新存在建立请求& 请求,
        bool 允许空特征,
        const std::optional<新状态强时间>& 初始状态时间) noexcept;

    // 调用方必须先解除场景成员字段；本函数只清理存在内部字段、特征、
    // 专属概念和存在组成关系，不删除传入节点本身。
    新存在操作状态 清理存在节点内容(
        稳定编码 存在节点) noexcept;

    新_特征类& 特征服务_;
    概念_存在类& 存在概念服务_;
    新_状态类& 状态服务_;
};

} // namespace 海中鱼巣
