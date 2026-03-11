
export module 相机接口模块;

import 外设模块;
import 基础数据类型模块;

import <string>;
import <cstdint>;

// ============================================================================
// 相机接口模块（升级版）
// 目的：
//   - 把“硬件相机驱动层”抽象成统一接口，供 D455/虚拟相机/未来相机复用
//   - 与 外设模块 对齐：支持 打开(外设启动参数) 与 处理外设指令
//   - 兼容旧驱动：仍可只实现 打开()/关闭()/采集一帧()
// ============================================================================

export enum class 枚举_相机能力 : std::uint32_t {
    无 = 0,
    深度 = 1u << 0,
    彩色 = 1u << 1,
    IMU = 1u << 2,
    点云 = 1u << 3,
};

export constexpr inline 枚举_相机能力 operator|(枚举_相机能力 a, 枚举_相机能力 b) {
    return static_cast<枚举_相机能力>(static_cast<std::uint32_t>(a) | static_cast<std::uint32_t>(b));
}
export constexpr inline bool operator&(枚举_相机能力 a, 枚举_相机能力 b) {
    return (static_cast<std::uint32_t>(a) & static_cast<std::uint32_t>(b)) != 0;
}

export struct 相机能力信息 {
    枚举_相机能力 能力 = 枚举_相机能力::无;
    bool 支持指令 = false;
    bool 支持设备时间戳 = false;
};

// ===== 抽象深度相机接口 =====
export class 抽象深度相机接口 {
public:
    virtual ~抽象深度相机接口() = default;

    // 旧接口（必须）
    virtual bool 打开() = 0;
    virtual void 关闭() = 0;
    virtual bool 采集一帧(结构体_原始场景帧& 输出) = 0;

    // 新接口（可选）：默认回退到旧接口
    virtual bool 打开(const 外设启动参数& p) {
        (void)p;
        return 打开();
    }

    // 新接口（可选）：默认不支持
    virtual 外设指令回执 处理外设指令(const 外设指令& cmd) {
        (void)cmd;
        return { false, -1, "该相机驱动不支持指令" };
    }

    // 可选：给外设层提供设备时间戳（微秒/毫秒均可，但请统一）
    virtual 时间戳 获取最后设备时间_微秒() const { return 0; }

    // 能力声明
    virtual 相机能力信息 能力() const {
        return { 枚举_相机能力::深度 | 枚举_相机能力::彩色, false, false };
    }
};
