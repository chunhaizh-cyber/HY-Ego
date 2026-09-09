module;

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <variant>
#include <vector>

export module 海中鱼巣.领域.合同.动态结构;

export import 海中鱼巣.领域.合同.场景状态使用绑定;
export import 海中鱼巣.领域.算法.有序I64特征比较;

export namespace 海中鱼巣 {

inline constexpr std::uint32_t 动态数据合同版本 = 1;

struct 动态信息身份 final {
    稳定编码 编码{};
    friend bool operator==(const 动态信息身份&, const 动态信息身份&) = default;
};
inline bool 有效(动态信息身份 v) noexcept { return 有效(v.编码); }

enum class 动态成员角色 : std::uint8_t { 同主体 = 1, 组成存在 = 2 };

struct 动态组成历史依据 final {
    std::uint64_t 事实截止 = 0;
    std::int64_t 发生起始UTC纳秒 = 0;
    std::int64_t 发生终止UTC纳秒 = 0;
    std::vector<稳定编码> 由父到子的组成关系身份组;
};

struct 动态直接成员 final {
    std::uint32_t 顺序 = 0;
    动态成员角色 角色 = 动态成员角色::同主体;
    动态信息身份 子动态;
    std::optional<动态组成历史依据> 组成依据;
};

struct 动态原子内容 final { 状态使用绑定身份 前绑定, 后绑定; };
struct 动态组合内容 final { std::vector<动态直接成员> 成员组; };
using 动态变化内容 = std::variant<动态原子内容, 动态组合内容>;

struct 动态信息 final {
    动态信息身份 身份;
    稳定编码 主体存在{};
    动态变化内容 变化;
};

struct 动态读取预算 final {
    std::uint64_t 最大候选数 = 0;
    std::uint64_t 最大动态数 = 0;
    std::uint64_t 最大来源边数 = 0;
    std::uint64_t 最大深度 = 0;
    std::uint64_t 最大读取材料数 = 0;
};

struct 动态原子变化依据 final {
    特征比较执行请求 比较请求;
    特征比较执行结果 比较结果;
};

struct 动态自有事实清单 final {
    稳定编码 族归属关系{}, 主体关系{}, 首次形成UTC值{};
    std::vector<稳定编码> 变化关系组;
};

struct 动态内容事实 final {
    std::uint64_t Gread = 0, H = 0;
    动态信息 信息;
    动态自有事实清单 自有事实;
    std::int64_t 首次形成UTC纳秒 = 0;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
};

struct 动态原子来源事实 final {
    动态信息身份 动态;
    状态使用绑定事实 前绑定, 后绑定;
    状态内容事实 前状态, 后状态;
};

struct 动态来源展开事实 final {
    std::uint64_t Gread = 0, H = 0;
    动态信息身份 根动态;
    std::vector<动态内容事实> 动态组;
    std::vector<动态原子来源事实> 原子来源组;
};

enum class 动态数据状态 : std::uint8_t {
    已创建=1, 精确重复=2, 已读取=3, 已退出=4, 已清理=5,
    入口拒绝=6, 未找到=7, 目标已退出=8,
    来源未找到=9, 来源已退出=10, 历史材料已清理=11,
    绑定不完整=12, 主体不一致=13, 正式特征类型不一致=14,
    时间不递增=15, 无变化=16, 比较依据不支持=17,
    组成发生期依据不足=18, 重复成员=19, 来源成环=20,
    数量预算不足=21, 事实代次漂移=22, 幂等冲突=23,
    引用冲突=24, 旧格式不支持=25, 依赖未实现=26,
    资源失败=27, 内部不一致=28, 已可能发布=29
};

enum class 动态发布阶段 : std::uint8_t {
    无写入=0, 已提交待读回=1, 已读回=2, 未知=3
};

struct 动态操作结果 final {
    动态数据状态 状态 = 动态数据状态::入口拒绝;
    std::uint32_t 合同版本 = 1;
    std::uint64_t Gread = 0, H = 0;
    std::optional<std::uint64_t> 发布代次;
    动态发布阶段 阶段 = 动态发布阶段::无写入;
};

struct 动态原子创建请求 final {
    std::uint32_t 合同版本=1; std::uint64_t G0=0, 来源截止=0;
    L1所有者范围写入幂等身份 幂等身份{};
    稳定编码 主体存在{}; 动态原子内容 变化;
    动态原子变化依据 变化依据; 动态读取预算 预算;
};
struct 动态组合创建请求 final {
    std::uint32_t 合同版本=1; std::uint64_t G0=0, 来源截止=0;
    L1所有者范围写入幂等身份 幂等身份{};
    稳定编码 主体存在{}; 动态组合内容 变化; 动态读取预算 预算;
};
struct 动态当前读取请求 final {
    std::uint32_t 合同版本=1; std::uint64_t G0=0;
    动态信息身份 身份; 动态读取预算 预算;
};
struct 动态历史读取请求 final {
    std::uint32_t 合同版本=1; std::uint64_t Gread=0,H=0;
    动态信息身份 身份; 动态读取预算 预算;
};
struct 动态按主体查询请求 final {
    std::uint32_t 合同版本=1; std::uint64_t Gread=0,H=0;
    稳定编码 主体存在{}; 动态读取预算 预算;
};
struct 动态按绑定查询请求 final {
    std::uint32_t 合同版本=1; std::uint64_t Gread=0,H=0;
    状态使用绑定身份 绑定; 动态读取预算 预算;
};
struct 动态按子动态查询请求 final {
    std::uint32_t 合同版本=1; std::uint64_t Gread=0,H=0;
    动态信息身份 子动态; 动态读取预算 预算;
};
enum class 动态展开方式 : std::uint8_t { 直接=1, 递归=2 };
struct 动态来源展开请求 final {
    std::uint32_t 合同版本=1; std::uint64_t Gread=0,H=0;
    动态信息身份 身份; 动态展开方式 方式=动态展开方式::递归;
    动态读取预算 预算;
};
struct 动态退出请求 final {
    std::uint32_t 合同版本=1; std::uint64_t G0=0;
    L1所有者范围写入幂等身份 幂等身份{};
    动态信息身份 身份; 动态读取预算 预算;
};
struct 动态清理请求 final {
    std::uint32_t 合同版本=1; std::uint64_t G0=0;
    L1所有者范围写入幂等身份 退出幂等身份{};
    L1物理清理幂等身份 清理幂等身份{};
    std::int64_t 当前UTC纳秒=0; 动态读取预算 预算;
};
struct 动态当前身份请求 final {
    std::uint32_t 合同版本=1; std::uint64_t G0=0; 动态信息身份 身份;
};
struct 动态历史身份请求 final {
    std::uint32_t 合同版本=1; std::uint64_t Gread=0,H=0; 动态信息身份 身份;
};

struct 动态单项结果 final {
    动态操作结果 操作; std::optional<动态内容事实> 内容;
    bool 成功() const noexcept;
};
struct 动态组结果 final {
    动态操作结果 操作; std::vector<动态内容事实> 动态组;
    bool 成功() const noexcept;
};
struct 动态展开结果 final {
    动态操作结果 操作; std::optional<动态来源展开事实> 来源;
    bool 成功() const noexcept;
};
struct 动态清理结果 final {
    动态操作结果 操作; std::vector<L1物理清理墓碑> 墓碑组;
    bool 成功() const noexcept;
};
struct 动态身份历史事实 final {
    std::uint64_t Gread=0,H=0; 动态信息身份 身份;
    稳定编码 族锚点{}, 族归属关系类型{}, 族归属关系{};
    std::uint64_t 创建事实代次=0;
    std::optional<std::uint64_t> 退出事实代次;
};
struct 动态身份历史结果 final {
    动态操作结果 操作; std::optional<动态身份历史事实> 见证;
    bool 成功() const noexcept;
};

struct 动态结构交付 final {
    稳定编码 格式锚点{}, 字段登记关系类型{};
    稳定编码 动态族锚点{}, 族归属关系类型{}, 主体关系类型{};
    稳定编码 前绑定关系类型{}, 后绑定关系类型{}, 同主体成员关系类型{};
    稳定编码 首次形成UTC属性类型{};
};
struct 动态保留策略 final {
    std::uint64_t 实例动态TTL纳秒=604800000000000ULL;
    std::uint64_t 实例动态数量上限=1000000;
};

} // namespace 海中鱼巣

namespace 动态合同细节 {
using namespace 海中鱼巣;
inline bool 预算有效(const 动态读取预算& v) noexcept {
    return v.最大候选数 && v.最大动态数 && v.最大来源边数
        && v.最大深度 && v.最大读取材料数;
}

inline bool 内容完整(const 动态内容事实& v) noexcept {
    try {
        if (!v.Gread || !v.H || v.H > v.Gread || !有效(v.信息.身份)
            || !有效(v.信息.主体存在) || v.首次形成UTC纳秒 < 0
            || !v.创建事实代次 || v.创建事实代次 > v.H
            || (v.退出事实代次 && v.H >= *v.退出事实代次)
            || v.信息.变化.valueless_by_exception()) return false;
        std::vector<稳定编码> ids{v.信息.身份.编码, v.自有事实.族归属关系,
            v.自有事实.主体关系, v.自有事实.首次形成UTC值};
        if (const auto* a=std::get_if<动态原子内容>(&v.信息.变化)) {
            if (!有效(a->前绑定)||!有效(a->后绑定)||a->前绑定==a->后绑定
                ||v.自有事实.变化关系组.size()!=2) return false;
        } else if (const auto* c=std::get_if<动态组合内容>(&v.信息.变化)) {
            if(c->成员组.empty()||v.自有事实.变化关系组.size()!=c->成员组.size())return false;
            for(std::size_t i=0;i<c->成员组.size();++i){const auto&m=c->成员组[i];
                if(m.顺序!=i+1||m.角色!=动态成员角色::同主体||m.组成依据||!有效(m.子动态))return false;
                for(std::size_t j=0;j<i;++j)if(c->成员组[j].子动态==m.子动态)return false;}
        } else return false;
        ids.insert(ids.end(),v.自有事实.变化关系组.begin(),v.自有事实.变化关系组.end());
        for(std::size_t i=0;i<ids.size();++i){if(!有效(ids[i]))return false;
            for(std::size_t j=0;j<i;++j)if(ids[i]==ids[j])return false;}
        return true;
    } catch (...) { return false; }
}
inline bool 读取头(const 动态操作结果&o)noexcept{return o.状态==动态数据状态::已读取
    &&o.合同版本==1&&o.Gread&&o.H&&o.H<=o.Gread&&!o.发布代次&&o.阶段==动态发布阶段::无写入;}
}

export namespace 海中鱼巣 {

inline bool 动态单项结果::成功() const noexcept {
    const auto repeated=操作.状态==动态数据状态::精确重复;
    const auto create=操作.状态==动态数据状态::已创建
        ||(repeated&&内容&&操作.发布代次&&操作.H==*操作.发布代次);
    const auto read=操作.状态==动态数据状态::已读取;
    const auto exit=操作.状态==动态数据状态::已退出
        ||(repeated&&内容&&操作.发布代次&&*操作.发布代次>1
            &&操作.H==*操作.发布代次-1);
    if(!内容||!动态合同细节::内容完整(*内容)||内容->Gread!=操作.Gread||内容->H!=操作.H)return false;
    if(create)return 操作.合同版本==1&&操作.发布代次&&*操作.发布代次
        &&操作.阶段==动态发布阶段::已读回&&操作.Gread>=*操作.发布代次
        &&操作.H==*操作.发布代次&&内容->创建事实代次==*操作.发布代次&&!内容->退出事实代次;
    if(read)return 动态合同细节::读取头(操作);
    if(exit)return 操作.合同版本==1&&操作.发布代次&&*操作.发布代次>1
        &&操作.阶段==动态发布阶段::已读回&&操作.Gread>=*操作.发布代次
        &&操作.H==*操作.发布代次-1&&内容->退出事实代次
        &&*内容->退出事实代次==*操作.发布代次;
    return false;
}

inline bool 动态组结果::成功() const noexcept {
    if(!动态合同细节::读取头(操作))return false;
    稳定编码 previous{};
    for(const auto&v:动态组){if(!动态合同细节::内容完整(v)||v.Gread!=操作.Gread||v.H!=操作.H
        ||(有效(previous)&&!(previous<v.信息.身份.编码)))return false;previous=v.信息.身份.编码;}
    return true;
}

inline bool 动态展开结果::成功() const noexcept {
    if(!动态合同细节::读取头(操作)||!来源||来源->Gread!=操作.Gread||来源->H!=操作.H
        ||!有效(来源->根动态))return false;
    稳定编码 previous{};
    for(const auto&v:来源->动态组){if(!动态合同细节::内容完整(v)||v.Gread!=操作.Gread||v.H!=操作.H
        ||(有效(previous)&&!(previous<v.信息.身份.编码)))return false;previous=v.信息.身份.编码;}
    if(!std::binary_search(来源->动态组.begin(),来源->动态组.end(),来源->根动态.编码,
        [](const auto&l,const auto&r){if constexpr(requires{l.信息;})return l.信息.身份.编码<r;else return l<r.信息.身份.编码;}))return false;
    previous={};
    for(const auto&v:来源->原子来源组){
        if(!有效(v.动态)||!状态使用绑定事实完整(v.前绑定)
            ||!状态使用绑定事实完整(v.后绑定)||!状态内容事实完整(v.前状态)||!状态内容事实完整(v.后状态)
            ||v.前绑定.Gread!=操作.Gread||v.前绑定.H!=操作.H
            ||v.后绑定.Gread!=操作.Gread||v.后绑定.H!=操作.H
            ||v.前状态.Gread!=操作.Gread||v.前状态.H!=操作.H
            ||v.后状态.Gread!=操作.Gread||v.后状态.H!=操作.H
            ||v.前绑定.信息.状态!=v.前状态.信息.身份
            ||v.后绑定.信息.状态!=v.后状态.信息.身份
            ||(有效(previous)&&!(previous<v.动态.编码)))return false;
        const auto d=std::lower_bound(来源->动态组.begin(),来源->动态组.end(),v.动态.编码,
            [](const auto&l,const auto&r){return l.信息.身份.编码<r;});
        if(d==来源->动态组.end()||d->信息.身份!=v.动态)return false;
        const auto*a=std::get_if<动态原子内容>(&d->信息.变化);
        if(!a||a->前绑定!=v.前绑定.信息.身份||a->后绑定!=v.后绑定.信息.身份)return false;
        previous=v.动态.编码;
    }
    for(const auto&d:来源->动态组){
        const auto*a=std::get_if<动态原子内容>(&d.信息.变化);
        if(!a)continue;
        const auto s=std::lower_bound(来源->原子来源组.begin(),来源->原子来源组.end(),d.信息.身份.编码,
            [](const auto&l,const auto&r){return l.动态.编码<r;});
        if(s==来源->原子来源组.end()||s->动态!=d.信息.身份)return false;
    }
    return true;
}

inline bool 动态清理结果::成功() const noexcept {
    if(操作.状态==动态数据状态::已读取)return 动态合同细节::读取头(操作)
        &&操作.Gread==操作.H&&墓碑组.empty();
    if((操作.状态!=动态数据状态::已清理&&操作.状态!=动态数据状态::精确重复)
        ||操作.合同版本!=1||!操作.发布代次||!*操作.发布代次||操作.阶段!=动态发布阶段::已读回
        ||!操作.Gread||操作.Gread<操作.H||操作.H!=*操作.发布代次||墓碑组.empty())return false;
    L1物理清理事实身份 previous{};bool have=false;
    for(const auto&v:墓碑组){L1物理清理事实身份 id{v.事实种类,v.编码};
        if(!L1物理清理事实身份有效(id)||!v.创建事实代次||!v.退出事实代次
            ||v.创建事实代次>=v.退出事实代次||v.退出事实代次>=v.物理清理事实代次
            ||v.物理清理事实代次!=*操作.发布代次||(have&&!(previous<id)))return false;
        previous=id;have=true;}
    return true;
}

inline bool 动态身份历史结果::成功() const noexcept {
    if(!动态合同细节::读取头(操作)||!见证||见证->Gread!=操作.Gread||见证->H!=操作.H
        ||!有效(见证->身份)||!有效(见证->族锚点)||!有效(见证->族归属关系类型)
        ||!有效(见证->族归属关系)||!见证->创建事实代次||见证->创建事实代次>见证->H
        ||(见证->退出事实代次&&见证->H>=*见证->退出事实代次))return false;
    const 稳定编码 ids[]{见证->身份.编码,见证->族锚点,见证->族归属关系类型,见证->族归属关系};
    for(std::size_t i=0;i<4;++i)for(std::size_t j=0;j<i;++j)if(ids[i]==ids[j])return false;
    return true;
}

class 动态结构只读提供者 {
public:
    virtual ~动态结构只读提供者() = default;
    virtual bool 绑定于(const L1事实基座服务&) const noexcept = 0;
    virtual 动态单项结果 读取当前动态(const 动态当前读取请求&) const = 0;
    virtual 动态单项结果 读取动态历史(const 动态历史读取请求&) const = 0;
    virtual 动态组结果 按主体查询动态(const 动态按主体查询请求&) const = 0;
    virtual 动态组结果 按绑定反查原子动态(const 动态按绑定查询请求&) const = 0;
    virtual 动态组结果 按子动态反查父动态(const 动态按子动态查询请求&) const = 0;
    virtual 动态展开结果 展开动态来源(const 动态来源展开请求&) const = 0;
    virtual 动态操作结果 确认当前动态结构身份(const 动态当前身份请求&) const = 0;
    virtual 动态身份历史结果 读取动态身份历史(const 动态历史身份请求&) const = 0;
};

} // namespace 海中鱼巣
