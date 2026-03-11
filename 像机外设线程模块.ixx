export module 相机外设线程模块;

// 说明：
//  - 本模块只负责：拉取相机外设帧 -> 相机帧处理器(点簇分割/存在提取) -> 发送“消息 + 摘要数据”
//  - 本模块不做“识别完成/认知落账”，不维护任务/方法树。
//  - 为了支持“静态观测稳定化”任务，本模块必须周期性发送“场景观测快照”（即便静态场景没有变化也要发）。
//
// 依赖：
//  - 外设模块：统一取帧/指令语义
//  - 相机写入工作流模块：相机帧处理器 + 帧处理结果(存在观测/轮廓观测)
//  - 存在提取模块/点簇分割模块 等由相机写入工作流模块间接依赖
//
// 重要：
//  - 变量/函数名使用中文；文件名建议英文，避免下载问题。

import 外设模块;
import 相机写入工作流模块;
import 基础数据类型模块;
import 通用函数模块;

import <vector>;
import <string>;
import <variant>;
import <functional>;
import <thread>;
import <atomic>;
import <chrono>;
import <cstdint>;

export enum class 枚举_相机外设消息类型 : std::uint8_t {
    未定义 = 0,
    帧质量更新 = 1,
    场景观测快照 = 2,
    环境异常 = 3,
    设备状态 = 4,
};

export enum class 枚举_相机环境异常码 : std::int32_t {
    未定义 = 0,
    取帧超时过多 = 10,
    连续处理失败 = 20,
    观测为空过多 = 30,
    外设已停止 = 40,
    外设故障 = 50,
};

export struct 结构体_相机帧质量指标 {
    std::string   设备ID{};
    std::uint64_t 帧序号 = 0;
    时间戳        时间_设备 = 0;
    时间戳        时间_系统 = 0;

    double 估计FPS = 0.0;
    double 处理耗时_ms = 0.0;

    std::uint32_t 丢包计数 = 0;
    std::size_t   观测数 = 0;
    std::size_t   轮廓数 = 0;
};

export struct 结构体_相机存在摘要 {
    std::int64_t 跟踪ID = -1;

    Vector3D 中心坐标{};
    Vector3D 尺寸{};
    Vector3D AABB_min{};
    Vector3D AABB_max{};

    // 轮廓签名：对 VecIU64 的稳定哈希（便于统计稳定性，不搬运大数组）
    std::uint64_t 轮廓签名_U64 = 0;

    // 可选：保留原始签名长度，便于诊断
    std::uint32_t 轮廓签名长度 = 0;
};

export struct 结构体_相机场景观测快照 {
    外设数据包头 头{};
    std::vector<结构体_相机存在摘要> 存在列表{};
};

export struct 结构体_相机环境异常 {
    外设数据包头 头{};
    枚举_相机环境异常码 异常码 = 枚举_相机环境异常码::未定义;
    std::string 诊断{};
};

export struct 结构体_相机设备状态 {
    外设状态信息 状态{};
};

export using 相机外设消息载体 = std::variant<
    std::monostate,
    结构体_相机帧质量指标,
    结构体_相机场景观测快照,
    结构体_相机环境异常,
    结构体_相机设备状态
>;

export struct 结构体_相机外设消息 {
    枚举_相机外设消息类型 类型 = 枚举_相机外设消息类型::未定义;
    相机外设消息载体     载荷{};
};

// ============================================================================
// 相机外设线程类：拉帧 + 处理 + 发消息
// ============================================================================
export class 相机外设线程类 final {
public:
    using 回调_投递消息 = std::function<void(const 结构体_相机外设消息&)>;

    struct 参数 {
        // 取帧语义
        int 等待帧超时毫秒 = 50;
        int 空转睡眠毫秒 = 1;

        // 快照节流：每 N 帧发送一次（1=每帧都发）
        int 快照每N帧发送一次 = 1;

        // 异常阈值
        int 连续取帧超时阈值 = 30;
        int 连续处理失败阈值 = 10;
        int 连续观测为空阈值 = 30;

        // 是否在处理失败时仍发送“帧质量更新”（建议 true，便于 Self 做健康监控）
        bool 处理失败也发送质量 = true;
    };

public:
    相机外设线程类(外设接口<结构体_原始场景帧>& cam, 相机写入参数 写入参数 = {}, 参数 p = {})
        : cam_(cam), processor_(std::move(写入参数)), p_(std::move(p)) {
        // 同步 session 参数（取帧超时/空转）到 processor 参数，避免两处配置打架
        auto cp = processor_.参数();
        cp.等待帧超时毫秒 = p_.等待帧超时毫秒;
        cp.空转睡眠毫秒 = p_.空转睡眠毫秒;
        processor_.设置参数(cp);
    }

    ~相机外设线程类() { 停止(); }

    void 设置投递回调(回调_投递消息 cb) { 投递_ = std::move(cb); }

    void 启动() {
        bool expected = false;
        if (!running_.compare_exchange_strong(expected, true)) return;
        worker_ = std::thread([this] { 循环_(); });
    }

    void 停止() {
        running_.store(false);
        if (worker_.joinable()) worker_.join();
    }

    bool 运行中() const noexcept { return running_.load(); }

    相机帧处理器& 处理器() noexcept { return processor_; }
    const 参数& 线程参数() const noexcept { return p_; }
    void 设置线程参数(const 参数& p) { p_ = p; }

private:
    void 私有_投递(结构体_相机外设消息&& m) {
        if (!投递_) return;
        投递_(m);
    }

    static inline double 私有_估计FPS(时间戳 nowUs, 时间戳& io_lastUs, double& io_emaFps) noexcept {
        if (io_lastUs <= 0 || nowUs <= io_lastUs) {
            io_lastUs = nowUs;
            return io_emaFps;
        }
        const double dt = double(nowUs - io_lastUs) / 1'000'000.0;
        io_lastUs = nowUs;
        if (dt <= 1e-9) return io_emaFps;
        const double fps = 1.0 / dt;
        // EMA 平滑，避免抖动
        const double a = 0.1;
        if (io_emaFps <= 0.0) io_emaFps = fps;
        else io_emaFps = (1.0 - a) * io_emaFps + a * fps;
        return io_emaFps;
    }

    static inline 结构体_相机存在摘要 私有_摘要化(const 结构体_存在观测& obs) {
        结构体_相机存在摘要 s{};
        s.跟踪ID = obs.跟踪ID;
        s.中心坐标 = obs.中心坐标;
        s.尺寸 = obs.尺寸;
        s.AABB_min = obs.AABB_min;
        s.AABB_max = obs.AABB_max;

        s.轮廓签名长度 = (std::uint32_t)obs.轮廓编码.size();
        s.轮廓签名_U64 = obs.轮廓编码.empty() ? 0ull : 通用函数模块::FNV1a64_vec_u64(obs.轮廓编码);
        return s;
    }

    void 私有_发送设备状态_并停止(枚举_相机环境异常码 why) {
        // 1) 发环境异常（可选）
        结构体_相机环境异常 e{};
        e.头.设备ID = cam_.状态().消息.empty() ? "" : ""; // 设备ID 从状态里不一定有；保持空即可
        e.异常码 = why;
        e.诊断 = cam_.状态().消息;
        私有_投递({ 枚举_相机外设消息类型::环境异常, e });

        // 2) 发设备状态
        结构体_相机设备状态 st{};
        st.状态 = cam_.状态();
        私有_投递({ 枚举_相机外设消息类型::设备状态, st });
    }

    void 循环_() {
        外设数据包<结构体_原始场景帧> pkg{};
        帧处理结果 out{};

        int 连续取帧超时 = 0;
        int 连续处理失败 = 0;
        int 连续观测为空 = 0;

        std::uint64_t 帧计数 = 0;

        时间戳 lastUs = 0;
        double emaFps = 0.0;

        while (running_.load()) {
            const auto r = cam_.等待下一包(pkg, p_.等待帧超时毫秒);
            if (r != 枚举_取数结果::成功) {
                if (r == 枚举_取数结果::超时) {
                    ++连续取帧超时;
                    if (连续取帧超时 >= p_.连续取帧超时阈值) {
                        结构体_相机环境异常 e{};
                        e.头 = pkg.头; // pkg 可能是旧值；但不重要
                        e.异常码 = 枚举_相机环境异常码::取帧超时过多;
                        e.诊断 = "相机等待下一包超时次数过多";
                        私有_投递({ 枚举_相机外设消息类型::环境异常, e });
                        连续取帧超时 = 0; // 降噪：避免刷屏
                    }
                }
                if (r == 枚举_取数结果::已停止) {
                    私有_发送设备状态_并停止(枚举_相机环境异常码::外设已停止);
                    break;
                }
                if (r == 枚举_取数结果::故障) {
                    私有_发送设备状态_并停止(枚举_相机环境异常码::外设故障);
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(p_.空转睡眠毫秒));
                continue;
            }

            连续取帧超时 = 0;
            ++帧计数;

            // 处理计时
            const auto t0 = std::chrono::high_resolution_clock::now();

            // 避免每帧拷贝大数组（深度/颜色/点云）
            out.原始场景帧 = std::make_shared<结构体_原始场景帧>(std::move(pkg.数据));

            bool ok = false;
            try {
                ok = processor_.处理一帧(out);
            }
            catch (...) {
                ok = false;
            }

            const auto t1 = std::chrono::high_resolution_clock::now();
            const double costMs = std::chrono::duration<double, std::milli>(t1 - t0).count();

            // 帧质量更新（建议每帧都发）
            if (ok || p_.处理失败也发送质量) {
                结构体_相机帧质量指标 q{};
                q.设备ID = pkg.头.设备ID;
                q.帧序号 = pkg.头.序号;
                q.时间_设备 = pkg.头.时间_设备;
                q.时间_系统 = pkg.头.时间_系统;
                q.丢包计数 = pkg.头.丢包计数;
                q.处理耗时_ms = costMs;

                // FPS 估计：优先系统到达时间；若为0则用本地当前时间
                时间戳 nowUs = (pkg.头.时间_系统 > 0) ? pkg.头.时间_系统 : 结构体_时间戳::当前_微秒();
                q.估计FPS = 私有_估计FPS(nowUs, lastUs, emaFps);

                if (ok) {
                    q.观测数 = out.存在观测列表.size();
                    q.轮廓数 = out.轮廓观测列表.size();
                }
                私有_投递({ 枚举_相机外设消息类型::帧质量更新, q });
            }

            if (!ok) {
                ++连续处理失败;
                if (连续处理失败 >= p_.连续处理失败阈值) {
                    结构体_相机环境异常 e{};
                    e.头 = pkg.头;
                    e.异常码 = 枚举_相机环境异常码::连续处理失败;
                    e.诊断 = "相机帧处理器连续处理失败";
                    私有_投递({ 枚举_相机外设消息类型::环境异常, e });
                    连续处理失败 = 0;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(p_.空转睡眠毫秒));
                continue;
            }
            连续处理失败 = 0;

            // 场景快照：用于“静态观测稳定化”任务（即使静态也要发）
            if (p_.快照每N帧发送一次 <= 1 || (int)(帧计数 % (std::uint64_t)p_.快照每N帧发送一次) == 0) {
                结构体_相机场景观测快照 snap{};
                snap.头 = pkg.头;
                snap.存在列表.reserve(out.存在观测列表.size());
                for (const auto& obs : out.存在观测列表) {
                    snap.存在列表.push_back(私有_摘要化(obs));
                }
                私有_投递({ 枚举_相机外设消息类型::场景观测快照, std::move(snap) });
            }

            // “观测为空”异常：连续空视野在静态稳定化时很关键（可能是黑夜/强光/阈值过严）
            if (out.存在观测列表.empty()) {
                ++连续观测为空;
                if (连续观测为空 >= p_.连续观测为空阈值) {
                    结构体_相机环境异常 e{};
                    e.头 = pkg.头;
                    e.异常码 = 枚举_相机环境异常码::观测为空过多;
                    e.诊断 = "存在观测连续为空次数过多";
                    私有_投递({ 枚举_相机外设消息类型::环境异常, e });
                    连续观测为空 = 0;
                }
            }
            else {
                连续观测为空 = 0;
            }
        }
    }

private:
    外设接口<结构体_原始场景帧>& cam_;
    相机帧处理器 processor_;
    参数 p_{};

    回调_投递消息 投递_{};

    std::atomic<bool> running_{ false };
    std::thread worker_;
};
