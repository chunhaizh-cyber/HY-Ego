//

export module 概念树模块;

import 数据仓库模块;
import 主信息定义模块;
import 语素环境模块;

import <cstdint>;
import <string>;
import <vector>;

using namespace 数据仓库模块;

export enum class 枚举_概念域 : std::uint8_t {
    未定义 = 0,
    特征 = 1,
    存在 = 2,
    状态 = 3,
    二次特征 = 4,
    因果 = 5,
    自然语言 = 6,
};

export enum class 枚举_概念关系 : std::uint8_t {
    未定义 = 0,
    is_a = 1,
    part_of = 2,
    opposite_of = 3,
    measured_by = 4,
    causes = 5,
    maps_to = 6,
    state_of = 7,
};

export class 概念树类 {
public:
    static 存在节点类* 获取或创建_概念域根(枚举_概念域 域, 时间戳 创建时间 = 结构体_时间戳::当前_微秒(), const std::string& 调用点 = "概念树类::获取或创建_概念域根")
    {
        if (域 == 枚举_概念域::未定义) return nullptr;

        初始化词表_懒();
        const 词性节点类* 域名 = 私有_取域名词(域);
        if (!域名) return nullptr;

        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
        });

        auto* root = 世界链.根指针;
        if (!root) return nullptr;

        if (root->子) {
            auto* first = static_cast<基础信息节点类*>(root->子);
            auto* it = first;
            do {
                auto* mi = it ? dynamic_cast<存在节点主信息类*>(it->主信息) : nullptr;
                if (mi && mi->名称 == 域名 && mi->类型 == 私有_词_概念域根()) {
                    return static_cast<存在节点类*>(it);
                }
                it = it ? static_cast<基础信息节点类*>(it->下) : nullptr;
            } while (it && it != first);
        }

        auto* mi = new 存在节点主信息类();
        mi->名称 = const_cast<词性节点类*>(域名);
        mi->类型 = const_cast<词性节点类*>(私有_词_概念域根());
        mi->创建时间 = 创建时间;
        return static_cast<存在节点类*>(世界链.添加子节点_已加锁(root, static_cast<基础信息基类*>(mi)));
    }

    static 存在节点类* 获取或创建_概念(枚举_概念域 域, const 词性节点类* 概念名, 时间戳 创建时间 = 结构体_时间戳::当前_微秒(), const std::string& 调用点 = "概念树类::获取或创建_概念")
    {
        if (域 == 枚举_概念域::未定义) return nullptr;
        if (!概念名) return nullptr;

        初始化词表_懒();

        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
        });

        auto* domainRoot = 获取或创建_概念域根(域, 创建时间);
        if (!domainRoot) return nullptr;

        if (domainRoot->子) {
            auto* first = static_cast<基础信息节点类*>(domainRoot->子);
            auto* it = first;
            do {
                auto* mi = it ? dynamic_cast<存在节点主信息类*>(it->主信息) : nullptr;
                if (mi && mi->名称 == 概念名 && mi->类型 == 私有_取域名词(域)) {
                    return static_cast<存在节点类*>(it);
                }
                it = it ? static_cast<基础信息节点类*>(it->下) : nullptr;
            } while (it && it != first);
        }

        auto* mi = new 存在节点主信息类();
        mi->名称 = const_cast<词性节点类*>(概念名);
        mi->类型 = const_cast<词性节点类*>(私有_取域名词(域));
        mi->创建时间 = 创建时间;
        return static_cast<存在节点类*>(世界链.添加子节点_已加锁(domainRoot, static_cast<基础信息基类*>(mi)));
    }

    static 存在节点类* 获取或创建_概念(枚举_概念域 域, const std::string& 概念词面, const std::string& 词性标签 = "名词", 时间戳 创建时间 = 结构体_时间戳::当前_微秒(), const std::string& 调用点 = "概念树类::获取或创建_概念(词面)")
    {
        if (概念词面.empty()) return nullptr;
        初始化词表_懒();
        const 词性节点类* name = 语素集.添加词性词(概念词面, 词性标签);
        return 获取或创建_概念(域, name, 创建时间, 调用点);
    }

    static 存在节点类* 注册关系_同域(
        存在节点类* 源概念,
        枚举_概念关系 关系,
        const 词性节点类* 目标概念名,
        const std::string& 调用点 = "概念树类::注册关系_同域")
    {
        if (!源概念 || !目标概念名) return nullptr;
        初始化词表_懒();

        const 词性节点类* relWord = 私有_取关系词(关系);
        if (!relWord) return nullptr;

        锁调度器守卫 锁({
            锁请求::写(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
        });

        if (源概念->子) {
            auto* first = static_cast<基础信息节点类*>(源概念->子);
            auto* it = first;
            do {
                auto* mi = it ? dynamic_cast<存在节点主信息类*>(it->主信息) : nullptr;
                if (mi && mi->类型 == relWord && mi->名称 == 目标概念名) {
                    return static_cast<存在节点类*>(it);
                }
                it = it ? static_cast<基础信息节点类*>(it->下) : nullptr;
            } while (it && it != first);
        }

        auto* mi = new 存在节点主信息类();
        mi->名称 = const_cast<词性节点类*>(目标概念名);
        mi->类型 = const_cast<词性节点类*>(relWord);
        return static_cast<存在节点类*>(世界链.添加子节点_已加锁(源概念, static_cast<基础信息基类*>(mi)));
    }

    static std::vector<const 词性节点类*> 列出关系目标(
        存在节点类* 源概念,
        枚举_概念关系 关系,
        const std::string& 调用点 = "概念树类::列出关系目标")
    {
        std::vector<const 词性节点类*> out;
        if (!源概念) return out;
        初始化词表_懒();

        const 词性节点类* relWord = 私有_取关系词(关系);
        if (!relWord) return out;

        锁调度器守卫 锁({
            锁请求::读(世界链.链表锁, 枚举_锁域::世界链, "世界链", 调用点)
        });

        if (!源概念->子) return out;

        auto* first = static_cast<基础信息节点类*>(源概念->子);
        auto* it = first;
        do {
            auto* mi = it ? dynamic_cast<存在节点主信息类*>(it->主信息) : nullptr;
            if (mi && mi->类型 == relWord && mi->名称) {
                out.push_back(mi->名称);
            }
            it = it ? static_cast<基础信息节点类*>(it->下) : nullptr;
        } while (it && it != first);

        return out;
    }

    static const 词性节点类* 取域名词(枚举_概念域 域) {
        初始化词表_懒();
        return 私有_取域名词(域);
    }

    static const 词性节点类* 取关系词(枚举_概念关系 r) {
        初始化词表_懒();
        return 私有_取关系词(r);
    }

private:
    static void 初始化词表_懒() {
        static bool inited = false;
        if (inited) return;

        (void)语素集.添加词性词("概念域根", "名词");
        (void)语素集.添加词性词("概念域_特征", "名词");
        (void)语素集.添加词性词("概念域_存在", "名词");
        (void)语素集.添加词性词("概念域_状态", "名词");
        (void)语素集.添加词性词("概念域_二次特征", "名词");
        (void)语素集.添加词性词("概念域_因果", "名词");
        (void)语素集.添加词性词("概念域_自然语言", "名词");

        (void)语素集.添加词性词("概念关系_is_a", "名词");
        (void)语素集.添加词性词("概念关系_part_of", "名词");
        (void)语素集.添加词性词("概念关系_opposite_of", "名词");
        (void)语素集.添加词性词("概念关系_measured_by", "名词");
        (void)语素集.添加词性词("概念关系_causes", "名词");
        (void)语素集.添加词性词("概念关系_maps_to", "名词");
        (void)语素集.添加词性词("概念关系_state_of", "名词");

        inited = true;
    }

    static const 词性节点类* 私有_词_概念域根() {
        static const 词性节点类* w = nullptr;
        if (!w) w = 语素集.添加词性词("概念域根", "名词");
        return w;
    }

    static const 词性节点类* 私有_取域名词(枚举_概念域 域) {
        switch (域) {
        case 枚举_概念域::特征:     return 语素集.添加词性词("概念域_特征", "名词");
        case 枚举_概念域::存在:     return 语素集.添加词性词("概念域_存在", "名词");
        case 枚举_概念域::状态:     return 语素集.添加词性词("概念域_状态", "名词");
        case 枚举_概念域::二次特征: return 语素集.添加词性词("概念域_二次特征", "名词");
        case 枚举_概念域::因果:     return 语素集.添加词性词("概念域_因果", "名词");
        case 枚举_概念域::自然语言: return 语素集.添加词性词("概念域_自然语言", "名词");
        default: return nullptr;
        }
    }

    static const 词性节点类* 私有_取关系词(枚举_概念关系 r) {
        switch (r) {
        case 枚举_概念关系::is_a:        return 语素集.添加词性词("概念关系_is_a", "名词");
        case 枚举_概念关系::part_of:     return 语素集.添加词性词("概念关系_part_of", "名词");
        case 枚举_概念关系::opposite_of: return 语素集.添加词性词("概念关系_opposite_of", "名词");
        case 枚举_概念关系::measured_by: return 语素集.添加词性词("概念关系_measured_by", "名词");
        case 枚举_概念关系::causes:      return 语素集.添加词性词("概念关系_causes", "名词");
        case 枚举_概念关系::maps_to:     return 语素集.添加词性词("概念关系_maps_to", "名词");
        case 枚举_概念关系::state_of:    return 语素集.添加词性词("概念关系_state_of", "名词");
        default: return nullptr;
        }
    }
};
