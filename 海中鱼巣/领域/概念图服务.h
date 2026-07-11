// 文件规则：概念图服务只编排四类概念根和概念专用关系；不得用名称、日志、显示或索引候选裁决概念事实。
#pragma once

#include "../核心/关系仓库.h"
#include "../核心/容错检查.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <vector>

namespace 海中鱼巣 {

enum class 概念根类别 : std::uint32_t {
    未定义 = 0,
    存在 = 1,
    动态 = 2,
    关系 = 3,
    因果 = 4
};

struct 概念根登记材料 {
    概念根类别 根类别 = 概念根类别::未定义;
    std::uint64_t 稳定非名称键 = 0;
    节点句柄 根节点;
};

class 概念图服务 {
public:
    概念图服务(const 节点仓库& 节点, 关系仓库& 关系)
        : 节点_(节点), 关系_(关系) {
    }

    std::optional<节点句柄> 登记概念根(const 概念根登记材料& 材料) {
        const auto 根记录 = 节点_.读取节点(材料.根节点);
        const auto 节点类别 = 根记录.has_value() ? 节点类型对应根类别(根记录->类型) : std::nullopt;
        if (!根类别有效(材料.根类别)
            || 材料.稳定非名称键 == 0
            || !节点类别.has_value()
            || 节点类别.value() != 材料.根类别) {
            return std::nullopt;
        }

        std::lock_guard<std::mutex> 图锁(图写锁_);
        if (!关系_.获取来源节点组(材料.根节点, 关系类型::概念上下位).empty()) {
            return std::nullopt;
        }

        std::unique_lock<std::shared_mutex> 根锁(根登记锁_);
        for (const auto& 已登记 : 根登记组_) {
            if (!已登记.has_value()) {
                continue;
            }
            if (已登记->根类别 == 材料.根类别) {
                if (已登记->稳定非名称键 == 材料.稳定非名称键 && 已登记->根节点 == 材料.根节点) {
                    return 已登记->根节点;
                }
                return std::nullopt;
            }
            if (已登记->稳定非名称键 == 材料.稳定非名称键 || 已登记->根节点 == 材料.根节点) {
                return std::nullopt;
            }
        }

        根登记组_[根类别索引(材料.根类别)] = 材料;
        const auto& 已发布 = 根登记组_[根类别索引(材料.根类别)];
        if (!追根因检查(已发布.has_value()
            && 已发布->根类别 == 材料.根类别
            && 已发布->稳定非名称键 == 材料.稳定非名称键
            && 已发布->根节点 == 材料.根节点,
            L"概念根登记发布后读回不符合内部预期。")) {
            return std::nullopt;
        }
        return 已发布->根节点;
    }

    std::optional<概念根登记材料> 读取概念根(概念根类别 类别) const {
        if (!根类别有效(类别)) {
            return std::nullopt;
        }
        std::shared_lock<std::shared_mutex> 锁(根登记锁_);
        const auto& 材料 = 根登记组_[根类别索引(类别)];
        if (!材料.has_value() || !节点_.节点是否有效(材料->根节点)) {
            return std::nullopt;
        }
        return 材料;
    }

    std::vector<概念根登记材料> 读取全部概念根() const {
        std::vector<概念根登记材料> 结果;
        std::shared_lock<std::shared_mutex> 锁(根登记锁_);
        结果.reserve(根登记组_.size());
        for (const auto& 材料 : 根登记组_) {
            if (材料.has_value() && 节点_.节点是否有效(材料->根节点)) {
                结果.push_back(材料.value());
            }
        }
        return 结果;
    }

    bool 节点是否已登记概念根(节点句柄 节点) const {
        if (!节点_.节点是否有效(节点)) {
            return false;
        }
        std::shared_lock<std::shared_mutex> 锁(根登记锁_);
        return std::any_of(根登记组_.begin(), 根登记组_.end(), [&节点](const auto& 材料) {
            return 材料.has_value() && 材料->根节点 == 节点;
        });
    }

    std::optional<概念根类别> 读取节点概念类别(节点句柄 节点) const {
        const auto 记录 = 节点_.读取节点(节点);
        if (!记录.has_value()) {
            return std::nullopt;
        }
        return 节点类型对应根类别(记录->类型);
    }

    bool 绑定实例支持概念(节点句柄 实例, 节点句柄 概念) {
        std::lock_guard<std::mutex> 图锁(图写锁_);
        const auto 实例类别 = 读取节点概念类别(实例);
        const auto 概念类别 = 读取节点概念类别(概念);
        if (实例 == 概念
            || !实例类别.has_value()
            || !概念类别.has_value()
            || 实例类别.value() != 概念类别.value()
            || 节点是否已登记概念根(实例)) {
            return false;
        }
        return 幂等创建专用关系(关系类型::实例支持概念, 实例, 概念);
    }

    std::vector<节点句柄> 读取实例支持概念(节点句柄 实例) const {
        return 关系_.获取目标节点组(实例, 关系类型::实例支持概念);
    }

    std::vector<节点句柄> 读取支持概念的实例(节点句柄 概念) const {
        return 关系_.获取来源节点组(概念, 关系类型::实例支持概念);
    }

    std::optional<节点句柄> 确保存在实例根支持(节点句柄 存在实例) {
        const auto 实例记录 = 节点_.读取节点(存在实例);
        const auto 根材料 = 读取概念根(概念根类别::存在);
        if (!实例记录.has_value()
            || 实例记录->类型 != 节点类型::存在
            || !根材料.has_value()
            || 存在实例 == 根材料->根节点) {
            return std::nullopt;
        }
        if (!绑定实例支持概念(存在实例, 根材料->根节点)) {
            return std::nullopt;
        }
        const auto 实例目标组 = 读取实例支持概念(存在实例);
        const auto 根来源组 = 读取支持概念的实例(根材料->根节点);
        const bool 双向可读 = std::find(实例目标组.begin(), 实例目标组.end(), 根材料->根节点) != 实例目标组.end()
            && std::find(根来源组.begin(), 根来源组.end(), 存在实例) != 根来源组.end();
        if (!追根因检查(双向可读, L"确保存在实例根支持后双向读回不符合内部预期。")) {
            return std::nullopt;
        }
        return 根材料->根节点;
    }

    bool 绑定概念上下位(节点句柄 上位概念, 节点句柄 下位概念) {
        std::lock_guard<std::mutex> 图锁(图写锁_);
        const auto 上位类别 = 读取节点概念类别(上位概念);
        const auto 下位类别 = 读取节点概念类别(下位概念);
        if (上位概念 == 下位概念
            || !上位类别.has_value()
            || !下位类别.has_value()
            || 上位类别.value() != 下位类别.value()
            || !节点是已知概念(上位概念)
            || !节点是已知概念(下位概念)) {
            return false;
        }
        if (关系_.存在关系(关系类型::概念上下位, 上位概念, 下位概念)) {
            return true;
        }
        if (沿概念下位可达(下位概念, 上位概念)) {
            return false;
        }
        return 幂等创建专用关系(关系类型::概念上下位, 上位概念, 下位概念);
    }

    std::vector<节点句柄> 读取概念直接上位(节点句柄 概念) const {
        return 关系_.获取来源节点组(概念, 关系类型::概念上下位);
    }

    std::vector<节点句柄> 读取概念直接下位(节点句柄 概念) const {
        return 关系_.获取目标节点组(概念, 关系类型::概念上下位);
    }

    bool 绑定概念定义材料(节点句柄 概念, 节点句柄 定义材料) {
        std::lock_guard<std::mutex> 图锁(图写锁_);
        const auto 材料记录 = 节点_.读取节点(定义材料);
        if (概念 == 定义材料
            || !节点是已知概念(概念)
            || !材料记录.has_value()
            || !定义材料类型允许(材料记录->类型)) {
            return false;
        }
        return 幂等创建专用关系(关系类型::概念定义材料, 概念, 定义材料);
    }

    std::vector<节点句柄> 读取概念定义材料(节点句柄 概念) const {
        return 关系_.获取目标节点组(概念, 关系类型::概念定义材料);
    }

private:
    static bool 根类别有效(概念根类别 类别) {
        return 类别 >= 概念根类别::存在 && 类别 <= 概念根类别::因果;
    }

    static std::size_t 根类别索引(概念根类别 类别) {
        return static_cast<std::size_t>(类别) - 1;
    }

    static std::optional<概念根类别> 节点类型对应根类别(节点类型 类型) {
        switch (类型) {
        case 节点类型::存在:
            return 概念根类别::存在;
        case 节点类型::动态:
            return 概念根类别::动态;
        case 节点类型::二次特征:
            return 概念根类别::关系;
        case 节点类型::因果引用:
            return 概念根类别::因果;
        default:
            return std::nullopt;
        }
    }

    static bool 定义材料类型允许(节点类型 类型) {
        return 类型 == 节点类型::特征
            || 类型 == 节点类型::状态
            || 类型 == 节点类型::存在
            || 类型 == 节点类型::动态
            || 类型 == 节点类型::二次特征
            || 类型 == 节点类型::因果引用;
    }

    bool 节点是已知概念(节点句柄 节点) const {
        return 节点是否已登记概念根(节点)
            || !关系_.获取来源节点组(节点, 关系类型::实例支持概念).empty()
            || !关系_.获取来源节点组(节点, 关系类型::概念上下位).empty()
            || !关系_.获取目标节点组(节点, 关系类型::概念上下位).empty()
            || !关系_.获取目标节点组(节点, 关系类型::概念定义材料).empty();
    }

    bool 沿概念下位可达(节点句柄 起点, 节点句柄 目标) const {
        std::vector<节点句柄> 待检查{起点};
        std::vector<节点句柄> 已检查;
        while (!待检查.empty()) {
            const auto 当前 = 待检查.back();
            待检查.pop_back();
            if (当前 == 目标) {
                return true;
            }
            if (std::find(已检查.begin(), 已检查.end(), 当前) != 已检查.end()) {
                continue;
            }
            已检查.push_back(当前);
            const auto 下位组 = 关系_.获取目标节点组(当前, 关系类型::概念上下位);
            待检查.insert(待检查.end(), 下位组.begin(), 下位组.end());
        }
        return false;
    }

    bool 幂等创建专用关系(关系类型 类型, 节点句柄 源节点, 节点句柄 目标节点) {
        if (关系_.存在关系(类型, 源节点, 目标节点)) {
            return true;
        }
        const auto 新关系 = 关系_.创建关系(类型, 源节点, 目标节点);
        return 追根因检查(句柄有效(新关系) && 关系_.存在关系(类型, 源节点, 目标节点),
            L"概念图专用关系写入后读回不符合内部预期。");
    }

    const 节点仓库& 节点_;
    关系仓库& 关系_;
    mutable std::shared_mutex 根登记锁_;
    std::array<std::optional<概念根登记材料>, 4> 根登记组_;
    std::mutex 图写锁_;
};

}
