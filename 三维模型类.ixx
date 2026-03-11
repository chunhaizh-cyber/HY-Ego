export module 三维模型模块;

import <vector>;
import <cstdint>;
import <cmath>;
import <algorithm>;

import 基础数据类型模块;
// 需要能看到 点簇增强结果 的完整定义（中心/尺寸/主方向1..3）
// 若你的项目里该结构体在其它模块，请把下面这一行改成正确的 import
import 点簇分割模块;

export class 三维模型类 {
public:
    // ===================== 对外：结果存储（4×4×4 bit 块） =====================
    struct 体素立方体4 {
        int N = 0;          // 分辨率，必须是 4 的倍数
        int NB = 0;         // N/4
        double s_mm = 1.0;  // 体素边长(mm)
        double L_mm = 0.0;  // 立方体边长(mm) = N*s_mm
        std::vector<std::uint64_t> blocks; // NB^3，每块 4×4×4 = 64bit

        void 初始化_全1(int N_, double s_mm_) {
            N = N_;
            s_mm = s_mm_;
            NB = (N > 0) ? (N / 4) : 0;
            L_mm = N * s_mm;
            blocks.assign((size_t)NB * NB * NB, ~0ull); // 全未知=1
        }

        // 从成熟 blocks 初始化（用于“引进成熟模型”）
        // - 不做融合、不做猜测：只校验尺寸并拷贝。
        // - N 必须为 4 的倍数；blocks.size() 必须为 (N/4)^3。
        // 返回：true=成功初始化；false=拒绝（调用方回退到 初始化_全1）。
        bool 初始化_从blocks(int N_, double s_mm_, const std::vector<std::uint64_t>& inBlocks)
        {
            if (N_ <= 0 || (N_ % 4) != 0) return false;
            const int NB_ = N_ / 4;
            const std::size_t need = (std::size_t)NB_ * (std::size_t)NB_ * (std::size_t)NB_;
            if (inBlocks.size() != need) return false;

            N = N_;
            s_mm = s_mm_;
            NB = NB_;
            L_mm = (double)N * (double)s_mm;
            blocks = inBlocks;
            return true;
        }

        inline bool 越界(int ix, int iy, int iz) const {
            return (ix < 0 || iy < 0 || iz < 0 || ix >= N || iy >= N || iz >= N);
        }

        inline void 清零体素(int ix, int iy, int iz) {
            if (越界(ix, iy, iz)) return;
            int bx = ix >> 2, by = iy >> 2, bz = iz >> 2;
            int lx = ix & 3, ly = iy & 3, lz = iz & 3;
            size_t bIndex = (size_t)bx + (size_t)NB * ((size_t)by + (size_t)NB * (size_t)bz);
            int bit = lx + 4 * (ly + 4 * lz); // 0..63
            blocks[bIndex] &= ~(1ull << bit);
        }

        // 读体素：返回 1=占据/未知，0=确定为空
        inline bool 取体素(int ix, int iy, int iz) const noexcept {
            if (越界(ix, iy, iz)) return false;
            int bx = ix >> 2, by = iy >> 2, bz = iz >> 2;
            int lx = ix & 3, ly = iy & 3, lz = iz & 3;
            size_t bIndex = (size_t)bx + (size_t)NB * ((size_t)by + (size_t)NB * (size_t)bz);
            int bit = lx + 4 * (ly + 4 * lz); // 0..63
            return ((blocks[bIndex] >> bit) & 1ull) != 0ull;
        }

        // 体素中心 -> 本体坐标（mm）。以体素立方体中心为 (0,0,0)。
        inline Vector3D 体素中心_本体坐标mm(int ix, int iy, int iz) const noexcept {
            const double half = 0.5 * (double)N;
            const double x = ((double)ix + 0.5 - half) * (double)s_mm;
            const double y = ((double)iy + 0.5 - half) * (double)s_mm;
            const double z = ((double)iz + 0.5 - half) * (double)s_mm;
            return Vector3D{ x, y, z };
        }


        // obj(mm) -> voxel index（以几何中心对齐：x=0 对应立方体中心）
        inline bool 坐标转索引(const Vector3D& p_mm, int& ix, int& iy, int& iz) const {
            double half = 0.5 * (double)N;
            ix = (int)std::floor((double)p_mm.x / s_mm + half);
            iy = (int)std::floor((double)p_mm.y / s_mm + half);
            iz = (int)std::floor((double)p_mm.z / s_mm + half);
            return !越界(ix, iy, iz);
        }
    };

    // ===================== 对外：存在本体基（相机坐标 -> 本体坐标） =====================
    struct 存在本体基 {
        Vector3D 原点_mm;   // 存在中心（相机坐标，mm）
        Vector3D 轴1;         // 本体X轴在相机坐标中的方向（单位向量）
        Vector3D 轴2;         // 本体Y轴（单位向量）
        Vector3D 轴3;         // 本体Z轴（单位向量，右手系）
    };

    // ===================== 对外：初始化与更新 =====================
    // 从 OBB 尺寸估算立方体参数；N 会向上取整到 4 的倍数
    static inline void 估算立方体参数_用OBB(const 点簇增强结果& r, double s_mm,
        double& outL_mm, int& outN, double L_scale = 1.2, int N_max = 384)
    {
        double L = std::max({ (double)r.尺寸.x, (double)r.尺寸.y, (double)r.尺寸.z });
        L *= L_scale;
        int N = (int)std::ceil(L / s_mm);
        N = round_up_mul(N, 4);
        if (N_max > 0) N = std::min(N, N_max);
        outL_mm = (double)N * s_mm; // 以 N 对齐后的边长为准
        outN = N;
    }

    // 构建稳定本体基：正交化 + 右手系 + 与 prev 的连续性
    static inline 存在本体基 构建本体基_稳定(const 点簇增强结果& r, const 存在本体基* prev = nullptr)
    {
        存在本体基 b;
        b.原点_mm = r.中心;

        Vector3D a1 = 归一化(r.主方向1);
        Vector3D a2 = 归一化(r.主方向2);
        Vector3D a3_hint = 归一化(r.主方向3);

        // --- 施密特：让 a2 与 a1 正交 ---
        a2 = 减(a2, 标量乘(a1, 点乘(a2, a1)));
        if (范数(a2) <= 1e-9) {
            // a2 退化：挑一个与 a1 不共线的备选轴再叉出 a2
            Vector3D tmp = (std::abs((double)a1.x) < 0.9) ? Vector3D{ 1,0,0 } : Vector3D{ 0,1,0 };
            a2 = 叉乘(tmp, a1);
        }
        a2 = 归一化(a2);

        // --- 右手系：a3 = a1 × a2 ---
        Vector3D a3 = 归一化(叉乘(a1, a2));

        // 尽量贴近 PCA 的第三轴提示，避免突然翻面
        if (点乘(a3, a3_hint) < 0) {
            a2 = 标量乘(a2, -1.0);
            a3 = 标量乘(a3, -1.0);
        }

        // 连续性：与上一帧对齐符号
        if (prev) {
            if (点乘(a1, prev->轴1) < 0) { // 整体翻
                a1 = 标量乘(a1, -1.0);
                a2 = 标量乘(a2, -1.0);
                a3 = 标量乘(a3, -1.0);
            }
            if (点乘(a2, prev->轴2) < 0) { // 翻 a2 及 a3 保持右手
                a2 = 标量乘(a2, -1.0);
                a3 = 标量乘(a3, -1.0);
            }
            // 兜底右手系
            Vector3D a3_check = 归一化(叉乘(a1, a2));
            if (点乘(a3_check, a3) < 0) {
                a2 = 标量乘(a2, -1.0);
                a3 = 标量乘(a3, -1.0);
            }
        }

        b.轴1 = a1; b.轴2 = a2; b.轴3 = a3;
        return b;
    }

    // 相机坐标点 P_cam(mm) -> 本体坐标 P_obj(mm)
    static inline Vector3D CamToObj(const 存在本体基& b, const Vector3D& P_cam_mm) {
        Vector3D v = 减(P_cam_mm, b.原点_mm);
        return Vector3D{ 点乘(v, b.轴1), 点乘(v, b.轴2), 点乘(v, b.轴3) };
    }

    // 相机原点 (0,0,0) -> 本体坐标
    static inline Vector3D CamOriginToObj(const 存在本体基& b) {
        Vector3D v = 标量乘(b.原点_mm, -1.0);
        return Vector3D{ 点乘(v, b.轴1), 点乘(v, b.轴2), 点乘(v, b.轴3) };
    }

    // ===================== 对外：雕刻入口（正面可见部分） =====================
    // 约定：体素值含义 1=可能是实体/未知(含遮挡后方)，0=确定为空
    // 本函数：沿射线把 [入口, 表面-ε] 区间置 0
    static inline void 雕刻_单射线(体素立方体4& S,
        const Vector3D& O_obj_mm, const Vector3D& P_obj_mm, double eps_mm)
    {
        // 统一用 double 计算
        Vec3d O{ (double)O_obj_mm.x, (double)O_obj_mm.y, (double)O_obj_mm.z };
        Vec3d P{ (double)P_obj_mm.x, (double)P_obj_mm.y, (double)P_obj_mm.z };
        雕刻_单射线_impl(S, O, P, eps_mm);
    }
    // 本体坐标 -> 相机坐标（mm）
    static inline Vector3D ObjToCam(const 存在本体基& b, const Vector3D& P_obj_mm) noexcept {
        return Vector3D{
            b.原点_mm.x + b.轴1.x * P_obj_mm.x + b.轴2.x * P_obj_mm.y + b.轴3.x * P_obj_mm.z,
            b.原点_mm.y + b.轴1.y * P_obj_mm.x + b.轴2.y * P_obj_mm.y + b.轴3.y * P_obj_mm.z,
            b.原点_mm.z + b.轴1.z * P_obj_mm.x + b.轴2.z * P_obj_mm.y + b.轴3.z * P_obj_mm.z
        };
    }
private:
    // ===================== 内部：double 向量（几何计算用） =====================
    struct Vec3d {
        double x{}, y{}, z{};
        Vec3d operator+(const Vec3d& r) const { return { x + r.x,y + r.y,z + r.z }; }
        Vec3d operator-(const Vec3d& r) const { return { x - r.x,y - r.y,z - r.z }; }
        Vec3d operator*(double s) const { return { x * s,y * s,z * s }; }
    };

    static inline double dot(const Vec3d& a, const Vec3d& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
    static inline double norm(const Vec3d& v) { return std::sqrt(dot(v, v)); }
    static inline Vec3d normalize(const Vec3d& v) {
        double n = norm(v);
        if (n <= 1e-12) return { 1,0,0 }; // 给一个稳妥的默认方向，避免 0 向量导致 step=0 全挂
        return { v.x / n, v.y / n, v.z / n };
    }
public:
    static inline int round_up_mul(int v, int m) { return (v + m - 1) / m * m; }
private:
    // ===================== 内部：Vector3D 工具 =====================
    static inline double 点乘(const Vector3D& a, const Vector3D& b) {
        return (double)a.x * (double)b.x + (double)a.y * (double)b.y + (double)a.z * (double)b.z;
    }
    static inline Vector3D 叉乘(const Vector3D& a, const Vector3D& b) {
        return Vector3D{
            (double)a.y * (double)b.z - (double)a.z * (double)b.y,
            (double)a.z * (double)b.x - (double)a.x * (double)b.z,
            (double)a.x * (double)b.y - (double)a.y * (double)b.x
        };
    }
    static inline double 范数(const Vector3D& v) { return std::sqrt(点乘(v, v)); }
    static inline Vector3D 归一化(const Vector3D& v) {
        double n = 范数(v);
        if (n <= 1e-12) return Vector3D{ 1,0,0 };
        return Vector3D{ v.x / n, v.y / n, v.z / n };
    }
    static inline Vector3D 标量乘(const Vector3D& v, double s) { return Vector3D{ v.x * s, v.y * s, v.z * s }; }
    static inline Vector3D 减(const Vector3D& a, const Vector3D& b) { return Vector3D{ a.x - b.x, a.y - b.y, a.z - b.z }; }

    // ===================== 内部：射线-AABB slab =====================
    static inline bool RayAABB(const Vec3d& O, const Vec3d& D, const Vec3d& bmin, const Vec3d& bmax,
        double& tEnter, double& tExit)
    {
        tEnter = -1e300;
        tExit = 1e300;

        auto slab = [&](double o, double d, double mn, double mx) -> bool {
            if (std::abs(d) < 1e-12) return (o >= mn && o <= mx);
            double inv = 1.0 / d;
            double t0 = (mn - o) * inv;
            double t1 = (mx - o) * inv;
            if (t0 > t1) std::swap(t0, t1);
            tEnter = std::max(tEnter, t0);
            tExit = std::min(tExit, t1);
            return tExit >= tEnter;
            };

        if (!slab(O.x, D.x, bmin.x, bmax.x)) return false;
        if (!slab(O.y, D.y, bmin.y, bmax.y)) return false;
        if (!slab(O.z, D.z, bmin.z, bmax.z)) return false;
        return tExit >= tEnter;
    }

    // ===================== 内部：3D DDA（逐体素清零） =====================
    static inline void 雕刻_DDA(体素立方体4& S, const Vec3d& O, const Vec3d& D, double t0, double t1)
    {
        Vec3d p0 = O + D * t0;

        int ix, iy, iz;
        if (!S.坐标转索引(Vector3D{ p0.x, p0.y, p0.z }, ix, iy, iz)) {
            Vec3d p0b = O + D * (t0 + 1e-6);
            if (!S.坐标转索引(Vector3D{ p0b.x, p0b.y, p0b.z }, ix, iy, iz)) return;
        }

        int stepX = (D.x > 0) ? 1 : (D.x < 0 ? -1 : 0);
        int stepY = (D.y > 0) ? 1 : (D.y < 0 ? -1 : 0);
        int stepZ = (D.z > 0) ? 1 : (D.z < 0 ? -1 : 0);

        auto nextBoundaryT = [&](int i, double d, double o) -> double {
            if (std::abs(d) < 1e-12) return 1e300;
            double half = 0.5 * (double)S.N;
            int next = (d > 0) ? (i + 1) : i;
            double boundary = ((double)next - half) * S.s_mm;
            return (boundary - o) / d;
            };

        double tMaxX = nextBoundaryT(ix, D.x, O.x);
        double tMaxY = nextBoundaryT(iy, D.y, O.y);
        double tMaxZ = nextBoundaryT(iz, D.z, O.z);

        double tDeltaX = (std::abs(D.x) < 1e-12) ? 1e300 : (S.s_mm / std::abs(D.x));
        double tDeltaY = (std::abs(D.y) < 1e-12) ? 1e300 : (S.s_mm / std::abs(D.y));
        double tDeltaZ = (std::abs(D.z) < 1e-12) ? 1e300 : (S.s_mm / std::abs(D.z));

        while (true) {
            S.清零体素(ix, iy, iz);

            double tNext = std::min({ tMaxX, tMaxY, tMaxZ });
            if (tNext > t1) break;

            if (tMaxX <= tMaxY && tMaxX <= tMaxZ) { ix += stepX; tMaxX += tDeltaX; }

            else if (tMaxY <= tMaxZ) { iy += stepY; tMaxY += tDeltaY; }
            else { iz += stepZ; tMaxZ += tDeltaZ; }

            if (ix < 0 || iy < 0 || iz < 0 || ix >= S.N || iy >= S.N || iz >= S.N) break;
        }
    }

    // ===================== 内部：单射线实现（double） =====================
    static inline void 雕刻_单射线_impl(体素立方体4& S, const Vec3d& O_obj, const Vec3d& P_obj, double eps_mm)
    {
        Vec3d v = P_obj - O_obj;
        double dist = norm(v);
        if (dist <= 1e-6) return;

        Vec3d D = v * (1.0 / dist);

        double halfL = 0.5 * S.L_mm;
        Vec3d bmin{ -halfL,-halfL,-halfL }, bmax{ +halfL,+halfL,+halfL };

        double tEnter, tExit;
        if (!RayAABB(O_obj, D, bmin, bmax, tEnter, tExit)) return;

        double t0 = std::max(tEnter, 0.0);
        double t1 = std::min(tExit, dist - eps_mm);
        if (t1 <= t0) return;

        雕刻_DDA(S, O_obj, D, t0, t1);
    }
};
