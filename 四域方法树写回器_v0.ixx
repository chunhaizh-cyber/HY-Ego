module;

#include <string>
#include <string_view>
#include <utility>

export module 四域方法树写回器_v0;

import 主信息定义模块;
import 数据仓库模块;
import 方法模块;
import 四域方法树匹配器_v0;
import 语素环境模块;

namespace 四域方法树写回器_v0_内部 {

using namespace 数据仓库模块;

inline const 词性节点类* 动作词_(std::string_view 名称)
{
    return 语素集.添加词性词(std::string(名称), "动词");
}

inline 方法首节点主信息类* 取首信息_(方法节点类* 节点) noexcept
{
    return 节点 && 节点->主信息
        ? dynamic_cast<方法首节点主信息类*>(节点->主信息)
        : nullptr;
}

inline 方法条件节点主信息类* 取条件信息_(方法节点类* 节点) noexcept
{
    return 节点 && 节点->主信息
        ? dynamic_cast<方法条件节点主信息类*>(节点->主信息)
        : nullptr;
}

inline 方法结果节点主信息类* 取结果信息_(方法节点类* 节点) noexcept
{
    return 节点 && 节点->主信息
        ? dynamic_cast<方法结果节点主信息类*>(节点->主信息)
        : nullptr;
}

template<class 谓词>
inline 方法节点类* 查找首个子节点_(方法节点类* 父节点, 谓词&& 匹配) noexcept
{
    auto* first = static_cast<方法节点类*>(父节点 ? 父节点->子 : nullptr);
    if (!first) return nullptr;

    auto* cur = first;
    do {
        if (匹配(cur)) return cur;
        cur = static_cast<方法节点类*>(cur->下);
    } while (cur && cur != first);
    return nullptr;
}

inline bool 动作句柄相同_(const 结构体_动作句柄& 左, const 结构体_动作句柄& 右) noexcept
{
    if (左.类型 != 右.类型) return false;
    switch (左.类型) {
    case 枚举_动作句柄类型::本能函数ID:
    return 左.本能ID == 右.本能ID;
    case 枚举_动作句柄类型::外部实现主键:
        return 左.外部实现主键 == 右.外部实现主键;
    case 枚举_动作句柄类型::动作序列主键:
        return 左.动作序列主键 == 右.动作序列主键;
    default:
        return false;
    }
}

inline 方法节点类* 创建子首节点_(
    方法类& 方法集,
    方法节点类* 父节点,
    std::string_view 动作名,
    const 结构体_动作句柄& 动作句柄,
    枚举_方法沉淀来源 来源,
    bool& 是否新建,
    const std::string& 调用点)
{
    是否新建 = false;
    if (!父节点) return nullptr;

    {
        锁调度器守卫 锁({
            锁请求::写(方法链.链表锁, 枚举_锁域::方法链, "方法链", 调用点)
            });

        if (auto* 已有 = 查找首个子节点_(父节点, [&](方法节点类* 节点) {
            auto* hmi = 取首信息_(节点);
            return hmi && 动作句柄相同_(hmi->动作句柄, 动作句柄);
            })) {
            if (auto* 首信息 = 取首信息_(已有)) {
                if (首信息->来源 == 枚举_方法沉淀来源::未定义) {
                    首信息->来源 = 来源;
                }
            }
            return 已有;
        }

        auto* mi = new 方法首节点主信息类(动作词_(动作名), 动作句柄);
        mi->来源 = 来源;
        auto* 节点 = static_cast<方法节点类*>(方法链.添加子节点_已加锁(
            父节点,
            static_cast<方法信息基类*>(mi)));
        if (节点) {
            方法集.初始化方法虚拟存在信息(节点, 0, 调用点 + "/初始化方法虚拟存在");
            是否新建 = true;
        }
        return 节点;
    }
}

inline 方法节点类* 取或创建条件节点_(
    方法类& 方法集,
    方法节点类* 首节点,
    const std::string& 条件主键,
    bool& 是否新建,
    const std::string& 调用点)
{
    是否新建 = false;
    if (!首节点) return nullptr;

    if (auto* 已有 = 查找首个子节点_(首节点, [&](方法节点类* 节点) {
        auto* 条件信息 = 取条件信息_(节点);
        return 条件信息 && 条件信息->条件场景模板主键 == 条件主键;
        })) {
        return 已有;
    }

    auto* 节点 = 方法集.创建条件节点(首节点, {}, 条件主键, 0, 0, 调用点);
    是否新建 = 节点 != nullptr;
    return 节点;
}

inline 方法节点类* 取或创建结果节点_(
    方法类& 方法集,
    方法节点类* 条件节点,
    const 场景节点类* 结果场景模板,
    bool& 是否新建,
    const std::string& 调用点)
{
    是否新建 = false;
    if (!条件节点) return nullptr;

    if (!结果场景模板) {
        if (auto* 已有 = 查找首个子节点_(条件节点, [](方法节点类* 节点) {
            return 取结果信息_(节点) != nullptr;
            })) {
            return 已有;
        }
    }

    auto* 节点 = 方法集.创建结果节点(条件节点, 结果场景模板, 结果场景模板 != nullptr, 调用点);
    是否新建 = 节点 != nullptr;
    return 节点;
}

} // namespace 四域方法树写回器_v0_内部

export enum class 枚举_四域方法树生长动作_v0 : std::int32_t
{
    未变化 = 0,
    发芽 = 1,
    抽枝 = 2,
    分叉 = 3
};

export struct 结构_四域方法树写回请求_v0
{
    方法节点类* 域方法树根 = nullptr;
    方法节点类* 父结果节点 = nullptr;
    std::string 条件主键{};
    std::string 动作名{};
    结构体_动作句柄 动作句柄{};
    枚举_方法沉淀来源 来源 = 枚举_方法沉淀来源::运行期临时;
    const 场景节点类* 结果场景模板 = nullptr;
};

export struct 结构_四域方法树写回结果_v0
{
    bool 成功 = false;
    枚举_四域方法树生长动作_v0 生长动作 = 枚举_四域方法树生长动作_v0::未变化;
    方法节点类* 挂载结果节点 = nullptr;
    方法节点类* 写回首节点 = nullptr;
    方法节点类* 写回条件节点 = nullptr;
    方法节点类* 写回结果节点 = nullptr;
    std::string 原因说明{};
};

export class 四域方法树写回器_v0
{
public:
    explicit 四域方法树写回器_v0(方法类& 方法集引用)
        : 方法集_(方法集引用)
    {
    }

    结构_四域方法树写回结果_v0 按成功路径写回(
        const 结构_四域方法树写回请求_v0& 请求) const
    {
        using namespace 四域方法树写回器_v0_内部;

        结构_四域方法树写回结果_v0 返回{};
        if (!请求.域方法树根) {
            返回.原因说明 = "域方法树根为空";
            return 返回;
        }
        if (!请求.动作句柄.有效()) {
            返回.原因说明 = "动作句柄无效";
            return 返回;
        }
        if (请求.条件主键.empty()) {
            返回.原因说明 = "条件主键为空";
            return 返回;
        }

        auto* 挂载结果节点 = 请求.父结果节点 && 取结果信息_(请求.父结果节点)
            ? 请求.父结果节点
            : 获取域根默认结果节点_v0(请求.域方法树根);
        if (!挂载结果节点) {
            返回.原因说明 = "未找到域根默认结果挂载点";
            return 返回;
        }
        返回.挂载结果节点 = 挂载结果节点;

        bool 新建首 = false;
        auto* 写回首节点 = 创建子首节点_(
            方法集_,
            挂载结果节点,
            请求.动作名.empty() ? std::string_view("四域自动生长动作") : std::string_view(请求.动作名),
            请求.动作句柄,
            请求.来源,
            新建首,
            "四域方法树写回器_v0::按成功路径写回/创建首节点");
        if (!写回首节点) {
            返回.原因说明 = "写回首节点失败";
            return 返回;
        }

        bool 新建条件 = false;
        auto* 写回条件节点 = 取或创建条件节点_(
            方法集_,
            写回首节点,
            请求.条件主键,
            新建条件,
            "四域方法树写回器_v0::按成功路径写回/创建条件节点");
        if (!写回条件节点) {
            返回.原因说明 = "写回条件节点失败";
            return 返回;
        }

        bool 新建结果 = false;
        auto* 写回结果节点 = 取或创建结果节点_(
            方法集_,
            写回条件节点,
            请求.结果场景模板,
            新建结果,
            "四域方法树写回器_v0::按成功路径写回/创建结果节点");
        if (!写回结果节点) {
            返回.原因说明 = "写回结果节点失败";
            return 返回;
        }

        返回.成功 = true;
        返回.写回首节点 = 写回首节点;
        返回.写回条件节点 = 写回条件节点;
        返回.写回结果节点 = 写回结果节点;
        if (新建首) {
            返回.生长动作 = 挂载结果节点 == 获取域根默认结果节点_v0(请求.域方法树根)
                ? 枚举_四域方法树生长动作_v0::发芽
                : 枚举_四域方法树生长动作_v0::抽枝;
        }
        else if (新建条件) {
            返回.生长动作 = 枚举_四域方法树生长动作_v0::分叉;
        }
        else if (新建结果) {
            返回.生长动作 = 枚举_四域方法树生长动作_v0::抽枝;
        }
        else {
            返回.生长动作 = 枚举_四域方法树生长动作_v0::未变化;
        }
        返回.原因说明 = "已写回本域方法树";
        return 返回;
    }

    bool 更新方法枝来源(
        方法节点类* 方法首节点,
        枚举_方法沉淀来源 新来源,
        const std::string& 调用点 = "四域方法树写回器_v0::更新方法枝来源") const
    {
        (void)调用点;

        auto* 首信息 = 四域方法树写回器_v0_内部::取首信息_(方法首节点);
        if (!首信息) {
            return false;
        }
        首信息->来源 = 新来源;
        return true;
    }

private:
    方法类& 方法集_;
};
