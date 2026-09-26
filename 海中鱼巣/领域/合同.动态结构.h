#pragma once

#include <cstdint>
#include <optional>
#include <variant>
#include <vector>

#include "合同.场景状态使用绑定.h"
#include "算法.有序I64特征比较.h"

namespace 海中鱼巣 {

inline constexpr std::uint32_t 动态数据合同版本 = 1;

struct 动态信息身份 final {
    稳定编码 编码{};
    friend bool operator==(const 动态信息身份&, const 动态信息身份&) = default;
};
inline bool 有效(动态信息身份 v) noexcept { return 有效(v.编码); }

enum class 动态成员角色 : std::uint8_t { 同主体 = 1, 组成存在 = 2 };

struct 动态直接成员 final {
    std::uint32_t 顺序 = 0;
    动态成员角色 角色 = 动态成员角色::同主体;
    动态信息身份 子动态;
};

struct 动态原子内容 final { 状态使用绑定身份 前绑定, 后绑定; };
struct 动态组合内容 final { std::vector<动态直接成员> 成员组; };
using 动态变化内容 = std::variant<动态原子内容, 动态组合内容>;

struct 动态信息 final {
    动态信息身份 身份;
    稳定编码 主体存在{};
    动态变化内容 变化;
};

struct 动态读取预算 final {
    std::uint64_t 最大候选数 = 0;
    std::uint64_t 最大动态数 = 0;
    std::uint64_t 最大来源边数 = 0;
    std::uint64_t 最大深度 = 0;
    std::uint64_t 最大读取材料数 = 0;
};

struct 动态原子变化依据 final {
    特征比较执行请求 比较请求;
    特征比较执行结果 比较结果;
};

struct 动态自有事实清单 final {
    稳定编码 族归属关系{}, 主体关系{}, 首次形成UTC值{};
    std::vector<稳定编码> 变化关系组;
};

struct 动态内容事实 final {
    std::uint64_t Gread = 0;
    动态信息 信息;
    动态自有事实清单 自有事实;
    std::int64_t 首次形成UTC纳秒 = 0;
    std::uint64_t 创建事实代次 = 0;
};

struct 动态原子来源事实 final {
    动态信息身份 动态;
    状态使用绑定事实 前绑定, 后绑定;
    状态内容事实 前状态, 后状态;
};

struct 动态来源展开事实 final {
    std::uint64_t Gread = 0;
    动态信息身份 根动态;
    std::vector<动态内容事实> 动态组;
    std::vector<动态原子来源事实> 原子来源组;
};

enum class 动态数据状态 : std::uint8_t {
    已创建=1, 精确重复=2, 已读取=3, 已删除=4,
    入口拒绝=6, 未找到=7,
    来源未找到=9,
    绑定不完整=12, 主体不一致=13, 正式特征类型不一致=14,
    时间不递增=15, 无变化=16, 比较依据不支持=17,
    组成发生期依据不足=18, 重复成员=19, 来源成环=20,
    数量预算不足=21, 事实代次漂移=22, 幂等冲突=23,
    引用冲突=24, 旧格式不支持=25, 依赖未实现=26,
    资源失败=27, 内部不一致=28, 已可能发布=29
};

enum class 动态发布阶段 : std::uint8_t {
    无写入=0, 已提交待读回=1, 已读回=2, 未知=3
};

struct 动态操作结果 final {
    动态数据状态 状态 = 动态数据状态::入口拒绝;
    std::uint32_t 合同版本 = 1;
    std::uint64_t Gread = 0;
    std::optional<std::uint64_t> 发布代次;
    动态发布阶段 阶段 = 动态发布阶段::无写入;
};

struct 动态原子创建请求 final {
    std::uint32_t 合同版本=1; std::uint64_t G0=0;
    L1所有者范围写入幂等身份 幂等身份{};
    稳定编码 主体存在{}; 动态原子内容 变化;
    动态原子变化依据 变化依据; 动态读取预算 预算;
};
struct 动态组合创建请求 final {
    std::uint32_t 合同版本=1; std::uint64_t G0=0;
    L1所有者范围写入幂等身份 幂等身份{};
    稳定编码 主体存在{}; 动态组合内容 变化; 动态读取预算 预算;
};
struct 动态当前读取请求 final {
    std::uint32_t 合同版本=1; std::uint64_t G0=0;
    动态信息身份 身份; 动态读取预算 预算;
};
struct 动态按主体查询请求 final {
    std::uint32_t 合同版本=1; std::uint64_t G0=0;
    稳定编码 主体存在{}; 动态读取预算 预算;
};
struct 动态按绑定查询请求 final {
    std::uint32_t 合同版本=1; std::uint64_t G0=0;
    状态使用绑定身份 绑定; 动态读取预算 预算;
};
struct 动态按子动态查询请求 final {
    std::uint32_t 合同版本=1; std::uint64_t G0=0;
    动态信息身份 子动态; 动态读取预算 预算;
};
enum class 动态展开方式 : std::uint8_t { 直接=1, 递归=2 };
struct 动态来源展开请求 final {
    std::uint32_t 合同版本=1; std::uint64_t G0=0;
    动态信息身份 身份; 动态展开方式 方式=动态展开方式::递归;
    动态读取预算 预算;
};
struct 动态退出请求 final {
    std::uint32_t 合同版本=1; std::uint64_t G0=0;
    L1所有者范围写入幂等身份 幂等身份{};
    动态信息身份 身份; 动态读取预算 预算;
};
struct 动态当前身份请求 final {
    std::uint32_t 合同版本=1; std::uint64_t G0=0; 动态信息身份 身份;
};

struct 动态单项结果 final {
    动态操作结果 操作; std::optional<动态内容事实> 内容;
};
struct 动态组结果 final {
    动态操作结果 操作; std::vector<动态内容事实> 动态组;
};
struct 动态展开结果 final {
    动态操作结果 操作; std::optional<动态来源展开事实> 来源;
};

struct 动态结构交付 final {
    稳定编码 格式锚点{}, 字段登记关系类型{};
    稳定编码 动态族锚点{}, 族归属关系类型{}, 主体关系类型{};
    稳定编码 前绑定关系类型{}, 后绑定关系类型{}, 同主体成员关系类型{};
    稳定编码 首次形成UTC属性类型{};
};
class 动态结构只读提供者 {
public:
    virtual ~动态结构只读提供者() = default;
    virtual bool 绑定于(const L1事实基座服务&) const noexcept = 0;
    virtual 动态单项结果 读取当前动态(const 动态当前读取请求&) const = 0;
    virtual 动态组结果 按主体查询动态(const 动态按主体查询请求&) const = 0;
    virtual 动态组结果 按绑定反查原子动态(const 动态按绑定查询请求&) const = 0;
    virtual 动态组结果 按子动态反查父动态(const 动态按子动态查询请求&) const = 0;
    virtual 动态展开结果 展开动态来源(const 动态来源展开请求&) const = 0;
    virtual 动态操作结果 确认当前动态结构身份(const 动态当前身份请求&) const = 0;
};

} // namespace 海中鱼巣
