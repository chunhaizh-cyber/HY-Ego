export module D455相机模块;

import 相机接口模块;
import 基础数据类型模块 ;
import <librealsense2/rs.hpp>;
import <iostream>;
import <algorithm>;
import <cstdint>;
import <cmath>;
import <vector>;
import <queue>;
import <limits>;
import <cstring>;


export struct 结构体_轮廓观测 {
    int 帧内序号 = 0;

    // ROI（在“对齐后的深度/彩色图”坐标系中）
    int x = 0, y = 0, w = 0, h = 0;

    // 轮廓掩膜：按 ROI 原始尺寸存储（0/1），后续“特征值”阶段再按规则缩放/编码
    // size = w*h
    std::vector<std::uint8_t> 掩膜;

    // 轮廓对应的原始彩色裁剪图（与掩膜同尺寸），用于后续继续提取“子存在/子轮廓”
    // size = w*h
    std::vector<Color> 裁剪颜色;

    // 可选：边界点（全图坐标），打包为 (y<<32 | x)
    std::vector<std::int64_t> 边界点;

    // 统计信息（相机坐标系，单位：米）
    Vector3D 中心{ 0,0,0 };
    Vector3D 尺寸{ 0,0,0 };
    Color    平均颜色{ 255,255,255 };
    int      像素数 = 0;
};

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

        // ===== 轮廓/存在初筛（前景分割 + 连通域）=====
        bool 启用轮廓提取 = true;

        // 背景深度学习：前 N 帧做平均得到背景；之后按 背景_更新系数 缓慢更新“非前景像素”
        int   背景学习帧数 = 30;
        float 背景_更新系数 = 0.01f;      // 0 表示不更新

        // 前景判定：|depth - bg| > 阈值（米）
        float 前景_深度差阈值_m = 0.06f;
        float 前景_最小深度_m = 0.15f;
        float 前景_最大深度_m = 4.0f;

        // 连通域筛选
        int 轮廓_最小像素数 = 400;
        int 轮廓_最大数量 = 32;

        // 形态学（简单填洞/去噪），0 表示关闭；1~2 通常足够
        int 轮廓_形态学半径 = 1;

        // 输出内容开关
        bool 轮廓_输出边界点 = false;
        bool 轮廓_输出裁剪颜色 = true;
        bool 轮廓_输出原始掩膜 = true;
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

            // ===== 6) 轮廓初筛（可选）：深度背景差异 -> 前景掩膜 -> 连通域（轮廓ROI/掩膜/裁剪图）=====
            if (cfg.启用轮廓提取) {
                最近轮廓.clear();
                提取轮廓(输出, 最近轮廓);

            }

            return true;

        }
        catch (const rs2::error& e) {
            std::cerr << "采集一帧失败: " << e.what() << std::endl;
            return false;
        }
    }


public:
    // 便捷接口：直接返回本帧提取到的轮廓（若 cfg.启用轮廓提取=false，则返回空）
    bool 采集一帧并提取轮廓(结构体_原始场景帧& 输出, std::vector<结构体_轮廓观测>& out轮廓) {
        if (!采集一帧(输出)) return false;
        out轮廓 = 最近轮廓;
        return true;
    }

    const std::vector<结构体_轮廓观测>& 获取最近轮廓观测() const { return 最近轮廓; }

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
private:
    // ===== 轮廓提取状态 =====
    std::vector<float> 背景深度;       // size = w*h
    std::vector<float> 背景权重;       // size = w*h（用于学习期平均）
    bool 背景已建立 = false;
    int  背景累计帧 = 0;

    std::vector<float> 上一帧深度;     // size = w*h（背景未建立时可用作“帧差”兜底）
    std::vector<结构体_轮廓观测> 最近轮廓;

private:
    static inline bool 在范围内(int x, int a, int b) { return x >= a && x <= b; }

    // 简单的 3x3 / 5x5 形态学：用于填洞/去毛刺（r=0 关闭）
    static void 形态学_闭运算(std::vector<std::uint8_t>& m, int w, int h, int r) {
        if (r <= 0) return;
        std::vector<std::uint8_t> tmp((size_t)w * (size_t)h, 0);

        auto idx = [&](int u, int v) { return (size_t)v * (size_t)w + (size_t)u; };

        // dilation
        for (int v = 0; v < h; ++v) {
            for (int u = 0; u < w; ++u) {
                bool on = false;
                for (int dv = -r; dv <= r && !on; ++dv) {
                    int y = v + dv;
                    if (!在范围内(y, 0, h - 1)) continue;
                    for (int du = -r; du <= r; ++du) {
                        int x = u + du;
                        if (!在范围内(x, 0, w - 1)) continue;
                        if (m[idx(x, y)] != 0) { on = true; break; }
                    }
                }
                tmp[idx(u, v)] = on ? 1 : 0;
            }
        }

        // erosion on tmp -> m
        for (int v = 0; v < h; ++v) {
            for (int u = 0; u < w; ++u) {
                bool on = true;
                for (int dv = -r; dv <= r && on; ++dv) {
                    int y = v + dv;
                    if (!在范围内(y, 0, h - 1)) { on = false; break; }
                    for (int du = -r; du <= r; ++du) {
                        int x = u + du;
                        if (!在范围内(x, 0, w - 1)) { on = false; break; }
                        if (tmp[idx(x, y)] == 0) { on = false; break; }
                    }
                }
                m[idx(u, v)] = on ? 1 : 0;
            }
        }
    }

    void 更新背景模型(const 结构体_原始场景帧& 帧, const std::vector<std::uint8_t>* 前景掩膜) {
        const int w = 帧.宽度, h = 帧.高度;
        const size_t N = (size_t)w * (size_t)h;

        if (背景深度.size() != N) {
            背景深度.assign(N, 0.0f);
            背景权重.assign(N, 0.0f);
            背景已建立 = false;
            背景累计帧 = 0;
        }

        // 学习期：做逐像素平均
        if (!背景已建立) {
            for (size_t i = 0; i < N; ++i) {
                float z = 帧.深度[i];
                if (z <= 0.0f) continue;
                float wgt = 背景权重[i];
                背景深度[i] = (背景深度[i] * wgt + z) / (wgt + 1.0f);
                背景权重[i] = wgt + 1.0f;
            }
            背景累计帧++;
            if (背景累计帧 >= std::max(1, cfg.背景学习帧数)) {
                背景已建立 = true;
            }
            return;
        }

        // 运行期：缓慢更新“非前景像素”的背景
        const float a = std::max(0.0f, std::min(1.0f, cfg.背景_更新系数));
        if (a <= 0.0f) return;

        for (size_t i = 0; i < N; ++i) {
            if (前景掩膜 && (*前景掩膜)[i]) continue; // 前景不更新
            float z = 帧.深度[i];
            if (z <= 0.0f) continue;
            float bg = 背景深度[i];
            if (bg <= 0.0f) {
                背景深度[i] = z;
            }
            else {
                背景深度[i] = bg * (1.0f - a) + z * a;
            }
        }
    }

    void 生成前景掩膜(const 结构体_原始场景帧& 帧, std::vector<std::uint8_t>& outMask) {
        const int w = 帧.宽度, h = 帧.高度;
        const size_t N = (size_t)w * (size_t)h;
        outMask.assign(N, 0);

        const float minZ = cfg.前景_最小深度_m;
        const float maxZ = cfg.前景_最大深度_m;
        const float th = cfg.前景_深度差阈值_m;

        const bool canUseBG = 背景已建立 && 背景深度.size() == N;
        const bool canUsePrev = (!canUseBG) && 上一帧深度.size() == N;

        for (size_t i = 0; i < N; ++i) {
            float z = 帧.深度[i];
            if (!(z > 0.0f && z >= minZ && z <= maxZ)) continue;

            float ref = 0.0f;
            if (canUseBG) ref = 背景深度[i];
            else if (canUsePrev) ref = 上一帧深度[i];

            bool fg = false;
            if (ref <= 0.0f) {
                // 背景缺失：背景模式下把“新出现深度”视为前景；帧差模式下避免全屏抖动，默认不算前景
                fg = canUseBG;
            }
            else {
                fg = std::fabs(z - ref) > th;
            }

            outMask[i] = fg ? 1 : 0;
        }

        // 简单形态学（默认 r=1）：填洞 + 去毛刺
        形态学_闭运算(outMask, w, h, cfg.轮廓_形态学半径);

        // 运行期背景更新（只用“非前景像素”）
        if (canUseBG) {
            更新背景模型(帧, &outMask);
        }
        else {
            // 背景学习期：先学习背景
            更新背景模型(帧, nullptr);
        }

        // 更新上一帧（供背景未建立时兜底）
        上一帧深度 = 帧.深度;
    }

    void 提取轮廓(const 结构体_原始场景帧& 帧, std::vector<结构体_轮廓观测>& out) {
        out.clear();

        const int w = 帧.宽度, h = 帧.高度;
        if (w <= 0 || h <= 0) return;

        // 1) 前景掩膜（深度背景差异）
        std::vector<std::uint8_t> mask;
        生成前景掩膜(帧, mask);

        // 2) 连通域提取
        const size_t N = (size_t)w * (size_t)h;
        std::vector<std::uint8_t> vis(N, 0);

        auto idx = [&](int u, int v) { return (size_t)v * (size_t)w + (size_t)u; };

        std::queue<int> q;
        int found = 0;
        int seq = 0;

        for (int v0 = 0; v0 < h; ++v0) {
            for (int u0 = 0; u0 < w; ++u0) {
                const size_t i0 = idx(u0, v0);
                if (mask[i0] == 0 || vis[i0]) continue;

                // BFS
                int minx = u0, maxx = u0, miny = v0, maxy = v0;
                int count = 0;

                float minX = std::numeric_limits<float>::infinity();
                float minY = std::numeric_limits<float>::infinity();
                float minZ = std::numeric_limits<float>::infinity();
                float maxX = -std::numeric_limits<float>::infinity();
                float maxY = -std::numeric_limits<float>::infinity();
                float maxZ = -std::numeric_limits<float>::infinity();

                std::uint64_t sumR = 0, sumG = 0, sumB = 0;
                std::vector<int> pixels;
                pixels.reserve(2048);

                q.push((int)i0);
                vis[i0] = 1;

                while (!q.empty()) {
                    int i = q.front(); q.pop();
                    int u = i % w;
                    int v = i / w;

                    ++count;
                    pixels.push_back(i);

                    if (u < minx) minx = u;
                    if (u > maxx) maxx = u;
                    if (v < miny) miny = v;
                    if (v > maxy) maxy = v;

                    // 3D bbox + avg color
                    const auto& P = 帧.点云[(size_t)i];
                    if (P.z > 0.0f) {
                        if (P.x < minX) minX = (float)P.x;
                        if (P.y < minY) minY = (float)P.y;
                        if (P.z < minZ) minZ = (float)P.z;
                        if (P.x > maxX) maxX = (float)P.x;
                        if (P.y > maxY) maxY = (float)P.y;
                        if (P.z > maxZ) maxZ = (float)P.z;
                    }

                    const auto& C = 帧.颜色[(size_t)i];
                    sumR += C.r;
                    sumG += C.g;
                    sumB += C.b;

                    // 4-neighbor
                    auto try_push = [&](int nu, int nv) {
                        if (!在范围内(nu, 0, w - 1) || !在范围内(nv, 0, h - 1)) return;
                        size_t ni = idx(nu, nv);
                        if (vis[ni]) return;
                        if (mask[ni] == 0) return;
                        vis[ni] = 1;
                        q.push((int)ni);
                        };
                    try_push(u - 1, v);
                    try_push(u + 1, v);
                    try_push(u, v - 1);
                    try_push(u, v + 1);
                }

                if (count < std::max(1, cfg.轮廓_最小像素数)) continue;

                if (!(std::isfinite(minX) && std::isfinite(maxX) && std::isfinite(minZ) && std::isfinite(maxZ))) {
                    continue;
                }

                结构体_轮廓观测 obs;
                obs.帧内序号 = seq++;
                obs.x = minx;
                obs.y = miny;
                obs.w = maxx - minx + 1;
                obs.h = maxy - miny + 1;
                obs.像素数 = count;

                obs.中心 = Vector3D{ (minX + maxX) * 0.5f, (minY + maxY) * 0.5f, (minZ + maxZ) * 0.5f };
                obs.尺寸 = Vector3D{ std::max(0.0f, maxX - minX), std::max(0.0f, maxY - minY), std::max(0.0f, maxZ - minZ) };

                const int denom = std::max(1, count);
                obs.平均颜色 = Color{
                    (std::uint8_t)std::min<std::uint64_t>(255, sumR / (std::uint64_t)denom),
                    (std::uint8_t)std::min<std::uint64_t>(255, sumG / (std::uint64_t)denom),
                    (std::uint8_t)std::min<std::uint64_t>(255, sumB / (std::uint64_t)denom)
                };

                // ROI 掩膜/裁剪图（按原始尺寸存储）
                const int rw = obs.w;
                const int rh = obs.h;

                if (cfg.轮廓_输出原始掩膜) {
                    obs.掩膜.assign((size_t)rw * (size_t)rh, 0);
                    for (int pi : pixels) {
                        int u = pi % w;
                        int v = pi / w;
                        int ru = u - obs.x;
                        int rv = v - obs.y;
                        obs.掩膜[(size_t)rv * (size_t)rw + (size_t)ru] = 1;
                    }
                }

                if (cfg.轮廓_输出裁剪颜色) {
                    obs.裁剪颜色.assign((size_t)rw * (size_t)rh, Color{ 0,0,0 });
                    for (int rv = 0; rv < rh; ++rv) {
                        int v = obs.y + rv;
                        for (int ru = 0; ru < rw; ++ru) {
                            int u = obs.x + ru;
                            obs.裁剪颜色[(size_t)rv * (size_t)rw + (size_t)ru] = 帧.颜色[(size_t)v * (size_t)w + (size_t)u];
                        }
                    }
                }

                if (cfg.轮廓_输出边界点 && cfg.轮廓_输出原始掩膜) {
                    // 用 ROI 掩膜判定边界（4邻域）
                    auto ridx = [&](int ru, int rv) { return (size_t)rv * (size_t)rw + (size_t)ru; };
                    for (int rv = 0; rv < rh; ++rv) {
                        for (int ru = 0; ru < rw; ++ru) {
                            if (obs.掩膜[ridx(ru, rv)] == 0) continue;
                            bool edge =
                                (ru == 0 || obs.掩膜[ridx(ru - 1, rv)] == 0) ||
                                (ru == rw - 1 || obs.掩膜[ridx(ru + 1, rv)] == 0) ||
                                (rv == 0 || obs.掩膜[ridx(ru, rv - 1)] == 0) ||
                                (rv == rh - 1 || obs.掩膜[ridx(ru, rv + 1)] == 0);
                            if (edge) {
                                int u = obs.x + ru;
                                int v = obs.y + rv;
                                obs.边界点.push_back((std::int64_t)((std::uint64_t)(std::uint32_t)v << 32) | (std::uint32_t)u);
                            }
                        }
                    }
                }

                out.push_back(std::move(obs));
                found++;
                if (found >= std::max(1, cfg.轮廓_最大数量)) {
                    return;
                }
            }
        }
    }

};
