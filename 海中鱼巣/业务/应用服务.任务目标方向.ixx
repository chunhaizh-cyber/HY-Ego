module;
#include <cstdint>
#include <new>
#include <optional>
#include <stdexcept>
export module 海中鱼巣.业务.应用服务.任务目标方向;
export import 海中鱼巣.业务.应用服务.需求目标方向;
export import 海中鱼巣.领域.数据服务.任务类;
export namespace 海中鱼巣 {
inline constexpr std::uint32_t 任务目标方向合同版本 = 1;
enum class 任务目标方向状态 : std::uint8_t {
    已计算=1,入口拒绝=2,任务未找到=3,任务已退出=4,
    来源需求未找到=5,来源需求已退出=6,来源不匹配=7,
    需求方向失败=8,事实代次漂移=9,预算不足=10,资源失败=11,内部不一致=12
};
struct 任务目标方向请求 final {
    std::uint32_t 版本=1;
    std::uint64_t G=0,请求身份=0;
    任务类记录身份 任务;
    特征信息身份 当前F;
    std::uint8_t 要求结果位=0;
    二次计算预算 预算;
    friend bool operator==(const 任务目标方向请求&,const 任务目标方向请求&)=default;
};
struct 任务目标方向结果 final {
    std::uint32_t 版本=1;
    任务目标方向状态 状态=任务目标方向状态::入口拒绝;
    任务目标方向请求 原请求;
    std::uint64_t G=0;
    任务类记录身份 任务;
    稳定编码 所属存在{};
    需求类记录身份 来源需求;
    稳定编码 来源需求所属存在{};
    std::optional<需求目标方向结果> 方向;
    bool 成功() const noexcept {
        const auto& r=原请求;
        if(版本!=1||状态!=任务目标方向状态::已计算||r.版本!=1||!r.G||!r.请求身份
            ||!有效(r.任务.值)||!有效(r.当前F)||r.要求结果位<1||r.要求结果位>7||!r.预算.有效()
            ||G!=r.G||任务!=r.任务||!有效(所属存在)||!有效(来源需求.值)
            ||来源需求所属存在!=所属存在||!方向||!方向->成功()
            ||方向->G!=G||方向->需求!=来源需求||方向->当前F!=r.当前F)return false;
        return 方向->原请求==需求目标方向请求{1,r.G,r.请求身份,来源需求,r.当前F,r.要求结果位,r.预算};
    }
};
class 任务目标方向应用服务 final {
    const L1事实基座服务& l1_;
    const 任务类数据服务& task_;
    const 需求类数据服务& demand_;
    const 需求目标方向应用服务& direction_;
    using S=任务目标方向状态;
    struct 失败{S 状态;};
    static void 要求(bool ok,S state=S::内部不一致){if(!ok)throw 失败{state};}
    static S 映射(任务类数据状态 state) noexcept {
        using T=任务类数据状态;
        switch(state){
        case T::入口拒绝:return S::入口拒绝;case T::未找到:return S::任务未找到;
        case T::所属存在未找到:case T::所属存在已退出:case T::来源需求不属于存在:return S::来源不匹配;
        case T::来源需求未找到:return S::来源需求未找到;case T::来源需求不可用:return S::来源需求已退出;
        case T::数量预算不足:return S::预算不足;case T::事实代次漂移:return S::事实代次漂移;
        case T::资源失败:return S::资源失败;default:return S::内部不一致;
        }
    }
    void 守卫(std::uint64_t g) const {
        const auto read=l1_.读取中性当前事实代次({L1中性CRUD合同版本});
        要求(read.状态==L1中性读取状态::成功,read.状态==L1中性读取状态::资源失败 ? S::资源失败:S::内部不一致);
        要求(read.合同版本==L1中性CRUD合同版本&&read.事实代次);
        要求(read.事实代次==g,S::事实代次漂移);
    }
public:
    任务目标方向应用服务(const L1事实基座服务& l1,const 任务类数据服务& task,
        const 需求类数据服务& demand,const 需求目标方向应用服务& direction)
        :l1_(l1),task_(task),demand_(demand),direction_(direction){
        if(!绑定于(l1))throw std::invalid_argument("任务方向数据绑定");
    }
    任务目标方向应用服务()=delete;
    任务目标方向应用服务(const 任务目标方向应用服务&)=delete;
    任务目标方向应用服务& operator=(const 任务目标方向应用服务&)=delete;
    任务目标方向应用服务(任务目标方向应用服务&&)=delete;
    任务目标方向应用服务& operator=(任务目标方向应用服务&&)=delete;
    bool 绑定于(const L1事实基座服务& l1) const noexcept {
        return &l1==&l1_&&task_.绑定于(l1)&&demand_.绑定于(l1)&&direction_.绑定于(l1);
    }
    // 任务层只核验来源并委托需求方向，不写任务、不复制比较核心。
    任务目标方向结果 读取并计算(const 任务目标方向请求& r) const noexcept {
        任务目标方向结果 out;out.原请求=r;out.G=r.G;out.任务=r.任务;
        try {
            要求(r.版本==1&&r.G&&r.请求身份&&有效(r.任务.值)&&有效(r.当前F)
                &&r.要求结果位>=1&&r.要求结果位<=7&&r.预算.有效(),S::入口拒绝);
            守卫(r.G);
            const auto task=task_.查询任务记录({任务类数据合同版本,r.G,r.任务,r.预算.最大读取材料数});
            if(task.状态!=任务类数据状态::已读取)throw 失败{映射(task.状态)};
            要求(task.成功()&&task.事实代次==r.G&&task.记录&&task.记录->身份==r.任务);
            out.所属存在=task.记录->所属存在;out.来源需求=task.记录->来源需求;
            const auto demand=demand_.查询需求记录({需求类数据合同版本,r.G,out.来源需求});
            if(demand.状态!=需求类数据状态::已读取){
                if(demand.状态==需求类数据状态::未找到)throw 失败{S::来源需求未找到};
                throw 失败{S::内部不一致};
            }
            要求(demand.成功()&&demand.事实代次==r.G&&demand.记录);
            out.来源需求所属存在=demand.记录->所属存在;
            要求(demand.记录->身份==out.来源需求&&out.来源需求所属存在==out.所属存在,S::来源不匹配);
            out.方向=direction_.读取并计算({1,r.G,r.请求身份,out.来源需求,r.当前F,r.要求结果位,r.预算});
            if(!out.方向->成功()){
                switch(out.方向->状态){
                case 需求目标方向状态::事实代次漂移:throw 失败{S::事实代次漂移};
                case 需求目标方向状态::预算不足:throw 失败{S::预算不足};
                case 需求目标方向状态::资源失败:throw 失败{S::资源失败};
                default:throw 失败{S::需求方向失败};
                }
            }
            守卫(r.G);out.状态=S::已计算;要求(out.成功());
        }catch(const 失败& e){out.状态=e.状态;}
        catch(const std::bad_alloc&){out.状态=S::资源失败;}
        catch(const std::length_error&){out.状态=S::资源失败;}
        catch(...){out.状态=S::内部不一致;}
        return out;
    }
};
}
