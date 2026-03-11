module;

export module 存在模块;

import 主信息定义模块;
import 数据仓库模块;

import <vector>;
import <string>;
import <cstdint>;
import <algorithm>;

using namespace 数据仓库模块;

// ============================================================
// 存在类：仅做“增删改查（CRUD）”的结构操作
// - 只负责：
//   1) 创建/挂接/删除：存在节点
//   2) 创建/挂接/删除：特征节点（作为存在/场景的子节点之一）
//   3) 查询：按类型枚举子节点、按类型查找第一个子特征
// - 不负责：
//   - 特征值写入/稳态/区间/比较
//   - 方法/任务/需求的任何逻辑
//   - 特征值唯一化/证据池
// ============================================================

export class 存在类 {
public:
    // ============================================================
    // 1) 存在 CRUD
    // ============================================================

    // 创建存在节点并挂到父节点：
    // - parent 可为 场景节点/存在节点/世界根（nullptr 表示世界根）
    // - 所有权：世界链会在删除节点时 delete 主信息
    static 存在节点类* 创建存在(基础信息节点类* parent, 存在节点主信息类* mi)
    {
        if (!mi) return nullptr;
        锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", "存在类::创建存在") });
        基础信息节点类* p = parent ? parent : 世界链.根指针;
        return static_cast<存在节点类*>(世界链.添加子节点_已加锁(p, static_cast<基础信息基类*>(mi)));
    }

    // 将已分配的存在节点挂到父节点
    static 存在节点类* 挂接存在(基础信息节点类* parent, 存在节点类* node)
    {
        if (!node) return nullptr;
        锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", "存在类::挂接存在") });
        基础信息节点类* p = parent ? parent : 世界链.根指针;
        return static_cast<存在节点类*>(世界链.添加子节点_已加锁(p, node));
    }

    // 删除存在节点（默认递归删除子树）
    static bool 删除存在(存在节点类* node)
    {
        if (!node) return false;
        锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", "存在类::删除存在") });
        世界链.删除节点_已加锁(node);
        return true;
    }

    // 更新存在节点主信息（替换指针，旧主信息会被 delete）
    // - 注意：newMi 必须来自 new，且不能被其他节点共享。
    static bool 更新存在主信息(存在节点类* node, 存在节点主信息类* newMi)
    {
        if (!node || !newMi) return false;
        锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", "存在类::更新存在主信息") });
        if (node->主信息 == static_cast<基础信息基类*>(newMi)) return true;
        delete node->主信息;
        node->主信息 = static_cast<基础信息基类*>(newMi);
        return true;
    }

    // 查询：获取某父节点下的全部子存在
    static std::vector<存在节点类*> 获取子存在(基础信息节点类* parent)
    {
        std::vector<存在节点类*> out;
        if (!parent || !parent->子) return out;

        锁调度器守卫 锁({ 锁请求::读(世界链.链表锁, 枚举_锁域::世界链, "世界链", "存在类::获取子存在") });

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

    // ============================================================
    // 2) 特征 CRUD（作为存在/场景的子节点）
    // ============================================================

    // 创建特征节点并挂到宿主（宿主通常是存在/场景）
    // - 所有权：世界链会在删除节点时 delete 主信息
    static 特征节点类* 创建特征(基础信息节点类* host, 特征节点主信息类* mi)
    {
        if (!host || !mi) return nullptr;
        锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", "存在类::创建特征") });
        return static_cast<特征节点类*>(世界链.添加子节点_已加锁(host, static_cast<基础信息基类*>(mi)));
    }

    // 便捷：创建一个“只填类型/名称”的特征主信息并挂接
    static 特征节点类* 创建特征_按类型(基础信息节点类* host, const 词性节点类* 特征类型, const 词性节点类* 特征名称 = nullptr)
    {
        if (!host || !特征类型) return nullptr;
        auto* mi = new 特征节点主信息类();
        mi->类型 = const_cast<词性节点类*>(特征类型);
        mi->名称 = const_cast<词性节点类*>(特征名称);
        return 创建特征(host, mi);
    }

    // 将已分配的特征节点挂到宿主
    static 特征节点类* 挂接特征(基础信息节点类* host, 特征节点类* node)
    {
        if (!host || !node) return nullptr;
        锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", "存在类::挂接特征") });
        return static_cast<特征节点类*>(世界链.添加子节点_已加锁(host, node));
    }

    // 删除特征节点
    static bool 删除特征(特征节点类* node)
    {
        if (!node) return false;
        锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", "存在类::删除特征") });
        世界链.删除节点_已加锁(node);
        return true;
    }

    // 更新特征节点主信息（替换指针，旧主信息会被 delete）
    static bool 更新特征主信息(特征节点类* node, 特征节点主信息类* newMi)
    {
        if (!node || !newMi) return false;
        锁调度器守卫 锁({ 锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", "存在类::更新特征主信息") });
        if (node->主信息 == static_cast<基础信息基类*>(newMi)) return true;
        delete node->主信息;
        node->主信息 = static_cast<基础信息基类*>(newMi);
        return true;
    }

    // 查询：获取宿主下所有子特征
    static std::vector<特征节点类*> 获取子特征(基础信息节点类* host)
    {
        std::vector<特征节点类*> out;
        if (!host || !host->子) return out;

        锁调度器守卫 锁({ 锁请求::读(世界链.链表锁, 枚举_锁域::世界链, "世界链", "存在类::获取子特征") });

        auto* start = static_cast<基础信息节点类*>(host->子);
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

    // 查找：宿主下按“类型”找第一个特征节点
    static 特征节点类* 查找子特征_按类型(基础信息节点类* host, const 词性节点类* 特征类型)
    {
        if (!host || !特征类型 || !host->子) return nullptr;

        锁调度器守卫 锁({ 锁请求::读(世界链.链表锁, 枚举_锁域::世界链, "世界链", "存在类::查找子特征_按类型") });

        auto* start = static_cast<基础信息节点类*>(host->子);
        auto* cur = start;
        do {
            auto* fmi = cur ? dynamic_cast<特征节点主信息类*>(cur->主信息) : nullptr;
            if (fmi && fmi->类型 == 特征类型) {
                return static_cast<特征节点类*>(cur);
            }
            cur = cur ? static_cast<基础信息节点类*>(cur->下) : nullptr;
        } while (cur && cur != start);

        return nullptr;
    }
};
