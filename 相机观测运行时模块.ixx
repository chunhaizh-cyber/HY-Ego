module;

#include <algorithm>
#include <atomic>
#include <bit>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
export module 相机观测运行时模块;

import D455相机外设模块;
import 主信息定义模块;
import 基础数据类型模块;
import 外设模块;
import 世界树环境模块;
import 日志模块;
import 特征类型定义模块;
import 相机写入工作流模块;
import 相机外设基类模块;
import 虚拟相机外设模块;
import 语素环境模块;


export struct 结构_相机观测运行时配置 {
    bool 启用自我桥接 = true;
    bool 启用世界树桥接写入 = false;
    bool 允许回退到虚拟相机 = true;
    bool 强制虚拟相机 = false;
    bool 实时优先模式 = true;
    int 期望FPS = 30;
    int 宽 = 848;
    int 高 = 480;
    std::uint64_t 桥接间隔帧数 = 5;
    std::uint64_t 观测日志间隔帧数 = 90;
    int 等待取帧超时毫秒 = 50;
};

export struct 结构_相机桥接存在快照 {
    std::int64_t 中心X_mm = 0;
    std::int64_t 中心Y_mm = 0;
    std::int64_t 中心Z_mm = 0;
    std::int64_t 尺寸X_mm = 0;
    std::int64_t 尺寸Y_mm = 0;
    std::int64_t 尺寸Z_mm = 0;
    std::uint64_t 轮廓块数 = 0;
    std::int64_t 跟踪ID = 0;
};

export struct 结构_相机观测运行时快照 {
    bool 运行中 = false;
    bool 使用虚拟相机 = false;
    std::uint64_t 已处理帧数 = 0;
    std::uint64_t 最近桥接候选数 = 0;
    时间戳 最近桥接时间 = 0;
    std::string 最近状态摘要;
};

export class 相机观测运行时类 {
public:
    explicit 相机观测运行时类(结构_相机观测运行时配置 配置 = {}) : 配置_(配置) {}
    ~相机观测运行时类() { 停止(); }

    bool 启动();
    void 请求退出();
    void 等待线程结束();
    void 停止();
    bool 是否正在运行() const { return 运行中_.load(); }
    结构_相机观测运行时快照 快照() const;
    std::vector<结构_相机桥接存在快照> 取最近桥接存在列表() const;

    void 锁定观测目标(const std::string& 存在主键);
    void 解除锁定观测();

private:
    static void 私有_清空场景子节点(场景节点类* 场景, const std::string& 调用点);
    static 场景节点类* 私有_确保桥接场景(
        场景节点类*& 缓存,
        const char* 名称,
        const char* 类型,
        const std::string& 调用点);
    static std::int64_t 私有_解析锁定跟踪ID(const std::string& 存在主键);
    static I64 私有_计算深度有效万分比(const 结构体_原始场景帧* frame);
    static bool 私有_构建深度占用候选(const 结构体_原始场景帧* frame, 结构体_存在观测& 输出);
    static VecIU64 私有_编码Vec3I64(const Vector3D& v);
    相机写入参数 私有_构建处理参数() const;

    bool 私有_提交存在观测到自我观察(const 帧处理结果& out, 时间戳 now);
    void 私有_循环();

private:
    mutable std::mutex 互斥_;
    结构_相机观测运行时配置 配置_{};
    相机帧处理器 处理器_{};
    std::unique_ptr<相机外设基类> 外设_{};
    std::thread 工作线程_{};
    std::atomic_bool 运行中_{ false };
    场景节点类* 自我观察桥输入场景_ = nullptr;
    场景节点类* 自我观察桥输出场景_ = nullptr;
    bool 使用虚拟相机_ = false;
    std::uint64_t 已处理帧数_ = 0;
    std::uint64_t 最近桥接候选数_ = 0;
    时间戳 最近桥接时间_ = 0;
    std::string 最近状态摘要_;
    std::vector<结构_相机桥接存在快照> 最近桥接存在列表_{};
    std::int64_t 锁定跟踪ID_ = 0;
    std::string 锁定目标主键_{};
};

inline bool 相机观测运行时类::启动() {
    if (工作线程_.joinable() && !运行中_.load()) {
        等待线程结束();
    }
    std::lock_guard<std::mutex> 锁(互斥_);
    if (运行中_.load()) return false;

    外设启动参数 启动参数{};
    启动参数.设备ID = "";
    启动参数.期望FPS = 配置_.期望FPS;
    启动参数.宽 = 配置_.宽;
    启动参数.高 = 配置_.高;
    启动参数.扩展.push_back({ "align_to_color", true });

    外设_.reset();
    使用虚拟相机_ = false;
    自我观察桥输入场景_ = nullptr;
    自我观察桥输出场景_ = nullptr;
    已处理帧数_ = 0;
    最近桥接候选数_ = 0;
    最近桥接时间_ = 0;
    最近状态摘要_.clear();

    bool 启动成功 = false;
    if (配置_.强制虚拟相机) {
        外设_ = std::make_unique<虚拟相机外设类>();
        启动成功 = 外设_->启动(启动参数);
        使用虚拟相机_ = 启动成功;
        if (启动成功) {
            最近状态摘要_ = "已按配置启用虚拟相机";
        }
    }
    else {
        外设_ = std::make_unique<D455相机外设类>();
        启动成功 = 外设_->启动(启动参数);
        if (!启动成功 && 配置_.允许回退到虚拟相机) {
            外设_.reset();
            外设_ = std::make_unique<虚拟相机外设类>();
            启动成功 = 外设_->启动(启动参数);
            使用虚拟相机_ = 启动成功;
            if (启动成功) {
                最近状态摘要_ = "D455 启动失败，已回退虚拟相机";
            }
        }
    }

    if (!启动成功) {
        外设_.reset();
        最近状态摘要_ = "相机启动失败";
        return false;
    }

    if (dynamic_cast<虚拟相机外设类*>(外设_.get()) != nullptr) {
        使用虚拟相机_ = true;
    }

    if (最近状态摘要_.empty()) {
        最近状态摘要_ = 使用虚拟相机_ ? "虚拟相机运行中" : "D455 相机运行中";
    }

    处理器_.设置参数(私有_构建处理参数());
    运行中_.store(true);
    工作线程_ = std::thread([this] { 私有_循环(); });
    return true;
}

inline void 相机观测运行时类::请求退出() {
    const bool 原先正在运行 = 运行中_.exchange(false);
    if (!原先正在运行 && !工作线程_.joinable() && !外设_) return;

    {
        std::lock_guard<std::mutex> 锁(互斥_);
        最近状态摘要_ = "停止请求中";
    }

    if (外设_) {
        日志::运行("[相机观测运行时] 请求退出：停止外设开始");
        外设_->停止();
        日志::运行("[相机观测运行时] 请求退出：停止外设完成");
    }
}

inline void 相机观测运行时类::等待线程结束() {
    if (工作线程_.joinable()) {
        if (std::this_thread::get_id() == 工作线程_.get_id()) {
            日志::运行("[相机观测运行时] 当前线程命中采集线程 self-join 风险，等待拥有者稍后回收");
            return;
        }
        日志::运行("[相机观测运行时] 等待采集线程结束开始");
        工作线程_.join();
        日志::运行("[相机观测运行时] 等待采集线程结束完成");
    }

    std::lock_guard<std::mutex> 锁(互斥_);
    外设_.reset();
    最近状态摘要_ = "已停止";
}

inline void 相机观测运行时类::停止() {
    日志::运行("[相机观测运行时] 停止开始");
    请求退出();
    等待线程结束();

    {
        std::lock_guard<std::mutex> 锁(互斥_);
        if (最近状态摘要_.empty()) {
            最近状态摘要_ = "已停止";
        }
    }
    日志::运行("[相机观测运行时] 停止完成");
}

inline 结构_相机观测运行时快照 相机观测运行时类::快照() const {
    std::lock_guard<std::mutex> 锁(互斥_);

    结构_相机观测运行时快照 结果{};
    结果.运行中 = 运行中_.load();
    结果.使用虚拟相机 = 使用虚拟相机_;
    结果.已处理帧数 = 已处理帧数_;
    结果.最近桥接候选数 = 最近桥接候选数_;
    结果.最近桥接时间 = 最近桥接时间_;
    结果.最近状态摘要 = 最近状态摘要_;
    return 结果;
}

inline std::vector<结构_相机桥接存在快照> 相机观测运行时类::取最近桥接存在列表() const {
    std::lock_guard<std::mutex> 锁(互斥_);
    return 最近桥接存在列表_;
}

inline std::int64_t 相机观测运行时类::私有_解析锁定跟踪ID(const std::string& 存在主键) {
    if (存在主键.empty()) {
        return 0;
    }

    std::string 数字段 = 存在主键;
    constexpr auto* 前缀 = "bridge_candidate_";
    if (数字段.rfind(前缀, 0) == 0) {
        数字段 = 数字段.substr(std::char_traits<char>::length(前缀));
    }
    if (数字段.empty()) {
        return 0;
    }

    errno = 0;
    char* 结束 = nullptr;
    const auto 跟踪ID = std::strtoll(数字段.c_str(), &结束, 10);
    if (errno != 0 || !结束 || *结束 != '\0') {
        return 0;
    }
    return static_cast<std::int64_t>(跟踪ID);
}

inline void 相机观测运行时类::锁定观测目标(const std::string& 存在主键) {
    const auto 跟踪ID = 私有_解析锁定跟踪ID(存在主键);
    std::lock_guard<std::mutex> 锁(互斥_);
    锁定目标主键_ = 存在主键;
    锁定跟踪ID_ = 跟踪ID;
    if (跟踪ID > 0) {
        最近状态摘要_ = "观测锁定: track_id=" + std::to_string(static_cast<long long>(跟踪ID));
    }
    else {
        最近状态摘要_ = "观测锁定失败：目标主键未携带有效跟踪ID";
    }
}

inline void 相机观测运行时类::解除锁定观测() {
    std::lock_guard<std::mutex> 锁(互斥_);
    锁定跟踪ID_ = 0;
    锁定目标主键_.clear();
    最近状态摘要_ = 使用虚拟相机_ ? "虚拟相机运行中（已解除观测锁定）" : "D455 相机运行中（已解除观测锁定）";
}

inline void 相机观测运行时类::私有_清空场景子节点(场景节点类* 场景, const std::string& 调用点) {
    while (场景 && 场景->子) {
        auto* child = static_cast<基础信息节点类*>(场景->子);
        if (!世界树.删除节点(child, 调用点)) {
            break;
        }
    }
}

inline 场景节点类* 相机观测运行时类::私有_确保桥接场景(
    场景节点类*& 缓存,
    const char* 名称,
    const char* 类型,
    const std::string& 调用点) {
    if (缓存 && 缓存->主信息) {
        return 缓存;
    }

    auto* parent = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
    if (!parent) return nullptr;

    auto* mi = new 场景节点主信息类();
    mi->名称 = 语素集.添加词性词(名称, "名词");
    mi->类型 = 语素集.添加词性词(类型, "名词");
    mi->最后观测时间 = 结构体_时间戳::当前_微秒();
    缓存 = 世界树.创建场景(parent, mi, 调用点);
    return 缓存;
}

inline I64 相机观测运行时类::私有_计算深度有效万分比(const 结构体_原始场景帧* frame) {
    if (!frame) return 0;
    const auto 总像素 = static_cast<std::uint64_t>(frame->宽度) * static_cast<std::uint64_t>(frame->高度);
    if (总像素 == 0) return 0;

    std::uint64_t 有效像素 = 0;
    if (!frame->深度有效.empty()) {
        for (auto v : frame->深度有效) 有效像素 += (v ? 1ull : 0ull);
    }
    else if (!frame->深度.empty()) {
        for (auto z : frame->深度) 有效像素 += (z > 0.0 ? 1ull : 0ull);
    }
    return static_cast<I64>((有效像素 * 10000ull) / 总像素);
}

inline bool 相机观测运行时类::私有_构建深度占用候选(
    const 结构体_原始场景帧* frame,
    结构体_存在观测& 输出) {
    if (!frame || frame->宽度 <= 0 || frame->高度 <= 0) return false;
    if (frame->深度.empty()) return false;

    const bool 有效掩膜可用 = frame->深度有效.size() == frame->深度.size();
    const bool 点云可用 = frame->点云.size() == frame->深度.size();
    const bool 可用内参 =
        frame->深度内参.有效 &&
        frame->深度内参.fx > 0.0 &&
        frame->深度内参.fy > 0.0;

    Vector3D 最小值{};
    Vector3D 最大值{};
    Vector3D 累加值{};
    bool 已初始化包围盒 = false;
    std::uint64_t 有效样本数 = 0;
    std::uint64_t 总样本数 = 0;

    constexpr int 采样步长 = 4;
    for (int v = 0; v < frame->高度; v += 采样步长) {
        for (int u = 0; u < frame->宽度; u += 采样步长) {
            ++总样本数;
            const auto 索引 = static_cast<std::size_t>(v) * static_cast<std::size_t>(frame->宽度)
                + static_cast<std::size_t>(u);
            if (索引 >= frame->深度.size()) continue;

            const double z = frame->深度[索引];
            const bool 深度有效 = 有效掩膜可用
                ? (frame->深度有效[索引] != 0)
                : (std::isfinite(z) && z > 0.0);
            if (!深度有效) continue;

            Vector3D p{};
            if (点云可用) {
                p = frame->点云[索引];
            }
            if (!std::isfinite(p.z) || p.z <= 0.0) {
                if (!(std::isfinite(z) && z > 0.0)) continue;
                if (可用内参) {
                    p.x = (static_cast<double>(u) - frame->深度内参.cx) * z / frame->深度内参.fx;
                    p.y = (static_cast<double>(v) - frame->深度内参.cy) * z / frame->深度内参.fy;
                    p.z = z;
                }
                else {
                    p.x = static_cast<double>(u);
                    p.y = static_cast<double>(v);
                    p.z = z;
                }
            }

            if (!已初始化包围盒) {
                最小值 = p;
                最大值 = p;
                已初始化包围盒 = true;
            }
            else {
                最小值.x = std::min(最小值.x, p.x);
                最小值.y = std::min(最小值.y, p.y);
                最小值.z = std::min(最小值.z, p.z);
                最大值.x = std::max(最大值.x, p.x);
                最大值.y = std::max(最大值.y, p.y);
                最大值.z = std::max(最大值.z, p.z);
            }

            累加值.x += p.x;
            累加值.y += p.y;
            累加值.z += p.z;
            ++有效样本数;
        }
    }

    if (有效样本数 < 16) return false;

    输出 = {};
    const auto 有效样本数f = static_cast<double>(有效样本数);
    输出.中心坐标 = Vector3D{
        累加值.x / 有效样本数f,
        累加值.y / 有效样本数f,
        累加值.z / 有效样本数f
    };
    输出.尺寸 = Vector3D{
        std::max(1.0, 最大值.x - 最小值.x),
        std::max(1.0, 最大值.y - 最小值.y),
        std::max(1.0, 最大值.z - 最小值.z)
    };
    输出.坐标有效 = true;
    输出.AABB_min = 最小值;
    输出.AABB_max = 最大值;
    输出.有效点数 = static_cast<std::uint32_t>(std::min<std::uint64_t>(有效样本数, 0xFFFFFFFFull));
    输出.点云覆盖率有效 = 总样本数 > 0
        ? static_cast<float>(static_cast<double>(有效样本数) / static_cast<double>(总样本数))
        : 0.0f;
    输出.点云覆盖率 = 输出.点云覆盖率有效;
    输出.质量分 = static_cast<double>(输出.点云覆盖率有效);
    输出.跟踪ID = 1;
    输出.时间 = frame->时间戳.系统到达时间_us > 0
        ? static_cast<时间戳>(frame->时间戳.系统到达时间_us)
        : static_cast<时间戳>(frame->时间戳.设备时间_us);
    输出.轮廓观测有效 = true;
    输出.重新计算距离();
    return true;
}

inline VecIU64 相机观测运行时类::私有_编码Vec3I64(const Vector3D& v) {
    return VecIU64{
        std::bit_cast<std::uint64_t>(static_cast<std::int64_t>(std::llround(v.x))),
        std::bit_cast<std::uint64_t>(static_cast<std::int64_t>(std::llround(v.y))),
        std::bit_cast<std::uint64_t>(static_cast<std::int64_t>(std::llround(v.z)))
    };
}

inline 相机写入参数 相机观测运行时类::私有_构建处理参数() const {
    auto 参数 = 处理器_.参数();
    参数.等待帧超时毫秒 = std::max(1, 配置_.等待取帧超时毫秒);
    if (!配置_.实时优先模式) {
        return 参数;
    }

    // D455 侧已经做过 RealSense 滤波链，这里优先减少重复处理和附加输出。
    参数.启用深度预处理 = false;
    参数.分割.采样步长 = std::max(2, 参数.分割.采样步长);
    参数.分割.输出裁剪图 = false;
    // 存在提取默认要求轮廓编码，轮廓编码依赖掩码；关闭掩码会导致观测恒为空。
    参数.分割.输出裁剪掩码 = true;
    参数.分割.输出边界点序列 = false;
    参数.分割.输出轮廓3D点 = false;
    参数.分割.计算颜色一致性 = false;
    参数.分割.计算深度噪声 = false;
    参数.分割.计算边界噪声 = false;
    参数.分割.计算质量分 = false;
    参数.分割.彩色轮廓采样步长 = std::max(4, 参数.分割.彩色轮廓采样步长);
    参数.分割.彩色轮廓最大点数 = std::max(256, std::min(参数.分割.彩色轮廓最大点数, 2048));
    参数.分割.最小点数 = std::max(16, std::min(参数.分割.最小点数, 40));

    // 实时桥接优先“先有候选”，在弱光/单色场景下放宽过滤门槛，避免长期 0 候选。
    参数.提取.最小有效点数 = std::max<std::size_t>(12, std::min<std::size_t>(参数.提取.最小有效点数, 48));
    参数.提取.最小点云像素覆盖率 = std::min(参数.提取.最小点云像素覆盖率, 0.05);
    参数.提取.最小尺寸 = std::min(参数.提取.最小尺寸, 5.0);
    参数.提取.要求颜色 = false;
    参数.提取.要求轮廓编码 = false;
    参数.提取.生成边缘图 = false;
    参数.提取.生成法线图 = false;
    参数.提取.生成置信度图 = false;
    参数.提取.生成多尺度轮廓 = false;
    return 参数;
}

inline bool 相机观测运行时类::私有_提交存在观测到自我观察(const 帧处理结果& out, 时间戳 now) {
    if (!配置_.启用世界树桥接写入) {
        return true;
    }

    auto* 输入场景 = 私有_确保桥接场景(
        自我观察桥输入场景_,
        "主窗口_自我观察输入",
        "主窗口相机桥接场景",
        "相机观测运行时类::提交存在观测到自我观察/确保输入场景");
    auto* 输出场景 = 私有_确保桥接场景(
        自我观察桥输出场景_,
        "主窗口_自我观察输出",
        "主窗口相机桥接场景",
        "相机观测运行时类::提交存在观测到自我观察/确保输出场景");
    if (!输入场景 || !输出场景) {
        日志::运行("[相机观测运行时] 失败：桥接输入或输出场景创建失败");
        return false;
    }

    if (auto* 输入主信息 = 输入场景 && 输入场景->主信息 ? dynamic_cast<场景节点主信息类*>(输入场景->主信息) : nullptr) {
        输入主信息->最后观测时间 = now;
    }
    if (auto* 输出主信息 = 输出场景 && 输出场景->主信息 ? dynamic_cast<场景节点主信息类*>(输出场景->主信息) : nullptr) {
        输出主信息->最后观测时间 = now;
    }

    static const auto* 深度有效万分比类型 = 语素集.添加词性词("深度有效万分比", "名词");
    static const auto* 候选集名称 = 语素集.添加词性词("候选存在集", "名词");
    static const auto* 候选集类型 = 语素集.添加词性词("候选集场景", "名词");
    static const auto* 候选存在名称 = 语素集.添加词性词("候选存在", "名词");
    static const auto* 候选存在类型 = 语素集.添加词性词("候选存在类型", "名词");
    static const auto* 跟踪ID类型 = 语素集.添加词性词("跟踪ID", "名词");

    (void)世界树.写入特征_I64(
        输入场景,
        深度有效万分比类型,
        私有_计算深度有效万分比(out.原始场景帧.get()),
        {},
        "相机观测运行时类::提交存在观测到自我观察/写深度有效万分比");

    auto* 候选场景 = 世界树.取或创建子场景_按名称(
        输入场景,
        候选集名称,
        now,
        "相机观测运行时类::提交存在观测到自我观察/取或创建候选场景");
    if (!候选场景) {
        日志::运行("[相机观测运行时] 失败：候选场景创建失败");
        return false;
    }
    if (auto* 候选主信息 = 候选场景->主信息 ? dynamic_cast<场景节点主信息类*>(候选场景->主信息) : nullptr) {
        候选主信息->名称 = 候选集名称;
        候选主信息->类型 = 候选集类型;
        候选主信息->最后观测时间 = now;
    }
    // 还没有明确“自我所在场景”时，先把相机桥接候选集作为最小可回放场景。
    if (!世界树.获取自我所在场景()) {
        世界树.自我所在场景 = 候选场景;
    }
    私有_清空场景子节点(候选场景, "相机观测运行时类::提交存在观测到自我观察/清空候选");

    std::vector<结构_相机桥接存在快照> 最新桥接存在列表{};
    最新桥接存在列表.reserve(out.存在观测列表.size());

    for (const auto& obs : out.存在观测列表) {
        最新桥接存在列表.push_back(结构_相机桥接存在快照{
            static_cast<std::int64_t>(std::llround(obs.中心坐标.x)),
            static_cast<std::int64_t>(std::llround(obs.中心坐标.y)),
            static_cast<std::int64_t>(std::llround(obs.中心坐标.z)),
            static_cast<std::int64_t>(std::llround(obs.尺寸.x)),
            static_cast<std::int64_t>(std::llround(obs.尺寸.y)),
            static_cast<std::int64_t>(std::llround(obs.尺寸.z)),
            static_cast<std::uint64_t>(obs.轮廓编码.size()),
            static_cast<std::int64_t>(obs.跟踪ID)
            });

        auto* miE = new 存在节点主信息类();
        miE->名称 = 候选存在名称;
        miE->类型 = 候选存在类型;
        miE->最后观测时间 = now;
        auto* e = 世界树.创建存在(
            候选场景,
            miE,
            "相机观测运行时类::提交存在观测到自我观察/创建候选存在");
        if (!e) continue;

        (void)世界树.写入特征_VecU(
            e,
            特征类型定义类::类型_绝对位置,
            私有_编码Vec3I64(obs.中心坐标),
            {},
            "相机观测运行时类::提交存在观测到自我观察/写中心");
        (void)世界树.写入特征_VecU(
            e,
            特征类型定义类::类型_尺寸,
            私有_编码Vec3I64(obs.尺寸),
            {},
            "相机观测运行时类::提交存在观测到自我观察/写尺寸");
        if (!obs.轮廓编码.empty()) {
            (void)世界树.写入特征_VecU(
                e,
                特征类型定义类::类型_轮廓编码,
                obs.轮廓编码,
                {},
                "相机观测运行时类::提交存在观测到自我观察/写轮廓");
        }
        (void)世界树.写入特征_I64(
            e,
            跟踪ID类型,
            static_cast<I64>(obs.跟踪ID),
            {},
            "相机观测运行时类::提交存在观测到自我观察/写跟踪ID");
        (void)世界树.写入特征_I64(
            e,
            特征类型定义类::类型_时间戳_us,
            static_cast<I64>(now),
            {},
            "相机观测运行时类::提交存在观测到自我观察/写时间戳");
    }

    {
        std::lock_guard<std::mutex> 锁(互斥_);
        最近桥接候选数_ = out.存在观测列表.size();
        最近桥接时间_ = now;
        最近桥接存在列表_ = std::move(最新桥接存在列表);
    }

    return true;
}

inline void 相机观测运行时类::私有_循环() {
    外设数据包<结构体_原始场景帧> pkg{};
    帧处理结果 out{};
    日志::运行("[相机观测运行时] 采集线程启动");
    日志::运行f(
        "[相机观测运行时] 运行参数: 启用自我桥接={}, 启用世界树桥接写入={}, 桥接间隔帧数={}, 观测日志间隔帧数={}",
        配置_.启用自我桥接,
        配置_.启用世界树桥接写入,
        static_cast<unsigned long long>(配置_.桥接间隔帧数),
        static_cast<unsigned long long>(配置_.观测日志间隔帧数));

    std::uint64_t 处理帧计数 = 0;
    std::uint64_t 桥接计数 = 0;
    while (运行中_.load()) {
        auto r = 外设_ ? 外设_->等待下一包(pkg, 配置_.等待取帧超时毫秒) : 枚举_取数结果::已停止;

        if (r != 枚举_取数结果::成功) {
            if (r == 枚举_取数结果::已停止 || r == 枚举_取数结果::故障) {
                break;
            }
            continue;
        }

        ++处理帧计数;
        out.原始场景帧 = std::make_shared<结构体_原始场景帧>(std::move(pkg.数据));
        const bool 处理成功 = 处理器_.处理一帧(out);
        bool 兜底候选成功 = false;
        if (!处理成功) {
            结构体_存在观测 粗候选{};
            if (私有_构建深度占用候选(out.原始场景帧.get(), 粗候选)) {
                out.存在观测列表.push_back(std::move(粗候选));
                兜底候选成功 = true;
            }
        }

        std::int64_t 当前锁定跟踪ID = 0;
        {
            std::lock_guard<std::mutex> 锁(互斥_);
            当前锁定跟踪ID = 锁定跟踪ID_;
        }
        if (当前锁定跟踪ID > 0 && !out.存在观测列表.empty()) {
            const auto 新末 = std::remove_if(
                out.存在观测列表.begin(),
                out.存在观测列表.end(),
                [当前锁定跟踪ID](const 结构体_存在观测& obs) {
                    return static_cast<std::int64_t>(obs.跟踪ID) != 当前锁定跟踪ID;
                });
            out.存在观测列表.erase(新末, out.存在观测列表.end());
        }

        std::vector<结构_相机桥接存在快照> 当前桥接存在列表{};
        当前桥接存在列表.reserve(out.存在观测列表.size());
        for (const auto& obs : out.存在观测列表) {
            当前桥接存在列表.push_back(结构_相机桥接存在快照{
                static_cast<std::int64_t>(std::llround(obs.中心坐标.x)),
                static_cast<std::int64_t>(std::llround(obs.中心坐标.y)),
                static_cast<std::int64_t>(std::llround(obs.中心坐标.z)),
                static_cast<std::int64_t>(std::llround(obs.尺寸.x)),
                static_cast<std::int64_t>(std::llround(obs.尺寸.y)),
                static_cast<std::int64_t>(std::llround(obs.尺寸.z)),
                static_cast<std::uint64_t>(obs.轮廓编码.size()),
                static_cast<std::int64_t>(obs.跟踪ID)
                });
        }
        const auto 当前桥接时间 = 结构体_时间戳::当前_微秒();
        {
            std::lock_guard<std::mutex> 锁(互斥_);
            已处理帧数_ = 处理帧计数;
            最近桥接候选数_ = out.存在观测列表.size();
            最近桥接时间_ = 当前桥接时间;
            最近桥接存在列表_ = std::move(当前桥接存在列表);
        }

        const bool 记录观测日志 =
            (处理帧计数 <= 3) ||
            (配置_.观测日志间隔帧数 > 0 && (处理帧计数 % 配置_.观测日志间隔帧数) == 0);
        if (记录观测日志) {
            std::size_t 深度有效数 = 0;
            if (out.原始场景帧 && !out.原始场景帧->深度.empty()) {
                if (!out.原始场景帧->深度有效.empty()) {
                    for (auto v : out.原始场景帧->深度有效) {
                        深度有效数 += (v ? 1u : 0u);
                    }
                }
                else {
                    for (auto z : out.原始场景帧->深度) {
                        深度有效数 += (z > 0.0 ? 1u : 0u);
                    }
                }
            }
            const auto 总像素数 = out.原始场景帧
                ? static_cast<std::uint64_t>(out.原始场景帧->宽度) * static_cast<std::uint64_t>(out.原始场景帧->高度)
                : 0ull;
            const auto 深度有效万分比 = 总像素数 > 0
                ? static_cast<std::uint64_t>((深度有效数 * 10000ull) / 总像素数)
                : 0ull;
            const auto 帧号 = out.原始场景帧 ? out.原始场景帧->时间戳.深度帧号 : 0u;
            const auto 深度样本数 = out.原始场景帧 ? out.原始场景帧->深度.size() : 0ull;
            const auto 点云样本数 = out.原始场景帧 ? out.原始场景帧->点云.size() : 0ull;
            日志::运行f(
                "[相机观测运行时] 帧={}, 处理成功={}, 兜底候选={}, 存在观测={}, 轮廓={}, 深度有效万分比={}, 深度样本={}, 点云样本={}",
                帧号,
                处理成功,
                兜底候选成功,
                out.存在观测列表.size(),
                out.轮廓观测列表.size(),
                深度有效万分比,
                static_cast<unsigned long long>(深度样本数),
                static_cast<unsigned long long>(点云样本数));
        }

        if (配置_.启用自我桥接 &&
            配置_.启用世界树桥接写入 &&
            配置_.桥接间隔帧数 > 0 &&
            (处理帧计数 % 配置_.桥接间隔帧数) == 0) {
            const auto 桥接时间 = 结构体_时间戳::当前_微秒();
            日志::运行f(
                "[相机观测运行时] 自我桥接开始：处理帧计数={}, 候选={}",
                处理帧计数,
                out.存在观测列表.size());
            const bool 桥接成功 = 私有_提交存在观测到自我观察(out, 桥接时间);
            ++桥接计数;
            if (桥接计数 <= 3 || 记录观测日志) {
                日志::运行f(
                    "[相机观测运行时] 自我桥接{}：处理帧计数={}, 候选={}, 时间={}",
                    桥接成功 ? "成功" : "失败",
                    处理帧计数,
                    out.存在观测列表.size(),
                    static_cast<long long>(桥接时间));
            }
        }
    }

    {
        std::lock_guard<std::mutex> 锁(互斥_);
        if (最近状态摘要_ != "停止中") {
            最近状态摘要_ = 运行中_.load() ? 最近状态摘要_ : "采集线程已退出";
        }
    }
    运行中_.store(false);
    日志::运行("[相机观测运行时] 采集线程退出");
}
