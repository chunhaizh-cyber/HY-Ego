// 文件规则：D455 相机采集器是唯一 RealSense SDK 边界；导出接口不得暴露 SDK 类型或指向 SDK 缓冲区的视图。
module;

// rs2.hpp 在 MSVC 模块中会因 rs2_option_value 的类内 pack 切换触发 C3614；
// 本模块使用同版本 SDK 的窄 C API 头，保持相同设备、profile、帧和标定语义。
#include <librealsense2/h/rs_context.h>
#include <librealsense2/h/rs_device.h>
#include <librealsense2/h/rs_frame.h>
#include <librealsense2/h/rs_pipeline.h>
#include <librealsense2/h/rs_sensor.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

export module 海中鱼巣.适配.采集器.D455相机;
import 海中鱼巣.适配.协议.D455采样材料;

export namespace 海中鱼巣 {

std::unique_ptr<D455帧来源> 创建并打开D455相机采集器(
    D455采集配置 配置,
    D455操作结果& 打开结果);

}

namespace 海中鱼巣 {
namespace {

inline constexpr int RealSense接口版本 = 2 * 10000 + 56 * 100 + 3;

class SDK错误槽 {
public:
    SDK错误槽() = default;
    SDK错误槽(const SDK错误槽&) = delete;
    SDK错误槽& operator=(const SDK错误槽&) = delete;

    ~SDK错误槽() {
        if (错误_ != nullptr) {
            rs2_free_error(错误_);
        }
    }

    rs2_error** 接收() noexcept {
        return &错误_;
    }

    bool 失败() const noexcept {
        return 错误_ != nullptr;
    }

private:
    rs2_error* 错误_ = nullptr;
};

struct 上下文释放器 { void operator()(rs2_context* 值) const noexcept { if (值) rs2_delete_context(值); } };
struct 设备列表释放器 { void operator()(rs2_device_list* 值) const noexcept { if (值) rs2_delete_device_list(值); } };
struct 设备释放器 { void operator()(rs2_device* 值) const noexcept { if (值) rs2_delete_device(值); } };
struct 管线释放器 { void operator()(rs2_pipeline* 值) const noexcept { if (值) rs2_delete_pipeline(值); } };
struct 配置释放器 { void operator()(rs2_config* 值) const noexcept { if (值) rs2_delete_config(值); } };
struct 管线配置释放器 { void operator()(rs2_pipeline_profile* 值) const noexcept { if (值) rs2_delete_pipeline_profile(值); } };
struct 流配置列表释放器 { void operator()(rs2_stream_profile_list* 值) const noexcept { if (值) rs2_delete_stream_profiles_list(值); } };
struct 传感器列表释放器 { void operator()(rs2_sensor_list* 值) const noexcept { if (值) rs2_delete_sensor_list(值); } };
struct 传感器释放器 { void operator()(rs2_sensor* 值) const noexcept { if (值) rs2_delete_sensor(值); } };
struct 帧释放器 { void operator()(rs2_frame* 值) const noexcept { if (值) rs2_release_frame(值); } };

using 上下文指针 = std::unique_ptr<rs2_context, 上下文释放器>;
using 设备列表指针 = std::unique_ptr<rs2_device_list, 设备列表释放器>;
using 设备指针 = std::unique_ptr<rs2_device, 设备释放器>;
using 管线指针 = std::unique_ptr<rs2_pipeline, 管线释放器>;
using 配置指针 = std::unique_ptr<rs2_config, 配置释放器>;
using 管线配置指针 = std::unique_ptr<rs2_pipeline_profile, 管线配置释放器>;
using 流配置列表指针 = std::unique_ptr<rs2_stream_profile_list, 流配置列表释放器>;
using 传感器列表指针 = std::unique_ptr<rs2_sensor_list, 传感器列表释放器>;
using 传感器指针 = std::unique_ptr<rs2_sensor, 传感器释放器>;
using 帧指针 = std::unique_ptr<rs2_frame, 帧释放器>;

std::string 读取设备信息(const rs2_device* 设备, rs2_camera_info 信息, bool& 调用成功) {
    SDK错误槽 支持错误;
    const int 支持 = rs2_supports_device_info(设备, 信息, 支持错误.接收());
    if (支持错误.失败()) {
        调用成功 = false;
        return {};
    }
    if (支持 == 0) {
        return {};
    }
    SDK错误槽 读取错误;
    const char* 值 = rs2_get_device_info(设备, 信息, 读取错误.接收());
    if (读取错误.失败() || 值 == nullptr) {
        调用成功 = false;
        return {};
    }
    return 值;
}

bool 是D455设备(const rs2_device* 设备, bool& 调用成功) {
    const auto 名称 = 读取设备信息(设备, RS2_CAMERA_INFO_NAME, 调用成功);
    const auto 产品线 = 读取设备信息(设备, RS2_CAMERA_INFO_PRODUCT_LINE, 调用成功);
    return 调用成功 && 名称.find("D455") != std::string::npos && 产品线 == "D400";
}

D455设备身份材料 转换设备身份(const rs2_device* 设备, bool& 调用成功) {
    return {
        读取设备信息(设备, RS2_CAMERA_INFO_NAME, 调用成功),
        读取设备信息(设备, RS2_CAMERA_INFO_SERIAL_NUMBER, 调用成功),
        读取设备信息(设备, RS2_CAMERA_INFO_FIRMWARE_VERSION, 调用成功),
        读取设备信息(设备, RS2_CAMERA_INFO_PRODUCT_ID, 调用成功)
    };
}

rs2_stream 转换流类型(D455流类型 类型) noexcept {
    switch (类型) {
    case D455流类型::彩色: return RS2_STREAM_COLOR;
    case D455流类型::深度: return RS2_STREAM_DEPTH;
    case D455流类型::红外: return RS2_STREAM_INFRARED;
    default: return RS2_STREAM_ANY;
    }
}

rs2_format 转换像素格式(D455像素格式 格式) noexcept {
    switch (格式) {
    case D455像素格式::RGB8: return RS2_FORMAT_RGB8;
    case D455像素格式::Z16: return RS2_FORMAT_Z16;
    case D455像素格式::Y8: return RS2_FORMAT_Y8;
    default: return RS2_FORMAT_ANY;
    }
}

D455时间戳域 转换时间戳域(rs2_timestamp_domain 时间戳域) noexcept {
    switch (时间戳域) {
    case RS2_TIMESTAMP_DOMAIN_HARDWARE_CLOCK: return D455时间戳域::硬件时钟;
    case RS2_TIMESTAMP_DOMAIN_SYSTEM_TIME: return D455时间戳域::系统时间;
    case RS2_TIMESTAMP_DOMAIN_GLOBAL_TIME: return D455时间戳域::全局时间;
    default: return D455时间戳域::未知;
    }
}

D455相机内参材料 转换内参(const rs2_intrinsics& 内参) {
    D455相机内参材料 材料;
    材料.宽 = static_cast<std::uint32_t>(内参.width);
    材料.高 = static_cast<std::uint32_t>(内参.height);
    材料.主点横坐标 = 内参.ppx;
    材料.主点纵坐标 = 内参.ppy;
    材料.横向焦距 = 内参.fx;
    材料.纵向焦距 = 内参.fy;
    材料.畸变模型 = static_cast<std::int32_t>(内参.model);
    std::copy(std::begin(内参.coeffs), std::end(内参.coeffs), 材料.畸变系数组.begin());
    return 材料;
}

D455相机外参材料 转换外参(
    const rs2_extrinsics& 外参,
    D455流类型 来源类型,
    std::uint32_t 来源索引,
    D455流类型 目标类型,
    std::uint32_t 目标索引) {
    D455相机外参材料 材料;
    材料.来源类型 = 来源类型;
    材料.来源索引 = 来源索引;
    材料.目标类型 = 目标类型;
    材料.目标索引 = 目标索引;
    std::copy(std::begin(外参.rotation), std::end(外参.rotation), 材料.旋转矩阵.begin());
    std::copy(std::begin(外参.translation), std::end(外参.translation), 材料.平移向量.begin());
    return 材料;
}

std::uint64_t 当前单调时间戳微秒() {
    return static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count());
}

bool 读取流配置(const rs2_stream_profile* 流配置, D455流配置& 结果) {
    rs2_stream 流类型 = RS2_STREAM_ANY;
    rs2_format 格式 = RS2_FORMAT_ANY;
    int 流索引 = 0;
    int 唯一编号 = 0;
    int 帧率 = 0;
    SDK错误槽 数据错误;
    rs2_get_stream_profile_data(
        流配置, &流类型, &格式, &流索引, &唯一编号, &帧率, 数据错误.接收());
    if (数据错误.失败()) {
        return false;
    }
    int 宽 = 0;
    int 高 = 0;
    SDK错误槽 分辨率错误;
    rs2_get_video_stream_resolution(流配置, &宽, &高, 分辨率错误.接收());
    if (分辨率错误.失败() || 宽 <= 0 || 高 <= 0 || 帧率 <= 0 || 流索引 < 0) {
        return false;
    }
    switch (流类型) {
    case RS2_STREAM_COLOR: 结果.类型 = D455流类型::彩色; break;
    case RS2_STREAM_DEPTH: 结果.类型 = D455流类型::深度; break;
    case RS2_STREAM_INFRARED: 结果.类型 = D455流类型::红外; break;
    default: return false;
    }
    switch (格式) {
    case RS2_FORMAT_RGB8: 结果.格式 = D455像素格式::RGB8; break;
    case RS2_FORMAT_Z16: 结果.格式 = D455像素格式::Z16; break;
    case RS2_FORMAT_Y8: 结果.格式 = D455像素格式::Y8; break;
    default: return false;
    }
    结果.流索引 = static_cast<std::uint32_t>(流索引);
    结果.宽 = static_cast<std::uint32_t>(宽);
    结果.高 = static_cast<std::uint32_t>(高);
    结果.帧率 = static_cast<std::uint32_t>(帧率);
    return true;
}

bool 流配置相同(const D455流配置& 左, const D455流配置& 右) noexcept {
    return 左.类型 == 右.类型
        && 左.流索引 == 右.流索引
        && 左.格式 == 右.格式
        && 左.宽 == 右.宽
        && 左.高 == 右.高
        && 左.帧率 == 右.帧率;
}

bool 复核实际流配置(rs2_pipeline_profile* 管线配置, const D455采集配置& 请求) {
    SDK错误槽 列表错误;
    流配置列表指针 列表(
        rs2_pipeline_profile_get_streams(管线配置, 列表错误.接收()));
    if (列表错误.失败() || !列表) {
        return false;
    }
    SDK错误槽 数量错误;
    const int 数量 = rs2_get_stream_profiles_count(列表.get(), 数量错误.接收());
    if (数量错误.失败() || 数量 <= 0) {
        return false;
    }
    bool 彩色匹配 = false;
    bool 深度匹配 = false;
    bool 左红外匹配 = false;
    bool 右红外匹配 = false;
    for (int 索引 = 0; 索引 < 数量; ++索引) {
        SDK错误槽 读取错误;
        const auto* 流配置 = rs2_get_stream_profile(列表.get(), 索引, 读取错误.接收());
        if (读取错误.失败() || 流配置 == nullptr) {
            return false;
        }
        D455流配置 实际;
        if (!读取流配置(流配置, 实际)) {
            continue;
        }
        彩色匹配 = 彩色匹配 || 流配置相同(实际, 请求.彩色);
        深度匹配 = 深度匹配 || 流配置相同(实际, 请求.深度);
        左红外匹配 = 左红外匹配 || 流配置相同(实际, 请求.左红外);
        右红外匹配 = 右红外匹配 || 流配置相同(实际, 请求.右红外);
    }
    return 彩色匹配 && 深度匹配 && 左红外匹配 && 右红外匹配;
}

bool 读取深度比例(const rs2_device* 设备, float& 深度比例) {
    SDK错误槽 列表错误;
    传感器列表指针 列表(rs2_query_sensors(设备, 列表错误.接收()));
    if (列表错误.失败() || !列表) {
        return false;
    }
    SDK错误槽 数量错误;
    const int 数量 = rs2_get_sensors_count(列表.get(), 数量错误.接收());
    if (数量错误.失败() || 数量 <= 0) {
        return false;
    }
    for (int 索引 = 0; 索引 < 数量; ++索引) {
        SDK错误槽 创建错误;
        传感器指针 传感器(rs2_create_sensor(列表.get(), 索引, 创建错误.接收()));
        if (创建错误.失败() || !传感器) {
            return false;
        }
        SDK错误槽 类型错误;
        const int 是深度传感器 = rs2_is_sensor_extendable_to(
            传感器.get(), RS2_EXTENSION_DEPTH_SENSOR, 类型错误.接收());
        if (类型错误.失败()) {
            return false;
        }
        if (是深度传感器 == 0) {
            continue;
        }
        SDK错误槽 比例错误;
        深度比例 = rs2_get_depth_scale(传感器.get(), 比例错误.接收());
        return !比例错误.失败() && std::isfinite(深度比例) && 深度比例 > 0.0F;
    }
    return false;
}

bool 读取帧流身份(const rs2_frame* 帧, rs2_stream& 类型, int& 流索引) {
    SDK错误槽 配置错误;
    const auto* 配置 = rs2_get_frame_stream_profile(帧, 配置错误.接收());
    if (配置错误.失败() || 配置 == nullptr) {
        return false;
    }
    rs2_format 格式 = RS2_FORMAT_ANY;
    int 唯一编号 = 0;
    int 帧率 = 0;
    SDK错误槽 数据错误;
    rs2_get_stream_profile_data(
        配置, &类型, &格式, &流索引, &唯一编号, &帧率, 数据错误.接收());
    return !数据错误.失败();
}

bool 复制图像平面(
    const rs2_frame* 帧,
    D455流类型 类型,
    std::uint32_t 流索引,
    D455像素格式 格式,
    float 深度比例,
    D455图像平面材料& 材料) {
    SDK错误槽 配置错误;
    const auto* 流配置 = rs2_get_frame_stream_profile(帧, 配置错误.接收());
    if (配置错误.失败() || 流配置 == nullptr) {
        return false;
    }
    int 宽 = 0;
    int 高 = 0;
    int 步长 = 0;
    int 位数 = 0;
    int 字节数 = 0;
    SDK错误槽 宽错误;
    宽 = rs2_get_frame_width(帧, 宽错误.接收());
    SDK错误槽 高错误;
    高 = rs2_get_frame_height(帧, 高错误.接收());
    SDK错误槽 步长错误;
    步长 = rs2_get_frame_stride_in_bytes(帧, 步长错误.接收());
    SDK错误槽 位数错误;
    位数 = rs2_get_frame_bits_per_pixel(帧, 位数错误.接收());
    SDK错误槽 字节错误;
    字节数 = rs2_get_frame_data_size(帧, 字节错误.接收());
    if (宽错误.失败() || 高错误.失败() || 步长错误.失败()
        || 位数错误.失败() || 字节错误.失败()
        || 宽 <= 0 || 高 <= 0 || 步长 <= 0 || 位数 <= 0 || 位数 % 8 != 0 || 字节数 <= 0) {
        return false;
    }
    SDK错误槽 数据错误;
    const void* 数据 = rs2_get_frame_data(帧, 数据错误.接收());
    SDK错误槽 编号错误;
    const auto 帧编号 = rs2_get_frame_number(帧, 编号错误.接收());
    SDK错误槽 时间错误;
    const double 时间戳 = rs2_get_frame_timestamp(帧, 时间错误.接收());
    SDK错误槽 时间域错误;
    const auto 时间域 = rs2_get_frame_timestamp_domain(帧, 时间域错误.接收());
    rs2_intrinsics 内参{};
    SDK错误槽 内参错误;
    rs2_get_video_stream_intrinsics(流配置, &内参, 内参错误.接收());
    if (数据错误.失败() || 编号错误.失败() || 时间错误.失败()
        || 时间域错误.失败() || 内参错误.失败() || 数据 == nullptr
        || 帧编号 == 0 || !std::isfinite(时间戳) || 时间戳 <= 0.0) {
        return false;
    }

    材料.流类型 = 类型;
    材料.流索引 = 流索引;
    材料.像素格式 = 格式;
    材料.宽 = static_cast<std::uint32_t>(宽);
    材料.高 = static_cast<std::uint32_t>(高);
    材料.每像素字节数 = static_cast<std::uint32_t>(位数 / 8);
    材料.步长 = static_cast<std::uint32_t>(步长);
    材料.实际字节数 = static_cast<std::uint64_t>(字节数);
    材料.源帧编号 = static_cast<std::uint64_t>(帧编号);
    材料.设备时间戳微秒 = static_cast<std::uint64_t>(std::llround(时间戳 * 1000.0));
    材料.时间戳域 = 转换时间戳域(时间域);
    材料.相机内参 = 转换内参(内参);
    材料.深度比例 = 深度比例;
    材料.只读字节组.resize(static_cast<std::size_t>(字节数));
    std::memcpy(材料.只读字节组.data(), 数据, 材料.只读字节组.size());
    return D455图像平面有效(材料);
}

bool 读取外参(
    const rs2_frame* 来源帧,
    const rs2_frame* 目标帧,
    D455流类型 来源类型,
    std::uint32_t 来源索引,
    D455流类型 目标类型,
    std::uint32_t 目标索引,
    D455相机外参材料& 材料) {
    SDK错误槽 来源错误;
    const auto* 来源配置 = rs2_get_frame_stream_profile(来源帧, 来源错误.接收());
    SDK错误槽 目标错误;
    const auto* 目标配置 = rs2_get_frame_stream_profile(目标帧, 目标错误.接收());
    if (来源错误.失败() || 目标错误.失败() || 来源配置 == nullptr || 目标配置 == nullptr) {
        return false;
    }
    rs2_extrinsics 外参{};
    SDK错误槽 外参错误;
    rs2_get_extrinsics(来源配置, 目标配置, &外参, 外参错误.接收());
    if (外参错误.失败()) {
        return false;
    }
    材料 = 转换外参(外参, 来源类型, 来源索引, 目标类型, 目标索引);
    return D455相机外参有效(材料);
}

std::uint64_t 计算标定摘要(const D455同步帧材料& 材料) {
    std::uint64_t 摘要 = 1469598103934665603ULL;
    const auto 混合 = [&](std::uint64_t 数值) {
        摘要 ^= 数值;
        摘要 *= 1099511628211ULL;
    };
    for (const unsigned char 字节 : 材料.设备身份.序列号) {
        混合(字节);
    }
    混合(材料.彩色.宽);
    混合(材料.彩色.高);
    混合(材料.深度.宽);
    混合(材料.深度.高);
    for (const float 分量 : 材料.深度到彩色.旋转矩阵) {
        混合(static_cast<std::uint64_t>(std::llround(std::fabs(分量) * 1'000'000.0F)));
    }
    return 摘要 == 0 ? 1 : 摘要;
}

}

class D455相机采集器 final : public D455帧来源 {
public:
    explicit D455相机采集器(D455采集配置 配置)
        : 请求配置_(std::move(配置)) {
    }

    ~D455相机采集器() override {
        关闭();
    }

    D455相机采集器(const D455相机采集器&) = delete;
    D455相机采集器& operator=(const D455相机采集器&) = delete;
    D455相机采集器(D455相机采集器&&) = delete;
    D455相机采集器& operator=(D455相机采集器&&) = delete;

    D455操作结果 打开() {
        if (!D455采集配置有效(请求配置_)) {
            return {false, false, D455采集拒绝原因::配置无效};
        }
        {
            std::scoped_lock 锁(状态互斥_);
            if (状态_ == D455采集器状态::运行中) {
                return {true, false, D455采集拒绝原因::无};
            }
            if (状态_ != D455采集器状态::未打开) {
                return {false, false, D455采集拒绝原因::已停止};
            }
        }

        try {
            SDK错误槽 上下文错误;
            上下文_.reset(rs2_create_context(RealSense接口版本, 上下文错误.接收()));
            if (上下文错误.失败() || !上下文_) {
                return 打开失败(D455采集拒绝原因::无设备, false);
            }
            SDK错误槽 列表错误;
            设备列表指针 设备组(rs2_query_devices(上下文_.get(), 列表错误.接收()));
            if (列表错误.失败() || !设备组) {
                return 打开失败(D455采集拒绝原因::无设备, false);
            }
            SDK错误槽 数量错误;
            const int 设备数量 = rs2_get_device_count(设备组.get(), 数量错误.接收());
            if (数量错误.失败()) {
                return 打开失败(D455采集拒绝原因::内部不一致, true);
            }

            设备指针 选中设备;
            for (int 索引 = 0; 索引 < 设备数量; ++索引) {
                SDK错误槽 创建设备错误;
                设备指针 当前设备(rs2_create_device(设备组.get(), 索引, 创建设备错误.接收()));
                if (创建设备错误.失败() || !当前设备) {
                    return 打开失败(D455采集拒绝原因::内部不一致, true);
                }
                bool 调用成功 = true;
                const auto 当前序列号 = 读取设备信息(
                    当前设备.get(), RS2_CAMERA_INFO_SERIAL_NUMBER, 调用成功);
                if (!调用成功) {
                    return 打开失败(D455采集拒绝原因::内部不一致, true);
                }
                if (!请求配置_.设备序列号.empty()
                    && 当前序列号 == 请求配置_.设备序列号) {
                    if (!是D455设备(当前设备.get(), 调用成功)) {
                        return 打开失败(
                            调用成功 ? D455采集拒绝原因::指定设备不是D455
                                : D455采集拒绝原因::内部不一致,
                            !调用成功);
                    }
                    选中设备 = std::move(当前设备);
                    break;
                }
                if (!请求配置_.设备序列号.empty()) {
                    continue;
                }
                if (!是D455设备(当前设备.get(), 调用成功)) {
                    if (!调用成功) {
                        return 打开失败(D455采集拒绝原因::内部不一致, true);
                    }
                    continue;
                }
                if (选中设备) {
                    return 打开失败(D455采集拒绝原因::多设备歧义, false);
                }
                选中设备 = std::move(当前设备);
            }
            if (!选中设备) {
                return 打开失败(
                    请求配置_.设备序列号.empty()
                        ? D455采集拒绝原因::无设备
                        : D455采集拒绝原因::指定设备不存在,
                    false);
            }

            bool 身份读取成功 = true;
            const auto 身份 = 转换设备身份(选中设备.get(), 身份读取成功);
            if (!身份读取成功 || !D455设备身份有效(身份)) {
                return 打开失败(D455采集拒绝原因::内部不一致, true);
            }
            float 深度比例 = 0.0F;
            if (!读取深度比例(选中设备.get(), 深度比例)) {
                return 打开失败(D455采集拒绝原因::内部不一致, true);
            }

            SDK错误槽 管线错误;
            管线_.reset(rs2_create_pipeline(上下文_.get(), 管线错误.接收()));
            SDK错误槽 配置错误;
            配置指针 配置(rs2_create_config(配置错误.接收()));
            if (管线错误.失败() || 配置错误.失败() || !管线_ || !配置) {
                return 打开失败(D455采集拒绝原因::内部不一致, true);
            }
            SDK错误槽 设备配置错误;
            rs2_config_enable_device(配置.get(), 身份.序列号.c_str(), 设备配置错误.接收());
            if (设备配置错误.失败()) {
                return 打开失败(D455采集拒绝原因::配置不支持, false);
            }
            const auto 启用流 = [&](const D455流配置& 流) {
                SDK错误槽 流错误;
                rs2_config_enable_stream(
                    配置.get(), 转换流类型(流.类型), static_cast<int>(流.流索引),
                    static_cast<int>(流.宽), static_cast<int>(流.高),
                    转换像素格式(流.格式), static_cast<int>(流.帧率), 流错误.接收());
                return !流错误.失败();
            };
            if (!启用流(请求配置_.彩色)
                || !启用流(请求配置_.深度)
                || !启用流(请求配置_.左红外)
                || !启用流(请求配置_.右红外)) {
                return 打开失败(D455采集拒绝原因::配置不支持, false);
            }

            SDK错误槽 启动错误;
            管线配置指针 管线配置(
                rs2_pipeline_start_with_config(管线_.get(), 配置.get(), 启动错误.接收()));
            if (启动错误.失败() || !管线配置) {
                return 打开失败(D455采集拒绝原因::配置不支持, false);
            }
            已启动管线_ = true;
            if (!复核实际流配置(管线配置.get(), 请求配置_)) {
                return 打开失败(D455采集拒绝原因::内部不一致, true);
            }
            SDK错误槽 实际设备错误;
            设备指针 实际设备(
                rs2_pipeline_profile_get_device(管线配置.get(), 实际设备错误.接收()));
            if (实际设备错误.失败() || !实际设备) {
                return 打开失败(D455采集拒绝原因::内部不一致, true);
            }
            bool 实际身份成功 = true;
            const auto 实际身份 = 转换设备身份(实际设备.get(), 实际身份成功);
            if (!实际身份成功 || 实际身份.序列号 != 身份.序列号) {
                return 打开失败(D455采集拒绝原因::内部不一致, true);
            }

            std::scoped_lock 锁(状态互斥_);
            设备身份_ = 实际身份;
            实际配置_ = 请求配置_;
            实际配置_.设备序列号 = 实际身份.序列号;
            深度比例_ = 深度比例;
            停止请求_.store(false);
            状态_ = D455采集器状态::运行中;
            return {true, false, D455采集拒绝原因::无};
        }
        catch (...) {
            return 打开失败(D455采集拒绝原因::内部不一致, true);
        }
    }

    D455采集结果 采样一次(std::uint32_t 超时毫秒) override {
        if (超时毫秒 == 0) {
            return {false, false, D455采集拒绝原因::配置无效, {}};
        }
        {
            std::scoped_lock 锁(状态互斥_);
            if (停止请求_.load()
                || 状态_ == D455采集器状态::停止请求
                || 状态_ == D455采集器状态::已关闭) {
                return {false, false, D455采集拒绝原因::已停止, {}};
            }
            if (状态_ != D455采集器状态::运行中 || !已启动管线_ || !管线_) {
                return {false, false, D455采集拒绝原因::当前未运行, {}};
            }
        }

        try {
            rs2_frame* 原同步帧 = nullptr;
            SDK错误槽 等待错误;
            const int 已取得 = rs2_pipeline_try_wait_for_frames(
                管线_.get(), &原同步帧, 超时毫秒, 等待错误.接收());
            帧指针 同步帧(原同步帧);
            if (等待错误.失败()) {
                return 停止请求_.load()
                    ? D455采集结果{false, false, D455采集拒绝原因::已停止, {}}
                    : D455采集结果{false, false, D455采集拒绝原因::设备断开, {}};
            }
            if (已取得 == 0 || !同步帧) {
                return {false, false, D455采集拒绝原因::等待超时, {}};
            }
            if (停止请求_.load()) {
                return {false, false, D455采集拒绝原因::已停止, {}};
            }

            SDK错误槽 数量错误;
            const int 帧数量 = rs2_embedded_frames_count(同步帧.get(), 数量错误.接收());
            if (数量错误.失败() || 帧数量 <= 0) {
                return {false, false, D455采集拒绝原因::同步帧不完整, {}};
            }
            帧指针 彩色帧;
            帧指针 深度帧;
            帧指针 左红外帧;
            帧指针 右红外帧;
            for (int 索引 = 0; 索引 < 帧数量; ++索引) {
                SDK错误槽 提取错误;
                帧指针 当前帧(rs2_extract_frame(同步帧.get(), 索引, 提取错误.接收()));
                if (提取错误.失败() || !当前帧) {
                    return {false, false, D455采集拒绝原因::同步帧不完整, {}};
                }
                rs2_stream 类型 = RS2_STREAM_ANY;
                int 流索引 = 0;
                if (!读取帧流身份(当前帧.get(), 类型, 流索引)) {
                    return {false, true, D455采集拒绝原因::内部不一致, {}};
                }
                if (类型 == RS2_STREAM_COLOR && 流索引 == 0 && !彩色帧) {
                    彩色帧 = std::move(当前帧);
                }
                else if (类型 == RS2_STREAM_DEPTH && 流索引 == 0 && !深度帧) {
                    深度帧 = std::move(当前帧);
                }
                else if (类型 == RS2_STREAM_INFRARED && 流索引 == 1 && !左红外帧) {
                    左红外帧 = std::move(当前帧);
                }
                else if (类型 == RS2_STREAM_INFRARED && 流索引 == 2 && !右红外帧) {
                    右红外帧 = std::move(当前帧);
                }
            }
            if (!彩色帧 || !深度帧 || !左红外帧 || !右红外帧) {
                return {false, false, D455采集拒绝原因::同步帧不完整, {}};
            }

            auto 材料 = std::make_shared<D455同步帧材料>();
            {
                std::scoped_lock 锁(状态互斥_);
                材料->设备身份 = 设备身份_;
                材料->批次号 = ++已完成批次数_;
            }
            材料->宿主接收单调时间戳 = 当前单调时间戳微秒();
            if (!复制图像平面(彩色帧.get(), D455流类型::彩色, 0, D455像素格式::RGB8, 0.0F, 材料->彩色)
                || !复制图像平面(深度帧.get(), D455流类型::深度, 0, D455像素格式::Z16, 深度比例_, 材料->深度)
                || !复制图像平面(左红外帧.get(), D455流类型::红外, 1, D455像素格式::Y8, 0.0F, 材料->左红外)
                || !复制图像平面(右红外帧.get(), D455流类型::红外, 2, D455像素格式::Y8, 0.0F, 材料->右红外)
                || !读取外参(深度帧.get(), 彩色帧.get(), D455流类型::深度, 0, D455流类型::彩色, 0, 材料->深度到彩色)
                || !读取外参(左红外帧.get(), 深度帧.get(), D455流类型::红外, 1, D455流类型::深度, 0, 材料->左红外到深度)
                || !读取外参(右红外帧.get(), 深度帧.get(), D455流类型::红外, 2, D455流类型::深度, 0, 材料->右红外到深度)) {
                return {false, true, D455采集拒绝原因::材料无效, {}};
            }
            材料->标定版本摘要 = 计算标定摘要(*材料);
            if (!D455同步帧材料有效(*材料)) {
                return {false, true, D455采集拒绝原因::材料无效, {}};
            }
            return {true, false, D455采集拒绝原因::无, std::move(材料)};
        }
        catch (...) {
            std::scoped_lock 锁(状态互斥_);
            状态_ = D455采集器状态::故障;
            return {false, true, D455采集拒绝原因::内部不一致, {}};
        }
    }

    void 请求停止() noexcept override {
        停止请求_.store(true);
        std::scoped_lock 锁(状态互斥_);
        if (状态_ == D455采集器状态::运行中) {
            状态_ = D455采集器状态::停止请求;
        }
    }

    void 关闭() noexcept override {
        停止请求_.store(true);
        bool 停止失败 = false;
        if (已启动管线_ && 管线_) {
            SDK错误槽 停止错误;
            rs2_pipeline_stop(管线_.get(), 停止错误.接收());
            停止失败 = 停止错误.失败();
            已启动管线_ = false;
        }
        管线_.reset();
        上下文_.reset();
        std::scoped_lock 锁(状态互斥_);
        状态_ = 停止失败 ? D455采集器状态::故障 : D455采集器状态::已关闭;
    }

    D455采集状态快照 读取状态快照() const override {
        std::scoped_lock 锁(状态互斥_);
        return {状态_, 设备身份_, 实际配置_, 已完成批次数_};
    }

private:
    D455操作结果 打开失败(D455采集拒绝原因 原因, bool 追根因) noexcept {
        if (已启动管线_ && 管线_) {
            SDK错误槽 停止错误;
            rs2_pipeline_stop(管线_.get(), 停止错误.接收());
            追根因 = 追根因 || 停止错误.失败();
            已启动管线_ = false;
        }
        管线_.reset();
        上下文_.reset();
        std::scoped_lock 锁(状态互斥_);
        状态_ = 追根因 ? D455采集器状态::故障 : D455采集器状态::未打开;
        return {false, 追根因, 原因};
    }

    D455采集配置 请求配置_;
    mutable std::mutex 状态互斥_;
    上下文指针 上下文_;
    管线指针 管线_;
    std::atomic<bool> 停止请求_{false};
    D455采集器状态 状态_ = D455采集器状态::未打开;
    bool 已启动管线_ = false;
    D455设备身份材料 设备身份_;
    D455采集配置 实际配置_;
    std::uint64_t 已完成批次数_ = 0;
    float 深度比例_ = 0.0F;
};

std::unique_ptr<D455帧来源> 创建并打开D455相机采集器(
    D455采集配置 配置,
    D455操作结果& 打开结果) {
    auto 采集器 = std::make_unique<D455相机采集器>(std::move(配置));
    打开结果 = 采集器->打开();
    if (!打开结果.成功) {
        return {};
    }
    return 采集器;
}

}
