module;
#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <new>
#include <optional>
#include <stdexcept>
#include <vector>

export module 海中鱼巣.领域.算法.有序I64特征比较;
export import 海中鱼巣.领域.数据服务.特征类;

export namespace 海中鱼巣 {
enum class 特征比较状态 : std::uint8_t {
    已比较=1,未注册=2,入口拒绝=3,版本漂移=4,
    许可拒绝=5, // 4180 历史数值洞；本 provider 永不产出。
    资源失败=6,内部不一致=7,未实现=8
};
enum class 特征比较拒绝原因 : std::uint8_t {
    无=0,未注册=1,注册不唯一=2,注册失效=3,算法版本不匹配=4,
    原始类型不匹配=5,单位不匹配=6,维度不匹配=7,左右角色不合法=8,
    值域不允许=9,坐标或位姿不成立=10,时间基准不成立=11,样本不足=12,
    差异不可表示=13,距离不可表示=14,误差合同不成立=15,输入版本失效=16,
    请求合同版本不匹配=17,请求身份或特征定义不完整=18,要求结果组合不支持=19,未实现=20
};
enum class 特征比较结果要求 : std::uint8_t { 排序三态=1,具名关系=2,差异材料=4 };
enum class 特征比较合同来源 : std::uint8_t { 当前独立绑定=1,已保存定义固定K=2 };
struct 有序I64比较合同快照 final {
    特征比较合同来源 来源=特征比较合同来源::当前独立绑定;
    特征I64比较绑定身份 K;
    特征类型身份 输入FT;
    特征I64比较用途 用途=特征I64比较用途::识别区分;
    特征比较算法族 算法族=特征比较算法族::标量有序比较与安全差异;
    std::uint32_t 算法版本=1;
    特征I64输入角色 左角色=特征I64输入角色::识别左,右角色=特征I64输入角色::识别右;
    std::uint8_t 上下文要求位=0;
    特征类标量量化合同 输入量化;
    std::uint32_t 误差合同版本=1;
    std::optional<std::int64_t> 误差预算,相等容差;
    std::optional<特征I64关系编码> 关系编码;
    std::vector<特征I64比较绑定输出事实> 输出组;
    std::uint64_t Gread=0,H=0;
    friend bool operator==(const 有序I64比较合同快照&,const 有序I64比较合同快照&)=default;
};
struct 特征比较输入 final {
    特征I64输入角色 角色=特征I64输入角色::识别左; std::int64_t 值=0;
};
struct 特征比较执行请求 final {
    std::uint32_t 版本=1; std::uint64_t 请求身份=0;
    有序I64比较合同快照 K;
    std::array<特征比较输入,2> 输入;
    std::uint8_t 要求结果位=0;
};
struct 特征比较执行输出 final {
    特征类型身份 输出FT;
    特征类标量结果角色 角色=特征类标量结果角色::排序;
    std::int64_t 值=0; 特征类标量量化合同 量化;
    特征类标量方向 方向=特征类标量方向::左对右;
};
struct 特征比较执行结果 final {
    std::uint32_t 版本=1;
    特征比较状态 状态=特征比较状态::入口拒绝;
    特征比较拒绝原因 拒绝原因=特征比较拒绝原因::请求合同版本不匹配;
    std::uint64_t 请求身份=0;
    特征I64比较绑定身份 K;
    std::uint32_t 算法版本=0;
    std::uint8_t 实际结果位=0;
    std::vector<特征比较执行输出> 输出组;
    bool 成功() const noexcept {
        if(版本!=1 || 状态!=特征比较状态::已比较 || 拒绝原因!=特征比较拒绝原因::无
            || !请求身份 || !有效(K) || 算法版本!=1 || 实际结果位<1 || 实际结果位>7
            || 输出组.empty() || 输出组.size()>3) return false;
        unsigned previous=0,bits=0;
        for(const auto& o:输出组) {
            const auto role=static_cast<unsigned>(o.角色);
            if(role<1 || role>3 || role<=previous || !有效(o.输出FT) || !o.量化.完整()
                || o.值<o.量化.下界 || o.值>o.量化.上界 || static_cast<unsigned>(o.方向)!=role
                || (role==1&&(o.值<-1||o.值>1))) return false;
            previous=role;bits|=1U<<(role-1);
        }
        return bits==实际结果位;
    }
};
// 向上送出诊断；这是计算应用内部纯执行器，不提供事实来源或路由资格。
class 有序I64特征比较提供者 final {
public:
    有序I64特征比较提供者() noexcept=default;
    特征比较执行结果 比较(const 特征比较执行请求& r) const noexcept {
        特征比较执行结果 out;out.请求身份=r.请求身份;out.K=r.K.K;out.算法版本=r.K.算法版本;
        auto fail=[&](特征比较状态 state,特征比较拒绝原因 why) {
            out.状态=state;out.拒绝原因=why;out.实际结果位=0;out.输出组.clear();
        };
        using S=特征比较状态;using E=特征比较拒绝原因;
        try {
            if(r.版本!=1) {fail(S::版本漂移,E::请求合同版本不匹配);return out;}
            const auto& k=r.K;
            if(!r.请求身份 || !有效(k.K) || !有效(k.输入FT) || !k.H || k.H>k.Gread
                || (k.来源!=特征比较合同来源::当前独立绑定 && k.来源!=特征比较合同来源::已保存定义固定K)) {
                fail(S::入口拒绝,E::请求身份或特征定义不完整);return out;
            }
            if(k.算法族!=特征比较算法族::标量有序比较与安全差异 || k.算法版本!=1) {
                fail(S::未实现,E::未实现);return out;
            }
            if(r.要求结果位<1 || r.要求结果位>7) {fail(S::入口拒绝,E::要求结果组合不支持);return out;}
            const auto purpose=static_cast<unsigned>(k.用途);
            if(purpose<1 || purpose>5 || static_cast<unsigned>(k.左角色)!=2*purpose-1
                || static_cast<unsigned>(k.右角色)!=2*purpose || r.输入[0].角色!=k.左角色 || r.输入[1].角色!=k.右角色
                || k.上下文要求位>31) {fail(S::入口拒绝,E::左右角色不合法);return out;}
            if(!k.输入量化.完整() || k.误差合同版本!=1 || (k.误差预算&&*k.误差预算<0)
                || (k.相等容差&&*k.相等容差<0)) {fail(S::入口拒绝,E::误差合同不成立);return out;}
            const auto left=r.输入[0].值,right=r.输入[1].值;
            if(left<k.输入量化.下界 || left>k.输入量化.上界 || right<k.输入量化.下界 || right>k.输入量化.上界) {
                fail(S::入口拒绝,E::值域不允许);return out;
            }
            特征I64比较绑定定义 definition{k.输入FT,k.用途,k.算法族,k.算法版本,k.左角色,k.右角色,
                k.上下文要求位,k.输入量化,k.误差合同版本,k.误差预算,k.相等容差,k.关系编码,{}};
            unsigned available=0;
            for(const auto& o:k.输出组) {
                if(!有效(o.输出关系)) {fail(S::入口拒绝,E::请求身份或特征定义不完整);return out;}
                definition.输出组.push_back({o.输出,o.输出FT});
                const auto role=static_cast<unsigned>(o.输出.角色);
                if(role<1||role>3){fail(S::入口拒绝,E::要求结果组合不支持);return out;}
                available|=1U<<(role-1);
            }
            if(!I64绑定定义完整(definition)||(r.要求结果位&~available)) {
                fail(S::入口拒绝,E::要求结果组合不支持);return out;
            }
            // 比较先于减法；幅度使用无符号差，覆盖 INT64_MIN..INT64_MAX。
            const std::int64_t order=left<right ? -1:left>right ? 1:0;
            const auto magnitude=left<right ? static_cast<std::uint64_t>(right)-static_cast<std::uint64_t>(left)
                :static_cast<std::uint64_t>(left)-static_cast<std::uint64_t>(right);
            const bool equivalent=order==0 || (k.相等容差 && magnitude<=static_cast<std::uint64_t>(*k.相等容差));
            for(const auto& o:k.输出组) {
                const auto role=static_cast<unsigned>(o.输出.角色);
                if(!(r.要求结果位&(1U<<(role-1)))) continue;
                std::int64_t value=order;
                if(role==2) value=equivalent ? k.关系编码->等价:order<0 ? k.关系编码->左小于:k.关系编码->左大于;
                if(role==3) {
                    if((left<0&&right>INT64_MAX+left)||(left>0&&right<INT64_MIN+left)) {
                        fail(S::入口拒绝,E::差异不可表示);return out;
                    }
                    value=right-left;
                }
                if(value<o.输出.量化.下界||value>o.输出.量化.上界) {fail(S::入口拒绝,E::值域不允许);return out;}
                out.输出组.push_back({o.输出FT,o.输出.角色,value,o.输出.量化,static_cast<特征类标量方向>(role)});
            }
            out.实际结果位=r.要求结果位;out.状态=S::已比较;out.拒绝原因=E::无;
            if(!out.成功()) fail(S::内部不一致,E::请求身份或特征定义不完整);
        } catch(const std::bad_alloc&) {fail(S::资源失败,E::无);}
        catch(const std::length_error&) {fail(S::资源失败,E::无);}
        catch(...) {fail(S::内部不一致,E::无);}
        return out;
    }
};
}
