export module  存在提取模块;

import 相机接口模块;
import 点簇分割模块;
import 基础数据类型模块;    
import <vector>;
import <cstdint>;
import <cmath>;
import <algorithm>;
import <limits>;
import <array>;



// ==============================
// 内部工具（简化版，仅保留必要函数）
// ==============================
export class 存在提取类 {
private:

    inline int idx(int u, int v, int w) { return v * w + u; }
    inline bool 点有效(const Vector3D& p) { return p.z > 0.0f; }
public:
    // ==============================
    // 单簇提取（旧接口：从原始点簇）—— 保留兼容性，但颜色忽略
    // ==============================
    bool 从点簇提取存在观测(const 结构体_原始场景帧& 帧,const 点簇& 簇,结构体_存在观测& out,const 观测提取参数& p = {})
    {
        if (簇.size() < static_cast<size_t>(p.最小有效点数)) return false;

        Vector3D sumP{ 0.0f, 0.0f, 0.0f };
        int validCnt = 0;

        Vector3D minP{ FLT_MAX, FLT_MAX, FLT_MAX };
        Vector3D maxP{ -FLT_MAX, -FLT_MAX, -FLT_MAX };

        for (const auto& pix : 簇) {
            int id = idx(pix.u, pix.v, 帧.宽度);
            if (id >= (int)帧.点云.size()) continue;
            const Vector3D& P = 帧.点云[id];
            if (p.忽略无效点 && !点有效(P)) continue;

            sumP.x += P.x; sumP.y += P.y; sumP.z += P.z;
            minP.x = std::min(minP.x, P.x); maxP.x = std::max(maxP.x, P.x);
            minP.y = std::min(minP.y, P.y); maxP.y = std::max(maxP.y, P.y);
            minP.z = std::min(minP.z, P.z); maxP.z = std::max(maxP.z, P.z);
            ++validCnt;
        }

        if (validCnt == 0) return false;

        out.中心坐标 = { sumP.x / validCnt, sumP.y / validCnt, sumP.z / validCnt };

        Vector3D aabbSize = {
            std::max(maxP.x - minP.x, 0.01f),
            std::max(maxP.y - minP.y, 0.01f),
            std::max(maxP.z - minP.z, 0.01f)
        };
        out.尺寸 = aabbSize;

        // 颜色先忽略：设为默认白色
        out.平均颜色 = { 255, 255, 255 };

        // 轮廓3D 和轮廓编码留空（新流程由增强点簇提供）
        out.轮廓3D.clear();
        out.轮廓编码.clear();

        return true;
    }

    // ==============================
    // 推荐主接口：从点簇增强结果提取（最高效）
    // ==============================
    bool 从点簇增强提取存在观测(const 点簇增强结果& enhanced,结构体_存在观测& out)
    {
        if (enhanced.簇.empty()) return false;

        out.时间 = 结构体_时间戳::当前();
        out.中心坐标 = enhanced.中心;
        out.尺寸 = enhanced.尺寸;

        // ===== 平均颜色计算（保持不变）=====
        out.平均颜色 = { 255, 255, 255 };
        if (enhanced.裁剪宽 > 0 && enhanced.裁剪高 > 0 &&
            enhanced.裁剪颜色.size() == enhanced.裁剪掩码.size())
        {
            size_t N = enhanced.裁剪颜色.size();
            std::uint64_t sumR = 0, sumG = 0, sumB = 0, cnt = 0;
            for (size_t i = 0; i < N; ++i) {
                if (enhanced.裁剪掩码[i] == 0) continue;
                const auto& c = enhanced.裁剪颜色[i];
                sumR += c.r; sumG += c.g; sumB += c.b;
                ++cnt;
            }
            if (cnt > 0) {
                out.平均颜色.r = static_cast<uint8_t>(sumR / cnt);
                out.平均颜色.g = static_cast<uint8_t>(sumG / cnt);
                out.平均颜色.b = static_cast<uint8_t>(sumB / cnt);
            }
        }

        // ===== 自适应选择目标网格大小（8 的倍数）=====
        int target_grid = 8;  // 最小 8x8

        if (enhanced.裁剪宽 > 0 && enhanced.裁剪高 > 0) {
            // 取宽高中较大者作为参考尺寸
            int max_side = std::max(enhanced.裁剪宽, enhanced.裁剪高);

            // 根据最大边长就近选择 8*n
            // 示例阈值（可调优）：
            // < 20    → 8
            // < 40    → 16
            // < 70    → 24
            // < 100   → 32
            // >=100   → 继续向上（40,48,... 上限建议 64 或 80）
            if (max_side >= 100) {
                target_grid = ((max_side / 8) + 1) * 8;     // 上取整倍数
                target_grid = std::min(target_grid, 64);    // 上限建议 64（4096维），避免维度爆炸
            }
            else if (max_side >= 70)  target_grid = 32;
            else if (max_side >= 40)  target_grid = 24;
            else if (max_side >= 20)  target_grid = 16;
            else                      target_grid = 8;
        }

        const int TOTAL_CELLS = target_grid * target_grid;
       // out.轮廓网格大小 = target_grid;
        out.轮廓编码.assign(TOTAL_CELLS, 0);

        // ===== 如果有有效掩码，才进行标准化采样 =====
        if (enhanced.裁剪宽 > 0 && enhanced.裁剪高 > 0 && !enhanced.裁剪掩码.empty()) {
            for (int ty = 0; ty < target_grid; ++ty) {
                float ny = static_cast<float>(ty) / (target_grid - 1.0f);
                float sy = ny * (enhanced.裁剪高 - 1);

                for (int tx = 0; tx < target_grid; ++tx) {
                    float nx = static_cast<float>(tx) / (target_grid - 1.0f);
                    float sx = nx * (enhanced.裁剪宽 - 1);

                    // 双线性插值采样（更平滑）
                    int x0 = static_cast<int>(std::floor(sx));
                    int x1 = std::min(x0 + 1, enhanced.裁剪宽 - 1);
                    int y0 = static_cast<int>(std::floor(sy));
                    int y1 = std::min(y0 + 1, enhanced.裁剪高 - 1);

                    x0 = std::max(0, x0);
                    y0 = std::max(0, y0);

                    float fx = sx - x0;
                    float fy = sy - y0;

                    auto sample = [&](int xx, int yy) -> uint8_t {
                        size_t idx = static_cast<size_t>(yy) * enhanced.裁剪宽 + xx;
                        return enhanced.裁剪掩码[idx];
                        };

                    float val =
                        (1.0f - fx) * (1.0f - fy) * sample(x0, y0) +
                        fx * (1.0f - fy) * sample(x1, y0) +
                        (1.0f - fx) * fy * sample(x0, y1) +
                        fx * fy * sample(x1, y1);

                    size_t dst_idx = static_cast<size_t>(ty) * target_grid + tx;
                    out.轮廓编码[dst_idx] = (val > 0.5f) ? 1 : 0;
                }
            }
        }

        // 主方向暂不处理（后续可加PCA）
        out.主方向1 = { 1.0f, 0.0f, 0.0f };
        out.主方向2 = { 0.0f, 1.0f, 0.0f };
        out.主方向3 = { 0.0f, 0.0f, 1.0f };

        return true;
    }

    // ==============================
    // 批量提取：推荐使用增强版
    // ==============================
    std::vector<结构体_存在观测> 从点簇增强列表提取观测(const std::vector<点簇增强结果>& 簇列表)
    {
        std::vector<结构体_存在观测> out;
        out.reserve(簇列表.size());

        for (const auto& enhanced : 簇列表) {
            结构体_存在观测 obs;
            if (从点簇增强提取存在观测(enhanced, obs)) {
                out.push_back(obs);
            }
        }
        return out;
    }

    // ✅ 兼容接口：外设端可能传入 (帧, 增强簇列表)
    std::vector<结构体_存在观测> 从点簇增强列表提取观测(
        const 结构体_原始场景帧& /*帧*/,
        const std::vector<点簇增强结果>& 簇列表)
    {
        // 增强结果已携带裁剪颜色/掩码等信息，因此这里不必再依赖帧。
        return 从点簇增强列表提取观测(簇列表);
    }


    // 保留旧批量接口（兼容）
    std::vector<结构体_存在观测> 从点簇列表提取观测(const 结构体_原始场景帧& 帧,const std::vector<点簇>& 簇列表,const 观测提取参数& p = {})
    {
        std::vector<结构体_存在观测> out;
        out.reserve(簇列表.size());

        for (const auto& c : 簇列表) {
            结构体_存在观测 obs;
            if (从点簇提取存在观测(帧, c, obs, p)) {
                out.push_back(obs);
            }
        }
        return out;
    }
};