#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "../核心/基础数据集.h"
#include "新_特征值类.h"
#include "概念_特征类.h"

namespace 海中鱼巣 {

using 新特征准确值 = 特征概念准确值;

struct 新特征信息 final {
    稳定编码 节点;
    稳定编码 持有节点;
    稳定编码 特征概念节点;
    特征概念值域 实例值域;
    std::optional<稳定编码> 实例单位;
    std::optional<新特征准确值> 当前值;
    std::vector<新特征准确值> 历史不同值;
    friend bool operator==(const 新特征信息&, const 新特征信息&) = default;
};

enum class 新特征写入状态 : std::uint8_t {
    已完成 = 1,
    无变化 = 2,
    入口拒绝 = 3,
    特征不存在 = 4,
    概念不存在 = 5,
    材料类型不相容 = 6,
    结构不一致 = 7,
    值已更新但聚合未完成 = 8,
    资源失败 = 9
};

struct 新特征值添加结果 final {
    新特征写入状态 状态 = 新特征写入状态::入口拒绝;
    bool 当前值已改变 = false;
    bool 历史集合已增加 = false;
    std::optional<特征概念聚合结果> 聚合结果;
};

class 新_特征类 final {
public:
    // 两个依赖服务必须操作全局基础数据集，并且比本对象存活更久。
    新_特征类(
        新_特征值类& 特征值服务,
        概念_特征类& 特征概念服务) noexcept;

    // 建立本类共享的类型节点与字段节点；重复调用不会重复建立。
    bool 初始化() noexcept;

    // 同一持有节点和同一特征概念只允许一个特征节点。
    稳定编码 建立或取得特征(
        稳定编码 持有节点,
        稳定编码 特征概念节点) noexcept;

    std::optional<稳定编码> 查询特征(
        稳定编码 持有节点,
        稳定编码 特征概念节点) const noexcept;

    std::optional<新特征信息> 获取特征(
        稳定编码 特征节点) const noexcept;

    bool 是特征节点(稳定编码 节点) const noexcept;

    // 更新当前值；历史集合仅在首次出现该准确值时增加。
    // 只有历史集合发生变化时，才按特征概念规则同步尝试一次聚合。
    新特征值添加结果 添加特征值(
        稳定编码 特征节点,
        const 新特征准确值& 新值) noexcept;

private:
    新_特征值类& 特征值服务_;
    概念_特征类& 特征概念服务_;
};

} // namespace 海中鱼巣
