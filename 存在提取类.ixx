// 存在提取模块.ixx（去 OpenCV 版本，基于 结构体_原始场景帧 + 点簇增强结果）
export module 存在提取模块;

import 基础数据类型模块;
import 点簇分割模块;
import 子存在提取模块;

import <vector>;
import <cstdint>;
import <cmath>;
import <algorithm>;
import <limits>;

export class 存在提取类
{
private:
    static inline bool 点云有效(const Vector3D& P)
    {
        return std::isfinite(P.x) && std::isfinite(P.y) && std::isfinite(P.z) && P.z > 0.0;
    }

    static inline Vector3D 取点云或退化(const 结构体_原始场景帧& 帧, int u, int v, bool& out有效)
    {
        out有效 = false;
        if (帧.有效点云()) {
            const std::size_t id = 帧.索引(u, v);
            if (id < 帧.点云.size()) {
                const auto P = 帧.点云[id];
                if (点云有效(P)) { out有效 = true; return P; }
            }
        }
        // 退化：只有深度 Z（x,y 无法恢复）
        const std::size_t id = 帧.索引(u, v);
        if (id < 帧.深度.size()) {
            const double z = 帧.深度[id];
            if (std::isfinite(z) && z > 0.0) {
                out有效 = true;
                return Vector3D{ 0.0, 0.0, z };
            }
        }
        return Vector3D{};
    }

    static std::vector<std::int64_t> 放大轮廓(const std::vector<std::int64_t>& base8x8, int 目标边长)
    {
        if ((int)base8x8.size() != 64 || 目标边长 <= 0) return {};
        std::vector<std::int64_t> out(static_cast<std::size_t>(目标边长) * 目标边长, 0);
        for (int y = 0; y < 目标边长; ++y) {
            for (int x = 0; x < 目标边长; ++x) {
                const int sx = x * 8 / 目标边长;
                const int sy = y * 8 / 目标边长;
                out[static_cast<std::size_t>(y) * 目标边长 + x] = base8x8[static_cast<std::size_t>(sy) * 8 + sx];
            }
        }
        return out;
    }

public:
    // =========================================================
    // 1) 从点簇（像素集合）提取存在观测：几何 + 均色
    //    说明：若帧.点云为“每像素点云”，结果最准确；否则退化为仅 Z 轴深度。
    // =========================================================
    bool 从点簇提取存在观测(
        const 结构体_原始场景帧& 帧,
        const 点簇& 簇,
        结构体_存在观测& out,
        const 观测提取参数& p = {}) const
    {
        if (!帧.有效深度() || 簇.empty()) return false;

        Vector3D sum{ 0,0,0 };
        int cnt = 0;

        Vector3D mn{ +std::numeric_limits<double>::infinity(),
                     +std::numeric_limits<double>::infinity(),
                     +std::numeric_limits<double>::infinity() };
        Vector3D mx{ -std::numeric_limits<double>::infinity(),
                     -std::numeric_limits<double>::infinity(),
                     -std::numeric_limits<double>::infinity() };

        std::uint64_t sr = 0, sg = 0, sb = 0;
        int ccnt = 0;
        const bool 有颜色 = 帧.有效颜色();

        for (const auto& pix : 簇) {
            if ((unsigned)pix.u >= (unsigned)帧.宽度 || (unsigned)pix.v >= (unsigned)帧.高度) continue;

            bool ok = false;
            const Vector3D P = 取点云或退化(帧, pix.u, pix.v, ok);
            if (!ok) continue;

            sum.x += P.x; sum.y += P.y; sum.z += P.z;
            mn.x = std::min(mn.x, P.x); mn.y = std::min(mn.y, P.y); mn.z = std::min(mn.z, P.z);
            mx.x = std::max(mx.x, P.x); mx.y = std::max(mx.y, P.y); mx.z = std::max(mx.z, P.z);
            ++cnt;

            if (有颜色) {
                const auto c = 帧.颜色[帧.索引(pix.u, pix.v)];
                sr += c.r; sg += c.g; sb += c.b;
                ++ccnt;
            }
        }

        if (cnt <= 0) return false;

        out = {};
        out.时间 = 结构体_时间戳::当前();
        out.中心坐标 = { sum.x / cnt, sum.y / cnt, sum.z / cnt };
        out.尺寸 = { std::max(0.0, mx.x - mn.x), std::max(0.0, mx.y - mn.y), std::max(0.0, mx.z - mn.z) };
        out.距离 = std::sqrt(out.中心坐标.x * out.中心坐标.x + out.中心坐标.y * out.中心坐标.y + out.中心坐标.z * out.中心坐标.z);

        if (ccnt > 0) {
            out.平均颜色 = Color{
                static_cast<std::uint8_t>(sr / ccnt),
                static_cast<std::uint8_t>(sg / ccnt),
                static_cast<std::uint8_t>(sb / ccnt)
            };
        }
        else {
            out.平均颜色 = Color{ 255,255,255 };
        }

        // 轮廓编码：若帧.轮廓已存在且非空，可直接复用；否则留空（推荐走点簇增强结果提取）
        if (!帧.轮廓.empty()) {
            out.轮廓编码 = 帧.轮廓;
        }

        return true;
    }

    // =========================================================
    // 2) 从点簇增强结果提取存在观测：几何 + ROI + 轮廓
    // =========================================================
    bool 从点簇增强提取存在观测(
        const 点簇增强结果& in,
        结构体_存在观测& out,
        const 观测提取参数& p = {}) const
    {
        if (in.簇.empty()) return false;

        out = {};
        out.时间 = 结构体_时间戳::当前();
        out.中心坐标 = in.中心;
        out.尺寸 = in.尺寸;
        out.距离 = std::sqrt(out.中心坐标.x * out.中心坐标.x + out.中心坐标.y * out.中心坐标.y + out.中心坐标.z * out.中心坐标.z);

        // ROI / 掩码 / 图像
        out.ROI_x = 0;  //
        out.ROI_y = 0;
        out.ROI_w = in.裁剪宽;
        out.ROI_h = in.裁剪高;
        out.裁剪图像_BGR = in.裁剪颜色;
        out.裁剪掩膜 = in.裁剪掩码;

        // 轮廓
        out.轮廓编码 = in.轮廓编码;
        if (p.生成多尺度轮廓 && (int)out.轮廓编码.size() == 64) {
            out.多尺度轮廓金字塔[0] = out.轮廓编码;      // 8x8
            out.多尺度轮廓金字塔[1] = 放大轮廓(out.轮廓编码, 16);
            out.多尺度轮廓金字塔[2] = 放大轮廓(out.轮廓编码, 32);
            out.多尺度轮廓金字塔[3] = 放大轮廓(out.轮廓编码, 64);
        }

        // 均色（尽量用掩码）
        if (!in.裁剪颜色.empty()) {
            std::uint64_t sr = 0, sg = 0, sb = 0;
            int cnt = 0;

            if (!in.裁剪掩码.empty() && (int)in.裁剪掩码.size() == (int)in.裁剪颜色.size()) {
                for (std::size_t i = 0; i < in.裁剪颜色.size(); ++i) {
                    if (!in.裁剪掩码[i]) continue;
                    const auto c = in.裁剪颜色[i];
                    sr += c.r; sg += c.g; sb += c.b;
                    ++cnt;
                }
            }
            else {
                for (const auto& c : in.裁剪颜色) {
                    sr += c.r; sg += c.g; sb += c.b;
                    ++cnt;
                }
            }

            if (cnt > 0) {
                out.平均颜色 = Color{
                    static_cast<std::uint8_t>(sr / cnt),
                    static_cast<std::uint8_t>(sg / cnt),
                    static_cast<std::uint8_t>(sb / cnt)
                };
            }
            else {
                out.平均颜色 = Color{ 255,255,255 };
            }
        }
        else {
            out.平均颜色 = Color{ 255,255,255 };
        }

        return true;
    }

    // =========================================================
    // 3) 批量：点簇增强列表 -> 存在观测列表
    // =========================================================
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

    // =========================================================
    // 4) 子存在：本模块只做“拼装流程”，切分算法在 子存在提取模块
    // =========================================================
    std::vector<点簇增强结果> 从父点簇增强提取子点簇增强(
        const 点簇增强结果& 父,
        const 子存在提取参数& sp) const
    {
        子存在提取类 splitter;
        return splitter.从父点簇增强提取子点簇增强(父, sp);
    }

    std::vector<结构体_存在观测> 从父点簇增强提取子存在观测(
        const 结构体_原始场景帧& 帧,
        const 点簇增强结果& 父,
        const 子存在提取参数& sp,
        const 观测提取参数& p = {}) const
    {
        std::vector<结构体_存在观测> out;

        auto kids = 从父点簇增强提取子点簇增强(父, sp);
        out.reserve(kids.size());

        for (auto& k : kids) {
            // 如果子簇增强结果的几何为空，尝试用帧点云重算（更准）
            if ((k.尺寸.x == 0.0 && k.尺寸.y == 0.0 && k.尺寸.z == 0.0) ||
                (k.中心.x == 0.0 && k.中心.y == 0.0 && k.中心.z == 0.0)) {

                结构体_存在观测 geo;
                if (从点簇提取存在观测(帧, k.簇, geo, p)) {
                    k.中心 = geo.中心坐标;
                    k.尺寸 = geo.尺寸;
                }
            }

            结构体_存在观测 obs;
            if (从点簇增强提取存在观测(k, obs, p)) {
                out.push_back(std::move(obs));
            }
        }
        return out;
    }
};
