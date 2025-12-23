
export module 存在提取模块;

import 基础数据类型模块;
import 点簇分割模块;
import 子存在提取模块;

import <vector>;
import <cstdint>;
import <cmath>;
import <algorithm>;

// 干净版：存在观测提取
// - 只依赖 点簇增强结果（点簇分割/增强模块输出）
// - 删除“从原始点簇 + 原始帧重算几何/颜色”的兼容路径
// - 保留：
//   1) 点簇增强结果 -> 结构体_存在观测
//   2) 点簇增强结果列表 -> 观测列表
//   3) 父点簇增强结果 -> 子存在观测列表（通过 子存在提取模块）

export class 存在提取类
{
private:
    static std::vector<轮廓块64> 放大轮廓_最近邻(const std::vector<轮廓块64>& base8x8, int 目标边长)
    {
        // base8x8 应为 8x8 的 64bit 编码（1 个 uint64）
        if (base8x8.size() != 1 || 目标边长 <= 0) return {};

        const int outBits = 目标边长 * 目标边长;
        const std::size_t outBlocks = static_cast<std::size_t>((outBits + 63) / 64);
        std::vector<轮廓块64> out(outBlocks, 0);

        const 轮廓块64 src = base8x8[0];

        for (int y = 0; y < 目标边长; ++y) {
            for (int x = 0; x < 目标边长; ++x) {
                const int sx = (x * 8) / 目标边长;
                const int sy = (y * 8) / 目标边长;
                const int src_bit = sy * 8 + sx;
                if ((src & (1ull << src_bit)) == 0) continue;

                const int dst_bit = y * 目标边长 + x;
                const std::size_t dst_block = static_cast<std::size_t>(dst_bit / 64);
                const int dst_offset = dst_bit % 64;
                out[dst_block] |= (1ull << dst_offset);
            }
        }

        return out;
    }

public:
    // 1) 从点簇增强结果提取存在观测（唯一推荐路径）
    bool 从点簇增强提取存在观测(
        const 点簇增强结果& in,
        结构体_存在观测& out,
        const 观测提取参数& p = {}) const
    {
        if (in.簇.empty() || in.有效点数 < p.最小有效点数) 
            return false;
        // 尺寸/中心校验（相机系）
        // 注意：很多真实物体（或虚拟相机的“矩形物体”）在某一轴上的厚度可能非常小。
        // 因此这里不强制 3 轴都 >= 最小尺寸，而是要求“至少 2 个轴”满足即可。
        // 另外：为兼容你工程里常用的“毫米阈值”，若阈值明显过大（>10），自动按 mm->m 做一次换算。

        double 最小中心Z = p.最小中心Z;
        double 最小尺寸 = p.最小尺寸;
        if (最小中心Z > 10.0) 最小中心Z *= 0.001; // mm -> m
        if (最小尺寸 > 10.0) 最小尺寸 *= 0.001;   // mm -> m

        if (in.中心.z < 最小中心Z)
            return false;
        //if (最小尺寸 > 0.0) {
        //    const int ok = (in.尺寸.x >= 最小尺寸) + (in.尺寸.y >= 最小尺寸) + (in.尺寸.z >= 最小尺寸);
        //    if (ok < 2) 
        //           return false;
        if (最小尺寸 > 0.0) {
            // 取最大两个轴作为“平面尺寸”
            double a = in.尺寸.x, b = in.尺寸.y, c = in.尺寸.z;
            double max1 = std::max({ a,b,c });
            double min1 = std::min({ a,b,c });
            double mid = a + b + c - max1 - min1;
            if (max1 < 最小尺寸 || mid < 最小尺寸) return false;
        }

        out = {};
        out.时间 = 结构体_时间戳::当前();
        out.坐标系 = 枚举_坐标系::相机坐标系;

        out.中心坐标 = in.中心;
        out.尺寸 = in.尺寸;
        out.重新计算距离();

        out.主方向1 = in.主方向1;
        out.主方向2 = in.主方向2;
        out.主方向3 = in.主方向3;

        out.平均颜色 = in.平均颜色;
        out.有效点数 = in.有效点数;
        out.深度噪声_MAD = static_cast<float>(in.深度噪声_MAD);
        out.PCA残差 = static_cast<float>(in.PCA残差);
        out.颜色一致性 = static_cast<float>(in.颜色一致性);
        out.质量分 = static_cast<float>(in.质量分);

        // ROI
        out.ROI.x = in.边界.umin;
        out.ROI.y = in.边界.vmin;
        out.ROI.w = in.边界.umax - in.边界.umin + 1;
        out.ROI.h = in.边界.vmax - in.边界.vmin + 1;

        // 裁剪图像（可选）
        if (!in.裁剪颜色.empty() && in.裁剪宽 > 0 && in.裁剪高 > 0) {
            out.裁剪BGR.w = in.裁剪宽;
            out.裁剪BGR.h = in.裁剪高;
            out.裁剪BGR.c = 3;
            out.裁剪BGR.data.resize(in.裁剪颜色.size() * 3);
            for (std::size_t i = 0; i < in.裁剪颜色.size(); ++i) {
                out.裁剪BGR.data[i * 3 + 0] = in.裁剪颜色[i].b;
                out.裁剪BGR.data[i * 3 + 1] = in.裁剪颜色[i].g;
                out.裁剪BGR.data[i * 3 + 2] = in.裁剪颜色[i].r;
            }
            out.缓冲掩码 = out.缓冲掩码 | 枚举_存在观测缓冲::裁剪BGR;
        }

        if (!in.裁剪掩码.empty() && in.裁剪宽 > 0 && in.裁剪高 > 0) {
            out.裁剪掩膜.w = in.裁剪宽;
            out.裁剪掩膜.h = in.裁剪高;
            out.裁剪掩膜.c = 1;
            out.裁剪掩膜.data = in.裁剪掩码;
            out.缓冲掩码 = out.缓冲掩码 | 枚举_存在观测缓冲::裁剪掩膜;
        }

        // 轮廓编码（8x8：1 个 uint64）
        //if (!in.轮廓编码.empty()) {
        //    if (p.严格轮廓维度64 && in.轮廓编码.size() != 1) 
        //        return false;
            out.轮廓编码.assign(in.轮廓编码.begin(), in.轮廓编码.end());
       // }
       // else if (p.要求轮廓编码) {
        //    return false;
        //}

        // 多尺度轮廓金字塔（可选）
        if (p.生成多尺度轮廓 && out.轮廓编码.size() == 1) {
            out.多尺度轮廓金字塔[0] = out.轮廓编码;
            out.多尺度轮廓金字塔[1] = 放大轮廓_最近邻(out.轮廓编码, 16);
            out.多尺度轮廓金字塔[2] = 放大轮廓_最近邻(out.轮廓编码, 32);
            out.多尺度轮廓金字塔[3] = 放大轮廓_最近邻(out.轮廓编码, 64);
        }

        return true;
    }

    // 2) 批量提取（薄封装）
    std::vector<结构体_存在观测> 从点簇增强列表提取观测(
        const std::vector<点簇增强结果>& 列表,
        const 观测提取参数& p = {}) const
    {
        std::vector<结构体_存在观测> out;
        out.reserve(列表.size());

        for (const auto& e : 列表) {
            结构体_存在观测 obs;
            if (从点簇增强提取存在观测(e, obs, p)) {
                out.push_back(std::move(obs));
            }
        }

        return out;
    }

    // 3) 子存在提取：父增强 -> 子增强 -> 子观测
    std::vector<结构体_存在观测> 从父点簇增强提取子存在观测(
        const 点簇增强结果& 父,
        const 子存在提取参数& sp,
        const 观测提取参数& p = {}) const
    {
        子存在提取类 splitter;
        auto kids = splitter.从父点簇增强提取子点簇增强(父, sp);

        std::vector<结构体_存在观测> out;
        out.reserve(kids.size());

        for (const auto& k : kids) {
            结构体_存在观测 obs;
            if (从点簇增强提取存在观测(k, obs, p)) {
                out.push_back(std::move(obs));
            }
        }

        return out;
    }
};
