module;
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstddef>
#include <exception>
#include <mutex>
#include <limits>
#include <optional>
#include <set>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

export module 海中鱼巣.业务.应用服务.特征概念类;
export import 海中鱼巣.领域.数据服务.概念树类;

export namespace 海中鱼巣 {
struct 共享特征应用预算 final {
    概念树共享预算 数据;
    std::uint64_t 最大派发步骤 = 0, 最大保留尝试 = 0, 最大遍历节点 = 0;
    friend bool operator==(const 共享特征应用预算 &, const 共享特征应用预算 &) = default;
};
struct 共享场景限制 final {
    概念树场景引用 S;
    概念树存在引用 E;
    std::uint64_t 原场景H = 0;
    friend bool operator==(const 共享场景限制 &, const 共享场景限制 &) = default;
};
struct 共享特征查找请求 final {
    std::uint32_t 版本 = 1;
    std::uint64_t Gread = 0, H = 0;
    概念树存在引用 E;
    概念树特征引用 F;
    稳定编码 值{}, 成员{};
    特征类型域身份 类型域;
    共享特征应用预算 预算;
    friend bool operator==(const 共享特征查找请求 &, const 共享特征查找请求 &) = default;
};
struct 共享特征实例读取请求 final {
    std::uint32_t 版本 = 1;
    std::uint64_t Gread = 0, H = 0;
    概念树存在引用 E;
    概念树特征引用 F;
    稳定编码 值{}, 成员{};
    特征类型域身份 类型域;
    共享特征应用预算 预算;
    friend bool operator==(const 共享特征实例读取请求 &, const 共享特征实例读取请求 &) = default;
};
struct 共享特征图请求 final {
    概念树图读取请求 图请求;
    概念树共享预算 预算;
};
struct 共享特征匹配事实 final {
    存在类结点 存在;
    特征类结点 特征;
    存在类成员引用 成员;
    特征类型域事实 类型域;
    std::vector<概念树共享定义事实> 最具体适用组;
    std::vector<概念树应用模板判定事实> 比较回执组;
    std::optional<概念树共享特征定义> 规范并集建议;
    特征长期只读用量 特征用量;
    bool 需显式治理 = false;
};
struct 共享特征实例事实 final {
    共享特征匹配事实 匹配;
    std::optional<概念树共享名称事实> 名称;
    std::optional<概念树共享定义事实> 名称概念;
};
enum class 共享特征业务状态 : std::uint8_t {
    完成=1,待继续,被后继覆盖,需显式治理,不支持,入口拒绝,预算不足,历史不可用,资源失败,内部不一致
};
enum class 共享特征阶段状态 : std::uint8_t { 未派发=1,部分确认,先前阶段待核验,全部确认 };
enum class 共享阶段角色 : std::uint8_t {
    新建F=1,加入E,积累观察,完整域,区间域,共同上位,完整父组,名称,实际适用,名称采用,治理,引用释放,概念退出,旧概念转换
};
enum class 共享步骤所有者 : std::uint8_t { 特征=1,存在,长期特征,概念 };
using 共享步骤目标 = std::variant<概念树共享特征定义,概念树概念身份>;
struct 共享待准备步骤 final {
    共享阶段角色 阶段角色=共享阶段角色::区间域;
    共享步骤目标 目标;
    friend bool operator==(const 共享待准备步骤&,const 共享待准备步骤&)=default;
};
struct 共享步骤键 final {
    共享阶段角色 阶段角色=共享阶段角色::区间域;
    共享步骤目标 目标;
    std::uint64_t key=0;
    friend bool operator==(const 共享步骤键&,const 共享步骤键&)=default;
};
struct 共享操作键组 final {
    std::uint64_t F新建key=0,E成员key=0,A观察key=0,TOPkey=0,名称key=0;
    std::vector<共享步骤键> 阶段键;
    friend bool operator==(const 共享操作键组&,const 共享操作键组&)=default;
};
struct 共享特征生成请求 final {
    std::uint32_t 版本=1;
    std::uint64_t Gread=0,原H=0;
    概念树存在引用 E;
    概念树特征类型引用 FT;
    特征类型域身份 类型域身份;
    特征跟踪身份 跟踪身份;
    特征类值 实际拟建值;
    std::uint64_t 观察标识=0;
    std::int64_t 时间=0;
    std::uint64_t 业务标识=0;
    共享操作键组 键组;
    共享特征应用预算 预算;
    friend bool operator==(const 共享特征生成请求&,const 共享特征生成请求&)=default;
};
struct 共享特征关联请求 final {
    std::uint32_t 版本=1;
    std::uint64_t Gread=0,H=0;
    概念树存在引用 E;
    概念树特征引用 F;
    稳定编码 值事实{},成员关系{};
    特征类型域身份 类型域身份;
    特征跟踪身份 跟踪身份;
    std::uint64_t 观察标识=0;
    std::int64_t 时间=0;
    std::uint64_t 业务标识=0;
    共享操作键组 键组;
    共享特征应用预算 预算;
    friend bool operator==(const 共享特征关联请求&,const 共享特征关联请求&)=default;
};
struct 共享特征治理请求 final {
    std::uint32_t 版本=1;
    std::uint64_t Gread=0,H=0;
    概念树概念身份 目标;
    稳定编码 当前生命周期值事实{};
    概念树生命周期 生命周期;
    概念树生命周期状态 预期状态=概念树生命周期状态::活跃,目标治理状态=概念树生命周期状态::活跃;
    std::vector<概念树直接上位事实> 预期父组,预期子组;
    std::optional<概念树共享实例见证> 重新采用见证;
    std::optional<std::vector<概念树共享子父替代>> 替代子父组;
    std::vector<概念树共享引用事实> 显式释放引用组;
    std::vector<共享步骤键> 阶段键;
    共享特征应用预算 预算;
    friend bool operator==(const 共享特征治理请求&,const 共享特征治理请求&)=default;
};
struct 共享特征转换请求 final {
    std::uint32_t 版本=1;
    std::uint64_t Gread=0,H=0;
    概念树概念身份 旧概念;
    特征类型域身份 类型域身份;
    std::uint64_t 新建key=0;
    共享特征应用预算 预算;
    friend bool operator==(const 共享特征转换请求&,const 共享特征转换请求&)=default;
};
using 共享特征原业务=std::variant<共享特征生成请求,共享特征关联请求,共享特征治理请求,共享特征转换请求>;
using 共享步骤原请求=std::variant<特征类新增请求,存在类成员新增请求,特征长期观察积累请求,概念树共享写请求>;
using 共享步骤回执=std::variant<特征类结点结果,存在类结点结果,特征长期写入结果<特征长期观测事实>,概念树共享写入结果>;
struct 共享步骤 final {
    共享阶段角色 阶段角色=共享阶段角色::新建F;
    共享步骤所有者 owner=共享步骤所有者::特征;
    共享步骤目标 目标;
    共享步骤原请求 原请求;
    std::optional<std::uint64_t> 首次H;
    std::optional<共享步骤回执> 回执提示;
};
struct 共享缺步骤恢复材料 final {
    共享步骤 定位尝试;
    std::uint64_t 键历史H=0;
    std::optional<共享步骤> 历史候选;
};
struct 共享特征继续材料 final {
    std::uint32_t 版本=1;
    共享特征原业务 原业务;
    std::uint64_t 原H=0;
    std::optional<std::uint64_t> 生成观察H;
    std::vector<共享步骤> 步骤,归档;
    std::vector<概念树共享定义事实> 冻结域集;
    std::uint64_t 遍历位置=0,冻结G=0;
    std::vector<共享步骤键> 补充键;
    std::optional<特征类结点> 已定位F;
    std::optional<存在类成员引用> 已定位成员;
    std::optional<特征长期观测事实> 已定位观察;
    std::optional<概念树共享定义事实> 已定位概念;
    std::optional<概念树共享名称事实> 已定位名称;
    std::vector<共享待准备步骤> 待补键目标组;
    std::optional<共享场景限制> 场景限制;
    std::optional<共享缺步骤恢复材料> 缺步骤恢复;
};
struct 共享特征继续请求 final {
    std::uint32_t 版本=1;
    std::uint64_t Gread=0;
    共享特征继续材料 材料;
    std::vector<共享步骤键> 补充键;
    共享特征应用预算 本次预算;
};
using 共享特征确认事实=std::variant<特征类结点,存在类成员引用,特征长期观测事实,概念树应用存在事实,
    概念树共享定义事实,概念树共享名称事实,概念树共享用途事实,概念树直接上位事实,概念树概念事实>;
struct 共享特征操作结果 final {
    std::uint32_t 版本=1;
    共享特征业务状态 状态=共享特征业务状态::入口拒绝;
    共享特征阶段状态 阶段=共享特征阶段状态::未派发;
    std::uint64_t 原H=0,Gread=0;
    bool 可能已发布=false;
    std::vector<共享特征确认事实> 已确认事实组;
    std::optional<共享特征继续材料> 继续材料;
    std::optional<共享特征实例事实> 实例;
    bool 成功()const noexcept{return 版本==1&&状态==共享特征业务状态::完成&&阶段==共享特征阶段状态::全部确认&&原H&&Gread>=原H&&!可能已发布&&继续材料.has_value()&&!继续材料->缺步骤恢复;}
};

} // namespace 海中鱼巣

namespace 海中鱼巣::共享特征应用内部 {
using D = 概念树数据状态;
struct 失败 final { D 状态; };
[[noreturn]] inline void 拒绝(D state = D::入口拒绝) { throw 失败{state}; }
inline std::uint64_t 当前(const 概念树类数据服务 &c) {
    auto result = c.读取当前事实代次();
    if (!result.成功()) 拒绝(result.状态);
    return result.Gread;
}
inline void 尾守卫(const 概念树类数据服务 &c, std::uint64_t g) { if (当前(c) != g) 拒绝(D::事实代次漂移); }
inline D 映射(特征引用读取状态 state) noexcept {
    switch (state) {
    case 特征引用读取状态::历史材料不可用: return D::历史材料不可用;
    case 特征引用读取状态::事实代次漂移: return D::事实代次漂移;
    case 特征引用读取状态::数量预算不足: return D::数量预算不足;
    case 特征引用读取状态::资源失败: return D::资源失败;
    case 特征引用读取状态::内部不一致: return D::内部不一致;
    case 特征引用读取状态::未找到: return D::未找到;
    case 特征引用读取状态::目标已退出: return D::目标已退出;
    default: return D::引用冲突;
    }
}
inline D 映射(特征长期状态 state) noexcept {
    switch (state) {
    case 特征长期状态::预算不足: return D::数量预算不足;
    case 特征长期状态::事实代次漂移: return D::事实代次漂移;
    case 特征长期状态::历史材料不可用: return D::历史材料不可用;
    case 特征长期状态::资源失败: return D::资源失败;

    case 特征长期状态::入口拒绝: return D::入口拒绝;
    case 特征长期状态::类型不支持: case 特征长期状态::未启用: return D::不支持;
    case 特征长期状态::未找到: return D::未找到;
    case 特征长期状态::引用冲突: return D::引用冲突;
    case 特征长期状态::幂等冲突: return D::幂等冲突;
    default: return D::内部不一致;
    }
}
inline bool 原子序(const 特征类型化原子域 &a, const 特征类型化原子域 &b) {
    if (a.域.index() != b.域.index()) return a.域.index() < b.域.index();
    if (const auto *x = std::get_if<特征I64闭区间>(&a.域)) {
        const auto &y = std::get<特征I64闭区间>(b.域);
        return x->下界 < y.下界 || (x->下界 == y.下界 && x->上界 < y.上界);
    }
    const auto &x = std::get<特征二值半径域>(a.域), &y = std::get<特征二值半径域>(b.域);
    return x.代表.像素 < y.代表.像素 || (x.代表.像素 == y.代表.像素 && x.半径 < y.半径);
}
// 只比较已由 owner 完整读回的抽象集合，不实现距离或近似包含。
inline bool 包含(const 概念树共享特征定义 &outer, const 概念树共享特征定义 &inner) {
    if (outer.FT != inner.FT || outer.类型 != inner.类型) return false;
    if (outer.完整域) return true;
    if (inner.完整域) return false;
    if (outer.类型.算法 == 特征长期算法::二值边界平均)
        return std::all_of(inner.原子组.begin(), inner.原子组.end(), [&](const auto &a) {
            return std::find(outer.原子组.begin(), outer.原子组.end(), a) != outer.原子组.end();
        });
    for (const auto &a : inner.原子组) {
        const auto &x = std::get<特征I64闭区间>(a.域);
        if (std::none_of(outer.原子组.begin(), outer.原子组.end(), [&](const auto &b) {
            const auto &y = std::get<特征I64闭区间>(b.域); return y.下界 <= x.下界 && x.上界 <= y.上界;
        })) return false;
    }
    return true;
}
inline 概念树共享特征定义 并集(const std::vector<概念树共享定义事实> &facts, const 概念树共享预算 &b) {
    if (facts.empty()) 拒绝(D::内部不一致);
    auto out = facts.front().共享定义;
    out.完整域 = false; out.原子组.clear();
    std::uint64_t pixels = 0;
    for (const auto &f : facts) {
        const auto &d = f.共享定义;
        if (d.FT != out.FT || d.类型 != out.类型) 拒绝(D::内部不一致);
        if (d.完整域) { out.完整域 = true; out.原子组.clear(); return out; }
        if (out.原子组.size() > b.最大原子数 || d.原子组.size() > b.最大原子数 - out.原子组.size()) 拒绝(D::数量预算不足);
        for (const auto &a : d.原子组) if (const auto *image = std::get_if<特征二值半径域>(&a.域)) {
            if (pixels > b.特征.最大像素数 || image->代表.像素.size() > b.特征.最大像素数 - pixels) 拒绝(D::数量预算不足);
            pixels += image->代表.像素.size();
        }
        out.原子组.insert(out.原子组.end(), d.原子组.begin(), d.原子组.end());
    }
    std::sort(out.原子组.begin(), out.原子组.end(), 原子序);
    if (out.类型.算法 == 特征长期算法::二值边界平均)
        out.原子组.erase(std::unique(out.原子组.begin(), out.原子组.end()), out.原子组.end());
    else {
        std::vector<特征类型化原子域> normalized;
        for (const auto &a : out.原子组) {
            const auto &x = std::get<特征I64闭区间>(a.域);
            if (!normalized.empty()) {
                auto &last = std::get<特征I64闭区间>(normalized.back().域);
                if (x.下界 <= last.上界 || (last.上界 != INT64_MAX && x.下界 == last.上界 + 1)) {
                    last.上界 = std::max(last.上界, x.上界); continue;
                }
            }
            normalized.push_back(a);
        }
        out.原子组 = std::move(normalized);
        if (out.原子组.size() == 1 && std::get<特征I64闭区间>(out.原子组.front().域) == std::get<特征I64闭区间>(out.类型.完整域)) {
            out.完整域 = true; out.原子组.clear();
        }
    }
    return out;
}
inline std::array<std::uint64_t,7> 用量数组(const 特征长期只读用量 &u) {
    return {u.记录数,u.关系数,u.属性数,u.样本数,u.像素数,u.点对数,u.历史数};
}
inline 特征长期预算 剩余(const 特征长期预算 &b, const 特征长期只读用量 &u) {
    auto out = b;
    const auto take = [](std::uint64_t &limit,std::uint64_t used) { if (used > limit) 拒绝(D::数量预算不足); limit -= used; };
    take(out.最大记录数,u.记录数);take(out.最大关系数,u.关系数);take(out.最大属性数,u.属性数);
    take(out.最大样本数,u.样本数);take(out.最大像素数,u.像素数);take(out.最大边界点对数,u.点对数);take(out.最大历史事实数,u.历史数);
    return out;
}
inline void 累加(特征长期只读用量 &a, const 特征长期只读用量 &v, const 特征长期预算 &b) {
    const auto remaining=剩余(b,a);
    (void)剩余(remaining,v);
    a.记录数+=v.记录数;a.关系数+=v.关系数;a.属性数+=v.属性数;a.样本数+=v.样本数;
    a.像素数+=v.像素数;a.点对数+=v.点对数;a.历史数+=v.历史数;
}
struct 业务失败 final { 共享特征业务状态 状态; };
[[noreturn]] inline void 业务拒绝(共享特征业务状态 s){throw 业务失败{s};}
inline bool 合法键(std::uint64_t key)noexcept{return key&&(key&0xFFFF000000000000ULL)!=0x4E43000000000000ULL;}
inline std::uint64_t 原截止(const 共享特征原业务&b){return std::visit([](const auto&r){
    if constexpr(std::is_same_v<std::decay_t<decltype(r)>,共享特征生成请求>)return r.原H;else return r.H;
},b);}
inline std::uint64_t 初始G(const 共享特征原业务&b){return std::visit([](const auto&r){return r.Gread;},b);}
inline 共享特征业务状态 业务映射(D s)noexcept{
    switch(s){
    case D::未找到:case D::目标已退出:return 共享特征业务状态::被后继覆盖;
    case D::不支持:return 共享特征业务状态::不支持;
    case D::数量预算不足:return 共享特征业务状态::预算不足;
    case D::历史材料不可用:return 共享特征业务状态::历史不可用;
    case D::资源失败:return 共享特征业务状态::资源失败;
    case D::入口拒绝:case D::引用冲突:case D::幂等冲突:return 共享特征业务状态::入口拒绝;
    case D::事实代次漂移:return 共享特征业务状态::待继续;
    default:return 共享特征业务状态::内部不一致;
    }
}
inline std::pair<std::uint64_t,std::uint64_t> 步骤头(const 共享步骤原请求&request){
    return std::visit([](const auto&r)->std::pair<std::uint64_t,std::uint64_t>{using T=std::decay_t<decltype(r)>;
        if constexpr(std::is_same_v<T,特征类新增请求>||std::is_same_v<T,存在类成员新增请求>)return {r.期望事实代次,r.幂等身份.值};
        else if constexpr(std::is_same_v<T,特征长期观察积累请求>)return {r.写头.期望G,r.写头.幂等键.值};
        else return std::visit([](const auto&q)->std::pair<std::uint64_t,std::uint64_t>{
            if constexpr(requires{q.写入头;})return {q.写入头.期望事实代次,q.写入头.幂等身份};
            else return {q.操作.写入头.期望事实代次,q.操作.写入头.幂等身份};
        },r);
    },request);
}
inline void 键表有效(const std::vector<共享步骤键>&keys,std::set<std::uint64_t>&used){
    for(std::size_t i=0;i<keys.size();++i){const auto&k=keys[i];
        if(!合法键(k.key)||k.目标.valueless_by_exception()||!used.insert(k.key).second)拒绝();
        if(k.阶段角色<共享阶段角色::区间域||k.阶段角色>共享阶段角色::旧概念转换)拒绝();
        for(std::size_t j=0;j<i;++j)if(keys[j].阶段角色==k.阶段角色&&keys[j].目标==k.目标)拒绝();
    }
}
inline void 场景前置(const 概念树类数据服务&c,const 存在类数据服务&e,const 场景类数据服务*s,
    const std::optional<共享场景限制>&limit,概念树存在引用 E,std::uint64_t g,const 概念树共享预算&b){
    if(!limit)return;
    if(!s||!c.使用同一场景基座(*s)||!s->使用存在服务(e)||limit->E!=E||!有效(limit->S.值)||!limit->原场景H||limit->原场景H>g)拒绝();
    for(auto h:{limit->原场景H,g}){
        auto fact=s->读取场景历史事实({1,g,h,limit->S.值,b.基础.最大世界成员数});
        if(!fact.成功()){
            switch(fact.状态){
            case 场景类数据状态::未找到:case 场景类数据状态::目标已退出:业务拒绝(共享特征业务状态::被后继覆盖);
            case 场景类数据状态::事实代次漂移:拒绝(D::事实代次漂移);
            case 场景类数据状态::历史材料已清理:拒绝(D::历史材料不可用);
            case 场景类数据状态::资源失败:拒绝(D::资源失败);
            case 场景类数据状态::数量预算不足:拒绝(D::数量预算不足);
            case 场景类数据状态::入口拒绝:拒绝();
            default:拒绝(D::内部不一致);
            }
        }
        if(fact.Gread!=g||fact.H!=h||fact.场景结点->结点!=limit->S.值)拒绝(D::内部不一致);
        if(std::count_if(fact.场景结点->存在组.begin(),fact.场景结点->存在组.end(),[&](const auto&m){return m.目标结点==E.值;})!=1)
            业务拒绝(共享特征业务状态::被后继覆盖);
    }
}

inline 共享步骤原请求 调整预算(共享步骤原请求 q,const 共享特征应用预算&b){
    std::visit([&](auto&r){using T=std::decay_t<decltype(r)>;
        if constexpr(std::is_same_v<T,特征长期观察积累请求>)r.预算=b.数据.特征;
        else if constexpr(std::is_same_v<T,概念树共享写请求>)std::visit([&](auto&w){w.预算=b.数据;
            if constexpr(requires{w.操作.预算;})w.操作.预算=b.数据.基础;
        },r);
    },q);return q;
}
inline bool 步骤等义(const 共享步骤原请求&a,const 共享步骤原请求&b){return 调整预算(a,{})==调整预算(b,{});}
inline std::uint64_t 保留尝试数(const 共享特征继续材料&m)noexcept{
    return m.步骤.size()+m.归档.size()+(m.缺步骤恢复 ? 1+(m.缺步骤恢复->历史候选.has_value() ? 1 : 0) : 0);
}
inline bool 历史可恢复(const 共享步骤&s)noexcept{
    if(s.owner!=共享步骤所有者::概念||s.原请求.index()!=3||s.目标.valueless_by_exception())return false;
    const auto*q=std::get_if<概念树共享写请求>(&s.原请求);if(!q||q->valueless_by_exception())return false;
    if(std::holds_alternative<概念树共享建立请求>(*q))return s.阶段角色==共享阶段角色::完整域||s.阶段角色==共享阶段角色::区间域||s.阶段角色==共享阶段角色::共同上位;
    if(std::holds_alternative<概念树共享上位请求>(*q))return s.阶段角色==共享阶段角色::完整父组;
    if(std::holds_alternative<概念树共享名称请求>(*q))return s.阶段角色==共享阶段角色::名称;
    if(std::holds_alternative<概念树共享使用请求>(*q))return s.阶段角色==共享阶段角色::实际适用||s.阶段角色==共享阶段角色::名称采用;
    return false;
}
inline void 核验历史步骤目标(const 共享步骤&s){
    if(!历史可恢复(s))拒绝();
    std::visit([&](const auto&r){using R=std::decay_t<decltype(r)>;
        if constexpr(std::is_same_v<R,概念树共享建立请求>){const auto*d=std::get_if<概念树共享特征定义>(&s.目标);if(!d||*d!=r.定义)拒绝();}
        else if constexpr(std::is_same_v<R,概念树共享上位请求>){const auto*id=std::get_if<概念树概念身份>(&s.目标);if(!id||*id!=r.下位)拒绝();}
        else if constexpr(std::is_same_v<R,概念树共享名称请求>){const auto*id=std::get_if<概念树概念身份>(&s.目标);if(!id||!r.新概念||*id!=*r.新概念)拒绝();}
        else if constexpr(std::is_same_v<R,概念树共享使用请求>){const auto*id=std::get_if<概念树概念身份>(&s.目标);if(!id||*id!=r.概念)拒绝();}
        else 拒绝();
    },std::get<概念树共享写请求>(s.原请求));
}

inline void 改写准备G(共享步骤原请求&q,std::uint64_t g){std::visit([&](auto&r){using T=std::decay_t<decltype(r)>;
    if constexpr(std::is_same_v<T,特征类新增请求>||std::is_same_v<T,存在类成员新增请求>)r.期望事实代次=g;
    else if constexpr(std::is_same_v<T,特征长期观察积累请求>)r.写头.期望G=g;
    else std::visit([&](auto&w){if constexpr(requires{w.写入头;})w.写入头.期望事实代次=g;else w.操作.写入头.期望事实代次=g;},r);
},q);}

} // namespace 海中鱼巣::共享特征应用内部

export namespace 海中鱼巣 {
class 特征概念应用服务 final {
    概念树类数据服务 &concepts_;
    特征类数据服务 &features_;
    存在类数据服务 &existences_;
    const 概念树概念身份 root_;
    inline static 概念树概念身份 特征概念根节点{};
    inline static std::mutex 根节点互斥_;
    inline static const 概念树类数据服务 *根绑定服务_ = nullptr;
    inline static std::size_t 根绑定实例数_ = 0;
    void 绑定全局根() {
        std::lock_guard lock(根节点互斥_);
        if (!根绑定实例数_) {
            if (根绑定服务_ || 有效(特征概念根节点.值)) throw std::logic_error("inconsistent concept root binding");
            特征概念根节点=root_;根绑定服务_=&concepts_;根绑定实例数_=1;return;
        }
        if (根绑定服务_!=&concepts_||特征概念根节点!=root_) throw std::invalid_argument("concept root is bound to another provider");
        if (根绑定实例数_==std::numeric_limits<std::size_t>::max()) throw std::length_error("concept root binding count exhausted");
        ++根绑定实例数_;
    }
    void 释放全局根() noexcept {
        std::lock_guard lock(根节点互斥_);
        if(!根绑定实例数_||根绑定服务_!=&concepts_||特征概念根节点!=root_)std::terminate();
        if(--根绑定实例数_==0){特征概念根节点={};根绑定服务_=nullptr;}
    }
    struct 运行会话 final {
        特征概念应用服务 &app;
        共享特征操作结果 &out;
        共享特征继续材料 &m;
        const 共享特征应用预算 &b;
        const 场景类数据服务 *scene;
        std::uint64_t g=0,调用数=0;
        bool 已派发历史候选=false;
        bool 未核验=false;
        std::set<std::size_t> 已核验;
        std::optional<std::size_t> 找步骤(共享阶段角色,const 共享步骤目标&)const;
        std::uint64_t 准备G(共享阶段角色,const 共享步骤目标&)const;
        std::uint64_t 取键(共享阶段角色,const 共享步骤目标&,std::uint64_t fixed=0);
        const 共享步骤回执&做步骤(共享阶段角色,const 共享步骤目标&,共享步骤原请求);
        const 共享步骤回执&确认步骤(std::size_t);
        const 共享步骤回执&恢复缺步骤(共享阶段角色,const 共享步骤目标&,共享步骤原请求);
        void 场景检查();
        void 当前实例检查();
    };

    void 推进转换(运行会话&);
    void 推进治理(运行会话&);
    共享特征操作结果 继续内部(const 共享特征继续请求&,const 共享场景限制*,const 场景类数据服务*);
    void 推进实例(运行会话&);
    void 推进域与名称(运行会话&,const 概念树共享特征定义&,概念树存在引用,特征类型域身份,const 特征观察见证&,std::uint64_t,std::uint64_t);
    共享步骤回执 派发(const 共享步骤原请求&r);
    void 核验纯材料(const 共享特征继续材料&,const 共享特征应用预算&,std::uint64_t)const;
    共享特征操作结果 推进(共享特征继续材料,std::uint64_t,const 共享特征应用预算&,const 场景类数据服务*);
    共享特征操作结果 开始(共享特征原业务,const std::optional<共享场景限制>&,const 场景类数据服务*);

    共享特征匹配事实 匹配(const 共享特征查找请求 &r, std::uint64_t g) const;
    template<class T,class Action> 概念树应用读取结果<T> 读取(std::uint32_t version,std::uint64_t g,std::uint64_t h,Action action) const {
        using namespace 共享特征应用内部;
        概念树应用读取结果<T> out;
        try {
            if(version!=1||!g||!h||h>g)拒绝();
            if(当前(concepts_)!=g)拒绝(D::事实代次漂移);
            out.Gread=g;out.H=h;out.数据=action(g);尾守卫(concepts_,g);out.状态=D::已读取;
        }catch(const 失败 &e){out.状态=e.状态;out.数据.reset();}
        catch(const std::bad_alloc&){out.状态=D::资源失败;out.数据.reset();}
        catch(const std::length_error&){out.状态=D::资源失败;out.数据.reset();}
        catch(...){out.状态=D::内部不一致;out.数据.reset();}
        return out;
    }
public:
    特征概念应用服务(概念树类数据服务 &c,特征类数据服务 &f,存在类数据服务 &e)
        :concepts_(c),features_(f),existences_(e),root_(c.特征根引用()){
        using namespace 共享特征应用内部;
        if(!有效(root_.值)||!c.共享应用已启用()||!c.使用特征存在服务(f,e))throw std::invalid_argument("shared feature service binding");
        const auto g=当前(c);概念树预算 b;
        b.最大概念数=b.最大关系数=b.最大来源数=b.最大支持数=b.最大世界成员数=b.最大特征属性数=1;
        const auto root=c.读取概念({{1,g,g},root_,b});
        if(!root.成功()||root.Gread!=g||root.H!=g||!root.概念||!root.概念->是本体根||root.概念->根角色!=概念树根角色::特征||
            root.概念->概念!=root_||root.概念->本体根!=root_||root.概念->定义||有效(root.概念->定义记录)||
            !root.概念->直接上位组.empty()||root.概念->治理状态!=概念树生命周期状态::活跃)throw std::invalid_argument("shared feature root facts");
        尾守卫(c,g);绑定全局根();
    }
    static std::optional<概念树概念身份> 读取特征概念根节点(){std::lock_guard lock(根节点互斥_);if(!根绑定实例数_)return std::nullopt;return 特征概念根节点;}
    ~特征概念应用服务()noexcept{释放全局根();}
    特征概念应用服务()=delete;
    特征概念应用服务(const 特征概念应用服务&)=delete;
    特征概念应用服务&operator=(const 特征概念应用服务&)=delete;
    特征概念应用服务(特征概念应用服务&&)=delete;
    特征概念应用服务&operator=(特征概念应用服务&&)=delete;
    bool 使用存在服务(const 存在类数据服务&e)const noexcept{return &e==&existences_;}
    bool 使用概念服务(const 概念树类数据服务&c)const noexcept{return &c==&concepts_;}
    共享特征操作结果 生成并关联共享特征(const 共享特征生成请求&r){return 开始(r,std::nullopt,nullptr);}
    共享特征操作结果 关联已有共享特征(const 共享特征关联请求&r){return 开始(r,std::nullopt,nullptr);}
    共享特征操作结果 生成并关联共享特征(const 共享特征生成请求&r,const 共享场景限制&s,const 场景类数据服务&provider){return 开始(r,s,&provider);}
    共享特征操作结果 关联已有共享特征(const 共享特征关联请求&r,const 共享场景限制&s,const 场景类数据服务&provider){return 开始(r,s,&provider);}
    共享特征操作结果 继续共享特征操作(const 共享特征继续请求&r);
    共享特征操作结果 继续共享特征操作(const 共享特征继续请求&r,const 共享场景限制&s,const 场景类数据服务&provider);
    共享特征操作结果 治理共享特征概念(const 共享特征治理请求&r){return 开始(r,std::nullopt,nullptr);}
    共享特征操作结果 转换旧特征概念(const 共享特征转换请求&r){return 开始(r,std::nullopt,nullptr);}

    概念树应用读取结果<共享特征匹配事实> 查找适用共享特征(const 共享特征查找请求&r)const{
        return 读取<共享特征匹配事实>(r.版本,r.Gread,r.H,[&](auto g){return 匹配(r,g);});
    }
    概念树应用读取结果<共享特征实例事实> 读取共享特征实例(const 共享特征实例读取请求&r)const;
    概念树应用读取结果<概念树应用图事实> 读取共享特征概念树(const 共享特征图请求&r)const{
        if(r.图请求.根角色!=概念树根角色::特征||r.图请求.预算!=r.预算.基础)return {};
        return concepts_.读取应用概念图({1,r.图请求,r.预算});
    }
};

共享特征匹配事实 特征概念应用服务::匹配(const 共享特征查找请求 &r,std::uint64_t g)const{
    using namespace 共享特征应用内部;
    if(!有效(r.E.值)||!有效(r.F.值)||!有效(r.值)||!有效(r.成员)||!有效(r.类型域.值)||!r.预算.最大遍历节点)拒绝();
    const auto &b=r.预算.数据;
    auto e=existences_.读取存在历史事实({1,g,r.H,r.E.值,b.基础.最大世界成员数});
    if(!e.成功())拒绝(映射(e.状态));
    if(e.Gread!=g||e.H!=r.H||e.存在->结点!=r.E.值)拒绝(D::内部不一致);
    const auto member=std::find_if(e.存在->特征组.begin(),e.存在->特征组.end(),[&](const auto&m){return m.成员关系==r.成员&&m.目标结点==r.F.值;});
    if(member==e.存在->特征组.end())拒绝(D::引用冲突);
    auto value=features_.按实例读取特征历史事实({1,g,r.H,r.F.值,b.基础.最大特征属性数});
    if(!value.成功())拒绝(映射(value.状态));
    if(value.Gread!=g||value.H!=r.H||value.特征->结点!=r.F.值||value.特征->值事实!=r.值)拒绝(D::引用冲突);
    auto type=features_.读取特征类型域({{1,g,r.H},r.类型域,b.特征});
    if(!type.成功())拒绝(映射(type.状态));
    if(type.Gread!=g||type.H!=r.H||type.数据->身份!=r.类型域||type.数据->内容.FT!=value.特征->特征类型)拒绝(D::引用冲突);
    auto domains=concepts_.读取共享特征域组({1,{1,g,r.H},概念树特征类型引用{value.特征->特征类型},b});
    if(!domains.成功())拒绝(domains.状态);
    if(domains.Gread!=g||domains.H!=r.H)拒绝(D::内部不一致);
    if(domains.数据->size()>r.预算.最大遍历节点)拒绝(D::数量预算不足);
    共享特征匹配事实 out;out.成员=*member;out.存在=std::move(*e.存在);out.特征=std::move(*value.特征);out.类型域=std::move(*type.数据);
    // 完整域组已由提供者核验全图；这里按真实父组建立从唯一 TOP 出发的拓扑遍历。
    const auto &group=*domains.数据;
    std::vector<std::vector<std::size_t>> children(group.size());
    std::vector<std::size_t> pending(group.size(),0),order;
    std::optional<std::size_t> top;
    for(std::size_t i=0;i<group.size();++i){
        const auto &d=group[i];
        if(d.共享定义.完整域){
            if(top||d.直接上位组.size()!=1||d.直接上位组.front().上位!=root_)拒绝(D::内部不一致);
            top=i;
        }else{
            if(d.直接上位组.empty())拒绝(D::内部不一致);
            std::set<std::size_t> parents;
            for(const auto&edge:d.直接上位组){
                auto parent=std::find_if(group.begin(),group.end(),[&](const auto&p){return p.概念==edge.上位;});
                if(edge.下位!=d.概念||parent==group.end()||parent->共享定义==d.共享定义||!包含(parent->共享定义,d.共享定义))拒绝(D::内部不一致);
                const auto j=static_cast<std::size_t>(parent-group.begin());
                if(!parents.insert(j).second)拒绝(D::内部不一致);
                children[j].push_back(i);++pending[i];
            }
        }
    }
    if(!group.empty()&&!top)拒绝(D::内部不一致);
    if(top)order.push_back(*top);
    for(std::size_t pos=0;pos<order.size();++pos){
        if(pos>=r.预算.最大遍历节点)拒绝(D::数量预算不足);
        for(const auto child:children[order[pos]]){if(!pending[child])拒绝(D::内部不一致);if(!--pending[child])order.push_back(child);}
    }
    if(order.size()!=group.size())拒绝(D::内部不一致);
    std::vector<概念树共享定义事实> applicable;
    for(const auto index:order){const auto&d=group[index];
        auto budget=b;budget.特征=剩余(b.特征,out.特征用量);
        auto decision=concepts_.判定应用特征模板({1,{1,g,r.H},d.概念,{r.F.值,r.值,out.特征.特征类型,r.H},budget});
        if(!decision.成功())拒绝(decision.状态);
        if(decision.Gread!=g||decision.H!=r.H||!std::holds_alternative<概念树共享定义事实>(decision.数据->模板)||
            std::get<概念树共享定义事实>(decision.数据->模板).概念!=d.概念)拒绝(D::内部不一致);
        累加(out.特征用量,decision.数据->特征用量,b.特征);
        if(decision.数据->适用)applicable.push_back(d);
        out.比较回执组.push_back(std::move(*decision.数据));
    }
    for(const auto &d:applicable){
        if(std::any_of(applicable.begin(),applicable.end(),[&](const auto&other){return other.概念!=d.概念&&other.共享定义!=d.共享定义&&包含(d.共享定义,other.共享定义);}))continue;
        out.最具体适用组.push_back(d);
        if(d.治理状态==概念树生命周期状态::退役)out.需显式治理=true;
    }
    if(!out.最具体适用组.empty())out.规范并集建议=并集(out.最具体适用组,b);
    return out;
}
概念树应用读取结果<共享特征实例事实> 特征概念应用服务::读取共享特征实例(const 共享特征实例读取请求&r)const{
    return 读取<共享特征实例事实>(r.版本,r.Gread,r.H,[&](auto g){
        using namespace 共享特征应用内部;
        共享特征实例事实 out;out.匹配=匹配({r.版本,g,r.H,r.E,r.F,r.值,r.成员,r.类型域,r.预算},g);
        auto names=concepts_.读取共享名称字段({1,{1,g,r.H},r.F,false,r.预算.数据});
        if(!names.成功())拒绝(names.状态);
        if(names.Gread!=g||names.H!=r.H||names.数据->size()>1)拒绝(D::内部不一致);
        if(!names.数据->empty()){
            out.名称=names.数据->front();
            auto named=concepts_.读取应用特征模板({1,{1,g,r.H},out.名称->概念,r.预算.数据});
            if(!named.成功())拒绝(named.状态);
            if(named.Gread!=g||named.H!=r.H||!std::holds_alternative<概念树共享定义事实>(*named.数据))拒绝(D::不支持);
            out.名称概念=std::get<概念树共享定义事实>(std::move(*named.数据));
        }
        return out;
    });
}
共享步骤回执 特征概念应用服务::派发(const 共享步骤原请求&r){
    return std::visit([&](const auto&q)->共享步骤回执{
        using T=std::decay_t<decltype(q)>;
        if constexpr(std::is_same_v<T,特征类新增请求>)return features_.新增特征(q);
        else if constexpr(std::is_same_v<T,存在类成员新增请求>)return existences_.新增成员关系(q);
        else if constexpr(std::is_same_v<T,特征长期观察积累请求>)return features_.积累特征观察(q);
        else return std::visit([&](const auto&w)->概念树共享写入结果{using W=std::decay_t<decltype(w)>;
            if constexpr(std::is_same_v<W,概念树共享建立请求>)return concepts_.创建共享特征概念(w);
            else if constexpr(std::is_same_v<W,概念树共享名称请求>)return concepts_.操作共享名称字段(w);
            else if constexpr(std::is_same_v<W,概念树共享使用请求>)return concepts_.记录共享概念使用(w);
            else if constexpr(std::is_same_v<W,概念树共享上位请求>)return concepts_.操作共享概念上位(w);
            else if constexpr(std::is_same_v<W,概念树共享生命周期请求>)return concepts_.迁移共享概念生命周期(w);
            else if constexpr(std::is_same_v<W,概念树共享退出请求>)return concepts_.退出共享特征概念(w);
            else return concepts_.释放共享概念引用(w);
        },q);
    },r);
}
void 特征概念应用服务::核验纯材料(const 共享特征继续材料&m,const 共享特征应用预算&b,std::uint64_t g)const{
    using namespace 共享特征应用内部;
    if(m.版本!=1||m.原业务.valueless_by_exception()||!m.原H||m.原H!=原截止(m.原业务)||!初始G(m.原业务)||初始G(m.原业务)>g||m.原H>初始G(m.原业务))拒绝();
    if(!b.最大派发步骤||!b.最大保留尝试||!b.最大遍历节点||保留尝试数(m)>b.最大保留尝试)拒绝(D::数量预算不足);
    std::set<std::uint64_t>keys;
    auto fixed=[&](std::uint64_t key,bool required){if(!key&&!required)return;if(!合法键(key)||!keys.insert(key).second)拒绝();};
    std::vector<共享步骤键> all;
    std::uint64_t business=0;
    std::visit([&](const auto&r){using T=std::decay_t<decltype(r)>;
        if(r.版本!=1)拒绝();
        if constexpr(std::is_same_v<T,共享特征生成请求>||std::is_same_v<T,共享特征关联请求>){
            business=r.业务标识;
            if(!business||!r.观察标识||r.时间<=0||!有效(r.E.值)||!有效(r.类型域身份.值)||!有效(r.跟踪身份.值))拒绝();
            if constexpr(std::is_same_v<T,共享特征生成请求>){if(!有效(r.FT.值)||r.实际拟建值.valueless_by_exception())拒绝();}
            else if(!有效(r.F.值)||!有效(r.值事实)||!有效(r.成员关系))拒绝();
            fixed(r.键组.F新建key,std::is_same_v<T,共享特征生成请求>);fixed(r.键组.E成员key,std::is_same_v<T,共享特征生成请求>);
            fixed(r.键组.A观察key,true);fixed(r.键组.TOPkey,true);fixed(r.键组.名称key,true);all=r.键组.阶段键;
            if(m.场景限制&&m.场景限制->E!=r.E)拒绝();
        }else if constexpr(std::is_same_v<T,共享特征治理请求>){
            if(!有效(r.目标.值)||!有效(r.当前生命周期值事实)||r.预期父组.empty())拒绝();all=r.阶段键;
            if(m.场景限制)拒绝();
        }else{if(!有效(r.旧概念.值)||!有效(r.类型域身份.值))拒绝();fixed(r.新建key,true);if(m.场景限制)拒绝();}
    },m.原业务);
    all.insert(all.end(),m.补充键.begin(),m.补充键.end());键表有效(all,keys);
    std::set<std::uint64_t>active;
    auto checkStep=[&](const 共享步骤&s,bool archived){
        if(s.原请求.valueless_by_exception()||s.目标.valueless_by_exception()||static_cast<unsigned>(s.owner)!=s.原请求.index()+1)拒绝();
        const auto [G,key]=步骤头(s.原请求);
        if(G<初始G(m.原业务)||G>g||!keys.contains(key)||(!archived&&!active.insert(key).second)||
            (s.首次H&&(*s.首次H<=G||*s.首次H>g)))拒绝();
        std::visit([&](const auto&q){using T=std::decay_t<decltype(q)>;
            if constexpr(std::is_same_v<T,特征类新增请求>){
                const auto*origin=std::get_if<共享特征生成请求>(&m.原业务);
                if(!origin||s.阶段角色!=共享阶段角色::新建F||key!=origin->键组.F新建key||q.特征类型!=origin->FT.值||q.特征值!=origin->实际拟建值)拒绝();
            }else if constexpr(std::is_same_v<T,存在类成员新增请求>){
                const auto*origin=std::get_if<共享特征生成请求>(&m.原业务);
                if(!origin||s.阶段角色!=共享阶段角色::加入E||key!=origin->键组.E成员key||q.存在结点!=origin->E.值||q.成员种类!=存在类成员种类::特征)拒绝();
            }else if constexpr(std::is_same_v<T,特征长期观察积累请求>){
                if(s.阶段角色!=共享阶段角色::积累观察)拒绝();
                std::visit([&](const auto&origin){using O=std::decay_t<decltype(origin)>;
                    if constexpr(std::is_same_v<O,共享特征生成请求>||std::is_same_v<O,共享特征关联请求>){
                        if(key!=origin.键组.A观察key||q.跟踪!=origin.跟踪身份||q.观察.存在上下文!=origin.E.值||q.观察.观察标识!=origin.观察标识||q.观察.时间纳秒!=origin.时间)拒绝();
                    }else 拒绝();
                },m.原业务);
            }else std::visit([&](const auto&w){using W=std::decay_t<decltype(w)>;
                if constexpr(std::is_same_v<W,概念树共享使用请求>){
                    if(!business||w.业务标识!=business||
                        (s.阶段角色!=共享阶段角色::实际适用&&s.阶段角色!=共享阶段角色::名称采用)||
                        w.用途角色!=(s.阶段角色==共享阶段角色::实际适用 ? 1 : 2))拒绝();
                    const auto*target=std::get_if<概念树概念身份>(&s.目标);
                    if(!target||*target!=w.概念||!m.已定位观察||w.观察!=m.已定位观察->身份||w.跟踪!=m.已定位观察->跟踪||
                        w.原H!=m.已定位观察->见证.H||w.时间!=m.已定位观察->见证.时间纳秒)拒绝();
                    std::visit([&](const auto&origin){using O=std::decay_t<decltype(origin)>;
                        if constexpr(std::is_same_v<O,共享特征生成请求>||std::is_same_v<O,共享特征关联请求>){
                            const auto&actual=m.已定位观察->见证;
                            if(w.跟踪!=origin.跟踪身份||w.时间!=origin.时间||actual.观察标识!=origin.观察标识||actual.存在上下文!=origin.E.值)拒绝();
                            if constexpr(std::is_same_v<O,共享特征生成请求>){
                                if(!m.生成观察H||w.原H!=*m.生成观察H||!m.已定位F||!m.已定位成员||actual.F!=m.已定位F->结点||
                                    actual.FT!=origin.FT.值||actual.值事实!=m.已定位F->值事实||actual.成员关系!=m.已定位成员->成员关系)拒绝();
                            }else if(w.原H!=origin.H||actual.F!=origin.F.值||actual.值事实!=origin.值事实||actual.成员关系!=origin.成员关系)拒绝();
                        }else 拒绝();
                    },m.原业务);
                    if(std::count_if(m.步骤.begin(),m.步骤.end(),[](const auto&step){return step.阶段角色==共享阶段角色::积累观察;})!=1)拒绝();
                }
            },q);
        },s.原请求);
    };
    for(const auto&s:m.归档)checkStep(s,true);
    for(const auto&s:m.步骤)checkStep(s,false);
    if(m.缺步骤恢复){
        if(!std::holds_alternative<共享特征生成请求>(m.原业务)&&!std::holds_alternative<共享特征关联请求>(m.原业务))拒绝();
        const auto&p=*m.缺步骤恢复;const auto&locate=p.定位尝试;
        checkStep(locate,true);核验历史步骤目标(locate);
        const auto key=步骤头(locate.原请求).second;
        if(locate.首次H||!locate.回执提示||p.键历史H<=1||p.键历史H-1<初始G(m.原业务)||p.键历史H-1>=g||
           active.contains(key)||std::any_of(m.步骤.begin(),m.步骤.end(),[&](const auto&s){return s.阶段角色==locate.阶段角色&&s.目标==locate.目标;}))拒绝();
        const auto*r=std::get_if<概念树共享写入结果>(&*locate.回执提示);
        if(!r||r->状态!=D::幂等冲突||!r->原请求||!r->首次H||*r->首次H!=p.键历史H||!r->Gread||p.键历史H>r->Gread||r->Gread>g||
           !步骤等义(locate.原请求,共享步骤原请求{*r->原请求}))拒绝();
        std::optional<std::uint64_t> bound;
        if(locate.阶段角色==共享阶段角色::完整域||locate.阶段角色==共享阶段角色::名称)
            std::visit([&](const auto&o){using O=std::decay_t<decltype(o)>;if constexpr(std::is_same_v<O,共享特征生成请求>||std::is_same_v<O,共享特征关联请求>)bound=locate.阶段角色==共享阶段角色::完整域 ? o.键组.TOPkey : o.键组.名称key;},m.原业务);
        else for(const auto&k:all)if(k.阶段角色==locate.阶段角色&&k.目标==locate.目标)bound=k.key;
        if(!bound||*bound!=key)拒绝();
        if(p.历史候选){
            const auto&candidate=*p.历史候选;checkStep(candidate,true);核验历史步骤目标(candidate);
            if(candidate.首次H||candidate.阶段角色!=locate.阶段角色||candidate.owner!=locate.owner||candidate.目标!=locate.目标||
               步骤头(candidate.原请求)!=std::pair<std::uint64_t,std::uint64_t>{p.键历史H-1,key})拒绝();
            auto comparable=candidate.原请求;改写准备G(comparable,步骤头(locate.原请求).first);
            auto&cq=std::get<概念树共享写请求>(comparable);const auto&lq=std::get<概念树共享写请求>(locate.原请求);
            if(cq.index()!=lq.index())拒绝();
            if(auto*n=std::get_if<概念树共享名称请求>(&cq)){const auto&old=std::get<概念树共享名称请求>(lq);n->预期=old.预期;if(!n->见证||!old.见证)拒绝();n->见证->H=old.见证->H;}
            if(auto*n=std::get_if<概念树共享上位请求>(&cq))n->预期父组=std::get<概念树共享上位请求>(lq).预期父组;
            if(!步骤等义(comparable,locate.原请求))拒绝();
            if(candidate.回执提示){const auto*receipt=std::get_if<概念树共享写入结果>(&*candidate.回执提示);
                if(!receipt||!receipt->原请求||!步骤等义(candidate.原请求,共享步骤原请求{*receipt->原请求}))拒绝();}
        }
    }
}

std::optional<std::size_t> 特征概念应用服务::运行会话::找步骤(共享阶段角色 role,const 共享步骤目标&target)const{
    std::optional<std::size_t> index;
    for(std::size_t i=0;i<m.步骤.size();++i)if(m.步骤[i].阶段角色==role&&m.步骤[i].目标==target){
        if(index)共享特征应用内部::拒绝();index=i;
    }
    return index;
}
std::uint64_t 特征概念应用服务::运行会话::准备G(共享阶段角色 role,const 共享步骤目标&target)const{
    if(const auto index=找步骤(role,target))return 共享特征应用内部::步骤头(m.步骤[*index].原请求).first;
    if(m.缺步骤恢复&&m.缺步骤恢复->定位尝试.阶段角色==role&&m.缺步骤恢复->定位尝试.目标==target)
        return m.缺步骤恢复->键历史H-1;
    return g;
}
std::uint64_t 特征概念应用服务::运行会话::取键(共享阶段角色 role,const 共享步骤目标&target,std::uint64_t fixed){
    using namespace 共享特征应用内部;
    if(fixed)return fixed;
    std::optional<std::uint64_t> key;
    auto scan=[&](const std::vector<共享步骤键>&group){for(const auto&k:group)if(k.阶段角色==role&&k.目标==target){if(key)拒绝();key=k.key;}};
    std::visit([&](const auto&r){using T=std::decay_t<decltype(r)>;
        if constexpr(std::is_same_v<T,共享特征生成请求>||std::is_same_v<T,共享特征关联请求>)scan(r.键组.阶段键);
        else if constexpr(std::is_same_v<T,共享特征治理请求>)scan(r.阶段键);
    },m.原业务);
    scan(m.补充键);
    if(key)return *key;
    m.待补键目标组={{role,target}};
    业务拒绝(共享特征业务状态::待继续);
}
void 特征概念应用服务::运行会话::场景检查(){
    std::visit([&](const auto&r){using T=std::decay_t<decltype(r)>;
        if constexpr(std::is_same_v<T,共享特征生成请求>||std::is_same_v<T,共享特征关联请求>)
            共享特征应用内部::场景前置(app.concepts_,app.existences_,scene,m.场景限制,r.E,g,b.数据);
    },m.原业务);
}
void 特征概念应用服务::运行会话::当前实例检查(){
    using namespace 共享特征应用内部;
    if(!m.已定位F||!m.已定位成员)拒绝(D::内部不一致);
    概念树存在引用 E;
    std::visit([&](const auto&r){if constexpr(requires{r.E;})E=r.E;else 拒绝();},m.原业务);
    auto e=app.existences_.读取存在历史事实({1,g,g,E.值,b.数据.基础.最大世界成员数});
    if(!e.成功())拒绝(映射(e.状态));
    if(e.Gread!=g||e.H!=g||e.存在->结点!=E.值)拒绝(D::内部不一致);
    if(std::count(e.存在->特征组.begin(),e.存在->特征组.end(),*m.已定位成员)!=1)业务拒绝(共享特征业务状态::被后继覆盖);
    auto f=app.features_.按实例读取特征历史事实({1,g,g,m.已定位F->结点,b.数据.基础.最大特征属性数});
    if(!f.成功())拒绝(映射(f.状态));
    if(f.Gread!=g||f.H!=g)拒绝(D::内部不一致);
    if(f.特征->结点!=m.已定位F->结点||f.特征->值事实!=m.已定位F->值事实||f.特征->特征类型!=m.已定位F->特征类型)
        业务拒绝(共享特征业务状态::被后继覆盖);
    场景检查();尾守卫(app.concepts_,g);
}
const 共享步骤回执& 特征概念应用服务::运行会话::做步骤(共享阶段角色 role,const 共享步骤目标&target,共享步骤原请求 request){
    using namespace 共享特征应用内部;
    auto found=找步骤(role,target);
    const bool existing=found.has_value();
    if(m.缺步骤恢复&&!found){
        if(m.缺步骤恢复->定位尝试.阶段角色!=role||m.缺步骤恢复->定位尝试.目标!=target)业务拒绝(共享特征业务状态::待继续);
        return 恢复缺步骤(role,target,std::move(request));
    }
    if(found&&!步骤等义(m.步骤[*found].原请求,request))拒绝();
    if(m.缺步骤恢复&&found)当前实例检查();
    if(调用数>=b.最大派发步骤)业务拒绝(共享特征业务状态::待继续);
    if(!found){
        if(保留尝试数(m)>=b.最大保留尝试)拒绝(D::数量预算不足);
        found=m.步骤.size();m.步骤.push_back({role,static_cast<共享步骤所有者>(request.index()+1),target,std::move(request),std::nullopt,std::nullopt});
    }
    场景检查();尾守卫(app.concepts_,g);
    auto &saved=m.步骤[*found];
    未核验=true;++调用数;
    auto result=app.派发(调整预算(saved.原请求,b));
    saved.回执提示=std::move(result);
    const auto success=std::visit([](const auto&r){return r.成功();},*saved.回执提示);
    if(!existing&&!success&&历史可恢复(saved)&&
       (std::holds_alternative<共享特征生成请求>(m.原业务)||std::holds_alternative<共享特征关联请求>(m.原业务))){
        const auto*locate=std::get_if<概念树共享写入结果>(&*saved.回执提示);
        if(locate&&locate->状态==D::幂等冲突&&locate->首次H){
            const auto h=*locate->首次H;
            if(!locate->原请求||!locate->Gread||h<=1||h>locate->Gread||h-1<初始G(m.原业务)||h-1>=g||
               !步骤等义(saved.原请求,共享步骤原请求{*locate->原请求}))拒绝(D::内部不一致);
            核验历史步骤目标(saved);
            static_assert(std::is_nothrow_move_constructible_v<共享步骤>);
            static_assert(std::is_nothrow_move_assignable_v<std::optional<共享缺步骤恢复材料>>);
            共享缺步骤恢复材料 pending{std::move(saved),h,std::nullopt};pending.定位尝试.首次H.reset();
            m.缺步骤恢复=std::move(pending);m.步骤.pop_back();
            未核验=true;业务拒绝(共享特征业务状态::待继续);
        }
    }

    std::visit([&](const auto&r){using T=std::decay_t<decltype(r)>;
        if constexpr(!std::is_same_v<T,特征类结点结果>&&!std::is_same_v<T,存在类结点结果>){if(r.首次H)saved.首次H=r.首次H;}
    },*saved.回执提示);
    if(!success){
        bool unbound=false;共享特征业务状态 failure=共享特征业务状态::内部不一致;
        std::visit([&](const auto&r){using T=std::decay_t<decltype(r)>;
            if constexpr(std::is_same_v<T,特征类结点结果>){
                unbound=r.状态==特征类数据状态::事实代次漂移&&!saved.首次H;
                if(r.状态==特征类数据状态::资源失败)failure=共享特征业务状态::资源失败;
                else if(r.状态==特征类数据状态::入口拒绝||r.状态==特征类数据状态::幂等冲突)failure=共享特征业务状态::入口拒绝;
                if(r.事实代次>步骤头(saved.原请求).first&&r.状态==特征类数据状态::已可能发布)saved.首次H=r.事实代次;
            }else if constexpr(std::is_same_v<T,存在类结点结果>){
                unbound=r.状态==存在类数据状态::事实代次漂移&&!saved.首次H;
                if(r.状态==存在类数据状态::资源失败)failure=共享特征业务状态::资源失败;
                else if(r.状态==存在类数据状态::入口拒绝||r.状态==存在类数据状态::幂等冲突)failure=共享特征业务状态::入口拒绝;
                if(r.事实代次>步骤头(saved.原请求).first&&r.状态==存在类数据状态::已可能发布)saved.首次H=r.事实代次;
            }else if constexpr(std::is_same_v<T,特征长期写入结果<特征长期观测事实>>){
                unbound=r.状态==特征长期状态::事实代次漂移&&!r.首次H&&(r.发布确定性==特征长期发布确定性::未派发||r.发布确定性==特征长期发布确定性::已确认未发布);
                failure=业务映射(映射(r.状态));
            }else{
                unbound=r.状态==D::事实代次漂移&&!r.首次H&&r.发布状态==概念树发布状态::未派发;
                failure=业务映射(r.状态);
            }
        },*saved.回执提示);
        if(unbound){
            if(保留尝试数(m)>=b.最大保留尝试)拒绝(D::数量预算不足);
            const auto fresh=当前(app.concepts_);g=fresh;
            auto prepared=saved.原请求;改写准备G(prepared,fresh);
            if(auto*conceptRequest=std::get_if<概念树共享写请求>(&prepared))std::visit([&](auto&w){
                using W=std::decay_t<decltype(w)>;
                if constexpr(std::is_same_v<W,概念树共享名称请求>){
                    当前实例检查();if(!w.见证)拒绝();w.见证->H=fresh;
                }else if constexpr(std::is_same_v<W,概念树共享生命周期请求>){
                    if(w.重新采用见证){
                        auto&v=*w.重新采用见证;
                        auto e=app.existences_.读取存在历史事实({1,fresh,fresh,v.E,b.数据.基础.最大世界成员数});
                        if(!e.成功())拒绝(映射(e.状态));
                        if(e.Gread!=fresh||e.H!=fresh||std::count_if(e.存在->特征组.begin(),e.存在->特征组.end(),[&](const auto&member){return member.成员关系==v.成员关系&&member.目标结点==v.F;})!=1)拒绝(D::引用冲突);
                        auto f=app.features_.按实例读取特征历史事实({1,fresh,fresh,v.F,b.数据.基础.最大特征属性数});
                        if(!f.成功())拒绝(映射(f.状态));
                        if(f.Gread!=fresh||f.H!=fresh||f.特征->值事实!=v.值事实)拒绝(D::引用冲突);
                        auto match=app.concepts_.判定应用特征模板({1,{1,fresh,fresh},w.操作.概念,{v.F,v.值事实,f.特征->特征类型,fresh},b.数据});
                        if(!match.成功())拒绝(match.状态);
                        if(match.Gread!=fresh||match.H!=fresh||!match.数据->适用)拒绝(D::引用冲突);
                        v.H=fresh;
                    }
                }
            },*conceptRequest);
            尾守卫(app.concepts_,fresh);
            m.归档.push_back(saved);saved.原请求=std::move(prepared);saved.首次H.reset();saved.回执提示.reset();
            业务拒绝(共享特征业务状态::待继续);
        }
        业务拒绝(failure);
    }
    return 确认步骤(*found);
}
const 共享步骤回执& 特征概念应用服务::运行会话::恢复缺步骤(共享阶段角色 role,const 共享步骤目标&target,共享步骤原请求 request){
    using namespace 共享特征应用内部;
    if(!m.缺步骤恢复)拒绝(D::内部不一致);
    auto&p=*m.缺步骤恢复;const auto&locate=p.定位尝试;
    if(locate.阶段角色!=role||locate.目标!=target||步骤头(request)!=std::pair<std::uint64_t,std::uint64_t>{p.键历史H-1,步骤头(locate.原请求).second}||p.键历史H-1>=g)拒绝();
    当前实例检查();
    if(p.历史候选){if(!步骤等义(p.历史候选->原请求,request))拒绝();}
    else{
        if(保留尝试数(m)>=b.最大保留尝试)拒绝(D::数量预算不足);
        共享步骤 candidate{role,共享步骤所有者::概念,target,std::move(request),std::nullopt,std::nullopt};
        核验历史步骤目标(candidate);p.历史候选=std::move(candidate);
    }
    app.核验纯材料(m,b,g);
    if(已派发历史候选||调用数>=b.最大派发步骤)业务拒绝(共享特征业务状态::待继续);
    场景检查();尾守卫(app.concepts_,g);未核验=true;已派发历史候选=true;++调用数;
    auto&candidate=*p.历史候选;candidate.回执提示=app.派发(调整预算(candidate.原请求,b));
    const auto*receipt=std::get_if<概念树共享写入结果>(&*candidate.回执提示);
    if(!receipt||!receipt->原请求||!步骤等义(candidate.原请求,共享步骤原请求{*receipt->原请求}))拒绝(D::内部不一致);
    if(receipt->状态!=D::精确重复||!receipt->成功())业务拒绝(业务映射(receipt->状态));
    if(!receipt->首次H||*receipt->首次H!=p.键历史H||receipt->Gread!=g)拒绝(D::内部不一致);
    // 所有分配先在候选副本完成；失败时原材料仍保存定位和完整历史尝试。
    auto next=m;auto&pending=*next.缺步骤恢复;
    next.归档.push_back(std::move(pending.定位尝试));
    pending.历史候选->首次H=pending.键历史H;
    const auto index=next.步骤.size();next.步骤.push_back(std::move(*pending.历史候选));next.缺步骤恢复.reset();
    app.核验纯材料(next,b,g);m=std::move(next);
    return 确认步骤(index);
}
const 共享步骤回执& 特征概念应用服务::运行会话::确认步骤(std::size_t index){
    using namespace 共享特征应用内部;
    auto&saved=m.步骤[index];
    std::visit([&](const auto&r){using T=std::decay_t<decltype(r)>;
        if constexpr(std::is_same_v<T,特征类结点结果>){saved.首次H=r.事实代次;out.已确认事实组.push_back(*r.特征);}
        else if constexpr(std::is_same_v<T,存在类结点结果>){
            saved.首次H=r.事实代次;const auto&raw=std::get<存在类成员新增请求>(saved.原请求);
            const auto it=std::find_if(r.存在结点->特征组.begin(),r.存在结点->特征组.end(),[&](const auto&m){return m.目标结点==raw.目标结点;});
            if(it==r.存在结点->特征组.end()||r.存在结点->结点!=raw.存在结点)拒绝(D::内部不一致);
            out.已确认事实组.push_back(*it);
        }else if constexpr(std::is_same_v<T,特征长期写入结果<特征长期观测事实>>)out.已确认事实组.push_back(*r.数据);
        else{
            if(r.概念)out.已确认事实组.push_back(*r.概念);
            if(r.名称)out.已确认事实组.push_back(*r.名称);
            if(r.用途)out.已确认事实组.push_back(*r.用途);
        }
    },*saved.回执提示);
    已核验.insert(index);未核验=m.缺步骤恢复.has_value()||已核验.size()!=m.步骤.size();out.阶段=共享特征阶段状态::部分确认;
    g=当前(app.concepts_);out.Gread=g;
    return *saved.回执提示;
}

void 特征概念应用服务::推进实例(运行会话&s){
    using namespace 共享特征应用内部;
    auto&m=s.m;auto&b=s.b;const auto*gen=std::get_if<共享特征生成请求>(&m.原业务);
    const auto*assoc=std::get_if<共享特征关联请求>(&m.原业务);
    if(!gen&&!assoc)拒绝();
    const auto E=gen?gen->E:assoc->E;
    const auto domainId=gen?gen->类型域身份:assoc->类型域身份;
    const auto trackId=gen?gen->跟踪身份:assoc->跟踪身份;
    const auto &keys=gen?gen->键组:assoc->键组;
    const auto obsId=gen?gen->观察标识:assoc->观察标识;
    const auto time=gen?gen->时间:assoc->时间;
    const auto business=gen?gen->业务标识:assoc->业务标识;
    s.场景检查();
    auto type=features_.读取特征类型域({{1,s.g,m.原H},domainId,b.数据.特征});
    if(!type.成功())拒绝(映射(type.状态));
    if(type.Gread!=s.g||type.H!=m.原H||type.数据->身份!=domainId)拒绝(D::内部不一致);
    const auto FT=type.数据->内容.FT;
    auto track=features_.读取特征跟踪({{1,s.g,m.原H},trackId,b.数据.特征});
    if(!track.成功())拒绝(映射(track.状态));
    if(track.Gread!=s.g||track.H!=m.原H||track.数据->size()!=1||track.数据->front().跟踪事实.身份!=trackId||
        track.数据->front().跟踪事实.内容.存在上下文.值!=E.值||track.数据->front().跟踪事实.内容.类型域!=domainId||
        track.数据->front().类型域事实.内容!=type.数据->内容)拒绝(D::引用冲突);
    const 概念树共享特征定义 topDefinition{概念树特征类型引用{FT},type.数据->内容,true,{}};
    auto readE=[&](std::uint64_t h){auto e=existences_.读取存在历史事实({1,s.g,h,E.值,b.数据.基础.最大世界成员数});
        if(!e.成功())拒绝(映射(e.状态));if(e.Gread!=s.g||e.H!=h||e.存在->结点!=E.值)拒绝(D::内部不一致);return std::move(*e.存在);};
    auto requireAbsent=[&](std::uint64_t h){auto e=readE(h);for(const auto&member:e.特征组){
        auto f=features_.按实例读取特征历史事实({1,s.g,h,member.目标结点,b.数据.基础.最大特征属性数});
        if(!f.成功())拒绝(映射(f.状态));if(f.Gread!=s.g||f.H!=h)拒绝(D::内部不一致);
        if(f.特征->特征类型==FT)拒绝(D::引用冲突);
    }};
    if(gen){
        if(gen->FT.值!=FT)拒绝();
        requireAbsent(m.原H);
        auto fg=s.准备G(共享阶段角色::新建F,topDefinition);requireAbsent(fg);
        if(!s.找步骤(共享阶段角色::新建F,topDefinition)){
            auto prospective=features_.预检长期特征拟建值({1,{1,s.g,s.g},domainId,FT,gen->实际拟建值,b.数据.特征});
            if(!prospective.成功())拒绝(映射(prospective.状态));
            if(prospective.Gread!=s.g||prospective.H!=s.g)拒绝(D::内部不一致);
        }
        特征类新增请求 fr{1,fg,L1所有者范围写入幂等身份{keys.F新建key},FT,gen->实际拟建值};
        const auto&fresult=std::get<特征类结点结果>(s.做步骤(共享阶段角色::新建F,topDefinition,fr));
        if(fresult.特征->特征类型!=FT)拒绝(D::内部不一致);
        m.已定位F=*fresult.特征;
        const auto eg=s.准备G(共享阶段角色::加入E,topDefinition);requireAbsent(eg);
        auto originalF=features_.按实例读取特征历史事实({1,s.g,eg,m.已定位F->结点,b.数据.基础.最大特征属性数});
        if(!originalF.成功())拒绝(映射(originalF.状态));
        if(originalF.Gread!=s.g||originalF.H!=eg||originalF.特征->值事实!=m.已定位F->值事实||originalF.特征->特征类型!=FT)拒绝(D::引用冲突);
        存在类成员新增请求 er{1,eg,L1所有者范围写入幂等身份{keys.E成员key},E.值,存在类成员种类::特征,m.已定位F->结点};
        const auto&eresult=std::get<存在类结点结果>(s.做步骤(共享阶段角色::加入E,topDefinition,er));
        const auto member=std::find_if(eresult.存在结点->特征组.begin(),eresult.存在结点->特征组.end(),[&](const auto&e){return e.目标结点==m.已定位F->结点;});
        if(member==eresult.存在结点->特征组.end())拒绝(D::内部不一致);
        m.已定位成员=*member;m.生成观察H=eresult.事实代次;
    }else{
        const auto e=readE(m.原H);
        const auto member=std::find_if(e.特征组.begin(),e.特征组.end(),[&](const auto&edge){return edge.目标结点==assoc->F.值&&edge.成员关系==assoc->成员关系;});
        if(member==e.特征组.end())拒绝(D::引用冲突);
        auto value=features_.按实例读取特征历史事实({1,s.g,m.原H,assoc->F.值,b.数据.基础.最大特征属性数});
        if(!value.成功())拒绝(映射(value.状态));
        if(value.Gread!=s.g||value.H!=m.原H||value.特征->值事实!=assoc->值事实||value.特征->特征类型!=FT)拒绝(D::引用冲突);
        m.已定位F=std::move(*value.特征);m.已定位成员=*member;
    }
    const auto observedH=gen?*m.生成观察H:m.原H;
    const 特征观察见证 witness{obsId,time,observedH,E.值,m.已定位F->结点,FT,m.已定位F->值事实,m.已定位成员->成员关系};
    const auto ag=s.准备G(共享阶段角色::积累观察,topDefinition);
    // 已准备的 A 请求先恢复其历史发布；只在首次准备时要求实例当前仍可用。
    if(!s.找步骤(共享阶段角色::积累观察,topDefinition))s.当前实例检查();
    特征长期观察积累请求 ar{{1,ag,L1所有者范围写入幂等身份{keys.A观察key}},trackId,witness,b.数据.特征};
    const auto&observation=std::get<特征长期写入结果<特征长期观测事实>>(s.做步骤(共享阶段角色::积累观察,topDefinition,ar));
    if(observation.数据->见证!=witness||observation.数据->跟踪!=trackId)拒绝(D::内部不一致);
    m.已定位观察=*observation.数据;
    auto create=[&](共享阶段角色 role,const 概念树共享特征定义&definition,概念树共享来源选择 source,std::vector<概念树概念身份>parents,std::uint64_t fixed=0){
        const auto pg=s.准备G(role,definition);
        const auto key=s.取键(role,definition,fixed);
        if(!s.找步骤(role,definition))s.当前实例检查();
        概念树共享建立请求 request{1,{1,pg,key},definition,std::move(source),std::move(parents),b.数据};
        const auto&result=std::get<概念树共享写入结果>(s.做步骤(role,definition,概念树共享写请求{request}));
        if(!result.概念||result.概念->共享定义!=definition)拒绝(D::内部不一致);
        if(result.概念->治理状态==概念树生命周期状态::退役)业务拒绝(共享特征业务状态::需显式治理);
        return *result.概念;
    };
    auto top=create(共享阶段角色::完整域,topDefinition,概念树类型完整域依据{domainId},{root_},keys.TOPkey);
    auto intervals=features_.读取长期区间({{1,s.g,m.已定位观察->创建G},trackId,b.数据.特征});
    if(!intervals.成功())拒绝(映射(intervals.状态));
    if(intervals.Gread!=s.g||intervals.H!=m.已定位观察->创建G)拒绝(D::内部不一致);
    std::vector<特征区间身份> ids;
    for(const auto&hit:m.已定位观察->全部命中)ids.push_back(hit.区间);
    if(m.已定位观察->本次新建&&std::find(ids.begin(),ids.end(),*m.已定位观察->本次新建)==ids.end())ids.push_back(*m.已定位观察->本次新建);
    std::sort(ids.begin(),ids.end(),[](auto a,auto b){return a.值.值<b.值.值;});
    if(ids.empty()||std::adjacent_find(ids.begin(),ids.end())!=ids.end())拒绝(D::内部不一致);
    for(auto id:ids){
        const auto it=std::find_if(intervals.数据->begin(),intervals.数据->end(),[&](const auto&i){return i.身份==id;});
        if(it==intervals.数据->end())拒绝(D::内部不一致);
        概念树共享特征定义 definition{概念树特征类型引用{FT},type.数据->内容,false,{it->原子域}};
        if(definition.类型.算法==特征长期算法::I64幅度&&std::get<特征I64闭区间>(it->原子域.域)==std::get<特征I64闭区间>(definition.类型.完整域)){definition.完整域=true;definition.原子组.clear();}
        create(共享阶段角色::区间域,definition,概念树区间依据{{id},{trackId}},definition.完整域 ? std::vector<概念树概念身份>{root_} : std::vector<概念树概念身份>{top.概念});
    }
    推进域与名称(s,topDefinition,E,domainId,witness,business,keys.名称key);
}

void 特征概念应用服务::推进域与名称(运行会话&s,const 概念树共享特征定义&topDefinition,概念树存在引用 E,
    特征类型域身份 domainId,const 特征观察见证&witness,std::uint64_t business,std::uint64_t nameKey){
    using namespace 共享特征应用内部;
    auto&m=s.m;const auto&b=s.b;
    if(!m.冻结G)m.冻结G=s.g;
    if(m.冻结G<m.原H||m.冻结G>s.g)拒绝();
    auto frozen=concepts_.读取共享特征域组({1,{1,s.g,m.冻结G},topDefinition.FT,b.数据});
    if(!frozen.成功())拒绝(frozen.状态);
    if(frozen.Gread!=s.g||frozen.H!=m.冻结G||frozen.数据->empty()||frozen.数据->size()>b.最大遍历节点)拒绝(D::内部不一致);
    if(!m.冻结域集.empty()){
        if(m.冻结域集.size()!=frozen.数据->size())拒绝();
        for(std::size_t i=0;i<m.冻结域集.size();++i)
            if(m.冻结域集[i].概念!=(*frozen.数据)[i].概念||m.冻结域集[i].共享定义!=(*frozen.数据)[i].共享定义)拒绝();
    }
    m.冻结域集=*frozen.数据;
    auto match=匹配({1,s.g,m.冻结G,E,概念树特征引用{witness.F},witness.值事实,witness.成员关系,domainId,b},s.g);
    if(match.最具体适用组.empty()||!match.规范并集建议)拒绝(D::内部不一致);
    if(match.需显式治理)业务拒绝(共享特征业务状态::需显式治理);
    auto definitions=m.冻结域集;
    const auto finalDefinition=*match.规范并集建议;
    if(match.最具体适用组.size()==1)m.已定位概念=match.最具体适用组.front();
    else{
        std::vector<概念树概念身份>source,parents;
        for(const auto&d:match.最具体适用组)source.push_back(d.概念);
        for(const auto&candidate:definitions){
            if(candidate.共享定义==finalDefinition||!包含(candidate.共享定义,finalDefinition))continue;
            if(std::any_of(definitions.begin(),definitions.end(),[&](const auto&other){return other.概念!=candidate.概念&&other.共享定义!=finalDefinition&&
                other.共享定义!=candidate.共享定义&&包含(candidate.共享定义,other.共享定义)&&包含(other.共享定义,finalDefinition);}))continue;
            parents.push_back(candidate.概念);
        }
        if(finalDefinition.完整域)parents={root_};
        if(parents.empty())拒绝(D::内部不一致);
        const auto pg=s.准备G(共享阶段角色::共同上位,finalDefinition);
        const auto key=s.取键(共享阶段角色::共同上位,finalDefinition);
        if(!s.找步骤(共享阶段角色::共同上位,finalDefinition))s.当前实例检查();
        概念树共享建立请求 request{1,{1,pg,key},finalDefinition,概念树并集依据{source},parents,b.数据};
        const auto&result=std::get<概念树共享写入结果>(s.做步骤(共享阶段角色::共同上位,finalDefinition,概念树共享写请求{request}));
        if(!result.概念||result.概念->共享定义!=finalDefinition)拒绝(D::内部不一致);
        if(result.概念->治理状态==概念树生命周期状态::退役)业务拒绝(共享特征业务状态::需显式治理);
        m.已定位概念=*result.概念;
        if(std::none_of(definitions.begin(),definitions.end(),[&](const auto&d){return d.概念==result.概念->概念;}))definitions.push_back(*result.概念);
    }
    auto domainLess=[](const auto&a,const auto&b){
        if(a.共享定义.完整域!=b.共享定义.完整域)return a.共享定义.完整域;
        return std::lexicographical_compare(a.共享定义.原子组.begin(),a.共享定义.原子组.end(),b.共享定义.原子组.begin(),b.共享定义.原子组.end(),原子序);
    };
    std::sort(definitions.begin(),definitions.end(),domainLess);
    if(definitions.size()>b.最大遍历节点)拒绝(D::数量预算不足);
    auto idLess=[](auto a,auto b){return a.值.值<b.值.值;};
    // 有限定义集先冻结；每个节点的一整组父边由单个 owner 事务处理。
    for(std::size_t i=0;i<definitions.size();++i){
        const auto&d=definitions[i];std::vector<概念树概念身份> parents;
        if(d.共享定义.完整域)parents={root_};
        else for(const auto&p:definitions){
            if(p.概念==d.概念||p.共享定义==d.共享定义||!包含(p.共享定义,d.共享定义))continue;
            if(std::any_of(definitions.begin(),definitions.end(),[&](const auto&q){return q.概念!=d.概念&&q.概念!=p.概念&&q.共享定义!=p.共享定义&&q.共享定义!=d.共享定义&&
                包含(p.共享定义,q.共享定义)&&包含(q.共享定义,d.共享定义);}))continue;
            parents.push_back(p.概念);
        }
        std::sort(parents.begin(),parents.end(),idLess);if(parents.empty())拒绝(D::内部不一致);
        const auto old=s.找步骤(共享阶段角色::完整父组,d.概念);
        const auto pg=s.准备G(共享阶段角色::完整父组,d.概念);
        auto actual=concepts_.读取应用概念定义({1,{1,s.g,pg},d.概念,b.数据});
        if(!actual.成功())拒绝(actual.状态);
        const auto*fact=std::get_if<概念树共享定义事实>(&*actual.数据);
        if(actual.Gread!=s.g||actual.H!=pg||!fact||fact->共享定义!=d.共享定义)拒绝(D::内部不一致);
        std::vector<概念树概念身份>existing;for(const auto&e:fact->直接上位组)existing.push_back(e.上位);
        std::sort(existing.begin(),existing.end(),idLess);
        const auto targetsParent=[&](const auto&key){return key.阶段角色==共享阶段角色::完整父组&&key.目标==共享步骤目标{d.概念};};
        bool acceptedKey=std::any_of(m.补充键.begin(),m.补充键.end(),targetsParent);
        std::visit([&](const auto&origin){using O=std::decay_t<decltype(origin)>;if constexpr(std::is_same_v<O,共享特征生成请求>||std::is_same_v<O,共享特征关联请求>)
            acceptedKey=acceptedKey||std::any_of(origin.键组.阶段键.begin(),origin.键组.阶段键.end(),targetsParent);},m.原业务);
        if(old||acceptedKey||existing!=parents||(m.缺步骤恢复&&m.缺步骤恢复->定位尝试.阶段角色==共享阶段角色::完整父组&&m.缺步骤恢复->定位尝试.目标==共享步骤目标{d.概念})){
            const auto key=s.取键(共享阶段角色::完整父组,d.概念);
            if(!old)s.当前实例检查();
            概念树共享上位请求 request{1,{1,pg,key},d.概念,fact->直接上位组,parents,b.数据};
            // 原 H 的未来退出在 provider 原请求等义中投影；保存的原预期本身保持不变。
            if(old)request.预期父组=std::get<概念树共享上位请求>(std::get<概念树共享写请求>(m.步骤[*old].原请求)).预期父组;
            else if(m.缺步骤恢复&&m.缺步骤恢复->历史候选&&m.缺步骤恢复->定位尝试.阶段角色==共享阶段角色::完整父组&&m.缺步骤恢复->定位尝试.目标==共享步骤目标{d.概念})
                request.预期父组=std::get<概念树共享上位请求>(std::get<概念树共享写请求>(m.缺步骤恢复->历史候选->原请求)).预期父组;
            const auto&result=std::get<概念树共享写入结果>(s.做步骤(共享阶段角色::完整父组,d.概念,概念树共享写请求{request}));
            if(!result.概念||result.概念->概念!=d.概念)拒绝(D::内部不一致);
        }
        m.遍历位置=i+1;
    }
    const auto target=m.已定位概念->概念;
    const auto ng=s.准备G(共享阶段角色::名称,target);
    auto names=concepts_.读取共享名称字段({1,{1,s.g,ng},概念树特征引用{witness.F},false,b.数据});
    if(!names.成功())拒绝(names.状态);
    if(names.Gread!=s.g||names.H!=ng||names.数据->size()>1)拒绝(D::内部不一致);
    std::optional<概念树共享名称事实>expected;if(!names.数据->empty())expected=names.数据->front();
    const auto oldName=s.找步骤(共享阶段角色::名称,target);
    if(!oldName)s.当前实例检查();
    if(m.缺步骤恢复&&m.缺步骤恢复->定位尝试.阶段角色==共享阶段角色::名称){
        auto e=existences_.读取存在历史事实({1,s.g,ng,E.值,b.数据.基础.最大世界成员数});
        if(!e.成功())拒绝(映射(e.状态));
        if(e.Gread!=s.g||e.H!=ng||std::count_if(e.存在->特征组.begin(),e.存在->特征组.end(),[&](const auto&x){return x.成员关系==witness.成员关系&&x.目标结点==witness.F;})!=1)拒绝(D::引用冲突);
        auto f=features_.按实例读取特征历史事实({1,s.g,ng,witness.F,b.数据.基础.最大特征属性数});
        if(!f.成功())拒绝(映射(f.状态));
        if(f.Gread!=s.g||f.H!=ng||f.特征->值事实!=witness.值事实||f.特征->特征类型!=topDefinition.FT.值)拒绝(D::引用冲突);
    }
    概念树共享实例见证 nameWitness{E.值,witness.F,witness.值事实,witness.成员关系,ng,witness.观察标识,witness.时间纳秒};
    概念树共享名称请求 name{1,{1,ng,nameKey},概念树特征引用{witness.F},expected,target,nameWitness,b.数据};
    if(oldName)name.预期=std::get<概念树共享名称请求>(std::get<概念树共享写请求>(m.步骤[*oldName].原请求)).预期;
    else if(m.缺步骤恢复&&m.缺步骤恢复->历史候选&&m.缺步骤恢复->定位尝试.阶段角色==共享阶段角色::名称)
        name.预期=std::get<概念树共享名称请求>(std::get<概念树共享写请求>(m.缺步骤恢复->历史候选->原请求)).预期;
    const auto&named=std::get<概念树共享写入结果>(s.做步骤(共享阶段角色::名称,target,概念树共享写请求{name}));
    if(!named.名称||named.名称->概念!=target||named.名称->F.值!=witness.F)拒绝(D::内部不一致);
    m.已定位名称=*named.名称;
    auto use=[&](概念树概念身份 id,std::uint32_t role){
        const auto stage=role==1 ? 共享阶段角色::实际适用 : 共享阶段角色::名称采用;
        const auto pg=s.准备G(stage,id),key=s.取键(stage,id);
        if(!s.找步骤(stage,id))s.当前实例检查();
        概念树共享使用请求 request{1,{1,pg,key},business,id,m.已定位观察->跟踪,m.已定位观察->身份,witness.H,witness.时间纳秒,role,b.数据};
        const auto&result=std::get<概念树共享写入结果>(s.做步骤(stage,id,概念树共享写请求{request}));
        if(!result.用途||result.用途->概念!=id||result.用途->跟踪!=request.跟踪||result.用途->观察!=request.观察||result.用途->原H!=request.原H||
            result.用途->时间!=request.时间||result.用途->用途角色!=role||
            (result.状态!=D::无须变更&&result.用途->业务标识!=business))拒绝(D::内部不一致);
    };
    std::set<std::uint64_t>used;
    for(const auto&decision:match.比较回执组)if(decision.适用){
        const auto*domain=std::get_if<概念树共享定义事实>(&decision.模板);
        if(!domain||!used.insert(domain->概念.值.值).second)拒绝(D::内部不一致);
        use(domain->概念,1);
    }
    use(target,2);
    s.当前实例检查();
    const auto fresh=s.g;
    auto instance=读取共享特征实例({1,fresh,fresh,E,概念树特征引用{witness.F},witness.值事实,witness.成员关系,domainId,b});
    if(!instance.成功())拒绝(instance.状态);
    if(instance.Gread!=fresh||instance.H!=fresh||!instance.数据->名称||instance.数据->名称->关系!=m.已定位名称->关系||instance.数据->名称->概念!=target||
        !instance.数据->名称概念||instance.数据->名称概念->共享定义!=finalDefinition)业务拒绝(共享特征业务状态::被后继覆盖);
    for(const auto&step:m.步骤)if(const auto*businessRequest=std::get_if<概念树共享写请求>(&step.原请求)){
        if(const auto*parent=std::get_if<概念树共享上位请求>(businessRequest)){
            auto current=concepts_.读取应用概念定义({1,{1,fresh,fresh},parent->下位,b.数据});
            if(!current.成功())拒绝(current.状态);
            const auto*fact=std::get_if<概念树共享定义事实>(&*current.数据);if(!fact)拒绝(D::内部不一致);
            std::vector<概念树概念身份>parents;for(const auto&edge:fact->直接上位组)parents.push_back(edge.上位);
            auto required=parent->新父组;std::sort(parents.begin(),parents.end(),idLess);std::sort(required.begin(),required.end(),idLess);
            if(parents!=required)业务拒绝(共享特征业务状态::被后继覆盖);
        }else if(const auto*usage=std::get_if<概念树共享使用请求>(businessRequest)){
            auto current=concepts_.读取共享概念使用({1,{1,fresh,fresh},usage->概念,b.数据});
            if(!current.成功())拒绝(current.状态);
            const auto&published=std::get<概念树共享写入结果>(*step.回执提示);
            if(!published.用途||std::count_if(current.数据->begin(),current.数据->end(),[&](const auto&u){return u==*published.用途;})!=1)拒绝(D::内部不一致);
        }
    }
    尾守卫(concepts_,fresh);s.out.实例=std::move(*instance.数据);
}

void 特征概念应用服务::推进转换(运行会话&s){
    using namespace 共享特征应用内部;auto&m=s.m;const auto&r=std::get<共享特征转换请求>(m.原业务);const auto&b=s.b;
    auto old=concepts_.读取应用概念定义({1,{1,s.g,m.原H},r.旧概念,b.数据});if(!old.成功())拒绝(old.状态);
    const auto*legacy=std::get_if<概念树概念事实>(&*old.数据);
    if(!legacy||legacy->是本体根||legacy->本体根!=root_||!legacy->定义||!std::holds_alternative<概念树特征定义>(*legacy->定义))拒绝(D::不支持);
    const auto&d=std::get<概念树特征定义>(*legacy->定义);
    auto type=features_.读取特征类型域({{1,s.g,m.原H},r.类型域身份,b.数据.特征});if(!type.成功())拒绝(映射(type.状态));
    if(type.Gread!=s.g||type.H!=m.原H||type.数据->内容.FT!=d.特征类型.值||type.数据->内容.算法!=特征长期算法::I64幅度)拒绝(D::不支持);
    特征I64闭区间 range;
    if(const auto*interval=std::get_if<概念树I64区间>(&d.值域))range={interval->下界,interval->上界};
    else{const auto*exact=std::get_if<std::int64_t>(&std::get<概念树精确值>(d.值域));if(!exact)拒绝(D::不支持);range={*exact,*exact};}
    概念树共享特征定义 definition{d.特征类型,type.数据->内容,false,{{type.数据->内容,range}}};
    if(range==std::get<特征I64闭区间>(type.数据->内容.完整域)){definition.完整域=true;definition.原子组.clear();}
    const auto pg=s.准备G(共享阶段角色::旧概念转换,r.旧概念);
    auto group=concepts_.读取共享特征域组({1,{1,s.g,pg},d.特征类型,b.数据});if(!group.成功())拒绝(group.状态);
    std::vector<概念树概念身份>parents;
    if(definition.完整域)parents={root_};
    else for(const auto&p:*group.数据){
        if(p.共享定义==definition||!包含(p.共享定义,definition))continue;
        if(std::any_of(group.数据->begin(),group.数据->end(),[&](const auto&q){return q.概念!=p.概念&&q.共享定义!=definition&&q.共享定义!=p.共享定义&&包含(p.共享定义,q.共享定义)&&包含(q.共享定义,definition);}))continue;
        parents.push_back(p.概念);
    }
    if(parents.empty())拒绝(D::未找到);
    概念树共享建立请求 request{1,{1,pg,r.新建key},definition,概念树旧概念依据{r.旧概念},parents,b.数据};
    const auto&result=std::get<概念树共享写入结果>(s.做步骤(共享阶段角色::旧概念转换,r.旧概念,概念树共享写请求{request}));
    if(!result.概念||result.概念->共享定义!=definition)拒绝(D::内部不一致);
    m.已定位概念=*result.概念;s.out.已确认事实组.push_back(*legacy);
    auto current=concepts_.读取应用概念定义({1,{1,s.g,s.g},result.概念->概念,b.数据});if(!current.成功())拒绝(current.状态);
    const auto*converted=std::get_if<概念树共享定义事实>(&*current.数据);
    if(!converted||converted->共享定义!=definition)拒绝(D::内部不一致);
    if(converted->治理状态==概念树生命周期状态::退役)业务拒绝(共享特征业务状态::需显式治理);
    尾守卫(concepts_,s.g);
}
void 特征概念应用服务::推进治理(运行会话&s){
    using namespace 共享特征应用内部;auto&m=s.m;const auto&r=std::get<共享特征治理请求>(m.原业务);const auto&b=s.b;
    auto original=concepts_.读取应用概念定义({1,{1,s.g,m.原H},r.目标,b.数据});if(!original.成功())拒绝(original.状态);
    const auto*fact=std::get_if<概念树共享定义事实>(&*original.数据);
    if(!fact||fact->本体根!=root_||fact->生命周期值事实!=r.当前生命周期值事实||fact->治理状态!=r.预期状态)拒绝(D::引用冲突);
    auto lifeAt=[](概念树生命周期 x,std::uint64_t h){if(x.退出事实代次&&*x.退出事实代次>h)x.退出事实代次.reset();return x;};
    auto edgesAt=[&](std::vector<概念树直接上位事实>v,std::uint64_t h){for(auto&e:v)e.生命周期=lifeAt(e.生命周期,h);std::sort(v.begin(),v.end(),[](const auto&a,const auto&b){return a.关系.值<b.关系.值;});return v;};
    if(lifeAt(fact->生命周期,m.原H)!=lifeAt(r.生命周期,m.原H)||edgesAt(fact->直接上位组,m.原H)!=edgesAt(r.预期父组,m.原H))拒绝(D::引用冲突);
    auto graph=concepts_.读取应用概念图({1,{{1,s.g,m.原H},概念树根角色::特征,false,b.数据.基础},b.数据});if(!graph.成功())拒绝(graph.状态);
    std::vector<概念树直接上位事实>children;for(const auto&e:graph.数据->直接边组)if(e.上位==r.目标)children.push_back(e);
    if(edgesAt(children,m.原H)!=edgesAt(r.预期子组,m.原H))拒绝(D::引用冲突);
    auto checkAdoption=[&](const 概念树共享实例见证&w){
        if(!w.H||w.H>s.g)拒绝();
        auto e=existences_.读取存在历史事实({1,s.g,w.H,w.E,b.数据.基础.最大世界成员数});
        if(!e.成功())拒绝(映射(e.状态));
        if(e.Gread!=s.g||e.H!=w.H||std::count_if(e.存在->特征组.begin(),e.存在->特征组.end(),[&](const auto&m){return m.成员关系==w.成员关系&&m.目标结点==w.F;})!=1)拒绝(D::引用冲突);
        auto f=features_.按实例读取特征历史事实({1,s.g,w.H,w.F,b.数据.基础.最大特征属性数});
        if(!f.成功())拒绝(映射(f.状态));
        if(f.Gread!=s.g||f.H!=w.H||f.特征->值事实!=w.值事实)拒绝(D::引用冲突);
        auto matched=concepts_.判定应用特征模板({1,{1,s.g,w.H},r.目标,{w.F,w.值事实,f.特征->特征类型,w.H},b.数据});
        if(!matched.成功())拒绝(matched.状态);
        if(matched.Gread!=s.g||matched.H!=w.H||!matched.数据->适用)拒绝(D::引用冲突);
    };
    if(r.重新采用见证){if(r.重新采用见证->H>m.原H)拒绝();checkAdoption(*r.重新采用见证);}
    for(const auto&ref:r.显式释放引用组)if(ref.概念!=r.目标)拒绝(D::引用冲突);
    if(!r.显式释放引用组.empty()){
        const auto pg=s.准备G(共享阶段角色::引用释放,r.目标),key=s.取键(共享阶段角色::引用释放,r.目标);
        概念树共享释放请求 request{1,{1,pg,key},r.显式释放引用组,b.数据};
        s.做步骤(共享阶段角色::引用释放,r.目标,概念树共享写请求{request});
    }
    const auto savedLife=s.找步骤(共享阶段角色::治理,r.目标);
    if(savedLife||r.预期状态!=r.目标治理状态){
        const auto pg=s.准备G(共享阶段角色::治理,r.目标),key=s.取键(共享阶段角色::治理,r.目标);
        auto adoption=r.重新采用见证;
        if(adoption){adoption->H=pg;checkAdoption(*adoption);}
        概念树共享生命周期请求 request{1,{{1,pg,key},r.目标,r.当前生命周期值事实,r.预期状态,r.目标治理状态,b.数据.基础},adoption,b.数据};
        const auto&result=std::get<概念树共享写入结果>(s.做步骤(共享阶段角色::治理,r.目标,概念树共享写请求{request}));
        if(!result.概念||result.概念->治理状态!=r.目标治理状态)拒绝(D::内部不一致);
        m.已定位概念=*result.概念;
    }else m.已定位概念=*fact;
    if(r.替代子父组){
        const auto pg=s.准备G(共享阶段角色::概念退出,r.目标),key=s.取键(共享阶段角色::概念退出,r.目标);
        概念树共享退出请求 request{1,{{1,pg,key},r.目标,m.已定位概念->生命周期值事实,b.数据.基础},*r.替代子父组,b.数据};
        const auto&result=std::get<概念树共享写入结果>(s.做步骤(共享阶段角色::概念退出,r.目标,概念树共享写请求{request}));
        if(!result.概念||!result.首次H||result.概念->生命周期.退出事实代次!=result.首次H)拒绝(D::内部不一致);
        m.已定位概念=*result.概念;
        auto fresh=concepts_.读取应用概念图({1,{{1,s.g,s.g},概念树根角色::特征,false,b.数据.基础},b.数据});
        if(!fresh.成功())拒绝(fresh.状态);
        if(fresh.Gread!=s.g||fresh.H!=s.g)拒绝(D::内部不一致);
        if(std::any_of(fresh.数据->节点组.begin(),fresh.数据->节点组.end(),[&](const auto&node){return std::visit([&](const auto&x){return x.概念==r.目标;},node);}))业务拒绝(共享特征业务状态::被后继覆盖);
        auto less=[](auto a,auto b){return a.值.值<b.值.值;};
        for(const auto&replacement:*r.替代子父组){
            auto child=std::find_if(fresh.数据->节点组.begin(),fresh.数据->节点组.end(),[&](const auto&node){return std::visit([&](const auto&x){return x.概念==replacement.子节点;},node);});
            if(child==fresh.数据->节点组.end())业务拒绝(共享特征业务状态::被后继覆盖);
            const auto*definition=std::get_if<概念树共享定义事实>(&*child);if(!definition)拒绝(D::内部不一致);
            std::vector<概念树概念身份>parents;for(const auto&edge:definition->直接上位组)parents.push_back(edge.上位);
            auto required=replacement.新父组;std::sort(parents.begin(),parents.end(),less);std::sort(required.begin(),required.end(),less);
            if(parents!=required)业务拒绝(共享特征业务状态::被后继覆盖);
        }
    }else{
        auto current=concepts_.读取应用概念定义({1,{1,s.g,s.g},r.目标,b.数据});if(!current.成功())拒绝(current.状态);
        const auto*now=std::get_if<概念树共享定义事实>(&*current.数据);
        if(!now||now->治理状态!=r.目标治理状态||now->生命周期值事实!=m.已定位概念->生命周期值事实)业务拒绝(共享特征业务状态::被后继覆盖);
        m.已定位概念=*now;s.out.已确认事实组.push_back(*now);
    }
    尾守卫(concepts_,s.g);
}
共享特征操作结果 特征概念应用服务::推进(共享特征继续材料 material,std::uint64_t g,const 共享特征应用预算&budget,const 场景类数据服务*scene){
    using namespace 共享特征应用内部;
    共享特征操作结果 out;out.原H=material.原H;out.Gread=g;out.继续材料=std::move(material);
    auto&m=*out.继续材料;运行会话 session{*this,out,m,budget,scene,g};
    session.未核验=!m.步骤.empty()||m.缺步骤恢复.has_value();out.可能已发布=session.未核验;
    if(session.未核验)out.阶段=共享特征阶段状态::先前阶段待核验;
    try{
        核验纯材料(m,budget,g);尾守卫(concepts_,g);m.待补键目标组.clear();
        if(std::holds_alternative<共享特征生成请求>(m.原业务)||std::holds_alternative<共享特征关联请求>(m.原业务))推进实例(session);
        else if(std::holds_alternative<共享特征治理请求>(m.原业务))推进治理(session);
        else 推进转换(session);
        if(m.缺步骤恢复||session.已核验.size()!=m.步骤.size())拒绝();
        尾守卫(concepts_,session.g);out.Gread=session.g;out.状态=共享特征业务状态::完成;
        out.阶段=共享特征阶段状态::全部确认;out.可能已发布=false;
        return out;
    }catch(const 业务失败&e){out.状态=e.状态;}
    catch(const 失败&e){out.状态=业务映射(e.状态);}
    catch(const std::bad_alloc&){out.状态=共享特征业务状态::资源失败;}
    catch(const std::length_error&){out.状态=共享特征业务状态::资源失败;}
    catch(...){out.状态=共享特征业务状态::内部不一致;}
    out.实例.reset();out.Gread=session.g;
    out.可能已发布=m.缺步骤恢复.has_value()||session.已核验.size()!=m.步骤.size();
    if(out.可能已发布)out.阶段=共享特征阶段状态::先前阶段待核验;
    else if(!out.已确认事实组.empty())out.阶段=共享特征阶段状态::部分确认;
    else out.阶段=共享特征阶段状态::未派发;
    return out;
}
共享特征操作结果 特征概念应用服务::开始(共享特征原业务 business,const std::optional<共享场景限制>&limit,const 场景类数据服务*scene){
    using namespace 共享特征应用内部;
    共享特征继续材料 m;m.原H=原截止(business);m.原业务=std::move(business);m.场景限制=limit;
    const auto g=初始G(m.原业务);const auto b=std::visit([](const auto&r){return r.预算;},m.原业务);
    return 推进(std::move(m),g,b,scene);
}
共享特征操作结果 特征概念应用服务::继续共享特征操作(const 共享特征继续请求&r){
    return 继续内部(r,nullptr,nullptr);
}
共享特征操作结果 特征概念应用服务::继续共享特征操作(const 共享特征继续请求&r,const 共享场景限制&limit,const 场景类数据服务&scene){
    return 继续内部(r,&limit,&scene);
}
共享特征操作结果 特征概念应用服务::继续内部(const 共享特征继续请求&r,const 共享场景限制*limit,const 场景类数据服务*scene){
    共享特征操作结果 failure;failure.原H=r.材料.原H;failure.Gread=r.Gread;
    failure.可能已发布=!r.材料.步骤.empty()||r.材料.缺步骤恢复.has_value();failure.阶段=failure.可能已发布 ? 共享特征阶段状态::先前阶段待核验 : 共享特征阶段状态::未派发;
    try{
        failure.继续材料=r.材料;
        if(r.版本!=1||bool(limit)!=r.材料.场景限制.has_value()||(limit&&*limit!=*r.材料.场景限制))return failure;
        auto m=r.材料;
        for(const auto&k:r.补充键){
            if(std::count_if(m.待补键目标组.begin(),m.待补键目标组.end(),[&](const auto&t){return t.阶段角色==k.阶段角色&&t.目标==k.目标;})!=1)return failure;
            m.补充键.push_back(k);
        }
        核验纯材料(m,r.本次预算,r.Gread);
        return 推进(std::move(m),r.Gread,r.本次预算,scene);
    }catch(const 共享特征应用内部::失败&e){failure.状态=共享特征应用内部::业务映射(e.状态);}
    catch(const std::bad_alloc&){failure.状态=共享特征业务状态::资源失败;}
    catch(const std::length_error&){failure.状态=共享特征业务状态::资源失败;}
    catch(...){failure.状态=共享特征业务状态::内部不一致;}
    return failure;
}

} // namespace 海中鱼巣
