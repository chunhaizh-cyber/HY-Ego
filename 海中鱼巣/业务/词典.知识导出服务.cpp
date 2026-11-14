#include "词典.知识导出服务.h"
#include "../领域/新_本能函数集.h"
#include "../领域/概念_存在类.h"
#include "../领域/概念_特征类.h"
#include "../领域/新_特征值类.h"

#include <algorithm>
#include <array>
#include <limits>
#include <new>
#include <stdexcept>
#include <thread>

namespace 海中鱼巣 {
namespace {
void 要求词典执行证据(bool 成立, const char* 诊断) {
    if (!成立) throw std::logic_error(诊断);
}
稳定编码 读取词典特征类型(概念_特征类& 概念, std::uint32_t 槽) {
    const auto r = 概念.读取当前先天特征类型槽(槽);
    if (r.状态 == 特征概念规则状态::资源失败) throw std::bad_alloc{};
    要求词典执行证据(r.状态 == 特征概念规则状态::已完成 && r.类型根.has_value(),
        "dictionary knowledge feature type unavailable");
    return *r.类型根;
}
}

新词典知识导出结果 导出词典条件结果(
    const 新方法条件定义& 基础条件, const 新本能函数本次条件& 动作前条件,
    const 新词典本能输入& 输入, 稳定编码 正式W,
    const 新本能词典读取回执& 终态, 概念_存在类& 存在概念,
    概念_特征类& 特征概念, const 新_特征值类& 值) {
    // 下列判断仅关联同一次执行的值式证据，不证明后来世界仍未变化。
    要求词典执行证据(终态.查阅结局 == 新词典查阅结局::命中
        && 终态.采用阶段 == 新词典材料采用阶段::已采用 && 终态.材料Q
        && 有效(终态.动作) && 终态.词条 == 输入.词条 && 终态.材料特征 == 输入.材料特征
        && 终态.变化阶段 && *终态.变化阶段 != 新词典变化处理阶段::待处理,
        "dictionary knowledge requires an adopted terminal receipt of this invocation");
    要求词典执行证据(!输入.批次摘要.empty() && 输入.外设场景标识 > 0
        && 输入.外设场景标识 <= static_cast<std::uint64_t>((std::numeric_limits<std::int64_t>::max)()),
        "dictionary knowledge source input malformed");
    const std::array 节点{输入.词条, 输入.材料特征, 输入.来源存在,
        输入.执行主体, 输入.批次特征, 输入.定位特征};
    const std::array 类型{新方法参数信息类型::词条, 新方法参数信息类型::特征,
        新方法参数信息类型::存在, 新方法参数信息类型::存在,
        新方法参数信息类型::特征, 新方法参数信息类型::特征};
    要求词典执行证据(动作前条件.参数角色组.size() == 节点.size(),
        "dictionary knowledge requires six actual parameters");
    for (std::size_t i = 0; i < 节点.size(); ++i) {
        const auto matches = std::count_if(动作前条件.参数角色组.begin(), 动作前条件.参数角色组.end(),
            [&](const auto& row) { return row.有序引用 && row.有序引用->序号 == i + 1
                && row.有序引用->信息类型 == 类型[i] && row.有序引用->信息节点 == 节点[i]; });
        要求词典执行证据(matches == 1, "dictionary knowledge actual parameter identity mismatch");
    }

    const auto 同键 = 存在概念.建立或取得词条同键范围(正式W);
    if (同键.状态 == 新词条同键操作状态::未找到)
        return {新词典知识导出状态::正式来源已退出, {}};
    if (同键.状态 == 新词条同键操作状态::资源失败) throw std::bad_alloc{};
    要求词典执行证据((同键.状态 == 新词条同键操作状态::已建立
        || 同键.状态 == 新词条同键操作状态::已复用) && 同键.信息,
        "dictionary knowledge same-key concept unavailable");
    要求词典执行证据(同键.信息->词条语言 == 输入.键.词条语言
        && 同键.信息->规范化合同 == 输入.键.规范化合同
        && 同键.信息->原规范化词面 == 输入.键.原规范化词面,
        "dictionary knowledge formal word differs from actual immutable query key");
    const auto 材料FT = 读取词典特征类型(特征概念, 先天特征类型槽::词典材料);
    const auto 批次FT = 读取词典特征类型(特征概念, 先天特征类型槽::词典来源批次);

    // 原登记合同在位置3只声明批次与来源标识两种F；不借本地F反查正式owner。
    std::optional<稳定编码> 定位FT;
    for (const auto& c : 基础条件.条件项组) {
        if (c.参数序号 != 3 || !c.特征类型概念节点 || *c.特征类型概念节点 == 批次FT) continue;
        要求词典执行证据(!定位FT || *定位FT == *c.特征类型概念节点,
            "dictionary knowledge source declaration is ambiguous");
        定位FT = c.特征类型概念节点;
    }
    要求词典执行证据(定位FT.has_value(), "dictionary knowledge source declaration missing");
    // 模拟动作前V可能是localV；用本次固定正文定位原本已存在的正式共享V。
    const auto 批次值 = 值.查询特征值(材料物理类型::UTF8字符串, 输入.批次摘要);
    要求词典执行证据(批次值.size() == 1, "dictionary knowledge shared batch material unavailable");
    稳定编码 批次范围;
    for (;;) {
        const auto 批次根 = 特征概念.获取特征概念(批次FT);
        要求词典执行证据(批次根.has_value(), "dictionary knowledge batch type unavailable");
        特征概念定义 批次域;
        批次域.材料类型 = 批次根->材料类型; 批次域.单位 = 批次根->单位;
        批次域.比较规则 = 批次根->比较规则; 批次域.聚合规则 = 批次根->聚合规则;
        批次域.值域 = 批次值;
        const auto 批次C = 特征概念.建立或取得特征概念强类型(批次域, 值, 批次FT);
        if (批次C.状态 == 特征概念规则状态::前提已变化) {
            std::this_thread::yield(); continue;
        }
        if (批次C.状态 == 特征概念规则状态::资源失败) throw std::bad_alloc{};
        要求词典执行证据((批次C.状态 == 特征概念规则状态::已完成
            || 批次C.状态 == 特征概念规则状态::无变化) && 批次C.概念节点,
            "dictionary knowledge exact batch domain unavailable");
        批次范围 = *批次C.概念节点;
        break;
    }
    const auto Q范围 = 存在概念.读取词典材料身份范围概念();
    const auto 材料根 = 特征概念.获取特征概念(材料FT);
    要求词典执行证据(Q范围.状态 == 正式结构操作状态::已读取 && Q范围.信息 && 材料根,
        "dictionary knowledge material identity domain unavailable");
    const auto* Q域 = std::get_if<特征概念信息引用值域>(&材料根->值域);
    要求词典执行证据(Q域 && Q域->语义 == 信息引用语义::信息对象
        && Q域->内容概念 == Q范围.信息->节点, "dictionary knowledge material domain is not Q");

    新方法条件结果定义 pair;
    pair.条件 = 基础条件;
    要求词典执行证据(pair.条件.关系规则 == 新方法项关系规则::全部成立
        || pair.条件.关系规则 == 新方法项关系规则::单项,
        "dictionary knowledge requires a conjunctive execution branch");
    pair.条件.关系规则 = 新方法项关系规则::全部成立;
    bool 已收窄W = false;
    for (auto& c : pair.条件.条件项组) {
        if (c.参数序号 == 1 && c.存在概念节点) {
            c.存在概念节点 = 同键.信息->概念节点; 已收窄W = true;
        }
    }
    if (!已收窄W) {
        新方法条件项定义 c; c.参数序号 = 1; c.存在概念节点 = 同键.信息->概念节点;
        pair.条件.条件项组.push_back(std::move(c));
    }
    新方法条件项定义 batch;
    batch.参数序号 = 5; batch.特征类型概念节点 = 批次FT; batch.特征值域概念节点 = 批次范围;
    if (std::find(pair.条件.条件项组.begin(), pair.条件.条件项组.end(), batch) == pair.条件.条件项组.end())
        pair.条件.条件项组.push_back(std::move(batch));
    新方法条件项定义 location;
    location.参数序号 = 6; location.特征类型概念节点 = 定位FT;
    const auto number = static_cast<std::int64_t>(输入.外设场景标识);
    location.精确数值域 = 特征概念I64闭区间{number, number};
    if (std::find(pair.条件.条件项组.begin(), pair.条件.条件项组.end(), location) == pair.条件.条件项组.end())
        pair.条件.条件项组.push_back(std::move(location));
    新方法结果项定义 result;
    result.参数序号 = 2; result.特征类型概念节点 = 材料FT; result.特征值域概念节点 = 材料FT;
    pair.结果.结果项组.push_back(std::move(result));
    if (动作前条件.执行分支节点) pair.来源执行分支节点组.push_back(*动作前条件.执行分支节点);
    return {新词典知识导出状态::已形成, std::move(pair)};
}
} // namespace 海中鱼巣
