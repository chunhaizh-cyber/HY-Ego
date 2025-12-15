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


export struct 存在观测 {
    时间戳 时间;

    Vector3D 中心;
    Vector3D 尺寸;

    Color 平均颜色;  // 仅 r,g,b 三通道

    // 可选：PCA 主轴（单位向量）
    Vector3D 主方向1{ 1.0f, 0.0f, 0.0f };
    Vector3D 主方向2{ 0.0f, 1.0f, 0.0f };
    Vector3D 主方向3{ 0.0f, 0.0f, 1.0f };

    std::vector<Vector3D> 轮廓3D;              // 可选：原始边界 3D 点（调试用）
    std::vector<std::int64_t> 轮廓编码;        // 核心：8×8=64 维封闭内部二值图
};
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
    bool 从点簇提取存在观测(
        const 结构体_原始场景帧& 帧,
        const 点簇& 簇,
        存在观测& out,
        const 观测提取参数& p = {})
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

        out.中心 = { sumP.x / validCnt, sumP.y / validCnt, sumP.z / validCnt };

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
    bool 从点簇增强提取存在观测(
        const 点簇增强结果& enhanced,
        存在观测& out)
    {
        if (enhanced.簇.empty()) return false;

        out.时间 = 结构体_时间戳::当前();
        out.中心 = enhanced.中心;
        out.尺寸 = enhanced.尺寸;

        out.轮廓编码 = enhanced.轮廓编码;     // 直接复制 64 维二值图
        out.轮廓3D = enhanced.轮廓3D;         // 调试用（可为空）

        // ===== 平均颜色：优先用“裁剪颜色+掩码” =====
        out.平均颜色 = { 255, 255, 255 };
        if (enhanced.裁剪宽 > 0 && enhanced.裁剪高 > 0) {
            const size_t N = (size_t)enhanced.裁剪宽 * (size_t)enhanced.裁剪高;
            if (enhanced.裁剪颜色.size() == N) {
                const bool hasMask = (enhanced.裁剪掩码.size() == N);
                std::uint64_t sumR = 0, sumG = 0, sumB = 0, cnt = 0;
                for (size_t i = 0; i < N; ++i) {
                    if (hasMask && enhanced.裁剪掩码[i] == 0) continue;
                    const auto& c = enhanced.裁剪颜色[i];
                    sumR += (std::uint64_t)c.r;
                    sumG += (std::uint64_t)c.g;
                    sumB += (std::uint64_t)c.b;
                    ++cnt;
                }
                if (cnt > 0) {
                    out.平均颜色.r = (std::uint8_t)(sumR / cnt);
                    out.平均颜色.g = (std::uint8_t)(sumG / cnt);
                    out.平均颜色.b = (std::uint8_t)(sumB / cnt);
                }
            }
        }

        // 主方向暂设为标准坐标轴（后续可接 PCA/OBB）
        out.主方向1 = { 1.0f, 0.0f, 0.0f };
        out.主方向2 = { 0.0f, 1.0f, 0.0f };
        out.主方向3 = { 0.0f, 0.0f, 1.0f };

        return true;
    }

    // ==============================
    // 批量提取：推荐使用增强版
    // ==============================
    std::vector<存在观测> 从点簇增强列表提取观测(const std::vector<点簇增强结果>& 簇列表)
    {
        std::vector<存在观测> out;
        out.reserve(簇列表.size());

        for (const auto& enhanced : 簇列表) {
            存在观测 obs;
            if (从点簇增强提取存在观测(enhanced, obs)) {
                out.push_back(obs);
            }
        }
        return out;
    }

    // ✅ 兼容接口：外设端可能传入 (帧, 增强簇列表)
    std::vector<存在观测> 从点簇增强列表提取观测(
        const 结构体_原始场景帧& /*帧*/,
        const std::vector<点簇增强结果>& 簇列表)
    {
        // 增强结果已携带裁剪颜色/掩码等信息，因此这里不必再依赖帧。
        return 从点簇增强列表提取观测(簇列表);
    }


    // 保留旧批量接口（兼容）
    std::vector<存在观测> 从点簇列表提取观测(
        const 结构体_原始场景帧& 帧,
        const std::vector<点簇>& 簇列表,
        const 观测提取参数& p = {})
    {
        std::vector<存在观测> out;
        out.reserve(簇列表.size());

        for (const auto& c : 簇列表) {
            存在观测 obs;
            if (从点簇提取存在观测(帧, c, obs, p)) {
                out.push_back(obs);
            }
        }
        return out;
    }
};