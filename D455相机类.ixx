export module D455相机模块;

import 相机接口模块;

import <librealsense2/rs.hpp>;
import <iostream>;
import <algorithm>;
import <cstdint>;
import <cmath>;

export class D455_相机实现 : public 抽象深度相机接口 {
public:
    struct 配置项 {
        // 流配置
        int 深度宽 = 640;
        int 深度高 = 480;
        int 彩色宽 = 640;
        int 彩色高 = 480;
        int 帧率 = 30;

        // ===== 颜色稳定（推荐开启）=====
        bool 彩色_自动曝光 = true;
        bool 彩色_自动白平衡 = true;

        // 若关闭自动，则使用手动值（仅在 supports 时生效）
        float 彩色_曝光 = 8000.0f;     // microseconds (范围随设备而变)
        float 彩色_增益 = 64.0f;        // 0..?
        float 彩色_白平衡 = 4500.0f;    // Kelvin

        // ===== 深度稳定（可选）=====
        bool 深度_启用发射器 = true;
        float 深度_激光功率 = 150.0f;   // D455 支持范围随设备而变

        // ===== 深度滤波链（推荐开启空间+时间+填洞）=====
        bool 启用视差域处理 = true;     // Spatial/Temporal 在 disparity 域更稳

        bool 启用空间滤波 = true;
        float 空间_平滑系数 = 0.5f;     // RS2_OPTION_FILTER_SMOOTH_ALPHA (0..1)
        float 空间_平滑阈值 = 20.0f;    // RS2_OPTION_FILTER_SMOOTH_DELTA
        float 空间_孔洞填充 = 0.0f;     // RS2_OPTION_HOLES_FILL (0..2)

        bool 启用时间滤波 = true;
        float 时间_平滑系数 = 0.4f;     // RS2_OPTION_FILTER_SMOOTH_ALPHA (0..1)
        float 时间_平滑阈值 = 20.0f;    // RS2_OPTION_FILTER_SMOOTH_DELTA
        float 时间_持久性 = 3.0f;       // RS2_OPTION_HOLES_FILL / PERSISTENCE（不同版本命名可能不同，supports 则生效）

        bool 启用填洞滤波 = true;
        float 填洞_模式 = 1.0f;         // RS2_OPTION_HOLES_FILL (0..2)

        // 降采样滤波默认不启用（因为会改变分辨率，容易破坏对齐）
        bool 启用降采样 = false;
        float 降采样_倍率 = 1.0f;       // RS2_OPTION_FILTER_MAGNITUDE，>1 会改分辨率（谨慎）
    };

public:
    explicit D455_相机实现(配置项 cfg = {})
        : cfg(cfg),
        对齐器(RS2_STREAM_DEPTH),
        深度到视差(true),
        视差到深度(false) {
    }

    bool 打开() override {
        try {
            rs2::config c;

            c.enable_stream(RS2_STREAM_DEPTH, cfg.深度宽, cfg.深度高, RS2_FORMAT_Z16, cfg.帧率);
            c.enable_stream(RS2_STREAM_COLOR, cfg.彩色宽, cfg.彩色高, RS2_FORMAT_ANY, cfg.帧率);

            profile = 管道.start(c);
            已打开 = true;

            // 深度内参
            auto 深度profile = profile.get_stream(RS2_STREAM_DEPTH).as<rs2::video_stream_profile>();
            深度内参 = 深度profile.get_intrinsics();

            // 深度尺度
            rs2::device dev = profile.get_device();
            if (auto ds = dev.first<rs2::depth_sensor>(); ds) {
                深度尺度 = ds.get_depth_scale();
            }

            // 配置传感器参数（自动曝光/白平衡、发射器等）
            配置传感器(dev);

            // 配置滤波器参数
            配置滤波链();

            return true;

        }
        catch (const rs2::error& e) {
            std::cerr << "D455 打开失败: " << e.what() << std::endl;
            return false;
        }
    }

    void 关闭() override {
        if (已打开) {
            try { 管道.stop(); }
            catch (...) {}
            已打开 = false;
        }
    }

    bool 采集一帧(结构体_原始场景帧& 输出) override {
        if (!已打开) return false;

        try {
            rs2::frameset frames = 管道.wait_for_frames();

            // ===== 1) 对齐：彩色对齐到深度 =====
            rs2::frameset aligned = 对齐器.process(frames);

            rs2::depth_frame depth = aligned.get_depth_frame();
            rs2::video_frame color = aligned.get_color_frame();
            if (!depth || !color) return false;

            // ===== 2) 深度滤波链（不改变分辨率的滤波默认安全）=====
            rs2::frame filtered = depth;

            // 降采样（谨慎，默认关闭；倍率>1会改变分辨率，可能影响对齐）
            if (cfg.启用降采样) {
                filtered = 降采样滤波.process(filtered);
            }

            if (cfg.启用视差域处理) {
                filtered = 深度到视差.process(filtered);
            }

            if (cfg.启用空间滤波) {
                filtered = 空间滤波.process(filtered);
            }

            if (cfg.启用时间滤波) {
                filtered = 时间滤波.process(filtered);
            }

            if (cfg.启用视差域处理) {
                filtered = 视差到深度.process(filtered);
            }

            if (cfg.启用填洞滤波) {
                filtered = 填洞滤波.process(filtered);
            }

            depth = filtered.as<rs2::depth_frame>();
            if (!depth) return false;

            const int w = depth.get_width();
            const int h = depth.get_height();

            输出.时间 = 结构体_时间戳::当前();
            输出.宽度 = w;
            输出.高度 = h;

            输出.深度.assign((size_t)w * (size_t)h, 0.0f);
            输出.颜色.assign((size_t)w * (size_t)h, Color{ 255,255,255 });
            输出.点云.assign((size_t)w * (size_t)h, Vector3D{ 0,0,0 });

            // ===== 3) 深度读取（更快：Z16 * depth_scale）=====
            const uint16_t* dp = (const uint16_t*)depth.get_data();
            if (!dp) return false;

            for (int i = 0; i < w * h; ++i) {
                输出.深度[(size_t)i] = (float)dp[i] * 深度尺度; // 米
            }

            // ===== 4) 彩色读取：支持 RGB/BGR/RGBA/BGRA + YUYV/UYVY 兜底 =====
            读取对齐彩色(color, 输出);

            // ===== 5) 点云生成（相机坐标系）=====
            生成点云(输出);

            return true;

        }
        catch (const rs2::error& e) {
            std::cerr << "采集一帧失败: " << e.what() << std::endl;
            return false;
        }
    }

private:
    bool 已打开 = false;
    配置项 cfg;

    rs2::pipeline 管道;
    rs2::pipeline_profile profile;

    rs2::align 对齐器;

    // 深度参数
    rs2_intrinsics 深度内参{};
    float 深度尺度 = 0.001f;

    // 滤波器
    rs2::decimation_filter 降采样滤波;
    rs2::disparity_transform 深度到视差;
    rs2::spatial_filter 空间滤波;
    rs2::temporal_filter 时间滤波;
    rs2::disparity_transform 视差到深度;
    rs2::hole_filling_filter 填洞滤波;

private:
    static inline size_t 索引(int u, int v, int w) {
        return (size_t)v * (size_t)w + (size_t)u;
    }

    static inline uint8_t clamp_u8(int x) {
        if (x < 0) return 0;
        if (x > 255) return 255;
        return (uint8_t)x;
    }

    static inline void yuv_to_rgb(uint8_t Y, uint8_t U, uint8_t V, uint8_t& R, uint8_t& G, uint8_t& B) {
        // 标准 BT.601 近似
        int C = (int)Y - 16;
        int D = (int)U - 128;
        int E = (int)V - 128;

        int r = (298 * C + 409 * E + 128) >> 8;
        int g = (298 * C - 100 * D - 208 * E + 128) >> 8;
        int b = (298 * C + 516 * D + 128) >> 8;

        R = clamp_u8(r);
        G = clamp_u8(g);
        B = clamp_u8(b);
    }

    void 配置传感器(rs2::device dev) {
        try {
            for (auto&& s : dev.query_sensors()) {
                // 尝试识别 RGB 传感器
                bool isColor = false;
                try {
                    auto name = s.get_info(RS2_CAMERA_INFO_NAME);
                    if (name && std::string(name).find("RGB") != std::string::npos) isColor = true;
                }
                catch (...) {}

                // 颜色：自动曝光/白平衡
                if (isColor) {
                    if (s.supports(RS2_OPTION_ENABLE_AUTO_EXPOSURE)) {
                        s.set_option(RS2_OPTION_ENABLE_AUTO_EXPOSURE, cfg.彩色_自动曝光 ? 1.0f : 0.0f);
                    }
                    if (s.supports(RS2_OPTION_ENABLE_AUTO_WHITE_BALANCE)) {
                        s.set_option(RS2_OPTION_ENABLE_AUTO_WHITE_BALANCE, cfg.彩色_自动白平衡 ? 1.0f : 0.0f);
                    }

                    if (!cfg.彩色_自动曝光) {
                        if (s.supports(RS2_OPTION_EXPOSURE)) s.set_option(RS2_OPTION_EXPOSURE, cfg.彩色_曝光);
                        if (s.supports(RS2_OPTION_GAIN))     s.set_option(RS2_OPTION_GAIN, cfg.彩色_增益);
                    }
                    if (!cfg.彩色_自动白平衡) {
                        if (s.supports(RS2_OPTION_WHITE_BALANCE)) s.set_option(RS2_OPTION_WHITE_BALANCE, cfg.彩色_白平衡);
                    }
                }

                // 深度：发射器/激光功率（若支持）
                if (s.is<rs2::depth_sensor>()) {
                    if (s.supports(RS2_OPTION_EMITTER_ENABLED)) {
                        s.set_option(RS2_OPTION_EMITTER_ENABLED, cfg.深度_启用发射器 ? 1.0f : 0.0f);
                    }
                    if (s.supports(RS2_OPTION_LASER_POWER)) {
                        // 注意：不同设备范围不同；这里仅在 supports 时设置
                        s.set_option(RS2_OPTION_LASER_POWER, cfg.深度_激光功率);
                    }
                }
            }
        }
        catch (...) {
            // 传感器配置失败不应阻断采集
        }
    }

    void 配置滤波链() {
        auto try_set = [](auto& f, rs2_option opt, float val) {
            try { if (f.supports(opt)) f.set_option(opt, val); }
            catch (...) {}
            };

        // decimation：倍率>1会改变分辨率（默认不启用）
        try_set(降采样滤波, RS2_OPTION_FILTER_MAGNITUDE, cfg.降采样_倍率);

        // spatial
        try_set(空间滤波, RS2_OPTION_FILTER_SMOOTH_ALPHA, cfg.空间_平滑系数);
        try_set(空间滤波, RS2_OPTION_FILTER_SMOOTH_DELTA, cfg.空间_平滑阈值);
        try_set(空间滤波, RS2_OPTION_HOLES_FILL, cfg.空间_孔洞填充);

        // temporal
        try_set(时间滤波, RS2_OPTION_FILTER_SMOOTH_ALPHA, cfg.时间_平滑系数);
        try_set(时间滤波, RS2_OPTION_FILTER_SMOOTH_DELTA, cfg.时间_平滑阈值);
        // 不同版本/设备可能是 PERSISTENCE_CONTROL，也可能复用 HOLES_FILL 等；supports 就会生效
      //  try_set(时间滤波, RS2_OPTION_PERSISTENCE_CONTROL, cfg.时间_持久性);
       

        // hole filling
        try_set(填洞滤波, RS2_OPTION_HOLES_FILL, cfg.填洞_模式);
    }

    void 读取对齐彩色(const rs2::video_frame& color, 结构体_原始场景帧& out) {
        const int w = out.宽度;
        const int h = out.高度;

        const int cw = color.get_width();
        const int ch = color.get_height();
        if (cw != w || ch != h) {
            // 对齐后理论上应一致；不一致就保留默认白色
            return;
        }

        const rs2_format fmt = color.get_profile().format();
        const int bpp = color.get_bytes_per_pixel();
        const int stride = color.get_stride_in_bytes();
        const uint8_t* base = (const uint8_t*)color.get_data();
        if (!base) return;

        auto write_rgb = [&](int u, int v, uint8_t R, uint8_t G, uint8_t B) {
            out.颜色[索引(u, v, w)] = Color{ R, G, B };
            };

        // 常见：RGB8/BGR8/RGBA8/BGRA8
        if (fmt == RS2_FORMAT_RGB8 || fmt == RS2_FORMAT_BGR8 || fmt == RS2_FORMAT_RGBA8 || fmt == RS2_FORMAT_BGRA8) {
            for (int v = 0; v < h; ++v) {
                const uint8_t* row = base + v * stride;
                for (int u = 0; u < w; ++u) {
                    const uint8_t* px = row + u * bpp;
                    switch (fmt) {
                    case RS2_FORMAT_RGB8:  write_rgb(u, v, px[0], px[1], px[2]); break;
                    case RS2_FORMAT_BGR8:  write_rgb(u, v, px[2], px[1], px[0]); break;
                    case RS2_FORMAT_RGBA8: write_rgb(u, v, px[0], px[1], px[2]); break;
                    case RS2_FORMAT_BGRA8: write_rgb(u, v, px[2], px[1], px[0]); break;
                    default: break;
                    }
                }
            }
            return;
        }

        // 兜底：YUYV / UYVY
        if (fmt == RS2_FORMAT_YUYV && bpp == 2) {
            for (int v = 0; v < h; ++v) {
                const uint8_t* row = base + v * stride;
                for (int u = 0; u < w; u += 2) {
                    const uint8_t* p = row + u * 2;
                    // Y0 U Y1 V
                    uint8_t Y0 = p[0], U = p[1], Y1 = p[2], V = p[3];
                    uint8_t R, G, B;
                    yuv_to_rgb(Y0, U, V, R, G, B); write_rgb(u, v, R, G, B);
                    if (u + 1 < w) { yuv_to_rgb(Y1, U, V, R, G, B); write_rgb(u + 1, v, R, G, B); }
                }
            }
            return;
        }

        if (fmt == RS2_FORMAT_UYVY && bpp == 2) {
            for (int v = 0; v < h; ++v) {
                const uint8_t* row = base + v * stride;
                for (int u = 0; u < w; u += 2) {
                    const uint8_t* p = row + u * 2;
                    // U Y0 V Y1
                    uint8_t U = p[0], Y0 = p[1], V = p[2], Y1 = p[3];
                    uint8_t R, G, B;
                    yuv_to_rgb(Y0, U, V, R, G, B); write_rgb(u, v, R, G, B);
                    if (u + 1 < w) { yuv_to_rgb(Y1, U, V, R, G, B); write_rgb(u + 1, v, R, G, B); }
                }
            }
            return;
        }

        // 其他格式暂不处理（保留默认白色）
    }

    void 生成点云(结构体_原始场景帧& 帧) {
        const float fx = 深度内参.fx;
        const float fy = 深度内参.fy;
        const float cx = 深度内参.ppx;
        const float cy = 深度内参.ppy;

        const int w = 帧.宽度;
        const int h = 帧.高度;

        for (int v = 0; v < h; ++v) {
            for (int u = 0; u < w; ++u) {
                const float z = 帧.深度[索引(u, v, w)];
                if (z <= 0.0f) {
                    帧.点云[索引(u, v, w)] = { 0,0,0 };
                    continue;
                }
                const float X = (u - cx) * z / fx;
                const float Y = (v - cy) * z / fy;
                const float Z = z;
                帧.点云[索引(u, v, w)] = Vector3D{ X, Y, Z };
            }
        }
    }
};
