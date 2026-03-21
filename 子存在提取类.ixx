module;
#include <vector>
#include <queue>
#include <cstdint>
#include <algorithm>
#include <cmath>
#include <limits>
#include <type_traits>

export module 子存在提取模块;

import 点簇分割模块;     // 点簇 / 点簇增强结果
import 基础数据类型模块; // Vector3D / 等基础类型（如需要）



export struct 子存在提取参数
{
    // 连通性：false=4连通；true=8连通
    bool 使用8连通 = true;

    // 过滤策略：按 ROI 面积比例过滤（太小/太大通常不是“子存在”）
    double 最小面积比 = 0.05;  // 子像素数 / (ROI_w*ROI_h)
    double 最大面积比 = 0.80;

    // 最小像素数（与 最小面积比 取 max）
    int 最小像素数 = 80;

    // 噪声抑制：过滤贴近父 ROI 边缘的子块（粘连/边缘噪声常见）
    bool 过滤贴边 = false;
    int 贴边像素阈值 = 1;

    // 组装子 ROI 时的留白（>=0）
    int 子ROI留白 = 0;

    // 输出限制（防止爆炸）
    std::size_t 最大子块数 = 32;

    // 递归切分：对子块继续切分（纯 2D ROI 掩膜层面）
    bool 允许递归 = false;
    int 最大深度 = 2;

    // 彩色边缘屏障切分：用彩色轮廓内部的边缘把父掩膜“切开”，更容易得到真实子存在
    bool 使用颜色边缘切分 = true;
    // 颜色差阈值（L1，0~255*3），越大越保守；建议 25~45
    int  颜色边缘阈值 = 28;
    // 屏障膨胀半径（像素），避免“漏缝”把两个子块又连回去
    int  颜色屏障膨胀 = 1;
    // true=只在父前景内生成屏障（推荐）；false=允许屏障影响背景（一般没必要）
    bool 屏障只作用前景 = true;
};

export struct 子掩膜组件
{
    int umin = 0, vmin = 0, umax = 0, vmax = 0;
    int 裁剪宽 = 0, 裁剪高 = 0;
    int 像素数 = 0;
    std::vector<std::uint8_t> 裁剪掩膜; // 大小=裁剪宽*裁剪高，bbox坐标系
};

export class 子存在提取类 {
private:
    struct 点索引 { int u = 0; int v = 0; };

    struct 子连通组件
    {
        int umin = 0, umax = 0, vmin = 0, vmax = 0;
        int 像素数 = 0;
        std::vector<点索引> 像素列表;
    };

    template<class MaskT>
    static std::vector<子连通组件> 提取子连通组件(
        const std::vector<MaskT>& 父掩膜,
        int 父ROI_w,
        int 父ROI_h,
        const 子存在提取参数& sp)
    {
        std::vector<子连通组件> 子组件;
        if (父ROI_w <= 0 || 父ROI_h <= 0) return 子组件;
        if (父掩膜.size() != static_cast<std::size_t>(父ROI_w) * 父ROI_h) return 子组件;

        const int ROI面积 = 父ROI_w * 父ROI_h;
        if (ROI面积 <= 0) return 子组件;

        // 统计父前景像素
        int 父前景像素 = 0;
        for (auto v : 父掩膜) if (v) ++父前景像素;
        if (父前景像素 <= 0) return 子组件;

        const int minPix = std::max(
            sp.最小像素数,
            static_cast<int>(std::ceil(ROI面积 * sp.最小面积比)));

        std::vector<std::uint8_t> visited(static_cast<std::size_t>(ROI面积), 0);

        // 避免 MSVC 在 module 下对 std::array constexpr + 条件引用 触发 C2131：
        static constexpr int offsets4[4][2] = {
            {-1, 0}, { 1, 0}, { 0,-1}, { 0, 1}
        };
        static constexpr int offsets8[8][2] = {
            {-1,-1}, {-1, 0}, {-1, 1},
            { 0,-1},          { 0, 1},
            { 1,-1}, { 1, 0}, { 1, 1}
        };
        const int (*offsets)[2] = sp.使用8连通 ? offsets8 : offsets4;
        const int offsetsCount = sp.使用8连通 ? 8 : 4;

        auto idx = [父ROI_w](int u, int v) -> std::size_t {
            return static_cast<std::size_t>(v) * 父ROI_w + static_cast<std::size_t>(u);
            };

        for (int v = 0; v < 父ROI_h; ++v) {
            for (int u = 0; u < 父ROI_w; ++u) {
                std::size_t i = idx(u, v);
                if (!父掩膜[i] || visited[i]) continue;

                子连通组件 comp;
                comp.umin = comp.umax = u;
                comp.vmin = comp.vmax = v;

                std::queue<点索引> q;
                q.push({ u, v });
                visited[i] = 1;

                while (!q.empty()) {
                    auto p = q.front(); q.pop();
                    comp.像素列表.push_back(p);

                    comp.umin = std::min(comp.umin, p.u);
                    comp.umax = std::max(comp.umax, p.u);
                    comp.vmin = std::min(comp.vmin, p.v);
                    comp.vmax = std::max(comp.vmax, p.v);

                    for (int oi = 0; oi < offsetsCount; ++oi) {
                        const int nu = p.u + offsets[oi][0];
                        const int nv = p.v + offsets[oi][1];
                        if (nu < 0 || nu >= 父ROI_w || nv < 0 || nv >= 父ROI_h) continue;

                        std::size_t ni = idx(nu, nv);
                        if (父掩膜[ni] && !visited[ni]) {
                            visited[ni] = 1;
                            q.push({ nu, nv });
                        }
                    }
                }

                comp.像素数 = static_cast<int>(comp.像素列表.size());
                const double 面积比 = static_cast<double>(comp.像素数) / static_cast<double>(ROI面积);

                if (comp.像素数 < minPix) continue;
                if (面积比 < sp.最小面积比 || 面积比 > sp.最大面积比) continue;

                if (sp.过滤贴边) {
                    const int m = sp.贴边像素阈值;
                    const bool touch =
                        comp.umin <= m || comp.vmin <= m ||
                        comp.umax >= 父ROI_w - 1 - m ||
                        comp.vmax >= 父ROI_h - 1 - m;
                    if (touch) continue;
                }

                子组件.push_back(std::move(comp));
                if (子组件.size() >= sp.最大子块数) break;
            }
            if (子组件.size() >= sp.最大子块数) break;
        }

        std::sort(子组件.begin(), 子组件.end(),
            [](const 子连通组件& a, const 子连通组件& b) { return a.像素数 > b.像素数; });

        return 子组件;
    }

    template<class EnhancedT>
    static void 组装子增强(
        const EnhancedT& 父,
        const 子连通组件& comp,
        const 子存在提取参数& sp,
        EnhancedT& 子)
    {
        const int fw = 父.裁剪宽;
        const int fh = 父.裁剪高;

        // bbox + 留白
        int umin = std::max(0, comp.umin - sp.子ROI留白);
        int vmin = std::max(0, comp.vmin - sp.子ROI留白);
        int umax = std::min(fw - 1, comp.umax + sp.子ROI留白);
        int vmax = std::min(fh - 1, comp.vmax + sp.子ROI留白);

        const int cw = umax - umin + 1;
        const int ch = vmax - vmin + 1;

        // 子边界（全局像素坐标）
        子.边界 = 父.边界;
        子.边界.umin = 父.边界.umin + umin;
        子.边界.vmin = 父.边界.vmin + vmin;
        子.边界.umax = 父.边界.umin + umax;
        子.边界.vmax = 父.边界.vmin + vmax;

        子.裁剪宽 = cw;
        子.裁剪高 = ch;

        using MaskT = typename std::remove_reference_t<decltype(父.裁剪掩码)>::value_type;
        using ColT = typename std::remove_reference_t<decltype(父.裁剪颜色)>::value_type;

        子.裁剪掩码.assign(static_cast<std::size_t>(cw) * ch, MaskT{ 0 });
        子.裁剪颜色.assign(static_cast<std::size_t>(cw) * ch, ColT{});

        auto pidx = [fw](int u, int v) -> std::size_t { return static_cast<std::size_t>(v) * fw + static_cast<std::size_t>(u); };
        auto cidx = [cw](int u, int v) -> std::size_t { return static_cast<std::size_t>(v) * cw + static_cast<std::size_t>(u); };

        // 先复制颜色（包含留白区域）
        for (int v = 0; v < ch; ++v) {
            for (int u = 0; u < cw; ++u) {
                const int pu = umin + u;
                const int pv = vmin + v;
                const std::size_t pi = pidx(pu, pv);
                const std::size_t ci = cidx(u, v);
                if (pi < 父.裁剪颜色.size()) 子.裁剪颜色[ci] = 父.裁剪颜色[pi];
            }
        }

        // 再写入连通组件像素为 1（不包含留白区域）
        for (const auto& p : comp.像素列表) {
            if (p.u < umin || p.u > umax || p.v < vmin || p.v > vmax) continue;
            const int lu = p.u - umin;
            const int lv = p.v - vmin;
            子.裁剪掩码[cidx(lu, lv)] = MaskT{ 1 };
        }

        // 子点簇：从父点簇中过滤（确保与子掩码一致）
        子.簇.clear();
        using PixT = typename std::remove_reference_t<decltype(父.簇)>::value_type;

        for (const auto& pix : 父.簇) {
            const int lu = pix.u - 父.边界.umin;   // 转父ROI坐标
            const int lv = pix.v - 父.边界.vmin;
            if (lu < umin || lu > umax || lv < vmin || lv > vmax) continue;

            const int cu = lu - umin;
            const int cv = lv - vmin;
            const std::size_t ci = static_cast<std::size_t>(cv) * cw + static_cast<std::size_t>(cu);
            if (ci < 子.裁剪掩码.size() && 子.裁剪掩码[ci]) {
                PixT p2 = pix;
                子.簇.push_back(p2);
            }
        }
    }


    static inline int 颜色差_L1(const Color& a, const Color& b)
    {
        return std::abs((int)a.r - (int)b.r) + std::abs((int)a.g - (int)b.g) + std::abs((int)a.b - (int)b.b);
    }

    static inline 颜色_BGR8 转_BGR8(std::uint64_t sumR, std::uint64_t sumG, std::uint64_t sumB, std::uint64_t cnt)
    {
        颜色_BGR8 out{};
        if (cnt == 0) return out;
        out.r = (std::uint8_t)std::clamp<std::uint64_t>(sumR / cnt, 0, 255);
        out.g = (std::uint8_t)std::clamp<std::uint64_t>(sumG / cnt, 0, 255);
        out.b = (std::uint8_t)std::clamp<std::uint64_t>(sumB / cnt, 0, 255);
        return out;
    }

    // 子块平均色：直接用连通组件像素列表在“父ROI彩色图”上取样（最快/最稳）
    static inline 颜色_BGR8 计算平均颜色_按像素列表(
        const std::vector<点索引>& pixels,
        const std::vector<Color>& roiColor,
        int w,
        int h,
        std::uint64_t* out_cnt = nullptr)
    {
        if (w <= 0 || h <= 0) return {};
        const std::size_t area = (std::size_t)w * (std::size_t)h;
        if (roiColor.size() != area) return {};

        std::uint64_t sumR = 0, sumG = 0, sumB = 0, cnt = 0;
        for (const auto& p : pixels) {
            if ((unsigned)p.u >= (unsigned)w || (unsigned)p.v >= (unsigned)h) continue;
            const std::size_t i = (std::size_t)p.v * (std::size_t)w + (std::size_t)p.u;
            const Color c = roiColor[i];
            sumR += c.r; sumG += c.g; sumB += c.b;
            ++cnt;
        }
        if (out_cnt) *out_cnt = cnt;
        return 转_BGR8(sumR, sumG, sumB, cnt);
    }

    // 父块平均色：父前景 AND NOT(子并集) AND NOT(可选屏障)
    static inline 颜色_BGR8 计算平均颜色_父排除子(
        const std::vector<std::uint8_t>& parentMask,
        const std::vector<Color>& roiColor,
        const std::vector<std::uint8_t>& childUnion,
        const std::vector<std::uint8_t>* barrier,
        int w,
        int h,
        std::uint64_t* out_cnt = nullptr)
    {
        if (w <= 0 || h <= 0) return {};
        const std::size_t area = (std::size_t)w * (std::size_t)h;
        if (parentMask.size() != area || roiColor.size() != area || childUnion.size() != area) return {};
        if (barrier && barrier->size() != area) barrier = nullptr;

        std::uint64_t sumR = 0, sumG = 0, sumB = 0, cnt = 0;
        for (std::size_t i = 0; i < area; ++i) {
            if (!parentMask[i]) continue;
            if (childUnion[i]) continue;
            if (barrier && (*barrier)[i]) continue;
            const Color c = roiColor[i];
            sumR += c.r; sumG += c.g; sumB += c.b;
            ++cnt;
        }
        if (out_cnt) *out_cnt = cnt;
        return 转_BGR8(sumR, sumG, sumB, cnt);
    }

    // 用彩色边缘生成“屏障”，并从父掩膜中扣除屏障像素，得到用于连通组件的工作掩膜
    // 说明：
    // - 只依赖裁剪颜色 + 裁剪掩膜（ROI 坐标系）
    // - 屏障会把高颜色梯度处打断，用于把“贴在一起但颜色不同”的子块分开
    static std::vector<std::uint8_t> 生成工作掩膜_颜色屏障(
        const std::vector<std::uint8_t>& 父掩膜,
        const std::vector<Color>& 裁剪颜色,
        int w,
        int h,
        const 子存在提取参数& sp,
        std::vector<std::uint8_t>* out_barrier = nullptr)
    {
        const std::size_t area = (std::size_t)w * (std::size_t)h;
        std::vector<std::uint8_t> work = 父掩膜;
        if (w <= 1 || h <= 1) return work;
        if (父掩膜.size() != area || 裁剪颜色.size() != area) return work;

        std::vector<std::uint8_t> barrier(area, 0);

        auto idx = [w](int u, int v)->std::size_t { return (std::size_t)v * (std::size_t)w + (std::size_t)u; };

        const int thr = std::max(1, sp.颜色边缘阈值);

        for (int v = 0; v < h; ++v) {
            for (int u = 0; u < w; ++u) {
                const std::size_t i = idx(u, v);
                if (sp.屏障只作用前景 && !父掩膜[i]) continue;

                const Color c0 = 裁剪颜色[i];
                int best = 0;

                // 右 / 下（避免重复）
                if (u + 1 < w) {
                    const std::size_t j = idx(u + 1, v);
                    if (!sp.屏障只作用前景 || 父掩膜[j]) {
                        best = std::max(best, 颜色差_L1(c0, 裁剪颜色[j]));
                    }
                }
                if (v + 1 < h) {
                    const std::size_t j = idx(u, v + 1);
                    if (!sp.屏障只作用前景 || 父掩膜[j]) {
                        best = std::max(best, 颜色差_L1(c0, 裁剪颜色[j]));
                    }
                }

                if (best >= thr) barrier[i] = 1;
            }
        }

        // 膨胀屏障，防“细缝”重连
        const int r = std::clamp(sp.颜色屏障膨胀, 0, 4);
        if (r > 0) {
            std::vector<std::uint8_t> b2 = barrier;
            for (int v = 0; v < h; ++v) {
                for (int u = 0; u < w; ++u) {
                    if (!barrier[idx(u, v)]) continue;
                    for (int dv = -r; dv <= r; ++dv) {
                        for (int du = -r; du <= r; ++du) {
                            const int nu = u + du;
                            const int nv = v + dv;
                            if ((unsigned)nu >= (unsigned)w || (unsigned)nv >= (unsigned)h) continue;
                            b2[idx(nu, nv)] = 1;
                        }
                    }
                }
            }
            barrier.swap(b2);
        }

        // 从前景中扣除屏障
        for (std::size_t i = 0; i < area; ++i) {
            if (barrier[i]) work[i] = 0;
        }

        if (out_barrier) *out_barrier = barrier;
        return work;
    }

    // 一层切分
    std::vector<点簇增强结果> 从父点簇增强提取子点簇增强_一层(点簇增强结果& 父, const 子存在提取参数& sp) const
    {
        std::vector<点簇增强结果> out;
        if (父.裁剪宽 <= 0 || 父.裁剪高 <= 0) return out;
        if (父.裁剪掩码.empty()) return out;

        const std::vector<std::uint8_t>* mask_ptr = &父.裁剪掩码;
        std::vector<std::uint8_t> work_mask;
        std::vector<std::uint8_t> barrier_mask;
        bool used_barrier = false;

        if (sp.使用颜色边缘切分 &&
            !父.裁剪颜色.empty() &&
            父.裁剪颜色.size() == 父.裁剪掩码.size())
        {
            work_mask = 生成工作掩膜_颜色屏障(父.裁剪掩码, 父.裁剪颜色, 父.裁剪宽, 父.裁剪高, sp, &barrier_mask);
            mask_ptr = &work_mask;
            used_barrier = true;
        }

        auto comps = 提取子连通组件(*mask_ptr, 父.裁剪宽, 父.裁剪高, sp);

        // 如果屏障切分导致只剩 0/1 个子块，则回退到原始掩膜（避免“切过头”）
        if (used_barrier && comps.size() <= 1) {
            comps = 提取子连通组件(父.裁剪掩码, 父.裁剪宽, 父.裁剪高, sp);
        }
        out.reserve(comps.size());

        // ================================
        // 颜色回填：子块颜色 + 父块“排除子区域后的颜色”
        // 说明：
        // - 颜色字段统一用 点簇增强结果::颜色（颜色_BGR8）
        // - 父色：父前景 AND NOT(子并集) AND NOT(屏障)
        // - 子色：按连通组件像素列表在父ROI裁剪颜色上取样
        // ================================
        const bool hasColor = (!父.裁剪颜色.empty() && 父.裁剪颜色.size() == 父.裁剪掩码.size());
        std::vector<std::uint8_t> childUnion;
        if (hasColor) {
            const std::size_t area = 父.裁剪掩码.size();
            childUnion.assign(area, 0);
            auto idx = [w = 父.裁剪宽](int u, int v)->std::size_t {
                return (std::size_t)v * (std::size_t)w + (std::size_t)u;
            };
            for (const auto& comp : comps) {
                for (const auto& p : comp.像素列表) {
                    if ((unsigned)p.u >= (unsigned)父.裁剪宽 || (unsigned)p.v >= (unsigned)父.裁剪高) continue;
                    childUnion[idx(p.u, p.v)] = 1;
                }
            }

            // 父色：优先“排除子”，若父专属像素太少则退化为“父前景整体”
            std::uint64_t cntParentOnly = 0;
            auto parentOnly = 计算平均颜色_父排除子(
                父.裁剪掩码, 父.裁剪颜色, childUnion,
                used_barrier ? &barrier_mask : nullptr,
                父.裁剪宽, 父.裁剪高,
                &cntParentOnly);

            std::uint64_t cntFinal = cntParentOnly;

            // 退化：父专属像素太少时（例如子几乎占满父）用整体父前景
            if (cntParentOnly < 32) {
                std::vector<std::uint8_t> empty(area, 0);
                std::uint64_t cntAll = 0;
                auto all = 计算平均颜色_父排除子(父.裁剪掩码, 父.裁剪颜色, empty, nullptr, 父.裁剪宽, 父.裁剪高, &cntAll);
                if (cntAll > 0) parentOnly = all;
                if (cntAll > 0) cntFinal = cntAll;
            }
            // 若仍无有效像素，则保持父.颜色不变（默认 255,255,255 或上游已有值）
            if (cntFinal > 0) 父.颜色 = parentOnly;
        }

        for (const auto& comp : comps) {
            点簇增强结果 子{};
            组装子增强(父, comp, sp, 子);

            if (hasColor) {
                std::uint64_t cntChild = 0;
                子.颜色 = 计算平均颜色_按像素列表(comp.像素列表, 父.裁剪颜色, 父.裁剪宽, 父.裁剪高, &cntChild);
                // 极小子块颜色很不稳定，允许保持默认值（通常你也会在上层过滤掉）
                (void)cntChild;
            }

            out.push_back(std::move(子));
            if (out.size() >= sp.最大子块数) break;
        }
        return out;
    }


public:
    // --------------------------------------------------------------------
    // 从“父裁剪掩膜”提取子掩膜组件（bbox + 子掩膜），用于“只有观测ROI、没有点云/点簇增强”的场景
    //   - 父掩膜尺寸必须 = 父ROI_w * 父ROI_h
    //   - 返回的 子掩膜组件::裁剪掩膜 是 bbox 坐标系
    // --------------------------------------------------------------------
    std::vector<子掩膜组件> 从父裁剪掩膜提取子掩膜组件(
        const std::vector<std::uint8_t>& 父掩膜,
        int 父ROI_w,
        int 父ROI_h,
        const 子存在提取参数& sp) const
    {
        std::vector<子掩膜组件> out;
        if (父ROI_w <= 0 || 父ROI_h <= 0) return out;
        if (父掩膜.size() != static_cast<std::size_t>(父ROI_w) * 父ROI_h) return out;

        const auto comps = 提取子连通组件(父掩膜, 父ROI_w, 父ROI_h, sp);
        out.reserve(comps.size());

        auto idx = [父ROI_w](int u, int v) -> std::size_t {
            return static_cast<std::size_t>(v) * 父ROI_w + static_cast<std::size_t>(u);
            };

        for (const auto& comp : comps) {
            // bbox + 留白
            int umin = std::max(0, comp.umin - sp.子ROI留白);
            int vmin = std::max(0, comp.vmin - sp.子ROI留白);
            int umax = std::min(父ROI_w - 1, comp.umax + sp.子ROI留白);
            int vmax = std::min(父ROI_h - 1, comp.vmax + sp.子ROI留白);

            const int cw = umax - umin + 1;
            const int ch = vmax - vmin + 1;
            if (cw <= 0 || ch <= 0) continue;

            子掩膜组件 r{};
            r.umin = umin; r.vmin = vmin; r.umax = umax; r.vmax = vmax;
            r.裁剪宽 = cw; r.裁剪高 = ch;
            r.像素数 = comp.像素数;
            r.裁剪掩膜.assign(static_cast<std::size_t>(cw) * ch, 0);

            auto cidx = [cw](int u, int v) -> std::size_t {
                return static_cast<std::size_t>(v) * cw + static_cast<std::size_t>(u);
                };

            // 写入连通组件像素为 1（不包含留白区域之外的像素）
            for (const auto& p : comp.像素列表) {
                if (p.u < umin || p.u > umax || p.v < vmin || p.v > vmax) continue;
                r.裁剪掩膜[cidx(p.u - umin, p.v - vmin)] = 1;
            }

            // 可选：如果留白区域也想“保留父掩膜”，可以在这里再做一次父掩膜拷贝
            // 当前选择：留白区域保持 0（更利于子块纯净）

            out.push_back(std::move(r));
            if (out.size() >= sp.最大子块数) break;
        }
        return out;
    }

    // 从“父点簇增强结果”提取子点簇增强结果（可选递归）
    // --------------------------------------------------------------------
    std::vector<点簇增强结果> 从父点簇增强提取子点簇增强(点簇增强结果& 父, const 子存在提取参数& sp) const
    {
        if (!sp.允许递归 || sp.最大深度 <= 1) {
            return 从父点簇增强提取子点簇增强_一层(父, sp);
        }

        // 递归：把“当前层结果”继续切分，最终返回所有叶子子块
        std::vector<点簇增强结果> all;
        std::vector<点簇增强结果> frontier = 从父点簇增强提取子点簇增强_一层(父, sp);

        for (int depth = 1; depth < sp.最大深度 && !frontier.empty(); ++depth) {
            std::vector<点簇增强结果> next;
            for (auto& node : frontier) {
                auto kids = 从父点簇增强提取子点簇增强_一层(node, sp);
                if (kids.empty()) {
                    all.push_back(node);
                }
                else {
                    next.insert(next.end(), std::make_move_iterator(kids.begin()), std::make_move_iterator(kids.end()));
                }
                if (all.size() + next.size() >= sp.最大子块数) break;
            }
            frontier = std::move(next);
            if (all.size() >= sp.最大子块数) break;
        }

        // frontier 剩余的也算叶子
        for (auto& n : frontier) {
            all.push_back(std::move(n));
            if (all.size() >= sp.最大子块数) break;
        }
        return all;
    }
};

