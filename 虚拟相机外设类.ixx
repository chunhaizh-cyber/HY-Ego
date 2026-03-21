module;
#include <string>
#include <cmath>
#include <cstdint>
#include <thread>
#include <chrono>
#include <variant>

#ifdef HY_HEADLESS_NO_MFC
#include "hy_windows.h"
#else
#include <afx.h>
#endif

export module 虚拟相机外设模块;

import 外设模块;
import 基础数据类型模块;
import 相机外设基类模块;


// ============================================================================
// 虚拟相机外设：无硬件也能跑完整流程（便于调试写入世界树/显示链路）
// 说明：本模块不再定义 相机外设基类，该基类已独立到 相机外设基类模块。
// ============================================================================

// ============================================================================
// 虚拟相机外设：生成简单深度图（两块矩形“物体”+背景无效深度）
// =========================================================================
export class 虚拟相机外设类 final : public 相机外设基类 {
public:
    虚拟相机外设类() = default;
    ~虚拟相机外设类() override = default;

protected:
    bool 启动设备(const 外设启动参数& p) override {
        w_ = (p.宽 > 0) ? p.宽 : 848;
        h_ = (p.高 > 0) ? p.高 : 480;
        fps_ = (p.期望FPS > 0) ? p.期望FPS : 30;
        if (w_ <= 0 || h_ <= 0) return false;
        if (fps_ <= 0) fps_ = 30;
        tick_ = 0;
        return true;
    }

    void 停止设备() override {
        // 无资源
    }

    外设指令回执 处理指令_设备(const 外设指令& cmd) override {
        if (cmd.类型 == 枚举_外设指令::设置参数 && cmd.参数名 == "fps") {
            if (auto p = std::get_if<std::int64_t>(&cmd.参数值_)) {
                fps_ = (int)*p;
                if (fps_ <= 0) fps_ = 30;
                return { true, 0, "fps 已更新" };
            }
        }
        return { false, -1, "虚拟相机未实现该指令" };
    }

    bool 抓取数据_阻塞(结构体_原始场景帧& out) override {
        const int period_ms = (fps_ > 0) ? (int)std::llround(1000.0 / (double)fps_) : 33;
        ::Sleep(period_ms > 0 ? static_cast<DWORD>(period_ms) : 1u);

        out = {};
        out.时间戳.系统到达时间_us = 结构体_时间戳::当前_微秒();
        out.宽度 = w_;
        out.高度 = h_;
        out.深度.assign((std::size_t)w_ * (std::size_t)h_, 0.0);  // 0=无效

        // 单位统一：深度/点云全部为 mm。
        const double jitter_mm = 10.0 * std::sin((double)tick_ * 0.15);

        auto draw_rect = [&](int x0, int y0, int x1, int y1, double z) {
            x0 = std::max(0, x0); y0 = std::max(0, y0);
            x1 = std::min(w_ - 1, x1); y1 = std::min(h_ - 1, y1);
            for (int y = y0; y <= y1; ++y) {
                for (int x = x0; x <= x1; ++x) {
                    out.深度[(std::size_t)y * (std::size_t)w_ + (std::size_t)x] = z;
                }
            }
            };

        draw_rect(w_ / 6, h_ / 3, w_ / 3, h_ * 2 / 3, 1000.0 + jitter_mm);
        draw_rect(w_ * 2 / 3, h_ / 4, w_ * 5 / 6, h_ / 2, 1350.0 - jitter_mm);

        out.颜色.assign(out.深度.size(), Color{ 255, 255, 255 });
        for (int y = h_ / 3; y <= h_ * 2 / 3; ++y)
            for (int x = w_ / 6; x <= w_ / 3; ++x)
                out.颜色[(std::size_t)y * (std::size_t)w_ + (std::size_t)x] = Color{ 255, 80, 80 };

        for (int y = h_ / 4; y <= h_ / 2; ++y)
            for (int x = w_ * 2 / 3; x <= w_ * 5 / 6; ++x)
                out.颜色[(std::size_t)y * (std::size_t)w_ + (std::size_t)x] = Color{ 80, 255, 80 };

        // 设置深度内参（虚拟内参）：让世界融合轮廓生成能够正常工作
        out.深度内参.fx = 600.0;
        out.深度内参.fy = 600.0;
        out.深度内参.cx = (double)(w_ - 1) * 0.5;
        out.深度内参.cy = (double)(h_ - 1) * 0.5;
        out.深度内参.宽度 = w_;
        out.深度内参.高度 = h_;
        out.深度内参.有效 = true;

        // 设置相机状态有效
        out.相机.有效 = true;

        // 生成点云（使用与深度内参相同的参数）：让点簇增强阶段能得到 3D 中心/尺寸/PCA，避免“有效点数=0”。
        out.点云.assign(out.深度.size(), Vector3D{ 0,0,0 });
        const double fx = out.深度内参.fx;
        const double fy = out.深度内参.fy;
        const double cx = out.深度内参.cx;
        const double cy = out.深度内参.cy;
        for (int v = 0; v < h_; ++v) {
            for (int u = 0; u < w_; ++u) {
                const std::size_t id = (std::size_t)v * (std::size_t)w_ + (std::size_t)u;
                const double z = out.深度[id];
                if (z <= 0.0) continue;
                const double X = ((double)u - cx) * z / fx;
                const double Y = ((double)v - cy) * z / fy;
                out.点云[id] = Vector3D{ X, Y, z };
            }
        }

        ++tick_;
        return true;
    }

private:
    int w_ = 848;
    int h_ = 480;
    int fps_ = 30;
    std::uint64_t tick_ = 0;
};
