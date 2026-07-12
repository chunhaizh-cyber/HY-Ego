// 文件规则：任务服务承接需求生成任务入口；线程不是动作来源。
#pragma once

#include "../核心/节点仓库.h"
#include "../核心/主信息仓库.h"
#include "../核心/关系仓库.h"
#include "../核心/容错检查.h"
#include "场景服务.h"
#include "存在服务.h"
#include "需求服务.h"
#include "状态服务.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <optional>
#include <vector>
#include <utility>

namespace 海中鱼巣 {

enum class 任务生命周期状态 : std::int64_t {
    未定义 = 0,
    已创建 = 1,
    已承接 = 2,
    筹办中 = 3,
    排队中 = 4,
    执行中 = 5,
    等待中 = 6,
    待重筹办 = 7,
    已完成 = 8,
    失败 = 9,
    取消 = 10
};

struct 任务承接材料 {
    节点句柄 来源需求;
    节点句柄 虚拟存在;
    节点句柄 运行场景;
    节点句柄 目标状态;
    节点句柄 创建状态;
};

enum class 任务请求状态 : std::uint32_t {
    可作为请求材料 = 1,
    无效任务 = 2,
    缺少任务方法关系 = 3,
    承接壳不完整 = 4
};

struct 任务生命周期迁移请求材料 {
    节点句柄 任务;
    节点句柄 当前状态;
    任务生命周期状态 目标状态 = 任务生命周期状态::未定义;
    任务请求状态 状态 = 任务请求状态::无效任务;
};

struct 任务筹办回执请求材料 {
    节点句柄 任务;
    节点句柄 来源需求;
    节点句柄 运行场景;
    任务请求状态 状态 = 任务请求状态::无效任务;
};

struct 任务执行桥请求材料 {
    节点句柄 任务;
    节点句柄 方法;
    任务请求状态 状态 = 任务请求状态::无效任务;
};

struct 任务非权威统计材料 {
    节点句柄 任务;
    std::uint64_t 状态节点数 = 0;
    任务请求状态 状态 = 任务请求状态::无效任务;
};

struct 任务生命周期迁移证据 {节点句柄 来源任务;节点句柄 来源方法;节点句柄 来源动作;节点句柄 来源状态;节点句柄 来源动态;节点句柄 取消授权来源;节点句柄 动作停止证据;bool operator==(const 任务生命周期迁移证据&)const=default;};
struct 任务生命周期提交请求 {节点句柄 任务;关系句柄 预期当前关系;std::uint32_t 预期生命周期版本=0;任务生命周期状态 目标阶段=任务生命周期状态::未定义;std::uint64_t 发生时间戳=0;std::uint64_t 幂等材料编号=0;任务生命周期迁移证据 证据;};
struct 任务生命周期当前材料 {节点句柄 任务;节点句柄 来源需求;节点句柄 主体;节点句柄 场景;关系句柄 关系;节点句柄 状态;任务生命周期状态 阶段=任务生命周期状态::未定义;std::uint32_t 版本=0;std::uint64_t 时间戳=0;std::optional<std::uint64_t> 幂等材料编号;std::optional<关系句柄> 前驱关系;任务生命周期迁移证据 证据;};
struct 任务生命周期历史项 {关系句柄 关系;节点句柄 状态;任务生命周期状态 阶段=任务生命周期状态::未定义;std::uint32_t 版本=0;std::uint64_t 时间戳=0;记录状态 记录阶段=记录状态::无效;std::optional<std::uint64_t> 幂等材料编号;std::optional<关系句柄> 前驱关系;任务生命周期迁移证据 证据;};
enum class 任务生命周期提交状态:std::uint32_t{已提交=1,幂等读回=2,已拒绝=3,内部不一致=4};
struct 任务生命周期提交结果 {任务生命周期提交状态 状态=任务生命周期提交状态::已拒绝;std::optional<任务生命周期历史项> 原迁移结果;std::optional<任务生命周期当前材料> 当前投影;};

struct 任务承接授权材料 {
    节点句柄 任务;
    节点句柄 需求;
    关系句柄 需求拥有任务关系;
    关系句柄 任务来源需求关系;
    bool operator==(const 任务承接授权材料&) const = default;
};

struct 任务选择权威节点来源 {
    节点句柄 节点;
    节点类型 类型 = 节点类型::未分类;
    bool operator==(const 任务选择权威节点来源&) const = default;
};

struct 任务选择权威关系来源 {
    关系句柄 关系;
    关系类型 类型 = 关系类型::普通父子;
    节点句柄 源节点;
    节点句柄 目标节点;
    std::int64_t 顺序号 = 0;
    bool operator==(const 任务选择权威关系来源&) const = default;
};

struct 任务方法选择请求 {
    std::uint32_t 格式版本 = 1;
    节点句柄 任务;
    节点句柄 来源需求;
    任务承接授权材料 承接授权;
    关系句柄 生命周期关系;
    std::uint32_t 生命周期版本 = 0;
    节点句柄 方法;
    std::uint64_t 幂等材料编号 = 0;
    std::uint32_t 排序规则版本 = 0;
    std::uint32_t 召回请求规则版本 = 0;
    std::uint64_t 索引发布版本 = 0;
    std::uint64_t 来源候选数量 = 0;
    std::uint64_t 重复输入数量 = 0;
    std::uint32_t 建议状态 = 0;
    std::vector<std::int64_t> 完整有序候选规范元组;
    std::vector<任务选择权威节点来源> 权威节点来源组;
    std::vector<任务选择权威关系来源> 权威关系来源组;
    bool operator==(const 任务方法选择请求&) const = default;
};

struct 任务方法选择记录快照 {
    节点句柄 记录节点;
    关系句柄 发布关系;
    关系句柄 方法投影关系;
    任务方法选择请求 请求;
};

struct 当前任务方法选择材料 {
    任务方法选择记录快照 记录;
    任务生命周期当前材料 生命周期;
};

enum class 任务方法选择提交状态 : std::uint32_t {
    已提交 = 1,
    幂等读回 = 2,
    已拒绝 = 3,
    内部不一致 = 4
};

struct 任务方法选择提交结果 {
    任务方法选择提交状态 状态 = 任务方法选择提交状态::已拒绝;
    std::optional<当前任务方法选择材料> 当前选择;
};

class 任务服务 {
public:
    任务服务(主信息仓库& 主信息, 节点仓库& 节点, 关系仓库& 关系)
        : 主信息_(主信息), 节点_(节点), 关系_(关系) {
    }
    任务服务(主信息仓库& a,节点仓库& b,关系仓库& c,结构事务接线 d)
        :主信息_(a),节点_(b),关系_(c),接线_(std::move(d)){}

    节点句柄 按需求创建任务(节点句柄 d,const 需求服务& 需求,存在服务& 存在,状态服务& 状态,
        std::uint64_t 时间,const 结构事务令牌& t){
        if(!验证独占(t)||时间==0||!需求.需求是否有效(d,t))return {};
        auto dm=需求.读取需求承接材料(d,t); if(!dm)return {};
        auto mi=主信息_.创建主信息(t); auto task=句柄有效(mi)?节点_.创建节点(节点类型::任务,mi,t):节点句柄{};
        std::vector<关系句柄> own;
        if(!句柄有效(task)){if(句柄有效(mi))(void)主信息_.删除主信息(mi,t);return {};}
        auto ec=存在.创建虚拟存在候选(task,t); if(!ec){清任务(task,mi,own,t);return {};}
        auto sc=状态.创建实例状态候选(dm->场景,ec->读取存在(),时间,static_cast<std::int64_t>(任务生命周期状态::已创建),t);
        if(!sc){(void)存在.撤销候选(*ec,t);清任务(task,mi,own,t);return {};}
        own={
            关系_.创建关系(关系类型::归属,d,task,0,t),关系_.创建关系(关系类型::引用,task,d,0,t),
            关系_.创建关系(关系类型::引用,task,ec->读取存在(),0,t),关系_.创建关系(关系类型::引用,task,dm->场景,0,t),
            关系_.创建关系(关系类型::模板,task,dm->目标状态,0,t),关系_.创建关系(关系类型::任务生命周期,task,sc->读取状态(),1,t)};
        const bool rel=std::all_of(own.begin(),own.end(),[](auto x){return 句柄有效(x);});
        auto cur=rel?读取任务生命周期当前材料(task,状态,t):std::optional<任务生命周期当前材料>{};
        if(!cur){清关系(own,t);(void)状态.撤销候选(*sc,t);(void)存在.撤销候选(*ec,t);清任务(task,mi,{},t);return {};}
        if(状态.确认候选(*sc,t)!=状态候选操作状态::已完成
            ||存在.确认候选(*ec,t)!=未发布候选操作状态::已完成){return {};}
        return task;
    }

    std::optional<任务生命周期当前材料> 读取任务生命周期当前材料(节点句柄 task,const 状态服务& 状态,const 结构事务令牌& t) const{
        if(!验证独占(t))return std::nullopt; auto rs=关系_.获取关系记录组(task,关系类型::任务生命周期,t);
        if(rs.size()!=1||rs[0].顺序号<=0||rs[0].状态!=记录状态::有效)return std::nullopt;
        auto sm=状态.读取状态材料(rs[0].目标节点,t); if(!sm||!生命周期值有效(sm->状态值))return std::nullopt;
        auto demand=唯一目标(task,关系类型::引用,节点类型::需求,t); if(!demand)return std::nullopt;
        const auto v=static_cast<std::uint32_t>(rs[0].顺序号);const auto idem=状态.读取状态幂等编号(sm->状态,t);const auto hist=读取任务生命周期历史(task,状态,t);std::optional<关系句柄> prev;if(v>1){for(const auto& h:hist)if(h.版本+1==v){if(prev)return std::nullopt;prev=h.关系;}if(!prev||!idem)return std::nullopt;}const auto ev=读取迁移证据(sm->状态,t);if(!ev)return std::nullopt;
        return 任务生命周期当前材料{task,*demand,sm->主体,sm->场景,{task.仓库编号,rs[0].关系编号,rs[0].版本号},sm->状态,static_cast<任务生命周期状态>(sm->状态值),v,sm->发生时间戳,idem,prev,*ev};
    }
    std::vector<任务生命周期历史项> 读取任务生命周期历史(节点句柄 task,const 状态服务& 状态,const 结构事务令牌& t) const{
        std::vector<任务生命周期历史项> out;if(!验证独占(t))return out;
        for(const auto& r:关系_.获取关系审计记录组(task,关系类型::任务生命周期,t)){
            auto sm=状态.读取状态材料(r.目标节点,t);if(!sm||r.顺序号<=0||!生命周期值有效(sm->状态值))continue;const auto v=static_cast<std::uint32_t>(r.顺序号);const auto idem=状态.读取状态幂等编号(r.目标节点,t);const auto ev=读取迁移证据(r.目标节点,t);if(!ev)continue;const auto rv=(r.状态==记录状态::已失效&&r.版本号>1)?r.版本号-1:r.版本号;out.push_back({{task.仓库编号,r.关系编号,rv},r.目标节点,static_cast<任务生命周期状态>(sm->状态值),v,sm->发生时间戳,r.状态,idem,std::nullopt,*ev});
        }
        std::sort(out.begin(),out.end(),[](const auto&a,const auto&b){return a.版本<b.版本;});for(std::size_t i=0;i<out.size();++i){if(out[i].版本!=i+1)return {};if(i>0)out[i].前驱关系=out[i-1].关系;if(i==0&&out[i].幂等材料编号)return {};if(i>0&&!out[i].幂等材料编号)return {};}return out;
    }
    任务生命周期提交结果 提交任务生命周期迁移(const 任务生命周期提交请求& r,状态服务& s,const 结构事务令牌& t){return 提交生命周期(r,s,t,false,false);}
    任务生命周期提交结果 提交任务失败(const 任务生命周期提交请求& r,状态服务& s,const 结构事务令牌& t){if(r.目标阶段!=任务生命周期状态::失败||(!句柄有效(r.证据.来源任务)&&!句柄有效(r.证据.来源方法)&&!句柄有效(r.证据.来源动作)&&!句柄有效(r.证据.来源动态)))return {};return 提交生命周期(r,s,t,true,false);}
    任务生命周期提交结果 提交任务取消(const 任务生命周期提交请求& r,状态服务& s,const 结构事务令牌& t){if(r.目标阶段!=任务生命周期状态::取消||!句柄有效(r.证据.取消授权来源))return {};return 提交生命周期(r,s,t,false,true);}

    std::optional<任务承接授权材料> 读取任务承接授权材料(
        节点句柄 任务, const 结构事务令牌& t) const {
        if (!验证独占(t)) return std::nullopt;
        const auto 任务记录 = 节点_.读取节点(任务, t);
        if (!任务记录 || 任务记录->类型 != 节点类型::任务) return std::nullopt;
        std::optional<关系记录> 后向;
        for (const auto& 记录 : 关系_.获取关系记录组(任务, 关系类型::引用, t)) {
            const auto 目标 = 节点_.读取节点(记录.目标节点, t);
            if (!目标 || 目标->类型 != 节点类型::需求) continue;
            if (后向) return std::nullopt;
            后向 = 记录;
        }
        if (!后向) return std::nullopt;
        std::optional<关系记录> 前向;
        for (const auto& 记录 : 关系_.获取来源关系记录组(任务, 关系类型::归属, t)) {
            if (记录.源节点 != 后向->目标节点) continue;
            if (前向) return std::nullopt;
            前向 = 记录;
        }
        if (!前向) return std::nullopt;
        return 任务承接授权材料{
            任务, 后向->目标节点,
            {前向->源节点.仓库编号, 前向->关系编号, 前向->版本号},
            {任务.仓库编号, 后向->关系编号, 后向->版本号}};
    }

    std::optional<任务方法选择记录快照> 读取任务方法选择审计(
        节点句柄 记录节点, const 结构事务令牌& t) const {
        if (!验证独占(t)) return std::nullopt;
        return 读取选择记录(记录节点, std::nullopt, t);
    }

    std::optional<当前任务方法选择材料> 读取当前任务方法选择材料(
        节点句柄 任务, const 状态服务& 状态, const 结构事务令牌& t) const {
        if (!验证独占(t)) return std::nullopt;
        const auto 当前生命周期 = 读取任务生命周期当前材料(任务, 状态, t);
        if (!当前生命周期) return std::nullopt;
        const auto 发布组 = 关系_.获取关系记录组(任务, 关系类型::任务方法选择, t);
        if (发布组.size() != 1) return std::nullopt;
        const auto 发布句柄 = 关系句柄{任务.仓库编号, 发布组[0].关系编号, 发布组[0].版本号};
        const auto 快照 = 读取选择记录(发布组[0].目标节点, 发布句柄, t);
        if (!快照) return std::nullopt;
        bool 所属筹办周期存在 = false;
        for (const auto& 历史 : 读取任务生命周期历史(任务, 状态, t)) {
            所属筹办周期存在 = 所属筹办周期存在
                || (历史.关系 == 快照->请求.生命周期关系
                    && 历史.版本 == 快照->请求.生命周期版本
                    && 历史.阶段 == 任务生命周期状态::筹办中);
        }
        if (!所属筹办周期存在) return std::nullopt;
        return 当前任务方法选择材料{*快照, *当前生命周期};
    }

    任务方法选择提交结果 提交任务方法选择(
        const 任务方法选择请求& 请求, const 状态服务& 状态, const 结构事务令牌& t) {
        if (!验证独占(t) || !选择请求形状完整(请求)) return {};
        const auto 当前生命周期 = 读取任务生命周期当前材料(请求.任务, 状态, t);
        const auto 当前授权 = 读取任务承接授权材料(请求.任务, t);
        if (!当前生命周期 || 当前生命周期->阶段 != 任务生命周期状态::筹办中
            || 当前生命周期->关系 != 请求.生命周期关系
            || 当前生命周期->版本 != 请求.生命周期版本
            || !当前授权 || *当前授权 != 请求.承接授权
            || 当前授权->需求 != 请求.来源需求
            || !复核权威来源(请求, t)) return {};

        std::vector<任务方法选择记录快照> 同键记录;
        for (const auto& 发布 : 关系_.获取关系审计记录组(请求.任务, 关系类型::任务方法选择, t)) {
            const auto 句柄 = 关系句柄{请求.任务.仓库编号, 发布.关系编号,
                发布.状态 == 记录状态::已失效 && 发布.版本号 > 1 ? 发布.版本号 - 1 : 发布.版本号};
            const auto 快照 = 读取选择记录(发布.目标节点, 句柄, t);
            if (快照 && 快照->请求.幂等材料编号 == 请求.幂等材料编号) 同键记录.push_back(*快照);
        }
        if (同键记录.size() > 1) {
            (void)追根因检查(false, L"同一任务方法选择幂等编号审计记录不唯一。");
            return {任务方法选择提交状态::内部不一致, std::nullopt};
        }
        if (同键记录.size() == 1) {
            if (!(同键记录[0].请求 == 请求)) return {};
            const auto 当前 = 读取当前任务方法选择材料(请求.任务, 状态, t);
            return 当前 && 当前->记录.记录节点 == 同键记录[0].记录节点
                ? 任务方法选择提交结果{任务方法选择提交状态::幂等读回, 当前}
                : 任务方法选择提交结果{};
        }
        if (!关系_.获取关系记录组(请求.任务, 关系类型::任务方法选择, t).empty()) return {};

        const auto 主信息 = 主信息_.创建主信息(t);
        const auto 记录节点 = 句柄有效(主信息)
            ? 节点_.创建节点(节点类型::任务方法选择记录, 主信息, t) : 节点句柄{};
        if (!句柄有效(记录节点)) {
            if (句柄有效(主信息)) (void)主信息_.删除主信息(主信息, t);
            return {};
        }
        auto 值组 = 编码选择请求(请求);
        bool 写入完整 = !值组.empty();
        for (std::size_t i = 0; 写入完整 && i < 值组.size(); ++i) {
            写入完整 = 主信息_.写入I64值(主信息, i, 值组[i], t);
        }
        关系句柄 投影;
        关系句柄 发布;
        if (写入完整) 投影 = 关系_.创建关系(关系类型::引用, 请求.任务, 请求.方法, 任务选择方法顺序号, t);
        if (句柄有效(投影)) {
            const std::array<std::int64_t, 3> 投影身份{
                static_cast<std::int64_t>(投影.仓库编号), static_cast<std::int64_t>(投影.关系编号),
                static_cast<std::int64_t>(投影.版本号)};
            auto 写入索引 = 值组.size();
            for (const auto 值 : 投影身份) {
                if (!主信息_.写入I64值(主信息, 写入索引++, 值, t)) { 写入完整 = false; break; }
            }
        }
        if (写入完整 && 句柄有效(投影)) {
            发布 = 关系_.创建关系(关系类型::任务方法选择, 请求.任务, 记录节点, 任务选择方法顺序号, t);
        }
        if (!句柄有效(发布)) {
            if (句柄有效(投影)) (void)关系_.删除关系(投影, t);
            (void)节点_.删除节点(记录节点, t);
            (void)主信息_.删除主信息(主信息, t);
            return {};
        }
        const auto 当前 = 读取当前任务方法选择材料(请求.任务, 状态, t);
        if (!当前 || 当前->记录.记录节点 != 记录节点 || 当前->记录.方法投影关系 != 投影) {
            (void)追根因检查(false, L"任务方法选择发布后完整读回不一致。");
            return {任务方法选择提交状态::内部不一致, 当前};
        }
        return {任务方法选择提交状态::已提交, 当前};
    }

    节点句柄 创建任务() {
        return 节点_.创建节点(节点类型::任务, 主信息_.创建主信息());
    }

    节点句柄 按需求创建任务(节点句柄 需求节点, const 需求服务& 需求, 存在服务& 存在,
        const 场景服务& 场景, 状态服务& 状态, 节点句柄 任务场景, std::uint64_t 发生时间戳) {
        if (!场景.场景是否有效(任务场景)) {
            return {};
        }
        const auto 承接材料 = 需求.读取需求承接材料(需求节点);
        if (!承接材料.has_value() || !(承接材料->场景 == 任务场景)) {
            return {};
        }
        return 按需求创建任务(需求节点, 需求, 存在, 状态, 发生时间戳);
    }

    节点句柄 按需求创建任务(节点句柄 需求节点, const 需求服务& 需求, 存在服务& 存在,
        状态服务& 状态, std::uint64_t 发生时间戳) {
        if (!需求.需求是否有效(需求节点) || 发生时间戳 == 0) {
            return {};
        }
        const auto 承接材料 = 需求.读取需求承接材料(需求节点);
        if (!承接材料.has_value()
            || !节点类型匹配(承接材料->主体, 节点类型::存在)
            || !节点类型匹配(承接材料->目标宿主, 节点类型::存在)
            || !节点类型匹配(承接材料->场景, 节点类型::场景)
            || !节点类型匹配(承接材料->目标状态, 节点类型::状态)) {
            return {};
        }
        const auto 任务节点 = 创建任务();
        if (!追根因检查(句柄有效(任务节点), L"按需求创建任务时任务节点创建不及预期。")) {
            return {};
        }
        const auto 任务虚拟存在 = 存在.创建虚拟存在(任务节点);
        if (!追根因检查(句柄有效(任务虚拟存在), L"按需求创建任务时任务虚拟存在创建不及预期。")) {
            return {};
        }
        const auto 需求拥有任务关系 = 关系_.创建关系(关系类型::归属, 需求节点, 任务节点);
        const auto 任务来源需求关系 = 关系_.创建关系(关系类型::引用, 任务节点, 需求节点);
        const auto 任务虚拟存在关系 = 关系_.创建关系(关系类型::引用, 任务节点, 任务虚拟存在);
        const auto 任务运行场景关系 = 关系_.创建关系(关系类型::引用, 任务节点, 承接材料->场景);
        const auto 任务目标状态关系 = 关系_.创建关系(关系类型::模板, 任务节点, 承接材料->目标状态);
        if (!追根因检查(句柄有效(需求拥有任务关系)
            && 句柄有效(任务来源需求关系)
            && 句柄有效(任务虚拟存在关系)
            && 句柄有效(任务运行场景关系)
            && 句柄有效(任务目标状态关系),
            L"按需求创建任务时承接关系组写入不及预期。")) {
            return {};
        }
        const auto 创建状态 = 记录任务生命周期状态(
            任务节点, 任务虚拟存在, 承接材料->场景, 任务生命周期状态::已创建, 发生时间戳, 状态);
        if (!追根因检查(句柄有效(创建状态) && 任务承接壳完整(任务节点, 状态),
            L"按需求创建任务后创建状态或承接壳读回不符合内部预期。")) {
            return {};
        }
        return 任务节点;
    }

    节点句柄 记录任务状态(节点句柄 任务节点, std::uint64_t 发生时间戳, std::int64_t 状态值,
        状态服务& 状态, 节点句柄 场景, 节点句柄 存在) {
        if (!节点类型匹配(任务节点, 节点类型::任务)
            || !节点类型匹配(场景, 节点类型::场景)
            || !节点类型匹配(存在, 节点类型::存在)
            || 发生时间戳 == 0
            || 状态值 == static_cast<std::int64_t>(任务生命周期状态::未定义)) {
            return {};
        }
        const auto 状态节点 = 状态.创建实例状态(场景, 存在, 发生时间戳, 状态值);
        if (!追根因检查(句柄有效(状态节点), L"记录任务状态时状态节点创建不及预期。")) {
            return {};
        }
        const auto 任务状态关系 = 关系_.创建关系(关系类型::引用, 任务节点, 状态节点);
        if (!追根因检查(句柄有效(任务状态关系), L"记录任务状态时任务状态关系写入不及预期。")) {
            return {};
        }
        return 状态节点;
    }

    节点句柄 记录任务生命周期状态(节点句柄 任务节点, 节点句柄 任务虚拟存在, 节点句柄 场景,
        任务生命周期状态 生命周期状态, std::uint64_t 发生时间戳, 状态服务& 状态) {
        return 记录任务状态(任务节点, 发生时间戳, static_cast<std::int64_t>(生命周期状态),
            状态, 场景, 任务虚拟存在);
    }

    std::optional<节点句柄> 读取任务来源需求(节点句柄 任务节点, const 状态服务& 状态) const {
        if (!任务承接壳完整(任务节点, 状态)) {
            return std::nullopt;
        }
        return 读取唯一目标(任务节点, 关系类型::引用, 节点类型::需求);
    }

    std::optional<节点句柄> 读取任务虚拟存在(节点句柄 任务节点, const 状态服务& 状态) const {
        if (!任务承接壳完整(任务节点, 状态)) {
            return std::nullopt;
        }
        return 读取唯一目标(任务节点, 关系类型::引用, 节点类型::存在);
    }

    std::optional<节点句柄> 读取任务运行场景(节点句柄 任务节点, const 状态服务& 状态) const {
        if (!任务承接壳完整(任务节点, 状态)) {
            return std::nullopt;
        }
        return 读取唯一目标(任务节点, 关系类型::引用, 节点类型::场景);
    }

    std::optional<节点句柄> 读取任务目标状态(节点句柄 任务节点, const 状态服务& 状态) const {
        if (!任务承接壳完整(任务节点, 状态)) {
            return std::nullopt;
        }
        return 读取唯一目标(任务节点, 关系类型::模板, 节点类型::状态);
    }

    std::optional<节点句柄> 读取任务创建状态(节点句柄 任务节点, const 状态服务& 状态) const {
        if (!节点类型匹配(任务节点, 节点类型::任务)) {
            return std::nullopt;
        }
        return 读取唯一任务状态(任务节点, 状态, 任务生命周期状态::已创建);
    }

    std::optional<节点句柄> 读取任务完成状态(节点句柄 任务节点, const 状态服务& 状态) const {
        if (!节点类型匹配(任务节点, 节点类型::任务)) {
            return std::nullopt;
        }
        return 读取唯一任务状态(任务节点, 状态, 任务生命周期状态::已完成);
    }

    std::optional<任务承接材料> 读取任务承接材料(节点句柄 任务节点, const 状态服务& 状态) const {
        if (!任务承接壳完整(任务节点, 状态)) {
            return std::nullopt;
        }
        const auto 来源需求 = 读取唯一目标(任务节点, 关系类型::引用, 节点类型::需求);
        const auto 虚拟存在 = 读取唯一目标(任务节点, 关系类型::引用, 节点类型::存在);
        const auto 运行场景 = 读取唯一目标(任务节点, 关系类型::引用, 节点类型::场景);
        const auto 目标状态 = 读取唯一目标(任务节点, 关系类型::模板, 节点类型::状态);
        const auto 创建状态 = 读取唯一任务状态(任务节点, 状态, 任务生命周期状态::已创建);
        if (!来源需求.has_value()
            || !虚拟存在.has_value()
            || !运行场景.has_value()
            || !目标状态.has_value()
            || !创建状态.has_value()) {
            return std::nullopt;
        }
        return 任务承接材料{
            来源需求.value(),
            虚拟存在.value(),
            运行场景.value(),
            目标状态.value(),
            创建状态.value()
        };
    }

    std::vector<节点句柄> 读取需求承接任务组(节点句柄 需求节点, const 状态服务& 状态) const {
        std::vector<节点句柄> 任务组;
        if (!节点类型匹配(需求节点, 节点类型::需求)) {
            return 任务组;
        }
        for (const auto& 来源节点 : 关系_.获取来源节点组(需求节点, 关系类型::引用)) {
            if (!节点类型匹配(来源节点, 节点类型::任务)) {
                continue;
            }
            const auto 承接材料 = 读取任务承接材料(来源节点, 状态);
            if (承接材料.has_value() && 承接材料->来源需求 == 需求节点) {
                任务组.push_back(来源节点);
            }
        }
        std::sort(任务组.begin(), 任务组.end(), [](const auto& 左, const auto& 右) {
            if (左.仓库编号 != 右.仓库编号) {
                return 左.仓库编号 < 右.仓库编号;
            }
            if (左.节点编号 != 右.节点编号) {
                return 左.节点编号 < 右.节点编号;
            }
            return 左.版本号 < 右.版本号;
        });
        任务组.erase(std::unique(任务组.begin(), 任务组.end()), 任务组.end());
        return 任务组;
    }

    关系句柄 记录任务实际结果状态(节点句柄 任务节点, 节点句柄 实际结果状态, const 状态服务& 状态) {
        if (!任务承接壳完整(任务节点, 状态)
            || !节点类型匹配(实际结果状态, 节点类型::状态)
            || 读取任务实际结果状态(任务节点, 状态).has_value()) {
            return {};
        }
        return 关系_.创建关系(关系类型::引用, 任务节点, 实际结果状态, 任务实际结果状态顺序号);
    }

    std::optional<节点句柄> 读取任务实际结果状态(节点句柄 任务节点, const 状态服务& 状态) const {
        if (!任务承接壳完整(任务节点, 状态)) {
            return std::nullopt;
        }
        return 读取唯一目标(任务节点, 关系类型::引用, 节点类型::状态, 任务实际结果状态顺序号);
    }

    节点句柄 记录任务完成状态(节点句柄 任务节点, std::uint64_t 发生时间戳, 状态服务& 状态) {
        const auto 承接材料 = 读取任务承接材料(任务节点, 状态);
        if (!承接材料.has_value()
            || !读取任务实际结果状态(任务节点, 状态).has_value()
            || 发生时间戳 == 0) {
            return {};
        }
        return 记录任务生命周期状态(
            任务节点, 承接材料->虚拟存在, 承接材料->运行场景, 任务生命周期状态::已完成, 发生时间戳, 状态);
    }

    关系句柄 选择任务方法(节点句柄 任务节点, 节点句柄 方法节点, const 状态服务& 状态) {
        if (!任务承接壳完整(任务节点, 状态)
            || !节点类型匹配(方法节点, 节点类型::方法)
            || 读取任务选择方法(任务节点, 状态).has_value()) {
            return {};
        }
        return 关系_.创建关系(关系类型::引用, 任务节点, 方法节点, 任务选择方法顺序号);
    }

    std::optional<节点句柄> 读取任务选择方法(节点句柄 任务节点, const 状态服务& 状态) const {
        if (!任务承接壳完整(任务节点, 状态)) {
            return std::nullopt;
        }
        return 读取唯一目标(任务节点, 关系类型::引用, 节点类型::方法, 任务选择方法顺序号);
    }

    任务生命周期迁移请求材料 读取生命周期迁移请求材料(节点句柄 任务节点,
        任务生命周期状态 目标状态, const 状态服务& 状态) const {
        if (!节点类型匹配(任务节点, 节点类型::任务)) {
            return {任务节点, {}, 目标状态, 任务请求状态::无效任务};
        }
        if (!任务承接壳完整(任务节点, 状态) || 目标状态 == 任务生命周期状态::未定义) {
            return {任务节点, {}, 目标状态, 任务请求状态::承接壳不完整};
        }
        const auto 创建状态 = 读取任务创建状态(任务节点, 状态);
        return {任务节点, 创建状态.value_or(节点句柄{}), 目标状态, 任务请求状态::可作为请求材料};
    }

    任务筹办回执请求材料 读取筹办回执请求材料(节点句柄 任务节点, const 状态服务& 状态) const {
        const auto 承接材料 = 读取任务承接材料(任务节点, 状态);
        if (!承接材料.has_value()) {
            return {任务节点, {}, {}, 任务请求状态::承接壳不完整};
        }
        return {任务节点, 承接材料->来源需求, 承接材料->运行场景, 任务请求状态::可作为请求材料};
    }

    任务执行桥请求材料 读取执行桥请求材料(节点句柄 任务节点, const 状态服务& 状态) const {
        if (!任务承接壳完整(任务节点, 状态)) {
            return {任务节点, {}, 任务请求状态::承接壳不完整};
        }
        const auto 方法 = 读取任务选择方法(任务节点, 状态);
        if (!方法.has_value()) {
            return {任务节点, {}, 任务请求状态::缺少任务方法关系};
        }
        return {任务节点, 方法.value(), 任务请求状态::可作为请求材料};
    }

    任务非权威统计材料 读取运行统计材料(节点句柄 任务节点, const 状态服务& 状态) const {
        if (!任务承接壳完整(任务节点, 状态)) {
            return {任务节点, 0, 任务请求状态::承接壳不完整};
        }
        std::uint64_t 状态节点数 = 0;
        for (const auto& 目标 : 关系_.获取目标节点组(任务节点, 关系类型::引用)) {
            if (节点类型匹配(目标, 节点类型::状态) && 状态.读取状态值(目标).has_value()) {
                ++状态节点数;
            }
        }
        return {任务节点, 状态节点数, 任务请求状态::可作为请求材料};
    }

private:
    static constexpr std::int64_t 任务实际结果状态顺序号 = 20;
    static constexpr std::int64_t 任务选择方法顺序号 = 30;
    static constexpr std::int64_t 迁移证据首顺序号=40,迁移证据尾顺序号=46;
    static constexpr std::int64_t 选择记录格式标记 = 0x54504C31;

    static void 追加句柄(std::vector<std::int64_t>& v, 节点句柄 h) {
        v.push_back(static_cast<std::int64_t>(h.仓库编号));
        v.push_back(static_cast<std::int64_t>(h.节点编号));
        v.push_back(static_cast<std::int64_t>(h.版本号));
    }
    static void 追加句柄(std::vector<std::int64_t>& v, 关系句柄 h) {
        v.push_back(static_cast<std::int64_t>(h.仓库编号));
        v.push_back(static_cast<std::int64_t>(h.关系编号));
        v.push_back(static_cast<std::int64_t>(h.版本号));
    }
    static void 追加U64(std::vector<std::int64_t>& v, std::uint64_t x) {
        v.push_back(static_cast<std::int64_t>(static_cast<std::uint32_t>(x)));
        v.push_back(static_cast<std::int64_t>(static_cast<std::uint32_t>(x >> 32)));
    }
    static std::vector<std::int64_t> 编码选择请求(const 任务方法选择请求& r) {
        std::vector<std::int64_t> v{选择记录格式标记, static_cast<std::int64_t>(r.格式版本)};
        追加句柄(v, r.任务); 追加句柄(v, r.来源需求);
        追加句柄(v, r.承接授权.需求拥有任务关系); 追加句柄(v, r.承接授权.任务来源需求关系);
        追加句柄(v, r.生命周期关系); v.push_back(r.生命周期版本); 追加句柄(v, r.方法);
        追加U64(v, r.幂等材料编号); v.push_back(r.排序规则版本); v.push_back(r.召回请求规则版本);
        追加U64(v, r.索引发布版本); 追加U64(v, r.来源候选数量); 追加U64(v, r.重复输入数量);
        v.push_back(r.建议状态); v.push_back(static_cast<std::int64_t>(r.完整有序候选规范元组.size()));
        v.insert(v.end(), r.完整有序候选规范元组.begin(), r.完整有序候选规范元组.end());
        v.push_back(static_cast<std::int64_t>(r.权威节点来源组.size()));
        for (const auto& s : r.权威节点来源组) { 追加句柄(v, s.节点); v.push_back(static_cast<std::int64_t>(s.类型)); }
        v.push_back(static_cast<std::int64_t>(r.权威关系来源组.size()));
        for (const auto& s : r.权威关系来源组) {
            追加句柄(v, s.关系); v.push_back(static_cast<std::int64_t>(s.类型));
            追加句柄(v, s.源节点); 追加句柄(v, s.目标节点); v.push_back(s.顺序号);
        }
        return v;
    }

    struct 选择记录解码器 {
        const std::vector<std::int64_t>& 值组;
        std::size_t 索引 = 0;
        bool 读取(std::int64_t& x) { if (索引 >= 值组.size()) return false; x = 值组[索引++]; return true; }
        bool 读取节点(节点句柄& h) { std::int64_t a{},b{},c{}; if(!读取(a)||!读取(b)||!读取(c)||a<=0||b<=0||c<=0)return false;h={static_cast<std::uint64_t>(a),static_cast<std::uint64_t>(b),static_cast<std::uint32_t>(c)};return true; }
        bool 读取关系(关系句柄& h) { std::int64_t a{},b{},c{}; if(!读取(a)||!读取(b)||!读取(c)||a<=0||b<=0||c<=0)return false;h={static_cast<std::uint64_t>(a),static_cast<std::uint64_t>(b),static_cast<std::uint32_t>(c)};return true; }
        bool 读取U64(std::uint64_t& x) { std::int64_t a{},b{};if(!读取(a)||!读取(b)||a<0||b<0||a>std::numeric_limits<std::uint32_t>::max()||b>std::numeric_limits<std::uint32_t>::max())return false;x=static_cast<std::uint32_t>(a)|(static_cast<std::uint64_t>(static_cast<std::uint32_t>(b))<<32);return true; }
    };

    static std::optional<std::pair<任务方法选择请求, 关系句柄>> 解码选择记录(
        const std::vector<std::int64_t>& 值组) {
        选择记录解码器 d{值组}; 任务方法选择请求 r; 关系句柄 投影; std::int64_t x{};
        if(!d.读取(x)||x!=选择记录格式标记||!d.读取(x)||x!=1)return std::nullopt;r.格式版本=1;
        if(!d.读取节点(r.任务)||!d.读取节点(r.来源需求))return std::nullopt;
        r.承接授权.任务=r.任务;r.承接授权.需求=r.来源需求;
        if(!d.读取关系(r.承接授权.需求拥有任务关系)||!d.读取关系(r.承接授权.任务来源需求关系)
            ||!d.读取关系(r.生命周期关系)||!d.读取(x)||x<=0)return std::nullopt;r.生命周期版本=static_cast<std::uint32_t>(x);
        if(!d.读取节点(r.方法)||!d.读取U64(r.幂等材料编号)||!d.读取(x)||x<=0)return std::nullopt;r.排序规则版本=static_cast<std::uint32_t>(x);
        if(!d.读取(x)||x<=0)return std::nullopt;r.召回请求规则版本=static_cast<std::uint32_t>(x);
        if(!d.读取U64(r.索引发布版本)||!d.读取U64(r.来源候选数量)||!d.读取U64(r.重复输入数量)||!d.读取(x)||x<=0)return std::nullopt;r.建议状态=static_cast<std::uint32_t>(x);
        if(!d.读取(x)||x<0||static_cast<std::uint64_t>(x)>值组.size())return std::nullopt;
        for(std::int64_t i=0;i<x;++i){std::int64_t y{};if(!d.读取(y))return std::nullopt;r.完整有序候选规范元组.push_back(y);}
        if(!d.读取(x)||x<0||static_cast<std::uint64_t>(x)>值组.size())return std::nullopt;const auto 节点来源数量=x;
        for(std::int64_t i=0;i<节点来源数量;++i){任务选择权威节点来源 s;std::int64_t 类型{};if(!d.读取节点(s.节点)||!d.读取(类型))return std::nullopt;s.类型=static_cast<节点类型>(类型);r.权威节点来源组.push_back(s);}
        if(!d.读取(x)||x<0||static_cast<std::uint64_t>(x)>值组.size())return std::nullopt;const auto 关系来源数量=x;
        for(std::int64_t i=0;i<关系来源数量;++i){任务选择权威关系来源 s;std::int64_t 类型{};if(!d.读取关系(s.关系)||!d.读取(类型))return std::nullopt;s.类型=static_cast<关系类型>(类型);if(!d.读取节点(s.源节点)||!d.读取节点(s.目标节点)||!d.读取(s.顺序号))return std::nullopt;r.权威关系来源组.push_back(s);}
        if(!d.读取关系(投影)||d.索引!=值组.size())return std::nullopt;
        return std::pair<任务方法选择请求,关系句柄>{std::move(r),投影};
    }

    bool 选择请求形状完整(const 任务方法选择请求& r) const {
        return r.格式版本 == 1 && 句柄有效(r.任务) && 句柄有效(r.来源需求)
            && r.承接授权.任务 == r.任务 && r.承接授权.需求 == r.来源需求
            && 句柄有效(r.承接授权.需求拥有任务关系) && 句柄有效(r.承接授权.任务来源需求关系)
            && 句柄有效(r.生命周期关系) && r.生命周期版本 != 0 && 句柄有效(r.方法)
            && r.幂等材料编号 != 0 && r.排序规则版本 != 0 && r.召回请求规则版本 != 0
            && r.索引发布版本 != 0 && r.来源候选数量 != 0
            && r.建议状态 == 4
            && r.完整有序候选规范元组.size() >= r.来源候选数量
            && !r.权威节点来源组.empty() && !r.权威关系来源组.empty();
    }

    bool 复核权威来源(const 任务方法选择请求& r, const 结构事务令牌& t) const {
        for(const auto& s:r.权威节点来源组){const auto n=节点_.读取节点(s.节点,t);if(!n||n->类型!=s.类型)return false;}
        for(const auto& s:r.权威关系来源组){const auto q=关系_.读取关系(s.关系,t);if(!q||q->类型!=s.类型||q->源节点!=s.源节点||q->目标节点!=s.目标节点||q->顺序号!=s.顺序号)return false;}
        return true;
    }

    std::optional<任务方法选择记录快照> 读取选择记录(
        节点句柄 记录节点, std::optional<关系句柄> 发布关系, const 结构事务令牌& t) const {
        const auto n=节点_.读取节点(记录节点,t);if(!n||n->类型!=节点类型::任务方法选择记录)return std::nullopt;
        const auto m=主信息_.读取主信息(n->主信息,t);if(!m)return std::nullopt;
        std::vector<std::int64_t> v;v.reserve(m->值容器.size());for(const auto& x:m->值容器){if(!x)return std::nullopt;v.push_back(*x);}
        const auto decoded=解码选择记录(v);if(!decoded)return std::nullopt;
        if(!发布关系){for(const auto& p:关系_.获取关系审计记录组(decoded->first.任务,关系类型::任务方法选择,t))if(p.目标节点==记录节点){if(发布关系)return std::nullopt;const auto ver=p.状态==记录状态::已失效&&p.版本号>1?p.版本号-1:p.版本号;发布关系=关系句柄{decoded->first.任务.仓库编号,p.关系编号,ver};}}
        if(!发布关系)return std::nullopt;
        std::optional<关系记录> 投影审计;
        for(const auto& p:关系_.获取关系审计记录组(decoded->first.任务,关系类型::引用,t)){
            if(p.关系编号!=decoded->second.关系编号)continue;
            if(投影审计)return std::nullopt;
            投影审计=p;
        }
        if(!投影审计||投影审计->源节点!=decoded->first.任务||投影审计->目标节点!=decoded->first.方法||投影审计->顺序号!=任务选择方法顺序号)return std::nullopt;
        return 任务方法选择记录快照{记录节点,*发布关系,decoded->second,decoded->first};
    }

    bool 失效当前选择(节点句柄 任务, const 状态服务&, const 结构事务令牌& t) {
        const auto 发布组=关系_.获取关系记录组(任务,关系类型::任务方法选择,t);
        if(发布组.empty())return true;
        if(发布组.size()!=1)return false;
        const auto 发布=关系句柄{任务.仓库编号,发布组[0].关系编号,发布组[0].版本号};
        const auto 记录=读取选择记录(发布组[0].目标节点,发布,t);
        if(!记录||记录->请求.任务!=任务)return false;
        const auto p=关系_.失效关系(记录->方法投影关系,t);if(p.状态!=关系状态变更状态::已变更||!p.完整())return false;
        const auto r=关系_.失效关系(发布,t);return r.状态==关系状态变更状态::已变更&&r.完整();
    }
    static bool 生命周期值有效(std::int64_t v){return v>=1&&v<=10;}
    static bool 是终态(任务生命周期状态 s){return s==任务生命周期状态::已完成||s==任务生命周期状态::失败||s==任务生命周期状态::取消;}
    static bool 合法普通迁移(任务生命周期状态 a,任务生命周期状态 b){return(a==任务生命周期状态::已创建&&b==任务生命周期状态::已承接)||(a==任务生命周期状态::已承接&&b==任务生命周期状态::筹办中)||(a==任务生命周期状态::筹办中&&b==任务生命周期状态::排队中)||(a==任务生命周期状态::排队中&&b==任务生命周期状态::执行中)||(a==任务生命周期状态::执行中&&b==任务生命周期状态::等待中)||(a==任务生命周期状态::等待中&&b==任务生命周期状态::执行中)||((a==任务生命周期状态::排队中||a==任务生命周期状态::执行中||a==任务生命周期状态::等待中)&&b==任务生命周期状态::待重筹办)||(a==任务生命周期状态::待重筹办&&b==任务生命周期状态::筹办中);}
    bool 类型匹配令牌(节点句柄 n,std::initializer_list<节点类型> ts,const 结构事务令牌& t)const{if(!句柄有效(n))return true;auto r=节点_.读取节点(n,t);return r&&std::find(ts.begin(),ts.end(),r->类型)!=ts.end();}
    bool 证据形状有效(const 任务生命周期迁移证据& e,const 结构事务令牌& t)const{return 类型匹配令牌(e.来源任务,{节点类型::任务},t)&&类型匹配令牌(e.来源方法,{节点类型::方法},t)&&类型匹配令牌(e.来源动作,{节点类型::方法},t)&&类型匹配令牌(e.来源状态,{节点类型::状态},t)&&类型匹配令牌(e.来源动态,{节点类型::动态},t)&&类型匹配令牌(e.取消授权来源,{节点类型::任务,节点类型::方法},t)&&类型匹配令牌(e.动作停止证据,{节点类型::状态,节点类型::动态},t);}
    std::optional<任务生命周期迁移证据> 读取迁移证据(节点句柄 s,const 结构事务令牌& t)const{任务生命周期迁移证据 e;std::array<bool,7> seen{};for(const auto& r:关系_.获取关系记录组(s,关系类型::任务生命周期证据,t)){if(r.顺序号<40||r.顺序号>46)return std::nullopt;auto i=static_cast<std::size_t>(r.顺序号-40);if(seen[i])return std::nullopt;seen[i]=true;switch(i){case 0:e.来源任务=r.目标节点;break;case 1:e.来源方法=r.目标节点;break;case 2:e.来源动作=r.目标节点;break;case 3:e.来源状态=r.目标节点;break;case 4:e.来源动态=r.目标节点;break;case 5:e.取消授权来源=r.目标节点;break;case 6:e.动作停止证据=r.目标节点;break;}}return 证据形状有效(e,t)?std::optional<任务生命周期迁移证据>{e}:std::nullopt;}
    std::vector<关系句柄> 创建证据关系(节点句柄 s,const 任务生命周期迁移证据& e,const 结构事务令牌& t){const std::array<节点句柄,7> a{e.来源任务,e.来源方法,e.来源动作,e.来源状态,e.来源动态,e.取消授权来源,e.动作停止证据};std::vector<关系句柄> o;for(std::size_t i=0;i<a.size();++i)if(句柄有效(a[i]))o.push_back(关系_.创建关系(关系类型::任务生命周期证据,s,a[i],40+static_cast<std::int64_t>(i),t));if(!std::all_of(o.begin(),o.end(),[](auto x){return 句柄有效(x);})){清关系(o,t);return {};}return o;}
    static bool 请求与历史同义(const 任务生命周期提交请求& r,const 任务生命周期历史项& h){return h.前驱关系==std::optional<关系句柄>{r.预期当前关系}&&r.预期生命周期版本+1==h.版本&&r.目标阶段==h.阶段&&r.发生时间戳==h.时间戳&&r.证据==h.证据;}
    任务生命周期提交结果 提交生命周期(const 任务生命周期提交请求& r,状态服务& s,const 结构事务令牌& t,bool fail,bool cancel){
        if(!验证独占(t)||r.幂等材料编号==0||r.发生时间戳==0||r.预期生命周期版本==0||!句柄有效(r.任务)||!句柄有效(r.预期当前关系)||!证据形状有效(r.证据,t))return{};auto hist=读取任务生命周期历史(r.任务,s,t);if(hist.empty())return{};std::vector<任务生命周期历史项> hits;for(const auto& h:hist)if(h.幂等材料编号==std::optional<std::uint64_t>{r.幂等材料编号})hits.push_back(h);auto cur=读取任务生命周期当前材料(r.任务,s,t);if(!cur)return{};if(hits.size()>1){(void)追根因检查(false,L"同一任务生命周期幂等编号历史不唯一。");return{任务生命周期提交状态::内部不一致,std::nullopt,cur};}if(hits.size()==1){if(!请求与历史同义(r,hits[0]))return{任务生命周期提交状态::已拒绝,std::nullopt,cur};return{任务生命周期提交状态::幂等读回,hits[0],cur};}
        if(cur->关系!=r.预期当前关系||cur->版本!=r.预期生命周期版本||是终态(cur->阶段))return{任务生命周期提交状态::已拒绝,std::nullopt,cur};if(fail){if(r.目标阶段!=任务生命周期状态::失败)return{};}else if(cancel){if(r.目标阶段!=任务生命周期状态::取消||(cur->阶段==任务生命周期状态::执行中&&!句柄有效(r.证据.动作停止证据)))return{};}else if(r.目标阶段==任务生命周期状态::已完成||r.目标阶段==任务生命周期状态::失败||r.目标阶段==任务生命周期状态::取消||!合法普通迁移(cur->阶段,r.目标阶段))return{};
        auto c=s.创建实例状态候选(cur->场景,cur->主体,r.发生时间戳,static_cast<std::int64_t>(r.目标阶段),r.幂等材料编号,t);if(!c)return{};auto er=创建证据关系(c->读取状态(),r.证据,t);const auto need=static_cast<std::size_t>(句柄有效(r.证据.来源任务))+static_cast<std::size_t>(句柄有效(r.证据.来源方法))+static_cast<std::size_t>(句柄有效(r.证据.来源动作))+static_cast<std::size_t>(句柄有效(r.证据.来源状态))+static_cast<std::size_t>(句柄有效(r.证据.来源动态))+static_cast<std::size_t>(句柄有效(r.证据.取消授权来源))+static_cast<std::size_t>(句柄有效(r.证据.动作停止证据));if(er.size()!=need){(void)s.撤销候选(*c,t);return{};}auto nr=关系_.创建关系(关系类型::任务生命周期,r.任务,c->读取状态(),static_cast<std::int64_t>(cur->版本+1),t);auto sm=s.读取状态材料(c->读取状态(),t);auto id=s.读取状态幂等编号(c->读取状态(),t);auto ev=读取迁移证据(c->读取状态(),t);if(!句柄有效(nr)||!sm||id!=std::optional<std::uint64_t>{r.幂等材料编号}||!ev||*ev!=r.证据){if(句柄有效(nr))(void)关系_.删除关系(nr,t);清关系(er,t);(void)s.撤销候选(*c,t);return{};}if(cur->阶段==任务生命周期状态::待重筹办&&r.目标阶段==任务生命周期状态::筹办中&&!失效当前选择(r.任务,s,t)){(void)追根因检查(false,L"任务重筹办失效旧方法选择不及预期。");return{任务生命周期提交状态::内部不一致,std::nullopt,std::nullopt};}auto off=关系_.失效关系(cur->关系,t);if(off.状态!=关系状态变更状态::已变更||!off.完整()){(void)追根因检查(false,L"任务生命周期唯一提交点失效旧关系不及预期。");return{任务生命周期提交状态::内部不一致,std::nullopt,std::nullopt};}auto nc=读取任务生命周期当前材料(r.任务,s,t);auto nh=读取任务生命周期历史(r.任务,s,t);if(!nc||nc->关系!=nr||nc->版本!=cur->版本+1||nh.size()!=hist.size()+1||s.确认候选(*c,t)!=状态候选操作状态::已完成){(void)追根因检查(false,L"任务生命周期提交后完整读回不一致。");return{任务生命周期提交状态::内部不一致,std::nullopt,nc};}return{任务生命周期提交状态::已提交,nh.back(),nc};}
    bool 验证独占(const 结构事务令牌& t) const{return 接线_.已接域()&&接线_.验证独占令牌(接线_.运行期状态,t);}
    std::optional<节点句柄> 唯一目标(节点句柄 s,关系类型 k,节点类型 n,const 结构事务令牌& t) const{std::optional<节点句柄> r;for(auto x:关系_.获取目标节点组(s,k,t)){auto q=节点_.读取节点(x,t);if(!q||q->类型!=n)continue;if(r)return std::nullopt;r=x;}return r;}
    void 清关系(const std::vector<关系句柄>& v,const 结构事务令牌& t){for(auto i=v.rbegin();i!=v.rend();++i)if(句柄有效(*i))(void)关系_.删除关系(*i,t);}
    void 清任务(节点句柄 n,主信息句柄 m,const std::vector<关系句柄>& v,const 结构事务令牌& t){清关系(v,t);if(句柄有效(n))(void)节点_.删除节点(n,t);if(句柄有效(m))(void)主信息_.删除主信息(m,t);}

    bool 任务承接壳完整(节点句柄 任务节点, const 状态服务& 状态) const {
        return 读取唯一目标(任务节点, 关系类型::引用, 节点类型::需求).has_value()
            && 读取唯一目标(任务节点, 关系类型::引用, 节点类型::存在).has_value()
            && 读取唯一目标(任务节点, 关系类型::引用, 节点类型::场景).has_value()
            && 读取唯一目标(任务节点, 关系类型::模板, 节点类型::状态).has_value()
            && 读取唯一任务状态(任务节点, 状态, 任务生命周期状态::已创建).has_value();
    }

    std::optional<节点句柄> 读取唯一目标(节点句柄 源节点, 关系类型 类型, 节点类型 目标类型) const {
        return 读取唯一目标(源节点, 类型, 目标类型, std::nullopt);
    }

    std::optional<节点句柄> 读取唯一目标(节点句柄 源节点, 关系类型 类型, 节点类型 目标类型,
        std::optional<std::int64_t> 顺序号) const {
        if (!节点类型匹配(源节点, 节点类型::任务)) {
            return std::nullopt;
        }
        const auto 目标组 = 顺序号.has_value()
            ? 关系_.获取目标节点组(源节点, 类型, 顺序号.value())
            : 关系_.获取目标节点组(源节点, 类型);
        std::optional<节点句柄> 唯一目标;
        for (const auto& 目标 : 目标组) {
            if (!节点类型匹配(目标, 目标类型)) {
                continue;
            }
            if (唯一目标.has_value()) {
                return std::nullopt;
            }
            唯一目标 = 目标;
        }
        return 唯一目标;
    }

    std::optional<节点句柄> 读取唯一任务状态(节点句柄 任务节点, const 状态服务& 状态,
        任务生命周期状态 生命周期状态) const {
        if (!节点类型匹配(任务节点, 节点类型::任务)) {
            return std::nullopt;
        }
        const auto 目标组 = 关系_.获取目标节点组(任务节点, 关系类型::引用);
        std::optional<节点句柄> 唯一状态;
        for (const auto& 目标 : 目标组) {
            if (!节点类型匹配(目标, 节点类型::状态)) {
                continue;
            }
            const auto 状态值 = 状态.读取状态值(目标);
            if (!状态值.has_value()
                || 状态值.value() != static_cast<std::int64_t>(生命周期状态)) {
                continue;
            }
            if (唯一状态.has_value()) {
                return std::nullopt;
            }
            唯一状态 = 目标;
        }
        return 唯一状态;
    }

    bool 节点类型匹配(节点句柄 节点句柄值, 节点类型 类型) const {
        const auto 记录 = 节点_.读取节点(节点句柄值);
        return 记录.has_value() && 记录->类型 == 类型;
    }

    主信息仓库& 主信息_;
    节点仓库& 节点_;
    关系仓库& 关系_;
    结构事务接线 接线_;
};

}
