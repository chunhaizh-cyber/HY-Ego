module;

#include <algorithm>
#include <array>
#include <cstdint>
#include <optional>
#include <vector>

export module 海中鱼巣.领域.合同.场景角色组织;

export import 海中鱼巣.领域.合同.场景状态使用绑定;
export import 海中鱼巣.领域.合同.动态结构;

export namespace 海中鱼巣 {

inline constexpr std::uint32_t 场景角色组织合同版本 = 2;

enum class 场景根角色 : std::uint8_t { 特征=1, 状态=2, 动态=3, 关系=4 };

enum class 场景角色数据状态 : std::uint8_t {
    已登记=1, 已启用=2, 已组织=3, 已读取=4, 已退出=5, 精确重复=6,
    入口拒绝=7, 未找到=8, 目标已退出=9, 场景角色未启用=10,
    场景角色已存在=11, 实例未组织=12, 实例已组织=13,
    引用冲突=14, 事实代次漂移=15, 幂等冲突=16, 数量预算不足=17,
    历史材料已清理=18, 资源失败=20, 内部不一致=21, 已可能发布=22
};

struct 场景事实生命周期 final { std::uint64_t 创建事实代次=0; std::optional<std::uint64_t> 退出事实代次; };
struct 场景节点见证 final { 稳定编码 编码; 场景事实生命周期 生命周期; };
struct 场景组织边见证 final { 稳定编码 编码,源,目标,关系类型; std::uint64_t 角色或顺序=0; 场景事实生命周期 生命周期; };
struct 场景根事实 final { 场景根角色 角色=场景根角色::特征; 场景节点见证 根; 场景组织边见证 绑定; };
struct 场景角色历史事实 final {
    std::uint64_t Gread=0,H=0; 稳定编码 场景; 存在身份来源历史见证 对象存在来源;
    场景节点见证 场景族锚点,场景族归属类型,根绑定类型; 场景组织边见证 场景角色登记边; std::array<场景根事实,4> 四根;
};

struct 场景当前身份请求 final { std::uint32_t 版本=2; std::uint64_t G0=0; 稳定编码 场景; };
struct 场景历史身份请求 final { std::uint32_t 版本=2; std::uint64_t Gread=0,H=0; 稳定编码 场景; };
struct 场景当前身份结果 final { std::uint32_t 版本=2; 场景角色数据状态 状态=场景角色数据状态::入口拒绝; std::uint64_t Gread=0; bool 成功(const 场景当前身份请求&) const noexcept; };
struct 场景角色历史结果 final { std::uint32_t 版本=2; 场景角色数据状态 状态=场景角色数据状态::入口拒绝; std::uint64_t Gread=0,H=0; std::optional<场景角色历史事实> 角色; bool 成功(const 场景历史身份请求&) const noexcept; };
struct 场景组织历史请求 final { std::uint32_t 版本=2; std::uint64_t Gread=0,H=0; 场景根角色 角色=场景根角色::状态; 稳定编码 实例; std::uint64_t 最大路径长度=0; };
struct 场景实例组织事实 final { std::uint64_t Gread=0,H=0; 场景角色历史事实 场景角色; 场景根角色 角色=场景根角色::状态; 稳定编码 根,实例; std::vector<场景节点见证> 路径节点; std::vector<场景组织边见证> 路径边; };
struct 场景组织历史结果 final { std::uint32_t 版本=2; 场景角色数据状态 状态=场景角色数据状态::入口拒绝; std::uint64_t Gread=0,H=0; std::optional<场景实例组织事实> 组织; bool 成功(const 场景组织历史请求&) const noexcept; };
struct 场景动态组织历史请求 final { std::uint32_t 版本=2; std::uint64_t Gread=0,H=0; 动态信息身份 动态; std::uint64_t 最大路径长度=0; };
using 场景动态组织事实=场景实例组织事实;
struct 场景动态组织历史结果 final { std::uint32_t 版本=2; 场景角色数据状态 状态=场景角色数据状态::入口拒绝; std::uint64_t Gread=0,H=0; std::optional<场景动态组织事实> 组织; bool 成功(const 场景动态组织历史请求&) const noexcept; };

class 场景动态组织只读提供者 { public: virtual ~场景动态组织只读提供者()=default; virtual bool 绑定于(const L1事实基座服务&) const noexcept=0; virtual 场景当前身份结果 确认当前场景角色(const 场景当前身份请求&) const=0; virtual 场景角色历史结果 读取场景角色历史(const 场景历史身份请求&) const=0; virtual 场景动态组织历史结果 读取动态场景组织历史(const 场景动态组织历史请求&) const=0; };

struct 场景角色结构交付 final { 状态使用绑定结构交付 绑定结构; 稳定编码 根绑定关系类型,状态组织关系类型,动态组织关系类型; };
struct 场景结构登记请求 final { std::uint32_t 版本=2; std::uint64_t G0=0; L1所有者范围写入幂等身份 幂等身份; };
struct 场景结构登记结果 final { std::uint32_t 版本=2; 场景角色数据状态 状态=场景角色数据状态::入口拒绝; std::uint64_t Gread=0,首次发布代次=0; std::optional<场景角色结构交付> 交付; bool 成功(const 场景结构登记请求&) const noexcept; };
struct 场景角色启用请求 final { std::uint32_t 版本=2; std::uint64_t G0=0; L1所有者范围写入幂等身份 幂等身份; 稳定编码 对象存在; };
struct 场景角色退出请求 final { std::uint32_t 版本=2; std::uint64_t G0=0; L1所有者范围写入幂等身份 幂等身份; 稳定编码 场景; };
struct 场景角色写结果 final { std::uint32_t 版本=2; 场景角色数据状态 状态=场景角色数据状态::入口拒绝; std::uint64_t Gread=0,首次发布代次=0; std::optional<场景角色历史事实> 角色; bool 启用成功(const 场景角色启用请求&) const noexcept; bool 退出成功(const 场景角色退出请求&) const noexcept; };
struct 场景状态组织请求 final { std::uint32_t 版本=2; std::uint64_t G0=0; L1所有者范围写入幂等身份 幂等身份; 稳定编码 场景,组织父; 状态信息身份 状态; std::uint64_t 最大路径长度=0; };
struct 场景动态组织请求 final { std::uint32_t 版本=2; std::uint64_t G0=0; L1所有者范围写入幂等身份 幂等身份; 稳定编码 场景,组织父; 动态信息身份 动态; std::uint64_t 最大路径长度=0; };
struct 场景实例组织回执_v2 final { std::uint64_t Gread=0,H=0; 稳定编码 场景,根,实例; 场景根角色 角色=场景根角色::状态; 场景组织边见证 直接组织边; };
struct 场景组织写结果_v2 final { std::uint32_t 版本=2; 场景角色数据状态 状态=场景角色数据状态::入口拒绝; std::uint64_t Gread=0,首次发布代次=0; std::optional<场景实例组织回执_v2> 组织; bool 状态组织成功(const 场景状态组织请求&) const noexcept; bool 动态组织成功(const 场景动态组织请求&) const noexcept; };

inline constexpr std::uint32_t 场景特征组织合同版本=1;
inline constexpr L1所有者范围写入幂等身份 场景特征组织扩展登记固定幂等身份{
    0x5343'454E'4546'4F52ULL
};

struct 场景特征组织扩展结构交付 final { 稳定编码 特征组织关系类型; };
struct 场景特征组织扩展登记请求 final { std::uint32_t 版本=1; std::uint64_t G0=0; };
struct 场景特征组织扩展登记结果 final {
    std::uint32_t 版本=1; 场景角色数据状态 状态=场景角色数据状态::入口拒绝;
    std::uint64_t Gread=0,首次发布代次=0;
    std::optional<场景特征组织扩展结构交付> 交付;
    bool 成功(const 场景特征组织扩展登记请求&) const noexcept;
};

struct 场景特征组织请求 final {
    std::uint32_t 版本=1; std::uint64_t G0=0; L1所有者范围写入幂等身份 幂等身份;
    稳定编码 场景,组织父; 特征信息身份 特征; std::uint64_t 最大路径长度=0;
};
struct 场景特征组织历史请求 final {
    std::uint32_t 版本=1; std::uint64_t Gread=0,H=0; 特征信息身份 特征;
    std::uint64_t 最大路径长度=0;
};
using 场景特征组织事实=场景实例组织事实;
struct 场景特征组织历史结果 final {
    std::uint32_t 版本=1; 场景角色数据状态 状态=场景角色数据状态::入口拒绝;
    std::uint64_t Gread=0,H=0; std::optional<场景特征组织事实> 组织;
    bool 成功(const 场景特征组织历史请求&) const noexcept;
};
struct 场景特征组织写结果_v1 final {
    std::uint32_t 版本=1; 场景角色数据状态 状态=场景角色数据状态::入口拒绝;
    std::uint64_t Gread=0,首次发布代次=0; std::optional<场景实例组织回执_v2> 组织;
    bool 成功(const 场景特征组织请求&) const noexcept;
};

static_assert(场景角色组织合同版本==2);
static_assert(static_cast<std::uint8_t>(场景根角色::特征)==1);
static_assert(static_cast<std::uint8_t>(场景根角色::状态)==2);
static_assert(static_cast<std::uint8_t>(场景根角色::动态)==3);
static_assert(static_cast<std::uint8_t>(场景根角色::关系)==4);
static_assert(static_cast<std::uint8_t>(场景角色数据状态::已登记)==1);
static_assert(static_cast<std::uint8_t>(场景角色数据状态::已可能发布)==22);

} // namespace 海中鱼巣

namespace 场景合同细节 {
using namespace 海中鱼巣;
inline bool 合法角色(场景根角色 v) noexcept { return v==场景根角色::特征||v==场景根角色::状态||v==场景根角色::动态||v==场景根角色::关系; }
inline bool 生命周期完整(const 场景事实生命周期& v,std::uint64_t h) noexcept {
    return v.创建事实代次&&v.创建事实代次<=h&&(!v.退出事实代次||*v.退出事实代次>h);
}
inline bool 角色完整(const 场景角色历史事实& v) noexcept {
    if(!v.Gread||!v.H||v.H>v.Gread||!有效(v.场景)||!存在身份来源历史见证完整(v.对象存在来源,v.H,v.场景))return false;
    const 场景节点见证* meta[]{&v.场景族锚点,&v.场景族归属类型,&v.根绑定类型};
    const 稳定编码 m[]{meta[0]->编码,meta[1]->编码,meta[2]->编码};
    for(std::size_t i=0;i<3;++i){if(!有效(m[i])||!生命周期完整(meta[i]->生命周期,v.H))return false;for(std::size_t j=0;j<i;++j)if(m[i]==m[j])return false;}
    if(!有效(v.场景角色登记边.编码)||v.场景角色登记边.源!=v.场景||v.场景角色登记边.目标!=m[0]||v.场景角色登记边.关系类型!=m[1]||v.场景角色登记边.角色或顺序!=1||v.场景角色登记边.生命周期.退出事实代次||!生命周期完整(v.场景角色登记边.生命周期,v.H))return false;
    const auto gc=v.场景角色登记边.生命周期.创建事实代次;
    if(v.对象存在来源.节点生命周期.创建事实代次>gc)return false;
    std::optional<std::uint64_t> commonExit;
    bool exitSet=false;
    std::array<稳定编码,8> own{};
    for(std::size_t i=0;i<4;++i){const auto& r=v.四根[i];
        if(static_cast<unsigned>(r.角色)!=i+1||!有效(r.根.编码)||!有效(r.绑定.编码)||r.绑定.源!=v.场景||r.绑定.目标!=r.根.编码||r.绑定.关系类型!=m[2]||r.绑定.角色或顺序!=i+1)return false;
        if(r.根.生命周期.创建事实代次!=gc||r.绑定.生命周期.创建事实代次!=gc||r.根.生命周期.退出事实代次!=r.绑定.生命周期.退出事实代次)return false;
        if(!exitSet){commonExit=r.根.生命周期.退出事实代次;exitSet=true;}else if(commonExit!=r.根.生命周期.退出事实代次)return false;
        if(gc>v.H||commonExit&&*commonExit<=v.H)return false;
        own[i*2]=r.根.编码;own[i*2+1]=r.绑定.编码;
    }
    for(std::size_t i=0;i<own.size();++i){if(own[i]==v.场景||own[i]==m[0]||own[i]==m[1]||own[i]==m[2]||own[i]==v.场景角色登记边.编码)return false;for(std::size_t j=0;j<i;++j)if(own[i]==own[j])return false;}
    if(v.场景角色登记边.编码==v.场景||v.场景角色登记边.编码==m[0]||v.场景角色登记边.编码==m[1]||v.场景角色登记边.编码==m[2])return false;
    return true;
}
inline bool 路径完整(const 场景实例组织事实& v,std::uint64_t 预算) noexcept {
    if(!角色完整(v.场景角色)||!v.Gread||v.Gread!=v.场景角色.Gread||v.H!=v.场景角色.H||!合法角色(v.角色)||!有效(v.根)||!有效(v.实例)||v.根==v.实例||v.路径边.empty()||v.路径边.size()>预算||v.路径节点.size()!=v.路径边.size()+1||v.路径节点.front().编码!=v.根||v.路径节点.back().编码!=v.实例)return false;
    const auto roleIndex=static_cast<std::size_t>(static_cast<unsigned>(v.角色)-1);
    if(roleIndex>=v.场景角色.四根.size()||v.场景角色.四根[roleIndex].根.编码!=v.根)return false;
    std::optional<稳定编码> type;
    for(std::size_t i=0;i<v.路径节点.size();++i){const auto& n=v.路径节点[i];if(!有效(n.编码)||!生命周期完整(n.生命周期,v.H))return false;for(std::size_t j=0;j<i;++j)if(v.路径节点[j].编码==n.编码)return false;}
    for(std::size_t i=0;i<v.路径边.size();++i){const auto& e=v.路径边[i];if(!有效(e.编码)||!有效(e.关系类型)||e.源!=v.路径节点[i].编码||e.目标!=v.路径节点[i+1].编码||e.角色或顺序!=1||!生命周期完整(e.生命周期,v.H))return false;if(!type)type=e.关系类型;else if(*type!=e.关系类型)return false;for(const auto&n:v.路径节点)if(n.编码==e.编码)return false;for(std::size_t j=0;j<i;++j)if(v.路径边[j].编码==e.编码)return false;}
    return true;
}
}

export namespace 海中鱼巣 {
inline bool 场景当前身份结果::成功(const 场景当前身份请求& r) const noexcept{return r.版本==2&&r.G0&&有效(r.场景)&&版本==2&&状态==场景角色数据状态::已读取&&Gread==r.G0;}
inline bool 场景角色历史结果::成功(const 场景历史身份请求& r) const noexcept{return r.版本==2&&r.Gread&&r.H&&r.H<=r.Gread&&有效(r.场景)&&版本==2&&状态==场景角色数据状态::已读取&&Gread==r.Gread&&H==r.H&&角色&&角色->Gread==Gread&&角色->H==H&&角色->场景==r.场景;}
inline bool 场景组织历史结果::成功(const 场景组织历史请求&r)const noexcept{if(r.版本!=2||!r.Gread||!r.H||r.H>r.Gread||(r.角色!=场景根角色::状态&&r.角色!=场景根角色::动态)||!有效(r.实例)||r.最大路径长度<1||r.最大路径长度>4096||版本!=2||状态!=场景角色数据状态::已读取||Gread!=r.Gread||H!=r.H||!组织||组织->Gread!=Gread||组织->H!=H||!有效(组织->场景角色.场景)||组织->场景角色.Gread!=Gread||组织->场景角色.H!=H||组织->角色!=r.角色||组织->实例!=r.实例||!有效(组织->根)||组织->路径边.empty()||组织->路径边.size()>r.最大路径长度||组织->路径节点.size()!=组织->路径边.size()+1||组织->路径节点.front().编码!=组织->根||组织->路径节点.back().编码!=r.实例)return false;const auto i=static_cast<std::size_t>(static_cast<unsigned>(r.角色)-1);return i<组织->场景角色.四根.size()&&组织->场景角色.四根[i].根.编码==组织->根;}
inline bool 场景动态组织历史结果::成功(const 场景动态组织历史请求&r)const noexcept{return r.版本==2&&r.Gread&&r.H&&r.H<=r.Gread&&有效(r.动态)&&r.最大路径长度>=1&&r.最大路径长度<=4096&&版本==2&&状态==场景角色数据状态::已读取&&Gread==r.Gread&&H==r.H&&组织&&组织->Gread==Gread&&组织->H==H&&组织->场景角色.Gread==Gread&&组织->场景角色.H==H&&有效(组织->场景角色.场景)&&组织->角色==场景根角色::动态&&组织->实例==r.动态.编码&&有效(组织->根)&&!组织->路径边.empty()&&组织->路径边.size()<=r.最大路径长度&&组织->路径节点.size()==组织->路径边.size()+1&&组织->路径节点.front().编码==组织->根&&组织->路径节点.back().编码==r.动态.编码&&组织->场景角色.四根[2].根.编码==组织->根;}
inline bool 场景结构登记结果::成功(const 场景结构登记请求&r)const noexcept{if(r.版本!=2||!r.G0||r.G0==UINT64_MAX||r.幂等身份.值!=1||版本!=2||(状态!=场景角色数据状态::已登记&&状态!=场景角色数据状态::精确重复)||首次发布代次!=r.G0+1||Gread<首次发布代次||!交付)return false;const 稳定编码 ids[]{交付->绑定结构.场景族锚点,交付->绑定结构.场景族归属关系类型,交付->绑定结构.状态使用绑定成员关系类型,交付->根绑定关系类型,交付->状态组织关系类型,交付->动态组织关系类型};for(std::size_t i=0;i<6;++i){if(!有效(ids[i]))return false;for(std::size_t j=0;j<i;++j)if(ids[i]==ids[j])return false;}return true;}
inline bool 场景角色写结果::启用成功(const 场景角色启用请求&r)const noexcept{return r.版本==2&&r.G0&&r.G0!=UINT64_MAX&&r.幂等身份.值>1&&有效(r.对象存在)&&版本==2&&(状态==场景角色数据状态::已启用||状态==场景角色数据状态::精确重复)&&首次发布代次==r.G0+1&&Gread>=首次发布代次&&角色&&角色->Gread==Gread&&角色->H==首次发布代次&&角色->场景==r.对象存在;}
inline bool 场景角色写结果::退出成功(const 场景角色退出请求&r)const noexcept{return r.版本==2&&r.G0&&r.G0!=UINT64_MAX&&r.幂等身份.值>1&&有效(r.场景)&&版本==2&&(状态==场景角色数据状态::已退出||状态==场景角色数据状态::精确重复)&&首次发布代次==r.G0+1&&Gread>=首次发布代次&&角色&&角色->Gread==Gread&&角色->H==首次发布代次-1&&角色->场景==r.场景;}
inline bool 场景组织写结果_v2::状态组织成功(const 场景状态组织请求&r)const noexcept{return r.版本==2&&r.G0&&r.G0!=UINT64_MAX&&r.幂等身份.值>1&&有效(r.场景)&&有效(r.组织父)&&有效(r.状态)&&r.最大路径长度>=1&&r.最大路径长度<=4096&&版本==2&&(状态==场景角色数据状态::已组织||状态==场景角色数据状态::精确重复)&&首次发布代次==r.G0+1&&Gread>=首次发布代次&&组织&&组织->Gread==Gread&&组织->H==首次发布代次&&组织->场景==r.场景&&组织->角色==场景根角色::状态&&组织->实例==r.状态.编码&&有效(组织->根)&&有效(组织->直接组织边.编码)&&有效(组织->直接组织边.关系类型)&&组织->直接组织边.源==r.组织父&&组织->直接组织边.目标==r.状态.编码&&组织->直接组织边.角色或顺序==1&&组织->直接组织边.生命周期.创建事实代次==首次发布代次&&!组织->直接组织边.生命周期.退出事实代次;}
inline bool 场景组织写结果_v2::动态组织成功(const 场景动态组织请求&r)const noexcept{return r.版本==2&&r.G0&&r.G0!=UINT64_MAX&&r.幂等身份.值>1&&有效(r.场景)&&有效(r.组织父)&&有效(r.动态)&&r.最大路径长度>=1&&r.最大路径长度<=4096&&版本==2&&(状态==场景角色数据状态::已组织||状态==场景角色数据状态::精确重复)&&首次发布代次==r.G0+1&&Gread>=首次发布代次&&组织&&组织->Gread==Gread&&组织->H==首次发布代次&&组织->场景==r.场景&&组织->角色==场景根角色::动态&&组织->实例==r.动态.编码&&有效(组织->根)&&有效(组织->直接组织边.编码)&&有效(组织->直接组织边.关系类型)&&组织->直接组织边.源==r.组织父&&组织->直接组织边.目标==r.动态.编码&&组织->直接组织边.角色或顺序==1&&组织->直接组织边.生命周期.创建事实代次==首次发布代次&&!组织->直接组织边.生命周期.退出事实代次;}
inline bool 场景特征组织扩展登记结果::成功(const 场景特征组织扩展登记请求&r)const noexcept{return r.版本==1&&r.G0&&r.G0!=UINT64_MAX&&版本==1&&(状态==场景角色数据状态::已登记||状态==场景角色数据状态::精确重复)&&首次发布代次==r.G0+1&&Gread>=首次发布代次&&交付&&有效(交付->特征组织关系类型);}
inline bool 场景特征组织历史结果::成功(const 场景特征组织历史请求&r)const noexcept{return r.版本==1&&r.Gread&&r.H&&r.H<=r.Gread&&有效(r.特征)&&r.最大路径长度>=1&&r.最大路径长度<=4096&&版本==1&&状态==场景角色数据状态::已读取&&Gread==r.Gread&&H==r.H&&组织&&组织->Gread==Gread&&组织->H==H&&组织->角色==场景根角色::特征&&组织->实例==r.特征.编码&&场景合同细节::路径完整(*组织,r.最大路径长度);}
inline bool 场景特征组织写结果_v1::成功(const 场景特征组织请求&r)const noexcept{return r.版本==1&&r.G0&&r.G0!=UINT64_MAX&&r.幂等身份.值>1&&r.幂等身份!=场景特征组织扩展登记固定幂等身份&&有效(r.场景)&&有效(r.组织父)&&有效(r.特征)&&r.最大路径长度>=1&&r.最大路径长度<=4096&&版本==1&&(状态==场景角色数据状态::已组织||状态==场景角色数据状态::精确重复)&&首次发布代次==r.G0+1&&Gread>=首次发布代次&&组织&&组织->Gread==Gread&&组织->H==首次发布代次&&组织->场景==r.场景&&组织->角色==场景根角色::特征&&组织->实例==r.特征.编码&&有效(组织->根)&&组织->直接组织边.源==r.组织父&&组织->直接组织边.目标==r.特征.编码&&有效(组织->直接组织边.编码)&&有效(组织->直接组织边.关系类型)&&组织->直接组织边.角色或顺序==1&&组织->直接组织边.生命周期.创建事实代次==首次发布代次&&!组织->直接组织边.生命周期.退出事实代次;}
}
