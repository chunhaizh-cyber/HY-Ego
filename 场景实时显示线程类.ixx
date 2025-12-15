module;
#include <chrono>
#include <mutex>

export module 场景实时显示线程模块;

import <vector>;
import <memory>;

import 线程模板模块;
import 场景实时显示模块;
import 相机接口模块;   // 结构体_原始场景帧
import 存在提取模块;   // 存在观测
import 基础数据类型模块; // 时间戳 
export class 场景显示线程类 : public 线程模板 {
public:
    场景显示线程类()
        : 显示器(场景显示参数{
            .显示点云 = true,
            .点云采样步长 = 4,
            .显示存在占位盒 = true,
            .显示存在轮廓点云 = false
            })
    {
        设置Tick间隔(std::chrono::milliseconds(16)); // ~60fps
    }

    void 提交(std::shared_ptr<结构体_原始场景帧> f,
        std::shared_ptr<std::vector<存在观测>> o)
    {
        {
            std::lock_guard<std::mutex> lk(data_mtx);
            latest帧 = std::move(f);
            latest观测 = std::move(o);
            有新数据 = true;
        }
        发送消息("NEW"); // 唤醒（即使没到 tick）
    }

protected:
    void 线程开始() override {
        显示器.初始化();
    }

    void 处理消息(const 消息& m) override {
        if (m.内容 == "CLEAR") {
            std::lock_guard<std::mutex> lk(data_mtx);
            latest帧.reset();
            latest观测.reset();
            有新数据 = true;
        }
        // "NEW" 不需要特别处理，tick 会自己刷新
    }

    void 线程空闲Tick() override {
        std::shared_ptr<结构体_原始场景帧> f;
        std::shared_ptr<std::vector<存在观测>> o;

        {
            std::lock_guard<std::mutex> lk(data_mtx);
            if (有新数据) {
                if (latest帧)   当前帧 = latest帧;
                if (latest观测) 当前观测 = latest观测;
                有新数据 = false;
            }
            f = 当前帧;
            o = 当前观测;
        }

        // 没数据也要刷新窗口（spinOnce 在 场景实时显示器::更新 内部）:contentReference[oaicite:3]{index=3}
        static 结构体_原始场景帧 空帧{};
        static std::vector<存在观测> 空观测{};

        const auto& useF = f ? *f : 空帧;
        const auto& useO = o ? *o : 空观测;

        if (!显示器.更新(useF, useO)) {
            // 用户关闭 viz 窗口
            请求退出();
        }
    }

private:
    场景实时显示器 显示器;

    std::mutex data_mtx;
    bool 有新数据 = false;

    std::shared_ptr<结构体_原始场景帧> latest帧;
    std::shared_ptr<std::vector<存在观测>> latest观测;

    std::shared_ptr<结构体_原始场景帧> 当前帧;
    std::shared_ptr<std::vector<存在观测>> 当前观测;
};
