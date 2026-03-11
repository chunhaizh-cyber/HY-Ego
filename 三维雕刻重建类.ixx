export module 三维雕刻重建模块;

import 三维模型模块;
import 基础数据类型模块;

import <vector>;
import <cstdint>;
import <cmath>;
import <algorithm>;

// 目标：把“单帧(点云 + ROI掩膜)”喂给 三维模型类 的射线雕刻。
// 设计原则：不依赖 RealSense / OpenCV；这样任何相机/离线数据都能复用。
export class 三维雕刻重建类 {
public:
    struct 结构体_雕刻参数 {
        int 最小采样步长 = 1;      // 最小采样步长：关键区域使用
        int 最大采样步长 = 4;      // 最大采样步长：快速区域使用
        int 内缩像素 = 1;          // 为避免边缘误雕，对 ROI mask 做一次腐蚀（0=不做）
        double 最小留空厚度 = 0.5;  // 最小表面前留空厚度（mm）
        double 最大留空厚度 = 3.0;  // 最大表面前留空厚度（mm）
        double eps_mm = -1.0;       // 表面前留空厚度；<0 时自动计算
        bool 自适应采样 = true;     // 是否启用自适应采样步长
        bool 动态留空厚度 = true;   // 是否根据质量动态调整留空厚度
    };

    static inline bool 点云有效(const Vector3D& p) {
        return std::isfinite((double)p.x) && std::isfinite((double)p.y) && std::isfinite((double)p.z) && ((double)p.z > 0.0);
    }

    static inline void ROI腐蚀(const std::vector<std::uint8_t>& in, int w, int h, int r, std::vector<std::uint8_t>& out) {
        out.assign((size_t)w * (size_t)h, 0);
        if (w <= 0 || h <= 0) return;
        if ((int)in.size() != w * h) return;

        if (r <= 0) {
            out = in;
            return;
        }

        auto idx = [&](int x, int y) -> size_t { return (size_t)y * (size_t)w + (size_t)x; };

        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                if (in[idx(x, y)] == 0) continue;

                bool ok = true;
                for (int dy = -r; dy <= r && ok; ++dy) {
                    int yy = y + dy;
                    if (yy < 0 || yy >= h) { ok = false; break; }
                    for (int dx = -r; dx <= r; ++dx) {
                        int xx = x + dx;
                        if (xx < 0 || xx >= w) { ok = false; break; }
                        if (in[idx(xx, yy)] == 0) { ok = false; break; }
                    }
                }
                out[idx(x, y)] = ok ? 1 : 0;
            }
        }
    }

    // 单帧雕刻（点云版）：
    // - 帧.点云：与帧宽高一致的相机坐标点云（单位 mm）
    // - roi：来自轮廓观测（x,y,w,h + 掩膜(0/1)）
    // - basis：存在本体基（Cam->Obj）
    static inline void 雕刻_用一帧_点云ROI(
        三维模型类::体素立方体4& S,
        const 结构体_原始场景帧& 帧,
        const 结构体_轮廓观测& roi,
        const 三维模型类::存在本体基& basis,
        const 结构体_雕刻参数& p = {})
    {
        const int W = 帧.宽度;
        const int H = 帧.高度;
        if (W <= 0 || H <= 0) return;

        const size_t N = (size_t)W * (size_t)H;
        if (帧.点云.size() != N) return;

        const int rx = roi.x;
        const int ry = roi.y;
        const int rw = roi.w;
        const int rh = roi.h;
        if (rw <= 0 || rh <= 0) return;
        if ((int)roi.掩膜.size() != rw * rh) return;

        // 动态计算表面留空厚度：根据体素大小和深度变化特性
        double eps = p.eps_mm;
        if (eps <= 0.0) {
            // 质量优先：默认使用1.5倍体素大小，增加稳定性
            eps = std::max(0.5, 1.5 * S.s_mm);
        }

        std::vector<std::uint8_t> mask_use;
        if (p.内缩像素 > 0) ROI腐蚀(roi.掩膜, rw, rh, p.内缩像素, mask_use);
        else mask_use = roi.掩膜;

        const Vector3D O_obj = 三维模型类::CamOriginToObj(basis);

        auto ridx = [&](int u, int v) -> size_t { return (size_t)v * (size_t)rw + (size_t)u; };
        auto gidx = [&](int u, int v) -> size_t { return (size_t)v * (size_t)W + (size_t)u; };
        
        // 检测边界区域：找到掩膜的边缘像素
        std::vector<std::uint8_t> is_boundary((size_t)rw * (size_t)rh, 0);
        for (int v = 0; v < rh; ++v) {
            for (int u = 0; u < rw; ++u) {
                if (mask_use[ridx(u, v)] == 0) continue;
                
                bool is_edge = false;
                for (int dv = -1; dv <= 1 && !is_edge; ++dv) {
                    for (int du = -1; du <= 1 && !is_edge; ++du) {
                        if (dv == 0 && du == 0) continue;
                        int nu = u + du;
                        int nv = v + dv;
                        if (nu >= 0 && nu < rw && nv >= 0 && nv < rh) {
                            if (mask_use[ridx(nu, nv)] == 0) {
                                is_edge = true;
                            }
                        } else {
                            is_edge = true;
                        }
                    }
                }
                
                if (is_edge) {
                    is_boundary[ridx(u, v)] = 1;
                }
            }
        }
        
        // 核心雕刻逻辑：对不同区域使用不同采样策略
        for (int vv = 0; vv < rh; ++vv) {
            const int V = ry + vv;
            if (V < 0 || V >= H) continue;

            for (int uu = 0; uu < rw; ++uu) {
                if (mask_use[ridx(uu, vv)] == 0) continue;

                // 判断是否为边界区域
                bool is_edge_pixel = (is_boundary[ridx(uu, vv)] == 1);
                
                // 边界区域使用更高采样密度（步长1），内部区域使用默认采样密度
                int current_stride = p.自适应采样 ? (is_edge_pixel ? 1 : p.最大采样步长) : p.最小采样步长;
                
                // 确保当前像素在采样网格上
                if ((uu % current_stride != 0) || (vv % current_stride != 0)) continue;
                
                const int U = rx + uu;
                if (U < 0 || U >= W) continue;

                const Vector3D& P_cam = 帧.点云[gidx(U, V)];
                if (!点云有效(P_cam)) continue;

                const Vector3D P_obj = 三维模型类::CamToObj(basis, P_cam);
                三维模型类::雕刻_单射线(S, O_obj, P_obj, eps);
                
                // 边界区域额外采样：对边界像素周围的几个点也进行雕刻，提高边界精度
                if (is_edge_pixel && p.自适应采样) {
                    static const int edge_extension = 1;
                    for (int dv = -edge_extension; dv <= edge_extension; ++dv) {
                        for (int du = -edge_extension; du <= edge_extension; ++du) {
                            if (dv == 0 && du == 0) continue;
                            
                            int nu = uu + du;
                            int nv = vv + dv;
                            
                            if (nu >= 0 && nu < rw && nv >= 0 && nv < rh) {
                                if (mask_use[ridx(nu, nv)] == 1) {
                                    int nU = rx + nu;
                                    int nV = ry + nv;
                                    
                                    if (nU >= 0 && nU < W && nV >= 0 && nV < H) {
                                        const Vector3D& nP_cam = 帧.点云[gidx(nU, nV)];
                                        if (点云有效(nP_cam)) {
                                            const Vector3D nP_obj = 三维模型类::CamToObj(basis, nP_cam);
                                            三维模型类::雕刻_单射线(S, O_obj, nP_obj, eps);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

};