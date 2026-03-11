export module 三维雕刻写入线程模块;

import <cstdint>;
import <vector>;
import <deque>;
import <unordered_map>;
import <mutex>;
import <condition_variable>;
import <thread>;
import <atomic>;
import <shared_mutex>;
import <algorithm>;

import 通用函数模块;

import 日志模块;

import 世界树环境模块;
import 特征值环境模块;
import 特征类型定义模块;
import 主信息定义模块;
import 基础数据类型模块;

// 异步写入：收集多帧雕刻结果，稳定后再写入存在的特征信息。
// 稳定判定：相邻两帧 blocks 的 Jaccard 相似度（intersection/union）足够高并连续多帧。

export class 三维雕刻写入线程类
{
public:
    struct 参数
    {
        std::size_t 最大排队 = 64;

        // 只有达到稳定后才会写入
        std::int32_t 需要稳定帧数 = 1;
        double 相邻帧相似阈值 = 0.885;

        // 已提交的模型与候选几乎相同则跳过（避免重复写入）
        double 已提交相似跳过阈值 = 0.995;

        // 两次写入之间至少隔多少帧（同一存在）
        std::int32_t 写入最小间隔帧 = 8;

        // 过小的模型直接不写（按占据bit数粗过滤）
        std::int32_t 最小占据bit数 = 128;

        // 分辨率策略：允许候选模型以“更高分辨率”覆盖旧模型
        bool 允许分辨率提升 = true;

        // 颜色特征类型（可自定义）；默认=类型_三维模型颜色
        const 词性节点类* 颜色特征类型 = nullptr;

        // blocks 特征类型（可自定义）；默认=类型_三维模型_体素立方体
        const 词性节点类* Blocks特征类型 = nullptr;

      
    };
    三维雕刻写入线程类() : p_{} {}



    struct 任务
    {
        存在节点类* e = nullptr;

        // 体素块：每块 64bit（编码方案由三维模型类决定）
        VecIU64 blocks;

        // 颜色：每体素一个 RGBA32（0xAARRGGBB），与 blocks 对应（允许为空）
        std::vector<std::uint32_t> colorsRGBA;

        int NB = 0;             // 每块包含体素数（通常 64）
        int 体素边长_mm = 0;    // voxel size
        bool 本帧包含更新 = true;
        std::uint64_t 轮廓锚 = 0; // 对齐到“最新稳态2D轮廓”的锚（采样hash），用于稳态计数/写入去抖
    };

public:
    explicit 三维雕刻写入线程类(const 参数& p = {}) : p_(p) {}
    ~三维雕刻写入线程类() { 停止(); }

    void 启动()
    {
        bool expected = false;
        if (!running_.compare_exchange_strong(expected, true)) return;

        worker_ = std::thread([this] { this->线程循环_(); });
    }

    void 停止()
    {
        if (!running_.exchange(false)) return;
        {
            std::lock_guard lk(mtx_);
            stop_ = true;
        }
        cv_.notify_all();
        if (worker_.joinable()) worker_.join();
        stop_ = false;
        清空队列_();
        清空状态_();
    }

    bool 正在运行() const noexcept { return running_.load(); }

    void 提交(const 任务& t)
    {
        if (!running_.load()) 启动();
        if (!t.e) return;
        if (!t.本帧包含更新) return;
        if (t.blocks.empty()) return;

        std::lock_guard lk(mtx_);
        if (q_.size() >= std::max<std::size_t>(1, p_.最大排队)) q_.pop_front();
        q_.push_back(t);
        cv_.notify_one();
    }

    void 提交(存在节点类* e, const 结构体_存在观测& obs, std::uint64_t 轮廓锚 = 0)
    {
        任务 t;
        t.e = e;
        t.blocks = obs.三维模型_体素立方体;
        t.colorsRGBA = obs.三维模型_体素颜色_RGBA;
        t.NB = obs.体素_nb;
        t.体素边长_mm = obs.体素边长_mm;
        t.本帧包含更新 = obs.本帧包含三维模型更新;
        t.轮廓锚 = 轮廓锚;
        提交(t);
    }

private:
    // =========================
    // popcount - 使用通用函数类的实现
    // =========================
    static std::int64_t 统计占据bit数(const VecIU64& v) noexcept
    {
        std::int64_t s = 0;
        for (auto x : v) s += 通用函数类::popcnt64(x);
        return s;
    }

    // 加权Jaccard相似度：对占据区域给予更高权重，提高稳定性判定的鲁棒性
    static double 相似度_Jaccard_Blocks(const VecIU64& a, const VecIU64& b) noexcept
    {
        if (a.empty() && b.empty()) return 1.0;
        if (a.empty() || b.empty()) return 0.0;

        const std::size_t na = a.size();
        const std::size_t nb = b.size();
        const std::size_t n = (na < nb) ? na : nb;

        std::int64_t inter = 0;
        std::int64_t uni = 0;
        std::int64_t total_occ = 0;

        // 计算基础的交并集
        for (std::size_t i = 0; i < n; ++i) {
            const std::uint64_t A = a[i];
            const std::uint64_t B = b[i];
            inter += 通用函数.popcnt64(A & B);
            uni += 通用函数.popcnt64(A | B);
            total_occ += 通用函数.popcnt64(A) + 通用函数.popcnt64(B);
        }
        for (std::size_t i = n; i < na; ++i) {
            uni += 通用函数.popcnt64(a[i]);
            total_occ += 通用函数.popcnt64(a[i]);
        }
        for (std::size_t i = n; i < nb; ++i) {
            uni += 通用函数.popcnt64(b[i]);
            total_occ += 通用函数.popcnt64(b[i]);
        }

        if (uni <= 0) return 1.0;
        if (total_occ == 0) return 1.0; // 都为空

        double base_sim = (double)inter / (double)uni;
        
        // 计算占据率
        double occ_rate = (double)total_occ / (double)(2 * 64 * ((na + nb) / 2));
        
        // 加权因子：占据率越高，权重越大，提高对重要区域的敏感性
        double weight = 0.5 + 0.5 * occ_rate;
        
        return (base_sim * weight) + (1.0 - weight) * (occ_rate > 0.1 ? 1.0 : 0.0);
    }

    // RGBA32 -> VecIU64：每个 u64 = (hi32=rgba1, lo32=rgba0)
    static void 打包_RGBA32_到VecIU64(const std::vector<std::uint32_t>& rgba, VecIU64& out)
    {
        out.clear();
        if (rgba.empty()) return;
        out.reserve((rgba.size() + 1) / 2);
        for (std::size_t i = 0; i < rgba.size(); i += 2) {
            const std::uint64_t lo = (std::uint64_t)rgba[i];
            const std::uint64_t hi = (i + 1 < rgba.size()) ? ((std::uint64_t)rgba[i + 1] << 32) : 0ull;
            out.push_back(lo | hi);
        }
    }

    // 采样签名：FNV-1a 64（对 VecIU64 采样）做粗签名
    static std::uint64_t 采样签名_u64vec(const VecIU64& v) noexcept
    {
        std::uint64_t h = 1469598103934665603ull;
        const std::uint64_t prime = 1099511628211ull;

        const std::size_t n = v.size();
        h ^= (std::uint64_t)n; h *= prime;
        if (n == 0) return h;

        const std::size_t step = (n < 64) ? 1 : (n / 64);
        for (std::size_t i = 0; i < n; i += step) { h ^= v[i]; h *= prime; }

        h ^= v.back(); h *= prime;
        return h;
    }

    std::uint64_t 计算任务签名_(const 任务& t) const noexcept
    {
        std::uint64_t h = 1469598103934665603ull;
        const std::uint64_t prime = 1099511628211ull;

        h ^= (std::uint64_t)std::max(0, t.NB);          h *= prime;
        h ^= (std::uint64_t)std::max(0, t.体素边长_mm); h *= prime;

        const std::uint64_t hb = 采样签名_u64vec(t.blocks);
        h ^= hb; h *= prime;

        if (!t.colorsRGBA.empty()) {
            std::uint64_t hc = 1469598103934665603ull;
            const std::size_t n = t.colorsRGBA.size();
            hc ^= (std::uint64_t)n; hc *= prime;
            const std::size_t step = (n < 64) ? 1 : (n / 64);
            for (std::size_t i = 0; i < n; i += step) { hc ^= (std::uint64_t)t.colorsRGBA[i]; hc *= prime; }
            hc ^= (std::uint64_t)t.colorsRGBA.back(); hc *= prime;

            h ^= hc; h *= prime;
        }

        return h;
    }
    // =========================
    // 特征写入：覆盖当前值
    // =========================
    // 线程模块不直接操作 特征值集/存在集/特征集。
    // 统一调用 世界树类 的应用写入口（内部会获取/创建特征值节点并覆盖当前值）。

private:
    struct 状态
    {
        VecIU64 上一帧Blocks;
        std::uint64_t 上一帧Sig = 0;
        int 上一帧NB = 0;
        int 上一帧Voxel = 0;
        std::uint64_t 上一帧轮廓锚 = 0;

        int 稳定计数 = 0;

        bool 已提交过 = false;
        VecIU64 已提交Blocks;
        std::uint64_t 已提交Sig = 0;
        int 已提交NB = 0;
        int 已提交Voxel = 0;
        std::uint64_t 已写入轮廓锚 = 0;

        int 距离上次写入帧数 = 0;
    };

    bool 候选分辨率更高_(const 任务& t, const 状态& st) const noexcept
    {
        if (!st.已提交过) return true;
        
        const int t_voxel_size = std::max(1, t.体素边长_mm);
        const int st_voxel_size = std::max(1, st.已提交Voxel);
        
        const int t_nb = std::max(1, t.NB);
        const int st_nb = std::max(1, st.已提交NB);
        
        // 计算实际体素数量
        const std::size_t t_voxel_count = t.blocks.size() * t_nb;
        const std::size_t st_voxel_count = st.已提交Blocks.size() * st_nb;
        
        // 计算模型覆盖的体积（体素数量 * 体素体积）
        const double t_volume = t_voxel_count * t_voxel_size * t_voxel_size * t_voxel_size;
        const double st_volume = st_voxel_count * st_voxel_size * st_voxel_size * st_voxel_size;
        
        // 1. 体素尺寸更小（更高分辨率）
        const bool voxel_resolution_better = (t_voxel_size < st_voxel_size);
        
        // 2. 体素数量更多（更详细的模型）
        const bool more_voxels = (t_voxel_count > st_voxel_count * 1.1); // 至少多10%
        
        // 3. 覆盖体积更大但分辨率不降低（更大的模型范围）
        const bool larger_volume_same_res = (t_volume > st_volume * 1.2) && (t_voxel_size <= st_voxel_size);
        
        // 4. 特殊情况：如果当前模型非常小，允许任何更大的模型覆盖
        const bool current_model_too_small = (st_voxel_count < 1000);
        
        return voxel_resolution_better || more_voxels || larger_volume_same_res || current_model_too_small;
    }

    bool 分辨率可比_(int nb1, int v1, int nb2, int v2) const noexcept
    {
        return (nb1 == nb2) && (v1 == v2);
    }


    void 处理任务_(const 任务& task)
    {
        if (!task.e) return;
        if (task.blocks.empty()) return;

        状态 st;
        {
            std::unique_lock lk(state_mtx_);
            st = state_[task.e];
        }

        // 计“到上次写入”的帧数（用于节流）
        st.距离上次写入帧数 += 1;

        // 轮廓锚变化：视为“新的稳态外观”，重置稳定计数与上一帧参考
        if (task.轮廓锚 != st.上一帧轮廓锚) {
            st.上一帧轮廓锚 = task.轮廓锚;
            st.稳定计数 = 0;
            st.上一帧Blocks.clear();
            st.上一帧Sig = 0;
            st.上一帧NB = 0;
            st.上一帧Voxel = 0;
            // 不清空“已提交”，允许跨锚复用（如果blocks几乎相同则跳过写入）
        }

        // -------------------------------------------------
        // 0) 分辨率太小：不建/不写三维模型（< 8*8*8 体素）
        // 说明：blocks 每块包含 NB 个体素（通常 NB=64）。
        // 最小需要 512 个体素（8^3），即 ceil(512/NB) 个 blocks。
        // -------------------------------------------------
        {
            const std::size_t nb = (task.NB > 0) ? (std::size_t)task.NB : (std::size_t)64;
            const std::size_t minBlocks = (512ull + nb - 1ull) / nb;
            if (task.blocks.size() < minBlocks) {
                // 仍更新“上一帧”缓存，避免下一帧突然变大时判定异常；但稳定计数清零。
                st.上一帧Blocks = task.blocks;
                st.上一帧NB = task.NB;
                st.上一帧Voxel = task.体素边长_mm;
                st.上一帧Sig = 0;
                st.稳定计数 = 0;

                std::unique_lock lk(state_mtx_);
                state_[task.e] = std::move(st);
                return;
            }
        }

        const std::int64_t occBits = 统计占据bit数(task.blocks);
        const std::uint64_t sig = 计算任务签名_(task);

        // =========================
        // 1) 稳定性：对“生成后的模型（blocks）”做比对
        // =========================
        const bool sameResPrev =
            (!st.上一帧Blocks.empty()) &&
            分辨率可比_(task.NB, task.体素边长_mm, st.上一帧NB, st.上一帧Voxel);

        double simPrev = 0.0;
        if (sameResPrev) simPrev = 相似度_Jaccard_Blocks(task.blocks, st.上一帧Blocks);

        if (!sameResPrev) {
            st.稳定计数 = 1;
        }
        else {
            if (simPrev >= p_.相邻帧相似阈值) st.稳定计数 += 1;
            else st.稳定计数 = 1;
        }

        st.上一帧Blocks = task.blocks;
        st.上一帧Sig = sig;
        st.上一帧NB = task.NB;
        st.上一帧Voxel = task.体素边长_mm;

        const bool 稳定 = (st.稳定计数 >= std::max(1, p_.需要稳定帧数));
        if (!稳定) {
            std::unique_lock lk(state_mtx_);
            state_[task.e] = std::move(st);
            return;
        }

        // =========================
        // 2) 过滤：过小/过稀疏的模型不写
        // =========================
        if (occBits < (std::int64_t)std::max(0, p_.最小占据bit数)) {
            std::unique_lock lk(state_mtx_);
            state_[task.e] = std::move(st);
            return;
        }

        // =========================
        // 3) 节流：同一存在两次写入间隔
        // =========================
        if (st.已提交过 && st.距离上次写入帧数 < std::max(0, p_.写入最小间隔帧)) {
            std::unique_lock lk(state_mtx_);
            state_[task.e] = std::move(st);
            return;
        }

        // =========================
        // 4) 写入前比对：候选 vs 已提交
        // =========================
        const bool sameResCommit =
            st.已提交过 &&
            分辨率可比_(task.NB, task.体素边长_mm, st.已提交NB, st.已提交Voxel);

        double simCommit = 0.0;
        if (sameResCommit) simCommit = 相似度_Jaccard_Blocks(task.blocks, st.已提交Blocks);

        const bool resBetter = 候选分辨率更高_(task, st);

        // 同分辨率且几乎一致：跳过写入
        if (st.已提交过 && sameResCommit &&
            simCommit >= p_.已提交相似跳过阈值 &&
            !resBetter)
        {
            st.稳定计数 = 0;
            std::unique_lock lk(state_mtx_);
            state_[task.e] = std::move(st);
            return;
        }

        // 分辨率不一致：只允许“更高分辨率”覆盖
        if (st.已提交过 && !sameResCommit) {
            if (!p_.允许分辨率提升 || !resBetter) {
                st.稳定计数 = 0;
                std::unique_lock lk(state_mtx_);
                state_[task.e] = std::move(st);
                return;
            }
            // 提升覆盖要求更强稳定
            if (st.稳定计数 < std::max(1, p_.需要稳定帧数 + 2)) {
                std::unique_lock lk(state_mtx_);
                state_[task.e] = std::move(st);
                return;
            }
        }

        // =========================
        // 5) 写入：blocks + colors + signature
        // =========================
        const 词性节点类* tBlocks = p_.Blocks特征类型 ? p_.Blocks特征类型 : 特征类型定义类::类型_三维模型_体素立方体;
        const 词性节点类* tColor = p_.颜色特征类型 ? p_.颜色特征类型 : 特征类型定义类::类型_三维模型颜色;
    

        if (tBlocks) 世界树.写VecIU64_覆盖当前(task.e, tBlocks, task.blocks);

        if (tColor && !task.colorsRGBA.empty()) {
            VecIU64 packed;
            打包_RGBA32_到VecIU64(task.colorsRGBA, packed);
            if (!packed.empty()) 世界树.写VecIU64_覆盖当前(task.e, tColor, packed);
        }        

        st.已提交过 = true;
        st.已提交Blocks = task.blocks;
        st.已提交Sig = sig;
        st.已提交NB = task.NB;
        st.已提交Voxel = task.体素边长_mm;
        st.距离上次写入帧数 = 0;
        st.稳定计数 = 0;

        std::unique_lock lk(state_mtx_);
        state_[task.e] = std::move(st);
    }

    void 线程循环_()
    {
        // 线程启动日志
        日志::运行("[三维雕刻写入线程] 线程启动");
        
        while (running_.load())
        {
            任务 task;
            {
                std::unique_lock lk(mtx_);
                cv_.wait(lk, [&] { return stop_ || !q_.empty() || !running_.load(); });
                if (!running_.load()) break;
                if (stop_) break;
                if (q_.empty()) continue;

                task = std::move(q_.front());
                q_.pop_front();
            }

            处理任务_(task);
        }
        
        // 线程结束日志
        日志::运行("[三维雕刻写入线程] 线程结束");
    }

    void 清空队列_()
    {
        std::lock_guard lk(mtx_);
        q_.clear();
    }

    void 清空状态_()
    {
        std::unique_lock lk(state_mtx_);
        state_.clear();
    }

private:
    参数 p_;

    std::thread worker_;
    std::atomic<bool> running_{ false };

    std::mutex mtx_;
    std::condition_variable cv_;
    std::deque<任务> q_;
    bool stop_ = false;

    std::mutex state_mtx_;
    std::unordered_map<const 存在节点类*, 状态> state_;
};
