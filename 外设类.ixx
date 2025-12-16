module;
#include <functional>
#include <memory>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>

export module 外设模块;
import 基础数据类型模块;
// 结构体_原始场景帧 / 相机接口
import 相机接口模块;
// 点簇分割参数 / 点簇增强结果 / 点簇分割类
import 点簇分割模块;
// 存在观测 / 观测提取参数 / 存在提取类
import 存在提取模块;

// ==============================
// 外设采集参数：集中管理可调项
// ==============================
export struct 外设采集参数 {
    点簇分割参数 分割{};
    观测提取参数 提取{};

    // 是否将观测融合进世界树（由实现文件决定是否依赖世界树相关模块）
    bool 融合到世界树 = true;

    // 采集失败/无帧时的退避，避免 while 空转
    std::chrono::milliseconds 无帧退避{ 1 };
};

// ==============================
// 外设类：对外只暴露“启动/停止 + 回调”
// 注意：这里只是模块接口声明；实现可放在同一模块的另一个实现单元中。
// ==============================
export class 外设类 {
public:
    using 场景帧回调 = std::function<void(
        std::shared_ptr<结构体_原始场景帧>,
        std::shared_ptr<std::vector<结构体_存在观测>>
        )>;

public:
    外设类() = default;
    ~外设类();

    外设类(const 外设类&) = delete;
    外设类& operator=(const 外设类&) = delete;

    // 非阻塞：内部创建线程
    bool 启动(场景帧回调 onFrame, 外设采集参数 p = {});

    // 停止线程并回收资源（建议析构也调用）
    void 停止();

    // 请求退出（通常只置标志，不阻塞等待；具体语义由实现决定）
    void 请求退出();

    bool 正在运行() const noexcept { return 运行标志.load(); }

    // 兼容接口：阻塞式采集（不要在 UI 线程调用）
    void 相机开始获取信息_阻塞(场景帧回调 onFrame, 外设采集参数 p = {});

private:
    void 采集循环_阻塞();

private:
    std::atomic_bool 运行标志{ false };
    std::thread 工作线程;

    场景帧回调 回调;
    外设采集参数 参数;
};
