#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include "新_特征类.h"

#include "引用保持.数据.h"

namespace 海中鱼巣 {

class 新_存在类;
class 新_动态类;
class 新_任务类;
class 新_状态类;
class 新状态结果引用参与接口;

struct 新事实字段保护变化 final {
    基础字段关系 修改前;
    基础字段关系 修改后;
};
struct 新事实追加保护增量 final {
    std::vector<稳定编码> 新建节点组;
    std::vector<基础字段关系> 新增字段组;
    std::vector<基础外部关系> 新增外部关系组;
    std::vector<新事实字段保护变化> 字段变化组;
};
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
    std::optional<新特征准确值> 固定准确值; // 无值表示变为未知（R98）。
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

class 新特征状态构造绑定 final {
private:
    新特征状态构造绑定() = default;
    std::array<稳定编码, 8> 字段_{};
    const 新_状态类* owner_ = nullptr;
    friend class 新_状态类;
    friend class 新_词类;
    friend class 新_自然语言世界树类;
};

// 只有状态、场景与隔离owner可构造各自未发布材料；其它调用者仅传递。
class 新特征状态参与材料 final {
public:
    新特征状态参与材料(新特征状态参与材料&&) noexcept = default;
    新特征状态参与材料& operator=(新特征状态参与材料&&) noexcept = default;
    新特征状态参与材料(const 新特征状态参与材料&) = delete;
    新特征状态参与材料& operator=(const 新特征状态参与材料&) = delete;
private:
    新特征状态参与材料() = default;
    基础节点组追加请求 请求_;
    std::size_t 状态槽_ = 0;
    const 新_状态类* owner_ = nullptr;
    新状态信息 本轮状态_;
    bool 结果引用已合并_ = false;
    friend class 新_状态类;
    friend class 新_场景类;
    friend class 新_模拟世界类;
};

// 同线程一次性共同发表准备；持有原owner锁直到完成或取消。
class 新_状态类;
class 新状态计数准备 final {
public:
    ~新状态计数准备();
    新状态计数准备(新状态计数准备&&) noexcept;
    新状态计数准备& operator=(新状态计数准备&&) noexcept;
    新状态计数准备(const 新状态计数准备&) = delete;
    新状态计数准备& operator=(const 新状态计数准备&) = delete;
private:
    struct 实现;
    explicit 新状态计数准备(std::unique_ptr<实现>) noexcept;
    std::unique_ptr<实现> 实现_;
    friend class 新_状态类;
};
struct 新状态计数准备结果 final {
    新状态操作状态 状态 = 新状态操作状态::入口拒绝;
    std::optional<新状态计数准备> 准备;
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

    // 上层组织者通过这些中立入口维护自己的状态引用；状态类不知道引用
    // 来自场景、动态、任务还是其它组合结构。
    新状态操作状态 增加引用(稳定编码 状态节点) noexcept;
    新状态操作状态 增加引用(稳定编码 状态节点, 新事实追加保护增量&) noexcept;
    新状态操作状态 撤销增加引用(稳定编码 状态节点) noexcept;
    新状态操作状态 减少引用(稳定编码 状态节点) noexcept;

    // 组合锁序：动态→状态→特征；场景读取须在取得任何计数准备之前完成。
    // 同owner只能有一份活动准备，批内同节点增量先合并；不可重入独立计数写口。
    // 归零删除由L1检查组合后的剩余引用；仍被引用时调用方保留原记录及清理责任，不能当变化无限重试。
    新状态计数准备结果 准备引用计数参与(const std::vector<新引用计数变化>&) noexcept;
    基础封存追加片段 封存计数参与(新状态计数准备&) const;
    void 登记计数发表(新状态计数准备&, const 基础片段发布回执&) noexcept;
    void 完成计数参与(新状态计数准备&) noexcept;
    void 取消计数参与(新状态计数准备&) noexcept;

private:
    friend class 新_词类;
    friend class 新_自然语言世界树类;
public:
    新特征状态构造绑定 取得特征状态构造绑定() const;
    新特征状态参与材料 准备特征状态(
        const 新特征状态构造绑定&, const 新特征当前值候选&,
        const std::variant<稳定编码, 基础组内节点引用>& 持有者,
        const 新状态强时间&, 基础节点组追加请求 附加请求 = {}) const;
    void 合并结果引用(新特征状态参与材料&, const 新状态结果引用参与接口&) const;
private:
    std::optional<新状态信息> 读取词条前状态(
        稳定编码 词条, 稳定编码 特征, const 新特征准确值& 旧值) const;
    friend class 新_存在类;
    friend class 新_动态类;
    friend class 新_任务类;
    friend class 新_方法类;

    // 状态只能由存在的实例特征当前值建立；类型和值从特征节点完整读回。
    新状态建立结果 建立状态(
        稳定编码 被描述存在节点,
        稳定编码 特征节点,
        const 新状态强时间& 强时间) noexcept;
    新状态建立结果 建立状态(稳定编码, 稳定编码,
        const 新状态强时间&, 新事实追加保护增量&) noexcept;

    // 调用方须确认F当前值实际改变为空；有值F不允许走此入口（R98）。
    新状态建立结果 建立变为未知状态(稳定编码, 稳定编码,
        const 新状态强时间&) noexcept;
    新状态建立结果 建立变为未知状态(稳定编码, 稳定编码,
        const 新状态强时间&, 新事实追加保护增量&) noexcept;

    新_特征类& 特征服务_;
};

} // namespace 海中鱼巣
