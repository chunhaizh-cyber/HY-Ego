module;
#include <librealsense2/rs.hpp>
#include <iostream>
#include <algorithm>
#include <cstdint>
#include <cmath>
#include <vector>
#include <queue>
#include <limits>
#include <cstring>
#include <string>
#include <optional>
#include <memory>
#include <variant>

export module D455相机模块;

import 相机接口模块;
import 基础数据类型模块;
import 外设模块;

// ============================================================
// D455 相机实现（升级版）
// - 仍然作为“相机驱动层”存在：打开/关闭/采集一帧
// - 新增：
//   1) 可选对齐目标（对齐到深度/对齐到彩色）
//   2) 可选指定彩色格式（避免 RS2_FORMAT_ANY 导致 YUYV）
//   3) 运行时参数设置：支持 外设.指令(设置参数) 直接映射到 RealSense 选项
//   4) 额外输出：保存并可查询最后一帧的“设备时间戳(微秒)”
// ============================================================

export enum class 枚举_D455对齐目标 : std::int8_t {
    对齐到深度 = 0,  // color -> depth（你原逻辑）
    对齐到彩色 = 1,  // depth -> color（更适合做颜色采样/2D轮廓）
};

export enum class 枚举_D455彩色格式 : std::int8_t {
    自动 = 0,
    RGB8,
    BGR8,
    RGBA8,
    BGRA8,
    YUYV,
    UYVY,
};

export struct 结构体_轮廓观测 {
    int 帧内序号 = 0;

    // ROI（在“对齐后的深度/彩色图”坐标系中）
    int x = 0, y = 0, w = 0, h = 0;

    // 轮廓掩膜：按 ROI 原始尺寸存储（0/1），后续“特征值”阶段再按规则缩放/编码
    // size = w*h
    std::vector<std::uint8_t> 掩膜;

    // 轮廓对应的原始彩色裁剪图（与掩膜同尺寸）
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

        枚举_D455对齐目标 对齐目标 = 枚举_D455对齐目标::对齐到深度;
        枚举_D455彩色格式 彩色格式 = 枚举_D455彩色格式::BGR8; // 推荐固定，减少兜底转换

        // ===== 颜色稳定（推荐开启）=====
        bool 彩色_自动曝光 = true;
        bool 彩色_自动白平衡 = true;

        // 若关闭自动，则使用手动值（仅在 supports 时生效）
        double 彩色_曝光 = 8000.0;       // microseconds（范围随设备而变）
        double 彩色_增益 = 64.0;         // 0..?
        double 彩色_白平衡 = 4500.0;     // Kelvin

        // ===== 深度稳定（可选）=====
        bool   深度_启用发射器 = true;
        double 深度_激光功率 = 150.0;    // D455 支持范围随设备而变

        // ===== 深度滤波链（推荐开启空间+时间+填洞）=====
        bool 启用视差域处理 = true;      // Spatial/Temporal 在 disparity 域更稳

        bool   启用空间滤波 = true;
        double 空间_平滑系数 = 0.5;      // RS2_OPTION_FILTER_SMOOTH_ALPHA (0..1)
        double 空间_平滑阈值 = 20.0;     // RS2_OPTION_FILTER_SMOOTH_DELTA
        double 空间_孔洞填充 = 0.0;      // RS2_OPTION_HOLES_FILL (0..2)

        bool   启用时间滤波 = true;
        double 时间_平滑系数 = 0.4;      // RS2_OPTION_FILTER_SMOOTH_ALPHA (0..1)
        double 时间_平滑阈值 = 20.0;     // RS2_OPTION_FILTER_SMOOTH_DELTA

        bool   启用填洞滤波 = true;
        double 填洞_模式 = 1.0;          // RS2_OPTION_HOLES_FILL (0..2)

        // 降采样滤波默认不启用（因为会改变分辨率，容易破坏对齐）
        bool   启用降采样 = false;
        double 降采样_倍率 = 1.0;        // RS2_OPTION_FILTER_MAGNITUDE，>1 会改分辨率（谨慎）

        // ===== 轮廓/存在初筛（前景分割 + 连通域）=====
        bool 启用轮廓提取 = true;

        // 背景深度学习：前 N 帧做平均得到背景；之后按 背景_更新系数 缓慢更新“非前景像素”
        int    背景学习帧数 = 30;
        double 背景_更新系数 = 0.01;     // 0 表示不更新

        // 前景判定：|depth - bg| > 阈值（米）
        double 前景_深度差阈值_m = 0.06;
        double 前景_最小深度_m = 0.15;
        double 前景_最大深度_m = 4.0;

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
    explicit D455_相机实现(配置项 cfg_ = {})
        : cfg(std::move(cfg_)),
        深度到视差(true),
        视差到深度(false) {
    }

    // ===== 新：用外设启动参数直接打开（可把“外设.扩展参数”透传进来）=====
    bool 打开(const 外设启动参数& p) {
        配置项 c = cfg;
        if (p.宽 > 0 && p.高 > 0) {
            c.深度宽 = p.宽; c.深度高 = p.高;
            c.彩色宽 = p.宽; c.彩色高 = p.高;
        }
        if (p.期望FPS > 0) c.帧率 = p.期望FPS;

        // 扩展参数：key/value（可按需继续加）
        for (const auto& kv : p.扩展) {
            应用KV到配置(c, kv.key, kv.val);
        }

        cfg = c;
        return 打开();
    }

    bool 打开() override {
        try {
            rs2::config c;

            c.enable_stream(RS2_STREAM_DEPTH, cfg.深度宽, cfg.深度高, RS2_FORMAT_Z16, cfg.帧率);

            // 优先固定彩色格式（BGR8/RGB8），减少读帧兜底成本；失败则回退 ANY
            rs2_format want = RS2_FORMAT_ANY;
            switch (cfg.彩色格式) {
            case 枚举_D455彩色格式::RGB8:  want = RS2_FORMAT_RGB8;  break;
            case 枚举_D455彩色格式::BGR8:  want = RS2_FORMAT_BGR8;  break;
            case 枚举_D455彩色格式::RGBA8: want = RS2_FORMAT_RGBA8; break;
            case 枚举_D455彩色格式::BGRA8: want = RS2_FORMAT_BGRA8; break;
            case 枚举_D455彩色格式::YUYV:  want = RS2_FORMAT_YUYV;  break;
            case 枚举_D455彩色格式::UYVY:  want = RS2_FORMAT_UYVY;  break;
            default:                         want = RS2_FORMAT_ANY;   break;
            }

            try {
                c.enable_stream(RS2_STREAM_COLOR, cfg.彩色宽, cfg.彩色高, want, cfg.帧率);
                profile = 管道.start(c);
            }
            catch (...) {
                // 回退
                rs2::config c2;
                c2.enable_stream(RS2_STREAM_DEPTH, cfg.深度宽, cfg.深度高, RS2_FORMAT_Z16, cfg.帧率);
                c2.enable_stream(RS2_STREAM_COLOR, cfg.彩色宽, cfg.彩色高, RS2_FORMAT_ANY, cfg.帧率);
                profile = 管道.start(c2);
            }

            已打开 = true;

            // 对齐器（可配置）
            const rs2_stream target = (cfg.对齐目标 == 枚举_D455对齐目标::对齐到彩色) ? RS2_STREAM_COLOR : RS2_STREAM_DEPTH;
            对齐器 = std::make_unique<rs2::align>(target);

            // 深度内参
            auto 深度profile = profile.get_stream(RS2_STREAM_DEPTH).as<rs2::video_stream_profile>();
            深度内参 = 深度profile.get_intrinsics();

            // 深度尺度 + 设备缓存
            device_ = profile.get_device();
            if (auto ds = device_.first<rs2::depth_sensor>(); ds) {
                深度尺度 = ds.get_depth_scale();
            }

            // 缓存传感器句柄，便于运行时设置参数
            缓存传感器(device_);

            // 配置传感器参数（自动曝光/白平衡、发射器等）
            配置传感器(device_);

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

    // 采集一帧：输出“对齐后”的深度/彩色/点云 + (可选)轮廓观测
    bool 采集一帧(结构体_原始场景帧& 输出) override {
        if (!已打开 || !对齐器) return false;

        try {
            rs2::frameset frames = 管道.wait_for_frames();

            // ===== 1) 对齐 =====
            rs2::frameset aligned = 对齐器->process(frames);

            rs2::depth_frame depth = aligned.get_depth_frame();
            rs2::video_frame color = aligned.get_color_frame();
            if (!depth || !color) return false;

            // ===== 2) 设备时间戳（微秒）=====
            // RealSense 的 timestamp 通常是毫秒（double），这里转为“微秒整数”存成员，供外设包头使用
            const double ts_ms = depth.get_timestamp();
            最后设备时间_微秒 = (时间戳)std::llround(ts_ms * 1000.0);

            // ===== 3) 深度滤波链（不改变分辨率的滤波默认安全）=====
            rs2::frame filtered = depth;

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

            const size_t N = (size_t)w * (size_t)h;
            if (输出.深度.size() != N) 输出.深度.resize(N);
            if (输出.颜色.size() != N) 输出.颜色.resize(N);
            if (输出.点云.size() != N) 输出.点云.resize(N);

            // ===== 4) 深度读取（更快：Z16 * depth_scale）=====
            const uint16_t* dp = (const uint16_t*)depth.get_data();
            if (!dp) return false;

            for (size_t i = 0; i < N; ++i) {
                输出.深度[i] = (double)dp[i] * 深度尺度; // 米
            }

            // ===== 5) 彩色读取：支持 RGB/BGR/RGBA/BGRA + YUYV/UYVY 兜底 =====
            // 若读取失败，使用白色填充以避免未初始化
            if (!读取对齐彩色(color, 输出)) {
                std::fill(输出.颜色.begin(), 输出.颜色.end(), Color{ 255,255,255 });
            }

            // ===== 6) 点云生成（相机坐标系）=====
            生成点云(输出);

            // ===== 7) 轮廓初筛（可选）：深度背景差异 -> 前景掩膜 -> 连通域 =====
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
    bool 采集一帧并提取轮廓(结构体_原始场景帧& 输出, std::vector<结构体_轮廓观测>& out轮廓) {
        if (!采集一帧(输出)) return false;
        out轮廓 = 最近轮廓;
        return true;
    }

    const std::vector<结构体_轮廓观测>& 获取最近轮廓观测() const { return 最近轮廓; }

    时间戳 获取最后设备时间_微秒() const { return 最后设备时间_微秒; }

    std::string 获取设备序列号() const {
        try {
            if (device_) {
                auto sn = device_.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER);
                return sn ? std::string(sn) : std::string{};
            }
        }
        catch (...) {}
        return {};
    }

    外设指令回执 处理外设指令(const 外设指令& cmd) {
        if (!已打开) return { false, -1, "D455 未打开" };

        if (cmd.类型 == 枚举_外设指令::设置参数) {
            if (设置运行时参数(cmd.参数名, cmd.参数值_)) {
                return { true, 0, "参数已应用" };
            }
            return { false, -2, "不支持该参数或设置失败" };
        }

        if (cmd.类型 == 枚举_外设指令::自检) {
            const bool ok = (bool)device_;
            return { ok, ok ? 0 : -3, ok ? "自检通过" : "设备句柄无效" };
        }

        if (cmd.类型 == 枚举_外设指令::重置) {
            return { false, -4, "建议在外设层执行：停止->打开" };
        }

        return { false, -5, "该指令由外设层处理或未实现" };
    }

private:
    static inline size_t 索引(int u, int v, int w) {
        return (size_t)v * (size_t)w + (size_t)u;
    }

    bool 已打开 = false;
    配置项 cfg;

    rs2::pipeline 管道;
    rs2::pipeline_profile profile;

    rs2::device device_{};

    std::unique_ptr<rs2::align> 对齐器;

    rs2_intrinsics 深度内参{};
    double 深度尺度 = 0.001;

    rs2::decimation_filter      降采样滤波;
    rs2::disparity_transform    深度到视差;
    rs2::spatial_filter         空间滤波;
    rs2::temporal_filter        时间滤波;
    rs2::disparity_transform    视差到深度;
    rs2::hole_filling_filter    填洞滤波;

    rs2::sensor 颜色传感器_{};
    rs2::sensor 深度传感器_{};
    bool 有颜色传感器_ = false;
    bool 有深度传感器_ = false;

    时间戳 最后设备时间_微秒 = 0;

    std::vector<double> 背景深度;
    std::vector<double> 背景权重;
    bool 背景已建立 = false;
    int  背景累计帧 = 0;

    std::vector<double> 上一帧深度;
    std::vector<结构体_轮廓观测> 最近轮廓;

private:
    static std::optional<double> 取double(const 外设参数值& v) {
        if (std::holds_alternative<double>(v)) return std::get<double>(v);
        if (std::holds_alternative<std::int64_t>(v)) return (double)std::get<std::int64_t>(v);
        if (std::holds_alternative<bool>(v)) return std::get<bool>(v) ? 1.0 : 0.0;
        return std::nullopt;
    }

    static std::optional<bool> 取bool(const 外设参数值& v) {
        if (std::holds_alternative<bool>(v)) return std::get<bool>(v);
        if (std::holds_alternative<std::int64_t>(v)) return std::get<std::int64_t>(v) != 0;
        if (std::holds_alternative<double>(v)) return std::get<double>(v) != 0.0;
        return std::nullopt;
    }

    static void 应用KV到配置(配置项& c, const std::string& key, const 外设参数值& val) {
        if (key == "align_to_color") {
            if (auto b = 取bool(val); b && *b) c.对齐目标 = 枚举_D455对齐目标::对齐到彩色;
            else c.对齐目标 = 枚举_D455对齐目标::对齐到深度;
        }
        else if (key == "color_auto_exposure") {
            if (auto b = 取bool(val)) c.彩色_自动曝光 = *b;
        }
        else if (key == "color_auto_white_balance") {
            if (auto b = 取bool(val)) c.彩色_自动白平衡 = *b;
        }
        else if (key == "color_exposure") {
            if (auto d = 取double(val)) c.彩色_曝光 = *d;
        }
        else if (key == "color_gain") {
            if (auto d = 取double(val)) c.彩色_增益 = *d;
        }
        else if (key == "color_white_balance") {
            if (auto d = 取double(val)) c.彩色_白平衡 = *d;
        }
        else if (key == "depth_emitter") {
            if (auto b = 取bool(val)) c.深度_启用发射器 = *b;
        }
        else if (key == "depth_laser_power") {
            if (auto d = 取double(val)) c.深度_激光功率 = *d;
        }
    }

    void 缓存传感器(const rs2::device& dev) {
        有颜色传感器_ = false;
        有深度传感器_ = false;
        try {
            for (auto&& s : dev.query_sensors()) {
                bool isColor = false;
                try {
                    auto name = s.get_info(RS2_CAMERA_INFO_NAME);
                    if (name && std::string(name).find("RGB") != std::string::npos) isColor = true;
                }
                catch (...) {}

                if (isColor && !有颜色传感器_) {
                    颜色传感器_ = s;
                    有颜色传感器_ = true;
                }
                if (s.is<rs2::depth_sensor>() && !有深度传感器_) {
                    深度传感器_ = s;
                    有深度传感器_ = true;
                }
            }
        }
        catch (...) {}
    }

    void 配置传感器(const rs2::device&) {
        auto try_set = [](rs2::sensor& s, rs2_option opt, double val) {
            try { if (s.supports(opt)) s.set_option(opt, (float)val); }
            catch (...) {}
            };

        if (有颜色传感器_) {
            rs2::sensor s = 颜色传感器_;
            try_set(s, RS2_OPTION_ENABLE_AUTO_EXPOSURE, cfg.彩色_自动曝光 ? 1.0 : 0.0);
            try_set(s, RS2_OPTION_ENABLE_AUTO_WHITE_BALANCE, cfg.彩色_自动白平衡 ? 1.0 : 0.0);

            if (!cfg.彩色_自动曝光) {
                try_set(s, RS2_OPTION_EXPOSURE, cfg.彩色_曝光);
                try_set(s, RS2_OPTION_GAIN, cfg.彩色_增益);
            }
            if (!cfg.彩色_自动白平衡) {
                try_set(s, RS2_OPTION_WHITE_BALANCE, cfg.彩色_白平衡);
            }
        }

        if (有深度传感器_) {
            rs2::sensor s = 深度传感器_;
            try_set(s, RS2_OPTION_EMITTER_ENABLED, cfg.深度_启用发射器 ? 1.0 : 0.0);
            try_set(s, RS2_OPTION_LASER_POWER, cfg.深度_激光功率);
        }
    }

    void 配置滤波链() {
        auto try_set = [](auto& f, rs2_option opt, double val) {
            try { if (f.supports(opt)) f.set_option(opt, (float)val); }
            catch (...) {}
            };

        try_set(降采样滤波, RS2_OPTION_FILTER_MAGNITUDE, cfg.降采样_倍率);

        try_set(空间滤波, RS2_OPTION_FILTER_SMOOTH_ALPHA, cfg.空间_平滑系数);
        try_set(空间滤波, RS2_OPTION_FILTER_SMOOTH_DELTA, cfg.空间_平滑阈值);
        try_set(空间滤波, RS2_OPTION_HOLES_FILL, cfg.空间_孔洞填充);

        try_set(时间滤波, RS2_OPTION_FILTER_SMOOTH_ALPHA, cfg.时间_平滑系数);
        try_set(时间滤波, RS2_OPTION_FILTER_SMOOTH_DELTA, cfg.时间_平滑阈值);

        try_set(填洞滤波, RS2_OPTION_HOLES_FILL, cfg.填洞_模式);
    }

    bool 设置运行时参数(const std::string& key, const 外设参数值& v) {
        auto try_set_sensor = [](rs2::sensor& s, rs2_option opt, double val) -> bool {
            try { if (s.supports(opt)) { s.set_option(opt, (float)val); return true; } }
            catch (...) {}
            return false;
            };

        // 颜色
        if (key == "color_auto_exposure") {
            if (!有颜色传感器_) return false;
            auto b = 取bool(v); if (!b) return false;
            cfg.彩色_自动曝光 = *b;
            return try_set_sensor(颜色传感器_, RS2_OPTION_ENABLE_AUTO_EXPOSURE, *b ? 1.0 : 0.0);
        }
        if (key == "color_auto_white_balance") {
            if (!有颜色传感器_) return false;
            auto b = 取bool(v); if (!b) return false;
            cfg.彩色_自动白平衡 = *b;
            return try_set_sensor(颜色传感器_, RS2_OPTION_ENABLE_AUTO_WHITE_BALANCE, *b ? 1.0 : 0.0);
        }
        if (key == "color_exposure") {
            if (!有颜色传感器_) return false;
            auto d = 取double(v); if (!d) return false;
            cfg.彩色_曝光 = *d;
            cfg.彩色_自动曝光 = false;
            try_set_sensor(颜色传感器_, RS2_OPTION_ENABLE_AUTO_EXPOSURE, 0.0);
            return try_set_sensor(颜色传感器_, RS2_OPTION_EXPOSURE, *d);
        }
        if (key == "color_gain") {
            if (!有颜色传感器_) return false;
            auto d = 取double(v); if (!d) return false;
            cfg.彩色_增益 = *d;
            cfg.彩色_自动曝光 = false;
            try_set_sensor(颜色传感器_, RS2_OPTION_ENABLE_AUTO_EXPOSURE, 0.0);
            return try_set_sensor(颜色传感器_, RS2_OPTION_GAIN, *d);
        }
        if (key == "color_white_balance") {
            if (!有颜色传感器_) return false;
            auto d = 取double(v); if (!d) return false;
            cfg.彩色_白平衡 = *d;
            cfg.彩色_自动白平衡 = false;
            try_set_sensor(颜色传感器_, RS2_OPTION_ENABLE_AUTO_WHITE_BALANCE, 0.0);
            return try_set_sensor(颜色传感器_, RS2_OPTION_WHITE_BALANCE, *d);
        }

        // 深度
        if (key == "depth_emitter") {
            if (!有深度传感器_) return false;
            auto b = 取bool(v); if (!b) return false;
            cfg.深度_启用发射器 = *b;
            return try_set_sensor(深度传感器_, RS2_OPTION_EMITTER_ENABLED, *b ? 1.0 : 0.0);
        }
        if (key == "depth_laser_power") {
            if (!有深度传感器_) return false;
            auto d = 取double(v); if (!d) return false;
            cfg.深度_激光功率 = *d;
            return try_set_sensor(深度传感器_, RS2_OPTION_LASER_POWER, *d);
        }

        // 滤波链（动态调参）
        auto try_set_filter = [&](const std::string& k, auto& f, rs2_option opt, double& cache) -> bool {
            if (k != key) return false;
            auto d = 取double(v); if (!d) return false;
            cache = *d;
            try { if (f.supports(opt)) { f.set_option(opt, (float)*d); return true; } }
            catch (...) {}
            return false;
            };

        if (try_set_filter("spatial_alpha", 空间滤波, RS2_OPTION_FILTER_SMOOTH_ALPHA, cfg.空间_平滑系数)) return true;
        if (try_set_filter("spatial_delta", 空间滤波, RS2_OPTION_FILTER_SMOOTH_DELTA, cfg.空间_平滑阈值)) return true;
        if (try_set_filter("spatial_holes", 空间滤波, RS2_OPTION_HOLES_FILL, cfg.空间_孔洞填充)) return true;

        if (try_set_filter("temporal_alpha", 时间滤波, RS2_OPTION_FILTER_SMOOTH_ALPHA, cfg.时间_平滑系数)) return true;
        if (try_set_filter("temporal_delta", 时间滤波, RS2_OPTION_FILTER_SMOOTH_DELTA, cfg.时间_平滑阈值)) return true;

        if (try_set_filter("holes_fill", 填洞滤波, RS2_OPTION_HOLES_FILL, cfg.填洞_模式)) return true;

        return false;
    }

    bool 读取对齐彩色(const rs2::video_frame& color, 结构体_原始场景帧& out) {
        const int w = out.宽度;
        const int h = out.高度;

        const int cw = color.get_width();
        const int ch = color.get_height();
        if (cw != w || ch != h) return false;

        const rs2_format fmt = color.get_profile().format();
        const int bpp = color.get_bytes_per_pixel();
        const int stride = color.get_stride_in_bytes();
        const uint8_t* base = (const uint8_t*)color.get_data();
        if (!base) return false;

        auto write_rgb = [&](int u, int v, uint8_t R, uint8_t G, uint8_t B) {
            out.颜色[索引(u, v, w)] = Color{ R, G, B };
            };

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
            return true;
        }

        if (fmt == RS2_FORMAT_YUYV && bpp == 2) {
            for (int v = 0; v < h; ++v) {
                const uint8_t* row = base + v * stride;
                for (int u = 0; u < w; u += 2) {
                    const uint8_t* p = row + u * 2;
                    uint8_t Y0 = p[0], U = p[1], Y1 = p[2], V = p[3];
                    uint8_t R, G, B;
                    yuv_to_rgb(Y0, U, V, R, G, B); write_rgb(u, v, R, G, B);
                    if (u + 1 < w) { yuv_to_rgb(Y1, U, V, R, G, B); write_rgb(u + 1, v, R, G, B); }
                }
            }
            return true;
        }

        if (fmt == RS2_FORMAT_UYVY && bpp == 2) {
            for (int v = 0; v < h; ++v) {
                const uint8_t* row = base + v * stride;
                for (int u = 0; u < w; u += 2) {
                    const uint8_t* p = row + u * 2;
                    uint8_t U = p[0], Y0 = p[1], V = p[2], Y1 = p[3];
                    uint8_t R, G, B;
                    yuv_to_rgb(Y0, U, V, R, G, B); write_rgb(u, v, R, G, B);
                    if (u + 1 < w) { yuv_to_rgb(Y1, U, V, R, G, B); write_rgb(u + 1, v, R, G, B); }
                }
            }
            return true;
        }

        return false;
    }

    void 生成点云(结构体_原始场景帧& 帧) {
        const double fx = 深度内参.fx;
        const double fy = 深度内参.fy;
        const double cx = 深度内参.ppx;
        const double cy = 深度内参.ppy;

        const int w = 帧.宽度;
        const int h = 帧.高度;

        for (int v = 0; v < h; ++v) {
            for (int u = 0; u < w; ++u) {
                const double z = 帧.深度[索引(u, v, w)];
                if (z <= 0.0) {
                    帧.点云[索引(u, v, w)] = { 0,0,0 };
                    continue;
                }
                const double X = (u - cx) * z / fx;
                const double Y = (v - cy) * z / fy;
                const double Z = z;
                帧.点云[索引(u, v, w)] = Vector3D{ X, Y, Z };
            }
        }
    }

    static inline uint8_t clamp_u8(int x) {
        if (x < 0) return 0;
        if (x > 255) return 255;
        return (uint8_t)x;
    }

    static inline void yuv_to_rgb(uint8_t Y, uint8_t U, uint8_t V, uint8_t& R, uint8_t& G, uint8_t& B) {
        int C = (int)Y - 16;
        int D = (int)U - 128;
        int E = (int)V - 128;
        int r = (298 * C + 409 * E + 128) >> 8;
        int g = (298 * C - 100 * D - 208 * E + 128) >> 8;
        int b = (298 * C + 516 * D + 128) >> 8;
        R = clamp_u8(r); G = clamp_u8(g); B = clamp_u8(b);
    }

    static inline bool 在范围内(int x, int a, int b) { return x >= a && x <= b; }

    static void 形态学_闭运算(std::vector<std::uint8_t>& m, int w, int h, int r) {
        if (r <= 0) return;
        std::vector<std::uint8_t> tmp((size_t)w * (size_t)h, 0);
        auto idx = [&](int u, int v) { return (size_t)v * (size_t)w + (size_t)u; };

        for (int v = 0; v < h; ++v) for (int u = 0; u < w; ++u) {
            bool on = false;
            for (int dv = -r; dv <= r && !on; ++dv) {
                int y = v + dv; if (!在范围内(y, 0, h - 1)) continue;
                for (int du = -r; du <= r; ++du) {
                    int x = u + du; if (!在范围内(x, 0, w - 1)) continue;
                    if (m[idx(x, y)] != 0) { on = true; break; }
                }
            }
            tmp[idx(u, v)] = on ? 1 : 0;
        }

        for (int v = 0; v < h; ++v) for (int u = 0; u < w; ++u) {
            bool on = true;
            for (int dv = -r; dv <= r && on; ++dv) {
                int y = v + dv; if (!在范围内(y, 0, h - 1)) { on = false; break; }
                for (int du = -r; du <= r; ++du) {
                    int x = u + du; if (!在范围内(x, 0, w - 1)) { on = false; break; }
                    if (tmp[idx(x, y)] == 0) { on = false; break; }
                }
            }
            m[idx(u, v)] = on ? 1 : 0;
        }
    }

    void 更新背景模型(const 结构体_原始场景帧& 帧, const std::vector<std::uint8_t>* 前景掩膜) {
        const int w = 帧.宽度, h = 帧.高度;
        const size_t N = (size_t)w * (size_t)h;

        if (背景深度.size() != N) {
            背景深度.assign(N, 0.0);
            背景权重.assign(N, 0.0);
            背景已建立 = false;
            背景累计帧 = 0;
        }

        if (!背景已建立) {
            for (size_t i = 0; i < N; ++i) {
                double z = 帧.深度[i];
                if (z <= 0.0) continue;
                double wgt = 背景权重[i];
                背景深度[i] = (背景深度[i] * wgt + z) / (wgt + 1.0);
                背景权重[i] = wgt + 1.0;
            }
            背景累计帧++;
            if (背景累计帧 >= std::max(1, cfg.背景学习帧数)) 背景已建立 = true;
            return;
        }

        const double a = std::max(0.0, std::min(1.0, cfg.背景_更新系数));
        if (a <= 0.0) return;

        for (size_t i = 0; i < N; ++i) {
            if (前景掩膜 && (*前景掩膜)[i]) continue;
            double z = 帧.深度[i];
            if (z <= 0.0) continue;
            double bg = 背景深度[i];
            if (bg <= 0.0) 背景深度[i] = z;
            else 背景深度[i] = bg * (1.0 - a) + z * a;
        }
    }

    void 生成前景掩膜(const 结构体_原始场景帧& 帧, std::vector<std::uint8_t>& outMask) {
        const int w = 帧.宽度, h = 帧.高度;
        const size_t N = (size_t)w * (size_t)h;
        outMask.assign(N, 0);

        const double minZ = cfg.前景_最小深度_m;
        const double maxZ = cfg.前景_最大深度_m;
        const double th = cfg.前景_深度差阈值_m;

        const bool canUseBG = 背景已建立 && 背景深度.size() == N;
        const bool canUsePrev = (!canUseBG) && 上一帧深度.size() == N;

        for (size_t i = 0; i < N; ++i) {
            double z = 帧.深度[i];
            if (!(z > 0.0 && z >= minZ && z <= maxZ)) continue;

            double ref = 0.0;
            if (canUseBG) ref = 背景深度[i];
            else if (canUsePrev) ref = 上一帧深度[i];

            bool fg = false;
            if (ref <= 0.0) fg = canUseBG;
            else fg = std::fabs(z - ref) > th;

            outMask[i] = fg ? 1 : 0;
        }

        形态学_闭运算(outMask, w, h, cfg.轮廓_形态学半径);

        if (canUseBG) 更新背景模型(帧, &outMask);
        else 更新背景模型(帧, nullptr);

        上一帧深度 = 帧.深度;
    }

    void 提取轮廓(const 结构体_原始场景帧& 帧, std::vector<结构体_轮廓观测>& out) {
        out.clear();

        const int w = 帧.宽度, h = 帧.高度;
        if (w <= 0 || h <= 0) return;

        std::vector<std::uint8_t> mask;
        生成前景掩膜(帧, mask);

        const size_t N = (size_t)w * (size_t)h;
        std::vector<std::uint8_t> vis(N, 0);

        auto idx = [&](int u, int v) { return (size_t)v * (size_t)w + (size_t)u; };

        std::queue<int> q;
        int found = 0;
        int seq = 0;

        for (int v0 = 0; v0 < h; ++v0) for (int u0 = 0; u0 < w; ++u0) {
            const size_t i0 = idx(u0, v0);
            if (mask[i0] == 0 || vis[i0]) continue;

            int minx = u0, maxx = u0, miny = v0, maxy = v0;
            int count = 0;

            double minX = std::numeric_limits<double>::infinity();
            double minY = std::numeric_limits<double>::infinity();
            double minZ = std::numeric_limits<double>::infinity();
            double maxX = -std::numeric_limits<double>::infinity();
            double maxY = -std::numeric_limits<double>::infinity();
            double maxZ = -std::numeric_limits<double>::infinity();

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

                minx = std::min(minx, u);
                maxx = std::max(maxx, u);
                miny = std::min(miny, v);
                maxy = std::max(maxy, v);

                const auto& P = 帧.点云[(size_t)i];
                if (P.z > 0.0) {
                    minX = std::min(minX, (double)P.x);
                    minY = std::min(minY, (double)P.y);
                    minZ = std::min(minZ, (double)P.z);
                    maxX = std::max(maxX, (double)P.x);
                    maxY = std::max(maxY, (double)P.y);
                    maxZ = std::max(maxZ, (double)P.z);
                }

                const auto& C = 帧.颜色[(size_t)i];
                sumR += C.r; sumG += C.g; sumB += C.b;

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
            if (!(std::isfinite(minX) && std::isfinite(maxX) && std::isfinite(minZ) && std::isfinite(maxZ))) continue;

            结构体_轮廓观测 obs;
            obs.帧内序号 = seq++;
            obs.x = minx; obs.y = miny;
            obs.w = maxx - minx + 1;
            obs.h = maxy - miny + 1;
            obs.像素数 = count;

            obs.中心 = Vector3D{ (minX + maxX) * 0.5, (minY + maxY) * 0.5, (minZ + maxZ) * 0.5 };
            obs.尺寸 = Vector3D{ std::max(0.0, maxX - minX), std::max(0.0, maxY - minY), std::max(0.0, maxZ - minZ) };

            const int denom = std::max(1, count);
            obs.平均颜色 = Color{
                (std::uint8_t)std::min<std::uint64_t>(255, sumR / (std::uint64_t)denom),
                (std::uint8_t)std::min<std::uint64_t>(255, sumG / (std::uint64_t)denom),
                (std::uint8_t)std::min<std::uint64_t>(255, sumB / (std::uint64_t)denom)
            };

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
                auto ridx = [&](int ru, int rv) { return (size_t)rv * (size_t)rw + (size_t)ru; };
                for (int rv = 0; rv < rh; ++rv) for (int ru = 0; ru < rw; ++ru) {
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

            out.push_back(std::move(obs));
            found++;
            if (found >= std::max(1, cfg.轮廓_最大数量)) return;
        }
    }
};
