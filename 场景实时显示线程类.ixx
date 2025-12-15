module;
#include <chrono>
#include <mutex>
#include <memory>

export module 场景实时显示线程模块;

import <vector>;
import <string>;

import 线程模板模块;
import 场景实时显示模块;
import 相机接口模块;        // 结构体_原始场景帧
import 世界树模块;            // 场景节点类
import 基础数据类型模块;      // 时间戳等
import 主信息定义模块;    // 结构体_原始场景帧 
import 宇宙环境模块;    // 时间戳等   

export class 场景显示线程类 : public 线程模板 {
public:
    场景显示线程类()
        : 显示器(场景显示参数{
            .显示实时点云 = true,
            .点云采样步长 = 4,
            .显示存在占位盒 = true,
            .显示存在标签 = true,
            .记忆淡化时间秒 = 15.0f
            })
    {
        设置Tick间隔(std::chrono::milliseconds(16)); // ~60fps
    }

    // 新提交接口：实时帧可选（用于点云背景），场景根必传（用于记忆存在）
    void 提交(std::shared_ptr<结构体_原始场景帧> 实时帧, 场景节点类* 当前场景根)
    {
        std::lock_guard<std::mutex> lk(data_mtx);
        latest实时帧 = std::move(实时帧);
        if (当前场景根) latest场景根 = 当前场景根;  // 只有非空才更新
        有新数据 = true;
        发送消息("NEW");
    }

    // 可选：强制清空实时点云（例如相机断开时）
    void 清空实时点云()
    {
        {
            std::lock_guard<std::mutex> lk(data_mtx);
            latest实时帧.reset();
            有新数据 = true;
        }
        发送消息("NEW");
    }

protected:
    void 线程开始() override {
        显示器.初始化();
    }

    void 处理消息(const 消息& m) override {
        if (m.内容 == "CLEAR") {
            std::lock_guard<std::mutex> lk(data_mtx);
            latest实时帧.reset();
            latest场景根 = nullptr;
            有新数据 = true;
            发送消息("NEW");
        }
    }

    void 线程空闲Tick() override {
        std::shared_ptr<结构体_原始场景帧> 使用帧 = nullptr;

        // ✅ 每帧都从世界树取“自我所在场景”
        场景节点类* 使用场景根 = (场景节点类*)g_宇宙.世界树.自我所在场景;

        {
            std::lock_guard<std::mutex> lk(data_mtx);

            if (有新数据) {
                使用帧 = latest实时帧;
                // ❌ 不再用 latest场景根 覆盖
                有新数据 = false;
            }
            else {
                使用帧 = 当前实时帧;
            }

            当前实时帧 = 使用帧;
            当前场景根 = 使用场景根;  // 缓存也保持为“自我所在场景”
        }

        if (!显示器.更新(使用帧.get(), 使用场景根)) {
            请求退出();
        }
    }



private:
    场景实时显示器 显示器;

    std::mutex data_mtx;
    bool 有新数据 = false;

    // 最新提交的数据
    std::shared_ptr<结构体_原始场景帧> latest实时帧;
    场景节点类* latest场景根 = nullptr;

    // 当前渲染使用的数据（避免频繁加锁）
    std::shared_ptr<结构体_原始场景帧> 当前实时帧;
    场景节点类* 当前场景根 = nullptr;
};