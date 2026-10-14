#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include "../领域/概念_存在类.h"

namespace 海中鱼巣 {

enum class 初始化最小模板 : std::uint8_t {
    概念归类, 特征整数, 特征概念, 组成定义, 显式别名, 结构化分量区间,
    指代定义声明
};
enum class 初始化定义作用域 : std::uint8_t { 概念定义声明 };
enum class 初始化符号角色类型 : std::uint8_t { 概念引用, 特征类型引用, 指代概念引用 };
enum class 初始化源角色 : std::uint8_t {
    下位概念, 上位概念, 主体概念, 特征类型,
    整数值, 特征值概念, 部分概念, 整体概念, 别名词面,
    分量序号, 区间下界, 区间上界,
    指代概念, 指代声明规则, 指代目标范围, 指代参数角色,
    指代参数类型, 指代特征约束
};

// 行号1基；字节、Unicode标量均为整份固定原文中的0基半开范围。
struct 初始化源范围 final {
    std::size_t 行号 = 1;
    std::size_t 字节起点 = 0, 字节终点 = 0;
    std::size_t 标量起点 = 0, 标量终点 = 0;
};
// 仅定位本次材料的符号组，不是正式概念、特征类型或词条身份。
struct 初始化符号引用 final {
    std::size_t 局部编号 = 0;
};
struct 初始化词面符号 final {
    初始化符号引用 引用;
    初始化符号角色类型 角色类型 = 初始化符号角色类型::概念引用;
    std::string 词面;
    初始化源范围 来源;
};
struct 初始化角色来源 final {
    初始化源角色 角色;
    std::size_t 行内词序 = 0;
    初始化源范围 来源;
};
struct 初始化概念归类材料 final { 初始化符号引用 下位, 上位; };
struct 初始化特征定义材料 final {
    初始化符号引用 主体概念, 特征类型;
    std::variant<std::int64_t, 初始化符号引用> 值;
};
struct 初始化组成定义材料 final { 初始化符号引用 部分, 整体; };
struct 初始化名称提议材料 final {
    初始化符号引用 概念;
    std::string 别名词面;
};
struct 初始化结构化分量区间材料 final {
    初始化符号引用 主体概念;
    std::int64_t 分量序号 = 0, 下界 = 0, 上界 = 0;
};
struct 初始化指代声明材料 final {
    初始化符号引用 指代概念;
    指代概念定义 定义;
};
using 初始化定义载荷 = std::variant<初始化概念归类材料,
    初始化特征定义材料, 初始化组成定义材料, 初始化名称提议材料,
    初始化结构化分量区间材料, 初始化指代声明材料>;
struct 初始化定义项 final {
    std::size_t 局部编号 = 0;
    初始化最小模板 模板;
    初始化定义作用域 作用域 = 初始化定义作用域::概念定义声明;
    初始化源范围 来源;
    std::vector<初始化角色来源> 角色来源组;
    初始化定义载荷 载荷;
};
struct 初始化定义材料 final {
    std::string 原文;
    std::vector<初始化词面符号> 符号组;
    std::vector<初始化定义项> 定义组;
};

enum class 初始化解释状态 : std::uint8_t {
    已完整解释, 空材料, 格式失败, 缺规则, 定义不合法
};
enum class 初始化工程控制 : std::uint8_t {
    已完成, 额度配置无效, 资源未完成
};
enum class 初始化诊断原因 : std::uint8_t {
    空材料, UTF8不合法, 分隔不合法, 不支持标点,
    无完整模板, 整数字面量不合法, 整数越界,
    额度配置无效, 额度到限,
    指代枚举词不合法, 指代参数不合法, 指代规则参数不符, 指代约束未支持
};
enum class 初始化额度种类 : std::uint8_t {
    源字节, 源标量, 非空行, 单行词, 单词字节, 单词标量, 角色符号
};
struct 初始化最小资源额度 final {
    std::size_t 最大源字节数 = 1024 * 1024;
    std::size_t 最大源标量数 = 262144;
    std::size_t 最大非空行数 = 4096;
    std::size_t 最大单行词数 = 16;
    std::size_t 最大单词字节数 = 1024;
    std::size_t 最大单词标量数 = 256;
    std::size_t 最大角色符号数 = 16384;
};
struct 初始化解释诊断 final {
    初始化诊断原因 原因;
    std::size_t 行号 = 1;
    std::size_t 字节起点 = 0, 字节终点 = 0;
    std::optional<std::size_t> 标量起点, 标量终点;
    std::optional<初始化额度种类> 额度种类;
};
struct 初始化最小解释结果 final {
    初始化工程控制 工程控制 = 初始化工程控制::已完成;
    std::optional<初始化解释状态> 解释状态;
    std::optional<初始化定义材料> 材料;
    std::optional<初始化解释诊断> 诊断;
};

// 无状态、零领域读写；仅整批完整解释时返回材料，符号逐出现保留、待后继绑定。
// 工程额度未完成无语义结论。分配/内部异常向上送出；正式调用方须在最终
// 工程边界调用现有工程异常终止，不能把异常转换成定义为假或继续初始化。
初始化最小解释结果 解释初始化标准文本(
    const std::string& 原文,
    const 初始化最小资源额度& 额度 = {});

} // namespace 海中鱼巣
