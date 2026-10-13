#include "服务需求.本能运行服务.h"
#include "新_世界树类.h"
#include "../领域/新_任务类.h"
#include "../领域/新_本能函数管理类.h"
#include "../适配/适配器.UTC时钟.h"
#include <algorithm>
#include <map>
#include <mutex>
#include <set>

namespace 海中鱼巣 {
namespace {
std::string_view 登记名(新服务需求获取方式 方式) noexcept {
    return 方式 == 新服务需求获取方式::主动 ? 新本能函数登记名_主动获取服务需求
        : 方式 == 新服务需求获取方式::被动 ? 新本能函数登记名_被动获取服务需求 : std::string_view{};
}
bool 规格一致(const 新方法信息& 方法, const 新服务需求获取配置& 配置,
    新服务需求获取方式 方式) {
    if (登记名(方式).empty() || 方法.本能函数登记名 != 登记名(方式)
        || 方法.作用域 != 新方法作用域::内部治理 || 方法.实现方式 != 新方法实现方式::本能
        || 方法.启用状态 != 新方法启用状态::启用 || 方法.参数规格组.size() != 5
        || !方法.方法头.作用存在概念组.empty() || !方法.方法头.涉及特征类型概念组.empty()
        || 方法.方法头.可能结果概念组 != std::vector<稳定编码>{配置.信息包存在概念节点}) return false;
    constexpr 新方法参数对象种类 种类[] = {新方法参数对象种类::服务要求来源人类,
        新方法参数对象种类::服务要求目标存在, 新方法参数对象种类::服务要求目标特征类型,
        新方法参数对象种类::服务要求目标定义, 新方法参数对象种类::服务要求确认};
    for (std::size_t i = 0; i < 5; ++i) {
        const auto 位置 = std::find_if(方法.参数规格组.begin(), 方法.参数规格组.end(),
            [&](const auto& 项) { return 项.参数角色节点 == 配置.参数角色概念组[i]; });
        if (位置 == 方法.参数规格组.end() || 位置->对象种类 != 种类[i]
            || 位置->要求场景 || 位置->存在概念节点 || !位置->特征规格组.empty()) return false;
    }
    return true;
}
}
struct 服务需求本能运行服务::实现 final {
    新_世界树类& 世界;
    新_方法类& 方法;
    新_任务类& 任务;
    新_动态类& 动态;
    概念_存在类& 存在概念;
    概念_特征类& 特征概念;
    const 新服务要求材料读取接口& 材料读取;
    新服务需求工程报告函数 报告;
    mutable std::mutex 互斥;
    std::optional<新服务需求获取配置> 配置;
    std::map<新服务需求获取方式, 稳定编码> 登记方法;
    struct 调用身份 { 稳定编码 方法, 执行; 新服务需求获取方式 方式; bool 动作已请求 = false; };
    std::map<const 方法参数场景*, 调用身份> 调用组;
    bool 初始化中 = false, 就绪 = false, 已关闭 = false;

    bool 校验任务方法(稳定编码 M, 稳定编码 X, 新服务需求获取方式 方式,
        const 新服务需求获取配置& 配置值, 稳定编码 抽象方法) const {
        const auto 执行 = 任务.获取执行实例(X);
        if (!执行 || 执行->状态 != 新任务执行状态::执行中 || 执行->结束时间
            || 执行->方法节点 != M) return false;
        const auto t = 任务.获取任务(执行->任务节点);
        const auto m = 方法.获取方法(M);
        if (!t || t->当前状态 != 新任务状态::执行中 || t->最近执行实例节点 != X
            || !m || !规格一致(*m, 配置值, 方式)) return false;
        if (t->方法实例节点) {
            if (!m->来源方法节点 || m->实例根节点 != t->方法实例节点) return false;
            const auto 图 = 方法.获取混合方法实例(*t->方法实例节点);
            if (!图) return false;
            auto 待查 = 图->候选方法实例根组;
            std::set<稳定编码> 已查;
            bool 找到 = false;
            while (!待查.empty()) {
                const auto 节点 = 待查.back(); 待查.pop_back();
                if (!已查.insert(节点).second) continue;
                const auto 项 = 方法.获取方法(节点);
                if (!项 || 项->实例根节点 != t->方法实例节点) return false;
                if (节点 == M) 找到 = true;
                for (const auto& 步骤 : 项->组合步骤组) 待查.push_back(步骤.调用方法节点);
            }
            if (!找到) return false;
        } else if (m->实例根节点 || m->来源方法节点 || M != 抽象方法) return false;
        auto 来源 = *m;
        std::set<稳定编码> 已查来源;
        while (来源.节点 != 抽象方法) {
            if (!已查来源.insert(来源.节点).second || !来源.来源方法节点) return false;
            const auto 上游 = 方法.获取方法(*来源.来源方法节点);
            if (!上游 || !规格一致(*上游, 配置值, 方式)) return false;
            来源 = *上游;
        }
        return !来源.来源方法节点 && !来源.实例根节点;
    }
};
服务需求本能运行服务::服务需求本能运行服务(新_世界树类& 世界, 新_方法类& 方法,
    新_任务类& 任务, 新_动态类& 动态, 概念_存在类& 存在概念, 概念_特征类& 特征概念,
    const 新服务要求材料读取接口& 材料读取, 新服务需求工程报告函数 报告) noexcept {
    try { 实现_ = std::make_unique<实现>(世界, 方法, 任务, 动态, 存在概念, 特征概念, 材料读取, 报告); }
    catch (...) { if (报告) 报告("服务需求本能运行服务：构造资源失败"); }
}
服务需求本能运行服务::~服务需求本能运行服务() = default;
bool 服务需求本能运行服务::初始化(const 新服务需求获取配置& 配置) noexcept {
    if (!实现_) return false;
    auto& s = *实现_;
    try {
        const auto 保活 = shared_from_this();
        {
            std::lock_guard 锁(s.互斥);
            if (s.已关闭 || s.初始化中 || (s.配置 && *s.配置 != 配置)) return false;
            if (s.就绪) return true;
            s.初始化中 = true;
        }
        struct 守卫 { 实现& s; ~守卫() { std::lock_guard 锁(s.互斥); s.初始化中 = false; } } 完成{s};
        const auto 场景 = s.世界.查询场景(配置.所在场景节点);
        if (!场景 || !s.世界.查询存在(配置.自我存在节点)
            || std::find(场景->直接成员存在组.begin(), 场景->直接成员存在组.end(), 配置.自我存在节点)
                == 场景->直接成员存在组.end()
            || !s.存在概念.获取存在概念(配置.信息包存在概念节点)) return false;
        std::set<稳定编码> 角色;
        for (auto 节点 : 配置.参数角色概念组)
            if (!角色.insert(节点).second || !s.特征概念.获取特征概念(节点)) return false;
        const auto 注册 = 新_本能函数管理类::初始化();
        if (注册 != 新本能函数登记状态::已登记 && 注册 != 新本能函数登记状态::精确重复) return false;
        { std::lock_guard 锁(s.互斥); s.配置 = 配置; }
        const auto 装配 = 新_本能函数管理类::装配服务需求获取端口(保活);
        if (装配 != 新本能函数端口装配状态::已装配 && 装配 != 新本能函数端口装配状态::精确重复) return false;
        const auto 参数 = s.方法.建立服务需求登记参数场景();
        if (!参数.参数场景) return false;
        for (auto 方式 : {新服务需求获取方式::主动, 新服务需求获取方式::被动}) {
            const auto 结果 = 新_本能函数管理类::执行(登记名(方式), *参数.参数场景);
            if (结果.结果.状态 != 新本能函数执行状态::已按合同返回) return false;
        }
        { std::lock_guard 锁(s.互斥); s.就绪 = s.登记方法.size() == 2; return s.就绪; }
    } catch (...) { 报告工程故障("获取服务需求：初始化异常"); return false; }
}
bool 服务需求本能运行服务::关闭() noexcept {
    if (!实现_) return false;
    try {
        { std::lock_guard 锁(实现_->互斥);
          if (实现_->初始化中) return false;
          if (实现_->已关闭) return true;
          实现_->已关闭 = true; }
        return 新_本能函数管理类::关闭服务需求获取端口(*this) == 新本能函数端口装配状态::已关闭;
    } catch (...) { return false; }
}
std::optional<新服务需求获取配置> 服务需求本能运行服务::读取登记材料() const noexcept {
    if (!实现_) return {};
    try { std::lock_guard 锁(实现_->互斥); return 实现_->配置; } catch (...) { return {}; }
}
新方法建立结果 服务需求本能运行服务::确保方法登记(新服务需求获取方式 方式,
    const 新方法知识建立请求& 请求) noexcept {
    if (!实现_ || 登记名(方式).empty()) return {};
    try {
        std::optional<新服务需求获取配置> 配置;
        { std::lock_guard 锁(实现_->互斥);
          if (实现_->已关闭 || !实现_->初始化中) return {};
          配置 = 实现_->配置; }
        if (!配置 || !请求.条件结果组.empty()) return {};
        新方法信息 校验;
        校验.本能函数登记名 = 登记名(方式); 校验.作用域 = 请求.作用域;
        校验.实现方式 = 请求.实现方式; 校验.方法头 = 请求.方法头; 校验.参数规格组 = 请求.参数规格组;
        if (!规格一致(校验, *配置, 方式)) return {};
        const auto 结果 = 实现_->方法.确保本能方法登记(std::string(登记名(方式)), 请求);
        if (!结果.方法节点 || (结果.状态 != 新方法操作状态::已建立 && 结果.状态 != 新方法操作状态::已找到)) return 结果;
        const auto 读回 = 实现_->方法.获取方法(*结果.方法节点);
        if (!读回 || !规格一致(*读回, *配置, 方式) || 读回->来源方法节点 || 读回->实例根节点
            || (结果.状态 == 新方法操作状态::已建立 && !读回->条件结果组.empty()))
            return {新方法操作状态::结构不一致, {}};
        { std::lock_guard 锁(实现_->互斥); 实现_->登记方法.insert_or_assign(方式, *结果.方法节点); }
        return 结果;
    } catch (...) { return {新方法操作状态::资源失败, {}}; }
}
新方法本能调用结果 服务需求本能运行服务::执行(稳定编码 M,
    const 方法参数场景& 场景, 稳定编码 X) noexcept {
    if (!实现_ || 场景.用途() != 新方法参数场景用途::执行 || 场景.方法节点() != M
        || !场景.使用服务要求读取接口(实现_->材料读取)) return {};
    try {
        const auto 保活 = shared_from_this();
        auto& s = *实现_;
        const auto m = s.方法.获取方法(M);
        if (!m) return {};
        const auto 方式 = m->本能函数登记名 == 新本能函数登记名_主动获取服务需求
            ? 新服务需求获取方式::主动 : 新服务需求获取方式::被动;
        std::optional<新服务需求获取配置> 配置;
        稳定编码 抽象;
        { std::lock_guard 锁(s.互斥);
          if (!s.就绪 || s.已关闭 || !s.登记方法.contains(方式)) return {};
          配置 = s.配置; 抽象 = s.登记方法.at(方式); }
        if (!配置 || !s.校验任务方法(M, X, 方式, *配置, 抽象)) return {};
        { std::lock_guard 锁(s.互斥);
          if (s.已关闭 || s.调用组.contains(&场景)) return {};
          for (const auto& 项 : s.调用组) if (项.second.执行 == X) return {};
          s.调用组.emplace(&场景, 实现::调用身份{M, X, 方式}); }
        struct 守卫 { 实现& s; const 方法参数场景* 场景;
            ~守卫() { std::lock_guard 锁(s.互斥); s.调用组.erase(场景); } } 完成{s, &场景};
        return s.方法.调用本能函数(M, 场景);
    } catch (...) { 报告工程故障("获取服务需求：执行绑定异常"); return {新方法本能调用状态::内部错误, {}}; }
}
bool 服务需求本能运行服务::核验执行(新服务需求获取方式 方式, const 方法参数场景& 场景) const noexcept {
    if (!实现_ || 场景.用途() != 新方法参数场景用途::执行
        || !场景.使用服务要求读取接口(实现_->材料读取)) return false;
    try {
        std::optional<实现::调用身份> 调用;
        std::optional<新服务需求获取配置> 配置;
        稳定编码 抽象;
        { std::lock_guard 锁(实现_->互斥);
          const auto it = 实现_->调用组.find(&场景);
          if (it == 实现_->调用组.end() || it->second.方式 != 方式 || !实现_->登记方法.contains(方式)) return false;
          调用 = it->second; 配置 = 实现_->配置; 抽象 = 实现_->登记方法.at(方式); }
        return 配置 && 场景.方法节点() == 调用->方法
            && 实现_->校验任务方法(调用->方法, 调用->执行, 方式, *配置, 抽象);
    } catch (...) { return false; }
}
新服务需求获取动作回执 服务需求本能运行服务::建立动作(新服务需求获取方式 方式,
    const 方法参数场景& 场景, 稳定编码 来源人类) noexcept {
    新服务需求获取动作回执 回执;
    if (!核验执行(方式, 场景)) return 回执;
    try {
        实现::调用身份 调用;
        新服务需求获取配置 配置;
        { std::lock_guard 锁(实现_->互斥);
          auto it = 实现_->调用组.find(&场景);
          if (it == 实现_->调用组.end() || it->second.动作已请求 || !实现_->配置) return 回执;
          it->second.动作已请求 = true; 调用 = it->second; 配置 = *实现_->配置; }
        const auto 确认 = 场景.复核绑定服务要求确认语义(配置.参数角色概念组[4]);
        if (确认.状态 != 新服务要求确认状态::当前有效 || !确认.确认
            || 确认.确认->来源人类节点 != 来源人类) return 回执;
        const auto 已有 = 实现_->动态.按来源查询动作(调用.执行);
        if (已有.状态 != 新动态操作状态::已完成) return 回执;
        if (已有.动作) { 回执.动作节点 = 已有.动作->节点; return 回执; }
        const auto 时间 = UTC时钟适配器{}.读取当前UTC时间证据_v1({});
        if (!时间.成功()) return 回执;
        const auto 发布 = 实现_->世界.建立动作动态(配置.所在场景节点,
            {配置.自我存在节点, 调用.方法, 调用.执行, 来源人类, std::nullopt,
                {新状态时间语义::实例绝对UTC, 时间.证据->UTC纳秒}});
        // 挂接失败可能已成功回滚删除；发布过程给出的编码不等于仍存在的事实。
        // 只交付owner当前仍可读回的同源动作，不把已删除编码写入运行材料。
        if (发布.失败回滚结果 != 新动态操作状态::已完成) {
            const auto 读回 = 实现_->动态.按来源查询动作(调用.执行);
            if (读回.状态 == 新动态操作状态::已完成 && 读回.动作)
                回执.动作节点 = 读回.动作->节点;
        }
        回执.已新建 = 发布.状态 == 新世界动作发布状态::已完成 && 回执.动作节点.has_value();
        return 回执;
    } catch (...) { 报告工程故障("获取服务需求：动作建立异常"); return 回执; }
}
void 服务需求本能运行服务::报告工程故障(std::string_view 文本) noexcept {
    if (实现_ && 实现_->报告) 实现_->报告(文本);
}
} // namespace 海中鱼巣
