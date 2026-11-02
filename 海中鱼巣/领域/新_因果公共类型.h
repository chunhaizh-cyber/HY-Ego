#pragma once

#include <cstdint>
#include <optional>
#include <vector>
#include <utility>

#include "新_状态类.h"
#include "新_内部治理对象引用.h"

namespace 海中鱼巣
{

enum class 新因果类型 : std::int64_t
{
    未知动作 = 1,
    已知动作 = 2
};

enum class 新因果动作触发方式 : std::int64_t
{
    尚未确定 = 0,
    主动调用 = 1,
    被动触发 = 2
};

enum class 新因果形成来源 : std::int64_t
{
    结果状态筛选 = 1,
    结果动态反向 = 2,
    动作事实反向 = 3,
    外部动作动态反向 = 4
};

enum class 新因果操作状态 : std::uint8_t
{
    已记录首例 = 1,
    已建立 = 2,
    已更新 = 3,
    已找到 = 4,
    无匹配 = 5,
    重复发生 = 6,
    入口拒绝 = 7,
    尚未初始化 = 8,
    状态不存在 = 9,
    动态不存在 = 10,
    动作证据不存在 = 11,
    依赖未配置 = 12,
    无共同条件 = 13,
    因果不存在 = 14,
    计数溢出 = 15,
    结构不一致 = 16,
    资源失败 = 17
};

// 状态模式保留实际参与者、特征类型和当时准确值。后续抽象可以把它
// 提升为存在概念或特征值域，但不能丢失本次形成因果的原始含义。
struct 新因果状态模式 final
{
    稳定编码 被描述存在节点;
    稳定编码 特征类型概念节点;
    新特征准确值 准确值;
    friend bool operator==(const 新因果状态模式 &, const 新因果状态模式 &) = default;
};

struct 新因果动态模式 final
{
    稳定编码 主题存在节点;
    稳定编码 动态概念节点;
    friend bool operator==(const 新因果动态模式 &, const 新因果动态模式 &) = default;
};

// 动作必须有执行主体。作用部位、作用对象和本能方法只在已知时保存。
struct 新因果动作定义 final
{
    稳定编码 动作概念节点;
    稳定编码 执行主体存在节点;
    std::optional<稳定编码> 作用部位存在节点;
    std::optional<稳定编码> 作用对象存在节点;
    std::optional<稳定编码> 本能方法节点;
    std::optional<新任务虚拟存在引用> 作用对象任务虚拟存在;
    新因果动作触发方式 触发方式 = 新因果动作触发方式::尚未确定;
    friend bool operator==(const 新因果动作定义 &, const 新因果动作定义 &) = default;
};

struct 新因果定义 final
{
    新因果类型 类型 = 新因果类型::未知动作;
    std::vector<新因果状态模式> 条件状态组;
    std::vector<新因果动态模式> 条件动态组;
    std::optional<新因果动作定义> 动作;
    新因果状态模式 结果状态;
    friend bool operator==(const 新因果定义 &, const 新因果定义 &) = default;
};

// 发生身份由上层为同一次现实发生统一提供。不同发现路径使用相同身份，
// 从而保证一次发生最多计数一次。
struct 新因果发生记录 final
{
    稳定编码 发生身份;
    稳定编码 场景节点;
    std::vector<稳定编码> 前置状态节点组;
    std::vector<稳定编码> 前置动态节点组;
    稳定编码 结果状态节点;
    std::optional<稳定编码> 结果动态节点;
    std::optional<稳定编码> 动作证据节点;
};

struct 新因果统计 final
{
    std::int64_t 符合出现次数 = 0;
    std::int64_t 不符合出现次数 = 0;
    std::int64_t 自我尝试次数 = 0;
    std::int64_t 自我尝试符合次数 = 0;
    friend bool operator==(const 新因果统计 &, const 新因果统计 &) = default;
};

enum class 新先天因果角色种类 : std::uint8_t { 普通存在 = 1, 任务信息 = 2 };
struct 新先天因果角色 final {
    std::int64_t 序号;
    新先天因果角色种类 种类;
    稳定编码 范围概念;
    friend bool operator==(const 新先天因果角色&, const 新先天因果角色&) = default;
};
enum class 新先天因果条件种类 : std::uint8_t {
    存在有效 = 1, 集合包含角色, 任务虚拟存在归属, 特征值域,
    需求任务关联, 直接父子关联, 完成执行评估一致, 完成来源一致,
    直接子接收完整, 人类确认锚点, 结果观察及归因,
    本来源未消费, 下层贡献未消费, 当前治理边界一致
};
struct 新先天因果条件 final {
    std::int64_t 序号;
    新先天因果条件种类 种类;
    稳定编码 关系概念;
    std::vector<std::int64_t> 角色组;
    std::vector<稳定编码> 概念参数组;
    friend bool operator==(const 新先天因果条件&, const 新先天因果条件&) = default;
};
struct 新先天因果加法结果 final {
    std::int64_t 结果存在角色, 加数任务角色;
    稳定编码 结果特征类型, 结果合法值域, 加数特征类型, 加数合法值域;
    friend bool operator==(const 新先天因果加法结果&, const 新先天因果加法结果&) = default;
};
struct 新先天因果定义 final {
    稳定编码 治理定义, 合同版本概念, 作用场景范围概念, 动作概念;
    新因果动作触发方式 触发方式 = 新因果动作触发方式::尚未确定;
    std::vector<新先天因果角色> 角色组;
    std::vector<新先天因果条件> 全部条件;
    新先天因果加法结果 唯一结果;
    friend bool operator==(const 新先天因果定义&, const 新先天因果定义&) = default;
};
struct 新先天因果信息 final {
    稳定编码 节点;
    新先天因果定义 定义;
    新因果统计 统计;
    std::vector<稳定编码> 发生身份组;
    friend bool operator==(const 新先天因果信息&, const 新先天因果信息&) = default;
};
struct 新先天因果概念组 final {
    稳定编码 合同版本概念;
    std::vector<std::pair<新先天因果条件种类, 稳定编码>> 关系概念组;
    friend bool operator==(const 新先天因果概念组&, const 新先天因果概念组&) = default;
};

enum class 新关系先天治理操作 : std::int64_t { 同步正式服务任务投影=1, 发布自我任务完成决议=2 };
enum class 新关系先天角色类别 : std::int64_t { 普通存在=1, 需求信息=2, 任务信息=3, 执行实例=4, 影响评估=5 };
struct 新关系先天角色 final {
    std::int64_t 序号;
    新关系先天角色类别 类别;
    稳定编码 范围概念;
    friend bool operator==(const 新关系先天角色&,const 新关系先天角色&)=default;
};
enum class 新关系先天条件种类 : std::int64_t {
    角色有效=1, 自我服务投影绑定, 需求父链归根, 排除需求有父,
    投影源对应完整, 当前引用集合合法, 当前集合等于投影, 投影待写入,
    需求准备非待完善, 任务关联需求, 任务最近执行, 执行关联任务,
    执行关联评估, 评估关联任务, 评估关联执行, 评估允许完成,
    自我接受完成决议, 任务状态属于, 执行状态属于
};
struct 新关系先天条件 final {
    std::int64_t 序号;
    新关系先天条件种类 种类;
    稳定编码 关系概念;
    std::vector<std::int64_t> 角色组;
    std::vector<稳定编码> 概念参数组;
    friend bool operator==(const 新关系先天条件&,const 新关系先天条件&)=default;
};
enum class 新关系先天结果种类 : std::int64_t { 正式服务任务投影=1, 特征值域=2 };
enum class 新关系先天结果变化 : std::int64_t { 改变=1, 保持=2 };
struct 新关系先天结果 final {
    新关系先天结果种类 种类;
    新关系先天结果变化 变化;
    std::int64_t 结果存在角色;
    稳定编码 结果特征类型,结果范围概念;
    std::vector<std::int64_t> 来源角色组;
    稳定编码 结果关系概念;
    friend bool operator==(const 新关系先天结果&,const 新关系先天结果&)=default;
};
struct 新关系先天因果定义 final {
    稳定编码 规则概念,治理定义,作用场景范围概念,动作概念;
    新因果动作触发方式 触发方式=新因果动作触发方式::被动触发;
    std::vector<新关系先天角色> 角色组;
    std::vector<新关系先天条件> 全部条件;
    新关系先天结果 唯一结果;
    friend bool operator==(const 新关系先天因果定义&,const 新关系先天因果定义&)=default;
};
struct 新关系先天因果信息 final {
    稳定编码 节点;
    新关系先天因果定义 定义;
    新因果统计 统计;
    std::vector<稳定编码> 发生身份组;
    friend bool operator==(const 新关系先天因果信息&,const 新关系先天因果信息&)=default;
};
struct 新关系先天元数据请求 final {
    稳定编码 自我概念,需求概念,任务概念;
    稳定编码 服务任务投影场景范围概念,任务治理场景范围概念;
    稳定编码 服务任务特征类型,任务状态特征类型;
    稳定编码 等待自我决议值域,已完成值域;
    friend bool operator==(const 新关系先天元数据请求&,const 新关系先天元数据请求&)=default;
};
struct 新关系先天元数据 final {
    新关系先天元数据请求 材料;
    std::vector<std::pair<新关系先天治理操作,稳定编码>> 治理定义组;
    std::vector<稳定编码> 规则概念组;
    std::vector<std::pair<新关系先天条件种类,稳定编码>> 关系概念组;
    std::vector<std::pair<新关系先天角色类别,稳定编码>> 角色范围组;
    稳定编码 执行已评估范围,完成决议范围,投影结果关系,状态结果关系;
    friend bool operator==(const 新关系先天元数据&,const 新关系先天元数据&)=default;
};

enum class 新来源前驱治理步骤 : std::uint8_t { 自我采用请求=1,管理关联任务 };
enum class 新来源前驱发生分支 : std::uint8_t { 新建=1,复用 };
enum class 新来源前驱求值阶段 : std::uint8_t { 动作前输入=1,动作接纳,动作后结果 };
enum class 新来源前驱角色 : std::uint8_t {
    自我=1,人类,接收消息,采用含义,被认知需求,自我需求,任务,完成服务父需求,认知出现定位
};
enum class 新来源前驱条件种类 : std::uint8_t {
    角色合法=1,人类主体,被认知需求归属,需求完整,正式行动请求,
    请求受话者,请求内容同需求,合法完成服务父域,同源自我需求尚无,
    同源自我需求已存在,自我请求采用机制,自我需求来源合法,
    自我需求活动且准备允许,管理承接机制,需求尚无任务,需求已有唯一任务,
    主体需求引用,来源自我需求对应,投影源条目对应
};
enum class 新来源前驱关系结果 : std::uint8_t { 同源自我需求=1,需求唯一任务 };
struct 新来源前驱条件 final {
    新来源前驱求值阶段 阶段{};
    std::uint32_t 序号=0;
    新来源前驱条件种类 种类{};
    稳定编码 关系概念;
    std::vector<新来源前驱角色> 角色组;
    std::vector<稳定编码> 概念参数组;
    friend bool operator==(const 新来源前驱条件&,const 新来源前驱条件&)=default;
};
struct 新服务取得来源前驱配置 final {
    稳定编码 自我概念,人类概念,需求概念,任务概念;
    稳定编码 交互者类型,交互者人类范围,对象需求类型,完整被认知需求概念,服务任务类型;
    稳定编码 自我采用场景范围,管理关联场景范围;
    friend bool operator==(const 新服务取得来源前驱配置&,const 新服务取得来源前驱配置&)=default;
};
struct 新来源前驱规则定义 final {
    稳定编码 规则概念,治理定义,动作概念,场景范围;
    新来源前驱治理步骤 步骤{};
    新来源前驱发生分支 分支{};
    新因果动作触发方式 触发方式=新因果动作触发方式::被动触发;
    std::vector<std::pair<新来源前驱角色,稳定编码>> 角色范围组;
    // 新建输出只能在动作后引入，不并入动作前输入AND。
    std::vector<新来源前驱角色> 输入角色组,动作引入角色组;
    std::vector<新来源前驱条件> 输入条件组,接纳核验组,结果校验组;
    新来源前驱关系结果 结果{};
    稳定编码 结果关系概念;
    std::vector<新来源前驱角色> 结果角色组;
    friend bool operator==(const 新来源前驱规则定义&,const 新来源前驱规则定义&)=default;
};
struct 新来源前驱规则信息 final {
    稳定编码 节点;
    新来源前驱规则定义 定义;
    新因果统计 统计;
    std::vector<稳定编码> 发生身份组;
    std::vector<基础联合节点快照> 完整保护;
    friend bool operator==(const 新来源前驱规则信息& a,const 新来源前驱规则信息& b) {
        return a.节点==b.节点&&a.定义==b.定义&&a.统计==b.统计&&a.发生身份组==b.发生身份组;
    }
};
struct 新服务取得来源前驱元数据 final {
    稳定编码 节点;
    新服务取得来源前驱配置 配置;
    std::vector<稳定编码> 规则概念组;
    std::vector<std::pair<新来源前驱治理步骤,稳定编码>> 治理定义组;
    std::vector<std::pair<新来源前驱角色,稳定编码>> 角色范围组;
    std::vector<std::pair<新来源前驱条件种类,稳定编码>> 条件关系组;
    std::vector<std::pair<新来源前驱关系结果,稳定编码>> 结果关系组;
    friend bool operator==(const 新服务取得来源前驱元数据&,const 新服务取得来源前驱元数据&)=default;
};
enum class 新来源前驱元数据读取状态 : std::uint8_t {
    已读取=1,未找到,前提已变化,依赖未就绪,资源失败
};
struct 新来源前驱链条件定义 final {
    稳定编码 所属元数据;
    新来源前驱条件种类 种类{};
    稳定编码 关系概念;
    std::vector<新来源前驱角色> 角色组;
    std::vector<稳定编码> 概念参数组;
    friend bool operator==(const 新来源前驱链条件定义&,const 新来源前驱链条件定义&)=default;
};
struct 新来源前驱元数据读取结果 final {
    新来源前驱元数据读取状态 状态=新来源前驱元数据读取状态::依赖未就绪;
    std::optional<新服务取得来源前驱元数据> 信息;
    std::vector<新来源前驱链条件定义> 链条件组;
    std::vector<基础联合节点快照> 完整保护;
    friend bool operator==(const 新来源前驱元数据读取结果& a,const 新来源前驱元数据读取结果& b) {
        return a.状态==b.状态&&a.信息==b.信息&&a.链条件组==b.链条件组;
    }
};
enum class 新来源前驱规则读取状态 : std::uint8_t {
    已读取=1,未找到,前提已变化,依赖未就绪,资源失败
};
struct 新来源前驱规则读取结果 final {
    新来源前驱规则读取状态 状态=新来源前驱规则读取状态::依赖未就绪;
    std::optional<新来源前驱规则信息> 信息;
    std::vector<基础联合节点快照> 完整保护;
};
struct 新来源前驱规则查询结果 final {
    新来源前驱规则读取状态 状态=新来源前驱规则读取状态::依赖未就绪;
    std::vector<新来源前驱规则信息> 规则组;
    std::vector<基础联合节点快照> 完整保护;
};

struct 新因果信息 final
{
    稳定编码 节点;
    新因果定义 定义;
    std::vector<新因果形成来源> 形成来源组;
    新因果统计 统计;
    std::vector<稳定编码> 已计数发生身份组;
    std::optional<稳定编码> 发生场景节点;
    friend bool operator==(const 新因果信息 &, const 新因果信息 &) = default;
};

struct 新因果概念角色
{
    std::int64_t 角色 = 0;
    稳定编码 存在概念节点;
    std::optional<新治理参与者种类> 治理类别;
    friend bool operator==(const 新因果概念角色 &, const 新因果概念角色 &) = default;
};
struct 新因果概念状态项
{
    std::int64_t 角色 = 0;
    稳定编码 特征类型概念节点;
    稳定编码 值域概念节点;
    friend bool operator==(const 新因果概念状态项 &, const 新因果概念状态项 &) = default;
};
struct 新因果概念动态项
{
    std::int64_t 角色 = 0;
    稳定编码 动态概念节点;
    friend bool operator==(const 新因果概念动态项 &, const 新因果概念动态项 &) = default;
};
struct 新因果概念动作项
{
    稳定编码 动作概念节点;
    std::int64_t 执行主体角色 = 0;
    std::optional<std::int64_t> 作用部位角色, 作用对象角色;
    新因果动作触发方式 触发方式 = 新因果动作触发方式::尚未确定;
    friend bool operator==(const 新因果概念动作项 &, const 新因果概念动作项 &) = default;
};
struct 新因果概念分支
{
    稳定编码 来源具体因果节点;
    std::vector<新因果概念状态项> 条件状态组;
    std::vector<新因果概念动态项> 条件动态组;
    新因果概念状态项 结果状态;
    friend bool operator==(const 新因果概念分支 &, const 新因果概念分支 &) = default;
};
struct 新概念因果定义
{
    新因果类型 类型 = 新因果类型::未知动作;
    稳定编码 场景概念节点;
    std::vector<新因果概念角色> 角色组;
    std::optional<新因果概念动作项> 动作;
    std::vector<新因果概念分支> 分支组;
    friend bool operator==(const 新概念因果定义 &, const 新概念因果定义 &) = default;
};
enum class 新因果共同抽象状态 : std::uint8_t
{
    已建立 = 1,
    已复用,
    来源不足,
    场景来源不足,
    无结构共性,
    角色对齐不唯一,
    无共同存在概念,
    精确值域尚不可表达,
    值不在合法类型域内,
    已读取,
    概念已不相容
};
struct 新概念因果信息
{
    稳定编码 节点;
    新概念因果定义 定义;
    std::vector<稳定编码> 来源发生身份组;
    std::int64_t 覆盖发生次数 = 0;
    friend bool operator==(const 新概念因果信息 &, const 新概念因果信息 &) = default;
};
struct 新因果共同抽象结果
{
    新因果共同抽象状态 状态 = 新因果共同抽象状态::来源不足;
    std::optional<新概念因果信息> 信息;
    friend bool operator==(const 新因果共同抽象结果 &, const 新因果共同抽象结果 &) = default;
};

struct 新因果处理结果 final
{
    新因果操作状态 状态 = 新因果操作状态::入口拒绝;
    std::optional<稳定编码> 因果节点;
    std::optional<新因果信息> 因果信息;
};

struct 新因果查询结果 final
{
    新因果操作状态 状态 = 新因果操作状态::入口拒绝;
    std::vector<稳定编码> 因果节点组;
};

struct 新因果世界范围 final
{
    稳定编码 世界根节点;
    std::vector<稳定编码> 具体因果节点组, 概念因果节点组;
    friend bool operator==(const 新因果世界范围 &, const 新因果世界范围 &) = default;
};

struct 新因果方法参考 final
{
    稳定编码 因果节点;
    稳定编码 本能方法节点;
    新因果动作定义 动作;
    新因果统计 统计;
    friend bool operator==(const 新因果方法参考 &, const 新因果方法参考 &) = default;
};

// 动作事实和外部动作识别尚未进入新版数据类，因此以显式依赖端口接入。
// 未装配端口时，动作路径必须返回“依赖未配置”。
class 新因果动作证据提供者
{
  public:
    virtual ~新因果动作证据提供者() = default;
    virtual std::optional<新因果动作定义> 读取自我动作事实(
        稳定编码 动作事实节点) const noexcept = 0;
    virtual std::optional<新因果动作定义> 推断外部动作(稳定编码 动作动态节点) const noexcept = 0;
};

} // namespace 海中鱼巣
