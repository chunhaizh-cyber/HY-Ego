// 文件规则：特征服务是特征值的唯一直接业务入口；高级服务不得直接访问特征值服务。
#pragma once

#include "../核心/节点仓库.h"
#include "../核心/主信息仓库.h"
#include "../核心/关系仓库.h"
#include "../核心/容错检查.h"
#include "二次特征服务.h"
#include "特征值服务.h"

#include <cstdint>
#include <optional>
#include <vector>

namespace 海中鱼巣 {

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
        auto 特征值节点 = 读取宿主当前特征值(槽位节点.value());
        if (!特征值节点.has_value()) {
            const auto 新特征值节点 = 创建特征值(槽位节点.value());
            if (!句柄有效(新特征值节点)) {
                return false;
            }
            特征值节点 = 新特征值节点;
        }
        return 写入I64特征值(槽位节点.value(), 特征值节点.value(), 值);
    }

    std::optional<std::int64_t> 读取宿主I64特征状态材料(节点句柄 宿主节点, 节点句柄 特征类型) const {
        const auto 槽位节点 = 读取宿主特征槽位(宿主节点, 特征类型);
        if (!槽位节点.has_value()) {
            return std::nullopt;
        }
        const auto 特征值节点 = 读取宿主当前特征值(槽位节点.value());
        if (!特征值节点.has_value()) {
            return std::nullopt;
        }
        return 读取I64特征值(槽位节点.value(), 特征值节点.value());
    }

    节点句柄 创建特征值(节点句柄 特征节点) {
        if (!节点类型匹配(特征节点, 节点类型::特征) || 特征值_ == nullptr || 关系_ == nullptr) {
            return {};
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
        return 特征值_->读取容器版本(特征值节点);
    }

    bool 特征值属于特征(节点句柄 特征节点, 节点句柄 特征值节点) const {
        return 关系_ != nullptr
            && 节点类型匹配(特征节点, 节点类型::特征)
            && 节点类型匹配(特征值节点, 节点类型::特征值)
            && 关系_->存在关系(关系类型::归属, 特征节点, 特征值节点);
    }

private:
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

    std::optional<节点句柄> 读取宿主当前特征值(节点句柄 槽位节点) const {
        if (关系_ == nullptr || !节点是特征(槽位节点)) {
            return std::nullopt;
        }
        const auto 候选组 = 关系_->获取目标节点组(槽位节点, 关系类型::归属);
        for (const auto& 候选 : 候选组) {
            if (节点类型匹配(候选, 节点类型::特征值)) {
                return 候选;
            }
        }
        return std::nullopt;
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
};

}
