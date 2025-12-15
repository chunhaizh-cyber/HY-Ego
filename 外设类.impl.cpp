module;
#include <utility>
#include <exception>
#include <thread>
#include <chrono>

module 外设模块;

// 相机
import D455相机模块;

// 世界树融合（可选）
import 宇宙环境模块;
import 三维场景管模块;
import 世界树模块;
import 存在提取模块;  

外设类::~外设类() {
    停止();
}

bool 外设类::启动(场景帧回调 onFrame, 外设采集参数 p) {
    bool expected = false;
    if (!运行标志.compare_exchange_strong(expected, true)) {
        return false; // already running
    }

    回调 = std::move(onFrame);
    参数 = p;

    工作线程 = std::thread([this] {
        try {
            采集循环_阻塞();
        }
        catch (...) {
            // 这里不要 throw 出线程边界；需要的话你可以接入日志模块
        }
        运行标志.store(false);
        });

    return true;
}

void 外设类::停止() {
    请求退出();
    if (工作线程.joinable() && std::this_thread::get_id() != 工作线程.get_id()) {
        工作线程.join();
    }
}

void 外设类::请求退出() {
    运行标志.store(false);
}

void 外设类::相机开始获取信息_阻塞(场景帧回调 onFrame, 外设采集参数 p) {
    bool expected = false;
    if (!运行标志.compare_exchange_strong(expected, true)) {
        return; // already running
    }

    回调 = std::move(onFrame);
    参数 = p;

    try {
        采集循环_阻塞();
    }
    catch (...) {
    }

    运行标志.store(false);
}

// ==============================
// 核心循环：采集一帧 -> 点簇分割 -> 观测提取 -> (可选)融合世界树 -> 回调输出
// ==============================
void 外设类::采集循环_阻塞() {
    D455_相机实现 相机;
    if (!相机.打开()) {
        return;
    }

    点簇分割类 分割器;
    存在提取类 提取器;

    // 可选：融合世界树（你之前约定 g_宇宙.世界树.自我所在场景 已初始化）
    std::unique_ptr<三维场景管理类> 场景管理器;
    if (参数.融合到世界树) {
        场景管理器 = std::make_unique<三维场景管理类>(
            g_宇宙.世界树,
            g_宇宙.世界树.自我所在场景
        );
    }

    while (运行标志.load()) {
        结构体_原始场景帧 帧{};
        if (!相机.采集一帧(帧)) {
            std::this_thread::sleep_for(参数.无帧退避);
            continue;
        }

        // 1) 点簇分割（增强）
        auto 点簇增强列表 = 分割器.分割点簇_增强(帧, 参数.分割);

        // 2) 观测提取（你更新后的签名：从增强结果直接提取）
        auto 观测列表 = 提取器.从点簇增强列表提取观测(帧, 点簇增强列表);

        // 3) 融合世界树（可选）
        if (场景管理器) {
            场景管理器->融合存在观测列表(观测列表);
        }

        // 4) 输出给显示线程/上层逻辑
        if (回调) {
            auto spFrame = std::make_shared<结构体_原始场景帧>(std::move(帧));
            auto spObs = std::make_shared<std::vector<存在观测>>(std::move(观测列表));
            回调(std::move(spFrame), std::move(spObs));
        }
    }
}
