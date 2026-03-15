export module D455相机外设模块;

import 相机外设基类模块;
import D455相机模块;       // D455_相机实现
import 相机接口模块;       // 抽象深度相机接口
import 外设模块;
import 基础数据类型模块;

import <memory>;
import <string>;

export class D455相机外设类 final : public 相机外设基类 {
public:
    explicit D455相机外设类(D455_相机实现::配置项 cfg = {})
        : cfg_(std::move(cfg)) {
    }

protected:
    // 设备时间戳：优先用 D455 的设备时间域（微秒）。
    时间戳 取设备时间戳(const 结构体_原始场景帧& d) const override {
        if (d.相机.设备时间_微秒 > 0) return d.相机.设备时间_微秒;
        if (d.时间戳.设备时间_us > 0) return static_cast<时间戳>(d.时间戳.设备时间_us);
        return static_cast<时间戳>(d.时间戳.系统到达时间_us);
    }

    bool 启动设备(const 外设启动参数& p) override {
        drv_ = std::make_unique<D455_相机实现>(cfg_);
        // 用新接口打开（能吃 p.扩展 KV）
        if (!drv_->打开(p)) {
            drv_.reset();
            return false;
        }
        return true;
    }

    void 停止设备() override {
        if (drv_) {
            drv_->关闭();
            drv_.reset();
        }
    }

    外设指令回执 处理指令_设备(const 外设指令& cmd) override {
        if (!drv_) return { false, -1, "D455 未启动" };
        // 直接把外设指令转发给驱动（你 D455 里已经实现了 设置参数/自检 等）
        return drv_->处理外设指令(cmd);
    }

    bool 抓取数据_阻塞(结构体_原始场景帧& out) override {
        if (!drv_) return false;
        return drv_->采集一帧(out);
    }

private:
    D455_相机实现::配置项 cfg_{};
    std::unique_ptr<D455_相机实现> drv_;
};
