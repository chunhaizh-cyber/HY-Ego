#pragma once
#include "服务要求.材料合同.h"
#include "新_自然语言世界树类.h"

namespace 海中鱼巣 {
enum class 新模拟需求来源种类 : std::uint8_t { 概念构造=1, 单向复制 };
enum class 新模拟需求状态 : std::uint8_t {
    已完成=1, 未找到, 输入不合法, 正式定义未就绪, 关系材料不足,
    引用类别未支持, 前提已变化, 资源失败, 内部不一致
};
struct 新模拟引用内容定位输入 final {
    std::uint32_t 引用需求输入=0;
    std::vector<稳定编码> 正式内容路径;
};
struct 新模拟需求输入 final {
    std::uint32_t 序号=0, 主体存在输入=0, 目标存在输入=0;
    稳定编码 内容概念;
    std::vector<稳定编码> 附加内容概念组;
    新需求目标定义 目标;
    新需求执行域 执行域{};
    std::int64_t 权重=0;
    新需求活动状态 活动状态{};
    std::optional<std::uint32_t> 父需求输入;
    std::optional<新需求根角色> 根角色;
    std::optional<新需求来源类型> 来源类型;
    std::uint32_t 理解输入=0, 确认输入=0, 来源要求输入=0, 外部要求输入=0;
    std::vector<std::uint32_t> 服务要求确认输入组;
    新模拟需求来源种类 来源=新模拟需求来源种类::概念构造;
    std::optional<稳定编码> 来源需求;
    std::optional<新模拟引用内容定位输入> 引用内容定位输入;
    新需求信息完善阶段 信息完善阶段=新需求信息完善阶段::已完善;
    std::optional<新部分服务要求目标> 已知目标;
};
struct 新模拟需求信息 final {
    稳定编码 节点, 内容概念, 需求主体存在节点, 目标存在节点;
    std::vector<稳定编码> 附加内容概念组;
    新需求目标定义 目标;
    新需求执行域 执行域{};
    std::int64_t 权重=0;
    新需求活动状态 活动状态{};
    std::optional<稳定编码> 父需求节点;
    std::optional<新需求根角色> 根角色;
    std::optional<新需求来源类型> 来源类型;
    std::optional<新需求认知来源> 认知来源;
    std::vector<稳定编码> 服务要求确认组;
    std::optional<稳定编码> 外部要求节点;
    新需求信息完善阶段 信息完善阶段=新需求信息完善阶段::已完善;
    std::optional<新部分服务要求目标> 已知目标;
    friend bool operator==(const 新模拟需求信息&,const 新模拟需求信息&)=default;
};
struct 新模拟需求读回 final {
    新模拟需求状态 状态=新模拟需求状态::未找到;
    std::optional<新模拟需求信息> 信息;
};
struct 新模拟理解输入 final {
    std::uint32_t 序号=0, 语言场景输入=0, 来源存在输入=0, 目标存在输入=0, 来源要求输入=0;
    std::string 公开会话标识;
    新需求目标定义 目标;
    服务要求目标语义快照 固定语义;
    std::optional<稳定编码> 来源身份;
};
struct 新模拟确认输入 final {
    std::uint32_t 序号=0, 理解输入=0;
    bool 需重新确认=false;
    std::optional<稳定编码> 来源身份;
};
struct 新模拟要求输入 final {
    std::uint32_t 序号=0, 来源存在输入=0;
    std::optional<std::uint32_t> 目标存在输入;
    std::string 公开会话标识, 来源要求标识;
    新部分服务要求目标 已知目标;
    std::uint32_t 理解输入=0, 确认输入=0;
    std::optional<稳定编码> 来源身份;
};
struct 新模拟语言实例输入 final {
    std::uint32_t 存在输入=0;
    稳定编码 概念;
    新自然语言概念实例种类 种类{};
};
struct 新模拟语言场景输入 final {
    std::uint32_t 序号=0;
    std::optional<稳定编码> 专属存在概念;
    std::vector<std::uint32_t> 特征输入组, 成员存在输入组, 边界存在输入组;
    std::vector<新模拟语言实例输入> 实例组;
    std::optional<稳定编码> 来源身份;
};
struct 新模拟需求输入图 final {
    std::vector<新模拟需求输入> 需求组;
    std::vector<新模拟理解输入> 理解组;
    std::vector<新模拟确认输入> 确认组;
    std::vector<新模拟要求输入> 要求组;
    std::vector<新模拟语言场景输入> 语言场景组;
    std::vector<基础联合节点快照> 正式定义保护;
    基础只读预期 存在范围语义保护;
};
template<class T> struct 新模拟需求关联读回 final {
    新模拟需求状态 状态=新模拟需求状态::未找到;
    std::optional<T> 信息;
};
struct 新模拟语言场景信息 final {
    稳定编码 节点;
    std::optional<稳定编码> 专属存在概念;
    std::vector<稳定编码> 特征节点组, 直接成员存在组, 边界存在组;
    std::vector<新自然语言概念实例信息> 概念实例组;
};
template<class T> struct 新模拟需求来源结果 final {
    新模拟需求状态 状态=新模拟需求状态::未找到;
    std::optional<T> 信息;
    std::vector<基础联合节点快照> 完整保护;
};
struct 新模拟需求构造依据 final { 稳定编码 人类概念; };
enum class 新模拟复制值种类 : std::uint8_t { I64=1, 普通材料, 存在引用, 需求引用, 存在集合, 需求集合, 任务集合=7, 词典材料引用=8 };
struct 新模拟词典材料引用 final {
    std::uint32_t 材料输入=0; // 完整材料输入组中的1起位置，不是现实Q编码。
    friend bool operator==(const 新模拟词典材料引用&,const 新模拟词典材料引用&)=default;
};
struct 新模拟复制准确值 final {
    新模拟复制值种类 种类;
    std::variant<std::int64_t,新特征值信息,std::uint32_t,std::vector<std::uint32_t>,新模拟词典材料引用> 内容;
    friend bool operator==(const 新模拟复制准确值&,const 新模拟复制准确值&)=default;
};
struct 新模拟复制值命中 final { 新模拟复制准确值 值; std::int64_t 命中次数=0; friend bool operator==(const 新模拟复制值命中&,const 新模拟复制值命中&)=default; };
struct 新模拟复制值域 final { std::variant<特征概念值域,std::vector<新模拟复制准确值>> 内容; friend bool operator==(const 新模拟复制值域&,const 新模拟复制值域&)=default; };
struct 新模拟完整特征复制输入 final {
    std::optional<稳定编码> 实例单位;
    std::optional<新模拟复制值域> 实例区间, 历史聚合;
    std::optional<新模拟复制准确值> 当前值, 标准值;
    std::vector<新模拟复制准确值> 历史不同值;
    std::vector<新模拟复制值命中> 历史值命中信息;
    friend bool operator==(const 新模拟完整特征复制输入&,const 新模拟完整特征复制输入&)=default;
};
} // namespace 海中鱼巣
