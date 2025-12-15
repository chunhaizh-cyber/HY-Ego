module;
#include <functional>
#include <memory>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>

export module 外设模块;

// 结构体_原始场景帧
import 相机接口模块;

// 点簇分割参数 / 点簇增强结果 / 点簇分割类
import 点簇分割模块;

// 存在观测 / 观测提取参数 / 存在提取类
import 存在提取模块;

// 仅用于 时间戳/一些基础结构（若你这里不需要，可删）
import 基础数据类型模块;

// ==============================
// 外设采集参数（把“可调项”集中到这里，避免散落在 impl）
// ==============================
export struct 外设采集参数 {
    点簇分割参数 分割{};
    观测提取参数 提取{};

    // 是否将观测融合进世界树（依赖 外设类.impl.cpp 里是否 import 宇宙环境模块/三维场景管模块）
    bool 融合到世界树 = true;

    // 采集失败/无帧时的退避，避免 while 空转
    std::chrono::milliseconds 无帧退避{ 1 };
};

// ==============================
// 外设类：对外只暴露“启动/停止 + 回调”
//  - 外设模块不再 include 海鱼Dlg.h，不再引用 主窗口指针
// ==============================
export class 外设类 {
public:
    using 场景帧回调 = std::function<void(
        std::shared_ptr<结构体_原始场景帧>,
        std::shared_ptr<std::vector<存在观测>>
        )>;

public:
    外设类() = default;
    ~外设类();

    外设类(const 外设类&) = delete;
    外设类& operator=(const 外设类&) = delete;

    // 非阻塞：内部创建线程
    bool 启动(场景帧回调 onFrame, 外设采集参数 p = {});
    void 停止();
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
