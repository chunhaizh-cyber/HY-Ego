// ======================== ConceptEngine.ixx ========================
// 概念引擎模块（V2 最小可用）
//
// 定位：
// - 监听“世界树中新建的存在实例”，从其特征集合中归纳出“存在概念模板”。
// - V1/V2：用“特征类型集合交集”生成概念；命名采用 hash，避免依赖自然语言生成。
// - 生成的概念节点注册在 概念域_存在 下；其定义用关系边 measured_by 指向“特征类型”。
//   （当前关系边只存 目标概念名词，允许跨域引用。）

export module 概念引擎模块;

import 数据仓库模块;
import 主信息定义模块;
import 通用函数模块;
import 语素环境模块;
import 概念树模块;

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
    struct 存在实例信息 {
        const 存在节点类* 实例节点 = nullptr;
        vector<const 词性节点类*> 特征类型集合;   // T(E)
    };

private:
    vector<存在实例信息> 存在实例列表_;

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

    // ============================================================
    // 入口：世界树中新建了一个存在实例
    // ============================================================
    void 处理新存在(存在节点类* 新存在节点)
    {
        if (!新存在节点) return;
        auto* mi = dynamic_cast<存在节点主信息类*>(新存在节点->主信息);
        if (!mi) return; // 不是存在节点

        // 1) 收集特征类型集合 T(E_new)
        vector<const 词性节点类*> T_new = 收集特征类型集合_线程安全(新存在节点);

        // 2) 与历史实例做交集 -> 生成/更新存在概念
        if (!T_new.empty() && !存在实例列表_.empty()) {
            unordered_set<string> keyset;
            keyset.reserve(T_new.size() * 2);
            for (auto* t : T_new) if (t) keyset.insert(t->获取主键());

            for (const auto& old : 存在实例列表_) {
                if (!old.实例节点) continue;
                if (old.特征类型集合.empty()) continue;

                auto inter = 求交集_by_主键(keyset, old.特征类型集合);
                if (inter.size() < 最小共同特征数_) continue;

                // 概念签名
                const std::uint64_t sig = 生成签名(inter);

                // 获取/创建概念节点（概念域_存在）
                auto* 新概念节点 = 获取或创建_存在概念(sig);
                if (!新概念节点) continue;

                // measured_by 边：指向特征类型（按词性节点名称存储）
                for (auto* ft : inter) {
                    if (!ft) continue;
                    (void)概念树类::注册关系_同域(新概念节点, 枚举_概念关系::measured_by, ft,
                        "概念引擎类::处理新存在::注册measured_by");
                }
            }
        }

        // 3) 记录本次实例
        记录新存在_(新存在节点, T_new);
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

    static std::uint64_t 生成签名(const vector<const 词性节点类*>& features)
    {
        // 稳定：按主键排序后混入
        std::uint64_t h = 1469598103934665603ull;
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

    void 记录新存在_(const 存在节点类* node, const vector<const 词性节点类*>& T)
    {
        存在实例信息 info;
        info.实例节点 = node;
        info.特征类型集合 = T;
        存在实例列表_.push_back(std::move(info));
    }
};
