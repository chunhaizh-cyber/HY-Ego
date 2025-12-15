// 概念_存在树模块.ixx
export module 存在概念树模块;

import 主信息定义模块;
import 宇宙链模块;

import <string>;
import <vector>;
import <unordered_map>;
import <unordered_set>;
import <algorithm>;

using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;

export struct 存在概念扩展信息 {
    存在节点类* 概念节点 = nullptr;
    vector<词性节点类*> 特征类型集合;  // 概念模板：S(C)
    string               集合Key;      // 规范化 key
};

export class 存在概念树类 {
public:
    using 节点类型 = 存在节点类;

private:
    节点类型* 根_存在概念 = nullptr;  // 概念_存在树的根（也在 宇宙链 上）

    vector<存在概念扩展信息>             概念扩展列表;
    unordered_map<string, 存在概念扩展信息*> Key到概念映射;

public:
    存在概念树类() = default;
    ~存在概念树类() = default;

    存在概念树类(const 存在概念树类&) = delete;
    存在概念树类& operator=(const 存在概念树类&) = delete;
    存在概念树类(存在概念树类&&) = default;
    存在概念树类& operator=(存在概念树类&&) = default;

    // 在 宇宙链 上建立一棵“存在概念树”的根
    void 初始化默认存在概念() {
        根_存在概念 = nullptr;
        概念扩展列表.clear();
        Key到概念映射.clear();

        auto* 主信息 = new 存在节点主信息类();   // 根概念本身可以是一个极空的“存在”
        基础信息基类* 基类指针 = 主信息;

        根_存在概念 = static_cast<节点类型*>(
            宇宙链.添加子节点(宇宙链.根指针, 基类指针)
            );
        根_存在概念->主键 = "概念_存在_根";

        存在概念扩展信息 ext;
        ext.概念节点 = 根_存在概念;
        ext.集合Key.clear();   // 根概念的模板特征集合视为 ∅

        概念扩展列表.push_back(ext);
        Key到概念映射[ext.集合Key] = &概念扩展列表.back();
    }

    节点类型* 获取根概念() const { return 根_存在概念; }

    const vector<存在概念扩展信息>& 获取全部扩展信息() const {
        return 概念扩展列表;
    }

    // —— 核心：给一组特征类型集合，创建或查找存在概念 —— 
    节点类型* 创建或查找概念(const vector<词性节点类*>& 特征类型集合) {
        vector<词性节点类*> 规范集合 = 去重并排序(特征类型集合);
        string key = 生成集合Key(规范集合);

        auto it = Key到概念映射.find(key);
        if (it != Key到概念映射.end()) {
            return it->second->概念节点;
        }

        return 创建概念节点_内部(规范集合, key);
    }

    节点类型* 按特征类型集合查找(const vector<词性节点类*>& 特征类型集合) const {
        vector<词性节点类*> 规范集合 = 去重并排序(特征类型集合);
        string key = 生成集合Key(规范集合);

        auto it = Key到概念映射.find(key);
        if (it == Key到概念映射.end()) return nullptr;
        return it->second->概念节点;
    }

private:
    static vector<词性节点类*> 去重并排序(const vector<词性节点类*>& 原始) {
        vector<词性节点类*> v = 原始;
        v.erase(std::remove(v.begin(), v.end(), nullptr), v.end());

        std::sort(v.begin(), v.end(),
            [](词性节点类* a, 词性节点类* b) {
                return a->主键 < b->主键;
            });

        v.erase(std::unique(v.begin(), v.end(),
            [](词性节点类* a, 词性节点类* b) {
                return a->主键 == b->主键;
            }), v.end());

        return v;
    }

    static string 生成集合Key(const vector<词性节点类*>& 集合) {
        string key;
        bool 第一 = true;
        for (auto* t : 集合) {
            if (!t) continue;
            if (!第一) key.push_back('|');
            key += t->主键;
            第一 = false;
        }
        return key;
    }

    节点类型* 创建概念节点_内部(const vector<词性节点类*>& 规范集合,
        const string& key) {
        if (!根_存在概念) {
            初始化默认存在概念();
        }

        // 1. 找“最具体的父概念”：S(父) ⊆ S_new 且 |S(父)| 最大
        节点类型* 父概念 = 选择父概念(规范集合);

        // 2. 在 宇宙链 上，作为父概念的子节点插入
        auto* 主信息 = new 存在节点主信息类();
        基础信息基类* 基类指针 = 主信息;

        节点类型* 新节点 = nullptr;
        if (父概念) {
            新节点 = static_cast<节点类型*>(
                宇宙链.添加子节点(父概念, 基类指针)
                );
        }
        else {
            // 理论上不会走到这里（至少有根_存在概念）
            新节点 = static_cast<节点类型*>(
                宇宙链.添加子节点(根_存在概念, 基类指针)
                );
        }

        // 3. 记录扩展信息 + 注册去重表
        存在概念扩展信息 ext;
        ext.概念节点 = 新节点;
        ext.特征类型集合 = 规范集合;
        ext.集合Key = key;

        概念扩展列表.push_back(ext);
        auto* extPtr = &概念扩展列表.back();
        Key到概念映射[ext.集合Key] = extPtr;

        // TODO：如果要实现“中间节点插入”，可以在这里重挂子树（S_new ⊆ S(child) 时）

        return 新节点;
    }

    节点类型* 选择父概念(const vector<词性节点类*>& S_new) const {
        if (!根_存在概念) return nullptr;

        unordered_set<string> S_new_key;
        S_new_key.reserve(S_new.size());
        for (auto* t : S_new) {
            if (!t) continue;
            S_new_key.insert(t->主键);
        }

        节点类型* 最佳父 = 根_存在概念;
        std::size_t 最佳父特征数 = 0;

        for (const auto& info : 概念扩展列表) {
            if (!info.概念节点) continue;
            const auto& S_parent = info.特征类型集合;

            bool 是子集 = true;
            for (auto* t : S_parent) {
                if (!t) continue;
                if (S_new_key.find(t->主键) == S_new_key.end()) {
                    是子集 = false;
                    break;
                }
            }
            if (!是子集) continue;

            std::size_t 当前特征数 = S_parent.size();
            if (当前特征数 > 最佳父特征数) {
                最佳父特征数 = 当前特征数;
                最佳父 = info.概念节点;
            }
        }

        return 最佳父 ? 最佳父 : 根_存在概念;
    }
};
