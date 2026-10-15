#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include <variant>

#include "../核心/基础数据集.h"

namespace 海中鱼巣 {

class 新_词类;
class 概念_存在类;

// 自然语言材料只在全局基础数据集中组织，不进入世界树。
extern 稳定编码 自然语言组织根节点;
extern 稳定编码 语言组织根节点;

struct 新语言建立请求 final {
    std::string 登记键;
    std::optional<稳定编码> 创建来源节点;
};

struct 新语言信息 final {
    稳定编码 节点;
    std::string 登记键;
    std::optional<稳定编码> 创建来源节点;
    friend bool operator==(const 新语言信息&, const 新语言信息&) = default;
};

enum class 新语言建立状态 : std::uint8_t {
    已建立 = 1,
    已复用 = 2,
    输入不合法 = 3,
    依赖不存在 = 4,
    结构不一致 = 5,
    资源失败 = 6
};

enum class 新语言查询状态 : std::uint8_t {
    已找到 = 1,
    未找到 = 2,
    输入不合法 = 3,
    结构不一致 = 4,
    资源失败 = 5
};

struct 新语言建立结果 final {
    新语言建立状态 状态 = 新语言建立状态::输入不合法;
    std::optional<稳定编码> 语言节点;
};

struct 新语言查询结果 final {
    新语言查询状态 状态 = 新语言查询状态::输入不合法;
    std::optional<新语言信息> 信息;
};

struct 新语言词条匹配片段 final {
    std::optional<稳定编码> 词条节点;
    std::size_t 原文字节起点 = 0;
    std::size_t 原文字节长度 = 0;
    std::size_t 原文码点起点 = 0;
    std::size_t 原文码点数量 = 0;
    std::string 原文;
    friend bool operator==(const 新语言词条匹配片段&,
        const 新语言词条匹配片段&) = default;
};

struct 新语言词条匹配结果 final {
    新语言查询状态 状态 = 新语言查询状态::输入不合法;
    std::vector<新语言词条匹配片段> 片段组;
};

enum class 新语言知识表达目的 : std::uint8_t { 陈述 = 1, 提问 = 2 };
enum class 新语言知识作用域 : std::uint8_t { 普通陈述 = 1, 提问范围 = 2 };
enum class 新语言知识角色种类 : std::uint8_t {
    领域对象引用 = 1, 概念引用, 特征类型引用, 准确值
};
enum class 新语言知识角色类别 : std::uint8_t {
    存在 = 1, 存在概念, 特征概念, 正式特征类型,
    I64标量特征类型, I64标量
};
enum class 新语言知识角色数量 : std::uint8_t { 单项 = 1 };
// 这是7240的角色类别匹配约束；首叶不实现任意条件表达式。
struct 新语言知识角色类别约束 final {
    std::int64_t 被约束角色 = 0;
    std::vector<新语言知识角色类别> 允许类别组;
    friend bool operator==(const 新语言知识角色类别约束&,
        const 新语言知识角色类别约束&) = default;
};
struct 新语言知识角色定义 final {
    std::int64_t 局部编号 = 0;
    新语言知识角色种类 种类;
    新语言知识角色数量 数量 = 新语言知识角色数量::单项;
    std::vector<新语言知识角色类别约束> 必要约束组;
    friend bool operator==(const 新语言知识角色定义&,
        const 新语言知识角色定义&) = default;
};
enum class 新语言知识含义操作 : std::uint8_t {
    概念归类 = 1, 特征描述, 组成描述, 信息请求
};
enum class 新语言知识确认范围 : std::uint8_t { 主张成立与否 = 1 };
enum class 新语言知识参数角色 : std::uint8_t {
    主体 = 1, 归类概念, 特征类型, 特征值, 整体, 部分
};
enum class 新语言知识特征值分支 : std::uint8_t { 准确值 = 1, 概念条件 };
struct 新语言知识角色参数 final {
    新语言知识参数角色 参数;
    std::int64_t 规则角色 = 0;
    friend bool operator==(const 新语言知识角色参数&,
        const 新语言知识角色参数&) = default;
};
struct 新语言知识被询含义 final {
    std::int64_t 局部编号 = 1;
    新语言知识含义操作 操作 = 新语言知识含义操作::概念归类;
    新语言知识作用域 作用域 = 新语言知识作用域::提问范围;
    std::vector<新语言知识角色参数> 参数组;
    friend bool operator==(const 新语言知识被询含义&,
        const 新语言知识被询含义&) = default;
};
struct 新语言知识含义项 final {
    std::int64_t 局部编号 = 0;
    新语言知识含义操作 操作;
    新语言知识作用域 作用域 = 新语言知识作用域::普通陈述;
    std::vector<新语言知识角色参数> 参数组;
    std::optional<新语言知识特征值分支> 特征值分支;
    std::optional<新语言知识被询含义> 被询含义;
    std::optional<新语言知识确认范围> 确认范围;
    friend bool operator==(const 新语言知识含义项&,
        const 新语言知识含义项&) = default;
};
struct 新语言知识目标描述 final {
    新语言知识表达目的 目的 = 新语言知识表达目的::陈述;
    std::vector<新语言知识角色定义> 角色组;
    std::vector<新语言知识含义项> 含义组;
    std::vector<新语言知识角色类别约束> 适用条件组;
    friend bool operator==(const 新语言知识目标描述&,
        const 新语言知识目标描述&) = default;
};
enum class 新语言知识表达方式 : std::uint8_t {
    名称表达 = 1, 明确对象描述, 类型化字面值, 已消解指代
};
struct 新语言知识固定词项 final {
    稳定编码 词条节点;
    friend bool operator==(const 新语言知识固定词项&,
        const 新语言知识固定词项&) = default;
};
struct 新语言知识角色表达项 final {
    std::int64_t 规则角色 = 0;
    新语言知识表达方式 方式;
    std::optional<稳定编码> 指代概念节点;
    friend bool operator==(const 新语言知识角色表达项&,
        const 新语言知识角色表达项&) = default;
};
struct 新语言知识子规则映射 final {
    std::int64_t 外层角色编号 = 0;
    std::int64_t 子规则角色编号 = 0;
    friend bool operator==(const 新语言知识子规则映射&,
        const 新语言知识子规则映射&) = default;
};
struct 新语言知识子规则项 final {
    稳定编码 子知识节点;
    std::vector<新语言知识子规则映射> 映射组;
    friend bool operator==(const 新语言知识子规则项&,
        const 新语言知识子规则项&) = default;
};
using 新语言知识结构项载荷 = std::variant<新语言知识固定词项,
    新语言知识角色表达项, 新语言知识子规则项>;
struct 新语言知识结构项 final {
    std::int64_t 局部编号 = 0;
    新语言知识结构项载荷 载荷;
    friend bool operator==(const 新语言知识结构项&,
        const 新语言知识结构项&) = default;
};
enum class 新语言知识结构类别 : std::uint8_t { 基础表达 = 1 };
struct 新语言知识描述结构 final {
    新语言知识结构类别 类别 = 新语言知识结构类别::基础表达;
    稳定编码 语言节点;
    std::vector<新语言知识结构项> 有序项组;
    friend bool operator==(const 新语言知识描述结构&,
        const 新语言知识描述结构&) = default;
};
struct 新语言知识定义 final {
    新语言知识目标描述 目标;
    新语言知识描述结构 结构;
    friend bool operator==(const 新语言知识定义&, const 新语言知识定义&) = default;
};
enum class 新语言知识来源种类 : std::uint8_t { 经确认先天定义 = 1 };
struct 新语言知识来源请求 final {
    新语言知识来源种类 种类 = 新语言知识来源种类::经确认先天定义;
    稳定编码 语言节点;
    std::string 固定原文;
};
struct 新语言知识来源信息 final {
    稳定编码 节点;
    新语言知识来源种类 种类;
    稳定编码 语言节点;
    std::string 固定原文;
};
struct 新语言知识来源绑定 final {
    稳定编码 来源节点;
    std::int64_t 起始标量 = 0, 结束标量 = 0;
    friend bool operator==(const 新语言知识来源绑定&,
        const 新语言知识来源绑定&) = default;
};
struct 新语言知识发布请求 final {
    新语言知识定义 定义;
    std::vector<新语言知识来源绑定> 来源组;
};
struct 新语言知识信息 final {
    稳定编码 节点, 目标节点, 结构节点;
    新语言知识定义 定义;
    std::vector<新语言知识来源绑定> 来源组;
};
enum class 新语言知识状态 : std::uint8_t {
    已完成 = 1, 未找到, 输入拒绝, 尚未初始化, 未支持,
    引用不可读, 读取漂移, 内部不一致, 工程未完成, 额度不足
};
enum class 新语言知识发布种类 : std::uint8_t { 已建立 = 1, 已复用 };
enum class 新语言知识发布阶段 : std::uint8_t { 前置, 定义发布, 来源补充, 完整读回, 完成 };
struct 新语言知识来源结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    std::optional<新语言知识发布种类> 发布种类;
    std::optional<新语言知识来源信息> 信息;
};
struct 新语言知识结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    新语言知识发布阶段 阶段 = 新语言知识发布阶段::前置;
    std::optional<新语言知识发布种类> 发布种类;
    std::optional<稳定编码> 已发生定义身份;
    std::optional<新语言知识信息> 信息;
    std::string 诊断说明;
};
struct 新语言知识候选列表结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    std::vector<新语言知识信息> 信息组;
};

// 语言节点登记语言身份；词面匹配只组合公开读取，不解释词义或绑定概念。
class 新_语言类 final {
public:
    // 只做第一阶段 UTF-8 合法性校验，不改变输入字节。
    static bool 严格UTF8字节串有效(const std::string& 文本) noexcept;

    bool 初始化() noexcept;

    新语言建立结果 建立或取得语言(
        const 新语言建立请求& 请求) noexcept;
    新语言查询结果 获取语言(稳定编码 语言节点) const noexcept;
    新语言查询结果 按登记键查询(
        const std::string& 登记键) const noexcept;
    bool 是语言节点(稳定编码 节点) const noexcept;

    // UTF-8 字节精确：不折叠大小写/空白，不做 NFC/NFKC 或编码修复；
    // 非空输入沿用严格校验（包括拒绝 U+FEFF、U+FFFD，允许 NUL）。
    // 原文区间为零基起点+长度；未知连续码点合并，引用为空。
    // 已找到表示完整处理（含空文本/全未知），失败总是返回空片段组。
    // 词库以本次枚举到的 ID 为界，不含随后新登记词；不持语言锁调用词服务。
    新语言词条匹配结果 按已登记词条最长匹配(
        稳定编码 语言节点,
        const std::string& 文本,
        const 新_词类& 词服务) const noexcept;

    // 结构发布与三段完整读回；工程异常向上送出，锁内不回调词服务。
    新语言知识状态 初始化语言知识结构();
    新语言知识来源结果 建立或取得知识先天来源(const 新语言知识来源请求& 请求);
    新语言知识来源结果 获取知识先天来源(稳定编码 来源节点) const;
    新语言知识来源结果 查询知识先天来源(const 新语言知识来源请求& 请求) const;
    新语言知识结果 建立或取得语言知识(
        const 新语言知识发布请求& 请求, const 新_词类& 词服务,
        const 概念_存在类& 存在概念服务);
    新语言知识结果 获取语言知识(稳定编码 知识节点,
        const 新_词类& 词服务, const 概念_存在类& 存在概念服务) const;
    新语言知识结果 查询语言知识(
        const 新语言知识定义& 定义, const 新_词类& 词服务,
        const 概念_存在类& 存在概念服务) const;
    新语言知识候选列表结果 枚举语言知识候选(
        稳定编码 语言节点, std::size_t 最大条目数,
        const 新_词类& 词服务, const 概念_存在类& 存在概念服务) const;

};

} // namespace 海中鱼巣
