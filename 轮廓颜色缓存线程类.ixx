module;
#include <afx.h>

export module 轮廓颜色缓存线程模块;

import 基础数据类型模块;

import <cstdint>;
import <vector>;
import <string>;
import <string_view>;
import <unordered_map>;
import <list>;
import <deque>;
import <mutex>;
import <shared_mutex>;
import <condition_variable>;
import <thread>;
import <atomic>;
import <chrono>;
import <optional>;

// ============================================================================
// 轮廓颜色缓存线程类（Contour-Color Cache Thread）
//
// 目标：
//  - 把“轮廓/存在的颜色贴图（或缩略图）”异步缓存起来，供 UI/渲染线程读取。
//  - 只做缓存（增/改/查/清），不做世界树写入，不做特征值暴露。
//
// 约定：
//  - 输入图像默认为 BGR8（OpenCV 对齐），data.size()==w*h*3。
//  - key 由上层决定：
//      * 可用 存在主键 hash
//      * 或 轮廓VecIU64签名 hash
//      * 或 世界树证据池 VecU句柄.id
//  - 本类不依赖 OpenCV/GL；只存原始字节。
// ============================================================================

export class 轮廓颜色缓存线程类 {
public:
    using U64 = std::uint64_t;
    using U32 = std::uint32_t;

    struct 图像BGR8 {
        U32 宽 = 0;
        U32 高 = 0;
        std::vector<std::uint8_t> 数据; // BGRBGR...

        bool 有效() const noexcept {
            const std::size_t need = (std::size_t)宽 * (std::size_t)高 * 3;
            return 宽 > 0 && 高 > 0 && 数据.size() == need;
        }
    };

    struct 缓存条目 {
        U64 key = 0;
        时间戳 写入时间 = 0;
        时间戳 最后命中 = 0;
        U64 命中次数 = 0;
        图像BGR8 图{};
    };

    struct 参数 {
        // 最大缓存条目数（LRU 淘汰）
        std::size_t 最大条目数 = 4096;

        // 过期时间（微秒）；0 表示不过期
        时间戳 过期微秒 = 0;

        // 是否在写入前做简单下采样（仅做 2^k 降采样，最省 CPU）
        bool 启用下采样 = true;

        // 下采样目标最大边（像素），<=0 表示不限制
        int 最大边 = 256;
    };

    轮廓颜色缓存线程类() = default;
    ~轮廓颜色缓存线程类() { 停止(); }

    轮廓颜色缓存线程类(const 轮廓颜色缓存线程类&) = delete;
    轮廓颜色缓存线程类& operator=(const 轮廓颜色缓存线程类&) = delete;

    // ============================================================
    // 生命周期
    // ============================================================
    void 启动(参数 p = {}) {
        停止();
        {
            std::unique_lock lk(mtxParam_);
            param_ = p;
        }
        stop_.store(false);
        worker_ = std::thread([this] { this->线程体_(); });
    }

    void 停止() {
        stop_.store(true);
        cv_.notify_all();
        if (worker_.joinable()) worker_.join();
        // 不自动清缓存；由上层决定是否清空
    }

    bool 运行中() const noexcept { return worker_.joinable() && !stop_.load(); }

    void 设置参数(参数 p) {
        std::unique_lock lk(mtxParam_);
        param_ = p;
    }

    参数 获取参数() const {
        std::shared_lock lk(mtxParam_);
        return param_;
    }

    // ============================================================
    // 写入（异步）
    // ============================================================
    void 提交(U64 key, 图像BGR8 img, 时间戳 ts = 0) {
        if (key == 0) return;
        if (!img.有效()) return;
        if (ts == 0) ts = 当前微秒_();

        任务 t;
        t.key = key;
        t.ts = ts;
        t.img = std::move(img);

        {
            std::unique_lock lk(mtxQ_);
            q_.push_back(std::move(t));
        }
        cv_.notify_one();
    }

    // ============================================================
    // 读（同步）
    // ============================================================
    bool 取(U64 key, 缓存条目& out) {
        if (key == 0) return false;
        std::unique_lock lk(mtxCache_);
        auto it = map_.find(key);
        if (it == map_.end()) return false;
        if (私有_已过期_已加锁(it->second)) {
            私有_移除_已加锁(it->second);
            map_.erase(key);
            return false;
        }

        // LRU：移到 front
        lru_.splice(lru_.begin(), lru_, it->second);
        it->second->命中次数++;
        it->second->最后命中 = 当前微秒_();
        out = *(it->second);
        return true;
    }

    bool 有(U64 key) {
        if (key == 0) return false;
        std::unique_lock lk(mtxCache_);
        auto it = map_.find(key);
        if (it == map_.end()) return false;
        if (私有_已过期_已加锁(it->second)) {
            私有_移除_已加锁(it->second);
            map_.erase(key);
            return false;
        }
        return true;
    }

    void 清空() {
        std::unique_lock lk(mtxCache_);
        lru_.clear();
        map_.clear();
    }

    std::size_t 条目数() const {
        std::unique_lock lk(mtxCache_);
        return map_.size();
    }

    // ============================================================
    // Key 工具：为 VecIU64 轮廓签名生成稳定 key（FNV-1a 64）
    // ============================================================
    static U64 生成Key_轮廓签名FNV(const VecIU64& sig) {
        // FNV-1a 64
        constexpr U64 offset = 14695981039346656037ULL;
        constexpr U64 prime = 1099511628211ULL;
        U64 h = offset;
        for (U64 v : sig) {
            for (int i = 0; i < 8; ++i) {
                std::uint8_t b = (std::uint8_t)((v >> (i * 8)) & 0xFF);
                h ^= (U64)b;
                h *= prime;
            }
        }
        // 防止 0 key
        return h == 0 ? 1 : h;
    }

private:
    struct 任务 {
        U64 key = 0;
        时间戳 ts = 0;
        图像BGR8 img{};
    };

    // 线程体：消费任务队列 -> 写入 LRU 缓存
    void 线程体_() {
        while (!stop_.load()) {
            任务 t;
            {
                std::unique_lock lk(mtxQ_);
                cv_.wait(lk, [&] { return stop_.load() || !q_.empty(); });
                if (stop_.load()) break;
                t = std::move(q_.front());
                q_.pop_front();
            }

            参数 p = 获取参数();
            if (p.启用下采样 && p.最大边 > 0) {
                t.img = 私有_快速降采样2幂_(std::move(t.img), p.最大边);
                if (!t.img.有效()) continue;
            }

            // 写入缓存（LRU）
            {
                std::unique_lock lk(mtxCache_);

                // 覆盖更新
                auto it = map_.find(t.key);
                if (it != map_.end()) {
                    // 更新内容
                    it->second->图 = std::move(t.img);
                    it->second->写入时间 = t.ts;
                    // 不清零统计
                    lru_.splice(lru_.begin(), lru_, it->second);
                }
                else {
                    缓存条目 e;
                    e.key = t.key;
                    e.写入时间 = t.ts;
                    e.最后命中 = 0;
                    e.命中次数 = 0;
                    e.图 = std::move(t.img);

                    lru_.push_front(std::move(e));
                    map_[t.key] = lru_.begin();
                }

                私有_淘汰_已加锁(p);
            }
        }
    }

    // 简易 2^k 降采样：每次 /2 取左上像素（极省 CPU，够做缩略图）
    static 图像BGR8 私有_快速降采样2幂_(图像BGR8 img, int maxSide) {
        if (!img.有效()) return {};
        if (maxSide <= 0) return img;

        while ((int)img.宽 > maxSide || (int)img.高 > maxSide) {
            const U32 nw = img.宽 / 2;
            const U32 nh = img.高 / 2;
            if (nw == 0 || nh == 0) break;

            std::vector<std::uint8_t> out;
            out.resize((std::size_t)nw * (std::size_t)nh * 3);

            for (U32 y = 0; y < nh; ++y) {
                for (U32 x = 0; x < nw; ++x) {
                    const std::size_t src = ((std::size_t)(y * 2) * img.宽 + (x * 2)) * 3;
                    const std::size_t dst = ((std::size_t)y * nw + x) * 3;
                    out[dst + 0] = img.数据[src + 0];
                    out[dst + 1] = img.数据[src + 1];
                    out[dst + 2] = img.数据[src + 2];
                }
            }

            img.宽 = nw;
            img.高 = nh;
            img.数据 = std::move(out);
        }
        return img;
    }

    // 淘汰策略：过期 + LRU 超容量
    void 私有_淘汰_已加锁(const 参数& p) {
        // 先按过期时间淘汰（从 LRU 尾部开始快速删除）
        if (p.过期微秒 > 0) {
            while (!lru_.empty()) {
                auto it = std::prev(lru_.end());
                if (!私有_已过期_已加锁(it)) break;
                map_.erase(it->key);
                lru_.pop_back();
            }
        }

        // 再按容量淘汰
        while (p.最大条目数 > 0 && map_.size() > p.最大条目数) {
            auto it = std::prev(lru_.end());
            map_.erase(it->key);
            lru_.pop_back();
        }
    }

    bool 私有_已过期_已加锁(const std::list<缓存条目>::iterator& it) const {
        参数 p = 获取参数();
        if (p.过期微秒 == 0) return false;
        const 时间戳 now = 当前微秒_();
        const 时间戳 age = (now >= it->写入时间) ? (now - it->写入时间) : 0;
        return age > p.过期微秒;
    }

    void 私有_移除_已加锁(const std::list<缓存条目>::iterator& it) {
        lru_.erase(it);
    }

    static 时间戳 当前微秒_() {
        using namespace std::chrono;
        return (时间戳)duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
    }

private:
    // 参数
    mutable std::shared_mutex mtxParam_{};
    参数 param_{};

    // 任务队列
    std::mutex mtxQ_{};
    std::condition_variable cv_{};
    std::deque<任务> q_{};

    // 缓存（LRU）
    mutable std::mutex mtxCache_{};
    std::list<缓存条目> lru_{};
    std::unordered_map<U64, std::list<缓存条目>::iterator> map_{};

    // 线程
    std::atomic_bool stop_{ false };
    std::thread worker_{};
};
