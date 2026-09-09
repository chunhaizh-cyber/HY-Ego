module;

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

export module 海中鱼巣.领域.数据服务.状态类;
export import 海中鱼巣.核心.合同.L1事实基座;
export import 海中鱼巣.领域.数据服务.特征类;

export namespace 海中鱼巣 {

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
    已创建 = 1, 精确重复 = 2, 已读取 = 3, 已退出 = 4, 已清理 = 5,
    入口拒绝 = 6, 未找到 = 7, 目标已退出 = 8,
    正式特征类型未找到 = 9, 正式特征类型已退出 = 10,
    准确值不相容 = 11, 旧格式不支持 = 12,
    事实代次漂移 = 13, 幂等冲突 = 14, 引用冲突 = 15,
    数量预算不足 = 16, 历史材料已清理 = 17,
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
struct 状态类保留策略 final {
    std::uint64_t 实例状态TTL纳秒 = 604800000000000ULL;
    std::uint64_t 实例状态数量上限 = 1000000;
};
struct 状态类结果头 final {
    状态类数据状态 状态 = 状态类数据状态::入口拒绝;
    std::uint32_t 合同版本 = 状态类数据合同版本;
    std::uint64_t 事实截止代次 = 0;
    std::optional<std::uint64_t> 变更事实代次;
};
struct 状态内容事实 final {
    std::uint64_t Gread = 0, H = 0;
    状态信息 信息;
    稳定编码 族归属关系, 正式特征类型关系, 准确值事实, 强时间事实, 首次形成UTC事实;
    std::int64_t 首次形成UTC纳秒 = 0;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
};
inline bool 状态内容事实完整(const 状态内容事实& v) noexcept {
    if (!v.Gread || !v.H || v.H > v.Gread || !有效(v.信息.身份)
        || !有效(v.信息.正式特征类型) || !浅层结构有效(v.信息.固定准确值)
        || (v.信息.强时间.语义 != 状态强时间语义::实例绝对UTC纳秒
            && v.信息.强时间.语义 != 状态强时间语义::抽象相对纳秒)
        || v.信息.强时间.纳秒 < 0 || v.首次形成UTC纳秒 < 0
        || !v.创建事实代次 || v.创建事实代次 > v.H
        || (v.退出事实代次 && v.H >= *v.退出事实代次))
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
struct 状态历史读取请求 final { std::uint32_t 合同版本=2; std::uint64_t Gread=0,H=0; 状态信息身份 身份; };
struct 状态按正式特征类型查询请求 final { std::uint32_t 合同版本=2; std::uint64_t G0=0;
    特征类型身份 正式特征类型; std::uint64_t 最大候选数量=0; };
struct 状态按固定准确值查询请求 final { std::uint32_t 合同版本=2; std::uint64_t G0=0;
    状态固定准确值 固定准确值; std::uint64_t 最大候选数量=0; };
struct 状态按强时间范围查询请求 final { std::uint32_t 合同版本=2; std::uint64_t G0=0;
    状态强时间语义 语义=状态强时间语义::实例绝对UTC纳秒;
    std::int64_t 起始纳秒=0,终止纳秒=0; std::uint64_t 最大候选数量=0; };
struct 状态退出请求 final { std::uint32_t 合同版本=2; std::uint64_t G0=0;
    L1所有者范围写入幂等身份 幂等身份{}; 状态信息身份 身份; };
struct 状态同步清理请求 final { std::uint32_t 合同版本=2; std::uint64_t G0=0;
    L1所有者范围写入幂等身份 退出幂等身份{}; L1物理清理幂等身份 清理幂等身份{};
    std::int64_t 当前UTC纳秒=0; std::uint64_t 最大扫描数量=0; };
struct 状态当前身份确认请求 final { std::uint32_t 合同版本=2; std::uint64_t G0=0; 状态信息身份 身份; };
struct 状态身份来源历史见证读取请求 final { std::uint32_t 合同版本=2; std::uint64_t Gread=0,H=0; 状态信息身份 身份; };

struct 状态创建结果 final { 状态类结果头 结果头; std::optional<状态内容事实> 内容;
    bool 成功() const noexcept { return (结果头.状态==状态类数据状态::已创建||结果头.状态==状态类数据状态::精确重复)
        &&结果头.合同版本==2&&结果头.事实截止代次&&结果头.变更事实代次
        &&*结果头.变更事实代次==结果头.事实截止代次&&内容
        &&内容->Gread==结果头.事实截止代次&&内容->H==结果头.事实截止代次
        &&状态内容事实完整(*内容)&&内容->创建事实代次==结果头.事实截止代次&&!内容->退出事实代次; } };
struct 状态读取结果 final { 状态类结果头 结果头; std::optional<状态内容事实> 内容;
    bool 成功() const noexcept { return 结果头.状态==状态类数据状态::已读取&&结果头.合同版本==2
        &&结果头.事实截止代次&&!结果头.变更事实代次&&内容
        &&内容->Gread==结果头.事实截止代次&&状态内容事实完整(*内容); } };
struct 状态组查询结果 final { 状态类结果头 结果头; std::vector<状态内容事实> 状态组;
    bool 成功() const noexcept {
        if(结果头.状态!=状态类数据状态::已读取||结果头.合同版本!=2
            ||!结果头.事实截止代次||结果头.变更事实代次)return false;
        for(std::size_t i=0;i<状态组.size();++i){const auto& v=状态组[i];
            if(v.Gread!=结果头.事实截止代次||v.H!=结果头.事实截止代次||!状态内容事实完整(v)
                ||(i&&!(状态组[i-1].信息.身份.编码<v.信息.身份.编码)))return false;}
        return true; } };
struct 状态退出结果 final { 状态类结果头 结果头; std::optional<状态内容事实> 已退出内容;
    bool 成功() const noexcept { return (结果头.状态==状态类数据状态::已退出||结果头.状态==状态类数据状态::精确重复)
        &&结果头.合同版本==2&&结果头.事实截止代次&&结果头.变更事实代次
        &&*结果头.变更事实代次==结果头.事实截止代次&&已退出内容
        &&结果头.事实截止代次>1&&已退出内容->Gread==结果头.事实截止代次
        &&已退出内容->H==结果头.事实截止代次-1&&状态内容事实完整(*已退出内容)
        &&已退出内容->退出事实代次==结果头.事实截止代次; } };
struct 状态同步清理结果 final { 状态类结果头 结果头; std::vector<L1物理清理墓碑> 墓碑组;
    bool 成功() const noexcept { const bool changed=(结果头.状态==状态类数据状态::已清理||结果头.状态==状态类数据状态::精确重复)
        &&结果头.变更事实代次&&*结果头.变更事实代次==结果头.事实截止代次&&!墓碑组.empty();
        const bool empty=结果头.状态==状态类数据状态::已读取&&!结果头.变更事实代次&&墓碑组.empty();
        if(结果头.合同版本!=2||!结果头.事实截止代次||!(changed||empty))return false;
        std::optional<稳定编码> owner;std::size_t nodes=0,relations=0,values=0;
        for(std::size_t i=0;i<墓碑组.size();++i){const auto& v=墓碑组[i];
            if(!有效(v.编码)||!有效(v.内部结构分区)||!v.创建事实代次||!v.退出事实代次
                ||v.创建事实代次>=v.退出事实代次
                ||v.退出事实代次>=v.物理清理事实代次
                ||v.物理清理事实代次!=结果头.事实截止代次)return false;
            if(!owner)owner=v.内部结构分区;else if(*owner!=v.内部结构分区)return false;
            if(v.事实种类==L1物理清理事实种类::节点)++nodes;
            else if(v.事实种类==L1物理清理事实种类::关系)++relations;
            else if(v.事实种类==L1物理清理事实种类::值)++values;
            else return false;
            for(std::size_t j=0;j<i;++j)if(墓碑组[j].编码==v.编码)return false;}
        if(changed&&(墓碑组.size()%6!=0||!nodes||relations!=nodes*2||values!=nodes*3))return false;
        return true; } };
struct 状态身份来源历史见证 final { std::uint64_t Gread=0,H=0; 状态信息身份 身份;
    稳定编码 状态族锚点,族归属关系类型,族归属关系; std::uint64_t 创建事实代次=0;
    std::optional<std::uint64_t> 退出事实代次; };
struct 状态身份来源历史见证读取结果 final { 状态类结果头 结果头; std::uint64_t H=0;
    std::optional<状态身份来源历史见证> 见证;
    bool 成功() const noexcept { return 结果头.状态==状态类数据状态::已读取&&结果头.合同版本==2
        &&结果头.事实截止代次&&!结果头.变更事实代次&&H&&见证
        &&见证->Gread==结果头.事实截止代次&&见证->H==H&&见证->Gread>=见证->H
        &&有效(见证->身份)&&有效(见证->状态族锚点)&&有效(见证->族归属关系类型)
        &&有效(见证->族归属关系)&&见证->身份.编码!=见证->状态族锚点
        &&见证->身份.编码!=见证->族归属关系类型&&见证->身份.编码!=见证->族归属关系
        &&见证->状态族锚点!=见证->族归属关系类型&&见证->状态族锚点!=见证->族归属关系
        &&见证->族归属关系类型!=见证->族归属关系
        &&见证->创建事实代次&&见证->创建事实代次<=见证->H
        &&(!见证->退出事实代次||见证->H<*见证->退出事实代次); } };

class 状态类数据服务 final {
public:
    状态类数据服务()=delete;
    状态类数据服务(const 状态类数据服务&)=delete;
    状态类数据服务& operator=(const 状态类数据服务&)=delete;
    状态类数据服务(状态类数据服务&&)=delete;
    状态类数据服务& operator=(状态类数据服务&&)=delete;
    状态类数据服务(L1事实基座服务& l1,const 特征类数据服务& feature,
        L1所有者范围写端口&& port,const 状态类结构交付& layout,状态类保留策略 policy={})
        :l1_(l1),feature_(feature),port_(std::move(port)),owner_(port_.所有者身份()),layout_(layout),policy_(policy){
        if(!绑定于(l1_)||!有效(owner_)||!布局浅层有效()||!policy_.实例状态TTL纳秒||!policy_.实例状态数量上限)
            throw std::invalid_argument("invalid state configuration");
        const auto o=l1_.读取当前结构所有者({L1所有者范围CRUD合同版本,owner_});
        if(o.状态!=L1所有者范围读取状态::成功||!o.所有者事实||o.所有者事实->所有者!=owner_
            ||o.所有者事实->范围种类!=L1所有者范围种类::独占结构范围||o.所有者事实->退出事实代次||!布局材料有效())
            throw std::invalid_argument("invalid state structure");
    }
    bool 绑定于(const L1事实基座服务& s) const noexcept{return &s==&l1_&&feature_.绑定于(s)&&port_.绑定于(s);}

    状态创建结果 创建状态(const 状态创建请求& r){
        if(!创建请求有效(r))return 创建失败(状态类数据状态::入口拒绝);
        try{
            if(auto replay=重放创建(r))return *replay;
            const auto check=feature_.核验正式特征类型准确值({1,r.G0,r.G0,r.正式特征类型,r.固定准确值});
            if(!check.成功())return 创建失败(映射特征(check.状态),r.G0);
            if(check.合同版本!=1||!check.事实||check.事实->Gread!=r.G0||check.事实->H!=r.G0
                ||check.事实->正式特征类型!=r.正式特征类型||check.事实->准确值!=r.固定准确值)
                return 创建失败(状态类数据状态::内部不一致,r.G0);
            const auto utc=当前UTC纳秒(); if(utc<0)return 创建失败(状态类数据状态::内部不一致,r.G0);
            auto ws=形成创建写集(r,utc); const auto saved=port_.提交所有者范围中性写集(ws);
            const auto s=映射写入(saved.状态,状态类数据状态::已创建);
            if(s!=状态类数据状态::已创建&&s!=状态类数据状态::精确重复)return 创建失败(s,saved.事实代次);
            if(!写入头完整(saved,r.幂等身份,ws.期望事实代次)||!创建映射完整(saved))return 创建失败(状态类数据状态::内部不一致,saved.事实代次);
            const auto id=映射编码(saved,节点键); if(!id)return 创建失败(状态类数据状态::内部不一致,saved.事实代次);
            const auto guard=读取当前代次();if(guard.状态!=状态类数据状态::已读取)return 创建失败(guard.状态,guard.Gread);
            auto read=读取历史({2,guard.Gread,saved.事实代次,{*id}},std::nullopt);
            if(!read.成功())return 创建失败(read.结果头.状态,read.结果头.事实截止代次);
            if(read.内容->创建事实代次!=saved.事实代次
                ||(read.内容->退出事实代次&&*read.内容->退出事实代次<=saved.事实代次))
                return 创建失败(状态类数据状态::内部不一致,guard.Gread);
            read.内容->Gread=saved.事实代次;read.内容->H=saved.事实代次;read.内容->退出事实代次.reset();
            return {{s,2,saved.事实代次,saved.事实代次},std::move(read.内容)};
        }catch(const std::bad_alloc&){return 创建失败(状态类数据状态::资源失败);}catch(...){return 创建失败(状态类数据状态::内部不一致);}
    }
    状态读取结果 读取当前状态(const 状态当前读取请求& r) const{
        if(r.合同版本!=2||!r.G0||!有效(r.身份))return 读取失败(状态类数据状态::入口拒绝);
        const auto current=读取当前代次();
        if(current.状态!=状态类数据状态::已读取)return 读取失败(current.状态,current.Gread);
        if(current.Gread!=r.G0)return 读取失败(状态类数据状态::事实代次漂移,current.Gread);
        auto out=读取历史({2,r.G0,r.G0,r.身份},std::nullopt);
        if(out.成功()){
            const auto after=读取当前代次();
            if(after.状态!=状态类数据状态::已读取)return 读取失败(after.状态,after.Gread);
            if(after.Gread!=r.G0)return 读取失败(状态类数据状态::事实代次漂移,after.Gread);
        }
        return out;
    }
    状态读取结果 读取状态历史内容(const 状态历史读取请求& r) const{
        if(r.合同版本!=2||!r.Gread||!r.H||r.H>r.Gread||!有效(r.身份))return 读取失败(状态类数据状态::入口拒绝);
        const auto current=读取当前代次();
        if(current.状态!=状态类数据状态::已读取)return 读取失败(current.状态,current.Gread);
        if(current.Gread!=r.Gread)return 读取失败(状态类数据状态::事实代次漂移,current.Gread);
        return 读取历史(r,std::nullopt);
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
                    ||!有效(e.编码)||e.角色或顺序!=1||!有效(e.源节点)||!e.创建事实代次||e.退出事实代次)
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
            const auto q=l1_.按属性类型读取所有者范围全部当前值({L1所有者范围属性类型当前值组读取合同版本,owner_,type,r.G0,r.最大候选数量});
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
            const auto q=l1_.按属性类型读取所有者范围全部当前值({L1所有者范围属性类型当前值组读取合同版本,owner_,type,r.G0,r.最大候选数量});
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
            const auto current=读取当前状态({2,r.G0,r.身份});if(!current.成功())return 退出失败(current.结果头.状态,current.结果头.事实截止代次);
            auto ws=形成退出写集(*current.内容,r.G0,r.幂等身份);const auto saved=port_.提交所有者范围中性写集(ws);
            const auto s=映射写入(saved.状态,状态类数据状态::已退出);
            if(s!=状态类数据状态::已退出&&s!=状态类数据状态::精确重复)return 退出失败(s,saved.事实代次);
            if(!写入头完整(saved,r.幂等身份,ws.期望事实代次)||!saved.新编码映射.empty())return 退出失败(状态类数据状态::内部不一致,saved.事实代次);
            const auto guard=读取当前代次();if(guard.状态!=状态类数据状态::已读取)return 退出失败(guard.状态,guard.Gread);
            auto read=读取历史({2,guard.Gread,saved.事实代次-1,r.身份},saved.事实代次);
            if(!read.成功())return 退出失败(read.结果头.状态,read.结果头.事实截止代次);
            read.内容->Gread=saved.事实代次;
            return {{s,2,saved.事实代次,saved.事实代次},std::move(read.内容)};
        }catch(const std::bad_alloc&){return 退出失败(状态类数据状态::资源失败);}catch(...){return 退出失败(状态类数据状态::内部不一致);}
    }
    状态同步清理结果 同步清理实例状态材料(const 状态同步清理请求& r){
        if(r.合同版本!=2||!r.G0||r.G0==std::numeric_limits<std::uint64_t>::max()
            ||!有效(r.退出幂等身份)||!r.清理幂等身份.值||r.当前UTC纳秒<0||!r.最大扫描数量)
            return 清理失败(状态类数据状态::入口拒绝);
        try{
            if(auto replay=重放清理(r))return *replay;
            const auto q=l1_.按属性类型读取所有者范围全部当前值({L1所有者范围属性类型当前值组读取合同版本,owner_,layout_.首次形成UTC属性类型,r.G0,r.最大扫描数量});
            if(!值组回显完整(q,owner_,layout_.首次形成UTC属性类型,r.G0))return 清理失败(状态类数据状态::内部不一致,q.读取事实代次);
            const auto s=映射组读取(q.状态);if(s!=状态类数据状态::已读取){if(!q.当前值.empty())return 清理失败(状态类数据状态::内部不一致,q.读取事实代次);return 清理失败(s,q.读取事实代次);}
            std::vector<状态内容事实> all;for(const auto& v:q.当前值){if(!当前值事实完整(v,layout_.首次形成UTC属性类型))return 清理失败(状态类数据状态::内部不一致,r.G0);
                auto read=读取当前状态({2,r.G0,{v.所属节点}});
                if(!read.成功())return 清理失败(read.结果头.状态,read.结果头.事实截止代次);
                const auto* formed=std::get_if<std::int64_t>(&v.材料);if(!formed||*formed!=read.内容->首次形成UTC纳秒)return 清理失败(状态类数据状态::内部不一致,r.G0);
                if(read.内容->信息.强时间.语义==状态强时间语义::实例绝对UTC纳秒)all.push_back(*read.内容);}
            std::sort(all.begin(),all.end(),[](const auto& a,const auto& b){return a.首次形成UTC纳秒<b.首次形成UTC纳秒
                ||(a.首次形成UTC纳秒==b.首次形成UTC纳秒&&a.信息.身份.编码<b.信息.身份.编码);});
            const auto excess = all.size() > policy_.实例状态数量上限
                ? all.size() - static_cast<std::size_t>(policy_.实例状态数量上限)
                : 0;
            std::vector<状态内容事实> victims;for(std::size_t i=0;i<all.size();++i){
                if(r.当前UTC纳秒<all[i].首次形成UTC纳秒)return 清理失败(状态类数据状态::内部不一致,r.G0);
                const auto age=static_cast<std::uint64_t>(r.当前UTC纳秒-all[i].首次形成UTC纳秒);
                if(i<excess||age>=policy_.实例状态TTL纳秒)victims.push_back(all[i]);}
            if(victims.empty())return {{状态类数据状态::已读取,2,r.G0,std::nullopt},{}};
            L1所有者范围写集请求 ws{L1所有者范围CRUD合同版本,r.G0,r.退出幂等身份};
            for(const auto& v:victims)添加退出身份(ws.退出事实,v);规范化退出组(ws.退出事实);
            const auto exited=port_.提交所有者范围中性写集(ws);const auto es=映射写入(exited.状态,状态类数据状态::已退出);
            if(es!=状态类数据状态::已退出&&es!=状态类数据状态::精确重复)return 清理失败(es,exited.事实代次);
            if(!写入头完整(exited,r.退出幂等身份,ws.期望事实代次)||!exited.新编码映射.empty())return 清理失败(状态类数据状态::内部不一致,exited.事实代次);
            return 执行清理(形成物理清理请求(exited.事实代次,r.清理幂等身份,ws.退出事实),exited.事实代次);
        }catch(const std::bad_alloc&){return 清理失败(状态类数据状态::资源失败);}catch(...){return 清理失败(状态类数据状态::内部不一致);}
    }
    状态类结果头 确认当前状态结构身份(const 状态当前身份确认请求& r) const{
        if(r.合同版本!=2||!r.G0||!有效(r.身份))return 头(状态类数据状态::入口拒绝);
        const auto current=读取当前代次();
        if(current.状态!=状态类数据状态::已读取)return 头(current.状态,current.Gread);
        if(current.Gread!=r.G0)return 头(状态类数据状态::事实代次漂移,current.Gread);
        auto w=读取身份({2,r.G0,r.G0,r.身份});
        return w.成功() ? 头(状态类数据状态::已读取,r.G0) : w.结果头;
    }
    状态身份来源历史见证读取结果 读取状态身份来源历史见证(const 状态身份来源历史见证读取请求& r) const{
        if(r.合同版本!=2||!r.Gread||!r.H||r.H>r.Gread||!有效(r.身份))return 身份失败(状态类数据状态::入口拒绝);
        const auto current=读取当前代次();
        if(current.状态!=状态类数据状态::已读取)return 身份失败(current.状态,current.Gread,r.H);
        if(current.Gread!=r.Gread)return 身份失败(状态类数据状态::事实代次漂移,current.Gread,r.H);
        return 读取身份(r);
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
            &&n->种类==kind&&n->属性类型表示==repr&&!n->退出事实代次;
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
    bool 值组回显完整(const L1所有者范围属性类型当前值组读取结果& q,
        L1结构所有者身份 owner,稳定编码 type,std::uint64_t g) const noexcept{
        return q.合同版本==L1所有者范围属性类型当前值组读取合同版本
            &&q.所有者==owner&&q.属性类型节点==type&&q.期望事实代次==g;
    }
    bool 当前值事实完整(const L1所有者范围值事实& v,稳定编码 type) const noexcept{
        return 有效(v.编码)&&有效(v.所属节点)&&v.属性类型节点==type&&v.来源节点==v.所属节点
            &&v.写入所有者==owner_&&v.创建事实代次&&!v.退出事实代次;
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
        auto read=读取历史({2,first.读取事实代次,saved.事实代次,{*id}},std::nullopt);
        if(!read.成功())return 创建失败(read.结果头.状态,read.结果头.事实截止代次);
        if(read.内容->创建事实代次!=saved.事实代次
            ||(read.内容->退出事实代次&&*read.内容->退出事实代次<=saved.事实代次))
            return 创建失败(状态类数据状态::内部不一致,first.读取事实代次);
        read.内容->Gread=saved.事实代次;read.内容->H=saved.事实代次;read.内容->退出事实代次.reset();
        return 状态创建结果{{状态类数据状态::精确重复,2,saved.事实代次,saved.事实代次},std::move(read.内容)};
    }

    bool 墓碑节点回显完整(const L1所有者范围历史读取结果& r,稳定编码 id) const noexcept{
        if(!r.物理清理事实代次||!r.物理清理墓碑)return false;
        const auto& m=*r.物理清理墓碑;
        return m.编码==id&&m.事实种类==L1所有者范围物理清理事实种类::节点
            &&m.内部结构分区==owner_.编码&&m.创建事实代次&&m.退出事实代次
            &&m.创建事实代次<m.退出事实代次
            &&m.退出事实代次<m.物理清理事实代次
            &&*r.物理清理事实代次==m.物理清理事实代次;
    }

    状态读取结果 读取历史(const 状态历史读取请求& r,std::optional<std::uint64_t> expectedExit) const{
        const auto nr=l1_.读取所有者范围历史事实({L1所有者范围CRUD合同版本,r.身份.编码});
        if(nr.合同版本!=L1所有者范围CRUD合同版本||nr.查询编码!=r.身份.编码)
            return 读取失败(状态类数据状态::内部不一致,nr.读取事实代次);
        if(nr.状态!=L1所有者范围读取状态::成功){
            if(nr.事实)return 读取失败(状态类数据状态::内部不一致,nr.读取事实代次);
            if(nr.状态==L1所有者范围读取状态::历史材料已清理
                &&(!墓碑节点回显完整(nr,r.身份.编码)))
                return 读取失败(状态类数据状态::内部不一致,nr.读取事实代次);
            if(nr.状态==L1所有者范围读取状态::历史材料已清理&&nr.读取事实代次!=r.Gread)
                return 读取失败(状态类数据状态::事实代次漂移,nr.读取事实代次);
            if(nr.状态!=L1所有者范围读取状态::历史材料已清理
                &&(nr.物理清理事实代次||nr.物理清理墓碑))
                return 读取失败(状态类数据状态::内部不一致,nr.读取事实代次);
            return 读取失败(映射读取(nr.状态),nr.读取事实代次);}
        if(!nr.事实||nr.物理清理事实代次||nr.物理清理墓碑)
            return 读取失败(状态类数据状态::内部不一致,nr.读取事实代次);
        if(nr.读取事实代次!=r.Gread)return 读取失败(状态类数据状态::事实代次漂移,nr.读取事实代次);
        const auto* n = nr.事实
            ? std::get_if<L1所有者范围节点事实>(&*nr.事实)
            : nullptr;
        if(!n||n->编码!=r.身份.编码||n->写入所有者!=owner_||n->种类!=节点种类::普通||n->属性类型表示||!n->创建事实代次)
            return 读取失败(状态类数据状态::旧格式不支持,r.Gread);
        if(n->创建事实代次>r.H)return 读取失败(状态类数据状态::未找到,r.Gread);
        if(n->退出事实代次&&*n->退出事实代次<=r.H)
            return 读取失败(状态类数据状态::目标已退出,r.Gread);
        if(expectedExit&&n->退出事实代次!=expectedExit)
            return 读取失败(expectedExit ? 状态类数据状态::内部不一致 : 状态类数据状态::目标已退出,r.Gread);
        const auto familyRead=读取唯一关系(r.身份.编码,layout_.状态族归属关系类型,r.Gread,r.H);
        if(familyRead.状态!=状态类数据状态::已读取)return 读取失败(familyRead.状态,familyRead.Gread);
        const auto ftRead=读取唯一关系(r.身份.编码,layout_.正式特征类型关系类型,r.Gread,r.H);
        if(ftRead.状态!=状态类数据状态::已读取)return 读取失败(ftRead.状态,ftRead.Gread);
        const auto& family=*familyRead.关系;
        const auto& ft=*ftRead.关系;
        const auto ar=l1_.读取所有者范围历史属性值组({L1所有者范围CRUD合同版本,r.身份.编码,r.H});
        if(ar.合同版本!=L1所有者范围CRUD合同版本||ar.所属节点!=r.身份.编码||ar.历史截止事实代次!=r.H)
            return 读取失败(状态类数据状态::内部不一致,ar.读取事实代次);
        if(ar.状态!=L1所有者范围读取状态::成功){if(!ar.属性值组.empty())return 读取失败(状态类数据状态::内部不一致,ar.读取事实代次);
            return 读取失败(映射读取(ar.状态),ar.读取事实代次);}
        if(ar.读取事实代次!=r.Gread)return 读取失败(状态类数据状态::事实代次漂移,ar.读取事实代次);
        if(family.目标节点!=layout_.状态族锚点||family.角色或顺序!=1||ft.角色或顺序!=1||!有效(ft.目标节点)||ar.属性值组.size()!=3)
            return 读取失败(状态类数据状态::旧格式不支持,r.Gread);
        const L1所有者范围值事实 *accurate=nullptr,*time=nullptr,*utc=nullptr;状态固定准确值 accurateValue;状态强时间 strong;
        for(const auto& v:ar.属性值组){if(!值共同有效(v,*n,n->退出事实代次))return 读取失败(状态类数据状态::内部不一致,r.Gread);
            if(v.属性类型节点==layout_.内联准确值属性类型){const auto* x=std::get_if<std::int64_t>(&v.材料);if(!x||accurate)return 读取失败(状态类数据状态::旧格式不支持,r.Gread);accurate=&v;accurateValue=*x;}
            else if(v.属性类型节点==layout_.引用准确值属性类型){const auto* x=std::get_if<std::vector<std::uint64_t>>(&v.材料);if(!x||x->size()!=1||!x->front()||accurate)return 读取失败(状态类数据状态::旧格式不支持,r.Gread);accurate=&v;accurateValue=特征值身份{{x->front()}};}
            else if(v.属性类型节点==layout_.实例绝对时间属性类型||v.属性类型节点==layout_.抽象相对时间属性类型){const auto* x=std::get_if<std::int64_t>(&v.材料);if(!x||*x<0||time)return 读取失败(状态类数据状态::旧格式不支持,r.Gread);time=&v;strong={v.属性类型节点==layout_.实例绝对时间属性类型
                    ? 状态强时间语义::实例绝对UTC纳秒
                    : 状态强时间语义::抽象相对纳秒,*x};}
            else if(v.属性类型节点==layout_.首次形成UTC属性类型){const auto* x=std::get_if<std::int64_t>(&v.材料);if(!x||*x<0||utc)return 读取失败(状态类数据状态::旧格式不支持,r.Gread);utc=&v;}
            else return 读取失败(状态类数据状态::旧格式不支持,r.Gread);}
        if(!accurate||!time||!utc||!同生命周期(*n,family,n->退出事实代次)||!同生命周期(*n,ft,n->退出事实代次))return 读取失败(状态类数据状态::内部不一致,r.Gread);
        状态内容事实 out{r.Gread,r.H,{r.身份,{ft.目标节点},accurateValue,strong},family.编码,ft.编码,accurate->编码,time->编码,utc->编码,
            std::get<std::int64_t>(utc->材料),n->创建事实代次,n->退出事实代次};
        return {{状态类数据状态::已读取,2,r.Gread,std::nullopt},std::move(out)};
    }
    唯一关系读取结果 读取唯一关系(稳定编码 source,稳定编码 type,std::uint64_t g,std::uint64_t h) const{
        const auto q=l1_.读取所有者范围历史关系组({L1所有者范围CRUD合同版本,L1所有者范围关系端点方向::源,source,type,h});
        if(q.合同版本!=L1所有者范围CRUD合同版本||q.方向!=L1所有者范围关系端点方向::源
            ||q.端点节点!=source||q.关系类型节点!=type||q.历史截止事实代次!=h)
            return {状态类数据状态::内部不一致,q.读取事实代次,std::nullopt};
        if(q.状态!=L1所有者范围读取状态::成功){if(!q.关系组.empty())return {状态类数据状态::内部不一致,q.读取事实代次,std::nullopt};
            return {映射读取(q.状态),q.读取事实代次,std::nullopt};}
        if(q.读取事实代次!=g)return {状态类数据状态::事实代次漂移,q.读取事实代次,std::nullopt};
        if(q.关系组.size()!=1)return {状态类数据状态::旧格式不支持,g,std::nullopt};
        const auto& e=q.关系组.front();
        if(e.源节点!=source||e.关系类型节点!=type)return {状态类数据状态::内部不一致,g,std::nullopt};
        return {状态类数据状态::已读取,g,e};}
    template<class F> bool 同生命周期(const L1所有者范围节点事实& n,const F& f,std::optional<std::uint64_t> exit) const noexcept{
        return f.写入所有者==owner_&&f.源节点==n.编码&&f.创建事实代次==n.创建事实代次&&f.退出事实代次==exit;}
    bool 值共同有效(const L1所有者范围值事实& v,const L1所有者范围节点事实& n,std::optional<std::uint64_t> exit) const noexcept{
        return v.写入所有者==owner_&&v.所属节点==n.编码&&v.来源节点==n.编码&&v.创建事实代次==n.创建事实代次&&v.退出事实代次==exit;}

    状态组查询结果 完整读取候选(std::uint64_t g,std::vector<状态信息身份> ids) const{
        std::sort(ids.begin(),ids.end(),[](auto a,auto b){return a.编码<b.编码;});ids.erase(std::unique(ids.begin(),ids.end()),ids.end());
        状态组查询结果 out{{状态类数据状态::已读取,2,g,std::nullopt},{}};
        for(auto id:ids){auto r=读取当前状态({2,g,id});if(!r.成功())return 组失败(r.结果头.状态,r.结果头.事实截止代次);out.状态组.push_back(*r.内容);}return out;
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
            ||first.首次写入结果->状态!=L1所有者范围写入状态::成功||!写入头完整(*first.首次写入结果,r.幂等身份,ws.期望事实代次)
            ||!first.首次写入结果->新编码映射.empty())return 退出失败(状态类数据状态::幂等冲突,first.读取事实代次);
        const auto saved=port_.提交所有者范围中性写集(ws);if(saved.状态!=L1所有者范围写入状态::精确重复||!写入头完整(saved,r.幂等身份,ws.期望事实代次))
            return 退出失败(映射写入(saved.状态,状态类数据状态::已退出),saved.事实代次);
        auto read=读取历史({2,first.读取事实代次,saved.事实代次-1,r.身份},saved.事实代次);
        if(!read.成功())return 退出失败(read.结果头.状态,read.结果头.事实截止代次);
        std::vector<稳定编码> expected;添加退出身份(expected,*read.内容);规范化退出组(expected);
        if(expected!=ws.退出事实)return 退出失败(状态类数据状态::幂等冲突,saved.事实代次);
        read.内容->Gread=saved.事实代次;
        return 状态退出结果{{状态类数据状态::精确重复,2,saved.事实代次,saved.事实代次},std::move(read.内容)};
    }
    std::optional<状态同步清理结果> 重放清理(const 状态同步清理请求& r){
        const auto first=port_.读取首次写入材料({L1所有者范围首次写入读取合同版本,r.退出幂等身份});if(first.状态==L1所有者范围读取状态::未找到){
            if(!首次读取回显完整(first,r.退出幂等身份)||!first.读取事实代次||first.首次规范化写集||first.首次写入结果)return 清理失败(状态类数据状态::内部不一致,first.读取事实代次);return std::nullopt;}
        if(!首次读取回显完整(first,r.退出幂等身份))return 清理失败(状态类数据状态::内部不一致,first.读取事实代次);
        if(first.状态!=L1所有者范围读取状态::成功){
            if(first.首次规范化写集||first.首次写入结果)return 清理失败(状态类数据状态::内部不一致,first.读取事实代次);
            return 清理失败(映射读取(first.状态),first.读取事实代次);}
        if(!first.读取事实代次||!first.首次规范化写集||!first.首次写入结果)return 清理失败(状态类数据状态::内部不一致,first.读取事实代次);
        const auto& ws=*first.首次规范化写集;if(!退出写集形状有效(ws,ws.期望事实代次,r.退出幂等身份)
            ||first.首次写入结果->状态!=L1所有者范围写入状态::成功||!写入头完整(*first.首次写入结果,r.退出幂等身份,ws.期望事实代次)
            ||!first.首次写入结果->新编码映射.empty())return 清理失败(状态类数据状态::内部不一致,first.读取事实代次);
        const auto exited=port_.提交所有者范围中性写集(ws);if(exited.状态!=L1所有者范围写入状态::精确重复||!写入头完整(exited,r.退出幂等身份,ws.期望事实代次))
            return 清理失败(映射写入(exited.状态,状态类数据状态::已退出),exited.事实代次);
        if(r.G0<exited.事实代次)return 清理失败(状态类数据状态::内部不一致,first.读取事实代次);
        return 执行清理(形成物理清理请求(r.G0,r.清理幂等身份,ws.退出事实),exited.事实代次);
    }
    static L1物理清理请求 形成物理清理请求(std::uint64_t g,L1物理清理幂等身份 key,const std::vector<稳定编码>& ids){
        L1物理清理请求 q{L1物理清理合同版本,g,key,{}};for(auto id:ids)q.待清理事实身份组.push_back({L1物理清理事实种类::值,id});return q;}
    bool 清理事实组结构完整(const std::vector<L1所有者范围节点事实>& nodes,
        const std::vector<L1所有者范围关系事实>& relations,const std::vector<L1所有者范围值事实>& values,
        std::uint64_t exitG) const noexcept{
        for(const auto& n:nodes){
            if(!有效(n.编码)||n.写入所有者!=owner_||n.种类!=节点种类::普通||n.属性类型表示
                ||!n.创建事实代次||n.退出事实代次!=exitG)return false;
            std::size_t family=0,ft=0,accurate=0,time=0,utc=0;
            for(const auto& e:relations){if(e.源节点!=n.编码)continue;
                if(!有效(e.编码)||e.写入所有者!=owner_||e.创建事实代次!=n.创建事实代次
                    ||e.退出事实代次!=exitG||e.角色或顺序!=1)return false;
                if(e.关系类型节点==layout_.状态族归属关系类型&&e.目标节点==layout_.状态族锚点)++family;
                else if(e.关系类型节点==layout_.正式特征类型关系类型&&有效(e.目标节点))++ft;
                else return false;}
            for(const auto& v:values){if(v.所属节点!=n.编码)continue;
                if(!有效(v.编码)||v.来源节点!=n.编码||v.写入所有者!=owner_
                    ||v.创建事实代次!=n.创建事实代次||v.退出事实代次!=exitG)return false;
                if(v.属性类型节点==layout_.内联准确值属性类型){if(!std::holds_alternative<std::int64_t>(v.材料))return false;++accurate;}
                else if(v.属性类型节点==layout_.引用准确值属性类型){const auto* ids=std::get_if<std::vector<std::uint64_t>>(&v.材料);if(!ids||ids->size()!=1||!ids->front())return false;++accurate;}
                else if(v.属性类型节点==layout_.实例绝对时间属性类型){const auto* x=std::get_if<std::int64_t>(&v.材料);if(!x||*x<0)return false;++time;}
                else if(v.属性类型节点==layout_.首次形成UTC属性类型){const auto* x=std::get_if<std::int64_t>(&v.材料);if(!x||*x<0)return false;++utc;}
                else return false;}
            if(family!=1||ft!=1||accurate!=1||time!=1||utc!=1)return false;
        }
        for(const auto& e:relations)if(std::none_of(nodes.begin(),nodes.end(),[&](const auto& n){return n.编码==e.源节点;}))return false;
        for(const auto& v:values)if(std::none_of(nodes.begin(),nodes.end(),[&](const auto& n){return n.编码==v.所属节点;}))return false;
        return true;
    }
    bool 清理结果完整(const L1物理清理结果& cleaned,const L1物理清理请求& q,std::uint64_t exitG) const noexcept{
        const bool first=cleaned.状态==L1物理清理状态::已清理&&cleaned.是否形成内存权威发布;
        const bool replay=cleaned.状态==L1物理清理状态::精确重复&&!cleaned.是否形成内存权威发布;
        if(cleaned.合同版本!=L1物理清理合同版本||cleaned.幂等身份!=q.幂等身份
            ||q.期望事实代次==std::numeric_limits<std::uint64_t>::max()
            ||cleaned.物理清理事实代次!=q.期望事实代次+1
            ||!(first||replay)||cleaned.稳定编码映射.size()!=q.待清理事实身份组.size()
            ||cleaned.墓碑组.size()!=q.待清理事实身份组.size())return false;
        for(const auto& expected:q.待清理事实身份组){std::size_t mappings=0,tombs=0;
            for(const auto& [id,code]:cleaned.稳定编码映射)if(id==expected){if(code!=expected.编码)return false;++mappings;}
            for(const auto& m:cleaned.墓碑组)if(m.编码==expected.编码){
                if(static_cast<L1物理清理事实种类>(m.事实种类)!=expected.事实种类
                    ||m.内部结构分区!=owner_.编码||!m.创建事实代次||m.创建事实代次>=exitG||m.退出事实代次!=exitG
                    ||m.物理清理事实代次!=cleaned.物理清理事实代次)return false;++tombs;}
            if(mappings!=1||tombs!=1)return false;}
        return true;
    }
    状态同步清理结果 执行清理(L1物理清理请求 q,std::uint64_t exitG){
        if(q.合同版本!=L1物理清理合同版本||!q.期望事实代次
            ||q.期望事实代次==std::numeric_limits<std::uint64_t>::max()||!q.幂等身份.值
            ||!exitG||q.期望事实代次<exitG||q.待清理事实身份组.empty()||q.待清理事实身份组.size()%6!=0)
            return 清理失败(状态类数据状态::内部不一致);
        std::vector<L1所有者范围节点事实> nodes;std::vector<L1所有者范围关系事实> relations;
        std::vector<L1所有者范围值事实> values;std::size_t tombstones=0;
        for(auto& item:q.待清理事实身份组){const auto raw=l1_.读取所有者范围历史事实({L1所有者范围CRUD合同版本,item.编码});
            if(raw.合同版本!=L1所有者范围CRUD合同版本||raw.查询编码!=item.编码)
                return 清理失败(状态类数据状态::内部不一致,raw.读取事实代次);
            if(raw.状态==L1所有者范围读取状态::成功&&raw.事实&&!raw.物理清理事实代次&&!raw.物理清理墓碑){
                if(const auto* n=std::get_if<L1所有者范围节点事实>(&*raw.事实)){item.事实种类=L1物理清理事实种类::节点;nodes.push_back(*n);}
                else if(const auto* e=std::get_if<L1所有者范围关系事实>(&*raw.事实)){item.事实种类=L1物理清理事实种类::关系;relations.push_back(*e);}
                else if(const auto* v=std::get_if<L1所有者范围值事实>(&*raw.事实)){item.事实种类=L1物理清理事实种类::值;values.push_back(*v);}
                else return 清理失败(状态类数据状态::内部不一致,raw.读取事实代次);
            }else if(raw.状态==L1所有者范围读取状态::历史材料已清理&&!raw.事实
                &&raw.物理清理事实代次&&raw.物理清理墓碑){
                const auto& m=*raw.物理清理墓碑;
                if(m.编码!=item.编码||m.内部结构分区!=owner_.编码||!m.创建事实代次
                    ||m.事实种类<L1所有者范围物理清理事实种类::节点
                    ||m.事实种类>L1所有者范围物理清理事实种类::值
                    ||m.创建事实代次>=m.退出事实代次||m.退出事实代次!=exitG
                    ||m.退出事实代次>=m.物理清理事实代次
                    ||m.物理清理事实代次!=*raw.物理清理事实代次)
                    return 清理失败(状态类数据状态::内部不一致,raw.读取事实代次);
                item.事实种类=static_cast<L1物理清理事实种类>(m.事实种类);++tombstones;
            }else return 清理失败(映射读取(raw.状态),raw.读取事实代次);
        }
        if(tombstones&&tombstones!=q.待清理事实身份组.size())return 清理失败(状态类数据状态::内部不一致);
        std::sort(q.待清理事实身份组.begin(),q.待清理事实身份组.end());
        for(std::size_t i=0;i<q.待清理事实身份组.size();++i)
            if(!L1物理清理事实身份有效(q.待清理事实身份组[i])
                ||(i&&q.待清理事实身份组[i].编码==q.待清理事实身份组[i-1].编码))
                return 清理失败(状态类数据状态::内部不一致);
        std::size_t nodeCount=0,relationCount=0,valueCount=0;for(const auto& id:q.待清理事实身份组){
            if(id.事实种类==L1物理清理事实种类::节点)++nodeCount;else if(id.事实种类==L1物理清理事实种类::关系)++relationCount;
            else if(id.事实种类==L1物理清理事实种类::值)++valueCount;else return 清理失败(状态类数据状态::内部不一致);}
        if(!nodeCount||relationCount!=nodeCount*2||valueCount!=nodeCount*3)return 清理失败(状态类数据状态::内部不一致);
        if(!tombstones&&!清理事实组结构完整(nodes,relations,values,exitG))return 清理失败(状态类数据状态::内部不一致);
        const auto cleaned=l1_.执行L1物理清理(q);const auto s=映射清理(cleaned.状态);
        if(s!=状态类数据状态::已清理&&s!=状态类数据状态::精确重复){
            if(tombstones&&(cleaned.状态==L1物理清理状态::事实代次漂移||cleaned.状态==L1物理清理状态::未找到))
                return 清理失败(状态类数据状态::幂等冲突,cleaned.物理清理事实代次);
            return 清理失败(s,cleaned.物理清理事实代次);}
        if(!清理结果完整(cleaned,q,exitG))return 清理失败(状态类数据状态::内部不一致,cleaned.物理清理事实代次);
        return {{s,2,cleaned.物理清理事实代次,cleaned.物理清理事实代次},cleaned.墓碑组};
    }
    状态身份来源历史见证读取结果 读取身份(const 状态身份来源历史见证读取请求& r) const{
        const auto nr=l1_.读取所有者范围历史事实({L1所有者范围CRUD合同版本,r.身份.编码});
        if(nr.合同版本!=L1所有者范围CRUD合同版本||nr.查询编码!=r.身份.编码)
            return 身份失败(状态类数据状态::内部不一致,nr.读取事实代次,r.H);
        if(nr.状态!=L1所有者范围读取状态::成功){
            if(nr.事实)return 身份失败(状态类数据状态::内部不一致,nr.读取事实代次,r.H);
            if(nr.状态==L1所有者范围读取状态::历史材料已清理
                &&(!墓碑节点回显完整(nr,r.身份.编码)))
                return 身份失败(状态类数据状态::内部不一致,nr.读取事实代次,r.H);
            if(nr.状态==L1所有者范围读取状态::历史材料已清理&&nr.读取事实代次!=r.Gread)
                return 身份失败(状态类数据状态::事实代次漂移,nr.读取事实代次,r.H);
            if(nr.状态!=L1所有者范围读取状态::历史材料已清理
                &&(nr.物理清理事实代次||nr.物理清理墓碑))
                return 身份失败(状态类数据状态::内部不一致,nr.读取事实代次,r.H);
            return 身份失败(映射读取(nr.状态),nr.读取事实代次,r.H);}
        if(!nr.事实||nr.物理清理事实代次||nr.物理清理墓碑)
            return 身份失败(状态类数据状态::内部不一致,nr.读取事实代次,r.H);
        if(nr.读取事实代次!=r.Gread)return 身份失败(状态类数据状态::事实代次漂移,nr.读取事实代次,r.H);
        const auto* n = nr.事实
            ? std::get_if<L1所有者范围节点事实>(&*nr.事实)
            : nullptr;
        if(!n||n->编码!=r.身份.编码)return 身份失败(状态类数据状态::内部不一致,r.Gread,r.H);
        if(n->写入所有者!=owner_||n->种类!=节点种类::普通||n->属性类型表示||!n->创建事实代次)
            return 身份失败(状态类数据状态::旧格式不支持,r.Gread,r.H);
        if(n->创建事实代次>r.H)return 身份失败(状态类数据状态::未找到,r.Gread,r.H);
        if(n->退出事实代次&&*n->退出事实代次<=r.H)return 身份失败(状态类数据状态::目标已退出,r.Gread,r.H);
        const auto familyRead=读取唯一关系(r.身份.编码,layout_.状态族归属关系类型,r.Gread,r.H);
        if(familyRead.状态!=状态类数据状态::已读取)return 身份失败(familyRead.状态,familyRead.Gread,r.H);
        const auto& family=*familyRead.关系;
        if(family.目标节点!=layout_.状态族锚点||family.角色或顺序!=1||!同生命周期(*n,family,n->退出事实代次))
            return 身份失败(状态类数据状态::旧格式不支持,r.Gread,r.H);
        const 稳定编码 witnessIds[]{r.身份.编码,layout_.状态族锚点,layout_.状态族归属关系类型,family.编码};
        for(std::size_t i=0;i<4;++i)for(std::size_t j=0;j<i;++j)if(witnessIds[i]==witnessIds[j])
            return 身份失败(状态类数据状态::内部不一致,r.Gread,r.H);
        状态身份来源历史见证 w{r.Gread,r.H,r.身份,layout_.状态族锚点,layout_.状态族归属关系类型,family.编码,n->创建事实代次,n->退出事实代次};
        return {{状态类数据状态::已读取,2,r.Gread,std::nullopt},r.H,std::move(w)};
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
    static 状态退出结果 退出失败(状态类数据状态 s,std::uint64_t g=0) noexcept{return {头(s,g),std::nullopt};}
    static 状态同步清理结果 清理失败(状态类数据状态 s,std::uint64_t g=0) noexcept{return {头(s,g),{}};}
    static 状态身份来源历史见证读取结果 身份失败(状态类数据状态 s,std::uint64_t g=0,std::uint64_t h=0) noexcept{return {头(s,g),h,std::nullopt};}
    static 状态类数据状态 映射特征(特征类型准确值核验状态 s) noexcept{switch(s){
        case 特征类型准确值核验状态::正式特征类型未找到:return 状态类数据状态::正式特征类型未找到;
        case 特征类型准确值核验状态::正式特征类型已退出:return 状态类数据状态::正式特征类型已退出;
        case 特征类型准确值核验状态::准确值未找到:case 特征类型准确值核验状态::准确值已退出:
        case 特征类型准确值核验状态::准确值不相容:return 状态类数据状态::准确值不相容;
        case 特征类型准确值核验状态::事实代次漂移:return 状态类数据状态::事实代次漂移;
        case 特征类型准确值核验状态::历史材料不可用:return 状态类数据状态::历史材料已清理;
        case 特征类型准确值核验状态::入口拒绝:return 状态类数据状态::入口拒绝;
        case 特征类型准确值核验状态::资源失败:return 状态类数据状态::资源失败;
        default:return 状态类数据状态::内部不一致;}}
    static 状态类数据状态 映射读取(L1所有者范围读取状态 s) noexcept{switch(s){
        case L1所有者范围读取状态::未找到:return 状态类数据状态::未找到;case L1所有者范围读取状态::已退出:return 状态类数据状态::目标已退出;
        case L1所有者范围读取状态::事实代次漂移:return 状态类数据状态::事实代次漂移;case L1所有者范围读取状态::历史材料已清理:return 状态类数据状态::历史材料已清理;
        case L1所有者范围读取状态::资源失败:return 状态类数据状态::资源失败;case L1所有者范围读取状态::入口拒绝:return 状态类数据状态::入口拒绝;default:return 状态类数据状态::内部不一致;}}
    static 状态类数据状态 映射组读取(L1所有者范围属性类型当前值组读取状态 s) noexcept{switch(s){
        case L1所有者范围属性类型当前值组读取状态::成功:return 状态类数据状态::已读取;case L1所有者范围属性类型当前值组读取状态::事实代次漂移:return 状态类数据状态::事实代次漂移;
        case L1所有者范围属性类型当前值组读取状态::数量预算不足:return 状态类数据状态::数量预算不足;case L1所有者范围属性类型当前值组读取状态::资源失败:return 状态类数据状态::资源失败;
        default:return 状态类数据状态::内部不一致;}}
    static 状态类数据状态 映射写入(L1所有者范围写入状态 s,状态类数据状态 ok) noexcept{switch(s){
        case L1所有者范围写入状态::成功:return ok;case L1所有者范围写入状态::精确重复:return 状态类数据状态::精确重复;case L1所有者范围写入状态::事实代次漂移:return 状态类数据状态::事实代次漂移;
        case L1所有者范围写入状态::幂等冲突:return 状态类数据状态::幂等冲突;case L1所有者范围写入状态::引用冲突:return 状态类数据状态::引用冲突;
        case L1所有者范围写入状态::资源失败:return 状态类数据状态::资源失败;default:return 状态类数据状态::内部不一致;}}
    static 状态类数据状态 映射清理(L1物理清理状态 s) noexcept{switch(s){case L1物理清理状态::已清理:return 状态类数据状态::已清理;
        case L1物理清理状态::精确重复:return 状态类数据状态::精确重复;case L1物理清理状态::事实代次漂移:return 状态类数据状态::事实代次漂移;
        case L1物理清理状态::引用冲突:return 状态类数据状态::引用冲突;case L1物理清理状态::同键冲突:return 状态类数据状态::幂等冲突;
        case L1物理清理状态::资源失败:return 状态类数据状态::资源失败;default:return 状态类数据状态::内部不一致;}}

    L1事实基座服务& l1_;const 特征类数据服务& feature_;L1所有者范围写端口 port_;L1结构所有者身份 owner_{};
    状态类结构交付 layout_;状态类保留策略 policy_;
};

} // namespace 海中鱼巣
