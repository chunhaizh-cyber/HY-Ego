#pragma once
#include "语言含义网.数据.h"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>
#include <variant>

#include "../核心/基础数据集.h"
#include "语言分界.数据.h"

namespace 海中鱼巣 {

class 新_词类;
class 概念_存在类;
class 新_特征类;
class 新_存在类;
class 概念_特征类;
struct 新接收认知来源键;
struct 新语言含义采用请求;
struct 新语言含义采用结果;
struct 新语言需求目标投影结果;
struct 新语言行动请求投影结果;
class 新语言采用来源读取接口;

enum class 语言解释读取模式 : std::uint8_t {
    显式额度=1, 本次正式有限集完整读取=2
};

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

enum class 新语言知识表达目的 : std::uint8_t { 陈述 = 1, 提问 = 2, 提出需求 = 3, 招呼 = 4 };
enum class 新语言知识作用域 : std::uint8_t { 普通陈述 = 1, 提问范围 = 2, 请求范围 = 3, 需求目标范围 = 4 };
enum class 新语言知识角色种类 : std::uint8_t {
    领域对象引用 = 1, 概念引用, 特征类型引用, 准确值, 查询变量, 子含义引用
};
enum class 新语言知识角色类别 : std::uint8_t {
    存在 = 1, 存在概念, 特征概念, 正式特征类型,
    I64标量特征类型, I64标量, 查询特征值, 子含义, 十进制整数, 正式信息节点
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
    概念归类 = 1, 特征描述, 组成描述, 信息请求, 需求描述, 缺口登记, 特征区间描述, 特征方向描述, 行动请求,
    // 招呼（CONV-EXPR-TEXT §3.5）：零角色、零参数、无子含义，种类由操作区分。
    招呼发起, 招呼维持, 招呼结束
};
enum class 新语言知识确认范围 : std::uint8_t { 主张成立与否 = 1, 填项内容 = 2 };
enum class 新语言知识参数角色 : std::uint8_t {
    主体 = 1, 归类概念, 特征类型, 特征值, 整体, 部分, 受询者, 询求者, 下界, 上界, 目标内容, 被询内容, 受请求者
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
    std::optional<新语言知识特征值分支> 特征值分支;
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
enum class 新语言知识特征方向 : std::uint8_t { 增加=1, 减少=2 };
enum class 新语言子含义用途 : std::uint8_t { 需求目标=1, 被询内容=2, 期望目标=3 };
struct 新语言局部角色引用 final {
    std::int64_t 含义编号=0, 角色编号=0;
    friend bool operator==(const 新语言局部角色引用&,const 新语言局部角色引用&)=default;
};
struct 新语言子含义映射 final {
    新语言局部角色引用 来源,目标;
    friend bool operator==(const 新语言子含义映射&,const 新语言子含义映射&)=default;
};
struct 新语言子含义边 final {
    std::int64_t 本节点角色编号=0,子含义编号=0;
    新语言子含义用途 用途=新语言子含义用途::需求目标;
    std::vector<新语言子含义映射> 显式映射组;
    friend bool operator==(const 新语言子含义边&,const 新语言子含义边&)=default;
};
struct 新语言图含义节点 final {
    std::int64_t 局部编号=0;
    新语言知识含义操作 操作=新语言知识含义操作::概念归类;
    新语言知识作用域 作用域=新语言知识作用域::普通陈述;
    std::vector<新语言知识角色定义> 角色组;
    std::vector<新语言知识角色参数> 参数组;
    std::optional<新语言知识特征值分支> 特征值分支;
    std::optional<新语言知识确认范围> 确认范围;
    std::optional<新语言知识特征方向> 方向;
    std::vector<新语言子含义边> 子含义组;
    friend bool operator==(const 新语言图含义节点&,const 新语言图含义节点&)=default;
};
struct 新语言知识含义图 final {
    std::int64_t 根含义编号=0;
    std::vector<新语言图含义节点> 节点组;
    friend bool operator==(const 新语言知识含义图&,const 新语言知识含义图&)=default;
};
struct 新语言知识目标描述 final {
    新语言知识表达目的 目的 = 新语言知识表达目的::陈述;
    std::vector<新语言知识角色定义> 角色组;
    std::vector<新语言知识含义项> 含义组;
    std::vector<新语言知识角色类别约束> 适用条件组;
    std::optional<新语言知识含义图> 含义图;
    friend bool operator==(const 新语言知识目标描述&,
        const 新语言知识目标描述&) = default;
};
enum class 新语言知识表达方式 : std::uint8_t {
    名称表达 = 1, 明确对象描述, 类型化字面值, 已消解指代, 带概念指代, 查询变量词面
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
struct 新语言知识图角色表达项 final {
    新语言局部角色引用 角色;
    新语言知识表达方式 方式=新语言知识表达方式::名称表达;
    std::optional<稳定编码> 指代概念节点, 指代名称关系节点;
    std::optional<稳定编码> 概念前词条节点, 概念后词条节点, 查询词条节点;
    friend bool operator==(const 新语言知识图角色表达项&,const 新语言知识图角色表达项&)=default;
};
struct 新语言知识图子规则项 final {
    新语言局部角色引用 子角色;
    std::vector<稳定编码> 允许子知识组;
    std::vector<新语言子含义映射> 映射组;
    friend bool operator==(const 新语言知识图子规则项&,const 新语言知识图子规则项&)=default;
};
using 新语言知识结构项载荷 = std::variant<新语言知识固定词项,
    新语言知识角色表达项, 新语言知识子规则项,新语言知识图角色表达项,新语言知识图子规则项>;
struct 新语言知识结构项 final {
    std::int64_t 局部编号 = 0;
    新语言知识结构项载荷 载荷;
    friend bool operator==(const 新语言知识结构项&,
        const 新语言知识结构项&) = default;
};
enum class 新语言知识结构类别 : std::uint8_t { 基础表达 = 1, 缺口声明 = 2 };
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
enum class 新语言知识来源保护状态 : std::uint8_t {
    已读取=1, 来源不存在, 尚未初始化, 输入拒绝, 前提已变化, 资源失败
};
struct 新语言知识来源保护结果 final {
    新语言知识来源保护状态 状态 = 新语言知识来源保护状态::输入拒绝;
    std::optional<新语言知识来源信息> 信息;
    std::optional<新语言信息> 语言信息;
    std::vector<基础联合节点快照> 完整保护;
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

enum class 新语言知识缺口编号 : std::uint8_t {
    G01析取与选择 = 1, G02认识模态, G03规范模态, G04通用时间与体,
    G05一般事件参与和语态, G06一般目的与意图, G07焦点排他与预设,
    G08程度结果趋向补语, G09独立感叹与语气, G10一般关系从句与名词化,
    G11模糊数量与程度, G12让步与同步, G13话题与证据来源, G14反问与省略续问
};
struct 新语言知识缺口定义 final {
    新语言知识缺口编号 编号 = 新语言知识缺口编号::G01析取与选择;
    std::string 标签;
    std::vector<std::string> 提示词面组;
    friend bool operator==(const 新语言知识缺口定义&,
        const 新语言知识缺口定义&) = default;
};
struct 新语言知识缺口信息 final {
    稳定编码 节点;
    稳定编码 语言节点;
    新语言知识缺口定义 定义;
    std::string 种子版本;
};
struct 新语言知识缺口结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    std::optional<新语言知识发布种类> 发布种类;
    std::optional<新语言知识缺口信息> 信息;
};
struct 新语言知识缺口列表结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    std::vector<新语言知识缺口信息> 信息组;
};


// —— 结构概念 / 表达模式 / 语法 / 语言结构对应表 DTO（按现有 新语言知识* 风格提案）——
enum class 新语言结构目的细目 : std::uint8_t {
    陈述 = 1, 填项问, 是非问, 指令
};
enum class 新语言结构信息种类 : std::uint8_t {
    特征值 = 1, 需求, 任务, 方法, 归类, 上下位, 拥有,
    否定归类, 否定特征有无, 否定特征值, 请求执行方法, 特征区间, 特征方向, 行动请求
};
enum class 新语言语法种类 : std::uint8_t { 语序 = 1, 形态, 一致 };
enum class 新语言结构对应状态 : std::uint8_t { 已登记 = 1, 缺口, 已替代 };
enum class 新语言结构来源种类 : std::uint8_t { 种子 = 1, 规则派生, 交互纠正 };
enum class 新语言表达组成种类 : std::uint8_t { 固定词面 = 1, 模式槽位 };

struct 新语言结构含义角色 final {
    std::string 局部编号; // E1 / FT1 / H1 / M1 …；本结构概念内固定
    新语言知识角色种类 种类 = 新语言知识角色种类::领域对象引用;
    新语言知识参数角色 参数角色 = 新语言知识参数角色::主体;
    std::vector<新语言知识角色类别> 允许类别组;
    bool 可选 = false;
    friend bool operator==(const 新语言结构含义角色&,
        const 新语言结构含义角色&) = default;
};
struct 新语言结构概念定义 final {
    std::string 种子标识; // 如 SC-特征值-陈述；非词条
    新语言结构信息种类 种类 = 新语言结构信息种类::特征值;
    新语言结构目的细目 细目 = 新语言结构目的细目::陈述;
    新语言知识含义操作 操作 = 新语言知识含义操作::特征描述;
    std::vector<新语言结构含义角色> 含义角色组;
    新语言结构来源种类 来源 = 新语言结构来源种类::种子;
    friend bool operator==(const 新语言结构概念定义&,
        const 新语言结构概念定义&) = default;
};
struct 新语言结构概念信息 final {
    稳定编码 节点;
    新语言结构概念定义 定义;
    std::string 种子版本;
};
struct 新语言结构概念结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    std::optional<新语言知识发布种类> 发布种类;
    std::optional<新语言结构概念信息> 信息;
    std::vector<稳定编码> 受阻节点组;
};
struct 新语言结构概念列表结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    std::vector<新语言结构概念信息> 信息组;
    std::vector<稳定编码> 受阻节点组;
};

struct 新语言表达模式槽位 final {
    std::string 字母; // 本模式局部；登记后本模式内固定
    bool 可选 = false;
    friend bool operator==(const 新语言表达模式槽位&,
        const 新语言表达模式槽位&) = default;
};
struct 新语言表达组成项 final {
    新语言表达组成种类 种类 = 新语言表达组成种类::固定词面;
    std::string 词面; // 固定虚词词面；发布时解析为词条引用
    std::string 槽位字母;
    friend bool operator==(const 新语言表达组成项&,
        const 新语言表达组成项&) = default;
};
struct 新语言槽位含义映射 final {
    std::string 槽位字母;
    std::string 含义角色局部编号;
    friend bool operator==(const 新语言槽位含义映射&,
        const 新语言槽位含义映射&) = default;
};
struct 新语言模式知识角色映射 final {
    std::string 含义角色局部编号;
    std::int64_t 知识图根角色编号=0;
    std::optional<std::int64_t> 知识图含义编号;
    friend bool operator==(const 新语言模式知识角色映射&,const 新语言模式知识角色映射&)=default;
};
struct 新语言表达模式定义 final {
    std::string 模式标识; // 如 SC-特征值-陈述.整数 / 请用M
    std::vector<新语言表达模式槽位> 槽位表;
    std::vector<新语言表达组成项> 组成项组;
    std::vector<新语言槽位含义映射> 映射组;
    std::vector<std::string> 适用语法编号组; // H-ORD-02 等；重建对应表时解析
    // 待审：修饰槽首批 = 紧急度(尽快/马上) + 期限(在T之前)；无值则省略。
    std::vector<std::string> 修饰槽标识组;
    std::optional<稳定编码> 实现知识节点;
    std::vector<新语言模式知识角色映射> 知识角色映射组;
    friend bool operator==(const 新语言表达模式定义&,
        const 新语言表达模式定义&) = default;
};
struct 新语言表达模式信息 final {
    稳定编码 节点;
    稳定编码 语言节点;
    稳定编码 结构概念节点;
    新语言表达模式定义 定义;
    std::string 种子版本;
    std::vector<稳定编码> 固定词条节点组; // 按固定词出现顺序，不含槽位项
};
struct 新语言表达模式结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    std::optional<新语言知识发布种类> 发布种类;
    std::optional<新语言表达模式信息> 信息;
    std::vector<稳定编码> 受阻节点组;
};
struct 新语言表达模式列表结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    std::vector<新语言表达模式信息> 信息组;
    std::vector<稳定编码> 受阻节点组;
};

struct 新语言语法规则定义 final {
    std::string 编号; // H-ORD-01
    新语言语法种类 种类 = 新语言语法种类::语序;
    std::string 条件;
    std::string 结果文本;
    friend bool operator==(const 新语言语法规则定义&,
        const 新语言语法规则定义&) = default;
};
struct 新语言语法规则信息 final {
    稳定编码 节点;
    稳定编码 语言节点;
    新语言语法规则定义 定义;
    std::string 种子版本;
    新语言结构来源种类 来源 = 新语言结构来源种类::种子;
};
struct 新语言语法规则结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    std::optional<新语言知识发布种类> 发布种类;
    std::optional<新语言语法规则信息> 信息;
};
struct 新语言语法规则列表结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    std::vector<新语言语法规则信息> 信息组;
};

struct 新语言量词搭配信息 final {
    稳定编码 节点;
    稳定编码 语言节点;
    std::string 量词词面;
    bool 是回退默认 = false;
    std::string 种子版本;
};
struct 新语言量词搭配结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    std::optional<新语言知识发布种类> 发布种类;
    std::optional<新语言量词搭配信息> 信息;
};
struct 新语言量词搭配列表结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    std::vector<新语言量词搭配信息> 信息组;
};
struct 新语言量词缺口信息 final {
    稳定编码 节点;
    稳定编码 语言节点;
    std::string 量词词面;
    std::string 样例原文;
    std::string 种子版本;
};
struct 新语言量词缺口结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    std::optional<新语言知识发布种类> 发布种类;
    std::optional<新语言量词缺口信息> 信息;
};
struct 新语言量词缺口列表结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    std::vector<新语言量词缺口信息> 信息组;
};

struct 新语言结构对应行 final {
    稳定编码 节点;
    稳定编码 语言节点;
    std::optional<稳定编码> 结构概念节点;
    std::optional<稳定编码> 表达模式节点;
    新语言结构信息种类 种类 = 新语言结构信息种类::特征值;
    新语言结构目的细目 细目 = 新语言结构目的细目::陈述;
    std::vector<稳定编码> 语法规则节点组;
    新语言结构对应状态 状态 = 新语言结构对应状态::已登记;
    新语言结构来源种类 来源 = 新语言结构来源种类::种子;
    std::string 种子版本;
    std::string 结构概念种子标识;
    std::string 模式标识;
    std::optional<稳定编码> 实现知识节点;
};
struct 新语言结构对应结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    std::optional<新语言知识发布种类> 发布种类;
    std::optional<新语言结构对应行> 信息;
    std::vector<稳定编码> 受阻节点组;
};
struct 新语言结构对应列表结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    std::vector<新语言结构对应行> 信息组;
    std::vector<稳定编码> 受阻节点组;
};
struct 新语言结构对应查询结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    std::vector<新语言结构对应行> 信息组; // 0 条且无工程失败 → 调用方可登记缺口
    std::vector<稳定编码> 受阻节点组;
};
struct 新语言结构对应重建结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    std::size_t 已登记行数 = 0;
    std::size_t 缺口行数 = 0;
    std::size_t 派生行数 = 0;
    std::vector<稳定编码> 受阻节点组;
};

struct 新语言结构模式声明 final {
    std::string 结构概念种子标识;
    新语言表达模式定义 定义;
    std::optional<std::string> 实现知识来源标签;
    friend bool operator==(const 新语言结构模式声明&, const 新语言结构模式声明&) = default;
};
struct 新语言结构量词声明 final {
    std::string 词面;
    bool 是回退默认 = false;
    friend bool operator==(const 新语言结构量词声明&, const 新语言结构量词声明&) = default;
};
struct 新语言结构缺口声明 final {
    新语言结构信息种类 种类;
    新语言结构目的细目 细目;
    friend bool operator==(const 新语言结构缺口声明&, const 新语言结构缺口声明&) = default;
};
struct 新语言结构定义集声明 final {
    std::string 种子版本;
    std::vector<新语言结构概念定义> 概念组;
    std::vector<新语言结构模式声明> 模式组;
    std::vector<新语言语法规则定义> 语法组;
    std::vector<新语言结构量词声明> 量词组;
    std::vector<新语言结构缺口声明> 缺口组;
    friend bool operator==(const 新语言结构定义集声明&, const 新语言结构定义集声明&) = default;
};
struct 新语言结构定义集发布请求 final {
    稳定编码 语言节点, 原种子来源节点;
    新语言结构定义集声明 声明;
};
struct 新语言结构定义集信息 final {
    稳定编码 节点, 语言节点, 原种子来源节点;
    新语言结构定义集声明 声明;
    std::vector<新语言结构概念信息> 概念组;
    std::vector<新语言表达模式信息> 模式组;
    std::vector<新语言语法规则信息> 语法组;
    std::vector<新语言量词搭配信息> 量词组;
};
struct 新语言结构定义集结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    std::optional<新语言知识发布种类> 发布种类;
    std::optional<新语言结构定义集信息> 信息;
    std::optional<稳定编码> 已发布节点;
    std::vector<稳定编码> 受阻节点组;
    std::string 诊断说明;
};
struct 新语言结构采用引用 final {
    稳定编码 定义集节点, 对应视图节点;
    friend bool operator==(const 新语言结构采用引用&, const 新语言结构采用引用&) = default;
};
struct 新语言结构采用请求 final {
    稳定编码 语言节点, 目标定义集节点;
    std::optional<新语言结构采用引用> 预期当前采用;
};
struct 新语言结构采用结果 final {
    新语言知识状态 状态 = 新语言知识状态::输入拒绝;
    bool 本次已发布 = false;
    std::optional<新语言结构采用引用> 采用;
    std::vector<稳定编码> 受阻节点组;
    std::string 诊断说明;
};

struct 新语言结构生成查询 final {
    稳定编码 语言节点;
    新语言结构信息种类 种类 = 新语言结构信息种类::特征值;
    新语言结构目的细目 细目 = 新语言结构目的细目::陈述;
};
struct 新语言结构理解查询 final {
    稳定编码 语言节点;
    std::optional<稳定编码> 表达模式节点;
    std::string 模式标识; // 与节点二选一；都空则输入拒绝
};
struct 新语言结构覆盖查询 final {
    稳定编码 语言节点; // 明确的本语言采用范围
    std::optional<新语言结构信息种类> 种类过滤;
    std::optional<新语言结构目的细目> 细目过滤;
};

enum class 新语言解释资源保护状态 : std::uint8_t {
    已读取=1, 尚未初始化, 输入拒绝, 前提已变化, 资源失败
};
struct 新语言解释资源保护结果 final {
    新语言解释资源保护状态 状态 = 新语言解释资源保护状态::输入拒绝;
    std::optional<新语言结构采用引用> 当前采用;
    std::optional<新语言结构定义集信息> 当前定义集;
    std::vector<新语言知识信息> 候选知识组;
    std::vector<基础联合节点快照> 完整保护;
};

// ASK-READ-SCENE v0.4 §3.3：接收来源场景中保存的场景信息（值陈述或招呼）。
// 只记录对方说了什么；不判断它回答了哪个问题（不做问答匹配），也不建立需求。
enum class 新接收场景信息种类 : std::uint8_t { 值陈述=1, 招呼 };
enum class 新值陈述值种类 : std::uint8_t { 准确整数=1, 整数闭区间, 变化方向, 特征概念条件, 待求 };
struct 新接收场景信息 final {
    新接收场景信息种类 种类=新接收场景信息种类::值陈述;
    // 来源场景＝接收来源场景（语言世界）；交互场景＝消息所在外设对话场景。
    稳定编码 来源消息{}, 来源场景{}, 交互场景{}, 说话者{};
    // 主体：已绑定存在，或待定（只有指称概念，或两者皆空）。
    std::optional<稳定编码> 主体存在, 主体概念, 特征类型;
    std::optional<新值陈述值种类> 值种类;
    std::optional<std::string> 准确整数, 区间下界, 区间上界;
    std::optional<稳定编码> 单位, 特征概念;
    std::optional<新语言知识特征方向> 方向;
    std::optional<新语言知识含义操作> 招呼操作;
    // 原文范围为码点区间；时刻为来源消息正式记录时间（UTC纳秒）。
    std::int64_t 原文起点=0, 原文终点=0, 时刻=0;
    friend bool operator==(const 新接收场景信息&,const 新接收场景信息&)=default;
};
enum class 新接收场景信息保存状态 : std::uint8_t { 已保存=1, 精确重复, 来源冲突, 前提已变化 };
struct 新接收场景信息保存结果 final {
    新接收场景信息保存状态 状态=新接收场景信息保存状态::前提已变化;
    std::optional<稳定编码> 陈述节点;
};
struct 新值陈述信息 final {
    稳定编码 陈述节点{};
    新接收场景信息 内容;
};

// ASK-READ-SCENE v0.5 §3.2：自我写入的语言信息——自我把找到的值写入自然语言世界，来源标明为自我。
// 写入来源是本次写入的唯一身份（每个写入来源至多一条）；来源场景由自然语言世界按写入来源建立。
struct 新自我写入语言信息 final {
    稳定编码 写入来源{}, 来源场景{}, 自我{}, 主体存在{}, 特征类型{};
    新值陈述值种类 值种类=新值陈述值种类::准确整数;
    std::optional<std::string> 准确整数, 区间下界, 区间上界;
    std::optional<稳定编码> 单位, 特征概念;
    std::optional<新语言知识特征方向> 方向;
    std::int64_t 时刻=0; // UTC纳秒
    friend bool operator==(const 新自我写入语言信息&,const 新自我写入语言信息&)=default;
};
struct 新自我写入信息保存结果 final {
    新接收场景信息保存状态 状态=新接收场景信息保存状态::前提已变化;
    std::optional<稳定编码> 信息节点;
};

// ASK-READ-SCENE v0.5 §3.2：出站问句在出站来源语言场景中建立的“问题”，带被问结构；“答案”初无值。
// 每个出站来源至多一个问题；问题须对应该场景已留档的出站具体表达。
struct 新语言问题定义 final {
    // 交互者：提问时该交互中的交互者（受话者）；主体待定的值陈述按说话者是否为该交互者比对。
    稳定编码 出站来源场景{}, 出站来源{}, 交互场景{}, 交互者{}, 被认知需求{}, 主体{}, 被问栏{};
    std::int64_t 提问时刻=0; // UTC纳秒；答案须严格晚于此时刻
    friend bool operator==(const 新语言问题定义&,const 新语言问题定义&)=default;
};
enum class 新问题答案来源 : std::uint8_t { 消息转换=1, 自我写入 };
struct 新问题答案引用 final {
    稳定编码 答案信息{};
    新问题答案来源 来源=新问题答案来源::消息转换;
    friend bool operator==(const 新问题答案引用&,const 新问题答案引用&)=default;
};
struct 新语言问题信息 final {
    稳定编码 问题节点{};
    新语言问题定义 定义;
    std::optional<新问题答案引用> 答案;
};
enum class 新语言问题建立状态 : std::uint8_t { 已建立=1, 已存在, 来源冲突, 出站留档不符, 前提已变化 };
struct 新语言问题建立结果 final {
    新语言问题建立状态 状态=新语言问题建立状态::前提已变化;
    std::optional<稳定编码> 问题节点;
};
// 按结构比对得到的全部答案（主体、被问栏一致且晚于提问），按登记顺序；多条值不同也全部如实返回。
struct 新问题答案查询结果 final {
    std::vector<新问题答案引用> 答案组;
};
enum class 新问题答案登记状态 : std::uint8_t { 已登记=1, 精确重复, 已有其它答案, 不是该问题的答案, 前提已变化 };
struct 新问题答案登记结果 final {
    新问题答案登记状态 状态=新问题答案登记状态::前提已变化;
    std::optional<新问题答案引用> 当前答案;
};

// 语言节点登记语言身份；词面匹配只组合公开读取，不解释词义或绑定概念。
class 新_语言类 final {
public:
    新语言解释资源保护结果 读取解释资源保护(
        稳定编码 语言, const 新_词类&, const 概念_存在类&) const;
    新语言含义采用结果 建立或取得接收采用含义(
        const 新语言含义采用请求&, 新_特征类&, 概念_特征类&,
        const 新语言采用来源读取接口&);
    新语言含义采用结果 读取接收采用含义(
        const 新接收认知来源键&, const 新_特征类&) const;
    新语言含义采用结果 读取采用含义(稳定编码, const 新_特征类&) const;
    新语言需求目标投影结果 读取采用需求目标(稳定编码, const 新_特征类&) const;
    新语言需求目标投影结果 读取采用指定需求目标(
        稳定编码, const 新接收认知来源键&, const 新_特征类&) const;
    新语言行动请求投影结果 读取采用行动请求保护(
        稳定编码, 稳定编码 预期受请求者, const 新_特征类&, const 新_存在类&) const;
    // 只做第一阶段 UTF-8 合法性校验，不改变输入字节。
    static bool 严格UTF8字节串有效(const std::string& 文本) noexcept;

    bool 初始化() noexcept;

    // 独立来源结构；不依赖完整含义、词条或被认知需求，不隐式初始化。
    新语言分界工程状态 初始化分界结构();
    新语言分界调用结果<新语言前置来源信息> 建立或取得前置来源(
        const 新语言前置来源请求&, const 新语言分界消息读取接口&);
    新语言分界调用结果<新语言前置来源信息> 读取前置来源(
        const 新接收认知来源键&, const 新语言分界消息读取接口&) const;

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
    void 初始化表达组织知识结构();
    新表达组织知识信息 建立或取得表达组织知识(const 新表达组织知识定义&);
    新表达组织知识候选 查询适用表达组织知识(const 新表达需求类型&) const;
    新表达组织知识信息 读取表达组织知识(稳定编码) const;
    std::optional<新出站具体表达信息> 建立或取得出站具体表达(稳定编码 场景,
        const 新语言含义网&,稳定编码 所用组织知识,const std::string& 原文,稳定编码 来源);
    std::optional<新出站具体表达信息> 读取出站具体表达(稳定编码 场景或来源) const;
    // ASK-READ-SCENE v0.4 §3.3：每条来源消息至多一条场景信息；同内容重复为精确重复，不同内容为来源冲突。
    新接收场景信息保存结果 建立或取得接收场景信息(const 新接收场景信息&);
    // 只读：某交互场景内某说话者在起始时刻（不含）之后的值陈述，按登记顺序；不含招呼。
    std::vector<新值陈述信息> 查询值陈述(稳定编码 交互场景, 稳定编码 说话者, std::int64_t 起始时刻) const;
    // ASK-READ-SCENE v0.5 §3.2 自我写入：每个写入来源至多一条；同内容重复为精确重复，不同内容为来源冲突。
    新自我写入信息保存结果 建立或取得自我写入信息(const 新自我写入语言信息&);
    // ASK-READ-SCENE v0.5 §3.2 运行1：出站问句留档后建立“问题”；返回问题节点。
    新语言问题建立结果 建立或取得出站问题(const 新语言问题定义&);
    std::optional<新语言问题信息> 读取问题(稳定编码 问题节点) const;
    // 只读：按出站来源（询问执行实例或直接调用身份）读取问题；每个出站来源至多一个问题。
    std::optional<新语言问题信息> 按出站来源读取问题(稳定编码 出站来源) const;
    // 只读：同一被认知需求、主体与被问栏的全部问题，按登记顺序（供“该问题已有答案则不问”核对）。
    std::vector<新语言问题信息> 查询同结构问题(稳定编码 被认知需求, 稳定编码 主体, 稳定编码 被问栏) const;
    // 只读运行2：按结构比对找答案——值陈述与自我写入信息中主体存在＝问题主体、特征类型＝被问栏、时刻晚于提问；
    // 值陈述主体待定时，说话者是该问题所在交互的交互者（同交互场景）、特征类型＝被问栏、时刻晚于提问也算。
    // 问题节点不在登记内属工程故障（抛出）。
    新问题答案查询结果 查找问题答案(稳定编码 问题节点) const;
    // 写入运行3：把问题“答案”写为引用该答案信息并记录来源；答案须在查找问题答案的结果内。
    新问题答案登记结果 登记问题答案(稳定编码 问题节点, 稳定编码 答案信息节点, 新问题答案来源 来源种类);
    // 运行4就绪通知端口（键：问题节点）：值陈述或自我写入信息新保存后，对每个结构一致、晚于提问且尚无答案的问题
    // 在语言锁外各通知一次。只提示重读，不携带结论。一个所有者；解除时等待在途投递结束。
    bool 绑定问题答案就绪通知(const void* 所有者, std::function<void(稳定编码 问题节点)> 通知) noexcept;
    void 解除问题答案就绪通知(const void* 所有者) noexcept;
    新语言知识来源结果 建立或取得知识先天来源(const 新语言知识来源请求& 请求);
    新语言知识来源结果 获取知识先天来源(稳定编码 来源节点) const;
    新语言知识来源保护结果 读取知识先天来源保护(稳定编码 来源节点) const;
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

    // 明确登记尚未具备可执行合同的功能词/构式缺口；幂等按(语言,编号,版本)复用。
    新语言知识缺口结果 建立或取得语言知识缺口(
        稳定编码 语言节点, const 新语言知识缺口定义& 定义,
        const std::string& 种子版本, const 新_词类& 词服务);
    新语言知识缺口结果 获取语言知识缺口(稳定编码 缺口节点) const;
    新语言知识缺口列表结果 枚举语言知识缺口(
        稳定编码 语言节点, std::size_t 最大条目数) const;

    // —— LANGUAGE-STRUCTURE-CONCEPT（7270 / nl-seed-v0.4）——
    // 结构概念挂语言知识；含义角色登记后不可改签名；表达模式自有槽位+映射。
    // 语言结构对应表为索引视图：仅经本类维护/查询，内部只用全局基础数据集。

    新语言结构概念结果 建立或取得结构概念(
        const 新语言结构概念定义& 定义, const std::string& 种子版本);
    新语言结构概念结果 获取结构概念(稳定编码 节点) const;
    新语言结构概念列表结果 枚举结构概念(std::size_t 最大条目数) const;
    新语言结构概念列表结果 枚举全部结构概念() const;

    新语言结构定义集结果 建立或取得结构定义集(
        const 新语言结构定义集发布请求&, const 新_词类&);
    新语言结构定义集结果 获取结构定义集(稳定编码 节点, const 新_词类& 词服务) const;
    新语言结构采用结果 采用结构定义集(
        const 新语言结构采用请求&, const 新_词类& 词服务);
    新语言结构采用结果 读取当前结构采用(稳定编码 语言节点, const 新_词类& 词服务) const;

    新语言表达模式结果 建立或取得表达模式(
        稳定编码 语言节点, 稳定编码 结构概念节点,
        const 新语言表达模式定义& 定义, const std::string& 种子版本,
        const 新_词类& 词服务);
    新语言表达模式结果 获取表达模式(稳定编码 节点, const 新_词类& 词服务) const;
    新语言表达模式列表结果 枚举表达模式(
        稳定编码 语言节点, std::size_t 最大条目数, const 新_词类& 词服务) const;
    新语言表达模式列表结果 枚举全部表达模式(
        稳定编码 语言节点, const 新_词类& 词服务) const;

    新语言语法规则结果 建立或取得语法规则(
        稳定编码 语言节点, const 新语言语法规则定义& 定义,
        const std::string& 种子版本);
    新语言语法规则结果 获取语法规则(稳定编码 节点) const;
    新语言语法规则列表结果 枚举语法规则(
        稳定编码 语言节点, std::size_t 最大条目数) const;
    新语言语法规则列表结果 枚举全部语法规则(稳定编码 语言节点) const;

    新语言量词搭配结果 建立或取得量词搭配(
        稳定编码 语言节点, const std::string& 量词词面,
        const std::string& 种子版本, bool 是回退默认);
    新语言量词搭配列表结果 枚举量词搭配(
        稳定编码 语言节点, std::size_t 最大条目数) const;
    新语言量词搭配列表结果 枚举全部量词搭配(稳定编码 语言节点) const;
    // 理解侧：数+未知量词+名 → 缺口+样例；生成未知搭配回退「个」并回显。
    新语言量词缺口结果 建立或取得量词缺口(
        稳定编码 语言节点, const std::string& 量词词面,
        const std::string& 样例原文, const std::string& 种子版本);
    新语言量词缺口列表结果 枚举量词缺口(
        稳定编码 语言节点, std::size_t 最大条目数) const;
    新语言量词缺口列表结果 枚举全部量词缺口(稳定编码 语言节点) const;
    std::string 解析生成用量词(
        稳定编码 语言节点, const std::optional<std::string>& 期望量词,
        bool& 采用了回退) const;

    // 只核验当前已采用的同版本完整视图；单项新知识不会隐式加入采用。
    新语言结构对应重建结果 重建语言结构对应表(
        稳定编码 语言节点, const std::string& 种子版本, const 新_词类& 词服务);
    新语言结构对应查询结果 查询结构对应_生成(
        const 新语言结构生成查询& 查询, const 新_词类& 词服务) const;
    新语言结构对应查询结果 查询结构对应_理解(
        const 新语言结构理解查询& 查询, const 新_词类& 词服务) const;
    新语言结构对应列表结果 查询结构对应_覆盖缺口(
        const 新语言结构覆盖查询& 查询, const 新_词类& 词服务) const;
    新语言结构对应列表结果 查询结构对应_跨语言(
        稳定编码 结构概念节点, const 新_词类& 词服务) const;
    新语言结构对应列表结果 枚举结构对应行(
        稳定编码 语言节点, std::size_t 最大条目数, const 新_词类& 词服务) const;

    // 保留既有有限任务问句派生登记；不补造六项派生含义，不扩大当前采用集。
    新语言结构对应重建结果 派生任务询问对应行(
        稳定编码 语言节点, const std::string& 种子版本, const 新_词类& 词服务);

    // 显式登记种类×细目×语言无模式的对应缺口行。
    新语言结构对应结果 建立或取得结构对应缺口(
        稳定编码 语言节点, 新语言结构信息种类 种类,
        新语言结构目的细目 细目, const std::string& 种子版本);

};



} // namespace 海中鱼巣
