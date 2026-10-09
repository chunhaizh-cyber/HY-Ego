#pragma once

#include "领域/新_自然语言世界树类.h"
#include "业务/新_世界树类.h"
#include "业务/治理二次特征.应用服务.h"
#include "线程/新_自我线程类.h"
#include "线程/新_任务管理线程类.h"

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

namespace 海中鱼巣 {

inline constexpr std::uint64_t 普通应用默认邮箱容量 = 256;
inline constexpr std::uint64_t 普通应用默认资源重试间隔毫秒 = 100;
inline constexpr std::uint64_t 普通应用默认线程进入等待毫秒 = 5000;
inline constexpr std::uint64_t 普通应用默认线程停止回收等待毫秒 = 5000;

struct 普通应用配置 final {
    std::uint64_t 自我线程邮箱容量 = 普通应用默认邮箱容量;
    std::uint64_t 任务管理线程邮箱容量 = 普通应用默认邮箱容量;
    std::uint64_t 资源重试间隔毫秒 = 普通应用默认资源重试间隔毫秒;
    std::uint64_t 线程进入等待毫秒 = 普通应用默认线程进入等待毫秒;
    std::uint64_t 线程停止回收等待毫秒 =
        普通应用默认线程停止回收等待毫秒;
    // 双根权重来自具名运行配置；普通应用默认让两根等权。
    std::int64_t 安全根初始权重 = 1;
    std::int64_t 服务根初始权重 = 1;
    std::string 第一权限人登录名 = "zch2005";
    std::string 第一权限人姓名 = "周春海";

    friend bool operator==(const 普通应用配置&, const 普通应用配置&) = default;
};

enum class 普通应用装配状态 : std::uint8_t {
    已装配 = 1,
    入口拒绝,
    世界树初始化失败,
    第一权限人初始化失败,
    自我治理基础初始化失败,
    自我初始化失败,
    本能函数登记失败,
    交互窗口初始化失败,
    本能函数端口装配失败,
    线程绑定失败,
    消息配置失败,
    资源失败,
    内部不一致,
    人类概念初始化失败,
    自然语言世界初始化失败,
    确认概念初始化失败,
    治理二次特征初始化失败
};

struct 普通应用装配结果 final {
    普通应用装配状态 状态 = 普通应用装配状态::入口拒绝;
    std::optional<新世界初始化结果> 世界树;
    std::optional<新自然语言世界初始化结果> 自然语言世界树;
    std::optional<稳定编码> 自然语言世界树根节点;
    std::optional<稳定编码> 第一权限人存在节点;
    std::optional<稳定编码> 第一权限人专属存在概念节点;
    std::optional<稳定编码> 人类存在概念节点;
    std::optional<稳定编码> 登录名特征概念节点;
    std::optional<稳定编码> 姓名特征概念节点;
    std::optional<稳定编码> 权限角色特征概念节点;
    std::optional<新自我治理基础结果> 自我治理基础;
    std::optional<新自我线程初始化结果> 自我;
    std::optional<新自我线程操作结果> 任务管理绑定;
    std::optional<新自我线程操作结果> 消息配置;
    std::optional<稳定编码> 交互窗口节点;
    std::optional<稳定编码> 交互窗口消息数量特征节点;
    std::optional<稳定编码> 交互窗口最新消息特征节点;
    std::optional<稳定编码> 是否收到消息二次特征类型根;
    std::optional<稳定编码> 未收到消息结果概念;
    std::optional<稳定编码> 已收到消息结果概念;
    std::optional<稳定编码> 语言语义差异二次特征类型根;
    std::optional<稳定编码> 无法确认结果概念;
    std::optional<稳定编码> 无法确认词条;
    std::optional<稳定编码> 无法确认名称关系;
    std::optional<稳定编码> 确认结果概念;
    std::optional<稳定编码> 确认词条;
    std::optional<稳定编码> 确认名称关系;
    std::optional<治理时间概念集合> 治理时间概念;

    bool 成功() const noexcept;
};

enum class 普通应用治理操作状态 : std::uint8_t {
    已完成 = 1,
    无变化,
    上下文不存在,
    任务管理启动失败,
    自我线程启动失败,
    自我线程停止失败,
    任务管理停止失败,
    内部不一致
};

struct 普通应用治理操作结果 final {
    普通应用治理操作状态 状态 = 普通应用治理操作状态::上下文不存在;
    std::optional<新任务管理操作结果> 任务管理;
    std::optional<新自我线程操作结果> 自我线程;

    bool 成功() const noexcept;
};

enum class 普通应用网页消息状态 : std::uint8_t {
    已接纳 = 1,
    精确重复,
    上下文不存在,
    会话冲突,
    消息序号冲突,
    消息序号不连续,
    会话标识材料失败,
    会话标识概念失败,
    交互者建立失败,
    当前交互者概念失败,
    当前交互者特征失败,
    比较关系失败,
    连接需求失败,
    连接需求未满足,
    入口拒绝,
    结构不一致,
    资源失败
};

struct 普通应用网页消息处理结果 final {
    普通应用网页消息状态 状态 = 普通应用网页消息状态::入口拒绝;
    std::optional<稳定编码> 交互者存在节点;
    std::optional<稳定编码> 当前交互者特征节点;
    // 兼容既有结果布局；现行网页入口不再把瞬时连接建立成服务根需求，
    // 因而本字段固定为空。
    std::optional<稳定编码> 连接需求节点;
    std::optional<稳定编码> 交互窗口消息数量特征节点;
    std::optional<稳定编码> 是否收到消息结果概念;

    bool 成功() const noexcept;
};

// 建立新版世界树、自我所在场景、唯一自我、需求双根、方法根和线程绑定；
// 本函数不创建OS线程。
普通应用装配结果 构造普通应用上下文(
    const 普通应用配置& 配置) noexcept;
普通应用装配结果 构造普通应用上下文() noexcept;

// 只解释调用方提交的时间合同投影，不执行任务或生成时间事实。
治理时序解释结果 计算普通应用时序语义(
    const 需求时序可达性裁决请求& 请求) noexcept;

// 先启动manager（其内部启动worker），再启动self；self启动后立即执行首次
// 双根复核、需求树全局扫描和缺失任务关联交接。
普通应用治理操作结果 启动普通应用治理线程() noexcept;

// 先停止并join self，再停止并join manager及其内部worker。
普通应用治理操作结果 停止并回收普通应用治理线程() noexcept;

// 接纳网页宿主已验证的UTF-8消息。首条消息建立本会话交互者，并把自我的
// “当前交互者”特征指向该存在；每条新消息递增交互窗口累计消息数量。
// 消息只提供连接观察材料，不建立服务根需求，也不解释正文或裁决连接状态。
普通应用网页消息处理结果 接收普通应用网页消息(
    std::string_view 会话标识,
    std::uint64_t 消息序号,
    std::string_view UTF8正文) noexcept;

} // namespace 海中鱼巣
