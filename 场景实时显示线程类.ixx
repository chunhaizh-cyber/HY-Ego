export module 场景实时显示线程模块;

import 场景实时显示模块;
import 世界树环境模块;
import 相机接口模块;
import 场景模块;
import 主信息定义模块;
import 基础数据类型模块;

import <thread>;
import <mutex>;
import <atomic>;
import <memory>;
import <chrono>;

export class 场景实时显示线程类 {
public:
    // 注意：实时渲染的“开关/参数/依赖特征类型集合”来自 概念_显示参数。
    // 这里的 参数 仅作为 概念_显示参数 缺失时的兜底。
    explicit 场景实时显示线程类(const 场景显示参数& 参数 = {})
        : 参数_(参数)
    {
        运行中.store(false);
    }

    ~场景实时显示线程类() {
        停止();
    }

    void 启动() {
        if (运行中.exchange(true)) return;
        工作线程 = std::jthread([this] { 线程函数(); });
    }

    void 停止() {
        运行中.store(false);
        if (工作线程.joinable()) {
            // jthread 析构会自动 request_stop + join；这里显式 join 以更确定
            工作线程.join();
        }
    }

    void 更新帧(const 结构体_原始场景帧& 帧) {
        std::scoped_lock lk(帧锁);
        最新帧 = std::make_unique<结构体_原始场景帧>(帧);
    }

private:
    void 线程函数() {
        // 重要：Viz3d 的创建与 spinOnce 必须在同一线程里完成（否则很容易 abort/随机崩溃）
        场景实时显示类 显示器(参数_);

        while (运行中.load()) {
            // 1) 当前场景：从世界树拿“自我所在场景”
            场景节点类* 当前场景 = 世界树.自我所在场景;

            // 2) 取最新帧（可为空）
            std::unique_ptr<结构体_原始场景帧> 帧;
            {
                std::scoped_lock lk(帧锁);
                if (最新帧) {
                    帧 = std::make_unique<结构体_原始场景帧>(*最新帧);
                }
            }

            // 3) 更新显示
            显示器.更新显示(当前场景, 帧.get());

            // 轻微让出 CPU（渲染内部 spinOnce 也会做事件轮询）
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

private:
    场景显示参数 参数_{};

    std::jthread 工作线程;
    std::atomic<bool> 运行中{ false };

    std::mutex 帧锁;
    std::unique_ptr<结构体_原始场景帧> 最新帧;
};
