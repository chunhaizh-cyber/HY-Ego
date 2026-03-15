// TaskExecutionModule.ixx
// ============================================================
// 任务执行器
// - 筹办：按需求找“能把当前状态推进到目标状态”的方法
// - 执行：顺序推进当前步骤
// - 命中：实际结果与预测结果按二次特征相等/相似匹配
// ============================================================

export module 任务执行模块;

import <cstdint>;
import <vector>;
import <string>;
import <functional>;
import <algorithm>;
import <unordered_set>;
import <limits>;
import <variant>;

import 日志模块;
import 主信息定义模块;
import 数据仓库模块;
import 任务模块;
import 方法模块;
import 方法环境模块;
import 本能动作管理模块;
import 需求模块;
import 需求环境模块;
import 状态模块;
import 世界树环境模块;
import 二次特征匹配模块;

using namespace 数据仓库模块;
using namespace 二次特征匹配模块;

export enum class 枚举_任务失败类型 : std::uint8_t {
    未定义 = 0,
    无叶子执行回调 = 1,
};

export enum class 枚举_任务反馈动作 : std::uint8_t {
    未定义 = 0,
    继续下一步 = 1,
    回跳重试 = 2,
    更新参数后重试 = 3,
    转入尝试学习 = 4,
    挂起等待 = 5,
};

export enum class 枚举_推荐参数 : std::uint8_t {
    未定义 = 0,
};

namespace 任务执行模块_detail {

    struct 结构_筹办分支 {
        任务节点类* 入口步骤 = nullptr;
        任务节点类* 目标预测结果 = nullptr;
        std::int64_t 评分Q = 0;
    };

    inline std::int64_t 绝对值I64(std::int64_t x) noexcept {
        if (x == std::numeric_limits<std::int64_t>::min()) return std::numeric_limits<std::int64_t>::max();
        return x < 0 ? -x : x;
    }

    inline bool 节点主键相同(const 基础信息节点类* 左, const 基础信息节点类* 右) noexcept {
        if (左 == 右) return true;
        if (!左 || !右) return false;
        return 左->获取主键() == 右->获取主键();
    }

    inline std::vector<任务节点类*> 枚举子节点(任务节点类* 父) {
        std::vector<任务节点类*> out{};
        if (!父) return out;
        auto* first = static_cast<任务节点类*>(父->子);
        if (!first) return out;
        auto* cur = first;
        do {
            out.push_back(cur);
            cur = static_cast<任务节点类*>(cur->下);
        } while (cur && cur != first);
        return out;
    }

    inline bool 是否终结(枚举_任务状态 st) noexcept {
        return st == 枚举_任务状态::完成
            || st == 枚举_任务状态::失败
            || st == 枚举_任务状态::取消
            || st == 枚举_任务状态::超时
            || st == 枚举_任务状态::无法执行;
    }

    inline 任务头结点信息* 取头信息(任务节点类* n) noexcept {
        if (!n || !n->主信息) return nullptr;
        return dynamic_cast<任务头结点信息*>(n->主信息);
    }

    inline 任务步骤节点信息* 取步骤信息(任务节点类* n) noexcept {
        if (!n || !n->主信息) return nullptr;
        return dynamic_cast<任务步骤节点信息*>(n->主信息);
    }

    inline 任务结果节点信息* 取结果信息(任务节点类* n) noexcept {
        if (!n || !n->主信息) return nullptr;
        return dynamic_cast<任务结果节点信息*>(n->主信息);
    }

    inline 场景节点主信息类* 取场景信息(场景节点类* n) noexcept {
        if (!n || !n->主信息) return nullptr;
        return dynamic_cast<场景节点主信息类*>(n->主信息);
    }

    inline 状态节点主信息类* 取状态信息(状态节点类* n) noexcept {
        if (!n || !n->主信息) return nullptr;
        return dynamic_cast<状态节点主信息类*>(n->主信息);
    }

    inline 方法首节点主信息类* 取方法首信息(方法节点类* n) noexcept {
        if (!n || !n->主信息) return nullptr;
        return dynamic_cast<方法首节点主信息类*>(n->主信息);
    }

    inline 方法条件节点主信息类* 取方法条件信息(方法节点类* n) noexcept {
        if (!n || !n->主信息) return nullptr;
        return dynamic_cast<方法条件节点主信息类*>(n->主信息);
    }

    inline 方法结果节点主信息类* 取方法结果信息(方法节点类* n) noexcept {
        if (!n || !n->主信息) return nullptr;
        return dynamic_cast<方法结果节点主信息类*>(n->主信息);
    }

    inline bool 是否可主动推进任务(枚举_任务状态 st) noexcept {
        return st == 枚举_任务状态::未定义
            || st == 枚举_任务状态::未启动
            || st == 枚举_任务状态::就绪
            || st == 枚举_任务状态::运行中
            || st == 枚举_任务状态::筹办中
            || st == 枚举_任务状态::排队中;
    }

    inline bool 是否可进入调度队列(枚举_任务状态 st) noexcept {
        return !是否终结(st)
            && st != 枚举_任务状态::挂起
            && st != 枚举_任务状态::等待中;
    }

    inline 任务头结点信息* 取所属任务头信息(任务节点类* n) noexcept {
        if (auto* h = 取头信息(n)) return h;
        if (auto* s = 取步骤信息(n)) return 取头信息(s->所属任务头结点);
        if (auto* r = 取结果信息(n)) return 取头信息(r->所属任务头结点);
        return nullptr;
    }

    inline 任务头结点信息* 取主任务头信息(任务节点类* n) noexcept {
        auto* h = 取所属任务头信息(n);
        while (h && h->父任务头结点) {
            auto* parent = 取头信息(h->父任务头结点);
            if (!parent || parent == h) break;
            h = parent;
        }
        return h;
    }

    inline std::int32_t 推导关联步骤序号(任务节点类* n) noexcept {
        if (auto* s = 取步骤信息(n)) return s->步骤序号;
        if (auto* r = 取结果信息(n)) {
            if (auto* src = 取步骤信息(r->来源步骤节点)) return src->步骤序号;
        }
        if (auto* h = 取头信息(n)) {
            if (auto* parentStep = 取步骤信息(h->来源父步骤节点)) return parentStep->步骤序号;
            if (auto* parentResult = 取结果信息(h->来源父结果节点)) {
                if (auto* src = 取步骤信息(parentResult->来源步骤节点)) return src->步骤序号;
            }
        }
        return 0;
    }

    inline std::size_t 统计直属步骤数量(任务节点类* 父节点) {
        std::size_t count = 0;
        for (auto* child : 枚举子节点(父节点)) {
            if (取步骤信息(child)) ++count;
        }
        return count;
    }

    inline bool 任务头有可推进直接子任务(const 任务头结点信息* headInfo) {
        if (!headInfo) return false;
        for (auto* childHead : headInfo->直属子任务头结点列表) {
            auto* childInfo = 取头信息(childHead);
            if (!childInfo || !childHead || !childHead->主信息) continue;
            if (是否终结(childInfo->状态)) continue;
            if (是否可主动推进任务(childInfo->状态)) return true;
        }
        return false;
    }

    inline bool 任务头近似是最小叶子任务(任务节点类* 任务头结点, const 任务头结点信息* headInfo) {
        if (!任务头结点 || !headInfo) return false;
        const auto stepCount = 统计直属步骤数量(任务头结点);
        if (stepCount > 1) return false;

        任务节点类* stepNode = headInfo->当前步骤节点;
        if (!stepNode || !取步骤信息(stepNode)) {
            for (auto* child : 枚举子节点(任务头结点)) {
                if (取步骤信息(child)) {
                    stepNode = child;
                    break;
                }
            }
        }

        if (!stepNode) return headInfo->任务树类型 == 枚举_任务树类型::叶子任务;
        auto* stepInfo = 取步骤信息(stepNode);
        if (!stepInfo) return false;
        return stepInfo->并发子任务头结点列表.empty();
    }

    inline std::int64_t 取优先级(任务节点类* 节点) noexcept {
        if (!节点 || !节点->主信息) return 0;

        auto* mi = 节点->主信息;
        const auto* 主任务头信息 = 取主任务头信息(节点);
        const auto 基准优先级 = 主任务头信息 ? 主任务头信息->基准优先级 : mi->基准优先级;
        std::int64_t score = 基准优先级 * 1000000LL + mi->局部优先级偏移;

        switch (mi->状态) {
        case 枚举_任务状态::就绪:
            score += 20000;
            break;
        case 枚举_任务状态::运行中:
        case 枚举_任务状态::筹办中:
            score += 10000;
            break;
        case 枚举_任务状态::执行中:
            score += 5000;
            break;
        case 枚举_任务状态::等待中:
        case 枚举_任务状态::挂起:
            score -= 900000;
            break;
        default:
            break;
        }

        if (auto* h = 取头信息(节点)) {
            if (h->父任务头结点) score += 300000;
            score += std::max<std::int64_t>(0, 80000 - static_cast<std::int64_t>(推导关联步骤序号(节点)) * 2000);
            if (mi->任务树类型 == 枚举_任务树类型::叶子任务) score += 120000;
            if (任务头近似是最小叶子任务(节点, h)) score += 150000;
            if (任务头有可推进直接子任务(h)) score -= 400000;
        }
        else if (auto* s = 取步骤信息(节点)) {
            score += std::max<std::int64_t>(0, 120000 - static_cast<std::int64_t>(s->步骤序号) * 4000);
            if (s->并发子任务头结点列表.empty()) score += 120000;
            if (s->当前选中方法首节点 || !s->可用方法首节点列表.empty()) score += 20000;
        }
        else if (auto* r = 取结果信息(节点)) {
            score += std::max<std::int64_t>(0, 40000 - static_cast<std::int64_t>(推导关联步骤序号(节点)) * 1000);
            if (r->结果角色 == 枚举_任务结果角色::实际结果) score -= 20000;
        }

        mi->调度优先级 = score;
        return score;
    }

    inline std::vector<方法节点类*> 枚举方法子节点(方法节点类* 父) {
        std::vector<方法节点类*> out{};
        if (!父) return out;
        auto* first = static_cast<方法节点类*>(父->子);
        if (!first) return out;
        auto* cur = first;
        do {
            out.push_back(cur);
            cur = static_cast<方法节点类*>(cur->下);
        } while (cur && cur != first);
        return out;
    }

    inline std::vector<方法节点类*> 枚举全部方法首节点() {
        std::vector<方法节点类*> out{};
        auto* root = 方法链.根指针;
        if (!root || !root->子) return out;
        auto* first = static_cast<方法节点类*>(root->子);
        auto* cur = first;
        do {
            if (取方法首信息(cur)) out.push_back(cur);
            cur = static_cast<方法节点类*>(cur->下);
        } while (cur && cur != first);
        return out;
    }

    inline std::vector<方法节点类*> 枚举方法直接条件节点(方法节点类* 方法首节点) {
        std::vector<方法节点类*> out{};
        for (auto* child : 枚举方法子节点(方法首节点)) {
            if (取方法条件信息(child)) out.push_back(child);
        }
        return out;
    }

    inline std::vector<方法节点类*> 枚举方法树结果节点(方法节点类* 方法首节点) {
        std::vector<方法节点类*> out{};
        if (!方法首节点) return out;

        std::function<void(方法节点类*)> dfs = [&](方法节点类* node) {
            if (!node) return;
            if (取方法结果信息(node)) out.push_back(node);
            for (auto* child : 枚举方法子节点(node)) dfs(child);
        };
        dfs(方法首节点);
        return out;
    }

    inline std::vector<状态节点类*> 枚举场景状态(场景节点类* 场景) {
        if (auto* smi = 取场景信息(场景)) return smi->状态列表;
        return {};
    }

    inline std::vector<const 二次特征节点类*> 枚举场景二次特征指针(场景节点类* 场景) {
        std::vector<const 二次特征节点类*> out{};
        auto* smi = 取场景信息(场景);
        if (!smi) return out;
        out.reserve(smi->关系列表.size());
        for (auto* node : smi->关系列表) {
            if (node && node->主信息) out.push_back(node);
        }
        return out;
    }

    inline 状态节点类* 查找场景状态_按目标特征(场景节点类* 场景, 状态节点类* 目标状态) {
        auto* 目标信息 = 取状态信息(目标状态);
        if (!目标信息 || !目标信息->状态特征) return nullptr;

        状态节点类* best = nullptr;
        时间戳 bestTs = 0;
        for (auto* state : 枚举场景状态(场景)) {
            auto* smi = 取状态信息(state);
            if (!smi || !smi->状态特征) continue;
            if (!节点主键相同(smi->状态特征, 目标信息->状态特征)) continue;
            if (!best || smi->发生时间 >= bestTs) {
                best = state;
                bestTs = smi->发生时间;
            }
        }
        return best;
    }

    inline 状态节点类* 查找方法结果状态_按目标特征(方法节点类* 方法结果节点, 状态节点类* 目标状态) {
        auto* rmi = 取方法结果信息(方法结果节点);
        if (!rmi) return nullptr;
        return 查找场景状态_按目标特征(&rmi->场景根节点, 目标状态);
    }

    inline std::vector<const 二次特征节点类*> 取结果摘要指针集(方法节点类* 方法结果节点) {
        std::vector<const 二次特征节点类*> out{};
        auto* rmi = 取方法结果信息(方法结果节点);
        if (!rmi) return out;
        out.reserve(rmi->结果摘要索引.size());
        for (auto& sec : rmi->结果摘要索引) out.push_back(&sec);
        return out;
    }

    inline std::int64_t 评分_方法结果满足需求(
        场景节点类* 当前场景,
        状态节点类* 目标状态,
        方法节点类* 方法结果节点)
    {
        auto* 目标信息 = 取状态信息(目标状态);
        auto* 预测状态 = 查找方法结果状态_按目标特征(方法结果节点, 目标状态);
        auto* 预测信息 = 取状态信息(预测状态);
        if (!目标信息 || !预测信息 || !目标信息->状态特征 || !预测信息->状态特征) return -1;
        if (!节点主键相同(目标信息->状态特征, 预测信息->状态特征)) return -1;

        auto* 当前状态 = 查找场景状态_按目标特征(当前场景, 目标状态);
        auto* 当前信息 = 取状态信息(当前状态);
        const auto& 目标值 = 目标信息->状态值;
        const auto& 预测值 = 预测信息->状态值;
        if (目标值.index() != 预测值.index()) return -1;

        std::int64_t score = 10000;
        if (目标信息->状态主体 && 预测信息->状态主体 && 节点主键相同(目标信息->状态主体, 预测信息->状态主体)) {
            score += 2000;
        }

        if (std::holds_alternative<I64>(目标值) && std::holds_alternative<I64>(预测值)) {
            const auto target = std::get<I64>(目标值);
            const auto predicted = std::get<I64>(预测值);
            const auto predictedGap = 绝对值I64(target - predicted);
            score += std::max<std::int64_t>(0, 10000 - std::min<std::int64_t>(10000, predictedGap));

            if (当前信息 && std::holds_alternative<I64>(当前信息->状态值)) {
                const auto current = std::get<I64>(当前信息->状态值);
                const auto currentGap = 绝对值I64(target - current);
                if (currentGap == 0) return -1;
                if (predictedGap > currentGap) return -1;

                const auto targetDelta = target - current;
                const auto predictedDelta = predicted - current;
                if (targetDelta != 0 && predictedDelta != 0 && ((targetDelta > 0) != (predictedDelta > 0))) {
                    return -1;
                }
                if (predictedGap < currentGap) score += 10000;
                if (predictedGap == 0) score += 10000;
            }
            else if (predictedGap == 0) {
                score += 12000;
            }
            return score;
        }

        if (预测值 == 目标值) return score + 20000;
        return -1;
    }

    inline bool 当前条件满足方法(
        方法节点类* 方法首节点,
        const std::vector<const 二次特征节点类*>& 当前条件特征)
    {
        auto conditions = 枚举方法直接条件节点(方法首节点);
        if (conditions.empty()) return true;

        for (auto* condNode : conditions) {
            auto* cmi = 取方法条件信息(condNode);
            if (!cmi) continue;
            if (cmi->条件判定索引.empty()) return true;
            I64 hit = 0;
            if (条件索引_满足(cmi->条件判定索引, 当前条件特征, &hit)) return true;
        }
        return false;
    }

    inline bool 状态值精确相等(const 特征快照值& 左, const 特征快照值& 右) noexcept {
        if (左.index() != 右.index()) return false;
        return 左 == 右;
    }

    inline 状态节点类* 查找场景状态_按目标模板(场景节点类* 场景, 状态节点类* 目标状态) {
        auto* 目标信息 = 取状态信息(目标状态);
        if (!目标信息 || !目标信息->状态特征) return nullptr;

        状态节点类* best = nullptr;
        时间戳 bestTs = 0;
        for (auto* state : 枚举场景状态(场景)) {
            auto* smi = 取状态信息(state);
            if (!smi || !smi->状态特征) continue;
            if (!节点主键相同(smi->状态特征, 目标信息->状态特征)) continue;
            if (目标信息->状态主体 && smi->状态主体 && !节点主键相同(smi->状态主体, 目标信息->状态主体)) continue;
            const auto ts = smi->发生时间 ? smi->发生时间 : smi->收到时间;
            if (!best || ts >= bestTs) {
                best = state;
                bestTs = ts;
            }
        }
        return best;
    }

    inline bool 场景已满足目标状态(场景节点类* 场景, 状态节点类* 目标状态) {
        auto* 目标信息 = 取状态信息(目标状态);
        auto* 当前状态 = 查找场景状态_按目标模板(场景, 目标状态);
        auto* 当前信息 = 取状态信息(当前状态);
        if (!目标信息 || !当前信息) return false;
        return 状态值精确相等(目标信息->状态值, 当前信息->状态值);
    }

    enum class 枚举_条件判定类别 : std::uint8_t {
        未定义 = 0,
        已满足 = 1,
        值未知需观察 = 2,
        未满足但可补 = 3,
        不可获取 = 4,
        无权限 = 5,
    };

    struct 结构_条件判定项 {
        状态节点类* 目标状态 = nullptr;
        状态节点类* 场景命中状态 = nullptr;
        枚举_条件判定类别 判定 = 枚举_条件判定类别::未定义;
        bool 来自世界树当前值 = false;
        bool 权限由兜底规则推定 = false;
    };

    inline bool 目标状态被规则禁止使用(const 状态节点主信息类* 目标信息) noexcept {
        // V1: 规则禁止仍未独立建模，先预留统一拦截口。
        (void)目标信息;
        return false;
    }

    inline bool 目标状态兜底可使用(const 状态节点主信息类* 目标信息, 结构_条件判定项* 明细 = nullptr) noexcept {
        if (明细) 明细->权限由兜底规则推定 = false;
        if (!目标信息) return false;

        auto* self = 世界树.自我指针;
        if (目标信息->状态主体 && self && 节点主键相同(目标信息->状态主体, self)) {
            return true;
        }

        if (目标状态被规则禁止使用(目标信息)) {
            if (明细) 明细->权限由兜底规则推定 = true;
            return false;
        }

        // V1 兜底：初始自我还分不清自我/他者时，默认允许尝试使用；
        // 后续再由学习出的归属/授权/禁止规则覆盖。
        if (明细) 明细->权限由兜底规则推定 = true;
        return true;
    }

    inline 枚举_条件判定类别 判定目标状态可用性(
        场景节点类* 当前场景,
        状态节点类* 目标状态,
        结构_条件判定项* 明细 = nullptr)
    {
        if (明细) {
            明细->目标状态 = 目标状态;
            明细->场景命中状态 = nullptr;
            明细->判定 = 枚举_条件判定类别::未定义;
            明细->来自世界树当前值 = false;
            明细->权限由兜底规则推定 = false;
        }

        auto* 目标信息 = 取状态信息(目标状态);
        if (!目标信息 || !目标信息->状态主体 || !目标信息->状态特征 || !目标信息->状态特征->主信息) {
            if (明细) 明细->判定 = 枚举_条件判定类别::不可获取;
            return 枚举_条件判定类别::不可获取;
        }

        if (!目标状态兜底可使用(目标信息, 明细)) {
            if (明细) 明细->判定 = 枚举_条件判定类别::无权限;
            return 枚举_条件判定类别::无权限;
        }

        auto* 当前状态 = 查找场景状态_按目标模板(当前场景, 目标状态);
        auto* 当前信息 = 取状态信息(当前状态);
        if (当前信息 && 状态值精确相等(目标信息->状态值, 当前信息->状态值)) {
            if (明细) {
                明细->场景命中状态 = 当前状态;
                明细->判定 = 枚举_条件判定类别::已满足;
            }
            return 枚举_条件判定类别::已满足;
        }

        auto* 特征主信息 = dynamic_cast<特征节点主信息类*>(目标信息->状态特征->主信息);
        if (!特征主信息 || !特征主信息->类型) {
            if (明细) 明细->判定 = 枚举_条件判定类别::不可获取;
            return 枚举_条件判定类别::不可获取;
        }

        const auto 当前快照 = 世界树.读取特征快照(
            目标信息->状态主体,
            特征主信息->类型,
            "任务执行模块_detail::判定目标状态可用性/读世界树当前值");
        if (!当前快照.has_value() || !特征快照有值(*当前快照)) {
            if (明细) 明细->判定 = 枚举_条件判定类别::值未知需观察;
            return 枚举_条件判定类别::值未知需观察;
        }

        if (状态值精确相等(目标信息->状态值, *当前快照)) {
            if (明细) {
                明细->判定 = 枚举_条件判定类别::已满足;
                明细->来自世界树当前值 = true;
            }
            return 枚举_条件判定类别::已满足;
        }

        if (明细) {
            明细->判定 = 枚举_条件判定类别::未满足但可补;
            明细->来自世界树当前值 = true;
        }
        return 枚举_条件判定类别::未满足但可补;
    }

    struct 结构_条件展开结果 {
        方法节点类* 条件节点 = nullptr;
        std::vector<状态节点类*> 未满足状态{};
        std::vector<状态节点类*> 待观察状态{};
        std::vector<状态节点类*> 不可获取状态{};
        std::vector<状态节点类*> 无权限状态{};
        std::vector<结构_条件判定项> 条件判定清单{};
        std::int64_t 命中条件数 = 0;
        std::int64_t 条件总数 = 0;
        bool 当前可直接执行 = true;
    };

    struct 结构_筹办方法候选 {
        方法节点类* 方法首节点 = nullptr;
        方法节点类* 匹配结果节点 = nullptr;
        方法节点类* 首选条件节点 = nullptr;
        std::vector<状态节点类*> 未满足条件状态{};
        std::vector<状态节点类*> 待观察条件状态{};
        std::vector<状态节点类*> 不可获取条件状态{};
        std::vector<状态节点类*> 无权限条件状态{};
        std::vector<结构_条件判定项> 条件判定清单{};
        std::int64_t 评分Q = 0;
        bool 可直接执行 = true;
    };

    inline 结构_条件展开结果 分析方法条件缺口(
        方法节点类* 方法首节点,
        场景节点类* 当前场景,
        const std::vector<const 二次特征节点类*>& 当前条件特征)
    {
        结构_条件展开结果 best{};
        auto conditions = 枚举方法直接条件节点(方法首节点);
        if (conditions.empty()) return best;

        best.当前可直接执行 = false;
        std::int64_t bestScore = std::numeric_limits<std::int64_t>::min();

        for (auto* condNode : conditions) {
            auto* cmi = 取方法条件信息(condNode);
            if (!cmi) continue;

            auto* condSceneInfo = 取场景信息(&cmi->场景根节点);
            std::vector<状态节点类*> condStates{};
            if (condSceneInfo) condStates = condSceneInfo->状态列表;

            I64 hit = 0;
            const bool 满足索引 = cmi->条件判定索引.empty() ? true : 条件索引_满足(cmi->条件判定索引, 当前条件特征, &hit);

            std::vector<状态节点类*> unmet{};
            std::vector<状态节点类*> 待观察{};
            std::vector<状态节点类*> 不可获取{};
            std::vector<状态节点类*> 无权限{};
            std::vector<结构_条件判定项> 清单{};
            unmet.reserve(condStates.size());
            待观察.reserve(condStates.size());
            不可获取.reserve(condStates.size());
            无权限.reserve(condStates.size());
            清单.reserve(condStates.size());
            for (auto* targetState : condStates) {
                if (!targetState) continue;
                结构_条件判定项 判定项{};
                const auto 判定 = 判定目标状态可用性(当前场景, targetState, &判定项);
                清单.push_back(判定项);
                if (判定 == 枚举_条件判定类别::未满足但可补) {
                    unmet.push_back(targetState);
                }
                else if (判定 == 枚举_条件判定类别::值未知需观察) {
                    待观察.push_back(targetState);
                }
                else if (判定 == 枚举_条件判定类别::不可获取) {
                    不可获取.push_back(targetState);
                }
                else if (判定 == 枚举_条件判定类别::无权限) {
                    无权限.push_back(targetState);
                }
            }

            const auto total = static_cast<std::int64_t>(
                cmi->条件判定索引.empty()
                ? condStates.size()
                : cmi->条件判定索引.size());
            const bool 当前可直接执行 = 满足索引 && unmet.empty() && 待观察.empty() && 不可获取.empty() && 无权限.empty();

            std::int64_t score = hit * 10000;
            score -= static_cast<std::int64_t>(unmet.size()) * 4000;
            score -= static_cast<std::int64_t>(待观察.size()) * 2500;
            score -= static_cast<std::int64_t>(不可获取.size()) * 8000;
            score -= static_cast<std::int64_t>(无权限.size()) * 12000;
            score -= std::max<std::int64_t>(0, total - hit) * 1000;
            if (当前可直接执行) score += 50000;

            if (!best.条件节点 || 当前可直接执行 || score > bestScore) {
                best.条件节点 = condNode;
                best.未满足状态 = std::move(unmet);
                best.待观察状态 = std::move(待观察);
                best.不可获取状态 = std::move(不可获取);
                best.无权限状态 = std::move(无权限);
                best.条件判定清单 = std::move(清单);
                best.命中条件数 = hit;
                best.条件总数 = total;
                best.当前可直接执行 = 当前可直接执行;
                bestScore = score;
            }

            if (当前可直接执行) return best;
        }

        return best;
    }

    inline 任务节点类* 选择首选步骤节点(任务节点类* 父节点) {
        任务节点类* best = nullptr;
        std::int64_t bestScore = std::numeric_limits<std::int64_t>::min();
        for (auto* child : 枚举子节点(父节点)) {
            auto* stepInfo = 取步骤信息(child);
            if (!stepInfo || !child || !child->主信息) continue;
            if (!是否可进入调度队列(stepInfo->状态)) continue;

            const auto score = 取优先级(child);
            if (!best || score > bestScore) {
                best = child;
                bestScore = score;
            }
        }
        return best;
    }

    struct 结构_步骤子任务汇总 {
        std::size_t 总数 = 0;
        std::size_t 完成数 = 0;
        std::size_t 失败数 = 0;
        std::size_t 无法执行数 = 0;
        std::size_t 终结数 = 0;
        任务节点类* 首选可推进子任务 = nullptr;
        任务节点类* 首个未终结子任务 = nullptr;
    };

    inline bool 子任务已清零(const 结构_步骤子任务汇总& x) noexcept {
        return x.总数 > 0 && x.终结数 == x.总数;
    }

    inline 结构_步骤子任务汇总 汇总步骤子任务(const 任务步骤节点信息* stepInfo) {
        结构_步骤子任务汇总 out{};
        if (!stepInfo) return out;

        for (auto* childHead : stepInfo->并发子任务头结点列表) {
            auto* childInfo = 取头信息(childHead);
            if (!childInfo || !childHead->主信息) continue;

            ++out.总数;
            if (childInfo->状态 == 枚举_任务状态::完成) {
                ++out.完成数;
                ++out.终结数;
                continue;
            }
            if (childInfo->状态 == 枚举_任务状态::无法执行) {
                ++out.无法执行数;
                ++out.终结数;
                continue;
            }
            if (是否终结(childInfo->状态)) {
                ++out.失败数;
                ++out.终结数;
                continue;
            }

            if (!out.首个未终结子任务) out.首个未终结子任务 = childHead;
            if (是否可主动推进任务(childInfo->状态)) {
                if (!out.首选可推进子任务 || 取优先级(childHead) > 取优先级(out.首选可推进子任务)) {
                    out.首选可推进子任务 = childHead;
                }
            }
        }
        return out;
    }

    inline std::vector<任务节点类*> 枚举步骤下结果节点(任务节点类* 步骤节点) {
        std::vector<任务节点类*> out{};
        for (auto* child : 枚举子节点(步骤节点)) {
            if (取结果信息(child)) out.push_back(child);
        }
        return out;
    }

    inline std::vector<任务节点类*> 枚举预测结果节点(任务节点类* 步骤节点) {
        std::vector<任务节点类*> out{};
        for (auto* resultNode : 枚举步骤下结果节点(步骤节点)) {
            auto* info = 取结果信息(resultNode);
            if (info && info->结果角色 == 枚举_任务结果角色::预测结果) out.push_back(resultNode);
        }
        return out;
    }

    inline 任务节点类* 选择首选预测结果节点(任务节点类* 步骤节点, 方法节点类* 优先方法首节点 = nullptr) {
        任务节点类* fallback = nullptr;
        for (auto* resultNode : 枚举预测结果节点(步骤节点)) {
            auto* info = 取结果信息(resultNode);
            if (!info) continue;
            if (!fallback) fallback = resultNode;
            if (优先方法首节点 && info->来源方法首节点 == 优先方法首节点) return resultNode;
        }
        return fallback;
    }

    inline 任务节点类* 选择步骤中的预测结果_按方法结果(任务节点类* 步骤节点, 方法节点类* 目标方法结果节点) {
        if (!目标方法结果节点) return nullptr;
        for (auto* resultNode : 枚举预测结果节点(步骤节点)) {
            auto* info = 取结果信息(resultNode);
            if (info && info->对应方法结果节点 == 目标方法结果节点) return resultNode;
        }
        return nullptr;
    }

    inline 任务节点类* 选择命中预测结果节点(
        任务节点类* 步骤节点,
        场景节点类* 实际结果场景,
        方法节点类* 来源方法首节点)
    {
        auto actualFeatures = 枚举场景二次特征指针(实际结果场景);
        任务节点类* best = nullptr;
        任务节点类* methodFallback = nullptr;
        std::int64_t bestScore = -1;

        for (auto* resultNode : 枚举预测结果节点(步骤节点)) {
            auto* info = 取结果信息(resultNode);
            if (!info) continue;
            if (!methodFallback && 来源方法首节点 && info->来源方法首节点 == 来源方法首节点) {
                methodFallback = resultNode;
            }

            std::int64_t score = -1;
            if (info->对应方法结果节点) {
                if (auto* rmi = 取方法结果信息(info->对应方法结果节点)) {
                    auto match = 结果摘要_匹配目标(rmi->结果摘要索引, actualFeatures);
                    score = match.得分_Q10000;
                    if (来源方法首节点 && info->来源方法首节点 == 来源方法首节点) score += 50;
                }
            }
            else if (来源方法首节点 && info->来源方法首节点 == 来源方法首节点) {
                score = 1;
            }

            if (score > bestScore) {
                bestScore = score;
                best = resultNode;
            }
        }

        if (best && bestScore > 0) return best;
        if (methodFallback) return methodFallback;
        return 选择首选预测结果节点(步骤节点, 来源方法首节点);
    }

    inline 任务节点类* 取命中结果后的下一步骤(任务节点类* 预测结果节点) {
        return 选择首选步骤节点(预测结果节点);
    }

    inline 枚举_分支选择动作 从反馈动作映射处置(bool 成功, 枚举_任务反馈动作 动作) {
        if (成功) return 枚举_分支选择动作::继续下一步;
        switch (动作) {
        case 枚举_任务反馈动作::回跳重试:
        case 枚举_任务反馈动作::更新参数后重试:
            return 枚举_分支选择动作::回跳重试;
        case 枚举_任务反馈动作::转入尝试学习:
            return 枚举_分支选择动作::转入尝试学习;
        case 枚举_任务反馈动作::挂起等待:
            return 枚举_分支选择动作::挂起等待;
        default:
            return 枚举_分支选择动作::回跳重试;
        }
    }

} // namespace 任务执行模块_detail

export class 任务执行器 {
public:
    struct 结构_叶子执行结果 {
        bool 成功 = false;
        场景节点类* 结果场景 = nullptr;
        std::string 摘要{};
        枚举_任务失败类型 失败类型 = 枚举_任务失败类型::未定义;
        枚举_任务反馈动作 建议动作 = 枚举_任务反馈动作::未定义;
        枚举_推荐参数 推荐参数 = 枚举_推荐参数::未定义;
        I64 推荐参数增量 = 0;
        I64 缺口_Q10000 = 0;
        枚举_任务树类型 推荐重试任务树类型 = 枚举_任务树类型::未定义;
        std::uintptr_t 失败对象指针 = 0;
    };

    using Fn_叶子执行 = std::function<结构_叶子执行结果(任务节点类* 步骤节点)>;
    using Fn_方法规避过滤 = std::function<bool(任务节点类* 任务头结点, 方法节点类* 方法首节点)>;

    struct 结构_配置 {
        std::int64_t 单步最大递归深度 = 0;
        std::int64_t 回跳重试上限 = 3;
        std::size_t 自动召回候选上限 = 5;
    };

public:
    explicit 任务执行器(结构_配置 cfg = {})
        : cfg_(cfg) {
    }

    void 设置叶子执行回调(Fn_叶子执行 fn) { leaf_exec_ = std::move(fn); }
    void 设置尝试学习任务(任务节点类* 学习头) { try_learn_head_ = 学习头; }
    void 设置方法规避过滤(Fn_方法规避过滤 fn) { avoid_filter_ = std::move(fn); }

    任务节点类* 仅筹办一步(任务节点类* 任务头结点) {
        using namespace 任务执行模块_detail;

        auto* headInfo = 取头信息(任务头结点);
        if (!headInfo || !任务头结点->主信息) return nullptr;
        if (是否终结(headInfo->状态)) return nullptr;

        auto* stepNode = 私有_选择当前步骤(任务头结点);
        if (!stepNode || 私有_步骤需要自动筹办(stepNode)) {
            stepNode = 私有_自动筹办任务(任务头结点);
            return stepNode;
        }

        私有_补全步骤预测结果(stepNode);
        (void)私有_确保步骤补条件子任务(任务头结点, stepNode);
        (void)任务类::设置任务当前步骤(任务头结点->主信息, stepNode, 结构体_时间戳::当前_微秒(), "任务执行器::仅筹办一步/设置当前步骤");
        return stepNode;
    }


    bool 推进一步(任务节点类* 任务头结点, 结构_任务返回消息* 返回消息 = nullptr) {
        using namespace 任务执行模块_detail;

        auto* headInfo = 取头信息(任务头结点);
        if (!headInfo || !任务头结点->主信息) return false;
        if (是否终结(headInfo->状态)) return false;

        日志::运行f(
            "[任务执行器] 推进一步开始: 任务={}, 状态={}, 当前步骤={}",
            (void*)任务头结点,
            static_cast<int>(headInfo->状态),
            (void*)headInfo->当前步骤节点);

        // 一旦任务重新进入单步推进，就清掉“等待学习唤醒”标记。
        headInfo->等待学习唤醒 = false;
        headInfo->等待学习方法首节点 = nullptr;

        auto* stepNode = 私有_选择当前步骤(任务头结点);
        日志::运行f(
            "[任务执行器] 推进一步选中步骤: 任务={}, 步骤={}, 需自动筹办={}",
            (void*)任务头结点,
            (void*)stepNode,
            stepNode ? (私有_步骤需要自动筹办(stepNode) ? 1 : 0) : 1);
        if (!stepNode || 私有_步骤需要自动筹办(stepNode)) {
            stepNode = 私有_自动筹办任务(任务头结点);
            日志::运行f(
                "[任务执行器] 推进一步自动筹办结果: 任务={}, 步骤={}",
                (void*)任务头结点,
                (void*)stepNode);
            if (!stepNode) {
                if (headInfo->是否真根任务) {
                    任务类::设置任务状态(任务头结点, 枚举_任务状态::待重筹办, "任务执行器::推进一步/真根任务筹办失败待重筹办");
                    (void)私有_写筹办失败返回消息(
                        任务头结点,
                        枚举_任务返回消息类型::步骤无法执行,
                        true,
                        "当前未能筹办出可执行步骤，等待后续重筹办",
                        返回消息);
                    return true;
                }

                任务类::设置任务状态(任务头结点, 枚举_任务状态::无法执行, "任务执行器::推进一步/普通任务筹办失败无法执行");
                (void)私有_写筹办失败返回消息(
                    任务头结点,
                    枚举_任务返回消息类型::步骤无法执行,
                    false,
                    "当前未能筹办出可执行步骤，任务进入无法执行",
                    返回消息);
                return true;
            }
        }

        if (headInfo->状态 == 枚举_任务状态::未定义
            || headInfo->状态 == 枚举_任务状态::未启动
            || headInfo->状态 == 枚举_任务状态::就绪
            || headInfo->状态 == 枚举_任务状态::筹办中) {
            任务类::设置任务状态(任务头结点, 枚举_任务状态::运行中, "任务执行器::推进一步/置运行中");
        }
        (void)任务类::标记任务最近调度(任务头结点->主信息, 结构体_时间戳::当前_微秒(), "任务执行器::推进一步/标记最近调度");
        return 私有_执行步骤(任务头结点, stepNode, 返回消息);
    }

private:
    任务节点类* 私有_选择当前步骤(任务节点类* 任务头结点) {
        using namespace 任务执行模块_detail;

        auto* headInfo = 取头信息(任务头结点);
        if (!headInfo) return nullptr;
        if (headInfo->当前步骤节点) {
            auto* currentInfo = 取步骤信息(headInfo->当前步骤节点);
            if (currentInfo && !是否终结(currentInfo->状态)) {
                return headInfo->当前步骤节点;
            }
        }

        auto* best = 选择首选步骤节点(任务头结点);
        if (best) {
            (void)任务类::设置任务调度优先级(best->主信息, best->主信息->调度优先级, 结构体_时间戳::当前_微秒(), "任务执行器::选择当前步骤/刷新优先级");
            (void)任务类::设置任务当前步骤(任务头结点->主信息, best, 结构体_时间戳::当前_微秒(), "任务执行器::选择当前步骤");
        }
        return best;
    }

    bool 私有_写筹办失败返回消息(
        任务节点类* 任务头结点,
        枚举_任务返回消息类型 类型,
        bool 需要重筹办,
        const std::string& 摘要,
        结构_任务返回消息* 返回消息) const
    {
        if (!返回消息 || !任务头结点 || !任务头结点->主信息) return false;

        auto* 头信息 = dynamic_cast<任务头结点信息*>(任务头结点->主信息);
        返回消息->类型 = 类型;
        返回消息->来源任务 = 任务头结点;
        返回消息->来源步骤 = nullptr;
        返回消息->相关子任务 = nullptr;
        返回消息->对应需求 = 头信息 ? 头信息->需求 : nullptr;
        返回消息->需求已满足 = false;
        返回消息->已进入下一阶段 = false;
        返回消息->需要重筹办 = 需要重筹办;
        返回消息->摘要 = 摘要;
        return true;
    }

    bool 私有_步骤需要自动筹办(任务节点类* 步骤节点) {
        using namespace 任务执行模块_detail;
        auto* stepInfo = 取步骤信息(步骤节点);
        if (!stepInfo) return true;
        if (stepInfo->当前选中方法首节点) return false;
        if (!stepInfo->可用方法首节点列表.empty()) return false;
        return true;
    }

    std::vector<任务执行模块_detail::结构_筹办方法候选> 私有_召回可筹办方法(任务节点类* 任务头结点) {
        using namespace 任务执行模块_detail;

        std::vector<结构_筹办方法候选> out{};
        auto* headInfo = 取头信息(任务头结点);
        if (!headInfo || !headInfo->需求 || !headInfo->需求->主信息) return out;
        auto* 目标状态 = headInfo->需求->主信息->被需求状态;
        if (!目标状态) return out;

        auto 当前条件特征 = 枚举场景二次特征指针(headInfo->场景);
        auto 追加候选 = [&](方法节点类* 方法首节点, 方法节点类* 匹配结果节点, std::int64_t 基础评分) {
            if (!方法首节点) return;
            auto 条件展开 = 分析方法条件缺口(方法首节点, headInfo->场景, 当前条件特征);
            if (!条件展开.不可获取状态.empty() || !条件展开.无权限状态.empty()) return;
            if (!条件展开.当前可直接执行
                && 条件展开.未满足状态.empty()
                && 条件展开.待观察状态.empty()
                && !枚举方法直接条件节点(方法首节点).empty()) return;

            结构_筹办方法候选 c{};
            c.方法首节点 = 方法首节点;
            c.匹配结果节点 = 匹配结果节点;
            c.首选条件节点 = 条件展开.条件节点;
            c.未满足条件状态 = 条件展开.未满足状态;
            c.待观察条件状态 = 条件展开.待观察状态;
            c.不可获取条件状态 = 条件展开.不可获取状态;
            c.无权限条件状态 = 条件展开.无权限状态;
            c.条件判定清单 = 条件展开.条件判定清单;
            c.可直接执行 = 条件展开.当前可直接执行;
            c.评分Q = 基础评分
                + (c.可直接执行 ? 30000 : 0)
                - static_cast<std::int64_t>(c.未满足条件状态.size()) * 5000
                - static_cast<std::int64_t>(c.待观察条件状态.size()) * 3000
                - static_cast<std::int64_t>(c.不可获取条件状态.size()) * 9000
                - static_cast<std::int64_t>(c.无权限条件状态.size()) * 12000;
            out.push_back(std::move(c));
        };

        for (auto* methodHead : 枚举全部方法首节点()) {
            if (!methodHead) continue;
            if (avoid_filter_ && !avoid_filter_(任务头结点, methodHead)) continue;

            结构_方法候选 best{};
            for (auto* resultNode : 枚举方法树结果节点(methodHead)) {
                const auto score = 评分_方法结果满足需求(headInfo->场景, 目标状态, resultNode);
                if (score <= 0) continue;
                if (!best.方法首节点 || score > best.评分Q) {
                    best.方法首节点 = methodHead;
                    best.匹配结果节点 = resultNode;
                    best.评分Q = score;
                    best.命中数量 = 1;
                    best.目标数量 = 1;
                }
            }
            if (best.方法首节点) 追加候选(best.方法首节点, best.匹配结果节点, best.评分Q);
        }

        std::sort(out.begin(), out.end(), [](const auto& a, const auto& b) {
            return a.评分Q > b.评分Q;
        });

        if (out.empty() && !headInfo->目标维度签名.empty()) {
            auto coarseHeads = 方法集.召回_按影响度量签名(
                headInfo->目标维度签名,
                cfg_.自动召回候选上限,
                "任务执行器::召回可筹办方法/粗召回");
            std::int64_t rank = static_cast<std::int64_t>(coarseHeads.size());
            for (auto* methodHead : coarseHeads) {
                if (!methodHead) continue;
                if (avoid_filter_ && !avoid_filter_(任务头结点, methodHead)) continue;
                auto resultNodes = 枚举方法树结果节点(methodHead);
                追加候选(methodHead, resultNodes.empty() ? nullptr : resultNodes.front(), rank > 0 ? rank * 10000 : 1);
                --rank;
            }

            std::sort(out.begin(), out.end(), [](const auto& a, const auto& b) {
                return a.评分Q > b.评分Q;
            });
        }

        if (out.size() > cfg_.自动召回候选上限) out.resize(cfg_.自动召回候选上限);
        return out;
    }

    void 私有_补全步骤预测结果(任务节点类* 步骤节点) {
        using namespace 任务执行模块_detail;

        auto* stepInfo = 取步骤信息(步骤节点);
        if (!stepInfo || !步骤节点->主信息) return;
        日志::运行f(
            "[任务执行器] 补全步骤预测结果开始: 步骤={}, 子首节点={}, 当前方法={}",
            (void*)步骤节点,
            (void*)步骤节点->子,
            (void*)stepInfo->当前选中方法首节点);

        std::vector<方法节点类*> methods = stepInfo->可用方法首节点列表;
        if (methods.empty() && stepInfo->当前选中方法首节点) methods.push_back(stepInfo->当前选中方法首节点);
        if (methods.empty()) return;
        日志::运行f(
            "[任务执行器] 补全步骤预测结果方法集就绪: 步骤={}, 方法数={}",
            (void*)步骤节点,
            static_cast<unsigned long long>(methods.size()));

        std::unordered_set<std::uintptr_t> existingResults{};
        std::unordered_set<std::uintptr_t> existingPlaceholders{};
        日志::运行f(
            "[任务执行器] 补全步骤预测结果扫描已有结果前: 步骤={}",
            (void*)步骤节点);
        for (auto* resultNode : 枚举预测结果节点(步骤节点)) {
            auto* info = 取结果信息(resultNode);
            if (!info) continue;
            if (info->对应方法结果节点) existingResults.insert(reinterpret_cast<std::uintptr_t>(info->对应方法结果节点));
            else if (info->来源方法首节点) existingPlaceholders.insert(reinterpret_cast<std::uintptr_t>(info->来源方法首节点));
        }
        日志::运行f(
            "[任务执行器] 补全步骤预测结果扫描已有结果后: 步骤={}, 已有结果数={}, 已有占位数={}",
            (void*)步骤节点,
            static_cast<unsigned long long>(existingResults.size()),
            static_cast<unsigned long long>(existingPlaceholders.size()));

        for (auto* methodHead : methods) {
            if (!methodHead) continue;
            日志::运行f(
                "[任务执行器] 补全步骤预测结果开始枚举方法结果: 步骤={}, 方法={}",
                (void*)步骤节点,
                (void*)methodHead);
            auto resultNodes = 枚举方法树结果节点(methodHead);
            日志::运行f(
                "[任务执行器] 补全步骤预测结果完成枚举方法结果: 步骤={}, 方法={}, 方法结果数={}",
                (void*)步骤节点,
                (void*)methodHead,
                static_cast<unsigned long long>(resultNodes.size()));
            if (resultNodes.empty()) {
                const auto key = reinterpret_cast<std::uintptr_t>(methodHead);
                if (!existingPlaceholders.insert(key).second) continue;

                任务类::结构_创建结果参数 arg{};
                arg.所属任务头结点 = stepInfo->所属任务头结点;
                arg.结果场景 = nullptr;
                arg.结果角色 = 枚举_任务结果角色::预测结果;
                arg.来源方法首节点 = methodHead;
                arg.命中后动作 = 枚举_分支选择动作::继续下一步;
                arg.局部优先级偏移 = 步骤节点->主信息->局部优先级偏移;
                arg.初始状态 = 枚举_任务状态::就绪;
                日志::运行f(
                    "[任务执行器] 补全步骤预测结果创建占位前: 步骤={}, 方法={}",
                    (void*)步骤节点,
                    (void*)methodHead);
                (void)任务类::创建结果节点(步骤节点, arg, "任务执行器::补全步骤预测结果/占位预测");
                日志::运行f(
                    "[任务执行器] 补全步骤预测结果创建占位后: 步骤={}, 方法={}",
                    (void*)步骤节点,
                    (void*)methodHead);
                continue;
            }

            for (auto* resultNode : resultNodes) {
                const auto key = reinterpret_cast<std::uintptr_t>(resultNode);
                if (!existingResults.insert(key).second) continue;

                auto* rmi = 任务执行模块_detail::取方法结果信息(resultNode);
                任务类::结构_创建结果参数 arg{};
                arg.所属任务头结点 = stepInfo->所属任务头结点;
                arg.结果场景 = rmi ? &rmi->场景根节点 : nullptr;
                arg.结果角色 = 枚举_任务结果角色::预测结果;
                arg.来源方法首节点 = methodHead;
                arg.对应方法结果节点 = resultNode;
                arg.命中后动作 = 枚举_分支选择动作::继续下一步;
                arg.局部优先级偏移 = 步骤节点->主信息->局部优先级偏移;
                arg.初始状态 = 枚举_任务状态::就绪;
                日志::运行f(
                    "[任务执行器] 补全步骤预测结果创建预测前: 步骤={}, 方法={}, 方法结果={}",
                    (void*)步骤节点,
                    (void*)methodHead,
                    (void*)resultNode);
                (void)任务类::创建结果节点(步骤节点, arg, "任务执行器::补全步骤预测结果");
                日志::运行f(
                    "[任务执行器] 补全步骤预测结果创建预测后: 步骤={}, 方法={}, 方法结果={}",
                    (void*)步骤节点,
                    (void*)methodHead,
                    (void*)resultNode);
            }
        }
        日志::运行f(
            "[任务执行器] 补全步骤预测结果完成: 步骤={}",
            (void*)步骤节点);
    }

    std::vector<方法节点类*> 私有_选择观察方法候选(
        状态节点类* 目标状态,
        时间戳 now,
        const std::string& 调用点)
    {
        using namespace 任务执行模块_detail;

        std::vector<方法节点类*> out{};
        auto* stateInfo = 取状态信息(目标状态);
        if (!stateInfo) return out;

        auto 追加本能方法 = [&](枚举_本能动作ID id) {
            if (!本能集.有(id)) return;
            auto* head = 方法集.查找或创建_本能方法首节点(id, now, 调用点);
            if (!head) return;
            if (auto q = 本能集.查询(id)) {
                if (!q->方法信息首节点) {
                    (void)本能集.补全方法信息(id, head);
                }
            }
            if (std::find(out.begin(), out.end(), head) == out.end()) {
                out.push_back(head);
            }
        };

        auto* featureInfo = (stateInfo->状态特征 && stateInfo->状态特征->主信息)
            ? dynamic_cast<特征节点主信息类*>(stateInfo->状态特征->主信息)
            : nullptr;
        const auto* featureType = featureInfo ? featureInfo->类型 : nullptr;
        const std::string featureKey = featureType ? featureType->获取主键() : std::string{};
        const bool 看起来像人类输入 = featureKey.find("人类输入") != std::string::npos;
        const bool 是自我内部状态 = stateInfo->是否内部状态()
            || (stateInfo->状态主体 && 世界树.自我指针 && 节点主键相同(stateInfo->状态主体, 世界树.自我指针));

        if (看起来像人类输入) {
            追加本能方法(枚举_本能动作ID::自我_读取最新人类输入);
        }
        if (是自我内部状态) {
            追加本能方法(枚举_本能动作ID::自我_读取核心状态);
        }

        追加本能方法(枚举_本能动作ID::自我_观察场景);
        return out;
    }

    需求节点类* 私有_创建补条件需求(
        任务节点类* 任务头结点,
        状态节点类* 目标状态,
        std::int64_t 权重,
        const std::string& 调用点)
    {
        using namespace 任务执行模块_detail;

        auto* headInfo = 取头信息(任务头结点);
        auto* stateInfo = 取状态信息(目标状态);
        if (!headInfo || !stateInfo) return nullptr;

        auto* parentNeedInfo = (headInfo->需求 && headInfo->需求->主信息) ? headInfo->需求->主信息 : nullptr;
        auto* 需求主体 = parentNeedInfo ? parentNeedInfo->需求主体 : dynamic_cast<存在节点类*>(stateInfo->状态主体);
        auto* 被需求存在 = dynamic_cast<存在节点类*>(stateInfo->状态主体);
        if (!被需求存在 && parentNeedInfo) 被需求存在 = parentNeedInfo->被需求存在;
        if (!需求主体) 需求主体 = 被需求存在;
        if (!需求主体 || !被需求存在) return nullptr;

        auto* sceneMi = new 场景节点主信息类();
        auto* demandScene = 世界树.创建场景(世界树.取内部世界(), sceneMi, 调用点 + "/创建需求场景");
        if (!demandScene) return nullptr;

        状态创建参数 stateArg{};
        stateArg.状态域 = stateInfo->状态域;
        stateArg.事件 = 枚举_存在状态事件::创建;
        stateArg.状态主体 = stateInfo->状态主体;
        stateArg.状态特征 = stateInfo->状态特征;
        stateArg.状态值 = stateInfo->状态值;
        stateArg.对应信息节点 = stateInfo->对应信息节点;
        stateArg.是否变化 = true;
        stateArg.now = 结构体_时间戳::当前_微秒();
        auto* demandState = 状态集.创建状态(demandScene, stateArg);
        if (!demandState) return nullptr;

        需求类::结构_创建参数 needArg{};
        needArg.描述 = parentNeedInfo ? parentNeedInfo->描述信息 : nullptr;
        needArg.类型 = parentNeedInfo ? parentNeedInfo->类型 : nullptr;
        needArg.发生场景 = headInfo->场景;
        needArg.生成时间 = stateArg.now;
        if (auto* 当前步骤信息 = (headInfo->当前步骤节点 && headInfo->当前步骤节点->主信息)
            ? dynamic_cast<任务步骤节点信息*>(headInfo->当前步骤节点->主信息)
            : nullptr) {
            if (当前步骤信息->超时截止时间 > 0) {
                needArg.有效截止 = 当前步骤信息->超时截止时间;
            }
        }
        if (needArg.有效截止 == 0 && parentNeedInfo && parentNeedInfo->需求有效截止 > 0) {
            needArg.有效截止 = parentNeedInfo->需求有效截止;
        }
        needArg.权重 = std::max<std::int64_t>(1, 权重);
        if (!headInfo->需求) {
            日志::运行f(
                "[任务执行器][不变量][提示] INV-007/INV-008: 补条件需求缺少父需求，将临时创建为顶层需求: 任务头={}, 状态主体={}, 状态特征={}",
                (void*)任务头结点,
                (void*)stateInfo->状态主体,
                (void*)stateInfo->状态特征);
        }
        return 需求集.创建需求(需求主体, 被需求存在, demandState, needArg, 调用点, headInfo->需求);
    }

    bool 私有_创建观察子任务(
        任务节点类* 任务头结点,
        任务节点类* 步骤节点,
        状态节点类* 目标状态,
        std::int64_t 权重,
        std::int64_t childIndex,
        bool 设为当前)
    {
        using namespace 任务执行模块_detail;

        auto* headInfo = 取头信息(任务头结点);
        auto* stepInfo = 取步骤信息(步骤节点);
        if (!headInfo || !stepInfo) return false;

        const auto now = 结构体_时间戳::当前_微秒();
        auto 观察方法列表 = 私有_选择观察方法候选(
            目标状态,
            now,
            "任务执行器::创建观察子任务/选择观察方法");
        if (观察方法列表.empty()) return false;

        auto* need = 私有_创建补条件需求(任务头结点, 目标状态, 权重, "任务执行器::创建观察子任务/创建观察需求");
        if (!need) return false;

        任务类::结构_创建任务头参数 任务参数{};
        任务参数.父任务头结点 = 任务头结点;
        任务参数.来源父步骤节点 = 步骤节点;
        任务参数.初始场景 = headInfo->场景;
        任务参数.局部优先级偏移 = std::max<std::int64_t>(0, 步骤节点->主信息->局部优先级偏移 + 60000 - childIndex * 1000);
        任务参数.任务树类型 = 枚举_任务树类型::叶子任务;
        任务参数.初始状态 = 枚举_任务状态::就绪;

        auto* childTask = 任务类::创建任务头结点(need, 任务参数, nullptr, "任务执行器::创建观察子任务/创建观察任务");
        if (!childTask || !childTask->主信息) return false;

        任务类::结构_创建步骤参数 步骤参数{};
        步骤参数.所属任务头结点 = childTask;
        步骤参数.步骤场景 = headInfo->场景;
        步骤参数.步骤序号 = 0;
        步骤参数.初始状态 = 枚举_任务状态::就绪;
        步骤参数.允许切换方法 = true;
        步骤参数.可用方法首节点列表 = 观察方法列表;
        步骤参数.当前选中方法首节点 = 观察方法列表.front();
        if (stepInfo->超时截止时间 > 0) {
            步骤参数.超时截止时间 = stepInfo->超时截止时间;
        }

        auto* observeStep = 任务类::创建步骤节点(childTask, 步骤参数, "任务执行器::创建观察子任务/创建观察步骤");
        if (!observeStep) return false;

        (void)任务类::设置任务当前步骤(childTask->主信息, observeStep, now, "任务执行器::创建观察子任务/设置当前步骤");
        (void)任务类::绑定步骤并发子任务(步骤节点, childTask, 设为当前, "任务执行器::创建观察子任务/绑定步骤");
        return true;
    }

    bool 私有_确保步骤补条件子任务(任务节点类* 任务头结点, 任务节点类* 步骤节点) {
        using namespace 任务执行模块_detail;

        auto* headInfo = 取头信息(任务头结点);
        auto* stepInfo = 取步骤信息(步骤节点);
        if (!headInfo || !stepInfo || !任务头结点->主信息 || !步骤节点->主信息) return false;
        if (!stepInfo->并发子任务头结点列表.empty()) return false;

        if (!stepInfo->当前选中方法首节点 && !stepInfo->可用方法首节点列表.empty()) {
            stepInfo->当前选中方法首节点 = stepInfo->可用方法首节点列表.front();
        }
        auto* methodHead = stepInfo->当前选中方法首节点;
        if (!methodHead) return false;

        auto 当前条件特征 = 枚举场景二次特征指针(headInfo->场景);
        auto gap = 分析方法条件缺口(methodHead, headInfo->场景, 当前条件特征);
        if (gap.当前可直接执行 || (gap.未满足状态.empty() && gap.待观察状态.empty())) return false;

        auto* parentNeedInfo = (headInfo->需求 && headInfo->需求->主信息) ? headInfo->需求->主信息 : nullptr;
        const auto needWeight = parentNeedInfo
            ? std::max<std::int64_t>(1, parentNeedInfo->权重)
            : std::max<std::int64_t>(1, 任务头结点->主信息->基准优先级);

        std::unordered_set<std::string> seen{};
        bool createdAny = false;
        std::int64_t childIndex = 0;

        for (auto* targetState : gap.未满足状态) {
            auto* targetInfo = 取状态信息(targetState);
            if (!targetInfo) continue;

            std::string key{};
            if (targetInfo->状态主体) key += targetInfo->状态主体->获取主键();
            key += "|";
            if (targetInfo->状态特征) key += targetInfo->状态特征->获取主键();
            key += "|";
            if (std::holds_alternative<I64>(targetInfo->状态值)) key += std::to_string(std::get<I64>(targetInfo->状态值));
            else key += std::to_string(targetInfo->状态值.index());
            if (!seen.insert(key).second) continue;

            auto* need = 私有_创建补条件需求(任务头结点, targetState, needWeight, "任务执行器::确保步骤补条件子任务/创建需求");
            if (!need) continue;

            任务类::结构_创建任务头参数 arg{};
            arg.父任务头结点 = 任务头结点;
            arg.来源父步骤节点 = 步骤节点;
            arg.初始场景 = headInfo->场景;
            arg.局部优先级偏移 = std::max<std::int64_t>(0, 步骤节点->主信息->局部优先级偏移 + 50000 - childIndex * 1000);
            arg.任务树类型 = 枚举_任务树类型::编排任务;
            arg.自动进入筹办阶段 = true;

            auto* childTask = 任务类::创建任务头结点(need, arg, nullptr, "任务执行器::确保步骤补条件子任务/创建子任务");
            if (!childTask) continue;

            (void)任务类::绑定步骤并发子任务(步骤节点, childTask, !createdAny, "任务执行器::确保步骤补条件子任务/绑定步骤");
            createdAny = true;
            ++childIndex;
        }

        for (auto* targetState : gap.待观察状态) {
            auto* targetInfo = 取状态信息(targetState);
            if (!targetInfo) continue;

            std::string key{ "observe|" };
            if (targetInfo->状态主体) key += targetInfo->状态主体->获取主键();
            key += "|";
            if (targetInfo->状态特征) key += targetInfo->状态特征->获取主键();
            key += "|";
            if (std::holds_alternative<I64>(targetInfo->状态值)) key += std::to_string(std::get<I64>(targetInfo->状态值));
            else key += std::to_string(targetInfo->状态值.index());
            if (!seen.insert(key).second) continue;

            if (私有_创建观察子任务(任务头结点, 步骤节点, targetState, needWeight, childIndex, !createdAny)) {
                createdAny = true;
                ++childIndex;
            }
        }

        if (createdAny) {
            任务类::设置任务状态(步骤节点, 枚举_任务状态::等待中, "任务执行器::确保步骤补条件子任务/步骤等待补条件");
            任务类::设置任务状态(任务头结点, 枚举_任务状态::运行中, "任务执行器::确保步骤补条件子任务/头结点等待补条件");
        }
        return createdAny;
    }

    任务节点类* 私有_自动筹办任务(任务节点类* 任务头结点) {
        using namespace 任务执行模块_detail;

        auto* headInfo = 取头信息(任务头结点);
        if (!headInfo || !任务头结点->主信息) return nullptr;

        auto* existed = 选择首选步骤节点(任务头结点);
        if (existed && !私有_步骤需要自动筹办(existed)) {
            私有_补全步骤预测结果(existed);
            (void)私有_确保步骤补条件子任务(任务头结点, existed);
            (void)任务类::设置任务当前步骤(任务头结点->主信息, existed, 结构体_时间戳::当前_微秒(), "任务执行器::自动筹办任务/沿用已有步骤");
            return existed;
        }

        任务类::设置任务状态(任务头结点, 枚举_任务状态::筹办中, "任务执行器::自动筹办任务/进入筹办");

        for (auto* child : 枚举子节点(任务头结点)) {
            if (!取步骤信息(child)) continue;
            if (!私有_步骤需要自动筹办(child)) continue;
            if (!枚举步骤下结果节点(child).empty()) continue;
            if (headInfo->当前步骤节点 == child) {
                (void)任务类::设置任务当前步骤(任务头结点->主信息, nullptr, 结构体_时间戳::当前_微秒(), "任务执行器::自动筹办任务/清空当前步骤");
            }
            (void)任务类::删除任务节点(child, "任务执行器::自动筹办任务/删除空白步骤");
        }

        auto candidates = 私有_召回可筹办方法(任务头结点);
        if (candidates.empty()) return nullptr;

        std::int32_t stepIndex = 0;
        for (const auto& candidate : candidates) {
            任务类::结构_创建步骤参数 arg{};
            arg.所属任务头结点 = 任务头结点;
            arg.步骤场景 = headInfo->场景;
            arg.可用方法首节点列表 = { candidate.方法首节点 };
            arg.当前选中方法首节点 = candidate.方法首节点;
            arg.局部优先级偏移 = candidate.评分Q;
            arg.步骤序号 = stepIndex++;
            arg.允许重试次数 = static_cast<std::int32_t>(cfg_.回跳重试上限);
            arg.允许切换方法 = false;
            auto* step = 任务类::创建步骤节点(任务头结点, arg, "任务执行器::自动筹办任务/创建并列步骤");
            if (!step) continue;
            私有_补全步骤预测结果(step);
            if (!candidate.可直接执行) (void)私有_确保步骤补条件子任务(任务头结点, step);
        }

        auto* best = 选择首选步骤节点(任务头结点);
        if (best) {
            (void)任务类::设置任务当前步骤(任务头结点->主信息, best, 结构体_时间戳::当前_微秒(), "任务执行器::自动筹办任务/设置首选步骤");
        }
        return best;
    }

    bool 私有_先推进步骤子任务(任务节点类* 任务头结点, 任务节点类* 步骤节点, bool& 子任务已全部完成, 结构_任务返回消息* 返回消息 = nullptr) {
        using namespace 任务执行模块_detail;

        子任务已全部完成 = false;
        auto* headInfo = 取头信息(任务头结点);
        auto* stepInfo = 取步骤信息(步骤节点);
        if (!headInfo || !stepInfo || !任务头结点->主信息 || !步骤节点->主信息) return false;

        auto 写子任务返回消息 = [&](枚举_任务返回消息类型 类型, 任务节点类* 相关子任务, bool 需要重筹办, const std::string& 摘要) {
            if (!返回消息) return;
            返回消息->类型 = 类型;
            返回消息->来源任务 = 任务头结点;
            返回消息->来源步骤 = 步骤节点;
            返回消息->相关子任务 = 相关子任务;
            返回消息->对应需求 = headInfo->需求;
            返回消息->需求已满足 = false;
            返回消息->已进入下一阶段 = false;
            返回消息->需要重筹办 = 需要重筹办;
            返回消息->摘要 = 摘要;
        };

        auto 清空当前子任务指针 = [&]() {
            if (stepInfo->当前子任务头结点 == nullptr && headInfo->当前子任务头结点 == nullptr) {
                return;
            }
            stepInfo->当前子任务头结点 = nullptr;
            headInfo->当前子任务头结点 = nullptr;
        };

        auto summary = 汇总步骤子任务(stepInfo);
        if (summary.总数 == 0) {
            清空当前子任务指针();
            return false;
        }

        if (子任务已清零(summary)) {
            清空当前子任务指针();
            if (summary.完成数 == summary.总数) {
                任务类::设置任务状态(步骤节点, 枚举_任务状态::就绪, "任务执行器::先推进步骤子任务/步骤恢复就绪");
                子任务已全部完成 = true;
                return true;
            }

            任务类::设置任务状态(步骤节点, 枚举_任务状态::待重筹办, "任务执行器::先推进步骤子任务/子任务清零待重筹办");
            任务类::设置任务状态(任务头结点, 枚举_任务状态::待重筹办, "任务执行器::先推进步骤子任务/头结点待重筹办");
            写子任务返回消息(
                summary.无法执行数 > 0 ? 枚举_任务返回消息类型::子任务无法执行 : 枚举_任务返回消息类型::子任务失败,
                nullptr,
                true,
                summary.无法执行数 > 0 ? "子任务已清零且包含无法执行分支，需要重新筹办" : "子任务已清零但未满足需求，需要重新筹办");
            return true;
        }

        if (summary.完成数 == summary.总数) {
            清空当前子任务指针();
            任务类::设置任务状态(步骤节点, 枚举_任务状态::就绪, "任务执行器::先推进步骤子任务/步骤恢复就绪");
            子任务已全部完成 = true;
            return true;
        }

        auto* childToRun = summary.首选可推进子任务 ? summary.首选可推进子任务 : summary.首个未终结子任务;
        if (childToRun) {
            (void)任务类::设置任务调度优先级(childToRun->主信息, childToRun->主信息->调度优先级, 结构体_时间戳::当前_微秒(), "任务执行器::先推进步骤子任务/刷新子任务优先级");
            (void)任务类::设置任务当前子任务(步骤节点->主信息, childToRun, 结构体_时间戳::当前_微秒(), "任务执行器::先推进步骤子任务/设置步骤当前子任务");
            (void)任务类::设置任务当前子任务(任务头结点->主信息, childToRun, 结构体_时间戳::当前_微秒(), "任务执行器::先推进步骤子任务/设置头结点当前子任务");
        }

        if (summary.首选可推进子任务) {
            任务类::设置任务状态(步骤节点, 枚举_任务状态::等待中, "任务执行器::先推进步骤子任务/步骤等待子任务");
            任务类::设置任务状态(任务头结点, 枚举_任务状态::运行中, "任务执行器::先推进步骤子任务/头结点调度子任务");
            写子任务返回消息(
                枚举_任务返回消息类型::生成子任务,
                summary.首选可推进子任务,
                false,
                "子任务已就绪，等待外层调度继续推进");
            return true;
        }

        任务类::设置任务状态(步骤节点, 枚举_任务状态::等待中, "任务执行器::先推进步骤子任务/步骤等待并发子任务");
        任务类::设置任务状态(任务头结点, 枚举_任务状态::等待中, "任务执行器::先推进步骤子任务/头结点等待并发子任务");
        写子任务返回消息(枚举_任务返回消息类型::生成子任务, childToRun, false, "当前没有可直接推进的子任务，父任务保持等待");
        return true;
    }

    bool 私有_切换到下一个方法(任务步骤节点信息* stepInfo) {
        if (!stepInfo || !stepInfo->允许切换方法) return false;
        const auto& methods = stepInfo->可用方法首节点列表;
        if (methods.size() < 2) return false;

        auto it = std::find(methods.begin(), methods.end(), stepInfo->当前选中方法首节点);
        if (it == methods.end()) {
            stepInfo->当前选中方法首节点 = methods.front();
            return true;
        }
        ++it;
        if (it == methods.end()) return false;
        stepInfo->当前选中方法首节点 = *it;
        return true;
    }

    bool 私有_执行步骤(任务节点类* 任务头结点, 任务节点类* 步骤节点, 结构_任务返回消息* 返回消息 = nullptr) {
        using namespace 任务执行模块_detail;

        auto* headInfo = 取头信息(任务头结点);
        auto* stepInfo = 取步骤信息(步骤节点);
        if (!headInfo || !stepInfo || !任务头结点->主信息 || !步骤节点->主信息) return false;

        日志::运行f(
            "[任务执行器] 执行步骤开始: 任务={}, 步骤={}, 方法={}, 并发子任务数={}",
            (void*)任务头结点,
            (void*)步骤节点,
            (void*)stepInfo->当前选中方法首节点,
            stepInfo->并发子任务头结点列表.size());

        auto 写返回消息 = [&](枚举_任务返回消息类型 类型, bool 需求已满足, bool 已进入下一阶段, bool 需要重筹办, const std::string& 摘要) {
            if (!返回消息) return;
            返回消息->类型 = 类型;
            返回消息->来源任务 = 任务头结点;
            返回消息->来源步骤 = 步骤节点;
            返回消息->相关子任务 = nullptr;
            返回消息->对应需求 = headInfo->需求;
            返回消息->需求已满足 = 需求已满足;
            返回消息->已进入下一阶段 = 已进入下一阶段;
            返回消息->需要重筹办 = 需要重筹办;
            返回消息->摘要 = 摘要;
        };

        (void)任务类::设置任务当前步骤(任务头结点->主信息, 步骤节点, 结构体_时间戳::当前_微秒(), "任务执行器::执行步骤/设置当前步骤");

        if (!stepInfo->当前选中方法首节点 && !stepInfo->可用方法首节点列表.empty()) {
            stepInfo->当前选中方法首节点 = stepInfo->可用方法首节点列表.front();
        }
        if (!stepInfo->当前选中方法首节点) {
            任务类::设置任务状态(步骤节点, 枚举_任务状态::无法执行, "任务执行器::执行步骤/无可执行方法");
            任务类::设置任务状态(任务头结点, 枚举_任务状态::无法执行, "任务执行器::执行步骤/任务无法执行");
            写返回消息(枚举_任务返回消息类型::步骤无法执行, false, false, false, "当前步骤没有可执行方法");
            return false;
        }

        日志::运行f(
            "[任务执行器] 执行步骤检查补条件前: 任务={}, 步骤={}, 方法={}",
            (void*)任务头结点,
            (void*)步骤节点,
            (void*)stepInfo->当前选中方法首节点);
        (void)私有_确保步骤补条件子任务(任务头结点, 步骤节点);
        日志::运行f(
            "[任务执行器] 执行步骤检查补条件后: 任务={}, 步骤={}, 并发子任务数={}",
            (void*)任务头结点,
            (void*)步骤节点,
            stepInfo->并发子任务头结点列表.size());

        bool 子任务已全部完成 = false;
        日志::运行f(
            "[任务执行器] 执行步骤进入子任务推进: 任务={}, 步骤={}",
            (void*)任务头结点,
            (void*)步骤节点);
        if (私有_先推进步骤子任务(任务头结点, 步骤节点, 子任务已全部完成, 返回消息)) {
            日志::运行f(
                "[任务执行器] 执行步骤子任务分支返回: 任务={}, 步骤={}, 子任务已全部完成={}, 返回类型={}",
                (void*)任务头结点,
                (void*)步骤节点,
                子任务已全部完成 ? 1 : 0,
                返回消息 ? static_cast<int>(返回消息->类型) : -1);
            if (!子任务已全部完成) {
                if (!返回消息 || 返回消息->类型 == 枚举_任务返回消息类型::未定义) {
                    写返回消息(枚举_任务返回消息类型::生成子任务, false, false, false, "当前步骤等待子任务结果");
                }
                return true;
            }
        }

        日志::运行f(
            "[任务执行器] 执行步骤子任务推进结束: 任务={}, 步骤={}, 子任务已全部完成={}",
            (void*)任务头结点,
            (void*)步骤节点,
            子任务已全部完成 ? 1 : 0);
        日志::运行f(
            "[任务执行器] 执行步骤补全预测结果前: 任务={}, 步骤={}",
            (void*)任务头结点,
            (void*)步骤节点);
        私有_补全步骤预测结果(步骤节点);
        日志::运行f(
            "[任务执行器] 执行步骤补全预测结果后: 任务={}, 步骤={}",
            (void*)任务头结点,
            (void*)步骤节点);
        日志::运行f(
            "[任务执行器] 执行步骤设置执行中前: 任务={}, 步骤={}",
            (void*)任务头结点,
            (void*)步骤节点);
        任务类::设置任务状态(步骤节点, 枚举_任务状态::执行中, "任务执行器::执行步骤/步骤执行中");
        日志::运行f(
            "[任务执行器] 执行步骤设置执行中后: 任务={}, 步骤={}",
            (void*)任务头结点,
            (void*)步骤节点);

        结构_叶子执行结果 r{};
        if (leaf_exec_) {
            日志::运行f(
                "[任务执行器] 执行步骤即将调用叶子回调: 任务={}, 步骤={}, 方法={}",
                (void*)任务头结点,
                (void*)步骤节点,
                (void*)stepInfo->当前选中方法首节点);
            r = leaf_exec_(步骤节点);
        }
        else {
            r.成功 = false;
            r.摘要 = "步骤执行回调未设置";
            r.失败类型 = 枚举_任务失败类型::无叶子执行回调;
            r.建议动作 = 枚举_任务反馈动作::转入尝试学习;
            r.推荐重试任务树类型 = 枚举_任务树类型::叶子任务;
        }

        auto* matchedPrediction = 选择命中预测结果节点(步骤节点, r.结果场景, stepInfo->当前选中方法首节点);
        auto* matchedInfo = 取结果信息(matchedPrediction);
        auto action = 从反馈动作映射处置(r.成功, r.建议动作);
        if (matchedInfo && matchedInfo->命中后动作 != 枚举_分支选择动作::未定义) {
            action = matchedInfo->命中后动作;
        }

        任务类::结构_创建结果参数 resultArg{};
        resultArg.所属任务头结点 = 任务头结点;
        resultArg.结果场景 = r.结果场景
            ? r.结果场景
            : ((matchedPrediction && matchedPrediction->主信息) ? matchedPrediction->主信息->场景 : 步骤节点->主信息->场景);
        resultArg.结果角色 = 枚举_任务结果角色::实际结果;
        resultArg.来源方法首节点 = stepInfo->当前选中方法首节点;
        resultArg.对应方法结果节点 = matchedInfo ? matchedInfo->对应方法结果节点 : nullptr;
        resultArg.命中预测结果节点 = matchedPrediction;
        resultArg.命中后动作 = action;
        resultArg.是否预测外结果 = matchedPrediction == nullptr;
        resultArg.置信度_Q10000 = matchedPrediction ? 10000 : 0;
        resultArg.局部优先级偏移 = (matchedPrediction && matchedPrediction->主信息) ? matchedPrediction->主信息->局部优先级偏移 : 步骤节点->主信息->局部优先级偏移;
        resultArg.错误码 = static_cast<std::int64_t>(r.失败类型);
        resultArg.初始状态 = r.成功 ? 枚举_任务状态::完成 : 枚举_任务状态::失败;
        auto* actualResultNode = 任务类::创建结果节点(步骤节点, resultArg, "任务执行器::执行步骤/追加实际结果");
        if (!actualResultNode) {
            写返回消息(枚举_任务返回消息类型::步骤失败, false, false, false, "步骤执行结果未能写入结果节点");
            return false;
        }

        (void)任务类::设置任务最近结果节点(任务头结点->主信息, actualResultNode, 结构体_时间戳::当前_微秒(), "任务执行器::执行步骤/记录最近结果");
        (void)任务类::设置任务当前分支动作(任务头结点->主信息, action, 结构体_时间戳::当前_微秒(), "任务执行器::执行步骤/记录动作");
        任务类::设置任务状态(步骤节点, r.成功 ? 枚举_任务状态::完成 : 枚举_任务状态::失败, "任务执行器::执行步骤/步骤结束");

        auto* nextStep = 取命中结果后的下一步骤(matchedPrediction);
        if (nextStep) {
            (void)任务类::设置任务当前步骤(任务头结点->主信息, nextStep, 结构体_时间戳::当前_微秒(), "任务执行器::执行步骤/进入下一步骤");
            任务类::设置任务状态(任务头结点, 枚举_任务状态::就绪, "任务执行器::执行步骤/头结点等待下一步回复");
            写返回消息(枚举_任务返回消息类型::步骤完成, false, true, false, r.摘要.empty() ? "步骤完成，已进入下一阶段" : r.摘要);
            return true;
        }

        if (!r.成功) {
            const auto retryLimit = stepInfo->允许重试次数 > 0 ? stepInfo->允许重试次数 : static_cast<std::int32_t>(cfg_.回跳重试上限);
            if (action == 枚举_分支选择动作::回跳重试 && stepInfo->已重试次数 < retryLimit) {
                ++stepInfo->已重试次数;
                (void)私有_切换到下一个方法(stepInfo);
                任务类::设置任务状态(步骤节点, 枚举_任务状态::就绪, "任务执行器::执行步骤/重试回到就绪");
                任务类::设置任务状态(任务头结点, 枚举_任务状态::运行中, "任务执行器::执行步骤/任务等待重试");
                return true;
            }

            if (action == 枚举_分支选择动作::转入尝试学习) {
                (void)私有_确保尝试学习任务();
                headInfo->等待学习唤醒 = true;
                headInfo->等待学习方法首节点 = stepInfo->当前选中方法首节点;
                任务类::设置任务状态(任务头结点, 枚举_任务状态::待重筹办, "任务执行器::执行步骤/转入尝试学习待重筹办");
                写返回消息(
                    枚举_任务返回消息类型::步骤无法执行,
                    false,
                    false,
                    true,
                    r.摘要.empty() ? "步骤执行失败，已转入尝试学习并等待重筹办" : r.摘要);
            }
            else if (action == 枚举_分支选择动作::挂起等待) {
                任务类::设置任务状态(任务头结点, 枚举_任务状态::挂起, "任务执行器::执行步骤/挂起等待");
                写返回消息(枚举_任务返回消息类型::步骤无法执行, false, false, false, r.摘要.empty() ? "步骤挂起等待外部回复" : r.摘要);
            }
            else {
                任务类::设置任务状态(任务头结点, 枚举_任务状态::失败, "任务执行器::执行步骤/任务失败");
                写返回消息(枚举_任务返回消息类型::步骤失败, false, false, false, r.摘要.empty() ? "步骤执行失败" : r.摘要);
            }
            return true;
        }

        bool 需求已满足 = false;
        bool 需要重筹办 = false;
        if (headInfo->需求) {
            const auto 判定 = 需求集.判断需求状态(headInfo->需求, "任务执行器::执行步骤/阶段完成后判断需求");
            需求已满足 = 判定.已满足 && !headInfo->是否真根任务;
            需要重筹办 = headInfo->是否真根任务 || !判定.已满足;
        }

        (void)任务类::设置任务当前步骤(
            任务头结点->主信息,
            nullptr,
            结构体_时间戳::当前_微秒(),
            "任务执行器::执行步骤/阶段结束清空当前步骤");

        if (需要重筹办) {
            任务类::设置任务状态(任务头结点, 枚举_任务状态::待重筹办, "任务执行器::执行步骤/任务待重筹办");
            写返回消息(枚举_任务返回消息类型::步骤完成, false, false, true, r.摘要.empty() ? "局部阶段结束，需求未满足，等待重筹办" : r.摘要);
            return true;
        }

        任务类::设置任务状态(任务头结点, 枚举_任务状态::完成, "任务执行器::执行步骤/任务完成");
        写返回消息(枚举_任务返回消息类型::步骤完成, 需求已满足 || !headInfo->需求, false, false, r.摘要.empty() ? "局部阶段结束，任务完成" : r.摘要);
        return true;
    }

    任务节点类* 私有_确保尝试学习任务() {
        if (try_learn_head_ && try_learn_head_->主信息) return try_learn_head_;

        auto* mi = new 任务头结点信息();
        mi->节点种类 = 枚举_任务节点种类::头结点;
        mi->状态 = 枚举_任务状态::未启动;
        mi->基准优先级 = 0;
        mi->局部优先级偏移 = 0;
        mi->调度优先级 = 0;
        mi->任务树类型 = 枚举_任务树类型::叶子任务;
        mi->场景 = nullptr;
        mi->任务虚拟存在 = nullptr;
        mi->需求 = nullptr;

        try_learn_head_ = 任务类::新建任务节点(mi, nullptr, "任务执行器::确保尝试学习任务");
        return try_learn_head_;
    }

private:
    结构_配置 cfg_{};
    Fn_叶子执行 leaf_exec_{};
    Fn_方法规避过滤 avoid_filter_{};
    任务节点类* try_learn_head_ = nullptr;
};

















