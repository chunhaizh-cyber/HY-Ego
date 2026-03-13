export module 因果信息模块;

import <cassert>;
import <vector>;
import <string>;
import <cstdint>;
import <chrono>;
import <unordered_map>;
import <algorithm>;

import 模板模块;
import 基础数据类型模块;
import 主信息定义模块;
import 通用函数模块;
import 状态模块;
import 动态模块;

export class 因果类 : public 链表模板<基础信息基类*> {
public:
    因果类() = default;
    ~因果类() = default;

    因果类(const 因果类&) = delete;
    因果类& operator=(const 因果类&) = delete;

private:
    基础信息节点类* 因果根_ = nullptr;
    std::unordered_map<std::string, 基础信息节点类*> 因果索引_{};

private:
    static 动态节点主信息类* 取动态主信息_(const 动态节点类* d) noexcept
    {
        return (d && d->主信息) ? dynamic_cast<动态节点主信息类*>(d->主信息) : nullptr;
    }

    static 状态节点主信息类* 取状态主信息_(const 状态节点类* s) noexcept
    {
        return (s && s->主信息) ? dynamic_cast<状态节点主信息类*>(s->主信息) : nullptr;
    }

    static 时间戳 当前时间_() noexcept
    {
        return static_cast<时间戳>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
    }

    static 时间戳 取状态时间_(const 状态节点类* s) noexcept
    {
        auto* mi = 取状态主信息_(s);
        if (!mi) return 0;
        return mi->发生时间 ? mi->发生时间 : mi->收到时间;
    }

    static void 追加唯一状态_(std::vector<状态节点类*>& out, 状态节点类* s)
    {
        if (!s) return;
        if (std::find(out.begin(), out.end(), s) != out.end()) return;
        out.push_back(s);
    }

    static void 追加唯一动态_(std::vector<动态节点类*>& out, 动态节点类* d)
    {
        if (!d) return;
        if (std::find(out.begin(), out.end(), d) != out.end()) return;
        out.push_back(d);
    }

    static void 收集动态状态列表_(const 动态节点类* d, std::vector<状态节点类*>& out)
    {
        auto* mi = 取动态主信息_(d);
        if (!mi) return;
        if (!mi->状态路径列表.empty()) {
            for (auto* s : mi->状态路径列表) 追加唯一状态_(out, s);
            return;
        }
        if (!mi->事件列表.empty()) {
            for (auto* s : mi->事件列表) 追加唯一状态_(out, s);
            return;
        }
        追加唯一状态_(out, static_cast<状态节点类*>(mi->初始状态));
        追加唯一状态_(out, static_cast<状态节点类*>(mi->结果状态));
    }

    static std::uint64_t 计算状态列表签名_(const std::vector<状态节点类*>& 状态列表) noexcept
    {
        if (状态列表.empty()) return 0ull;

        std::vector<std::uint64_t> pack;
        pack.reserve(状态列表.size() * 5);
        for (auto* s : 状态列表) {
            auto* mi = 取状态主信息_(s);
            pack.push_back((std::uint64_t)(std::uintptr_t)(mi ? mi->状态主体 : nullptr));
            pack.push_back((std::uint64_t)(std::uintptr_t)(mi ? mi->状态特征 : nullptr));
            pack.push_back((std::uint64_t)取状态事件(s));
            pack.push_back((std::uint64_t)(mi ? (mi->发生时间 ? mi->发生时间 : mi->收到时间) : 0));
            if (mi && std::holds_alternative<I64>(mi->状态值)) pack.push_back((std::uint64_t)std::get<I64>(mi->状态值));
            else pack.push_back((std::uint64_t)(std::uintptr_t)s);
        }
        return 通用函数模块::FNV1a64(pack.data(), pack.size() * sizeof(std::uint64_t));
    }

    static std::uint64_t 计算动态序列签名_(const std::vector<动态节点类*>& 动态序列) noexcept
    {
        if (动态序列.empty()) return 0ull;

        std::vector<std::uint64_t> pack;
        pack.reserve(动态序列.size() * 4);
        for (auto* d : 动态序列) {
            auto* mi = 取动态主信息_(d);
            pack.push_back((std::uint64_t)(std::uintptr_t)(mi ? mi->动态主体 : nullptr));
            pack.push_back((std::uint64_t)(std::uintptr_t)(mi ? mi->动态特征 : nullptr));
            pack.push_back((std::uint64_t)(mi ? mi->动态路径签名 : 0ull));
            pack.push_back((std::uint64_t)(mi ? mi->结束时间 : 0));
        }
        return 通用函数模块::FNV1a64(pack.data(), pack.size() * sizeof(std::uint64_t));
    }

    static 时间戳 取动态开始时间_(const 动态节点类* d) noexcept
    {
        auto* mi = 取动态主信息_(d);
        return mi ? mi->开始时间 : 0;
    }

    static 时间戳 取动态结束时间_(const 动态节点类* d) noexcept
    {
        auto* mi = 取动态主信息_(d);
        return mi ? mi->结束时间 : 0;
    }

    static std::int64_t 估计稳定度Q_(const 因果主信息类* mi) noexcept
    {
        if (!mi) return 0;
        const std::int64_t 次数分 = mi->观察次数 * 100;
        const std::int64_t 动作分 = mi->动作信息 ? 300 : 0;
        const std::int64_t 条件分 = mi->条件签名 != 0 ? 200 : 0;
        const std::int64_t 过程分 = mi->过程签名 != 0 ? 200 : 0;
        const std::int64_t 结果分 = mi->结果签名 != 0 ? 200 : 0;
        return 次数分 + 动作分 + 条件分 + 过程分 + 结果分;
    }

    static std::string 生成因果键_(基础信息节点类* 动作信息, std::uint64_t 条件签名, std::uint64_t 过程签名, std::uint64_t 结果签名) noexcept
    {
        return std::to_string(reinterpret_cast<std::uintptr_t>(动作信息)) + "|" +
            std::to_string(条件签名) + "|" + std::to_string(过程签名) + "->" + std::to_string(结果签名);
    }

    static std::string 生成动态同型键_(const 动态节点类* d) noexcept
    {
        auto* mi = 取动态主信息_(d);
        if (!mi) return {};
        return std::to_string(reinterpret_cast<std::uintptr_t>(mi->动态主体)) + "|" +
            std::to_string(reinterpret_cast<std::uintptr_t>(mi->动态特征)) + "|" +
            std::to_string(mi->动态路径签名);
    }

    static bool 动态同型_(const 动态节点类* 左, const 动态节点类* 右) noexcept
    {
        if (!左 || !右) return false;
        return 生成动态同型键_(左) == 生成动态同型键_(右);
    }

    static void 排序状态按时间_(std::vector<状态节点类*>& 列表)
    {
        std::sort(列表.begin(), 列表.end(), [](const 状态节点类* a, const 状态节点类* b) {
            return 取状态时间_(a) < 取状态时间_(b);
        });
    }

    static void 排序动态按时间_(std::vector<动态节点类*>& 列表)
    {
        std::sort(列表.begin(), 列表.end(), [](const 动态节点类* a, const 动态节点类* b) {
            const auto sa = 取动态开始时间_(a);
            const auto sb = 取动态开始时间_(b);
            if (sa != sb) return sa < sb;
            return 取动态结束时间_(a) < 取动态结束时间_(b);
        });
    }

public:
    基础信息节点类* 初始化因果根(
        基础信息节点类* 父节点 = nullptr,
        const 词性节点类* 名称 = nullptr,
        const 词性节点类* 类型 = nullptr)
    {
        if (因果根_) return 因果根_;
        auto* p = 父节点 ? 父节点 : 根指针;
        auto* mi = new 因果主信息类(名称, 类型);
        因果根_ = 添加子节点(p, static_cast<基础信息基类*>(mi));
        return 因果根_;
    }

    基础信息节点类* 获取因果根() const noexcept { return 因果根_; }

    基础信息节点类* 创建或累计因果信息(
        const std::vector<状态节点类*>& 条件状态列表,
        基础信息节点类* 动作信息,
        const std::vector<动态节点类*>& 过程动态列表,
        const std::vector<状态节点类*>& 结果状态列表,
        动态节点类* 因动态 = nullptr,
        动态节点类* 果动态 = nullptr,
        时间戳 now = 0,
        const std::string& /*调用点*/ = "因果类::创建或累计因果信息")
    {
        if (条件状态列表.empty() || 结果状态列表.empty() || 过程动态列表.empty() || !动作信息) return nullptr;
        if (!因果根_) 初始化因果根();
        if (now == 0) now = 当前时间_();

        const auto 条件签名 = 计算状态列表签名_(条件状态列表);
        const auto 过程签名 = 计算动态序列签名_(过程动态列表);
        const auto 结果签名 = 计算状态列表签名_(结果状态列表);
        if (条件签名 == 0 || 过程签名 == 0 || 结果签名 == 0) return nullptr;

        const auto key = 生成因果键_(动作信息, 条件签名, 过程签名, 结果签名);
        auto it = 因果索引_.find(key);
        if (it != 因果索引_.end()) {
            auto* node = it->second;
            auto* mi = node && node->主信息 ? dynamic_cast<因果主信息类*>(node->主信息) : nullptr;
            if (!mi) return node;
            mi->因信息 = 因动态;
            mi->果信息 = 果动态;
            mi->锚点动态 = 果动态 ? 果动态 : 因动态;
            mi->条件状态列表 = 条件状态列表;
            mi->动作信息 = 动作信息;
            mi->过程动态列表 = 过程动态列表;
            mi->过程签名 = 过程签名;
            mi->结果状态列表 = 结果状态列表;
            mi->因果主体 = 动作信息;
            mi->条件签名 = 条件签名;
            mi->结果签名 = 结果签名;
            if (mi->首次发生时间 == 0) mi->首次发生时间 = now;
            mi->最近发生时间 = now;
            mi->观察次数 += 1;
            mi->稳定度Q = 估计稳定度Q_(mi);
            return node;
        }

        auto* mi = new 因果主信息类();
        mi->因信息 = 因动态;
        mi->果信息 = 果动态;
        mi->锚点动态 = 果动态 ? 果动态 : 因动态;
        mi->条件状态列表 = 条件状态列表;
        mi->动作信息 = 动作信息;
        mi->过程动态列表 = 过程动态列表;
        mi->过程签名 = 过程签名;
        mi->结果状态列表 = 结果状态列表;
        mi->因果主体 = 动作信息;
        mi->条件签名 = 条件签名;
        mi->结果签名 = 结果签名;
        mi->首次发生时间 = now;
        mi->最近发生时间 = now;
        mi->观察次数 = 1;
        mi->稳定度Q = 估计稳定度Q_(mi);
        auto* node = 添加子节点(因果根_, static_cast<基础信息基类*>(mi));
        if (node) 因果索引_.emplace(key, node);
        return node;
    }

    std::vector<动态节点类*> 搜索同型动态(
        场景节点类* 场景,
        动态节点类* 锚点动态) const
    {
        std::vector<动态节点类*> out;
        if (!场景 || !场景->主信息 || !锚点动态) return out;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!smi) return out;

        for (auto* d : smi->动态列表) {
            if (!d) continue;
            if (!动态同型_(d, 锚点动态)) continue;
            out.push_back(d);
        }
        排序动态按时间_(out);
        return out;
    }

    void 根据锚点动态分段事件流(
        场景节点类* 场景,
        动态节点类* 锚点动态,
        std::vector<std::vector<状态节点类*>>& 事件分段组,
        std::vector<std::vector<动态节点类*>>& 动态分段组) const
    {
        事件分段组.clear();
        动态分段组.clear();
        if (!场景 || !场景->主信息 || !锚点动态) return;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!smi) return;

        auto 锚点组 = 搜索同型动态(场景, 锚点动态);
        if (锚点组.size() < 2) return;

        smi->清理空指针();
        const auto 最早事件时间 = [&]() -> 时间戳 {
            时间戳 t = 0;
            for (auto* s : smi->状态列表) {
                const auto ts = 取状态时间_(s);
                if (!ts) continue;
                if (t == 0 || ts < t) t = ts;
            }
            return t;
        }();

        for (std::size_t i = 0; i < 锚点组.size(); ++i) {
            const auto 段结束 = 取动态结束时间_(锚点组[i]);
            时间戳 段开始 = 最早事件时间;
            if (i > 0) {
                段开始 = 取动态结束时间_(锚点组[i - 1]);
            }
            std::vector<状态节点类*> 事件段;
            for (auto* s : smi->状态列表) {
                if (!s) continue;
                const auto ts = 取状态时间_(s);
                if (!ts) continue;
                if (ts < 段开始 || ts > 段结束) continue;
                事件段.push_back(s);
            }
            std::vector<动态节点类*> 动态段;
            for (auto* d : smi->动态列表) {
                if (!d) continue;
                const auto ds = 取动态开始时间_(d);
                const auto de = 取动态结束时间_(d);
                if (de < 段开始 || ds > 段结束) continue;
                动态段.push_back(d);
            }
            排序状态按时间_(事件段);
            排序动态按时间_(动态段);
            if (!事件段.empty() || !动态段.empty()) {
                事件分段组.push_back(std::move(事件段));
                动态分段组.push_back(std::move(动态段));
            }
        }
    }

    基础信息节点类* 从事件分段提炼初步因果信息(
        场景节点类* 场景,
        动态节点类* 锚点动态,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::从事件分段提炼初步因果信息",
        std::size_t 最少重复次数 = 2)
    {
        if (!场景 || !锚点动态) return nullptr;
        if (now == 0) now = 当前时间_();

        std::vector<std::vector<状态节点类*>> 事件分段组;
        std::vector<std::vector<动态节点类*>> 动态分段组;
        根据锚点动态分段事件流(场景, 锚点动态, 事件分段组, 动态分段组);
        if (事件分段组.size() < 最少重复次数 || 动态分段组.size() < 最少重复次数) return nullptr;

        std::unordered_map<std::string, std::size_t> 频次{};
        std::unordered_map<std::string, 动态节点类*> 代表动态{};
        const auto 锚点键 = 生成动态同型键_(锚点动态);
        for (const auto& 段 : 动态分段组) {
            std::vector<std::string> 本段已计{};
            for (auto* d : 段) {
                const auto key = 生成动态同型键_(d);
                if (key.empty()) continue;
                if (std::find(本段已计.begin(), 本段已计.end(), key) != 本段已计.end()) continue;
                本段已计.push_back(key);
                频次[key] += 1;
                if (!代表动态.contains(key)) 代表动态.emplace(key, d);
            }
        }

        基础信息节点类* 最后结果 = nullptr;
        for (std::size_t i = 0; i < 事件分段组.size(); ++i) {
            auto& 事件段 = 事件分段组[i];
            auto& 动态段 = 动态分段组[i];
            if (动态段.empty()) continue;

            std::vector<动态节点类*> 过程动态列表;
            for (auto* d : 动态段) {
                const auto key = 生成动态同型键_(d);
                if (key.empty()) continue;
                if (key != 锚点键 && 频次[key] < 最少重复次数) continue;
                追加唯一动态_(过程动态列表, d);
            }
            if (过程动态列表.empty()) 追加唯一动态_(过程动态列表, 锚点动态);
            排序动态按时间_(过程动态列表);

            const auto 过程开始 = 取动态开始时间_(过程动态列表.front());
            const auto 过程结束 = 取动态结束时间_(过程动态列表.back());
            std::vector<状态节点类*> 条件状态列表;
            std::vector<状态节点类*> 结果状态列表;
            for (auto* s : 事件段) {
                const auto ts = 取状态时间_(s);
                if (!ts) continue;
                if (ts <= 过程开始) 追加唯一状态_(条件状态列表, s);
                if (ts >= 过程结束) 追加唯一状态_(结果状态列表, s);
            }
            if (条件状态列表.empty()) 收集动态状态列表_(过程动态列表.front(), 条件状态列表);
            if (结果状态列表.empty()) 收集动态状态列表_(过程动态列表.back(), 结果状态列表);
            if (条件状态列表.empty() || 结果状态列表.empty()) continue;

            auto* 首mi = 取动态主信息_(过程动态列表.front());
            auto* 末mi = 取动态主信息_(过程动态列表.back());
            auto* 动作信息 = 首mi && 首mi->动态主体 ? 首mi->动态主体 : (末mi ? 末mi->动态主体 : nullptr);
            if (!动作信息) continue;

            auto* node = 创建或累计因果信息(
                条件状态列表,
                动作信息,
                过程动态列表,
                结果状态列表,
                过程动态列表.front(),
                过程动态列表.back(),
                now,
                调用点);
            auto* mi = node && node->主信息 ? dynamic_cast<因果主信息类*>(node->主信息) : nullptr;
            if (!mi) continue;
            mi->锚点动态 = 锚点动态;
            mi->事件分段列表 = 事件段;
            mi->分段开始时间 = 事件段.empty() ? 0 : 取状态时间_(事件段.front());
            mi->分段结束时间 = 事件段.empty() ? 0 : 取状态时间_(事件段.back());
            最后结果 = node;
        }
        return 最后结果;
    }

    基础信息节点类* 从动态序列创建或累计因果信息(
        const std::vector<动态节点类*>& 相关动态序列,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::从动态序列创建或累计因果信息")
    {
        if (相关动态序列.size() < 2) return nullptr;

        auto* 首mi = 取动态主信息_(相关动态序列.front());
        auto* 末mi = 取动态主信息_(相关动态序列.back());
        if (!首mi || !末mi) return nullptr;

        std::vector<状态节点类*> 条件状态列表;
        std::vector<状态节点类*> 结果状态列表;
        收集动态状态列表_(相关动态序列.front(), 条件状态列表);
        收集动态状态列表_(相关动态序列.back(), 结果状态列表);
        if (条件状态列表.empty() || 结果状态列表.empty()) return nullptr;

        auto* 动作信息 = 首mi->动态主体 ? 首mi->动态主体 : 末mi->动态主体;
        if (!动作信息) return nullptr;

        auto* node = 创建或累计因果信息(
            条件状态列表,
            动作信息,
            相关动态序列,
            结果状态列表,
            相关动态序列.front(),
            相关动态序列.back(),
            now,
            调用点);
        auto* mi = node && node->主信息 ? dynamic_cast<因果主信息类*>(node->主信息) : nullptr;
        if (mi) {
            mi->锚点动态 = 相关动态序列.back();
            mi->事件分段列表.clear();
            for (auto* d : 相关动态序列) 收集动态状态列表_(d, mi->事件分段列表);
            排序状态按时间_(mi->事件分段列表);
            mi->分段开始时间 = mi->事件分段列表.empty() ? 0 : 取状态时间_(mi->事件分段列表.front());
            mi->分段结束时间 = mi->事件分段列表.empty() ? 0 : 取状态时间_(mi->事件分段列表.back());
        }
        return node;
    }

    基础信息节点类* 动态记录后刷新因果(
        场景节点类* 场景,
        动态节点类* 新动态,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::动态记录后刷新因果",
        std::size_t 最大序列长度 = 6)
    {
        if (!场景 || !场景->主信息 || !新动态) return nullptr;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        auto* 新mi = 取动态主信息_(新动态);
        if (!smi || !新mi || !新mi->动态主体) return nullptr;

        if (auto* node = 从事件分段提炼初步因果信息(场景, 新动态, now, 调用点 + "/事件分段")) {
            return node;
        }

        std::vector<动态节点类*> 相关动态;
        for (auto* d : smi->动态列表) {
            if (!d) continue;
            auto* mi = 取动态主信息_(d);
            if (!mi) continue;
            if (mi->动态主体 != 新mi->动态主体) continue;
            if (取动态结束时间_(d) > 取动态结束时间_(新动态)) continue;
            相关动态.push_back(d);
        }
        if (相关动态.size() < 2) return nullptr;

        基础信息节点类* 最后结果 = nullptr;
        const std::size_t maxLen = (std::min)(最大序列长度, 相关动态.size());
        for (std::size_t len = 2; len <= maxLen; ++len) {
            std::vector<动态节点类*> 序列(相关动态.end() - (std::ptrdiff_t)len, 相关动态.end());
            if (auto* node = 从动态序列创建或累计因果信息(序列, now, 调用点 + "/动态序列")) {
                最后结果 = node;
            }
        }
        return 最后结果;
    }
};

export inline 因果类 因果集{};

