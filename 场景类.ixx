module;

export module 场景模块;

import 主信息定义模块;
import 数据仓库模块;

import <vector>;
import <string>;
import <cstdint>;

using namespace 数据仓库模块;

// ============================================================
// 场景类：仅做“增删改查（CRUD）”的结构操作
// - 只负责：
//   1) 创建/挂接/删除：场景节点
//   2) 查询：列出子场景/子存在/子特征
//   3) 更新：替换场景主信息（必要时）
// - 不负责：
//   - 空间变换/坐标单位/匹配/索引/比较
//   - 特征值写入/稳态/区间
// ============================================================

export class 场景类 {
public:
    // ============================================================
    // 1) 场景 CRUD
    // ============================================================

    // 创建场景节点并挂到父节点：
    // - parent 可为 场景节点/存在节点/世界根（nullptr 表示世界根）
    // - 所有权：世界链会在删除节点时 delete 主信息
    static 场景节点类* 创建场景(基础信息节点类* parent, 场景节点主信息类* mi)
    {
        if (!mi) return nullptr;
        锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", "场景类::创建场景") });
        基础信息节点类* p = parent ? parent : 世界链.根指针;
        return static_cast<场景节点类*>(世界链.添加子节点_已加锁(p, static_cast<基础信息基类*>(mi)));
    }

    // 将已分配的场景节点挂到父节点
    static 场景节点类* 挂接场景(基础信息节点类* parent, 场景节点类* node)
    {
        if (!node) return nullptr;
        锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", "场景类::挂接场景") });
        基础信息节点类* p = parent ? parent : 世界链.根指针;
        return static_cast<场景节点类*>(世界链.添加子节点_已加锁(p, node));
    }

    // 删除场景节点（递归删除子树）
    static bool 删除场景(场景节点类* node)
    {
        if (!node) return false;
        锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", "场景类::删除场景") });
        世界链.删除节点_已加锁(node);
        return true;
    }

    // 更新场景主信息（替换指针，旧主信息会被 delete）
    // - 注意：newMi 必须来自 new，且不能被其他节点共享。
    static bool 更新场景主信息(场景节点类* node, 场景节点主信息类* newMi)
    {
        if (!node || !newMi) return false;
        锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", "场景类::更新场景主信息") });
        if (node->主信息 == static_cast<基础信息基类*>(newMi)) return true;
        delete node->主信息;
        node->主信息 = static_cast<基础信息基类*>(newMi);
        return true;
    }

    // ============================================================
    // 2) 查询：子节点枚举
    // ============================================================

    static std::vector<场景节点类*> 获取子场景(基础信息节点类* parent)
    {
        std::vector<场景节点类*> out;
        if (!parent || !parent->子) return out;

        锁调度器守卫 锁({ 锁请求::读(世界链.链表锁, 枚举_锁域::世界链, "世界链", "场景类::获取子场景") });

        auto* start = static_cast<基础信息节点类*>(parent->子);
        auto* cur = start;
        do {
            if (cur && cur->主信息) {
                if (dynamic_cast<场景节点主信息类*>(cur->主信息)) {
                    out.push_back(static_cast<场景节点类*>(cur));
                }
            }
            cur = cur ? static_cast<基础信息节点类*>(cur->下) : nullptr;
        } while (cur && cur != start);

        return out;
    }

    static std::vector<存在节点类*> 获取子存在(基础信息节点类* parent)
    {
        std::vector<存在节点类*> out;
        if (!parent || !parent->子) return out;

        锁调度器守卫 锁({ 锁请求::读(世界链.链表锁, 枚举_锁域::世界链, "世界链", "场景类::获取子存在") });

        auto* start = static_cast<基础信息节点类*>(parent->子);
        auto* cur = start;
        do {
            if (cur && cur->主信息) {
                if (dynamic_cast<存在节点主信息类*>(cur->主信息)) {
                    out.push_back(static_cast<存在节点类*>(cur));
                }
            }
            cur = cur ? static_cast<基础信息节点类*>(cur->下) : nullptr;
        } while (cur && cur != start);

        return out;
    }

    static std::vector<特征节点类*> 获取子特征(基础信息节点类* parent)
    {
        std::vector<特征节点类*> out;
        if (!parent || !parent->子) return out;

        锁调度器守卫 锁({ 锁请求::读(世界链.链表锁, 枚举_锁域::世界链, "世界链", "场景类::获取子特征") });

        auto* start = static_cast<基础信息节点类*>(parent->子);
        auto* cur = start;
        do {
            if (cur && cur->主信息) {
                if (dynamic_cast<特征节点主信息类*>(cur->主信息)) {
                    out.push_back(static_cast<特征节点类*>(cur));
                }
            }
            cur = cur ? static_cast<基础信息节点类*>(cur->下) : nullptr;
        } while (cur && cur != start);

        return out;
    }
};
