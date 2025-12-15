// 概念引擎模块.ixx
export module 概念引擎模块;

import 主信息定义模块;
import 存在概念树模块;

import <vector>;
import <unordered_set>;
import <string>;
import <algorithm>;

using std::vector;
using std::unordered_set;
using std::string;

export class 概念引擎类 {
public:
    struct 存在实例信息 {
        存在节点类* 实例节点 = nullptr;
        vector<词性节点类*> 特征类型集合;   // T(E)
    };

private:
    vector<存在实例信息> 存在实例列表;

public:
    概念引擎类() = default;
    ~概念引擎类() = default;

    概念引擎类(const 概念引擎类&) = delete;
    概念引擎类& operator=(const 概念引擎类&) = delete;
    概念引擎类(概念引擎类&&) = default;
    概念引擎类& operator=(概念引擎类&&) = default;

    // —— 入口：世界树中新建了一个存在实例 —— 
    void 处理新存在(存在节点类* 新存在节点, 存在概念树类& 存在概念树) {
        if (!新存在节点) return;

        auto* 主信息 = dynamic_cast<存在节点主信息类*>(新存在节点->主信息);
        if (!主信息) {
            // 不是“存在节点”，忽略
            return;
        }

        // 1. 收集特征类型集合 T(E_new)
        vector<词性节点类*> T_new = 收集特征类型集合(新存在节点);
        if (T_new.empty()) {
            // 没特征，先记录，后面可以补特征再重评估
            记录新存在(新存在节点, T_new);
            return;
        }

        unordered_set<string> T_new_keyset;
        for (auto* t : T_new) {
            if (!t) continue;
            T_new_keyset.insert(t->主键);
        }

        // 2. 与所有历史实例做交集
        for (const auto& 旧实例 : 存在实例列表) {
            if (!旧实例.实例节点) continue;
            if (旧实例.特征类型集合.empty()) continue;

            vector<词性节点类*> 交集 =
                求交集_by_主键(T_new_keyset, 旧实例.特征类型集合);
            if (交集.empty()) continue;

            // 3. 交集 => 存在概念模板，交给 概念树 处理（内部去重+插入）
            (void)存在概念树.创建或查找概念(交集);
        }

        // 4. 记录本次实例
        记录新存在(新存在节点, T_new);
    }

private:
    static vector<词性节点类*> 收集特征类型集合(存在节点类* 存在) {
        vector<词性节点类*> 结果;
        if (!存在) return 结果;

        auto* 子 = 存在->子;
        if (!子) return 结果;

        auto* 起点 = 子;
        auto* 当前 = 子;

        do {
            if (!当前 || !当前->主信息) break;

            auto* 特征主信息 = dynamic_cast<特征节点主信息类*>(当前->主信息);
            if (特征主信息 && 特征主信息->类型) {
                结果.push_back(特征主信息->类型);
            }

            当前 = 当前->下;
            if (!当前) break;
        } while (当前 != 起点);

        // 去重
        std::sort(结果.begin(), 结果.end(),
            [](词性节点类* a, 词性节点类* b) {
                return a->主键 < b->主键;
            });
        结果.erase(std::unique(结果.begin(), 结果.end(),
            [](词性节点类* a, 词性节点类* b) {
                return a->主键 == b->主键;
            }), 结果.end());

        return 结果;
    }

    static vector<词性节点类*> 求交集_by_主键(
        const unordered_set<string>& T_new_keyset,
        const vector<词性节点类*>& 旧特征集合)
    {
        vector<词性节点类*> 结果;
        结果.reserve(旧特征集合.size());

        for (auto* t : 旧特征集合) {
            if (!t) continue;
            if (T_new_keyset.find(t->主键) != T_new_keyset.end()) {
                结果.push_back(t);
            }
        }

        std::sort(结果.begin(), 结果.end(),
            [](词性节点类* a, 词性节点类* b) {
                return a->主键 < b->主键;
            });
        结果.erase(std::unique(结果.begin(), 结果.end(),
            [](词性节点类* a, 词性节点类* b) {
                return a->主键 == b->主键;
            }), 结果.end());

        return 结果;
    }

    void 记录新存在(存在节点类* 节点,
        const vector<词性节点类*>& 特征类型集合) {
        存在实例信息 info;
        info.实例节点 = 节点;
        info.特征类型集合 = 特征类型集合;
        存在实例列表.push_back(info);
    }
};
