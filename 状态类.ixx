export module 状态模块;

import <vector>;
import <string>;
import <cstdint>;
import <algorithm>;
import <functional>;
import <variant>;

import 模板模块;
import 基础数据类型模块;
import 主信息定义模块;

// ============================================================
// 状态模块：统一状态表达层
// 约定：
// - 状态节点不入世界链，不设主键；仅作为“过程场景”内部节点存在。
// - 状态节点统一表达“谁在某个时间点具有什么状态值”。
// - 状态分为两大域：内部状态、世界状态。
//   - 内部状态：自我系统内部虚拟存在的状态，如任务/需求/方法等。
//   - 世界状态：世界树中某主体在某时刻的特征值快照。
// - 状态节点的顺序有意义（时间序列），因此本模块会把 场景主信息.状态列表 维持为按时间升序排列。
// - 状态节点的释放由调用者决定（通常：过程场景结束时统一释放）。
// ============================================================

export enum class 枚举_存在状态事件 : std::uint8_t {
    未定义 = 0,
    创建 = 1,
    消失 = 2,
    变化 = 3
};

export struct 状态创建参数 {
    枚举_状态域 状态域 = 枚举_状态域::未定义;
    枚举_存在状态事件 事件 = 枚举_存在状态事件::未定义;
    基础信息节点类* 状态主体 = nullptr;
    特征节点类* 状态特征 = nullptr;
    特征快照值 状态值{};
    基础信息节点类* 对应信息节点 = nullptr;
    bool 是否变化 = false;
    时间戳 now = 0;
};

// 对 主信息定义模块 中状态骨架做运行期扩展：补一个明确的事件类型字段。
export class 状态节点主信息_扩展类 final : public 状态节点主信息类 {
public:
    枚举_存在状态事件 事件 = 枚举_存在状态事件::未定义;

public:
    状态节点主信息_扩展类() = default;

    explicit 状态节点主信息_扩展类(const 状态创建参数& p)
    {
        事件 = p.事件;
        状态域 = p.状态域;
        状态主体 = p.状态主体;
        状态特征 = p.状态特征;
        状态值 = p.状态值;
        对应信息节点 = p.对应信息节点 ? p.对应信息节点 : (p.状态特征 ? static_cast<基础信息节点类*>(p.状态特征) : p.状态主体);
        是否变化 = p.是否变化;
        收到时间 = p.now;
        发生时间 = p.now;
    }
};

export inline 枚举_存在状态事件 取状态事件(const 状态节点类* s) noexcept
{
    if (!s || !s->主信息) return 枚举_存在状态事件::未定义;
    if (auto* ex = dynamic_cast<状态节点主信息_扩展类*>(s->主信息)) return ex->事件;
    if (auto* mi = dynamic_cast<状态节点主信息类*>(s->主信息)) return mi->是否变化 ? 枚举_存在状态事件::变化 : 枚举_存在状态事件::未定义;
    return 枚举_存在状态事件::未定义;
}

export class 状态类 {
public:
    struct 结构_最近两次I64状态方向结果 {
        bool 可比较 = false;
        I64 方向 = 0;
        状态节点类* 最近状态 = nullptr;
        状态节点类* 前一状态 = nullptr;
    };

    状态类() = default;
    ~状态类() = default;

    状态类(const 状态类&) = delete;
    状态类& operator=(const 状态类&) = delete;

private:
    static 时间戳 取状态排序时间_(const 状态节点类* 状态) noexcept
    {
        auto* mi = (状态 && 状态->主信息) ? dynamic_cast<状态节点主信息类*>(状态->主信息) : nullptr;
        if (!mi) return 0;
        return mi->发生时间 ? mi->发生时间 : mi->收到时间;
    }

    static void 按时间插入状态_(场景节点主信息类* smi, 状态节点类* 状态)
    {
        if (!smi || !状态) return;
        auto& 列表 = smi->状态列表;
        const 时间戳 新状态时间 = 取状态排序时间_(状态);
        auto it = std::upper_bound(
            列表.begin(),
            列表.end(),
            新状态时间,
            [](时间戳 目标时间, const 状态节点类* 已有状态) {
                return 目标时间 < 取状态排序时间_(已有状态);
            });
        列表.insert(it, 状态);
    }

public:
    using 状态后处理函数 = std::function<void(场景节点类*, 状态节点类*, 时间戳, const std::string&)>;
public:
    状态节点类* 创建状态(
        场景节点类* 场景,
        const 状态创建参数& p,
        状态后处理函数 后处理 = {},
        const std::string& 调用点 = "状态类::创建状态")
    {
        (void)调用点;
        if (!场景 || !场景->主信息) return nullptr;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!smi) return nullptr;

        auto* n = new 状态节点类{};
        n->父 = n->子 = nullptr;
        n->上 = n->下 = nullptr;
        n->根 = nullptr;
        n->链上 = n->链下 = nullptr;
        n->子节点数量 = 0;

        auto* mi = new 状态节点主信息_扩展类(p);
        n->主信息 = mi;

        按时间插入状态_(smi, n);
        if (后处理) {
            const auto hookNow = mi->发生时间 ? mi->发生时间 : (mi->收到时间 ? mi->收到时间 : p.now);
            后处理(场景, n, hookNow, 调用点);
        }
        return n;
    }

    // 兼容旧接口：默认仍可作为“过程事件状态”使用。
    状态节点类* 创建状态(
        场景节点类* 场景,
        枚举_存在状态事件 事件,
        基础信息节点类* 对应信息节点,
        bool 是否变化 = false,
        时间戳 now = 0,
        状态后处理函数 后处理 = {},
        const std::string& 调用点 = "状态类::创建状态/兼容")
    {
        状态创建参数 p{};
        p.状态域 = 枚举_状态域::未定义;
        p.事件 = 事件;
        p.对应信息节点 = 对应信息节点;
        p.状态主体 = 对应信息节点;
        p.是否变化 = 是否变化;
        p.now = now;
        return 创建状态(场景, p, std::move(后处理), 调用点);
    }

    状态节点类* 创建内部状态(
        场景节点类* 场景,
        基础信息节点类* 内部主体,
        特征节点类* 状态特征 = nullptr,
        const 特征快照值& 状态值 = {},
        枚举_存在状态事件 事件 = 枚举_存在状态事件::变化,
        bool 是否变化 = true,
        时间戳 now = 0,
        状态后处理函数 后处理 = {},
        const std::string& 调用点 = "状态类::创建内部状态")
    {
        状态创建参数 p{};
        p.状态域 = 枚举_状态域::内部状态;
        p.事件 = 事件;
        p.状态主体 = 内部主体;
        p.状态特征 = 状态特征;
        p.状态值 = 状态值;
        p.对应信息节点 = 状态特征 ? static_cast<基础信息节点类*>(状态特征) : 内部主体;
        p.是否变化 = 是否变化;
        p.now = now;
        return 创建状态(场景, p, std::move(后处理), 调用点);
    }

    状态节点类* 创建世界状态(
        场景节点类* 场景,
        基础信息节点类* 世界主体,
        特征节点类* 状态特征,
        const 特征快照值& 状态值,
        枚举_存在状态事件 事件 = 枚举_存在状态事件::变化,
        bool 是否变化 = true,
        时间戳 now = 0,
        状态后处理函数 后处理 = {},
        const std::string& 调用点 = "状态类::创建世界状态")
    {
        状态创建参数 p{};
        p.状态域 = 枚举_状态域::世界状态;
        p.事件 = 事件;
        p.状态主体 = 世界主体;
        p.状态特征 = 状态特征;
        p.状态值 = 状态值;
        p.对应信息节点 = 状态特征 ? static_cast<基础信息节点类*>(状态特征) : 世界主体;
        p.是否变化 = 是否变化;
        p.now = now;
        return 创建状态(场景, p, std::move(后处理), 调用点);
    }

    状态节点类* 记录内部特征状态(
        场景节点类* 场景,
        基础信息节点类* 内部主体,
        特征节点类* 状态特征,
        const 特征快照值& 状态值,
        枚举_存在状态事件 事件 = 枚举_存在状态事件::变化,
        bool 是否变化 = true,
        时间戳 now = 0,
        状态后处理函数 后处理 = {},
        const std::string& 调用点 = "状态类::记录内部特征状态")
    {
        (void)调用点;
        if (!场景 || !内部主体) return nullptr;
        if (事件 != 枚举_存在状态事件::创建 && !是否变化) return nullptr;
        return 创建内部状态(场景, 内部主体, 状态特征, 状态值, 事件, 是否变化, now, std::move(后处理), 调用点);
    }

    状态节点类* 记录世界特征状态(
        场景节点类* 场景,
        基础信息节点类* 世界主体,
        特征节点类* 状态特征,
        const 特征快照值& 状态值,
        枚举_存在状态事件 事件 = 枚举_存在状态事件::变化,
        bool 是否变化 = true,
        时间戳 now = 0,
        状态后处理函数 后处理 = {},
        const std::string& 调用点 = "状态类::记录世界特征状态")
    {
        (void)调用点;
        if (!场景 || !世界主体 || !状态特征) return nullptr;
        if (事件 != 枚举_存在状态事件::创建 && !是否变化) return nullptr;
        return 创建世界状态(场景, 世界主体, 状态特征, 状态值, 事件, 是否变化, now, std::move(后处理), 调用点);
    }

    状态节点类* 查找最近状态(
        场景节点类* 场景,
        枚举_存在状态事件 事件,
        const 基础信息节点类* 对应信息节点 = nullptr) const
    {
        if (!场景 || !场景->主信息) return nullptr;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!smi) return nullptr;

        for (auto it = smi->状态列表.rbegin(); it != smi->状态列表.rend(); ++it) {
            auto* s = *it;
            if (!s || !s->主信息) continue;

            if (对应信息节点) {
                auto* mi = dynamic_cast<状态节点主信息类*>(s->主信息);
                if (!mi || mi->对应信息节点 != 对应信息节点) continue;
            }

            if (事件 != 枚举_存在状态事件::未定义) {
                if (取状态事件(s) != 事件) continue;
            }

            return s;
        }
        return nullptr;
    }

    状态节点类* 查找最近状态_按状态域(
        场景节点类* 场景,
        枚举_状态域 状态域,
        const 基础信息节点类* 状态主体 = nullptr) const
    {
        if (!场景 || !场景->主信息) return nullptr;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!smi) return nullptr;

        for (auto it = smi->状态列表.rbegin(); it != smi->状态列表.rend(); ++it) {
            auto* s = *it;
            auto* mi = (s && s->主信息) ? dynamic_cast<状态节点主信息类*>(s->主信息) : nullptr;
            if (!mi) continue;
            if (状态域 != 枚举_状态域::未定义 && mi->状态域 != 状态域) continue;
            if (状态主体 && mi->状态主体 != 状态主体) continue;
            return s;
        }
        return nullptr;
    }

    结构_最近两次I64状态方向结果 比较最近两次I64状态方向(
        场景节点类* 场景,
        枚举_状态域 状态域,
        const 基础信息节点类* 状态主体 = nullptr,
        const 特征节点类* 状态特征 = nullptr) const
    {
        结构_最近两次I64状态方向结果 结果{};
        if (!场景 || !场景->主信息) return 结果;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!smi) return 结果;

        I64 最近值 = 0;
        bool 已有最近值 = false;

        for (auto it = smi->状态列表.rbegin(); it != smi->状态列表.rend(); ++it) {
            auto* s = *it;
            auto* mi = (s && s->主信息) ? dynamic_cast<状态节点主信息类*>(s->主信息) : nullptr;
            if (!mi) continue;
            if (状态域 != 枚举_状态域::未定义 && mi->状态域 != 状态域) continue;
            if (状态主体 && mi->状态主体 != 状态主体) continue;
            if (状态特征 && mi->状态特征 != 状态特征) continue;

            const auto* 值 = std::get_if<I64>(&mi->状态值);
            if (!值) continue;

            if (!已有最近值) {
                最近值 = *值;
                已有最近值 = true;
                结果.最近状态 = s;
                continue;
            }

            结果.前一状态 = s;
            结果.可比较 = true;
            if (最近值 > *值) {
                结果.方向 = 1;
            }
            else if (最近值 < *值) {
                结果.方向 = -1;
            }
            else {
                结果.方向 = 0;
            }
            return 结果;
        }

        return 结果;
    }

    bool 移除状态(
        场景节点类* 场景,
        状态节点类* 状态节点,
        bool 释放内存 = true)
    {
        if (!场景 || !场景->主信息 || !状态节点) return false;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!smi) return false;

        auto& v = smi->状态列表;
        auto it = std::find(v.begin(), v.end(), 状态节点);
        if (it == v.end()) return false;

        v.erase(it);

        if (释放内存) {
            delete 状态节点->主信息;
            状态节点->主信息 = nullptr;
            delete 状态节点;
        }
        return true;
    }

    void 清空场景状态(场景节点类* 场景, bool 释放内存 = true)
    {
        if (!场景 || !场景->主信息) return;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!smi) return;

        if (释放内存) {
            for (auto* s : smi->状态列表) {
                if (!s) continue;
                delete s->主信息;
                s->主信息 = nullptr;
                delete s;
            }
        }
        smi->状态列表.clear();
    }
};

export 状态类 通用状态集;
export 状态类 状态集;

