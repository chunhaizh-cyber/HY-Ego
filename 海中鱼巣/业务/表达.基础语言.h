#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "../领域/新_语言类.h"
#include "../领域/信息内容.数据.h"

namespace 海中鱼巣 {

class 新_词类;
class 新_方法类;
class 概念_名称关系类;
class 概念_存在类;
class 概念_特征类;
class 新_存在类;
struct 普通应用网页自我归类表达请求;
struct 普通应用网页自我归类表达结果;

enum class 基础表达目标节点类型 : std::uint8_t { 存在 = 1 };
enum class 基础表达来源种类 : std::uint8_t { 网页自我待发 = 1 };
struct 基础表达角色来源快照 final {
    基础表达来源种类 种类 = 基础表达来源种类::网页自我待发;
    std::string 公开会话标识;
    稳定编码 说话者存在节点{};
    稳定编码 交互窗口节点{};
    std::uint64_t 读取时页面末序号 = 0;
    friend bool operator==(const 基础表达角色来源快照&,
        const 基础表达角色来源快照&) = default;
};
class 基础表达网页自我来源 final {
public:
    基础表达网页自我来源(const 基础表达网页自我来源&) = default;
    基础表达网页自我来源(基础表达网页自我来源&&) = default;
    基础表达网页自我来源& operator=(const 基础表达网页自我来源&) = default;
    基础表达网页自我来源& operator=(基础表达网页自我来源&&) = default;
    const 基础表达角色来源快照& 读取() const noexcept { return 快照_; }
private:
    explicit 基础表达网页自我来源(基础表达角色来源快照 快照)
        : 快照_(std::move(快照)) {}
    基础表达角色来源快照 快照_;
    friend 普通应用网页自我归类表达结果 生成普通应用网页自我归类表达(
        const 普通应用网页自我归类表达请求&) noexcept;
};
struct 基础表达指代选择 final {
    std::int64_t 规则角色 = 0;
    稳定编码 指代概念节点{};
    基础表达目标节点类型 目标类型 = 基础表达目标节点类型::存在;
    稳定编码 目标存在节点{};
};
struct 基础表达指代出现 final {
    std::int64_t 局部出现编号 = 0;
    基础表达角色来源快照 作用域来源;
    稳定编码 指代概念节点{};
    稳定编码 指代名称关系节点{};
    基础表达目标节点类型 目标类型 = 基础表达目标节点类型::存在;
    稳定编码 目标存在节点{};
    稳定编码 所用知识节点{};
    std::vector<std::int64_t> 结构项路径;
    std::size_t 起始标量 = 0, 结束标量 = 0;
};

enum class 基础表达参数种类 : std::uint8_t { 概念 = 1, 领域对象, 特征类型, 准确值 };
struct 基础表达参数 final {
    新语言知识参数角色 参数;
    新语言知识角色类别 类别;
    基础表达参数种类 种类;
    std::variant<稳定编码, std::int64_t> 内容;
    std::optional<稳定编码> 选用名称关系;
};
struct 语言图引用值 final {
    新语言知识角色类别 类别=新语言知识角色类别::存在;
    稳定编码 节点{};
    std::optional<稳定编码> 来源概念,选用名称关系;
    friend bool operator==(const 语言图引用值&,const 语言图引用值&)=default;
};
struct 语言图整数值 final {
    std::string 规范十进制;
    std::optional<稳定编码> 单位;
    friend bool operator==(const 语言图整数值&,const 语言图整数值&)=default;
};
struct 语言图子出现引用 final {
    std::int64_t 出现编号=0;
    friend bool operator==(const 语言图子出现引用&,const 语言图子出现引用&)=default;
};
using 语言图绑定值=std::variant<语言图引用值,语言图整数值,语言图子出现引用>;
struct 语言图角色绑定 final {
    std::int64_t 角色编号=0;
    语言图绑定值 值;
    friend bool operator==(const 语言图角色绑定&,const 语言图角色绑定&)=default;
};
struct 语言图含义出现 final {
    std::int64_t 出现编号=0;
    稳定编码 所用知识节点{};
    std::int64_t 定义含义编号=0;
    新语言知识作用域 有效作用域=新语言知识作用域::普通陈述;
    std::vector<std::int64_t> 结构项路径;
    std::vector<语言图角色绑定> 角色组;
    std::optional<std::pair<std::size_t,std::size_t>> 原文范围;
    friend bool operator==(const 语言图含义出现&,const 语言图含义出现&)=default;
};
struct 语言图含义材料 final {
    稳定编码 语言节点{};
    新语言知识含义图 定义图;
    std::int64_t 根出现编号=0;
    std::vector<语言图含义出现> 出现组;
    std::optional<std::string> 固定原文;
    std::optional<稳定编码> 未核验来源语言记录节点;
    std::optional<正式信息参数投影> 正式需求来源;
};
struct 基础表达请求 final {
    稳定编码 语言节点;
    新语言知识表达目的 目的;
    新语言知识含义操作 操作;
    新语言知识作用域 作用域;
    std::vector<基础表达参数> 参数组;
    std::size_t 最大知识条目数;
    std::size_t 最大名称条目数;
    std::size_t 最大输出标量数;
    std::optional<稳定编码> 受询者;
    std::optional<新语言知识含义操作> 被询操作;
    std::optional<新语言知识确认范围> 确认范围;
    std::optional<基础表达指代选择> 指代选择;
    std::optional<基础表达网页自我来源> 网页自我来源;
    std::optional<语言图含义材料> 含义图;
};
enum class 基础表达状态 : std::uint8_t {
    已生成, 输入拒绝, 缺规则, 缺名称, 名称待选, 缺指称, 多规则待选,
    能力未支持, 引用不可读, 读取漂移, 额度不足, 工程未完成, 内部不一致
};
struct 基础表达对齐项 final {
    std::size_t 起始标量, 结束标量;
    std::int64_t 结构项编号;
    std::optional<新语言知识参数角色> 参数;
    std::optional<稳定编码> 词条节点;
    std::optional<稳定编码> 名称关系节点;
    std::optional<稳定编码> 所属知识节点;
    std::vector<std::int64_t> 结构项路径;
    std::optional<稳定编码> 指代名称关系节点;
};
struct 基础表达结果 final {
    基础表达状态 状态 = 基础表达状态::输入拒绝;
    std::string 文本;
    std::optional<稳定编码> 知识节点;
    std::vector<基础表达参数> 已用绑定;
    std::vector<基础表达对齐项> 对齐组;
    std::optional<稳定编码> 已核验受询者;
    std::vector<基础表达指代出现> 指代出现组;
    std::vector<稳定编码> 受阻节点组;
    std::optional<语言图含义材料> 已用含义图;
    std::vector<稳定编码> 已用知识节点组;
};

基础表达结果 生成基础语言表达(
    const 基础表达请求& 请求,
    const 新_语言类& 语言服务, const 新_词类& 词服务,
    const 概念_名称关系类& 名称服务,
    const 概念_存在类& 存在概念服务,
    const 概念_特征类& 特征概念服务,
    const 新_存在类& 受询者存在服务);

// 理解入口钩子：若匹配 SC-请求执行方法，则经 新_方法类::查询候选方法 取候选；
// 不写任务完成态，不绕过 5260。
// 待审：需求侧信息请求与 S13 一并实现；信息请求（含 S13）不走请求执行方法理解入口。
struct 结构概念理解请求 final {
    稳定编码 语言节点;
    std::string 模式标识; // 或由切分匹配得到
    std::optional<稳定编码> 表达模式节点;
    // 方法查找可选过滤（由调用方从槽位绑定投影）
    std::vector<稳定编码> 可能结果概念组;
};
struct 结构概念理解结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    std::vector<新语言结构对应行> 对应行组;
    std::vector<稳定编码> 候选方法节点组; // 仅请求执行方法时填充
    std::string 诊断说明;
    std::vector<稳定编码> 受阻节点组;
};
结构概念理解结果 理解结构概念表达(
    const 结构概念理解请求& 请求,
    const 新_语言类& 语言服务, const 新_词类& 词服务,
    const 新_方法类* 方法服务);

} // namespace 海中鱼巣
