module;
#include <vector>
#include <string>
#include <cstdint>
#include <unordered_map>
#include <algorithm>
#include <variant>

export module 动态模块;


import 模板模块;
import 基础数据类型模块;
import 主信息定义模块;
import 数据仓库模块;
import 通用函数模块;
import 状态模块;
using namespace 数据仓库模块; 
// ============================================================
// 动态模块：
// - “叶子动态”像状态一样：不入世界链、不设主键，仅存放于过程场景的 动态列表（顺序序列）。
// - “提炼动态”写入 动态树（入世界链、有主键），用于沉淀可重复的动态片段。
// - 动态融合：在动态序列中发现重复出现的连续片段 -> 生成父动态节点（挂在动态树根下）。
// ============================================================

// 提炼动态主信息（扩展 动态节点主信息类）
// - 指纹：序列的 FNV1a64
// - 序列：由“叶子动态符号”组成（稳定、可持久化）
// - 重复次数：本次提炼时在序列中出现的非重叠次数
export class 提炼动态主信息类 final : public 动态节点主信息类 {
public:
    std::uint64_t 指纹 = 0;
    std::vector<std::uint64_t> 序列{};
    std::int32_t 重复次数 = 0;
    时间戳 创建时间 = 0;

public:
    提炼动态主信息类() = default;
};

export class 动态类 {
public:
    动态类() = default;
    ~动态类() = default;

    动态类(const 动态类&) = delete;
    动态类& operator=(const 动态类&) = delete;

private:
    // 动态树根（入世界链）。注意：叶子动态不在世界链里。
    动态节点类* 动态树根_ = nullptr;

    // 运行期索引：用于避免重复写入同一序列（指纹 -> 节点）
    std::unordered_map<std::uint64_t, 动态节点类*> 提炼索引_{};

    static constexpr 时间戳 默认事件动态保留时长_微秒_ = 60 * 1000 * 1000;
    static constexpr std::size_t 默认事件动态最大保留数_ = 256;

private:
    static inline std::uint64_t 私有_FNV64(const void* p, std::size_t bytes) noexcept
    {
        if (!p || bytes == 0) return 0ull;
        return 通用函数模块::FNV1a64(p, bytes);
    }

    // 叶子动态符号：把“初始/结果状态 + 事件类型 + 对应信息节点”压成一个稳定的 u64
    static std::uint64_t 私有_叶子动态符号(const 动态节点类* d) noexcept
    {
        if (!d || !d->主信息) return 0ull;

        // 1) 若是“提炼动态”，直接用其指纹作为符号（便于层级继续生长）
        if (auto* ex = dynamic_cast<提炼动态主信息类*>(d->主信息)) {
            return ex->指纹;
        }

        // 2) 普通叶子动态：基于 初始/结果状态 的事件/对应节点 + 指针值 计算符号
        auto* mi = dynamic_cast<动态节点主信息类*>(d->主信息);
        if (!mi) return 0ull;

        const 状态节点类* s0 = (const 状态节点类*)mi->初始状态;
        const 状态节点类* s1 = (const 状态节点类*)mi->结果状态;

        // 尽量用“事件+对应信息节点”组合；没有扩展信息时退化为指针。
        std::uint64_t pack[8]{};

        std::size_t k = 0;
        {
            auto e = 取状态事件(s0);
            pack[k++] = (std::uint64_t)e;
            if (s0 && s0->主信息) {
                auto* smi0 = dynamic_cast<状态节点主信息类*>(s0->主信息);
                pack[k++] = (std::uint64_t)(std::uintptr_t)(smi0 ? smi0->对应信息节点 : nullptr);
                pack[k++] = (std::uint64_t)(smi0 ? (smi0->是否变化 ? 1 : 0) : 0);
            }
            pack[k++] = (std::uint64_t)(std::uintptr_t)s0;
        }
        {
            auto e = 取状态事件(s1);
            pack[k++] = (std::uint64_t)e;
            if (s1 && s1->主信息) {
                auto* smi1 = dynamic_cast<状态节点主信息类*>(s1->主信息);
                pack[k++] = (std::uint64_t)(std::uintptr_t)(smi1 ? smi1->对应信息节点 : nullptr);
                pack[k++] = (std::uint64_t)(smi1 ? (smi1->是否变化 ? 1 : 0) : 0);
            }
            pack[k++] = (std::uint64_t)(std::uintptr_t)s1;
        }

        return 私有_FNV64(pack, sizeof(pack));
    }

    static bool 私有_序列相等(const std::vector<std::uint64_t>& a, int ia,
        const std::vector<std::uint64_t>& b, int ib, int len) noexcept
    {
        if (len <= 0) return true;
        for (int i = 0; i < len; ++i) {
            if (a[(std::size_t)ia + (std::size_t)i] != b[(std::size_t)ib + (std::size_t)i]) return false;
        }
        return true;
    }

    static std::vector<int> 私有_选择非重叠(std::vector<int> pos, int len)
    {
        std::sort(pos.begin(), pos.end());
        std::vector<int> out;
        out.reserve(pos.size());
        int last = -1'000'000'000;
        for (int p : pos) {
            if (out.empty() || p >= last + len) {
                out.push_back(p);
                last = p;
            }
        }
        return out;
    }

    static 状态节点主信息类* 私有_取状态主信息(const 状态节点类* s) noexcept
    {
        return (s && s->主信息) ? dynamic_cast<状态节点主信息类*>(s->主信息) : nullptr;
    }

    static 时间戳 私有_取状态时间(const 状态节点类* s) noexcept
    {
        auto* mi = 私有_取状态主信息(s);
        if (!mi) return 0;
        return mi->发生时间 ? mi->发生时间 : mi->收到时间;
    }

    static void 私有_追加唯一事件(std::vector<状态节点类*>& out, 状态节点类* s)
    {
        if (!s) return;
        if (std::find(out.begin(), out.end(), s) != out.end()) return;
        out.push_back(s);
    }

    static void 私有_追加唯一状态路径(std::vector<状态节点类*>& out, 状态节点类* s)
    {
        if (!s) return;
        if (!out.empty() && out.back() == s) return;
        out.push_back(s);
    }

    static void 私有_收集动态事件(const 动态节点类* d, std::vector<状态节点类*>& out)
    {
        auto* mi = (d && d->主信息) ? dynamic_cast<动态节点主信息类*>(d->主信息) : nullptr;
        if (!mi) return;
        if (!mi->事件列表.empty()) {
            for (auto* s : mi->事件列表) 私有_追加唯一事件(out, s);
            return;
        }
        私有_追加唯一事件(out, (状态节点类*)mi->初始状态);
        私有_追加唯一事件(out, (状态节点类*)mi->结果状态);
    }
    static 动态节点主信息类* 私有_取动态主信息(const 动态节点类* d) noexcept
    {
        return (d && d->主信息) ? dynamic_cast<动态节点主信息类*>(d->主信息) : nullptr;
    }

    static 时间戳 私有_取动态时间锚点(const 动态节点类* d) noexcept
    {
        auto* mi = 私有_取动态主信息(d);
        if (!mi) return 0;
        return mi->结束时间 ? mi->结束时间 : mi->开始时间;
    }

    static void 私有_清理场景事件动态(
        场景节点类* 场景,
        时间戳 now = 0)
    {
        if (!场景 || !场景->主信息) return;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!smi) return;

        smi->清理空指针();
        if (smi->事件动态列表.empty()) return;
        if (now == 0) now = 结构体_时间戳::当前_微秒();

        if (默认事件动态保留时长_微秒_ > 0 && now > 默认事件动态保留时长_微秒_) {
            const auto 最早保留时间 = now - 默认事件动态保留时长_微秒_;
            smi->事件动态列表.erase(
                std::remove_if(
                    smi->事件动态列表.begin(),
                    smi->事件动态列表.end(),
                    [&](动态节点类* d) {
                        const auto t = 私有_取动态时间锚点(d);
                        return t != 0 && t < 最早保留时间;
                    }),
                smi->事件动态列表.end());
        }

        if (默认事件动态最大保留数_ > 0 && smi->事件动态列表.size() > 默认事件动态最大保留数_) {
            const auto 需裁剪数 = smi->事件动态列表.size() - 默认事件动态最大保留数_;
            smi->事件动态列表.erase(
                smi->事件动态列表.begin(),
                smi->事件动态列表.begin() + static_cast<std::ptrdiff_t>(需裁剪数));
        }
    }

    static void 私有_记录原始动态事件(
        场景节点类* 场景,
        动态节点类* 原始动态,
        时间戳 now = 0)
    {
        if (!场景 || !场景->主信息 || !原始动态) return;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!smi) return;
        smi->添加事件动态(原始动态);
        私有_清理场景事件动态(场景, now);
    }

    static std::uint64_t 私有_状态路径签名(const std::vector<状态节点类*>& 路径) noexcept
    {
        if (路径.empty()) return 0ull;

        std::vector<std::uint64_t> pack;
        pack.reserve(路径.size() * 5);
        for (auto* s : 路径) {
            auto* mi = 私有_取状态主信息(s);
            pack.push_back((std::uint64_t)(std::uintptr_t)(mi ? mi->状态主体 : nullptr));
            pack.push_back((std::uint64_t)(std::uintptr_t)(mi ? mi->状态特征 : nullptr));
            pack.push_back((std::uint64_t)取状态事件(s));
            pack.push_back((std::uint64_t)私有_取状态时间(s));
            if (mi && std::holds_alternative<I64>(mi->状态值)) pack.push_back((std::uint64_t)std::get<I64>(mi->状态值));
            else pack.push_back((std::uint64_t)(std::uintptr_t)s);
        }
        return 私有_FNV64(pack.data(), pack.size() * sizeof(std::uint64_t));
    }

    static void 私有_收集动态路径(const 动态节点类* d, std::vector<状态节点类*>& out)
    {
        auto* mi = 私有_取动态主信息(d);
        if (!mi) return;
        if (!mi->状态路径列表.empty()) {
            for (auto* s : mi->状态路径列表) 私有_追加唯一状态路径(out, s);
            return;
        }
        私有_追加唯一状态路径(out, (状态节点类*)mi->初始状态);
        私有_追加唯一状态路径(out, (状态节点类*)mi->结果状态);
    }

    static bool 私有_状态值不同(const 状态节点类* a, const 状态节点类* b) noexcept
    {
        auto* ma = 私有_取状态主信息(a);
        auto* mb = 私有_取状态主信息(b);
        if (!ma || !mb) return a != b;
        return ma->状态值 != mb->状态值;
    }

    static std::vector<状态节点类*> 私有_提取代表事件(
        const std::vector<状态节点类*>& 全事件,
        std::size_t 最大保留数 = 4)
    {
        std::vector<状态节点类*> 去空;
        去空.reserve(全事件.size());
        for (auto* s : 全事件) {
            if (s) 去空.push_back(s);
        }
        if (去空.size() <= 2 || 去空.size() <= 最大保留数) return 去空;

        std::vector<状态节点类*> out;
        out.reserve((std::min)(去空.size(), 最大保留数));
        out.push_back(去空.front());

        for (std::size_t i = 1; i + 1 < 去空.size(); ++i) {
            auto* cur = 去空[i];
            auto* prev = out.back();
            if (取状态事件(cur) != 取状态事件(prev) || 私有_状态值不同(cur, prev)) {
                out.push_back(cur);
                if (out.size() + 1 >= 最大保留数) break;
            }
        }

        if (out.back() != 去空.back()) out.push_back(去空.back());
        return out;
    }

    static void 私有_挂接子动态(动态节点类* 父动态, 动态节点类* 子动态) noexcept
    {
        if (!父动态 || !子动态) return;

        子动态->父 = 父动态;
        if (!父动态->子) {
            父动态->子 = 子动态;
            子动态->上 = 子动态;
            子动态->下 = 子动态;
            父动态->子节点数量 = 1;
            return;
        }

        auto* first = 父动态->子;
        auto* last = first->上 ? first->上 : first;
        子动态->上 = last;
        子动态->下 = first;
        last->下 = 子动态;
        first->上 = 子动态;
        父动态->子节点数量 += 1;
    }

    static bool 私有_时间段相交(时间戳 a0, 时间戳 a1, 时间戳 b0, 时间戳 b1) noexcept
    {
        if (a0 > a1) std::swap(a0, a1);
        if (b0 > b1) std::swap(b0, b1);
        return !(a1 < b0 || b1 < a0);
    }

    static bool 私有_可形成基础动态(const 状态节点类* 前状态, const 状态节点类* 后状态) noexcept
    {
        if (!前状态 || !后状态 || 前状态 == 后状态) return false;
        auto* 前mi = 私有_取状态主信息(前状态);
        auto* 后mi = 私有_取状态主信息(后状态);
        if (!前mi || !后mi) return false;
        if (!前mi->状态主体 || !后mi->状态主体) return false;
        if (!前mi->状态特征 || !后mi->状态特征) return false;
        if (前mi->状态主体 != 后mi->状态主体) return false;
        if (前mi->状态特征 != 后mi->状态特征) return false;
        return 私有_取状态时间(前状态) <= 私有_取状态时间(后状态);
    }

    static bool 私有_动态可聚合(const 动态节点类* 左动态, const 动态节点类* 右动态) noexcept
    {
        auto* 左mi = (左动态 && 左动态->主信息) ? dynamic_cast<动态节点主信息类*>(左动态->主信息) : nullptr;
        auto* 右mi = (右动态 && 右动态->主信息) ? dynamic_cast<动态节点主信息类*>(右动态->主信息) : nullptr;
        if (!左mi || !右mi) return false;
        if (!左mi->动态主体 || 左mi->动态主体 != 右mi->动态主体) return false;
        return 私有_时间段相交(左mi->开始时间, 左mi->结束时间, 右mi->开始时间, 右mi->结束时间);
    }

    动态节点类* 私有_创建空动态节点_() const
    {
        auto* n = new 动态节点类{};
        n->父 = n->子 = nullptr;
        n->上 = n->下 = nullptr;
        n->根 = nullptr;
        n->链上 = n->链下 = nullptr;
        n->子节点数量 = 0;
        return n;
    }
    动态节点类* 私有_构造聚合动态节点_(
        const std::vector<动态节点类*>& 子动态序列,
        时间戳 now = 0,
        bool 仅保留代表状态 = true,
        std::size_t 最大保留状态数 = 4) const
    {
        if (子动态序列.empty()) return nullptr;

        动态节点类* 首动态 = nullptr;
        动态节点类* 末动态 = nullptr;
        动态节点主信息类* 首mi = nullptr;
        动态节点主信息类* 末mi = nullptr;
        for (auto* d : 子动态序列) {
            auto* mi = 私有_取动态主信息(d);
            if (!mi) continue;
            if (!首动态) {
                首动态 = d;
                首mi = mi;
            }
            末动态 = d;
            末mi = mi;
        }
        if (!首动态 || !首mi || !末动态 || !末mi) return nullptr;

        auto* 父动态 = 私有_创建空动态节点_();
        auto* 父mi = new 动态节点主信息类();
        父mi->初始状态 = 首mi->初始状态;
        父mi->结果状态 = 末mi->结果状态;
        父mi->动态主体 = 首mi->动态主体;
        父mi->动态特征 = 首mi->动态特征;
        父mi->开始时间 = 首mi->开始时间;
        父mi->结束时间 = 末mi->结束时间 ? 末mi->结束时间 : now;

        std::vector<状态节点类*> 全事件;
        std::vector<状态节点类*> 状态路径;
        for (auto* 子动态 : 子动态序列) {
            auto* 子mi = 私有_取动态主信息(子动态);
            if (!子mi) continue;
            if (子mi->动态主体 != 父mi->动态主体) 父mi->动态主体 = nullptr;
            if (子mi->动态特征 != 父mi->动态特征) 父mi->动态特征 = nullptr;
            if (父mi->开始时间 == 0 || (子mi->开始时间 != 0 && 子mi->开始时间 < 父mi->开始时间)) 父mi->开始时间 = 子mi->开始时间;
            if (子mi->结束时间 > 父mi->结束时间) 父mi->结束时间 = 子mi->结束时间;
            私有_挂接子动态(父动态, 子动态);
            私有_收集动态事件(子动态, 全事件);
            私有_收集动态路径(子动态, 状态路径);
        }

        父mi->事件列表 = 仅保留代表状态 ? 私有_提取代表事件(全事件, 最大保留状态数) : 全事件;
        父mi->状态路径列表 = 仅保留代表状态 ? 私有_提取代表事件(状态路径, 最大保留状态数) : 状态路径;
        if (!父mi->状态路径列表.empty()) {
            父mi->初始状态 = 父mi->状态路径列表.front();
            父mi->结果状态 = 父mi->状态路径列表.back();
        } else if (!父mi->事件列表.empty()) {
            父mi->初始状态 = 父mi->事件列表.front();
            父mi->结果状态 = 父mi->事件列表.back();
        }
        父mi->动态路径签名 = 私有_状态路径签名(父mi->状态路径列表.empty() ? 父mi->事件列表 : 父mi->状态路径列表);

        父动态->主信息 = 父mi;
        return 父动态;
    }

    // 把 seq 写入动态树（若已存在则复用）
    动态节点类* 私有_写入动态树_序列(const std::vector<std::uint64_t>& seq, int repeat, 时间戳 now)
    {
        if (seq.empty()) return nullptr;
        if (!动态树根_) return nullptr;

        const std::uint64_t fp = 私有_FNV64(seq.data(), seq.size() * sizeof(std::uint64_t));

        // 1) 快路径：运行期索引
        if (auto it = 提炼索引_.find(fp); it != 提炼索引_.end()) {
            return it->second;
        }

        // 2) 写入世界链：创建 child 节点，挂到 动态树根_ 之下
        动态节点类* child = nullptr;
        {
            锁调度器守卫 lk({
                锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链.链表锁", "动态类::私有_写入动态树_序列")
                });

            auto* mi = new 提炼动态主信息类();
          //  mi->主信息类型 = 枚举_主信息类型::动态;
            mi->指纹 = fp;
            mi->序列 = seq;
            mi->重复次数 = repeat;
            mi->创建时间 = now;

            // 可选：设定 初始/结果状态（这里用 nullptr；若你未来希望回溯，可在此设置）
            mi->初始状态 = nullptr;
            mi->结果状态 = nullptr;

            child = 世界链.添加子节点_已加锁(动态树根_, (基础信息基类*)mi);
        }

        if (child) {
            提炼索引_.emplace(fp, child);
        }
        return child;
    }

public:
    // ============================================================
    // 动态树根初始化（入世界链）
    // - 父节点为 nullptr 时，挂到 世界链.根指针 下
    // ============================================================
    动态节点类* 初始化动态树根(
        动态节点类* 父节点,
        const 词性节点类* 名称,
        const 词性节点类* 类型,
        时间戳 /*now*/ = 0)
    {
        if (动态树根_) return 动态树根_;

        动态节点类* parent = 父节点 ? 父节点 : 世界链.根指针;

        锁调度器守卫 lk({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链.链表锁", "动态类::初始化动态树根")
            });

        auto* mi = new 动态节点主信息类(名称, 类型);
     //   mi->主信息类型 = 枚举_主信息类型::动态;
        mi->初始状态 = nullptr;
        mi->结果状态 = nullptr;

        动态树根_ = 世界链.添加子节点_已加锁(parent, (基础信息基类*)mi);
        return 动态树根_;
    }

    动态节点类* 获取动态树根() const noexcept { return 动态树根_; }
    void 设置动态树根(动态节点类* root) noexcept { 动态树根_ = root; }

    // ============================================================
    // 创建叶子动态（不入世界链）
    // - 由相邻两个状态组成最基础的动态信息节点
    // - 以事件列表缓存该动态涉及的状态事件
    // ============================================================
    动态节点类* 创建叶子动态(
        场景节点类* 场景,
        状态节点类* 初始状态,
        状态节点类* 结果状态,
        时间戳 now = 0)
    {
        if (!场景 || !场景->主信息) return nullptr;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!smi) return nullptr;
        if (!私有_可形成基础动态(初始状态, 结果状态)) return nullptr;

        auto* 初始mi = 私有_取状态主信息(初始状态);
        auto* 结果mi = 私有_取状态主信息(结果状态);
        if (!初始mi || !结果mi) return nullptr;

        if (now == 0) now = 私有_取状态时间(结果状态);

        auto* n = 私有_创建空动态节点_();
        auto* mi = new 动态节点主信息类();
        mi->初始状态 = (基础信息节点类*)初始状态;
        mi->结果状态 = (基础信息节点类*)结果状态;
        mi->动态主体 = 结果mi->状态主体;
        mi->动态特征 = 结果mi->状态特征;
        mi->开始时间 = 私有_取状态时间(初始状态);
        mi->结束时间 = 私有_取状态时间(结果状态);
        if (mi->结束时间 == 0) mi->结束时间 = now;
        if (mi->开始时间 == 0) mi->开始时间 = mi->结束时间;
        私有_追加唯一事件(mi->事件列表, 初始状态);
        私有_追加唯一事件(mi->事件列表, 结果状态);
        私有_追加唯一状态路径(mi->状态路径列表, 初始状态);
        私有_追加唯一状态路径(mi->状态路径列表, 结果状态);
        mi->动态路径签名 = 私有_状态路径签名(mi->状态路径列表);
        n->主信息 = mi;

        smi->添加动态(n);
        私有_记录原始动态事件(场景, n, now ? now : mi->结束时间);
        return n;
    }

    状态节点类* 查找相邻前状态(
        场景节点类* 场景,
        状态节点类* 新状态) const
    {
        if (!场景 || !场景->主信息 || !新状态) return nullptr;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!smi) return nullptr;

        for (auto it = smi->状态列表.rbegin(); it != smi->状态列表.rend(); ++it) {
            auto* 旧状态 = *it;
            if (!旧状态 || 旧状态 == 新状态) continue;
            if (私有_可形成基础动态(旧状态, 新状态)) return 旧状态;
        }
        return nullptr;
    }
    动态节点类* 聚合连续动态节点(
        场景节点类* 场景,
        int 起始索引,
        int 数量,
        时间戳 now = 0,
        bool 仅保留代表状态 = true,
        std::size_t 最大保留状态数 = 4)
    {
        if (!场景 || !场景->主信息 || 起始索引 < 0 || 数量 < 2) return nullptr;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!smi) return nullptr;
        if ((std::size_t)起始索引 >= smi->动态列表.size()) return nullptr;

        const std::size_t start = (std::size_t)起始索引;
        const std::size_t endExclusive = start + (std::size_t)数量;
        if (endExclusive > smi->动态列表.size()) return nullptr;

        std::vector<动态节点类*> 子动态序列;
        子动态序列.reserve((std::size_t)数量);
        for (std::size_t i = start; i < endExclusive; ++i) {
            auto* d = smi->动态列表[i];
            if (!d || !私有_取动态主信息(d)) return nullptr;
            子动态序列.push_back(d);
        }

        auto* 父动态 = 私有_构造聚合动态节点_(子动态序列, now, 仅保留代表状态, 最大保留状态数);
        if (!父动态) return nullptr;

        std::vector<动态节点类*> out;
        out.reserve(smi->动态列表.size() - (std::size_t)数量 + 1);
        for (std::size_t i = 0; i < start; ++i) out.push_back(smi->动态列表[i]);
        out.push_back(父动态);
        for (std::size_t i = endExclusive; i < smi->动态列表.size(); ++i) out.push_back(smi->动态列表[i]);
        smi->动态列表 = std::move(out);
        return 父动态;
    }

    动态节点类* 尝试聚合同主体同时段动态(
        场景节点类* 场景,
        动态节点类* 新动态,
        时间戳 now = 0)
    {
        if (!场景 || !场景->主信息 || !新动态) return nullptr;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        auto* 新mi = (新动态->主信息) ? dynamic_cast<动态节点主信息类*>(新动态->主信息) : nullptr;
        if (!smi || !新mi || !新mi->动态主体) return nullptr;

        int idx = -1;
        for (int i = (int)smi->动态列表.size() - 1; i >= 0; --i) {
            if (smi->动态列表[(std::size_t)i] == 新动态) { idx = i; break; }
        }
        if (idx < 0) return nullptr;

        时间戳 聚合开始 = 新mi->开始时间;
        时间戳 聚合结束 = 新mi->结束时间;
        int start = idx;
        for (int i = idx - 1; i >= 0; --i) {
            auto* 候选 = smi->动态列表[(std::size_t)i];
            auto* 候选mi = (候选 && 候选->主信息) ? dynamic_cast<动态节点主信息类*>(候选->主信息) : nullptr;
            if (!候选mi || 候选mi->动态主体 != 新mi->动态主体) break;
            if (!私有_时间段相交(候选mi->开始时间, 候选mi->结束时间, 聚合开始, 聚合结束)) break;
            start = i;
            if (候选mi->开始时间 < 聚合开始) 聚合开始 = 候选mi->开始时间;
            if (候选mi->结束时间 > 聚合结束) 聚合结束 = 候选mi->结束时间;
        }

        if (start == idx) return nullptr;

        return 聚合连续动态节点(场景, start, idx - start + 1, now, true, 4);
    }

    动态节点类* 状态记录后刷新动态(
        场景节点类* 场景,
        状态节点类* 新状态,
        时间戳 now = 0,
        const std::string& /*调用点*/ = "动态类::状态记录后刷新动态")
    {
        if (!场景 || !新状态) return nullptr;
        auto* 前状态 = 查找相邻前状态(场景, 新状态);
        if (!前状态) return nullptr;

        auto* 叶子动态 = 创建叶子动态(场景, 前状态, 新状态, now);
        if (!叶子动态) return nullptr;

        if (auto* 父动态 = 尝试聚合同主体同时段动态(场景, 叶子动态, now)) {
            return 父动态;
        }
        return 叶子动态;
    }

    // ============================================================
    // 动态融合（从场景动态序列中提炼可重复片段 -> 写入动态树）
    // ============================================================
    动态节点类* 尝试融合并写入动态树(
        场景节点类* 场景,
        int minDynCountToFuse = 5,
        int minLen = 2,
        int minRepeat = 2,
        int maxLen = 16,
        时间戳 now = 0)
    {
        if (!场景 || !场景->主信息) return nullptr;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!smi) return nullptr;

        const int N = (int)smi->动态列表.size();
        if (N < minDynCountToFuse) return nullptr;
        if (minLen < 1) minLen = 1;
        if (minRepeat < 2) minRepeat = 2;
        if (maxLen < minLen) maxLen = minLen;

        // 1) 建符号序列
        std::vector<std::uint64_t> sym;
        sym.reserve((std::size_t)N);
        for (auto* d : smi->动态列表) sym.push_back(私有_叶子动态符号(d));

        // 2) 搜索重复片段
        struct 候选 {
            int len = 0;
            std::uint64_t hash = 0;
            std::vector<int> pos;   // 非重叠位置
            int score = 0;          // len * pos.size()
            int exemplar = 0;       // 用第一个位置作为样例
        } best;

        const int Lmax = std::min<int>(maxLen, N / minRepeat);
        for (int L = minLen; L <= Lmax; ++L) {
            std::unordered_map<std::uint64_t, std::vector<int>> occ;
            occ.reserve((std::size_t)N);

            for (int i = 0; i + L <= N; ++i) {
                const std::uint64_t h = 私有_FNV64(sym.data() + i, (std::size_t)L * sizeof(std::uint64_t));
                occ[h].push_back(i);
            }

            for (auto& kv : occ) {
                auto& vpos = kv.second;
                if ((int)vpos.size() < minRepeat) continue;

                // hash 冲突保护：只保留与 exemplar 真正相等的起点
                const int ex = vpos.front();
                std::vector<int> good;
                good.reserve(vpos.size());
                good.push_back(ex);
                for (std::size_t k = 1; k < vpos.size(); ++k) {
                    const int p = vpos[k];
                    if (私有_序列相等(sym, ex, sym, p, L)) good.push_back(p);
                }
                if ((int)good.size() < minRepeat) continue;

                auto nonOverlap = 私有_选择非重叠(std::move(good), L);
                if ((int)nonOverlap.size() < minRepeat) continue;

                const int score = L * (int)nonOverlap.size();
                if (score > best.score || (score == best.score && L > best.len)) {
                    best.len = L;
                    best.hash = kv.first;
                    best.pos = std::move(nonOverlap);
                    best.score = score;
                    best.exemplar = ex;
                }
            }
        }

        if (best.len <= 0 || (int)best.pos.size() < minRepeat) return nullptr;

        // 3) 取样例序列
        std::vector<std::uint64_t> seq;
        seq.reserve((std::size_t)best.len);
        for (int i = 0; i < best.len; ++i) seq.push_back(sym[(std::size_t)best.exemplar + (std::size_t)i]);

        // 4) 写入动态树（若已存在则复用）
        动态节点类* parent = 私有_写入动态树_序列(seq, (int)best.pos.size(), now);
        if (!parent) return nullptr;

        // 5) 用 parent 压缩场景动态序列
        std::vector<动态节点类*> out;
        out.reserve(smi->动态列表.size());

        std::size_t nextOcc = 0;
        int nextStart = (nextOcc < best.pos.size()) ? best.pos[nextOcc] : 1'000'000'000;

        for (int i = 0; i < N;) {
            if (i == nextStart) {
                bool ok = true;
                for (int k = 0; k < best.len; ++k) {
                    if (sym[(std::size_t)i + (std::size_t)k] != seq[(std::size_t)k]) { ok = false; break; }
                }
                if (ok) {
                    out.push_back(parent);
                    i += best.len;
                    nextOcc++;
                    nextStart = (nextOcc < best.pos.size()) ? best.pos[nextOcc] : 1'000'000'000;
                    continue;
                }
            }

            out.push_back(smi->动态列表[(std::size_t)i]);
            ++i;
        }

        smi->动态列表 = std::move(out);
        return parent;
    }
};

export 动态类 动态集;













