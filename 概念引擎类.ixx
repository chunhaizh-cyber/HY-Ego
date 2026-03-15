export module 概念引擎模块;

import 数据仓库模块;
import 主信息定义模块;
import 通用函数模块;
import 语素环境模块;
import 概念树模块;
import 日志模块;

import <vector>;
import <unordered_map>;
import <unordered_set>;
import <string>;
import <algorithm>;

using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::string;

using namespace 数据仓库模块;
using namespace 通用函数模块;
export class 概念引擎类 {
public:
    enum class 枚举_根需求投影结果 : std::uint8_t {
        未知 = 0,
        正向 = 1,
        负向 = 2,
        中性 = 3,
        条件性正向 = 4,
        条件性负向 = 5
    };

    enum class 枚举_候选稳定状态 : std::uint8_t {
        观察中 = 0,
        弱稳定 = 1,
        稳定 = 2,
        降级中 = 3
    };

    enum class 枚举_决策参与级别 : std::uint8_t {
        不参与 = 0,
        弱参与 = 1,
        中参与 = 2,
        强参与 = 3
    };

    struct 存在实例信息 {
        const 存在节点类* 实例节点 = nullptr;
        string 实例主键;
        string 关系类型键;
        时间戳 观察时间 = 0;
        vector<const 词性节点类*> 特征类型集合;   // T(E)
        unordered_set<string> 特征主键集合;
    };

private:
    struct 候选概念账本 {
        std::uint64_t 候选签名 = 0;
        string 候选签名文本;
        枚举_概念域 概念域 = 枚举_概念域::存在;
        string 关系类型键;
        vector<const 词性节点类*> 特征类型集合;
        unordered_set<string> 特征主键集合;

        std::uint32_t 证据次数 = 0;
        std::uint32_t 反例次数 = 0;
        时间戳 首次观察时间 = 0;
        时间戳 最近观察时间 = 0;
        二次特征主信息类::枚举_时间窗 时间窗 = 二次特征主信息类::枚举_时间窗::未定义;

        枚举_根需求投影结果 根需求投影结果 = 枚举_根需求投影结果::未知;
        枚举_候选稳定状态 稳定状态 = 枚举_候选稳定状态::观察中;
        枚举_决策参与级别 决策参与级别 = 枚举_决策参与级别::不参与;

        std::int64_t 一致性Q = 0;
        std::int64_t 可复现度Q = 0;
        std::uint32_t 来源存在覆盖数 = 0;

        unordered_set<string> 支持实例主键集合;
        存在节点类* 概念锚点节点 = nullptr;
    };

    vector<存在实例信息> 存在实例列表_;
    unordered_map<std::uint64_t, 候选概念账本> 候选概念账本表_;

    // 去重：概念签名(hash->概念节点)
    unordered_map<std::uint64_t, 存在节点类*> 存在概念缓存_;

    // 生成门槛：交集至少多少个特征类型才尝试生成概念
    std::size_t 最小共同特征数_ = 3;

public:
    概念引擎类() = default;
    ~概念引擎类() = default;

    概念引擎类(const 概念引擎类&) = delete;
    概念引擎类& operator=(const 概念引擎类&) = delete;
    概念引擎类(概念引擎类&&) = default;
    概念引擎类& operator=(概念引擎类&&) = default;

    void 设置最小共同特征数(std::size_t n) { 最小共同特征数_ = (n < 1 ? 1 : n); }
    std::size_t 候选概念账本数量() const { return 候选概念账本表_.size(); }

    // ============================================================
    // 入口：世界树中新建了一个存在实例
    // ============================================================
    void 处理新存在(存在节点类* 新存在节点)
    {
        if (!新存在节点) return;
        auto* mi = dynamic_cast<存在节点主信息类*>(新存在节点->主信息);
        if (!mi) return; // 不是存在节点

        // 1) 收集当前实例快照（特征集合 + 时间 + 类型签名）
        auto 新实例 = 构建存在实例信息_(新存在节点);
        if (新实例.特征类型集合.empty()) {
            记录新存在_(std::move(新实例));
            return;
        }

        unordered_set<std::uint64_t> 本轮已入账签名;
        unordered_set<std::uint64_t> 本轮已记反例签名;

        // 2) 与历史实例做交集 -> 先入候选账本，避免同周期重复记账
        if (!存在实例列表_.empty()) {
            for (const auto& old : 存在实例列表_) {
                if (!old.实例节点) continue;
                if (old.特征类型集合.empty()) continue;

                const auto 时间窗 = 判定时间窗_(新实例.观察时间, old.观察时间);
                const auto 关系类型键 = 组合关系类型键_(新实例.关系类型键, old.关系类型键);
                auto inter = 求交集_by_主键(新实例.特征主键集合, old.特征类型集合);
                if (inter.size() >= 最小共同特征数_) {
                    const std::uint64_t sig = 生成候选签名_(关系类型键, 时间窗, inter);
                    if (!本轮已入账签名.insert(sig).second) {
                        continue;
                    }

                    auto& 账本 = 取或创建候选账本_(sig, 关系类型键, 时间窗, inter);
                    const auto 旧状态 = 账本.稳定状态;
                    const auto 旧决策级别 = 账本.决策参与级别;
                    更新候选账本_(账本, 新实例, old);
                    尝试落稳定概念锚点_(账本);

                    if (账本.证据次数 == 1) {
                        日志::运行f(
                            "[概念引擎][候选] 新账本: 签名={}, 关系类型={}, 时间窗={}, 特征数={}",
                            账本.候选签名文本,
                            账本.关系类型键,
                            时间窗文本_(账本.时间窗),
                            账本.特征类型集合.size());
                    }
                    else if (旧状态 != 账本.稳定状态 || 旧决策级别 != 账本.决策参与级别) {
                        日志::运行f(
                            "[概念引擎][候选] 状态更新: 签名={}, 证据={}, 反例={}, 状态={} -> {}, 决策={} -> {}",
                            账本.候选签名文本,
                            账本.证据次数,
                            账本.反例次数,
                            候选稳定状态文本_(旧状态),
                            候选稳定状态文本_(账本.稳定状态),
                            决策参与级别文本_(旧决策级别),
                            决策参与级别文本_(账本.决策参与级别));
                    }
                }
                else {
                    尝试记录候选反例_(新实例, old, 时间窗, inter.size(), 本轮已记反例签名, 本轮已入账签名);
                }
            }
        }

        // 3) 记录本次实例
        记录新存在_(std::move(新实例));
    }

private:
    // ============================================================
    // 特征类型集合：从存在子链提取所有“特征节点”的 类型
    // ============================================================
    static vector<const 词性节点类*> 收集特征类型集合_线程安全(存在节点类* 存在)
    {
        vector<const 词性节点类*> out;
        if (!存在 || !存在->子) return out;

        锁调度器守卫 锁({
            锁请求::读(世界链.链表锁, 枚举_锁域::世界链, "世界链", "概念引擎类::收集特征类型集合")
            });

        auto* first = static_cast<基础信息节点类*>(存在->子);
        auto* it = first;
        do {
            auto* fmi = it ? dynamic_cast<特征节点主信息类*>(it->主信息) : nullptr;
            if (fmi && fmi->类型) out.push_back(fmi->类型);
            it = it ? static_cast<基础信息节点类*>(it->下) : nullptr;
        } while (it && it != first);

        // 去重（按主键）
        std::sort(out.begin(), out.end(), [](const 词性节点类* a, const 词性节点类* b) {
            return a->获取主键() < b->获取主键();
            });
        out.erase(std::unique(out.begin(), out.end(), [](const 词性节点类* a, const 词性节点类* b) {
            return a->获取主键() == b->获取主键();
            }), out.end());

        return out;
    }

    static vector<const 词性节点类*> 求交集_by_主键(
        const unordered_set<string>& T_new_keyset,
        const vector<const 词性节点类*>& old)
    {
        vector<const 词性节点类*> out;
        out.reserve(old.size());

        for (auto* t : old) {
            if (!t) continue;
            if (T_new_keyset.find(t->获取主键()) != T_new_keyset.end()) out.push_back(t);
        }

        std::sort(out.begin(), out.end(), [](const 词性节点类* a, const 词性节点类* b) {
            return a->获取主键() < b->获取主键();
            });
        out.erase(std::unique(out.begin(), out.end(), [](const 词性节点类* a, const 词性节点类* b) {
            return a->获取主键() == b->获取主键();
            }), out.end());

        return out;
    }

    static 时间戳 取观察时间_(const 存在节点主信息类* mi)
    {
        if (!mi) return 结构体_时间戳::当前_微秒();
        if (mi->最后观测时间 != 0) return mi->最后观测时间;
        if (mi->创建时间 != 0) return mi->创建时间;
        return 结构体_时间戳::当前_微秒();
    }

    static string 取存在类型键_(const 存在节点主信息类* mi)
    {
        if (!mi || !mi->类型) return string("__未定义存在类型__");
        return mi->类型->获取主键();
    }

    static string 组合关系类型键_(const string& a, const string& b)
    {
        if (a <= b) return a + "|" + b;
        return b + "|" + a;
    }

    static 二次特征主信息类::枚举_时间窗 判定时间窗_(时间戳 a, 时间戳 b)
    {
        if (a == 0 || b == 0) return 二次特征主信息类::枚举_时间窗::未定义;
        const 时间戳 delta = (a >= b) ? (a - b) : (b - a);
        if (delta <= 1000000) return 二次特征主信息类::枚举_时间窗::短;
        if (delta <= 60000000) return 二次特征主信息类::枚举_时间窗::中;
        return 二次特征主信息类::枚举_时间窗::长;
    }

    static std::uint64_t 生成候选签名_(
        const string& 关系类型键,
        二次特征主信息类::枚举_时间窗 时间窗,
        const vector<const 词性节点类*>& features)
    {
        std::uint64_t h = 1469598103934665603ull;
        h = Hash64_混入字符串(h, 关系类型键);
        h = Hash64_混入字符串(h, std::to_string(static_cast<int>(时间窗)));
        for (auto* t : features) {
            if (!t) continue;
            h = Hash64_混入字符串(h, t->获取主键());
        }
        return h;
    }

    static string to_hex(std::uint64_t v)
    {
        static const char* hex = "0123456789ABCDEF";
        string s;
        s.resize(16);
        for (int i = 15; i >= 0; --i) {
            s[i] = hex[(int)(v & 0xFULL)];
            v >>= 4;
        }
        return s;
    }

    static const char* 时间窗文本_(二次特征主信息类::枚举_时间窗 时间窗)
    {
        switch (时间窗) {
        case 二次特征主信息类::枚举_时间窗::短: return "短";
        case 二次特征主信息类::枚举_时间窗::中: return "中";
        case 二次特征主信息类::枚举_时间窗::长: return "长";
        default: return "未定义";
        }
    }

    static const char* 投影结果文本_(枚举_根需求投影结果 结果)
    {
        switch (结果) {
        case 枚举_根需求投影结果::正向: return "正向";
        case 枚举_根需求投影结果::负向: return "负向";
        case 枚举_根需求投影结果::中性: return "中性";
        case 枚举_根需求投影结果::条件性正向: return "条件性正向";
        case 枚举_根需求投影结果::条件性负向: return "条件性负向";
        default: return "未知";
        }
    }

    static const char* 候选稳定状态文本_(枚举_候选稳定状态 状态)
    {
        switch (状态) {
        case 枚举_候选稳定状态::弱稳定: return "弱稳定";
        case 枚举_候选稳定状态::稳定: return "稳定";
        case 枚举_候选稳定状态::降级中: return "降级中";
        default: return "观察中";
        }
    }

    static const char* 决策参与级别文本_(枚举_决策参与级别 级别)
    {
        switch (级别) {
        case 枚举_决策参与级别::弱参与: return "弱参与";
        case 枚举_决策参与级别::中参与: return "中参与";
        case 枚举_决策参与级别::强参与: return "强参与";
        default: return "不参与";
        }
    }

    static std::int64_t 计算一致性Q_(std::uint32_t 证据次数, std::uint32_t 反例次数)
    {
        const auto 总样本 = static_cast<std::uint64_t>(证据次数) + static_cast<std::uint64_t>(反例次数);
        if (总样本 == 0) return 0;
        return static_cast<std::int64_t>((static_cast<std::uint64_t>(证据次数) * 10000ULL) / 总样本);
    }

    static std::int64_t 计算可复现度Q_(std::uint32_t 来源存在覆盖数)
    {
        return static_cast<std::int64_t>(std::min<std::uint32_t>(10000, 来源存在覆盖数 * 2500U));
    }

    static 枚举_根需求投影结果 评估根需求投影结果_(const 候选概念账本& /*账本*/)
    {
        // P1 先只把投影位留出来；真正投影逻辑后续再接根需求链。
        return 枚举_根需求投影结果::未知;
    }

    static 枚举_候选稳定状态 评估候选稳定状态_(const 候选概念账本& 账本)
    {
        if (账本.反例次数 > 0 && 账本.一致性Q < 7000) {
            return 枚举_候选稳定状态::降级中;
        }
        if (账本.证据次数 >= 3 &&
            账本.一致性Q >= 8500 &&
            账本.可复现度Q >= 7500 &&
            账本.根需求投影结果 != 枚举_根需求投影结果::未知) {
            return 枚举_候选稳定状态::稳定;
        }
        if (账本.证据次数 >= 2 &&
            账本.一致性Q >= 7000 &&
            账本.可复现度Q >= 5000) {
            return 枚举_候选稳定状态::弱稳定;
        }
        return 枚举_候选稳定状态::观察中;
    }

    static 枚举_决策参与级别 评估决策参与级别_(const 候选概念账本& 账本)
    {
        if (账本.稳定状态 != 枚举_候选稳定状态::稳定) {
            return 枚举_决策参与级别::不参与;
        }
        if (账本.根需求投影结果 == 枚举_根需求投影结果::未知) {
            return 枚举_决策参与级别::不参与;
        }
        return 枚举_决策参与级别::弱参与;
    }

    存在实例信息 构建存在实例信息_(存在节点类* 节点)
    {
        存在实例信息 info;
        if (!节点) return info;

        auto* mi = dynamic_cast<存在节点主信息类*>(节点->主信息);
        info.实例节点 = 节点;
        info.实例主键 = 节点->获取主键();
        info.关系类型键 = 取存在类型键_(mi);
        info.观察时间 = 取观察时间_(mi);
        info.特征类型集合 = 收集特征类型集合_线程安全(节点);
        for (auto* t : info.特征类型集合) {
            if (!t) continue;
            info.特征主键集合.insert(t->获取主键());
        }
        return info;
    }

    候选概念账本& 取或创建候选账本_(
        std::uint64_t 候选签名,
        const string& 关系类型键,
        二次特征主信息类::枚举_时间窗 时间窗,
        const vector<const 词性节点类*>& 特征类型集合)
    {
        auto [it, inserted] = 候选概念账本表_.try_emplace(候选签名);
        auto& 账本 = it->second;
        if (inserted) {
            账本.候选签名 = 候选签名;
            账本.候选签名文本 = to_hex(候选签名);
            账本.关系类型键 = 关系类型键;
            账本.时间窗 = 时间窗;
            账本.特征类型集合 = 特征类型集合;
            for (auto* 特征类型 : 特征类型集合) {
                if (!特征类型) continue;
                账本.特征主键集合.insert(特征类型->获取主键());
            }
        }
        return 账本;
    }

    void 刷新候选账本派生字段_(候选概念账本& 账本)
    {
        账本.来源存在覆盖数 = static_cast<std::uint32_t>(账本.支持实例主键集合.size());
        账本.一致性Q = 计算一致性Q_(账本.证据次数, 账本.反例次数);
        账本.可复现度Q = 计算可复现度Q_(账本.来源存在覆盖数);
        账本.根需求投影结果 = 评估根需求投影结果_(账本);
        账本.稳定状态 = 评估候选稳定状态_(账本);
        账本.决策参与级别 = 评估决策参与级别_(账本);
    }

    void 更新候选账本_(候选概念账本& 账本, const 存在实例信息& 新实例, const 存在实例信息& 旧实例)
    {
        ++账本.证据次数;
        if (账本.首次观察时间 == 0 || 新实例.观察时间 < 账本.首次观察时间) {
            账本.首次观察时间 = 新实例.观察时间;
        }
        账本.最近观察时间 = std::max(账本.最近观察时间, std::max(新实例.观察时间, 旧实例.观察时间));
        if (!新实例.实例主键.empty()) 账本.支持实例主键集合.insert(新实例.实例主键);
        if (!旧实例.实例主键.empty()) 账本.支持实例主键集合.insert(旧实例.实例主键);
        刷新候选账本派生字段_(账本);
    }

    void 尝试记录候选反例_(
        const 存在实例信息& 新实例,
        const 存在实例信息& 旧实例,
        二次特征主信息类::枚举_时间窗 时间窗,
        std::size_t 当前交集大小,
        unordered_set<std::uint64_t>& 本轮已记反例签名,
        const unordered_set<std::uint64_t>& 本轮已入账签名)
    {
        // P1 先做最保守的反例判定：
        // 仅当“同类型比较 + 同时间窗 + 当前样本与已有候选只差一步就命中”时，才记一次反例。
        if (新实例.关系类型键 != 旧实例.关系类型键) return;
        const auto 关系类型键 = 组合关系类型键_(新实例.关系类型键, 旧实例.关系类型键);

        for (auto& [签名, 账本] : 候选概念账本表_) {
            if (账本.关系类型键 != 关系类型键) continue;
            if (账本.时间窗 != 时间窗) continue;
            if (本轮已入账签名.find(签名) != 本轮已入账签名.end()) continue;
            if (!本轮已记反例签名.insert(签名).second) continue;

            if (账本.特征主键集合.empty()) continue;
            if (当前交集大小 + 1 < 账本.特征主键集合.size()) continue;
            if (当前交集大小 >= 账本.特征主键集合.size()) continue;

            ++账本.反例次数;
            账本.最近观察时间 = std::max(账本.最近观察时间, std::max(新实例.观察时间, 旧实例.观察时间));
            if (!新实例.实例主键.empty()) 账本.支持实例主键集合.insert(新实例.实例主键);
            if (!旧实例.实例主键.empty()) 账本.支持实例主键集合.insert(旧实例.实例主键);
            const auto 旧状态 = 账本.稳定状态;
            刷新候选账本派生字段_(账本);

            日志::运行f(
                "[概念引擎][候选] 反例入账: 签名={}, 反例={}, 投影={}, 状态={} -> {}",
                账本.候选签名文本,
                账本.反例次数,
                投影结果文本_(账本.根需求投影结果),
                候选稳定状态文本_(旧状态),
                候选稳定状态文本_(账本.稳定状态));
        }
    }

    存在节点类* 获取或创建_存在概念(std::uint64_t sig)
    {
        if (auto it = 存在概念缓存_.find(sig); it != 存在概念缓存_.end()) {
            return it->second;
        }

        // 名称：存在概念_<HEX>
        const string name = string("存在概念_") + to_hex(sig);
        const 词性节点类* w = 语素集.添加词性词(name, "名词");

        const string 创建点= "概念引擎类::获取或创建_存在概念";
        auto* 新概念节点 = 概念树类::获取或创建_概念(枚举_概念域::存在, w,结构体_时间戳::当前_微秒(), 创建点);
        if (!新概念节点) return nullptr;

        存在概念缓存_[sig] = 新概念节点;
        return 新概念节点;
    }

    void 尝试落稳定概念锚点_(候选概念账本& 账本)
    {
        if (账本.概念锚点节点) return;
        if (账本.稳定状态 != 枚举_候选稳定状态::稳定) return;
        if (账本.根需求投影结果 == 枚举_根需求投影结果::未知) return;

        auto* 概念节点 = 获取或创建_存在概念(账本.候选签名);
        if (!概念节点) return;

        for (auto* ft : 账本.特征类型集合) {
            if (!ft) continue;
            (void)概念树类::注册关系_同域(
                概念节点,
                枚举_概念关系::measured_by,
                ft,
                "概念引擎类::尝试落稳定概念锚点");
        }

        账本.概念锚点节点 = 概念节点;
        日志::运行f(
            "[概念引擎][候选] 稳定概念落锚: 签名={}, 证据={}, 决策={}",
            账本.候选签名文本,
            账本.证据次数,
            决策参与级别文本_(账本.决策参与级别));
    }

    void 记录新存在_(存在实例信息 info)
    {
        if (!info.实例节点) return;
        存在实例列表_.push_back(std::move(info));
    }
};

