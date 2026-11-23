#pragma once

#include <span>
#include <functional>
#include <variant>
#include <vector>
#include "../领域/新_方法公共类型.h"
#include "../领域/本能词典.数据.h"
#include "../领域/本能词条归类.数据.h"

namespace 海中鱼巣 {
class 概念_存在类;
class 概念_特征类;
class 新_特征值类;
class 新_方法类;
class 正式概念身份读取接口;
struct 新词典归类信息概念组;
struct 正式信息参数投影;
struct 新隔离调用证据;
struct 新隔离归类材料读取;
struct 新隔离归类特征读取;
struct 新隔离归类值读取;
struct 新隔离归类其他读取;
struct 新特征值信息;
struct 新本能函数本次条件;

using 新归类读取指针=std::variant<const 新隔离归类材料读取*,const 新隔离归类特征读取*,
    const 新隔离归类值读取*,const 新隔离归类其他读取*>;
struct 新归类有序读取 final {std::uint64_t 序号;新归类读取指针 读取;};
// 返回的指针借用证据中的元素；调用方须保持证据存活且不修改其读取组。
std::vector<新归类有序读取> 整理归类读取顺序(const 新隔离调用证据&);
std::vector<新归类有序读取> 整理归类读取顺序(const 新隔离调用证据&&)=delete;
enum class 新归类正式材料状态 : std::uint8_t {
    已取得=1,材料不适用,含本地身份,正式身份未取得,已停止
};
struct 新归类正式材料结果 final {
    新归类正式材料状态 状态=新归类正式材料状态::材料不适用;
    std::vector<std::uint64_t> 概念组;
};
// 仅用于概念集合语义的实际读取；不写入值域、概念或词典。
新归类正式材料结果 取得归类正式概念材料(const 新特征值信息&,
    std::span<const 稳定编码> 样例本地身份,
    std::span<const 正式概念身份读取接口* const>,const std::function<bool()>& 停止检查={});
struct 新归类正式名称读取 final {
    std::vector<std::uint64_t> 普通名称概念组,全部名称概念组;
    std::uint64_t 前序内容读取数=0;
    bool 动作前=false,归类结果已提交=false;
};
struct 新归类正式名称读取结果 final {
    新归类正式材料状态 状态=新归类正式材料状态::材料不适用;
    std::vector<新归类正式名称读取> 读取组;
};
// 转换同次实际名称读取；不输出样例词条或名称节点。
新归类正式名称读取结果 转换归类名称读取(const 新隔离调用证据&,
    std::span<const 稳定编码> 样例本地身份,
    std::span<const 正式概念身份读取接口* const>,const std::function<bool()>& 停止检查={});
enum class 新词典知识导出状态 : std::uint8_t {
    已形成 = 1, 正式来源已退出
};
struct 新词典知识导出结果 final {
    新词典知识导出状态 状态;
    std::optional<新方法条件结果定义> 定义;
};

// 只抽象已经采用的本次材料；不查询词典、不重读当前F、不发表方法配对。
新词典知识导出结果 导出词典条件结果(
    const 新方法条件定义& 基础条件,
    const 新本能函数本次条件& 动作前条件,
    const 新词典本能输入& 实际输入,
    稳定编码 正式来源词条,
    const 新本能词典读取回执& 已采用终态,
    概念_存在类&, 概念_特征类&, const 新_特征值类&);
// 仅形成与本次采用概念有关的片段；调用方还需合并实际读取条件，不得直接当作完整配对发布。
struct 新归类概念知识片段 final {
    新方法条件项定义 采用概念条件;
    新方法参数关系 目标词条关系;
    新方法结果项定义 挂接结果;
};
// 消费同次关闭证据；仍只返回概念片段，其余实际条件由调用方合入。
std::optional<新归类概念知识片段> 从归类关闭证据构造概念片段(
    const 新隔离调用证据&, const 新词典归类信息概念组&, 稳定编码 挂接特征类型,
    概念_存在类&, 新_方法类&, std::span<const 正式概念身份读取接口* const>);
// 只构造候选；调用方仍须完成其他必要读取、样例前态及发布路径的处理。
std::optional<新方法条件结果定义> 构造归类学习候选(
    const 新方法初始必要条件定义&,const 新本能函数本次条件&,
    const 新隔离调用证据&,std::span<const 新方法初始场景事实限定>,
    const 新词典归类信息概念组&,稳定编码 挂接特征类型,
    概念_存在类&,新_方法类&,std::span<const 正式概念身份读取接口* const>);
} // namespace 海中鱼巣
