#pragma once

#include "../领域/新_词类.h"
#include "../领域/新_语言类.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace 海中鱼巣 {

class 概念_存在类;
class 概念_名称关系类;

struct 初始化语言知识请求 final {
    std::string 标准文本;
    稳定编码 语言节点, 规范化合同节点;
};

enum class 初始化语言知识状态 : std::uint8_t {
    已完成 = 1, 文本未完成, 定义拒绝, 语言未完成,
    词条未完成, 来源未完成, 知识未完成, 读回未完成
};
enum class 初始化语言知识阶段 : std::uint8_t {
    整批解码预检, 结构初始化, 功能词处理, 来源处理, 知识处理, 缺口处理,
    结构概念处理, 对应表重建, 完整读回, 完成
};
enum class 初始化语言知识工程控制 : std::uint8_t { 已完成 = 1, 资源未完成 };
enum class 初始化语言知识额度种类 : std::uint8_t {
    源字节 = 1, 源标量, 非空行, 知识块, 单块角色, 单块参数, 单块结构项
};
struct 初始化语言知识词项 final {
    std::string 词面;
    std::optional<新词条建立结果> 发布;
    std::optional<新词条查询结果> 读回;
};
struct 初始化语言知识项 final {
    std::string 来源标签;
    std::int64_t 起始标量 = 0, 结束标量 = 0;
    std::optional<新语言知识定义> 定稿;
    std::optional<新语言知识结果> 发布;
    std::optional<新语言知识结果> 读回;
};
struct 初始化语言知识缺口项 final {
    新语言知识缺口定义 定义;
    std::optional<新语言知识缺口结果> 发布;
    std::optional<新语言知识缺口结果> 读回;
};
struct 初始化语言知识结果 final {
    初始化语言知识工程控制 工程控制 = 初始化语言知识工程控制::已完成;
    std::optional<初始化语言知识状态> 状态;
    std::optional<初始化语言知识额度种类> 到限额度;
    初始化语言知识阶段 阶段 = 初始化语言知识阶段::整批解码预检;
    std::string 种子版本;
    std::vector<初始化语言知识词项> 词项组;
    std::optional<新语言知识来源结果> 来源发布, 来源读回;
    std::optional<新语言结构定义集结果> 结构定义集发布, 结构定义集读回;
    std::optional<新语言结构采用结果> 结构采用发布, 结构采用读回;
    std::vector<稳定编码> 受阻节点组;
    std::vector<初始化语言知识项> 知识项组;
    std::vector<初始化语言知识缺口项> 缺口项组;
    std::size_t 结构概念数 = 0;
    std::size_t 表达模式数 = 0;
    std::size_t 语法规则数 = 0;
    std::size_t 量词搭配数 = 0;
    std::size_t 对应表已登记数 = 0;
    std::size_t 对应表缺口数 = 0;
    std::size_t 对应表派生数 = 0;
    std::optional<std::size_t> 当前项;
    std::string 诊断说明;
};

// 先整批解码及结构预检，再依次发布词、来源及知识；跨owner不作虚假回滚。
// 工程异常向上送出，由生产装配的最终工程边界诊断并终止。
初始化语言知识结果 初始化最小语言知识(
    const 初始化语言知识请求& 请求, 新_语言类& 语言服务, 新_词类& 词服务,
    概念_存在类& 存在概念服务, 概念_名称关系类& 名称服务);

// 完整复核同一份固定原文；零CRUD，发布字段始终为空，不修补缺失。
初始化语言知识结果 读取最小语言知识(
    const 初始化语言知识请求& 请求, const 新_语言类& 语言服务,
    const 新_词类& 词服务, const 概念_存在类& 存在概念服务,
    const 概念_名称关系类& 名称服务);

} // namespace 海中鱼巣
