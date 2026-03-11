module;
#define _USE_MATH_DEFINES
#include <cmath>
#include <unordered_set>


// 点簇分割模块.ixx（去 OpenCV 版本，基于 结构体_原始场景帧）
export module 点簇分割模块;

import 基础数据类型模块;
import 通用函数模块;

import <vector>;
import <cstdint>;
import <cmath>;
import <algorithm>;
import <limits>;
import <utility>;
import <array>;
import <type_traits>;

import <chrono>;

export struct 点簇分割参数 {
    // 统一单位约定：所有深度/距离/尺寸阈值均为“毫米 (mm)”。

    // ===== 深度有效性过滤 =====
    double  最小深度 = 150.0;           // mm
    double  最大深度 = 8000.0;          // mm
    bool    忽略无效点 = true;          // z<=0 或 NaN 的点直接丢弃

    // ===== 连通性判断 =====
    double  邻域最大三维距离 = 40.0;    // mm（点云可用时优先）
    double  邻域最大深度差 = 60.0;      // mm（点云不可用时退化）
    bool    使用8邻域 = true;           // false=4邻域

    // ===== 性能与噪声过滤 =====
    int     采样步长 = 1;               // >1 跳点扫描加速
    int     最小点数 = 80;              // 小簇直接丢弃

    // ===== 裁剪输出控制 =====
    bool    输出裁剪图 = true;          // 输出 bbox+边距 的原彩图
    bool    输出裁剪掩码 = true;        // 输出二值掩码
    int     裁剪边距 = 2;               // bbox 外扩像素
    bool    掩码膨胀一次 = true;        // 修复断裂
    bool    掩码填洞 = true;            // 更封闭
    int     最大裁剪像素 = 256 * 256;   // 防爆内存

    // ===== 轮廓与外观增强控制 =====
    double  边缘颜色权重 = 0.35;        // 0~1：平均颜色时边缘像素权重（越小越鲁棒）

    // ===== 质量计算开关 =====
    bool    计算PCA主方向 = true;
    bool    计算质量分 = true;
    bool    计算颜色一致性 = true;
    bool    计算深度噪声 = true;
    bool    计算边界噪声 = true;

    // ===== 调试/精细匹配输出 =====
    bool    输出边界点序列 = true;      // 建议默认开：边界像素序列(v<<32|u)
    bool    输出轮廓3D点 = true;         // 建议默认开：轮廓3D（点云反投影）

    // ===== 轮廓尺度（8*2^N）控制 =====
    // 848x480 这种分辨率下，“正方形标准边长”自然会走到 1024(=8*2^7)，所以默认给 1024 / N=7 很合适。
    int     轮廓最大边长 = 1024;        // 0 表示不限制（但你当前 v2 实现里 0 会出坑，见下方提示）
    int     轮廓最大N = 7;              // 0..10；默认 7 => 8*2^7=1024

    // ===== 彩色轮廓参数（映射到 彩色边缘轮廓参数）=====
    int     彩色轮廓差分阈值 = 28;      // 1..255；越大越“保守”（更少边缘点）
    int     彩色轮廓采样步长 = 3;       // 1..；越大越省点
    int     彩色轮廓最大点数 = 4096;    // 建议 2k~10k；太大容易内存飙升

    // ===== 占据金字塔（若后续要在本模块生成，可用它做开关/层数）=====
    int     占据金字塔层数 = 0;         // 0=不生成；>0=生成层数（例如 5/6/7）
  
    bool    过滤尺寸为零的簇 = true;
    double  尺寸为零阈值_mm = 1e-3; // 0.001mm，视为“零”
};


export struct 点簇性能统计 {
    // 单位：微秒(us)。统计的是“本次调用 分割点簇/分割点簇_增强”累计耗时。
    std::uint64_t 调用次数 = 0;

    std::uint64_t floodfill_us = 0;
    std::uint64_t pca_us = 0;
    std::uint64_t contour_extract_us = 0;
    std::uint64_t contour_encode_us = 0;
    std::uint64_t color_stats_us = 0;
    std::uint64_t roi_crop_us = 0;
    std::uint64_t total_us = 0;

    std::uint64_t 输出簇数 = 0;
    std::uint64_t 输出像素点数 = 0;

    void 清零() { *this = {}; }

    double floodfill_ms() const { return floodfill_us / 1000.0; }
    double pca_ms() const { return pca_us / 1000.0; }
    double contour_extract_ms() const { return contour_extract_us / 1000.0; }
    double contour_encode_ms() const { return contour_encode_us / 1000.0; }
    double color_stats_ms() const { return color_stats_us / 1000.0; }
    double roi_crop_ms() const { return roi_crop_us / 1000.0; }
    double total_ms() const { return total_us / 1000.0; }
};


// ============================================================================
// 深度预处理（空间滤波 + 时间滤波 + 洞填补）
// 说明：本模块的 点簇分割类 以 结构体_原始场景帧.深度 / 点云 为输入。
//      为了让 “中心/尺寸/PCA/轮廓” 更稳定，建议在生成点云之前先对深度做预处理：
//      1) 空间滤波：边缘保留的邻域平滑（抑制椒盐/飞点）
//      2) 时间滤波：跨帧指数平滑（抑制抖动）
//      3) 洞填补：对无效深度的小孔做邻域补全（减少轮廓破碎）
//
//      这里实现的是“去 OpenCV / 去 RealSense SDK 依赖”的轻量版本：
//      - 你可以在相机模块中：先处理深度 -> 再反投影生成点云 -> 再进入点簇分割。
// ============================================================================

export struct 深度滤波参数
{
    // 深度有效性///性能吞金兽
//    bool    忽略无效点 = true;
//    double  最小深度 = 10.0;        // <=0 表示不限制
//    double  最大深度 = 10000.0;        // <=0 表示不限制
//
//    // --- 空间滤波 ---
//    bool    启用空间滤波 = true;
//    int     空间半径 = 1;           // 1=3x3, 2=5x5
//    double  空间最大深度差 = 20;  // 毫米：邻域差超过此值不参与平均（保边）
//    double  空间平滑强度 = 0.55;    // 0~1：越大越平滑
//
//    // --- 时间滤波 ---
//    bool    启用时间滤波 = true;
//    double  时间平滑强度 = 0.4;     // 0~1：越大越“跟随当前帧”
//    double  时间最大深度差 = 30;  // 毫米：差太大则认为发生遮挡/跳变，不做融合
//    bool    无效继承上一帧 = false; // 当前无效时是否沿用上一帧（谨慎开启）
//
//    // --- 洞填补 ---
//    bool    启用洞填补 = true;
//    int     洞填补半径 = 2;         // 2=5x5 邻域
//    int     洞填补迭代 = 1;         // 1~2 通常足够
    
    // 深度有效性///性能吞金兽
    bool    忽略无效点 = true;
    double  最小深度 = 150;        // <=0 表示不限制
    double  最大深度 = 2500.0;        // <=0 表示不限制

    // --- 空间滤波 ---
    bool    启用空间滤波 = true;
    int     空间半径 = 1;           // 1=3x3, 2=5x5
    double  空间最大深度差 = 20;  // 毫米：邻域差超过此值不参与平均（保边）
    double  空间平滑强度 = 0.65;    // 0~1：越大越平滑

    // --- 时间滤波 ---
    bool    启用时间滤波 = true;
    double  时间平滑强度 = 0.6;     // 0~1：越大越“跟随当前帧”
    double  时间最大深度差 = 40;  // 毫米：差太大则认为发生遮挡/跳变，不做融合
    bool    无效继承上一帧 = false; // 当前无效时是否沿用上一帧（谨慎开启）

    // --- 洞填补 ---
    bool    启用洞填补 = false;
    int     洞填补半径 = 2;         // 2=5x5 邻域
    int     洞填补迭代 = 1;         // 1~2 通常足够

};

export class 深度滤波链类
{
private:
    int 宽 = 0;
    int 高 = 0;
    std::vector<double> 上一帧;   // 上一帧“输出”（用于时间滤波）

    static inline bool 深度有效(double z, const 深度滤波参数& p)
    {
        if (p.忽略无效点 && (!std::isfinite(z) || z <= 0.0)) return false;
        if (p.最小深度 > 0.0 && z < p.最小深度) return false;
        if (p.最大深度 > 0.0 && z > p.最大深度) return false;
        return true;
    }

    static inline double 取中位数(std::vector<double>& v)
    {
        if (v.empty()) return 0.0;
        const std::size_t mid = v.size() / 2;
        std::nth_element(v.begin(), v.begin() + mid, v.end());
        return v[mid];
    }

    static std::vector<double> 空间滤波(
        const std::vector<double>& in,
        int w, int h,
        const 深度滤波参数& p)
    {
        std::vector<double> out = in;
        if (!p.启用空间滤波) return out;

        const int r = std::max(1, p.空间半径);
        const double a = std::clamp(p.空间平滑强度, 0.0, 1.0);
        const double delta = std::max(1e-9, p.空间最大深度差);

        auto at = [&](int x, int y) -> double {
            return in[static_cast<std::size_t>(y) * w + x];
            };

        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                const std::size_t id = static_cast<std::size_t>(y) * w + x;
                const double z0 = in[id];
                if (!深度有效(z0, p)) continue;

                double sum = 0.0;
                int cnt = 0;

                const int x0 = std::max(0, x - r);
                const int x1 = std::min(w - 1, x + r);
                const int y0 = std::max(0, y - r);
                const int y1 = std::min(h - 1, y + r);

                for (int yy = y0; yy <= y1; ++yy) {
                    for (int xx = x0; xx <= x1; ++xx) {
                        const double zn = at(xx, yy);
                        if (!深度有效(zn, p)) continue;
                        if (std::abs(zn - z0) > delta) continue; // 保边
                        sum += zn;
                        ++cnt;
                    }
                }

                if (cnt > 0) {
                    const double avg = sum / cnt;
                    out[id] = (1.0 - a) * z0 + a * avg;
                }
            }
        }
        return out;
    }

    std::vector<double> 时间滤波(
        const std::vector<double>& in,
        int w, int h,
        const 深度滤波参数& p)
    {
        std::vector<double> out = in;
        if (!p.启用时间滤波) return out;

        const double a = std::clamp(p.时间平滑强度, 0.0, 1.0);
        const double delta = std::max(1e-9, p.时间最大深度差);

        if ((int)上一帧.size() != w * h) {
            // 分辨率变化或首次：直接用当前帧作为历史
            上一帧 = in;
            宽 = w; 高 = h;
            return out;
        }

        for (int i = 0; i < w * h; ++i) {
            const double z = in[static_cast<std::size_t>(i)];
            const double zp = 上一帧[static_cast<std::size_t>(i)];

            const bool zv = 深度有效(z, p);
            const bool pv = 深度有效(zp, p);

            if (zv && pv) {
                if (std::abs(z - zp) <= delta) {
                    out[static_cast<std::size_t>(i)] = a * z + (1.0 - a) * zp;
                }
                else {
                    // 跳变：直接跟随当前帧（避免拖影）
                    out[static_cast<std::size_t>(i)] = z;
                }
            }
            else if (!zv && pv && p.无效继承上一帧) {
                out[static_cast<std::size_t>(i)] = zp;
            }
            // 其它情况：保持 in
        }

        // 更新历史（用 out，避免“滤波被回写成原始”）
        上一帧 = out;
        宽 = w; 高 = h;
        return out;
    }

    static std::vector<double> 洞填补(
        const std::vector<double>& in,
        int w, int h,
        const 深度滤波参数& p)
    {
        std::vector<double> out = in;
        if (!p.启用洞填补) return out;

        const int r = std::max(1, p.洞填补半径);
        const int iters = std::max(1, p.洞填补迭代);

        auto idx = [&](int x, int y) -> std::size_t {
            return static_cast<std::size_t>(y) * w + x;
            };

        for (int iter = 0; iter < iters; ++iter) {
            std::vector<double> cur = out;

            for (int y = 0; y < h; ++y) {
                for (int x = 0; x < w; ++x) {
                    const std::size_t id = idx(x, y);
                    const double z0 = cur[id];
                    if (深度有效(z0, p)) continue; // 只填无效

                    std::vector<double> neigh;
                    neigh.reserve((2 * r + 1) * (2 * r + 1));

                    const int x0 = std::max(0, x - r);
                    const int x1 = std::min(w - 1, x + r);
                    const int y0 = std::max(0, y - r);
                    const int y1 = std::min(h - 1, y + r);

                    for (int yy = y0; yy <= y1; ++yy) {
                        for (int xx = x0; xx <= x1; ++xx) {
                            const double zn = cur[idx(xx, yy)];
                            if (!深度有效(zn, p)) continue;
                            neigh.push_back(zn);
                        }
                    }

                    if (!neigh.empty()) {
                        // 用中位数补洞，鲁棒且不易被飞点拉偏
                        out[id] = 取中位数(neigh);
                    }
                }
            }
        }

        return out;
    }

public:
    void 重置()
    {
        宽 = 0; 高 = 0;
        上一帧.clear();
    }

    // 处理深度：返回新向量（不改输入）
    std::vector<double> 处理深度(const std::vector<double>& depth, int w, int h, const 深度滤波参数& p)
    {
        if (w <= 0 || h <= 0 || (int)depth.size() != w * h) return depth;

        std::vector<double> d = depth;

        // 推荐顺序：空间 -> 时间 -> 洞填补
        d = 空间滤波(d, w, h, p);
        d = 时间滤波(d, w, h, p);
        d = 洞填补(d, w, h, p);

        // 时间滤波内部已经更新 上一帧
        if (!p.启用时间滤波) {
            // 若没开时间滤波，也保持历史为当前输出，方便用户下次开启
            上一帧 = d; 宽 = w; 高 = h;
        }

        return d;
    }

    // 直接就地更新帧.深度（供相机模块在生成点云之前调用）
    void 处理到帧(结构体_原始场景帧& 帧, const 深度滤波参数& p)
    {
        if (!帧.有效深度()) return;
        const int w = 帧.宽度;
        const int h = 帧.高度;
        if (w <= 0 || h <= 0) return;
        if ((int)帧.深度.size() != w * h) return;

        帧.深度 = 处理深度(帧.深度, w, h, p);
    }
};


// ============================================================================
// 深度边缘轮廓提取（2D 深度边缘 -> 反投影 3D）
// 说明：
// - 相比“用簇内全部像素做占据”，从深度不连续边缘提轮廓通常更干净
// - 建议流程：深度预处理（空间+时间+洞填补） -> 点云 -> 点簇 -> 深度边缘轮廓 -> 3D 轮廓
// ============================================================================

export struct 深度边缘轮廓参数
{
    // 毫米：相邻像素深度差超过该值则认为出现深度边缘
    double 深度边缘阈值 = 20;
    // false=4 邻域；true=8 邻域
    bool   使用8邻域 = false;
    // >1 会下采样边缘点（省内存，后续比对更快）
    int    采样步长 = 1;
    // 防止某些大簇边缘点过多
    int    最大轮廓点数 = 4096;
};


// ============================================================================
// 点簇观测质量（可选输出，用于世界树融合按质量加权）
// 说明：不改变 点簇增强结果 的结构也能把质量信息传出去。
// ============================================================================

export struct 点簇质量信息 {
    std::uint32_t 有效点数 = 0;
    double        有效点比例 = 0.0;     // 有效点数 / 簇.size()
    double        深度噪声_MAD = 0.0;   // 深度中位绝对偏差（毫米，鲁棒）
    double        PCA残差 = 1.0;        // 越小越好
    double        边界噪声 = 0.0;       // 越小越好（深度梯度统计）
    double        颜色一致性 = 0.0;     // 0~1，越大越好
    double        质量分 = 0.0;         // 0~1

    Color     平均颜色{};
    Vector3D      主方向1{ 1,0,0 };
    Vector3D      主方向2{ 0,1,0 };
    Vector3D      主方向3{ 0,0,1 };
};

// ============================================================================
// 成员探测（避免 if constexpr(requires(...))）
// 说明：你清理/裁剪 点簇增强结果/参数结构体 字段时，本文件仍可编译。
// ============================================================================

// 从“点簇像素集合”提取深度边缘轮廓像素（可选同时输出 3D 轮廓点：从点云取对应像素的 3D 点）
export inline void 提取轮廓像素_深度边缘(
    const 结构体_原始场景帧& 帧,
    const 点簇& 簇,
    const 点簇边界框& box,
    const 点簇分割参数& 分割参数,
    std::vector<std::uint64_t>& out边界像素,
    std::vector<Vector3D>* out轮廓3D = nullptr,
    const 深度边缘轮廓参数& p = {})
{
    out边界像素.clear();
    if (out轮廓3D) out轮廓3D->clear();

    if (!帧.有效深度() || 簇.empty()) return;

    const int w = 帧.宽度;
    const int h = 帧.高度;
    if (w <= 0 || h <= 0) return;
    if ((int)帧.深度.size() != w * h) return;

    const int bw = std::max(1, box.umax - box.umin + 1);
    const int bh = std::max(1, box.vmax - box.vmin + 1);

    // 局部 membership mask：快速判断邻居是否同簇
    std::vector<std::uint8_t> m(static_cast<std::size_t>(bw) * static_cast<std::size_t>(bh), 0);
    for (const auto& pix : 簇) {
        const int lu = pix.u - box.umin;
        const int lv = pix.v - box.vmin;
        if ((unsigned)lu >= (unsigned)bw || (unsigned)lv >= (unsigned)bh) continue;
        m[static_cast<std::size_t>(lv) * static_cast<std::size_t>(bw) + static_cast<std::size_t>(lu)] = 1;
    }

    const int offsets4[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
    const int offsets8[8][2] = { {1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1} };
    const auto* offsets = p.使用8邻域 ? offsets8 : offsets4;
    const int offset_count = p.使用8邻域 ? 8 : 4;

    const int step = std::max(1, p.采样步长);
    out边界像素.reserve(std::min<std::size_t>(簇.size() / (std::size_t)step + 16, (std::size_t)p.最大轮廓点数));
    if (out轮廓3D) out轮廓3D->reserve(out边界像素.capacity());

    auto 深度有效 = [&](double z)->bool {
        if (分割参数.忽略无效点 && (!std::isfinite(z) || z <= 0.0)) return false;
        if (分割参数.最小深度 > 0.0 && z < 分割参数.最小深度) return false;
        if (分割参数.最大深度 > 0.0 && z > 分割参数.最大深度) return false;
        return true;
        };

    auto 取点云 = [&](int u, int v, Vector3D& outP)->bool {
        if (!帧.有效点云()) return false;
        if ((unsigned)u >= (unsigned)w || (unsigned)v >= (unsigned)h) return false;
        const std::size_t id = static_cast<std::size_t>(v) * static_cast<std::size_t>(w) + static_cast<std::size_t>(u);
        if (id >= 帧.点云.size()) return false;
        outP = 帧.点云[id];
        return std::isfinite(outP.x) && std::isfinite(outP.y) && std::isfinite(outP.z) && outP.z > 0.0;
        };

    int picked = 0;
    for (std::size_t i = 0; i < 簇.size(); i += (std::size_t)step) {
        const auto& pix = 簇[i];
        const int u = pix.u;
        const int v = pix.v;
        if ((unsigned)u >= (unsigned)w || (unsigned)v >= (unsigned)h) continue;

        const double z = 帧.深度[static_cast<std::size_t>(v) * static_cast<std::size_t>(w) + static_cast<std::size_t>(u)];
        if (!深度有效(z)) continue;

        bool is_edge = false;
        for (int k = 0; k < offset_count; ++k) {
            const int nu = u + offsets[k][0];
            const int nv = v + offsets[k][1];
            if ((unsigned)nu >= (unsigned)w || (unsigned)nv >= (unsigned)h) { is_edge = true; break; }

            // 同簇邻居不算轮廓
            const int lnu = nu - box.umin;
            const int lnv = nv - box.vmin;
            if ((unsigned)lnu < (unsigned)bw && (unsigned)lnv < (unsigned)bh) {
                if (m[static_cast<std::size_t>(lnv) * static_cast<std::size_t>(bw) + static_cast<std::size_t>(lnu)]) continue;
            }

            const double nz = 帧.深度[static_cast<std::size_t>(nv) * static_cast<std::size_t>(w) + static_cast<std::size_t>(nu)];
            if (!深度有效(nz)) { is_edge = true; break; }

            if (std::fabs(nz - z) >= p.深度边缘阈值) { is_edge = true; break; }
        }

        if (!is_edge) continue;

        out边界像素.push_back(通用函数模块::打包像素UV(u, v));
        if (out轮廓3D) {
            Vector3D P{};
            if (取点云(u, v, P)) out轮廓3D->push_back(P);
        }

        if (++picked >= p.最大轮廓点数) break;
    }
}

// 便捷：直接得到 3D 轮廓点（反投影结果）
export inline std::vector<Vector3D> 提取轮廓3D_深度边缘(
    const 结构体_原始场景帧& 帧,
    const 点簇& 簇,
    const 点簇边界框& box,
    const 点簇分割参数& 分割参数,
    const 深度边缘轮廓参数& p = {})
{
    std::vector<std::uint64_t> edges;
    std::vector<Vector3D> out;
    提取轮廓像素_深度边缘(帧, 簇, box, 分割参数, edges, &out, p);
    return out;
}


// ============================================================================
// 彩色边缘轮廓提取（2D 彩色边缘 -> 反投影 3D）
// 说明：
// - 当有对齐后的彩色图时，优先使用彩色边缘轮廓可获得更接近“视觉真实”的轮廓
// - 轮廓点的 3D 坐标来自点云（同像素索引）
// ============================================================================

export struct 彩色边缘轮廓参数
{
    // 颜色差阈值：0~255（越大越“保守”）
    int  颜色差阈值 = 28;
    // false=4 邻域；true=8 邻域
    bool 使用8邻域 = false;
    // >1 会下采样边缘点（省内存）
    int  采样步长 = 1;
    // 防止大簇边缘点过多
    int  最大轮廓点数 = 4096;
};

export inline int 颜色差_L1(const Color& a, const Color& b)
{
    return std::abs((int)a.r - (int)b.r) + std::abs((int)a.g - (int)b.g) + std::abs((int)a.b - (int)b.b);
}

// 从“点簇像素集合”提取彩色边缘轮廓像素（可选同时输出 3D 轮廓点）
// 策略：
// - 先判定该像素是否在簇的边界（邻居不属于簇 / 越界）
// - 再用彩色差分确认是否为“彩色边缘”
export inline void 提取轮廓像素_彩色边缘(
    const 结构体_原始场景帧& 帧,
    const 点簇& 簇,
    const 点簇边界框& box,
    const 点簇分割参数& 分割参数,
    std::vector<std::uint64_t>& out边界像素,
    std::vector<Vector3D>* out轮廓3D = nullptr,
    const 彩色边缘轮廓参数& p = {})
{
    out边界像素.clear();
    if (out轮廓3D) out轮廓3D->clear();

    if (!帧.有效颜色() || !帧.有效点云() || 簇.empty()) return;

    const int w = 帧.宽度;
    const int h = 帧.高度;
    if (w <= 0 || h <= 0) return;
    if ((int)帧.颜色.size() != w * h) return;
    if ((int)帧.点云.size() != w * h) return;

    const int bw = std::max(1, box.umax - box.umin + 1);
    const int bh = std::max(1, box.vmax - box.vmin + 1);

    // 局部 membership mask：快速判断邻居是否同簇
    std::vector<std::uint8_t> m(static_cast<std::size_t>(bw) * static_cast<std::size_t>(bh), 0);
    for (const auto& pix : 簇) {
        const int lu = pix.u - box.umin;
        const int lv = pix.v - box.vmin;
        if ((unsigned)lu >= (unsigned)bw || (unsigned)lv >= (unsigned)bh) continue;
        m[static_cast<std::size_t>(lv) * static_cast<std::size_t>(bw) + static_cast<std::size_t>(lu)] = 1;
    }

    const int offsets4[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
    const int offsets8[8][2] = { {1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1} };
    const auto* offsets = p.使用8邻域 ? offsets8 : offsets4;
    const int offset_count = p.使用8邻域 ? 8 : 4;

    const int step = std::max(1, p.采样步长);
    out边界像素.reserve(std::min<std::size_t>(簇.size() / (std::size_t)step + 16, (std::size_t)p.最大轮廓点数));
    if (out轮廓3D) out轮廓3D->reserve(out边界像素.capacity());

    auto 点云有效 = [&](const Vector3D& P)->bool {
        return std::isfinite(P.x) && std::isfinite(P.y) && std::isfinite(P.z) && P.z > 0.0;
        };

    int picked = 0;
    for (std::size_t i = 0; i < 簇.size(); i += (std::size_t)step) {
        const auto& pix = 簇[i];
        const int u = pix.u;
        const int v = pix.v;
        if ((unsigned)u >= (unsigned)w || (unsigned)v >= (unsigned)h) continue;

        const Color c0 = 帧.颜色[static_cast<std::size_t>(v) * (std::size_t)w + (std::size_t)u];

        bool is_boundary = false;
        int best_diff = 0;

        for (int k = 0; k < offset_count; ++k) {
            const int nu = u + offsets[k][0];
            const int nv = v + offsets[k][1];
            if ((unsigned)nu >= (unsigned)w || (unsigned)nv >= (unsigned)h) { is_boundary = true; continue; }

            // 同簇邻居不算边界
            const int lnu = nu - box.umin;
            const int lnv = nv - box.vmin;
            if ((unsigned)lnu < (unsigned)bw && (unsigned)lnv < (unsigned)bh) {
                if (m[static_cast<std::size_t>(lnv) * (std::size_t)bw + (std::size_t)lnu]) continue;
            }

            is_boundary = true;

            const Color cn = 帧.颜色[static_cast<std::size_t>(nv) * (std::size_t)w + (std::size_t)nu];
            best_diff = std::max(best_diff, 颜色差_L1(c0, cn));
        }

        if (!is_boundary) continue;
        if (best_diff < p.颜色差阈值) continue;

        out边界像素.push_back(通用函数模块::打包像素UV(u, v));
        if (out轮廓3D) {
            const std::size_t id = static_cast<std::size_t>(v) * (std::size_t)w + (std::size_t)u;
            const Vector3D P = 帧.点云[id];
            if (点云有效(P)) out轮廓3D->push_back(P);
        }

        if (++picked >= p.最大轮廓点数) break;
    }
}


// ============================================================================
// 占据金字塔（稀疏体素命中码）
// 编码：code = (level << 60) | morton3D(x,y,z)
// level: 0..11 （最多12层）
// 每层分辨率：N = 4 << level （即 4,8,16,...,8192）
// ============================================================================



export class 点簇分割类
{
private:
    static inline bool 尺寸存在零轴(const Vector3D& s, const 点簇分割参数& p)
    {
        const double eps = (p.尺寸为零阈值_mm > 0.0) ? p.尺寸为零阈值_mm : 0.0;

        const double ax = std::fabs(s.x);
        const double ay = std::fabs(s.y);
        const double az = std::fabs(s.z);

        if (!std::isfinite(ax) || !std::isfinite(ay) || !std::isfinite(az)) return true;

        if (eps == 0.0) {
            return (ax == 0.0) || (ay == 0.0) || (az == 0.0);
        }
        return (ax <= eps) || (ay <= eps) || (az <= eps);
    }

    static inline int 向上取整到8倍(int x) {
        return (x + 7) & ~7;
    }

    static inline int 计算轮廓标准边长(int cw, int ch, int Nmin = 32, int Nmax = 256) {
        int m = std::max(cw, ch);
        int N = std::max(Nmin, 向上取整到8倍(m));
        if (Nmax > 0) N = std::min(N, Nmax);
        return N;
    }

    static inline int 索引(int u, int v, int w) { return v * w + u; }

    static inline bool 深度有效(double z, const 点簇分割参数& p)
    {
        // 重要：这里必须严格处理 NaN/Inf/<=0。
        // 否则会把“无效深度”当作可连通点，导致大面积误连通，最终常常只剩 1~2 个大簇。
        if (p.忽略无效点) {
            if (!std::isfinite(z) || z <= 0.0) return false;
        }
        else {
            if (!std::isfinite(z)) return false; // 即便不忽略无效点，也不能放行 NaN/Inf
        }

        const double minZ = p.最小深度;
        const double maxZ = p.最大深度;
        if (minZ > 0.0 && z < minZ) return false;
        if (maxZ > 0.0 && z > maxZ) return false;
        return true;
    }

    static inline bool 点云有效(const Vector3D& P)
    {
        return std::isfinite(P.x) && std::isfinite(P.y) && std::isfinite(P.z) && P.z > 0.0;
    }

    static bool 取点云(const 结构体_原始场景帧& 帧, int u, int v, Vector3D& outP)
    {
        if (!帧.有效点云()) return false;
        const int w = 帧.宽度;
        const int h = 帧.高度;
        if ((unsigned)u >= (unsigned)w || (unsigned)v >= (unsigned)h) return false;
        const std::size_t id = static_cast<std::size_t>(v) * static_cast<std::size_t>(w) + static_cast<std::size_t>(u);
        if (id >= 帧.点云.size()) return false;
        outP = 帧.点云[id];
        return 点云有效(outP);
    }

    static bool 邻接连通(
        const 结构体_原始场景帧& 帧,
        int idxA, int idxB,
        const 点簇分割参数& p)
    {
        const int w = 帧.宽度;

        const double za = 帧.深度[static_cast<std::size_t>(idxA)];
        const double zb = 帧.深度[static_cast<std::size_t>(idxB)];
        if (!深度有效(za, p) || !深度有效(zb, p)) return false;

        // 1) 深度差门槛（纯深度就能跑）
        double maxDz = p.邻域最大深度差;
        if (std::abs(za - zb) > maxDz) return false;

        // 2) 点云欧氏距离门槛（点云可用时更稳）
        double maxD3 = p.邻域最大三维距离;
        if (maxD3 > 0.0 && 帧.有效点云()) {
            const int ua = idxA % w, va = idxA / w;
            const int ub = idxB % w, vb = idxB / w;

            Vector3D Pa{}, Pb{};
            if (取点云(帧, ua, va, Pa) && 取点云(帧, ub, vb, Pb)) {
                const double dx = Pa.x - Pb.x;
                const double dy = Pa.y - Pb.y;
                const double dz = Pa.z - Pb.z;
                const double dist = std::sqrt(dx * dx + dy * dy + dz * dz);
                if (dist > maxD3) return false;
            }
        }

        return true;
    }

    // ===== 3D 几何（OBB + PCA 主方向）=====
// 说明：
// - 旧版仅做 AABB（沿相机轴），对旋转物体尺寸会抖。
// - 这里改为：PCA 估计主方向 -> 在主方向上投影得到 OBB 尺寸（更稳定）。
// - 同时输出 PCA 残差/有效点数，供“质量分/可信度”使用。
    struct 结构体_Mat3
    {
        double a[3][3]{};
    };

    static inline double 点积(const Vector3D& p, const Vector3D& q) { return p.x * q.x + p.y * q.y + p.z * q.z; }

    static inline Vector3D 归一化(Vector3D v)
    {
        const double n2 = v.x * v.x + v.y * v.y + v.z * v.z;
        if (!std::isfinite(n2) || n2 <= 1e-20) return { 1,0,0 };
        const double inv = 1.0 / std::sqrt(n2);
        v.x *= inv; v.y *= inv; v.z *= inv;
        return v;
    }

    static inline Vector3D 取列(const 结构体_Mat3& M, int col) { return { M.a[0][col], M.a[1][col], M.a[2][col] }; }
    static inline void 设列(结构体_Mat3& M, int col, const Vector3D& v) { M.a[0][col] = v.x; M.a[1][col] = v.y; M.a[2][col] = v.z; }

    static inline 结构体_Mat3 单位阵()
    {
        结构体_Mat3 I{};
        I.a[0][0] = I.a[1][1] = I.a[2][2] = 1.0;
        return I;
    }

    // Jacobi：对称 3x3 特征分解（稳定、无依赖）
    static void Jacobi对称特征分解(const 结构体_Mat3& A, 结构体_Mat3& V, double d[3])
    {
        结构体_Mat3 D = A;
        V = 单位阵();

        auto rot = [&](int p, int q) {
            if (std::fabs(D.a[p][q]) < 1e-18) return;

            const double app = D.a[p][p];
            const double aqq = D.a[q][q];
            const double apq = D.a[p][q];

            const double phi = 0.5 * std::atan2(2.0 * apq, (aqq - app));
            const double c = std::cos(phi);
            const double s = std::sin(phi);

            // 更新 D（相似变换）
            const double app2 = c * c * app - 2.0 * s * c * apq + s * s * aqq;
            const double aqq2 = s * s * app + 2.0 * s * c * apq + c * c * aqq;
            D.a[p][p] = app2;
            D.a[q][q] = aqq2;
            D.a[p][q] = D.a[q][p] = 0.0;

            for (int k = 0; k < 3; ++k) {
                if (k == p || k == q) continue;
                const double dkp = D.a[k][p];
                const double dkq = D.a[k][q];
                const double dkp2 = c * dkp - s * dkq;
                const double dkq2 = s * dkp + c * dkq;
                D.a[k][p] = D.a[p][k] = dkp2;
                D.a[k][q] = D.a[q][k] = dkq2;
            }

            // 更新 V
            for (int k = 0; k < 3; ++k) {
                const double vkp = V.a[k][p];
                const double vkq = V.a[k][q];
                V.a[k][p] = c * vkp - s * vkq;
                V.a[k][q] = s * vkp + c * vkq;
            }
            };

        // 固定迭代次数（3x3 足够快）
        for (int it = 0; it < 24; ++it) {
            // 找最大非对角
            int p = 0, q = 1;
            double m01 = std::fabs(D.a[0][1]);
            double m02 = std::fabs(D.a[0][2]);
            double m12 = std::fabs(D.a[1][2]);
            if (m02 > m01 && m02 >= m12) { p = 0; q = 2; }
            else if (m12 > m01 && m12 > m02) { p = 1; q = 2; }

            const double off = std::max({ m01, m02, m12 });
            if (off < 1e-12) break;
            rot(p, q);
        }

        d[0] = D.a[0][0];
        d[1] = D.a[1][1];
        d[2] = D.a[2][2];
    }

    static void 计算簇3D信息(
        const 结构体_原始场景帧& 帧,
        const 点簇& 簇,
        Vector3D& 中心,
        Vector3D& 尺寸,
        Vector3D* out主方向1 = nullptr,
        Vector3D* out主方向2 = nullptr,
        Vector3D* out主方向3 = nullptr,
        double* outPCA残差 = nullptr,
        std::uint32_t* out有效点数 = nullptr)
    {
        if (!帧.有效点云() || 簇.empty()) {
            中心 = { 0,0,0 };
            尺寸 = { 0,0,0 };
            if (out主方向1) *out主方向1 = { 1,0,0 };
            if (out主方向2) *out主方向2 = { 0,1,0 };
            if (out主方向3) *out主方向3 = { 0,0,1 };
            if (outPCA残差) *outPCA残差 = 1.0;
            if (out有效点数) *out有效点数 = 0;
            return;
        }

        // Pass 1：均值
        Vector3D mean{ 0,0,0 };
        std::uint32_t cnt = 0;

        for (const auto& pix : 簇) {
            Vector3D P{};
            if (!取点云(帧, pix.u, pix.v, P)) continue;
            mean.x += P.x; mean.y += P.y; mean.z += P.z;
            ++cnt;
        }
        if (out有效点数) *out有效点数 = cnt;
        if (cnt == 0) {
            中心 = { 0,0,0 };
            尺寸 = { 0,0,0 };
            if (outPCA残差) *outPCA残差 = 1.0;
            return;
        }
        mean.x /= cnt; mean.y /= cnt; mean.z /= cnt;

        // 小点数时 PCA 不稳定：退回 AABB
        if (cnt < 16) {
            Vector3D mn{ +std::numeric_limits<double>::infinity(),
                         +std::numeric_limits<double>::infinity(),
                         +std::numeric_limits<double>::infinity() };
            Vector3D mx{ -std::numeric_limits<double>::infinity(),
                         -std::numeric_limits<double>::infinity(),
                         -std::numeric_limits<double>::infinity() };
            for (const auto& pix : 簇) {
                Vector3D P{};
                if (!取点云(帧, pix.u, pix.v, P)) continue;
                mn.x = std::min(mn.x, P.x); mn.y = std::min(mn.y, P.y); mn.z = std::min(mn.z, P.z);
                mx.x = std::max(mx.x, P.x); mx.y = std::max(mx.y, P.y); mx.z = std::max(mx.z, P.z);
            }
            中心 = mean;
            尺寸 = { std::max(0.0, mx.x - mn.x), std::max(0.0, mx.y - mn.y), std::max(0.0, mx.z - mn.z) };
            if (out主方向1) *out主方向1 = { 1,0,0 };
            if (out主方向2) *out主方向2 = { 0,1,0 };
            if (out主方向3) *out主方向3 = { 0,0,1 };
            if (outPCA残差) *outPCA残差 = 1.0;
            return;
        }

        // Pass 2：协方差
        结构体_Mat3 C{};
        for (const auto& pix : 簇) {
            Vector3D P{};
            if (!取点云(帧, pix.u, pix.v, P)) continue;
            const double x = P.x - mean.x;
            const double y = P.y - mean.y;
            const double z = P.z - mean.z;
            C.a[0][0] += x * x; C.a[0][1] += x * y; C.a[0][2] += x * z;
            C.a[1][0] += y * x; C.a[1][1] += y * y; C.a[1][2] += y * z;
            C.a[2][0] += z * x; C.a[2][1] += z * y; C.a[2][2] += z * z;
        }
        const double invN = 1.0 / static_cast<double>(cnt);
        for (int r = 0; r < 3; ++r) for (int c = 0; c < 3; ++c) C.a[r][c] *= invN;

        // 特征分解
        结构体_Mat3 V{};
        double eval[3]{};
        Jacobi对称特征分解(C, V, eval);

        // 排序：按方差从大到小（主方向稳定）
        int idxs[3]{ 0,1,2 };
        std::sort(idxs, idxs + 3, [&](int i, int j) { return eval[i] > eval[j]; });

        Vector3D a1 = 归一化(取列(V, idxs[0]));
        Vector3D a2 = 归一化(取列(V, idxs[1]));
        Vector3D a3 = 归一化(取列(V, idxs[2]));

        if (out主方向1) *out主方向1 = a1;
        if (out主方向2) *out主方向2 = a2;
        if (out主方向3) *out主方向3 = a3;

        const double sume = std::max(1e-18, eval[0] + eval[1] + eval[2]);
        const double smallest = eval[idxs[2]];
        const double pca_res = std::clamp(smallest / sume, 0.0, 1.0);
        if (outPCA残差) *outPCA残差 = pca_res;


        // Pass 3：在主方向上投影得到 OBB 尺寸，并计算 OBB 中心（鲁棒范围：p05/p95，抗边缘噪声/离群点）
        auto 取分位 = [](std::vector<double>& v, double q) -> double {
            if (v.empty()) return 0.0;
            q = std::clamp(q, 0.0, 1.0);
            const size_t n = v.size();
            const size_t k = (n <= 1) ? 0 : static_cast<size_t>(std::llround(q * (double)(n - 1)));
            std::nth_element(v.begin(), v.begin() + k, v.end());
            return v[k];
            };

        std::vector<double> p1s; p1s.reserve(cnt);
        std::vector<double> p2s; p2s.reserve(cnt);
        std::vector<double> p3s; p3s.reserve(cnt);

        // 在 PCA 主方向上投影（以 mean 为局部原点）
        for (const auto& pix : 簇) {
            Vector3D P{};
            if (!取点云(帧, pix.u, pix.v, P)) continue;
            const double dx = P.x - mean.x;
            const double dy = P.y - mean.y;
            const double dz = P.z - mean.z;
            p1s.push_back(dx * a1.x + dy * a1.y + dz * a1.z);
            p2s.push_back(dx * a2.x + dy * a2.y + dz * a2.z);
            p3s.push_back(dx * a3.x + dy * a3.y + dz * a3.z);
        }

        if (p1s.empty()) {
            中心 = mean;
            尺寸 = { 0,0,0 };
        }
        else {
            double mn1 = 0, mx1 = 0, mn2 = 0, mx2 = 0, mn3 = 0, mx3 = 0;

            // 点足够多时用分位数（更鲁棒），点少时退回 min/max
            if (p1s.size() >= 32) {
                mn1 = 取分位(p1s, 0.05); mx1 = 取分位(p1s, 0.95);
                mn2 = 取分位(p2s, 0.05); mx2 = 取分位(p2s, 0.95);
                mn3 = 取分位(p3s, 0.05); mx3 = 取分位(p3s, 0.95);
            }
            else {
                auto [a, b] = std::minmax_element(p1s.begin(), p1s.end()); mn1 = *a; mx1 = *b;
                auto [c, d] = std::minmax_element(p2s.begin(), p2s.end()); mn2 = *c; mx2 = *d;
                auto [e, f] = std::minmax_element(p3s.begin(), p3s.end()); mn3 = *e; mx3 = *f;
            }

            if (mn1 > mx1) std::swap(mn1, mx1);
            if (mn2 > mx2) std::swap(mn2, mx2);
            if (mn3 > mx3) std::swap(mn3, mx3);

            const double c1 = (mn1 + mx1) * 0.5;
            const double c2 = (mn2 + mx2) * 0.5;
            const double c3 = (mn3 + mx3) * 0.5;

            中心 = {
                mean.x + c1 * a1.x + c2 * a2.x + c3 * a3.x,
                mean.y + c1 * a1.y + c2 * a2.y + c3 * a3.y,
                mean.z + c1 * a1.z + c2 * a2.z + c3 * a3.z
            };

            尺寸 = {
                std::max(0.0, (mx1 - mn1)),
                std::max(0.0, (mx2 - mn2)),
                std::max(0.0, (mx3 - mn3))
            };
        }

    }



    // ===== 边界噪声：深度边缘强度统计（越大越“毛糙/不稳定”）=====
    static double 计算边界噪声_深度梯度(
        const 结构体_原始场景帧& 帧,
        const std::vector<std::uint64_t>& edges)
    {
        const int w = 帧.宽度;
        const int h = 帧.高度;
        if (w <= 0 || h <= 0) return 0.0;
        if ((int)帧.深度.size() != w * h) return 0.0;
        if (edges.empty()) return 0.0;

        auto idx2 = [&](int u, int v)->std::size_t { return static_cast<std::size_t>(v) * (std::size_t)w + (std::size_t)u; };

        double sum = 0.0;
        int cnt = 0;

        const int offsets[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };

        for (auto uv : edges) {
            const int u = 通用函数模块::解包U(uv);
            const int v = 通用函数模块::解包V(uv);
            if ((unsigned)u >= (unsigned)w || (unsigned)v >= (unsigned)h) continue;

            const double z = 帧.深度[idx2(u, v)];
            if (!std::isfinite(z) || z <= 0.0) continue;

            double mx = 0.0;
            for (auto& of : offsets) {
                const int nu = u + of[0], nv = v + of[1];
                if ((unsigned)nu >= (unsigned)w || (unsigned)nv >= (unsigned)h) continue;
                const double nz = 帧.深度[idx2(nu, nv)];
                if (!std::isfinite(nz) || nz <= 0.0) continue;
                mx = std::max(mx, std::fabs(nz - z));
            }
            sum += mx;
            ++cnt;
        }

        return (cnt > 0) ? (sum / cnt) : 0.0;
    }


    // ===== 深度噪声：中位绝对偏差（MAD，鲁棒）=====
    static double 计算深度噪声_MAD(
        const 结构体_原始场景帧& 帧,
        const 点簇& 簇,
        double* out中位深度 = nullptr)
    {
        if (!帧.有效深度() || 簇.empty()) {
            if (out中位深度) *out中位深度 = 0.0;
            return 0.0;
        }

        const int w = 帧.宽度;
        const int h = 帧.高度;
        if (w <= 0 || h <= 0) {
            if (out中位深度) *out中位深度 = 0.0;
            return 0.0;
        }
        if ((int)帧.深度.size() != w * h) {
            if (out中位深度) *out中位深度 = 0.0;
            return 0.0;
        }

        std::vector<double> z;
        z.reserve(簇.size());

        auto idx2 = [&](int u, int v) -> std::size_t {
            return static_cast<std::size_t>(v) * static_cast<std::size_t>(w) + static_cast<std::size_t>(u);
            };

        for (const auto& p : 簇) {
            const int u = p.u, v = p.v;
            if ((unsigned)u >= (unsigned)w || (unsigned)v >= (unsigned)h) continue;
            const double d = 帧.深度[idx2(u, v)];
            if (!std::isfinite(d) || d <= 0.0) continue;
            z.push_back(d);
        }

        if (z.empty()) {
            if (out中位深度) *out中位深度 = 0.0;
            return 0.0;
        }

        // 使用更鲁棒的中位数计算：处理偶数个点时取中间两个的平均值
        const std::size_t n = z.size();
        const std::size_t mid = n / 2;
        std::nth_element(z.begin(), z.begin() + mid, z.end());
        double med;
        if (n % 2 == 0) {
            auto second_mid = z.begin() + mid - 1;
            std::nth_element(z.begin(), second_mid, z.end());
            med = (*second_mid + z[mid]) / 2.0;
        } else {
            med = z[mid];
        }
        if (out中位深度) *out中位深度 = med;

        for (auto& v : z) v = std::fabs(v - med);

        // 计算MAD时同样处理偶数个点的情况
        std::nth_element(z.begin(), z.begin() + mid, z.end());
        double mad;
        if (n % 2 == 0) {
            auto second_mid = z.begin() + mid - 1;
            std::nth_element(z.begin(), second_mid, z.end());
            mad = (*second_mid + z[mid]) / 2.0;
        } else {
            mad = z[mid];
        }

        return mad;
    }

    // ===== 轮廓完整度：评估轮廓的封闭性和完整性 =====
    static double 计算轮廓完整度(
        const std::vector<std::uint64_t>& edges,
        const 点簇边界框& box,
        const 结构体_原始场景帧& 帧,
        const 点簇& 簇)
    {
        if (edges.empty() || box.umin == box.umax || box.vmin == box.vmax) {
            return 0.0;
        }

        const int w = 帧.宽度;
        const int h = 帧.高度;
        
        // 计算簇的面积和周长
        const int cluster_area = static_cast<int>(簇.size());
        const int contour_length = static_cast<int>(edges.size());
        
        // 计算边界框的面积和理论周长
        const int bbox_width = box.umax - box.umin + 1;
        const int bbox_height = box.vmax - box.vmin + 1;
        const int bbox_area = bbox_width * bbox_height;
        const double bbox_perimeter = 2.0 * (bbox_width + bbox_height);
        
        // 1. 面积比例：簇面积与边界框面积的比值
        double area_ratio = static_cast<double>(cluster_area) / static_cast<double>(bbox_area);
        area_ratio = std::clamp(area_ratio, 0.0, 1.0);
        
        // 2. 轮廓紧凑度：理论圆形的周长与实际周长的比值
        double compactness = 0.0;
        if (contour_length > 0) {
            double ideal_perimeter = 2.0 * std::sqrt(M_PI * cluster_area);
            compactness = ideal_perimeter / static_cast<double>(contour_length);
            compactness = std::clamp(compactness, 0.0, 1.0);
        }
        
        // 3. 边界覆盖率：实际轮廓长度与边界框周长的比值
        double boundary_coverage = static_cast<double>(contour_length) / bbox_perimeter;
        boundary_coverage = std::clamp(boundary_coverage, 0.0, 2.0); // 允许超过1.0，因为可能有复杂轮廓
        boundary_coverage = std::min(boundary_coverage, 1.0); // 但最终限制在0~1
        
        // 4. 检查轮廓连通性：简单判断轮廓点是否形成一个连续的闭环
        bool is_connected = true;
        if (contour_length > 0) {
            // 创建一个边界点的映射，便于快速查找
            std::unordered_set<std::uint64_t> edge_set(edges.begin(), edges.end());
            
            // 检查每个边界点的4邻域是否有其他边界点
            int disconnected_count = 0;
            for (auto uv : edges) {
                const int u = 通用函数模块::解包U(uv);
                const int v = 通用函数模块::解包V(uv);
                
                bool has_neighbor = false;
                const int offsets[4][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
                for (auto& of : offsets) {
                    const int nu = u + of[0];
                    const int nv = v + of[1];
                    if (nu < 0 || nu >= w || nv < 0 || nv >= h) continue;
                    
                    std::uint64_t n_uv = 通用函数模块::打包像素UV(nu, nv);
                    if (edge_set.count(n_uv) > 0) {
                        has_neighbor = true;
                        break;
                    }
                }
                
                if (!has_neighbor) {
                    disconnected_count++;
                }
            }
            
            // 如果超过10%的边界点没有邻域，则认为轮廓不连通
            if (disconnected_count > contour_length * 0.1) {
                is_connected = false;
            }
        }
        
        double connectivity_score = is_connected ? 1.0 : 0.5;
        
        // 综合计算轮廓完整度：权重可以根据实际情况调整
        double completeness = 0.0;
        completeness += 0.3 * area_ratio;
        completeness += 0.3 * compactness;
        completeness += 0.2 * boundary_coverage;
        completeness += 0.2 * connectivity_score;
        
        return std::clamp(completeness, 0.0, 1.0);
    }


    // ===== 平均颜色：严格按对齐后的 (u,v) 取样，边缘降权 =====
    static 颜色_BGR8 计算簇平均颜色_边缘降权(
        const 结构体_原始场景帧& 帧,
        const 点簇& 簇,
        const 点簇边界框& box,
        const std::vector<std::uint64_t>& edges,
        double 边缘权重,
        double* out颜色一致性 = nullptr)
    {
        颜色_BGR8 out{};
        if (!帧.有效颜色() || 簇.empty()) {
            if (out颜色一致性) *out颜色一致性 = 0.0;
            return out;
        }

        const int w = 帧.宽度;
        const int h = 帧.高度;
        if (w <= 0 || h <= 0) {
            if (out颜色一致性) *out颜色一致性 = 0.0;
            return out;
        }
        if ((int)帧.颜色.size() != w * h) {
            // 颜色未按深度对齐时，本函数无法保证严格映射（请在相机侧先 align）
            if (out颜色一致性) *out颜色一致性 = 0.0;
            return out;
        }

        // 在 bbox 内做一个边缘掩码（查边缘像素 O(1)）
        const int bw = std::max(1, box.umax - box.umin + 1);
        const int bh = std::max(1, box.vmax - box.vmin + 1);
        std::vector<std::uint8_t> edgeMask(static_cast<std::size_t>(bw) * (std::size_t)bh, 0);
        for (auto uv : edges) {
            const int u = 通用函数模块::解包U(uv) - box.umin;
            const int v = 通用函数模块::解包V(uv) - box.vmin;
            if ((unsigned)u < (unsigned)bw && (unsigned)v < (unsigned)bh)
                edgeMask[(std::size_t)v * (std::size_t)bw + (std::size_t)u] = 1;
        }

        auto gid = [&](int u, int v)->std::size_t { return (std::size_t)v * (std::size_t)w + (std::size_t)u; };

        double wr = 0.0, wg = 0.0, wb = 0.0;
        double sumw = 0.0;

        // 可选：计算颜色一致性（简易方差）
        double mr = 0.0, mg = 0.0, mb = 0.0;
        double vrr = 0.0, vgg = 0.0, vbb = 0.0;

        for (const auto& pix : 簇) {
            const int u = pix.u, v = pix.v;
            if ((unsigned)u >= (unsigned)w || (unsigned)v >= (unsigned)h) continue;

            double wgt = 1.0;
            const int lu = u - box.umin;
            const int lv = v - box.vmin;
            if ((unsigned)lu < (unsigned)bw && (unsigned)lv < (unsigned)bh) {
                if (edgeMask[(std::size_t)lv * (std::size_t)bw + (std::size_t)lu]) wgt = 边缘权重;
            }

            const Color c = 帧.颜色[gid(u, v)];
            const double r = (double)c.r;
            const double g = (double)c.g;
            const double b = (double)c.b;

            wr += wgt * r;
            wg += wgt * g;
            wb += wgt * b;
            sumw += wgt;
        }

        if (sumw <= 1e-9) {
            if (out颜色一致性) *out颜色一致性 = 0.0;
            return out;
        }

        mr = wr / sumw; mg = wg / sumw; mb = wb / sumw;

        if (out颜色一致性) {
            // 再跑一遍算方差（避免存所有颜色）
            for (const auto& pix : 簇) {
                const int u = pix.u, v = pix.v;
                if ((unsigned)u >= (unsigned)w || (unsigned)v >= (unsigned)h) continue;

                double wgt = 1.0;
                const int lu = u - box.umin;
                const int lv = v - box.vmin;
                if ((unsigned)lu < (unsigned)bw && (unsigned)lv < (unsigned)bh) {
                    if (edgeMask[(std::size_t)lv * (std::size_t)bw + (std::size_t)lu]) wgt = 边缘权重;
                }

                const Color c = 帧.颜色[gid(u, v)];
                vrr += wgt * ((double)c.r - mr) * ((double)c.r - mr);
                vgg += wgt * ((double)c.g - mg) * ((double)c.g - mg);
                vbb += wgt * ((double)c.b - mb) * ((double)c.b - mb);
            }
            const double var = (vrr + vgg + vbb) / (3.0 * sumw);
            // 一致性：方差越小越接近 1
            *out颜色一致性 = 1.0 / (1.0 + var);
        }

        auto sat = [](double x)->std::uint8_t {
            x = std::clamp(x, 0.0, 255.0);
            return (std::uint8_t)std::llround(x);
            };

        out.r = sat(mr);
        out.g = sat(mg);
        out.b = sat(mb);



        return out;
    }

    // ===== 颜色直方图：HSV 量化 8H-8S-4V = 256bin（边缘降权） =====
    // - H: [0,360) -> 8 桶 (45°/桶)
    // - S: [0,1]   -> 8 桶
    // - V: [0,1]   -> 4 桶
    // 返回计数（可选：后续写入特征值集前再做归一化/压缩）
    static inline void RGB转HSV_01(const Color& c, double& H, double& S, double& V)
    {
        const double r = (double)c.r / 255.0;
        const double g = (double)c.g / 255.0;
        const double b = (double)c.b / 255.0;
        const double mx = std::max({ r,g,b });
        const double mn = std::min({ r,g,b });
        const double d = mx - mn;

        V = mx;
        S = (mx <= 1e-12) ? 0.0 : (d / mx);

        if (d <= 1e-12) {
            H = 0.0;
        }
        else {
            double h;
            if (mx == r)      h = (g - b) / d;
            else if (mx == g) h = 2.0 + (b - r) / d;
            else              h = 4.0 + (r - g) / d;
            h *= 60.0;
            if (h < 0.0) h += 360.0;
            H = h;
        }
    }

    static inline int HSV量化bin256(const Color& c)
    {
        double H, S, V;
        RGB转HSV_01(c, H, S, V);

        int hi = (int)(H / 45.0);
        int si = (int)(S * 8.0);
        int vi = (int)(V * 4.0);
        hi = std::clamp(hi, 0, 7);
        si = std::clamp(si, 0, 7);
        vi = std::clamp(vi, 0, 3);
        return (hi << 5) | (si << 2) | vi;
    }

    static std::vector<int> 计算HSV直方图256_边缘降权(
        const 结构体_原始场景帧& 帧,
        const 点簇& 簇,
        const 点簇边界框& box,
        const std::vector<std::uint64_t>& edges,
        double 边缘权重)
    {
        std::vector<int> hist(256, 0);
        if (!帧.有效颜色() || 簇.empty()) return hist;

        const int w = 帧.宽度;
        const int h = 帧.高度;
        if (w <= 0 || h <= 0) return hist;
        if ((int)帧.颜色.size() != w * h) return hist;

        // 边缘掩码（bbox 局部坐标）
        const int bw = std::max(1, box.umax - box.umin + 1);
        const int bh = std::max(1, box.vmax - box.vmin + 1);
        std::vector<std::uint8_t> edgeMask((std::size_t)bw * (std::size_t)bh, 0);
        for (auto uv : edges) {
            const int u = 通用函数模块::解包U(uv) - box.umin;
            const int v = 通用函数模块::解包V(uv) - box.vmin;
            if ((unsigned)u < (unsigned)bw && (unsigned)v < (unsigned)bh)
                edgeMask[(std::size_t)v * (std::size_t)bw + (std::size_t)u] = 1;
        }

        auto gid = [&](int u, int v)->std::size_t { return (std::size_t)v * (std::size_t)w + (std::size_t)u; };

        std::array<double, 256> acc{};
        double sumw = 0.0;

        for (const auto& pix : 簇) {
            const int u = pix.u, v = pix.v;
            if ((unsigned)u >= (unsigned)w || (unsigned)v >= (unsigned)h) continue;

            double wgt = 1.0;
            const int lu = u - box.umin;
            const int lv = v - box.vmin;
            if ((unsigned)lu < (unsigned)bw && (unsigned)lv < (unsigned)bh) {
                if (edgeMask[(std::size_t)lv * (std::size_t)bw + (std::size_t)lu]) wgt = 边缘权重;
            }

            const Color c = 帧.颜色[gid(u, v)];
            const int bi = HSV量化bin256(c);
            acc[(std::size_t)bi] += wgt;
            sumw += wgt;
        }

        if (sumw <= 1e-9) return hist;
        for (int i = 0; i < 256; ++i) hist[(std::size_t)i] = (int)std::llround(acc[(std::size_t)i]);
        return hist;
    }

    // ===== 表面类型（粗分类）：用现有质量指标做轻量规则树 =====
    // 输出 int：0 未知, 1 光滑, 2 粗糙, 3 透明/反光, 4 金属/镜面(近似)
    static inline int 粗分类表面类型(
        double 有效点比例,
        double 颜色一致性,
        double 深度噪声_MAD,
        double 中位深度,
        double PCA残差,
        double 边界噪声,
        double 深度边缘阈值)
    {
        const double mad_scale = std::max(3.0, 3.0 + 0.01 * std::max(0.0, 中位深度));
        const double mad_norm = std::clamp(深度噪声_MAD / mad_scale, 0.0, 1.0);
        double noise_norm = 0.0;
        if (深度边缘阈值 > 1e-9) noise_norm = std::clamp(边界噪声 / (深度边缘阈值 * 3.0), 0.0, 1.0);
        const double pca_norm = std::clamp(PCA残差 * 3.0, 0.0, 1.0);

        if (有效点比例 < 0.35 && (mad_norm > 0.6 || noise_norm > 0.6)) return 3;
        if (有效点比例 < 0.6 && 颜色一致性 < 0.35 && (mad_norm > 0.35 || noise_norm > 0.35)) return 4;
        if (mad_norm < 0.25 && noise_norm < 0.25 && pca_norm < 0.35 && 颜色一致性 > 0.55) return 1;
        if (mad_norm > 0.45 || noise_norm > 0.45) return 2;
        return 0;
    }

public:
    // ===== 标准尺寸轮廓编码：8*8*n*n（n=2^N；标准边长S=8<<N）=====
  // 输出：vector<int64_t>，长度 = n*n，每个元素是一个 8x8 块（64bit）
    static std::vector<std::uint64_t> 计算轮廓编码_从边界像素(
        const std::vector<std::uint64_t>& edges,
        const 点簇边界框& box,
        int N = -1,                 // -1 表示自动推导
        int Nmax = 10,              // 最大支持到 8<<10 = 8192
        bool 进行封闭增强 = true)   // 仍然支持“膨胀+填洞”
    {
        if (edges.empty()) return {};

        const int bw = std::max(1, box.umax - box.umin + 1);
        const int bh = std::max(1, box.vmax - box.vmin + 1);
        const int side_raw = std::max(bw, bh); // 方形包含边长（像素）

        auto ceil_log2_u32 = [](std::uint32_t v)->int {
            int n = 0;
            std::uint32_t p = 1;
            while (p < v) { p <<= 1; ++n; }
            return n;
            };

        // 自动推导 N：保证标准方形边长 >= 原始方形边长（保守，不缩小）
        if (N < 0) {
            const int blocks = (side_raw + 7) / 8;           // 需要多少个 8 像素块覆盖
            N = (blocks <= 1) ? 0 : ceil_log2_u32((std::uint32_t)blocks);
        }
        N = std::clamp(N, 0, Nmax);

        const int n = 1 << N;          // 每行/列 8x8 块数量
        const int S = 8 << N;          // 标准边长（像素格）

        // 计算“中心对齐方形 bbox”： [su0, su0+side_raw-1], [sv0, sv0+side_raw-1]
        const int side = side_raw;
        const int su0 = ((box.umin + box.umax) - (side - 1)) / 2; // 允许为负
        const int sv0 = ((box.vmin + box.vmax) - (side - 1)) / 2;

        // 先落到 SxS 掩码上（便于膨胀/填洞）
        std::vector<std::uint8_t> g((std::size_t)S * (std::size_t)S, 0);

        for (auto uv : edges) {
            const int u = 通用函数模块::解包U(uv);
            const int v = 通用函数模块::解包V(uv);

            const int du = u - su0;
            const int dv = v - sv0;
            if ((unsigned)du >= (unsigned)side || (unsigned)dv >= (unsigned)side) continue;

            int gx = (int)((std::int64_t)du * S / side);
            int gy = (int)((std::int64_t)dv * S / side);
            gx = std::clamp(gx, 0, S - 1);
            gy = std::clamp(gy, 0, S - 1);
            g[(std::size_t)gy * (std::size_t)S + (std::size_t)gx] = 1;
        }

        if (进行封闭增强) {
            通用函数模块::二值掩码_膨胀一次_8邻域(g, S, S);
            通用函数模块::二值掩码_填洞_8邻域(g, S, S);
        }

        // 打包为 n*n 个 8x8 块
        std::vector<std::uint64_t> blocks((std::size_t)n * (std::size_t)n, 0ull);

        for (int y = 0; y < S; ++y) {
            const int by = y >> 3;
            const int ly = y & 7;
            for (int x = 0; x < S; ++x) {
                if (!g[(std::size_t)y * (std::size_t)S + (std::size_t)x]) continue;

                const int bx = x >> 3;
                const int lx = x & 7;

                const std::size_t bid = (std::size_t)by * (std::size_t)n + (std::size_t)bx;
                const std::uint64_t bit = 1ull << (ly * 8 + lx);
                blocks[bid] |= bit;
            }
        }

        std::vector<std::uint64_t> out;
        out.resize(blocks.size());
        for (std::size_t i = 0; i < blocks.size(); ++i) out[i] = (std::int64_t)blocks[i];
        return out;
    }

    static std::vector<std::uint64_t> 计算轮廓编码(
        const 点簇& 簇,
        const 点簇边界框& box,
        int N = -1,
        int Nmax = 10,
        bool 进行封闭增强 = true)
    {
        if (簇.empty()) return {};

        const int bw = std::max(1, box.umax - box.umin + 1);
        const int bh = std::max(1, box.vmax - box.vmin + 1);
        const int side_raw = std::max(bw, bh);

        auto ceil_log2_u32 = [](std::uint32_t v)->int {
            int n = 0;
            std::uint32_t p = 1;
            while (p < v) { p <<= 1; ++n; }
            return n;
            };

        if (N < 0) {
            const int blocks = (side_raw + 7) / 8;
            N = (blocks <= 1) ? 0 : ceil_log2_u32((std::uint32_t)blocks);
        }
        N = std::clamp(N, 0, Nmax);

        const int n = 1 << N;
        const int S = 8 << N;

        const int side = side_raw;
        const int su0 = ((box.umin + box.umax) - (side - 1)) / 2;
        const int sv0 = ((box.vmin + box.vmax) - (side - 1)) / 2;

        std::vector<std::uint8_t> g((std::size_t)S * (std::size_t)S, 0);

        for (const auto& p : 簇) {
            const int du = p.u - su0;
            const int dv = p.v - sv0;
            if ((unsigned)du >= (unsigned)side || (unsigned)dv >= (unsigned)side) continue;

            int gx = (int)((std::int64_t)du * S / side);
            int gy = (int)((std::int64_t)dv * S / side);
            gx = std::clamp(gx, 0, S - 1);
            gy = std::clamp(gy, 0, S - 1);
            g[(std::size_t)gy * (std::size_t)S + (std::size_t)gx] = 1;
        }

        if (进行封闭增强) {
            通用函数模块::二值掩码_膨胀一次_8邻域(g, S, S);
            通用函数模块::二值掩码_填洞_8邻域(g, S, S);
        }

        std::vector<std::uint64_t> blocks((std::size_t)n * (std::size_t)n, 0ull);

        for (int y = 0; y < S; ++y) {
            const int by = y >> 3;
            const int ly = y & 7;
            for (int x = 0; x < S; ++x) {
                if (!g[(std::size_t)y * (std::size_t)S + (std::size_t)x]) continue;

                const int bx = x >> 3;
                const int lx = x & 7;

                const std::size_t bid = (std::size_t)by * (std::size_t)n + (std::size_t)bx;
                const std::uint64_t bit = 1ull << (ly * 8 + lx);
                blocks[bid] |= bit;
            }
        }

        std::vector<std::uint64_t> out;
        out.resize(blocks.size());
        for (std::size_t i = 0; i < blocks.size(); ++i) out[i] = (std::int64_t)blocks[i];
        return out;
    }

    // 轮廓3D -> 占据金字塔(稀疏码) + 每个码对应的平均颜色
// outCodes[lv][k] <-> outColors[lv][k] 一一对应（同序同长度）


public:
    // ===== 基础分割：只输出点簇（像素集合）=====
    std::vector<点簇> 分割点簇(const 结构体_原始场景帧& 帧, const 点簇分割参数& 参数, 点簇性能统计* perf = nullptr) const
    {
        
        const auto _t0 = 结构体_时间戳::当前_微秒();;
        std::uint64_t _outPts = 0;

        std::vector<点簇> 输出;

        if (!帧.有效深度())
            return 输出;

        const int w = 帧.宽度;
        const int h = 帧.高度;
        const int step = std::max(1, 参数.采样步长);

        std::vector<std::uint8_t> visited(static_cast<std::size_t>(w) * h, 0);
        std::vector<int> stack;
        stack.reserve(4096);

        // 邻域 offsets
        const int offsets4[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
        const int offsets8[8][2] = { {1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1} };
        const auto* offsets = 参数.使用8邻域 ? offsets8 : offsets4;
        const int offset_count = 参数.使用8邻域 ? 8 : 4;

        for (int v = 0; v < h; v += step) {
            for (int u = 0; u < w; u += step) {

                const int id = 索引(u, v, w);
                if (visited[static_cast<std::size_t>(id)]) continue;

                const double z = 帧.深度[static_cast<std::size_t>(id)];
                if (!深度有效(z, 参数)) { visited[static_cast<std::size_t>(id)] = 1; continue; }

                // 新簇
                点簇 当前簇;
                当前簇.reserve(512);

                visited[static_cast<std::size_t>(id)] = 1;
                stack.clear();
                stack.push_back(id);

                while (!stack.empty()) {
                    const int cur = stack.back();
                    stack.pop_back();

                    const int cu = cur % w;
                    const int cv = cur / w;
                    点索引 pi; pi.u = cu; pi.v = cv; 当前簇.push_back(pi);

                    for (int k = 0; k < offset_count; ++k) {
                        const int nu = cu + offsets[k][0];
                        const int nv = cv + offsets[k][1];
                        if ((unsigned)nu >= (unsigned)w || (unsigned)nv >= (unsigned)h) continue;
                        if ((nu % step) != 0 || (nv % step) != 0) continue;

                        const int nid = 索引(nu, nv, w);
                        if (visited[static_cast<std::size_t>(nid)]) continue;

                        // 深度无效直接标记，避免重复访问
                        const double nz = 帧.深度[static_cast<std::size_t>(nid)];
                        if (!深度有效(nz, 参数)) { visited[static_cast<std::size_t>(nid)] = 1; continue; }

                        if (邻接连通(帧, cur, nid, 参数)) {
                            visited[static_cast<std::size_t>(nid)] = 1;
                            stack.push_back(nid);
                        }
                    }
                }

                if ((int)当前簇.size() >= 参数.最小点数) {
                    _outPts += (std::uint64_t)当前簇.size();
                    输出.push_back(std::move(当前簇));
                }
            }
        }

        if (perf) {
            const auto _t1 = 结构体_时间戳::当前_微秒();;
            perf->floodfill_us += _t1 - _t0;
            perf->输出簇数 += (std::uint64_t)输出.size();
            perf->输出像素点数 += _outPts;
        }

        return 输出;
    }

    // ===== 点簇 + 边界框 =====
    std::vector<点簇结果> 分割点簇_带边界框(const 结构体_原始场景帧& 帧, const 点簇分割参数& 参数, 点簇性能统计* perf = nullptr) const
    {
        auto 簇列表 = 分割点簇(帧, 参数, perf);

        std::vector<点簇结果> out;
        out.reserve(簇列表.size());


        for (auto& c : 簇列表) {
            点簇边界框 box{};
            if (!c.empty()) {
                box.umin = box.umax = c[0].u;
                box.vmin = box.vmax = c[0].v;
                for (auto& p : c) {
                    box.umin = std::min(box.umin, p.u);
                    box.umax = std::max(box.umax, p.u);
                    box.vmin = std::min(box.vmin, p.v);
                    box.vmax = std::max(box.vmax, p.v);
                }
            }
            out.push_back({ std::move(c), box });
        }
        return out;
    }

    // ===== 推荐：增强输出（含裁剪 ROI/掩码/轮廓8x8/3D几何）=====
    std::vector<点簇增强结果> 分割点簇_增强(const 结构体_原始场景帧& 帧, const 点簇分割参数& 参数, std::vector<点簇质量信息>* out质量 = nullptr, 点簇性能统计* perf = nullptr) const
    {
      
        const auto _t_total0 = 结构体_时间戳::当前_微秒();;
        if (perf) perf->调用次数 += 1;

        auto 簇列表 = 分割点簇(帧, 参数, perf);

        std::vector<点簇增强结果> out;
        out.reserve(簇列表.size());

        const int w = 帧.宽度;
        const int h = 帧.高度;
        const bool 有颜色 = 帧.有效颜色();

        for (auto& c : 簇列表) {
            if (c.size() < static_cast<std::size_t>(参数.最小点数)) continue;

            点簇增强结果 res;
            res.簇 = std::move(c);

            // 边界框
            if (!res.簇.empty()) {
                res.边界.umin = res.边界.umax = res.簇[0].u;
                res.边界.vmin = res.边界.vmax = res.簇[0].v;
                for (const auto& p : res.簇) {
                    res.边界.umin = std::min(res.边界.umin, p.u);
                    res.边界.umax = std::max(res.边界.umax, p.u);
                    res.边界.vmin = std::min(res.边界.vmin, p.v);
                    res.边界.vmax = std::max(res.边界.vmax, p.v);
                }
            }

            // ===== 轮廓标准化尺度 N（8*2^N） =====
            const int bw0 = std::max(1, res.边界.umax - res.边界.umin + 1);
            const int bh0 = std::max(1, res.边界.vmax - res.边界.vmin + 1);
            const int side_raw = std::max(bw0, bh0);

            auto ceil_log2_u32 = [](std::uint32_t v)->int {
                int n = 0;
                std::uint32_t p = 1;
                while (p < v) { p <<= 1; ++n; }
                return n;
                };
            auto floor_log2_u32 = [](std::uint32_t v)->int {
                int n = 0;
                while (v > 1) { v >>= 1; ++n; }
                return n;
                };

            // 以“就近匹配”的方式把 side_raw 归一到 8*2^N。
            // N = ceil(log2(ceil(side_raw/8)))
            int blocks = (side_raw + 7) / 8;
            int N2D = (blocks <= 1) ? 0 : ceil_log2_u32((std::uint32_t)blocks);

            // 默认允许到 1024(=8*2^7)，可通过参数扩展/收缩
            int maxN = 7;
            if (参数.轮廓最大边长 >= 8) {
                const int b = std::max(1, (int)参数.轮廓最大边长 / 8);
                maxN = std::clamp(floor_log2_u32((std::uint32_t)b), 0, 10);
            }
            if (参数.轮廓最大N > 0) {
                maxN = std::clamp((int)参数.轮廓最大N, 0, 10);
                N2D = std::clamp(N2D, 0, maxN);
            }

            res.轮廓尺度N = N2D;
            res.轮廓标准边长 = (int)(8 << N2D);


            // 3D 几何（OBB 尺寸 + PCA 主方向）
            Vector3D 主方向1{ 1,0,0 }, 主方向2{ 0,1,0 }, 主方向3{ 0,0,1 };
            double   PCA残差 = 1.0;
            std::uint32_t 有效点数 = 0;
            if (perf) {
                const auto _t0 = 结构体_时间戳::当前_微秒();
                计算簇3D信息(帧, res.簇, res.中心, res.尺寸, &主方向1, &主方向2, &主方向3, &PCA残差, &有效点数);
                const auto _t1 =结构体_时间戳::当前_微秒();
                perf->pca_us += _t1 - _t0;
            }
            else {
                计算簇3D信息(帧, res.簇, res.中心, res.尺寸, &主方向1, &主方向2, &主方向3, &PCA残差, &有效点数);
                if (参数.过滤尺寸为零的簇) {
                    if (尺寸存在零轴(res.尺寸, 参数)) {
                        continue;
                    }
                }
            }

            // 输出点云簇（3D 点列表），便于后续还原/调试
            {
                std::vector<Vector3D> pts3D;
                pts3D.reserve(res.簇.size());
                for (const auto& pix : res.簇) {
                    const size_t id = (size_t)pix.v * (size_t)w + (size_t)pix.u;
                    if (id >= 帧.点云.size()) continue;
                    const Vector3D& P = 帧.点云[id];
                    if (!点云有效(P)) continue;
                    pts3D.push_back(P);
                }

                // 注意：某些自定义容器支持 push_back(Vector3D) 但不支持直接用 std::vector<Vector3D> 赋值。
                // 因此这里统一采用 clear()+push_back() 写入，确保兼容性。
                res.点云簇.clear();
                for (const auto& P : pts3D) res.点云簇.push_back(P);
            }


            // 轮廓提取：优先彩色边缘（更像真实轮廓），失败则回退深度边缘
            const auto _t_contour0 = 结构体_时间戳::当前_微秒();
            深度边缘轮廓参数 dp{};
            if (参数.邻域最大深度差 > 0.0) {
                dp.深度边缘阈值 = std::max(dp.深度边缘阈值, 参数.邻域最大深度差);
            }

            std::vector<std::uint64_t> 边界像素;
            std::vector<Vector3D>      轮廓3D_tmp;
            std::vector<Vector3D>* p轮廓3D = nullptr;
            p轮廓3D = &轮廓3D_tmp;

            bool got_contour = false;
            Vector3D contour_mn{ +INFINITY,+INFINITY,+INFINITY };
            Vector3D contour_mx{ -INFINITY,-INFINITY,-INFINITY };
            bool have_contour_aabb = false;

            if (有颜色) {
                彩色边缘轮廓参数 cp{};
                cp.颜色差阈值 = std::clamp((int)参数.彩色轮廓差分阈值, 1, 255);
                cp.采样步长 = std::max(1, (int)参数.彩色轮廓采样步长);
                cp.最大轮廓点数 = std::max(64, (int)参数.彩色轮廓最大点数);
                提取轮廓像素_彩色边缘(帧, res.簇, res.边界, 参数, 边界像素, p轮廓3D, cp);
                got_contour = !边界像素.empty();
            }
            if (!got_contour) {
                提取轮廓像素_深度边缘(帧, res.簇, res.边界, 参数, 边界像素, p轮廓3D, dp);
            }

            if (p轮廓3D && !轮廓3D_tmp.empty()) {
                (void)通用函数模块::计算AABB尺寸_点集(轮廓3D_tmp, &contour_mn, &contour_mx);
                have_contour_aabb = true;
            }

            if (have_contour_aabb) {
                res.轮廓尺寸 = Vector3D{
                    std::max(0.0, contour_mx.x - contour_mn.x),
                    std::max(0.0, contour_mx.y - contour_mn.y),
                    std::max(0.0, contour_mx.z - contour_mn.z)
                };
            }

            // ===== 生成“对齐的”轮廓3D + 轮廓颜色（1:1 对应）=====
 // 注意：边界像素可能多于轮廓3D_tmp（因为点云无效时不会 push 3D）
 // 为了保证占据金字塔颜色一一对应，这里从边界像素重新构建对齐数据
            std::vector<Vector3D> 轮廓3D_align;
            std::vector<Color>    轮廓颜色_align;

            if (!边界像素.empty() && 帧.有效点云()) {
                轮廓3D_align.reserve(边界像素.size());
                轮廓颜色_align.reserve(边界像素.size());

                const bool 颜色可用 = 帧.有效颜色() && ((int)帧.颜色.size() == w * h);
                auto 点云有效2 = [&](const Vector3D& P)->bool {
                    return std::isfinite(P.x) && std::isfinite(P.y) && std::isfinite(P.z) && P.z > 0.0;
                    };

                for (auto uv : 边界像素) {
                    const int u = 通用函数模块::解包U(uv);
                    const int v = 通用函数模块::解包V(uv);
                    if ((unsigned)u >= (unsigned)w || (unsigned)v >= (unsigned)h) continue;

                    const std::size_t id = (std::size_t)v * (std::size_t)w + (std::size_t)u;
                    if (id >= 帧.点云.size()) continue;

                    const Vector3D P = 帧.点云[id];
                    if (!点云有效2(P)) continue;

                    轮廓3D_align.push_back(P);

                    if (颜色可用) 轮廓颜色_align.push_back(帧.颜色[id]);
                    else          轮廓颜色_align.push_back(Color{ 128,128,128 });
                }
            }

            // 用对齐后的轮廓3D更新轮廓AABB/轮廓尺寸（更一致）
            if (!轮廓3D_align.empty()) {
                (void)通用函数模块::计算AABB尺寸_点集(轮廓3D_align, &contour_mn, &contour_mx);
                have_contour_aabb = true;

                res.轮廓尺寸 = Vector3D{
                    std::max(0.0, contour_mx.x - contour_mn.x),
                    std::max(0.0, contour_mx.y - contour_mn.y),
                    std::max(0.0, contour_mx.z - contour_mn.z)
                };
            }
            if (perf) {
                const auto _t_contour1 =结构体_时间戳::当前_微秒();
                perf->contour_extract_us +=_t_contour1 - _t_contour0;
            }


            // ===== 轮廓编码（实心剪影特征） =====
            // 你的要求：轮廓内部应为 1（实心）。
            // 因此编码统一使用“簇内像素集合”生成占据，而不是仅用边界像素（边界会很稀疏且易断裂）。
            // 边界像素仍然保留用于：边界点序列、轮廓3D、颜色边缘分析。
            if (perf) {
                const auto _t0 =结构体_时间戳::当前_微秒();
                res.轮廓编码 = 计算轮廓编码(res.簇, res.边界, res.轮廓尺度N, maxN, true);
                const auto _t1 =结构体_时间戳::当前_微秒();
                perf->contour_encode_us +=_t1 - _t0;
            }
            else {
                res.轮廓编码 = 计算轮廓编码(res.簇, res.边界, res.轮廓尺度N, maxN, true);
            }

            // 边界像素序列（全图坐标打包：v<<32|u）
            res.边界点序列.clear();
            res.边界点序列.reserve(边界像素.size());
            for (auto uv : 边界像素) res.边界点序列.push_back(static_cast<std::int64_t>(uv));

            // ===== 外观统计：平均颜色 + 颜色一致性 + HSV 直方图 =====
            const auto _t_color0 = 结构体_时间戳::当前_微秒();
            if (有颜色) {
                double 颜色一致性 = 0.0;
                res.颜色 = 计算簇平均颜色_边缘降权(帧, res.簇, res.边界, 边界像素, 0.35, &颜色一致性);
                res.颜色一致性 = std::clamp(颜色一致性, 0.0, 1.0);

                // bin = (Hbin<<5) | (Sbin<<2) | Vbin
                res.颜色直方图 = 计算HSV直方图256_边缘降权(帧, res.簇, res.边界, 边界像素, 0.35);
            }
            else {
                res.颜色一致性 = 0.0;
            }
            if (perf && 有颜色) {
                const auto _t_color1 =结构体_时间戳::当前_微秒();
                perf->color_stats_us += _t_color1 - _t_color0;
            }

            // ===== 观测质量：有效点比例 + 深度噪声(MAD) + 边界噪声 + 轮廓完整度 + 综合质量分 =====
            res.有效点数 = (std::size_t)有效点数;
            res.有效点比例 = res.簇.empty() ? 0.0 : (double)有效点数 / (double)res.簇.size();

            res.深度噪声_MAD = 计算深度噪声_MAD(帧, res.簇, &res.中位深度);
            res.边界噪声 = 边界像素.empty() ? 0.0 : 计算边界噪声_深度梯度(帧, 边界像素);
            res.PCA残差 = PCA残差;
            
            // 计算轮廓完整度并存储
            res.轮廓完整度 = 计算轮廓完整度(边界像素, res.边界, 帧, res.簇);

            // 质量分：0~1（越大越好）
            {
                const double mad_scale = std::max(3.0, 3.0 + 0.01 * std::max(0.0, res.中位深度));
                const double mad_norm = std::clamp(res.深度噪声_MAD / mad_scale, 0.0, 1.0);
                const double pca_norm = std::clamp(res.PCA残差 * 3.0, 0.0, 1.0);
                const double edge_th = std::max(1e-6, std::max(参数.邻域最大深度差, 1.0));
                const double bnd_norm = std::clamp(res.边界噪声 / (edge_th * 3.0), 0.0, 1.0);

                // 权重：有效点(0.30) + 深度噪声(0.20) + PCA(0.15) + 边界噪声(0.10) + 颜色一致性(0.10) + 轮廓完整度(0.15)
                double q = 0.0;
                q += 0.30 * std::clamp(res.有效点比例, 0.0, 1.0);
                q += 0.20 * (1.0 - mad_norm);
                q += 0.15 * (1.0 - pca_norm);
                q += 0.10 * (1.0 - bnd_norm);
                q += 0.10 * std::clamp(res.颜色一致性, 0.0, 1.0);
                q += 0.15 * std::clamp(res.轮廓完整度, 0.0, 1.0);

                // 小样本惩罚：有效点过少时大幅降权
                if (res.有效点比例 < 0.2) {
                    q *= std::clamp(res.有效点比例 / 0.2, 0.0, 1.0);
                }
                res.质量分 = std::clamp(q, 0.0, 1.0);
            }

            // 选定用于占据金字塔的输入（优先对齐版，否则退回 tmp）
            const std::vector<Vector3D>* occPts = nullptr;
            const std::vector<Color>* occCols = nullptr;

            if (!轮廓3D_align.empty() && 轮廓颜色_align.size() == 轮廓3D_align.size()) {
                occPts = &轮廓3D_align;
                occCols = &轮廓颜色_align;
            }
            else if (!轮廓3D_tmp.empty()) {
                occPts = &轮廓3D_tmp;
                occCols = nullptr; // 没有对齐颜色就用默认色（或你也可以在此处补一份颜色列表）
            }

            // 保存轮廓3D（调试/精细匹配用）
            res.轮廓3D.clear();
            if (occPts) {
                res.轮廓3D.reserve(occPts->size());
                for (const auto& p : *occPts) res.轮廓3D.push_back(p);
            }

            // 裁剪输出（可选）
            const auto _t_roi0 =  结构体_时间戳::当前_微秒();
            if ((参数.输出裁剪图 || 参数.输出裁剪掩码) && w > 0 && h > 0) {
                int u0 = std::max(0, res.边界.umin - 参数.裁剪边距);
                int v0 = std::max(0, res.边界.vmin - 参数.裁剪边距);
                int u1 = std::min(w - 1, res.边界.umax + 参数.裁剪边距);
                int v1 = std::min(h - 1, res.边界.vmax + 参数.裁剪边距);

                const int cw = std::max(0, u1 - u0 + 1);
                const int ch = std::max(0, v1 - v0 + 1);

                const std::size_t area = static_cast<std::size_t>(cw) * static_cast<std::size_t>(ch);
                if (cw > 0 && ch > 0 && (int)area <= 参数.最大裁剪像素) {
                    // 记录裁剪窗口左上角，供下游把裁剪掩码映射回全图
                    res.裁剪x = u0;
                    res.裁剪y = v0;
                    res.裁剪宽 = cw;
                    res.裁剪高 = ch;

                    if (参数.输出裁剪图) {
                        res.裁剪颜色.resize(area, Color{ 255,255,255 });
                        if (有颜色) {
                            for (int yy = 0; yy < ch; ++yy) {
                                for (int xx = 0; xx < cw; ++xx) {
                                    const int gu = u0 + xx;
                                    const int gv = v0 + yy;
                                    const std::size_t gid = static_cast<std::size_t>(gv) * w + gu;
                                    res.裁剪颜色[static_cast<std::size_t>(yy) * cw + xx] = 帧.颜色[gid];
                                }
                            }
                        }
                    }

                    if (参数.输出裁剪掩码) {
                        res.裁剪掩码.assign(area, 0);
                        for (const auto& p : res.簇) {
                            const int lu = p.u - u0;
                            const int lv = p.v - v0;
                            if ((unsigned)lu >= (unsigned)cw || (unsigned)lv >= (unsigned)ch) continue;
                            res.裁剪掩码[static_cast<std::size_t>(lv) * cw + lu] = 1;
                        }

                        if (参数.掩码膨胀一次) 通用函数模块::二值掩码_膨胀一次_8邻域(res.裁剪掩码, cw, ch);
                        if (参数.掩码填洞) 通用函数模块::二值掩码_填洞_8邻域(res.裁剪掩码, cw, ch);
                    }
                }
            }
            if (perf && (参数.输出裁剪图 || 参数.输出裁剪掩码)) {
                const auto _t_roi1 =结构体_时间戳::当前_微秒();
                perf->roi_crop_us += _t_roi1 - _t_roi0;
            }
            out.push_back(std::move(res));
        }

        if (perf) {
            const auto _t_total1 = 结构体_时间戳::当前_微秒();
            perf->total_us += _t_total1 - _t_total0;
        }

        return out;
    }
};
