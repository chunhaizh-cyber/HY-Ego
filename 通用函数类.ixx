module;
#include <Windows.h>
#include <cstdint>
#include <bit>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <string_view>
#include <limits>
#include <variant>
#include <type_traits>
#include <typeinfo>
#include <random>

export module 通用函数模块;
import 基础数据类型模块;



namespace 通用函数模块 {

    ////////////////////////////////////////////////////////////////////////////////////////////////
       // //   特征值类
       ////////////////////////////////////////////////////////////////////////////////////////////////

    /// 统计 64 位整数的 1-bit 个数（popcount）。
    /// - 用途：海明距离、位图特征计数、稀疏度统计等。
    /// - 参数：x 输入位图。
    /// - 返回：x 中置位(bit=1)的数量。
    /// - 说明：MSVC 走 __popcnt64，其它编译器走 builtin。
    export inline int popcnt64(std::uint64_t x) noexcept {
#if defined(_MSC_VER)
        return (int)__popcnt64(x);
#else
        return __builtin_popcountll(x);
#endif
    }

    // =========================
       // 3) 工具：popcount（可换成 std::popcount if <bit> 可用）
       // =========================
    /// std::popcount 的薄封装（保持接口统一）。
    /// - 用途：在工具函数中统一使用 u64 版本的 popcount。
    /// - 返回：x 的置位数。
    export inline std::uint64_t popcount_u64(std::uint64_t x) {
        return (std::uint64_t)std::popcount(x);
    }


    // 可选：部分观测 mask（未知不惩罚）
    /// 带可见掩码的位图海明距离（按可见位数归一化）。
    /// - mask_visible=1 的 bit 表示该位置“可见/可信”。
    /// - 返回：popcount((a^b)&mask) / popcount(mask)。
    /// - 若 vis==0：返回 1，表示信息全未知时按最不确定处理。
    export inline std::uint64_t Bit_海明距离_掩码(const std::uint64_t& a, const std::uint64_t& b, const std::uint64_t& mask_visible) {
        const std::uint64_t x = (a ^ b) & mask_visible;
        const std::uint64_t diff = popcount_u64(x);
        const std::uint64_t vis = popcount_u64(mask_visible);
        if (vis == 0) return 1; // 没有可见信息：当作最不确定
        return diff / vis;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // //   二值轮廓图 <-> VecIU64（8x8 bit-blocks）唯一编解码
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 约定（唯一性）：
    // - 将像素二值图按 8x8 分块，每块编码为一个 u64（bit0 对应块内(0,0)，bit=y*8+x）。
    // - blocks 以“块网格行优先”存储：idx = by*side + bx。
    // - 默认输出为“正方形且边长为2的幂”的块网格，便于后续金字塔/快速匹配。

    /// 整数向上整除：ceil(a / b)。
    /// - 用途：把像素尺寸换算为 8x8 block 数、把数量换算为分组数等。
    /// - 若 b<=0：返回 0。
    export inline std::int32_t 向上整除(std::int32_t a, std::int32_t b) noexcept {
        return (b <= 0) ? 0 : (a + b - 1) / b;
    }

    /// 取得不小于 x 的最小 2 的幂。
    /// - 用途：把块网格扩展成 2^k 便于金字塔/匹配。
    export inline std::int32_t next_pow2(std::int32_t x) noexcept {
        if (x <= 1) return 1;
        std::uint32_t v = (std::uint32_t)(x - 1);
        v |= v >> 1; v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16;
        return (std::int32_t)(v + 1);
    }

    /// 将二值图（0/1 或 0/255）压缩为 VecIU64 blocks。
    /// - sideBlocks: 0 表示自动选择覆盖(w,h)的最小正方形块网格，并向上取 2 的幂；>0 表示强制使用指定边长
    /// - threshold: 阈值，大于等于该值的像素被视为前景
    export inline VecIU64 二值图_压缩为VecIU64(const std::uint8_t* bin, std::int32_t w, std::int32_t h, std::int32_t sideBlocks = 0, std::uint8_t threshold = 1)
    {
        VecIU64 out;
        if (!bin || w <= 0 || h <= 0) return out;

        const std::int32_t bx_need = 向上整除(w, 8);
        const std::int32_t by_need = 向上整除(h, 8);
        std::int32_t side = std::max(bx_need, by_need);
        if (sideBlocks > 0) {
            side = sideBlocks;
        }
        else {
            side = next_pow2(side);
        }
        if (side <= 0) return out;

        out.assign((std::size_t)side * (std::size_t)side, 0ull);

        for (std::int32_t y = 0; y < h; ++y) {
            const std::int32_t by = y >> 3;
            if (by >= side) break;
            const std::int32_t ly = y & 7;
            const std::uint64_t rowBitBase = (std::uint64_t)(ly * 8);

            const std::uint8_t* row = bin + (std::size_t)y * (std::size_t)w;
            for (std::int32_t x = 0; x < w; ++x) {
                const std::int32_t bx = x >> 3;
                if (bx >= side) break;
                if (row[x] < threshold) continue;

                const std::int32_t lx = x & 7;
                const std::uint64_t bit = rowBitBase + (std::uint64_t)lx;
                out[(std::size_t)by * (std::size_t)side + (std::size_t)bx] |= (1ull << bit);
            }
        }

        return out;
    }

    /// 将 VecIU64 blocks 还原为像素二值图（0/255）。
    /// - outW/outH: 输出还原后的尺寸：sideBlocks*8
    /// - cropW/cropH: 裁剪尺寸，若大于0且小于还原尺寸则裁剪
    export inline std::vector<std::uint8_t> VecIU64_还原为二值图(const VecIU64& blocks, std::int32_t& outW, std::int32_t& outH, std::int32_t cropW = -1, std::int32_t cropH = -1)
    {
        outW = 0; outH = 0;
        std::vector<std::uint8_t> img;
        const std::size_t n = blocks.size();
        if (n == 0) return img;

        const double s = std::sqrt((double)n);
        const std::int32_t side = (std::int32_t)std::llround(s);
        if ((std::size_t)side * (std::size_t)side != n || side <= 0) return img;

        const std::int32_t W = side * 8;
        const std::int32_t H = side * 8;

        std::int32_t cw = (cropW > 0 && cropW < W) ? cropW : W;
        std::int32_t ch = (cropH > 0 && cropH < H) ? cropH : H;

        outW = cw;
        outH = ch;
        img.assign((std::size_t)cw * (std::size_t)ch, 0);

        for (std::int32_t y = 0; y < ch; ++y) {
            const std::int32_t by = y >> 3;
            const std::int32_t ly = y & 7;
            const std::uint64_t rowBitBase = (std::uint64_t)(ly * 8);

            for (std::int32_t x = 0; x < cw; ++x) {
                const std::int32_t bx = x >> 3;
                const std::int32_t lx = x & 7;
                const std::size_t idx = (std::size_t)by * (std::size_t)side + (std::size_t)bx;
                const std::uint64_t mask = blocks[idx];
                const std::uint64_t bit = rowBitBase + (std::uint64_t)lx;
                img[(std::size_t)y * (std::size_t)cw + (std::size_t)x] = ((mask >> bit) & 1ull) ? 255 : 0;
            }
        }

        return img;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 通用：像素索引打包/解包、三维AABB统计、Morton空间编码
    //
    // 这些函数属于通用底层工具：
    // - 像素索引打包：用于轮廓边界点(u,v)的紧凑存储、队列/集合去重等
    // - AABB统计：用于点云/轮廓/网格快速求尺寸与边界
    // - Morton编码：用于占据金字塔、体素索引、块缓存的稳定Key
    //
    // 约定：
    // - 像素打包格式：uv = (uint64(v) << 32) | uint32(u)
    // - AABB：忽略非有限数(NaN/Inf)，输出尺寸会被夹到>=0
    // - Morton3D_13：每轴13bit(0..8191)，交错到低39bit，适合 (level<<60)|morton 的占据金字塔编码
    // ==================================================================================

    /// 将像素坐标(u,v)打包为一个 64 位整数：uv = (v<<32)|u。
    /// - 用途：边界点/像素集合的紧凑存储与快速去重。
    export inline std::uint64_t 打包像素UV(int u, int v) noexcept
    {
        return (static_cast<std::uint64_t>(static_cast<std::uint32_t>(v)) << 32)
            | static_cast<std::uint64_t>(static_cast<std::uint32_t>(u));
    }

    /// 从打包的 uv 中解出 u（低32位）。
    export inline int 解包U(std::uint64_t uv) noexcept
    {
        return static_cast<int>(static_cast<std::uint32_t>(uv & 0xFFFFFFFFu));
    }

    /// 从打包的 uv 中解出 v（高32位）。
    export inline int 解包V(std::uint64_t uv) noexcept
    {
        return static_cast<int>(static_cast<std::uint32_t>(uv >> 32));
    }

    /// 从点集计算AABB尺寸（可选输出min/max）。
    /// - 返回值：{dx,dy,dz}，单位与输入一致（一般为mm）
    /// - 若 pts 为空或全为无效点，min 会保持 +Inf，max 为 -Inf，返回尺寸为0。
    export inline Vector3D 计算AABB尺寸_点集(const std::vector<Vector3D>& pts, Vector3D* outMin = nullptr, Vector3D* outMax = nullptr)
    {
        Vector3D mn{ +INFINITY, +INFINITY, +INFINITY };
        Vector3D mx{ -INFINITY, -INFINITY, -INFINITY };
        for (const auto& P : pts) {
            if (!std::isfinite(P.x) || !std::isfinite(P.y) || !std::isfinite(P.z)) continue;
            mn.x = std::min(mn.x, P.x); mn.y = std::min(mn.y, P.y); mn.z = std::min(mn.z, P.z);
            mx.x = std::max(mx.x, P.x); mx.y = std::max(mx.y, P.y); mx.z = std::max(mx.z, P.z);
        }
        if (outMin) *outMin = mn;
        if (outMax) *outMax = mx;
        return Vector3D{
            std::max(0.0, mx.x - mn.x),
            std::max(0.0, mx.y - mn.y),
            std::max(0.0, mx.z - mn.z)
        };
    }

    /// Morton(Z-order) 3D 编码：每轴 13bit 交错到低 39bit。
    /// - 输入会被截断到 13bit（0..8191）。
    /// - 用途

// =============================================================================================
// 地面 / 水平线 / 垂直线（无 IMU）辅助函数（实时省内存版）
//
// 设计约束（与你最近的二次特征比较规则一致）：
// 1) 不在世界树/特征值链里保存整帧点云；仅在“本帧处理”中临时使用点云估计地面 up。
// 2) 坐标系约定：相机坐标系中，+Y 为“上方”（worldUp = (0,+1,0)）。
// 3) 地面作为水平线：用地面法线 up 定义水平面；方向比较只取水平投影后的“夹角”。
// 4) “重心方向为垂直线”：对任一存在，垂直线为 L(t)=centroid + t*up；高度为点到平面的有符号距离。
// 5) 二次特征统一“减法差值”：角度用“差异标量”落地（lhs=q_theta, rhs=0 => delta=q_theta）。
//
// 注意：
// - 无 IMU 时地面估计必然有失败概率，因此必须提供回退策略（例如沿用上一次 up，或仅比较距离）。
// - up 的符号必须消歧，否则会出现“法线翻面”导致夹角/高度符号跳变。
// =============================================================================================

/// RANSAC 平面拟合参数（无 IMU 地面估计）
    export struct 参数_地面平面RANSAC {
        // RANSAC 迭代次数：越大越稳但越慢。实时建议 60~120。
        std::uint32_t 迭代次数 = 80;

        // 内点距离阈值（mm）。深度噪声大可适当增大；过大则会把墙/桌面也吸进来。
        double 内点阈值_mm = 15.0;

        // 最小内点数：低于该值认为失败（地面不可用）。实时建议 300~1500，取决于下采样规模。
        std::uint32_t 最小内点数 = 800;

        // 预过滤：只考虑这些点（相机坐标系）
        // - z 范围：剔除过近/过远
        double 最小Z_mm = 300.0;
        double 最大Z_mm = 6000.0;

        // - y 上限：通常地面在相机下方（y 更小）。若你的坐标系地面在 y<0，可保持默认 0。
        double 最大Y_mm = 0.0;

        // 下采样：最多使用多少点参与 RANSAC（避免 O(N*iters) 过大）
        std::size_t 最大参与点数 = 4000;
    };

    /// 地面平面拟合结果
    export struct 结果_地面平面 {
        bool 成功 = false;

        // 地面法线（单位向量），已消歧为朝 +Y 半球
        Vector3D up{ 0.0, 1.0, 0.0 };

        // 平面方程：up · x + d = 0
        double d = 0.0;

        // 统计：内点数/平均残差（mm）
        std::uint32_t 内点数 = 0;
        double 平均残差_mm = 0.0;
    };

    /// 向量点积
    export inline double 点积(const Vector3D& a, const Vector3D& b) noexcept { return a.x * b.x + a.y * b.y + a.z * b.z; }

    /// 向量叉积
    export inline Vector3D 叉积(const Vector3D& a, const Vector3D& b) noexcept {
        return Vector3D{ a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
    }

    /// 向量长度平方
    export inline double 长度平方(const Vector3D& v) noexcept { return 点积(v, v); }

    /// 向量长度
    export inline double 长度(const Vector3D& v) noexcept { return std::sqrt(长度平方(v)); }

    /// 向量归一化（失败返回 {0,0,0}，并可通过 outOk 得知）
    export inline Vector3D 归一化(const Vector3D& v, bool* outOk = nullptr, double eps = 1e-9) noexcept {
        const double l = 长度(v);
        if (l <= eps || !std::isfinite(l)) {
            if (outOk) *outOk = false;
            return Vector3D{ 0.0, 0.0, 0.0 };
        }
        if (outOk) *outOk = true;
        const double inv = 1.0 / l;
        return Vector3D{ v.x * inv, v.y * inv, v.z * inv };
    }

    /// clamp 到 [-1,1]（用于 acos 的数值稳定）
    export inline double clamp_m11(double x) noexcept {
        if (x < -1.0) return -1.0;
        if (x > 1.0) return  1.0;
        return x;
    }

    /// 安全减：lhs-rhs（溢出饱和到 int64 min/max）
    export inline std::int64_t 安全减(std::int64_t lhs, std::int64_t rhs) noexcept {
        // 使用更兼容的方式实现，避免 __int128
        if (rhs < 0) {
            // lhs - (negative) = lhs + abs(rhs)
            if (lhs > std::numeric_limits<std::int64_t>::max() + rhs) {
                return std::numeric_limits<std::int64_t>::max();
            }
        }
        else {
            // lhs - positive
            if (lhs < std::numeric_limits<std::int64_t>::min() + rhs) {
                return std::numeric_limits<std::int64_t>::min();
            }
        }
        return lhs - rhs;
    }

    /// 无 IMU 地面平面估计（RANSAC）
    /// - 输入：相机坐标系点集（mm）
    /// - 输出：
    ///   result.up：单位法线，保证 dot(up, (0,+1,0)) >= 0
    ///   result.d ：平面方程 up·x + d = 0
    /// - 备注：
    ///   1) 该函数仅用于“当前帧”估计地面，不建议把整帧点云落库。
    ///   2) 若失败，建议上层沿用上一次成功的 up（带过期计数），或退化到仅比较距离/轮廓等特征。
    export inline 结果_地面平面 估计地面平面_无IMU_RANSAC(const std::vector<Vector3D>& pts, const 参数_地面平面RANSAC& p = {})
    {
        结果_地面平面 out{};

        // 预过滤 + 下采样（实时省时省内存：只挑部分点参与）
        std::vector<Vector3D> cand;
        cand.reserve(std::min<std::size_t>(pts.size(), p.最大参与点数));

        // 简单 stride 下采样：在大规模点云下能显著减少 RANSAC 计算
        const std::size_t stride = (pts.size() > p.最大参与点数 && p.最大参与点数 > 0)
            ? std::max<std::size_t>(1, pts.size() / p.最大参与点数)
            : 1;

        for (std::size_t i = 0; i < pts.size(); i += stride) {
            const auto& P = pts[i];
            if (!std::isfinite(P.x) || !std::isfinite(P.y) || !std::isfinite(P.z)) continue;
            if (P.z < p.最小Z_mm || P.z > p.最大Z_mm) continue;
            if (P.y > p.最大Y_mm) continue; // 通常地面在相机下方（y 更小）
            cand.push_back(P);
            if (cand.size() >= p.最大参与点数) break;
        }

        if (cand.size() < 3) return out;

        // RANSAC 随机数（固定 seed：保证调试复现；若你希望更随机，可用时间戳做 seed）
        std::mt19937 rng(1234567u);
        std::uniform_int_distribution<std::size_t> uid(0, cand.size() - 1);

        const Vector3D worldUp{ 0.0, 1.0, 0.0 };

        std::uint32_t bestInliers = 0;
        double bestMeanAbs = std::numeric_limits<double>::infinity();
        Vector3D bestN{ 0.0, 1.0, 0.0 };
        double bestD = 0.0;

        // 临时内点标记（避免每轮都 new 大数组）
        std::vector<std::uint8_t> inlierMask(cand.size(), 0);

        for (std::uint32_t it = 0; it < p.迭代次数; ++it) {
            // 取 3 个不重复点
            std::size_t i0 = uid(rng), i1 = uid(rng), i2 = uid(rng);
            if (i0 == i1 || i0 == i2 || i1 == i2) { --it; continue; }

            const Vector3D& A = cand[i0];
            const Vector3D& B = cand[i1];
            const Vector3D& C = cand[i2];

            const Vector3D AB{ B.x - A.x, B.y - A.y, B.z - A.z };
            const Vector3D AC{ C.x - A.x, C.y - A.y, C.z - A.z };

            bool okN = false;
            Vector3D n = 归一化(叉积(AB, AC), &okN);
            if (!okN) { continue; }

            // 法线消歧：保证朝 +Y 半球
            if (点积(n, worldUp) < 0.0) {
                n.x = -n.x; n.y = -n.y; n.z = -n.z;
            }

            // 平面：n·x + d = 0，过点 A
            const double d = -点积(n, A);

            // 统计内点
            std::uint32_t inliers = 0;
            double sumAbs = 0.0;

            const double thr = p.内点阈值_mm;
            for (std::size_t k = 0; k < cand.size(); ++k) {
                const auto& P = cand[k];
                const double dist = std::abs(点积(n, P) + d); // n 已单位化 => 距离单位 mm
                const bool inl = (dist <= thr);
                inlierMask[k] = (std::uint8_t)(inl ? 1 : 0);
                if (inl) { ++inliers; sumAbs += dist; }
            }

            if (inliers < p.最小内点数) continue;

            const double meanAbs = sumAbs / (double)inliers;

            // 选择策略：优先内点多，其次平均残差小
            if (inliers > bestInliers || (inliers == bestInliers && meanAbs < bestMeanAbs)) {
                bestInliers = inliers;
                bestMeanAbs = meanAbs;
                bestN = n;
                bestD = d;
            }
        }

        if (bestInliers < p.最小内点数) {
            return out; // 失败
        }

        out.成功 = true;
        out.up = bestN;
        out.d = bestD;
        out.内点数 = bestInliers;
        out.平均残差_mm = std::isfinite(bestMeanAbs) ? bestMeanAbs : 0.0;
        return out;
    }

    /// 点到平面的有符号距离（mm）：up·x + d
    /// - up 必须为单位向量，且已按 +Y 消歧
    /// - 返回：朝 up 方向为正
    export inline double 点到平面有符号距离_mm(const Vector3D& x, const Vector3D& up, double d) noexcept {
        return 点积(up, x) + d;
    }

    /// 投影到地面水平面：v_h = v - (v·up) up
    /// - 用途：地面为水平线时，方向比较只在水平面内进行（抑制俯仰导致的方向抖动）。
    export inline Vector3D 投影到水平面(const Vector3D& v, const Vector3D& up) noexcept {
        const double t = 点积(v, up);
        return Vector3D{ v.x - t * up.x, v.y - t * up.y, v.z - t * up.z };
    }

    /// 计算水平夹角（rad）
    /// - 输入：c1/c2 为“相机原点 -> 目标重心”的向量（即重心坐标本身）
    /// - 输出：theta_h ∈ [0,π]
    /// - 返回：成功/失败（失败通常因为水平分量过小，目标接近正上/正下）
    export inline bool 计算水平夹角_rad(const Vector3D& c1, const Vector3D& c2, const Vector3D& up, double& outTheta, double eps_h = 1e-6) noexcept
    {
        const Vector3D v1h = 投影到水平面(c1, up);
        const Vector3D v2h = 投影到水平面(c2, up);

        const double l1 = 长度(v1h);
        const double l2 = 长度(v2h);
        if (!(l1 > eps_h) || !(l2 > eps_h)) return false;

        const double c = clamp_m11(点积(v1h, v2h) / (l1 * l2));
        outTheta = std::acos(c);
        return std::isfinite(outTheta);
    }

    /// 量化角度到“微弧度”整数（int64）
    /// - 统一比较规则：所有二次特征最终都落到 int64 delta 上做区间命中
    /// - 微弧度是一个折中：足够精细且不会太容易溢出
    export inline std::int64_t 量化角度_微弧度(double theta_rad) noexcept
    {
        // 防止 NaN/Inf
        if (!std::isfinite(theta_rad)) return 0;
        constexpr double S = 1e6; // micro-radian
        const double q = std::round(theta_rad * S);
        if (q > (double)(std::numeric_limits<std::int64_t>::max)()) return (std::numeric_limits<std::int64_t>::max)();
        if (q < (double)(std::numeric_limits<std::int64_t>::min)()) return (std::numeric_limits<std::int64_t>::min)();
        return (std::int64_t)q;
    }

    /// 计算“水平夹角差异”的量化标量（微弧度）
    /// - 按你的“统一减法”落地方式：把差异当 lhs，rhs=0 => delta=lhs
    /// - 返回：成功/失败；失败时 out_q_theta 不变
    export inline bool 计算水平夹角差异_量化微弧度(const Vector3D& c1, const Vector3D& c2, const Vector3D& up, std::int64_t& out_q_theta, double eps_h = 1e-6) noexcept
    {
        double theta = 0.0;
        if (!计算水平夹角_rad(c1, c2, up, theta, eps_h)) return false;
        out_q_theta = 量化角度_微弧度(theta);
        return true;
    }

    export inline std::uint64_t Morton3D_13(std::uint32_t x, std::uint32_t y, std::uint32_t z) noexcept
    {
        x &= 0x1FFFu; y &= 0x1FFFu; z &= 0x1FFFu;
        std::uint64_t m = 0;
        for (int b = 0; b < 13; ++b) {
            m |= (std::uint64_t)((x >> b) & 1u) << (3 * b + 0);
            m |= (std::uint64_t)((y >> b) & 1u) << (3 * b + 1);
            m |= (std::uint64_t)((z >> b) & 1u) << (3 * b + 2);
        }
        return m;
    }

    /// 占据金字塔编码：高4bit存 level，低39bit存 Morton3D_13。
    /// - 约定：code = (level<<60) | morton。
    export inline std::uint64_t 编码_占据金字塔(std::uint32_t level, std::uint32_t x, std::uint32_t y, std::uint32_t z) noexcept
    {
        return (static_cast<std::uint64_t>(level & 0x0Fu) << 60) | Morton3D_13(x, y, z);
    }

    /// 从占据金字塔编码中提取 level（高4bit）。
    export inline std::uint32_t 解码_占据金字塔_level(std::uint64_t code) noexcept
    {
        return static_cast<std::uint32_t>((code >> 60) & 0x0Fu);
    }

    // =========================
    // 二值掩码工具（像素级，轻量实现，无OpenCV依赖）
    //
    // 约定：mask 为 w*h 的数组，非0表示前景(1)，0表示背景(0)。
    // - 膨胀一次：8邻域，把邻居的前景扩张到本像素
    // - 填洞：把被前景包围的背景区域填为前景（仅填内部孔洞，不影响外部背景）
    // =========================

    /// 对二值掩码进行一次8邻域膨胀操作。
    /// - mask：输入输出掩码，w*h 的数组，非0表示前景，0表示背景
    /// - w, h：掩码的宽度和高度
    export inline void 二值掩码_膨胀一次_8邻域(std::vector<std::uint8_t>& mask, int w, int h)
    {
        if (w <= 0 || h <= 0) return;
        const std::size_t n = (std::size_t)w * (std::size_t)h;
        if (mask.size() != n) return;
        std::vector<std::uint8_t> out(mask);
        auto idx = [&](int u, int v)->std::size_t { return (std::size_t)v * (std::size_t)w + (std::size_t)u; };
        for (int v = 0; v < h; ++v) {
            for (int u = 0; u < w; ++u) {
                if (mask[idx(u, v)]) { out[idx(u, v)] = 255; continue; }
                bool hit = false;
                for (int dv = -1; dv <= 1 && !hit; ++dv) {
                    const int vv = v + dv;
                    if ((unsigned)vv >= (unsigned)h) continue;
                    for (int du = -1; du <= 1; ++du) {
                        const int uu = u + du;
                        if ((unsigned)uu >= (unsigned)w) continue;
                        if (mask[idx(uu, vv)]) { hit = true; break; }
                    }
                }
                if (hit) out[idx(u, v)] = 255;
            }
        }
        mask.swap(out);
    }

    /// 对二值掩码进行8邻域填洞操作。
    /// - mask：输入输出掩码，w*h 的数组，非0表示前景，0表示背景
    /// - w, h：掩码的宽度和高度
    export inline void 二值掩码_填洞_8邻域(std::vector<std::uint8_t>& mask, int w, int h)
    {
        if (w <= 0 || h <= 0) return;
        const std::size_t n = (std::size_t)w * (std::size_t)h;
        if (mask.size() != n) return;
        std::vector<std::uint8_t> vis(n, 0);
        std::vector<std::uint32_t> q;
        q.reserve((std::size_t)w + (std::size_t)h);
        auto idx = [&](int u, int v)->std::size_t { return (std::size_t)v * (std::size_t)w + (std::size_t)u; };
        auto push_if_bg = [&](int u, int v) {
            const std::size_t id = idx(u, v);
            if (vis[id]) return;
            if (mask[id]) return;
            vis[id] = 1;
            q.push_back((std::uint32_t)id);
            };
        // 1) 从边界上的背景像素做洪泛，标记为“外部背景”
        for (int u = 0; u < w; ++u) { push_if_bg(u, 0); if (h > 1) push_if_bg(u, h - 1); }
        for (int v = 0; v < h; ++v) { push_if_bg(0, v); if (w > 1) push_if_bg(w - 1, v); }
        const int du8[8] = { 1,-1,0,0, 1,1,-1,-1 };
        const int dv8[8] = { 0,0,1,-1, 1,-1,1,-1 };
        std::size_t head = 0;
        while (head < q.size()) {
            const std::size_t id = (std::size_t)q[head++];
            const int u = (int)(id % (std::size_t)w);
            const int v = (int)(id / (std::size_t)w);
            for (int k = 0; k < 8; ++k) {
                const int uu = u + du8[k];
                const int vv = v + dv8[k];
                if ((unsigned)uu >= (unsigned)w || (unsigned)vv >= (unsigned)h) continue;
                push_if_bg(uu, vv);
            }
        }
        // 2) 未被标记的背景像素就是内部孔洞，填为前景
        for (std::size_t i = 0; i < n; ++i) {
            if (!mask[i] && !vis[i]) mask[i] = 255;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // //   辅助函数
    ////////////////////////////////////////////////////////////////////////////////////////////////

    /// 打包 RGBA 颜色
    /// - 参数：r 红色通道，g 绿色通道，b 蓝色通道，a alpha通道
    /// - 返回：打包后的 RGBA 颜色（AARRGGBB）
    export inline std::uint32_t PackRGBA(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255) {
        return (std::uint32_t)a << 24 | (std::uint32_t)r << 16 | (std::uint32_t)g << 8 | (std::uint32_t)b;
    }

    /// FNV1a64 哈希函数
    /// - 参数：data 数据指针，bytes 数据大小
    /// - 返回：哈希值
    export inline std::uint64_t FNV1a64(const void* data, std::size_t bytes) {
        const std::uint8_t* p = (const std::uint8_t*)data;
        std::uint64_t h = 1469598103934665603ull;
        for (std::size_t i = 0; i < bytes; ++i) {
            h ^= (std::uint64_t)p[i];
            h *= 1099511628211ull;
        }
        return h;
    }

    /// FNV1a64 哈希（vector 版本）
    /// - 注意：对 vector 取的是“元素数据”，而不是 vector 对象本身。
    export inline std::uint64_t FNV1a64_vec_u64(const std::vector<std::uint64_t>& v) noexcept {
        if (v.empty()) return 0ull;
        return FNV1a64(v.data(), v.size() * sizeof(std::uint64_t));
    }
    export inline std::uint64_t FNV1a64_vec_i64(const std::vector<std::int64_t>& v) noexcept {
        if (v.empty()) return 0ull;
        return FNV1a64(v.data(), v.size() * sizeof(std::int64_t));
    }

    //==============================================================================================
    // 比较/哈希小工具（通用）
    // 说明：
    // - 这些工具原先分散在“主信息定义模块”里；按你的规则迁移到“通用函数模块”，便于复用。
    // - 若未来需要更复杂的比较（相似/包含），请在特征值/语义模块里实现；这里保持最小可用。
    //==============================================================================================

  


    /// int->Q10000 夹紧到 [0,10000]。
    export inline std::uint32_t Clamp_Q10000(std::int64_t v) noexcept
    {
        if (v <= 0) return 0;
        if (v >= 10000) return 10000;
        return (std::uint32_t)v;
    }

    /// u64 的 floor(log2(x))；x==0 -> 0。
    export inline std::uint32_t ilog2_u64(std::uint64_t x) noexcept
    {
        if (x == 0) return 0;
#if defined(_MSC_VER)
        unsigned long idx = 0;
        _BitScanReverse64(&idx, x);
        return (std::uint32_t)idx;
#else
        return (std::uint32_t)(63u - (std::uint32_t)__builtin_clzll(x));
#endif
    }



    //==============================================================================================
    // 指数趋近（用于大范围无符号标量的“平滑调节”）
    // 约定：
    // - intensity >= 0：0 表示不变化
    // - k > 0：曲率/速度参数
    // - 下限：0
    // - 上限：maxInclusive，但“保留1间隙” => 最高只能到 (maxInclusive - 1)
    //==============================================================================================

    /// U64 指数趋近下限（0）。
    /// - 当前值越大，下降越快；intensity 越大，下降越快；k 越大，下降越快。
    /// - 返回值保证：0 <= out <= 当前值。
    export inline std::uint64_t U64_指数趋近下限(std::uint64_t 当前值, double intensity, double k) noexcept
    {
        if (当前值 == 0) return 0;
        if (!(intensity > 0.0) || !(k > 0.0)) return 当前值;
        if (!std::isfinite(intensity) || !std::isfinite(k)) return 当前值;

        const long double x = (long double)k * (long double)intensity;

        // exp(-x)；x 很大时直接近似为 0
        const long double decay = (x >= 80.0L) ? 0.0L : std::exp(-x);

        long double newv = (long double)当前值 * decay;
        if (newv <= 0.0L) return 0;

        // 四舍五入，同时保证单调不增
        std::uint64_t out = (std::uint64_t)(newv + 0.5L);
        if (out > 当前值) out = 当前值;

        // 避免“intensity>0 但取整后完全不动”
        if (out == 当前值) out = 当前值 - 1;

        return out;
    }

    /// U64 指数趋近上限（maxInclusive），并保留 1 的间隙（永远不达到 maxInclusive）。
    /// - 最高返回：maxInclusive-1（若 maxInclusive==0 则返回 0）。
    /// - 返回值保证：当前值 <= out <= maxInclusive-1（在可提升范围内）。
    export inline std::uint64_t U64_指数趋近上限_保留1间隙(
        std::uint64_t 当前值,
        std::uint64_t maxInclusive,
        double intensity,
        double k) noexcept
    {
        if (maxInclusive == 0) return 0;
        const std::uint64_t top = maxInclusive - 1;

        // 即使不变化，也要保证遵守“保留1间隙”的约定
        if (!(intensity > 0.0) || !(k > 0.0) || !std::isfinite(intensity) || !std::isfinite(k)) {
            return (当前值 > top) ? top : 当前值;
        }

        if (当前值 >= top) return top;

        const long double x = (long double)k * (long double)intensity;
        const long double decay = (x >= 80.0L) ? 0.0L : std::exp(-x);

        // new = top - (top-current) * exp(-x)
        const long double diff = (long double)(top - 当前值);
        long double newv = (long double)top - diff * decay;

        // 四舍五入，同时保证单调不减 + 不越界
        std::uint64_t out = (std::uint64_t)(newv + 0.5L);
        if (out > top) out = top;
        if (out < 当前值) out = 当前值; // 防御浮点误差

        // 避免“intensity>0 但取整后完全不动”
        if (out == 当前值) out = 当前值 + 1;
        if (out > top) out = top;

        return out;
    }

    /// 解包 RGBA_AARRGGBB
    /// - 参数：rgba 打包后的颜色，r 红色通道，g 绿色通道，b 蓝色通道，a alpha通道
    export inline void 解包RGBA_AARRGGBB(std::uint32_t rgba, std::uint8_t& r, std::uint8_t& g, std::uint8_t& b, std::uint8_t& a) {
        a = (std::uint8_t)((rgba >> 24) & 0xFFu);
        r = (std::uint8_t)((rgba >> 16) & 0xFFu);
        g = (std::uint8_t)((rgba >> 8) & 0xFFu);
        b = (std::uint8_t)((rgba) & 0xFFu);
    }
    /// 将浮点值夹紧到 [0,1]。
    /// - 用途：归一化距离/相似度，避免越界。
    /// - 参数：v 任意浮点。
    /// - 返回：若 v<0 返回0，v>1 返回1，否则返回v。
    export inline float clamp01(float v) noexcept { return v < 0.f ? 0.f : (v > 1.f ? 1.f : v); }
    /// long double 的绝对值。
    /// - 用途：给 long double 计算做统一的 abs，避免隐式转换。
    export inline long double abs_ld(long double x) noexcept { return x < 0 ? -x : x; }

    /// 安全地取 |x| 并转为 double。
    /// - 用途：归一化分母、距离度量时避免 INT64_MIN 的 abs 溢出。
    /// - 返回：|x|（若 x==INT64_MIN，则返回 INT64_MAX）。
    export inline double norm_abs_i64(std::int64_t x) noexcept {
        // 避免 abs(INT64_MIN) 溢出
        if (x == (std::numeric_limits<std::int64_t>::min)()) return (double)(std::numeric_limits<std::int64_t>::max)();
        return (double)std::llabs(x);
    }
    /// 无符号标量的相对差异度：|x-y| / max(1, max(x,y))，并夹紧到[0,1]。
    /// - 用途：把不同量纲的计数/长度等标量差异变成可比较的归一化距离。
    export inline double scalar_rel_diff_u64(std::uint64_t x, std::uint64_t y) noexcept {
        const std::uint64_t d = (x > y) ? (x - y) : (y - x);
        const std::uint64_t den = std::max<std::uint64_t>(1, std::max(x, y));
        return clamp01((float)d / (float)den);
    }
    /// 有符号标量的相对差异度：|x-y| / max(1, max(|x|,|y|))，并夹紧到[0,1]。
    /// - 用途：对可能为负的标量（偏移量、坐标等）做稳健归一化。
    export inline double scalar_rel_diff_i64(std::int64_t x, std::int64_t y) noexcept {
        const long double lx = (long double)x;
        const long double ly = (long double)y;
        const long double d = abs_ld(lx - ly);
        const long double den = std::max<long double>(1.0L, std::max(abs_ld(lx), abs_ld(ly)));
        return clamp01((float)(d / den));
    }

    /// 64 位海明距离（归一化到[0,1]）：popcount(x^y)/64。
    /// - 用途：位图特征、掩码特征的距离度量。
    export inline double hamming_u64(std::uint64_t x, std::uint64_t y) noexcept {
        return (double)std::popcount(x ^ y) / 64.0;
    }

    /// VecIU64 的海明距离（归一化到[0,1]）。
    /// - 规则：对齐部分 popcount(xor)，多出来的尾部按自身 popcount 计入差异。
    /// - 返回：diff_bits / (max(nx,ny)*64)。
    export inline double hamming_vec_u64(const VecIU64& x, const VecIU64& y) noexcept {
        const size_t nx = x.size(), ny = y.size();
        const size_t n = std::min(nx, ny);
        std::uint64_t diff_bits = 0;
        for (size_t i = 0; i < n; ++i) diff_bits += (std::uint64_t)std::popcount(x[i] ^ y[i]);
        for (size_t i = n; i < nx; ++i) diff_bits += (std::uint64_t)std::popcount(x[i]);
        for (size_t i = n; i < ny; ++i) diff_bits += (std::uint64_t)std::popcount(y[i]);
        const std::uint64_t total_bits = (std::uint64_t)std::max(nx, ny) * 64ull;
        if (total_bits == 0) return 0.0;
        return (double)diff_bits / (double)total_bits;
    }
    /// VecI64 的 L1 归一化距离（0..1）。
    /// - 分子：sum |a_i-b_i|
    /// - 分母：sum max(1, max(|a_i|,|b_i|))
    /// - 说明：支持不等长，缺失部分按与 0 比较。
    export inline double l1_vec_i64_norm(const VecI64& a, const VecI64& b) noexcept {
        const size_t n = std::min(a.size(), b.size());
        long double num = 0.0L, den = 0.0L;

        auto add = [&](std::int64_t x, std::int64_t y) {
            const long double lx = (long double)x;
            const long double ly = (long double)y;
            num += abs_ld(lx - ly);
            den += std::max<long double>(1.0L, std::max(abs_ld(lx), abs_ld(ly)));
            };

        for (size_t i = 0; i < n; ++i) add(a[i], b[i]);
        for (size_t i = n; i < a.size(); ++i) add(a[i], 0);
        for (size_t i = n; i < b.size(); ++i) add(0, b[i]);

        if (den <= 0.0L) return 0.0;
        return clamp01((float)(num / den));
    }
    /// VecI64 的 L2 归一化距离（0..1）。
    /// - 分子：sum (a_i-b_i)^2
    /// - 分母：sum max(1, max(|a_i|,|b_i|))^2
    /// - 返回：sqrt(num/den) 并夹紧。
    export inline double l2_vec_i64_norm(const VecI64& a, const VecI64& b) noexcept {
        const size_t n = std::min(a.size(), b.size());
        long double num = 0.0L, den = 0.0L;

        auto add = [&](std::int64_t x, std::int64_t y) {
            const long double lx = (long double)x;
            const long double ly = (long double)y;
            const long double d = (lx - ly);
            num += d * d;
            const long double m = std::max<long double>(1.0L, std::max(abs_ld(lx), abs_ld(ly)));
            den += m * m;
            };

        for (size_t i = 0; i < n; ++i) add(a[i], b[i]);
        for (size_t i = n; i < a.size(); ++i) add(a[i], 0);
        for (size_t i = n; i < b.size(); ++i) add(0, b[i]);

        if (den <= 0.0L) return 0.0;
        return clamp01((float)std::sqrt((double)(num / den)));
    }

    /// VecI64 的余弦距离（0..1），并映射到 (1-cos)/2。
    /// - 用途：方向类/直方图类特征的相似度对比。
    /// - 注意：若任一向量模长为0，返回 1.0（最不相似/不确定）。
    export inline double cosine_distance_vec_i64_norm(const VecI64& a, const VecI64& b) noexcept {
        const size_t n = std::min(a.size(), b.size());
        long double dot = 0.0L, na2 = 0.0L, nb2 = 0.0L;

        for (size_t i = 0; i < n; ++i) {
            const long double x = (long double)a[i];
            const long double y = (long double)b[i];
            dot += x * y;
            na2 += x * x;
            nb2 += y * y;
        }
        for (size_t i = n; i < a.size(); ++i) { const long double x = (long double)a[i]; na2 += x * x; }
        for (size_t i = n; i < b.size(); ++i) { const long double y = (long double)b[i]; nb2 += y * y; }

        if (na2 <= 0.0L || nb2 <= 0.0L) return 1.0;

        long double cosv = dot / (std::sqrt(na2) * std::sqrt(nb2));
        if (cosv > 1.0L) cosv = 1.0L;
        if (cosv < -1.0L) cosv = -1.0L;
        return (double)((1.0L - cosv) * 0.5L);
    }

    /// BGR 颜色距离（0..1）：(|db|+|dg|+|dr|)/765。
    /// - 输入：a,b 至少包含3个分量，分别为 B,G,R（0..255）。
    /// - 返回：归一化后的差异度，越大表示越不相似。
    export inline double color_distance_bgr_norm(const VecI64& a, const VecI64& b) noexcept {
        if (a.size() < 3 || b.size() < 3) return 1.0;
        const long double db = abs_ld((long double)a[0] - (long double)b[0]);
        const long double dg = abs_ld((long double)a[1] - (long double)b[1]);
        const long double dr = abs_ld((long double)a[2] - (long double)b[2]);
        return clamp01((float)((db + dg + dr) / 765.0L));
    }


    /// 字符串比较（用于查找/过滤）。
    /// - 采用 std::string 的字典序比较语义。
    /// - 返回：1 满足，0 不满足，-1 条件不支持。
    /// 字符串按条件比较。
    /// - 用途：词条、名称等字段的筛选。
    /// - 返回：1 表示满足条件，0 表示不满足，-1 表示未知/不支持。
    export inline std::int64_t 比较_字符串条件(const std::string& a, const std::string& b) {
     
         return (a == b) ? 1 : 0;
       
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // //   特征类
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // ===== 特征稳态区间工具（标量=1维）=====

    /// 标量转 VecI64（单元素向量）。
    /// - 用途：把标量特征统一走 VecI64 的稳态区间工具。
    /// 标量转 VecI64（单元素）。
    /// - 用途：把标量特征统一视作 1 维向量，复用区间/距离工具。
    export inline VecI64 标量转VecI64(std::int64_t v) { return VecI64{ v }; }

    /// 由点创建多维区间（点作为区间的唯一边界）。
    /// - 用途：从单点初始化区间，用于后续扩张或比较。
    export inline I64多维区间 区间由点创建(const VecI64& x)
    {
        I64多维区间 box;
        box.维度.reserve(x.size());
        for (auto v : x) box.维度.push_back({ v, v });
        return box;
    }

    /// 判断点 x 与区间 box 的维度数是否一致。
    /// - 用途：避免区间运算维度不匹配导致越界。
    export inline bool 维度一致(const VecI64& x, const I64多维区间& box) noexcept
    {
        return !x.empty() && box.维度数() == x.size();
    }

    /// 判断点是否落在区间内（每维允许 ±tol）。
    /// - 用途：稳态命中/候选归并。
    export inline bool 点在区间(const VecI64& x, const I64多维区间& box, std::int64_t tol = 0) noexcept
    {
        if (!box.有效() || !维度一致(x, box)) return false;
        for (std::size_t i = 0; i < x.size(); ++i) {
            const auto lo = box.维度[i].低值 - tol;
            const auto hi = box.维度[i].高值 + tol;
            if (x[i] < lo || x[i] > hi) return false;
        }
        return true;
    }

    /// 点到区间的 L∞ 距离（最大维度间隙），0 表示在区间内。
    /// - 用途：计算“离稳态多远”，便于做阈值判定。
    export inline std::int64_t 点到区间_Linf(const VecI64& x, const I64多维区间& box) noexcept
    {
        if (!box.有效() || !维度一致(x, box)) return INT64_MAX;

        std::int64_t d = 0;
        for (std::size_t i = 0; i < x.size(); ++i) {
            const auto lo = box.维度[i].低值;
            const auto hi = box.维度[i].高值;
            std::int64_t gi = 0;
            if (x[i] < lo) gi = lo - x[i];
            else if (x[i] > hi) gi = x[i] - hi;
            if (gi > d) d = gi;
        }
        return d;
    }

    /// 将点并入区间（扩张 box 的每维 lo/hi）。
    /// - 若 box 无效则用点初始化。
    export inline bool 区间并入点(I64多维区间& box, const VecI64& x) noexcept
    {
        if (x.empty()) return false;

        if (!box.有效()) {
            box = 区间由点创建(x);
            return true;
        }
        if (!维度一致(x, box)) return false;

        for (std::size_t i = 0; i < x.size(); ++i) {
            box.维度[i].低值 = std::min(box.维度[i].低值, x[i]);
            box.维度[i].高值 = std::max(box.维度[i].高值, x[i]);
        }
        return true;
    }

    // 区间相交（允许每维 ±tol）
    /// 判断两个多维区间是否相交（每维允许 ±tol）。
    /// - 用途：稳态簇合并/候选交叠判断。
    export inline bool 区间相交(const I64多维区间& A, const I64多维区间& B, std::int64_t tol = 0) noexcept
    {
        if (!A.有效() || !B.有效() || A.维度数() != B.维度数()) return false;
        for (std::size_t i = 0; i < A.维度数(); ++i) {
            const auto alo = A.维度[i].低值 - tol, ahi = A.维度[i].高值 + tol;
            const auto blo = B.维度[i].低值 - tol, bhi = B.维度[i].高值 + tol;
            if (ahi < blo || bhi < alo) return false;
        }
        return true;
    }

    // A 是否包含 B（允许每维 ±tol）
    /// 判断区间 A 是否包含区间 B（每维允许 ±tol）。
    /// - 用途：层级稳态、父子范围约束。
    export inline bool 区间包含(const I64多维区间& A, const I64多维区间& B, std::int64_t tol = 0) noexcept
    {
        if (!A.有效() || !B.有效() || A.维度数() != B.维度数()) return false;
        for (std::size_t i = 0; i < A.维度数(); ++i) {
            if ((A.维度[i].低值 - tol) > B.维度[i].低值) return false;
            if ((A.维度[i].高值 + tol) < B.维度[i].高值) return false;
        }
        return true;
    }

    // 区间到区间 L∞ 间隙，0 表示相交
    /// 区间到区间的 L∞ 间隙，0 表示相交。
    /// - 用途：做距离式合并策略（间隙小则可合并）。
    export inline std::int64_t 区间间隙_Linf(const I64多维区间& A, const I64多维区间& B) noexcept
    {
        if (!A.有效() || !B.有效() || A.维度数() != B.维度数()) return INT64_MAX;

        std::int64_t d = 0;
        for (std::size_t i = 0; i < A.维度数(); ++i) {
            std::int64_t gi = 0;
            if (A.维度[i].高值 < B.维度[i].低值) gi = B.维度[i].低值 - A.维度[i].高值;
            else if (B.维度[i].高值 < A.维度[i].低值) gi = A.维度[i].低值 - B.维度[i].高值;
            if (gi > d) d = gi;
        }
        return d;
    }

    /// 合并两个多维区间：A 扩张为覆盖 A∪B。
    /// - 若 A 无效则 A=B。
    export inline bool 区间合并(I64多维区间& A, const I64多维区间& B) noexcept
    {
        if (!B.有效()) return false;
        if (!A.有效()) { A = B; return true; }
        if (A.维度数() != B.维度数()) return false;

        for (std::size_t i = 0; i < A.维度数(); ++i) {
            A.维度[i].低值 = std::min(A.维度[i].低值, B.维度[i].低值);
            A.维度[i].高值 = std::max(A.维度[i].高值, B.维度[i].高值);
        }
        return true;
    }


    /// 64 位海明距离（返回置位差异的 bit 数）。
    /// - 返回：popcount(a^b)。
    export inline std::int64_t 海明_u64(std::uint64_t a, std::uint64_t b) noexcept {
        return (std::int64_t)popcnt64(a ^ b);
    }

    /// VecIU64 的海明距离（返回 bit 数，不归一化）。
    /// - 用途：当你需要“绝对差异位数”而非 0..1 归一化距离时使用。
    /// - 规则：尾部不等长按每多一个块惩罚 64 bit。
    export inline std::int64_t 海明_vec_u64(const VecIU64& a, const VecIU64& b) noexcept {
        const size_t n = std::min(a.size(), b.size());
        std::int64_t sum = 0;
        for (size_t i = 0; i < n; ++i) sum += 海明_u64(a[i], b[i]);
        // 尾部不等长可以视为惩罚：每多一个块加64
        sum += (std::int64_t)((a.size() > n ? a.size() - n : 0) + (b.size() > n ? b.size() - n : 0)) * 64;
        return sum;
    }

    /// int64 的 L1 距离：|a-b|。

    export inline std::int64_t L1_i64(I64 a, I64 b) noexcept {
        const std::int64_t d = a - b;
        return d >= 0 ? d : -d;
    }

    /// VecI64 的 L1 距离（不归一化）。
    /// - 用途：用于阈值判断、排序等需要绝对尺度的场景。
    /// - 规则：不等长部分按与 0 的差异计入。
    export inline std::int64_t L1_vec_i64(const VecI64& a, const VecI64& b) noexcept {
        const size_t n = std::min(a.size(), b.size());
        std::int64_t sum = 0;
        for (size_t i = 0; i < n; ++i) sum += L1_i64(a[i], b[i]);
        // 不等长可按绝对值惩罚
        for (size_t i = n; i < a.size(); ++i) sum += (a[i] >= 0 ? a[i] : -a[i]);
        for (size_t i = n; i < b.size(); ++i) sum += (b[i] >= 0 ? b[i] : -b[i]);
        return sum;
    }

    // L2：返回平方距离
    /// VecI64 的 L2 距离平方（返回 sum (a_i-b_i)^2）。
    /// - 用途：需要避免 sqrt 的快速比较。
    export inline std::int64_t L2sq_vec_i64(const VecI64& a, const VecI64& b) noexcept {
        const size_t n = std::min(a.size(), b.size());
        long long sum = 0;
        for (size_t i = 0; i < n; ++i) {
            const long long d = (long long)a[i] - (long long)b[i];
            sum += d * d;
        }
        // 不等长：追加项按自身平方惩罚
        for (size_t i = n; i < a.size(); ++i) { long long d = (long long)a[i]; sum += d * d; }
        for (size_t i = n; i < b.size(); ++i) { long long d = (long long)b[i]; sum += d * d; }
        return (std::int64_t)sum;
    }

    /// 判断两个1D AABB是否相交。
    export inline bool AABB相交_1D(std::int64_t amin, std::int64_t amax, std::int64_t bmin, std::int64_t bmax)
    {
        return !(amax < bmin || bmax < amin);
    }

    /// 计算两个3D AABB的交叠体积。
    export inline double AABB交叠体积(
        std::int64_t aMinX, std::int64_t aMaxX, std::int64_t aMinY, std::int64_t aMaxY, std::int64_t aMinZ, std::int64_t aMaxZ,
        std::int64_t bMinX, std::int64_t bMaxX, std::int64_t bMinY, std::int64_t bMaxY, std::int64_t bMinZ, std::int64_t bMaxZ)
    {
        const std::int64_t ix = std::max<std::int64_t>(0, std::min(aMaxX, bMaxX) - std::max(aMinX, bMinX));
        const std::int64_t iy = std::max<std::int64_t>(0, std::min(aMaxY, bMaxY) - std::max(aMinY, bMinY));
        const std::int64_t iz = std::max<std::int64_t>(0, std::min(aMaxZ, bMaxZ) - std::max(aMinZ, bMinZ));
        return (double)ix * (double)iy * (double)iz;
    }

    /// 为子存在生成部件键（简化版，仅使用子存在自身信息）。
    /// - 用途：当没有父存在信息时使用。
    export inline std::uint64_t 生成子存在部件键(const 结构体_存在观测& sub) {
        // 使用子存在的中心坐标、尺寸和轮廓编码生成部件键
        // 量化到 5mm 网格（更抗抖）
        constexpr double grid_mm = 5.0;
        const std::int64_t cx = (std::int64_t)std::llround(sub.中心坐标.x / grid_mm);
        const std::int64_t cy = (std::int64_t)std::llround(sub.中心坐标.y / grid_mm);
        const std::int64_t cz = (std::int64_t)std::llround(sub.中心坐标.z / grid_mm);

        const std::int64_t sx = (std::int64_t)std::llround(sub.尺寸.x / grid_mm);
        const std::int64_t sy = (std::int64_t)std::llround(sub.尺寸.y / grid_mm);
        const std::int64_t sz = (std::int64_t)std::llround(sub.尺寸.z / grid_mm);

        // 2D 签名：轮廓编码 hash（若为空则为 0）
        const std::uint64_t sig2d = sub.轮廓编码.empty() ? 0ull : FNV1a64_vec_u64(sub.轮廓编码);

        struct Pack {
            std::int64_t cx, cy, cz;
            std::int64_t sx, sy, sz;
            std::uint64_t sig2d;
        } pack{ cx, cy, cz, sx, sy, sz, sig2d };

        return FNV1a64(&pack, sizeof(pack));
    }

    /// 生成“子在父局部坐标系下的 AABB”（6个整数：minXYZ/maxXYZ）。
    /// - 用途：对子存在做父内部世界的空间索引、粗匹配、快速相交测试。
    /// - 做法：把子 OBB 投影到父 PCA 轴坐标系，计算各轴半径，再给出局部 AABB。
    export inline VecI64 生成相对AABB_父局部(const 结构体_存在观测& sub, const 结构体_存在观测& parent) {
        // 先把子 OBB 投影到父 PCA 主轴坐标系，再生成父局部 AABB
        auto dot3 = [](const Vector3D& a, const Vector3D& b)->double { return a.x * b.x + a.y * b.y + a.z * b.z; };
        auto norm3 = [&](const Vector3D& v)->double { return std::sqrt(dot3(v, v)); };
        auto safe_unit = [&](Vector3D v)->Vector3D {
            const double n = norm3(v);
            if (n <= 1e-9) return { 1,0,0 };
            return { v.x / n, v.y / n, v.z / n };
            };

        const Vector3D pa1 = safe_unit(parent.主方向1);
        const Vector3D pa2 = safe_unit(parent.主方向2);
        const Vector3D pa3 = safe_unit(parent.主方向3);

        const Vector3D sa1 = safe_unit(sub.主方向1);
        const Vector3D sa2 = safe_unit(sub.主方向2);
        const Vector3D sa3 = safe_unit(sub.主方向3);

        const Vector3D d = { sub.中心坐标.x - parent.中心坐标.x,
                             sub.中心坐标.y - parent.中心坐标.y,
                             sub.中心坐标.z - parent.中心坐标.z };

        // 子中心在父局部坐标系中的坐标（父原点为 parent.中心坐标，轴向为 parent 主轴）
        const double cx = dot3(d, pa1);
        const double cy = dot3(d, pa2);
        const double cz = dot3(d, pa3);

        // 子 OBB 在父主轴坐标系下的各轴半径（投影后的半长）
        const double rx = 0.5 * (sub.尺寸.x * std::abs(dot3(sa1, pa1)) +
            sub.尺寸.y * std::abs(dot3(sa2, pa1)) +
            sub.尺寸.z * std::abs(dot3(sa3, pa1)));
        const double ry = 0.5 * (sub.尺寸.x * std::abs(dot3(sa1, pa2)) +
            sub.尺寸.y * std::abs(dot3(sa2, pa2)) +
            sub.尺寸.z * std::abs(dot3(sa3, pa2)));
        const double rz = 0.5 * (sub.尺寸.x * std::abs(dot3(sa1, pa3)) +
            sub.尺寸.y * std::abs(dot3(sa2, pa3)) +
            sub.尺寸.z * std::abs(dot3(sa3, pa3)));

        // 量化到整数（1mm 网格）
        const std::int64_t minx = (std::int64_t)std::llround(cx - rx);
        const std::int64_t miny = (std::int64_t)std::llround(cy - ry);
        const std::int64_t minz = (std::int64_t)std::llround(cz - rz);
        const std::int64_t maxx = (std::int64_t)std::llround(cx + rx);
        const std::int64_t maxy = (std::int64_t)std::llround(cy + ry);
        const std::int64_t maxz = (std::int64_t)std::llround(cz + rz);

        return VecI64{ minx, miny, minz, maxx, maxy, maxz };
    }

    // ================ 量化工具（毫米 mm + 方向 Q10000） ================
    /// 毫米值量化为整数（四舍五入）。
    /// - 用途：把 double(mm) 写入 VecI64 特征值，确保确定性。
    export inline std::int64_t 取整_mm(double mm) { return (std::int64_t)std::llround(mm); }
    /// 单位向量分量量化为 Q10000（四舍五入）。
    /// - 说明：把 [-1,1] 的方向分量放大 10000 后转整数，便于存储/比较。
    export inline std::int64_t 取整_Q10000(double x) { return (std::int64_t)std::llround(x * 10000.0); }


    /// 轮廓编码相似度（0..1）。
    /// - 基于 blocks 的按位差异：sim = 1 - popcount(xor)/(64*n)。
    /// - 注意：只比较共同长度 n=min(|a|,|b|)，更长部分不计入（适合同规格编码）。
    export inline float 轮廓相似度_VecIU64(const VecIU64& a, const VecIU64& b) {
        const std::size_t n = std::min(a.size(), b.size());
        if (n == 0) return 0.0f;
        std::uint64_t dist = 0;
        for (std::size_t i = 0; i < n; ++i) dist += (std::uint64_t)popcnt64(a[i] ^ b[i]);
        const double bits = 64.0 * (double)n;
        const double sim = 1.0 - (double)dist / bits;
        return (float)std::max(0.0, std::min(1.0, sim));
    }

    // ================ 体素处理工具 ================
    /// 从体素立方体 blocks 推断维度。
    /// - 用途：将一维 blocks 数组转换为三维体素网格维度。
    /// - 返回：是否成功推断维度。
    export inline bool 推断体素维度_从blocks(const std::vector<std::uint64_t>& blocks, int& outNB, int& outN) noexcept
    {
        outNB = 0; outN = 0;
        const std::size_t m = blocks.size();
        if (m == 0) return false;

        int nb = (int)std::llround(std::cbrt((double)m));
        if (nb < 1) nb = 1;
        while ((std::size_t)(nb + 1) * (nb + 1) * (nb + 1) <= m) ++nb;
        while ((std::size_t)nb * nb * nb > m) --nb;
        if ((std::size_t)nb * nb * nb != m) return false;

        outNB = nb;
        outN = nb * 4;
        return outN > 0;
    }

    /// 检查指定坐标的体素是否被占据。
    /// - 用途：判断体素是否存在，用于生成表面网格。
    export inline bool 体素占据_4x4x4(const std::vector<std::uint64_t>& blocks, int N, int NB, int ix, int iy, int iz) noexcept
    {
        if (ix < 0 || iy < 0 || iz < 0 || ix >= N || iy >= N || iz >= N) return false;
        const int bx = ix >> 2;
        const int by = iy >> 2;
        const int bz = iz >> 2;
        const int lx = ix & 3;
        const int ly = iy & 3;
        const int lz = iz & 3;
        const std::size_t bIndex = (std::size_t)bx + (std::size_t)NB * ((std::size_t)by + (std::size_t)NB * (std::size_t)bz);
        if (bIndex >= blocks.size()) return false;
        const int bit = lx + 4 * (ly + 4 * lz);
        return ((blocks[bIndex] >> bit) & 1ULL) != 0ULL;
    }

    /// 检查体素的指定面是否可见（六邻域）。
    /// - 用途：判断体素面是否需要渲染，用于生成表面网格。
    export inline bool 面可见_六邻域(const std::vector<std::uint64_t>& blocks, int N, int NB, int ix, int iy, int iz, int dx, int dy, int dz) noexcept
    {
        return !体素占据_4x4x4(blocks, N, NB, ix + dx, iy + dy, iz + dz);
    }

    // ================ 颜色处理工具 ================
    export struct ColorScheme {
        enum class Kind : std::uint8_t { 无, 每u64一个RGBA, 每u64两个RGBA };
        Kind kind = Kind::无;
        std::uint32_t fallback = 0xFF90FF90u;
        std::size_t nvox = 0;
    };

    /// 推断颜色方案。
    /// - 用途：根据颜色数组大小推断颜色存储方案。
    export inline ColorScheme 推断颜色方案_(const VecIU64& colors, std::size_t nvox, std::uint32_t fallback)
    {
        ColorScheme cs;
        cs.fallback = fallback;
        cs.nvox = nvox;
        if (colors.empty() || nvox == 0) return cs;

        if (colors.size() == nvox) {
            cs.kind = ColorScheme::Kind::每u64一个RGBA;
            return cs;
        }
        if (colors.size() * 2 >= nvox) {
            cs.kind = ColorScheme::Kind::每u64两个RGBA;
            return cs;
        }

        // 其它情况：暂不支持
        cs.kind = ColorScheme::Kind::无;
        return cs;
    }

    // 从点阵图中获取指定坐标的像素值（0或1）
    export inline double 获取像素值(const 点阵图& 图, int x, int y)
    {
        if (x < 0 || x >= 图.宽 || y < 0 || y >= 图.高)
            return 0.0;

        const std::size_t 像素索引 = static_cast<std::size_t>(y) * static_cast<std::size_t>(图.宽) + static_cast<std::size_t>(x);
        const std::size_t 块索引 = 像素索引 / 64;
        const std::size_t 位索引 = 像素索引 % 64;

        if (块索引 >= 图.数据.size())
            return 0.0;

        return (图.数据[块索引] & (1ULL << 位索引)) ? 1.0 : 0.0;
    }
    /// 从颜色数组中获取RGBA32颜色。
    /// - 用途：根据颜色方案从颜色数组中提取单个颜色。
    export inline bool 取颜色RGBA32_(const VecIU64& colors, const ColorScheme& cs, std::size_t idx, std::uint32_t& rgba_out) noexcept
    {
        if (cs.kind == ColorScheme::Kind::无 || colors.empty()) {
            rgba_out = cs.fallback;
            return false;
        }
        if (idx >= cs.nvox) {
            rgba_out = cs.fallback;
            return false;
        }

        if (cs.kind == ColorScheme::Kind::每u64一个RGBA) {
            const std::uint64_t w = colors[idx];
            rgba_out = (std::uint32_t)(w & 0xFFFFFFFFull);
            // 如果写入端用的是 0x00RRGGBB，这里补 A
            if ((rgba_out & 0xFF000000u) == 0) rgba_out |= 0xFF000000u;
            return true;
        }

        // 每u64两个RGBA
        const std::size_t j = idx >> 1;
        if (j >= colors.size()) {
            rgba_out = cs.fallback;
            return false;
        }
        const std::uint64_t w = colors[j];
        if ((idx & 1u) == 0u) rgba_out = (std::uint32_t)(w & 0xFFFFFFFFull);
        else rgba_out = (std::uint32_t)((w >> 32) & 0xFFFFFFFFull);
        if ((rgba_out & 0xFF000000u) == 0) rgba_out |= 0xFF000000u;
        return true;
    }
    // 双线性插值函数
    export inline double 双线性插值(const 点阵图& 图, double x, double y)
    {
        // 计算四个相邻像素的坐标
        int x0 = static_cast<int>(std::floor(x));
        int x1 = x0 + 1;
        int y0 = static_cast<int>(std::floor(y));
        int y1 = y0 + 1;

        // 计算插值权重
        double dx = x - x0;
        double dy = y - y0;

        // 获取四个相邻像素的值
        double v00 = 获取像素值(图, x0, y0);
        double v01 = 获取像素值(图, x1, y0);
        double v10 = 获取像素值(图, x0, y1);
        double v11 = 获取像素值(图, x1, y1);

        // 双线性插值
        double v0 = v00 * (1.0 - dx) + v01 * dx;
        double v1 = v10 * (1.0 - dx) + v11 * dx;
        return v0 * (1.0 - dy) + v1 * dy;
    }
    // 轮廓重采样函数：将输入轮廓调整到目标分辨率
    export inline 点阵图 重采样轮廓(const 点阵图& 输入轮廓, std::int32_t 目标宽, std::int32_t 目标高)
    {
        点阵图 输出轮廓;
        输出轮廓.宽 = 目标宽;
        输出轮廓.高 = 目标高;

        if (目标宽 <= 0 || 目标高 <= 0 || 输入轮廓.宽 <= 0 || 输入轮廓.高 <= 0)
        {
            // 空图
            输出轮廓.数据.clear();
            return 输出轮廓;
        }

        // 计算缩放因子
        double 缩放_x = static_cast<double>(输入轮廓.宽 - 1) / static_cast<double>(目标宽 - 1);
        double 缩放_y = static_cast<double>(输入轮廓.高 - 1) / static_cast<double>(目标高 - 1);

        // 计算需要的块数
        const std::size_t N = static_cast<std::size_t>(目标宽) * static_cast<std::size_t>(目标高);
        const std::size_t 期望块数 = (N + 63u) / 64u;
        输出轮廓.数据.assign(期望块数, 0);

        // 对每个目标像素进行重采样
        for (int y = 0; y < 目标高; ++y)
        {
            for (int x = 0; x < 目标宽; ++x)
            {
                // 计算在原始图像中的坐标
                double src_x = x * 缩放_x;
                double src_y = y * 缩放_y;

                // 使用双线性插值获取值
                double val = 双线性插值(输入轮廓, src_x, src_y);

                // 根据值设置输出像素
                if (val >= 0.5)
                {
                    // 设置对应位
                    const std::size_t 像素索引 = static_cast<std::size_t>(y) * static_cast<std::size_t>(目标宽) + static_cast<std::size_t>(x);
                    const std::size_t 块索引 = 像素索引 / 64;
                    const std::size_t 位索引 = 像素索引 % 64;
                    输出轮廓.数据[块索引] |= (1ULL << 位索引);
                }
            }
        }

        return 输出轮廓;
    }




    // 计算轮廓的 IoU（交并比）
    export inline double 计算_轮廓_IoU(const 点阵图& A, const 点阵图& B)
    {
        if (A.宽 != B.宽 || A.高 != B.高)
            return 0.0;

        int 交集 = 0;
        int 并集 = 0;

        for (int y = 0; y < A.高; ++y)
        {
            for (int x = 0; x < A.宽; ++x)
            {
                bool a = 获取像素值(A, x, y) >= 0.5;
                bool b = 获取像素值(B, x, y) >= 0.5;
                if (a && b)
                    ++交集;
                if (a || b)
                    ++并集;
            }
        }

        if (并集 == 0)
            return 0.0;

        return static_cast<double>(交集) / static_cast<double>(并集);
    }

    // 计算轮廓的汉明相似度
    export inline double 计算_轮廓_汉明相似度(const 点阵图& A, const 点阵图& B)
    {
        if (A.宽 != B.宽 || A.高 != B.高)
            return 0.0;

        int 相同 = 0;
        int 总像素数 = A.宽 * A.高;

        for (int y = 0; y < A.高; ++y)
        {
            for (int x = 0; x < A.宽; ++x)
            {
                bool a = 获取像素值(A, x, y) >= 0.5;
                bool b = 获取像素值(B, x, y) >= 0.5;
                if (a == b)
                    ++相同;
            }
        }

        return static_cast<double>(相同) / static_cast<double>(总像素数);
    }

    // 智能轮廓比较：自动处理分辨率不一致的情况
    export inline double 计算_轮廓_相似度(const 点阵图& A, const 点阵图& B, bool 使用IoU = false)
    {
        // 如果分辨率相同，直接比较
        if (A.宽 == B.宽 && A.高 == B.高)
        {
            if (使用IoU)
            {
                return 计算_轮廓_IoU(A, B);
            }
            else
            {
                return 计算_轮廓_汉明相似度(A, B);
            }
        }

        // 分辨率不同，重采样到较大的分辨率
        std::int32_t 目标宽 = std::max(A.宽, B.宽);
        std::int32_t 目标高 = std::max(A.高, B.高);

        点阵图 A重采样 = 重采样轮廓(A, 目标宽, 目标高);
        点阵图 B重采样 = 重采样轮廓(B, 目标宽, 目标高);

        // 比较重采样后的轮廓
        if (使用IoU)
        {
            return 计算_轮廓_IoU(A重采样, B重采样);
        }
        else
        {
            return 计算_轮廓_汉明相似度(A重采样, B重采样);
        }
    }

    export constexpr std::int64_t 饱和减(std::int64_t 左值, std::int64_t 右值) noexcept {
        const std::int64_t 最大 = (std::numeric_limits<std::int64_t>::max)();
        const std::int64_t 最小 = (std::numeric_limits<std::int64_t>::min)();

        // 目标：返回 clamp(左值 - 右值, 最小..最大)，且不使用 __int128、不触发溢出 UB
        if (右值 > 0) {
            // 下溢：左值 - 右值 < 最小  <=>  左值 < 最小 + 右值
            if (左值 < 最小 + 右值) return 最小;
            return 左值 - 右值;
        }

        if (右值 < 0) {
            // 上溢：左值 - 右值 > 最大  <=>  左值 > 最大 + 右值   (右值为负)
            if (左值 > 最大 + 右值) return 最大;
            return 左值 - 右值;
        }

        return 左值;
    }
    export constexpr std::int64_t 饱和加(std::int64_t 左值, std::int64_t 右值) noexcept
    {
        const std::int64_t 最大 = (std::numeric_limits<std::int64_t>::max)();
        const std::int64_t 最小 = (std::numeric_limits<std::int64_t>::min)();

        // 目标：返回 clamp(左值 + 右值, 最小..最大)，且不使用 __int128、不触发溢出 UB
        if (右值 > 0) {
            // 上溢：左值 + 右值 > 最大  <=>  左值 > 最大 - 右值
            if (左值 > 最大 - 右值) return 最大;
        }
        else if (右值 < 0) {
            // 下溢：左值 + 右值 < 最小  <=>  左值 < 最小 - 右值
            if (左值 < 最小 - 右值) return 最小;
        }

        return 左值 + 右值;
    }
    // -------------------- 小工具：遍历同层环（安全防御版） --------------------
    export   template<class NodePtr, class Fn>
        void 遍历同层环(NodePtr start, Fn&& fn) {
        if (!start) return;
        auto it = start;
        int guard = 0;
        do {
            fn(it);
            it = it ? it->下 : nullptr;
            if (++guard > 8192) break; // 防御：避免链损坏导致死循环
        } while (it && it != start);
    }

   

  

  export inline void 追加唯一(std::vector<std::string>& v, std::string s) {
        if (std::find(v.begin(), v.end(), s) == v.end()) {
            v.push_back(std::move(s));
        }
    }
  export inline void 拼接字符串(std::string& base, const std::string& addition) {
        if (!base.empty() && !addition.empty()) {
            base += "; ";
        }
        base += addition;
  }
  // ============ 量化/还原（Q10000） ============

  export inline double 还原_Q10000(std::int64_t q) { return (double)q / 10000.0; }





  // 相对坐标严格相等（mm整数三元）
  static inline bool 坐标严格相等_3i(const std::vector<std::int64_t>& a, const std::vector<std::int64_t>& b) noexcept {
      return a.size() >= 3 && b.size() >= 3 && a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
  }

  // AABB重叠（中心+尺寸，允许容差）
  static inline bool AABB重叠(const std::vector<std::int64_t>& ca, const std::vector<std::int64_t>& sa,
      const std::vector<std::int64_t>& cb, const std::vector<std::int64_t>& sb,
      std::int64_t tol) noexcept
  {
      if (ca.size() < 3 || sa.size() < 3 || cb.size() < 3 || sb.size() < 3) return false;

      auto abs64 = [](std::int64_t x) { return x < 0 ? -x : x; };
      const std::int64_t dx = abs64(ca[0] - cb[0]);
      const std::int64_t dy = abs64(ca[1] - cb[1]);
      const std::int64_t dz = abs64(ca[2] - cb[2]);

      const std::int64_t hx = (sa[0] + sb[0]) / 2 + tol;
      const std::int64_t hy = (sa[1] + sb[1]) / 2 + tol;
      const std::int64_t hz = (sa[2] + sb[2]) / 2 + tol;

      return dx <= hx && dy <= hy && dz <= hz;
  }

  // 遮挡友好轮廓相似：inter/min(popcnt)，适合“被遮挡=子集”
  static inline float 轮廓相似_遮挡友好(const std::vector<std::uint64_t>& A,
      const std::vector<std::uint64_t>& B,
      std::uint64_t& outMinCnt) noexcept
  {
      outMinCnt = 0;
      const std::size_t n = std::min(A.size(), B.size());
      if (n == 0) return 0.0f;

      std::uint64_t inter = 0, cntA = 0, cntB = 0;
      for (std::size_t i = 0; i < n; ++i) {
          inter += (std::uint64_t)popcnt64(A[i] & B[i]);
          cntA += (std::uint64_t)popcnt64(A[i]);
          cntB += (std::uint64_t)popcnt64(B[i]);
      }
      outMinCnt = (cntA < cntB) ? cntA : cntB;
      if (outMinCnt == 0) return 0.0f;
      return clamp01((float)inter / (float)outMinCnt);
  }
  /// 混入字符串（按字节）到已有 hash。
  export inline std::uint64_t Hash64_混入字符串(std::uint64_t h, const std::string& s) noexcept
  {
      constexpr std::uint64_t prime = 1099511628211ull;
      for (unsigned char c : s) {
          h ^= (std::uint64_t)c;
          h *= prime;
      }
      return h;
  }
}; //namespace



