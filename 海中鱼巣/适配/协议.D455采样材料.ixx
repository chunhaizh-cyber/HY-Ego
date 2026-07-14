// 文件规则：D455 采样材料协议只定义 SDK 无关值式材料与帧来源合同；不得包含 RealSense、线程、仓库或领域服务。
module;

#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

export module 海中鱼巣.适配.协议.D455采样材料;

export namespace 海中鱼巣 {

inline constexpr std::uint32_t D455采样材料协议版本 = 1;

enum class D455流类型 : std::uint8_t {
    未知 = 0,
    彩色 = 1,
    深度 = 2,
    红外 = 3
};

enum class D455像素格式 : std::uint8_t {
    未知 = 0,
    RGB8 = 1,
    Z16 = 2,
    Y8 = 3
};

enum class D455时间戳域 : std::uint8_t {
    未知 = 0,
    硬件时钟 = 1,
    系统时间 = 2,
    全局时间 = 3
};

enum class D455采集器状态 : std::uint8_t {
    未打开 = 0,
    运行中 = 1,
    停止请求 = 2,
    已关闭 = 3,
    故障 = 4
};

enum class D455采集拒绝原因 : std::uint8_t {
    无 = 0,
    配置无效 = 1,
    当前未运行 = 2,
    已停止 = 3,
    无设备 = 4,
    多设备歧义 = 5,
    指定设备不存在 = 6,
    指定设备不是D455 = 7,
    配置不支持 = 8,
    等待超时 = 9,
    设备断开 = 10,
    同步帧不完整 = 11,
    材料无效 = 12,
    内部不一致 = 13
};

struct D455流配置 {
    D455流类型 类型 = D455流类型::未知;
    std::uint32_t 流索引 = 0;
    D455像素格式 格式 = D455像素格式::未知;
    std::uint32_t 宽 = 0;
    std::uint32_t 高 = 0;
    std::uint32_t 帧率 = 0;
};

struct D455采集配置 {
    std::string 设备序列号;
    D455流配置 彩色;
    D455流配置 深度;
    D455流配置 左红外;
    D455流配置 右红外;
    std::uint32_t 采样超时毫秒 = 0;
};

struct D455设备身份材料 {
    std::string 名称;
    std::string 序列号;
    std::string 固件版本;
    std::string 产品编号;
};

struct D455相机内参材料 {
    std::uint32_t 宽 = 0;
    std::uint32_t 高 = 0;
    float 主点横坐标 = 0.0F;
    float 主点纵坐标 = 0.0F;
    float 横向焦距 = 0.0F;
    float 纵向焦距 = 0.0F;
    std::int32_t 畸变模型 = 0;
    std::array<float, 5> 畸变系数组{};
};

struct D455相机外参材料 {
    D455流类型 来源类型 = D455流类型::未知;
    std::uint32_t 来源索引 = 0;
    D455流类型 目标类型 = D455流类型::未知;
    std::uint32_t 目标索引 = 0;
    std::array<float, 9> 旋转矩阵{};
    std::array<float, 3> 平移向量{};
};

struct D455图像平面材料 {
    D455流类型 流类型 = D455流类型::未知;
    std::uint32_t 流索引 = 0;
    D455像素格式 像素格式 = D455像素格式::未知;
    std::uint32_t 宽 = 0;
    std::uint32_t 高 = 0;
    std::uint32_t 每像素字节数 = 0;
    std::uint32_t 步长 = 0;
    std::uint64_t 实际字节数 = 0;
    std::uint64_t 源帧编号 = 0;
    std::uint64_t 设备时间戳微秒 = 0;
    D455时间戳域 时间戳域 = D455时间戳域::未知;
    std::vector<std::byte> 只读字节组;
    D455相机内参材料 相机内参;
    float 深度比例 = 0.0F;
};

struct D455同步帧材料 {
    D455设备身份材料 设备身份;
    std::uint64_t 宿主接收单调时间戳 = 0;
    std::uint64_t 批次号 = 0;
    std::uint64_t 标定版本摘要 = 0;
    D455图像平面材料 彩色;
    D455图像平面材料 深度;
    D455图像平面材料 左红外;
    D455图像平面材料 右红外;
    D455相机外参材料 深度到彩色;
    D455相机外参材料 左红外到深度;
    D455相机外参材料 右红外到深度;
};

struct D455操作结果 {
    bool 成功 = false;
    bool 追根因 = false;
    D455采集拒绝原因 拒绝原因 = D455采集拒绝原因::无;
};

struct D455采集结果 {
    bool 成功 = false;
    bool 追根因 = false;
    D455采集拒绝原因 拒绝原因 = D455采集拒绝原因::无;
    std::shared_ptr<const D455同步帧材料> 材料;
};

struct D455采集状态快照 {
    D455采集器状态 状态 = D455采集器状态::未打开;
    D455设备身份材料 设备身份;
    D455采集配置 实际配置;
    std::uint64_t 已完成批次数 = 0;
};

class D455帧来源 {
public:
    virtual ~D455帧来源() = default;
    virtual D455采集结果 采样一次(std::uint32_t 超时毫秒) = 0;
    virtual void 请求停止() noexcept = 0;
    virtual void 关闭() noexcept = 0;
    virtual D455采集状态快照 读取状态快照() const = 0;
};

inline D455采集配置 生成D455默认采集配置(std::uint32_t 超时毫秒 = 2000) {
    D455采集配置 配置;
    配置.彩色 = {D455流类型::彩色, 0, D455像素格式::RGB8, 1280, 720, 30};
    配置.深度 = {D455流类型::深度, 0, D455像素格式::Z16, 848, 480, 30};
    配置.左红外 = {D455流类型::红外, 1, D455像素格式::Y8, 848, 480, 30};
    配置.右红外 = {D455流类型::红外, 2, D455像素格式::Y8, 848, 480, 30};
    配置.采样超时毫秒 = 超时毫秒;
    return 配置;
}

inline bool D455流配置有效(const D455流配置& 配置) noexcept {
    if (配置.宽 == 0 || 配置.高 == 0 || 配置.帧率 == 0) {
        return false;
    }
    switch (配置.类型) {
    case D455流类型::彩色:
        return 配置.流索引 == 0 && 配置.格式 == D455像素格式::RGB8;
    case D455流类型::深度:
        return 配置.流索引 == 0 && 配置.格式 == D455像素格式::Z16;
    case D455流类型::红外:
        return (配置.流索引 == 1 || 配置.流索引 == 2)
            && 配置.格式 == D455像素格式::Y8;
    default:
        return false;
    }
}

inline bool D455采集配置有效(const D455采集配置& 配置) noexcept {
    return 配置.采样超时毫秒 != 0
        && D455流配置有效(配置.彩色)
        && D455流配置有效(配置.深度)
        && D455流配置有效(配置.左红外)
        && D455流配置有效(配置.右红外)
        && 配置.左红外.流索引 == 1
        && 配置.右红外.流索引 == 2;
}

inline std::uint32_t D455格式每像素字节数(D455像素格式 格式) noexcept {
    switch (格式) {
    case D455像素格式::RGB8:
        return 3;
    case D455像素格式::Z16:
        return 2;
    case D455像素格式::Y8:
        return 1;
    default:
        return 0;
    }
}

inline bool D455设备身份有效(const D455设备身份材料& 身份) noexcept {
    return !身份.名称.empty()
        && !身份.序列号.empty()
        && !身份.固件版本.empty()
        && !身份.产品编号.empty();
}

inline bool D455相机内参有效(const D455相机内参材料& 内参) noexcept {
    if (内参.宽 == 0 || 内参.高 == 0
        || !std::isfinite(内参.主点横坐标)
        || !std::isfinite(内参.主点纵坐标)
        || !std::isfinite(内参.横向焦距)
        || !std::isfinite(内参.纵向焦距)
        || 内参.横向焦距 <= 0.0F
        || 内参.纵向焦距 <= 0.0F) {
        return false;
    }
    for (const float 系数 : 内参.畸变系数组) {
        if (!std::isfinite(系数)) {
            return false;
        }
    }
    return true;
}

inline bool D455相机外参有效(const D455相机外参材料& 外参) noexcept {
    if (外参.来源类型 == D455流类型::未知
        || 外参.目标类型 == D455流类型::未知) {
        return false;
    }
    bool 有旋转分量 = false;
    for (const float 分量 : 外参.旋转矩阵) {
        if (!std::isfinite(分量)) {
            return false;
        }
        有旋转分量 = 有旋转分量 || std::fabs(分量) > 0.000001F;
    }
    for (const float 分量 : 外参.平移向量) {
        if (!std::isfinite(分量)) {
            return false;
        }
    }
    return 有旋转分量;
}

inline bool D455图像平面有效(const D455图像平面材料& 平面) noexcept {
    const auto 预期每像素字节数 = D455格式每像素字节数(平面.像素格式);
    if (平面.流类型 == D455流类型::未知
        || 预期每像素字节数 == 0
        || 平面.宽 == 0
        || 平面.高 == 0
        || 平面.每像素字节数 != 预期每像素字节数
        || 平面.源帧编号 == 0
        || 平面.设备时间戳微秒 == 0
        || 平面.时间戳域 == D455时间戳域::未知
        || !D455相机内参有效(平面.相机内参)
        || 平面.相机内参.宽 != 平面.宽
        || 平面.相机内参.高 != 平面.高) {
        return false;
    }
    const auto 最小步长 = static_cast<std::uint64_t>(平面.宽) * 平面.每像素字节数;
    const auto 预期字节数 = static_cast<std::uint64_t>(平面.步长) * 平面.高;
    if (平面.步长 < 最小步长
        || 平面.实际字节数 != 预期字节数
        || 平面.实际字节数 != 平面.只读字节组.size()) {
        return false;
    }
    return 平面.流类型 == D455流类型::深度
        ? std::isfinite(平面.深度比例) && 平面.深度比例 > 0.0F
        : 平面.深度比例 == 0.0F;
}

inline std::uint64_t 计算D455同步帧字节数(const D455同步帧材料& 材料) noexcept {
    return 材料.彩色.实际字节数
        + 材料.深度.实际字节数
        + 材料.左红外.实际字节数
        + 材料.右红外.实际字节数;
}

inline bool D455同步帧材料有效(const D455同步帧材料& 材料) noexcept {
    return D455设备身份有效(材料.设备身份)
        && 材料.宿主接收单调时间戳 != 0
        && 材料.批次号 != 0
        && 材料.标定版本摘要 != 0
        && D455图像平面有效(材料.彩色)
        && 材料.彩色.流类型 == D455流类型::彩色
        && 材料.彩色.流索引 == 0
        && 材料.彩色.像素格式 == D455像素格式::RGB8
        && D455图像平面有效(材料.深度)
        && 材料.深度.流类型 == D455流类型::深度
        && 材料.深度.流索引 == 0
        && 材料.深度.像素格式 == D455像素格式::Z16
        && D455图像平面有效(材料.左红外)
        && 材料.左红外.流类型 == D455流类型::红外
        && 材料.左红外.流索引 == 1
        && 材料.左红外.像素格式 == D455像素格式::Y8
        && D455图像平面有效(材料.右红外)
        && 材料.右红外.流类型 == D455流类型::红外
        && 材料.右红外.流索引 == 2
        && 材料.右红外.像素格式 == D455像素格式::Y8
        && D455相机外参有效(材料.深度到彩色)
        && 材料.深度到彩色.来源类型 == D455流类型::深度
        && 材料.深度到彩色.来源索引 == 0
        && 材料.深度到彩色.目标类型 == D455流类型::彩色
        && 材料.深度到彩色.目标索引 == 0
        && D455相机外参有效(材料.左红外到深度)
        && 材料.左红外到深度.来源类型 == D455流类型::红外
        && 材料.左红外到深度.来源索引 == 1
        && 材料.左红外到深度.目标类型 == D455流类型::深度
        && 材料.左红外到深度.目标索引 == 0
        && D455相机外参有效(材料.右红外到深度)
        && 材料.右红外到深度.来源类型 == D455流类型::红外
        && 材料.右红外到深度.来源索引 == 2
        && 材料.右红外到深度.目标类型 == D455流类型::深度
        && 材料.右红外到深度.目标索引 == 0
        && 计算D455同步帧字节数(材料) != 0;
}

inline std::uint64_t 计算D455材料摘要(const D455同步帧材料& 材料) noexcept {
    std::uint64_t 摘要 = 1469598103934665603ULL;
    const auto 混合 = [&](std::uint64_t 数值) {
        摘要 ^= 数值;
        摘要 *= 1099511628211ULL;
    };
    for (const unsigned char 字节 : 材料.设备身份.序列号) {
        混合(字节);
    }
    混合(材料.批次号);
    混合(材料.标定版本摘要);
    混合(材料.彩色.源帧编号);
    混合(材料.深度.源帧编号);
    混合(材料.左红外.源帧编号);
    混合(材料.右红外.源帧编号);
    混合(计算D455同步帧字节数(材料));
    return 摘要 == 0 ? 1 : 摘要;
}

}
