module;
#include <afx.h>

export module 场景实时显示线程模块;

// ============================================================================
// 场景实时显示线程类
//
// 新逻辑定位：
//  - 不持有 OpenGL 上下文；渲染由 UI/渲染层完成。
//  - 本线程只负责周期性刷新：从世界树提取“可视化快照”。
//  - 世界树访问必须通过 `世界树环境模块` 的全局实例 `世界树`。
//  - 不暴露特征值节点；读取只通过 特征节点主信息.当前快照(I64/VecU句柄)。
//
// 线程模型：
//  - 1 个工作线程，循环刷新并发布最新快照（atomic shared_ptr）。
//  - 参数/场景/缓存指针通过轻量锁保护，线程内读取快照后无嵌套锁。
// ============================================================================

// re-export：使用者只 import 本模块即可拿到 场景实时显示类 与配置/快照结构
export import 场景实时显示模块;

import 世界树环境模块;
import 主信息定义模块;
import <thread>;
import <atomic>;
import <chrono>;
import <memory>;
import <mutex>;
import <shared_mutex>;
import <functional>;

export class 场景实时显示线程类 {
public:
    using 显示类 = 场景实时显示类;
    using 结构_显示配置 = 场景实时显示类::结构_显示配置;
    using 结构_场景可视化快照 = 场景实时显示类::结构_场景可视化快照;

public:
    场景实时显示线程类() = default;
    explicit 场景实时显示线程类(const 结构_显示配置& cfg) : cfg_(cfg) {}

    ~场景实时显示线程类() { 停止(); }

    场景实时显示线程类(const 场景实时显示线程类&) = delete;
    场景实时显示线程类& operator=(const 场景实时显示线程类&) = delete;

public:
    // 周期刷新（默认 33ms ~ 30FPS）
    void 启动(std::chrono::milliseconds period = std::chrono::milliseconds(33)) {
        bool expected = false;
        if (!running_.compare_exchange_strong(expected, true)) return;
        period_ = period;

        worker_ = std::jthread([this](std::stop_token st) { this->线程循环_(st); });
    }

    void 停止() {
        bool expected = true;
        if (!running_.compare_exchange_strong(expected, false)) return;

        if (worker_.joinable()) {
            worker_.request_stop();
            worker_.join();
        }

        displayPtr_.store(nullptr, std::memory_order_release);
        latest_.store(nullptr, std::memory_order_release);
    }

    bool 是否正在运行() const noexcept { return running_.load(std::memory_order_acquire); }

    bool 是否已就绪() const noexcept { return displayPtr_.load(std::memory_order_acquire) != nullptr; }

public:
    // ===== 配置/场景/缓存（线程安全） =====

    结构_显示配置 获取显示配置() const {
        std::shared_lock lk(cfgMtx_);
        return cfg_;
    }

    void 设置显示配置(const 结构_显示配置& cfg) {
        {
            std::unique_lock lk(cfgMtx_);
            cfg_ = cfg;
        }
        // 若显示器已存在，立即应用（不加 cfgMtx_，避免嵌套）
        if (auto* d = displayPtr_.load(std::memory_order_acquire)) {
            d->设置配置(cfg);
        }
    }

    void 修改显示配置(const std::function<void(结构_显示配置&)>& fn) {
        结构_显示配置 snap;
        {
            std::unique_lock lk(cfgMtx_);
            fn(cfg_);
            snap = cfg_;
        }
        if (auto* d = displayPtr_.load(std::memory_order_acquire)) {
            d->设置配置(snap);
        }
    }

    // 显示哪个场景：为空则默认 世界树.获取自我所在场景()
    void 设置场景(场景节点类* s) {
        {
            std::unique_lock lk(objMtx_);
            scene_ = s;
        }
        if (auto* d = displayPtr_.load(std::memory_order_acquire)) {
            d->设置场景(s);
        }
    }

    场景节点类* 获取场景() const {
        std::shared_lock lk(objMtx_);
        return scene_;
    }

    // 可选：绑定轮廓颜色缓存
    void 绑定轮廓颜色缓存(轮廓颜色缓存线程类* cache) {
        {
            std::unique_lock lk(objMtx_);
            cache_ = cache;
        }
        if (auto* d = displayPtr_.load(std::memory_order_acquire)) {
            d->绑定轮廓颜色缓存(cache);
        }
    }

public:
    // 取最新快照（拷贝）；未就绪则返回空快照
    结构_场景可视化快照 取快照() const {
        auto p = latest_.load(std::memory_order_acquire);
        if (!p) return {};
        return *p;
    }

    // 立即刷新一次（同步）
    void 刷新一次() {
        auto* d = displayPtr_.load(std::memory_order_acquire);
        if (!d) return;
        d->刷新一次();
        auto snap = std::make_shared<结构_场景可视化快照>(d->取快照());
        latest_.store(std::move(snap), std::memory_order_release);
    }

private:
    void 线程循环_(std::stop_token st) {
        // 创建显示器并发布指针
        auto display = std::make_unique<显示类>();

        // 应用配置/场景/缓存
        {
            std::shared_lock lk(cfgMtx_);
            display->设置配置(cfg_);
        }
        {
            std::shared_lock lk(objMtx_);
            display->设置场景(scene_);
            display->绑定轮廓颜色缓存(cache_);
        }

        displayPtr_.store(display.get(), std::memory_order_release);

        while (!st.stop_requested() && running_.load(std::memory_order_acquire)) {
            // 若未指定场景，则每轮取当前自我所在场景（避免外部切场景需强同步）
            场景节点类* s = nullptr;
            {
                std::shared_lock lk(objMtx_);
                s = scene_;
            }
            if (!s) s = 世界树.获取自我所在场景();
            display->设置场景(s);

            display->刷新一次();
            auto snap = std::make_shared<结构_场景可视化快照>(display->取快照());
            latest_.store(std::move(snap), std::memory_order_release);

            std::this_thread::sleep_for(period_);
        }

        displayPtr_.store(nullptr, std::memory_order_release);
        running_.store(false, std::memory_order_release);
    }

private:
    // 配置
    mutable std::shared_mutex cfgMtx_{};
    结构_显示配置 cfg_{};

    // 场景/缓存
    mutable std::shared_mutex objMtx_{};
    场景节点类* scene_ = nullptr;
    轮廓颜色缓存线程类* cache_ = nullptr;

    // 线程
    std::jthread worker_{};
    std::atomic_bool running_{ false };
    std::chrono::milliseconds period_{ 33 };

    // 输出
    std::atomic<显示类*> displayPtr_{ nullptr };
    std::atomic<std::shared_ptr<const 结构_场景可视化快照>> latest_{ nullptr };
};
