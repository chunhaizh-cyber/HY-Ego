// 点簇分割模块.ixx（去 OpenCV 版本，基于 结构体_原始场景帧）
export module 点簇分割模块;

import 基础数据类型模块;

import <vector>;
import <cstdint>;
import <cmath>;
import <algorithm>;
import <limits>;
import <utility>;


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
    // 深度有效性
    bool    忽略无效点 = true;
    double  最小深度 = 0.0;        // <=0 表示不限制
    double  最大深度 = 0.0;        // <=0 表示不限制

    // --- 空间滤波 ---
    bool    启用空间滤波 = true;
    int     空间半径 = 1;           // 1=3x3, 2=5x5
    double  空间最大深度差 = 0.03;  // 米：邻域差超过此值不参与平均（保边）
    double  空间平滑强度 = 0.55;    // 0~1：越大越平滑

    // --- 时间滤波 ---
    bool    启用时间滤波 = true;
    double  时间平滑强度 = 0.4;     // 0~1：越大越“跟随当前帧”
    double  时间最大深度差 = 0.03;  // 米：差太大则认为发生遮挡/跳变，不做融合
    bool    无效继承上一帧 = false; // 当前无效时是否沿用上一帧（谨慎开启）

    // --- 洞填补 ---
    bool    启用洞填补 = true;
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
    // 米：相邻像素深度差超过该值则认为出现深度边缘
    double 深度边缘阈值 = 0.03;
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
    double        深度噪声_MAD = 0.0;   // 深度中位绝对偏差（米，鲁棒）
    double        PCA残差 = 1.0;        // 越小越好
    double        边界噪声 = 0.0;       // 越小越好（深度梯度统计）
    double        颜色一致性 = 0.0;     // 0~1，越大越好
    double        质量分 = 0.0;         // 0~1

    Color         平均颜色{};
    Vector3D      主方向1{ 1,0,0 };
    Vector3D      主方向2{ 0,1,0 };
    Vector3D      主方向3{ 0,0,1 };
};



// 像素打包： (v << 32) | u
export inline std::uint64_t 打包像素UV(int u, int v)
{
    return (static_cast<std::uint64_t>(static_cast<std::uint32_t>(v)) << 32)
        | static_cast<std::uint64_t>(static_cast<std::uint32_t>(u));
}
export inline int 解包U(std::uint64_t uv) { return static_cast<int>(static_cast<std::uint32_t>(uv & 0xFFFFFFFFu)); }
export inline int 解包V(std::uint64_t uv) { return static_cast<int>(static_cast<std::uint32_t>(uv >> 32)); }

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

        out边界像素.push_back(打包像素UV(u, v));
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

export class 点簇分割类
{
private:
    static inline int 索引(int u, int v, int w) { return v * w + u; }

    static inline bool 深度有效(double z, const 点簇分割参数& p)
    {
        if (p.忽略无效点 && (!std::isfinite(z) || z <= 0.0)) return false;
        // 兼容：若阈值明显过大（>10），按 mm->m 换算一次
        double minZ = p.最小深度;
        double maxZ = p.最大深度;
        if (minZ > 10.0) minZ *= 0.001;
        if (maxZ > 10.0) maxZ *= 0.001;
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
        if (maxDz > 10.0) maxDz *= 0.001;
        if (std::abs(za - zb) > maxDz) return false;

        // 2) 点云欧氏距离门槛（点云可用时更稳）
        double maxD3 = p.邻域最大三维距离;
        if (maxD3 > 10.0) maxD3 *= 0.001;
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

        // Pass 3：在主方向上投影得到 OBB 尺寸，并计算 OBB 中心
        double mn1 = +std::numeric_limits<double>::infinity(), mx1 = -mn1;
        double mn2 = +std::numeric_limits<double>::infinity(), mx2 = -mn2;
        double mn3 = +std::numeric_limits<double>::infinity(), mx3 = -mn3;

        for (const auto& pix : 簇) {
            Vector3D P{};
            if (!取点云(帧, pix.u, pix.v, P)) continue;
            const double p1 = 点积(P, a1);
            const double p2 = 点积(P, a2);
            const double p3 = 点积(P, a3);
            mn1 = std::min(mn1, p1); mx1 = std::max(mx1, p1);
            mn2 = std::min(mn2, p2); mx2 = std::max(mx2, p2);
            mn3 = std::min(mn3, p3); mx3 = std::max(mx3, p3);
        }

        const double c1 = 0.5 * (mn1 + mx1);
        const double c2 = 0.5 * (mn2 + mx2);
        const double c3 = 0.5 * (mn3 + mx3);

        中心 = { a1.x * c1 + a2.x * c2 + a3.x * c3,
                a1.y * c1 + a2.y * c2 + a3.y * c3,
                a1.z * c1 + a2.z * c2 + a3.z * c3 };

        尺寸 = { std::max(0.0, mx1 - mn1), std::max(0.0, mx2 - mn2), std::max(0.0, mx3 - mn3) };
    }

    // ===== 小工具：对 0/1 掩码做一次 3x3 膨胀 =====
    static void 掩码膨胀一次(std::vector<std::uint8_t>& m, int w, int h)
    {
        if (m.empty() || w <= 2 || h <= 2) return;
        std::vector<std::uint8_t> src = m;
        auto at = [&](int x, int y) -> std::uint8_t { return src[static_cast<std::size_t>(y) * w + x]; };

        for (int y = 1; y < h - 1; ++y) {
            for (int x = 1; x < w - 1; ++x) {
                if (at(x, y)) continue;
                std::uint8_t v =
                    at(x - 1, y - 1) | at(x, y - 1) | at(x + 1, y - 1) |
                    at(x - 1, y) | at(x + 1, y) |
                    at(x - 1, y + 1) | at(x, y + 1) | at(x + 1, y + 1);
                if (v) m[static_cast<std::size_t>(y) * w + x] = 1;
            }
        }
    }

    // ===== 小工具：填洞（从边界 flood fill 背景，再把未触达的 0 视为洞填为 1）=====
    static void 掩码填洞(std::vector<std::uint8_t>& m, int w, int h)
    {
        if (m.empty() || w <= 2 || h <= 2) return;

        std::vector<std::uint8_t> vis(static_cast<std::size_t>(w) * h, 0);
        std::vector<int> q;
        q.reserve(w * h / 8);

        auto push_if = [&](int x, int y) {
            const std::size_t id = static_cast<std::size_t>(y) * w + x;
            if (vis[id]) return;
            if (m[id]) return; // 前景不走
            vis[id] = 1;
            q.push_back(static_cast<int>(id));
            };

        // 边界入队
        for (int x = 0; x < w; ++x) { push_if(x, 0); push_if(x, h - 1); }
        for (int y = 0; y < h; ++y) { push_if(0, y); push_if(w - 1, y); }

        auto pop = [&]() -> int { int id = q.back(); q.pop_back(); return id; };

        while (!q.empty()) {
            const int id = pop();
            const int x = id % w;
            const int y = id / w;

            const int nx[4] = { x - 1, x + 1, x, x };
            const int ny[4] = { y, y, y - 1, y + 1 };
            for (int k = 0; k < 4; ++k) {
                int xx = nx[k], yy = ny[k];
                if ((unsigned)xx >= (unsigned)w || (unsigned)yy >= (unsigned)h) continue;
                push_if(xx, yy);
            }
        }

        // 未被访问到的背景 = 洞
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                const std::size_t id = static_cast<std::size_t>(y) * w + x;
                if (!m[id] && !vis[id]) m[id] = 1;
            }
        }
    }

    // ===== 8x8 轮廓编码：直接从像素点映射到 8x8，再做填洞 =====

// ===== 8x8 轮廓编码（深度边缘版）：先提边缘，再映射到 8x8，再做封闭增强 =====
// ===== 8x8 轮廓编码：复用已提取的边界像素 =====
    static std::vector<std::int64_t> 计算轮廓编码8x8_从边界像素(
        const std::vector<std::uint64_t>& edges,
        const 点簇边界框& box)
    {
        if (edges.empty()) return {};

        std::vector<std::uint8_t> g(64, 0);
        const int bw = std::max(1, box.umax - box.umin + 1);
        const int bh = std::max(1, box.vmax - box.vmin + 1);

        for (auto uv : edges) {
            const int u = 解包U(uv);
            const int v = 解包V(uv);

            int gx = (u - box.umin) * 8 / bw;
            int gy = (v - box.vmin) * 8 / bh;
            gx = std::clamp(gx, 0, 7);
            gy = std::clamp(gy, 0, 7);
            g[static_cast<std::size_t>(gy) * 8 + static_cast<std::size_t>(gx)] = 1;
        }

        // 封闭性增强：一次膨胀 + 填洞（让轮廓更稳定可匹配）
        掩码膨胀一次(g, 8, 8);
        掩码填洞(g, 8, 8);

        std::vector<std::int64_t> out(64, 0);
        for (int i = 0; i < 64; ++i) out[i] = g[static_cast<std::size_t>(i)] ? 1 : 0;
        return out;
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
            const int u = 解包U(uv);
            const int v = 解包V(uv);
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

        const std::size_t mid = z.size() / 2;
        std::nth_element(z.begin(), z.begin() + mid, z.end());
        const double med = z[mid];
        if (out中位深度) *out中位深度 = med;

        for (auto& v : z) v = std::fabs(v - med);

        std::nth_element(z.begin(), z.begin() + mid, z.end());
        const double mad = z[mid];

        return mad;
    }


    // ===== 平均颜色：严格按对齐后的 (u,v) 取样，边缘降权 =====
    static Color 计算簇平均颜色_边缘降权(
        const 结构体_原始场景帧& 帧,
        const 点簇& 簇,
        const 点簇边界框& box,
        const std::vector<std::uint64_t>& edges,
        double 边缘权重,
        double* out颜色一致性 = nullptr)
    {
        Color out{};
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
            const int u = 解包U(uv) - box.umin;
            const int v = 解包V(uv) - box.vmin;
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
    static std::vector<std::int64_t> 计算轮廓编码8x8_深度边缘(
        const 结构体_原始场景帧& 帧,
        const 点簇& 簇,
        const 点簇边界框& box,
        const 点簇分割参数& 参数)
    {
        // 让边缘阈值跟随分割参数（如果分割时用的深度差更大，则轮廓也不要太敏感）
        深度边缘轮廓参数 cp{};
        if (参数.邻域最大深度差 > 0.0) {
            cp.深度边缘阈值 = std::max(cp.深度边缘阈值, 参数.邻域最大深度差);
        }

        std::vector<std::uint64_t> edges;
        提取轮廓像素_深度边缘(帧, 簇, box, 参数, edges, nullptr, cp);

        if (edges.empty()) {
            // 回退：没提到边缘时，用“簇占据映射”
            return 计算轮廓编码8x8(簇, box);
        }

        std::vector<std::uint8_t> g(64, 0);
        const int bw = std::max(1, box.umax - box.umin + 1);
        const int bh = std::max(1, box.vmax - box.vmin + 1);

        for (auto uv : edges) {
            const int u = 解包U(uv);
            const int v = 解包V(uv);

            int gx = (u - box.umin) * 8 / bw;
            int gy = (v - box.vmin) * 8 / bh;
            gx = std::clamp(gx, 0, 7);
            gy = std::clamp(gy, 0, 7);
            g[static_cast<std::size_t>(gy) * 8 + gx] = 1;
        }

        // 封闭性增强：一次膨胀 + 填洞（让轮廓更稳定可匹配）
        掩码膨胀一次(g, 8, 8);
        掩码填洞(g, 8, 8);

        std::vector<std::int64_t> out(64, 0);
        for (int i = 0; i < 64; ++i) out[i] = g[static_cast<std::size_t>(i)] ? 1 : 0;
        return out;
    }

    static std::vector<std::int64_t> 计算轮廓编码8x8(const 点簇& 簇, const 点簇边界框& box)
    {
        std::vector<std::uint8_t> g(64, 0);
        const int bw = std::max(1, box.umax - box.umin + 1);
        const int bh = std::max(1, box.vmax - box.vmin + 1);

        for (const auto& p : 簇) {
            int gx = (p.u - box.umin) * 8 / bw;
            int gy = (p.v - box.vmin) * 8 / bh;
            gx = std::clamp(gx, 0, 7);
            gy = std::clamp(gy, 0, 7);
            g[static_cast<std::size_t>(gy) * 8 + gx] = 1;
        }

        // 对 8x8 做一次膨胀 + 填洞，增强封闭性
        掩码膨胀一次(g, 8, 8);
        掩码填洞(g, 8, 8);

        std::vector<std::int64_t> out(64, 0);
        for (int i = 0; i < 64; ++i) out[i] = g[static_cast<std::size_t>(i)] ? 1 : 0;
        return out;
    }

public:
    // ===== 基础分割：只输出点簇（像素集合）=====
    std::vector<点簇> 分割点簇(const 结构体_原始场景帧& 帧, const 点簇分割参数& 参数) const
    {
        std::vector<点簇> 输出;

        if (!帧.有效深度()) return 输出;

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
                    当前簇.push_back({ cu, cv });

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
                    输出.push_back(std::move(当前簇));
                }
            }
        }

        return 输出;
    }

    // ===== 点簇 + 边界框 =====
    std::vector<点簇结果> 分割点簇_带边界框(const 结构体_原始场景帧& 帧, const 点簇分割参数& 参数) const
    {
        auto 簇列表 = 分割点簇(帧, 参数);

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
    std::vector<点簇增强结果> 分割点簇_增强(const 结构体_原始场景帧& 帧, const 点簇分割参数& 参数, std::vector<点簇质量信息>* out质量 = nullptr) const
    {
        auto 簇列表 = 分割点簇(帧, 参数);

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

            // 3D 几何（OBB 尺寸 + PCA 主方向）
            Vector3D 主方向1{ 1,0,0 }, 主方向2{ 0,1,0 }, 主方向3{ 0,0,1 };
            double   PCA残差 = 1.0;
            std::uint32_t 有效点数 = 0;
            计算簇3D信息(帧, res.簇, res.中心, res.尺寸, &主方向1, &主方向2, &主方向3, &PCA残差, &有效点数);

            // 深度边缘轮廓：2D 边界像素 -> 反投影 3D 轮廓点（轮廓更干净）
            深度边缘轮廓参数 cp{};
            if (参数.邻域最大深度差 > 0.0) {
                cp.深度边缘阈值 = std::max(cp.深度边缘阈值, 参数.邻域最大深度差);
            }

            std::vector<std::uint64_t> 边界像素;
            std::vector<Vector3D>      轮廓3D_tmp;
            std::vector<Vector3D>* p轮廓3D = nullptr;

            if constexpr (requires { res.轮廓3D; }) p轮廓3D = &轮廓3D_tmp;
            else if constexpr (requires { res.轮廓3D; }) p轮廓3D = &轮廓3D_tmp;

            提取轮廓像素_深度边缘(帧, res.簇, res.边界, 参数, 边界像素, p轮廓3D, cp);

            // 轮廓 8x8（不依赖裁剪掩码）
            if (!边界像素.empty()) {
                res.轮廓编码 = 计算轮廓编码8x8_从边界像素(边界像素, res.边界);
            }
            else {
                res.轮廓编码 = 计算轮廓编码8x8(res.簇, res.边界);
            }

            // 可选：边界像素序列（全图坐标，y<<32|x）
            if constexpr (requires { res.边界点序列; }) {
                res.边界点序列.clear();
                res.边界点序列.reserve(边界像素.size());
                for (auto uv : 边界像素) res.边界点序列.push_back(static_cast<std::int64_t>(uv));
            }

            // 可选：输出 3D 轮廓点
            if constexpr (requires { res.轮廓3D; }) {
                if (p轮廓3D) res.轮廓3D = std::move(轮廓3D_tmp);
            }
            else if constexpr (requires { res.轮廓3D; }) {
                if (p轮廓3D) res.轮廓3D = std::move(轮廓3D_tmp);
            }

            // 颜色：严格按对齐映射取样，边缘降权（平均色更准）
            double 边缘颜色权重 = 0.35;
            if constexpr (requires { 参数.边缘颜色权重; }) {
                边缘颜色权重 = std::clamp((double)参数.边缘颜色权重, 0.0, 1.0);
            }
            double 颜色一致性 = 0.0;
            Color 平均颜色 = 计算簇平均颜色_边缘降权(帧, res.簇, res.边界, 边界像素, 边缘颜色权重, &颜色一致性);

            if constexpr (requires { res.平均颜色; }) res.平均颜色 = 平均颜色;
            else if constexpr (requires { res.平均色; }) res.平均色 = 平均颜色;

            // 质量分/可信度（用于世界树融合时按质量加权）
            const double 有效点比例 = res.簇.empty() ? 0.0 : (double)有效点数 / (double)res.簇.size();
            const double 边界噪声 = 计算边界噪声_深度梯度(帧, 边界像素);

            double 中位深度 = 0.0;
            const double 深度噪声_MAD = 计算深度噪声_MAD(帧, res.簇, &中位深度);

            // 噪声归一化：距离越远允许的 MAD 越大（经验标尺：3mm + 1cm/m）
            const double mad_scale = std::max(0.003, 0.003 + 0.01 * std::max(0.0, 中位深度));
            const double mad_norm = std::clamp(深度噪声_MAD / mad_scale, 0.0, 1.0);

            double noise_norm = 0.0;
            if (cp.深度边缘阈值 > 1e-9) noise_norm = std::clamp(边界噪声 / (cp.深度边缘阈值 * 3.0), 0.0, 1.0);
            const double pca_norm = std::clamp(PCA残差 * 3.0, 0.0, 1.0);

            double q = std::clamp(有效点比例 * (1.0 - noise_norm) * (1.0 - pca_norm) * (1.0 - mad_norm), 0.0, 1.0);
            q = std::clamp(q * (0.5 + 0.5 * 颜色一致性), 0.0, 1.0);


            // 可选：写回 PCA 主方向
            if constexpr (requires { res.主方向1; }) res.主方向1 = 主方向1;
            if constexpr (requires { res.主方向2; }) res.主方向2 = 主方向2;
            if constexpr (requires { res.主方向3; }) res.主方向3 = 主方向3;

            // 可选：写回质量细项
            if constexpr (requires { res.有效点数; }) res.有效点数 = 有效点数;
            if constexpr (requires { res.有效点比例; }) res.有效点比例 = 有效点比例;

            if constexpr (requires { res.深度噪声_MAD; }) res.深度噪声_MAD = 深度噪声_MAD;
            if constexpr (requires { res.中位深度; }) res.中位深度 = 中位深度;


            if constexpr (requires { res.PCA残差; }) res.PCA残差 = PCA残差;
            else if constexpr (requires { res.PCA_残差; }) res.PCA_残差 = PCA残差;

            if constexpr (requires { res.边界噪声; }) res.边界噪声 = 边界噪声;
            if constexpr (requires { res.颜色一致性; }) res.颜色一致性 = 颜色一致性;

            if constexpr (requires { res.质量分; }) res.质量分 = q;
            else if constexpr (requires { res.可信度; }) res.可信度 = q;
            else if constexpr (requires { res.质量; }) res.质量 = q;
            // 裁剪输出（可选）
            if ((参数.输出裁剪图 || 参数.输出裁剪掩码) && w > 0 && h > 0) {
                int u0 = std::max(0, res.边界.umin - 参数.裁剪边距);
                int v0 = std::max(0, res.边界.vmin - 参数.裁剪边距);
                int u1 = std::min(w - 1, res.边界.umax + 参数.裁剪边距);
                int v1 = std::min(h - 1, res.边界.vmax + 参数.裁剪边距);

                const int cw = std::max(0, u1 - u0 + 1);
                const int ch = std::max(0, v1 - v0 + 1);

                const std::size_t area = static_cast<std::size_t>(cw) * static_cast<std::size_t>(ch);
                if (cw > 0 && ch > 0 && (int)area <= 参数.最大裁剪像素) {
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

                        if (参数.掩码膨胀一次) 掩码膨胀一次(res.裁剪掩码, cw, ch);
                        if (参数.掩码填洞) 掩码填洞(res.裁剪掩码, cw, ch);
                    }
                }
            }

            // 可选：输出质量信息（与 out 一一对应）
            if (out质量) {
                点簇质量信息 qi{};
                qi.有效点数 = 有效点数;
                qi.有效点比例 = 有效点比例;
                qi.深度噪声_MAD = 深度噪声_MAD;
                qi.PCA残差 = PCA残差;
                qi.边界噪声 = 边界噪声;
                qi.颜色一致性 = 颜色一致性;
                qi.质量分 = q;
                qi.平均颜色 = 平均颜色;
                qi.主方向1 = 主方向1;
                qi.主方向2 = 主方向2;
                qi.主方向3 = 主方向3;
                out质量->push_back(qi);
            }

            out.push_back(std::move(res));
        }

        return out;
    }
};
