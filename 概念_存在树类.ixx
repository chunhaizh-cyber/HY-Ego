module;
#include <windows.h>
// 概念_存在树模块.ixx
export module 存在概念树模块;

import 主信息定义模块;
import 数据仓库模块;
import 语素环境模块;
import 特征值环境模块;
import 特征类型定义模块;

import <string>;
import <vector>;
import <unordered_map>;
import <unordered_set>;
import <algorithm>;
import <cstdint>;

using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;



    // =====================================================================
    // 小工具：vector 指针集合操作
    // =====================================================================
    template<class T>
    inline bool 包含指针(const std::vector<T*>& v, const T* p) {
        return std::find(v.begin(), v.end(), p) != v.end();
    }

    template<class T>
    inline void 添加唯一(std::vector<T*>& v, T* p) {
        if (!p) return;
        if (!包含指针(v, p)) v.push_back(p);
    }

    template<class T>
    inline void 清理空(std::vector<T*>& v) {
        auto it = std::remove(v.begin(), v.end(), nullptr);
        if (it != v.end()) v.erase(it, v.end());
    }

    // 子链遍历：同层环结构
    template<class NodeT, class Fn>
    inline void 遍历子树(NodeT* root, Fn&& fn) {
        if (!root) return;
        std::vector<NodeT*> stk;
        stk.push_back(root);

        while (!stk.empty()) {
            NodeT* n = stk.back();
            stk.pop_back();
            if (!n) continue;

            fn(n);

            if (auto* c = (NodeT*)n->子) {
                auto* it = c;
                do {
                    stk.push_back((NodeT*)it);
                    it = (NodeT*)it->下;
                } while (it && it != c);
            }
        }
    }

    inline 存在节点主信息类* 取存在主信息(存在节点类* n) {
        return n ? dynamic_cast<存在节点主信息类*>((基础信息基类*)n->主信息) : nullptr;
    }

    inline 特征节点主信息类* 取特征主信息(特征节点类* f) {
        return f ? dynamic_cast<特征节点主信息类*>((基础信息基类*)f->主信息) : nullptr;
    }

    // =====================================================================
    // 兼容“新存在主信息接口”：优先使用 mi->添加特征 / mi->添加子存在 / mi->整理索引
    // （如果你还没把那些接口合并到工程里，这里会自动回退到 push_back + 重建缓存）
    // =====================================================================


    // =====================================================================
    // 特征占位：概念节点的“模板特征类型集合”实体化
    // =====================================================================


    // =====================================================================
    // 存档/重载兼容：鲁棒收集特征类型集合
    // - 优先走 mi->特征索引
    // - 若索引为空（例如反序列化后未重建），则扫描子链里“特征节点”
    // =====================================================================

   

export class 存在概念树类 {
public:
    using 节点类型 = 存在节点类;

private:
    节点类型* 根_存在概念 = nullptr;      // 概念_存在树的根（也在 世界链 上）
    节点类型* 概念_显示参数 = nullptr;    // 抽象配置节点（也在世界链上）

    // ===== 概念去重：特征类型集合 Key -> 概念节点 =====
    unordered_map<string, 节点类型*> Key到概念节点;

    // ===== 概念节点列表：只放“特征集合概念格”节点（用于选父概念） =====
    vector<节点类型*> 概念节点列表;

    // ===== 辅助/配置节点（例如：概念_显示参数 及其子树） =====
    vector<节点类型*> 辅助节点列表;
    unordered_set<节点类型*> 辅助子树节点集合;

public:
    存在概念树类() = default;
    ~存在概念树类() = default;

    存在概念树类(const 存在概念树类&) = delete;
    存在概念树类& operator=(const 存在概念树类&) = delete;
    存在概念树类(存在概念树类&&) = default;
    存在概念树类& operator=(存在概念树类&&) = default;


  


};
