// 点簇分割模块.ixx（去 OpenCV 版本，基于 结构体_原始场景帧）
export module 点簇分割模块;

import 基础数据类型模块;

import <vector>;
import <cstdint>;
import <cmath>;
import <algorithm>;
import <limits>;

export class 点簇分割类
{
private:
    static inline int 索引(int u, int v, int w) { return v * w + u; }

    static inline bool 深度有效(double z, const 点簇分割参数& p)
    {
        if (p.忽略无效点 && (!std::isfinite(z) || z <= 0.0)) return false;
        if (p.最小深度 > 0.0 && z < p.最小深度) return false;
        if (p.最大深度 > 0.0 && z > p.最大深度) return false;
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
        if (std::abs(za - zb) > p.邻域最大深度差) return false;

        // 2) 点云欧氏距离门槛（点云可用时更稳）
        if (p.邻域最大三维距离 > 0.0 && 帧.有效点云()) {
            const int ua = idxA % w, va = idxA / w;
            const int ub = idxB % w, vb = idxB / w;

            Vector3D Pa{}, Pb{};
            if (取点云(帧, ua, va, Pa) && 取点云(帧, ub, vb, Pb)) {
                const double dx = Pa.x - Pb.x;
                const double dy = Pa.y - Pb.y;
                const double dz = Pa.z - Pb.z;
                const double dist = std::sqrt(dx * dx + dy * dy + dz * dz);
                if (dist > p.邻域最大三维距离) return false;
            }
        }

        return true;
    }

    static void 计算簇3D信息(const 结构体_原始场景帧& 帧, const 点簇& 簇, Vector3D& 中心, Vector3D& 尺寸)
    {
        if (!帧.有效点云() || 簇.empty()) {
            中心 = { 0,0,0 };
            尺寸 = { 0,0,0 };
            return;
        }

        Vector3D sum{ 0,0,0 };
        int cnt = 0;

        Vector3D mn{ +std::numeric_limits<double>::infinity(),
                     +std::numeric_limits<double>::infinity(),
                     +std::numeric_limits<double>::infinity() };
        Vector3D mx{ -std::numeric_limits<double>::infinity(),
                     -std::numeric_limits<double>::infinity(),
                     -std::numeric_limits<double>::infinity() };

        for (const auto& pix : 簇) {
            Vector3D P{};
            if (!取点云(帧, pix.u, pix.v, P)) continue;
            sum.x += P.x; sum.y += P.y; sum.z += P.z;
            mn.x = std::min(mn.x, P.x); mn.y = std::min(mn.y, P.y); mn.z = std::min(mn.z, P.z);
            mx.x = std::max(mx.x, P.x); mx.y = std::max(mx.y, P.y); mx.z = std::max(mx.z, P.z);
            ++cnt;
        }

        if (cnt <= 0) {
            中心 = { 0,0,0 };
            尺寸 = { 0,0,0 };
            return;
        }

        中心 = { sum.x / cnt, sum.y / cnt, sum.z / cnt };
        尺寸 = { std::max(0.0, mx.x - mn.x), std::max(0.0, mx.y - mn.y), std::max(0.0, mx.z - mn.z) };
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
    std::vector<点簇增强结果> 分割点簇_增强(const 结构体_原始场景帧& 帧, const 点簇分割参数& 参数) const
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

            // 3D 几何
            计算簇3D信息(帧, res.簇, res.中心, res.尺寸);

            // 轮廓 8x8（不依赖裁剪掩码）
            res.轮廓编码 = 计算轮廓编码8x8(res.簇, res.边界);

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

            out.push_back(std::move(res));
        }

        return out;
    }
};
