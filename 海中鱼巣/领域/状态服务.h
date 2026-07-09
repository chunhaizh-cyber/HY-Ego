// 文件规则：状态服务区分实例状态和抽象状态；实例状态不得直接作为长期语素对应信息。
#pragma once

#include "../核心/节点仓库.h"
#include "../核心/主信息仓库.h"
#include "../核心/关系仓库.h"
#include "../核心/容错检查.h"

#include <cstdint>
#include <limits>
#include <optional>
#include <vector>

namespace 海中鱼巣 {

struct 状态材料 {
    节点句柄 状态;
    节点句柄 场景;
    节点句柄 主体;
    std::int64_t 状态值 = 0;
    std::uint64_t 发生时间戳 = 0;
};

enum class 实例状态引用阻断原因 : std::uint32_t {
    未阻断 = 0,
    无效状态 = 1,
    非实例状态 = 2,
    已被结构引用 = 3
};

struct 实例状态引用阻断结果 {
    节点句柄 状态;
    实例状态引用阻断原因 原因 = 实例状态引用阻断原因::无效状态;
};

class 状态服务 {
public:
    状态服务(主信息仓库& 主信息, 节点仓库& 节点, 关系仓库& 关系)
        : 主信息_(主信息), 节点_(节点), 关系_(关系) {
    }

    节点句柄 创建实例状态(节点句柄 场景, 节点句柄 存在, std::uint64_t 发生时间戳, std::int64_t 状态值) {
        if (!节点类型匹配(场景, 节点类型::场景) || !节点类型匹配(存在, 节点类型::存在)
            || !发生时间戳可用(发生时间戳)) {
            return {};
        }
        const auto 状态节点 = 创建状态节点(状态值, 发生时间戳);
        if (!追根因检查(句柄有效(状态节点), L"创建实例状态时状态节点创建不及预期。")) {
            return {};
        }
        const auto 场景关系 = 关系_.创建关系(关系类型::运行期临时, 场景, 状态节点);
        const auto 存在关系 = 关系_.创建关系(关系类型::引用, 状态节点, 存在);
        const auto 场景材料关系 = 关系_.创建关系(关系类型::引用, 状态节点, 场景);
        if (!追根因检查(句柄有效(场景关系) && 句柄有效(存在关系) && 句柄有效(场景材料关系),
            L"创建实例状态时运行期临时或引用关系写入不及预期。")) {
            return {};
        }
        const auto 材料 = 读取状态材料(状态节点);
        if (!追根因检查(材料.has_value() && 材料->场景 == 场景 && 材料->主体 == 存在
            && 材料->状态值 == 状态值 && 材料->发生时间戳 == 发生时间戳,
            L"创建实例状态后读回材料不符合内部预期。")) {
            return {};
        }
        return 状态节点;
    }

    节点句柄 创建抽象状态(std::int64_t 状态值) {
        const auto 状态节点 = 创建状态节点(状态值);
        const auto 读回状态值 = 读取状态值(状态节点);
        if (!追根因检查(读回状态值.has_value() && 读回状态值.value() == 状态值 && !状态是否实例状态(状态节点),
            L"创建抽象状态后读回材料不符合内部预期。")) {
            return {};
        }
        return 状态节点;
    }

    节点句柄 提升实例状态(节点句柄 实例状态) {
        const auto 实例记录 = 节点_.读取节点(实例状态);
        if (!实例记录.has_value() || 实例记录->类型 != 节点类型::状态
            || !关系_.存在目标关系(关系类型::运行期临时, 实例状态)) {
            return {};
        }
        const auto 状态值 = 主信息_.读取I64值(实例记录->主信息);
        if (!状态值.has_value()) {
            return {};
        }
        const auto 抽象状态 = 创建抽象状态(状态值.value());
        if (!句柄有效(抽象状态)) {
            return {};
        }
        关系_.创建关系(关系类型::引用, 实例状态, 抽象状态);
        return 抽象状态;
    }

    std::optional<std::int64_t> 读取状态值(节点句柄 状态节点) const {
        const auto 记录 = 节点_.读取节点(状态节点);
        if (!记录.has_value() || 记录->类型 != 节点类型::状态) {
            return std::nullopt;
        }
        return 主信息_.读取I64值(记录->主信息);
    }

    std::optional<std::uint64_t> 读取发生时间戳(节点句柄 状态节点) const {
        const auto 记录 = 节点_.读取节点(状态节点);
        if (!记录.has_value() || 记录->类型 != 节点类型::状态) {
            return std::nullopt;
        }
        const auto 发生时间戳 = 主信息_.读取I64值(记录->主信息, 发生时间戳槽位);
        if (!发生时间戳.has_value() || 发生时间戳.value() <= 0) {
            return std::nullopt;
        }
        return static_cast<std::uint64_t>(发生时间戳.value());
    }

    bool 状态是否实例状态(节点句柄 状态节点) const {
        const auto 记录 = 节点_.读取节点(状态节点);
        return 记录.has_value() && 记录->类型 == 节点类型::状态
            && 关系_.存在目标关系(关系类型::运行期临时, 状态节点);
    }

    std::optional<节点句柄> 读取状态主体(节点句柄 状态节点) const {
        if (!状态是否实例状态(状态节点)) {
            return std::nullopt;
        }
        return 读取唯一引用目标(状态节点, 节点类型::存在);
    }

    std::optional<节点句柄> 读取状态场景(节点句柄 状态节点) const {
        if (!状态是否实例状态(状态节点)) {
            return std::nullopt;
        }
        return 读取唯一引用目标(状态节点, 节点类型::场景);
    }

    std::vector<节点句柄> 读取场景状态(节点句柄 场景) const {
        std::vector<节点句柄> 状态组;
        if (!节点类型匹配(场景, 节点类型::场景)) {
            return 状态组;
        }
        const auto 目标组 = 关系_.获取目标节点组(场景, 关系类型::运行期临时);
        for (const auto& 目标 : 目标组) {
            if (节点类型匹配(目标, 节点类型::状态)) {
                状态组.push_back(目标);
            }
        }
        return 状态组;
    }

    std::optional<状态材料> 读取状态材料(节点句柄 状态节点) const {
        if (!状态是否实例状态(状态节点)) {
            return std::nullopt;
        }
        const auto 主体 = 读取状态主体(状态节点);
        const auto 场景 = 读取状态场景(状态节点);
        const auto 状态值 = 读取状态值(状态节点);
        const auto 时间戳 = 读取发生时间戳(状态节点);
        if (!主体.has_value() || !场景.has_value() || !状态值.has_value() || !时间戳.has_value()) {
            return std::nullopt;
        }
        return 状态材料{状态节点, 场景.value(), 主体.value(), 状态值.value(), 时间戳.value()};
    }

    实例状态引用阻断结果 读取实例状态引用阻断(节点句柄 状态节点) const {
        if (!节点类型匹配(状态节点, 节点类型::状态)) {
            return {状态节点, 实例状态引用阻断原因::无效状态};
        }
        if (!状态是否实例状态(状态节点)) {
            return {状态节点, 实例状态引用阻断原因::非实例状态};
        }
        if (关系_.存在目标关系(关系类型::因果来源, 状态节点)
            || 关系_.存在目标关系(关系类型::引用, 状态节点)) {
            return {状态节点, 实例状态引用阻断原因::已被结构引用};
        }
        return {状态节点, 实例状态引用阻断原因::未阻断};
    }

private:
    static constexpr std::uint64_t 发生时间戳槽位 = 1;

    bool 发生时间戳可用(std::uint64_t 发生时间戳) const {
        return 发生时间戳 != 0
            && 发生时间戳 <= static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max());
    }

    节点句柄 创建状态节点(std::int64_t 状态值, std::optional<std::uint64_t> 发生时间戳 = std::nullopt) {
        if (发生时间戳.has_value() && !发生时间戳可用(发生时间戳.value())) {
            return {};
        }
        const auto 主信息句柄 = 主信息_.创建主信息();
        if (!追根因检查(主信息_.写入I64值(主信息句柄, 状态值), L"创建状态节点时状态值写入不及预期。")) {
            return {};
        }
        if (发生时间戳.has_value()
            && !追根因检查(主信息_.写入I64值(主信息句柄, 发生时间戳槽位, static_cast<std::int64_t>(发生时间戳.value())),
                L"创建状态节点时发生时间戳写入不及预期。")) {
            return {};
        }
        return 节点_.创建节点(节点类型::状态, 主信息句柄);
    }

    bool 节点类型匹配(节点句柄 节点句柄值, 节点类型 类型) const {
        const auto 记录 = 节点_.读取节点(节点句柄值);
        return 记录.has_value() && 记录->类型 == 类型;
    }

    std::optional<节点句柄> 读取唯一引用目标(节点句柄 源节点, 节点类型 目标类型) const {
        const auto 目标组 = 关系_.获取目标节点组(源节点, 关系类型::引用);
        std::optional<节点句柄> 唯一目标;
        for (const auto& 目标 : 目标组) {
            if (!节点类型匹配(目标, 目标类型)) {
                continue;
            }
            if (唯一目标.has_value()) {
                return std::nullopt;
            }
            唯一目标 = 目标;
        }
        return 唯一目标;
    }

    主信息仓库& 主信息_;
    节点仓库& 节点_;
    关系仓库& 关系_;
};

}
