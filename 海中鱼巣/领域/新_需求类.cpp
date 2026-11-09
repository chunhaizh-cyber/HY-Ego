#include "新_需求类.h"
#include "新_任务类.h"
#include "概念_存在类.h"
#include "新_语言类.h"
#include "语言含义图.数据.h"
#include "../核心/容错检查.h"
#include "../核心/日志系统.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <condition_variable>
#include <cwchar>
#include <limits>
#include <map>
#include <mutex>
#include <new>
#include <set>
#include <sstream>
#include <tuple>
#include <thread>
#include <utility>
#include <source_location>
#ifdef HY_EGO_DEBUG_LOG_DEMAND_SOURCE_GUARD
#include <cstdio>
#endif

namespace 海中鱼巣::领域同步内部 { extern std::recursive_mutex 存在特征互斥; }

namespace 海中鱼巣 {
#ifdef HY_EGO_DEBUG_LOG_DEMAND_SOURCE_GUARD
namespace 需求守卫诊断内部 {
thread_local char 最近故障[256]{};
}
#endif
struct 新_需求类::接收认知来源保护结果 final {
    新需求操作状态 状态=新需求操作状态::认知来源不合法;
    std::vector<基础联合节点快照> 完整保护;
};
namespace {
template<class 根解析器>
bool 比较关系适用纯判据(const 二次特征概念信息& 比较,稳定编码 FT,稳定编码 域,根解析器&& 读取根) {
    if(比较.参照特征概念节点!=std::optional{域})return false;
    bool exact=false;
    for(const auto& input:比较.输入定义组)if(input.来源种类==二次特征输入来源种类::特征准确值) {
        if(!input.特征类型概念节点)return false;
        const auto root=读取根(*input.特征类型概念节点);if(!root||*root!=FT)return false;exact=true;
    }return exact;
}
}

新需求目标定义核验结果 核验需求目标定义保护(const 新需求目标定义& t,
    const 概念_特征类& fc,const 概念_二次特征类& rc) {
    using S=新需求目标定义核验状态;
    新需求目标定义核验结果 result;
    if(t.引用内容定位)return result;
    const bool 扩展=t.新增成员来源见证.has_value();
    std::map<稳定编码,基础联合节点快照> protectedNodes;
    const auto merge=[&](const auto& read) {
        if(read.前提已变化){result.状态=S::前提已变化;return false;}
        if(read.状态==基础结构操作状态::资源失败){result.状态=S::资源失败;return false;}
        if(read.状态==基础结构操作状态::入口拒绝||read.状态==基础结构操作状态::节点不存在){result.状态=S::输入不合法;return false;}
        if(read.状态!=基础结构操作状态::已完成||!read.信息||read.预期节点组.empty()){result.状态=S::内部不一致;return false;}
        for(const auto& n:read.预期节点组) {
            const auto [it,added]=protectedNodes.emplace(n.结构.节点.编码,n);
            if(!added&&(it->second.结构.字段组!=n.结构.字段组||it->second.入向字段组!=n.入向字段组
                ||it->second.入向外部关系组!=n.入向外部关系组||it->second.出向外部关系组!=n.出向外部关系组)) {
                result.状态=S::前提已变化;return false;
            }
        }return true;
    };
    const auto f=fc.读取特征概念定义保护(t.目标特征类型概念节点);
    if(!merge(f)||f.信息->类型根!=t.目标特征类型概念节点)return result;
    if(t.目标形式==新需求目标形式::变化方向) {
        if(t.目标值域概念节点||t.目标比较关系概念节点||!t.目标变化方向
            ||(*t.目标变化方向!=新需求变化方向::增加&&*t.目标变化方向!=新需求变化方向::减少))return result;
    } else if(t.目标形式==新需求目标形式::值) {
        if(t.目标变化方向||!t.目标值域概念节点||!t.目标比较关系概念节点)return result;
        const auto d=fc.读取特征概念定义保护(*t.目标值域概念节点);
        const auto b=rc.读取二次特征定义保护(*t.目标比较关系概念节点);
        if(!merge(d)||!merge(b)||d.信息->类型根!=f.信息->类型根
            ||b.信息->参照特征概念节点!=t.目标值域概念节点)return result;
        std::map<稳定编码,稳定编码> inputRoots;
        for(const auto& i:b.信息->输入定义组) {
            if(!i.特征类型概念节点) {
                if(i.来源种类==二次特征输入来源种类::特征准确值)return result;
                continue;
            }
            const auto in=fc.读取特征概念定义保护(*i.特征类型概念节点);
            if(!merge(in))return result;
            inputRoots.emplace(*i.特征类型概念节点,in.信息->类型根);
        }
        if(!比较关系适用纯判据(*b.信息,f.信息->类型根,*t.目标值域概念节点,[&](稳定编码 C)->std::optional<稳定编码> {
            const auto it=inputRoots.find(C);return it==inputRoots.end()?std::nullopt:std::optional{it->second};
        }))return result;
        if(!merge(fc.读取特征概念定义保护(b.信息->输出特征概念.节点)))return result;
    } else return result;
    std::vector<稳定编码> ids;for(const auto& [id,n]:protectedNodes)ids.push_back(id);
    const auto fresh=全局基础数据集.查询节点组完整结构(ids);
    if(fresh.状态!=基础结构操作状态::已完成){
        if(fresh.状态==基础结构操作状态::资源失败)result.状态=S::资源失败;
        else result.状态=S::内部不一致;
        return result;
    }
    if(fresh.节点组.size()!=ids.size()){result.状态=S::内部不一致;return result;}
    for(const auto& n:fresh.节点组) {
        if(!n.快照){result.状态=S::前提已变化;return result;}
        const auto& old=protectedNodes.at(n.查询节点);const auto& x=*n.快照;
        if(old.结构.字段组!=x.结构.字段组||old.入向字段组!=x.入向字段组
            ||old.入向外部关系组!=x.入向外部关系组||old.出向外部关系组!=x.出向外部关系组) {
            result.状态=S::前提已变化;return result;
        }result.完整保护.push_back(x);
    }
    result.状态=扩展 ? S::未支持 : S::已完成;return result;
}

新需求当前满足结果 映射新增成员目标满足(const 新需求新增成员判定结果& r) {
    using S=新需求新增成员读取状态; using R=新需求当前满足状态;
    if(r.状态!=S::已读取 && (r.当前集合||r.原始新增数量||!r.见证组.empty()||!r.完整保护.empty()))
        throw std::logic_error("新增成员技术非成功携带成功载荷");
    switch(r.状态) {
    case S::已读取:
        if(!r.当前集合||!r.原始新增数量||r.完整保护.empty()||*r.原始新增数量<0
            ||(*r.原始新增数量==0&&!r.见证组.empty()))
            throw std::logic_error("新增成员成功判定载荷不完整");
        if(*r.原始新增数量==0)return {R::需增加,{},1};
        if(r.见证组.empty())return {R::未满足但无有向差值,{},{}};
        return {R::已满足,{},0};
    case S::前提已变化:return {R::目标引用前提已变化,{},{}};
    case S::依赖未就绪:return {R::目标内容依赖未就绪,{},{}};
    case S::未支持:return {R::目标内容未支持,{},{}};
    case S::资源失败:return {R::资源失败,{},{}};
    }
    throw std::logic_error("未知新增成员读取状态");
}

稳定编码 需求树根节点{};
稳定编码 安全根需求节点{};
稳定编码 服务根需求节点{};

namespace {

std::recursive_mutex 新需求互斥;

// 向上送出：取消仅结束本次读取，不产生部分全集或业务否定。
void 检查需求读取停止(std::stop_token 停止) {
    if (停止.stop_requested()) throw 新需求读取取消{};
}

std::unique_lock<std::recursive_mutex> 获取需求读取锁(std::stop_token 停止) {
    检查需求读取停止(停止);
    std::unique_lock<std::recursive_mutex> 锁(新需求互斥, std::defer_lock);
    if (!停止.stop_possible()) {
        锁.lock();
    } else {
        std::mutex 停车互斥;
        std::condition_variable_any 停车;
        while (!锁.try_lock()) {
            std::unique_lock 停车锁(停车互斥);
            (void)停车.wait_for(停车锁, 停止, std::chrono::milliseconds(10), [] { return false; });
            检查需求读取停止(停止);
        }
    }
    检查需求读取停止(停止);
    return 锁;
}

// 持有原回调引用，不复制function。必须在owner锁之后构造，先通知后释放锁。
struct 关联检查离域通知 final {
    const std::function<void()>& 通知;
    ~关联检查离域通知() noexcept {
        try { if (通知) 通知(); } catch (...) {}
    }
};

稳定编码 节点类型字段{};
稳定编码 需求树根节点类型{};
稳定编码 需求节点类型{};
稳定编码 需求列表项节点类型{};
稳定编码 目标存在字段{};
稳定编码 目标特征类型字段{};
稳定编码 目标形式字段{};
稳定编码 目标值域字段{};
稳定编码 目标比较关系字段{};
稳定编码 目标变化方向字段{};
稳定编码 需求目标承载格式字段{};

struct 需求目标读取上下文 final {
    新_特征类& 特征;
    概念_特征类& 概念;
};
bool 需求保护相同(const 基础联合节点快照&, const 基础联合节点快照&);
bool 并需求保护(std::vector<基础联合节点快照>&, const std::vector<基础联合节点快照>&);
bool 需求内容保护未变(const std::vector<基础联合节点快照>&);
bool 加需求存在保护(稳定编码, 新_存在类&, std::vector<基础联合节点快照>&);
新需求信息 读取需求完整物理(const 需求目标读取上下文&, 稳定编码);
稳定编码 来源类型字段{};
稳定编码 来源承接字段{};
稳定编码 服务要求确认字段{};
稳定编码 服务需求取得执行字段{};
稳定编码 外部服务要求字段{};
稳定编码 活动状态字段{};
稳定编码 需求权重字段{};
稳定编码 根角色字段{};
稳定编码 需求列表项字段{};
稳定编码 列表成员字段{};
稳定编码 安全根登记字段{};
稳定编码 服务根登记字段{};
稳定编码 需求主体字段{}, 需求执行域字段{}, 认知理解字段{}, 认知内容确认字段{};
稳定编码 认知来源要求字段{}, 被认知需求登记字段{}, 需求归属格式字段{};
稳定编码 首次接收消息字段{}, 首次接收范围字段{}, 首次接收路径字段{}, 首次采用含义字段{};
稳定编码 行动请求消息字段{}, 行动请求含义字段{}, 行动请求原需求字段{}, 行动请求人类字段{};
稳定编码 行动请求受话者字段{}, 行动请求范围字段{}, 行动请求路径字段{};
稳定编码 集合发布场景字段{}, 集合发布动作字段{};
稳定编码 需求首次发布首状态字段{};
稳定编码 需求发生关联类型{}, 发生场景字段{}, 发生主体字段{}, 发生特征字段{}, 发生需求字段{};
稳定编码 发生动作字段{}, 发生来源字段{}, 发生状态字段{}, 发生方式字段{};
稳定编码 需求信息完善阶段字段{};
稳定编码 需求迁移记录类型{}, 需求迁移来源项类型{}, 需求迁移登记字段{};
稳定编码 迁移场景字段{}, 迁移主体字段{}, 迁移原特征字段{}, 迁移新特征字段{}, 迁移当前需求字段{}, 迁移当前状态字段{};
稳定编码 迁移来源项字段{}, 迁移项需求字段{}, 迁移项动作字段{}, 迁移项前状态字段{}, 迁移项后状态字段{}, 迁移项序号字段{};
稳定编码 引用内容需求字段{}, 引用内容路径字段{}, 复合分解来源字段{}, 复合分解来源类型{};
稳定编码 目标新增成员见证字段{}, 新增成员见证登记字段{}, 新增成员见证类型{};
稳定编码 新增成员格式字段{}, 新增成员来源种类字段{}, 新增成员源类型字段{};
稳定编码 新增成员基准字段{}, 新增成员指定主体字段{}, 新增成员指定需求字段{};
稳定编码 复合父任务字段{}, 复合父需求字段{}, 复合结构概念字段{}, 复合择一约束字段{}, 复合所选分支字段{};
稳定编码 复合子路径字段{}, 复合子要求字段{};

稳定编码 服务取得实例登记字段{},服务取得实例来源字段{},服务取得实例记录类型{},实例原父目标类型{};
稳定编码 实例格式字段{},实例父任务字段{},实例目标槽字段{},实例根路径字段{},实例主体字段{};
稳定编码 实例父需求字段{},实例原父目标字段{},实例人类概念字段{},实例需求身份概念字段{};
稳定编码 实例完整内容概念字段{},实例需求字段{},实例复用指定字段{},所属实例记录字段{};

// 本owner物理校验不取需求锁，也不反入D业务解码，供F持锁身份回调使用。
新服务取得目标实例信息 解码服务取得实例记录物理(稳定编码,std::vector<基础联合节点快照>&);
void 核验服务取得D来源物理(const 基础节点结构&,std::vector<基础联合节点快照>&);

constexpr std::int64_t 需求父子关系角色 = 1;

bool 节点仍存在(稳定编码 节点) noexcept {
    return 有效(节点) && 全局基础数据集.查询节点(节点).has_value();
}

std::vector<稳定编码> 需求结构固定节点组() {
    return {
        需求树根节点, 节点类型字段, 需求树根节点类型, 需求节点类型,
        需求列表项节点类型, 目标存在字段, 目标特征类型字段,
        目标形式字段, 目标值域字段, 目标比较关系字段,
        目标变化方向字段, 需求目标承载格式字段,
        来源类型字段, 来源承接字段, 服务要求确认字段, 服务需求取得执行字段, 外部服务要求字段, 活动状态字段, 需求权重字段,
        根角色字段,
        需求列表项字段, 列表成员字段, 安全根登记字段, 服务根登记字段,
        需求主体字段, 需求执行域字段, 认知理解字段, 认知内容确认字段,
        认知来源要求字段, 被认知需求登记字段, 需求归属格式字段,
        首次接收消息字段, 首次接收范围字段, 首次接收路径字段, 首次采用含义字段,
        行动请求消息字段, 行动请求含义字段, 行动请求原需求字段, 行动请求人类字段,
        行动请求受话者字段, 行动请求范围字段, 行动请求路径字段,
        集合发布场景字段, 集合发布动作字段, 需求首次发布首状态字段, 需求信息完善阶段字段,
        需求发生关联类型,发生场景字段,发生主体字段,发生特征字段,发生需求字段,
        发生动作字段,发生来源字段,发生状态字段,发生方式字段,
        需求迁移记录类型, 需求迁移来源项类型, 需求迁移登记字段,
        迁移场景字段, 迁移主体字段, 迁移原特征字段, 迁移新特征字段, 迁移当前需求字段, 迁移当前状态字段,
        迁移来源项字段, 迁移项需求字段, 迁移项动作字段, 迁移项前状态字段, 迁移项后状态字段, 迁移项序号字段,
        目标新增成员见证字段,新增成员见证登记字段,新增成员见证类型,
        新增成员格式字段,新增成员来源种类字段,新增成员源类型字段,
        新增成员基准字段,新增成员指定主体字段,新增成员指定需求字段,
        引用内容需求字段,引用内容路径字段,复合分解来源字段,复合分解来源类型,
        复合父任务字段,复合父需求字段,复合结构概念字段,复合择一约束字段,复合所选分支字段,复合子路径字段,复合子要求字段,
        服务取得实例登记字段,服务取得实例来源字段,服务取得实例记录类型,实例原父目标类型,
        实例格式字段,实例父任务字段,实例目标槽字段,实例根路径字段,实例主体字段,实例父需求字段,
        实例原父目标字段,实例人类概念字段,实例需求身份概念字段,实例完整内容概念字段,实例需求字段,实例复用指定字段,所属实例记录字段};
}

bool 结构已初始化() {
    const auto 节点组=需求结构固定节点组();
    return std::all_of(节点组.begin(), 节点组.end(), [](稳定编码 节点) {
        return 有效(节点);
    });
}

bool 结构仍存在() {
    if (!结构已初始化()) return false;
    const auto 节点组=需求结构固定节点组();
    if (!std::all_of(节点组.begin(), 节点组.end(), 节点仍存在)) return false;
    const auto 根类型组 = 全局基础数据集.查询字段(
        需求树根节点, 节点类型字段);
    if (根类型组.size() != 1) return false;
    const auto* 根类型 = std::get_if<稳定编码>(&根类型组.front().内容);
    return 根类型 && *根类型 == 需求树根节点类型;
}

std::optional<稳定编码> 读取唯一节点字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 目标 = std::get_if<稳定编码>(&字段组.front().内容);
    return 目标 ? std::optional<稳定编码>{*目标} : std::nullopt;
}

std::optional<std::int64_t> 读取唯一I64字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&字段组.front().内容);
    if (!值) return std::nullopt;
    const auto* I64 = std::get_if<std::int64_t>(&值->材料);
    return I64 ? std::optional<std::int64_t>{*I64} : std::nullopt;
}

std::vector<稳定编码> 读取多节点字段(稳定编码 节点, 稳定编码 字段) {
    std::vector<稳定编码> 结果;
    for (const auto& 关系 : 全局基础数据集.查询字段(节点, 字段)) {
        const auto* 目标 = std::get_if<稳定编码>(&关系.内容);
        if (目标) 结果.push_back(*目标);
    }
    std::sort(结果.begin(), 结果.end());
    结果.erase(std::unique(结果.begin(), 结果.end()), 结果.end());
    return 结果;
}

bool 是活动状态值(std::int64_t 值) noexcept {
    return 值 >= static_cast<std::int64_t>(新需求活动状态::活动)
        && 值 <= static_cast<std::int64_t>(新需求活动状态::失活);
}

std::optional<新需求来源类型> 转换来源类型(std::int64_t 值) noexcept {
    if (值 == static_cast<std::int64_t>(新需求来源类型::本能定义)) {
        return 新需求来源类型::本能定义;
    }
    if (值 == static_cast<std::int64_t>(新需求来源类型::任务子目标)) {
        return 新需求来源类型::任务子目标;
    }
    if (值 == static_cast<std::int64_t>(新需求来源类型::外部服务要求)) return 新需求来源类型::外部服务要求;
    if (值 == static_cast<std::int64_t>(新需求来源类型::行动请求)) return 新需求来源类型::行动请求;
    return std::nullopt;
}

std::vector<基础外部关系> 查询父需求关系(稳定编码 需求节点) {
    std::vector<基础外部关系> 结果;
    for (const auto& 关系 : 全局基础数据集.查询目标关系(
        需求节点, 基础外部关系类型::父子)) {
        if (关系.角色或顺序 == 需求父子关系角色) 结果.push_back(关系);
    }
    return 结果;
}

std::vector<基础外部关系> 查询子需求关系(稳定编码 需求节点) {
    std::vector<基础外部关系> 结果;
    for (const auto& 关系 : 全局基础数据集.查询源关系(
        需求节点, 基础外部关系类型::父子)) {
        if (关系.角色或顺序 == 需求父子关系角色) 结果.push_back(关系);
    }
    return 结果;
}

bool 是需求类型节点(稳定编码 节点) {
    const auto 类型 = 读取唯一节点字段(节点, 节点类型字段);
    return 类型 && *类型 == 需求节点类型;
}

bool 是列表项类型节点(稳定编码 节点) {
    const auto 类型 = 读取唯一节点字段(节点, 节点类型字段);
    return 类型 && *类型 == 需求列表项节点类型;
}

std::optional<新需求根角色> 转换根角色(std::int64_t 值) noexcept {
    if (值 == static_cast<std::int64_t>(新需求根角色::安全)) {
        return 新需求根角色::安全;
    }
    if (值 == static_cast<std::int64_t>(新需求根角色::服务)) {
        return 新需求根角色::服务;
    }
    return std::nullopt;
}

bool 目标相同(const 新需求目标定义& 左, const 新需求目标定义& 右) {
    return 左 == 右;
}

std::optional<新需求目标形式> 转换目标形式(std::int64_t 值) noexcept {
    if (值 == static_cast<std::int64_t>(新需求目标形式::值)) {
        return 新需求目标形式::值;
    }
    if (值 == static_cast<std::int64_t>(新需求目标形式::变化方向)) {
        return 新需求目标形式::变化方向;
    }
    return std::nullopt;
}

std::optional<新需求变化方向> 转换目标变化方向(std::int64_t 值) noexcept {
    if (值 == static_cast<std::int64_t>(新需求变化方向::增加)) {
        return 新需求变化方向::增加;
    }
    if (值 == static_cast<std::int64_t>(新需求变化方向::减少)) {
        return 新需求变化方向::减少;
    }
    return std::nullopt;
}

bool 目标形状合法(const 新需求目标定义& 目标) noexcept {
    if(目标.新增成员来源见证) {
        const auto& c=*目标.新增成员来源见证;
        if(目标.引用内容定位||目标.目标形式!=新需求目标形式::值||目标.目标变化方向
            ||!有效(c.基准材料节点)||!有效(c.要求.源特征类型概念节点)
            ||(c.要求.指定被认知需求&&!c.要求.指定来源主体)
            ||(c.要求.指定来源主体&&!有效(*c.要求.指定来源主体))
            ||(c.要求.指定被认知需求&&!有效(*c.要求.指定被认知需求)))return false;
    }
    if (!有效(目标.目标特征类型概念节点)) return false;
    if(目标.引用内容定位)return 目标.目标形式==新需求目标形式::值
        &&目标.目标值域概念节点&&有效(*目标.目标值域概念节点)
        &&!目标.目标比较关系概念节点&&!目标.目标变化方向
        &&有效(目标.引用内容定位->引用需求节点)&&目标.引用内容定位->正式内容路径.size()<=1
        &&std::all_of(目标.引用内容定位->正式内容路径.begin(),目标.引用内容定位->正式内容路径.end(),[](auto n){return 有效(n);});
    if (目标.目标形式 == 新需求目标形式::值) {
        return 目标.目标值域概念节点.has_value()
            && 有效(*目标.目标值域概念节点)
            && 目标.目标比较关系概念节点.has_value()
            && 有效(*目标.目标比较关系概念节点)
            && !目标.目标变化方向.has_value();
    }
    if (目标.目标形式 == 新需求目标形式::变化方向) {
        return !目标.目标值域概念节点.has_value()
            && !目标.目标比较关系概念节点.has_value()
            && 目标.目标变化方向.has_value()
            && 转换目标变化方向(static_cast<std::int64_t>(
                *目标.目标变化方向)).has_value();
    }
    return false;
}

std::optional<新需求新增成员见证约束> 读取新增成员约束(稳定编码);
std::optional<新需求目标定义> 读取目标定义(稳定编码 节点) {
    const auto 特征类型 = 读取唯一节点字段(节点, 目标特征类型字段);
    const auto 形式值 = 读取唯一I64字段(节点, 目标形式字段);
    const auto 形式 = 形式值 ? 转换目标形式(*形式值) : std::nullopt;
    if (!特征类型 || !形式) return std::nullopt;

    新需求目标定义 目标;
    目标.新增成员来源见证=读取新增成员约束(节点);
    目标.目标特征类型概念节点 = *特征类型;
    目标.目标形式 = *形式;
    const auto 固定D=读取唯一节点字段(节点,引用内容需求字段);
    const auto 路径=读取多节点字段(节点,引用内容路径字段);
    if(路径.size()>1||(!固定D&&!路径.empty()))return {};
    if(固定D)目标.引用内容定位=新需求引用内容定位{*固定D,路径};
    const auto 值域组 = 全局基础数据集.查询字段(节点, 目标值域字段);
    const auto 关系组 = 全局基础数据集.查询字段(节点, 目标比较关系字段);
    const auto 方向组 = 全局基础数据集.查询字段(节点, 目标变化方向字段);
    if (值域组.size() > 1 || 关系组.size() > 1 || 方向组.size() > 1) {
        return std::nullopt;
    }
    if (!值域组.empty()) 目标.目标值域概念节点 = 读取唯一节点字段(
        节点, 目标值域字段);
    if (!关系组.empty()) 目标.目标比较关系概念节点 = 读取唯一节点字段(
        节点, 目标比较关系字段);
    if (!方向组.empty()) {
        const auto 方向值 = 读取唯一I64字段(节点, 目标变化方向字段);
        目标.目标变化方向 = 方向值
            ? 转换目标变化方向(*方向值) : std::nullopt;
    }
    return 目标形状合法(目标)
        ? std::optional<新需求目标定义>{目标} : std::nullopt;
}

// 完整列表读回不能把禁止字段的错类型载荷解释成“未设置”。
bool 原始目标字段一致(稳定编码 节点, const 新需求目标定义& 目标) {
    if(读取新增成员约束(节点)!=目标.新增成员来源见证)return false;
    const auto 固定D=读取唯一节点字段(节点,引用内容需求字段);
    const auto 路径=读取多节点字段(节点,引用内容路径字段);
    if(目标.引用内容定位) {
        if(固定D!=目标.引用内容定位->引用需求节点||路径!=目标.引用内容定位->正式内容路径)return false;
    } else if(固定D||!路径.empty())return false;
    const auto 节点字段一致 = [&](稳定编码 字段, std::optional<稳定编码> 期望) {
        const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
        if (!期望) return 字段组.empty();
        if (字段组.size() != 1) return false;
        const auto* 内容 = std::get_if<稳定编码>(&字段组.front().内容);
        return 内容 && *内容 == *期望;
    };
    if (!节点字段一致(目标值域字段, 目标.目标值域概念节点)
        || !节点字段一致(目标比较关系字段, 目标.目标比较关系概念节点)) return false;
    const auto 方向组 = 全局基础数据集.查询字段(节点, 目标变化方向字段);
    if (!目标.目标变化方向) return 方向组.empty();
    return 方向组.size() == 1 && 读取唯一I64字段(节点, 目标变化方向字段)
        == static_cast<std::int64_t>(*目标.目标变化方向);
}

std::optional<新需求复合分解来源> 读取复合来源(稳定编码);
std::optional<新需求信息> 读取需求信息(const 需求目标读取上下文& 上下文, 稳定编码 节点) {
    if (!结构已初始化()
        || 读取唯一节点字段(需求树根节点,节点类型字段) != 需求树根节点类型
        || !节点仍存在(节点) || !是需求类型节点(节点)) return {};
    const auto d = 读取需求完整物理(上下文, 节点);
    return d.执行域 == 新需求执行域::自我执行 ? std::optional{d} : std::nullopt;
}
std::optional<新需求列表项信息> 读取列表项信息(稳定编码 节点) {
    if (!结构已初始化()
        || 读取唯一节点字段(需求树根节点,节点类型字段) != 需求树根节点类型
        || !节点仍存在(节点) || !是列表项类型节点(节点)) {
        return std::nullopt;
    }
    const auto 成员 = 读取多节点字段(节点, 列表成员字段);
    if (成员.empty() || std::any_of(成员.begin(), 成员.end(), [](稳定编码 需求) {
        return !是需求类型节点(需求);
    })) {
        return std::nullopt;
    }

    新需求列表项信息 结果;
    结果.节点 = 节点;
    结果.需求节点组 = 成员;
    const auto 目标存在 = 读取唯一节点字段(节点, 目标存在字段);
    const auto 目标 = 读取目标定义(节点);
    if (!目标存在 || !目标) return std::nullopt;
    结果.目标存在节点 = *目标存在;
    结果.目标 = *目标;
    const auto 根角色组 = 全局基础数据集.查询字段(节点, 根角色字段);
    if (!根角色组.empty()) {
        if (根角色组.size() != 1 || 成员.size() != 1) return std::nullopt;
        const auto 根角色值 = 读取唯一I64字段(节点, 根角色字段);
        const auto 根角色 = 根角色值 ? 转换根角色(*根角色值) : std::nullopt;
        if (!根角色) return std::nullopt;
        结果.根角色 = *根角色;
        return 结果;
    }

    return 结果;
}

// 仅使用冻结schema与L1。概念比较持锁回调本段时，不得取得任何领域锁。
struct 需求物理读取失败 final {
    基础结构操作状态 原因;
    unsigned 诊断行=0;
    explicit 需求物理读取失败(基础结构操作状态 状态,
        const std::source_location& 位置=std::source_location::current()) noexcept
        : 原因(状态),诊断行(位置.line()) {}
};
void 要求需求物理(bool 成立,
    const std::source_location& 位置=std::source_location::current()) {
    if (!成立) throw 需求物理读取失败{基础结构操作状态::内部不一致,位置};
}
基础节点结构 读取需求物理节点(稳定编码 节点) {
    auto r = 全局基础数据集.查询完整节点结构(节点);
    if (r.状态 != 基础结构操作状态::已完成) throw 需求物理读取失败{r.状态};
    要求需求物理(r.结构 && r.结构->节点.编码 == 节点);
    return std::move(*r.结构);
}
std::array<稳定编码, 5> 需求目标字段键() {
    return {目标特征类型字段, 目标形式字段, 目标值域字段, 目标比较关系字段, 目标变化方向字段};
}
std::array<std::uint32_t, 5> 需求目标字段槽() {
    using namespace 先天特征类型槽;
    return {需求目标FT, 需求目标形式, 目标域, 目标比较, 目标方向};
}
bool 是F承载需求(const 基础节点结构& raw) {
    const 基础字段内容* marker = nullptr;
    for (const auto& f : raw.字段组) if (f.字段节点 == 需求目标承载格式字段) {
        要求需求物理(!marker); marker = &f.内容;
    }
    if (!marker) return false;
    const auto* v = std::get_if<基础值>(marker);
    const auto* n = v ? std::get_if<std::int64_t>(&v->材料) : nullptr;
    要求需求物理(n && *n == 2); return true;
}

// 只用于提议源：目标/来源/活动及父归属保持，权重由采用时的当前写集负责。
基础只读预期 构造需求提议源前提(const 基础联合节点快照& raw) {
    基础只读预期 out;
    out.节点组 = {raw.结构.节点, {需求权重字段}};
    基础只读字段集合预期 fields{raw.结构.节点.编码, {}, {需求权重字段}, {}};
    for (const auto& f : raw.结构.字段组)
        if (f.字段节点 != 需求权重字段) fields.字段组.push_back(f);
    out.字段集合组.push_back(std::move(fields));
    基础只读外部关系集合预期 parent{raw.结构.节点.编码, false,
        基础外部关系类型::父子, 需求父子关系角色, {}, {}};
    for (const auto& r : raw.入向外部关系组)
        if (r.类型 == parent.类型 && r.角色或顺序 == *parent.角色或顺序)
            parent.关系组.push_back(r);
    out.外部关系集合组.push_back(std::move(parent));
    return out;
}
bool 需求提议源未变(const 基础只读预期& p) {
    const auto s = 全局基础数据集.核验只读预期(p);
    if (s == 基础节点组追加状态::资源失败) throw std::bad_alloc{};
    if (s == 基础节点组追加状态::预期不符 || s == 基础节点组追加状态::节点不存在
        || s == 基础节点组追加状态::引用不存在) return false;
    要求需求物理(s == 基础节点组追加状态::已完成); return true;
}

// 仅为旧逻辑解码构造调用期投影。真实raw和保护永远不被此投影替换。
基础节点结构 投影需求目标实例(const 需求目标读取上下文& ctx, const 基础节点结构& raw,
    std::vector<基础联合节点快照>* 完整保护 = nullptr,bool 来源枚举读取 = false,
    const 基础只读预期* 提议源 = nullptr) {
    if (!是F承载需求(raw)) return raw;
    std::lock_guard lock(领域同步内部::存在特征互斥);
    std::vector<基础联合节点快照> protect;
    if (提议源) {
        if (!需求提议源未变(*提议源)) throw 新需求目标定义核验状态::前提已变化;
    } else {
        const auto captured = 全局基础数据集.查询节点组完整结构({raw.节点.编码});
        if (captured.状态 == 基础结构操作状态::资源失败) throw std::bad_alloc{};
        要求需求物理(captured.状态 == 基础结构操作状态::已完成 && captured.节点组.size() == 1);
        if (!captured.节点组.front().快照 || captured.节点组.front().快照->结构.字段组 != raw.字段组)
            throw 新需求目标定义核验状态::前提已变化;
        protect.push_back(*captured.节点组.front().快照);
    }
    const auto 合并来源读取保护 = [&](const std::vector<基础联合节点快照>& 组) {
        if (!并需求保护(protect, 组) || (完整保护 && !并需求保护(*完整保护, 组)))
            throw 新需求目标定义核验状态::前提已变化;
    };
    if (来源枚举读取) 合并来源读取保护(protect);
    const auto keys = 需求目标字段键(); const auto slots = 需求目标字段槽();
    std::set<稳定编码> used;
    基础节点结构 projected = raw;
    std::erase_if(projected.字段组, [](const auto& f) { return f.字段节点 == 需求目标承载格式字段; });
    for (std::size_t i = 0; i < keys.size(); ++i) {
        auto it = std::find_if(projected.字段组.begin(), projected.字段组.end(), [&](const auto& f) { return f.字段节点 == keys[i]; });
        要求需求物理(it != projected.字段组.end()
            && std::count_if(raw.字段组.begin(), raw.字段组.end(), [&](const auto& f) { return f.字段节点 == keys[i]; }) == 1);
        const auto* F = std::get_if<稳定编码>(&it->内容);
        要求需求物理(F && 有效(*F) && used.insert(*F).second);
        const auto root = ctx.概念.读取先天特征类型槽(slots[i]);
        if (root.状态 == 特征概念规则状态::资源失败) throw std::bad_alloc{};
        if (来源枚举读取) {
            if (root.状态 == 特征概念规则状态::前提已变化) throw 新需求目标定义核验状态::前提已变化;
            if (root.状态 == 特征概念规则状态::读取接口未装配 || root.状态 == 特征概念规则状态::值不存在)
                throw 新需求目标定义核验状态::依赖未就绪;
        }
        if (来源枚举读取) {
            要求需求物理(root.状态 == 特征概念规则状态::已完成 && !root.完整保护组.empty());
            合并来源读取保护(root.完整保护组);
        }
        要求需求物理(root.状态 == 特征概念规则状态::已完成 && root.类型根);
        if (来源枚举读取) {
            // 先证明本F确属固定槽，再把实际值reader的未装配作为依赖，
            // 不能用值读取覆盖缺口掩盖稳定的F身份/类型错配。
            const auto head = ctx.特征.读取特征头保护(*F);
            if (head.状态 == 新特征头保护状态::资源失败) throw std::bad_alloc{};
            if (head.状态 == 新特征头保护状态::前提已变化 || head.状态 == 新特征头保护状态::特征不存在)
                throw 新需求目标定义核验状态::前提已变化;
            if (head.状态 == 新特征头保护状态::依赖未就绪) throw 新需求目标定义核验状态::依赖未就绪;
            要求需求物理(head.状态 == 新特征头保护状态::已读取 && head.信息 && !head.完整保护.empty());
            // 实际头先碰撞调用者预采批，再判断固定槽身份，避免瞬时变化被误判为稳定损坏。
            合并来源读取保护(head.完整保护);
            要求需求物理(head.信息->特征节点 == *F && head.信息->特征类型根 == *root.类型根
                && !head.信息->上级特征节点);
        }
        const auto read = ctx.特征.读取特征完整保护(*F);
        if (read.状态 == 新特征完整读取状态::资源失败) throw std::bad_alloc{};
        if (read.状态 == 新特征完整读取状态::前提已变化) throw 新需求目标定义核验状态::前提已变化;
        if (来源枚举读取) {
            if (read.状态 == 新特征完整读取状态::特征不存在) throw 新需求目标定义核验状态::前提已变化;
            if (read.状态 == 新特征完整读取状态::依赖未就绪) throw 新需求目标定义核验状态::依赖未就绪;
            if (read.状态 == 新特征完整读取状态::已读取 && read.信息)
                合并来源读取保护(read.信息->完整保护组);
        }
        要求需求物理(read.状态 == 新特征完整读取状态::已读取 && read.信息
            && read.信息->特征.节点 == *F && read.信息->特征类型根 == *root.类型根
            && !read.信息->特征.上级特征节点);
        const auto own = std::find_if(read.信息->完整保护组.begin(), read.信息->完整保护组.end(), [&](const auto& p) { return p.结构.节点.编码 == *F; });
        要求需求物理(own != read.信息->完整保护组.end());
        std::size_t holders = 0;
        for (const auto& f : own->入向字段组) if (std::find(keys.begin(), keys.end(), f.字段节点) != keys.end()) {
            要求需求物理(f.所属节点 == raw.节点.编码 && f.字段节点 == keys[i]); ++holders;
        }
        要求需求物理(holders == 1);
        if (!并需求保护(protect, root.完整保护组) || !并需求保护(protect, read.信息->完整保护组))
            throw 新需求目标定义核验状态::前提已变化;
        if (!read.信息->特征.当前值) projected.字段组.erase(it);
        else std::visit([&](const auto& v) {
            using T = std::decay_t<decltype(v)>;
            if constexpr (std::is_same_v<T, std::int64_t>) it->内容 = 基础值{v};
            else it->内容 = v;
        }, *read.信息->特征.当前值);
    }
    if (!需求内容保护未变(protect) || (提议源 && !需求提议源未变(*提议源)))
        throw 新需求目标定义核验状态::前提已变化;
    if (完整保护 && !并需求保护(*完整保护, protect)) throw 新需求目标定义核验状态::前提已变化;
    return projected;
}

struct 需求物理游标 final {
    const 基础节点结构& 结构;
    std::set<稳定编码> 已读;
    std::vector<const 基础字段内容*> 内容组(稳定编码 字段) {
        要求需求物理(已读.insert(字段).second);
        std::vector<const 基础字段内容*> r;
        for (const auto& f : 结构.字段组) {
            要求需求物理(f.所属节点 == 结构.节点.编码);
            if (f.字段节点 == 字段) r.push_back(&f.内容);
        }
        return r;
    }
    std::optional<稳定编码> 引用(稳定编码 字段, bool 必填 = true) {
        const auto r = 内容组(字段);
        要求需求物理(r.size() <= 1 && (!必填 || !r.empty()));
        if (r.empty()) return {};
        const auto* p = std::get_if<稳定编码>(r.front());
        要求需求物理(p && 有效(*p)); return *p;
    }
    std::optional<std::int64_t> 数值(稳定编码 字段, bool 必填 = true) {
        const auto r = 内容组(字段);
        要求需求物理(r.size() <= 1 && (!必填 || !r.empty()));
        if (r.empty()) return {};
        const auto* p = std::get_if<基础值>(r.front());
        const auto* n = p ? std::get_if<std::int64_t>(&p->材料) : nullptr;
        要求需求物理(n != nullptr); return *n;
    }
    std::vector<稳定编码> 引用组(稳定编码 字段) {
        std::vector<稳定编码> r;
        for (const auto* v : 内容组(字段)) {
            const auto* p = std::get_if<稳定编码>(v);
            要求需求物理(p && 有效(*p)); r.push_back(*p);
        }
        std::sort(r.begin(), r.end());
        要求需求物理(std::adjacent_find(r.begin(), r.end()) == r.end()); return r;
    }
    新需求目标定义 目标() {
        新需求目标定义 r;
        const auto cw=引用(目标新增成员见证字段,false);
        if(cw)r.新增成员来源见证=读取新增成员约束(结构.节点.编码);
        r.目标特征类型概念节点 = *引用(目标特征类型字段);
        const auto 形式 = 转换目标形式(*数值(目标形式字段));
        要求需求物理(形式.has_value()); r.目标形式 = *形式;
        r.目标值域概念节点 = 引用(目标值域字段, false);
        r.目标比较关系概念节点 = 引用(目标比较关系字段, false);
        const auto 固定D=引用(引用内容需求字段,false);
        const auto 路径=引用组(引用内容路径字段);
        要求需求物理(路径.size()<=1&&(固定D||路径.empty()));
        if(固定D)r.引用内容定位=新需求引用内容定位{*固定D,路径};
        const auto 方向 = 数值(目标变化方向字段, false);
        if (方向) { r.目标变化方向 = 转换目标变化方向(*方向); 要求需求物理(r.目标变化方向.has_value()); }
        要求需求物理(目标形状合法(r)); return r;
    }
    void 完成() const {
        for (const auto& f : 结构.字段组) 要求需求物理(已读.contains(f.字段节点));
    }
};
std::optional<新需求新增成员见证约束> 解码新增成员约束值(稳定编码 节点) {
    const auto raw=读取需求物理节点(节点);需求物理游标 v{raw,{}};
    要求需求物理(v.引用(节点类型字段)==新增成员见证类型
        &&v.数值(新增成员格式字段)==1&&v.数值(新增成员来源种类字段)==1);
    新需求新增成员见证约束 r;
    r.要求.源特征类型概念节点=*v.引用(新增成员源类型字段);
    r.基准材料节点=*v.引用(新增成员基准字段);
    r.要求.指定来源主体=v.引用(新增成员指定主体字段,false);
    r.要求.指定被认知需求=v.引用(新增成员指定需求字段,false);v.完成();
    要求需求物理(!r.要求.指定被认知需求||r.要求.指定来源主体);
    const auto 登记=全局基础数据集.查询字段(需求树根节点,新增成员见证登记字段);
    要求需求物理(std::count_if(登记.begin(),登记.end(),[&](const auto& f){
        const auto* p=std::get_if<稳定编码>(&f.内容);return p&&*p==节点;
    })==1);
    return r;
}
std::optional<新需求新增成员见证约束> 读取新增成员约束(稳定编码 节点) {
    const auto f=全局基础数据集.查询字段(节点,目标新增成员见证字段);
    要求需求物理(f.size()<=1);if(f.empty())return {};
    const auto* p=std::get_if<稳定编码>(&f.front().内容);要求需求物理(p&&有效(*p));
    return 解码新增成员约束值(*p);
}

基础联合节点快照 取得服务实例物理保护(稳定编码 节点,std::vector<基础联合节点快照>& 保护) {
    const auto r=全局基础数据集.查询节点组完整结构({节点});
    if(r.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
    要求需求物理(r.状态==基础结构操作状态::已完成&&r.节点组.size()==1
        &&r.节点组.front().查询节点==节点&&r.节点组.front().快照.has_value()
        &&r.节点组.front().快照->结构.节点.编码==节点);
    const auto& n=*r.节点组.front().快照;
    if(!并需求保护(保护,{n}))throw 新需求目标定义核验状态::前提已变化;
    return n;
}

void 追加服务取得原父目标快照字段(std::vector<基础组内初始字段>& 字段,
    稳定编码 父需求,const 新需求完整目标& g,std::vector<基础联合节点快照>& 保护) {
    try {
    (void)取得服务实例物理保护(父需求,保护);
    const auto& t=g.目标;
    const auto refs=全局基础数据集.查询字段(父需求,目标新增成员见证字段);
    要求需求物理(refs.size()<=1);
    if(t.新增成员来源见证.has_value()!=!refs.empty())throw 新服务取得目标实例状态::前提已变化;
    字段.push_back({目标存在字段,g.目标存在节点});
    字段.push_back({目标特征类型字段,t.目标特征类型概念节点});
    字段.push_back({目标形式字段,基础值{static_cast<std::int64_t>(t.目标形式)}});
    if(t.目标值域概念节点)字段.push_back({目标值域字段,*t.目标值域概念节点});
    if(t.目标比较关系概念节点)字段.push_back({目标比较关系字段,*t.目标比较关系概念节点});
    if(t.目标变化方向)字段.push_back({目标变化方向字段,基础值{static_cast<std::int64_t>(*t.目标变化方向)}});
    if(t.引用内容定位) {
        字段.push_back({引用内容需求字段,t.引用内容定位->引用需求节点});
        for(const auto FT:t.引用内容定位->正式内容路径)字段.push_back({引用内容路径字段,FT});
    }
    if(!refs.empty()) {
        const auto* cw=std::get_if<稳定编码>(&refs.front().内容);要求需求物理(cw&&有效(*cw));
        (void)取得服务实例物理保护(*cw,保护);
        if(解码新增成员约束值(*cw)!=t.新增成员来源见证)throw 新服务取得目标实例状态::前提已变化;
        (void)取得服务实例物理保护(t.新增成员来源见证->基准材料节点,保护);
        字段.push_back({目标新增成员见证字段,*cw});
    }
    if(!需求内容保护未变(保护))throw 新服务取得目标实例状态::前提已变化;
    } catch(新需求目标定义核验状态 s) {
        if(s==新需求目标定义核验状态::前提已变化)throw 新服务取得目标实例状态::前提已变化;
        throw;
    }
}

新服务取得目标实例信息 解码服务取得实例记录物理(稳定编码 R,std::vector<基础联合节点快照>& 保护) {
    const auto raw=取得服务实例物理保护(R,保护);需求物理游标 c{raw.结构,{}};
    要求需求物理(c.引用(节点类型字段)==服务取得实例记录类型&&c.数值(实例格式字段)==1
        &&raw.入向外部关系组.empty()&&raw.出向外部关系组.empty());
    新服务取得目标实例信息 out;out.出生记录=R;
    out.键.父取得任务=*c.引用(实例父任务字段);out.键.目标槽类型=*c.引用(实例目标槽字段);
    out.键.服务对象=*c.引用(实例主体字段);
    const auto paths=c.内容组(实例根路径字段);要求需求物理(paths.size()==1);
    const auto* bv=std::get_if<基础值>(paths.front());
    const auto* path=bv ? std::get_if<std::vector<std::uint64_t>>(&bv->材料) : nullptr;
    要求需求物理(path&&path->empty());
    out.父需求=*c.引用(实例父需求字段);const auto original=*c.引用(实例原父目标字段);
    out.人类概念=*c.引用(实例人类概念字段);out.需求身份概念=*c.引用(实例需求身份概念字段);
    out.完整内容概念=*c.引用(实例完整内容概念字段);out.需求节点=*c.引用(实例需求字段);
    const auto reuse=c.数值(实例复用指定字段);要求需求物理(reuse==0||reuse==1);c.完成();
    out.复用指定已有需求=reuse==1;
    const auto saved=取得服务实例物理保护(original,保护);需求物理游标 v{saved.结构,{}};
    要求需求物理(v.引用(节点类型字段)==实例原父目标类型&&v.引用(所属实例记录字段)==R
        &&saved.入向外部关系组.empty()&&saved.出向外部关系组.empty());
    out.原父目标.目标存在节点=*v.引用(目标存在字段);out.原父目标.目标=v.目标();v.完成();
    要求需求物理(std::count_if(saved.入向字段组.begin(),saved.入向字段组.end(),[&](const auto& f){
        return f.所属节点==R&&f.字段节点==实例原父目标字段;})==1);
    if(out.原父目标.目标.新增成员来源见证) {
        const auto cw=读取唯一节点字段(original,目标新增成员见证字段);要求需求物理(cw.has_value());
        (void)取得服务实例物理保护(*cw,保护);
        (void)取得服务实例物理保护(out.原父目标.目标.新增成员来源见证->基准材料节点,保护);
    }
    const auto d=取得服务实例物理保护(out.需求节点,保护);
    需求物理游标 dc{d.结构,{}};
    要求需求物理(dc.引用(节点类型字段)==需求节点类型&&dc.引用(需求主体字段)==out.键.服务对象
        &&dc.数值(需求执行域字段)==static_cast<std::int64_t>(新需求执行域::被认知));
    const auto sources=dc.引用组(服务取得实例来源字段);
    要求需求物理(std::binary_search(sources.begin(),sources.end(),R));
    要求需求物理(读取唯一节点字段(需求树根节点,节点类型字段)==需求树根节点类型);
    const auto count=[&](const auto& fields,稳定编码 owner,稳定编码 key,稳定编码 target) {
        return std::count_if(fields.begin(),fields.end(),[&](const auto& f){
            const auto* p=std::get_if<稳定编码>(&f.内容);
            return f.所属节点==owner&&f.字段节点==key&&p&&*p==target;});
    };
    要求需求物理(count(raw.入向字段组,需求树根节点,服务取得实例登记字段,R)==1
        &&count(raw.入向字段组,original,所属实例记录字段,R)==1
        &&count(d.结构.字段组,out.需求节点,服务取得实例来源字段,R)==1
        &&count(raw.入向字段组,out.需求节点,服务取得实例来源字段,R)==1
        &&count(d.入向字段组,R,实例需求字段,out.需求节点)==1
        &&count(d.入向字段组,需求树根节点,被认知需求登记字段,out.需求节点)==1
        &&count(d.结构.字段组,out.需求节点,节点类型字段,需求节点类型)==1
        &&count(d.结构.字段组,out.需求节点,需求主体字段,out.键.服务对象)==1);
    for(const auto& f:raw.入向字段组)if(f.字段节点==服务取得实例登记字段||f.字段节点==服务取得实例来源字段)
        要求需求物理((f.字段节点==服务取得实例登记字段&&f.所属节点==需求树根节点)
            ||(f.字段节点==服务取得实例来源字段&&f.所属节点==out.需求节点));
    for(const auto& f:saved.入向字段组)if(f.字段节点==实例原父目标字段)要求需求物理(f.所属节点==R);
    for(const auto& f:raw.入向字段组)if(f.字段节点==所属实例记录字段)要求需求物理(f.所属节点==original);
    return out;
}

void 核验服务取得D来源物理(const 基础节点结构& raw,std::vector<基础联合节点快照>& 保护) {
    std::set<稳定编码> seen;
    for(const auto& f:raw.字段组)if(f.字段节点==服务取得实例来源字段) {
        const auto* ref=std::get_if<稳定编码>(&f.内容);
        要求需求物理(ref&&有效(*ref)&&seen.insert(*ref).second);
        const auto record=解码服务取得实例记录物理(*ref,保护);
        要求需求物理(record.需求节点==raw.节点.编码);
    }
}

std::optional<新需求复合分解来源> 读取复合来源(稳定编码 D) {
    const auto 节点=读取唯一节点字段(D,复合分解来源字段);
    if(!节点)return {};
    const auto 结构=读取需求物理节点(*节点);需求物理游标 c{结构,{}};
    要求需求物理(c.引用(节点类型字段)==复合分解来源类型);
    新需求复合分解来源 来源;
    来源.父任务节点=*c.引用(复合父任务字段);来源.父需求节点=*c.引用(复合父需求字段);
    来源.父目标存在节点=*c.引用(目标存在字段);来源.父目标=c.目标();
    来源.结构概念节点=*c.引用(复合结构概念字段);
    来源.择一约束节点=c.引用(复合择一约束字段,false);来源.所选分支概念=c.引用(复合所选分支字段,false);
    来源.子目标内容路径=c.引用组(复合子路径字段);来源.子目标要求概念=*c.引用(复合子要求字段);c.完成();
    if(是存在归属分解(来源))
        要求需求物理(!来源.子目标内容路径.empty()&&!来源.择一约束节点&&!来源.所选分支概念);
    else 要求需求物理(来源.子目标内容路径.size()==1&&来源.父目标.引用内容定位
        &&来源.择一约束节点.has_value()==来源.所选分支概念.has_value());
    return 来源;
}
bool 部分需求目标形状合法(const 新部分服务要求目标& t) {
    for(const auto& n:{t.目标存在节点,t.目标特征类型概念节点,t.目标值域概念节点,t.目标比较关系概念节点})if(n&&!有效(*n))return false;
    if(t.目标形式&&*t.目标形式!=新需求目标形式::值&&*t.目标形式!=新需求目标形式::变化方向)return false;
    if(t.目标变化方向&&!转换目标变化方向(static_cast<std::int64_t>(*t.目标变化方向)))return false;
    if(t.目标变化方向&&(t.目标值域概念节点||t.目标比较关系概念节点))return false;
    if(t.目标形式==新需求目标形式::值&&t.目标变化方向)return false;
    if(t.目标形式==新需求目标形式::变化方向&&(t.目标值域概念节点||t.目标比较关系概念节点))return false;
    return true;
}
std::optional<新需求完整目标> 投影已知需求目标(const 新部分服务要求目标& t) {
    if(!部分需求目标形状合法(t)||!t.目标存在节点||!t.目标特征类型概念节点||!t.目标形式)return {};
    新需求目标定义 g{*t.目标特征类型概念节点,*t.目标形式,t.目标值域概念节点,t.目标比较关系概念节点,t.目标变化方向};
    if(!目标形状合法(g))return {};
    return 新需求完整目标{*t.目标存在节点,g};
}
新部分服务要求目标 转已知需求目标(const 新需求完整目标& t) {
    return {t.目标存在节点,t.目标.目标特征类型概念节点,t.目标.目标形式,t.目标.目标值域概念节点,t.目标.目标比较关系概念节点,t.目标.目标变化方向};
}
新需求治理信息 解码需求治理物理(const 需求目标读取上下文& 上下文,
    const 基础节点结构& 原始结构, bool 仅迁移旧形状 = false,
    std::vector<基础联合节点快照>* 完整保护 = nullptr,bool 来源枚举读取 = false,
    const 基础只读预期* 提议源 = nullptr) {
    const auto 结构 = 投影需求目标实例(上下文, 原始结构,完整保护,来源枚举读取,提议源);
    需求物理游标 c{结构, {}};
    要求需求物理(c.引用(节点类型字段) == 需求节点类型);
    新需求治理信息 d;
    d.节点 = 结构.节点.编码;
    const auto 主体 = c.引用(需求主体字段, !仅迁移旧形状);
    const auto 域 = c.数值(需求执行域字段, !仅迁移旧形状);
    要求需求物理(主体.has_value() == 域.has_value());
    if (主体) {
        要求需求物理(*域 == 1 || *域 == 2);
        d.需求主体存在节点 = *主体;
        d.执行域 = static_cast<新需求执行域>(*域);
    }
    const auto 理解 = c.引用(认知理解字段, false);
    const auto 确认 = c.引用(认知内容确认字段, false);
    const auto 要求 = c.引用(认知来源要求字段, false);
    const auto 首次消息 = c.引用(首次接收消息字段, false);
    const auto 首次含义 = c.引用(首次采用含义字段, false);
    const auto 首次范围 = c.内容组(首次接收范围字段);
    const auto 首次路径 = c.内容组(首次接收路径字段);
    const auto 请求消息=c.引用(行动请求消息字段,false),请求含义=c.引用(行动请求含义字段,false);
    const auto 原需求=c.引用(行动请求原需求字段,false),请求人类=c.引用(行动请求人类字段,false);
    const auto 受话者=c.引用(行动请求受话者字段,false);
    const auto 请求范围=c.内容组(行动请求范围字段),请求路径=c.内容组(行动请求路径字段);
    if(请求消息||请求含义||原需求||请求人类||受话者||!请求范围.empty()||!请求路径.empty()) {
        要求需求物理(请求消息&&请求含义&&原需求&&请求人类&&受话者&&请求范围.size()==1&&请求路径.size()==1);
        const auto* r=std::get_if<基础值>(请求范围.front());
        const auto* p=std::get_if<基础值>(请求路径.front());
        const auto* range=r?std::get_if<std::vector<std::uint64_t>>(&r->材料):nullptr;
        const auto* path=p?std::get_if<std::vector<std::int64_t>>(&p->材料):nullptr;
        要求需求物理(range&&range->size()==2&&range->front()<range->back()&&path&&!path->empty()
            &&std::all_of(path->begin(),path->end(),[](auto n){return n>=0;}));
        d.行动请求来源=新行动请求承接来源{{{*请求消息,range->front(),range->back(),*path},*请求含义},*原需求,*请求人类,*受话者};
    }
    if (首次消息 || 首次含义 || !首次范围.empty() || !首次路径.empty()) {
        要求需求物理(首次消息 && 首次含义 && 首次范围.size()==1 && 首次路径.size()==1);
        const auto* 范围材料=std::get_if<基础值>(首次范围.front());
        const auto* 路径材料=std::get_if<基础值>(首次路径.front());
        const auto* 范围 = 范围材料 ? std::get_if<std::vector<std::uint64_t>>(&范围材料->材料) : nullptr;
        const auto* 路径 = 路径材料 ? std::get_if<std::vector<std::int64_t>>(&路径材料->材料) : nullptr;
        要求需求物理(范围&&范围->size()==2&&范围->front()<范围->back()&&路径
            &&(路径->empty() ? 范围->front()==0
                : std::all_of(路径->begin(),路径->end(),[](auto n){return n>=0;})));
        d.首次接收认知来源=新接收认知来源{{*首次消息,范围->front(),范围->back(),*路径},*首次含义};
    }
    const auto 发布场景 = c.引用(集合发布场景字段, false);
    const auto 发布动作 = c.引用(集合发布动作字段, false);
    const auto 首状态 = c.引用(需求首次发布首状态字段, false);
    要求需求物理(!首状态 || (发布场景 && 发布动作));
    if (首状态) (void)读取需求物理节点(*首状态);
    要求需求物理(发布场景.has_value() == 发布动作.has_value());
    if (发布场景) {
        要求需求物理(d.执行域 == 新需求执行域::被认知);
        (void)读取需求物理节点(*发布场景);
        (void)读取需求物理节点(*发布动作);
    }
    if (理解 || 确认 || 要求) {
        要求需求物理(理解 && 确认);
        d.认知来源 = 新需求认知来源{*理解, *确认, 要求};
    }
    const auto 阶段=c.数值(需求信息完善阶段字段,false);
    const auto 目标存在 = c.引用(目标存在字段, false);
    if(阶段) {
        要求需求物理(d.执行域==新需求执行域::被认知&&(*阶段==1||*阶段==2));
        d.信息完善阶段=static_cast<新需求信息完善阶段>(*阶段);
        新部分服务要求目标 t;t.目标存在节点=目标存在;t.目标特征类型概念节点=c.引用(目标特征类型字段,false);
        if(const auto f=c.数值(目标形式字段,false)){t.目标形式=转换目标形式(*f);要求需求物理(t.目标形式.has_value());}
        t.目标值域概念节点=c.引用(目标值域字段,false);t.目标比较关系概念节点=c.引用(目标比较关系字段,false);
        if(const auto f=c.数值(目标变化方向字段,false)){t.目标变化方向=转换目标变化方向(*f);要求需求物理(t.目标变化方向.has_value());}
        要求需求物理(部分需求目标形状合法(t));d.已知目标=t;d.完整目标=投影已知需求目标(t);
    } else if (目标存在) d.完整目标 = 新需求完整目标{*目标存在, c.目标()};
    const auto 活动 = *c.数值(活动状态字段); d.权重 = *c.数值(需求权重字段);
    要求需求物理(是活动状态值(活动) && d.权重 >= 0);
    d.活动状态 = static_cast<新需求活动状态>(活动);
    const auto 根 = c.数值(根角色字段, false), 来源 = c.数值(来源类型字段, false);
    d.来源承接节点 = c.引用(来源承接字段, false);
    d.服务要求确认组 = c.引用组(服务要求确认字段);
    d.服务需求取得执行节点 = c.引用(服务需求取得执行字段, false);
    d.外部要求节点 = c.引用(外部服务要求字段, false);
    const auto 复合=c.引用(复合分解来源字段,false);
    if(复合)d.复合来源=读取复合来源(d.节点);
    const auto 实例来源=c.引用组(服务取得实例来源字段);
    if(!实例来源.empty()) {
        要求需求物理(d.执行域==新需求执行域::被认知);
        std::vector<基础联合节点快照> 保护;
        核验服务取得D来源物理(原始结构,保护);
        if(!需求内容保护未变(保护))throw 新需求目标定义核验状态::前提已变化;
        if(完整保护&&!并需求保护(*完整保护,保护))throw 新需求目标定义核验状态::前提已变化;
    }
    if (d.服务需求取得执行节点) {
        要求需求物理(!d.服务要求确认组.empty());
        (void)读取需求物理节点(*d.服务需求取得执行节点);
    }
    c.完成();
    const auto 父 = 查询父需求关系(d.节点);
    if (d.执行域 == 新需求执行域::被认知) {
        要求需求物理(!d.行动请求来源&&!d.复合来源&&(!d.完整目标||(!d.完整目标->目标.引用内容定位&&!d.完整目标->目标.新增成员来源见证)));
        要求需求物理(!(d.首次接收认知来源&&d.认知来源));
        if(d.首次接收认知来源)要求需求物理(阶段&&d.完整目标);
        if(!阶段){要求需求物理(d.完整目标&&d.认知来源);d.信息完善阶段=新需求信息完善阶段::已完善;d.已知目标=转已知需求目标(*d.完整目标);}
        if(d.信息完善阶段==新需求信息完善阶段::已完善)要求需求物理(d.完整目标&&(d.认知来源||d.首次接收认知来源));
        else 要求需求物理(!d.认知来源);
        要求需求物理(!根 && !来源 && !d.来源承接节点
            && !d.外部要求节点 && d.服务要求确认组.empty() && !d.服务需求取得执行节点
            && d.权重 == 0 && 父.empty() && 查询子需求关系(d.节点).empty());
        return d;
    }
    要求需求物理(!d.认知来源&&!d.首次接收认知来源);
    if (根) {
        d.根角色 = 转换根角色(*根);
        要求需求物理(d.根角色 && d.完整目标 && !d.外部要求节点 && !来源 && !d.来源承接节点
            && d.服务要求确认组.empty() && !d.服务需求取得执行节点 && 父.empty());
        要求需求物理(d.节点 == (*d.根角色 == 新需求根角色::服务 ? 服务根需求节点 : 安全根需求节点));
    } else {
        要求需求物理(来源.has_value()); d.来源类型 = 转换来源类型(*来源);
        要求需求物理(d.来源类型 && 父.size() == 1 && 父.front().目标节点 == d.节点
            && 有效(父.front().源节点) && 父.front().源节点 != d.节点);
        d.父需求节点 = 父.front().源节点;
        要求需求物理((*d.来源类型 == 新需求来源类型::任务子目标) == d.来源承接节点.has_value());
        if (d.来源类型 == 新需求来源类型::外部服务要求)
            要求需求物理(d.外部要求节点
                && d.完整目标 && d.服务要求确认组.size() == 1);
        else 要求需求物理(d.完整目标 && !d.外部要求节点);
        要求需求物理(d.服务要求确认组.empty() || d.来源类型 != 新需求来源类型::本能定义);
    }
    要求需求物理((d.来源类型==新需求来源类型::行动请求)==d.行动请求来源.has_value());
    if(d.完整目标&&d.完整目标->目标.新增成员来源见证)
        要求需求物理(!d.根角色&&!d.行动请求来源&&!d.复合来源&&!d.外部要求节点
            &&d.服务要求确认组.empty()&&!d.服务需求取得执行节点
            &&d.来源类型==新需求来源类型::任务子目标&&d.来源承接节点&&d.父需求节点);
    if(d.行动请求来源)要求需求物理(!d.根角色&&d.执行域==新需求执行域::自我执行
        &&d.需求主体存在节点==d.行动请求来源->受请求者节点&&d.完整目标&&!阶段
        &&!d.外部要求节点&&d.服务要求确认组.empty()&&!d.服务需求取得执行节点
        &&!d.来源承接节点&&!d.复合来源&&!d.完整目标->目标.引用内容定位);
    if(d.完整目标&&d.完整目标->目标.引用内容定位) {
        要求需求物理(!d.根角色&&!d.外部要求节点&&d.服务要求确认组.empty()&&!d.服务需求取得执行节点);
    }
    if(d.复合来源&&是存在归属分解(*d.复合来源)) {
        const auto& s=*d.复合来源;
        要求需求物理(d.完整目标&&!d.完整目标->目标.引用内容定位
            &&d.来源类型==新需求来源类型::任务子目标&&d.父需求节点==s.父需求节点
            &&d.来源承接节点==s.父任务节点&&d.完整目标->目标.目标形式==新需求目标形式::值
            &&!s.子目标内容路径.empty()&&d.完整目标->目标.目标特征类型概念节点==s.子目标内容路径.back()
            &&d.完整目标->目标.目标值域概念节点==s.子目标要求概念);
    } else if(d.复合来源) {
        const auto& s=*d.复合来源;
        要求需求物理(d.完整目标&&d.完整目标->目标.引用内容定位
            &&d.来源类型==新需求来源类型::任务子目标&&d.父需求节点==s.父需求节点
            &&d.来源承接节点==s.父任务节点&&s.父目标.引用内容定位
            &&d.完整目标->目标存在节点==s.父目标存在节点
            &&d.完整目标->目标.引用内容定位->引用需求节点==s.父目标.引用内容定位->引用需求节点
            &&d.完整目标->目标.引用内容定位->正式内容路径==s.子目标内容路径
            &&d.完整目标->目标.目标值域概念节点==s.子目标要求概念
            &&s.结构概念节点==s.父目标.目标值域概念节点);
    }
    return d;
}
新需求信息 投影完整需求(const 新需求治理信息& d) {
    要求需求物理(d.完整目标.has_value());
    return {d.节点, d.根角色, d.父需求节点, d.完整目标->目标存在节点,
        d.完整目标->目标, d.来源类型, d.来源承接节点, d.权重, d.活动状态,
        d.服务要求确认组, d.服务需求取得执行节点,
        d.需求主体存在节点, d.执行域, d.认知来源,d.复合来源,d.首次接收认知来源,d.行动请求来源};
}
新需求信息 解码需求物理(const 需求目标读取上下文& 上下文, const 基础节点结构& 结构) {
    return 投影完整需求(解码需求治理物理(上下文, 结构));
}
新需求治理信息 读取需求治理物理(const 需求目标读取上下文& 上下文, 稳定编码 节点) {
    return 解码需求治理物理(上下文, 读取需求物理节点(节点));
}
新需求信息 读取需求完整物理(const 需求目标读取上下文& 上下文, 稳定编码 节点) {
    return 解码需求物理(上下文, 读取需求物理节点(节点));
}
新需求列表项信息 解码需求列表物理(const 基础节点结构& 结构) {
    需求物理游标 c{结构, {}};
    要求需求物理(c.引用(节点类型字段) == 需求列表项节点类型);
    新需求列表项信息 l;
    l.节点 = 结构.节点.编码; l.目标存在节点 = *c.引用(目标存在字段); l.目标 = c.目标();
    const auto 根 = c.数值(根角色字段, false);
    if (根) { l.根角色 = 转换根角色(*根); 要求需求物理(l.根角色.has_value()); }
    l.需求节点组 = c.引用组(列表成员字段); c.完成();
    要求需求物理(!l.需求节点组.empty() && (!根 || l.需求节点组.size() == 1)); return l;
}
std::vector<稳定编码> 读取需求根登记物理() {
    std::vector<基础联合节点快照> 保护;
    const auto 根快照=取得服务实例物理保护(需求树根节点,保护);
    需求物理游标 c{根快照.结构, {}};
    要求需求物理(c.引用(节点类型字段) == 需求树根节点类型);
    要求需求物理(有效(安全根需求节点) && 有效(服务根需求节点) && 安全根需求节点 != 服务根需求节点);
    要求需求物理(c.引用(安全根登记字段) == 安全根需求节点
        && c.引用(服务根登记字段) == 服务根需求节点);
    auto 列表 = c.引用组(需求列表项字段);
    (void)c.引用组(被认知需求登记字段);
    (void)c.引用组(新增成员见证登记字段);
    const auto 实例组=c.引用组(服务取得实例登记字段);
    要求需求物理(c.数值(需求归属格式字段) == 1);
    c.完成();
    std::set<稳定编码> 类型全集;
    for(const auto& f:全局基础数据集.查询目标字段(服务取得实例记录类型,节点类型字段))
        要求需求物理(类型全集.insert(f.所属节点).second);
    要求需求物理(std::vector<稳定编码>(类型全集.begin(),类型全集.end())==实例组);
    std::set<std::tuple<稳定编码,稳定编码,std::vector<稳定编码>,稳定编码>> 实例键组;
    std::set<稳定编码> 快照组;
    for(const auto R:实例组) {
        const auto i=解码服务取得实例记录物理(R,保护);
        要求需求物理(实例键组.emplace(i.键.父取得任务,i.键.目标槽类型,i.键.正式根路径,i.键.服务对象).second);
        const auto saved=读取唯一节点字段(R,实例原父目标字段);要求需求物理(saved&&快照组.insert(*saved).second);
    }
    std::set<稳定编码> 快照类型全集;
    for(const auto& f:全局基础数据集.查询目标字段(实例原父目标类型,节点类型字段))
        要求需求物理(快照类型全集.insert(f.所属节点).second);
    要求需求物理(快照类型全集==快照组);
    // 枚举范围由本函数自身的根快照负责，不再隐含借用局部R解码的保护。
    if(!实例组.empty()) {
        要求需求物理(!保护.empty());
        if(!需求内容保护未变(保护))throw 新需求目标定义核验状态::前提已变化;
    }
    return 列表;
}
稳定编码 核验需求列表物理(const 需求目标读取上下文& 上下文, const 新需求信息& d) {
    const auto 反查 = 全局基础数据集.查询字段引用节点结构(d.节点, 列表成员字段);
    if (反查.状态 != 基础结构操作状态::已完成) throw 需求物理读取失败{反查.状态};
    要求需求物理(反查.引用字段组.size() == 1 && 反查.来源节点组.size() == 1);
    const auto l = 解码需求列表物理(反查.来源节点组.front());
    const auto& f = 反查.引用字段组.front();
    要求需求物理(f.所属节点 == l.节点 && f.字段节点 == 列表成员字段
        && std::get_if<稳定编码>(&f.内容) && std::get<稳定编码>(f.内容) == d.节点);
    要求需求物理(l.目标存在节点 == d.目标存在节点 && l.目标 == d.目标 && l.根角色 == d.根角色
        && std::binary_search(l.需求节点组.begin(), l.需求节点组.end(), d.节点));
    const auto 登记 = 读取需求根登记物理();
    要求需求物理(std::binary_search(登记.begin(), 登记.end(), l.节点));
    const auto 回指 = 全局基础数据集.查询字段引用节点结构(l.节点, 需求列表项字段);
    if (回指.状态 != 基础结构操作状态::已完成) throw 需求物理读取失败{回指.状态};
    要求需求物理(回指.引用字段组.size() == 1
        && 回指.引用字段组.front().所属节点 == 需求树根节点);
    for (const auto 成员 : l.需求节点组) {
        const auto m = 成员 == d.节点 ? d : 读取需求完整物理(上下文, 成员);
        要求需求物理(m.目标存在节点 == l.目标存在节点 && m.目标 == l.目标 && m.根角色 == l.根角色);
    }
    return l.节点;
}

void 回滚字段(const std::vector<稳定编码>& 字段组) noexcept {
    for (auto 位置 = 字段组.rbegin(); 位置 != 字段组.rend(); ++位置) {
        全局基础数据集.删除字段(*位置);
    }
}

struct 已建根材料 final {
    稳定编码 需求;
    稳定编码 列表项;
    std::vector<稳定编码> 需求字段;
    std::vector<稳定编码> 列表字段;
};

void 回滚根材料(已建根材料& 材料) noexcept {
    回滚字段(材料.列表字段);
    if (有效(材料.列表项)) 全局基础数据集.删除节点(材料.列表项);
    回滚字段(材料.需求字段);
    if (有效(材料.需求)) 全局基础数据集.删除节点(材料.需求);
}

std::optional<已建根材料> 建立根材料(
    新需求根角色 角色,
    const 新根需求定义& 定义) {
    已建根材料 结果;
    结果.需求 = 全局基础数据集.新建节点();
    if (!有效(结果.需求)) return std::nullopt;

    auto 加节点字段 = [&](稳定编码 字段, 稳定编码 目标) {
        const auto 关系 = 全局基础数据集.添加字段节点(结果.需求, 字段, 目标);
        if (有效(关系)) 结果.需求字段.push_back(关系);
        return 有效(关系);
    };
    auto 加I64字段 = [&](稳定编码 字段, std::int64_t 值) {
        const auto 关系 = 全局基础数据集.添加字段值(
            结果.需求, 字段, 基础值{值});
        if (有效(关系)) 结果.需求字段.push_back(关系);
        return 有效(关系);
    };
    if (!加节点字段(节点类型字段, 需求节点类型)
        || !加节点字段(需求主体字段, 定义.目标存在节点)
        || !加I64字段(需求执行域字段, static_cast<std::int64_t>(新需求执行域::自我执行))
        || !加I64字段(根角色字段, static_cast<std::int64_t>(角色))
        || !加节点字段(目标存在字段, 定义.目标存在节点)
        || !加节点字段(目标特征类型字段,
            定义.目标.目标特征类型概念节点)
        || !加I64字段(目标形式字段,
            static_cast<std::int64_t>(定义.目标.目标形式))
        || !加I64字段(需求权重字段, 定义.初始权重)
        || !加I64字段(活动状态字段,
            static_cast<std::int64_t>(新需求活动状态::活动))) {
        回滚根材料(结果);
        return std::nullopt;
    }
    if (定义.目标.目标值域概念节点
        && !加节点字段(目标值域字段,
            *定义.目标.目标值域概念节点)) {
        回滚根材料(结果);
        return std::nullopt;
    }
    if (定义.目标.目标比较关系概念节点
        && !加节点字段(目标比较关系字段,
            *定义.目标.目标比较关系概念节点)) {
        回滚根材料(结果);
        return std::nullopt;
    }
    if (定义.目标.目标变化方向
        && !加I64字段(目标变化方向字段,
            static_cast<std::int64_t>(*定义.目标.目标变化方向))) {
        回滚根材料(结果);
        return std::nullopt;
    }

    结果.列表项 = 全局基础数据集.新建节点();
    if (!有效(结果.列表项)) {
        回滚根材料(结果);
        return std::nullopt;
    }
    auto 加列表节点字段 = [&](稳定编码 字段, 稳定编码 目标) {
        const auto 关系 = 全局基础数据集.添加字段节点(
            结果.列表项, 字段, 目标);
        if (有效(关系)) 结果.列表字段.push_back(关系);
        return 有效(关系);
    };
    auto 加列表I64字段 = [&](稳定编码 字段, std::int64_t 值) {
        const auto 关系 = 全局基础数据集.添加字段值(
            结果.列表项, 字段, 基础值{值});
        if (有效(关系)) 结果.列表字段.push_back(关系);
        return 有效(关系);
    };
    if (!加列表节点字段(节点类型字段, 需求列表项节点类型)
        || !加列表I64字段(根角色字段, static_cast<std::int64_t>(角色))
        || !加列表节点字段(目标存在字段, 定义.目标存在节点)
        || !加列表节点字段(目标特征类型字段,
            定义.目标.目标特征类型概念节点)
        || !加列表I64字段(目标形式字段,
            static_cast<std::int64_t>(定义.目标.目标形式))
        || !加列表节点字段(列表成员字段, 结果.需求)) {
        回滚根材料(结果);
        return std::nullopt;
    }
    if (定义.目标.目标值域概念节点
        && !加列表节点字段(目标值域字段,
            *定义.目标.目标值域概念节点)) {
        回滚根材料(结果);
        return std::nullopt;
    }
    if (定义.目标.目标比较关系概念节点
        && !加列表节点字段(目标比较关系字段,
            *定义.目标.目标比较关系概念节点)) {
        回滚根材料(结果);
        return std::nullopt;
    }
    if (定义.目标.目标变化方向
        && !加列表I64字段(目标变化方向字段,
            static_cast<std::int64_t>(*定义.目标.目标变化方向))) {
        回滚根材料(结果);
        return std::nullopt;
    }
    return 结果;
}

std::optional<稳定编码> 取得特征类型根(
    稳定编码 概念节点, const 概念_特征类& 服务) {
    const auto 关系 = 服务.比较单特征概念(概念节点, 概念节点);
    if (关系.状态 != 特征概念规则状态::已完成 || !关系.特征类型根) {
        return std::nullopt;
    }
    return 关系.特征类型根;
}

bool 比较关系适用于目标(
    const 二次特征概念信息& 比较关系,
    稳定编码 特征类型根,
    稳定编码 目标值域,
    const 概念_特征类& 特征概念服务) {
    return 比较关系适用纯判据(比较关系,特征类型根,目标值域,[&](稳定编码 C) {
        return 取得特征类型根(C,特征概念服务);
    });
}

enum class 准确值目标位置 : std::uint8_t {
    低于 = 1,
    命中 = 2,
    高于 = 3,
    域外但无单一方向 = 4
};

struct 准确值定位结果 final {
    bool 结构有效 = false;
    准确值目标位置 位置 = 准确值目标位置::域外但无单一方向;
    std::optional<std::int64_t> 准确差异;
    // CONCEPT-INFO-MEMBERSHIP R-1：存在引用值的归属无法判定（认知不足），位置无效。
    bool 认知不足 = false, 前提已变化 = false;
    std::vector<存在概念认知缺口> 认知缺口组;
};

std::int64_t 饱和差(std::int64_t 目标, std::int64_t 当前) noexcept {
    const auto 最大 = (std::numeric_limits<std::int64_t>::max)();
    const auto 最小 = (std::numeric_limits<std::int64_t>::min)();
    if (当前 < 0 && 目标 > 最大 + 当前) {
        return (std::numeric_limits<std::int64_t>::max)();
    }
    if (当前 > 0 && 目标 < 最小 + 当前) {
        return (std::numeric_limits<std::int64_t>::min)();
    }
    return 目标 - 当前;
}

bool I64命中区间(std::int64_t 值,
    const std::vector<特征概念I64闭区间>& 区间组) noexcept {
    return std::any_of(区间组.begin(), 区间组.end(), [&](const auto& 区间) {
        return 区间.下界 <= 值 && 值 <= 区间.上界;
    });
}

准确值定位结果 定位准确值(
    const 新特征准确值& 当前值,
    const 特征概念信息& 目标概念,
    const 新_特征值类& 特征值服务,
    const 新_存在类& 存在服务) {
    准确值定位结果 结果;
    if (目标概念.材料类型 == 特征概念材料物理类型::I64标量) {
        const auto* 当前I64 = std::get_if<std::int64_t>(&当前值);
        const auto* 区间组 = std::get_if<std::vector<特征概念I64闭区间>>(
            &目标概念.值域);
        if (!当前I64 || !区间组 || 区间组->empty()) return 结果;
        结果.结构有效 = true;
        if (I64命中区间(*当前I64, *区间组)) {
            结果.位置 = 准确值目标位置::命中;
            结果.准确差异 = 0;
        } else if (*当前I64 < 区间组->front().下界) {
            结果.位置 = 准确值目标位置::低于;
            结果.准确差异 = 饱和差(区间组->front().下界, *当前I64);
        } else if (*当前I64 > 区间组->back().上界) {
            结果.位置 = 准确值目标位置::高于;
            结果.准确差异 = 饱和差(区间组->back().上界, *当前I64);
        } else {
            结果.位置 = 准确值目标位置::域外但无单一方向;
        }
        return 结果;
    }

    const auto* 当前节点 = std::get_if<稳定编码>(&当前值);
    if (!当前节点 || !有效(*当前节点)) return 结果;
    if (目标概念.材料类型 == 特征概念材料物理类型::存在节点引用) {
        const auto* 引用域 = std::get_if<特征概念存在引用值域>(
            &目标概念.值域);
        if (!引用域 || !存在服务.是存在节点(*当前节点)
            || !存在服务.是存在概念节点(引用域->存在概念节点)) {
            return 结果;
        }
        // CONCEPT-INFO-MEMBERSHIP：属于为命中，不属于为域外；未知是认知不足，不是未满足。
        const auto 判定 = 存在服务.比较存在与概念信息(*当前节点, 引用域->存在概念节点);
        if (判定.状态 == 特征概念规则状态::资源失败) throw std::bad_alloc{};
        if (判定.状态 == 特征概念规则状态::前提已变化) { 结果.结构有效 = true; 结果.前提已变化 = true; return 结果; }
        if (判定.状态 != 特征概念规则状态::已完成) return 结果;
        结果.结构有效 = true;
        if (判定.归属 == 特征成员概念归属::未知) {
            结果.认知不足 = true;
            结果.认知缺口组 = 判定.认知缺口组;
            return 结果;
        }
        结果.位置 = 判定.归属 == 特征成员概念归属::属于
            ? 准确值目标位置::命中 : 准确值目标位置::域外但无单一方向;
        return 结果;
    }
    const auto 当前信息 = 特征值服务.获取特征值(*当前节点);
    if (!当前信息) return 结果;

    if (std::holds_alternative<特征概念全材料值域>(目标概念.值域)) {
        结果.结构有效 = true;
        结果.位置 = 准确值目标位置::命中;
        return 结果;
    }
    if (const auto* 枚举 = std::get_if<std::vector<稳定编码>>(&目标概念.值域)) {
        结果.结构有效 = true;
        结果.位置 = std::binary_search(枚举->begin(), 枚举->end(), *当前节点)
            ? 准确值目标位置::命中 : 准确值目标位置::域外但无单一方向;
        return 结果;
    }
    const auto* 结构域 = std::get_if<特征概念结构化I64值域>(&目标概念.值域);
    const auto* 当前分量 = std::get_if<std::vector<std::int64_t>>(&当前信息->材料);
    if (!结构域 || !当前分量 || 当前分量->size() != 结构域->分量.size()) {
        return 结果;
    }
    结果.结构有效 = true;
    结果.位置 = 准确值目标位置::命中;
    for (std::size_t i = 0; i < 当前分量->size(); ++i) {
        if (!I64命中区间((*当前分量)[i], 结构域->分量[i].值域)) {
            结果.位置 = 准确值目标位置::域外但无单一方向;
            break;
        }
    }
    return 结果;
}

bool 关系接受位置(
    二次特征结果域种类 关系, 准确值目标位置 位置) noexcept {
    switch (关系) {
    case 二次特征结果域种类::任意:
        return true;
    case 二次特征结果域种类::小于:
    case 二次特征结果域种类::低于参照值域:
        return 位置 == 准确值目标位置::低于;
    case 二次特征结果域种类::小于等于:
        return 位置 == 准确值目标位置::低于
            || 位置 == 准确值目标位置::命中;
    case 二次特征结果域种类::等于:
    case 二次特征结果域种类::包含于参照值域:
        return 位置 == 准确值目标位置::命中;
    case 二次特征结果域种类::大于等于:
        return 位置 == 准确值目标位置::高于
            || 位置 == 准确值目标位置::命中;
    case 二次特征结果域种类::大于:
    case 二次特征结果域种类::高于参照值域:
        return 位置 == 准确值目标位置::高于;
    case 二次特征结果域种类::不包含于参照值域:
        return 位置 != 准确值目标位置::命中;
    }
    return false;
}

enum class 所需方向 : std::uint8_t { 无 = 0, 增加 = 1, 减少 = 2, 冲突 = 3 };

所需方向 关系所需方向(
    二次特征结果域种类 关系, 准确值目标位置 位置) noexcept {
    if (关系接受位置(关系, 位置)) return 所需方向::无;
    switch (关系) {
    case 二次特征结果域种类::小于:
    case 二次特征结果域种类::小于等于:
    case 二次特征结果域种类::低于参照值域:
        return 所需方向::减少;
    case 二次特征结果域种类::等于:
    case 二次特征结果域种类::包含于参照值域:
        if (位置 == 准确值目标位置::低于) return 所需方向::增加;
        if (位置 == 准确值目标位置::高于) return 所需方向::减少;
        return 所需方向::冲突;
    case 二次特征结果域种类::大于:
    case 二次特征结果域种类::大于等于:
    case 二次特征结果域种类::高于参照值域:
        return 所需方向::增加;
    case 二次特征结果域种类::任意:
        return 所需方向::无;
    case 二次特征结果域种类::不包含于参照值域:
        return 所需方向::冲突;
    }
    return 所需方向::冲突;
}

所需方向 合并方向(所需方向 左, 所需方向 右) noexcept {
    if (左 == 所需方向::无) return 右;
    if (右 == 所需方向::无 || 左 == 右) return 左;
    return 所需方向::冲突;
}

struct 需求权重修改项 final {
    稳定编码 字段关系;
    std::int64_t 原权重;
    std::int64_t 新权重;
};

enum class 需求权重写入状态 : std::uint8_t {
    成功 = 1,
    资源失败 = 2,
    回滚失败 = 3
};

bool 追加需求权重修改(
    稳定编码 需求节点,
    std::int64_t 新权重,
    std::vector<需求权重修改项>& 修改组) {
    if (新权重 < 0) return false;
    const auto 字段组 = 全局基础数据集.查询字段(需求节点, 需求权重字段);
    if (字段组.size() != 1) return false;
    const auto* 值 = std::get_if<基础值>(&字段组.front().内容);
    if (!值) return false;
    const auto* 原权重 = std::get_if<std::int64_t>(&值->材料);
    if (!原权重 || *原权重 < 0) return false;
    if (*原权重 != 新权重) {
        修改组.push_back({字段组.front().编码, *原权重, 新权重});
    }
    return true;
}

bool 构建子树权重修改(
    const 需求目标读取上下文& 上下文,
    稳定编码 父需求节点,
    std::int64_t 父权重,
    const std::optional<std::pair<稳定编码, 新需求活动状态>>& 活动状态覆盖,
    std::set<稳定编码>& 已访问,
    std::vector<需求权重修改项>& 修改组) {
    auto 子关系组 = 查询子需求关系(父需求节点);
    std::sort(子关系组.begin(), 子关系组.end(), [](const auto& 左, const auto& 右) {
        return 左.目标节点 < 右.目标节点;
    });
    if (std::adjacent_find(子关系组.begin(), 子关系组.end(),
            [](const auto& 左, const auto& 右) {
                return 左.目标节点 == 右.目标节点;
            }) != 子关系组.end()) {
        return false;
    }

    struct 子需求材料 final {
        稳定编码 节点;
        新需求活动状态 活动状态;
        std::int64_t 新权重 = 0;
    };
    std::vector<子需求材料> 子需求组;
    子需求组.reserve(子关系组.size());
    std::size_t 参与数量 = 0;
    for (const auto& 关系 : 子关系组) {
        if (关系.源节点 != 父需求节点 || !已访问.insert(关系.目标节点).second) {
            return false;
        }
        const auto 子需求 = std::optional{读取需求治理物理(上下文, 关系.目标节点)};
        if (子需求->根角色 || !子需求->父需求节点
            || *子需求->父需求节点 != 父需求节点) {
            return false;
        }
        auto 活动状态 = 子需求->活动状态;
        if (活动状态覆盖 && 活动状态覆盖->first == 关系.目标节点) {
            活动状态 = 活动状态覆盖->second;
        }
        if (活动状态 != 新需求活动状态::失活) ++参与数量;
        子需求组.push_back({关系.目标节点, 活动状态, 0});
    }

    const auto 基础份额 = 参与数量 == 0
        ? 0 : 父权重 / static_cast<std::int64_t>(参与数量);
    auto 余数 = 参与数量 == 0
        ? 0 : 父权重 % static_cast<std::int64_t>(参与数量);
    for (auto& 子需求 : 子需求组) {
        if (子需求.活动状态 != 新需求活动状态::失活) {
            子需求.新权重 = 基础份额;
            if (余数 > 0) {
                ++子需求.新权重;
                --余数;
            }
        }
        if (!追加需求权重修改(子需求.节点, 子需求.新权重, 修改组)
            || !构建子树权重修改(上下文, 子需求.节点, 子需求.新权重,
                活动状态覆盖, 已访问, 修改组)) {
            return false;
        }
    }
    return true;
}

需求权重写入状态 应用需求权重修改(
    const std::vector<需求权重修改项>& 修改组) noexcept {
    std::size_t 已应用数量 = 0;
    for (; 已应用数量 < 修改组.size(); ++已应用数量) {
        const auto& 修改 = 修改组[已应用数量];
        if (!全局基础数据集.修改字段值(
                修改.字段关系, 基础值{修改.新权重})) {
            bool 回滚成功 = true;
            while (已应用数量 > 0) {
                --已应用数量;
                const auto& 已应用 = 修改组[已应用数量];
                if (!全局基础数据集.修改字段值(
                        已应用.字段关系, 基础值{已应用.原权重})) {
                    回滚成功 = false;
                }
            }
            return 回滚成功
                ? 需求权重写入状态::资源失败
                : 需求权重写入状态::回滚失败;
        }
    }
    return 需求权重写入状态::成功;
}

} // namespace

新_需求类::新_需求类(
    新_特征值类& 特征值服务,
    新_存在类& 存在服务,
    新_特征类& 特征服务,
    概念_特征类& 特征概念服务,
    概念_二次特征类& 二次特征概念服务,
    概念_存在类& 存在概念服务) noexcept
    : 特征值服务_(特征值服务),
      存在服务_(存在服务),
      特征服务_(特征服务),
      特征概念服务_(特征概念服务),
      二次特征概念服务_(二次特征概念服务),
      存在概念服务_(存在概念服务) {}

bool 新_需求类::绑定关联检查通知(
    const void* 所有者, std::function<void()> 通知) noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        if (!所有者 || !通知) return false;
        if (关联检查通知所有者_) return 关联检查通知所有者_ == 所有者;
        关联检查通知_ = std::move(通知);
        关联检查通知所有者_ = 所有者;
        return true;
    } catch (...) { return false; }
}

void 新_需求类::解除关联检查通知(const void* 所有者) noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        if (!所有者 || 关联检查通知所有者_ != 所有者) return;
        关联检查通知_ = {};
        关联检查通知所有者_ = nullptr;
    } catch (...) {}
}

bool 新_需求类::初始化() noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        if (结构已初始化()) return 结构仍存在();

        std::vector<稳定编码*> 待建{
            &节点类型字段, &需求树根节点类型, &需求节点类型,
            &需求列表项节点类型, &目标存在字段,
            &目标特征类型字段, &目标形式字段,
            &目标值域字段, &目标比较关系字段, &目标变化方向字段, &需求目标承载格式字段,
            &来源类型字段, &来源承接字段, &服务要求确认字段, &服务需求取得执行字段, &外部服务要求字段, &活动状态字段, &需求权重字段, &根角色字段,
            &需求列表项字段, &列表成员字段,
            &安全根登记字段, &服务根登记字段, &需求树根节点,
            &需求主体字段, &需求执行域字段, &认知理解字段, &认知内容确认字段,
            &认知来源要求字段, &被认知需求登记字段, &需求归属格式字段,
            &首次接收消息字段, &首次接收范围字段, &首次接收路径字段, &首次采用含义字段,
            &行动请求消息字段, &行动请求含义字段, &行动请求原需求字段, &行动请求人类字段,
            &行动请求受话者字段, &行动请求范围字段, &行动请求路径字段,
            &集合发布场景字段, &集合发布动作字段, &需求首次发布首状态字段, &需求信息完善阶段字段,
            &需求发生关联类型,&发生场景字段,&发生主体字段,&发生特征字段,&发生需求字段,
            &发生动作字段,&发生来源字段,&发生状态字段,&发生方式字段,
            &需求迁移记录类型, &需求迁移来源项类型, &需求迁移登记字段,
            &迁移场景字段, &迁移主体字段, &迁移原特征字段, &迁移新特征字段, &迁移当前需求字段, &迁移当前状态字段,
            &迁移来源项字段, &迁移项需求字段, &迁移项动作字段, &迁移项前状态字段, &迁移项后状态字段, &迁移项序号字段,
            &目标新增成员见证字段,&新增成员见证登记字段,&新增成员见证类型,
            &新增成员格式字段,&新增成员来源种类字段,&新增成员源类型字段,
            &新增成员基准字段,&新增成员指定主体字段,&新增成员指定需求字段,
            &引用内容需求字段,&引用内容路径字段,&复合分解来源字段,&复合分解来源类型,
            &复合父任务字段,&复合父需求字段,&复合结构概念字段,&复合择一约束字段,&复合所选分支字段,&复合子路径字段,&复合子要求字段,
            &服务取得实例登记字段,&服务取得实例来源字段,&服务取得实例记录类型,&实例原父目标类型,
            &实例格式字段,&实例父任务字段,&实例目标槽字段,&实例根路径字段,&实例主体字段,&实例父需求字段,
            &实例原父目标字段,&实例人类概念字段,&实例需求身份概念字段,&实例完整内容概念字段,&实例需求字段,&实例复用指定字段,&所属实例记录字段};
        std::vector<稳定编码*> 已建;
        for (auto* 身份 : 待建) {
            *身份 = 全局基础数据集.新建节点();
            if (!有效(*身份)) {
                for (auto 位置 = 已建.rbegin(); 位置 != 已建.rend(); ++位置) {
                    全局基础数据集.删除节点(**位置);
                    **位置 = {};
                }
                return false;
            }
            已建.push_back(身份);
        }
        const auto 根类型字段 = 全局基础数据集.添加字段节点(
            需求树根节点, 节点类型字段, 需求树根节点类型);
        if (!有效(根类型字段)) {
            for (auto 位置 = 已建.rbegin(); 位置 != 已建.rend(); ++位置) {
                全局基础数据集.删除节点(**位置);
                **位置 = {};
            }
            return false;
        }
        return true;
    } catch (...) {
        return false;
    }
}

新需求操作状态 新_需求类::确保目标特征实例承载内部(稳定编码 D) noexcept {
    try { return 确保目标特征实例承载可抛内部(D); }
    catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"需求目标实例迁移", L"五字段迁移工程故障；不回滚已挂接需求。"); }
}

新需求操作状态 新_需求类::确保目标特征实例承载可抛内部(稳定编码 D) {
    using S = 新需求操作状态;
    std::vector<稳定编码> 私有F;
    bool 已挂接 = false;
    const auto 清理 = [&]() noexcept {
        if (已挂接 || 私有F.empty()) return;
        auto p = 特征服务_.准备特征组清理(私有F, {});
        if (p.状态 != 新特征清理准备状态::已准备 || !p.准备)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"需求目标实例迁移", L"未挂接特征不能安全清理，保留现场。");
        特征服务_.执行已准备特征组清理(*p.准备); 私有F.clear();
    };
    try {
        std::lock_guard demandLock(新需求互斥);
        std::lock_guard featureLock(领域同步内部::存在特征互斥);
        const 需求目标读取上下文 ctx{特征服务_, 特征概念服务_};
        const auto 原始 = 全局基础数据集.查询节点组完整结构({D});
        if (原始.状态 == 基础结构操作状态::资源失败) return S::资源失败;
        要求需求物理(原始.状态 == 基础结构操作状态::已完成 && 原始.节点组.size() == 1);
        if (!原始.节点组.front().快照) return S::需求不存在;
        const auto raw = *原始.节点组.front().快照;
        const auto before = 解码需求治理物理(ctx, raw.结构);
        if (是F承载需求(raw.结构)) return S::无变化;
        const auto keys = 需求目标字段键(); const auto slots = 需求目标字段槽();
        私有F.reserve(keys.size());
        for (std::size_t i = 0; i < keys.size(); ++i) {
            const auto root = 特征概念服务_.读取先天特征类型槽(slots[i]);
            if (root.状态 == 特征概念规则状态::资源失败) { 清理(); return S::资源失败; }
            if (root.状态 == 特征概念规则状态::前提已变化) { 清理(); return S::前提已变化; }
            if (root.状态 == 特征概念规则状态::值不存在 || root.状态 == 特征概念规则状态::读取接口未装配) {
                清理(); return S::目标内容依赖未就绪;
            }
            要求需求物理(root.状态 == 特征概念规则状态::已完成 && root.类型根);
            const auto f = 特征服务_.建立特征强类型(*root.类型根, std::nullopt);
            if (f.状态 == 新特征写入状态::资源失败) { 清理(); return S::资源失败; }
            要求需求物理(f.状态 == 新特征写入状态::已完成 && f.特征节点);
            私有F.push_back(*f.特征节点);
            const auto it = std::find_if(raw.结构.字段组.begin(), raw.结构.字段组.end(), [&](const auto& v) { return v.字段节点 == keys[i]; });
            if (it != raw.结构.字段组.end()) {
                新特征准确值 value;
                if (const auto* ref = std::get_if<稳定编码>(&it->内容)) value = *ref;
                else {
                    const auto* v = std::get_if<基础值>(&it->内容);
                    const auto* n = v ? std::get_if<std::int64_t>(&v->材料) : nullptr;
                    要求需求物理(n); value = *n;
                }
                const auto added = 特征服务_.添加特征值(*f.特征节点, value);
                if (added.状态 == 新特征写入状态::资源失败) { 清理(); return S::资源失败; }
                要求需求物理(added.状态 == 新特征写入状态::已完成 || added.状态 == 新特征写入状态::无变化);
            }
        }
        // F全部建立后才捕获依赖，避免本次合法FC引用计数变化使旧预期失效。
        基础节点组追加请求 request; request.预期联合节点组.push_back(raw);
        for (std::size_t i = 0; i < keys.size(); ++i) {
            const auto f = 特征服务_.读取特征完整保护(私有F[i]);
            if (f.状态 == 新特征完整读取状态::资源失败) { 清理(); return S::资源失败; }
            if (f.状态 == 新特征完整读取状态::前提已变化) { 清理(); return S::前提已变化; }
            要求需求物理(f.状态 == 新特征完整读取状态::已读取 && f.信息);
            if (!并需求保护(request.预期联合节点组, f.信息->完整保护组)) { 清理(); return S::前提已变化; }
            const auto it = std::find_if(raw.结构.字段组.begin(), raw.结构.字段组.end(), [&](const auto& v) { return v.字段节点 == keys[i]; });
            if (it == raw.结构.字段组.end()) request.追加字段组.push_back({D, keys[i], 私有F[i]});
            else request.替换字段组.push_back({it->编码, 私有F[i]});
        }
        request.追加字段组.push_back({D, 需求目标承载格式字段, 基础值{std::int64_t{2}}});
        const auto commit = 全局基础数据集.原子追加节点组(request);
        if (commit.状态 == 基础节点组追加状态::预期不符) { 清理(); return S::前提已变化; }
        if (commit.状态 == 基础节点组追加状态::资源失败) { 清理(); return S::资源失败; }
        要求需求物理(commit.状态 == 基础节点组追加状态::已完成); 已挂接 = true;
        要求需求物理(读取需求治理物理(ctx, D) == before);
        return S::已修改;
    } catch (const std::bad_alloc&) {
        if (已挂接) throw;
        清理(); return S::资源失败;
    } catch (const std::length_error&) {
        if (已挂接) throw;
        清理(); return S::资源失败;
    }
    catch (新需求目标定义核验状态 s) {
        if(s!=新需求目标定义核验状态::前提已变化) { 清理(); throw; }
        if (已挂接) throw;
        清理(); return S::前提已变化;
    }
    catch (...) { 清理(); throw; }
}

void 新_需求类::要求目标特征实例承载(稳定编码 D) noexcept {
    if (!目标特征实例承载已启用_) return;
    const auto s = 确保目标特征实例承载内部(D);
    if (s != 新需求操作状态::已修改 && s != 新需求操作状态::无变化)
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"需求目标实例承载", L"已成立需求的目标实例迁移未完成，保留事实并中止。");
}

新需求操作状态 新_需求类::启用目标特征实例承载() noexcept {
    try {
        std::lock_guard lock(新需求互斥);
        if (!结构仍存在()) return 新需求操作状态::尚未初始化;
        const auto all = 查询全部需求记录();
        if (all.状态 != 新需求操作状态::已找到) return all.状态;
        std::set<稳定编码> ids;
        for (const auto& d : all.需求组) ids.insert(d.节点);
        for (const auto& d : all.待完善需求组) ids.insert(d.节点);
        bool changed = false;
        for (const auto D : ids) {
            const auto s = 确保目标特征实例承载内部(D);
            if (s != 新需求操作状态::已修改 && s != 新需求操作状态::无变化) return s;
            changed = changed || s == 新需求操作状态::已修改;
        }
        目标特征实例承载已启用_ = true;
        return changed ? 新需求操作状态::已修改 : 新需求操作状态::无变化;
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"启用需求目标实例承载", L"迁移读取工程故障，禁止开放线程。"); }
}

新需求操作状态 新_需求类::迁移既有自我需求归属(稳定编码 唯一自我) noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        if (!存在服务_.是存在节点(唯一自我)) return 新需求操作状态::目标存在不合法;
        const auto 格式组 = 全局基础数据集.查询字段(需求树根节点, 需求归属格式字段);
        if (!格式组.empty()) {
            if (读取唯一I64字段(需求树根节点, 需求归属格式字段) != 1)
                return 新需求操作状态::结构不一致;
            const auto 全集 = 查询全部需求记录();
            if (全集.状态 != 新需求操作状态::已找到) return 全集.状态;
            const auto 列表全集 = 查询全部需求列表();
            if (列表全集.状态 != 新需求操作状态::已找到) return 列表全集.状态;
            for (const auto& d : 全集.需求组)
                if (d.执行域 == 新需求执行域::自我执行 && d.需求主体存在节点 != 唯一自我)
                    return 新需求操作状态::结构不一致;
            return 新需求操作状态::无变化;
        }
        if (!全局基础数据集.查询字段(需求树根节点, 被认知需求登记字段).empty())
            return 新需求操作状态::结构不一致;
        std::vector<稳定编码> 节点组{需求树根节点};
        for (const auto& f : 全局基础数据集.查询目标字段(需求节点类型, 节点类型字段))
            节点组.push_back(f.所属节点);
        for (const auto& f : 全局基础数据集.查询目标字段(需求列表项节点类型, 节点类型字段))
            节点组.push_back(f.所属节点);
        std::sort(节点组.begin(), 节点组.end());
        if (std::adjacent_find(节点组.begin(), 节点组.end()) != 节点组.end())
            return 新需求操作状态::结构不一致;
        const auto 快照 = 全局基础数据集.查询节点组完整结构(节点组);
        要求需求物理(快照.状态 == 基础结构操作状态::已完成 && 快照.节点组.size() == 节点组.size());
        基础节点组追加请求 发布;
        std::map<稳定编码, 新需求治理信息> 需求;
        std::map<稳定编码, 新需求列表项信息> 列表;
        for (const auto& 项 : 快照.节点组) {
            要求需求物理(项.快照.has_value());
            const auto& s = *项.快照;
            发布.预期联合节点组.push_back(s);
            if (s.结构.节点.编码 == 需求树根节点) {
                需求物理游标 根游标{s.结构, {}};
                要求需求物理(根游标.引用(节点类型字段) == 需求树根节点类型
                    && 根游标.引用(安全根登记字段) == 安全根需求节点
                    && 根游标.引用(服务根登记字段) == 服务根需求节点);
                (void)根游标.引用组(需求列表项字段);
                要求需求物理(根游标.引用组(服务取得实例登记字段).empty());
                要求需求物理(根游标.引用组(被认知需求登记字段).empty()
                    && !根游标.数值(需求归属格式字段, false));
                根游标.完成();
                continue;
            }
            if (是需求类型节点(s.结构.节点.编码)) {
                const auto d = 解码需求治理物理(需求目标读取上下文{特征服务_, 特征概念服务_}, s.结构, true);
                if (d.认知来源 || (有效(d.需求主体存在节点)
                    && (d.需求主体存在节点 != 唯一自我 || d.执行域 != 新需求执行域::自我执行)))
                    return 新需求操作状态::结构不一致;
                if (!有效(d.需求主体存在节点)) {
                    发布.追加字段组.push_back({d.节点, 需求主体字段, 唯一自我});
                    发布.追加字段组.push_back({d.节点, 需求执行域字段,
                        基础值{static_cast<std::int64_t>(新需求执行域::自我执行)}});
                }
                需求.emplace(d.节点, d);
            } else 列表.emplace(s.结构.节点.编码, 解码需求列表物理(s.结构));
        }
        if (!需求.contains(安全根需求节点) || !需求.contains(服务根需求节点))
            return 新需求操作状态::结构不一致;
        const auto& 安全 = 需求.at(安全根需求节点); const auto& 服务 = 需求.at(服务根需求节点);
        if (安全.根角色 != 新需求根角色::安全 || 服务.根角色 != 新需求根角色::服务
            || !安全.完整目标 || !服务.完整目标 || 安全.完整目标->目标存在节点 != 唯一自我
            || 服务.完整目标->目标存在节点 != 唯一自我
            || 读取唯一节点字段(需求树根节点, 安全根登记字段) != 安全根需求节点
            || 读取唯一节点字段(需求树根节点, 服务根登记字段) != 服务根需求节点)
            return 新需求操作状态::结构不一致;
        for (const auto& [编码, d] : 需求) {
            std::set<稳定编码> 访问;
            auto 当前 = 编码;
            while (true) {
                if (!访问.insert(当前).second || !需求.contains(当前)) return 新需求操作状态::结构不一致;
                const auto& p = 需求.at(当前);
                if (p.根角色) break;
                if (!p.父需求节点) return 新需求操作状态::结构不一致;
                当前 = *p.父需求节点;
            }
            if (当前 != 安全根需求节点 && 当前 != 服务根需求节点) return 新需求操作状态::结构不一致;
        }
        const auto 登记 = 读取多节点字段(需求树根节点, 需求列表项字段);
        if (登记.size() != 列表.size()) return 新需求操作状态::结构不一致;
        std::set<稳定编码> 覆盖;
        using 迁移列表键 = std::tuple<std::optional<新需求根角色>, 稳定编码,
            稳定编码, 新需求目标形式, std::optional<稳定编码>,
            std::optional<稳定编码>, std::optional<新需求变化方向>>;
        std::set<迁移列表键> 列表键组;
        for (const auto& [编码, l] : 列表) {
            if (!列表键组.emplace(l.根角色, l.目标存在节点, l.目标.目标特征类型概念节点,
                    l.目标.目标形式, l.目标.目标值域概念节点, l.目标.目标比较关系概念节点,
                    l.目标.目标变化方向).second) return 新需求操作状态::结构不一致;
            if (std::count(登记.begin(), 登记.end(), 编码) != 1) return 新需求操作状态::结构不一致;
            const auto 列表反查 = 全局基础数据集.查询目标字段(编码, 需求列表项字段);
            if (列表反查.size() != 1 || 列表反查.front().所属节点 != 需求树根节点)
                return 新需求操作状态::结构不一致;
            for (const auto 成员 : l.需求节点组) {
                if (!需求.contains(成员) || !覆盖.insert(成员).second) return 新需求操作状态::结构不一致;
                const auto 成员反查 = 全局基础数据集.查询目标字段(成员, 列表成员字段);
                if (成员反查.size() != 1 || 成员反查.front().所属节点 != 编码)
                    return 新需求操作状态::结构不一致;
                const auto& d = 需求.at(成员);
                if (!d.完整目标 || d.完整目标->目标存在节点 != l.目标存在节点
                    || d.完整目标->目标 != l.目标 || d.根角色 != l.根角色)
                    return 新需求操作状态::结构不一致;
            }
        }
        if (覆盖.size() != 需求.size()) return 新需求操作状态::结构不一致;
        发布.追加字段组.push_back({需求树根节点, 需求归属格式字段, 基础值{std::int64_t{1}}});
        const auto 写入 = 全局基础数据集.原子追加节点组(发布);
        if (写入.状态 != 基础节点组追加状态::已完成)
            return 写入.状态 == 基础节点组追加状态::资源失败 ? 新需求操作状态::资源失败 : 新需求操作状态::结构不一致;
        const auto 回读 = 查询全部需求记录();
        要求需求物理(回读.状态 == 新需求操作状态::已找到);
        return 新需求操作状态::已修改;
    } catch (const 需求物理读取失败&) { return 新需求操作状态::结构不一致; }
    catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"迁移需求归属", L"需求归属迁移工程异常"); }
}

新需求双根初始化结果 新_需求类::建立或取得本能双根(
    const 新需求双根初始化请求& 请求) noexcept {
    新需求双根初始化结果 结果;
    try {
        std::lock_guard 锁(新需求互斥);
        if (!结构仍存在()) {
            结果.状态 = 新需求操作状态::尚未初始化;
            return 结果;
        }
        if (请求.安全根.初始权重 < 0 || 请求.服务根.初始权重 < 0) {
            结果.状态 = 新需求操作状态::权重不合法;
            return 结果;
        }

        const auto 核验根定义 = [&](const 新根需求定义& 定义)
            -> std::optional<稳定编码> {
            if (定义.目标.引用内容定位 || 定义.目标.新增成员来源见证 || !存在服务_.是存在节点(定义.目标存在节点)
                || !目标形状合法(定义.目标)
                || 定义.目标.目标形式 != 新需求目标形式::值 || 定义.目标.目标变化方向) {
                return std::nullopt;
            }
            const auto 目标存在 = 存在服务_.获取存在(定义.目标存在节点);
            const auto 目标根 = 取得特征类型根(
                定义.目标.目标特征类型概念节点, 特征概念服务_);
            if (!目标存在 || !目标根
                || *目标根 != 定义.目标.目标特征类型概念节点) {
                return std::nullopt;
            }
            const auto 目标值域 = 特征概念服务_.获取特征概念(*定义.目标.目标值域概念节点);
            const auto 比较关系 = 二次特征概念服务_.获取二次特征概念(*定义.目标.目标比较关系概念节点);
            const auto 域类型根=取得特征类型根(*定义.目标.目标值域概念节点,特征概念服务_);
            constexpr auto 最大值=(std::numeric_limits<std::int64_t>::max)();
            const auto* 区间组=目标值域 ? std::get_if<std::vector<特征概念I64闭区间>>(&目标值域->值域) : nullptr;
            const std::vector<二次特征概念输入定义> 必要输入{
                {0,二次特征输入角色::左输入,二次特征输入来源种类::特征准确值,*目标根},
                {1,二次特征输入角色::右输入,二次特征输入来源种类::特征准确值,*目标根}};
            if (!目标值域 || 目标值域->材料类型!=特征概念材料物理类型::I64标量
                || !域类型根 || *域类型根!=*目标根 || !区间组 || 区间组->size()!=1
                || 区间组->front().下界!=最大值 || 区间组->front().上界!=最大值
                || !比较关系 || 比较关系->算法!=二次特征算法::有向特征差值
                || 比较关系->输入定义组!=必要输入 || 比较关系->结果域种类组.empty()
                || !std::all_of(比较关系->结果域种类组.begin(),比较关系->结果域种类组.end(),[](auto 关系){
                    return 关系==二次特征结果域种类::等于||关系==二次特征结果域种类::包含于参照值域;
                }) || !比较关系适用于目标(*比较关系,*目标根,*定义.目标.目标值域概念节点,特征概念服务_))
                return std::nullopt;
            std::size_t 命中数量 = 0;
            for (const auto 特征节点 : 目标存在->特征节点组) {
                const auto 特征 = 特征服务_.获取特征(特征节点);
                const auto 实际根 = 特征
                    ? 取得特征类型根(特征->特征概念节点, 特征概念服务_)
                    : std::nullopt;
                if (!特征 || !实际根) return std::nullopt;
                if (*实际根 == *目标根) ++命中数量;
            }
            return 命中数量 == 1 ? 目标根 : std::nullopt;
        };

        const auto 安全类型根 = 核验根定义(请求.安全根);
        const auto 服务类型根 = 核验根定义(请求.服务根);
        if (!安全类型根 || !服务类型根) {
            结果.状态 = 新需求操作状态::双根定义冲突;
            return 结果;
        }
        if (请求.安全根.目标存在节点 != 请求.服务根.目标存在节点
            || 请求.安全根.目标.目标特征类型概念节点
                == 请求.服务根.目标.目标特征类型概念节点) {
            结果.状态 = 新需求操作状态::双根定义冲突;
            return 结果;
        }

        const bool 已有安全 = 有效(安全根需求节点);
        const bool 已有服务 = 有效(服务根需求节点);
        if (已有安全 || 已有服务) {
            if (!已有安全 || !已有服务) {
                结果.状态 = 新需求操作状态::结构不一致;
                return 结果;
            }
            // 迁移也不能让冲突初始化请求产生写入。只在这条私有入口允许
            // 按旧形状读回，先核对两个原定义，再进入一次原子补字段。
            const auto 旧安全 = 解码需求治理物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 读取需求物理节点(安全根需求节点), true);
            const auto 旧服务 = 解码需求治理物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 读取需求物理节点(服务根需求节点), true);
            if (旧安全.根角色 != 新需求根角色::安全 || 旧服务.根角色 != 新需求根角色::服务
                || !旧安全.完整目标 || !旧服务.完整目标
                || 旧安全.完整目标->目标存在节点 != 请求.安全根.目标存在节点
                || 旧服务.完整目标->目标存在节点 != 请求.服务根.目标存在节点
                || 旧安全.完整目标->目标 != 请求.安全根.目标 || 旧服务.完整目标->目标 != 请求.服务根.目标
                || 旧安全.权重 != 请求.安全根.初始权重 || 旧服务.权重 != 请求.服务根.初始权重) {
                结果.状态 = 新需求操作状态::双根定义冲突; return 结果;
            }
            const auto 迁移 = 迁移既有自我需求归属(请求.安全根.目标存在节点);
            if (迁移 != 新需求操作状态::已修改 && 迁移 != 新需求操作状态::无变化) {
                结果.状态 = 迁移; return 结果;
            }
            const auto 安全信息 = 读取需求信息(需求目标读取上下文{特征服务_, 特征概念服务_}, 安全根需求节点);
            const auto 服务信息 = 读取需求信息(需求目标读取上下文{特征服务_, 特征概念服务_}, 服务根需求节点);
            if (!安全信息 || !服务信息
                || 安全信息->根角色 != 新需求根角色::安全
                || 服务信息->根角色 != 新需求根角色::服务
                || 安全信息->目标存在节点 != 请求.安全根.目标存在节点
                || 服务信息->目标存在节点 != 请求.服务根.目标存在节点
                || 安全信息->目标 != 请求.安全根.目标
                || 服务信息->目标 != 请求.服务根.目标
                || 安全信息->权重 != 请求.安全根.初始权重
                || 服务信息->权重 != 请求.服务根.初始权重) {
                结果.状态 = 新需求操作状态::双根定义冲突;
                return 结果;
            }
            const auto 安全列表字段组 = 全局基础数据集.查询目标字段(
                安全根需求节点, 列表成员字段);
            const auto 服务列表字段组 = 全局基础数据集.查询目标字段(
                服务根需求节点, 列表成员字段);
            const auto 安全登记 = 读取唯一节点字段(
                需求树根节点, 安全根登记字段);
            const auto 服务登记 = 读取唯一节点字段(
                需求树根节点, 服务根登记字段);
            const auto 安全列表项 = 安全列表字段组.size() == 1
                ? 读取列表项信息(安全列表字段组.front().所属节点)
                : std::nullopt;
            const auto 服务列表项 = 服务列表字段组.size() == 1
                ? 读取列表项信息(服务列表字段组.front().所属节点)
                : std::nullopt;
            if (安全列表字段组.size() != 1 || 服务列表字段组.size() != 1
                || !安全登记 || *安全登记 != 安全根需求节点
                || !服务登记 || *服务登记 != 服务根需求节点
                || !安全列表项 || !服务列表项
                || 安全列表项->根角色 != 新需求根角色::安全
                || 服务列表项->根角色 != 新需求根角色::服务
                || 安全列表项->目标存在节点 != 请求.安全根.目标存在节点
                || 服务列表项->目标存在节点 != 请求.服务根.目标存在节点
                || 安全列表项->目标 != 请求.安全根.目标
                || 服务列表项->目标 != 请求.服务根.目标
                || 安全列表项->需求节点组.size() != 1
                || 服务列表项->需求节点组.size() != 1
                || 安全列表项->需求节点组.front() != 安全根需求节点
                || 服务列表项->需求节点组.front() != 服务根需求节点) {
                结果.状态 = 新需求操作状态::结构不一致;
                return 结果;
            }
            结果.状态 = 新需求操作状态::已复用;
            结果.安全根 = 安全根需求节点;
            结果.服务根 = 服务根需求节点;
            结果.安全根列表项 = 安全列表字段组.front().所属节点;
            结果.服务根列表项 = 服务列表字段组.front().所属节点;
            要求目标特征实例承载(安全根需求节点);
            要求目标特征实例承载(服务根需求节点);
            return 结果;
        }

        if (!全局基础数据集.查询字段(需求树根节点, 安全根登记字段).empty()
            || !全局基础数据集.查询字段(需求树根节点, 服务根登记字段).empty()) {
            结果.状态 = 新需求操作状态::结构不一致;
            return 结果;
        }

        const 关联检查离域通知 变更通知{关联检查通知_};
        auto 安全材料 = 建立根材料(
            新需求根角色::安全, 请求.安全根);
        if (!安全材料) {
            结果.状态 = 新需求操作状态::资源失败;
            return 结果;
        }
        auto 服务材料 = 建立根材料(
            新需求根角色::服务, 请求.服务根);
        if (!服务材料) {
            回滚根材料(*安全材料);
            结果.状态 = 新需求操作状态::资源失败;
            return 结果;
        }

        std::vector<稳定编码> 根字段;
        auto 注册 = [&](稳定编码 字段, 稳定编码 目标) {
            const auto 关系 = 全局基础数据集.添加字段节点(需求树根节点, 字段, 目标);
            if (有效(关系)) 根字段.push_back(关系);
            return 有效(关系);
        };
        const auto 注册归属格式 = [&]() {
            const auto 关系 = 全局基础数据集.添加字段值(
                需求树根节点, 需求归属格式字段, 基础值{std::int64_t{1}});
            if (有效(关系)) 根字段.push_back(关系);
            return 有效(关系);
        };
        if (!注册(安全根登记字段, 安全材料->需求)
            || !注册(服务根登记字段, 服务材料->需求)
            || !注册(需求列表项字段, 安全材料->列表项)
            || !注册(需求列表项字段, 服务材料->列表项)
            || !注册归属格式()) {
            回滚字段(根字段);
            回滚根材料(*服务材料);
            回滚根材料(*安全材料);
            结果.状态 = 新需求操作状态::资源失败;
            return 结果;
        }

        安全根需求节点 = 安全材料->需求;
        服务根需求节点 = 服务材料->需求;
        结果.状态 = 新需求操作状态::已建立;
        结果.安全根 = 安全根需求节点;
        结果.服务根 = 服务根需求节点;
        结果.安全根列表项 = 安全材料->列表项;
        结果.服务根列表项 = 服务材料->列表项;
        要求目标特征实例承载(安全根需求节点);
        要求目标特征实例承载(服务根需求节点);
        return 结果;
    } catch (const std::bad_alloc&) {
        结果.状态 = 新需求操作状态::资源失败;
        return 结果;
    } catch(const std::length_error&){结果.状态=新需求操作状态::资源失败;return 结果;}
    catch(const std::exception& 异常){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"建立或取得本能双根",L"双根完整M目标建立或读回工程异常，保留已发表事实",异常.what());}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"建立或取得本能双根",L"双根完整M目标建立发生未知工程异常，保留已发表事实");}
}

新需求建立结果 新_需求类::建立或取得新增成员见证需求已核验(
    const 新普通需求建立请求& q,const 新需求新增成员见证要求& requirement) {
    using S=新需求操作状态;
    新需求建立结果 prefix;
    try {
        if(q.目标.新增成员来源见证||q.目标.引用内容定位||q.复合来源
            ||q.来源类型!=新需求来源类型::任务子目标||!q.来源承接节点)return {S::入口拒绝,{},{}};
        const 新需求新增成员读取接口* reader=nullptr;
        const 特征任务成员读取接口* taskReader=nullptr;
        const auto lookup=[&]()->std::optional<新需求信息>{
            std::optional<新需求信息> found;
            for(const auto& edge:查询子需求关系(q.父需求节点)){
                const auto d=获取需求(edge.目标节点);要求需求物理(d.has_value());
                auto target=d->目标;target.新增成员来源见证.reset();
                if(d->目标存在节点!=q.目标存在节点||target!=q.目标||d->来源类型!=q.来源类型||d->来源承接节点!=q.来源承接节点)continue;
                if(d->目标.新增成员来源见证&&d->目标.新增成员来源见证->要求!=requirement)continue;
                要求需求物理(!found);found=d;
            }return found;
        };
        std::optional<新需求信息> old;
        {std::lock_guard lock(新需求互斥);reader=新增成员读取_;taskReader=服务要求任务读取接口_;old=lookup();}
        if(old&&!old->目标.新增成员来源见证)return {S::目标内容未支持,{},{}}; // 旧目标需重新形成，不静默富化。
        if(!reader||!taskReader)return {S::目标内容依赖未就绪,{},{}};
        const auto taskSnapshot=全局基础数据集.查询节点组完整结构({*q.来源承接节点});
        if(taskSnapshot.状态==基础结构操作状态::资源失败)return {S::资源失败,{},{}};
        要求需求物理(taskSnapshot.状态==基础结构操作状态::已完成&&taskSnapshot.节点组.size()==1);
        if(!taskSnapshot.节点组.front().快照)return {S::前提已变化,{},{}};
        const auto taskIdentity=taskReader->读取任务所属需求(*q.来源承接节点);
        if(taskIdentity.状态==特征任务成员读取状态::资源失败)return {S::资源失败,{},{}};
        if(taskIdentity.状态==特征任务成员读取状态::未装配)return {S::目标内容依赖未就绪,{},{}};
        if(taskIdentity.状态!=特征任务成员读取状态::已读取||taskIdentity.需求节点!=q.父需求节点)return {S::任务来源未接通,{},{}};
        新普通需求建立请求 request=q;std::vector<基础联合节点快照> protect;
        if(old)request.目标=old->目标;
        else {
            auto baseline=reader->读取新增成员目标基准(q.目标存在节点,requirement.源特征类型概念节点);
            switch(baseline.状态){
            case 新需求新增成员读取状态::已读取:要求需求物理(baseline.基准.has_value());break;
            case 新需求新增成员读取状态::前提已变化:return {S::前提已变化,{},{}};
            case 新需求新增成员读取状态::资源失败:return {S::资源失败,{},{}};
            case 新需求新增成员读取状态::依赖未就绪:return {S::目标内容依赖未就绪,{},{}};
            case 新需求新增成员读取状态::未支持:return {S::目标内容未支持,{},{}};
            default:要求需求物理(false);
            }
            request.目标.新增成员来源见证=新需求新增成员见证约束{requirement,baseline.基准->材料节点};protect=std::move(baseline.完整保护);
        }
        auto definition=核验新增成员目标定义保护({request.目标存在节点,request.目标});
        if(definition.状态==新需求目标定义核验状态::资源失败)return {S::资源失败,{},{}};
        if(definition.状态==新需求目标定义核验状态::前提已变化)return {S::前提已变化,{},{}};
        if(definition.状态!=新需求目标定义核验状态::已完成)return {S::目标内容未支持,{},{}};
        if(!并需求保护(protect,definition.完整保护))return {S::前提已变化,{},{}};
        if(!并需求保护(protect,{*taskSnapshot.节点组.front().快照})||!需求内容保护未变(protect))return {S::前提已变化,{},{}};
        std::lock_guard lock(新需求互斥);
        const auto fresh=lookup();
        if(fresh){
            if(!fresh->目标.新增成员来源见证)return {S::目标内容未支持,{},{}};
            if(old&&fresh!=old)return {S::前提已变化,{},{}};
            const auto list=核验需求列表物理(需求目标读取上下文{特征服务_,特征概念服务_},*fresh);
            prefix={S::已复用,fresh->节点,list};
            if(目标特征实例承载已启用_){
                const auto ready=确保目标特征实例承载可抛内部(fresh->节点);
                if(ready==S::资源失败)throw std::bad_alloc{};
                if(ready==S::前提已变化)throw 新需求目标定义核验状态::前提已变化;
                要求需求物理(ready==S::已修改||ready==S::无变化);
            }
            return prefix;
        }
        if(old)return {S::前提已变化,{},{}};
        return 建立或取得普通需求来源可抛内部(request,true,{},{},prefix,protect);
    }catch(const std::bad_alloc&){auto r=prefix;r.状态=S::资源失败;return r;}
    catch(const std::length_error&){auto r=prefix;r.状态=S::资源失败;return r;}
    catch(const 新需求目标定义核验状态 s){要求需求物理(s==新需求目标定义核验状态::前提已变化);auto r=prefix;r.状态=S::前提已变化;return r;}
    catch(const 需求物理读取失败& e){if(e.原因==基础结构操作状态::资源失败){auto r=prefix;r.状态=S::资源失败;return r;}throw;}
}
新需求建立结果 新_需求类::建立或取得普通需求(
    const 新普通需求建立请求& 请求) noexcept {
    return 建立或取得普通需求已核验(请求, false);
}

struct 新_需求类::服务取得角色核验批 final {
    enum class 状态值 : std::uint8_t { 已核验=1,输入不适用,前提已变化,依赖未就绪,资源失败 };
    状态值 状态=状态值::输入不适用;
    std::vector<基础联合节点快照> 完整保护;
};

新_需求类::服务取得角色核验批 新_需求类::读取服务取得角色核验批(
    稳定编码 H,稳定编码 human,稳定编码 demand,稳定编码 complete) const {
    using S=服务取得角色核验批::状态值;
    服务取得角色核验批 out;
    try {
        if(!有效(H)||!有效(human)||!有效(demand)||!有效(complete))return out;
        const auto merge=[&](const auto& p) {
            if(!并需求保护(out.完整保护,p))throw S::前提已变化;
        };
        const auto concepts=存在概念服务_.读取先天需求任务方法概念();
        const auto content=存在概念服务_.读取引用内容筹办概念();
        const auto structureStatus=[&](正式结构操作状态 s) {
            if(s==正式结构操作状态::依赖未就绪||s==正式结构操作状态::未找到)throw S::依赖未就绪;
            if(s==正式结构操作状态::前提已变化)throw S::前提已变化;
            要求需求物理(s==正式结构操作状态::已读取);
        };
        structureStatus(concepts.状态);structureStatus(content.状态);
        要求需求物理(concepts.信息&&content.信息);
        if(concepts.信息->需求!=demand||content.信息->完整被认知需求!=complete)return out;
        merge(content.完整保护组);
        for(const auto C:{demand,complete}) {
            const auto r=存在概念服务_.读取正式概念身份保护(C);
            if(r.状态==正式概念读取状态::依赖未就绪)throw S::依赖未就绪;
            if(r.状态==正式概念读取状态::前提已变化)throw S::前提已变化;
            要求需求物理(r.状态==正式概念读取状态::已读取&&r.身份&&r.身份->概念节点==C);
            merge(r.身份->完整保护组);
        }
        const auto ordinary=[&](稳定编码 C) {
            auto r=存在概念服务_.读取普通存在概念定义保护(C);
            if(r.状态==正式概念读取状态::依赖未就绪)throw S::依赖未就绪;
            if(r.状态==正式概念读取状态::前提已变化)throw S::前提已变化;
            要求需求物理(r.状态==正式概念读取状态::已读取&&r.信息&&r.信息->节点==C);
            merge(r.完整保护组);return *r.信息;
        };
        const auto humanDefinition=ordinary(human);
        if(humanDefinition.种类!=存在概念种类::抽象||humanDefinition.对应存在节点)return out;
        const auto e=存在服务_.读取存在身份保护(H);
        if(e.前提已变化)throw S::前提已变化;
        if(e.状态==基础结构操作状态::资源失败)throw S::资源失败;
        if(e.状态==基础结构操作状态::节点不存在||e.状态==基础结构操作状态::入口拒绝)return out;
        要求需求物理(e.状态==基础结构操作状态::已完成&&e.信息&&e.信息->存在节点==H);
        merge(e.预期节点组);
        // CONCEPT-INFO-MEMBERSHIP：H属于人类调用唯一归属算法，并入依据保护；未证明属于则不核验。
        const auto 判定=存在服务_.比较存在与概念信息(H,human,true);
        if(判定.状态==特征概念规则状态::资源失败)throw S::资源失败;
        if(判定.状态==特征概念规则状态::前提已变化)throw S::前提已变化;
        要求需求物理(判定.状态==特征概念规则状态::已完成);
        merge(判定.依据保护);
        if(判定.归属!=特征成员概念归属::属于)return out;
        if(!需求内容保护未变(out.完整保护))throw S::前提已变化;
        out.状态=S::已核验;return out;
    } catch(S s) {out.状态=s;out.完整保护.clear();return out;}
    catch(const std::bad_alloc&){out.状态=S::资源失败;out.完整保护.clear();return out;}
    catch(const std::length_error&){out.状态=S::资源失败;out.完整保护.clear();return out;}
}

新服务取得目标实例结果 新_需求类::读取服务取得目标实例可抛内部(
    const 新服务取得目标实例键& key,新服务取得目标实例结果& out) const {
    return 读取服务取得目标实例核心可抛内部(key.父取得任务,key.目标槽类型,key.正式根路径,key.服务对象,out);
}

新服务取得目标实例结果 新_需求类::读取服务取得目标实例(const 新服务取得目标实例键& key) const noexcept {
    using S=新服务取得目标实例状态;
    新服务取得目标实例结果 out;
    try {return 读取服务取得目标实例可抛内部(key,out);}
    catch(S s){
        if(s!=S::前提已变化)工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取服务取得目标实例",L"未知实例读取控制状态。");
        out.状态=s;
    }
    catch(新需求目标定义核验状态 s){
        if(s!=新需求目标定义核验状态::前提已变化)
            工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取服务取得目标实例",L"未知目标读取状态。");
        out.状态=S::前提已变化;
    }
    catch(const 需求物理读取失败& e){
        if(e.原因!=基础结构操作状态::资源失败)
            工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取服务取得目标实例",L"正式出生记录或需求结构损坏。");
        out.状态=S::资源失败;
    }
    catch(const std::bad_alloc&){out.状态=S::资源失败;}
    catch(const std::length_error&){out.状态=S::资源失败;}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取服务取得目标实例",L"实例正式读取工程故障；保留已出生事实。");}
    if(!out.信息)out.完整保护.clear();return out;
}

thread_local int 服务取得出生失败行 = 0;
新服务取得目标实例结果 新_需求类::建立或取得服务取得目标实例(
    const 新服务取得目标实例请求& q) noexcept {
    using S=新服务取得目标实例状态;
    using R=服务取得角色核验批::状态值;
    服务取得出生失败行=0;
    新服务取得目标实例结果 out;
    const auto 返回=[&](S 当前状态) {
        out.状态=当前状态;
        if(!out.信息)out.完整保护.clear();
        return out;
    };
    try {
        std::lock_guard lock(新需求互斥);
        if(!结构仍存在()){out.状态=S::依赖未就绪;return out;}
        if(!有效(q.键.父取得任务)||!有效(q.键.目标槽类型)||!有效(q.键.服务对象)
            ||!q.键.正式根路径.empty()||!有效(q.父需求)||!有效(q.人类概念)
            ||!有效(q.需求身份概念)||!有效(q.完整内容概念)||q.正式前驱保护.empty()
            ||(q.指定已有需求&&!有效(*q.指定已有需求)))
            {out.状态=S::输入不适用;return out;}
        auto 槽实例=读取服务取得目标实例核心可抛内部(q.键.父取得任务,q.键.目标槽类型,q.键.正式根路径,std::nullopt,out);
        out=std::move(槽实例);
        if(out.状态!=S::未找到&&out.状态!=S::已复用&&out.状态!=S::已出生待补骨架)return out;
        要求需求物理(!out.完整保护.empty());
        if(out.信息) {
            const auto& old=*out.信息;
            if(old.键!=q.键||old.父需求!=q.父需求||old.原父目标!=q.原父目标||old.人类概念!=q.人类概念
                ||old.需求身份概念!=q.需求身份概念||old.完整内容概念!=q.完整内容概念
                ||old.复用指定已有需求!=q.指定已有需求.has_value()||(q.指定已有需求&&old.需求节点!=q.指定已有需求))
                {out.状态=S::绑定冲突;return out;}
        }
        const auto roles=读取服务取得角色核验批(q.键.服务对象,q.人类概念,q.需求身份概念,q.完整内容概念);
        switch(roles.状态) {
        case R::已核验:break;
        case R::输入不适用:return 返回(out.信息 ? S::前提已变化 : S::输入不适用);
        case R::前提已变化:return 返回(S::前提已变化);
        case R::依赖未就绪:return 返回(S::依赖未就绪);
        case R::资源失败:return 返回(S::资源失败);
        default:要求需求物理(false);
        }
        基础节点组追加请求 request;request.预期联合节点组=roles.完整保护;
        // 同一无H槽读取的完整范围进入出生CAS；旧H记录不能被完整key过滤掉。
        if(!并需求保护(request.预期联合节点组,out.完整保护)){服务取得出生失败行=__LINE__;throw S::前提已变化;}
        // 正式前驱保护为本步依据（交互者特征、H属于人类的依据、本槽实例）；按容许新增入向预期
        // （2026-11-08用户裁决：采用只复核要采用的这一步；本体改变或原引用删除即前提已变化，无关新增引用不影响）。
        request.容许新增入向预期组.insert(request.容许新增入向预期组.end(),q.正式前驱保护.begin(),q.正式前驱保护.end());
        const auto parentRaw=取得服务实例物理保护(q.父需求,request.预期联合节点组);
        (void)投影需求目标实例(需求目标读取上下文{特征服务_,特征概念服务_},parentRaw.结构,&request.预期联合节点组);
        const auto parent=读取需求治理信息可抛内部(q.父需求);
        if(!parent||parent->执行域!=新需求执行域::自我执行||parent->完整目标!=std::optional{q.原父目标})
            {服务取得出生失败行=__LINE__;throw S::前提已变化;}
        const auto slot=特征概念服务_.读取特征概念定义保护(q.键.目标槽类型);
        if(slot.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        if(slot.前提已变化){服务取得出生失败行=__LINE__;throw S::前提已变化;}
        要求需求物理(slot.状态==基础结构操作状态::已完成&&slot.信息&&slot.信息->类型根==q.键.目标槽类型);
        if(!并需求保护(request.预期联合节点组,slot.预期节点组)){服务取得出生失败行=__LINE__;throw S::前提已变化;}
        std::vector<基础组内初始字段> savedFields{{节点类型字段,实例原父目标类型}};
        追加服务取得原父目标快照字段(savedFields,q.父需求,q.原父目标,request.预期联合节点组);
        bool created=false;
        if(out.信息) {
            const auto saved=*读取唯一节点字段(out.信息->出生记录,实例原父目标字段);
            if(读取唯一节点字段(saved,目标新增成员见证字段)!=读取唯一节点字段(q.父需求,目标新增成员见证字段))
                {服务取得出生失败行=__LINE__;throw S::前提已变化;}
        } else {
            (void)取得服务实例物理保护(需求树根节点,request.预期联合节点组);
            (void)取得服务实例物理保护(q.键.父取得任务,request.预期联合节点组);
            if(q.指定已有需求) {
                const auto old=读取需求治理信息可抛内部(*q.指定已有需求);
                if(!old||old->需求主体存在节点!=q.键.服务对象||old->执行域!=新需求执行域::被认知)
                    return 返回(S::输入不适用);
                const auto check=判定被认知需求内容保护(*q.指定已有需求,q.键.服务对象,q.完整内容概念);
                switch(check.状态) {
                case 新引用内容目标状态::已满足:
                case 新引用内容目标状态::未满足:
                case 新引用内容目标状态::分支待定:break;
                case 新引用内容目标状态::资源失败:throw std::bad_alloc{};
                case 新引用内容目标状态::引用前提已变化:{服务取得出生失败行=__LINE__;throw S::前提已变化;}
                case 新引用内容目标状态::依赖未就绪:return 返回(S::依赖未就绪);
                case 新引用内容目标状态::未支持:
                case 新引用内容目标状态::定义不合法:return 返回(S::输入不适用);
                default:要求需求物理(false);
                }
                要求需求物理(!check.完整保护组.empty());
                if(!并需求保护(request.预期联合节点组,check.完整保护组)){服务取得出生失败行=__LINE__;throw S::前提已变化;}
                (void)取得服务实例物理保护(*q.指定已有需求,request.预期联合节点组);
            }
            // 先分配值式前缀的所有动态存储，原子提交后只填新身份并move。
            新服务取得目标实例信息 info;
            info.键=q.键;info.父需求=q.父需求;info.原父目标=q.原父目标;
            info.人类概念=q.人类概念;info.需求身份概念=q.需求身份概念;info.完整内容概念=q.完整内容概念;
            info.复用指定已有需求=q.指定已有需求.has_value();
            if(q.指定已有需求)info.需求节点=*q.指定已有需求;
            基础组内新节点 record;
            record.字段组={{节点类型字段,服务取得实例记录类型},{实例格式字段,基础值{std::int64_t{1}}},
                {实例父任务字段,q.键.父取得任务},{实例目标槽字段,q.键.目标槽类型},
                {实例根路径字段,基础值{std::vector<std::uint64_t>{}}},{实例主体字段,q.键.服务对象},
                {实例父需求字段,q.父需求},{实例原父目标字段,基础组内节点引用{1}},
                {实例人类概念字段,q.人类概念},{实例需求身份概念字段,q.需求身份概念},
                {实例完整内容概念字段,q.完整内容概念},{实例复用指定字段,基础值{std::int64_t{q.指定已有需求 ? 1 : 0}}}};
            if(q.指定已有需求)record.字段组.push_back({实例需求字段,*q.指定已有需求});
            else record.字段组.push_back({实例需求字段,基础组内节点引用{2}});
            request.新节点组.push_back(std::move(record));
            savedFields.push_back({所属实例记录字段,基础组内节点引用{0}});
            基础组内新节点 saved;saved.字段组=std::move(savedFields);request.新节点组.push_back(std::move(saved));
            if(q.指定已有需求)request.追加字段组.push_back({*q.指定已有需求,服务取得实例来源字段,基础组内节点引用{0}});
            else {
                基础组内新节点 d;
                d.字段组={{节点类型字段,需求节点类型},{需求主体字段,q.键.服务对象},
                    {需求执行域字段,基础值{static_cast<std::int64_t>(新需求执行域::被认知)}},
                    {活动状态字段,基础值{static_cast<std::int64_t>(新需求活动状态::活动)}},
                    {需求权重字段,基础值{std::int64_t{0}}},
                    {需求信息完善阶段字段,基础值{static_cast<std::int64_t>(新需求信息完善阶段::待完善)}},
                    {服务取得实例来源字段,基础组内节点引用{0}}};
                request.新节点组.push_back(std::move(d));
                request.追加字段组.push_back({需求树根节点,被认知需求登记字段,基础组内节点引用{2}});
            }
            request.追加字段组.push_back({需求树根节点,服务取得实例登记字段,基础组内节点引用{0}});
            const auto commit=全局基础数据集.原子追加节点组(request);
            if(commit.状态==基础节点组追加状态::预期不符){
                服务取得出生失败行=__LINE__;
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
                try {
                    std::wostringstream t;
                    auto 依据组=联合快照组节点(request.容许新增入向预期组);
                    {const auto 严格=联合快照组节点(request.预期联合节点组);依据组.insert(严格.begin(),严格.end());}
                    const auto 查=[&](const std::vector<基础联合节点快照>& 组,bool 严格,const wchar_t* 名){
                        for(const auto& 预期:组){
                            const auto 今=全局基础数据集.查询节点组完整结构({预期.结构.节点.编码});
                            if(今.状态!=基础结构操作状态::已完成||今.节点组.size()!=1||!今.节点组[0].快照){t<<名<<L"节点="<<预期.结构.节点.编码.值<<L"(不可读)；";continue;}
                            const auto& 当前=*今.节点组[0].快照;
                            const bool 同=严格 ? (当前.结构.字段组==预期.结构.字段组&&当前.入向字段组==预期.入向字段组&&当前.入向外部关系组==预期.入向外部关系组&&当前.出向外部关系组==预期.出向外部关系组)
                                : 联合快照容许变化判定(当前,预期,依据组);
                            if(同)continue;
                            t<<名<<L"节点="<<预期.结构.节点.编码.值<<L"本体"<<预期.结构.字段组.size()<<L"->"<<当前.结构.字段组.size()
                             <<L"入向"<<预期.入向字段组.size()<<L"->"<<当前.入向字段组.size()<<L"；";
                        }
                    };
                    查(request.预期联合节点组,true,L"严格");查(request.容许新增入向预期组,false,L"容许");
                    (void)记录调试日志(L"SIMULATION_PROGRESS",L"服务取得目标实例出生",L"预期不符明细",t.str());
                } catch(...) {}
#endif
                throw S::前提已变化;}
            if(commit.状态==基础节点组追加状态::资源失败)throw std::bad_alloc{};
            要求需求物理(commit.状态==基础节点组追加状态::已完成&&commit.新节点组.size()==(q.指定已有需求 ? 2u : 3u));
            info.出生记录=commit.新节点组[0];if(!q.指定已有需求)info.需求节点=commit.新节点组[2];
            out.信息=std::move(info);out.状态=S::已出生待补骨架;out.完整保护.clear();created=true;
        }
        if(!created&&!需求内容保护未变(request.预期联合节点组)){服务取得出生失败行=__LINE__;throw S::前提已变化;}
        // 复用/补骨架同样受本步依据（正式前驱保护，容许新增入向）约束：本体改变或原引用删除即前提已变化。
        if(!created&&!request.容许新增入向预期组.empty()) {
            std::vector<稳定编码> 依据节点;for(const auto& 项:request.容许新增入向预期组)依据节点.push_back(项.结构.节点.编码);
            const auto 当前=全局基础数据集.查询节点组完整结构(依据节点);
            if(当前.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            if(当前.状态!=基础结构操作状态::已完成||当前.节点组.size()!=依据节点.size()){服务取得出生失败行=__LINE__;throw S::前提已变化;}
            auto 依据组=联合快照组节点(request.容许新增入向预期组);
            {const auto 严格=联合快照组节点(request.预期联合节点组);依据组.insert(严格.begin(),严格.end());}
            for(const auto& 预期:request.容许新增入向预期组){
                const auto 今=std::find_if(当前.节点组.begin(),当前.节点组.end(),[&](const auto& x){return x.查询节点==预期.结构.节点.编码;});
                if(今==当前.节点组.end()||!今->快照||!联合快照容许变化判定(*今->快照,预期,依据组))
                    {服务取得出生失败行=__LINE__;throw S::前提已变化;}
            }
        }
        const auto skeleton=确保目标特征实例承载可抛内部(out.信息->需求节点);
        if(skeleton==新需求操作状态::资源失败)throw std::bad_alloc{};
        if(skeleton==新需求操作状态::前提已变化){服务取得出生失败行=__LINE__;throw S::前提已变化;}
        if(skeleton==新需求操作状态::目标内容依赖未就绪){out.状态=S::依赖未就绪;return out;}
        要求需求物理(skeleton==新需求操作状态::已修改||skeleton==新需求操作状态::无变化);
        新服务取得目标实例结果 fresh;
        (void)读取服务取得目标实例可抛内部(q.键,fresh);
        if(fresh.信息)out=std::move(fresh);
        else {
            // 末次读回可能在再次取得R之前遇到具名资源/依赖/变化，不能丢已出生前缀。
            要求需求物理(fresh.状态==S::资源失败||fresh.状态==S::依赖未就绪||fresh.状态==S::前提已变化);
            out.状态=fresh.状态;
        }
        if(out.状态==S::已复用&&created)out.状态=S::已建立;
        return out;
    } catch(S s){
        if(s!=S::前提已变化)工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"建立服务取得目标实例",L"未知实例控制状态。");
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
        try{(void)记录调试日志(L"SIMULATION_PROGRESS",L"服务取得目标实例出生",L"前提已变化",L"行="+std::to_wstring(服务取得出生失败行));}catch(...){}
#endif
        out.状态=s;
    }
    catch(新需求目标定义核验状态 s){
        if(s!=新需求目标定义核验状态::前提已变化)工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"建立服务取得目标实例",L"未知目标控制状态。");
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
        try{(void)记录调试日志(L"SIMULATION_PROGRESS",L"服务取得目标实例出生",L"目标定义前提已变化",L"行="+std::to_wstring(服务取得出生失败行));}catch(...){}
#endif
        out.状态=S::前提已变化;
    }
    catch(const 需求物理读取失败& e){
        if(e.原因!=基础结构操作状态::资源失败)工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"建立服务取得目标实例",L"正式结构损坏；保留已出生事实。");
        out.状态=S::资源失败;
    }
    catch(const std::bad_alloc&){out.状态=S::资源失败;}
    catch(const std::length_error&){out.状态=S::资源失败;}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"建立服务取得目标实例",L"需求出生工程故障；不回滚真实前缀。");}
    if(!out.信息)out.完整保护.clear();return out;
}

新需求建立结果 新_需求类::建立或取得普通需求已核验(
    const 新普通需求建立请求& 请求,
    bool 允许任务子目标, const std::vector<基础联合节点快照>& 复合采用保护) noexcept {
    if(请求.目标.新增成员来源见证)return {新需求操作状态::入口拒绝,{},{}};
    if(请求.目标.引用内容定位)
        return 建立或取得引用内容需求内部(请求,允许任务子目标,复合采用保护);
    if(!复合采用保护.empty())return {新需求操作状态::入口拒绝,{},{}};
    return 建立或取得普通需求内部(请求, 允许任务子目标, std::nullopt);
}

新需求建立结果 新_需求类::建立或取得复合提议需求已核验(
    const 新普通需求建立请求& q, const 引用内容提议读取& 批) noexcept {
    if(!批.需求 || !q.复合来源 || !q.目标.引用内容定位 || q.目标.新增成员来源见证
        || 批.需求->节点!=q.父需求节点
        || 批.需求->目标存在节点!=q.复合来源->父目标存在节点
        || 批.需求->目标!=q.复合来源->父目标
        || 批.源前提.字段集合组.empty())return {新需求操作状态::入口拒绝,{},{}};
    return 建立或取得引用内容需求内部(q,true,批.判定.完整保护组,&批.源前提);
}

新需求建立结果 新_需求类::建立或取得归属分解需求已核验(const 新普通需求建立请求& q) noexcept {
    if(!q.复合来源||!是存在归属分解(*q.复合来源)||q.目标.引用内容定位||q.目标.新增成员来源见证
        ||q.目标.目标形式!=新需求目标形式::值)return {新需求操作状态::入口拒绝,{},{}};
    return 建立或取得引用内容需求内部(q,true,{},nullptr);
}

新需求建立结果 新_需求类::建立或取得普通需求内部(
    const 新普通需求建立请求& 请求, bool 允许任务子目标,
    std::optional<稳定编码> 初始服务要求确认,
    std::optional<稳定编码> 初始服务需求取得执行节点) noexcept {
    新需求建立结果 已成立前缀;
    try { return 建立或取得普通需求来源可抛内部(请求,允许任务子目标,
        初始服务要求确认,初始服务需求取得执行节点,已成立前缀); }
    catch (const 需求物理读取失败& e) {
        if(!已成立前缀.需求节点 && e.原因==基础结构操作状态::资源失败)
            return {新需求操作状态::资源失败,{},{}};
    } catch (const std::bad_alloc&) {
        if(!已成立前缀.需求节点)return {新需求操作状态::资源失败,{},{}};
    } catch (const std::length_error&) {
        if(!已成立前缀.需求节点)return {新需求操作状态::资源失败,{},{}};
    } catch (新需求目标定义核验状态 s) {
        // 旧普通消费者仍由原入口消费：仅空前缀保留旧资源返回的兼容债。
        if(!已成立前缀.需求节点&&s==新需求目标定义核验状态::前提已变化)
            return {新需求操作状态::资源失败,{},{}};
    } catch (...) {}
    工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"建立普通需求",L"普通出生工程故障或已成立后读回失败；保留事实。");
}

新需求建立结果 新_需求类::建立或取得普通需求来源可抛内部(
    const 新普通需求建立请求& 请求,bool 允许任务子目标,
    std::optional<稳定编码> 初始服务要求确认,
    std::optional<稳定编码> 初始服务需求取得执行节点,新需求建立结果& 已成立前缀,
    const std::vector<基础联合节点快照>& 新增目标保护) {
    if(请求.目标.新增成员来源见证&&(新增目标保护.empty()||初始服务要求确认||初始服务需求取得执行节点))
        return {新需求操作状态::入口拒绝,{},{}};
    if(请求.目标.引用内容定位) {
        if(初始服务要求确认||初始服务需求取得执行节点)return {新需求操作状态::入口拒绝,{},{}};
        return 建立或取得引用内容需求内部(请求,允许任务子目标);
    }
    if(请求.复合来源)return {新需求操作状态::入口拒绝,{},{}};
    新需求建立结果 结果;
    std::unique_lock<std::recursive_mutex> 锁;
    稳定编码 需求节点;
    std::vector<稳定编码> 需求字段组;
    std::optional<稳定编码> 父子关系, 列表成员关系, 新列表节点, 新列表登记字段;
    std::vector<稳定编码> 新列表字段组;
    std::vector<需求权重修改项> 权重修改组;
    std::optional<稳定编码> 同次已有;
    bool 权重已尝试 = false;
    const auto 清理节点 = [](稳定编码 节点) noexcept {
        if (!有效(节点)) return true;
        const auto 结构 = 全局基础数据集.查询完整节点结构(节点);
        if (结构.状态 == 基础结构操作状态::节点不存在) return true;
        if (结构.状态 != 基础结构操作状态::已完成 || !结构.结构) return false;
        bool 成功 = true;
        for (const auto& f : 结构.结构->字段组)
            if (!全局基础数据集.删除字段(f.编码)) 成功 = false;
        if (!全局基础数据集.删除节点(节点)) 成功 = false;
        return 成功;
    };
    const auto 回滚 = [&]() noexcept {
        bool 成功 = true;
        if (权重已尝试) for (auto i = 权重修改组.rbegin(); i != 权重修改组.rend(); ++i)
            if (!全局基础数据集.修改字段值(i->字段关系, 基础值{i->原权重})) 成功 = false;
        if (列表成员关系 && !全局基础数据集.删除字段(*列表成员关系)) 成功 = false;
        if (父子关系 && !全局基础数据集.删除关系(*父子关系)) 成功 = false;
        if (新列表登记字段 && !全局基础数据集.删除字段(*新列表登记字段)) 成功 = false;
        if (!清理节点(需求节点)) 成功 = false;
        if (新列表节点 && !清理节点(*新列表节点)) 成功 = false;
        if (!成功)
            工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"建立普通需求",L"成立边界前安全清理失败，保留现场。");
        return 成功;
    };
    const auto 补齐目标实例 = [&](稳定编码 D) {
        if(!目标特征实例承载已启用_)return;
        const auto s=确保目标特征实例承载可抛内部(D);
        if(s==新需求操作状态::资源失败)throw 需求物理读取失败{基础结构操作状态::资源失败};
        if(s==新需求操作状态::前提已变化)throw 新需求目标定义核验状态::前提已变化;
        要求需求物理(s==新需求操作状态::已修改||s==新需求操作状态::无变化);
    };
    try {
        锁 = std::unique_lock<std::recursive_mutex>{新需求互斥};
        if (!结构仍存在()) {
            结果.状态 = 新需求操作状态::尚未初始化;
            return 结果;
        }
        if (初始服务需求取得执行节点) {
            要求需求物理(初始服务要求确认.has_value()
                && 有效(*初始服务需求取得执行节点)
                && 请求.来源类型 == 新需求来源类型::任务子目标);
            (void)读取需求物理节点(*初始服务需求取得执行节点);
            const auto 来源=查询服务需求取得来源可抛内部(*初始服务需求取得执行节点);
            if(来源.状态==新服务要求采用关系状态::资源失败)
                throw 需求物理读取失败{基础结构操作状态::资源失败};
            要求需求物理(来源.状态==新服务要求采用关系状态::已读取&&来源.需求节点组.size()<=1);
            if(!来源.需求节点组.empty())同次已有=来源.需求节点组.front();
        }
        const auto 来源类型值 = static_cast<std::int64_t>(请求.来源类型);
        if (!转换来源类型(来源类型值)
            || 请求.来源类型 == 新需求来源类型::外部服务要求
            || 请求.来源类型 == 新需求来源类型::行动请求
            || (请求.来源类型 == 新需求来源类型::本能定义
                && 请求.来源承接节点)
            || (请求.来源类型 == 新需求来源类型::任务子目标
                && (!请求.来源承接节点
                    || !有效(*请求.来源承接节点)))) {
            结果.状态 = 新需求操作状态::入口拒绝;
            return 结果;
        }
        const auto 父需求 = 读取需求信息(需求目标读取上下文{特征服务_, 特征概念服务_}, 请求.父需求节点);
        if (!父需求) {
            结果.状态 = 新需求操作状态::父需求不合法;
            return 结果;
        }
        if (!存在服务_.是存在节点(请求.目标存在节点)) {
            结果.状态 = 新需求操作状态::目标存在不合法;
            return 结果;
        }
        if (!目标形状合法(请求.目标)) {
            结果.状态 = 新需求操作状态::目标形式不合法;
            return 结果;
        }
        if (!特征概念服务_.是特征概念节点(
                请求.目标.目标特征类型概念节点)) {
            结果.状态 = 新需求操作状态::目标特征类型不合法;
            return 结果;
        }
        if (请求.目标.目标形式 == 新需求目标形式::值) {
            if (!特征概念服务_.是特征概念节点(
                    *请求.目标.目标值域概念节点)) {
                结果.状态 = 新需求操作状态::目标值域不合法;
                return 结果;
            }
            if (!二次特征概念服务_.是二次特征概念(
                    *请求.目标.目标比较关系概念节点)) {
                结果.状态 = 新需求操作状态::比较关系不合法;
                return 结果;
            }
            const auto 类型根 = 取得特征类型根(
                *请求.目标.目标值域概念节点, 特征概念服务_);
            if (!类型根 || *类型根 != 请求.目标.目标特征类型概念节点) {
                结果.状态 = 新需求操作状态::目标值域不合法;
                return 结果;
            }
            const auto 比较关系 = 二次特征概念服务_.获取二次特征概念(
                *请求.目标.目标比较关系概念节点);
            if (!比较关系 || !比较关系适用于目标(*比较关系, *类型根,
                    *请求.目标.目标值域概念节点, 特征概念服务_)) {
                结果.状态 = 新需求操作状态::比较关系不合法;
                return 结果;
            }
        }

        if (请求.来源类型 == 新需求来源类型::任务子目标
            && !允许任务子目标) {
            结果.状态 = 新需求操作状态::任务来源未接通;
            return 结果;
        }

        std::optional<新需求列表项信息> 列表项;
        for (const auto 候选 : 读取多节点字段(
            需求树根节点, 需求列表项字段)) {
            const auto 信息 = 读取列表项信息(候选);
            if (!信息) {
                结果.状态 = 新需求操作状态::结构不一致;
                return 结果;
            }
            if (信息->根角色) continue;
            if (信息->目标存在节点 == 请求.目标存在节点
                && 目标相同(信息->目标, 请求.目标)) {
                if (列表项) {
                    结果.状态 = 新需求操作状态::结构不一致;
                    return 结果;
                }
                列表项 = *信息;
            }
        }
        if (列表项) {
            for (const auto 候选 : 列表项->需求节点组) {
                const auto 信息 = 读取需求信息(需求目标读取上下文{特征服务_, 特征概念服务_}, 候选);
                if (!信息 || 信息->根角色 || !信息->父需求节点
                    || !信息->来源类型) {
                    结果.状态 = 新需求操作状态::结构不一致;
                    return 结果;
                }
                if (*信息->父需求节点 == 请求.父需求节点
                    && *信息->来源类型 == 请求.来源类型
                    && 信息->来源承接节点 == 请求.来源承接节点) {
                    结果.状态 = 新需求操作状态::已复用;
                    结果.需求节点 = 信息->节点;
                    结果.需求列表项节点 = 列表项->节点;
                    要求需求物理(!同次已有||同次已有==信息->节点);
                    已成立前缀=结果;
                    补齐目标实例(信息->节点);
                    return 结果;
                }
            }
        }

        要求需求物理(!同次已有);
        if(请求.目标.新增成员来源见证) {
            using S=新需求操作状态;
            const auto& c=*请求.目标.新增成员来源见证;
            基础节点组追加请求 batch;batch.预期联合节点组=新增目标保护;
            if(!需求内容保护未变(batch.预期联合节点组))return {S::前提已变化,{},{}};
            std::optional<稳定编码> cw;
            for(const auto id:读取多节点字段(需求树根节点,新增成员见证登记字段))
                if(解码新增成员约束值(id)==请求.目标.新增成员来源见证){要求需求物理(!cw);cw=id;}
            std::set<稳定编码> ids{需求树根节点,请求.父需求节点,*请求.来源承接节点};
            if(列表项)ids.insert(列表项->节点);if(cw)ids.insert(*cw);
            auto children=查询子需求关系(请求.父需求节点);
            std::sort(children.begin(),children.end(),[](const auto& a,const auto& b){return a.目标节点<b.目标节点;});
            std::vector<新需求信息> ds;std::size_t active=1;
            for(const auto& edge:children){const auto d=读取需求信息(需求目标读取上下文{特征服务_,特征概念服务_},edge.目标节点);
                要求需求物理(d&&d->父需求节点==请求.父需求节点);ds.push_back(*d);if(d->活动状态!=新需求活动状态::失活)++active;}
            const auto share=父需求->权重/static_cast<std::int64_t>(active);
            auto rem=父需求->权重%static_cast<std::int64_t>(active);
            std::vector<需求权重修改项> changes;
            for(const auto& d:ds){要求需求物理(ids.insert(d.节点).second);
                const auto weight=d.活动状态==新需求活动状态::失活 ? 0 : share+(rem>0?(--rem,1):0);
                要求需求物理(追加需求权重修改(d.节点,weight,changes)
                    &&构建子树权重修改(需求目标读取上下文{特征服务_,特征概念服务_},d.节点,weight,{},ids,changes));}
            for(const auto& w:changes)batch.替换字段组.push_back({w.字段关系,基础值{w.新权重}});
            const auto snapshots=全局基础数据集.查询节点组完整结构({ids.begin(),ids.end()});
            if(snapshots.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            要求需求物理(snapshots.状态==基础结构操作状态::已完成&&snapshots.节点组.size()==ids.size());
            for(const auto& n:snapshots.节点组){if(!n.快照||!并需求保护(batch.预期联合节点组,{*n.快照}))return {S::前提已变化,{},{}};}
            const auto addTarget=[&](auto& fields){fields.push_back({目标存在字段,请求.目标存在节点});
                fields.push_back({目标特征类型字段,请求.目标.目标特征类型概念节点});
                fields.push_back({目标形式字段,基础值{static_cast<std::int64_t>(请求.目标.目标形式)}});
                fields.push_back({目标值域字段,*请求.目标.目标值域概念节点});
                fields.push_back({目标比较关系字段,*请求.目标.目标比较关系概念节点});};
            基础组内新节点 node;node.字段组={{节点类型字段,需求节点类型},{需求主体字段,父需求->需求主体存在节点},
                {需求执行域字段,基础值{static_cast<std::int64_t>(新需求执行域::自我执行)}},
                {来源类型字段,基础值{static_cast<std::int64_t>(请求.来源类型)}},{来源承接字段,*请求.来源承接节点},
                {活动状态字段,基础值{static_cast<std::int64_t>(新需求活动状态::活动)}},{需求权重字段,基础值{share+(rem>0?1:0)}}};
            addTarget(node.字段组);batch.新节点组.push_back(std::move(node));
            std::optional<std::size_t> listIndex;
            if(列表项)batch.追加字段组.push_back({列表项->节点,列表成员字段,基础组内节点引用{0}});
            else {listIndex=batch.新节点组.size();基础组内新节点 list;
                list.字段组={{节点类型字段,需求列表项节点类型},{列表成员字段,基础组内节点引用{0}}};addTarget(list.字段组);
                batch.新节点组.push_back(std::move(list));batch.追加字段组.push_back({需求树根节点,需求列表项字段,基础组内节点引用{*listIndex}});}
            const auto addCw=[&](auto value){batch.新节点组[0].字段组.push_back({目标新增成员见证字段,value});
                if(listIndex)batch.新节点组[*listIndex].字段组.push_back({目标新增成员见证字段,value});};
            if(cw)addCw(*cw);
            else {const auto index=batch.新节点组.size();基础组内新节点 value;
                value.字段组={{节点类型字段,新增成员见证类型},{新增成员格式字段,基础值{std::int64_t{1}}},
                    {新增成员来源种类字段,基础值{std::int64_t{1}}},{新增成员源类型字段,c.要求.源特征类型概念节点},{新增成员基准字段,c.基准材料节点}};
                if(c.要求.指定来源主体)value.字段组.push_back({新增成员指定主体字段,*c.要求.指定来源主体});
                if(c.要求.指定被认知需求)value.字段组.push_back({新增成员指定需求字段,*c.要求.指定被认知需求});
                batch.新节点组.push_back(std::move(value));addCw(基础组内节点引用{index});
                batch.追加字段组.push_back({需求树根节点,新增成员见证登记字段,基础组内节点引用{index}});}
            batch.追加外部关系组.push_back({请求.父需求节点,基础组内节点引用{0},基础外部关系类型::父子,需求父子关系角色});
            const auto committed=全局基础数据集.原子追加节点组(batch);
            if(committed.状态==基础节点组追加状态::预期不符)return {S::前提已变化,{},{}};
            if(committed.状态==基础节点组追加状态::资源失败)throw std::bad_alloc{};
            要求需求物理(committed.状态==基础节点组追加状态::已完成&&committed.新节点组.size()==batch.新节点组.size());
            已成立前缀={S::已建立,committed.新节点组.front(),列表项 ? std::optional{列表项->节点} : std::optional{committed.新节点组[*listIndex]}};
            补齐目标实例(*已成立前缀.需求节点);
            const auto read=获取需求(*已成立前缀.需求节点);要求需求物理(read&&read->目标==请求.目标);
            const 关联检查离域通知 notice{关联检查通知_};return 已成立前缀;
        }
        基础节点建立请求 初始请求;
        if (初始服务要求确认) {
            初始请求.字段组 = {
                {节点类型字段, 需求节点类型}, {目标存在字段, 请求.目标存在节点},
                {需求主体字段, 父需求->需求主体存在节点},
                {需求执行域字段, 基础值{static_cast<std::int64_t>(新需求执行域::自我执行)}},
                {目标特征类型字段, 请求.目标.目标特征类型概念节点},
                {目标形式字段, 基础值{static_cast<std::int64_t>(请求.目标.目标形式)}},
                {来源类型字段, 基础值{来源类型值}},
                {活动状态字段, 基础值{static_cast<std::int64_t>(新需求活动状态::活动)}},
                {需求权重字段, 基础值{std::int64_t{0}}},
                {服务要求确认字段, *初始服务要求确认}};
            if (初始服务需求取得执行节点)
                初始请求.字段组.push_back({服务需求取得执行字段, *初始服务需求取得执行节点});
            if (请求.目标.目标值域概念节点) 初始请求.字段组.push_back({目标值域字段, *请求.目标.目标值域概念节点});
            if (请求.目标.目标比较关系概念节点) 初始请求.字段组.push_back({目标比较关系字段, *请求.目标.目标比较关系概念节点});
            if (请求.目标.目标变化方向) 初始请求.字段组.push_back({目标变化方向字段, 基础值{static_cast<std::int64_t>(*请求.目标.目标变化方向)}});
            if (请求.来源承接节点) 初始请求.字段组.push_back({来源承接字段, *请求.来源承接节点});
        }
        const auto 原子出生 = 初始服务要求确认 ? 全局基础数据集.原子建立节点(初始请求)
            : 基础节点建立结果{};
        需求节点 = 初始服务要求确认 ? 原子出生.节点.value_or(稳定编码{})
            : 全局基础数据集.新建节点();
        if (!有效(需求节点)) {
            结果.状态 = !初始服务要求确认 || 原子出生.状态 == 基础结构操作状态::资源失败
                ? 新需求操作状态::资源失败 : 新需求操作状态::结构不一致;
            return 结果;
        }
        const auto 加需求节点字段 = [&](稳定编码 字段, 稳定编码 目标) {
            if (初始服务要求确认) return true; // 完整身份及确认来源已一次出生。
            const auto 关系 = 全局基础数据集.添加字段节点(
                需求节点, 字段, 目标);
            if (有效(关系)) 需求字段组.push_back(关系);
            return 有效(关系);
        };
        const auto 加需求I64字段 = [&](稳定编码 字段, std::int64_t 值) {
            if (初始服务要求确认) return true;
            const auto 关系 = 全局基础数据集.添加字段值(
                需求节点, 字段, 基础值{值});
            if (有效(关系)) 需求字段组.push_back(关系);
            return 有效(关系);
        };
        if (!加需求节点字段(节点类型字段, 需求节点类型)
            || !加需求节点字段(需求主体字段, 父需求->需求主体存在节点)
            || !加需求I64字段(需求执行域字段, static_cast<std::int64_t>(新需求执行域::自我执行))
            || !加需求节点字段(目标存在字段, 请求.目标存在节点)
            || !加需求节点字段(目标特征类型字段,
                请求.目标.目标特征类型概念节点)
            || !加需求I64字段(目标形式字段,
                static_cast<std::int64_t>(请求.目标.目标形式))
            || !加需求I64字段(来源类型字段, 来源类型值)
            || !加需求I64字段(活动状态字段,
                static_cast<std::int64_t>(新需求活动状态::活动))
            || !加需求I64字段(需求权重字段, 0)) {
            结果.状态 = 回滚() ? 新需求操作状态::资源失败 : 新需求操作状态::结构不一致;
            return 结果;
        }
        if (请求.目标.目标值域概念节点
            && !加需求节点字段(目标值域字段,
                *请求.目标.目标值域概念节点)) {
            结果.状态 = 回滚() ? 新需求操作状态::资源失败 : 新需求操作状态::结构不一致;
            return 结果;
        }
        if (请求.目标.目标比较关系概念节点
            && !加需求节点字段(目标比较关系字段,
                *请求.目标.目标比较关系概念节点)) {
            结果.状态 = 回滚() ? 新需求操作状态::资源失败 : 新需求操作状态::结构不一致;
            return 结果;
        }
        if (请求.目标.目标变化方向
            && !加需求I64字段(目标变化方向字段,
                static_cast<std::int64_t>(*请求.目标.目标变化方向))) {
            结果.状态 = 回滚() ? 新需求操作状态::资源失败 : 新需求操作状态::结构不一致;
            return 结果;
        }
        if (请求.来源承接节点
            && !加需求节点字段(
                来源承接字段, *请求.来源承接节点)) {
            结果.状态 = 回滚() ? 新需求操作状态::资源失败 : 新需求操作状态::结构不一致;
            return 结果;
        }
        const auto 已建父子关系 = 全局基础数据集.添加关系(
            请求.父需求节点, 需求节点,
            基础外部关系类型::父子, 需求父子关系角色);
        if (!有效(已建父子关系)) {
            结果.状态 = 回滚() ? 新需求操作状态::资源失败 : 新需求操作状态::结构不一致;
            return 结果;
        }
        父子关系 = 已建父子关系;

        稳定编码 列表节点{};
        if (列表项) {
            列表节点 = 列表项->节点;
        } else {
            列表节点 = 全局基础数据集.新建节点();
            if (!有效(列表节点)) {
                结果.状态 = 回滚() ? 新需求操作状态::资源失败 : 新需求操作状态::结构不一致;
                return 结果;
            }
            新列表节点 = 列表节点;
            const auto 加列表节点字段 = [&](稳定编码 字段, 稳定编码 目标) {
                const auto 关系 = 全局基础数据集.添加字段节点(
                    列表节点, 字段, 目标);
                if (有效(关系)) 新列表字段组.push_back(关系);
                return 有效(关系);
            };
            if (!加列表节点字段(节点类型字段, 需求列表项节点类型)
                || !加列表节点字段(目标存在字段, 请求.目标存在节点)
                || !加列表节点字段(目标特征类型字段,
                    请求.目标.目标特征类型概念节点)) {
                结果.状态 = 回滚() ? 新需求操作状态::资源失败 : 新需求操作状态::结构不一致;
                return 结果;
            }
            const auto 形式关系 = 全局基础数据集.添加字段值(
                列表节点, 目标形式字段,
                基础值{static_cast<std::int64_t>(请求.目标.目标形式)});
            if (!有效(形式关系)) {
                结果.状态 = 回滚() ? 新需求操作状态::资源失败 : 新需求操作状态::结构不一致;
                return 结果;
            }
            新列表字段组.push_back(形式关系);
            if (请求.目标.目标值域概念节点
                && !加列表节点字段(目标值域字段,
                    *请求.目标.目标值域概念节点)) {
                结果.状态 = 回滚() ? 新需求操作状态::资源失败 : 新需求操作状态::结构不一致;
                return 结果;
            }
            if (请求.目标.目标比较关系概念节点
                && !加列表节点字段(目标比较关系字段,
                    *请求.目标.目标比较关系概念节点)) {
                结果.状态 = 回滚() ? 新需求操作状态::资源失败 : 新需求操作状态::结构不一致;
                return 结果;
            }
            if (请求.目标.目标变化方向) {
                const auto 方向关系 = 全局基础数据集.添加字段值(
                    列表节点, 目标变化方向字段,
                    基础值{static_cast<std::int64_t>(
                        *请求.目标.目标变化方向)});
                if (!有效(方向关系)) {
                    结果.状态 = 回滚() ? 新需求操作状态::资源失败 : 新需求操作状态::结构不一致;
                    return 结果;
                }
                新列表字段组.push_back(方向关系);
            }
            const auto 登记 = 全局基础数据集.添加字段节点(
                需求树根节点, 需求列表项字段, 列表节点);
            if (!有效(登记)) {
                结果.状态 = 回滚() ? 新需求操作状态::资源失败 : 新需求操作状态::结构不一致;
                return 结果;
            }
            新列表登记字段 = 登记;
        }
        const auto 成员 = 全局基础数据集.添加字段节点(
            列表节点, 列表成员字段, 需求节点);
        if (!有效(成员)) {
            结果.状态 = 回滚() ? 新需求操作状态::资源失败 : 新需求操作状态::结构不一致;
            return 结果;
        }
        列表成员关系 = 成员;

        if (!读取需求信息(需求目标读取上下文{特征服务_, 特征概念服务_}, 需求节点) || !读取列表项信息(列表节点)) {
            回滚();
            结果.状态 = 新需求操作状态::结构不一致;
            return 结果;
        }
        if (初始服务要求确认) {
            const auto 读回 = 读取需求完整物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 需求节点);
            要求需求物理(读回.服务要求确认组 == std::vector<稳定编码>{*初始服务要求确认}
                && 读回.目标存在节点 == 请求.目标存在节点 && 读回.目标 == 请求.目标
                && 读回.父需求节点 == 请求.父需求节点 && 读回.来源类型 == 请求.来源类型
                && 读回.来源承接节点 == 请求.来源承接节点
                && 读回.服务需求取得执行节点 == 初始服务需求取得执行节点);
            要求需求物理(核验需求列表物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 读回) == 列表节点);
        }
        std::set<稳定编码> 已访问{请求.父需求节点};
        if (!构建子树权重修改(需求目标读取上下文{特征服务_, 特征概念服务_}, 请求.父需求节点, 父需求->权重,
                std::nullopt, 已访问, 权重修改组)) {
            回滚();
            结果.状态 = 新需求操作状态::结构不一致;
            return 结果;
        }
        权重已尝试 = true;
        const auto 权重状态 = 应用需求权重修改(权重修改组);
        if (权重状态 != 需求权重写入状态::成功) {
            const auto 已回滚 = 回滚();
            结果.状态 = 已回滚 && 权重状态 == 需求权重写入状态::资源失败
                ? 新需求操作状态::资源失败
                : 新需求操作状态::结构不一致;
            return 结果;
        }

        结果.状态 = 新需求操作状态::已建立;
        结果.需求节点 = 需求节点;
        结果.需求列表项节点 = 列表节点;
        已成立前缀=结果; // D/list/父子/来源与权重均已成立，随后失败不得回滚。
        if (初始服务要求确认) {
            const auto 读回 = 读取需求完整物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 需求节点);
            要求需求物理(读回.服务需求取得执行节点 == 初始服务需求取得执行节点);
            要求需求物理(核验需求列表物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 读回) == 列表节点);
        }

        const 关联检查离域通知 变更通知{关联检查通知_};
        补齐目标实例(需求节点);
        return 结果;
    } catch (...) {
        if(!已成立前缀.需求节点&&!回滚())
            工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"建立普通需求",L"成立边界前安全清理失败，保留现场。");
        throw;
    }
}

void 新_需求类::装配服务要求确认读取接口(
    const 新服务要求确认读取接口* 接口) noexcept {
    // 调用方保证启动前/全部join后的生命周期边界；禁止运行期替换。
    if (!接口 || !服务要求确认读取接口_ || 服务要求确认读取接口_ == 接口)
        服务要求确认读取接口_ = 接口;
}

新部分服务要求读取结果 新_需求类::读取部分服务要求(
    稳定编码 要求节点) const noexcept {
    using S = 新部分服务要求读取状态;
    try {
        if (!有效(要求节点)) return {S::材料不存在, {}, {}};
        要求需求物理(部分服务要求读取接口_ != nullptr);
        auto 结果 = 部分服务要求读取接口_->读取部分服务要求(要求节点);
        if (结果.状态 == S::材料不存在) {
            要求需求物理(!结果.材料);
            return 结果;
        }
        要求需求物理(结果.状态 == S::已读取 && 结果.材料
            && 结果.材料->要求节点 == 要求节点 && 有效(结果.材料->来源人类节点)
            && !结果.发布预期节点组.empty());
        return 结果;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取部分服务要求", L"正式来源材料读取或身份核验故障");
    }
}

新服务要求确认读取结果 新_需求类::读取服务要求确认身份(
    稳定编码 确认节点) const noexcept {
    using S = 新服务要求确认状态;
    try {
        if (!服务要求确认读取接口_) return {S::未装配, {}};
        const auto 结果 = 服务要求确认读取接口_->读取服务要求确认身份(确认节点);
        if (结果.状态 == S::已读取) {
            要求需求物理(结果.确认 && 有效(确认节点) && 结果.确认->确认节点 == 确认节点
                && 有效(结果.确认->理解节点) && 有效(结果.确认->来源人类节点)
                && 有效(结果.确认->目标存在节点) && 目标形状合法(结果.确认->目标));
        } else {
            要求需求物理(结果.状态 == S::未装配 || 结果.状态 == S::确认不存在
                || 结果.状态 == S::结构不一致 || 结果.状态 == S::资源失败);
        }
        return 结果;
    } catch (const std::bad_alloc&) { return {S::资源失败, {}}; }
    catch (...) { return {S::结构不一致, {}}; }
}

新服务要求确认保护结果 新_需求类::准备服务要求确认语义保护(
    稳定编码 确认节点) const noexcept {
    if (!服务要求确认读取接口_)
        return {新服务要求确认保护状态::未装配, {}};
    return 服务要求确认读取接口_->准备服务要求确认语义保护(确认节点);
}

新服务要求确认读取结果 新_需求类::复核服务要求确认语义(
    稳定编码 确认节点) const noexcept {
    using S = 新服务要求确认状态;
    try {
        if (!服务要求确认读取接口_) return {S::未装配, {}};
        const auto 结果 = 服务要求确认读取接口_->复核服务要求确认语义(确认节点);
        if (结果.状态 == S::当前有效) {
            要求需求物理(结果.确认 && 有效(确认节点) && 结果.确认->确认节点 == 确认节点
                && 有效(结果.确认->理解节点) && 有效(结果.确认->来源人类节点)
                && 有效(结果.确认->目标存在节点) && 目标形状合法(结果.确认->目标));
        } else {
            要求需求物理(结果.状态 == S::需重新确认 || 结果.状态 == S::未装配
                || 结果.状态 == S::确认不存在 || 结果.状态 == S::结构不一致
                || 结果.状态 == S::资源失败);
        }
        return 结果;
    } catch (const std::bad_alloc&) { return {S::资源失败, {}}; }
    catch (...) { return {S::结构不一致, {}}; }
}

新需求操作状态 新_需求类::配置服务要求采用来源已核验(
    稳定编码 结算需求, 稳定编码 结算任务,
    const 特征任务成员读取接口& 任务读取) noexcept {
    try { return 配置服务要求采用来源可抛内部(结算需求,结算任务,任务读取); }
    catch (const 需求物理读取失败& e) {
        return e.原因 == 基础结构操作状态::资源失败 ? 新需求操作状态::资源失败 : 新需求操作状态::结构不一致;
    } catch (const std::bad_alloc&) { return 新需求操作状态::资源失败; }
    catch (...) { return 新需求操作状态::结构不一致; }
}

新需求操作状态 新_需求类::配置服务要求采用来源可抛内部(
    稳定编码 结算需求, 稳定编码 结算任务,
    const 特征任务成员读取接口& 任务读取) {
    {
        std::lock_guard 锁(新需求互斥);
        if (!结构已初始化()) return 新需求操作状态::尚未初始化;
        if (!有效(结算需求) || !有效(结算任务)) return 新需求操作状态::入口拒绝;
        if (服务要求任务读取接口_ && (服务要求任务读取接口_ != &任务读取
            || 服务要求结算需求_ != 结算需求 || 服务要求结算任务_ != 结算任务))
            return 新需求操作状态::入口拒绝;
        const auto 父 = 读取需求完整物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 结算需求), 根 = 读取需求完整物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 服务根需求节点);
        (void)核验需求列表物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 父); (void)核验需求列表物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 根);
        if (父.根角色 || 父.父需求节点 != 服务根需求节点 || 根.根角色 != 新需求根角色::服务)
            return 新需求操作状态::父需求不合法;
        const auto 对应 = 任务读取.读取任务所属需求(结算任务);
        if (对应.状态 == 特征任务成员读取状态::资源失败) return 新需求操作状态::资源失败;
        if (对应.状态 != 特征任务成员读取状态::已读取 || 对应.需求节点 != 结算需求)
            return 新需求操作状态::任务来源未接通;
        // 重复核验可能发生在运行期采用中；冻结配置不能再次写入，
        // 否则即使写回相同值，也会与无锁来源读取产生数据竞争。
        if (服务要求任务读取接口_) return 新需求操作状态::无变化;
        服务要求结算需求_ = 结算需求; 服务要求结算任务_ = 结算任务;
        服务要求任务读取接口_ = &任务读取;
        return 新需求操作状态::已修改;
    }
}

新服务要求采用关系结果 新_需求类::查询服务要求采用关系(稳定编码 确认节点) const noexcept {
    using S = 新服务要求采用关系状态;
    try { return 查询服务要求采用关系可抛内部(确认节点); }
    catch (const 需求物理读取失败& e) {
        return {e.原因 == 基础结构操作状态::资源失败 ? S::资源失败 : S::结构不一致, {}};
    } catch (const std::bad_alloc&) { return {S::资源失败, {}}; }
    catch (...) { return {S::结构不一致, {}}; }
}

新服务要求采用关系结果 新_需求类::查询服务要求采用关系可抛内部(稳定编码 确认节点) const {
    using S = 新服务要求采用关系状态;
    using C = 新服务要求确认状态;
    {
        if (!结构已初始化() || !服务要求确认读取接口_) return {S::未装配, {}};
        const auto 确认 = 服务要求确认读取接口_->读取服务要求确认身份(确认节点);
        if (确认.状态 == C::未装配) return {S::未装配, {}};
        if (确认.状态 == C::确认不存在) return {S::确认不存在, {}};
        if (确认.状态 == C::资源失败) return {S::资源失败, {}};
        要求需求物理(确认.状态 == C::已读取 && 确认.确认
            && 确认.确认->确认节点 == 确认节点);
        const auto 来源 = 全局基础数据集.查询字段引用节点结构(确认节点, 服务要求确认字段);
        if (来源.状态 != 基础结构操作状态::已完成) throw 需求物理读取失败{来源.状态};
        要求需求物理(来源.引用字段组.size() == 来源.来源节点组.size()
            && 来源.来源节点组.size() <= 1);
        if (来源.来源节点组.empty()) return {S::已读取, {}};
        if (!服务要求任务读取接口_ || !有效(服务要求结算需求_) || !有效(服务要求结算任务_))
            return {S::未装配, {}};
        const auto d = 解码需求物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 来源.来源节点组.front());
        要求需求物理(来源.引用字段组.front().所属节点 == d.节点
            && 来源.引用字段组.front().字段节点 == 服务要求确认字段
            && std::get_if<稳定编码>(&来源.引用字段组.front().内容)
            && std::get<稳定编码>(来源.引用字段组.front().内容) == 确认节点);
        const auto 治理 = 解码需求治理物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 来源.来源节点组.front());
        const bool 合法来源 = (d.来源类型 == 新需求来源类型::任务子目标 && d.来源承接节点 == 服务要求结算任务_)
            || (d.来源类型 == 新需求来源类型::外部服务要求 && 治理.外部要求节点
                && 确认.确认->来源要求节点 == 治理.外部要求节点 && !d.来源承接节点);
        要求需求物理(!d.根角色 && d.父需求节点 == 服务要求结算需求_
            && 合法来源
            && std::binary_search(d.服务要求确认组.begin(), d.服务要求确认组.end(), 确认节点));
        (void)核验需求列表物理(需求目标读取上下文{特征服务_, 特征概念服务_}, d);
        const auto 父 = 读取需求完整物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 服务要求结算需求_);
        const auto 根 = 读取需求完整物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 服务根需求节点);
        要求需求物理(!父.根角色 && 父.父需求节点 == 服务根需求节点 && 根.根角色 == 新需求根角色::服务);
        (void)核验需求列表物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 父); (void)核验需求列表物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 根);
        const auto 对应 = 服务要求任务读取接口_->读取任务所属需求(服务要求结算任务_);
        if (对应.状态 == 特征任务成员读取状态::资源失败) return {S::资源失败, {}};
        if (对应.状态 == 特征任务成员读取状态::未装配) return {S::未装配, {}};
        要求需求物理(对应.状态 == 特征任务成员读取状态::已读取
            && 对应.需求节点 == 服务要求结算需求_);
        // 每个引用都必须是完整正式确认；只比较固定含义，不捕获当前概念。
        for (const auto c : d.服务要求确认组) {
            const auto r = 服务要求确认读取接口_->读取服务要求确认身份(c);
            if (r.状态 == C::资源失败) return {S::资源失败, {}};
            if (r.状态 == C::未装配) return {S::未装配, {}};
            要求需求物理(r.状态 == C::已读取 && r.确认 && r.确认->确认节点 == c
                && r.确认->来源人类节点 == d.目标存在节点
                && r.确认->目标存在节点 == d.目标存在节点 && r.确认->目标 == d.目标);
            const auto 同义 = 服务要求确认读取接口_->比较服务要求确认固定语义(确认节点, c);
            if (同义.状态 == C::资源失败) return {S::资源失败, {}};
            if (同义.状态 == C::未装配) return {S::未装配, {}};
            要求需求物理(同义.状态 == C::已读取 && 同义.同义 && *同义.同义);
        }
        return {S::已读取, {d.节点}};
    }
}

新服务要求采用关系结果 新_需求类::查询服务需求取得来源(
    稳定编码 取得执行节点) const noexcept {
    using S = 新服务要求采用关系状态;
    try { return 查询服务需求取得来源可抛内部(取得执行节点); }
    catch (const 需求物理读取失败& e) {
        return {e.原因 == 基础结构操作状态::资源失败 ? S::资源失败 : S::结构不一致, {}};
    } catch (const std::bad_alloc&) { return {S::资源失败, {}}; }
    catch (...) { return {S::结构不一致, {}}; }
}

新服务要求采用关系结果 新_需求类::查询服务需求取得来源可抛内部(
    稳定编码 取得执行节点) const {
    using S = 新服务要求采用关系状态;
    {
        std::lock_guard 锁(新需求互斥);
        if (!结构已初始化() || !服务要求确认读取接口_ || !服务要求任务读取接口_
            || !有效(服务要求结算需求_) || !有效(服务要求结算任务_)) return {S::未装配, {}};
        要求需求物理(有效(取得执行节点));
        (void)读取需求物理节点(取得执行节点);
        auto 全集读取 = 查询全部需求列表可抛内部(std::stop_token{});
        const auto* 全集结果 = std::get_if<新需求列表全集结果>(&全集读取);
        要求需求物理(全集结果);
        const auto& 全集 = *全集结果;
        if (全集.状态 != 新需求操作状态::已找到)
            return {全集.状态 == 新需求操作状态::资源失败 ? S::资源失败 : S::结构不一致, {}};
        std::vector<稳定编码> 结果;
        for (const auto& 投影 : 全集.需求组) {
            if (投影.服务需求取得执行节点 != 取得执行节点) continue;
            const auto d = 读取需求完整物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 投影.节点);
            const bool 合法来源 = (d.来源类型 == 新需求来源类型::任务子目标
                && d.来源承接节点 == 服务要求结算任务_)
                || (d.来源类型 == 新需求来源类型::外部服务要求
                    && 投影.外部要求节点 && !d.来源承接节点);
            要求需求物理(投影.完整目标 && d == 投影完整需求(投影) && !d.根角色 && d.父需求节点 == 服务要求结算需求_
                && 合法来源
                && !d.服务要求确认组.empty());
            (void)核验需求列表物理(需求目标读取上下文{特征服务_, 特征概念服务_}, d);
            for (const auto 确认 : d.服务要求确认组) {
                const auto 采用 = 查询服务要求采用关系可抛内部(确认);
                if (采用.状态 == S::资源失败 || 采用.状态 == S::未装配) return 采用;
                要求需求物理(采用.状态 == S::已读取 && 采用.需求节点组 == std::vector<稳定编码>{d.节点});
            }
            结果.push_back(d.节点);
        }
        要求需求物理(结果.size() <= 1);
        // 只检查此强类型字段的入向闭包，防止半发布D被正式全集遗漏后盲建第二D。
        const auto 引用 = 全局基础数据集.查询字段引用节点结构(取得执行节点, 服务需求取得执行字段);
        if (引用.状态 != 基础结构操作状态::已完成) throw 需求物理读取失败{引用.状态};
        要求需求物理(引用.引用字段组.size() == 结果.size() && 引用.来源节点组.size() == 结果.size());
        if (!结果.empty()) {
            const auto& 字段 = 引用.引用字段组.front();
            const auto* 目标 = std::get_if<稳定编码>(&字段.内容);
            要求需求物理(字段.字段节点 == 服务需求取得执行字段 && 字段.所属节点 == 结果.front()
                && 目标 && *目标 == 取得执行节点
                && 引用.来源节点组.front().节点.编码 == 结果.front());
        }
        return {S::已读取, std::move(结果)};
    }
}

新服务要求执行守卫结果 新_需求类::复核需求服务要求语义(稳定编码 需求节点) const noexcept {
    return 复核需求服务要求语义内部(需求节点,nullptr);
}
新服务要求执行守卫结果 新_需求类::复核需求服务要求语义内部(
    稳定编码 需求节点,提议源收集批* 源) const noexcept {
    using S = 新服务要求执行守卫状态;
    using C = 新服务要求确认状态;
#ifdef HY_EGO_DEBUG_LOG_DEMAND_SOURCE_GUARD
    bool 有行动请求=false;
    const char* 诊断步骤="ENTER";
    auto 诊断需求=需求节点;
    std::size_t 诊断链深=0;
    需求守卫诊断内部::最近故障[0]=0;
    const auto 记录守卫故障=[&](const char* 分类,int 物理原因,unsigned 物理行) noexcept {
        (void)_snprintf_s(需求守卫诊断内部::最近故障,_TRUNCATE,
            ";guardOrigin=%s;step=%s;initialD=%llu;currentD=%llu;depth=%llu;requestSeen=%u;physical=%d;physicalLine=%u",
            分类,诊断步骤,static_cast<unsigned long long>(需求节点.值),
            static_cast<unsigned long long>(诊断需求.值),static_cast<unsigned long long>(诊断链深),
            有行动请求 ? 1u : 0u,物理原因,物理行);
    };
#define HY_EGO_DEMAND_GUARD_STEP(步骤,节点) do { 诊断步骤=步骤;诊断需求=节点; } while(false)
#define HY_EGO_DEMAND_GUARD_FAILURE(分类,原因,行) 记录守卫故障(分类,原因,行)
#define HY_EGO_DEMAND_GUARD_DEPTH() (++诊断链深)
#else
#define HY_EGO_DEMAND_GUARD_STEP(步骤,节点) ((void)0)
#define HY_EGO_DEMAND_GUARD_FAILURE(分类,原因,行) ((void)0)
#define HY_EGO_DEMAND_GUARD_DEPTH() ((void)0)
#endif
    try {
        if (!结构已初始化()) return {S::未装配, {}, {}};
        std::set<稳定编码> 已读, 确认集合;
        std::optional<稳定编码> 阻塞;
        std::vector<新需求信息> chain;
        std::vector<基础联合节点快照> protect;
        std::vector<新行动请求承接来源> requests;
        {
            // 只在本域锁内冻结父链及原始预期；一切语言/确认owner调用在此作用域之后。
            std::lock_guard lock(新需求互斥);
            auto id=需求节点;
            for(;;) {
                HY_EGO_DEMAND_GUARD_STEP("RAW_READ",id);
                const auto raw=全局基础数据集.查询节点组完整结构({id});
                if(raw.状态==基础结构操作状态::资源失败)return {S::资源失败,{},{}};
                要求需求物理(raw.状态==基础结构操作状态::已完成&&raw.节点组.size()==1);
                if(!raw.节点组.front().快照) {
                    if(chain.empty())return {S::需求不存在,{},{}};
                    throw 新需求目标定义核验状态::前提已变化;
                }
                const auto& n=*raw.节点组.front().快照;
                HY_EGO_DEMAND_GUARD_STEP("SOURCE_KIND",id);
#ifdef HY_EGO_DEBUG_LOG_DEMAND_SOURCE_GUARD
                for(const auto& 字段:n.结构.字段组) {
                    if(字段.字段节点!=来源类型字段)continue;
                    if(const auto* 值=std::get_if<基础值>(&字段.内容))
                        if(const auto* 类别=std::get_if<std::int64_t>(&值->材料))
                            if(*类别==static_cast<std::int64_t>(新需求来源类型::行动请求))有行动请求=true;
                }
#endif
                HY_EGO_DEMAND_GUARD_STEP("MERGE_RAW",id);
                if(!源&&!并需求保护(protect,{n})){
                    return {S::前提已变化,{},{}};
                }
                const auto 本源=源 ? 构造需求提议源前提(n) : 基础只读预期{};
                HY_EGO_DEMAND_GUARD_STEP("PROJECT_TARGET",id);
                const auto 目标投影=投影需求目标实例(需求目标读取上下文{特征服务_,特征概念服务_},n.结构,&protect,false,源 ? &本源 : nullptr);
                HY_EGO_DEMAND_GUARD_STEP("DECODE_DEMAND",id);
                const auto d=解码需求治理物理(需求目标读取上下文{特征服务_,特征概念服务_},目标投影);
                if(源) {
                    if(d.执行域==新需求执行域::自我执行) {
                        合并提议源前提(源->前提,本源);源->需求节点组.insert(id);
                    } else if(!并需求保护(protect,{n}))return {S::前提已变化,{},{}};
                }
                if(!d.完整目标||d.信息完善阶段==新需求信息完善阶段::待完善)return {S::需求信息待完善,id,{}};
                HY_EGO_DEMAND_GUARD_STEP("CHECK_CHAIN",id);
                要求需求物理(已读.insert(id).second);
                auto full=投影完整需求(d);
                HY_EGO_DEMAND_GUARD_STEP("CHECK_LIST",id);
                (void)核验需求列表物理(需求目标读取上下文{特征服务_,特征概念服务_},full);
                chain.push_back(std::move(full));
                HY_EGO_DEMAND_GUARD_DEPTH();
                if(d.根角色)break;
                HY_EGO_DEMAND_GUARD_STEP("PARENT_LINK",id);
                要求需求物理(d.父需求节点.has_value());id=*d.父需求节点;
            }
        }
        for(const auto& d:chain) {
            HY_EGO_DEMAND_GUARD_STEP("REQUEST_SOURCE",d.节点);
            if(d.行动请求来源) {
                const auto fresh=读取行动请求承接来源保护(*d.行动请求来源);
                if(fresh.状态==新行动请求来源读取状态::资源失败)return {S::资源失败,{},{}};
                if(fresh.状态==新行动请求来源读取状态::前提已变化)return {S::前提已变化,d.节点,{}};
                if(fresh.状态==新行动请求来源读取状态::依赖未就绪)return {S::未装配,{},{}};
                要求需求物理(fresh.状态==新行动请求来源读取状态::已读取&&fresh.信息
                    &&fresh.信息->目标==新需求完整目标{d.目标存在节点,d.目标}
                    &&d.父需求节点==服务要求结算需求_&&d.需求主体存在节点==d.行动请求来源->受请求者节点);
                // 同U的唯一Ds也纳入保护；后代的目标无需等于祖先Ds的目标。
                const auto adopted=查询行动请求自我需求内部(*d.行动请求来源,源);
                if(adopted.状态==新需求操作状态::资源失败)return {S::资源失败,{},{}};
                if(adopted.状态==新需求操作状态::前提已变化)return {S::前提已变化,d.节点,{}};
                要求需求物理(adopted.状态==新需求操作状态::已找到&&adopted.信息&&adopted.信息->节点==d.节点);
                if(!并需求保护(protect,fresh.完整保护)||!并需求保护(protect,adopted.完整保护))return {S::前提已变化,d.节点,{}};
                requests.push_back(*d.行动请求来源);
            }
            for (const auto c : d.服务要求确认组) {
                HY_EGO_DEMAND_GUARD_STEP("OLD_CONFIRMATION",d.节点);
                if (!服务要求确认读取接口_) return {S::未装配, {}, {}};
                const auto 采用 = 查询服务要求采用关系可抛内部(c);
                if (采用.状态 == 新服务要求采用关系状态::资源失败) return {S::资源失败, {}, {}};
                if (采用.状态 == 新服务要求采用关系状态::未装配) return {S::未装配, {}, {}};
                要求需求物理(采用.状态 == 新服务要求采用关系状态::已读取
                    && 采用.需求节点组 == std::vector<稳定编码>{d.节点});
                const auto r = 服务要求确认读取接口_->复核服务要求确认语义(c);
                if (r.状态 == C::资源失败) return {S::资源失败, {}, {}};
                if (r.状态 == C::未装配) return {S::未装配, {}, {}};
                要求需求物理((r.状态 == C::当前有效 || r.状态 == C::需重新确认) && r.确认
                    && r.确认->确认节点 == c && r.确认->来源人类节点 == d.目标存在节点
                    && r.确认->目标存在节点 == d.目标存在节点 && r.确认->目标 == d.目标);
                确认集合.insert(c);
                if (r.状态 == C::需重新确认 && !阻塞) 阻塞 = d.节点;
            }
        }
        HY_EGO_DEMAND_GUARD_STEP("DEDUP_REQUESTS",需求节点);
        std::sort(requests.begin(),requests.end(),[](const auto& a,const auto& b){return a.需求来源.采用含义节点<b.需求来源.采用含义节点;});
        for(std::size_t i=1;i<requests.size();++i)if(requests[i-1].需求来源.采用含义节点==requests[i].需求来源.采用含义节点)
            要求需求物理(requests[i-1]==requests[i]);
        requests.erase(std::unique(requests.begin(),requests.end()),requests.end());
        if(!核同提议源(源))return {S::前提已变化,{},{}};
        if(requests.empty()) {
            if(确认集合.empty())return {S::不涉及,{},{}};
            // 旧确认的语义owner未提供联合保护，不能把父链快照伪装成其完整保护。
            return {阻塞 ? S::需重新确认 : S::当前有效,阻塞,{确认集合.begin(),确认集合.end()}};
        }
        if(!需求内容保护未变(protect))return {S::前提已变化,{},{}};
        return {阻塞 ? S::需重新确认 : S::当前有效, 阻塞,
            std::vector<稳定编码>(确认集合.begin(), 确认集合.end()),std::move(requests),std::move(protect)};
    } catch (新需求目标定义核验状态 s) {
        if(s!=新需求目标定义核验状态::前提已变化)
            工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"复核需求服务来源",L"未知目标核验异常。");
        return {S::前提已变化,{},{}};
    } catch (const 需求物理读取失败& e) {
        if(e.原因!=基础结构操作状态::资源失败)
            HY_EGO_DEMAND_GUARD_FAILURE("PHYSICAL_READ_FAILURE",static_cast<int>(e.原因),e.诊断行);
        return {e.原因 == 基础结构操作状态::资源失败 ? S::资源失败 : S::结构不一致, {}, {}};
    } catch (const std::bad_alloc&) { return {S::资源失败, {}, {}}; }
    catch (const std::length_error&) { return {S::资源失败, {}, {}}; }
    catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"复核需求服务来源",L"正式父链或来源结构损坏"); }
}
#undef HY_EGO_DEMAND_GUARD_STEP
#undef HY_EGO_DEMAND_GUARD_FAILURE
#undef HY_EGO_DEMAND_GUARD_DEPTH

新服务要求需求建立结果 新_需求类::建立或取得服务要求需求已核验(
    稳定编码 确认节点) noexcept {
    return 建立或取得服务要求需求内部(确认节点, std::nullopt);
}

新服务要求需求建立结果 新_需求类::建立或取得服务要求需求已核验(
    稳定编码 确认节点, 稳定编码 取得执行节点) noexcept {
    if (!有效(取得执行节点)) return {新服务要求需求建立状态::结构不一致, {}};
    return 建立或取得服务要求需求内部(确认节点, 取得执行节点);
}

新服务要求需求建立结果 新_需求类::建立或取得服务要求需求内部(
    稳定编码 确认节点, std::optional<稳定编码> 取得执行节点) noexcept {
    using S = 新服务要求需求建立状态;
    using C = 新服务要求确认状态;
    新服务要求需求建立结果 结果;
    结果.建立结果.emplace();
    auto& 前缀=*结果.建立结果;
    const auto 返回=[&](S s) { 结果.状态=s;return 结果; };
    try {
        std::lock_guard 锁(新需求互斥);
        if (!结构已初始化() || !服务要求确认读取接口_ || !服务要求任务读取接口_) return 结果;
        std::optional<新需求信息> 同次已有;
        if (取得执行节点) {
            // 唯一合法调用方self已核验X返回和动作；本owner只核验节点与出生来源闭包。
            (void)读取需求物理节点(*取得执行节点);
            const auto 来源 = 查询服务需求取得来源可抛内部(*取得执行节点);
            if (来源.状态 == 新服务要求采用关系状态::资源失败)return 返回(S::资源失败);
            if (来源.状态 == 新服务要求采用关系状态::未装配)return 返回(S::未装配);
            要求需求物理(来源.状态==新服务要求采用关系状态::已读取);
            要求需求物理(来源.需求节点组.size() <= 1);
            if (!来源.需求节点组.empty()) {
                前缀.需求节点=来源.需求节点组.front();
                同次已有 = 读取需求完整物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 来源.需求节点组.front());
                要求需求物理(同次已有->服务需求取得执行节点 == 取得执行节点
                    && std::binary_search(同次已有->服务要求确认组.begin(),
                        同次已有->服务要求确认组.end(), 确认节点));
            }
        }
        const auto 映射确认 = [](C 状态) {
            if (状态 == C::需重新确认) return S::需重新确认;
            if (状态 == C::未装配) return S::未装配;
            if (状态 == C::资源失败) return S::资源失败;
            if (状态 == C::确认不存在) return S::确认不合法;
            return S::结构不一致;
        };
        const auto 新确认 = 服务要求确认读取接口_->复核服务要求确认语义(确认节点);
        if (新确认.状态 != C::当前有效) {
            要求需求物理(映射确认(新确认.状态)!=S::结构不一致);
            return 返回(映射确认(新确认.状态));
        }
        要求需求物理(新确认.确认 && 新确认.确认->确认节点 == 确认节点
            && 新确认.确认->来源人类节点 == 新确认.确认->目标存在节点);
        if (新确认.确认->来源要求节点) {
            const 新外部服务需求采用结果 已知{新外部服务需求采用状态::工程故障,
                前缀.需求节点,前缀.需求列表项节点};
            const auto 外部 = 建立或取得外部服务需求已核验({
                *新确认.确认->来源要求节点, 新确认.确认->理解节点, 确认节点}, 取得执行节点,&已知);
            using E = 新外部服务需求采用状态;
            if(外部.需求节点)前缀.需求节点=外部.需求节点;
            if(外部.需求列表项节点)前缀.需求列表项节点=外部.需求列表项节点;
            if(外部.状态==E::前提已变化||外部.状态==E::资源失败) {
                if(外部.状态==E::前提已变化) {
                    前缀.状态=新需求操作状态::前提已变化;
                    return 返回(S::前提已变化);
                }
                前缀.状态=新需求操作状态::资源失败;
                return 返回(S::资源失败);
            }
            if (外部.状态 == E::材料待完善 || 外部.状态 == E::需重新确认)
                return 返回(S::需重新确认);
            if (外部.状态 == E::来源不符 || 外部.状态 == E::要求不存在)
                return 返回(S::确认不合法);
            要求需求物理((外部.状态 == E::已建立 || 外部.状态 == E::已复用)
                && 外部.需求节点 && 外部.需求列表项节点);
            // 仅新建D记录本次X；复用的历史R/D不补写或转移取得来源。
            const bool 新建 = 外部.状态 == E::已建立;
            return {新建 ? S::已建立 : S::已复用,
                新需求建立结果{新建 ? 新需求操作状态::已建立 : 新需求操作状态::已复用,
                    外部.需求节点, 外部.需求列表项节点}};
        }
        const auto 配置 = 配置服务要求采用来源可抛内部(
            服务要求结算需求_, 服务要求结算任务_, *服务要求任务读取接口_);
        if(配置==新需求操作状态::资源失败)return 返回(S::资源失败);
        要求需求物理(配置==新需求操作状态::无变化||配置==新需求操作状态::已修改);
        // 即使是新同义确认，也须先拒绝已有的异D/半发布来源，不能先追加再发现损坏。
        const auto 已采用关系 = 查询服务要求采用关系可抛内部(确认节点);
        if(已采用关系.状态==新服务要求采用关系状态::资源失败)return 返回(S::资源失败);
        if(已采用关系.状态==新服务要求采用关系状态::未装配)return 返回(S::未装配);
        要求需求物理(已采用关系.状态==新服务要求采用关系状态::已读取);
        if(!已采用关系.需求节点组.empty()) {
            要求需求物理(已采用关系.需求节点组.size()==1
                &&(!前缀.需求节点||前缀.需求节点==已采用关系.需求节点组.front()));
            前缀.需求节点=已采用关系.需求节点组.front();
        }
        const 新普通需求建立请求 请求{服务要求结算需求_, 新确认.确认->目标存在节点,
            新确认.确认->目标, 新需求来源类型::任务子目标, 服务要求结算任务_};
        if (同次已有) 要求需求物理(同次已有->目标存在节点 == 请求.目标存在节点
            && 同次已有->目标 == 请求.目标 && 同次已有->父需求节点 == 请求.父需求节点
            && 同次已有->来源类型 == 请求.来源类型 && 同次已有->来源承接节点 == 请求.来源承接节点);
        auto 列表读取=查询全部需求列表可抛内部(std::stop_token{});
        const auto* 列表结果=std::get_if<新需求列表全集结果>(&列表读取);
        要求需求物理(列表结果);
        const auto& 列表=*列表结果;
        if(列表.状态==新需求操作状态::资源失败)return 返回(S::资源失败);
        要求需求物理(列表.状态==新需求操作状态::已找到);
        std::optional<新需求信息> 已有;
        for (const auto& d : 列表.需求组) if (!d.根角色 && d.完整目标 && d.完整目标->目标存在节点 == 请求.目标存在节点
            && d.完整目标->目标 == 请求.目标 && d.父需求节点 == 请求.父需求节点
            && d.来源类型 == 请求.来源类型 && d.来源承接节点 == 请求.来源承接节点) {
            要求需求物理(!已有&&(!前缀.需求节点||前缀.需求节点==d.节点));
            前缀.需求节点=d.节点;
            已有 = 读取需求完整物理(需求目标读取上下文{特征服务_, 特征概念服务_}, d.节点);
        }
        要求需求物理(!同次已有 || (已有 && 已有->节点 == 同次已有->节点));
        if (已有) {
            要求需求物理(已采用关系.需求节点组.empty()
                || 已采用关系.需求节点组 == std::vector<稳定编码>{已有->节点});
            if (已有->服务要求确认组.empty()) return 返回(S::既有需求来源冲突);
            const auto 列表节点 = 核验需求列表物理(需求目标读取上下文{特征服务_, 特征概念服务_}, *已有);
            前缀.需求列表项节点=列表节点;
            for (const auto c : 已有->服务要求确认组) {
                const auto 旧确认 = 服务要求确认读取接口_->复核服务要求确认语义(c);
                if (旧确认.状态 != C::当前有效) {
                    要求需求物理(映射确认(旧确认.状态)!=S::结构不一致);
                    return 返回(映射确认(旧确认.状态));
                }
                要求需求物理(旧确认.确认 && 旧确认.确认->确认节点 == c
                    && 旧确认.确认->来源人类节点 == 已有->目标存在节点
                    && 旧确认.确认->目标存在节点 == 已有->目标存在节点 && 旧确认.确认->目标 == 已有->目标);
                const auto 同义 = 服务要求确认读取接口_->比较服务要求确认固定语义(c, 确认节点);
                if (同义.状态 != C::已读取) {
                    要求需求物理(映射确认(同义.状态)!=S::结构不一致);
                    return 返回(映射确认(同义.状态));
                }
                要求需求物理(同义.同义.has_value());
                if (!*同义.同义) return 返回(S::既有需求来源冲突);
            }
            结果.建立结果 = 新需求建立结果{新需求操作状态::已复用, 已有->节点, 列表节点};
            if (!std::binary_search(已有->服务要求确认组.begin(), 已有->服务要求确认组.end(), 确认节点)) {
                const auto 字段 = 全局基础数据集.添加字段节点(已有->节点, 服务要求确认字段, 确认节点);
                if (!有效(字段)) { 结果.状态 = S::资源失败; return 结果; }
                auto 预期 = 已有->服务要求确认组; 预期.push_back(确认节点); std::sort(预期.begin(), 预期.end());
                const auto 读回 = 读取需求完整物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 已有->节点);
                要求需求物理(读回.服务要求确认组 == 预期 && 读回.目标 == 已有->目标
                    && 读回.服务需求取得执行节点 == 已有->服务需求取得执行节点);
                (void)核验需求列表物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 读回);
            }
            结果.状态 = S::已复用;
            if(目标特征实例承载已启用_) {
                const auto s=确保目标特征实例承载可抛内部(已有->节点);
                if(s==新需求操作状态::资源失败)return 返回(S::资源失败);
                if(s==新需求操作状态::前提已变化)return 返回(S::前提已变化);
                要求需求物理(s==新需求操作状态::已修改||s==新需求操作状态::无变化);
            }
        } else {
            要求需求物理(已采用关系.需求节点组.empty()&&!前缀.需求节点);
            const auto 建立 = 建立或取得普通需求来源可抛内部(请求, true, 确认节点, 取得执行节点,前缀);
            if (建立.状态 != 新需求操作状态::已建立) {
                if(建立.状态==新需求操作状态::资源失败)return 返回(S::资源失败);
                if(建立.状态==新需求操作状态::前提已变化)return 返回(S::前提已变化);
                要求需求物理(false);
            }
            要求需求物理(建立.需求节点 && 建立.需求列表项节点);
            结果.状态 = S::已建立;
        }
        // 发布后fresh失败保留已发生的正式D及来源，不回滚、覆盖或盲建第二D。
        const auto 守卫 = 复核需求服务要求语义(*结果.建立结果->需求节点);
        if (守卫.状态 == 新服务要求执行守卫状态::需重新确认) 结果.状态 = S::需重新确认;
        else if (守卫.状态 == 新服务要求执行守卫状态::前提已变化) 结果.状态 = S::前提已变化;
        else if (守卫.状态 == 新服务要求执行守卫状态::资源失败) 结果.状态 = S::资源失败;
        else if (守卫.状态 == 新服务要求执行守卫状态::未装配) 结果.状态 = S::未装配;
        else 要求需求物理(守卫.状态 == 新服务要求执行守卫状态::当前有效);
        return 结果;
    } catch (const 需求物理读取失败& e) {
        if(e.原因!=基础结构操作状态::资源失败)
            工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"采用服务要求需求",L"正式来源或需求结构损坏，保留事实。");
        结果.状态 = S::资源失败;
    } catch (const std::bad_alloc&) { 结果.状态 = S::资源失败; }
    catch (const std::length_error&) { 结果.状态 = S::资源失败; }
    catch (新需求目标定义核验状态 s) {
        if(s!=新需求目标定义核验状态::前提已变化)
            工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"采用服务要求需求",L"未知目标核验异常。");
        结果.状态=S::前提已变化;
    }
    catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"采用服务要求需求",L"采用工程故障，保留已成立前缀。"); }
    return 结果;
}

新行动请求来源读取结果 新_需求类::读取行动请求承接来源保护(
    const 新行动请求承接来源& s) const noexcept {
    using S=新行动请求来源读取状态;
    try {
        const auto& k=s.需求来源.键;
        if(!有效(k.接收消息节点)||!有效(s.需求来源.采用含义节点)||!有效(s.被认知需求节点)
            ||!有效(s.来源人类节点)||!有效(s.受请求者节点)||k.原文起点>=k.原文终点
            ||k.根出现路径.empty()||!std::all_of(k.根出现路径.begin(),k.根出现路径.end(),[](auto n){return n>=0;}))
            return {S::输入不适用,{},{}};
        // 装配只在线程启动前/最终join后变动；语言调用不持需求锁。
        const auto* language=接收认知语言读取接口_;
        if(!language||!结构已初始化())return {S::依赖未就绪,{},{}};
        const auto request=language->读取采用行动请求保护(s.需求来源.采用含义节点,s.受请求者节点,特征服务_,存在服务_);
        if(request.状态==新语言含义采用状态::资源失败)return {S::资源失败,{},{}};
        if(request.状态==新语言含义采用状态::前提已变化||request.状态==新语言含义采用状态::未找到)
            return {S::前提已变化,{},{}};
        if(request.状态!=新语言含义采用状态::已采用&&request.状态!=新语言含义采用状态::精确重复)
            return {S::输入不适用,{},{}};
        要求需求物理(request.信息&&!request.完整保护.empty());
        const auto& r=*request.信息;
        if(r.采用含义节点!=s.需求来源.采用含义节点||r.说话者H!=s.来源人类节点
            ||r.受请求者E!=s.受请求者节点||r.需求来源!=k||r.请求来源.接收消息节点!=k.接收消息节点)
            return {S::输入不适用,{},{}};
        const auto found=查询接收认知来源需求(s.需求来源,*language,特征服务_);
        if(found.状态==新需求操作状态::资源失败)return {S::资源失败,{},{}};
        if(found.状态==新需求操作状态::前提已变化||found.状态==新需求操作状态::需求不存在)
            return {S::前提已变化,{},{}};
        if(found.状态==新需求操作状态::尚未初始化)return {S::依赖未就绪,{},{}};
        if(found.状态!=新需求操作状态::已找到)return {S::输入不适用,{},{}};
        要求需求物理(found.信息.has_value());
        if(found.信息->节点!=s.被认知需求节点)return {S::输入不适用,{},{}};
        const auto qualification=读取被认知需求完整资格保护(s.被认知需求节点,s.来源人类节点);
        if(qualification.状态==新被认知需求资格状态::资源失败)return {S::资源失败,{},{}};
        if(qualification.状态==新被认知需求资格状态::前提已变化)return {S::前提已变化,{},{}};
        if(qualification.状态==新被认知需求资格状态::依赖未就绪)return {S::依赖未就绪,{},{}};
        if(qualification.状态!=新被认知需求资格状态::已读取)return {S::输入不适用,{},{}};
        要求需求物理(qualification.信息.has_value());
        const auto& d=qualification.信息->需求;
        if(d!=*found.信息)return {S::前提已变化,{},{}};
        要求需求物理(d.完整目标&&d.已知目标&&d.执行域==新需求执行域::被认知&&!d.父需求节点
            &&d.需求主体存在节点==s.来源人类节点&&d.首次接收认知来源==std::optional{s.需求来源});
        // 原目标核验精确检查FT/值域/比较与U的期望需求映射，不只比较非零身份。
        const auto target=读取接收认知需求来源保护(*language,特征服务_,s.需求来源,s.来源人类节点,*d.已知目标);
        if(target.状态==新需求操作状态::资源失败)return {S::资源失败,{},{}};
        if(target.状态==新需求操作状态::前提已变化)return {S::前提已变化,{},{}};
        要求需求物理(target.状态==新需求操作状态::已找到);
        auto protect=request.完整保护;
        if(!并需求保护(protect,found.完整保护)||!并需求保护(protect,qualification.完整保护)
            ||!并需求保护(protect,target.完整保护))return {S::前提已变化,{},{}};
        for(const auto id:{s.来源人类节点,s.受请求者节点,d.完整目标->目标存在节点}) {
            if(!加需求存在保护(id,存在服务_,protect))return {S::前提已变化,{},{}};
        }
        if(!需求内容保护未变(protect))return {S::前提已变化,{},{}};
        return {S::已读取,新行动请求来源材料{s,*d.完整目标},std::move(protect)};
    }catch(const 新需求目标定义核验状态 s){要求需求物理(s==新需求目标定义核验状态::前提已变化);return {S::前提已变化,{},{}};}
    catch(const 需求物理读取失败& e){
        if(e.原因==基础结构操作状态::资源失败)return {S::资源失败,{},{}};
        if(e.原因==基础结构操作状态::节点不存在)return {S::前提已变化,{},{}};
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取行动请求来源",L"正式来源结构损坏");
    }catch(const std::bad_alloc&){return {S::资源失败,{},{}};}
    catch(const std::length_error&){return {S::资源失败,{},{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取行动请求来源",L"正式来源保护读取工程故障");}
}

新接收认知需求读取结果 新_需求类::查询行动请求自我需求已加锁(
    const 新行动请求承接来源& s,const std::vector<基础联合节点快照>& source,提议源收集批* 源) const {
    using S=新需求操作状态;
    auto protect=source;
    const auto capture=[&](稳定编码 id) {
        const auto r=全局基础数据集.查询节点组完整结构({id});
        if(r.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        要求需求物理(r.状态==基础结构操作状态::已完成&&r.节点组.size()==1);
        if(!r.节点组.front().快照)throw 新需求目标定义核验状态::前提已变化;
        const auto n=*r.节点组.front().快照;
        需求物理游标 c{n.结构,{}};
        const bool 自我源=源&&c.引用(节点类型字段,false)==需求节点类型
            &&c.数值(需求执行域字段,false)==static_cast<std::int64_t>(新需求执行域::自我执行);
        if(自我源) {
            const auto p=构造需求提议源前提(n);
            合并提议源前提(源->前提,p);源->需求节点组.insert(id);
            (void)投影需求目标实例(需求目标读取上下文{特征服务_,特征概念服务_},n.结构,&protect,false,&p);
        } else {
            if(!并需求保护(protect,{n}))throw 新需求目标定义核验状态::前提已变化;
            if(读取唯一节点字段(id,节点类型字段)==需求节点类型)
                (void)投影需求目标实例(需求目标读取上下文{特征服务_,特征概念服务_},n.结构,&protect);
        }
        return n;
    };
    (void)capture(s.需求来源.采用含义节点); // 入向查重闭包与发布CAS共用同一U保护。
    const auto refs=全局基础数据集.查询字段引用节点结构(s.需求来源.采用含义节点,行动请求含义字段);
    if(refs.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
    要求需求物理(refs.状态==基础结构操作状态::已完成&&refs.引用字段组.size()==refs.来源节点组.size()
        &&refs.来源节点组.size()<=1);
    std::optional<新需求治理信息> result;
    if(!refs.来源节点组.empty()) {
        const auto raw=capture(refs.来源节点组.front().节点.编码);
        const auto p=源 ? 构造需求提议源前提(raw) : 基础只读预期{};
        const auto projected=投影需求目标实例(需求目标读取上下文{特征服务_,特征概念服务_},raw.结构,&protect,false,源 ? &p : nullptr);
        auto d=解码需求治理物理(需求目标读取上下文{特征服务_,特征概念服务_},源 ? projected : raw.结构);
        要求需求物理(d.来源类型==新需求来源类型::行动请求&&d.行动请求来源&&d.完整目标);
        if(d.行动请求来源!=std::optional{s}||d.执行域!=新需求执行域::自我执行
            ||d.需求主体存在节点!=s.受请求者节点||d.父需求节点!=服务要求结算需求_)
            return {S::同来源目标冲突,{},{}};
        const auto list=核验需求列表物理(需求目标读取上下文{特征服务_,特征概念服务_},投影完整需求(d));
        (void)capture(list);(void)capture(*d.父需求节点);(void)capture(需求树根节点);
        const auto father=读取需求治理物理(需求目标读取上下文{特征服务_,特征概念服务_},*d.父需求节点);
        const auto service=读取需求治理物理(需求目标读取上下文{特征服务_,特征概念服务_},服务根需求节点);
        (void)capture(服务根需求节点);
        要求需求物理(!father.根角色&&father.父需求节点==服务根需求节点&&father.完整目标
            &&father.执行域==新需求执行域::自我执行&&father.需求主体存在节点==s.受请求者节点
            &&service.根角色==新需求根角色::服务&&service.需求主体存在节点==s.受请求者节点);
        result=std::move(d);
    }
    if(!需求内容保护未变(protect)||!核同提议源(源))return {S::前提已变化,{},{}};
    return {result?S::已找到:S::需求不存在,std::move(result),std::move(protect)};
}

新接收认知需求读取结果 新_需求类::查询行动请求自我需求(const 新行动请求承接来源& s) const noexcept {
    return 查询行动请求自我需求内部(s,nullptr);
}
新接收认知需求读取结果 新_需求类::查询行动请求自我需求内部(
    const 新行动请求承接来源& s,提议源收集批* 源) const noexcept {
    using S=新需求操作状态;using R=新行动请求来源读取状态;
    try {
        const auto source=读取行动请求承接来源保护(s);
        switch(source.状态) {
        case R::输入不适用:return {S::认知来源不合法,{},{}};
        case R::前提已变化:return {S::前提已变化,{},{}};
        case R::依赖未就绪:return {S::尚未初始化,{},{}};
        case R::资源失败:return {S::资源失败,{},{}};
        default:要求需求物理(source.状态==R::已读取&&source.信息);
        }
        std::lock_guard lock(新需求互斥);
        auto found=查询行动请求自我需求已加锁(s,source.完整保护,源);
        if(found.状态==S::已找到&&found.信息->完整目标!=std::optional{source.信息->目标})
            return {S::同来源目标冲突,{},{}};
        return found;
    }catch(const 新需求目标定义核验状态 s){要求需求物理(s==新需求目标定义核验状态::前提已变化);return {S::前提已变化,{},{}};}
    catch(const 需求物理读取失败& e){if(e.原因==基础结构操作状态::资源失败)return {S::资源失败,{},{}};
        if(e.原因==基础结构操作状态::节点不存在)return {S::前提已变化,{},{}};
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"查询行动请求自我需求",L"正式需求结构损坏");}
    catch(const std::bad_alloc&){return {S::资源失败,{},{}};}
    catch(const std::length_error&){return {S::资源失败,{},{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"查询行动请求自我需求",L"同源查询工程故障");}
}

新行动请求采用结果 新_需求类::建立或取得行动请求需求已核验(
    const 新行动请求承接来源& s,稳定编码 parent) noexcept {
    using S=新行动请求采用状态;using R=新行动请求来源读取状态;
    try {
        const auto source=读取行动请求承接来源保护(s);
        switch(source.状态) {
        case R::输入不适用:return {S::来源不适用,{},{}};
        case R::前提已变化:return {S::前提已变化,{},{}};
        case R::依赖未就绪:return {S::依赖未就绪,{},{}};
        case R::资源失败:return {S::资源失败,{},{}};
        default:要求需求物理(source.状态==R::已读取&&source.信息&&!source.完整保护.empty());
        }
        std::optional<关联检查离域通知> notify;
        std::unique_lock lock(新需求互斥);
        if(!结构仍存在()||!有效(服务要求结算需求_))return {S::依赖未就绪,{},{}};
        if(parent!=服务要求结算需求_)return {S::父域不符,{},{}};
        const auto ctx=需求目标读取上下文{特征服务_,特征概念服务_};
        const auto father=读取需求治理物理(ctx,parent);
        const auto service=读取需求治理物理(ctx,服务根需求节点);
        if(father.父需求节点!=服务根需求节点||father.根角色||!father.完整目标
            ||father.执行域!=新需求执行域::自我执行||father.需求主体存在节点!=s.受请求者节点
            ||service.根角色!=新需求根角色::服务||service.需求主体存在节点!=s.受请求者节点)
            return {S::父域不符,{},{}};
        const auto found=查询行动请求自我需求已加锁(s,source.完整保护);
        if(found.状态==新需求操作状态::前提已变化)return {S::前提已变化,{},{}};
        if(found.状态==新需求操作状态::同来源目标冲突)return {S::同源冲突,{},{}};
        基础节点组追加请求 publish;publish.预期联合节点组=found.完整保护;
        const auto capture=[&](稳定编码 id) {
            const auto r=全局基础数据集.查询节点组完整结构({id});
            if(r.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            要求需求物理(r.状态==基础结构操作状态::已完成&&r.节点组.size()==1);
            if(!r.节点组.front().快照||!并需求保护(publish.预期联合节点组,{*r.节点组.front().快照}))
                throw 新需求目标定义核验状态::前提已变化;
            if(读取唯一节点字段(id,节点类型字段)==需求节点类型)
                (void)投影需求目标实例(ctx,r.节点组.front().快照->结构,&publish.预期联合节点组);
        };
        const auto finish=[&](稳定编码 id,S status)->新行动请求采用结果 {
            const auto complete=确保目标特征实例承载内部(id);
            if(complete==新需求操作状态::资源失败)return {S::资源失败,id,{}};
            if(complete==新需求操作状态::前提已变化)return {S::前提已变化,id,{}};
            if(complete==新需求操作状态::目标内容依赖未就绪)return {S::依赖未就绪,id,{}};
            要求需求物理(complete==新需求操作状态::已修改||complete==新需求操作状态::无变化);
            // 五F补齐会改变来源读集中的FT引用闭包；成功仅在新读回后给出。
            const auto read=查询行动请求自我需求已加锁(s,{});
            if(read.状态==新需求操作状态::前提已变化)return {S::前提已变化,id,{}};
            要求需求物理(read.状态==新需求操作状态::已找到&&read.信息&&read.信息->节点==id
                &&read.信息->完整目标==std::optional{source.信息->目标});
            const auto list=核验需求列表物理(ctx,投影完整需求(*read.信息));
            lock.unlock();
            const auto fresh=查询行动请求自我需求(s);
            if(fresh.状态==新需求操作状态::资源失败)return {S::资源失败,id,list};
            if(fresh.状态==新需求操作状态::前提已变化)return {S::前提已变化,id,list};
            if(fresh.状态==新需求操作状态::尚未初始化)return {S::依赖未就绪,id,list};
            要求需求物理(fresh.状态==新需求操作状态::已找到&&fresh.信息&&fresh.信息->节点==id
                &&fresh.信息->完整目标==std::optional{source.信息->目标});
            return {status,id,list};
        };
        if(found.状态==新需求操作状态::已找到) {
            要求需求物理(found.信息.has_value());
            if(found.信息->完整目标!=std::optional{source.信息->目标})return {S::同源冲突,{},{}};
            return finish(found.信息->节点,S::已复用);
        }
        要求需求物理(found.状态==新需求操作状态::需求不存在);
        capture(parent);capture(需求树根节点);capture(服务根需求节点);
        std::vector<新需求治理信息> children;
        std::size_t count=1;
        for(const auto& edge:查询子需求关系(parent)) {
            auto child=读取需求治理物理(ctx,edge.目标节点);
            要求需求物理(child.父需求节点==parent&&!child.根角色&&child.完整目标);
            if(child.活动状态!=新需求活动状态::失活)++count;
            children.push_back(std::move(child));
        }
        std::sort(children.begin(),children.end(),[](const auto& a,const auto& b){return a.节点<b.节点;});
        const auto share=father.权重/static_cast<std::int64_t>(count);
        auto remainder=father.权重%static_cast<std::int64_t>(count);
        std::vector<需求权重修改项> changes;std::set<稳定编码> seen{parent};
        for(const auto& child:children) {
            要求需求物理(seen.insert(child.节点).second);
            const auto weight=child.活动状态==新需求活动状态::失活 ? 0 : share+(remainder>0 ? (--remainder,1) : 0);
            要求需求物理(追加需求权重修改(child.节点,weight,changes)
                &&构建子树权重修改(ctx,child.节点,weight,{},seen,changes));
        }
        for(const auto id:seen)capture(id);
        for(const auto& change:changes)publish.替换字段组.push_back({change.字段关系,基础值{change.新权重}});
        const auto& goal=source.信息->目标;
        const auto list=按完整目标查询(goal.目标存在节点,goal.目标);
        if(list.状态==新需求操作状态::资源失败)return {S::资源失败,{},{}};
        要求需求物理(list.状态==新需求操作状态::已找到||list.状态==新需求操作状态::需求不存在);
        const auto addGoal=[&](std::vector<基础组内初始字段>& fields) {
            fields.push_back({目标存在字段,goal.目标存在节点});fields.push_back({目标特征类型字段,goal.目标.目标特征类型概念节点});
            fields.push_back({目标形式字段,基础值{static_cast<std::int64_t>(goal.目标.目标形式)}});
            if(goal.目标.目标值域概念节点)fields.push_back({目标值域字段,*goal.目标.目标值域概念节点});
            if(goal.目标.目标比较关系概念节点)fields.push_back({目标比较关系字段,*goal.目标.目标比较关系概念节点});
            if(goal.目标.目标变化方向)fields.push_back({目标变化方向字段,基础值{static_cast<std::int64_t>(*goal.目标.目标变化方向)}});
        };
        基础组内新节点 demand;
        demand.字段组={{节点类型字段,需求节点类型},{需求主体字段,s.受请求者节点},
            {需求执行域字段,基础值{static_cast<std::int64_t>(新需求执行域::自我执行)}},
            {来源类型字段,基础值{static_cast<std::int64_t>(新需求来源类型::行动请求)}},
            {活动状态字段,基础值{static_cast<std::int64_t>(新需求活动状态::活动)}},{需求权重字段,基础值{share}},
            {行动请求消息字段,s.需求来源.键.接收消息节点},{行动请求含义字段,s.需求来源.采用含义节点},
            {行动请求原需求字段,s.被认知需求节点},{行动请求人类字段,s.来源人类节点},{行动请求受话者字段,s.受请求者节点},
            {行动请求范围字段,基础值{std::vector<std::uint64_t>{s.需求来源.键.原文起点,s.需求来源.键.原文终点}}},
            {行动请求路径字段,基础值{s.需求来源.键.根出现路径}}};
        addGoal(demand.字段组);publish.新节点组.push_back(std::move(demand));
        publish.追加外部关系组.push_back({parent,基础组内节点引用{0},基础外部关系类型::父子,需求父子关系角色});
        if(list.需求列表项节点) {
            capture(*list.需求列表项节点);
            const auto members=读取多节点字段(*list.需求列表项节点,列表成员字段);
            for(const auto id:members)capture(id);
            publish.追加字段组.push_back({*list.需求列表项节点,列表成员字段,基础组内节点引用{0}});
        } else {
            基础组内新节点 item;item.字段组.push_back({节点类型字段,需求列表项节点类型});addGoal(item.字段组);
            item.字段组.push_back({列表成员字段,基础组内节点引用{0}});publish.新节点组.push_back(std::move(item));
            publish.追加字段组.push_back({需求树根节点,需求列表项字段,基础组内节点引用{1}});
        }
        if(!需求内容保护未变(publish.预期联合节点组))return {S::前提已变化,{},{}};
        for(const auto& n:publish.预期联合节点组)publish.预期既有节点组.push_back(n.结构);
        const auto commit=全局基础数据集.原子追加节点组(publish);
        if(commit.状态==基础节点组追加状态::预期不符)return {S::前提已变化,{},{}};
        if(commit.状态==基础节点组追加状态::资源失败)return {S::资源失败,{},{}};
        要求需求物理(commit.状态==基础节点组追加状态::已完成&&commit.新节点组.size()==publish.新节点组.size());
        notify.emplace(关联检查通知_);
        return finish(commit.新节点组.front(),S::已建立);
    }catch(const 新需求目标定义核验状态 s){要求需求物理(s==新需求目标定义核验状态::前提已变化);return {S::前提已变化,{},{}};}
    catch(const 需求物理读取失败& e){if(e.原因==基础结构操作状态::资源失败)return {S::资源失败,{},{}};
        if(e.原因==基础结构操作状态::节点不存在)return {S::前提已变化,{},{}};
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"采用行动请求需求",L"需求组合发布结构损坏");}
    catch(const std::bad_alloc&){return {S::资源失败,{},{}};}
    catch(const std::length_error&){return {S::资源失败,{},{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"采用行动请求需求",L"需求组合发布工程故障，保留已发布前缀");}
}

新外部服务需求采用结果 新_需求类::建立或取得外部服务需求已核验(
    const 新完整服务需求材料包& 信息包,
    std::optional<稳定编码> 取得执行节点,
    const 新外部服务需求采用结果* 已知前缀) noexcept {
    using S = 新外部服务需求采用状态;
    新外部服务需求采用结果 前缀;
    if(已知前缀) {
        前缀.需求节点=已知前缀->需求节点;
        前缀.需求列表项节点=已知前缀->需求列表项节点;
    }
    const auto 返回=[&](S s) { 前缀.状态=s;return 前缀; };
    try {
        std::lock_guard 锁(新需求互斥);
        要求需求物理(!前缀.需求列表项节点||前缀.需求节点);
        if (!有效(信息包.要求节点) || !有效(信息包.理解节点) || !有效(信息包.确认节点))
            return 返回(S::来源不符);
        要求需求物理(结构仍存在() && 部分服务要求读取接口_ && 服务要求确认读取接口_
            && 有效(服务要求结算需求_) && 有效(服务要求结算任务_));
        if (取得执行节点) (void)读取需求物理节点(*取得执行节点);
        if(前缀.需求节点) {
            const auto 已知=读取需求治理物理(需求目标读取上下文{特征服务_,特征概念服务_},*前缀.需求节点);
            要求需求物理(已知.完整目标&&已知.外部要求节点==信息包.要求节点
                &&已知.父需求节点==服务要求结算需求_&&已知.来源类型==新需求来源类型::外部服务要求
                &&!已知.来源承接节点&&已知.服务要求确认组==std::vector<稳定编码>{信息包.确认节点});
            const auto 列表=核验需求列表物理(需求目标读取上下文{特征服务_,特征概念服务_},投影完整需求(已知));
            要求需求物理(!前缀.需求列表项节点||前缀.需求列表项节点==列表);
            前缀.需求列表项节点=列表;
        }
        {
            const auto 读取 = 部分服务要求读取接口_->读取部分服务要求(信息包.要求节点);
            if (读取.状态 == 新部分服务要求读取状态::材料不存在) {
                要求需求物理(!前缀.需求节点);
                return 返回(S::要求不存在);
            }
            要求需求物理(读取.状态 == 新部分服务要求读取状态::已读取 && 读取.材料
                && 读取.材料->要求节点 == 信息包.要求节点 && !读取.发布预期节点组.empty());
            const auto& 材料 = *读取.材料;
            if (!查询服务要求缺项(材料).empty() || 材料.理解节点 != 信息包.理解节点
                || 材料.确认节点 != 信息包.确认节点)
                return 返回(S::材料待完善);
            const auto 确认 = 服务要求确认读取接口_->复核服务要求确认语义(信息包.确认节点);
            if (确认.状态 == 新服务要求确认状态::需重新确认)
                return 返回(S::需重新确认);
            if (确认.状态 == 新服务要求确认状态::资源失败)return 返回(S::资源失败);
            要求需求物理(确认.状态 == 新服务要求确认状态::当前有效 && 确认.确认);
            const auto& 确认材料 = *确认.确认;
            const auto& 目标材料 = 材料.已知目标;
            if (确认材料.来源要求节点 != 信息包.要求节点
                || 确认材料.理解节点 != 信息包.理解节点
                || 确认材料.确认节点 != 信息包.确认节点
                || 确认材料.来源人类节点 != 材料.来源人类节点
                || 确认材料.目标存在节点 != 材料.来源人类节点
                || 目标材料.目标存在节点 != 确认材料.目标存在节点
                || 目标材料.目标特征类型概念节点 != 确认材料.目标.目标特征类型概念节点
                || 目标材料.目标形式 != 确认材料.目标.目标形式
                || 目标材料.目标值域概念节点 != 确认材料.目标.目标值域概念节点
                || 目标材料.目标比较关系概念节点 != 确认材料.目标.目标比较关系概念节点
                || 目标材料.目标变化方向 != 确认材料.目标.目标变化方向)
                return 返回(S::来源不符);
            const auto 父 = 读取需求治理物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 服务要求结算需求_);
            要求需求物理(父.父需求节点 == 服务根需求节点 && 父.完整目标);
            const auto 来源 = 全局基础数据集.查询字段引用节点结构(信息包.要求节点, 外部服务要求字段);
            if(来源.状态!=基础结构操作状态::已完成)throw 需求物理读取失败{来源.状态};
            要求需求物理(来源.来源节点组.size() == 来源.引用字段组.size() && 来源.来源节点组.size() <= 1);
            const auto 复核已发布 = [&](稳定编码 需求, S 成功状态) {
                要求需求物理(!前缀.需求节点||前缀.需求节点==需求);
                前缀.需求节点=需求;
                if(目标特征实例承载已启用_) {
                    const auto s=确保目标特征实例承载可抛内部(需求);
                    if(s==新需求操作状态::前提已变化)return 返回(S::前提已变化);
                    if(s==新需求操作状态::资源失败)return 返回(S::资源失败);
                    要求需求物理(s==新需求操作状态::已修改||s==新需求操作状态::无变化);
                }
                const auto 已有 = 读取需求治理物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 需求);
                要求需求物理(已有.完整目标 && 已有.外部要求节点 == 信息包.要求节点
                    && 已有.父需求节点 == 服务要求结算需求_
                    && 已有.来源类型 == 新需求来源类型::外部服务要求
                    && !已有.来源承接节点
                    && 已有.完整目标->目标存在节点 == 确认材料.目标存在节点
                    && 已有.完整目标->目标 == 确认材料.目标
                    && 已有.服务要求确认组 == std::vector<稳定编码>{信息包.确认节点});
                if (成功状态 == S::已建立)
                    要求需求物理(已有.服务需求取得执行节点 == 取得执行节点);
                const auto 列表节点 = 核验需求列表物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 投影完整需求(已有));
                要求需求物理(!前缀.需求列表项节点||前缀.需求列表项节点==列表节点);
                前缀.需求列表项节点=列表节点;
                const auto 采用 = 查询服务要求采用关系可抛内部(信息包.确认节点);
                if(采用.状态==新服务要求采用关系状态::资源失败)return 返回(S::资源失败);
                要求需求物理(采用.状态 == 新服务要求采用关系状态::已读取
                    && 采用.需求节点组 == std::vector<稳定编码>{需求});
                if(取得执行节点) {
                    const auto 同次=查询服务需求取得来源可抛内部(*取得执行节点);
                    if(同次.状态==新服务要求采用关系状态::资源失败)return 返回(S::资源失败);
                    要求需求物理(同次.状态==新服务要求采用关系状态::已读取
                        &&(同次.需求节点组.empty()||同次.需求节点组==std::vector<稳定编码>{需求}));
                }
                const auto 守卫 = 复核需求服务要求语义(需求);
                if (守卫.状态 == 新服务要求执行守卫状态::需重新确认)
                    return 返回(S::需重新确认);
                if(守卫.状态==新服务要求执行守卫状态::前提已变化)return 返回(S::前提已变化);
                if(守卫.状态==新服务要求执行守卫状态::资源失败)return 返回(S::资源失败);
                要求需求物理(守卫.状态 == 新服务要求执行守卫状态::当前有效);
                return 返回(成功状态);
            };
            if (!来源.来源节点组.empty())
                return 复核已发布(来源.来源节点组.front().节点.编码, S::已复用);
            要求需求物理(!前缀.需求节点); // 已知D只能续核同一R，绝不重建。
            const auto 采用 = 查询服务要求采用关系可抛内部(信息包.确认节点);
            if(采用.状态==新服务要求采用关系状态::资源失败)return 返回(S::资源失败);
            要求需求物理(采用.状态 == 新服务要求采用关系状态::已读取 && 采用.需求节点组.empty());

            基础节点组追加请求 发布;
            发布.预期联合节点组 = 读取.发布预期节点组;
            bool 预期已漂移 = false;
            const auto 加预期 = [&](稳定编码 节点) {
                const auto 查询 = 全局基础数据集.查询节点组完整结构({节点});
                if(查询.状态!=基础结构操作状态::已完成)throw 需求物理读取失败{查询.状态};
                要求需求物理(查询.节点组.size() == 1 && 查询.节点组.front().快照);
                const auto& 快照 = *查询.节点组.front().快照;
                const auto 位置 = std::find_if(发布.预期联合节点组.begin(), 发布.预期联合节点组.end(),
                    [&](const auto& 项) { return 项.结构.节点.编码 == 节点; });
                if (位置 == 发布.预期联合节点组.end()) 发布.预期联合节点组.push_back(快照);
                else if (位置->结构.字段组 != 快照.结构.字段组
                    || 位置->入向字段组 != 快照.入向字段组 || 位置->入向外部关系组 != 快照.入向外部关系组
                    || 位置->出向外部关系组 != 快照.出向外部关系组) 预期已漂移 = true;
            };
            加预期(服务要求结算需求_);
            加预期(需求树根节点);
            if (取得执行节点) 加预期(*取得执行节点);
            const auto 子组 = 查询子需求关系(服务要求结算需求_);
            std::vector<新需求治理信息> 子需求;
            std::size_t 活动数 = 1;
            for (const auto& 子边 : 子组) {
                auto 子 = 读取需求治理物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 子边.目标节点);
                要求需求物理(子.父需求节点 == 服务要求结算需求_ && !子.根角色 && 子.完整目标);
                if (子.活动状态 != 新需求活动状态::失活) ++活动数;
                子需求.push_back(std::move(子));
            }
            std::sort(子需求.begin(), 子需求.end(), [](const auto& 左, const auto& 右) { return 左.节点 < 右.节点; });
            const auto 份额 = 父.权重 / static_cast<std::int64_t>(活动数);
            auto 余数 = 父.权重 % static_cast<std::int64_t>(活动数);
            std::vector<需求权重修改项> 修改组;
            std::set<稳定编码> 已见{服务要求结算需求_};
            for (const auto& 子 : 子需求) {
                要求需求物理(已见.insert(子.节点).second);
                const auto 权重 = 子.活动状态 == 新需求活动状态::失活 ? 0 : 份额 + (余数 > 0 ? (--余数, 1) : 0);
                要求需求物理(追加需求权重修改(子.节点, 权重, 修改组)
                    && 构建子树权重修改(需求目标读取上下文{特征服务_, 特征概念服务_}, 子.节点, 权重, {}, 已见, 修改组));
            }
            for (const auto 节点 : 已见) 加预期(节点);
            for (const auto& 修改 : 修改组)
                发布.替换字段组.push_back({修改.字段关系, 基础值{修改.新权重}});
            const auto 列表 = 按完整目标查询可抛内部(确认材料.目标存在节点, 确认材料.目标);
            if(列表.状态==新需求操作状态::资源失败)return 返回(S::资源失败);
            要求需求物理(列表.状态 == 新需求操作状态::已找到 || 列表.状态 == 新需求操作状态::需求不存在);
            const auto 加目标 = [&](std::vector<基础组内初始字段>& 字段组) {
                字段组.push_back({目标存在字段, 确认材料.目标存在节点});
                字段组.push_back({目标特征类型字段, 确认材料.目标.目标特征类型概念节点});
                字段组.push_back({目标形式字段, 基础值{static_cast<std::int64_t>(确认材料.目标.目标形式)}});
                if (确认材料.目标.目标值域概念节点)
                    字段组.push_back({目标值域字段, *确认材料.目标.目标值域概念节点});
                if (确认材料.目标.目标比较关系概念节点)
                    字段组.push_back({目标比较关系字段, *确认材料.目标.目标比较关系概念节点});
                if (确认材料.目标.目标变化方向)
                    字段组.push_back({目标变化方向字段, 基础值{static_cast<std::int64_t>(*确认材料.目标.目标变化方向)}});
            };
            基础组内新节点 需求;
            需求.字段组 = {{节点类型字段, 需求节点类型},
                {需求主体字段, 父.需求主体存在节点},
                {需求执行域字段, 基础值{static_cast<std::int64_t>(新需求执行域::自我执行)}},
                {来源类型字段, 基础值{static_cast<std::int64_t>(新需求来源类型::外部服务要求)}},
                {外部服务要求字段, 信息包.要求节点}, {服务要求确认字段, 信息包.确认节点},
                {活动状态字段, 基础值{static_cast<std::int64_t>(新需求活动状态::活动)}},
                {需求权重字段, 基础值{份额}}};
            加目标(需求.字段组);
            if (取得执行节点) 需求.字段组.push_back({服务需求取得执行字段, *取得执行节点});
            发布.新节点组.push_back(std::move(需求));
            发布.追加外部关系组.push_back({服务要求结算需求_, 基础组内节点引用{0},
                基础外部关系类型::父子, 需求父子关系角色});
            if (列表.需求列表项节点) {
                加预期(*列表.需求列表项节点);
                发布.追加字段组.push_back({*列表.需求列表项节点, 列表成员字段, 基础组内节点引用{0}});
            } else {
                基础组内新节点 新列表;
                新列表.字段组.push_back({节点类型字段, 需求列表项节点类型});
                加目标(新列表.字段组);
                新列表.字段组.push_back({列表成员字段, 基础组内节点引用{0}});
                发布.新节点组.push_back(std::move(新列表));
                发布.追加字段组.push_back({需求树根节点, 需求列表项字段, 基础组内节点引用{1}});
            }
            if (预期已漂移) return 返回(S::前提已变化);
            for (const auto& 快照 : 发布.预期联合节点组) 发布.预期既有节点组.push_back(快照.结构);
            const auto 写入 = 全局基础数据集.原子追加节点组(发布);
            if (写入.状态 == 基础节点组追加状态::预期不符) return 返回(S::前提已变化);
            if (写入.状态 == 基础节点组追加状态::资源失败) return 返回(S::资源失败);
            要求需求物理(写入.状态 == 基础节点组追加状态::已完成
                && 写入.新节点组.size() == 发布.新节点组.size());
            前缀.需求节点=写入.新节点组.front();
            if(列表.需求列表项节点)前缀.需求列表项节点=列表.需求列表项节点;
            else 前缀.需求列表项节点=写入.新节点组[1];
            const 关联检查离域通知 通知{关联检查通知_};
            return 复核已发布(写入.新节点组.front(), S::已建立);
        }
    } catch (const 需求物理读取失败& e) {
        if(e.原因==基础结构操作状态::资源失败)return 返回(S::资源失败);
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"采用完整外部服务需求",L"正式物理结构故障；保留已成立前缀。");
    } catch (const std::bad_alloc&) { return 返回(S::资源失败); }
    catch (const std::length_error&) { return 返回(S::资源失败); }
    catch (新需求目标定义核验状态 s) {
        if(s==新需求目标定义核验状态::前提已变化)return 返回(S::前提已变化);
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"采用完整外部服务需求",L"未知目标核验异常。");
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"采用完整外部服务需求", L"完整材料核验或需求原子发布故障");
    }
}

std::vector<新服务要求缺项> 查询服务要求缺项(const 新部分服务要求材料& r) {
    std::vector<新服务要求缺项> v;
    const auto& g = r.已知目标;
    if (!g.目标存在节点) v.push_back(新服务要求缺项::目标存在);
    if (!g.目标特征类型概念节点) v.push_back(新服务要求缺项::目标特征类型);
    if (!g.目标形式) v.push_back(新服务要求缺项::目标形式);
    if (g.目标形式 == 新需求目标形式::值) {
        if (!g.目标值域概念节点) v.push_back(新服务要求缺项::目标值域);
        if (!g.目标比较关系概念节点) v.push_back(新服务要求缺项::目标比较关系);
    } else if (g.目标形式 == 新需求目标形式::变化方向 && !g.目标变化方向)
        v.push_back(新服务要求缺项::目标变化方向);
    if (!r.确认节点) v.push_back(新服务要求缺项::明确确认);
    return v;
}

void 新_需求类::装配部分服务要求读取接口(const 新部分服务要求读取接口* 接口) noexcept {
    std::lock_guard 锁(新需求互斥);
    部分服务要求读取接口_ = 接口;
}

struct 新_需求类::来源候选枚举保护批 final {
    std::vector<基础联合节点快照> 完整保护;
    std::set<稳定编码> 已捕获原始闭包节点组;
    std::set<稳定编码> 已采集本域节点组;
    // 私有单D治理口已定点预采解码依赖，不再为它展开枚举闭包。
    bool 仅当前需求 = false;
};

std::optional<新需求治理信息> 新_需求类::读取需求治理信息(稳定编码 节点) const noexcept {
    try { return 读取需求治理信息可抛内部(节点); }
    catch (const std::bad_alloc&) { return std::nullopt; }
    catch (const std::length_error&) { return std::nullopt; }
    catch (const 需求物理读取失败& e) {
        // 旧/不可恢复物理布局：受阻返回空，不工程终止、不删改持久化。
        if (e.原因 != 基础结构操作状态::资源失败) {
            try {
                std::wostringstream 文;
                文 << L"需求治理物理读受阻；D=" << 节点.值
                    << L"；physical=" << static_cast<int>(e.原因)
                    << L"；physicalLine=" << e.诊断行;
                // 热路径只记 D/physical，避免控制采集全量扫时二次查询完整结构拖长共享锁占用。
                (void)记录运行日志(L"需求治理", L"读取需求治理信息", 文.str());
            } catch (...) {}
        }
        return std::nullopt;
    }
    catch (新需求目标定义核验状态) { return std::nullopt; }
    catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取需求治理信息", L"正式需求或服务要求结构读取故障");
    }
}

std::optional<新需求治理信息> 新_需求类::读取需求治理信息可抛内部(
    稳定编码 节点,来源候选枚举保护批* 保护,const 基础只读预期* 提议源,
    std::vector<基础联合节点快照>* 提议依赖) const {
    {
        std::lock_guard 锁(新需求互斥);
        if (!结构已初始化() || !有效(节点)) return std::nullopt;
        if(保护&&!保护->仅当前需求)追加来源候选枚举本域保护(节点,*保护);
        const auto q = 全局基础数据集.查询完整节点结构(节点);
        if (q.状态 == 基础结构操作状态::节点不存在) {
            if (保护) throw 新需求目标定义核验状态::前提已变化;
            return std::nullopt;
        }
        if(q.状态 != 基础结构操作状态::已完成) throw 需求物理读取失败{q.状态};
        要求需求物理(q.结构.has_value());
        if (保护) {
            const auto 已采 = std::find_if(保护->完整保护.begin(), 保护->完整保护.end(),
                [&](const auto& p) { return p.结构.节点.编码 == 节点; });
            要求需求物理(已采 != 保护->完整保护.end());
            if (q.结构->节点 != 已采->结构.节点 || q.结构->字段组 != 已采->结构.字段组)
                throw 新需求目标定义核验状态::前提已变化;
            const auto 类型 = 需求物理游标{*q.结构, {}}.引用(节点类型字段, false);
            if (!类型 || *类型 != 需求节点类型) return std::nullopt;
        } else if (!是需求类型节点(节点)) return std::nullopt;
        auto d = 解码需求治理物理(需求目标读取上下文{特征服务_, 特征概念服务_}, *q.结构,false,
            保护 ? &保护->完整保护 : 提议依赖,保护 != nullptr,提议源);
        const auto 核存在=[&](稳定编码 E) {
            if(!保护){要求需求物理(存在服务_.是存在节点(E));return;}
            const auto r=存在服务_.读取存在身份保护(E);
            if(r.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            if(r.前提已变化||r.状态==基础结构操作状态::节点不存在)
                throw 新需求目标定义核验状态::前提已变化;
            要求需求物理(r.状态==基础结构操作状态::已完成&&r.信息&&r.信息->存在节点==E
                &&!r.预期节点组.empty());
            if(!并需求保护(保护->完整保护,r.预期节点组))throw 新需求目标定义核验状态::前提已变化;
        };
        核存在(d.需求主体存在节点);
        if (d.执行域 == 新需求执行域::被认知) {
            要求需求物理(d.已知目标&&d.信息完善阶段);
            if(d.已知目标->目标存在节点)核存在(*d.已知目标->目标存在节点);
            const auto 登记 = 全局基础数据集.查询目标字段(节点, 被认知需求登记字段);
            要求需求物理(登记.size() == 1 && 登记.front().所属节点 == 需求树根节点
                && 全局基础数据集.查询目标字段(节点, 列表成员字段).empty());
        } else {
            要求需求物理(读取唯一节点字段(安全根需求节点, 目标存在字段) == d.需求主体存在节点
                && 读取唯一节点字段(服务根需求节点, 目标存在字段) == d.需求主体存在节点
                && 全局基础数据集.查询目标字段(节点, 被认知需求登记字段).empty());
        }
        if (d.外部要求节点) {
            if(保护&&!部分服务要求读取接口_)throw 新需求目标定义核验状态::依赖未就绪;
            要求需求物理(部分服务要求读取接口_ != nullptr);
            const auto r = 部分服务要求读取接口_->读取部分服务要求(*d.外部要求节点);
            要求需求物理(r.状态 == 新部分服务要求读取状态::已读取 && r.材料
                && r.材料->要求节点 == d.外部要求节点 && 有效(r.材料->来源人类节点));
            if(保护||提议依赖) {
                要求需求物理(!r.发布预期节点组.empty());
                if(!并需求保护(保护 ? 保护->完整保护 : *提议依赖,r.发布预期节点组))
                    throw 新需求目标定义核验状态::前提已变化;
            }
            d.外部要求材料 = r.材料;
            if (d.完整目标) {
                const auto& g = r.材料->已知目标;
                const auto& t = d.完整目标->目标;
                要求需求物理(g.目标存在节点 == d.完整目标->目标存在节点
                    && g.目标特征类型概念节点 == t.目标特征类型概念节点
                    && g.目标形式 == t.目标形式 && g.目标值域概念节点 == t.目标值域概念节点
                    && g.目标比较关系概念节点 == t.目标比较关系概念节点 && g.目标变化方向 == t.目标变化方向
                    && r.材料->确认节点 && d.服务要求确认组 == std::vector<稳定编码>{*r.材料->确认节点});
            }
        }
        return d;
    }
}

std::optional<新需求治理信息> 新_需求类::读取当前需求治理信息(稳定编码 节点) const {
    try { return 读取需求治理信息可抛内部(节点); }
    catch(const 需求物理读取失败& e) {
        if(e.原因==基础结构操作状态::资源失败)throw std::bad_alloc{};
        throw;
    }
}

std::optional<新需求信息> 新_需求类::获取需求(
    稳定编码 需求节点) const noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        const auto d = 读取需求治理信息(需求节点);
        return d&&d->完整目标&&(d->执行域!=新需求执行域::被认知||d->信息完善阶段==新需求信息完善阶段::已完善)
            ? std::optional{投影完整需求(*d)} : std::nullopt;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取完整需求投影", L"正式需求读取故障");
    }
}

std::optional<新需求列表项信息> 新_需求类::获取需求列表项(
    稳定编码 列表项节点) const noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        return 读取列表项信息(列表项节点);
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<稳定编码> 新_需求类::查询根需求(
    新需求根角色 角色) const noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        const auto 节点 = 角色 == 新需求根角色::安全
            ? 安全根需求节点 : 服务根需求节点;
        const auto 信息 = 读取需求信息(需求目标读取上下文{特征服务_, 特征概念服务_}, 节点);
        return 信息 && 信息->根角色 == 角色
            ? std::optional<稳定编码>{节点} : std::nullopt;
    } catch (...) {
        return std::nullopt;
    }
}

新需求全集结果 新_需求类::查询全部需求() const noexcept {
    auto 读取 = 查询全部需求(std::stop_token{});
    if (auto* 结果 = std::get_if<新需求全集结果>(&读取)) return std::move(*结果);
    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取全部需求", L"无停止能力的读取返回取消或无值结果。");
}

新需求全集可停止结果 新_需求类::查询全部需求(std::stop_token 停止) const noexcept {
    try { return 查询全部需求可抛内部(停止); }
    catch (const std::bad_alloc&) { return 新需求全集结果{新需求操作状态::资源失败,{}}; }
    catch (const 需求物理读取失败& e) {
        if(e.原因==基础结构操作状态::资源失败)return 新需求全集结果{新需求操作状态::资源失败,{}};
        // 3100：结构不一致不得折算为空成功集；旧布局受阻返回结构化状态，不工程终止。
        try {
            std::wostringstream 文;
            文 << L"需求全集物理读受阻；physical=" << static_cast<int>(e.原因)
                << L"；physicalLine=" << e.诊断行;
            (void)记录运行日志(L"需求治理", L"读取全部需求", 文.str());
        } catch (...) {}
        return 新需求全集结果{新需求操作状态::结构不一致,{}};
    } catch (新需求目标定义核验状态 s) {
        if(s==新需求目标定义核验状态::前提已变化)return 新需求全集结果{新需求操作状态::前提已变化,{}};
        if(s==新需求目标定义核验状态::资源失败)return 新需求全集结果{新需求操作状态::资源失败,{}};
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取全部需求", L"需求全集收到未约定的目标读取状态");
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取全部需求", L"需求全集读取工程故障"); }
}

新需求全集可停止结果 新_需求类::查询全部需求可抛内部(
    std::stop_token 停止,来源候选枚举保护批* 保护) const {
    新需求全集结果 结果;
    try {
        auto 锁 = 获取需求读取锁(停止);
        if(保护)追加来源候选枚举固定保护(*保护);
        if (!结构仍存在()) {
            结果.状态 = 新需求操作状态::尚未初始化;
            return 结果;
        }
        检查需求读取停止(停止);
        if(保护)追加来源候选枚举本域保护(安全根需求节点,*保护);
        const auto 安全 = 读取需求信息(需求目标读取上下文{特征服务_, 特征概念服务_}, 安全根需求节点);
        if (安全) 检查需求读取停止(停止);
        if(保护)追加来源候选枚举本域保护(服务根需求节点,*保护);
        const auto 服务 = 读取需求信息(需求目标读取上下文{特征服务_, 特征概念服务_}, 服务根需求节点);
        if (!安全 || !服务 || 安全根需求节点 == 服务根需求节点
            || 安全->根角色 != 新需求根角色::安全
            || 服务->根角色 != 新需求根角色::服务
            || 安全->父需求节点 || 服务->父需求节点) {
            结果.状态 = 新需求操作状态::结构不一致;
            return 结果;
        }
        检查需求读取停止(停止);
        (void)读取需求根登记物理();
        检查需求读取停止(停止);

        std::vector<稳定编码> 待访问{安全根需求节点, 服务根需求节点};
        std::set<稳定编码> 已发现{安全根需求节点, 服务根需求节点};
        for (std::size_t 位置 = 0; 位置 < 待访问.size(); ++位置) {
            检查需求读取停止(停止);
            const auto 父 = 待访问[位置];
            const auto 父信息 = 读取需求治理信息可抛内部(父,保护);
            if (!父信息 || 父信息->执行域 != 新需求执行域::自我执行) {
                结果.状态 = 新需求操作状态::结构不一致;
                结果.需求节点组.clear();
                return 结果;
            }
            检查需求读取停止(停止);
            (void)核验需求列表物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 投影完整需求(*父信息));
            检查需求读取停止(停止);
            const auto 子关系 = 查询子需求关系(父);
            检查需求读取停止(停止);
            for (const auto& 关系 : 子关系) {
                检查需求读取停止(停止);
                const auto 子 = 关系.目标节点;
                const auto 子信息 = 读取需求治理信息可抛内部(子,保护);
                if (!子信息 || 子信息->执行域 != 新需求执行域::自我执行 || 子信息->父需求节点 != 父
                    || 子信息->根角色 || !已发现.insert(子).second) {
                    结果.状态 = 新需求操作状态::结构不一致;
                    结果.需求节点组.clear();
                    return 结果;
                }
                检查需求读取停止(停止);
                待访问.push_back(子);
            }
            检查需求读取停止(停止);
        }

        std::vector<稳定编码> 全部类型节点;
        检查需求读取停止(停止);
        const auto 类型字段 = 全局基础数据集.查询目标字段(需求节点类型, 节点类型字段);
        检查需求读取停止(停止);
        for (const auto& 字段 : 类型字段) {
            检查需求读取停止(停止);
            if(保护) {
                if(已发现.contains(字段.所属节点))
                    追加来源候选枚举本域保护(字段.所属节点,*保护);
                else (void)取得服务实例物理保护(字段.所属节点,保护->完整保护);
            }
            if (!是需求类型节点(字段.所属节点)) {
                结果.状态 = 新需求操作状态::结构不一致;
                return 结果;
            }
            检查需求读取停止(停止);
            全部类型节点.push_back(字段.所属节点);
        }
        检查需求读取停止(停止);
        std::sort(全部类型节点.begin(), 全部类型节点.end());
        if (std::adjacent_find(全部类型节点.begin(), 全部类型节点.end())
                != 全部类型节点.end()) {
            结果.状态 = 新需求操作状态::结构不一致;
            return 结果;
        }
        检查需求读取停止(停止);
        std::sort(待访问.begin(), 待访问.end());
        检查需求读取停止(停止);
        auto 两域并集 = 待访问;
        const auto 被认知登记 = 全局基础数据集.查询字段(需求树根节点, 被认知需求登记字段);
        检查需求读取停止(停止);
        for (const auto& f : 被认知登记) {
            检查需求读取停止(停止);
            const auto* 编码 = std::get_if<稳定编码>(&f.内容);
            if (!编码 || !有效(*编码) || !已发现.insert(*编码).second) {
                结果.状态 = 新需求操作状态::结构不一致; return 结果;
            }
            // 自我全集只需确认登记节点不冒充自我；其内容归全记录查询核验。
            if(读取唯一I64字段(*编码,需求执行域字段)
                !=static_cast<std::int64_t>(新需求执行域::被认知)) {
                结果.状态 = 新需求操作状态::结构不一致; return 结果;
            }
            检查需求读取停止(停止);
            两域并集.push_back(*编码);
        }
        检查需求读取停止(停止);
        std::sort(两域并集.begin(), 两域并集.end());
        if (两域并集 != 全部类型节点) {
            结果.状态 = 新需求操作状态::结构不一致;
            return 结果;
        }
        检查需求读取停止(停止);
        结果.需求节点组 = std::move(待访问);
        结果.状态 = 新需求操作状态::已找到;
        检查需求读取停止(停止);
        return 结果;
    } catch (const 新需求读取取消&) {
        return 新需求读取取消{};
    }
}

新需求记录全集结果 新_需求类::查询全部需求记录() const noexcept {
    try {
        return 查询全部需求记录可抛内部();
    } catch(const std::bad_alloc&) {return {新需求操作状态::资源失败,{},{}};}
    catch(const 需求物理读取失败& e) {
        if(e.原因==基础结构操作状态::资源失败)return {新需求操作状态::资源失败,{},{}};
        try {
            std::wostringstream 文;
            文 << L"需求记录全集物理读受阻；physical=" << static_cast<int>(e.原因)
                << L"；physicalLine=" << e.诊断行;
            (void)记录运行日志(L"需求治理", L"读取全部需求记录", 文.str());
        } catch (...) {}
        return {新需求操作状态::结构不一致,{},{}};
    } catch(新需求目标定义核验状态 s) {
        if(s==新需求目标定义核验状态::前提已变化)return {新需求操作状态::前提已变化,{},{}};
        if(s==新需求目标定义核验状态::资源失败)return {新需求操作状态::资源失败,{},{}};
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取全部需求记录", L"需求记录全集收到未约定的目标状态");
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取全部需求记录", L"需求记录全集读取故障"); }
}

新需求记录全集结果 新_需求类::查询全部需求记录可抛内部(来源候选枚举保护批* 保护,
    std::vector<新需求治理信息>* 同次完整治理组) const {
        if(同次完整治理组)同次完整治理组->clear();
        std::lock_guard 锁(新需求互斥);
        const auto 全集 = 查询全部需求可抛内部(std::stop_token{},保护);
        const auto* 结果=std::get_if<新需求全集结果>(&全集);
        要求需求物理(结果!=nullptr);
        const auto& 自我=*结果;
        if (自我.状态 != 新需求操作状态::已找到) return {自我.状态, {}};
        新需求记录全集结果 r{新需求操作状态::已找到, {}};
        std::vector<新需求治理信息> 完整治理;
        std::set<std::pair<稳定编码, 稳定编码>> 认知键;
        auto 编码组 = 自我.需求节点组;
        const auto 外部 = 读取多节点字段(需求树根节点, 被认知需求登记字段);
        编码组.insert(编码组.end(), 外部.begin(), 外部.end());
        std::sort(编码组.begin(), 编码组.end());
        for (const auto 编码 : 编码组) {
            const auto d=读取需求治理信息可抛内部(编码,保护);要求需求物理(d.has_value());
            if(d->执行域==新需求执行域::被认知&&d->认知来源
                &&!认知键.emplace(d->需求主体存在节点,d->认知来源->理解节点).second)
                return {新需求操作状态::结构不一致,{},{}};
            if(d->执行域==新需求执行域::被认知&&d->信息完善阶段==新需求信息完善阶段::待完善)r.待完善需求组.push_back(*d);
            else {
                要求需求物理(d->完整目标.has_value());r.需求组.push_back(投影完整需求(*d));
                if(同次完整治理组)完整治理.push_back(*d);
            }
        }
        if(同次完整治理组)*同次完整治理组=std::move(完整治理);
        return r;
}

新需求记录全集结果 新_需求类::查询主体需求(稳定编码 主体存在, 新需求执行域 域) const noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        if (域 != 新需求执行域::自我执行 && 域 != 新需求执行域::被认知)
            return {新需求操作状态::执行域不适用, {}};
        if (!存在服务_.是存在节点(主体存在)) return {新需求操作状态::目标存在不合法, {}};
        auto r = 查询全部需求记录();
        if (r.状态 == 新需求操作状态::已找到) {
            std::erase_if(r.需求组, [&](const auto& d) { return d.需求主体存在节点 != 主体存在 || d.执行域 != 域; });
            std::erase_if(r.待完善需求组,[&](const auto& d){return d.需求主体存在节点!=主体存在||d.执行域!=域;});
        }
        return r;
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查询主体需求", L"主体需求查询故障"); }
}

新需求记录全集结果 新_需求类::查询被认知需求来源(稳定编码 理解节点) const noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        if (!有效(理解节点) || !节点仍存在(理解节点)) return {新需求操作状态::认知来源不合法, {}};
        auto r = 查询全部需求记录();
        if (r.状态 == 新需求操作状态::已找到) {
            std::erase_if(r.需求组, [&](const auto& d) {
                return d.执行域 != 新需求执行域::被认知 || !d.认知来源 || d.认知来源->理解节点 != 理解节点;
            });
            std::erase_if(r.待完善需求组,[&](const auto& d){return d.执行域!=新需求执行域::被认知||!d.认知来源||d.认知来源->理解节点!=理解节点;});
        }
        return r;
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查询认知需求来源", L"认知需求来源查询故障"); }
}

namespace {
bool 需求保护相同(const 基础联合节点快照& a,const 基础联合节点快照& b) {
    return a.结构.节点==b.结构.节点&&a.结构.字段组==b.结构.字段组&&a.入向字段组==b.入向字段组
        &&a.入向外部关系组==b.入向外部关系组&&a.出向外部关系组==b.出向外部关系组;
}
bool 并需求保护(std::vector<基础联合节点快照>& out,const std::vector<基础联合节点快照>& in) {
    for(const auto& x:in){const auto it=std::find_if(out.begin(),out.end(),[&](const auto& p){return p.结构.节点==x.结构.节点;});
        if(it==out.end())out.push_back(x);else if(!需求保护相同(*it,x))return false;}return true;
}
bool 需求内容保护未变(const std::vector<基础联合节点快照>& 保护) {
    std::map<稳定编码,const 基础联合节点快照*> 原组;
    for(const auto& 项:保护) {
        const auto [位置,新增]=原组.emplace(项.结构.节点.编码,&项);
        if(!新增&&!需求保护相同(*位置->second,项))return false;
    }
    std::vector<稳定编码> 节点组;
    for(const auto& [节点,快照]:原组)节点组.push_back(节点);
    const auto 当前=全局基础数据集.查询节点组完整结构(节点组);
    if(当前.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
    要求需求物理(当前.状态==基础结构操作状态::已完成&&当前.节点组.size()==节点组.size());
    for(std::size_t i=0;i<节点组.size();++i) {
        const auto& 项=当前.节点组[i];
        要求需求物理(项.查询节点==节点组[i]&&(!项.快照||项.快照->结构.节点.编码==节点组[i]));
        if(!项.快照||!需求保护相同(*原组.at(节点组[i]),*项.快照))return false;
    }
    return true;
}
正式直接内容项 投影需求内容(稳定编码 FT,std::uint32_t 槽,const 新需求治理信息& 需求) {
    using namespace 先天特征类型槽;
    using 值类=正式内容值种类;
    using 状态=正式内容值状态;
    正式直接内容项 项{FT,{值类::I64数值,状态::当前未知,{}}};
    const auto 引用=[&](值类 类型,std::optional<稳定编码> 值) {
        项.内容.种类=类型;
        if(值){要求需求物理(有效(*值));项.内容.值=正式内容准确值{*值};项.内容.状态=状态::已知;}
    };
    const auto 数值=[&](std::optional<std::int64_t> 值) {
        if(值){项.内容.值=正式内容准确值{*值};项.内容.状态=状态::已知;}
    };
    const auto 目标=需求.已知目标 ? *需求.已知目标
        :需求.完整目标 ? 转已知需求目标(*需求.完整目标) :新部分服务要求目标{};
    const bool 被认知=需求.执行域==新需求执行域::被认知;
    switch(槽) {
    case 需求主体:引用(值类::存在引用,需求.需求主体存在节点);break;
    case 需求执行域:数值(static_cast<std::int64_t>(需求.执行域));break;
    case 需求权重:数值(需求.权重);break;
    case 需求活动:数值(static_cast<std::int64_t>(需求.活动状态));break;
    case 需求目标存在:引用(值类::存在引用,目标.目标存在节点);break;
    case 需求目标FT:引用(值类::概念引用,目标.目标特征类型概念节点);break;
    case 需求目标形式:if(目标.目标形式)数值(static_cast<std::int64_t>(*目标.目标形式));break;
    case 目标域:引用(值类::概念引用,目标.目标值域概念节点);
        if(目标.目标形式==新需求目标形式::变化方向)项.内容.状态=状态::不适用;break;
    case 目标比较:引用(值类::概念引用,目标.目标比较关系概念节点);
        if(目标.目标形式==新需求目标形式::变化方向)项.内容.状态=状态::不适用;break;
    case 目标方向:if(目标.目标变化方向)数值(static_cast<std::int64_t>(*目标.目标变化方向));
        if(目标.目标形式==新需求目标形式::值)项.内容.状态=状态::不适用;break;
    case 父需求:引用(值类::需求引用,需求.父需求节点);
        if(被认知||需求.根角色)项.内容.状态=状态::不适用;break;
    case 需求来源:if(需求.来源类型)数值(static_cast<std::int64_t>(*需求.来源类型));
        if(被认知||需求.根角色)项.内容.状态=状态::不适用;break;
    case 需求根角色:if(需求.根角色)数值(static_cast<std::int64_t>(*需求.根角色));
        else 项.内容.状态=状态::不适用;break;
    case 理解来源:引用(值类::语言理解引用,需求.认知来源
        ?std::optional{需求.认知来源->理解节点}:std::nullopt);
        if(!被认知)项.内容.状态=状态::不适用;break;
    case 内容确认来源:引用(值类::内容确认引用,需求.认知来源
        ?std::optional{需求.认知来源->内容确认节点}:std::nullopt);
        if(!被认知)项.内容.状态=状态::不适用;break;
    default:要求需求物理(false);
    }
    要求需求物理((项.内容.状态==状态::已知)==项.内容.值.has_value());
    return 项;
}
bool 加需求存在保护(稳定编码 id,新_存在类& E,std::vector<基础联合节点快照>& protect) {
    const auto p=E.读取存在身份保护(id);
    if(p.前提已变化)throw 新需求目标定义核验状态::前提已变化;
    if(p.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
    if(p.状态==基础结构操作状态::入口拒绝||p.状态==基础结构操作状态::节点不存在)return false;
    if(p.状态!=基础结构操作状态::已完成||!p.信息||p.信息->存在节点!=id||p.预期节点组.empty())
        throw std::runtime_error("partial demand existence identity protection unavailable");
    if(!并需求保护(protect,p.预期节点组))throw 新需求目标定义核验状态::前提已变化;
    return true;
}
新需求操作状态 采集正式需求引用语义(
    const 新需求治理信息& 需求,
    新_存在类& 存在,
    概念_特征类& 特征概念,
    概念_二次特征类& 二次特征概念,
    正式信息参数投影& 输出,
    std::vector<基础联合节点快照>& 联合保护) {
    using 状态类=新需求操作状态;
    if(需求.完整目标&&需求.完整目标->目标.新增成员来源见证)return 状态类::目标内容未支持;
    try {
        // 稳定坏结构是工程错误；若已有D/F或定义保护改变，则只返回前提变化。
        const auto 要求稳定结构=[&](bool 成立) {
            if(成立)return;
            if(!需求内容保护未变(联合保护))throw 状态类::前提已变化;
            throw std::runtime_error("formal demand reference semantics invariant broken");
        };
        std::map<稳定编码,特征概念定义保护信息> 特征组;
        std::map<稳定编码,二次特征概念信息> 二次组;
        const auto 取特征=[&](稳定编码 概念节点)->const 特征概念定义保护信息& {
            if(const auto 位置=特征组.find(概念节点);位置!=特征组.end())return 位置->second;
            要求稳定结构(有效(概念节点));
            const auto 读取=特征概念.读取特征概念定义保护(概念节点);
            if(读取.前提已变化)throw 状态类::前提已变化;
            if(读取.状态==基础结构操作状态::资源失败)throw 状态类::资源失败;
            要求稳定结构(读取.状态==基础结构操作状态::已完成&&读取.信息
                &&读取.信息->定义.节点==概念节点&&有效(读取.信息->类型根)&&!读取.预期节点组.empty());
            if(!并需求保护(联合保护,读取.预期节点组))throw 状态类::前提已变化;
            return 特征组.emplace(概念节点,*读取.信息).first->second;
        };
        const auto 采特征=[&](稳定编码 概念节点)->const 特征概念定义保护信息& {
            const auto& 定义=取特征(概念节点);
            const auto& 根=取特征(定义.类型根);
            要求稳定结构(根.类型根==根.定义.节点);
            return 定义;
        };
        const auto 取二次=[&](稳定编码 概念节点)->const 二次特征概念信息& {
            if(const auto 位置=二次组.find(概念节点);位置!=二次组.end())return 位置->second;
            要求稳定结构(有效(概念节点));
            const auto 读取=二次特征概念.读取二次特征定义保护(概念节点);
            if(读取.前提已变化)throw 状态类::前提已变化;
            if(读取.状态==基础结构操作状态::资源失败)throw 状态类::资源失败;
            要求稳定结构(读取.状态==基础结构操作状态::已完成&&读取.信息
                &&读取.信息->节点==概念节点&&有效(读取.信息->二次特征类型根节点)&&!读取.预期节点组.empty());
            if(!并需求保护(联合保护,读取.预期节点组))throw 状态类::前提已变化;
            return 二次组.emplace(概念节点,*读取.信息).first->second;
        };
        const auto 采二次=[&](稳定编码 概念节点)->const 二次特征概念信息& {
            const auto& 定义=取二次(概念节点);
            const auto& 根=取二次(定义.二次特征类型根节点);
            要求稳定结构(根.二次特征类型根节点==根.节点);
            for(const auto* 项:{&定义,&根}) {
                要求稳定结构(项->输出特征概念.节点==项->节点);
                const auto& 输出定义=采特征(项->输出特征概念.节点);
                要求稳定结构(输出定义.定义==项->输出特征概念
                    &&输出定义.类型根==项->二次特征类型根节点);
                if(项->参照特征概念节点)采特征(*项->参照特征概念节点);
                for(const auto& 输入:项->输入定义组)
                    if(输入.特征类型概念节点)采特征(*输入.特征类型概念节点);
            }
            return 定义;
        };
        正式引用概念语义 语义;
        const auto 收束=[&]() {
            for(auto& [概念节点,定义]:特征组)语义.特征概念定义组.push_back(std::move(定义));
            for(auto& [概念节点,定义]:二次组)语义.二次特征定义组.push_back(std::move(定义));
            输出.引用概念语义=std::move(语义);
            return 状态类::已找到;
        };
        要求稳定结构(需求.执行域==新需求执行域::自我执行||需求.执行域==新需求执行域::被认知);
        要求稳定结构(需求.执行域!=新需求执行域::自我执行||需求.完整目标.has_value());

        // 必须先从原完整目标保全定位；部分视图不承载它，不能先投影后丢失。
        if(需求.完整目标&&需求.完整目标->目标.引用内容定位) {
            const auto& 完整=*需求.完整目标;
            要求稳定结构(目标形状合法(完整.目标));
            const auto& 定位=*完整.目标.引用内容定位;
            语义.引用内容定位=正式内容引用定位{定位.引用需求节点,定位.正式内容路径};
            if(!加需求存在保护(完整.目标存在节点,存在,联合保护))要求稳定结构(false);
            const auto& 类型定义=采特征(完整.目标.目标特征类型概念节点);
            要求稳定结构(类型定义.类型根==完整.目标.目标特征类型概念节点);
            语义.状态=正式引用概念语义状态::引用内容未展开;
            // 域C可能属于CE，不调用FC/RC读口、不要求RC，也不展开路径。
            return 收束();
        }

        要求稳定结构(需求.完整目标.has_value()||需求.已知目标.has_value());
        const auto 目标=需求.完整目标 ? 转已知需求目标(*需求.完整目标) : *需求.已知目标;
        要求稳定结构(部分需求目标形状合法(目标));
        if(需求.完整目标)要求稳定结构(目标形状合法(需求.完整目标->目标));
        if(目标.目标存在节点&&!加需求存在保护(*目标.目标存在节点,存在,联合保护))要求稳定结构(false);
        if(目标.目标特征类型概念节点) {
            const auto& 类型定义=采特征(*目标.目标特征类型概念节点);
            要求稳定结构(类型定义.类型根==*目标.目标特征类型概念节点);
        }
        if(目标.目标值域概念节点) {
            const auto& 域=采特征(*目标.目标值域概念节点);
            if(目标.目标特征类型概念节点)要求稳定结构(域.类型根==*目标.目标特征类型概念节点);
        }
        if(目标.目标比较关系概念节点) {
            const auto& 比较=采二次(*目标.目标比较关系概念节点);
            if(目标.目标值域概念节点)要求稳定结构(比较.参照特征概念节点==目标.目标值域概念节点);
            if(目标.目标特征类型概念节点)for(const auto& 输入:比较.输入定义组)
                if(输入.来源种类==二次特征输入来源种类::特征准确值)
                    要求稳定结构(输入.特征类型概念节点
                        &&特征组.at(*输入.特征类型概念节点).类型根==*目标.目标特征类型概念节点);
            if(需求.完整目标)要求稳定结构(比较关系适用纯判据(比较,*目标.目标特征类型概念节点,
                *目标.目标值域概念节点,[&](稳定编码 概念节点)->std::optional<稳定编码> {
                    const auto 位置=特征组.find(概念节点);
                    return 位置==特征组.end()?std::nullopt:std::optional{位置->second.类型根};
                }));
        }
        语义.状态=需求.完整目标 ? 正式引用概念语义状态::完整 : 正式引用概念语义状态::部分;
        return 收束();
    }catch(状态类 状态) {
        if(状态==状态类::前提已变化||状态==状态类::资源失败||状态==状态类::尚未初始化)return 状态;
        throw;
    }
}
新需求操作状态 核验部分需求目标(const 新部分服务要求目标& t,新_存在类& E,概念_特征类& F,
    概念_二次特征类& R,std::vector<基础联合节点快照>& protect) {
    using S=新需求操作状态;
    if(!部分需求目标形状合法(t))return S::目标形式不合法;
    if(t.目标存在节点&&(!E.是存在节点(*t.目标存在节点)||!加需求存在保护(*t.目标存在节点,E,protect)))return S::目标存在不合法;
    const auto feature=[&](稳定编码 C,bool root)->std::optional<稳定编码> {
        const auto p=F.读取特征概念定义保护(C);
        if(p.前提已变化)throw 新需求目标定义核验状态::前提已变化;
        if(p.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        if(p.状态==基础结构操作状态::入口拒绝||p.状态==基础结构操作状态::节点不存在)return {};
        if(p.状态!=基础结构操作状态::已完成)throw std::runtime_error("partial demand feature definition read failed");
        if(!p.信息)throw std::runtime_error("partial demand feature definition projection missing");
        if(!并需求保护(protect,p.预期节点组))throw 新需求目标定义核验状态::前提已变化;
        if(root&&p.信息->类型根!=C)return {};
        return p.信息->类型根;
    };
    if(t.目标特征类型概念节点&&!feature(*t.目标特征类型概念节点,true))return S::目标特征类型不合法;
    if(t.目标值域概念节点){const auto root=feature(*t.目标值域概念节点,false);
        if(!root||(t.目标特征类型概念节点&&*root!=*t.目标特征类型概念节点))return S::目标值域不合法;}
    if(t.目标比较关系概念节点){const auto p=R.读取二次特征定义保护(*t.目标比较关系概念节点);
        if(p.前提已变化)throw 新需求目标定义核验状态::前提已变化;
        if(p.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        if(p.状态==基础结构操作状态::入口拒绝||p.状态==基础结构操作状态::节点不存在)return S::比较关系不合法;
        if(p.状态!=基础结构操作状态::已完成)throw std::runtime_error("partial demand comparison definition read failed");
        if(!p.信息)throw std::runtime_error("partial demand comparison definition projection missing");
        if(!并需求保护(protect,p.预期节点组))throw 新需求目标定义核验状态::前提已变化;
        if(t.目标值域概念节点&&p.信息->参照特征概念节点!=t.目标值域概念节点)return S::比较关系不合法;
        if(t.目标特征类型概念节点)for(const auto& i:p.信息->输入定义组)
            if(i.来源种类==二次特征输入来源种类::特征准确值&&(!i.特征类型概念节点
                ||feature(*i.特征类型概念节点,false)!=t.目标特征类型概念节点))return S::比较关系不合法;
    }
    if(const auto full=投影已知需求目标(t)){
        const auto p=核验需求目标定义保护(full->目标,F,R);
        if(p.状态==新需求目标定义核验状态::前提已变化)throw p.状态;
        if(p.状态==新需求目标定义核验状态::资源失败)throw std::bad_alloc{};
        if(p.状态==新需求目标定义核验状态::内部不一致)throw std::runtime_error("partial demand target definition corrupt");
        if(p.状态!=新需求目标定义核验状态::已完成)return S::比较关系不合法;
        if(!并需求保护(protect,p.完整保护))throw 新需求目标定义核验状态::前提已变化;
    }return S::已找到;
}
std::map<稳定编码,基础字段内容> 需求已知字段(const 新部分服务要求目标& t,新需求信息完善阶段 phase,
    const std::optional<新需求认知来源>& source) {
    std::map<稳定编码,基础字段内容> f;
    f.emplace(需求信息完善阶段字段,基础值{static_cast<std::int64_t>(phase)});
    if(t.目标存在节点)f.emplace(目标存在字段,*t.目标存在节点);
    if(t.目标特征类型概念节点)f.emplace(目标特征类型字段,*t.目标特征类型概念节点);
    if(t.目标形式)f.emplace(目标形式字段,基础值{static_cast<std::int64_t>(*t.目标形式)});
    if(t.目标值域概念节点)f.emplace(目标值域字段,*t.目标值域概念节点);
    if(t.目标比较关系概念节点)f.emplace(目标比较关系字段,*t.目标比较关系概念节点);
    if(t.目标变化方向)f.emplace(目标变化方向字段,基础值{static_cast<std::int64_t>(*t.目标变化方向)});
    if(source){f.emplace(认知理解字段,source->理解节点);f.emplace(认知内容确认字段,source->内容确认节点);
        if(source->来源要求节点)f.emplace(认知来源要求字段,*source->来源要求节点);}return f;
}
}
bool operator==(const 新需求新增成员基准结果& a,const 新需求新增成员基准结果& b) {
    return a.状态==b.状态&&a.基准==b.基准&&a.完整保护.size()==b.完整保护.size()
        &&std::equal(a.完整保护.begin(),a.完整保护.end(),b.完整保护.begin(),需求保护相同);
}
bool operator==(const 新需求新增成员判定结果& a,const 新需求新增成员判定结果& b) {
    return a.状态==b.状态&&a.当前集合==b.当前集合&&a.原始新增数量==b.原始新增数量&&a.见证组==b.见证组
        &&a.完整保护.size()==b.完整保护.size()&&std::equal(a.完整保护.begin(),a.完整保护.end(),b.完整保护.begin(),需求保护相同);
}
bool 新_需求类::装配新增成员读取接口(const 新需求新增成员读取接口* p) noexcept {
    try {std::lock_guard lock(新需求互斥);
        if(p&&新增成员读取_&&新增成员读取_!=p)return false;
        新增成员读取_=p;return true;
    }catch(...){return false;}
}
新需求目标定义核验结果 新_需求类::核验新增成员目标定义保护(const 新需求完整目标& t) const noexcept {
    using S=新需求目标定义核验状态;
    try {
        if(!t.目标.新增成员来源见证||!目标形状合法(t.目标))return {};
        auto plain=t.目标;plain.新增成员来源见证.reset();
        auto r=核验需求目标定义保护(plain,特征概念服务_,二次特征概念服务_);
        if(r.状态!=S::已完成)return r;
        const auto& x=*t.目标.新增成员来源见证;
        const auto selfRoot=获取需求(服务根需求节点);
        if(!selfRoot||selfRoot->根角色!=新需求根角色::服务||selfRoot->目标存在节点!=t.目标存在节点)return {S::未支持,{}};
        const auto innate=特征概念服务_.获取先天特征概念();
        if(!innate)return {S::依赖未就绪,{}};
        if(x.要求.源特征类型概念节点!=innate->服务任务集合)return {S::未支持,{}};
        const auto ft=特征概念服务_.读取特征概念定义保护(x.要求.源特征类型概念节点);
        if(ft.状态==基础结构操作状态::资源失败)return {S::资源失败,{}};
        if(ft.前提已变化)return {S::前提已变化,{}};
        if(ft.状态!=基础结构操作状态::已完成||!ft.信息
            ||ft.信息->类型根!=x.要求.源特征类型概念节点)return {};
        const auto& sourceDefinition=ft.信息->定义;
        if(sourceDefinition.材料类型!=特征概念材料物理类型::U64数组||sourceDefinition.单位
            ||sourceDefinition.比较规则!=特征概念比较规则::服务任务集合身份判等
            ||sourceDefinition.聚合规则!=特征概念聚合规则::不自动聚合)return {S::未支持,{}};
        const auto def=二次特征概念服务_.读取二次特征定义保护(plain.目标特征类型概念节点);
        if(def.状态==基础结构操作状态::资源失败)return {S::资源失败,{}};
        if(def.前提已变化)return {S::前提已变化,{}};
        if(def.状态!=基础结构操作状态::已完成||!def.信息)return {S::未支持,{}};
        const auto& k=*def.信息;
        const auto domain=特征概念服务_.获取特征概念(*plain.目标值域概念节点);
        if(k.算法!=二次特征算法::服务任务来源新增数||k.输入定义组.size()!=2||k.参照特征概念节点||k.二次特征类型根节点!=k.节点
            ||k.输出特征概念.材料类型!=特征概念材料物理类型::I64标量||k.输出特征概念.单位
            ||k.输出特征概念.值域!=特征概念值域{std::vector<特征概念I64闭区间>{{0,(std::numeric_limits<std::int64_t>::max)()}}}
            ||!domain||domain->单位||domain->材料类型!=特征概念材料物理类型::I64标量
            ||domain->值域!=特征概念值域{std::vector<特征概念I64闭区间>{{1,(std::numeric_limits<std::int64_t>::max)()}}})return {S::未支持,{}};
        for(std::size_t i=0;i<2;++i){const auto& in=k.输入定义组[i];
            if(in.特征类型概念节点!=x.要求.源特征类型概念节点||in.来源种类!=二次特征输入来源种类::特征准确值
                ||in.顺序!=static_cast<std::int64_t>(i)||in.角色!=(i==0?二次特征输入角色::左输入:二次特征输入角色::右输入))return {S::未支持,{}};}
        const auto compare=二次特征概念服务_.获取二次特征概念(*plain.目标比较关系概念节点);
        if(!compare||compare->算法!=二次特征算法::有向特征差值||compare->输入定义组.size()!=2
            ||compare->参照特征概念节点!=plain.目标值域概念节点||compare->结果域种类组.empty()
            ||!std::all_of(compare->结果域种类组.begin(),compare->结果域种类组.end(),[](auto kind){return kind==二次特征结果域种类::包含于参照值域;}))return {S::未支持,{}};
        for(std::size_t i=0;i<2;++i){const auto& in=compare->输入定义组[i];
            if(in.特征类型概念节点!=plain.目标特征类型概念节点||in.来源种类!=二次特征输入来源种类::特征准确值
                ||in.顺序!=static_cast<std::int64_t>(i)||in.角色!=(i==0?二次特征输入角色::左输入:二次特征输入角色::右输入))return {S::未支持,{}};}
        const auto material=特征值服务_.获取特征值(x.基准材料节点);
        const auto* members=material?std::get_if<std::vector<std::uint64_t>>(&material->材料):nullptr;
        要求需求物理(material&&material->物理类型==材料物理类型::U64数组&&members
            &&std::is_sorted(members->begin(),members->end())&&std::adjacent_find(members->begin(),members->end())==members->end());
        if(!并需求保护(r.完整保护,ft.预期节点组)||!并需求保护(r.完整保护,def.预期节点组))return {S::前提已变化,{}};
        if(!加需求存在保护(t.目标存在节点,存在服务_,r.完整保护))return {S::前提已变化,{}};
        if(x.要求.指定来源主体&&!加需求存在保护(*x.要求.指定来源主体,存在服务_,r.完整保护))return {S::前提已变化,{}};
        std::vector<稳定编码> ids{x.基准材料节点};
        if(x.要求.指定被认知需求){
            const auto d=读取需求治理信息(*x.要求.指定被认知需求);
            if(!d||d->执行域!=新需求执行域::被认知||d->需求主体存在节点!=*x.要求.指定来源主体)return {};
            ids.push_back(d->节点);
        }
        const auto nodes=全局基础数据集.查询节点组完整结构(ids);
        if(nodes.状态==基础结构操作状态::资源失败)return {S::资源失败,{}};
        要求需求物理(nodes.状态==基础结构操作状态::已完成&&nodes.节点组.size()==ids.size());
        for(const auto& n:nodes.节点组){if(!n.快照)return {S::前提已变化,{}};
            if(!并需求保护(r.完整保护,{*n.快照}))return {S::前提已变化,{}};}
        const auto 当前服务根=获取需求(服务根需求节点);
        if(!当前服务根||当前服务根->根角色!=新需求根角色::服务||当前服务根->目标存在节点!=t.目标存在节点
            ||!需求内容保护未变(r.完整保护))return {S::前提已变化,{}};
        return r;
    }catch(const 新需求目标定义核验状态 s){要求需求物理(s==新需求目标定义核验状态::前提已变化);return {S::前提已变化,{}};}
    catch(const std::bad_alloc&){return {S::资源失败,{}};}
    catch(const std::length_error&){return {S::资源失败,{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"核验新增成员目标",L"正式目标定义读取发生工程故障");}
}
新需求新增成员判定结果 新_需求类::读取新增成员来源见证(稳定编码 D) const noexcept {
    return 读取新增成员来源见证内部(D,nullptr);
}
新需求新增成员判定结果 新_需求类::读取新增成员来源见证内部(稳定编码 D,提议源收集批* 源) const noexcept {
    using S=新需求新增成员读取状态;
    try {
        const 新需求新增成员读取接口* reader=nullptr;std::optional<新需求信息> d;
        std::vector<基础联合节点快照> protect;
        {std::lock_guard lock(新需求互斥);
            d=源 ? 读取提议源需求(D,*源,protect) : 获取需求(D);reader=新增成员读取_;
            if(!d||!d->目标.新增成员来源见证)return {S::未支持,{},{},{},{}};
            const auto cw=读取唯一节点字段(D,目标新增成员见证字段);要求需求物理(cw.has_value());
            const auto list=核验需求列表物理(需求目标读取上下文{特征服务_,特征概念服务_},*d);
            const std::vector<稳定编码> ids=源 ? std::vector<稳定编码>{*cw,list}
                : std::vector<稳定编码>{D,*cw,list};
            const auto nodes=全局基础数据集.查询节点组完整结构(ids);
            if(nodes.状态==基础结构操作状态::资源失败)return {S::资源失败,{},{},{},{}};
            要求需求物理(nodes.状态==基础结构操作状态::已完成&&nodes.节点组.size()==ids.size());
            for(const auto& n:nodes.节点组){if(!n.快照)return {S::前提已变化,{},{},{},{}};protect.push_back(*n.快照);}
        }
        if(!reader)return {S::依赖未就绪,{},{},{},{}};
        // 沿已经装配的provider借用，不建立第二装配或延长其生命周期。
        const auto task=源 ? dynamic_cast<const 新_任务类*>(reader) : nullptr;
        auto r=task && &task->需求服务_==this
            ? task->判定新增成员来源内部({d->目标存在节点,d->目标},源)
            : reader->判定新增成员来源({d->目标存在节点,d->目标});
        if(r.状态!=S::已读取)return r;
        {std::lock_guard lock(新需求互斥);if(!提议需求值相同(获取需求(D),d,源))return {S::前提已变化,{},{},{},{}};}
        if(!并需求保护(r.完整保护,protect)||!需求内容保护未变(r.完整保护)||!核同提议源(源))return {S::前提已变化,{},{},{},{}};
        return r;
    }catch(const 新需求目标定义核验状态 s){要求需求物理(s==新需求目标定义核验状态::前提已变化);return {S::前提已变化,{},{},{},{}};}
    catch(const std::bad_alloc&){return {S::资源失败,{},{},{},{}};}
    catch(const std::length_error&){return {S::资源失败,{},{},{},{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取新增成员见证",L"锁外来源读取发生工程故障");}
}
新需求操作状态 新_需求类::装配引用内容目标读取接口(const 新引用内容目标读取接口* 接口) noexcept {
    std::lock_guard 锁(新需求互斥);
    if(引用内容目标读取接口_==接口)return 新需求操作状态::无变化;
    if(接口&&引用内容目标读取接口_)return 新需求操作状态::入口拒绝;
    引用内容目标读取接口_=接口;return 新需求操作状态::已修改;
}
新引用内容目标判定 新_需求类::复核引用内容目标(稳定编码 D) const noexcept {
    return 读取引用内容提议(D, true).判定;
}
新_需求类::引用内容提议读取 新_需求类::读取引用内容提议(稳定编码 D, bool 完整源) const noexcept {
    using S=新引用内容目标状态;
    引用内容提议读取 out;
    const auto 失败 = [&](S s) { 引用内容提议读取 r; r.判定.状态=s; return r; };
    try {
        新需求完整目标 目标;
        const 新引用内容目标读取接口* 端口=nullptr;
        std::vector<基础联合节点快照> 保护;
        {
            std::lock_guard 锁(新需求互斥);
            if(!结构已初始化())return {};
            const auto 当前=全局基础数据集.查询节点组完整结构({D});
            if(当前.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            要求需求物理(当前.状态==基础结构操作状态::已完成&&当前.节点组.size()==1);
            if(!当前.节点组.front().快照)return 失败(S::引用前提已变化);
            const auto& 原D=*当前.节点组.front().快照;
            if(完整源)保护.push_back(原D);
            else out.源前提=构造需求提议源前提(原D);
            const 需求目标读取上下文 ctx{特征服务_, 特征概念服务_};
            const auto projected=投影需求目标实例(ctx,原D.结构,&保护,false,完整源 ? nullptr : &out.源前提);
            const auto 信息=解码需求治理物理(ctx,projected);
            if(信息.执行域!=新需求执行域::自我执行||!信息.完整目标||!信息.完整目标->目标.引用内容定位)
                return 失败(S::定义不合法);
            if(!完整源)out.需求=投影完整需求(信息);
            目标=*信息.完整目标;端口=引用内容目标读取接口_;
            if(const auto 来源=读取唯一节点字段(D,复合分解来源字段)) {
                const auto 记录=全局基础数据集.查询节点组完整结构({*来源});
                if(记录.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
                要求需求物理(记录.状态==基础结构操作状态::已完成&&记录.节点组.size()==1&&记录.节点组.front().快照);
                保护.push_back(*记录.节点组.front().快照);
            }
        }
        if(!端口)return {};
        auto 结果=端口->求值引用内容目标(目标);
        if(结果.状态==S::资源失败||结果.状态==S::引用前提已变化)return 失败(结果.状态);
        if(!并需求保护(保护,结果.完整保护组)||!需求内容保护未变(保护)
            ||(!完整源&&!需求提议源未变(out.源前提)))return 失败(S::引用前提已变化);
        结果.完整保护组=std::move(保护);out.判定=std::move(结果);return out;
    }catch(const std::bad_alloc&){return 失败(S::资源失败);}
    catch(新需求目标定义核验状态 s){要求需求物理(s==新需求目标定义核验状态::前提已变化);return 失败(S::引用前提已变化);}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"复核引用内容目标",L"锁外内容目标求值发生工程错误");}
}
bool 新_需求类::核同引用内容提议源(const 引用内容提议读取& 批) const {
    return 批.需求 && 需求提议源未变(批.源前提);
}
void 新_需求类::合并提议源前提(基础只读预期& out,const 基础只读预期& p) {
    out.节点组.insert(out.节点组.end(),p.节点组.begin(),p.节点组.end());
    out.字段集合组.insert(out.字段集合组.end(),p.字段集合组.begin(),p.字段集合组.end());
    out.外部关系集合组.insert(out.外部关系集合组.end(),p.外部关系集合组.begin(),p.外部关系集合组.end());
}
bool 新_需求类::核同提议源(const 提议源收集批* p) const {
    return !p || p->需求节点组.empty() || 需求提议源未变(p->前提);
}
bool 新_需求类::提议需求值相同(const std::optional<新需求信息>& a,
    const std::optional<新需求信息>& b,const 提议源收集批* p) noexcept {
    if(!a||!b)return a.has_value()==b.has_value();
    if(!p||!p->需求节点组.contains(a->节点))return a==b;
    const auto 内容=[](const 新需求信息& d) {
        return std::tie(d.节点,d.根角色,d.父需求节点,d.目标存在节点,d.目标,d.来源类型,d.来源承接节点,
            d.活动状态,d.服务要求确认组,d.服务需求取得执行节点,d.需求主体存在节点,d.执行域,
            d.认知来源,d.复合来源,d.首次接收认知来源,d.行动请求来源);
    };
    return 内容(*a)==内容(*b);
}
void 新_需求类::采集提议源需求本体保护(稳定编码 D,提议源收集批& 源,
    std::vector<基础联合节点快照>& 保护) const {
    std::lock_guard 锁(新需求互斥);
    const auto r=全局基础数据集.查询节点组完整结构({D});
    if(r.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
    要求需求物理(r.状态==基础结构操作状态::已完成&&r.节点组.size()==1);
    if(!r.节点组.front().快照)throw 新需求目标定义核验状态::前提已变化;
    const auto& raw=*r.节点组.front().快照;
    需求物理游标 c{raw.结构,{}};
    if(c.引用(节点类型字段,false)==需求节点类型
        &&c.数值(需求执行域字段,false)==static_cast<std::int64_t>(新需求执行域::自我执行)){
        合并提议源前提(源.前提,构造需求提议源前提(raw));源.需求节点组.insert(D);
    }else if(!并需求保护(保护,{raw}))throw 新需求目标定义核验状态::前提已变化;
}
std::optional<新需求信息> 新_需求类::读取提议源需求(稳定编码 D,提议源收集批& 源,
    std::vector<基础联合节点快照>& 保护,std::optional<新需求治理信息>* 治理输出) const try {
    std::lock_guard 锁(新需求互斥);
    if(!结构已初始化())return {};
    const auto r=全局基础数据集.查询节点组完整结构({D});
    if(r.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
    要求需求物理(r.状态==基础结构操作状态::已完成&&r.节点组.size()==1);
    if(!r.节点组.front().快照)return {};
    const auto& raw=*r.节点组.front().快照;
    const auto p=构造需求提议源前提(raw);
    const auto d=读取需求治理信息可抛内部(D,nullptr,&p,&保护);
    if(!d)return {};
    if(治理输出)*治理输出=d;
    if(!需求提议源未变(p))throw 新需求目标定义核验状态::前提已变化;
    if(d->执行域==新需求执行域::自我执行) {
        合并提议源前提(源.前提,p);源.需求节点组.insert(D);
    } else if(!并需求保护(保护,{raw}))throw 新需求目标定义核验状态::前提已变化;
    if(!d->完整目标||(d->执行域==新需求执行域::被认知&&d->信息完善阶段!=新需求信息完善阶段::已完善))return {};
    if(const auto R=读取唯一节点字段(D,复合分解来源字段)) {
        const auto record=全局基础数据集.查询节点组完整结构({*R});
        if(record.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        要求需求物理(record.状态==基础结构操作状态::已完成&&record.节点组.size()==1&&record.节点组.front().快照);
        if(!并需求保护(保护,{*record.节点组.front().快照}))throw 新需求目标定义核验状态::前提已变化;
    }
    return 投影完整需求(*d);
} catch(const 需求物理读取失败& e) {
    if(e.原因==基础结构操作状态::资源失败)throw std::bad_alloc{};
    throw;
}
新引用内容目标判定 新_需求类::求值引用内容条件目标(const 新需求完整目标& 目标) const noexcept {
    using S=新引用内容目标状态;
    try {
        if(!目标.目标.引用内容定位||!目标形状合法(目标.目标))
            return {S::定义不合法,{},{},{},{},{}};
        const 新引用内容目标读取接口* 端口=nullptr;
        {std::lock_guard 锁(新需求互斥);if(!结构已初始化())return {};端口=引用内容目标读取接口_;}
        if(!端口)return {};
        return 端口->求值引用内容目标(目标);
    }catch(const std::bad_alloc&){return {S::资源失败,{},{},{},{},{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"求值引用内容条件目标",L"锁外条件内容求值发生工程错误");}
}
新引用内容目标判定 新_需求类::求值指定需求内容目标(const 新需求完整目标& 目标) const noexcept {
    using S=新引用内容目标状态;
    try {
        if(!目标.目标.引用内容定位||!目标形状合法(目标.目标))
            return {S::定义不合法,{},{},{},{},{}};
        const 新引用内容目标读取接口* 端口=nullptr;
        {std::lock_guard 锁(新需求互斥);if(!结构已初始化())return {};端口=引用内容目标读取接口_;}
        if(!端口)return {};
        return 端口->求值指定需求内容(目标);
    }catch(const std::bad_alloc&){return {S::资源失败,{},{},{},{},{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"求值指定需求内容目标",L"锁外指定需求内容求值发生工程错误");}
}
thread_local int 引用内容建立失败行 = 0; // 诊断：引用内容需求建立返回“前提已变化”的行号
新需求建立结果 新_需求类::建立或取得引用内容需求内部(const 新普通需求建立请求& q,bool 允许任务子目标,
    const std::vector<基础联合节点快照>& 复合采用保护,const 基础只读预期* 源前提) noexcept {
    using S=新需求操作状态;
    try {
        // CONCEPT-INFO-MEMBERSHIP §4.4：同一CAS出生路径也承接存在归属分解子需求（普通值目标）。
        const bool 归属分解=q.复合来源&&是存在归属分解(*q.复合来源);
        if(!q.目标.引用内容定位&&!归属分解)return {S::入口拒绝,{},{}};
        if(q.目标.引用内容定位&&q.目标.引用内容定位->正式内容路径.size()>1)return {S::目标内容未支持,{},{}};
        if(!目标形状合法(q.目标))return {S::目标形式不合法,{},{}};
        if(q.来源类型!=新需求来源类型::本能定义&&q.来源类型!=新需求来源类型::任务子目标)return {S::入口拒绝,{},{}};
        if(q.来源类型==新需求来源类型::任务子目标&&(!允许任务子目标||!q.来源承接节点))return {S::任务来源未接通,{},{}};
        if(q.来源类型==新需求来源类型::本能定义&&(q.来源承接节点||q.复合来源))return {S::入口拒绝,{},{}};
        if(q.复合来源&&!归属分解&&复合采用保护.empty()&&!源前提)return {S::入口拒绝,{},{}};
        const 新引用内容目标读取接口* 端口=nullptr;
        {std::lock_guard 锁(新需求互斥);if(!结构已初始化())return {S::尚未初始化,{},{}};端口=引用内容目标读取接口_;}
        if(!归属分解&&!端口)return {S::目标内容依赖未就绪,{},{}};
        auto 定义=归属分解 ? 核验需求目标定义保护(q.目标,特征概念服务_,二次特征概念服务_)
            : 端口->核验引用内容目标定义保护({q.目标存在节点,q.目标});
        switch(定义.状态) {
        case 新需求目标定义核验状态::已完成:break;
        case 新需求目标定义核验状态::依赖未就绪:return {S::目标内容依赖未就绪,{},{}};
        case 新需求目标定义核验状态::未支持:return {S::目标内容未支持,{},{}};
        case 新需求目标定义核验状态::前提已变化:{引用内容建立失败行=__LINE__;return {S::前提已变化,{},{}};}
        case 新需求目标定义核验状态::资源失败:return {S::资源失败,{},{}};
        case 新需求目标定义核验状态::输入不合法:return {S::目标形式不合法,{},{}};
        default:要求需求物理(false);
        }
        std::lock_guard 锁(新需求互斥);
        // 快照保护核对已删除（2026-11-09用户裁决，以实时读取为准）。
        (void)复合采用保护;(void)源前提;
        const auto 父=读取需求治理信息(q.父需求节点);
        if(!父||父->执行域!=新需求执行域::自我执行||!父->完整目标)return {S::父需求不合法,{},{}};
        if(归属分解) {
            const auto& 来源=*q.复合来源;
            if(!允许任务子目标||q.来源类型!=新需求来源类型::任务子目标||来源.父任务节点!=q.来源承接节点
                ||来源.父需求节点!=q.父需求节点||来源.父目标存在节点!=父->完整目标->目标存在节点
                ||来源.父目标!=父->完整目标->目标||来源.父目标.目标形式!=新需求目标形式::值
                ||!来源.父目标.目标值域概念节点||来源.子目标内容路径.empty()
                ||来源.子目标内容路径.back()!=q.目标.目标特征类型概念节点
                ||来源.子目标要求概念!=q.目标.目标值域概念节点
                ||来源.择一约束节点||来源.所选分支概念){引用内容建立失败行=__LINE__;return {S::前提已变化,{},{}};}
        } else if(q.复合来源) {
            const auto& 来源=*q.复合来源;
            if(!允许任务子目标||q.来源类型!=新需求来源类型::任务子目标||来源.父任务节点!=q.来源承接节点
                ||来源.父需求节点!=q.父需求节点||来源.父目标存在节点!=父->完整目标->目标存在节点
                ||来源.父目标!=父->完整目标->目标||!来源.父目标.引用内容定位
                ||来源.父目标.引用内容定位->引用需求节点!=q.目标.引用内容定位->引用需求节点
                ||来源.结构概念节点!=来源.父目标.目标值域概念节点
                ||来源.子目标内容路径!=q.目标.引用内容定位->正式内容路径
                ||来源.子目标要求概念!=q.目标.目标值域概念节点
                ||来源.择一约束节点.has_value()!=来源.所选分支概念.has_value()){引用内容建立失败行=__LINE__;return {S::前提已变化,{},{}};}
        }
        std::optional<新需求列表项信息> 列表;
        for(const auto 节点:读取多节点字段(需求树根节点,需求列表项字段)) {
            const auto l=读取列表项信息(节点);要求需求物理(l.has_value());
            if(l->根角色||l->目标存在节点!=q.目标存在节点||!目标相同(l->目标,q.目标))continue;
            要求需求物理(!列表);列表=*l;
            for(const auto D:l->需求节点组) {
                const auto d=读取需求治理信息(D);要求需求物理(d.has_value());
                // 缺花括号修正：原写法缩进看似在if内，实际 return 无条件执行，使同列表项任一成员（含其它父需求的）被当作复用。
                if(d->活动状态!=新需求活动状态::失活&&d->父需求节点==q.父需求节点
                    &&d->来源类型==q.来源类型&&d->来源承接节点==q.来源承接节点&&d->复合来源==q.复合来源) {
                    要求目标特征实例承载(D);
                    return {S::已复用,D,l->节点};
                }
            }
        }
        // 2026-11-09用户裁决：删除快照保护核对，以实时读取为准。目标定义只作合法性校验，不作提交预期；
        // 提交只对写入目标（父需求、被改权重的子需求、需求树根、列表项）做原子预期，防止并发写丢失或重复出生。
        基础节点组追加请求 请求;
        std::set<稳定编码> 已见{q.父需求节点};
        auto 子关系=查询子需求关系(q.父需求节点);
        std::sort(子关系.begin(),子关系.end(),[](const auto& a,const auto& b){return a.目标节点<b.目标节点;});
        std::vector<新需求治理信息> 子需求;
        std::size_t 活动数=1;
        for(const auto& 关系:子关系) {
            const auto d=读取需求治理信息(关系.目标节点);要求需求物理(d&&d->父需求节点==q.父需求节点);
            子需求.push_back(*d);if(d->活动状态!=新需求活动状态::失活)++活动数;
        }
        const auto 份额=父->权重/static_cast<std::int64_t>(活动数);
        auto 余数=父->权重%static_cast<std::int64_t>(活动数);
        std::vector<需求权重修改项> 修改;
        for(const auto& 子:子需求) {
            要求需求物理(已见.insert(子.节点).second);
            const auto 权重=子.活动状态==新需求活动状态::失活 ? 0 : 份额+(余数>0 ? (--余数,1) : 0);
            要求需求物理(追加需求权重修改(子.节点,权重,修改)&&构建子树权重修改(需求目标读取上下文{特征服务_, 特征概念服务_}, 子.节点,权重,{},已见,修改));
        }
        for(const auto& 项:修改)请求.替换字段组.push_back({项.字段关系,基础值{项.新权重}});
        已见.insert(需求树根节点);if(列表)已见.insert(列表->节点);
        const auto 捕获=全局基础数据集.查询节点组完整结构({已见.begin(),已见.end()});
        if(捕获.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        要求需求物理(捕获.状态==基础结构操作状态::已完成&&捕获.节点组.size()==已见.size());
        for(const auto& 项:捕获.节点组) {
            if(!项.快照){引用内容建立失败行=__LINE__;return {S::前提已变化,{},{}};}
            if(!并需求保护(请求.预期联合节点组,{*项.快照})){引用内容建立失败行=__LINE__;return {S::前提已变化,{},{}};}
        }
        const auto 加目标=[&](std::vector<基础组内初始字段>& 字段,稳定编码 E,const 新需求目标定义& t) {
            字段.push_back({目标存在字段,E});字段.push_back({目标特征类型字段,t.目标特征类型概念节点});
            字段.push_back({目标形式字段,基础值{static_cast<std::int64_t>(t.目标形式)}});
            if(t.目标值域概念节点)字段.push_back({目标值域字段,*t.目标值域概念节点});
            if(t.目标比较关系概念节点)字段.push_back({目标比较关系字段,*t.目标比较关系概念节点});
            if(t.目标变化方向)字段.push_back({目标变化方向字段,基础值{static_cast<std::int64_t>(*t.目标变化方向)}});
            if(t.引用内容定位) {
                字段.push_back({引用内容需求字段,t.引用内容定位->引用需求节点});
                for(const auto FT:t.引用内容定位->正式内容路径)字段.push_back({引用内容路径字段,FT});
            }
        };
        基础组内新节点 新D;
        新D.字段组={{节点类型字段,需求节点类型},{需求主体字段,父->需求主体存在节点},
            {需求执行域字段,基础值{static_cast<std::int64_t>(新需求执行域::自我执行)}},
            {来源类型字段,基础值{static_cast<std::int64_t>(q.来源类型)}},
            {活动状态字段,基础值{static_cast<std::int64_t>(新需求活动状态::活动)}},
            {需求权重字段,基础值{份额+(余数>0 ? 1 : 0)}}};
        if(q.来源承接节点)新D.字段组.push_back({来源承接字段,*q.来源承接节点});
        加目标(新D.字段组,q.目标存在节点,q.目标);请求.新节点组.push_back(std::move(新D));
        std::optional<std::size_t> 新列表;
        if(列表)请求.追加字段组.push_back({列表->节点,列表成员字段,基础组内节点引用{0}});
        else {
            新列表=请求.新节点组.size();基础组内新节点 l;
            l.字段组={{节点类型字段,需求列表项节点类型},{列表成员字段,基础组内节点引用{0}}};
            加目标(l.字段组,q.目标存在节点,q.目标);请求.新节点组.push_back(std::move(l));
            请求.追加字段组.push_back({需求树根节点,需求列表项字段,基础组内节点引用{*新列表}});
        }
        if(q.复合来源) {
            const auto& 来源=*q.复合来源;const auto 位置=请求.新节点组.size();基础组内新节点 记录;
            记录.字段组={{节点类型字段,复合分解来源类型},{复合父任务字段,来源.父任务节点},
                {复合父需求字段,来源.父需求节点},{复合结构概念字段,来源.结构概念节点},{复合子要求字段,来源.子目标要求概念}};
            加目标(记录.字段组,来源.父目标存在节点,来源.父目标);
            if(来源.择一约束节点)记录.字段组.push_back({复合择一约束字段,*来源.择一约束节点});
            if(来源.所选分支概念)记录.字段组.push_back({复合所选分支字段,*来源.所选分支概念});
            for(const auto FT:来源.子目标内容路径)记录.字段组.push_back({复合子路径字段,FT});
            请求.新节点组.push_back(std::move(记录));请求.新节点组[0].字段组.push_back({复合分解来源字段,基础组内节点引用{位置}});
        }
        请求.追加外部关系组.push_back({q.父需求节点,基础组内节点引用{0},基础外部关系类型::父子,需求父子关系角色});
        const auto 提交=全局基础数据集.原子追加节点组(请求);
        if(提交.状态==基础节点组追加状态::预期不符){引用内容建立失败行=__LINE__;return {S::前提已变化,{},{}};}
        要求需求物理(提交.状态==基础节点组追加状态::已完成&&提交.新节点组.size()==请求.新节点组.size());
        const auto D=提交.新节点组.front();const auto 读回=读取需求治理信息(D);
        要求需求物理(读回&&读回->完整目标==std::optional{新需求完整目标{q.目标存在节点,q.目标}}&&读回->复合来源==q.复合来源);
        const 关联检查离域通知 通知{关联检查通知_};
        要求目标特征实例承载(D);
        return {S::已建立,D,列表 ? std::optional{列表->节点} : std::optional{提交.新节点组[*新列表]}};
    }catch(const std::bad_alloc&){return {S::资源失败,{},{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"建立引用内容需求",L"新内容目标CAS建立发生工程错误");}
}
// 实时合并（2026-11-09用户裁决，以实时读取为准）：同一节点后读取者取代先读取者。
void 实时并保护(std::vector<基础联合节点快照>& 保护,const std::vector<基础联合节点快照>& 组){
    for(const auto& 项:组){
        const auto 旧=std::find_if(保护.begin(),保护.end(),[&](const auto& x){return x.结构.节点.编码==项.结构.节点.编码;});
        if(旧==保护.end())保护.push_back(项);else *旧=项;
    }
}
#ifdef HY_EGO_DEBUG_LOG_SIMULATION_PROGRESS
#ifndef HY_前提诊断
#define HY_前提诊断(文件,行) do{try{(void)记录调试日志(L"SIMULATION_PROGRESS",L"前提已变化返回",文件,L"行="+std::to_wstring(行));}catch(...){}}while(0)
#endif
#else
#ifndef HY_前提诊断
#define HY_前提诊断(文件,行) ((void)0)
#endif
#endif
新需求内容读取结果 新_需求类::读取被认知需求直接内容保护(
    稳定编码 D,稳定编码 E,const std::vector<稳定编码>& FT组) const noexcept {
    using S=新需求内容读取状态;
    try {
        if(!有效(D)||!有效(E))return {};
        std::set<稳定编码> 请求FT;
        for(const auto FT:FT组)if(!有效(FT)||!请求FT.insert(FT).second)return {};
        新需求治理信息 需求;
        std::vector<基础联合节点快照> 保护;
        const 新服务要求确认读取接口* 确认读取=nullptr;
        const 新_语言类* 语言读取=nullptr;
        {
            std::lock_guard 锁(新需求互斥);
            if(!结构已初始化())return {S::依赖未就绪,{},{}};
            const auto 身份=读取需求节点引用身份(D);
            if(身份.状态==特征概念规则状态::值不存在)return {S::需求不存在,{},{}};
            if(身份.状态==特征概念规则状态::读取接口未装配)return {S::依赖未就绪,{},{}};
            if(身份.状态==特征概念规则状态::资源失败)throw std::bad_alloc{};
            if(身份.状态==特征概念规则状态::前提已变化){HY_前提诊断(L"新_需求类.cpp",__LINE__);return {S::前提已变化,{},{}};}
            要求需求物理(身份.状态==特征概念规则状态::已完成&&身份.身份&&!身份.身份保护.empty());
            保护=身份.身份保护;
            const auto 原D=std::find_if(保护.begin(),保护.end(),[&](const auto& 项){return 项.结构.节点.编码==D;});
            要求需求物理(原D!=保护.end());
            需求=解码需求治理物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 原D->结构);
            if(需求.需求主体存在节点!=E)return {S::主体不符,{},{}};
            if(需求.执行域!=新需求执行域::被认知)return {S::非被认知需求,{},{}};
            要求需求物理(需求.已知目标&&需求.信息完善阶段);
            要求需求物理(std::count_if(原D->入向字段组.begin(),原D->入向字段组.end(),[&](const auto& 项){
                return 项.所属节点==需求树根节点&&项.字段节点==被认知需求登记字段;})==1);
            // 自己字段中的正式引用由本owner定位，其它owner内容不在此解析。
            std::set<稳定编码> 引用组{需求树根节点};
            for(const auto& 项:原D->结构.字段组)
                if(const auto* 引用=std::get_if<稳定编码>(&项.内容))引用组.insert(*引用);
            const auto 引用快照=全局基础数据集.查询节点组完整结构({引用组.begin(),引用组.end()});
            if(引用快照.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            要求需求物理(引用快照.状态==基础结构操作状态::已完成&&引用快照.节点组.size()==引用组.size());
            std::vector<基础联合节点快照> 引用保护;
            for(const auto& 项:引用快照.节点组) {
                if(!项.快照){HY_前提诊断(L"新_需求类.cpp",__LINE__);return {S::前提已变化,{},{}};}
                要求需求物理(项.查询节点==项.快照->结构.节点.编码);
                引用保护.push_back(*项.快照);
            }
            实时并保护(保护,引用保护);
            确认读取=服务要求确认读取接口_;
            语言读取=接收认知语言读取接口_;
        }
        // 已释放需求锁；正式槽与其它owner身份均经公开reader读取。
        using namespace 先天特征类型槽;
        const std::uint32_t 槽组[]{需求主体,需求执行域,需求目标存在,需求目标FT,需求目标形式,
            需求权重,需求活动,目标域,目标比较,目标方向,父需求,需求来源,需求根角色,理解来源,内容确认来源};
        std::map<稳定编码,std::uint32_t> FT映射;
        for(const auto 槽:槽组) {
            const auto 读取=特征概念服务_.读取先天特征类型槽(槽);
            if(读取.状态==特征概念规则状态::值不存在||读取.状态==特征概念规则状态::读取接口未装配)
                return {S::依赖未就绪,{},{}};
            if(读取.状态==特征概念规则状态::前提已变化){HY_前提诊断(L"新_需求类.cpp",__LINE__);return {S::前提已变化,{},{}};}
            if(读取.状态==特征概念规则状态::资源失败)throw std::bad_alloc{};
            要求需求物理(读取.状态==特征概念规则状态::已完成&&读取.类型根&&!读取.完整保护组.empty());
            要求需求物理(FT映射.emplace(*读取.类型根,槽).second);
            实时并保护(保护,读取.完整保护组);
        }
        for(const auto FT:FT组)if(!FT映射.contains(FT))return {S::内容项未支持,{},{}};
        if(!加需求存在保护(E,存在服务_,保护)) {
            if(!需求内容保护未变(保护)){HY_前提诊断(L"新_需求类.cpp",__LINE__);return {S::前提已变化,{},{}};}
            要求需求物理(false);
        }
        const auto 核验=核验部分需求目标(*需求.已知目标,存在服务_,特征概念服务_,二次特征概念服务_,保护);
        if(核验!=新需求操作状态::已找到) {
            if(!需求内容保护未变(保护)){HY_前提诊断(L"新_需求类.cpp",__LINE__);return {S::前提已变化,{},{}};}
            要求需求物理(false);
        }
        if(需求.认知来源) {
            if(!确认读取)return {S::依赖未就绪,{},{}};
            const auto 确认=确认读取->读取服务要求确认身份(需求.认知来源->内容确认节点);
            if(确认.状态==新服务要求确认状态::资源失败)throw std::bad_alloc{};
            if(确认.状态==新服务要求确认状态::未装配)return {S::依赖未就绪,{},{}};
            if(!需求内容保护未变(保护)){HY_前提诊断(L"新_需求类.cpp",__LINE__);return {S::前提已变化,{},{}};}
            要求需求物理(确认.状态==新服务要求确认状态::已读取&&确认.确认&&需求.完整目标);
            const auto& 来源=*确认.确认;
            要求需求物理(来源.确认节点==需求.认知来源->内容确认节点&&来源.理解节点==需求.认知来源->理解节点
                &&来源.来源人类节点==E&&来源.来源要求节点==需求.认知来源->来源要求节点
                &&来源.目标存在节点==需求.完整目标->目标存在节点&&来源.目标==需求.完整目标->目标);
        }
        if(需求.首次接收认知来源) {
            if(!语言读取)return {S::依赖未就绪,{},{}};
            const auto 来源=读取接收认知需求来源保护(*语言读取,特征服务_,*需求.首次接收认知来源,E,*需求.已知目标);
            if(来源.状态==新需求操作状态::尚未初始化)return {S::依赖未就绪,{},{}};
            if(来源.状态==新需求操作状态::资源失败)return {S::资源失败,{},{}};
            if(来源.状态==新需求操作状态::前提已变化){HY_前提诊断(L"新_需求类.cpp",__LINE__);return {S::前提已变化,{},{}};}
            if(来源.状态!=新需求操作状态::已找到) {
                if(!需求内容保护未变(保护)){HY_前提诊断(L"新_需求类.cpp",__LINE__);return {S::前提已变化,{},{}};}
                要求需求物理(false);
            }
            实时并保护(保护,来源.完整保护);
            const auto 同源=查询接收认知来源需求(*需求.首次接收认知来源,*语言读取,特征服务_);
            if(同源.状态==新需求操作状态::资源失败)return {S::资源失败,{},{}};
            if(同源.状态==新需求操作状态::前提已变化){HY_前提诊断(L"新_需求类.cpp",__LINE__);return {S::前提已变化,{},{}};}
            if(同源.状态==新需求操作状态::尚未初始化)return {S::依赖未就绪,{},{}};
            要求需求物理(同源.状态==新需求操作状态::已找到&&同源.信息&&同源.信息->节点==D);
            实时并保护(保护,同源.完整保护);
        }
        新被认知需求直接内容 输出{D,E,*需求.信息完善阶段,{}};
        for(const auto FT:FT组)输出.内容项组.push_back(投影需求内容(FT,FT映射.at(FT),需求));
        return {S::已读取,std::move(输出),std::move(保护)};
    }catch(新需求目标定义核验状态 状态) {
        if(状态==新需求目标定义核验状态::前提已变化){HY_前提诊断(L"新_需求类.cpp",__LINE__);return {S::前提已变化,{},{}};}
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"需求直接内容读取",L"目标核验工程状态异常");
    }catch(const 需求物理读取失败& 错误) {
        if(错误.原因==基础结构操作状态::节点不存在){HY_前提诊断(L"新_需求类.cpp",__LINE__);return {S::前提已变化,{},{}};}
        if(错误.原因==基础结构操作状态::资源失败)return {S::资源失败,{},{}};
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"需求直接内容读取",L"正式需求物理结构损坏");
    }catch(const std::bad_alloc&) {return {S::资源失败,{},{}};}
    catch(const std::exception& 错误) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"需求直接内容读取",L"正式内容读取工程错误",错误.what());
    }catch(...) {工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"需求直接内容读取",L"未知工程错误");}
}
void 新_需求类::装配接收认知语言读取接口(const 新_语言类* 接口) noexcept {
    if(接口&&接收认知语言读取接口_&&接口!=接收认知语言读取接口_)
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"装配接收认知语言读取",L"不得替换已绑定语言事实对象");
    接收认知语言读取接口_=接口;
}
新被认知需求资格读取结果 新_需求类::读取被认知需求完整资格保护(
    稳定编码 D,稳定编码 H) const noexcept {
    using S=新被认知需求资格状态;
    try {
        const auto 信息=读取需求治理信息(D);
        if(!信息)return {S::需求不存在,{},{}};
        if(信息->需求主体存在节点!=H)return {S::主体不符,{},{}};
        if(信息->执行域!=新需求执行域::被认知)return {S::非被认知需求,{},{}};
        要求需求物理(!(信息->认知来源&&信息->首次接收认知来源));
        if(!信息->完整目标||(!信息->认知来源&&!信息->首次接收认知来源))
            return {S::内容待完善,{},{}};
        std::vector<基础联合节点快照> 保护;
        auto 种类=新被认知需求来源种类::首次接收认知;
        std::optional<新服务要求确认语义保护> 旧来源保护;
        if(信息->认知来源) {
            种类=新被认知需求来源种类::旧理解内容确认;
            auto 来源=准备服务要求确认语义保护(信息->认知来源->内容确认节点);
            switch(来源.状态) {
            case 新服务要求确认保护状态::需重新确认:return {S::需重新确认,{},{}};
            case 新服务要求确认保护状态::确认不存在:return {S::来源不符,{},{}};
            case 新服务要求确认保护状态::未装配:return {S::依赖未就绪,{},{}};
            case 新服务要求确认保护状态::前提已变化:return {S::前提已变化,{},{}};
            case 新服务要求确认保护状态::资源失败:return {S::资源失败,{},{}};
            default:要求需求物理(来源.状态==新服务要求确认保护状态::已准备&&来源.保护);
            }
            const auto& c=来源.保护->确认();
            if(c.确认节点!=信息->认知来源->内容确认节点||c.理解节点!=信息->认知来源->理解节点
                ||c.来源人类节点!=H||c.来源要求节点!=信息->认知来源->来源要求节点
                ||c.目标存在节点!=信息->完整目标->目标存在节点||c.目标!=信息->完整目标->目标)
                return {S::来源不符,{},{}};
            保护=来源.保护->预期节点组();旧来源保护=std::move(来源.保护);
        } else {
            const auto* 语言=接收认知语言读取接口_;
            if(!语言)return {S::依赖未就绪,{},{}};
            要求需求物理(信息->已知目标.has_value());
            auto 来源=读取接收认知需求来源保护(*语言,特征服务_,*信息->首次接收认知来源,H,*信息->已知目标);
            if(来源.状态==新需求操作状态::前提已变化)return {S::前提已变化,{},{}};
            if(来源.状态==新需求操作状态::资源失败)return {S::资源失败,{},{}};
            if(来源.状态==新需求操作状态::尚未初始化)return {S::依赖未就绪,{},{}};
            if(来源.状态!=新需求操作状态::已找到)return {S::来源不符,{},{}};
            保护=std::move(来源.完整保护);
            const auto 同源=查询接收认知来源需求(*信息->首次接收认知来源,*语言,特征服务_);
            if(同源.状态==新需求操作状态::资源失败)return {S::资源失败,{},{}};
            if(同源.状态==新需求操作状态::前提已变化)return {S::前提已变化,{},{}};
            if(同源.状态==新需求操作状态::尚未初始化)return {S::依赖未就绪,{},{}};
            if(同源.状态!=新需求操作状态::已找到||!同源.信息||同源.信息->节点!=D)
                return {S::来源不符,{},{}};
            if(!并需求保护(保护,同源.完整保护))return {S::前提已变化,{},{}};
        }
        const auto 概念=存在概念服务_.读取引用内容筹办概念();
        if(概念.状态==正式结构操作状态::前提已变化)return {S::前提已变化,{},{}};
        if(概念.状态==正式结构操作状态::依赖未就绪||概念.状态==正式结构操作状态::未找到)
            return {S::依赖未就绪,{},{}};
        要求需求物理(概念.状态==正式结构操作状态::已读取&&概念.信息);
        if(!并需求保护(保护,概念.完整保护组))return {S::前提已变化,{},{}};
        const auto 内容=判定被认知需求内容保护(D,H,概念.信息->完整被认知需求);
        switch(内容.状态) {
        case 新引用内容目标状态::未满足:case 新引用内容目标状态::分支待定:return {S::内容待完善,{},{}};
        case 新引用内容目标状态::引用前提已变化:return {S::前提已变化,{},{}};
        case 新引用内容目标状态::资源失败:return {S::资源失败,{},{}};
        case 新引用内容目标状态::依赖未就绪:case 新引用内容目标状态::未支持:
        case 新引用内容目标状态::定义不合法:return {S::依赖未就绪,{},{}};
        default:要求需求物理(内容.状态==新引用内容目标状态::已满足);
        }
        if(!并需求保护(保护,内容.完整保护组)||读取需求治理信息(D)!=信息
            ||!需求内容保护未变(保护))return {S::前提已变化,{},{}};
        return {S::已读取,新被认知需求完整资格{*信息,种类},std::move(保护)};
    }catch(新需求目标定义核验状态 s) {
        要求需求物理(s==新需求目标定义核验状态::前提已变化);return {S::前提已变化,{},{}};
    }catch(const std::bad_alloc&){return {S::资源失败,{},{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取被认知需求资格",L"完整资格读取工程错误");}
}
新被认知需求信息更新结果 新_需求类::完善接收认知需求信息(
    稳定编码 D,const 新需求治理信息& 预期) noexcept {
    using S=新需求操作状态;
    try {
        const auto 资格=读取被认知需求完整资格保护(D,预期.需求主体存在节点);
        if(资格.状态==新被认知需求资格状态::资源失败)return {S::资源失败,{}};
        if(资格.状态==新被认知需求资格状态::前提已变化)return {S::前提已变化,{}};
        if(资格.状态==新被认知需求资格状态::依赖未就绪)return {S::尚未初始化,{}};
        if(资格.状态==新被认知需求资格状态::需求不存在)return {S::需求不存在,{}};
        if(资格.状态!=新被认知需求资格状态::已读取)return {S::认知来源不合法,{}};
        要求需求物理(资格.信息.has_value());
        if(资格.信息->来源种类!=新被认知需求来源种类::首次接收认知)return {S::认知来源不合法,{}};
        std::unique_lock 锁(新需求互斥);
        const auto 当前=读取需求治理信息(D);
        if(!当前||D!=预期.节点||*当前!=预期||资格.信息->需求!=预期)return {S::前提已变化,{}};
        if(当前->信息完善阶段==新需求信息完善阶段::已完善) {
            if(!需求内容保护未变(资格.完整保护))return {S::前提已变化,{}};
            return {S::无变化,*当前};
        }
        要求需求物理(当前->信息完善阶段==新需求信息完善阶段::待完善);
        基础节点组追加请求 请求;请求.预期联合节点组=资格.完整保护;
        const auto 原D=std::find_if(请求.预期联合节点组.begin(),请求.预期联合节点组.end(),
            [&](const auto& n){return n.结构.节点.编码==D;});
        要求需求物理(原D!=请求.预期联合节点组.end());
        const auto 阶段=std::find_if(原D->结构.字段组.begin(),原D->结构.字段组.end(),
            [](const auto& f){return f.字段节点==需求信息完善阶段字段;});
        要求需求物理(阶段!=原D->结构.字段组.end());
        请求.替换字段组.push_back({阶段->编码,基础值{static_cast<std::int64_t>(新需求信息完善阶段::已完善)}});
        const auto 提交=全局基础数据集.原子追加节点组(请求);
        if(提交.状态==基础节点组追加状态::预期不符)return {S::前提已变化,{}};
        if(提交.状态==基础节点组追加状态::资源失败)return {S::资源失败,{}};
        要求需求物理(提交.状态==基础节点组追加状态::已完成);
        auto 预期后=预期;预期后.信息完善阶段=新需求信息完善阶段::已完善;
        锁.unlock();
        const auto 读回=读取被认知需求完整资格保护(D,预期.需求主体存在节点);
        if(读回.状态==新被认知需求资格状态::资源失败)return {S::资源失败,{}};
        if(读回.状态==新被认知需求资格状态::前提已变化)return {S::前提已变化,{}};
        if(读回.状态!=新被认知需求资格状态::已读取||!读回.信息||读回.信息->需求!=预期后) {
            if(读取需求治理信息(D)!=std::optional{预期后})return {S::前提已变化,{}};
            要求需求物理(false);
        }
        return {S::已修改,读回.信息->需求};
    }catch(const std::bad_alloc&){return {S::资源失败,{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"完善接收认知需求",L"同需求阶段发布工程错误");}
}
新需求内容判定结果 新_需求类::判定被认知需求内容保护(
    稳定编码 D,稳定编码 E,稳定编码 C,const std::vector<稳定编码>& 正式内容路径) const noexcept {
    using S=新引用内容目标状态;using F=正式内容符合状态;
    try {
        if(!有效(D)||!有效(E)||!有效(C))return {S::定义不合法,{},{},{}};
        if(正式内容路径.size()>1)return {S::未支持,{},{},{}};
        const auto 定义=存在概念服务_.读取正式结构概念保护(C);
        if(定义.状态==正式结构操作状态::依赖未就绪)return {};
        if(定义.状态==正式结构操作状态::前提已变化)return {S::引用前提已变化,{},{},{}};
        if(定义.状态==正式结构操作状态::未找到||定义.状态==正式结构操作状态::输入拒绝)
            return {S::定义不合法,{},{},{}};
        要求需求物理(定义.状态==正式结构操作状态::已读取&&定义.信息&&!定义.完整保护组.empty());
        if((!std::holds_alternative<基础范围概念定义>(定义.信息->定义)
                &&!std::holds_alternative<信息结构概念定义>(定义.信息->定义))
            ||(正式内容路径.empty()&&!std::holds_alternative<信息结构概念定义>(定义.信息->定义)))
            return {S::定义不合法,{},{},{}};
        auto 保护=定义.完整保护组;
        std::vector<稳定编码> FT组;
        if(!正式内容路径.empty())FT组=正式内容路径;
        else for(std::uint32_t 槽=先天特征类型槽::需求主体;槽<=先天特征类型槽::内容确认来源;++槽) {
            const auto r=特征概念服务_.读取先天特征类型槽(槽);
            if(r.状态==特征概念规则状态::前提已变化)return {S::引用前提已变化,{},{},{}};
            if(r.状态==特征概念规则状态::资源失败)return {S::资源失败,{},{},{}};
            if(!r.类型根)return {};
            if(!并需求保护(保护,r.完整保护组))return {S::引用前提已变化,{},{},{}};
            FT组.push_back(*r.类型根);
        }
        const auto 内容=读取被认知需求直接内容保护(D,E,FT组);
        if(内容.状态==新需求内容读取状态::资源失败)return {S::资源失败,{},{},{}};
        if(内容.状态==新需求内容读取状态::依赖未就绪)return {};
        if(内容.状态==新需求内容读取状态::内容项未支持)return {S::未支持,{},{},{}};
        if(内容.状态==新需求内容读取状态::输入不合法)return {S::定义不合法,{},{},{}};
        if(内容.状态!=新需求内容读取状态::已读取||!内容.信息)return {S::引用前提已变化,{},{},{}};
        if(!并需求保护(保护,内容.完整保护组))return {S::引用前提已变化,{},{},{}};
        std::map<稳定编码,正式内容值> 值组;
        for(const auto& 项:内容.信息->内容项组)
            if(!值组.emplace(项.特征类型,项.内容).second)
                throw std::runtime_error("成功读取的正式内容包含重复特征类型");
        bool 身份依赖未就绪=false;
        const 正式引用身份核验器 身份=[&](基础概念原语 p,const auto& 数值,const auto&,const 正式内容值& v)->正式内容符合结果 {
            if(v.种类==正式内容值种类::I64数值)return {F::不符合,{}};
            const auto 节点=std::get<稳定编码>(*v.值);
            if(p==基础概念原语::合法存在) {
                if(v.种类!=正式内容值种类::存在引用)return {F::不符合,{}};
                const auto r=存在服务_.读取存在身份保护(节点);
                if(r.前提已变化)return {F::前提已变化,{}};
                if(r.状态==基础结构操作状态::资源失败)return {F::资源失败,{}};
                if(r.状态!=基础结构操作状态::已完成&&r.状态!=基础结构操作状态::节点不存在&&r.状态!=基础结构操作状态::入口拒绝)
                    throw std::runtime_error("内容存在范围身份reader工程失败");
                if(r.状态==基础结构操作状态::已完成&&!r.信息)throw std::runtime_error("内容存在范围成功缺身份");
                return {r.信息 ? F::符合 : F::不符合,r.预期节点组};
            }
            if(p==基础概念原语::具名领域身份&&数值.size()==1) {
                if(数值.front()==2||数值.front()==3) {
                    const auto 种类=数值.front()==2 ? 正式内容值种类::语言理解引用 : 正式内容值种类::内容确认引用;
                    const bool 同来源=v.种类==种类&&std::any_of(值组.begin(),值组.end(),[&](const auto& x){return x.second.种类==种类&&x.second.值==v.值;});
                    return {同来源 ? F::符合 : F::不符合,内容.完整保护组};
                }
                if(数值.front()==15&&v.种类==正式内容值种类::需求引用) {
                    const auto r=读取需求节点引用身份(节点);
                    if(r.状态==特征概念规则状态::资源失败)return {F::资源失败,{}};
                    if(r.状态==特征概念规则状态::前提已变化)return {F::前提已变化,{}};
                    if(r.状态==特征概念规则状态::读取接口未装配){身份依赖未就绪=true;return {F::未支持,{}};}
                    if(r.状态!=特征概念规则状态::已完成&&r.状态!=特征概念规则状态::值不存在)
                        throw std::runtime_error("具名需求范围身份reader工程失败");
                    return {r.身份 ? F::符合 : F::不符合,r.身份保护};
                }
                return {F::未支持,{}};
            }
            if(p==基础概念原语::正式信息身份)return {F::符合,内容.完整保护组};
            if(v.种类!=正式内容值种类::概念引用)return {F::不符合,{}};
            if(p==基础概念原语::合法FT根||p==基础概念原语::合法特征概念) {
                const auto r=特征概念服务_.读取特征概念定义保护(节点);
                if(r.前提已变化)return {F::前提已变化,{}};
                if(r.状态==基础结构操作状态::资源失败)return {F::资源失败,{}};
                if(r.状态!=基础结构操作状态::已完成&&r.状态!=基础结构操作状态::节点不存在&&r.状态!=基础结构操作状态::入口拒绝)
                    throw std::runtime_error("内容特征范围定义reader工程失败");
                if(r.状态==基础结构操作状态::已完成&&!r.信息)throw std::runtime_error("内容特征范围成功缺定义");
                return {r.信息&&(p!=基础概念原语::合法FT根||r.信息->类型根==节点) ? F::符合 : F::不符合,r.预期节点组};
            }
            if(p==基础概念原语::合法比较关系) {
                const auto r=二次特征概念服务_.读取二次特征定义保护(节点);
                if(r.前提已变化)return {F::前提已变化,{}};
                if(r.状态==基础结构操作状态::资源失败)return {F::资源失败,{}};
                if(r.状态!=基础结构操作状态::已完成&&r.状态!=基础结构操作状态::节点不存在&&r.状态!=基础结构操作状态::入口拒绝)
                    throw std::runtime_error("内容比较范围定义reader工程失败");
                if(r.状态==基础结构操作状态::已完成&&!r.信息)throw std::runtime_error("内容比较范围成功缺定义");
                return {r.信息 ? F::符合 : F::不符合,r.预期节点组};
            }
            if(p==基础概念原语::正式概念身份) {
                const 正式概念身份读取接口* 提供者[]{&特征概念服务_,&二次特征概念服务_,&存在概念服务_};
                for(const auto* owner:提供者) {
                    const auto r=owner->读取正式概念身份保护(节点);
                    if(r.状态==正式概念读取状态::前提已变化)return {F::前提已变化,{}};
                    if(r.状态==正式概念读取状态::依赖未就绪){身份依赖未就绪=true;return {F::未支持,{}};}
                    if(r.身份)return {F::符合,r.身份->完整保护组};
                }
                return {F::不符合,{}};
            }
            return {F::未支持,{}};
        };
        const 正式直接内容读取器 读=[&](稳定编码 实例,稳定编码 FT) {
            if(实例!=D)throw std::runtime_error("结构判定器切换了需求实例");
            const auto 项=值组.find(FT);
            if(项==值组.end())throw F::未支持;
            return 项->second;
        };
        const 正式关联约束核验器 关联=[&](稳定编码 实例,const 结构约束定义& r)->正式内容符合结果 {
            if(实例!=D)throw std::runtime_error("关联判定器切换了需求实例");
            if(r.原语!=结构约束原语::域属于FT&&r.原语!=结构约束原语::比较适用于目标)return {F::未支持,{}};
            // 只能复用同D读取器已经核验的正式801/802语义，不能把任意FT组合判通过。
            const auto 槽FT=[&](std::uint32_t 槽)->稳定编码 {
                const auto v=特征概念服务_.读取先天特征类型槽(槽);
                if(v.状态==特征概念规则状态::前提已变化)throw F::前提已变化;
                if(v.状态==特征概念规则状态::资源失败)throw F::资源失败;
                if(!v.类型根)throw F::未支持;
                if(!并需求保护(保护,v.完整保护组))throw F::前提已变化;
                return *v.类型根;
            };
            const std::vector<稳定编码> 预期 = r.原语==结构约束原语::域属于FT
 ? std::vector<稳定编码>{槽FT(先天特征类型槽::目标域),槽FT(先天特征类型槽::需求目标FT)}
                : std::vector<稳定编码>{槽FT(先天特征类型槽::目标比较),槽FT(先天特征类型槽::需求目标FT),槽FT(先天特征类型槽::目标域)};
            if(r.项目FT组!=预期||!r.整数参数.empty()||!r.引用参数.empty())return {F::未支持,{}};
            for(const auto FT:r.项目FT组) {
                const auto 值=读(D,FT);if(值.状态==正式内容值状态::当前未知)return {F::当前未知,{}};
                if(值.状态==正式内容值状态::不适用)return {F::不符合,{}};
            }
            // 同D的完整typed读取已核FC/FT/RC及U/C关联，不能跨D拼齐。
            return {F::符合,内容.完整保护组};
        };
        新需求内容判定结果 输出;
        F 判定=F::未支持;
        if(正式内容路径.empty()) {
            auto r=存在概念服务_.判定正式结构内容(C,D,读,身份,关联);
            判定=r.状态;输出.必要项目组=std::move(r.项目组);输出.分支组=std::move(r.分支组);
            if(!并需求保护(保护,r.完整保护组))return {S::引用前提已变化,{},{},{}};
        } else {
            auto r=存在概念服务_.比较正式内容范围(C,读(D,正式内容路径.front()),身份);
            判定=r.状态;输出.必要项目组.push_back({正式内容路径.front(),C,判定});
            if(!并需求保护(保护,r.完整保护组))return {S::引用前提已变化,{},{},{}};
        }
        if(判定==F::资源失败)return {S::资源失败,{},{},{}};
        if(判定==F::前提已变化||!需求内容保护未变(保护))return {S::引用前提已变化,{},{},{}};
        输出.状态=判定==F::符合 ? S::已满足 : 判定==F::未支持 ? S::未支持 : S::未满足;
        if(身份依赖未就绪)输出.状态=S::依赖未就绪;
        if(std::any_of(输出.分支组.begin(),输出.分支组.end(),[](const auto& x){return x.状态==F::当前未知;}))输出.状态=S::分支待定;
        输出.完整保护组=std::move(保护);return 输出;
    }catch(F 状态) {
        if(状态==F::前提已变化)return {S::引用前提已变化,{},{},{}};
        if(状态==F::资源失败)return {S::资源失败,{},{},{}};
        要求需求物理(状态==F::未支持);return {S::未支持,{},{},{}};
    }catch(const std::bad_alloc&){return {S::资源失败,{},{},{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"需求纯内容判定",L"正式内容判定工程错误");}
}

正式信息参数读取结果 新_需求类::读取正式信息参数保护(
    稳定编码 D,稳定编码 C) const noexcept {
    using S=正式信息参数读取状态;
    try {
        // 范围身份只由概念owner出具；不把其它正式结构一概解释成需求。
        const auto 先天=存在概念服务_.读取先天需求任务方法概念();
        if(先天.状态==正式结构操作状态::依赖未就绪||先天.状态==正式结构操作状态::未找到)
            return {S::依赖未就绪,{},{}};
        if(先天.状态==正式结构操作状态::前提已变化)return {S::前提已变化,{},{}};
        要求需求物理(先天.状态==正式结构操作状态::已读取&&先天.信息);
        if(C!=先天.信息->需求)return {S::范围未支持,{},{}};
        const auto 范围=存在概念服务_.读取正式概念身份保护(C);
        if(范围.状态==正式概念读取状态::依赖未就绪)return {S::依赖未就绪,{},{}};
        if(范围.状态==正式概念读取状态::前提已变化)return {S::前提已变化,{},{}};
        要求需求物理(范围.状态==正式概念读取状态::已读取&&范围.身份
            &&范围.身份->概念节点==C&&!范围.身份->完整保护组.empty());
        if(!有效(D))return {S::信息不存在,{},{}};
        std::vector<基础联合节点快照> 保护=范围.身份->完整保护组;
        新需求治理信息 需求;
        {
            std::lock_guard 锁(新需求互斥);
            if(!结构已初始化())return {S::依赖未就绪,{},{}};
            const auto 当前=全局基础数据集.查询节点组完整结构({D});
            if(当前.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            if(当前.状态==基础结构操作状态::节点不存在)return {S::信息不存在,{},{}};
            要求需求物理(当前.状态==基础结构操作状态::已完成&&当前.节点组.size()==1
                &&当前.节点组.front().查询节点==D);
            if(!当前.节点组.front().快照)return {S::信息不存在,{},{}};
            const auto& 原D=*当前.节点组.front().快照;
            // 仅识别本owner的类型字段。普通存在等异类节点没有需求成功载荷。
            const auto 类型=std::find_if(原D.结构.字段组.begin(),原D.结构.字段组.end(),[](const auto& 项){
                const auto* 值=std::get_if<稳定编码>(&项.内容);
                return 项.字段节点==节点类型字段&&值&&*值==需求节点类型;
            });
            if(类型==原D.结构.字段组.end())return {S::信息不存在,{},{}};
            const auto 身份=读取需求节点引用身份(D);
            if(身份.状态==特征概念规则状态::资源失败)throw std::bad_alloc{};
            if(身份.状态==特征概念规则状态::读取接口未装配)return {S::依赖未就绪,{},{}};
            if(身份.状态==特征概念规则状态::前提已变化||身份.状态==特征概念规则状态::值不存在)
                return {S::前提已变化,{},{}};
            要求需求物理(身份.状态==特征概念规则状态::已完成&&身份.身份
                &&身份.身份->需求节点==D&&!身份.身份保护.empty());
            if(!并需求保护(保护,{原D})||!并需求保护(保护,身份.身份保护))return {S::前提已变化,{},{}};
            需求=解码需求治理物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 原D.结构);
            if(需求.完整目标&&需求.完整目标->目标.新增成员来源见证)return {S::范围未支持,{},{}};
            if(需求.执行域==新需求执行域::自我执行) {
                // 只保护本需求明确引用的节点；不解析其它owner私有字段或伪造确认来源。
                std::set<稳定编码> 引用组{需求树根节点};
                for(const auto& 项:原D.结构.字段组)
                    if(const auto* 引用=std::get_if<稳定编码>(&项.内容))引用组.insert(*引用);
                if(需求.父需求节点)引用组.insert(*需求.父需求节点);
                const auto 引用=全局基础数据集.查询节点组完整结构({引用组.begin(),引用组.end()});
                if(引用.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
                要求需求物理(引用.状态==基础结构操作状态::已完成&&引用.节点组.size()==引用组.size());
                std::vector<基础联合节点快照> 引用保护;
                for(const auto& 项:引用.节点组) {
                    if(!项.快照)return {S::前提已变化,{},{}};
                    要求需求物理(项.查询节点==项.快照->结构.节点.编码);
                    引用保护.push_back(*项.快照);
                }
                if(!并需求保护(保护,引用保护))return {S::前提已变化,{},{}};
            }
        }
        // 需求锁外取得FT及其它owner的正式材料，最后统一fresh核验联合保护。
        using namespace 先天特征类型槽;
        const std::uint32_t 槽组[]{需求主体,需求执行域,需求目标存在,需求目标FT,需求目标形式,
            需求权重,需求活动,目标域,目标比较,目标方向,父需求,需求来源,需求根角色,理解来源,内容确认来源};
        std::vector<稳定编码> FT组;
        std::set<稳定编码> 已读FT;
        for(const auto 槽:槽组) {
            const auto FT=特征概念服务_.读取先天特征类型槽(槽);
            if(FT.状态==特征概念规则状态::值不存在||FT.状态==特征概念规则状态::读取接口未装配)
                return {S::依赖未就绪,{},{}};
            if(FT.状态==特征概念规则状态::前提已变化)return {S::前提已变化,{},{}};
            if(FT.状态==特征概念规则状态::资源失败)throw std::bad_alloc{};
            要求需求物理(FT.状态==特征概念规则状态::已完成&&FT.类型根&&!FT.完整保护组.empty());
            要求需求物理(已读FT.insert(*FT.类型根).second);
            FT组.push_back(*FT.类型根);
            if(!并需求保护(保护,FT.完整保护组))return {S::前提已变化,{},{}};
        }
        正式信息参数投影 输出{D,C,{}};
        if(需求.执行域==新需求执行域::被认知) {
            const auto 内容=读取被认知需求直接内容保护(D,需求.需求主体存在节点,FT组);
            if(内容.状态==新需求内容读取状态::依赖未就绪)return {S::依赖未就绪,{},{}};
            if(内容.状态==新需求内容读取状态::资源失败)return {S::资源失败,{},{}};
            if(内容.状态==新需求内容读取状态::前提已变化||内容.状态==新需求内容读取状态::需求不存在
                ||内容.状态==新需求内容读取状态::主体不符||内容.状态==新需求内容读取状态::非被认知需求)
                return {S::前提已变化,{},{}};
            要求需求物理(内容.状态==新需求内容读取状态::已读取&&内容.信息
                &&内容.信息->需求节点==D&&内容.信息->内容项组.size()==FT组.size()&&!内容.完整保护组.empty());
            if(!并需求保护(保护,内容.完整保护组))return {S::前提已变化,{},{}};
            输出.直接内容组=内容.信息->内容项组;
        } else {
            要求需求物理(需求.执行域==新需求执行域::自我执行&&!需求.认知来源);
            if(!加需求存在保护(需求.需求主体存在节点,存在服务_,保护)) {
                if(!需求内容保护未变(保护))return {S::前提已变化,{},{}};
                要求需求物理(false);
            }
            for(std::size_t i=0;i<FT组.size();++i)
                输出.直接内容组.push_back(投影需求内容(FT组[i],槽组[i],需求));
        }
        const auto 语义状态=采集正式需求引用语义(需求,存在服务_,特征概念服务_,二次特征概念服务_,输出,保护);
        if(语义状态==新需求操作状态::前提已变化)return {S::前提已变化,{},{}};
        if(语义状态==新需求操作状态::资源失败)return {S::资源失败,{},{}};
        if(语义状态==新需求操作状态::尚未初始化)return {S::依赖未就绪,{},{}};
        要求需求物理(语义状态==新需求操作状态::已找到);
        if(!需求内容保护未变(保护))return {S::前提已变化,{},{}};
        return {S::已读取,std::move(输出),std::move(保护)};
    }catch(新需求目标定义核验状态 状态) {
        if(状态==新需求目标定义核验状态::前提已变化)return {S::前提已变化,{},{}};
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"正式信息参数读取",L"需求保护工程状态异常");
    }catch(const 需求物理读取失败& 错误) {
        if(错误.原因==基础结构操作状态::节点不存在)return {S::前提已变化,{},{}};
        if(错误.原因==基础结构操作状态::资源失败)return {S::资源失败,{},{}};
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"正式信息参数读取",L"需求物理结构损坏");
    }catch(const std::bad_alloc&) {return {S::资源失败,{},{}};}
    catch(const std::exception& 错误) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"正式信息参数读取",L"需求参数读取工程错误",错误.what());
    }catch(...) {工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"正式信息参数读取",L"未知工程错误");}
}

新接收认知需求读取结果 新_需求类::查询接收认知来源需求(const 新接收认知来源键& K) const noexcept {
    using S=新需求操作状态;
    try {
        std::lock_guard lock(新需求互斥);
        if(!结构仍存在())return {S::尚未初始化,{},{}};
        if(!有效(K.接收消息节点)||K.原文起点!=0||K.原文终点==0||!K.根出现路径.empty())return {S::认知来源不合法,{},{}};
        return 查询接收认知来源需求已加锁(K,{});
    }catch(新需求目标定义核验状态 s){要求需求物理(s==新需求目标定义核验状态::前提已变化);return {S::前提已变化,{},{}};}
    catch(const 需求物理读取失败& e){
        if(e.原因==基础结构操作状态::资源失败)return {S::资源失败,{},{}};
        if(e.原因==基础结构操作状态::节点不存在)return {S::前提已变化,{},{}};
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取接收认知需求",L"首次接收来源需求结构损坏");}
    catch(const std::bad_alloc&){return {S::资源失败,{},{}};}
    catch(const std::length_error&){return {S::资源失败,{},{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取接收认知需求",L"首次接收来源需求读取发生工程故障");}
}

新接收认知需求读取结果 新_需求类::查询接收认知来源需求已加锁(
    const 新接收认知来源键& K,const std::vector<基础联合节点快照>& 来源保护) const {
        using S=新需求操作状态;
        std::vector<基础联合节点快照> protect=来源保护;
        const auto capture=[&](稳定编码 id) {
            const auto r=全局基础数据集.查询节点组完整结构({id});
            if(r.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            要求需求物理(r.状态==基础结构操作状态::已完成&&r.节点组.size()==1);
            if(!r.节点组.front().快照)throw 新需求目标定义核验状态::前提已变化;
            const auto n=*r.节点组.front().快照;
            if(!并需求保护(protect,{n}))throw 新需求目标定义核验状态::前提已变化;
            return n;
        };
        (void)capture(需求树根节点);
        std::optional<新需求治理信息> result;
        std::vector<新接收认知来源键> seen;
        for(const auto id:读取多节点字段(需求树根节点,被认知需求登记字段)) {
            const auto n=capture(id);
            (void)投影需求目标实例(需求目标读取上下文{特征服务_,特征概念服务_},n.结构,&protect);
            const auto d=解码需求治理物理(需求目标读取上下文{特征服务_,特征概念服务_},n.结构);
            要求需求物理(d.执行域==新需求执行域::被认知);
            if(!d.首次接收认知来源)continue;
            const auto& key=d.首次接收认知来源->键;
            要求需求物理(std::find(seen.begin(),seen.end(),key)==seen.end());seen.push_back(key);
            if(key==K)result=d;
        }
        if(!需求内容保护未变(protect))return {S::前提已变化,{},{}};
        return {result ? S::已找到 : S::需求不存在,std::move(result),std::move(protect)};
}

新接收认知需求读取结果 新_需求类::查询接收认知来源需求(const 新接收认知来源& 来源,
    const 新_语言类& 语言,const 新_特征类& 特征) const noexcept {
    using S=新需求操作状态;
    try {
        if(!有效(来源.采用含义节点))return {S::认知来源不合法,{},{}};
        const auto U=语言.读取采用含义(来源.采用含义节点,特征);
        if(U.状态==新语言含义采用状态::资源失败)return {S::资源失败,{},{}};
        if(U.状态==新语言含义采用状态::前提已变化)return {S::前提已变化,{},{}};
        if(U.状态!=新语言含义采用状态::已采用&&U.状态!=新语言含义采用状态::精确重复)
            return {S::认知来源不合法,{},{}};
        要求需求物理(U.信息&&!U.完整保护.empty());
        if(U.信息->节点!=来源.采用含义节点||U.信息->来源.接收消息节点!=来源.键.接收消息节点)
            return {S::同来源目标冲突,{},{}};
        const auto P=语言.读取采用指定需求目标(来源.采用含义节点,来源.键,特征);
        if(P.状态==新语言含义采用状态::资源失败)return {S::资源失败,{},{}};
        if(P.状态==新语言含义采用状态::前提已变化)return {S::前提已变化,{},{}};
        if(P.状态!=新语言含义采用状态::已采用&&P.状态!=新语言含义采用状态::精确重复)
            return {S::认知来源不合法,{},{}};
        要求需求物理(P.信息&&!P.完整保护.empty());
        if(P.信息->需求来源!=来源.键||P.信息->采用含义节点!=来源.采用含义节点
            ||P.信息->需求主体!=U.信息->说话者节点)return {S::同来源目标冲突,{},{}};
        auto 保护=U.完整保护;
        if(!并需求保护(保护,P.完整保护))return {S::前提已变化,{},{}};
        std::lock_guard 锁(新需求互斥);
        if(!结构仍存在())return {S::尚未初始化,{},{}};
        auto result=查询接收认知来源需求已加锁(来源.键,保护);
        if(result.状态==S::已找到) {
            要求需求物理(result.信息.has_value());
            if(result.信息->首次接收认知来源!=std::optional{来源}
                ||result.信息->需求主体存在节点!=P.信息->需求主体)return {S::同来源目标冲突,{},{}};
        }
        return result;
    }catch(新需求目标定义核验状态 s){要求需求物理(s==新需求目标定义核验状态::前提已变化);return {S::前提已变化,{},{}};}
    catch(const 需求物理读取失败& e){
        if(e.原因==基础结构操作状态::资源失败)return {S::资源失败,{},{}};
        if(e.原因==基础结构操作状态::节点不存在)return {S::前提已变化,{},{}};
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取接收认知需求",L"首次接收来源需求结构损坏");}
    catch(const std::bad_alloc&){return {S::资源失败,{},{}};}
    catch(const std::length_error&){return {S::资源失败,{},{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取接收认知需求",L"首次接收来源需求读取发生工程故障");}
}

新_需求类::接收认知来源保护结果 新_需求类::读取接收认知需求来源保护(
    const 新_语言类& language,const 新_特征类& features,const 新接收认知来源& 来源,
    稳定编码 预期主体,const 新部分服务要求目标& 已知目标) const {
    using S=新需求操作状态;
        // 跨owner读取必须在需求锁外完成；完整U及目标定义读集进入最终追加CAS。
        const auto U=language.读取采用含义(来源.采用含义节点,features);
        const auto state=[](新语言含义采用状态 s) {
            if(s==新语言含义采用状态::前提已变化)return S::前提已变化;
            if(s==新语言含义采用状态::资源失败)return S::资源失败;
            if(s==新语言含义采用状态::来源冲突)return S::同来源目标冲突;
            if(s==新语言含义采用状态::未找到)return S::前提已变化;
            return S::认知来源不合法;
        };
        if(U.状态!=新语言含义采用状态::已采用&&U.状态!=新语言含义采用状态::精确重复)return {state(U.状态),{}};
        要求需求物理(U.信息&& !U.完整保护.empty());
        if(U.信息->节点!=来源.采用含义节点||U.信息->来源.接收消息节点!=来源.键.接收消息节点)return {S::同来源目标冲突,{}};
        const auto P=language.读取采用指定需求目标(来源.采用含义节点,来源.键,features);
        if(P.状态!=新语言含义采用状态::已采用&&P.状态!=新语言含义采用状态::精确重复)return {state(P.状态),{}};
        要求需求物理(P.信息&&!P.完整保护.empty());const auto& p=*P.信息;
        const auto full=投影已知需求目标(已知目标);
        if(!full||p.采用含义节点!=来源.采用含义节点||p.需求来源!=来源.键||!有效(p.需求主体)
            ||p.需求主体!=U.信息->说话者节点||p.需求主体!=预期主体
            ||full->目标存在节点!=p.目标存在||full->目标.目标特征类型概念节点!=p.目标特征类型)return {S::同来源目标冲突,{}};
        std::vector<基础联合节点快照> protect=U.完整保护;
        if(!并需求保护(protect,P.完整保护))return {S::前提已变化,{}};
        const auto FT=特征概念服务_.读取特征概念定义保护(p.目标特征类型);
        const auto add=[&](const auto& r) {
            if(r.前提已变化)throw 新需求目标定义核验状态::前提已变化;
            if(r.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            if(r.状态==基础结构操作状态::入口拒绝||r.状态==基础结构操作状态::节点不存在)return false;
            要求需求物理(r.状态==基础结构操作状态::已完成&&r.信息&&!r.预期节点组.empty());
            if(!并需求保护(protect,r.预期节点组))throw 新需求目标定义核验状态::前提已变化;
            return true;
        };
        if(!add(FT)||FT.信息->类型根!=p.目标特征类型)return {S::目标特征类型不合法,{}};
        const auto& ft=FT.信息->定义;
        if(ft.材料类型!=特征概念材料物理类型::I64标量)return {S::目标特征类型不合法,{}};
        if(p.方向) {
            const auto expected = *p.方向 == 新语言知识特征方向::增加 ? 新需求变化方向::增加 : 新需求变化方向::减少;
            if(p.准确整数||p.区间下界||p.区间上界||full->目标.目标形式!=新需求目标形式::变化方向
                ||full->目标.目标变化方向!=std::optional{expected}||p.单位)return {S::目标方向不合法,{}};
        } else {
            if(full->目标.目标形式!=新需求目标形式::值||!full->目标.目标值域概念节点
                ||!full->目标.目标比较关系概念节点)return {S::目标形式不合法,{}};
            const auto D=特征概念服务_.读取特征概念定义保护(*full->目标.目标值域概念节点);
            const auto C=二次特征概念服务_.读取二次特征定义保护(*full->目标.目标比较关系概念节点);
            if(!add(D))return {S::目标值域不合法,{}};
            if(!add(C))return {S::比较关系不合法,{}};
            const auto& domain=D.信息->定义;
            const auto* intervals=std::get_if<std::vector<特征概念I64闭区间>>(&domain.值域);
            if(domain.材料类型!=ft.材料类型
                ||D.信息->类型根!=p.目标特征类型||domain.单位!=ft.单位||p.单位!=ft.单位
                ||domain.比较规则!=ft.比较规则||domain.聚合规则!=ft.聚合规则
                ||!intervals||intervals->size()!=1)return {S::目标值域不合法,{}};
            const auto range=特征概念服务_.比较值域(ft.材料类型,domain.值域,ft.值域);
            if(range.状态==特征概念规则状态::资源失败)throw std::bad_alloc{};
            if(range.状态!=特征概念规则状态::已完成||!range.关系
                ||(*range.关系!=特征概念值域关系::相等&&*range.关系!=特征概念值域关系::右包含左))return {S::目标值域不合法,{}};
            const auto& interval=intervals->front();
            if(p.准确整数) {
                if(p.区间下界||p.区间上界||*p.准确整数!=std::to_string(interval.下界)
                    ||interval.下界!=interval.上界)return {S::目标值域不合法,{}};
            } else if(!p.区间下界||!p.区间上界||*p.区间下界!=std::to_string(interval.下界)
                ||*p.区间上界!=std::to_string(interval.上界))return {S::目标值域不合法,{}};
            const std::vector<二次特征概念输入定义> inputs{
                {0,二次特征输入角色::左输入,二次特征输入来源种类::特征准确值,p.目标特征类型},
                {1,二次特征输入角色::右输入,二次特征输入来源种类::特征准确值,p.目标特征类型}};
            const auto& comparison=*C.信息;
            if(comparison.算法!=二次特征算法::有向特征差值||comparison.输入定义组!=inputs
                ||comparison.参照特征概念节点!=full->目标.目标值域概念节点
                ||std::find(comparison.结果域种类组.begin(),comparison.结果域种类组.end(),二次特征结果域种类::包含于参照值域)==comparison.结果域种类组.end())return {S::比较关系不合法,{}};
        }
    if(!需求内容保护未变(protect))return {S::前提已变化,{}};
    return {S::已找到,std::move(protect)};
}
新需求建立结果 新_需求类::建立或取得接收认知需求(const 新接收认知需求建立请求& q,
    const 新_语言类& language,const 新_特征类& features) noexcept {
    using S=新需求操作状态;
    try {
        const auto U=language.读取采用含义(q.来源.采用含义节点,features);
        if(U.状态==新语言含义采用状态::前提已变化)return {S::前提已变化,{},{}};
        if(U.状态==新语言含义采用状态::资源失败)return {S::资源失败,{},{}};
        if(U.状态!=新语言含义采用状态::已采用&&U.状态!=新语言含义采用状态::精确重复) {
            if(U.状态==新语言含义采用状态::来源冲突)return {S::同来源目标冲突,{},{}};
            return {S::认知来源不合法,{},{}};
        }
        要求需求物理(U.信息&&!U.完整保护.empty());
        auto 来源=读取接收认知需求来源保护(language,features,q.来源,U.信息->说话者节点,q.已知目标);
        if(来源.状态!=S::已找到)return {来源.状态,{},{}};
        if(!并需求保护(来源.完整保护,U.完整保护))return {S::前提已变化,{},{}};
        return 建立待完善被认知需求内部({U.信息->说话者节点,q.已知目标},q.来源,来源.完整保护);
    }catch(新需求目标定义核验状态 s){要求需求物理(s==新需求目标定义核验状态::前提已变化);return {S::前提已变化,{},{}};}
    catch(const std::bad_alloc&){return {S::资源失败,{},{}};}
    catch(const std::length_error&){return {S::资源失败,{},{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"建立接收认知需求",L"首次接收认知需求建立发生工程故障");}
}

新需求建立结果 新_需求类::建立待完善被认知需求(const 新待完善被认知需求建立请求& q) noexcept {
    return 建立待完善被认知需求内部(q,{},{});
}
新需求建立结果 新_需求类::建立待完善被认知需求内部(const 新待完善被认知需求建立请求& q,
    const std::optional<新接收认知来源>& 来源,const std::vector<基础联合节点快照>& 来源保护) noexcept {
    try {
        std::lock_guard lock(新需求互斥);using S=新需求操作状态;
        if(!结构仍存在())return {S::尚未初始化,{},{}};
        基础节点组追加请求 r;
        r.预期联合节点组=来源保护;
        if(来源) {
            要求需求物理(!来源保护.empty());
            const auto existing=查询接收认知来源需求已加锁(来源->键,来源保护);
            if(existing.状态==S::已找到) {
                要求需求物理(existing.信息.has_value());const auto& old=*existing.信息;
                if(old.首次接收认知来源!=来源||old.需求主体存在节点!=q.需求主体存在节点
                    ||old.已知目标!=std::optional{q.已知目标})return {S::同来源目标冲突,{},{}};
                if(!需求内容保护未变(来源保护))return {S::前提已变化,{},{}};
                要求目标特征实例承载(old.节点);return {S::已复用,old.节点,{}};
            }
            if(existing.状态!=S::需求不存在)return {existing.状态,{},{}};
            if(!并需求保护(r.预期联合节点组,existing.完整保护))return {S::前提已变化,{},{}};
        }
        if(!存在服务_.是存在节点(q.需求主体存在节点)||!加需求存在保护(q.需求主体存在节点,存在服务_,r.预期联合节点组))return {S::目标存在不合法,{},{}};
        const auto status=核验部分需求目标(q.已知目标,存在服务_,特征概念服务_,二次特征概念服务_,r.预期联合节点组);
        if(status!=S::已找到)return {status,{},{}};
        std::vector<稳定编码> ids{需求树根节点,q.需求主体存在节点};if(q.已知目标.目标存在节点)ids.push_back(*q.已知目标.目标存在节点);
        std::sort(ids.begin(),ids.end());ids.erase(std::unique(ids.begin(),ids.end()),ids.end());
        const auto captured=全局基础数据集.查询节点组完整结构(ids);
        if(captured.状态==基础结构操作状态::资源失败)return {S::资源失败,{},{}};
        要求需求物理(captured.状态==基础结构操作状态::已完成&&captured.节点组.size()==ids.size());
        std::vector<基础联合节点快照> nodes;for(const auto& x:captured.节点组){if(!x.快照)return {S::前提已变化,{},{}};nodes.push_back(*x.快照);}
        if(!并需求保护(r.预期联合节点组,nodes))return {S::前提已变化,{},{}};
        基础组内新节点 d;d.字段组={{节点类型字段,需求节点类型},{需求主体字段,q.需求主体存在节点},
            {需求执行域字段,基础值{static_cast<std::int64_t>(新需求执行域::被认知)}},{需求权重字段,基础值{std::int64_t{0}}},
            {活动状态字段,基础值{static_cast<std::int64_t>(新需求活动状态::活动)}}};
        for(const auto& [key,value]:需求已知字段(q.已知目标,新需求信息完善阶段::待完善,{}))
            std::visit([&](const auto& v){d.字段组.push_back({key,v});},value);
        if(来源) {
            d.字段组.push_back({首次接收消息字段,来源->键.接收消息节点});
            d.字段组.push_back({首次采用含义字段,来源->采用含义节点});
            d.字段组.push_back({首次接收范围字段,基础值{std::vector<std::uint64_t>{来源->键.原文起点,来源->键.原文终点}}});
            d.字段组.push_back({首次接收路径字段,基础值{来源->键.根出现路径}});
        }
        r.新节点组.push_back(std::move(d));r.追加字段组.push_back({需求树根节点,被认知需求登记字段,基础组内节点引用{0}});
        const auto commit=全局基础数据集.原子追加节点组(r);
        if(commit.状态==基础节点组追加状态::预期不符)return {S::前提已变化,{},{}};
        if(commit.状态==基础节点组追加状态::资源失败)return {S::资源失败,{},{}};
        要求需求物理(commit.状态==基础节点组追加状态::已完成&&commit.新节点组.size()==1);
        const auto D=commit.新节点组.front();const auto read=读取需求治理信息(D);
        要求需求物理(read&&read->需求主体存在节点==q.需求主体存在节点&&read->已知目标==std::optional{q.已知目标}
            &&read->信息完善阶段==新需求信息完善阶段::待完善&&!read->认知来源&&read->首次接收认知来源==来源);
        要求目标特征实例承载(D);
        return {S::已建立,D,{}};
    }catch(新需求目标定义核验状态 s){要求需求物理(s==新需求目标定义核验状态::前提已变化);return {新需求操作状态::前提已变化,{},{}};}
    catch(const 需求物理读取失败& e){
        if(e.原因==基础结构操作状态::资源失败)return {新需求操作状态::资源失败,{},{}};
        if(e.原因==基础结构操作状态::节点不存在)return {新需求操作状态::前提已变化,{},{}};
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"建立待完善被认知需求",L"需求结构损坏");}
    catch(const std::bad_alloc&){return {新需求操作状态::资源失败,{},{}};}
    catch(const std::length_error&){return {新需求操作状态::资源失败,{},{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"建立待完善被认知需求",L"待完善需求建立发生工程故障");}
}
新被认知需求信息更新结果 新_需求类::更新被认知需求信息(const 新被认知需求信息更新请求& q) noexcept {
    try {
        std::unique_lock lock(新需求互斥);using S=新需求操作状态;
        const auto old=读取需求治理信息(q.需求节点);if(!old)return {S::需求不存在,{}};
        if(old->执行域!=新需求执行域::被认知)return {S::执行域不适用,{}};
        if(old->首次接收认知来源)return {S::认知来源不合法,{}};
        if(q.预期信息.节点!=q.需求节点||*old!=q.预期信息)return {S::前提已变化,{}};
        要求目标特征实例承载(q.需求节点);
        基础节点组追加请求 r;
        if(!加需求存在保护(old->需求主体存在节点,存在服务_,r.预期联合节点组))throw std::runtime_error("recognized demand subject identity missing");
        const auto status=核验部分需求目标(q.已知目标,存在服务_,特征概念服务_,二次特征概念服务_,r.预期联合节点组);
        if(status!=S::已找到)return {status,{}};
        std::optional<新服务要求确认语义保护> source;
        auto phase=新需求信息完善阶段::待完善;
        if(q.认知来源){
            const auto full=投影已知需求目标(q.已知目标);
            if(!full||!有效(q.认知来源->理解节点)||!有效(q.认知来源->内容确认节点))return {S::认知来源不合法,{}};
            if(!服务要求确认读取接口_)throw std::runtime_error("explicit demand confirmation owner not installed");
            const auto* reader=服务要求确认读取接口_;
            lock.unlock();
            auto p=reader->准备服务要求确认语义保护(q.认知来源->内容确认节点);
            lock.lock();
            if(p.状态==新服务要求确认保护状态::需重新确认)return {S::需重新确认,{}};
            if(p.状态==新服务要求确认保护状态::确认不存在)return {S::认知来源不合法,{}};
            if(p.状态==新服务要求确认保护状态::前提已变化)return {S::前提已变化,{}};
            if(p.状态==新服务要求确认保护状态::未装配)throw std::runtime_error("explicit demand confirmation dependency unavailable");
            要求需求物理(p.状态==新服务要求确认保护状态::已准备&&p.保护);
            const auto& c=p.保护->确认();
            if(c.来源人类节点!=old->需求主体存在节点||c.理解节点!=q.认知来源->理解节点||c.确认节点!=q.认知来源->内容确认节点
                ||c.来源要求节点!=q.认知来源->来源要求节点||c.目标存在节点!=full->目标存在节点||c.目标!=full->目标)return {S::认知来源不合法,{}};
            if(!并需求保护(r.预期联合节点组,p.保护->预期节点组()))return {S::前提已变化,{}};
            source=std::move(p.保护);phase=新需求信息完善阶段::已完善;
        }
        // 来源保护取得后再进入共享锁，避免确认owner与F的反向锁序。
        std::lock_guard featureLock(领域同步内部::存在特征互斥);
        std::vector<稳定编码> ids{q.需求节点,old->需求主体存在节点,需求树根节点};
        if(q.认知来源)for(const auto other:读取多节点字段(需求树根节点,被认知需求登记字段)) {
            ids.push_back(other);if(other==q.需求节点)continue;
            const auto d=读取需求治理信息(other);要求需求物理(d&&d->执行域==新需求执行域::被认知);
            if(d->需求主体存在节点==old->需求主体存在节点&&d->认知来源&&d->认知来源->理解节点==q.认知来源->理解节点)
                return {S::同来源目标冲突,{}};
        }
        if(q.已知目标.目标存在节点)ids.push_back(*q.已知目标.目标存在节点);
        std::sort(ids.begin(),ids.end());ids.erase(std::unique(ids.begin(),ids.end()),ids.end());
        const auto captured=全局基础数据集.查询节点组完整结构(ids);
        要求需求物理(captured.状态==基础结构操作状态::已完成&&captured.节点组.size()==ids.size());
        std::optional<基础联合节点快照> D;
        std::vector<基础联合节点快照> nodes;for(const auto& x:captured.节点组){if(!x.快照)return {S::前提已变化,{}};nodes.push_back(*x.快照);if(x.查询节点==q.需求节点)D=x.快照;}
        if(!D||!并需求保护(r.预期联合节点组,nodes)||读取需求治理信息(q.需求节点)!=old)return {S::前提已变化,{}};
        if(old->已知目标==std::optional{q.已知目标}&&old->信息完善阶段==phase&&old->认知来源==q.认知来源)return {S::无变化,*old};
        auto desired=需求已知字段(q.已知目标,phase,q.认知来源);
        const bool F承载=是F承载需求(D->结构);
        const auto keys=需求目标字段键();
        std::vector<新特征当前值设置项> settings;
        if(F承载) {
            (void)投影需求目标实例(需求目标读取上下文{特征服务_, 特征概念服务_}, D->结构, &r.预期联合节点组);
            for(const auto key:keys) {
                const auto f=std::find_if(D->结构.字段组.begin(),D->结构.字段组.end(),[&](const auto& x){return x.字段节点==key;});
                要求需求物理(f!=D->结构.字段组.end());
                const auto* id=std::get_if<稳定编码>(&f->内容);要求需求物理(id);
                std::optional<新特征准确值> value;
                if(const auto v=desired.find(key);v!=desired.end()) {
                    if(const auto* ref=std::get_if<稳定编码>(&v->second))value=新特征准确值{*ref};
                    else {const auto* material=std::get_if<基础值>(&v->second);
                        const auto* n=material?std::get_if<std::int64_t>(&material->材料):nullptr;要求需求物理(n);value=新特征准确值{*n};}
                }
                settings.push_back({*id,value});desired.erase(key);
            }
        }
        std::set<稳定编码> managed{目标存在字段,目标特征类型字段,目标形式字段,目标值域字段,目标比较关系字段,目标变化方向字段,
            需求信息完善阶段字段,认知理解字段,认知内容确认字段,认知来源要求字段};
        if(F承载)for(const auto key:keys)managed.erase(key);
        std::set<稳定编码> present;
        for(const auto& f:D->结构.字段组)if(managed.contains(f.字段节点)){
            present.insert(f.字段节点);const auto it=desired.find(f.字段节点);
            if(it==desired.end())r.删除字段关系组.push_back(f.编码);
            else if(it->second!=f.内容)r.替换字段组.push_back({f.编码,it->second});
        }
        for(const auto& [key,value]:desired)if(!present.contains(key))std::visit([&](const auto& v){r.追加字段组.push_back({q.需求节点,key,v});},value);
        const bool rawChanged=!r.删除字段关系组.empty()||!r.替换字段组.empty()||!r.追加字段组.empty();
        if(F承载) {
            auto prepared=特征服务_.准备特征当前值组(settings);
            if(prepared.状态==新特征I64准备状态::预期已变化)return {S::前提已变化,{}};
            if(prepared.状态==新特征I64准备状态::资源失败)throw std::bad_alloc{};
            要求需求物理(prepared.状态==新特征I64准备状态::已准备&&prepared.准备);
            std::vector<基础封存追加片段> parts;
            if(rawChanged) {
                const auto sealed=全局基础数据集.封存原子追加片段(r);
                if(sealed.状态==基础节点组追加状态::预期不符){特征服务_.取消特征当前值组(*prepared.准备);return {S::前提已变化,{}};}
                if(sealed.状态==基础节点组追加状态::资源失败){特征服务_.取消特征当前值组(*prepared.准备);throw std::bad_alloc{};}
                要求需求物理(sealed.状态==基础节点组追加状态::已完成&&sealed.片段);parts.push_back(*sealed.片段);
            } else if(!需求内容保护未变(r.预期联合节点组)) {
                特征服务_.取消特征当前值组(*prepared.准备);return {S::前提已变化,{}};
            }
            const auto committed=特征服务_.提交特征当前值组(*prepared.准备,parts);
            if(committed.状态!=基础节点组追加状态::已完成) {
                特征服务_.取消特征当前值组(*prepared.准备);
                if(committed.状态==基础节点组追加状态::预期不符)return {S::前提已变化,{}};
                if(committed.状态==基础节点组追加状态::资源失败)throw std::bad_alloc{};
                要求需求物理(false);
            }
            要求需求物理(committed.附加片段回执组.size()==parts.size());
            for(std::size_t i=0;i<parts.size();++i)要求需求物理(committed.附加片段回执组[i].对应片段(parts[i]));
            const auto completed=特征服务_.完成特征当前值组(*prepared.准备);
            要求需求物理(completed.项结果组.size()==settings.size());
            特征服务_.结束特征当前值组(*prepared.准备);
        } else {
            if(!rawChanged)return {S::无变化,*old};
            const auto commit=全局基础数据集.原子追加节点组(r);
            if(commit.状态==基础节点组追加状态::预期不符)return {S::前提已变化,{}};
            要求需求物理(commit.状态==基础节点组追加状态::已完成);
        }
        const auto read=读取需求治理信息(q.需求节点);
        要求需求物理(read&&read->已知目标==std::optional{q.已知目标}&&read->信息完善阶段==phase
            &&read->认知来源==q.认知来源&&read->需求主体存在节点==old->需求主体存在节点);
        return {S::已修改,*read};
    }catch(新需求目标定义核验状态 s){要求需求物理(s==新需求目标定义核验状态::前提已变化);return {新需求操作状态::前提已变化,{}};}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"更新被认知需求信息",L"同一需求信息更新发生工程故障");}
}
新需求建立结果 新_需求类::建立或取得被认知需求(const 新被认知需求建立请求& 请求) noexcept {
    if(请求.目标.引用内容定位||请求.目标.新增成员来源见证)return {新需求操作状态::目标形式不合法,{},{}};
    try {
        std::lock_guard 锁(新需求互斥);
        using S = 新需求操作状态;
        if (!结构仍存在() || !有效(安全根需求节点) || !有效(服务根需求节点)) return {S::尚未初始化, {}, {}};
        if (!服务要求确认读取接口_) return {S::入口拒绝, {}, {}};
        if (!存在服务_.是存在节点(请求.需求主体存在节点) || !存在服务_.是存在节点(请求.目标存在节点))
            return {S::目标存在不合法, {}, {}};
        if (!目标形状合法(请求.目标)) return {S::目标形式不合法, {}, {}};
        if (!特征概念服务_.是特征概念节点(请求.目标.目标特征类型概念节点))
            return {S::目标特征类型不合法, {}, {}};
        if (请求.目标.目标形式 == 新需求目标形式::值) {
            const auto 类型根 = 取得特征类型根(*请求.目标.目标值域概念节点, 特征概念服务_);
            if (!类型根 || *类型根 != 请求.目标.目标特征类型概念节点) return {S::目标值域不合法, {}, {}};
            const auto 比较 = 二次特征概念服务_.获取二次特征概念(*请求.目标.目标比较关系概念节点);
            if (!比较 || !比较关系适用于目标(*比较, *类型根, *请求.目标.目标值域概念节点, 特征概念服务_))
                return {S::比较关系不合法, {}, {}};
        }
        if (!有效(请求.认知来源.理解节点) || !有效(请求.认知来源.内容确认节点))
            return {S::认知来源不合法, {}, {}};
        for (;;) {
            const auto 来源 = 服务要求确认读取接口_->复核服务要求确认语义(请求.认知来源.内容确认节点);
            if (来源.状态 == 新服务要求确认状态::需重新确认) return {S::需重新确认, {}, {}};
            if (来源.状态 == 新服务要求确认状态::未装配) return {S::入口拒绝, {}, {}};
            if (来源.状态 == 新服务要求确认状态::确认不存在) return {S::认知来源不合法, {}, {}};
            要求需求物理(来源.状态 == 新服务要求确认状态::当前有效 && 来源.确认);
            const auto& c = *来源.确认;
            if (c.确认节点 != 请求.认知来源.内容确认节点 || c.理解节点 != 请求.认知来源.理解节点
                || c.来源人类节点 != 请求.需求主体存在节点 || c.目标存在节点 != 请求.目标存在节点
                || c.目标 != 请求.目标 || c.来源要求节点 != 请求.认知来源.来源要求节点)
                return {S::认知来源不合法, {}, {}};
            const auto 全集 = 查询全部需求记录();
            if (全集.状态 != S::已找到) return {全集.状态, {}, {}};
            for (const auto& d : 全集.需求组) {
                if (d.执行域 != 新需求执行域::被认知 || d.需求主体存在节点 != 请求.需求主体存在节点
                    || !d.认知来源 || d.认知来源->理解节点 != 请求.认知来源.理解节点) continue;
                if (d.目标存在节点 != 请求.目标存在节点 || d.目标 != 请求.目标 || *d.认知来源 != 请求.认知来源)
                    return {S::同来源目标冲突, {}, {}};
                要求目标特征实例承载(d.节点);
                return {S::已复用, d.节点, {}};
            }
            std::vector<稳定编码> 预期节点{需求树根节点, 请求.需求主体存在节点, 请求.目标存在节点,
                请求.认知来源.理解节点, 请求.认知来源.内容确认节点, 请求.目标.目标特征类型概念节点};
            if (请求.目标.目标值域概念节点) 预期节点.push_back(*请求.目标.目标值域概念节点);
            if (请求.目标.目标比较关系概念节点) 预期节点.push_back(*请求.目标.目标比较关系概念节点);
            if (请求.认知来源.来源要求节点) 预期节点.push_back(*请求.认知来源.来源要求节点);
            std::sort(预期节点.begin(), 预期节点.end());
            预期节点.erase(std::unique(预期节点.begin(), 预期节点.end()), 预期节点.end());
            const auto 快照 = 全局基础数据集.查询节点组完整结构(预期节点);
            要求需求物理(快照.状态 == 基础结构操作状态::已完成 && 快照.节点组.size() == 预期节点.size());
            基础节点组追加请求 发布;
            for (const auto& 项 : 快照.节点组) { 要求需求物理(项.快照.has_value()); 发布.预期联合节点组.push_back(*项.快照); }
            // 快照之后重新核验来源；发布再核对相同节点的联合预期。
            // 不把先读到的确认DTO与后来捕获的已变节点拼接成合法发布。
            const auto 再核验 = 服务要求确认读取接口_->复核服务要求确认语义(请求.认知来源.内容确认节点);
            if (再核验.状态 == 新服务要求确认状态::需重新确认) return {S::需重新确认, {}, {}};
            if (再核验.状态 == 新服务要求确认状态::确认不存在) return {S::认知来源不合法, {}, {}};
            要求需求物理(再核验.状态 == 新服务要求确认状态::当前有效 && 再核验.确认);
            if (*再核验.确认 != c) continue;
            基础组内新节点 d;
            d.字段组 = {{节点类型字段, 需求节点类型}, {需求主体字段, 请求.需求主体存在节点},
                {需求执行域字段, 基础值{static_cast<std::int64_t>(新需求执行域::被认知)}},
                {需求信息完善阶段字段,基础值{static_cast<std::int64_t>(新需求信息完善阶段::已完善)}},
                {目标存在字段, 请求.目标存在节点}, {目标特征类型字段, 请求.目标.目标特征类型概念节点},
                {目标形式字段, 基础值{static_cast<std::int64_t>(请求.目标.目标形式)}},
                {需求权重字段, 基础值{std::int64_t{0}}},
                {活动状态字段, 基础值{static_cast<std::int64_t>(新需求活动状态::活动)}},
                {认知理解字段, 请求.认知来源.理解节点}, {认知内容确认字段, 请求.认知来源.内容确认节点}};
            if (请求.认知来源.来源要求节点) d.字段组.push_back({认知来源要求字段, *请求.认知来源.来源要求节点});
            if (请求.目标.目标值域概念节点) d.字段组.push_back({目标值域字段, *请求.目标.目标值域概念节点});
            if (请求.目标.目标比较关系概念节点) d.字段组.push_back({目标比较关系字段, *请求.目标.目标比较关系概念节点});
            if (请求.目标.目标变化方向) d.字段组.push_back({目标变化方向字段, 基础值{static_cast<std::int64_t>(*请求.目标.目标变化方向)}});
            发布.新节点组.push_back(std::move(d));
            发布.追加字段组.push_back({需求树根节点, 被认知需求登记字段, 基础组内节点引用{0}});
            const auto 写入 = 全局基础数据集.原子追加节点组(发布);
            if (写入.状态 == 基础节点组追加状态::预期不符) continue;
            if (写入.状态 == 基础节点组追加状态::资源失败) return {S::资源失败, {}, {}};
            要求需求物理(写入.状态 == 基础节点组追加状态::已完成 && 写入.新节点组.size() == 1);
            const auto 回读 = 获取需求(写入.新节点组.front());
            要求需求物理(回读 && 回读->执行域 == 新需求执行域::被认知
                && 回读->需求主体存在节点 == 请求.需求主体存在节点 && 回读->目标存在节点 == 请求.目标存在节点
                && 回读->目标 == 请求.目标 && 回读->认知来源 == 请求.认知来源);
            要求目标特征实例承载(回读->节点);
            return {S::已建立, 回读->节点, {}};
        }
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立被认知需求", L"被认知需求发布故障"); }
}

struct 新需求特征事实凭据::实现 final {
    新需求特征事实依据 内容;
    const 新_世界树类* 世界;
    std::thread::id 线程 = std::this_thread::get_id();
    void 核对() const noexcept {
        if (!世界 || 线程 != std::this_thread::get_id())
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"需求事实凭据", L"签发来源无效或跨线程使用");
    }
};
新需求特征事实凭据::新需求特征事实凭据(新需求特征事实依据 a, const 新_世界树类* w) noexcept {
    try { 实现_ = std::make_unique<实现>(std::move(a), w); 实现_->核对(); }
    catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"需求事实凭据", L"凭据分配故障"); }
}
新需求特征事实凭据::新需求特征事实凭据(新需求特征事实凭据&& x) noexcept {
    if (x.实现_) x.实现_->核对(); 实现_ = std::move(x.实现_);
}
新需求特征事实凭据& 新需求特征事实凭据::operator=(新需求特征事实凭据&& x) noexcept {
    if (实现_) 实现_->核对(); if (x.实现_) x.实现_->核对();
    if (this != &x) 实现_ = std::move(x.实现_); return *this;
}
新需求特征事实凭据::~新需求特征事实凭据() noexcept { if (实现_) 实现_->核对(); }
const 新需求特征事实依据& 新需求特征事实凭据::依据() const noexcept {
    if (!实现_) 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"需求事实凭据", L"凭据已移动");
    实现_->核对(); return 实现_->内容;
}
struct 新需求特征迁移派生凭据::实现 final {
    新需求特征迁移派生依据 内容;
    const 新_需求类* owner;
    std::thread::id 线程 = std::this_thread::get_id();
    void 核对() const noexcept {
        if (!owner || 线程 != std::this_thread::get_id())
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"需求迁移派生凭据", L"owner无效或跨线程使用");
    }
};
新需求特征迁移派生凭据::新需求特征迁移派生凭据(std::unique_ptr<实现> x) noexcept : 实现_(std::move(x)) {}
新需求特征迁移派生凭据::新需求特征迁移派生凭据(新需求特征迁移派生凭据&& x) noexcept {
    if (x.实现_) x.实现_->核对(); 实现_ = std::move(x.实现_);
}
新需求特征迁移派生凭据& 新需求特征迁移派生凭据::operator=(新需求特征迁移派生凭据&& x) noexcept {
    if (实现_) 实现_->核对(); if (x.实现_) x.实现_->核对();
    if (this != &x) 实现_ = std::move(x.实现_); return *this;
}
新需求特征迁移派生凭据::~新需求特征迁移派生凭据() noexcept { if (实现_) 实现_->核对(); }
const 新需求特征迁移派生依据& 新需求特征迁移派生凭据::依据() const noexcept {
    if (!实现_) 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"需求迁移派生凭据", L"凭据已移动");
    实现_->核对(); return 实现_->内容;
}

struct 新需求集合发布准备::实现 final {
    const 新_需求类* owner = nullptr;
    std::thread::id 线程 = std::this_thread::get_id();
    新需求集合发布关联 关联;
    新需求发生核心定位 发生;
    std::optional<基础封存追加片段> 封存;
    void 核对() const noexcept {
        if (线程 != std::this_thread::get_id())
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"需求集合发布", L"准备不可跨线程");
    }
};
新需求集合发布准备::新需求集合发布准备(std::unique_ptr<实现> 内容) noexcept : 实现_(std::move(内容)) {}
新需求集合发布准备::新需求集合发布准备(新需求集合发布准备&& 其它) noexcept {
    if (其它.实现_) 其它.实现_->核对();
    实现_ = std::move(其它.实现_);
}
新需求集合发布准备& 新需求集合发布准备::operator=(新需求集合发布准备&& 其它) noexcept {
    if (实现_) 实现_->核对();
    if (其它.实现_) 其它.实现_->核对();
    if (this != &其它) 实现_ = std::move(其它.实现_);
    return *this;
}
新需求集合发布准备::~新需求集合发布准备() noexcept { if (实现_) 实现_->核对(); }
const 基础封存追加片段& 新需求集合发布准备::片段() const noexcept {
    if (!实现_ || !实现_->封存)
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"需求集合发布", L"准备缺少封存片段");
    实现_->核对(); return *实现_->封存;
}
const 新需求发生核心定位& 新需求集合发布准备::发生定位() const noexcept {
    if(!实现_||!实现_->owner)工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"需求发生准备",L"准备已移动或owner无效");
    实现_->核对();return 实现_->发生;
}

新需求集合发布关联结果 新_需求类::读取主体需求集合发布关联(稳定编码 需求节点) const noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        const auto 读取 = 全局基础数据集.查询完整节点结构(需求节点);
        if (读取.状态 == 基础结构操作状态::节点不存在) return {新需求操作状态::需求不存在, {}};
        要求需求物理(读取.状态 == 基础结构操作状态::已完成 && 读取.结构);
        const auto D = 解码需求治理物理(需求目标读取上下文{特征服务_, 特征概念服务_}, *读取.结构);
        std::optional<稳定编码> 场景, 动作;
        for (const auto& f : 读取.结构->字段组) {
            if (f.字段节点 != 集合发布场景字段 && f.字段节点 != 集合发布动作字段) continue;
            const auto* p = std::get_if<稳定编码>(&f.内容);
            要求需求物理(p && 有效(*p));
            auto& 项 = f.字段节点 == 集合发布场景字段 ? 场景 : 动作;
            要求需求物理(!项); 项 = *p;
        }
        要求需求物理(场景.has_value() == 动作.has_value());
        if (!场景) return {新需求操作状态::已找到, {}};
        要求需求物理(D.执行域 == 新需求执行域::被认知);
        return {新需求操作状态::已找到, 新需求集合发布关联{需求节点, D.需求主体存在节点, *场景, *动作}};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取需求集合来源", L"来源字段或需求结构不完整");
    }
}

新需求集合发布准备结果 新_需求类::准备主体需求特征发布(
    稳定编码 D, 稳定编码 主体, 稳定编码 F, 稳定编码 场景, 稳定编码 动作,
    const 基础联合节点快照& F保护) noexcept {
    try {
        using S = 新需求操作状态;
        std::unique_lock 锁(新需求互斥);
        const auto 信息 = 读取需求治理信息(D);
        if (!信息) return {S::需求不存在, {}};
        if (信息->执行域 != 新需求执行域::被认知) return {S::执行域不适用, {}};
        if (信息->需求主体存在节点 != 主体) return {S::认知来源不合法, {}};
        if(信息->信息完善阶段!=新需求信息完善阶段::已完善)return {S::目标内容依赖未就绪,{}};
        要求需求物理(信息->信息完善阶段&&F保护.结构.节点.编码 == F);
        const auto D快照 = 全局基础数据集.查询节点组完整结构({D});
        要求需求物理(D快照.状态 == 基础结构操作状态::已完成
            && D快照.节点组.size() == 1 && D快照.节点组[0].快照);
        std::vector<基础联合节点快照> 来源保护;
        if(信息->信息完善阶段==新需求信息完善阶段::已完善) {
            锁.unlock();
            const auto 资格=读取被认知需求完整资格保护(D,主体);
            switch(资格.状态) {
            case 新被认知需求资格状态::需重新确认:return {S::需重新确认,{}};
            case 新被认知需求资格状态::前提已变化:return {S::前提已变化,{}};
            case 新被认知需求资格状态::资源失败:return {S::资源失败,{}};
            case 新被认知需求资格状态::依赖未就绪:return {S::尚未初始化,{}};
            case 新被认知需求资格状态::已读取:break;
            default:return {S::认知来源不合法,{}};
            }
            要求需求物理(资格.信息.has_value());
            if(资格.信息->需求!=*信息)return {S::前提已变化,{}};
            来源保护=资格.完整保护;
            锁.lock();
            if(读取需求治理信息(D)!=信息)return {S::前提已变化,{}};
        }
        const auto 已有 = 读取主体需求集合发布关联(D);
        要求需求物理(已有.状态 == S::已找到);
        if (已有.关联) {
            要求需求物理(已有.关联->主体存在节点 == 主体);
            return {S::已复用, {}};
        }
        std::set<稳定编码> 编码{D, 主体, F, 场景, 动作};
        if(信息->已知目标&&信息->已知目标->目标存在节点)编码.insert(*信息->已知目标->目标存在节点);
        for(const auto& n:来源保护)编码.insert(n.结构.节点.编码);
        const auto 快照 = 全局基础数据集.查询节点组完整结构(std::vector<稳定编码>{编码.begin(), 编码.end()});
        要求需求物理(快照.状态 == 基础结构操作状态::已完成 && 快照.节点组.size() == 编码.size());
        基础节点组追加请求 请求;
        const auto 相同 = [](const auto& a, const auto& b) {
            return a.结构.节点 == b.结构.节点 && a.结构.字段组 == b.结构.字段组
                && a.入向字段组 == b.入向字段组 && a.入向外部关系组 == b.入向外部关系组
                && a.出向外部关系组 == b.出向外部关系组;
        };
        for (const auto& 项 : 快照.节点组) {
            if (!项.快照) return {S::前提已变化, {}};
            const auto& k = *项.快照;
            if (项.查询节点 == F && (k.结构.节点 != F保护.结构.节点
                || k.结构.字段组 != F保护.结构.字段组 || k.入向字段组 != F保护.入向字段组
                || k.入向外部关系组 != F保护.入向外部关系组 || k.出向外部关系组 != F保护.出向外部关系组))
                return {S::前提已变化, {}};
            if (项.查询节点 == D && !相同(k, *D快照.节点组[0].快照)) return {S::前提已变化, {}};
            for (const auto& p : 来源保护)
                if (项.查询节点 == p.结构.节点.编码 && !相同(k, p)) return {S::前提已变化, {}};
            请求.预期联合节点组.push_back(k);
        }
        if (读取需求治理信息(D) != 信息) return {S::前提已变化, {}};
        请求.追加字段组 = {{D, 集合发布场景字段, 场景}, {D, 集合发布动作字段, 动作}};
        const auto 封存 = 全局基础数据集.封存原子追加片段(请求);
        if (封存.状态 == 基础节点组追加状态::预期不符) return {S::前提已变化, {}};
        要求需求物理(封存.状态 == 基础节点组追加状态::已完成 && 封存.片段);
        auto 内容 = std::make_unique<新需求集合发布准备::实现>();
        内容->owner = this; 内容->关联 = {D, 主体, 场景, 动作}; 内容->封存 = *封存.片段;
        内容->发生={场景,主体,F,D,动作,新需求发生方式::新需求};
        return {S::已建立, 新需求集合发布准备{std::move(内容)}};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"准备需求集合来源", L"封存准备遇到工程异常");
    }
}

新需求集合发布准备结果 新_需求类::准备主体需求再次需要(
    稳定编码 D,稳定编码 H,稳定编码 F,稳定编码 C,稳定编码 A,const 基础联合节点快照& F保护) noexcept {
    using S=新需求操作状态;
    try {
        std::unique_lock 锁(新需求互斥);
        const auto 信息=读取需求治理信息(D);
        if(!信息)return {S::需求不存在,{}};
        if(信息->执行域!=新需求执行域::被认知)return {S::执行域不适用,{}};
        if(信息->需求主体存在节点!=H)return {S::认知来源不合法,{}};
        if(信息->信息完善阶段!=新需求信息完善阶段::已完善)return {S::目标内容依赖未就绪,{}};
        if(信息->活动状态!=新需求活动状态::已满足)return {S::需求未满足,{}};
        const auto 首次=读取主体需求特征发布关联(D);
        if(!首次.关联)return {S::认知来源不合法,{}};
        要求需求物理(首次.关联->主体存在节点==H&&F保护.结构.节点.编码==F);
        锁.unlock();
        const auto 资格=读取被认知需求完整资格保护(D,H);
        if(资格.状态==新被认知需求资格状态::需重新确认)return {S::需重新确认,{}};
        if(资格.状态==新被认知需求资格状态::前提已变化)return {S::前提已变化,{}};
        if(资格.状态==新被认知需求资格状态::资源失败)throw std::bad_alloc{};
        if(资格.状态!=新被认知需求资格状态::已读取||!资格.信息)return {S::认知来源不合法,{}};
        锁.lock();
        if(读取需求治理信息(D)!=信息||资格.信息->需求!=*信息)return {S::前提已变化,{}};
        std::set<稳定编码> 编码{D,H,F,C,A};
        for(const auto& 项:资格.完整保护)编码.insert(项.结构.节点.编码);
        const auto 批=全局基础数据集.查询节点组完整结构({编码.begin(),编码.end()});
        要求需求物理(批.状态==基础结构操作状态::已完成&&批.节点组.size()==编码.size());
        基础节点组追加请求 请求;
        for(const auto& 项:批.节点组) {
            if(!项.快照)return {S::前提已变化,{}};
            const auto 相同=[](const auto& x,const auto& y){return x.结构.节点==y.结构.节点&&x.结构.字段组==y.结构.字段组
                &&x.入向字段组==y.入向字段组&&x.入向外部关系组==y.入向外部关系组&&x.出向外部关系组==y.出向外部关系组;};
            if(项.查询节点==F&&!相同(*项.快照,F保护))return {S::前提已变化,{}};
            for(const auto& 原:资格.完整保护)if(原.结构.节点.编码==项.查询节点&&!相同(原,*项.快照))return {S::前提已变化,{}};
            if(项.查询节点==D) {
                const auto 字段=std::find_if(项.快照->结构.字段组.begin(),项.快照->结构.字段组.end(),
                    [](const auto& x){return x.字段节点==活动状态字段;});
                要求需求物理(字段!=项.快照->结构.字段组.end());
                请求.替换字段组.push_back({字段->编码,基础值{static_cast<std::int64_t>(新需求活动状态::活动)}});
            }
            请求.预期联合节点组.push_back(*项.快照);
        }
        const auto 封存=全局基础数据集.封存原子追加片段(请求);
        if(封存.状态==基础节点组追加状态::预期不符)return {S::前提已变化,{}};
        要求需求物理(封存.状态==基础节点组追加状态::已完成&&封存.片段);
        auto 内容=std::make_unique<新需求集合发布准备::实现>();
        内容->owner=this;内容->关联={D,H,C,A};内容->发生={C,H,F,D,A,新需求发生方式::已满足再次需要};内容->封存=*封存.片段;
        return {S::已建立,新需求集合发布准备{std::move(内容)}};
    }catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"准备主体需求再次需要",L"再次需要来源或组合片段工程异常");}
}

void 新_需求类::完成主体需求再次需要(const 新需求集合发布准备& 准备,const 基础片段发布回执& 回执) noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        要求需求物理(准备.实现_&&准备.实现_->owner==this&&回执.对应片段(准备.片段()));
        const auto& 定位=准备.发生定位();
        要求需求物理(定位.方式==新需求发生方式::已满足再次需要&&回执.新节点组().empty());
        const auto 当前=读取需求治理信息(定位.需求节点);
        要求需求物理(当前&&当前->活动状态==新需求活动状态::活动&&当前->需求主体存在节点==定位.主体存在节点);
    }catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"完成主体需求再次需要",L"共同提交回执或活动状态不符");}
}

namespace {
新需求发生关联读取结果 读取需求发生关联物理(稳定编码 节点) {
    const auto 读=全局基础数据集.查询完整节点结构(节点);
    要求需求物理(读.状态==基础结构操作状态::已完成&&读.结构);
    需求物理游标 c{*读.结构,{}};
    要求需求物理(c.引用(节点类型字段)==需求发生关联类型);
    新需求发生关联 r;
    r.定位.场景节点=*c.引用(发生场景字段);r.定位.主体存在节点=*c.引用(发生主体字段);
    r.定位.特征节点=*c.引用(发生特征字段);r.定位.需求节点=*c.引用(发生需求字段);
    r.定位.动作节点=*c.引用(发生动作字段);r.动作来源节点=*c.引用(发生来源字段);r.状态节点=*c.引用(发生状态字段);
    const auto 方式=*c.数值(发生方式字段);
    要求需求物理(方式==1||方式==2);r.定位.方式=static_cast<新需求发生方式>(方式);c.完成();
    for(const auto n:{r.定位.场景节点,r.定位.主体存在节点,r.定位.特征节点,r.定位.需求节点,
        r.定位.动作节点,r.动作来源节点,r.状态节点})要求需求物理(节点仍存在(n));
    return {新需求发生关联读取状态::已读取,r};
}
新需求发生关联读取结果 按需求发生引用读取(稳定编码 目标,稳定编码 字段) {
    const auto 索引=全局基础数据集.查询目标字段(目标,字段);
    if(索引.empty())return {};
    要求需求物理(索引.size()==1);
    return 读取需求发生关联物理(索引.front().所属节点);
}
}
新需求发生关联读取结果 新_需求类::读取主体需求发生关联按动作(稳定编码 A) const noexcept {
    try{std::lock_guard 锁(新需求互斥);return 按需求发生引用读取(A,发生动作字段);}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"按动作读需求发生",L"发生来源引用损坏");}
}
新需求发生关联读取结果 新_需求类::读取主体需求发生关联按来源(稳定编码 来源) const noexcept {
    try{std::lock_guard 锁(新需求互斥);return 按需求发生引用读取(来源,发生来源字段);}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"按来源读需求发生",L"发生来源引用损坏");}
}
新需求发生关联读取结果 新_需求类::读取主体需求发生关联按状态(稳定编码 S) const noexcept {
    try{std::lock_guard 锁(新需求互斥);return 按需求发生引用读取(S,发生状态字段);}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"按状态读需求发生",L"发生状态引用损坏");}
}
新需求操作状态 新_需求类::记录主体需求发生关联(const 新需求发生关联& r) noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        const auto a=按需求发生引用读取(r.定位.动作节点,发生动作字段);
        const auto o=按需求发生引用读取(r.动作来源节点,发生来源字段);
        const auto s=按需求发生引用读取(r.状态节点,发生状态字段);
        if(a.关联||o.关联||s.关联) {
            要求需求物理(a.关联==std::optional{r}&&o.关联==a.关联&&s.关联==a.关联);return 新需求操作状态::已复用;
        }
        要求需求物理(r.定位.方式==新需求发生方式::新需求||r.定位.方式==新需求发生方式::已满足再次需要);
        基础节点组追加请求 q;
        q.新节点组.push_back({{{节点类型字段,需求发生关联类型},{发生场景字段,r.定位.场景节点},
            {发生主体字段,r.定位.主体存在节点},{发生特征字段,r.定位.特征节点},{发生需求字段,r.定位.需求节点},
            {发生动作字段,r.定位.动作节点},{发生来源字段,r.动作来源节点},{发生状态字段,r.状态节点},
            {发生方式字段,基础值{static_cast<std::int64_t>(r.定位.方式)}}}});
        for(const auto n:{r.定位.场景节点,r.定位.主体存在节点,r.定位.特征节点,r.定位.需求节点,r.定位.动作节点,
            r.动作来源节点,r.状态节点}) {要求需求物理(节点仍存在(n));q.只读预期.节点组.push_back({n});}
        const auto 发=全局基础数据集.原子追加节点组(q);
        要求需求物理(发.状态==基础节点组追加状态::已完成&&发.新节点组.size()==1);
        要求需求物理(读取需求发生关联物理(发.新节点组.front()).关联==std::optional{r});return 新需求操作状态::已建立;
    }catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"记录主体需求发生",L"同次来源冲突或发生关联发表失败");}
}
bool 新_需求类::是需求迁移恢复状态(稳定编码 S) const noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        const auto 组=全局基础数据集.查询目标字段(S,迁移当前状态字段);
        if(组.empty())return false;
        要求需求物理(组.size()==1);
        const auto 记录=全局基础数据集.查询字段(组.front().所属节点,节点类型字段);
        要求需求物理(记录.size()==1&&记录.front().内容==基础字段内容{需求迁移记录类型});return true;
    }catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取迁移恢复状态分类",L"迁移状态引用损坏");}
}

void 新_需求类::完成主体需求特征发布(
    const 新需求集合发布准备& 准备, const 基础片段发布回执& 回执) noexcept {
    try {
        要求需求物理(准备.实现_ && 准备.实现_->owner == this);
        准备.实现_->核对();
        要求需求物理(回执.对应片段(准备.片段()) && 回执.新节点组().empty());
        const auto 关联 = 读取主体需求集合发布关联(准备.实现_->关联.需求节点);
        要求需求物理(关联.状态 == 新需求操作状态::已找到 && 关联.关联 == 准备.实现_->关联);
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"完成需求集合来源", L"联合回执或来源读回不符");
    }
}

新需求特征发布关联结果 新_需求类::读取主体需求特征发布关联(稳定编码 D) const noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        const auto r = 读取主体需求集合发布关联(D);
        新需求特征发布关联结果 out; out.状态 = r.状态;
        if (!r.关联) return out;
        const auto& a = *r.关联;
        const auto 字段 = 全局基础数据集.查询字段(D, 需求首次发布首状态字段);
        要求需求物理(字段.size() <= 1);
        std::optional<稳定编码> S;
        if (!字段.empty()) {
            const auto* p = std::get_if<稳定编码>(&字段[0].内容);
            要求需求物理(p && 有效(*p) && 节点仍存在(*p)); S = *p;
        }
        out.关联 = 新需求特征发布关联{a.需求节点, a.主体存在节点, a.场景节点, a.动作节点, S};
        return out;
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取需求特征来源", L"来源结构读取故障"); }
}

namespace {
bool 需求完整快照相同(const 基础联合节点快照& a, const 基础联合节点快照& b) {
    return a.结构.节点 == b.结构.节点 && a.结构.字段组 == b.结构.字段组
        && a.入向字段组 == b.入向字段组 && a.入向外部关系组 == b.入向外部关系组
        && a.出向外部关系组 == b.出向外部关系组;
}
bool 加入需求完整保护(基础节点组追加请求& 请求, const std::vector<基础联合节点快照>& 组) {
    for (const auto& 项 : 组) {
        const auto it = std::find_if(请求.预期联合节点组.begin(), 请求.预期联合节点组.end(),
            [&](const auto& x) { return x.结构.节点.编码 == 项.结构.节点.编码; });
        if (it == 请求.预期联合节点组.end()) 请求.预期联合节点组.push_back(项);
        else if (!需求完整快照相同(*it, 项)) return false;
    }
    return true;
}
std::optional<基础联合节点快照> 需求当前完整快照(稳定编码 n) {
    const auto r = 全局基础数据集.查询节点组完整结构({n});
    要求需求物理(r.状态 == 基础结构操作状态::已完成 && r.节点组.size() == 1);
    return r.节点组.front().快照;
}
新需求操作状态 需求封存联合追加(基础节点组追加请求 请求, const 基础封存追加片段& 保护) {
    const auto p = 全局基础数据集.封存原子追加片段(请求);
    if (p.状态 == 基础节点组追加状态::预期不符) return 新需求操作状态::前提已变化;
    要求需求物理(p.状态 == 基础节点组追加状态::已完成 && p.片段);
    const auto r = 全局基础数据集.原子追加封存片段组({*p.片段, 保护});
    if (r.状态 == 基础节点组追加状态::预期不符) return 新需求操作状态::前提已变化;
    要求需求物理(r.状态 == 基础节点组追加状态::已完成 && r.片段回执组.size() == 2
        && r.片段回执组[0].对应片段(*p.片段) && r.片段回执组[1].对应片段(保护));
    return 新需求操作状态::已建立;
}
}

新需求操作状态 新_需求类::完成主体需求首状态关联(稳定编码 D, const 新需求特征事实凭据& 凭据) noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        const auto& 依据 = 凭据.依据(); const auto& x = 依据.已核验输入;
        要求需求物理(x.用途 == 新需求特征事实用途::首次状态 && x.当前需求节点 == D
            && x.动作节点 && x.实际状态节点 && !有效(x.原特征节点) && x.有序来源组.empty());
        const auto 来源 = 读取主体需求特征发布关联(D);
        要求需求物理(来源.状态 == 新需求操作状态::已找到 && 来源.关联
            && 来源.关联->主体存在节点 == x.主体存在节点 && 来源.关联->场景节点 == x.场景节点
            && 来源.关联->动作节点 == *x.动作节点);
        if (来源.关联->首状态节点) {
            要求需求物理(来源.关联->首状态节点 == x.实际状态节点); return 新需求操作状态::已复用;
        }
        const auto 身份 = 读取需求节点引用身份(D);
        要求需求物理(身份.状态 == 特征概念规则状态::已完成 && 身份.身份 && 身份.身份->主体存在节点 == x.主体存在节点);
        基础节点组追加请求 请求;
        if (!加入需求完整保护(请求, 依据.完整保护组) || !加入需求完整保护(请求, 身份.身份保护))
            return 新需求操作状态::前提已变化;
        请求.追加字段组.push_back({D, 需求首次发布首状态字段, *x.实际状态节点});
        const auto r = 需求封存联合追加(std::move(请求), 依据.保护片段);
        if (r == 新需求操作状态::前提已变化) return r;
        const auto 后 = 读取主体需求特征发布关联(D);
        要求需求物理(后.关联 && 后.关联->首状态节点 == x.实际状态节点);
        return 新需求操作状态::已建立;
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"需求首状态关联", L"凭据绑定、联合发布或读回故障"); }
}

struct 新需求特征迁移准备::实现 final {
    const 新_需求类* owner = nullptr;
    std::thread::id 线程 = std::this_thread::get_id();
    新需求特征事实核验输入 输入;
    std::optional<基础封存追加片段> 封存;
    void 核对() const noexcept {
        if (!owner || 线程 != std::this_thread::get_id())
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"需求迁移准备", L"owner或线程不符");
    }
};
新需求特征迁移准备::新需求特征迁移准备(std::unique_ptr<实现> x) noexcept : 实现_(std::move(x)) {}
新需求特征迁移准备::新需求特征迁移准备(新需求特征迁移准备&& x) noexcept {
    if (x.实现_) x.实现_->核对(); 实现_ = std::move(x.实现_);
}
新需求特征迁移准备& 新需求特征迁移准备::operator=(新需求特征迁移准备&& x) noexcept {
    if (实现_) 实现_->核对(); if (x.实现_) x.实现_->核对();
    if (this != &x) 实现_ = std::move(x.实现_); return *this;
}
新需求特征迁移准备::~新需求特征迁移准备() noexcept { if (实现_) 实现_->核对(); }
const 基础封存追加片段& 新需求特征迁移准备::片段() const noexcept {
    if (!实现_ || !实现_->封存) 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"需求迁移准备", L"缺少封存片段");
    实现_->核对(); return *实现_->封存;
}

新需求特征迁移读取结果 新_需求类::读取需求特征迁移(稳定编码 F) const noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        const auto 登记 = 全局基础数据集.查询字段(F, 需求迁移登记字段);
        if (登记.empty()) return {新需求操作状态::已找到, {}};
        要求需求物理(登记.size() == 1);
        const auto* n = std::get_if<稳定编码>(&登记[0].内容); 要求需求物理(n && 有效(*n));
        const auto 结构 = 全局基础数据集.查询完整节点结构(*n);
        要求需求物理(结构.状态 == 基础结构操作状态::已完成 && 结构.结构);
        需求物理游标 c{*结构.结构, {}};
        要求需求物理(c.引用(节点类型字段) == 需求迁移记录类型);
        新需求特征迁移信息 out;
        out.迁移记录节点 = *n; out.场景节点 = *c.引用(迁移场景字段); out.主体存在节点 = *c.引用(迁移主体字段);
        out.原特征节点 = *c.引用(迁移原特征字段); out.新特征节点 = *c.引用(迁移新特征字段);
        out.当前需求节点 = *c.引用(迁移当前需求字段); out.迁移当前状态节点 = c.引用(迁移当前状态字段, false);
        要求需求物理(out.新特征节点 == F);
        std::map<std::int64_t, 新需求特征迁移来源项> 有序;
        for (const auto& 项 : c.引用组(迁移来源项字段)) {
            const auto s = 全局基础数据集.查询完整节点结构(项);
            要求需求物理(s.状态 == 基础结构操作状态::已完成 && s.结构);
            需求物理游标 v{*s.结构, {}};
            要求需求物理(v.引用(节点类型字段) == 需求迁移来源项类型);
            const auto 序号 = *v.数值(迁移项序号字段);
            新需求特征迁移来源项 r{*v.引用(迁移项需求字段), *v.引用(迁移项动作字段),
                *v.引用(迁移项前状态字段), *v.引用(迁移项后状态字段)};
            v.完成(); 要求需求物理(序号 >= 0 && 有序.emplace(序号, r).second);
        }
        c.完成(); 要求需求物理(!有序.empty());
        std::set<稳定编码> D组;
        for (const auto& [序号, 项] : 有序) {
            要求需求物理(序号 == static_cast<std::int64_t>(out.有序来源组.size()) && D组.insert(项.需求节点).second);
            const auto 来源 = 读取主体需求集合发布关联(项.需求节点);
            要求需求物理(来源.状态 == 新需求操作状态::已找到 && 来源.关联
                && 来源.关联->主体存在节点 == out.主体存在节点 && 来源.关联->场景节点 == out.场景节点
                && 来源.关联->动作节点 == 项.动作节点);
            out.有序来源组.push_back(项);
        }
        要求需求物理(out.有序来源组.back().需求节点 == out.当前需求节点);
        const auto 当前F = 特征服务_.获取特征(F);
        要求需求物理(当前F.has_value());
        for (const auto D : D组) 要求需求物理(std::find(当前F->历史不同值.begin(), 当前F->历史不同值.end(), 新特征准确值{D}) != 当前F->历史不同值.end());
        return {新需求操作状态::已找到, std::move(out)};
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取需求迁移映射", L"迁移结构或来源读回故障"); }
}

新需求特征迁移准备结果 新_需求类::准备需求特征迁移(const 新需求特征事实凭据& 凭据) noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        const auto& a = 凭据.依据(); const auto& x = a.已核验输入;
        要求需求物理(x.用途 == 新需求特征事实用途::旧集合迁移 && !x.动作节点 && !x.实际状态节点
            && 有效(x.原特征节点) && 有效(x.新特征节点) && !x.有序来源组.empty()
            && x.有序来源组.back().需求节点 == x.当前需求节点);
        const auto 已有 = 读取需求特征迁移(x.新特征节点);
        if (已有.信息) {
            const auto& m = *已有.信息;
            要求需求物理(m.场景节点 == x.场景节点 && m.主体存在节点 == x.主体存在节点
                && m.原特征节点 == x.原特征节点 && m.当前需求节点 == x.当前需求节点 && m.有序来源组 == x.有序来源组);
            return {新需求操作状态::已复用, {}};
        }
        基础节点组追加请求 请求;
        if (!加入需求完整保护(请求, a.完整保护组)) return {新需求操作状态::前提已变化, {}};
        基础组内新节点 映射;
        映射.字段组 = {{节点类型字段, 需求迁移记录类型}, {迁移场景字段, x.场景节点}, {迁移主体字段, x.主体存在节点},
            {迁移原特征字段, x.原特征节点}, {迁移新特征字段, x.新特征节点}, {迁移当前需求字段, x.当前需求节点}};
        请求.新节点组.push_back(std::move(映射));
        std::set<稳定编码> 已见;
        for (const auto& 项 : x.有序来源组) {
            要求需求物理(已见.insert(项.需求节点).second);
            const auto D = 获取需求(项.需求节点);
            const auto 来源 = 读取主体需求集合发布关联(项.需求节点);
            要求需求物理(D && D->执行域 == 新需求执行域::被认知 && D->认知来源 && D->需求主体存在节点 == x.主体存在节点
                && 来源.关联 && 来源.关联->场景节点 == x.场景节点 && 来源.关联->动作节点 == 项.动作节点);
            const auto 身份 = 读取需求节点引用身份(项.需求节点);
            要求需求物理(身份.状态 == 特征概念规则状态::已完成 && 身份.身份);
            if (!加入需求完整保护(请求, 身份.身份保护)) return {新需求操作状态::前提已变化, {}};
            const auto i = 请求.新节点组.size();
            请求.新节点组.push_back({{{节点类型字段, 需求迁移来源项类型}, {迁移项需求字段, 项.需求节点},
                {迁移项动作字段, 项.动作节点}, {迁移项前状态字段, 项.前状态节点}, {迁移项后状态字段, 项.后状态节点},
                {迁移项序号字段, 基础值{基础原始值{static_cast<std::int64_t>(i - 1)}}}}});
            请求.新节点组[0].字段组.push_back({迁移来源项字段, 基础组内节点引用{i}});
        }
        请求.追加字段组.push_back({x.新特征节点, 需求迁移登记字段, 基础组内节点引用{0}});
        const auto 封存 = 全局基础数据集.封存原子追加片段(请求);
        if (封存.状态 == 基础节点组追加状态::预期不符) return {新需求操作状态::前提已变化, {}};
        要求需求物理(封存.状态 == 基础节点组追加状态::已完成 && 封存.片段);
        // 世界封存保护的全部快照已原样进入本片段；只读片段不另造可变证明。
        auto 内容 = std::make_unique<新需求特征迁移准备::实现>();
        内容->owner = this; 内容->输入 = x; 内容->封存 = *封存.片段;
        return {新需求操作状态::已建立, 新需求特征迁移准备{std::move(内容)}};
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"准备需求迁移", L"迁移证据或封存工程故障"); }
}
void 新_需求类::完成需求特征迁移核心(const 新需求特征迁移准备& p, const 基础片段发布回执& r) noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        要求需求物理(p.实现_ && p.实现_->owner == this); p.实现_->核对();
        要求需求物理(r.对应片段(p.片段()) && r.新节点组().size() == p.实现_->输入.有序来源组.size() + 1);
        const auto m = 读取需求特征迁移(p.实现_->输入.新特征节点);
        要求需求物理(m.信息 && m.信息->迁移记录节点 == r.新节点组()[0]
            && m.信息->有序来源组 == p.实现_->输入.有序来源组);
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"完成需求迁移核心", L"回执或映射读回不符"); }
}
新需求操作状态 新_需求类::完成需求特征迁移状态(稳定编码 F, const 新需求特征事实凭据& 凭据) noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        const auto& a = 凭据.依据(); const auto& x = a.已核验输入;
        要求需求物理(x.用途 == 新需求特征事实用途::迁移当前状态 && x.新特征节点 == F && x.实际状态节点 && !x.动作节点);
        const auto m = 读取需求特征迁移(F); 要求需求物理(m.信息.has_value());
        const auto& v = *m.信息;
        要求需求物理(v.场景节点 == x.场景节点 && v.主体存在节点 == x.主体存在节点 && v.原特征节点 == x.原特征节点
            && v.当前需求节点 == x.当前需求节点 && v.有序来源组 == x.有序来源组);
        if (v.迁移当前状态节点) { 要求需求物理(v.迁移当前状态节点 == x.实际状态节点); return 新需求操作状态::已复用; }
        基础节点组追加请求 请求;
        if (!加入需求完整保护(请求, a.完整保护组)) return 新需求操作状态::前提已变化;
        const auto 快照 = 需求当前完整快照(v.迁移记录节点); 要求需求物理(快照.has_value());
        if (!加入需求完整保护(请求, {*快照})) return 新需求操作状态::前提已变化;
        请求.追加字段组.push_back({v.迁移记录节点, 迁移当前状态字段, *x.实际状态节点});
        const auto r = 需求封存联合追加(std::move(请求), a.保护片段);
        if (r == 新需求操作状态::前提已变化) return r;
        const auto 后 = 读取需求特征迁移(F); 要求需求物理(后.信息 && 后.信息->迁移当前状态节点 == x.实际状态节点);
        return 新需求操作状态::已建立;
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"完成需求迁移状态", L"状态凭据绑定或发表故障"); }
}

新需求特征迁移派生准备结果 新_需求类::准备需求特征迁移派生(稳定编码 F, const 新需求特征事实凭据& 凭据) const noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        const auto& a = 凭据.依据(); const auto& x = a.已核验输入;
        要求需求物理(x.用途 == 新需求特征事实用途::旧集合迁移 && x.新特征节点 == F);
        const auto m = 读取需求特征迁移(F); 要求需求物理(m.信息.has_value());
        const auto& v = *m.信息;
        要求需求物理(v.场景节点 == x.场景节点 && v.主体存在节点 == x.主体存在节点 && v.原特征节点 == x.原特征节点
            && v.当前需求节点 == x.当前需求节点 && v.有序来源组 == x.有序来源组 && !v.有序来源组.empty());
        const auto 特征 = 特征服务_.获取特征(F);
        要求需求物理(特征 && 特征->当前值 == std::optional<新特征准确值>{新特征准确值{x.当前需求节点}});
        const auto 定义 = 特征概念服务_.获取特征概念(特征->特征概念节点);
        要求需求物理(定义 && 定义->材料类型 == 特征概念材料物理类型::需求节点引用 && 定义->比较规则 == 特征概念比较规则::需求身份精确);
        基础节点组追加请求 请求;
        if (!加入需求完整保护(请求, a.完整保护组)) return {新需求操作状态::前提已变化, {}};
        std::vector<稳定编码> 编码{F, v.迁移记录节点};
        for (const auto& f : 全局基础数据集.查询字段(v.迁移记录节点, 迁移来源项字段)) {
            const auto* n = std::get_if<稳定编码>(&f.内容); 要求需求物理(n); 编码.push_back(*n);
        }
        for (const auto& 项 : 特征->历史不同值) {
            const auto* D = std::get_if<稳定编码>(&项); 要求需求物理(D);
            const auto 身份 = 读取需求节点引用身份(*D); 要求需求物理(身份.状态 == 特征概念规则状态::已完成 && 身份.身份);
            if (!加入需求完整保护(请求, 身份.身份保护)) return {新需求操作状态::前提已变化, {}};
        }
        for (const auto n : 编码) {
            const auto s = 需求当前完整快照(n); 要求需求物理(s.has_value());
            if (!加入需求完整保护(请求, {*s})) return {新需求操作状态::前提已变化, {}};
        }
        const auto f = std::find_if(请求.预期联合节点组.begin(), 请求.预期联合节点组.end(), [&](const auto& s) { return s.结构.节点.编码 == F; });
        要求需求物理(f != 请求.预期联合节点组.end());
        const auto 封存 = 全局基础数据集.封存原子追加片段(请求);
        if (封存.状态 == 基础节点组追加状态::预期不符) return {新需求操作状态::前提已变化, {}};
        要求需求物理(封存.状态 == 基础节点组追加状态::已完成 && 封存.片段);
        const auto 类型根 = 取得特征类型根(定义->节点, 特征概念服务_); 要求需求物理(类型根.has_value());
        auto 内容 = std::make_unique<新需求特征迁移派生凭据::实现>(新需求特征迁移派生依据{
            x.场景节点, x.主体存在节点, x.原特征节点, F, *类型根,
            x.当前需求节点, v.有序来源组.back().后状态节点, *f, 请求.预期联合节点组, *封存.片段}, this);
        return {新需求操作状态::已建立, 新需求特征迁移派生凭据{std::move(内容)}};
    } catch (...) { 工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"准备需求迁移派生", L"映射与事实凭据核验故障"); }
}

新需求列表全集结果 新_需求类::查询全部需求列表() const noexcept {
    auto 读取 = 查询全部需求列表(std::stop_token{});
    if (auto* 结果 = std::get_if<新需求列表全集结果>(&读取)) return std::move(*结果);
    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取全部需求列表", L"无停止能力的读取返回取消或无值结果。");
}

新需求列表全集可停止结果 新_需求类::查询全部需求列表(std::stop_token 停止) const noexcept {
    try { return 查询全部需求列表可抛内部(停止); }
    catch (...) { return 新需求列表全集结果{新需求操作状态::资源失败,{},{}}; }
}

新需求列表全集可停止结果 新_需求类::查询全部需求列表可抛内部(
    std::stop_token 停止,来源候选枚举保护批* 保护) const {
    新需求列表全集结果 结果;
    结果.状态 = 新需求操作状态::结构不一致;
    try {
        auto 锁 = 获取需求读取锁(停止);
        auto 全集读取 = 查询全部需求可抛内部(停止,保护);
        if (std::holds_alternative<新需求读取取消>(全集读取)) throw 新需求读取取消{};
        auto* 全集结果 = std::get_if<新需求全集结果>(&全集读取);
        if (!全集结果)
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取全部需求列表", L"需求全集返回无值结果。");
        const auto& 全集 = *全集结果;
        if (全集.状态 != 新需求操作状态::已找到) {
            结果.状态 = 全集.状态;
            return 结果;
        }
        检查需求读取停止(停止);
        std::map<稳定编码, 新需求治理信息> 需求映射;
        for (const auto 节点 : 全集.需求节点组) {
            检查需求读取停止(停止);
            const auto 信息 = 读取需求治理信息可抛内部(节点,保护);
            if (!信息) return 结果;
            检查需求读取停止(停止);
            if (信息->完整目标) {
                const auto 物理 = 读取需求治理物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 节点);
                if (物理.完整目标 != 信息->完整目标) return 结果;
                检查需求读取停止(停止);
            }
            if (!需求映射.emplace(节点, *信息).second) return 结果;
        }

        std::set<稳定编码> 登记列表;
        检查需求读取停止(停止);
        const auto 列表登记字段 = 全局基础数据集.查询字段(需求树根节点, 需求列表项字段);
        检查需求读取停止(停止);
        for (const auto& 字段 : 列表登记字段) {
            检查需求读取停止(停止);
            const auto* 节点 = std::get_if<稳定编码>(&字段.内容);
            if (!节点 || !是列表项类型节点(*节点)
                || !登记列表.insert(*节点).second) return 结果;
            检查需求读取停止(停止);
        }
        std::set<稳定编码> 类型列表;
        检查需求读取停止(停止);
        const auto 列表类型字段 = 全局基础数据集.查询目标字段(需求列表项节点类型, 节点类型字段);
        检查需求读取停止(停止);
        for (const auto& 字段 : 列表类型字段) {
            检查需求读取停止(停止);
            if (!是列表项类型节点(字段.所属节点)
                || !类型列表.insert(字段.所属节点).second) return 结果;
            检查需求读取停止(停止);
        }
        if (登记列表 != 类型列表) return 结果;
        检查需求读取停止(停止);

        // 根列表与普通列表分账；普通需求可合法具有与根相同的目标。
        using 见证键=std::tuple<稳定编码,稳定编码,std::optional<稳定编码>,std::optional<稳定编码>>;
        using 列表键 = std::tuple<std::optional<新需求根角色>, 稳定编码,
            稳定编码, 新需求目标形式, std::optional<稳定编码>,
            std::optional<稳定编码>, std::optional<新需求变化方向>,std::optional<见证键>>;
        std::set<列表键> 目标键组;
        std::set<稳定编码> 已覆盖需求;
        std::vector<新需求列表项信息> 列表候选;
        for (const auto 节点 : 登记列表) {
            检查需求读取停止(停止);
            if(保护)追加来源候选枚举原始闭包保护(节点,*保护);
            auto 信息 = 读取列表项信息(节点);
            if (!信息) return 结果;
            检查需求读取停止(停止);
            if (!原始目标字段一致(节点, 信息->目标)) return 结果;
            检查需求读取停止(停止);
            const auto& 目标 = 信息->目标;
            std::optional<见证键> cw;
            if(目标.新增成员来源见证){const auto& x=*目标.新增成员来源见证;
                cw=见证键{x.要求.源特征类型概念节点,x.基准材料节点,x.要求.指定来源主体,x.要求.指定被认知需求};}
            if (!目标键组.emplace(信息->根角色, 信息->目标存在节点,
                    目标.目标特征类型概念节点, 目标.目标形式,
                    目标.目标值域概念节点, 目标.目标比较关系概念节点,
                    目标.目标变化方向,cw).second) return 结果;
            std::set<稳定编码> 成员;
            检查需求读取停止(停止);
            const auto 成员字段 = 全局基础数据集.查询字段(节点, 列表成员字段);
            检查需求读取停止(停止);
            for (const auto& 字段 : 成员字段) {
                检查需求读取停止(停止);
                const auto* 需求节点 = std::get_if<稳定编码>(&字段.内容);
                if (!需求节点 || !成员.insert(*需求节点).second
                    || !已覆盖需求.insert(*需求节点).second) return 结果;
                const auto 需求 = 需求映射.find(*需求节点);
                if (需求 == 需求映射.end()
                    || !需求->second.完整目标
                    || 需求->second.完整目标->目标存在节点 != 信息->目标存在节点
                    || 需求->second.完整目标->目标 != 目标
                    || 需求->second.根角色 != 信息->根角色) return 结果;
                检查需求读取停止(停止);
            }
            if (成员.empty() || (信息->根角色 && 成员.size() != 1)) return 结果;
            检查需求读取停止(停止);
            信息->需求节点组.assign(成员.begin(), 成员.end());
            列表候选.push_back(std::move(*信息));
            检查需求读取停止(停止);
        }
        for (const auto& [id, d] : 需求映射) {
            检查需求读取停止(停止);
            if (已覆盖需求.contains(id) != d.完整目标.has_value()) return 结果;
        }
        检查需求读取停止(停止);
        std::vector<新需求治理信息> 需求候选;
        需求候选.reserve(需求映射.size());
        for (auto& [节点, 信息] : 需求映射) {
            检查需求读取停止(停止);
            (void)节点;
            需求候选.push_back(std::move(信息));
        }
        结果.列表项组 = std::move(列表候选);
        结果.需求组 = std::move(需求候选);
        结果.状态 = 新需求操作状态::已找到;
        检查需求读取停止(停止);
        return 结果;
    } catch (const 新需求读取取消&) {
        return 新需求读取取消{};
    }
}

std::vector<稳定编码> 新_需求类::查询直接子需求(
    稳定编码 需求节点) const noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        std::vector<稳定编码> 结果;
        if (!读取需求治理信息(需求节点)) return 结果;
        for (const auto& 关系 : 查询子需求关系(需求节点)) {
            if (读取需求治理信息(关系.目标节点)) 结果.push_back(关系.目标节点);
        }
        std::sort(结果.begin(), 结果.end());
        结果.erase(std::unique(结果.begin(), 结果.end()), 结果.end());
        return 结果;
    } catch (...) {
        return {};
    }
}

std::vector<稳定编码> 新_需求类::查询目标存在需求(
    稳定编码 目标存在节点) const noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        std::vector<稳定编码> 结果;
        for (const auto& 字段 : 全局基础数据集.查询目标字段(
            目标存在节点, 目标存在字段)) {
            if (读取需求信息(需求目标读取上下文{特征服务_, 特征概念服务_}, 字段.所属节点)) 结果.push_back(字段.所属节点);
        }
        std::sort(结果.begin(), 结果.end());
        结果.erase(std::unique(结果.begin(), 结果.end()), 结果.end());
        return 结果;
    } catch (...) {
        return {};
    }
}

新需求精确查询结果 新_需求类::按完整目标查询(
    稳定编码 目标存在节点,
    const 新需求目标定义& 目标) const noexcept {
    try { return 按完整目标查询可抛内部(目标存在节点,目标); }
    catch (...) { return 新需求精确查询结果{新需求操作状态::资源失败,{},{}}; }
}

新需求精确查询结果 新_需求类::按完整目标查询可抛内部(
    稳定编码 目标存在节点,const 新需求目标定义& 目标) const {
    新需求精确查询结果 结果;
    {
        std::lock_guard 锁(新需求互斥);
        if (!结构仍存在()) {
            结果.状态 = 新需求操作状态::尚未初始化;
            return 结果;
        }
        std::optional<新需求列表项信息> 唯一命中;
        for (const auto 列表项 : 读取多节点字段(需求树根节点, 需求列表项字段)) {
            const auto 信息 = 读取列表项信息(列表项);
            if (!信息) {
                结果.状态 = 新需求操作状态::结构不一致;
                return 结果;
            }
            if (信息->根角色) continue;
            if (信息->目标存在节点 == 目标存在节点
                && 目标相同(信息->目标, 目标)) {
                if (唯一命中) {
                    结果.状态 = 新需求操作状态::结构不一致;
                    return 结果;
                }
                唯一命中 = *信息;
            }
        }
        if (唯一命中) {
            结果.状态 = 新需求操作状态::已找到;
            结果.需求列表项节点 = 唯一命中->节点;
            结果.需求节点组 = 唯一命中->需求节点组;
            return 结果;
        }
        结果.状态 = 新需求操作状态::需求不存在;
        return 结果;
    }
}

namespace {
struct 筹办保护前提已变化 final {};
struct 筹办保护依赖未就绪 final {};
}

struct 新_需求类::筹办保护收集批 final {
    enum class 中止原因 : std::uint8_t { 无,前提已变化,依赖未就绪 };
    std::vector<基础联合节点快照> 完整保护;
    std::set<稳定编码> 已采集本域节点组;
    中止原因 原因=中止原因::无;
    [[noreturn]] void 变化() {原因=中止原因::前提已变化;throw 筹办保护前提已变化{};}
    [[noreturn]] void 未就绪() {原因=中止原因::依赖未就绪;throw 筹办保护依赖未就绪{};}
    void 合并(const std::vector<基础联合节点快照>& p) {
        if(!并需求保护(完整保护,p))变化();
    }
    基础联合节点快照 捕获(稳定编码 n) {
        const auto r=全局基础数据集.查询节点组完整结构({n});
        if(r.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        要求需求物理(r.状态==基础结构操作状态::已完成&&r.节点组.size()==1&&r.节点组.front().查询节点==n);
        if(!r.节点组.front().快照)变化();
        要求需求物理(r.节点组.front().快照->结构.节点.编码==n);
        合并({*r.节点组.front().快照});return *r.节点组.front().快照;
    }
    特征概念定义保护信息 特征定义(const 概念_特征类& owner,稳定编码 n) {
        const auto identity=owner.读取正式概念身份保护(n);
        if(identity.状态==正式概念读取状态::依赖未就绪)未就绪();
        if(identity.状态==正式概念读取状态::前提已变化)变化();
        要求需求物理(identity.状态==正式概念读取状态::已读取&&identity.身份&&identity.身份->概念节点==n
            &&!identity.身份->完整保护组.empty());
        合并(identity.身份->完整保护组);
        const auto r=owner.读取特征概念定义保护(n);
        if(r.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        if(r.前提已变化)变化();
        要求需求物理(r.状态==基础结构操作状态::已完成&&r.信息&&r.信息->定义.节点==n&&!r.预期节点组.empty());
        合并(r.预期节点组);return *r.信息;
    }
    二次特征概念信息 二次定义(const 概念_二次特征类& owner,const 概念_特征类& fc,稳定编码 n) {
        const auto identity=owner.读取正式概念身份保护(n);
        if(identity.状态==正式概念读取状态::依赖未就绪)未就绪();
        if(identity.状态==正式概念读取状态::前提已变化)变化();
        要求需求物理(identity.状态==正式概念读取状态::已读取&&identity.身份&&identity.身份->概念节点==n
            &&!identity.身份->完整保护组.empty());
        合并(identity.身份->完整保护组);
        const auto r=owner.读取二次特征定义保护(n);
        if(r.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        if(r.前提已变化)变化();
        要求需求物理(r.状态==基础结构操作状态::已完成&&r.信息&&r.信息->节点==n&&!r.预期节点组.empty());
        合并(r.预期节点组);
        for(const auto& input:r.信息->输入定义组)if(input.特征类型概念节点)(void)特征定义(fc,*input.特征类型概念节点);
        if(r.信息->参照特征概念节点)(void)特征定义(fc,*r.信息->参照特征概念节点);
        (void)特征定义(fc,r.信息->输出特征概念.节点);
        return *r.信息;
    }
    bool 比较适用(const 二次特征概念信息& comparison,稳定编码 FT,稳定编码 domain,const 概念_特征类& fc) {
        std::map<稳定编码,稳定编码> roots;
        for(const auto& input:comparison.输入定义组)if(input.特征类型概念节点)
            roots.emplace(*input.特征类型概念节点,特征定义(fc,*input.特征类型概念节点).类型根);
        return 比较关系适用纯判据(comparison,FT,domain,[&](稳定编码 C)->std::optional<稳定编码>{
            const auto it=roots.find(C);return it==roots.end() ? std::nullopt : std::optional{it->second};
        });
    }
    void 核同() {if(!需求内容保护未变(完整保护))变化();}
};

void 新_需求类::追加筹办本域保护(稳定编码 Ds,筹办保护收集批& 批,bool 来源枚举读取) const {
    if(!结构已初始化())批.未就绪();
    (void)批.捕获(需求树根节点);
    std::vector<稳定编码> pending{Ds,安全根需求节点,服务根需求节点};
    // 根登记严格读取还会核所有出生记录/来源快照，只沿本owner正式键扩展。
    for(const auto R:读取多节点字段(需求树根节点,服务取得实例登记字段))pending.push_back(R);
    for(std::size_t i=0;i<pending.size();++i) {
        const auto n=pending[i];
        if(!批.已采集本域节点组.insert(n).second)continue;
        const auto raw=批.捕获(n);
        需求物理游标 c{raw.结构,{}};
        const auto type=*c.引用(节点类型字段);
        for(const auto& f:raw.结构.字段组) {
            // 中性完整保护引用端点；不解释其它owner的字段。
            if(const auto* ref=std::get_if<稳定编码>(&f.内容)) {
                (void)批.捕获(*ref);
                if(f.字段节点==引用内容需求字段||f.字段节点==新增成员指定需求字段
                    ||f.字段节点==复合父需求字段||f.字段节点==实例父需求字段||f.字段节点==实例需求字段)
                    pending.push_back(*ref);
            }
        }
        if(type==需求节点类型) {
            if(是F承载需求(raw.结构)) {
                for(const auto slot:需求目标字段槽()) {
                    const auto ft=特征概念服务_.读取先天特征类型槽(slot);
                    if(ft.状态==特征概念规则状态::资源失败)throw std::bad_alloc{};
                    if(ft.状态==特征概念规则状态::前提已变化)批.变化();
                    if(ft.状态==特征概念规则状态::读取接口未装配||ft.状态==特征概念规则状态::值不存在)批.未就绪();
                    要求需求物理(ft.状态==特征概念规则状态::已完成&&ft.类型根);
                    批.合并(ft.完整保护组);
                }
            }
            (void)投影需求目标实例(需求目标读取上下文{特征服务_,特征概念服务_},raw.结构,&批.完整保护,来源枚举读取);
            for(const auto& f:raw.入向字段组)if(f.字段节点==列表成员字段)pending.push_back(f.所属节点);
            for(const auto& rel:raw.入向外部关系组)
                if(rel.类型==基础外部关系类型::父子&&rel.角色或顺序==需求父子关系角色)pending.push_back(rel.源节点);
            for(const auto& f:raw.结构.字段组)
                if(f.字段节点==目标新增成员见证字段||f.字段节点==复合分解来源字段||f.字段节点==服务取得实例来源字段) {
                    const auto* ref=std::get_if<稳定编码>(&f.内容);要求需求物理(ref&&有效(*ref));pending.push_back(*ref);
                }
        } else if(type==需求列表项节点类型) {
            for(const auto D:c.引用组(列表成员字段))pending.push_back(D);
        } else if(type==服务取得实例记录类型) {
            const auto record=解码服务取得实例记录物理(n,批.完整保护);
            pending.push_back(record.父需求);pending.push_back(record.需求节点);
            pending.push_back(*读取唯一节点字段(n,实例原父目标字段));
        } else if(type==实例原父目标类型) {
            const auto cw=c.引用(目标新增成员见证字段,false);if(cw)pending.push_back(*cw);
        } else 要求需求物理(type==新增成员见证类型||type==复合分解来源类型);
    }
}

void 新_需求类::追加来源候选枚举原始闭包保护(
    稳定编码 起点,来源候选枚举保护批& 批) const {
    // 先固定真实来源再沿本owner的具名引用扩展；中性端点不解释为另一领域对象。
    const auto 捕获=[&](稳定编码 n) {
        要求需求物理(有效(n));
        const auto r=全局基础数据集.查询节点组完整结构({n});
        if(r.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        要求需求物理(r.状态==基础结构操作状态::已完成&&r.节点组.size()==1&&r.节点组.front().查询节点==n);
        if(!r.节点组.front().快照) {
            const bool 曾采=std::any_of(批.完整保护.begin(),批.完整保护.end(),[&](const auto& p){return p.结构.节点.编码==n;});
            if(曾采||!需求内容保护未变(批.完整保护))throw 筹办保护前提已变化{};
            要求需求物理(false);
        }
        const auto& raw=*r.节点组.front().快照;
        要求需求物理(raw.结构.节点.编码==n);
        if(!并需求保护(批.完整保护,{raw}))throw 筹办保护前提已变化{};
        return raw;
    };
    std::vector<稳定编码> pending{起点};
    for(std::size_t i=0;i<pending.size();++i) {
        const auto n=pending[i];
        if(批.已捕获原始闭包节点组.contains(n))continue;
        const auto raw=捕获(n);
        批.已捕获原始闭包节点组.insert(n);
        需求物理游标 c{raw.结构,{}};const auto type=*c.引用(节点类型字段);
        if(n==需求树根节点)要求需求物理(type==需求树根节点类型);
        要求需求物理(type==需求树根节点类型||type==需求节点类型||type==需求列表项节点类型
            ||type==服务取得实例记录类型||type==实例原父目标类型||type==新增成员见证类型||type==复合分解来源类型);
        for(const auto& f:raw.结构.字段组) {
            const auto* ref=std::get_if<稳定编码>(&f.内容);
            if(!ref)continue;
            (void)捕获(*ref);
            bool 展开=false;
            if(type==需求树根节点类型)展开=f.字段节点==安全根登记字段||f.字段节点==服务根登记字段
                ||f.字段节点==需求列表项字段||f.字段节点==被认知需求登记字段
                ||f.字段节点==新增成员见证登记字段||f.字段节点==服务取得实例登记字段;
            else if(type==需求节点类型)展开=f.字段节点==引用内容需求字段||f.字段节点==目标新增成员见证字段
                ||f.字段节点==复合分解来源字段||f.字段节点==服务取得实例来源字段;
            else if(type==需求列表项节点类型)展开=f.字段节点==列表成员字段
                ||f.字段节点==目标新增成员见证字段||f.字段节点==引用内容需求字段;
            else if(type==服务取得实例记录类型)展开=f.字段节点==实例父需求字段
                ||f.字段节点==实例需求字段||f.字段节点==实例原父目标字段;
            else if(type==实例原父目标类型)展开=f.字段节点==所属实例记录字段
                ||f.字段节点==目标新增成员见证字段||f.字段节点==引用内容需求字段;
            else if(type==新增成员见证类型)展开=f.字段节点==新增成员指定需求字段;
            else if(type==复合分解来源类型)展开=f.字段节点==复合父需求字段
                ||f.字段节点==引用内容需求字段||f.字段节点==目标新增成员见证字段;
            if(展开)pending.push_back(*ref);
        }
        if(type==需求节点类型) {
            for(const auto& f:raw.入向字段组)if(f.字段节点==列表成员字段)pending.push_back(f.所属节点);
            for(const auto& rel:raw.入向外部关系组)
                if(rel.类型==基础外部关系类型::父子&&rel.角色或顺序==需求父子关系角色)pending.push_back(rel.源节点);
            for(const auto& rel:raw.出向外部关系组)
                if(rel.类型==基础外部关系类型::父子&&rel.角色或顺序==需求父子关系角色)pending.push_back(rel.目标节点);
        }
    }
}

void 新_需求类::追加来源候选枚举固定保护(来源候选枚举保护批& 批) const {
    if(!结构已初始化()||!有效(安全根需求节点)||!有效(服务根需求节点))throw 筹办保护依赖未就绪{};
    auto nodes=需求结构固定节点组();
    nodes.push_back(安全根需求节点);nodes.push_back(服务根需求节点);
    std::sort(nodes.begin(),nodes.end());nodes.erase(std::unique(nodes.begin(),nodes.end()),nodes.end());
    const auto r=全局基础数据集.查询节点组完整结构(nodes);
    if(r.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
    要求需求物理(r.状态==基础结构操作状态::已完成&&r.节点组.size()==nodes.size());
    bool 缺失=false,曾采缺失=false;
    for(std::size_t i=0;i<nodes.size();++i) {
        const auto& p=r.节点组[i];要求需求物理(p.查询节点==nodes[i]);
        if(!p.快照) {
            缺失=true;
            曾采缺失=曾采缺失||std::any_of(批.完整保护.begin(),批.完整保护.end(),[&](const auto& old){return old.结构.节点.编码==nodes[i];});
            continue;
        }
        要求需求物理(p.快照->结构.节点.编码==nodes[i]);
        if(!并需求保护(批.完整保护,{*p.快照}))throw 筹办保护前提已变化{};
    }
    if(缺失) {
        if(曾采缺失||!需求内容保护未变(批.完整保护))throw 筹办保护前提已变化{};
        要求需求物理(false);
    }
    追加来源候选枚举原始闭包保护(需求树根节点,批);
    追加来源候选枚举原始闭包保护(安全根需求节点,批);
    追加来源候选枚举原始闭包保护(服务根需求节点,批);
}

void 新_需求类::追加来源候选枚举本域保护(稳定编码 D,来源候选枚举保护批& 批) const {
    追加来源候选枚举原始闭包保护(D,批);
    筹办保护收集批 本域;
    本域.完整保护.swap(批.完整保护);
    本域.已采集本域节点组.swap(批.已采集本域节点组);
    try {
        追加筹办本域保护(D,本域,true);
    } catch (...) {
        本域.完整保护.swap(批.完整保护);
        本域.已采集本域节点组.swap(批.已采集本域节点组);
        throw;
    }
    本域.完整保护.swap(批.完整保护);
    本域.已采集本域节点组.swap(批.已采集本域节点组);
}

新_需求类::来源准备单需求治理可停止结果 新_需求类::读取来源准备单需求治理保护(
    稳定编码 D,std::stop_token 停止) const {
    using S=来源准备治理读取状态;
    来源候选枚举保护批 批;
    批.仅当前需求=true;
    const auto 返回=[](S 状态)->来源准备单需求治理可停止结果 {
        return 来源准备单需求治理保护结果{状态,{},{}};
    };
    try {
        try {
            检查需求读取停止(停止);
            要求需求物理(有效(D));
            auto 锁=获取需求读取锁(停止);
            if(!结构已初始化())return 返回(S::尚未初始化);
            if(!有效(安全根需求节点)||!有效(服务根需求节点))throw 筹办保护依赖未就绪{};
            const auto 捕获=[&](稳定编码 n) {
                检查需求读取停止(停止);
                return 取得服务实例物理保护(n,批.完整保护);
            };
            要求需求物理(读取唯一节点字段(需求树根节点,节点类型字段)==需求树根节点类型);
            检查需求读取停止(停止);
            const auto 首读=全局基础数据集.查询节点组完整结构({D});
            if(首读.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            要求需求物理(首读.状态==基础结构操作状态::已完成&&首读.节点组.size()==1
                &&首读.节点组.front().查询节点==D);
            检查需求读取停止(停止);
            const auto& 初始=首读.节点组.front().快照;
            bool 是需求=false;
            bool 已被正式需求域引用=D==安全根需求节点||D==服务根需求节点;
            if(初始) {
                要求需求物理(初始->结构.节点.编码==D);
                if(!并需求保护(批.完整保护,{*初始}))throw 筹办保护前提已变化{};
                const auto 类型=需求物理游标{初始->结构,{}}.引用(节点类型字段,false);
                是需求=类型&&*类型==需求节点类型;
                已被正式需求域引用=已被正式需求域引用
                    ||std::any_of(初始->入向字段组.begin(),初始->入向字段组.end(),[](const auto& f) {
                        return f.字段节点==安全根登记字段||f.字段节点==服务根登记字段
                            ||f.字段节点==被认知需求登记字段||f.字段节点==列表成员字段;
                    });
                const auto 是需求父子=[](const auto& r) {
                    return r.类型==基础外部关系类型::父子&&r.角色或顺序==需求父子关系角色;
                };
                已被正式需求域引用=已被正式需求域引用
                    ||std::any_of(初始->入向外部关系组.begin(),初始->入向外部关系组.end(),是需求父子)
                    ||std::any_of(初始->出向外部关系组.begin(),初始->出向外部关系组.end(),是需求父子);
            }
            if(!是需求) {
                if(!批.完整保护.empty()&&!需求内容保护未变(批.完整保护))throw 筹办保护前提已变化{};
                要求需求物理(!已被正式需求域引用);
                检查需求读取停止(停止);
                return 来源准备单需求治理保护结果{S::需求不存在,{},std::move(批.完整保护)};
            }
            检查需求读取停止(停止);
            const auto 预采附属=[&](const 基础节点结构& raw) {
                for(const auto& f:raw.字段组) {
                    if(f.字段节点!=目标新增成员见证字段&&f.字段节点!=需求首次发布首状态字段
                        &&f.字段节点!=集合发布场景字段&&f.字段节点!=集合发布动作字段
                        &&f.字段节点!=服务需求取得执行字段)continue;
                    const auto* n=std::get_if<稳定编码>(&f.内容);
                    要求需求物理(n&&有效(*n));
                    (void)捕获(*n);
                }
            };
            预采附属(初始->结构);
            for(const auto& f:初始->结构.字段组) {
                if(f.字段节点!=复合分解来源字段&&f.字段节点!=服务取得实例来源字段)continue;
                const auto* n=std::get_if<稳定编码>(&f.内容);
                要求需求物理(n&&有效(*n));
                const auto 附属=捕获(*n);
                if(f.字段节点==复合分解来源字段)预采附属(附属.结构);
                else {
                    const auto saved=需求物理游标{附属.结构,{}}.引用(实例原父目标字段);
                    const auto 原目标=捕获(*saved);
                    预采附属(原目标.结构);
                }
            }
            auto 信息=读取需求治理信息可抛内部(D,&批);
            检查需求读取停止(停止);
            if(!信息)throw 筹办保护前提已变化{};
            要求需求物理(信息->节点==D&&!批.完整保护.empty());
            if(!需求内容保护未变(批.完整保护))throw 筹办保护前提已变化{};
            检查需求读取停止(停止);
            return 来源准备单需求治理保护结果{S::已读取,std::move(信息),std::move(批.完整保护)};
        } catch(const 需求物理读取失败& e) {
            if(e.原因==基础结构操作状态::资源失败)throw std::bad_alloc{};
            if(!批.完整保护.empty()&&!需求内容保护未变(批.完整保护))throw 筹办保护前提已变化{};
            throw;
        }
    } catch(const 新需求读取取消&) {return 新需求读取取消{};}
    catch(const 筹办保护前提已变化&) {return 返回(S::前提已变化);}
    catch(const 筹办保护依赖未就绪&) {return 返回(S::依赖未就绪);}
    catch(新需求目标定义核验状态 s) {
        if(s==新需求目标定义核验状态::前提已变化)return 返回(S::前提已变化);
        if(s==新需求目标定义核验状态::依赖未就绪)return 返回(S::依赖未就绪);
        if(s==新需求目标定义核验状态::资源失败)return 返回(S::资源失败);
        throw;
    } catch(const std::bad_alloc&) {return 返回(S::资源失败);}
    catch(const std::length_error&) {return 返回(S::资源失败);}
}

新_需求类::来源准备自我列表域可停止结果 新_需求类::读取来源准备自我列表域保护(
    std::stop_token 停止) const {
    using S=来源准备治理读取状态;
    来源候选枚举保护批 批;
    const auto 返回=[](S 状态)->来源准备自我列表域可停止结果 {
        return 来源准备自我列表域保护结果{状态,{},{},{}};
    };
    try {
        try {
            检查需求读取停止(停止);
            auto 锁=获取需求读取锁(停止);
            if(!结构已初始化())return 返回(S::尚未初始化);
            检查需求读取停止(停止);
            auto 读取=查询全部需求列表可抛内部(停止,&批);
            if(std::holds_alternative<新需求读取取消>(读取))return 新需求读取取消{};
            auto* 结果=std::get_if<新需求列表全集结果>(&读取);
            要求需求物理(结果!=nullptr);
            检查需求读取停止(停止);
            if(结果->状态==新需求操作状态::资源失败)return 返回(S::资源失败);
            if(结果->状态==新需求操作状态::前提已变化)return 返回(S::前提已变化);
            if(结果->状态==新需求操作状态::目标内容依赖未就绪)return 返回(S::依赖未就绪);
            // 已初始化但结构不存在/覆盖不一致，不降为正常空域或未初始化。
            if(!需求内容保护未变(批.完整保护))throw 筹办保护前提已变化{};
            要求需求物理(结果->状态==新需求操作状态::已找到
                &&!结果->列表项组.empty()&&!结果->需求组.empty()&&!批.完整保护.empty());
            检查需求读取停止(停止);
            return 来源准备自我列表域保护结果{S::已读取,std::move(结果->列表项组),
                std::move(结果->需求组),std::move(批.完整保护)};
        } catch(const 需求物理读取失败& e) {
            if(e.原因==基础结构操作状态::资源失败)throw std::bad_alloc{};
            if(!需求内容保护未变(批.完整保护))throw 筹办保护前提已变化{};
            throw;
        }
    } catch(const 新需求读取取消&) {return 新需求读取取消{};}
    catch(const 筹办保护前提已变化&) {return 返回(S::前提已变化);}
    catch(const 筹办保护依赖未就绪&) {return 返回(S::依赖未就绪);}
    catch(新需求目标定义核验状态 s) {
        if(s==新需求目标定义核验状态::前提已变化)return 返回(S::前提已变化);
        if(s==新需求目标定义核验状态::依赖未就绪)return 返回(S::依赖未就绪);
        if(s==新需求目标定义核验状态::资源失败)return 返回(S::资源失败);
        throw;
    } catch(const std::bad_alloc&) {return 返回(S::资源失败);}
    catch(const std::length_error&) {return 返回(S::资源失败);}
}

新服务取得目标实例结果 新_需求类::读取服务取得目标实例核心可抛内部(
    稳定编码 父取得任务,稳定编码 目标槽类型,const std::vector<稳定编码>& 正式根路径,
    std::optional<稳定编码> 限定H,新服务取得目标实例结果& 前缀) const {
    using 状态=新服务取得目标实例状态;
    using 角色状态=服务取得角色核验批::状态值;
    来源候选枚举保护批 批;
    const auto 返回=[&](状态 当前状态) {
        前缀.状态=当前状态;
        if(前缀.信息||(当前状态==状态::未找到&&!限定H))
            前缀.完整保护=std::move(批.完整保护);
        return 前缀;
    };
    try {
        try {
            std::lock_guard 锁(新需求互斥);
            if(!有效(父取得任务)||!有效(目标槽类型)||!正式根路径.empty()||(限定H&&!有效(*限定H)))
                return 返回(状态::输入不适用);
            if(!结构已初始化())return 返回(状态::依赖未就绪);
            // 固定三端点的完整入向域及全部正式raw闭包，先采保护再作严格结构核验。
            追加来源候选枚举固定保护(批);
            (void)取得服务实例物理保护(父取得任务,批.完整保护);
            const auto 槽=特征概念服务_.读取特征概念定义保护(目标槽类型);
            if(槽.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            if(槽.前提已变化)throw 筹办保护前提已变化{};
            if(槽.状态==基础结构操作状态::节点不存在)return 返回(状态::输入不适用);
            要求需求物理(槽.状态==基础结构操作状态::已完成&&槽.信息&&!槽.预期节点组.empty());
            if(槽.信息->类型根!=目标槽类型)return 返回(状态::输入不适用);
            if(!并需求保护(批.完整保护,槽.预期节点组))throw 筹办保护前提已变化{};
            (void)读取需求根登记物理();
            bool 同槽冲突=false;
            for(const auto 记录节点:读取多节点字段(需求树根节点,服务取得实例登记字段)) {
                auto 记录=解码服务取得实例记录物理(记录节点,批.完整保护);
                if(记录.键.父取得任务!=父取得任务||记录.键.目标槽类型!=目标槽类型
                    ||记录.键.正式根路径!=正式根路径)continue;
                // 不能先按H过滤；第二个同槽R无论H相同与否都违反唯一绑定。
                if(前缀.信息)同槽冲突=true;
                else 前缀.信息=std::move(记录);
            }
            if(!需求内容保护未变(批.完整保护))throw 筹办保护前提已变化{};
            要求需求物理(!同槽冲突);
            if(!前缀.信息)return 返回(状态::未找到);
            auto& 信息=*前缀.信息;
            前缀.状态=状态::已出生待补骨架;
            if(限定H&&*限定H!=信息.键.服务对象)return 返回(状态::绑定冲突);
            const auto 角色=读取服务取得角色核验批(信息.键.服务对象,信息.人类概念,信息.需求身份概念,信息.完整内容概念);
            switch(角色.状态) {
            case 角色状态::已核验:break;
            case 角色状态::输入不适用:case 角色状态::前提已变化:throw 筹办保护前提已变化{};
            case 角色状态::依赖未就绪:throw 筹办保护依赖未就绪{};
            case 角色状态::资源失败:throw std::bad_alloc{};
            default:要求需求物理(false);
            }
            要求需求物理(!角色.完整保护.empty());
            if(!并需求保护(批.完整保护,角色.完整保护))throw 筹办保护前提已变化{};
            // 沿唯一治理读取核心收集真实五F/FT依赖，不要求当前需求内容已经满足。
            const auto 父需求=读取需求治理信息可抛内部(信息.父需求,&批);
            if(!父需求||父需求->执行域!=新需求执行域::自我执行
                ||父需求->完整目标!=std::optional{信息.原父目标})throw 筹办保护前提已变化{};
            const auto 原目标=*读取唯一节点字段(信息.出生记录,实例原父目标字段);
            if(读取唯一节点字段(信息.父需求,目标新增成员见证字段)!=读取唯一节点字段(原目标,目标新增成员见证字段))
                throw 筹办保护前提已变化{};
            const auto 需求=读取需求治理信息可抛内部(信息.需求节点,&批);
            要求需求物理(需求&&需求->执行域==新需求执行域::被认知
                &&需求->需求主体存在节点==信息.键.服务对象);
            const auto 原始需求=取得服务实例物理保护(信息.需求节点,批.完整保护);
            信息.骨架就绪=是F承载需求(原始需求.结构);
            if(!需求内容保护未变(批.完整保护))throw 筹办保护前提已变化{};
            return 返回(信息.骨架就绪 ? 状态::已复用 : 状态::已出生待补骨架);
        } catch(const 需求物理读取失败& 错误) {
            if(错误.原因==基础结构操作状态::资源失败)throw std::bad_alloc{};
            if(!需求内容保护未变(批.完整保护))throw 筹办保护前提已变化{};
            throw;
        }
    } catch(const 筹办保护前提已变化&) {return 返回(状态::前提已变化);}
    catch(const 筹办保护依赖未就绪&) {return 返回(状态::依赖未就绪);}
    catch(新需求目标定义核验状态 当前状态) {
        if(当前状态==新需求目标定义核验状态::前提已变化)return 返回(状态::前提已变化);
        if(当前状态==新需求目标定义核验状态::依赖未就绪)return 返回(状态::依赖未就绪);
        if(当前状态==新需求目标定义核验状态::资源失败)return 返回(状态::资源失败);
        if(前缀.信息&&!批.完整保护.empty())前缀.完整保护=std::move(批.完整保护);
        throw;
    } catch(...) {
        if(前缀.信息&&!批.完整保护.empty())前缀.完整保护=std::move(批.完整保护);
        throw;
    }
}

新服务取得目标实例结果 新_需求类::读取服务取得目标槽实例(
    稳定编码 父取得任务,稳定编码 目标槽类型,const std::vector<稳定编码>& 正式根路径) const noexcept {
    using 状态=新服务取得目标实例状态;
    新服务取得目标实例结果 前缀;
    try {return 读取服务取得目标实例核心可抛内部(父取得任务,目标槽类型,正式根路径,std::nullopt,前缀);}
    catch(const 需求物理读取失败& 错误) {
        if(错误.原因!=基础结构操作状态::资源失败)
            工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取服务取得目标槽实例",L"正式槽实例或需求结构损坏。");
        前缀.状态=状态::资源失败;
    } catch(const std::bad_alloc&) {前缀.状态=状态::资源失败;}
    catch(const std::length_error&) {前缀.状态=状态::资源失败;}
    catch(...) {工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取服务取得目标槽实例",L"实例正式读取工程故障；保留已出生事实。");}
    if(!前缀.信息)前缀.完整保护.clear();
    return 前缀;
}

新来源前驱候选枚举结果 新_需求类::读取服务取得主体被认知需求候选保护(稳定编码 H) const noexcept {
    using S=新来源前驱候选枚举状态;
    来源候选枚举保护批 批;
    try {
        try {
            if(!有效(H))return {S::输入不适用,{},{},{},{}};
            const auto identity=存在服务_.读取存在身份保护(H);
            if(identity.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            if(identity.前提已变化)throw 筹办保护前提已变化{};
            if(identity.状态==基础结构操作状态::节点不存在)return {S::输入不适用,{},{},{},{}};
            if(identity.状态==基础结构操作状态::内部不一致) {
                // 非存在对象不适用；已具有正式存在类型而身份损坏不能当普通未找到。
                要求需求物理(!存在服务_.是存在节点(H));
                return {S::输入不适用,{},{},{},{}};
            }
            要求需求物理(identity.状态==基础结构操作状态::已完成&&identity.信息
                &&identity.信息->存在节点==H&&!identity.预期节点组.empty());
            批.完整保护=identity.预期节点组;
            std::lock_guard 锁(新需求互斥);
            std::vector<新需求治理信息> 完整治理;
            auto records=查询全部需求记录可抛内部(&批,&完整治理);
            if(records.状态==新需求操作状态::尚未初始化)throw 筹办保护依赖未就绪{};
            if(records.状态==新需求操作状态::前提已变化)throw 筹办保护前提已变化{};
            if(records.状态==新需求操作状态::资源失败)throw std::bad_alloc{};
            要求需求物理(records.状态==新需求操作状态::已找到);
            std::erase_if(完整治理,[&](const auto& d){return d.需求主体存在节点!=H||d.执行域!=新需求执行域::被认知;});
            std::erase_if(records.待完善需求组,[&](const auto& d){return d.需求主体存在节点!=H||d.执行域!=新需求执行域::被认知;});
            std::set<稳定编码> 唯一D;
            for(const auto& d:完整治理)要求需求物理(有效(d.节点)&&唯一D.insert(d.节点).second
                &&d.完整目标.has_value()&&d.信息完善阶段!=新需求信息完善阶段::待完善);
            for(const auto& d:records.待完善需求组)要求需求物理(有效(d.节点)&&唯一D.insert(d.节点).second
                &&d.信息完善阶段==新需求信息完善阶段::待完善);
            要求需求物理(!批.完整保护.empty());
            if(!需求内容保护未变(批.完整保护))throw 筹办保护前提已变化{};
            return {S::已读取,H,std::move(完整治理),std::move(records.待完善需求组),std::move(批.完整保护)};
        } catch(const 需求物理读取失败& e) {
            if(e.原因==基础结构操作状态::资源失败)throw std::bad_alloc{};
            if(!需求内容保护未变(批.完整保护))throw 筹办保护前提已变化{};
            throw;
        }
    } catch(const 筹办保护前提已变化&) {return {S::前提已变化,{},{},{},{}};}
    catch(const 筹办保护依赖未就绪&) {return {S::依赖未就绪,{},{},{},{}};}
    catch(新需求目标定义核验状态 s) {
        if(s==新需求目标定义核验状态::前提已变化)return {S::前提已变化,{},{},{},{}};
        if(s==新需求目标定义核验状态::依赖未就绪)return {S::依赖未就绪,{},{},{},{}};
        if(s==新需求目标定义核验状态::资源失败)return {S::资源失败,{},{},{},{}};
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取服务取得主体被认知需求候选保护", L"候选读取收到未约定的目标状态");
    } catch(const std::bad_alloc&) {return {S::资源失败,{},{},{},{}};}
    catch(const std::length_error&) {return {S::资源失败,{},{},{},{}};}
    catch (...) {工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取服务取得主体被认知需求候选保护", L"需求候选完整域读取工程故障");}
}

void 新_需求类::追加筹办存在值域归属保护(稳定编码 E,稳定编码 C,筹办保护收集批& 批) const {
    const auto identity=存在服务_.读取存在身份保护(E);
    if(identity.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
    if(identity.前提已变化||identity.状态==基础结构操作状态::节点不存在)批.变化();
    要求需求物理(identity.状态==基础结构操作状态::已完成&&identity.信息&&identity.信息->存在节点==E
        &&!identity.预期节点组.empty());
    批.合并(identity.预期节点组);
    const auto ordinary=[&](稳定编码 n) {
        const auto r=存在概念服务_.读取普通存在概念定义保护(n);
        if(r.状态==正式概念读取状态::依赖未就绪)批.未就绪();
        if(r.状态==正式概念读取状态::前提已变化)批.变化();
        要求需求物理(r.状态==正式概念读取状态::已读取&&r.信息&&r.信息->节点==n&&!r.完整保护组.empty());
        批.合并(r.完整保护组);return *r.信息;
    };
    (void)ordinary(C);
    // CONCEPT-INFO-MEMBERSHIP：保护唯一归属算法实际读取的材料（实例关系或定义匹配，含递归）。
    const auto 判定=存在服务_.比较存在与概念信息(E,C,true);
    if(判定.状态==特征概念规则状态::资源失败)throw std::bad_alloc{};
    if(判定.状态==特征概念规则状态::前提已变化)批.变化();
    else {
        要求需求物理(判定.状态==特征概念规则状态::已完成);
        批.合并(判定.依据保护);
    }
    批.核同();
}

新行动请求筹办资格保护结果 新_需求类::读取行动请求自我需求筹办资格保护(
    const 新行动请求承接来源& sourceKey,稳定编码 Ds) const noexcept {
    using S=新行动请求筹办资格读取状态;
    using R=新行动请求来源读取状态;
    新行动请求筹办资格保护结果 out;out.需求节点=Ds;
    try {
        if(!有效(Ds)){out.状态=S::输入不适用;return out;}
        // 先锁外取得正式来源和唯一同源映射，不依赖T是否已建立。
        const auto source=读取行动请求承接来源保护(sourceKey);
        switch(source.状态) {
        case R::输入不适用:out.状态=S::输入不适用;return out;
        case R::前提已变化:out.状态=S::前提已变化;return out;
        case R::依赖未就绪:out.状态=S::依赖未就绪;return out;
        case R::资源失败:out.状态=S::资源失败;return out;
        case R::已读取:break;
        default:要求需求物理(false);
        }
        要求需求物理(source.信息&&source.信息->来源==sourceKey&&!source.完整保护.empty());
        const auto mapping=查询行动请求自我需求(sourceKey);
        if(mapping.状态==新需求操作状态::资源失败)throw std::bad_alloc{};
        if(mapping.状态==新需求操作状态::前提已变化)throw 筹办保护前提已变化{};
        if(mapping.状态==新需求操作状态::尚未初始化)throw 筹办保护依赖未就绪{};
        if(mapping.状态==新需求操作状态::认知来源不合法)throw 筹办保护前提已变化{};
        if(!需求内容保护未变(source.完整保护))throw 筹办保护前提已变化{};
        if(mapping.状态==新需求操作状态::需求不存在){out.状态=S::输入不适用;return out;}
        要求需求物理(mapping.状态==新需求操作状态::已找到&&mapping.信息&&!mapping.完整保护.empty());
        if(mapping.信息->节点!=Ds){out.状态=S::输入不适用;return out;}
        筹办保护收集批 批;
        批.合并(source.完整保护);批.合并(mapping.完整保护);
        const auto baseline=*mapping.信息;
        要求需求物理(baseline.行动请求来源==std::optional{sourceKey}&&baseline.完整目标==std::optional{source.信息->目标}
            &&baseline.需求主体存在节点==sourceKey.受请求者节点&&baseline.执行域==新需求执行域::自我执行
            &&baseline.父需求节点==服务要求结算需求_&&baseline.来源类型==新需求来源类型::行动请求);
        新需求筹办资格结果 qualification;
        {
            std::unique_lock lock(新需求互斥);
            追加筹办本域保护(Ds,批);
            const auto current=读取需求治理信息可抛内部(Ds);
            if(current!=mapping.信息)批.变化();
        }
        // 核心独占自己的需求锁层级，使特殊目标的unlock真正释放最外需求锁。
        qualification=复核筹办资格可抛内部(Ds,&批);
        // 特殊目标在原核心中按原锁序释放/重取；末尾仍核同最初来源、映射和全部事实。
        const auto fresh=查询行动请求自我需求(sourceKey);
        if(fresh.状态==新需求操作状态::资源失败)throw std::bad_alloc{};
        if(fresh.状态==新需求操作状态::尚未初始化)批.未就绪();
        if(fresh.状态==新需求操作状态::前提已变化||fresh.状态==新需求操作状态::认知来源不合法
            ||fresh.状态==新需求操作状态::需求不存在)批.变化();
        // 同源冲突若来自读取期间的目标/映射变化，先让真实旧保护识别变化。
        批.核同();
        要求需求物理(fresh.状态==新需求操作状态::已找到&&fresh.信息&&!fresh.完整保护.empty());
        if(fresh.信息!=mapping.信息)批.变化();
        批.合并(fresh.完整保护);
        {
            std::lock_guard lock(新需求互斥);
            const auto current=读取需求治理信息可抛内部(Ds);
            if(current!=mapping.信息)批.变化();
        }
        批.核同();
        if(qualification.状态==新需求筹办资格状态::资源失败)throw std::bad_alloc{};
        if(qualification.状态==新需求筹办资格状态::材料已变化)批.变化();
        要求需求物理(qualification.状态!=新需求筹办资格状态::结构不一致
            &&qualification.状态!=新需求筹办资格状态::目标定义不合法
            &&qualification.状态!=新需求筹办资格状态::需求不存在);
        if(qualification.需求)要求需求物理(qualification.需求==std::optional{投影完整需求(baseline)});
        要求需求物理(!批.完整保护.empty());
        out.状态=S::已读取;out.资格=std::move(qualification);out.完整保护=std::move(批.完整保护);return out;
    } catch(const 筹办保护前提已变化&){out.状态=S::前提已变化;}
    catch(const 筹办保护依赖未就绪&){out.状态=S::依赖未就绪;}
    catch(新需求目标定义核验状态 s) {
        if(s!=新需求目标定义核验状态::前提已变化)
            工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取行动请求筹办资格",L"未知目标保护状态。");
        out.状态=S::前提已变化;
    }
    catch(const 需求物理读取失败& e) {
        if(e.原因!=基础结构操作状态::资源失败)
            工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取行动请求筹办资格",L"正式同源需求或本域结构损坏。");
        out.状态=S::资源失败;
    }
    catch(const std::bad_alloc&){out.状态=S::资源失败;}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取行动请求筹办资格",L"受保护资格读取工程故障；不生成资格或执行许可。");}
    out.资格.reset();out.完整保护.clear();return out;
}

新需求筹办资格结果 新_需求类::复核筹办资格(稳定编码 需求节点) const noexcept {
    try {return 复核筹办资格可抛内部(需求节点,nullptr);}
    catch(const std::bad_alloc&){新需求筹办资格结果 r;r.状态=新需求筹办资格状态::资源失败;return r;}
    catch(...){工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"复核需求筹办资格",L"正式目标资格读取故障");}
}

新需求筹办资格结果 新_需求类::复核筹办资格可抛内部(稳定编码 需求节点,筹办保护收集批* 批,
    提议源收集批* 源,std::vector<基础联合节点快照>* 提议依赖) const {
    using S = 新需求筹办资格状态;
    新需求筹办资格结果 r;
    const auto 当前完整=[&]() -> std::optional<新需求信息> {
        const auto d=读取需求治理信息可抛内部(需求节点);
        return d&&d->完整目标&&(d->执行域!=新需求执行域::被认知||d->信息完善阶段==新需求信息完善阶段::已完善)
            ? std::optional{投影完整需求(*d)} : std::nullopt;
    };
        std::unique_lock 锁(新需求互斥);
        if(批)追加筹办本域保护(需求节点,*批);
        const auto 治理 = 读取需求治理信息可抛内部(需求节点);
        if (!治理) { r.状态 = S::需求不存在; return r; }
        if (!治理->完整目标||治理->信息完善阶段==新需求信息完善阶段::待完善) { r.状态 = S::需求待完善; return r; }
        r.需求 = 投影完整需求(*治理);
        const auto& d = *r.需求;
        if (d.执行域 != 新需求执行域::自我执行) return r;
        if (d.活动状态 != 新需求活动状态::活动) { r.状态 = S::需求非活动; return r; }
        if(d.目标.新增成员来源见证) {
            锁.unlock();
            auto def=核验新增成员目标定义保护({d.目标存在节点,d.目标});
            if(批) {
                if(def.状态==新需求目标定义核验状态::资源失败)throw std::bad_alloc{};
                if(def.状态==新需求目标定义核验状态::前提已变化)批->变化();
                if(def.状态==新需求目标定义核验状态::依赖未就绪)批->未就绪();
                批->合并(def.完整保护);
            }
            if(def.状态==新需求目标定义核验状态::资源失败){r.状态=S::资源失败;return r;}
            if(def.状态==新需求目标定义核验状态::前提已变化){r.状态=S::材料已变化;return r;}
            if(def.状态!=新需求目标定义核验状态::已完成)return r;
            const auto derived=二次特征概念服务_.读取二次特征定义保护(d.目标.目标特征类型概念节点);
            if(derived.状态==基础结构操作状态::资源失败){r.状态=S::资源失败;return r;}
            if(derived.前提已变化){r.状态=S::材料已变化;return r;}
            要求需求物理(derived.状态==基础结构操作状态::已完成&&derived.信息);
            if(批)批->合并(derived.预期节点组);
            if(!并需求保护(def.完整保护,derived.预期节点组)){r.状态=S::材料已变化;return r;}
            r.派生定义=derived.信息;
            const auto evidence=读取新增成员来源见证内部(需求节点,源);
            if(批) {
                if(evidence.状态==新需求新增成员读取状态::资源失败)throw std::bad_alloc{};
                if(evidence.状态==新需求新增成员读取状态::前提已变化)批->变化();
                if(evidence.状态==新需求新增成员读取状态::依赖未就绪)批->未就绪();
                批->合并(evidence.完整保护);
            }
            r.当前满足=映射新增成员目标满足(evidence);
            switch(r.当前满足->状态) {
            case 新需求当前满足状态::已满足:r.状态=S::目标已满足;break;
            case 新需求当前满足状态::需增加:
            case 新需求当前满足状态::未满足但无有向差值:r.状态=S::允许筹办;break;
            case 新需求当前满足状态::目标引用前提已变化:r.状态=S::材料已变化;break;
            case 新需求当前满足状态::目标内容依赖未就绪:r.状态=S::等待普通材料;break;
            case 新需求当前满足状态::资源失败:r.状态=S::资源失败;break;
            default:r.状态=S::目标定义不合法;break;
            }
            锁.lock();
            if(!提议需求值相同(当前完整(),r.需求,源)||!需求内容保护未变(def.完整保护)
                ||(evidence.状态==新需求新增成员读取状态::已读取&&!需求内容保护未变(evidence.完整保护)))r.状态=S::材料已变化;
            return r;
        }
        if(d.目标.引用内容定位) {
            锁.unlock();
            r.当前满足=复核当前满足可抛内部(需求节点,批,源,提议依赖);
            switch(r.当前满足->状态) {
            case 新需求当前满足状态::已满足:r.状态=S::目标已满足;break;
            case 新需求当前满足状态::未满足但无有向差值:
            case 新需求当前满足状态::目标内容分支待定:r.状态=S::允许筹办;break;
            case 新需求当前满足状态::目标内容未支持:
            case 新需求当前满足状态::目标内容依赖未就绪:r.状态=S::等待普通材料;break;
            case 新需求当前满足状态::目标引用前提已变化:r.状态=S::材料已变化;break;
            case 新需求当前满足状态::目标内容定义不合法:r.状态=S::目标定义不合法;break;
            case 新需求当前满足状态::资源失败:r.状态=S::资源失败;break;
            default:工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"引用内容筹办资格",L"内容目标满足状态不在正式映射内");
            }
            if(!提议需求值相同(当前完整(),r.需求,源))r.状态=S::材料已变化;
            return r;
        }
        if(批) {
            const auto e=存在服务_.读取存在身份保护(d.目标存在节点);
            if(e.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            if(e.前提已变化||e.状态==基础结构操作状态::节点不存在)批->变化();
            要求需求物理(e.状态==基础结构操作状态::已完成&&e.信息&&e.信息->存在节点==d.目标存在节点&&!e.预期节点组.empty());
            批->合并(e.预期节点组);
        }
        if ((!批&&!存在服务_.是存在节点(d.目标存在节点)) || !目标形状合法(d.目标)) return r;
        if(批) {
            const auto def=核验需求目标定义保护(d.目标,特征概念服务_,二次特征概念服务_);
            if(def.状态==新需求目标定义核验状态::资源失败)throw std::bad_alloc{};
            if(def.状态==新需求目标定义核验状态::前提已变化)批->变化();
            if(def.状态==新需求目标定义核验状态::依赖未就绪)批->未就绪();
            批->合并(def.完整保护);
            if(def.状态!=新需求目标定义核验状态::已完成)return r;
        }
        const auto 根 = 批 ? std::optional{批->特征定义(特征概念服务_,d.目标.目标特征类型概念节点).类型根}
            : 取得特征类型根(d.目标.目标特征类型概念节点,特征概念服务_);
        if (!根 || *根 != d.目标.目标特征类型概念节点) return r;
        基础节点组追加请求 保护;
        const auto 加身份 = [&](稳定编码 n, const 正式概念身份读取接口& owner) {
            const auto p = owner.读取正式概念身份保护(n);
            if(批&&p.状态==正式概念读取状态::依赖未就绪)批->未就绪();
            if (p.状态 == 正式概念读取状态::前提已变化) { r.状态 = S::材料已变化; return false; }
            if (p.状态 != 正式概念读取状态::已读取 || !p.身份) { r.状态 = S::结构不一致; return false; }
            if (!加入需求完整保护(保护, p.身份->完整保护组)) { r.状态 = S::材料已变化; return false; }
            if(批)批->合并(p.身份->完整保护组);
            return true;
        };
        if (!加身份(*根, 特征概念服务_)) return r;
        if (d.目标.目标形式 == 新需求目标形式::值) {
            const auto 域根 = 批 ? std::optional{批->特征定义(特征概念服务_,*d.目标.目标值域概念节点).类型根}
                : 取得特征类型根(*d.目标.目标值域概念节点,特征概念服务_);
            const auto 比较 = 批 ? std::optional{批->二次定义(二次特征概念服务_,特征概念服务_,*d.目标.目标比较关系概念节点)}
                : 二次特征概念服务_.获取二次特征概念(*d.目标.目标比较关系概念节点);
            if (!域根 || *域根 != *根 || !比较 || !(批
                ? 批->比较适用(*比较,*根,*d.目标.目标值域概念节点,特征概念服务_)
                : 比较关系适用于目标(*比较,*根,*d.目标.目标值域概念节点,特征概念服务_))) return r;
            if (!加身份(*d.目标.目标值域概念节点, 特征概念服务_)
                || !加身份(*d.目标.目标比较关系概念节点, 二次特征概念服务_)) return r;
        }
        const auto 身份 = 二次特征概念服务_.读取正式概念身份保护(*根);
        if(批&&身份.状态==正式概念读取状态::依赖未就绪)批->未就绪();
        if (身份.状态 == 正式概念读取状态::前提已变化) { r.状态 = S::材料已变化; return r; }
        if (身份.状态 == 正式概念读取状态::依赖未就绪) { r.状态 = S::结构不一致; return r; }
        if (身份.状态 == 正式概念读取状态::已读取) {
            if(批)(void)批->二次定义(二次特征概念服务_,特征概念服务_,*根);
            const auto p = 二次特征概念服务_.读取二次特征定义保护(*根);
            if (p.状态 == 基础结构操作状态::资源失败) { r.状态 = S::资源失败; return r; }
            if (p.前提已变化) { r.状态 = S::材料已变化; return r; }
            if (p.状态 != 基础结构操作状态::已完成 || !p.信息 || !身份.身份) {
                r.状态 = S::结构不一致; return r;
            }
            if (!加入需求完整保护(保护, 身份.身份->完整保护组)
                || !加入需求完整保护(保护, p.预期节点组)) { r.状态 = S::材料已变化; return r; }
            if(批){批->合并(身份.身份->完整保护组);批->合并(p.预期节点组);}
            if (p.信息->二次特征类型根节点 != *根 || p.信息->输入定义组.empty()) return r;
            std::int64_t 序 = 0;
            for (const auto& in : p.信息->输入定义组) {
                if (in.顺序 != 序++) return r;
                if (in.特征类型概念节点 && !加身份(*in.特征类型概念节点, 特征概念服务_)) return r;
            }
            r.派生定义 = p.信息;
        }
        r.当前满足 = 复核当前满足可抛内部(需求节点,批,源,提议依赖);
        switch (r.当前满足->状态) {
        case 新需求当前满足状态::已满足: r.状态 = S::目标已满足; break;
        case 新需求当前满足状态::需增加:
        case 新需求当前满足状态::需减少:
        case 新需求当前满足状态::未满足但无有向差值:
        case 新需求当前满足状态::需要执行前后比较: r.状态 = S::允许筹办; break;
        // CONCEPT-INFO-MEMBERSHIP R-1：认知不足不结束、不等待，允许筹办以按目标分解形成认知子需求。
        case 新需求当前满足状态::当前值认知不足: r.状态 = S::允许筹办; break;
        case 新需求当前满足状态::目标特征尚未形成:
        case 新需求当前满足状态::当前值不存在:
            r.状态 = !d.根角色 && r.派生定义 && d.目标.目标形式 == 新需求目标形式::值
                ? S::允许筹办 : S::等待普通材料; break;
        case 新需求当前满足状态::资源失败: r.状态 = S::资源失败; break;
        case 新需求当前满足状态::需求信息待完善: r.状态 = S::需求待完善; break;
        case 新需求当前满足状态::结构不一致: r.状态 = S::结构不一致; break;
        default: r.状态 = S::目标定义不合法; break;
        }
        if (!提议需求值相同(当前完整(),r.需求,源) || (!批&&!存在服务_.是存在节点(d.目标存在节点))) {
            r.状态 = S::材料已变化; return r;
        }
        std::sort(保护.预期联合节点组.begin(), 保护.预期联合节点组.end(),
            [](const auto& a, const auto& b) { return a.结构.节点.编码 < b.结构.节点.编码; });
        std::vector<稳定编码> ids;
        for (const auto& s : 保护.预期联合节点组) ids.push_back(s.结构.节点.编码);
        const auto fresh = 全局基础数据集.查询节点组完整结构(ids);
        if (fresh.状态 == 基础结构操作状态::资源失败) { r.状态 = S::资源失败; return r; }
        要求需求物理(fresh.状态 == 基础结构操作状态::已完成 && fresh.节点组.size() == ids.size());
        for (std::size_t i=0; i<ids.size(); ++i)
            if (fresh.节点组[i].查询节点 != ids[i] || !fresh.节点组[i].快照
                || !需求完整快照相同(保护.预期联合节点组[i], *fresh.节点组[i].快照)) {
                r.状态 = S::材料已变化; return r;
            }
        return r;
}

新需求当前满足结果 新_需求类::复核当前满足(
    稳定编码 需求节点) const noexcept {
    try {return 复核当前满足可抛内部(需求节点,nullptr);}
    catch(...){return {新需求当前满足状态::资源失败,{},{}};}
}

新需求当前满足结果 新_需求类::复核当前满足可抛内部(
    稳定编码 需求节点,筹办保护收集批* 批,提议源收集批* 源,
    std::vector<基础联合节点快照>* 提议依赖) const {
    新需求当前满足结果 结果;
        std::unique_lock 锁(新需求互斥);
        if(批)追加筹办本域保护(需求节点,*批);
        const auto 治理 = 读取需求治理信息可抛内部(需求节点);
        if (治理 && (!治理->完整目标||治理->信息完善阶段==新需求信息完善阶段::待完善)) {
            结果.状态 = 新需求当前满足状态::需求信息待完善;
            return 结果;
        }
        const auto 需求 = 治理 ? std::optional{投影完整需求(*治理)} : std::nullopt;
        if(需求&&需求->目标.新增成员来源见证) {
            锁.unlock();
            const auto evidence=读取新增成员来源见证内部(需求节点,源);
            if(批) {
                if(evidence.状态==新需求新增成员读取状态::资源失败)throw std::bad_alloc{};
                if(evidence.状态==新需求新增成员读取状态::前提已变化)批->变化();
                if(evidence.状态==新需求新增成员读取状态::依赖未就绪)批->未就绪();
                批->合并(evidence.完整保护);
            }
            return 映射新增成员目标满足(evidence);
        }
        if(需求&&需求->目标.引用内容定位) {
            锁.unlock();
            引用内容提议读取 内容;
            if(源) {
                内容=读取引用内容提议(需求节点);
                if(内容.需求) {
                    合并提议源前提(源->前提,内容.源前提);源->需求节点组.insert(需求节点);
                }
                if(提议依赖&&!并需求保护(*提议依赖,内容.判定.完整保护组))
                    return {新需求当前满足状态::目标引用前提已变化,{},{}};
            }
            const auto 判定=源 ? std::move(内容.判定) : 复核引用内容目标(需求节点);
            using S=新引用内容目标状态;
            using R=新需求当前满足状态;
            if(批) {
                if(判定.状态==S::资源失败)throw std::bad_alloc{};
                if(判定.状态==S::引用前提已变化)批->变化();
                if(判定.状态==S::依赖未就绪)批->未就绪();
                批->合并(判定.完整保护组);
            }
            R 状态=R::目标内容未支持;
            switch(判定.状态) {
            case S::已满足:状态=R::已满足;break;
            case S::未满足:状态=R::未满足但无有向差值;break;
            case S::分支待定:状态=R::目标内容分支待定;break;
            case S::未支持:状态=R::目标内容未支持;break;
            case S::依赖未就绪:状态=R::目标内容依赖未就绪;break;
            case S::引用前提已变化:状态=R::目标引用前提已变化;break;
            case S::定义不合法:状态=R::目标内容定义不合法;break;
            case S::资源失败:状态=R::资源失败;break;
            }
            return {状态,判定.实际特征节点,{}};
        }
        if (!需求) {
            结果.状态 = 结构仍存在()
                ? 新需求当前满足状态::需求不存在
                : 新需求当前满足状态::结构不一致;
            return 结果;
        }

        std::optional<稳定编码> 实际特征;
        std::optional<新特征信息> 已保护实际特征;
        if(批) {
            const auto identity=存在服务_.读取存在身份保护(需求->目标存在节点);
            if(identity.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            if(identity.前提已变化||identity.状态==基础结构操作状态::节点不存在)批->变化();
            要求需求物理(identity.状态==基础结构操作状态::已完成&&identity.信息
                &&identity.信息->存在节点==需求->目标存在节点&&!identity.预期节点组.empty());
            批->合并(identity.预期节点组);
            const auto specific=存在概念服务_.读取普通存在概念定义保护(identity.信息->专属存在概念节点);
            if(specific.状态==正式概念读取状态::依赖未就绪)批->未就绪();
            if(specific.状态==正式概念读取状态::前提已变化)批->变化();
            要求需求物理(specific.状态==正式概念读取状态::已读取&&specific.信息
                &&specific.信息->节点==identity.信息->专属存在概念节点
                &&specific.信息->种类==存在概念种类::专属&&specific.信息->对应存在节点==需求->目标存在节点);
            批->合并(specific.完整保护组);
        }
        // GetE原有合法特征/无父校验保留；此处不改存在owner的内部读取范围。
        const auto 目标存在 = 存在服务_.获取存在(需求->目标存在节点);
        if (!目标存在) {
            结果.状态 = 新需求当前满足状态::结构不一致;
            return 结果;
        }
        if(批) {
            if(目标存在->父存在节点) {
                const auto parent=存在服务_.读取存在身份保护(*目标存在->父存在节点);
                if(parent.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
                if(parent.前提已变化||parent.状态==基础结构操作状态::节点不存在)批->变化();
                要求需求物理(parent.状态==基础结构操作状态::已完成&&parent.信息
                    &&parent.信息->存在节点==*目标存在->父存在节点);
                批->合并(parent.预期节点组);
                if(存在服务_.获取存在(需求->目标存在节点)!=目标存在)批->变化();
            }
            std::set<稳定编码> seen;
            std::vector<新特征头信息> heads;
            for(const auto F:目标存在->特征节点组) {
                要求需求物理(有效(F)&&seen.insert(F).second);
                const auto head=特征服务_.读取特征头保护(F);
                if(head.状态==新特征头保护状态::资源失败)throw std::bad_alloc{};
                if(head.状态==新特征头保护状态::前提已变化)批->变化();
                if(head.状态==新特征头保护状态::依赖未就绪)批->未就绪();
                if(head.状态==新特征头保护状态::特征不存在) {
                    // E确已变化才是变化；稳定E仍引用缺失F是结构损坏，不伪无F。
                    const auto fresh=存在服务_.获取存在(需求->目标存在节点);
                    if(fresh!=目标存在)批->变化();
                    批->核同();要求需求物理(false);
                }
                要求需求物理(head.状态==新特征头保护状态::已读取&&head.信息&&!head.完整保护.empty()
                    &&head.信息->特征节点==F&&有效(head.信息->特征概念节点)&&有效(head.信息->特征类型根)
                    &&!head.信息->上级特征节点);
                批->合并(head.完整保护);heads.push_back(*head.信息);
            }
            // 本定位批只按完整头计数；未命中F不调用完整值保护reader。
            std::optional<新特征头信息> match;
            std::size_t count=0;
            for(const auto& h:heads)if(h.特征类型根==需求->目标.目标特征类型概念节点){++count;match=h;}
            const auto freshIdentity=存在服务_.读取存在身份保护(需求->目标存在节点);
            if(freshIdentity.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
            if(freshIdentity.前提已变化||freshIdentity.状态==基础结构操作状态::节点不存在)批->变化();
            要求需求物理(freshIdentity.状态==基础结构操作状态::已完成&&freshIdentity.信息
                &&freshIdentity.信息->存在节点==目标存在->节点
                &&freshIdentity.信息->专属存在概念节点==目标存在->专属存在概念节点);
            批->合并(freshIdentity.预期节点组);
            if(存在服务_.获取存在(需求->目标存在节点)!=目标存在)批->变化();
            批->核同();
            要求需求物理(count<=1);
            if(match) {
                实际特征=match->特征节点;
                const auto value=特征服务_.读取特征完整保护(*实际特征);
                if(value.状态==新特征完整读取状态::资源失败)throw std::bad_alloc{};
                if(value.状态==新特征完整读取状态::前提已变化||value.状态==新特征完整读取状态::特征不存在)批->变化();
                if(value.状态==新特征完整读取状态::依赖未就绪||value.状态==新特征完整读取状态::类型未支持)批->未就绪();
                要求需求物理(value.状态==新特征完整读取状态::已读取&&value.信息
                    &&value.信息->特征.节点==match->特征节点&&value.信息->特征.特征概念节点==match->特征概念节点
                    &&value.信息->特征类型根==match->特征类型根
                    &&value.信息->特征.上级特征节点==match->上级特征节点&&!value.信息->完整保护组.empty());
                批->合并(value.信息->完整保护组);已保护实际特征=value.信息->特征;
            }
        } else for (const auto 候选 : 目标存在->特征节点组) {
            const auto 特征 = 特征服务_.获取特征(候选);
            if (!特征) {
                结果.状态 = 新需求当前满足状态::结构不一致;
                return 结果;
            }
            const auto 类型根 = 取得特征类型根(
                特征->特征概念节点, 特征概念服务_);
            if (类型根 && *类型根 == 需求->目标.目标特征类型概念节点) {
                if (实际特征) {
                    结果.状态 = 新需求当前满足状态::结构不一致;
                    return 结果;
                }
                实际特征 = 候选;
            }
        }
        if (!实际特征) {
            结果.状态 = 新需求当前满足状态::目标特征尚未形成;
            return 结果;
        }
        结果.实际特征节点 = *实际特征;

        const auto 特征 = 批 ? 已保护实际特征 : 特征服务_.获取特征(*实际特征);
        if (!特征) {
            结果.状态 = 新需求当前满足状态::结构不一致;
            return 结果;
        }
        if (!特征->当前值) {
            结果.状态 = 新需求当前满足状态::当前值不存在;
            return 结果;
        }
        if (需求->目标.目标形式 == 新需求目标形式::变化方向) {
            结果.状态 = 需求->根角色 ? 新需求当前满足状态::结构不一致 : 新需求当前满足状态::需要执行前后比较;
            return 结果;
        }
        if (!需求->目标.目标值域概念节点
            || !需求->目标.目标比较关系概念节点) {
            结果.状态 = 新需求当前满足状态::结构不一致;
            return 结果;
        }
        const auto 目标概念 = 批
            ? std::optional{批->特征定义(特征概念服务_,*需求->目标.目标值域概念节点).定义}
            : 特征概念服务_.获取特征概念(*需求->目标.目标值域概念节点);
        const auto 比较概念 = 批
            ? std::optional{批->二次定义(二次特征概念服务_,特征概念服务_,*需求->目标.目标比较关系概念节点)}
            : 二次特征概念服务_.获取二次特征概念(*需求->目标.目标比较关系概念节点);
        if (!目标概念 || !比较概念) {
            结果.状态 = 新需求当前满足状态::结构不一致;
            return 结果;
        }
        const auto 实际根 = 批
            ? std::optional{批->特征定义(特征概念服务_,特征->特征概念节点).类型根}
            : 取得特征类型根(特征->特征概念节点,特征概念服务_);
        const auto 目标根 = 批
            ? std::optional{批->特征定义(特征概念服务_,*需求->目标.目标值域概念节点).类型根}
            : 取得特征类型根(*需求->目标.目标值域概念节点,特征概念服务_);
        if (!实际根 || !目标根 || *实际根 != *目标根
            || *目标根 != 需求->目标.目标特征类型概念节点) {
            结果.状态 = 新需求当前满足状态::结构不一致;
            return 结果;
        }
        if (!(批 ? 批->比较适用(*比较概念,*目标根,*需求->目标.目标值域概念节点,特征概念服务_)
            : 比较关系适用于目标(*比较概念,*目标根,*需求->目标.目标值域概念节点,特征概念服务_))) {
            结果.状态 = 新需求当前满足状态::比较关系不适用;
            return 结果;
        }

        if(需求->根角色) {
            constexpr auto 最大值=(std::numeric_limits<std::int64_t>::max)();
            const auto* 区间组=std::get_if<std::vector<特征概念I64闭区间>>(&目标概念->值域);
            const auto* 当前值=std::get_if<std::int64_t>(&*特征->当前值);
            const std::vector<二次特征概念输入定义> 必要输入{
                {0,二次特征输入角色::左输入,二次特征输入来源种类::特征准确值,*目标根},
                {1,二次特征输入角色::右输入,二次特征输入来源种类::特征准确值,*目标根}};
            if(目标概念->材料类型!=特征概念材料物理类型::I64标量||!区间组||区间组->size()!=1
                ||区间组->front().下界!=最大值||区间组->front().上界!=最大值
                ||比较概念->算法!=二次特征算法::有向特征差值||比较概念->输入定义组!=必要输入
                ||比较概念->结果域种类组.empty()
                ||!std::all_of(比较概念->结果域种类组.begin(),比较概念->结果域种类组.end(),[](auto 关系){
                    return 关系==二次特征结果域种类::等于||关系==二次特征结果域种类::包含于参照值域;
                })||!当前值||(*需求->根角色==新需求根角色::安全&&*当前值<0)) {
                结果.状态=新需求当前满足状态::结构不一致;return 结果;
            }
            if(*当前值<0) {
                wchar_t 原因[256]{};
                (void)swprintf_s(原因,L"根准确差M-current超出I64；D=%llu，F=%llu，根角色=%lld，current=%lld。负服务值合法，不饱和、不回绕、不写入。",
                    static_cast<unsigned long long>(需求节点.值),static_cast<unsigned long long>(实际特征->值),
                    static_cast<long long>(*需求->根角色),static_cast<long long>(*当前值));
                工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"复核根需求当前满足",原因);
            }
            结果.准确差异=最大值-*当前值;
            结果.状态=*当前值==最大值 ? 新需求当前满足状态::已满足 : 新需求当前满足状态::需增加;
            return 结果;
        }

        if (const auto* 集合目标 = std::get_if<特征概念任务集合值域>(&目标概念->值域)) {
            // 原集合比较尚不提供完整任务/需求/确认映射保护；旧口仍照常计算。
            if(批)批->未就绪();
            if (目标概念->单位 || 特征->实例单位) {
                结果.状态 = 新需求当前满足状态::结构不一致;
                return 结果;
            }
            // 1/0只为本次包含判定，不是材料相似度的10000/0载荷，更不导出增减方向。
            if (比较概念->算法 != 二次特征算法::特征材料相似度
                || 比较概念->结果域种类组.empty()
                || !std::all_of(比较概念->结果域种类组.begin(), 比较概念->结果域种类组.end(),
                    [](auto 关系) { return 关系 == 二次特征结果域种类::包含于参照值域; })) {
                结果.状态 = 新需求当前满足状态::比较关系不适用;
                return 结果;
            }
            const auto 比较 = 特征服务_.比较任务集合条件(*目标根, *特征->当前值, *集合目标);
            if (比较.状态 != 特征概念规则状态::已完成) {
                switch (比较.状态) {
                case 特征概念规则状态::资源失败:
                    结果.状态 = 新需求当前满足状态::资源失败; break;
                case 特征概念规则状态::读取接口未装配:
                    结果.状态 = 新需求当前满足状态::读取接口未装配; break;
                default:
                    结果.状态 = 新需求当前满足状态::结构不一致; break;
                }
                return 结果;
            }
            if (比较.比较值 != 0 && 比较.比较值 != 1) {
                结果.状态 = 新需求当前满足状态::结构不一致;
                return 结果;
            }
            结果.状态 = *比较.比较值 == 1 ? 新需求当前满足状态::已满足
                : 新需求当前满足状态::未满足但无有向差值;
            return 结果;
        }

        if(批) {
            if(const auto* domain=std::get_if<特征概念存在引用值域>(&目标概念->值域)) {
                const auto* current=std::get_if<稳定编码>(&*特征->当前值);
                要求需求物理(current&&有效(*current));
                追加筹办存在值域归属保护(*current,domain->存在概念节点,*批);
            } else if(目标概念->材料类型!=特征概念材料物理类型::I64标量) {
                // 其它材料的定位还会读材料owner；本叶没有该值型完整保护提供者。
                批->未就绪();
            }
        }
        const auto 定位 = 定位准确值(
            *特征->当前值, *目标概念, 特征值服务_, 存在服务_);
        if (!定位.结构有效) {
            结果.状态 = 新需求当前满足状态::比较关系不适用;
            return 结果;
        }
        if (定位.前提已变化) {
            结果.状态 = 新需求当前满足状态::目标引用前提已变化;
            return 结果;
        }
        if (定位.认知不足) {
            结果.状态 = 新需求当前满足状态::当前值认知不足;
            结果.认知缺口组 = 定位.认知缺口组;
            return 结果;
        }
        结果.准确差异 = 定位.准确差异;

        auto 关系组 = 比较概念->结果域种类组;
        if (关系组.empty()) 关系组.push_back(二次特征结果域种类::任意);
        if (std::any_of(关系组.begin(), 关系组.end(), [&](auto 关系) {
            return 关系接受位置(关系, 定位.位置);
        })) {
            结果.状态 = 新需求当前满足状态::已满足;
            if (定位.位置 == 准确值目标位置::命中) 结果.准确差异 = 0;
            return 结果;
        }

        auto 方向 = 所需方向::无;
        for (const auto 关系 : 关系组) {
            方向 = 合并方向(方向, 关系所需方向(关系, 定位.位置));
        }
        if (方向 == 所需方向::增加) {
            结果.状态 = 新需求当前满足状态::需增加;
            return 结果;
        }
        if (方向 == 所需方向::减少) {
            结果.状态 = 需求->根角色
                ? 新需求当前满足状态::双根负差非法
                : 新需求当前满足状态::需减少;
            return 结果;
        }
        结果.状态 = 需求->根角色
            ? 新需求当前满足状态::比较关系不适用
            : 新需求当前满足状态::未满足但无有向差值;
        return 结果;
}

std::optional<std::int64_t> 新_需求类::查询需求权重(
    稳定编码 需求节点) const noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        const auto 需求 = 读取需求治理信息(需求节点);
        return 需求 ? std::optional<std::int64_t>{需求->权重} : std::nullopt;
    } catch (...) {
        return std::nullopt;
    }
}

新需求操作状态 新_需求类::修改根需求权重(
    稳定编码 根需求节点,
    std::int64_t 新权重) noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        if (新权重 < 0) return 新需求操作状态::权重不合法;
        const auto 根需求 = 读取需求信息(需求目标读取上下文{特征服务_, 特征概念服务_}, 根需求节点);
        if (!根需求) return 新需求操作状态::需求不存在;
        if (!根需求->根角色) return 新需求操作状态::入口拒绝;

        std::vector<需求权重修改项> 修改组;
        if (!追加需求权重修改(根需求节点, 新权重, 修改组)) {
            return 新需求操作状态::结构不一致;
        }
        std::set<稳定编码> 已访问{根需求节点};
        if (!构建子树权重修改(需求目标读取上下文{特征服务_, 特征概念服务_},
                根需求节点, 新权重, std::nullopt, 已访问, 修改组)) {
            return 新需求操作状态::结构不一致;
        }
        if (修改组.empty()) return 新需求操作状态::无变化;
        switch (应用需求权重修改(修改组)) {
        case 需求权重写入状态::成功:
            return 新需求操作状态::已修改;
        case 需求权重写入状态::资源失败:
            return 新需求操作状态::资源失败;
        case 需求权重写入状态::回滚失败:
            return 新需求操作状态::结构不一致;
        }
        return 新需求操作状态::结构不一致;
    } catch (...) {
        return 新需求操作状态::资源失败;
    }
}

新需求操作状态 新_需求类::修改活动状态(
    稳定编码 需求节点,
    新需求活动状态 新状态) noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        if (!是活动状态值(static_cast<std::int64_t>(新状态))) {
            return 新需求操作状态::入口拒绝;
        }
        const auto 信息 = 读取需求治理信息(需求节点);
        if (!信息) return 新需求操作状态::需求不存在;
        if (信息->活动状态 == 新状态) return 新需求操作状态::无变化;
        const auto 字段组 = 全局基础数据集.查询字段(需求节点, 活动状态字段);
        if (字段组.size() != 1) return 新需求操作状态::结构不一致;

        std::vector<需求权重修改项> 权重修改组;
        const bool 参与资格变化 =
            (信息->活动状态 == 新需求活动状态::失活)
            != (新状态 == 新需求活动状态::失活);
        if (参与资格变化 && !信息->根角色 && 信息->执行域 == 新需求执行域::自我执行) {
            if (!信息->父需求节点) return 新需求操作状态::结构不一致;
            const auto 父需求 = 读取需求治理信息(*信息->父需求节点);
            if (!父需求) return 新需求操作状态::结构不一致;
            std::set<稳定编码> 已访问{*信息->父需求节点};
            const auto 覆盖 = std::make_optional(
                std::make_pair(需求节点, 新状态));
            if (!构建子树权重修改(需求目标读取上下文{特征服务_, 特征概念服务_}, *信息->父需求节点, 父需求->权重,
                    覆盖, 已访问, 权重修改组)) {
                return 新需求操作状态::结构不一致;
            }
        }

        if (!全局基础数据集.修改字段值(
                字段组.front().编码,
                基础值{static_cast<std::int64_t>(新状态)})) {
            return 新需求操作状态::资源失败;
        }
        const auto 权重写入 = 应用需求权重修改(权重修改组);
        if (权重写入 == 需求权重写入状态::成功) {
            return 新需求操作状态::已修改;
        }
        const bool 状态回滚成功 = 全局基础数据集.修改字段值(
            字段组.front().编码,
            基础值{static_cast<std::int64_t>(信息->活动状态)});
        if (!状态回滚成功 || 权重写入 == 需求权重写入状态::回滚失败) {
            return 新需求操作状态::结构不一致;
        }
        return 新需求操作状态::资源失败;
    } catch (...) {
        return 新需求操作状态::资源失败;
    }
}

特征概念规则状态 新_需求类::校验集合成员需求身份(稳定编码 需求节点) const noexcept {
    return 读取需求集合成员身份(需求节点).状态;
}

特征需求成员读取结果 新_需求类::读取需求集合成员身份(稳定编码 需求节点) const noexcept {
    const auto 读取 = 读取需求节点引用身份(需求节点);
    return {读取.状态, 读取.身份};
}

特征需求引用读取结果 新_需求类::读取需求节点引用身份(稳定编码 需求节点) const noexcept {
    特征需求引用读取结果 结果;
    const auto 校验 = [&]() -> 特征概念规则状态 {
    try {
        // 不取需求锁、不反入需求业务。F公开读口使用已有共享领域递归锁。
        if (!结构已初始化()) return 特征概念规则状态::读取接口未装配;
        if (!有效(需求节点)) return 特征概念规则状态::入口拒绝;
        const auto 读取 = 全局基础数据集.查询节点组完整结构({需求节点});
        if (读取.状态 == 基础结构操作状态::节点不存在) return 特征概念规则状态::值不存在;
        if (读取.状态 == 基础结构操作状态::资源失败) return 特征概念规则状态::资源失败;
        if (读取.状态 == 基础结构操作状态::已完成 && 读取.节点组.size() == 1
            && 读取.节点组.front().查询节点 == 需求节点 && !读取.节点组.front().快照)
            return 特征概念规则状态::值不存在;
        if (读取.状态 != 基础结构操作状态::已完成 || 读取.节点组.size() != 1
            || 读取.节点组.front().查询节点 != 需求节点
            || !读取.节点组.front().快照
            || 读取.节点组.front().快照->结构.节点.编码 != 需求节点)
            return 特征概念规则状态::内部不一致;
        const auto& 快照 = *读取.节点组.front().快照;
        结果.身份保护.push_back(快照);
        // 只有限物理读回；不调用本叶角色核验/持锁getter，避免F→需求锁闭环。
        核验服务取得D来源物理(快照.结构,结果.身份保护);
        const auto 投影 = 投影需求目标实例(需求目标读取上下文{特征服务_, 特征概念服务_}, 快照.结构, &结果.身份保护);
        const auto& 字段组 = 投影.字段组;
        bool 形状正确 = true;
        const auto 数量 = [&](稳定编码 字段) {
            return std::count_if(字段组.begin(), 字段组.end(), [&](const auto& 项) { return 项.字段节点 == 字段; });
        };
        const auto 唯一内容 = [&](稳定编码 字段) -> const 基础字段内容* {
            const 基础字段内容* 内容 = nullptr;
            for (const auto& 项 : 字段组) {
                if (项.所属节点 != 需求节点) { 形状正确 = false; return nullptr; }
                if (项.字段节点 != 字段) continue;
                if (内容) { 形状正确 = false; return nullptr; }
                内容 = &项.内容;
            }
            if (!内容) 形状正确 = false;
            return 内容;
        };
        const auto 节点字段 = [&](稳定编码 字段) -> std::optional<稳定编码> {
            const auto* 内容 = 唯一内容(字段);
            const auto* 值 = 内容 ? std::get_if<稳定编码>(内容) : nullptr;
            if (!值 || !有效(*值)) { 形状正确 = false; return std::nullopt; }
            return *值;
        };
        const auto 数值字段 = [&](稳定编码 字段) -> std::optional<std::int64_t> {
            const auto* 内容 = 唯一内容(字段);
            const auto* 值 = 内容 ? std::get_if<基础值>(内容) : nullptr;
            const auto* 数值 = 值 ? std::get_if<std::int64_t>(&值->材料) : nullptr;
            if (!数值) { 形状正确 = false; return std::nullopt; }
            return *数值;
        };
        const auto 类型 = 节点字段(节点类型字段);
        if (!形状正确) return 特征概念规则状态::内部不一致;
        if (类型 != 需求节点类型) return 特征概念规则状态::值不存在;
        const auto 主体 = 节点字段(需求主体字段);
        const auto 域 = 数值字段(需求执行域字段);
        if (!域 || (*域 != 1 && *域 != 2)) return 特征概念规则状态::内部不一致;
        if (主体) 结果.身份 = 特征需求成员身份{需求节点, *主体,
            *域 == static_cast<std::int64_t>(新需求执行域::被认知)};
        const bool 请求字段=数量(行动请求消息字段)||数量(行动请求含义字段)||数量(行动请求原需求字段)
            ||数量(行动请求人类字段)||数量(行动请求受话者字段)||数量(行动请求范围字段)||数量(行动请求路径字段);
        const auto source=数量(来源类型字段)?数值字段(来源类型字段):std::optional<std::int64_t>{};
        const bool 请求来源=source==static_cast<std::int64_t>(新需求来源类型::行动请求);
        if(数量(需求信息完善阶段字段)||数量(引用内容需求字段)||数量(目标新增成员见证字段)||请求字段||请求来源) {
            const auto d=解码需求治理物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 投影);
            const bool 内容目标=数量(引用内容需求字段)!=0;
            const bool 新增目标=数量(目标新增成员见证字段)!=0;
            if(请求来源) {
                if(*域!=static_cast<std::int64_t>(新需求执行域::自我执行)||!d.行动请求来源||!d.完整目标
                    ||d.需求主体存在节点!=d.行动请求来源->受请求者节点)
                    return 特征概念规则状态::内部不一致;
            } else if(新增目标) {
                if(*域!=static_cast<std::int64_t>(新需求执行域::自我执行)||!d.完整目标||!d.完整目标->目标.新增成员来源见证
                    ||d.来源类型!=新需求来源类型::任务子目标||!d.来源承接节点||!d.父需求节点||d.根角色)
                    return 特征概念规则状态::内部不一致;
            } else if(内容目标) {
                if(*域!=static_cast<std::int64_t>(新需求执行域::自我执行)||!d.完整目标||!d.完整目标->目标.引用内容定位)
                    return 特征概念规则状态::内部不一致;
            } else if(*域!=static_cast<std::int64_t>(新需求执行域::被认知)||!d.已知目标||!d.信息完善阶段)
                return 特征概念规则状态::内部不一致;
            // 不取需求锁、不fresh确认；合法阶段身份可被正式F引用。
            const auto registered=std::count_if(快照.入向字段组.begin(),快照.入向字段组.end(),[&](const auto& f){
                return f.所属节点==需求树根节点&&f.字段节点==被认知需求登记字段;});
            if(!内容目标&&!请求来源&&!新增目标&&(registered!=1||std::any_of(快照.入向字段组.begin(),快照.入向字段组.end(),[&](const auto& f){return f.字段节点==列表成员字段;})))
                return 特征概念规则状态::内部不一致;
            if((请求来源||新增目标)&&(registered!=0||std::count_if(快照.入向字段组.begin(),快照.入向字段组.end(),[&](const auto& f){return f.字段节点==列表成员字段;})!=1))
                return 特征概念规则状态::内部不一致;
            std::set<稳定编码> references;
            if(新增目标){references.insert(需求树根节点);references.insert(d.完整目标->目标.新增成员来源见证->基准材料节点);}
            for(const auto& f:字段组)if(const auto* ref=std::get_if<稳定编码>(&f.内容))references.insert(*ref);
            const auto refs=全局基础数据集.查询节点组完整结构({references.begin(),references.end()});
            if(refs.状态==基础结构操作状态::资源失败)return 特征概念规则状态::资源失败;
            if(refs.状态!=基础结构操作状态::已完成||refs.节点组.size()!=references.size())return 特征概念规则状态::内部不一致;
            for(const auto& ref:refs.节点组){if(!ref.快照)return 特征概念规则状态::内部不一致;结果.身份保护.push_back(*ref.快照);}
            return 特征概念规则状态::已完成;
        }
        const bool 有目标 = 数量(目标存在字段) != 0;
        std::optional<std::int64_t> 形式;
        if (有目标) {
            (void)节点字段(目标存在字段);
            (void)节点字段(目标特征类型字段);
            形式 = 数值字段(目标形式字段);
        } else if (数量(目标特征类型字段) || 数量(目标形式字段)
            || 数量(目标值域字段) || 数量(目标比较关系字段) || 数量(目标变化方向字段)) 形状正确 = false;
        const auto 活动 = 数值字段(活动状态字段);
        const auto 权重 = 数值字段(需求权重字段);
        if (!形状正确 || !活动 || !是活动状态值(*活动) || !权重 || *权重 < 0)
            return 特征概念规则状态::内部不一致;
        if (形式 == static_cast<std::int64_t>(新需求目标形式::值)) {
            (void)节点字段(目标值域字段);
            (void)节点字段(目标比较关系字段);
            if (数量(目标变化方向字段) != 0) 形状正确 = false;
        } else if (形式 == static_cast<std::int64_t>(新需求目标形式::变化方向)) {
            const auto 方向 = 数值字段(目标变化方向字段);
            if (!方向 || !转换目标变化方向(*方向) || 数量(目标值域字段) != 0
                || 数量(目标比较关系字段) != 0) 形状正确 = false;
        } else if (有目标) 形状正确 = false;
        if (*域 == static_cast<std::int64_t>(新需求执行域::被认知)) {
            (void)节点字段(认知理解字段);
            (void)节点字段(认知内容确认字段);
            if (数量(集合发布场景字段) || 数量(集合发布动作字段)) {
                (void)节点字段(集合发布场景字段);
                (void)节点字段(集合发布动作字段);
            }
            if (数量(需求首次发布首状态字段)) {
                (void)节点字段(需求首次发布首状态字段);
                if (数量(集合发布场景字段) != 1 || 数量(集合发布动作字段) != 1) 形状正确 = false;
            }
            if (数量(认知来源要求字段)) (void)节点字段(认知来源要求字段);
            if (!有目标 || *权重 != 0 || 数量(根角色字段) || 数量(来源类型字段)
                || 数量(来源承接字段) || 数量(外部服务要求字段)
                || 数量(服务要求确认字段) || 数量(服务需求取得执行字段)) 形状正确 = false;
            return 形状正确 ? 特征概念规则状态::已完成 : 特征概念规则状态::内部不一致;
        }
        if (数量(认知理解字段) || 数量(认知内容确认字段) || 数量(认知来源要求字段)
            || 数量(集合发布场景字段) || 数量(集合发布动作字段) || 数量(需求首次发布首状态字段)) 形状正确 = false;
        if (数量(根角色字段) != 0) {
            const auto 角色 = 数值字段(根角色字段);
            if (!角色 || !转换根角色(*角色) || 数量(来源类型字段) != 0
                || 数量(来源承接字段) != 0 || 数量(服务要求确认字段) != 0
                || 数量(服务需求取得执行字段) != 0 || 数量(外部服务要求字段) != 0 || !有目标) 形状正确 = false;
        } else {
            const auto 来源 = 数值字段(来源类型字段);
            if (来源 == static_cast<std::int64_t>(新需求来源类型::任务子目标)) {
                (void)节点字段(来源承接字段);
                if (!有目标 || 数量(外部服务要求字段)) 形状正确 = false;
            } else if (来源 == static_cast<std::int64_t>(新需求来源类型::外部服务要求)) {
                (void)节点字段(外部服务要求字段);
                if (!有目标 || 数量(来源承接字段)
                    || 数量(服务要求确认字段) != 1) 形状正确 = false;
            } else if (来源 != static_cast<std::int64_t>(新需求来源类型::本能定义)
                || 数量(来源承接字段) != 0 || 数量(外部服务要求字段) != 0 || !有目标) 形状正确 = false;
            if (来源 != static_cast<std::int64_t>(新需求来源类型::任务子目标)
                && 来源 != static_cast<std::int64_t>(新需求来源类型::外部服务要求)
                && 数量(服务要求确认字段) != 0) 形状正确 = false;
        }
        std::set<稳定编码> 确认组;
        for (const auto& 字段 : 字段组) if (字段.字段节点 == 服务要求确认字段) {
            const auto* 确认 = std::get_if<稳定编码>(&字段.内容);
            if (!确认 || !有效(*确认) || !确认组.insert(*确认).second) 形状正确 = false;
        }
        if (数量(服务需求取得执行字段) != 0) {
            (void)节点字段(服务需求取得执行字段);
            if (确认组.empty()) 形状正确 = false;
        }
        return 形状正确 ? 特征概念规则状态::已完成 : 特征概念规则状态::内部不一致;
    } catch (const std::bad_alloc&) { return 特征概念规则状态::资源失败; }
    catch (新需求目标定义核验状态 s) {
        return s == 新需求目标定义核验状态::前提已变化 ? 特征概念规则状态::前提已变化 : 特征概念规则状态::内部不一致;
    }
    catch (新服务取得目标实例状态 s) {
        return s==新服务取得目标实例状态::前提已变化 ? 特征概念规则状态::前提已变化 : 特征概念规则状态::内部不一致;
    }
    catch (...) { return 特征概念规则状态::内部不一致; }
    };
    结果.状态 = 校验();
    if (结果.状态 == 特征概念规则状态::已完成) {
        try { if (!需求内容保护未变(结果.身份保护)) 结果.状态 = 特征概念规则状态::前提已变化; }
        catch (const std::bad_alloc&) { 结果.状态 = 特征概念规则状态::资源失败; }
        catch (...) { 结果.状态 = 特征概念规则状态::内部不一致; }
    }
    if (结果.状态 != 特征概念规则状态::已完成) {
        结果.身份.reset();
        结果.身份保护.clear();
    }
    return 结果;
}

bool 新_需求类::是需求节点(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(新需求互斥);
        return 读取需求治理信息(节点).has_value();
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"核验需求身份", L"需求身份读取故障");
    }
}

// R102（SET-DOMAIN-MEMBER-CONCEPT §4）：不取需求锁；只读冻结schema与L1，按先天需求槽投影全部需求。
特征概念规则状态 新_需求类::读取需求直接内容无锁(稳定编码 D,
    std::vector<std::pair<稳定编码, 正式内容值>>& 内容组,
    std::vector<基础联合节点快照>& 保护) const noexcept {
    using R = 特征概念规则状态;
    内容组.clear(); 保护.clear();
    try {
        const auto 身份 = 读取需求节点引用身份(D);
        if (身份.状态 != R::已完成) return 身份.状态;
        if (!身份.身份 || 身份.身份保护.empty()) return R::内部不一致;
        保护 = 身份.身份保护;
        const auto 原D = std::find_if(保护.begin(), 保护.end(),
            [&](const auto& 项) { return 项.结构.节点.编码 == D; });
        if (原D == 保护.end()) return R::内部不一致;
        const auto 需求 = 解码需求治理物理(需求目标读取上下文{特征服务_, 特征概念服务_}, 原D->结构);
        for (std::uint32_t 槽 = 先天特征类型槽::需求主体; 槽 <= 先天特征类型槽::内容确认来源; ++槽) {
            const auto r = 特征概念服务_.读取先天特征类型槽(槽);
            if (r.状态 == R::值不存在) return R::读取接口未装配;
            if (r.状态 != R::已完成) return r.状态;
            if (!r.类型根) return R::内部不一致;
            if (!并需求保护(保护, r.完整保护组)) return R::前提已变化;
            内容组.emplace_back(*r.类型根, 投影需求内容(*r.类型根, 槽, 需求).内容);
        }
        return R::已完成;
    } catch (新需求目标定义核验状态 s) {
        return s == 新需求目标定义核验状态::前提已变化 ? R::前提已变化 : R::内部不一致;
    } catch (const 需求物理读取失败& 错误) {
        return 错误.原因 == 基础结构操作状态::节点不存在 ? R::前提已变化
            : 错误.原因 == 基础结构操作状态::资源失败 ? R::资源失败 : R::内部不一致;
    } catch (const std::bad_alloc&) { return R::资源失败; }
    catch (...) { return R::内部不一致; }
}

特征成员概念归属结果 新_需求类::判定需求集合成员概念(稳定编码 D, 稳定编码 C) const noexcept {
    using R = 特征概念规则状态; using F = 正式内容符合状态; using G = 特征成员概念归属;
    try {
        if (!有效(D) || !有效(C)) return {R::入口拒绝, G::未知};
        正式嵌套判定防护 防护(D, C);
        if (!防护.允许()) return {R::已完成, G::未知};
        const auto 先天 = 存在概念服务_.读取先天需求任务方法概念();
        if (先天.状态 != 正式结构操作状态::已读取 || !先天.信息) return {R::读取接口未装配, G::未知};
        // 设计3.1：需求集合只认槽1需求或其下位信息结构概念。
        const auto 合法 = 存在概念服务_.正式结构概念属于(C, 先天.信息->需求);
        if (!合法) return {R::前提已变化, G::未知};
        if (!*合法) return {R::入口拒绝, G::未知};
        if (C == 先天.信息->需求) {
            const auto 身份 = 读取需求节点引用身份(D);
            if (身份.状态 == R::已完成) return {R::已完成, 身份.身份 ? G::属于 : G::不属于};
            if (身份.状态 == R::值不存在) return {R::已完成, G::不属于};
            return {身份.状态, G::未知};
        }
        std::vector<std::pair<稳定编码, 正式内容值>> 内容组;
        std::vector<基础联合节点快照> 保护;
        const auto 读 = 读取需求直接内容无锁(D, 内容组, 保护);
        if (读 == R::值不存在) return {R::已完成, G::不属于};
        if (读 != R::已完成) return {读, G::未知};
        bool 依赖未就绪 = false;
        const auto 查 = [&](稳定编码 FT) -> const 正式内容值* {
            for (const auto& [ft, v] : 内容组) if (ft == FT) return &v;
            return nullptr;
        };
        const 正式直接内容读取器 读取 = [&](稳定编码 实例, 稳定编码 FT) -> 正式内容值 {
            if (实例 != D) throw std::runtime_error("需求归属判定切换了实例");
            const auto* v = 查(FT);
            if (!v) throw F::未支持;
            return *v;
        };
        const 正式引用身份核验器 身份 = [&](基础概念原语 p, const std::vector<std::int64_t>& 数值,
            const std::vector<稳定编码>&, const 正式内容值& v) -> 正式内容符合结果 {
            if (v.种类 == 正式内容值种类::I64数值 || !v.值) return {F::不符合, {}};
            const auto 节点 = std::get<稳定编码>(*v.值);
            if (p == 基础概念原语::合法存在) {
                if (v.种类 != 正式内容值种类::存在引用) return {F::不符合, {}};
                const auto r = 存在服务_.读取存在身份保护(节点);
                if (r.前提已变化) return {F::前提已变化, {}};
                if (r.状态 == 基础结构操作状态::资源失败) return {F::资源失败, {}};
                return {r.信息 ? F::符合 : F::不符合, r.预期节点组};
            }
            if (p == 基础概念原语::具名领域身份 && 数值.size() == 1) {
                if (数值.front() == 2 || 数值.front() == 3) {
                    const auto 种类 = 数值.front() == 2 ? 正式内容值种类::语言理解引用 : 正式内容值种类::内容确认引用;
                    const bool 同来源 = v.种类 == 种类 && std::any_of(内容组.begin(), 内容组.end(),
                        [&](const auto& x) { return x.second.种类 == 种类 && x.second.值 == v.值; });
                    return {同来源 ? F::符合 : F::不符合, {}};
                }
                if (数值.front() == 15 && v.种类 == 正式内容值种类::需求引用) {
                    const auto r = 读取需求节点引用身份(节点);
                    if (r.状态 == R::资源失败) return {F::资源失败, {}};
                    if (r.状态 == R::前提已变化) return {F::前提已变化, {}};
                    if (r.状态 == R::读取接口未装配) { 依赖未就绪 = true; return {F::未支持, {}}; }
                    return {r.身份 ? F::符合 : F::不符合, r.身份保护};
                }
                return {F::未支持, {}};
            }
            if (p == 基础概念原语::正式信息身份) return {F::符合, {}};
            if (v.种类 != 正式内容值种类::概念引用) return {F::不符合, {}};
            if (p == 基础概念原语::合法FT根 || p == 基础概念原语::合法特征概念) {
                const auto r = 特征概念服务_.读取特征概念定义保护(节点);
                if (r.前提已变化) return {F::前提已变化, {}};
                if (r.状态 == 基础结构操作状态::资源失败) return {F::资源失败, {}};
                return {r.信息 && (p != 基础概念原语::合法FT根 || r.信息->类型根 == 节点) ? F::符合 : F::不符合, r.预期节点组};
            }
            if (p == 基础概念原语::合法比较关系) {
                const auto r = 二次特征概念服务_.读取二次特征定义保护(节点);
                if (r.前提已变化) return {F::前提已变化, {}};
                if (r.状态 == 基础结构操作状态::资源失败) return {F::资源失败, {}};
                return {r.信息 ? F::符合 : F::不符合, r.预期节点组};
            }
            if (p == 基础概念原语::正式概念身份) {
                const 正式概念身份读取接口* 提供者[]{&特征概念服务_, &二次特征概念服务_, &存在概念服务_};
                for (const auto* owner : 提供者) {
                    const auto r = owner->读取正式概念身份保护(节点);
                    if (r.状态 == 正式概念读取状态::前提已变化) return {F::前提已变化, {}};
                    if (r.状态 == 正式概念读取状态::依赖未就绪) { 依赖未就绪 = true; return {F::未支持, {}}; }
                    if (r.身份) return {F::符合, r.身份->完整保护组};
                }
                return {F::不符合, {}};
            }
            return {F::未支持, {}};
        };
        // 801/802 同D关联：沿用判定被认知需求内容保护的同D读取口径。
        const 正式关联约束核验器 关联 = [&](稳定编码 实例, const 结构约束定义& r) -> 正式内容符合结果 {
            if (实例 != D) throw std::runtime_error("需求归属关联切换了实例");
            if (r.原语 != 结构约束原语::域属于FT && r.原语 != 结构约束原语::比较适用于目标) return {F::未支持, {}};
            for (const auto FT : r.项目FT组) {
                const auto* 值 = 查(FT);
                if (!值) return {F::未支持, {}};
                if (值->状态 == 正式内容值状态::当前未知) return {F::当前未知, {}};
                if (值->状态 == 正式内容值状态::不适用) return {F::不符合, {}};
            }
            return {F::符合, {}};
        };
        const 正式结构范围判定器 嵌套 = [&](const 正式内容值& v, 稳定编码 C2) -> 特征成员概念归属结果 {
            if (!v.值 || !std::holds_alternative<稳定编码>(*v.值)) return {R::已完成, G::未知};
            const auto 节点 = std::get<稳定编码>(*v.值);
            特征成员概念归属结果 r;
            if (v.种类 == 正式内容值种类::需求引用) r = 判定需求集合成员概念(节点, C2);
            else if (v.种类 == 正式内容值种类::存在引用)
                r = static_cast<const 特征存在成员读取接口&>(存在服务_).判定存在集合成员概念(节点, C2);
            else return {R::已完成, G::未知}; // 需求内容不含任务/方法引用
            if (r.状态 == R::读取接口未装配) 依赖未就绪 = true;
            return r;
        };
        const auto 结果 = 存在概念服务_.判定正式结构内容(C, D, 读取, 身份, 关联, {}, 嵌套);
        if (结果.状态 == F::资源失败) return {R::资源失败, G::未知};
        if (结果.状态 == F::前提已变化) return {R::前提已变化, G::未知};
        if (!并需求保护(保护, 结果.完整保护组) || !需求内容保护未变(保护)) return {R::前提已变化, G::未知};
        if (结果.状态 == F::符合) return {R::已完成, G::属于};
        if (结果.状态 == F::不符合) return {R::已完成, G::不属于};
        if (依赖未就绪) return {R::读取接口未装配, G::未知};
        return {R::已完成, G::未知};
    } catch (const std::bad_alloc&) { return {R::资源失败, G::未知}; }
    catch (...) { return {R::内部不一致, G::未知}; }
}

} // namespace 海中鱼巣
