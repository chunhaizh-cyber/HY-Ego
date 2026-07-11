// 文件规则：特征服务是特征值的唯一直接业务入口；高级服务不得直接访问特征值服务。
#pragma once

#include "../核心/节点仓库.h"
#include "../核心/主信息仓库.h"
#include "../核心/关系仓库.h"
#include "../核心/容错检查.h"
#include "二次特征服务.h"
#include "特征值服务.h"

#include <algorithm>
#include <cstdint>
#include <mutex>
#include <optional>
#include <vector>

namespace 海中鱼巣 {

struct 概念特征约束材料 {
    节点句柄 宿主;
    节点句柄 特征定义;
    节点句柄 实例槽位;
    节点句柄 当前特征值;
    特征值原始类型 原始类型 = 特征值原始类型::未建立;
    std::optional<std::uint64_t> 容器版本;
    std::optional<std::int64_t> I64值;
    std::vector<std::int64_t> VecI64值;
    std::vector<std::uint64_t> VecU64值;

    bool 完整() const {
        const bool 身份完整 = 句柄有效(宿主)
            && 句柄有效(特征定义)
            && 句柄有效(实例槽位)
            && 句柄有效(当前特征值)
            && !(特征定义 == 实例槽位)
            && !(特征定义 == 当前特征值)
            && !(实例槽位 == 当前特征值);
        if (!身份完整) {
            return false;
        }
        switch (原始类型) {
        case 特征值原始类型::I64:
            return I64值.has_value()
                && VecI64值.empty()
                && VecU64值.empty()
                && !容器版本.has_value();
        case 特征值原始类型::VecI64:
            return !I64值.has_value()
                && !VecI64值.empty()
                && VecU64值.empty()
                && 容器版本.has_value()
                && 容器版本.value() != 0;
        case 特征值原始类型::VecU64:
            return !I64值.has_value()
                && VecI64值.empty()
                && !VecU64值.empty()
                && 容器版本.has_value()
                && 容器版本.value() != 0;
        default:
            return false;
        }
    }
};

class 特征服务 {
public:
    特征服务(主信息仓库& 主信息, 节点仓库& 节点)
        : 主信息_(主信息), 节点_(节点), 关系_(nullptr), 二次特征_(nullptr), 特征值_(nullptr) {
    }

    特征服务(主信息仓库& 主信息, 节点仓库& 节点, 关系仓库& 关系, 二次特征服务& 二次特征, 特征值服务& 特征值)
        : 主信息_(主信息), 节点_(节点), 关系_(&关系), 二次特征_(&二次特征), 特征值_(&特征值) {
    }

    节点句柄 创建特征类型() {
        return 节点_.创建节点(节点类型::特征, 主信息_.创建主信息());
    }

    节点句柄 创建二次特征() {
        if (二次特征_ == nullptr) {
            return {};
        }
        return 二次特征_->创建二次特征();
    }

    bool 节点是特征(节点句柄 特征节点) const {
        return 节点类型匹配(特征节点, 节点类型::特征);
    }

    std::optional<节点句柄> 读取特征身份(节点句柄 特征节点) const {
        if (!节点是特征(特征节点)) {
            return std::nullopt;
        }
        return 特征节点;
    }

    std::optional<节点句柄> 读取特征语义类型(节点句柄 特征节点) const {
        return 读取特征身份(特征节点);
    }

    节点句柄 创建实例特征槽位(节点句柄 宿主节点, 节点句柄 特征类型) {
        if (关系_ == nullptr || !节点类型是允许特征宿主(宿主节点) || !节点是特征(特征类型)
            || 读取宿主特征槽位(宿主节点, 特征类型).has_value()) {
            return {};
        }
        const auto 槽位节点 = 创建特征类型();
        if (!追根因检查(句柄有效(槽位节点), L"创建实例特征槽位时特征节点创建不及预期。")) {
            return {};
        }
        const auto 宿主关系 = 关系_->创建关系(关系类型::归属, 宿主节点, 槽位节点);
        if (!追根因检查(句柄有效(宿主关系), L"创建实例特征槽位时宿主归属关系写入不及预期。")) {
            return {};
        }
        const auto 类型关系 = 关系_->创建关系(关系类型::模板, 槽位节点, 特征类型);
        if (!追根因检查(句柄有效(类型关系), L"创建实例特征槽位时特征模板关系写入不及预期。")) {
            return {};
        }
        return 槽位节点;
    }

    std::optional<节点句柄> 读取宿主特征槽位(节点句柄 宿主节点, 节点句柄 特征类型) const {
        if (关系_ == nullptr || !节点类型是允许特征宿主(宿主节点) || !节点是特征(特征类型)) {
            return std::nullopt;
        }
        const auto 候选组 = 关系_->获取目标节点组(宿主节点, 关系类型::归属);
        for (const auto& 候选 : 候选组) {
            if (节点是特征(候选) && 关系_->存在关系(关系类型::模板, 候选, 特征类型)) {
                return 候选;
            }
        }
        return std::nullopt;
    }

    std::vector<节点句柄> 读取宿主特征集合(节点句柄 宿主节点) const {
        std::vector<节点句柄> 特征集合;
        if (关系_ == nullptr || !节点类型是允许特征宿主(宿主节点)) {
            return 特征集合;
        }
        const auto 候选组 = 关系_->获取目标节点组(宿主节点, 关系类型::归属);
        for (const auto& 候选 : 候选组) {
            if (节点是特征(候选) && 槽位绑定特征类型(候选)) {
                特征集合.push_back(候选);
            }
        }
        return 特征集合;
    }

    bool 写入宿主I64特征状态材料(节点句柄 宿主节点, 节点句柄 特征类型, std::int64_t 值) {
        if (特征值_ == nullptr) {
            return false;
        }
        const auto 槽位节点 = 读取宿主特征槽位(宿主节点, 特征类型);
        if (!槽位节点.has_value()) {
            return false;
        }
        const auto 当前值材料 = 读取实例槽位当前值材料(槽位节点.value());
        if (当前值材料.状态 == 实例槽位当前值状态::无效
            || 当前值材料.状态 == 实例槽位当前值状态::多值歧义) {
            return false;
        }
        auto 特征值节点 = 当前值材料.当前特征值;
        if (当前值材料.状态 == 实例槽位当前值状态::无当前值) {
            const auto 新特征值节点 = 创建特征值(槽位节点.value());
            if (!句柄有效(新特征值节点)) {
                return false;
            }
            特征值节点 = 新特征值节点;
        }
        return 写入I64特征值(槽位节点.value(), 特征值节点, 值);
    }

    std::optional<std::int64_t> 读取宿主I64特征状态材料(节点句柄 宿主节点, 节点句柄 特征类型) const {
        const auto 槽位节点 = 读取宿主特征槽位(宿主节点, 特征类型);
        if (!槽位节点.has_value()) {
            return std::nullopt;
        }
        const auto 特征值节点 = 读取实例槽位当前特征值(槽位节点.value());
        if (!特征值节点.has_value()) {
            return std::nullopt;
        }
        return 读取I64特征值(槽位节点.value(), 特征值节点.value());
    }

    std::optional<节点句柄> 读取实例槽位当前特征值(节点句柄 槽位节点) const {
        const auto 材料 = 读取实例槽位当前值材料(槽位节点);
        if (材料.状态 != 实例槽位当前值状态::唯一当前值) {
            return std::nullopt;
        }
        return 材料.当前特征值;
    }

    节点句柄 创建特征值(节点句柄 特征节点) {
        if (!节点类型匹配(特征节点, 节点类型::特征) || 特征值_ == nullptr || 关系_ == nullptr) {
            return {};
        }
        std::unique_lock<std::mutex> 槽位写锁(实例槽位当前值写锁_, std::defer_lock);
        if (节点是实例特征槽位(特征节点)) {
            槽位写锁.lock();
            const auto 当前值材料 = 读取实例槽位当前值材料(特征节点);
            if (当前值材料.状态 != 实例槽位当前值状态::无当前值) {
                return {};
            }
        }
        const auto 特征值节点 = 特征值_->创建特征值();
        if (!追根因检查(句柄有效(特征值节点), L"创建特征值并归属时特征值节点创建不及预期。")) {
            return {};
        }
        const auto 归属关系 = 关系_->创建关系(关系类型::归属, 特征节点, 特征值节点);
        if (!追根因检查(句柄有效(归属关系), L"创建特征值并归属时归属关系写入不及预期。")) {
            return {};
        }
        return 特征值节点;
    }

    bool 写入I64特征值(节点句柄 特征节点, 节点句柄 特征值节点, std::int64_t 值) {
        if (!特征值属于特征(特征节点, 特征值节点) || 特征值_ == nullptr) {
            return false;
        }
        return 特征值_->写入I64值(特征值节点, 值);
    }

    std::optional<std::int64_t> 读取I64特征值(节点句柄 特征节点, 节点句柄 特征值节点) const {
        if (!特征值属于特征(特征节点, 特征值节点) || 特征值_ == nullptr) {
            return std::nullopt;
        }
        return 特征值_->读取I64值(特征值节点);
    }

    bool 写入VecI64特征值(
        节点句柄 特征节点,
        节点句柄 特征值节点,
        const std::vector<std::int64_t>& 值) {
        if (!特征值属于特征(特征节点, 特征值节点)
            || 特征值_ == nullptr
            || 值.empty()
            || 值.size() > 特征值服务::最大序列元素数量) {
            return false;
        }
        return 特征值_->写入VecI64值(特征值节点, 值);
    }

    std::optional<std::vector<std::int64_t>> 读取VecI64特征值(
        节点句柄 特征节点,
        节点句柄 特征值节点) const {
        if (!特征值属于特征(特征节点, 特征值节点) || 特征值_ == nullptr) {
            return std::nullopt;
        }
        return 特征值_->读取VecI64值(特征值节点);
    }

    bool 写入VecU64特征值(
        节点句柄 特征节点,
        节点句柄 特征值节点,
        const std::vector<std::uint64_t>& 值) {
        if (!特征值属于特征(特征节点, 特征值节点)
            || 特征值_ == nullptr
            || 值.empty()
            || 值.size() > 特征值服务::最大序列元素数量) {
            return false;
        }
        return 特征值_->写入VecU64值(特征值节点, 值);
    }

    std::optional<std::vector<std::uint64_t>> 读取VecU64特征值(
        节点句柄 特征节点,
        节点句柄 特征值节点) const {
        if (!特征值属于特征(特征节点, 特征值节点) || 特征值_ == nullptr) {
            return std::nullopt;
        }
        return 特征值_->读取VecU64值(特征值节点);
    }

    std::optional<海中鱼巣::特征值原始类型> 读取特征值原始类型(
        节点句柄 特征节点,
        节点句柄 特征值节点) const {
        if (!特征值属于特征(特征节点, 特征值节点) || 特征值_ == nullptr) {
            return std::nullopt;
        }
        return 特征值_->读取原始类型(特征值节点);
    }

    std::optional<std::uint64_t> 读取特征值原始版本(
        节点句柄 特征节点,
        节点句柄 特征值节点) const {
        if (!特征值属于特征(特征节点, 特征值节点) || 特征值_ == nullptr) {
            return std::nullopt;
        }
        return 特征值_->读取原始值版本(特征值节点);
    }

    std::optional<特征值原始材料> 读取特征值原始材料(
        节点句柄 特征节点,
        节点句柄 特征值节点) const {
        if (!特征值属于特征(特征节点, 特征值节点) || 特征值_ == nullptr) {
            return std::nullopt;
        }
        return 特征值_->读取原始值材料(特征值节点);
    }

    std::optional<std::vector<概念特征约束材料>> 读取宿主概念特征约束组(节点句柄 宿主节点) const {
        if (关系_ == nullptr || 特征值_ == nullptr || !节点类型是允许特征宿主(宿主节点)) {
            return std::nullopt;
        }
        std::vector<概念特征约束材料> 结果;
        const auto 槽位组 = 读取宿主特征集合(宿主节点);
        结果.reserve(槽位组.size());
        for (const auto& 槽位 : 槽位组) {
            const auto 特征定义 = 读取槽位唯一特征定义(槽位);
            const auto 当前值 = 读取实例槽位当前特征值(槽位);
            if (!特征定义.has_value() || !当前值.has_value()) {
                return std::nullopt;
            }
            const auto 原始材料 = 读取特征值原始材料(槽位, 当前值.value());
            if (!原始材料.has_value()) {
                return std::nullopt;
            }

            概念特征约束材料 材料;
            材料.宿主 = 宿主节点;
            材料.特征定义 = 特征定义.value();
            材料.实例槽位 = 槽位;
            材料.当前特征值 = 当前值.value();
            材料.原始类型 = 原始材料->原始类型;
            switch (材料.原始类型) {
            case 特征值原始类型::I64:
                材料.I64值 = 原始材料->I64值;
                break;
            case 特征值原始类型::VecI64:
                材料.VecI64值 = 原始材料->VecI64值;
                材料.容器版本 = 原始材料->原始值版本;
                break;
            case 特征值原始类型::VecU64:
                材料.VecU64值 = 原始材料->VecU64值;
                材料.容器版本 = 原始材料->原始值版本;
                break;
            default:
                return std::nullopt;
            }
            if (!材料.完整()) {
                return std::nullopt;
            }
            结果.push_back(std::move(材料));
        }
        std::sort(结果.begin(), 结果.end(), [](const auto& 左, const auto& 右) {
            if (节点句柄小于(左.特征定义, 右.特征定义)) {
                return true;
            }
            if (节点句柄小于(右.特征定义, 左.特征定义)) {
                return false;
            }
            return 节点句柄小于(左.实例槽位, 右.实例槽位);
        });
        return 结果;
    }

    bool 特征值属于特征(节点句柄 特征节点, 节点句柄 特征值节点) const {
        return 关系_ != nullptr
            && 节点类型匹配(特征节点, 节点类型::特征)
            && 节点类型匹配(特征值节点, 节点类型::特征值)
            && 关系_->存在关系(关系类型::归属, 特征节点, 特征值节点);
    }

private:
    enum class 实例槽位当前值状态 : std::uint32_t {
        无效 = 0,
        无当前值 = 1,
        唯一当前值 = 2,
        多值歧义 = 3
    };

    struct 实例槽位当前值材料 {
        实例槽位当前值状态 状态 = 实例槽位当前值状态::无效;
        节点句柄 当前特征值;
    };

    static bool 节点句柄小于(const 节点句柄& 左, const 节点句柄& 右) {
        if (左.仓库编号 != 右.仓库编号) {
            return 左.仓库编号 < 右.仓库编号;
        }
        if (左.节点编号 != 右.节点编号) {
            return 左.节点编号 < 右.节点编号;
        }
        return 左.版本号 < 右.版本号;
    }

    bool 节点类型匹配(节点句柄 节点句柄值, 节点类型 类型) const {
        const auto 记录 = 节点_.读取节点(节点句柄值);
        return 记录.has_value() && 记录->类型 == 类型;
    }

    bool 节点类型是允许特征宿主(节点句柄 节点句柄值) const {
        const auto 记录 = 节点_.读取节点(节点句柄值);
        if (!记录.has_value()) {
            return false;
        }
        return 记录->类型 == 节点类型::存在
            || 记录->类型 == 节点类型::场景
            || 记录->类型 == 节点类型::需求
            || 记录->类型 == 节点类型::任务
            || 记录->类型 == 节点类型::方法;
    }

    std::optional<节点句柄> 读取槽位唯一特征定义(节点句柄 槽位节点) const {
        if (关系_ == nullptr || !节点是特征(槽位节点)) {
            return std::nullopt;
        }
        std::optional<节点句柄> 结果;
        const auto 候选组 = 关系_->获取目标节点组(槽位节点, 关系类型::模板);
        for (const auto& 候选 : 候选组) {
            if (!节点是特征(候选)) {
                continue;
            }
            if (结果.has_value()) {
                return std::nullopt;
            }
            结果 = 候选;
        }
        return 结果;
    }

    实例槽位当前值材料 读取实例槽位当前值材料(节点句柄 槽位节点) const {
        实例槽位当前值材料 结果;
        if (关系_ == nullptr || !节点是实例特征槽位(槽位节点)) {
            return 结果;
        }
        std::vector<节点句柄> 当前值组;
        const auto 候选组 = 关系_->获取目标节点组(槽位节点, 关系类型::归属);
        for (const auto& 候选 : 候选组) {
            if (!节点类型匹配(候选, 节点类型::特征值)) {
                return 结果;
            }
            当前值组.push_back(候选);
        }
        std::sort(当前值组.begin(), 当前值组.end(), 节点句柄小于);
        当前值组.erase(std::unique(当前值组.begin(), 当前值组.end()), 当前值组.end());
        if (当前值组.empty()) {
            结果.状态 = 实例槽位当前值状态::无当前值;
            return 结果;
        }
        if (当前值组.size() == 1) {
            结果.状态 = 实例槽位当前值状态::唯一当前值;
            结果.当前特征值 = 当前值组.front();
            return 结果;
        }
        (void)追根因检查(false, L"实例特征槽位存在多个归属特征值，不能选择第一个。" );
        结果.状态 = 实例槽位当前值状态::多值歧义;
        return 结果;
    }

    bool 节点是实例特征槽位(节点句柄 槽位节点) const {
        if (关系_ == nullptr || !节点是特征(槽位节点) || !槽位绑定特征类型(槽位节点)) {
            return false;
        }
        const auto 宿主候选组 = 关系_->获取来源节点组(槽位节点, 关系类型::归属);
        return std::any_of(宿主候选组.begin(), 宿主候选组.end(), [this](const auto& 宿主候选) {
            return 节点类型是允许特征宿主(宿主候选);
        });
    }

    bool 槽位绑定特征类型(节点句柄 槽位节点) const {
        if (关系_ == nullptr || !节点是特征(槽位节点)) {
            return false;
        }
        const auto 类型候选组 = 关系_->获取目标节点组(槽位节点, 关系类型::模板);
        for (const auto& 类型候选 : 类型候选组) {
            if (节点是特征(类型候选)) {
                return true;
            }
        }
        return false;
    }

    主信息仓库& 主信息_;
    节点仓库& 节点_;
    关系仓库* 关系_;
    二次特征服务* 二次特征_;
    特征值服务* 特征值_;
    mutable std::mutex 实例槽位当前值写锁_;
};

}
