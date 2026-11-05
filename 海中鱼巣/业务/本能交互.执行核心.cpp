#include "本能交互.执行核心.h"
#include "../领域/新_方法参数场景.h"
#include <stdexcept>
#include <algorithm>
#include <array>
#include <utility>
namespace 海中鱼巣 {
namespace {
class 隔离连接后端 final : public 新连接执行后端 {
    const 方法参数场景& 场景;
    新方法隔离执行接口& 接口;
    新连接执行现场& 现场;
    新交互绑定 绑定;
    新消息连接输入构成 构成;
public:
    隔离连接后端(const 方法参数场景& s,新方法隔离执行接口& i,
        新连接执行现场& q,新交互绑定 b,新消息连接输入构成 c)
        : 场景(s),接口(i),现场(q),绑定(b),构成(c) {}
    bool 已请求停止() const override { return 接口.请求停止已成立(); }
    新连接动作读回 建立动作(const 方法参数场景& s) override {
        const auto f=s.读取绑定特征(绑定.交互者特征);
        if (!f) throw std::logic_error("connection bound interaction feature unavailable");
        return {接口.建立阶段动作(s,{2,f->特征概念节点,新隔离动作阶段::主动作}),true};
    }
    std::optional<新状态信息> 读取最新接收数量状态() override {
        return 接口.读取最新特征状态(绑定.外设场景,绑定.窗口,绑定.接收数量特征);
    }
    新二次特征计算结果 比较状态(const 新状态信息& 最新,const 新状态信息& 上轮) const override {
        return 接口.比较接收数量状态(最新,上轮);
    }
    新交互写入读回 写入交互者(稳定编码 a) override {
        if (!现场.关系发布许可已冻结 || !现场.基准状态 || !现场.更新状态)
            throw std::logic_error("connection publication lacks receive-state evidence");
        return 接口.写入本次连接交互者(场景,
            {绑定,现场.身份,a,现场.基准状态->节点,现场.更新状态->节点});
    }
    稳定编码 读取连接关系() const override { return 构成.关系概念; }
};
}

新本能函数本次条件结果对 执行隔离连接核心(const 方法参数场景& s) {
    新本能函数本次条件结果对 r;
    auto* w=s.获取模拟运行接口();
    auto* i=w ? w->获取隔离执行接口() : nullptr;
    if (!i) { r.结果.状态=新本能函数执行状态::依赖未装配; return r; }
    std::array<稳定编码,6> p{};
    for (std::uint32_t n=1;n<=p.size();++n) {
        const auto* v=s.查询参数(n);
        if (!v) throw std::logic_error("connection ordered binding unavailable");
        p[n-1]=v->信息节点;
    }
    const auto 外设=std::find_if(s.全部角色绑定().begin(),s.全部角色绑定().end(),[](const auto& b) {
        return b.有序引用 && b.有序引用->序号==5;
    });
    if (外设==s.全部角色绑定().end() || !外设->世界场景节点)
        throw std::logic_error("connection peripheral scene binding unavailable");
    const auto c=i->读取连接构成();
    if (c.状态!=新交互求值状态::已取得 || !c.构成)
        throw std::logic_error("connection declared composition unavailable");
    const auto n=i->读取当前调用身份();
    新连接执行现场 q;
    q.身份=n.调用节点; q.方法身份=n.方法节点; q.绑定=s.全部角色绑定();
    const 新交互绑定 b{p[0],p[1],p[2],p[3],p[4],p[5],*外设->世界场景节点};
    隔离连接后端 后端(s,*i,q,b,*c.构成);
    for (;;) {
        const auto 前=i->读取进展序号();
        推进连接核心(s,q,后端);
        if (q.阶段!=新连接状态::待接收状态更新 && q.阶段!=新连接状态::待关系发布)
            return q.运行;
        if (后端.已请求停止()) continue;
        // 环境事件在首次读取基准之后交付，不冒充方法的结果。
        const auto 输入=w->推进模拟输入();
        if (前==i->读取进展序号()
            && (输入==新模拟操作状态::输入耗尽 || 输入==新模拟操作状态::等待触发))
            return q.运行;
    }
}

新本能函数本次条件结果对 推进连接核心(const 方法参数场景& 场景,
    新连接执行现场& q,新连接执行后端& 后端) {
    const auto 停止=[&] {
        if (!后端.已请求停止()) return false;
        q.阶段=新连接状态::已取消;
        q.运行.结果.状态=q.动作 ? 新本能函数执行状态::动作结果未知 : 新本能函数执行状态::入口拒绝;
        q.运行.结果.结果取得状态=q.动作 ? 新本能函数结果取得状态::部分取得 : 新本能函数结果取得状态::未取得;
        return true;
    };
    if (停止() || q.阶段==新连接状态::已确认关系) return q.运行;
    if (!q.基准状态) {
        q.运行.条件.执行分支节点=场景.执行分支节点();
        for (const auto& b:q.绑定) {
            新本能函数条件角色材料 材料;
            材料.有序引用=b.有序引用; 材料.世界场景节点=b.世界场景节点;
            if (b.有序引用 && b.有序引用->信息类型==新方法参数信息类型::特征) {
                const auto f=场景.读取绑定特征(b.有序引用->信息节点);
                if (!f) throw std::logic_error("connection bound feature unavailable");
                材料.实际特征组.push_back({f->节点,f->当前值});
            }
            q.运行.条件.参数角色组.push_back(std::move(材料));
        }
        q.基准状态=后端.读取最新接收数量状态();
        if (!q.基准状态) {
            q.阶段=新连接状态::前提已变化;
            q.运行.结果.状态=新本能函数执行状态::入口拒绝;
            return q.运行;
        }
        q.运行.结果.状态=新本能函数执行状态::异步已受理;
        q.运行.结果.结果取得状态=新本能函数结果取得状态::待取得;
        return q.运行; // 历史接收仅作基准，不据此确认本次连接。
    }
    if (q.阶段==新连接状态::待接收状态更新) {
        const auto 最新=后端.读取最新接收数量状态();
        if (!最新) throw std::logic_error("connection receive-count state disappeared");
        if (最新->节点==q.基准状态->节点) return q.运行;
        if (最新->来源实例特征节点!=q.基准状态->来源实例特征节点
            || 最新->被描述存在节点!=q.基准状态->被描述存在节点
            || 最新->强时间.语义!=q.基准状态->强时间.语义
            || 最新->强时间.纳秒<=q.基准状态->强时间.纳秒)
            throw std::logic_error("connection receive-state source or time inconsistent");
        const auto 比较=后端.比较状态(*最新,*q.基准状态);
        if (比较.状态!=新二次特征计算状态::已完成 || !比较.结果值)
            throw std::logic_error("connection secondary receive-state calculation failed");
        if (*比较.结果值==0) { q.基准状态=最新; return q.运行; }
        q.更新状态=最新;
        q.关系发布许可已冻结=true;
        q.阶段=新连接状态::待关系发布;
    }
    if (q.阶段==新连接状态::待关系发布) {
        if (停止()) return q.运行;
        if (!q.动作) {
            const auto a=后端.建立动作(场景);
            q.动作=a.动作;
            if (q.动作) {
                q.运行.结果.动作动态节点=q.动作;
                q.运行.结果.动作阶段=新本能函数动作阶段::已进入动作;
                q.运行.结果.状态=新本能函数执行状态::动作结果未知;
                q.运行.结果.结果取得状态=新本能函数结果取得状态::部分取得;
            }
            if (!a.前提仍具备) {
                q.阶段=新连接状态::前提已变化;
                q.运行.结果.状态=q.动作 ? 新本能函数执行状态::动作结果未知 : 新本能函数执行状态::入口拒绝;
                q.运行.结果.结果取得状态=q.动作 ? 新本能函数结果取得状态::部分取得 : 新本能函数结果取得状态::未取得;
                return q.运行;
            }
            if (!q.动作) throw std::logic_error("connection action was not recorded");
        }
        if (停止()) return q.运行;
        const auto 写回=后端.写入交互者(*q.动作);
        if (写回.状态==新交互求值状态::前提变化) {
            q.阶段=新连接状态::前提已变化;
            q.运行.结果.状态=新本能函数执行状态::动作结果未知;
            q.运行.结果.结果取得状态=新本能函数结果取得状态::部分取得;
            return q.运行;
        }
        if (写回.状态!=新交互求值状态::已取得)
            throw std::logic_error("connection interaction feature publication failed");
        if (写回.新状态节点) q.运行.结果.已形成状态节点组.push_back(*写回.新状态节点);
        if (写回.变化动态节点) q.运行.结果.已形成动态节点组.push_back(*写回.变化动态节点);
        const auto* f=场景.查询参数(4);
        if (!f) throw std::logic_error("connection target binding unavailable");
        q.运行.结果.建议观察特征节点组={f->信息节点};
        q.运行.结果.已取得场景事实组={{{后端.读取连接关系(),1,2,3,4},q.身份,*q.动作}};
        q.运行.结果.状态=新本能函数执行状态::已按合同返回;
        q.运行.结果.动作阶段=新本能函数动作阶段::已按函数合同返回;
        q.运行.结果.结果取得状态=新本能函数结果取得状态::已取得;
        q.阶段=新连接状态::已确认关系;
    }
    return q.运行;
}

新本能函数本次条件结果对 执行服务对象核心(const 方法参数场景& 场景,
    新本能交互运行接口& 后端,const 新方法初始必要条件定义& 必要条件) {
    新本能函数本次条件结果对 运行;
    运行.结果.状态=新本能函数执行状态::参数不匹配;
    // v3：第5参数为服务对象特征，写回目标是它而非交互者（R93）。
    const std::array<新方法参数信息类型,5> 参数类型{{
        新方法参数信息类型::存在,新方法参数信息类型::存在,
        新方法参数信息类型::存在,新方法参数信息类型::特征,新方法参数信息类型::特征}};
    if (场景.全部角色绑定().size()!=参数类型.size()) return 运行;
    for (std::uint32_t i=0;i<参数类型.size();++i) {
        const auto* 参数=场景.查询参数(i+1);
        if (!参数 || 参数->信息类型!=参数类型[i] || !有效(参数->信息节点)) return 运行;
    }
    // 参数形状正确但必要条件尚未具备，不是参数规格错误。
    运行.结果.状态=新本能函数执行状态::入口拒绝;
    运行.结果.结果取得状态=新本能函数结果取得状态::待取得;
    新交互绑定 绑定;
    const auto 复核=[&]() {
        if (后端.已请求停止()) {
            if (运行.结果.动作阶段==新本能函数动作阶段::未进入动作)
                运行.结果.结果取得状态=新本能函数结果取得状态::未取得;
            return false;
        }
        const auto 本次必要条件=后端.读取本次必要条件(场景);
        if (!本次必要条件 || *本次必要条件!=必要条件) return false;
        if (后端.读取绑定环境(场景,绑定)!=新交互求值状态::已取得) return false;
        for (const auto& 条件:必要条件.条件.条件项组) {
            const auto* 参数=场景.查询参数(条件.参数序号);
            if (!参数||!条件.存在概念节点
                ||后端.核验绑定存在概念(参数->信息节点,*条件.存在概念节点)!=新存在概念匹配状态::匹配) return false;
        }
        for (const auto& 限定:必要条件.场景事实限定组)
            if (后端.读取场景事实(限定,场景.全部角色绑定())!=新交互求值状态::已取得) return false;
        return 绑定.自我!=绑定.对端;
    };
    if (!复核()) return 运行;
    运行.条件.执行分支节点=场景.执行分支节点();
    for (const auto& 项:场景.全部角色绑定()) {
        新本能函数条件角色材料 条件;
        条件.有序引用=项.有序引用; 条件.世界场景节点=项.世界场景节点;
        if (项.有序引用&&项.有序引用->信息类型==新方法参数信息类型::特征) {
            const auto F=后端.读取交互者(项.有序引用->信息节点);
            if (!F) return 运行;
            条件.实际特征组.push_back({F->节点,F->当前值});
        }
        运行.条件.参数角色组.push_back(std::move(条件));
    }
    if (!复核()) return 运行;
    // 写回目标是容器中的参数5——自我的“服务对象”特征（R93）；交互者特征（参数4）只作连接条件。
    const auto* 结果参数=场景.查询参数(5);
    if (!结果参数) return 运行;
    auto 写回绑定=绑定;
    写回绑定.交互者特征=结果参数->信息节点;
    const auto 动作=后端.建立本次动作(场景,写回绑定);
    if (!有效(动作)) return 运行;
    运行.结果.动作动态节点=动作;
    运行.结果.动作阶段=新本能函数动作阶段::已进入动作;
    运行.结果.状态=新本能函数执行状态::动作结果未知;
    运行.结果.结果取得状态=新本能函数结果取得状态::部分取得;
    if (!复核()) return 运行;
    const auto 写回=后端.写入交互者(写回绑定,动作);
    if (写回.状态!=新交互求值状态::已取得) return 运行;
    const auto F=后端.读取交互者(写回绑定.交互者特征);
    if (!F||F->当前值!=std::optional<新特征准确值>{新特征准确值{写回绑定.对端}})
        throw std::logic_error("service object feature writeback differs after successful publication");
    if (写回.新状态节点) 运行.结果.已形成状态节点组.push_back(*写回.新状态节点);
    if (写回.变化动态节点) 运行.结果.已形成动态节点组.push_back(*写回.变化动态节点);
    运行.结果.建议观察特征节点组={写回绑定.交互者特征};
    运行.结果.状态=新本能函数执行状态::已按合同返回;
    运行.结果.动作阶段=新本能函数动作阶段::已按函数合同返回;
    运行.结果.结果取得状态=新本能函数结果取得状态::已取得;
    return 运行;
}
}
