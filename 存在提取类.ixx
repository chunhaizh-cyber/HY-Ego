module;

#include <unordered_map>
#include <vector>
#include <cstdint>
#include <cmath>
#include <algorithm>
#include <bit>
#include <mutex>

export module 存在提取模块;

import 基础数据类型模块;
import 点簇分割模块;
import 子存在提取模块;

import <queue>;

// 本文件是“精简版”。
// 目标：
//  1) 只做确定性的数据搬运：点簇增强结果/轮廓观测 -> 存在观测
//  2) 不在此处生成三维模型/雕刻（交给 三维雕刻重建类 + 三维雕刻写入线程）
//  3) 不保留未使用接口、不引入实验性/尝试性逻辑

export class 存在提取类
{
private:
    struct _Track {
        std::int64_t id = -1;
        Vector3D     center{};
        Vector3D     size{};
        时间戳       last = 0;
        int          miss = 0;
    };

    std::int64_t _nextId = 0;
    std::vector<_Track> _tracks;

    // ===================== 父点簇增强缓存（折中方案：按需生成子存在） =====================
    // 目的：不把大数据写进世界树；只在需要生成子存在/重雕刻时，从这里取回“最近一帧”的父点簇增强结果。
    // Key：观测.跟踪ID（由 分配跟踪ID 生成）
    struct _父增强缓存项 {
        点簇增强结果 父{};
        时间戳 ts = 0;
    };

    static inline std::mutex g_父增强缓存_mtx{};
    static inline std::unordered_map<std::int64_t, _父增强缓存项> g_父增强缓存{};
    static inline std::size_t g_父增强缓存_max = 128;

    static inline void _父增强缓存_清理_已加锁() {
        if (g_父增强缓存.size() <= g_父增强缓存_max) return;
        // 简单淘汰：按时间戳从旧到新删一批（避免维护复杂 LRU 结构）
        // 每次超限删除约 1/4，摊销成本低。
        const std::size_t target = g_父增强缓存_max - g_父增强缓存_max / 4;
        while (g_父增强缓存.size() > target) {
            auto oldest = g_父增强缓存.begin();
            for (auto it = g_父增强缓存.begin(); it != g_父增强缓存.end(); ++it) {
                if (it->second.ts < oldest->second.ts) oldest = it;
            }
            g_父增强缓存.erase(oldest);
        }
    }

    static inline void _写父增强缓存_已加锁(std::int64_t trackId, const 点簇增强结果& src, 时间戳 ts) {
        if (trackId < 0) return;
        auto& e = g_父增强缓存[trackId];
        e.父 = src; // 复制一份（折中方案：避免跨线程悬空引用；如需更省内存可改 shared_ptr）
        e.ts = ts;
        _父增强缓存_清理_已加锁();
    }


    static inline bool 点云有效(const Vector3D& P) noexcept {
        return std::isfinite(P.x) && std::isfinite(P.y) && std::isfinite(P.z) && P.z > 0.0;
    }

    // ===================== 标准化 2D 轮廓：ROI 二值图 -> 标准正方形(N=8<<k)，并可同步标准化颜色 =====================
    // 规则（你的口径）：
    //  - 在 ROI 掩膜内找“有效像素”的紧包围盒 bbox
    //  - 若 bbox 贴到 ROI 的某一侧（xmin==0 / xmax==w-1 ...），认为该侧是轮廓边界，在该侧外侧额外留 1 像素空白
    //  - 形成最小包含正方形 side0，再缩放到标准边长 sideStd=8<<N（N 取更保守的 ceil）
    static inline void 标准化ROI_轮廓二值与颜色(
        const 结构体_轮廓观测& in, int sideStd, std::vector<std::uint8_t>& stdMask, std::vector<std::uint8_t>& stdBGR)
    {
        stdMask.assign((std::size_t)sideStd * (std::size_t)sideStd, 0);
        stdBGR.clear();

        if (sideStd <= 0) return;
        if (in.w <= 0 || in.h <= 0) return;
        if ((int)in.掩膜.size() != in.w * in.h) return;

        const bool hasColor = ((int)in.裁剪颜色.size() == in.w * in.h);

        // 1) bbox（紧包围）
        int xmin = in.w, ymin = in.h, xmax = -1, ymax = -1;
        for (int y = 0; y < in.h; ++y) {
            for (int x = 0; x < in.w; ++x) {
                if (in.掩膜[(std::size_t)y * (std::size_t)in.w + (std::size_t)x] == 0) continue;
                xmin = std::min(xmin, x); ymin = std::min(ymin, y);
                xmax = std::max(xmax, x); ymax = std::max(ymax, y);
            }
        }
        if (xmax < xmin || ymax < ymin) return;

        const int bw = xmax - xmin + 1;
        const int bh = ymax - ymin + 1;

        // 2) 贴边侧留 1px
        const int padL = (xmin == 0) ? 1 : 0;
        const int padR = (xmax == in.w - 1) ? 1 : 0;
        const int padT = (ymin == 0) ? 1 : 0;
        const int padB = (ymax == in.h - 1) ? 1 : 0;

        const int side0 = std::max(bw + padL + padR, bh + padT + padB);

        // 3) 生成 side0×side0 的 srcSquare（掩膜+可选颜色）
        std::vector<std::uint8_t> srcMask((std::size_t)side0 * (std::size_t)side0, 0);
        std::vector<Color>        srcColor;
        if (hasColor) srcColor.assign((std::size_t)side0 * (std::size_t)side0, Color{ 0,0,0 });

        const int placeW = bw + padL + padR;
        const int placeH = bh + padT + padB;
        const int offX = (side0 - placeW) / 2 + padL;
        const int offY = (side0 - placeH) / 2 + padT;

        auto srcIdx = [&](int x, int y)->std::size_t { return (std::size_t)y * (std::size_t)side0 + (std::size_t)x; };
        auto inIdx = [&](int x, int y)->std::size_t { return (std::size_t)y * (std::size_t)in.w + (std::size_t)x; };

        for (int y = 0; y < bh; ++y) {
            for (int x = 0; x < bw; ++x) {
                const int sx = xmin + x;
                const int sy = ymin + y;
                if (in.掩膜[inIdx(sx, sy)] == 0) continue;

                const int dx = offX + x;
                const int dy = offY + y;
                if ((unsigned)dx >= (unsigned)side0 || (unsigned)dy >= (unsigned)side0) continue;

                srcMask[srcIdx(dx, dy)] = 1;
                if (hasColor) srcColor[srcIdx(dx, dy)] = in.裁剪颜色[inIdx(sx, sy)];
            }
        }

        // 4) 缩放到 sideStd（双线性插值抗锯齿）
        auto dstIdx = [&](int x, int y)->std::size_t { return (std::size_t)y * (std::size_t)sideStd + (std::size_t)x; };
        
        for (int y = 0; y < sideStd; ++y) {
            // 计算源坐标（带小数部分）
            double sy_double = ((double)y * (double)side0) / (double)sideStd;
            int sy = (int)sy_double;
            double dy = sy_double - sy;
            
            // 确保源坐标在有效范围内
            int sy0 = std::clamp(sy, 0, side0 - 1);
            int sy1 = std::clamp(sy + 1, 0, side0 - 1);
            
            for (int x = 0; x < sideStd; ++x) {
                double sx_double = ((double)x * (double)side0) / (double)sideStd;
                int sx = (int)sx_double;
                double dx = sx_double - sx;
                
                int sx0 = std::clamp(sx, 0, side0 - 1);
                int sx1 = std::clamp(sx + 1, 0, side0 - 1);
                
                // 双线性插值
                double v00 = srcMask[srcIdx(sx0, sy0)];
                double v01 = srcMask[srcIdx(sx1, sy0)];
                double v10 = srcMask[srcIdx(sx0, sy1)];
                double v11 = srcMask[srcIdx(sx1, sy1)];
                
                double v0 = v00 * (1.0 - dx) + v01 * dx;
                double v1 = v10 * (1.0 - dx) + v11 * dx;
                double v = v0 * (1.0 - dy) + v1 * dy;
                
                // 阈值处理，增强边界清晰度
                stdMask[dstIdx(x, y)] = (v > 0.5) ? 1 : 0;
            }
        }

        if (hasColor) {
            stdBGR.assign((std::size_t)sideStd * (std::size_t)sideStd * 3ull, 0);
            for (int y = 0; y < sideStd; ++y) {
                const int sy = (int)((std::int64_t)y * (std::int64_t)side0 / (std::int64_t)sideStd);
                for (int x = 0; x < sideStd; ++x) {
                    const int sx = (int)((std::int64_t)x * (std::int64_t)side0 / (std::int64_t)sideStd);
                    if (srcMask[srcIdx(sx, sy)] == 0) continue;
                    const Color c = srcColor[srcIdx(sx, sy)];
                    const std::size_t di = ((std::size_t)y * (std::size_t)sideStd + (std::size_t)x) * 3ull;
                    stdBGR[di + 0] = c.b;
                    stdBGR[di + 1] = c.g;
                    stdBGR[di + 2] = c.r;
                }
            }
        }
    }

    // 生成彩色截图（方案1：在存在观测构建阶段生成）
    static inline void 生成彩色截图(const 点簇增强结果& in, 结构体_存在观测& out) {
        // 1. 从裁剪颜色和裁剪掩码中生成彩色截图
        const int 截图宽度 = in.裁剪宽;
        const int 截图高度 = in.裁剪高;
        
        // 检查必要数据是否存在
        if (截图宽度 <= 0 || 截图高度 <= 0) return;
        if (in.裁剪颜色.size() != (size_t)截图宽度 * 截图高度) return;
        if (in.裁剪掩码.size() != (size_t)截图宽度 * 截图高度) return;
        
        // 2. 创建RGBA格式的截图（带alpha通道）
        std::vector<std::uint8_t> 彩色截图_RGBA((size_t)截图宽度 * (size_t)截图高度 * 4);
        
        for (int vv = 0; vv < 截图高度; ++vv) {
            for (int uu = 0; uu < 截图宽度; ++uu) {
                size_t index = (size_t)vv * (size_t)截图宽度 + (size_t)uu;
                if (in.裁剪掩码[index] != 0) {
                    // 前景像素：使用原始颜色
                    const Color& color = in.裁剪颜色[index];
                    彩色截图_RGBA[index * 4 + 0] = color.r;
                    彩色截图_RGBA[index * 4 + 1] = color.g;
                    彩色截图_RGBA[index * 4 + 2] = color.b;
                    彩色截图_RGBA[index * 4 + 3] = 255; // 不透明
                } else {
                    // 背景像素：透明
                    彩色截图_RGBA[index * 4 + 0] = 0;
                    彩色截图_RGBA[index * 4 + 1] = 0;
                    彩色截图_RGBA[index * 4 + 2] = 0;
                    彩色截图_RGBA[index * 4 + 3] = 0; // 透明
                }
            }
        }
        
        // 3. 存储彩色截图和元信息到存在观测
        // 注意：这里我们暂时使用未压缩的RGBA数据，实际项目中可以添加压缩逻辑
        out.彩色截图 = std::move(彩色截图_RGBA);
        out.彩色截图元信息 = { 截图宽度, 截图高度, 1, 0, 1, 0 }; // [宽度, 高度, 格式(RGBA8), 步长(0=连续), 有alpha, 未压缩]
    }
    
    // N×N 二值图（N 必须是 8 的倍数）打包为 8×8 Bits64 块
    static inline void 打包二值图_8x8块(const std::vector<std::uint8_t>& maskN, int N, std::vector<U64>& outBlocks)
    {
        outBlocks.clear();
        if (N <= 0 || (N % 8) != 0) return;
        if ((int)maskN.size() != N * N) return;

        const int BS = N / 8;
        outBlocks.assign((std::size_t)BS * (std::size_t)BS, (U64)0);

        auto idx = [&](int x, int y)->std::size_t { return (std::size_t)y * (std::size_t)N + (std::size_t)x; };
        for (int by = 0; by < BS; ++by) {
            for (int bx = 0; bx < BS; ++bx) {
                std::uint64_t bits = 0ull;
                for (int ly = 0; ly < 8; ++ly) {
                    for (int lx = 0; lx < 8; ++lx) {
                        const int x = bx * 8 + lx;
                        const int y = by * 8 + ly;
                        if (maskN[idx(x, y)]) bits |= (1ull << (ly * 8 + lx));
                    }
                }
                outBlocks[(std::size_t)by * (std::size_t)BS + (std::size_t)bx] = (U64)bits;
            }
        }
    }

    // 从轮廓像素尺寸推导 N，使 sideStd=8<<N，偏向“更保守的 ceil”
    static inline int 推导压缩层级N_从轮廓像素尺寸(int w, int h, int base = 8, int Nmax = 12) noexcept
    {
        if (w <= 0 || h <= 0 || base <= 0 || Nmax < 0) return 0;
        const int side = std::max(w, h);
        if (side <= base) return 0;

        const int ratio = side / base;
        int Nfloor = (ratio > 0) ? (std::bit_width((unsigned)ratio) - 1) : 0;
        Nfloor = std::clamp(Nfloor, 0, Nmax);

        const std::int64_t lower = (std::int64_t)base << Nfloor;
        if (lower >= side) return Nfloor;

        const int Nceil = std::min(Nfloor + 1, Nmax);
        const std::int64_t upper = (std::int64_t)base << Nceil;
        const std::int64_t dLow = (std::int64_t)side - lower;
        const std::int64_t dUp = upper - (std::int64_t)side;
        return (dUp <= dLow) ? Nceil : Nfloor;
    }

    void 分配跟踪ID(std::vector<结构体_存在观测>& obsList, double fixed_gate_mm, int maxMiss)
    {
        std::vector<char> matched(_tracks.size(), 0);

        auto dist2 = [](const Vector3D& a, const Vector3D& b) {
            const double dx = a.x - b.x, dy = a.y - b.y, dz = a.z - b.z;
            return dx * dx + dy * dy + dz * dz;
            };

        auto max3 = [](double a, double b, double c) {
            return std::max(a, std::max(b, c));
            };

        auto gate_from_size = [&](const Vector3D& s) {
            const double md = max3(std::abs(s.x), std::abs(s.y), std::abs(s.z));
            // 自适应门限（单位mm）：大物体允许更大漂移，小物体更严格
            return std::clamp(0.35 * md + 120.0, 150.0, 1200.0);
            };

        const auto now = 结构体_时间戳::当前_微秒(); // 如果你想更严谨，可改用 ob.时间（下面备注）

        for (auto& ob : obsList)
        {
            int best = -1;
            double bestD2 = 0.0;

            // 1) 计算本观测 gate2（固定 gate 或自适应 gate）
            double gate_mm = fixed_gate_mm;
            if (gate_mm <= 0.0) {
                // 用 ob 与 track 的尺度共同决定门限，避免大物体断轨、小物体串轨
                // 先用 ob 尺度估一个基准
                gate_mm = gate_from_size(ob.尺寸);
            }
            double gate2 = gate_mm * gate_mm;

            // 2) 在未匹配 track 里找最近的
            bestD2 = gate2;
            for (int i = 0; i < (int)_tracks.size(); ++i) {
                if (matched[i]) continue;

                // 若你希望更稳，可以让 gate 同时参考 track.size（尤其 track 已稳定时）
                if (fixed_gate_mm <= 0.0) {
                    const double gate_track = gate_from_size(_tracks[i].size);
                    const double gate_use = std::max(gate_mm, gate_track);
                    gate2 = gate_use * gate_use;
                    if (gate2 < bestD2) bestD2 = gate2; // 收紧 bestD2 逻辑：保证比较基准是当前 gate2
                }

                const double d2 = dist2(ob.中心坐标, _tracks[i].center);
                if (d2 < bestD2) { bestD2 = d2; best = i; }
            }

            if (best >= 0) {
                ob.跟踪ID = _tracks[best].id;
                matched[best] = 1;

                _tracks[best].center = ob.中心坐标;
                _tracks[best].size = ob.尺寸;
                _tracks[best].last = now;
                _tracks[best].miss = 0;
            }
            else {
                _Track t;
                t.id = _nextId++;
                t.center = ob.中心坐标;
                t.size = ob.尺寸;
                t.last = now;
                t.miss = 0;
                ob.跟踪ID = t.id;

                _tracks.push_back(t);
                matched.push_back(1);
            }
        }

        for (int i = 0; i < (int)_tracks.size(); ++i) {
            if (!matched[i]) _tracks[i].miss++;
        }

        _tracks.erase(
            std::remove_if(_tracks.begin(), _tracks.end(),
                [&](const _Track& t) { return t.miss > maxMiss; }),
            _tracks.end());
    }


public:
    // ================ 父点簇增强缓存（供三维场景管理类按需取回） ================
    static void 设置父增强缓存上限(std::size_t n) {
        std::lock_guard<std::mutex> lk(g_父增强缓存_mtx);
        g_父增强缓存_max = std::max<std::size_t>(8, n);
        _父增强缓存_清理_已加锁();
    }

    static void 清空父点簇增强缓存() {
        std::lock_guard<std::mutex> lk(g_父增强缓存_mtx);
        g_父增强缓存.clear();
    }

    static bool 尝试获取父点簇增强缓存(std::int64_t trackId, 点簇增强结果& out, 时间戳* outTs = nullptr) {
        std::lock_guard<std::mutex> lk(g_父增强缓存_mtx);
        auto it = g_父增强缓存.find(trackId);
        if (it == g_父增强缓存.end()) return false;
        out = it->second.父;
        if (outTs) *outTs = it->second.ts;
        return true;
    }

    // 1) 点簇增强结果 -> 存在观测（推荐路径）
    bool 从点簇增强提取存在观测(const 点簇增强结果& in, 结构体_存在观测& out, const 观测提取参数& p = {}, std::shared_ptr<const 结构体_原始场景帧> 原始场景帧 = nullptr)
    {
        const std::size_t effPts = (in.有效点数 > 0 ? in.有效点数
            : (!in.点云簇.empty() ? in.点云簇.size() : in.点云簇.size()));

        if (effPts < p.最小有效点数) return false;
        if (in.中心.z < p.最小中心Z) return false;
        if (in.尺寸.x < p.最小尺寸 || in.尺寸.y < p.最小尺寸 || in.尺寸.z < p.最小尺寸) return false;

        out = {};
        out.时间 = 结构体_时间戳::当前_微秒();
        out.坐标系 = 枚举_坐标系::相机坐标系;

        out.中心坐标 = in.中心;
        out.坐标有效 = true;
        out.尺寸 = in.尺寸;
        out.重新计算距离();

        out.主方向1 = in.主方向1;
        out.主方向2 = in.主方向2;
        out.主方向3 = in.主方向3;

        // AABB
        {
            const auto& pts = (!in.点云簇.empty() ? in.点云簇 : in.点云簇);
            if (!pts.empty()) {
                Vector3D mn = pts[0], mx = pts[0];
                for (const auto& P : pts) {
                    if (!点云有效(P)) continue;
                    mn.x = std::min(mn.x, P.x); mn.y = std::min(mn.y, P.y); mn.z = std::min(mn.z, P.z);
                    mx.x = std::max(mx.x, P.x); mx.y = std::max(mx.y, P.y); mx.z = std::max(mx.z, P.z);
                }
                out.AABB_min = mn;
                out.AABB_max = mx;
            }
        }

        out.点云簇 = in.点云簇;
        out.平均颜色 = in.颜色;
        out.颜色一致性 = (float)in.颜色一致性;
        out.有效点数 = (std::uint32_t)effPts;
        out.深度噪声_MAD = (float)in.深度噪声_MAD;
        out.PCA残差 = (float)in.PCA残差;

        // 质量分：若上游没填，就给 1.0
        out.质量分 = (in.质量分 > 0.0 ? in.质量分 : 1.0);
        
        // 生成彩色截图（方案1：在存在观测构建阶段生成）
        生成彩色截图(in, out);

        // ROI
        out.ROI.x = in.边界.umin;
        out.ROI.y = in.边界.vmin;
        out.ROI.w = in.边界.umax - in.边界.umin + 1;
        out.ROI.h = in.边界.vmax - in.边界.vmin + 1;

        // 裁剪缓冲（原始 ROI）
        if (!in.裁剪颜色.empty() && in.裁剪宽 > 0 && in.裁剪高 > 0) {
            out.裁剪BGR.w = in.裁剪宽;
            out.裁剪BGR.h = in.裁剪高;
            out.裁剪BGR.c = 3;
            out.裁剪BGR.data.resize(in.裁剪颜色.size() * 3ull);
            for (std::size_t i = 0; i < in.裁剪颜色.size(); ++i) {
                out.裁剪BGR.data[i * 3 + 0] = in.裁剪颜色[i].b;
                out.裁剪BGR.data[i * 3 + 1] = in.裁剪颜色[i].g;
                out.裁剪BGR.data[i * 3 + 2] = in.裁剪颜色[i].r;
            }
            out.缓冲掩码 = (枚举_存在观测缓冲)((std::uint32_t)out.缓冲掩码 | (std::uint32_t)枚举_存在观测缓冲::裁剪BGR);
        }

        if (!in.裁剪掩码.empty() && in.裁剪宽 > 0 && in.裁剪高 > 0) {
            out.裁剪掩膜.w = in.裁剪宽;
            out.裁剪掩膜.h = in.裁剪高;
            out.裁剪掩膜.c = 1;
            out.裁剪掩膜.data = in.裁剪掩码;
            out.缓冲掩码 = (枚举_存在观测缓冲)((std::uint32_t)out.缓冲掩码 | (std::uint32_t)枚举_存在观测缓冲::裁剪掩膜);
        }

        // 轮廓基层
        out.轮廓编码 = in.轮廓编码;
        out.轮廓3D = in.轮廓3D;

        out.边界点序列.clear();
        out.边界点序列.reserve(in.边界点序列.size());
        for (auto v : in.边界点序列) out.边界点序列.push_back((std::uint64_t)v);

        out.轮廓尺度N = in.轮廓尺度N;
        out.轮廓标准边长 = in.轮廓标准边长;
        out.尺寸 = in.轮廓尺寸;

        out.轮廓最大N = out.轮廓尺度N;
        out.轮廓最大边长 = out.轮廓标准边长;
        if (p.要求轮廓编码 && out.轮廓编码.empty()) return false;
        if (p.要求颜色 && (out.裁剪BGR.data.empty() && out.平均颜色.r == 0 && out.平均颜色.g == 0 && out.平均颜色.b == 0)) {
            // 这里不强行依赖裁剪图；如果你只要求“平均颜色”，那上游应已填
            // 但考虑到默认平均颜色可能是 255,255,255，这里不做更复杂判断。
        }

        // 设置轮廓观测有效为true，因为所有必要条件都已满足
        out.轮廓观测有效 = true;

        // 设置原始场景帧
        out.原始场景帧 = 原始场景帧;

        return true;
    }

    // 2) 批量提取
    std::vector<结构体_存在观测> 从点簇增强列表提取观测(const std::vector<点簇增强结果>& 列表, const 观测提取参数& p = {}, std::shared_ptr<const 结构体_原始场景帧> 原始场景帧 = nullptr)
    {
        std::vector<结构体_存在观测> out;
        out.reserve(列表.size());

        // 与 out 同步的“来源点簇增强结果”指针，用于在分配跟踪ID后回填缓存
        std::vector<const 点簇增强结果*> srcPtr;
        srcPtr.reserve(列表.size());

        for (const auto& e : 列表) {
            结构体_存在观测 obs;
            if (从点簇增强提取存在观测(e, obs, p, 原始场景帧)) {
                out.push_back(std::move(obs));
                srcPtr.push_back(&e);
            }
        }

        // 统一分配跨帧跟踪ID
        分配跟踪ID(out, /*gate_mm=*/180.0, /*maxMiss=*/10);

        // 写入父增强缓存：trackId -> 最近一帧父点簇增强
        {
            std::lock_guard<std::mutex> lk(g_父增强缓存_mtx);
            for (std::size_t i = 0; i < out.size() && i < srcPtr.size(); ++i) {
                const auto tid = out[i].跟踪ID;
                const auto ts = out[i].时间;
                if (srcPtr[i]) _写父增强缓存_已加锁(tid, *srcPtr[i], ts);
            }
        }

        return out;
    }


    // 3) 子存在提取（流程拼装，不做额外尝试）
    std::vector<结构体_存在观测> 从父点簇增强提取子存在观测(
        点簇增强结果& 父, const 子存在提取参数& sp, const 观测提取参数& p = {}, std::shared_ptr<const 结构体_原始场景帧> 原始场景帧 = nullptr)
    {
        子存在提取类 splitter;
        auto kids = splitter.从父点簇增强提取子点簇增强(父, sp);

        std::vector<结构体_存在观测> out;
        out.reserve(kids.size());

        for (const auto& k : kids) {
            结构体_存在观测 obs;
            if (从点簇增强提取存在观测(k, obs, p, 原始场景帧)) out.push_back(std::move(obs));
        }
        return out;
    }

    // 4) 轮廓观测 -> 存在观测（用于“轮廓优先”的入口）
    bool 从轮廓观测提取存在观测(const 结构体_轮廓观测& in, 结构体_存在观测& out, const 观测提取参数& p = {})
    {
        if (in.w <= 0 || in.h <= 0) return false;
        if (in.中心.z < p.最小中心Z) return false;
        if (in.尺寸.x < p.最小尺寸 || in.尺寸.y < p.最小尺寸 || in.尺寸.z < p.最小尺寸) return false;

        out = {};
        out.时间 = 结构体_时间戳::当前_微秒();
        out.坐标系 = 枚举_坐标系::相机坐标系;

        out.中心坐标 = in.中心;
        out.坐标有效 = true;
        out.尺寸 = in.尺寸;
        out.重新计算距离();

        out.ROI.x = in.x;
        out.ROI.y = in.y;
        out.ROI.w = in.w;
        out.ROI.h = in.h;

        out.平均颜色.b = (std::uint8_t)in.平均颜色.b;
        out.平均颜色.g = (std::uint8_t)in.平均颜色.g;
        out.平均颜色.r = (std::uint8_t)in.平均颜色.r;

        // 点云簇（可选）
        out.点云簇 = in.点云簇;
        out.有效点数 = (std::uint32_t)out.点云簇.size();

        // 若没有掩膜，仍可输出几何（用于跟踪）；但无法生成标准轮廓编码
        if (in.掩膜.empty() || (int)in.掩膜.size() != in.w * in.h) {
            return !p.要求轮廓编码;
        }

        // 1) bbox + 贴边留 1px，推导标准边长
        int xmin = in.w, ymin = in.h, xmax = -1, ymax = -1;
        for (int y = 0; y < in.h; ++y) {
            for (int x = 0; x < in.w; ++x) {
                if (in.掩膜[(std::size_t)y * (std::size_t)in.w + (std::size_t)x] == 0) continue;
                xmin = std::min(xmin, x); ymin = std::min(ymin, y);
                xmax = std::max(xmax, x); ymax = std::max(ymax, y);
            }
        }
        if (xmax < xmin || ymax < ymin) return !p.要求轮廓编码;

        const int bw = xmax - xmin + 1;
        const int bh = ymax - ymin + 1;

        const int padL = (xmin == 0) ? 1 : 0;
        const int padR = (xmax == in.w - 1) ? 1 : 0;
        const int padT = (ymin == 0) ? 1 : 0;
        const int padB = (ymax == in.h - 1) ? 1 : 0;

        const int side0 = std::max(bw + padL + padR, bh + padT + padB);

        const int base = 8;
        const int Nmax = 10;
        const int N = 推导压缩层级N_从轮廓像素尺寸(side0, side0, base, Nmax);
        const int sideStd = base << N;

        out.轮廓尺度N = N;
        out.轮廓标准边长 = sideStd;

        // 2) 标准化二值图 + 标准化颜色（可选）
        std::vector<std::uint8_t> maskStd;
        std::vector<std::uint8_t> bgrStd;
        标准化ROI_轮廓二值与颜色(in, sideStd, maskStd, bgrStd);

        // 3) 写入标准裁剪缓冲（现在裁剪缓冲代表“标准数据”）
        out.裁剪掩膜.w = sideStd;
        out.裁剪掩膜.h = sideStd;
        out.裁剪掩膜.c = 1;
        out.裁剪掩膜.data = maskStd;
        out.缓冲掩码 = (枚举_存在观测缓冲)((std::uint32_t)out.缓冲掩码 | (std::uint32_t)枚举_存在观测缓冲::裁剪掩膜);

        if (!bgrStd.empty()) {
            out.裁剪BGR.w = sideStd;
            out.裁剪BGR.h = sideStd;
            out.裁剪BGR.c = 3;
            out.裁剪BGR.data = std::move(bgrStd);
            out.缓冲掩码 = (枚举_存在观测缓冲)((std::uint32_t)out.缓冲掩码 | (std::uint32_t)枚举_存在观测缓冲::裁剪BGR);
        }

        // 4) 标准二值图 -> 8×8 Bits64 块
        out.轮廓编码.clear();
        打包二值图_8x8块(maskStd, sideStd, out.轮廓编码);

        return !(p.要求轮廓编码 && out.轮廓编码.empty());
    }

    // 5) 统一输出：obs 与 roi 严格 1:1 对齐（用于下游雕刻线程）
    bool 从点簇增强列表提取观测与轮廓观测(
        const std::vector<点簇增强结果>& 列表, std::vector<结构体_存在观测>& outObs, std::vector<结构体_轮廓观测>& outRoi, const 观测提取参数& p = {}, std::shared_ptr<const 结构体_原始场景帧> 原始场景帧 = nullptr)
    {
        outObs.clear();
        outRoi.clear();
        outObs.reserve(列表.size());
        outRoi.reserve(列表.size());

        int seq = 0;
        for (const auto& r : 列表)
        {
            结构体_存在观测 obs{};
            if (!从点簇增强提取存在观测(r, obs, p, 原始场景帧)) continue;

            结构体_轮廓观测 roi{};
            roi.帧内序号 = seq++;

            // 轮廓观测的 ROI：优先使用“裁剪窗口”（bbox+边距），确保掩膜/裁剪颜色能正确映射回全图。
            // 若未输出裁剪窗口，则退化为 bbox。
            if (r.裁剪宽 > 0 && r.裁剪高 > 0) {
                roi.x = r.裁剪x;
                roi.y = r.裁剪y;
                roi.w = r.裁剪宽;
                roi.h = r.裁剪高;
            }
            else {
                roi.x = r.边界.umin;
                roi.y = r.边界.vmin;
                roi.w = r.边界.umax - r.边界.umin + 1;
                roi.h = r.边界.vmax - r.边界.vmin + 1;
            }

            roi.中心 = r.中心;
            roi.尺寸 = r.尺寸;
            roi.平均颜色.r = r.颜色.r;
            roi.平均颜色.g = r.颜色.g;
            roi.平均颜色.b = r.颜色.b;

            const std::size_t roiN = (std::size_t)std::max(0, roi.w) * (std::size_t)std::max(0, roi.h);

            // 裁剪颜色：当上游提供了裁剪窗口时，直接复用（尺寸应一致）；否则仅在尺寸匹配时复用。
            if (!r.裁剪颜色.empty() && r.裁剪宽 > 0 && r.裁剪高 > 0 && r.裁剪颜色.size() == (std::size_t)r.裁剪宽 * (std::size_t)r.裁剪高) {
                if (r.裁剪宽 == roi.w && r.裁剪高 == roi.h) {
                    roi.裁剪颜色 = r.裁剪颜色;
                }
            }

            // 点云簇
            roi.点云簇 = r.点云簇;

            // 掩膜：若上游输出了裁剪掩码，则直接复用（它与裁剪窗口一一对应）。
            roi.掩膜.clear();
            roi.像素数 = 0;
            if (!r.裁剪掩码.empty() && r.裁剪宽 > 0 && r.裁剪高 > 0 && r.裁剪掩码.size() == (std::size_t)r.裁剪宽 * (std::size_t)r.裁剪高) {
                if (r.裁剪宽 == roi.w && r.裁剪高 == roi.h) {
                    roi.掩膜 = r.裁剪掩码;
                    for (auto v : roi.掩膜) roi.像素数 += (v != 0);
                }
            }

            outObs.push_back(std::move(obs));
            outRoi.push_back(std::move(roi));
        }

        分配跟踪ID(outObs, /*gate_mm=*/180.0, /*maxMiss=*/10);
        for (std::size_t i = 0; i < outObs.size() && i < outRoi.size(); ++i) {
            outRoi[i].跟踪ID = outObs[i].跟踪ID;
        }
        return !outObs.empty();
    }
};

