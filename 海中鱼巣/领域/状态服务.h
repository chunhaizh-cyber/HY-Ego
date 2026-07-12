// 文件规则：状态服务区分实例状态和抽象状态；实例状态不得直接作为长期语素对应信息。
#pragma once

#include "../核心/节点仓库.h"
#include "../核心/主信息仓库.h"
#include "../核心/关系仓库.h"
#include "../核心/容错检查.h"
#include "../核心/结构事务接线.数据.h"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <optional>
#include <vector>
#include <array>
#include <utility>

namespace 海中鱼巣 {

class 状态服务;
enum class 状态候选操作状态 : std::uint32_t { 已完成 = 1, 幂等已撤销 = 2, 已拒绝 = 3, 内部不一致 = 4 };
class 状态未发布候选 {
public:
    状态未发布候选() = delete;
    状态未发布候选(const 状态未发布候选&) = delete;
    状态未发布候选& operator=(const 状态未发布候选&) = delete;
    状态未发布候选(状态未发布候选&&) noexcept = default;
    状态未发布候选& operator=(状态未发布候选&&) noexcept = default;
    节点句柄 读取状态() const { return 状态_; }
private:
    friend class 状态服务;
    enum class 阶段 : std::uint8_t { 未发布, 已确认, 已撤销 };
    状态未发布候选(const 状态服务* 服务, 结构事务令牌 令牌, 节点句柄 状态,
        主信息句柄 主信息, std::array<关系句柄, 3> 关系组)
        : 服务_(服务), 令牌_(令牌), 状态_(状态), 主信息_(主信息), 关系组_(关系组) {}
    const 状态服务* 服务_ = nullptr;
    结构事务令牌 令牌_;
    节点句柄 状态_;
    主信息句柄 主信息_;
    std::array<关系句柄, 3> 关系组_{};
    阶段 阶段_ = 阶段::未发布;
};

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

    状态服务(主信息仓库& 主信息, 节点仓库& 节点, 关系仓库& 关系, 结构事务接线 接线)
        : 主信息_(主信息), 节点_(节点), 关系_(关系), 接线_(std::move(接线)) {}

    std::optional<状态未发布候选> 创建实例状态候选(节点句柄 场景, 节点句柄 存在,
        std::uint64_t 时间戳, std::int64_t 值, const 结构事务令牌& 令牌) {
        if (!验证独占(令牌) || !发生时间戳可用(时间戳)) return std::nullopt;
        const auto 场景记录 = 节点_.读取节点(场景, 令牌);
        const auto 存在记录 = 节点_.读取节点(存在, 令牌);
        if (!场景记录 || 场景记录->类型 != 节点类型::场景
            || !存在记录 || 存在记录->类型 != 节点类型::存在) return std::nullopt;
        const auto 主信息 = 主信息_.创建主信息(令牌);
        if (!句柄有效(主信息) || !主信息_.写入I64值(主信息, 0, 值, 令牌)
            || !主信息_.写入I64值(主信息, 发生时间戳槽位, static_cast<std::int64_t>(时间戳), 令牌)) {
            if (句柄有效(主信息)) (void)主信息_.删除主信息(主信息, 令牌);
            return std::nullopt;
        }
        const auto 状态 = 节点_.创建节点(节点类型::状态, 主信息, 令牌);
        std::array<关系句柄, 3> 关系组{};
        if (句柄有效(状态)) {
            关系组[0] = 关系_.创建关系(关系类型::运行期临时, 场景, 状态, 0, 令牌);
            关系组[1] = 关系_.创建关系(关系类型::引用, 状态, 存在, 0, 令牌);
            关系组[2] = 关系_.创建关系(关系类型::引用, 状态, 场景, 0, 令牌);
        }
        if (句柄有效(状态) && std::all_of(关系组.begin(), 关系组.end(), [](auto x){return 句柄有效(x);}))
            return 状态未发布候选{this, 令牌, 状态, 主信息, 关系组};
        for (auto it=关系组.rbegin(); it!=关系组.rend(); ++it) if (句柄有效(*it)) (void)关系_.删除关系(*it,令牌);
        if (句柄有效(状态)) (void)节点_.删除节点(状态,令牌);
        (void)主信息_.删除主信息(主信息,令牌);
        return std::nullopt;
    }

    状态候选操作状态 撤销候选(状态未发布候选& 候选, const 结构事务令牌& 令牌) {
        if (候选.服务_!=this || !令牌匹配(候选.令牌_,令牌) || !验证独占(令牌)) return 状态候选操作状态::已拒绝;
        if (候选.阶段_==状态未发布候选::阶段::已撤销) return 状态候选操作状态::幂等已撤销;
        if (候选.阶段_!=状态未发布候选::阶段::未发布
            || !节点_.读取节点(候选.状态_,令牌) || !主信息_.读取主信息(候选.主信息_,令牌)) return 状态候选操作状态::内部不一致;
        for (const auto& 关系 : 候选.关系组_) if (!关系_.读取关系(关系,令牌)) return 状态候选操作状态::内部不一致;
        bool 完成=true; for(auto it=候选.关系组_.rbegin();it!=候选.关系组_.rend();++it) 完成=关系_.删除关系(*it,令牌)&&完成;
        完成=节点_.删除节点(候选.状态_,令牌)&&完成; 完成=主信息_.删除主信息(候选.主信息_,令牌)&&完成;
        if(!完成) return 状态候选操作状态::内部不一致;
        候选.阶段_=状态未发布候选::阶段::已撤销; return 状态候选操作状态::已完成;
    }
    状态候选操作状态 确认候选(状态未发布候选& 候选,const 结构事务令牌& 令牌){
        if(候选.服务_!=this||!令牌匹配(候选.令牌_,令牌)||!验证独占(令牌)
            ||候选.阶段_!=状态未发布候选::阶段::未发布) return 状态候选操作状态::已拒绝;
        候选.阶段_=状态未发布候选::阶段::已确认; return 状态候选操作状态::已完成;
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
    std::optional<状态材料> 读取状态材料(节点句柄 s,const 结构事务令牌& t) const {
        if(!验证独占(t)) return std::nullopt; auto r=节点_.读取节点(s,t);
        if(!r||r->类型!=节点类型::状态||!关系_.存在目标关系(关系类型::运行期临时,s,t)) return std::nullopt;
        std::optional<节点句柄> p,c; for(auto x:关系_.获取目标节点组(s,关系类型::引用,t)){
            auto n=节点_.读取节点(x,t); if(!n)continue;
            if(n->类型==节点类型::存在){if(p)return std::nullopt;p=x;}
            if(n->类型==节点类型::场景){if(c)return std::nullopt;c=x;}
        }
        auto v=主信息_.读取I64值(r->主信息,0,t); auto tm=主信息_.读取I64值(r->主信息,发生时间戳槽位,t);
        if(!p||!c||!v||!tm||*tm<=0)return std::nullopt; return 状态材料{s,*c,*p,*v,static_cast<std::uint64_t>(*tm)};
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
    bool 验证独占(const 结构事务令牌& 令牌) const { return 接线_.已接域()&&接线_.验证独占令牌(接线_.运行期状态,令牌); }
    static bool 令牌匹配(const 结构事务令牌& a,const 结构事务令牌& b){return a.域编号==b.域编号&&a.运行期纪元==b.运行期纪元&&a.许可序号==b.许可序号&&a.类型==b.类型;}

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
    结构事务接线 接线_;
};

}
