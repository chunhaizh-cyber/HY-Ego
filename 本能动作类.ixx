
export module 本能动作模块;

// ============================================================================
// 本能动作模块（通用函数）
// ----------------------------------------------------------------------------
// 规范：
//  - 本模块只包含“本能动作需要的通用函数”。
//  - 不包含任何具体业务本能方法实现（相机启动/识别/生成存在…）。
//  - 具体业务实现请放在：外设本能方法_相机模块 / 自我本能方法_相机认知模块 等。
// ============================================================================

import <cstdint>;
import <string>;
import <vector>;
import <variant>;
import <algorithm>;
import <limits>;
import <functional>;

import 基础数据类型模块;
import 主信息定义模块;
import 通用函数模块;
using namespace 通用函数模块;

export class 本能动作类 {
    // ============================================================
    // 1) Step1 工具：确保方法首节点条件模板写入“需要的参数类型”
    //    - 写入位置：方法首节点主信息 `场景根节点` 的子链（特征节点，无值 => 只约束类型）
    //    - 幂等：同类型不重复写
    // ============================================================
public:
    本能动作类() {};
    ~本能动作类() {};

   
    // ============================================================
    // 2) Step4 工具：统一回执写入（最小字段）
    //    - 你工程的“场景写入原语”可能在世界树环境模块；此处只提供通用字段打包。
    //    - 调用方可自行决定写入到输出场景的哪个子节点。
    // ============================================================
    struct 结构体_本能回执最小字段 {
        bool 成功 = false;
        I64 错误码 = 0;
        std::string 诊断{};
        时间戳 耗时_us = 0;
        时间戳 时间_系统 = 0;
    };

    inline void 回执_拼接诊断(std::string& io, const std::string& add)
    {
        if (add.empty()) return;
        if (!io.empty() && io.back() != ';') io.push_back(';');
        io += add;
    }

    // ============================================================
    // 3) Step2 工具：场景模式匹配（形参模板↔实参绑定）
    //    输出：绑定表 + 匹配度总分 + 歧义标记 + 未满足约束列表
    //    规则：
    //      - 默认 AND；OR 用“特征节点无值且有子节点”表示
    //      - 值相等为“算法相等”（差值<=阈值），不是数学 ==
    //      - 同类型多候选：取最高分，并列按主键最小，top1-top2<=阈值 => 歧义
    //      - 多余实参：忽略
    // ============================================================

    struct 结构体_场景模式匹配参数 {
        I64 I64容忍误差 = 0;
        I64 歧义分差阈值 = 50;

        // 外部差值算法（可接二次特征比较）；返回代价（越小越好）；<0 表示不支持
        std::function<I64(const 词性节点类* 类型, const 特征快照值& 输入值, const 特征快照值& 模式值)> 差值算法{};
    };

    struct 结构体_场景模式匹配绑定 {
        const 基础信息节点类* 模式节点 = nullptr;
        基础信息节点类* 输入节点 = nullptr;
        I64 匹配分 = 0;
    };

    struct 结构体_场景模式匹配结果 {
        bool 匹配成功 = false;
        bool 有歧义 = false;
        I64 总分 = 0;

        std::vector<结构体_场景模式匹配绑定> 绑定表{};
        std::vector<const 基础信息节点类*> 未满足模式节点{};
    };
   
    private:
        static inline bool 是特征节点(const 基础信息节点类* n) {
            return n && n->主信息 && dynamic_cast<特征节点主信息类*>(n->主信息) != nullptr;
        }
        static inline const 特征节点主信息类* 取特征主信息(const 基础信息节点类* n) {
            return (n && n->主信息) ? dynamic_cast<特征节点主信息类*>(n->主信息) : nullptr;
        }
        static inline bool 特征有值(const 基础信息节点类* n) {
            auto* f = 取特征主信息(n);
            if (!f) return false;
            return !std::holds_alternative<std::monostate>(f->当前快照);
        }
        static inline const 特征快照值* 取特征值(const 基础信息节点类* n) {
            auto* f = 取特征主信息(n);
            if (!f) return nullptr;
            return &f->当前快照;
        }
        static inline const 词性节点类* 取类型(const 基础信息节点类* n) {
            auto* mi = (n && n->主信息) ? dynamic_cast<基础信息基类*>(n->主信息) : nullptr;
            return mi ? mi->类型 : nullptr;
        }
        static inline std::string 取主键(const 基础信息节点类* n) {
            return n ? n->获取主键() : std::string{};
        }
        static inline std::vector<基础信息节点类*> 枚举子节点(基础信息节点类* parent) {
            std::vector<基础信息节点类*> out;
            if (!parent || !parent->子) return out;
            auto* start = static_cast<基础信息节点类*>(parent->子);
            auto* cur = start;
            do { out.push_back(cur); cur = cur ? static_cast<基础信息节点类*>(cur->下) : nullptr; } while (cur && cur != start);
            return out;
        }
        static inline bool 是OR组(const 基础信息节点类* 模式节点) {
            // 仅允许“特征节点 + 无值 + 有子节点”作为 OR 组
            if (!是特征节点(模式节点)) return false;
            if (特征有值(模式节点)) return false;
            if (!模式节点 || !模式节点->子) return false;
            return true;
        }

        static inline I64 兜底差值_I64(I64 a, I64 b)
        {
            const I64 最大 = (std::numeric_limits<I64>::max)();

            // 返回 |a-b|，若超出 I64 则饱和到 最大
            if (a >= b) {
                if (b < 0 && a > 最大 + b) return 最大;
                return a - b;
            }
            else {
                if (a < 0 && b > 最大 + a) return 最大;
                return b - a;
            }
        }

        static inline I64 差值(const 结构体_场景模式匹配参数& p, const 词性节点类* 类型,
            const 特征快照值& 输入, const 特征快照值& 模式)
        {
            if (p.差值算法) {
                const I64 cost = p.差值算法(类型, 输入, 模式);
                if (cost >= 0) return cost;
            }
            if (std::holds_alternative<I64>(输入) && std::holds_alternative<I64>(模式)) {
                return 兜底差值_I64(std::get<1>(输入), std::get<1>(模式));
            }
            // 其它类型暂不支持 => 最大代价
            return std::numeric_limits<I64>::max();
        }

        struct 局部结果 { bool ok = true; bool ambiguous = false; I64 score = 0; };

        static inline std::vector<const 基础信息节点类*> 枚举子节点_只读(const 基础信息节点类* parent)
        {
            std::vector<const 基础信息节点类*> out;
            if (!parent || !parent->子) return out;

            auto* start = static_cast<const 基础信息节点类*>(parent->子);
            auto* cur = start;
            do {
                out.push_back(cur);
                cur = cur ? static_cast<const 基础信息节点类*>(cur->下) : nullptr;
            } while (cur && cur != start);

            return out;
        }

     
        static inline I64 值匹配分(const 结构体_场景模式匹配参数& p,
            const 词性节点类* 类型,
            const 基础信息节点类* 输入节点,
            const 基础信息节点类* 模式节点)
        {
            if (!是特征节点(模式节点)) return 0;

            const bool 模式有 = 特征有值(模式节点);
            if (!模式有) {
                // 叶子无值 => 任意值（给小分）；OR 组的无值由上层控制
                return 100;
            }

            const auto* pv = 取特征值(模式节点);
            const auto* iv = 取特征值(输入节点);
            if (!pv || !iv) return 0;

            const I64 cost = 差值(p, 类型, *iv, *pv);
            if (cost == std::numeric_limits<I64>::max()) return 0;

            if (std::holds_alternative<I64>(*iv) && std::holds_alternative<I64>(*pv)) {
                if (cost <= p.I64容忍误差) return 1000;
                const I64 s = 1000 - std::min<I64>(1000, cost);
                return std::max<I64>(0, s);
            }
            const I64 s = 1000 - std::min<I64>(1000, cost);
            return std::max<I64>(0, s);
        }

    
       
        static inline 局部结果 挑选绑定(基础信息节点类* 输入父, const 基础信息节点类* 模式子, const 结构体_场景模式匹配参数& p, 结构体_场景模式匹配结果& io)
        {
            局部结果 r{};
            if (!输入父 || !模式子) { r.ok = false; return r; }
            const 词性节点类* t = 取类型(模式子);
            if (!t) { r.ok = false; io.未满足模式节点.push_back(模式子); return r; }

            struct 候选 { 基础信息节点类* n = nullptr; I64 s = 0; };
            std::vector<候选> cand;

            for (auto* inChild : 枚举子节点(输入父)) {
                if (!inChild) continue;
                if (取类型(inChild) != t) continue;

                I64 s = 1000;
                s += 值匹配分(p, t, inChild, 模式子);
                cand.push_back({ inChild, s });
            }

            if (cand.empty()) { r.ok = false; io.未满足模式节点.push_back(模式子); return r; }

            std::sort(cand.begin(), cand.end(), [](const 候选& a, const 候选& b) {
                if (a.s != b.s) return a.s > b.s;
                return 取主键(a.n) < 取主键(b.n);
                });

            io.绑定表.push_back({ 模式子, cand[0].n, cand[0].s });
            r.score += cand[0].s;

            if (cand.size() >= 2) {
                const I64 diff = 兜底差值_I64(cand[0].s, cand[1].s);
                if (diff <= p.歧义分差阈值) {
                    r.ambiguous = true;
                    io.有歧义 = true;
                }
            }
            return r;
        }
        static inline 局部结果 递归匹配子节点(基础信息节点类* 输入节点,const 基础信息节点类* 模式父节点,bool OR模式,const 结构体_场景模式匹配参数& p,结构体_场景模式匹配结果& io);
        static inline 局部结果 递归匹配节点(基础信息节点类* 输入节点,const 基础信息节点类* 模式节点,const 结构体_场景模式匹配参数& p,结构体_场景模式匹配结果& io);

  
 public:
     static inline void 方法首节点_确保条件参数类型(方法节点类* 方法首节点, const std::vector<const 词性节点类*>& 需要类型表)
     {
         if (!方法首节点 || !方法首节点->主信息) return;

         auto* hmi = dynamic_cast<方法首节点主信息类*>(方法首节点->主信息);
         if (!hmi) return;

         场景节点类* root = &hmi->场景根节点;

         auto 有类型 = [&](const 词性节点类* t) -> bool {
             if (!root || !root->子 || !t) return false;
             auto* start = static_cast<基础信息节点类*>(root->子);
             auto* cur = start;
             do {
                 if (cur && cur->主信息) {
                     if (auto* fi = dynamic_cast<特征节点主信息类*>(cur->主信息)) {
                         if (fi->类型 == t) return true;
                     }
                 }
                 cur = cur ? static_cast<基础信息节点类*>(cur->下) : nullptr;
             } while (cur && cur != start);
             return false;
             };

         auto 挂子_仅同层环 = [&](基础信息节点类* 父, 基础信息节点类* 子) {
             if (!父 || !子) return;
             子->父 = 父;
             子->根 = 父->根 ? 父->根 : 父;

             if (!父->子) {
                 父->子 = 子;
                 子->上 = 子;
                 子->下 = 子;
             }
             else {
                 auto* start = static_cast<基础信息节点类*>(父->子);
                 auto* last = static_cast<基础信息节点类*>(start->上);
                 last->下 = 子;
                 子->上 = last;
                 子->下 = start;
                 start->上 = 子;
             }
             };

         for (auto* t : 需要类型表) {
             if (!t) continue;
             if (有类型(t)) continue;

             auto* info = new 特征节点主信息类();
             info->类型 = t;
             info->当前快照 = std::monostate{};

             auto* n = new 基础信息节点类();
             n->主信息 = info;

             // 方法模板内主键：无需全局唯一，但需确定性，便于调试与去重
             n->设置主键(std::string("cond_") + t->获取主键());

             挂子_仅同层环(static_cast<基础信息节点类*>(root), n);
         }
     }

     static inline 结构体_场景模式匹配结果 场景_模式匹配(场景节点类* 输入场景根, const 场景节点类* 模式场景根, const 结构体_场景模式匹配参数& p = {})
    {
        结构体_场景模式匹配结果 out{};
        if (!输入场景根 || !模式场景根) return out;

        auto r = 递归匹配节点(输入场景根, 模式场景根, p, out);

        out.匹配成功 = r.ok;
        out.总分 = r.score;
        return out;
    }
};


本能动作类::局部结果 本能动作类::递归匹配节点(
    基础信息节点类* 输入节点,
    const 基础信息节点类* 模式节点,
    const 结构体_场景模式匹配参数& p,
    结构体_场景模式匹配结果& io)
{
    局部结果 r{};
    if (!输入节点 || !模式节点) {
        r.ok = false;
        return r;
    }

    if (!模式节点->子) {
        return r; // 叶子：当前层已匹配完成
    }

    return 递归匹配子节点(输入节点, 模式节点, 是OR组(模式节点), p, io);
}

本能动作类::局部结果 本能动作类::递归匹配子节点(基础信息节点类* 输入节点, const 基础信息节点类* 模式父节点, bool OR模式, const 结构体_场景模式匹配参数& p, 结构体_场景模式匹配结果& io)
{
    局部结果 r{};
    const auto 模式子列表 = 枚举子节点_只读(模式父节点);

    if (OR模式) {
        bool any = false;
        I64 bestScore = (std::numeric_limits<I64>::min)();
        结构体_场景模式匹配结果 bestIo = io;

        for (const 基础信息节点类* patChild : 模式子列表) {
            auto tmp = io;

            auto pick = 挑选绑定(输入节点, patChild, p, tmp);
            if (!pick.ok) continue;

            基础信息节点类* boundIn = tmp.绑定表.back().输入节点;
            auto sub = 递归匹配节点(boundIn, patChild, p, tmp);
            if (!sub.ok) continue;

            const I64 score = pick.score + sub.score;
            if (score > bestScore) {
                bestScore = score;
                bestIo = std::move(tmp);
                any = true;
            }
        }

        if (!any) {
            r.ok = false;
            io.未满足模式节点.push_back(模式父节点);
            return r;
        }

        io = std::move(bestIo);
        r.score += bestScore;
        return r;
    }

    for (const 基础信息节点类* patChild : 模式子列表) {
        auto pick = 挑选绑定(输入节点, patChild, p, io);
        if (!pick.ok) {
            r.ok = false;
            return r;
        }

        r.score += pick.score;
        if (pick.ambiguous) r.ambiguous = true;

        基础信息节点类* boundIn = io.绑定表.back().输入节点;
        auto sub = 递归匹配节点(boundIn, patChild, p, io);
        if (!sub.ok) {
            r.ok = false;
            return r;
        }

        r.score += sub.score;
        if (sub.ambiguous) r.ambiguous = true;
    }

    return r;
}
