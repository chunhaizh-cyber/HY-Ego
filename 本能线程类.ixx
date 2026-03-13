export module 本能线程模块;

import <array>;
import <cstdint>;
import <string>;
import <vector>;
import <deque>;
import <unordered_map>;
import <mutex>;
import <condition_variable>;
import <thread>;
import <atomic>;
import <functional>;
import <sstream>;
import <utility>;
import <algorithm>;

import 日志模块;
import 线程模板模块;
import 主信息定义模块;

// ===========================
// 数据包（快照/结果）——纯数据，不携带指针生命周期
// ===========================
export struct 绝对坐标快照 {
    // 业务侧自行决定如何填充。推荐：相对位置(mm) + 场景/相机位姿(4x4) 或其它必要信息。
    std::array<std::int64_t, 3> 相对位置_mm{ 0,0,0 };
    // 行主序4x4
    std::array<float, 16> 世界变换4x4{ 1,0,0,0,
                                       0,1,0,0,
                                       0,0,1,0,
                                       0,0,0,1 };
};

export struct 绝对坐标结果 {
    const 存在节点类* e = nullptr;
    std::array<std::int64_t, 3> 绝对位置_mm{ 0,0,0 };
};

export struct 子特征快照 {
    const 存在节点类* 父 = nullptr;
    std::uint64_t 版本签名 = 0;

    // 典型输入：2D轮廓编码 blocks（8x8 tile blocks）
    std::vector<std::uint64_t> 轮廓blocks;

    // 可选：观测裁剪图的像素/掩膜（如你需要从相机原图做颜色统计）
    // 注意：这里不强依赖 OpenCV；业务侧可以自行决定编码方式。
    std::int32_t 裁剪宽 = 0;
    std::int32_t 裁剪高 = 0;
    std::vector<std::uint8_t> 裁剪BGR;   // 3*w*h
    std::vector<std::uint8_t> 裁剪mask;  // 1*w*h (0/255)
};

export struct 子部件结果 {
    std::string 部件键; // 用于索引（例如 "handle"、"button" 或 hash key）

    std::array<std::int64_t, 3> 相对位置_mm{ 0,0,0 }; // 相对父存在
    std::array<std::int64_t, 3> 尺寸_mm{ 1,1,1 };

    std::vector<std::uint64_t> 轮廓blocks; // 子轮廓编码
    std::array<std::int64_t, 3> 颜色_BGR{ 0,0,0 };     // 平均色或代表色
};

export struct 子特征结果 {
    const 存在节点类* 父 = nullptr;
    std::uint64_t 版本签名 = 0;
    std::vector<子部件结果> 部件;
};

// ===========================
// 本能线程：
// - 前台：消息线程（合并/去重/快照/回写）
// - 后台：worker（重计算，不拿世界树锁）
// 设计目标：
// - 大计算不阻塞消息线程/渲染线程
// - 写世界树仍保持串行（由回写回调保证正确加锁）
// ===========================
export class 本能线程类 {
public:
    // 业务侧提供：快照/计算/回写
    using Fn取绝对快照 = std::function<bool(const 存在节点类* e, 绝对坐标快照& out)>;
    using Fn算绝对坐标 = std::function<bool(const 绝对坐标快照& in, 绝对坐标结果& out)>;
    using Fn写入绝对坐标 = std::function<void(const 绝对坐标结果& r)>;

    using Fn取子特征快照 = std::function<bool(const 存在节点类* 父, std::uint64_t sig, 子特征快照& out)>;
    using Fn算子特征 = std::function<bool(const 子特征快照& in, 子特征结果& out)>;
    using Fn写入子特征 = std::function<void(const 子特征结果& r)>;

public:
    本能线程类() {
        // 绑定消息处理
        msg_.设置处理函数([this](const 消息& m) { this->处理消息_(m); });
    }

    ~本能线程类() { 停止(); }

    本能线程类(const 本能线程类&) = delete;
    本能线程类& operator=(const 本能线程类&) = delete;

public:
    void 设置_绝对坐标流程(Fn取绝对快照 snap, Fn算绝对坐标 calc, Fn写入绝对坐标 commit) {
        fn_abs_snap_ = std::move(snap);
        fn_abs_calc_ = std::move(calc);
        fn_abs_commit_ = std::move(commit);
    }

    void 设置_子特征流程(Fn取子特征快照 snap, Fn算子特征 calc, Fn写入子特征 commit) {
        fn_sub_snap_ = std::move(snap);
        fn_sub_calc_ = std::move(calc);
        fn_sub_commit_ = std::move(commit);
    }

public:
    void 启动() {
        if (running_.exchange(true)) return;
        stop_.store(false);

        // worker
        worker_ = std::thread([this] { this->worker线程_(); });

        // 消息线程
        msg_.启动();

        日志::运行("[本能线程] 启动");
    }

    void 停止() {
        if (!running_.load()) return;
        stop_.store(true);

        // 先停消息线程，避免继续入队
        msg_.停止();

        // 再停worker
        {
            std::unique_lock lk(w_mtx_);
            w_cv_.notify_all();
        }
        if (worker_.joinable()) worker_.join();

        // 清理
        {
            std::unique_lock lk(state_mtx_);
            pending_abs_.clear();
            running_abs_.clear();
            pending_sub_sig_.clear();
            running_sub_sig_.clear();
        }
        {
            std::unique_lock lk(w_mtx_);
            w_q_.clear();
            r_q_.clear();
        }

        running_.store(false);
        日志::运行("[本能线程] 停止");
    }

    bool 是否正在运行() const noexcept { return running_.load(); }

public:
    // ===========================
    // 对外：投递任务（建议由别的线程调用）
    // ===========================

    // 绝对坐标刷新：同一个存在合并（last-wins）
    void 请求刷新绝对坐标(const 存在节点类* e) {
        if (!e) return;
        msg_.发送合并消息(键_abs_(e), "1");
    }

    // 子特征提取：同一个存在合并（last-wins），内容带版本签名
    void 请求提取子特征(const 存在节点类* 父, std::uint64_t sig) {
        if (!父) return;
        msg_.发送合并消息(键_sub_(父), u64hex_(sig));
    }

    // 存在移除：立即清理 pending / running / 队列（方案A配合删除回调）
    void 通知存在已移除(const 存在节点类* e) {
        if (!e) return;
        msg_.发送消息(消息{ "", std::string("rm|") + ptrhex_(e) });
    }

private:
    // ===========================
    // worker 任务/结果
    // ===========================
    enum class 枚举_任务类型 : std::uint8_t { 绝对坐标 = 1, 子特征 = 2 };

    struct 任务 {
        枚举_任务类型 type{};
        const 存在节点类* e = nullptr; // 绝对坐标：e；子特征：父
        std::uint64_t sig = 0;          // 子特征版本
        绝对坐标快照 abs_in;
        子特征快照 sub_in;
    };

    struct 结果 {
        枚举_任务类型 type{};
        const 存在节点类* e = nullptr;
        std::uint64_t sig = 0;
        绝对坐标结果 abs_out;
        子特征结果 sub_out;
        bool ok = false;
    };

private:
    // ===========================
    // 消息线程：解析消息 + 调度 worker + 回写
    // ===========================
    void 处理消息_(const 消息& m) {
        if (stop_.load()) return;

        // 1) 删除通知
        if (!m.内容.empty() && m.内容.rfind("rm|", 0) == 0) {
            const auto* e = (const 存在节点类*)ptrfromhex_(m.内容.substr(3));
            if (e) 清理存在_(e);
            return;
        }

        // 2) worker 完成
        if (!m.内容.empty() && m.内容.rfind("done|", 0) == 0) {
            // done|type|ptr|sig
            处理完成消息_(m.内容);
            return;
        }

        // 3) 合并任务消息（按 key）
        if (!m.key.empty()) {
            if (m.key.rfind("abs:", 0) == 0) {
                const auto* e = (const 存在节点类*)ptrfromhex_(m.key.substr(4));
                if (e) 安排绝对坐标_(e);
                return;
            }
            if (m.key.rfind("sub:", 0) == 0) {
                const auto* 父 = (const 存在节点类*)ptrfromhex_(m.key.substr(4));
                std::uint64_t sig = hexu64_(m.内容);
                if (父) 安排子特征_(父, sig);
                return;
            }
        }
    }

    void 安排绝对坐标_(const 存在节点类* e) {
        if (!fn_abs_snap_ || !fn_abs_calc_ || !fn_abs_commit_) return;

        {
            std::unique_lock lk(state_mtx_);
            pending_abs_[e] = true;
            if (running_abs_[e]) return; // 已在算
            running_abs_[e] = true;
        }

        // 快照化（尽快返回，不在此做重计算）
        绝对坐标快照 snap;
        if (!fn_abs_snap_(e, snap)) {
            std::unique_lock lk(state_mtx_);
            running_abs_[e] = false;
            pending_abs_[e] = false;
            return;
        }

        任务 t;
        t.type = 枚举_任务类型::绝对坐标;
        t.e = e;
        t.abs_in = std::move(snap);

        入队任务_(std::move(t));
    }

    void 安排子特征_(const 存在节点类* 父, std::uint64_t sig) {
        if (!fn_sub_snap_ || !fn_sub_calc_ || !fn_sub_commit_) return;

        {
            std::unique_lock lk(state_mtx_);
            pending_sub_sig_[父] = sig;

            // 如果正在算同一个sig，则无需重复触发
            auto itRun = running_sub_sig_.find(父);
            if (itRun != running_sub_sig_.end() && itRun->second != 0) {
                return;
            }
            running_sub_sig_[父] = sig;
        }

        子特征快照 snap;
        if (!fn_sub_snap_(父, sig, snap)) {
            std::unique_lock lk(state_mtx_);
            running_sub_sig_[父] = 0;
            // pending 保留（可能下一次快照可用），也可选择置0
            return;
        }

        任务 t;
        t.type = 枚举_任务类型::子特征;
        t.e = 父;
        t.sig = sig;
        t.sub_in = std::move(snap);

        入队任务_(std::move(t));
    }

    void 处理完成消息_(const std::string& s) {
        // done|type|ptr|sig
        // type: 1 abs, 2 sub
        auto parts = split_(s, '|');
        if (parts.size() < 4) return;

        int type = std::atoi(parts[1].c_str());
        const auto* e = (const 存在节点类*)ptrfromhex_(parts[2]);
        std::uint64_t sig = hexu64_(parts[3]);

        结果 r;
        bool ok = false;
        {
            std::unique_lock lk(w_mtx_);
            // 从结果队列中找一个匹配的（队列不大，线性找即可）
            for (auto it = r_q_.begin(); it != r_q_.end(); ++it) {
                if ((int)it->type == type && it->e == e && it->sig == sig) {
                    r = std::move(*it);
                    r_q_.erase(it);
                    ok = true;
                    break;
                }
            }
        }
        if (!ok) return;

        // 回写（由业务侧回调负责加锁）
        try {
            if (r.ok) {
                if (r.type == 枚举_任务类型::绝对坐标) fn_abs_commit_(r.abs_out);
                else if (r.type == 枚举_任务类型::子特征) fn_sub_commit_(r.sub_out);
            }
        } catch (...) {}

        // 更新状态 & 触发下一轮（如果有新的 pending）
        if (r.type == 枚举_任务类型::绝对坐标) {
            bool need_again = false;
            {
                std::unique_lock lk(state_mtx_);
                running_abs_[e] = false;
                // 如果在运行期间又来了请求，则再跑一次
                if (pending_abs_[e]) {
                    pending_abs_[e] = false;
                    // 这里不直接安排，避免递归，发一条合并消息
                    need_again = true;
                }
            }
            if (need_again) msg_.发送合并消息(键_abs_(e), "1");
        }
        else if (r.type == 枚举_任务类型::子特征) {
            std::uint64_t pending = 0;
            {
                std::unique_lock lk(state_mtx_);
                running_sub_sig_[e] = 0;
                pending = pending_sub_sig_[e];
            }
            if (pending != 0 && pending != sig) {
                // 新版本来了，跑最新
                msg_.发送合并消息(键_sub_(e), u64hex_(pending));
            }
        }
    }

    void 清理存在_(const 存在节点类* e) {
        {
            std::unique_lock lk(state_mtx_);
            pending_abs_.erase(e);
            running_abs_.erase(e);
            pending_sub_sig_.erase(e);
            running_sub_sig_.erase(e);
        }

        // 清理 worker 队列中相关任务
        {
            std::unique_lock lk(w_mtx_);
            for (auto it = w_q_.begin(); it != w_q_.end(); ) {
                if (it->e == e) it = w_q_.erase(it);
                else ++it;
            }
            for (auto it = r_q_.begin(); it != r_q_.end(); ) {
                if (it->e == e) it = r_q_.erase(it);
                else ++it;
            }
        }
    }

private:
    void 入队任务_(任务 t) {
        if (stop_.load()) return;
        {
            std::unique_lock lk(w_mtx_);
            w_q_.push_back(std::move(t));
        }
        w_cv_.notify_one();
    }

    void worker线程_() {
        日志::运行("[本能线程] worker 启动");

        while (!stop_.load()) {
            任务 t;
            bool has = false;

            {
                std::unique_lock lk(w_mtx_);
                w_cv_.wait(lk, [&] { return stop_.load() || !w_q_.empty(); });
                if (stop_.load()) break;
                if (!w_q_.empty()) {
                    t = std::move(w_q_.front());
                    w_q_.pop_front();
                    has = true;
                }
            }

            if (!has) continue;

            结果 r;
            r.type = t.type;
            r.e = t.e;
            r.sig = t.sig;

            try {
                if (t.type == 枚举_任务类型::绝对坐标) {
                    r.abs_out.e = t.e;
                    r.ok = fn_abs_calc_ ? fn_abs_calc_(t.abs_in, r.abs_out) : false;
                }
                else if (t.type == 枚举_任务类型::子特征) {
                    r.sub_out.父 = t.e;
                    r.sub_out.版本签名 = t.sig;
                    r.ok = fn_sub_calc_ ? fn_sub_calc_(t.sub_in, r.sub_out) : false;
                }
            }
            catch (...) { r.ok = false; }

            {
                std::unique_lock lk(w_mtx_);
                r_q_.push_back(r);
            }

            // 通知消息线程回写（不要合并，避免漏写）
            msg_.发送消息(消息{ "", std::string("done|") + std::to_string((int)r.type) + "|" + ptrhex_(r.e) + "|" + u64hex_(r.sig) });
        }

        日志::运行("[本能线程] worker 结束");
    }

private:
    // ===========================
    // 小工具
    // ===========================
    static std::string ptrhex_(const void* p) {
        std::uintptr_t v = (std::uintptr_t)p;
        std::ostringstream os;
        os << std::hex << v;
        return os.str();
    }
    static std::uintptr_t ptrfromhex_(const std::string& s) {
        std::uintptr_t v = 0;
        std::istringstream is(s);
        is >> std::hex >> v;
        return v;
    }
    static std::string u64hex_(std::uint64_t v) {
        std::ostringstream os;
        os << std::hex << v;
        return os.str();
    }
    static std::uint64_t hexu64_(const std::string& s) {
        std::uint64_t v = 0;
        std::istringstream is(s);
        is >> std::hex >> v;
        return v;
    }
    static std::vector<std::string> split_(const std::string& s, char sep) {
        std::vector<std::string> out;
        std::string cur;
        for (char c : s) {
            if (c == sep) {
                out.push_back(cur);
                cur.clear();
            }
            else cur.push_back(c);
        }
        out.push_back(cur);
        return out;
    }

    static std::string 键_abs_(const 存在节点类* e) { return std::string("abs:") + ptrhex_(e); }
    static std::string 键_sub_(const 存在节点类* e) { return std::string("sub:") + ptrhex_(e); }

private:
    // 消息线程
    线程模板类 msg_;

    // worker
    std::mutex w_mtx_;
    std::condition_variable w_cv_;
    std::deque<任务> w_q_;
    std::deque<结果> r_q_;
    std::thread worker_;

    // 去重状态
    std::mutex state_mtx_;
    std::unordered_map<const 存在节点类*, bool> pending_abs_;
    std::unordered_map<const 存在节点类*, bool> running_abs_;

    std::unordered_map<const 存在节点类*, std::uint64_t> pending_sub_sig_;
    std::unordered_map<const 存在节点类*, std::uint64_t> running_sub_sig_;

    // 回调
    Fn取绝对快照 fn_abs_snap_;
    Fn算绝对坐标 fn_abs_calc_;
    Fn写入绝对坐标 fn_abs_commit_;

    Fn取子特征快照 fn_sub_snap_;
    Fn算子特征 fn_sub_calc_;
    Fn写入子特征 fn_sub_commit_;

    std::atomic_bool running_{ false };
    std::atomic_bool stop_{ false };
};
