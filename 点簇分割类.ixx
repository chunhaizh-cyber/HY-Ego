export module 点簇分割模块;

import 相机接口模块;
import 基础数据类型模块;
import <vector>;
import <cstdint>;
import <cmath>;
import <algorithm>;
import <queue>;
import <limits>;
import <array>;

// ===== 点索引 / 点簇 =====
export struct 点索引 {
    int u = 0;
    int v = 0;
};

export using 点簇 = std::vector<点索引>;

// ===== 参数 =====
export struct 点簇分割参数 {
    // 只考虑这个距离范围内的点（米）
    float 最小深度 = 0.15f;
    float 最大深度 = 8.0f;

    // 邻域连接阈值（米）
    float 邻域最大三维距离 = 0.04f;

    // 当点云不可用时，用深度差做退化连接判断（米）
    float 邻域最大深度差 = 0.06f;

    // 4 邻域 or 8 邻域
    bool 使用8邻域 = true;

    // 性能优化：>1 时跳点扫描
    int 采样步长 = 1;

    // 过滤噪声：点数太少的簇丢弃
    int 最小点数 = 80;

    // 可选：点云 z==0 的点是否直接忽略
    bool 忽略无效点 = true;
};

// ===== 输出结构 =====
export struct 点簇边界框 {
    int umin = 0, umax = 0;
    int vmin = 0, vmax = 0;
};

export struct 点簇结果 {
    点簇        簇;
    点簇边界框  边界;
};


export class 点簇分割类 {
private:
    // ===== 内部工具 =====

    static inline int 索引(int u, int v, int w) { return v * w + u; }

    inline bool 深度有效(float z, const 点簇分割参数& p) {
        return (z >= p.最小深度 && z <= p.最大深度);
    }

    inline bool 点有效(const Vector3D& P) {
        return (P.z > 0.0f);
    }

    inline bool 邻接连通(const 结构体_原始场景帧& 帧, int idxA, int idxB, const 点簇分割参数& p, bool 点云可用) {
        const float zA = 帧.深度[idxA];
        const float zB = 帧.深度[idxB];

        if (!深度有效(zA, p) || !深度有效(zB, p))
            return false;

        if (点云可用) {
            const auto& A = 帧.点云[idxA];
            const auto& B = 帧.点云[idxB];

            if (p.忽略无效点 && (!点有效(A) || !点有效(B)))
                return false;

            const double dx = A.x - B.x;
            const double dy = A.y - B.y;
            const double dz = A.z - B.z;
            const double d2 = dx * dx + dy * dy + dz * dz;

            const double thr2 = static_cast<double>(p.邻域最大三维距离) * p.邻域最大三维距离;
            return d2 <= thr2;
        }

        // 点云不可用：退化为深度差
        return std::fabs(zA - zB) <= p.邻域最大深度差;
    }

    // 计算簇的 3D 中心和包围盒尺寸
    static void 计算簇3D信息(
        const 结构体_原始场景帧& 帧,
        const 点簇& 簇,
        Vector3D& out中心,
        Vector3D& out尺寸)
    {
        if (簇.empty()) return;

        double minx = std::numeric_limits<double>::infinity();
        double maxx = -std::numeric_limits<double>::infinity();
        double miny = std::numeric_limits<double>::infinity();
        double maxy = -std::numeric_limits<double>::infinity();
        double minz = std::numeric_limits<double>::infinity();
        double maxz = -std::numeric_limits<double>::infinity();

        double sumx = 0.0, sumy = 0.0, sumz = 0.0;
        int validCnt = 0;

        for (const auto& pix : 簇) {
            int idx = 索引(pix.u, pix.v, 帧.宽度);
            if (idx >= (int)帧.点云.size()) continue;
            const Vector3D& p = 帧.点云[idx];
            if (p.z <= 0.0f) continue;

            sumx += p.x; sumy += p.y; sumz += p.z;
            minx = std::min(minx, (double)p.x); maxx = std::max(maxx, (double)p.x);
            miny = std::min(miny, (double)p.y); maxy = std::max(maxy, (double)p.y);
            minz = std::min(minz, (double)p.z); maxz = std::max(maxz, (double)p.z);
            ++validCnt;
        }

        if (validCnt == 0) return;

        out中心 = { static_cast<float>(sumx / validCnt),
                    static_cast<float>(sumy / validCnt),
                    static_cast<float>(sumz / validCnt) };

        out尺寸 = {
            static_cast<float>(maxx - minx),
            static_cast<float>(maxy - miny),
            static_cast<float>(maxz - minz)
        };

        // 防止尺寸为0导致后续除零
        constexpr float MIN_SIZE = 0.01f;
        if (out尺寸.x < MIN_SIZE) out尺寸.x = MIN_SIZE;
        if (out尺寸.y < MIN_SIZE) out尺寸.y = MIN_SIZE;
        if (out尺寸.z < MIN_SIZE) out尺寸.z = MIN_SIZE;
    }

    // 计算单个点簇的 8×8 封闭轮廓二值图（内部已填充）
    static std::vector<std::int64_t> 计算封闭轮廓二值图(const 结构体_原始场景帧& 帧,const 点簇& 簇,const Vector3D& 中心3D,const Vector3D& 尺寸3D)
    {
        constexpr int GRID = 8;
        constexpr int TOTAL = GRID * GRID;  // 64
        std::vector<std::int64_t> 编码(TOTAL, 0);

        if (簇.size() < 20) {  // 点太少无法形成可靠轮廓
            return 编码;
        }

        const double eps = 1e-6;
        double min_x = 中心3D.x - 尺寸3D.x * 0.5 - eps;
        double max_x = 中心3D.x + 尺寸3D.x * 0.5 + eps;
        double min_y = 中心3D.y - 尺寸3D.y * 0.5 - eps;
        double max_y = 中心3D.y + 尺寸3D.y * 0.5 + eps;

        double range_x = max_x - min_x;
        double range_y = max_y - min_y;

        std::vector<std::vector<int>> grid(GRID, std::vector<int>(GRID, 0));

        // 步骤1：投影所有点到 XY 平面并标记
        for (const auto& pix : 簇) {
            int idx = 索引(pix.u, pix.v, 帧.宽度);
            if (idx >= (int)帧.点云.size()) continue;

            const Vector3D& pt = 帧.点云[idx];
            if (pt.z <= 0.0f) continue;

            // 过滤背面点，减少干扰
            if (std::abs(pt.z - 中心3D.z) > 尺寸3D.z * 0.7) continue;

            double nx = (pt.x - min_x) / range_x;
            double ny = (pt.y - min_y) / range_y;

            int ix = static_cast<int>(nx * GRID);
            int iy = static_cast<int>(ny * GRID);

            ix = std::clamp(ix, 0, GRID - 1);
            iy = std::clamp(iy, 0, GRID - 1);

            grid[iy][ix] = 1;
        }

        // 步骤2：简单膨胀填充内部（模拟形态学闭操作）
        auto temp = grid;
        for (int y = 0; y < GRID; ++y) {
            for (int x = 0; x < GRID; ++x) {
                if (grid[y][x] == 1) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        for (int dx = -1; dx <= 1; ++dx) {
                            int ny = y + dy, nx = x + dx;
                            if (ny >= 0 && ny < GRID && nx >= 0 && nx < GRID) {
                                temp[ny][nx] = 1;
                            }
                        }
                    }
                }
            }
        }
        grid = temp;

        // 步骤3：展平为 64 维向量
        int pos = 0;
        for (int y = 0; y < GRID; ++y) {
            for (int x = 0; x < GRID; ++x) {
                编码[pos++] = grid[y][x];
            }
        }

        return 编码;
    }

public:

    // ===== 基础分割：只返回点簇（u,v 列表）=====
    std::vector<点簇> 分割点簇(const 结构体_原始场景帧& 帧, const 点簇分割参数& 参数) {
        std::vector<点簇> 输出;

        const int w = 帧.宽度;
        const int h = 帧.高度;

        if (w <= 0 || h <= 0) return 输出;
        if ((int)帧.深度.size() != w * h) return 输出;

        const bool 点云可用 = ((int)帧.点云.size() == w * h);

        const int step = std::max(1, 参数.采样步长);

        std::vector<std::uint8_t> visited(w * h, 0);

        // 统一使用 8 邻域数组，运行时根据参数控制遍历数量
        constexpr std::array<std::array<int, 2>, 8> offsets_all =
        { {{-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}} };

        const size_t offset_count = 参数.使用8邻域 ? 8 : 4;

        std::vector<int> queue;

        for (int v = 0; v < h; v += step) {
            for (int u = 0; u < w; u += step) {
                const int idx = 索引(u, v, w);
                if (visited[idx]) continue;
                if (!深度有效(帧.深度[idx], 参数)) continue;

                点簇 当前簇;
                queue.clear();
                queue.push_back(idx);
                visited[idx] = 1;

                while (!queue.empty()) {
                    const int curIdx = queue.back();
                    queue.pop_back();

                    const int cu = curIdx % w;
                    const int cv = curIdx / w;

                    当前簇.push_back({ cu, cv });

                    // 只遍历需要的邻域数量
                    for (size_t i = 0; i < offset_count; ++i) {
                        const auto& d = offsets_all[i];
                        const int nu = cu + d[0];
                        const int nv = cv + d[1];

                        if ((unsigned)nu >= (unsigned)w || (unsigned)nv >= (unsigned)h) continue;
                        if ((nu % step) != 0 || (nv % step) != 0) continue;

                        const int nidx = 索引(nu, nv, w);
                        if (visited[nidx]) continue;

                        if (邻接连通(帧, curIdx, nidx, 参数, 点云可用)) {
                            visited[nidx] = 1;
                            queue.push_back(nidx);
                        }
                    }
                }

                if ((int)当前簇.size() >= 参数.最小点数) {
                    输出.push_back(当前簇);
                }
            }
        }

        return 输出;
    }

    // ===== 返回点簇 + 边界框 =====
    std::vector<点簇结果> 分割点簇_带边界框(const 结构体_原始场景帧& 帧,const 点簇分割参数& 参数 ) {
        auto 簇列表 = 分割点簇(帧, 参数);

        std::vector<点簇结果> out;
        out.reserve(簇列表.size());

        for (auto& c : 簇列表) {
            点簇边界框 box;
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

            out.push_back({ c, box });
        }
        return out;
    }

    // ===== 推荐主接口：返回完整增强信息（含封闭轮廓二值图）=====
    std::vector<点簇增强结果> 分割点簇_增强(
        const 结构体_原始场景帧& 帧,
        const 点簇分割参数& 参数)
    {
        auto 原始簇列表 = 分割点簇(帧, 参数);

        std::vector<点簇增强结果> 结果;
        结果.reserve(原始簇列表.size());

        for (auto& c : 原始簇列表) {
            // 过滤点数太少的簇
            if (c.size() < static_cast<size_t>(参数.最小点数)) {
                continue;
            }

            点簇增强结果 res;
            res.簇 = std::move(c);  // 可选：move 提升性能

            // ===== 计算像素级边界框 =====
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

            // ===== 计算 3D 中心与包围盒尺寸 =====
            计算簇3D信息(帧, res.簇, res.中心, res.尺寸);

            // ===== 计算自适应的裁剪区域（高分辨率掩码 + 颜色）=====
            res.裁剪宽 = res.边界.umax - res.边界.umin + 1;
            res.裁剪高 = res.边界.vmax - res.边界.vmin + 1;

            // 边界异常保护
            if (res.裁剪宽 <= 0 || res.裁剪高 <= 0) {
                // 无效边界，直接加入空结果（或 continue，视需求而定）
                res.裁剪宽 = res.裁剪高 = 0;
                res.裁剪颜色.clear();
                res.裁剪掩码.clear();
                结果.push_back(std::move(res));
                continue;
            }

            const size_t total_pixels = static_cast<size_t>(res.裁剪宽) * res.裁剪高;

            res.裁剪颜色.resize(total_pixels);
            res.裁剪掩码.assign(total_pixels, 0);  // 全初始化为背景

            // ===== 填充裁剪颜色和前景掩码 =====
            for (const auto& pix : res.簇) {
                const int local_u = pix.u - res.边界.umin;
                const int local_v = pix.v - res.边界.vmin;

                // 理论上不会越界，但加安全检查更稳健
                if (local_u < 0 || local_u >= res.裁剪宽 || local_v < 0 || local_v >= res.裁剪高) {
                    continue;
                }

                const size_t local_idx = static_cast<size_t>(local_v) * res.裁剪宽 + local_u;

                const int global_idx = 索引(pix.u, pix.v, 帧.宽度);
                if (global_idx >= 0 && static_cast<size_t>(global_idx) < 帧.颜色.size()) {
                    res.裁剪颜色[local_idx] = 帧.颜色[global_idx];
                }
                res.裁剪掩码[local_idx] = 1;  // 前景点
            }

            // ===== 轮廓编码留空：在存在提取阶段再根据裁剪掩码标准化生成 =====
            res.轮廓编码.clear();

            // ===== 调试用轮廓3D 可选保留（当前不强制填充）=====
            res.轮廓3D.clear();

            结果.push_back(std::move(res));
        }

        return 结果;
    }
};