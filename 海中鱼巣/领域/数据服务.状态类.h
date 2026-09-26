#pragma once

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <limits>
#include <new>
#include <optional>
#include <stdexcept>
#include <utility>
#include <variant>
#include <vector>
#include "../核心/L1事实基座.数据.h"

#include "数据服务.特征类.h"

namespace 海中鱼巣 {

inline constexpr std::uint32_t 状态类数据合同版本 = 2;

struct 状态信息身份 final {
    稳定编码 编码{};
    friend bool operator==(const 状态信息身份&, const 状态信息身份&) = default;
};
inline bool 有效(状态信息身份 v) noexcept { return 有效(v.编码); }

enum class 状态强时间语义 : std::uint8_t { 实例绝对UTC纳秒 = 1, 抽象相对纳秒 = 2 };
struct 状态强时间 final {
    状态强时间语义 语义 = 状态强时间语义::实例绝对UTC纳秒;
    std::int64_t 纳秒 = 0;
    friend bool operator==(const 状态强时间&, const 状态强时间&) = default;
};
using 状态固定准确值 = 特征准确值;
struct 状态信息 final {
    状态信息身份 身份;
    特征类型身份 正式特征类型;
    状态固定准确值 固定准确值;
    状态强时间 强时间;
    friend bool operator==(const 状态信息&, const 状态信息&) = default;
};

enum class 状态类数据状态 : std::uint8_t {
    已创建 = 1, 精确重复 = 2, 已读取 = 3, 已删除 = 4,
    入口拒绝 = 6, 未找到 = 7,
    正式特征类型未找到 = 9, 准确值不相容 = 11,
    旧格式不支持 = 12, 事实代次漂移 = 13,
    幂等冲突 = 14, 引用冲突 = 15, 数量预算不足 = 16,
    资源失败 = 18, 内部不一致 = 19
};

struct 状态类结构交付 final {
    稳定编码 状态族锚点;
    稳定编码 状态族归属关系类型;
    稳定编码 正式特征类型关系类型;
    稳定编码 内联准确值属性类型;
    稳定编码 引用准确值属性类型;
    稳定编码 实例绝对时间属性类型;
    稳定编码 抽象相对时间属性类型;
    稳定编码 首次形成UTC属性类型;
};
struct 状态类结果头 final {
    状态类数据状态 状态 = 状态类数据状态::入口拒绝;
    std::uint32_t 合同版本 = 状态类数据合同版本;
    std::uint64_t 事实截止代次 = 0;
    std::optional<std::uint64_t> 变更事实代次;
};
struct 状态内容事实 final {
    std::uint64_t Gread = 0;
    状态信息 信息;
    稳定编码 族归属关系, 正式特征类型关系, 准确值事实, 强时间事实, 首次形成UTC事实;
    std::int64_t 首次形成UTC纳秒 = 0;
    std::uint64_t 创建事实代次 = 0;
};
inline bool 状态内容事实完整(const 状态内容事实& v) noexcept {
    if (!v.Gread || !有效(v.信息.身份)
        || !有效(v.信息.正式特征类型) || !浅层结构有效(v.信息.固定准确值)
        || (v.信息.强时间.语义 != 状态强时间语义::实例绝对UTC纳秒
            && v.信息.强时间.语义 != 状态强时间语义::抽象相对纳秒)
        || v.信息.强时间.纳秒 < 0 || v.首次形成UTC纳秒 < 0
        || !v.创建事实代次 || v.创建事实代次 > v.Gread)
        return false;
    const 稳定编码 ids[]{v.信息.身份.编码, v.族归属关系,
        v.正式特征类型关系, v.准确值事实, v.强时间事实, v.首次形成UTC事实};
    for (std::size_t i = 0; i < 6; ++i) {
        if (!有效(ids[i])) return false;
        for (std::size_t j = 0; j < i; ++j) if (ids[i] == ids[j]) return false;
    }
    return true;
}

struct 状态创建请求 final { std::uint32_t 合同版本=2; std::uint64_t G0=0;
    L1所有者范围写入幂等身份 幂等身份{}; 特征类型身份 正式特征类型;
    状态固定准确值 固定准确值; 状态强时间 强时间; };
struct 状态当前读取请求 final { std::uint32_t 合同版本=2; std::uint64_t G0=0; 状态信息身份 身份; };
struct 状态按正式特征类型查询请求 final { std::uint32_t 合同版本=2; std::uint64_t G0=0;
    特征类型身份 正式特征类型; std::uint64_t 最大候选数量=0; };
struct 状态按固定准确值查询请求 final { std::uint32_t 合同版本=2; std::uint64_t G0=0;
    状态固定准确值 固定准确值; std::uint64_t 最大候选数量=0; };
struct 状态按强时间范围查询请求 final { std::uint32_t 合同版本=2; std::uint64_t G0=0;
    状态强时间语义 语义=状态强时间语义::实例绝对UTC纳秒;
    std::int64_t 起始纳秒=0,终止纳秒=0; std::uint64_t 最大候选数量=0; };
struct 状态退出请求 final { std::uint32_t 合同版本=2; std::uint64_t G0=0;
    L1所有者范围写入幂等身份 幂等身份{}; 状态信息身份 身份; };
struct 状态当前身份确认请求 final { std::uint32_t 合同版本=2; std::uint64_t G0=0; 状态信息身份 身份; };

struct 状态创建结果 final { 状态类结果头 结果头; std::optional<状态内容事实> 内容; };
struct 状态读取结果 final { 状态类结果头 结果头; std::optional<状态内容事实> 内容; };
struct 状态组查询结果 final { 状态类结果头 结果头; std::vector<状态内容事实> 状态组; };
struct 状态退出结果 final { 状态类结果头 结果头; };

class 状态类数据服务 final {
public:
    状态类数据服务()=delete;
    状态类数据服务(const 状态类数据服务&)=delete;
    状态类数据服务& operator=(const 状态类数据服务&)=delete;
    状态类数据服务(状态类数据服务&&)=delete;
    状态类数据服务& operator=(状态类数据服务&&)=delete;
    状态类数据服务(L1事实基座服务& l1,const 特征类数据服务& feature,
        L1所有者范围写端口&& port,const 状态类结构交付& layout)
        :l1_(l1),feature_(feature),port_(std::move(port)),owner_(port_.所有者身份()),layout_(layout){
        if(!绑定于(l1_)||!有效(owner_)||!布局浅层有效())
            throw std::invalid_argument("invalid state configuration");
        const auto o=l1_.读取当前结构所有者({L1所有者范围CRUD合同版本,owner_});
        if(o.状态!=L1所有者范围读取状态::成功||!o.所有者事实||o.所有者事实->所有者!=owner_
            ||o.所有者事实->范围种类!=L1所有者范围种类::独占结构范围||!布局材料有效())
            throw std::invalid_argument("invalid state structure");
    }
    bool 绑定于(const L1事实基座服务& s) const noexcept{return &s==&l1_&&feature_.绑定于(s)&&port_.绑定于(s);}

    状态创建结果 创建状态(const 状态创建请求& r){
        if(!创建请求有效(r))return 创建失败(状态类数据状态::入口拒绝);
        try{
            if(auto replay=重放创建(r))return *replay;
            const auto check=feature_.核验正式特征类型准确值({1,r.G0,r.正式特征类型,r.固定准确值});
            if(!check.成功())return 创建失败(映射特征(check.状态),r.G0);
            if(check.合同版本!=1||!check.事实||check.事实->Gread!=r.G0
                ||check.事实->正式特征类型!=r.正式特征类型||check.事实->准确值!=r.固定准确值)
                return 创建失败(状态类数据状态::内部不一致,r.G0);
            const auto utc=当前UTC纳秒(); if(utc<0)return 创建失败(状态类数据状态::内部不一致,r.G0);
            auto ws=形成创建写集(r,utc); const auto saved=port_.提交所有者范围中性写集(ws);
            const auto s=映射写入(saved.状态,状态类数据状态::已创建);
            if(s!=状态类数据状态::已创建&&s!=状态类数据状态::精确重复)return 创建失败(s,saved.事实代次);
            if(!写入头完整(saved,r.幂等身份,ws.期望事实代次)||!创建映射完整(saved))return 创建失败(状态类数据状态::内部不一致,saved.事实代次);
            const auto id=映射编码(saved,节点键); if(!id)return 创建失败(状态类数据状态::内部不一致,saved.事实代次);
            const auto guard=读取当前代次();if(guard.状态!=状态类数据状态::已读取)return 创建失败(guard.状态,guard.Gread);
            auto read=读取当前内容(guard.Gread,{*id});
            if(read.结果头.状态!=状态类数据状态::已读取||!read.内容
                ||read.结果头.事实截止代次!=guard.Gread)
                return 创建失败(read.结果头.状态,read.结果头.事实截止代次);
            if(read.内容->创建事实代次!=saved.事实代次)
                return 创建失败(状态类数据状态::内部不一致,guard.Gread);
            return {{s,2,guard.Gread,saved.事实代次},std::move(read.内容)};
        }catch(const std::bad_alloc&){return 创建失败(状态类数据状态::资源失败);}catch(...){return 创建失败(状态类数据状态::内部不一致);}
    }
    状态读取结果 读取当前状态(const 状态当前读取请求& r) const{
        if(r.合同版本!=2||!r.G0||!有效(r.身份))return 读取失败(状态类数据状态::入口拒绝);
        const auto current=读取当前代次();
        if(current.状态!=状态类数据状态::已读取)return 读取失败(current.状态,current.Gread);
        if(current.Gread!=r.G0)return 读取失败(状态类数据状态::事实代次漂移,current.Gread);
        auto out=读取当前内容(r.G0,r.身份);
        if(out.结果头.状态==状态类数据状态::已读取&&out.内容){
            const auto after=读取当前代次();
            if(after.状态!=状态类数据状态::已读取)return 读取失败(after.状态,after.Gread);
            if(after.Gread!=r.G0)return 读取失败(状态类数据状态::事实代次漂移,after.Gread);
        }
        return out;
    }

    状态组查询结果 按正式特征类型查询当前状态组(const 状态按正式特征类型查询请求& r) const{
        if(r.合同版本!=2||!r.G0||!有效(r.正式特征类型)||!r.最大候选数量)return 组失败(状态类数据状态::入口拒绝);
        try{
            const auto q=l1_.读取所有者范围当前目标关系组({L1所有者范围CRUD合同版本,r.正式特征类型.编码,layout_.正式特征类型关系类型});
            if(q.合同版本!=L1所有者范围CRUD合同版本||q.目标节点!=r.正式特征类型.编码
                ||q.关系类型节点!=layout_.正式特征类型关系类型)
                return 组失败(状态类数据状态::内部不一致,q.读取事实代次);
            if(q.状态!=L1所有者范围读取状态::成功){if(!q.关系组.empty())return 组失败(状态类数据状态::内部不一致,q.读取事实代次);
                return 组失败(映射读取(q.状态),q.读取事实代次);}
            if(q.读取事实代次!=r.G0)return 组失败(状态类数据状态::事实代次漂移,q.读取事实代次);
            std::vector<状态信息身份> ids;for(const auto& e:q.关系组){
                if(e.写入所有者!=owner_)continue;
                if(e.目标节点!=r.正式特征类型.编码||e.关系类型节点!=layout_.正式特征类型关系类型
                    ||!有效(e.编码)||e.角色或顺序!=1||!有效(e.源节点)||!e.创建事实代次)
                    return 组失败(状态类数据状态::内部不一致,r.G0);
                ids.push_back({e.源节点});}
            if(ids.size()>r.最大候选数量)return 组失败(状态类数据状态::数量预算不足,r.G0);
            return 完整读取候选(r.G0,std::move(ids));
        }catch(const std::bad_alloc&){return 组失败(状态类数据状态::资源失败);}catch(...){return 组失败(状态类数据状态::内部不一致);}
    }
    状态组查询结果 按固定准确值查询当前状态组(const 状态按固定准确值查询请求& r) const{
        if(r.合同版本!=2||!r.G0||!浅层结构有效(r.固定准确值)||!r.最大候选数量)return 组失败(状态类数据状态::入口拒绝);
        const auto type = std::holds_alternative<std::int64_t>(r.固定准确值)
            ? layout_.内联准确值属性类型
            : layout_.引用准确值属性类型;
        try{
            const auto q=l1_.读取所有者范围属性类型当前完整值组({L1所有者范围属性类型当前完整值组读取合同版本,owner_,type,r.G0});
            if(!值组回显完整(q,owner_,type,r.G0))return 组失败(状态类数据状态::内部不一致,q.读取事实代次);
            const auto s=映射组读取(q.状态);if(s!=状态类数据状态::已读取){if(!q.当前值.empty())return 组失败(状态类数据状态::内部不一致,q.读取事实代次);return 组失败(s,q.读取事实代次);}
            std::vector<状态信息身份> ids;for(const auto& v:q.当前值){if(!当前值事实完整(v,type))return 组失败(状态类数据状态::内部不一致,r.G0);
                if(准确材料相等(v.材料,r.固定准确值))ids.push_back({v.所属节点});}
            return 完整读取候选(r.G0,std::move(ids));
        }catch(const std::bad_alloc&){return 组失败(状态类数据状态::资源失败);}catch(...){return 组失败(状态类数据状态::内部不一致);}
    }
    状态组查询结果 按强时间范围查询当前状态组(const 状态按强时间范围查询请求& r) const{
        if(r.合同版本!=2||!r.G0||!时间语义有效(r.语义)||r.起始纳秒<0||r.终止纳秒<r.起始纳秒||!r.最大候选数量)
            return 组失败(状态类数据状态::入口拒绝);
        const auto type = r.语义 == 状态强时间语义::实例绝对UTC纳秒
            ? layout_.实例绝对时间属性类型
            : layout_.抽象相对时间属性类型;
        try{
            const auto q=l1_.读取所有者范围属性类型当前完整值组({L1所有者范围属性类型当前完整值组读取合同版本,owner_,type,r.G0});
            if(!值组回显完整(q,owner_,type,r.G0))return 组失败(状态类数据状态::内部不一致,q.读取事实代次);
            const auto s=映射组读取(q.状态);if(s!=状态类数据状态::已读取){if(!q.当前值.empty())return 组失败(状态类数据状态::内部不一致,q.读取事实代次);return 组失败(s,q.读取事实代次);}
            std::vector<状态信息身份> ids;for(const auto& v:q.当前值){const auto* n=std::get_if<std::int64_t>(&v.材料);
                if(!当前值事实完整(v,type)||!n||*n<0)return 组失败(状态类数据状态::内部不一致,r.G0);if(*n>=r.起始纳秒&&*n<=r.终止纳秒)ids.push_back({v.所属节点});}
            return 完整读取候选(r.G0,std::move(ids));
        }catch(const std::bad_alloc&){return 组失败(状态类数据状态::资源失败);}catch(...){return 组失败(状态类数据状态::内部不一致);}
    }
    状态退出结果 退出状态(const 状态退出请求& r){
        if(r.合同版本!=2||!r.G0||r.G0==std::numeric_limits<std::uint64_t>::max()
            ||!有效(r.幂等身份)||!有效(r.身份))return 退出失败(状态类数据状态::入口拒绝);
        try{
            if(auto replay=重放退出(r))return *replay;
            const auto current=读取当前状态({2,r.G0,r.身份});
            if(current.结果头.状态!=状态类数据状态::已读取||!current.内容)
                return 退出失败(current.结果头.状态,current.结果头.事实截止代次);
            auto ws=形成退出写集(*current.内容,r.G0,r.幂等身份);const auto saved=port_.提交所有者范围中性写集(ws);
            const auto s=映射写入(saved.状态,状态类数据状态::已删除);
            if(s!=状态类数据状态::已删除&&s!=状态类数据状态::精确重复)return 退出失败(s,saved.事实代次);
            if(!写入头完整(saved,r.幂等身份,ws.期望事实代次)||!saved.新编码映射.empty())return 退出失败(状态类数据状态::内部不一致,saved.事实代次);
            const auto guard=读取当前代次();if(guard.状态!=状态类数据状态::已读取)return 退出失败(guard.状态,guard.Gread);
            return {{s,2,guard.Gread,saved.事实代次}};
        }catch(const std::bad_alloc&){return 退出失败(状态类数据状态::资源失败);}catch(...){return 退出失败(状态类数据状态::内部不一致);}
    }
    状态类结果头 确认当前状态结构身份(const 状态当前身份确认请求& r) const{
        if(r.合同版本!=2||!r.G0||!有效(r.身份))return 头(状态类数据状态::入口拒绝);
        const auto current=读取当前代次();
        if(current.状态!=状态类数据状态::已读取)return 头(current.状态,current.Gread);
        if(current.Gread!=r.G0)return 头(状态类数据状态::事实代次漂移,current.Gread);
        auto w=读取当前内容(r.G0,r.身份);
        return w.结果头.状态==状态类数据状态::已读取&&w.内容
            ? 头(状态类数据状态::已读取,r.G0) : w.结果头;
    }

private:
    inline static constexpr L1所有者范围写集本地键 节点键{1},族关系键{2},FT关系键{3},准确值键{4},时间键{5},首次UTC键{6};
    struct 唯一关系读取结果 final {
        状态类数据状态 状态 = 状态类数据状态::内部不一致;
        std::uint64_t Gread = 0;
        std::optional<L1所有者范围关系事实> 关系;
    };
    struct 当前代次读取结果 final {
        状态类数据状态 状态 = 状态类数据状态::内部不一致;
        std::uint64_t Gread = 0;
    };

    bool 布局浅层有效() const noexcept{
        const 稳定编码 ids[]={layout_.状态族锚点,layout_.状态族归属关系类型,layout_.正式特征类型关系类型,
            layout_.内联准确值属性类型,layout_.引用准确值属性类型,layout_.实例绝对时间属性类型,
            layout_.抽象相对时间属性类型,layout_.首次形成UTC属性类型};
        for(std::size_t i=0;i<8;++i){if(!有效(ids[i]))return false;for(std::size_t j=0;j<i;++j)if(ids[i]==ids[j])return false;}return true;
    }
    bool 布局材料有效() const{
        return 节点类型有效(layout_.状态族锚点,节点种类::普通,std::nullopt)
            &&节点类型有效(layout_.状态族归属关系类型,节点种类::普通,std::nullopt)
            &&节点类型有效(layout_.正式特征类型关系类型,节点种类::普通,std::nullopt)
            &&节点类型有效(layout_.内联准确值属性类型,节点种类::属性类型,L1所有者范围值表示种类::I64)
            &&节点类型有效(layout_.引用准确值属性类型,节点种类::属性类型,L1所有者范围值表示种类::U64组)
            &&节点类型有效(layout_.实例绝对时间属性类型,节点种类::属性类型,L1所有者范围值表示种类::I64)
            &&节点类型有效(layout_.抽象相对时间属性类型,节点种类::属性类型,L1所有者范围值表示种类::I64)
            &&节点类型有效(layout_.首次形成UTC属性类型,节点种类::属性类型,L1所有者范围值表示种类::I64);
    }
    bool 节点类型有效(稳定编码 id,节点种类 kind,std::optional<L1所有者范围值表示种类> repr) const{
        const auto q=l1_.读取所有者范围当前节点({L1所有者范围CRUD合同版本,id});
        const auto* n = q.事实
            ? std::get_if<L1所有者范围节点事实>(&*q.事实)
            : nullptr;
        return q.状态==L1所有者范围读取状态::成功&&n&&n->编码==id&&n->写入所有者==owner_
            &&n->种类==kind&&n->属性类型表示==repr;
    }
    static bool 时间语义有效(状态强时间语义 s) noexcept{return s==状态强时间语义::实例绝对UTC纳秒||s==状态强时间语义::抽象相对纳秒;}
    static bool 创建请求有效(const 状态创建请求& r) noexcept{return r.合同版本==2&&r.G0
        &&r.G0!=std::numeric_limits<std::uint64_t>::max()&&有效(r.幂等身份)
        &&有效(r.正式特征类型)&&浅层结构有效(r.固定准确值)&&时间语义有效(r.强时间.语义)&&r.强时间.纳秒>=0;}
    static std::int64_t 当前UTC纳秒() noexcept{return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();}
    当前代次读取结果 读取当前代次() const{
        const auto q=l1_.读取中性当前事实代次({L1中性CRUD合同版本});
        if(q.状态==L1中性读取状态::资源失败)return {状态类数据状态::资源失败,q.事实代次};
        if(q.状态!=L1中性读取状态::成功||q.合同版本!=L1中性CRUD合同版本||!q.事实代次)
            return {状态类数据状态::内部不一致,q.事实代次};
        return {状态类数据状态::已读取,q.事实代次};}
    bool 值组回显完整(const L1所有者范围属性类型当前完整值组读取结果_v2& q,
        L1结构所有者身份 owner,稳定编码 type,std::uint64_t g) const noexcept{
        return q.合同版本==L1所有者范围属性类型当前完整值组读取合同版本
            &&q.所有者==owner&&q.属性类型节点==type&&q.期望事实代次==g;
    }
    bool 当前值事实完整(const L1所有者范围值事实& v,稳定编码 type) const noexcept{
        return 有效(v.编码)&&有效(v.所属节点)&&v.属性类型节点==type&&v.来源节点==v.所属节点
            &&v.写入所有者==owner_&&v.创建事实代次;
    }

    L1所有者范围写集请求 形成创建写集(const 状态创建请求& r,std::int64_t utc) const{
        L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,r.G0,r.幂等身份};
        ws.节点={{节点键,节点种类::普通,std::nullopt}};
        ws.关系={{族关系键,节点键,layout_.状态族锚点,layout_.状态族归属关系类型,1},
            {FT关系键,节点键,r.正式特征类型.编码,layout_.正式特征类型关系类型,1}};
        const auto at = std::holds_alternative<std::int64_t>(r.固定准确值)
            ? layout_.内联准确值属性类型
            : layout_.引用准确值属性类型;
        L1所有者范围原始值材料 av=std::holds_alternative<std::int64_t>(r.固定准确值)
            ? L1所有者范围原始值材料{std::get<std::int64_t>(r.固定准确值)}
            : L1所有者范围原始值材料{std::vector<std::uint64_t>{std::get<特征值身份>(r.固定准确值).编码.值}};
        const auto tt = r.强时间.语义 == 状态强时间语义::实例绝对UTC纳秒
            ? layout_.实例绝对时间属性类型
            : layout_.抽象相对时间属性类型;
        ws.值={{准确值键,节点键,at,std::move(av),节点键},{时间键,节点键,tt,r.强时间.纳秒,节点键},
            {首次UTC键,节点键,layout_.首次形成UTC属性类型,utc,节点键}};
        ws.属性槽变更={{节点键,at,准确值键},{节点键,tt,时间键},{节点键,layout_.首次形成UTC属性类型,首次UTC键}};return ws;
    }
    static std::optional<std::int64_t> 查找首次UTC(const L1所有者范围写集请求& ws){for(const auto& v:ws.值)if(v.本地键==首次UTC键)
        if(const auto* n=std::get_if<std::int64_t>(&v.材料))return *n;return std::nullopt;}
    std::optional<状态创建结果> 重放创建(const 状态创建请求& r){
        const auto first=port_.读取首次写入材料({L1所有者范围首次写入读取合同版本,r.幂等身份});
        if(!首次读取回显完整(first,r.幂等身份))return 创建失败(状态类数据状态::内部不一致,first.读取事实代次);
        if(first.状态==L1所有者范围读取状态::未找到){if(!first.读取事实代次||first.首次规范化写集||first.首次写入结果)
                return 创建失败(状态类数据状态::内部不一致,first.读取事实代次);return std::nullopt;}
        if(first.状态!=L1所有者范围读取状态::成功){
            if(first.首次规范化写集||first.首次写入结果)return 创建失败(状态类数据状态::内部不一致,first.读取事实代次);
            return 创建失败(映射读取(first.状态),first.读取事实代次);}
        if(!first.读取事实代次||!first.首次规范化写集||!first.首次写入结果)
            return 创建失败(状态类数据状态::内部不一致,first.读取事实代次);
        const auto& ws=*first.首次规范化写集;const auto utc=查找首次UTC(ws);
        if(!utc||ws!=形成创建写集(r,*utc))return 创建失败(状态类数据状态::幂等冲突,first.读取事实代次);
        if(first.首次写入结果->状态!=L1所有者范围写入状态::成功
            ||!写入头完整(*first.首次写入结果,r.幂等身份,ws.期望事实代次)
            ||!创建映射完整(*first.首次写入结果))
            return 创建失败(状态类数据状态::内部不一致,first.读取事实代次);
        const auto saved=port_.提交所有者范围中性写集(ws);
        if(saved.状态!=L1所有者范围写入状态::精确重复||!写入头完整(saved,r.幂等身份,ws.期望事实代次))return 创建失败(映射写入(saved.状态,状态类数据状态::已创建),saved.事实代次);
        if(!创建映射完整(saved))return 创建失败(状态类数据状态::内部不一致,saved.事实代次);
        if(saved.新编码映射!=first.首次写入结果->新编码映射)return 创建失败(状态类数据状态::内部不一致,saved.事实代次);
        const auto id=映射编码(saved,节点键);if(!id)return 创建失败(状态类数据状态::内部不一致,saved.事实代次);
        auto read=读取当前内容(first.读取事实代次,{*id});
        if(read.结果头.状态!=状态类数据状态::已读取||!read.内容)
            return 创建失败(read.结果头.状态,read.结果头.事实截止代次);
        if(read.内容->创建事实代次!=saved.事实代次)
            return 创建失败(状态类数据状态::内部不一致,first.读取事实代次);
        return 状态创建结果{{状态类数据状态::精确重复,2,first.读取事实代次,saved.事实代次},std::move(read.内容)};
    }

    状态读取结果 读取当前内容(std::uint64_t g,状态信息身份 id) const{
        const auto nr=l1_.读取所有者范围当前事实(
            {L1所有者范围当前事实读取合同版本_v2,owner_,id.编码,g});
        if(nr.合同版本!=L1所有者范围当前事实读取合同版本_v2||nr.所有者!=owner_
            ||nr.事实编码!=id.编码||nr.期望事实代次!=g)
            return 读取失败(状态类数据状态::内部不一致,nr.读取事实代次);
        const auto ns=映射当前事实读取(nr.状态);
        if(ns!=状态类数据状态::已读取){
            if(nr.载荷)return 读取失败(状态类数据状态::内部不一致,nr.读取事实代次);
            return 读取失败(ns,nr.读取事实代次);}
        if(nr.读取事实代次!=g||!nr.载荷)return 读取失败(状态类数据状态::内部不一致,nr.读取事实代次);
        const auto* n=std::get_if<L1所有者范围节点事实>(&*nr.载荷);
        if(!n||n->编码!=id.编码||n->写入所有者!=owner_||n->种类!=节点种类::普通
            ||n->属性类型表示||!n->创建事实代次||n->创建事实代次>g)
            return 读取失败(状态类数据状态::旧格式不支持,g);
        const auto familyRead=读取唯一关系(id.编码,layout_.状态族归属关系类型,g);
        if(familyRead.状态!=状态类数据状态::已读取)return 读取失败(familyRead.状态,familyRead.Gread);
        const auto ftRead=读取唯一关系(id.编码,layout_.正式特征类型关系类型,g);
        if(ftRead.状态!=状态类数据状态::已读取)return 读取失败(ftRead.状态,ftRead.Gread);
        const auto& family=*familyRead.关系;
        const auto& ft=*ftRead.关系;
        const auto ar=l1_.读取所有者范围所属节点当前完整值组(
            {L1所有者范围所属节点当前完整值组读取合同版本_v2,owner_,id.编码,g});
        if(ar.合同版本!=L1所有者范围所属节点当前完整值组读取合同版本_v2
            ||ar.所有者!=owner_||ar.所属节点!=id.编码||ar.期望事实代次!=g)
            return 读取失败(状态类数据状态::内部不一致,ar.读取事实代次);
        const auto as=映射所属节点当前值组读取(ar.状态);
        if(as!=状态类数据状态::已读取){if(!ar.载荷.empty())return 读取失败(状态类数据状态::内部不一致,ar.读取事实代次);
            return 读取失败(as,ar.读取事实代次);}
        if(ar.读取事实代次!=g)return 读取失败(状态类数据状态::事实代次漂移,ar.读取事实代次);
        if(family.目标节点!=layout_.状态族锚点||family.角色或顺序!=1||ft.角色或顺序!=1||!有效(ft.目标节点)||ar.载荷.size()!=3)
            return 读取失败(状态类数据状态::旧格式不支持,g);
        const L1所有者范围值事实 *accurate=nullptr,*time=nullptr,*utc=nullptr;状态固定准确值 accurateValue;状态强时间 strong;
        for(const auto& v:ar.载荷){if(!值共同有效(v,*n))return 读取失败(状态类数据状态::内部不一致,g);
            if(v.属性类型节点==layout_.内联准确值属性类型){const auto* x=std::get_if<std::int64_t>(&v.材料);if(!x||accurate)return 读取失败(状态类数据状态::旧格式不支持,g);accurate=&v;accurateValue=*x;}
            else if(v.属性类型节点==layout_.引用准确值属性类型){const auto* x=std::get_if<std::vector<std::uint64_t>>(&v.材料);if(!x||x->size()!=1||!x->front()||accurate)return 读取失败(状态类数据状态::旧格式不支持,g);accurate=&v;accurateValue=特征值身份{{x->front()}};}
            else if(v.属性类型节点==layout_.实例绝对时间属性类型||v.属性类型节点==layout_.抽象相对时间属性类型){const auto* x=std::get_if<std::int64_t>(&v.材料);if(!x||*x<0||time)return 读取失败(状态类数据状态::旧格式不支持,g);time=&v;strong={v.属性类型节点==layout_.实例绝对时间属性类型
                    ? 状态强时间语义::实例绝对UTC纳秒
                    : 状态强时间语义::抽象相对纳秒,*x};}
            else if(v.属性类型节点==layout_.首次形成UTC属性类型){const auto* x=std::get_if<std::int64_t>(&v.材料);if(!x||*x<0||utc)return 读取失败(状态类数据状态::旧格式不支持,g);utc=&v;}
            else return 读取失败(状态类数据状态::旧格式不支持,g);}
        if(!accurate||!time||!utc||!同生命周期(*n,family)||!同生命周期(*n,ft))return 读取失败(状态类数据状态::内部不一致,g);
        状态内容事实 out{g,{id,{ft.目标节点},accurateValue,strong},family.编码,ft.编码,accurate->编码,time->编码,utc->编码,
            std::get<std::int64_t>(utc->材料),n->创建事实代次};
        return {{状态类数据状态::已读取,2,g,std::nullopt},std::move(out)};
    }
    唯一关系读取结果 读取唯一关系(稳定编码 source,稳定编码 type,std::uint64_t g) const{
        const auto q=l1_.读取所有者范围当前源关系组({L1所有者范围CRUD合同版本,source,type});
        if(q.合同版本!=L1所有者范围CRUD合同版本||q.源节点!=source||q.关系类型节点!=type)
            return {状态类数据状态::内部不一致,q.读取事实代次,std::nullopt};
        if(q.状态!=L1所有者范围读取状态::成功){if(!q.关系组.empty())return {状态类数据状态::内部不一致,q.读取事实代次,std::nullopt};
            return {映射读取(q.状态),q.读取事实代次,std::nullopt};}
        if(q.读取事实代次!=g)return {状态类数据状态::事实代次漂移,q.读取事实代次,std::nullopt};
        std::vector<L1所有者范围关系事实> own;for(const auto& e:q.关系组)if(e.写入所有者==owner_)own.push_back(e);
        if(own.size()!=1)return {own.empty()?状态类数据状态::旧格式不支持:状态类数据状态::内部不一致,g,std::nullopt};
        const auto& e=own.front();
        if(e.源节点!=source||e.关系类型节点!=type)return {状态类数据状态::内部不一致,g,std::nullopt};
        return {状态类数据状态::已读取,g,e};}
    template<class F> bool 同生命周期(const L1所有者范围节点事实& n,const F& f) const noexcept{
        return f.写入所有者==owner_&&f.源节点==n.编码&&f.创建事实代次==n.创建事实代次;}
    bool 值共同有效(const L1所有者范围值事实& v,const L1所有者范围节点事实& n) const noexcept{
        return v.写入所有者==owner_&&v.所属节点==n.编码&&v.来源节点==n.编码&&v.创建事实代次==n.创建事实代次;}

    状态组查询结果 完整读取候选(std::uint64_t g,std::vector<状态信息身份> ids) const{
        std::sort(ids.begin(),ids.end(),[](auto a,auto b){return a.编码<b.编码;});ids.erase(std::unique(ids.begin(),ids.end()),ids.end());
        状态组查询结果 out{{状态类数据状态::已读取,2,g,std::nullopt},{}};
        for(auto id:ids){auto r=读取当前状态({2,g,id});if(r.结果头.状态!=状态类数据状态::已读取||!r.内容)return 组失败(r.结果头.状态,r.结果头.事实截止代次);out.状态组.push_back(*r.内容);}return out;
    }
    static bool 准确材料相等(const L1所有者范围原始值材料& m,const 状态固定准确值& v){
        if(const auto* n=std::get_if<std::int64_t>(&v)){const auto* x=std::get_if<std::int64_t>(&m);return x&&*x==*n;}
        const auto* x=std::get_if<std::vector<std::uint64_t>>(&m);return x&&x->size()==1&&x->front()==std::get<特征值身份>(v).编码.值;
    }
    L1所有者范围写集请求 形成退出写集(const 状态内容事实& v,std::uint64_t g,L1所有者范围写入幂等身份 key) const{
        L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,g,key};添加退出身份(ws.退出事实,v);规范化退出组(ws.退出事实);return ws;}
    static void 添加退出身份(std::vector<稳定编码>& out,const 状态内容事实& v){out.insert(out.end(),{v.信息.身份.编码,v.族归属关系,
        v.正式特征类型关系,v.准确值事实,v.强时间事实,v.首次形成UTC事实});}
    static void 规范化退出组(std::vector<稳定编码>& ids){std::sort(ids.begin(),ids.end());}
    bool 首次读取回显完整(const L1所有者范围首次写入读取结果& first,
        L1所有者范围写入幂等身份 key) const noexcept{
        return first.合同版本==L1所有者范围首次写入读取合同版本
            &&first.所有者==owner_&&first.写入幂等身份==key;
    }
    static bool 退出写集形状有效(const L1所有者范围写集请求& ws,std::uint64_t g,
        L1所有者范围写入幂等身份 key) noexcept{
        if(ws.合同版本!=L1所有者范围CRUD合同版本||ws.期望事实代次!=g
            ||ws.写入幂等身份!=key||!ws.节点.empty()||!ws.关系.empty()||!ws.值.empty()
            ||!ws.属性槽变更.empty()||ws.退出事实.empty()||ws.退出事实.size()%6!=0
            ||!std::is_sorted(ws.退出事实.begin(),ws.退出事实.end()))return false;
        for(std::size_t i=0;i<ws.退出事实.size();++i)
            if(!有效(ws.退出事实[i])||(i&&ws.退出事实[i]==ws.退出事实[i-1]))return false;
        return true;
    }
    std::optional<状态退出结果> 重放退出(const 状态退出请求& r){
        const auto first=port_.读取首次写入材料({L1所有者范围首次写入读取合同版本,r.幂等身份});if(first.状态==L1所有者范围读取状态::未找到){
            if(!首次读取回显完整(first,r.幂等身份)||!first.读取事实代次||first.首次规范化写集||first.首次写入结果)return 退出失败(状态类数据状态::内部不一致,first.读取事实代次);return std::nullopt;}
        if(!首次读取回显完整(first,r.幂等身份))return 退出失败(状态类数据状态::内部不一致,first.读取事实代次);
        if(first.状态!=L1所有者范围读取状态::成功){
            if(first.首次规范化写集||first.首次写入结果)return 退出失败(状态类数据状态::内部不一致,first.读取事实代次);
            return 退出失败(映射读取(first.状态),first.读取事实代次);}
        if(!first.读取事实代次||!first.首次规范化写集||!first.首次写入结果)return 退出失败(状态类数据状态::内部不一致,first.读取事实代次);
        const auto& ws=*first.首次规范化写集;if(!退出写集形状有效(ws,r.G0,r.幂等身份)||ws.退出事实.size()!=6
            ||!std::binary_search(ws.退出事实.begin(),ws.退出事实.end(),r.身份.编码)
            ||first.首次写入结果->状态!=L1所有者范围写入状态::成功||!写入头完整(*first.首次写入结果,r.幂等身份,ws.期望事实代次)
            ||!first.首次写入结果->新编码映射.empty())return 退出失败(状态类数据状态::幂等冲突,first.读取事实代次);
        const auto saved=port_.提交所有者范围中性写集(ws);if(saved.状态!=L1所有者范围写入状态::精确重复||!写入头完整(saved,r.幂等身份,ws.期望事实代次))
            return 退出失败(映射写入(saved.状态,状态类数据状态::已删除),saved.事实代次);
        const auto guard=读取当前代次();
        if(guard.状态!=状态类数据状态::已读取)return 退出失败(guard.状态,guard.Gread);
        return 状态退出结果{{状态类数据状态::精确重复,2,guard.Gread,saved.事实代次}};
    }
    bool 写入头完整(const L1所有者范围写入结果& r,L1所有者范围写入幂等身份 key,std::uint64_t expected) const noexcept{
        const bool first=r.状态==L1所有者范围写入状态::成功&&r.是否形成内存权威发布&&r.重试边界==L1所有者范围重试边界::不适用;
        const bool replay=r.状态==L1所有者范围写入状态::精确重复&&!r.是否形成内存权威发布&&r.重试边界==L1所有者范围重试边界::原幂等身份读回收敛;
        return expected!=std::numeric_limits<std::uint64_t>::max()
            &&r.合同版本==L1所有者范围CRUD合同版本&&r.所有者==owner_&&r.写入幂等身份==key
            &&r.事实代次==expected+1&&(first||replay);}
    static std::optional<稳定编码> 映射编码(const L1所有者范围写入结果& r,L1所有者范围写集本地键 key){std::optional<稳定编码> out;
        for(const auto& [k,id]:r.新编码映射)if(k==key){if(out||!有效(id))return std::nullopt;out=id;}return out;}
    static bool 创建映射完整(const L1所有者范围写入结果& r) noexcept{
        const L1所有者范围写集本地键 keys[]{节点键,族关系键,FT关系键,准确值键,时间键,首次UTC键};
        if(r.新编码映射.size()!=6)return false;
        for(std::size_t i=0;i<6;++i){std::size_t count=0;稳定编码 id{};
            for(const auto& [k,v]:r.新编码映射)if(k==keys[i]){id=v;++count;}
            if(count!=1||!有效(id))return false;
            for(std::size_t j=0;j<i;++j){const auto prior=映射编码(r,keys[j]);if(!prior||*prior==id)return false;}}
        return true;
    }

    static 状态类结果头 头(状态类数据状态 s,std::uint64_t g=0,std::optional<std::uint64_t> changed=std::nullopt) noexcept{return {s,2,g,changed};}
    static 状态创建结果 创建失败(状态类数据状态 s,std::uint64_t g=0) noexcept{return {头(s,g),std::nullopt};}
    static 状态读取结果 读取失败(状态类数据状态 s,std::uint64_t g=0) noexcept{return {头(s,g),std::nullopt};}
    static 状态组查询结果 组失败(状态类数据状态 s,std::uint64_t g=0) noexcept{return {头(s,g),{}};}
    static 状态退出结果 退出失败(状态类数据状态 s,std::uint64_t g=0) noexcept{return {头(s,g)};}
    static 状态类数据状态 映射特征(特征类型准确值核验状态 s) noexcept{switch(s){
        case 特征类型准确值核验状态::正式特征类型未找到:return 状态类数据状态::正式特征类型未找到;
        case 特征类型准确值核验状态::准确值未找到:
        case 特征类型准确值核验状态::准确值不相容:return 状态类数据状态::准确值不相容;
        case 特征类型准确值核验状态::事实代次漂移:return 状态类数据状态::事实代次漂移;
        case 特征类型准确值核验状态::入口拒绝:return 状态类数据状态::入口拒绝;
        case 特征类型准确值核验状态::资源失败:return 状态类数据状态::资源失败;
        default:return 状态类数据状态::内部不一致;}}
    static 状态类数据状态 映射读取(L1所有者范围读取状态 s) noexcept{switch(s){
        case L1所有者范围读取状态::未找到:return 状态类数据状态::未找到;
        case L1所有者范围读取状态::事实代次漂移:return 状态类数据状态::事实代次漂移;
        case L1所有者范围读取状态::资源失败:return 状态类数据状态::资源失败;case L1所有者范围读取状态::入口拒绝:return 状态类数据状态::入口拒绝;default:return 状态类数据状态::内部不一致;}}
    static 状态类数据状态 映射组读取(L1所有者范围属性类型当前完整值组读取状态_v2 s) noexcept{switch(s){
        case L1所有者范围属性类型当前完整值组读取状态_v2::成功:return 状态类数据状态::已读取;case L1所有者范围属性类型当前完整值组读取状态_v2::事实代次漂移:return 状态类数据状态::事实代次漂移;
        case L1所有者范围属性类型当前完整值组读取状态_v2::资源失败:return 状态类数据状态::资源失败;
        default:return 状态类数据状态::内部不一致;}}
    static 状态类数据状态 映射当前事实读取(L1所有者范围当前事实读取状态_v2 s) noexcept{switch(s){
        case L1所有者范围当前事实读取状态_v2::成功:return 状态类数据状态::已读取;
        case L1所有者范围当前事实读取状态_v2::未找到:return 状态类数据状态::未找到;
        case L1所有者范围当前事实读取状态_v2::事实代次漂移:return 状态类数据状态::事实代次漂移;
        case L1所有者范围当前事实读取状态_v2::资源失败:return 状态类数据状态::资源失败;
        case L1所有者范围当前事实读取状态_v2::入口拒绝:return 状态类数据状态::入口拒绝;
        default:return 状态类数据状态::内部不一致;}}
    static 状态类数据状态 映射所属节点当前值组读取(L1所有者范围所属节点当前完整值组读取状态_v2 s) noexcept{switch(s){
        case L1所有者范围所属节点当前完整值组读取状态_v2::成功:return 状态类数据状态::已读取;
        case L1所有者范围所属节点当前完整值组读取状态_v2::未找到:return 状态类数据状态::未找到;
        case L1所有者范围所属节点当前完整值组读取状态_v2::事实代次漂移:return 状态类数据状态::事实代次漂移;
        case L1所有者范围所属节点当前完整值组读取状态_v2::资源失败:return 状态类数据状态::资源失败;
        case L1所有者范围所属节点当前完整值组读取状态_v2::入口拒绝:return 状态类数据状态::入口拒绝;
        default:return 状态类数据状态::内部不一致;}}
    static 状态类数据状态 映射写入(L1所有者范围写入状态 s,状态类数据状态 ok) noexcept{switch(s){
        case L1所有者范围写入状态::成功:return ok;case L1所有者范围写入状态::精确重复:return 状态类数据状态::精确重复;case L1所有者范围写入状态::事实代次漂移:return 状态类数据状态::事实代次漂移;
        case L1所有者范围写入状态::幂等冲突:return 状态类数据状态::幂等冲突;case L1所有者范围写入状态::引用冲突:return 状态类数据状态::引用冲突;
        case L1所有者范围写入状态::资源失败:return 状态类数据状态::资源失败;default:return 状态类数据状态::内部不一致;}}
    L1事实基座服务& l1_;const 特征类数据服务& feature_;L1所有者范围写端口 port_;L1结构所有者身份 owner_{};
    状态类结构交付 layout_;
};

} // namespace 海中鱼巣
