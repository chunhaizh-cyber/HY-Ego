#include "方法问题归档.模拟世界.h"
#include <bit>
#include <limits>
#include <stdexcept>
#include <tuple>
#include <type_traits>

namespace 海中鱼巣::方法问题归档内部 {
namespace {
// 格式1逐字段编码，不复制C++布局。所有长度/判别字和整数各占一个U64字；
// 有符号数保存I64位模式，字符串逐UTF8字节保存，不依赖主机字节序。
template<格式 F, class T> auto 成员(T& x) {
    using U = std::remove_cv_t<T>;
    if constexpr (std::is_same_v<U, 稳定编码>) return std::tie(x.值);
    else if constexpr (std::is_same_v<U, 特征概念I64闭区间>) return std::tie(x.下界,x.上界);
    else if constexpr (std::is_same_v<U, 特征概念全材料值域>) return std::tuple{};
    else if constexpr (std::is_same_v<U, 特征概念I64分量定义>) return std::tie(x.角色,x.单位,x.值域);
    else if constexpr (std::is_same_v<U, 特征概念结构化I64值域>) return std::tie(x.分量);
    else if constexpr (std::is_same_v<U, 特征概念存在引用值域>) return std::tie(x.存在概念节点);
    else if constexpr (std::is_same_v<U, 特征概念任务集合值域>) return std::tie(x.必含任务节点组,
        x.至少一个任务,x.必含需求任务组,x.必含服务要求任务组);
    else if constexpr (std::is_same_v<U, 新方法参数特征规格>) return std::tie(x.特征类型概念节点,x.值域概念节点,x.二次特征概念组);
    else if constexpr (std::is_same_v<U, 新方法参数集合成员约束>) return std::tie(x.集合参数角色节点,x.集合特征类型概念节点);
    else if constexpr (std::is_same_v<U, 新方法有序参数规格>) return std::tie(x.序号,x.信息类型,x.范围概念节点);
    else if constexpr (std::is_same_v<U, 新方法有序参数引用>) return std::tie(x.序号,x.信息类型,x.信息节点);
    else if constexpr (std::is_same_v<U, 新方法参数规格>) {
        if constexpr (F == 格式::有序参数任务执行组织) return std::tie(x.参数角色节点,x.存在概念节点,
            x.要求场景,x.特征规格组,x.对象种类,x.集合成员约束,x.有序规格);
        else return std::tie(x.参数角色节点,x.存在概念节点,
            x.要求场景,x.特征规格组,x.对象种类,x.集合成员约束);
    }
    else if constexpr (std::is_same_v<U, 新方法实际特征绑定>) return std::tie(x.特征节点);
    else if constexpr (std::is_same_v<U, 新服务要求分项引用>) return std::tie(x.理解节点,x.分项);
    else if constexpr (std::is_same_v<U, 新服务要求确认引用>) return std::tie(x.确认节点);
    else if constexpr (std::is_same_v<U, 新方法参数角色绑定>) {
        if constexpr (F == 格式::有序参数任务执行组织) return std::tie(x.参数角色节点,x.世界场景节点,
            x.实际存在节点,x.实际特征组,x.任务虚拟存在引用,x.服务要求分项引用,x.服务要求确认引用,x.有序引用);
        else return std::tie(x.参数角色节点,x.世界场景节点,
            x.实际存在节点,x.实际特征组,x.任务虚拟存在引用,x.服务要求分项引用,x.服务要求确认引用);
    }
    else if constexpr (std::is_same_v<U, 新任务本能采用定位>) return std::tie(x.组织节点,x.方法作用域节点,x.配对要求节点);
    else if constexpr (std::is_same_v<U, 新任务本能采用材料>) return std::tie(x.定位,x.来源方法节点,x.来源配对节点,
        x.来源条件节点,x.来源结果节点,x.参数要求组,x.原始条件,x.需要条件,x.结果,x.线性关系组,x.参数绑定组);
    else if constexpr (std::is_same_v<U, 新需求目标定义>) return std::tie(x.目标特征类型概念节点,x.目标形式,
        x.目标值域概念节点,x.目标比较关系概念节点,x.目标变化方向);
    else if constexpr (std::is_same_v<U, 新服务要求目标分项>) return std::tie(x.目标形式,
        x.目标值域概念节点,x.目标比较关系概念节点,x.目标变化方向);
    else if constexpr (std::is_same_v<U, 新服务要求分项材料>) return std::tie(x.引用,x.值);
    else if constexpr (std::is_same_v<U, 新服务要求确认目标>) return std::tie(x.确认节点,x.理解节点,
        x.来源人类节点,x.目标存在节点,x.目标);
    else if constexpr (std::is_same_v<U, 新服务需求信息包>) return std::tie(x.来源人类节点,x.目标存在节点,
        x.目标,x.分项来源,x.确认);
    else if constexpr (std::is_same_v<U, 新服务对象窗口来源>) return std::tie(x.窗口存在节点,x.所在场景节点,
        x.实际存在节点,x.公开会话标识,x.已接纳消息序号);
    else if constexpr (std::is_same_v<U, 新特征匹配定义快照>) return std::tie(x.概念节点,x.特征类型根,
        x.材料类型,x.单位,x.比较规则,x.值域);
    else if constexpr (std::is_same_v<U, 新特征匹配实例快照>) return std::tie(x.特征节点,x.上级特征节点,
        x.定义,x.实例单位,x.当前值,x.当前完整材料,x.实例区间值);
    else if constexpr (std::is_same_v<U, 新存在概念必需特征快照>) return std::tie(x.来源,x.定义);
    else if constexpr (std::is_same_v<U, 新存在概念匹配结果>) return std::tie(x.状态,x.缺少特征类型组,x.不符合特征类型组);
    else if constexpr (std::is_same_v<U, 新存在概念匹配材料>) return std::tie(x.存在节点,x.专属存在概念节点,
        x.目标存在概念节点,x.必需特征组,x.实际特征组);
    else if constexpr (std::is_same_v<U, 新存在概念匹配采集结果>) return std::tie(x.匹配,x.材料,
        x.尚缺固定比较依据特征组,x.采集期间材料变化);
    else if constexpr (std::is_same_v<U, 新服务对象材料>) return std::tie(x.实际存在节点,x.人类概念节点,
        x.专属存在概念节点,x.属于人类,x.概念匹配状态,x.服务对象节点,x.来源,x.概念匹配采集,x.实际特征快照组);
    else if constexpr (std::is_same_v<U, 新特征值命中信息>) return std::tie(x.值,x.命中次数);
    else if constexpr (std::is_same_v<U, 新特征信息>) return std::tie(x.节点,x.上级特征节点,x.特征概念节点,
        x.实例区间值,x.实例历史聚合值域,x.实例单位,x.当前值,x.标准值,x.历史不同值,x.历史值命中信息);
    else if constexpr (std::is_same_v<U, 特征概念信息>) return std::tie(x.节点,x.材料类型,x.值域,x.单位,
        x.比较规则,x.聚合规则,x.名称关系,x.当前引用实例数量);
    else if constexpr (std::is_same_v<U, 存在概念特征项>) return std::tie(x.特征类型概念,x.具体特征概念);
    else if constexpr (std::is_same_v<U, 存在概念信息>) return std::tie(x.节点,x.种类,x.对应存在节点,
        x.特征组,x.名称关系,x.上位概念组,x.下位概念组);
    else if constexpr (std::is_same_v<U, 新特征值信息>) return std::tie(x.节点,x.物理类型,x.材料);
    else if constexpr (std::is_same_v<U, 新场景信息>) return std::tie(x.节点,x.父场景节点,x.专属存在概念节点,
        x.特征节点组,x.直接成员存在组,x.边界存在组,x.直接子场景组,x.状态节点组,x.动态节点组);
    else if constexpr (std::is_same_v<U, 新存在信息>) return std::tie(x.节点,x.父存在节点,
        x.专属存在概念节点,x.特征节点组,x.子存在节点组);
    else if constexpr (std::is_same_v<U, 新状态强时间>) return std::tie(x.语义,x.纳秒);
    else if constexpr (std::is_same_v<U, 新状态信息>) return std::tie(x.节点,x.被描述存在节点,
        x.来源实例特征节点,x.特征类型概念节点,x.固定准确值,x.强时间,x.被引用次数);
    else if constexpr (std::is_same_v<U, 新任务虚拟存在引用>) return std::tie(x.任务节点,x.虚拟存在节点,x.状态特征节点);
    else if constexpr (std::is_same_v<U, 新动作结果关联>) return std::tie(x.类型,x.结果动态节点);
    else if constexpr (std::is_same_v<U, 新动态跨特征时间项>) return std::tie(x.前状态节点,x.后状态节点,
        x.前特征类型概念节点,x.后特征类型概念节点,x.二次特征类型概念节点,x.结果值域概念节点,x.时间差纳秒);
    else if constexpr (std::is_same_v<U, 新动态信息>) return std::tie(x.节点,x.种类,x.主题存在节点,
        x.状态节点组,x.成员动态节点组,x.跨特征时间组,x.动作定义节点,x.动作来源节点,
        x.动作目标存在节点,x.动作目标特征节点,x.动作目标种类,x.动作目标任务虚拟存在,
        x.动作时间,x.动作结果组,x.动态概念节点,x.被引用次数);
    else if constexpr (std::is_same_v<U, 新方法条件项定义>) {
        if constexpr (F == 格式::有序参数任务执行组织) return std::tie(x.参数角色节点,x.存在概念节点,
            x.特征类型概念节点,x.特征值域概念节点,x.二次特征概念节点,x.状态或动态概念节点,x.精确数值域,x.参数序号);
        else return std::tie(x.参数角色节点,x.存在概念节点,
            x.特征类型概念节点,x.特征值域概念节点,x.二次特征概念节点,x.状态或动态概念节点,x.精确数值域);
    }
    else if constexpr (std::is_same_v<U, 新方法结果项定义>) {
        if constexpr (F == 格式::有序参数任务执行组织) return std::tie(x.参数角色节点,x.特征类型概念节点,
            x.特征值域概念节点,x.变化方向概念节点,x.状态或动态概念节点,x.主要结果,x.精确数值域,x.参数序号);
        else return std::tie(x.参数角色节点,x.特征类型概念节点,
            x.特征值域概念节点,x.变化方向概念节点,x.状态或动态概念节点,x.主要结果,x.精确数值域);
    }
    else if constexpr (std::is_same_v<U, 新方法结果确认要求>) return std::tie(x.必要证据质量,x.最大等待纳秒,
        x.等待条件概念节点,x.多证据关系,x.禁止结果关系规则,x.禁止结果项组);
    else if constexpr (std::is_same_v<U, 新方法条件定义>) return std::tie(x.关系规则,x.条件项组);
    else if constexpr (std::is_same_v<U, 新方法结果定义>) return std::tie(x.关系规则,x.结果项组,x.确认要求);
    else if constexpr (std::is_same_v<U, 新方法条件结果线性关系>) return std::tie(x.条件序号,x.结果序号,
        x.方向,x.原条件域,x.原结果域);
    // 格式1的嵌套布局保持七字段，配对来源仅在问题头末尾作带标签扩展。
    else if constexpr (std::is_same_v<U, 新方法条件结果信息>) return std::tie(x.配对节点,x.条件节点,
        x.结果节点,x.条件,x.结果,x.命中次数,x.线性关系组);
    else if constexpr (std::is_same_v<U, 新本能函数条件特征材料>) return std::tie(x.特征节点,x.动作前准确值);
    else if constexpr (std::is_same_v<U, 新本能函数条件角色材料>) {
        if constexpr (F == 格式::旧方法副本) return std::tie(x.参数角色节点,x.世界场景节点,
            x.实际存在节点,x.实际特征组,x.任务虚拟存在引用);
        else if constexpr (F == 格式::有序参数任务执行组织) return std::tie(x.参数角色节点,x.世界场景节点,
            x.实际存在节点,x.实际特征组,x.任务虚拟存在引用,x.服务要求分项引用,x.服务要求确认引用,
            x.服务要求分项快照,x.服务要求确认快照,x.有序引用);
        else return std::tie(x.参数角色节点,x.世界场景节点,x.实际存在节点,x.实际特征组,
            x.任务虚拟存在引用,x.服务要求分项引用,x.服务要求确认引用,x.服务要求分项快照,x.服务要求确认快照);
    }
    else if constexpr (std::is_same_v<U, 新本能函数本次条件>) return std::tie(x.参数角色组);
    else if constexpr (std::is_same_v<U, 新本能二次特征观察结果>) return std::tie(x.二次特征类型概念节点,
        x.上次累计值,x.本次累计值,x.结果值,x.结果值域概念节点);
    else if constexpr (std::is_same_v<U, 新本能函数本次结果>) {
        if constexpr (F == 格式::旧方法副本) return std::tie(x.状态,x.动作阶段,x.结果取得状态,
            x.动作动态节点,x.已形成状态节点组,x.已形成动态节点组,x.已形成二次特征结果组,x.建议观察特征节点组);
        else return std::tie(x.状态,x.动作阶段,x.结果取得状态,x.动作动态节点,x.已形成状态节点组,
            x.已形成动态节点组,x.已形成二次特征结果组,x.建议观察特征节点组,x.服务需求信息包,x.服务对象观察材料);
    }
    else if constexpr (std::is_same_v<U, 新本能函数本次条件结果对>) return std::tie(x.条件,x.结果);
    else if constexpr (std::is_same_v<U, 新场景快照外部引用>) return std::tie(x.种类,x.来源节点,x.状态);
    else if constexpr (std::is_same_v<U, 新场景快照缺口>) return std::tie(x.种类,x.来源节点,x.原因);
    else static_assert(!sizeof(T), "归档类型必须显式列出全部字段");
}
template<class T> struct 可选型 : std::false_type {};
template<class T> struct 可选型<std::optional<T>> : std::true_type {};
template<class T> struct 数组型 : std::false_type {};
template<class T> struct 数组型<std::vector<T>> : std::true_type {};
template<class T> struct 固定数组型 : std::false_type {};
template<class T, std::size_t N> struct 固定数组型<std::array<T,N>> : std::true_type {};
template<class T> struct 变体型 : std::false_type {};
template<class... T> struct 变体型<std::variant<T...>> : std::true_type {};
template<class T> struct 元组型 : std::false_type {};
template<class... T> struct 元组型<std::tuple<T...>> : std::true_type {};

struct 解码器 final {
    const std::vector<std::uint64_t>& 数据;
    std::size_t 位置 = 0;
    std::uint64_t 字() {
        if (位置 == 数据.size()) throw std::runtime_error("归档载荷截断");
        return 数据[位置++];
    }
    std::size_t 长度() {
        const auto n = 字();
        if (n > 数据.size() - 位置) throw std::runtime_error("归档长度越界");
        return static_cast<std::size_t>(n);
    }
};
template<格式 F, class T> void 编码(std::vector<std::uint64_t>& out, const T& x) {
    if constexpr (std::is_same_v<T, 特征概念任务集合值域> && F == 格式::旧方法副本) {
        throw std::runtime_error("当前归档格式不支持任务集合约束域");
    } else if constexpr (std::is_enum_v<T>) 编码<F>(out,static_cast<std::underlying_type_t<T>>(x));
    else if constexpr (std::is_integral_v<T>) {
        if constexpr (std::is_signed_v<T>) out.push_back(std::bit_cast<std::uint64_t>(static_cast<std::int64_t>(x)));
        else out.push_back(static_cast<std::uint64_t>(x));
    } else if constexpr (std::is_same_v<T,std::string>) {
        out.push_back(x.size()); for (const unsigned char c : x) out.push_back(c);
    } else if constexpr (可选型<T>::value) {
        编码<F>(out,x.has_value()); if (x) 编码<F>(out,*x);
    } else if constexpr (数组型<T>::value) {
        out.push_back(x.size()); for (const auto& v : x) 编码<F>(out,v);
    } else if constexpr (固定数组型<T>::value) {
        for (const auto& v : x) 编码<F>(out,v);
    } else if constexpr (变体型<T>::value) {
        if (x.valueless_by_exception()) throw std::runtime_error("归档变体无值");
        out.push_back(x.index()); std::visit([&](const auto& v) { 编码<F>(out,v); },x);
    } else if constexpr (元组型<T>::value) {
        std::apply([&](const auto&... v) { (编码<F>(out,v),...); },x);
    } else 编码<F>(out,成员<F>(x));
}
template<格式 F, class T> void 解码(解码器&,T&);
template<格式 F, std::size_t I=0,class T> void 解码变体(解码器& in,T& x,std::uint64_t index) {
    if constexpr (I < std::variant_size_v<T>) {
        if (index == I) { x.template emplace<I>(); 解码<F>(in,std::get<I>(x)); }
        else 解码变体<F,I+1>(in,x,index);
    } else throw std::runtime_error("归档变体判别非法");
}
template<格式 F, class T> void 解码(解码器& in,T& x) {
    if constexpr (std::is_same_v<T, 特征概念任务集合值域> && F == 格式::旧方法副本) {
        throw std::runtime_error("当前归档格式不支持任务集合约束域");
    } else if constexpr (std::is_enum_v<T>) {
        std::underlying_type_t<T> v{}; 解码<F>(in,v); x=static_cast<T>(v);
    } else if constexpr (std::is_integral_v<T>) {
        const auto bits=in.字();
        if constexpr (std::is_signed_v<T>) {
            const auto v=std::bit_cast<std::int64_t>(bits);
            if (v<(std::numeric_limits<T>::min)() || v>(std::numeric_limits<T>::max)())
                throw std::runtime_error("归档整数越界");
            x=static_cast<T>(v);
        } else {
            if (bits>static_cast<std::uint64_t>((std::numeric_limits<T>::max)()))
                throw std::runtime_error("归档无符号整数越界");
            x=static_cast<T>(bits);
        }
    } else if constexpr (std::is_same_v<T,std::string>) {
        const auto n=in.长度(); x.resize(n);
        for (auto& c:x) { const auto v=in.字(); if (v>255) throw std::runtime_error("归档字节越界");
            c=static_cast<char>(static_cast<unsigned char>(v)); }
    } else if constexpr (可选型<T>::value) {
        bool 有=false; 解码<F>(in,有); if (有) { x.emplace(); 解码<F>(in,*x); } else x.reset();
    } else if constexpr (数组型<T>::value) {
        const auto n=in.长度(); x.resize(n); for (auto& v:x) 解码<F>(in,v);
    } else if constexpr (固定数组型<T>::value) {
        for (auto& v : x) 解码<F>(in,v);
    } else if constexpr (变体型<T>::value) 解码变体<F>(in,x,in.字());
    else if constexpr (元组型<T>::value) std::apply([&](auto&... v) { (解码<F>(in,v),...); },x);
    else { auto fields=成员<F>(x); 解码<F>(in,fields); }
}
template<格式 F, class T> std::vector<std::uint64_t> 编成字(const T& x) {
    std::vector<std::uint64_t> out; 编码<F>(out,x); return out;
}
template<格式 F, class T> void 从字解出(const std::vector<std::uint64_t>& data,T& x) {
    解码器 in{data}; 解码<F>(in,x);
    if (in.位置!=data.size()) throw std::runtime_error("归档载荷有尾随字段");
}
template<格式 F, class T> auto 问题头(T& x) {
    if constexpr (F == 格式::旧方法副本) return std::tie(x.任务节点,x.执行实例节点,x.来源抽象方法节点,x.实例方法节点,
        x.采用配对,x.本次运行对,x.影响评估节点,x.原方向验证值,x.原值域验证值,
        x.问题组,x.符合预测状态组,x.预计但未发生项组,x.计划外状态组,x.实际状态组);
    else return std::tie(x.任务节点,x.执行实例节点,x.来源抽象方法节点,*x.任务采用,
        x.本次运行对,x.影响评估节点,x.原方向验证值,x.原值域验证值,
        x.问题组,x.符合预测状态组,x.预计但未发生项组,x.计划外状态组,x.实际状态组);
}
template<class T> auto 快照头(T& x) {
    return std::tie(x.阶段,x.采集开始,x.采集结束,x.入口场景组,x.外部引用组,x.缺口组);
}
template<格式 F, class T> void 编组(std::vector<编码记录>& out,std::int64_t kind,const std::vector<T>& group) {
    for (const auto& x:group) out.push_back({kind,编成字<F>(x)});
}
template<格式 F, class T> void 解项(const 编码记录& r,std::vector<T>& group) {
    T x{}; 从字解出<F>(r.载荷,x);
    if (!有效(x.节点) || (!group.empty() && !(group.back().节点<x.节点)))
        throw std::runtime_error("归档对象来源重复或乱序");
    group.push_back(std::move(x));
}
template<格式 F> std::vector<std::uint64_t> 编码问题头格式(const 新方法问题反馈材料& x) {
    auto out = 编成字<F>(问题头<F>(x));
    if constexpr (F == 格式::旧方法副本) if (x.采用配对.来源配对节点) {
        if (!有效(*x.采用配对.来源配对节点)) throw std::runtime_error("配对来源编码无效");
        out.push_back(0x4D50534F55524345ULL);
        out.push_back(1);
        out.push_back(x.采用配对.来源配对节点->值);
    }
    return out;
}
template<格式 F> 新方法问题反馈材料 解码问题头格式(const std::vector<std::uint64_t>& data) {
    新方法问题反馈材料 x;
    if constexpr (F != 格式::旧方法副本) {
        x.采用来源=新方法问题采用来源::任务执行组织; x.任务采用.emplace();
    }
    auto fields=问题头<F>(x);
    解码器 in{data}; 解码<F>(in,fields);
    if (in.位置 != data.size()) {
        if constexpr (F != 格式::旧方法副本) throw std::runtime_error("问题头载荷有尾随字段");
        if (data.size() - in.位置 != 3 || in.字() != 0x4D50534F55524345ULL
            || in.字() != 1) throw std::runtime_error("问题头来源扩展非法");
        const 稳定编码 来源{in.字()};
        if (!有效(来源)) throw std::runtime_error("问题头来源编码无效");
        x.采用配对.来源配对节点 = 来源;
    }
    return x;
}
template<格式 F> std::vector<编码记录> 编码场景快照格式(const 新执行场景快照& x) {
    std::vector<编码记录> out; out.push_back({0,编成字<F>(快照头(x))});
    编组<F>(out,1,x.场景组); 编组<F>(out,2,x.存在组); 编组<F>(out,3,x.特征组);
    编组<F>(out,4,x.特征概念组); 编组<F>(out,5,x.存在概念组); 编组<F>(out,6,x.值材料组);
    编组<F>(out,7,x.状态组); 编组<F>(out,8,x.动态组); return out;
}
template<格式 F> 新执行场景快照 解码场景快照格式(const std::vector<编码记录>& records) {
    if (records.empty() || records.front().类型!=0) throw std::runtime_error("归档快照头缺失");
    新执行场景快照 x; auto fields=快照头(x); 从字解出<F>(records.front().载荷,fields);
    std::int64_t previous=0;
    for (std::size_t i=1;i<records.size();++i) {
        const auto& r=records[i];
        if (r.类型<previous) throw std::runtime_error("归档对象类别乱序");
        previous=r.类型;
        switch (r.类型) {
        case 1: 解项<F>(r,x.场景组); break;
        case 2: 解项<F>(r,x.存在组); break;
        case 3: 解项<F>(r,x.特征组); break;
        case 4: 解项<F>(r,x.特征概念组); break;
        case 5: 解项<F>(r,x.存在概念组); break;
        case 6: 解项<F>(r,x.值材料组); break;
        case 7: 解项<F>(r,x.状态组); break;
        case 8: 解项<F>(r,x.动态组); break;
        default: throw std::runtime_error("归档对象类型非法");
        }
    }
    return x;
}
} // namespace

bool 采用身份有效(const 新方法问题反馈材料& x) noexcept {
    if (x.采用来源 == 新方法问题采用来源::旧方法副本)
        return !x.任务采用 && 有效(x.实例方法节点) && 有效(x.采用配对.配对节点)
            && 有效(x.采用配对.条件节点) && 有效(x.采用配对.结果节点);
    if (x.采用来源 != 新方法问题采用来源::任务执行组织 || 有效(x.实例方法节点)
        || !x.任务采用 || x.采用配对 != 新方法条件结果信息{}) return false;
    const auto& a=*x.任务采用;
    return 有效(a.定位.组织节点) && 有效(a.定位.方法作用域节点) && 有效(a.定位.配对要求节点)
        && 有效(a.来源方法节点) && a.来源方法节点==x.来源抽象方法节点
        && 有效(a.来源配对节点) && 有效(a.来源条件节点) && 有效(a.来源结果节点);
}
bool 含有序参数(const 新方法问题反馈材料& x) noexcept {
    const auto 条件含有序 = [](const 新方法条件定义& c) {
        for (const auto& item : c.条件项组) if (item.参数序号 != 0) return true;
        return false;
    };
    const auto 结果含有序 = [](const 新方法结果定义& r) {
        for (const auto& item : r.结果项组) if (item.参数序号 != 0) return true;
        for (const auto& item : r.确认要求.禁止结果项组) if (item.参数序号 != 0) return true;
        return false;
    };
    if (条件含有序(x.采用配对.条件) || 结果含有序(x.采用配对.结果)) return true;
    if (x.本次运行对) for (const auto& item : x.本次运行对->条件.参数角色组)
        if (item.有序引用) return true;
    if (x.任务采用) {
        const auto& a = *x.任务采用;
        for (const auto& item : a.参数要求组) if (item.有序规格) return true;
        for (const auto& item : a.参数绑定组) if (item.有序引用) return true;
        if (条件含有序(a.原始条件) || 条件含有序(a.需要条件) || 结果含有序(a.结果)) return true;
    }
    return false;
}
std::vector<std::uint64_t> 编码问题头(const 新方法问题反馈材料& x, 格式 f) {
    if (!采用身份有效(x)) throw std::runtime_error("问题采用来源与载荷不一致");
    if (含有序参数(x) != (f == 格式::有序参数任务执行组织))
        throw std::runtime_error("有序参数材料与归档格式不一致");
    switch (f) {
    case 格式::旧方法副本:
        if (x.采用来源 != 新方法问题采用来源::旧方法副本) break;
        return 编码问题头格式<格式::旧方法副本>(x);
    case 格式::任务执行组织:
        if (x.采用来源 != 新方法问题采用来源::任务执行组织) break;
        return 编码问题头格式<格式::任务执行组织>(x);
    case 格式::有序参数任务执行组织:
        if (x.采用来源 != 新方法问题采用来源::任务执行组织) break;
        return 编码问题头格式<格式::有序参数任务执行组织>(x);
    }
    throw std::runtime_error("问题格式与采用来源不一致");
}
新方法问题反馈材料 解码问题头(const std::vector<std::uint64_t>& data, 格式 f) {
    新方法问题反馈材料 x;
    switch (f) {
    case 格式::旧方法副本: x=解码问题头格式<格式::旧方法副本>(data); break;
    case 格式::任务执行组织: x=解码问题头格式<格式::任务执行组织>(data); break;
    case 格式::有序参数任务执行组织: x=解码问题头格式<格式::有序参数任务执行组织>(data); break;
    default: throw std::runtime_error("未知问题归档格式");
    }
    if (!采用身份有效(x)) throw std::runtime_error("问题采用载荷非法");
    if (含有序参数(x) != (f == 格式::有序参数任务执行组织))
        throw std::runtime_error("有序参数载荷与归档格式不一致");
    return x;
}
std::vector<编码记录> 编码场景快照(const 新执行场景快照& x, 格式 f) {
    switch (f) {
    case 格式::旧方法副本: return 编码场景快照格式<格式::旧方法副本>(x);
    case 格式::任务执行组织: return 编码场景快照格式<格式::任务执行组织>(x);
    case 格式::有序参数任务执行组织: return 编码场景快照格式<格式::有序参数任务执行组织>(x);
    }
    throw std::runtime_error("未知场景归档格式");
}
新执行场景快照 解码场景快照(const std::vector<编码记录>& records, 格式 f) {
    switch (f) {
    case 格式::旧方法副本: return 解码场景快照格式<格式::旧方法副本>(records);
    case 格式::任务执行组织: return 解码场景快照格式<格式::任务执行组织>(records);
    case 格式::有序参数任务执行组织: return 解码场景快照格式<格式::有序参数任务执行组织>(records);
    }
    throw std::runtime_error("未知场景归档格式");
}
} // namespace 海中鱼巣::方法问题归档内部
