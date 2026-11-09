#include "服务值结算.本能运行服务.h"
#include "新_世界树类.h"
#include "../领域/新_方法类.h"
#include "../领域/新_任务类.h"
#include "../领域/新_因果类.h"
#include "../核心/容错检查.h"
#include <algorithm>
#include <array>
#include <limits>
#include <mutex>
#include <new>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>

namespace 海中鱼巣 {
namespace {
std::optional<std::int64_t> 单值(const 概念_特征类& 服务, 稳定编码 节点) {
    const auto 概念 = 服务.获取特征概念(节点);
    if (!概念 || 概念->材料类型 != 特征概念材料物理类型::I64标量) return std::nullopt;
    const auto* 域 = std::get_if<std::vector<特征概念I64闭区间>>(&概念->值域);
    if (!域 || 域->size() != 1 || 域->front().下界 != 域->front().上界) return std::nullopt;
    return 域->front().下界;
}
bool 含有(const std::vector<稳定编码>& 集合, 稳定编码 节点) {
    return std::find(集合.begin(), 集合.end(), 节点) != 集合.end();
}
std::optional<稳定编码> 取得特征类型根(const 概念_特征类& 服务, 稳定编码 概念节点) noexcept {
    const auto 关系 = 服务.比较单特征概念(概念节点, 概念节点);
    return 关系.状态 == 特征概念规则状态::已完成
            && 关系.关系 == 单特征概念关系::同一
        ? 关系.特征类型根 : std::nullopt;
}
新服务值最终消费结果 投影最终来源结果(
    const 新服务值最终来源定位& 定位, const 新任务最终来源读取结果& 读取) {
    const bool 有历史 = 读取.状态 == 新任务最终来源读取状态::已完成
        || 读取.状态 == 新任务最终来源读取状态::已入账待收尾;
    const bool 有来源 = 有历史 || 读取.状态 == 新任务最终来源读取状态::可新消费
        ||读取.状态==新任务最终来源读取状态::已准备待续;
    if (读取.来源.has_value() != 有来源 || 读取.既有消费.has_value() != 有历史
        || (读取.来源 && 读取.来源->来源 != 定位))
        throw std::logic_error("final source result shape mismatch");
    if (读取.既有消费 && (!有效(读取.既有消费->消费节点)
        || 读取.既有消费->来源.来源 != 定位
        || 读取.既有消费->收尾阶段节点.has_value()
            != (读取.状态 == 新任务最终来源读取状态::已完成)))
        throw std::logic_error("final U or J5 result shape mismatch");
    新服务值最终消费结果 结果;
    结果.来源 = 定位;
    switch (读取.状态) {
    case 新任务最终来源读取状态::可新消费:
        结果.状态 = 新服务值最终消费状态::尚未消费; break;
    case 新任务最终来源读取状态::已完成:
        结果.状态 = 新服务值最终消费状态::已完成; break;
    case 新任务最终来源读取状态::已入账待收尾:
        结果.状态 = 新服务值最终消费状态::已入账待收尾; break;
    case 新任务最终来源读取状态::来源条件不具备:
        结果.状态 = 新服务值最终消费状态::来源条件不具备; break;
    case 新任务最终来源读取状态::来源已变化:
        结果.状态 = 新服务值最终消费状态::来源已变化; break;
    case 新任务最终来源读取状态::等待配置:
        结果.状态 = 新服务值最终消费状态::等待配置; break;
    case 新任务最终来源读取状态::等待同源工作:
        结果.状态 = 新服务值最终消费状态::等待同源工作; break;
    case 新任务最终来源读取状态::资源延后:
        结果.状态 = 新服务值最终消费状态::资源延后; break;
    case 新任务最终来源读取状态::已准备待续:
        结果.状态 = 新服务值最终消费状态::已准备待续; break;
    default: throw std::logic_error("unknown final source status");
    }
    if (读取.既有消费) {
        结果.消费节点 = 读取.既有消费->消费节点;
        结果.普通动作节点 = 读取.既有消费->普通动作节点;
        结果.收尾阶段节点 = 读取.既有消费->收尾阶段节点;
    }
    return 结果;
}
}

struct 服务值结算本能运行服务::实现 final {
    新_世界树类& 世界;
    新_方法类& 方法;
    新_任务类& 任务;
    新_需求类& 需求;
    新_场景类& 场景;
    新_动态类& 动态;
    新_特征类& 特征;
    概念_特征类& 概念;
    概念_二次特征类& 二次;
    新任务治理世界协作接口& 治理世界;
    新服务值结算工程报告函数 报告;
    mutable std::mutex 互斥;
    std::mutex 初始化互斥;
    std::mutex 最终消费串行互斥;
    std::optional<新服务值结算登记材料> 材料;
    std::optional<稳定编码> 需求节点;
    std::optional<新服务值治理定义> 治理定义;
    std::optional<新普通需求建立请求> 待发布首子请求;
    bool 停止 = false, 已报告 = false;
    std::string 首条诊断;
};

服务值结算本能运行服务::服务值结算本能运行服务(新_世界树类& 世界, 新_方法类& 方法,
    新_任务类& 任务, 新_需求类& 需求, 新_场景类& 场景, 新_动态类& 动态,
    新_特征类& 特征, 概念_特征类& 概念, 概念_二次特征类& 二次,
    新任务治理世界协作接口& 治理世界, 新服务值结算工程报告函数 报告) noexcept {
    try { 实现_ = std::make_unique<实现>(世界, 方法, 任务, 需求, 场景, 动态, 特征, 概念, 二次, 治理世界, 报告); }
    catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"构造服务值结算运行服务", L"结算运行服务分配失败。");
    }
}
服务值结算本能运行服务::~服务值结算本能运行服务() = default;

void 服务值结算本能运行服务::报告并停止(std::string_view 错误) noexcept {
    if (!实现_) return;
    try { std::lock_guard 锁(实现_->互斥);
        实现_->停止 = true;
        if (实现_->首条诊断.empty()) 实现_->首条诊断.assign(错误);
    } catch (...) {}
    // 适配合同仅复制诊断并启动独立工程报告线程；不通知自我或同步弹窗。
    报告待处理工程异常();
}
void 服务值结算本能运行服务::报告待处理工程异常() noexcept {
    if (!实现_) return;
    try {
        std::string 诊断; 新服务值结算工程报告函数 报告 = nullptr;
        { std::lock_guard 锁(实现_->互斥);
          if (!实现_->停止 || 实现_->已报告) return;
          诊断 = 实现_->首条诊断.empty() ? "服务值结算：工程停止" : 实现_->首条诊断;
          报告 = 实现_->报告; 实现_->已报告 = true; }
        if (报告) 报告(诊断);
    } catch (...) {}
}
std::optional<新服务值结算登记材料> 服务值结算本能运行服务::读取登记材料() const noexcept {
    if (!实现_) return std::nullopt;
    try { std::lock_guard 锁(实现_->互斥); return 实现_->停止 ? std::nullopt : 实现_->材料; }
    catch (...) { return std::nullopt; }
}

新服务值治理初始化结果 服务值结算本能运行服务::初始化治理(
    稳定编码 场景, 稳定编码 自我, 稳定编码 服务特征, 稳定编码 服务根) noexcept {
    新服务值治理初始化结果 结果;
    try {
        std::lock_guard 初始化锁(实现_->初始化互斥);
        auto& s = *实现_;
        { std::lock_guard 锁(s.互斥);
          if (s.停止) { 结果.状态 = 新服务值结算协作状态::工程停止; return 结果; }
          // 自我所在场景随迁移变化（6220 §2），不作为登记身份比较。
          if (s.材料 && (s.材料->自我存在 != 自我
              || s.材料->服务值特征 != 服务特征 || s.材料->服务根需求 != 服务根))
              throw std::logic_error("governance initialization identity changed"); }
        const auto 根 = s.需求.获取需求(服务根);
        const auto E = s.世界.查询存在(自我);
        const auto F = s.世界.查询特征(服务特征);
        const auto S = s.世界.查询场景(场景);
        if (!根 || !E || !F || !S || 根->根角色 != 新需求根角色::服务
            || 根->目标存在节点 != 自我 || 根->目标.目标形式 != 新需求目标形式::值
            || 根->目标.目标变化方向 || !根->目标.目标值域概念节点 || !根->目标.目标比较关系概念节点
            || !含有(S->直接成员存在组, 自我) || !含有(E->特征节点组, 服务特征)
            || 取得特征类型根(s.概念, F->特征概念节点) != 根->目标.目标特征类型概念节点)
            throw std::logic_error("governance initialization real identity mismatch");
        const auto 根当前结果=s.需求.复核当前满足(服务根);
        if((根当前结果.状态!=新需求当前满足状态::已满足&&根当前结果.状态!=新需求当前满足状态::需增加)
            ||根当前结果.实际特征节点!=std::optional{服务特征}||!有效(服务特征)||!根当前结果.准确差异)
            throw std::logic_error("governance initialization service root M owner readback incomplete");
        新服务值结算登记材料 m;
        m.自我所在场景 = 场景; m.自我存在 = 自我; m.服务值特征 = 服务特征;
        m.服务根需求 = 服务根; m.自我概念 = E->专属存在概念节点;
        m.服务值类型根 = 根->目标.目标特征类型概念节点;
        auto 类型 = s.概念.获取特征概念(m.服务值类型根);
        if (!类型 || 类型->材料类型 != 特征概念材料物理类型::I64标量)
            throw std::logic_error("governance service feature type is not I64");
        const 特征概念值域 有符号域 = std::vector<特征概念I64闭区间>{{
            (std::numeric_limits<std::int64_t>::min)(), (std::numeric_limits<std::int64_t>::max)()}};
        if (类型->值域 != 有符号域) {
            const 特征概念值域 历史非负域 = std::vector<特征概念I64闭区间>{{0, (std::numeric_limits<std::int64_t>::max)()}};
            if (类型->值域 != 历史非负域 || !s.概念.更新特征概念值域(m.服务值类型根,
                    有符号域, s.世界.获取事实服务引用().特征值服务))
                throw std::logic_error("governance signed service domain unavailable");
            类型 = s.概念.获取特征概念(m.服务值类型根);
            if (!类型 || 类型->值域 != 有符号域) throw std::logic_error("governance signed domain readback failed");
        }
        const auto 全集 = s.需求.查询全部需求();
        if (全集.状态 != 新需求操作状态::已找到)
            throw std::logic_error("governance demand list unavailable");
        std::optional<新需求信息> 已有;
        for (auto D : 全集.需求节点组) {
            const auto d = s.需求.获取需求(D);
            const auto g = s.需求.读取需求治理信息(D);
            if (!g) throw std::logic_error("governance demand record corrupt");
            if (g->父需求节点 != 服务根) continue;
            if (!d || 已有) throw std::logic_error("governance first child absent target or ambiguous");
            已有 = d;
        }
        if (已有) {
            if (已有->目标存在节点 != 自我 || 已有->来源类型 != 新需求来源类型::本能定义
                || 已有->来源承接节点 || 已有->目标.目标特征类型概念节点 != m.服务值类型根
                || 已有->目标.目标形式 != 新需求目标形式::值 || !已有->目标.目标值域概念节点
                || !已有->目标.目标比较关系概念节点 || !单值(s.概念, *已有->目标.目标值域概念节点)
                || 取得特征类型根(s.概念, *已有->目标.目标值域概念节点) != m.服务值类型根)
                throw std::logic_error("governance existing D target mismatch");
            const std::vector<二次特征概念输入定义> 输入{
                {0, 二次特征输入角色::左输入, 二次特征输入来源种类::特征准确值, m.服务值类型根},
                {1, 二次特征输入角色::右输入, 二次特征输入来源种类::特征准确值, m.服务值类型根}};
            const auto 关系 = s.二次.获取二次特征概念(*已有->目标.目标比较关系概念节点);
            const auto 目标单值 = *单值(s.概念, *已有->目标.目标值域概念节点);
            const 特征概念值域 精确相等域 = std::vector<特征概念I64闭区间>{{目标单值, 目标单值}};
            if (!关系 || 关系->算法 != 二次特征算法::有向特征差值 || 关系->输入定义组 != 输入
                || 关系->参照特征概念节点 != 已有->目标.目标值域概念节点
                || 关系->输出特征概念.材料类型 != 特征概念材料物理类型::I64标量
                || 关系->输出特征概念.比较规则 != 特征概念比较规则::I64数值
                || 关系->输出特征概念.单位 || 关系->输出特征概念.值域 != 精确相等域
                || std::find(关系->结果域种类组.begin(), 关系->结果域种类组.end(), 二次特征结果域种类::等于) == 关系->结果域种类组.end())
                throw std::logic_error("governance existing D is not formal equality");
            m.目标值域 = *已有->目标.目标值域概念节点;
            m.准确相等概念 = *已有->目标.目标比较关系概念节点;
        } else {
            if (!s.待发布首子请求) {
                if (!F->当前值 || !std::holds_alternative<std::int64_t>(*F->当前值))
                    throw std::logic_error("governance initial A0 is not actual I64");
                const auto A0 = std::get<std::int64_t>(*F->当前值);
                if (A0 == (std::numeric_limits<std::int64_t>::max)())
                    throw std::overflow_error("governance initial target A0+1 overflow");
                const 特征概念值域 目标域 = std::vector<特征概念I64闭区间>{{A0 + 1, A0 + 1}};
                const auto 比较 = s.概念.比较值域(类型->材料类型, 类型->值域, 目标域);
                if (比较.状态 != 特征概念规则状态::已完成 || !比较.关系)
                    throw std::logic_error("governance target domain comparison failed");
                if (*比较.关系 == 特征概念值域关系::相等) m.目标值域 = m.服务值类型根;
                else {
                    特征概念定义 定义;
                    定义.材料类型 = 类型->材料类型; 定义.值域 = 目标域; 定义.单位 = 类型->单位;
                    定义.比较规则 = 类型->比较规则; 定义.聚合规则 = 类型->聚合规则;
                    m.目标值域 = s.世界.建立特征概念(定义, m.服务值类型根);
                }
                if (!有效(m.目标值域)) throw std::logic_error("governance target singleton not published");
                const std::vector<二次特征概念输入定义> 输入{
                    {0, 二次特征输入角色::左输入, 二次特征输入来源种类::特征准确值, m.服务值类型根},
                    {1, 二次特征输入角色::右输入, 二次特征输入来源种类::特征准确值, m.服务值类型根}};
                const auto 比较概念 = s.二次.建立或取得默认二次特征概念(二次特征算法::有向特征差值, 输入, m.目标值域);
                if (!比较概念.概念节点) throw std::logic_error("governance target comparison unavailable");
                const auto 相等 = s.二次.建立或取得参照结果域概念(*比较概念.概念节点, 二次特征结果域种类::等于);
                if (!相等.概念节点) throw std::logic_error("governance target equality unavailable");
                m.准确相等概念 = *相等.概念节点;
                新普通需求建立请求 请求;
                请求.父需求节点 = 服务根; 请求.目标存在节点 = 自我; 请求.来源类型 = 新需求来源类型::本能定义;
                请求.目标.目标特征类型概念节点 = m.服务值类型根;
                请求.目标.目标值域概念节点 = m.目标值域; 请求.目标.目标比较关系概念节点 = m.准确相等概念;
                s.待发布首子请求 = 请求;
            }
            const auto 建立 = s.需求.建立或取得普通需求(*s.待发布首子请求);
            if (建立.需求节点) 已有 = s.需求.获取需求(*建立.需求节点);
            else {
                const auto 复查 = s.需求.查询全部需求();
                if (复查.状态 != 新需求操作状态::已找到) throw std::logic_error("governance first D publication unresolved");
                for (auto D : 复查.需求节点组) {
                    const auto d = s.需求.获取需求(D);
                    if (!d || d->父需求节点 != 服务根) continue;
                    if (已有 || d->目标存在节点 != 自我 || d->目标 != s.待发布首子请求->目标
                        || d->来源类型 != 新需求来源类型::本能定义 || d->来源承接节点)
                        throw std::logic_error("governance first D publication ambiguous");
                    已有 = d;
                }
            }
            if (!已有 || 已有->父需求节点 != 服务根 || 已有->目标 != s.待发布首子请求->目标)
                throw std::logic_error("governance first D readback mismatch");
            m.目标值域 = *已有->目标.目标值域概念节点;
            m.准确相等概念 = *已有->目标.目标比较关系概念节点;
        }
        const auto 定义 = s.任务.确保服务值治理定义(m.自我概念, m.服务值类型根);
        const auto 读回 = s.任务.读取服务值治理定义(定义.节点);
        if (!读回 || *读回 != 定义) throw std::logic_error("governance definition readback mismatch");
        const auto 先天 = s.概念.获取先天特征概念();
        if (!先天) throw std::logic_error("governance task collection type missing");
        m.服务任务集合类型根 = 先天->服务任务集合;
        // 定义级初始化，不是执行一次结算。状态、动作实例和经验发生仍由真实运行产生。
        const auto 状态类型 = s.任务.获取任务状态特征类型();
        const auto 任务服务值类型 = s.任务.获取任务服务值特征类型();
        if (!状态类型 || !任务服务值类型)
            throw std::logic_error("innate settlement task feature types unavailable");
        const auto 状态根 = s.概念.获取特征概念(*状态类型);
        if (!状态根) throw std::logic_error("innate settlement state type unreadable");
        特征概念定义 完成定义;
        完成定义.材料类型 = 状态根->材料类型;
        const auto 完成值 = static_cast<std::int64_t>(新任务状态::已完成);
        完成定义.值域 = std::vector<特征概念I64闭区间>{{完成值, 完成值}};
        完成定义.单位 = 状态根->单位;
        完成定义.比较规则 = 状态根->比较规则;
        完成定义.聚合规则 = 状态根->聚合规则;
        const auto 完成域 = s.世界.建立特征概念(完成定义, *状态类型);
        if (!有效(完成域)) throw std::logic_error("innate completed state domain unavailable");
        const auto 任务范围 = s.任务.获取任务信息范围概念();
        auto 事实 = s.世界.获取事实服务引用();
        const auto 关系概念 = 事实.因果服务.确保先天因果概念(定义.节点);
        动作方法投影请求 动作请求;
        动作请求.动作定义节点 = 定义.节点;
        动作请求.执行主体概念节点 = m.自我概念;
        动作请求.普通目标概念节点 = m.自我概念;
        动作请求.目标特征类型概念节点 = m.服务值类型根;
        const auto 动作概念 = s.动态.确保动作定义概念(动作请求);
        if (!动作概念.概念节点) throw std::logic_error("innate settlement action concept unavailable");
        新先天因果定义 因果;
        因果.治理定义 = 定义.节点;
        因果.合同版本概念 = 关系概念.合同版本概念;
        因果.作用场景范围概念 = S->专属存在概念节点;
        因果.动作概念 = *动作概念.概念节点;
        因果.触发方式 = 新因果动作触发方式::被动触发;
        因果.角色组 = {{0, 新先天因果角色种类::普通存在, m.自我概念},
            {1, 新先天因果角色种类::任务信息, 任务范围}};
        因果.唯一结果 = {0, 1, m.服务值类型根, m.服务值类型根,
            *任务服务值类型, *任务服务值类型};
        using 条件种类 = 新先天因果条件种类;
        const auto 添加条件 = [&](条件种类 种类, std::vector<std::int64_t> 角色,
            std::vector<稳定编码> 参数) {
            const auto 位置 = std::find_if(关系概念.关系概念组.begin(), 关系概念.关系概念组.end(),
                [&](const auto& 项) { return 项.first == 种类; });
            if (位置 == 关系概念.关系概念组.end())
                throw std::logic_error("innate settlement relation concept missing");
            因果.全部条件.push_back({static_cast<std::int64_t>(因果.全部条件.size()),
                种类, 位置->second, std::move(角色), std::move(参数)});
        };
        添加条件(条件种类::存在有效, {0}, {m.自我概念});
        添加条件(条件种类::集合包含角色, {0, 1}, {m.服务任务集合类型根, 任务范围});
        添加条件(条件种类::任务虚拟存在归属, {1}, {任务范围});
        添加条件(条件种类::特征值域, {0}, {m.服务值类型根, m.服务值类型根});
        添加条件(条件种类::特征值域, {1}, {*状态类型, 完成域});
        添加条件(条件种类::特征值域, {1}, {*任务服务值类型, *任务服务值类型});
        for (const auto 种类 : {条件种类::需求任务关联, 条件种类::直接父子关联,
            条件种类::完成执行评估一致, 条件种类::完成来源一致, 条件种类::直接子接收完整,
            条件种类::人类确认锚点, 条件种类::结果观察及归因,
            条件种类::本来源未消费, 条件种类::下层贡献未消费})
            添加条件(种类, {1}, {});
        添加条件(条件种类::当前治理边界一致, {0, 1}, {定义.节点});
        const auto 因果节点 = 事实.因果服务.确保先天因果(因果);
        const auto 因果读回 = 事实.因果服务.读取先天因果(因果节点);
        if (!因果读回 || 因果读回->定义 != 因果)
            throw std::logic_error("innate settlement definition readback mismatch");
        { std::lock_guard 锁(s.互斥);
          结果.状态 = s.治理定义 ? 新服务值结算协作状态::精确重复 : 新服务值结算协作状态::已完成;
          if (s.治理定义 && *s.治理定义 != 定义) throw std::logic_error("governance definition changed");
          s.材料 = m; s.治理定义 = 定义; s.需求节点 = 已有->节点; }
        结果.结算需求节点 = 已有->节点; 结果.治理定义 = 定义;
        return 结果;
    } catch (const std::exception& e) {
        报告并停止(e.what()); 结果.状态 = 新服务值结算协作状态::工程停止; return 结果;
    } catch (...) {
        报告并停止("governance initialization unknown failure");
        结果.状态 = 新服务值结算协作状态::工程停止; return 结果;
    }
}

新本能函数本次条件结果对 服务值结算本能运行服务::执行贡献治理工作(
    const 新任务贡献反馈定位& 来源) noexcept {
    try {
        if (!实现_ || !有效(来源.任务节点) || !有效(来源.执行实例节点)
            || !有效(来源.影响评估节点)) throw std::logic_error("贡献治理工作来源或运行服务缺失");
        const auto 来源变化 = [&]() {
            新本能函数本次条件结果对 返回;
            返回.结果.状态 = 新本能函数执行状态::已按合同返回;
            返回.结果.结果取得状态 = 新本能函数结果取得状态::已取得;
            新任务贡献治理返回材料 治理;
            治理.来源 = 来源;
            治理.推进.状态 = 新任务贡献推进状态::预期已变化;
            返回.结果.任务贡献治理 = std::move(治理);
            return 返回;
        };
        const auto 参数 = 实现_->任务.读取任务贡献治理参数(来源);
        if (参数.状态 == 新任务贡献治理参数读取状态::来源已变化) {
            if (参数.参数) throw std::logic_error("变化来源携带治理参数");
            return 来源变化();
        }
        if (参数.状态 != 新任务贡献治理参数读取状态::已读取 || !参数.参数
            || 参数.参数->完成来源 != 来源) throw std::logic_error("治理参数来源不一致");
        // 治理只依赖具名T/X/A及task owner的正式复核，不借用方法身份或参数场景。
        // 不持运行服务最终消费锁；task在其原锁区间再次核验输入并发布事实。
        auto 材料 = 实现_->任务.执行任务贡献治理(*参数.参数);
        if (材料.返回.来源 != 来源
            || 材料.返回.推进.状态 == 新任务贡献推进状态::工程故障)
            throw std::logic_error("贡献治理owner返回不一致");
        新本能函数本次条件结果对 返回;
        返回.条件.任务贡献治理参数 = std::move(材料.实际参数);
        返回.条件.治理动作前特征组 = std::move(材料.动作前特征组);
        返回.结果.状态 = 新本能函数执行状态::已按合同返回;
        返回.结果.动作阶段 = 材料.返回.本次动作组.empty()
            ? 新本能函数动作阶段::未进入动作 : 新本能函数动作阶段::已按函数合同返回;
        返回.结果.结果取得状态 = 新本能函数结果取得状态::已取得;
        返回.结果.任务贡献治理 = std::move(材料.返回);
        const auto& r = 返回.结果;
        实现_->任务.核验任务贡献治理返回材料(*r.任务贡献治理);
        return 返回;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"执行贡献治理工作",
            L"贡献治理来源、task执行或正式读回不一致。");
    }
}

新服务值最终消费结果 服务值结算本能运行服务::读取最终来源结果(
    const 新服务值最终来源定位& 来源) const noexcept {
    try {
        if (!实现_) throw std::logic_error("final runtime unavailable");
        // 历史U先于今天配置/确认读取；task reader负责完整冻结证据核验。
        return 投影最终来源结果(来源, 实现_->任务.读取任务最终消费来源(来源));
    } catch(const std::bad_alloc&){return {新服务值最终消费状态::资源延后,来源};}
    catch(const std::length_error&){return {新服务值最终消费状态::资源延后,来源};}
    catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取最终来源结果", L"正式最终消费来源或U/J5读回失败");
    }
}

std::optional<新服务值最终消费结果> 服务值结算本能运行服务::读取或收尾既有最终消费(
    const 新服务值最终来源定位& 来源) noexcept {
    try {
        if (!实现_) throw std::logic_error("final runtime unavailable");
        std::lock_guard 串行锁(实现_->最终消费串行互斥);
        const auto 读取 = 实现_->任务.读取任务最终消费来源(来源);
        if (读取.状态 == 新任务最终来源读取状态::可新消费||读取.状态==新任务最终来源读取状态::已准备待续) return std::nullopt;
        auto 结果 = 投影最终来源结果(来源, 读取);
        if (读取.状态 == 新任务最终来源读取状态::已完成) {
            结果.状态 = 新服务值最终消费状态::精确重复;
            return 结果;
        }
        if (读取.状态 == 新任务最终来源读取状态::已入账待收尾) {
            if (!读取.既有消费) throw std::logic_error("final U resume evidence missing");
            auto 准备 = 实现_->任务.准备任务最终消费(来源, 读取.既有消费->来源.边界.能力);
            if(准备.状态==新任务最终消费准备状态::资源延后)return 新服务值最终消费结果{新服务值最终消费状态::资源延后,来源};
            if (准备.状态 != 新任务最终消费准备状态::已入账待收尾 || !准备.准备)
                throw std::logic_error("final U resume token missing");
            return 实现_->任务.完成已入账任务最终收尾(*准备.准备);
        }
        return 结果;
    } catch(const std::bad_alloc&){return 新服务值最终消费结果{新服务值最终消费状态::资源延后,来源};}
    catch(const std::length_error&){return 新服务值最终消费结果{新服务值最终消费状态::资源延后,来源};}
    catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取或收尾既有最终消费", L"既有最终消费不完整或收尾失败");
    }
}

新服务值最终消费结果 服务值结算本能运行服务::执行最终来源工作(
    const 新服务值最终来源定位& 来源) noexcept {
    return 消费正式来源(来源);
}
新服务值最终消费结果 服务值结算本能运行服务::消费正式来源(
    const 新服务值最终来源定位& 定位) noexcept {
    try {
        if (const auto 旧 = 读取或收尾既有最终消费(定位)) return *旧;
        std::lock_guard 串行锁(实现_->最终消费串行互斥);
        auto& s = *实现_;
        const auto m = 读取登记材料();
        std::optional<新服务值治理定义> 定义;
        { std::lock_guard 锁(s.互斥); 定义 = s.治理定义; }
        if (!m || !定义) return {新服务值最终消费状态::等待配置, 定位};
        const auto 正式定义 = s.任务.读取服务值治理定义(定义->节点);
        if (!正式定义 || *正式定义 != *定义)
            throw std::logic_error("final governance definition changed");
        const 新结算能力引用 能力{新结算能力类别::自我服务值治理, 定义->节点};
        const auto 读取 = s.任务.读取任务最终消费来源(定位);
        if (读取.状态 != 新任务最终来源读取状态::可新消费&&读取.状态!=新任务最终来源读取状态::已准备待续)
            return 投影最终来源结果(定位, 读取);
        if (!读取.来源) throw std::logic_error("final source missing");
        const auto& 鲜读 = *读取.来源;
        // 来源场景须是自我当前所在场景，不与登记时场景比较（6220 §2）。
        const auto 当前自我场景 = s.世界.读取自我所在场景();
        if (!当前自我场景) throw std::logic_error("final self scene unregistered");
        if (鲜读.边界.能力 != 能力 || 鲜读.自我场景节点 != *当前自我场景
            || 鲜读.自我存在节点 != m->自我存在 || 鲜读.自我特征节点 != m->服务值特征)
            throw std::logic_error("final governance source identity mismatch");
        const auto E = s.世界.查询存在(m->自我存在);
        if (!E) throw std::logic_error("final self unreadable");
        std::optional<新特征信息> 集合;
        for (auto F : E->特征节点组) {
            const auto f = s.世界.查询特征(F);
            if (!f) throw std::logic_error("final self feature unreadable");
            if (取得特征类型根(s.概念, f->特征概念节点) != m->服务任务集合类型根) continue;
            if (集合) throw std::logic_error("final task collection ambiguous");
            集合 = f;
        }
        if (!集合 || !集合->当前值) throw std::logic_error("final task collection current value missing");
        const auto* 集合码 = std::get_if<稳定编码>(&*集合->当前值);
        if (!集合码 || !有效(*集合码)) throw std::logic_error("final task collection material identity invalid");
        const auto 集合值 = s.世界.获取事实服务引用().特征值服务.获取特征值(*集合码);
        if (!集合值 || 集合值->物理类型 != 材料物理类型::U64数组)
            throw std::logic_error("final task collection material missing or wrong type");
        const auto* 成员 = 集合值 ? std::get_if<std::vector<std::uint64_t>>(&集合值->材料) : nullptr;
        if (!成员) throw std::logic_error("final task collection payload invalid");
        std::set<std::uint64_t> 已见;
        for (auto 编码 : *成员)
            if (!编码 || !已见.insert(编码).second || !s.任务.获取任务(稳定编码{编码}))
                throw std::logic_error("final task collection member invalid or duplicated");
        if (!已见.contains(定位.来源任务节点.值))
            return {新服务值最终消费状态::来源条件不具备, 定位};
        auto 准备 = s.任务.准备任务最终消费(定位, 能力);
        if(准备.状态==新任务最终消费准备状态::资源延后)return {新服务值最终消费状态::资源延后,定位};
        if (准备.状态 == 新任务最终消费准备状态::已完成) {
            if (!准备.既有消费) throw std::logic_error("repeated final U missing");
            return 投影最终来源结果(定位, s.任务.读取任务最终消费来源(定位));
        }
        if (准备.状态 == 新任务最终消费准备状态::已入账待收尾) {
            if (!准备.准备) throw std::logic_error("resume final U token missing");
            return s.任务.完成已入账任务最终收尾(*准备.准备);
        }
        if (准备.状态 != 新任务最终消费准备状态::已准备)
            return 投影最终来源结果(定位, s.任务.读取任务最终消费来源(定位));
        if (!准备.准备) throw std::logic_error("final source token missing");
        const auto 冻结 = s.任务.读取任务最终消费准备(*准备.准备);
        const auto& 来源 = 冻结.来源;
        if (来源.来源 != 定位 || 来源.边界.能力 != 能力
            || 来源.自我场景节点 != *当前自我场景 || 来源.自我存在节点 != m->自我存在
            || 来源.自我特征节点 != m->服务值特征 || !来源.任务特征.服务值
            || 来源.任务特征.状态.当前值 != 9) // R108：F为累计值，入账按来源K贡献，不再要求F等于本次贡献
            throw std::logic_error("published P identity mismatch");
        auto 世界准备 = 实现_->世界.准备存在I64增量({来源.自我场景节点, 来源.自我存在节点,
            来源.自我特征节点, 来源.贡献值, 来源.边界.能力.节点, 冻结.准备节点, 来源.自我存在节点});
        const auto 见证 = 实现_->世界.读取存在I64组合准备(世界准备);
        const auto 片段 = 实现_->任务.封存任务最终消费(*准备.准备,
            {见证.场景节点, 见证.存在节点, 见证.特征节点, 见证.前值, 见证.后值,
                见证.核心时间, 见证.普通动作节点, 见证.前状态节点, 见证.特征保护});
        const auto 世界结果 = 实现_->世界.提交存在I64增量并形成动态(世界准备, 片段);
        const auto 核心 = 实现_->任务.接收任务最终消费核心(*准备.准备, 世界结果.附加片段回执);
        const auto 完成 = 实现_->任务.完成任务最终消费(*准备.准备, 世界结果);
        if (完成.状态 != 新服务值最终消费状态::已完成 || 完成.消费节点 != 核心.消费节点
            || !完成.收尾阶段节点) throw std::logic_error("final U/J5 completion incomplete");
        const auto U = 实现_->任务.读取任务最终消费(定位.完成结算节点);
        if (!U || U->消费节点 != 完成.消费节点 || U->收尾阶段节点 != 完成.收尾阶段节点)
            throw std::logic_error("final U completion not readable");

        const auto 独立 = 读取最终来源结果(定位);
        if (独立.状态 != 新服务值最终消费状态::已完成 || 独立.消费节点 != U->消费节点
            || 独立.收尾阶段节点 != U->收尾阶段节点)
            throw std::logic_error("final source independent readback mismatch");
        return 独立;
    } catch(const std::bad_alloc&){return {新服务值最终消费状态::资源延后,定位};}
    catch(const std::length_error&){return {新服务值最终消费状态::资源延后,定位};}
    catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"消费正式来源",
            L"最终消费发布或收尾不完整，保留正式事实且禁止重放");
    }
}
} // namespace 海中鱼巣
