#include "模拟学习.服务.h"
#include "模拟学习.算法.h"
#include "../领域/新_模拟世界类.h"
#include "../领域/新_方法类.h"
#include "../领域/新_本能函数管理类.h"
#include "../领域/概念_特征类.h"
#include "../核心/容错检查.h"
#include <algorithm>
#include <limits>
#include <memory>
#include <new>

namespace 海中鱼巣 {
新本能函数本次条件结果对 新_模拟世界类::执行贡献治理场景(const 方法参数场景&) noexcept {
    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"模拟贡献治理",
        L"贡献治理尚无隔离模拟合同，禁止访问真实任务或治理世界。");
}
新本能函数本次条件结果对 新_模拟世界类::消费最终来源(const 方法参数场景&) noexcept {
    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"模拟最终贡献消费",
        L"Q结算尚无模拟合同，禁止进入真实消费或借用旧加一样例。");
}
namespace {
bool 同一学习条件(const 新方法条件定义& 左, const 新方法条件定义& 右) {
    if (左.关系规则 != 右.关系规则) return false;
    if (左.关系规则 == 新方法项关系规则::全部成立
        || 左.关系规则 == 新方法项关系规则::任一成立) {
        return std::is_permutation(左.条件项组.begin(), 左.条件项组.end(),
            右.条件项组.begin(), 右.条件项组.end());
    }
    return 左 == 右;
}
std::optional<std::int64_t> 读取单值域(const 概念_特征类& 服务, 稳定编码 节点, 稳定编码 类型) {
    const auto info = 服务.获取特征概念(节点);
    const auto type = 服务.获取特征概念(类型);
    if (!info || !type || info->材料类型 != 特征概念材料物理类型::I64标量
        || type->材料类型 != 特征概念材料物理类型::I64标量 || info->单位 != type->单位) return std::nullopt;
    const auto 比较 = 服务.比较单特征概念(节点, 类型);
    if (比较.状态 != 特征概念规则状态::已完成 || 比较.特征类型根 != 类型 || !比较.关系
        || (*比较.关系 != 单特征概念关系::同一 && *比较.关系 != 单特征概念关系::右为上位)) return std::nullopt;
    const auto* 域 = std::get_if<std::vector<特征概念I64闭区间>>(&info->值域);
    if (!域 || 域->size() != 1 || 域->front().下界 != 域->front().上界
        || 域->front().下界 == (std::numeric_limits<std::int64_t>::min)()
        || 域->front().下界 == (std::numeric_limits<std::int64_t>::max)()) return std::nullopt;
    return 域->front().下界;
}
}
模拟学习服务::模拟学习服务(新_方法类& 方法, 概念_特征类& 概念,
    const 新服务值结算本能端口& 登记) noexcept : 方法_(方法), 概念_(概念), 登记读取_(登记) {}

新学习运行异常::新学习运行异常(稳定编码 方法节点, 新学习运行异常原因 原因) noexcept
    : 记录_{方法节点,原因} {}
新学习运行异常::新学习运行异常(稳定编码 方法节点, 新学习运行异常原因 原因, std::string 诊断)
    : 记录_{方法节点, 原因, std::move(诊断)} {}
const 新学习运行异常记录& 新学习运行异常::记录() const noexcept { return 记录_; }
const char* 新学习运行异常::what() const noexcept {
    if (!记录_.诊断.empty()) return 记录_.诊断.c_str();
    switch (记录_.原因) {
    case 新学习运行异常原因::规格未支持: return "unsupported simulation specification";
    case 新学习运行异常原因::停止取消: return "learning operation cancelled";
    case 新学习运行异常原因::输入不合法: return "invalid learning input";
    case 新学习运行异常原因::资源不足: return "learning resource exhausted";
    case 新学习运行异常原因::结构不一致: return "learning structure inconsistent";
    case 新学习运行异常原因::本能运行异常: return "instinct execution abnormal";
    case 新学习运行异常原因::发布结果未知: return "learning publication indeterminate";
    default: return "learning internal error";
    }
}

新学习结束通知 模拟学习服务::学习(稳定编码 方法节点, const std::function<bool()>& 停止检查) {
    using 原因 = 新学习运行异常原因;
    bool 已发表配对 = false;
    const auto 检查停止 = [&]() {
        bool 停止=false;
        try { 停止=停止检查 && 停止检查(); }
        catch (const std::bad_alloc&) { throw 新学习运行异常(方法节点,原因::资源不足); }
        catch (...) { throw 新学习运行异常(方法节点,原因::内部错误); }
        if (停止) throw 新学习运行异常(方法节点,原因::停止取消);
    };
    try {
        检查停止();
        if (!有效(方法节点)) throw 新学习运行异常(方法节点,原因::输入不合法);
        const auto 方法 = 方法_.获取方法(方法节点);
        if (!方法) throw 新学习运行异常(方法节点,原因::结构不一致);
        if (方法->来源方法节点 || 方法->实例根节点 || 方法->实现方式 != 新方法实现方式::本能)
            throw 新学习运行异常(方法节点,原因::输入不合法);
        if (!方法->本能函数登记名 || 方法->本能函数登记名->empty())
            throw 新学习运行异常(方法节点,原因::结构不一致);
        // 旧加一方法的知识保持原样；停用后的学习消息不再产生样例或写入。
        if (方法->本能函数登记名 == 新本能函数登记名_服务值结算
            && 方法->启用状态 != 新方法启用状态::启用) return {方法节点};
        if (方法->启用状态 != 新方法启用状态::启用)
            throw 新学习运行异常(方法节点,原因::输入不合法);
        if(方法->参数规格组.empty()||方法->参数规格组.front().有序规格) {
            学习有序本能(*方法,停止检查);return {方法节点};
        }
        if (方法->本能函数登记名 != 新本能函数登记名_服务值结算)
            throw 新学习运行异常(方法节点,原因::规格未支持,"legacy role parameter instantiation is not generic");
        const auto 登记 = 方法_.查询本能登记(std::string(新本能函数登记名_服务值结算));
        if (登记.状态 == 新方法操作状态::资源失败)
            throw 新学习运行异常(方法节点,原因::资源不足);
        if (登记.状态 != 新方法操作状态::已找到 || 登记.方法节点 != 方法节点)
            throw 新学习运行异常(方法节点,原因::结构不一致);
        // Q 消费依赖最终贡献证明与真实原子消费，旧加一样例不能替代该语义。
        // 登记查询已核验 Q 的完整规格；在读取现实端口、构造样例之前结束。
        if (std::any_of(方法->参数规格组.begin(), 方法->参数规格组.end(),
            [](const auto& 规格) { return 规格.对象种类 == 新方法参数对象种类::任务虚拟存在
                && 规格.特征规格组.size() == 2; })) return {方法节点};
        // 唯一现实端口调用是只读登记材料；不借现实当前值扩大样例范围。
        const auto 材料 = 登记读取_.读取登记材料();
        if (!材料) throw 新学习运行异常(方法节点,原因::结构不一致);
        const auto& m = *材料;
        if (m.冻结前值 < 0 || m.冻结前值 >= (std::numeric_limits<std::int64_t>::max)() - 1
            || 方法->参数规格组.size() != 2) throw 新学习运行异常(方法节点,原因::输入不合法);
        bool 有服务 = false, 有任务 = false;
        for (const auto& 规格 : 方法->参数规格组) {
            if (规格.存在概念节点 || 规格.特征规格组.empty())
                throw 新学习运行异常(方法节点,原因::输入不合法);
            const auto& 特征 = 规格.特征规格组.front();
            if (!特征.值域概念节点 || !特征.二次特征概念组.empty())
                throw 新学习运行异常(方法节点,原因::结构不一致);
            if (规格.参数角色节点 == m.服务值类型根 && !有服务
                && 规格.对象种类 == 新方法参数对象种类::普通世界 && 规格.要求场景
                && !规格.集合成员约束 && 规格.特征规格组.size() == 2
                && 规格.特征规格组[1] == 新方法参数特征规格{m.服务任务集合类型根,m.服务任务非空值域,{}}
                && 特征.特征类型概念节点 == m.服务值类型根 && 特征.值域概念节点 == m.动作前单值域
                && 读取单值域(概念_, *特征.值域概念节点, m.服务值类型根) == m.冻结前值) 有服务 = true;
            else if (规格.参数角色节点 == m.任务状态类型根 && !有任务
                && 规格.对象种类 == 新方法参数对象种类::任务虚拟存在 && !规格.要求场景
                && 规格.特征规格组.size() == 1
                && 规格.集合成员约束 == 新方法参数集合成员约束{m.服务值类型根,m.服务任务集合类型根}
                && 特征.特征类型概念节点 == m.任务状态类型根 && 特征.值域概念节点 == m.任务完成值域
                && 读取单值域(概念_, *特征.值域概念节点, m.任务状态类型根) == 9) 有任务 = true;
            else throw 新学习运行异常(方法节点,原因::输入不合法);
        }
        if (!有服务 || !有任务 || !有效(m.增加方向概念))
            throw 新学习运行异常(方法节点,原因::结构不一致);
        const auto 集合定义 = 概念_.获取特征概念(m.服务任务非空值域);
        特征概念任务集合值域 非空要求; 非空要求.至少一个任务 = true;
        if (!集合定义 || 集合定义->值域 != 特征概念值域{非空要求})
            throw 新学习运行异常(方法节点,原因::结构不一致);
        数值模拟细分器 细分;
        std::exception_ptr 求值异常;
        std::shared_ptr<新_模拟世界类> 样例;
        const auto 求值 = [&](const std::vector<std::int64_t>& 输入) -> 数值模拟样例结果 {
          try {
            检查停止();
            if (输入 != std::vector<std::int64_t>{m.冻结前值,9})
                throw 新学习运行异常(方法节点,原因::结构不一致);
            样例 = std::make_shared<新_模拟世界类>(方法_);
            const auto 建立 = 样例->建立服务结算样例(方法节点, m, 输入[0], 输入[1]);
            if (建立.状态 != 新模拟世界状态::已完成)
                throw 新学习运行异常(方法节点,建立.状态==新模拟世界状态::资源失败
                    ? 原因::资源不足 : 原因::结构不一致);
            const auto 绑定 = 样例->读取参数绑定();
            if (!绑定) throw 新学习运行异常(方法节点,原因::结构不一致);
            const auto 场景 = 方法_.建立模拟参数场景(方法节点, *绑定, 样例);
            if (场景.状态 == 新方法操作状态::资源失败)
                throw 新学习运行异常(方法节点,原因::资源不足);
            if (!场景.参数场景) throw 新学习运行异常(方法节点,原因::结构不一致);
            检查停止();
            const auto 调用 = 方法_.调用本能函数(方法节点, *场景.参数场景);
            const auto fresh = 样例->读取样例();
            if (调用.状态 == 新方法本能调用状态::资源失败)
                throw 新学习运行异常(方法节点,原因::资源不足);
            if (调用.状态 != 新方法本能调用状态::已调用 || !调用.本次条件结果对)
                throw 新学习运行异常(方法节点,原因::本能运行异常);
            if (!fresh || !fresh->动作节点 || !fresh->前状态节点 || fresh->任务当前值 != 9)
                throw 新学习运行异常(方法节点,原因::结构不一致);
            const auto& 运行 = *调用.本次条件结果对;
            if (运行.结果.状态 != 新本能函数执行状态::已按合同返回
                || 运行.结果.动作阶段 != 新本能函数动作阶段::已按函数合同返回)
                throw 新学习运行异常(方法节点,原因::本能运行异常);
            if (运行.结果.动作动态节点 != fresh->动作节点
                || 运行.条件.参数角色组.size() != 2)
                throw 新学习运行异常(方法节点,原因::结构不一致);
            const std::vector<新本能函数条件角色材料> 预期条件{
                {m.服务值类型根,fresh->场景节点,fresh->自我节点,
                    {{fresh->服务特征节点,新特征准确值{输入[0]}},
                        {fresh->集合特征节点,新特征准确值{fresh->集合材料节点}}},std::nullopt},
                {m.任务状态类型根,std::nullopt,std::nullopt,
                    {{fresh->来源任务.状态特征节点,新特征准确值{输入[1]}}},fresh->来源任务}};
            if (运行.条件.参数角色组 != 预期条件)
                throw 新学习运行异常(方法节点,原因::结构不一致);
            // 正常未知不读取残留后状态，不以缺少尚未取得的结果包装工程失败。
            switch (运行.结果.结果取得状态) {
            case 新本能函数结果取得状态::未取得:
            case 新本能函数结果取得状态::待取得:
            case 新本能函数结果取得状态::部分取得:
            case 新本能函数结果取得状态::无法取得:
                return {数值模拟样例状态::结果未知,{}};
            case 新本能函数结果取得状态::已取得: break;
            default: throw 新学习运行异常(方法节点,原因::结构不一致);
            }
            if (!fresh->后状态节点 || !fresh->结果动态节点
                || 运行.结果.已形成状态节点组 != std::vector<稳定编码>{*fresh->后状态节点}
                || 运行.结果.已形成动态节点组 != std::vector<稳定编码>{*fresh->结果动态节点})
                throw 新学习运行异常(方法节点,原因::结构不一致);
            return {数值模拟样例状态::已求值,{fresh->服务当前值}};
          } catch (...) { 求值异常=std::current_exception(); return {}; }
        };
        const auto 初始化 = 细分.初始化({{{m.冻结前值,m.冻结前值},{9,9}},1},求值);
        if (求值异常) std::rethrow_exception(求值异常);
        if (初始化 != 数值模拟操作状态::就绪)
            throw 新学习运行异常(方法节点,初始化==数值模拟操作状态::资源失败
                ? 原因::资源不足 : 原因::结构不一致);
        for (;;) {
            检查停止();
            const auto 推进 = 细分.推进();
            if (求值异常) std::rethrow_exception(求值异常);
            if (推进.状态 == 数值模拟操作状态::已完成) break;
            if (推进.状态 != 数值模拟操作状态::推进中 && 推进.状态 != 数值模拟操作状态::就绪) {
                throw 新学习运行异常(方法节点,推进.状态==数值模拟操作状态::资源失败
                    ? 原因::资源不足 : 原因::结构不一致);
            }
        }
        const auto 样例材料 = 细分.读取材料();
        if (样例材料.状态 == 数值模拟操作状态::资源失败)
            throw 新学习运行异常(方法节点,原因::资源不足);
        if (样例材料.状态 == 数值模拟操作状态::已完成
            && (!样例材料.未知输入组.empty() || !样例材料.不合法输入组.empty()
                || !样例材料.同输入差异组.empty() || !样例材料.未完善区域组.empty()))
            return {方法节点}; // 本轮结束，未知/差异不交给旧的单R发布入口。
        if (样例材料.状态 != 数值模拟操作状态::已完成 || 样例材料.样例组.size() != 1
            || 样例材料.实际求值组.size() != 2 || 样例材料.实际求值组[0].收尾复测
            || !样例材料.实际求值组[1].收尾复测 || 样例材料.完成区域组.size() != 1
            || 样例材料.样例组.front().输入值组 != std::vector<std::int64_t>{m.冻结前值,9}
            || 样例材料.样例组.front().输出值组.size() != 1) {
            throw 新学习运行异常(方法节点,原因::结构不一致);
        }
        const auto fresh = 样例 ? 样例->读取样例() : std::nullopt;
        const auto 输出 = 样例材料.样例组.front().输出值组.front();
        if (!fresh || fresh->服务当前值 != 输出 || !fresh->动作节点 || !fresh->前状态节点
            || !fresh->后状态节点 || !fresh->结果动态节点 || fresh->任务当前值 != 9) {
            throw 新学习运行异常(方法节点,原因::结构不一致);
        }
        新方法条件结果定义 新配对;
        新配对.条件.关系规则 = 新方法项关系规则::全部成立;
        新方法条件项定义 服务条件; 服务条件.参数角色节点 = m.服务值类型根;
        服务条件.特征类型概念节点 = m.服务值类型根; 服务条件.精确数值域 = 特征概念I64闭区间{m.冻结前值,m.冻结前值};
        新方法条件项定义 任务条件; 任务条件.参数角色节点 = m.任务状态类型根;
        任务条件.特征类型概念节点 = m.任务状态类型根; 任务条件.精确数值域 = 特征概念I64闭区间{9,9};
        新方法条件项定义 集合条件; 集合条件.参数角色节点 = m.服务值类型根;
        集合条件.特征类型概念节点 = m.服务任务集合类型根;
        集合条件.特征值域概念节点 = m.服务任务非空值域;
        新配对.条件.条件项组 = {服务条件,任务条件,集合条件};
        新方法结果项定义 服务结果; 服务结果.参数角色节点 = m.服务值类型根;
        服务结果.特征类型概念节点 = m.服务值类型根; 服务结果.精确数值域 = 特征概念I64闭区间{输出,输出};
        服务结果.变化方向概念节点 = m.增加方向概念; 服务结果.主要结果 = true;
        新配对.结果.结果项组 = {服务结果};
        检查停止();
        const auto 发布 = 方法_.追加模拟条件结果(方法节点,{新配对});
        已发表配对 = 发布.状态 == 新方法学习发布状态::已发布
            || 发布.状态 == 新方法学习发布状态::发布结果未知;
        switch (发布.状态) {
        case 新方法学习发布状态::发布结果未知:
            throw 新学习运行异常(方法节点,原因::发布结果未知);
        case 新方法学习发布状态::资源失败:
            throw 新学习运行异常(方法节点,原因::资源不足);
        case 新方法学习发布状态::入口拒绝:
        case 新方法学习发布状态::条件结果不合法:
            throw 新学习运行异常(方法节点,原因::输入不合法);
        case 新方法学习发布状态::方法不可用:
        case 新方法学习发布状态::结构不一致:
            throw 新学习运行异常(方法节点,原因::结构不一致);
        default: break;
        }
        if (发布.状态 == 新方法学习发布状态::同条件结果冲突) {
            if (!发布.新配对节点组.empty() || !发布.已有配对节点 || !有效(*发布.已有配对节点)) {
                throw 新学习运行异常(方法节点,原因::结构不一致);
            }
            const auto 已有方法=方法_.获取方法(方法节点);
            if (!已有方法 || std::count_if(已有方法->条件结果组.begin(),已有方法->条件结果组.end(),
                [&](const auto& 对) { return 对.配对节点==*发布.已有配对节点
                    && 同一学习条件(对.条件,新配对.条件) && 对.结果!=新配对.结果; })!=1)
                throw 新学习运行异常(方法节点,原因::结构不一致);
            return {方法节点};
        }
        const bool 重复 = 发布.状态 == 新方法学习发布状态::精确重复;
        if ((!重复 && (发布.状态 != 新方法学习发布状态::已发布
                || 发布.新配对节点组.size() != 1 || 发布.已有配对节点))
            || (重复 && (!发布.新配对节点组.empty() || !发布.已有配对节点
                || !有效(*发布.已有配对节点)))) {
            throw 新学习运行异常(方法节点,已发表配对 ? 原因::发布结果未知 : 原因::结构不一致);
        }
        const auto 配对节点 = 重复 ? *发布.已有配对节点 : 发布.新配对节点组.front();
        const auto 最终方法 = 方法_.获取方法(方法节点);
        const auto 读回失败 = 已发表配对 ? 原因::发布结果未知 : 原因::结构不一致;
        if (!最终方法) throw 新学习运行异常(方法节点,读回失败);
        const auto 匹配 = std::find_if(最终方法->条件结果组.begin(), 最终方法->条件结果组.end(),
            [&](const auto& 项) { return 项.配对节点 == 配对节点; });
        if (匹配 == 最终方法->条件结果组.end()
            || std::count_if(最终方法->条件结果组.begin(), 最终方法->条件结果组.end(),
                [&](const auto& 项) { return 项.配对节点 == 配对节点; }) != 1
            || !同一学习条件(匹配->条件, 新配对.条件) || 匹配->结果 != 新配对.结果
            || (!重复 && 匹配->命中次数 != 0)) {
            throw 新学习运行异常(方法节点,读回失败);
        }
        return {方法节点};
    } catch (const 新学习运行异常&) {
        throw;
    } catch (const std::bad_alloc&) {
        throw 新学习运行异常(方法节点,已发表配对 ? 原因::发布结果未知 : 原因::资源不足);
    } catch (const std::exception& e) {
        throw 新学习运行异常(方法节点,已发表配对 ? 原因::发布结果未知 : 原因::内部错误,e.what());
    } catch (...) {
        throw 新学习运行异常(方法节点,已发表配对 ? 原因::发布结果未知 : 原因::内部错误);
    }
}
} // namespace 海中鱼巣

namespace 海中鱼巣 {

void 模拟学习服务::学习有序本能(const 新方法信息& m,const std::function<bool()>& 停止检查) {
    const auto 停止=[&] { if(停止检查&&停止检查())throw 新学习运行异常(m.节点,新学习运行异常原因::停止取消); };
    const auto 不支持 = [&](std::uint32_t 位置, const char* 原因) -> void {
        throw 新学习运行异常(m.节点, 新学习运行异常原因::规格未支持,
            "simulation parameter " + std::to_string(位置) + ": " + 原因);
    };
    std::vector<新模拟有序参数输入> base;
    std::vector<std::optional<新特征匹配定义快照>> defs;
    for(const auto& spec:m.参数规格组) {
        if(!spec.有序规格) 不支持(0,"mixed ordered and role parameters");
        base.push_back({*spec.有序规格});
        defs.push_back(spec.有序规格->信息类型==新方法参数信息类型::特征
            ?方法_.读取模拟特征范围(spec.有序规格->范围概念节点):std::nullopt);
        if(spec.有序规格->信息类型==新方法参数信息类型::特征&&!defs.back())
            throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
    }
    const auto N=base.size();
    // 同一F的参数限定和存在原型均取正式值域交集，不丢弃任一限定。
    const auto 收窄特征范围 = [&](std::size_t k,const 新特征匹配定义快照& d) {
        if(defs[k]->特征类型根!=d.特征类型根||defs[k]->材料类型!=d.材料类型
            ||defs[k]->比较规则!=d.比较规则)
            throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,"aliased feature definitions disagree");
        if(std::holds_alternative<特征概念全材料值域>(defs[k]->值域)) {
                defs[k]->值域=d.值域;
            } else if(std::holds_alternative<特征概念全材料值域>(d.值域)) {
                // 另一侧为全集，保留本侧已有约束。
            } else if(const auto* a=std::get_if<std::vector<特征概念I64闭区间>>(&defs[k]->值域)) {
                const auto* b=std::get_if<std::vector<特征概念I64闭区间>>(&d.值域);
                if(!b) 不支持(static_cast<std::uint32_t>(k+1),"prototype range intersection shape");
                std::vector<特征概念I64闭区间> intersection;
                for(const auto& x:*a)for(const auto& y:*b) {
                    const auto l=(std::max)(x.下界,y.下界),h=(std::min)(x.上界,y.上界);
                    if(l<=h)intersection.push_back({l,h});
                }
                defs[k]->值域=std::move(intersection);
            } else if (auto* a = std::get_if<特征概念结构化I64值域>(&defs[k]->值域)) {
                const auto* b = std::get_if<特征概念结构化I64值域>(&d.值域);
                if (!b || a->分量.size() != b->分量.size())
                    不支持(static_cast<std::uint32_t>(k+1),"prototype structured range shape");
                for (std::size_t n=0;n<a->分量.size();++n) {
                    auto& 左=a->分量[n]; const auto& 右=b->分量[n];
                    if (左.角色!=右.角色 || 左.单位!=右.单位)
                        不支持(static_cast<std::uint32_t>(k+1),"prototype component roles or units differ");
                    std::vector<特征概念I64闭区间> 交集;
                    for (const auto& x:左.值域) for (const auto& y:右.值域) {
                        const auto l=(std::max)(x.下界,y.下界),h=(std::min)(x.上界,y.上界);
                        if(l<=h)交集.push_back({l,h});
                    }
                    左.值域=std::move(交集);
                }
            } else if (defs[k]->值域 != d.值域) {
                const auto 比较=概念_.比较值域(defs[k]->特征类型根,defs[k]->值域,d.值域);
                if(比较.状态==特征概念规则状态::资源失败)throw 新学习运行异常(m.节点,新学习运行异常原因::资源不足);
                if(比较.状态!=特征概念规则状态::已完成||!比较.关系)
                    不支持(static_cast<std::uint32_t>(k+1),"prototype domain comparison is unsupported");
                if(*比较.关系==特征概念值域关系::左包含右)defs[k]->值域=d.值域;
                else if(*比较.关系==特征概念值域关系::相等||*比较.关系==特征概念值域关系::右包含左){}
                else if(*比较.关系==特征概念值域关系::分离)defs[k]->值域=std::vector<稳定编码>{};
                else {
                    const auto* 有限=std::get_if<std::vector<稳定编码>>(&defs[k]->值域);
                    const auto* 另一有限=std::get_if<std::vector<稳定编码>>(&d.值域);
                    if(!有限&&!另一有限)不支持(static_cast<std::uint32_t>(k+1),"nonfinite intersecting prototype domains");
                    const auto& 取样=有限 ? *有限 : *另一有限;
                    const auto& 范围=有限 ? d.值域 : defs[k]->值域;
                    std::vector<稳定编码> 交集;
                    for(const auto v:取样) {
                        const auto r=概念_.比较值域(defs[k]->特征类型根,特征概念值域{std::vector<稳定编码>{v}},范围);
                        if(r.状态!=特征概念规则状态::已完成||!r.关系)
                            不支持(static_cast<std::uint32_t>(k+1),"finite prototype element comparison");
                        if(*r.关系==特征概念值域关系::相等||*r.关系==特征概念值域关系::右包含左)交集.push_back(v);
                    }
                    defs[k]->值域=std::move(交集);
                }
            }
    };
    std::vector<std::size_t> 代表(N);
    for(std::size_t i=0;i<N;++i)代表[i]=i;
    const auto 根=[&](std::size_t i){while(代表[i]!=i)i=代表[i];return i;};
    for(const auto& r:m.初始模拟关系组) {
        if(!r.左参数序号||r.左参数序号>N||!r.右参数序号||r.右参数序号>N)
            throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,"relation parameter out of range");
        if(r.种类==新方法参数关系种类::同一) {
            const auto a=根(r.左参数序号-1),b=根(r.右参数序号-1);
            if(base[a].规格.信息类型!=base[b].规格.信息类型)
                throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,"alias information type mismatch");
            代表[(std::max)(a,b)]=(std::min)(a,b);
        }
    }
    for(std::size_t i=0;i<N;++i){代表[i]=根(i);if(代表[i]!=i)base[i].同一参数=static_cast<std::uint32_t>(代表[i]+1);}
    for(std::size_t i=0;i<N;++i)if(代表[i]!=i&&defs[i])
        收窄特征范围(代表[i],*defs[i]);
    for(const auto& r:m.初始模拟关系组) {
        const auto a=代表[r.左参数序号-1],b=代表[r.右参数序号-1];
        if(r.种类==新方法参数关系种类::不同&&a==b)
            throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,"conflicting same/different relations");
        if(r.种类==新方法参数关系种类::特征所属存在) {
            auto& p=base[a]; const auto owner=static_cast<std::uint32_t>(b+1);
            if(p.所属存在参数&&p.所属存在参数!=owner)
                throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,"conflicting feature owners");
            p.所属存在参数=owner;
        }
        if(r.种类==新方法参数关系种类::集合包含存在)
            base[a].集合成员参数组.push_back(static_cast<std::uint32_t>(b+1));
    }
    for(std::size_t i=0;i<N;++i)if(代表[i]!=i) {
        base[i].所属存在参数=base[代表[i]].所属存在参数;
        base[i].集合成员参数组=base[代表[i]].集合成员参数组;
    }
    // 存在范围中的正式特征成为原型的一部分，不凭位置邻接给参数添关系。
    for(std::size_t i=0;i<base.size();++i) {
        if(defs[i])if(const auto* 引用=std::get_if<特征概念存在引用值域>(&defs[i]->值域)) {
            if(std::none_of(base.begin(),base.end(),[&](const auto& p) {
                return p.规格.信息类型==新方法参数信息类型::存在&&p.规格.范围概念节点==引用->存在概念节点;
            })) {
                // 类型范围缺少原型时追加私有存在；已有同范围原型复用，递归引用由此闭环。
                base.push_back({{0,新方法参数信息类型::存在,引用->存在概念节点}});
                defs.push_back(std::nullopt);
            }
        }
        if(base[i].规格.信息类型!=新方法参数信息类型::存在)continue;
        const auto owner=static_cast<std::uint32_t>((i<N ? 代表[i] : i)+1);
        const auto ec=方法_.读取模拟存在范围(base[i].规格.范围概念节点);
        if(!ec)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
        for(const auto& required:ec->特征组) {
            const auto d=方法_.读取模拟特征范围(required.具体特征概念);
            if(!d)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
            auto k=base.size();
            for(std::size_t j=0;j<base.size();++j)
                if(base[j].所属存在参数==owner&&defs[j]&&defs[j]->特征类型根==d->特征类型根) {k=j;break;}
            if(k==base.size()) {
                base.push_back({{0,新方法参数信息类型::特征,required.具体特征概念},owner});
                defs.push_back(d);
            } else { 收窄特征范围(k,*d); }
        }
    }
    for(std::size_t i=0;i<N;++i)if(代表[i]!=i&&defs[i])defs[i]=defs[代表[i]];
    std::vector<通用模拟候选轴> axes;
    std::vector<std::vector<std::size_t>> axisFor(base.size());
    std::vector<bool> 集合选择(base.size(),false),引用选择(base.size(),false);
    for(std::size_t i=0;i<base.size();++i) if(defs[i]) {
        if(i<N&&代表[i]!=i){axisFor[i]=axisFor[代表[i]];集合选择[i]=集合选择[代表[i]];引用选择[i]=引用选择[代表[i]];continue;}
        if (const auto* 结构 = std::get_if<特征概念结构化I64值域>(&defs[i]->值域)) {
            if (defs[i]->材料类型 != 特征概念材料物理类型::I64数组)
                不支持(static_cast<std::uint32_t>(i+1),"structured non-array material needs payload shape constraints");
            if (结构->分量.empty())
                throw 新学习运行异常(m.节点,新学习运行异常原因::输入不合法,"empty structured integer domain");
            for (const auto& 分量 : 结构->分量) {
                axisFor[i].push_back(axes.size());
                axes.push_back({static_cast<std::uint32_t>(axes.size()+1),生成通用模拟整数候选(分量.值域)});
            }
            continue;
        }
        std::vector<通用模拟候选值> values;
        if(const auto* 引用=std::get_if<特征概念存在引用值域>(&defs[i]->值域)) {
            if(i<N)不支持(static_cast<std::uint32_t>(i+1),"explicit reference C/R needs value-position representation");
            引用选择[i]=true;
            for(std::size_t j=0;j<base.size();++j)
                if(base[j].规格.信息类型==新方法参数信息类型::存在
                    &&base[j].规格.范围概念节点==引用->存在概念节点&&(j>=N||代表[j]==j))
                    values.emplace_back(static_cast<std::int64_t>(j+1));
            if(values.empty())不支持(static_cast<std::uint32_t>(i+1),"existence-reference prototype not found");
            values=生成通用模拟枚举候选(std::move(values),m.节点.值+i);
        }
        else if(const auto* r=std::get_if<std::vector<特征概念I64闭区间>>(&defs[i]->值域))
            values=生成通用模拟整数候选(*r);
        else if(const auto* r=std::get_if<std::vector<稳定编码>>(&defs[i]->值域)) {
            for(auto v:*r)values.emplace_back(v);
            values=生成通用模拟枚举候选(std::move(values),m.节点.值+i);
        } else if(const auto* r=std::get_if<特征概念存在集合值域>(&defs[i]->值域)) {
            if(r->至少一个存在||!r->必含存在节点组.empty()) 不支持(static_cast<std::uint32_t>(i+1),"referenced existence collection needs isolated binding");
            values.emplace_back(std::int64_t{0}); // 空集合原型的本地选择号，不是特征的数值。
        } else { 不支持(static_cast<std::uint32_t>(i+1),"range has no finite candidate declaration"); }
        axisFor[i].push_back(axes.size());
        axes.push_back({static_cast<std::uint32_t>(axes.size()+1),std::move(values)});
    }
    std::vector<std::size_t> eventAxes;
    for(const auto pos:m.初始模拟事件参数组) {
        if(!pos||pos>N||!defs[pos-1])throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
        const auto* domain=std::get_if<std::vector<特征概念I64闭区间>>(&defs[pos-1]->值域);
        if(!domain) 不支持(pos,"event parameter is not an integer domain");
        eventAxes.push_back(axes.size());
        axes.push_back({static_cast<std::uint32_t>(axes.size()+1),生成通用模拟整数候选(*domain)});
    }
    通用模拟组合游标 cursor(std::move(axes));
    while(!cursor.已结束()) {
        停止();
        const auto values=cursor.读取当前组合();
        auto input=base;
        for(std::size_t i=0;i<input.size();++i) if(defs[i]) {
            if (std::holds_alternative<特征概念结构化I64值域>(defs[i]->值域)) {
                std::vector<std::int64_t> 分量;
                分量.reserve(axisFor[i].size());
                for (const auto 轴 : axisFor[i]) 分量.push_back(std::get<std::int64_t>(values[轴]));
                input[i].材料 = 新特征值材料{std::move(分量)};
                continue;
            }
            const auto& x=values[axisFor[i].front()];
            if(引用选择[i])input[i].引用存在参数=static_cast<std::uint32_t>(std::get<std::int64_t>(x));
            else if(defs[i]->材料类型==特征概念材料物理类型::I64标量)input[i].值=std::get<std::int64_t>(x);
            else if(集合选择[i]) {
                input[i].材料=新特征值材料{std::vector<std::uint64_t>{}};
                input[i].集合成员参数组.push_back(static_cast<std::uint32_t>(std::get<std::int64_t>(x)));
            }
            else if(const auto* v=std::get_if<稳定编码>(&x)) {
                const auto material=方法_.读取模拟候选材料(*v);
                if(!material)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
                // 引用集合的现实成员不能未经映射带入隔离样例。
                if(defs[i]->比较规则==特征概念比较规则::存在集合身份判等) {
                    const auto* a=std::get_if<std::vector<std::uint64_t>>(&material->材料);
                    if(!a||!a->empty()) 不支持(static_cast<std::uint32_t>(i+1),"enumerated existence references need isolated binding");
                }
                input[i].材料=material->材料;
            } else input[i].材料=新特征值材料{std::vector<std::uint64_t>{}};
        }
        std::vector<新模拟输入事件> events;
        for(std::size_t i=0;i<eventAxes.size();++i)
            events.push_back({m.初始模拟事件参数组[i],std::get<std::int64_t>(values[eventAxes[i]])});
        auto world=std::make_shared<新_模拟世界类>(方法_);
        const auto created=world->建立有序样例(m.节点,input,events);
        if(created.状态!=新模拟世界状态::已完成)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
        const auto bindings=world->读取参数绑定();
        if(!bindings)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
        auto context=方法_.建立模拟参数场景(m.节点,*bindings,world);
        if(!context.参数场景)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
        std::vector<std::optional<新特征信息>> before(N);
        std::vector<std::optional<新特征值信息>> beforeMaterial(N);
        for(std::size_t i=0;i<N;++i) if(defs[i]) {
            before[i]=context.参数场景->读取绑定特征((*bindings)[i].有序引用->信息节点);
            if(!before[i]||!before[i]->当前值)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
            if(const auto* v=std::get_if<稳定编码>(&*before[i]->当前值))
                beforeMaterial[i]=world->读取模拟特征值材料(*v);
        }
        停止();
        const auto call=方法_.调用本能函数(m.节点,*context.参数场景);
        if(call.状态!=新方法本能调用状态::已调用||!call.本次条件结果对)
            throw 新学习运行异常(m.节点,新学习运行异常原因::本能运行异常);
        const auto& run=*call.本次条件结果对;
        std::optional<新方法条件结果定义> 本组已发表;
        if(run.结果.状态==新本能函数执行状态::已按合同返回
            &&run.结果.结果取得状态==新本能函数结果取得状态::已取得) {
            新方法条件结果定义 pair;
            pair.条件.关系规则=新方法项关系规则::全部成立;
            pair.条件.参数关系组=m.初始模拟关系组;
            pair.结果.关系规则=新方法项关系规则::全部成立;
            for(std::size_t i=0;i<N;++i) {
                新方法条件项定义 c;c.参数序号=static_cast<std::uint32_t>(i+1);
                if(!defs[i])c.存在概念节点=base[i].规格.范围概念节点;
                else {
                    c.特征类型概念节点=defs[i]->特征类型根;
                    if(const auto* v=std::get_if<std::int64_t>(&*before[i]->当前值))c.精确数值域=特征概念I64闭区间{*v,*v};
                    else {
                        if(!beforeMaterial[i])throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
                        if(defs[i]->比较规则==特征概念比较规则::存在集合身份判等) {
                            const auto* members=std::get_if<std::vector<std::uint64_t>>(&beforeMaterial[i]->材料);
                            if(!members)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
                            if(members->empty())c.特征值域概念节点=方法_.保存模拟材料值域(base[i].规格.范围概念节点,*beforeMaterial[i]);
                            else {
                                c.特征值域概念节点=base[i].规格.范围概念节点;
                                for(std::size_t j=0;j<N;++j)if(!defs[j]
                                    &&std::find(members->begin(),members->end(),(*bindings)[j].有序引用->信息节点.值)!=members->end()) {
                                    const 新方法参数关系 relation{新方法参数关系种类::集合包含存在,
                                        static_cast<std::uint32_t>(i+1),static_cast<std::uint32_t>(j+1)};
                                    if(std::find(pair.条件.参数关系组.begin(),pair.条件.参数关系组.end(),relation)==pair.条件.参数关系组.end())
                                        pair.条件.参数关系组.push_back(relation);
                                }
                            }
                        } else c.特征值域概念节点=方法_.保存模拟材料值域(base[i].规格.范围概念节点,*beforeMaterial[i]);
                    }
                }
                pair.条件.条件项组.push_back(std::move(c));
                if(!defs[i])continue;
                const auto id=(*bindings)[i].有序引用->信息节点;
                if(!run.结果.建议观察特征节点组.empty()
                    &&std::find(run.结果.建议观察特征节点组.begin(),run.结果.建议观察特征节点组.end(),id)==run.结果.建议观察特征节点组.end())continue;
                const auto f=context.参数场景->读取绑定特征(id);
                if(!f||!f->当前值)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
                新方法结果项定义 r;r.参数序号=static_cast<std::uint32_t>(i+1);r.特征类型概念节点=defs[i]->特征类型根;
                if(const auto* v=std::get_if<std::int64_t>(&*f->当前值))r.精确数值域=特征概念I64闭区间{*v,*v};
                else {
                    const auto material=world->读取模拟特征值材料(std::get<稳定编码>(*f->当前值));
                    if(!material)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
                    if(defs[i]->比较规则==特征概念比较规则::存在集合身份判等) {
                        const auto* members=std::get_if<std::vector<std::uint64_t>>(&material->材料);
                        if(!members)throw 新学习运行异常(m.节点,新学习运行异常原因::结构不一致);
                        for(std::size_t j=0;j<N;++j) if(!defs[j]
                            &&std::find(members->begin(),members->end(),(*bindings)[j].有序引用->信息节点.值)!=members->end())
                            pair.结果.参数关系组.push_back({新方法参数关系种类::集合包含存在,static_cast<std::uint32_t>(i+1),static_cast<std::uint32_t>(j+1)});
                    } else r.特征值域概念节点=方法_.保存模拟材料值域(base[i].规格.范围概念节点,*material);
                }
                pair.结果.结果项组.push_back(std::move(r));
            }
            if(!pair.结果.结果项组.empty()||!pair.结果.参数关系组.empty()) {
                const auto published=方法_.追加模拟条件结果(m.节点,{pair});
                if(published.状态!=新方法学习发布状态::已发布&&published.状态!=新方法学习发布状态::精确重复)
                    throw 新学习运行异常(m.节点,published.状态==新方法学习发布状态::发布结果未知
                        ?新学习运行异常原因::发布结果未知:新学习运行异常原因::结构不一致);
                const auto fresh=方法_.获取方法(m.节点);
                if(!fresh||!std::any_of(fresh->条件结果组.begin(),fresh->条件结果组.end(),[&](const auto& p) {
                    return p.条件==pair.条件&&p.结果==pair.结果&&p.命中次数==0;
                }))throw 新学习运行异常(m.节点,新学习运行异常原因::发布结果未知);
                本组已发表=pair;
            }
        }
        context.参数场景.reset();
        world->释放有序样例();
        if (本组已发表) {
            const auto 释放后=方法_.获取方法(m.节点);
            if(!释放后||!std::any_of(释放后->条件结果组.begin(),释放后->条件结果组.end(),[&](const auto& p) {
                return p.条件==本组已发表->条件&&p.结果==本组已发表->结果&&p.命中次数==0
                    &&p.知识来源==新方法知识来源种类::模拟;
            }))throw 新学习运行异常(m.节点,新学习运行异常原因::发布结果未知,"knowledge invalid after sample release");
        }
        cursor.推进();
    }
}

}
