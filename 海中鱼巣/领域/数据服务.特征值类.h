#pragma once

#include <cstdint>
#include <new>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>
#include "../核心/服务.L1事实基座.h"
#include "数据服务.不可变材料.h"



namespace 海中鱼巣 {

struct 特征值身份 final {
    稳定编码 编码{};
    friend bool operator==(const 特征值身份&,
        const 特征值身份&) = default;
};

inline bool 有效(特征值身份 身份) noexcept {
    return 有效(身份.编码);
}

struct 特征值独立材料引用 final {
    稳定编码 身份{};
    friend bool operator==(const 特征值独立材料引用&,
        const 特征值独立材料引用&) = default;
};

using 特征值内容 = std::variant<std::int64_t,
    std::vector<std::int64_t>, std::vector<std::uint64_t>,
    特征值独立材料引用>;

enum class 特征值表示类型 : std::uint8_t {
    I64 = 1,
    I64组 = 2,
    U64组 = 3,
    独立材料引用 = 4
};

struct 特征值信息 final {
    特征值身份 值身份{};
    特征值内容 值内容;
    friend bool operator==(const 特征值信息&,
        const 特征值信息&) = default;
};

enum class 特征值读取错误 : std::uint8_t {
    入口拒绝, 未找到, 材料已清理, 能力未提供, 资源失败, 内部不一致
};

using 特征值读取结果 = std::variant<特征值信息, 特征值读取错误>;

enum class 特征值读取状态_B1 : std::uint8_t {
    已读取=1, 未找到=8, 入口拒绝=9,
    类型不相容=15, 规则未提供=17, 事实代次漂移=22,
    数量预算不足=24, 资源失败=26,
    内部不一致=27, 旧格式不支持=29
};
struct 特征值完整读取请求_B1 final {
    std::uint32_t 版本{1}; std::uint64_t Gread{};
    特征值身份 值{}; 世界结构预算_B1 预算{};
    friend bool operator==(const 特征值完整读取请求_B1&,
                           const 特征值完整读取请求_B1&) = default;
};
struct 完整特征值_B1 final {
    特征值身份 身份{}; 特征值表示类型 表示{特征值表示类型::I64};
    特征值内容 内容;
    friend bool operator==(const 完整特征值_B1&,const 完整特征值_B1&) = default;
};
struct 特征值完整读取结果_B1 final {
    std::uint32_t 版本{1}; 特征值读取状态_B1 状态{特征值读取状态_B1::入口拒绝};
    std::uint64_t Gread{}; std::optional<完整特征值_B1> 值;
    std::optional<材料事实_B1> 材料; 世界结构用量_B1 用量{};
    bool 成功(const 特征值完整读取请求_B1&) const noexcept;
};

// B2 只保存 L1 已定义的精确 U64 序列；不得在此解释轮廓、体素或其它业务格式。
struct 特征值U64组保存请求_B2 final {
    std::uint32_t 版本{1}; std::uint64_t G0{};
    L1所有者范围写入幂等身份 幂等{};
    std::vector<std::uint64_t> 内容; 世界结构预算_B1 预算{};
};
enum class 特征值U64组保存状态_B2 : std::uint8_t {
    已复用=1, 已保存=2, 入口拒绝=8, 结构未就绪=9, 许可拒绝=10,
    未找到=11, 事实代次漂移=13, 数量预算不足=14,
    幂等冲突=15, 引用冲突=16, 资源失败=17, 内部不一致=18,
    已可能发布=19
};
struct 特征值U64组保存结果_B2 final {
    std::uint32_t 版本{1}; 特征值U64组保存状态_B2 状态{特征值U64组保存状态_B2::入口拒绝};
    std::uint64_t Gread{}; std::optional<特征值身份> 值;
    世界结构用量_B1 用量{};
    bool 成功(const 特征值U64组保存请求_B2&) const noexcept;
};
struct 特征值U64组结构登记请求_B2 final {
    std::uint32_t 版本{1}; std::uint64_t G0{};
    L1所有者范围写入幂等身份 幂等{};
};
struct 特征值U64组结构交付_B2 final {
    L1结构所有者身份 所有者{};
    稳定编码 承载节点{}; 稳定编码 U64组属性类型节点{}; 稳定编码 来源节点{};
};
enum class 特征值U64组结构登记状态_B2 : std::uint8_t {
    已登记=1, 入口拒绝=8, 许可拒绝=9, 未找到=10,
    事实代次漂移=12, 幂等冲突=13, 引用冲突=14, 资源失败=15,
    内部不一致=16, 已可能发布=17
};
struct 特征值U64组结构登记结果_B2 final {
    std::uint32_t 版本{1}; 特征值U64组结构登记状态_B2 状态{特征值U64组结构登记状态_B2::入口拒绝};
    std::uint64_t Gread{}; std::optional<特征值U64组结构交付_B2> 交付;
    bool 成功(const 特征值U64组结构登记请求_B2&) const noexcept;
};

class 特征值类数据服务 final {
public:
    explicit 特征值类数据服务(const L1事实基座服务& 第一层服务) noexcept
        : 第一层服务_(第一层服务), 材料服务_(nullptr) {}
    特征值类数据服务(const L1事实基座服务& 第一层服务,
                     const 不可变材料数据服务& 材料服务) noexcept
        : 第一层服务_(第一层服务),
          材料服务_(材料服务.绑定于(第一层服务) ? &材料服务 : nullptr) {}
    特征值类数据服务(const L1事实基座服务& 第一层服务,
                     L1所有者范围写端口&& 写端口,
                     const 特征值U64组结构交付_B2& 结构) noexcept;

    特征值类数据服务() = delete;
    特征值类数据服务(const 特征值类数据服务&) = delete;
    特征值类数据服务& operator=(const 特征值类数据服务&) = delete;
    特征值类数据服务(特征值类数据服务&&) = delete;
    特征值类数据服务& operator=(特征值类数据服务&&) = delete;

    bool 绑定于(const L1事实基座服务& 第一层服务) const noexcept {
        return &第一层服务 == &第一层服务_;
    }

    static 特征值表示类型 获取值表示类型(
        const 特征值信息& 特征值) noexcept {
        return std::visit([](const auto& 值) noexcept {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, std::int64_t>)
                return 特征值表示类型::I64;
            else if constexpr (std::is_same_v<类型,
                    std::vector<std::int64_t>>)
                return 特征值表示类型::I64组;
            else if constexpr (std::is_same_v<类型,
                    std::vector<std::uint64_t>>)
                return 特征值表示类型::U64组;
            else
                return 特征值表示类型::独立材料引用;
        }, 特征值.值内容);
    }

    static 特征值身份 获取值身份(const 特征值信息& 特征值) noexcept {
        return 特征值.值身份;
    }

    static const 特征值内容& 获取值内容(
        const 特征值信息& 特征值) noexcept {
        return 特征值.值内容;
    }

    static bool 内容结构有效(const 特征值内容& 内容) noexcept {
        if (内容.valueless_by_exception()) return false;
        if (const auto* 引用 = std::get_if<特征值独立材料引用>(&内容))
            return 有效(引用->身份);
        return true;
    }

    // 诊断责任：向上送出。这里只读取仍然存在的当前值事实。
    特征值读取结果 获取特征值(特征值身份 身份) const {
        if (!有效(身份)) return 特征值读取错误::入口拒绝;
        try {
            const L1所有者范围事实读取请求 请求{
                L1所有者范围CRUD合同版本, 身份.编码};
            auto 当前 = 第一层服务_.读取所有者范围当前值(请求);
            if (当前.状态 == L1所有者范围读取状态::成功)
                return 投影读取事实(身份, 当前.事实);
            return 映射读取错误(当前.状态);
        } catch (const std::bad_alloc&) {
            return 特征值读取错误::资源失败;
        } catch (const std::length_error&) {
            return 特征值读取错误::资源失败;
        } catch (...) {
            return 特征值读取错误::内部不一致;
        }
    }

    特征值完整读取结果_B1 获取完整值(
        const 特征值完整读取请求_B1& 请求) const noexcept;
    特征值完整读取结果_B1 读取完整U64组_B2(
        const 特征值完整读取请求_B1& 请求) const noexcept;
    特征值U64组保存结果_B2 保存U64组(
        const 特征值U64组保存请求_B2& 请求) noexcept;
    static 特征值U64组结构登记结果_B2 登记U64组结构_B2(
        const L1事实基座服务&, L1所有者范围写端口&,
        const 特征值U64组结构登记请求_B2&) noexcept;

private:
    static 特征值读取错误 映射读取错误(L1所有者范围读取状态 状态) noexcept {
        switch (状态) {
        case L1所有者范围读取状态::未找到:
            return 特征值读取错误::未找到;
        case L1所有者范围读取状态::入口拒绝:
            return 特征值读取错误::入口拒绝;
        case L1所有者范围读取状态::资源失败:
            return 特征值读取错误::资源失败;
        default: return 特征值读取错误::内部不一致;
        }
    }

    static 特征值读取结果 投影读取事实(特征值身份 身份,
        const std::optional<L1所有者范围事实副本>& 事实) {
        if (!事实) return 特征值读取错误::内部不一致;
        const auto* 值 = std::get_if<L1所有者范围值事实>(&*事实);
        if (值 == nullptr) return 特征值读取错误::入口拒绝;
        if (值->编码 != 身份.编码 || !有效(值->所属节点)
            || !有效(值->属性类型节点) || !有效(值->来源节点)
            || !有效(值->写入所有者) || 值->创建事实代次 == 0
            || !L1所有者范围原始材料完整(值->材料))
            return 特征值读取错误::内部不一致;
        auto 内容 = 转换为特征值内容(值->材料);
        if (!内容) return 特征值读取错误::能力未提供;
        return 特征值信息{身份, std::move(*内容)};
    }

    // 独立材料分支必须由正式不可变材料服务强类型读回。当前正式代码
    // 没有该 provider，因此不能仅凭非零编码把它投影为已核验内容。
    static std::optional<特征值内容> 转换为特征值内容(
        const L1所有者范围原始值材料& 材料) {
        return std::visit([](const auto& 值)
                -> std::optional<特征值内容> {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型,
                    L1所有者范围独立材料引用>)
                return std::nullopt;
            else
                return 特征值内容{值};
        }, 材料);
    }

    const L1事实基座服务& 第一层服务_;
    const 不可变材料数据服务* 材料服务_;
    std::optional<L1所有者范围写端口> 写端口_;
    std::optional<特征值U64组结构交付_B2> U64组结构_;
};

} // namespace 海中鱼巣

namespace 海中鱼巣 {

inline bool 特征值完整读取结果_B1::成功(
    const 特征值完整读取请求_B1& r) const noexcept {
    try {
    if (r.版本!=1||!r.Gread||!有效(r.值)||!世界结构预算有效(r.预算)||
        版本 != 1 || 状态 != 特征值读取状态_B1::已读取 || Gread != r.Gread ||
        !值 || 值->身份 != r.值 || 值->内容.valueless_by_exception()||
        !用量.最大值数||用量.最大值数>r.预算.最大值数||
        用量.最大节点数>r.预算.最大节点数||用量.最大关系数>r.预算.最大关系数||
        用量.最大祖先数>r.预算.最大祖先数||用量.最大后代数>r.预算.最大后代数||
        用量.最大候选数>r.预算.最大候选数||用量.最大值元素数>r.预算.最大值元素数||
        用量.最大材料字节数>r.预算.最大材料字节数||用量.最大域原子数>r.预算.最大域原子数)
        return false;
    const auto index=值->内容.index();
    switch(值->表示){
    case 特征值表示类型::I64:if(index!=0)return false;break;
    case 特征值表示类型::I64组:if(index!=1)return false;break;
    case 特征值表示类型::U64组:if(index!=2)return false;break;
    case 特征值表示类型::独立材料引用:if(index!=3)return false;break;
    default:return false;
    }
    const auto* ref=std::get_if<特征值独立材料引用>(&值->内容);
    if(static_cast<bool>(ref)!=材料.has_value())return false;
    if(!ref){
        if(材料||用量.最大值数!=1)return false;
        if(const auto* scalar=std::get_if<std::int64_t>(&值->内容))return 用量.最大值元素数==0;
        if(const auto* group=std::get_if<std::vector<std::int64_t>>(&值->内容))return 用量.最大值元素数==group->size();
        if(const auto* group=std::get_if<std::vector<std::uint64_t>>(&值->内容))return 用量.最大值元素数==group->size();
        return false;
    }
    if(!有效(ref->身份)||材料->材料.值!=ref->身份||用量.最大值数<4)return false;
    auto materialUsage=用量;--materialUsage.最大值数;
    材料读取结果_B1 proof;proof.状态=材料状态_B1::已读取;proof.Gread=Gread;
    proof.用量=materialUsage;proof.材料=材料;
    return proof.成功({1,Gread,{ref->身份},r.预算});
    } catch (...) {
        return false;
    }
}

inline 特征值完整读取结果_B1 特征值类数据服务::获取完整值(
    const 特征值完整读取请求_B1& r) const noexcept {
    特征值完整读取结果_B1 out; out.Gread=r.Gread;
    try {
        if(r.版本!=1||!r.Gread||!有效(r.值)||
           !世界结构预算有效(r.预算)) return out;
        if(r.预算.最大值数<1){out.状态=特征值读取状态_B1::数量预算不足;return out;}
        const auto raw=第一层服务_.读取所有者范围当前值(
            {L1所有者范围CRUD合同版本,r.值.编码}); out.Gread=raw.读取事实代次;
        ++out.用量.最大值数;
        if(raw.读取事实代次!=r.Gread){out.状态=特征值读取状态_B1::事实代次漂移;return out;}
        if(raw.状态!=L1所有者范围读取状态::成功||!raw.事实){
            switch(raw.状态){case L1所有者范围读取状态::未找到:out.状态=特征值读取状态_B1::未找到;break;
            case L1所有者范围读取状态::资源失败:out.状态=特征值读取状态_B1::资源失败;break;
            default:out.状态=特征值读取状态_B1::内部不一致;}return out;
        }
        const auto*v=std::get_if<L1所有者范围值事实>(&*raw.事实);
        if(!v||v->编码!=r.值.编码){out.状态=特征值读取状态_B1::内部不一致;return out;}
        if(v->创建事实代次>r.Gread){out.状态=特征值读取状态_B1::未找到;return out;}
        特征值内容 content; 特征值表示类型 representation;
        if(const auto*x=std::get_if<std::int64_t>(&v->材料)){content=*x;representation=特征值表示类型::I64;}
        else if(const auto*x=std::get_if<std::vector<std::int64_t>>(&v->材料)){if(x->size()>r.预算.最大值元素数){out.状态=特征值读取状态_B1::数量预算不足;return out;}content=*x;out.用量.最大值元素数=x->size();representation=特征值表示类型::I64组;}
        else if(const auto*x=std::get_if<std::vector<std::uint64_t>>(&v->材料)){if(x->size()>r.预算.最大值元素数){out.状态=特征值读取状态_B1::数量预算不足;return out;}content=*x;out.用量.最大值元素数=x->size();representation=特征值表示类型::U64组;}
        else {
            const auto&ref=std::get<L1所有者范围独立材料引用>(v->材料);
            content=特征值独立材料引用{ref.编码};representation=特征值表示类型::独立材料引用;
            if(!材料服务_){out.状态=特征值读取状态_B1::规则未提供;return out;}
            if(r.预算.最大值数<4){out.状态=特征值读取状态_B1::数量预算不足;return out;}
            auto materialBudget=r.预算;--materialBudget.最大值数;
            const 材料读取请求_B1 mr{1,r.Gread,{ref.编码},materialBudget};auto material=材料服务_->读取材料(mr);
            out.Gread=material.Gread;
            out.用量.最大节点数+=material.用量.最大节点数;
            out.用量.最大关系数+=material.用量.最大关系数;
            out.用量.最大值数+=material.用量.最大值数;
            out.用量.最大祖先数+=material.用量.最大祖先数;
            out.用量.最大后代数+=material.用量.最大后代数;
            out.用量.最大候选数+=material.用量.最大候选数;
            out.用量.最大值元素数+=material.用量.最大值元素数;
            out.用量.最大材料字节数+=material.用量.最大材料字节数;
            out.用量.最大域原子数+=material.用量.最大域原子数;
            if(!material.成功(mr)){
                out.状态=material.状态==材料状态_B1::未找到 ? 特征值读取状态_B1::未找到 :
                    material.状态==材料状态_B1::事实代次漂移 ? 特征值读取状态_B1::事实代次漂移 :
                    material.状态==材料状态_B1::数量预算不足 ? 特征值读取状态_B1::数量预算不足 :
                    material.状态==材料状态_B1::资源失败 ? 特征值读取状态_B1::资源失败 :
                    material.状态==材料状态_B1::规则未提供 ? 特征值读取状态_B1::规则未提供 : 特征值读取状态_B1::内部不一致;
                return out;
            }
            out.材料=std::move(material.材料);
        }
        const auto tail=第一层服务_.读取中性当前事实代次({L1中性CRUD合同版本});
        if(tail.状态!=L1中性读取状态::成功){out.状态=tail.状态==L1中性读取状态::资源失败 ? 特征值读取状态_B1::资源失败 : 特征值读取状态_B1::内部不一致;return out;}
        if(tail.事实代次!=r.Gread){out.Gread=tail.事实代次;out.状态=特征值读取状态_B1::事实代次漂移;return out;}
        out.值=完整特征值_B1{r.值,representation,std::move(content)};out.状态=特征值读取状态_B1::已读取;
        return out;
    } catch(const std::bad_alloc&){out.状态=特征值读取状态_B1::资源失败;}
      catch(const std::length_error&){out.状态=特征值读取状态_B1::资源失败;}
      catch(...){out.状态=特征值读取状态_B1::内部不一致;}
    return out;
}

} // namespace 海中鱼巣
