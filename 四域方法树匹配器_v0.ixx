module;

#include <cstdint>
#include <string>
#include <string_view>

export module 四域方法树匹配器_v0;

import 主信息定义模块;

namespace 四域方法树匹配器_v0_内部 {

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

inline bool 是否默认条件_(const 方法条件节点主信息类* 条件信息) noexcept
{
    return 条件信息
        && 条件信息->条件场景模板主键.find("默认条件") != std::string::npos;
}

inline bool 是否默认续接_(const 方法首节点主信息类* 首信息) noexcept
{
    return 首信息
        && 首信息->动作句柄.类型 == 枚举_动作句柄类型::动作序列主键
        && 首信息->动作句柄.动作序列主键.find("默认续接") != std::string::npos;
}

inline std::string 指针键_(const void* 指针)
{
    return 指针 ? std::to_string(reinterpret_cast<std::uintptr_t>(指针)) : std::string("0");
}

inline std::string 桶化Q10000_(std::int64_t 数值)
{
    if (数值 <= 0) return "0";
    if (数值 < 3000) return "低";
    if (数值 < 7000) return "中";
    return "高";
}

} // namespace 四域方法树匹配器_v0_内部

export struct 结构_四域方法树条件签名输入_v0
{
    std::string 域标签{};
    存在节点类* 任务虚拟存在 = nullptr;
    方法节点类* 当前方法首节点 = nullptr;

    std::int64_t 安全值_Q10000 = 0;
    std::int64_t 服务值_Q10000 = 0;
    std::int64_t 需求值_Q10000 = 0;
    std::int64_t 当前预算_Q10000 = 0;

    bool 最近结果稳定 = true;
    bool 最近执行失败 = false;
    bool 风险变坏 = false;
    bool 检查点通过 = true;
    bool 依赖缺失 = false;
    bool 需要新子任务或新步骤 = false;
    bool 关键方法不再可用 = false;
    bool 已满足完成判据 = false;
};

export struct 结构_四域方法树匹配请求_v0
{
    方法节点类* 域方法树根 = nullptr;
    std::string 条件主键{};
};

export struct 结构_四域方法树匹配结果_v0
{
    bool 找到 = false;
    bool 需转本域兜底 = false;
    方法节点类* 域方法树根 = nullptr;
    方法节点类* 父结果节点 = nullptr;
    方法节点类* 命中首节点 = nullptr;
    方法节点类* 命中条件节点 = nullptr;
    方法节点类* 命中结果节点 = nullptr;
    方法节点类* 建议执行首节点 = nullptr;
    std::string 原因说明{};
};

export 方法节点类* 获取域根默认结果节点_v0(方法节点类* 域方法树根)
{
    using namespace 四域方法树匹配器_v0_内部;

    auto* 默认条件 = 查找首个子节点_(
        域方法树根,
        [](方法节点类* 节点) {
            return 是否默认条件_(取条件信息_(节点));
        });
    if (!默认条件) {
        默认条件 = 查找首个子节点_(
            域方法树根,
            [](方法节点类* 节点) { return 取条件信息_(节点) != nullptr; });
    }
    if (!默认条件) return nullptr;

    return 查找首个子节点_(
        默认条件,
        [](方法节点类* 节点) { return 取结果信息_(节点) != nullptr; });
}

export class 四域方法树匹配器_v0
{
public:
    static std::string 生成条件主键(const 结构_四域方法树条件签名输入_v0& 输入)
    {
        using namespace 四域方法树匹配器_v0_内部;

        std::string 主键 = "四域方法树/";
        主键 += 输入.域标签.empty() ? "未定义" : 输入.域标签;
        主键 += "/条件";
        主键 += "/任务虚拟存在=" + 指针键_(输入.任务虚拟存在);
        主键 += "/当前方法=" + 指针键_(输入.当前方法首节点);
        主键 += "/安全=" + 桶化Q10000_(输入.安全值_Q10000);
        主键 += "/服务=" + 桶化Q10000_(输入.服务值_Q10000);
        主键 += "/需求=" + 桶化Q10000_(输入.需求值_Q10000);
        主键 += "/预算=" + 桶化Q10000_(输入.当前预算_Q10000);
        主键 += 输入.最近结果稳定 ? "/稳" : "/不稳";
        主键 += 输入.最近执行失败 ? "/失败" : "/未失败";
        主键 += 输入.风险变坏 ? "/风险坏" : "/风险稳";
        主键 += 输入.检查点通过 ? "/检过" : "/检未过";
        主键 += 输入.依赖缺失 ? "/缺依赖" : "/依赖足";
        主键 += 输入.需要新子任务或新步骤 ? "/需细分" : "/不细分";
        主键 += 输入.关键方法不再可用 ? "/方法失效" : "/方法可用";
        主键 += 输入.已满足完成判据 ? "/已完成" : "/未完成";
        return 主键;
    }

    结构_四域方法树匹配结果_v0 匹配(
        const 结构_四域方法树匹配请求_v0& 请求) const
    {
        using namespace 四域方法树匹配器_v0_内部;

        结构_四域方法树匹配结果_v0 返回{};
        返回.域方法树根 = 请求.域方法树根;
        返回.父结果节点 = 获取域根默认结果节点_v0(请求.域方法树根);
        if (!请求.域方法树根 || !返回.父结果节点) {
            返回.需转本域兜底 = true;
            返回.原因说明 = "域方法树缺少默认结果挂载点";
            return 返回;
        }

        auto* firstHead = static_cast<方法节点类*>(返回.父结果节点->子);
        if (!firstHead) {
            返回.需转本域兜底 = true;
            返回.原因说明 = "域方法树尚无可匹配子枝";
            return 返回;
        }

        auto* cur = firstHead;
        do {
            auto* 首信息 = 取首信息_(cur);
            if (!首信息 || 是否默认续接_(首信息)) {
                cur = static_cast<方法节点类*>(cur->下);
                continue;
            }

            auto* 命中条件 = 查找首个子节点_(
                cur,
                [&](方法节点类* 节点) {
                    auto* 条件信息 = 取条件信息_(节点);
                    return 条件信息
                        && !条件信息->条件场景模板主键.empty()
                        && 条件信息->条件场景模板主键 == 请求.条件主键;
                });
            if (命中条件) {
                返回.找到 = true;
                返回.命中首节点 = cur;
                返回.命中条件节点 = 命中条件;
                返回.命中结果节点 = 查找首个子节点_(
                    命中条件,
                    [](方法节点类* 节点) { return 取结果信息_(节点) != nullptr; });
                返回.建议执行首节点 = cur;
                返回.原因说明 = "命中本域方法树已有枝条";
                return 返回;
            }

            cur = static_cast<方法节点类*>(cur->下);
        } while (cur && cur != firstHead);

        返回.需转本域兜底 = true;
        返回.原因说明 = "未命中特定方法枝，需转本域兜底";
        return 返回;
    }
};
