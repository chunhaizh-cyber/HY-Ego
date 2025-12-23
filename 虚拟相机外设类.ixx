
module;
#include <afx.h>

export module 虚拟相机外设模块;

import 外设模块;
import 基础数据类型模块;

import <string>;
import <cmath>;
import <cstdint>;
import <thread>;
import <chrono>;

// ============================================================================
// 相机外设模块
//  - 提供：相机外设抽象基类（继承 外设运行基类<结构体_原始场景帧>）
//  - 附带：虚拟相机外设（无硬件也能跑完整流程，便于调试写入世界树）
// ============================================================================

export class 相机外设基类 : public 外设运行基类<结构体_原始场景帧> {
public:
    virtual ~相机外设基类() = default;
};

// ============================================================================
// 虚拟相机外设：生成简单深度图（两块矩形“物体”+背景无效深度）
// =========================================================================
export class 虚拟相机外设类 final : public 相机外设基类 {
public:
    虚拟相机外设类() = default;
    ~虚拟相机外设类() override = default;

protected:
    bool 启动设备(const 外设启动参数& p) override {
        w_ = (p.宽 > 0) ? p.宽 : 640;
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
        std::this_thread::sleep_for(std::chrono::milliseconds(period_ms));

        out = {};
        out.时间 = 结构体_时间戳::当前();
        out.宽度 = w_;
        out.高度 = h_;
        out.深度.assign((std::size_t)w_ * (std::size_t)h_, 0.0);  // 0=无效

        const double jitter = 0.01 * std::sin((double)tick_ * 0.15);

        auto draw_rect = [&](int x0, int y0, int x1, int y1, double z) {
            x0 = std::max(0, x0); y0 = std::max(0, y0);
            x1 = std::min(w_ - 1, x1); y1 = std::min(h_ - 1, y1);
            for (int y = y0; y <= y1; ++y) {
                for (int x = x0; x <= x1; ++x) {
                    out.深度[(std::size_t)y * (std::size_t)w_ + (std::size_t)x] = z;
                }
            }
            };

        draw_rect(w_ / 6, h_ / 3, w_ / 3, h_ * 2 / 3, 1.0 + jitter);
        draw_rect(w_ * 2 / 3, h_ / 4, w_ * 5 / 6, h_ / 2, 1.35 - jitter);

        out.颜色.assign(out.深度.size(), Color{ 255, 255, 255 });
        for (int y = h_ / 3; y <= h_ * 2 / 3; ++y)
            for (int x = w_ / 6; x <= w_ / 3; ++x)
                out.颜色[(std::size_t)y * (std::size_t)w_ + (std::size_t)x] = Color{ 255, 80, 80 };

        for (int y = h_ / 4; y <= h_ / 2; ++y)
            for (int x = w_ * 2 / 3; x <= w_ * 5 / 6; ++x)
                out.颜色[(std::size_t)y * (std::size_t)w_ + (std::size_t)x] = Color{ 80, 255, 80 };

        // 生成点云（虚拟内参）：让点簇增强阶段能得到 3D 中心/尺寸/PCA，避免“有效点数=0”。
        // 这里使用 pinhole 模型的一个合理默认，足够用于调试整条链路。
        out.点云.assign(out.深度.size(), Vector3D{ 0,0,0 });
        const double fx = 600.0;
        const double fy = 600.0;
        const double cx = (double)(w_ - 1) * 0.5;
        const double cy = (double)(h_ - 1) * 0.5;
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
    int w_ = 640;
    int h_ = 480;
    int fps_ = 30;
    std::uint64_t tick_ = 0;
};
