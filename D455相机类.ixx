module;
#include <windows.h>
#include <librealsense2/rs.hpp>
#include <iostream>
#include <algorithm>
#include <cstdint>
#include <cmath>
#include <vector>
#include <queue>
#include <deque>
#include <limits>
#include <cstring>
#include <string>
#include <optional>
#include <memory>
#include <variant>
#include <chrono>


export module D455相机模块;

import 相机接口模块;
import 基础数据类型模块;
import 外设模块;
import 日志模块;
// ============================================================
// D455 相机实现（升级版）
// - 作为“相机驱动层”存在：打开/关闭/采集一帧
// - 本文件保留原有功能（轮廓提取仍可用于调试），但工程推荐下游使用点簇分割/存在提取
// - IMU 约定：本工程将 IMU 视为必需输入（gyro+accel）；若设备不支持则打开失败
//
// ============================================================
// 【输出契约 v1.1（工程约定，建议长期保持一致）】
// 目标：让“一帧”可复现、可同步、可验收。
// 0) 单位与坐标系：
//   - 深度/点云/几何：mm；像素坐标：px
//   - 默认输出为“深度已对齐到彩色”(align-to-color)，因此(u,v)坐标系以彩色为准
//   - 点云为相机坐标系(mm)
// 1) 时间戳（建议在 结构体_原始场景帧::时间戳 中同时提供）：
//   - 设备时间_us：depth.get_timestamp(ms)->us，与IMU样本同域
//   - 系统到达时间_us：优先用 frame metadata time-of-arrival，否则用主机时间
//   - 时间域：frame.get_frame_timestamp_domain() 映射到 枚举_时间域
//   - 深度/彩色帧号：用于丢帧/串帧排查
// 2) 标定快照（至少可引用）：深度尺度、内参、外参、对齐模式
// 3) 深度有效性：建议输出 深度有效(u,v) 掩膜（而非仅靠 z<=0）
// 4) IMU：必须输出原始样本序列（gyro/accel，带设备时间_us），并可选输出融合姿态
// 5) 职责边界：驱动层不负责世界树更新；轮廓/存在建议由下游模块统一生成
// ============================================================



export enum class 枚举_D455彩色格式 : std::int8_t {
    自动 = 0,
    RGB8,
    BGR8,
    RGBA8,
    BGRA8,
    YUYV,
    UYVY,
};



export class D455_相机实现 : public 抽象深度相机接口 {
public:
    struct 配置项 {
        // 流配置
        int 深度宽 = 848;
        int 深度高 = 480;
        int 彩色宽 = 848;
        int 彩色高 = 480;
        int 帧率 = 30;
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

        // ===== 轮廓/存在初筛（前景分割 + 连通域）=====
        // 建议：轮廓/ROI 统一由“点簇增强→存在提取”链路生成。
        // 驱动层的连通域轮廓主要用于调试（避免双轨 ROI 语义打架）。
        bool 启用轮廓提取 = false;

        // ===== 轮廓精修（推荐开启：颜色+深度约束，让边界更清晰）=====
        bool 轮廓_颜色深度细化 = true;
        int  轮廓_ROI外扩像素 = 6;
        int  轮廓_颜色差阈值_L1 = 55;          // |dr|+|dg|+|db| 与全局均值
        int  轮廓_邻域颜色差阈值_L1 = 80;       // 与邻域父像素的差（抑制跨边缘渗漏）
        double 轮廓_深度范围扩展_mm = 50;      // 在种子深度 min/max 上下扩一点
        int  轮廓_细化闭运算半径 = 1;           // 对 ROI 掩膜做闭运算填洞/去噪

        bool 轮廓_截图描边 = true;
        Color 轮廓_描边色 = Color{ 255, 0, 0 };

        bool 轮廓_输出点云簇 = true;

        // 背景深度学习：前 N 帧做平均得到背景；之后按 背景_更新系数 缓慢更新“非前景像素”
        int    背景学习帧数 = 30;
        double 背景_更新系数 = 0.01;     // 0 表示不更新

        // 前景判定：|depth - bg| > 阈值（毫米）
        double 前景_深度差阈值_mm = 60.0;
        double 前景_最小深度_mm = 150.0;
        double 前景_最大深度_mm = 4000.0;

        // 连通域筛选
        int 轮廓_最小像素数 = 400;
        int 轮廓_最大数量 = 32;

        // 形态学（简单填洞/去噪），0 表示关闭；1~2 通常足够
        int 轮廓_形态学半径 = 1;

        // 输出内容开关
        bool 轮廓_输出边界点 = false;
        bool 轮廓_输出裁剪颜色 = true;
        bool 轮廓_输出原始掩膜 = true;

        // ===== IMU / 相机姿态（用于视角键 ViewKey 与强融合规则） =====
        bool 启用IMU = true;
        int  陀螺仪FPS = 400;
        int  加速度计FPS = 200;

        // 互补滤波（Mahony 风格）参数：越大收敛越快，但可能更抖
        double IMU_Kp = 2.0;
        double IMU_Ki = 0.0;
        int    IMU_预热样本 = 30;

        // ViewKey 姿态量化角度（度）
        int ViewKey_角度量化_deg = 2;
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
            日志::运行("[D455相机] 开始初始化相机...");
            rs2::config c;

            c.enable_stream(RS2_STREAM_DEPTH, cfg.深度宽, cfg.深度高, RS2_FORMAT_Z16, cfg.帧率);
            日志::运行f("[D455相机] 配置深度流：宽度={}, 高度={}, 帧率={}fps", cfg.深度宽, cfg.深度高, cfg.帧率);

            // IMU（必需）：本工程约定 gyro + accel 必须启用
            cfg.启用IMU = true;
            c.enable_stream(RS2_STREAM_GYRO, RS2_FORMAT_MOTION_XYZ32F, cfg.陀螺仪FPS);
            c.enable_stream(RS2_STREAM_ACCEL, RS2_FORMAT_MOTION_XYZ32F, cfg.加速度计FPS);
            日志::运行f("[D455相机] 配置IMU流：陀螺仪={}fps, 加速度计={}fps", cfg.陀螺仪FPS, cfg.加速度计FPS);

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
                日志::运行f("[D455相机] 配置彩色流：宽度={}, 高度={}, 帧率={}fps, 格式={}", cfg.彩色宽, cfg.彩色高, cfg.帧率, (int)want);
                profile = 管道.start(c);
            }
            catch (...) {
                // 回退
                日志::运行("[D455相机] 彩色流格式配置失败，使用回退配置...");
                rs2::config c2;
                c2.enable_stream(RS2_STREAM_DEPTH, cfg.深度宽, cfg.深度高, RS2_FORMAT_Z16, cfg.帧率);
                c2.enable_stream(RS2_STREAM_COLOR, cfg.彩色宽, cfg.彩色高, RS2_FORMAT_ANY, cfg.帧率);

                // IMU（必需）：fallback 也必须启用
                c2.enable_stream(RS2_STREAM_GYRO, RS2_FORMAT_MOTION_XYZ32F, cfg.陀螺仪FPS);
                c2.enable_stream(RS2_STREAM_ACCEL, RS2_FORMAT_MOTION_XYZ32F, cfg.加速度计FPS);
                profile = 管道.start(c2);
                日志::运行("[D455相机] 回退配置启动成功");
            }

            已打开 = true;
            日志::运行("[D455相机] 管道启动成功，相机已打开");

            // 对齐器（可配置）
            对齐器 = std::make_unique<rs2::align>(RS2_STREAM_COLOR);
            日志::运行("[D455相机] 初始化对齐器（对齐到彩色流）");

            // 深度内参
            auto 深度profile = profile.get_stream(RS2_STREAM_DEPTH).as<rs2::video_stream_profile>();
            深度内参 = 深度profile.get_intrinsics();
            日志::运行f("[D455相机] 获取深度内参：fx={}, fy={}, cx={}, cy={}, 宽度={}, 高度={}", 
                深度内参.fx, 深度内参.fy, 深度内参.ppx, 深度内参.ppy, 深度内参.width, 深度内参.height);

            // 深度尺度 + 设备缓存
            device_ = profile.get_device();
            if (auto ds = device_.first<rs2::depth_sensor>(); ds) {
                深度尺度 = ds.get_depth_scale();
                日志::运行f("[D455相机] 获取深度尺度：{}", 深度尺度);
            }

            // 缓存传感器句柄，便于运行时设置参数
            缓存传感器(device_);
            日志::运行f("[D455相机] 传感器缓存完成：颜色传感器={}, 深度传感器={}", 有颜色传感器_, 有深度传感器_);

            // IMU 传感器（motion_sensor）缓存 + 状态重置
            缓存IMU传感器(device_);
            日志::运行f("[D455相机] IMU传感器缓存完成：有IMU传感器={}", 有IMU传感器_);
            if (!有IMU传感器_) {
                日志::运行("[D455相机] 打开失败：未检测到 IMU（motion_sensor），但本工程要求 IMU 必需。");
                try { 管道.stop(); }
                catch (...) {}
                已打开 = false;
                return false;
            }
            重置IMU状态();
            日志::运行("[D455相机] IMU状态重置完成");

            // 配置传感器参数（自动曝光/白平衡、发射器等）
            配置传感器(device_);
            日志::运行("[D455相机] 传感器参数配置完成");

            // 配置滤波器参数
            配置滤波链();
            日志::运行("[D455相机] 滤波链参数配置完成");

            日志::运行("[D455相机] 相机初始化完成，返回成功");
            return true;
        }
        catch (const rs2::error& e) {
            日志::运行f("[D455相机] 打开失败: {}", e.what());
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

            // ===== 0) IMU（必需）：收集并更新姿态（尽量在取图像时间戳前处理） =====
            if (!有IMU传感器_) return false;
            cfg.启用IMU = true;
            本帧IMU样本_输出.clear();
            收集IMU(frames);

            // ===== 1) 对齐 =====
            rs2::frameset aligned = 对齐器->process(frames);

            rs2::depth_frame depth = aligned.get_depth_frame();
            rs2::video_frame color = aligned.get_color_frame();
            if (!depth || !color) return false;
            // 对齐到彩色后：像素坐标系 = color
            auto cprof = color.get_profile().as<rs2::video_stream_profile>();
            深度内参 = cprof.get_intrinsics();
            // ===== 2) 设备时间戳（微秒）=====
            // RealSense 的 timestamp 通常是毫秒（double），这里转为“微秒整数”存成员，供外设包头使用
            const double ts_ms = depth.get_timestamp();
            最后设备时间_微秒 = (时间戳)std::llround(ts_ms * 1000.0);

            // ===== 2.0) 输出契约：时间戳/时间域/帧号（若结构体提供） =====
            if constexpr (requires { 输出.时间戳; }) {
                输出.时间戳.设备时间_us = (std::uint64_t)最后设备时间_微秒;
                输出.时间戳.域 = 映射时间域(depth.get_frame_timestamp_domain());

                // 系统到达时间：优先取 metadata（单位通常为 ms）
                if (depth.supports_frame_metadata(RS2_FRAME_METADATA_TIME_OF_ARRIVAL)) {
                    const double toa_ms = (double)depth.get_frame_metadata(RS2_FRAME_METADATA_TIME_OF_ARRIVAL);
                    输出.时间戳.系统到达时间_us = (std::uint64_t)std::llround(toa_ms * 1000.0);
                }
                else {
                    输出.时间戳.系统到达时间_us = 主机当前时间_us();
                }

                // 帧号：优先取 frame API，metadata 作为可选覆盖，用于丢帧/串帧排查。
                输出.时间戳.深度帧号 = static_cast<std::uint32_t>(depth.get_frame_number());
                输出.时间戳.彩色帧号 = static_cast<std::uint32_t>(color.get_frame_number());
                if (depth.supports_frame_metadata(RS2_FRAME_METADATA_FRAME_COUNTER)) {
                    输出.时间戳.深度帧号 = static_cast<std::uint32_t>(
                        depth.get_frame_metadata(RS2_FRAME_METADATA_FRAME_COUNTER));
                }
                if (color.supports_frame_metadata(RS2_FRAME_METADATA_FRAME_COUNTER)) {
                    输出.时间戳.彩色帧号 = static_cast<std::uint32_t>(
                        color.get_frame_metadata(RS2_FRAME_METADATA_FRAME_COUNTER));
                }
            }

            // ===== 2.1) 输出相机状态（设备时间域） =====
            输出.相机 = 生成相机状态(最后设备时间_微秒);
            最后相机状态 = 输出.相机;
            最后视角键 = 计算视角键(输出.相机);

            // 输出契约：IMU 原始样本（若结构体提供）
            if constexpr (requires { 输出.IMU样本; }) {
                输出.IMU样本 = 本帧IMU样本_输出;
            }

            // ===== 3) 深度滤波链（不改变分辨率的滤波默认安全）=====
            rs2::frame filtered = depth;


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
            auto prof = depth.get_profile().as<rs2::video_stream_profile>();

            const int w = depth.get_width();
            const int h = depth.get_height();

            // 说明：结构体_原始场景帧 不再使用旧字段“时间”。
            // 时间信息统一由 输出.时间戳（设备时间/系统到达时间/域/帧号）承载。
            输出.宽度 = w;
            输出.高度 = h;

            if constexpr (requires { 输出.深度内参; }) {
                输出.深度内参.fx = (double)深度内参.fx;
                输出.深度内参.fy = (double)深度内参.fy;
                输出.深度内参.cx = (double)深度内参.ppx;
                输出.深度内参.cy = (double)深度内参.ppy;
                输出.深度内参.宽度 = w;
                输出.深度内参.高度 = h;
                输出.深度内参.有效 = true;
            }


            if constexpr (requires { 输出.深度已对齐到彩色; }) {
                输出.深度已对齐到彩色 = true;
            }
            if constexpr (requires { 输出.深度单位_mm; }) {
                // 本实现输出的 深度[] 已是 mm
                输出.深度单位_mm = 1.0;
            }

            const size_t N = (size_t)w * (size_t)h;
            if (输出.深度.size() != N) 输出.深度.resize(N);
            if (输出.颜色.size() != N) 输出.颜色.resize(N);
            if (输出.点云.size() != N) 输出.点云.resize(N);

            if constexpr (requires { 输出.深度有效; }) {
                if (输出.深度有效.size() != N) 输出.深度有效.resize(N);
            }

            // ===== 4) 深度读取（更快：Z16 * depth_scale）=====
            const uint16_t* dp = (const uint16_t*)depth.get_data();
            if (!dp) return false;

            for (size_t i = 0; i < N; ++i) {
                const uint16_t raw = dp[i];
                输出.深度[i] = (double)raw * 深度尺度 * 1000.0; // mm
                if constexpr (requires { 输出.深度有效; }) {
                    输出.深度有效[i] = (raw != 0) ? (std::uint8_t)1 : (std::uint8_t)0;
                }
            }

            // ===== 5) 彩色读取：支持 RGB/BGR/RGBA/BGRA + YUYV/UYVY 兜底 =====
            // 若读取失败，使用白色填充以避免未初始化
            if (!读取对齐彩色(color, 输出)) {
                std::fill(输出.颜色.begin(), 输出.颜色.end(), Color{ 255,255,255 });
            }

            // ===== 6) 点云生成（相机坐标系）=====
            生成点云(输出);


            输出.轮廓观测列表.clear();
            if (cfg.启用轮廓提取) {
                提取轮廓(输出, 输出.轮廓观测列表);
                最近轮廓 = 输出.轮廓观测列表;
            }
            else {
                最近轮廓.clear();
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

    // ===== 新增：相机姿态/视角键（用于强融合 ViewKey） =====
    const 结构体_相机状态& 获取最后相机状态() const { return 最后相机状态; }
    U64 获取最后视角键() const { return 最后视角键; }

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

    // ===== IMU / 姿态估计 =====
    rs2::sensor IMU传感器_{};
    bool 有IMU传感器_ = false;

    struct IMU样本 {
        enum class 枚举_类型 : std::uint8_t { Gyro = 0, Accel = 1 };
        枚举_类型 类型{};
        float x = 0, y = 0, z = 0;
        时间戳 t_us = 0;
    };

    std::deque<IMU样本> IMU缓冲; // 按时间戳递增
    std::size_t IMU缓冲上限 = 4096;

    // 本帧从 frameset 中采集到的 IMU 原始样本（输出到 结构体_原始场景帧::IMU样本）
    std::vector<结构体_IMU样本> 本帧IMU样本_输出;

    Quaternion IMU姿态{};
    Vector3D IMU位置{};  // IMU积分位置或外部SLAM/VIO提供的位置（mm）
    Vector3D IMU速度{};  // IMU积分速度（m/s）
    bool IMU姿态有效 = false;
    int IMU已处理样本 = 0;
    时间戳 IMU最后时间_us = 0;
    Vector3D IMU最后加速度{ 0,0,0 };
    bool IMU有加速度 = false;
    Vector3D IMU积分误差{ 0,0,0 };

    结构体_相机状态 最后相机状态{};
    U64 最后视角键 = 0;

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

    static inline std::uint64_t 主机当前时间_us() {
        using namespace std::chrono;
        return (std::uint64_t)duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
    }

    static inline 枚举_时间域 映射时间域(rs2_timestamp_domain d) {
        switch (d) {
        case RS2_TIMESTAMP_DOMAIN_HARDWARE_CLOCK: return 枚举_时间域::设备硬件时钟;
        case RS2_TIMESTAMP_DOMAIN_SYSTEM_TIME:     return 枚举_时间域::系统时钟;
        case RS2_TIMESTAMP_DOMAIN_GLOBAL_TIME:     return 枚举_时间域::全局时间;
        default:                                   return 枚举_时间域::未知;
        }
    }

    static void 应用KV到配置(配置项& c, const std::string& key, const 外设参数值& val) {
        if (key == "color_auto_exposure") {
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

    void 缓存IMU传感器(const rs2::device& dev) {
        有IMU传感器_ = false;
        try {
            auto ms = dev.first<rs2::motion_sensor>();
            if (ms) {
                IMU传感器_ = ms;
                有IMU传感器_ = true;
            }
        }
        catch (...) {
            有IMU传感器_ = false;
        }
    }

    void 重置IMU状态() {
        IMU缓冲.clear();
        IMU姿态 = Quaternion::单位();
        IMU位置 = { 0,0,0 };
        IMU速度 = { 0,0,0 };
        IMU姿态有效 = true;
        IMU已处理样本 = 0;
        IMU最后时间_us = 0;
        IMU最后加速度 = { 0,0,0 };
        IMU有加速度 = true;
        IMU积分误差 = { 0,0,0 };
    }

    static inline Vector3D 叉乘(const Vector3D& a, const Vector3D& b) {
        return {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }

    static inline double 点乘(const Vector3D& a, const Vector3D& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static inline double 长度(const Vector3D& a) {
        return std::sqrt(点乘(a, a));
    }

    static inline Vector3D 归一化(const Vector3D& a) {
        const double n = 长度(a);
        if (n <= 1e-12) return { 0,0,0 };
        return { a.x / n, a.y / n, a.z / n };
    }

    static Quaternion 从欧拉(double roll, double pitch, double yaw) {
        // Z(yaw) * Y(pitch) * X(roll)
        const double cr = std::cos(roll * 0.5);
        const double sr = std::sin(roll * 0.5);
        const double cp = std::cos(pitch * 0.5);
        const double sp = std::sin(pitch * 0.5);
        const double cy = std::cos(yaw * 0.5);
        const double sy = std::sin(yaw * 0.5);

        Quaternion q;
        q.w = cy * cp * cr + sy * sp * sr;
        q.x = cy * cp * sr - sy * sp * cr;
        q.y = sy * cp * sr + cy * sp * cr;
        q.z = sy * cp * cr - cy * sp * sr;
        q.归一化();
        return q;
    }

    static void 转欧拉(const Quaternion& qin, double& roll, double& pitch, double& yaw) {
        Quaternion q = qin;
        q.归一化();

        // roll (x-axis rotation)
        const double sinr_cosp = 2.0 * (q.w * q.x + q.y * q.z);
        const double cosr_cosp = 1.0 - 2.0 * (q.x * q.x + q.y * q.y);
        roll = std::atan2(sinr_cosp, cosr_cosp);

        // pitch (y-axis rotation)
        const double sinp = 2.0 * (q.w * q.y - q.z * q.x);
        if (std::abs(sinp) >= 1.0) pitch = std::copysign(3.14159265358979323846 / 2.0, sinp);
        else pitch = std::asin(sinp);

        // yaw (z-axis rotation)
        const double siny_cosp = 2.0 * (q.w * q.z + q.x * q.y);
        const double cosy_cosp = 1.0 - 2.0 * (q.y * q.y + q.z * q.z);
        yaw = std::atan2(siny_cosp, cosy_cosp);
    }

    void 收集IMU(const rs2::frameset& frames) {
        // 将当前 frameset 内的 motion_frame 收集到缓冲，并按时间排序后逐条融合
        std::vector<IMU样本> batch;
        batch.reserve(4);

        for (auto it = frames.begin(); it != frames.end(); ++it) {
            rs2::frame f = *it;
            if (!f) continue;
            if (!f.is<rs2::motion_frame>()) continue;

            try {
                auto mf = f.as<rs2::motion_frame>();
                auto st = mf.get_profile().stream_type();
                const double ts_ms = mf.get_timestamp();
                const 时间戳 t_us = (时间戳)std::llround(ts_ms * 1000.0);
                rs2_vector v = mf.get_motion_data();

                if (st == RS2_STREAM_GYRO) {
                    batch.push_back({ IMU样本::枚举_类型::Gyro, v.x, v.y, v.z, t_us });
                    // 输出：陀螺样本（rad/s）
                    本帧IMU样本_输出.push_back(结构体_IMU样本{ (std::uint64_t)t_us, Vector3D{0,0,0}, Vector3D{ (double)v.x, (double)v.y, (double)v.z } });
                }
                else if (st == RS2_STREAM_ACCEL) {
                    batch.push_back({ IMU样本::枚举_类型::Accel, v.x, v.y, v.z, t_us });
                    // 输出：加速度样本（m/s^2）
                    本帧IMU样本_输出.push_back(结构体_IMU样本{ (std::uint64_t)t_us, Vector3D{ (double)v.x, (double)v.y, (double)v.z }, Vector3D{0,0,0} });
                }
            }
            catch (...) {
            }
        }


        if (batch.empty()) return;
        std::sort(batch.begin(), batch.end(), [](const IMU样本& a, const IMU样本& b) { return a.t_us < b.t_us; });

        for (const auto& s : batch) {
            IMU缓冲.push_back(s);
        }
        while (IMU缓冲.size() > IMU缓冲上限) IMU缓冲.pop_front();

        // 逐条融合更新
        for (const auto& s : batch) {
            处理单个IMU样本(s);
        }
    }

    void 处理单个IMU样本(const IMU样本& s) {
        if (s.类型 == IMU样本::枚举_类型::Accel) {
            IMU最后加速度 = { (double)s.x, (double)s.y, (double)s.z };
            IMU有加速度 = true;

            // 初次初始化：仅用重力方向估 roll/pitch，yaw=0
            if (!IMU姿态有效) {
                const Vector3D a = 归一化(IMU最后加速度);
                // 常用：roll = atan2(ay, az), pitch = atan2(-ax, sqrt(ay^2+az^2))
                const double roll = std::atan2(a.y, a.z);
                const double pitch = std::atan2(-a.x, std::sqrt(a.y * a.y + a.z * a.z));
                IMU姿态 = 从欧拉(roll, pitch, 0.0);
            }
            return;
        }

        // Gyro：用互补( Mahony 风格 )更新姿态
        const 时间戳 t = s.t_us;
        if (IMU最后时间_us == 0) {
            IMU最后时间_us = t;
            return;
        }

        const double dt = (double)(t - IMU最后时间_us) * 1e-6;
        IMU最后时间_us = t;

        if (dt <= 0.0 || dt > 0.2) {
            // 时间断层：不积分，避免爆炸
            IMU积分误差 = { 0,0,0 };
            return;
        }

        Vector3D gyro{ (double)s.x, (double)s.y, (double)s.z }; // RealSense gyro 单位通常为 rad/s

        if (IMU有加速度) {
            Vector3D acc = 归一化(IMU最后加速度);
            // 估计重力方向：将世界重力(0,0,1)旋到设备坐标
            Vector3D g_est = IMU姿态.旋转({ 0,0,1 });
            g_est = 归一化(g_est);

            Vector3D err = 叉乘(acc, g_est);

            // 积分项
            if (cfg.IMU_Ki > 0.0) {
                IMU积分误差 += err * (cfg.IMU_Ki * dt);
            }

            gyro += err * cfg.IMU_Kp + IMU积分误差;
        }

        // 四元数积分：q_dot = 0.5 * q ⊗ (0,gyro)
        Quaternion q = IMU姿态;
        q.归一化();
        Quaternion omega{ 0.0, gyro.x, gyro.y, gyro.z };
        Quaternion qdot = q * omega;
        qdot.w *= 0.5; qdot.x *= 0.5; qdot.y *= 0.5; qdot.z *= 0.5;

        q.w += qdot.w * dt;
        q.x += qdot.x * dt;
        q.y += qdot.y * dt;
        q.z += qdot.z * dt;
        q.归一化();
        IMU姿态 = q;

        // IMU位置积分（简单的加速度积分，需要重力补偿和漂移校正）
        if (IMU有加速度 && IMU姿态有效) {
            // 重力补偿：从加速度中减去重力分量
            Vector3D g_world{ 0, 0, 9.81 }; // 重力加速度（m/s²）
            Vector3D g_cam = IMU姿态.共轭().旋转(g_world); // 相机坐标系下的重力
            Vector3D acc_cam = IMU最后加速度; // 原始加速度（m/s²）
            Vector3D acc_world = IMU姿态.旋转(acc_cam - g_cam); // 世界坐标系下的加速度（去除重力）
            
            // 速度积分（简单欧拉积分）
            IMU速度 += acc_world * dt;
            
            // 位置积分（转换为毫米）
            IMU位置 += IMU速度 * dt * 1000.0; // m/s → mm/s → mm
        }

        IMU已处理样本++;
        if (!IMU姿态有效 && IMU已处理样本 >= cfg.IMU_预热样本) {
            IMU姿态有效 = true;
        }
    }

    结构体_相机状态 生成相机状态(时间戳 device_us) const {
        结构体_相机状态 s;
        s.设备时间_微秒 = device_us;
        s.姿态 = IMU姿态;
        s.位置 = IMU位置;  // 使用IMU积分位置或外部SLAM/VIO提供的位置
        // IMU 为必需：有效性仅取决于 IMU 传感器存在 + 融合姿态有效
        s.有效 = (有IMU传感器_ && IMU姿态有效);
        return s;
    }

    static inline std::int64_t 量化角度_deg(double rad, int step_deg) {
        const double deg = rad * 180.0 / 3.14159265358979323846;
        const double step = (double)std::max(1, step_deg);
        return (std::int64_t)std::llround(deg / step);
    }

    static inline U64 FNV1a64_bytes(const void* data, std::size_t n) {
        const std::uint8_t* p = (const std::uint8_t*)data;
        U64 h = 14695981039346656037ull;
        for (std::size_t i = 0; i < n; ++i) {
            h ^= (U64)p[i];
            h *= 1099511628211ull;
        }
        return h;
    }

    U64 计算视角键(const 结构体_相机状态& s) const {
        if (!s.有效) return 0;
        double roll = 0, pitch = 0, yaw = 0;
        转欧拉(s.姿态, roll, pitch, yaw);

        const std::int64_t qr = 量化角度_deg(roll, cfg.ViewKey_角度量化_deg);
        const std::int64_t qp = 量化角度_deg(pitch, cfg.ViewKey_角度量化_deg);
        const std::int64_t qy = 量化角度_deg(yaw, cfg.ViewKey_角度量化_deg);

        struct Pack { std::int64_t r, p, y; } pack{ qr, qp, qy };
        return FNV1a64_bytes(&pack, sizeof(pack));
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

        const double minZ = cfg.前景_最小深度_mm;
        const double maxZ = cfg.前景_最大深度_mm;
        const double th = cfg.前景_深度差阈值_mm;

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

    static inline bool 点云有效(const Vector3D& p) {
        return std::isfinite((double)p.x) && std::isfinite((double)p.y) && std::isfinite((double)p.z) && (double)p.z > 0.0;
    }

    static inline int 颜色差_L1(const Color& a, const Color& b) {
        return std::abs((int)a.r - (int)b.r) + std::abs((int)a.g - (int)b.g) + std::abs((int)a.b - (int)b.b);
    }

    // 分位数（就地 nth_element）：用于鲁棒中心/尺寸，避免少量离群点把 min/max 拉飞
    static inline double 取分位_就地(std::vector<double>& v, double q01) {
        if (v.empty()) return std::numeric_limits<double>::quiet_NaN();
        q01 = std::clamp(q01, 0.0, 1.0);
        const size_t n = v.size();
        const size_t k = (n <= 1) ? 0 : (size_t)std::floor(q01 * (double)(n - 1));
        auto it = v.begin() + (std::ptrdiff_t)k;
        std::nth_element(v.begin(), it, v.end());
        return *it;
    }

    static void 提取边界点_4邻域(const std::vector<std::uint8_t>& roiMask, int rx, int ry, int rw, int rh, std::vector<std::int64_t>& outBoundary) {
        outBoundary.clear();
        if (rw <= 0 || rh <= 0) return;

        auto ridx = [&](int u, int v) { return (size_t)v * (size_t)rw + (size_t)u; };

        for (int v = 0; v < rh; ++v) for (int u = 0; u < rw; ++u) {
            if (roiMask[ridx(u, v)] == 0) continue;
            bool edge =
                (u == 0 || roiMask[ridx(u - 1, v)] == 0) ||
                (u == rw - 1 || roiMask[ridx(u + 1, v)] == 0) ||
                (v == 0 || roiMask[ridx(u, v - 1)] == 0) ||
                (v == rh - 1 || roiMask[ridx(u, v + 1)] == 0);
            if (edge) {
                const int U = rx + u;
                const int V = ry + v;
                outBoundary.push_back((std::int64_t)((std::uint64_t)(std::uint32_t)V << 32) | (std::uint32_t)U);
            }
        }
    }

    void 细化掩膜_颜色深度(const 结构体_原始场景帧& 帧, int rx, int ry, int rw, int rh, std::vector<std::uint8_t>& roiMask) {
        if (!cfg.轮廓_颜色深度细化) return;
        if (rw <= 0 || rh <= 0) return;

        const int W = 帧.宽度;
        const int H = 帧.高度;
        if (W <= 0 || H <= 0) return;

        auto ridx = [&](int u, int v) { return (size_t)v * (size_t)rw + (size_t)u; };
        auto gidx = [&](int u, int v) { return (size_t)v * (size_t)W + (size_t)u; };

        // 1) 计算种子均值颜色 + 种子深度范围
        std::uint64_t sumR = 0, sumG = 0, sumB = 0;
        int seedCount = 0;

        double zMin = std::numeric_limits<double>::infinity();
        double zMax = -std::numeric_limits<double>::infinity();

        for (int v = 0; v < rh; ++v) for (int u = 0; u < rw; ++u) {
            if (roiMask[ridx(u, v)] == 0) continue;
            const int U = rx + u;
            const int V = ry + v;
            if (!在范围内(U, 0, W - 1) || !在范围内(V, 0, H - 1)) continue;

            const size_t gi = gidx(U, V);
            const double z = 帧.深度[gi];
            if (z > 0.0) { zMin = std::min(zMin, z); zMax = std::max(zMax, z); }

            const Color c = 帧.颜色[gi];
            sumR += c.r; sumG += c.g; sumB += c.b;
            ++seedCount;
        }
        if (seedCount <= 0 || !std::isfinite(zMin) || !std::isfinite(zMax)) return;

        const Color meanC{
            (std::uint8_t)std::min<std::uint64_t>(255, sumR / (std::uint64_t)seedCount),
            (std::uint8_t)std::min<std::uint64_t>(255, sumG / (std::uint64_t)seedCount),
            (std::uint8_t)std::min<std::uint64_t>(255, sumB / (std::uint64_t)seedCount)
        };

        double zLo = zMin - cfg.轮廓_深度范围扩展_mm;
        double zHi = zMax + cfg.轮廓_深度范围扩展_mm;

        // 安全夹取：不允许超出“前景深度范围”，避免细化时把远处背景也吸进来
        zLo = std::max(zLo, cfg.前景_最小深度_mm);
        zHi = std::min(zHi, cfg.前景_最大深度_mm);
        if (!(zLo <= zHi)) return;

        // 2) 多源 BFS：颜色 + 深度双约束
        std::vector<std::uint8_t> vis((size_t)rw * (size_t)rh, 0);
        std::queue<int> q;

        for (int v = 0; v < rh; ++v) for (int u = 0; u < rw; ++u) {
            if (roiMask[ridx(u, v)] == 0) continue;
            const int id = v * rw + u;
            vis[(size_t)id] = 1;
            q.push(id);
        }

        while (!q.empty()) {
            const int id = q.front(); q.pop();
            const int u0 = id % rw;
            const int v0 = id / rw;

            const int U0 = rx + u0;
            const int V0 = ry + v0;
            if (!在范围内(U0, 0, W - 1) || !在范围内(V0, 0, H - 1)) continue;

            const Color c0 = 帧.颜色[gidx(U0, V0)];

            auto try_add = [&](int nu, int nv) {
                if (!在范围内(nu, 0, rw - 1) || !在范围内(nv, 0, rh - 1)) return;
                const int nid = nv * rw + nu;
                if (vis[(size_t)nid]) return;
                vis[(size_t)nid] = 1;

                const int U = rx + nu;
                const int V = ry + nv;
                if (!在范围内(U, 0, W - 1) || !在范围内(V, 0, H - 1)) return;

                const size_t gi = gidx(U, V);
                const double z = 帧.深度[gi];
                if (!(z > 0.0 && z >= zLo && z <= zHi)) return;

                const Color c = 帧.颜色[gi];
                if (颜色差_L1(c, meanC) > cfg.轮廓_颜色差阈值_L1) return;
                if (颜色差_L1(c, c0) > cfg.轮廓_邻域颜色差阈值_L1) return;

                roiMask[ridx(nu, nv)] = 1;
                q.push(nid);
                };

            try_add(u0 - 1, v0);
            try_add(u0 + 1, v0);
            try_add(u0, v0 - 1);
            try_add(u0, v0 + 1);
        }

        // 3) ROI 小范围闭运算：填洞 + 让轮廓更“连贯”
        if (cfg.轮廓_细化闭运算半径 > 0) {
            形态学_闭运算(roiMask, rw, rh, cfg.轮廓_细化闭运算半径);
        }
    }

    bool 构建轮廓观测_由像素集合(const 结构体_原始场景帧& 帧,const std::vector<int>& pixels,int minx0, int maxx0, int miny0, int maxy0,int seq,结构体_轮廓观测& outObs)
    {
        const int W = 帧.宽度;
        const int H = 帧.高度;
        if (W <= 0 || H <= 0) return false;
        if (pixels.empty()) return false;

        // ROI：对连通域外扩一点，给边界“留气口”
        const int pad = std::max(0, cfg.轮廓_ROI外扩像素);
        const int rx = std::max(0, minx0 - pad);
        const int ry = std::max(0, miny0 - pad);
        const int rX = std::min(W - 1, maxx0 + pad);
        const int rY = std::min(H - 1, maxy0 + pad);
        const int rw = rX - rx + 1;
        const int rh = rY - ry + 1;
        if (rw <= 0 || rh <= 0) return false;

        std::vector<std::uint8_t> roiMask((size_t)rw * (size_t)rh, 0);
        auto ridx = [&](int u, int v) { return (size_t)v * (size_t)rw + (size_t)u; };

        // 种子：连通域像素
        for (int pi : pixels) {
            const int u = pi % W;
            const int v = pi / W;
            if (!在范围内(u, rx, rX) || !在范围内(v, ry, rY)) continue;
            roiMask[ridx(u - rx, v - ry)] = 1;
        }

        // 颜色+深度细化
        细化掩膜_颜色深度(帧, rx, ry, rw, rh, roiMask);

        // 统计掩膜像素数（粗过滤）
        int maskCount = 0;
        for (auto b : roiMask) if (b) ++maskCount;
        if (maskCount < std::max(1, cfg.轮廓_最小像素数)) return false;

        结构体_轮廓观测 obs;
        obs.帧内序号 = seq;
        obs.x = rx; obs.y = ry; obs.w = rw; obs.h = rh;

        // 裁剪颜色
        if (cfg.轮廓_输出裁剪颜色) {
            obs.裁剪颜色.assign((size_t)rw * (size_t)rh, Color{ 0,0,0 });
            for (int vv = 0; vv < rh; ++vv) {
                const int V = ry + vv;
                for (int uu = 0; uu < rw; ++uu) {
                    const int U = rx + uu;
                    obs.裁剪颜色[ridx(uu, vv)] = 帧.颜色[(size_t)V * (size_t)W + (size_t)U];
                }
            }
        }

        // 边界点（必要时也用于截图描边）
        std::vector<std::int64_t> boundary;
        if (cfg.轮廓_输出边界点 || cfg.轮廓_截图描边) {
            提取边界点_4邻域(roiMask, rx, ry, rw, rh, boundary);
            if (cfg.轮廓_输出边界点) obs.边界点 = boundary;
            if (cfg.轮廓_截图描边 && cfg.轮廓_输出裁剪颜色) {
                for (auto packed : boundary) {
                    const int U = (int)(packed & 0xFFFFFFFFu);
                    const int V = (int)((std::uint64_t)packed >> 32);
                    const int uu = U - rx;
                    const int vv = V - ry;
                    if (!在范围内(uu, 0, rw - 1) || !在范围内(vv, 0, rh - 1)) continue;
                    obs.裁剪颜色[ridx(uu, vv)] = cfg.轮廓_描边色;
                }
            }
        }

        // 输出掩膜
        if (cfg.轮廓_输出原始掩膜) {
            obs.掩膜 = roiMask;
        }

        // 计算 3D 点云簇 + 中心/尺寸 + 平均颜色
        // 计算 3D 点云簇 + 中心/尺寸 + 平均颜色（鲁棒：p05/p95）
        const double zMin_mm = cfg.前景_最小深度_mm;
        const double zMax_mm = cfg.前景_最大深度_mm;
        constexpr double qLo = 0.05; // p05
        constexpr double qHi = 0.95; // p95

        std::vector<double> xs, ys, zs;
        xs.reserve((size_t)maskCount);
        ys.reserve((size_t)maskCount);
        zs.reserve((size_t)maskCount);

        std::uint64_t sumR = 0, sumG = 0, sumB = 0;
        int validCnt = 0;

        if (cfg.轮廓_输出点云簇) {
            obs.点云簇.clear();
            obs.点云簇.reserve((size_t)maskCount);
        }

        for (int vv = 0; vv < rh; ++vv) {
            const int V = ry + vv;
            for (int uu = 0; uu < rw; ++uu) {
                if (roiMask[ridx(uu, vv)] == 0) continue;
                const int U = rx + uu;
                const size_t gi = (size_t)V * (size_t)W + (size_t)U;

                const Vector3D& P = 帧.点云[gi];
                if (!点云有效(P)) continue;

                const double zx = (double)P.z;
                if (!(zx >= zMin_mm && zx <= zMax_mm)) continue; // 关键：夹取在前景深度范围内，避免“远处墙/空洞”污染尺寸

                xs.push_back((double)P.x);
                ys.push_back((double)P.y);
                zs.push_back(zx);

                const Color C = 帧.颜色[gi];
                sumR += C.r; sumG += C.g; sumB += C.b;
                ++validCnt;

                if (cfg.轮廓_输出点云簇) {
                    obs.点云簇.push_back(P);
                }
            }
        }

        if (validCnt <= 0) return false;

        // 鲁棒范围：用 p05/p95 替代 min/max，抗离群点
        const double x05 = 取分位_就地(xs, qLo);
        const double x95 = 取分位_就地(xs, qHi);
        const double y05 = 取分位_就地(ys, qLo);
        const double y95 = 取分位_就地(ys, qHi);
        const double z05 = 取分位_就地(zs, qLo);
        const double z95 = 取分位_就地(zs, qHi);

        if (!(std::isfinite(x05) && std::isfinite(x95) && std::isfinite(z05) && std::isfinite(z95))) return false;
        if (!(z05 >= zMin_mm && z95 <= zMax_mm)) return false;

        obs.像素数 = validCnt;
        obs.中心 = Vector3D{ (x05 + x95) * 0.5, (y05 + y95) * 0.5, (z05 + z95) * 0.5 };
        obs.尺寸 = Vector3D{ std::max(0.0, x95 - x05), std::max(0.0, y95 - y05), std::max(0.0, z95 - z05) };

        obs.平均颜色 = Color{
            (std::uint8_t)std::min<std::uint64_t>(255, sumR / (std::uint64_t)std::max(1, validCnt)),
            (std::uint8_t)std::min<std::uint64_t>(255, sumG / (std::uint64_t)std::max(1, validCnt)),
            (std::uint8_t)std::min<std::uint64_t>(255, sumB / (std::uint64_t)std::max(1, validCnt))
        };

        outObs = std::move(obs);
        return true;
    }

    void 提取轮廓(const 结构体_原始场景帧& 帧, std::vector<结构体_轮廓观测>& out) {


        const int w = 帧.宽度, h = 帧.高度;
        if (w <= 0 || h <= 0) return;

        // 1) 深度前景初筛（快速）：背景差 / 帧差 -> mask
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

            // 2) 连通域聚合（4 邻域）
            int minx = u0, maxx = u0, miny = v0, maxy = v0;
            int count = 0;

            std::vector<int> pixels;
            pixels.reserve(2048);

            q.push((int)i0);
            vis[i0] = 1;

            while (!q.empty()) {
                const int i = q.front(); q.pop();
                const int u = i % w;
                const int v = i / w;

                ++count;
                pixels.push_back(i);

                minx = std::min(minx, u);
                maxx = std::max(maxx, u);
                miny = std::min(miny, v);
                maxy = std::max(maxy, v);

                auto try_push = [&](int nu, int nv) {
                    if (!在范围内(nu, 0, w - 1) || !在范围内(nv, 0, h - 1)) return;
                    const size_t ni = idx(nu, nv);
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

            // 3) 由像素集合构建“清晰轮廓 + 彩色截图 + 点云簇”
            结构体_轮廓观测 obs;
            if (!构建轮廓观测_由像素集合(帧, pixels, minx, maxx, miny, maxy, seq, obs)) continue;

            out.push_back(std::move(obs));
            ++seq;

            ++found;
            if (found >= std::max(1, cfg.轮廓_最大数量)) return;
        }

    }




};
