module;
#include <afx.h>

export module 场景实时显示模块;

import 基础数据类型模块;
import 主信息定义模块;
import 世界树环境模块;
import 轮廓颜色缓存线程模块;

import <cstdint>;
import <string>;
import <string_view>;
import <vector>;
import <optional>;
import <variant>;
import <mutex>;
import <shared_mutex>;
import <thread>;
import <atomic>;
import <chrono>;

// ============================================================================
// 场景实时显示类（Scene Realtime Display）
//
// 新逻辑定位：
//  - 只负责“从世界树提取可视化快照 + 可选后台刷新线程”。
//  - 不访问特征值节点；值读取只通过：特征节点主信息.当前快照（I64 或 VecU句柄）。
//  - 不依赖 OpenGL/OpenCV：渲染由外部 UI/渲染模块完成。
//
// 额外能力：
//  - 可选对接“轮廓颜色缓存线程”：在快照中附带缩略图元信息（可选拷贝像素）。
//    默认关闭，避免 UI/缓存锁争用与拷贝开销。
// ============================================================================
export struct 场景显示参数 {
    bool 显示坐标轴 = true;
    bool 显示网格地面 = true;
    bool 显示实时点云 = false;
    bool 显示存在框 = true;          // 占位：线框 OBB
    bool 显示存在主轴 = true;
    bool 显示存在点云簇 = true;       // 使用 结构体_轮廓观测::点云簇

    // 体素网格（异步）
    bool 显示体素网格 = true;
    bool 网格仅表面 = true;
    int  网格体素采样步长 = 1;
    int  网格提交间隔帧 = 5;
    int  每帧最大网格提交 = 2;
    std::uint32_t 网格默认色 = 0xFF90FF90u; // 0xAARRGGBB

    // 0=相对位置, 1=绝对位置
    int 坐标模式 = 0;

    // 相机坐标映射：RealSense 相机坐标 (x右,y下,z前) -> OpenGL (x右,y上,z前)
    bool 使用相机坐标系映射 = true;

    // 点云采样步长（>=1）
    int 点云采样步长 = 4;

    // 存在可视超时：超过该时间未观测则不绘制（避免“方框越画越多”）
    int 存在可视超时_ms = 800;


    // 可视过滤策略（推荐：按“连续未命中帧”而不是按时间，避免低帧率/高延迟误杀）
    bool 启用按帧失联过滤 = false;
    int  存在可视失联帧 = 120;         // 连续未命中超过该帧数则不绘制（0=禁用）
    float 可视最小可信度 = 0.02f;     // 可信度低于该阈值且未命中时可隐藏（<=0=禁用）

    // 兼容：仍可按时间过滤（默认关闭；仅在你的时间域统一后再打开）
    bool 启用存在可视超时过滤 = false;
    // 视锥裁剪（单位：mm）
    float 近裁剪_mm = 10.0f;
    float 远裁剪_mm = 8000.0f;

    // OBB 主轴缩放
    float 主轴长度缩放 = 0.6f;

    // 采样颜色：
    // - 实时点云：若帧有颜色则采样颜色，否则白色
    // - 点云簇：使用观测平均颜色
    bool 点云使用颜色 = true;
    bool 显示体素立方体 = true;
    bool 显示彩色截图 = true;
    bool 显示轮廓编码 = false; // 已取消显示
    bool 显示边界序列点 = false;
    bool 自动选择显示模式 = true;

    // 存在彩色轮廓（优先：来自 结构体_存在观测 的 裁剪BGR+裁剪掩膜；兜底：存在特征的轮廓编码 + 默认色）
    bool 显示存在彩色轮廓 = true;
};
export class 场景实时显示类 {
public:
    using I64 = std::int64_t;

    // =========================
    // 显示配置：把“特征类型”映射到几何/贴图语义
    // =========================
    struct 结构_显示配置 {
        // 位置（mm）：建议拆为三个 I64 特征
        const 词性节点类* 类型_中心X = nullptr;
        const 词性节点类* 类型_中心Y = nullptr;
        const 词性节点类* 类型_中心Z = nullptr;

        // 尺寸（mm）：建议拆为三个 I64 特征
        const 词性节点类* 类型_尺寸X = nullptr;
        const 词性节点类* 类型_尺寸Y = nullptr;
        const 词性节点类* 类型_尺寸Z = nullptr;

        // 轮廓/颜色等“证据块”（VecIU64 -> 证据池句柄）
        const 词性节点类* 类型_轮廓编码 = nullptr;

        // 若轮廓颜色缓存的 key 由 VecU句柄.主信息指针 决定，可开启
        bool 使用轮廓句柄作为颜色key = true;

        // 只显示最近观测过的存在（可用存在主信息里的 连续未命中帧 过滤）
        bool 启用过期过滤 = false;
        std::uint32_t 最大连续未命中帧 = 60;

        // ===== 可选：从轮廓颜色缓存取元信息/缩略图 =====
        bool 启用颜色缓存读取 = false;              // 是否尝试读取颜色缓存
        bool 拷贝颜色像素数据 = false;              // true: 拷贝 BGR8 像素到快照（开销大）
        std::size_t 每帧最多读取颜色条目 = 16;      // 限流：最多读取多少个存在的颜色缓存（避免卡顿）
    };

    struct 结构_颜色缓存摘要 {
        std::uint64_t key = 0;
        时间戳 写入时间 = 0;
        std::uint32_t 宽 = 0;
        std::uint32_t 高 = 0;
        bool 命中 = false;

        // 可选：只有 cfg.拷贝颜色像素数据=true 时填充
        std::optional<轮廓颜色缓存线程类::图像BGR8> 图像{};
    };

    struct 结构_存在可视化 {
        std::string 存在主键;

        // 空表示缺失
        std::optional<I64> 中心X_mm;
        std::optional<I64> 中心Y_mm;
        std::optional<I64> 中心Z_mm;

        std::optional<I64> 尺寸X_mm;
        std::optional<I64> 尺寸Y_mm;
        std::optional<I64> 尺寸Z_mm;

        std::optional<VecU句柄> 轮廓句柄;

        // 可选：给渲染层去轮廓颜色缓存取贴图
        std::optional<std::uint64_t> 颜色缓存key;
        std::optional<结构_颜色缓存摘要> 颜色缓存{};

        // 可选：调试/叠加信息
        std::uint32_t 连续命中帧 = 0;
        std::uint32_t 连续未命中帧 = 0;
        std::uint32_t 连续静止帧 = 0;
    };

    struct 结构_场景可视化快照 {
        时间戳 时间 = 0;
        std::string 场景主键;
        std::uint64_t 帧序号 = 0;
        std::vector<结构_存在可视化> 存在列表{};
    };

public:
    场景实时显示类() = default;
    ~场景实时显示类() { 停止(); }

    void 设置配置(const 结构_显示配置& cfg) {
        std::unique_lock lk(mtx_);
        cfg_ = cfg;
    }

    结构_显示配置 获取配置() const {
        std::shared_lock lk(mtx_);
        return cfg_;
    }

    void 设置场景(场景节点类* s) {
        std::unique_lock lk(mtx_);
        场景_ = s;
    }

    场景节点类* 获取场景() const {
        std::shared_lock lk(mtx_);
        return 场景_;
    }

    void 绑定轮廓颜色缓存(轮廓颜色缓存线程类* cache) {
        std::unique_lock lk(mtx_);
        cache_ = cache;
    }

    // =========================
    // 后台刷新线程（可选）
    // =========================
    void 启动(std::chrono::milliseconds period = std::chrono::milliseconds(33)) {
        bool expected = false;
        if (!running_.compare_exchange_strong(expected, true)) return;
        period_ = period;
        th_ = std::thread([this]() { this->私有_线程循环(); });
    }

    void 停止() {
        bool expected = true;
        if (!running_.compare_exchange_strong(expected, false)) return;
        if (th_.joinable()) th_.join();
    }

    bool 运行中() const noexcept { return running_.load(); }

    // =========================
    // 主动刷新一次：从世界树提取快照
    // =========================
    void 刷新一次() {
        场景节点类* s = nullptr;
        结构_显示配置 cfg;
        轮廓颜色缓存线程类* cache = nullptr;
        {
            std::shared_lock lk(mtx_);
            s = 场景_;
            cfg = cfg_;
            cache = cache_;
        }
        if (!s) return;

        结构_场景可视化快照 snap{};
        snap.时间 = 结构体_时间戳::当前_微秒();
        snap.帧序号 = ++frameNo_;
        snap.场景主键 = s->获取主键();

        // 枚举场景下所有存在
        auto exists = 世界树.获取子存在(s, "场景实时显示类::刷新一次");
        snap.存在列表.reserve(exists.size());

        std::size_t 颜色读取计数 = 0;

        for (auto* e : exists) {
            if (!e) continue;
            auto* emi = dynamic_cast<存在节点主信息类*>(e->主信息);
            if (cfg.启用过期过滤 && emi) {
                if (emi->连续未命中帧 > cfg.最大连续未命中帧) continue;
            }

            结构_存在可视化 v{};
            v.存在主键 = e->获取主键();
            if (emi) {
                v.连续命中帧 = emi->连续命中帧;
                v.连续未命中帧 = emi->连续未命中帧;
                v.连续静止帧 = emi->连续静止帧;
            }

            // 位置
            v.中心X_mm = 私有_取I64快照_按类型(e, cfg.类型_中心X);
            v.中心Y_mm = 私有_取I64快照_按类型(e, cfg.类型_中心Y);
            v.中心Z_mm = 私有_取I64快照_按类型(e, cfg.类型_中心Z);

            // 尺寸
            v.尺寸X_mm = 私有_取I64快照_按类型(e, cfg.类型_尺寸X);
            v.尺寸Y_mm = 私有_取I64快照_按类型(e, cfg.类型_尺寸Y);
            v.尺寸Z_mm = 私有_取I64快照_按类型(e, cfg.类型_尺寸Z);

            // 轮廓（VecU句柄）
            if (cfg.类型_轮廓编码) {
                if (auto h = 私有_取VecU句柄快照_按类型(e, cfg.类型_轮廓编码)) {
                    v.轮廓句柄 = h;
                    if (cfg.使用轮廓句柄作为颜色key) {
                        v.颜色缓存key = static_cast<std::uint64_t>(h->主信息指针);
                    }
                }
            }

            // 可选：读取颜色缓存
            if (cfg.启用颜色缓存读取 && cache && v.颜色缓存key.has_value()) {
                if (颜色读取计数 < cfg.每帧最多读取颜色条目) {
                    轮廓颜色缓存线程类::缓存条目 ce{};
                    const std::uint64_t key = *v.颜色缓存key;
                    if (cache->取(key, ce)) {
                        结构_颜色缓存摘要 info{};
                        info.key = key;
                        info.命中 = true;
                        info.写入时间 = ce.写入时间;
                        info.宽 = ce.图.宽;
                        info.高 = ce.图.高;
                        if (cfg.拷贝颜色像素数据) {
                            info.图像 = std::move(ce.图);
                        }
                        v.颜色缓存 = std::move(info);
                        ++颜色读取计数;
                    }
                    else {
                        结构_颜色缓存摘要 info{};
                        info.key = key;
                        info.命中 = false;
                        v.颜色缓存 = std::move(info);
                        ++颜色读取计数;
                    }
                }
            }

            snap.存在列表.push_back(std::move(v));
        }

        {
            std::unique_lock lk(mtx_);
            last_ = std::move(snap);
        }
    }

    // 取最新快照（拷贝）
    结构_场景可视化快照 取快照() const {
        std::shared_lock lk(mtx_);
        return last_;
    }
   
private:
    // 从 host(存在/场景) 的子特征中按类型读取 I64 当前快照
    static std::optional<I64> 私有_取I64快照_按类型(基础信息节点类* host, const 词性节点类* type) {
        if (!host || !type) return std::nullopt;
        auto* f = 世界树.查找子特征_按类型(host, type, "场景实时显示类::取I64");
        if (!f) return std::nullopt;
        auto* fmi = dynamic_cast<特征节点主信息类*>(f->主信息);
        if (!fmi) return std::nullopt;
        if (auto* pv = std::get_if<I64>(&fmi->当前快照)) return *pv;
        return std::nullopt;
    }

    static std::optional<VecU句柄> 私有_取VecU句柄快照_按类型(基础信息节点类* host, const 词性节点类* type) {
        if (!host || !type) return std::nullopt;
        auto* f = 世界树.查找子特征_按类型(host, type, "场景实时显示类::取VecU句柄");
        if (!f) return std::nullopt;
        auto* fmi = dynamic_cast<特征节点主信息类*>(f->主信息);
        if (!fmi) return std::nullopt;
        if (auto* ph = std::get_if<VecU句柄>(&fmi->当前快照)) return *ph;
        return std::nullopt;
    }

    void 私有_线程循环() {
        while (running_.load()) {
            刷新一次();
            std::this_thread::sleep_for(period_);
        }
    }

private:
    mutable std::shared_mutex mtx_{};

    结构_显示配置 cfg_{};
    场景节点类* 场景_ = nullptr;
    轮廓颜色缓存线程类* cache_ = nullptr;

    结构_场景可视化快照 last_{};

    std::atomic_bool running_{ false };
    std::chrono::milliseconds period_{ 33 };
    std::thread th_{};

    std::atomic<std::uint64_t> frameNo_{ 0 };
};

