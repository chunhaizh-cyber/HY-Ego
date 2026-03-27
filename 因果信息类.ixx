module;
#include <cassert>
#include <vector>
#include <deque>
#include <string>
#include <cstdint>
#include <chrono>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <variant>
#include <fstream>
#include <limits>
#include <type_traits>

export module 因果信息模块;


import 模板模块;
import 基础数据类型模块;
import 数据仓库模块;
import 主信息定义模块;
import 内部方法总接口;
import 通用函数模块;
import 状态模块;
import 动态模块;
import 语素环境模块;

export class 因果类 : public 链表模板<基础信息基类*> {
public:
    因果类() = default;
    ~因果类() = default;

    因果类(const 因果类&) = delete;
    因果类& operator=(const 因果类&) = delete;

    struct 结构_因果草案 {
        场景节点类* 场景 = nullptr;
        std::vector<状态节点类*> 条件状态列表{};
        std::vector<动态节点类*> 过程动态列表{};
        std::vector<状态节点类*> 结果状态列表{};
        动态节点类* 因动态 = nullptr;
        动态节点类* 果动态 = nullptr;
        基础信息节点类* 动作主体 = nullptr;
        基础信息节点类* 动作语义 = nullptr;
    };

    struct 结构_因果链草案 {
        场景节点类* 场景 = nullptr;
        枚举_因果校验结果 失败原因 = 枚举_因果校验结果::合法;
        枚举_因果锚点类型 推断锚点类型 = 枚举_因果锚点类型::未定义;
        std::uint64_t 推断锚点主键 = 0;
        std::uint32_t 条件层级 = 0;
        std::uint32_t 结果层级 = 0;
        时间戳 记录时间 = 0;
        std::vector<状态节点类*> 条件状态列表{};
        std::vector<动态节点类*> 过程动态列表{};
        std::vector<状态节点类*> 结果状态列表{};
        动态节点类* 因动态 = nullptr;
        动态节点类* 果动态 = nullptr;
        基础信息节点类* 动作主体 = nullptr;
        基础信息节点类* 动作语义 = nullptr;
    };

private:
    enum class 枚举_因果生成通道 : std::uint8_t {
        未定义 = 0,
        场景 = 1,
        存在 = 2
    };

    基础信息节点类* 因果根_ = nullptr;
    std::unordered_map<std::string, 基础信息节点类*> 因果索引_{};
    std::unordered_map<std::string, std::vector<基础信息节点类*>> 因果索引_结果特征_{};
    std::unordered_map<std::string, std::vector<基础信息节点类*>> 因果索引_动作主体_{};
    std::unordered_map<std::string, std::vector<基础信息节点类*>> 因果索引_动作语义_{};
    std::unordered_map<std::string, std::vector<基础信息节点类*>> 因果索引_来源方法_{};
    std::unordered_map<std::string, std::vector<基础信息节点类*>> 因果索引_来源任务_{};
    std::unordered_map<std::string, 基础信息节点类*> 抽象因果索引_{};
    std::unordered_map<std::string, std::vector<基础信息节点类*>> 抽象因果索引_结果特征_{};
    std::unordered_map<std::string, std::vector<基础信息节点类*>> 抽象因果索引_动作模板_{};
    std::vector<结构_因果链草案> 跨层因果链草案队列_{};
    static constexpr std::uint32_t 因果存档魔数_ = 0x53554143u; // 'CAUS'

private:
    template<class T>
    static inline void 写POD_(std::ostream& os, const T& v) noexcept
    {
        static_assert(std::is_trivially_copyable_v<T>);
        os.write(reinterpret_cast<const char*>(&v), sizeof(T));
    }

    template<class T>
    static inline bool 读POD_(std::istream& is, T& v) noexcept
    {
        static_assert(std::is_trivially_copyable_v<T>);
        is.read(reinterpret_cast<char*>(&v), sizeof(T));
        return static_cast<bool>(is);
    }

    static inline void 写字符串_(std::ostream& os, const std::string& s) noexcept
    {
        const std::uint32_t n = static_cast<std::uint32_t>(s.size());
        写POD_(os, n);
        if (n) os.write(s.data(), n);
    }

    static inline bool 读字符串_(std::istream& is, std::string& out, std::uint32_t 上限 = 16u * 1024u * 1024u) noexcept
    {
        std::uint32_t n = 0;
        if (!读POD_(is, n)) return false;
        if (n > 上限) return false;
        out.assign(n, '\0');
        if (n) is.read(out.data(), n);
        return static_cast<bool>(is);
    }

    static inline std::uint64_t 索引或空_(
        const std::unordered_map<const 基础信息节点类*, std::uint64_t>& mp,
        const 基础信息节点类* p) noexcept
    {
        const std::uint64_t NIL = (std::numeric_limits<std::uint64_t>::max)();
        if (!p) return NIL;
        auto it = mp.find(p);
        return (it == mp.end()) ? NIL : it->second;
    }

    static inline 基础信息节点类* 由索引取指针_(
        const std::vector<基础信息节点类*>& vec,
        std::uint64_t idx) noexcept
    {
        const std::uint64_t NIL = (std::numeric_limits<std::uint64_t>::max)();
        if (idx == NIL || idx >= static_cast<std::uint64_t>(vec.size())) return nullptr;
        return vec[static_cast<std::size_t>(idx)];
    }

    static inline void 写字符串列表_(std::ostream& os, const std::vector<std::string>& 列表) noexcept
    {
        const std::uint32_t n = static_cast<std::uint32_t>(列表.size());
        写POD_(os, n);
        for (const auto& s : 列表) {
            写字符串_(os, s);
        }
    }

    static inline bool 读字符串列表_(std::istream& is, std::vector<std::string>& 列表) noexcept
    {
        std::uint32_t n = 0;
        if (!读POD_(is, n)) return false;
        列表.clear();
        列表.reserve(n);
        for (std::uint32_t i = 0; i < n; ++i) {
            std::string s{};
            if (!读字符串_(is, s)) return false;
            列表.push_back(std::move(s));
        }
        return true;
    }

    template<class TNode>
    static std::vector<std::string> 节点列表主键_(const std::vector<TNode*>& 列表)
    {
        std::vector<std::string> out{};
        out.reserve(列表.size());
        for (auto* 节点 : 列表) {
            out.push_back(节点 ? 节点->获取主键() : std::string{});
        }
        return out;
    }

    static std::string 节点主键_(const 基础信息节点类* 节点) noexcept
    {
        return 节点 ? 节点->获取主键() : std::string{};
    }

    static std::uint64_t 解析运行期主键_(const std::string& 主键文本) noexcept
    {
        if (主键文本.empty()) return 0;
        if (主键文本.rfind("hash:", 0) == 0) {
            try {
                return static_cast<std::uint64_t>(std::stoull(主键文本.substr(5)));
            }
            catch (...) {
                return 0;
            }
        }
        return static_cast<std::uint64_t>(std::hash<std::string>{}(主键文本));
    }

    static 枚举_因果结构类型 识别因果结构类型_(const 基础信息基类* 主信息) noexcept
    {
        if (auto* mi = dynamic_cast<const 因果兼容主信息类*>(主信息)) return mi->结构类型;
        if (dynamic_cast<const 因果模板主信息类*>(主信息)) return 枚举_因果结构类型::模板;
        if (dynamic_cast<const 因果链主信息类*>(主信息)) return 枚举_因果结构类型::链;
        if (dynamic_cast<const 因果实例主信息类*>(主信息)) return 枚举_因果结构类型::实例;
        return 枚举_因果结构类型::未定义;
    }

    static bool 写入因果主信息_(std::ostream& os, const 基础信息基类* 主信息) noexcept
    {
        const auto kind = 识别因果结构类型_(主信息);
        写POD_(os, static_cast<std::uint8_t>(kind));

        switch (kind)
        {
        case 枚举_因果结构类型::模板: {
            auto* mi = dynamic_cast<const 因果模板主信息类*>(主信息);
            if (!mi) return false;
            写POD_(os, static_cast<std::uint8_t>(mi->适用锚点类型));
            写POD_(os, mi->适用层级);
            写POD_(os, mi->适用状态面签名);
            写字符串列表_(os, mi->条件模板主键列表_存档.empty() ? 节点列表主键_(mi->条件模板列表) : mi->条件模板主键列表_存档);
            写字符串_(os, mi->动作模板主键_存档.empty() ? 节点主键_(mi->动作模板) : mi->动作模板主键_存档);
            写字符串列表_(os, mi->结果模板主键列表_存档.empty() ? 节点列表主键_(mi->结果模板列表) : mi->结果模板主键列表_存档);
            写字符串列表_(os, mi->证据实例主键列表_存档.empty() ? 节点列表主键_(mi->证据实例列表) : mi->证据实例主键列表_存档);
            写字符串列表_(os, mi->代表证据主键列表_存档.empty() ? 节点列表主键_(mi->代表证据列表) : mi->代表证据主键列表_存档);
            写字符串列表_(os, mi->反例主键列表_存档.empty() ? 节点列表主键_(mi->反例实例列表) : mi->反例主键列表_存档);
            写POD_(os, mi->证据次数);
            写POD_(os, mi->成立次数);
            写POD_(os, mi->失败次数);
            写POD_(os, mi->最近命中时间);
            写POD_(os, mi->最近失败时间);
            写POD_(os, mi->稳定度);
            写字符串_(os, mi->典型条件方向摘要);
            写字符串_(os, mi->典型动作语义摘要);
            写字符串_(os, mi->典型结果方向摘要);
            写字符串_(os, mi->最常见失败边界摘要);
            return static_cast<bool>(os);
        }
        case 枚举_因果结构类型::链: {
            auto* mi = dynamic_cast<const 因果链主信息类*>(主信息);
            if (!mi) return false;
            写字符串列表_(os, mi->因果实例主键序列_存档.empty() ? 节点列表主键_(mi->因果实例序列) : mi->因果实例主键序列_存档);
            写字符串_(os, mi->起点锚点主键_存档);
            写字符串_(os, mi->终点锚点主键_存档);
            写POD_(os, mi->起始时间);
            写POD_(os, mi->结束时间);
            return static_cast<bool>(os);
        }
        case 枚举_因果结构类型::未定义:
        case 枚举_因果结构类型::实例:
        default: {
            auto* mi = dynamic_cast<const 因果实例主信息类*>(主信息);
            auto* 兼容 = dynamic_cast<const 因果兼容主信息类*>(主信息);
            if (!mi) return false;
            写POD_(os, static_cast<std::uint8_t>(mi->锚点类型));
            写字符串_(os, mi->锚点主键_存档);
            写POD_(os, mi->状态层级);
            写POD_(os, mi->状态面签名);
            写POD_(os, mi->条件时间);
            写POD_(os, mi->动作时间);
            写POD_(os, mi->结果时间);
            写字符串列表_(os, mi->条件状态主键列表_存档.empty() ? 节点列表主键_(mi->条件状态列表) : mi->条件状态主键列表_存档);
            写字符串_(os, mi->动作主体主键_存档.empty() ? 节点主键_(mi->动作主体) : mi->动作主体主键_存档);
            写字符串_(os, mi->动作语义主键_存档.empty() ? 节点主键_(mi->动作语义) : mi->动作语义主键_存档);
            写字符串_(os, mi->动作语义键);
            写字符串_(os, mi->触发动态主键_存档.empty() ? 节点主键_(mi->触发动态) : mi->触发动态主键_存档);
            写字符串_(os, mi->主结果状态主键_存档.empty() ? 节点主键_(mi->主结果状态) : mi->主结果状态主键_存档);
            写字符串_(os, mi->主结果动态主键_存档.empty() ? 节点主键_(mi->主结果动态) : mi->主结果动态主键_存档);
            写字符串列表_(os, mi->结果状态主键列表_存档.empty() ? 节点列表主键_(mi->结果状态列表) : mi->结果状态主键列表_存档);
            写字符串列表_(os, mi->结果动态主键列表_存档.empty() ? 节点列表主键_(mi->结果动态列表) : mi->结果动态主键列表_存档);
            写POD_(os, static_cast<std::uint8_t>(mi->来源类型));
            写字符串_(os, mi->来源方法主键_存档);
            写字符串_(os, mi->来源任务主键_存档);
            写POD_(os, mi->置信度);
            const std::uint8_t 已验证 = mi->已验证 ? 1u : 0u;
            写POD_(os, 已验证);

            const std::string 累计键 = 兼容 ? 兼容->因果累计键_存档 : std::string{};
            写字符串_(os, 累计键);
            写POD_(os, 兼容 ? 兼容->首次发生时间 : 0);
            写POD_(os, 兼容 ? 兼容->最近发生时间 : 0);
            写POD_(os, 兼容 ? 兼容->观察次数 : 0ll);
            写POD_(os, 兼容 ? 兼容->稳定度Q : 0ll);
            return static_cast<bool>(os);
        }
        }
    }

    static 基础信息基类* 读取因果主信息_(std::istream& is, std::uint16_t 存档版本) noexcept
    {
        std::uint8_t kindRaw = 0;
        if (!读POD_(is, kindRaw)) return nullptr;
        const auto kind = static_cast<枚举_因果结构类型>(kindRaw);

        switch (kind)
        {
        case 枚举_因果结构类型::模板: {
            auto* mi = new 因果模板主信息类();
            if (!读POD_(is, kindRaw)) { delete mi; return nullptr; }
            mi->适用锚点类型 = static_cast<枚举_因果锚点类型>(kindRaw);
            if (!读POD_(is, mi->适用层级)) { delete mi; return nullptr; }
            if (!读POD_(is, mi->适用状态面签名)) { delete mi; return nullptr; }
            std::string 旧版主结果特征主键{};
            std::string 旧版抽象键{};
            if (存档版本 < 2) {
                if (!读字符串_(is, 旧版主结果特征主键)) { delete mi; return nullptr; }
            }
            if (!读字符串列表_(is, mi->条件模板主键列表_存档)) { delete mi; return nullptr; }
            if (!读字符串_(is, mi->动作模板主键_存档)) { delete mi; return nullptr; }
            if (!读字符串列表_(is, mi->结果模板主键列表_存档)) { delete mi; return nullptr; }
            if (!读字符串列表_(is, mi->证据实例主键列表_存档)) { delete mi; return nullptr; }
            if (存档版本 >= 2) {
                if (!读字符串列表_(is, mi->代表证据主键列表_存档)) { delete mi; return nullptr; }
                if (!读字符串列表_(is, mi->反例主键列表_存档)) { delete mi; return nullptr; }
            }
            else {
                mi->代表证据主键列表_存档 = mi->证据实例主键列表_存档;
                if (!读字符串_(is, 旧版抽象键)) { delete mi; return nullptr; }
            }
            if (!读POD_(is, mi->证据次数)) { delete mi; return nullptr; }
            if (!读POD_(is, mi->成立次数)) { delete mi; return nullptr; }
            if (!读POD_(is, mi->失败次数)) { delete mi; return nullptr; }
            if (!读POD_(is, mi->最近命中时间)) { delete mi; return nullptr; }
            if (存档版本 >= 2) {
                if (!读POD_(is, mi->最近失败时间)) { delete mi; return nullptr; }
            }
            if (!读POD_(is, mi->稳定度)) { delete mi; return nullptr; }
            if (存档版本 >= 2) {
                if (!读字符串_(is, mi->典型条件方向摘要)) { delete mi; return nullptr; }
                if (!读字符串_(is, mi->典型动作语义摘要)) { delete mi; return nullptr; }
                if (!读字符串_(is, mi->典型结果方向摘要)) { delete mi; return nullptr; }
                if (!读字符串_(is, mi->最常见失败边界摘要)) { delete mi; return nullptr; }
            }
            mi->动作模板 = nullptr;
            (void)旧版主结果特征主键;
            (void)旧版抽象键;
            return mi;
        }
        case 枚举_因果结构类型::链: {
            auto* mi = new 因果链主信息类();
            if (!读字符串列表_(is, mi->因果实例主键序列_存档)) { delete mi; return nullptr; }
            if (!读字符串_(is, mi->起点锚点主键_存档)) { delete mi; return nullptr; }
            if (!读字符串_(is, mi->终点锚点主键_存档)) { delete mi; return nullptr; }
            if (!读POD_(is, mi->起始时间)) { delete mi; return nullptr; }
            if (!读POD_(is, mi->结束时间)) { delete mi; return nullptr; }
            mi->起点锚点主键 = 解析运行期主键_(mi->起点锚点主键_存档);
            mi->终点锚点主键 = 解析运行期主键_(mi->终点锚点主键_存档);
            return mi;
        }
        case 枚举_因果结构类型::未定义:
        case 枚举_因果结构类型::实例:
        default: {
            auto* mi = new 因果主信息类();
            mi->结构类型 = 枚举_因果结构类型::实例;

            if (!读POD_(is, kindRaw)) { delete mi; return nullptr; }
            mi->锚点类型 = static_cast<枚举_因果锚点类型>(kindRaw);
            if (!读字符串_(is, mi->锚点主键_存档)) { delete mi; return nullptr; }
            if (!读POD_(is, mi->状态层级)) { delete mi; return nullptr; }
            if (!读POD_(is, mi->状态面签名)) { delete mi; return nullptr; }
            if (!读POD_(is, mi->条件时间)) { delete mi; return nullptr; }
            if (!读POD_(is, mi->动作时间)) { delete mi; return nullptr; }
            if (!读POD_(is, mi->结果时间)) { delete mi; return nullptr; }
            if (!读字符串列表_(is, mi->条件状态主键列表_存档)) { delete mi; return nullptr; }
            if (!读字符串_(is, mi->动作主体主键_存档)) { delete mi; return nullptr; }
            if (!读字符串_(is, mi->动作语义主键_存档)) { delete mi; return nullptr; }
            if (!读字符串_(is, mi->动作语义键)) { delete mi; return nullptr; }
            if (!读字符串_(is, mi->触发动态主键_存档)) { delete mi; return nullptr; }
            if (!读字符串_(is, mi->主结果状态主键_存档)) { delete mi; return nullptr; }
            if (!读字符串_(is, mi->主结果动态主键_存档)) { delete mi; return nullptr; }
            if (!读字符串列表_(is, mi->结果状态主键列表_存档)) { delete mi; return nullptr; }
            if (!读字符串列表_(is, mi->结果动态主键列表_存档)) { delete mi; return nullptr; }
            if (!读POD_(is, kindRaw)) { delete mi; return nullptr; }
            mi->来源类型 = static_cast<枚举_因果来源类型>(kindRaw);
            if (!读字符串_(is, mi->来源方法主键_存档)) { delete mi; return nullptr; }
            if (!读字符串_(is, mi->来源任务主键_存档)) { delete mi; return nullptr; }
            if (!读POD_(is, mi->置信度)) { delete mi; return nullptr; }
            std::uint8_t 已验证 = 0;
            if (!读POD_(is, 已验证)) { delete mi; return nullptr; }
            if (!读字符串_(is, mi->因果累计键_存档)) { delete mi; return nullptr; }
            if (!读POD_(is, mi->首次发生时间)) { delete mi; return nullptr; }
            if (!读POD_(is, mi->最近发生时间)) { delete mi; return nullptr; }
            if (!读POD_(is, mi->观察次数)) { delete mi; return nullptr; }
            if (!读POD_(is, mi->稳定度Q)) { delete mi; return nullptr; }

            mi->锚点主键 = 解析运行期主键_(mi->锚点主键_存档);
            mi->来源方法主键 = 解析运行期主键_(mi->来源方法主键_存档);
            mi->来源任务主键 = 解析运行期主键_(mi->来源任务主键_存档);
            mi->已验证 = (已验证 != 0);
            mi->锚点信息 = nullptr;
            mi->动作主体 = nullptr;
            mi->动作语义 = nullptr;
            mi->动作信息 = nullptr;
            mi->动作动态 = nullptr;
            mi->触发动态 = nullptr;
            mi->主结果状态 = nullptr;
            mi->主结果动态 = nullptr;
            mi->归纳因果节点 = nullptr;
            mi->结果状态 = nullptr;
            mi->因信息 = nullptr;
            mi->果信息 = nullptr;
            mi->锚点动态 = nullptr;
            return mi;
        }
        }
    }

    static 动态节点主信息类* 取动态主信息_(const 动态节点类* d) noexcept
    {
        return (d && d->主信息) ? dynamic_cast<动态节点主信息类*>(d->主信息) : nullptr;
    }

    static 状态节点主信息类* 取状态主信息_(const 状态节点类* s) noexcept
    {
        return (s && s->主信息) ? dynamic_cast<状态节点主信息类*>(s->主信息) : nullptr;
    }

    static const 状态节点主信息类* 取状态主信息_(const 状态节点主信息类* s) noexcept
    {
        return s;
    }

    static 状态节点主信息类 复制状态主信息_(const 状态节点类* 状态) noexcept
    {
        auto* 主信息 = 取状态主信息_(状态);
        return 主信息 ? *主信息 : 状态节点主信息类{};
    }

    static void 填充状态主信息列表_(
        std::vector<状态节点主信息类>& 目标,
        const std::vector<状态节点类*>& 源) noexcept
    {
        目标.clear();
        目标.reserve(源.size());
        for (auto* 状态 : 源) {
            auto* 主信息 = 取状态主信息_(状态);
            if (!主信息) continue;
            目标.push_back(*主信息);
        }
    }

    static void 复制状态主信息列表_(
        std::vector<状态节点主信息类>& 目标,
        const std::vector<状态节点主信息类>& 源) noexcept
    {
        目标.clear();
        目标.reserve(源.size());
        for (const auto& 状态 : 源) {
            目标.push_back(状态);
        }
    }

    static std::string 生成状态主信息存档键_(const 状态节点主信息类& 状态) noexcept
    {
        if (!状态.状态主体 || !状态.状态特征) return {};
        return std::string{"subject="}
            + 状态.状态主体->获取主键()
            + "|feature="
            + 状态.状态特征->获取主键()
            + "|";
    }

    static void 填充状态主键列表_(
        std::vector<std::string>& 目标,
        const std::vector<状态节点主信息类>& 源) noexcept
    {
        目标.clear();
        目标.reserve(源.size());
        for (const auto& 状态 : 源) {
            const auto 键 = 生成状态主信息存档键_(状态);
            if (键.empty()) continue;
            if (std::find(目标.begin(), 目标.end(), 键) == 目标.end()) {
                目标.push_back(键);
            }
        }
    }

    static void 同步实例因果状态主信息缓存_(
        因果实例主信息类* mi) noexcept
    {
        if (!mi) return;

        if (mi->条件状态主信息列表.empty() && !mi->条件状态列表.empty()) {
            填充状态主信息列表_(mi->条件状态主信息列表, mi->条件状态列表);
        }
        if (mi->结果状态主信息列表.empty() && !mi->结果状态列表.empty()) {
            填充状态主信息列表_(mi->结果状态主信息列表, mi->结果状态列表);
        }
        if (mi->主结果状态主信息列表.empty()) {
            if (auto* 主结果主信息 = 取状态主信息_(mi->主结果状态)) {
                mi->主结果状态主信息列表.push_back(*主结果主信息);
            }
            else if (!mi->结果状态主信息列表.empty()) {
                mi->主结果状态主信息列表.push_back(mi->结果状态主信息列表.back());
            }
        }
    }

    template<class F>
    static void 遍历实例条件状态主信息_(
        const 因果实例主信息类& 实例,
        F&& 访问) noexcept
    {
        if (!实例.条件状态主信息列表.empty()) {
            for (const auto& 状态 : 实例.条件状态主信息列表) {
                访问(状态);
            }
            return;
        }
        for (auto* 状态 : 实例.条件状态列表) {
            if (auto* 主信息 = 取状态主信息_(状态)) {
                访问(*主信息);
            }
        }
    }

    template<class F>
    static void 遍历实例结果状态主信息_(
        const 因果实例主信息类& 实例,
        F&& 访问) noexcept
    {
        if (!实例.结果状态主信息列表.empty()) {
            for (const auto& 状态 : 实例.结果状态主信息列表) {
                访问(状态);
            }
            return;
        }
        for (auto* 状态 : 实例.结果状态列表) {
            if (auto* 主信息 = 取状态主信息_(状态)) {
                访问(*主信息);
            }
        }
    }

    static const 状态节点主信息类* 取实例主结果状态主信息_(
        const 因果实例主信息类& 实例) noexcept
    {
        if (!实例.主结果状态主信息列表.empty()) {
            return &实例.主结果状态主信息列表.back();
        }
        if (auto* 主信息 = 取状态主信息_(实例.主结果状态)) {
            return 主信息;
        }
        if (!实例.结果状态主信息列表.empty()) {
            return &实例.结果状态主信息列表.back();
        }
        if (!实例.结果状态列表.empty()) {
            return 取状态主信息_(实例.结果状态列表.back());
        }
        if (auto* 兼容 = dynamic_cast<const 因果兼容主信息类*>(&实例)) {
            return 取状态主信息_(兼容->结果状态);
        }
        return nullptr;
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

    static 时间戳 取状态时间_(const 状态节点主信息类* s) noexcept
    {
        if (!s) return 0;
        return s->发生时间 ? s->发生时间 : s->收到时间;
    }

    static 特征节点类* 取状态特征_(const 状态节点类* s) noexcept
    {
        auto* mi = 取状态主信息_(s);
        return mi ? mi->状态特征 : nullptr;
    }

    static 特征节点类* 取状态特征_(const 状态节点主信息类* s) noexcept
    {
        return s ? s->状态特征 : nullptr;
    }

    static std::string 取特征主键_(const 特征节点类* f) noexcept
    {
        return f ? f->获取主键() : std::string{};
    }

    static std::string 取特征文本_(const 特征节点类* f) noexcept
    {
        auto* fi = (f && f->主信息) ? dynamic_cast<特征节点主信息类*>(f->主信息) : nullptr;
        auto* 类型 = fi ? fi->类型 : nullptr;
        auto* 词信息 = (类型 && 类型->父 && 类型->父->主信息)
            ? dynamic_cast<词主信息类*>(类型->父->主信息)
            : nullptr;
        return 词信息 ? 词信息->词 : std::string{};
    }

    static std::string 取状态特征主键_(const 状态节点类* s) noexcept
    {
        return 取特征主键_(取状态特征_(s));
    }

    static std::string 取状态特征主键_(const 状态节点主信息类* s) noexcept
    {
        return 取特征主键_(取状态特征_(s));
    }

    static std::string 取状态特征文本_(const 状态节点类* s) noexcept
    {
        return 取特征文本_(取状态特征_(s));
    }

    static std::string 取状态特征文本_(const 状态节点主信息类* s) noexcept
    {
        return 取特征文本_(取状态特征_(s));
    }

    static std::string 取状态原因类别_(const 状态节点类* s) noexcept
    {
        auto* mi = 取状态主信息_(s);
        return mi ? mi->变化原因类别 : std::string{};
    }

    static std::string 取状态原因类别_(const 状态节点主信息类* s) noexcept
    {
        return s ? s->变化原因类别 : std::string{};
    }

    static std::uint64_t 计算文本签名值_(const std::string& 文本) noexcept
    {
        return 文本.empty() ? 0 : static_cast<std::uint64_t>(std::hash<std::string>{}(文本));
    }

    template<class NodeT>
    static std::string 生成节点稳定键_(const NodeT* node) noexcept
    {
        return std::to_string(计算节点运行期主键_(node));
    }

    static std::string 生成状态主体稳定键_(const 状态节点类* 状态) noexcept
    {
        auto* smi = 取状态主信息_(状态);
        return smi && smi->状态主体 ? 生成节点稳定键_(smi->状态主体) : std::string{"0"};
    }

    static std::string 生成状态主体稳定键_(const 状态节点主信息类* 状态) noexcept
    {
        return 状态 && 状态->状态主体 ? 生成节点稳定键_(状态->状态主体) : std::string{"0"};
    }

    static std::string 生成状态特征稳定键_(const 状态节点类* 状态) noexcept
    {
        return std::to_string(计算节点运行期主键_(取状态特征_(状态)));
    }

    static std::string 生成状态特征稳定键_(const 状态节点主信息类* 状态) noexcept
    {
        return std::to_string(计算节点运行期主键_(取状态特征_(状态)));
    }

    static std::string 生成状态域稳定键_(const 状态节点类* 状态) noexcept
    {
        auto* smi = 取状态主信息_(状态);
        return std::to_string(smi ? static_cast<std::uint32_t>(smi->状态域) : 0u);
    }

    static std::string 生成状态域稳定键_(const 状态节点主信息类* 状态) noexcept
    {
        return std::to_string(状态 ? static_cast<std::uint32_t>(状态->状态域) : 0u);
    }

    static std::string 生成状态规约键_(const 状态节点类* 状态) noexcept
    {
        if (!状态) return {};
        auto* smi = 取状态主信息_(状态);
        if (!smi || !smi->状态主体 || !smi->状态特征) return {};
        return std::string{"subject="} + 生成状态主体稳定键_(状态)
            + "|domain=" + 生成状态域稳定键_(状态)
            + "|feature=" + 生成状态特征稳定键_(状态);
    }

    static std::string 生成状态规约键_(const 状态节点主信息类* 状态) noexcept
    {
        if (!状态 || !状态->状态主体 || !状态->状态特征) return {};
        return std::string{"subject="} + 生成状态主体稳定键_(状态)
            + "|domain=" + 生成状态域稳定键_(状态)
            + "|feature=" + 生成状态特征稳定键_(状态);
    }

    static std::string 取动态特征主键_(const 动态节点类* d) noexcept
    {
        auto* mi = 取动态主信息_(d);
        return mi ? 取特征主键_(mi->动态特征) : std::string{};
    }

    static const 词性节点类* 特征_任务当前方法首节点指针_() noexcept
    {
        static const 词性节点类* type = 语素集.添加词性词("任务_当前方法首节点指针", "名词");
        return type;
    }

    static const 词性节点类* 特征_任务最近方法首节点指针_() noexcept
    {
        static const 词性节点类* type = 语素集.添加词性词("任务_最近方法首节点指针", "名词");
        return type;
    }

    static std::optional<std::uintptr_t> 读取任务执行账指针_已持有世界锁_(
        任务信息基类* 主信息,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        if (!主信息 || !主信息->任务虚拟存在 || !特征类型) return std::nullopt;
        const auto 目标主键 = 特征类型->获取主键();
        if (目标主键.empty() || !主信息->任务虚拟存在->子) return std::nullopt;

        auto* first = static_cast<基础信息节点类*>(主信息->任务虚拟存在->子);
        auto* it = first;
        do {
            auto* 特征信息 = it ? dynamic_cast<特征节点主信息类*>(it->主信息) : nullptr;
            if (特征信息 && 特征信息->类型 && 特征信息->类型->获取主键() == 目标主键) {
                if (std::holds_alternative<指针句柄>(特征信息->当前快照)) {
                    return std::get<指针句柄>(特征信息->当前快照).指针;
                }
                return std::nullopt;
            }
            it = it ? static_cast<基础信息节点类*>(it->下) : nullptr;
        } while (it && it != first);
        return std::nullopt;
    }

    static std::optional<std::uintptr_t> 读取任务执行账指针_(
        任务信息基类* 主信息,
        const 词性节点类* 特征类型,
        const std::string& 调用点) noexcept
    {
        数据仓库模块::锁调度器守卫 锁({
            数据仓库模块::锁请求::读(数据仓库模块::世界链.链表锁, 数据仓库模块::枚举_锁域::世界链, "世界链", 调用点)
        });
        return 读取任务执行账指针_已持有世界锁_(主信息, 特征类型, 调用点);
    }

    static bool 是否自我核心特征文本_(const std::string& 文本) noexcept
    {
        return 文本 == "安全值" || 文本 == "服务值";
    }

    static constexpr std::uint64_t 状态面_未定义_ = 0ull;
    static constexpr std::uint64_t 状态面_几何_ = 0x1001ull;
    static constexpr std::uint64_t 状态面_能量_ = 0x1002ull;
    static constexpr std::uint64_t 状态面_功能_ = 0x1003ull;
    static constexpr std::uint64_t 状态面_评价_ = 0x1004ull;
    static constexpr std::uint64_t 状态面_任务_ = 0x1005ull;

    static bool 文本包含_(const std::string& 文本, const char* 关键词) noexcept
    {
        return 关键词 && !文本.empty() && 文本.find(关键词) != std::string::npos;
    }

    static std::uint64_t 推断状态面签名_按标签_(const std::string& 标签) noexcept
    {
        if (标签.empty()) return 状态面_未定义_;

        if (文本包含_(标签, "安全值")
            || 文本包含_(标签, "服务值")
            || 文本包含_(标签, "情绪")
            || 文本包含_(标签, "评估_")
            || 文本包含_(标签, "置信度")
            || 文本包含_(标签, "信任度")
            || 文本包含_(标签, "质量分")) {
            return 状态面_评价_;
        }

        if (文本包含_(标签, "任务_")
            || 文本包含_(标签, "需求_")) {
            return 状态面_任务_;
        }

        if (文本包含_(标签, "电量")
            || 文本包含_(标签, "电压")
            || 文本包含_(标签, "电流")
            || 文本包含_(标签, "功耗")
            || 文本包含_(标签, "功率")
            || 文本包含_(标签, "温度")
            || 文本包含_(标签, "扭矩")
            || 文本包含_(标签, "负载")
            || 文本包含_(标签, "能量")) {
            return 状态面_能量_;
        }

        if (文本包含_(标签, "位置")
            || 文本包含_(标签, "距离")
            || 文本包含_(标签, "尺寸")
            || 文本包含_(标签, "方向")
            || 文本包含_(标签, "接触")
            || 文本包含_(标签, "脱离")
            || 文本包含_(标签, "遮挡")
            || 文本包含_(标签, "高度")
            || 文本包含_(标签, "轮廓")
            || 文本包含_(标签, "体素")
            || 文本包含_(标签, "可见")) {
            return 状态面_几何_;
        }

        if (文本包含_(标签, "方法_")
            || 文本包含_(标签, "节点_")
            || 文本包含_(标签, "首节点_")
            || 文本包含_(标签, "条件_")
            || 文本包含_(标签, "结果_")
            || 文本包含_(标签, "相机_")
            || 文本包含_(标签, "连接状态")
            || 文本包含_(标签, "观测质量")
            || 文本包含_(标签, "缓存")
            || 文本包含_(标签, "识别率")
            || 文本包含_(标签, "延迟")) {
            return 状态面_功能_;
        }

        return 状态面_未定义_;
    }

    static std::uint64_t 推断状态面签名_(const 状态节点类* 状态) noexcept
    {
        if (!状态) return 状态面_未定义_;
        std::string 标签 = 取状态特征文本_(状态);
        const auto 特征主键 = 取状态特征主键_(状态);
        if (!特征主键.empty()) {
            if (!标签.empty()) 标签 += "|";
            标签 += 特征主键;
        }
        return 推断状态面签名_按标签_(标签);
    }

    static std::uint64_t 推断状态面签名_(const 状态节点主信息类* 状态) noexcept
    {
        if (!状态) return 状态面_未定义_;
        std::string 标签 = 取状态特征文本_(状态);
        const auto 特征主键 = 取状态特征主键_(状态);
        if (!特征主键.empty()) {
            if (!标签.empty()) 标签 += "|";
            标签 += 特征主键;
        }
        return 推断状态面签名_按标签_(标签);
    }

    struct 结构_状态面汇总结果 {
        std::uint64_t 签名 = 状态面_未定义_;
        bool 有冲突 = false;
    };

    static void 合并状态面签名_(结构_状态面汇总结果& out, std::uint64_t 签名) noexcept
    {
        if (out.有冲突 || 签名 == 状态面_未定义_) return;
        if (out.签名 == 状态面_未定义_) {
            out.签名 = 签名;
            return;
        }
        if (out.签名 != 签名) {
            out.有冲突 = true;
        }
    }

    static 结构_状态面汇总结果 汇总状态面签名_(
        const std::vector<状态节点类*>& 条件状态列表,
        const std::vector<状态节点类*>& 结果状态列表) noexcept
    {
        结构_状态面汇总结果 out{};
        for (auto* 状态 : 条件状态列表) {
            合并状态面签名_(out, 推断状态面签名_(状态));
            if (out.有冲突) return out;
        }
        for (auto* 状态 : 结果状态列表) {
            合并状态面签名_(out, 推断状态面签名_(状态));
            if (out.有冲突) return out;
        }
        return out;
    }

    static bool 是否技术性条件特征文本_(const std::string& 文本) noexcept
    {
        return 文本 == "方法_调用相位"
            || 文本 == "方法_运行状态"
            || 文本 == "方法_最后调用时间"
            || 文本 == "方法_本能动作ID"
            || 文本 == "方法_动作句柄类型"
            || 是否原始时间状态特征文本(文本);
    }

    static std::string 生成因动作动态键_(const 动态节点类* d) noexcept
    {
        auto* mi = 取动态主信息_(d);
        if (!mi) return {};
        std::string key = 取动态特征主键_(d);
        key += "|path=";
        key += std::to_string(mi->动态路径签名);
        if (mi->来源类型 == 枚举_动态来源类型::方法调用) {
            key += "|source=method";
            if (mi->来源动作名) {
                key += "|action=";
                key += mi->来源动作名->获取主键();
            }
            else if (mi->来源方法首节点) {
                key += "|method=";
                key += std::to_string(reinterpret_cast<std::uintptr_t>(mi->来源方法首节点));
            }
            key += "|ok=";
            key += mi->来源执行成功 ? "1" : "0";
        }
        return key;
    }

    static std::string 生成因果作用域键_(
        枚举_因果锚点类型 锚点类型,
        std::uint64_t 锚点主键,
        std::uint32_t 状态层级,
        std::uint64_t 状态面签名) noexcept
    {
        return std::string{"|anchorType="} + std::to_string(static_cast<std::uint32_t>(锚点类型))
            + "|anchor=" + std::to_string(锚点主键)
            + "|level=" + std::to_string(状态层级)
            + "|face=" + std::to_string(状态面签名)
            + "|";
    }

    static std::string 生成因果作用域键_(const 因果实例主信息类& x) noexcept
    {
        return 生成因果作用域键_(x.锚点类型, x.锚点主键, x.状态层级, x.状态面签名);
    }

    static std::string 生成抽象因果作用域键_(
        枚举_因果锚点类型 锚点类型,
        std::uint32_t 状态层级,
        std::uint64_t 状态面签名) noexcept
    {
        return std::string{"|anchorType="}
            + std::to_string(static_cast<std::uint32_t>(锚点类型))
            + "|level=" + std::to_string(状态层级)
            + "|face=" + std::to_string(状态面签名) + "|";
    }

    static std::string 生成抽象因果作用域键_(const 因果模板主信息类& x) noexcept
    {
        return 生成抽象因果作用域键_(x.适用锚点类型, x.适用层级, x.适用状态面签名);
    }

    static std::string 生成状态因果片段_(
        const 状态节点类* 状态,
        bool 方向可比较,
        std::int64_t 方向值) noexcept
    {
        if (!状态) return {};
        return std::string{"subject="} + 生成状态主体稳定键_(状态)
            + "|feature=" + 生成状态特征稳定键_(状态)
            + "|domain=" + 生成状态域稳定键_(状态)
            + "|dir=" + (方向可比较 ? std::to_string(方向值) : std::string{"na"})
            + "|reason=" + std::to_string(计算文本签名值_(取状态原因类别_(状态)));
    }

    static std::string 生成来源方法动作语义键_(std::uint64_t 来源方法主键) noexcept
    {
        return 来源方法主键 == 0
            ? std::string{}
            : (std::string{"method:"} + std::to_string(来源方法主键));
    }

    static std::string 生成来源任务动作语义键_(std::uint64_t 来源任务主键) noexcept
    {
        return 来源任务主键 == 0
            ? std::string{}
            : (std::string{"task:"} + std::to_string(来源任务主键));
    }

    static std::string 计算动作语义签名键_(
        const 因果实例主信息类& x,
        动态节点类* 因动作动态 = nullptr) noexcept
    {
        if (x.动作语义) {
            return std::string{"node:"} + 生成节点稳定键_(x.动作语义);
        }
        if (!x.动作语义主键_存档.empty()) {
            return std::string{"node:"} + std::to_string(解析存档主键_(x.动作语义主键_存档));
        }

        std::uint64_t 方法键 = x.来源方法主键;
        if (方法键 == 0 && !x.来源方法主键_存档.empty()) {
            方法键 = 解析存档主键_(x.来源方法主键_存档);
        }
        if (const auto 语义键 = 生成来源方法动作语义键_(方法键); !语义键.empty()) {
            return 语义键;
        }

        std::uint64_t 任务键 = x.来源任务主键;
        if (任务键 == 0 && !x.来源任务主键_存档.empty()) {
            任务键 = 解析存档主键_(x.来源任务主键_存档);
        }
        if (const auto 语义键 = 生成来源任务动作语义键_(任务键); !语义键.empty()) {
            return 语义键;
        }

        const auto 因动作键 = 生成因动作动态键_(因动作动态 ? 因动作动态 : (x.触发动态 ? x.触发动态 : x.动作动态));
        if (!因动作键.empty()) {
            return std::string{"dynamic:"} + std::to_string(计算文本签名值_(因动作键));
        }
        return {};
    }

    static std::string 生成动作语义累计键_(
        const 因果实例主信息类& x,
        动态节点类* 因动作动态) noexcept
    {
        const auto 语义键 = x.动作语义键.empty()
            ? 计算动作语义签名键_(x, 因动作动态)
            : x.动作语义键;
        return 语义键.empty() ? std::string{"none"} : 语义键;
    }

    static std::string 生成动作主体索引键_(const 因果实例主信息类& x) noexcept
    {
        const auto 主体键 = x.动作主体
            ? 生成节点稳定键_(x.动作主体)
            : (x.动作主体主键_存档.empty() ? std::string{} : std::to_string(解析存档主键_(x.动作主体主键_存档)));
        if (主体键.empty()) return {};
        return std::string{"kind=actor"} + 生成因果作用域键_(x)
            + "subject=" + 主体键 + "|";
    }

    static std::string 生成动作语义索引键_(const 因果实例主信息类& x) noexcept
    {
        const auto 语义键 = x.动作语义键.empty()
            ? 计算动作语义签名键_(x)
            : x.动作语义键;
        if (语义键.empty()) return {};
        return std::string{"kind=semantic"} + 生成因果作用域键_(x)
            + "semantic=" + 语义键 + "|";
    }

    static std::string 生成来源方法索引键_(const 因果实例主信息类& x) noexcept
    {
        std::uint64_t 方法键 = x.来源方法主键;
        if (方法键 == 0 && !x.来源方法主键_存档.empty()) {
            方法键 = 解析存档主键_(x.来源方法主键_存档);
        }
        if (方法键 == 0) return {};
        return std::string{"kind=method"} + 生成因果作用域键_(x)
            + "method=" + std::to_string(方法键) + "|";
    }

    static std::string 生成来源任务索引键_(const 因果实例主信息类& x) noexcept
    {
        std::uint64_t 任务键 = x.来源任务主键;
        if (任务键 == 0 && !x.来源任务主键_存档.empty()) {
            任务键 = 解析存档主键_(x.来源任务主键_存档);
        }
        if (任务键 == 0) return {};
        return std::string{"kind=task"} + 生成因果作用域键_(x)
            + "task=" + std::to_string(任务键) + "|";
    }

    static std::string 生成结果特征索引键_(
        const 因果实例主信息类& x,
        const 状态节点类* 结果状态) noexcept
    {
        if (!结果状态) return {};
        auto* 结果特征 = 取状态特征_(结果状态);
        if (!结果特征) return {};
        return std::string{"kind=result"} + 生成因果作用域键_(x)
            + "subject=" + 生成状态主体稳定键_(结果状态)
            + "|feature=" + 生成节点稳定键_(结果特征) + "|";
    }

    static std::string 生成结果特征索引键_(
        const 因果实例主信息类& x,
        const 状态节点主信息类* 结果状态) noexcept
    {
        if (!结果状态) return {};
        auto* 结果特征 = 取状态特征_(结果状态);
        if (!结果特征) return {};
        return std::string{"kind=result"} + 生成因果作用域键_(x)
            + "subject=" + 生成状态主体稳定键_(结果状态)
            + "|feature=" + 生成节点稳定键_(结果特征) + "|";
    }

    static std::string 生成抽象结果特征索引键_(const 因果模板主信息类& x) noexcept
    {
        auto* 主结果特征 = 取抽象模板主结果特征_(x);
        if (!主结果特征) return {};
        return std::string{"kind=abstract_result"} + 生成抽象因果作用域键_(x)
            + "feature=" + 生成节点稳定键_(主结果特征) + "|";
    }

    static std::string 生成抽象动作模板索引键_(const 因果模板主信息类& x) noexcept
    {
        if (!x.动作模板) return {};
        return std::string{"kind=abstract_action"} + 生成抽象因果作用域键_(x)
            + "action=" + 生成节点稳定键_(x.动作模板) + "|";
    }

    static bool 索引作用域匹配_(
        const std::string& 键,
        枚举_因果锚点类型 锚点类型,
        std::uint64_t 锚点主键,
        std::uint32_t 状态层级,
        std::uint64_t 状态面签名) noexcept
    {
        if (锚点类型 != 枚举_因果锚点类型::未定义) {
            const auto needle = std::string{"|anchorType="} + std::to_string(static_cast<std::uint32_t>(锚点类型)) + "|";
            if (键.find(needle) == std::string::npos) return false;
        }
        if (锚点主键 != 0) {
            const auto needle = std::string{"|anchor="} + std::to_string(锚点主键) + "|";
            if (键.find(needle) == std::string::npos) return false;
        }
        if (状态层级 != 0) {
            const auto needle = std::string{"|level="} + std::to_string(状态层级) + "|";
            if (键.find(needle) == std::string::npos) return false;
        }
        if (状态面签名 != 0) {
            const auto needle = std::string{"|face="} + std::to_string(状态面签名) + "|";
            if (键.find(needle) == std::string::npos) return false;
        }
        return true;
    }

    static bool 抽象索引作用域匹配_(
        const std::string& 键,
        枚举_因果锚点类型 锚点类型,
        std::uint32_t 状态层级,
        std::uint64_t 状态面签名) noexcept
    {
        return 索引作用域匹配_(键, 锚点类型, 0, 状态层级, 状态面签名);
    }

    void 从作用域索引收集因果_(
        const std::unordered_map<std::string, std::vector<基础信息节点类*>>& 索引,
        const std::string& 键片段,
        std::size_t 上限,
        枚举_因果锚点类型 锚点类型,
        std::uint64_t 锚点主键,
        std::uint32_t 状态层级,
        std::uint64_t 状态面签名,
        std::vector<基础信息节点类*>& out) const
    {
        if (键片段.empty()) return;
        for (const auto& [键, 节点列表] : 索引) {
            if (键.find(键片段) == std::string::npos) continue;
            if (!索引作用域匹配_(键, 锚点类型, 锚点主键, 状态层级, 状态面签名)) continue;
            for (auto* node : 节点列表) {
                追加唯一因果_(out, node);
            }
        }
        排序因果按稳定度_(out);
        if (上限 > 0 && out.size() > 上限) out.resize(上限);
    }

    static void 追加唯一模板节点_(std::vector<基础信息节点类*>& out, 基础信息节点类* node)
    {
        if (!node) return;
        if (std::find(out.begin(), out.end(), node) == out.end()) {
            out.push_back(node);
        }
    }

    static void 排序抽象因果按稳定度_(std::vector<基础信息节点类*>& out)
    {
        std::sort(out.begin(), out.end(), [](基础信息节点类* a, 基础信息节点类* b) {
            auto* ami = (a && a->主信息) ? dynamic_cast<因果模板主信息类*>(a->主信息) : nullptr;
            auto* bmi = (b && b->主信息) ? dynamic_cast<因果模板主信息类*>(b->主信息) : nullptr;
            const auto a稳定度 = ami ? ami->稳定度 : std::numeric_limits<std::int64_t>::min();
            const auto b稳定度 = bmi ? bmi->稳定度 : std::numeric_limits<std::int64_t>::min();
            if (a稳定度 != b稳定度) return a稳定度 > b稳定度;

            const auto a证据 = ami ? ami->证据次数 : 0ull;
            const auto b证据 = bmi ? bmi->证据次数 : 0ull;
            if (a证据 != b证据) return a证据 > b证据;

            const auto a成立 = ami ? ami->成立次数 : 0ull;
            const auto b成立 = bmi ? bmi->成立次数 : 0ull;
            if (a成立 != b成立) return a成立 > b成立;

            const auto a主键 = a ? a->获取主键() : std::string{};
            const auto b主键 = b ? b->获取主键() : std::string{};
            return a主键 < b主键;
        });
    }

    static void 排序因果链按长度和时间_(std::vector<基础信息节点类*>& out)
    {
        std::sort(out.begin(), out.end(), [](基础信息节点类* a, 基础信息节点类* b) {
            auto* ami = (a && a->主信息) ? dynamic_cast<因果链主信息类*>(a->主信息) : nullptr;
            auto* bmi = (b && b->主信息) ? dynamic_cast<因果链主信息类*>(b->主信息) : nullptr;
            const auto a长度 = ami ? ami->因果实例序列.size() : 0ull;
            const auto b长度 = bmi ? bmi->因果实例序列.size() : 0ull;
            if (a长度 != b长度) return a长度 > b长度;

            const auto a结束 = ami ? ami->结束时间 : 0;
            const auto b结束 = bmi ? bmi->结束时间 : 0;
            if (a结束 != b结束) return a结束 > b结束;

            const auto a开始 = ami ? ami->起始时间 : 0;
            const auto b开始 = bmi ? bmi->起始时间 : 0;
            if (a开始 != b开始) return a开始 > b开始;

            const auto a主键 = a ? a->获取主键() : std::string{};
            const auto b主键 = b ? b->获取主键() : std::string{};
            return a主键 < b主键;
        });
    }

    void 从作用域索引收集抽象因果_(
        const std::unordered_map<std::string, std::vector<基础信息节点类*>>& 索引,
        const std::string& 键片段,
        std::size_t 上限,
        枚举_因果锚点类型 锚点类型,
        std::uint32_t 状态层级,
        std::uint64_t 状态面签名,
        std::vector<基础信息节点类*>& out) const
    {
        if (键片段.empty()) return;
        for (const auto& [键, 节点列表] : 索引) {
            if (键.find(键片段) == std::string::npos) continue;
            if (!抽象索引作用域匹配_(键, 锚点类型, 状态层级, 状态面签名)) continue;
            for (auto* node : 节点列表) {
                追加唯一模板节点_(out, node);
            }
        }
        排序抽象因果按稳定度_(out);
        if (上限 > 0 && out.size() > 上限) out.resize(上限);
    }

    static bool 是否内部状态_(const 状态节点类* 状态) noexcept
    {
        auto* smi = 取状态主信息_(状态);
        return smi && smi->是否内部状态();
    }

    static bool 动态是否内部变化_(const 动态节点类* 动态) noexcept
    {
        auto* dmi = 取动态主信息_(动态);
        if (!dmi) return false;
        auto* 初始状态 = dmi->初始状态 ? static_cast<状态节点类*>(dmi->初始状态) : nullptr;
        auto* 结果状态 = dmi->结果状态 ? static_cast<状态节点类*>(dmi->结果状态) : nullptr;
        return 是否内部状态_(初始状态) || 是否内部状态_(结果状态);
    }

    static 枚举_因果生成通道 另一因果生成通道_(枚举_因果生成通道 通道) noexcept
    {
        if (通道 == 枚举_因果生成通道::场景) return 枚举_因果生成通道::存在;
        if (通道 == 枚举_因果生成通道::存在) return 枚举_因果生成通道::场景;
        return 枚举_因果生成通道::未定义;
    }

    static 枚举_因果生成通道 推断因果实例通道_(
        const 场景节点类* 场景,
        const 因果实例主信息类& 规范实例) noexcept
    {
        if (规范实例.锚点类型 == 枚举_因果锚点类型::存在) {
            return 枚举_因果生成通道::存在;
        }
        if (规范实例.锚点类型 == 枚举_因果锚点类型::场景) {
            return 枚举_因果生成通道::场景;
        }
        auto* smi = (场景 && 场景->主信息) ? dynamic_cast<场景节点主信息类*>(场景->主信息) : nullptr;
        return (smi && smi->是否内部世界()) ? 枚举_因果生成通道::存在 : 枚举_因果生成通道::场景;
    }

    static 枚举_因果生成通道 推断优先生成通道_(
        const 场景节点类* 场景,
        const 动态节点类* 锚点动态) noexcept
    {
        auto* smi = (场景 && 场景->主信息) ? dynamic_cast<场景节点主信息类*>(场景->主信息) : nullptr;
        if (smi && smi->是否内部世界()) {
            return 枚举_因果生成通道::存在;
        }
        if (动态是否内部变化_(锚点动态)) {
            return 枚举_因果生成通道::存在;
        }
        return 枚举_因果生成通道::场景;
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

    static void 追加唯一因果_(std::vector<基础信息节点类*>& out, 基础信息节点类* node)
    {
        if (!node) return;
        if (std::find(out.begin(), out.end(), node) != out.end()) return;
        out.push_back(node);
    }

    static void 清空实例因果动态引用_(基础信息节点类* node) noexcept
    {
        auto* mi = (node && node->主信息) ? dynamic_cast<因果主信息类*>(node->主信息) : nullptr;
        if (!mi) return;
        mi->条件状态列表.clear();
        mi->结果状态列表.clear();
        mi->主结果状态 = nullptr;
        mi->动作动态 = nullptr;
        mi->触发动态 = nullptr;
        mi->触发动态主键_存档.clear();
        mi->结果动态列表.clear();
        mi->结果动态主键列表_存档.clear();
        mi->主结果动态 = nullptr;
        mi->主结果动态主键_存档.clear();
        mi->因信息 = nullptr;
        mi->果信息 = nullptr;
        mi->锚点动态 = nullptr;
        mi->过程动态列表.clear();
        同步实例因果存档缓存_(mi);
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

    static void 收集场景状态列表_(const 场景节点类* 场景, std::vector<状态节点类*>& out)
    {
        if (!场景 || !场景->主信息) return;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!smi) return;
        std::unordered_map<std::string, 状态节点类*> 最新状态映射{};
        for (auto* 状态 : smi->状态列表) {
            if (!状态) continue;
            const auto 规约键 = 生成状态规约键_(状态);
            if (规约键.empty()) continue;
            auto it = 最新状态映射.find(规约键);
            if (it == 最新状态映射.end()) {
                最新状态映射.emplace(规约键, 状态);
                continue;
            }
            if (取状态时间_(状态) >= 取状态时间_(it->second)) {
                it->second = 状态;
            }
        }
        for (const auto& [_, 状态] : 最新状态映射) {
            追加唯一状态_(out, 状态);
        }
        std::sort(out.begin(), out.end(), [](const 状态节点类* a, const 状态节点类* b) {
            return 取状态时间_(a) < 取状态时间_(b);
        });
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

    template<class NodeT>
    static std::uint64_t 计算节点运行期主键_(const NodeT* node) noexcept
    {
        if (!node) return 0;
        const auto key = node->获取主键();
        if (!key.empty()) {
            return static_cast<std::uint64_t>(std::hash<std::string>{}(key));
        }
        return static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(node));
    }

    static std::string 编码哈希主键_(
        std::uint64_t 运行期主键) noexcept
    {
        return 运行期主键 == 0
            ? std::string{}
            : (std::string{"hash:"} + std::to_string(运行期主键));
    }

    static std::uint64_t 解析存档主键_(
        const std::string& 主键文本) noexcept
    {
        if (主键文本.empty()) return 0;
        if (主键文本.rfind("hash:", 0) == 0) {
            try {
                return static_cast<std::uint64_t>(std::stoull(主键文本.substr(5)));
            }
            catch (...) {
                return 0;
            }
        }
        return static_cast<std::uint64_t>(std::hash<std::string>{}(主键文本));
    }

    template<class NodeT>
    static std::vector<std::string> 收集节点主键列表_(
        const std::vector<NodeT*>& 节点列表) noexcept
    {
        std::vector<std::string> out{};
        out.reserve(节点列表.size());
        for (auto* 节点 : 节点列表) {
            if (!节点) continue;
            out.push_back(节点->获取主键());
        }
        return out;
    }

    static void 同步实例因果存档缓存_(
        因果实例主信息类* mi) noexcept
    {
        if (!mi) return;
        同步实例因果状态主信息缓存_(mi);

        if (mi->锚点信息) {
            if (mi->锚点主键_存档.empty()) {
                mi->锚点主键_存档 = mi->锚点信息->获取主键();
            }
            if (mi->锚点主键 == 0) {
                mi->锚点主键 = 计算节点运行期主键_(mi->锚点信息);
            }
        }
        else if (mi->锚点主键_存档.empty()) {
            mi->锚点主键_存档 = 编码哈希主键_(mi->锚点主键);
        }
        else if (mi->锚点主键 == 0) {
            mi->锚点主键 = 解析存档主键_(mi->锚点主键_存档);
        }

        if (mi->条件状态主键列表_存档.empty()) {
            if (!mi->条件状态主信息列表.empty()) {
                填充状态主键列表_(mi->条件状态主键列表_存档, mi->条件状态主信息列表);
            }
            else if (!mi->条件状态列表.empty()) {
                mi->条件状态主键列表_存档 = 收集节点主键列表_(mi->条件状态列表);
            }
        }
        if (mi->动作主体 && mi->动作主体主键_存档.empty()) {
            mi->动作主体主键_存档 = mi->动作主体->获取主键();
        }
        if (mi->动作语义 && mi->动作语义主键_存档.empty()) {
            mi->动作语义主键_存档 = mi->动作语义->获取主键();
        }
        if (!mi->触发动态 && mi->动作动态) {
            mi->触发动态 = mi->动作动态;
        }
        if (!mi->动作动态 && mi->触发动态) {
            mi->动作动态 = mi->触发动态;
        }
        if (mi->触发动态 && mi->触发动态主键_存档.empty()) {
            mi->触发动态主键_存档 = mi->触发动态->获取主键();
        }
        if (!mi->主结果状态 && mi->主结果状态主信息列表.empty() && mi->主结果状态主键_存档.empty()) {
            auto* 主结果动态信息 = 取动态主信息_(mi->主结果动态 ? mi->主结果动态 : mi->触发动态);
            if (主结果动态信息 && 主结果动态信息->结果状态) {
                mi->主结果状态 = static_cast<状态节点类*>(主结果动态信息->结果状态);
            }
        }
        if (!mi->主结果状态 && mi->主结果状态主信息列表.empty() && mi->主结果状态主键_存档.empty() && !mi->结果状态列表.empty()) {
            mi->主结果状态 = mi->结果状态列表.back();
        }
        if (mi->主结果状态) {
            追加唯一状态_(mi->结果状态列表, mi->主结果状态);
            if (mi->主结果状态主键_存档.empty()) {
                mi->主结果状态主键_存档 = mi->主结果状态->获取主键();
            }
        }
        else if (mi->主结果状态主键_存档.empty() && !mi->主结果状态主信息列表.empty()) {
            mi->主结果状态主键_存档 = 生成状态主信息存档键_(mi->主结果状态主信息列表.back());
        }
        if (mi->结果状态主键列表_存档.empty()) {
            if (!mi->结果状态主信息列表.empty()) {
                填充状态主键列表_(mi->结果状态主键列表_存档, mi->结果状态主信息列表);
            }
            else if (!mi->结果状态列表.empty()) {
                mi->结果状态主键列表_存档 = 收集节点主键列表_(mi->结果状态列表);
            }
        }
        if (!mi->主结果动态 && mi->主结果动态主键_存档.empty() && !mi->结果动态列表.empty()) {
            mi->主结果动态 = mi->结果动态列表.back();
        }
        if (mi->主结果动态) {
            追加唯一动态_(mi->结果动态列表, mi->主结果动态);
            if (mi->主结果动态主键_存档.empty()) {
                mi->主结果动态主键_存档 = mi->主结果动态->获取主键();
            }
        }
        if (mi->结果动态主键列表_存档.empty() && !mi->结果动态列表.empty()) {
            mi->结果动态主键列表_存档 = 收集节点主键列表_(mi->结果动态列表);
        }
        if (mi->状态面签名 == 0) {
            mi->状态面签名 = 汇总状态面签名_(mi->条件状态列表, mi->结果状态列表).签名;
        }

        if (mi->来源方法主键_存档.empty()) {
            auto* dmi = 取动态主信息_(mi->触发动态 ? mi->触发动态 : mi->动作动态);
            if (dmi && dmi->来源方法首节点) {
                mi->来源方法主键_存档 = dmi->来源方法首节点->获取主键();
            }
            else if (mi->来源方法主键 != 0) {
                mi->来源方法主键_存档 = 编码哈希主键_(mi->来源方法主键);
            }
        }
        if (mi->来源方法主键 == 0) {
            mi->来源方法主键 = 解析存档主键_(mi->来源方法主键_存档);
        }
        if (mi->来源任务主键_存档.empty() && mi->来源任务主键 != 0) {
            mi->来源任务主键_存档 = 编码哈希主键_(mi->来源任务主键);
        }
        if (mi->来源任务主键 == 0) {
            mi->来源任务主键 = 解析存档主键_(mi->来源任务主键_存档);
        }
        mi->动作语义键 = 计算动作语义签名键_(*mi);

        if (!mi->动作信息) {
            mi->动作信息 = mi->动作主体;
        }
    }

    static 特征节点类* 取实例主结果特征_(
        const 因果实例主信息类* mi) noexcept
    {
        if (!mi) return nullptr;
        if (const auto* 主结果状态 = 取实例主结果状态主信息_(*mi)) {
            if (auto* 特征 = 主结果状态->状态特征) return 特征;
        }
        if (auto* 兼容 = dynamic_cast<const 因果兼容主信息类*>(mi)) {
            if (auto* 特征 = 取状态特征_(兼容->结果状态)) return 特征;
        }
        return nullptr;
    }

    static std::vector<因果实例节点类*>* 取锚点实例因果列表_(
        基础信息节点类* 锚点) noexcept
    {
        if (!锚点 || !锚点->主信息) return nullptr;
        if (auto* smi = dynamic_cast<场景节点主信息类*>(锚点->主信息)) return &smi->实例因果列表;
        if (auto* emi = dynamic_cast<存在节点主信息类*>(锚点->主信息)) return &emi->实例因果列表;
        return nullptr;
    }

    static std::vector<std::string>* 取锚点实例因果存档列表_(
        基础信息节点类* 锚点) noexcept
    {
        if (!锚点 || !锚点->主信息) return nullptr;
        if (auto* smi = dynamic_cast<场景节点主信息类*>(锚点->主信息)) return &smi->实例因果主键列表_存档;
        if (auto* emi = dynamic_cast<存在节点主信息类*>(锚点->主信息)) return &emi->实例因果主键列表_存档;
        return nullptr;
    }

    static std::vector<因果链节点类*>* 取锚点因果链列表_(
        基础信息节点类* 锚点) noexcept
    {
        if (!锚点 || !锚点->主信息) return nullptr;
        if (auto* smi = dynamic_cast<场景节点主信息类*>(锚点->主信息)) return &smi->因果链列表;
        if (auto* emi = dynamic_cast<存在节点主信息类*>(锚点->主信息)) return &emi->因果链列表;
        return nullptr;
    }

    static std::vector<std::string>* 取锚点因果链存档列表_(
        基础信息节点类* 锚点) noexcept
    {
        if (!锚点 || !锚点->主信息) return nullptr;
        if (auto* smi = dynamic_cast<场景节点主信息类*>(锚点->主信息)) return &smi->因果链主键列表_存档;
        if (auto* emi = dynamic_cast<存在节点主信息类*>(锚点->主信息)) return &emi->因果链主键列表_存档;
        return nullptr;
    }

    static void 同步锚点因果链存档缓存_(基础信息节点类* 锚点) noexcept
    {
        auto* 列表 = 取锚点因果链列表_(锚点);
        auto* 存档列表 = 取锚点因果链存档列表_(锚点);
        if (!列表 || !存档列表) return;
        存档列表->clear();
        存档列表->reserve(列表->size());
        for (auto* 节点 : *列表) {
            if (!节点) continue;
            const auto 主键 = 节点->获取主键();
            if (!主键.empty()) {
                存档列表->push_back(主键);
            }
        }
    }

    static bool 是否实例因果锚点宿主_(const 基础信息节点类* 锚点) noexcept
    {
        if (!锚点 || !锚点->主信息) return false;
        return dynamic_cast<const 场景节点主信息类*>(锚点->主信息) != nullptr
            || dynamic_cast<const 存在节点主信息类*>(锚点->主信息) != nullptr;
    }

    static std::string 生成抽象因果键_(
        枚举_因果锚点类型 锚点类型,
        std::uint32_t 状态层级,
        std::uint64_t 状态面签名,
        特征节点类* 主结果特征) noexcept
    {
        if (!主结果特征) return {};
        return std::string{"abstract|anchor="}
            + std::to_string(static_cast<unsigned>(锚点类型))
            + "|level=" + std::to_string(状态层级)
            + "|face=" + std::to_string(状态面签名)
            + "|feature=" + 生成节点稳定键_(主结果特征);
    }

    static 特征节点类* 取抽象模板主结果特征_(
        const 因果模板主信息类& 模板) noexcept
    {
        const auto 从实例取特征 = [](const 因果实例节点类* 实例节点) noexcept -> 特征节点类* {
            auto* 实例 = (实例节点 && 实例节点->主信息)
                ? dynamic_cast<因果实例主信息类*>(实例节点->主信息)
                : nullptr;
            return 取实例主结果特征_(实例);
        };

        for (auto* 节点 : 模板.代表证据列表) {
            if (auto* 特征 = 从实例取特征(节点)) return 特征;
        }
        for (auto* 节点 : 模板.证据实例列表) {
            if (auto* 特征 = 从实例取特征(节点)) return 特征;
        }
        return nullptr;
    }

    static std::string 生成抽象模板签名键_(
        const 因果模板主信息类& 模板) noexcept
    {
        auto* 主结果特征 = 取抽象模板主结果特征_(模板);
        return 生成抽象因果键_(
            模板.适用锚点类型,
            模板.适用层级,
            模板.适用状态面签名,
            主结果特征);
    }

    static void 同步因果模板存档缓存_(
        因果模板主信息类* mi) noexcept
    {
        if (!mi) return;
        if (mi->条件模板主键列表_存档.empty()) {
            mi->条件模板主键列表_存档 = 收集节点主键列表_(mi->条件模板列表);
        }
        if (mi->动作模板 && mi->动作模板主键_存档.empty()) {
            mi->动作模板主键_存档 = mi->动作模板->获取主键();
        }
        if (mi->结果模板主键列表_存档.empty()) {
            mi->结果模板主键列表_存档 = 收集节点主键列表_(mi->结果模板列表);
        }
        if (mi->证据实例主键列表_存档.empty()) {
            mi->证据实例主键列表_存档 = 收集节点主键列表_(mi->证据实例列表);
        }
        if (mi->代表证据主键列表_存档.empty()) {
            mi->代表证据主键列表_存档 = 收集节点主键列表_(mi->代表证据列表);
        }
        if (mi->反例主键列表_存档.empty()) {
            mi->反例主键列表_存档 = 收集节点主键列表_(mi->反例实例列表);
        }
    }

    void 登记抽象因果索引_(
        基础信息节点类* node,
        const 因果模板主信息类& 模板)
    {
        if (!node) return;
        if (const auto 抽象键 = 生成抽象模板签名键_(模板); !抽象键.empty()) {
            抽象因果索引_[抽象键] = node;
        }
        if (const auto 键 = 生成抽象结果特征索引键_(模板); !键.empty()) {
            auto& vec = 抽象因果索引_结果特征_[键];
            追加唯一模板节点_(vec, node);
        }
        if (const auto 键 = 生成抽象动作模板索引键_(模板); !键.empty()) {
            auto& vec = 抽象因果索引_动作模板_[键];
            追加唯一模板节点_(vec, node);
        }
    }

    std::vector<因果实例节点类*> 收集抽象证据实例_(
        枚举_因果锚点类型 锚点类型,
        std::uint32_t 状态层级,
        std::uint64_t 状态面签名,
        特征节点类* 主结果特征) const
    {
        std::vector<因果实例节点类*> out{};
        if (!主结果特征) return out;
        for (auto* it = 根指针 ? 根指针->链下 : nullptr; it && it != 根指针; it = it->链下) {
            auto* mi = (it && it->主信息) ? dynamic_cast<因果实例主信息类*>(it->主信息) : nullptr;
            auto* 兼容 = (it && it->主信息) ? dynamic_cast<因果主信息类*>(it->主信息) : nullptr;
            if (!mi || !兼容) continue;
            if (兼容->结构类型 != 枚举_因果结构类型::实例) continue;
            auto* 父节点 = it->父 ? static_cast<基础信息节点类*>(it->父) : nullptr;
            if (!是否实例因果锚点宿主_(父节点)) continue;
            if (mi->锚点类型 != 锚点类型) continue;
            if (mi->状态层级 != 状态层级) continue;
            if (mi->状态面签名 != 状态面签名) continue;
            if (取实例主结果特征_(mi) != 主结果特征) continue;
            out.push_back(static_cast<因果实例节点类*>(it));
        }
        std::sort(out.begin(), out.end(), [](因果实例节点类* a, 因果实例节点类* b) {
            auto* ami = (a && a->主信息) ? dynamic_cast<因果主信息类*>(a->主信息) : nullptr;
            auto* bmi = (b && b->主信息) ? dynamic_cast<因果主信息类*>(b->主信息) : nullptr;
            const auto a最近 = ami ? ami->最近发生时间 : 0;
            const auto b最近 = bmi ? bmi->最近发生时间 : 0;
            if (a最近 != b最近) return a最近 > b最近;
            return a < b;
        });
        return out;
    }

    std::vector<因果实例节点类*> 收集抽象反例实例_(
        枚举_因果锚点类型 锚点类型,
        std::uint32_t 状态层级,
        std::uint64_t 状态面签名,
        特征节点类* 主结果特征,
        const std::string& 动作语义键) const
    {
        std::vector<因果实例节点类*> out{};
        if (!主结果特征 || 动作语义键.empty()) return out;
        for (auto* it = 根指针 ? 根指针->链下 : nullptr; it && it != 根指针; it = it->链下) {
            auto* mi = (it && it->主信息) ? dynamic_cast<因果实例主信息类*>(it->主信息) : nullptr;
            auto* 兼容 = (it && it->主信息) ? dynamic_cast<因果主信息类*>(it->主信息) : nullptr;
            if (!mi || !兼容) continue;
            if (兼容->结构类型 != 枚举_因果结构类型::实例) continue;
            auto* 父节点 = it->父 ? static_cast<基础信息节点类*>(it->父) : nullptr;
            if (!是否实例因果锚点宿主_(父节点)) continue;
            if (mi->锚点类型 != 锚点类型) continue;
            if (mi->状态层级 != 状态层级) continue;
            if (mi->状态面签名 != 状态面签名) continue;
            if (mi->动作语义键 != 动作语义键) continue;

            auto* 当前结果特征 = 取实例主结果特征_(mi);
            const bool 是失败样本 = !mi->已验证 || 当前结果特征 != 主结果特征;
            if (!是失败样本) continue;
            out.push_back(static_cast<因果实例节点类*>(it));
        }
        std::sort(out.begin(), out.end(), [](因果实例节点类* a, 因果实例节点类* b) {
            auto* ami = (a && a->主信息) ? dynamic_cast<因果主信息类*>(a->主信息) : nullptr;
            auto* bmi = (b && b->主信息) ? dynamic_cast<因果主信息类*>(b->主信息) : nullptr;
            const auto a最近 = ami ? ami->最近发生时间 : 0;
            const auto b最近 = bmi ? bmi->最近发生时间 : 0;
            if (a最近 != b最近) return a最近 > b最近;
            return a < b;
        });
        return out;
    }

    static std::vector<因果实例节点类*> 选取代表证据实例_(
        const std::vector<因果实例节点类*>& 输入,
        std::size_t 上限 = 8)
    {
        if (输入.size() <= 上限) return 输入;
        std::vector<因果实例节点类*> out{};
        out.reserve(上限);
        for (std::size_t i = 0; i < 上限; ++i) {
            out.push_back(输入[i]);
        }
        return out;
    }

    static std::string 拼接键摘要_(
        const std::vector<std::string>& 键列表,
        std::size_t 上限 = 4)
    {
        if (键列表.empty()) return {};
        std::string out{};
        const auto n = (std::min)(上限, 键列表.size());
        for (std::size_t i = 0; i < n; ++i) {
            if (!out.empty()) out += " | ";
            out += 键列表[i];
        }
        if (键列表.size() > n) {
            out += " | +" + std::to_string(键列表.size() - n);
        }
        return out;
    }

    static std::string 取词显示文本_(const 词性节点类* 词) noexcept
    {
        if (!词 || !词->父 || !词->父->主信息) return {};
        auto* 词信息 = dynamic_cast<词主信息类*>(词->父->主信息);
        if (词信息 && !词信息->词.empty()) return 词信息->词;
        return 词->获取主键();
    }

    static std::string 生成模板摘要_(
        const std::vector<二次特征节点类*>& 模板列表,
        std::size_t 上限 = 4)
    {
        return 拼接键摘要_(收集节点主键列表_(模板列表), 上限);
    }

    static std::string 快照值摘要文本_(const 特征快照值& 值) noexcept
    {
        if (const auto* i = std::get_if<I64>(&值)) {
            return std::to_string(*i);
        }
        if (std::holds_alternative<指针句柄>(值)) {
            return "Ptr";
        }
        if (std::holds_alternative<VecU句柄>(值)) {
            return "VecU";
        }
        return {};
    }

    static std::string 生成状态快照摘要_(const 状态节点主信息类* 状态) noexcept
    {
        if (!状态) return {};
        const auto 特征文本 = 取状态特征文本_(状态);
        if (特征文本.empty()) return {};
        const auto 值文本 = 快照值摘要文本_(状态->状态值);
        if (值文本.empty()) return 特征文本;
        return 特征文本 + "=" + 值文本;
    }

    static std::string 汇总高频摘要文本_(
        const std::unordered_map<std::string, std::uint64_t>& 频次,
        std::size_t 上限 = 4)
    {
        if (频次.empty()) return {};
        std::vector<std::pair<std::string, std::uint64_t>> 项目(频次.begin(), 频次.end());
        std::sort(项目.begin(), 项目.end(), [](const auto& a, const auto& b) {
            if (a.second != b.second) return a.second > b.second;
            return a.first < b.first;
        });

        std::string out{};
        const auto n = (std::min)(上限, 项目.size());
        for (std::size_t i = 0; i < n; ++i) {
            if (!out.empty()) out += " | ";
            out += 项目[i].first;
        }
        if (项目.size() > n) {
            out += " | +" + std::to_string(项目.size() - n);
        }
        return out;
    }

    static std::string 生成动作模板摘要_(基础信息节点类* 动作模板) noexcept
    {
        if (!动作模板) return {};
        if (动作模板->主信息) {
            if (auto* 特征信息 = dynamic_cast<特征节点主信息类*>(动作模板->主信息)) {
                if (const auto 文本 = 取词显示文本_(特征信息->类型); !文本.empty()) {
                    return 文本;
                }
            }
            if (auto* 方法首信息 = dynamic_cast<方法首节点主信息类*>(动作模板->主信息)) {
                if (const auto 文本 = 取词显示文本_(方法首信息->动作名); !文本.empty()) {
                    return 文本;
                }
            }
        }
        return 动作模板->获取主键();
    }

    static 时间戳 取实例最近发生时间_(const 因果实例节点类* 节点) noexcept
    {
        auto* mi = (节点 && 节点->主信息) ? dynamic_cast<因果主信息类*>(节点->主信息) : nullptr;
        return mi ? mi->最近发生时间 : 0;
    }

    static std::uint64_t 取实例观察次数_(const 因果实例节点类* 节点) noexcept
    {
        auto* mi = (节点 && 节点->主信息) ? dynamic_cast<因果主信息类*>(节点->主信息) : nullptr;
        if (!mi) return 0;
        return mi->观察次数 == 0 ? 1ull : static_cast<std::uint64_t>(mi->观察次数);
    }

    static std::uint64_t 汇总实例观察次数_(
        const std::vector<因果实例节点类*>& 节点列表) noexcept
    {
        std::uint64_t out = 0;
        for (auto* 节点 : 节点列表) {
            out += 取实例观察次数_(节点);
        }
        return out;
    }

    static 结构_状态面汇总结果 汇总状态面签名_(
        const std::vector<状态节点主信息类>& 条件状态列表,
        const std::vector<状态节点主信息类>& 结果状态列表) noexcept
    {
        结构_状态面汇总结果 out{};
        for (const auto& 状态 : 条件状态列表) {
            合并状态面签名_(out, 推断状态面签名_(&状态));
            if (out.有冲突) return out;
        }
        for (const auto& 状态 : 结果状态列表) {
            合并状态面签名_(out, 推断状态面签名_(&状态));
            if (out.有冲突) return out;
        }
        return out;
    }

    static std::string 生成失败边界摘要_(
        const std::vector<因果实例节点类*>& 反例实例)
    {
        if (反例实例.empty()) return {};
        auto* 实例 = (反例实例.front() && 反例实例.front()->主信息)
            ? dynamic_cast<因果实例主信息类*>(反例实例.front()->主信息)
            : nullptr;
        auto* 结果特征 = 取实例主结果特征_(实例);
        if (结果特征) {
            return std::string{"resultFeature="} + 结果特征->获取主键();
        }
        return std::string{"counterexample="} + 反例实例.front()->获取主键();
    }

    static 枚举_因果锚点类型 识别因果锚点类型_(const 基础信息节点类* node) noexcept
    {
        if (!node || !node->主信息) return 枚举_因果锚点类型::未定义;
        if (dynamic_cast<场景节点主信息类*>(node->主信息)) return 枚举_因果锚点类型::场景;
        if (dynamic_cast<存在节点主信息类*>(node->主信息)) return 枚举_因果锚点类型::存在;
        return 枚举_因果锚点类型::未定义;
    }

    static 基础信息节点类* 推断动作主体_(
        基础信息节点类* 显式动作主体,
        动态节点类* 因动作动态,
        动态节点类* 果动态) noexcept
    {
        if (显式动作主体) return 显式动作主体;
        auto 取动态主体 = [](动态节点类* 动态) noexcept -> 基础信息节点类* {
            auto* mi = 取动态主信息_(动态);
            return mi ? mi->动态主体 : nullptr;
        };
        if (auto* 主体 = 取动态主体(因动作动态)) return 主体;
        return 取动态主体(果动态);
    }

    static 基础信息节点类* 推断动作语义_(
        基础信息节点类* 显式动作语义,
        基础信息节点类* 动作主体,
        动态节点类* 因动作动态,
        动态节点类* 果动态) noexcept
    {
        if (显式动作语义 && 显式动作语义 != 动作主体) return 显式动作语义;
        auto 取动态语义 = [&](动态节点类* 动态) noexcept -> 基础信息节点类* {
            auto* mi = 取动态主信息_(动态);
            if (!mi) return nullptr;
            if (mi->动态特征 && mi->动态特征 != 动作主体) return mi->动态特征;
            return nullptr;
        };
        if (auto* 语义 = 取动态语义(因动作动态)) return 语义;
        return 取动态语义(果动态);
    }

    static 基础信息节点类* 推断因果锚点信息_(
        场景节点类* 场景,
        const std::vector<状态节点类*>& 条件状态列表,
        状态节点类* 结果状态,
        基础信息节点类* 动作主体) noexcept
    {
        基础信息节点类* 共同主体 = nullptr;
        bool 已有主体 = false;
        bool 全同主体 = true;

        auto 吸收状态主体 = [&](状态节点类* 状态) noexcept {
            auto* smi = 取状态主信息_(状态);
            if (!smi || !smi->状态主体) {
                全同主体 = false;
                return;
            }
            if (!已有主体) {
                共同主体 = smi->状态主体;
                已有主体 = true;
                return;
            }
            if (共同主体 != smi->状态主体) {
                全同主体 = false;
            }
        };

        for (auto* 条件状态 : 条件状态列表) 吸收状态主体(条件状态);
        吸收状态主体(结果状态);

        if (已有主体 && 全同主体 && 识别因果锚点类型_(共同主体) != 枚举_因果锚点类型::未定义) {
            return 共同主体;
        }
        if (识别因果锚点类型_(动作主体) != 枚举_因果锚点类型::未定义) {
            return 动作主体;
        }
        return 场景;
    }

    static std::uint32_t 推断状态层级_(
        const 状态节点类* 状态,
        const 基础信息节点类* 锚点信息,
        枚举_因果锚点类型 锚点类型) noexcept
    {
        auto* smi = 取状态主信息_(状态);
        if (!smi) return 0;

        if (锚点类型 == 枚举_因果锚点类型::存在) {
            if (smi->状态主体 == 锚点信息) return 1;
            if (smi->状态域 == 枚举_状态域::内部状态) return 2;
            return 1;
        }
        if (锚点类型 == 枚举_因果锚点类型::场景) {
            if (smi->状态域 == 枚举_状态域::世界状态) return 1;
            if (smi->状态域 == 枚举_状态域::内部状态) return 2;
            return 1;
        }
        return smi->状态域 == 枚举_状态域::内部状态 ? 2u : 1u;
    }

    static std::uint32_t 推断状态层级_(
        const 状态节点主信息类* 状态,
        const 基础信息节点类* 锚点信息,
        枚举_因果锚点类型 锚点类型) noexcept
    {
        if (!状态) return 0;

        if (锚点类型 == 枚举_因果锚点类型::存在) {
            if (状态->状态主体 == 锚点信息) return 1;
            if (状态->状态域 == 枚举_状态域::内部状态) return 2;
            return 1;
        }
        if (锚点类型 == 枚举_因果锚点类型::场景) {
            if (状态->状态域 == 枚举_状态域::世界状态) return 1;
            if (状态->状态域 == 枚举_状态域::内部状态) return 2;
            return 1;
        }
        return 状态->状态域 == 枚举_状态域::内部状态 ? 2u : 1u;
    }

    static std::uint32_t 汇总状态层级_(
        const std::vector<状态节点类*>& 状态列表,
        const 基础信息节点类* 锚点信息,
        枚举_因果锚点类型 锚点类型) noexcept
    {
        std::uint32_t out = 0;
        for (auto* 状态 : 状态列表) {
            const auto 层级 = 推断状态层级_(状态, 锚点信息, 锚点类型);
            if (层级 == 0) continue;
            if (out == 0) {
                out = 层级;
                continue;
            }
            if (out != 层级) {
                return 0;
            }
        }
        return out;
    }

    static 时间戳 推断条件时间_(
        const std::vector<状态节点类*>& 条件状态列表,
        时间戳 fallback = 0) noexcept
    {
        时间戳 out = 0;
        for (auto* 条件状态 : 条件状态列表) {
            const auto t = 取状态时间_(条件状态);
            if (t > out) out = t;
        }
        return out ? out : fallback;
    }

    static 时间戳 推断动作时间_(
        动态节点类* 动作动态,
        const std::vector<动态节点类*>& 过程动态列表,
        时间戳 fallback = 0) noexcept
    {
        if (动作动态) {
            const auto t = 取动态开始时间_(动作动态);
            if (t) return t;
        }
        if (!过程动态列表.empty()) {
            const auto t = 取动态开始时间_(过程动态列表.front());
            if (t) return t;
        }
        return fallback;
    }

    static 时间戳 推断结果时间_(
        状态节点类* 结果状态,
        动态节点类* 果动态,
        const std::vector<动态节点类*>& 过程动态列表,
        时间戳 fallback = 0) noexcept
    {
        const auto 状态时间 = 取状态时间_(结果状态);
        if (状态时间) return 状态时间;
        if (果动态) {
            const auto t = 取动态结束时间_(果动态);
            if (t) return t;
        }
        if (!过程动态列表.empty()) {
            const auto t = 取动态结束时间_(过程动态列表.back());
            if (t) return t;
        }
        return fallback;
    }

    static 枚举_因果来源类型 推断因果来源类型_(
        动态节点类* 动作动态,
        const std::vector<动态节点类*>& 过程动态列表,
        基础信息节点类* 动作语义) noexcept
    {
        auto* 动作动态信息 = 取动态主信息_(动作动态);
        if (动作动态信息 && 动作动态信息->来源类型 == 枚举_动态来源类型::方法调用) {
            return 枚举_因果来源类型::执行生成;
        }
        for (auto* 动态 : 过程动态列表) {
            auto* mi = 取动态主信息_(动态);
            if (mi && mi->来源类型 == 枚举_动态来源类型::方法调用) {
                return 枚举_因果来源类型::执行生成;
            }
        }
        if (动作语义 && 动作语义->主信息 && dynamic_cast<方法信息基类*>(动作语义->主信息)) {
            return 枚举_因果来源类型::执行生成;
        }
        return 枚举_因果来源类型::观察生成;
    }

    static std::uint64_t 推断来源方法主键_(
        动态节点类* 动作动态,
        const std::vector<动态节点类*>& 过程动态列表,
        基础信息节点类* 动作语义) noexcept
    {
        auto 取动态来源方法主键 = [](动态节点类* 动态) noexcept -> std::uint64_t {
            auto* mi = 取动态主信息_(动态);
            return (mi && mi->来源方法首节点) ? 计算节点运行期主键_(mi->来源方法首节点) : 0;
        };

        if (const auto k = 取动态来源方法主键(动作动态)) return k;
        for (auto* 动态 : 过程动态列表) {
            if (const auto k = 取动态来源方法主键(动态)) return k;
        }
        if (动作语义 && 动作语义->主信息 && dynamic_cast<方法信息基类*>(动作语义->主信息)) {
            return 计算节点运行期主键_(动作语义);
        }
        return 0;
    }

    static std::uint64_t 推断来源任务主键_(
        动态节点类* 动作动态,
        const std::vector<动态节点类*>& 过程动态列表,
        const std::vector<状态节点类*>& 条件状态列表,
        const std::vector<状态节点类*>& 结果状态列表,
        基础信息节点类* 动作主体,
        std::uint64_t 来源方法主键,
        const std::string& 调用点 = "因果类::推断来源任务主键") noexcept
    {
        using namespace 数据仓库模块;

        auto 取动态来源方法指针 = [](动态节点类* 动态) noexcept -> std::uintptr_t {
            auto* mi = 取动态主信息_(动态);
            return (mi && mi->来源方法首节点)
                ? reinterpret_cast<std::uintptr_t>(mi->来源方法首节点)
                : 0;
        };

        std::uintptr_t 来源方法指针 = 取动态来源方法指针(动作动态);
        if (来源方法指针 == 0) {
            for (auto* 动态 : 过程动态列表) {
                if ((来源方法指针 = 取动态来源方法指针(动态)) != 0) break;
            }
        }

        std::vector<基础信息节点类*> 候选主体{};
        auto 追加候选主体 = [&](基础信息节点类* 主体) noexcept {
            if (!主体) return;
            if (std::find(候选主体.begin(), 候选主体.end(), 主体) == 候选主体.end()) {
                候选主体.push_back(主体);
            }
        };
        auto 吸收状态主体 = [&](状态节点类* 状态) noexcept {
            auto* smi = 取状态主信息_(状态);
            if (smi && smi->状态主体) 追加候选主体(smi->状态主体);
        };

        追加候选主体(动作主体);
        for (auto* 条件状态 : 条件状态列表) 吸收状态主体(条件状态);
        for (auto* 结果状态 : 结果状态列表) 吸收状态主体(结果状态);

        auto 推导所属任务头结点 = [](任务节点类* 节点) noexcept -> 任务节点类* {
            if (!节点 || !节点->主信息) return nullptr;
            if (dynamic_cast<任务头结点信息*>(节点->主信息)) return 节点;
            if (auto* 步骤信息 = dynamic_cast<任务步骤节点信息*>(节点->主信息)) return 步骤信息->所属任务头结点;
            if (auto* 结果信息 = dynamic_cast<任务结果节点信息*>(节点->主信息)) return 结果信息->所属任务头结点;
            return nullptr;
        };

        auto 方法指针命中来源方法 = [&](std::uintptr_t 方法指针) noexcept -> bool {
            if (方法指针 == 0) return false;
            if (来源方法指针 != 0 && 方法指针 == 来源方法指针) return true;
            if (来源方法主键 != 0) {
                auto* 方法节点 = reinterpret_cast<方法节点类*>(方法指针);
                if (方法节点 && 计算节点运行期主键_(方法节点) == 来源方法主键) {
                    return true;
                }
            }
            return false;
        };

        任务节点类* 主体命中任务头 = nullptr;
        任务节点类* 当前方法命中任务头 = nullptr;
        任务节点类* 最近方法命中任务头 = nullptr;

        锁调度器守卫 锁({
            锁请求::读(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点 + "/世界链"),
            锁请求::读(任务链.链表锁, 枚举_锁域::任务链, "任务链", 调用点 + "/任务链")
        });

        for (auto* it = 任务链.根指针 ? 任务链.根指针->链下 : nullptr; it && it != 任务链.根指针; it = it->链下) {
            auto* 任务节点 = static_cast<任务节点类*>(it);
            auto* 任务信息 = (任务节点 && 任务节点->主信息)
                ? dynamic_cast<任务信息基类*>(任务节点->主信息)
                : nullptr;
            if (!任务信息) continue;

            if (!主体命中任务头 && !候选主体.empty() && 任务信息->任务虚拟存在) {
                if (std::find(候选主体.begin(), 候选主体.end(), static_cast<基础信息节点类*>(任务信息->任务虚拟存在)) != 候选主体.end()) {
                    主体命中任务头 = 推导所属任务头结点(任务节点);
                    if (主体命中任务头) break;
                }
            }

            auto* 头信息 = dynamic_cast<任务头结点信息*>(任务节点->主信息);
            if (!头信息) continue;

            if (!当前方法命中任务头) {
                const auto 当前方法指针 = 读取任务执行账指针_已持有世界锁_(
                    头信息,
                    特征_任务当前方法首节点指针_(),
                    调用点 + "/读任务当前方法");
                if (当前方法指针.has_value() && 方法指针命中来源方法(*当前方法指针)) {
                    当前方法命中任务头 = 任务节点;
                }
            }

            if (!最近方法命中任务头) {
                const auto 最近方法指针 = 读取任务执行账指针_已持有世界锁_(
                    头信息,
                    特征_任务最近方法首节点指针_(),
                    调用点 + "/读任务最近方法");
                if (最近方法指针.has_value() && 方法指针命中来源方法(*最近方法指针)) {
                    最近方法命中任务头 = 任务节点;
                }
            }
        }

        if (主体命中任务头) return 计算节点运行期主键_(主体命中任务头);
        if (当前方法命中任务头) return 计算节点运行期主键_(当前方法命中任务头);
        if (最近方法命中任务头) return 计算节点运行期主键_(最近方法命中任务头);
        return 0;
    }

    static const char* 取因果校验结果文本_(枚举_因果校验结果 结果) noexcept
    {
        switch (结果) {
        case 枚举_因果校验结果::合法: return "合法";
        case 枚举_因果校验结果::条件为空: return "条件为空";
        case 枚举_因果校验结果::结果为空: return "结果为空";
        case 枚举_因果校验结果::锚点不一致: return "锚点不一致";
        case 枚举_因果校验结果::层级不一致: return "层级不一致";
        case 枚举_因果校验结果::时间逆序: return "时间逆序";
        case 枚举_因果校验结果::动作语义缺失: return "动作语义缺失";
        case 枚举_因果校验结果::跨层直连: return "跨层直连";
        case 枚举_因果校验结果::状态面不一致: return "状态面不一致";
        default: return "未知校验结果";
        }
    }

    static void 填充实例因果规范字段_(
        因果实例主信息类* mi,
        场景节点类* 场景,
        动态节点类* 因动作动态,
        动态节点类* 果动态,
        const std::vector<状态节点类*>& 条件状态列表,
        基础信息节点类* 动作主体,
        基础信息节点类* 动作语义,
        const std::vector<动态节点类*>& 过程动态列表,
        状态节点类* 结果状态,
        时间戳 now) noexcept
    {
        if (!mi) return;

        mi->动作主体 = 推断动作主体_(动作主体, 因动作动态, 果动态);
        mi->动作语义 = 推断动作语义_(动作语义, mi->动作主体, 因动作动态, 果动态);
        mi->动作信息 = mi->动作主体;
        mi->动作动态 = 因动作动态;
        mi->触发动态 = 因动作动态 ? 因动作动态 : 果动态;

        auto* 锚点信息 = 推断因果锚点信息_(场景, 条件状态列表, 结果状态, mi->动作主体);
        if (auto* 兼容mi = dynamic_cast<因果主信息类*>(mi)) {
            兼容mi->结构类型 = 枚举_因果结构类型::实例;
        }
        mi->锚点信息 = 锚点信息;
        mi->锚点类型 = 识别因果锚点类型_(锚点信息);
        mi->锚点主键 = 计算节点运行期主键_(锚点信息);
        mi->状态层级 = 0;
        for (auto* 条件状态 : 条件状态列表) {
            const auto 层级 = 推断状态层级_(条件状态, 锚点信息, mi->锚点类型);
            if (层级 != 0) {
                mi->状态层级 = 层级;
                break;
            }
        }
        if (mi->状态层级 == 0) {
            mi->状态层级 = 推断状态层级_(结果状态, 锚点信息, mi->锚点类型);
        }
        mi->条件时间 = 推断条件时间_(条件状态列表, now);
        mi->动作时间 = 推断动作时间_(因动作动态, 过程动态列表, mi->条件时间 ? mi->条件时间 : now);
        mi->结果时间 = 推断结果时间_(结果状态, 果动态, 过程动态列表, mi->动作时间 ? mi->动作时间 : now);
        mi->结果状态列表.clear();
        if (结果状态) mi->结果状态列表.push_back(结果状态);
        mi->主结果状态 = 结果状态;
        mi->结果动态列表.clear();
        if (果动态) {
            mi->结果动态列表.push_back(果动态);
        }
        else if (!过程动态列表.empty() && 过程动态列表.back()) {
            mi->结果动态列表.push_back(过程动态列表.back());
        }
        mi->主结果动态 = 果动态 ? 果动态
            : (mi->结果动态列表.empty() ? nullptr : mi->结果动态列表.back());
        同步实例因果状态主信息缓存_(mi);
        mi->状态面签名 = 汇总状态面签名_(mi->条件状态列表, mi->结果状态列表).签名;
        mi->来源类型 = 推断因果来源类型_(mi->触发动态, 过程动态列表, mi->动作语义);
        mi->来源方法主键 = 推断来源方法主键_(mi->触发动态, 过程动态列表, mi->动作语义);
        mi->来源任务主键 = 推断来源任务主键_(
            mi->触发动态,
            过程动态列表,
            mi->条件状态列表,
            mi->结果状态列表,
            mi->动作主体,
            mi->来源方法主键);
        mi->动作语义键 = 计算动作语义签名键_(*mi, 因动作动态);
        mi->置信度 = (mi->来源类型 == 枚举_因果来源类型::推断生成) ? 0 : 10000;
        mi->已验证 = (mi->来源类型 != 枚举_因果来源类型::推断生成);
    }

    static void 写入实例因果规范字段_(
        因果实例主信息类* 目标,
        const 因果实例主信息类& 源) noexcept
    {
        if (!目标) return;
        目标->锚点类型 = 源.锚点类型;
        目标->锚点主键 = 源.锚点主键;
        目标->状态层级 = 源.状态层级;
        目标->状态面签名 = 源.状态面签名;
        目标->锚点信息 = 源.锚点信息;
        目标->条件时间 = 源.条件时间;
        目标->动作时间 = 源.动作时间;
        目标->结果时间 = 源.结果时间;
        目标->条件状态列表 = 源.条件状态列表;
        复制状态主信息列表_(目标->条件状态主信息列表, 源.条件状态主信息列表);
        目标->动作主体 = 源.动作主体;
        目标->动作语义 = 源.动作语义;
        目标->动作语义键 = 源.动作语义键;
        目标->动作信息 = 源.动作信息;
        目标->动作动态 = 源.动作动态;
        目标->触发动态 = 源.触发动态;
        目标->结果状态列表 = 源.结果状态列表;
        目标->主结果状态 = 源.主结果状态;
        复制状态主信息列表_(目标->结果状态主信息列表, 源.结果状态主信息列表);
        复制状态主信息列表_(目标->主结果状态主信息列表, 源.主结果状态主信息列表);
        目标->结果动态列表 = 源.结果动态列表;
        目标->主结果动态 = 源.主结果动态;
        目标->来源类型 = 源.来源类型;
        目标->来源任务主键 = 源.来源任务主键;
        目标->来源方法主键 = 源.来源方法主键;
        目标->置信度 = 源.置信度;
        目标->已验证 = 源.已验证;
        目标->锚点主键_存档 = 源.锚点主键_存档;
        目标->条件状态主键列表_存档 = 源.条件状态主键列表_存档;
        目标->动作主体主键_存档 = 源.动作主体主键_存档;
        目标->动作语义主键_存档 = 源.动作语义主键_存档;
        目标->触发动态主键_存档 = 源.触发动态主键_存档;
        目标->结果状态主键列表_存档 = 源.结果状态主键列表_存档;
        目标->主结果状态主键_存档 = 源.主结果状态主键_存档;
        目标->结果动态主键列表_存档 = 源.结果动态主键列表_存档;
        目标->主结果动态主键_存档 = 源.主结果动态主键_存档;
        目标->来源任务主键_存档 = 源.来源任务主键_存档;
        目标->来源方法主键_存档 = 源.来源方法主键_存档;
        同步实例因果存档缓存_(目标);
    }

    static 枚举_因果校验结果 校验因果实例合法性_(const 因果实例主信息类& x) noexcept
    {
        const bool 有条件状态 = !x.条件状态主信息列表.empty() || !x.条件状态列表.empty();
        const bool 有结果状态 = !x.主结果状态主信息列表.empty() || !x.结果状态主信息列表.empty() || !x.结果状态列表.empty();
        if (!有条件状态) return 枚举_因果校验结果::条件为空;
        if (!有结果状态 && x.结果动态列表.empty()) return 枚举_因果校验结果::结果为空;
        if (x.锚点类型 == 枚举_因果锚点类型::未定义 || !x.锚点信息 || x.锚点主键 == 0) {
            return 枚举_因果校验结果::锚点不一致;
        }
        if (x.条件时间 == 0 || x.动作时间 == 0 || x.结果时间 == 0) {
            return 枚举_因果校验结果::时间逆序;
        }
        if (!(x.条件时间 <= x.动作时间 && x.动作时间 <= x.结果时间)) {
            return 枚举_因果校验结果::时间逆序;
        }
        if (x.动作语义键.empty() && 计算动作语义签名键_(x).empty()) {
            return 枚举_因果校验结果::动作语义缺失;
        }

        std::uint32_t 条件参考层级 = x.状态层级;
        std::uint32_t 结果参考层级 = x.状态层级;
        auto 归一化状态层级 = [&](const 状态节点主信息类& 状态) noexcept -> std::uint32_t {
            if (!状态.状态主体) return 0;
            if (x.锚点类型 == 枚举_因果锚点类型::存在 && 状态.状态主体 != x.锚点信息) {
                return 0;
            }
            return 推断状态层级_(&状态, x.锚点信息, x.锚点类型);
        };

        bool 条件非法 = false;
        遍历实例条件状态主信息_(x, [&](const 状态节点主信息类& 条件状态) noexcept {
            const auto 层级 = 归一化状态层级(条件状态);
            if (层级 == 0) {
                条件非法 = true;
                return;
            }
            if (条件参考层级 == 0) {
                条件参考层级 = 层级;
            }
            else if (条件参考层级 != 层级) {
                条件非法 = true;
            }
        });
        if (条件非法) {
            return 条件参考层级 == 0 ? 枚举_因果校验结果::锚点不一致 : 枚举_因果校验结果::层级不一致;
        }
        bool 结果非法 = false;
        遍历实例结果状态主信息_(x, [&](const 状态节点主信息类& 结果状态) noexcept {
            const auto 层级 = 归一化状态层级(结果状态);
            if (层级 == 0) {
                结果非法 = true;
                return;
            }
            if (结果参考层级 == 0) {
                结果参考层级 = 层级;
            }
            else if (结果参考层级 != 层级) {
                结果非法 = true;
            }
        });
        if (结果非法) {
            return 结果参考层级 == 0 ? 枚举_因果校验结果::锚点不一致 : 枚举_因果校验结果::层级不一致;
        }
        if (条件参考层级 != 0 && 结果参考层级 != 0 && 条件参考层级 != 结果参考层级) {
            return 枚举_因果校验结果::跨层直连;
        }
        if (x.状态层级 != 0) {
            if ((条件参考层级 != 0 && x.状态层级 != 条件参考层级)
                || (结果参考层级 != 0 && x.状态层级 != 结果参考层级)) {
                return 枚举_因果校验结果::层级不一致;
            }
        }

        const auto 状态面结果 = (!x.条件状态主信息列表.empty() || !x.结果状态主信息列表.empty())
            ? 汇总状态面签名_(x.条件状态主信息列表, x.结果状态主信息列表)
            : 汇总状态面签名_(x.条件状态列表, x.结果状态列表);
        if (状态面结果.有冲突) {
            return 枚举_因果校验结果::状态面不一致;
        }
        if (x.状态面签名 != 0
            && 状态面结果.签名 != 0
            && x.状态面签名 != 状态面结果.签名) {
            return 枚举_因果校验结果::状态面不一致;
        }
        return 枚举_因果校验结果::合法;
    }

    static 内部方法::结果<因果实例主信息类> 规范化因果草案_(
        const 结构_因果草案& 草案,
        时间戳 now,
        枚举_因果校验结果* 输出校验结果 = nullptr) noexcept
    {
        内部方法::结果<因果实例主信息类> out{};
        if (输出校验结果) *输出校验结果 = 枚举_因果校验结果::合法;
        if (now == 0) now = 当前时间_();

        因果实例主信息类 规范实例{};
        for (auto* 条件状态 : 草案.条件状态列表) {
            追加唯一状态_(规范实例.条件状态列表, 条件状态);
        }

        std::vector<动态节点类*> 规范过程动态列表{};
        for (auto* 动态 : 草案.过程动态列表) {
            追加唯一动态_(规范过程动态列表, 动态);
        }

        std::vector<状态节点类*> 规范结果状态列表{};
        for (auto* 结果状态 : 草案.结果状态列表) {
            追加唯一状态_(规范结果状态列表, 结果状态);
        }

        auto* 因动作动态 = 选择因动作动态_(草案.因动态, 规范过程动态列表);
        auto* 果动态 = 草案.果动态 ? 草案.果动态 : (规范过程动态列表.empty() ? nullptr : 规范过程动态列表.back());
        auto* 主结果动作动态 = 选择主结果动作动态_(果动态, 规范过程动态列表);
        auto* 结果状态 = 选择结果状态_(主结果动作动态 ? 主结果动作动态 : 果动态, 规范过程动态列表, 规范结果状态列表);
        if (结果状态) {
            追加唯一状态_(规范结果状态列表, 结果状态);
        }
        补足同特征前态条件_(草案.场景, 结果状态, 规范实例.条件状态列表);
        if (规范实例.条件状态列表.empty()) {
            if (输出校验结果) *输出校验结果 = 枚举_因果校验结果::条件为空;
            out.错误信息 = 取因果校验结果文本_(枚举_因果校验结果::条件为空);
            return out;
        }

        填充实例因果规范字段_(
            &规范实例,
            草案.场景,
            因动作动态,
            主结果动作动态 ? 主结果动作动态 : 果动态,
            规范实例.条件状态列表,
            草案.动作主体,
            草案.动作语义,
            规范过程动态列表,
            结果状态,
            now);

        规范实例.结果状态列表.clear();
        for (auto* 状态 : 规范结果状态列表) {
            追加唯一状态_(规范实例.结果状态列表, 状态);
        }
        if (规范实例.结果状态列表.empty()) {
            追加唯一状态_(规范实例.结果状态列表, 结果状态);
        }
        规范实例.主结果状态 = 结果状态
            ? 结果状态
            : (规范实例.结果状态列表.empty() ? nullptr : 规范实例.结果状态列表.back());
        同步实例因果状态主信息缓存_(&规范实例);

        规范实例.结果动态列表.clear();
        if (草案.果动态) {
            追加唯一动态_(规范实例.结果动态列表, 草案.果动态);
        }
        if (主结果动作动态 && 主结果动作动态 != 草案.果动态) {
            追加唯一动态_(规范实例.结果动态列表, 主结果动作动态);
        }
        else if (果动态) {
            追加唯一动态_(规范实例.结果动态列表, 果动态);
        }
        规范实例.主结果动态 = 主结果动作动态
            ? 主结果动作动态
            : (草案.果动态
                ? 草案.果动态
                : (果动态 ? 果动态
                    : (规范实例.结果动态列表.empty() ? nullptr : 规范实例.结果动态列表.back())));

        规范实例.状态面签名 = 汇总状态面签名_(规范实例.条件状态列表, 规范实例.结果状态列表).签名;

        const auto 校验结果 = 校验因果实例合法性_(规范实例);
        if (校验结果 != 枚举_因果校验结果::合法) {
            if (输出校验结果) *输出校验结果 = 校验结果;
            out.错误信息 = 取因果校验结果文本_(校验结果);
            return out;
        }

        out.成功 = true;
        写入实例因果规范字段_(&out.数据, 规范实例);
        return out;
    }

    static std::int64_t 估计稳定度Q_(const 因果主信息类* mi) noexcept
    {
        if (!mi) return 0;
        const std::int64_t 次数分 = mi->观察次数 * 100;
        const std::int64_t 动作分 = (!mi->动作语义键.empty() || mi->动作语义) ? 300 : (mi->动作主体 ? 200 : (mi->触发动态 ? 100 : 0));
        const std::int64_t 条件分 = !mi->条件状态列表.empty() ? 200 : 0;
        const std::int64_t 条件方向分 = static_cast<std::int64_t>(
            std::count(mi->条件方向可比较列表.begin(), mi->条件方向可比较列表.end(), static_cast<std::uint8_t>(1))) * 50;
        const std::int64_t 过程分 = !mi->过程动态列表.empty() ? 150 : 0;
        const std::int64_t 结果分 = mi->结果状态 ? 250 : 0;
        const std::int64_t 方向分 = mi->结果方向可比较 ? 300 : 100;
        return 次数分 + 动作分 + 条件分 + 条件方向分 + 过程分 + 结果分 + 方向分;
    }

    static bool 提取状态方向_(const 状态节点类* 前状态, const 状态节点类* 当前状态, std::int64_t& 方向值) noexcept
    {
        auto* 前mi = 取状态主信息_(前状态);
        auto* 当前mi = 取状态主信息_(当前状态);
        if (!前mi || !当前mi) return false;
        if (前mi->状态特征 != 当前mi->状态特征) return false;
        const auto* 前值 = std::get_if<I64>(&前mi->状态值);
        const auto* 当前值 = std::get_if<I64>(&当前mi->状态值);
        if (!前值 || !当前值) return false;
        方向值 = (*当前值 > *前值) ? 1 : ((*当前值 < *前值) ? -1 : 0);
        return true;
    }

    static 状态节点类* 查找前一同特征状态_(
        const 场景节点类* 场景,
        状态节点类* 当前状态) noexcept
    {
        if (!场景 || !场景->主信息 || !当前状态) return nullptr;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        auto* 当前信息 = 取状态主信息_(当前状态);
        if (!smi || !当前信息) return nullptr;

        const auto 当前时间 = 取状态时间_(当前状态);
        状态节点类* 最佳 = nullptr;
        时间戳 最佳时间 = 0;
        for (auto* 状态 : smi->状态列表) {
            if (!状态 || 状态 == 当前状态) continue;
            auto* 状态信息 = 取状态主信息_(状态);
            if (!状态信息) continue;
            if (状态信息->状态主体 != 当前信息->状态主体) continue;
            if (状态信息->状态特征 != 当前信息->状态特征) continue;

            const auto t = 取状态时间_(状态);
            if (!t) continue;
            if (当前时间 != 0 && t >= 当前时间) continue;
            if (!最佳 || t > 最佳时间) {
                最佳 = 状态;
                最佳时间 = t;
            }
        }
        return 最佳;
    }

    static 二次特征节点类* 查找状态方向二次特征_(
        const 场景节点类* 场景,
        状态节点类* 前状态,
        状态节点类* 当前状态) noexcept
    {
        if (!场景 || !场景->主信息 || !前状态 || !当前状态) return nullptr;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!smi) return nullptr;

        for (auto* 关系节点 : smi->关系列表) {
            auto* 关系信息 = (关系节点 && 关系节点->主信息)
                ? dynamic_cast<二次特征主信息类*>(关系节点->主信息)
                : nullptr;
            if (!关系信息) continue;
            if (关系信息->度量签名_链键 != "状态|值变化方向") continue;
            if (关系信息->左对象 == 当前状态 && 关系信息->右对象 == 前状态) {
                return 关系节点;
            }
        }
        return nullptr;
    }

    struct 结构_状态方向摘要 {
        状态节点类* 前状态 = nullptr;
        二次特征节点类* 方向二次特征 = nullptr;
        std::string 方向链键{};
        std::int64_t 方向值 = 0;
        bool 可比较 = false;
    };

    static 结构_状态方向摘要 解析状态方向_(
        const 场景节点类* 场景,
        状态节点类* 当前状态) noexcept
    {
        结构_状态方向摘要 out{};
        if (!场景 || !当前状态) return out;
        out.前状态 = 查找前一同特征状态_(场景, 当前状态);
        out.可比较 = 提取状态方向_(out.前状态, 当前状态, out.方向值);
        out.方向二次特征 = 查找状态方向二次特征_(场景, out.前状态, 当前状态);
        auto* 方向信息 = (out.方向二次特征 && out.方向二次特征->主信息)
            ? dynamic_cast<二次特征主信息类*>(out.方向二次特征->主信息)
            : nullptr;
        out.方向链键 = 方向信息 ? 方向信息->度量签名_链键 : std::string{"状态|值变化方向"};
        return out;
    }

    static 动态节点类* 选择主结果动作动态_(
        动态节点类* 果动态,
        const std::vector<动态节点类*>& 过程动态列表) noexcept
    {
        auto 是显式动作动态 = [](动态节点类* 动态) noexcept -> bool {
            auto* mi = 取动态主信息_(动态);
            if (!mi) return false;
            if (mi->来源类型 != 枚举_动态来源类型::未定义) return true;
            if (mi->来源动作名 || mi->来源方法首节点) return true;
            return mi->初始状态 == nullptr && mi->结果状态 == nullptr
                && mi->动态主体 != nullptr
                && mi->动态特征 != nullptr;
        };
        auto 打分 = [&](动态节点类* 动态) noexcept -> int {
            auto* mi = 取动态主信息_(动态);
            if (!mi || !mi->结果状态) return -1;
            if (!是显式动作动态(动态)) return -1;
            switch (mi->来源动作相位) {
            case 枚举_动作事件相位::完成:
                return 4;
            case 枚举_动作事件相位::失败:
                return 3;
            case 枚举_动作事件相位::未定义:
                return 2;
            case 枚举_动作事件相位::开始运行:
                return 1;
            default:
                return 0;
            }
        };

        动态节点类* 最优 = nullptr;
        int 最优分 = -1;
        auto 更新最优 = [&](动态节点类* 动态) noexcept {
            const int 分 = 打分(动态);
            if (分 > 最优分) {
                最优 = 动态;
                最优分 = 分;
            }
        };

        更新最优(果动态);
        for (auto* 动态 : 过程动态列表) {
            更新最优(动态);
        }
        return 最优;
    }

    static 状态节点类* 选择结果状态_(
        动态节点类* 果动态,
        const std::vector<动态节点类*>& 过程动态列表,
        const std::vector<状态节点类*>& 候选结果状态列表) noexcept
    {
        if (auto* 主结果动作动态 = 选择主结果动作动态_(果动态, 过程动态列表)) {
            auto* 主结果动作信息 = 取动态主信息_(主结果动作动态);
            if (主结果动作信息 && 主结果动作信息->结果状态) {
                return static_cast<状态节点类*>(主结果动作信息->结果状态);
            }
        }

        if (果动态) {
            auto* 果动态信息 = 取动态主信息_(果动态);
            if (果动态信息 && 果动态信息->结果状态) {
                return static_cast<状态节点类*>(果动态信息->结果状态);
            }
        }

        if (!过程动态列表.empty()) {
            auto* 末动态信息 = 取动态主信息_(过程动态列表.back());
            if (末动态信息 && 末动态信息->结果状态) {
                return static_cast<状态节点类*>(末动态信息->结果状态);
            }
        }

        状态节点类* 最新状态 = nullptr;
        时间戳 最新时间 = 0;
        for (auto* 状态 : 候选结果状态列表) {
            const auto t = 取状态时间_(状态);
            if (!最新状态 || t >= 最新时间) {
                最新状态 = 状态;
                最新时间 = t;
            }
        }
        return 最新状态;
    }

    static bool 是否显式动作动态_(动态节点类* 动态) noexcept
    {
        auto* mi = 取动态主信息_(动态);
        if (!mi) return false;
        if (mi->来源类型 != 枚举_动态来源类型::未定义) return true;
        if (mi->来源动作名 || mi->来源方法首节点) return true;
        return mi->初始状态 == nullptr && mi->结果状态 == nullptr
            && mi->动态主体 != nullptr
            && mi->动态特征 != nullptr;
    }

    static 动态节点类* 选择因动作动态_(
        动态节点类* 因动态,
        const std::vector<动态节点类*>& 过程动态列表) noexcept
    {
        auto 打分 = [&](动态节点类* 动态) noexcept -> int {
            if (!是否显式动作动态_(动态)) return -1;
            auto* mi = 取动态主信息_(动态);
            if (!mi) return -1;
            switch (mi->来源动作相位) {
            case 枚举_动作事件相位::开始运行:
                return 3;
            case 枚举_动作事件相位::未定义:
                return 2;
            case 枚举_动作事件相位::完成:
            case 枚举_动作事件相位::失败:
                return 1;
            default:
                return 0;
            }
        };

        动态节点类* 最优 = nullptr;
        int 最优分 = -1;
        auto 更新最优 = [&](动态节点类* 动态) noexcept {
            const int 分 = 打分(动态);
            if (分 > 最优分) {
                最优 = 动态;
                最优分 = 分;
            }
        };

        更新最优(因动态);
        for (auto* 动态 : 过程动态列表) {
            更新最优(动态);
        }
        if (最优) return 最优;
        if (因动态) return 因动态;
        return 过程动态列表.empty() ? nullptr : 过程动态列表.front();
    }

    static std::vector<状态节点类*> 规约条件状态列表_(
        const std::vector<状态节点类*>& 原始条件状态列表,
        状态节点类* 结果状态)
    {
        const auto 结果特征文本 = 取状态特征文本_(结果状态);
        std::unordered_map<std::string, 状态节点类*> 最新状态映射{};
        for (auto* 状态 : 原始条件状态列表) {
            const auto 特征文本 = 取状态特征文本_(状态);
            if (是否原始时间状态特征文本(特征文本)) continue;
            const auto 规约键 = 生成状态规约键_(状态);
            if (规约键.empty()) continue;
            // 允许“同主体同特征”的前态进入条件集，支持同特征传导型因果；
            // 这里只排除与结果完全同一个状态节点，避免同一节点同时落入因和果。
            if (状态 == 结果状态) continue;
            auto it = 最新状态映射.find(规约键);
            if (it == 最新状态映射.end()) {
                最新状态映射.emplace(规约键, 状态);
                continue;
            }
            if (取状态时间_(状态) >= 取状态时间_(it->second)) {
                it->second = 状态;
            }
        }

        std::vector<状态节点类*> out{};
        out.reserve(最新状态映射.size());
        for (const auto& [_, 状态] : 最新状态映射) {
            if (状态) out.push_back(状态);
        }

        const bool 结果是自我核心特征 = 是否自我核心特征文本_(结果特征文本);
        const bool 存在自我核心条件 = std::any_of(out.begin(), out.end(), [](const 状态节点类* 状态) {
            return 是否自我核心特征文本_(取状态特征文本_(状态));
        });
        if (结果是自我核心特征 || 存在自我核心条件) {
            std::vector<状态节点类*> 去技术条件后{};
            去技术条件后.reserve(out.size());
            for (auto* 状态 : out) {
                if (!状态) continue;
                const auto 特征文本 = 取状态特征文本_(状态);
                if (是否技术性条件特征文本_(特征文本)) continue;
                去技术条件后.push_back(状态);
            }
            if (!去技术条件后.empty()) {
                out = std::move(去技术条件后);
            }
        }

        排序状态按时间_(out);
        return out;
    }

    static void 补足同特征前态条件_(
        const 场景节点类* 场景,
        状态节点类* 结果状态,
        std::vector<状态节点类*>& 条件状态列表) noexcept
    {
        if (!场景 || !结果状态) return;
        auto* 前状态 = 查找前一同特征状态_(场景, 结果状态);
        if (!前状态) return;
        追加唯一状态_(条件状态列表, 前状态);
    }

    static std::string 生成因果键_(
        const 因果实例主信息类& 规范实例,
        const std::vector<状态节点类*>& 条件状态列表,
        const std::vector<std::uint8_t>& 条件方向可比较列表,
        const std::vector<std::int64_t>& 条件方向值列表,
        动态节点类* 因动作动态,
        状态节点类* 结果状态,
        bool 结果方向可比较,
        std::int64_t 结果方向值) noexcept
    {
        if (条件状态列表.empty() || !结果状态) return {};
        if (条件状态列表.size() != 条件方向可比较列表.size()) return {};
        if (条件状态列表.size() != 条件方向值列表.size()) return {};

        std::string key = 生成因果作用域键_(规范实例);
        for (std::size_t i = 0; i < 条件状态列表.size(); ++i) {
            key += "cond[";
            key += 生成状态因果片段_(条件状态列表[i], 条件方向可比较列表[i] != 0, 条件方向值列表[i]);
            key += "]";
        }
        key += "|actor=";
        key += 规范实例.动作主体 ? 生成节点稳定键_(规范实例.动作主体) : std::string{"0"};
        key += "|semantic=";
        key += 生成动作语义累计键_(规范实例, 因动作动态);
        key += "|result[";
        key += 生成状态因果片段_(结果状态, 结果方向可比较, 结果方向值);
        key += "]";
        return key;
    }

    static std::string 生成动态同型键_(const 动态节点类* d) noexcept
    {
        auto* mi = 取动态主信息_(d);
        if (!mi) return {};
        std::string key = std::to_string(reinterpret_cast<std::uintptr_t>(mi->动态主体)) + "|" +
            std::to_string(reinterpret_cast<std::uintptr_t>(mi->动态特征)) + "|" +
            std::to_string(mi->动态路径签名);
        if (mi->来源类型 == 枚举_动态来源类型::方法调用) {
            key += "|source=method";
            if (mi->来源动作名) {
                key += "|action=";
                key += mi->来源动作名->获取主键();
            }
            else if (mi->来源方法首节点) {
                key += "|method=";
                key += std::to_string(reinterpret_cast<std::uintptr_t>(mi->来源方法首节点));
            }
            key += "|ok=";
            key += mi->来源执行成功 ? "1" : "0";
        }
        return key;
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

    static void 排序因果按稳定度_(std::vector<基础信息节点类*>& 列表)
    {
        std::sort(列表.begin(), 列表.end(), [](const 基础信息节点类* a, const 基础信息节点类* b) {
            const auto* ami = (a && a->主信息) ? dynamic_cast<因果主信息类*>(a->主信息) : nullptr;
            const auto* bmi = (b && b->主信息) ? dynamic_cast<因果主信息类*>(b->主信息) : nullptr;
            const auto aq = ami ? ami->稳定度Q : 0;
            const auto bq = bmi ? bmi->稳定度Q : 0;
            if (aq != bq) return aq > bq;
            const auto at = ami ? ami->最近发生时间 : 0;
            const auto bt = bmi ? bmi->最近发生时间 : 0;
            return at > bt;
        });
    }

    static const std::vector<动态节点类*>* 取时序动态源_(const 场景节点主信息类* smi) noexcept
    {
        if (!smi) return nullptr;
        if (!smi->事件动态列表.empty()) return &smi->事件动态列表;
        return &smi->动态列表;
    }

    static 动态节点类* 尝试映射到时序锚点_(
        const 场景节点主信息类* smi,
        动态节点类* 锚点动态) noexcept
    {
        if (!smi || !锚点动态) return 锚点动态;
        const auto* 动态源 = 取时序动态源_(smi);
        if (!动态源 || 动态源->empty()) return 锚点动态;
        if (std::find(动态源->begin(), 动态源->end(), 锚点动态) != 动态源->end()) return 锚点动态;

        auto* 锚点mi = 取动态主信息_(锚点动态);
        if (!锚点mi || !锚点mi->动态主体) return 锚点动态;

        const auto 锚点开始 = 取动态开始时间_(锚点动态);
        const auto 锚点结束 = 取动态结束时间_(锚点动态);
        动态节点类* 最佳 = nullptr;
        时间戳 最佳结束时间 = 0;

        for (auto* d : *动态源) {
            auto* mi = 取动态主信息_(d);
            if (!mi || mi->动态主体 != 锚点mi->动态主体) continue;
            if (锚点mi->动态特征 && mi->动态特征 && mi->动态特征 != 锚点mi->动态特征) continue;

            const auto 开始 = 取动态开始时间_(d);
            const auto 结束 = 取动态结束时间_(d);
            const bool 时间相交 = !(结束 < 锚点开始 || 开始 > 锚点结束);
            const bool 早于锚点结束 = (锚点结束 == 0) || (结束 <= 锚点结束);
            if (!时间相交 && !早于锚点结束) continue;

            if (!最佳 || 结束 >= 最佳结束时间) {
                最佳 = d;
                最佳结束时间 = 结束;
            }
        }
        return 最佳 ? 最佳 : 锚点动态;
    }

    void 登记反向因果索引_(
        基础信息节点类* node,
        const 因果实例主信息类& 规范实例)
    {
        if (!node) return;
        if (const auto 键 = 生成来源方法索引键_(规范实例); !键.empty()) {
            auto& vec = 因果索引_来源方法_[键];
            追加唯一因果_(vec, node);
        }
        if (const auto 键 = 生成来源任务索引键_(规范实例); !键.empty()) {
            auto& vec = 因果索引_来源任务_[键];
            追加唯一因果_(vec, node);
        }
        if (const auto 键 = 生成动作主体索引键_(规范实例); !键.empty()) {
            auto& vec = 因果索引_动作主体_[键];
            追加唯一因果_(vec, node);
        }
        if (const auto 键 = 生成动作语义索引键_(规范实例); !键.empty()) {
            auto& vec = 因果索引_动作语义_[键];
            追加唯一因果_(vec, node);
        }
        if (!规范实例.结果状态主信息列表.empty()) {
            for (const auto& 结果状态 : 规范实例.结果状态主信息列表) {
                const auto 键 = 生成结果特征索引键_(规范实例, &结果状态);
                if (键.empty()) continue;
                auto& vec = 因果索引_结果特征_[键];
                追加唯一因果_(vec, node);
            }
            return;
        }
        for (auto* 结果状态 : 规范实例.结果状态列表) {
            const auto 键 = 生成结果特征索引键_(规范实例, 结果状态);
            if (键.empty()) continue;
            auto& vec = 因果索引_结果特征_[键];
            追加唯一因果_(vec, node);
        }
    }

    static void 写入因果观测字段_(
        因果主信息类* mi,
        const 因果实例主信息类& 规范实例,
        动态节点类* 因动作动态,
        动态节点类* 果动态,
        const std::vector<二次特征节点类*>& 条件方向二次特征列表,
        const std::vector<std::int64_t>& 条件方向值列表,
        const std::vector<std::uint8_t>& 条件方向可比较列表,
        const std::vector<动态节点类*>& 过程动态列表,
        状态节点类* 结果状态,
        const 结构_状态方向摘要& 结果方向)
    {
        if (!mi) return;
        写入实例因果规范字段_(mi, 规范实例);
        mi->因信息 = 因动作动态;
        mi->果信息 = 果动态;
        mi->锚点动态 = 果动态 ? 果动态 : 因动作动态;
        mi->条件方向二次特征列表 = 条件方向二次特征列表;
        mi->条件方向值列表 = 条件方向值列表;
        mi->条件方向可比较列表 = 条件方向可比较列表;
        mi->过程动态列表 = 过程动态列表;
        mi->结果状态 = 结果状态;
        mi->结果方向二次特征 = 结果方向.方向二次特征;
        mi->结果方向链键 = 结果方向.方向链键;
        mi->结果方向值 = 结果方向.方向值;
        mi->结果方向可比较 = 结果方向.可比较;
        mi->因果主体 = mi->动作主体;
    }

    static bool 尝试事务式写入因果观测字段_(
        因果主信息类* mi,
        const 因果实例主信息类& 规范实例,
        动态节点类* 因动作动态,
        动态节点类* 果动态,
        const std::vector<二次特征节点类*>& 条件方向二次特征列表,
        const std::vector<std::int64_t>& 条件方向值列表,
        const std::vector<std::uint8_t>& 条件方向可比较列表,
        const std::vector<动态节点类*>& 过程动态列表,
        状态节点类* 结果状态,
        const 结构_状态方向摘要& 结果方向) noexcept
    {
        if (!mi) return false;

        // 先在临时副本上预写并校验，避免命中旧节点时把原实例写脏。
        因果主信息类 临时副本(*mi);
        写入因果观测字段_(
            &临时副本,
            规范实例,
            因动作动态,
            果动态,
            条件方向二次特征列表,
            条件方向值列表,
            条件方向可比较列表,
            过程动态列表,
            结果状态,
            结果方向);
        if (校验因果实例合法性_(临时副本) != 枚举_因果校验结果::合法) {
            return false;
        }

        写入因果观测字段_(
            mi,
            规范实例,
            因动作动态,
            果动态,
            条件方向二次特征列表,
            条件方向值列表,
            条件方向可比较列表,
            过程动态列表,
            结果状态,
            结果方向);
        return true;
    }

    static void 追加唯一模板_(
        std::vector<二次特征节点类*>& 列表,
        二次特征节点类* 节点) noexcept
    {
        if (!节点) return;
        if (std::find(列表.begin(), 列表.end(), 节点) == 列表.end()) {
            列表.push_back(节点);
        }
    }

    static 因果链节点类* 查找因果链节点_(
        基础信息节点类* 宿主节点,
        const std::vector<std::string>& 因果主键序列) noexcept
    {
        if (!宿主节点 || !宿主节点->子 || 因果主键序列.empty()) return nullptr;
        auto* head = 宿主节点->子;
        auto* it = head;
        do {
            auto* mi = (it && it->主信息) ? dynamic_cast<因果链主信息类*>(it->主信息) : nullptr;
            if (mi && mi->因果实例主键序列_存档 == 因果主键序列) {
                return static_cast<因果链节点类*>(it);
            }
            it = it ? it->下 : nullptr;
        } while (it && it != head);
        return nullptr;
    }

    static 时间戳 取实例排序时间_(const 因果主信息类& 实例) noexcept
    {
        if (实例.条件时间) return 实例.条件时间;
        if (实例.动作时间) return 实例.动作时间;
        if (实例.结果时间) return 实例.结果时间;
        return 实例.最近发生时间;
    }

    static bool 实例规范字段齐全_(const 因果实例主信息类& 实例) noexcept
    {
        const bool 有条件状态 = !实例.条件状态主信息列表.empty() || !实例.条件状态列表.empty();
        const bool 有结果状态 = !实例.主结果状态主信息列表.empty()
            || !实例.结果状态主信息列表.empty()
            || !实例.结果状态列表.empty();
        return 实例.锚点类型 != 枚举_因果锚点类型::未定义
            && 实例.锚点主键 != 0
            && 实例.状态层级 != 0
            && 实例.条件时间 != 0
            && 实例.动作时间 != 0
            && 实例.结果时间 != 0
            && 有条件状态
            && (有结果状态 || !实例.结果动态列表.empty())
            && (!实例.主结果状态主信息列表.empty() || 实例.主结果状态 != nullptr || 实例.主结果动态 != nullptr)
            && !实例.动作语义键.empty();
    }

    static bool 实例可提升为模板_(const 因果主信息类& 实例) noexcept
    {
        constexpr std::int64_t 最小模板证据次数 = 3;
        if (实例.是否原始观测实例) return false;
        if (实例.观察次数 < 最小模板证据次数) return false;
        if (!实例规范字段齐全_(实例)) return false;
        if (校验因果实例合法性_(实例) != 枚举_因果校验结果::合法) return false;
        if (实例.条件方向二次特征列表.empty() || !实例.结果方向二次特征) return false;
        return true;
    }

    static bool 实例可纳入因果链_(const 因果主信息类& 实例) noexcept
    {
        if (实例.是否原始观测实例) return false;
        if (!实例规范字段齐全_(实例)) return false;
        return 校验因果实例合法性_(实例) == 枚举_因果校验结果::合法;
    }

    static std::string 计算草案动作语义键_(const 结构_因果链草案& 草案) noexcept
    {
        if (草案.动作语义) {
            return std::string{"node:"} + 生成节点稳定键_(草案.动作语义);
        }
        const auto 动态键 = 生成因动作动态键_(草案.因动态 ? 草案.因动态 : 草案.果动态);
        return 动态键.empty() ? std::string{} : (std::string{"dynamic:"} + std::to_string(计算文本签名值_(动态键)));
    }

    static bool 实例命中跨层草案_(
        const 因果主信息类& 实例,
        const 结构_因果链草案& 草案) noexcept
    {
        if (!实例可纳入因果链_(实例)) return false;

        constexpr 时间戳 最大桥接间隔 = 30ll * 1000ll * 1000ll; // 30 秒
        const auto 实例时间 = 实例.结果时间 ? 实例.结果时间 : 取实例排序时间_(实例);
        if (草案.记录时间 != 0 && 实例时间 != 0) {
            const auto dt = (实例时间 > 草案.记录时间) ? (实例时间 - 草案.记录时间) : (草案.记录时间 - 实例时间);
            if (dt > 最大桥接间隔) return false;
        }

        const auto 草案语义键 = 计算草案动作语义键_(草案);
        if (!草案语义键.empty() && !实例.动作语义键.empty() && 草案语义键 != 实例.动作语义键) {
            return false;
        }
        if (草案语义键.empty() && 草案.动作主体 && 实例.动作主体 && 草案.动作主体 != 实例.动作主体) {
            return false;
        }

        if (草案.推断锚点主键 != 0 && 实例.锚点主键 == 草案.推断锚点主键) {
            if (草案.条件层级 != 0 && 实例.状态层级 == 草案.条件层级 && 草案.结果层级 != 0 && 草案.条件层级 == 草案.结果层级) {
                return false;
            }
        }
        return true;
    }

    static std::string 生成状态桥接键_(const 状态节点类* 状态) noexcept
    {
        if (!状态) return {};
        auto* 主信息 = 状态->主信息 ? dynamic_cast<状态节点主信息类*>(状态->主信息) : nullptr;
        if (!主信息 || !主信息->状态主体 || !主信息->状态特征) return {};
        return std::string{"subject="} + 生成状态主体稳定键_(状态)
            + "|feature=" + 生成状态特征稳定键_(状态) + "|";
    }

    static std::string 生成状态桥接键_(const 状态节点主信息类* 状态) noexcept
    {
        if (!状态 || !状态->状态主体 || !状态->状态特征) return {};
        return std::string{"subject="} + 生成状态主体稳定键_(状态)
            + "|feature=" + 生成状态特征稳定键_(状态) + "|";
    }

    static std::vector<std::string> 收集实例条件桥接键_(
        const 因果实例主信息类& 实例)
    {
        std::vector<std::string> out{};
        const auto 预估数量 = !实例.条件状态主信息列表.empty()
            ? 实例.条件状态主信息列表.size()
            : 实例.条件状态列表.size();
        out.reserve(预估数量);
        遍历实例条件状态主信息_(实例, [&](const 状态节点主信息类& 状态) {
            if (const auto 键 = 生成状态桥接键_(&状态); !键.empty()) {
                if (std::find(out.begin(), out.end(), 键) == out.end()) {
                    out.push_back(键);
                }
            }
        });
        return out;
    }

    static std::vector<std::string> 收集实例结果桥接键_(
        const 因果实例主信息类& 实例)
    {
        std::vector<std::string> out{};
        if (const auto* 主结果状态 = 取实例主结果状态主信息_(实例)) {
            if (const auto 键 = 生成状态桥接键_(主结果状态); !键.empty()) {
                out.push_back(键);
            }
        }
        遍历实例结果状态主信息_(实例, [&](const 状态节点主信息类& 状态) {
            if (const auto 键 = 生成状态桥接键_(&状态); !键.empty()) {
                if (std::find(out.begin(), out.end(), 键) == out.end()) {
                    out.push_back(键);
                }
            }
        });
        return out;
    }

    static bool 实例之间可桥接_(
        const 因果实例主信息类& 前,
        const 因果实例主信息类& 后) noexcept
    {
        const auto 前时间 = 前.结果时间 ? 前.结果时间 : (前.动作时间 ? 前.动作时间 : 前.条件时间);
        const auto 后时间 = 后.条件时间 ? 后.条件时间 : (后.动作时间 ? 后.动作时间 : 后.结果时间);
        if (前时间 != 0 && 后时间 != 0 && 前时间 > 后时间) return false;

        const auto 结果键 = 收集实例结果桥接键_(前);
        const auto 条件键 = 收集实例条件桥接键_(后);
        if (结果键.empty() || 条件键.empty()) return false;
        for (const auto& 左 : 结果键) {
            if (std::find(条件键.begin(), 条件键.end(), 左) != 条件键.end()) {
                return true;
            }
        }
        return false;
    }

    基础信息节点类* 尝试根据锚点实例组织因果链_(
        基础信息节点类* 锚点,
        基础信息节点类* 新实例节点,
        const std::string& 调用点)
    {
        if (!锚点 || !新实例节点) return nullptr;
        auto* 锚点实例列表 = 取锚点实例因果列表_(锚点);
        if (!锚点实例列表 || 锚点实例列表->size() < 2) return nullptr;

        std::vector<基础信息节点类*> 有序实例{};
        有序实例.reserve(锚点实例列表->size());
        for (auto* 节点 : *锚点实例列表) {
            auto* mi = (节点 && 节点->主信息) ? dynamic_cast<因果主信息类*>(节点->主信息) : nullptr;
            if (mi && 实例可纳入因果链_(*mi)) {
                有序实例.push_back(节点);
            }
        }
        if (有序实例.size() < 2) return nullptr;

        std::sort(有序实例.begin(), 有序实例.end(), [&](基础信息节点类* a, 基础信息节点类* b) {
            auto* ami = (a && a->主信息) ? dynamic_cast<因果主信息类*>(a->主信息) : nullptr;
            auto* bmi = (b && b->主信息) ? dynamic_cast<因果主信息类*>(b->主信息) : nullptr;
            const auto at = ami ? 取实例排序时间_(*ami) : 0;
            const auto bt = bmi ? 取实例排序时间_(*bmi) : 0;
            if (at != bt) return at < bt;
            return a->获取主键() < b->获取主键();
        });

        auto itNew = std::find(有序实例.begin(), 有序实例.end(), 新实例节点);
        if (itNew == 有序实例.end()) return nullptr;
        const std::size_t 新索引 = static_cast<std::size_t>(std::distance(有序实例.begin(), itNew));

        std::vector<基础信息节点类*> 序列{ 新实例节点 };
        std::size_t 当前前索引 = 新索引;
        while (当前前索引 > 0) {
            bool 命中 = false;
            for (std::size_t i = 当前前索引; i-- > 0;) {
                auto* 前 = dynamic_cast<因果实例主信息类*>(有序实例[i]->主信息);
                auto* 后 = dynamic_cast<因果实例主信息类*>(序列.front()->主信息);
                if (!前 || !后) continue;
                if (!实例之间可桥接_(*前, *后)) continue;
                序列.insert(序列.begin(), 有序实例[i]);
                当前前索引 = i;
                命中 = true;
                break;
            }
            if (!命中) break;
        }

        std::size_t 当前后索引 = 新索引;
        while (当前后索引 + 1 < 有序实例.size()) {
            bool 命中 = false;
            for (std::size_t i = 当前后索引 + 1; i < 有序实例.size(); ++i) {
                auto* 前 = dynamic_cast<因果实例主信息类*>(序列.back()->主信息);
                auto* 后 = dynamic_cast<因果实例主信息类*>(有序实例[i]->主信息);
                if (!前 || !后) continue;
                if (!实例之间可桥接_(*前, *后)) continue;
                序列.push_back(有序实例[i]);
                当前后索引 = i;
                命中 = true;
                break;
            }
            if (!命中) break;
        }

        if (序列.size() < 2) return nullptr;
        return 组织因果链_(序列, 调用点 + "/本地桥接");
    }

    基础信息节点类* 组织因果链_(
        const std::vector<基础信息节点类*>& 输入因果集,
        const std::string& /*调用点*/ = "因果类::组织因果链")
    {
        std::vector<基础信息节点类*> 序列{};
        for (auto* 节点 : 输入因果集) {
            auto* mi = (节点 && 节点->主信息) ? dynamic_cast<因果主信息类*>(节点->主信息) : nullptr;
            if (!mi || !实例可纳入因果链_(*mi)) continue;
            if (std::find(序列.begin(), 序列.end(), 节点) == 序列.end()) {
                序列.push_back(节点);
            }
        }
        if (序列.size() < 2) return nullptr;

        std::sort(序列.begin(), 序列.end(), [&](基础信息节点类* a, 基础信息节点类* b) {
            auto* ami = (a && a->主信息) ? dynamic_cast<因果主信息类*>(a->主信息) : nullptr;
            auto* bmi = (b && b->主信息) ? dynamic_cast<因果主信息类*>(b->主信息) : nullptr;
            const auto at = ami ? 取实例排序时间_(*ami) : 0;
            const auto bt = bmi ? 取实例排序时间_(*bmi) : 0;
            if (at != bt) return at < bt;
            return a->获取主键() < b->获取主键();
        });

        for (std::size_t i = 1; i < 序列.size(); ++i) {
            auto* 前 = dynamic_cast<因果实例主信息类*>(序列[i - 1]->主信息);
            auto* 后 = dynamic_cast<因果实例主信息类*>(序列[i]->主信息);
            if (!前 || !后) return nullptr;
            if (!实例之间可桥接_(*前, *后)) return nullptr;
        }

        std::vector<std::string> 主键序列{};
        主键序列.reserve(序列.size());
        for (auto* 节点 : 序列) {
            主键序列.push_back(节点 ? 节点->获取主键() : std::string{});
        }

        auto* 宿主节点 = 序列.front() && 序列.front()->父
            ? static_cast<基础信息节点类*>(序列.front()->父)
            : nullptr;
        if (!是否实例因果锚点宿主_(宿主节点)) {
            宿主节点 = 序列.front();
        }
        auto* 链节点 = 查找因果链节点_(宿主节点, 主键序列);
        auto* 链信息 = 链节点 && 链节点->主信息
            ? dynamic_cast<因果链主信息类*>(链节点->主信息)
            : nullptr;
        if (!链信息) {
            链信息 = new 因果链主信息类();
            链节点 = static_cast<因果链节点类*>(添加子节点(宿主节点, static_cast<基础信息基类*>(链信息)));
            if (!链节点) {
                delete 链信息;
                return nullptr;
            }
        }

        链信息->因果实例序列.clear();
        链信息->因果实例主键序列_存档 = 主键序列;
        for (auto* 节点 : 序列) {
            链信息->因果实例序列.push_back(static_cast<因果实例节点类*>(节点));
        }

        auto* 首信息 = dynamic_cast<因果主信息类*>(序列.front()->主信息);
        auto* 末信息 = dynamic_cast<因果主信息类*>(序列.back()->主信息);
        链信息->起点锚点主键 = 首信息 ? 首信息->锚点主键 : 0;
        链信息->终点锚点主键 = 末信息 ? 末信息->锚点主键 : 0;
        链信息->起点锚点主键_存档 = 首信息 ? 首信息->锚点主键_存档 : std::string{};
        链信息->终点锚点主键_存档 = 末信息 ? 末信息->锚点主键_存档 : std::string{};
        链信息->起始时间 = 首信息 ? 取实例排序时间_(*首信息) : 0;
        链信息->结束时间 = 末信息 ? (末信息->结果时间 ? 末信息->结果时间 : 取实例排序时间_(*末信息)) : 0;
        if (是否实例因果锚点宿主_(宿主节点)) {
            if (auto* 列表 = 取锚点因果链列表_(宿主节点)) {
                if (std::find(列表->begin(), 列表->end(), 链节点) == 列表->end()) {
                    列表->push_back(链节点);
                }
            }
            同步锚点因果链存档缓存_(宿主节点);
        }
        return 链节点;
    }

    void 尝试根据跨层草案组织因果链_(
        基础信息节点类* 新实例节点,
        const 因果主信息类& 新实例,
        时间戳 now)
    {
        if (!新实例节点 || 跨层因果链草案队列_.empty() || !实例可纳入因果链_(新实例)) return;

        for (auto it = 跨层因果链草案队列_.rbegin(); it != 跨层因果链草案队列_.rend(); ++it) {
            if (!实例命中跨层草案_(新实例, *it)) continue;

            std::vector<基础信息节点类*> 候选序列{};
            候选序列.push_back(新实例节点);
            for (auto* 节点 = 根指针 ? static_cast<基础信息节点类*>(根指针->链下) : nullptr;
                节点 && 节点 != 根指针;
                节点 = static_cast<基础信息节点类*>(节点->链下)) {
                if (节点 == 新实例节点) continue;
                auto* 父节点 = 节点->父 ? static_cast<基础信息节点类*>(节点->父) : nullptr;
                if (!是否实例因果锚点宿主_(父节点)) continue;
                auto* mi = (节点 && 节点->主信息) ? dynamic_cast<因果主信息类*>(节点->主信息) : nullptr;
                if (!mi) continue;
                if (!实例命中跨层草案_(*mi, *it)) continue;
                if (std::find(候选序列.begin(), 候选序列.end(), 节点) == 候选序列.end()) {
                    候选序列.push_back(节点);
                }
            }

            if (候选序列.size() < 2) continue;
            if (auto* 链节点 = 组织因果链_(候选序列, "因果类::尝试根据跨层草案组织因果链")) {
                const auto 索引 = static_cast<std::size_t>(std::distance(it, 跨层因果链草案队列_.rend())) - 1;
                if (索引 < 跨层因果链草案队列_.size()) {
                    跨层因果链草案队列_.erase(跨层因果链草案队列_.begin() + static_cast<std::ptrdiff_t>(索引));
                }
                (void)链节点;
                return;
            }
        }
        (void)now;
    }

    void 同步因果模板镜像_(
        基础信息节点类* 归纳节点,
        const 因果主信息类& 实例,
        时间戳 now)
    {
        // 新规则：抽象因果只存于因果树，不再挂回实例因果节点。
        (void)归纳节点;
        (void)实例;
        (void)now;
    }

    基础信息节点类* 创建原始观测节点_(
        基础信息节点类* 归纳节点,
        const 因果实例主信息类& 规范实例,
        动态节点类* 因动作动态,
        动态节点类* 果动态,
        const std::vector<二次特征节点类*>& 条件方向二次特征列表,
        const std::vector<std::int64_t>& 条件方向值列表,
        const std::vector<std::uint8_t>& 条件方向可比较列表,
        const std::vector<动态节点类*>& 过程动态列表,
        状态节点类* 结果状态,
        const 结构_状态方向摘要& 结果方向,
        时间戳 now,
        std::int64_t 原始观测序号)
    {
        // 新规则：实例因果直接挂到场景/存在锚点，不再生成原始观测子节点。
        (void)归纳节点;
        (void)规范实例;
        (void)因动作动态;
        (void)果动态;
        (void)条件方向二次特征列表;
        (void)条件方向值列表;
        (void)条件方向可比较列表;
        (void)过程动态列表;
        (void)结果状态;
        (void)结果方向;
        (void)now;
        (void)原始观测序号;
        return nullptr;
    }

    void 记录跨层因果链草案_(
        const 结构_因果草案& 草案,
        枚举_因果校验结果 失败原因,
        时间戳 now = 0) noexcept
    {
        if (失败原因 != 枚举_因果校验结果::跨层直连) return;
        auto* 主结果状态 = 选择结果状态_(草案.果动态, 草案.过程动态列表, 草案.结果状态列表);
        if (!主结果状态) return;

        结构_因果链草案 draft{};
        draft.场景 = 草案.场景;
        draft.失败原因 = 失败原因;
        draft.记录时间 = now ? now : 当前时间_();
        draft.条件状态列表 = 草案.条件状态列表;
        draft.过程动态列表 = 草案.过程动态列表;
        draft.结果状态列表 = 草案.结果状态列表;
        draft.因动态 = 草案.因动态;
        draft.果动态 = 草案.果动态;
        draft.动作主体 = 草案.动作主体;
        draft.动作语义 = 草案.动作语义;

        auto* 锚点信息 = 推断因果锚点信息_(草案.场景, 草案.条件状态列表, 主结果状态, 草案.动作主体);
        draft.推断锚点类型 = 识别因果锚点类型_(锚点信息);
        draft.推断锚点主键 = 计算节点运行期主键_(锚点信息);
        draft.条件层级 = 汇总状态层级_(草案.条件状态列表, 锚点信息, draft.推断锚点类型);
        draft.结果层级 = 汇总状态层级_(草案.结果状态列表, 锚点信息, draft.推断锚点类型);

        constexpr std::size_t 最大草案数 = 128;
        if (跨层因果链草案队列_.size() >= 最大草案数) {
            跨层因果链草案队列_.erase(跨层因果链草案队列_.begin());
        }
        跨层因果链草案队列_.push_back(std::move(draft));
    }

    void 删除引用实例的因果链_(
        const std::vector<std::string>& 实例主键列表,
        const std::string& 调用点 = "因果类::删除引用实例的因果链")
    {
        (void)调用点;
        if (实例主键列表.empty()) return;
        std::unordered_set<std::string> 待删除主键集(实例主键列表.begin(), 实例主键列表.end());
        std::vector<基础信息节点类*> 待删除链节点{};

        for (auto* it = 根指针 ? static_cast<基础信息节点类*>(根指针->链下) : nullptr;
            it && it != 根指针;
            it = static_cast<基础信息节点类*>(it->链下)) {
            auto* 链信息 = (it && it->主信息) ? dynamic_cast<因果链主信息类*>(it->主信息) : nullptr;
            if (!链信息) continue;
            bool 命中 = false;
            for (const auto& 主键 : 链信息->因果实例主键序列_存档) {
                if (待删除主键集.find(主键) != 待删除主键集.end()) {
                    命中 = true;
                    break;
                }
            }
            if (命中) {
                待删除链节点.push_back(it);
            }
        }

        for (auto* 链节点 : 待删除链节点) {
            auto* 宿主 = 链节点 && 链节点->父 ? static_cast<基础信息节点类*>(链节点->父) : nullptr;
            if (是否实例因果锚点宿主_(宿主)) {
                if (auto* 列表 = 取锚点因果链列表_(宿主)) {
                    列表->erase(std::remove(列表->begin(), 列表->end(), static_cast<因果链节点类*>(链节点)), 列表->end());
                }
                if (auto* 存档列表 = 取锚点因果链存档列表_(宿主)) {
                    存档列表->erase(std::remove(存档列表->begin(), 存档列表->end(), 链节点->获取主键()), 存档列表->end());
                }
            }
            删除节点(链节点);
        }
    }

public:
    static 内部方法::结果<因果实例主信息类> 规范化因果草案(
        const 结构_因果草案& 草案,
        时间戳 now = 0) noexcept
    {
        return 规范化因果草案_(草案, now);
    }

    static 枚举_因果校验结果 校验因果实例合法性(const 因果实例主信息类& x) noexcept
    {
        return 校验因果实例合法性_(x);
    }

    void 按特征裁剪锚点实例因果(
        基础信息节点类* 锚点,
        因果实例节点类* 优先保留节点 = nullptr,
        std::size_t 每特征最大保留数 = 50,
        const std::string& 调用点 = "因果类::按特征裁剪锚点实例因果")
    {
        if (!锚点 || 每特征最大保留数 == 0) return;
        auto* 锚点列表 = 取锚点实例因果列表_(锚点);
        auto* 锚点存档列表 = 取锚点实例因果存档列表_(锚点);
        if (!锚点列表 || !锚点存档列表) return;

        auto* 优先信息 = (优先保留节点 && 优先保留节点->主信息)
            ? dynamic_cast<因果实例主信息类*>(优先保留节点->主信息)
            : nullptr;
        auto* 主结果特征 = 取实例主结果特征_(优先信息);
        if (!主结果特征) return;

        std::vector<因果实例节点类*> 同特征实例{};
        同特征实例.reserve(锚点列表->size());
        for (auto* 节点 : *锚点列表) {
            auto* 信息 = (节点 && 节点->主信息)
                ? dynamic_cast<因果实例主信息类*>(节点->主信息)
                : nullptr;
            if (!信息) continue;
            if (取实例主结果特征_(信息) != 主结果特征) continue;
            同特征实例.push_back(节点);
        }
        if (同特征实例.size() <= 每特征最大保留数) return;

        const auto 取排序时间 = [](const 因果实例主信息类* 信息) noexcept -> 时间戳 {
            if (!信息) return 0;
            if (auto* 兼容 = dynamic_cast<const 因果主信息类*>(信息)) {
                if (兼容->最近发生时间 != 0) return 兼容->最近发生时间;
                if (兼容->首次发生时间 != 0) return 兼容->首次发生时间;
            }
            if (信息->结果时间 != 0) return 信息->结果时间;
            if (信息->动作时间 != 0) return 信息->动作时间;
            return 信息->条件时间;
        };

        std::sort(同特征实例.begin(), 同特征实例.end(), [&](因果实例节点类* a, 因果实例节点类* b) {
            if (a == 优先保留节点) return true;
            if (b == 优先保留节点) return false;
            auto* ami = (a && a->主信息) ? dynamic_cast<因果实例主信息类*>(a->主信息) : nullptr;
            auto* bmi = (b && b->主信息) ? dynamic_cast<因果实例主信息类*>(b->主信息) : nullptr;
            const auto a时间 = 取排序时间(ami);
            const auto b时间 = 取排序时间(bmi);
            if (a时间 != b时间) return a时间 > b时间;
            return a < b;
        });

        bool 已裁剪 = false;
        std::vector<std::string> 被裁剪主键列表{};
        for (std::size_t i = 每特征最大保留数; i < 同特征实例.size(); ++i) {
            auto* 待删除节点 = 同特征实例[i];
            if (!待删除节点) continue;
            auto* 待删除信息 = (待删除节点 && 待删除节点->主信息)
                ? dynamic_cast<因果主信息类*>(待删除节点->主信息)
                : nullptr;
            if (!待删除节点->获取主键().empty()) {
                被裁剪主键列表.push_back(待删除节点->获取主键());
            }
            if (待删除信息 && !待删除信息->因果累计键_存档.empty()) {
                因果索引_.erase(待删除信息->因果累计键_存档);
            }
            锚点列表->erase(std::remove(锚点列表->begin(), 锚点列表->end(), 待删除节点), 锚点列表->end());
            const auto 主键 = 待删除节点->获取主键();
            if (!主键.empty()) {
                锚点存档列表->erase(std::remove(锚点存档列表->begin(), 锚点存档列表->end(), 主键), 锚点存档列表->end());
            }
            删除节点(待删除节点);
            已裁剪 = true;
        }

        if (已裁剪) {
            删除引用实例的因果链_(被裁剪主键列表, 调用点 + "/删除引用链");
            重建索引(调用点 + "/重建索引");
        }
    }

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

    基础信息节点类* 同步抽象因果到因果树(
        因果实例节点类* 实例节点,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::同步抽象因果到因果树")
    {
        (void)调用点;
        if (!实例节点 || !实例节点->主信息) return nullptr;
        auto* mi = dynamic_cast<因果实例主信息类*>(实例节点->主信息);
        if (!mi) return nullptr;
        auto* 主结果特征 = 取实例主结果特征_(mi);
        if (!主结果特征) return nullptr;
        if (now == 0) now = 当前时间_();

        auto 同特征实例 = 收集抽象证据实例_(
            mi->锚点类型,
            mi->状态层级,
            mi->状态面签名,
            主结果特征);
        const auto 有效成立次数 = 汇总实例观察次数_(同特征实例);
        if (有效成立次数 < 3) return nullptr;

        if (!因果根_) 初始化因果根();
        if (!因果根_) return nullptr;

        const auto 抽象键 = 生成抽象因果键_(mi->锚点类型, mi->状态层级, mi->状态面签名, 主结果特征);
        基础信息节点类* 模板节点 = nullptr;
        for (auto* 节点 = 因果根_ ? static_cast<基础信息节点类*>(因果根_->子) : nullptr;
            节点 && 因果根_ && 节点 != 因果根_;
            节点 = (节点->下 && 节点->下 != 因果根_->子) ? static_cast<基础信息节点类*>(节点->下) : nullptr) {
            auto* t = 节点->主信息 ? dynamic_cast<因果模板主信息类*>(节点->主信息) : nullptr;
            if (!t) continue;
            if (生成抽象模板签名键_(*t) == 抽象键) {
                模板节点 = 节点;
                break;
            }
        }

        auto 同步模板 = [&](因果模板主信息类* t) {
            std::vector<二次特征节点类*> 条件模板列表{};
            std::vector<二次特征节点类*> 结果模板列表{};
            std::unordered_map<基础信息节点类*, std::size_t> 动作模板频次{};
            std::unordered_map<std::string, std::uint64_t> 条件摘要频次{};
            std::unordered_map<std::string, std::uint64_t> 结果摘要频次{};
            const auto 反例实例 = 收集抽象反例实例_(
                mi->锚点类型,
                mi->状态层级,
                mi->状态面签名,
                主结果特征,
                mi->动作语义键);
            for (auto* 证据节点 : 同特征实例) {
                auto* 证据 = (证据节点 && 证据节点->主信息)
                    ? dynamic_cast<因果主信息类*>(证据节点->主信息)
                    : nullptr;
                if (!证据) continue;
                const auto 观察次数 = 取实例观察次数_(证据节点);
                for (auto* 条件模板 : 证据->条件方向二次特征列表) {
                    追加唯一模板_(条件模板列表, 条件模板);
                }
                遍历实例条件状态主信息_(*证据, [&](const 状态节点主信息类& 状态) {
                    const auto 特征文本 = 取状态特征文本_(&状态);
                    if (特征文本.empty() || 是否技术性条件特征文本_(特征文本)) return;
                    const auto 摘要 = 生成状态快照摘要_(&状态);
                    if (!摘要.empty()) {
                        条件摘要频次[摘要] += 观察次数;
                    }
                });
                if (证据->结果方向二次特征) {
                    追加唯一模板_(结果模板列表, 证据->结果方向二次特征);
                }
                if (const auto* 主结果状态 = 取实例主结果状态主信息_(*证据)) {
                    const auto 结果摘要 = 生成状态快照摘要_(主结果状态);
                    if (!结果摘要.empty()) {
                        结果摘要频次[结果摘要] += 观察次数;
                    }
                }
                if (证据->动作语义) {
                    动作模板频次[证据->动作语义] += static_cast<std::size_t>(观察次数);
                }
            }

            基础信息节点类* 最优动作模板 = nullptr;
            std::size_t 最优动作频次 = 0;
            for (const auto& [动作模板, 频次] : 动作模板频次) {
                if (!最优动作模板 || 频次 > 最优动作频次) {
                    最优动作模板 = 动作模板;
                    最优动作频次 = 频次;
                }
            }

            const auto 代表证据 = 选取代表证据实例_(同特征实例, 8);
            const auto 代表反例 = 选取代表证据实例_(反例实例, 8);
            时间戳 最近命中时间 = 0;
            for (auto* 节点 : 同特征实例) {
                最近命中时间 = (std::max)(最近命中时间, 取实例最近发生时间_(节点));
            }
            时间戳 最近失败时间 = 0;
            for (auto* 节点 : 反例实例) {
                最近失败时间 = (std::max)(最近失败时间, 取实例最近发生时间_(节点));
            }
            const auto 成立次数 = 汇总实例观察次数_(同特征实例);
            const auto 失败次数 = 汇总实例观察次数_(反例实例);
            const auto 证据次数 = 成立次数 + 失败次数;
            const auto 成立率Q = 证据次数 == 0
                ? 0ll
                : static_cast<std::int64_t>((成立次数 * 10000ull) / 证据次数);
            const auto 规模Q = static_cast<std::int64_t>((std::min)(证据次数, 100ull) * 50ull);

            t->适用锚点类型 = mi->锚点类型;
            t->适用层级 = mi->状态层级;
            t->适用状态面签名 = mi->状态面签名;
            t->条件模板列表 = std::move(条件模板列表);
            t->动作模板 = 最优动作模板 ? 最优动作模板 : mi->动作语义;
            t->结果模板列表 = std::move(结果模板列表);
            t->证据实例列表 = 同特征实例;
            t->代表证据列表 = 代表证据;
            t->反例实例列表 = 代表反例;
            t->证据次数 = 证据次数;
            t->成立次数 = 成立次数;
            t->失败次数 = 失败次数;
            t->最近命中时间 = 最近命中时间;
            t->最近失败时间 = 最近失败时间;
            t->稳定度 = 成立率Q + 规模Q;
            t->典型条件方向摘要 = 生成模板摘要_(t->条件模板列表);
            if (t->典型条件方向摘要.empty()) {
                t->典型条件方向摘要 = 汇总高频摘要文本_(条件摘要频次);
            }
            t->典型动作语义摘要 = 生成动作模板摘要_(t->动作模板);
            t->典型结果方向摘要 = 生成模板摘要_(t->结果模板列表);
            if (t->典型结果方向摘要.empty()) {
                t->典型结果方向摘要 = 汇总高频摘要文本_(结果摘要频次, 2);
            }
            t->最常见失败边界摘要 = 生成失败边界摘要_(反例实例);
            同步因果模板存档缓存_(t);
        };

        if (!模板节点) {
            auto* t = new 因果模板主信息类();
            同步模板(t);
            模板节点 = 添加子节点(因果根_, static_cast<基础信息基类*>(t));
            if (auto* 模板信息 = 模板节点 && 模板节点->主信息
                ? dynamic_cast<因果模板主信息类*>(模板节点->主信息)
                : nullptr) {
                登记抽象因果索引_(模板节点, *模板信息);
            }
            return 模板节点;
        }

        if (auto* t = dynamic_cast<因果模板主信息类*>(模板节点->主信息)) {
            同步模板(t);
            登记抽象因果索引_(模板节点, *t);
        }
        return 模板节点;
    }

    基础信息节点类* 存储实例因果到锚点(
        基础信息节点类* 锚点,
        因果实例主信息类* 主信息,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::存储实例因果到锚点")
    {
        (void)调用点;
        if (!锚点 || !主信息) return nullptr;
        if (!是否实例因果锚点宿主_(锚点)) return nullptr;
        auto* 锚点列表 = 取锚点实例因果列表_(锚点);
        auto* 锚点存档列表 = 取锚点实例因果存档列表_(锚点);
        if (!锚点列表 || !锚点存档列表) return nullptr;

        主信息->锚点信息 = 锚点;
        主信息->锚点类型 = 识别因果锚点类型_(锚点);
        同步实例因果存档缓存_(主信息);

        auto* 节点 = 添加子节点(锚点, static_cast<基础信息基类*>(主信息));
        if (!节点) return nullptr;

        auto* 实例节点 = static_cast<因果实例节点类*>(节点);
        if (std::find(锚点列表->begin(), 锚点列表->end(), 实例节点) == 锚点列表->end()) {
            锚点列表->push_back(实例节点);
        }
        if (!节点->获取主键().empty()
            && std::find(锚点存档列表->begin(), 锚点存档列表->end(), 节点->获取主键()) == 锚点存档列表->end()) {
            锚点存档列表->push_back(节点->获取主键());
        }
        if (auto* 兼容 = dynamic_cast<因果主信息类*>(主信息)) {
            兼容->结构类型 = 枚举_因果结构类型::实例;
            if (兼容->首次发生时间 == 0) 兼容->首次发生时间 = now;
            if (兼容->最近发生时间 == 0) 兼容->最近发生时间 = now;
            if (兼容->观察次数 == 0) 兼容->观察次数 = 1;
            兼容->是否原始观测实例 = false;
            兼容->原始观测序号 = 0;
            兼容->归纳因果节点 = nullptr;
            if (兼容->因果累计键_存档.empty()) {
                兼容->因果累计键_存档 = 节点->获取主键();
            }
            兼容->稳定度Q = 估计稳定度Q_(兼容);
            if (!兼容->因果累计键_存档.empty()) {
                因果索引_[兼容->因果累计键_存档] = 节点;
            }
        }
        按特征裁剪锚点实例因果(锚点, 实例节点, 50, 调用点 + "/按特征裁剪");
        登记反向因果索引_(节点, *static_cast<因果主信息类*>(主信息));
        同步抽象因果到因果树(实例节点, now, 调用点 + "/抽象");
        尝试根据锚点实例组织因果链_(锚点, 实例节点, 调用点 + "/链");
        if (auto* 兼容 = dynamic_cast<因果主信息类*>(主信息)) {
            尝试根据跨层草案组织因果链_(实例节点, *兼容, now);
        }
        return 节点;
    }

    bool 保存到文件(const std::string& 文件名) const
    {
        读锁守卫 lk(链表锁);
        return 保存到文件_已加锁(文件名);
    }

    bool 从文件载入(
        const std::string& 文件名,
        const std::string& 调用点 = "因果类::从文件载入")
    {
        {
            写锁守卫 lk(链表锁);
            if (!从文件载入_已加锁(文件名)) return false;
        }
        重建索引(调用点 + "/重建索引");
        return true;
    }

    void 重建因果索引(const std::string& 调用点 = "因果类::重建因果索引")
    {
        重建索引(调用点);
    }

    基础信息节点类* 组织因果链(
        const std::vector<基础信息节点类*>& 输入因果集,
        const std::string& 调用点 = "因果类::组织因果链")
    {
        return 组织因果链_(输入因果集, 调用点);
    }

    void 重建索引(const std::string& 调用点 = "因果类::重建索引")
    {
        using namespace 数据仓库模块;
        (void)调用点;

        因果索引_.clear();
        因果索引_结果特征_.clear();
        因果索引_动作主体_.clear();
        因果索引_动作语义_.clear();
        因果索引_来源方法_.clear();
        因果索引_来源任务_.clear();
        抽象因果索引_.clear();
        抽象因果索引_结果特征_.clear();
        抽象因果索引_动作模板_.clear();

        std::unordered_map<std::string, 基础信息节点类*> 因果节点按主键_{};

        if (!因果根_ || 因果根_->父 != 根指针) {
            因果根_ = nullptr;
            for (auto* it = 根指针 ? 根指针->链下 : nullptr; it && it != 根指针; it = it->链下) {
                auto* mi = (it && it->主信息) ? dynamic_cast<因果主信息类*>(it->主信息) : nullptr;
                if (!mi) continue;
                if (it->父 == 根指针) {
                    因果根_ = it;
                    break;
                }
            }
        }

        auto 从世界链解析节点 = [&](const std::string& 主键) noexcept -> 基础信息节点类* {
            return 主键.empty() ? nullptr : 世界链.查找主键(主键);
        };

        for (auto* it = 根指针 ? 根指针->链下 : nullptr; it && it != 根指针; it = it->链下) {
            if (auto* smi = (it && it->主信息) ? dynamic_cast<场景节点主信息类*>(it->主信息) : nullptr) {
                smi->实例因果列表.clear();
                smi->实例因果主键列表_存档.clear();
                smi->因果链列表.clear();
                smi->因果链主键列表_存档.clear();
                continue;
            }
            if (auto* emi = (it && it->主信息) ? dynamic_cast<存在节点主信息类*>(it->主信息) : nullptr) {
                emi->实例因果列表.clear();
                emi->实例因果主键列表_存档.clear();
                emi->因果链列表.clear();
                emi->因果链主键列表_存档.clear();
                continue;
            }
        }

        for (auto* it = 根指针 ? 根指针->链下 : nullptr; it && it != 根指针; it = it->链下) {
            if (!it->获取主键().empty()) {
                因果节点按主键_[it->获取主键()] = it;
            }
        }

        auto 修复实例因果引用 = [&](因果主信息类* mi) {
            if (!mi) return;

            if (!mi->锚点信息 && !mi->锚点主键_存档.empty()) {
                mi->锚点信息 = 从世界链解析节点(mi->锚点主键_存档);
            }
            if (mi->条件状态列表.empty() && !mi->条件状态主键列表_存档.empty()) {
                for (const auto& 主键 : mi->条件状态主键列表_存档) {
                    if (auto* 节点 = 从世界链解析节点(主键)) {
                        mi->条件状态列表.push_back(static_cast<状态节点类*>(节点));
                    }
                }
            }
            if (!mi->动作主体 && !mi->动作主体主键_存档.empty()) {
                mi->动作主体 = 从世界链解析节点(mi->动作主体主键_存档);
            }
            if (!mi->动作语义 && !mi->动作语义主键_存档.empty()) {
                mi->动作语义 = 从世界链解析节点(mi->动作语义主键_存档);
            }
            if (!mi->触发动态 && !mi->触发动态主键_存档.empty()) {
                mi->触发动态 = static_cast<动态节点类*>(从世界链解析节点(mi->触发动态主键_存档));
            }
            if (!mi->动作动态 && mi->触发动态) {
                mi->动作动态 = mi->触发动态;
            }
            if (mi->结果状态列表.empty() && !mi->结果状态主键列表_存档.empty()) {
                for (const auto& 主键 : mi->结果状态主键列表_存档) {
                    if (auto* 节点 = 从世界链解析节点(主键)) {
                        mi->结果状态列表.push_back(static_cast<状态节点类*>(节点));
                    }
                }
            }
            if (!mi->主结果状态 && !mi->主结果状态主键_存档.empty()) {
                if (auto* 节点 = 从世界链解析节点(mi->主结果状态主键_存档)) {
                    mi->主结果状态 = static_cast<状态节点类*>(节点);
                }
            }
            if (!mi->结果状态 && mi->主结果状态) {
                mi->结果状态 = mi->主结果状态;
            }
            if (!mi->主结果状态 && mi->结果状态) {
                mi->主结果状态 = mi->结果状态;
            }
            if (mi->结果动态列表.empty() && !mi->结果动态主键列表_存档.empty()) {
                for (const auto& 主键 : mi->结果动态主键列表_存档) {
                    if (auto* 节点 = 从世界链解析节点(主键)) {
                        mi->结果动态列表.push_back(static_cast<动态节点类*>(节点));
                    }
                }
            }
            if (!mi->主结果动态 && !mi->主结果动态主键_存档.empty()) {
                if (auto* 节点 = 从世界链解析节点(mi->主结果动态主键_存档)) {
                    mi->主结果动态 = static_cast<动态节点类*>(节点);
                }
            }
            同步实例因果存档缓存_(mi);
        };

        auto 修复模板引用 = [&](因果模板主信息类* mi) {
            if (!mi) return;
            if (!mi->条件模板主键列表_存档.empty()) {
                mi->条件模板列表.clear();
                for (const auto& 主键 : mi->条件模板主键列表_存档) {
                    if (auto* 节点 = 从世界链解析节点(主键)) {
                        mi->条件模板列表.push_back(static_cast<二次特征节点类*>(节点));
                    }
                }
            }
            if (!mi->动作模板主键_存档.empty()) {
                mi->动作模板 = 从世界链解析节点(mi->动作模板主键_存档);
            }
            if (!mi->结果模板主键列表_存档.empty()) {
                mi->结果模板列表.clear();
                for (const auto& 主键 : mi->结果模板主键列表_存档) {
                    if (auto* 节点 = 从世界链解析节点(主键)) {
                        mi->结果模板列表.push_back(static_cast<二次特征节点类*>(节点));
                    }
                }
            }
            if (!mi->证据实例主键列表_存档.empty()) {
                mi->证据实例列表.clear();
                for (const auto& 主键 : mi->证据实例主键列表_存档) {
                    auto it = 因果节点按主键_.find(主键);
                    if (it != 因果节点按主键_.end()) {
                        mi->证据实例列表.push_back(static_cast<因果实例节点类*>(it->second));
                    }
                }
            }
            if (!mi->代表证据主键列表_存档.empty()) {
                mi->代表证据列表.clear();
                for (const auto& 主键 : mi->代表证据主键列表_存档) {
                    auto it = 因果节点按主键_.find(主键);
                    if (it != 因果节点按主键_.end()) {
                        mi->代表证据列表.push_back(static_cast<因果实例节点类*>(it->second));
                    }
                }
            }
            if (!mi->反例主键列表_存档.empty()) {
                mi->反例实例列表.clear();
                for (const auto& 主键 : mi->反例主键列表_存档) {
                    auto it = 因果节点按主键_.find(主键);
                    if (it != 因果节点按主键_.end()) {
                        mi->反例实例列表.push_back(static_cast<因果实例节点类*>(it->second));
                    }
                }
            }
            同步因果模板存档缓存_(mi);
        };

        auto 修复因果链引用 = [&](因果链主信息类* mi) {
            if (!mi) return;
            if (mi->因果实例序列.empty() && !mi->因果实例主键序列_存档.empty()) {
                for (const auto& 主键 : mi->因果实例主键序列_存档) {
                    auto it = 因果节点按主键_.find(主键);
                    if (it != 因果节点按主键_.end()) {
                        mi->因果实例序列.push_back(static_cast<因果实例节点类*>(it->second));
                    }
                }
            }
            if (mi->起点锚点主键 == 0 && !mi->起点锚点主键_存档.empty()) {
                mi->起点锚点主键 = 解析运行期主键_(mi->起点锚点主键_存档);
            }
            if (mi->终点锚点主键 == 0 && !mi->终点锚点主键_存档.empty()) {
                mi->终点锚点主键 = 解析运行期主键_(mi->终点锚点主键_存档);
            }
        };

        for (auto* it = 根指针 ? 根指针->链下 : nullptr; it && it != 根指针; it = it->链下) {
            if (it == 因果根_) continue;

            if (auto* 模板 = (it && it->主信息) ? dynamic_cast<因果模板主信息类*>(it->主信息) : nullptr) {
                修复模板引用(模板);
                登记抽象因果索引_(it, *模板);
                continue;
            }
            if (auto* 链 = (it && it->主信息) ? dynamic_cast<因果链主信息类*>(it->主信息) : nullptr) {
                修复因果链引用(链);
                auto* 宿主 = it->父 ? static_cast<基础信息节点类*>(it->父) : nullptr;
                if (是否实例因果锚点宿主_(宿主)) {
                    if (auto* 列表 = 取锚点因果链列表_(宿主)) {
                        if (std::find(列表->begin(), 列表->end(), static_cast<因果链节点类*>(it)) == 列表->end()) {
                            列表->push_back(static_cast<因果链节点类*>(it));
                        }
                    }
                    if (auto* 存档列表 = 取锚点因果链存档列表_(宿主)) {
                        if (!it->获取主键().empty()
                            && std::find(存档列表->begin(), 存档列表->end(), it->获取主键()) == 存档列表->end()) {
                            存档列表->push_back(it->获取主键());
                        }
                    }
                }
                continue;
            }

            auto* mi = (it && it->主信息) ? dynamic_cast<因果主信息类*>(it->主信息) : nullptr;
            if (!mi) continue;
            修复实例因果引用(mi);
            auto* 锚点 = mi->锚点信息 ? mi->锚点信息 : (it->父 ? static_cast<基础信息节点类*>(it->父) : nullptr);
            if (!是否实例因果锚点宿主_(锚点)) {
                continue;
            }
            if (auto* 列表 = 取锚点实例因果列表_(锚点)) {
                if (std::find(列表->begin(), 列表->end(), static_cast<因果实例节点类*>(it)) == 列表->end()) {
                    列表->push_back(static_cast<因果实例节点类*>(it));
                }
            }
            if (auto* 存档列表 = 取锚点实例因果存档列表_(锚点)) {
                if (!it->获取主键().empty()
                    && std::find(存档列表->begin(), 存档列表->end(), it->获取主键()) == 存档列表->end()) {
                    存档列表->push_back(it->获取主键());
                }
            }
            if (mi->是否原始观测实例 && !mi->归纳因果节点 && it->父 && it->父 != 因果根_) {
                mi->归纳因果节点 = static_cast<基础信息节点类*>(it->父);
            }
            if (!mi->是否原始观测实例 && !mi->因果累计键_存档.empty()) {
                因果索引_.try_emplace(mi->因果累计键_存档, it);
            }
            if (!mi->是否原始观测实例) {
                登记反向因果索引_(it, *mi);
            }
        }
    }

private:
    bool 保存到文件_已加锁(const std::string& 文件名) const
    {
        std::ofstream ofs(文件名, std::ios::binary);
        if (!ofs || !根指针) return false;

        const std::uint16_t ver = 2;
        写POD_(ofs, 因果存档魔数_);
        写POD_(ofs, ver);

        std::vector<基础信息节点类*> nodes{};
        nodes.reserve(1024);
        nodes.push_back(根指针);
        for (auto* it = static_cast<基础信息节点类*>(根指针->链下); it && it != 根指针; it = static_cast<基础信息节点类*>(it->链下)) {
            nodes.push_back(it);
        }

        const std::uint64_t count = static_cast<std::uint64_t>(nodes.size());
        写POD_(ofs, count);

        std::unordered_map<const 基础信息节点类*, std::uint64_t> idx{};
        idx.reserve(static_cast<std::size_t>(count) * 2);
        for (std::uint64_t i = 0; i < count; ++i) {
            idx[nodes[static_cast<std::size_t>(i)]] = i;
        }

        for (auto* n : nodes) {
            写字符串_(ofs, n ? n->获取主键() : std::string{});
            const std::int64_t childCnt = n ? n->子节点数量 : 0;
            写POD_(ofs, childCnt);

            写POD_(ofs, 索引或空_(idx, n ? static_cast<基础信息节点类*>(n->上) : nullptr));
            写POD_(ofs, 索引或空_(idx, n ? static_cast<基础信息节点类*>(n->下) : nullptr));
            写POD_(ofs, 索引或空_(idx, n ? static_cast<基础信息节点类*>(n->父) : nullptr));
            写POD_(ofs, 索引或空_(idx, n ? static_cast<基础信息节点类*>(n->子) : nullptr));
            写POD_(ofs, 索引或空_(idx, n ? static_cast<基础信息节点类*>(n->根) : nullptr));
            写POD_(ofs, 索引或空_(idx, n ? static_cast<基础信息节点类*>(n->链上) : nullptr));
            写POD_(ofs, 索引或空_(idx, n ? static_cast<基础信息节点类*>(n->链下) : nullptr));

            const std::uint8_t has = (n && n->主信息) ? 1u : 0u;
            写POD_(ofs, has);
            if (has) {
                if (!写入因果主信息_(ofs, n->主信息)) return false;
            }
        }

        return static_cast<bool>(ofs);
    }

    bool 从文件载入_已加锁(const std::string& 文件名)
    {
        std::ifstream ifs(文件名, std::ios::binary);
        if (!ifs) return false;

        std::uint32_t magic = 0;
        std::uint16_t ver = 0;
        if (!读POD_(ifs, magic)) return false;
        if (!读POD_(ifs, ver)) return false;
        if (magic != 因果存档魔数_) return false;
        if (ver != 1 && ver != 2) return false;

        std::uint64_t count = 0;
        if (!读POD_(ifs, count)) return false;
        if (count == 0 || count > 10'000'000ull) return false;

        struct 结构_索引包 {
            std::uint64_t up{}, dn{}, pa{}, ch{}, ro{}, cu{}, cd{};
        };

        std::vector<基础信息节点类*> nodes(count, nullptr);
        std::vector<结构_索引包> links(count);

        for (std::uint64_t i = 0; i < count; ++i) {
            auto* n = new 基础信息节点类{};
            nodes[static_cast<std::size_t>(i)] = n;

            std::string 主键{};
            if (!读字符串_(ifs, 主键)) goto FAIL;
            n->设置主键(主键);
            if (!读POD_(ifs, n->子节点数量)) goto FAIL;

            if (!读POD_(ifs, links[static_cast<std::size_t>(i)].up)) goto FAIL;
            if (!读POD_(ifs, links[static_cast<std::size_t>(i)].dn)) goto FAIL;
            if (!读POD_(ifs, links[static_cast<std::size_t>(i)].pa)) goto FAIL;
            if (!读POD_(ifs, links[static_cast<std::size_t>(i)].ch)) goto FAIL;
            if (!读POD_(ifs, links[static_cast<std::size_t>(i)].ro)) goto FAIL;
            if (!读POD_(ifs, links[static_cast<std::size_t>(i)].cu)) goto FAIL;
            if (!读POD_(ifs, links[static_cast<std::size_t>(i)].cd)) goto FAIL;

            std::uint8_t has = 0;
            if (!读POD_(ifs, has)) goto FAIL;
            if (has) {
                n->主信息 = 读取因果主信息_(ifs, ver);
                if (!n->主信息) goto FAIL;
            }
        }

        for (std::uint64_t i = 0; i < count; ++i) {
            auto* n = nodes[static_cast<std::size_t>(i)];
            const auto& L = links[static_cast<std::size_t>(i)];
            n->上 = 由索引取指针_(nodes, L.up);
            n->下 = 由索引取指针_(nodes, L.dn);
            n->父 = 由索引取指针_(nodes, L.pa);
            n->子 = 由索引取指针_(nodes, L.ch);
            n->根 = 由索引取指针_(nodes, L.ro);
            n->链上 = 由索引取指针_(nodes, L.cu);
            n->链下 = 由索引取指针_(nodes, L.cd);
        }

        删除链表_已加锁();
        根指针 = nodes[0];
        因果根_ = nullptr;
        for (auto* it = static_cast<基础信息节点类*>(根指针->链下); it && it != 根指针; it = static_cast<基础信息节点类*>(it->链下)) {
            auto* mi = (it && it->主信息) ? dynamic_cast<因果主信息类*>(it->主信息) : nullptr;
            if (!mi) continue;
            if (it->父 == 根指针) {
                因果根_ = it;
                break;
            }
        }
        return true;

    FAIL:
        for (auto* n : nodes) {
            if (!n) continue;
            delete n->主信息;
            n->主信息 = nullptr;
            delete n;
        }
        return false;
    }

    基础信息节点类* 创建或累计因果信息_按通道(
        场景节点类* 场景,
        const std::vector<状态节点类*>& 条件状态列表,
        基础信息节点类* 动作主体,
        基础信息节点类* 动作语义,
        const std::vector<动态节点类*>& 过程动态列表,
        const std::vector<状态节点类*>& 结果候选状态列表,
        枚举_因果生成通道 通道,
        动态节点类* 因动态 = nullptr,
        动态节点类* 果动态 = nullptr,
        时间戳 now = 0,
        基础信息节点类* 期望锚点 = nullptr,
        const std::string& /*调用点*/ = "因果类::创建或累计因果信息_按通道")
    {
        if (!场景 || 过程动态列表.empty()) return nullptr;
        if (now == 0) now = 当前时间_();

        结构_因果草案 草案{};
        草案.场景 = 场景;
        草案.条件状态列表 = 条件状态列表;
        草案.过程动态列表 = 过程动态列表;
        草案.结果状态列表 = 结果候选状态列表;
        草案.因动态 = 因动态;
        草案.果动态 = 果动态;
        草案.动作主体 = 动作主体;
        草案.动作语义 = 动作语义;

        枚举_因果校验结果 规范校验结果 = 枚举_因果校验结果::合法;
        const auto 规范结果 = 规范化因果草案_(草案, now, &规范校验结果);
        if (!规范结果.成功) {
            if (规范校验结果 == 枚举_因果校验结果::跨层直连) {
                记录跨层因果链草案_(草案, 规范校验结果, now);
            }
            return nullptr;
        }
        if (推断因果实例通道_(场景, 规范结果.数据) != 通道) return nullptr;
        if (期望锚点 && 规范结果.数据.锚点信息 != 期望锚点) return nullptr;

        auto* 主结果动作动态 = 选择主结果动作动态_(
            规范结果.数据.主结果动态,
            过程动态列表);
        auto* 因动作动态 = 选择因动作动态_(
            规范结果.数据.动作动态 ? 规范结果.数据.动作动态 : 因动态,
            过程动态列表);
        auto* 结果状态 = 规范结果.数据.主结果状态;
        if (!因动作动态 || !结果状态) return nullptr;

        std::vector<二次特征节点类*> 条件方向二次特征列表{};
        std::vector<std::int64_t> 条件方向值列表{};
        std::vector<std::uint8_t> 条件方向可比较列表{};
        条件方向二次特征列表.reserve(规范结果.数据.条件状态列表.size());
        条件方向值列表.reserve(规范结果.数据.条件状态列表.size());
        条件方向可比较列表.reserve(规范结果.数据.条件状态列表.size());
        for (auto* 条件状态 : 规范结果.数据.条件状态列表) {
            const auto 条件方向 = 解析状态方向_(场景, 条件状态);
            条件方向二次特征列表.push_back(条件方向.方向二次特征);
            条件方向值列表.push_back(条件方向.方向值);
            条件方向可比较列表.push_back(条件方向.可比较 ? 1u : 0u);
        }
        const auto 结果方向 = 解析状态方向_(场景, 结果状态);
        const auto 因果累计键 = 生成因果键_(
            规范结果.数据,
            规范结果.数据.条件状态列表,
            条件方向可比较列表,
            条件方向值列表,
            因动作动态,
            结果状态,
            结果方向.可比较,
            结果方向.方向值);

        if (!因果累计键.empty()) {
            auto it = 因果索引_.find(因果累计键);
            if (it != 因果索引_.end() && it->second && it->second->主信息) {
                auto* 已有节点 = static_cast<因果实例节点类*>(it->second);
                auto* 已有信息 = dynamic_cast<因果主信息类*>(已有节点->主信息);
                if (已有信息) {
                    if (!尝试事务式写入因果观测字段_(
                        已有信息,
                        规范结果.数据,
                        因动作动态,
                        主结果动作动态 ? 主结果动作动态 : (果动态 ? 果动态 : 规范结果.数据.主结果动态),
                        条件方向二次特征列表,
                        条件方向值列表,
                        条件方向可比较列表,
                        过程动态列表,
                        结果状态,
                        结果方向)) {
                        return 已有节点;
                    }
                    if (已有信息->首次发生时间 == 0) {
                        已有信息->首次发生时间 = now;
                    }
                    已有信息->最近发生时间 = now;
                    已有信息->观察次数 = (已有信息->观察次数 == 0)
                        ? 1
                        : (已有信息->观察次数 + 1);
                    已有信息->是否原始观测实例 = false;
                    已有信息->原始观测序号 = 0;
                    已有信息->归纳因果节点 = nullptr;
                    已有信息->因果累计键_存档 = 因果累计键;
                    已有信息->稳定度Q = 估计稳定度Q_(已有信息);
                    同步实例因果存档缓存_(已有信息);
                    登记反向因果索引_(已有节点, *已有信息);
                    同步抽象因果到因果树(已有节点, now, "因果类::创建或累计因果信息_按通道/累计/抽象");
                    return 已有节点;
                }
            }
        }

        auto* mi = new 因果主信息类();
        写入因果观测字段_(
            mi,
            规范结果.数据,
            因动作动态,
            主结果动作动态 ? 主结果动作动态 : (果动态 ? 果动态 : 规范结果.数据.主结果动态),
            条件方向二次特征列表,
            条件方向值列表,
            条件方向可比较列表,
            过程动态列表,
            结果状态,
            结果方向);
        if (校验因果实例合法性_(*mi) != 枚举_因果校验结果::合法) {
            delete mi;
            return nullptr;
        }
        mi->首次发生时间 = now;
        mi->最近发生时间 = now;
        mi->观察次数 = 1;
        mi->稳定度Q = 估计稳定度Q_(mi);
        mi->是否原始观测实例 = false;
        mi->原始观测序号 = 0;
        mi->归纳因果节点 = nullptr;
        mi->因果累计键_存档 = 因果累计键;
        return 存储实例因果到锚点(
            规范结果.数据.锚点信息,
            mi,
            now,
            "因果类::创建或累计因果信息_按通道");
    }

public:
    void 调试清空运行期缓存() noexcept
    {
        因果根_ = nullptr;
        因果索引_.clear();
        因果索引_结果特征_.clear();
        因果索引_动作主体_.clear();
        因果索引_动作语义_.clear();
        因果索引_来源方法_.clear();
        因果索引_来源任务_.clear();
        抽象因果索引_.clear();
        抽象因果索引_结果特征_.clear();
        抽象因果索引_动作模板_.clear();
        跨层因果链草案队列_.clear();
    }

    基础信息节点类* 创建或累计场景因果实例(
        场景节点类* 场景,
        const std::vector<状态节点类*>& 条件状态列表,
        基础信息节点类* 动作主体,
        基础信息节点类* 动作语义,
        const std::vector<动态节点类*>& 过程动态列表,
        const std::vector<状态节点类*>& 结果候选状态列表,
        动态节点类* 因动态 = nullptr,
        动态节点类* 果动态 = nullptr,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::创建或累计场景因果实例")
    {
        return 创建或累计因果信息_按通道(
            场景,
            条件状态列表,
            动作主体,
            动作语义,
            过程动态列表,
            结果候选状态列表,
            枚举_因果生成通道::场景,
            因动态,
            果动态,
            now,
            场景,
            调用点);
    }

    基础信息节点类* 创建或累计存在因果实例(
        存在节点类* 锚点存在,
        场景节点类* 场景,
        const std::vector<状态节点类*>& 条件状态列表,
        基础信息节点类* 动作主体,
        基础信息节点类* 动作语义,
        const std::vector<动态节点类*>& 过程动态列表,
        const std::vector<状态节点类*>& 结果候选状态列表,
        动态节点类* 因动态 = nullptr,
        动态节点类* 果动态 = nullptr,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::创建或累计存在因果实例")
    {
        if (!锚点存在) return nullptr;
        return 创建或累计因果信息_按通道(
            场景,
            条件状态列表,
            动作主体,
            动作语义,
            过程动态列表,
            结果候选状态列表,
            枚举_因果生成通道::存在,
            因动态,
            果动态,
            now,
            锚点存在,
            调用点);
    }

    基础信息节点类* 创建或累计场景因果信息(
        场景节点类* 场景,
        const std::vector<状态节点类*>& 条件状态列表,
        基础信息节点类* 动作主体,
        基础信息节点类* 动作语义,
        const std::vector<动态节点类*>& 过程动态列表,
        const std::vector<状态节点类*>& 结果候选状态列表,
        动态节点类* 因动态 = nullptr,
        动态节点类* 果动态 = nullptr,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::创建或累计场景因果信息")
    {
        return 创建或累计场景因果实例(
            场景,
            条件状态列表,
            动作主体,
            动作语义,
            过程动态列表,
            结果候选状态列表,
            因动态,
            果动态,
            now,
            调用点);
    }

    基础信息节点类* 创建或累计存在自因果信息(
        场景节点类* 场景,
        const std::vector<状态节点类*>& 条件状态列表,
        基础信息节点类* 动作主体,
        基础信息节点类* 动作语义,
        const std::vector<动态节点类*>& 过程动态列表,
        const std::vector<状态节点类*>& 结果候选状态列表,
        动态节点类* 因动态 = nullptr,
        动态节点类* 果动态 = nullptr,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::创建或累计存在自因果信息")
    {
        return 创建或累计因果信息_按通道(
            场景,
            条件状态列表,
            动作主体,
            动作语义,
            过程动态列表,
            结果候选状态列表,
            枚举_因果生成通道::存在,
            因动态,
            果动态,
            now,
            nullptr,
            调用点);
    }

    基础信息节点类* 创建或累计因果信息(
        场景节点类* 场景,
        const std::vector<状态节点类*>& 条件状态列表,
        基础信息节点类* 动作信息,
        const std::vector<动态节点类*>& 过程动态列表,
        const std::vector<状态节点类*>& 结果候选状态列表,
        动态节点类* 因动态 = nullptr,
        动态节点类* 果动态 = nullptr,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::创建或累计因果信息/兼容")
    {
        return 创建或累计因果信息(
            场景,
            条件状态列表,
            动作信息,
            nullptr,
            过程动态列表,
            结果候选状态列表,
            因动态,
            果动态,
            now,
            调用点);
    }

    基础信息节点类* 创建或累计因果信息(
        场景节点类* 场景,
        const std::vector<状态节点类*>& 条件状态列表,
        基础信息节点类* 动作主体,
        基础信息节点类* 动作语义,
        const std::vector<动态节点类*>& 过程动态列表,
        const std::vector<状态节点类*>& 结果候选状态列表,
        动态节点类* 因动态 = nullptr,
        动态节点类* 果动态 = nullptr,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::创建或累计因果信息")
    {
        auto* node = 创建或累计存在自因果信息(
            场景,
            条件状态列表,
            动作主体,
            动作语义,
            过程动态列表,
            结果候选状态列表,
            因动态,
            果动态,
            now,
            调用点 + "/存在");
        if (node) return node;
        return 创建或累计场景因果信息(
            场景,
            条件状态列表,
            动作主体,
            动作语义,
            过程动态列表,
            结果候选状态列表,
            因动态,
            果动态,
            now,
            调用点 + "/场景");
    }

    const std::vector<结构_因果链草案>& 获取跨层因果链草案队列() const noexcept
    {
        return 跨层因果链草案队列_;
    }

    void 清空跨层因果链草案队列() noexcept
    {
        跨层因果链草案队列_.clear();
    }

    std::vector<基础信息节点类*> 获取实例因果列表(std::size_t 上限 = 0) const
    {
        std::vector<基础信息节点类*> out{};
        auto* 根 = 数据仓库模块::世界链.根指针;
        for (auto* 当前 = 根 ? static_cast<基础信息节点类*>(根->链下) : nullptr;
            当前 && 当前 != 根;
            当前 = static_cast<基础信息节点类*>(当前->链下)) {
            auto* 信息 = (当前 && 当前->主信息) ? dynamic_cast<因果主信息类*>(当前->主信息) : nullptr;
            if (!信息 || 信息->结构类型 != 枚举_因果结构类型::实例) continue;
            auto* 父节点 = 当前->父 ? static_cast<基础信息节点类*>(当前->父) : nullptr;
            if (!是否实例因果锚点宿主_(父节点)) continue;
            out.push_back(当前);
        }

        排序因果按稳定度_(out);
        if (上限 > 0 && out.size() > 上限) out.resize(上限);
        return out;
    }

    std::vector<基础信息节点类*> 获取抽象因果列表(std::size_t 上限 = 0) const
    {
        std::vector<基础信息节点类*> out{};
        auto* 因果根 = 获取因果根();
        auto* 首节点 = 因果根 ? static_cast<基础信息节点类*>(因果根->子) : nullptr;
        if (!首节点) return out;

        auto* 当前 = 首节点;
        do {
            if (当前 && 当前->主信息 && dynamic_cast<因果模板主信息类*>(当前->主信息)) {
                out.push_back(当前);
            }
            当前 = static_cast<基础信息节点类*>(当前->下);
        } while (当前 && 当前 != 首节点);

        排序抽象因果按稳定度_(out);
        if (上限 > 0 && out.size() > 上限) out.resize(上限);
        return out;
    }

    std::vector<基础信息节点类*> 获取因果链列表(std::size_t 上限 = 0) const
    {
        std::vector<基础信息节点类*> out{};
        auto* 根 = 数据仓库模块::世界链.根指针;
        for (auto* 当前 = 根 ? static_cast<基础信息节点类*>(根->链下) : nullptr;
            当前 && 当前 != 根;
            当前 = static_cast<基础信息节点类*>(当前->链下)) {
            auto* 信息 = (当前 && 当前->主信息) ? dynamic_cast<因果链主信息类*>(当前->主信息) : nullptr;
            if (!信息) continue;
            auto* 父节点 = 当前->父 ? static_cast<基础信息节点类*>(当前->父) : nullptr;
            if (!是否实例因果锚点宿主_(父节点)) continue;
            out.push_back(当前);
        }

        排序因果链按长度和时间_(out);
        if (上限 > 0 && out.size() > 上限) out.resize(上限);
        return out;
    }

    基础信息节点类* 按抽象键获取抽象因果(const std::string& 抽象键) const
    {
        if (抽象键.empty()) return nullptr;
        auto it = 抽象因果索引_.find(抽象键);
        return it == 抽象因果索引_.end() ? nullptr : it->second;
    }

    std::vector<基础信息节点类*> 按结果特征查询抽象因果(
        特征节点类* 主结果特征,
        std::size_t 上限 = 16,
        枚举_因果锚点类型 适用锚点类型 = 枚举_因果锚点类型::未定义,
        std::uint32_t 适用层级 = 0,
        std::uint64_t 适用状态面签名 = 0) const
    {
        std::vector<基础信息节点类*> out{};
        if (!主结果特征) return out;
        const auto 键片段 = std::string{"|feature="} + 生成节点稳定键_(主结果特征) + "|";
        从作用域索引收集抽象因果_(
            抽象因果索引_结果特征_,
            键片段,
            上限,
            适用锚点类型,
            适用层级,
            适用状态面签名,
            out);
        return out;
    }

    std::vector<基础信息节点类*> 按动作模板查询抽象因果(
        基础信息节点类* 动作模板,
        std::size_t 上限 = 16,
        枚举_因果锚点类型 适用锚点类型 = 枚举_因果锚点类型::未定义,
        std::uint32_t 适用层级 = 0,
        std::uint64_t 适用状态面签名 = 0) const
    {
        std::vector<基础信息节点类*> out{};
        if (!动作模板) return out;
        const auto 键片段 = std::string{"|action="} + 生成节点稳定键_(动作模板) + "|";
        从作用域索引收集抽象因果_(
            抽象因果索引_动作模板_,
            键片段,
            上限,
            适用锚点类型,
            适用层级,
            适用状态面签名,
            out);
        return out;
    }

    std::vector<基础信息节点类*> 按结果特征查询因果(
        特征节点类* 结果特征,
        std::size_t 上限 = 16,
        枚举_因果锚点类型 锚点类型 = 枚举_因果锚点类型::未定义,
        std::uint64_t 锚点主键 = 0,
        std::uint32_t 状态层级 = 0,
        std::uint64_t 状态面签名 = 0) const
    {
        std::vector<基础信息节点类*> out;
        if (!结果特征) return out;
        const auto 键片段 = std::string{"|feature="} + 生成节点稳定键_(结果特征) + "|";
        从作用域索引收集因果_(因果索引_结果特征_, 键片段, 上限, 锚点类型, 锚点主键, 状态层级, 状态面签名, out);
        return out;
    }

    std::vector<基础信息节点类*> 按动作主体查询因果(
        基础信息节点类* 动作主体,
        std::size_t 上限 = 16,
        枚举_因果锚点类型 锚点类型 = 枚举_因果锚点类型::未定义,
        std::uint64_t 锚点主键 = 0,
        std::uint32_t 状态层级 = 0,
        std::uint64_t 状态面签名 = 0) const
    {
        std::vector<基础信息节点类*> out;
        if (!动作主体) return out;
        const auto 键片段 = std::string{"|subject="} + 生成节点稳定键_(动作主体) + "|";
        从作用域索引收集因果_(因果索引_动作主体_, 键片段, 上限, 锚点类型, 锚点主键, 状态层级, 状态面签名, out);
        return out;
    }

    std::vector<基础信息节点类*> 按动作语义键查询因果(
        const std::string& 动作语义键,
        std::size_t 上限 = 16,
        枚举_因果锚点类型 锚点类型 = 枚举_因果锚点类型::未定义,
        std::uint64_t 锚点主键 = 0,
        std::uint32_t 状态层级 = 0,
        std::uint64_t 状态面签名 = 0) const
    {
        std::vector<基础信息节点类*> out;
        if (动作语义键.empty()) return out;
        const auto 键片段 = std::string{"|semantic="} + 动作语义键 + "|";
        从作用域索引收集因果_(因果索引_动作语义_, 键片段, 上限, 锚点类型, 锚点主键, 状态层级, 状态面签名, out);
        return out;
    }

    std::vector<基础信息节点类*> 按动作语义查询因果(
        基础信息节点类* 动作语义,
        std::size_t 上限 = 16,
        枚举_因果锚点类型 锚点类型 = 枚举_因果锚点类型::未定义,
        std::uint64_t 锚点主键 = 0,
        std::uint32_t 状态层级 = 0,
        std::uint64_t 状态面签名 = 0) const
    {
        if (!动作语义) return {};
        return 按动作语义键查询因果(
            std::string{"node:"} + 生成节点稳定键_(动作语义),
            上限,
            锚点类型,
            锚点主键,
            状态层级,
            状态面签名);
    }

    std::vector<基础信息节点类*> 按来源方法查询因果(
        方法节点类* 来源方法首节点,
        std::size_t 上限 = 16,
        枚举_因果锚点类型 锚点类型 = 枚举_因果锚点类型::未定义,
        std::uint64_t 锚点主键 = 0,
        std::uint32_t 状态层级 = 0,
        std::uint64_t 状态面签名 = 0) const
    {
        if (!来源方法首节点) return {};
        return 按动作语义键查询因果(
            生成来源方法动作语义键_(计算节点运行期主键_(来源方法首节点)),
            上限,
            锚点类型,
            锚点主键,
            状态层级,
            状态面签名);
    }

    std::vector<基础信息节点类*> 按来源任务查询因果(
        std::uint64_t 来源任务主键,
        std::size_t 上限 = 16,
        枚举_因果锚点类型 锚点类型 = 枚举_因果锚点类型::未定义,
        std::uint64_t 锚点主键 = 0,
        std::uint32_t 状态层级 = 0,
        std::uint64_t 状态面签名 = 0) const
    {
        if (来源任务主键 == 0) return {};
        return 按动作语义键查询因果(
            生成来源任务动作语义键_(来源任务主键),
            上限,
            锚点类型,
            锚点主键,
            状态层级,
            状态面签名);
    }

    std::vector<基础信息节点类*> 按动作信息查询因果(
        基础信息节点类* 动作信息,
        std::size_t 上限 = 16) const
    {
        return 按动作主体查询因果(动作信息, 上限);
    }

    std::vector<动态节点类*> 搜索同型动态(
        场景节点类* 场景,
        动态节点类* 锚点动态) const
    {
        std::vector<动态节点类*> out;
        if (!场景 || !场景->主信息 || !锚点动态) return out;
        auto* smi = dynamic_cast<场景节点主信息类*>(场景->主信息);
        if (!smi) return out;

        const auto* 动态源 = 取时序动态源_(smi);
        auto* 时序锚点 = 尝试映射到时序锚点_(smi, 锚点动态);
        if (!动态源 || !时序锚点) return out;

        for (auto* d : *动态源) {
            if (!d) continue;
            if (!动态同型_(d, 时序锚点)) continue;
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
        const auto* 动态源 = 取时序动态源_(smi);
        if (!动态源) return;
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
            for (auto* d : *动态源) {
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

private:
    基础信息节点类* 从事件分段提炼初步因果信息_按通道(
        场景节点类* 场景,
        动态节点类* 锚点动态,
        枚举_因果生成通道 通道,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::从事件分段提炼初步因果信息_按通道",
        std::size_t 最少重复次数 = 2)
    {
        (void)场景;
        (void)锚点动态;
        (void)通道;
        (void)now;
        (void)调用点;
        (void)最少重复次数;
        return nullptr;
    }

    基础信息节点类* 从动态序列创建或累计因果信息_按通道(
        场景节点类* 场景,
        const std::vector<动态节点类*>& 相关动态序列,
        枚举_因果生成通道 通道,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::从动态序列创建或累计因果信息_按通道")
    {
        (void)场景;
        (void)相关动态序列;
        (void)通道;
        (void)now;
        (void)调用点;
        return nullptr;
    }

    基础信息节点类* 从单个动作动态创建或累计因果实例_按通道(
        场景节点类* 场景,
        动态节点类* 动作动态,
        枚举_因果生成通道 通道,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::从单个动作动态创建或累计因果实例_按通道")
    {
        if (!场景 || !动作动态) return nullptr;
        auto* 动态信息 = 取动态主信息_(动作动态);
        if (!动态信息 || !是否显式动作动态_(动作动态)) return nullptr;
        if (动态信息->来源动作相位 == 枚举_动作事件相位::开始运行) return nullptr;
        if (now == 0) now = 当前时间_();

        std::vector<动态节点类*> 过程动态列表{ 动作动态 };
        std::vector<状态节点类*> 条件状态列表{};
        std::vector<状态节点类*> 结果状态列表{};

        if (动态信息->初始状态) {
            追加唯一状态_(条件状态列表, static_cast<状态节点类*>(动态信息->初始状态));
        }
        if (动态信息->结果状态) {
            追加唯一状态_(结果状态列表, static_cast<状态节点类*>(动态信息->结果状态));
        }

        if (条件状态列表.empty()) {
            std::vector<状态节点类*> 动态状态列表{};
            收集动态状态列表_(动作动态, 动态状态列表);
            for (auto* 状态 : 动态状态列表) {
                if (!状态 || 状态 == 动态信息->结果状态) continue;
                追加唯一状态_(条件状态列表, 状态);
            }
        }
        if (条件状态列表.empty()) {
            收集场景状态列表_(动态信息->来源输入场景, 条件状态列表);
        }

        if (结果状态列表.empty()) {
            std::vector<状态节点类*> 动态状态列表{};
            收集动态状态列表_(动作动态, 动态状态列表);
            for (auto* 状态 : 动态状态列表) {
                if (!状态 || 状态 == 动态信息->初始状态) continue;
                追加唯一状态_(结果状态列表, 状态);
            }
        }
        if (结果状态列表.empty()) {
            收集场景状态列表_(动态信息->来源输出场景, 结果状态列表);
        }
        if (结果状态列表.empty()) {
            收集动态状态列表_(动作动态, 结果状态列表);
        }

        auto* 动作主体 = 推断动作主体_(nullptr, 动作动态, 动作动态);
        auto* 动作语义 = 推断动作语义_(nullptr, 动作主体, 动作动态, 动作动态);
        if (!动作主体 && !动作语义) return nullptr;

        return 创建或累计因果信息_按通道(
            场景,
            条件状态列表,
            动作主体,
            动作语义,
            过程动态列表,
            结果状态列表,
            通道,
            动作动态,
            动作动态,
            now,
            nullptr,
            调用点);
    }

    基础信息节点类* 动态记录后刷新因果_按通道(
        场景节点类* 场景,
        动态节点类* 新动态,
        枚举_因果生成通道 通道,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::动态记录后刷新因果_按通道",
        std::size_t 最大序列长度 = 6)
    {
        (void)最大序列长度;
        return 从单个动作动态创建或累计因果实例_按通道(
            场景,
            新动态,
            通道,
            now,
            调用点 + "/单动作动态");
    }

    基础信息节点类* 动态记录后刷新因果_按通道(
        场景节点类* 场景,
        const 运行时动态主信息记录* 新动态,
        枚举_因果生成通道 通道,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::动态记录后刷新因果_按通道/运行时",
        std::size_t 最大序列长度 = 6)
    {
        if (!场景 || !新动态) return nullptr;
        (void)最大序列长度;

        std::deque<状态节点主信息类> 临时状态主信息池{};
        std::deque<状态节点类> 临时状态节点池{};
        auto 包装状态 = [&](const 运行时状态主信息记录& 源) -> 状态节点类* {
            临时状态主信息池.emplace_back();
            auto& 临时状态主信息 = 临时状态主信息池.back();
            临时状态主信息.状态域 = 源.状态域;
            临时状态主信息.收到时间 = 源.收到时间;
            临时状态主信息.发生时间 = 源.发生时间;
            临时状态主信息.状态主体 = 源.状态主体;
            临时状态主信息.状态特征 = 源.状态特征;
            临时状态主信息.状态值 = 源.状态值;
            临时状态主信息.对应信息节点 = 源.对应信息节点;
            临时状态主信息.是否变化 = 源.是否变化;
            临时状态主信息.变化原因类别 = 源.变化原因类别;
            临时状态主信息.变化原因说明 = 源.变化原因说明;

            临时状态节点池.emplace_back();
            auto& 临时状态节点 = 临时状态节点池.back();
            临时状态节点.主信息 = &临时状态主信息;
            return &临时状态节点;
        };

        动态节点主信息类 临时主信息{};
        if (新动态->初始状态.has_value()) {
            临时主信息.初始状态 = 包装状态(*新动态->初始状态);
        }
        if (新动态->结果状态.has_value()) {
            临时主信息.结果状态 = 包装状态(*新动态->结果状态);
        }
        临时主信息.动态主体 = 新动态->动态主体;
        临时主信息.动态特征 = 新动态->动态特征;
        临时主信息.开始时间 = 新动态->开始时间;
        临时主信息.结束时间 = 新动态->结束时间;
        临时主信息.来源类型 = 新动态->来源类型;
        临时主信息.来源方法首节点 = 新动态->来源方法首节点;
        临时主信息.来源动作名 = 新动态->来源动作名;
        临时主信息.来源动作相位 = 新动态->来源动作相位;
        临时主信息.来源输入场景 = 新动态->来源输入场景;
        临时主信息.来源输出场景 = 新动态->来源输出场景;
        临时主信息.来源执行成功 = 新动态->来源执行成功;
        临时主信息.来源错误码 = 新动态->来源错误码;
        for (const auto& 状态 : 新动态->事件列表) {
            临时主信息.事件列表.push_back(包装状态(状态));
        }
        for (const auto& 状态 : 新动态->状态路径列表) {
            临时主信息.状态路径列表.push_back(包装状态(状态));
        }
        临时主信息.动态路径签名 = 新动态->动态路径签名;

        动态节点类 临时动态{};
        临时动态.主信息 = &临时主信息;

        auto* 结果 = 从单个动作动态创建或累计因果实例_按通道(
            场景,
            &临时动态,
            通道,
            now,
            调用点 + "/单动作动态");
        清空实例因果动态引用_(结果);
        return 结果;
    }

public:
    基础信息节点类* 从事件分段提炼场景因果信息(
        场景节点类* 场景,
        动态节点类* 锚点动态,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::从事件分段提炼场景因果信息",
        std::size_t 最少重复次数 = 2)
    {
        (void)场景;
        (void)锚点动态;
        (void)now;
        (void)调用点;
        (void)最少重复次数;
        return nullptr;
    }

    基础信息节点类* 从事件分段提炼存在自因果信息(
        场景节点类* 场景,
        动态节点类* 锚点动态,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::从事件分段提炼存在自因果信息",
        std::size_t 最少重复次数 = 2)
    {
        (void)场景;
        (void)锚点动态;
        (void)now;
        (void)调用点;
        (void)最少重复次数;
        return nullptr;
    }

    基础信息节点类* 从事件分段提炼初步因果信息(
        场景节点类* 场景,
        动态节点类* 锚点动态,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::从事件分段提炼初步因果信息",
        std::size_t 最少重复次数 = 2)
    {
        (void)场景;
        (void)锚点动态;
        (void)now;
        (void)调用点;
        (void)最少重复次数;
        return nullptr;
    }

    基础信息节点类* 从动态序列创建或累计场景因果信息(
        场景节点类* 场景,
        const std::vector<动态节点类*>& 相关动态序列,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::从动态序列创建或累计场景因果信息")
    {
        (void)场景;
        (void)相关动态序列;
        (void)now;
        (void)调用点;
        return nullptr;
    }

    基础信息节点类* 从动态序列创建或累计存在自因果信息(
        场景节点类* 场景,
        const std::vector<动态节点类*>& 相关动态序列,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::从动态序列创建或累计存在自因果信息")
    {
        (void)场景;
        (void)相关动态序列;
        (void)now;
        (void)调用点;
        return nullptr;
    }

    基础信息节点类* 从动态序列创建或累计因果信息(
        场景节点类* 场景,
        const std::vector<动态节点类*>& 相关动态序列,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::从动态序列创建或累计因果信息")
    {
        (void)场景;
        (void)相关动态序列;
        (void)now;
        (void)调用点;
        return nullptr;
    }

    基础信息节点类* 动态记录后刷新场景因果(
        场景节点类* 场景,
        动态节点类* 新动态,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::动态记录后刷新场景因果",
        std::size_t 最大序列长度 = 6)
    {
        return 动态记录后刷新因果_按通道(
            场景,
            新动态,
            枚举_因果生成通道::场景,
            now,
            调用点,
            最大序列长度);
    }

    基础信息节点类* 动态记录后刷新存在自因果(
        场景节点类* 场景,
        动态节点类* 新动态,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::动态记录后刷新存在自因果",
        std::size_t 最大序列长度 = 6)
    {
        return 动态记录后刷新因果_按通道(
            场景,
            新动态,
            枚举_因果生成通道::存在,
            now,
            调用点,
            最大序列长度);
    }

    基础信息节点类* 动态记录后刷新因果(
        场景节点类* 场景,
        动态节点类* 新动态,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::动态记录后刷新因果",
        std::size_t 最大序列长度 = 6)
    {
        const auto 优先通道 = 推断优先生成通道_(场景, 新动态);
        if (auto* node = 动态记录后刷新因果_按通道(场景, 新动态, 优先通道, now, 调用点, 最大序列长度)) {
            return node;
        }
        const auto 回退通道 = 另一因果生成通道_(优先通道);
        if (回退通道 == 枚举_因果生成通道::未定义) return nullptr;
        return 动态记录后刷新因果_按通道(场景, 新动态, 回退通道, now, 调用点, 最大序列长度);
    }

    基础信息节点类* 动态记录后刷新因果(
        场景节点类* 场景,
        const 运行时动态主信息记录* 新动态,
        时间戳 now = 0,
        const std::string& 调用点 = "因果类::动态记录后刷新因果/运行时",
        std::size_t 最大序列长度 = 6)
    {
        if (!场景 || !新动态) return nullptr;
        auto* smi = (场景 && 场景->主信息) ? dynamic_cast<场景节点主信息类*>(场景->主信息) : nullptr;
        const auto 优先通道 = (smi && smi->是否内部世界())
            ? 枚举_因果生成通道::存在
            : 枚举_因果生成通道::场景;
        if (auto* node = 动态记录后刷新因果_按通道(场景, 新动态, 优先通道, now, 调用点, 最大序列长度)) {
            return node;
        }
        const auto 回退通道 = 另一因果生成通道_(优先通道);
        if (回退通道 == 枚举_因果生成通道::未定义) return nullptr;
        return 动态记录后刷新因果_按通道(场景, 新动态, 回退通道, now, 调用点, 最大序列长度);
    }
};

export inline 因果类 因果集{};


