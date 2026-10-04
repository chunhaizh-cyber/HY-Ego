#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "新_特征类.h"

namespace 海中鱼巣 {

class 新_存在类;
class 新_场景类;
class 新_动态类;

enum class 新状态时间语义 : std::int64_t {
    实例绝对UTC = 1,
    抽象相对 = 2
};

struct 新状态强时间 final {
    新状态时间语义 语义 = 新状态时间语义::实例绝对UTC;
    std::int64_t 纳秒 = 0;
    friend bool operator==(const 新状态强时间&, const 新状态强时间&) = default;
};

struct 新状态信息 final {
    稳定编码 节点;
    稳定编码 被描述存在节点;
    // 建立状态时使用的实例特征身份。特征概念只说明类型/值域，不能替代
    // 同一存在下具体实例特征的来源身份。
    稳定编码 来源实例特征节点;
    稳定编码 特征类型概念节点;
    新特征准确值 固定准确值;
    新状态强时间 强时间;
    std::int64_t 被引用次数 = 0;
    friend bool operator==(const 新状态信息&, const 新状态信息&) = default;
};

enum class 新状态操作状态 : std::uint8_t {
    已完成 = 1,
    入口拒绝 = 2,
    存在不存在 = 3,
    特征不存在 = 4,
    特征当前值不存在 = 5,
    时间不合法 = 6,
    状态不存在 = 7,
    状态仍被引用 = 8,
    结构不一致 = 9,
    资源失败 = 10,
    引用计数溢出 = 11
};

struct 新状态建立结果 final {
    新状态操作状态 状态 = 新状态操作状态::入口拒绝;
    std::optional<稳定编码> 状态节点;
    std::optional<新状态信息> 状态信息;
};

class 新_状态类 final {
public:
    explicit 新_状态类(新_特征类& 特征服务) noexcept;

    bool 初始化() noexcept;

    std::optional<新状态信息> 获取状态(
        稳定编码 状态节点) const noexcept;
    bool 是状态节点(稳定编码 节点) const noexcept;

    std::vector<稳定编码> 按存在查询状态(
        稳定编码 被描述存在节点) const noexcept;
    std::vector<稳定编码> 按实例特征查询状态(
        稳定编码 来源实例特征节点) const noexcept;
    std::vector<稳定编码> 按特征类型查询状态(
        稳定编码 特征类型概念节点) const noexcept;
    std::vector<稳定编码> 按准确值查询状态(
        const 新特征准确值& 固定准确值) const noexcept;
    std::vector<稳定编码> 按强时间查询状态(
        const 新状态强时间& 强时间) const noexcept;
    std::vector<稳定编码> 查找完全相同状态(
        稳定编码 被描述存在节点,
        稳定编码 来源实例特征节点,
        稳定编码 特征类型概念节点,
        const 新特征准确值& 固定准确值,
        const 新状态强时间& 强时间) const noexcept;

    // 状态内容不可修改。存在任何场景列表、动态端点或其它引用时拒绝删除。
    新状态操作状态 删除状态(稳定编码 状态节点) noexcept;

private:
    friend class 新_存在类;
    friend class 新_场景类;
    friend class 新_动态类;

    // 状态只能由存在的实例特征当前值建立；类型和值从特征节点完整读回。
    新状态建立结果 建立状态(
        稳定编码 被描述存在节点,
        稳定编码 特征节点,
        const 新状态强时间& 强时间) noexcept;

    // 业务引用所有者必须与自己的关系写入/退出成对调用。
    新状态操作状态 增加引用(稳定编码 状态节点) noexcept;
    // 仅用于同一上层写事务尚未发布时撤销刚完成的增加引用；恢复原计数，
    // 即使恢复后为零也不触发状态删除。
    新状态操作状态 撤销增加引用(稳定编码 状态节点) noexcept;
    // 只有本次减法使计数归零时才自动删除状态；新建时的零不触发删除。
    新状态操作状态 减少引用(稳定编码 状态节点) noexcept;

    新_特征类& 特征服务_;
};

} // namespace 海中鱼巣
