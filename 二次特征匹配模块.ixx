module;

#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

export module 二次特征匹配模块;

import 主信息定义模块;
import 通用函数模块;
import 日志模块;
import 基础数据类型模块;

namespace 二次特征匹配模块_detail {
  

    inline bool 区间有效(const I64区间 & 区间值) noexcept {
        return 区间值.高值 >= 区间值.低值;
    }

    inline bool 词性主键相等(const 词性节点类* a, const 词性节点类* b) noexcept {
        if (a == b) return true;
        if (!a || !b) return false;
        // 以主键作为稳定比较
        const auto& ka = a->获取主键();
        const auto& kb = b->获取主键();
        if (!ka.empty() && !kb.empty()) return ka == kb;
        return false;
    }

    inline const 二次特征主信息类* MI(const 二次特征节点类* n) noexcept {
        return n ? dynamic_cast<const 二次特征主信息类*>(n->主信息) : nullptr;
    }

    inline const 二次特征主信息类* MI(const 二次特征节点类& n) noexcept {
        return dynamic_cast<const 二次特征主信息类*>(n.主信息);
    }

    inline bool 概念模板_相等_弱(const 二次特征节点类* a, const 二次特征节点类* b) noexcept {
        if (a == b) return true;
        if (!a || !b) return false;

        const auto* ma = MI(a);
        const auto* mb = MI(b);
        if (!ma || !mb) return false;

        // 1) 先尝试概念名称
        if (ma->概念名称 && mb->概念名称) {
            if (词性主键相等(ma->概念名称, mb->概念名称)) return true;
        }

        // 2) 再退化为：同链键 + 同区间
        if (!ma->度量签名_链键.empty() && ma->度量签名_链键 == mb->度量签名_链键) {
            if (ma->区间层级 == mb->区间层级 && ma->标量区间.低值 == mb->标量区间.低值 && ma->标量区间.高值 == mb->标量区间.高值) {
                return true;
            }
        }
        return false;
    }

    inline bool 指针约束匹配(const 基础信息节点类* 条件侧, const 基础信息节点类* 状态侧, bool 条件空视为通配) noexcept {
        if (!条件侧) return 条件空视为通配;
        return 条件侧 == 状态侧;
    }

} // namespace 二次特征匹配模块_detail


export namespace 二次特征匹配模块 {

    using I64 = std::int64_t;

    export struct 结构_匹配配置 {
        bool 使用对象绑定 = true;          // 是否要求 主体/客体/左对象/右对象 一致（条件侧非空才约束）
        bool 条件空指针视为通配 = true;    // 条件侧某个绑定为空时，是否视为不约束
        bool 允许用概念名称弱匹配 = true;  // 指针不同但主键相同，仍认为概念匹配
    };

    export struct 结构_匹配结果 {
        I64 命中数 = 0;
        I64 摘要数 = 0;
        I64 目标数 = 0;

        I64 精确率_Q10000 = 0;
        I64 召回率_Q10000 = 0;
        I64 得分_Q10000 = 0; // F1：2*hit/(a+b)
    };

    // ============================================================
    // 二次特征满足判定：state 是否满足 condition
    // - 最小契约：链键相同
    // - 可选：对象绑定约束
    // - 概念约束优先级：概念模板 > 概念名称 > 标量区间 > 无概念约束
    // ============================================================
    export inline bool 二次特征_满足(
        const 二次特征主信息类* 条件,
        const 二次特征主信息类* 状态,
        const 结构_匹配配置& 配置 = {})
    {
        using namespace 二次特征匹配模块_detail;
        if (!条件 || !状态) return false;
        if (条件->度量签名_链键.empty() || 状态->度量签名_链键.empty()) return false;
        if (条件->度量签名_链键 != 状态->度量签名_链键) return false;

        if (配置.使用对象绑定) {
            if (!指针约束匹配(条件->主体, 状态->主体, 配置.条件空指针视为通配)) return false;
            if (!指针约束匹配(条件->客体, 状态->客体, 配置.条件空指针视为通配)) return false;
            if (!指针约束匹配(条件->左对象, 状态->左对象, 配置.条件空指针视为通配)) return false;
            if (!指针约束匹配(条件->右对象, 状态->右对象, 配置.条件空指针视为通配)) return false;
        }

        // 1) 概念模板约束
        if (条件->概念模板) {
            if (状态->概念模板 == 条件->概念模板) return true;

            if (配置.允许用概念名称弱匹配 && 状态->概念模板) {
                if (概念模板_相等_弱(状态->概念模板, 条件->概念模板)) return true;
            }
            return false;
        }

        // 2) 概念名称约束
        if (条件->概念名称) {
            // 状态直接填概念名称
            if (状态->概念名称 && 词性主键相等(条件->概念名称, 状态->概念名称)) return true;

            // 状态只填了模板
            if (状态->概念模板) {
                const auto* mt = dynamic_cast<const 二次特征主信息类*>(状态->概念模板->主信息);
                if (mt && mt->概念名称 && 词性主键相等(条件->概念名称, mt->概念名称)) return true;
            }
            return false;
        }

        // 3) 区间约束（标量区间）
        if (区间有效(条件->标量区间)) {
            // state 若给了区间，做区间交；否则做点命中
            if (区间有效(状态->标量区间)) {
                const I64 lo = std::max<I64>(条件->标量区间.低值, 状态->标量区间.低值);
                const I64 hi = std::min<I64>(条件->标量区间.高值, 状态->标量区间.高值);
                return hi >= lo;
            }
            return (状态->标量值 >= 条件->标量区间.低值 && 状态->标量值 <= 条件->标量区间.高值);
        }

        // 4) 无概念约束：只要链键 + 对象绑定匹配就算满足
        return true;
    }


    // ============================================================
    // 条件索引满足：条件索引(值集) 是否被 当前判定集(指针集) 覆盖
    // - 返回 true 表示“全部满足”
    // - out_满足数 可用于统计/调试
    // ============================================================
    export inline bool 条件索引_满足(
        const std::vector<二次特征节点类>& 条件索引,
        const std::vector<const 二次特征节点类*>& 当前判定集,
        I64* out_满足数 = nullptr,
        const 结构_匹配配置& 配置 = {})
    {
        using namespace 二次特征匹配模块_detail;
        if (out_满足数) *out_满足数 = 0;

        if (条件索引.empty()) {
            // 空条件：恒真
            if (out_满足数) *out_满足数 = 0;
            return true;
        }

        if (当前判定集.empty()) {
            // 没有可用判定集：保守上层可选择“视为通过”或“视为不确定”。这里返回 false。
            // 注意：任务执行模块会在“前置为空”场景下选择绕过此判定。
            return false;
        }

        I64 命中 = 0;
        for (const auto& c : 条件索引) {
            const auto* cmi = MI(c);
            if (!cmi) continue;

            bool ok = false;
            for (const auto* sNode : 当前判定集) {
                const auto* smi = MI(sNode);
                if (!smi) continue;
                if (二次特征_满足(cmi, smi, 配置)) {
                    ok = true;
                    break;
                }
            }
            if (ok) ++命中;
        }

        if (out_满足数) *out_满足数 = 命中;
        return 命中 >= (I64)条件索引.size();
    }


    // ============================================================
    // 结果摘要匹配：摘要(值集) 与 目标(指针集)
    // - 用“摘要为条件，目标为状态”的方向做匹配
    // - 命中去重：一个目标最多贡献一次命中
    // ============================================================
    export inline 结构_匹配结果 结果摘要_匹配目标(
        const std::vector<二次特征节点类>& 摘要索引,
        const std::vector<const 二次特征节点类*>& 目标判定集,
        const 结构_匹配配置& 配置 = {})
    {
        using namespace 二次特征匹配模块_detail;

        结构_匹配结果 out{};
        out.摘要数 = (I64)摘要索引.size();
        out.目标数 = (I64)目标判定集.size();

        if (摘要索引.empty() || 目标判定集.empty()) {
            out.命中数 = 0;
            out.精确率_Q10000 = 0;
            out.召回率_Q10000 = 0;
            out.得分_Q10000 = 0;
            return out;
        }

        std::vector<const 二次特征主信息类*> 目标MI;
        目标MI.reserve(目标判定集.size());
        for (auto* n : 目标判定集) {
            if (auto* mi = MI(n)) 目标MI.push_back(mi);
            else 目标MI.push_back(nullptr);
        }

        std::vector<bool> 已用;
        已用.assign(目标MI.size(), false);

        I64 hit = 0;
        for (const auto& s : 摘要索引) {
            const auto* smi = MI(s);
            if (!smi) continue;

            int best = -1;
            for (std::size_t i = 0; i < 目标MI.size(); ++i) {
                if (已用[i]) continue;
                const auto* tmi = 目标MI[i];
                if (!tmi) continue;
                if (二次特征_满足(smi, tmi, 配置)) {
                    best = (int)i;
                    break;
                }
            }
            if (best >= 0) {
                已用[(std::size_t)best] = true;
                ++hit;
            }
        }

        out.命中数 = hit;

        auto 比率Q = [](I64 num, I64 den) -> I64 {
            if (den <= 0) return 0;
            if (num <= 0) return 0;
            const long double x = (long double)num * 10000.0L / (long double)den;
            I64 r = (I64)std::llround((double)x);
            if (r < 0) r = 0;
            if (r > 10000) r = 10000;
            return r;
        };

        out.精确率_Q10000 = 比率Q(out.命中数, out.摘要数);
        out.召回率_Q10000 = 比率Q(out.命中数, out.目标数);
        out.得分_Q10000 = 比率Q(2 * out.命中数, out.摘要数 + out.目标数);
        return out;
    }

} // namespace 二次特征匹配模块
