module;

#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <bit>
#include <limits>
#include <mutex>
#include <new>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

#define L2_TASK_STRUCTURE_NO_INCLUDES
#define L2_TASK_SUBGOAL_ACCEPTANCE_RECORD_NO_INCLUDES

export module 海中鱼巣.领域.服务.L2任务结构;

export import 海中鱼巣.领域.合同.L2结构公共;
export import 海中鱼巣.领域.合同.任务结果消费身份;
import 海中鱼巣.核心.服务.L1事实基座;
import 海中鱼巣.领域.服务.L2需求结构;
import 海中鱼巣.领域.服务.L2存在结构;
import 海中鱼巣.领域.服务.L2方法结构;
import 海中鱼巣.领域.服务.L2状态结构;
import 海中鱼巣.领域.服务.L2特征结构;

export {
#include "L2任务结构.数据.h"
#include "L2任务子目标承接记录.数据.h"
}

export namespace 海中鱼巣 {

// ===== 所有者交付固定身份 =====
inline constexpr L1所有者范围建立幂等身份 任务所有者建立身份{
    0x4C31'5441'534B'4F57ULL};

class L2任务结构服务;
class L2任务所有者交付;
class L2任务方法路径写入端口;
class L2任务子目标承接记录写入端口;

// ===== 任务方法路径私有承载能力 =====
class L2任务方法路径写入能力交付 final {
public:
    L2任务方法路径写入能力交付() = delete;
    L2任务方法路径写入能力交付(
        const L2任务方法路径写入能力交付&) = delete;
    L2任务方法路径写入能力交付& operator=(
        const L2任务方法路径写入能力交付&) = delete;
    L2任务方法路径写入能力交付(
        L2任务方法路径写入能力交付&& 其它) noexcept
        : 所有者_(其它.所有者_), 有效_(其它.有效_) {
        其它.有效_ = false;
    }
    L2任务方法路径写入能力交付& operator=(
        L2任务方法路径写入能力交付&&) = delete;

private:
    explicit L2任务方法路径写入能力交付(
        L1结构所有者身份 所有者) noexcept
        : 所有者_(所有者), 有效_(海中鱼巣::有效(所有者)) {}

    friend class L2任务所有者交付;
    friend std::optional<L2任务方法路径写入端口>
    尝试形成L2任务方法路径写入端口(
        L2任务结构服务&, L2任务方法路径写入能力交付&&) noexcept;
    L1结构所有者身份 所有者_{};
    bool 有效_ = false;
};

class L2任务方法路径写入端口 final {
public:
    L2任务方法路径写入端口() = delete;
    L2任务方法路径写入端口(const L2任务方法路径写入端口&) = delete;
    L2任务方法路径写入端口& operator=(
        const L2任务方法路径写入端口&) = delete;
    L2任务方法路径写入端口(L2任务方法路径写入端口&& 其它) noexcept
        : 服务_(其它.服务_) {
        其它.服务_ = nullptr;
    }
    L2任务方法路径写入端口& operator=(
        L2任务方法路径写入端口&&) = delete;

    bool 有效() const noexcept { return 服务_ != nullptr; }
    L2新增任务方法路径结果 写入已验证任务方法路径(
        L2新增任务方法路径请求 请求) noexcept;

private:
    explicit L2任务方法路径写入端口(L2任务结构服务& 服务) noexcept
        : 服务_(&服务) {}
    friend std::optional<L2任务方法路径写入端口>
    尝试形成L2任务方法路径写入端口(
        L2任务结构服务&, L2任务方法路径写入能力交付&&) noexcept;
    L2任务结构服务* 服务_ = nullptr;
};

// ===== 任务子目标承接记录私有承载能力 =====
class L2任务子目标承接记录写入能力交付 final {
public:
    L2任务子目标承接记录写入能力交付() = delete;
    L2任务子目标承接记录写入能力交付(
        const L2任务子目标承接记录写入能力交付&) = delete;
    L2任务子目标承接记录写入能力交付& operator=(
        const L2任务子目标承接记录写入能力交付&) = delete;
    L2任务子目标承接记录写入能力交付(
        L2任务子目标承接记录写入能力交付&& 其它) noexcept
        : 所有者_(其它.所有者_), 有效_(其它.有效_) {
        其它.有效_ = false;
    }
    L2任务子目标承接记录写入能力交付& operator=(
        L2任务子目标承接记录写入能力交付&&) = delete;

private:
    explicit L2任务子目标承接记录写入能力交付(
        L1结构所有者身份 所有者) noexcept
        : 所有者_(所有者), 有效_(海中鱼巣::有效(所有者.编码)) {}
    friend class L2任务所有者交付;
    friend std::optional<L2任务子目标承接记录写入端口>
    尝试形成L2任务子目标承接记录写入端口(
        L2任务结构服务&,
        L2任务子目标承接记录写入能力交付&&) noexcept;
    L1结构所有者身份 所有者_{};
    bool 有效_ = false;
};

class L2任务子目标承接记录写入端口 final {
public:
    L2任务子目标承接记录写入端口() = delete;
    L2任务子目标承接记录写入端口(
        const L2任务子目标承接记录写入端口&) = delete;
    L2任务子目标承接记录写入端口& operator=(
        const L2任务子目标承接记录写入端口&) = delete;
    L2任务子目标承接记录写入端口(
        L2任务子目标承接记录写入端口&& 其它) noexcept
        : 服务_(其它.服务_) { 其它.服务_ = nullptr; }
    L2任务子目标承接记录写入端口& operator=(
        L2任务子目标承接记录写入端口&&) = delete;

    bool 有效() const noexcept { return 服务_ != nullptr; }
    L2任务子目标承接记录写入结果 写入已验证新增记录(
        L2新增任务子目标承接记录请求 请求) noexcept;
    L2任务子目标承接记录写入结果 写入已验证绑定子需求(
        L2绑定任务子目标承接记录子需求请求 请求) noexcept;
    L2任务子目标承接记录写入结果 写入已验证登记回流(
        L2登记任务子目标承接记录回流请求 请求) noexcept;
    L2任务子目标承接记录写入结果 写入已验证退出记录(
        L2退出任务子目标承接记录请求 请求) noexcept;

private:
    explicit L2任务子目标承接记录写入端口(
        L2任务结构服务& 服务) noexcept : 服务_(&服务) {}
    friend std::optional<L2任务子目标承接记录写入端口>
    尝试形成L2任务子目标承接记录写入端口(
        L2任务结构服务&,
        L2任务子目标承接记录写入能力交付&&) noexcept;
    L2任务结构服务* 服务_ = nullptr;
};

std::optional<L2任务子目标承接记录写入端口>
尝试形成L2任务子目标承接记录写入端口(
    L2任务结构服务&,
    L2任务子目标承接记录写入能力交付&&) noexcept;

std::optional<L2任务方法路径写入端口>
尝试形成L2任务方法路径写入端口(
    L2任务结构服务&, L2任务方法路径写入能力交付&&) noexcept;

// ===== 所有者交付 =====
class L2任务所有者交付 final {
public:
    L2任务所有者交付() = delete;
    L2任务所有者交付(const L2任务所有者交付&) = delete;
    L2任务所有者交付& operator=(const L2任务所有者交付&) = delete;
    L2任务所有者交付(L2任务所有者交付&&) noexcept = default;
    L2任务所有者交付& operator=(L2任务所有者交付&&) = delete;

    bool 有效() const noexcept { return 写入端口_.有效(); }
    bool 绑定于(const L1事实基座服务& 服务) const noexcept {
        return 写入端口_.绑定于(服务);
    }
    L1结构所有者身份 所有者身份() const noexcept {
        return 写入端口_.所有者身份();
    }
    std::optional<L2任务方法路径写入能力交付>
    提取任务方法路径写入能力交付() noexcept {
        if (!有效() || 任务方法路径写入能力已提取_)
            return std::nullopt;
        任务方法路径写入能力已提取_ = true;
        return L2任务方法路径写入能力交付{所有者身份()};
    }
    std::optional<L2任务子目标承接记录写入能力交付>
    提取任务子目标承接记录写入能力交付() noexcept {
        if (!有效() || 任务子目标承接记录写入能力已提取_)
            return std::nullopt;
        任务子目标承接记录写入能力已提取_ = true;
        return L2任务子目标承接记录写入能力交付{所有者身份()};
    }

private:
    explicit L2任务所有者交付(L1所有者范围写端口&& 端口) noexcept
        : 写入端口_(std::move(端口)) {}
    friend std::optional<L2任务所有者交付> 尝试形成L2任务所有者交付(
        const L1事实基座服务&, L1所有者范围交付&&) noexcept;
    friend class L2任务结构服务;
    L1所有者范围写端口 写入端口_;
    bool 任务方法路径写入能力已提取_ = false;
    bool 任务子目标承接记录写入能力已提取_ = false;
};

std::optional<L2任务所有者交付> 尝试形成L2任务所有者交付(
    const L1事实基座服务&, L1所有者范围交付&&) noexcept;

} // namespace 海中鱼巣

namespace 海中鱼巣::L2任务结构内部 {

// ===== 身份来源定位 =====
struct 任务身份来源定位 final {
    L1结构所有者身份 所有者;
    稳定编码 任务锚点;
    稳定编码 任务族归属关系类型;
    稳定编码 任务族归属关系;
    稳定编码 虚拟存在锚点;
    稳定编码 虚拟存在归属关系类型;
    稳定编码 虚拟存在归属关系;
    稳定编码 路径族归属关系类型;
    稳定编码 路径族归属关系;
    稳定编码 实例方法族归属关系类型;
    稳定编码 实例方法族归属关系;
    稳定编码 路径筹办轮次属性类型;
    稳定编码 路径来源方法属性类型;
    稳定编码 路径来源方法内容版本属性类型;
    稳定编码 路径条件绑定属性类型;
    稳定编码 路径输入绑定属性类型;
    稳定编码 路径参数绑定属性类型;
    稳定编码 路径动作入口属性类型;
    稳定编码 路径完整有序动作组属性类型_v2;
    稳定编码 路径预期结果属性类型;
    稳定编码 路径验证合同属性类型;
    稳定编码 路径归因合同属性类型;
    稳定编码 实例方法执行轮次属性类型;
    稳定编码 结果锚点;
    稳定编码 结果族归属关系;
    稳定编码 结果来源材料版本属性类型;
    稳定编码 结果运行代次属性类型;
    稳定编码 治理状态锚点;
    稳定编码 治理状态族归属关系类型锚点;
    稳定编码 治理状态族归属关系锚点;
    稳定编码 目标裁决证据锚点;
    稳定编码 目标裁决证据族归属关系类型锚点;
    稳定编码 目标裁决证据族归属关系锚点;
    稳定编码 治理状态生命周期发生时间属性类型;
    稳定编码 治理状态材料属性类型;
    稳定编码 目标裁决材料属性类型;
    std::uint64_t 建立事实代次 = 0;
};

// ===== 结构类型定位 =====
struct 任务结构类型定位 final {
    稳定编码 任务族归属关系类型;
    稳定编码 任务需求列表项引用关系类型;
    稳定编码 任务虚拟存在归属关系类型;
    稳定编码 任务来源需求关系类型;
    稳定编码 任务正式存在关系类型;
    稳定编码 路径族归属关系类型;
    稳定编码 路径任务引用关系类型;
    稳定编码 当前选中路径关系类型;
    稳定编码 实例方法族归属关系类型;
    稳定编码 实例方法任务引用关系类型;
    稳定编码 实例方法路径引用关系类型;
    稳定编码 实例方法当前关系类型;
    稳定编码 结果族归属关系类型;
    稳定编码 任务实际结果槽关系类型;
    稳定编码 结果实例方法关系类型;
    稳定编码 结果实际状态关系类型;
    稳定编码 结果动态证据关系类型;
    稳定编码 结果方法关系类型;
    稳定编码 结果动作入口关系类型;
    稳定编码 结果场景关系类型;
    稳定编码 结果主体关系类型;
    稳定编码 治理状态族归属关系类型;
    稳定编码 目标裁决证据族归属关系类型;
    稳定编码 任务当前治理状态关系类型;
    稳定编码 治理状态迁移证据关系类型;
    稳定编码 状态虚拟存在关系类型;
    稳定编码 状态场景关系类型;
    稳定编码 状态前驱关系类型;
    稳定编码 状态路径关系类型;
    稳定编码 状态实例关系类型;
    稳定编码 状态实际结果关系类型;
    稳定编码 状态目标裁决证据关系类型;
    稳定编码 证据任务关系类型;
    稳定编码 证据实例关系类型;
    稳定编码 证据实际结果关系类型;
    稳定编码 证据目标来源关系类型;
    稳定编码 证据目标宿主关系类型;
    稳定编码 证据目标特征关系类型;
    稳定编码 证据目标状态合同关系类型;
    稳定编码 证据实际状态关系类型;
    稳定编码 证据动态关系类型;
    稳定编码 证据方法关系类型;
    稳定编码 证据比较注册关系类型;
    std::uint64_t 建立事实代次 = 0;
};

struct 任务初次筹办治理定位 final {
    稳定编码 任务轮次锚点;
    稳定编码 任务轮次族归属关系类型;
    稳定编码 任务轮次任务关系类型;
    稳定编码 任务当前轮次关系类型;
    稳定编码 任务轮次序号属性类型;
    稳定编码 治理函数定义节点;
    稳定编码 治理函数定义材料属性类型;
    稳定编码 治理动作调用锚点;
    稳定编码 治理动作族归属关系类型;
    稳定编码 治理动作函数关系类型;
    稳定编码 治理动作任务关系类型;
    稳定编码 治理动作轮次关系类型;
    稳定编码 治理动作筹办轮次关系类型;
    稳定编码 治理动作触发前态关系类型;
    稳定编码 治理动作调用材料属性类型;
    std::uint64_t 建立事实代次 = 0;
};

struct 任务轮次结算登记定位 final {
    稳定编码 结算锚点;
    稳定编码 结算族关系类型;
    稳定编码 任务关系类型;
    稳定编码 任务轮次关系类型;
    稳定编码 实际结果关系类型;
    稳定编码 验证归因收口关系类型;
    稳定编码 消费分配记录关系类型;
    稳定编码 需求核算关系类型;
    稳定编码 普通价值结算关系类型;
    稳定编码 学习能力变化关系类型;
    稳定编码 执行冻结收口关系类型;
    稳定编码 授权关系类型;
    稳定编码 授权收口关系类型;
    稳定编码 任务运行停止证据关系类型;
    稳定编码 结算材料属性类型;
    稳定编码 自我继续治理函数定义;
    稳定编码 轮次收束治理函数定义;
    稳定编码 结算治理动作关系类型;
    稳定编码 筹办权威任务轮次关系类型;
    稳定编码 筹办权威阶段特征实例关系类型;
    稳定编码 筹办权威初始阶段状态关系类型;
    稳定编码 决议引用锚点;
    稳定编码 决议引用族关系类型;
    稳定编码 决议引用任务关系类型;
    稳定编码 决议引用轮次关系类型;
    稳定编码 决议引用自我决议关系类型;
    稳定编码 决议引用材料属性类型;
    稳定编码 生命周期收口锚点;
    稳定编码 生命周期收口族关系类型;
    稳定编码 生命周期收口任务关系类型;
    稳定编码 生命周期收口轮次关系类型;
    稳定编码 生命周期收口结算关系类型;
    稳定编码 生命周期收口决议关系类型;
    稳定编码 生命周期收口停止证据关系类型;
    稳定编码 生命周期收口材料属性类型;
    std::uint64_t 建立事实代次 = 0;
};

inline constexpr std::uint32_t 任务轮次结算登记起始键 = 0x0301'280EU;
inline constexpr std::uint32_t 任务轮次结算锚点键 = 0x0301'280EU;
inline constexpr std::uint32_t 任务轮次结算族关系类型键 = 0x0301'280FU;
inline constexpr std::uint32_t 任务轮次结算任务关系类型键 = 0x0301'2810U;
inline constexpr std::uint32_t 任务轮次结算任务轮次关系类型键 = 0x0301'2811U;
inline constexpr std::uint32_t 任务轮次结算实际结果关系类型键 = 0x0301'2812U;
inline constexpr std::uint32_t 任务轮次结算消费分配关系类型键 = 0x0301'2813U;
inline constexpr std::uint32_t 任务轮次结算需求核算关系类型键 = 0x0301'2814U;
inline constexpr std::uint32_t 任务轮次结算普通价值关系类型键 = 0x0301'2815U;
inline constexpr std::uint32_t 任务轮次结算学习变化关系类型键 = 0x0301'2816U;
inline constexpr std::uint32_t 任务轮次结算执行冻结收口关系类型键 = 0x0301'2817U;
inline constexpr std::uint32_t 任务轮次结算授权关系类型键 = 0x0301'2818U;
inline constexpr std::uint32_t 任务轮次结算授权收口关系类型键 = 0x0301'2819U;
inline constexpr std::uint32_t 任务轮次结算运行停止关系类型键 = 0x0301'281AU;
inline constexpr std::uint32_t 任务轮次结算材料属性类型键 = 0x0301'281BU;
inline constexpr std::uint32_t 任务轮次结算验证归因收口关系类型键 = 0x0301'281CU;
inline constexpr std::uint32_t 自我继续治理函数定义键 = 0x0301'281DU;
inline constexpr std::uint32_t 自我继续治理函数定义材料值键 = 0x0301'281EU;
inline constexpr std::uint32_t 任务后继决议引用锚点键 = 0x0301'281FU;
inline constexpr std::uint32_t 任务后继决议引用族关系类型键 = 0x0301'2820U;
inline constexpr std::uint32_t 任务后继决议引用任务关系类型键 = 0x0301'2821U;
inline constexpr std::uint32_t 任务后继决议引用轮次关系类型键 = 0x0301'2822U;
inline constexpr std::uint32_t 任务后继决议引用自我决议关系类型键 = 0x0301'2823U;
inline constexpr std::uint32_t 任务后继决议引用材料属性类型键 = 0x0301'2824U;
inline constexpr std::uint32_t 任务生命周期收口锚点键 = 0x0301'2825U;
inline constexpr std::uint32_t 任务生命周期收口族关系类型键 = 0x0301'2826U;
inline constexpr std::uint32_t 任务生命周期收口任务关系类型键 = 0x0301'2827U;
inline constexpr std::uint32_t 任务生命周期收口轮次关系类型键 = 0x0301'2828U;
inline constexpr std::uint32_t 任务生命周期收口结算关系类型键 = 0x0301'2829U;
inline constexpr std::uint32_t 任务生命周期收口决议关系类型键 = 0x0301'282AU;
inline constexpr std::uint32_t 任务生命周期收口停止证据关系类型键 = 0x0301'282BU;
inline constexpr std::uint32_t 任务生命周期收口材料属性类型键 = 0x0301'282CU;
inline constexpr std::uint32_t 轮次收束治理函数定义键 = 0x0301'282DU;
inline constexpr std::uint32_t 轮次收束治理函数定义材料值键 = 0x0301'282EU;
inline constexpr std::uint32_t 任务轮次结算治理动作关系类型键 = 0x0301'282FU;
inline constexpr std::uint32_t 筹办权威任务轮次关系类型键 = 0x0301'2830U;
inline constexpr std::uint32_t 筹办权威阶段特征实例关系类型键 = 0x0301'2831U;
inline constexpr std::uint32_t 筹办权威初始阶段状态关系类型键 = 0x0301'2832U;
inline constexpr std::uint32_t 任务轮次结算登记末键 = 0x0301'2832U;

static_assert(任务轮次结算登记末键 - 任务轮次结算登记起始键 + 1U == 37U);

inline constexpr L1所有者范围写入幂等身份 任务轮次结算登记幂等身份{
    0x4C32'5452'5345'5454ULL};

// ===== 任务筹办正式选择登记定位 =====
struct 任务正式选择登记定位 final {
    稳定编码 选择记录锚点;
    稳定编码 冻结材料锚点;
    稳定编码 完整请求属性类型;
    稳定编码 选择记录族归属关系类型;
    稳定编码 冻结材料族归属关系类型;
    稳定编码 任务当前正式选择关系类型;
    稳定编码 选择记录方法引用关系类型;
    稳定编码 选择记录路径引用关系类型;
    稳定编码 选择记录来源需求关系类型;
    稳定编码 选择记录稳定证据关系类型;
    稳定编码 选择记录冻结材料关系类型;
    std::uint64_t 建立事实代次 = 0;
};

inline constexpr std::uint32_t 选择记录锚点本地键值 = 0x0301'2060U;
inline constexpr std::uint32_t 冻结材料锚点本地键值 = 0x0301'2061U;
inline constexpr std::uint32_t 正式选择完整请求属性类型本地键值 = 0x0301'2062U;
inline constexpr std::uint32_t 选择记录族归属关系类型本地键值 = 0x0301'2063U;
inline constexpr std::uint32_t 冻结材料族归属关系类型本地键值 = 0x0301'2064U;
inline constexpr std::uint32_t 任务当前正式选择关系类型本地键值 = 0x0301'2065U;
inline constexpr std::uint32_t 选择记录方法引用关系类型本地键值 = 0x0301'2066U;
inline constexpr std::uint32_t 选择记录路径引用关系类型本地键值 = 0x0301'2067U;
inline constexpr std::uint32_t 选择记录来源需求关系类型本地键值 = 0x0301'2068U;
inline constexpr std::uint32_t 选择记录稳定证据关系类型本地键值 = 0x0301'2069U;
inline constexpr std::uint32_t 选择记录冻结材料关系类型本地键值 = 0x0301'206AU;

inline constexpr L1所有者范围写入幂等身份 任务正式选择登记幂等身份{
    0x4C32'5441'534B'5343ULL};

// ===== 固定幂等身份 =====
inline constexpr L1所有者范围写入幂等身份 任务身份来源登记幂等身份{
    0x4C32'5441'534B'4944ULL};
inline constexpr L1所有者范围写入幂等身份 任务路径动作组v2登记幂等身份{
    0x4C32'5441'534B'5632ULL};
inline constexpr L1所有者范围写入幂等身份 任务结构类型登记幂等身份{
    0x4C32'5441'534B'5459ULL};
inline constexpr L1所有者范围写入幂等身份 任务实际结果身份来源登记幂等身份{
    0x4C32'5441'5253'4944ULL};
inline constexpr L1所有者范围写入幂等身份 任务实际结果结构类型登记幂等身份{
    0x4C32'5441'5253'5459ULL};
inline constexpr L1所有者范围写入幂等身份 治理状态身份来源登记幂等身份{
    0x4C32'5441'5347'4944ULL};
inline constexpr L1所有者范围写入幂等身份 治理状态结构类型登记幂等身份{
    0x4C32'5441'5347'5459ULL};

// ===== 本地键值 =====
inline constexpr std::uint32_t 任务锚点本地键值 = 0x0301'2001U;
inline constexpr std::uint32_t 任务族归属关系类型本地键值 = 0x0301'2002U;
inline constexpr std::uint32_t 任务族归属关系本地键值 = 0x0301'2003U;
inline constexpr std::uint32_t 虚拟存在锚点本地键值 = 0x0301'2004U;
inline constexpr std::uint32_t 虚拟存在归属关系类型本地键值 = 0x0301'2005U;
inline constexpr std::uint32_t 虚拟存在归属关系本地键值 = 0x0301'2006U;
inline constexpr std::uint32_t 路径族归属关系类型本地键值 = 0x0301'2007U;
inline constexpr std::uint32_t 路径族归属关系本地键值 = 0x0301'2008U;
inline constexpr std::uint32_t 实例方法族归属关系类型本地键值 = 0x0301'2009U;
inline constexpr std::uint32_t 实例方法族归属关系本地键值 = 0x0301'200AU;
inline constexpr std::uint32_t 路径筹办轮次属性类型本地键值 = 0x0301'200BU;
inline constexpr std::uint32_t 路径来源方法属性类型本地键值 = 0x0301'200CU;
inline constexpr std::uint32_t 路径来源方法内容版本属性类型本地键值 = 0x0301'200DU;
inline constexpr std::uint32_t 路径条件绑定属性类型本地键值 = 0x0301'200EU;
inline constexpr std::uint32_t 路径输入绑定属性类型本地键值 = 0x0301'200FU;
inline constexpr std::uint32_t 路径参数绑定属性类型本地键值 = 0x0301'2010U;
inline constexpr std::uint32_t 路径动作入口属性类型本地键值 = 0x0301'2011U;
inline constexpr std::uint32_t 路径预期结果属性类型本地键值 = 0x0301'2012U;
inline constexpr std::uint32_t 路径验证合同属性类型本地键值 = 0x0301'2013U;
inline constexpr std::uint32_t 路径归因合同属性类型本地键值 = 0x0301'2014U;
inline constexpr std::uint32_t 实例方法执行轮次属性类型本地键值 = 0x0301'2015U;
inline constexpr std::uint32_t 结果锚点本地键值 = 0x0301'2016U;
inline constexpr std::uint32_t 结果族归属关系本地键值 = 0x0301'2017U;
inline constexpr std::uint32_t 结果来源材料版本属性类型本地键值 = 0x0301'2018U;
inline constexpr std::uint32_t 结果运行代次属性类型本地键值 = 0x0301'2019U;
inline constexpr std::uint32_t 路径完整有序动作组属性类型本地键值_v2 =
    0x0301'201AU;

inline constexpr std::uint32_t 任务族归属关系类型键值 = 0x0301'2111U;
inline constexpr std::uint32_t 任务需求列表项引用关系类型键值 = 0x0301'2112U;
inline constexpr std::uint32_t 任务虚拟存在归属关系类型键值 = 0x0301'2113U;
inline constexpr std::uint32_t 任务来源需求关系类型键值 = 0x0301'2114U;
inline constexpr std::uint32_t 任务正式存在关系类型键值 = 0x0301'2115U;
inline constexpr std::uint32_t 路径族归属关系类型键值 = 0x0301'2121U;
inline constexpr std::uint32_t 路径任务引用关系类型键值 = 0x0301'2122U;
inline constexpr std::uint32_t 当前选中路径关系类型键值 = 0x0301'2123U;
inline constexpr std::uint32_t 实例方法族归属关系类型键值 = 0x0301'2131U;
inline constexpr std::uint32_t 实例方法任务引用关系类型键值 = 0x0301'2132U;
inline constexpr std::uint32_t 实例方法路径引用关系类型键值 = 0x0301'2133U;
inline constexpr std::uint32_t 实例方法当前关系类型键值 = 0x0301'2134U;
inline constexpr std::uint32_t 结果族归属关系类型键值 = 0x0301'2141U;
inline constexpr std::uint32_t 任务实际结果槽关系类型键值 = 0x0301'2142U;
inline constexpr std::uint32_t 结果实例方法关系类型键值 = 0x0301'2143U;
inline constexpr std::uint32_t 结果实际状态关系类型键值 = 0x0301'2144U;
inline constexpr std::uint32_t 结果动态证据关系类型键值 = 0x0301'2145U;
inline constexpr std::uint32_t 结果方法关系类型键值 = 0x0301'2146U;
inline constexpr std::uint32_t 结果动作入口关系类型键值 = 0x0301'2147U;
inline constexpr std::uint32_t 结果场景关系类型键值 = 0x0301'2148U;
inline constexpr std::uint32_t 结果主体关系类型键值 = 0x0301'2149U;

inline constexpr std::uint32_t 治理状态锚点本地键值 = 0x0301'2040U;
inline constexpr std::uint32_t 治理状态族归属关系类型锚点本地键值 = 0x0301'2041U;
inline constexpr std::uint32_t 治理状态族归属关系锚点本地键值 = 0x0301'2042U;
inline constexpr std::uint32_t 目标裁决证据锚点本地键值 = 0x0301'2043U;
inline constexpr std::uint32_t 目标裁决证据族归属关系类型锚点本地键值 = 0x0301'2044U;
inline constexpr std::uint32_t 目标裁决证据族归属关系锚点本地键值 = 0x0301'2045U;
inline constexpr std::uint32_t 治理状态生命周期发生时间属性类型本地键值 = 0x0301'2046U;
inline constexpr std::uint32_t 治理状态材料属性类型本地键值 = 0x0301'2047U;
inline constexpr std::uint32_t 目标裁决材料属性类型本地键值 = 0x0301'2048U;

inline constexpr std::uint32_t 治理状态族归属关系类型键值 = 0x0301'2150U;
inline constexpr std::uint32_t 目标裁决证据族归属关系类型键值 = 0x0301'2151U;
inline constexpr std::uint32_t 任务当前治理状态关系类型键值 = 0x0301'2152U;
inline constexpr std::uint32_t 治理状态迁移证据关系类型键值 = 0x0301'2153U;
inline constexpr std::uint32_t 状态虚拟存在关系类型键值 = 0x0301'2154U;
inline constexpr std::uint32_t 状态场景关系类型键值 = 0x0301'2155U;
inline constexpr std::uint32_t 状态前驱关系类型键值 = 0x0301'2156U;
inline constexpr std::uint32_t 状态路径关系类型键值 = 0x0301'2157U;
inline constexpr std::uint32_t 状态实例关系类型键值 = 0x0301'2158U;
inline constexpr std::uint32_t 状态实际结果关系类型键值 = 0x0301'2159U;
inline constexpr std::uint32_t 状态目标裁决证据关系类型键值 = 0x0301'215AU;
inline constexpr std::uint32_t 证据任务关系类型键值 = 0x0301'215BU;
inline constexpr std::uint32_t 证据实例关系类型键值 = 0x0301'215CU;
inline constexpr std::uint32_t 证据实际结果关系类型键值 = 0x0301'215DU;
inline constexpr std::uint32_t 证据目标来源关系类型键值 = 0x0301'215EU;
inline constexpr std::uint32_t 证据目标宿主关系类型键值 = 0x0301'215FU;
inline constexpr std::uint32_t 证据目标特征关系类型键值 = 0x0301'2160U;
inline constexpr std::uint32_t 证据目标状态合同关系类型键值 = 0x0301'2161U;
inline constexpr std::uint32_t 证据实际状态关系类型键值 = 0x0301'2162U;
inline constexpr std::uint32_t 证据动态关系类型键值 = 0x0301'2163U;
inline constexpr std::uint32_t 证据方法关系类型键值 = 0x0301'2164U;
inline constexpr std::uint32_t 证据比较注册关系类型键值 = 0x0301'2165U;

static_assert(证据比较注册关系类型键值
    - 治理状态族归属关系类型键值 + 1U == 22U);

// ===== 任务轮次、治理函数定义与治理动作登记 =====
inline constexpr std::uint32_t 任务初次治理登记起始键 = 0x0301'2180U;
inline constexpr std::uint32_t 任务轮次锚点键 = 0x0301'2180U;
inline constexpr std::uint32_t 任务轮次族归属关系类型键 = 0x0301'2181U;
inline constexpr std::uint32_t 任务轮次任务关系类型键 = 0x0301'2182U;
inline constexpr std::uint32_t 任务当前轮次关系类型键 = 0x0301'2183U;
inline constexpr std::uint32_t 任务轮次序号属性类型键 = 0x0301'2184U;
inline constexpr std::uint32_t 治理函数定义节点键 = 0x0301'2185U;
inline constexpr std::uint32_t 治理函数定义材料属性类型键 = 0x0301'2186U;
inline constexpr std::uint32_t 治理动作调用锚点键 = 0x0301'2187U;
inline constexpr std::uint32_t 治理动作调用关系类型起始键 = 0x0301'2188U;
inline constexpr std::uint32_t 治理动作调用材料属性类型键 = 0x0301'218EU;
inline constexpr std::uint32_t 任务初次治理登记末键 = 0x0301'218EU;
inline constexpr std::uint32_t 治理函数定义材料值写入键 = 0x0301'218FU;

static_assert(任务初次治理登记末键 - 任务初次治理登记起始键 + 1U == 15U);
static_assert(治理函数定义材料值写入键 == 任务初次治理登记末键 + 1U);
static_assert(治理函数定义节点键 != 治理函数定义材料属性类型键);
static_assert(治理函数定义节点键 != 治理函数定义材料值写入键);
static_assert(治理函数定义材料属性类型键 != 治理函数定义材料值写入键);

// ===== 写入本地键值（新增任务） =====
inline constexpr std::uint32_t 任务节点写入本地键值 = 0x0301'2201U;
static_assert(治理函数定义材料值写入键 < 任务节点写入本地键值);
inline constexpr std::uint32_t 虚拟存在节点写入本地键值 = 0x0301'2202U;
inline constexpr std::uint32_t 任务族归属关系写入本地键值 = 0x0301'2203U;
inline constexpr std::uint32_t 任务需求列表项引用关系写入本地键值 = 0x0301'2204U;
inline constexpr std::uint32_t 任务虚拟存在归属关系写入本地键值 = 0x0301'2205U;

// ===== 写入本地键值（新增任务方法路径） =====
inline constexpr std::uint32_t 路径节点写入本地键值 = 0x0301'2206U;
inline constexpr std::uint32_t 路径族归属关系写入本地键值 = 0x0301'2207U;
inline constexpr std::uint32_t 路径任务引用关系写入本地键值 = 0x0301'2208U;
inline constexpr std::uint32_t 当前选中路径关系写入本地键值 = 0x0301'2209U;
inline constexpr std::uint32_t 路径筹办轮次值写入本地键值 = 0x0301'220FU;
inline constexpr std::uint32_t 路径来源方法值写入本地键值 = 0x0301'2210U;
inline constexpr std::uint32_t 路径来源方法内容版本值写入本地键值 = 0x0301'2211U;
inline constexpr std::uint32_t 路径条件绑定值写入本地键值 = 0x0301'2212U;
inline constexpr std::uint32_t 路径输入绑定值写入本地键值 = 0x0301'2213U;
inline constexpr std::uint32_t 路径参数绑定值写入本地键值 = 0x0301'2214U;
inline constexpr std::uint32_t 路径动作入口值写入本地键值 = 0x0301'2215U;
inline constexpr std::uint32_t 路径预期结果值写入本地键值 = 0x0301'2216U;
inline constexpr std::uint32_t 路径验证合同值写入本地键值 = 0x0301'2217U;
inline constexpr std::uint32_t 路径归因合同值写入本地键值 = 0x0301'2218U;

// ===== 写入本地键值（创建实例方法） =====
inline constexpr std::uint32_t 实例方法节点写入本地键值 = 0x0301'220AU;
inline constexpr std::uint32_t 实例方法族归属关系写入本地键值 = 0x0301'220BU;
inline constexpr std::uint32_t 实例方法任务引用关系写入本地键值 = 0x0301'220CU;
inline constexpr std::uint32_t 实例方法路径引用关系写入本地键值 = 0x0301'220DU;
inline constexpr std::uint32_t 实例方法当前关系写入本地键值 = 0x0301'220EU;

// ===== 写入本地键值（任务筹办正式选择组合事务） =====
inline constexpr std::uint32_t 正式选择记录节点写入本地键值 = 0x0301'2280U;
inline constexpr std::uint32_t 执行绑定冻结材料节点写入本地键值 = 0x0301'2281U;
inline constexpr std::uint32_t 正式选择完整请求值写入本地键值 = 0x0301'2282U;
inline constexpr std::uint32_t 选择记录族归属关系写入本地键值 = 0x0301'2283U;
inline constexpr std::uint32_t 冻结材料族归属关系写入本地键值 = 0x0301'2284U;
inline constexpr std::uint32_t 任务当前正式选择关系写入本地键值 = 0x0301'2285U;
inline constexpr std::uint32_t 选择记录方法引用关系写入本地键值 = 0x0301'2286U;
inline constexpr std::uint32_t 选择记录路径引用关系写入本地键值 = 0x0301'2287U;
inline constexpr std::uint32_t 选择记录冻结材料关系写入本地键值 = 0x0301'2288U;
inline constexpr std::uint32_t 选择记录来源需求关系写入基值 = 0x0301'2300U;
inline constexpr std::uint32_t 选择记录稳定证据关系写入基值 = 0x0301'2400U;

// ===== 写入本地键值（任务实际结果） =====
inline constexpr std::uint32_t 结果节点写入本地键值 = 0x0301'2231U;
inline constexpr std::uint32_t 结果族归属关系写入本地键值 = 0x0301'2232U;
inline constexpr std::uint32_t 结果实例方法关系写入本地键值 = 0x0301'2233U;
inline constexpr std::uint32_t 结果实际状态关系写入本地键值 = 0x0301'2234U;
inline constexpr std::uint32_t 结果动态证据关系写入本地键值 = 0x0301'2235U;
inline constexpr std::uint32_t 结果方法关系写入本地键值 = 0x0301'2236U;
inline constexpr std::uint32_t 结果动作入口关系写入本地键值 = 0x0301'2237U;
inline constexpr std::uint32_t 结果场景关系写入本地键值 = 0x0301'2238U;
inline constexpr std::uint32_t 结果主体关系写入本地键值 = 0x0301'2239U;
inline constexpr std::uint32_t 任务结果槽关系写入本地键值 = 0x0301'223AU;
inline constexpr std::uint32_t 结果来源材料版本值写入本地键值 = 0x0301'223BU;
inline constexpr std::uint32_t 结果运行代次值写入本地键值 = 0x0301'223CU;

inline constexpr std::uint32_t 新治理状态节点写入本地键值 = 0x0301'2250U;
inline constexpr std::uint32_t 新目标裁决证据节点写入本地键值 = 0x0301'2251U;
inline constexpr std::uint32_t 状态族归属关系写入本地键值 = 0x0301'2252U;
inline constexpr std::uint32_t 证据族归属关系写入本地键值 = 0x0301'2253U;
inline constexpr std::uint32_t 任务当前治理状态关系写入本地键值 = 0x0301'2254U;
inline constexpr std::uint32_t 状态虚拟存在关系写入本地键值 = 0x0301'2255U;
inline constexpr std::uint32_t 状态场景关系写入本地键值 = 0x0301'2256U;
inline constexpr std::uint32_t 状态前驱关系写入本地键值 = 0x0301'2257U;
inline constexpr std::uint32_t 状态路径关系写入本地键值 = 0x0301'2258U;
inline constexpr std::uint32_t 状态实例关系写入本地键值 = 0x0301'2259U;
inline constexpr std::uint32_t 状态实际结果关系写入本地键值 = 0x0301'225AU;
inline constexpr std::uint32_t 状态目标裁决证据关系写入本地键值 = 0x0301'225BU;
inline constexpr std::uint32_t 状态关系15任务写入本地键值 = 0x0301'225CU;
inline constexpr std::uint32_t 状态关系15方法写入本地键值 = 0x0301'225DU;
inline constexpr std::uint32_t 状态关系15动作写入本地键值 = 0x0301'225EU;
inline constexpr std::uint32_t 状态关系15状态写入本地键值 = 0x0301'225FU;
inline constexpr std::uint32_t 状态关系15动态写入本地键值 = 0x0301'2260U;
inline constexpr std::uint32_t 证据任务关系写入本地键值 = 0x0301'2261U;
inline constexpr std::uint32_t 证据实例关系写入本地键值 = 0x0301'2262U;
inline constexpr std::uint32_t 证据实际结果关系写入本地键值 = 0x0301'2263U;
inline constexpr std::uint32_t 证据目标来源关系写入本地键值 = 0x0301'2264U;
inline constexpr std::uint32_t 证据目标宿主关系写入本地键值 = 0x0301'2265U;
inline constexpr std::uint32_t 证据目标特征关系写入本地键值 = 0x0301'2266U;
inline constexpr std::uint32_t 证据目标状态合同关系写入本地键值 = 0x0301'2267U;
inline constexpr std::uint32_t 证据实际状态关系写入本地键值 = 0x0301'2268U;
inline constexpr std::uint32_t 证据动态关系写入本地键值 = 0x0301'2269U;
inline constexpr std::uint32_t 证据方法关系写入本地键值 = 0x0301'226AU;
inline constexpr std::uint32_t 证据比较注册关系写入本地键值 = 0x0301'226BU;
inline constexpr std::uint32_t 治理状态发生时间值写入本地键值 = 0x0301'226CU;
inline constexpr std::uint32_t 治理状态材料值写入本地键值 = 0x0301'226DU;
inline constexpr std::uint32_t 目标裁决材料值写入本地键值 = 0x0301'226EU;

static_assert(目标裁决材料值写入本地键值
    - 新治理状态节点写入本地键值 + 1U == 31U);

// ===== 幂等身份常量 =====
inline constexpr L1所有者范围写入幂等身份 路径登记幂等身份基{
    0x4C32'5041'5448'4944ULL};
inline constexpr L1所有者范围写入幂等身份 实例方法登记幂等身份基{
    0x4C32'494E'5354'4D54ULL};

// ===== 失败构造器 =====
inline L2任务身份来源读取结果 形成任务身份来源读取失败(
    L2结构状态 状态, std::uint64_t 截止 = 0) noexcept {
    L2任务身份来源读取结果 结果;
    结果.结果头.合同版本 = L2结构合同版本;
    结果.结果头.状态 = 状态;
    结果.结果头.事实截止代次 = 截止;
    return 结果;
}

// ===== 中性当前事实代次辅助 =====
inline std::uint64_t 读取中性当前事实代次(
    const L1事实基座服务& 服务) noexcept {
    const auto 代次 = 服务.读取中性当前事实代次({L1中性CRUD合同版本});
    return 代次.状态 == L1中性读取状态::成功 ? 代次.事实代次 : 0;
}

// 诊断责任：向上送出；固定登记恢复只复用 L1 首次请求的原 G0。
std::uint64_t 读取固定登记期望代次(const L1事实基座服务& 第一层服务,
    L1所有者范围写端口& 写入端口,
    L1所有者范围写入幂等身份 幂等身份) {
    const auto 首次 = 写入端口.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本, 幂等身份});
    if (首次.状态 == L1所有者范围读取状态::成功) {
        if (首次.合同版本 != L1所有者范围首次写入读取合同版本
            || 首次.所有者 != 写入端口.所有者身份()
            || 首次.写入幂等身份 != 幂等身份
            || !首次.首次规范化写集 || !首次.首次写入结果
            || 首次.首次规范化写集->写入幂等身份 != 幂等身份
            || 首次.首次规范化写集->期望事实代次 == 0
            || 首次.首次写入结果->所有者 != 写入端口.所有者身份()
            || 首次.首次写入结果->写入幂等身份 != 幂等身份
            || 首次.首次写入结果->事实代次 == 0)
            throw std::runtime_error(
                "L2 task fixed registration first material invalid");
        return 首次.首次规范化写集->期望事实代次;
    }
    if (首次.状态 != L1所有者范围读取状态::未找到)
        throw std::runtime_error("L2 task fixed registration first read failed");
    const auto 当前 = 读取中性当前事实代次(第一层服务);
    if (当前 == 0)
        throw std::runtime_error("L2 task fixed registration generation failed");
    return 当前;
}

// ===== 身份来源初始化 =====
任务身份来源定位 初始化任务身份来源(
    const L1事实基座服务& 第一层服务,
    L1所有者范围写端口& 写入端口) {
    const auto 所有者 = 写入端口.所有者身份();
    const auto 所有者读取 = 第一层服务.读取当前结构所有者(
        {L1所有者范围CRUD合同版本, 所有者});
    if (所有者读取.状态 != L1所有者范围读取状态::成功
        || 所有者读取.合同版本 != L1所有者范围CRUD合同版本
        || 所有者读取.查询所有者 != 所有者 || !所有者读取.所有者事实
        || 所有者读取.所有者事实->所有者 != 所有者
        || 所有者读取.所有者事实->范围种类
            != L1所有者范围种类::独占结构范围
        || 所有者读取.所有者事实->创建事实代次 == 0
        || 所有者读取.所有者事实->退出事实代次)
        throw std::runtime_error("L2 task owner fact is invalid");

    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 所有者读取.所有者事实->创建事实代次;
    写集.写入幂等身份 = 任务身份来源登记幂等身份;
    写集.节点 = {
        {{任务锚点本地键值}, 节点种类::普通, std::nullopt},
        {{任务族归属关系类型本地键值}, 节点种类::普通, std::nullopt},
        {{任务族归属关系本地键值}, 节点种类::普通, std::nullopt},
        {{虚拟存在锚点本地键值}, 节点种类::普通, std::nullopt},
        {{虚拟存在归属关系类型本地键值}, 节点种类::普通, std::nullopt},
        {{虚拟存在归属关系本地键值}, 节点种类::普通, std::nullopt},
        {{路径族归属关系类型本地键值}, 节点种类::普通, std::nullopt},
        {{路径族归属关系本地键值}, 节点种类::普通, std::nullopt},
        {{实例方法族归属关系类型本地键值}, 节点种类::普通, std::nullopt},
        {{实例方法族归属关系本地键值}, 节点种类::普通, std::nullopt},
        {{路径筹办轮次属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::I64},
        {{路径来源方法属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::I64},
        {{路径来源方法内容版本属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::I64},
        {{路径条件绑定属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::独立材料引用},
        {{路径输入绑定属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::独立材料引用},
        {{路径参数绑定属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::独立材料引用},
        {{路径动作入口属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::独立材料引用},
        {{路径预期结果属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::独立材料引用},
        {{路径验证合同属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::独立材料引用},
        {{路径归因合同属性类型本地键值}, 节点种类::属性类型,
            L1所有者范围值表示种类::独立材料引用}};
    const auto 写入 = 写入端口.提交所有者范围中性写集(写集);
    const bool 首次完整 = 写入.状态 == L1所有者范围写入状态::成功
        && 写入.是否形成内存权威发布
        && 写入.重试边界 == L1所有者范围重试边界::不适用;
    const bool 重复完整 = 写入.状态 == L1所有者范围写入状态::精确重复
        && !写入.是否形成内存权威发布
        && 写入.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛;
    if (写入.合同版本 != L1所有者范围CRUD合同版本
        || 写入.所有者 != 所有者
        || 写入.写入幂等身份 != 任务身份来源登记幂等身份
        || 写入.事实代次 == 0 || (!首次完整 && !重复完整)
        || 写入.新编码映射.size() != 20)
        throw std::runtime_error("L2 task identity source init failed");

    稳定编码 任务锚点, 任务族类型, 任务族关系;
    稳定编码 虚拟存在锚点, 虚拟存在类型, 虚拟存在关系;
    稳定编码 路径族类型, 路径族关系;
    稳定编码 实例方法族类型, 实例方法族关系;
    稳定编码 路径筹办轮次属性类型, 路径来源方法属性类型,
        路径来源方法内容版本属性类型;
    稳定编码 路径条件绑定属性类型, 路径输入绑定属性类型,
        路径参数绑定属性类型, 路径动作入口属性类型,
        路径预期结果属性类型, 路径验证合同属性类型,
        路径归因合同属性类型;
    for (const auto& [本地键, 编码] : 写入.新编码映射) {
        if (!有效(编码))
            throw std::runtime_error("L2 task id source mapping invalid");
        if (本地键.值 == 任务锚点本地键值) 任务锚点 = 编码;
        else if (本地键.值 == 任务族归属关系类型本地键值)
            任务族类型 = 编码;
        else if (本地键.值 == 任务族归属关系本地键值)
            任务族关系 = 编码;
        else if (本地键.值 == 虚拟存在锚点本地键值)
            虚拟存在锚点 = 编码;
        else if (本地键.值 == 虚拟存在归属关系类型本地键值)
            虚拟存在类型 = 编码;
        else if (本地键.值 == 虚拟存在归属关系本地键值)
            虚拟存在关系 = 编码;
        else if (本地键.值 == 路径族归属关系类型本地键值)
            路径族类型 = 编码;
        else if (本地键.值 == 路径族归属关系本地键值)
            路径族关系 = 编码;
        else if (本地键.值 == 实例方法族归属关系类型本地键值)
            实例方法族类型 = 编码;
        else if (本地键.值 == 实例方法族归属关系本地键值)
            实例方法族关系 = 编码;
        else if (本地键.值 == 路径筹办轮次属性类型本地键值)
            路径筹办轮次属性类型 = 编码;
        else if (本地键.值 == 路径来源方法属性类型本地键值)
            路径来源方法属性类型 = 编码;
        else if (本地键.值 == 路径来源方法内容版本属性类型本地键值)
            路径来源方法内容版本属性类型 = 编码;
        else if (本地键.值 == 路径条件绑定属性类型本地键值)
            路径条件绑定属性类型 = 编码;
        else if (本地键.值 == 路径输入绑定属性类型本地键值)
            路径输入绑定属性类型 = 编码;
        else if (本地键.值 == 路径参数绑定属性类型本地键值)
            路径参数绑定属性类型 = 编码;
        else if (本地键.值 == 路径动作入口属性类型本地键值)
            路径动作入口属性类型 = 编码;
        else if (本地键.值 == 路径预期结果属性类型本地键值)
            路径预期结果属性类型 = 编码;
        else if (本地键.值 == 路径验证合同属性类型本地键值)
            路径验证合同属性类型 = 编码;
        else if (本地键.值 == 路径归因合同属性类型本地键值)
            路径归因合同属性类型 = 编码;
        else throw std::runtime_error("L2 task id source mapping unknown");
    }
    const 稳定编码 编码组[] = {任务锚点, 任务族类型, 任务族关系,
        虚拟存在锚点, 虚拟存在类型, 虚拟存在关系,
        路径族类型, 路径族关系, 实例方法族类型, 实例方法族关系,
        路径筹办轮次属性类型, 路径来源方法属性类型,
        路径来源方法内容版本属性类型,
        路径条件绑定属性类型, 路径输入绑定属性类型,
        路径参数绑定属性类型, 路径动作入口属性类型,
        路径预期结果属性类型, 路径验证合同属性类型,
        路径归因合同属性类型};
    for (std::size_t 左 = 0; 左 < 20; ++左) {
        if (!有效(编码组[左]))
            throw std::runtime_error(
                "L2 task identity source mapping incomplete");
        for (std::size_t 右 = 0; 右 < 左; ++右)
            if (编码组[左] == 编码组[右])
                throw std::runtime_error(
                    "L2 task identity source mapping duplicate");
    }

    const auto 检查节点 = [&](稳定编码 编码,
        std::optional<L1所有者范围值表示种类> 期望表示 = std::nullopt) {
        const auto 读取 = 第一层服务.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 编码});
        const auto* 节点 = 读取.事实
            ? std::get_if<L1所有者范围节点事实>(&*读取.事实) : nullptr;
        if (读取.状态 != L1所有者范围读取状态::成功
            || 读取.合同版本 != L1所有者范围CRUD合同版本
            || 读取.查询编码 != 编码 || !节点 || 节点->编码 != 编码
            || 节点->写入所有者 != 所有者
            || !节点->当前属性.empty()
            || 节点->创建事实代次 != 写入.事实代次
            || 节点->退出事实代次)
            return false;
        if (期望表示)
            return 节点->种类 == 节点种类::属性类型
                && 节点->属性类型表示
                && *节点->属性类型表示 == *期望表示;
        return 节点->种类 == 节点种类::普通
            && !节点->属性类型表示;
    };
    for (std::size_t i = 0; i < 20; ++i) {
        std::optional<L1所有者范围值表示种类> 期望表示;
        if (i >= 10 && i < 13)
            期望表示 = L1所有者范围值表示种类::I64;
        else if (i >= 13)
            期望表示 = L1所有者范围值表示种类::独立材料引用;
        if (!检查节点(编码组[i], 期望表示))
            throw std::runtime_error(
                "L2 task identity source readback invalid");
    }

    L1所有者范围写集请求 v2写集;
    v2写集.合同版本 = L1所有者范围CRUD合同版本;
    v2写集.期望事实代次 = 写入.事实代次;
    v2写集.写入幂等身份 = 任务路径动作组v2登记幂等身份;
    v2写集.节点 = {{{路径完整有序动作组属性类型本地键值_v2},
        节点种类::属性类型, L1所有者范围值表示种类::U64组}};
    const auto v2写入 = 写入端口.提交所有者范围中性写集(v2写集);
    const bool v2首次完整 = v2写入.状态 == L1所有者范围写入状态::成功
        && v2写入.是否形成内存权威发布
        && v2写入.重试边界 == L1所有者范围重试边界::不适用;
    const bool v2重复完整 =
        v2写入.状态 == L1所有者范围写入状态::精确重复
        && !v2写入.是否形成内存权威发布
        && v2写入.重试边界
            == L1所有者范围重试边界::原幂等身份读回收敛;
    if (v2写入.合同版本 != L1所有者范围CRUD合同版本
        || v2写入.所有者 != 所有者
        || v2写入.写入幂等身份 != 任务路径动作组v2登记幂等身份
        || v2写入.事实代次 == 0 || (!v2首次完整 && !v2重复完整)
        || v2写入.新编码映射.size() != 1
        || v2写入.新编码映射.front().first.值
            != 路径完整有序动作组属性类型本地键值_v2
        || !有效(v2写入.新编码映射.front().second))
        throw std::runtime_error("L2 task path action group v2 registry failed");
    const auto 路径完整有序动作组属性类型_v2 =
        v2写入.新编码映射.front().second;
    const auto v2读回 = 第一层服务.读取所有者范围历史事实({
        L1所有者范围CRUD合同版本, 路径完整有序动作组属性类型_v2});
    const auto* v2节点 = v2读回.事实
        ? std::get_if<L1所有者范围节点事实>(&*v2读回.事实) : nullptr;
    if (v2读回.状态 != L1所有者范围读取状态::成功 || !v2节点
        || v2节点->写入所有者 != 所有者
        || v2节点->种类 != 节点种类::属性类型
        || v2节点->属性类型表示
            != L1所有者范围值表示种类::U64组)
        throw std::runtime_error("L2 task path action group v2 readback failed");

    任务身份来源定位 定位;
    定位.所有者 = 所有者;
    定位.任务锚点 = 任务锚点;
    定位.任务族归属关系类型 = 任务族类型;
    定位.任务族归属关系 = 任务族关系;
    定位.虚拟存在锚点 = 虚拟存在锚点;
    定位.虚拟存在归属关系类型 = 虚拟存在类型;
    定位.虚拟存在归属关系 = 虚拟存在关系;
    定位.路径族归属关系类型 = 路径族类型;
    定位.路径族归属关系 = 路径族关系;
    定位.实例方法族归属关系类型 = 实例方法族类型;
    定位.实例方法族归属关系 = 实例方法族关系;
    定位.路径筹办轮次属性类型 = 路径筹办轮次属性类型;
    定位.路径来源方法属性类型 = 路径来源方法属性类型;
    定位.路径来源方法内容版本属性类型 = 路径来源方法内容版本属性类型;
    定位.路径条件绑定属性类型 = 路径条件绑定属性类型;
    定位.路径输入绑定属性类型 = 路径输入绑定属性类型;
    定位.路径参数绑定属性类型 = 路径参数绑定属性类型;
    定位.路径动作入口属性类型 = 路径动作入口属性类型;
    定位.路径完整有序动作组属性类型_v2 = 路径完整有序动作组属性类型_v2;
    定位.路径预期结果属性类型 = 路径预期结果属性类型;
    定位.路径验证合同属性类型 = 路径验证合同属性类型;
    定位.路径归因合同属性类型 = 路径归因合同属性类型;
    定位.建立事实代次 = 写入.事实代次;
    return 定位;
}

// ===== 结构类型初始化 =====
任务结构类型定位 初始化任务结构类型(
    const L1事实基座服务& 第一层服务,
    L1所有者范围写端口& 写入端口,
    const 任务身份来源定位& 来源定位) {
    const auto 所有者 = 来源定位.所有者;
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 读取固定登记期望代次(
        第一层服务, 写入端口, 任务结构类型登记幂等身份);
    写集.写入幂等身份 = 任务结构类型登记幂等身份;
    写集.节点 = {
        {{任务族归属关系类型键值}, 节点种类::普通, std::nullopt},
        {{任务需求列表项引用关系类型键值}, 节点种类::普通, std::nullopt},
        {{任务虚拟存在归属关系类型键值}, 节点种类::普通, std::nullopt},
        {{路径族归属关系类型键值}, 节点种类::普通, std::nullopt},
        {{路径任务引用关系类型键值}, 节点种类::普通, std::nullopt},
        {{当前选中路径关系类型键值}, 节点种类::普通, std::nullopt},
        {{实例方法族归属关系类型键值}, 节点种类::普通, std::nullopt},
        {{实例方法任务引用关系类型键值}, 节点种类::普通, std::nullopt},
        {{实例方法路径引用关系类型键值}, 节点种类::普通, std::nullopt},
        {{实例方法当前关系类型键值}, 节点种类::普通, std::nullopt}};
    const auto 写入 = 写入端口.提交所有者范围中性写集(写集);
    const bool 首次完整 = 写入.状态 == L1所有者范围写入状态::成功
        && 写入.是否形成内存权威发布
        && 写入.重试边界 == L1所有者范围重试边界::不适用;
    const bool 重复完整 = 写入.状态 == L1所有者范围写入状态::精确重复
        && !写入.是否形成内存权威发布
        && 写入.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛;
    if (写入.合同版本 != L1所有者范围CRUD合同版本
        || 写入.所有者 != 所有者
        || 写入.写入幂等身份 != 任务结构类型登记幂等身份
        || 写入.事实代次 == 0 || (!首次完整 && !重复完整)
        || 写入.新编码映射.size() != 10)
        throw std::runtime_error("L2 task type registry init failed");

    任务结构类型定位 定位{};
    for (const auto& [本地键, 编码] : 写入.新编码映射) {
        if (!有效(编码))
            throw std::runtime_error("L2 task type mapping invalid");
        if (本地键.值 == 任务族归属关系类型键值)
            定位.任务族归属关系类型 = 编码;
        else if (本地键.值 == 任务需求列表项引用关系类型键值)
            定位.任务需求列表项引用关系类型 = 编码;
        else if (本地键.值 == 任务虚拟存在归属关系类型键值)
            定位.任务虚拟存在归属关系类型 = 编码;
        else if (本地键.值 == 路径族归属关系类型键值)
            定位.路径族归属关系类型 = 编码;
        else if (本地键.值 == 路径任务引用关系类型键值)
            定位.路径任务引用关系类型 = 编码;
        else if (本地键.值 == 当前选中路径关系类型键值)
            定位.当前选中路径关系类型 = 编码;
        else if (本地键.值 == 实例方法族归属关系类型键值)
            定位.实例方法族归属关系类型 = 编码;
        else if (本地键.值 == 实例方法任务引用关系类型键值)
            定位.实例方法任务引用关系类型 = 编码;
        else if (本地键.值 == 实例方法路径引用关系类型键值)
            定位.实例方法路径引用关系类型 = 编码;
        else if (本地键.值 == 实例方法当前关系类型键值)
            定位.实例方法当前关系类型 = 编码;
        else
            throw std::runtime_error("L2 task type mapping unknown");
    }
    定位.建立事实代次 = 写入.事实代次;
    return 定位;
}

任务初次筹办治理定位 初始化任务初次筹办治理登记(
    const L1事实基座服务& 第一层服务,
    L1所有者范围写端口& 写入端口,
    任务结构类型定位& 类型定位,
    稳定编码 任务轮次锚点,
    稳定编码 任务轮次族归属关系类型,
    稳定编码 任务轮次任务关系类型,
    稳定编码 任务当前轮次关系类型,
    稳定编码 任务轮次序号属性类型) {
    const auto 键 = [](std::uint32_t 值) noexcept {
        return L1所有者范围写集本地键{值};
    };
    const auto 提交 = [&](L1所有者范围写集请求 写集,
        std::size_t 映射数量, const char* 错误) {
        写集.合同版本 = L1所有者范围CRUD合同版本;
        写集.期望事实代次 = 读取固定登记期望代次(
            第一层服务, 写入端口, 写集.写入幂等身份);
        const auto 写入 = 写入端口.提交所有者范围中性写集(写集);
        const bool 首次 = 写入.状态 == L1所有者范围写入状态::成功
            && 写入.是否形成内存权威发布
            && 写入.重试边界 == L1所有者范围重试边界::不适用;
        const bool 重复 = 写入.状态 == L1所有者范围写入状态::精确重复
            && !写入.是否形成内存权威发布
            && 写入.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛;
        if (写入.合同版本 != L1所有者范围CRUD合同版本
            || 写入.所有者 != 写入端口.所有者身份()
            || 写入.写入幂等身份 != 写集.写入幂等身份
            || 写入.事实代次 == 0 || (!首次 && !重复)
            || 写入.新编码映射.size() != 映射数量)
            throw std::runtime_error(错误);
        return 写入;
    };

    L1所有者范围写集请求 核心类型写集;
    核心类型写集.写入幂等身份 = {0x4C32'5452'434F'5245ULL};
    核心类型写集.节点 = {
        {键(任务来源需求关系类型键值), 节点种类::普通, std::nullopt},
        {键(任务正式存在关系类型键值), 节点种类::普通, std::nullopt}};
    const auto 核心类型写入 = 提交(std::move(核心类型写集), 2,
        "L2 task round core type registry init failed");
    for (const auto& [本地键, 编码] : 核心类型写入.新编码映射) {
        if (!有效(编码))
            throw std::runtime_error("L2 task round core type mapping invalid");
        if (本地键.值 == 任务来源需求关系类型键值)
            类型定位.任务来源需求关系类型 = 编码;
        else if (本地键.值 == 任务正式存在关系类型键值)
            类型定位.任务正式存在关系类型 = 编码;
        else throw std::runtime_error("L2 task round core type mapping unknown");
    }

    任务初次筹办治理定位 定位;
    定位.任务轮次锚点 = 任务轮次锚点;
    定位.任务轮次族归属关系类型 = 任务轮次族归属关系类型;
    定位.任务轮次任务关系类型 = 任务轮次任务关系类型;
    定位.任务当前轮次关系类型 = 任务当前轮次关系类型;
    定位.任务轮次序号属性类型 = 任务轮次序号属性类型;

    L1所有者范围写集请求 定义写集;
    定义写集.写入幂等身份 = {0x4C32'5447'464E'4446ULL};
    定义写集.节点 = {
        {键(治理函数定义节点键), 节点种类::普通, std::nullopt},
        {键(治理函数定义材料属性类型键), 节点种类::属性类型,
            L1所有者范围值表示种类::U64组}};
    定义写集.值 = {{键(治理函数定义材料值写入键),
        键(治理函数定义节点键), 键(治理函数定义材料属性类型键),
        std::vector<std::uint64_t>{1, L2任务治理函数合同版本,
            L2初次筹办推进规则版本}, 键(治理函数定义节点键)}};
    定义写集.属性槽变更 = {{键(治理函数定义节点键),
        键(治理函数定义材料属性类型键), 键(治理函数定义材料值写入键)}};
    const auto 定义写入 = 提交(std::move(定义写集), 3,
        "L2 task governance definition registry init failed");

    L1所有者范围写集请求 调用写集;
    调用写集.写入幂等身份 = {0x4C32'5447'4143'544EULL};
    调用写集.节点.push_back(
        {键(治理动作调用锚点键), 节点种类::普通, std::nullopt});
    for (std::uint32_t 偏移 = 0; 偏移 < 6; ++偏移)
        调用写集.节点.push_back({键(治理动作调用关系类型起始键 + 偏移),
            节点种类::普通, std::nullopt});
    调用写集.节点.push_back({键(治理动作调用材料属性类型键),
        节点种类::属性类型, L1所有者范围值表示种类::U64组});
    const auto 调用写入 = 提交(std::move(调用写集), 8,
        "L2 task governance action registry init failed");

    const auto 接纳 = [&](const L1所有者范围写入结果& 写入) {
        for (const auto& [本地键, 编码] : 写入.新编码映射) {
            if (!有效(编码))
                throw std::runtime_error("L2 task initial governance mapping invalid");
            switch (本地键.值) {
            case 治理函数定义节点键:
                定位.治理函数定义节点 = 编码; break;
            case 治理函数定义材料属性类型键:
                定位.治理函数定义材料属性类型 = 编码; break;
            case 治理动作调用锚点键:
                定位.治理动作调用锚点 = 编码; break;
            case 治理动作调用关系类型起始键 + 0:
                定位.治理动作族归属关系类型 = 编码; break;
            case 治理动作调用关系类型起始键 + 1:
                定位.治理动作函数关系类型 = 编码; break;
            case 治理动作调用关系类型起始键 + 2:
                定位.治理动作任务关系类型 = 编码; break;
            case 治理动作调用关系类型起始键 + 3:
                定位.治理动作轮次关系类型 = 编码; break;
            case 治理动作调用关系类型起始键 + 4:
                定位.治理动作筹办轮次关系类型 = 编码; break;
            case 治理动作调用关系类型起始键 + 5:
                定位.治理动作触发前态关系类型 = 编码; break;
            case 治理动作调用材料属性类型键:
                定位.治理动作调用材料属性类型 = 编码; break;
            case 治理函数定义材料值写入键: break;
            default: throw std::runtime_error(
                "L2 task initial governance mapping unknown");
            }
        }
    };
    接纳(定义写入); 接纳(调用写入);
    const 稳定编码 必需[] = {类型定位.任务来源需求关系类型,
        类型定位.任务正式存在关系类型, 定位.任务轮次锚点,
        定位.任务轮次族归属关系类型, 定位.任务轮次任务关系类型,
        定位.任务当前轮次关系类型, 定位.任务轮次序号属性类型,
        定位.治理函数定义节点, 定位.治理函数定义材料属性类型,
        定位.治理动作调用锚点, 定位.治理动作族归属关系类型,
        定位.治理动作函数关系类型, 定位.治理动作任务关系类型,
        定位.治理动作轮次关系类型, 定位.治理动作筹办轮次关系类型,
        定位.治理动作触发前态关系类型, 定位.治理动作调用材料属性类型};
    if (std::any_of(std::begin(必需), std::end(必需),
            [](稳定编码 编码) noexcept { return !有效(编码); }))
        throw std::runtime_error("L2 task initial governance registry incomplete");
    定位.建立事实代次 = 调用写入.事实代次;
    return 定位;
}

任务轮次结算登记定位 初始化任务轮次结算登记(
    const L1事实基座服务& 第一层服务,
    L1所有者范围写端口& 写入端口,
    const 任务初次筹办治理定位& 治理) {
    const auto 键 = [](std::uint32_t 值) noexcept {
        return L1所有者范围写集本地键{值};
    };
    L1所有者范围写集请求 写集;
    const auto 首次 = 写入端口.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本,
            任务轮次结算登记幂等身份});
    if (首次.状态 == L1所有者范围读取状态::成功
        && 首次.首次规范化写集) {
        写集 = *首次.首次规范化写集;
    } else {
        const auto 当前 = 读取中性当前事实代次(第一层服务);
        if (当前 == 0)
            throw std::runtime_error("L2 task settlement registry cutoff unavailable");
        写集.合同版本 = L1所有者范围CRUD合同版本;
        写集.期望事实代次 = 当前;
        写集.写入幂等身份 = 任务轮次结算登记幂等身份;
        const auto 普通 = [&](std::uint32_t 本地键) {
            写集.节点.push_back({键(本地键), 节点种类::普通, std::nullopt});
        };
        const auto U64组 = [&](std::uint32_t 本地键) {
            写集.节点.push_back({键(本地键), 节点种类::属性类型,
                L1所有者范围值表示种类::U64组});
        };
        for (std::uint32_t 本地键 = 任务轮次结算登记起始键;
            本地键 <= 任务轮次结算验证归因收口关系类型键; ++本地键) {
            if (本地键 == 任务轮次结算材料属性类型键) U64组(本地键);
            else 普通(本地键);
        }
        普通(自我继续治理函数定义键);
        写集.值.push_back({键(自我继续治理函数定义材料值键),
            键(自我继续治理函数定义键), 治理.治理函数定义材料属性类型,
            std::vector<std::uint64_t>{
                static_cast<std::uint64_t>(L2任务治理函数种类::自我继续开启新轮次),
                L2任务治理函数合同版本, L2任务后继机械规则版本},
            键(自我继续治理函数定义键)});
        写集.属性槽变更.push_back({键(自我继续治理函数定义键),
            治理.治理函数定义材料属性类型,
            键(自我继续治理函数定义材料值键)});
        for (std::uint32_t 本地键 = 任务后继决议引用锚点键;
            本地键 <= 任务后继决议引用材料属性类型键; ++本地键) {
            if (本地键 == 任务后继决议引用材料属性类型键) U64组(本地键);
            else 普通(本地键);
        }
        for (std::uint32_t 本地键 = 任务生命周期收口锚点键;
            本地键 <= 任务生命周期收口材料属性类型键; ++本地键) {
            if (本地键 == 任务生命周期收口材料属性类型键) U64组(本地键);
            else 普通(本地键);
        }
        普通(轮次收束治理函数定义键);
        写集.值.push_back({键(轮次收束治理函数定义材料值键),
            键(轮次收束治理函数定义键), 治理.治理函数定义材料属性类型,
            std::vector<std::uint64_t>{
                static_cast<std::uint64_t>(L2任务治理函数种类::轮次收束),
                L2任务治理函数合同版本, L2任务轮次结算规则版本},
            键(轮次收束治理函数定义键)});
        写集.属性槽变更.push_back({键(轮次收束治理函数定义键),
            治理.治理函数定义材料属性类型,
            键(轮次收束治理函数定义材料值键)});
        for (std::uint32_t 本地键 = 任务轮次结算治理动作关系类型键;
            本地键 <= 筹办权威初始阶段状态关系类型键; ++本地键)
            普通(本地键);
    }
    const auto 写入 = 写入端口.提交所有者范围中性写集(写集);
    const bool 首次完整 = 写入.状态 == L1所有者范围写入状态::成功
        && 写入.是否形成内存权威发布
        && 写入.重试边界 == L1所有者范围重试边界::不适用;
    const bool 重复完整 = 写入.状态 == L1所有者范围写入状态::精确重复
        && !写入.是否形成内存权威发布
        && 写入.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛;
    if (写入.合同版本 != L1所有者范围CRUD合同版本
        || 写入.所有者 != 写入端口.所有者身份()
        || 写入.写入幂等身份 != 任务轮次结算登记幂等身份
        || 写入.事实代次 == 0 || (!首次完整 && !重复完整)
        || 写入.新编码映射.size() != 37U)
        throw std::runtime_error("L2 task settlement registry failed");

    任务轮次结算登记定位 定位;
    std::uint32_t 已接纳 = 0;
    const auto 设 = [&](稳定编码& 目标, 稳定编码 编码) {
        if (有效(目标) || !有效(编码))
            throw std::runtime_error("L2 task settlement registry duplicate mapping");
        目标 = 编码; ++已接纳;
    };
    for (const auto& [本地键, 编码] : 写入.新编码映射) {
        switch (本地键.值) {
        case 任务轮次结算锚点键: 设(定位.结算锚点, 编码); break;
        case 任务轮次结算族关系类型键: 设(定位.结算族关系类型, 编码); break;
        case 任务轮次结算任务关系类型键: 设(定位.任务关系类型, 编码); break;
        case 任务轮次结算任务轮次关系类型键: 设(定位.任务轮次关系类型, 编码); break;
        case 任务轮次结算实际结果关系类型键: 设(定位.实际结果关系类型, 编码); break;
        case 任务轮次结算消费分配关系类型键: 设(定位.消费分配记录关系类型, 编码); break;
        case 任务轮次结算需求核算关系类型键: 设(定位.需求核算关系类型, 编码); break;
        case 任务轮次结算普通价值关系类型键: 设(定位.普通价值结算关系类型, 编码); break;
        case 任务轮次结算学习变化关系类型键: 设(定位.学习能力变化关系类型, 编码); break;
        case 任务轮次结算执行冻结收口关系类型键: 设(定位.执行冻结收口关系类型, 编码); break;
        case 任务轮次结算授权关系类型键: 设(定位.授权关系类型, 编码); break;
        case 任务轮次结算授权收口关系类型键: 设(定位.授权收口关系类型, 编码); break;
        case 任务轮次结算运行停止关系类型键: 设(定位.任务运行停止证据关系类型, 编码); break;
        case 任务轮次结算材料属性类型键: 设(定位.结算材料属性类型, 编码); break;
        case 任务轮次结算验证归因收口关系类型键: 设(定位.验证归因收口关系类型, 编码); break;
        case 自我继续治理函数定义键: 设(定位.自我继续治理函数定义, 编码); break;
        case 自我继续治理函数定义材料值键: ++已接纳; break;
        case 任务后继决议引用锚点键: 设(定位.决议引用锚点, 编码); break;
        case 任务后继决议引用族关系类型键: 设(定位.决议引用族关系类型, 编码); break;
        case 任务后继决议引用任务关系类型键: 设(定位.决议引用任务关系类型, 编码); break;
        case 任务后继决议引用轮次关系类型键: 设(定位.决议引用轮次关系类型, 编码); break;
        case 任务后继决议引用自我决议关系类型键: 设(定位.决议引用自我决议关系类型, 编码); break;
        case 任务后继决议引用材料属性类型键: 设(定位.决议引用材料属性类型, 编码); break;
        case 任务生命周期收口锚点键: 设(定位.生命周期收口锚点, 编码); break;
        case 任务生命周期收口族关系类型键: 设(定位.生命周期收口族关系类型, 编码); break;
        case 任务生命周期收口任务关系类型键: 设(定位.生命周期收口任务关系类型, 编码); break;
        case 任务生命周期收口轮次关系类型键: 设(定位.生命周期收口轮次关系类型, 编码); break;
        case 任务生命周期收口结算关系类型键: 设(定位.生命周期收口结算关系类型, 编码); break;
        case 任务生命周期收口决议关系类型键: 设(定位.生命周期收口决议关系类型, 编码); break;
        case 任务生命周期收口停止证据关系类型键: 设(定位.生命周期收口停止证据关系类型, 编码); break;
        case 任务生命周期收口材料属性类型键: 设(定位.生命周期收口材料属性类型, 编码); break;
        case 轮次收束治理函数定义键: 设(定位.轮次收束治理函数定义, 编码); break;
        case 轮次收束治理函数定义材料值键: ++已接纳; break;
        case 任务轮次结算治理动作关系类型键: 设(定位.结算治理动作关系类型, 编码); break;
        case 筹办权威任务轮次关系类型键: 设(定位.筹办权威任务轮次关系类型, 编码); break;
        case 筹办权威阶段特征实例关系类型键: 设(定位.筹办权威阶段特征实例关系类型, 编码); break;
        case 筹办权威初始阶段状态关系类型键: 设(定位.筹办权威初始阶段状态关系类型, 编码); break;
        default: throw std::runtime_error("L2 task settlement registry unknown mapping");
        }
    }
    if (已接纳 != 37U)
        throw std::runtime_error("L2 task settlement registry incomplete");
    定位.建立事实代次 = 写入.事实代次;
    return 定位;
}

// 任务实际结果的登记独立于既有任务登记；不得复用旧幂等身份重放。
void 初始化任务实际结果登记(
    const L1事实基座服务& 第一层服务,
    L1所有者范围写端口& 写入端口,
    任务身份来源定位& 来源定位,
    任务结构类型定位& 类型定位) {
    const auto 键 = [](std::uint32_t 值) noexcept {
        return L1所有者范围写集本地键{值};
    };
    L1所有者范围写集请求 身份写集;
    身份写集.合同版本 = L1所有者范围CRUD合同版本;
    身份写集.期望事实代次 = 读取固定登记期望代次(
        第一层服务, 写入端口, 任务实际结果身份来源登记幂等身份);
    身份写集.写入幂等身份 = 任务实际结果身份来源登记幂等身份;
    身份写集.节点 = {
        {键(实例方法执行轮次属性类型本地键值), 节点种类::属性类型,
            L1所有者范围值表示种类::I64},
        {键(结果锚点本地键值), 节点种类::普通, std::nullopt},
        {键(结果族归属关系本地键值), 节点种类::普通, std::nullopt},
        {键(结果来源材料版本属性类型本地键值), 节点种类::属性类型,
            L1所有者范围值表示种类::U64组},
        {键(结果运行代次属性类型本地键值), 节点种类::属性类型,
            L1所有者范围值表示种类::U64组}};
    const auto 身份提交 = 写入端口.提交所有者范围中性写集(身份写集);
    if ((身份提交.状态 != L1所有者范围写入状态::成功
            && 身份提交.状态 != L1所有者范围写入状态::精确重复)
        || 身份提交.事实代次 == 0 || 身份提交.新编码映射.size() != 5)
        throw std::runtime_error("task result identity registry failed");
    for (const auto& [本地键, 编码] : 身份提交.新编码映射) {
        if (!有效(编码)) throw std::runtime_error("task result identity code invalid");
        if (本地键.值 == 实例方法执行轮次属性类型本地键值)
            来源定位.实例方法执行轮次属性类型 = 编码;
        else if (本地键.值 == 结果锚点本地键值) 来源定位.结果锚点 = 编码;
        else if (本地键.值 == 结果族归属关系本地键值) 来源定位.结果族归属关系 = 编码;
        else if (本地键.值 == 结果来源材料版本属性类型本地键值)
            来源定位.结果来源材料版本属性类型 = 编码;
        else if (本地键.值 == 结果运行代次属性类型本地键值)
            来源定位.结果运行代次属性类型 = 编码;
        else throw std::runtime_error("task result identity key unknown");
    }
    L1所有者范围写集请求 类型写集;
    类型写集.合同版本 = L1所有者范围CRUD合同版本;
    类型写集.期望事实代次 = 读取固定登记期望代次(
        第一层服务, 写入端口, 任务实际结果结构类型登记幂等身份);
    类型写集.写入幂等身份 = 任务实际结果结构类型登记幂等身份;
    类型写集.节点 = {
        {键(结果族归属关系类型键值), 节点种类::普通, std::nullopt},
        {键(任务实际结果槽关系类型键值), 节点种类::普通, std::nullopt},
        {键(结果实例方法关系类型键值), 节点种类::普通, std::nullopt},
        {键(结果实际状态关系类型键值), 节点种类::普通, std::nullopt},
        {键(结果动态证据关系类型键值), 节点种类::普通, std::nullopt},
        {键(结果方法关系类型键值), 节点种类::普通, std::nullopt},
        {键(结果动作入口关系类型键值), 节点种类::普通, std::nullopt},
        {键(结果场景关系类型键值), 节点种类::普通, std::nullopt},
        {键(结果主体关系类型键值), 节点种类::普通, std::nullopt}};
    const auto 类型提交 = 写入端口.提交所有者范围中性写集(类型写集);
    if ((类型提交.状态 != L1所有者范围写入状态::成功
            && 类型提交.状态 != L1所有者范围写入状态::精确重复)
        || 类型提交.事实代次 == 0 || 类型提交.新编码映射.size() != 9)
        throw std::runtime_error("task result type registry failed");
    for (const auto& [本地键, 编码] : 类型提交.新编码映射) {
        if (!有效(编码)) throw std::runtime_error("task result type code invalid");
        if (本地键.值 == 结果族归属关系类型键值) 类型定位.结果族归属关系类型 = 编码;
        else if (本地键.值 == 任务实际结果槽关系类型键值) 类型定位.任务实际结果槽关系类型 = 编码;
        else if (本地键.值 == 结果实例方法关系类型键值) 类型定位.结果实例方法关系类型 = 编码;
        else if (本地键.值 == 结果实际状态关系类型键值) 类型定位.结果实际状态关系类型 = 编码;
        else if (本地键.值 == 结果动态证据关系类型键值) 类型定位.结果动态证据关系类型 = 编码;
        else if (本地键.值 == 结果方法关系类型键值) 类型定位.结果方法关系类型 = 编码;
        else if (本地键.值 == 结果动作入口关系类型键值) 类型定位.结果动作入口关系类型 = 编码;
        else if (本地键.值 == 结果场景关系类型键值) 类型定位.结果场景关系类型 = 编码;
        else if (本地键.值 == 结果主体关系类型键值) 类型定位.结果主体关系类型 = 编码;
        else throw std::runtime_error("task result type key unknown");
    }
}

// 任务治理状态和目标裁决证据使用独立登记身份，避免改变既有四笔登记。
void 初始化任务治理状态登记(
    const L1事实基座服务& 第一层服务,
    L1所有者范围写端口& 写入端口,
    任务身份来源定位& 来源定位,
    任务结构类型定位& 类型定位) {
    const auto 键 = [](std::uint32_t 值) noexcept {
        return L1所有者范围写集本地键{值};
    };
    const auto 提交登记 = [&](L1所有者范围写集请求 写集,
        std::size_t 期望数量, const char* 错误) {
        const auto 提交 = 写入端口.提交所有者范围中性写集(写集);
        const bool 首次 = 提交.状态 == L1所有者范围写入状态::成功
            && 提交.是否形成内存权威发布
            && 提交.重试边界 == L1所有者范围重试边界::不适用;
        const bool 重复 = 提交.状态 == L1所有者范围写入状态::精确重复
            && !提交.是否形成内存权威发布
            && 提交.重试边界 ==
                L1所有者范围重试边界::原幂等身份读回收敛;
        if (提交.合同版本 != L1所有者范围CRUD合同版本
            || 提交.所有者 != 来源定位.所有者 || 提交.事实代次 == 0
            || (!首次 && !重复) || 提交.新编码映射.size() != 期望数量)
            throw std::runtime_error(错误);
        return 提交;
    };

    L1所有者范围写集请求 身份写集;
    身份写集.合同版本 = L1所有者范围CRUD合同版本;
    身份写集.期望事实代次 = 读取固定登记期望代次(
        第一层服务, 写入端口, 治理状态身份来源登记幂等身份);
    身份写集.写入幂等身份 = 治理状态身份来源登记幂等身份;
    身份写集.节点 = {
        {键(治理状态锚点本地键值), 节点种类::普通, std::nullopt},
        {键(治理状态族归属关系类型锚点本地键值), 节点种类::普通,
            std::nullopt},
        {键(治理状态族归属关系锚点本地键值), 节点种类::普通,
            std::nullopt},
        {键(目标裁决证据锚点本地键值), 节点种类::普通, std::nullopt},
        {键(目标裁决证据族归属关系类型锚点本地键值), 节点种类::普通,
            std::nullopt},
        {键(目标裁决证据族归属关系锚点本地键值), 节点种类::普通,
            std::nullopt},
        {键(治理状态生命周期发生时间属性类型本地键值),
            节点种类::属性类型, L1所有者范围值表示种类::I64},
        {键(治理状态材料属性类型本地键值), 节点种类::属性类型,
            L1所有者范围值表示种类::U64组},
        {键(目标裁决材料属性类型本地键值), 节点种类::属性类型,
            L1所有者范围值表示种类::U64组}};
    const auto 身份提交 = 提交登记(std::move(身份写集), 9,
        "task governance identity registry failed");
    for (const auto& [本地键, 编码] : 身份提交.新编码映射) {
        if (!有效(编码))
            throw std::runtime_error("task governance identity code invalid");
        switch (本地键.值) {
        case 治理状态锚点本地键值: 来源定位.治理状态锚点 = 编码; break;
        case 治理状态族归属关系类型锚点本地键值:
            来源定位.治理状态族归属关系类型锚点 = 编码; break;
        case 治理状态族归属关系锚点本地键值:
            来源定位.治理状态族归属关系锚点 = 编码; break;
        case 目标裁决证据锚点本地键值: 来源定位.目标裁决证据锚点 = 编码; break;
        case 目标裁决证据族归属关系类型锚点本地键值:
            来源定位.目标裁决证据族归属关系类型锚点 = 编码; break;
        case 目标裁决证据族归属关系锚点本地键值:
            来源定位.目标裁决证据族归属关系锚点 = 编码; break;
        case 治理状态生命周期发生时间属性类型本地键值:
            来源定位.治理状态生命周期发生时间属性类型 = 编码; break;
        case 治理状态材料属性类型本地键值:
            来源定位.治理状态材料属性类型 = 编码; break;
        case 目标裁决材料属性类型本地键值:
            来源定位.目标裁决材料属性类型 = 编码; break;
        default: throw std::runtime_error("task governance identity key unknown");
        }
    }

    L1所有者范围写集请求 类型写集;
    类型写集.合同版本 = L1所有者范围CRUD合同版本;
    类型写集.期望事实代次 = 读取固定登记期望代次(
        第一层服务, 写入端口, 治理状态结构类型登记幂等身份);
    类型写集.写入幂等身份 = 治理状态结构类型登记幂等身份;
    for (std::uint32_t 值 = 治理状态族归属关系类型键值;
        值 <= 证据比较注册关系类型键值; ++值)
        类型写集.节点.push_back({键(值), 节点种类::普通, std::nullopt});
    const auto 类型提交 = 提交登记(std::move(类型写集), 22,
        "task governance type registry failed");
    for (const auto& [本地键, 编码] : 类型提交.新编码映射) {
        if (!有效(编码))
            throw std::runtime_error("task governance type code invalid");
        switch (本地键.值) {
        case 治理状态族归属关系类型键值: 类型定位.治理状态族归属关系类型 = 编码; break;
        case 目标裁决证据族归属关系类型键值: 类型定位.目标裁决证据族归属关系类型 = 编码; break;
        case 任务当前治理状态关系类型键值: 类型定位.任务当前治理状态关系类型 = 编码; break;
        case 治理状态迁移证据关系类型键值: 类型定位.治理状态迁移证据关系类型 = 编码; break;
        case 状态虚拟存在关系类型键值: 类型定位.状态虚拟存在关系类型 = 编码; break;
        case 状态场景关系类型键值: 类型定位.状态场景关系类型 = 编码; break;
        case 状态前驱关系类型键值: 类型定位.状态前驱关系类型 = 编码; break;
        case 状态路径关系类型键值: 类型定位.状态路径关系类型 = 编码; break;
        case 状态实例关系类型键值: 类型定位.状态实例关系类型 = 编码; break;
        case 状态实际结果关系类型键值: 类型定位.状态实际结果关系类型 = 编码; break;
        case 状态目标裁决证据关系类型键值: 类型定位.状态目标裁决证据关系类型 = 编码; break;
        case 证据任务关系类型键值: 类型定位.证据任务关系类型 = 编码; break;
        case 证据实例关系类型键值: 类型定位.证据实例关系类型 = 编码; break;
        case 证据实际结果关系类型键值: 类型定位.证据实际结果关系类型 = 编码; break;
        case 证据目标来源关系类型键值: 类型定位.证据目标来源关系类型 = 编码; break;
        case 证据目标宿主关系类型键值: 类型定位.证据目标宿主关系类型 = 编码; break;
        case 证据目标特征关系类型键值: 类型定位.证据目标特征关系类型 = 编码; break;
        case 证据目标状态合同关系类型键值: 类型定位.证据目标状态合同关系类型 = 编码; break;
        case 证据实际状态关系类型键值: 类型定位.证据实际状态关系类型 = 编码; break;
        case 证据动态关系类型键值: 类型定位.证据动态关系类型 = 编码; break;
        case 证据方法关系类型键值: 类型定位.证据方法关系类型 = 编码; break;
        case 证据比较注册关系类型键值: 类型定位.证据比较注册关系类型 = 编码; break;
        default: throw std::runtime_error("task governance type key unknown");
        }
    }
}

// ===== 身份来源投影解释：辅助函数 =====

// 诊断责任：无适用错误分支；按精确编码查找唯一节点项。
const L1所有者范围一致节点读取结果项* 查找唯一任务节点项(
    const L1所有者范围一致当前读取结果& 读取,
    稳定编码 编码) noexcept {
    const L1所有者范围一致节点读取结果项* 找到 = nullptr;
    for (const auto& 项 : 读取.节点) {
        if (项.查询编码 != 编码) continue;
        if (找到) return nullptr;
        找到 = &项;
    }
    return 找到;
}

// 诊断责任：无适用错误分支；按精确源查找唯一来源关系组。
const L1所有者范围一致源关系组读取结果项* 查找唯一任务来源关系组(
    const L1所有者范围一致当前读取结果& 读取,
    稳定编码 源节点, 稳定编码 关系类型) noexcept {
    const L1所有者范围一致源关系组读取结果项* 找到 = nullptr;
    for (const auto& 项 : 读取.源关系组) {
        if (项.源节点 != 源节点
            || 项.关系类型节点 != 关系类型) continue;
        if (找到) return nullptr;
        找到 = &项;
    }
    return 找到;
}

// 诊断责任：无适用错误分支；L1 一致读取状态映射。
L2结构状态 映射任务身份来源一致读取失败状态(
    L1所有者范围一致当前读取状态 状态) noexcept {
    switch (状态) {
    case L1所有者范围一致当前读取状态::入口拒绝:
        return L2结构状态::入口拒绝;
    case L1所有者范围一致当前读取状态::许可拒绝:
        return L2结构状态::许可拒绝;
    case L1所有者范围一致当前读取状态::事实代次漂移:
        return L2结构状态::事实代次漂移;
    case L1所有者范围一致当前读取状态::资源失败:
        return L2结构状态::资源失败;
    case L1所有者范围一致当前读取状态::成功:
    case L1所有者范围一致当前读取状态::内部不一致:
        return L2结构状态::内部不一致;
    }
    return L2结构状态::内部不一致;
}

// 诊断责任：无适用错误分支；当前读取项目状态映射。
L2结构状态 映射任务身份来源当前项目状态(
    L1所有者范围一致当前读取项目状态 状态) noexcept {
    switch (状态) {
    case L1所有者范围一致当前读取项目状态::成功:
        return L2结构状态::已读取;
    case L1所有者范围一致当前读取项目状态::未找到:
        return L2结构状态::未找到;
    case L1所有者范围一致当前读取项目状态::已退出:
        return L2结构状态::已退出;
    case L1所有者范围一致当前读取项目状态::属性未设置:
    case L1所有者范围一致当前读取项目状态::种类不匹配:
        return L2结构状态::内部不一致;
    }
    return L2结构状态::内部不一致;
}

// 身份来源投影预检结果
struct 任务身份来源预检结果 final {
    L2结构状态 状态 = L2结构状态::内部不一致;
    std::uint64_t 事实截止代次 = 0;
    bool 代次漂移 = false;
    bool 成功() const noexcept {
        return 状态 == L2结构状态::已读取 && 事实截止代次 != 0;
    }
};

// 诊断责任：无适用错误分支；核对身份来源一致投影顶层成功或精确失败形状。
任务身份来源预检结果 解释任务身份来源一致读取顶层(
    const L1所有者范围一致当前读取结果& 读取,
    std::uint64_t 期望代次) noexcept {
    if (读取.状态 == L1所有者范围一致当前读取状态::成功) {
        if (读取.合同版本 == L1所有者范围一致当前读取合同版本
            && 读取.期望事实代次 == 期望代次
            && 读取.读取事实代次 == 期望代次
            && 读取.读取事实代次 != 0)
            return {L2结构状态::已读取, 读取.读取事实代次, false};
        return {L2结构状态::内部不一致, 读取.读取事实代次, false};
    }
    const bool 六组空 = 读取.所有者.empty() && 读取.节点.empty()
        && 读取.关系.empty() && 读取.值.empty()
        && 读取.属性值.empty() && 读取.源关系组.empty()
        && 读取.目标关系组.empty();
    const bool 代次完整 = 读取.状态
        == L1所有者范围一致当前读取状态::事实代次漂移
        ? 读取.读取事实代次 != 0 && 读取.读取事实代次 != 期望代次
        : 读取.读取事实代次 == 0;
    if (读取.合同版本 != L1所有者范围一致当前读取合同版本
        || 读取.期望事实代次 != 期望代次 || !六组空 || !代次完整)
        return {L2结构状态::内部不一致, 读取.读取事实代次, false};
    if (读取.状态 == L1所有者范围一致当前读取状态::事实代次漂移)
        return {L2结构状态::事实代次漂移,
            读取.读取事实代次, true};
    return {映射任务身份来源一致读取失败状态(读取.状态),
        读取.读取事实代次, false};
}

// 身份来源投影解释中间结果
struct 任务身份来源投影解释 final {
    L2结构状态 状态 = L2结构状态::内部不一致;
    std::uint64_t 事实截止代次 = 0;
    std::optional<L2任务身份来源事实> 来源;
};

// 诊断责任：向上送出；从已成功的一致投影中验证并构造任务节点族身份来源事实。
// 验证项：任务 owner 归属、任务族锚点证明、族归属关系类型证明、
//         族归属关系证明、角色或顺序、5 个生命周期。
任务身份来源投影解释 解释任务身份来源投影(
    const L1所有者范围一致当前读取结果& 读取,
    const 任务身份来源定位& 来源定位,
    稳定编码 任务编码,
    std::uint64_t 截止) {
    const auto 失败 = [&](L2结构状态 状态) noexcept {
        return 任务身份来源投影解释{状态, 截止, std::nullopt};
    };

    // 预期形状：1 所有者、3 节点、0 关系、0 值、1 源关系组、0 目标关系组
    if (读取.所有者.size() != 1 || 读取.节点.size() != 3
        || !读取.关系.empty() || !读取.值.empty()
        || 读取.源关系组.size() != 1
        || !读取.目标关系组.empty())
        return 失败(L2结构状态::内部不一致);

    const auto& 所有者项 = 读取.所有者.front();
    const auto* 节点项 = 查找唯一任务节点项(读取, 任务编码);
    const auto* 锚点项 = 查找唯一任务节点项(读取, 来源定位.任务锚点);
    const auto* 类型项 = 查找唯一任务节点项(读取, 来源定位.任务族归属关系类型);
    const auto* 族归属组项 = 查找唯一任务来源关系组(
        读取, 任务编码, 来源定位.任务族归属关系类型);
    if (所有者项.查询所有者 != 来源定位.所有者 || !节点项 || !锚点项
        || !类型项 || !族归属组项)
        return 失败(L2结构状态::内部不一致);

    // 任务节点：存在性、归属、当前性
    if (节点项->状态 != L1所有者范围一致当前读取项目状态::成功) {
        if (节点项->事实) return 失败(L2结构状态::内部不一致);
        return {映射任务身份来源当前项目状态(节点项->状态), 截止, std::nullopt};
    }
    if (!节点项->事实) return 失败(L2结构状态::内部不一致);
    const auto& 节点 = *节点项->事实;
    if (节点.写入所有者 != 来源定位.所有者
        || 节点.种类 != 节点种类::普通
        || 节点.属性类型表示 || 节点.退出事实代次)
        return 失败(L2结构状态::入口拒绝);

    // 所有者、族锚点、族归属关系类型：内部节点完整性
    if (所有者项.状态 != L1所有者范围一致当前读取项目状态::成功
        || !所有者项.所有者事实
        || 所有者项.所有者事实->所有者 != 来源定位.所有者
        || 所有者项.所有者事实->范围种类
            != L1所有者范围种类::独占结构范围
        || 所有者项.所有者事实->退出事实代次
        || 锚点项->状态 != L1所有者范围一致当前读取项目状态::成功
        || 类型项->状态 != L1所有者范围一致当前读取项目状态::成功
        || !锚点项->事实 || !类型项->事实)
        return 失败(L2结构状态::内部不一致);
    const auto& 锚点 = *锚点项->事实;
    const auto& 类型 = *类型项->事实;
    const auto 内部节点完整 = [&](
        const L1所有者范围节点事实& 内部节点) noexcept {
        return 内部节点.写入所有者 == 来源定位.所有者
            && 内部节点.种类 == 节点种类::普通
            && !内部节点.属性类型表示 && !内部节点.退出事实代次
            && 内部节点.当前属性.empty();
    };
    if (!内部节点完整(锚点) || !内部节点完整(类型)
        || 锚点.编码 != 来源定位.任务锚点
        || 类型.编码 != 来源定位.任务族归属关系类型
        || 锚点.创建事实代次 != 来源定位.建立事实代次
        || 类型.创建事实代次 != 来源定位.建立事实代次
        || 所有者项.所有者事实->创建事实代次
            > 来源定位.建立事实代次
        || 来源定位.建立事实代次 > 节点.创建事实代次)
        return 失败(L2结构状态::内部不一致);

    // 族归属关系：恰有一条当前族归属关系
    if (族归属组项->成员.size() != 1)
        return 失败(L2结构状态::内部不一致);
    const auto& 族归属关系 = 族归属组项->成员.front().关系;
    if (族归属关系.写入所有者 != 来源定位.所有者
        || 族归属关系.源节点 != 任务编码
        || 族归属关系.目标节点 != 来源定位.任务锚点
        || 族归属关系.关系类型节点 != 来源定位.任务族归属关系类型
        || 族归属关系.退出事实代次
        || 族归属关系.创建事实代次 != 节点.创建事实代次
        || 族归属关系.角色或顺序 != 1)
        return 失败(L2结构状态::内部不一致);

    // 构造 L2任务身份来源事实
    L2任务身份来源事实 来源;
    来源.任务所有者 = 来源定位.所有者;
    来源.任务节点 = 任务编码;
    来源.任务族锚点 = 来源定位.任务锚点;
    来源.任务族归属关系类型 = 来源定位.任务族归属关系类型;
    来源.任务族归属关系 = 族归属关系.编码;
    来源.任务节点生命周期 = {节点.创建事实代次, 节点.退出事实代次};
    来源.族归属关系生命周期 = {族归属关系.创建事实代次,
        族归属关系.退出事实代次};

    if (!L2任务身份来源事实完整(来源))
        return 失败(L2结构状态::内部不一致);

    return {L2结构状态::已读取, 截止, std::move(来源)};
}

// 诊断责任：向上送出；一次 owner-aware 当前投影读取任务身份来源事实。
L2任务身份来源读取结果 读取任务身份来源当前投影(
    const L1事实基座服务& 第一层服务,
    const 任务身份来源定位& 来源定位,
    const L2任务身份来源读取请求& 请求,
    std::uint64_t 期望代次) {
    if (请求.任务.值 == 来源定位.任务锚点
        || 请求.任务.值 == 来源定位.任务族归属关系类型) {
        L2任务身份来源读取结果 结果;
        结果.结果头.合同版本 = L2结构合同版本;
        结果.结果头.状态 = L2结构状态::入口拒绝;
        结果.结果头.事实截止代次 = 期望代次;
        return 结果;
    }

    L1所有者范围一致当前读取请求 L1请求;
    L1请求.合同版本 = L1所有者范围一致当前读取合同版本;
    L1请求.期望事实代次 = 期望代次;
    L1请求.所有者 = {来源定位.所有者};
    L1请求.节点 = {请求.任务.值, 来源定位.任务锚点,
        来源定位.任务族归属关系类型};
    L1请求.源关系组 = {{请求.任务.值, 来源定位.任务族归属关系类型}};
    const auto 读取 = 第一层服务.尝试读取所有者范围一致当前投影(L1请求);

    const auto 顶层 = 解释任务身份来源一致读取顶层(读取, 期望代次);
    if (!顶层.成功()) {
        L2任务身份来源读取结果 结果;
        结果.结果头.合同版本 = L2结构合同版本;
        结果.结果头.状态 = 顶层.状态;
        结果.结果头.事实截止代次 = 顶层.事实截止代次;
        return 结果;
    }

    const auto 解释 = 解释任务身份来源投影(
        读取, 来源定位, 请求.任务.值, 顶层.事实截止代次);
    L2任务身份来源读取结果 结果;
    结果.结果头.合同版本 = L2结构合同版本;
    结果.结果头.状态 = 解释.状态;
    结果.结果头.事实截止代次 = 解释.事实截止代次;
    结果.读取类别 = 请求.读取类别;
    结果.历史截止事实代次 = 请求.历史截止事实代次;
    结果.身份来源 = std::move(解释.来源);
    return 结果;
}

// ===== 写入：辅助函数 =====

// 诊断责任：无适用错误分支；纯值枚举映射覆盖全部中性写入状态。
L2结构状态 映射任务写入状态(
    L1所有者范围写入状态 状态) noexcept {
    switch (状态) {
    case L1所有者范围写入状态::成功: return L2结构状态::已提交;
    case L1所有者范围写入状态::精确重复: return L2结构状态::精确重复;
    case L1所有者范围写入状态::入口拒绝: return L2结构状态::入口拒绝;
    case L1所有者范围写入状态::许可拒绝: return L2结构状态::许可拒绝;
    case L1所有者范围写入状态::事实代次漂移: return L2结构状态::事实代次漂移;
    case L1所有者范围写入状态::幂等冲突: return L2结构状态::幂等冲突;
    case L1所有者范围写入状态::资源失败: return L2结构状态::资源失败;
    case L1所有者范围写入状态::引用冲突: return L2结构状态::引用冲突;
    case L1所有者范围写入状态::未找到:
    case L1所有者范围写入状态::已退出:
    case L1所有者范围写入状态::内部不一致:
        return L2结构状态::内部不一致;
    }
    return L2结构状态::内部不一致;
}

// 诊断责任：无适用错误分支；完整 64 位纯值映射不读取事实。
L1所有者范围写入幂等身份 映射任务幂等键(
    L2结构幂等身份 身份) noexcept {
    return {身份.值};
}

// 写入代次预检结果
struct 任务写入预检结果 final {
    L2结构状态 状态 = L2结构状态::内部不一致;
    std::uint64_t 事实截止代次 = 0;
    bool 直接提交 = false;
    bool 成功() const noexcept {
        return 状态 == L2结构状态::已读取;
    }
};

// 诊断责任：向上送出；检查当前事实代次是否匹配期望。
任务写入预检结果 检查任务写入当前代次(
    const L1事实基座服务& 第一层服务, std::uint64_t 期望代次) {
    const auto 读取 = 第一层服务.读取中性当前事实代次(
        {L1中性CRUD合同版本});
    if (读取.合同版本 != L1中性CRUD合同版本)
        return {L2结构状态::内部不一致, 读取.事实代次, false};
    if (读取.状态 != L1中性读取状态::成功)
        return {L2结构状态::内部不一致, 读取.事实代次, false};
    if (读取.事实代次 == 0)
        return {L2结构状态::内部不一致, 0, false};
    if (读取.事实代次 != 期望代次)
        return {L2结构状态::事实代次漂移, 读取.事实代次, true};
    return {L2结构状态::已读取, 读取.事实代次, false};
}

// 写入编码映射查找结果
struct 任务写入编码映射 final {
    稳定编码 任务节点;
    稳定编码 虚拟存在节点;
    稳定编码 任务族归属关系;
    稳定编码 任务需求列表项引用关系;
    稳定编码 任务虚拟存在归属关系;
};

// 诊断责任：无适用错误分支；按本地键从写入结果提取稳定编码。
std::optional<任务写入编码映射> 查找任务写入编码映射(
    const L1所有者范围写入结果& 写入) noexcept {
    if (写入.新编码映射.size() != 5) return std::nullopt;
    任务写入编码映射 映射;
    bool 找到任务节点 = false, 找到虚拟存在 = false;
    bool 找到族归属 = false, 找到列表项引用 = false, 找到虚拟归属 = false;
    for (const auto& [本地键, 编码] : 写入.新编码映射) {
        if (!有效(编码)) return std::nullopt;
        if (本地键.值 == 任务节点写入本地键值) {
            映射.任务节点 = 编码; 找到任务节点 = true;
        } else if (本地键.值 == 虚拟存在节点写入本地键值) {
            映射.虚拟存在节点 = 编码; 找到虚拟存在 = true;
        } else if (本地键.值 == 任务族归属关系写入本地键值) {
            映射.任务族归属关系 = 编码; 找到族归属 = true;
        } else if (本地键.值 == 任务需求列表项引用关系写入本地键值) {
            映射.任务需求列表项引用关系 = 编码; 找到列表项引用 = true;
        } else if (本地键.值 == 任务虚拟存在归属关系写入本地键值) {
            映射.任务虚拟存在归属关系 = 编码; 找到虚拟归属 = true;
        } else return std::nullopt;
    }
    if (!找到任务节点 || !找到虚拟存在 || !找到族归属
        || !找到列表项引用 || !找到虚拟归属)
        return std::nullopt;
    // 编码唯一性
    const 稳定编码 编码组[] = {映射.任务节点, 映射.虚拟存在节点,
        映射.任务族归属关系, 映射.任务需求列表项引用关系,
        映射.任务虚拟存在归属关系};
    for (std::size_t 左 = 0; 左 < 5; ++左)
        for (std::size_t 右 = 0; 右 < 左; ++右)
            if (编码组[左] == 编码组[右]) return std::nullopt;
    return 映射;
}

// 写入提交读回结果
struct 任务写入提交读回结果 final {
    L2结构状态 状态 = L2结构状态::内部不一致;
    std::uint64_t 事实截止代次 = 0;
    任务写入编码映射 编码映射;
};

// ===== 路径写入辅助 =====

// 路径写入编码映射查找结果
struct 路径写入编码映射 final {
    稳定编码 路径节点;
    稳定编码 路径族归属关系;
    稳定编码 路径任务引用关系;
    稳定编码 路径筹办轮次值;
    稳定编码 路径来源方法值;
    稳定编码 路径来源方法内容版本值;
    稳定编码 路径条件绑定值;
    稳定编码 路径输入绑定值;
    稳定编码 路径参数绑定值;
    稳定编码 路径动作入口值;
    稳定编码 路径预期结果值;
    稳定编码 路径验证合同值;
    稳定编码 路径归因合同值;
};

// 诊断责任：无适用错误分支；按本地键从写入结果提取稳定编码。
std::optional<路径写入编码映射> 查找路径写入编码映射(
    const L1所有者范围写入结果& 写入) noexcept {
    if (写入.新编码映射.size() != 13) return std::nullopt;
    路径写入编码映射 映射;
    bool 找到路径节点 = false, 找到族归属 = false, 找到任务引用 = false;
    bool 找到筹办轮次 = false, 找到来源方法 = false, 找到内容版本 = false;
    bool 找到条件绑定 = false, 找到输入绑定 = false, 找到参数绑定 = false;
    bool 找到动作入口 = false, 找到预期结果 = false, 找到验证合同 = false;
    bool 找到归因合同 = false;
    for (const auto& [本地键, 编码] : 写入.新编码映射) {
        if (!有效(编码)) return std::nullopt;
        if (本地键.值 == 路径节点写入本地键值) {
            映射.路径节点 = 编码; 找到路径节点 = true;
        } else if (本地键.值 == 路径族归属关系写入本地键值) {
            映射.路径族归属关系 = 编码; 找到族归属 = true;
        } else if (本地键.值 == 路径任务引用关系写入本地键值) {
            映射.路径任务引用关系 = 编码; 找到任务引用 = true;
        } else if (本地键.值 == 路径筹办轮次值写入本地键值) {
            映射.路径筹办轮次值 = 编码; 找到筹办轮次 = true;
        } else if (本地键.值 == 路径来源方法值写入本地键值) {
            映射.路径来源方法值 = 编码; 找到来源方法 = true;
        } else if (本地键.值 == 路径来源方法内容版本值写入本地键值) {
            映射.路径来源方法内容版本值 = 编码; 找到内容版本 = true;
        } else if (本地键.值 == 路径条件绑定值写入本地键值) {
            映射.路径条件绑定值 = 编码; 找到条件绑定 = true;
        } else if (本地键.值 == 路径输入绑定值写入本地键值) {
            映射.路径输入绑定值 = 编码; 找到输入绑定 = true;
        } else if (本地键.值 == 路径参数绑定值写入本地键值) {
            映射.路径参数绑定值 = 编码; 找到参数绑定 = true;
        } else if (本地键.值 == 路径动作入口值写入本地键值) {
            映射.路径动作入口值 = 编码; 找到动作入口 = true;
        } else if (本地键.值 == 路径预期结果值写入本地键值) {
            映射.路径预期结果值 = 编码; 找到预期结果 = true;
        } else if (本地键.值 == 路径验证合同值写入本地键值) {
            映射.路径验证合同值 = 编码; 找到验证合同 = true;
        } else if (本地键.值 == 路径归因合同值写入本地键值) {
            映射.路径归因合同值 = 编码; 找到归因合同 = true;
        } else return std::nullopt;
    }
    if (!找到路径节点 || !找到族归属 || !找到任务引用
        || !找到筹办轮次 || !找到来源方法 || !找到内容版本
        || !找到条件绑定 || !找到输入绑定 || !找到参数绑定
        || !找到动作入口 || !找到预期结果 || !找到验证合同
        || !找到归因合同)
        return std::nullopt;
    // 编码唯一性
    const 稳定编码 编码组[] = {映射.路径节点, 映射.路径族归属关系,
        映射.路径任务引用关系, 映射.路径筹办轮次值,
        映射.路径来源方法值, 映射.路径来源方法内容版本值,
        映射.路径条件绑定值, 映射.路径输入绑定值,
        映射.路径参数绑定值, 映射.路径动作入口值,
        映射.路径预期结果值, 映射.路径验证合同值,
        映射.路径归因合同值};
    for (std::size_t 左 = 0; 左 < 13; ++左)
        for (std::size_t 右 = 0; 右 < 左; ++右)
            if (编码组[左] == 编码组[右]) return std::nullopt;
    return 映射;
}

// 路径写入提交读回结果
struct 路径写入提交读回结果 final {
    L2结构状态 状态 = L2结构状态::内部不一致;
    std::uint64_t 事实截止代次 = 0;
    路径写入编码映射 编码映射;
};

// 诊断责任：向上送出；提交路径写集并验证首次完整或精确重复。
路径写入提交读回结果 提交并验证路径写集(
    L1所有者范围写端口& 写入端口,
    const L1所有者范围写集请求& 写集) {
    const auto 写入 = 写入端口.提交所有者范围中性写集(写集);
    路径写入提交读回结果 结果;
    结果.状态 = 映射任务写入状态(写入.状态);
    结果.事实截止代次 = 写入.事实代次;

    if (写入.合同版本 != L1所有者范围CRUD合同版本
        || 写入.所有者 != 写入端口.所有者身份()
        || 写入.写入幂等身份 != 写集.写入幂等身份) {
        结果.状态 = L2结构状态::内部不一致;
        return 结果;
    }
    if (结果.状态 != L2结构状态::已提交
        && 结果.状态 != L2结构状态::精确重复) {
        if (!写入.新编码映射.empty() || 写入.是否形成内存权威发布) {
            结果.状态 = L2结构状态::内部不一致;
            return 结果;
        }
        return 结果;
    }
    const bool 首次完整 = 写入.状态 == L1所有者范围写入状态::成功
        && 写入.是否形成内存权威发布
        && 写入.重试边界 == L1所有者范围重试边界::不适用;
    const bool 重复完整 = 写入.状态 == L1所有者范围写入状态::精确重复
        && !写入.是否形成内存权威发布
        && 写入.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛;
    if (写入.事实代次 == 0 || (!首次完整 && !重复完整)) {
        结果.状态 = L2结构状态::内部不一致;
        return 结果;
    }
    const auto 映射 = 查找路径写入编码映射(写入);
    if (!映射) {
        结果.状态 = L2结构状态::内部不一致;
        return 结果;
    }
    结果.编码映射 = *映射;
    return 结果;
}

// 诊断责任：无适用错误分支；64位纯值映射不读取事实。
L1所有者范围写入幂等身份 映射路径幂等键(
    L2结构幂等身份 身份) noexcept {
    return {身份.值};
}

// 读回新增任务方法路径结果
L2新增任务方法路径结果 读回新增任务方法路径(
    const L1事实基座服务& 第一层服务,
    const 任务身份来源定位& 来源定位,
    const 任务结构类型定位& 类型定位,
    const L2新增任务方法路径请求& 请求,
    const 路径写入编码映射& 编码映射,
    std::uint64_t 写入代次) {
    const auto 失败 = [&](L2结构状态 状态) noexcept {
        L2新增任务方法路径结果 结果;
        结果.结果头.合同版本 = L2结构合同版本;
        结果.结果头.状态 = 状态;
        结果.结果头.事实截止代次 = 写入代次;
        结果.结果头.变更事实代次 = 写入代次;
        return 结果;
    };

    // 读回路径节点
    const auto 节点读取 = 第一层服务.读取所有者范围历史事实(
        {L1所有者范围CRUD合同版本, 编码映射.路径节点});
    const auto* 路径节点 = 节点读取.事实
        ? std::get_if<L1所有者范围节点事实>(&*节点读取.事实) : nullptr;
    if (节点读取.状态 != L1所有者范围读取状态::成功
        || !路径节点
        || 路径节点->编码 != 编码映射.路径节点
        || 路径节点->写入所有者 != 来源定位.所有者
        || 路径节点->种类 != 节点种类::普通
        || 路径节点->属性类型表示
        || 路径节点->退出事实代次
        || 路径节点->创建事实代次 != 写入代次)
        return 失败(L2结构状态::内部不一致);

    // 读回所有关系
    const auto 读回关系 = [&](稳定编码 编码)
        -> std::optional<L1所有者范围关系事实> {
        const auto 读取 = 第一层服务.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 编码});
        const auto* 关系 = 读取.事实
            ? std::get_if<L1所有者范围关系事实>(&*读取.事实)
            : nullptr;
        if (读取.状态 != L1所有者范围读取状态::成功 || !关系
            || 关系->编码 != 编码
            || 关系->写入所有者 != 来源定位.所有者
            || 关系->退出事实代次
            || 关系->创建事实代次 != 写入代次)
            return std::nullopt;
        return *关系;
    };

    const auto 族归属 = 读回关系(编码映射.路径族归属关系);
    const auto 任务引用 = 读回关系(编码映射.路径任务引用关系);
    if (!族归属 || !任务引用)
        return 失败(L2结构状态::内部不一致);

    // 验证族归属关系：路径 -> 路径锚点
    if (族归属->源节点 != 编码映射.路径节点
        || 族归属->目标节点 != 来源定位.路径族归属关系
        || 族归属->关系类型节点 != 类型定位.路径族归属关系类型
        || 族归属->角色或顺序 != 1)
        return 失败(L2结构状态::内部不一致);

    // 验证任务引用关系：路径 -> 任务
    if (任务引用->源节点 != 编码映射.路径节点
        || 任务引用->目标节点 != 请求.任务.值
        || 任务引用->关系类型节点 != 类型定位.路径任务引用关系类型
        || 任务引用->角色或顺序 != 1)
        return 失败(L2结构状态::内部不一致);

    // 构造 L2任务方法路径身份
    L2任务方法路径身份 路径身份{编码映射.路径节点};

    // 读回 10 个值（筹办轮次、来源方法、来源方法内容版本、
    // 条件绑定、输入绑定、参数绑定、动作入口、预期结果、验证合同、归因合同）
    const auto 读回值 = [&](稳定编码 编码,
        稳定编码 属性类型) -> std::optional<L1所有者范围值事实> {
        const auto 读取 = 第一层服务.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 编码});
        const auto* 值 = 读取.事实
            ? std::get_if<L1所有者范围值事实>(&*读取.事实) : nullptr;
        if (读取.状态 != L1所有者范围读取状态::成功 || !值
            || 值->编码 != 编码
            || 值->所属节点 != 编码映射.路径节点
            || 值->属性类型节点 != 属性类型
            || 值->写入所有者 != 来源定位.所有者
            || 值->退出事实代次
            || 值->创建事实代次 != 写入代次)
            return std::nullopt;
        return *值;
    };

    const auto 筹办轮次值 = 读回值(编码映射.路径筹办轮次值,
        来源定位.路径筹办轮次属性类型);
    const auto 来源方法值 = 读回值(编码映射.路径来源方法值,
        来源定位.路径来源方法属性类型);
    const auto 内容版本值 = 读回值(编码映射.路径来源方法内容版本值,
        来源定位.路径来源方法内容版本属性类型);
    const auto 条件绑定值 = 读回值(编码映射.路径条件绑定值,
        来源定位.路径条件绑定属性类型);
    const auto 输入绑定值 = 读回值(编码映射.路径输入绑定值,
        来源定位.路径输入绑定属性类型);
    const auto 参数绑定值 = 读回值(编码映射.路径参数绑定值,
        来源定位.路径参数绑定属性类型);
    const auto 动作入口值 = 读回值(编码映射.路径动作入口值,
        来源定位.路径动作入口属性类型);
    const auto 预期结果值 = 读回值(编码映射.路径预期结果值,
        来源定位.路径预期结果属性类型);
    const auto 验证合同值 = 读回值(编码映射.路径验证合同值,
        来源定位.路径验证合同属性类型);
    const auto 归因合同值 = 读回值(编码映射.路径归因合同值,
        来源定位.路径归因合同属性类型);
    if (!筹办轮次值 || !来源方法值 || !内容版本值
        || !条件绑定值 || !输入绑定值 || !参数绑定值
        || !动作入口值 || !预期结果值 || !验证合同值
        || !归因合同值)
        return 失败(L2结构状态::内部不一致);

    // 验证节点属性槽已设置（10 个）
    if (路径节点->当前属性.size() != 10)
        return 失败(L2结构状态::内部不一致);
    bool 找到轮次槽 = false, 找到方法槽 = false, 找到版本槽 = false;
    bool 找到条件槽 = false, 找到输入槽 = false, 找到参数槽 = false;
    bool 找到动作槽 = false, 找到预期槽 = false, 找到验证槽 = false;
    bool 找到归因槽 = false;
    for (const auto& 槽 : 路径节点->当前属性) {
        if (槽.属性类型节点 == 来源定位.路径筹办轮次属性类型
            && 槽.当前值 == 编码映射.路径筹办轮次值)
            找到轮次槽 = true;
        else if (槽.属性类型节点 == 来源定位.路径来源方法属性类型
            && 槽.当前值 == 编码映射.路径来源方法值)
            找到方法槽 = true;
        else if (槽.属性类型节点 == 来源定位.路径来源方法内容版本属性类型
            && 槽.当前值 == 编码映射.路径来源方法内容版本值)
            找到版本槽 = true;
        else if (槽.属性类型节点 == 来源定位.路径条件绑定属性类型
            && 槽.当前值 == 编码映射.路径条件绑定值)
            找到条件槽 = true;
        else if (槽.属性类型节点 == 来源定位.路径输入绑定属性类型
            && 槽.当前值 == 编码映射.路径输入绑定值)
            找到输入槽 = true;
        else if (槽.属性类型节点 == 来源定位.路径参数绑定属性类型
            && 槽.当前值 == 编码映射.路径参数绑定值)
            找到参数槽 = true;
        else if (槽.属性类型节点 == 来源定位.路径动作入口属性类型
            && 槽.当前值 == 编码映射.路径动作入口值)
            找到动作槽 = true;
        else if (槽.属性类型节点 == 来源定位.路径预期结果属性类型
            && 槽.当前值 == 编码映射.路径预期结果值)
            找到预期槽 = true;
        else if (槽.属性类型节点 == 来源定位.路径验证合同属性类型
            && 槽.当前值 == 编码映射.路径验证合同值)
            找到验证槽 = true;
        else if (槽.属性类型节点 == 来源定位.路径归因合同属性类型
            && 槽.当前值 == 编码映射.路径归因合同值)
            找到归因槽 = true;
    }
    if (!找到轮次槽 || !找到方法槽 || !找到版本槽
        || !找到条件槽 || !找到输入槽 || !找到参数槽
        || !找到动作槽 || !找到预期槽 || !找到验证槽
        || !找到归因槽)
        return 失败(L2结构状态::内部不一致);

    // 提取 int64 材料（筹办轮次、来源方法、来源方法内容版本）
    const auto 取I64 = [](
        const std::optional<L1所有者范围值事实>& 值)
        -> std::optional<std::int64_t> {
        if (!值) return std::nullopt;
        if (auto* p = std::get_if<std::int64_t>(&值->材料))
            return *p;
        return std::nullopt;
    };
    const auto 轮次 = 取I64(筹办轮次值);
    const auto 方法 = 取I64(来源方法值);
    const auto 版本 = 取I64(内容版本值);
    if (!轮次 || !方法 || !版本)
        return 失败(L2结构状态::内部不一致);

    // 提取独立材料引用（条件绑定、输入绑定、参数绑定、动作入口、
    // 预期结果、验证合同、归因合同）
    const auto 取引用 = [](
        const std::optional<L1所有者范围值事实>& 值)
        -> std::optional<稳定编码> {
        if (!值) return std::nullopt;
        if (auto* p = std::get_if<L1所有者范围独立材料引用>(
            &值->材料))
            return p->编码;
        return std::nullopt;
    };
    const auto 条件绑定 = 取引用(条件绑定值);
    const auto 输入绑定 = 取引用(输入绑定值);
    const auto 参数绑定 = 取引用(参数绑定值);
    const auto 动作入口 = 取引用(动作入口值);
    const auto 预期结果 = 取引用(预期结果值);
    const auto 验证合同 = 取引用(验证合同值);
    const auto 归因合同 = 取引用(归因合同值);
    if (!条件绑定 || !输入绑定 || !参数绑定
        || !动作入口 || !预期结果 || !验证合同 || !归因合同)
        return 失败(L2结构状态::内部不一致);

    // 构造 L2任务方法路径事实（从 L1 事实构造，不从请求构造）
    L2任务方法路径事实 路径事实;
    路径事实.身份 = 路径身份;
    路径事实.任务 = 请求.任务;
    路径事实.筹办轮次 = static_cast<std::uint64_t>(*轮次);
    路径事实.来源方法 = L2方法身份{稳定编码{
        static_cast<std::uint64_t>(*方法)}};
    路径事实.来源方法内容版本 = 稳定编码{
        static_cast<std::uint64_t>(*版本)};
    路径事实.完整路径材料.条件绑定 = *条件绑定;
    路径事实.完整路径材料.输入绑定 = *输入绑定;
    路径事实.完整路径材料.参数绑定 = *参数绑定;
    路径事实.完整路径材料.动作入口 = *动作入口;
    路径事实.完整路径材料.预期结果 = *预期结果;
    路径事实.完整路径材料.验证合同 = *验证合同;
    路径事实.完整路径材料.归因合同 = *归因合同;
    路径事实.生命周期 = {写入代次, std::nullopt};

    L2新增任务方法路径结果 结果;
    结果.结果头.合同版本 = L2结构合同版本;
    结果.结果头.状态 = L2结构状态::已提交;
    结果.结果头.事实截止代次 = 写入代次;
    结果.结果头.变更事实代次 = 写入代次;
    结果.路径 = std::move(路径事实);
    return 结果;
}

// 读取任务方法路径当前投影：验证路径身份、任务归属、族锚点、
// 10 个属性值（筹办轮次/来源方法/内容版本/条件绑定/输入绑定/
// 参数绑定/动作入口/预期结果/验证合同/归因合同）、2 条源关系组、生命周期。
L2任务方法路径读取结果 读取任务方法路径当前投影(
    const L1事实基座服务& 第一层服务,
    const 任务身份来源定位& 来源定位,
    const 任务结构类型定位& 类型定位,
    const L2任务方法路径读取请求& 请求,
    std::uint64_t 期望代次,
    const 任务正式选择登记定位* 正式选择定位 = nullptr) {
    const auto 失败 = [&](L2结构状态 状态) noexcept {
        L2任务方法路径读取结果 结果;
        结果.结果头.合同版本 = L2结构合同版本;
        结果.结果头.状态 = 状态;
        结果.结果头.事实截止代次 = 期望代次;
        结果.读取类别 = 请求.读取类别;
        结果.历史截止事实代次 = 请求.历史截止事实代次;
        return 结果;
    };

    L1所有者范围一致当前读取请求 L1请求;
    L1请求.合同版本 = L1所有者范围一致当前读取合同版本;
    L1请求.期望事实代次 = 期望代次;
    L1请求.所有者 = {来源定位.所有者};
    L1请求.节点 = {请求.路径.值, 来源定位.路径族归属关系,
        类型定位.路径族归属关系类型};
    L1请求.源关系组 = {
        {请求.路径.值, 类型定位.路径族归属关系类型},
        {请求.路径.值, 类型定位.路径任务引用关系类型}};
    L1请求.属性值 = {
        {请求.路径.值, 来源定位.路径筹办轮次属性类型},
        {请求.路径.值, 来源定位.路径来源方法属性类型},
        {请求.路径.值, 来源定位.路径来源方法内容版本属性类型},
        {请求.路径.值, 来源定位.路径条件绑定属性类型},
        {请求.路径.值, 来源定位.路径输入绑定属性类型},
        {请求.路径.值, 来源定位.路径参数绑定属性类型},
        {请求.路径.值, 来源定位.路径动作入口属性类型},
        {请求.路径.值, 来源定位.路径预期结果属性类型},
        {请求.路径.值, 来源定位.路径验证合同属性类型},
        {请求.路径.值, 来源定位.路径归因合同属性类型}};
    if (正式选择定位)
        L1请求.目标关系组 = {{请求.路径.值,
            正式选择定位->选择记录路径引用关系类型}};
    const auto 读取 = 第一层服务.尝试读取所有者范围一致当前投影(L1请求);

    if (读取.状态 != L1所有者范围一致当前读取状态::成功
        || 读取.合同版本 != L1所有者范围一致当前读取合同版本
        || 读取.期望事实代次 != 期望代次
        || 读取.读取事实代次 != 期望代次)
        return 失败(L2结构状态::内部不一致);

    // 形状：1 所有者、3 节点、0 显式关系/值、10 属性值、2 源关系组、0 目标组
    if (读取.所有者.size() != 1 || 读取.节点.size() != 3
        || !读取.关系.empty() || !读取.值.empty()
        || 读取.属性值.size() != 10
        || 读取.源关系组.size() != 2
        || 读取.目标关系组.size() != (正式选择定位 ? 1U : 0U))
        return 失败(L2结构状态::内部不一致);

    // 所有者
    const auto& 所有者项 = 读取.所有者[0];
    if (所有者项.状态 != L1所有者范围一致当前读取项目状态::成功
        || !所有者项.所有者事实
        || 所有者项.所有者事实->所有者 != 来源定位.所有者)
        return 失败(L2结构状态::内部不一致);

    // 查找路径节点
    const auto 找节点 = [&](稳定编码 编码)
        -> const L1所有者范围一致节点读取结果项* {
        for (const auto& 项 : 读取.节点)
            if (项.查询编码 == 编码) return &项;
        return nullptr;
    };
    const auto* 路径节点项 = 找节点(请求.路径.值);
    if (!路径节点项)
        return 失败(L2结构状态::未找到);
    if (路径节点项->状态 != L1所有者范围一致当前读取项目状态::成功)
        return 失败(映射任务身份来源当前项目状态(路径节点项->状态));
    if (!路径节点项->事实)
        return 失败(L2结构状态::内部不一致);
    const auto& 路径节点 = *路径节点项->事实;
    if (路径节点.编码 != 请求.路径.值
        || 路径节点.写入所有者 != 来源定位.所有者
        || 路径节点.种类 != 节点种类::普通
        || 路径节点.退出事实代次)
        return 失败(L2结构状态::入口拒绝);

    // 族锚点和族类型节点
    const auto* 族锚点项 = 找节点(来源定位.路径族归属关系);
    const auto* 族类型项 = 找节点(类型定位.路径族归属关系类型);
    if (!族锚点项 || !族类型项
        || 族锚点项->状态 != L1所有者范围一致当前读取项目状态::成功
        || 族类型项->状态 != L1所有者范围一致当前读取项目状态::成功
        || !族锚点项->事实 || !族类型项->事实
        || 族锚点项->事实->编码 != 来源定位.路径族归属关系
        || 族类型项->事实->编码 != 类型定位.路径族归属关系类型)
        return 失败(L2结构状态::内部不一致);

    // 查找源关系组
    const auto 找源组 = [&](稳定编码 源, 稳定编码 类型)
        -> const L1所有者范围一致源关系组读取结果项* {
        for (const auto& 组 : 读取.源关系组)
            if (组.源节点 == 源 && 组.关系类型节点 == 类型)
                return &组;
        return nullptr;
    };

    // 族归属组：恰有 1 条当前成员
    const auto* 族归属组 = 找源组(请求.路径.值,
        类型定位.路径族归属关系类型);
    if (!族归属组 || 族归属组->成员.size() != 1)
        return 失败(L2结构状态::内部不一致);
    const auto& 族成员 = 族归属组->成员[0];
    if (族成员.关系.源节点 != 请求.路径.值
        || 族成员.关系.目标节点 != 来源定位.路径族归属关系
        || 族成员.关系.关系类型节点 != 类型定位.路径族归属关系类型
        || 族成员.关系.角色或顺序 != 1
        || 族成员.关系.退出事实代次)
        return 失败(L2结构状态::内部不一致);

    // 任务引用组：恰有 1 条当前成员
    const auto* 任务引用组 = 找源组(请求.路径.值,
        类型定位.路径任务引用关系类型);
    if (!任务引用组 || 任务引用组->成员.size() != 1)
        return 失败(L2结构状态::内部不一致);
    const auto& 引用成员 = 任务引用组->成员[0];
    if (引用成员.关系.源节点 != 请求.路径.值
        || 引用成员.关系.关系类型节点 != 类型定位.路径任务引用关系类型
        || 引用成员.关系.角色或顺序 != 1
        || 引用成员.关系.退出事实代次)
        return 失败(L2结构状态::内部不一致);

    // 查找属性值投影
    const auto 找属性值 = [&](稳定编码 属性类型)
        -> const L1所有者范围一致属性值读取结果项* {
        for (const auto& 项 : 读取.属性值)
            if (项.节点 == 请求.路径.值 && 项.属性类型 == 属性类型)
                return &项;
        return nullptr;
    };
    const auto* 轮次项 = 找属性值(来源定位.路径筹办轮次属性类型);
    const auto* 方法项 = 找属性值(来源定位.路径来源方法属性类型);
    const auto* 版本项 = 找属性值(
        来源定位.路径来源方法内容版本属性类型);
    const auto* 条件项 = 找属性值(来源定位.路径条件绑定属性类型);
    const auto* 输入项 = 找属性值(来源定位.路径输入绑定属性类型);
    const auto* 参数项 = 找属性值(来源定位.路径参数绑定属性类型);
    const auto* 动作项 = 找属性值(来源定位.路径动作入口属性类型);
    const auto* 预期项 = 找属性值(来源定位.路径预期结果属性类型);
    const auto* 验证项 = 找属性值(来源定位.路径验证合同属性类型);
    const auto* 归因项 = 找属性值(来源定位.路径归因合同属性类型);
    if (!轮次项 || !方法项 || !版本项 || !条件项 || !输入项
        || !参数项 || !动作项 || !预期项 || !验证项 || !归因项)
        return 失败(L2结构状态::内部不一致);
    const auto 已设置 = [](const auto& 项) noexcept {
        return 项.状态 == L1所有者范围一致当前读取项目状态::成功
            && 项.投影.has_value();
    };
    const auto 未设置 = [](const auto& 项) noexcept {
        return 项.状态 == L1所有者范围一致当前读取项目状态::属性未设置
            && !项.投影;
    };
    if (!已设置(*轮次项) || !已设置(*方法项) || !已设置(*版本项)
        || !已设置(*动作项) || !已设置(*预期项) || !已设置(*验证项))
        return 失败(L2结构状态::内部不一致);
    const bool 旧四值完整 = 已设置(*条件项) && 已设置(*输入项)
        && 已设置(*参数项) && 已设置(*归因项);
    const bool 新四值全空 = 未设置(*条件项) && 未设置(*输入项)
        && 未设置(*参数项) && 未设置(*归因项);
    if (旧四值完整 == 新四值全空)
        return 失败(L2结构状态::内部不一致);

    // 验证属性值事实
    const auto 验证值 = [](const L1所有者范围一致属性值投影& 投影,
        稳定编码 路径编码, 稳定编码 属性类型,
        const L1结构所有者身份& 所有者) -> bool {
        const auto& 值 = 投影.当前值事实;
        return 值.所属节点 == 路径编码
            && 值.属性类型节点 == 属性类型
            && 值.写入所有者 == 所有者
            && !值.退出事实代次;
    };
    if (!验证值(*轮次项->投影, 请求.路径.值,
        来源定位.路径筹办轮次属性类型, 来源定位.所有者)
        || !验证值(*方法项->投影, 请求.路径.值,
        来源定位.路径来源方法属性类型, 来源定位.所有者)
        || !验证值(*版本项->投影, 请求.路径.值,
        来源定位.路径来源方法内容版本属性类型, 来源定位.所有者)
        || !验证值(*动作项->投影, 请求.路径.值,
        来源定位.路径动作入口属性类型, 来源定位.所有者)
        || !验证值(*预期项->投影, 请求.路径.值,
        来源定位.路径预期结果属性类型, 来源定位.所有者)
        || !验证值(*验证项->投影, 请求.路径.值,
        来源定位.路径验证合同属性类型, 来源定位.所有者))
        return 失败(L2结构状态::内部不一致);
    if (旧四值完整
        && (!验证值(*条件项->投影, 请求.路径.值,
                来源定位.路径条件绑定属性类型, 来源定位.所有者)
            || !验证值(*输入项->投影, 请求.路径.值,
                来源定位.路径输入绑定属性类型, 来源定位.所有者)
            || !验证值(*参数项->投影, 请求.路径.值,
                来源定位.路径参数绑定属性类型, 来源定位.所有者)
            || !验证值(*归因项->投影, 请求.路径.值,
                来源定位.路径归因合同属性类型, 来源定位.所有者)))
        return 失败(L2结构状态::内部不一致);

    // 提取 I64 材料（筹办轮次、来源方法、来源方法内容版本）
    const auto 取I64 = [](const L1所有者范围一致属性值投影& 投影)
        -> std::optional<std::int64_t> {
        if (auto* p = std::get_if<std::int64_t>(&投影.当前值事实.材料))
            return *p;
        return std::nullopt;
    };
    const auto 轮次 = 取I64(*轮次项->投影);
    const auto 方法 = 取I64(*方法项->投影);
    const auto 版本 = 取I64(*版本项->投影);
    if (!轮次 || !方法 || !版本)
        return 失败(L2结构状态::内部不一致);

    // 提取独立材料引用（条件绑定、输入绑定、参数绑定、动作入口、
    // 预期结果、验证合同、归因合同）
    const auto 取引用 = [](const L1所有者范围一致属性值投影& 投影)
        -> std::optional<稳定编码> {
        if (auto* p = std::get_if<L1所有者范围独立材料引用>(
            &投影.当前值事实.材料))
            return p->编码;
        return std::nullopt;
    };
    const auto 动作入口 = 取引用(*动作项->投影);
    const auto 预期结果 = 取引用(*预期项->投影);
    const auto 验证合同 = 取引用(*验证项->投影);
    if (!动作入口 || !预期结果 || !验证合同)
        return 失败(L2结构状态::内部不一致);
    std::optional<稳定编码> 条件绑定;
    std::optional<稳定编码> 输入绑定;
    std::optional<稳定编码> 参数绑定;
    std::optional<稳定编码> 归因合同;
    if (旧四值完整) {
        条件绑定 = 取引用(*条件项->投影);
        输入绑定 = 取引用(*输入项->投影);
        参数绑定 = 取引用(*参数项->投影);
        归因合同 = 取引用(*归因项->投影);
        if (!条件绑定 || !输入绑定 || !参数绑定 || !归因合同)
            return 失败(L2结构状态::内部不一致);
        if (正式选择定位 && !读取.目标关系组.front().成员.empty())
            return 失败(L2结构状态::内部不一致);
    } else {
        if (!正式选择定位 || 读取.目标关系组.front().成员.size() != 1)
            return 失败(L2结构状态::内部不一致);
        const auto& 反向路径关系 =
            读取.目标关系组.front().成员.front();
        const auto 选择记录 = 反向路径关系.关系.源节点;
        if (反向路径关系.关系.目标节点 != 请求.路径.值
            || 反向路径关系.关系.关系类型节点
                != 正式选择定位->选择记录路径引用关系类型
            || 反向路径关系.关系.角色或顺序 != 1
            || 反向路径关系.关系.退出事实代次
            || 反向路径关系.对端节点.编码 != 选择记录
            || 反向路径关系.对端节点.写入所有者 != 来源定位.所有者
            || 反向路径关系.对端节点.种类 != 节点种类::普通
            || 反向路径关系.对端节点.退出事实代次
            || 反向路径关系.对端节点.创建事实代次
                != 路径节点.创建事实代次)
            return 失败(L2结构状态::内部不一致);
        L1所有者范围一致当前读取请求 选择读取请求;
        选择读取请求.合同版本 = L1所有者范围一致当前读取合同版本;
        选择读取请求.期望事实代次 = 期望代次;
        选择读取请求.所有者 = {来源定位.所有者};
        选择读取请求.源关系组 = {
            {选择记录, 正式选择定位->选择记录路径引用关系类型},
            {选择记录, 正式选择定位->选择记录冻结材料关系类型}};
        const auto 选择读取 = 第一层服务.尝试读取所有者范围一致当前投影(
            选择读取请求);
        if (选择读取.状态 != L1所有者范围一致当前读取状态::成功
            || 选择读取.读取事实代次 != 期望代次
            || 选择读取.所有者.size() != 1
            || 选择读取.源关系组.size() != 2)
            return 失败(L2结构状态::内部不一致);
        const L1所有者范围一致源关系组读取结果项* 路径组 = nullptr;
        const L1所有者范围一致源关系组读取结果项* 冻结组 = nullptr;
        for (const auto& 组 : 选择读取.源关系组) {
            if (组.关系类型节点
                == 正式选择定位->选择记录路径引用关系类型)
                路径组 = &组;
            else if (组.关系类型节点
                == 正式选择定位->选择记录冻结材料关系类型)
                冻结组 = &组;
        }
        if (!路径组 || !冻结组 || 路径组->成员.size() != 1
            || 冻结组->成员.size() != 1
            || 路径组->成员.front().关系.目标节点 != 请求.路径.值
            || 路径组->成员.front().关系.源节点 != 选择记录
            || 路径组->成员.front().关系.角色或顺序 != 1
            || 冻结组->成员.front().关系.源节点 != 选择记录
            || 冻结组->成员.front().关系.角色或顺序 != 1
            || 冻结组->成员.front().关系.退出事实代次
            || 冻结组->成员.front().对端节点.写入所有者
                != 来源定位.所有者
            || 冻结组->成员.front().对端节点.种类 != 节点种类::普通
            || 冻结组->成员.front().对端节点.退出事实代次
            || 冻结组->成员.front().对端节点.创建事实代次
                != 路径节点.创建事实代次)
            return 失败(L2结构状态::内部不一致);
        条件绑定 = 冻结组->成员.front().关系.目标节点;
        输入绑定 = *条件绑定;
        参数绑定 = *条件绑定;
        归因合同 = 选择记录;
    }

    // 构造路径事实
    L2任务方法路径事实 路径事实;
    路径事实.身份 = 请求.路径;
    路径事实.任务 = L2任务身份{引用成员.关系.目标节点};
    路径事实.筹办轮次 = static_cast<std::uint64_t>(*轮次);
    路径事实.来源方法 = L2方法身份{稳定编码{
        static_cast<std::uint64_t>(*方法)}};
    路径事实.来源方法内容版本 = 稳定编码{
        static_cast<std::uint64_t>(*版本)};
    路径事实.完整路径材料.条件绑定 = *条件绑定;
    路径事实.完整路径材料.输入绑定 = *输入绑定;
    路径事实.完整路径材料.参数绑定 = *参数绑定;
    路径事实.完整路径材料.动作入口 = *动作入口;
    路径事实.完整路径材料.预期结果 = *预期结果;
    路径事实.完整路径材料.验证合同 = *验证合同;
    路径事实.完整路径材料.归因合同 = *归因合同;
    路径事实.生命周期 = {路径节点.创建事实代次,
        路径节点.退出事实代次};

    L2任务方法路径读取结果 结果;
    结果.结果头.合同版本 = L2结构合同版本;
    结果.结果头.状态 = L2结构状态::已读取;
    结果.结果头.事实截止代次 = 期望代次;
    结果.读取类别 = 请求.读取类别;
    结果.历史截止事实代次 = 请求.历史截止事实代次;
    结果.路径 = std::move(路径事实);
    return 结果;
}

// 读取当前选中路径当前投影：0 合法空 / 1 当前路径 / >1 内部不一致。
L2读取当前选中路径结果 读取当前选中路径当前投影(
    const L1事实基座服务& 第一层服务,
    const 任务身份来源定位& 来源定位,
    const 任务结构类型定位& 类型定位,
    const L2读取当前选中路径请求& 请求,
    std::uint64_t 期望代次) {
    const auto 失败 = [&](L2结构状态 状态) noexcept {
        L2读取当前选中路径结果 结果;
        结果.结果头.合同版本 = L2结构合同版本;
        结果.结果头.状态 = 状态;
        结果.结果头.事实截止代次 = 期望代次;
        结果.读取类别 = 请求.读取类别;
        结果.历史截止事实代次 = 请求.历史截止事实代次;
        return 结果;
    };

    L1所有者范围一致当前读取请求 L1请求;
    L1请求.合同版本 = L1所有者范围一致当前读取合同版本;
    L1请求.期望事实代次 = 期望代次;
    L1请求.所有者 = {来源定位.所有者};
    L1请求.节点 = {请求.任务.值, 来源定位.任务锚点,
        来源定位.任务族归属关系类型};
    L1请求.源关系组 = {
        {请求.任务.值, 来源定位.任务族归属关系类型},
        {请求.任务.值, 类型定位.当前选中路径关系类型}};
    const auto 读取 = 第一层服务.尝试读取所有者范围一致当前投影(L1请求);

    if (读取.状态 != L1所有者范围一致当前读取状态::成功
        || 读取.合同版本 != L1所有者范围一致当前读取合同版本
        || 读取.期望事实代次 != 期望代次
        || 读取.读取事实代次 != 期望代次)
        return 失败(L2结构状态::内部不一致);

    // 形状：1 所有者、3 节点、0 显式关系/值、0 属性值、2 源关系组、0 目标组
    if (读取.所有者.size() != 1 || 读取.节点.size() != 3
        || !读取.关系.empty() || !读取.值.empty()
        || !读取.属性值.empty()
        || 读取.源关系组.size() != 2
        || !读取.目标关系组.empty())
        return 失败(L2结构状态::内部不一致);

    // 验证任务节点存在且当前
    const auto 找节点 = [&](稳定编码 编码)
        -> const L1所有者范围一致节点读取结果项* {
        for (const auto& 项 : 读取.节点)
            if (项.查询编码 == 编码) return &项;
        return nullptr;
    };
    const auto* 任务节点项 = 找节点(请求.任务.值);
    if (!任务节点项)
        return 失败(L2结构状态::未找到);
    if (任务节点项->状态 != L1所有者范围一致当前读取项目状态::成功)
        return 失败(映射任务身份来源当前项目状态(任务节点项->状态));
    if (!任务节点项->事实
        || 任务节点项->事实->编码 != 请求.任务.值
        || 任务节点项->事实->写入所有者 != 来源定位.所有者
        || 任务节点项->事实->种类 != 节点种类::普通
        || 任务节点项->事实->退出事实代次)
        return 失败(L2结构状态::入口拒绝);

    // 查找当前选中路径关系组
    const auto* 当前路径组 = [&]()
        -> const L1所有者范围一致源关系组读取结果项* {
        for (const auto& 组 : 读取.源关系组)
            if (组.源节点 == 请求.任务.值
                && 组.关系类型节点 == 类型定位.当前选中路径关系类型)
                return &组;
        return nullptr;
    }();
    if (!当前路径组)
        return 失败(L2结构状态::内部不一致);

    // 筛选当前（未退出）成员
    std::vector<const L1所有者范围一致关系对端投影*> 当前成员;
    for (const auto& 成员 : 当前路径组->成员)
        if (!成员.关系.退出事实代次)
            当前成员.push_back(&成员);

    // 0 当前成员：合法空结果
    if (当前成员.empty()) {
        L2读取当前选中路径结果 结果;
        结果.结果头.合同版本 = L2结构合同版本;
        结果.结果头.状态 = L2结构状态::已读取;
        结果.结果头.事实截止代次 = 期望代次;
        结果.读取类别 = 请求.读取类别;
        结果.历史截止事实代次 = 请求.历史截止事实代次;
        return 结果;  // optional 留空
    }

    // >1 当前成员：内部不一致
    if (当前成员.size() > 1)
        return 失败(L2结构状态::内部不一致);

    // 恰有 1 条当前选中路径关系
    const auto& 成员 = *当前成员[0];
    if (成员.关系.源节点 != 请求.任务.值
        || 成员.关系.关系类型节点 != 类型定位.当前选中路径关系类型)
        return 失败(L2结构状态::内部不一致);

    L2读取当前选中路径结果 结果;
    结果.结果头.合同版本 = L2结构合同版本;
    结果.结果头.状态 = L2结构状态::已读取;
    结果.结果头.事实截止代次 = 期望代次;
    结果.读取类别 = 请求.读取类别;
    结果.历史截止事实代次 = 请求.历史截止事实代次;
    结果.当前选中路径 = L2任务方法路径身份{成员.关系.目标节点};
    return 结果;
}

// 结果节点是不可变追加事实：该投影仅从 L1 当前一致视图还原，不从请求回填。
std::optional<L2任务实际结果事实> 读取任务实际结果当前投影(
    const L1事实基座服务& 第一层服务,
    const 任务身份来源定位& 来源定位,
    const 任务结构类型定位& 类型定位,
    L2任务实际结果身份 结果身份, std::uint64_t 期望代次) {
    L1所有者范围一致当前读取请求 请求;
    请求.合同版本 = L1所有者范围一致当前读取合同版本;
    请求.期望事实代次 = 期望代次;
    请求.所有者 = {来源定位.所有者};
    请求.节点 = {结果身份.值};
    请求.源关系组 = {
        {结果身份.值, 类型定位.结果族归属关系类型},
        {结果身份.值, 类型定位.结果实例方法关系类型},
        {结果身份.值, 类型定位.结果实际状态关系类型},
        {结果身份.值, 类型定位.结果动态证据关系类型},
        {结果身份.值, 类型定位.结果方法关系类型},
        {结果身份.值, 类型定位.结果动作入口关系类型},
        {结果身份.值, 类型定位.结果场景关系类型},
        {结果身份.值, 类型定位.结果主体关系类型}};
    请求.目标关系组 = {{结果身份.值, 类型定位.任务实际结果槽关系类型}};
    请求.属性值 = {{结果身份.值, 来源定位.结果来源材料版本属性类型},
        {结果身份.值, 来源定位.结果运行代次属性类型}};
    const auto 读取 = 第一层服务.尝试读取所有者范围一致当前投影(请求);
    if (读取.状态 != L1所有者范围一致当前读取状态::成功
        || 读取.读取事实代次 != 期望代次 || 读取.节点.size() != 1
        || 读取.源关系组.size() != 8 || 读取.目标关系组.size() != 1
        || 读取.属性值.size() != 2) return std::nullopt;
    const auto& 槽 = 读取.目标关系组.front();
    if (槽.成员.size() != 1 || 槽.成员.front().关系.目标节点 != 结果身份.值
        || 槽.成员.front().关系.角色或顺序 <= 0) return std::nullopt;
    const auto 取目标 = [&](稳定编码 类型) -> std::optional<稳定编码> {
        for (const auto& 组 : 读取.源关系组) if (组.关系类型节点 == 类型) {
            if (组.成员.size() != 1 || 组.成员.front().关系.退出事实代次) return std::nullopt;
            return 组.成员.front().关系.目标节点;
        }
        return std::nullopt;
    };
    const auto 族归属 = 取目标(类型定位.结果族归属关系类型);
    const auto 实例 = 取目标(类型定位.结果实例方法关系类型);
    const auto 状态 = 取目标(类型定位.结果实际状态关系类型);
    const auto 动态 = 取目标(类型定位.结果动态证据关系类型);
    const auto 方法 = 取目标(类型定位.结果方法关系类型);
    const auto 动作 = 取目标(类型定位.结果动作入口关系类型);
    const auto 场景 = 取目标(类型定位.结果场景关系类型);
    const auto 主体 = 取目标(类型定位.结果主体关系类型);
    const auto 取单U64 = [&](稳定编码 属性) -> std::optional<std::uint64_t> {
        for (const auto& 项 : 读取.属性值) if (项.属性类型 == 属性 && 项.投影) {
            const auto* 值组 = std::get_if<std::vector<std::uint64_t>>(
                &项.投影->当前值事实.材料);
            if (!值组 || 值组->size() != 1 || 值组->front() == 0)
                return std::nullopt;
            return 值组->front();
        }
        return std::nullopt;
    };
    const auto 版本 = 取单U64(来源定位.结果来源材料版本属性类型);
    const auto 运行 = 取单U64(来源定位.结果运行代次属性类型);
    if (!族归属 || *族归属 != 来源定位.结果族归属关系
        || !实例 || !状态 || !动态 || !方法 || !动作 || !场景
        || !主体 || !版本 || !运行) return std::nullopt;
    L2任务实际结果事实 事实;
    事实.身份 = 结果身份;
    事实.任务 = L2任务身份{槽.成员.front().关系.源节点};
    事实.实例方法 = L2实例方法身份{*实例};
    事实.执行轮次 = static_cast<std::uint64_t>(槽.成员.front().关系.角色或顺序);
    事实.方法 = L2方法身份{*方法}; 事实.动作入口 = L2方法动作入口身份{*动作};
    事实.场景 = L2场景身份{*场景}; 事实.主体 = L2存在身份{*主体};
    事实.实际状态 = L2状态身份{*状态}; 事实.动态证据 = L2动态身份{*动态};
    事实.来源材料版本 = *版本; 事实.运行代次 = *运行;
    const auto& 节点 = 读取.节点.front();
    if (!节点.事实 || 节点.事实->退出事实代次) return std::nullopt;
    事实.生命周期 = {节点.事实->创建事实代次, std::nullopt};
    return L2任务实际结果事实完整(事实) ? std::optional<L2任务实际结果事实>{事实} : std::nullopt;
}

// 不可变结果的历史投影以调用方截止读取节点、全部关系和值；观察代次只用于
// 证明各笔历史读取来自同一当前仓库观察，不把当前代次写回历史事实。
std::optional<L2任务实际结果事实> 读取任务实际结果历史投影(
    const L1事实基座服务& 第一层服务,
    const 任务身份来源定位& 来源定位,
    const 任务结构类型定位& 类型定位,
    L2任务实际结果身份 结果身份, std::uint64_t 截止,
    std::uint64_t 观察代次) {
    const auto 节点读取 = 第一层服务.读取所有者范围历史事实(
        {L1所有者范围CRUD合同版本, 结果身份.值});
    const auto* 节点 = 节点读取.事实
        ? std::get_if<L1所有者范围节点事实>(&*节点读取.事实) : nullptr;
    if (节点读取.状态 != L1所有者范围读取状态::成功 || !节点
        || 节点->编码 != 结果身份.值 || 节点->写入所有者 != 来源定位.所有者
        || 节点->种类 != 节点种类::普通 || 节点->属性类型表示
        || 节点->创建事实代次 == 0 || 节点->创建事实代次 > 截止
        || (节点->退出事实代次 && *节点->退出事实代次 <= 截止))
        return std::nullopt;
    const auto 读唯一源关系 = [&](稳定编码 类型) -> std::optional<稳定编码> {
        const auto 读取 = 第一层服务.读取所有者范围历史关系组(
            {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源,
                结果身份.值, 类型, 截止});
        if (读取.状态 != L1所有者范围读取状态::成功
            || 读取.历史截止事实代次 != 截止
            || 读取.读取事实代次 != 观察代次 || 读取.关系组.size() != 1)
            return std::nullopt;
        const auto& 关系 = 读取.关系组.front();
        if (关系.写入所有者 != 来源定位.所有者
            || 关系.源节点 != 结果身份.值 || 关系.关系类型节点 != 类型
            || 关系.角色或顺序 != 1 || 关系.创建事实代次 != 节点->创建事实代次
            || (关系.退出事实代次 && *关系.退出事实代次 <= 截止))
            return std::nullopt;
        return 关系.目标节点;
    };
    const auto 族归属 = 读唯一源关系(类型定位.结果族归属关系类型);
    const auto 实例 = 读唯一源关系(类型定位.结果实例方法关系类型);
    const auto 状态 = 读唯一源关系(类型定位.结果实际状态关系类型);
    const auto 动态 = 读唯一源关系(类型定位.结果动态证据关系类型);
    const auto 方法 = 读唯一源关系(类型定位.结果方法关系类型);
    const auto 动作 = 读唯一源关系(类型定位.结果动作入口关系类型);
    const auto 场景 = 读唯一源关系(类型定位.结果场景关系类型);
    const auto 主体 = 读唯一源关系(类型定位.结果主体关系类型);
    const auto 槽读取 = 第一层服务.读取所有者范围历史关系组(
        {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::目标,
            结果身份.值, 类型定位.任务实际结果槽关系类型, 截止});
    if (!族归属 || *族归属 != 来源定位.结果族归属关系 || !实例 || !状态
        || !动态 || !方法 || !动作 || !场景 || !主体
        || 槽读取.状态 != L1所有者范围读取状态::成功
        || 槽读取.历史截止事实代次 != 截止
        || 槽读取.读取事实代次 != 观察代次 || 槽读取.关系组.size() != 1)
        return std::nullopt;
    const auto& 槽 = 槽读取.关系组.front();
    if (槽.写入所有者 != 来源定位.所有者 || 槽.目标节点 != 结果身份.值
        || 槽.关系类型节点 != 类型定位.任务实际结果槽关系类型
        || 槽.角色或顺序 <= 0 || 槽.创建事实代次 != 节点->创建事实代次
        || (槽.退出事实代次 && *槽.退出事实代次 <= 截止))
        return std::nullopt;
    const auto 值读取 = 第一层服务.读取所有者范围历史属性值组(
        {L1所有者范围CRUD合同版本, 结果身份.值, 截止});
    if (值读取.状态 != L1所有者范围读取状态::成功
        || 值读取.历史截止事实代次 != 截止
        || 值读取.读取事实代次 != 观察代次 || 值读取.属性值组.size() != 2)
        return std::nullopt;
    const auto 取单U64 = [&](稳定编码 属性) -> std::optional<std::uint64_t> {
        const L1所有者范围值事实* 找到 = nullptr;
        for (const auto& 值 : 值读取.属性值组) {
            if (值.属性类型节点 != 属性) continue;
            if (找到) return std::nullopt;
            找到 = std::addressof(值);
        }
        if (!找到 || 找到->写入所有者 != 来源定位.所有者
            || 找到->所属节点 != 结果身份.值 || 找到->来源节点 != 结果身份.值)
            return std::nullopt;
        const auto* 材料 = std::get_if<std::vector<std::uint64_t>>(&找到->材料);
        return 材料 && 材料->size() == 1 && 材料->front() != 0
            ? std::optional<std::uint64_t>{材料->front()} : std::nullopt;
    };
    const auto 版本 = 取单U64(来源定位.结果来源材料版本属性类型);
    const auto 运行 = 取单U64(来源定位.结果运行代次属性类型);
    if (!版本 || !运行) return std::nullopt;
    L2任务实际结果事实 事实;
    事实.身份 = 结果身份;
    事实.任务 = L2任务身份{槽.源节点};
    事实.实例方法 = L2实例方法身份{*实例};
    事实.执行轮次 = static_cast<std::uint64_t>(槽.角色或顺序);
    事实.方法 = L2方法身份{*方法};
    事实.动作入口 = L2方法动作入口身份{*动作};
    事实.场景 = L2场景身份{*场景};
    事实.主体 = L2存在身份{*主体};
    事实.实际状态 = L2状态身份{*状态};
    事实.动态证据 = L2动态身份{*动态};
    事实.来源材料版本 = *版本;
    事实.运行代次 = *运行;
    事实.生命周期 = {节点->创建事实代次, 节点->退出事实代次};
    return L2任务实际结果事实完整(事实)
        ? std::optional<L2任务实际结果事实>{事实} : std::nullopt;
}

std::vector<std::uint64_t> 编码任务目标裁决材料(
    const L2任务目标裁决结构材料& 材料) {
    return {1, 材料.治理输入幂等键, 材料.任务.值.值,
        材料.实例方法.值.值, 材料.执行轮次,
        材料.任务实际结果.值.值, 材料.任务目标来源.值.值,
        材料.目标宿主.值.值, 材料.目标特征.值.值,
        材料.目标状态合同.值.值, 材料.实际状态.值.值,
        材料.动态证据.值.值, 材料.方法.值.值,
        材料.路径预期结果.值, 材料.路径验证合同.值,
        材料.比较注册.值.值, 材料.比较算法版本,
        static_cast<std::uint8_t>(材料.具名关系), 材料.允许关系位,
        static_cast<std::uint8_t>(材料.目标达成状态),
        static_cast<std::uint8_t>(材料.后继准备),
        材料.来源共同事实截止};
}

std::optional<L2任务目标裁决结构材料> 解码任务目标裁决材料(
    const std::vector<std::uint64_t>& 值) noexcept {
    if (值.size() != 22 || 值[0] != 1
        || 值[16] > (std::numeric_limits<std::uint32_t>::max)()
        || 值[17] > (std::numeric_limits<std::uint8_t>::max)()
        || 值[18] > (std::numeric_limits<std::uint8_t>::max)()
        || 值[19] > (std::numeric_limits<std::uint8_t>::max)()
        || 值[20] > (std::numeric_limits<std::uint8_t>::max)())
        return std::nullopt;
    L2任务目标裁决结构材料 材料;
    材料.治理输入幂等键 = 值[1];
    材料.任务 = L2任务身份{稳定编码{值[2]}};
    材料.实例方法 = L2实例方法身份{稳定编码{值[3]}};
    材料.执行轮次 = 值[4];
    材料.任务实际结果 = L2任务实际结果身份{稳定编码{值[5]}};
    材料.任务目标来源 = L2需求列表项身份{稳定编码{值[6]}};
    材料.目标宿主 = L2存在身份{稳定编码{值[7]}};
    材料.目标特征 = L2特征定义身份{稳定编码{值[8]}};
    材料.目标状态合同 = L2目标状态合同身份{稳定编码{值[9]}};
    材料.实际状态 = L2状态身份{稳定编码{值[10]}};
    材料.动态证据 = L2动态身份{稳定编码{值[11]}};
    材料.方法 = L2方法身份{稳定编码{值[12]}};
    材料.路径预期结果 = 稳定编码{值[13]};
    材料.路径验证合同 = 稳定编码{值[14]};
    材料.比较注册 = L2特征比较注册身份{稳定编码{值[15]}};
    材料.比较算法版本 = static_cast<std::uint32_t>(值[16]);
    材料.具名关系 = static_cast<L2特征比较具名关系>(值[17]);
    材料.允许关系位 = static_cast<std::uint8_t>(值[18]);
    材料.目标达成状态 = static_cast<L2任务目标达成状态>(值[19]);
    材料.后继准备 = static_cast<L2任务目标裁决后继准备>(值[20]);
    材料.来源共同事实截止 = 值[21];
    return L2任务目标裁决结构材料完整(材料)
        ? std::optional<L2任务目标裁决结构材料>{材料} : std::nullopt;
}

std::vector<std::uint64_t> 编码任务治理状态材料(
    const L2任务治理状态事实& 事实) {
    return {1, 事实.生命周期版本,
        static_cast<std::uint8_t>(事实.业务状态段),
        static_cast<std::uint8_t>(事实.重新触发状态),
        static_cast<std::uint8_t>(事实.执行冻结当前性),
        static_cast<std::uint8_t>(事实.方法运行当前性),
        static_cast<std::uint8_t>(事实.执行结果类型),
        static_cast<std::uint8_t>(事实.目标达成状态),
        static_cast<std::uint8_t>(事实.本轮推进状态),
        static_cast<std::uint8_t>(事实.验证裁决状态),
        static_cast<std::uint8_t>(事实.结果当前性),
        事实.来源执行轮次, 事实.来源共同事实截止,
        事实.任务.值.值, 事实.任务虚拟存在.值.值,
        事实.来源方法路径.值.值, 事实.来源实例方法.值.值,
        事实.来源任务实际结果.值.值};
}

std::optional<L2任务治理状态事实> 解码任务治理状态材料(
    const std::vector<std::uint64_t>& 值) noexcept {
    if (值.size() != 18 || 值[0] != 1)
        return std::nullopt;
    for (std::size_t i = 2; i <= 10; ++i)
        if (值[i] > (std::numeric_limits<std::uint8_t>::max)())
            return std::nullopt;
    L2任务治理状态事实 事实;
    事实.生命周期版本 = 值[1];
    事实.业务状态段 = static_cast<L2任务业务状态段>(值[2]);
    事实.重新触发状态 = static_cast<L2任务重新触发状态>(值[3]);
    事实.执行冻结当前性 = static_cast<L2任务执行冻结当前性>(值[4]);
    事实.方法运行当前性 = static_cast<L2任务方法运行当前性>(值[5]);
    事实.执行结果类型 = static_cast<L2任务执行结果类型>(值[6]);
    事实.目标达成状态 = static_cast<L2任务目标达成状态>(值[7]);
    事实.本轮推进状态 = static_cast<L2任务本轮推进状态>(值[8]);
    事实.验证裁决状态 = static_cast<L2任务验证裁决状态>(值[9]);
    事实.结果当前性 = static_cast<L2任务结果当前性>(值[10]);
    事实.来源执行轮次 = 值[11];
    事实.来源共同事实截止 = 值[12];
    事实.任务 = L2任务身份{稳定编码{值[13]}};
    事实.任务虚拟存在 = L2任务虚拟存在身份{稳定编码{值[14]}};
    事实.来源方法路径 = L2任务方法路径身份{稳定编码{值[15]}};
    事实.来源实例方法 = L2实例方法身份{稳定编码{值[16]}};
    事实.来源任务实际结果 = L2任务实际结果身份{稳定编码{值[17]}};
    return 事实;
}

std::optional<L2任务目标裁决证据事实> 读取目标裁决证据历史投影(
    const L1事实基座服务& 第一层服务,
    const 任务身份来源定位& 来源定位,
    const 任务结构类型定位& 类型定位,
    L2任务目标裁决证据身份 身份, std::uint64_t 截止,
    std::uint64_t 观察代次) {
    const auto 节点读取 = 第一层服务.读取所有者范围历史事实(
        {L1所有者范围CRUD合同版本, 身份.值});
    const auto* 节点 = 节点读取.事实
        ? std::get_if<L1所有者范围节点事实>(&*节点读取.事实) : nullptr;
    if (节点读取.状态 != L1所有者范围读取状态::成功 || !节点
        || 节点读取.读取事实代次 != 观察代次 || 节点->编码 != 身份.值
        || 节点->写入所有者 != 来源定位.所有者
        || 节点->种类 != 节点种类::普通 || 节点->属性类型表示
        || 节点->创建事实代次 == 0 || 节点->创建事实代次 > 截止
        || (节点->退出事实代次 && *节点->退出事实代次 <= 截止))
        return std::nullopt;
    const auto 读唯一关系 = [&](稳定编码 类型, std::int64_t 角色 = 1)
        -> std::optional<稳定编码> {
        const auto 读取 = 第一层服务.读取所有者范围历史关系组(
            {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源,
                身份.值, 类型, 截止});
        if (读取.状态 != L1所有者范围读取状态::成功
            || 读取.读取事实代次 != 观察代次
            || 读取.历史截止事实代次 != 截止 || 读取.关系组.size() != 1)
            return std::nullopt;
        const auto& 关系 = 读取.关系组.front();
        if (关系.写入所有者 != 来源定位.所有者
            || 关系.源节点 != 身份.值 || 关系.关系类型节点 != 类型
            || 关系.角色或顺序 != 角色
            || 关系.创建事实代次 != 节点->创建事实代次
            || (关系.退出事实代次 && *关系.退出事实代次 <= 截止))
            return std::nullopt;
        return 关系.目标节点;
    };
    const auto 族 = 读唯一关系(类型定位.目标裁决证据族归属关系类型);
    const auto 任务 = 读唯一关系(类型定位.证据任务关系类型);
    const auto 实例 = 读唯一关系(类型定位.证据实例关系类型);
    const auto 结果 = 读唯一关系(类型定位.证据实际结果关系类型);
    const auto 来源 = 读唯一关系(类型定位.证据目标来源关系类型);
    const auto 宿主 = 读唯一关系(类型定位.证据目标宿主关系类型);
    const auto 特征 = 读唯一关系(类型定位.证据目标特征关系类型);
    const auto 合同 = 读唯一关系(类型定位.证据目标状态合同关系类型);
    const auto 状态 = 读唯一关系(类型定位.证据实际状态关系类型);
    const auto 动态 = 读唯一关系(类型定位.证据动态关系类型);
    const auto 方法 = 读唯一关系(类型定位.证据方法关系类型);
    const auto 比较 = 读唯一关系(类型定位.证据比较注册关系类型);
    if (!族 || *族 != 来源定位.目标裁决证据族归属关系锚点
        || !任务 || !实例 || !结果 || !来源 || !宿主 || !特征 || !合同
        || !状态 || !动态 || !方法 || !比较)
        return std::nullopt;
    const auto 值读取 = 第一层服务.读取所有者范围历史属性值组(
        {L1所有者范围CRUD合同版本, 身份.值, 截止});
    if (值读取.状态 != L1所有者范围读取状态::成功
        || 值读取.读取事实代次 != 观察代次
        || 值读取.历史截止事实代次 != 截止 || 值读取.属性值组.size() != 1)
        return std::nullopt;
    const auto& 值事实 = 值读取.属性值组.front();
    const auto* 向量 = std::get_if<std::vector<std::uint64_t>>(&值事实.材料);
    if (值事实.写入所有者 != 来源定位.所有者
        || 值事实.所属节点 != 身份.值
        || 值事实.属性类型节点 != 来源定位.目标裁决材料属性类型
        || 值事实.来源节点 != 身份.值 || !向量)
        return std::nullopt;
    const auto 材料 = 解码任务目标裁决材料(*向量);
    if (!材料 || 材料->任务.值 != *任务 || 材料->实例方法.值 != *实例
        || 材料->任务实际结果.值 != *结果
        || 材料->任务目标来源.值 != *来源 || 材料->目标宿主.值 != *宿主
        || 材料->目标特征.值 != *特征 || 材料->目标状态合同.值 != *合同
        || 材料->实际状态.值 != *状态 || 材料->动态证据.值 != *动态
        || 材料->方法.值 != *方法 || 材料->比较注册.值 != *比较)
        return std::nullopt;
    L2任务目标裁决证据事实 事实{身份, *材料,
        {节点->创建事实代次, 节点->退出事实代次}};
    return L2任务目标裁决证据事实完整(事实)
        ? std::optional<L2任务目标裁决证据事实>{事实} : std::nullopt;
}

struct 任务治理状态投影 final {
    L2任务治理状态事实 状态;
    L2任务目标裁决证据事实 证据;
};

std::optional<任务治理状态投影> 读取任务治理状态历史投影(
    const L1事实基座服务& 第一层服务,
    const 任务身份来源定位& 来源定位,
    const 任务结构类型定位& 类型定位,
    L2任务治理状态身份 身份, std::uint64_t 截止,
    std::uint64_t 观察代次) {
    const auto 节点读取 = 第一层服务.读取所有者范围历史事实(
        {L1所有者范围CRUD合同版本, 身份.值});
    const auto* 节点 = 节点读取.事实
        ? std::get_if<L1所有者范围节点事实>(&*节点读取.事实) : nullptr;
    if (节点读取.状态 != L1所有者范围读取状态::成功 || !节点
        || 节点读取.读取事实代次 != 观察代次 || 节点->编码 != 身份.值
        || 节点->写入所有者 != 来源定位.所有者
        || 节点->种类 != 节点种类::普通 || 节点->属性类型表示
        || 节点->创建事实代次 == 0 || 节点->创建事实代次 > 截止
        || (节点->退出事实代次 && *节点->退出事实代次 <= 截止))
        return std::nullopt;
    const auto 读组 = [&](稳定编码 类型) {
        return 第一层服务.读取所有者范围历史关系组(
            {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源,
                身份.值, 类型, 截止});
    };
    const auto 读唯一 = [&](稳定编码 类型) -> std::optional<稳定编码> {
        const auto 读取 = 读组(类型);
        if (读取.状态 != L1所有者范围读取状态::成功
            || 读取.读取事实代次 != 观察代次
            || 读取.历史截止事实代次 != 截止 || 读取.关系组.size() != 1)
            return std::nullopt;
        const auto& 关系 = 读取.关系组.front();
        if (关系.写入所有者 != 来源定位.所有者
            || 关系.源节点 != 身份.值 || 关系.关系类型节点 != 类型
            || 关系.角色或顺序 != 1
            || 关系.创建事实代次 != 节点->创建事实代次
            || (关系.退出事实代次 && *关系.退出事实代次 <= 截止))
            return std::nullopt;
        return 关系.目标节点;
    };
    const auto 族 = 读唯一(类型定位.治理状态族归属关系类型);
    const auto 虚拟 = 读唯一(类型定位.状态虚拟存在关系类型);
    const auto 场景 = 读唯一(类型定位.状态场景关系类型);
    const auto 路径 = 读唯一(类型定位.状态路径关系类型);
    const auto 实例 = 读唯一(类型定位.状态实例关系类型);
    const auto 结果 = 读唯一(类型定位.状态实际结果关系类型);
    const auto 证据身份 = 读唯一(类型定位.状态目标裁决证据关系类型);
    if (!族 || *族 != 来源定位.治理状态族归属关系锚点
        || !虚拟 || !场景 || !路径 || !实例 || !结果 || !证据身份)
        return std::nullopt;
    const auto 值读取 = 第一层服务.读取所有者范围历史属性值组(
        {L1所有者范围CRUD合同版本, 身份.值, 截止});
    if (值读取.状态 != L1所有者范围读取状态::成功
        || 值读取.读取事实代次 != 观察代次
        || 值读取.历史截止事实代次 != 截止 || 值读取.属性值组.size() != 2)
        return std::nullopt;
    std::optional<std::int64_t> 发生时间;
    std::optional<L2任务治理状态事实> 状态材料;
    for (const auto& 值事实 : 值读取.属性值组) {
        if (值事实.写入所有者 != 来源定位.所有者
            || 值事实.所属节点 != 身份.值 || 值事实.来源节点 != 身份.值)
            return std::nullopt;
        if (值事实.属性类型节点 == 来源定位.治理状态生命周期发生时间属性类型) {
            const auto* 值 = std::get_if<std::int64_t>(&值事实.材料);
            if (!值 || 发生时间) return std::nullopt;
            发生时间 = *值;
        } else if (值事实.属性类型节点 == 来源定位.治理状态材料属性类型) {
            const auto* 值 = std::get_if<std::vector<std::uint64_t>>(&值事实.材料);
            if (!值 || 状态材料) return std::nullopt;
            状态材料 = 解码任务治理状态材料(*值);
        } else return std::nullopt;
    }
    if (!发生时间 || !状态材料) return std::nullopt;
    auto 状态事实 = *状态材料;
    状态事实.身份 = 身份;
    状态事实.运行场景 = L2场景身份{*场景};
    状态事实.生命周期发生时间 = *发生时间;
    状态事实.目标裁决证据 = L2任务目标裁决证据身份{*证据身份};
    状态事实.生命周期 = {节点->创建事实代次, 节点->退出事实代次};
    if (状态事实.任务虚拟存在.值 != *虚拟
        || 状态事实.来源方法路径.值 != *路径
        || 状态事实.来源实例方法.值 != *实例
        || 状态事实.来源任务实际结果.值 != *结果)
        return std::nullopt;
    const auto 前驱组 = 读组(类型定位.状态前驱关系类型);
    if (前驱组.状态 != L1所有者范围读取状态::成功
        || 前驱组.读取事实代次 != 观察代次
        || 前驱组.历史截止事实代次 != 截止)
        return std::nullopt;
    if (前驱组.关系组.empty()) 状态事实.前一治理状态.reset();
    else if (前驱组.关系组.size() == 1
        && 前驱组.关系组.front().角色或顺序 == 1)
        状态事实.前一治理状态 = L2任务治理状态身份{
            前驱组.关系组.front().目标节点};
    else return std::nullopt;
    const auto 当前关系 = 第一层服务.读取所有者范围历史关系组(
        {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::目标,
            身份.值, 类型定位.任务当前治理状态关系类型, 截止});
    if (当前关系.状态 != L1所有者范围读取状态::成功
        || 当前关系.读取事实代次 != 观察代次
        || 当前关系.历史截止事实代次 != 截止
        || 当前关系.关系组.size() != 1)
        return std::nullopt;
    const auto& 当前 = 当前关系.关系组.front();
    if (当前.写入所有者 != 来源定位.所有者
        || 当前.目标节点 != 身份.值
        || 当前.关系类型节点 != 类型定位.任务当前治理状态关系类型
        || 当前.角色或顺序 <= 0
        || static_cast<std::uint64_t>(当前.角色或顺序) != 状态事实.生命周期版本
        || 当前.创建事实代次 != 节点->创建事实代次)
        return std::nullopt;
    状态事实.任务 = L2任务身份{当前.源节点};
    const auto 证据 = 读取目标裁决证据历史投影(第一层服务, 来源定位,
        类型定位, 状态事实.目标裁决证据, 截止, 观察代次);
    if (!证据 || 证据->材料.任务 != 状态事实.任务
        || 证据->材料.实例方法 != 状态事实.来源实例方法
        || 证据->材料.执行轮次 != 状态事实.来源执行轮次
        || 证据->材料.任务实际结果 != 状态事实.来源任务实际结果
        || 证据->材料.目标达成状态 != 状态事实.目标达成状态
        || 证据->材料.来源共同事实截止 != 状态事实.来源共同事实截止)
        return std::nullopt;
    const auto 迁移证据 = 读组(类型定位.治理状态迁移证据关系类型);
    if (迁移证据.状态 != L1所有者范围读取状态::成功
        || 迁移证据.读取事实代次 != 观察代次
        || 迁移证据.历史截止事实代次 != 截止
        || 迁移证据.关系组.size() != 5)
        return std::nullopt;
    std::optional<稳定编码> 角色[5];
    for (const auto& 关系 : 迁移证据.关系组) {
        if (关系.写入所有者 != 来源定位.所有者
            || 关系.源节点 != 身份.值
            || 关系.关系类型节点 != 类型定位.治理状态迁移证据关系类型
            || 关系.角色或顺序 < 40 || 关系.角色或顺序 > 44)
            return std::nullopt;
        auto& 槽 = 角色[static_cast<std::size_t>(关系.角色或顺序 - 40)];
        if (槽) return std::nullopt;
        槽 = 关系.目标节点;
    }
    if (!角色[0] || !角色[1] || !角色[2] || !角色[3] || !角色[4]
        || *角色[0] != 证据->材料.任务.值
        || *角色[1] != 证据->材料.方法.值
        || *角色[2] != 证据->材料.方法.值
        || *角色[3] != 证据->材料.实际状态.值
        || *角色[4] != 证据->材料.动态证据.值
        || !L2任务治理状态事实完整(状态事实))
        return std::nullopt;
    return 任务治理状态投影{状态事实, *证据};
}

L1所有者范围写集请求 形成执行结果待验证首态写集(
    const L2新增执行结果待验证状态请求& 请求,
    L2场景身份 场景,
    const 任务身份来源定位& 来源,
    const 任务结构类型定位& 类型) {
    const auto 键 = [](std::uint32_t 值) noexcept {
        return L1所有者范围写集本地键{值};
    };
    const auto 状态键 = 键(新治理状态节点写入本地键值);
    const auto 证据键 = 键(新目标裁决证据节点写入本地键值);
    L2任务治理状态事实 状态;
    状态.任务 = 请求.任务;
    状态.任务虚拟存在 = 请求.任务虚拟存在;
    状态.运行场景 = 场景;
    状态.生命周期发生时间 = 请求.生命周期发生时间;
    状态.生命周期版本 = 1;
    状态.业务状态段 = L2任务业务状态段::执行结果待验证;
    状态.重新触发状态 = L2任务重新触发状态::未触发;
    状态.执行冻结当前性 = L2任务执行冻结当前性::当前;
    状态.方法运行当前性 = L2任务方法运行当前性::已结束;
    状态.执行结果类型 = L2任务执行结果类型::产生可比较结果;
    状态.目标达成状态 = 请求.目标裁决.目标达成状态;
    状态.本轮推进状态 = L2任务本轮推进状态::未裁决;
    状态.验证裁决状态 = L2任务验证裁决状态::未验证;
    状态.结果当前性 = L2任务结果当前性::当前;
    状态.来源方法路径 = 请求.来源方法路径;
    状态.来源实例方法 = 请求.来源实例方法;
    状态.来源执行轮次 = 请求.目标裁决.执行轮次;
    状态.来源任务实际结果 = 请求.来源任务实际结果;
    状态.来源共同事实截止 = 请求.目标裁决.来源共同事实截止;

    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 请求.请求头.期望事实代次;
    写集.写入幂等身份 = {请求.幂等身份.值};
    写集.节点 = {{状态键, 节点种类::普通, std::nullopt},
        {证据键, 节点种类::普通, std::nullopt}};
    写集.关系 = {
        {键(状态族归属关系写入本地键值), 状态键,
            来源.治理状态族归属关系锚点, 类型.治理状态族归属关系类型, 1},
        {键(证据族归属关系写入本地键值), 证据键,
            来源.目标裁决证据族归属关系锚点,
            类型.目标裁决证据族归属关系类型, 1},
        {键(任务当前治理状态关系写入本地键值), 请求.任务.值,
            状态键, 类型.任务当前治理状态关系类型, 1},
        {键(状态虚拟存在关系写入本地键值), 状态键,
            请求.任务虚拟存在.值, 类型.状态虚拟存在关系类型, 1},
        {键(状态场景关系写入本地键值), 状态键, 场景.值,
            类型.状态场景关系类型, 1},
        {键(状态路径关系写入本地键值), 状态键,
            请求.来源方法路径.值, 类型.状态路径关系类型, 1},
        {键(状态实例关系写入本地键值), 状态键,
            请求.来源实例方法.值, 类型.状态实例关系类型, 1},
        {键(状态实际结果关系写入本地键值), 状态键,
            请求.来源任务实际结果.值, 类型.状态实际结果关系类型, 1},
        {键(状态目标裁决证据关系写入本地键值), 状态键, 证据键,
            类型.状态目标裁决证据关系类型, 1},
        {键(状态关系15任务写入本地键值), 状态键,
            请求.任务.值, 类型.治理状态迁移证据关系类型, 40},
        {键(状态关系15方法写入本地键值), 状态键,
            请求.目标裁决.方法.值, 类型.治理状态迁移证据关系类型, 41},
        {键(状态关系15动作写入本地键值), 状态键,
            请求.目标裁决.方法.值, 类型.治理状态迁移证据关系类型, 42},
        {键(状态关系15状态写入本地键值), 状态键,
            请求.目标裁决.实际状态.值,
            类型.治理状态迁移证据关系类型, 43},
        {键(状态关系15动态写入本地键值), 状态键,
            请求.目标裁决.动态证据.值,
            类型.治理状态迁移证据关系类型, 44},
        {键(证据任务关系写入本地键值), 证据键,
            请求.目标裁决.任务.值, 类型.证据任务关系类型, 1},
        {键(证据实例关系写入本地键值), 证据键,
            请求.目标裁决.实例方法.值, 类型.证据实例关系类型, 1},
        {键(证据实际结果关系写入本地键值), 证据键,
            请求.目标裁决.任务实际结果.值, 类型.证据实际结果关系类型, 1},
        {键(证据目标来源关系写入本地键值), 证据键,
            请求.目标裁决.任务目标来源.值, 类型.证据目标来源关系类型, 1},
        {键(证据目标宿主关系写入本地键值), 证据键,
            请求.目标裁决.目标宿主.值, 类型.证据目标宿主关系类型, 1},
        {键(证据目标特征关系写入本地键值), 证据键,
            请求.目标裁决.目标特征.值, 类型.证据目标特征关系类型, 1},
        {键(证据目标状态合同关系写入本地键值), 证据键,
            请求.目标裁决.目标状态合同.值,
            类型.证据目标状态合同关系类型, 1},
        {键(证据实际状态关系写入本地键值), 证据键,
            请求.目标裁决.实际状态.值, 类型.证据实际状态关系类型, 1},
        {键(证据动态关系写入本地键值), 证据键,
            请求.目标裁决.动态证据.值, 类型.证据动态关系类型, 1},
        {键(证据方法关系写入本地键值), 证据键,
            请求.目标裁决.方法.值, 类型.证据方法关系类型, 1},
        {键(证据比较注册关系写入本地键值), 证据键,
            请求.目标裁决.比较注册.值, 类型.证据比较注册关系类型, 1}};
    写集.值 = {
        {键(治理状态发生时间值写入本地键值), 状态键,
            来源.治理状态生命周期发生时间属性类型,
            请求.生命周期发生时间, 状态键},
        {键(治理状态材料值写入本地键值), 状态键,
            来源.治理状态材料属性类型, 编码任务治理状态材料(状态), 状态键},
        {键(目标裁决材料值写入本地键值), 证据键,
            来源.目标裁决材料属性类型,
            编码任务目标裁决材料(请求.目标裁决), 证据键}};
    写集.属性槽变更 = {
        {状态键, 来源.治理状态生命周期发生时间属性类型,
            键(治理状态发生时间值写入本地键值)},
        {状态键, 来源.治理状态材料属性类型,
            键(治理状态材料值写入本地键值)},
        {证据键, 来源.目标裁决材料属性类型,
            键(目标裁决材料值写入本地键值)}};
    return 写集;
}

L1所有者范围写集请求 形成目标未达成迁移写集(
    const L2提交目标未达成待重筹办迁移请求& 请求,
    const L2任务治理状态事实& 前态,
    const L2任务目标裁决证据事实& 证据,
    稳定编码 当前治理关系编码,
    稳定编码 当前路径关系编码,
    稳定编码 当前实例关系编码,
    const 任务身份来源定位& 来源,
    const 任务结构类型定位& 类型) {
    const auto 键 = [](std::uint32_t 值) noexcept {
        return L1所有者范围写集本地键{值};
    };
    const auto 状态键 = 键(新治理状态节点写入本地键值);
    L2任务治理状态事实 状态 = 前态;
    状态.身份 = L2任务治理状态身份{};
    状态.生命周期发生时间 = 请求.生命周期发生时间;
    ++状态.生命周期版本;
    状态.业务状态段 = L2任务业务状态段::目标未达成待重筹办;
    状态.重新触发状态 = L2任务重新触发状态::已触发;
    状态.执行冻结当前性 = L2任务执行冻结当前性::已失效;
    状态.目标达成状态 = L2任务目标达成状态::未达成;
    状态.前一治理状态 = 前态.身份;
    状态.生命周期 = {};

    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 请求.请求头.期望事实代次;
    写集.写入幂等身份 = {请求.幂等身份.值};
    写集.节点 = {{状态键, 节点种类::普通, std::nullopt}};
    写集.关系 = {
        {键(状态族归属关系写入本地键值), 状态键,
            来源.治理状态族归属关系锚点, 类型.治理状态族归属关系类型, 1},
        {键(任务当前治理状态关系写入本地键值), 请求.任务.值,
            状态键, 类型.任务当前治理状态关系类型,
            static_cast<std::int64_t>(状态.生命周期版本)},
        {键(状态虚拟存在关系写入本地键值), 状态键,
            请求.任务虚拟存在.值, 类型.状态虚拟存在关系类型, 1},
        {键(状态场景关系写入本地键值), 状态键, 前态.运行场景.值,
            类型.状态场景关系类型, 1},
        {键(状态前驱关系写入本地键值), 状态键,
            请求.前一治理状态.值, 类型.状态前驱关系类型, 1},
        {键(状态路径关系写入本地键值), 状态键,
            前态.来源方法路径.值, 类型.状态路径关系类型, 1},
        {键(状态实例关系写入本地键值), 状态键,
            前态.来源实例方法.值, 类型.状态实例关系类型, 1},
        {键(状态实际结果关系写入本地键值), 状态键,
            前态.来源任务实际结果.值, 类型.状态实际结果关系类型, 1},
        {键(状态目标裁决证据关系写入本地键值), 状态键,
            请求.目标裁决证据.值, 类型.状态目标裁决证据关系类型, 1},
        {键(状态关系15任务写入本地键值), 状态键,
            证据.材料.任务.值, 类型.治理状态迁移证据关系类型, 40},
        {键(状态关系15方法写入本地键值), 状态键,
            证据.材料.方法.值, 类型.治理状态迁移证据关系类型, 41},
        {键(状态关系15动作写入本地键值), 状态键,
            证据.材料.方法.值, 类型.治理状态迁移证据关系类型, 42},
        {键(状态关系15状态写入本地键值), 状态键,
            证据.材料.实际状态.值, 类型.治理状态迁移证据关系类型, 43},
        {键(状态关系15动态写入本地键值), 状态键,
            证据.材料.动态证据.值, 类型.治理状态迁移证据关系类型, 44}};
    写集.值 = {
        {键(治理状态发生时间值写入本地键值), 状态键,
            来源.治理状态生命周期发生时间属性类型,
            请求.生命周期发生时间, 状态键},
        {键(治理状态材料值写入本地键值), 状态键,
            来源.治理状态材料属性类型, 编码任务治理状态材料(状态), 状态键}};
    写集.属性槽变更 = {
        {状态键, 来源.治理状态生命周期发生时间属性类型,
            键(治理状态发生时间值写入本地键值)},
        {状态键, 来源.治理状态材料属性类型,
            键(治理状态材料值写入本地键值)}};
    写集.退出事实 = {
        当前治理关系编码,
        当前路径关系编码,
        当前实例关系编码};
    return 写集;
}

struct 任务筹办轮次定位 final {
    L1结构所有者身份 所有者;
    稳定编码 权威记录锚点;
    稳定编码 后继记录锚点;
    std::vector<稳定编码> 关系类型;
    std::vector<稳定编码> 属性类型;
    std::uint64_t 建立事实代次 = 0;
};

inline constexpr L1所有者范围写入幂等身份
    任务筹办轮次登记幂等身份{0x4C32'5255'4C45'4155ULL};
inline constexpr std::uint32_t 筹办轮次锚点起始键 = 0x0301'2700U;
inline constexpr std::uint32_t 筹办轮次关系类型起始键 = 0x0301'2710U;
inline constexpr std::uint32_t 筹办轮次属性类型起始键 = 0x0301'2730U;
inline constexpr std::uint32_t 后继准备记录节点写入键 = 0x0301'2750U;
inline constexpr std::uint32_t 筹办轮次关系写入起始键 = 0x0301'2760U;
inline constexpr std::uint32_t 筹办轮次值写入起始键 = 0x0301'2780U;
inline constexpr std::uint32_t 初次融合权威节点写入键 = 0x0301'2640U;
inline constexpr std::uint32_t 初次融合权威关系写入起始键 = 0x0301'2641U;

L1所有者范围写集请求 形成统一目标未达成迁移写集(
    const L2提交目标未达成待重筹办迁移请求& 请求,
    const L2任务治理状态事实& 前态,
    const L2任务目标裁决证据事实& 证据,
    稳定编码 当前治理关系编码,
    稳定编码 当前路径关系编码,
    稳定编码 当前实例关系编码,
    稳定编码 当前筹办准备关系编码,
    const L2任务当前筹办准备事实& 当前准备,
    const 任务身份来源定位& 来源,
    const 任务结构类型定位& 类型,
    const 任务筹办轮次定位& 轮次定位) {
    auto 写集 = 形成目标未达成迁移写集(请求, 前态, 证据,
        当前治理关系编码, 当前路径关系编码, 当前实例关系编码,
        来源, 类型);
    const auto& 推进 = *请求.统一轮次推进;
    const auto 键 = [](std::uint32_t 值) noexcept {
        return L1所有者范围写集本地键{值};
    };
    const auto 后继键 = 键(后继准备记录节点写入键);
    const auto 状态键 = 键(新治理状态节点写入本地键值);
    写集.节点.push_back({后继键, 节点种类::普通, std::nullopt});
    写集.关系.push_back({键(筹办轮次关系写入起始键 + 5), 后继键,
        轮次定位.后继记录锚点, 轮次定位.关系类型[5], 1});
    写集.关系.push_back({键(筹办轮次关系写入起始键 + 6), 后继键,
        请求.任务.值, 轮次定位.关系类型[6], 1});
    写集.关系.push_back({键(筹办轮次关系写入起始键 + 7), 后继键,
        请求.任务虚拟存在.值, 轮次定位.关系类型[7], 1});
    写集.关系.push_back({键(筹办轮次关系写入起始键 + 8), 后继键,
        当前准备.轮次权威.需求列表项.值, 轮次定位.关系类型[8], 1});
    写集.关系.push_back({键(筹办轮次关系写入起始键 + 9), 后继键,
        推进.轮次权威.值, 轮次定位.关系类型[9], 1});
    写集.关系.push_back({键(筹办轮次关系写入起始键 + 10), 后继键,
        推进.首次准备记录.值, 轮次定位.关系类型[10], 1});
    if (推进.期望前一后继准备记录)
        写集.关系.push_back({键(筹办轮次关系写入起始键 + 11), 后继键,
            推进.期望前一后继准备记录->值,
            轮次定位.关系类型[11], 1});
    写集.关系.push_back({键(筹办轮次关系写入起始键 + 12), 后继键,
        前态.来源任务实际结果.值, 轮次定位.关系类型[12], 1});
    写集.关系.push_back({键(筹办轮次关系写入起始键 + 13), 后继键,
        请求.旧当前路径.值, 轮次定位.关系类型[13], 1});
    写集.关系.push_back({键(筹办轮次关系写入起始键 + 14), 后继键,
        请求.旧当前实例.值, 轮次定位.关系类型[14], 1});
    写集.关系.push_back({键(筹办轮次关系写入起始键 + 15), 后继键,
        请求.目标裁决证据.值, 轮次定位.关系类型[15], 1});
    写集.关系.push_back({键(筹办轮次关系写入起始键 + 16), 后继键,
        状态键, 轮次定位.关系类型[16], 1});
    写集.关系.push_back({键(筹办轮次关系写入起始键 + 17),
        请求.任务.值, 后继键, 轮次定位.关系类型[17], 1});
    const auto 新轮次 = 推进.期望前一筹办轮次 + 1;
    const auto 发布代次 = 请求.请求头.期望事实代次 + 1;
    写集.值.push_back({键(筹办轮次值写入起始键 + 1), 后继键,
        轮次定位.属性类型[1],
        std::vector<std::uint64_t>{
            static_cast<std::uint64_t>(L2任务后继筹办触发类型::实际结果未达成)},
        后继键});
    写集.值.push_back({键(筹办轮次值写入起始键 + 2), 后继键,
        轮次定位.属性类型[2],
        std::vector<std::uint64_t>{推进.触发请求身份, 推进.运行代次,
            推进.触发业务幂等身份.值}, 后继键});
    写集.值.push_back({键(筹办轮次值写入起始键 + 3), 后继键,
        轮次定位.属性类型[3],
        std::vector<std::uint64_t>{推进.期望前一筹办轮次, 新轮次},
        后继键});
    写集.值.push_back({键(筹办轮次值写入起始键 + 4), 后继键,
        轮次定位.属性类型[4],
        std::vector<std::uint64_t>{推进.来源共同事实截止, 发布代次},
        后继键});
    写集.值.push_back({键(筹办轮次值写入起始键 + 5), 后继键,
        轮次定位.属性类型[5],
        std::vector<std::uint64_t>{推进.准备幂等身份.值}, 后继键});
    for (std::uint32_t 偏移 = 1; 偏移 < 6; ++偏移)
        写集.属性槽变更.push_back({后继键,
            轮次定位.属性类型[偏移],
            键(筹办轮次值写入起始键 + 偏移)});
    写集.退出事实.push_back(当前筹办准备关系编码);
    return 写集;
}

L1所有者范围写集请求 形成子目标回流后继筹办准备写集(
    const L2提交任务子目标回流后继筹办准备请求& 请求,
    稳定编码 当前筹办准备关系编码,
    const L2任务当前筹办准备事实& 当前准备,
    const 任务筹办轮次定位& 轮次定位) {
    const auto 键 = [](std::uint32_t 值) noexcept {
        return L1所有者范围写集本地键{值};
    };
    const auto 后继键 = 键(后继准备记录节点写入键);
    const auto& 推进 = 请求.统一轮次推进;
    const auto 新轮次 = 推进.期望前一筹办轮次 + 1;
    const auto 发布代次 = 请求.请求头.期望事实代次 + 1;

    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 请求.请求头.期望事实代次;
    写集.写入幂等身份 = {请求.幂等身份.值};
    写集.节点 = {{后继键, 节点种类::普通, std::nullopt}};
    写集.关系 = {
        {键(筹办轮次关系写入起始键 + 5), 后继键,
            轮次定位.后继记录锚点, 轮次定位.关系类型[5], 1},
        {键(筹办轮次关系写入起始键 + 6), 后继键,
            请求.任务.值, 轮次定位.关系类型[6], 1},
        {键(筹办轮次关系写入起始键 + 7), 后继键,
            当前准备.任务.任务虚拟存在.值, 轮次定位.关系类型[7], 1},
        {键(筹办轮次关系写入起始键 + 8), 后继键,
            当前准备.轮次权威.需求列表项.值,
            轮次定位.关系类型[8], 1},
        {键(筹办轮次关系写入起始键 + 9), 后继键,
            推进.轮次权威.值, 轮次定位.关系类型[9], 1},
        {键(筹办轮次关系写入起始键 + 10), 后继键,
            推进.首次准备记录.值, 轮次定位.关系类型[10], 1},
        {键(筹办轮次关系写入起始键 + 12), 后继键,
            请求.回流记录.值, 轮次定位.关系类型[12], 1},
        {键(筹办轮次关系写入起始键 + 17), 请求.任务.值,
            后继键, 轮次定位.关系类型[17], 1}};
    if (推进.期望前一后继准备记录) {
        写集.关系.push_back({键(筹办轮次关系写入起始键 + 11),
            后继键, 推进.期望前一后继准备记录->值,
            轮次定位.关系类型[11], 1});
    }
    写集.值 = {
        {键(筹办轮次值写入起始键 + 1), 后继键,
            轮次定位.属性类型[1],
            std::vector<std::uint64_t>{static_cast<std::uint64_t>(
                L2任务后继筹办触发类型::子目标回流)}, 后继键},
        {键(筹办轮次值写入起始键 + 2), 后继键,
            轮次定位.属性类型[2],
            std::vector<std::uint64_t>{推进.触发请求身份,
                推进.运行代次, 推进.触发业务幂等身份.值}, 后继键},
        {键(筹办轮次值写入起始键 + 3), 后继键,
            轮次定位.属性类型[3],
            std::vector<std::uint64_t>{推进.期望前一筹办轮次,
                新轮次}, 后继键},
        {键(筹办轮次值写入起始键 + 4), 后继键,
            轮次定位.属性类型[4],
            std::vector<std::uint64_t>{推进.来源共同事实截止,
                发布代次}, 后继键},
        {键(筹办轮次值写入起始键 + 5), 后继键,
            轮次定位.属性类型[5],
            std::vector<std::uint64_t>{推进.准备幂等身份.值}, 后继键}};
    for (std::uint32_t 偏移 = 1; 偏移 < 6; ++偏移) {
        写集.属性槽变更.push_back({后继键,
            轮次定位.属性类型[偏移],
            键(筹办轮次值写入起始键 + 偏移)});
    }
    写集.退出事实 = {当前筹办准备关系编码};
    return 写集;
}

bool 子目标回流后继与请求一致(
    const L2任务后继筹办准备记录事实& 记录,
    const L2提交任务子目标回流后继筹办准备请求& 请求) noexcept {
    const auto& 推进 = 请求.统一轮次推进;
    return 记录.触发类型 == L2任务后继筹办触发类型::子目标回流
        && 记录.触发事实 == 请求.回流记录.值
        && 记录.任务 == 请求.任务
        && 记录.轮次权威 == 推进.轮次权威
        && 记录.首次准备记录 == 推进.首次准备记录
        && 记录.前一后继准备记录 == 推进.期望前一后继准备记录
        && 记录.前一筹办轮次 == 推进.期望前一筹办轮次
        && 记录.新筹办轮次 == 推进.期望前一筹办轮次 + 1
        && 记录.触发请求身份 == 推进.触发请求身份
        && 记录.运行代次 == 推进.运行代次
        && 记录.触发业务幂等身份 == 推进.触发业务幂等身份
        && 记录.来源共同事实截止 == 推进.来源共同事实截止
        && 记录.准备幂等身份 == 请求.幂等身份
        && !记录.旧路径 && !记录.旧实例 && !记录.目标裁决证据
        && !记录.正式待重筹办状态;
}

std::optional<稳定编码> 查找治理写入编码(
    const L1所有者范围写入结果& 写入, std::uint32_t 本地键值) noexcept {
    std::optional<稳定编码> 找到;
    for (const auto& [本地键, 编码] : 写入.新编码映射) {
        if (本地键.值 != 本地键值) continue;
        if (找到 || !有效(编码)) return std::nullopt;
        找到 = 编码;
    }
    return 找到;
}

std::optional<L2实例方法事实> 读取实例方法当前投影(
    const L1事实基座服务& L1, const 任务身份来源定位& 来源,
    const 任务结构类型定位& 类型, L2实例方法身份 身份, std::uint64_t G0,
    const 任务正式选择登记定位* 正式选择定位 = nullptr) {
    L1所有者范围一致当前读取请求 请求;
    请求.合同版本 = L1所有者范围一致当前读取合同版本; 请求.期望事实代次 = G0;
    请求.所有者 = {来源.所有者}; 请求.节点 = {身份.值};
    请求.源关系组 = {{身份.值, 类型.实例方法任务引用关系类型}, {身份.值, 类型.实例方法路径引用关系类型}};
    请求.属性值 = {{身份.值, 来源.实例方法执行轮次属性类型}};
    const auto 读 = L1.尝试读取所有者范围一致当前投影(请求);
    if (读.状态 != L1所有者范围一致当前读取状态::成功 || 读.读取事实代次 != G0 || 读.节点.size()!=1 || 读.源关系组.size()!=2 || 读.属性值.size()!=1) return std::nullopt;
    const auto 取 = [&](稳定编码 类型)->std::optional<稳定编码>{ for(const auto& 组:读.源关系组) if(组.关系类型节点==类型 && 组.成员.size()==1 && !组.成员[0].关系.退出事实代次) return 组.成员[0].关系.目标节点; return std::nullopt;};
    const auto 任务=取(类型.实例方法任务引用关系类型), 路径=取(类型.实例方法路径引用关系类型);
    if(!任务||!路径||!读.属性值[0].投影) return std::nullopt;
    const auto* 轮=std::get_if<std::int64_t>(&读.属性值[0].投影->当前值事实.材料);
    if(!轮||*轮<=0||!读.节点[0].事实) return std::nullopt;
    L2任务方法路径读取请求 路径请求{{L2结构合同版本,G0},L2读取类别::当前,L2任务方法路径身份{*路径}};
    const auto 路径结果=读取任务方法路径当前投影(
        L1,来源,类型,路径请求,G0,正式选择定位);
    if(!路径结果.成功()||!路径结果.路径||路径结果.路径->任务!=L2任务身份{*任务}) return std::nullopt;
    return L2实例方法事实{身份,L2任务身份{*任务},L2任务方法路径身份{*路径},路径结果.路径->筹办轮次,static_cast<std::uint64_t>(*轮),路径结果.路径->来源方法,路径结果.路径->来源方法内容版本,{读.节点[0].事实->创建事实代次,std::nullopt}};
}

std::optional<L2实例方法事实> 读取实例方法_v2当前投影(
    const L1事实基座服务& L1, const 任务身份来源定位& 来源,
    const 任务结构类型定位& 类型, L2实例方法身份 身份, std::uint64_t G0,
    L2任务身份 预期任务, L2任务方法路径身份 预期路径,
    std::uint64_t 预期筹办轮次, L2方法身份 预期方法,
    L2方法内容版本 预期方法内容版本) {
    L1所有者范围一致当前读取请求 请求;
    请求.合同版本 = L1所有者范围一致当前读取合同版本;
    请求.期望事实代次 = G0;
    请求.所有者 = {来源.所有者};
    请求.节点 = {身份.值};
    请求.源关系组 = {{身份.值, 类型.实例方法任务引用关系类型},
        {身份.值, 类型.实例方法路径引用关系类型}};
    请求.属性值 = {{身份.值, 来源.实例方法执行轮次属性类型}};
    const auto 读 = L1.尝试读取所有者范围一致当前投影(请求);
    if (读.状态 != L1所有者范围一致当前读取状态::成功
        || 读.读取事实代次 != G0 || 读.节点.size() != 1
        || 读.源关系组.size() != 2 || 读.属性值.size() != 1
        || !读.节点.front().事实
        || 读.节点.front().状态
            != L1所有者范围一致当前读取项目状态::成功)
        return std::nullopt;
    const auto 取 = [&](稳定编码 关系类型) -> std::optional<稳定编码> {
        for (const auto& 组 : 读.源关系组)
            if (组.关系类型节点 == 关系类型 && 组.成员.size() == 1
                && !组.成员.front().关系.退出事实代次
                && 组.成员.front().关系.角色或顺序 == 1)
                return 组.成员.front().关系.目标节点;
        return std::nullopt;
    };
    const auto 任务 = 取(类型.实例方法任务引用关系类型);
    const auto 路径 = 取(类型.实例方法路径引用关系类型);
    if (!任务 || !路径 || L2任务身份{*任务} != 预期任务
        || L2任务方法路径身份{*路径} != 预期路径
        || !读.属性值.front().投影)
        return std::nullopt;
    const auto* 执行轮次 = std::get_if<std::int64_t>(
        &读.属性值.front().投影->当前值事实.材料);
    if (!执行轮次 || *执行轮次 <= 0) return std::nullopt;
    return L2实例方法事实{身份, 预期任务, 预期路径, 预期筹办轮次,
        static_cast<std::uint64_t>(*执行轮次), 预期方法,
        预期方法内容版本.值,
        {读.节点.front().事实->创建事实代次, std::nullopt}};
}

// 诊断责任：向上送出；提交写集并验证首次完整或精确重复。
任务写入提交读回结果 提交并验证任务写集(
    L1所有者范围写端口& 写入端口,
    const L1所有者范围写集请求& 写集) {
    const auto 写入 = 写入端口.提交所有者范围中性写集(写集);
    任务写入提交读回结果 结果;
    结果.状态 = 映射任务写入状态(写入.状态);
    结果.事实截止代次 = 写入.事实代次;

    if (写入.合同版本 != L1所有者范围CRUD合同版本
        || 写入.所有者 != 写入端口.所有者身份()
        || 写入.写入幂等身份 != 写集.写入幂等身份) {
        结果.状态 = L2结构状态::内部不一致;
        return 结果;
    }
    if (结果.状态 != L2结构状态::已提交
        && 结果.状态 != L2结构状态::精确重复) {
        if (!写入.新编码映射.empty() || 写入.是否形成内存权威发布) {
            结果.状态 = L2结构状态::内部不一致;
            return 结果;
        }
        return 结果;
    }
    const bool 首次完整 = 写入.状态 == L1所有者范围写入状态::成功
        && 写入.是否形成内存权威发布
        && 写入.重试边界 == L1所有者范围重试边界::不适用;
    const bool 重复完整 = 写入.状态 == L1所有者范围写入状态::精确重复
        && !写入.是否形成内存权威发布
        && 写入.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛;
    if (写入.事实代次 == 0 || (!首次完整 && !重复完整)) {
        结果.状态 = L2结构状态::内部不一致;
        return 结果;
    }
    const auto 映射 = 查找任务写入编码映射(写入);
    if (!映射) {
        结果.状态 = L2结构状态::内部不一致;
        return 结果;
    }
    结果.编码映射 = *映射;
    return 结果;
}

// 诊断责任：向上送出；提交退出写集并验证首次完整或精确重复。
任务写入提交读回结果 提交并验证任务退出写集(
    L1所有者范围写端口& 写入端口,
    const L1所有者范围写集请求& 写集) {
    const auto 写入 = 写入端口.提交所有者范围中性写集(写集);
    任务写入提交读回结果 结果;
    结果.状态 = 映射任务写入状态(写入.状态);
    结果.事实截止代次 = 写入.事实代次;

    if (写入.合同版本 != L1所有者范围CRUD合同版本
        || 写入.所有者 != 写入端口.所有者身份()
        || 写入.写入幂等身份 != 写集.写入幂等身份) {
        结果.状态 = L2结构状态::内部不一致;
        return 结果;
    }
    if (结果.状态 != L2结构状态::已提交
        && 结果.状态 != L2结构状态::精确重复) {
        if (!写入.新编码映射.empty() || 写入.是否形成内存权威发布) {
            结果.状态 = L2结构状态::内部不一致;
            return 结果;
        }
        return 结果;
    }
    const bool 首次完整 = 写入.状态 == L1所有者范围写入状态::成功
        && 写入.是否形成内存权威发布
        && 写入.重试边界 == L1所有者范围重试边界::不适用;
    const bool 重复完整 = 写入.状态 == L1所有者范围写入状态::精确重复
        && !写入.是否形成内存权威发布
        && 写入.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛;
    if (写入.事实代次 == 0 || (!首次完整 && !重复完整)
        || !写入.新编码映射.empty()) {
        结果.状态 = L2结构状态::内部不一致;
        return 结果;
    }
    return 结果;
}

// 诊断责任：向上送出；读回所有新建事实并构造完整写入结果。
L2新增任务结果 读回新增任务(
    const L1事实基座服务& 第一层服务,
    const 任务身份来源定位& 来源定位,
    const 任务结构类型定位& 类型定位,
    const L2新增任务请求& 请求,
    const 任务写入编码映射& 编码映射,
    L2结构状态 写入状态,
    std::uint64_t 写入代次) {
    const auto 失败 = [&](L2结构状态 状态) noexcept {
        L2新增任务结果 结果;
        结果.结果头.合同版本 = L2结构合同版本;
        结果.结果头.状态 = 状态;
        结果.结果头.事实截止代次 = 写入代次;
        结果.结果头.变更事实代次 = 写入代次;
        return 结果;
    };
    if (写入状态 != L2结构状态::已提交
        && 写入状态 != L2结构状态::精确重复)
        return 失败(L2结构状态::内部不一致);
    const bool 精确重复 = 写入状态 == L2结构状态::精确重复;
    const auto 创建与退出形状有效 = [&](std::uint64_t 创建代次,
        const std::optional<std::uint64_t>& 退出代次) noexcept {
        return 创建代次 == 写入代次 && (精确重复 || !退出代次);
    };

    const auto 任务身份 = L2任务身份{编码映射.任务节点};
    const auto 虚拟存在身份 = L2任务虚拟存在身份{编码映射.虚拟存在节点};

    // 读回任务节点
    const auto 任务节点读取 = 第一层服务.读取所有者范围历史事实(
        {L1所有者范围CRUD合同版本, 编码映射.任务节点});
    const auto* 任务节点 = 任务节点读取.事实
        ? std::get_if<L1所有者范围节点事实>(&*任务节点读取.事实)
        : nullptr;
    if (任务节点读取.状态 != L1所有者范围读取状态::成功
        || !任务节点
        || 任务节点->编码 != 编码映射.任务节点
        || 任务节点->写入所有者 != 来源定位.所有者
        || 任务节点->种类 != 节点种类::普通
        || 任务节点->属性类型表示
        || !创建与退出形状有效(任务节点->创建事实代次,
            任务节点->退出事实代次))
        return 失败(L2结构状态::内部不一致);

    // 读回虚拟存在节点
    const auto 虚拟存在节点读取 = 第一层服务.读取所有者范围历史事实(
        {L1所有者范围CRUD合同版本, 编码映射.虚拟存在节点});
    const auto* 虚拟存在节点 = 虚拟存在节点读取.事实
        ? std::get_if<L1所有者范围节点事实>(&*虚拟存在节点读取.事实)
        : nullptr;
    if (虚拟存在节点读取.状态 != L1所有者范围读取状态::成功
        || !虚拟存在节点
        || 虚拟存在节点->编码 != 编码映射.虚拟存在节点
        || 虚拟存在节点->写入所有者 != 来源定位.所有者
        || 虚拟存在节点->种类 != 节点种类::普通
        || 虚拟存在节点->属性类型表示
        || !创建与退出形状有效(虚拟存在节点->创建事实代次,
            虚拟存在节点->退出事实代次))
        return 失败(L2结构状态::内部不一致);

    // 读回所有关系
    const auto 读回关系 = [&](稳定编码 编码)
        -> std::optional<L1所有者范围关系事实> {
        const auto 读取 = 第一层服务.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 编码});
        const auto* 关系 = 读取.事实
            ? std::get_if<L1所有者范围关系事实>(&*读取.事实)
            : nullptr;
        if (读取.状态 != L1所有者范围读取状态::成功 || !关系
            || 关系->编码 != 编码
            || 关系->写入所有者 != 来源定位.所有者
            || !创建与退出形状有效(关系->创建事实代次,
                关系->退出事实代次))
            return std::nullopt;
        return *关系;
    };

    const auto 族归属 = 读回关系(编码映射.任务族归属关系);
    const auto 列表项引用 = 读回关系(编码映射.任务需求列表项引用关系);
    const auto 虚拟存在归属 = 读回关系(编码映射.任务虚拟存在归属关系);
    if (!族归属 || !列表项引用 || !虚拟存在归属)
        return 失败(L2结构状态::内部不一致);

    if (精确重复) {
        const auto 退出代次 = 任务节点->退出事实代次;
        const bool 同代退出 = 虚拟存在节点->退出事实代次 == 退出代次
            && 族归属->退出事实代次 == 退出代次
            && 列表项引用->退出事实代次 == 退出代次
            && 虚拟存在归属->退出事实代次 == 退出代次;
        if (!同代退出 || (退出代次 && *退出代次 <= 写入代次))
            return 失败(L2结构状态::内部不一致);
    }

    // 验证族归属关系：任务 -> 任务锚点
    if (族归属->源节点 != 编码映射.任务节点
        || 族归属->目标节点 != 来源定位.任务锚点
        || 族归属->关系类型节点 != 来源定位.任务族归属关系类型
        || 族归属->角色或顺序 != 1)
        return 失败(L2结构状态::内部不一致);

    // 验证需求列表项引用关系：任务 -> 需求列表项
    if (列表项引用->源节点 != 编码映射.任务节点
        || 列表项引用->目标节点 != 请求.需求列表项.值
        || 列表项引用->关系类型节点
            != 类型定位.任务需求列表项引用关系类型
        || 列表项引用->角色或顺序 != 1)
        return 失败(L2结构状态::内部不一致);

    // 验证虚拟存在归属关系：任务 -> 虚拟存在
    if (虚拟存在归属->源节点 != 编码映射.任务节点
        || 虚拟存在归属->目标节点 != 编码映射.虚拟存在节点
        || 虚拟存在归属->关系类型节点
            != 来源定位.虚拟存在归属关系类型
        || 虚拟存在归属->角色或顺序 != 1)
        return 失败(L2结构状态::内部不一致);

    // 构造 L2任务身份来源事实
    L2任务身份来源事实 身份来源;
    身份来源.任务所有者 = 来源定位.所有者;
    身份来源.任务节点 = 编码映射.任务节点;
    身份来源.任务族锚点 = 来源定位.任务锚点;
    身份来源.任务族归属关系类型 = 来源定位.任务族归属关系类型;
    身份来源.任务族归属关系 = 编码映射.任务族归属关系;
    身份来源.任务节点生命周期 = {任务节点->创建事实代次,
        std::nullopt};
    身份来源.族归属关系生命周期 = {族归属->创建事实代次,
        std::nullopt};
    if (!L2任务身份来源事实完整(身份来源))
        return 失败(L2结构状态::内部不一致);

    // 构造 L2任务事实
    L2任务事实 任务事实;
    任务事实.身份 = 任务身份;
    任务事实.需求列表项 = 请求.需求列表项;
    任务事实.任务虚拟存在 = 虚拟存在身份;
    任务事实.生命周期 = {写入代次, std::nullopt};

    L2新增任务结果 结果;
    结果.结果头.合同版本 = L2结构合同版本;
    结果.结果头.状态 = 写入状态;
    结果.结果头.事实截止代次 = 写入代次;
    结果.结果头.变更事实代次 = 写入代次;
    结果.任务 = std::move(任务事实);
    结果.身份来源 = std::move(身份来源);
    return 结果;
}

// ===== 问题3：读取任务当前投影 =====

// 诊断责任：向上送出；一次 owner-aware 一致投影读取任务完整事实。
L2任务读取结果 读取任务当前投影(
    const L1事实基座服务& 第一层服务,
    const 任务身份来源定位& 来源定位,
    const 任务结构类型定位& 类型定位,
    const L2任务读取请求& 请求,
    std::uint64_t 期望代次) {
    const auto 失败 = [&](L2结构状态 状态) noexcept {
        L2任务读取结果 结果;
        结果.结果头.合同版本 = L2结构合同版本;
        结果.结果头.状态 = 状态;
        结果.结果头.事实截止代次 = 期望代次;
        结果.读取类别 = 请求.读取类别;
        结果.历史截止事实代次 = 请求.历史截止事实代次;
        return 结果;
    };

    L1所有者范围一致当前读取请求 L1请求;
    L1请求.合同版本 = L1所有者范围一致当前读取合同版本;
    L1请求.期望事实代次 = 期望代次;
    L1请求.所有者 = {来源定位.所有者};
    L1请求.节点 = {请求.任务.值, 来源定位.任务锚点,
        来源定位.任务族归属关系类型};
    L1请求.源关系组 = {
        {请求.任务.值, 来源定位.任务族归属关系类型},
        {请求.任务.值, 类型定位.任务需求列表项引用关系类型},
        {请求.任务.值, 来源定位.虚拟存在归属关系类型}};
    const auto 读取 = 第一层服务.尝试读取所有者范围一致当前投影(L1请求);

    const auto 顶层 = 解释任务身份来源一致读取顶层(读取, 期望代次);
    if (!顶层.成功())
        return 失败(顶层.状态);

    // 形状：1 所有者、3 节点、0 关系、0 值、3 源关系组、0 目标关系组
    if (读取.所有者.size() != 1 || 读取.节点.size() != 3
        || !读取.关系.empty() || !读取.值.empty()
        || 读取.源关系组.size() != 3
        || !读取.目标关系组.empty())
        return 失败(L2结构状态::内部不一致);

    const auto* 节点项 = 查找唯一任务节点项(读取, 请求.任务.值);
    const auto* 锚点项 = 查找唯一任务节点项(读取, 来源定位.任务锚点);
    const auto* 类型项 = 查找唯一任务节点项(读取, 来源定位.任务族归属关系类型);
    const auto* 族归属组项 = 查找唯一任务来源关系组(
        读取, 请求.任务.值, 来源定位.任务族归属关系类型);
    const auto* 列表项引用组项 = 查找唯一任务来源关系组(
        读取, 请求.任务.值, 类型定位.任务需求列表项引用关系类型);
    const auto* 虚拟存在归属组项 = 查找唯一任务来源关系组(
        读取, 请求.任务.值, 来源定位.虚拟存在归属关系类型);
    if (!节点项 || !锚点项 || !类型项 || !族归属组项
        || !列表项引用组项 || !虚拟存在归属组项)
        return 失败(L2结构状态::内部不一致);

    // 任务节点
    if (节点项->状态 != L1所有者范围一致当前读取项目状态::成功
        || !节点项->事实)
        return 失败(映射任务身份来源当前项目状态(节点项->状态));
    const auto& 节点 = *节点项->事实;
    if (节点.写入所有者 != 来源定位.所有者
        || 节点.种类 != 节点种类::普通
        || 节点.属性类型表示 || 节点.退出事实代次)
        return 失败(L2结构状态::入口拒绝);

    // 锚点/类型完整性
    if (锚点项->状态 != L1所有者范围一致当前读取项目状态::成功
        || !锚点项->事实
        || 类型项->状态 != L1所有者范围一致当前读取项目状态::成功
        || !类型项->事实)
        return 失败(L2结构状态::内部不一致);
    const auto& 锚点 = *锚点项->事实;
    const auto& 类型 = *类型项->事实;
    const auto 内部节点完整 = [&](
        const L1所有者范围节点事实& 内部节点) noexcept {
        return 内部节点.写入所有者 == 来源定位.所有者
            && 内部节点.种类 == 节点种类::普通
            && !内部节点.属性类型表示 && !内部节点.退出事实代次
            && 内部节点.当前属性.empty();
    };
    if (!内部节点完整(锚点) || !内部节点完整(类型)
        || 锚点.编码 != 来源定位.任务锚点
        || 类型.编码 != 来源定位.任务族归属关系类型
        || 锚点.创建事实代次 != 来源定位.建立事实代次
        || 类型.创建事实代次 != 来源定位.建立事实代次)
        return 失败(L2结构状态::内部不一致);

    // 3 条关系：每条恰有一个当前成员
    const auto 提取当前关系 = [&](
        const L1所有者范围一致源关系组读取结果项* 组)
        -> const L1所有者范围一致关系对端投影* {
        if (!组 || 组->成员.size() != 1) return nullptr;
        const auto& 成员 = 组->成员.front();
        if (成员.关系.退出事实代次
            || 成员.关系.创建事实代次 != 节点.创建事实代次
            || 成员.关系.角色或顺序 != 1)
            return nullptr;
        return &成员;
    };
    const auto* 族归属成员 = 提取当前关系(族归属组项);
    const auto* 列表项引用成员 = 提取当前关系(列表项引用组项);
    const auto* 虚拟存在归属成员 = 提取当前关系(虚拟存在归属组项);
    if (!族归属成员 || !列表项引用成员 || !虚拟存在归属成员)
        return 失败(L2结构状态::内部不一致);

    // 族归属：任务 -> 任务锚点
    if (族归属成员->关系.源节点 != 请求.任务.值
        || 族归属成员->关系.目标节点 != 来源定位.任务锚点
        || 族归属成员->关系.关系类型节点
            != 来源定位.任务族归属关系类型)
        return 失败(L2结构状态::内部不一致);

    // 需求列表项引用：任务 -> 需求列表项
    if (列表项引用成员->关系.源节点 != 请求.任务.值
        || 列表项引用成员->关系.关系类型节点
            != 类型定位.任务需求列表项引用关系类型)
        return 失败(L2结构状态::内部不一致);

    // 虚拟存在归属：任务 -> 虚拟存在
    if (虚拟存在归属成员->关系.源节点 != 请求.任务.值
        || 虚拟存在归属成员->关系.关系类型节点
            != 来源定位.虚拟存在归属关系类型)
        return 失败(L2结构状态::内部不一致);
    const auto& 虚拟存在节点 = 虚拟存在归属成员->对端节点;
    if (虚拟存在节点.写入所有者 != 来源定位.所有者
        || 虚拟存在节点.种类 != 节点种类::普通
        || 虚拟存在节点.属性类型表示
        || 虚拟存在节点.退出事实代次
        || 虚拟存在节点.创建事实代次 != 节点.创建事实代次)
        return 失败(L2结构状态::内部不一致);

    // 构造 L2任务身份来源事实
    L2任务身份来源事实 身份来源;
    身份来源.任务所有者 = 来源定位.所有者;
    身份来源.任务节点 = 请求.任务.值;
    身份来源.任务族锚点 = 来源定位.任务锚点;
    身份来源.任务族归属关系类型 = 来源定位.任务族归属关系类型;
    身份来源.任务族归属关系 = 族归属成员->关系.编码;
    身份来源.任务节点生命周期 = {节点.创建事实代次,
        节点.退出事实代次};
    身份来源.族归属关系生命周期 = {族归属成员->关系.创建事实代次,
        族归属成员->关系.退出事实代次};
    if (!L2任务身份来源事实完整(身份来源))
        return 失败(L2结构状态::内部不一致);

    // 构造 L2任务事实
    L2任务事实 任务事实;
    任务事实.身份 = 请求.任务;
    任务事实.需求列表项 = L2需求列表项身份{
        列表项引用成员->关系.目标节点};
    任务事实.任务虚拟存在 = L2任务虚拟存在身份{
        虚拟存在节点.编码};
    任务事实.生命周期 = {节点.创建事实代次, 节点.退出事实代次};

    L2任务读取结果 结果;
    结果.结果头.合同版本 = L2结构合同版本;
    结果.结果头.状态 = L2结构状态::已读取;
    结果.结果头.事实截止代次 = 顶层.事实截止代次;
    结果.读取类别 = 请求.读取类别;
    结果.历史截止事实代次 = 请求.历史截止事实代次;
    结果.任务 = std::move(任务事实);
    结果.身份来源 = std::move(身份来源);
    return 结果;
}

// 诊断责任：向上送出；读取由“任务轮次与正式存在引用”入口形成的当前任务。
// 该入口复用既有 L2任务事实 结果形状，但第三项身份来自正式存在关系，
// 不要求外部正式存在节点与 task owner 同所有者或同创建代次。
L2任务读取结果 读取任务轮次核心当前投影(
    const L1事实基座服务& 第一层服务,
    const 任务身份来源定位& 来源定位,
    const 任务结构类型定位& 类型定位,
    const L2任务读取请求& 请求,
    std::uint64_t 期望代次) {
    const auto 失败 = [&](L2结构状态 状态) noexcept {
        L2任务读取结果 结果;
        结果.结果头 = {L2结构合同版本, 状态, 期望代次, std::nullopt};
        结果.读取类别 = 请求.读取类别;
        结果.历史截止事实代次 = 请求.历史截止事实代次;
        return 结果;
    };

    L1所有者范围一致当前读取请求 L1请求;
    L1请求.合同版本 = L1所有者范围一致当前读取合同版本;
    L1请求.期望事实代次 = 期望代次;
    L1请求.所有者 = {来源定位.所有者};
    L1请求.节点 = {请求.任务.值, 来源定位.任务锚点,
        来源定位.任务族归属关系类型};
    L1请求.源关系组 = {
        {请求.任务.值, 来源定位.任务族归属关系类型},
        {请求.任务.值, 类型定位.任务需求列表项引用关系类型},
        {请求.任务.值, 类型定位.任务正式存在关系类型}};
    const auto 读取 = 第一层服务.尝试读取所有者范围一致当前投影(L1请求);
    const auto 顶层 = 解释任务身份来源一致读取顶层(读取, 期望代次);
    if (!顶层.成功()) return 失败(顶层.状态);
    if (读取.所有者.size() != 1 || 读取.节点.size() != 3
        || !读取.关系.empty() || !读取.值.empty()
        || 读取.源关系组.size() != 3 || !读取.目标关系组.empty())
        return 失败(L2结构状态::内部不一致);

    const auto* 节点项 = 查找唯一任务节点项(读取, 请求.任务.值);
    const auto* 锚点项 = 查找唯一任务节点项(读取, 来源定位.任务锚点);
    const auto* 类型项 = 查找唯一任务节点项(
        读取, 来源定位.任务族归属关系类型);
    const auto* 族归属组项 = 查找唯一任务来源关系组(
        读取, 请求.任务.值, 来源定位.任务族归属关系类型);
    const auto* 列表项引用组项 = 查找唯一任务来源关系组(
        读取, 请求.任务.值, 类型定位.任务需求列表项引用关系类型);
    const auto* 正式存在组项 = 查找唯一任务来源关系组(
        读取, 请求.任务.值, 类型定位.任务正式存在关系类型);
    if (!节点项 || !锚点项 || !类型项 || !族归属组项
        || !列表项引用组项 || !正式存在组项)
        return 失败(L2结构状态::内部不一致);
    if (节点项->状态 != L1所有者范围一致当前读取项目状态::成功
        || !节点项->事实)
        return 失败(映射任务身份来源当前项目状态(节点项->状态));
    const auto& 节点 = *节点项->事实;
    if (节点.写入所有者 != 来源定位.所有者
        || 节点.种类 != 节点种类::普通 || 节点.属性类型表示
        || 节点.退出事实代次)
        return 失败(L2结构状态::入口拒绝);

    const auto 内部节点完整 = [&](const L1所有者范围节点事实& 内部节点) noexcept {
        return 内部节点.写入所有者 == 来源定位.所有者
            && 内部节点.种类 == 节点种类::普通
            && !内部节点.属性类型表示 && !内部节点.退出事实代次
            && 内部节点.当前属性.empty();
    };
    if (锚点项->状态 != L1所有者范围一致当前读取项目状态::成功
        || !锚点项->事实
        || 类型项->状态 != L1所有者范围一致当前读取项目状态::成功
        || !类型项->事实 || !内部节点完整(*锚点项->事实)
        || !内部节点完整(*类型项->事实)
        || 锚点项->事实->编码 != 来源定位.任务锚点
        || 类型项->事实->编码 != 来源定位.任务族归属关系类型
        || 锚点项->事实->创建事实代次 != 来源定位.建立事实代次
        || 类型项->事实->创建事实代次 != 来源定位.建立事实代次)
        return 失败(L2结构状态::内部不一致);

    const auto 提取当前关系 = [&](const auto* 组)
        -> const L1所有者范围一致关系对端投影* {
        if (!组 || 组->成员.size() != 1) return nullptr;
        const auto& 成员 = 组->成员.front();
        if (成员.关系.退出事实代次
            || 成员.关系.创建事实代次 != 节点.创建事实代次
            || 成员.关系.角色或顺序 != 1)
            return nullptr;
        return &成员;
    };
    const auto* 族归属成员 = 提取当前关系(族归属组项);
    const auto* 列表项引用成员 = 提取当前关系(列表项引用组项);
    const auto* 正式存在成员 = 提取当前关系(正式存在组项);
    if (!族归属成员 || !列表项引用成员 || !正式存在成员)
        return 失败(L2结构状态::内部不一致);
    if (族归属成员->关系.源节点 != 请求.任务.值
        || 族归属成员->关系.目标节点 != 来源定位.任务锚点
        || 族归属成员->关系.关系类型节点
            != 来源定位.任务族归属关系类型
        || 列表项引用成员->关系.源节点 != 请求.任务.值
        || 列表项引用成员->关系.关系类型节点
            != 类型定位.任务需求列表项引用关系类型
        || 正式存在成员->关系.源节点 != 请求.任务.值
        || 正式存在成员->关系.关系类型节点
            != 类型定位.任务正式存在关系类型)
        return 失败(L2结构状态::内部不一致);
    const auto& 正式存在节点 = 正式存在成员->对端节点;
    if (正式存在节点.种类 != 节点种类::普通
        || 正式存在节点.属性类型表示 || 正式存在节点.退出事实代次)
        return 失败(L2结构状态::内部不一致);

    L2任务身份来源事实 身份来源{来源定位.所有者, 请求.任务.值,
        来源定位.任务锚点, 来源定位.任务族归属关系类型,
        族归属成员->关系.编码, {节点.创建事实代次, 节点.退出事实代次},
        {族归属成员->关系.创建事实代次,
            族归属成员->关系.退出事实代次}};
    if (!L2任务身份来源事实完整(身份来源))
        return 失败(L2结构状态::内部不一致);
    L2任务事实 任务事实{请求.任务,
        L2需求列表项身份{列表项引用成员->关系.目标节点},
        L2任务虚拟存在身份{正式存在节点.编码},
        {节点.创建事实代次, 节点.退出事实代次}};
    L2任务读取结果 结果;
    结果.结果头 = {L2结构合同版本, L2结构状态::已读取,
        顶层.事实截止代次, std::nullopt};
    结果.读取类别 = 请求.读取类别;
    结果.历史截止事实代次 = 请求.历史截止事实代次;
    结果.任务 = std::move(任务事实);
    结果.身份来源 = std::move(身份来源);
    return 结果;
}

// ===== 问题4：按需求列表项读取当前任务 =====

// 诊断责任：向上送出；反向查询需求列表项→任务引用关系，0合法空/1当前任务/>1不一致。
L2按需求列表项读取当前任务结果 按需求列表项读取当前任务当前投影(
    const L1事实基座服务& 第一层服务,
    const 任务身份来源定位& 来源定位,
    const 任务结构类型定位& 类型定位,
    const L2按需求列表项读取当前任务请求& 请求,
    std::uint64_t 期望代次) {
    const auto 失败 = [&](L2结构状态 状态) noexcept {
        L2按需求列表项读取当前任务结果 结果;
        结果.结果头.合同版本 = L2结构合同版本;
        结果.结果头.状态 = 状态;
        结果.结果头.事实截止代次 = 期望代次;
        return 结果;
    };
    const auto 空集成功 = [&]() noexcept {
        L2按需求列表项读取当前任务结果 结果;
        结果.结果头.合同版本 = L2结构合同版本;
        结果.结果头.状态 = L2结构状态::已读取;
        结果.结果头.事实截止代次 = 期望代次;
        return 结果;
    };

    L1所有者范围一致当前读取请求 L1请求;
    L1请求.合同版本 = L1所有者范围一致当前读取合同版本;
    L1请求.期望事实代次 = 期望代次;
    L1请求.所有者 = {来源定位.所有者};
    L1请求.目标关系组 = {
        {请求.需求列表项.值,
            类型定位.任务需求列表项引用关系类型}};
    const auto 读取 = 第一层服务.尝试读取所有者范围一致当前投影(L1请求);

    const auto 顶层 = 解释任务身份来源一致读取顶层(读取, 期望代次);
    if (!顶层.成功())
        return 失败(顶层.状态);

    // 形状：1 所有者、0 节点、0 关系、0 值、0 源关系组、1 目标关系组
    if (读取.所有者.size() != 1 || !读取.节点.empty()
        || !读取.关系.empty() || !读取.值.empty()
        || !读取.源关系组.empty()
        || 读取.目标关系组.size() != 1)
        return 失败(L2结构状态::内部不一致);

    const auto& 组 = 读取.目标关系组.front();
    if (组.目标节点 != 请求.需求列表项.值
        || 组.关系类型节点
            != 类型定位.任务需求列表项引用关系类型)
        return 失败(L2结构状态::内部不一致);

    // 统计当前有效成员
    std::vector<const L1所有者范围一致关系对端投影*> 当前成员;
    for (const auto& 成员 : 组.成员) {
        if (!成员.关系.退出事实代次
            && 成员.关系.关系类型节点
                == 类型定位.任务需求列表项引用关系类型
            && 成员.关系.目标节点 == 请求.需求列表项.值)
            当前成员.push_back(&成员);
    }

    if (当前成员.empty()) return 空集成功();
    if (当前成员.size() > 1)
        return 失败(L2结构状态::内部不一致);

    // 恰一条当前任务引用 → 读取任务完整事实
    const auto 任务编码 = 当前成员.front()->关系.源节点;
    L2任务读取请求 任务请求;
    任务请求.请求头.合同版本 = L2结构合同版本;
    任务请求.请求头.期望事实代次 = 期望代次;
    任务请求.读取类别 = L2读取类别::当前;
    任务请求.任务 = L2任务身份{任务编码};
    const auto 任务结果 = 读取任务当前投影(
        第一层服务, 来源定位, 类型定位, 任务请求, 期望代次);
    if (!任务结果.成功())
        return 失败(任务结果.结果头.状态);

    L2按需求列表项读取当前任务结果 结果;
    结果.结果头.合同版本 = L2结构合同版本;
    结果.结果头.状态 = L2结构状态::已读取;
    结果.结果头.事实截止代次 = 顶层.事实截止代次;
    结果.任务 = 任务结果.任务;
    return 结果;
}

// ===== 问题5：读取任务虚拟存在当前投影 =====

// 诊断责任：向上送出；从任务→虚拟存在归属关系对端获取虚拟存在节点。
L2任务虚拟存在读取结果 读取任务虚拟存在当前投影(
    const L1事实基座服务& 第一层服务,
    const 任务身份来源定位& 来源定位,
    const L2任务虚拟存在读取请求& 请求,
    std::uint64_t 期望代次) {
    const auto 失败 = [&](L2结构状态 状态) noexcept {
        L2任务虚拟存在读取结果 结果;
        结果.结果头.合同版本 = L2结构合同版本;
        结果.结果头.状态 = 状态;
        结果.结果头.事实截止代次 = 期望代次;
        结果.读取类别 = 请求.读取类别;
        结果.历史截止事实代次 = 请求.历史截止事实代次;
        return 结果;
    };

    L1所有者范围一致当前读取请求 L1请求;
    L1请求.合同版本 = L1所有者范围一致当前读取合同版本;
    L1请求.期望事实代次 = 期望代次;
    L1请求.所有者 = {来源定位.所有者};
    L1请求.节点 = {请求.任务.值, 来源定位.任务锚点,
        来源定位.任务族归属关系类型};
    L1请求.源关系组 = {
        {请求.任务.值, 来源定位.虚拟存在归属关系类型}};
    const auto 读取 = 第一层服务.尝试读取所有者范围一致当前投影(L1请求);

    const auto 顶层 = 解释任务身份来源一致读取顶层(读取, 期望代次);
    if (!顶层.成功())
        return 失败(顶层.状态);

    // 形状：1 所有者、3 节点、0 关系、0 值、1 源关系组、0 目标关系组
    if (读取.所有者.size() != 1 || 读取.节点.size() != 3
        || !读取.关系.empty() || !读取.值.empty()
        || 读取.源关系组.size() != 1
        || !读取.目标关系组.empty())
        return 失败(L2结构状态::内部不一致);

    const auto* 节点项 = 查找唯一任务节点项(读取, 请求.任务.值);
    const auto* 虚拟存在归属组项 = 查找唯一任务来源关系组(
        读取, 请求.任务.值, 来源定位.虚拟存在归属关系类型);
    if (!节点项 || !虚拟存在归属组项)
        return 失败(L2结构状态::内部不一致);

    // 任务节点
    if (节点项->状态 != L1所有者范围一致当前读取项目状态::成功
        || !节点项->事实)
        return 失败(映射任务身份来源当前项目状态(节点项->状态));
    const auto& 节点 = *节点项->事实;
    if (节点.写入所有者 != 来源定位.所有者
        || 节点.种类 != 节点种类::普通
        || 节点.属性类型表示 || 节点.退出事实代次)
        return 失败(L2结构状态::入口拒绝);

    // 虚拟存在归属关系：恰有一条当前成员
    if (虚拟存在归属组项->成员.size() != 1)
        return 失败(L2结构状态::内部不一致);
    const auto& 成员 = 虚拟存在归属组项->成员.front();
    if (成员.关系.退出事实代次
        || 成员.关系.源节点 != 请求.任务.值
        || 成员.关系.关系类型节点
            != 来源定位.虚拟存在归属关系类型
        || 成员.关系.角色或顺序 != 1
        || 成员.关系.创建事实代次 != 节点.创建事实代次)
        return 失败(L2结构状态::内部不一致);

    // 对端节点（虚拟存在）
    const auto& 虚拟存在节点 = 成员.对端节点;
    if (虚拟存在节点.写入所有者 != 来源定位.所有者
        || 虚拟存在节点.种类 != 节点种类::普通
        || 虚拟存在节点.属性类型表示
        || 虚拟存在节点.退出事实代次
        || 虚拟存在节点.创建事实代次 != 节点.创建事实代次)
        return 失败(L2结构状态::内部不一致);

    // 构造 L2任务虚拟存在事实
    L2任务虚拟存在事实 事实;
    事实.身份 = L2任务虚拟存在身份{虚拟存在节点.编码};
    事实.任务 = 请求.任务;
    事实.生命周期 = {虚拟存在节点.创建事实代次,
        虚拟存在节点.退出事实代次};

    L2任务虚拟存在读取结果 结果;
    结果.结果头.合同版本 = L2结构合同版本;
    结果.结果头.状态 = L2结构状态::已读取;
    结果.结果头.事实截止代次 = 顶层.事实截止代次;
    结果.读取类别 = 请求.读取类别;
    结果.历史截止事实代次 = 请求.历史截止事实代次;
    结果.任务虚拟存在 = std::move(事实);
    return 结果;
}

// ===== 问题6：退出任务 =====

// 诊断责任：向上送出；验证任务及其自有关系当前有效，可安全退出。
L2结构状态 验证任务退出事实闭包当前有效(
    const L1所有者范围一致当前读取结果& 读取,
    const 任务身份来源定位& 来源定位,
    const 任务结构类型定位& 类型定位,
    稳定编码 任务编码) noexcept {
    // 形状：1 所有者、3 节点、0 关系、0 值、3 源关系组、0 目标关系组
    if (读取.所有者.size() != 1 || 读取.节点.size() != 3
        || !读取.关系.empty() || !读取.值.empty()
        || 读取.源关系组.size() != 3
        || !读取.目标关系组.empty())
        return L2结构状态::内部不一致;

    const auto* 节点项 = 查找唯一任务节点项(读取, 任务编码);
    if (!节点项
        || 节点项->状态
            != L1所有者范围一致当前读取项目状态::成功
        || !节点项->事实)
        return L2结构状态::内部不一致;
    const auto& 节点 = *节点项->事实;
    if (节点.写入所有者 != 来源定位.所有者
        || 节点.种类 != 节点种类::普通
        || 节点.属性类型表示 || 节点.退出事实代次)
        return L2结构状态::入口拒绝;

    const auto* 族归属组项 = 查找唯一任务来源关系组(
        读取, 任务编码, 来源定位.任务族归属关系类型);
    const auto* 列表项引用组项 = 查找唯一任务来源关系组(
        读取, 任务编码,
        类型定位.任务需求列表项引用关系类型);
    const auto* 虚拟存在归属组项 = 查找唯一任务来源关系组(
        读取, 任务编码, 来源定位.虚拟存在归属关系类型);
    if (!族归属组项 || !列表项引用组项
        || !虚拟存在归属组项)
        return L2结构状态::内部不一致;

    // 每条关系恰有一个当前成员
    const auto 验证当前关系 = [&](
        const L1所有者范围一致源关系组读取结果项* 组,
        稳定编码 期望关系类型) noexcept -> bool {
        if (!组 || 组->成员.size() != 1) return false;
        const auto& 成员 = 组->成员.front();
        return !成员.关系.退出事实代次
            && 成员.关系.源节点 == 任务编码
            && 成员.关系.关系类型节点 == 期望关系类型
            && 成员.关系.创建事实代次 == 节点.创建事实代次
            && 成员.关系.角色或顺序 == 1;
    };
    if (!验证当前关系(族归属组项, 来源定位.任务族归属关系类型)
        || !验证当前关系(列表项引用组项,
            类型定位.任务需求列表项引用关系类型)
        || !验证当前关系(虚拟存在归属组项,
            来源定位.虚拟存在归属关系类型))
        return L2结构状态::内部不一致;

    return L2结构状态::已读取;
}

// 诊断责任：向上送出；读回退出后的任务节点和关系。
L2任务退出结果 读回退出任务(
    const L1事实基座服务& 第一层服务,
    const 任务身份来源定位& 来源定位,
    const L2任务退出请求& 请求,
    稳定编码 虚拟存在编码,
    稳定编码 族归属编码,
    稳定编码 列表项引用编码,
    稳定编码 虚拟存在归属编码,
    std::uint64_t 写入代次) {
    const auto 失败 = [&](L2结构状态 状态) noexcept {
        L2任务退出结果 结果;
        结果.结果头.合同版本 = L2结构合同版本;
        结果.结果头.状态 = 状态;
        结果.结果头.事实截止代次 = 写入代次;
        结果.结果头.变更事实代次 = 写入代次;
        return 结果;
    };

    // 读回任务节点
    const auto 节点读取 = 第一层服务.读取所有者范围历史事实(
        {L1所有者范围CRUD合同版本, 请求.任务.值});
    const auto* 节点 = 节点读取.事实
        ? std::get_if<L1所有者范围节点事实>(&*节点读取.事实)
        : nullptr;
    if (节点读取.状态 != L1所有者范围读取状态::成功
        || !节点
        || 节点->编码 != 请求.任务.值
        || 节点->写入所有者 != 来源定位.所有者
        || 节点->种类 != 节点种类::普通
        || 节点->属性类型表示
        || !节点->退出事实代次
        || 节点->退出事实代次 != 写入代次)
        return 失败(L2结构状态::内部不一致);

    // 读回虚拟存在节点
    const auto 虚拟存在节点读取 = 第一层服务.读取所有者范围历史事实(
        {L1所有者范围CRUD合同版本, 虚拟存在编码});
    const auto* 虚拟存在节点 = 虚拟存在节点读取.事实
        ? std::get_if<L1所有者范围节点事实>(
            &*虚拟存在节点读取.事实)
        : nullptr;
    if (虚拟存在节点读取.状态 != L1所有者范围读取状态::成功
        || !虚拟存在节点
        || 虚拟存在节点->编码 != 虚拟存在编码
        || 虚拟存在节点->写入所有者 != 来源定位.所有者
        || 虚拟存在节点->种类 != 节点种类::普通
        || 虚拟存在节点->属性类型表示
        || !虚拟存在节点->退出事实代次
        || 虚拟存在节点->退出事实代次 != 写入代次)
        return 失败(L2结构状态::内部不一致);

    // 读回 3 条退出关系
    const auto 读回关系 = [&](稳定编码 编码)
        -> std::optional<L1所有者范围关系事实> {
        const auto 读取 = 第一层服务.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 编码});
        const auto* 关系 = 读取.事实
            ? std::get_if<L1所有者范围关系事实>(&*读取.事实)
            : nullptr;
        if (读取.状态 != L1所有者范围读取状态::成功 || !关系
            || 关系->编码 != 编码
            || 关系->写入所有者 != 来源定位.所有者
            || !关系->退出事实代次
            || 关系->退出事实代次 != 写入代次
            || 关系->创建事实代次 != 节点->创建事实代次)
            return std::nullopt;
        return *关系;
    };

    const auto 族归属 = 读回关系(族归属编码);
    const auto 列表项引用 = 读回关系(列表项引用编码);
    const auto 虚拟存在归属 = 读回关系(虚拟存在归属编码);
    if (!族归属 || !列表项引用 || !虚拟存在归属)
        return 失败(L2结构状态::内部不一致);

    // 构造退出后任务事实
    L2任务事实 任务事实;
    任务事实.身份 = 请求.任务;
    任务事实.需求列表项 = L2需求列表项身份{
        列表项引用->目标节点};
    任务事实.任务虚拟存在 = L2任务虚拟存在身份{
        虚拟存在编码};
    任务事实.生命周期 = {节点->创建事实代次,
        节点->退出事实代次};

    L2任务退出结果 结果;
    结果.结果头.合同版本 = L2结构合同版本;
    结果.结果头.状态 = L2结构状态::已提交;
    结果.结果头.事实截止代次 = 写入代次;
    结果.结果头.变更事实代次 = 写入代次;
    结果.退出后任务 = std::move(任务事实);
    return 结果;
}

struct 正式选择序列写入器 final {
    std::vector<std::uint64_t> 值;
    void 写(std::uint64_t 数) { 值.push_back(数); }
    void 写有符号(std::int64_t 数) { 值.push_back(std::bit_cast<std::uint64_t>(数)); }
    void 写编码(稳定编码 编码) { 写(编码.值); }
    void 写生命周期(const L2生命周期& 生命周期) {
        写(生命周期.创建事实代次);
        写(生命周期.退出事实代次.value_or(0));
    }
    void 写原始值(const L2原始值材料& 材料) {
        写(材料.index());
        std::visit([&](const auto& 值材料) {
            using 类型 = std::decay_t<decltype(值材料)>;
            if constexpr (std::is_same_v<类型, std::int64_t>) {
                写有符号(值材料);
            } else if constexpr (std::is_same_v<类型, std::vector<std::int64_t>>) {
                写(值材料.size());
                for (const auto 值项 : 值材料) 写有符号(值项);
            } else if constexpr (std::is_same_v<类型, std::vector<std::uint64_t>>) {
                写(值材料.size());
                for (const auto 值项 : 值材料) 写(值项);
            } else {
                写编码(值材料.编码);
            }
        }, 材料);
    }
    void 写属性(const L2属性事实& 属性) {
        写编码(属性.属性类型身份); 写编码(属性.值稳定编码);
        写原始值(属性.类型化不可变材料); 写编码(属性.来源稳定编码);
        写(属性.创建事实代次); 写(属性.退出事实代次.value_or(0));
    }
    void 写规格(const L2方法规格材料& 规格) {
        写(规格.角色); 写(规格.顺序); 写原始值(规格.值);
    }
    void 写任务身份来源(const L2任务身份来源事实& 来源) {
        写编码(来源.任务所有者.编码); 写编码(来源.任务节点);
        写编码(来源.任务族锚点); 写编码(来源.任务族归属关系类型);
        写编码(来源.任务族归属关系); 写生命周期(来源.任务节点生命周期);
        写生命周期(来源.族归属关系生命周期);
    }
    void 写方法生命周期(const L2方法生命周期事实& 生命周期) {
        写编码(生命周期.方法.值); 写(生命周期.生命周期版本.值);
        写有符号(生命周期.发生时间); 写编码(生命周期.来源稳定编码);
        写生命周期(生命周期.生命周期);
    }
    void 写方法条件(const L2方法条件事实& 条件) {
        写编码(条件.身份.值); 写编码(条件.方法.值); 写(条件.作用对象角色);
        写编码(条件.目标对象类型.值); 写(条件.场景.has_value());
        if (条件.场景) 写编码(条件.场景->值);
        写编码(条件.特征定义.值); 写编码(条件.条件状态合同.值);
        写编码(条件.来源稳定编码); 写(条件.内容版本.值);
        写生命周期(条件.生命周期);
    }
    void 写结果类型(const L2方法结果类型引用& 类型) {
        写(类型.index());
        std::visit([&](const auto& 身份) { 写编码(身份.值); }, 类型);
    }
    void 写方法结果(const L2方法结果事实& 结果) {
        写编码(结果.身份.值); 写编码(结果.方法.值); 写(结果.作用对象角色);
        写编码(结果.目标对象类型.值); 写编码(结果.特征定义.值);
        写(static_cast<std::uint64_t>(结果.变化方向)); 写结果类型(结果.结果类型);
        写编码(结果.来源稳定编码); 写(结果.内容版本.值);
        写生命周期(结果.生命周期);
    }
    void 写输入规格(const L2方法输入规格事实& 输入) {
        写编码(输入.身份.值); 写编码(输入.方法.值); 写(输入.条件.has_value());
        if (输入.条件) 写编码(输入.条件->值);
        写(输入.作用对象角色); 写(输入.顺序); 写规格(输入.规格);
        写编码(输入.来源稳定编码); 写(输入.内容版本.值);
        写生命周期(输入.生命周期);
    }
    void 写限制条件(const L2方法限制条件事实& 限制) {
        写编码(限制.身份.值); 写编码(限制.方法.值); 写(限制.条件.has_value());
        if (限制.条件) 写编码(限制.条件->值);
        写(static_cast<std::uint64_t>(限制.类别)); 写(限制.作用对象角色);
        写(限制.顺序); 写规格(限制.规格); 写编码(限制.来源稳定编码);
        写(限制.内容版本.值); 写生命周期(限制.生命周期);
    }
    void 写动作入口(const L2方法动作入口事实& 动作) {
        写编码(动作.身份.值); 写编码(动作.方法.值); 写编码(动作.输入场景.值);
        写编码(动作.输出场景.值); 写(动作.稳定动作键.值);
        写(动作.内容版本.值); 写编码(动作.来源稳定编码);
        写生命周期(动作.生命周期);
    }
    void 写动作冻结项_v2(const L2任务执行路径动作冻结项_v2& 动作) {
        写编码(动作.动作入口.值); 写编码(动作.方法.值);
        写(动作.方法内容版本.值); 写(动作.顺序);
        写(动作.稳定动作键.值);
        写(static_cast<std::uint64_t>(动作.作用范围));
        写编码(动作.作用主体约束.值); 写(动作.场景约束.has_value());
        if (动作.场景约束) 写编码(动作.场景约束->值);
        写编码(动作.动作范围材料); 写(动作.技术许可合同版本);
        写编码(动作.预期结果材料); 写编码(动作.结果验证合同);
        写编码(动作.动作归因合同);
        写(动作.来源动作生命周期.创建事实代次);
        写(动作.来源动作生命周期.退出事实代次.has_value());
        if (动作.来源动作生命周期.退出事实代次)
            写(*动作.来源动作生命周期.退出事实代次);
    }
    void 写场景成员(const L2存在场景成员引用事实& 成员) {
        写编码(成员.关系稳定编码); 写编码(成员.场景.值);
        写编码(成员.存在.值); 写(成员.顺序); 写生命周期(成员.生命周期);
    }
    void 写特征值(const L2特征值事实& 特征值) {
        写编码(特征值.值稳定编码); 写编码(特征值.特征实例.值);
        写原始值(特征值.类型化不可变材料); 写编码(特征值.来源稳定编码);
        写生命周期(特征值.生命周期);
    }
    void 写状态(const L2状态事实& 状态) {
        写编码(状态.身份.值); 写编码(状态.主体存在.值);
        写编码(状态.特征实例.值); 写属性(状态.精确值); 写属性(状态.时间值);
        写编码(状态.来源稳定编码); 写生命周期(状态.生命周期);
    }
};

struct 正式选择序列读取器 final {
    const std::vector<std::uint64_t>& 值;
    std::size_t 位置 = 0;
    std::optional<std::uint64_t> 读() noexcept {
        if (位置 >= 值.size()) return std::nullopt;
        return 值[位置++];
    }
    std::optional<std::int64_t> 读有符号() noexcept {
        const auto 数 = 读();
        return 数 ? std::optional<std::int64_t>{std::bit_cast<std::int64_t>(*数)}
                  : std::nullopt;
    }
    std::optional<稳定编码> 读编码() noexcept {
        const auto 数 = 读(); return 数 ? std::optional<稳定编码>{{*数}} : std::nullopt;
    }
    bool 结束() const noexcept { return 位置 == 值.size(); }
};

bool 读生命周期(正式选择序列读取器& 读, L2生命周期& 生命周期) noexcept {
    const auto 创建 = 读.读(); const auto 退出 = 读.读();
    if (!创建 || !退出) return false;
    生命周期.创建事实代次 = *创建;
    生命周期.退出事实代次 = *退出 == 0 ? std::nullopt
        : std::optional<std::uint64_t>{*退出};
    return true;
}

bool 读原始值(正式选择序列读取器& 读, L2原始值材料& 材料) {
    const auto 分支 = 读.读(); if (!分支) return false;
    if (*分支 == 0) { const auto 值 = 读.读有符号(); if (!值) return false; 材料 = *值; return true; }
    if (*分支 == 1) {
        const auto 数量 = 读.读(); if (!数量 || *数量 > 1'000'000) return false;
        std::vector<std::int64_t> 组; 组.reserve(static_cast<std::size_t>(*数量));
        for (std::uint64_t i = 0; i < *数量; ++i) { const auto 值 = 读.读有符号(); if (!值) return false; 组.push_back(*值); }
        材料 = std::move(组); return true;
    }
    if (*分支 == 2) {
        const auto 数量 = 读.读(); if (!数量 || *数量 > 1'000'000) return false;
        std::vector<std::uint64_t> 组; 组.reserve(static_cast<std::size_t>(*数量));
        for (std::uint64_t i = 0; i < *数量; ++i) { const auto 值 = 读.读(); if (!值) return false; 组.push_back(*值); }
        材料 = std::move(组); return true;
    }
    if (*分支 == 3) { const auto 编码 = 读.读编码(); if (!编码) return false; 材料 = L2独立材料引用{*编码}; return true; }
    return false;
}

bool 读属性(正式选择序列读取器& 读, L2属性事实& 属性) {
    const auto 类型 = 读.读编码(); const auto 值编码 = 读.读编码();
    if (!类型 || !值编码 || !读原始值(读, 属性.类型化不可变材料)) return false;
    const auto 来源 = 读.读编码(); const auto 创建 = 读.读(); const auto 退出 = 读.读();
    if (!来源 || !创建 || !退出) return false;
    属性.属性类型身份 = *类型; 属性.值稳定编码 = *值编码;
    属性.来源稳定编码 = *来源; 属性.创建事实代次 = *创建;
    属性.退出事实代次 = *退出 == 0 ? std::nullopt : std::optional<std::uint64_t>{*退出};
    return true;
}

bool 读规格(正式选择序列读取器& 读, L2方法规格材料& 规格) {
    const auto 角色 = 读.读(); const auto 顺序 = 读.读();
    if (!角色 || !顺序 || !读原始值(读, 规格.值)) return false;
    规格.角色 = *角色; 规格.顺序 = *顺序; return true;
}

bool 读任务身份来源(正式选择序列读取器& 读, L2任务身份来源事实& 来源) noexcept {
    const auto 所有者 = 读.读编码(); const auto 任务 = 读.读编码();
    const auto 锚点 = 读.读编码(); const auto 类型 = 读.读编码(); const auto 关系 = 读.读编码();
    if (!所有者 || !任务 || !锚点 || !类型 || !关系
        || !读生命周期(读, 来源.任务节点生命周期)
        || !读生命周期(读, 来源.族归属关系生命周期)) return false;
    来源.任务所有者 = {*所有者}; 来源.任务节点 = *任务; 来源.任务族锚点 = *锚点;
    来源.任务族归属关系类型 = *类型; 来源.任务族归属关系 = *关系; return true;
}

bool 读方法生命周期(正式选择序列读取器& 读, L2方法生命周期事实& 生命周期) noexcept {
    const auto 方法 = 读.读编码(); const auto 版本 = 读.读();
    const auto 时间 = 读.读有符号(); const auto 来源 = 读.读编码();
    if (!方法 || !版本 || !时间 || !来源 || !读生命周期(读, 生命周期.生命周期)) return false;
    生命周期.方法 = L2方法身份{*方法}; 生命周期.生命周期版本 = {*版本};
    生命周期.发生时间 = *时间; 生命周期.来源稳定编码 = *来源; return true;
}

bool 读方法条件(正式选择序列读取器& 读, L2方法条件事实& 条件) noexcept {
    const auto 身份 = 读.读编码(); const auto 方法 = 读.读编码(); const auto 角色 = 读.读();
    const auto 类型 = 读.读编码(); const auto 有场景 = 读.读();
    if (!身份 || !方法 || !角色 || !类型 || !有场景 || *有场景 > 1) return false;
    std::optional<稳定编码> 场景; if (*有场景) { 场景 = 读.读编码(); if (!场景) return false; }
    const auto 特征 = 读.读编码(); const auto 合同 = 读.读编码();
    const auto 来源 = 读.读编码(); const auto 版本 = 读.读();
    if (!特征 || !合同 || !来源 || !版本 || !读生命周期(读, 条件.生命周期)) return false;
    条件.身份 = L2方法条件身份{*身份}; 条件.方法 = L2方法身份{*方法}; 条件.作用对象角色 = *角色;
    条件.目标对象类型 = L2概念身份{*类型};
    条件.场景 = 场景 ? std::optional<L2场景身份>{L2场景身份{*场景}} : std::nullopt;
    条件.特征定义 = L2特征定义身份{*特征}; 条件.条件状态合同 = L2目标状态合同身份{*合同};
    条件.来源稳定编码 = *来源; 条件.内容版本 = {*版本}; return true;
}

bool 读结果类型(正式选择序列读取器& 读, L2方法结果类型引用& 类型) noexcept {
    const auto 分支 = 读.读(); const auto 身份 = 读.读编码(); if (!分支 || !身份) return false;
    if (*分支 == 0) { 类型 = L2目标状态合同身份{*身份}; return true; }
    if (*分支 == 1) { 类型 = L2概念身份{*身份}; return true; }
    return false;
}

bool 读方法结果(正式选择序列读取器& 读, L2方法结果事实& 结果) noexcept {
    const auto 身份 = 读.读编码(); const auto 方法 = 读.读编码(); const auto 角色 = 读.读();
    const auto 类型 = 读.读编码(); const auto 特征 = 读.读编码(); const auto 方向 = 读.读();
    if (!身份 || !方法 || !角色 || !类型 || !特征 || !方向 || !读结果类型(读, 结果.结果类型)) return false;
    const auto 来源 = 读.读编码(); const auto 版本 = 读.读();
    if (!来源 || !版本 || !读生命周期(读, 结果.生命周期)) return false;
    结果.身份 = L2方法结果身份{*身份}; 结果.方法 = L2方法身份{*方法}; 结果.作用对象角色 = *角色;
    结果.目标对象类型 = L2概念身份{*类型}; 结果.特征定义 = L2特征定义身份{*特征};
    结果.变化方向 = static_cast<L2方法变化方向>(*方向); 结果.来源稳定编码 = *来源;
    结果.内容版本 = {*版本}; return true;
}

bool 读输入规格(正式选择序列读取器& 读, L2方法输入规格事实& 输入) {
    const auto 身份 = 读.读编码(); const auto 方法 = 读.读编码(); const auto 有条件 = 读.读();
    if (!身份 || !方法 || !有条件 || *有条件 > 1) return false;
    std::optional<稳定编码> 条件; if (*有条件) { 条件 = 读.读编码(); if (!条件) return false; }
    const auto 角色 = 读.读(); const auto 顺序 = 读.读();
    if (!角色 || !顺序 || !读规格(读, 输入.规格)) return false;
    const auto 来源 = 读.读编码(); const auto 版本 = 读.读();
    if (!来源 || !版本 || !读生命周期(读, 输入.生命周期)) return false;
    输入.身份 = L2方法输入规格身份{*身份}; 输入.方法 = L2方法身份{*方法};
    输入.条件 = 条件 ? std::optional<L2方法条件身份>{L2方法条件身份{*条件}} : std::nullopt;
    输入.作用对象角色 = *角色; 输入.顺序 = *顺序; 输入.来源稳定编码 = *来源; 输入.内容版本 = {*版本}; return true;
}

bool 读限制条件(正式选择序列读取器& 读, L2方法限制条件事实& 限制) {
    const auto 身份 = 读.读编码(); const auto 方法 = 读.读编码(); const auto 有条件 = 读.读();
    if (!身份 || !方法 || !有条件 || *有条件 > 1) return false;
    std::optional<稳定编码> 条件; if (*有条件) { 条件 = 读.读编码(); if (!条件) return false; }
    const auto 类别 = 读.读(); const auto 角色 = 读.读(); const auto 顺序 = 读.读();
    if (!类别 || !角色 || !顺序 || !读规格(读, 限制.规格)) return false;
    const auto 来源 = 读.读编码(); const auto 版本 = 读.读();
    if (!来源 || !版本 || !读生命周期(读, 限制.生命周期)) return false;
    限制.身份 = L2方法限制条件身份{*身份}; 限制.方法 = L2方法身份{*方法};
    限制.条件 = 条件 ? std::optional<L2方法条件身份>{L2方法条件身份{*条件}} : std::nullopt;
    限制.类别 = static_cast<L2方法限制条件类别>(*类别); 限制.作用对象角色 = *角色;
    限制.顺序 = *顺序; 限制.来源稳定编码 = *来源; 限制.内容版本 = {*版本}; return true;
}

bool 读动作入口(正式选择序列读取器& 读, L2方法动作入口事实& 动作) noexcept {
    const auto 身份 = 读.读编码(); const auto 方法 = 读.读编码(); const auto 入 = 读.读编码();
    const auto 出 = 读.读编码(); const auto 键 = 读.读(); const auto 版本 = 读.读(); const auto 来源 = 读.读编码();
    if (!身份 || !方法 || !入 || !出 || !键 || !版本 || !来源 || !读生命周期(读, 动作.生命周期)) return false;
    动作.身份 = L2方法动作入口身份{*身份}; 动作.方法 = L2方法身份{*方法};
    动作.输入场景 = L2场景身份{*入}; 动作.输出场景 = L2场景身份{*出};
    动作.稳定动作键 = {*键}; 动作.内容版本 = {*版本}; 动作.来源稳定编码 = *来源; return true;
}

bool 读动作冻结项_v2(
    正式选择序列读取器& 读,
    L2任务执行路径动作冻结项_v2& 动作) noexcept {
    const auto 身份 = 读.读编码(); const auto 方法 = 读.读编码();
    const auto 内容版本 = 读.读(); const auto 顺序 = 读.读();
    const auto 稳定键 = 读.读(); const auto 作用范围 = 读.读();
    const auto 主体 = 读.读编码(); const auto 有场景 = 读.读();
    if (!身份 || !方法 || !内容版本 || !顺序 || !稳定键
        || !作用范围 || !主体 || !有场景 || *有场景 > 1)
        return false;
    std::optional<L2场景身份> 场景;
    if (*有场景) {
        const auto 场景编码 = 读.读编码();
        if (!场景编码) return false;
        场景 = L2场景身份{*场景编码};
    }
    const auto 范围材料 = 读.读编码(); const auto 许可版本 = 读.读();
    const auto 预期 = 读.读编码(); const auto 验证 = 读.读编码();
    const auto 归因 = 读.读编码(); const auto 创建 = 读.读();
    const auto 有退出 = 读.读();
    if (!范围材料 || !许可版本 || !预期 || !验证 || !归因
        || !创建 || !有退出 || *有退出 > 1)
        return false;
    std::optional<std::uint64_t> 退出;
    if (*有退出) {
        退出 = 读.读();
        if (!退出) return false;
    }
    动作.动作入口 = L2方法动作入口身份{*身份};
    动作.方法 = L2方法身份{*方法};
    动作.方法内容版本 = {*内容版本};
    动作.顺序 = *顺序; 动作.稳定动作键 = {*稳定键};
    动作.作用范围 = static_cast<L2方法动作作用范围>(*作用范围);
    动作.作用主体约束 = L2存在身份{*主体}; 动作.场景约束 = 场景;
    动作.动作范围材料 = *范围材料;
    动作.技术许可合同版本 = static_cast<std::uint32_t>(*许可版本);
    动作.预期结果材料 = *预期; 动作.结果验证合同 = *验证;
    动作.动作归因合同 = *归因;
    动作.来源动作生命周期 = {*创建, 退出};
    return true;
}

bool 读场景成员(正式选择序列读取器& 读, L2存在场景成员引用事实& 成员) noexcept {
    const auto 关系 = 读.读编码(); const auto 场景 = 读.读编码(); const auto 存在 = 读.读编码(); const auto 顺序 = 读.读();
    if (!关系 || !场景 || !存在 || !顺序 || !读生命周期(读, 成员.生命周期)) return false;
    成员.关系稳定编码 = *关系; 成员.场景 = L2场景身份{*场景}; 成员.存在 = L2存在身份{*存在}; 成员.顺序 = *顺序; return true;
}

bool 读特征值(正式选择序列读取器& 读, L2特征值事实& 特征值) {
    const auto 值 = 读.读编码(); const auto 实例 = 读.读编码();
    if (!值 || !实例 || !读原始值(读, 特征值.类型化不可变材料)) return false;
    const auto 来源 = 读.读编码(); if (!来源 || !读生命周期(读, 特征值.生命周期)) return false;
    特征值.值稳定编码 = *值; 特征值.特征实例 = L2特征实例身份{*实例}; 特征值.来源稳定编码 = *来源; return true;
}

bool 读状态(正式选择序列读取器& 读, L2状态事实& 状态) {
    const auto 身份 = 读.读编码(); const auto 主体 = 读.读编码(); const auto 实例 = 读.读编码();
    if (!身份 || !主体 || !实例 || !读属性(读, 状态.精确值) || !读属性(读, 状态.时间值)) return false;
    const auto 来源 = 读.读编码(); if (!来源 || !读生命周期(读, 状态.生命周期)) return false;
    状态.身份 = L2状态身份{*身份}; 状态.主体存在 = L2存在身份{*主体};
    状态.特征实例 = L2特征实例身份{*实例}; 状态.来源稳定编码 = *来源; return true;
}

std::vector<std::uint64_t> 编码正式选择完整请求(
    const L2发布任务筹办正式选择请求& 请求) {
    正式选择序列写入器 写;
    写.写(2); 写.写(请求.请求头.合同版本); 写.写(请求.请求头.期望事实代次);
    写.写(请求.来源材料.index());
    std::visit([&](const auto& 来源) {
        using 来源类型 = std::decay_t<decltype(来源)>;
        写.写(来源.合同版本);
        if constexpr (std::is_same_v<来源类型,
                L2任务初次筹办规范化来源材料>) {
            写.写编码(来源.首次准备记录.值);
            写.写编码(来源.任务.值); 写.写编码(来源.来源需求.值);
            写.写编码(来源.需求列表项.值);
            写.写(来源.首次目标比较共同事实截止G0);
            写.写(来源.任务建立事实代次G1); 写.写(来源.筹办轮次);
            写.写(来源.运行代次);
        } else {
            写.写(来源.原重筹办意图幂等身份.值);
            写.写编码(来源.任务.值); 写.写编码(来源.旧路径.值);
            写.写编码(来源.旧实例.值); 写.写(来源.旧执行轮次);
            写.写编码(来源.旧实际结果.值);
            写.写编码(来源.正式目标裁决证据.值);
            写.写编码(来源.正式待重筹办状态.值);
            写.写(来源.原共同事实截止); 写.写(来源.迁移后事实代次);
            写.写(来源.旧筹办轮次); 写.写(来源.新筹办轮次);
            写.写(来源.运行代次);
        }
    }, 请求.来源材料);
    写.写任务身份来源(请求.任务身份来源);
    写.写方法生命周期(请求.方法生命周期);
    写.写(请求.选择规则版本); 写.写(请求.召回规则版本);
    写.写(请求.来源需求组.size());
    for (const auto& 需求 : 请求.来源需求组) 写.写编码(需求.值);
    写.写(请求.稳定证据组.size());
    for (const auto& 证据 : 请求.稳定证据组) {
        写.写(static_cast<std::uint64_t>(证据.类别)); 写.写编码(证据.身份);
    }
    const auto& 冻结 = 请求.冻结材料;
    写.写编码(冻结.任务.值); 写.写(冻结.筹办轮次);
    写.写编码(冻结.方法.值); 写.写(冻结.方法内容版本.值);
    写.写(冻结.条件证据组.size());
    for (const auto& 项 : 冻结.条件证据组) {
        写.写方法条件(项.条件); 写.写编码(项.绑定目标.值);
        写.写(项.场景.has_value()); if (项.场景) 写.写编码(项.场景->值);
        写.写(项.场景成员.has_value()); if (项.场景成员) 写.写场景成员(*项.场景成员);
        写.写状态(项.当前状态); 写.写(static_cast<std::uint64_t>(项.具名关系));
        写.写(项.允许关系位);
    }
    写.写(冻结.输入冻结组.size());
    for (const auto& 项 : 冻结.输入冻结组) { 写.写输入规格(项.输入规格); 写.写编码(项.绑定目标.值); }
    写.写(冻结.参数冻结组.size());
    for (const auto& 项 : 冻结.参数冻结组) {
        写.写编码(项.来源输入规格.值); 写.写(项.作用对象角色);
        写.写规格(项.参数规格); 写.写编码(项.绑定目标.值);
    }
    写.写(冻结.限制证据组.size());
    for (const auto& 项 : 冻结.限制证据组) {
        写.写限制条件(项.限制条件); 写.写编码(项.绑定目标.值);
        写.写编码(项.解释器.值); 写.写(项.解释规则版本);
        写.写(项.当前事实证据.index());
        std::visit([&](const auto& 事实) {
            using 类型 = std::decay_t<decltype(事实)>;
            if constexpr (std::is_same_v<类型, L2状态事实>) 写.写状态(事实);
            else if constexpr (std::is_same_v<类型, L2存在场景成员引用事实>) 写.写场景成员(事实);
            else if constexpr (std::is_same_v<类型, L2特征值事实>) 写.写特征值(事实);
            else 写.写编码(事实.值);
        }, 项.当前事实证据);
        写.写(项.已满足);
    }
    写.写动作入口(冻结.动作入口); 写.写方法结果(冻结.主轴结果);
    写.写编码(冻结.验证合同.值); 写.写(冻结.归因证据.index());
    std::visit([&](const auto& 归因) { 写.写编码(归因.值); }, 冻结.归因证据);
    写.写(冻结.失效事实截止);
    return std::move(写.值);
}

std::optional<L2发布任务筹办正式选择请求> 解码正式选择完整请求(
    const std::vector<std::uint64_t>& 值) {
    正式选择序列读取器 读{值};
    L2发布任务筹办正式选择请求 请求;
    const auto 格式 = 读.读(); const auto 合同 = 读.读(); const auto 期望 = 读.读();
    if (!格式 || *格式 != 2 || !合同 || !期望) return std::nullopt;
    请求.请求头 = {static_cast<std::uint32_t>(*合同), *期望};
    const auto 来源分支 = 读.读(); const auto 来源合同 = 读.读();
    if (!来源分支 || !来源合同) return std::nullopt;
    if (*来源分支 == 0) {
        const auto 记录 = 读.读编码(); const auto 任务 = 读.读编码();
        const auto 需求 = 读.读编码(); const auto 列表项 = 读.读编码();
        const auto G0 = 读.读(); const auto G1 = 读.读();
        const auto 轮次 = 读.读(); const auto 运行 = 读.读();
        if (!记录 || !任务 || !需求 || !列表项 || !G0 || !G1 || !轮次 || !运行)
            return std::nullopt;
        请求.来源材料 = L2任务初次筹办规范化来源材料{
            static_cast<std::uint32_t>(*来源合同),
            L2任务初次筹办准备记录身份{*记录}, L2任务身份{*任务},
            L2需求身份{*需求}, L2需求列表项身份{*列表项},
            *G0, *G1, *轮次, *运行};
    } else if (*来源分支 == 1) {
        const auto 幂等 = 读.读(); const auto 任务 = 读.读编码();
        const auto 旧路径 = 读.读编码(); const auto 旧实例 = 读.读编码();
        const auto 旧执行 = 读.读(); const auto 旧结果 = 读.读编码();
        const auto 裁决 = 读.读编码(); const auto 状态 = 读.读编码();
        const auto 原截止 = 读.读(); const auto 迁移 = 读.读();
        const auto 旧筹办 = 读.读(); const auto 新筹办 = 读.读();
        const auto 运行 = 读.读();
        if (!幂等 || !任务 || !旧路径 || !旧实例 || !旧执行 || !旧结果
            || !裁决 || !状态 || !原截止 || !迁移 || !旧筹办 || !新筹办
            || !运行) return std::nullopt;
        请求.来源材料 = L2任务重筹办规范化来源材料{
            static_cast<std::uint32_t>(*来源合同),
            L2任务重筹办意图幂等身份{*幂等}, L2任务身份{*任务},
            L2任务方法路径身份{*旧路径}, L2实例方法身份{*旧实例},
            *旧执行, L2任务实际结果身份{*旧结果},
            L2任务目标裁决证据身份{*裁决},
            L2任务治理状态身份{*状态}, *原截止, *迁移,
            *旧筹办, *新筹办, *运行};
    } else return std::nullopt;
    if (!读任务身份来源(读, 请求.任务身份来源)
        || !读方法生命周期(读, 请求.方法生命周期)) return std::nullopt;
    const auto 选择规则 = 读.读(); const auto 召回规则 = 读.读();
    const auto 来源数 = 读.读(); if (!选择规则 || !召回规则 || !来源数 || *来源数 > 1'000'000) return std::nullopt;
    请求.选择规则版本 = static_cast<std::uint32_t>(*选择规则);
    请求.召回规则版本 = static_cast<std::uint32_t>(*召回规则);
    请求.来源需求组.reserve(static_cast<std::size_t>(*来源数));
    for (std::uint64_t i = 0; i < *来源数; ++i) { const auto 身份 = 读.读编码(); if (!身份) return std::nullopt; 请求.来源需求组.emplace_back(*身份); }
    const auto 证据数 = 读.读(); if (!证据数 || *证据数 > 1'000'000) return std::nullopt;
    请求.稳定证据组.reserve(static_cast<std::size_t>(*证据数));
    for (std::uint64_t i = 0; i < *证据数; ++i) {
        const auto 类别 = 读.读(); const auto 身份 = 读.读编码(); if (!类别 || !身份) return std::nullopt;
        请求.稳定证据组.push_back({static_cast<L2任务选择稳定证据类别>(*类别), *身份});
    }
    auto& 冻结 = 请求.冻结材料;
    const auto 冻结任务 = 读.读编码(); const auto 筹办 = 读.读(); const auto 方法 = 读.读编码(); const auto 内容 = 读.读();
    if (!冻结任务 || !筹办 || !方法 || !内容) return std::nullopt;
    冻结.任务 = L2任务身份{*冻结任务}; 冻结.筹办轮次 = *筹办;
    冻结.方法 = L2方法身份{*方法}; 冻结.方法内容版本 = {*内容};
    const auto 条件数 = 读.读(); if (!条件数 || *条件数 > 1'000'000) return std::nullopt;
    冻结.条件证据组.reserve(static_cast<std::size_t>(*条件数));
    for (std::uint64_t i = 0; i < *条件数; ++i) {
        L2任务条件当前就绪冻结证据 项;
        if (!读方法条件(读, 项.条件)) return std::nullopt;
        const auto 绑定 = 读.读编码(); const auto 有场景 = 读.读();
        if (!绑定 || !有场景 || *有场景 > 1) return std::nullopt;
        项.绑定目标 = L2存在身份{*绑定};
        if (*有场景) { const auto 场景 = 读.读编码(); if (!场景) return std::nullopt; 项.场景 = L2场景身份{*场景}; }
        const auto 有成员 = 读.读(); if (!有成员 || *有成员 > 1) return std::nullopt;
        if (*有成员) { L2存在场景成员引用事实 成员; if (!读场景成员(读, 成员)) return std::nullopt; 项.场景成员 = std::move(成员); }
        if (!读状态(读, 项.当前状态)) return std::nullopt;
        const auto 关系 = 读.读(); const auto 位 = 读.读(); if (!关系 || !位) return std::nullopt;
        项.具名关系 = static_cast<L2特征比较具名关系>(*关系); 项.允许关系位 = static_cast<std::uint8_t>(*位);
        冻结.条件证据组.push_back(std::move(项));
    }
    const auto 输入数 = 读.读(); if (!输入数 || *输入数 > 1'000'000) return std::nullopt;
    for (std::uint64_t i = 0; i < *输入数; ++i) { L2任务输入规格冻结项 项; if (!读输入规格(读, 项.输入规格)) return std::nullopt; const auto 绑定 = 读.读编码(); if (!绑定) return std::nullopt; 项.绑定目标 = L2存在身份{*绑定}; 冻结.输入冻结组.push_back(std::move(项)); }
    const auto 参数数 = 读.读(); if (!参数数 || *参数数 > 1'000'000) return std::nullopt;
    for (std::uint64_t i = 0; i < *参数数; ++i) { L2任务参数绑定冻结项 项; const auto 来源输入 = 读.读编码(); const auto 角色 = 读.读(); if (!来源输入 || !角色 || !读规格(读, 项.参数规格)) return std::nullopt; const auto 绑定 = 读.读编码(); if (!绑定) return std::nullopt; 项.来源输入规格 = L2方法输入规格身份{*来源输入}; 项.作用对象角色 = *角色; 项.绑定目标 = L2存在身份{*绑定}; 冻结.参数冻结组.push_back(std::move(项)); }
    const auto 限制数 = 读.读(); if (!限制数 || *限制数 > 1'000'000) return std::nullopt;
    for (std::uint64_t i = 0; i < *限制数; ++i) {
        L2任务限制条件复判冻结证据 项; if (!读限制条件(读, 项.限制条件)) return std::nullopt;
        const auto 绑定 = 读.读编码(); const auto 解释器 = 读.读编码(); const auto 规则 = 读.读(); const auto 分支 = 读.读();
        if (!绑定 || !解释器 || !规则 || !分支) return std::nullopt;
        项.绑定目标 = L2存在身份{*绑定}; 项.解释器 = L2方法限制解释器身份{*解释器}; 项.解释规则版本 = static_cast<std::uint32_t>(*规则);
        if (*分支 == 0) { L2状态事实 事实; if (!读状态(读, 事实)) return std::nullopt; 项.当前事实证据 = std::move(事实); }
        else if (*分支 == 1) { L2存在场景成员引用事实 事实; if (!读场景成员(读, 事实)) return std::nullopt; 项.当前事实证据 = std::move(事实); }
        else if (*分支 == 2) { L2特征值事实 事实; if (!读特征值(读, 事实)) return std::nullopt; 项.当前事实证据 = std::move(事实); }
        else if (*分支 == 3) { const auto 身份 = 读.读编码(); if (!身份) return std::nullopt; 项.当前事实证据 = L2因果身份{*身份}; }
        else return std::nullopt;
        const auto 满足 = 读.读(); if (!满足 || *满足 > 1) return std::nullopt; 项.已满足 = *满足 != 0;
        冻结.限制证据组.push_back(std::move(项));
    }
    if (!读动作入口(读, 冻结.动作入口) || !读方法结果(读, 冻结.主轴结果)) return std::nullopt;
    const auto 验证 = 读.读编码(); const auto 归因分支 = 读.读();
    const auto 归因 = 读.读编码(); const auto 失效 = 读.读();
    if (!验证 || !归因分支 || !归因 || !失效 || !读.结束()) return std::nullopt;
    冻结.验证合同 = L2目标状态合同身份{*验证};
    if (*归因分支 == 0)
        冻结.归因证据 = L2任务初次筹办准备记录身份{*归因};
    else if (*归因分支 == 1)
        冻结.归因证据 = L2任务目标裁决证据身份{*归因};
    else return std::nullopt;
    冻结.失效事实截止 = *失效;
    return L2发布任务筹办正式选择请求有效(请求)
        ? std::optional<L2发布任务筹办正式选择请求>{std::move(请求)}
        : std::nullopt;
}

std::vector<std::uint64_t> 编码正式选择完整请求_v2(
    const L2发布任务筹办正式选择请求_v2& 请求) {
    正式选择序列写入器 写;
    写.写(L2任务筹办正式选择请求序列格式版本_v2);
    写.写(请求.合同版本); 写.写(请求.请求头.合同版本);
    写.写(请求.请求头.期望事实代次);
    const auto& 基础 = 请求.基础材料;
    写.写(基础.来源材料.index());
    std::visit([&](const auto& 来源) {
        using 来源类型 = std::decay_t<decltype(来源)>;
        写.写(来源.合同版本);
        if constexpr (std::is_same_v<来源类型,
                L2任务初次筹办规范化来源材料>) {
            写.写编码(来源.首次准备记录.值); 写.写编码(来源.任务.值);
            写.写编码(来源.来源需求.值); 写.写编码(来源.需求列表项.值);
            写.写(来源.首次目标比较共同事实截止G0);
            写.写(来源.任务建立事实代次G1); 写.写(来源.筹办轮次);
            写.写(来源.运行代次);
        } else {
            写.写(来源.原重筹办意图幂等身份.值); 写.写编码(来源.任务.值);
            写.写编码(来源.旧路径.值); 写.写编码(来源.旧实例.值);
            写.写(来源.旧执行轮次); 写.写编码(来源.旧实际结果.值);
            写.写编码(来源.正式目标裁决证据.值);
            写.写编码(来源.正式待重筹办状态.值);
            写.写(来源.原共同事实截止); 写.写(来源.迁移后事实代次);
            写.写(来源.旧筹办轮次); 写.写(来源.新筹办轮次);
            写.写(来源.运行代次);
        }
    }, 基础.来源材料);
    写.写任务身份来源(基础.任务身份来源);
    写.写方法生命周期(基础.方法生命周期);
    写.写(基础.选择规则版本); 写.写(基础.召回规则版本);
    写.写(基础.来源需求组.size());
    for (const auto& 需求 : 基础.来源需求组) 写.写编码(需求.值);
    写.写(基础.稳定证据组.size());
    for (const auto& 证据 : 基础.稳定证据组) {
        写.写(static_cast<std::uint64_t>(证据.类别)); 写.写编码(证据.身份);
    }
    const auto& 冻结 = 请求.冻结材料;
    写.写(冻结.合同版本); 写.写编码(冻结.任务.值);
    写.写(冻结.筹办轮次); 写.写编码(冻结.方法.值);
    写.写(冻结.方法内容版本.值);
    写.写(冻结.条件证据组.size());
    for (const auto& 项 : 冻结.条件证据组) {
        写.写方法条件(项.条件); 写.写编码(项.绑定目标.值);
        写.写(项.场景.has_value()); if (项.场景) 写.写编码(项.场景->值);
        写.写(项.场景成员.has_value());
        if (项.场景成员) 写.写场景成员(*项.场景成员);
        写.写状态(项.当前状态); 写.写(static_cast<std::uint64_t>(项.具名关系));
        写.写(项.允许关系位);
    }
    写.写(冻结.输入冻结组.size());
    for (const auto& 项 : 冻结.输入冻结组) {
        写.写输入规格(项.输入规格); 写.写编码(项.绑定目标.值);
    }
    写.写(冻结.参数冻结组.size());
    for (const auto& 项 : 冻结.参数冻结组) {
        写.写编码(项.来源输入规格.值); 写.写(项.作用对象角色);
        写.写规格(项.参数规格); 写.写编码(项.绑定目标.值);
    }
    写.写(冻结.限制证据组.size());
    for (const auto& 项 : 冻结.限制证据组) {
        写.写限制条件(项.限制条件); 写.写编码(项.绑定目标.值);
        写.写编码(项.解释器.值); 写.写(项.解释规则版本);
        写.写(项.当前事实证据.index());
        std::visit([&](const auto& 事实) {
            using 类型 = std::decay_t<decltype(事实)>;
            if constexpr (std::is_same_v<类型, L2状态事实>) 写.写状态(事实);
            else if constexpr (std::is_same_v<类型, L2存在场景成员引用事实>) 写.写场景成员(事实);
            else if constexpr (std::is_same_v<类型, L2特征值事实>) 写.写特征值(事实);
            else 写.写编码(事实.值);
        }, 项.当前事实证据);
        写.写(项.已满足);
    }
    写.写(L2任务路径动作组序列格式版本_v2);
    写.写(冻结.完整有序动作组.size());
    for (const auto& 动作 : 冻结.完整有序动作组) 写.写动作冻结项_v2(动作);
    写.写方法结果(冻结.主轴结果); 写.写编码(冻结.验证合同.值);
    写.写(冻结.归因证据.index());
    std::visit([&](const auto& 归因) { 写.写编码(归因.值); }, 冻结.归因证据);
    写.写(冻结.失效事实截止);
    return std::move(写.值);
}

std::optional<L2发布任务筹办正式选择请求_v2>
解码正式选择完整请求_v2(const std::vector<std::uint64_t>& 值) {
    正式选择序列读取器 读{值};
    L2发布任务筹办正式选择请求_v2 请求;
    const auto 格式 = 读.读(); const auto 专属 = 读.读();
    const auto 共同 = 读.读(); const auto G0 = 读.读();
    if (!格式 || *格式 != L2任务筹办正式选择请求序列格式版本_v2
        || !专属 || *专属 != L2任务筹办正式选择合同版本_v2
        || !共同 || !G0) return std::nullopt;
    请求.合同版本 = static_cast<std::uint32_t>(*专属);
    请求.请求头 = {static_cast<std::uint32_t>(*共同), *G0};
    请求.基础材料.合同版本 = L2任务筹办正式选择合同版本_v2;
    const auto 来源分支 = 读.读(); const auto 来源合同 = 读.读();
    if (!来源分支 || !来源合同) return std::nullopt;
    if (*来源分支 == 0) {
        const auto 记录 = 读.读编码(); const auto 任务 = 读.读编码();
        const auto 需求 = 读.读编码(); const auto 列表项 = 读.读编码();
        const auto 来源G0 = 读.读(); const auto G1 = 读.读();
        const auto 轮次 = 读.读(); const auto 运行 = 读.读();
        if (!记录 || !任务 || !需求 || !列表项 || !来源G0 || !G1 || !轮次 || !运行)
            return std::nullopt;
        请求.基础材料.来源材料 = L2任务初次筹办规范化来源材料{
            static_cast<std::uint32_t>(*来源合同),
            L2任务初次筹办准备记录身份{*记录}, L2任务身份{*任务},
            L2需求身份{*需求}, L2需求列表项身份{*列表项},
            *来源G0, *G1, *轮次, *运行};
    } else if (*来源分支 == 1) {
        const auto 幂等 = 读.读(); const auto 任务 = 读.读编码();
        const auto 旧路径 = 读.读编码(); const auto 旧实例 = 读.读编码();
        const auto 旧执行 = 读.读(); const auto 旧结果 = 读.读编码();
        const auto 裁决 = 读.读编码(); const auto 状态 = 读.读编码();
        const auto 原截止 = 读.读(); const auto 迁移 = 读.读();
        const auto 旧筹办 = 读.读(); const auto 新筹办 = 读.读();
        const auto 运行 = 读.读();
        if (!幂等 || !任务 || !旧路径 || !旧实例 || !旧执行 || !旧结果
            || !裁决 || !状态 || !原截止 || !迁移 || !旧筹办 || !新筹办 || !运行)
            return std::nullopt;
        请求.基础材料.来源材料 = L2任务重筹办规范化来源材料{
            static_cast<std::uint32_t>(*来源合同),
            L2任务重筹办意图幂等身份{*幂等}, L2任务身份{*任务},
            L2任务方法路径身份{*旧路径}, L2实例方法身份{*旧实例},
            *旧执行, L2任务实际结果身份{*旧结果},
            L2任务目标裁决证据身份{*裁决}, L2任务治理状态身份{*状态},
            *原截止, *迁移, *旧筹办, *新筹办, *运行};
    } else return std::nullopt;
    auto& 基础 = 请求.基础材料;
    if (!读任务身份来源(读, 基础.任务身份来源)
        || !读方法生命周期(读, 基础.方法生命周期)) return std::nullopt;
    const auto 选择规则 = 读.读(); const auto 召回规则 = 读.读();
    const auto 来源数 = 读.读();
    if (!选择规则 || !召回规则 || !来源数 || *来源数 > 1024) return std::nullopt;
    基础.选择规则版本 = static_cast<std::uint32_t>(*选择规则);
    基础.召回规则版本 = static_cast<std::uint32_t>(*召回规则);
    for (std::uint64_t i = 0; i < *来源数; ++i) {
        const auto 身份 = 读.读编码(); if (!身份) return std::nullopt;
        基础.来源需求组.emplace_back(*身份);
    }
    const auto 证据数 = 读.读();
    if (!证据数 || *证据数 > 1024) return std::nullopt;
    for (std::uint64_t i = 0; i < *证据数; ++i) {
        const auto 类别 = 读.读(); const auto 身份 = 读.读编码();
        if (!类别 || !身份) return std::nullopt;
        基础.稳定证据组.push_back({static_cast<L2任务选择稳定证据类别>(*类别), *身份});
    }
    auto& 冻结 = 请求.冻结材料;
    const auto 冻结合同 = 读.读(); const auto 冻结任务 = 读.读编码();
    const auto 筹办 = 读.读(); const auto 方法 = 读.读编码();
    const auto 内容 = 读.读();
    if (!冻结合同 || !冻结任务 || !筹办 || !方法 || !内容) return std::nullopt;
    冻结.合同版本 = static_cast<std::uint32_t>(*冻结合同);
    冻结.任务 = L2任务身份{*冻结任务}; 冻结.筹办轮次 = *筹办;
    冻结.方法 = L2方法身份{*方法}; 冻结.方法内容版本 = {*内容};
    const auto 条件数 = 读.读(); if (!条件数 || *条件数 > 1'000'000) return std::nullopt;
    for (std::uint64_t i = 0; i < *条件数; ++i) {
        L2任务条件当前就绪冻结证据 项;
        if (!读方法条件(读, 项.条件)) return std::nullopt;
        const auto 绑定 = 读.读编码(); const auto 有场景 = 读.读();
        if (!绑定 || !有场景 || *有场景 > 1) return std::nullopt;
        项.绑定目标 = L2存在身份{*绑定};
        if (*有场景) { const auto 场景 = 读.读编码(); if (!场景) return std::nullopt; 项.场景 = L2场景身份{*场景}; }
        const auto 有成员 = 读.读(); if (!有成员 || *有成员 > 1) return std::nullopt;
        if (*有成员) { L2存在场景成员引用事实 成员; if (!读场景成员(读, 成员)) return std::nullopt; 项.场景成员 = std::move(成员); }
        if (!读状态(读, 项.当前状态)) return std::nullopt;
        const auto 关系 = 读.读(); const auto 位 = 读.读();
        if (!关系 || !位) return std::nullopt;
        项.具名关系 = static_cast<L2特征比较具名关系>(*关系);
        项.允许关系位 = static_cast<std::uint8_t>(*位);
        冻结.条件证据组.push_back(std::move(项));
    }
    const auto 输入数 = 读.读(); if (!输入数 || *输入数 > 1'000'000) return std::nullopt;
    for (std::uint64_t i = 0; i < *输入数; ++i) { L2任务输入规格冻结项 项; if (!读输入规格(读, 项.输入规格)) return std::nullopt; const auto 绑定 = 读.读编码(); if (!绑定) return std::nullopt; 项.绑定目标 = L2存在身份{*绑定}; 冻结.输入冻结组.push_back(std::move(项)); }
    const auto 参数数 = 读.读(); if (!参数数 || *参数数 > 1'000'000) return std::nullopt;
    for (std::uint64_t i = 0; i < *参数数; ++i) { L2任务参数绑定冻结项 项; const auto 来源输入 = 读.读编码(); const auto 角色 = 读.读(); if (!来源输入 || !角色 || !读规格(读, 项.参数规格)) return std::nullopt; const auto 绑定 = 读.读编码(); if (!绑定) return std::nullopt; 项.来源输入规格 = L2方法输入规格身份{*来源输入}; 项.作用对象角色 = *角色; 项.绑定目标 = L2存在身份{*绑定}; 冻结.参数冻结组.push_back(std::move(项)); }
    const auto 限制数 = 读.读(); if (!限制数 || *限制数 > 1'000'000) return std::nullopt;
    for (std::uint64_t i = 0; i < *限制数; ++i) {
        L2任务限制条件复判冻结证据 项; if (!读限制条件(读, 项.限制条件)) return std::nullopt;
        const auto 绑定 = 读.读编码(); const auto 解释器 = 读.读编码(); const auto 规则 = 读.读(); const auto 分支 = 读.读();
        if (!绑定 || !解释器 || !规则 || !分支) return std::nullopt;
        项.绑定目标 = L2存在身份{*绑定}; 项.解释器 = L2方法限制解释器身份{*解释器}; 项.解释规则版本 = static_cast<std::uint32_t>(*规则);
        if (*分支 == 0) { L2状态事实 事实; if (!读状态(读, 事实)) return std::nullopt; 项.当前事实证据 = std::move(事实); }
        else if (*分支 == 1) { L2存在场景成员引用事实 事实; if (!读场景成员(读, 事实)) return std::nullopt; 项.当前事实证据 = std::move(事实); }
        else if (*分支 == 2) { L2特征值事实 事实; if (!读特征值(读, 事实)) return std::nullopt; 项.当前事实证据 = std::move(事实); }
        else if (*分支 == 3) { const auto 身份 = 读.读编码(); if (!身份) return std::nullopt; 项.当前事实证据 = L2因果身份{*身份}; }
        else return std::nullopt;
        const auto 满足 = 读.读(); if (!满足 || *满足 > 1) return std::nullopt;
        项.已满足 = *满足 != 0; 冻结.限制证据组.push_back(std::move(项));
    }
    const auto 动作格式 = 读.读(); const auto 动作数 = 读.读();
    if (!动作格式 || *动作格式 != L2任务路径动作组序列格式版本_v2
        || !动作数 || *动作数 == 0 || *动作数 > 1'000'000) return std::nullopt;
    for (std::uint64_t i = 0; i < *动作数; ++i) {
        L2任务执行路径动作冻结项_v2 动作;
        if (!读动作冻结项_v2(读, 动作)) return std::nullopt;
        冻结.完整有序动作组.push_back(std::move(动作));
    }
    if (!读方法结果(读, 冻结.主轴结果)) return std::nullopt;
    const auto 验证 = 读.读编码(); const auto 归因分支 = 读.读();
    const auto 归因 = 读.读编码(); const auto 失效 = 读.读();
    if (!验证 || !归因分支 || !归因 || !失效 || !读.结束()) return std::nullopt;
    冻结.验证合同 = L2目标状态合同身份{*验证};
    if (*归因分支 == 0) 冻结.归因证据 = L2任务初次筹办准备记录身份{*归因};
    else if (*归因分支 == 1) 冻结.归因证据 = L2任务目标裁决证据身份{*归因};
    else return std::nullopt;
    冻结.失效事实截止 = *失效;
    请求.方法动作组正式读回.状态 = L2方法完整有序动作组读取状态::已读取;
    请求.方法动作组正式读回.本次正式读回截止 = *G0;
    for (const auto& 动作 : 冻结.完整有序动作组) {
        请求.方法动作组正式读回.完整有序动作入口组.push_back({
            动作.动作入口, 动作.方法, 动作.方法内容版本, 动作.顺序,
            动作.稳定动作键, 动作.作用范围, 动作.作用主体约束,
            动作.场景约束, 动作.动作范围材料, 动作.技术许可合同版本,
            动作.预期结果材料, 动作.结果验证合同, 动作.动作归因合同,
            动作.来源动作生命周期});
    }
    return L2发布任务筹办正式选择请求_v2有效(请求)
        ? std::optional<L2发布任务筹办正式选择请求_v2>{std::move(请求)}
        : std::nullopt;
}

struct 任务筹办来源首次账定位结果_v2 final {
    L2任务筹办正式选择读取状态_v2 状态 =
        L2任务筹办正式选择读取状态_v2::内部错误;
    L1所有者范围写入幂等身份 写入幂等身份;
    std::optional<L2发布任务筹办正式选择请求_v2> 首次请求;
    std::optional<L1所有者范围写集请求> 首次规范化写集;
    std::optional<L1所有者范围写入结果> 首次写入结果;
};

std::uint64_t 映射任务筹办来源首次账起始值(
    const L2任务筹办来源幂等身份& 来源) noexcept;

任务筹办来源首次账定位结果_v2 定位任务筹办来源首次账_v2(
    const L1所有者范围写端口& 写入端口,
    const L2任务筹办来源幂等身份& 来源) {
    constexpr std::uint64_t 探测步长 = 0x9E37'79B9'7F4A'7C15ULL;
    auto 候选 = 映射任务筹办来源首次账起始值(来源);
    if (候选 == 0) 候选 = 探测步长;
    for (std::size_t 次数 = 0; 次数 < 4096; ++次数) {
        const L1所有者范围写入幂等身份 幂等身份{候选};
        const auto 首次 = 写入端口.读取首次写入材料({
            L1所有者范围首次写入读取合同版本, 幂等身份});
        if (首次.状态 == L1所有者范围读取状态::未找到)
            return {L2任务筹办正式选择读取状态_v2::未找到, 幂等身份};
        if (首次.状态 != L1所有者范围读取状态::成功
            || !首次.首次规范化写集 || !首次.首次写入结果)
            return {};
        const std::vector<std::uint64_t>* 完整值 = nullptr;
        for (const auto& 值项 : 首次.首次规范化写集->值)
            if (值项.本地键.值 == 正式选择完整请求值写入本地键值) {
                完整值 = std::get_if<std::vector<std::uint64_t>>(&值项.材料);
                break;
            }
        const auto 首次请求 = 完整值
            ? 解码正式选择完整请求_v2(*完整值) : std::nullopt;
        if (首次请求 && L2取得任务筹办来源幂等身份(
                首次请求->基础材料.来源材料) == 来源)
            return {L2任务筹办正式选择读取状态_v2::已读取,
                幂等身份, 首次请求, *首次.首次规范化写集,
                *首次.首次写入结果};
        候选 += 探测步长;
        if (候选 == 0) 候选 += 探测步长;
    }
    return {};
}

struct 任务筹办来源首次账定位结果 final {
    L2结构状态 状态 = L2结构状态::内部不一致;
    L1所有者范围写入幂等身份 写入幂等身份;
    std::optional<L2发布任务筹办正式选择请求> 首次请求;
    std::optional<L1所有者范围写集请求> 首次规范化写集;
    std::optional<L1所有者范围写入结果> 首次写入结果;
};

std::uint64_t 映射任务筹办来源首次账起始值(
    const L2任务筹办来源幂等身份& 来源) noexcept {
    return std::visit([](const auto& 身份) noexcept {
        using 身份类型 = std::decay_t<decltype(身份)>;
        if constexpr (std::is_same_v<身份类型,
                L2任务初次筹办准备记录身份>)
            return 身份.值.值 ^ 0xA3C5'9AC3'5A3C'C35AULL;
        else
            return 身份.值 ^ 0x5C3A'653C'A5C3'3CA5ULL;
    }, 来源);
}

任务筹办来源首次账定位结果 定位任务筹办来源首次账(
    const L1所有者范围写端口& 写入端口,
    const L2任务筹办来源幂等身份& 来源) {
    constexpr std::uint64_t 探测步长 = 0x9E37'79B9'7F4A'7C15ULL;
    auto 候选 = 映射任务筹办来源首次账起始值(来源);
    if (候选 == 0) 候选 = 探测步长;
    for (std::size_t 次数 = 0; 次数 < 4096; ++次数) {
        const L1所有者范围写入幂等身份 幂等身份{候选};
        const auto 首次 = 写入端口.读取首次写入材料({
            L1所有者范围首次写入读取合同版本, 幂等身份});
        if (首次.状态 == L1所有者范围读取状态::未找到)
            return {L2结构状态::未找到, 幂等身份,
                std::nullopt, std::nullopt, std::nullopt};
        if (首次.状态 != L1所有者范围读取状态::成功
            || !首次.首次规范化写集 || !首次.首次写入结果)
            return {L2结构状态::内部不一致, 幂等身份,
                std::nullopt, std::nullopt, std::nullopt};
        const std::vector<std::uint64_t>* 完整值 = nullptr;
        for (const auto& 值项 : 首次.首次规范化写集->值) {
            if (值项.本地键.值
                == 正式选择完整请求值写入本地键值) {
                完整值 = std::get_if<std::vector<std::uint64_t>>(&值项.材料);
                break;
            }
        }
        const auto 首次请求 = 完整值
            ? 解码正式选择完整请求(*完整值) : std::nullopt;
        if (首次请求
            && L2取得任务筹办来源幂等身份(首次请求->来源材料) == 来源)
            return {L2结构状态::已读取, 幂等身份, 首次请求,
                *首次.首次规范化写集, *首次.首次写入结果};
        候选 += 探测步长;
        if (候选 == 0) 候选 += 探测步长;
    }
    return {L2结构状态::数量预算不足, {},
        std::nullopt, std::nullopt, std::nullopt};
}

任务正式选择登记定位 初始化任务正式选择登记(
    const L1事实基座服务& 第一层服务,
    L1所有者范围写端口& 写入端口) {
    const auto 键 = [](std::uint32_t 值) noexcept {
        return L1所有者范围写集本地键{值};
    };
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 读取固定登记期望代次(
        第一层服务, 写入端口, 任务正式选择登记幂等身份);
    写集.写入幂等身份 = 任务正式选择登记幂等身份;
    写集.节点 = {
        {键(选择记录锚点本地键值), 节点种类::普通, std::nullopt},
        {键(冻结材料锚点本地键值), 节点种类::普通, std::nullopt},
        {键(正式选择完整请求属性类型本地键值), 节点种类::属性类型,
            L1所有者范围值表示种类::U64组},
        {键(选择记录族归属关系类型本地键值), 节点种类::普通, std::nullopt},
        {键(冻结材料族归属关系类型本地键值), 节点种类::普通, std::nullopt},
        {键(任务当前正式选择关系类型本地键值), 节点种类::普通, std::nullopt},
        {键(选择记录方法引用关系类型本地键值), 节点种类::普通, std::nullopt},
        {键(选择记录路径引用关系类型本地键值), 节点种类::普通, std::nullopt},
        {键(选择记录来源需求关系类型本地键值), 节点种类::普通, std::nullopt},
        {键(选择记录稳定证据关系类型本地键值), 节点种类::普通, std::nullopt},
        {键(选择记录冻结材料关系类型本地键值), 节点种类::普通, std::nullopt}};
    const auto 写入 = 写入端口.提交所有者范围中性写集(写集);
    if ((写入.状态 != L1所有者范围写入状态::成功
            && 写入.状态 != L1所有者范围写入状态::精确重复)
        || 写入.新编码映射.size() != 写集.节点.size())
        throw std::runtime_error("task selection registry failed");
    任务正式选择登记定位 定位;
    定位.建立事实代次 = 写入.事实代次;
    for (const auto& [本地键, 编码] : 写入.新编码映射) {
        switch (本地键.值) {
        case 选择记录锚点本地键值: 定位.选择记录锚点 = 编码; break;
        case 冻结材料锚点本地键值: 定位.冻结材料锚点 = 编码; break;
        case 正式选择完整请求属性类型本地键值:
            定位.完整请求属性类型 = 编码; break;
        case 选择记录族归属关系类型本地键值:
            定位.选择记录族归属关系类型 = 编码; break;
        case 冻结材料族归属关系类型本地键值:
            定位.冻结材料族归属关系类型 = 编码; break;
        case 任务当前正式选择关系类型本地键值:
            定位.任务当前正式选择关系类型 = 编码; break;
        case 选择记录方法引用关系类型本地键值:
            定位.选择记录方法引用关系类型 = 编码; break;
        case 选择记录路径引用关系类型本地键值:
            定位.选择记录路径引用关系类型 = 编码; break;
        case 选择记录来源需求关系类型本地键值:
            定位.选择记录来源需求关系类型 = 编码; break;
        case 选择记录稳定证据关系类型本地键值:
            定位.选择记录稳定证据关系类型 = 编码; break;
        case 选择记录冻结材料关系类型本地键值:
            定位.选择记录冻结材料关系类型 = 编码; break;
        default: throw std::runtime_error("task selection registry mapping invalid");
        }
    }
    if (!有效(定位.选择记录锚点) || !有效(定位.冻结材料锚点)
        || !有效(定位.完整请求属性类型)
        || !有效(定位.任务当前正式选择关系类型)
        || !有效(定位.选择记录冻结材料关系类型))
        throw std::runtime_error("task selection registry incomplete");
    return 定位;
}

} // namespace 海中鱼巣::L2任务结构内部

namespace 海中鱼巣::L2任务结构内部 {

// ===== 任务子目标承接记录登记 =====
struct 任务子目标承接记录定位 final {
    L1结构所有者身份 所有者;
    稳定编码 记录锚点;
    稳定编码 记录族关系类型;
    稳定编码 任务组关系类型;
    稳定编码 所属任务关系类型;
    稳定编码 虚拟存在关系类型;
    稳定编码 父需求关系类型;
    稳定编码 来源方法关系类型;
    稳定编码 主轴结果关系类型;
    稳定编码 来源条件关系类型;
    稳定编码 目标宿主关系类型;
    稳定编码 目标特征关系类型;
    稳定编码 目标合同关系类型;
    稳定编码 条件场景关系类型;
    稳定编码 子需求关系类型;
    稳定编码 子任务关系类型;
    稳定编码 实际状态关系类型;
    稳定编码 动态证据关系类型;
    稳定编码 筹办轮次属性类型;
    稳定编码 方法内容版本属性类型;
    稳定编码 方法规格版本属性类型;
    稳定编码 作用对象角色属性类型;
    稳定编码 条件来源属性类型;
    稳定编码 来源截止属性类型;
    稳定编码 建立幂等属性类型;
    稳定编码 状态属性类型;
    稳定编码 回流发生时间属性类型;
    稳定编码 回流版本属性类型;
    稳定编码 回流截止属性类型;
    std::uint64_t 建立事实代次 = 0;
};

inline constexpr L1所有者范围写入幂等身份
    子目标承接记录登记幂等身份{0x4C32'5355'4247'5459ULL};

inline constexpr std::uint32_t 子目标记录锚点键 = 0x0301'2300U;
inline constexpr std::uint32_t 子目标记录关系类型起始键 = 0x0301'2310U;
inline constexpr std::uint32_t 子目标记录属性类型起始键 = 0x0301'2340U;

任务子目标承接记录定位 初始化任务子目标承接记录登记(
    const L1事实基座服务& 第一层服务,
    L1所有者范围写端口& 写入端口) {
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 读取固定登记期望代次(
        第一层服务, 写入端口, 子目标承接记录登记幂等身份);
    写集.写入幂等身份 = 子目标承接记录登记幂等身份;
    写集.节点.push_back({{子目标记录锚点键}, 节点种类::普通, std::nullopt});
    for (std::uint32_t 偏移 = 0; 偏移 < 16; ++偏移)
        写集.节点.push_back({{子目标记录关系类型起始键 + 偏移},
            节点种类::普通, std::nullopt});
    for (std::uint32_t 偏移 = 0; 偏移 < 11; ++偏移)
        写集.节点.push_back({{子目标记录属性类型起始键 + 偏移},
            节点种类::属性类型, L1所有者范围值表示种类::U64组});
    const auto 写入 = 写入端口.提交所有者范围中性写集(写集);
    if ((写入.状态 != L1所有者范围写入状态::成功
            && 写入.状态 != L1所有者范围写入状态::精确重复)
        || 写入.事实代次 == 0 || 写入.新编码映射.size() != 28)
        throw std::runtime_error("L2 task subgoal record registry init failed");

    任务子目标承接记录定位 定位;
    定位.所有者 = 写入端口.所有者身份();
    for (const auto& [本地键, 编码] : 写入.新编码映射) {
        if (!有效(编码))
            throw std::runtime_error("L2 task subgoal registry mapping invalid");
        const auto 键 = 本地键.值;
        if (键 == 子目标记录锚点键) 定位.记录锚点 = 编码;
        else if (键 == 子目标记录关系类型起始键 + 0) 定位.记录族关系类型 = 编码;
        else if (键 == 子目标记录关系类型起始键 + 1) 定位.任务组关系类型 = 编码;
        else if (键 == 子目标记录关系类型起始键 + 2) 定位.所属任务关系类型 = 编码;
        else if (键 == 子目标记录关系类型起始键 + 3) 定位.虚拟存在关系类型 = 编码;
        else if (键 == 子目标记录关系类型起始键 + 4) 定位.父需求关系类型 = 编码;
        else if (键 == 子目标记录关系类型起始键 + 5) 定位.来源方法关系类型 = 编码;
        else if (键 == 子目标记录关系类型起始键 + 6) 定位.主轴结果关系类型 = 编码;
        else if (键 == 子目标记录关系类型起始键 + 7) 定位.来源条件关系类型 = 编码;
        else if (键 == 子目标记录关系类型起始键 + 8) 定位.目标宿主关系类型 = 编码;
        else if (键 == 子目标记录关系类型起始键 + 9) 定位.目标特征关系类型 = 编码;
        else if (键 == 子目标记录关系类型起始键 + 10) 定位.目标合同关系类型 = 编码;
        else if (键 == 子目标记录关系类型起始键 + 11) 定位.条件场景关系类型 = 编码;
        else if (键 == 子目标记录关系类型起始键 + 12) 定位.子需求关系类型 = 编码;
        else if (键 == 子目标记录关系类型起始键 + 13) 定位.子任务关系类型 = 编码;
        else if (键 == 子目标记录关系类型起始键 + 14) 定位.实际状态关系类型 = 编码;
        else if (键 == 子目标记录关系类型起始键 + 15) 定位.动态证据关系类型 = 编码;
        else if (键 == 子目标记录属性类型起始键 + 0) 定位.筹办轮次属性类型 = 编码;
        else if (键 == 子目标记录属性类型起始键 + 1) 定位.方法内容版本属性类型 = 编码;
        else if (键 == 子目标记录属性类型起始键 + 2) 定位.方法规格版本属性类型 = 编码;
        else if (键 == 子目标记录属性类型起始键 + 3) 定位.作用对象角色属性类型 = 编码;
        else if (键 == 子目标记录属性类型起始键 + 4) 定位.条件来源属性类型 = 编码;
        else if (键 == 子目标记录属性类型起始键 + 5) 定位.来源截止属性类型 = 编码;
        else if (键 == 子目标记录属性类型起始键 + 6) 定位.建立幂等属性类型 = 编码;
        else if (键 == 子目标记录属性类型起始键 + 7) 定位.状态属性类型 = 编码;
        else if (键 == 子目标记录属性类型起始键 + 8) 定位.回流发生时间属性类型 = 编码;
        else if (键 == 子目标记录属性类型起始键 + 9) 定位.回流版本属性类型 = 编码;
        else if (键 == 子目标记录属性类型起始键 + 10) 定位.回流截止属性类型 = 编码;
        else throw std::runtime_error("L2 task subgoal registry mapping unknown");
    }
    const 稳定编码 必需编码[] = {定位.记录锚点, 定位.记录族关系类型,
        定位.任务组关系类型, 定位.所属任务关系类型, 定位.虚拟存在关系类型,
        定位.父需求关系类型, 定位.来源方法关系类型, 定位.主轴结果关系类型,
        定位.来源条件关系类型, 定位.目标宿主关系类型, 定位.目标特征关系类型,
        定位.目标合同关系类型, 定位.条件场景关系类型, 定位.子需求关系类型,
        定位.子任务关系类型, 定位.实际状态关系类型, 定位.动态证据关系类型,
        定位.筹办轮次属性类型, 定位.方法内容版本属性类型,
        定位.方法规格版本属性类型, 定位.作用对象角色属性类型,
        定位.条件来源属性类型, 定位.来源截止属性类型, 定位.建立幂等属性类型,
        定位.状态属性类型, 定位.回流发生时间属性类型,
        定位.回流版本属性类型, 定位.回流截止属性类型};
    for (const auto 编码 : 必需编码) {
        if (!有效(编码))
            throw std::runtime_error("L2 task subgoal registry incomplete");
        const auto 读取 = 第一层服务.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 编码});
        const auto* 节点 = 读取.事实
            ? std::get_if<L1所有者范围节点事实>(&*读取.事实) : nullptr;
        if (读取.状态 != L1所有者范围读取状态::成功 || !节点
            || 节点->写入所有者 != 定位.所有者 || 节点->退出事实代次)
            throw std::runtime_error("L2 task subgoal registry readback failed");
    }
    定位.建立事实代次 = 写入.事实代次;
    return 定位;
}

inline constexpr std::uint32_t 子目标记录节点写入键 = 0x0301'2400U;
inline constexpr std::uint32_t 子目标记录关系写入起始键 = 0x0301'2410U;
inline constexpr std::uint32_t 子目标记录值写入起始键 = 0x0301'2440U;

inline L2结构状态 映射子目标记录读取状态(
    L1所有者范围读取状态 状态) noexcept {
    switch (状态) {
    case L1所有者范围读取状态::成功: return L2结构状态::已读取;
    case L1所有者范围读取状态::入口拒绝: return L2结构状态::入口拒绝;
    case L1所有者范围读取状态::许可拒绝: return L2结构状态::许可拒绝;
    case L1所有者范围读取状态::未找到: return L2结构状态::未找到;
    case L1所有者范围读取状态::已退出: return L2结构状态::已退出;
    case L1所有者范围读取状态::属性未设置: return L2结构状态::属性未设置;
    case L1所有者范围读取状态::事实代次漂移:
        return L2结构状态::事实代次漂移;
    case L1所有者范围读取状态::资源失败: return L2结构状态::资源失败;
    case L1所有者范围读取状态::内部不一致:
        return L2结构状态::内部不一致;
    }
    return L2结构状态::内部不一致;
}

std::optional<L2任务子目标承接记录事实> 读取任务子目标承接记录历史投影(
    const L1事实基座服务& 第一层服务,
    const 任务子目标承接记录定位& 定位,
    L2任务子目标承接记录身份 身份,
    std::uint64_t 截止, std::uint64_t 观察代次) {
    const auto 节点读取 = 第一层服务.读取所有者范围历史事实(
        {L1所有者范围CRUD合同版本, 身份.值});
    const auto* 节点 = 节点读取.事实
        ? std::get_if<L1所有者范围节点事实>(&*节点读取.事实) : nullptr;
    if (节点读取.状态 != L1所有者范围读取状态::成功 || !节点
        || 节点->编码 != 身份.值 || 节点->写入所有者 != 定位.所有者
        || 节点->种类 != 节点种类::普通 || 节点->属性类型表示
        || 节点->创建事实代次 == 0 || 节点->创建事实代次 > 截止
        || (节点->退出事实代次 && *节点->退出事实代次 <= 截止))
        return std::nullopt;

    const auto 读源关系组 = [&](稳定编码 类型) {
        return 第一层服务.读取所有者范围历史关系组(
            {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源,
                身份.值, 类型, 截止});
    };
    const auto 读唯一源目标 = [&](稳定编码 类型)
        -> std::optional<稳定编码> {
        const auto 读取 = 读源关系组(类型);
        if (读取.状态 != L1所有者范围读取状态::成功
            || 读取.历史截止事实代次 != 截止
            || 读取.读取事实代次 != 观察代次 || 读取.关系组.size() != 1)
            return std::nullopt;
        const auto& 关系 = 读取.关系组.front();
        if (关系.写入所有者 != 定位.所有者 || 关系.源节点 != 身份.值
            || 关系.关系类型节点 != 类型 || 关系.角色或顺序 != 1)
            return std::nullopt;
        return 关系.目标节点;
    };
    const auto 读可选源目标 = [&](稳定编码 类型,
        bool& 完整) -> std::optional<稳定编码> {
        const auto 读取 = 读源关系组(类型);
        if (读取.状态 != L1所有者范围读取状态::成功
            || 读取.历史截止事实代次 != 截止
            || 读取.读取事实代次 != 观察代次 || 读取.关系组.size() > 1) {
            完整 = false;
            return std::nullopt;
        }
        if (读取.关系组.empty()) return std::nullopt;
        const auto& 关系 = 读取.关系组.front();
        if (关系.写入所有者 != 定位.所有者 || 关系.源节点 != 身份.值
            || 关系.关系类型节点 != 类型 || 关系.角色或顺序 != 1) {
            完整 = false;
            return std::nullopt;
        }
        return 关系.目标节点;
    };

    const auto 族 = 读唯一源目标(定位.记录族关系类型);
    const auto 任务 = 读唯一源目标(定位.所属任务关系类型);
    const auto 虚拟存在 = 读唯一源目标(定位.虚拟存在关系类型);
    const auto 父需求 = 读唯一源目标(定位.父需求关系类型);
    const auto 方法 = 读唯一源目标(定位.来源方法关系类型);
    const auto 主轴结果 = 读唯一源目标(定位.主轴结果关系类型);
    const auto 条件 = 读唯一源目标(定位.来源条件关系类型);
    const auto 目标宿主 = 读唯一源目标(定位.目标宿主关系类型);
    const auto 目标特征 = 读唯一源目标(定位.目标特征关系类型);
    const auto 目标合同 = 读唯一源目标(定位.目标合同关系类型);
    bool 可选完整 = true;
    const auto 场景 = 读可选源目标(定位.条件场景关系类型, 可选完整);
    const auto 子需求 = 读可选源目标(定位.子需求关系类型, 可选完整);
    const auto 子任务 = 读可选源目标(定位.子任务关系类型, 可选完整);
    const auto 实际状态 = 读可选源目标(定位.实际状态关系类型, 可选完整);
    const auto 动态 = 读可选源目标(定位.动态证据关系类型, 可选完整);
    if (!族 || *族 != 定位.记录锚点 || !任务 || !虚拟存在 || !父需求
        || !方法 || !主轴结果 || !条件 || !目标宿主 || !目标特征
        || !目标合同 || !可选完整)
        return std::nullopt;
    const auto 组读取 = 第一层服务.读取所有者范围历史关系组(
        {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::目标,
            身份.值, 定位.任务组关系类型, 截止});
    if (组读取.状态 != L1所有者范围读取状态::成功
        || 组读取.读取事实代次 != 观察代次 || 组读取.关系组.size() != 1
        || 组读取.关系组.front().源节点 != *任务
        || 组读取.关系组.front().目标节点 != 身份.值
        || 组读取.关系组.front().角色或顺序 != 1)
        return std::nullopt;

    const auto 值读取 = 第一层服务.读取所有者范围历史属性值组(
        {L1所有者范围CRUD合同版本, 身份.值, 截止});
    if (值读取.状态 != L1所有者范围读取状态::成功
        || 值读取.历史截止事实代次 != 截止
        || 值读取.读取事实代次 != 观察代次)
        return std::nullopt;
    const auto 取单值 = [&](稳定编码 属性) -> std::optional<std::uint64_t> {
        const L1所有者范围值事实* 找到 = nullptr;
        for (const auto& 值 : 值读取.属性值组) {
            if (值.属性类型节点 != 属性) continue;
            if (找到 || 值.所属节点 != 身份.值 || 值.来源节点 != 身份.值
                || 值.写入所有者 != 定位.所有者)
                return std::nullopt;
            找到 = std::addressof(值);
        }
        if (!找到) return std::nullopt;
        const auto* 材料 = std::get_if<std::vector<std::uint64_t>>(&找到->材料);
        return 材料 && 材料->size() == 1
            ? std::optional<std::uint64_t>{材料->front()} : std::nullopt;
    };
    const auto 轮次 = 取单值(定位.筹办轮次属性类型);
    const auto 内容版本 = 取单值(定位.方法内容版本属性类型);
    const auto 规格版本 = 取单值(定位.方法规格版本属性类型);
    const auto 角色 = 取单值(定位.作用对象角色属性类型);
    const auto 条件来源 = 取单值(定位.条件来源属性类型);
    const auto 来源截止 = 取单值(定位.来源截止属性类型);
    const auto 建立幂等 = 取单值(定位.建立幂等属性类型);
    const auto 状态值 = 取单值(定位.状态属性类型);
    if (!轮次 || !内容版本 || !规格版本 || !角色 || !条件来源
        || !来源截止 || !建立幂等 || !状态值 || *轮次 == 0
        || *内容版本 == 0 || *规格版本 == 0 || *角色 == 0
        || *条件来源 == 0 || *来源截止 == 0 || *建立幂等 == 0
        || *状态值 < 1 || *状态值 > 3)
        return std::nullopt;

    L2任务子目标承接记录事实 事实;
    事实.身份 = 身份;
    事实.任务 = L2任务身份{*任务};
    事实.任务虚拟存在 = L2任务虚拟存在身份{*虚拟存在};
    事实.筹办轮次 = *轮次;
    事实.父需求 = L2需求身份{*父需求};
    事实.来源 = {L2方法身份{*方法}, L2方法内容版本{*内容版本},
        L2方法规格版本{*规格版本}, L2方法结果身份{*主轴结果},
        L2方法条件身份{*条件}, *角色, 稳定编码{*条件来源}};
    事实.子目标 = {L2存在身份{*目标宿主}, L2特征定义身份{*目标特征},
        L2目标状态合同身份{*目标合同},
        场景 ? std::optional<L2场景身份>{L2场景身份{*场景}} : std::nullopt};
    事实.来源共同事实截止 = *来源截止;
    事实.建立幂等身份 = {*建立幂等};
    事实.状态 = static_cast<L2任务子目标承接记录状态>(*状态值);
    if (子需求) 事实.子需求 = L2需求身份{*子需求};
    if (事实.状态 == L2任务子目标承接记录状态::子需求已回流待父需求重判) {
        const auto 时间 = 取单值(定位.回流发生时间属性类型);
        const auto 版本 = 取单值(定位.回流版本属性类型);
        const auto 回流截止 = 取单值(定位.回流截止属性类型);
        if (!实际状态 || !时间 || !版本 || !回流截止
            || *时间 == 0 || *版本 == 0 || *回流截止 == 0)
            return std::nullopt;
        L2任务子目标回流材料 回流;
        if (子任务) 回流.子需求承接任务 = L2任务身份{*子任务};
        回流.实际状态 = L2状态身份{*实际状态};
        if (动态) 回流.动态证据 = L2动态身份{*动态};
        回流.发生时间 = static_cast<std::int64_t>(*时间);
        回流.回流版本 = *版本;
        回流.共同事实截止 = *回流截止;
        事实.回流 = 回流;
    }
    const bool 状态守恒 =
        (事实.状态 == L2任务子目标承接记录状态::待绑定子需求
            && !事实.子需求 && !事实.回流 && !实际状态 && !子任务 && !动态)
        || (事实.状态 == L2任务子目标承接记录状态::等待子需求
            && 事实.子需求 && !事实.回流 && !实际状态 && !子任务 && !动态)
        || (事实.状态 == L2任务子目标承接记录状态::子需求已回流待父需求重判
            && 事实.子需求 && 事实.回流 && 实际状态);
    if (!状态守恒) return std::nullopt;
    事实.生命周期 = {节点->创建事实代次, 节点->退出事实代次};
    return 事实;
}

L1所有者范围写集请求 形成新增任务子目标承接记录写集(
    const L2新增任务子目标承接记录请求& 请求,
    const 任务子目标承接记录定位& 定位) {
    const auto 键 = [](std::uint32_t 值) { return L1所有者范围写集本地键{值}; };
    const auto 记录键 = 键(子目标记录节点写入键);
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 请求.请求头.期望事实代次;
    写集.写入幂等身份 = {请求.幂等身份.值};
    写集.节点 = {{记录键, 节点种类::普通, std::nullopt}};
    const auto 加关系 = [&](std::uint32_t 偏移, L1所有者范围事实引用 源,
        L1所有者范围事实引用 目标, 稳定编码 类型) {
        写集.关系.push_back({键(子目标记录关系写入起始键 + 偏移),
            源, 目标, 类型, 1});
    };
    加关系(0, 记录键, 定位.记录锚点, 定位.记录族关系类型);
    加关系(1, 请求.任务.值, 记录键, 定位.任务组关系类型);
    加关系(2, 记录键, 请求.任务.值, 定位.所属任务关系类型);
    加关系(3, 记录键, 请求.任务虚拟存在.值, 定位.虚拟存在关系类型);
    加关系(4, 记录键, 请求.父需求.值, 定位.父需求关系类型);
    加关系(5, 记录键, 请求.来源.方法.值, 定位.来源方法关系类型);
    加关系(6, 记录键, 请求.来源.主轴结果.值, 定位.主轴结果关系类型);
    加关系(7, 记录键, 请求.来源.条件.值, 定位.来源条件关系类型);
    加关系(8, 记录键, 请求.子目标.目标宿主.值, 定位.目标宿主关系类型);
    加关系(9, 记录键, 请求.子目标.目标特征.值, 定位.目标特征关系类型);
    加关系(10, 记录键, 请求.子目标.目标状态合同.值, 定位.目标合同关系类型);
    if (请求.子目标.条件场景)
        加关系(11, 记录键, 请求.子目标.条件场景->值, 定位.条件场景关系类型);
    const auto 加值 = [&](std::uint32_t 偏移, 稳定编码 类型,
        std::uint64_t 材料) {
        const auto 值键 = 键(子目标记录值写入起始键 + 偏移);
        写集.值.push_back({值键, 记录键, 类型,
            std::vector<std::uint64_t>{材料}, 记录键});
        写集.属性槽变更.push_back({记录键, 类型, 值键});
    };
    加值(0, 定位.筹办轮次属性类型, 请求.筹办轮次);
    加值(1, 定位.方法内容版本属性类型, 请求.来源.方法内容版本.值);
    加值(2, 定位.方法规格版本属性类型, 请求.来源.方法规格版本.值);
    加值(3, 定位.作用对象角色属性类型, 请求.来源.作用对象角色);
    加值(4, 定位.条件来源属性类型, 请求.来源.条件来源稳定编码.值);
    加值(5, 定位.来源截止属性类型, 请求.来源共同事实截止);
    加值(6, 定位.建立幂等属性类型, 请求.幂等身份.值);
    加值(7, 定位.状态属性类型,
        static_cast<std::uint64_t>(L2任务子目标承接记录状态::待绑定子需求));
    return 写集;
}

std::optional<L1所有者范围值事实> 查找记录当前属性值(
    const L1事实基座服务& 第一层服务,
    const 任务子目标承接记录定位& 定位,
    L2任务子目标承接记录身份 记录, 稳定编码 属性,
    std::uint64_t 截止, std::uint64_t 观察代次) {
    const auto 读取 = 第一层服务.读取所有者范围历史属性值组(
        {L1所有者范围CRUD合同版本, 记录.值, 截止});
    if (读取.状态 != L1所有者范围读取状态::成功
        || 读取.读取事实代次 != 观察代次) return std::nullopt;
    const L1所有者范围值事实* 找到 = nullptr;
    for (const auto& 值 : 读取.属性值组) {
        if (值.属性类型节点 != 属性) continue;
        if (找到 || 值.写入所有者 != 定位.所有者) return std::nullopt;
        找到 = std::addressof(值);
    }
    return 找到 ? std::optional<L1所有者范围值事实>{*找到} : std::nullopt;
}

std::optional<L1所有者范围关系事实> 查找记录当前关系(
    const L1事实基座服务& 第一层服务,
    const 任务子目标承接记录定位& 定位,
    L2任务子目标承接记录身份 记录, 稳定编码 类型,
    std::uint64_t 截止, std::uint64_t 观察代次,
    L1所有者范围关系端点方向 方向 = L1所有者范围关系端点方向::源) {
    const auto 读取 = 第一层服务.读取所有者范围历史关系组(
        {L1所有者范围CRUD合同版本, 方向, 记录.值, 类型, 截止});
    if (读取.状态 != L1所有者范围读取状态::成功
        || 读取.读取事实代次 != 观察代次 || 读取.关系组.size() != 1
        || 读取.关系组.front().写入所有者 != 定位.所有者)
        return std::nullopt;
    return 读取.关系组.front();
}

L1所有者范围写集请求 形成绑定任务子目标承接记录写集(
    const L2绑定任务子目标承接记录子需求请求& 请求,
    const 任务子目标承接记录定位& 定位,
    const L1所有者范围值事实& 旧状态值) {
    const auto 键 = [](std::uint32_t 值) { return L1所有者范围写集本地键{值}; };
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 请求.请求头.期望事实代次;
    写集.写入幂等身份 = {请求.幂等身份.值};
    写集.关系 = {{键(子目标记录关系写入起始键), 请求.记录.值,
        请求.子需求.值, 定位.子需求关系类型, 1}};
    const auto 状态键 = 键(子目标记录值写入起始键);
    写集.值 = {{状态键, 请求.记录.值, 定位.状态属性类型,
        std::vector<std::uint64_t>{static_cast<std::uint64_t>(
            L2任务子目标承接记录状态::等待子需求)}, 请求.记录.值}};
    写集.属性槽变更 = {{请求.记录.值, 定位.状态属性类型, 状态键}};
    写集.退出事实 = {旧状态值.编码};
    return 写集;
}

L1所有者范围写集请求 形成登记任务子目标承接记录回流写集(
    const L2登记任务子目标承接记录回流请求& 请求,
    const 任务子目标承接记录定位& 定位,
    const L1所有者范围值事实& 旧状态值) {
    const auto 键 = [](std::uint32_t 值) { return L1所有者范围写集本地键{值}; };
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 请求.请求头.期望事实代次;
    写集.写入幂等身份 = {请求.幂等身份.值};
    std::uint32_t 关系偏移 = 0;
    if (请求.回流.子需求承接任务)
        写集.关系.push_back({键(子目标记录关系写入起始键 + 关系偏移++),
            请求.记录.值, 请求.回流.子需求承接任务->值,
            定位.子任务关系类型, 1});
    写集.关系.push_back({键(子目标记录关系写入起始键 + 关系偏移++),
        请求.记录.值, 请求.回流.实际状态.值, 定位.实际状态关系类型, 1});
    if (请求.回流.动态证据)
        写集.关系.push_back({键(子目标记录关系写入起始键 + 关系偏移),
            请求.记录.值, 请求.回流.动态证据->值, 定位.动态证据关系类型, 1});
    const auto 加值 = [&](std::uint32_t 偏移, 稳定编码 类型, std::uint64_t 材料) {
        const auto 值键 = 键(子目标记录值写入起始键 + 偏移);
        写集.值.push_back({值键, 请求.记录.值, 类型,
            std::vector<std::uint64_t>{材料}, 请求.记录.值});
        写集.属性槽变更.push_back({请求.记录.值, 类型, 值键});
    };
    加值(0, 定位.状态属性类型, static_cast<std::uint64_t>(
        L2任务子目标承接记录状态::子需求已回流待父需求重判));
    加值(1, 定位.回流发生时间属性类型,
        static_cast<std::uint64_t>(请求.回流.发生时间));
    加值(2, 定位.回流版本属性类型, 请求.回流.回流版本);
    加值(3, 定位.回流截止属性类型, 请求.回流.共同事实截止);
    写集.退出事实 = {旧状态值.编码};
    return 写集;
}

std::optional<L1所有者范围写集请求> 形成退出任务子目标承接记录写集(
    const L1事实基座服务& 第一层服务,
    const L2退出任务子目标承接记录请求& 请求,
    const 任务子目标承接记录定位& 定位,
    std::uint64_t 观察代次) {
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 请求.请求头.期望事实代次;
    写集.写入幂等身份 = {请求.幂等身份.值};
    写集.退出事实.push_back(请求.记录.值);
    const 稳定编码 源关系类型[] = {定位.记录族关系类型,
        定位.所属任务关系类型, 定位.虚拟存在关系类型,
        定位.父需求关系类型, 定位.来源方法关系类型,
        定位.主轴结果关系类型, 定位.来源条件关系类型,
        定位.目标宿主关系类型, 定位.目标特征关系类型,
        定位.目标合同关系类型, 定位.条件场景关系类型,
        定位.子需求关系类型, 定位.子任务关系类型,
        定位.实际状态关系类型, 定位.动态证据关系类型};
    for (const auto 类型 : 源关系类型) {
        const auto 读取 = 第一层服务.读取所有者范围历史关系组(
            {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源,
                请求.记录.值, 类型, 请求.请求头.期望事实代次});
        if (读取.状态 != L1所有者范围读取状态::成功
            || 读取.读取事实代次 != 观察代次 || 读取.关系组.size() > 1)
            return std::nullopt;
        if (!读取.关系组.empty()) 写集.退出事实.push_back(读取.关系组.front().编码);
    }
    const auto 组 = 第一层服务.读取所有者范围历史关系组(
        {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::目标,
            请求.记录.值, 定位.任务组关系类型, 请求.请求头.期望事实代次});
    if (组.状态 != L1所有者范围读取状态::成功
        || 组.读取事实代次 != 观察代次 || 组.关系组.size() != 1)
        return std::nullopt;
    写集.退出事实.push_back(组.关系组.front().编码);
    const auto 值 = 第一层服务.读取所有者范围历史属性值组(
        {L1所有者范围CRUD合同版本, 请求.记录.值,
            请求.请求头.期望事实代次});
    if (值.状态 != L1所有者范围读取状态::成功
        || 值.读取事实代次 != 观察代次 || 值.属性值组.size() < 8)
        return std::nullopt;
    for (const auto& 当前值 : 值.属性值组) 写集.退出事实.push_back(当前值.编码);
    std::sort(写集.退出事实.begin(), 写集.退出事实.end(),
        [](稳定编码 左, 稳定编码 右) { return 左.值 < 右.值; });
    if (std::adjacent_find(写集.退出事实.begin(), 写集.退出事实.end())
        != 写集.退出事实.end()) return std::nullopt;
    return 写集;
}

struct 子目标记录提交结果 final {
    L2结构状态 状态 = L2结构状态::内部不一致;
    std::uint64_t 事实代次 = 0;
    std::vector<std::pair<L1所有者范围写集本地键, 稳定编码>> 编码映射;
};

子目标记录提交结果 提交任务子目标承接记录写集(
    L1所有者范围写端口& 写入端口,
    const L1所有者范围写集请求& 写集,
    std::uint64_t 当前代次) {
    if (当前代次 != 写集.期望事实代次) {
        const auto 首次 = 写入端口.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本, 写集.写入幂等身份});
        if (首次.状态 == L1所有者范围读取状态::未找到)
            return {L2结构状态::事实代次漂移, 当前代次, {}};
        if (首次.状态 != L1所有者范围读取状态::成功
            || !首次.首次规范化写集 || !首次.首次写入结果)
            return {映射子目标记录读取状态(首次.状态), 当前代次, {}};
        if (*首次.首次规范化写集 != 写集)
            return {L2结构状态::幂等冲突, 当前代次, {}};
        const auto 重放 = 写入端口.提交所有者范围中性写集(
            *首次.首次规范化写集);
        if (重放.状态 != L1所有者范围写入状态::精确重复)
            return {映射任务写入状态(重放.状态), 重放.事实代次, {}};
        return {L2结构状态::精确重复, 重放.事实代次, 重放.新编码映射};
    }
    const auto 写入 = 写入端口.提交所有者范围中性写集(写集);
    return {映射任务写入状态(写入.状态), 写入.事实代次,
        写入.新编码映射};
}

} // namespace 海中鱼巣::L2任务结构内部

export namespace 海中鱼巣 {

// ===== 所有者交付验证 =====
inline std::optional<L2任务所有者交付> 尝试形成L2任务所有者交付(
    const L1事实基座服务& 第一层读取服务,
    L1所有者范围交付&& 原始交付) noexcept {
    try {
        const auto& 建立 = 原始交付.建立结果;
        const bool 首次完整 = 建立.状态 == L1所有者范围管理状态::成功
            && 建立.是否形成内存权威发布
            && 建立.重试边界 == L1所有者范围重试边界::不适用;
        const bool 重复完整 = 建立.状态 == L1所有者范围管理状态::精确重复
            && !建立.是否形成内存权威发布
            && 建立.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛;
        if (建立.合同版本 != L1所有者范围CRUD合同版本
            || 建立.建立幂等身份 != 任务所有者建立身份
            || 建立.事实代次 == 0 || (!首次完整 && !重复完整)
            || !建立.所有者事实 || !原始交付.写入端口
            || 建立.所有者事实->范围种类 != L1所有者范围种类::独占结构范围
            || 建立.所有者事实->创建事实代次 == 0
            || 建立.所有者事实->退出事实代次
            || 原始交付.写入端口->所有者身份()
                != 建立.所有者事实->所有者
            || !原始交付.写入端口->有效()
            || !原始交付.写入端口->绑定于(第一层读取服务))
            return std::nullopt;
        const auto 当前 = 第一层读取服务.读取当前结构所有者(
            {L1所有者范围CRUD合同版本, 建立.所有者事实->所有者});
        if (当前.状态 != L1所有者范围读取状态::成功
            || 当前.合同版本 != L1所有者范围CRUD合同版本
            || 当前.查询所有者 != 建立.所有者事实->所有者
            || !当前.所有者事实 || *当前.所有者事实 != *建立.所有者事实)
            return std::nullopt;
        return L2任务所有者交付{std::move(*原始交付.写入端口)};
    } catch (...) {
        return std::nullopt;
    }
}

} // namespace 海中鱼巣

namespace 海中鱼巣::L2任务结构内部 {

// ===== 任务初次筹办首次准备记录 =====

struct 任务初次筹办准备记录定位 final {
    L1结构所有者身份 所有者;
    稳定编码 记录锚点;
    L2任务初次筹办准备记录关系类型身份组 关系类型;
    稳定编码 完整请求属性类型;
    稳定编码 首次裁决属性类型;
    稳定编码 筹办轮次属性类型;
    std::uint64_t 建立事实代次 = 0;
};

任务筹办轮次定位 初始化任务筹办轮次登记(
    const L1事实基座服务& 第一层服务,
    L1所有者范围写端口& 写入端口) {
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 读取固定登记期望代次(
        第一层服务, 写入端口, 任务筹办轮次登记幂等身份);
    写集.写入幂等身份 = 任务筹办轮次登记幂等身份;
    for (std::uint32_t 偏移 = 0; 偏移 < 2; ++偏移)
        写集.节点.push_back({{筹办轮次锚点起始键 + 偏移},
            节点种类::普通, std::nullopt});
    for (std::uint32_t 偏移 = 0; 偏移 < 18; ++偏移)
        写集.节点.push_back({{筹办轮次关系类型起始键 + 偏移},
            节点种类::普通, std::nullopt});
    for (std::uint32_t 偏移 = 0; 偏移 < 6; ++偏移)
        写集.节点.push_back({{筹办轮次属性类型起始键 + 偏移},
            节点种类::属性类型, L1所有者范围值表示种类::U64组});
    const auto 写入 = 写入端口.提交所有者范围中性写集(写集);
    if ((写入.状态 != L1所有者范围写入状态::成功
            && 写入.状态 != L1所有者范围写入状态::精确重复)
        || 写入.事实代次 == 0 || 写入.新编码映射.size() != 26)
        throw std::runtime_error("L2 task planning round registry init failed");
    任务筹办轮次定位 定位;
    定位.所有者 = 写入端口.所有者身份();
    定位.关系类型.resize(18);
    定位.属性类型.resize(6);
    for (const auto& [本地键, 编码] : 写入.新编码映射) {
        if (!有效(编码))
            throw std::runtime_error("L2 task planning round registry mapping invalid");
        const auto 键值 = 本地键.值;
        if (键值 == 筹办轮次锚点起始键) 定位.权威记录锚点 = 编码;
        else if (键值 == 筹办轮次锚点起始键 + 1) 定位.后继记录锚点 = 编码;
        else if (键值 >= 筹办轮次关系类型起始键
            && 键值 < 筹办轮次关系类型起始键 + 18)
            定位.关系类型[键值 - 筹办轮次关系类型起始键] = 编码;
        else if (键值 >= 筹办轮次属性类型起始键
            && 键值 < 筹办轮次属性类型起始键 + 6)
            定位.属性类型[键值 - 筹办轮次属性类型起始键] = 编码;
        else throw std::runtime_error("L2 task planning round registry mapping unknown");
    }
    if (!有效(定位.权威记录锚点) || !有效(定位.后继记录锚点)
        || std::any_of(定位.关系类型.begin(), 定位.关系类型.end(),
            [](稳定编码 编码) noexcept { return !有效(编码); })
        || std::any_of(定位.属性类型.begin(), 定位.属性类型.end(),
            [](稳定编码 编码) noexcept { return !有效(编码); }))
        throw std::runtime_error("L2 task planning round registry incomplete");
    定位.建立事实代次 = 写入.事实代次;
    return 定位;
}

inline constexpr L1所有者范围写入幂等身份
    初次筹办准备记录登记幂等身份{0x4C32'494E'4954'504CULL};
inline constexpr std::uint32_t 初次筹办记录锚点键 = 0x0301'2500U;
inline constexpr std::uint32_t 初次筹办记录关系类型起始键 = 0x0301'2510U;
inline constexpr std::uint32_t 初次筹办记录属性类型起始键 = 0x0301'2520U;

// ===== P00：任务轮次核心独立登记 =====
struct 任务轮次核心定位 final {
    L1结构所有者身份 所有者;
    稳定编码 任务轮次锚点;
    稳定编码 任务轮次族归属关系类型;
    稳定编码 任务轮次任务关系类型;
    稳定编码 任务当前轮次关系类型;
    稳定编码 任务轮次序号属性类型;
    std::uint64_t 建立事实代次 = 0;
};

// P00 当前任务与首轮的单次结构化读回投影；公开入口只做一次状态映射。
struct 当前任务轮次结构化投影 final {
    L2结构状态 公共状态 = L2结构状态::内部不一致;
    L2按任务读取当前任务与轮次状态 专用状态 =
        L2按任务读取当前任务与轮次状态::内部不一致;
    std::optional<L2任务事实> 任务;
    std::optional<L2任务轮次事实> 当前任务轮次;
    std::uint64_t 本次正式读回截止 = 0;
};

inline constexpr L1所有者范围写入幂等身份
    任务轮次核心登记幂等身份{0x4C32'5030'3052'4547ULL};
inline constexpr std::uint32_t 任务轮次核心登记键起始 = 0x0301'2180U;
inline constexpr std::uint32_t 任务轮次核心登记键数量 = 5U;

任务轮次核心定位 初始化任务轮次核心登记(
    const L1事实基座服务& 第一层服务,
    L1所有者范围写端口& 写入端口) {
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 读取固定登记期望代次(
        第一层服务, 写入端口, 任务轮次核心登记幂等身份);
    写集.写入幂等身份 = 任务轮次核心登记幂等身份;
    for (std::uint32_t 偏移 = 0; 偏移 < 4; ++偏移)
        写集.节点.push_back({{任务轮次核心登记键起始 + 偏移},
            节点种类::普通, std::nullopt});
    写集.节点.push_back({{任务轮次核心登记键起始 + 4},
        节点种类::属性类型, L1所有者范围值表示种类::U64组});
    const auto 写入 = 写入端口.提交所有者范围中性写集(写集);
    if ((写入.状态 != L1所有者范围写入状态::成功
            && 写入.状态 != L1所有者范围写入状态::精确重复)
        || 写入.事实代次 == 0
        || 写入.新编码映射.size() != 任务轮次核心登记键数量)
        throw std::runtime_error("P00 task round registry failed");
    任务轮次核心定位 定位;
    定位.所有者 = 写入端口.所有者身份();
    for (const auto& [本地键, 编码] : 写入.新编码映射) {
        if (!有效(编码) || 本地键.值 < 任务轮次核心登记键起始
            || 本地键.值 >= 任务轮次核心登记键起始 + 5)
            throw std::runtime_error("P00 task round registry mapping invalid");
        switch (本地键.值 - 任务轮次核心登记键起始) {
        case 0: 定位.任务轮次锚点 = 编码; break;
        case 1: 定位.任务轮次族归属关系类型 = 编码; break;
        case 2: 定位.任务轮次任务关系类型 = 编码; break;
        case 3: 定位.任务当前轮次关系类型 = 编码; break;
        case 4: 定位.任务轮次序号属性类型 = 编码; break;
        default: throw std::runtime_error("P00 task round registry mapping unknown");
        }
    }

    const 稳定编码 必需[] = {定位.任务轮次锚点,
        定位.任务轮次族归属关系类型, 定位.任务轮次任务关系类型,
        定位.任务当前轮次关系类型, 定位.任务轮次序号属性类型};
    for (const auto 编码 : 必需) {
        const auto 读取 = 第一层服务.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 编码});
        const auto* 节点 = 读取.事实
            ? std::get_if<L1所有者范围节点事实>(&*读取.事实) : nullptr;
        if (读取.状态 != L1所有者范围读取状态::成功 || !节点
            || 节点->写入所有者 != 定位.所有者 || 节点->退出事实代次)
            throw std::runtime_error("P00 task round registry readback failed");
    }
    定位.建立事实代次 = 写入.事实代次;
    return 定位;
}

任务初次筹办准备记录定位 初始化任务初次筹办准备记录登记(
    const L1事实基座服务& 第一层服务,
    L1所有者范围写端口& 写入端口) {
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 读取固定登记期望代次(
        第一层服务, 写入端口, 初次筹办准备记录登记幂等身份);
    写集.写入幂等身份 = 初次筹办准备记录登记幂等身份;
    写集.节点.push_back({{初次筹办记录锚点键},
        节点种类::普通, std::nullopt});
    for (std::uint32_t 偏移 = 0; 偏移 < 4; ++偏移)
        写集.节点.push_back({{初次筹办记录关系类型起始键 + 偏移},
            节点种类::普通, std::nullopt});
    for (std::uint32_t 偏移 = 0; 偏移 < 3; ++偏移)
        写集.节点.push_back({{初次筹办记录属性类型起始键 + 偏移},
            节点种类::属性类型, L1所有者范围值表示种类::U64组});
    const auto 写入 = 写入端口.提交所有者范围中性写集(写集);
    if ((写入.状态 != L1所有者范围写入状态::成功
            && 写入.状态 != L1所有者范围写入状态::精确重复)
        || 写入.事实代次 == 0 || 写入.新编码映射.size() != 8)
        throw std::runtime_error("L2 task initial planning record registry init failed");

    任务初次筹办准备记录定位 定位;
    定位.所有者 = 写入端口.所有者身份();
    for (const auto& [本地键, 编码] : 写入.新编码映射) {
        if (!有效(编码))
            throw std::runtime_error("L2 task initial planning registry mapping invalid");
        const auto 键 = 本地键.值;
        if (键 == 初次筹办记录锚点键) 定位.记录锚点 = 编码;
        else if (键 == 初次筹办记录关系类型起始键 + 0)
            定位.关系类型.记录族归属关系类型 = 编码;
        else if (键 == 初次筹办记录关系类型起始键 + 1)
            定位.关系类型.记录任务关系类型 = 编码;
        else if (键 == 初次筹办记录关系类型起始键 + 2)
            定位.关系类型.记录任务虚拟存在关系类型 = 编码;
        else if (键 == 初次筹办记录关系类型起始键 + 3)
            定位.关系类型.记录需求列表项关系类型 = 编码;
        else if (键 == 初次筹办记录属性类型起始键 + 0)
            定位.完整请求属性类型 = 编码;
        else if (键 == 初次筹办记录属性类型起始键 + 1)
            定位.首次裁决属性类型 = 编码;
        else if (键 == 初次筹办记录属性类型起始键 + 2)
            定位.筹办轮次属性类型 = 编码;
        else throw std::runtime_error("L2 task initial planning registry mapping unknown");
    }
    const 稳定编码 必需编码[] = {定位.记录锚点,
        定位.关系类型.记录族归属关系类型,
        定位.关系类型.记录任务关系类型,
        定位.关系类型.记录任务虚拟存在关系类型,
        定位.关系类型.记录需求列表项关系类型,
        定位.完整请求属性类型, 定位.首次裁决属性类型,
        定位.筹办轮次属性类型};
    for (const auto 编码 : 必需编码) {
        if (!有效(编码))
            throw std::runtime_error("L2 task initial planning registry incomplete");
        const auto 读取 = 第一层服务.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 编码});
        const auto* 节点 = 读取.事实
            ? std::get_if<L1所有者范围节点事实>(&*读取.事实) : nullptr;
        if (读取.状态 != L1所有者范围读取状态::成功 || !节点
            || 节点->写入所有者 != 定位.所有者 || 节点->退出事实代次)
            throw std::runtime_error("L2 task initial planning registry readback failed");
    }
    定位.建立事实代次 = 写入.事实代次;
    return 定位;
}

inline constexpr std::uint32_t 初次筹办任务节点写入键 = 0x0301'2600U;
inline constexpr std::uint32_t 初次筹办虚拟存在节点写入键 = 0x0301'2601U;
inline constexpr std::uint32_t 初次筹办记录节点写入键 = 0x0301'2602U;
inline constexpr std::uint32_t 初次筹办任务关系写入起始键 = 0x0301'2610U;
inline constexpr std::uint32_t 初次筹办任务来源需求关系写入键 = 0x0301'2613U;
inline constexpr std::uint32_t 任务轮次核心任务正式存在关系写入键 = 0x0301'2614U;
inline constexpr std::uint32_t 任务轮次核心轮次节点写入键 = 0x0301'2660U;
inline constexpr std::uint32_t 任务轮次核心轮次族归属关系写入键 = 0x0301'2661U;
inline constexpr std::uint32_t 任务轮次核心轮次任务关系写入键 = 0x0301'2662U;
inline constexpr std::uint32_t 任务轮次核心当前轮次关系写入键 = 0x0301'2663U;
inline constexpr std::uint32_t 任务轮次核心轮次序号值写入键 = 0x0301'2664U;
static_assert(任务轮次核心任务正式存在关系写入键
    == 0x0301'2614U);
static_assert(初次筹办任务来源需求关系写入键 == 0x0301'2613U
    && 任务轮次核心任务正式存在关系写入键 == 0x0301'2614U);
static_assert(任务轮次核心轮次节点写入键 == 0x0301'2660U
    && 任务轮次核心轮次族归属关系写入键 == 0x0301'2661U
    && 任务轮次核心轮次任务关系写入键 == 0x0301'2662U
    && 任务轮次核心当前轮次关系写入键 == 0x0301'2663U
    && 任务轮次核心轮次序号值写入键 == 0x0301'2664U);
static_assert(初次筹办任务来源需求关系写入键
    != 初次筹办任务节点写入键
    && 初次筹办任务来源需求关系写入键
        != 初次筹办任务关系写入起始键
    && 初次筹办任务来源需求关系写入键
        != 初次筹办任务关系写入起始键 + 1U
    && 初次筹办任务来源需求关系写入键
        != 初次筹办任务关系写入起始键 + 2U);
static_assert(任务轮次核心任务正式存在关系写入键
    != 初次筹办任务节点写入键
    && 任务轮次核心任务正式存在关系写入键
        != 初次筹办任务关系写入起始键
    && 任务轮次核心任务正式存在关系写入键
        != 初次筹办任务关系写入起始键 + 1U
    && 任务轮次核心任务正式存在关系写入键
        != 初次筹办任务关系写入起始键 + 2U
    && 任务轮次核心任务正式存在关系写入键
        != 初次筹办任务来源需求关系写入键);
static_assert(任务轮次核心轮次节点写入键
    + 4U == 任务轮次核心轮次序号值写入键);
static_assert(初次筹办任务节点写入键 == 0x0301'2600U
    && 初次筹办任务关系写入起始键 == 0x0301'2610U
    && 初次筹办任务来源需求关系写入键 == 0x0301'2613U
    && 任务轮次核心任务正式存在关系写入键 == 0x0301'2614U
    && 任务轮次核心轮次节点写入键 == 0x0301'2660U
    && 任务轮次核心轮次族归属关系写入键 == 0x0301'2661U
    && 任务轮次核心轮次任务关系写入键 == 0x0301'2662U
    && 任务轮次核心当前轮次关系写入键 == 0x0301'2663U
    && 任务轮次核心轮次序号值写入键 == 0x0301'2664U);
constexpr bool P00任务轮次写入键两两互异() noexcept {
    constexpr std::uint32_t 键[] = {
        初次筹办任务节点写入键,
        初次筹办任务关系写入起始键,
        初次筹办任务关系写入起始键 + 1U,
        初次筹办任务来源需求关系写入键,
        任务轮次核心任务正式存在关系写入键,
        任务轮次核心轮次节点写入键,
        任务轮次核心轮次族归属关系写入键,
        任务轮次核心轮次任务关系写入键,
        任务轮次核心当前轮次关系写入键,
        任务轮次核心轮次序号值写入键};
    constexpr std::size_t 数量 = sizeof(键) / sizeof(键[0]);
    static_assert(数量 == 10);
    for (std::size_t i = 0; i < 数量; ++i)
        for (std::size_t j = i + 1; j < 数量; ++j)
            if (键[i] == 键[j]) return false;
    return true;
}
static_assert(P00任务轮次写入键两两互异());
static_assert(任务轮次核心轮次节点写入键
    != 任务轮次核心轮次族归属关系写入键
    && 任务轮次核心轮次节点写入键
        != 任务轮次核心轮次任务关系写入键
    && 任务轮次核心轮次节点写入键
        != 任务轮次核心当前轮次关系写入键
    && 任务轮次核心轮次族归属关系写入键
        != 任务轮次核心轮次任务关系写入键
    && 任务轮次核心轮次族归属关系写入键
        != 任务轮次核心当前轮次关系写入键
    && 任务轮次核心轮次任务关系写入键
        != 任务轮次核心当前轮次关系写入键);
inline constexpr std::uint32_t 初次筹办记录关系写入起始键 = 0x0301'2620U;
inline constexpr std::uint32_t 初次筹办记录值写入起始键 = 0x0301'2630U;
inline constexpr std::uint32_t 初次筹办任务轮次写入起始键 = 0x0301'2660U;
inline constexpr std::uint32_t 初次筹办治理动作写入起始键 = 0x0301'2680U;

struct 初次筹办融合写入编码映射 final {
    稳定编码 任务节点;
    稳定编码 虚拟存在节点;
    稳定编码 记录节点;
    稳定编码 任务族归属关系;
    稳定编码 任务需求列表项引用关系;
    稳定编码 任务虚拟存在归属关系;
    稳定编码 记录族归属关系;
    稳定编码 记录任务关系;
    稳定编码 记录任务虚拟存在关系;
    稳定编码 记录需求列表项关系;
    稳定编码 完整请求值;
    稳定编码 首次裁决值;
    稳定编码 筹办轮次值;
    稳定编码 轮次权威节点;
    稳定编码 权威族关系;
    稳定编码 权威任务关系;
    稳定编码 权威虚拟存在关系;
    稳定编码 权威列表项关系;
    稳定编码 权威首次记录关系;
    稳定编码 当前筹办准备关系;
    稳定编码 权威版本值;

    bool 统一轮次写入() const noexcept {
        return 有效(轮次权威节点) && 有效(权威族关系)
            && 有效(权威任务关系) && 有效(权威虚拟存在关系)
            && 有效(权威列表项关系) && 有效(权威首次记录关系)
            && 有效(当前筹办准备关系) && 有效(权威版本值);
    }
};

std::optional<初次筹办融合写入编码映射> 查找初次筹办融合写入编码映射(
    const L1所有者范围写入结果& 写入) noexcept {
    if (写入.新编码映射.size() != 13
        && 写入.新编码映射.size() != 21) return std::nullopt;
    初次筹办融合写入编码映射 映射;
    std::uint32_t 已找到 = 0;
    const auto 设置 = [&](稳定编码& 目标, 稳定编码 编码) noexcept {
        if (有效(目标) || !有效(编码)) return false;
        目标 = 编码;
        ++已找到;
        return true;
    };
    for (const auto& [本地键, 编码] : 写入.新编码映射) {
        const auto 键 = 本地键.值;
        bool 合法 = false;
        if (键 == 初次筹办任务节点写入键) 合法 = 设置(映射.任务节点, 编码);
        else if (键 == 初次筹办虚拟存在节点写入键) 合法 = 设置(映射.虚拟存在节点, 编码);
        else if (键 == 初次筹办记录节点写入键) 合法 = 设置(映射.记录节点, 编码);
        else if (键 == 初次筹办任务关系写入起始键 + 0) 合法 = 设置(映射.任务族归属关系, 编码);
        else if (键 == 初次筹办任务关系写入起始键 + 1) 合法 = 设置(映射.任务需求列表项引用关系, 编码);
        else if (键 == 初次筹办任务关系写入起始键 + 2) 合法 = 设置(映射.任务虚拟存在归属关系, 编码);
        else if (键 == 初次筹办记录关系写入起始键 + 0) 合法 = 设置(映射.记录族归属关系, 编码);
        else if (键 == 初次筹办记录关系写入起始键 + 1) 合法 = 设置(映射.记录任务关系, 编码);
        else if (键 == 初次筹办记录关系写入起始键 + 2) 合法 = 设置(映射.记录任务虚拟存在关系, 编码);
        else if (键 == 初次筹办记录关系写入起始键 + 3) 合法 = 设置(映射.记录需求列表项关系, 编码);
        else if (键 == 初次筹办记录值写入起始键 + 0) 合法 = 设置(映射.完整请求值, 编码);
        else if (键 == 初次筹办记录值写入起始键 + 1) 合法 = 设置(映射.首次裁决值, 编码);
        else if (键 == 初次筹办记录值写入起始键 + 2) 合法 = 设置(映射.筹办轮次值, 编码);
        else if (键 == 初次融合权威节点写入键) 合法 = 设置(映射.轮次权威节点, 编码);
        else if (键 == 初次融合权威关系写入起始键 + 0) 合法 = 设置(映射.权威族关系, 编码);
        else if (键 == 初次融合权威关系写入起始键 + 1) 合法 = 设置(映射.权威任务关系, 编码);
        else if (键 == 初次融合权威关系写入起始键 + 2) 合法 = 设置(映射.权威虚拟存在关系, 编码);
        else if (键 == 初次融合权威关系写入起始键 + 3) 合法 = 设置(映射.权威列表项关系, 编码);
        else if (键 == 初次融合权威关系写入起始键 + 4) 合法 = 设置(映射.权威首次记录关系, 编码);
        else if (键 == 初次融合权威关系写入起始键 + 5) 合法 = 设置(映射.当前筹办准备关系, 编码);
        else if (键 == 筹办轮次值写入起始键 + 0) 合法 = 设置(映射.权威版本值, 编码);
        if (!合法) return std::nullopt;
    }
    const auto 期望数量 = 写入.新编码映射.size();
    return 已找到 == 期望数量
        && (期望数量 == 13 || 映射.统一轮次写入())
        ? std::optional{映射} : std::nullopt;
}

std::optional<std::vector<std::uint64_t>> 编码初次筹办完整请求(
    const L2任务初次筹办准备完整请求材料& 材料) {
    if (!L2任务初次筹办准备完整请求材料完整(材料))
        return std::nullopt;
    return std::vector<std::uint64_t>{材料.外层合同版本,
        材料.准备幂等身份, 材料.请求身份, 材料.运行代次,
        材料.请求头.合同版本, 材料.请求头.期望事实代次,
        材料.需求.值.值, 材料.阶段幂等身份.存在建立.值,
        材料.阶段幂等身份.任务核心建立.值,
        材料.阶段幂等身份.阶段特征实例建立.值,
        材料.阶段幂等身份.首态建立.值,
        材料.阶段幂等身份.P1建立.值,
        材料.阶段幂等身份.首迁移.值,
        材料.治理函数合同版本, 材料.初次筹办推进规则版本};
}

struct 初次筹办完整请求解码结果 final {
    L2任务初次筹办请求材料状态 状态 =
        L2任务初次筹办请求材料状态::无记录;
    std::optional<L2任务初次筹办准备完整请求材料> 材料;
};

std::optional<初次筹办完整请求解码结果> 解码初次筹办完整请求(
    const L1所有者范围原始值材料& 原始) noexcept {
    const auto* 值 = std::get_if<std::vector<std::uint64_t>>(&原始);
    if (!值) return std::nullopt;
    if (值->size() == 8) {
        const L2结构请求头 请求头{
            static_cast<std::uint32_t>((*值)[4]), (*值)[5]};
        if ((*值)[0] != 1 || (*值)[1] == 0 || (*值)[2] == 0
            || (*值)[3] == 0 || !L2任务结构请求头合同有效(请求头)
            || 请求头.期望事实代次 == 0 || (*值)[6] == 0
            || (*值)[7] == 0 || (*值)[1] != (*值)[7])
            return std::nullopt;
        return 初次筹办完整请求解码结果{
            L2任务初次筹办请求材料状态::初次请求版本待迁移,
            std::nullopt};
    }
    if (值->size() != 15) return std::nullopt;
    L2任务初次筹办准备完整请求材料 材料;
    材料.外层合同版本 = static_cast<std::uint32_t>((*值)[0]);
    材料.准备幂等身份 = (*值)[1];
    材料.请求身份 = (*值)[2];
    材料.运行代次 = (*值)[3];
    材料.请求头 = {static_cast<std::uint32_t>((*值)[4]), (*值)[5]};
    材料.需求 = L2需求身份{稳定编码{(*值)[6]}};
    材料.阶段幂等身份 = {{(*值)[7]}, {(*值)[8]}, {(*值)[9]},
        {(*值)[10]}, {(*值)[11]}, {(*值)[12]}};
    材料.治理函数合同版本 = static_cast<std::uint32_t>((*值)[13]);
    材料.初次筹办推进规则版本 = static_cast<std::uint32_t>((*值)[14]);
    return L2任务初次筹办准备完整请求材料完整(材料)
        ? std::optional{初次筹办完整请求解码结果{
            L2任务初次筹办请求材料状态::当前版本, std::move(材料)}}
        : std::nullopt;
}

void 编码生命周期(std::vector<std::uint64_t>& 输出,
    const L2生命周期& 生命周期) {
    输出.push_back(生命周期.创建事实代次);
    输出.push_back(生命周期.退出事实代次.value_or(0));
}

L2生命周期 解码生命周期(const std::vector<std::uint64_t>& 输入,
    std::size_t& 游标) {
    const auto 创建 = 输入.at(游标++);
    const auto 退出 = 输入.at(游标++);
    return {创建, 退出 == 0 ? std::nullopt : std::optional{退出}};
}

std::optional<std::vector<std::uint64_t>> 编码初次筹办首次裁决(
    const L2任务初次筹办首次需求裁决材料& 材料) {
    if (!L2任务初次筹办首次需求裁决材料完整(材料))
        return std::nullopt;
    const auto* 当前I64 = std::get_if<std::int64_t>(
        &材料.当前特征值.类型化不可变材料);
    if (!当前I64 || !材料.需求事实.当前所属存在关系
        || !材料.需求事实.当前目标宿主关系
        || !材料.需求事实.当前目标特征关系
        || !材料.需求事实.当前目标合同关系
        || !材料.所属列表项.当前所属存在关系
        || !材料.所属列表项.当前目标宿主关系
        || !材料.所属列表项.当前目标特征关系
        || !材料.所属列表项.当前目标合同关系)
        return std::nullopt;
    std::vector<std::uint64_t> 输出;
    输出.reserve(80);
    const auto 加编码 = [&](稳定编码 编码) { 输出.push_back(编码.值); };
    输出.push_back(材料.裁决规则版本);
    输出.push_back(材料.裁决请求身份);
    加编码(材料.需求事实.身份.值);
    加编码(材料.需求事实.所属存在.值);
    加编码(材料.需求事实.目标宿主存在.值);
    加编码(材料.需求事实.目标特征定义.值);
    加编码(材料.需求事实.目标状态合同.值);
    编码生命周期(输出, 材料.需求事实.生命周期);
    const auto 编码需求关系 = [&](稳定编码 关系, 稳定编码 需求,
        稳定编码 目标, const L2生命周期& 生命周期) {
        加编码(关系); 加编码(需求); 加编码(目标);
        编码生命周期(输出, 生命周期);
    };
    编码需求关系(材料.需求事实.当前所属存在关系->关系稳定编码,
        材料.需求事实.当前所属存在关系->需求.值,
        材料.需求事实.当前所属存在关系->所属存在.值,
        材料.需求事实.当前所属存在关系->生命周期);
    编码需求关系(材料.需求事实.当前目标宿主关系->关系稳定编码,
        材料.需求事实.当前目标宿主关系->需求.值,
        材料.需求事实.当前目标宿主关系->目标宿主存在.值,
        材料.需求事实.当前目标宿主关系->生命周期);
    编码需求关系(材料.需求事实.当前目标特征关系->关系稳定编码,
        材料.需求事实.当前目标特征关系->需求.值,
        材料.需求事实.当前目标特征关系->目标特征定义.值,
        材料.需求事实.当前目标特征关系->生命周期);
    编码需求关系(材料.需求事实.当前目标合同关系->关系稳定编码,
        材料.需求事实.当前目标合同关系->需求.值,
        材料.需求事实.当前目标合同关系->目标状态合同.值,
        材料.需求事实.当前目标合同关系->生命周期);
    加编码(材料.当前成员关系.关系稳定编码);
    加编码(材料.当前成员关系.列表项.值);
    加编码(材料.当前成员关系.需求.值);
    编码生命周期(输出, 材料.当前成员关系.生命周期);
    加编码(材料.所属列表项.身份.值);
    加编码(材料.所属列表项.所属存在.值);
    加编码(材料.所属列表项.目标宿主存在.值);
    加编码(材料.所属列表项.目标特征定义.值);
    加编码(材料.所属列表项.目标状态合同.值);
    编码生命周期(输出, 材料.所属列表项.生命周期);
    const auto 编码列表项关系 = [&](稳定编码 关系, 稳定编码 列表项,
        稳定编码 目标, const L2生命周期& 生命周期) {
        加编码(关系); 加编码(列表项); 加编码(目标);
        编码生命周期(输出, 生命周期);
    };
    编码列表项关系(材料.所属列表项.当前所属存在关系->关系稳定编码,
        材料.所属列表项.当前所属存在关系->列表项.值,
        材料.所属列表项.当前所属存在关系->所属存在.值,
        材料.所属列表项.当前所属存在关系->生命周期);
    编码列表项关系(材料.所属列表项.当前目标宿主关系->关系稳定编码,
        材料.所属列表项.当前目标宿主关系->列表项.值,
        材料.所属列表项.当前目标宿主关系->目标宿主存在.值,
        材料.所属列表项.当前目标宿主关系->生命周期);
    编码列表项关系(材料.所属列表项.当前目标特征关系->关系稳定编码,
        材料.所属列表项.当前目标特征关系->列表项.值,
        材料.所属列表项.当前目标特征关系->目标特征定义.值,
        材料.所属列表项.当前目标特征关系->生命周期);
    编码列表项关系(材料.所属列表项.当前目标合同关系->关系稳定编码,
        材料.所属列表项.当前目标合同关系->列表项.值,
        材料.所属列表项.当前目标合同关系->目标状态合同.值,
        材料.所属列表项.当前目标合同关系->生命周期);
    加编码(材料.当前特征实例.身份.值);
    加编码(材料.当前特征实例.宿主存在.值);
    加编码(材料.当前特征实例.特征定义.值);
    编码生命周期(输出, 材料.当前特征实例.生命周期);
    加编码(材料.当前特征值.值稳定编码);
    加编码(材料.当前特征值.特征实例.值);
    输出.push_back(std::bit_cast<std::uint64_t>(*当前I64));
    加编码(材料.当前特征值.来源稳定编码);
    编码生命周期(输出, 材料.当前特征值.生命周期);
    加编码(材料.目标合同.身份.值);
    加编码(材料.目标合同.特征定义.值);
    加编码(材料.目标合同.比较注册.值);
    输出.push_back(static_cast<std::uint64_t>(材料.目标合同.比较用途));
    输出.push_back(std::bit_cast<std::uint64_t>(材料.目标合同.目标I64));
    输出.push_back(材料.目标合同.允许关系位);
    输出.push_back(材料.目标合同.规则版本);
    输出.push_back(材料.目标合同.首次幂等身份.值);
    输出.push_back(材料.目标合同.首次期望事实代次);
    加编码(材料.目标合同.来源稳定编码);
    加编码(材料.目标合同.特征定义关系稳定编码);
    加编码(材料.目标合同.比较注册关系稳定编码);
    加编码(材料.目标合同.目标值稳定编码);
    加编码(材料.目标合同.合同值稳定编码);
    编码生命周期(输出, 材料.目标合同.生命周期);
    加编码(材料.比较注册.值);
    输出.push_back(材料.比较算法版本);
    输出.push_back(static_cast<std::uint64_t>(材料.具名关系));
    输出.push_back(材料.允许关系位);
    输出.push_back(材料.首次共同事实截止G0);
    return 输出;
}

std::optional<L2任务初次筹办首次需求裁决材料> 解码初次筹办首次裁决(
    const L1所有者范围原始值材料& 原始) noexcept {
    const auto* 输入 = std::get_if<std::vector<std::uint64_t>>(&原始);
    if (!输入) return std::nullopt;
    try {
        std::size_t 游标 = 0;
        const auto 取 = [&]() { return 输入->at(游标++); };
        const auto 编码 = [&]() { return 稳定编码{取()}; };
        const auto 解码需求关系 = [&]() {
            const auto 关系 = 编码();
            const auto 需求 = L2需求身份{编码()};
            const auto 目标 = 编码();
            const auto 生命周期 = 解码生命周期(*输入, 游标);
            return std::tuple{关系, 需求, 目标, 生命周期};
        };
        L2任务初次筹办首次需求裁决材料 材料;
        材料.裁决规则版本 = static_cast<std::uint32_t>(取());
        材料.裁决请求身份 = 取();
        材料.需求事实.身份 = L2需求身份{编码()};
        材料.需求事实.所属存在 = L2存在身份{编码()};
        材料.需求事实.目标宿主存在 = L2存在身份{编码()};
        材料.需求事实.目标特征定义 = L2特征定义身份{编码()};
        材料.需求事实.目标状态合同 = L2目标状态合同身份{编码()};
        材料.需求事实.生命周期 = 解码生命周期(*输入, 游标);
        {
            const auto [关系, 需求, 目标, 生命周期] = 解码需求关系();
            材料.需求事实.当前所属存在关系 =
                L2需求所属存在关系事实{关系, 需求, L2存在身份{目标}, 生命周期};
        }
        {
            const auto [关系, 需求, 目标, 生命周期] = 解码需求关系();
            材料.需求事实.当前目标宿主关系 =
                L2需求目标宿主关系事实{关系, 需求, L2存在身份{目标}, 生命周期};
        }
        {
            const auto [关系, 需求, 目标, 生命周期] = 解码需求关系();
            材料.需求事实.当前目标特征关系 =
                L2需求目标特征关系事实{关系, 需求, L2特征定义身份{目标}, 生命周期};
        }
        {
            const auto [关系, 需求, 目标, 生命周期] = 解码需求关系();
            材料.需求事实.当前目标合同关系 =
                L2需求目标合同关系事实{关系, 需求, L2目标状态合同身份{目标}, 生命周期};
        }
        材料.当前成员关系.关系稳定编码 = 编码();
        材料.当前成员关系.列表项 = L2需求列表项身份{编码()};
        材料.当前成员关系.需求 = L2需求身份{编码()};
        材料.当前成员关系.生命周期 = 解码生命周期(*输入, 游标);
        材料.所属列表项.身份 = L2需求列表项身份{编码()};
        材料.所属列表项.所属存在 = L2存在身份{编码()};
        材料.所属列表项.目标宿主存在 = L2存在身份{编码()};
        材料.所属列表项.目标特征定义 = L2特征定义身份{编码()};
        材料.所属列表项.目标状态合同 = L2目标状态合同身份{编码()};
        材料.所属列表项.生命周期 = 解码生命周期(*输入, 游标);
        const auto 解码列表项关系 = [&]() {
            const auto 关系 = 编码();
            const auto 列表项 = L2需求列表项身份{编码()};
            const auto 目标 = 编码();
            const auto 生命周期 = 解码生命周期(*输入, 游标);
            return std::tuple{关系, 列表项, 目标, 生命周期};
        };
        {
            const auto [关系, 列表项, 目标, 生命周期] = 解码列表项关系();
            材料.所属列表项.当前所属存在关系 =
                L2列表项所属存在关系事实{关系, 列表项, L2存在身份{目标}, 生命周期};
        }
        {
            const auto [关系, 列表项, 目标, 生命周期] = 解码列表项关系();
            材料.所属列表项.当前目标宿主关系 =
                L2列表项目标宿主关系事实{关系, 列表项, L2存在身份{目标}, 生命周期};
        }
        {
            const auto [关系, 列表项, 目标, 生命周期] = 解码列表项关系();
            材料.所属列表项.当前目标特征关系 =
                L2列表项目标特征关系事实{关系, 列表项, L2特征定义身份{目标}, 生命周期};
        }
        {
            const auto [关系, 列表项, 目标, 生命周期] = 解码列表项关系();
            材料.所属列表项.当前目标合同关系 =
                L2列表项目标合同关系事实{关系, 列表项, L2目标状态合同身份{目标}, 生命周期};
        }
        材料.当前特征实例.身份 = L2特征实例身份{编码()};
        材料.当前特征实例.宿主存在 = L2存在身份{编码()};
        材料.当前特征实例.特征定义 = L2特征定义身份{编码()};
        材料.当前特征实例.生命周期 = 解码生命周期(*输入, 游标);
        材料.当前特征值.值稳定编码 = 编码();
        材料.当前特征值.特征实例 = L2特征实例身份{编码()};
        材料.当前特征值.类型化不可变材料 = std::bit_cast<std::int64_t>(取());
        材料.当前特征值.来源稳定编码 = 编码();
        材料.当前特征值.生命周期 = 解码生命周期(*输入, 游标);
        材料.当前特征实例.当前值 = 材料.当前特征值;
        材料.目标合同.身份 = L2目标状态合同身份{编码()};
        材料.目标合同.特征定义 = L2特征定义身份{编码()};
        材料.目标合同.比较注册 = L2特征比较注册身份{编码()};
        材料.目标合同.比较用途 = static_cast<L2特征比较用途>(取());
        材料.目标合同.目标I64 = std::bit_cast<std::int64_t>(取());
        材料.目标合同.允许关系位 = static_cast<std::uint8_t>(取());
        材料.目标合同.规则版本 = static_cast<std::uint32_t>(取());
        材料.目标合同.首次幂等身份 = L2结构幂等身份{取()};
        材料.目标合同.首次期望事实代次 = 取();
        材料.目标合同.来源稳定编码 = 编码();
        材料.目标合同.特征定义关系稳定编码 = 编码();
        材料.目标合同.比较注册关系稳定编码 = 编码();
        材料.目标合同.目标值稳定编码 = 编码();
        材料.目标合同.合同值稳定编码 = 编码();
        材料.目标合同.生命周期 = 解码生命周期(*输入, 游标);
        材料.比较注册 = L2特征比较注册身份{编码()};
        材料.比较算法版本 = static_cast<std::uint32_t>(取());
        材料.具名关系 = static_cast<L2特征比较具名关系>(取());
        材料.允许关系位 = static_cast<std::uint8_t>(取());
        材料.首次共同事实截止G0 = 取();
        if (游标 != 输入->size()
            || !L2任务初次筹办首次需求裁决材料完整(材料))
            return std::nullopt;
        return 材料;
    } catch (...) {
        return std::nullopt;
    }
}

L1所有者范围写集请求 形成初次筹办融合写集(
    const L2新增任务与首次初次筹办准备记录请求& 请求,
    const 任务身份来源定位& 来源,
    const 任务结构类型定位& 类型,
    const 任务初次筹办准备记录定位& 记录定位,
    const 任务筹办轮次定位& 轮次定位) {
    const auto 完整请求 = 编码初次筹办完整请求(请求.完整请求);
    const auto 首次裁决 = 编码初次筹办首次裁决(请求.首次需求裁决);
    if (!完整请求 || !首次裁决)
        throw std::runtime_error("initial planning record material encode failed");
    const auto 键 = [](std::uint32_t 值) noexcept {
        return L1所有者范围写集本地键{值};
    };
    const auto 任务键 = 键(初次筹办任务节点写入键);
    const auto 虚拟存在键 = 键(初次筹办虚拟存在节点写入键);
    const auto 记录键 = 键(初次筹办记录节点写入键);
    const auto 权威键 = 键(初次融合权威节点写入键);
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 请求.请求头.期望事实代次;
    写集.写入幂等身份 = {请求.完整请求.准备幂等身份};
    写集.节点 = {{任务键, 节点种类::普通, std::nullopt},
        {虚拟存在键, 节点种类::普通, std::nullopt},
        {记录键, 节点种类::普通, std::nullopt},
        {权威键, 节点种类::普通, std::nullopt}};
    写集.关系 = {
        {键(初次筹办任务关系写入起始键 + 0), 任务键,
            来源.任务锚点, 来源.任务族归属关系类型, 1},
        {键(初次筹办任务关系写入起始键 + 1), 任务键,
            请求.首次需求裁决.所属列表项.身份.值,
            类型.任务需求列表项引用关系类型, 1},
        {键(初次筹办任务关系写入起始键 + 2), 任务键,
            虚拟存在键, 来源.虚拟存在归属关系类型, 1},
        {键(初次筹办记录关系写入起始键 + 0), 记录键,
            记录定位.记录锚点,
            记录定位.关系类型.记录族归属关系类型, 1},
        {键(初次筹办记录关系写入起始键 + 1), 记录键,
            任务键, 记录定位.关系类型.记录任务关系类型, 1},
        {键(初次筹办记录关系写入起始键 + 2), 记录键,
            虚拟存在键,
            记录定位.关系类型.记录任务虚拟存在关系类型, 1},
        {键(初次筹办记录关系写入起始键 + 3), 记录键,
            请求.首次需求裁决.所属列表项.身份.值,
            记录定位.关系类型.记录需求列表项关系类型, 1},
        {键(初次融合权威关系写入起始键 + 0), 权威键,
            轮次定位.权威记录锚点, 轮次定位.关系类型[0], 1},
        {键(初次融合权威关系写入起始键 + 1), 权威键,
            任务键, 轮次定位.关系类型[1], 1},
        {键(初次融合权威关系写入起始键 + 2), 权威键,
            虚拟存在键, 轮次定位.关系类型[2], 1},
        {键(初次融合权威关系写入起始键 + 3), 权威键,
            请求.首次需求裁决.所属列表项.身份.值,
            轮次定位.关系类型[3], 1},
        {键(初次融合权威关系写入起始键 + 4), 权威键,
            记录键, 轮次定位.关系类型[4], 1},
        {键(初次融合权威关系写入起始键 + 5), 任务键,
            记录键, 轮次定位.关系类型[17], 1}};
    写集.值 = {
        {键(初次筹办记录值写入起始键 + 0), 记录键,
            记录定位.完整请求属性类型, *完整请求, 记录键},
        {键(初次筹办记录值写入起始键 + 1), 记录键,
            记录定位.首次裁决属性类型, *首次裁决, 记录键},
        {键(初次筹办记录值写入起始键 + 2), 记录键,
            记录定位.筹办轮次属性类型,
            std::vector<std::uint64_t>{1}, 记录键},
        {键(筹办轮次值写入起始键 + 0), 权威键,
            轮次定位.属性类型[0],
            std::vector<std::uint64_t>{L2任务筹办轮次权威规则版本},
            权威键}};
    写集.属性槽变更 = {
        {记录键, 记录定位.完整请求属性类型,
            键(初次筹办记录值写入起始键 + 0)},
        {记录键, 记录定位.首次裁决属性类型,
            键(初次筹办记录值写入起始键 + 1)},
        {记录键, 记录定位.筹办轮次属性类型,
            键(初次筹办记录值写入起始键 + 2)},
        {权威键, 轮次定位.属性类型[0],
            键(筹办轮次值写入起始键 + 0)}};
    return 写集;
}

enum class 任务轮次核心历史读取诊断 : std::uint8_t {
    成功, 未找到, 已退出, 许可拒绝, 事实代次漂移, 资源失败, 内部不一致
};

template<class 事实类型>
const 事实类型* 读取初次筹办历史事实(const L1事实基座服务& 第一层服务,
    稳定编码 编码, std::optional<事实类型>& 保存,
    任务轮次核心历史读取诊断* 诊断 = nullptr) {
    const auto 读取 = 第一层服务.读取所有者范围历史事实(
        {L1所有者范围CRUD合同版本, 编码});
    const auto* 事实 = 读取.事实 ? std::get_if<事实类型>(&*读取.事实) : nullptr;
    if (诊断) {
        switch (读取.状态) {
        case L1所有者范围读取状态::成功: *诊断 = 事实
            ? 任务轮次核心历史读取诊断::成功
            : 任务轮次核心历史读取诊断::内部不一致; break;
        case L1所有者范围读取状态::未找到: *诊断 =
            任务轮次核心历史读取诊断::未找到; break;
        case L1所有者范围读取状态::已退出: *诊断 =
            任务轮次核心历史读取诊断::已退出; break;
        case L1所有者范围读取状态::许可拒绝: *诊断 =
            任务轮次核心历史读取诊断::许可拒绝; break;
        case L1所有者范围读取状态::事实代次漂移: *诊断 =
            任务轮次核心历史读取诊断::事实代次漂移; break;
        case L1所有者范围读取状态::资源失败: *诊断 =
            任务轮次核心历史读取诊断::资源失败; break;
        default: *诊断 = 任务轮次核心历史读取诊断::内部不一致; break;
        }
    }
    if (读取.状态 != L1所有者范围读取状态::成功 || !事实)
        return nullptr;
    保存 = *事实;
    return &*保存;
}

struct 任务轮次核心写入编码映射 final {
    稳定编码 任务节点;
    稳定编码 任务族归属关系;
    稳定编码 任务需求列表项引用关系;
    稳定编码 任务来源需求关系;
    稳定编码 任务正式存在关系;
    稳定编码 轮次节点;
    稳定编码 轮次族归属关系;
    稳定编码 轮次任务关系;
    稳定编码 任务当前轮次关系;
    稳定编码 轮次序号值;
};

std::optional<任务轮次核心写入编码映射> 查找任务轮次核心写入编码映射(
    const L1所有者范围写入结果& 写入) noexcept {
    if (写入.新编码映射.size() != 10) return std::nullopt;
    任务轮次核心写入编码映射 映射;
    const auto 设置 = [](稳定编码& 目标, 稳定编码 编码) noexcept {
        if (有效(目标) || !有效(编码)) return false;
        目标 = 编码; return true;
    };
    for (const auto& [本地键, 编码] : 写入.新编码映射) {
        bool 命中 = false;
        switch (本地键.值) {
        case 初次筹办任务节点写入键:
            命中 = 设置(映射.任务节点, 编码); break;
        case 初次筹办任务关系写入起始键 + 0:
            命中 = 设置(映射.任务族归属关系, 编码); break;
        case 初次筹办任务关系写入起始键 + 1:
            命中 = 设置(映射.任务需求列表项引用关系, 编码); break;
        case 初次筹办任务来源需求关系写入键:
            命中 = 设置(映射.任务来源需求关系, 编码); break;
        case 任务轮次核心任务正式存在关系写入键:
            命中 = 设置(映射.任务正式存在关系, 编码); break;
        case 任务轮次核心轮次节点写入键:
            命中 = 设置(映射.轮次节点, 编码); break;
        case 任务轮次核心轮次族归属关系写入键:
            命中 = 设置(映射.轮次族归属关系, 编码); break;
        case 任务轮次核心轮次任务关系写入键:
            命中 = 设置(映射.轮次任务关系, 编码); break;
        case 任务轮次核心当前轮次关系写入键:
            命中 = 设置(映射.任务当前轮次关系, 编码); break;
        case 任务轮次核心轮次序号值写入键:
            命中 = 设置(映射.轮次序号值, 编码); break;
        default: break;
        }
        if (!命中) return std::nullopt;
    }
    return 有效(映射.任务节点) && 有效(映射.任务族归属关系)
        && 有效(映射.任务需求列表项引用关系)
        && 有效(映射.任务来源需求关系)
        && 有效(映射.任务正式存在关系) && 有效(映射.轮次节点)
        && 有效(映射.轮次族归属关系) && 有效(映射.轮次任务关系)
        && 有效(映射.任务当前轮次关系) && 有效(映射.轮次序号值)
        ? std::optional{映射} : std::nullopt;
}

L1所有者范围写集请求 形成任务轮次核心写集(
    const L2新增任务轮次与正式存在引用请求& 请求,
    const 任务身份来源定位& 来源, const 任务结构类型定位& 类型,
    const 任务轮次核心定位& 轮次) {
    const auto 键 = [](std::uint32_t 值) noexcept {
        return L1所有者范围写集本地键{值};
    };
    const auto 任务键 = 键(初次筹办任务节点写入键);
    const auto 轮次键 = 键(任务轮次核心轮次节点写入键);
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 请求.请求头.期望事实代次;
    写集.写入幂等身份 = {请求.幂等身份.值};
    写集.节点 = {{任务键, 节点种类::普通, std::nullopt},
        {轮次键, 节点种类::普通, std::nullopt}};
    写集.关系 = {
        {键(初次筹办任务关系写入起始键 + 0), 任务键,
            来源.任务锚点, 来源.任务族归属关系类型, 1},
        {键(初次筹办任务关系写入起始键 + 1), 任务键,
            请求.首次需求裁决.所属列表项.身份.值,
            类型.任务需求列表项引用关系类型, 1},
        {键(初次筹办任务来源需求关系写入键), 任务键,
            请求.首次需求裁决.需求事实.身份.值,
            类型.任务来源需求关系类型, 1},
        {键(任务轮次核心任务正式存在关系写入键), 任务键,
            请求.正式存在.值, 类型.任务正式存在关系类型, 1},
        {键(任务轮次核心轮次族归属关系写入键), 轮次键,
            轮次.任务轮次锚点, 轮次.任务轮次族归属关系类型, 1},
        {键(任务轮次核心轮次任务关系写入键), 轮次键,
            任务键, 轮次.任务轮次任务关系类型, 1},
        {键(任务轮次核心当前轮次关系写入键), 任务键,
            轮次键, 轮次.任务当前轮次关系类型, 1}};
    写集.值 = {{键(任务轮次核心轮次序号值写入键), 轮次键,
        轮次.任务轮次序号属性类型, std::vector<std::uint64_t>{1}, 轮次键}};
    写集.属性槽变更 = {{轮次键, 轮次.任务轮次序号属性类型,
        键(任务轮次核心轮次序号值写入键)}};
    return 写集;
}

L2新增任务轮次与正式存在引用结果 读回任务轮次核心(
    const L1事实基座服务& 第一层服务,
    const 任务身份来源定位& 来源, const 任务结构类型定位& 类型,
    const 任务轮次核心定位& 轮次,
    const 任务轮次核心写入编码映射& 映射,
    L2结构状态 返回状态, std::uint64_t G) {
    const auto 失败 = [&](L2结构状态 状态) noexcept {
        L2新增任务轮次与正式存在引用结果 结果;
        结果.结果头 = {L2结构合同版本, 状态, 0, std::nullopt};
        return 结果;
    };
    const auto 读取失败状态 = [&](任务轮次核心历史读取诊断 诊断) noexcept {
        switch (诊断) {
        case 任务轮次核心历史读取诊断::许可拒绝:
            return 失败(L2结构状态::许可拒绝);
        case 任务轮次核心历史读取诊断::资源失败:
            return 失败(L2结构状态::资源失败);
        case 任务轮次核心历史读取诊断::事实代次漂移:
            return 失败(L2结构状态::事实代次漂移);
        case 任务轮次核心历史读取诊断::未找到:
            return 失败(L2结构状态::未找到);
        case 任务轮次核心历史读取诊断::已退出:
            return 失败(L2结构状态::已退出);
        default:
            return 失败(L2结构状态::内部不一致);
        }
    };
    if ((返回状态 != L2结构状态::已提交
            && 返回状态 != L2结构状态::精确重复) || G == 0)
        return 失败(L2结构状态::内部不一致);
    std::optional<L1所有者范围节点事实> 任务保存, 轮次保存;
    任务轮次核心历史读取诊断 任务诊断 = 任务轮次核心历史读取诊断::内部不一致;
    任务轮次核心历史读取诊断 轮次诊断 = 任务轮次核心历史读取诊断::内部不一致;
    const auto* 任务节点 = 读取初次筹办历史事实(
        第一层服务, 映射.任务节点, 任务保存, &任务诊断);
    const auto* 轮次节点 = 读取初次筹办历史事实(
        第一层服务, 映射.轮次节点, 轮次保存, &轮次诊断);
    if (!任务节点) return 读取失败状态(任务诊断);
    if (!轮次节点) return 读取失败状态(轮次诊断);
    if (!任务节点 || !轮次节点 || 任务节点->写入所有者 != 来源.所有者
        || 轮次节点->写入所有者 != 来源.所有者
        || 任务节点->种类 != 节点种类::普通
        || 轮次节点->种类 != 节点种类::普通
        || 任务节点->创建事实代次 != G || 轮次节点->创建事实代次 != G
        || 任务节点->退出事实代次 || 轮次节点->退出事实代次)
        return 失败(L2结构状态::内部不一致);
    const 稳定编码 关系编码[] = {映射.任务族归属关系,
        映射.任务需求列表项引用关系, 映射.任务来源需求关系,
        映射.任务正式存在关系, 映射.轮次族归属关系,
        映射.轮次任务关系, 映射.任务当前轮次关系};
    std::vector<L1所有者范围关系事实> 关系;
    关系.reserve(7);
    for (const auto 编码 : 关系编码) {
        std::optional<L1所有者范围关系事实> 保存;
        任务轮次核心历史读取诊断 诊断 =
            任务轮次核心历史读取诊断::内部不一致;
        const auto* 事实 = 读取初次筹办历史事实(
            第一层服务, 编码, 保存, &诊断);
        if (!事实) return 读取失败状态(诊断);
        if (事实->写入所有者 != 来源.所有者
            || 事实->创建事实代次 != G || 事实->退出事实代次)
            return 失败(L2结构状态::内部不一致);
        关系.push_back(*事实);
    }
    const auto& 任务族 = 关系[0]; const auto& 列表项 = 关系[1];
    const auto& 来源需求 = 关系[2]; const auto& 正式存在 = 关系[3];
    const auto& 轮次族 = 关系[4]; const auto& 轮次任务 = 关系[5];
    const auto& 当前轮次 = 关系[6];
    if (任务族.源节点 != 映射.任务节点
        || 任务族.目标节点 != 来源.任务锚点
        || 任务族.关系类型节点 != 来源.任务族归属关系类型
        || 任务族.角色或顺序 != 1
        || 列表项.源节点 != 映射.任务节点
        || 列表项.关系类型节点 != 类型.任务需求列表项引用关系类型
        || 列表项.角色或顺序 != 1
        || 来源需求.源节点 != 映射.任务节点
        || 来源需求.关系类型节点 != 类型.任务来源需求关系类型
        || 来源需求.角色或顺序 != 1
        || 正式存在.源节点 != 映射.任务节点
        || 正式存在.关系类型节点 != 类型.任务正式存在关系类型
        || 正式存在.角色或顺序 != 1
        || 轮次族.源节点 != 映射.轮次节点
        || 轮次族.目标节点 != 轮次.任务轮次锚点
        || 轮次族.关系类型节点 != 轮次.任务轮次族归属关系类型
        || 轮次族.角色或顺序 != 1
        || 轮次任务.源节点 != 映射.轮次节点
        || 轮次任务.目标节点 != 映射.任务节点
        || 轮次任务.关系类型节点 != 轮次.任务轮次任务关系类型
        || 轮次任务.角色或顺序 != 1
        || 当前轮次.源节点 != 映射.任务节点
        || 当前轮次.目标节点 != 映射.轮次节点
        || 当前轮次.关系类型节点 != 轮次.任务当前轮次关系类型
        || 当前轮次.角色或顺序 != 1)
        return 失败(L2结构状态::内部不一致);
    std::optional<L1所有者范围值事实> 序号保存;
    const auto* 序号 = 读取初次筹办历史事实(
        第一层服务, 映射.轮次序号值, 序号保存);
    const auto* 序号材料 = 序号
        ? std::get_if<std::vector<std::uint64_t>>(&序号->材料) : nullptr;
    if (!序号 || 序号->写入所有者 != 来源.所有者
        || 序号->所属节点 != 映射.轮次节点
        || 序号->来源节点 != 映射.轮次节点
        || 序号->属性类型节点 != 轮次.任务轮次序号属性类型
        || 序号->创建事实代次 != G || 序号->退出事实代次
        || !序号材料 || 序号材料->size() != 1 || 序号材料->front() != 1)
        return 失败(L2结构状态::内部不一致);
    L2新增任务轮次与正式存在引用结果 结果;
    结果.结果头 = {L2结构合同版本, 返回状态, G, G};
    结果.任务 = L2任务事实{L2任务身份{映射.任务节点},
        L2需求列表项身份{列表项.目标节点},
        L2任务虚拟存在身份{正式存在.目标节点}, {G, std::nullopt}};
    结果.任务身份来源 = L2任务身份来源事实{来源.所有者, 映射.任务节点,
        来源.任务锚点, 来源.任务族归属关系类型, 映射.任务族归属关系,
        {G, std::nullopt}, {G, std::nullopt}};
    结果.来源需求关系 = L2任务来源需求关系事实{
        映射.任务来源需求关系, 结果.任务->身份,
        L2需求身份{来源需求.目标节点}, 1, {G, std::nullopt}};
    结果.正式存在关系 = L2任务正式存在关系事实{
        映射.任务正式存在关系, 结果.任务->身份,
        L2存在身份{正式存在.目标节点}, {G, std::nullopt}};
    结果.任务轮次 = L2任务轮次事实{L2任务轮次身份{映射.轮次节点},
        结果.任务->身份, 1, {G, std::nullopt}};
    return 结果.成功() ? 结果 : 失败(L2结构状态::内部不一致);
}

struct 首次P1治理写入编码映射 final {
    稳定编码 记录节点, 记录族关系, 记录任务关系, 记录存在关系,
        记录列表项关系, 完整请求值, 首次裁决值, 筹办轮次值;
    稳定编码 P1节点, P1族关系, P1任务关系, P1存在关系,
        P1列表项关系, P1记录关系, 当前准备关系, P1材料值;
    稳定编码 A1节点, A1族关系, A1函数关系, A1任务关系,
        A1轮次关系, A1P1关系, A1前态关系, A1材料值;
};

std::optional<首次P1治理写入编码映射> 查找首次P1治理写入编码映射(
    const L1所有者范围写入结果& 写入) noexcept {
    if (写入.新编码映射.size() != 24) return std::nullopt;
    首次P1治理写入编码映射 映射;
    std::uint32_t 数量 = 0;
    const auto 设置 = [&](稳定编码& 目标, 稳定编码 编码) noexcept {
        if (有效(目标) || !有效(编码)) return false;
        目标 = 编码; ++数量; return true;
    };
    for (const auto& [本地键, 编码] : 写入.新编码映射) {
        bool 合法 = false;
        switch (本地键.值) {
        case 初次筹办记录节点写入键: 合法 = 设置(映射.记录节点, 编码); break;
        case 初次筹办记录关系写入起始键 + 0: 合法 = 设置(映射.记录族关系, 编码); break;
        case 初次筹办记录关系写入起始键 + 1: 合法 = 设置(映射.记录任务关系, 编码); break;
        case 初次筹办记录关系写入起始键 + 2: 合法 = 设置(映射.记录存在关系, 编码); break;
        case 初次筹办记录关系写入起始键 + 3: 合法 = 设置(映射.记录列表项关系, 编码); break;
        case 初次筹办记录值写入起始键 + 0: 合法 = 设置(映射.完整请求值, 编码); break;
        case 初次筹办记录值写入起始键 + 1: 合法 = 设置(映射.首次裁决值, 编码); break;
        case 初次筹办记录值写入起始键 + 2: 合法 = 设置(映射.筹办轮次值, 编码); break;
        case 初次融合权威节点写入键: 合法 = 设置(映射.P1节点, 编码); break;
        case 初次融合权威关系写入起始键 + 0: 合法 = 设置(映射.P1族关系, 编码); break;
        case 初次融合权威关系写入起始键 + 1: 合法 = 设置(映射.P1任务关系, 编码); break;
        case 初次融合权威关系写入起始键 + 2: 合法 = 设置(映射.P1存在关系, 编码); break;
        case 初次融合权威关系写入起始键 + 3: 合法 = 设置(映射.P1列表项关系, 编码); break;
        case 初次融合权威关系写入起始键 + 4: 合法 = 设置(映射.P1记录关系, 编码); break;
        case 初次融合权威关系写入起始键 + 5: 合法 = 设置(映射.当前准备关系, 编码); break;
        case 筹办轮次值写入起始键 + 0: 合法 = 设置(映射.P1材料值, 编码); break;
        case 初次筹办治理动作写入起始键 + 0: 合法 = 设置(映射.A1节点, 编码); break;
        case 初次筹办治理动作写入起始键 + 1: 合法 = 设置(映射.A1族关系, 编码); break;
        case 初次筹办治理动作写入起始键 + 2: 合法 = 设置(映射.A1函数关系, 编码); break;
        case 初次筹办治理动作写入起始键 + 3: 合法 = 设置(映射.A1任务关系, 编码); break;
        case 初次筹办治理动作写入起始键 + 4: 合法 = 设置(映射.A1轮次关系, 编码); break;
        case 初次筹办治理动作写入起始键 + 5: 合法 = 设置(映射.A1P1关系, 编码); break;
        case 初次筹办治理动作写入起始键 + 6: 合法 = 设置(映射.A1前态关系, 编码); break;
        case 初次筹办治理动作写入起始键 + 7: 合法 = 设置(映射.A1材料值, 编码); break;
        default: break;
        }
        if (!合法) return std::nullopt;
    }
    return 数量 == 24 ? std::optional{映射} : std::nullopt;
}

L1所有者范围写集请求 形成首次P1治理写集(
    const L2新增首次准备记录与P1请求& 请求,
    const 任务初次筹办准备记录定位& 记录定位,
    const 任务筹办轮次定位& 轮次定位,
    const 任务初次筹办治理定位& 治理) {
    const auto 完整请求 = 编码初次筹办完整请求(请求.完整请求);
    const auto 首次裁决 = 编码初次筹办首次裁决(请求.首次需求裁决);
    if (!完整请求 || !首次裁决)
        throw std::runtime_error("initial P1 material encode failed");
    const auto 键 = [](std::uint32_t 值) noexcept {
        return L1所有者范围写集本地键{值};
    };
    const auto 记录键 = 键(初次筹办记录节点写入键);
    const auto P1键 = 键(初次融合权威节点写入键);
    const auto A1键 = 键(初次筹办治理动作写入起始键);
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 请求.请求头.期望事实代次;
    写集.写入幂等身份 = {请求.幂等身份.值};
    写集.节点 = {{记录键, 节点种类::普通, std::nullopt},
        {P1键, 节点种类::普通, std::nullopt},
        {A1键, 节点种类::普通, std::nullopt}};
    写集.关系 = {
        {键(初次筹办记录关系写入起始键 + 0), 记录键,
            记录定位.记录锚点, 记录定位.关系类型.记录族归属关系类型, 1},
        {键(初次筹办记录关系写入起始键 + 1), 记录键,
            请求.任务.值, 记录定位.关系类型.记录任务关系类型, 1},
        {键(初次筹办记录关系写入起始键 + 2), 记录键,
            请求.正式存在.值, 记录定位.关系类型.记录任务虚拟存在关系类型, 1},
        {键(初次筹办记录关系写入起始键 + 3), 记录键,
            请求.首次需求裁决.所属列表项.身份.值,
            记录定位.关系类型.记录需求列表项关系类型, 1},
        {键(初次融合权威关系写入起始键 + 0), P1键,
            轮次定位.权威记录锚点, 轮次定位.关系类型[0], 1},
        {键(初次融合权威关系写入起始键 + 1), P1键,
            请求.任务.值, 轮次定位.关系类型[1], 1},
        {键(初次融合权威关系写入起始键 + 2), P1键,
            请求.正式存在.值, 轮次定位.关系类型[2], 1},
        {键(初次融合权威关系写入起始键 + 3), P1键,
            请求.首次需求裁决.所属列表项.身份.值, 轮次定位.关系类型[3], 1},
        {键(初次融合权威关系写入起始键 + 4), P1键,
            记录键, 轮次定位.关系类型[4], 1},
        {键(初次融合权威关系写入起始键 + 5), 请求.任务.值,
            记录键, 轮次定位.关系类型[17], 1},
        {键(初次筹办治理动作写入起始键 + 1), A1键,
            治理.治理动作调用锚点, 治理.治理动作族归属关系类型, 1},
        {键(初次筹办治理动作写入起始键 + 2), A1键,
            治理.治理函数定义节点, 治理.治理动作函数关系类型, 1},
        {键(初次筹办治理动作写入起始键 + 3), A1键,
            请求.任务.值, 治理.治理动作任务关系类型, 1},
        {键(初次筹办治理动作写入起始键 + 4), A1键,
            请求.任务轮次.值, 治理.治理动作轮次关系类型, 1},
        {键(初次筹办治理动作写入起始键 + 5), A1键,
            P1键, 治理.治理动作筹办轮次关系类型, 1},
        {键(初次筹办治理动作写入起始键 + 6), A1键,
            请求.待初次筹办首态.值, 治理.治理动作触发前态关系类型, 1}};
    写集.值 = {
        {键(初次筹办记录值写入起始键 + 0), 记录键,
            记录定位.完整请求属性类型, *完整请求, 记录键},
        {键(初次筹办记录值写入起始键 + 1), 记录键,
            记录定位.首次裁决属性类型, *首次裁决, 记录键},
        {键(初次筹办记录值写入起始键 + 2), 记录键,
            记录定位.筹办轮次属性类型, std::vector<std::uint64_t>{1}, 记录键},
        {键(筹办轮次值写入起始键 + 0), P1键, 轮次定位.属性类型[0],
            std::vector<std::uint64_t>{L2任务筹办轮次权威规则版本,
                请求.任务轮次.值.值, 请求.正式存在.值.值,
                请求.阶段特征实例.值.值, 请求.待初次筹办首态.值.值}, P1键},
        {键(初次筹办治理动作写入起始键 + 7), A1键,
            治理.治理动作调用材料属性类型,
            std::vector<std::uint64_t>{请求.请求头.期望事实代次,
                请求.幂等身份.值, L2任务治理函数合同版本,
                L2初次筹办推进规则版本}, A1键}};
    写集.属性槽变更 = {
        {记录键, 记录定位.完整请求属性类型,
            键(初次筹办记录值写入起始键 + 0)},
        {记录键, 记录定位.首次裁决属性类型,
            键(初次筹办记录值写入起始键 + 1)},
        {记录键, 记录定位.筹办轮次属性类型,
            键(初次筹办记录值写入起始键 + 2)},
        {P1键, 轮次定位.属性类型[0], 键(筹办轮次值写入起始键 + 0)},
        {A1键, 治理.治理动作调用材料属性类型,
            键(初次筹办治理动作写入起始键 + 7)}};
    return 写集;
}

L2新增首次准备记录与P1结果 读回首次P1治理(
    const L1事实基座服务& 第一层服务,
    const 任务身份来源定位& 来源,
    const 任务初次筹办准备记录定位& 记录定位,
    const 任务筹办轮次定位& 轮次定位,
    const 任务初次筹办治理定位& 治理,
    const 首次P1治理写入编码映射& 映射,
    L2结构状态 返回状态, std::uint64_t G) {
    const auto 失败 = [&](L2结构状态 状态) noexcept {
        L2新增首次准备记录与P1结果 结果;
        结果.结果头 = {L2结构合同版本, 状态, G, G};
        return 结果;
    };
    if ((返回状态 != L2结构状态::已提交
            && 返回状态 != L2结构状态::精确重复) || G == 0)
        return 失败(L2结构状态::内部不一致);
    std::optional<L1所有者范围节点事实> 记录保存, P1保存, A1保存;
    const auto* 记录节点 = 读取初次筹办历史事实(
        第一层服务, 映射.记录节点, 记录保存);
    const auto* P1节点 = 读取初次筹办历史事实(
        第一层服务, 映射.P1节点, P1保存);
    const auto* A1节点 = 读取初次筹办历史事实(
        第一层服务, 映射.A1节点, A1保存);
    if (!记录节点 || !P1节点 || !A1节点
        || 记录节点->写入所有者 != 来源.所有者
        || P1节点->写入所有者 != 来源.所有者
        || A1节点->写入所有者 != 来源.所有者
        || 记录节点->创建事实代次 != G || P1节点->创建事实代次 != G
        || A1节点->创建事实代次 != G || 记录节点->退出事实代次
        || P1节点->退出事实代次 || A1节点->退出事实代次)
        return 失败(L2结构状态::内部不一致);
    const 稳定编码 关系编码[] = {映射.记录族关系, 映射.记录任务关系,
        映射.记录存在关系, 映射.记录列表项关系, 映射.P1族关系,
        映射.P1任务关系, 映射.P1存在关系, 映射.P1列表项关系,
        映射.P1记录关系, 映射.当前准备关系, 映射.A1族关系,
        映射.A1函数关系, 映射.A1任务关系, 映射.A1轮次关系,
        映射.A1P1关系, 映射.A1前态关系};
    std::vector<L1所有者范围关系事实> 关系;
    for (const auto 编码 : 关系编码) {
        std::optional<L1所有者范围关系事实> 保存;
        const auto* 事实 = 读取初次筹办历史事实(第一层服务, 编码, 保存);
        if (!事实 || 事实->写入所有者 != 来源.所有者
            || 事实->创建事实代次 != G || 事实->退出事实代次)
            return 失败(L2结构状态::内部不一致);
        关系.push_back(*事实);
    }
    const auto& 记录族 = 关系[0]; const auto& 记录任务 = 关系[1];
    const auto& 记录存在 = 关系[2]; const auto& 记录列表项 = 关系[3];
    const auto& P1族 = 关系[4]; const auto& P1任务 = 关系[5];
    const auto& P1存在 = 关系[6]; const auto& P1列表项 = 关系[7];
    const auto& P1记录 = 关系[8]; const auto& 当前准备 = 关系[9];
    const auto& A1族 = 关系[10]; const auto& A1函数 = 关系[11];
    const auto& A1任务 = 关系[12]; const auto& A1轮次 = 关系[13];
    const auto& A1P1 = 关系[14]; const auto& A1前态 = 关系[15];
    if (记录族.源节点 != 映射.记录节点
        || 记录族.目标节点 != 记录定位.记录锚点
        || 记录族.关系类型节点 != 记录定位.关系类型.记录族归属关系类型
        || 记录任务.源节点 != 映射.记录节点
        || 记录任务.关系类型节点 != 记录定位.关系类型.记录任务关系类型
        || 记录存在.源节点 != 映射.记录节点
        || 记录存在.关系类型节点 != 记录定位.关系类型.记录任务虚拟存在关系类型
        || 记录列表项.源节点 != 映射.记录节点
        || 记录列表项.关系类型节点 != 记录定位.关系类型.记录需求列表项关系类型
        || P1族.源节点 != 映射.P1节点 || P1族.目标节点 != 轮次定位.权威记录锚点
        || P1族.关系类型节点 != 轮次定位.关系类型[0]
        || P1任务.源节点 != 映射.P1节点 || P1任务.目标节点 != 记录任务.目标节点
        || P1任务.关系类型节点 != 轮次定位.关系类型[1]
        || P1存在.源节点 != 映射.P1节点 || P1存在.目标节点 != 记录存在.目标节点
        || P1存在.关系类型节点 != 轮次定位.关系类型[2]
        || P1列表项.源节点 != 映射.P1节点 || P1列表项.目标节点 != 记录列表项.目标节点
        || P1列表项.关系类型节点 != 轮次定位.关系类型[3]
        || P1记录.源节点 != 映射.P1节点 || P1记录.目标节点 != 映射.记录节点
        || P1记录.关系类型节点 != 轮次定位.关系类型[4]
        || 当前准备.源节点 != 记录任务.目标节点
        || 当前准备.目标节点 != 映射.记录节点
        || 当前准备.关系类型节点 != 轮次定位.关系类型[17]
        || A1族.源节点 != 映射.A1节点 || A1族.目标节点 != 治理.治理动作调用锚点
        || A1族.关系类型节点 != 治理.治理动作族归属关系类型
        || A1函数.源节点 != 映射.A1节点 || A1函数.目标节点 != 治理.治理函数定义节点
        || A1函数.关系类型节点 != 治理.治理动作函数关系类型
        || A1任务.源节点 != 映射.A1节点 || A1任务.目标节点 != 记录任务.目标节点
        || A1任务.关系类型节点 != 治理.治理动作任务关系类型
        || A1轮次.源节点 != 映射.A1节点
        || A1轮次.关系类型节点 != 治理.治理动作轮次关系类型
        || A1P1.源节点 != 映射.A1节点 || A1P1.目标节点 != 映射.P1节点
        || A1P1.关系类型节点 != 治理.治理动作筹办轮次关系类型
        || A1前态.源节点 != 映射.A1节点
        || A1前态.关系类型节点 != 治理.治理动作触发前态关系类型)
        return 失败(L2结构状态::内部不一致);
    std::optional<L1所有者范围值事实> 完整请求保存, 首次裁决保存,
        轮次值保存, P1材料保存, A1材料保存;
    const auto* 完整请求值 = 读取初次筹办历史事实(
        第一层服务, 映射.完整请求值, 完整请求保存);
    const auto* 首次裁决值 = 读取初次筹办历史事实(
        第一层服务, 映射.首次裁决值, 首次裁决保存);
    const auto* 轮次值 = 读取初次筹办历史事实(
        第一层服务, 映射.筹办轮次值, 轮次值保存);
    const auto* P1材料值 = 读取初次筹办历史事实(
        第一层服务, 映射.P1材料值, P1材料保存);
    const auto* A1材料值 = 读取初次筹办历史事实(
        第一层服务, 映射.A1材料值, A1材料保存);
    if (!完整请求值 || !首次裁决值 || !轮次值 || !P1材料值 || !A1材料值)
        return 失败(L2结构状态::内部不一致);
    const auto 完整请求 = 解码初次筹办完整请求(完整请求值->材料);
    const auto 首次裁决 = 解码初次筹办首次裁决(首次裁决值->材料);
    const auto* 轮次材料 = std::get_if<std::vector<std::uint64_t>>(&轮次值->材料);
    const auto* P1材料 = std::get_if<std::vector<std::uint64_t>>(&P1材料值->材料);
    const auto* A1材料 = std::get_if<std::vector<std::uint64_t>>(&A1材料值->材料);
    if (!完整请求 || 完整请求->状态 != L2任务初次筹办请求材料状态::当前版本
        || !完整请求->材料 || !首次裁决 || !轮次材料 || 轮次材料->size() != 1
        || 轮次材料->front() != 1 || !P1材料 || P1材料->size() != 5
        || (*P1材料)[0] != L2任务筹办轮次权威规则版本
        || (*P1材料)[1] != A1轮次.目标节点.值
        || (*P1材料)[2] != P1存在.目标节点.值
        || (*P1材料)[3] == 0 || (*P1材料)[4] != A1前态.目标节点.值
        || !A1材料 || A1材料->size() != 4 || (*A1材料)[0] == 0
        || (*A1材料)[1] != 完整请求->材料->阶段幂等身份.P1建立.值
        || (*A1材料)[2] != L2任务治理函数合同版本
        || (*A1材料)[3] != L2初次筹办推进规则版本)
        return 失败(L2结构状态::内部不一致);
    std::optional<L1所有者范围节点事实> 任务节点保存;
    const auto* 任务节点 = 读取初次筹办历史事实(
        第一层服务, 记录任务.目标节点, 任务节点保存);
    if (!任务节点 || 任务节点->写入所有者 != 来源.所有者
        || 任务节点->创建事实代次 <= 首次裁决->首次共同事实截止G0
        || 任务节点->创建事实代次 > G)
        return 失败(L2结构状态::内部不一致);
    L2新增首次准备记录与P1结果 结果;
    结果.结果头 = {L2结构合同版本, 返回状态, G, G};
    L2任务初次筹办准备记录事实 记录;
    记录.身份 = L2任务初次筹办准备记录身份{映射.记录节点};
    记录.完整请求 = *完整请求->材料;
    记录.首次需求裁决 = *首次裁决;
    记录.任务 = L2任务身份{记录任务.目标节点};
    记录.任务虚拟存在 = L2任务虚拟存在身份{记录存在.目标节点};
    记录.需求列表项 = L2需求列表项身份{记录列表项.目标节点};
    记录.筹办轮次 = 1;
    记录.首次任务建立代次G1 = 任务节点->创建事实代次;
    记录.生命周期 = {G, std::nullopt};
    记录.任务轮次 = L2任务轮次身份{A1轮次.目标节点};
    记录.正式存在 = L2存在身份{P1存在.目标节点};
    记录.阶段特征实例 = L2特征实例身份{稳定编码{(*P1材料)[3]}};
    记录.待初次筹办首态 = L2状态身份{A1前态.目标节点};
    L2任务筹办轮次权威记录事实 P1;
    P1.身份 = L2任务筹办轮次权威记录身份{映射.P1节点};
    P1.任务 = 记录.任务;
    P1.任务虚拟存在 = 记录.任务虚拟存在; P1.需求列表项 = 记录.需求列表项;
    P1.首次准备记录 = 记录.身份; P1.权威规则版本 = L2任务筹办轮次权威规则版本;
    P1.建立事实代次 = G; P1.生命周期 = {G, std::nullopt};
    P1.任务轮次 = 记录.任务轮次; P1.正式存在 = 记录.正式存在;
    P1.阶段特征实例 = 记录.阶段特征实例; P1.待初次筹办首态 = 记录.待初次筹办首态;
    L2任务治理动作调用事实 A1;
    A1.身份 = L2任务治理动作调用身份{映射.A1节点};
    A1.被调用治理函数 = L2任务治理函数身份{A1函数.目标节点};
    A1.任务 = 记录.任务; A1.任务轮次 = *记录.任务轮次; A1.筹办轮次 = P1.身份;
    A1.触发前态 = *记录.待初次筹办首态; A1.来源共同事实截止 = (*A1材料)[0];
    A1.调用幂等来源 = L2结构幂等身份{(*A1材料)[1]};
    A1.函数合同版本 = static_cast<std::uint32_t>((*A1材料)[2]);
    A1.规则版本 = static_cast<std::uint32_t>((*A1材料)[3]); A1.生命周期 = {G, std::nullopt};
    结果.首次准备记录 = std::move(记录); 结果.P1 = std::move(P1);
    结果.初次筹办推进调用 = std::move(A1);
    return 结果.成功() ? 结果 : 失败(L2结构状态::内部不一致);
}

L2任务治理函数定义读取结果 读取治理函数定义投影(
    const L1事实基座服务& 第一层服务,
    const 任务身份来源定位& 来源,
    const 任务初次筹办治理定位& 治理,
    const L2任务治理函数定义读取请求& 请求) {
    const auto 观察 = 读取中性当前事实代次(第一层服务);
    const auto 截止 = 请求.读取类别 == L2读取类别::当前
        ? 观察 : 请求.历史截止事实代次;
    const auto 失败 = [&](L2结构状态 状态) noexcept {
        L2任务治理函数定义读取结果 结果;
        结果.结果头 = {L2结构合同版本, 状态, 截止, std::nullopt};
        return 结果;
    };
    if (观察 == 0 || 截止 == 0 || 截止 > 观察)
        return 失败(L2结构状态::入口拒绝);
    std::optional<L1所有者范围节点事实> 节点保存;
    const auto* 节点 = 读取初次筹办历史事实(
        第一层服务, 治理.治理函数定义节点, 节点保存);
    if (!节点 || 节点->写入所有者 != 来源.所有者
        || 节点->创建事实代次 == 0 || 节点->创建事实代次 > 截止
        || (节点->退出事实代次 && *节点->退出事实代次 <= 截止))
        return 失败(L2结构状态::未找到);
    const auto 属性 = 第一层服务.读取所有者范围历史属性值组(
        {L1所有者范围CRUD合同版本, 治理.治理函数定义节点, 截止});
    if (属性.状态 != L1所有者范围读取状态::成功
        || 属性.历史截止事实代次 != 截止 || 属性.读取事实代次 != 观察
        || 属性.属性值组.size() != 1)
        return 失败(L2结构状态::内部不一致);
    const auto& 值 = 属性.属性值组.front();
    const auto* 材料 = std::get_if<std::vector<std::uint64_t>>(&值.材料);
    if (值.写入所有者 != 来源.所有者 || 值.所属节点 != 治理.治理函数定义节点
        || 值.属性类型节点 != 治理.治理函数定义材料属性类型
        || 值.来源节点 != 治理.治理函数定义节点 || !材料 || 材料->size() != 3
        || (*材料)[0] != static_cast<std::uint64_t>(L2任务治理函数种类::初次筹办推进)
        || (*材料)[1] != L2任务治理函数合同版本
        || (*材料)[2] != L2初次筹办推进规则版本)
        return 失败(L2结构状态::内部不一致);
    L2任务治理函数定义读取结果 结果;
    结果.结果头 = {L2结构合同版本, L2结构状态::已读取, 截止, std::nullopt};
    L2任务治理函数定义事实 定义;
    定义.身份 = L2任务治理函数身份{治理.治理函数定义节点};
    定义.种类 = L2任务治理函数种类::初次筹办推进;
    定义.合同版本 = static_cast<std::uint32_t>((*材料)[1]);
    定义.规则版本 = static_cast<std::uint32_t>((*材料)[2]);
    定义.生命周期 = {节点->创建事实代次, std::nullopt};
    结果.定义 = std::move(定义);
    return 结果.成功() ? 结果 : 失败(L2结构状态::内部不一致);
}

L2任务治理动作调用读取结果 读取治理动作调用投影(
    const L1事实基座服务& 第一层服务,
    const 任务身份来源定位& 来源,
    const 任务初次筹办治理定位& 治理,
    const L2任务治理动作调用读取请求& 请求) {
    const auto 观察 = 读取中性当前事实代次(第一层服务);
    const auto 截止 = 请求.读取类别 == L2读取类别::当前
        ? 观察 : 请求.历史截止事实代次;
    const auto 失败 = [&](L2结构状态 状态) noexcept {
        L2任务治理动作调用读取结果 结果;
        结果.结果头 = {L2结构合同版本, 状态, 截止, std::nullopt};
        return 结果;
    };
    if (观察 == 0 || 截止 == 0 || 截止 > 观察 || !有效(请求.调用.值))
        return 失败(L2结构状态::入口拒绝);
    std::optional<L1所有者范围节点事实> 节点保存;
    const auto* 节点 = 读取初次筹办历史事实(第一层服务, 请求.调用.值, 节点保存);
    if (!节点 || 节点->写入所有者 != 来源.所有者
        || 节点->创建事实代次 == 0 || 节点->创建事实代次 > 截止
        || (节点->退出事实代次 && *节点->退出事实代次 <= 截止))
        return 失败(L2结构状态::未找到);
    const 稳定编码 类型[] = {治理.治理动作族归属关系类型,
        治理.治理动作函数关系类型, 治理.治理动作任务关系类型,
        治理.治理动作轮次关系类型, 治理.治理动作筹办轮次关系类型,
        治理.治理动作触发前态关系类型};
    稳定编码 目标[6]{};
    for (std::size_t i = 0; i < 6; ++i) {
        const auto 读取 = 第一层服务.读取所有者范围历史关系组({
            L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源,
            请求.调用.值, 类型[i], 截止});
        if (读取.状态 != L1所有者范围读取状态::成功
            || 读取.历史截止事实代次 != 截止 || 读取.读取事实代次 != 观察
            || 读取.关系组.size() != 1)
            return 失败(L2结构状态::内部不一致);
        const auto& 关系 = 读取.关系组.front();
        if (关系.写入所有者 != 来源.所有者 || 关系.源节点 != 请求.调用.值
            || 关系.关系类型节点 != 类型[i] || 关系.角色或顺序 != 1)
            return 失败(L2结构状态::内部不一致);
        目标[i] = 关系.目标节点;
    }
    if (目标[0] != 治理.治理动作调用锚点 || 目标[1] != 治理.治理函数定义节点)
        return 失败(L2结构状态::内部不一致);
    const auto 属性 = 第一层服务.读取所有者范围历史属性值组(
        {L1所有者范围CRUD合同版本, 请求.调用.值, 截止});
    if (属性.状态 != L1所有者范围读取状态::成功
        || 属性.历史截止事实代次 != 截止 || 属性.读取事实代次 != 观察
        || 属性.属性值组.size() != 1)
        return 失败(L2结构状态::内部不一致);
    const auto& 值 = 属性.属性值组.front();
    const auto* 材料 = std::get_if<std::vector<std::uint64_t>>(&值.材料);
    if (值.写入所有者 != 来源.所有者 || 值.所属节点 != 请求.调用.值
        || 值.属性类型节点 != 治理.治理动作调用材料属性类型
        || 值.来源节点 != 请求.调用.值 || !材料 || 材料->size() != 4
        || (*材料)[0] == 0 || (*材料)[1] == 0
        || (*材料)[2] != L2任务治理函数合同版本
        || (*材料)[3] != L2初次筹办推进规则版本)
        return 失败(L2结构状态::内部不一致);
    L2任务治理动作调用读取结果 结果;
    结果.结果头 = {L2结构合同版本, L2结构状态::已读取, 截止, std::nullopt};
    结果.调用 = L2任务治理动作调用事实{请求.调用,
        L2任务治理函数身份{目标[1]}, L2任务身份{目标[2]},
        L2任务轮次身份{目标[3]}, L2任务筹办轮次权威记录身份{目标[4]},
        L2状态身份{目标[5]}, (*材料)[0], L2结构幂等身份{(*材料)[1]},
        static_cast<std::uint32_t>((*材料)[2]),
        static_cast<std::uint32_t>((*材料)[3]), {节点->创建事实代次, std::nullopt}};
    return 结果.成功() ? 结果 : 失败(L2结构状态::内部不一致);
}

L2新增任务与首次初次筹办准备记录结果 读回初次筹办融合首次结果(
    const L1事实基座服务& 第一层服务,
    const 任务身份来源定位& 来源,
    const 任务结构类型定位& 类型,
    const 任务初次筹办准备记录定位& 记录定位,
    const 任务筹办轮次定位& 轮次定位,
    const 初次筹办融合写入编码映射& 映射,
    L2结构状态 返回状态,
    std::uint64_t G1) {
    const auto 失败 = [&](L2结构状态 状态) noexcept {
        L2新增任务与首次初次筹办准备记录结果 结果;
        结果.结果头 = {L2结构合同版本, 状态, G1, G1};
        return 结果;
    };
    if ((返回状态 != L2结构状态::已提交
            && 返回状态 != L2结构状态::精确重复)
        || G1 == 0)
        return 失败(L2结构状态::内部不一致);
    std::optional<L1所有者范围节点事实> 任务节点保存, 虚拟节点保存, 记录节点保存;
    const auto* 任务节点 = 读取初次筹办历史事实(
        第一层服务, 映射.任务节点, 任务节点保存);
    const auto* 虚拟节点 = 读取初次筹办历史事实(
        第一层服务, 映射.虚拟存在节点, 虚拟节点保存);
    const auto* 记录节点 = 读取初次筹办历史事实(
        第一层服务, 映射.记录节点, 记录节点保存);
    if (!任务节点 || !虚拟节点 || !记录节点
        || 任务节点->写入所有者 != 来源.所有者
        || 虚拟节点->写入所有者 != 来源.所有者
        || 记录节点->写入所有者 != 来源.所有者
        || 任务节点->创建事实代次 != G1
        || 虚拟节点->创建事实代次 != G1
        || 记录节点->创建事实代次 != G1
        || 记录节点->退出事实代次)
        return 失败(L2结构状态::内部不一致);
    const 稳定编码 关系编码[] = {映射.任务族归属关系,
        映射.任务需求列表项引用关系, 映射.任务虚拟存在归属关系,
        映射.记录族归属关系, 映射.记录任务关系,
        映射.记录任务虚拟存在关系, 映射.记录需求列表项关系};
    std::vector<L1所有者范围关系事实> 关系;
    关系.reserve(7);
    for (const auto 编码值 : 关系编码) {
        std::optional<L1所有者范围关系事实> 保存;
        const auto* 当前 = 读取初次筹办历史事实(第一层服务, 编码值, 保存);
        if (!当前 || 当前->写入所有者 != 来源.所有者
            || 当前->创建事实代次 != G1)
            return 失败(L2结构状态::内部不一致);
        关系.push_back(*当前);
    }
    const auto& 任务族 = 关系[0];
    const auto& 列表项引用 = 关系[1];
    const auto& 虚拟归属 = 关系[2];
    const auto& 记录族 = 关系[3];
    const auto& 记录任务 = 关系[4];
    const auto& 记录虚拟 = 关系[5];
    const auto& 记录列表项 = 关系[6];
    if (任务族.源节点 != 映射.任务节点
        || 任务族.目标节点 != 来源.任务锚点
        || 任务族.关系类型节点 != 来源.任务族归属关系类型
        || 列表项引用.源节点 != 映射.任务节点
        || 列表项引用.关系类型节点 != 类型.任务需求列表项引用关系类型
        || 虚拟归属.源节点 != 映射.任务节点
        || 虚拟归属.目标节点 != 映射.虚拟存在节点
        || 虚拟归属.关系类型节点 != 来源.虚拟存在归属关系类型
        || 记录族.源节点 != 映射.记录节点
        || 记录族.目标节点 != 记录定位.记录锚点
        || 记录族.关系类型节点
            != 记录定位.关系类型.记录族归属关系类型
        || 记录任务.源节点 != 映射.记录节点
        || 记录任务.目标节点 != 映射.任务节点
        || 记录任务.关系类型节点
            != 记录定位.关系类型.记录任务关系类型
        || 记录虚拟.源节点 != 映射.记录节点
        || 记录虚拟.目标节点 != 映射.虚拟存在节点
        || 记录虚拟.关系类型节点
            != 记录定位.关系类型.记录任务虚拟存在关系类型
        || 记录列表项.源节点 != 映射.记录节点
        || 记录列表项.目标节点 != 列表项引用.目标节点
        || 记录列表项.关系类型节点
            != 记录定位.关系类型.记录需求列表项关系类型)
        return 失败(L2结构状态::内部不一致);
    const 稳定编码 值编码[] = {映射.完整请求值,
        映射.首次裁决值, 映射.筹办轮次值};
    std::vector<L1所有者范围值事实> 值;
    值.reserve(3);
    for (const auto 编码值 : 值编码) {
        std::optional<L1所有者范围值事实> 保存;
        const auto* 当前 = 读取初次筹办历史事实(第一层服务, 编码值, 保存);
        if (!当前 || 当前->写入所有者 != 来源.所有者
            || 当前->所属节点 != 映射.记录节点
            || 当前->来源节点 != 映射.记录节点
            || 当前->创建事实代次 != G1 || 当前->退出事实代次)
            return 失败(L2结构状态::内部不一致);
        值.push_back(*当前);
    }
    if (值[0].属性类型节点 != 记录定位.完整请求属性类型
        || 值[1].属性类型节点 != 记录定位.首次裁决属性类型
        || 值[2].属性类型节点 != 记录定位.筹办轮次属性类型)
        return 失败(L2结构状态::内部不一致);
    const auto 完整请求 = 解码初次筹办完整请求(值[0].材料);
    const auto 首次裁决 = 解码初次筹办首次裁决(值[1].材料);
    const auto* 轮次 = std::get_if<std::vector<std::uint64_t>>(&值[2].材料);
    if (!完整请求
        || 完整请求->状态 != L2任务初次筹办请求材料状态::当前版本
        || !完整请求->材料 || !首次裁决 || !轮次
        || 轮次->size() != 1 || 轮次->front() != 1
        || 完整请求->材料->请求头.期望事实代次 >= G1
        || 首次裁决->所属列表项.身份.值 != 列表项引用.目标节点)
        return 失败(L2结构状态::内部不一致);
    L2任务初次筹办准备记录事实 记录事实;
    记录事实.身份 = L2任务初次筹办准备记录身份{映射.记录节点};
    记录事实.完整请求 = *完整请求->材料;
    记录事实.首次需求裁决 = *首次裁决;
    记录事实.任务 = L2任务身份{映射.任务节点};
    记录事实.任务虚拟存在 = L2任务虚拟存在身份{映射.虚拟存在节点};
    记录事实.需求列表项 = L2需求列表项身份{列表项引用.目标节点};
    记录事实.筹办轮次 = 1;
    记录事实.首次任务建立代次G1 = G1;
    记录事实.生命周期 = {G1, std::nullopt};
    L2任务事实 任务事实{记录事实.任务, 记录事实.需求列表项,
        记录事实.任务虚拟存在, {G1, std::nullopt}};
    L2任务身份来源事实 身份来源{来源.所有者, 映射.任务节点,
        来源.任务锚点, 来源.任务族归属关系类型,
        映射.任务族归属关系, {G1, std::nullopt}, {G1, std::nullopt}};
    if (!L2任务初次筹办准备记录事实完整(记录事实)
        || !L2任务身份来源事实完整(身份来源))
        return 失败(L2结构状态::内部不一致);
    L2新增任务与首次初次筹办准备记录结果 结果;
    结果.结果头 = {L2结构合同版本, 返回状态, G1, G1};
    if (映射.统一轮次写入()) {
        std::optional<L1所有者范围节点事实> 权威节点保存;
        const auto* 权威节点 = 读取初次筹办历史事实(
            第一层服务, 映射.轮次权威节点, 权威节点保存);
        if (!权威节点 || 权威节点->写入所有者 != 来源.所有者
            || 权威节点->创建事实代次 != G1 || 权威节点->退出事实代次)
            return 失败(L2结构状态::内部不一致);
        const 稳定编码 权威关系编码[] = {映射.权威族关系,
            映射.权威任务关系, 映射.权威虚拟存在关系,
            映射.权威列表项关系, 映射.权威首次记录关系,
            映射.当前筹办准备关系};
        std::vector<L1所有者范围关系事实> 权威关系;
        权威关系.reserve(6);
        for (const auto 编码值 : 权威关系编码) {
            std::optional<L1所有者范围关系事实> 保存;
            const auto* 当前 = 读取初次筹办历史事实(
                第一层服务, 编码值, 保存);
            if (!当前 || 当前->写入所有者 != 来源.所有者
                || 当前->创建事实代次 != G1)
                return 失败(L2结构状态::内部不一致);
            权威关系.push_back(*当前);
        }
        const auto& 权威族 = 权威关系[0];
        const auto& 权威任务 = 权威关系[1];
        const auto& 权威虚拟 = 权威关系[2];
        const auto& 权威列表项 = 权威关系[3];
        const auto& 权威首次 = 权威关系[4];
        const auto& 当前准备 = 权威关系[5];
        if (权威族.源节点 != 映射.轮次权威节点
            || 权威族.目标节点 != 轮次定位.权威记录锚点
            || 权威族.关系类型节点 != 轮次定位.关系类型[0]
            || 权威任务.源节点 != 映射.轮次权威节点
            || 权威任务.目标节点 != 映射.任务节点
            || 权威任务.关系类型节点 != 轮次定位.关系类型[1]
            || 权威虚拟.源节点 != 映射.轮次权威节点
            || 权威虚拟.目标节点 != 映射.虚拟存在节点
            || 权威虚拟.关系类型节点 != 轮次定位.关系类型[2]
            || 权威列表项.源节点 != 映射.轮次权威节点
            || 权威列表项.目标节点 != 列表项引用.目标节点
            || 权威列表项.关系类型节点 != 轮次定位.关系类型[3]
            || 权威首次.源节点 != 映射.轮次权威节点
            || 权威首次.目标节点 != 映射.记录节点
            || 权威首次.关系类型节点 != 轮次定位.关系类型[4]
            || 当前准备.源节点 != 映射.任务节点
            || 当前准备.目标节点 != 映射.记录节点
            || 当前准备.关系类型节点 != 轮次定位.关系类型[17]
            || 当前准备.角色或顺序 != 1)
            return 失败(L2结构状态::内部不一致);
        std::optional<L1所有者范围值事实> 权威版本保存;
        const auto* 权威版本 = 读取初次筹办历史事实(
            第一层服务, 映射.权威版本值, 权威版本保存);
        const auto* 权威版本材料 = 权威版本
            ? std::get_if<std::vector<std::uint64_t>>(&权威版本->材料) : nullptr;
        if (!权威版本 || 权威版本->所属节点 != 映射.轮次权威节点
            || 权威版本->属性类型节点 != 轮次定位.属性类型[0]
            || !权威版本材料 || 权威版本材料->size() != 1
            || 权威版本材料->front() != L2任务筹办轮次权威规则版本)
            return 失败(L2结构状态::内部不一致);
        L2任务筹办轮次权威记录事实 权威事实;
        权威事实.身份 = L2任务筹办轮次权威记录身份{映射.轮次权威节点};
        权威事实.任务 = 记录事实.任务;
        权威事实.任务虚拟存在 = 记录事实.任务虚拟存在;
        权威事实.需求列表项 = 记录事实.需求列表项;
        权威事实.首次准备记录 = 记录事实.身份;
        权威事实.权威规则版本 = L2任务筹办轮次权威规则版本;
        权威事实.建立事实代次 = G1;
        权威事实.生命周期 = {G1, std::nullopt};
        if (!L2任务筹办轮次权威记录事实完整(权威事实))
            return 失败(L2结构状态::内部不一致);
        结果.轮次分区 = L2任务筹办轮次分区::统一轮次权威;
        结果.轮次权威 = std::move(权威事实);
    } else {
        结果.轮次分区 = L2任务筹办轮次分区::遗留实例轮次;
    }
    结果.首次准备记录 = std::move(记录事实);
    结果.任务 = std::move(任务事实);
    结果.身份来源 = std::move(身份来源);
    return 结果;
}

L2按初次筹办准备幂等身份读取首次记录结果
按初次筹办准备幂等身份读取首次记录投影(
    const L1事实基座服务& 第一层服务,
    const L1所有者范围写端口& 写入端口,
    const 任务身份来源定位& 来源,
    const 任务结构类型定位& 类型,
    const 任务初次筹办准备记录定位& 记录定位,
    const 任务筹办轮次定位& 轮次定位,
    const 任务初次筹办治理定位& 治理定位,
    const L2按初次筹办准备幂等身份读取首次记录请求& 请求,
    std::uint64_t 截止) {
    const auto 失败 = [&](L2结构状态 状态) noexcept {
        L2按初次筹办准备幂等身份读取首次记录结果 结果;
        结果.结果头 = {L2结构合同版本, 状态, 截止, std::nullopt};
        return 结果;
    };
    const auto 首次 = 写入端口.读取首次写入材料(
        {L1所有者范围首次写入读取合同版本,
            {请求.准备幂等身份}});
    if (首次.状态 == L1所有者范围读取状态::未找到)
        return 失败(L2结构状态::已读取);
    if (首次.状态 != L1所有者范围读取状态::成功
        || 首次.合同版本 != L1所有者范围首次写入读取合同版本
        || 首次.所有者 != 来源.所有者
        || 首次.写入幂等身份.值 != 请求.准备幂等身份
        || 首次.读取事实代次 != 截止
        || !首次.首次规范化写集 || !首次.首次写入结果)
        return 失败(映射子目标记录读取状态(首次.状态));
    const auto 新映射 = 查找首次P1治理写入编码映射(*首次.首次写入结果);
    const auto 旧映射 = 查找初次筹办融合写入编码映射(*首次.首次写入结果);
    if (!新映射 && !旧映射) {
        const auto 普通任务 = 查找任务写入编码映射(*首次.首次写入结果);
        return 普通任务 ? 失败(L2结构状态::已读取)
            : 失败(L2结构状态::内部不一致);
    }
    std::optional<初次筹办完整请求解码结果> 原请求;
    for (const auto& 值 : 首次.首次规范化写集->值) {
        if (值.本地键.值 != 初次筹办记录值写入起始键) continue;
        if (原请求) return 失败(L2结构状态::内部不一致);
        原请求 = 解码初次筹办完整请求(值.材料);
    }
    if (!原请求) return 失败(L2结构状态::内部不一致);
    if (原请求->状态
        == L2任务初次筹办请求材料状态::初次请求版本待迁移) {
        auto 结果 = 失败(L2结构状态::已读取);
        结果.材料状态 =
            L2任务初次筹办请求材料状态::初次请求版本待迁移;
        return 结果;
    }
    if (原请求->状态 != L2任务初次筹办请求材料状态::当前版本
        || !原请求->材料)
        return 失败(L2结构状态::内部不一致);
    L2按初次筹办准备幂等身份读取首次记录结果 结果;
    结果.结果头 = {L2结构合同版本, L2结构状态::已读取,
        截止, std::nullopt};
    结果.材料状态 = L2任务初次筹办请求材料状态::当前版本;
    if (新映射) {
        const auto 首次结果 = 读回首次P1治理(第一层服务, 来源,
            记录定位, 轮次定位, 治理定位, *新映射,
            L2结构状态::精确重复, 首次.首次写入结果->事实代次);
        if (!首次结果.成功() || !首次结果.首次准备记录)
            return 失败(首次结果.结果头.状态);
        const auto 任务 = 读取任务当前投影(第一层服务, 来源, 类型,
            {{L2结构合同版本, 截止}, L2读取类别::当前,
                首次结果.首次准备记录->任务, 0}, 截止);
        if (!任务.成功() || !任务.任务 || !任务.身份来源)
            return 失败(L2结构状态::内部不一致);
        结果.首次准备记录 = std::move(首次结果.首次准备记录);
        结果.P1 = std::move(首次结果.P1);
        结果.初次筹办推进调用 =
            std::move(首次结果.初次筹办推进调用);
        结果.首次任务 = std::move(任务.任务);
        结果.首次任务身份来源 = std::move(任务.身份来源);
    } else {
        const auto 首次结果 = 读回初次筹办融合首次结果(第一层服务,
            来源, 类型, 记录定位, 轮次定位, *旧映射,
            L2结构状态::精确重复, 首次.首次写入结果->事实代次);
        if (!首次结果.成功()) return 失败(首次结果.结果头.状态);
        结果.首次准备记录 = std::move(首次结果.首次准备记录);
        结果.首次任务 = std::move(首次结果.任务);
        结果.首次任务身份来源 = std::move(首次结果.身份来源);
    }
    return 结果;
}

std::optional<L2任务筹办轮次权威记录事实> 读取筹办轮次权威历史投影(
    const L1事实基座服务& 第一层服务,
    const 任务筹办轮次定位& 定位,
    L2任务筹办轮次权威记录身份 身份,
    std::uint64_t 截止, std::uint64_t 观察代次) {
    const auto 节点读取 = 第一层服务.读取所有者范围历史事实(
        {L1所有者范围CRUD合同版本, 身份.值});
    const auto* 节点 = 节点读取.事实
        ? std::get_if<L1所有者范围节点事实>(&*节点读取.事实) : nullptr;
    if (节点读取.状态 != L1所有者范围读取状态::成功 || !节点
        || 节点->写入所有者 != 定位.所有者
        || 节点->创建事实代次 == 0 || 节点->创建事实代次 > 截止
        || (节点->退出事实代次 && *节点->退出事实代次 <= 截止))
        return std::nullopt;
    const auto 读唯一关系 = [&](std::size_t 索引) -> std::optional<稳定编码> {
        const auto 读取 = 第一层服务.读取所有者范围历史关系组({
            L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源,
            身份.值, 定位.关系类型[索引], 截止});
        if (读取.状态 != L1所有者范围读取状态::成功
            || 读取.历史截止事实代次 != 截止
            || 读取.读取事实代次 != 观察代次 || 读取.关系组.size() != 1)
            return std::nullopt;
        const auto& 关系 = 读取.关系组.front();
        if (关系.写入所有者 != 定位.所有者 || 关系.源节点 != 身份.值
            || 关系.关系类型节点 != 定位.关系类型[索引]
            || 关系.角色或顺序 != 1)
            return std::nullopt;
        return 关系.目标节点;
    };
    const auto 族 = 读唯一关系(0);
    const auto 任务 = 读唯一关系(1);
    const auto 虚拟 = 读唯一关系(2);
    const auto 列表项 = 读唯一关系(3);
    const auto 首次 = 读唯一关系(4);
    const auto 值读取 = 第一层服务.读取所有者范围历史属性值组(
        {L1所有者范围CRUD合同版本, 身份.值, 截止});
    if (!族 || *族 != 定位.权威记录锚点 || !任务 || !虚拟 || !列表项
        || !首次 || 值读取.状态 != L1所有者范围读取状态::成功
        || 值读取.历史截止事实代次 != 截止
        || 值读取.读取事实代次 != 观察代次
        || 值读取.属性值组.size() != 1)
        return std::nullopt;
    const auto& 版本值 = 值读取.属性值组.front();
    const auto* 版本 = std::get_if<std::vector<std::uint64_t>>(&版本值.材料);
    if (版本值.属性类型节点 != 定位.属性类型[0]
        || !版本 || 版本->size() != 1)
        return std::nullopt;
    L2任务筹办轮次权威记录事实 事实;
    事实.身份 = 身份;
    事实.任务 = L2任务身份{*任务};
    事实.任务虚拟存在 = L2任务虚拟存在身份{*虚拟};
    事实.需求列表项 = L2需求列表项身份{*列表项};
    事实.首次准备记录 = L2任务初次筹办准备记录身份{*首次};
    事实.权威规则版本 = static_cast<std::uint32_t>(版本->front());
    事实.建立事实代次 = 节点->创建事实代次;
    事实.生命周期 = {节点->创建事实代次, 节点->退出事实代次};
    return L2任务筹办轮次权威记录事实完整(事实)
        ? std::optional{事实} : std::nullopt;
}

std::optional<L2任务后继筹办准备记录事实> 读取后继筹办准备历史投影(
    const L1事实基座服务& 第一层服务,
    const 任务筹办轮次定位& 定位,
    L2任务后继筹办准备记录身份 身份,
    std::uint64_t 截止, std::uint64_t 观察代次) {
    const auto 节点读取 = 第一层服务.读取所有者范围历史事实(
        {L1所有者范围CRUD合同版本, 身份.值});
    const auto* 节点 = 节点读取.事实
        ? std::get_if<L1所有者范围节点事实>(&*节点读取.事实) : nullptr;
    if (节点读取.状态 != L1所有者范围读取状态::成功 || !节点
        || 节点->写入所有者 != 定位.所有者
        || 节点->创建事实代次 == 0 || 节点->创建事实代次 > 截止
        || (节点->退出事实代次 && *节点->退出事实代次 <= 截止))
        return std::nullopt;
    const auto 读关系 = [&](std::size_t 索引) {
        return 第一层服务.读取所有者范围历史关系组({
            L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源,
            身份.值, 定位.关系类型[索引], 截止});
    };
    const auto 读唯一 = [&](std::size_t 索引) -> std::optional<稳定编码> {
        const auto 读取 = 读关系(索引);
        if (读取.状态 != L1所有者范围读取状态::成功
            || 读取.历史截止事实代次 != 截止
            || 读取.读取事实代次 != 观察代次 || 读取.关系组.size() != 1)
            return std::nullopt;
        const auto& 关系 = 读取.关系组.front();
        if (关系.写入所有者 != 定位.所有者 || 关系.源节点 != 身份.值
            || 关系.关系类型节点 != 定位.关系类型[索引]
            || 关系.角色或顺序 != 1)
            return std::nullopt;
        return 关系.目标节点;
    };
    const auto 族 = 读唯一(5);
    const auto 任务 = 读唯一(6);
    const auto 虚拟 = 读唯一(7);
    const auto 列表项 = 读唯一(8);
    const auto 权威 = 读唯一(9);
    const auto 首次 = 读唯一(10);
    const auto 前一组 = 读关系(11);
    const auto 触发 = 读唯一(12);
    const auto 读可选关系 = [&](std::size_t 索引,
        std::optional<稳定编码>& 目标) noexcept {
        const auto 读取 = 读关系(索引);
        if (读取.状态 != L1所有者范围读取状态::成功
            || 读取.历史截止事实代次 != 截止
            || 读取.读取事实代次 != 观察代次
            || 读取.关系组.size() > 1)
            return false;
        if (读取.关系组.empty()) return true;
        const auto& 关系 = 读取.关系组.front();
        if (关系.写入所有者 != 定位.所有者
            || 关系.源节点 != 身份.值
            || 关系.关系类型节点 != 定位.关系类型[索引]
            || 关系.角色或顺序 != 1)
            return false;
        目标 = 关系.目标节点;
        return true;
    };
    std::optional<稳定编码> 旧路径;
    std::optional<稳定编码> 旧实例;
    std::optional<稳定编码> 裁决;
    std::optional<稳定编码> 正式状态;
    const bool 旧引用关系有效 = 读可选关系(13, 旧路径)
        && 读可选关系(14, 旧实例)
        && 读可选关系(15, 裁决)
        && 读可选关系(16, 正式状态);
    if (!族 || *族 != 定位.后继记录锚点 || !任务 || !虚拟 || !列表项
        || !权威 || !首次 || !触发 || !旧引用关系有效
        || 前一组.状态 != L1所有者范围读取状态::成功
        || 前一组.历史截止事实代次 != 截止
        || 前一组.读取事实代次 != 观察代次 || 前一组.关系组.size() > 1)
        return std::nullopt;
    std::optional<L2任务后继筹办准备记录身份> 前一;
    if (!前一组.关系组.empty()) {
        const auto& 关系 = 前一组.关系组.front();
        if (关系.写入所有者 != 定位.所有者 || 关系.源节点 != 身份.值
            || 关系.关系类型节点 != 定位.关系类型[11]
            || 关系.角色或顺序 != 1)
            return std::nullopt;
        前一 = L2任务后继筹办准备记录身份{关系.目标节点};
    }
    const auto 值读取 = 第一层服务.读取所有者范围历史属性值组(
        {L1所有者范围CRUD合同版本, 身份.值, 截止});
    if (值读取.状态 != L1所有者范围读取状态::成功
        || 值读取.历史截止事实代次 != 截止
        || 值读取.读取事实代次 != 观察代次
        || 值读取.属性值组.size() != 5)
        return std::nullopt;
    const auto 取材料 = [&](std::size_t 属性索引)
        -> const std::vector<std::uint64_t>* {
        const L1所有者范围值事实* 找到 = nullptr;
        for (const auto& 值 : 值读取.属性值组) {
            if (值.属性类型节点 != 定位.属性类型[属性索引]) continue;
            if (找到) return nullptr;
            找到 = std::addressof(值);
        }
        return 找到
            ? std::get_if<std::vector<std::uint64_t>>(&找到->材料) : nullptr;
    };
    const auto* 触发类型 = 取材料(1);
    const auto* 触发材料 = 取材料(2);
    const auto* 轮次材料 = 取材料(3);
    const auto* 代次材料 = 取材料(4);
    const auto* 幂等材料 = 取材料(5);
    if (!触发类型 || 触发类型->size() != 1
        || !触发材料 || 触发材料->size() != 3
        || !轮次材料 || 轮次材料->size() != 2
        || !代次材料 || 代次材料->size() != 2
        || !幂等材料 || 幂等材料->size() != 1)
        return std::nullopt;
    const auto 触发类型值 = static_cast<L2任务后继筹办触发类型>(
        触发类型->front());
    const bool 旧引用形状 =
        (触发类型值 == L2任务后继筹办触发类型::实际结果未达成
            && 旧路径 && 旧实例 && 裁决 && 正式状态)
        || (触发类型值 == L2任务后继筹办触发类型::子目标回流
            && !旧路径 && !旧实例 && !裁决 && !正式状态);
    if (!旧引用形状) return std::nullopt;

    L2任务后继筹办准备记录事实 事实;
    事实.身份 = 身份;
    事实.轮次权威 = L2任务筹办轮次权威记录身份{*权威};
    事实.任务 = L2任务身份{*任务};
    事实.任务虚拟存在 = L2任务虚拟存在身份{*虚拟};
    事实.需求列表项 = L2需求列表项身份{*列表项};
    事实.首次准备记录 = L2任务初次筹办准备记录身份{*首次};
    事实.前一后继准备记录 = 前一;
    事实.触发类型 = 触发类型值;
    事实.触发事实 = *触发;
    if (旧路径) 事实.旧路径 = L2任务方法路径身份{*旧路径};
    if (旧实例) 事实.旧实例 = L2实例方法身份{*旧实例};
    if (裁决) 事实.目标裁决证据 = L2任务目标裁决证据身份{*裁决};
    if (正式状态)
        事实.正式待重筹办状态 = L2任务治理状态身份{*正式状态};
    事实.触发请求身份 = (*触发材料)[0];
    事实.运行代次 = (*触发材料)[1];
    事实.触发业务幂等身份 = L2结构幂等身份{(*触发材料)[2]};
    事实.前一筹办轮次 = (*轮次材料)[0];
    事实.新筹办轮次 = (*轮次材料)[1];
    事实.来源共同事实截止 = (*代次材料)[0];
    事实.准备发布事实代次 = (*代次材料)[1];
    事实.准备幂等身份 = L2结构幂等身份{幂等材料->front()};
    事实.生命周期 = {节点->创建事实代次, 节点->退出事实代次};
    return L2任务后继筹办准备记录事实完整(事实)
        ? std::optional{事实} : std::nullopt;
}

L2按任务读取筹办轮次分区结果 读取筹办轮次分区投影(
    const L1事实基座服务& 第一层服务,
    const L1所有者范围写端口& 写入端口,
    const 任务身份来源定位& 来源,
    const 任务结构类型定位& 类型,
    const 任务初次筹办准备记录定位& 初次定位,
    const 任务筹办轮次定位& 轮次定位,
    const 任务初次筹办治理定位& 治理定位,
    const L2按任务读取筹办轮次分区请求& 请求,
    std::uint64_t 截止, std::uint64_t 观察代次) {
    L2按任务读取筹办轮次分区结果 结果;
    结果.结果头 = {L2结构合同版本, L2结构状态::已读取,
        观察代次, std::nullopt};
    const auto 读反向组 = [&](std::size_t 索引) {
        return 第一层服务.读取所有者范围历史关系组({
            L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::目标,
            请求.任务.值, 轮次定位.关系类型[索引], 截止});
    };
    const auto 权威组 = 读反向组(1);
    const auto 后继组 = 读反向组(6);
    const auto 当前组 = 第一层服务.读取所有者范围历史关系组({
        L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源,
        请求.任务.值, 轮次定位.关系类型[17], 截止});
    const auto 组有效 = [&](const auto& 读取) noexcept {
        return 读取.状态 == L1所有者范围读取状态::成功
            && 读取.历史截止事实代次 == 截止
            && 读取.读取事实代次 == 观察代次;
    };
    if (!组有效(权威组) || !组有效(后继组) || !组有效(当前组)) {
        结果.分区 = L2任务筹办轮次分区::内部不一致;
        结果.结果头.状态 = L2结构状态::内部不一致;
        return 结果;
    }
    if (权威组.关系组.empty() && 后继组.关系组.empty()
        && 当前组.关系组.empty()) {
        const auto 任务读取 = 读取任务当前投影(第一层服务, 来源, 类型,
            {{L2结构合同版本, 观察代次}, L2读取类别::历史,
                请求.任务, 截止}, 截止);
        if (!任务读取.成功() || !任务读取.任务) {
            结果.分区 = L2任务筹办轮次分区::内部不一致;
            结果.结果头.状态 = L2结构状态::未找到;
            return 结果;
        }
        结果.分区 = L2任务筹办轮次分区::遗留实例轮次;
        return 结果;
    }
    if (权威组.关系组.size() != 1 || 当前组.关系组.size() != 1) {
        结果.分区 = L2任务筹办轮次分区::内部不一致;
        结果.结果头.状态 = L2结构状态::内部不一致;
        return 结果;
    }
    const auto& 权威关系 = 权威组.关系组.front();
    const auto& 当前关系 = 当前组.关系组.front();
    if (权威关系.目标节点 != 请求.任务.值
        || 权威关系.关系类型节点 != 轮次定位.关系类型[1]
        || 当前关系.源节点 != 请求.任务.值
        || 当前关系.关系类型节点 != 轮次定位.关系类型[17]
        || 当前关系.角色或顺序 != 1) {
        结果.分区 = L2任务筹办轮次分区::内部不一致;
        结果.结果头.状态 = L2结构状态::内部不一致;
        return 结果;
    }
    const auto 权威 = 读取筹办轮次权威历史投影(第一层服务,
        轮次定位, L2任务筹办轮次权威记录身份{权威关系.源节点},
        截止, 观察代次);
    if (!权威 || 权威->任务 != 请求.任务) {
        结果.分区 = L2任务筹办轮次分区::内部不一致;
        结果.结果头.状态 = L2结构状态::内部不一致;
        return 结果;
    }
    const auto 值读取 = 第一层服务.读取所有者范围历史属性值组(
        {L1所有者范围CRUD合同版本, 权威->首次准备记录.值, 截止});
    std::optional<初次筹办完整请求解码结果> 完整请求;
    if (值读取.状态 == L1所有者范围读取状态::成功
        && 值读取.历史截止事实代次 == 截止
        && 值读取.读取事实代次 == 观察代次) {
        for (const auto& 值 : 值读取.属性值组) {
            if (值.属性类型节点 == 初次定位.完整请求属性类型)
                完整请求 = 解码初次筹办完整请求(值.材料);
        }
    }
    if (!完整请求) {
        结果.分区 = L2任务筹办轮次分区::内部不一致;
        结果.结果头.状态 = L2结构状态::内部不一致;
        return 结果;
    }
    if (完整请求->状态
        == L2任务初次筹办请求材料状态::初次请求版本待迁移) {
        结果.分区 = L2任务筹办轮次分区::初次请求版本待迁移;
        结果.结果头.状态 = L2结构状态::已读取;
        return 结果;
    }
    if (完整请求->状态 != L2任务初次筹办请求材料状态::当前版本
        || !完整请求->材料) {
        结果.分区 = L2任务筹办轮次分区::内部不一致;
        结果.结果头.状态 = L2结构状态::内部不一致;
        return 结果;
    }
    const auto 首次读取 = 按初次筹办准备幂等身份读取首次记录投影(
        第一层服务, 写入端口, 来源, 类型, 初次定位, 轮次定位, 治理定位,
        {{L2结构合同版本, 观察代次},
            完整请求->材料->准备幂等身份},
        观察代次);
    if (!首次读取.成功() || !首次读取.首次准备记录 || !首次读取.首次任务
        || 首次读取.首次准备记录->身份 != 权威->首次准备记录
        || 首次读取.首次准备记录->首次任务建立代次G1 > 截止) {
        结果.分区 = L2任务筹办轮次分区::内部不一致;
        结果.结果头.状态 = L2结构状态::内部不一致;
        return 结果;
    }
    L2任务当前筹办准备事实 当前;
    当前.轮次权威 = *权威;
    当前.任务 = *首次读取.首次任务;
    当前.首次准备记录 = *首次读取.首次准备记录;
    if (当前关系.目标节点 == 权威->首次准备记录.值) {
        if (!后继组.关系组.empty()) {
            结果.分区 = L2任务筹办轮次分区::内部不一致;
            结果.结果头.状态 = L2结构状态::内部不一致;
            return 结果;
        }
        当前.当前筹办轮次 = 1;
    } else {
        const auto 后继 = 读取后继筹办准备历史投影(第一层服务,
            轮次定位, L2任务后继筹办准备记录身份{当前关系.目标节点},
            截止, 观察代次);
        if (!后继 || 后继->任务 != 请求.任务
            || 后继->轮次权威 != 权威->身份
            || 后继->首次准备记录 != 权威->首次准备记录
            || 后继组.关系组.empty()) {
            结果.分区 = L2任务筹办轮次分区::内部不一致;
            结果.结果头.状态 = L2结构状态::内部不一致;
            return 结果;
        }
        auto 链节点 = *后继;
        while (链节点.新筹办轮次 > 2) {
            if (!链节点.前一后继准备记录) {
                结果.分区 = L2任务筹办轮次分区::内部不一致;
                结果.结果头.状态 = L2结构状态::内部不一致;
                return 结果;
            }
            const auto 前一 = 读取后继筹办准备历史投影(第一层服务,
                轮次定位, *链节点.前一后继准备记录, 截止, 观察代次);
            if (!前一 || 前一->新筹办轮次 + 1 != 链节点.新筹办轮次
                || 前一->轮次权威 != 权威->身份
                || 前一->首次准备记录 != 权威->首次准备记录) {
                结果.分区 = L2任务筹办轮次分区::内部不一致;
                结果.结果头.状态 = L2结构状态::内部不一致;
                return 结果;
            }
            链节点 = *前一;
        }
        if (链节点.新筹办轮次 != 2 || 链节点.前一后继准备记录) {
            结果.分区 = L2任务筹办轮次分区::内部不一致;
            结果.结果头.状态 = L2结构状态::内部不一致;
            return 结果;
        }
        当前.当前后继准备记录 = *后继;
        当前.当前筹办轮次 = 后继->新筹办轮次;
    }
    if (!L2任务当前筹办准备事实完整(当前)) {
        结果.分区 = L2任务筹办轮次分区::内部不一致;
        结果.结果头.状态 = L2结构状态::内部不一致;
        return 结果;
    }
    结果.分区 = L2任务筹办轮次分区::统一轮次权威;
    结果.统一当前准备 = std::move(当前);
    return 结果;
}

} // namespace 海中鱼巣::L2任务结构内部

export namespace 海中鱼巣 {

// ===== 任务结构服务 =====
class L2任务结构服务 final {
public:
    explicit L2任务结构服务(const L1事实基座服务& 第一层服务,
        const L2需求结构服务& 需求结构服务,
        L2任务所有者交付&& 交付)
        : L1_(第一层服务), 需求结构服务_(需求结构服务),
          第一层写入端口_(验证并移动交付(第一层服务, 交付)),
          身份来源定位_(L2任务结构内部::初始化任务身份来源(
              L1_, 第一层写入端口_)),
          类型定位_(L2任务结构内部::初始化任务结构类型(
              L1_, 第一层写入端口_, 身份来源定位_)),
          任务轮次核心定位_(
              L2任务结构内部::初始化任务轮次核心登记(
                  L1_, 第一层写入端口_)),
          初次筹办治理定位_(
              L2任务结构内部::初始化任务初次筹办治理登记(
                  L1_, 第一层写入端口_, 类型定位_,
                  任务轮次核心定位_.任务轮次锚点,
                  任务轮次核心定位_.任务轮次族归属关系类型,
                  任务轮次核心定位_.任务轮次任务关系类型,
                  任务轮次核心定位_.任务当前轮次关系类型,
                  任务轮次核心定位_.任务轮次序号属性类型)),
          轮次结算定位_(
              L2任务结构内部::初始化任务轮次结算登记(
                  L1_, 第一层写入端口_, 初次筹办治理定位_)),
          初次筹办记录定位_(
              L2任务结构内部::初始化任务初次筹办准备记录登记(
                  L1_, 第一层写入端口_)),
          筹办轮次定位_(
              L2任务结构内部::初始化任务筹办轮次登记(
                  L1_, 第一层写入端口_)),
          子目标记录定位_(
              L2任务结构内部::初始化任务子目标承接记录登记(
                  L1_, 第一层写入端口_)),
          正式选择定位_(
              L2任务结构内部::初始化任务正式选择登记(
                  L1_, 第一层写入端口_)) {
        L2任务结构内部::初始化任务实际结果登记(
            L1_, 第一层写入端口_, 身份来源定位_, 类型定位_);
        L2任务结构内部::初始化任务治理状态登记(
            L1_, 第一层写入端口_, 身份来源定位_, 类型定位_);
    }

    L2任务轮次结算结果 建立或读取任务轮次结算(
        const L2任务轮次结算请求& 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        return 建立或读取任务轮次结算_已锁定(请求);
    }

    L2任务轮次结算读取结果 读取任务轮次结算(
        const L2任务轮次结算读取请求& 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        return 读取任务轮次结算_已锁定(请求);
    }

#ifdef ARCH_L4_SELF_GOVERNANCE_TEST_SEAM
    void ARCH_注入任务实际结果提交后首次读回失败一次() noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        ARCH_任务实际结果提交后首次读回失败一次_ = true;
    }

    std::optional<L2任务来源需求关系事实>
    ARCH_追加任务来源需求关系(
        const L2结构请求头& 请求头,
        L2结构幂等身份 幂等身份,
        L2任务身份 任务,
        L2需求身份 来源需求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        if (!L2结构请求头合同有效(请求头)
            || 请求头.期望事实代次 == 0
            || !L2结构幂等身份有效(幂等身份)
            || !有效(任务.值) || !有效(来源需求.值))
            return std::nullopt;
        try {
            constexpr std::uint32_t 关系本地键值 = 0x0301'2F01U;
            L1所有者范围写集请求 写集;
            写集.合同版本 = L1所有者范围CRUD合同版本;
            写集.期望事实代次 = 请求头.期望事实代次;
            写集.写入幂等身份 = {幂等身份.值};
            写集.关系 = {{{关系本地键值}, 任务.值, 来源需求.值,
                类型定位_.任务来源需求关系类型, 1}};
            const auto 写入 = 第一层写入端口_.提交所有者范围中性写集(写集);
            if ((写入.状态 != L1所有者范围写入状态::成功
                    && 写入.状态 != L1所有者范围写入状态::精确重复)
                || 写入.事实代次 == 0)
                return std::nullopt;
            稳定编码 关系编码{};
            for (const auto& [本地键, 编码] : 写入.新编码映射)
                if (本地键.值 == 关系本地键值) 关系编码 = 编码;
            if (!有效(关系编码)) return std::nullopt;
            const auto 读回 = L1_.读取所有者范围历史事实({
                L1所有者范围CRUD合同版本, 关系编码});
            const auto* 关系 = 读回.事实
                ? std::get_if<L1所有者范围关系事实>(&*读回.事实) : nullptr;
            if (读回.状态 != L1所有者范围读取状态::成功 || !关系
                || 关系->编码 != 关系编码
                || 关系->写入所有者 != 身份来源定位_.所有者
                || 关系->源节点 != 任务.值 || 关系->目标节点 != 来源需求.值
                || 关系->关系类型节点 != 类型定位_.任务来源需求关系类型
                || 关系->角色或顺序 != 1
                || 关系->创建事实代次 != 写入.事实代次
                || 关系->退出事实代次)
                return std::nullopt;
            return L2任务来源需求关系事实{关系编码, 任务, 来源需求, 1,
                {关系->创建事实代次, std::nullopt}};
        } catch (...) {
            return std::nullopt;
        }
    }

    std::uint64_t ARCH_退出任务来源需求关系(
        const L2结构请求头& 请求头,
        L2结构幂等身份 幂等身份,
        const L2任务来源需求关系事实& 关系) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        if (!L2结构请求头合同有效(请求头)
            || 请求头.期望事实代次 == 0
            || !L2结构幂等身份有效(幂等身份)
            || !有效(关系.关系稳定编码) || !有效(关系.任务.值)
            || !有效(关系.来源需求.值) || 关系.来源顺序 != 1
            || 关系.生命周期.创建事实代次 == 0
            || 关系.生命周期.退出事实代次)
            return 0;
        try {
            L1所有者范围写集请求 写集;
            写集.合同版本 = L1所有者范围CRUD合同版本;
            写集.期望事实代次 = 请求头.期望事实代次;
            写集.写入幂等身份 = {幂等身份.值};
            写集.退出事实 = {关系.关系稳定编码};
            const auto 写入 = 第一层写入端口_.提交所有者范围中性写集(写集);
            if ((写入.状态 != L1所有者范围写入状态::成功
                    && 写入.状态 != L1所有者范围写入状态::精确重复)
                || 写入.事实代次 == 0)
                return 0;
            const auto 读回 = L1_.读取所有者范围历史事实({
                L1所有者范围CRUD合同版本, 关系.关系稳定编码});
            const auto* 已退出 = 读回.事实
                ? std::get_if<L1所有者范围关系事实>(&*读回.事实) : nullptr;
            return 读回.状态 == L1所有者范围读取状态::成功 && 已退出
                && 已退出->编码 == 关系.关系稳定编码
                && 已退出->写入所有者 == 身份来源定位_.所有者
                && 已退出->源节点 == 关系.任务.值
                && 已退出->目标节点 == 关系.来源需求.值
                && 已退出->关系类型节点 == 类型定位_.任务来源需求关系类型
                && 已退出->角色或顺序 == 1
                && 已退出->退出事实代次 == 写入.事实代次
                ? 写入.事实代次 : 0;
        } catch (...) {
            return 0;
        }
    }
#endif

    L2任务后继继续写入结果 写入或读取任务后继继续事实(
        const L2任务后继继续写入请求& 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        return 写入或读取任务后继继续事实_已锁定(请求);
    }

    L2任务后继继续读取结果 读取任务后继继续事实(
        const L2任务后继继续读取请求& 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        return 读取任务后继继续事实_已锁定(请求);
    }

    L2任务生命周期收口写入结果 写入或读取任务生命周期收口(
        const L2任务生命周期收口写入请求& 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        return 写入或读取任务生命周期收口_已锁定(请求);
    }

    L2任务生命周期收口读取结果 读取任务生命周期收口(
        const L2任务生命周期收口读取请求& 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        return 读取任务生命周期收口_已锁定(请求);
    }

    // ===== P00：新任务核心与 R1 同一原子写集 =====
    L2新增任务轮次与正式存在引用结果
    新增任务轮次与正式存在引用(
        L2新增任务轮次与正式存在引用请求 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [](L2结构状态 状态,
            std::uint64_t = 0) noexcept {
            L2新增任务轮次与正式存在引用结果 结果;
            // 新增入口所有非成功分支均不得泄露观察代次或变更代次。
            结果.结果头 = {L2结构合同版本, 状态, 0, std::nullopt};
            return 结果;
        };
        try {
            if (!L2新增任务轮次与正式存在引用请求有效(请求))
                return 失败(L2结构状态::入口拒绝);
            const auto 当前 = L2任务结构内部::读取中性当前事实代次(L1_);
            if (当前 == 0) return 失败(L2结构状态::内部不一致);
            if (当前 != 请求.请求头.期望事实代次) {
                const auto 请求写集 = L2任务结构内部::形成任务轮次核心写集(
                    请求, 身份来源定位_, 类型定位_, 任务轮次核心定位_);
                const auto 首次 = 第一层写入端口_.读取首次写入材料({
                    L1所有者范围首次写入读取合同版本,
                    请求写集.写入幂等身份});
                if (首次.状态 == L1所有者范围读取状态::未找到)
                    return 失败(L2结构状态::事实代次漂移);
                if (首次.状态 != L1所有者范围读取状态::成功
                    || 首次.合同版本
                        != L1所有者范围首次写入读取合同版本
                    || 首次.所有者 != 第一层写入端口_.所有者身份()
                    || 首次.写入幂等身份 != 请求写集.写入幂等身份
                    || 首次.读取事实代次 != 当前
                    || !首次.首次规范化写集 || !首次.首次写入结果)
                    return 失败(L2任务结构内部::映射子目标记录读取状态(
                        首次.状态));
                if (*首次.首次规范化写集 != 请求写集)
                    return 失败(L2结构状态::幂等冲突);
                const auto 重放 = 第一层写入端口_.提交所有者范围中性写集(
                    *首次.首次规范化写集);
                if (重放.状态 != L1所有者范围写入状态::精确重复
                    || 重放.事实代次 != 首次.首次写入结果->事实代次)
                    return 失败(L2任务结构内部::映射任务写入状态(重放.状态));
                const auto 映射 = L2任务结构内部::
                    查找任务轮次核心写入编码映射(重放);
                if (!映射) return 失败(L2结构状态::内部不一致);
                auto 结果 = L2任务结构内部::读回任务轮次核心(
                    L1_, 身份来源定位_, 类型定位_, 任务轮次核心定位_,
                    *映射, L2结构状态::精确重复, 重放.事实代次);
                if (!结果.成功() || !结果.任务 || !结果.来源需求关系
                    || !结果.正式存在关系
                    || 结果.任务->需求列表项
                        != 请求.首次需求裁决.所属列表项.身份
                    || 结果.来源需求关系->来源需求
                        != 请求.首次需求裁决.需求事实.身份
                    || 结果.正式存在关系->正式存在 != 请求.正式存在)
                    return 失败(L2结构状态::内部不一致);
                return 结果;
            }
            const auto 映射独立读取失败 = [&](L2结构状态 状态,
                std::uint64_t) noexcept {
                if (状态 == L2结构状态::许可拒绝)
                    return 失败(L2结构状态::许可拒绝);
                if (状态 == L2结构状态::资源失败)
                    return 失败(L2结构状态::资源失败);
                if (状态 == L2结构状态::事实代次漂移)
                    return 失败(L2结构状态::事实代次漂移);
                return 失败(L2结构状态::引用冲突);
            };
            const auto 需求读取 = 需求结构服务_.读取需求完整({
                {L2结构合同版本, 当前}, L2读取类别::当前,
                请求.首次需求裁决.需求事实.身份, 0});
            if (需求读取.结果头.状态 != L2结构状态::已读取
                || 需求读取.结果头.事实截止代次 != 当前
                || !需求读取.需求
                || 需求读取.需求->身份 != 请求.首次需求裁决.需求事实.身份
                || !L2需求事实完整(*需求读取.需求, 当前)
                || 需求读取.需求->生命周期.退出事实代次)
                return 映射独立读取失败(需求读取.结果头.状态,
                    需求读取.结果头.事实截止代次);
            const auto 列表项读取 = 需求结构服务_.读取需求列表项完整({
                {L2结构合同版本, 当前}, L2读取类别::当前,
                请求.首次需求裁决.所属列表项.身份, 0});
            if (列表项读取.结果头.状态 != L2结构状态::已读取
                || 列表项读取.结果头.事实截止代次 != 当前
                || !列表项读取.列表项
                || 列表项读取.列表项->身份
                    != 请求.首次需求裁决.所属列表项.身份
                || !L2需求列表项事实完整(*列表项读取.列表项, 当前)
                || 列表项读取.列表项->生命周期.退出事实代次)
                return 映射独立读取失败(列表项读取.结果头.状态,
                    列表项读取.结果头.事实截止代次);
            if (列表项读取.列表项->目标宿主存在
                    != 请求.首次需求裁决.需求事实.目标宿主存在
                || 列表项读取.列表项->目标特征定义
                    != 请求.首次需求裁决.需求事实.目标特征定义
                || 列表项读取.列表项->目标状态合同
                    != 请求.首次需求裁决.需求事实.目标状态合同)
                return 失败(L2结构状态::引用冲突);
            const auto E读取 = L1_.读取所有者范围历史事实(
                {L1所有者范围CRUD合同版本, 请求.正式存在.值});
            const auto* E = E读取.事实
                ? std::get_if<L1所有者范围节点事实>(&*E读取.事实) : nullptr;
            if (E读取.状态 == L1所有者范围读取状态::许可拒绝)
                return 失败(L2结构状态::许可拒绝);
            if (E读取.状态 == L1所有者范围读取状态::资源失败)
                return 失败(L2结构状态::资源失败);
            if (E读取.状态 == L1所有者范围读取状态::事实代次漂移)
                return 失败(L2结构状态::事实代次漂移);
            if (E读取.状态 == L1所有者范围读取状态::未找到)
                return 失败(L2结构状态::未找到);
            if (E读取.状态 == L1所有者范围读取状态::已退出)
                return 失败(L2结构状态::已退出);
            if (E读取.状态 != L1所有者范围读取状态::成功 || !E
                || E->种类 != 节点种类::普通 || E->属性类型表示
                || E->退出事实代次 || E->创建事实代次 > 当前)
                return 失败(L2结构状态::引用冲突);
            const auto 写集 = L2任务结构内部::形成任务轮次核心写集(
                请求, 身份来源定位_, 类型定位_, 任务轮次核心定位_);
            const auto 写入 = 第一层写入端口_.提交所有者范围中性写集(写集);
            const auto 状态 = L2任务结构内部::映射任务写入状态(写入.状态);
            if (写入.合同版本 != L1所有者范围CRUD合同版本
                || 写入.所有者 != 第一层写入端口_.所有者身份()
                || 写入.写入幂等身份 != 写集.写入幂等身份)
                return 失败(L2结构状态::内部不一致);
            if (状态 != L2结构状态::已提交
                && 状态 != L2结构状态::精确重复)
                return 失败(状态);
            const auto 映射 = L2任务结构内部::
                查找任务轮次核心写入编码映射(写入);
            if (!映射 || 写入.事实代次 == 0)
                return 失败(L2结构状态::内部不一致);
            auto 结果 = L2任务结构内部::读回任务轮次核心(
                L1_, 身份来源定位_, 类型定位_, 任务轮次核心定位_,
                *映射, 状态, 写入.事实代次);
            if (!结果.成功()
                || !结果.任务 || !结果.来源需求关系 || !结果.正式存在关系
                || 结果.任务->需求列表项
                    != 请求.首次需求裁决.所属列表项.身份
                || 结果.来源需求关系->来源需求
                    != 请求.首次需求裁决.需求事实.身份
                || 结果.正式存在关系->正式存在 != 请求.正式存在)
                return 失败(L2结构状态::内部不一致);
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2结构状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致);
        }
    }

    // ===== P00：按建立幂等身份完整读回 =====
    L2按建立幂等身份读取任务轮次核心结果
    按建立幂等身份读取任务轮次核心(
        L2按建立幂等身份读取任务轮次核心请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 截止 = L2任务结构内部::读取中性当前事实代次(L1_);
        const auto 失败 = [&](L2结构状态 状态) noexcept {
            L2按建立幂等身份读取任务轮次核心结果 结果;
            结果.结果头 = {L2结构合同版本, 状态, 0, std::nullopt};
            return 结果;
        };
        try {
            if (!L2按建立幂等身份读取任务轮次核心请求有效(请求))
                return 失败(L2结构状态::入口拒绝);
            if (截止 == 0)
                return 失败(L2结构状态::内部不一致);
            if (请求.请求头.期望事实代次 != 截止)
                return 失败(L2结构状态::事实代次漂移);
            const auto 首次 = 第一层写入端口_.读取首次写入材料(
                {L1所有者范围首次写入读取合同版本,
                    {请求.建立幂等身份.值}});
            if (首次.状态 == L1所有者范围读取状态::未找到)
                return 失败(L2结构状态::未找到);
            if (首次.状态 != L1所有者范围读取状态::成功
                || !首次.首次写入结果)
                return 失败(L2任务结构内部::映射子目标记录读取状态(
                    首次.状态));
            const auto 映射 = L2任务结构内部::
                查找任务轮次核心写入编码映射(*首次.首次写入结果);
            if (!映射) return 失败(L2结构状态::内部不一致);
            const auto 核心 = L2任务结构内部::读回任务轮次核心(
                L1_, 身份来源定位_, 类型定位_, 任务轮次核心定位_,
                *映射, L2结构状态::精确重复,
                首次.首次写入结果->事实代次);
            if (!核心.成功()) return 失败(核心.结果头.状态);
            L2按建立幂等身份读取任务轮次核心结果 结果;
            结果.结果头 = {L2结构合同版本, L2结构状态::已读取,
                截止, std::nullopt};
            结果.任务 = 核心.任务;
            结果.任务身份来源 = 核心.任务身份来源;
            结果.来源需求关系 = 核心.来源需求关系;
            结果.正式存在关系 = 核心.正式存在关系;
            结果.任务轮次 = 核心.任务轮次;
            结果.本次正式读回截止 = 截止;
            const auto 读后截止 = L2任务结构内部::读取中性当前事实代次(L1_);
            if (读后截止 == 0)
                return 失败(L2结构状态::内部不一致);
            if (读后截止 != 截止)
                return 失败(L2结构状态::事实代次漂移);
            return 结果.成功() ? 结果 : 失败(L2结构状态::内部不一致);
        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2结构状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致);
        }
    }

    // ===== 首次准备记录、P1 与 A1 =====
    L2新增首次准备记录与P1结果 新增首次准备记录与P1(
        L2新增首次准备记录与P1请求 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [&](L2结构状态 状态,
            std::uint64_t 截止 = 0) noexcept {
            L2新增首次准备记录与P1结果 结果;
            结果.结果头 = {L2结构合同版本, 状态, 截止, std::nullopt};
            return 结果;
        };
        try {
            if (!L2新增首次准备记录与P1请求有效(请求))
                return 失败(L2结构状态::入口拒绝);
            const auto 代次 = L2任务结构内部::检查任务写入当前代次(
                L1_, 请求.请求头.期望事实代次);
            if (!代次.成功() && !代次.直接提交)
                return 失败(代次.状态, 代次.事实截止代次);
            if (代次.成功()) {
                const auto 核心首次 = 第一层写入端口_.读取首次写入材料(
                    {L1所有者范围首次写入读取合同版本,
                        {请求.完整请求.阶段幂等身份.任务核心建立.值}});
                const auto 核心映射 = 核心首次.首次写入结果
                    ? L2任务结构内部::查找任务轮次核心写入编码映射(
                        *核心首次.首次写入结果)
                    : std::nullopt;
                const auto 核心 = 核心映射
                    ? L2任务结构内部::读回任务轮次核心(
                        L1_, 身份来源定位_, 类型定位_, 任务轮次核心定位_,
                        *核心映射, L2结构状态::精确重复,
                        核心首次.首次写入结果->事实代次)
                    : L2新增任务轮次与正式存在引用结果{};
                if (核心首次.状态 != L1所有者范围读取状态::成功
                    || !核心.成功() || !核心.任务 || !核心.来源需求关系
                    || !核心.正式存在关系 || !核心.任务轮次
                    || 核心.任务->身份 != 请求.任务
                    || 核心.任务->需求列表项
                        != 请求.首次需求裁决.所属列表项.身份
                    || 核心.任务->任务虚拟存在.值 != 请求.正式存在.值
                    || 核心.来源需求关系->来源需求
                        != 请求.首次需求裁决.需求事实.身份
                    || 核心.正式存在关系->正式存在 != 请求.正式存在
                    || 核心.任务轮次->身份 != 请求.任务轮次
                    || 核心.任务轮次->任务 != 请求.任务
                    || 核心.任务轮次->轮次序号 != 1)
                    return 失败(L2结构状态::引用冲突, 代次.事实截止代次);
                const 稳定编码 引用[] = {请求.任务轮次.值, 请求.正式存在.值,
                    请求.阶段特征实例.值, 请求.待初次筹办首态.值};
                for (const auto 编码 : 引用) {
                    std::optional<L1所有者范围节点事实> 保存;
                    const auto* 节点 = L2任务结构内部::读取初次筹办历史事实(
                        L1_, 编码, 保存);
                    if (!节点 || 节点->创建事实代次 > 请求.请求头.期望事实代次
                        || 节点->退出事实代次)
                        return 失败(L2结构状态::引用冲突, 代次.事实截止代次);
                }
                const auto 定义 = L2任务结构内部::读取治理函数定义投影(
                    L1_, 身份来源定位_, 初次筹办治理定位_,
                    {请求.请求头, L2读取类别::当前, 0});
                if (!定义.成功())
                    return 失败(L2结构状态::引用冲突, 代次.事实截止代次);
            }
            const auto 写集 = L2任务结构内部::形成首次P1治理写集(
                请求, 初次筹办记录定位_, 筹办轮次定位_, 初次筹办治理定位_);
            const auto 写入 = 第一层写入端口_.提交所有者范围中性写集(写集);
            const auto 状态 = L2任务结构内部::映射任务写入状态(写入.状态);
            if (写入.合同版本 != L1所有者范围CRUD合同版本
                || 写入.所有者 != 第一层写入端口_.所有者身份()
                || 写入.写入幂等身份 != 写集.写入幂等身份)
                return 失败(L2结构状态::内部不一致, 写入.事实代次);
            if (状态 != L2结构状态::已提交 && 状态 != L2结构状态::精确重复) {
                if (!写入.新编码映射.empty() || 写入.是否形成内存权威发布)
                    return 失败(L2结构状态::内部不一致, 写入.事实代次);
                return 失败(状态, 写入.事实代次);
            }
            const auto 映射 =
                L2任务结构内部::查找首次P1治理写入编码映射(写入);
            if (!映射 || 写入.事实代次 == 0)
                return 失败(L2结构状态::内部不一致, 写入.事实代次);
            return L2任务结构内部::读回首次P1治理(L1_, 身份来源定位_,
                初次筹办记录定位_, 筹办轮次定位_, 初次筹办治理定位_,
                *映射, 状态, 写入.事实代次);
        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2结构状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致);
        }
    }

    L2任务治理函数定义读取结果 读取任务治理函数定义(
        L2任务治理函数定义读取请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        try {
            if (!L2任务结构请求头合同有效(请求.请求头)
                || 请求.请求头.期望事实代次 == 0
                || (请求.读取类别 == L2读取类别::当前
                    && 请求.历史截止事实代次 != 0)
                || (请求.读取类别 == L2读取类别::历史
                    && 请求.历史截止事实代次 == 0))
                return {};
            return L2任务结构内部::读取治理函数定义投影(
                L1_, 身份来源定位_, 初次筹办治理定位_, 请求);
        } catch (...) { return {}; }
    }

    L2任务治理动作调用读取结果 读取任务治理动作调用(
        L2任务治理动作调用读取请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        try {
            if (!L2任务结构请求头合同有效(请求.请求头)
                || 请求.请求头.期望事实代次 == 0
                || !有效(请求.调用.值)
                || (请求.读取类别 == L2读取类别::当前
                    && 请求.历史截止事实代次 != 0)
                || (请求.读取类别 == L2读取类别::历史
                    && 请求.历史截止事实代次 == 0))
                return {};
            return L2任务结构内部::读取治理动作调用投影(
                L1_, 身份来源定位_, 初次筹办治理定位_, 请求);
        } catch (...) { return {}; }
    }

    L2任务治理动作统一读取结果_v2 按身份读取任务治理动作调用_v2(
        const L2任务治理动作统一读取请求_v2& 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [](L2任务治理动作统一读取状态_v2 状态) noexcept {
            L2任务治理动作统一读取结果_v2 结果;
            结果.状态 = 状态;
            return 结果;
        };
        const auto 映射中性失败 = [&](L1中性读取状态 状态) noexcept {
            switch (状态) {
            case L1中性读取状态::许可拒绝:
                return 失败(L2任务治理动作统一读取状态_v2::许可拒绝);
            case L1中性读取状态::资源失败:
                return 失败(L2任务治理动作统一读取状态_v2::资源失败);
            default:
                return 失败(L2任务治理动作统一读取状态_v2::内部错误);
            }
        };
        const auto 映射所有者失败 = [&](L1所有者范围读取状态 状态) noexcept {
            switch (状态) {
            case L1所有者范围读取状态::许可拒绝:
                return 失败(L2任务治理动作统一读取状态_v2::许可拒绝);
            case L1所有者范围读取状态::事实代次漂移:
                return 失败(L2任务治理动作统一读取状态_v2::当前性漂移);
            case L1所有者范围读取状态::资源失败:
                return 失败(L2任务治理动作统一读取状态_v2::资源失败);
            default:
                return 失败(L2任务治理动作统一读取状态_v2::内部错误);
            }
        };
        if (请求.合同版本 != L2任务治理动作统一读取合同版本_v2
            || !L2任务结构请求头合同有效(请求.请求头)
            || 请求.请求头.期望事实代次 == 0 || !有效(请求.身份.值))
            return 失败(L2任务治理动作统一读取状态_v2::入口拒绝);
        try {
            const auto G0 = 请求.请求头.期望事实代次;
            const auto 检查当前代次 = [&]()
                -> std::optional<L2任务治理动作统一读取结果_v2> {
                const auto 读取 = L1_.读取中性当前事实代次(
                    {L1中性CRUD合同版本});
                if (读取.状态 != L1中性读取状态::成功)
                    return 映射中性失败(读取.状态);
                if (读取.合同版本 != L1中性CRUD合同版本
                    || 读取.事实代次 == 0)
                    return 失败(L2任务治理动作统一读取状态_v2::内部错误);
                if (读取.事实代次 != G0)
                    return 失败(L2任务治理动作统一读取状态_v2::当前性漂移);
                return std::nullopt;
            };
            if (const auto 守卫失败 = 检查当前代次()) return *守卫失败;

            const auto 节点读取 = L1_.读取所有者范围历史事实(
                {L1所有者范围CRUD合同版本, 请求.身份.值});
            if (节点读取.状态 == L1所有者范围读取状态::未找到
                || 节点读取.状态 == L1所有者范围读取状态::已退出)
                return 失败(L2任务治理动作统一读取状态_v2::未找到);
            if (节点读取.状态 != L1所有者范围读取状态::成功)
                return 映射所有者失败(节点读取.状态);
            if (节点读取.合同版本 != L1所有者范围CRUD合同版本
                || 节点读取.查询编码 != 请求.身份.值
                || 节点读取.读取事实代次 != G0 || !节点读取.事实)
                return 失败(L2任务治理动作统一读取状态_v2::当前性漂移);
            const auto* 节点 = std::get_if<L1所有者范围节点事实>(
                &*节点读取.事实);
            if (!节点 || 节点->写入所有者 != 第一层写入端口_.所有者身份()
                || 节点->创建事实代次 == 0 || 节点->创建事实代次 > G0
                || (节点->退出事实代次 && *节点->退出事实代次 <= G0))
                return 失败(L2任务治理动作统一读取状态_v2::引用冲突);

            const 稳定编码 关系类型[] = {
                初次筹办治理定位_.治理动作族归属关系类型,
                初次筹办治理定位_.治理动作函数关系类型,
                初次筹办治理定位_.治理动作任务关系类型,
                初次筹办治理定位_.治理动作轮次关系类型,
                初次筹办治理定位_.治理动作筹办轮次关系类型,
                初次筹办治理定位_.治理动作触发前态关系类型};
            稳定编码 目标[6]{};
            for (std::size_t i = 0; i < 6; ++i) {
                const auto 读取 = L1_.读取所有者范围历史关系组({
                    L1所有者范围CRUD合同版本,
                    L1所有者范围关系端点方向::源,
                    请求.身份.值, 关系类型[i], G0});
                if (读取.状态 != L1所有者范围读取状态::成功)
                    return 映射所有者失败(读取.状态);
                if (读取.合同版本 != L1所有者范围CRUD合同版本
                    || 读取.方向 != L1所有者范围关系端点方向::源
                    || 读取.端点节点 != 请求.身份.值
                    || 读取.关系类型节点 != 关系类型[i]
                    || 读取.历史截止事实代次 != G0
                    || 读取.读取事实代次 != G0)
                    return 失败(L2任务治理动作统一读取状态_v2::当前性漂移);
                if (读取.关系组.size() > 1)
                    return 失败(L2任务治理动作统一读取状态_v2::多义);
                if (读取.关系组.empty())
                    return 失败(L2任务治理动作统一读取状态_v2::引用冲突);
                const auto& 关系 = 读取.关系组.front();
                if (关系.写入所有者 != 第一层写入端口_.所有者身份()
                    || 关系.源节点 != 请求.身份.值
                    || 关系.关系类型节点 != 关系类型[i]
                    || 关系.角色或顺序 != 1 || !有效(关系.目标节点)
                    || 关系.创建事实代次 == 0 || 关系.创建事实代次 > G0
                    || (关系.退出事实代次 && *关系.退出事实代次 <= G0))
                    return 失败(L2任务治理动作统一读取状态_v2::引用冲突);
                目标[i] = 关系.目标节点;
            }
            if (目标[0] != 初次筹办治理定位_.治理动作调用锚点)
                return 失败(L2任务治理动作统一读取状态_v2::引用冲突);

            const auto 属性读取 = L1_.读取所有者范围历史属性值组({
                L1所有者范围CRUD合同版本, 请求.身份.值, G0});
            if (属性读取.状态 != L1所有者范围读取状态::成功)
                return 映射所有者失败(属性读取.状态);
            if (属性读取.合同版本 != L1所有者范围CRUD合同版本
                || 属性读取.所属节点 != 请求.身份.值
                || 属性读取.历史截止事实代次 != G0
                || 属性读取.读取事实代次 != G0)
                return 失败(L2任务治理动作统一读取状态_v2::当前性漂移);
            const std::vector<std::uint64_t>* 动作材料 = nullptr;
            for (const auto& 值 : 属性读取.属性值组) {
                if (值.属性类型节点
                    != 初次筹办治理定位_.治理动作调用材料属性类型)
                    continue;
                if (动作材料)
                    return 失败(L2任务治理动作统一读取状态_v2::多义);
                if (值.写入所有者 != 第一层写入端口_.所有者身份()
                    || 值.所属节点 != 请求.身份.值
                    || 值.来源节点 != 请求.身份.值
                    || 值.创建事实代次 == 0 || 值.创建事实代次 > G0
                    || (值.退出事实代次 && *值.退出事实代次 <= G0))
                    return 失败(L2任务治理动作统一读取状态_v2::引用冲突);
                动作材料 = std::get_if<std::vector<std::uint64_t>>(&值.材料);
                if (!动作材料)
                    return 失败(L2任务治理动作统一读取状态_v2::引用冲突);
            }
            if (!动作材料 || 动作材料->size() != 4
                || (*动作材料)[0] == 0 || (*动作材料)[1] == 0
                || (*动作材料)[2] != L2任务治理函数合同版本)
                return 失败(L2任务治理动作统一读取状态_v2::引用冲突);

            struct 已登记定位 final {
                稳定编码 函数;
                std::uint32_t 规则版本;
                L2任务治理函数种类 种类;
            };
            const 已登记定位 已登记[] = {
                {初次筹办治理定位_.治理函数定义节点,
                    L2初次筹办推进规则版本,
                    L2任务治理函数种类::初次筹办推进},
                {轮次结算定位_.轮次收束治理函数定义,
                    L2任务轮次结算规则版本,
                    L2任务治理函数种类::轮次收束},
                {轮次结算定位_.自我继续治理函数定义,
                    L2任务后继机械规则版本,
                    L2任务治理函数种类::自我继续开启新轮次}};
            const 已登记定位* 命中 = nullptr;
            for (const auto& 定位 : 已登记) {
                if (定位.函数 != 目标[1]
                    || 定位.规则版本 != (*动作材料)[3])
                    continue;
                if (命中)
                    return 失败(L2任务治理动作统一读取状态_v2::多义);
                命中 = &定位;
            }
            if (!命中)
                return 失败(L2任务治理动作统一读取状态_v2::动作合同不支持);

            const auto 函数节点读取 = L1_.读取所有者范围历史事实(
                {L1所有者范围CRUD合同版本, 命中->函数});
            if (函数节点读取.状态 != L1所有者范围读取状态::成功)
                return 映射所有者失败(函数节点读取.状态);
            if (函数节点读取.合同版本 != L1所有者范围CRUD合同版本
                || 函数节点读取.查询编码 != 命中->函数
                || 函数节点读取.读取事实代次 != G0 || !函数节点读取.事实)
                return 失败(L2任务治理动作统一读取状态_v2::当前性漂移);
            const auto* 函数节点 = std::get_if<L1所有者范围节点事实>(
                &*函数节点读取.事实);
            if (!函数节点
                || 函数节点->写入所有者 != 第一层写入端口_.所有者身份()
                || 函数节点->创建事实代次 == 0 || 函数节点->创建事实代次 > G0
                || (函数节点->退出事实代次
                    && *函数节点->退出事实代次 <= G0))
                return 失败(L2任务治理动作统一读取状态_v2::引用冲突);
            const auto 函数属性读取 = L1_.读取所有者范围历史属性值组({
                L1所有者范围CRUD合同版本, 命中->函数, G0});
            if (函数属性读取.状态 != L1所有者范围读取状态::成功)
                return 映射所有者失败(函数属性读取.状态);
            if (函数属性读取.合同版本 != L1所有者范围CRUD合同版本
                || 函数属性读取.所属节点 != 命中->函数
                || 函数属性读取.历史截止事实代次 != G0
                || 函数属性读取.读取事实代次 != G0)
                return 失败(L2任务治理动作统一读取状态_v2::当前性漂移);
            const std::vector<std::uint64_t>* 函数材料 = nullptr;
            for (const auto& 值 : 函数属性读取.属性值组) {
                if (值.属性类型节点
                    != 初次筹办治理定位_.治理函数定义材料属性类型)
                    continue;
                if (函数材料)
                    return 失败(L2任务治理动作统一读取状态_v2::多义);
                if (值.写入所有者 != 第一层写入端口_.所有者身份()
                    || 值.所属节点 != 命中->函数 || 值.来源节点 != 命中->函数
                    || 值.创建事实代次 == 0 || 值.创建事实代次 > G0
                    || (值.退出事实代次 && *值.退出事实代次 <= G0))
                    return 失败(L2任务治理动作统一读取状态_v2::引用冲突);
                函数材料 = std::get_if<std::vector<std::uint64_t>>(&值.材料);
                if (!函数材料)
                    return 失败(L2任务治理动作统一读取状态_v2::引用冲突);
            }
            if (!函数材料 || 函数材料->size() != 3
                || (*函数材料)[0] != static_cast<std::uint64_t>(命中->种类)
                || (*函数材料)[1] != L2任务治理函数合同版本
                || (*函数材料)[2] != 命中->规则版本)
                return 失败(L2任务治理动作统一读取状态_v2::引用冲突);

            if (!有效(目标[2]) || !有效(目标[3]) || !有效(目标[4])
                || !有效(目标[5]) || (*动作材料)[0] >= 节点->创建事实代次)
                return 失败(L2任务治理动作统一读取状态_v2::引用冲突);
            if (const auto 守卫失败 = 检查当前代次()) return *守卫失败;
            L2任务治理动作统一读取结果_v2 结果;
            结果.状态 = L2任务治理动作统一读取状态_v2::已读取;
            结果.治理动作 = L2任务治理动作调用事实{
                请求.身份, L2任务治理函数身份{目标[1]},
                L2任务身份{目标[2]}, L2任务轮次身份{目标[3]},
                L2任务筹办轮次权威记录身份{目标[4]},
                L2状态身份{目标[5]}, (*动作材料)[0],
                L2结构幂等身份{(*动作材料)[1]},
                static_cast<std::uint32_t>((*动作材料)[2]),
                static_cast<std::uint32_t>((*动作材料)[3]),
                {节点->创建事实代次, std::nullopt}};
            结果.本次正式读回截止 = G0;
            return 结果.成功() ? 结果
                : 失败(L2任务治理动作统一读取状态_v2::内部错误);
        } catch (const std::bad_alloc&) {
            return 失败(L2任务治理动作统一读取状态_v2::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2任务治理动作统一读取状态_v2::资源失败);
        } catch (...) {
            return 失败(L2任务治理动作统一读取状态_v2::内部错误);
        }
    }

    // ===== 新增任务与首次初次筹办准备记录 =====
    L2新增任务与首次初次筹办准备记录结果
    新增任务与首次初次筹办准备记录(
        L2新增任务与首次初次筹办准备记录请求 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [&](L2结构状态 状态,
            std::uint64_t 截止 = 0) noexcept {
            L2新增任务与首次初次筹办准备记录结果 结果;
            结果.结果头 = {L2结构合同版本, 状态, 截止, std::nullopt};
            return 结果;
        };
        try {
            if (!L2新增任务与首次初次筹办准备记录请求有效(请求))
                return 失败(L2结构状态::入口拒绝);
            const auto 代次 = L2任务结构内部::检查任务写入当前代次(
                L1_, 请求.请求头.期望事实代次);
            if (!代次.成功() && !代次.直接提交)
                return 失败(代次.状态, 代次.事实截止代次);
            if (代次.成功()) {
                const L2按需求列表项读取当前任务请求 当前任务请求{
                    请求.请求头, 请求.首次需求裁决.所属列表项.身份};
                const auto 当前任务 =
                    L2任务结构内部::按需求列表项读取当前任务当前投影(
                        L1_, 身份来源定位_, 类型定位_, 当前任务请求,
                        请求.请求头.期望事实代次);
                if (!当前任务.成功())
                    return 失败(当前任务.结果头.状态,
                        当前任务.结果头.事实截止代次);
                if (当前任务.任务)
                    return 失败(L2结构状态::引用冲突,
                        当前任务.结果头.事实截止代次);
                L2需求列表项身份读取请求 列表项请求;
                列表项请求.合同版本 = L2结构合同版本;
                列表项请求.列表项 = 请求.首次需求裁决.所属列表项.身份;
                const auto 列表项验证 = 需求结构服务_.读取需求列表项身份(
                    列表项请求);
                if (!列表项验证.成功())
                    return 失败(L2结构状态::引用冲突,
                        代次.事实截止代次);
            }
            const auto 写集 = L2任务结构内部::形成初次筹办融合写集(
                请求, 身份来源定位_, 类型定位_, 初次筹办记录定位_,
                筹办轮次定位_);
            const auto 写入 = 第一层写入端口_.提交所有者范围中性写集(写集);
            const auto 状态 = L2任务结构内部::映射任务写入状态(写入.状态);
            if (写入.合同版本 != L1所有者范围CRUD合同版本
                || 写入.所有者 != 第一层写入端口_.所有者身份()
                || 写入.写入幂等身份 != 写集.写入幂等身份)
                return 失败(L2结构状态::内部不一致, 写入.事实代次);
            if (状态 != L2结构状态::已提交
                && 状态 != L2结构状态::精确重复) {
                if (!写入.新编码映射.empty()
                    || 写入.是否形成内存权威发布)
                    return 失败(L2结构状态::内部不一致, 写入.事实代次);
                return 失败(状态, 写入.事实代次);
            }
            const bool 首次完整 = 写入.状态 == L1所有者范围写入状态::成功
                && 写入.是否形成内存权威发布
                && 写入.重试边界 == L1所有者范围重试边界::不适用;
            const bool 重复完整 =
                写入.状态 == L1所有者范围写入状态::精确重复
                && !写入.是否形成内存权威发布
                && 写入.重试边界
                    == L1所有者范围重试边界::原幂等身份读回收敛;
            const auto 映射 =
                L2任务结构内部::查找初次筹办融合写入编码映射(写入);
            if (写入.事实代次 == 0 || (!首次完整 && !重复完整) || !映射)
                return 失败(L2结构状态::内部不一致, 写入.事实代次);
            return L2任务结构内部::读回初次筹办融合首次结果(
                L1_, 身份来源定位_, 类型定位_, 初次筹办记录定位_,
                筹办轮次定位_,
                *映射, 状态, 写入.事实代次);
        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2结构状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致);
        }
    }

    // ===== 按准备幂等身份读取首次记录 =====
    L2按初次筹办准备幂等身份读取首次记录结果
    按初次筹办准备幂等身份读取首次记录(
        L2按初次筹办准备幂等身份读取首次记录请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 截止 = L2任务结构内部::读取中性当前事实代次(L1_);
        const auto 失败 = [&](L2结构状态 状态) noexcept {
            L2按初次筹办准备幂等身份读取首次记录结果 结果;
            结果.结果头 = {L2结构合同版本, 状态, 截止, std::nullopt};
            return 结果;
        };
        if (!L2任务结构请求头合同有效(请求.请求头)
            || 请求.请求头.期望事实代次 == 0
            || 请求.准备幂等身份 == 0 || 截止 == 0)
            return 失败(L2结构状态::入口拒绝);
        try {
            return L2任务结构内部::
                按初次筹办准备幂等身份读取首次记录投影(
                    L1_, 第一层写入端口_, 身份来源定位_, 类型定位_,
                    初次筹办记录定位_, 筹办轮次定位_, 初次筹办治理定位_,
                    请求, 截止);
        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2结构状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致);
        }
    }

    // ===== 按记录身份读取首次初次筹办准备记录 =====
    L2任务初次筹办准备记录读取结果 读取首次初次筹办准备记录(
        L2任务初次筹办准备记录读取请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 截止 = L2任务结构内部::读取中性当前事实代次(L1_);
        const auto 失败 = [&](L2结构状态 状态) noexcept {
            L2任务初次筹办准备记录读取结果 结果;
            结果.结果头 = {L2结构合同版本, 状态, 截止, std::nullopt};
            结果.读取类别 = 请求.读取类别;
            结果.历史截止事实代次 = 请求.历史截止事实代次;
            return 结果;
        };
        if (!L2任务初次筹办准备记录读取请求有效(请求) || 截止 == 0)
            return 失败(L2结构状态::入口拒绝);
        if (请求.请求头.期望事实代次 != 截止)
            return 失败(L2结构状态::事实代次漂移);
        try {
            const auto 观察截止 = 请求.读取类别 == L2读取类别::当前
                ? 截止 : 请求.历史截止事实代次;
            const auto 节点读取 = L1_.读取所有者范围历史事实(
                {L1所有者范围CRUD合同版本, 请求.首次准备记录.值});
            const auto* 节点 = 节点读取.事实
                ? std::get_if<L1所有者范围节点事实>(&*节点读取.事实)
                : nullptr;
            if (节点读取.状态 != L1所有者范围读取状态::成功 || !节点)
                return 失败(L2任务结构内部::映射子目标记录读取状态(
                    节点读取.状态));
            if (节点->写入所有者 != 身份来源定位_.所有者
                || 节点->创建事实代次 > 观察截止
                || (节点->退出事实代次
                    && *节点->退出事实代次 <= 观察截止))
                return 失败(L2结构状态::未找到);
            const auto 值读取 = L1_.读取所有者范围历史属性值组(
                {L1所有者范围CRUD合同版本, 请求.首次准备记录.值,
                    观察截止});
            if (值读取.状态 != L1所有者范围读取状态::成功
                || 值读取.历史截止事实代次 != 观察截止)
                return 失败(L2任务结构内部::映射子目标记录读取状态(
                    值读取.状态));
            std::optional<L2任务结构内部::初次筹办完整请求解码结果>
                完整请求;
            for (const auto& 值 : 值读取.属性值组) {
                if (值.属性类型节点 != 初次筹办记录定位_.完整请求属性类型)
                    continue;
                if (完整请求) return 失败(L2结构状态::内部不一致);
                完整请求 = L2任务结构内部::解码初次筹办完整请求(值.材料);
            }
            if (!完整请求) return 失败(L2结构状态::内部不一致);
            if (完整请求->状态 ==
                L2任务初次筹办请求材料状态::初次请求版本待迁移) {
                auto 结果 = 失败(L2结构状态::已读取);
                结果.材料状态 =
                    L2任务初次筹办请求材料状态::初次请求版本待迁移;
                return 结果;
            }
            if (完整请求->状态
                    != L2任务初次筹办请求材料状态::当前版本
                || !完整请求->材料)
                return 失败(L2结构状态::内部不一致);
            const auto 首次 = L2任务结构内部::
                按初次筹办准备幂等身份读取首次记录投影(
                    L1_, 第一层写入端口_, 身份来源定位_, 类型定位_,
                    初次筹办记录定位_, 筹办轮次定位_, 初次筹办治理定位_,
                    {{L2结构合同版本, 截止},
                        完整请求->材料->准备幂等身份},
                    截止);
            if (!首次.成功() || !首次.首次准备记录
                || 首次.首次准备记录->身份 != 请求.首次准备记录)
                return 失败(首次.结果头.状态 == L2结构状态::已读取
                    ? L2结构状态::内部不一致 : 首次.结果头.状态);
            L2任务初次筹办准备记录读取结果 结果;
            结果.结果头 = {L2结构合同版本, L2结构状态::已读取,
                截止, std::nullopt};
            结果.材料状态 = L2任务初次筹办请求材料状态::当前版本;
            结果.读取类别 = 请求.读取类别;
            结果.历史截止事实代次 = 请求.历史截止事实代次;
            结果.首次准备记录 = std::move(首次.首次准备记录);
            结果.首次任务 = std::move(首次.首次任务);
            结果.首次任务身份来源 = std::move(首次.首次任务身份来源);
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2结构状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致);
        }
    }

    L2按任务读取筹办轮次分区结果 按任务读取筹办轮次分区(
        L2按任务读取筹办轮次分区请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 观察代次 = L2任务结构内部::读取中性当前事实代次(L1_);
        L2按任务读取筹办轮次分区结果 失败;
        失败.结果头 = {L2结构合同版本, L2结构状态::入口拒绝,
            观察代次, std::nullopt};
        if (!L2按任务读取筹办轮次分区请求有效(请求)
            || 观察代次 == 0 || 请求.请求头.期望事实代次 != 观察代次)
            return 失败;
        try {
            const auto 截止 = 请求.读取类别 == L2读取类别::当前
                ? 观察代次 : 请求.历史截止事实代次;
            return L2任务结构内部::读取筹办轮次分区投影(
                L1_, 第一层写入端口_, 身份来源定位_, 类型定位_,
                初次筹办记录定位_, 筹办轮次定位_, 初次筹办治理定位_,
                请求, 截止, 观察代次);
        } catch (const std::bad_alloc&) {
            失败.结果头.状态 = L2结构状态::资源失败;
            return 失败;
        } catch (...) {
            失败.结果头.状态 = L2结构状态::内部不一致;
            return 失败;
        }
    }

    L2任务筹办轮次权威记录读取结果 读取筹办轮次权威记录(
        L2任务筹办轮次权威记录读取请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 观察代次 = L2任务结构内部::读取中性当前事实代次(L1_);
        L2任务筹办轮次权威记录读取结果 结果;
        结果.结果头 = {L2结构合同版本, L2结构状态::入口拒绝,
            观察代次, std::nullopt};
        if (!L2任务结构请求头合同有效(请求.请求头)
            || 请求.请求头.期望事实代次 != 观察代次
            || !有效(请求.轮次权威.值)
            || !L2任务读取类别有效(请求.读取类别,
                请求.历史截止事实代次, 观察代次))
            return 结果;
        try {
            const auto 截止 = 请求.读取类别 == L2读取类别::当前
                ? 观察代次 : 请求.历史截止事实代次;
            结果.轮次权威 = L2任务结构内部::读取筹办轮次权威历史投影(
                L1_, 筹办轮次定位_, 请求.轮次权威, 截止, 观察代次);
            结果.结果头.状态 = 结果.轮次权威
                ? L2结构状态::已读取 : L2结构状态::未找到;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.结果头.状态 = L2结构状态::资源失败;
            return 结果;
        } catch (...) {
            结果.结果头.状态 = L2结构状态::内部不一致;
            return 结果;
        }
    }

    L2任务后继筹办准备记录读取结果 读取后继筹办准备记录(
        L2任务后继筹办准备记录读取请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 观察代次 = L2任务结构内部::读取中性当前事实代次(L1_);
        L2任务后继筹办准备记录读取结果 结果;
        结果.结果头 = {L2结构合同版本, L2结构状态::入口拒绝,
            观察代次, std::nullopt};
        if (!L2任务结构请求头合同有效(请求.请求头)
            || 请求.请求头.期望事实代次 != 观察代次
            || !有效(请求.后继准备记录.值)
            || !L2任务读取类别有效(请求.读取类别,
                请求.历史截止事实代次, 观察代次))
            return 结果;
        try {
            const auto 截止 = 请求.读取类别 == L2读取类别::当前
                ? 观察代次 : 请求.历史截止事实代次;
            结果.后继准备记录 =
                L2任务结构内部::读取后继筹办准备历史投影(
                    L1_, 筹办轮次定位_, 请求.后继准备记录,
                    截止, 观察代次);
            结果.结果头.状态 = 结果.后继准备记录
                ? L2结构状态::已读取 : L2结构状态::未找到;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.结果头.状态 = L2结构状态::资源失败;
            return 结果;
        } catch (...) {
            结果.结果头.状态 = L2结构状态::内部不一致;
            return 结果;
        }
    }

    L2按后继筹办准备幂等身份读取记录结果
    按后继筹办准备幂等身份读取记录(
        L2按后继筹办准备幂等身份读取记录请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 观察代次 = L2任务结构内部::读取中性当前事实代次(L1_);
        L2按后继筹办准备幂等身份读取记录结果 结果;
        结果.结果头 = {L2结构合同版本, L2结构状态::入口拒绝,
            观察代次, std::nullopt};
        if (!L2任务结构请求头合同有效(请求.请求头)
            || 请求.请求头.期望事实代次 != 观察代次
            || !L2结构幂等身份有效(请求.准备幂等身份))
            return 结果;
        try {
            const auto 首次 = 第一层写入端口_.读取首次写入材料({
                L1所有者范围首次写入读取合同版本,
                {请求.准备幂等身份.值}});
            if (首次.状态 == L1所有者范围读取状态::未找到) {
                结果.结果头.状态 = L2结构状态::已读取;
                return 结果;
            }
            if (首次.状态 != L1所有者范围读取状态::成功
                || !首次.首次写入结果) {
                结果.结果头.状态 = L2结构状态::内部不一致;
                return 结果;
            }
            const auto 编码 = L2任务结构内部::查找治理写入编码(
                *首次.首次写入结果,
                L2任务结构内部::后继准备记录节点写入键);
            if (!编码) {
                结果.结果头.状态 = L2结构状态::已读取;
                return 结果;
            }
            结果.后继准备记录 =
                L2任务结构内部::读取后继筹办准备历史投影(
                    L1_, 筹办轮次定位_,
                    L2任务后继筹办准备记录身份{*编码},
                    首次.首次写入结果->事实代次, 观察代次);
            结果.结果头.状态 = 结果.后继准备记录
                ? L2结构状态::已读取 : L2结构状态::内部不一致;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.结果头.状态 = L2结构状态::资源失败;
            return 结果;
        } catch (...) {
            结果.结果头.状态 = L2结构状态::内部不一致;
            return 结果;
        }
    }

    L2提交任务子目标回流后继筹办准备结果
    提交任务子目标回流后继筹办准备(
        L2提交任务子目标回流后继筹办准备请求 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [](L2结构状态 状态,
            std::uint64_t 截止 = 0) noexcept {
            L2提交任务子目标回流后继筹办准备结果 结果;
            结果.结果头 = {L2结构合同版本, 状态, 截止, std::nullopt};
            return 结果;
        };
        try {
            if (!L2提交任务子目标回流后继筹办准备请求有效(请求))
                return 失败(L2结构状态::入口拒绝);
            const auto 当前代次 = L2任务结构内部::读取中性当前事实代次(L1_);
            if (当前代次 == 0) return 失败(L2结构状态::内部不一致);

            const auto 首次 = 第一层写入端口_.读取首次写入材料({
                L1所有者范围首次写入读取合同版本,
                {请求.幂等身份.值}});
            if (首次.状态 == L1所有者范围读取状态::成功) {
                if (!首次.首次写入结果)
                    return 失败(L2结构状态::内部不一致, 当前代次);
                const auto 后继编码 = L2任务结构内部::查找治理写入编码(
                    *首次.首次写入结果,
                    L2任务结构内部::后继准备记录节点写入键);
                if (!后继编码)
                    return 失败(L2结构状态::幂等冲突, 当前代次);
                const auto 后继 = L2任务结构内部::读取后继筹办准备历史投影(
                    L1_, 筹办轮次定位_,
                    L2任务后继筹办准备记录身份{*后继编码},
                    首次.首次写入结果->事实代次, 当前代次);
                if (!后继)
                    return 失败(L2结构状态::内部不一致, 当前代次);
                if (!L2任务结构内部::子目标回流后继与请求一致(
                        *后继, 请求))
                    return 失败(L2结构状态::幂等冲突, 当前代次);
                L2提交任务子目标回流后继筹办准备结果 结果;
                结果.结果头 = {L2结构合同版本, L2结构状态::精确重复,
                    首次.首次写入结果->事实代次,
                    首次.首次写入结果->事实代次};
                结果.后继准备记录 = *后继;
                return 结果;
            }
            if (首次.状态 != L1所有者范围读取状态::未找到) {
                return 失败(L2任务结构内部::映射子目标记录读取状态(
                    首次.状态), 当前代次);
            }
            if (当前代次 != 请求.请求头.期望事实代次)
                return 失败(L2结构状态::事实代次漂移, 当前代次);
            const auto Gs = 当前代次;

            const auto 回流记录 =
                L2任务结构内部::读取任务子目标承接记录历史投影(
                    L1_, 子目标记录定位_, 请求.回流记录, Gs, Gs);
            if (!回流记录)
                return 失败(L2结构状态::未找到, Gs);
            if (回流记录->状态 !=
                    L2任务子目标承接记录状态::子需求已回流待父需求重判
                || 回流记录->任务 != 请求.任务
                || !有效(回流记录->父需求.值)
                || !回流记录->子需求 || !有效(回流记录->子需求->值)
                || !回流记录->回流
                || !L2任务子目标回流材料有效(*回流记录->回流))
                return 失败(L2结构状态::引用冲突, Gs);

            const auto 已触发组 = L1_.读取所有者范围历史关系组({
                L1所有者范围CRUD合同版本,
                L1所有者范围关系端点方向::目标, 请求.回流记录.值,
                筹办轮次定位_.关系类型[12], Gs});
            if (已触发组.状态 != L1所有者范围读取状态::成功
                || 已触发组.历史截止事实代次 != Gs
                || 已触发组.读取事实代次 != Gs
                || 已触发组.关系组.size() > 1)
                return 失败(L2结构状态::内部不一致, Gs);
            if (!已触发组.关系组.empty()) {
                const auto 既有 =
                    L2任务结构内部::读取后继筹办准备历史投影(
                        L1_, 筹办轮次定位_,
                        L2任务后继筹办准备记录身份{
                            已触发组.关系组.front().源节点}, Gs, Gs);
                if (!既有) return 失败(L2结构状态::内部不一致, Gs);
                if (L2任务结构内部::子目标回流后继与请求一致(
                        *既有, 请求)) {
                    L2提交任务子目标回流后继筹办准备结果 结果;
                    结果.结果头 = {L2结构合同版本,
                        L2结构状态::精确重复,
                        既有->准备发布事实代次,
                        既有->准备发布事实代次};
                    结果.后继准备记录 = *既有;
                    return 结果;
                }
                L2提交任务子目标回流后继筹办准备结果 冲突 =
                    失败(L2结构状态::引用冲突, Gs);
                冲突.后继准备记录 = *既有;
                return 冲突;
            }

            const auto 分区 = L2任务结构内部::读取筹办轮次分区投影(
                L1_, 第一层写入端口_, 身份来源定位_, 类型定位_,
                初次筹办记录定位_, 筹办轮次定位_, 初次筹办治理定位_,
                {{L2结构合同版本, Gs}, L2读取类别::当前,
                    请求.任务, 0}, Gs, Gs);
            if (!分区.成功())
                return 失败(分区.结果头.状态, Gs);
            if (分区.分区 != L2任务筹办轮次分区::统一轮次权威
                || !分区.统一当前准备)
                return 失败(L2结构状态::引用冲突, Gs);
            const auto& 当前准备 = *分区.统一当前准备;
            const auto& 推进 = 请求.统一轮次推进;
            const auto 当前后继身份 = 当前准备.当前后继准备记录
                ? std::optional<L2任务后继筹办准备记录身份>{
                    当前准备.当前后继准备记录->身份}
                : std::nullopt;
            if (当前准备.任务.身份 != 请求.任务
                || 当前准备.轮次权威.身份 != 推进.轮次权威
                || 当前准备.首次准备记录.身份 != 推进.首次准备记录
                || 当前后继身份 != 推进.期望前一后继准备记录
                || 当前准备.当前筹办轮次 != 推进.期望前一筹办轮次)
                return 失败(L2结构状态::引用冲突, Gs);
            if (当前准备.当前筹办轮次 ==
                (std::numeric_limits<std::uint64_t>::max)())
                return 失败(L2结构状态::内部不一致, Gs);

            const auto 当前关系组 = L1_.读取所有者范围历史关系组({
                L1所有者范围CRUD合同版本,
                L1所有者范围关系端点方向::源, 请求.任务.值,
                筹办轮次定位_.关系类型[17], Gs});
            const auto 期望当前目标 = 当前后继身份
                ? 当前后继身份->值 : 当前准备.首次准备记录.身份.值;
            if (当前关系组.状态 != L1所有者范围读取状态::成功
                || 当前关系组.历史截止事实代次 != Gs
                || 当前关系组.读取事实代次 != Gs
                || 当前关系组.关系组.size() != 1
                || 当前关系组.关系组.front().目标节点 != 期望当前目标
                || 当前关系组.关系组.front().角色或顺序 != 1)
                return 失败(L2结构状态::内部不一致, Gs);

            const auto 写集 =
                L2任务结构内部::形成子目标回流后继筹办准备写集(
                    请求, 当前关系组.关系组.front().编码,
                    当前准备, 筹办轮次定位_);
            const auto 写入 = 第一层写入端口_.提交所有者范围中性写集(写集);
            const auto 状态 = L2任务结构内部::映射任务写入状态(写入.状态);
            if ((状态 != L2结构状态::已提交
                    && 状态 != L2结构状态::精确重复)
                || 写入.事实代次 == 0)
                return 失败(状态, 写入.事实代次);
            const auto 后继编码 = L2任务结构内部::查找治理写入编码(
                写入, L2任务结构内部::后继准备记录节点写入键);
            const auto 后继 = 后继编码
                ? L2任务结构内部::读取后继筹办准备历史投影(
                    L1_, 筹办轮次定位_,
                    L2任务后继筹办准备记录身份{*后继编码},
                    写入.事实代次, 写入.事实代次)
                : std::nullopt;
            if (!后继
                || !L2任务结构内部::子目标回流后继与请求一致(
                    *后继, 请求))
                return 失败(L2结构状态::内部不一致, 写入.事实代次);
            const auto 新分区 = L2任务结构内部::读取筹办轮次分区投影(
                L1_, 第一层写入端口_, 身份来源定位_, 类型定位_,
                初次筹办记录定位_, 筹办轮次定位_, 初次筹办治理定位_,
                {{L2结构合同版本, 写入.事实代次}, L2读取类别::当前,
                    请求.任务, 0}, 写入.事实代次, 写入.事实代次);
            if (!新分区.成功() || !新分区.统一当前准备
                || !新分区.统一当前准备->当前后继准备记录
                || 新分区.统一当前准备->当前后继准备记录->身份
                    != 后继->身份)
                return 失败(L2结构状态::内部不一致, 写入.事实代次);
            L2提交任务子目标回流后继筹办准备结果 结果;
            结果.结果头 = {L2结构合同版本, 状态,
                写入.事实代次, 写入.事实代次};
            结果.后继准备记录 = *后继;
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2结构状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致);
        }
    }

    L2任务结构服务(const L2任务结构服务&) = delete;
    L2任务结构服务& operator=(const L2任务结构服务&) = delete;

    L2按任务读取来源需求关系组结果 按任务读取来源需求关系组(
        L2按任务读取来源需求关系组请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [](L2结构状态 状态) noexcept {
            L2按任务读取来源需求关系组结果 结果;
            结果.结果头 = {L2结构合同版本, 状态, 0, std::nullopt};
            return 结果;
        };
        if (请求.合同版本 != L2按任务读取来源需求关系组合同版本
            || !L2结构请求头合同有效(请求.请求头)
            || 请求.请求头.期望事实代次 == 0
            || !有效(请求.任务.值))
            return 失败(L2结构状态::入口拒绝);
        try {
            const auto G0 = 请求.请求头.期望事实代次;
            const auto 读前 = L2任务结构内部::读取中性当前事实代次(L1_);
            if (读前 == 0 || 读前 != G0)
                return 失败(L2结构状态::事实代次漂移);

            L1所有者范围一致当前读取请求 查询;
            查询.合同版本 = L1所有者范围一致当前读取合同版本;
            查询.期望事实代次 = G0;
            查询.所有者 = {身份来源定位_.所有者};
            查询.节点 = {请求.任务.值};
            查询.源关系组 = {{请求.任务.值,
                类型定位_.任务来源需求关系类型}};
            const auto 读取 = L1_.尝试读取所有者范围一致当前投影(查询);
            if (读取.状态 != L1所有者范围一致当前读取状态::成功) {
                return 失败(L2任务结构内部::
                    映射任务身份来源一致读取失败状态(读取.状态));
            }
            if (读取.合同版本 != L1所有者范围一致当前读取合同版本
                || 读取.期望事实代次 != G0 || 读取.读取事实代次 != G0
                || 读取.所有者.size() != 1 || 读取.节点.size() != 1
                || !读取.关系.empty() || !读取.值.empty()
                || !读取.属性值.empty() || 读取.源关系组.size() != 1
                || !读取.目标关系组.empty())
                return 失败(L2结构状态::内部不一致);

            const auto& 所有者 = 读取.所有者.front();
            const auto& 任务节点项 = 读取.节点.front();
            const auto& 关系组 = 读取.源关系组.front();
            if (所有者.查询所有者 != 身份来源定位_.所有者
                || 所有者.状态
                    != L1所有者范围一致当前读取项目状态::成功
                || !所有者.所有者事实
                || 所有者.所有者事实->所有者 != 身份来源定位_.所有者
                || 所有者.所有者事实->退出事实代次)
                return 失败(L2结构状态::内部不一致);
            if (任务节点项.查询编码 != 请求.任务.值)
                return 失败(L2结构状态::内部不一致);
            if (任务节点项.状态
                != L1所有者范围一致当前读取项目状态::成功) {
                if (任务节点项.事实)
                    return 失败(L2结构状态::内部不一致);
                return 失败(L2任务结构内部::
                    映射任务身份来源当前项目状态(任务节点项.状态));
            }
            if (!任务节点项.事实
                || 任务节点项.事实->编码 != 请求.任务.值
                || 任务节点项.事实->写入所有者
                    != 身份来源定位_.所有者
                || 任务节点项.事实->种类 != 节点种类::普通
                || 任务节点项.事实->属性类型表示
                || 任务节点项.事实->创建事实代次 == 0
                || 任务节点项.事实->创建事实代次 > G0
                || 任务节点项.事实->退出事实代次
                || 关系组.源节点 != 请求.任务.值
                || 关系组.关系类型节点
                    != 类型定位_.任务来源需求关系类型)
                return 失败(L2结构状态::引用冲突);

            L2按任务读取来源需求关系组结果 结果;
            结果.来源需求关系组.reserve(关系组.成员.size());
            for (const auto& 成员 : 关系组.成员) {
                const auto& 关系 = 成员.关系;
                const auto& 需求节点 = 成员.对端节点;
                if (!有效(关系.编码)
                    || 关系.写入所有者 != 身份来源定位_.所有者
                    || 关系.源节点 != 请求.任务.值
                    || 关系.关系类型节点
                        != 类型定位_.任务来源需求关系类型
                    || 关系.角色或顺序 != 1
                    || !有效(关系.目标节点)
                    || 关系.创建事实代次 == 0
                    || 关系.创建事实代次 > G0
                    || 关系.退出事实代次
                    || 需求节点.编码 != 关系.目标节点
                    || 需求节点.种类 != 节点种类::普通
                    || 需求节点.属性类型表示
                    || 需求节点.创建事实代次 == 0
                    || 需求节点.创建事实代次 > G0
                    || 需求节点.退出事实代次)
                    return 失败(L2结构状态::引用冲突);
                结果.来源需求关系组.push_back({关系.编码, 请求.任务,
                    L2需求身份{关系.目标节点}, 1,
                    {关系.创建事实代次, std::nullopt}});
            }
            std::sort(结果.来源需求关系组.begin(),
                结果.来源需求关系组.end(), [](const auto& 左,
                    const auto& 右) noexcept {
                    return 左.来源需求.值.值 != 右.来源需求.值.值
                        ? 左.来源需求.值.值 < 右.来源需求.值.值
                        : 左.关系稳定编码.值 < 右.关系稳定编码.值;
                });
            for (std::size_t i = 0; i < 结果.来源需求关系组.size(); ++i)
                for (std::size_t j = 0; j < i; ++j)
                    if (结果.来源需求关系组[j].关系稳定编码
                        == 结果.来源需求关系组[i].关系稳定编码)
                        return 失败(L2结构状态::引用冲突);

            const auto 读后 = L2任务结构内部::读取中性当前事实代次(L1_);
            if (读后 == 0 || 读后 != G0)
                return 失败(L2结构状态::事实代次漂移);
            结果.结果头 = {L2结构合同版本, L2结构状态::已读取,
                G0, std::nullopt};
            结果.任务 = 请求.任务;
            结果.本次正式读回截止 = G0;
            if (!结果.成功() || 结果.任务 != 请求.任务
                || 结果.结果头.事实截止代次 != G0
                || 结果.本次正式读回截止 != G0)
                return 失败(L2结构状态::内部不一致);
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2结构状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致);
        }
    }

    // ===== 读取任务身份来源 =====
    L2任务身份来源读取结果 读取任务身份来源(
        L2任务身份来源读取请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 截止 = L2任务结构内部::读取中性当前事实代次(L1_);
        if (!L2任务身份来源读取请求有效(请求))
            return L2任务结构内部::形成任务身份来源读取失败(
                L2结构状态::入口拒绝, 截止);
        if (请求.读取类别 != L2读取类别::当前)
            return L2任务结构内部::形成任务身份来源读取失败(
                L2结构状态::入口拒绝, 截止);
        try {
            return L2任务结构内部::读取任务身份来源当前投影(
                L1_, 身份来源定位_, 请求, 截止);
        } catch (const std::bad_alloc&) {
            return L2任务结构内部::形成任务身份来源读取失败(
                L2结构状态::资源失败, 截止);
        } catch (...) {
            return L2任务结构内部::形成任务身份来源读取失败(
                L2结构状态::内部不一致, 截止);
        }
    }

    // ===== 新增任务 =====
    // 一次 owner-aware 原子写集：2 节点 + 3 关系同代发布。
    // 任务节点、虚拟存在节点、任务族归属、任务->需求列表项引用、任务->虚拟存在归属。
    L2新增任务结果 新增任务(L2新增任务请求 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [&](L2结构状态 状态,
            std::uint64_t 截止 = 0) noexcept {
            L2新增任务结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = 状态;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        };
        try {
            if (!L2新增任务请求有效(请求))
                return 失败(L2结构状态::入口拒绝);
            const auto 代次 = L2任务结构内部::检查任务写入当前代次(
                L1_, 请求.请求头.期望事实代次);
            if (!代次.成功() && !代次.直接提交)
                return 失败(代次.状态, 代次.事实截止代次);

            if (代次.成功()) {
                const L2按需求列表项读取当前任务请求 当前任务请求{
                    请求.请求头, 请求.需求列表项};
                const auto 当前任务 =
                    L2任务结构内部::按需求列表项读取当前任务当前投影(
                        L1_, 身份来源定位_, 类型定位_, 当前任务请求,
                        请求.请求头.期望事实代次);
                if (!当前任务.成功())
                    return 失败(当前任务.结果头.状态,
                        当前任务.结果头.事实截止代次);
                if (当前任务.任务)
                    return 失败(L2结构状态::引用冲突,
                        当前任务.结果头.事实截止代次);
            }

            // 跨服务验证需求列表项当前有效
            L2需求列表项身份读取请求 列表项请求;
            列表项请求.合同版本 = L2结构合同版本;
            列表项请求.列表项 = 请求.需求列表项;
            const auto 列表项验证 = 需求结构服务_.读取需求列表项身份(
                列表项请求);
            if (!列表项验证.成功())
                return 失败(L2结构状态::引用冲突, 代次.事实截止代次);

            const auto 本地键 = [](std::uint32_t 值) noexcept {
                return L1所有者范围写集本地键{值};
            };
            const auto 键任务节点 = 本地键(
                L2任务结构内部::任务节点写入本地键值);
            const auto 键虚拟存在节点 = 本地键(
                L2任务结构内部::虚拟存在节点写入本地键值);

            L1所有者范围写集请求 写集;
            写集.合同版本 = L1所有者范围CRUD合同版本;
            写集.期望事实代次 = 请求.请求头.期望事实代次;
            写集.写入幂等身份 = L2任务结构内部::映射任务幂等键(
                请求.幂等身份);

            // 2 个节点
            写集.节点 = {
                {键任务节点, 节点种类::普通, std::nullopt},
                {键虚拟存在节点, 节点种类::普通, std::nullopt}};

            // 3 条关系
            写集.关系 = {
                // 任务族归属：任务 -> 任务锚点，类型=任务族归属关系类型，角色=1
                {本地键(L2任务结构内部::任务族归属关系写入本地键值),
                    键任务节点, 身份来源定位_.任务锚点,
                    身份来源定位_.任务族归属关系类型, 1},
                // 任务 -> 需求列表项引用
                {本地键(L2任务结构内部::任务需求列表项引用关系写入本地键值),
                    键任务节点, 请求.需求列表项.值,
                    类型定位_.任务需求列表项引用关系类型, 1},
                // 任务 -> 虚拟存在归属
                {本地键(L2任务结构内部::任务虚拟存在归属关系写入本地键值),
                    键任务节点, 键虚拟存在节点,
                    身份来源定位_.虚拟存在归属关系类型, 1}};

            const auto 提交 = L2任务结构内部::提交并验证任务写集(
                第一层写入端口_, 写集);
            if (提交.状态 != L2结构状态::已提交
                && 提交.状态 != L2结构状态::精确重复)
                return 失败(提交.状态, 提交.事实截止代次);

            return L2任务结构内部::读回新增任务(
                L1_, 身份来源定位_, 类型定位_, 请求,
                提交.编码映射, 提交.状态, 提交.事实截止代次);
        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2结构状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致);
        }
    }

    // ===== P00：按任务同 G0 读取当前任务与 R1 =====
    L2按任务读取当前任务与轮次结果 按任务读取当前任务与轮次(
        L2按任务读取当前任务与轮次请求 请求) const noexcept {
        L2按任务读取当前任务与轮次结果 结果;
        if (!L2按任务读取当前任务与轮次请求有效(请求)) {
            结果.结果头 = {L2结构合同版本, L2结构状态::入口拒绝,
                0, std::nullopt};
            结果.状态 = L2按任务读取当前任务与轮次状态::入口拒绝;
            return 结果;
        }
        const auto 投影 = 读取当前任务轮次结构化投影(
            请求.任务, 请求.请求头.期望事实代次);
        结果.结果头 = {L2结构合同版本, 投影.公共状态,
            投影.本次正式读回截止, std::nullopt};
        结果.状态 = 投影.专用状态;
        结果.任务 = 投影.任务;
        结果.当前任务轮次 = 投影.当前任务轮次;
        结果.本次正式读回截止 = 投影.本次正式读回截止;
        if (结果.成功()
            && (结果.任务->身份 != 请求.任务
                || 结果.本次正式读回截止 != 请求.请求头.期望事实代次)) {
            L2按任务读取当前任务与轮次结果 失败结果;
            失败结果.结果头 = {L2结构合同版本,
                L2结构状态::内部不一致, 0, std::nullopt};
            失败结果.状态 = L2按任务读取当前任务与轮次状态::内部不一致;
            return 失败结果;
        }
        return 结果;
    }

    L2按任务读取当前任务与轮次结果 按任务读取当前任务与轮次原始(
        L2按任务读取当前任务与轮次请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [](L2结构状态 公共状态,
            L2按任务读取当前任务与轮次状态 专用状态) noexcept {
            L2按任务读取当前任务与轮次结果 结果;
            结果.结果头 = {L2结构合同版本, 公共状态, 0, std::nullopt};
            结果.状态 = 专用状态;
            return 结果;
        };
        if (!L2按任务读取当前任务与轮次请求有效(请求))
            return 失败(L2结构状态::入口拒绝,
                L2按任务读取当前任务与轮次状态::入口拒绝);
        try {
            const auto G0 = 请求.请求头.期望事实代次;
            const auto 当前代次 = L2任务结构内部::读取中性当前事实代次(L1_);
            if (当前代次 == 0)
                return 失败(L2结构状态::内部不一致,
                    L2按任务读取当前任务与轮次状态::内部不一致);
            if (当前代次 != G0)
                return 失败(L2结构状态::事实代次漂移,
                    L2按任务读取当前任务与轮次状态::事实代次漂移);
            const auto 节点读取 = L1_.读取所有者范围历史事实(
                {L1所有者范围CRUD合同版本, 请求.任务.值});
            const auto* 任务节点 = 节点读取.事实
                ? std::get_if<L1所有者范围节点事实>(&*节点读取.事实) : nullptr;
            if (节点读取.状态 == L1所有者范围读取状态::未找到)
                return 失败(L2结构状态::未找到,
                    L2按任务读取当前任务与轮次状态::任务未找到);
            if (节点读取.状态 == L1所有者范围读取状态::许可拒绝)
                return 失败(L2结构状态::许可拒绝,
                    L2按任务读取当前任务与轮次状态::许可拒绝);
            if (节点读取.状态 == L1所有者范围读取状态::资源失败)
                return 失败(L2结构状态::资源失败,
                    L2按任务读取当前任务与轮次状态::资源失败);
            if (节点读取.状态 != L1所有者范围读取状态::成功 || !任务节点)
                return 失败(L2结构状态::内部不一致,
                    L2按任务读取当前任务与轮次状态::内部不一致);
            if (任务节点->退出事实代次
                && *任务节点->退出事实代次 <= G0)
                return 失败(L2结构状态::已退出,
                    L2按任务读取当前任务与轮次状态::任务已退出);
            if (任务节点->写入所有者 != 第一层写入端口_.所有者身份()
                || 任务节点->种类 != 节点种类::普通
                || 任务节点->创建事实代次 == 0
                || 任务节点->创建事实代次 > G0)
                return 失败(L2结构状态::内部不一致,
                    L2按任务读取当前任务与轮次状态::引用冲突);
            enum class 关系读取诊断 : std::uint8_t {
                成功, 未找到, 多义, 许可拒绝, 资源失败, 事实代次漂移, 内部不一致
            };
            const auto 读唯一关系 = [&](稳定编码 类型,
                关系读取诊断* 诊断 = nullptr)
                -> std::optional<L1所有者范围关系事实> {
                const auto 读取 = L1_.读取所有者范围历史关系组({
                    L1所有者范围CRUD合同版本,
                    L1所有者范围关系端点方向::源, 请求.任务.值,
                    类型, G0});
                if (读取.状态 == L1所有者范围读取状态::许可拒绝) {
                    if (诊断) *诊断 = 关系读取诊断::许可拒绝;
                    return std::nullopt;
                }
                if (读取.状态 == L1所有者范围读取状态::资源失败) {
                    if (诊断) *诊断 = 关系读取诊断::资源失败;
                    return std::nullopt;
                }
                if (读取.状态 != L1所有者范围读取状态::成功
                    || 读取.历史截止事实代次 != G0
                    || 读取.读取事实代次 != G0) {
                    if (诊断) *诊断 =
                        读取.读取事实代次 != G0
                            ? 关系读取诊断::事实代次漂移
                            : 关系读取诊断::内部不一致;
                    return std::nullopt;
                }
                if (读取.关系组.empty()) {
                    if (诊断) *诊断 = 关系读取诊断::未找到;
                    return std::nullopt;
                }
                if (读取.关系组.size() > 1) {
                    if (诊断) *诊断 = 关系读取诊断::多义;
                    return std::nullopt;
                }
                const auto& 关系 = 读取.关系组.front();
                if (关系.写入所有者 != 第一层写入端口_.所有者身份()
                    || 关系.源节点 != 请求.任务.值
                    || 关系.关系类型节点 != 类型
                    || 关系.角色或顺序 != 1
                    || 关系.创建事实代次 == 0
                    || 关系.创建事实代次 > G0
                    || (关系.退出事实代次 && *关系.退出事实代次 <= G0)) {
                    if (诊断) *诊断 = 关系读取诊断::内部不一致;
                    return std::nullopt;
                }
                if (诊断) *诊断 = 关系读取诊断::成功;
                return 关系;
            };
            const auto 身份来源读取 = L2任务结构内部::读取任务身份来源当前投影(
                L1_, 身份来源定位_, {{L2结构合同版本, G0},
                    L2读取类别::当前, 请求.任务, 0}, G0);
            if (身份来源读取.结果头.状态 == L2结构状态::许可拒绝)
                return 失败(L2结构状态::许可拒绝,
                    L2按任务读取当前任务与轮次状态::许可拒绝);
            if (身份来源读取.结果头.状态 == L2结构状态::资源失败)
                return 失败(L2结构状态::资源失败,
                    L2按任务读取当前任务与轮次状态::资源失败);
            if (身份来源读取.结果头.状态 == L2结构状态::事实代次漂移)
                return 失败(L2结构状态::事实代次漂移,
                    L2按任务读取当前任务与轮次状态::事实代次漂移);
            if (!身份来源读取.成功()
                || 身份来源读取.结果头.事实截止代次 != G0
                || 身份来源读取.身份来源->任务节点 != 请求.任务.值
                || 身份来源读取.身份来源->任务节点生命周期.创建事实代次
                    != 任务节点->创建事实代次
                || 身份来源读取.身份来源->族归属关系生命周期.创建事实代次
                    != 任务节点->创建事实代次)
                return 失败(L2结构状态::内部不一致,
                    L2按任务读取当前任务与轮次状态::引用冲突);
            关系读取诊断 列表关系诊断 = 关系读取诊断::内部不一致;
            关系读取诊断 正式存在关系诊断 = 关系读取诊断::内部不一致;
            const auto 列表关系 = 读唯一关系(
                类型定位_.任务需求列表项引用关系类型, &列表关系诊断);
            const auto 正式存在关系 = 读唯一关系(
                类型定位_.任务正式存在关系类型, &正式存在关系诊断);
            关系读取诊断 当前关系诊断 = 关系读取诊断::内部不一致;
            const auto 当前关系 = 读唯一关系(
                任务轮次核心定位_.任务当前轮次关系类型, &当前关系诊断);
            if ((!列表关系 || !正式存在关系)
                && (列表关系诊断 == 关系读取诊断::许可拒绝
                    || 正式存在关系诊断 == 关系读取诊断::许可拒绝))
                return 失败(L2结构状态::许可拒绝,
                    L2按任务读取当前任务与轮次状态::许可拒绝);
            if ((!列表关系 || !正式存在关系)
                && (列表关系诊断 == 关系读取诊断::资源失败
                    || 正式存在关系诊断 == 关系读取诊断::资源失败))
                return 失败(L2结构状态::资源失败,
                    L2按任务读取当前任务与轮次状态::资源失败);
            if ((!列表关系 || !正式存在关系)
                && (列表关系诊断 == 关系读取诊断::事实代次漂移
                    || 正式存在关系诊断 == 关系读取诊断::事实代次漂移))
                return 失败(L2结构状态::事实代次漂移,
                    L2按任务读取当前任务与轮次状态::事实代次漂移);
            if (!列表关系 || !正式存在关系)
                return 失败(L2结构状态::内部不一致,
                    L2按任务读取当前任务与轮次状态::引用冲突);
            if (!当前关系) {
                switch (当前关系诊断) {
                case 关系读取诊断::未找到:
                    return 失败(L2结构状态::内部不一致,
                        L2按任务读取当前任务与轮次状态::当前轮次未找到);
                case 关系读取诊断::多义:
                    return 失败(L2结构状态::内部不一致,
                        L2按任务读取当前任务与轮次状态::关系多义);
                case 关系读取诊断::许可拒绝:
                    return 失败(L2结构状态::许可拒绝,
                        L2按任务读取当前任务与轮次状态::许可拒绝);
                case 关系读取诊断::资源失败:
                    return 失败(L2结构状态::资源失败,
                        L2按任务读取当前任务与轮次状态::资源失败);
                case 关系读取诊断::事实代次漂移:
                    return 失败(L2结构状态::事实代次漂移,
                        L2按任务读取当前任务与轮次状态::事实代次漂移);
                default:
                    return 失败(L2结构状态::内部不一致,
                        L2按任务读取当前任务与轮次状态::引用冲突);
                }
            }
            const auto 正式存在节点读取 = L1_.读取所有者范围历史事实({
                L1所有者范围CRUD合同版本, 正式存在关系->目标节点});
            const auto* 正式存在节点 = 正式存在节点读取.事实
                ? std::get_if<L1所有者范围节点事实>(
                    &*正式存在节点读取.事实) : nullptr;
            if (正式存在节点读取.状态 == L1所有者范围读取状态::许可拒绝)
                return 失败(L2结构状态::许可拒绝,
                    L2按任务读取当前任务与轮次状态::许可拒绝);
            if (正式存在节点读取.状态 == L1所有者范围读取状态::资源失败)
                return 失败(L2结构状态::资源失败,
                    L2按任务读取当前任务与轮次状态::资源失败);
            if (正式存在节点读取.状态 != L1所有者范围读取状态::成功
                || !正式存在节点
                || 正式存在节点->种类 != 节点种类::普通
                || 正式存在节点->属性类型表示
                || 正式存在节点->创建事实代次 == 0
                || 正式存在节点->创建事实代次 > G0
                || (正式存在节点->退出事实代次
                    && *正式存在节点->退出事实代次 <= G0))
                return 失败(L2结构状态::内部不一致,
                    L2按任务读取当前任务与轮次状态::引用冲突);
            const auto 轮次读取 = L1_.读取所有者范围历史事实(
                {L1所有者范围CRUD合同版本, 当前关系->目标节点});
            const auto* 轮次节点 = 轮次读取.事实
                ? std::get_if<L1所有者范围节点事实>(&*轮次读取.事实) : nullptr;
            if (轮次读取.状态 == L1所有者范围读取状态::许可拒绝)
                return 失败(L2结构状态::许可拒绝,
                    L2按任务读取当前任务与轮次状态::许可拒绝);
            if (轮次读取.状态 == L1所有者范围读取状态::资源失败)
                return 失败(L2结构状态::资源失败,
                    L2按任务读取当前任务与轮次状态::资源失败);
            if (轮次读取.状态 != L1所有者范围读取状态::成功 || !轮次节点
                || 轮次节点->写入所有者 != 第一层写入端口_.所有者身份()
                || 轮次节点->种类 != 节点种类::普通
                || 轮次节点->创建事实代次 == 0
                || 轮次节点->创建事实代次 > G0
                || (轮次节点->退出事实代次
                    && *轮次节点->退出事实代次 <= G0))
                return 失败(L2结构状态::内部不一致,
                    L2按任务读取当前任务与轮次状态::引用冲突);
            const auto 轮次关系 = [&](稳定编码 类型,
                关系读取诊断* 诊断 = nullptr)
                -> std::optional<L1所有者范围关系事实> {
                const auto 读取 = L1_.读取所有者范围历史关系组({
                    L1所有者范围CRUD合同版本,
                    L1所有者范围关系端点方向::源, 当前关系->目标节点,
                    类型, G0});
                if (读取.状态 == L1所有者范围读取状态::许可拒绝) {
                    if (诊断) *诊断 = 关系读取诊断::许可拒绝;
                    return std::nullopt;
                }
                if (读取.状态 == L1所有者范围读取状态::资源失败) {
                    if (诊断) *诊断 = 关系读取诊断::资源失败;
                    return std::nullopt;
                }
                if (读取.状态 != L1所有者范围读取状态::成功
                    || 读取.历史截止事实代次 != G0
                    || 读取.读取事实代次 != G0) {
                    if (诊断) *诊断 =
                        读取.读取事实代次 != G0
                            ? 关系读取诊断::事实代次漂移
                            : 关系读取诊断::内部不一致;
                    return std::nullopt;
                }
                if (读取.关系组.empty()) {
                    if (诊断) *诊断 = 关系读取诊断::未找到;
                    return std::nullopt;
                }
                if (读取.关系组.size() > 1) {
                    if (诊断) *诊断 = 关系读取诊断::多义;
                    return std::nullopt;
                }
                const auto& 关系 = 读取.关系组.front();
                if (关系.写入所有者 != 第一层写入端口_.所有者身份()
                    || 关系.源节点 != 当前关系->目标节点
                    || 关系.关系类型节点 != 类型
                    || 关系.角色或顺序 != 1
                    || 关系.创建事实代次 == 0 || 关系.创建事实代次 > G0
                    || (关系.退出事实代次 && *关系.退出事实代次 <= G0)) {
                    if (诊断) *诊断 = 关系读取诊断::内部不一致;
                    return std::nullopt;
                }
                if (诊断) *诊断 = 关系读取诊断::成功;
                return 关系;
            };
            关系读取诊断 轮次族诊断 = 关系读取诊断::内部不一致;
            关系读取诊断 轮次任务诊断 = 关系读取诊断::内部不一致;
            const auto 轮次族 = 轮次关系(
                任务轮次核心定位_.任务轮次族归属关系类型, &轮次族诊断);
            const auto 轮次任务 = 轮次关系(
                任务轮次核心定位_.任务轮次任务关系类型, &轮次任务诊断);
            if ((!轮次族 || !轮次任务)
                && (轮次族诊断 == 关系读取诊断::许可拒绝
                    || 轮次任务诊断 == 关系读取诊断::许可拒绝))
                return 失败(L2结构状态::许可拒绝,
                    L2按任务读取当前任务与轮次状态::许可拒绝);
            if ((!轮次族 || !轮次任务)
                && (轮次族诊断 == 关系读取诊断::资源失败
                    || 轮次任务诊断 == 关系读取诊断::资源失败))
                return 失败(L2结构状态::资源失败,
                    L2按任务读取当前任务与轮次状态::资源失败);
            if ((!轮次族 || !轮次任务)
                && (轮次族诊断 == 关系读取诊断::事实代次漂移
                    || 轮次任务诊断 == 关系读取诊断::事实代次漂移))
                return 失败(L2结构状态::事实代次漂移,
                    L2按任务读取当前任务与轮次状态::事实代次漂移);
            if (!轮次族 || !轮次任务
                || 轮次族->目标节点 != 任务轮次核心定位_.任务轮次锚点
                || 轮次任务->目标节点 != 请求.任务.值)
                return 失败(L2结构状态::内部不一致,
                    L2按任务读取当前任务与轮次状态::引用冲突);
            const auto 值读取 = L1_.读取所有者范围历史属性值组(
                {L1所有者范围CRUD合同版本, 当前关系->目标节点, G0});
            if (值读取.状态 == L1所有者范围读取状态::许可拒绝)
                return 失败(L2结构状态::许可拒绝,
                    L2按任务读取当前任务与轮次状态::许可拒绝);
            if (值读取.状态 == L1所有者范围读取状态::资源失败)
                return 失败(L2结构状态::资源失败,
                    L2按任务读取当前任务与轮次状态::资源失败);
            if (值读取.读取事实代次 != G0)
                return 失败(L2结构状态::事实代次漂移,
                    L2按任务读取当前任务与轮次状态::事实代次漂移);
            if (值读取.状态 != L1所有者范围读取状态::成功
                || 值读取.历史截止事实代次 != G0
                || 值读取.读取事实代次 != G0)
                return 失败(L2结构状态::内部不一致,
                    L2按任务读取当前任务与轮次状态::内部不一致);
            const L1所有者范围值事实* 序号事实 = nullptr;
            for (const auto& 值 : 值读取.属性值组) {
                if (值.属性类型节点
                    == 任务轮次核心定位_.任务轮次序号属性类型) {
                    if (序号事实) return 失败(L2结构状态::内部不一致,
                        L2按任务读取当前任务与轮次状态::引用冲突);
                    序号事实 = &值;
                }
            }
            const auto* 序号 = 序号事实
                ? std::get_if<std::vector<std::uint64_t>>(&序号事实->材料)
                : nullptr;
            if (!序号事实 || !序号 || 序号->size() != 1 || 序号->front() == 0
                || 序号事实->所属节点 != 当前关系->目标节点
                || 序号事实->来源节点 != 当前关系->目标节点
                || 序号事实->创建事实代次 == 0
                || 序号事实->创建事实代次 > G0
                || 序号事实->退出事实代次)
                return 失败(L2结构状态::内部不一致,
                    L2按任务读取当前任务与轮次状态::引用冲突);
            const auto 轮次序号 = 序号->front();
            L2按任务读取当前任务与轮次结果 结果;
            结果.结果头 = {L2结构合同版本, L2结构状态::已读取,
                G0, std::nullopt};
            结果.状态 = L2按任务读取当前任务与轮次状态::已读取;
            结果.任务 = L2任务事实{请求.任务,
                L2需求列表项身份{列表关系->目标节点},
                L2任务虚拟存在身份{正式存在关系->目标节点},
                {任务节点->创建事实代次, 任务节点->退出事实代次}};
            结果.当前任务轮次 = L2任务轮次事实{
                L2任务轮次身份{当前关系->目标节点}, 请求.任务, 轮次序号,
                {轮次节点->创建事实代次, 轮次节点->退出事实代次}};
            结果.本次正式读回截止 = G0;
            const auto 读后代次 = L2任务结构内部::读取中性当前事实代次(L1_);
            if (读后代次 == 0)
                return 失败(L2结构状态::内部不一致,
                    L2按任务读取当前任务与轮次状态::内部不一致);
            if (读后代次 != G0)
                return 失败(L2结构状态::事实代次漂移,
                    L2按任务读取当前任务与轮次状态::事实代次漂移);
            return 结果.成功() ? 结果 : 失败(L2结构状态::内部不一致,
                L2按任务读取当前任务与轮次状态::内部不一致);
        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败,
                L2按任务读取当前任务与轮次状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2结构状态::资源失败,
                L2按任务读取当前任务与轮次状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致,
                L2按任务读取当前任务与轮次状态::内部不一致);
        }
    }

    // ===== 读取任务 =====
    L2任务读取结果 读取任务(
        L2任务读取请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 截止 = L2任务结构内部::读取中性当前事实代次(L1_);
        if (!L2任务读取请求有效(请求) || 截止 == 0) {
            L2任务读取结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = L2结构状态::入口拒绝;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        }
        if (请求.读取类别 != L2读取类别::当前) {
            L2任务读取结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = L2结构状态::入口拒绝;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        }
        try {
            return L2任务结构内部::读取任务当前投影(
                L1_, 身份来源定位_, 类型定位_, 请求, 截止);
        } catch (const std::bad_alloc&) {
            L2任务读取结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = L2结构状态::资源失败;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        } catch (...) {
            L2任务读取结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = L2结构状态::内部不一致;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        }
    }

    // ===== 按需求列表项读取当前任务 =====
    L2按需求列表项读取当前任务结果 按需求列表项读取当前任务(
        L2按需求列表项读取当前任务请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 截止 = L2任务结构内部::读取中性当前事实代次(L1_);
        if (!L2按需求列表项读取当前任务请求有效(请求) || 截止 == 0) {
            L2按需求列表项读取当前任务结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = L2结构状态::入口拒绝;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        }
        try {
            return L2任务结构内部::按需求列表项读取当前任务当前投影(
                L1_, 身份来源定位_, 类型定位_, 请求, 截止);
        } catch (const std::bad_alloc&) {
            L2按需求列表项读取当前任务结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = L2结构状态::资源失败;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        } catch (...) {
            L2按需求列表项读取当前任务结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = L2结构状态::内部不一致;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        }
    }

    // ===== 读取任务虚拟存在 =====
    L2任务虚拟存在读取结果 读取任务虚拟存在(
        L2任务虚拟存在读取请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 截止 = L2任务结构内部::读取中性当前事实代次(L1_);
        if (!L2任务虚拟存在读取请求有效(请求) || 截止 == 0) {
            L2任务虚拟存在读取结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = L2结构状态::入口拒绝;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        }
        if (请求.读取类别 != L2读取类别::当前) {
            L2任务虚拟存在读取结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = L2结构状态::入口拒绝;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        }
        try {
            return L2任务结构内部::读取任务虚拟存在当前投影(
                L1_, 身份来源定位_, 请求, 截止);
        } catch (const std::bad_alloc&) {
            L2任务虚拟存在读取结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = L2结构状态::资源失败;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        } catch (...) {
            L2任务虚拟存在读取结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = L2结构状态::内部不一致;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        }
    }

    // ===== 退出任务 =====
    // 同代闭合任务核心 owner 自有结构：退出任务节点、族归属关系、
    // 需求列表项引用关系、任务虚拟存在归属关系。后继强引用保护由 L1 闭包承接。
    L2任务退出结果 退出任务(
        L2任务退出请求 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [&](L2结构状态 状态,
            std::uint64_t 截止 = 0) noexcept {
            L2任务退出结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = 状态;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        };
        try {
            if (!L2任务退出请求有效(请求))
                return 失败(L2结构状态::入口拒绝);
            const auto 代次 = L2任务结构内部::检查任务写入当前代次(
                L1_, 请求.请求头.期望事实代次);
            if (!代次.成功() && !代次.直接提交)
                return 失败(代次.状态, 代次.事实截止代次);

            // 验证任务及其自有关系当前有效
            L1所有者范围一致当前读取请求 L1请求;
            L1请求.合同版本 = L1所有者范围一致当前读取合同版本;
            L1请求.期望事实代次 = 请求.请求头.期望事实代次;
            L1请求.所有者 = {身份来源定位_.所有者};
            L1请求.节点 = {请求.任务.值, 身份来源定位_.任务锚点,
                身份来源定位_.任务族归属关系类型};
            L1请求.源关系组 = {
                {请求.任务.值, 身份来源定位_.任务族归属关系类型},
                {请求.任务.值, 类型定位_.任务需求列表项引用关系类型},
                {请求.任务.值, 身份来源定位_.虚拟存在归属关系类型}};
            const auto 读取 = L1_.尝试读取所有者范围一致当前投影(L1请求);
            const auto 顶层 = L2任务结构内部::
                解释任务身份来源一致读取顶层(读取,
                    请求.请求头.期望事实代次);
            if (!顶层.成功())
                return 失败(顶层.状态, 顶层.事实截止代次);
            const auto 闭包状态 = L2任务结构内部::
                验证任务退出事实闭包当前有效(读取,
                    身份来源定位_, 类型定位_, 请求.任务.值);
            if (闭包状态 != L2结构状态::已读取)
                return 失败(闭包状态, 顶层.事实截止代次);

            // 查找 3 条关系的编码用于退出
            const auto 查找关系编码 = [&](
                稳定编码 关系类型) -> 稳定编码 {
                for (const auto& 组 : 读取.源关系组) {
                    if (组.关系类型节点 != 关系类型) continue;
                    if (组.成员.size() != 1) continue;
                    return 组.成员.front().关系.编码;
                }
                return {};
            };
            const auto 族归属编码 = 查找关系编码(
                身份来源定位_.任务族归属关系类型);
            const auto 列表项引用编码 = 查找关系编码(
                类型定位_.任务需求列表项引用关系类型);
            const auto 虚拟存在归属编码 = 查找关系编码(
                身份来源定位_.虚拟存在归属关系类型);

            // 从虚拟存在归属关系获取虚拟存在节点编码
            const auto 虚拟存在编码 = [&]() -> 稳定编码 {
                for (const auto& 组 : 读取.源关系组) {
                    if (组.关系类型节点
                        != 身份来源定位_.虚拟存在归属关系类型)
                        continue;
                    if (组.成员.size() != 1) continue;
                    return 组.成员.front().关系.目标节点;
                }
                return {};
            }();

            // 首次准备记录保留为不可变历史；任务退出只同代闭合其
            // 记录->任务与记录->任务虚拟存在强关系。
            L1所有者范围一致当前读取请求 记录关系请求;
            记录关系请求.合同版本 = L1所有者范围一致当前读取合同版本;
            记录关系请求.期望事实代次 = 请求.请求头.期望事实代次;
            记录关系请求.所有者 = {身份来源定位_.所有者};
            记录关系请求.目标关系组 = {
                {请求.任务.值,
                    初次筹办记录定位_.关系类型.记录任务关系类型},
                {虚拟存在编码,
                    初次筹办记录定位_.关系类型.记录任务虚拟存在关系类型}};
            const auto 记录关系读取 =
                L1_.尝试读取所有者范围一致当前投影(记录关系请求);
            if (记录关系读取.状态
                    != L1所有者范围一致当前读取状态::成功
                || 记录关系读取.读取事实代次
                    != 请求.请求头.期望事实代次
                || 记录关系读取.目标关系组.size() != 2)
                return 失败(L2结构状态::内部不一致,
                    记录关系读取.读取事实代次);
            std::optional<稳定编码> 记录任务关系编码;
            std::optional<稳定编码> 记录虚拟关系编码;
            std::optional<稳定编码> 首次记录节点;
            for (const auto& 组 : 记录关系读取.目标关系组) {
                if (组.成员.empty()) continue;
                if (组.成员.size() != 1)
                    return 失败(L2结构状态::内部不一致,
                        记录关系读取.读取事实代次);
                const auto& 成员 = 组.成员.front();
                if (成员.关系.目标节点 != 组.目标节点
                    || 成员.关系.关系类型节点 != 组.关系类型节点
                    || 成员.关系.退出事实代次)
                    return 失败(L2结构状态::内部不一致,
                        记录关系读取.读取事实代次);
                if (首次记录节点 && *首次记录节点 != 成员.关系.源节点)
                    return 失败(L2结构状态::内部不一致,
                        记录关系读取.读取事实代次);
                首次记录节点 = 成员.关系.源节点;
                if (组.关系类型节点
                    == 初次筹办记录定位_.关系类型.记录任务关系类型)
                    记录任务关系编码 = 成员.关系.编码;
                else if (组.关系类型节点 == 初次筹办记录定位_.关系类型.
                        记录任务虚拟存在关系类型)
                    记录虚拟关系编码 = 成员.关系.编码;
                else return 失败(L2结构状态::内部不一致,
                    记录关系读取.读取事实代次);
            }
            if (记录任务关系编码.has_value()
                    != 记录虚拟关系编码.has_value())
                return 失败(L2结构状态::内部不一致,
                    记录关系读取.读取事实代次);

            const auto 读轮次关系组 = [&](L1所有者范围关系端点方向 方向,
                稳定编码 端点, std::size_t 类型索引) {
                return L1_.读取所有者范围历史关系组({
                    L1所有者范围CRUD合同版本, 方向, 端点,
                    筹办轮次定位_.关系类型[类型索引],
                    请求.请求头.期望事实代次});
            };
            const auto 当前筹办组 = 读轮次关系组(
                L1所有者范围关系端点方向::源, 请求.任务.值, 17);
            const auto 权威任务组 = 读轮次关系组(
                L1所有者范围关系端点方向::目标, 请求.任务.值, 1);
            const auto 权威虚拟组 = 读轮次关系组(
                L1所有者范围关系端点方向::目标, 虚拟存在编码, 2);
            if (当前筹办组.状态 != L1所有者范围读取状态::成功
                || 权威任务组.状态 != L1所有者范围读取状态::成功
                || 权威虚拟组.状态 != L1所有者范围读取状态::成功
                || 当前筹办组.读取事实代次 != 请求.请求头.期望事实代次
                || 权威任务组.读取事实代次 != 请求.请求头.期望事实代次
                || 权威虚拟组.读取事实代次 != 请求.请求头.期望事实代次
                || 当前筹办组.关系组.size() > 1
                || 权威任务组.关系组.size() > 1
                || 权威虚拟组.关系组.size() > 1
                || 当前筹办组.关系组.empty() != 权威任务组.关系组.empty()
                || 当前筹办组.关系组.empty() != 权威虚拟组.关系组.empty())
                return 失败(L2结构状态::内部不一致,
                    请求.请求头.期望事实代次);
            if (!权威任务组.关系组.empty()
                && 权威任务组.关系组.front().源节点
                    != 权威虚拟组.关系组.front().源节点)
                return 失败(L2结构状态::内部不一致,
                    请求.请求头.期望事实代次);

            // 构造退出写集：退出 2 节点 + 3 关系
            L1所有者范围写集请求 写集;
            写集.合同版本 = L1所有者范围CRUD合同版本;
            写集.期望事实代次 = 请求.请求头.期望事实代次;
            写集.写入幂等身份 = L2任务结构内部::映射任务幂等键(
                请求.幂等身份);
            写集.退出事实 = {请求.任务.值, 虚拟存在编码,
                族归属编码, 列表项引用编码, 虚拟存在归属编码};
            if (记录任务关系编码) {
                写集.退出事实.push_back(*记录任务关系编码);
                写集.退出事实.push_back(*记录虚拟关系编码);
            }
            if (!当前筹办组.关系组.empty()) {
                写集.退出事实.push_back(当前筹办组.关系组.front().编码);
                写集.退出事实.push_back(权威任务组.关系组.front().编码);
                写集.退出事实.push_back(权威虚拟组.关系组.front().编码);
            }

            const auto 提交 = L2任务结构内部::提交并验证任务退出写集(
                第一层写入端口_, 写集);
            if (提交.状态 != L2结构状态::已提交
                && 提交.状态 != L2结构状态::精确重复)
                return 失败(提交.状态, 提交.事实截止代次);

            return L2任务结构内部::读回退出任务(
                L1_, 身份来源定位_, 请求,
                虚拟存在编码, 族归属编码, 列表项引用编码,
                虚拟存在归属编码,
                提交.事实截止代次);
        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致);
        }
    }

private:
    friend class L2任务方法路径写入端口;
    friend class L2任务子目标承接记录写入端口;
    friend std::optional<L2任务方法路径写入端口>
    尝试形成L2任务方法路径写入端口(
        L2任务结构服务&, L2任务方法路径写入能力交付&&) noexcept;
    friend std::optional<L2任务子目标承接记录写入端口>
    尝试形成L2任务子目标承接记录写入端口(
        L2任务结构服务&,
        L2任务子目标承接记录写入能力交付&&) noexcept;

    L2任务子目标承接记录写入结果 写入已验证新增任务子目标承接记录(
        L2新增任务子目标承接记录请求 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [](L2结构状态 状态, std::uint64_t 截止 = 0) {
            L2任务子目标承接记录写入结果 结果;
            结果.结果头 = {L2结构合同版本, 状态, 截止, std::nullopt};
            return 结果;
        };
        try {
            if (!L2新增任务子目标承接记录请求有效(请求))
                return 失败(L2结构状态::入口拒绝);
            const auto 当前代次 = L2任务结构内部::读取中性当前事实代次(L1_);
            if (当前代次 == 0) return 失败(L2结构状态::内部不一致);
            const auto 写集 = L2任务结构内部::形成新增任务子目标承接记录写集(
                请求, 子目标记录定位_);
            const auto 提交 = L2任务结构内部::提交任务子目标承接记录写集(
                第一层写入端口_, 写集, 当前代次);
            if (提交.状态 != L2结构状态::已提交
                && 提交.状态 != L2结构状态::精确重复)
                return 失败(提交.状态, 提交.事实代次);
            稳定编码 记录编码;
            for (const auto& [本地键, 编码] : 提交.编码映射)
                if (本地键.值 == L2任务结构内部::子目标记录节点写入键)
                    记录编码 = 编码;
            if (!有效(记录编码)) return 失败(L2结构状态::内部不一致, 提交.事实代次);
            const auto 投影 = L2任务结构内部::读取任务子目标承接记录历史投影(
                L1_, 子目标记录定位_, L2任务子目标承接记录身份{记录编码},
                提交.事实代次, L2任务结构内部::读取中性当前事实代次(L1_));
            if (!投影) return 失败(L2结构状态::内部不一致, 提交.事实代次);
            L2任务子目标承接记录写入结果 结果;
            结果.结果头 = {L2结构合同版本, 提交.状态,
                提交.事实代次, 提交.事实代次};
            结果.记录 = *投影;
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2结构状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致);
        }
    }

    L2任务子目标承接记录写入结果 写入已验证绑定任务子目标承接记录子需求(
        L2绑定任务子目标承接记录子需求请求 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [](L2结构状态 状态, std::uint64_t 截止 = 0) {
            L2任务子目标承接记录写入结果 结果;
            结果.结果头 = {L2结构合同版本, 状态, 截止, std::nullopt};
            return 结果;
        };
        try {
            if (!L2绑定任务子目标承接记录子需求请求有效(请求))
                return 失败(L2结构状态::入口拒绝);
            const auto 当前代次 = L2任务结构内部::读取中性当前事实代次(L1_);
            const auto 前态 = L2任务结构内部::读取任务子目标承接记录历史投影(
                L1_, 子目标记录定位_, 请求.记录,
                请求.请求头.期望事实代次, 当前代次);
            if (!前态) return 失败(当前代次 == 请求.请求头.期望事实代次
                ? L2结构状态::未找到 : L2结构状态::事实代次漂移, 当前代次);
            if (前态->状态 != L2任务子目标承接记录状态::待绑定子需求
                || 前态->子需求 || 前态->回流)
                return 失败(L2结构状态::引用冲突, 当前代次);
            const auto 旧状态 = L2任务结构内部::查找记录当前属性值(
                L1_, 子目标记录定位_, 请求.记录,
                子目标记录定位_.状态属性类型,
                请求.请求头.期望事实代次, 当前代次);
            if (!旧状态) return 失败(L2结构状态::内部不一致, 当前代次);
            const auto 写集 = L2任务结构内部::形成绑定任务子目标承接记录写集(
                请求, 子目标记录定位_, *旧状态);
            const auto 提交 = L2任务结构内部::提交任务子目标承接记录写集(
                第一层写入端口_, 写集, 当前代次);
            if (提交.状态 != L2结构状态::已提交
                && 提交.状态 != L2结构状态::精确重复)
                return 失败(提交.状态, 提交.事实代次);
            const auto 观察 = L2任务结构内部::读取中性当前事实代次(L1_);
            const auto 投影 = L2任务结构内部::读取任务子目标承接记录历史投影(
                L1_, 子目标记录定位_, 请求.记录, 提交.事实代次, 观察);
            if (!投影 || 投影->状态 != L2任务子目标承接记录状态::等待子需求
                || !投影->子需求 || *投影->子需求 != 请求.子需求)
                return 失败(L2结构状态::内部不一致, 提交.事实代次);
            return {{L2结构合同版本, 提交.状态, 提交.事实代次,
                提交.事实代次}, *投影};
        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2结构状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致);
        }
    }

    L2任务子目标承接记录写入结果 写入已验证登记任务子目标承接记录回流(
        L2登记任务子目标承接记录回流请求 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [](L2结构状态 状态, std::uint64_t 截止 = 0) {
            L2任务子目标承接记录写入结果 结果;
            结果.结果头 = {L2结构合同版本, 状态, 截止, std::nullopt};
            return 结果;
        };
        try {
            if (!L2登记任务子目标承接记录回流请求有效(请求))
                return 失败(L2结构状态::入口拒绝);
            const auto 当前代次 = L2任务结构内部::读取中性当前事实代次(L1_);
            const auto 前态 = L2任务结构内部::读取任务子目标承接记录历史投影(
                L1_, 子目标记录定位_, 请求.记录,
                请求.请求头.期望事实代次, 当前代次);
            if (!前态) return 失败(当前代次 == 请求.请求头.期望事实代次
                ? L2结构状态::未找到 : L2结构状态::事实代次漂移, 当前代次);
            if (前态->状态 != L2任务子目标承接记录状态::等待子需求
                || !前态->子需求 || *前态->子需求 != 请求.子需求 || 前态->回流)
                return 失败(L2结构状态::引用冲突, 当前代次);
            const auto 旧状态 = L2任务结构内部::查找记录当前属性值(
                L1_, 子目标记录定位_, 请求.记录,
                子目标记录定位_.状态属性类型,
                请求.请求头.期望事实代次, 当前代次);
            if (!旧状态) return 失败(L2结构状态::内部不一致, 当前代次);
            const auto 写集 =
                L2任务结构内部::形成登记任务子目标承接记录回流写集(
                    请求, 子目标记录定位_, *旧状态);
            const auto 提交 = L2任务结构内部::提交任务子目标承接记录写集(
                第一层写入端口_, 写集, 当前代次);
            if (提交.状态 != L2结构状态::已提交
                && 提交.状态 != L2结构状态::精确重复)
                return 失败(提交.状态, 提交.事实代次);
            const auto 观察 = L2任务结构内部::读取中性当前事实代次(L1_);
            const auto 投影 = L2任务结构内部::读取任务子目标承接记录历史投影(
                L1_, 子目标记录定位_, 请求.记录, 提交.事实代次, 观察);
            if (!投影
                || 投影->状态
                    != L2任务子目标承接记录状态::子需求已回流待父需求重判
                || !投影->回流 || *投影->回流 != 请求.回流)
                return 失败(L2结构状态::内部不一致, 提交.事实代次);
            return {{L2结构合同版本, 提交.状态, 提交.事实代次,
                提交.事实代次}, *投影};
        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2结构状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致);
        }
    }

    L2任务子目标承接记录写入结果 写入已验证退出任务子目标承接记录(
        L2退出任务子目标承接记录请求 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [](L2结构状态 状态, std::uint64_t 截止 = 0) {
            L2任务子目标承接记录写入结果 结果;
            结果.结果头 = {L2结构合同版本, 状态, 截止, std::nullopt};
            return 结果;
        };
        try {
            if (!L2退出任务子目标承接记录请求有效(请求))
                return 失败(L2结构状态::入口拒绝);
            const auto 当前代次 = L2任务结构内部::读取中性当前事实代次(L1_);
            const auto 前态 = L2任务结构内部::读取任务子目标承接记录历史投影(
                L1_, 子目标记录定位_, 请求.记录,
                请求.请求头.期望事实代次, 当前代次);
            if (!前态) return 失败(当前代次 == 请求.请求头.期望事实代次
                ? L2结构状态::未找到 : L2结构状态::事实代次漂移, 当前代次);
            const auto 写集 = L2任务结构内部::形成退出任务子目标承接记录写集(
                L1_, 请求, 子目标记录定位_, 当前代次);
            if (!写集) return 失败(L2结构状态::内部不一致, 当前代次);
            const auto 提交 = L2任务结构内部::提交任务子目标承接记录写集(
                第一层写入端口_, *写集, 当前代次);
            if (提交.状态 != L2结构状态::已提交
                && 提交.状态 != L2结构状态::精确重复)
                return 失败(提交.状态, 提交.事实代次);
            auto 最后投影 = *前态;
            最后投影.生命周期.退出事实代次 = 提交.事实代次;
            return {{L2结构合同版本, 提交.状态, 提交.事实代次,
                提交.事实代次}, 最后投影};
        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2结构状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致);
        }
    }

public:
    // 结构服务只读桥：不暴露写端口、owner、锁或内部投影材料。
    L2任务子目标承接记录读取结果 读取任务子目标承接记录已验证投影(
        L2任务子目标承接记录读取请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        L2任务子目标承接记录读取结果 结果;
        const auto 观察 = L2任务结构内部::读取中性当前事实代次(L1_);
        结果.结果头 = {L2结构合同版本, L2结构状态::入口拒绝,
            观察, std::nullopt};
        结果.读取类别 = 请求.读取类别;
        结果.历史截止事实代次 = 请求.历史截止事实代次;
        if (!L2任务子目标承接记录读取请求有效(请求)) return 结果;
        if (请求.请求头.期望事实代次 != 观察) {
            结果.结果头.状态 = L2结构状态::事实代次漂移;
            return 结果;
        }
        try {
            const auto 截止 = 请求.读取类别 == L2读取类别::当前
                ? 观察 : 请求.历史截止事实代次;
            结果.记录 = L2任务结构内部::读取任务子目标承接记录历史投影(
                L1_, 子目标记录定位_, 请求.记录, 截止, 观察);
            结果.结果头.状态 = 结果.记录 ? L2结构状态::已读取
                : L2结构状态::未找到;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.结果头.状态 = L2结构状态::资源失败;
            结果.记录.reset();
            return 结果;
        } catch (...) {
            结果.结果头.状态 = L2结构状态::内部不一致;
            结果.记录.reset();
            return 结果;
        }
    }

    L2按任务轮次读取子目标承接记录组结果
    按任务轮次读取子目标承接记录组已验证投影(
        L2按任务轮次读取子目标承接记录组请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        L2按任务轮次读取子目标承接记录组结果 结果;
        const auto 观察 = L2任务结构内部::读取中性当前事实代次(L1_);
        结果.结果头 = {L2结构合同版本, L2结构状态::入口拒绝,
            观察, std::nullopt};
        结果.读取类别 = 请求.读取类别;
        结果.历史截止事实代次 = 请求.历史截止事实代次;
        if (!L2按任务轮次读取子目标承接记录组请求有效(请求)) return 结果;
        if (请求.请求头.期望事实代次 != 观察) {
            结果.结果头.状态 = L2结构状态::事实代次漂移;
            return 结果;
        }
        try {
            const auto 截止 = 请求.读取类别 == L2读取类别::当前
                ? 观察 : 请求.历史截止事实代次;
            const auto 组 = L1_.读取所有者范围历史关系组(
                {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源,
                    请求.任务.值, 子目标记录定位_.任务组关系类型, 截止});
            if (组.状态 != L1所有者范围读取状态::成功
                || 组.读取事实代次 != 观察) {
                结果.结果头.状态 = L2任务结构内部::映射子目标记录读取状态(组.状态);
                return 结果;
            }
            结果.实际扫描记录数 = 组.关系组.size();
            if (组.关系组.size() > 请求.最大扫描记录数) {
                结果.结果头.状态 = L2结构状态::数量预算不足;
                结果.实际扫描记录数 = 0;
                return 结果;
            }
            for (const auto& 关系 : 组.关系组) {
                const auto 投影 =
                    L2任务结构内部::读取任务子目标承接记录历史投影(
                        L1_, 子目标记录定位_,
                        L2任务子目标承接记录身份{关系.目标节点}, 截止, 观察);
                if (!投影) {
                    结果.结果头.状态 = L2结构状态::内部不一致;
                    结果.记录组.clear();
                    return 结果;
                }
                if (投影->筹办轮次 == 请求.筹办轮次)
                    结果.记录组.push_back(*投影);
            }
            if (结果.记录组.size() > 请求.最大返回记录数) {
                结果.结果头.状态 = L2结构状态::数量预算不足;
                结果.实际扫描记录数 = 0;
                结果.记录组.clear();
                return 结果;
            }
            std::sort(结果.记录组.begin(), 结果.记录组.end(),
                [](const auto& 左, const auto& 右) {
                    return 左.身份.值.值 < 右.身份.值.值;
                });
            结果.结果头.状态 = L2结构状态::已读取;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.结果头.状态 = L2结构状态::资源失败;
            结果.实际扫描记录数 = 0;
            结果.记录组.clear();
            return 结果;
        } catch (...) {
            结果.结果头.状态 = L2结构状态::内部不一致;
            结果.实际扫描记录数 = 0;
            结果.记录组.clear();
            return 结果;
        }
    }

private:
    // ===== 已验证任务方法路径原子承载 =====
    // 一次 owner-aware 原子写集：1 节点 + 2 关系 + 10 值同代发布。
    // 路径节点、路径族归属（路径→路径锚点）、路径任务引用（路径→任务）、
    // 筹办轮次、来源方法、来源方法内容版本、条件绑定、输入绑定、
    // 参数绑定、动作入口、预期结果、验证合同、归因合同。
    L2新增任务方法路径结果 写入已验证任务方法路径(
        L2新增任务方法路径请求 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [&](L2结构状态 状态,
            std::uint64_t 截止 = 0) noexcept {
            L2新增任务方法路径结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = 状态;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        };
        try {
            if (!L2新增任务方法路径请求有效(请求))
                return 失败(L2结构状态::入口拒绝);
            const auto 代次 = L2任务结构内部::检查任务写入当前代次(
                L1_, 请求.请求头.期望事实代次);
            if (!代次.成功() && !代次.直接提交)
                return 失败(代次.状态, 代次.事实截止代次);

            // 验证任务身份：读取任务当前投影确认任务存在且当前
            L2任务读取请求 任务读取请求;
            任务读取请求.请求头.合同版本 = L2结构合同版本;
            任务读取请求.任务 = 请求.任务;
            任务读取请求.读取类别 = L2读取类别::当前;
            任务读取请求.历史截止事实代次 = 0;
            const auto 任务验证 = 读取任务当前投影(
                L1_, 身份来源定位_, 类型定位_,
                任务读取请求, 请求.请求头.期望事实代次);
            if (任务验证.结果头.状态 != L2结构状态::已读取)
                return 失败(L2结构状态::引用冲突,
                    任务验证.结果头.事实截止代次);

            const auto 本地键 = [](std::uint32_t 值) noexcept {
                return L1所有者范围写集本地键{值};
            };
            const auto 键路径节点 = 本地键(
                L2任务结构内部::路径节点写入本地键值);

            L1所有者范围写集请求 写集;
            写集.合同版本 = L1所有者范围CRUD合同版本;
            写集.期望事实代次 = 请求.请求头.期望事实代次;
            写集.写入幂等身份 = L2任务结构内部::映射路径幂等键(
                请求.幂等身份);

            // 1 个节点
            写集.节点 = {
                {键路径节点, 节点种类::普通, std::nullopt}};

            // 2 条关系
            写集.关系 = {
                // 路径族归属：路径 -> 路径锚点，类型=路径族归属关系类型，角色=1
                {本地键(L2任务结构内部::路径族归属关系写入本地键值),
                    键路径节点, 身份来源定位_.路径族归属关系,
                    类型定位_.路径族归属关系类型, 1},
                // 路径任务引用：路径 -> 任务，类型=路径任务引用关系类型，角色=1
                {本地键(L2任务结构内部::路径任务引用关系写入本地键值),
                    键路径节点, 请求.任务.值,
                    类型定位_.路径任务引用关系类型, 1}};

            // 10 个值：筹办轮次、来源方法、来源方法内容版本、
            // 条件绑定、输入绑定、参数绑定、动作入口、预期结果、
            // 验证合同、归因合同
            写集.值 = {
                // 筹办轮次（I64）
                {本地键(L2任务结构内部::路径筹办轮次值写入本地键值),
                    键路径节点, 身份来源定位_.路径筹办轮次属性类型,
                    static_cast<std::int64_t>(请求.筹办轮次),
                    键路径节点},
                // 来源方法（I64，稳定编码.值）
                {本地键(L2任务结构内部::路径来源方法值写入本地键值),
                    键路径节点, 身份来源定位_.路径来源方法属性类型,
                    static_cast<std::int64_t>(请求.来源方法.值.值),
                    键路径节点},
                // 来源方法内容版本（I64，稳定编码.值）
                {本地键(L2任务结构内部::路径来源方法内容版本值写入本地键值),
                    键路径节点, 身份来源定位_.路径来源方法内容版本属性类型,
                    static_cast<std::int64_t>(请求.来源方法内容版本.值),
                    键路径节点},
                // 条件绑定（独立材料引用）
                {本地键(L2任务结构内部::路径条件绑定值写入本地键值),
                    键路径节点, 身份来源定位_.路径条件绑定属性类型,
                    L1所有者范围独立材料引用{
                        请求.完整路径材料.条件绑定},
                    键路径节点},
                // 输入绑定（独立材料引用）
                {本地键(L2任务结构内部::路径输入绑定值写入本地键值),
                    键路径节点, 身份来源定位_.路径输入绑定属性类型,
                    L1所有者范围独立材料引用{
                        请求.完整路径材料.输入绑定},
                    键路径节点},
                // 参数绑定（独立材料引用）
                {本地键(L2任务结构内部::路径参数绑定值写入本地键值),
                    键路径节点, 身份来源定位_.路径参数绑定属性类型,
                    L1所有者范围独立材料引用{
                        请求.完整路径材料.参数绑定},
                    键路径节点},
                // 动作入口（独立材料引用）
                {本地键(L2任务结构内部::路径动作入口值写入本地键值),
                    键路径节点, 身份来源定位_.路径动作入口属性类型,
                    L1所有者范围独立材料引用{
                        请求.完整路径材料.动作入口},
                    键路径节点},
                // 预期结果（独立材料引用）
                {本地键(L2任务结构内部::路径预期结果值写入本地键值),
                    键路径节点, 身份来源定位_.路径预期结果属性类型,
                    L1所有者范围独立材料引用{
                        请求.完整路径材料.预期结果},
                    键路径节点},
                // 验证合同（独立材料引用）
                {本地键(L2任务结构内部::路径验证合同值写入本地键值),
                    键路径节点, 身份来源定位_.路径验证合同属性类型,
                    L1所有者范围独立材料引用{
                        请求.完整路径材料.验证合同},
                    键路径节点},
                // 归因合同（独立材料引用）
                {本地键(L2任务结构内部::路径归因合同值写入本地键值),
                    键路径节点, 身份来源定位_.路径归因合同属性类型,
                    L1所有者范围独立材料引用{
                        请求.完整路径材料.归因合同},
                    键路径节点}};

            // 10 个属性槽变更：将路径节点的 10 个属性槽指向新值
            写集.属性槽变更 = {
                {键路径节点, 身份来源定位_.路径筹办轮次属性类型,
                    {L2任务结构内部::路径筹办轮次值写入本地键值}},
                {键路径节点, 身份来源定位_.路径来源方法属性类型,
                    {L2任务结构内部::路径来源方法值写入本地键值}},
                {键路径节点, 身份来源定位_.路径来源方法内容版本属性类型,
                    {L2任务结构内部::路径来源方法内容版本值写入本地键值}},
                {键路径节点, 身份来源定位_.路径条件绑定属性类型,
                    {L2任务结构内部::路径条件绑定值写入本地键值}},
                {键路径节点, 身份来源定位_.路径输入绑定属性类型,
                    {L2任务结构内部::路径输入绑定值写入本地键值}},
                {键路径节点, 身份来源定位_.路径参数绑定属性类型,
                    {L2任务结构内部::路径参数绑定值写入本地键值}},
                {键路径节点, 身份来源定位_.路径动作入口属性类型,
                    {L2任务结构内部::路径动作入口值写入本地键值}},
                {键路径节点, 身份来源定位_.路径预期结果属性类型,
                    {L2任务结构内部::路径预期结果值写入本地键值}},
                {键路径节点, 身份来源定位_.路径验证合同属性类型,
                    {L2任务结构内部::路径验证合同值写入本地键值}},
                {键路径节点, 身份来源定位_.路径归因合同属性类型,
                    {L2任务结构内部::路径归因合同值写入本地键值}}};

            const auto 提交 = L2任务结构内部::提交并验证路径写集(
                第一层写入端口_, 写集);
            if (提交.状态 != L2结构状态::已提交
                && 提交.状态 != L2结构状态::精确重复)
                return 失败(提交.状态, 提交.事实截止代次);

            return L2任务结构内部::读回新增任务方法路径(
                L1_, 身份来源定位_, 类型定位_, 请求,
                提交.编码映射, 提交.事实截止代次);
        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2结构状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致);
        }
    }

public:
    // ===== 读取任务方法路径 =====
    L2任务方法路径读取结果 读取任务方法路径(
        L2任务方法路径读取请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 截止 = L2任务结构内部::读取中性当前事实代次(L1_);
        if (!L2任务方法路径读取请求有效(请求) || 截止 == 0) {
            L2任务方法路径读取结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = L2结构状态::入口拒绝;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        }
        if (请求.读取类别 != L2读取类别::当前) {
            L2任务方法路径读取结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = L2结构状态::入口拒绝;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        }
        try {
            return L2任务结构内部::读取任务方法路径当前投影(
                L1_, 身份来源定位_, 类型定位_, 请求, 截止,
                &正式选择定位_);
        } catch (const std::bad_alloc&) {
            L2任务方法路径读取结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = L2结构状态::资源失败;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        } catch (...) {
            L2任务方法路径读取结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = L2结构状态::内部不一致;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        }
    }

    // ===== 按任务读取当前方法路径组 =====
    L2按任务读取当前方法路径组结果 按任务读取当前方法路径组(
        L2按任务读取当前方法路径组请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 截止 = L2任务结构内部::读取中性当前事实代次(L1_);
        L2按任务读取当前方法路径组结果 结果;
        结果.结果头.合同版本 = L2结构合同版本;
        结果.结果头.状态 =
            (L2按任务读取当前方法路径组请求有效(请求) && 截止 != 0)
                ? L2结构状态::已读取
                : L2结构状态::入口拒绝;
        结果.结果头.事实截止代次 = 截止;
        结果.读取类别 = 请求.读取类别;
        结果.历史截止事实代次 = 请求.历史截止事实代次;
        return 结果;
    }

    // ===== 按任务与筹办轮次读取路径组 =====
    L2按任务与筹办轮次读取路径组结果 按任务与筹办轮次读取路径组(
        L2按任务与筹办轮次读取路径组请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 截止 = L2任务结构内部::读取中性当前事实代次(L1_);
        L2按任务与筹办轮次读取路径组结果 结果;
        结果.结果头.合同版本 = L2结构合同版本;
        结果.结果头.状态 =
            (L2按任务与筹办轮次读取路径组请求有效(请求) && 截止 != 0)
                ? L2结构状态::已读取
                : L2结构状态::入口拒绝;
        结果.结果头.事实截止代次 = 截止;
        结果.读取类别 = 请求.读取类别;
        结果.历史截止事实代次 = 请求.历史截止事实代次;
        结果.筹办轮次 = 请求.筹办轮次;
        return 结果;
    }

    // ===== 退出任务方法路径 =====
    L2任务方法路径退出结果 退出任务方法路径(
        L2任务方法路径退出请求 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 代次前 = L2任务结构内部::读取中性当前事实代次(L1_);
        L2任务方法路径退出结果 结果;
        结果.结果头.合同版本 = L2结构合同版本;
        结果.结果头.状态 = L2结构状态::入口拒绝;
        结果.结果头.事实截止代次 = 代次前;
        return 结果;
    }

    // ===== 原子发布任务筹办正式选择、路径、冻结材料与实例 =====
    L2发布任务筹办正式选择结果 发布任务筹办正式选择(
        L2发布任务筹办正式选择请求 请求) noexcept {
        (void)请求;
        L2发布任务筹办正式选择结果 兼容拒绝;
        兼容拒绝.结果头 = {L2结构合同版本,
            L2结构状态::入口拒绝, 0, std::nullopt};
        return 兼容拒绝;
    }

    L2发布任务筹办正式选择结果_v2 发布任务筹办正式选择_v2(
        const L2发布任务筹办正式选择请求_v2& 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [](L2任务筹办正式选择状态_v2 状态) noexcept {
            L2发布任务筹办正式选择结果_v2 结果; 结果.状态 = 状态; return 结果;
        };
        const auto 映射写入 = [](L1所有者范围写入状态 状态) noexcept {
            switch (状态) {
            case L1所有者范围写入状态::成功:
                return L2任务筹办正式选择状态_v2::已形成;
            case L1所有者范围写入状态::精确重复:
                return L2任务筹办正式选择状态_v2::精确重复;
            case L1所有者范围写入状态::幂等冲突:
                return L2任务筹办正式选择状态_v2::幂等冲突;
            case L1所有者范围写入状态::入口拒绝:
                return L2任务筹办正式选择状态_v2::入口拒绝;
            case L1所有者范围写入状态::许可拒绝:
                return L2任务筹办正式选择状态_v2::许可拒绝;
            case L1所有者范围写入状态::事实代次漂移:
                return L2任务筹办正式选择状态_v2::当前性漂移;
            case L1所有者范围写入状态::引用冲突:
                return L2任务筹办正式选择状态_v2::引用冲突;
            case L1所有者范围写入状态::资源失败:
                return L2任务筹办正式选择状态_v2::资源失败;
            default:
                return L2任务筹办正式选择状态_v2::内部错误;
            }
        };
        try {
            if (!L2发布任务筹办正式选择请求_v2有效(请求))
                return 失败(L2任务筹办正式选择状态_v2::入口拒绝);
            if (请求.基础材料.来源需求组.size() > 1024
                || 请求.基础材料.稳定证据组.size() > 1024)
                return 失败(L2任务筹办正式选择状态_v2::资源失败);
            const auto 来源 = L2取得任务筹办来源幂等身份(
                请求.基础材料.来源材料);
            const auto 首次账 = L2任务结构内部::定位任务筹办来源首次账_v2(
                第一层写入端口_, 来源);
            if (首次账.状态 != L2任务筹办正式选择读取状态_v2::未找到
                && 首次账.状态 != L2任务筹办正式选择读取状态_v2::已读取)
                return 失败(L2任务筹办正式选择状态_v2::内部错误);
            const auto 当前代次 = L2任务结构内部::读取中性当前事实代次(L1_);
            if (当前代次 == 0)
                return 失败(L2任务筹办正式选择状态_v2::内部错误);
            if (首次账.状态 == L2任务筹办正式选择读取状态_v2::已读取) {
                if (!首次账.首次请求 || !首次账.首次规范化写集
                    || !首次账.首次写入结果)
                    return 失败(L2任务筹办正式选择状态_v2::内部错误);
                if (*首次账.首次请求 != 请求)
                    return 失败(L2任务筹办正式选择状态_v2::幂等冲突);
                const auto 重放 = 第一层写入端口_.提交所有者范围中性写集(
                    *首次账.首次规范化写集);
                if (重放.状态 != L1所有者范围写入状态::精确重复
                    || 重放.事实代次 != 首次账.首次写入结果->事实代次)
                    return 失败(映射写入(重放.状态));
                const auto 选择编码 = L2任务结构内部::查找治理写入编码(
                    *首次账.首次写入结果,
                    L2任务结构内部::正式选择记录节点写入本地键值);
                if (!选择编码)
                    return 失败(L2任务筹办正式选择状态_v2::内部错误);
                const auto G1 = 首次账.首次写入结果->事实代次;
                const auto 读回 = 读取正式选择投影_v2(
                    L2任务方法选择记录身份{*选择编码}, G1,
                    L2读取类别::历史, G1);
                if (!读回.成功()) {
                    auto 结果 = 失败(L2任务筹办正式选择状态_v2::已可能发布);
                    结果.提交见证 = L2任务筹办正式选择提交见证_v2{
                        第一层写入端口_.所有者身份(), 来源, G1, 1};
                    return 结果;
                }
                L2发布任务筹办正式选择结果_v2 结果;
                结果.状态 = L2任务筹办正式选择状态_v2::精确重复;
                结果.正式选择 = 读回.正式选择;
                结果.冻结材料 = 读回.冻结材料;
                结果.实例方法 = 读回.实例方法;
                结果.提交见证 = L2任务筹办正式选择提交见证_v2{
                    第一层写入端口_.所有者身份(), 来源, G1, 1};
                结果.本次正式读回截止 = G1;
                return 结果;
            }
            // 首次账不存在时，current 生产只允许初次筹办谱系。
            // legacy 重筹办来源仅保留上方既有账精确重放资格。
            const auto* 初次 = std::get_if<L2任务初次筹办规范化来源材料>(
                &请求.基础材料.来源材料);
            const auto* 初次归因 =
                std::get_if<L2任务初次筹办准备记录身份>(
                    &请求.冻结材料.归因证据);
            if (!初次 || 请求.基础材料.稳定证据组.size() != 1
                || 请求.基础材料.稳定证据组.front().类别
                    != L2任务选择稳定证据类别::初次筹办准备记录
                || 请求.基础材料.稳定证据组.front().身份
                    != 初次->首次准备记录.值
                || !初次归因 || *初次归因 != 初次->首次准备记录)
                return 失败(L2任务筹办正式选择状态_v2::入口拒绝);
            if (当前代次 != 请求.请求头.期望事实代次)
                return 失败(L2任务筹办正式选择状态_v2::当前性漂移);
            const auto G0 = 当前代次;
            const auto 来源任务 = L2取得任务筹办任务(请求.基础材料.来源材料);
            const auto 新筹办轮次 = L2取得任务筹办轮次(请求.基础材料.来源材料);
            const auto 身份来源 = L2任务结构内部::读取任务身份来源当前投影(
                L1_, 身份来源定位_, {{L2结构合同版本, G0},
                    L2读取类别::当前, 来源任务, 0}, G0);
            const auto 任务 = L2任务结构内部::读取任务轮次核心当前投影(
                L1_, 身份来源定位_, 类型定位_,
                {{L2结构合同版本, G0}, L2读取类别::当前, 来源任务, 0}, G0);
            if (!身份来源.成功() || !身份来源.身份来源
                || *身份来源.身份来源 != 请求.基础材料.任务身份来源
                || !任务.成功() || !任务.任务)
                return 失败(L2任务筹办正式选择状态_v2::引用冲突);
            L1所有者范围一致当前读取请求 当前请求;
            当前请求.合同版本 = L1所有者范围一致当前读取合同版本;
            当前请求.期望事实代次 = G0;
            当前请求.所有者 = {身份来源定位_.所有者};
            当前请求.源关系组 = {
                {来源任务.值, 类型定位_.当前选中路径关系类型},
                {来源任务.值, 类型定位_.实例方法当前关系类型},
                {来源任务.值, 正式选择定位_.任务当前正式选择关系类型}};
            const auto 当前 = L1_.尝试读取所有者范围一致当前投影(当前请求);
            if (当前.状态 != L1所有者范围一致当前读取状态::成功
                || 当前.读取事实代次 != G0 || 当前.源关系组.size() != 3)
                return 失败(L2任务筹办正式选择状态_v2::内部错误);
            const L1所有者范围一致源关系组读取结果项* 路径组 = nullptr;
            const L1所有者范围一致源关系组读取结果项* 实例组 = nullptr;
            const L1所有者范围一致源关系组读取结果项* 选择组 = nullptr;
            for (const auto& 组 : 当前.源关系组) {
                if (组.关系类型节点 == 类型定位_.当前选中路径关系类型) 路径组 = &组;
                else if (组.关系类型节点 == 类型定位_.实例方法当前关系类型) 实例组 = &组;
                else if (组.关系类型节点 == 正式选择定位_.任务当前正式选择关系类型) 选择组 = &组;
            }
            if (!路径组 || !实例组 || !选择组
                || 路径组->成员.size() > 1 || 实例组->成员.size() > 1
                || 选择组->成员.size() > 1)
                return 失败(L2任务筹办正式选择状态_v2::引用冲突);
            std::optional<L2任务方法路径身份> 旧路径;
            std::optional<L2实例方法身份> 旧实例;
            std::optional<L2任务方法选择记录身份> 旧选择;
            std::uint64_t 新执行轮次 = 1;
            if (!路径组->成员.empty() || !实例组->成员.empty()
                || !选择组->成员.empty())
                return 失败(L2任务筹办正式选择状态_v2::引用冲突);
            const auto 键 = [](std::uint32_t 值) noexcept {
                return L1所有者范围写集本地键{值};
            };
            const auto 选择键 = 键(L2任务结构内部::正式选择记录节点写入本地键值);
            const auto 冻结键 = 键(L2任务结构内部::执行绑定冻结材料节点写入本地键值);
            const auto 路径键 = 键(L2任务结构内部::路径节点写入本地键值);
            const auto 实例键 = 键(L2任务结构内部::实例方法节点写入本地键值);
            L1所有者范围写集请求 写集;
            写集.合同版本 = L1所有者范围CRUD合同版本;
            写集.期望事实代次 = G0; 写集.写入幂等身份 = 首次账.写入幂等身份;
            写集.节点 = {{选择键, 节点种类::普通, std::nullopt},
                {冻结键, 节点种类::普通, std::nullopt},
                {路径键, 节点种类::普通, std::nullopt},
                {实例键, 节点种类::普通, std::nullopt}};
            写集.关系 = {
                {键(L2任务结构内部::选择记录族归属关系写入本地键值), 选择键,
                    正式选择定位_.选择记录锚点, 正式选择定位_.选择记录族归属关系类型, 1},
                {键(L2任务结构内部::冻结材料族归属关系写入本地键值), 冻结键,
                    正式选择定位_.冻结材料锚点, 正式选择定位_.冻结材料族归属关系类型, 1},
                {键(L2任务结构内部::任务当前正式选择关系写入本地键值), 来源任务.值,
                    选择键, 正式选择定位_.任务当前正式选择关系类型, 1},
                {键(L2任务结构内部::选择记录方法引用关系写入本地键值), 选择键,
                    请求.冻结材料.方法.值, 正式选择定位_.选择记录方法引用关系类型, 1},
                {键(L2任务结构内部::选择记录路径引用关系写入本地键值), 选择键,
                    路径键, 正式选择定位_.选择记录路径引用关系类型, 1},
                {键(L2任务结构内部::选择记录冻结材料关系写入本地键值), 选择键,
                    冻结键, 正式选择定位_.选择记录冻结材料关系类型, 1},
                {键(L2任务结构内部::路径族归属关系写入本地键值), 路径键,
                    身份来源定位_.路径族归属关系, 类型定位_.路径族归属关系类型, 1},
                {键(L2任务结构内部::路径任务引用关系写入本地键值), 路径键,
                    来源任务.值, 类型定位_.路径任务引用关系类型, 1},
                {键(L2任务结构内部::当前选中路径关系写入本地键值), 来源任务.值,
                    路径键, 类型定位_.当前选中路径关系类型, 1},
                {键(L2任务结构内部::实例方法族归属关系写入本地键值), 实例键,
                    身份来源定位_.实例方法族归属关系, 类型定位_.实例方法族归属关系类型, 1},
                {键(L2任务结构内部::实例方法任务引用关系写入本地键值), 实例键,
                    来源任务.值, 类型定位_.实例方法任务引用关系类型, 1},
                {键(L2任务结构内部::实例方法路径引用关系写入本地键值), 实例键,
                    路径键, 类型定位_.实例方法路径引用关系类型, 1},
                {键(L2任务结构内部::实例方法当前关系写入本地键值), 来源任务.值,
                    实例键, 类型定位_.实例方法当前关系类型, 1}};
            for (std::size_t i = 0; i < 请求.基础材料.来源需求组.size(); ++i)
                写集.关系.push_back({键(L2任务结构内部::选择记录来源需求关系写入基值
                    + static_cast<std::uint32_t>(i)), 选择键,
                    请求.基础材料.来源需求组[i].值,
                    正式选择定位_.选择记录来源需求关系类型,
                    static_cast<std::int64_t>(i + 1)});
            for (std::size_t i = 0; i < 请求.基础材料.稳定证据组.size(); ++i)
                写集.关系.push_back({键(L2任务结构内部::选择记录稳定证据关系写入基值
                    + static_cast<std::uint32_t>(i)), 选择键,
                    请求.基础材料.稳定证据组[i].身份,
                    正式选择定位_.选择记录稳定证据关系类型,
                    static_cast<std::int64_t>(请求.基础材料.稳定证据组[i].类别)});
            const auto 动作组序列 = [&] {
                L2任务结构内部::正式选择序列写入器 写;
                写.写(L2任务路径动作组序列格式版本_v2);
                写.写(请求.冻结材料.完整有序动作组.size());
                for (const auto& 动作 : 请求.冻结材料.完整有序动作组)
                    写.写动作冻结项_v2(动作);
                return std::move(写.值);
            }();
            写集.值 = {
                {键(L2任务结构内部::正式选择完整请求值写入本地键值), 选择键,
                    正式选择定位_.完整请求属性类型,
                    L2任务结构内部::编码正式选择完整请求_v2(请求), 选择键},
                {键(L2任务结构内部::路径筹办轮次值写入本地键值), 路径键,
                    身份来源定位_.路径筹办轮次属性类型,
                    static_cast<std::int64_t>(新筹办轮次), 路径键},
                {键(L2任务结构内部::路径来源方法值写入本地键值), 路径键,
                    身份来源定位_.路径来源方法属性类型,
                    static_cast<std::int64_t>(请求.冻结材料.方法.值.值), 路径键},
                {键(L2任务结构内部::路径来源方法内容版本值写入本地键值), 路径键,
                    身份来源定位_.路径来源方法内容版本属性类型,
                    static_cast<std::int64_t>(请求.冻结材料.方法内容版本.值), 路径键},
                {键(L2任务结构内部::路径动作入口值写入本地键值), 路径键,
                    身份来源定位_.路径完整有序动作组属性类型_v2,
                    动作组序列, 路径键},
                {键(L2任务结构内部::路径预期结果值写入本地键值), 路径键,
                    身份来源定位_.路径预期结果属性类型,
                    L1所有者范围独立材料引用{请求.冻结材料.主轴结果.身份.值}, 路径键},
                {键(L2任务结构内部::路径验证合同值写入本地键值), 路径键,
                    身份来源定位_.路径验证合同属性类型,
                    L1所有者范围独立材料引用{请求.冻结材料.验证合同.值}, 路径键},
                {键(L2任务结构内部::实例方法执行轮次属性类型本地键值), 实例键,
                    身份来源定位_.实例方法执行轮次属性类型,
                    static_cast<std::int64_t>(新执行轮次), 实例键}};
            写集.属性槽变更 = {
                {选择键, 正式选择定位_.完整请求属性类型,
                    {L2任务结构内部::正式选择完整请求值写入本地键值}},
                {路径键, 身份来源定位_.路径筹办轮次属性类型,
                    {L2任务结构内部::路径筹办轮次值写入本地键值}},
                {路径键, 身份来源定位_.路径来源方法属性类型,
                    {L2任务结构内部::路径来源方法值写入本地键值}},
                {路径键, 身份来源定位_.路径来源方法内容版本属性类型,
                    {L2任务结构内部::路径来源方法内容版本值写入本地键值}},
                {路径键, 身份来源定位_.路径完整有序动作组属性类型_v2,
                    {L2任务结构内部::路径动作入口值写入本地键值}},
                {路径键, 身份来源定位_.路径预期结果属性类型,
                    {L2任务结构内部::路径预期结果值写入本地键值}},
                {路径键, 身份来源定位_.路径验证合同属性类型,
                    {L2任务结构内部::路径验证合同值写入本地键值}},
                {实例键, 身份来源定位_.实例方法执行轮次属性类型,
                    {L2任务结构内部::实例方法执行轮次属性类型本地键值}}};
            const auto 写入 = 第一层写入端口_.提交所有者范围中性写集(写集);
            const auto 状态 = 映射写入(写入.状态);
            if (状态 != L2任务筹办正式选择状态_v2::已形成
                && 状态 != L2任务筹办正式选择状态_v2::精确重复)
                return 失败(状态);
            const auto 选择编码 = L2任务结构内部::查找治理写入编码(
                写入, L2任务结构内部::正式选择记录节点写入本地键值);
            const auto 提交见证 = L2任务筹办正式选择提交见证_v2{
                第一层写入端口_.所有者身份(), 来源, 写入.事实代次, 1};
            if (!选择编码) {
                auto 结果 = 失败(L2任务筹办正式选择状态_v2::已可能发布);
                结果.提交见证 = 提交见证; return 结果;
            }
            const auto 读回 = 读取正式选择投影_v2(
                L2任务方法选择记录身份{*选择编码}, 写入.事实代次,
                L2读取类别::当前, 0);
            if (!读回.成功()) {
                auto 结果 = 失败(L2任务筹办正式选择状态_v2::已可能发布);
                结果.提交见证 = 提交见证; return 结果;
            }
            L2发布任务筹办正式选择结果_v2 结果;
            结果.状态 = 状态; 结果.正式选择 = 读回.正式选择;
            结果.冻结材料 = 读回.冻结材料; 结果.实例方法 = 读回.实例方法;
            结果.提交见证 = 提交见证; 结果.旧当前选择 = 旧选择;
            结果.旧当前路径 = 旧路径; 结果.旧当前实例 = 旧实例;
            结果.本次正式读回截止 = 写入.事实代次;
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(L2任务筹办正式选择状态_v2::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2任务筹办正式选择状态_v2::资源失败);
        } catch (...) {
            return 失败(L2任务筹办正式选择状态_v2::内部错误);
        }
    }

    L2任务筹办正式选择读取结果_v2 读取任务方法选择_v2(
        const L2任务方法选择读取请求_v2& 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        if (!L2任务方法选择读取请求_v2有效(请求))
            return {};
        const auto 当前 = L2任务结构内部::读取中性当前事实代次(L1_);
        if (当前 == 0 || 当前 != 请求.请求头.期望事实代次) {
            L2任务筹办正式选择读取结果_v2 结果;
            结果.状态 = L2任务筹办正式选择读取状态_v2::当前性漂移;
            return 结果;
        }
        const auto 截止 = 请求.读取类别 == L2读取类别::当前
            ? 当前 : 请求.历史截止事实代次;
        return 读取正式选择投影_v2(
            请求.选择记录, 截止, 请求.读取类别, 请求.历史截止事实代次);
    }

    L2任务筹办正式选择读取结果_v2 按任务读取当前方法选择_v2(
        const L2按任务读取当前方法选择请求_v2& 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        L2任务筹办正式选择读取结果_v2 结果;
        if (!L2按任务读取当前方法选择请求_v2有效(请求)) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::入口拒绝;
            return 结果;
        }
        try {
            const auto G0 = L2任务结构内部::读取中性当前事实代次(L1_);
            if (G0 == 0 || G0 != 请求.请求头.期望事实代次) {
                结果.状态 = L2任务筹办正式选择读取状态_v2::当前性漂移;
                return 结果;
            }
            L1所有者范围一致当前读取请求 查询;
            查询.合同版本 = L1所有者范围一致当前读取合同版本;
            查询.期望事实代次 = G0; 查询.所有者 = {身份来源定位_.所有者};
            查询.源关系组 = {{请求.任务.值,
                正式选择定位_.任务当前正式选择关系类型}};
            const auto 读取 = L1_.尝试读取所有者范围一致当前投影(查询);
            if (读取.状态 != L1所有者范围一致当前读取状态::成功
                || 读取.读取事实代次 != G0 || 读取.源关系组.size() != 1) {
                结果.状态 = L2任务筹办正式选择读取状态_v2::内部错误;
                return 结果;
            }
            const auto& 成员 = 读取.源关系组.front().成员;
            if (成员.empty()) { 结果.状态 = L2任务筹办正式选择读取状态_v2::未找到; return 结果; }
            if (成员.size() != 1) { 结果.状态 = L2任务筹办正式选择读取状态_v2::关系多义; return 结果; }
            return 读取正式选择投影_v2(
                L2任务方法选择记录身份{成员.front().关系.目标节点},
                G0, L2读取类别::当前, 0);
        } catch (const std::bad_alloc&) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::资源失败;
            return 结果;
        } catch (...) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::内部错误;
            return 结果;
        }
    }

    L2按筹办来源幂等身份读取正式选择结果_v2
    按筹办来源幂等身份读取正式选择_v2(
        const L2按筹办来源幂等身份读取正式选择请求_v2& 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        L2按筹办来源幂等身份读取正式选择结果_v2 结果;
        if (!L2按筹办来源幂等身份读取正式选择请求_v2有效(请求)) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::入口拒绝;
            return 结果;
        }
        try {
            const auto G0 = L2任务结构内部::读取中性当前事实代次(L1_);
            if (G0 == 0 || G0 != 请求.请求头.期望事实代次) {
                结果.状态 = L2任务筹办正式选择读取状态_v2::当前性漂移;
                return 结果;
            }
            const auto 首次 = L2任务结构内部::定位任务筹办来源首次账_v2(
                第一层写入端口_, 请求.原筹办来源幂等身份);
            if (首次.状态 != L2任务筹办正式选择读取状态_v2::已读取
                || !首次.首次请求 || !首次.首次写入结果) {
                结果.状态 = 首次.状态; return 结果;
            }
            if (请求.原完整请求 && *请求.原完整请求 != *首次.首次请求) {
                结果.状态 = L2任务筹办正式选择读取状态_v2::引用冲突;
                return 结果;
            }
            const auto 选择编码 = L2任务结构内部::查找治理写入编码(
                *首次.首次写入结果,
                L2任务结构内部::正式选择记录节点写入本地键值);
            if (!选择编码) return 结果;
            const auto G1 = 首次.首次写入结果->事实代次;
            const auto 读回 = 读取正式选择投影_v2(
                L2任务方法选择记录身份{*选择编码}, G1,
                L2读取类别::历史, G1);
            结果.状态 = 读回.状态;
            if (!读回.成功()) return 结果;
            结果.首次完整请求 = *首次.首次请求;
            结果.正式选择 = 读回.正式选择; 结果.冻结材料 = 读回.冻结材料;
            结果.实例方法 = 读回.实例方法; 结果.本次正式读回截止 = G1;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::资源失败;
            return 结果;
        } catch (...) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::内部错误;
            return 结果;
        }
    }

    // ===== 读取任务方法选择 =====
    L2任务方法选择读取结果 读取任务方法选择(
        L2任务方法选择读取请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 当前代次 = L2任务结构内部::读取中性当前事实代次(L1_);
        L2任务方法选择读取结果 结果;
        结果.结果头 = {L2结构合同版本, L2结构状态::入口拒绝,
            当前代次, std::nullopt};
        结果.读取类别 = 请求.读取类别;
        结果.历史截止事实代次 = 请求.历史截止事实代次;
        if (!L2任务方法选择读取请求有效(请求) || 当前代次 == 0)
            return 结果;
        const auto 截止 = 请求.读取类别 == L2读取类别::当前
            ? 当前代次 : 请求.历史截止事实代次;
        if (请求.请求头.期望事实代次 != 当前代次
            || 截止 == 0 || 截止 > 当前代次)
            return 结果;
        try {
            结果 = 读取正式选择当前投影(请求.选择记录, 截止);
            结果.读取类别 = 请求.读取类别;
            结果.历史截止事实代次 = 请求.历史截止事实代次;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.结果头.状态 = L2结构状态::资源失败;
            return 结果;
        } catch (...) {
            结果.结果头.状态 = L2结构状态::内部不一致;
            return 结果;
        }
    }

    // ===== 按任务读取当前方法选择 =====
    L2按任务读取当前方法选择结果 按任务读取当前方法选择(
        L2按任务读取当前方法选择请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 当前代次 = L2任务结构内部::读取中性当前事实代次(L1_);
        L2按任务读取当前方法选择结果 结果;
        结果.结果头 = {L2结构合同版本, L2结构状态::入口拒绝,
            当前代次, std::nullopt};
        if (!L2按任务读取当前方法选择请求有效(请求)
            || 当前代次 == 0 || 请求.请求头.期望事实代次 != 当前代次)
            return 结果;
        try {
            L1所有者范围一致当前读取请求 读取请求;
            读取请求.合同版本 = L1所有者范围一致当前读取合同版本;
            读取请求.期望事实代次 = 当前代次;
            读取请求.所有者 = {身份来源定位_.所有者};
            读取请求.源关系组 = {{请求.任务.值,
                正式选择定位_.任务当前正式选择关系类型}};
            const auto 读取 = L1_.尝试读取所有者范围一致当前投影(读取请求);
            if (读取.状态 != L1所有者范围一致当前读取状态::成功
                || 读取.读取事实代次 != 当前代次
                || 读取.源关系组.size() != 1) {
                结果.结果头.状态 = L2结构状态::内部不一致;
                return 结果;
            }
            const auto& 成员 = 读取.源关系组.front().成员;
            if (成员.empty()) {
                结果.结果头.状态 = L2结构状态::已读取;
                return 结果;
            }
            if (成员.size() != 1) {
                结果.结果头.状态 = L2结构状态::内部不一致;
                return 结果;
            }
            const auto 正式读回 = 读取正式选择当前投影(
                L2任务方法选择记录身份{成员.front().关系.目标节点}, 当前代次);
            if (!正式读回.成功()) {
                结果.结果头.状态 = 正式读回.结果头.状态;
                return 结果;
            }
            结果.正式选择 = 正式读回.正式选择;
            结果.冻结材料 = 正式读回.冻结材料;
            结果.实例方法 = 正式读回.实例方法;
            结果.结果头.状态 = L2结构状态::已读取;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.结果头.状态 = L2结构状态::资源失败;
            return 结果;
        } catch (...) {
            结果.结果头.状态 = L2结构状态::内部不一致;
            return 结果;
        }
    }

    // ===== 按筹办来源幂等身份读取正式选择 =====
    L2按筹办来源幂等身份读取正式选择结果
    按筹办来源幂等身份读取正式选择(
        L2按筹办来源幂等身份读取正式选择请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 当前代次 = L2任务结构内部::读取中性当前事实代次(L1_);
        L2按筹办来源幂等身份读取正式选择结果 结果;
        结果.结果头 = {L2结构合同版本, L2结构状态::入口拒绝,
            当前代次, std::nullopt};
        if (!L2按筹办来源幂等身份读取正式选择请求有效(请求)
            || 当前代次 == 0 || 请求.请求头.期望事实代次 != 当前代次)
            return 结果;
        try {
            const auto 首次 = L2任务结构内部::定位任务筹办来源首次账(
                第一层写入端口_, 请求.原筹办来源幂等身份);
            if (首次.状态 == L2结构状态::未找到) {
                结果.结果头.状态 = L2结构状态::未找到;
                return 结果;
            }
            if (首次.状态 != L2结构状态::已读取
                || !首次.首次请求 || !首次.首次写入结果) {
                结果.结果头.状态 = 首次.状态;
                return 结果;
            }
            if (请求.原完整请求 && *请求.原完整请求 != *首次.首次请求) {
                结果.结果头.状态 = L2结构状态::幂等冲突;
                return 结果;
            }
            const auto 选择编码 = L2任务结构内部::查找治理写入编码(
                *首次.首次写入结果,
                L2任务结构内部::正式选择记录节点写入本地键值);
            if (!选择编码) {
                结果.结果头.状态 = L2结构状态::内部不一致;
                return 结果;
            }
            const auto 正式读回 = 读取正式选择当前投影(
                L2任务方法选择记录身份{*选择编码}, 当前代次);
            if (!正式读回.成功()) {
                结果.结果头.状态 = 正式读回.结果头.状态;
                return 结果;
            }
            结果.首次完整请求 = *首次.首次请求;
            结果.正式选择 = 正式读回.正式选择;
            结果.冻结材料 = 正式读回.冻结材料;
            结果.实例方法 = 正式读回.实例方法;
            结果.结果头.状态 = L2结构状态::已读取;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.结果头.状态 = L2结构状态::资源失败;
            return 结果;
        } catch (...) {
            结果.结果头.状态 = L2结构状态::内部不一致;
            return 结果;
        }
    }

    // ===== 读取当前选中路径 =====
    L2读取当前选中路径结果 读取当前选中路径(
        L2读取当前选中路径请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 截止 = L2任务结构内部::读取中性当前事实代次(L1_);
        if (!L2读取当前选中路径请求有效(请求) || 截止 == 0) {
            L2读取当前选中路径结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = L2结构状态::入口拒绝;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        }
        if (请求.读取类别 != L2读取类别::当前) {
            L2读取当前选中路径结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = L2结构状态::入口拒绝;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        }
        try {
            return L2任务结构内部::读取当前选中路径当前投影(
                L1_, 身份来源定位_, 类型定位_, 请求, 截止);
        } catch (const std::bad_alloc&) {
            L2读取当前选中路径结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = L2结构状态::资源失败;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        } catch (...) {
            L2读取当前选中路径结果 结果;
            结果.结果头.合同版本 = L2结构合同版本;
            结果.结果头.状态 = L2结构状态::内部不一致;
            结果.结果头.事实截止代次 = 截止;
            return 结果;
        }
    }

    // ===== 创建实例方法 =====
    L2创建实例方法结果 创建实例方法(
        L2创建实例方法请求 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [&](L2结构状态 状态, std::uint64_t 截止 = 0) {
            L2创建实例方法结果 结果; 结果.结果头 = {L2结构合同版本, 状态, 截止, std::nullopt}; return 结果; };
        try {
            if (!L2创建实例方法请求有效(请求)) return 失败(L2结构状态::入口拒绝);
            const auto 代次 = L2任务结构内部::检查任务写入当前代次(L1_, 请求.请求头.期望事实代次);
            if (!代次.成功() && !代次.直接提交) return 失败(代次.状态, 代次.事实截止代次);
            L2读取当前选中路径请求 当前请求{{L2结构合同版本, 请求.请求头.期望事实代次}, L2读取类别::当前, 请求.任务};
            const auto 当前 = L2任务结构内部::读取当前选中路径当前投影(L1_, 身份来源定位_, 类型定位_, 当前请求, 请求.请求头.期望事实代次);
            if (!当前.成功() || !当前.当前选中路径 || *当前.当前选中路径 != 请求.当前选中路径)
                return 失败(L2结构状态::引用冲突, 当前.结果头.事实截止代次);
            L2任务方法路径读取请求 路径请求{{L2结构合同版本, 请求.请求头.期望事实代次}, L2读取类别::当前, 请求.当前选中路径};
            const auto 路径 = L2任务结构内部::读取任务方法路径当前投影(
                L1_, 身份来源定位_, 类型定位_, 路径请求,
                请求.请求头.期望事实代次, &正式选择定位_);
            if (!路径.成功() || !路径.路径 || 路径.路径->任务 != 请求.任务
                || 路径.路径->筹办轮次 != 请求.筹办轮次
                || 路径.路径->来源方法 != 请求.来源方法
                || 路径.路径->来源方法内容版本 != 请求.来源方法内容版本)
                return 失败(L2结构状态::引用冲突, 路径.结果头.事实截止代次);
            L1所有者范围一致当前读取请求 当前实例查询;
            当前实例查询.合同版本 = L1所有者范围一致当前读取合同版本;
            当前实例查询.期望事实代次 = 请求.请求头.期望事实代次;
            当前实例查询.所有者 = {身份来源定位_.所有者};
            当前实例查询.源关系组 = {{请求.任务.值, 类型定位_.实例方法当前关系类型}};
            const auto 已有 = L1_.尝试读取所有者范围一致当前投影(当前实例查询);
            if (已有.状态 != L1所有者范围一致当前读取状态::成功
                || 已有.读取事实代次 != 请求.请求头.期望事实代次
                || 已有.源关系组.size() != 1) return 失败(L2结构状态::内部不一致, 已有.读取事实代次);
            if (!已有.源关系组.front().成员.empty())
                return 失败(L2结构状态::引用冲突, 请求.请求头.期望事实代次);
            const auto 键 = [](std::uint32_t 值) noexcept { return L1所有者范围写集本地键{值}; };
            const auto 实例键 = 键(L2任务结构内部::实例方法节点写入本地键值);
            L1所有者范围写集请求 写集;
            写集.合同版本 = L1所有者范围CRUD合同版本; 写集.期望事实代次 = 请求.请求头.期望事实代次; 写集.写入幂等身份 = {请求.幂等身份.值};
            写集.节点 = {{实例键, 节点种类::普通, std::nullopt}};
            写集.关系 = {
                {键(L2任务结构内部::实例方法族归属关系写入本地键值), 实例键, 身份来源定位_.实例方法族归属关系, 类型定位_.实例方法族归属关系类型, 1},
                {键(L2任务结构内部::实例方法任务引用关系写入本地键值), 实例键, 请求.任务.值, 类型定位_.实例方法任务引用关系类型, 1},
                {键(L2任务结构内部::实例方法路径引用关系写入本地键值), 实例键, 请求.当前选中路径.值, 类型定位_.实例方法路径引用关系类型, 1},
                {键(L2任务结构内部::实例方法当前关系写入本地键值), 请求.任务.值, 实例键, 类型定位_.实例方法当前关系类型, 1}};
            写集.值 = {{键(0x0301'220FU), 实例键, 身份来源定位_.实例方法执行轮次属性类型, static_cast<std::int64_t>(请求.执行轮次), 实例键}};
            写集.属性槽变更 = {{实例键, 身份来源定位_.实例方法执行轮次属性类型, {0x0301'220FU}}};
            const auto 写入 = 第一层写入端口_.提交所有者范围中性写集(写集);
            if (写入.状态 != L1所有者范围写入状态::成功 || 写入.新编码映射.empty()) return 失败(L2任务结构内部::映射任务写入状态(写入.状态), 写入.事实代次);
            稳定编码 实例编码{}; for (const auto& [本地键, 编码] : 写入.新编码映射) if (本地键.值 == L2任务结构内部::实例方法节点写入本地键值) 实例编码 = 编码;
            if (!有效(实例编码)) return 失败(L2结构状态::内部不一致, 写入.事实代次);
            L2创建实例方法结果 结果; 结果.结果头 = {L2结构合同版本, L2结构状态::已提交, 写入.事实代次, 写入.事实代次};
            结果.实例方法 = L2实例方法事实{L2实例方法身份{实例编码}, 请求.任务, 请求.当前选中路径, 请求.筹办轮次, 请求.执行轮次, 请求.来源方法, 请求.来源方法内容版本, {写入.事实代次, std::nullopt}};
            return 结果;
        } catch (const std::bad_alloc&) { return 失败(L2结构状态::资源失败); }
        catch (...) { return 失败(L2结构状态::内部不一致); }
    }

    // ===== 读取实例方法 =====
    L2实例方法读取结果 读取实例方法(
        L2实例方法读取请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 截止 = L2任务结构内部::读取中性当前事实代次(L1_);
        L2实例方法读取结果 结果;
        结果.结果头.合同版本 = L2结构合同版本;
        结果.结果头.状态 = L2实例方法读取请求有效(请求) && 截止 == 请求.请求头.期望事实代次 ? L2结构状态::已读取 : L2结构状态::入口拒绝;
        结果.结果头.事实截止代次 = 截止;
        结果.读取类别 = 请求.读取类别;
        结果.历史截止事实代次 = 请求.历史截止事实代次;
        if (结果.结果头.状态 != L2结构状态::已读取 || 请求.读取类别 != L2读取类别::当前) { 结果.结果头.状态=L2结构状态::入口拒绝; return 结果; }
        try { 结果.实例方法=L2任务结构内部::读取实例方法当前投影(L1_,身份来源定位_,类型定位_,请求.实例方法,截止,&正式选择定位_); if(!结果.实例方法) 结果.结果头.状态=L2结构状态::未找到; return 结果; } catch (...) {结果.结果头.状态=L2结构状态::内部不一致;return 结果;}
    }

    // ===== 按任务读取当前实例方法 =====
    L2按任务读取当前实例方法结果 按任务读取当前实例方法(
        L2按任务读取当前实例方法请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 截止 = L2任务结构内部::读取中性当前事实代次(L1_);
        L2按任务读取当前实例方法结果 结果;
        结果.结果头.合同版本 = L2结构合同版本;
        结果.结果头.状态 = L2按任务读取当前实例方法请求有效(请求) && 截止 == 请求.请求头.期望事实代次 ? L2结构状态::已读取 : L2结构状态::入口拒绝;
        结果.结果头.事实截止代次 = 截止;
        结果.读取类别 = 请求.读取类别;
        结果.历史截止事实代次 = 请求.历史截止事实代次;
        if(结果.结果头.状态!=L2结构状态::已读取||请求.读取类别!=L2读取类别::当前){结果.结果头.状态=L2结构状态::入口拒绝;return 结果;}
        try { L1所有者范围一致当前读取请求 查询; 查询.合同版本=L1所有者范围一致当前读取合同版本;查询.期望事实代次=截止;查询.所有者={身份来源定位_.所有者};查询.源关系组={{请求.任务.值,类型定位_.实例方法当前关系类型}};const auto 读=L1_.尝试读取所有者范围一致当前投影(查询);if(读.状态!=L1所有者范围一致当前读取状态::成功||读.读取事实代次!=截止||读.源关系组.size()!=1){结果.结果头.状态=L2结构状态::内部不一致;return 结果;}if(读.源关系组[0].成员.empty())return 结果;if(读.源关系组[0].成员.size()!=1){结果.结果头.状态=L2结构状态::内部不一致;return 结果;}结果.当前实例方法=L2任务结构内部::读取实例方法当前投影(L1_,身份来源定位_,类型定位_,L2实例方法身份{读.源关系组[0].成员[0].关系.目标节点},截止,&正式选择定位_);if(!结果.当前实例方法)结果.结果头.状态=L2结构状态::内部不一致;return 结果;}catch(...){结果.结果头.状态=L2结构状态::内部不一致;return 结果;}
    }

private:
    L2记录任务实际结果结果 记录任务实际结果核心(
        L2记录任务实际结果请求 请求,
        bool 使用v2路径 = false) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [&](L2结构状态 状态, std::uint64_t 截止 = 0) {
            L2记录任务实际结果结果 结果; 结果.结果头 = {L2结构合同版本, 状态, 截止, std::nullopt}; return 结果; };
        try {
            if (!L2记录任务实际结果请求有效(请求)) return 失败(L2结构状态::入口拒绝);
            const auto 代次 = L2任务结构内部::检查任务写入当前代次(L1_, 请求.请求头.期望事实代次);
            if (!代次.成功() && !代次.直接提交) return 失败(代次.状态, 代次.事实截止代次);
            const auto G0 = 请求.请求头.期望事实代次;
            if (使用v2路径) {
                const auto 任务读取 =
                    L2任务结构内部::读取任务轮次核心当前投影(
                        L1_, 身份来源定位_, 类型定位_,
                        {{L2结构合同版本, G0}, L2读取类别::当前,
                            请求.任务, 0}, G0);
                if (!任务读取.成功() || !任务读取.任务
                    || 任务读取.任务->身份 != 请求.任务)
                    return 失败(L2结构状态::引用冲突, G0);
            } else {
                const auto 任务读取 = L2任务结构内部::读取任务当前投影(
                    L1_, 身份来源定位_, 类型定位_,
                    {{L2结构合同版本, G0}, L2读取类别::当前,
                        请求.任务}, G0);
                if (!任务读取.成功() || !任务读取.任务
                    || 任务读取.任务->身份 != 请求.任务)
                    return 失败(L2结构状态::引用冲突, G0);
            }
            L2读取当前选中路径请求 当前请求{
                {L2结构合同版本, G0}, L2读取类别::当前, 请求.任务};
            const auto 当前读取 = L2任务结构内部::读取当前选中路径当前投影(
                L1_, 身份来源定位_, 类型定位_, 当前请求, G0);
            if (!当前读取.成功() || !当前读取.当前选中路径)
                return 失败(L2结构状态::引用冲突, G0);
            if (使用v2路径) {
                L1所有者范围一致当前读取请求 选择请求;
                选择请求.合同版本 = L1所有者范围一致当前读取合同版本;
                选择请求.期望事实代次 = G0;
                选择请求.所有者 = {身份来源定位_.所有者};
                选择请求.源关系组 = {{请求.任务.值,
                    正式选择定位_.任务当前正式选择关系类型}};
                const auto 选择读取 = L1_.尝试读取所有者范围一致当前投影(选择请求);
                if (选择读取.状态 != L1所有者范围一致当前读取状态::成功
                    || 选择读取.读取事实代次 != G0
                    || 选择读取.源关系组.size() != 1
                    || 选择读取.源关系组.front().成员.size() != 1)
                    return 失败(L2结构状态::引用冲突, G0);
                const auto v2 = 读取正式选择投影_v2(
                    L2任务方法选择记录身份{
                        选择读取.源关系组.front().成员.front().关系.目标节点},
                    G0, L2读取类别::当前, 0);
                if (!v2.成功() || !v2.正式选择 || !v2.实例方法
                    || v2.正式选择->任务 != 请求.任务
                    || v2.正式选择->方法 != 请求.方法
                    || v2.正式选择->路径.身份 != *当前读取.当前选中路径
                    || v2.实例方法->身份 != 请求.实例方法
                    || v2.实例方法->任务 != 请求.任务
                    || v2.实例方法->当前选中路径
                        != *当前读取.当前选中路径
                    || v2.实例方法->执行轮次 != 请求.执行轮次
                    || v2.实例方法->来源方法 != 请求.方法
                    || v2.实例方法->来源方法内容版本.值
                        != v2.正式选择->路径.来源方法内容版本.值)
                    return 失败(L2结构状态::引用冲突, G0);
                std::size_t 命中 = 0;
                for (const auto& 动作 :
                    v2.正式选择->路径.完整路径材料.完整有序动作组)
                    if (动作.动作入口 == 请求.动作入口) ++命中;
                if (命中 != 1) return 失败(L2结构状态::引用冲突, G0);
            } else {
                const auto 实例读取 =
                    L2任务结构内部::读取实例方法当前投影(
                        L1_, 身份来源定位_, 类型定位_, 请求.实例方法,
                        G0, &正式选择定位_);
                if (!实例读取 || 实例读取->任务 != 请求.任务
                    || 实例读取->当前选中路径
                        != *当前读取.当前选中路径
                    || 实例读取->执行轮次 != 请求.执行轮次
                    || 实例读取->来源方法 != 请求.方法)
                    return 失败(L2结构状态::引用冲突, G0);
                L2任务方法路径读取请求 路径请求{
                    {L2结构合同版本, G0}, L2读取类别::当前,
                    实例读取->当前选中路径};
                const auto 路径读取 = L2任务结构内部::读取任务方法路径当前投影(
                    L1_, 身份来源定位_, 类型定位_, 路径请求, G0,
                    &正式选择定位_);
                if (!路径读取.成功() || !路径读取.路径
                    || 路径读取.路径->任务 != 请求.任务
                    || 路径读取.路径->来源方法 != 请求.方法
                    || 路径读取.路径->完整路径材料.动作入口
                        != 请求.动作入口.值)
                    return 失败(L2结构状态::引用冲突, G0);
            }
            L1所有者范围一致当前读取请求 槽请求;
            槽请求.合同版本 = L1所有者范围一致当前读取合同版本;
            槽请求.期望事实代次 = 请求.请求头.期望事实代次;
            槽请求.所有者 = {身份来源定位_.所有者};
            槽请求.源关系组 = {{请求.任务.值, 类型定位_.任务实际结果槽关系类型}};
            const auto 槽读取 = L1_.尝试读取所有者范围一致当前投影(槽请求);
            if (槽读取.状态 != L1所有者范围一致当前读取状态::成功
                || 槽读取.读取事实代次 != 请求.请求头.期望事实代次
                || 槽读取.源关系组.size() != 1) return 失败(L2结构状态::内部不一致, 槽读取.读取事实代次);
            const L1所有者范围一致关系对端投影* 同轮成员 = nullptr;
            for (const auto& 成员 : 槽读取.源关系组.front().成员) {
                if (成员.关系.退出事实代次
                    || 成员.关系.角色或顺序 != static_cast<std::int64_t>(请求.执行轮次)) continue;
                if (同轮成员)
                    return 失败(L2结构状态::内部不一致, G0);
                同轮成员 = std::addressof(成员);
            }
            if (同轮成员) {
                const auto 已有 = L2任务结构内部::读取任务实际结果当前投影(
                    L1_, 身份来源定位_, 类型定位_,
                    L2任务实际结果身份{同轮成员->关系.目标节点}, G0);
                if (!已有) return 失败(L2结构状态::内部不一致, G0);
                const bool 同义 = 已有->任务 == 请求.任务
                    && 已有->实例方法 == 请求.实例方法 && 已有->执行轮次 == 请求.执行轮次
                    && 已有->方法 == 请求.方法 && 已有->动作入口 == 请求.动作入口
                    && 已有->场景 == 请求.场景 && 已有->主体 == 请求.主体
                    && 已有->实际状态 == 请求.实际状态 && 已有->动态证据 == 请求.动态证据
                    && 已有->来源材料版本 == 请求.来源材料版本 && 已有->运行代次 == 请求.运行代次;
                if (!同义) return 失败(L2结构状态::引用冲突, G0);
                L2记录任务实际结果结果 重复;
                重复.结果头 = {L2结构合同版本, L2结构状态::精确重复,
                    G0, std::nullopt};
                重复.实际结果 = *已有;
                return 重复;
            }
            const auto 键 = [](std::uint32_t 值) noexcept { return L1所有者范围写集本地键{值}; };
            const auto 结果键 = 键(L2任务结构内部::结果节点写入本地键值);
            L1所有者范围写集请求 写集;
            写集.合同版本 = L1所有者范围CRUD合同版本; 写集.期望事实代次 = 请求.请求头.期望事实代次; 写集.写入幂等身份 = {请求.幂等身份.值};
            写集.节点 = {{结果键, 节点种类::普通, std::nullopt}};
            写集.关系 = {
                {键(L2任务结构内部::结果族归属关系写入本地键值), 结果键, 身份来源定位_.结果族归属关系, 类型定位_.结果族归属关系类型, 1},
                {键(L2任务结构内部::结果实例方法关系写入本地键值), 结果键, 请求.实例方法.值, 类型定位_.结果实例方法关系类型, 1},
                {键(L2任务结构内部::结果实际状态关系写入本地键值), 结果键, 请求.实际状态.值, 类型定位_.结果实际状态关系类型, 1},
                {键(L2任务结构内部::结果动态证据关系写入本地键值), 结果键, 请求.动态证据.值, 类型定位_.结果动态证据关系类型, 1},
                {键(L2任务结构内部::结果方法关系写入本地键值), 结果键, 请求.方法.值, 类型定位_.结果方法关系类型, 1},
                {键(L2任务结构内部::结果动作入口关系写入本地键值), 结果键, 请求.动作入口.值, 类型定位_.结果动作入口关系类型, 1},
                {键(L2任务结构内部::结果场景关系写入本地键值), 结果键, 请求.场景.值, 类型定位_.结果场景关系类型, 1},
                {键(L2任务结构内部::结果主体关系写入本地键值), 结果键, 请求.主体.值, 类型定位_.结果主体关系类型, 1},
                {键(L2任务结构内部::任务结果槽关系写入本地键值), 请求.任务.值, 结果键, 类型定位_.任务实际结果槽关系类型, static_cast<std::int64_t>(请求.执行轮次)}};
            写集.值 = {
                {键(L2任务结构内部::结果来源材料版本值写入本地键值), 结果键,
                    身份来源定位_.结果来源材料版本属性类型,
                    std::vector<std::uint64_t>{请求.来源材料版本}, 结果键},
                {键(L2任务结构内部::结果运行代次值写入本地键值), 结果键,
                    身份来源定位_.结果运行代次属性类型,
                    std::vector<std::uint64_t>{请求.运行代次}, 结果键}};
            写集.属性槽变更 = {
                {结果键, 身份来源定位_.结果来源材料版本属性类型, {L2任务结构内部::结果来源材料版本值写入本地键值}},
                {结果键, 身份来源定位_.结果运行代次属性类型, {L2任务结构内部::结果运行代次值写入本地键值}}};
            const auto 写入 = 第一层写入端口_.提交所有者范围中性写集(写集);
            if (写入.状态 != L1所有者范围写入状态::成功) return 失败(L2任务结构内部::映射任务写入状态(写入.状态), 写入.事实代次);
            稳定编码 编码{}; for (const auto& [本地键, 值] : 写入.新编码映射) if (本地键.值 == L2任务结构内部::结果节点写入本地键值) 编码 = 值;
            std::optional<L2任务实际结果事实> 事实;
#ifdef ARCH_L4_SELF_GOVERNANCE_TEST_SEAM
            if (ARCH_任务实际结果提交后首次读回失败一次_) {
                ARCH_任务实际结果提交后首次读回失败一次_ = false;
            } else
#endif
            if (有效(编码)) {
                事实 = L2任务结构内部::读取任务实际结果当前投影(
                    L1_, 身份来源定位_, 类型定位_,
                    L2任务实际结果身份{编码}, 写入.事实代次);
            }
            if (!事实) return 失败(L2结构状态::内部不一致, 写入.事实代次);
            L2记录任务实际结果结果 结果; 结果.结果头 = {L2结构合同版本, L2结构状态::已提交, 写入.事实代次, 写入.事实代次}; 结果.实际结果 = *事实; return 结果;
        } catch (const std::bad_alloc&) { return 失败(L2结构状态::资源失败); }
        catch (...) { return 失败(L2结构状态::内部不一致); }
    }

public:
    // v1 只保留源码兼容，确定性拒绝且不进入事实基座。
    L2记录任务实际结果结果 记录任务实际结果(
        L2记录任务实际结果请求) noexcept {
        L2记录任务实际结果结果 结果;
        结果.结果头 = {L2结构合同版本, L2结构状态::入口拒绝,
            0, std::nullopt};
        return 结果;
    }

    L2记录任务实际结果结果_v2 记录任务实际结果_v2(
        L2记录任务实际结果请求_v2 请求) noexcept {
        const auto 失败 = [](L2记录任务实际结果状态_v2 状态) noexcept {
            return L2记录任务实际结果结果_v2{
                状态, std::nullopt, std::nullopt, 0};
        };
        const auto 映射失败 = [](L2结构状态 状态) noexcept {
            switch (状态) {
            case L2结构状态::入口拒绝:
                return L2记录任务实际结果状态_v2::入口拒绝;
            case L2结构状态::许可拒绝:
                return L2记录任务实际结果状态_v2::许可拒绝;
            case L2结构状态::事实代次漂移:
            case L2结构状态::已退出:
                return L2记录任务实际结果状态_v2::当前性漂移;
            case L2结构状态::幂等冲突:
                return L2记录任务实际结果状态_v2::幂等冲突;
            case L2结构状态::引用冲突:
            case L2结构状态::未找到:
                return L2记录任务实际结果状态_v2::引用冲突;
            case L2结构状态::资源失败:
                return L2记录任务实际结果状态_v2::资源失败;
            default:
                return L2记录任务实际结果状态_v2::内部错误;
            }
        };
        if (!L2记录任务实际结果请求_v2有效(请求))
            return 失败(L2记录任务实际结果状态_v2::入口拒绝);
        try {
            const auto 首次 = 第一层写入端口_.读取首次写入材料({
                L1所有者范围首次写入读取合同版本,
                L1所有者范围写入幂等身份{请求.幂等身份.值}});
            if (首次.状态 == L1所有者范围读取状态::成功) {
                if (!首次.首次写入结果 || 首次.首次写入结果->事实代次 == 0)
                    return 失败(L2记录任务实际结果状态_v2::内部错误);
                稳定编码 结果编码{};
                for (const auto& [本地键, 编码] :
                    首次.首次写入结果->新编码映射) {
                    if (本地键.值 == L2任务结构内部::结果节点写入本地键值)
                        结果编码 = 编码;
                }
                const auto 观察 = L2任务结构内部::读取中性当前事实代次(L1_);
                const auto 首次G1 = 首次.首次写入结果->事实代次;
                const auto 事实 = 有效(结果编码) && 观察 >= 首次G1
                    ? L2任务结构内部::读取任务实际结果历史投影(
                        L1_, 身份来源定位_, 类型定位_,
                        L2任务实际结果身份{结果编码}, 首次G1, 观察)
                    : std::nullopt;
                if (!事实)
                    return {L2记录任务实际结果状态_v2::已可能发布,
                        std::nullopt,
                        L2任务实际结果提交见证{第一层写入端口_.所有者身份(),
                            请求.幂等身份, 首次G1, 1}, 0};
                if (事实->任务 != 请求.任务 || 事实->实例方法 != 请求.实例方法
                    || 事实->执行轮次 != 请求.执行轮次
                    || 事实->方法 != 请求.方法 || 事实->动作入口 != 请求.动作入口
                    || 事实->场景 != 请求.场景 || 事实->主体 != 请求.主体
                    || 事实->实际状态 != 请求.实际状态
                    || 事实->动态证据 != 请求.动态证据
                    || 事实->来源材料版本 != 请求.来源材料版本
                    || 事实->运行代次 != 请求.运行代次)
                    return 失败(L2记录任务实际结果状态_v2::幂等冲突);
                L2记录任务实际结果结果_v2 结果{
                    L2记录任务实际结果状态_v2::精确重复, 事实,
                    L2任务实际结果提交见证{第一层写入端口_.所有者身份(),
                        请求.幂等身份, 首次G1, 1}, 观察};
                return 结果.成功() ? 结果
                    : 失败(L2记录任务实际结果状态_v2::内部错误);
            }
            if (首次.状态 != L1所有者范围读取状态::未找到) {
                if (首次.状态 == L1所有者范围读取状态::许可拒绝)
                    return 失败(L2记录任务实际结果状态_v2::许可拒绝);
                if (首次.状态 == L1所有者范围读取状态::事实代次漂移
                    || 首次.状态 == L1所有者范围读取状态::已退出)
                    return 失败(L2记录任务实际结果状态_v2::当前性漂移);
                if (首次.状态 == L1所有者范围读取状态::资源失败)
                    return 失败(L2记录任务实际结果状态_v2::资源失败);
                return 失败(L2记录任务实际结果状态_v2::内部错误);
            }
            const L2记录任务实际结果请求 v1{请求.请求头, 请求.幂等身份,
                请求.任务, 请求.实例方法, 请求.执行轮次, 请求.方法,
                请求.动作入口, 请求.场景, 请求.主体, 请求.实际状态,
                请求.动态证据, 请求.来源材料版本, 请求.运行代次};
            const auto 写入 = 记录任务实际结果核心(v1, true);
            if (写入.结果头.状态 != L2结构状态::已提交
                && 写入.结果头.状态 != L2结构状态::精确重复) {
                const auto 提交后首次 = 第一层写入端口_.读取首次写入材料({
                    L1所有者范围首次写入读取合同版本,
                    L1所有者范围写入幂等身份{请求.幂等身份.值}});
                if (提交后首次.状态 == L1所有者范围读取状态::成功
                    && 提交后首次.首次写入结果
                    && 提交后首次.首次写入结果->事实代次 != 0)
                    return {L2记录任务实际结果状态_v2::已可能发布,
                        std::nullopt,
                        L2任务实际结果提交见证{
                            第一层写入端口_.所有者身份(), 请求.幂等身份,
                            提交后首次.首次写入结果->事实代次, 1}, 0};
                return 失败(映射失败(写入.结果头.状态));
            }
            if (!写入.实际结果 || 写入.结果头.事实截止代次 == 0)
                return 失败(L2记录任务实际结果状态_v2::内部错误);
            const auto 见证 = L2任务实际结果提交见证{
                第一层写入端口_.所有者身份(), 请求.幂等身份,
                写入.结果头.事实截止代次, 1};
            L2记录任务实际结果结果_v2 结果{
                写入.结果头.状态 == L2结构状态::精确重复
                    ? L2记录任务实际结果状态_v2::精确重复
                    : L2记录任务实际结果状态_v2::已形成,
                写入.实际结果, 见证, 写入.结果头.事实截止代次};
            return 结果.成功() ? 结果
                : L2记录任务实际结果结果_v2{
                    L2记录任务实际结果状态_v2::已可能发布,
                    std::nullopt, 见证, 0};
        } catch (const std::bad_alloc&) {
            return 失败(L2记录任务实际结果状态_v2::资源失败);
        } catch (...) {
            return 失败(L2记录任务实际结果状态_v2::内部错误);
        }
    }

    L2任务实际结果读取结果 读取任务实际结果(
        L2任务实际结果读取请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        L2任务实际结果读取结果 结果;
        const auto 观察代次 = L2任务结构内部::读取中性当前事实代次(L1_);
        结果.结果头 = {L2结构合同版本, L2结构状态::入口拒绝,
            观察代次, std::nullopt};
        结果.读取类别 = 请求.读取类别; 结果.历史截止事实代次 = 请求.历史截止事实代次;
        if (!L2任务实际结果读取请求有效(请求)
            || 观察代次 != 请求.请求头.期望事实代次) return 结果;
        try {
            const auto 节点读取 = L1_.读取所有者范围历史事实(
                {L1所有者范围CRUD合同版本, 请求.实际结果.值});
            if (节点读取.读取事实代次 != 观察代次) {
                结果.结果头.状态 = L2结构状态::内部不一致;
                return 结果;
            }
            if (节点读取.状态 == L1所有者范围读取状态::未找到) {
                结果.结果头.状态 = L2结构状态::已读取;
                return 结果;
            }
            const auto* 节点 = 节点读取.事实
                ? std::get_if<L1所有者范围节点事实>(&*节点读取.事实) : nullptr;
            if (节点读取.状态 != L1所有者范围读取状态::成功 || !节点) {
                结果.结果头.状态 = L2结构状态::内部不一致;
                return 结果;
            }
            if (请求.读取类别 == L2读取类别::当前) {
                const auto 事实 = L2任务结构内部::读取任务实际结果当前投影(
                    L1_, 身份来源定位_, 类型定位_, 请求.实际结果, 观察代次);
                if (!事实) {
                    结果.结果头.状态 = L2结构状态::内部不一致;
                    return 结果;
                }
                结果.实际结果 = *事实;
            } else {
                const auto 截止 = 请求.历史截止事实代次;
                if (节点->创建事实代次 <= 截止
                    && (!节点->退出事实代次 || *节点->退出事实代次 > 截止)) {
                    const auto 事实 = L2任务结构内部::读取任务实际结果历史投影(
                        L1_, 身份来源定位_, 类型定位_, 请求.实际结果,
                        截止, 观察代次);
                    if (!事实) {
                        结果.结果头.状态 = L2结构状态::内部不一致;
                        return 结果;
                    }
                    结果.实际结果 = *事实;
                }
            }
            结果.结果头.状态 = L2结构状态::已读取;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.结果头.状态 = L2结构状态::资源失败;
            return 结果;
        } catch (...) {
            结果.结果头.状态 = L2结构状态::内部不一致;
            return 结果;
        }
    }

    L2按任务与执行轮次读取实际结果结果 按任务与执行轮次读取实际结果(
        L2按任务与执行轮次读取实际结果请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        L2按任务与执行轮次读取实际结果结果 结果;
        const auto 观察代次 = L2任务结构内部::读取中性当前事实代次(L1_);
        结果.结果头 = {L2结构合同版本, L2结构状态::入口拒绝,
            观察代次, std::nullopt};
        结果.读取类别 = 请求.读取类别;
        结果.历史截止事实代次 = 请求.历史截止事实代次;
        if (!L2按任务与执行轮次读取实际结果请求有效(请求)
            || 观察代次 != 请求.请求头.期望事实代次) return 结果;
        try {
            std::optional<L2任务实际结果身份> 结果身份;
            if (请求.读取类别 == L2读取类别::当前) {
                L1所有者范围一致当前读取请求 槽请求;
                槽请求.合同版本 = L1所有者范围一致当前读取合同版本;
                槽请求.期望事实代次 = 观察代次;
                槽请求.所有者 = {身份来源定位_.所有者};
                槽请求.源关系组 = {{请求.任务.值,
                    类型定位_.任务实际结果槽关系类型}};
                const auto 槽 = L1_.尝试读取所有者范围一致当前投影(槽请求);
                if (槽.状态 != L1所有者范围一致当前读取状态::成功
                    || 槽.读取事实代次 != 观察代次 || 槽.源关系组.size() != 1) {
                    结果.结果头.状态 = L2结构状态::内部不一致;
                    return 结果;
                }
                for (const auto& 成员 : 槽.源关系组.front().成员) {
                    if (成员.关系.退出事实代次
                        || 成员.关系.角色或顺序
                            != static_cast<std::int64_t>(请求.执行轮次)) continue;
                    if (结果身份) {
                        结果.结果头.状态 = L2结构状态::内部不一致;
                        return 结果;
                    }
                    结果身份 = L2任务实际结果身份{成员.关系.目标节点};
                }
            } else {
                const auto 截止 = 请求.历史截止事实代次;
                const auto 槽 = L1_.读取所有者范围历史关系组(
                    {L1所有者范围CRUD合同版本,
                        L1所有者范围关系端点方向::源, 请求.任务.值,
                        类型定位_.任务实际结果槽关系类型, 截止});
                if (槽.状态 != L1所有者范围读取状态::成功
                    || 槽.历史截止事实代次 != 截止
                    || 槽.读取事实代次 != 观察代次) {
                    结果.结果头.状态 = L2结构状态::内部不一致;
                    return 结果;
                }
                for (const auto& 关系 : 槽.关系组) {
                    if (关系.写入所有者 != 身份来源定位_.所有者
                        || 关系.源节点 != 请求.任务.值
                        || 关系.关系类型节点 != 类型定位_.任务实际结果槽关系类型
                        || (关系.退出事实代次 && *关系.退出事实代次 <= 截止)) {
                        结果.结果头.状态 = L2结构状态::内部不一致;
                        return 结果;
                    }
                    if (关系.角色或顺序
                        != static_cast<std::int64_t>(请求.执行轮次)) continue;
                    if (结果身份) {
                        结果.结果头.状态 = L2结构状态::内部不一致;
                        return 结果;
                    }
                    结果身份 = L2任务实际结果身份{关系.目标节点};
                }
            }
            if (结果身份) {
                const auto 事实 = 请求.读取类别 == L2读取类别::当前
                    ? L2任务结构内部::读取任务实际结果当前投影(
                        L1_, 身份来源定位_, 类型定位_, *结果身份, 观察代次)
                    : L2任务结构内部::读取任务实际结果历史投影(
                        L1_, 身份来源定位_, 类型定位_, *结果身份,
                        请求.历史截止事实代次, 观察代次);
                if (!事实) {
                    结果.结果头.状态 = L2结构状态::内部不一致;
                    return 结果;
                }
                结果.实际结果 = *事实;
            }
            结果.结果头.状态 = L2结构状态::已读取;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.结果头.状态 = L2结构状态::资源失败;
            return 结果;
        } catch (...) {
            结果.结果头.状态 = L2结构状态::内部不一致;
            return 结果;
        }
    }

    // ===== 登记执行结果待验证首态 =====
    L2新增执行结果待验证状态结果 新增执行结果待验证状态(
        L2新增执行结果待验证状态请求 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [&](L2结构状态 状态, std::uint64_t 截止 = 0) {
            L2新增执行结果待验证状态结果 结果;
            结果.结果头 = {L2结构合同版本, 状态, 截止, std::nullopt};
            return 结果;
        };
        try {
            if (!L2新增执行结果待验证状态请求有效(请求))
                return 失败(L2结构状态::入口拒绝);

            // legacy 入口只允许既有幂等账精确重放；首次请求不再形成
            // 任务治理状态或目标裁决证据。
            {
                const auto 首次 = 第一层写入端口_.读取首次写入材料(
                    {L1所有者范围首次写入读取合同版本,
                        {请求.幂等身份.值}});
                if (首次.状态 == L1所有者范围读取状态::未找到)
                    return 失败(L2结构状态::入口拒绝);
                if (首次.状态 != L1所有者范围读取状态::成功)
                    return 失败(L2任务结构内部::映射子目标记录读取状态(
                        首次.状态));
                if (!首次.首次规范化写集 || !首次.首次写入结果)
                    return 失败(L2结构状态::内部不一致);
                const auto 当前代次 =
                    L2任务结构内部::读取中性当前事实代次(L1_);
                if (当前代次 == 0)
                    return 失败(L2结构状态::内部不一致);
                const auto& 首次结果 = *首次.首次写入结果;
                const auto 状态编码 = L2任务结构内部::查找治理写入编码(
                    首次结果, L2任务结构内部::新治理状态节点写入本地键值);
                const auto 证据编码 = L2任务结构内部::查找治理写入编码(
                    首次结果, L2任务结构内部::新目标裁决证据节点写入本地键值);
                if (!状态编码 || !证据编码)
                    return 失败(L2结构状态::内部不一致);
                const auto 投影 = 状态编码
                    ? L2任务结构内部::读取任务治理状态历史投影(
                        L1_, 身份来源定位_, 类型定位_,
                        L2任务治理状态身份{*状态编码}, 首次结果.事实代次,
                        当前代次)
                    : std::nullopt;
                if (!投影 || 投影->证据.身份.值 != *证据编码)
                    return 失败(L2结构状态::内部不一致);
                if (投影->状态.任务 != 请求.任务
                    || 投影->状态.任务虚拟存在 != 请求.任务虚拟存在
                    || 投影->状态.生命周期发生时间 != 请求.生命周期发生时间
                    || 投影->状态.来源方法路径 != 请求.来源方法路径
                    || 投影->状态.来源实例方法 != 请求.来源实例方法
                    || 投影->状态.来源任务实际结果 != 请求.来源任务实际结果
                    || 投影->证据.材料 != 请求.目标裁决)
                    return 失败(L2结构状态::幂等冲突);
                const auto 重放 = 第一层写入端口_.提交所有者范围中性写集(
                    *首次.首次规范化写集);
                if (重放.状态 != L1所有者范围写入状态::精确重复) {
                    const auto 状态 = L2任务结构内部::映射任务写入状态(
                        重放.状态);
                    return 失败(状态 == L2结构状态::已提交
                        ? L2结构状态::内部不一致 : 状态);
                }
                if (重放.事实代次 != 首次结果.事实代次
                    || 重放.新编码映射 != 首次结果.新编码映射)
                    return 失败(L2结构状态::内部不一致);
                L2新增执行结果待验证状态结果 结果;
                结果.结果头 = {L2结构合同版本, L2结构状态::精确重复,
                    首次结果.事实代次, 首次结果.事实代次};
                结果.已登记状态 = 投影->状态;
                结果.目标裁决证据 = 投影->证据;
                return 结果;
            }

        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2结构状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致);
        }
    }

    // ===== 提交目标未达成待重筹办迁移 =====
    L2提交目标未达成待重筹办迁移结果 提交目标未达成待重筹办迁移(
        L2提交目标未达成待重筹办迁移请求 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [&](L2结构状态 状态, std::uint64_t 截止 = 0) {
            L2提交目标未达成待重筹办迁移结果 结果;
            结果.结果头 = {L2结构合同版本, 状态, 截止, std::nullopt};
            return 结果;
        };
        try {
            if (!L2提交目标未达成待重筹办迁移请求有效(请求))
                return 失败(L2结构状态::入口拒绝);
            // legacy 迁移入口只允许既有幂等账精确重放；无账请求不再
            // 建立待重筹办状态、目标裁决证据或后继准备记录。
            {
                const auto 首次 = 第一层写入端口_.读取首次写入材料(
                    {L1所有者范围首次写入读取合同版本,
                        {请求.幂等身份.值}});
                if (首次.状态 == L1所有者范围读取状态::未找到)
                    return 失败(L2结构状态::入口拒绝);
                if (首次.状态 != L1所有者范围读取状态::成功)
                    return 失败(L2任务结构内部::映射子目标记录读取状态(
                        首次.状态));
                if (!首次.首次规范化写集 || !首次.首次写入结果)
                    return 失败(L2结构状态::内部不一致);
                const auto 当前代次 =
                    L2任务结构内部::读取中性当前事实代次(L1_);
                if (当前代次 == 0)
                    return 失败(L2结构状态::内部不一致);
                const auto& 首次结果 = *首次.首次写入结果;
                const auto 状态编码 = L2任务结构内部::查找治理写入编码(
                    首次结果, L2任务结构内部::新治理状态节点写入本地键值);
                if (!状态编码)
                    return 失败(L2结构状态::内部不一致);
                const auto 目标投影 = 状态编码
                    ? L2任务结构内部::读取任务治理状态历史投影(
                        L1_, 身份来源定位_, 类型定位_,
                        L2任务治理状态身份{*状态编码}, 首次结果.事实代次,
                        当前代次)
                    : std::nullopt;
                const auto 前态投影 = L2任务结构内部::读取任务治理状态历史投影(
                    L1_, 身份来源定位_, 类型定位_, 请求.前一治理状态,
                    请求.请求头.期望事实代次, 当前代次);
                if (!目标投影 || !前态投影)
                    return 失败(L2结构状态::内部不一致);
                if (目标投影->状态.任务 != 请求.任务
                    || 目标投影->状态.任务虚拟存在 != 请求.任务虚拟存在
                    || 目标投影->状态.生命周期发生时间 != 请求.生命周期发生时间
                    || 目标投影->状态.前一治理状态 !=请求.前一治理状态
                    || 目标投影->状态.来源方法路径 != 请求.旧当前路径
                    || 目标投影->状态.来源实例方法 != 请求.旧当前实例
                    || 目标投影->证据.身份 != 请求.目标裁决证据)
                    return 失败(L2结构状态::幂等冲突);
                const auto 重放 = 第一层写入端口_.提交所有者范围中性写集(
                    *首次.首次规范化写集);
                if (重放.状态 != L1所有者范围写入状态::精确重复) {
                    const auto 状态 = L2任务结构内部::映射任务写入状态(
                        重放.状态);
                    return 失败(状态 == L2结构状态::已提交
                        ? L2结构状态::内部不一致 : 状态);
                }
                if (重放.事实代次 != 首次结果.事实代次
                    || 重放.新编码映射 != 首次结果.新编码映射)
                    return 失败(L2结构状态::内部不一致);
                L2提交目标未达成待重筹办迁移结果 结果;
                结果.结果头 = {L2结构合同版本, L2结构状态::精确重复,
                    首次结果.事实代次, 首次结果.事实代次};
                结果.目标状态 = 目标投影->状态;
                结果.前驱状态 = 前态投影->状态;
                结果.目标裁决证据 = 目标投影->证据;
                const auto 旧路径读取 = L2任务结构内部::读取任务方法路径当前投影(
                    L1_, 身份来源定位_, 类型定位_,
                    {{L2结构合同版本, 当前代次}, L2读取类别::当前,
                        请求.旧当前路径}, 当前代次, &正式选择定位_);
                结果.旧路径 = 旧路径读取.路径;
                结果.旧实例 = L2任务结构内部::读取实例方法当前投影(
                    L1_, 身份来源定位_, 类型定位_, 请求.旧当前实例,
                    当前代次, &正式选择定位_);
                if (!结果.旧路径 || !结果.旧实例)
                    return 失败(L2结构状态::内部不一致, 当前代次);
                const auto 后继编码 = L2任务结构内部::查找治理写入编码(
                    首次结果, L2任务结构内部::后继准备记录节点写入键);
                if (请求.轮次分区 == L2任务筹办轮次分区::统一轮次权威) {
                    if (!后继编码) return 失败(L2结构状态::幂等冲突, 当前代次);
                    结果.后继准备记录 =
                        L2任务结构内部::读取后继筹办准备历史投影(
                            L1_, 筹办轮次定位_,
                            L2任务后继筹办准备记录身份{*后继编码},
                            首次结果.事实代次, 当前代次);
                    const auto& 推进 = *请求.统一轮次推进;
                    if (!结果.后继准备记录
                        || 结果.后继准备记录->准备幂等身份 != 请求.幂等身份
                        || 结果.后继准备记录->任务 != 请求.任务
                        || 结果.后继准备记录->轮次权威 != 推进.轮次权威
                        || 结果.后继准备记录->首次准备记录 != 推进.首次准备记录
                        || 结果.后继准备记录->前一后继准备记录 !=
                            推进.期望前一后继准备记录
                        || 结果.后继准备记录->前一筹办轮次 !=
                            推进.期望前一筹办轮次
                        || 结果.后继准备记录->触发请求身份 != 推进.触发请求身份
                        || 结果.后继准备记录->运行代次 != 推进.运行代次
                        || 结果.后继准备记录->触发业务幂等身份 !=
                            推进.触发业务幂等身份
                        || 结果.后继准备记录->来源共同事实截止 !=
                            推进.来源共同事实截止)
                        return 失败(L2结构状态::幂等冲突, 当前代次);
                    结果.轮次分区 = L2任务筹办轮次分区::统一轮次权威;
                } else {
                    if (后继编码) return 失败(L2结构状态::幂等冲突, 当前代次);
                    结果.轮次分区 = L2任务筹办轮次分区::遗留实例轮次;
                }
                return 结果;
            }
        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2结构状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致);
        }
    }

    L2任务治理状态读取结果 读取任务治理状态(
        L2任务治理状态读取请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        L2任务治理状态读取结果 结果;
        const auto 观察代次 = L2任务结构内部::读取中性当前事实代次(L1_);
        结果.结果头 = {L2结构合同版本, L2结构状态::入口拒绝,
            观察代次, std::nullopt};
        结果.读取类别 = 请求.读取类别;
        结果.历史截止事实代次 = 请求.历史截止事实代次;
        if (!L2任务治理状态读取请求有效(请求)
            || 请求.请求头.期望事实代次 != 观察代次)
            return 结果;
        try {
            const auto 截止 = 请求.读取类别 == L2读取类别::当前
                ? 观察代次 : 请求.历史截止事实代次;
            const auto 投影 = L2任务结构内部::读取任务治理状态历史投影(
                L1_, 身份来源定位_, 类型定位_, 请求.状态, 截止, 观察代次);
            if (投影) {
                结果.状态 = 投影->状态;
                结果.目标裁决证据 = 投影->证据;
            }
            结果.结果头.状态 = L2结构状态::已读取;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.结果头.状态 = L2结构状态::资源失败;
            return 结果;
        } catch (...) {
            结果.结果头.状态 = L2结构状态::内部不一致;
            return 结果;
        }
    }

    L2任务当前治理状态读取结果 读取任务当前治理状态(
        L2任务当前治理状态读取请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        L2任务当前治理状态读取结果 结果;
        const auto 观察代次 = L2任务结构内部::读取中性当前事实代次(L1_);
        结果.结果头 = {L2结构合同版本, L2结构状态::入口拒绝,
            观察代次, std::nullopt};
        if (!L2任务当前治理状态读取请求有效(请求)
            || 请求.请求头.期望事实代次 != 观察代次)
            return 结果;
        try {
            const auto 关系组 = L1_.读取所有者范围历史关系组(
                {L1所有者范围CRUD合同版本,
                    L1所有者范围关系端点方向::源, 请求.任务.值,
                    类型定位_.任务当前治理状态关系类型, 观察代次});
            if (关系组.状态 != L1所有者范围读取状态::成功
                || 关系组.读取事实代次 != 观察代次
                || 关系组.关系组.size() > 1) {
                结果.结果头.状态 = L2结构状态::内部不一致;
                return 结果;
            }
            if (!关系组.关系组.empty()) {
                const auto 投影 = L2任务结构内部::读取任务治理状态历史投影(
                    L1_, 身份来源定位_, 类型定位_,
                    L2任务治理状态身份{关系组.关系组.front().目标节点},
                    观察代次, 观察代次);
                if (!投影 || 投影->状态.任务 != 请求.任务) {
                    结果.结果头.状态 = L2结构状态::内部不一致;
                    return 结果;
                }
                结果.当前状态 = 投影->状态;
                结果.目标裁决证据 = 投影->证据;
            }
            结果.结果头.状态 = L2结构状态::已读取;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.结果头.状态 = L2结构状态::资源失败;
            return 结果;
        } catch (...) {
            结果.结果头.状态 = L2结构状态::内部不一致;
            return 结果;
        }
    }

    // ===== 退出当前实例方法 =====
    L2退出当前实例方法结果 退出当前实例方法(
        L2退出当前实例方法请求 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 代次前 = L2任务结构内部::读取中性当前事实代次(L1_);
        L2退出当前实例方法结果 结果;
        结果.结果头.合同版本 = L2结构合同版本;
        结果.结果头.状态 = L2结构状态::入口拒绝;
        结果.结果头.事实截止代次 = 代次前;
        return 结果;
    }

    // ===== 读取实例方法历史 =====
    L2实例方法历史读取结果 读取实例方法历史(
        L2实例方法历史读取请求 请求) const noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 截止 = L2任务结构内部::读取中性当前事实代次(L1_);
        L2实例方法历史读取结果 结果;
        结果.结果头.合同版本 = L2结构合同版本;
        结果.结果头.状态 =
            (L2实例方法历史读取请求有效(请求) && 截止 != 0)
                ? L2结构状态::已读取
                : L2结构状态::入口拒绝;
        结果.结果头.事实截止代次 = 截止;
        结果.历史截止事实代次 = 请求.历史截止事实代次;
        return 结果;
    }

private:
    L2任务结构内部::当前任务轮次结构化投影
    读取当前任务轮次结构化投影(
        L2任务身份 任务, std::uint64_t G0) const noexcept {
        L2任务结构内部::当前任务轮次结构化投影 投影;
        if (!有效(任务.值) || G0 == 0) {
            投影.公共状态 = L2结构状态::入口拒绝;
            投影.专用状态 = L2按任务读取当前任务与轮次状态::入口拒绝;
            return 投影;
        }
        L2按任务读取当前任务与轮次请求 原始请求;
        原始请求.合同版本 = L2按任务读取当前任务与轮次合同版本;
        原始请求.请求头 = {L2结构合同版本, G0};
        原始请求.任务 = 任务;
        const auto 原始 = 按任务读取当前任务与轮次原始(原始请求);
        投影.公共状态 = 原始.结果头.状态;
        投影.专用状态 = 原始.状态;
        投影.任务 = 原始.任务;
        投影.当前任务轮次 = 原始.当前任务轮次;
        投影.本次正式读回截止 = 原始.本次正式读回截止;
        return 投影;
    }

    std::optional<L1所有者范围节点事实> 读取任务所有者活动节点(
        稳定编码 身份, std::uint64_t 截止) const {
        const auto 读取 = L1_.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 身份});
        const auto* 节点 = 读取.事实
            ? std::get_if<L1所有者范围节点事实>(&*读取.事实) : nullptr;
        if (读取.状态 != L1所有者范围读取状态::成功 || !节点
            || 节点->写入所有者 != 第一层写入端口_.所有者身份()
            || 节点->创建事实代次 == 0 || 节点->创建事实代次 > 截止
            || (节点->退出事实代次 && *节点->退出事实代次 <= 截止))
            return std::nullopt;
        return *节点;
    }

    std::optional<std::vector<L1所有者范围关系事实>> 读取任务所有者活动关系组(
        稳定编码 源, 稳定编码 类型, std::uint64_t 截止) const {
        const auto 读取 = L1_.读取所有者范围历史关系组({
            L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源,
            源, 类型, 截止});
        if (读取.状态 != L1所有者范围读取状态::成功
            || 读取.历史截止事实代次 != 截止) return std::nullopt;
        std::vector<L1所有者范围关系事实> 结果;
        for (const auto& 关系 : 读取.关系组) {
            if (关系.写入所有者 != 第一层写入端口_.所有者身份()
                || 关系.源节点 != 源 || 关系.关系类型节点 != 类型
                || 关系.创建事实代次 == 0 || 关系.创建事实代次 > 截止
                || (关系.退出事实代次 && *关系.退出事实代次 <= 截止))
                return std::nullopt;
            结果.push_back(关系);
        }
        std::sort(结果.begin(), 结果.end(), [](const auto& 左, const auto& 右) {
            return 左.角色或顺序 < 右.角色或顺序;
        });
        return 结果;
    }

    std::optional<std::vector<std::uint64_t>> 读取任务所有者唯一U64材料(
        稳定编码 节点, 稳定编码 属性类型, std::uint64_t 截止) const {
        const auto 读取 = L1_.读取所有者范围历史属性值组(
            {L1所有者范围CRUD合同版本, 节点, 截止});
        if (读取.状态 != L1所有者范围读取状态::成功
            || 读取.历史截止事实代次 != 截止) return std::nullopt;
        const std::vector<std::uint64_t>* 命中 = nullptr;
        for (const auto& 值 : 读取.属性值组) {
            if (值.属性类型节点 != 属性类型) continue;
            if (值.写入所有者 != 第一层写入端口_.所有者身份()
                || 值.所属节点 != 节点 || 值.来源节点 != 节点
                || 值.创建事实代次 == 0 || 值.创建事实代次 > 截止
                || (值.退出事实代次 && *值.退出事实代次 <= 截止)
                || 命中) return std::nullopt;
            命中 = std::get_if<std::vector<std::uint64_t>>(&值.材料);
            if (!命中) return std::nullopt;
        }
        return 命中 ? std::optional{*命中} : std::nullopt;
    }

    static std::optional<稳定编码> 查找任务写入映射(
        const L1所有者范围写入结果& 写入, std::uint32_t 本地键) noexcept {
        for (const auto& [键, 编码] : 写入.新编码映射)
            if (键.值 == 本地键 && 有效(编码)) return 编码;
        return std::nullopt;
    }

    static bool 任务轮次结算候选有效(
        const L2任务轮次结算事实& 候选) noexcept {
        if (有效(候选.身份.值) || 候选.生命周期.创建事实代次 != 0
            || 候选.生命周期.退出事实代次) return false;
        auto 规范化 = 候选;
        规范化.身份 = L2任务轮次结算身份{稳定编码{1}};
        规范化.生命周期 = {1, std::nullopt};
        return L2任务轮次结算事实完整(规范化);
    }

    std::optional<L2任务治理动作调用事实> 读取指定任务治理动作(
        L2任务治理动作调用身份 身份, std::uint64_t 截止,
        稳定编码 预期治理函数, std::uint32_t 预期规则版本) const {
        const auto 节点 = 读取任务所有者活动节点(身份.值, 截止);
        const 稳定编码 类型[] = {
            初次筹办治理定位_.治理动作族归属关系类型,
            初次筹办治理定位_.治理动作函数关系类型,
            初次筹办治理定位_.治理动作任务关系类型,
            初次筹办治理定位_.治理动作轮次关系类型,
            初次筹办治理定位_.治理动作筹办轮次关系类型,
            初次筹办治理定位_.治理动作触发前态关系类型};
        稳定编码 目标[6]{};
        for (std::size_t i = 0; i < 6; ++i) {
            const auto 关系 = 读取任务所有者活动关系组(身份.值, 类型[i], 截止);
            if (!关系 || 关系->size() != 1 || 关系->front().角色或顺序 != 1)
                return std::nullopt;
            目标[i] = 关系->front().目标节点;
        }
        const auto 材料 = 读取任务所有者唯一U64材料(身份.值,
            初次筹办治理定位_.治理动作调用材料属性类型, 截止);
        if (!节点 || !材料 || 材料->size() != 4
            || 目标[0] != 初次筹办治理定位_.治理动作调用锚点
            || 目标[1] != 预期治理函数 || (*材料)[0] == 0 || (*材料)[1] == 0
            || (*材料)[2] != L2任务治理函数合同版本
            || (*材料)[3] != 预期规则版本)
            return std::nullopt;
        L2任务治理动作调用事实 事实{身份,
            L2任务治理函数身份{目标[1]}, L2任务身份{目标[2]},
            L2任务轮次身份{目标[3]},
            L2任务筹办轮次权威记录身份{目标[4]}, L2状态身份{目标[5]},
            (*材料)[0], L2结构幂等身份{(*材料)[1]},
            static_cast<std::uint32_t>((*材料)[2]),
            static_cast<std::uint32_t>((*材料)[3]),
            {节点->创建事实代次, std::nullopt}};
        return L2任务治理动作调用事实完整(事实)
            ? std::optional{事实} : std::nullopt;
    }

    L2任务轮次结算读取结果 读取任务轮次结算_已锁定(
        const L2任务轮次结算读取请求& 请求) const noexcept {
        const auto 失败 = [&](L2结构状态 状态) {
            L2任务轮次结算读取结果 结果;
            结果.结果头 = {L2结构合同版本, 状态,
                请求.非零读取截止, std::nullopt};
            return 结果;
        };
        try {
            if (请求.合同版本 != L2任务轮次结算合同版本
                || !有效(请求.结算.值) || !有效(请求.任务.值)
                || !有效(请求.任务轮次.值) || 请求.非零读取截止 == 0)
                return 失败(L2结构状态::入口拒绝);
            const auto 节点 = 读取任务所有者活动节点(
                请求.结算.值, 请求.非零读取截止);
            if (!节点) return 失败(L2结构状态::未找到);
            const auto 材料 = 读取任务所有者唯一U64材料(
                请求.结算.值, 轮次结算定位_.结算材料属性类型,
                请求.非零读取截止);
            if (!材料 || 材料->size() != 5
                || (*材料)[0] != L2任务轮次结算合同版本)
                return 失败(L2结构状态::内部不一致);
            const auto 单一 = [&](稳定编码 类型,
                bool 必需 = true) -> std::optional<稳定编码> {
                const auto 关系 = 读取任务所有者活动关系组(
                    请求.结算.值, 类型, 请求.非零读取截止);
                if (!关系 || 关系->size() > 1 || (必需 && 关系->size() != 1))
                    return std::nullopt;
                if (关系->empty()) return 稳定编码{};
                return 关系->front().角色或顺序 == 1
                    ? std::optional{关系->front().目标节点} : std::nullopt;
            };
            const auto 族 = 单一(轮次结算定位_.结算族关系类型);
            const auto 任务 = 单一(轮次结算定位_.任务关系类型);
            const auto 轮次 = 单一(轮次结算定位_.任务轮次关系类型);
            const auto 实际 = 单一(轮次结算定位_.实际结果关系类型, false);
            const auto 验证 = 单一(轮次结算定位_.验证归因收口关系类型);
            const auto 分配 = 单一(轮次结算定位_.消费分配记录关系类型);
            const auto 冻结 = 单一(轮次结算定位_.执行冻结收口关系类型);
            const auto 授权 = 单一(轮次结算定位_.授权关系类型, false);
            const auto 授权收口 = 单一(轮次结算定位_.授权收口关系类型, false);
            const auto 停止 = 单一(轮次结算定位_.任务运行停止证据关系类型);
            const auto 治理动作 = 单一(轮次结算定位_.结算治理动作关系类型);
            if (!族 || !任务 || !轮次 || !实际 || !验证 || !分配 || !冻结
                || !授权 || !授权收口 || !停止 || !治理动作
                || *族 != 轮次结算定位_.结算锚点
                || *任务 != 请求.任务.值 || *轮次 != 请求.任务轮次.值)
                return 失败(L2结构状态::内部不一致);

            const auto 需求组 = 读取任务所有者活动关系组(
                请求.结算.值, 轮次结算定位_.需求核算关系类型,
                请求.非零读取截止);
            const auto 普通组 = 读取任务所有者活动关系组(
                请求.结算.值, 轮次结算定位_.普通价值结算关系类型,
                请求.非零读取截止);
            const auto 学习组 = 读取任务所有者活动关系组(
                请求.结算.值, 轮次结算定位_.学习能力变化关系类型,
                请求.非零读取截止);
            if (!需求组 || 需求组->empty() || !普通组 || !学习组)
                return 失败(L2结构状态::内部不一致);
            const auto 连续 = [](const auto& 关系组) noexcept {
                for (std::size_t i = 0; i < 关系组.size(); ++i)
                    if (关系组[i].角色或顺序 != static_cast<std::int64_t>(i + 1))
                        return false;
                return true;
            };
            if (!连续(*需求组) || !连续(*普通组) || !连续(*学习组))
                return 失败(L2结构状态::内部不一致);

            L2任务轮次结算事实 事实;
            事实.身份 = 请求.结算;
            事实.任务 = 请求.任务;
            事实.任务轮次 = 请求.任务轮次;
            if (有效(*实际)) 事实.实际结果 = L2任务实际结果身份{*实际};
            事实.结论 = static_cast<任务轮次结论>((*材料)[1]);
            事实.验证归因收口 = L2任务验证归因收口身份{*验证};
            事实.结果消费分配记录 = L2任务结果消费分配记录身份{*分配};
            for (const auto& 关系 : *需求组)
                事实.需求核算结果组.push_back(L2需求本轮核算身份{关系.目标节点});
            for (const auto& 关系 : *普通组)
                事实.适用后继结果组.emplace_back(L2普通价值结算身份{关系.目标节点});
            for (const auto& 关系 : *学习组)
                事实.适用后继结果组.emplace_back(L2学习能力变化身份{关系.目标节点});
            事实.执行冻结收口 = L2任务执行冻结收口身份{*冻结};
            事实.授权收口.形态 = static_cast<任务轮次授权收口形态>((*材料)[2]);
            if (有效(*授权)) 事实.授权收口.授权 = L2自我现实执行授权身份{*授权};
            if (有效(*授权收口))
                事实.授权收口.收口 = L2自我现实执行授权收口身份{*授权收口};
            事实.任务运行停止证据 = L2任务运行停止证据身份{*停止};
            事实.来源共同事实截止 = (*材料)[3];
            事实.结算规则版本 = static_cast<std::uint32_t>((*材料)[4]);
            事实.生命周期 = {节点->创建事实代次, std::nullopt};
            const auto 动作 = 读取指定任务治理动作(
                L2任务治理动作调用身份{*治理动作}, 请求.非零读取截止,
                轮次结算定位_.轮次收束治理函数定义,
                L2任务轮次结算规则版本);
            if (!L2任务轮次结算事实完整(事实) || !动作
                || 动作->任务 != 请求.任务 || 动作->任务轮次 != 请求.任务轮次)
                return 失败(L2结构状态::内部不一致);
            L2任务轮次结算读取结果 结果;
            结果.结果头 = {L2结构合同版本, L2结构状态::已读取,
                请求.非零读取截止, std::nullopt};
            结果.结算 = std::move(事实);
            结果.轮次收束治理动作 = std::move(动作);
            结果.本次正式读回截止 = 请求.非零读取截止;
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2结构状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致);
        }
    }

    L2任务轮次结算结果 建立或读取任务轮次结算_已锁定(
        const L2任务轮次结算请求& 请求) noexcept {
        const auto 失败 = [&](L2结构状态 状态, std::uint64_t 截止 = 0) {
            L2任务轮次结算结果 结果;
            结果.结果头 = {L2结构合同版本, 状态, 截止, std::nullopt};
            return 结果;
        };
        try {
            if (请求.请求头.合同版本 != L2结构合同版本
                || 请求.请求头.期望事实代次 == 0
                || !L2结构幂等身份有效(请求.幂等身份)
                || 请求.幂等身份.值
                    == L2任务结构内部::任务轮次结算登记幂等身份.值
                || !有效(请求.触发前态.值)
                || !任务轮次结算候选有效(请求.候选事实))
                return 失败(L2结构状态::入口拒绝);
            const auto& 事实 = 请求.候选事实;
            if (事实.需求核算结果组.size() > 100000U
                || 事实.适用后继结果组.size() > 100000U)
                return 失败(L2结构状态::数量预算不足);
            const auto G0 = 请求.请求头.期望事实代次;
            const auto 观察 = L2任务结构内部::读取中性当前事实代次(L1_);
            if (观察 != G0) return 失败(L2结构状态::事实代次漂移, 观察);
            const auto P关系读取 = L1_.读取所有者范围历史关系组({
                L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::目标,
                事实.任务.值, 筹办轮次定位_.关系类型[1], G0});
            if (P关系读取.状态 != L1所有者范围读取状态::成功
                || P关系读取.历史截止事实代次 != G0
                || P关系读取.关系组.size() != 1)
                return 失败(L2结构状态::事实代次漂移, G0);
            const auto& P任务关系 = P关系读取.关系组.front();
            if (P任务关系.写入所有者 != 第一层写入端口_.所有者身份()
                || P任务关系.目标节点 != 事实.任务.值 || P任务关系.退出事实代次)
                return 失败(L2结构状态::事实代次漂移, G0);
            const auto P身份 = L2任务筹办轮次权威记录身份{P任务关系.源节点};
            auto P = 读取自我继续筹办权威事实(P身份, G0);
            if (!P) P = L2任务结构内部::读取筹办轮次权威历史投影(
                L1_, 筹办轮次定位_, P身份, G0, 观察);
            if (!P || P->任务 != 事实.任务 || !P->任务轮次
                || *P->任务轮次 != 事实.任务轮次)
                return 失败(L2结构状态::引用冲突, G0);
            L1所有者范围写集请求 写集;
            写集.合同版本 = L1所有者范围CRUD合同版本;
            写集.期望事实代次 = 请求.请求头.期望事实代次;
            写集.写入幂等身份 = {请求.幂等身份.值};
            constexpr std::uint32_t 节点键 = 1, 材料值键 = 2;
            constexpr std::uint32_t 动作键 = 3, 动作材料值键 = 4;
            写集.节点 = {{{节点键}, 节点种类::普通, std::nullopt},
                {{动作键}, 节点种类::普通, std::nullopt}};
            写集.值.push_back({{材料值键},
                L1所有者范围写集本地键{节点键},
                轮次结算定位_.结算材料属性类型,
                std::vector<std::uint64_t>{L2任务轮次结算合同版本,
                    static_cast<std::uint64_t>(事实.结论),
                    static_cast<std::uint64_t>(事实.授权收口.形态),
                    事实.来源共同事实截止, 事实.结算规则版本},
                L1所有者范围写集本地键{节点键}});
            写集.值.push_back({{动作材料值键},
                L1所有者范围写集本地键{动作键},
                初次筹办治理定位_.治理动作调用材料属性类型,
                std::vector<std::uint64_t>{事实.来源共同事实截止,
                    请求.幂等身份.值, L2任务治理函数合同版本,
                    L2任务轮次结算规则版本},
                L1所有者范围写集本地键{动作键}});
            写集.属性槽变更 = {
                {L1所有者范围写集本地键{节点键},
                    轮次结算定位_.结算材料属性类型, {材料值键}},
                {L1所有者范围写集本地键{动作键},
                    初次筹办治理定位_.治理动作调用材料属性类型,
                    {动作材料值键}}};
            std::uint32_t 关系键 = 10;
            const auto 关系 = [&](std::uint32_t 源键,
                L1所有者范围事实引用 目标, L1所有者范围事实引用 类型,
                std::int64_t 角色 = 1) {
                写集.关系.push_back({{关系键++},
                    L1所有者范围写集本地键{源键}, 目标, 类型, 角色});
            };
            关系(节点键, 轮次结算定位_.结算锚点, 轮次结算定位_.结算族关系类型);
            关系(节点键, 事实.任务.值, 轮次结算定位_.任务关系类型);
            关系(节点键, 事实.任务轮次.值, 轮次结算定位_.任务轮次关系类型);
            if (事实.实际结果)
                关系(节点键, 事实.实际结果->值, 轮次结算定位_.实际结果关系类型);
            关系(节点键, 事实.验证归因收口.值,
                轮次结算定位_.验证归因收口关系类型);
            关系(节点键, 事实.结果消费分配记录.值,
                轮次结算定位_.消费分配记录关系类型);
            for (std::size_t i = 0; i < 事实.需求核算结果组.size(); ++i)
                关系(节点键, 事实.需求核算结果组[i].值,
                    轮次结算定位_.需求核算关系类型,
                    static_cast<std::int64_t>(i + 1));
            std::int64_t 普通序号 = 0, 学习序号 = 0;
            for (const auto& 后继 : 事实.适用后继结果组) {
                if (const auto* 普通 = std::get_if<L2普通价值结算身份>(&后继))
                    关系(节点键, 普通->值, 轮次结算定位_.普通价值结算关系类型,
                        ++普通序号);
                else 关系(节点键, std::get<L2学习能力变化身份>(后继).值,
                    轮次结算定位_.学习能力变化关系类型, ++学习序号);
            }
            关系(节点键, 事实.执行冻结收口.值,
                轮次结算定位_.执行冻结收口关系类型);
            if (事实.授权收口.授权)
                关系(节点键, 事实.授权收口.授权->值,
                    轮次结算定位_.授权关系类型);
            if (事实.授权收口.收口)
                关系(节点键, 事实.授权收口.收口->值,
                    轮次结算定位_.授权收口关系类型);
            关系(节点键, 事实.任务运行停止证据.值,
                轮次结算定位_.任务运行停止证据关系类型);
            关系(动作键, 初次筹办治理定位_.治理动作调用锚点,
                初次筹办治理定位_.治理动作族归属关系类型);
            关系(动作键, 轮次结算定位_.轮次收束治理函数定义,
                初次筹办治理定位_.治理动作函数关系类型);
            关系(动作键, 事实.任务.值,
                初次筹办治理定位_.治理动作任务关系类型);
            关系(动作键, 事实.任务轮次.值,
                初次筹办治理定位_.治理动作轮次关系类型);
            关系(动作键, P任务关系.源节点,
                初次筹办治理定位_.治理动作筹办轮次关系类型);
            关系(动作键, 请求.触发前态.值,
                初次筹办治理定位_.治理动作触发前态关系类型);
            关系(节点键, L1所有者范围写集本地键{动作键},
                轮次结算定位_.结算治理动作关系类型);
            const auto 写入 = 第一层写入端口_.提交所有者范围中性写集(写集);
            const auto 状态 = L2任务结构内部::映射任务写入状态(写入.状态);
            if (状态 != L2结构状态::已提交 && 状态 != L2结构状态::精确重复)
                return 失败(状态, 写入.事实代次);
            const auto 身份 = 查找任务写入映射(写入, 节点键);
            const auto 动作身份 = 查找任务写入映射(写入, 动作键);
            if (身份 && 动作身份) {
                const auto 读回 = 读取任务轮次结算_已锁定({
                    L2任务轮次结算合同版本, L2任务轮次结算身份{*身份},
                    事实.任务, 事实.任务轮次, 写入.事实代次});
                auto 期望 = 事实;
                期望.身份 = L2任务轮次结算身份{*身份};
                期望.生命周期 = {写入.事实代次, std::nullopt};
                L2任务治理动作调用事实 期望动作{
                    L2任务治理动作调用身份{*动作身份},
                    L2任务治理函数身份{轮次结算定位_.轮次收束治理函数定义},
                    事实.任务, 事实.任务轮次,
                    L2任务筹办轮次权威记录身份{P任务关系.源节点},
                    请求.触发前态, 事实.来源共同事实截止,
                    请求.幂等身份, L2任务治理函数合同版本,
                    L2任务轮次结算规则版本,
                    {写入.事实代次, std::nullopt}};
                if (读回.成功() && 读回.结算 && 读回.轮次收束治理动作
                    && *读回.结算 == 期望
                    && *读回.轮次收束治理动作 == 期望动作) {
                    L2任务轮次结算结果 结果;
                    结果.结果头 = {L2结构合同版本, 状态, 写入.事实代次,
                        写入.事实代次};
                    结果.结算 = std::move(读回.结算);
                    结果.轮次收束治理动作 = std::move(读回.轮次收束治理动作);
                    结果.本次正式读回截止 = 读回.本次正式读回截止;
                    return 结果;
                }
            }
            L2任务轮次结算结果 结果;
            结果.结果头 = {L2结构合同版本, L2结构状态::内部不一致,
                写入.事实代次, std::nullopt};
            结果.提交见证 = L2任务轮次结算提交见证{
                第一层写入端口_.所有者身份(), 请求.幂等身份,
                写入.事实代次, 1};
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2结构状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致);
        }
    }

    std::optional<L2任务后继决议引用事实> 读取任务后继决议引用事实(
        稳定编码 身份, std::uint64_t 截止) const {
        const auto 节点 = 读取任务所有者活动节点(身份, 截止);
        const auto 材料 = 读取任务所有者唯一U64材料(
            身份, 轮次结算定位_.决议引用材料属性类型, 截止);
        if (!节点 || !材料 || 材料->size() != 4
            || (*材料)[0] != L2任务后继机械合同版本
            || (*材料)[2] == 0 || (*材料)[3] != L2任务后继机械规则版本)
            return std::nullopt;
        const auto 取 = [&](稳定编码 类型) -> std::optional<稳定编码> {
            const auto 关系 = 读取任务所有者活动关系组(身份, 类型, 截止);
            if (!关系 || 关系->size() != 1 || 关系->front().角色或顺序 != 1)
                return std::nullopt;
            return 关系->front().目标节点;
        };
        const auto 族 = 取(轮次结算定位_.决议引用族关系类型);
        const auto 任务 = 取(轮次结算定位_.决议引用任务关系类型);
        const auto 轮次 = 取(轮次结算定位_.决议引用轮次关系类型);
        const auto 决议 = 取(轮次结算定位_.决议引用自我决议关系类型);
        if (!族 || !任务 || !轮次 || !决议
            || *族 != 轮次结算定位_.决议引用锚点) return std::nullopt;
        L2任务后继决议引用事实 事实{身份, L2任务身份{*任务},
            L2任务轮次身份{*轮次}, *决议,
            static_cast<自我任务后继指令>((*材料)[1]),
            {节点->创建事实代次, std::nullopt}};
        return L2任务后继决议引用事实完整(事实)
            ? std::optional{事实} : std::nullopt;
    }

    L2任务生命周期收口读取结果 读取任务生命周期收口_已锁定(
        const L2任务生命周期收口读取请求& 请求) const noexcept {
        const auto 失败 = [&](L2任务生命周期收口状态 状态) {
            L2任务生命周期收口读取结果 结果;
            结果.状态 = 状态;
            return 结果;
        };
        try {
            if (请求.合同版本 != L2任务后继机械合同版本
                || !有效(请求.任务.值) || !有效(请求.已收束任务轮次.值)
                || !有效(请求.决议引用) || !有效(请求.生命周期收口)
                || 请求.非零读取截止 == 0)
                return 失败(L2任务生命周期收口状态::入口拒绝);
            const auto 决议引用 = 读取任务后继决议引用事实(
                请求.决议引用, 请求.非零读取截止);
            const auto 节点 = 读取任务所有者活动节点(
                请求.生命周期收口, 请求.非零读取截止);
            const auto 材料 = 读取任务所有者唯一U64材料(
                请求.生命周期收口, 轮次结算定位_.生命周期收口材料属性类型,
                请求.非零读取截止);
            if (!决议引用 || !节点 || !材料 || 材料->size() != 4
                || (*材料)[0] != L2任务后继机械合同版本)
                return 失败(L2任务生命周期收口状态::未找到);
            const auto 取 = [&](稳定编码 类型) -> std::optional<稳定编码> {
                const auto 关系 = 读取任务所有者活动关系组(
                    请求.生命周期收口, 类型, 请求.非零读取截止);
                if (!关系 || 关系->size() != 1 || 关系->front().角色或顺序 != 1)
                    return std::nullopt;
                return 关系->front().目标节点;
            };
            const auto 族 = 取(轮次结算定位_.生命周期收口族关系类型);
            const auto 任务 = 取(轮次结算定位_.生命周期收口任务关系类型);
            const auto 轮次 = 取(轮次结算定位_.生命周期收口轮次关系类型);
            const auto 结算 = 取(轮次结算定位_.生命周期收口结算关系类型);
            const auto 决议 = 取(轮次结算定位_.生命周期收口决议关系类型);
            const auto 停止 = 取(轮次结算定位_.生命周期收口停止证据关系类型);
            if (!族 || !任务 || !轮次 || !结算 || !决议 || !停止
                || *族 != 轮次结算定位_.生命周期收口锚点
                || *任务 != 请求.任务.值 || *轮次 != 请求.已收束任务轮次.值
                || *决议 != 决议引用->自我后继决议
                || 决议引用->任务 != 请求.任务
                || 决议引用->已收束任务轮次 != 请求.已收束任务轮次)
                return 失败(L2任务生命周期收口状态::内部不一致);
            L2任务生命周期收口事实 收口{请求.生命周期收口,
                static_cast<L2任务生命周期收口种类>((*材料)[1]),
                请求.任务, 请求.已收束任务轮次,
                L2任务轮次结算身份{*结算}, *决议,
                L2任务运行停止证据身份{*停止}, (*材料)[2],
                static_cast<std::uint32_t>((*材料)[3]),
                {节点->创建事实代次, std::nullopt}};
            if (!L2任务生命周期收口事实完整(收口))
                return 失败(L2任务生命周期收口状态::内部不一致);
            L2任务生命周期收口读取结果 结果;
            结果.状态 = L2任务生命周期收口状态::已读取;
            结果.决议引用 = *决议引用;
            结果.生命周期收口 = std::move(收口);
            结果.本次正式读回截止 = 请求.非零读取截止;
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(L2任务生命周期收口状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2任务生命周期收口状态::资源失败);
        } catch (...) {
            return 失败(L2任务生命周期收口状态::内部不一致);
        }
    }

    static L2任务生命周期收口状态 映射生命周期收口写入状态(
        L1所有者范围写入状态 状态) noexcept {
        switch (状态) {
        case L1所有者范围写入状态::成功:
            return L2任务生命周期收口状态::已形成;
        case L1所有者范围写入状态::精确重复:
            return L2任务生命周期收口状态::精确重复;
        case L1所有者范围写入状态::入口拒绝:
            return L2任务生命周期收口状态::入口拒绝;
        case L1所有者范围写入状态::许可拒绝:
            return L2任务生命周期收口状态::许可拒绝;
        case L1所有者范围写入状态::事实代次漂移:
            return L2任务生命周期收口状态::当前性漂移;
        case L1所有者范围写入状态::幂等冲突:
            return L2任务生命周期收口状态::幂等冲突;
        case L1所有者范围写入状态::引用冲突:
            return L2任务生命周期收口状态::引用冲突;
        case L1所有者范围写入状态::资源失败:
            return L2任务生命周期收口状态::资源失败;
        default:
            return L2任务生命周期收口状态::内部不一致;
        }
    }

    L2任务生命周期收口写入结果 写入或读取任务生命周期收口_已锁定(
        const L2任务生命周期收口写入请求& 请求) noexcept {
        const auto 失败 = [&](L2任务生命周期收口状态 状态) {
            L2任务生命周期收口写入结果 结果;
            结果.状态 = 状态;
            return 结果;
        };
        try {
            const bool 指令形状 =
                (请求.指令 == 自我任务后继指令::结束当前任务
                    && 请求.种类 == L2任务生命周期收口种类::正常结束)
                || (请求.指令 == 自我任务后继指令::取消任务
                    && 请求.种类 == L2任务生命周期收口种类::取消收口);
            if (请求.请求头.合同版本 != L2结构合同版本
                || 请求.请求头.期望事实代次 == 0
                || !L2结构幂等身份有效(请求.幂等身份)
                || !有效(请求.自我后继决议) || !有效(请求.任务.值)
                || !有效(请求.已收束任务轮次.值)
                || !有效(请求.轮次结算.值)
                || !有效(请求.任务运行停止证据.值)
                || 请求.发生时间 == 0
                || 请求.规则版本 != L2任务后继机械规则版本
                || !指令形状)
                return 失败(L2任务生命周期收口状态::入口拒绝);
            const auto 结算 = 读取任务轮次结算_已锁定({
                L2任务轮次结算合同版本, 请求.轮次结算, 请求.任务,
                请求.已收束任务轮次, 请求.请求头.期望事实代次});
            if (!结算.成功())
                return 失败(L2任务生命周期收口状态::当前性漂移);
            L1所有者范围写集请求 写集;
            写集.合同版本 = L1所有者范围CRUD合同版本;
            写集.期望事实代次 = 请求.请求头.期望事实代次;
            写集.写入幂等身份 = {请求.幂等身份.值};
            constexpr std::uint32_t 决议节点键 = 1, 收口节点键 = 2;
            constexpr std::uint32_t 决议值键 = 3, 收口值键 = 4;
            写集.节点 = {{{决议节点键}, 节点种类::普通, std::nullopt},
                {{收口节点键}, 节点种类::普通, std::nullopt}};
            写集.值 = {
                {{决议值键}, L1所有者范围写集本地键{决议节点键},
                    轮次结算定位_.决议引用材料属性类型,
                    std::vector<std::uint64_t>{L2任务后继机械合同版本,
                        static_cast<std::uint64_t>(请求.指令), 请求.发生时间,
                        请求.规则版本},
                    L1所有者范围写集本地键{决议节点键}},
                {{收口值键}, L1所有者范围写集本地键{收口节点键},
                    轮次结算定位_.生命周期收口材料属性类型,
                    std::vector<std::uint64_t>{L2任务后继机械合同版本,
                        static_cast<std::uint64_t>(请求.种类), 请求.发生时间,
                        请求.规则版本},
                    L1所有者范围写集本地键{收口节点键}}};
            写集.属性槽变更 = {
                {L1所有者范围写集本地键{决议节点键},
                    轮次结算定位_.决议引用材料属性类型, {决议值键}},
                {L1所有者范围写集本地键{收口节点键},
                    轮次结算定位_.生命周期收口材料属性类型, {收口值键}}};
            std::uint32_t 关系键 = 10;
            const auto 关系 = [&](std::uint32_t 源键, 稳定编码 目标, 稳定编码 类型) {
                写集.关系.push_back({{关系键++},
                    L1所有者范围写集本地键{源键}, 目标, 类型, 1});
            };
            关系(决议节点键, 轮次结算定位_.决议引用锚点,
                轮次结算定位_.决议引用族关系类型);
            关系(决议节点键, 请求.任务.值, 轮次结算定位_.决议引用任务关系类型);
            关系(决议节点键, 请求.已收束任务轮次.值,
                轮次结算定位_.决议引用轮次关系类型);
            关系(决议节点键, 请求.自我后继决议,
                轮次结算定位_.决议引用自我决议关系类型);
            关系(收口节点键, 轮次结算定位_.生命周期收口锚点,
                轮次结算定位_.生命周期收口族关系类型);
            关系(收口节点键, 请求.任务.值,
                轮次结算定位_.生命周期收口任务关系类型);
            关系(收口节点键, 请求.已收束任务轮次.值,
                轮次结算定位_.生命周期收口轮次关系类型);
            关系(收口节点键, 请求.轮次结算.值,
                轮次结算定位_.生命周期收口结算关系类型);
            关系(收口节点键, 请求.自我后继决议,
                轮次结算定位_.生命周期收口决议关系类型);
            关系(收口节点键, 请求.任务运行停止证据.值,
                轮次结算定位_.生命周期收口停止证据关系类型);
            const auto 写入 = 第一层写入端口_.提交所有者范围中性写集(写集);
            const auto 状态 = 映射生命周期收口写入状态(写入.状态);
            if (状态 != L2任务生命周期收口状态::已形成
                && 状态 != L2任务生命周期收口状态::精确重复)
                return 失败(状态);
            const auto 决议身份 = 查找任务写入映射(写入, 决议节点键);
            const auto 收口身份 = 查找任务写入映射(写入, 收口节点键);
            if (决议身份 && 收口身份) {
                const auto 读回 = 读取任务生命周期收口_已锁定({
                    L2任务后继机械合同版本, 请求.任务, 请求.已收束任务轮次,
                    *决议身份, *收口身份, 写入.事实代次});
                if (读回.成功()) {
                    L2任务生命周期收口写入结果 结果;
                    结果.状态 = 状态;
                    结果.决议引用 = std::move(读回.决议引用);
                    结果.生命周期收口 = std::move(读回.生命周期收口);
                    结果.本次正式读回截止 = 读回.本次正式读回截止;
                    return 结果;
                }
            }
            L2任务生命周期收口写入结果 结果;
            结果.状态 = L2任务生命周期收口状态::已可能发布;
            结果.提交见证 = L2任务后继提交见证{
                第一层写入端口_.所有者身份(), 请求.幂等身份,
                写入.事实代次, 1};
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(L2任务生命周期收口状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2任务生命周期收口状态::资源失败);
        } catch (...) {
            return 失败(L2任务生命周期收口状态::内部不一致);
        }
    }

    std::optional<L2任务轮次事实> 读取任务轮次机械事实(
        L2任务轮次身份 身份, L2任务身份 任务, std::uint64_t 截止) const {
        const auto 节点 = 读取任务所有者活动节点(身份.值, 截止);
        const auto 族 = 读取任务所有者活动关系组(身份.值,
            初次筹办治理定位_.任务轮次族归属关系类型, 截止);
        const auto 任务关系 = 读取任务所有者活动关系组(身份.值,
            初次筹办治理定位_.任务轮次任务关系类型, 截止);
        const auto 材料 = 读取任务所有者唯一U64材料(身份.值,
            初次筹办治理定位_.任务轮次序号属性类型, 截止);
        if (!节点 || !族 || 族->size() != 1
            || 族->front().目标节点 != 初次筹办治理定位_.任务轮次锚点
            || 族->front().角色或顺序 != 1
            || !任务关系 || 任务关系->size() != 1
            || 任务关系->front().目标节点 != 任务.值
            || 任务关系->front().角色或顺序 != 1
            || !材料 || 材料->size() != 1 || 材料->front() == 0)
            return std::nullopt;
        L2任务轮次事实 事实{身份, 任务, 材料->front(),
            {节点->创建事实代次, std::nullopt}};
        return L2任务轮次事实完整(事实) ? std::optional{事实} : std::nullopt;
    }

    std::optional<L2任务治理动作调用事实> 读取自我继续治理动作(
        L2任务治理动作调用身份 身份, std::uint64_t 截止) const {
        return 读取指定任务治理动作(身份, 截止,
            轮次结算定位_.自我继续治理函数定义,
            L2任务后继机械规则版本);
    }

    std::optional<L2任务筹办轮次权威记录事实> 读取自我继续筹办权威事实(
        L2任务筹办轮次权威记录身份 身份, std::uint64_t 截止) const {
        const auto 节点 = 读取任务所有者活动节点(身份.值, 截止);
        const auto 材料 = 读取任务所有者唯一U64材料(
            身份.值, 筹办轮次定位_.属性类型[0], 截止);
        const 稳定编码 类型[] = {
            筹办轮次定位_.关系类型[0],
            筹办轮次定位_.关系类型[1],
            筹办轮次定位_.关系类型[2],
            筹办轮次定位_.关系类型[3],
            筹办轮次定位_.关系类型[4],
            轮次结算定位_.筹办权威任务轮次关系类型,
            轮次结算定位_.筹办权威阶段特征实例关系类型,
            轮次结算定位_.筹办权威初始阶段状态关系类型};
        稳定编码 目标[8]{};
        for (std::size_t i = 0; i < 8; ++i) {
            const auto 关系 = 读取任务所有者活动关系组(身份.值, 类型[i], 截止);
            if (!关系 || 关系->size() != 1 || 关系->front().角色或顺序 != 1)
                return std::nullopt;
            目标[i] = 关系->front().目标节点;
        }
        if (!节点 || !材料 || 材料->size() != 1
            || 目标[0] != 筹办轮次定位_.权威记录锚点
            || 材料->front() != L2任务筹办轮次权威规则版本)
            return std::nullopt;
        L2任务筹办轮次权威记录事实 事实;
        事实.身份 = 身份;
        事实.任务 = L2任务身份{目标[1]};
        事实.任务虚拟存在 = L2任务虚拟存在身份{目标[2]};
        事实.需求列表项 = L2需求列表项身份{目标[3]};
        事实.首次准备记录 = L2任务初次筹办准备记录身份{目标[4]};
        事实.权威规则版本 = static_cast<std::uint32_t>(材料->front());
        事实.建立事实代次 = 节点->创建事实代次;
        事实.生命周期 = {节点->创建事实代次, std::nullopt};
        事实.任务轮次 = L2任务轮次身份{目标[5]};
        事实.正式存在 = L2存在身份{目标[2]};
        事实.阶段特征实例 = L2特征实例身份{目标[6]};
        事实.待初次筹办首态 = L2状态身份{目标[7]};
        return L2任务初次筹办P1事实完整(事实)
            ? std::optional{事实} : std::nullopt;
    }

    L2任务后继继续读取结果 读取任务后继继续事实_已锁定(
        const L2任务后继继续读取请求& 请求) const noexcept {
        const auto 失败 = [&](L2任务后继继续状态 状态) {
            L2任务后继继续读取结果 结果;
            结果.状态 = 状态;
            return 结果;
        };
        try {
            if (请求.合同版本 != L2任务后继机械合同版本
                || !有效(请求.任务.值) || !有效(请求.已收束任务轮次.值)
                || !有效(请求.新任务轮次.值) || !有效(请求.新筹办轮次.值)
                || !有效(请求.继续治理动作.值) || !有效(请求.决议引用)
                || 请求.非零读取截止 == 0)
                return 失败(L2任务后继继续状态::入口拒绝);
            const auto 新轮次 = 读取任务轮次机械事实(
                请求.新任务轮次, 请求.任务, 请求.非零读取截止);
            const auto 新筹办 = 读取自我继续筹办权威事实(
                请求.新筹办轮次, 请求.非零读取截止);
            const auto 动作 = 读取自我继续治理动作(
                请求.继续治理动作, 请求.非零读取截止);
            const auto 决议 = 读取任务后继决议引用事实(
                请求.决议引用, 请求.非零读取截止);
            if (!新轮次 || !新筹办 || !动作 || !决议)
                return 失败(L2任务后继继续状态::未找到);
            if (新轮次->任务 != 请求.任务
                || 新筹办->任务 != 请求.任务
                || !新筹办->任务轮次 || *新筹办->任务轮次 != 请求.新任务轮次
                || 动作->任务 != 请求.任务
                || 动作->任务轮次 != 请求.新任务轮次
                || 动作->筹办轮次 != 请求.新筹办轮次
                || 决议->任务 != 请求.任务
                || 决议->已收束任务轮次 != 请求.已收束任务轮次
                || 决议->指令 != 自我任务后继指令::继续当前任务)
                return 失败(L2任务后继继续状态::内部不一致);
            L2任务后继继续读取结果 结果;
            结果.状态 = L2任务后继继续状态::已读取;
            结果.新任务轮次 = *新轮次;
            结果.新筹办轮次 = *新筹办;
            结果.继续治理动作 = *动作;
            结果.决议引用 = *决议;
            结果.本次正式读回截止 = 请求.非零读取截止;
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(L2任务后继继续状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2任务后继继续状态::资源失败);
        } catch (...) {
            return 失败(L2任务后继继续状态::内部不一致);
        }
    }

    static L2任务后继继续状态 映射任务后继继续写入状态(
        L1所有者范围写入状态 状态) noexcept {
        switch (状态) {
        case L1所有者范围写入状态::成功:
            return L2任务后继继续状态::已形成;
        case L1所有者范围写入状态::精确重复:
            return L2任务后继继续状态::精确重复;
        case L1所有者范围写入状态::入口拒绝:
            return L2任务后继继续状态::入口拒绝;
        case L1所有者范围写入状态::许可拒绝:
            return L2任务后继继续状态::许可拒绝;
        case L1所有者范围写入状态::事实代次漂移:
            return L2任务后继继续状态::当前性漂移;
        case L1所有者范围写入状态::幂等冲突:
            return L2任务后继继续状态::幂等冲突;
        case L1所有者范围写入状态::引用冲突:
            return L2任务后继继续状态::引用冲突;
        case L1所有者范围写入状态::资源失败:
            return L2任务后继继续状态::资源失败;
        default:
            return L2任务后继继续状态::内部不一致;
        }
    }

    L2任务后继继续写入结果 写入或读取任务后继继续事实_已锁定(
        const L2任务后继继续写入请求& 请求) noexcept {
        const auto 失败 = [&](L2任务后继继续状态 状态) {
            L2任务后继继续写入结果 结果;
            结果.状态 = 状态;
            return 结果;
        };
        try {
            if (请求.请求头.合同版本 != L2结构合同版本
                || 请求.请求头.期望事实代次 == 0
                || !L2结构幂等身份有效(请求.幂等身份)
                || 请求.指令 != 自我任务后继指令::继续当前任务
                || !有效(请求.自我后继决议) || !有效(请求.任务.值)
                || !有效(请求.已收束任务轮次.值)
                || !有效(请求.轮次结算.值) || !有效(请求.触发前态.值)
                || 请求.新任务轮次序号 < 2 || 请求.新筹办轮次序号 != 1
                || 请求.来源共同事实截止 == 0 || 请求.发生时间 == 0
                || 请求.规则版本 != L2任务后继机械规则版本)
                return 失败(L2任务后继继续状态::入口拒绝);
            const auto G0 = 请求.请求头.期望事实代次;
            const auto 结算 = 读取任务轮次结算_已锁定({
                L2任务轮次结算合同版本, 请求.轮次结算, 请求.任务,
                请求.已收束任务轮次, G0});
            if (!结算.成功()) return 失败(L2任务后继继续状态::当前性漂移);
            const auto 当前轮次组 = 读取任务所有者活动关系组(请求.任务.值,
                初次筹办治理定位_.任务当前轮次关系类型, G0);
            if (!当前轮次组 || 当前轮次组->size() != 1
                || 当前轮次组->front().角色或顺序 != 1
                || 当前轮次组->front().目标节点 != 请求.已收束任务轮次.值)
                return 失败(L2任务后继继续状态::当前性漂移);
            const auto 旧轮次 = 读取任务轮次机械事实(
                请求.已收束任务轮次, 请求.任务, G0);
            if (!旧轮次 || 旧轮次->轮次序号 == (std::numeric_limits<std::uint64_t>::max)()
                || 请求.新任务轮次序号 != 旧轮次->轮次序号 + 1)
                return 失败(L2任务后继继续状态::引用冲突);
            const auto P关系读取 = L1_.读取所有者范围历史关系组({
                L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::目标,
                请求.任务.值, 筹办轮次定位_.关系类型[1], G0});
            if (P关系读取.状态 != L1所有者范围读取状态::成功
                || P关系读取.历史截止事实代次 != G0
                || P关系读取.关系组.size() != 1)
                return 失败(L2任务后继继续状态::当前性漂移);
            const auto& 旧P任务关系 = P关系读取.关系组.front();
            if (旧P任务关系.写入所有者 != 第一层写入端口_.所有者身份()
                || 旧P任务关系.目标节点 != 请求.任务.值
                || 旧P任务关系.退出事实代次)
                return 失败(L2任务后继继续状态::当前性漂移);
            const auto 观察 = L2任务结构内部::读取中性当前事实代次(L1_);
            if (观察 != G0) return 失败(L2任务后继继续状态::当前性漂移);
            const auto 旧P身份 =
                L2任务筹办轮次权威记录身份{旧P任务关系.源节点};
            auto 旧P = 读取自我继续筹办权威事实(旧P身份, G0);
            if (!旧P) 旧P = L2任务结构内部::读取筹办轮次权威历史投影(
                L1_, 筹办轮次定位_, 旧P身份, G0, 观察);
            if (!旧P || 旧P->任务 != 请求.任务 || !旧P->任务轮次
                || *旧P->任务轮次 != 请求.已收束任务轮次
                || !旧P->正式存在 || !旧P->阶段特征实例 || !旧P->待初次筹办首态)
                return 失败(L2任务后继继续状态::当前性漂移);

            L1所有者范围写集请求 写集;
            写集.合同版本 = L1所有者范围CRUD合同版本;
            写集.期望事实代次 = G0;
            写集.写入幂等身份 = {请求.幂等身份.值};
            constexpr std::uint32_t R键 = 1, P键 = 2, A键 = 3, D键 = 4;
            constexpr std::uint32_t R值键 = 5, P值键 = 6, A值键 = 7, D值键 = 8;
            写集.节点 = {{{R键}, 节点种类::普通, std::nullopt},
                {{P键}, 节点种类::普通, std::nullopt},
                {{A键}, 节点种类::普通, std::nullopt},
                {{D键}, 节点种类::普通, std::nullopt}};
            写集.值 = {
                {{R值键}, L1所有者范围写集本地键{R键},
                    初次筹办治理定位_.任务轮次序号属性类型,
                    std::vector<std::uint64_t>{请求.新任务轮次序号},
                    L1所有者范围写集本地键{R键}},
                {{P值键}, L1所有者范围写集本地键{P键},
                    筹办轮次定位_.属性类型[0],
                    std::vector<std::uint64_t>{L2任务筹办轮次权威规则版本},
                    L1所有者范围写集本地键{P键}},
                {{A值键}, L1所有者范围写集本地键{A键},
                    初次筹办治理定位_.治理动作调用材料属性类型,
                    std::vector<std::uint64_t>{请求.来源共同事实截止,
                        请求.幂等身份.值, L2任务治理函数合同版本,
                        请求.规则版本}, L1所有者范围写集本地键{A键}},
                {{D值键}, L1所有者范围写集本地键{D键},
                    轮次结算定位_.决议引用材料属性类型,
                    std::vector<std::uint64_t>{L2任务后继机械合同版本,
                        static_cast<std::uint64_t>(请求.指令), 请求.发生时间,
                        请求.规则版本}, L1所有者范围写集本地键{D键}}};
            写集.属性槽变更 = {
                {L1所有者范围写集本地键{R键},
                    初次筹办治理定位_.任务轮次序号属性类型, {R值键}},
                {L1所有者范围写集本地键{P键},
                    筹办轮次定位_.属性类型[0], {P值键}},
                {L1所有者范围写集本地键{A键},
                    初次筹办治理定位_.治理动作调用材料属性类型, {A值键}},
                {L1所有者范围写集本地键{D键},
                    轮次结算定位_.决议引用材料属性类型, {D值键}}};
            std::uint32_t 关系键 = 20;
            const auto 关系 = [&](std::uint32_t 源键, L1所有者范围事实引用 目标,
                L1所有者范围事实引用 类型) {
                写集.关系.push_back({{关系键++},
                    L1所有者范围写集本地键{源键}, 目标, 类型, 1});
            };
            关系(R键, 初次筹办治理定位_.任务轮次锚点,
                初次筹办治理定位_.任务轮次族归属关系类型);
            关系(R键, 请求.任务.值, 初次筹办治理定位_.任务轮次任务关系类型);
            关系(P键, 筹办轮次定位_.权威记录锚点, 筹办轮次定位_.关系类型[0]);
            关系(P键, 请求.任务.值, 筹办轮次定位_.关系类型[1]);
            关系(P键, 旧P->任务虚拟存在.值, 筹办轮次定位_.关系类型[2]);
            关系(P键, 旧P->需求列表项.值, 筹办轮次定位_.关系类型[3]);
            关系(P键, 旧P->首次准备记录.值, 筹办轮次定位_.关系类型[4]);
            关系(P键, L1所有者范围写集本地键{R键},
                轮次结算定位_.筹办权威任务轮次关系类型);
            关系(P键, 旧P->阶段特征实例->值,
                轮次结算定位_.筹办权威阶段特征实例关系类型);
            关系(P键, 旧P->待初次筹办首态->值,
                轮次结算定位_.筹办权威初始阶段状态关系类型);
            关系(A键, 初次筹办治理定位_.治理动作调用锚点,
                初次筹办治理定位_.治理动作族归属关系类型);
            关系(A键, 轮次结算定位_.自我继续治理函数定义,
                初次筹办治理定位_.治理动作函数关系类型);
            关系(A键, 请求.任务.值, 初次筹办治理定位_.治理动作任务关系类型);
            关系(A键, L1所有者范围写集本地键{R键},
                初次筹办治理定位_.治理动作轮次关系类型);
            关系(A键, L1所有者范围写集本地键{P键},
                初次筹办治理定位_.治理动作筹办轮次关系类型);
            关系(A键, 请求.触发前态.值,
                初次筹办治理定位_.治理动作触发前态关系类型);
            关系(D键, 轮次结算定位_.决议引用锚点,
                轮次结算定位_.决议引用族关系类型);
            关系(D键, 请求.任务.值, 轮次结算定位_.决议引用任务关系类型);
            关系(D键, 请求.已收束任务轮次.值,
                轮次结算定位_.决议引用轮次关系类型);
            关系(D键, 请求.自我后继决议,
                轮次结算定位_.决议引用自我决议关系类型);
            写集.关系.push_back({{关系键++}, 请求.任务.值,
                L1所有者范围写集本地键{R键},
                初次筹办治理定位_.任务当前轮次关系类型, 1});
            写集.退出事实 = {当前轮次组->front().编码, 旧P任务关系.编码};
            const auto 写入 = 第一层写入端口_.提交所有者范围中性写集(写集);
            const auto 状态 = 映射任务后继继续写入状态(写入.状态);
            if (状态 != L2任务后继继续状态::已形成
                && 状态 != L2任务后继继续状态::精确重复)
                return 失败(状态);
            const auto R = 查找任务写入映射(写入, R键);
            const auto P = 查找任务写入映射(写入, P键);
            const auto A = 查找任务写入映射(写入, A键);
            const auto D = 查找任务写入映射(写入, D键);
            if (R && P && A && D) {
                const auto 读回 = 读取任务后继继续事实_已锁定({
                    L2任务后继机械合同版本, 请求.任务, 请求.已收束任务轮次,
                    L2任务轮次身份{*R}, L2任务筹办轮次权威记录身份{*P},
                    L2任务治理动作调用身份{*A}, *D, 写入.事实代次});
                if (读回.成功()) {
                    L2任务后继继续写入结果 结果;
                    结果.状态 = 状态;
                    结果.新任务轮次 = std::move(读回.新任务轮次);
                    结果.新筹办轮次 = std::move(读回.新筹办轮次);
                    结果.继续治理动作 = std::move(读回.继续治理动作);
                    结果.决议引用 = std::move(读回.决议引用);
                    结果.本次正式读回截止 = 读回.本次正式读回截止;
                    return 结果;
                }
            }
            L2任务后继继续写入结果 结果;
            结果.状态 = L2任务后继继续状态::已可能发布;
            结果.提交见证 = L2任务后继提交见证{
                第一层写入端口_.所有者身份(), 请求.幂等身份,
                写入.事实代次, 1};
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(L2任务后继继续状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2任务后继继续状态::资源失败);
        } catch (...) {
            return 失败(L2任务后继继续状态::内部不一致);
        }
    }

    L2任务筹办正式选择读取结果_v2 读取正式选择投影_v2(
        L2任务方法选择记录身份 选择记录,
        std::uint64_t 截止,
        L2读取类别 读取类别,
        std::uint64_t 历史截止) const {
        L2任务筹办正式选择读取结果_v2 结果;
        结果.读取类别 = 读取类别;
        结果.历史截止事实代次 = 历史截止;
        if (!有效(选择记录.值) || 截止 == 0) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::入口拒绝;
            return 结果;
        }
        L1所有者范围一致当前读取请求 选择请求;
        选择请求.合同版本 = L1所有者范围一致当前读取合同版本;
        选择请求.期望事实代次 = 截止;
        选择请求.所有者 = {身份来源定位_.所有者};
        选择请求.节点 = {选择记录.值};
        选择请求.属性值 = {{选择记录.值, 正式选择定位_.完整请求属性类型}};
        选择请求.源关系组 = {
            {选择记录.值, 正式选择定位_.选择记录方法引用关系类型},
            {选择记录.值, 正式选择定位_.选择记录路径引用关系类型},
            {选择记录.值, 正式选择定位_.选择记录来源需求关系类型},
            {选择记录.值, 正式选择定位_.选择记录稳定证据关系类型},
            {选择记录.值, 正式选择定位_.选择记录冻结材料关系类型}};
        const auto 选择读取 = L1_.尝试读取所有者范围一致当前投影(选择请求);
        if (选择读取.状态 != L1所有者范围一致当前读取状态::成功
            || 选择读取.读取事实代次 != 截止
            || 选择读取.节点.size() != 1 || 选择读取.属性值.size() != 1
            || 选择读取.源关系组.size() != 5) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::内部错误;
            return 结果;
        }
        if (选择读取.节点.front().状态
                == L1所有者范围一致当前读取项目状态::未找到) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::未找到;
            return 结果;
        }
        if (选择读取.节点.front().状态
                != L1所有者范围一致当前读取项目状态::成功
            || !选择读取.节点.front().事实
            || !选择读取.属性值.front().投影) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::引用冲突;
            return 结果;
        }
        const auto* 完整值 = std::get_if<std::vector<std::uint64_t>>(
            &选择读取.属性值.front().投影->当前值事实.材料);
        const auto 首次请求 = 完整值
            ? L2任务结构内部::解码正式选择完整请求_v2(*完整值)
            : std::nullopt;
        if (!首次请求) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::引用冲突;
            return 结果;
        }
        const auto 找组 = [&](稳定编码 类型)
            -> const L1所有者范围一致源关系组读取结果项* {
            for (const auto& 组 : 选择读取.源关系组)
                if (组.关系类型节点 == 类型) return &组;
            return nullptr;
        };
        const auto* 方法组 = 找组(正式选择定位_.选择记录方法引用关系类型);
        const auto* 路径组 = 找组(正式选择定位_.选择记录路径引用关系类型);
        const auto* 需求组 = 找组(正式选择定位_.选择记录来源需求关系类型);
        const auto* 证据组 = 找组(正式选择定位_.选择记录稳定证据关系类型);
        const auto* 冻结组 = 找组(正式选择定位_.选择记录冻结材料关系类型);
        if (!方法组 || !路径组 || !需求组 || !证据组 || !冻结组
            || 方法组->成员.size() != 1 || 路径组->成员.size() != 1
            || 冻结组->成员.size() != 1
            || 需求组->成员.size() != 首次请求->基础材料.来源需求组.size()
            || 证据组->成员.size() != 首次请求->基础材料.稳定证据组.size()) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::引用冲突;
            return 结果;
        }
        const auto 方法 = L2方法身份{方法组->成员.front().关系.目标节点};
        const auto 路径身份 = L2任务方法路径身份{
            路径组->成员.front().关系.目标节点};
        const auto 冻结身份 = L2任务执行绑定冻结材料身份{
            冻结组->成员.front().关系.目标节点};
        if (方法 != 首次请求->冻结材料.方法) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::引用冲突;
            return 结果;
        }
        for (std::size_t i = 0; i < 需求组->成员.size(); ++i)
            if (需求组->成员[i].关系.角色或顺序 != static_cast<std::int64_t>(i + 1)
                || L2需求身份{需求组->成员[i].关系.目标节点}
                    != 首次请求->基础材料.来源需求组[i]) {
                结果.状态 = L2任务筹办正式选择读取状态_v2::引用冲突;
                return 结果;
            }
        for (std::size_t i = 0; i < 证据组->成员.size(); ++i) {
            const auto& 证据 = 首次请求->基础材料.稳定证据组[i];
            if (证据组->成员[i].关系.角色或顺序
                    != static_cast<std::int64_t>(证据.类别)
                || 证据组->成员[i].关系.目标节点 != 证据.身份) {
                结果.状态 = L2任务筹办正式选择读取状态_v2::引用冲突;
                return 结果;
            }
        }
        L1所有者范围一致当前读取请求 路径请求;
        路径请求.合同版本 = L1所有者范围一致当前读取合同版本;
        路径请求.期望事实代次 = 截止;
        路径请求.所有者 = {身份来源定位_.所有者};
        路径请求.节点 = {路径身份.值, 身份来源定位_.路径族归属关系,
            类型定位_.路径族归属关系类型};
        路径请求.源关系组 = {
            {路径身份.值, 类型定位_.路径族归属关系类型},
            {路径身份.值, 类型定位_.路径任务引用关系类型}};
        路径请求.目标关系组 = {{路径身份.值,
            正式选择定位_.选择记录路径引用关系类型}};
        路径请求.属性值 = {
            {路径身份.值, 身份来源定位_.路径筹办轮次属性类型},
            {路径身份.值, 身份来源定位_.路径来源方法属性类型},
            {路径身份.值, 身份来源定位_.路径来源方法内容版本属性类型},
            {路径身份.值, 身份来源定位_.路径完整有序动作组属性类型_v2},
            {路径身份.值, 身份来源定位_.路径预期结果属性类型},
            {路径身份.值, 身份来源定位_.路径验证合同属性类型}};
        const auto 路径读取 = L1_.尝试读取所有者范围一致当前投影(路径请求);
        if (路径读取.状态 != L1所有者范围一致当前读取状态::成功
            || 路径读取.读取事实代次 != 截止 || 路径读取.节点.size() != 3
            || 路径读取.源关系组.size() != 2
            || 路径读取.目标关系组.size() != 1
            || 路径读取.属性值.size() != 6) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::引用冲突;
            return 结果;
        }
        const L1所有者范围一致节点读取结果项* 路径节点 = nullptr;
        for (const auto& 节点 : 路径读取.节点)
            if (节点.查询编码 == 路径身份.值) 路径节点 = &节点;
        if (!路径节点 || !路径节点->事实
            || 路径节点->状态 != L1所有者范围一致当前读取项目状态::成功) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::引用冲突;
            return 结果;
        }
        const auto 找属性 = [&](稳定编码 类型)
            -> const L1所有者范围值事实* {
            for (const auto& 属性 : 路径读取.属性值)
                if (属性.属性类型 == 类型 && 属性.投影)
                    return &属性.投影->当前值事实;
            return nullptr;
        };
        const auto* 轮次值 = 找属性(身份来源定位_.路径筹办轮次属性类型);
        const auto* 方法值 = 找属性(身份来源定位_.路径来源方法属性类型);
        const auto* 版本值 = 找属性(身份来源定位_.路径来源方法内容版本属性类型);
        const auto* 动作组值 = 找属性(身份来源定位_.路径完整有序动作组属性类型_v2);
        const auto* 预期值 = 找属性(身份来源定位_.路径预期结果属性类型);
        const auto* 验证值 = 找属性(身份来源定位_.路径验证合同属性类型);
        const auto 取I64 = [](const L1所有者范围值事实* 值)
            -> std::optional<std::int64_t> {
            return 值 ? (std::get_if<std::int64_t>(&值->材料)
                ? std::optional<std::int64_t>{*std::get_if<std::int64_t>(&值->材料)}
                : std::nullopt) : std::nullopt;
        };
        const auto 取引用 = [](const L1所有者范围值事实* 值)
            -> std::optional<稳定编码> {
            const auto* 引用 = 值
                ? std::get_if<L1所有者范围独立材料引用>(&值->材料) : nullptr;
            return 引用 ? std::optional<稳定编码>{引用->编码} : std::nullopt;
        };
        const auto 轮次 = 取I64(轮次值); const auto 路径方法 = 取I64(方法值);
        const auto 版本 = 取I64(版本值); const auto 预期 = 取引用(预期值);
        const auto 验证 = 取引用(验证值);
        const auto* 动作序列 = 动作组值
            ? std::get_if<std::vector<std::uint64_t>>(&动作组值->材料) : nullptr;
        if (!轮次 || !路径方法 || !版本 || !预期 || !验证 || !动作序列) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::引用冲突;
            return 结果;
        }
        L2任务结构内部::正式选择序列读取器 动作读{*动作序列};
        const auto 动作格式 = 动作读.读(); const auto 动作数 = 动作读.读();
        std::vector<L2任务执行路径动作冻结项_v2> 动作组;
        if (!动作格式 || *动作格式 != L2任务路径动作组序列格式版本_v2
            || !动作数 || *动作数 == 0 || *动作数 > 1'000'000) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::引用冲突;
            return 结果;
        }
        动作组.reserve(static_cast<std::size_t>(*动作数));
        for (std::uint64_t i = 0; i < *动作数; ++i) {
            L2任务执行路径动作冻结项_v2 动作;
            if (!L2任务结构内部::读动作冻结项_v2(动作读, 动作)) {
                结果.状态 = L2任务筹办正式选择读取状态_v2::引用冲突;
                return 结果;
            }
            动作组.push_back(std::move(动作));
        }
        if (!动作读.结束() || 动作组 != 首次请求->冻结材料.完整有序动作组) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::引用冲突;
            return 结果;
        }
        const auto 来源任务 = L2取得任务筹办任务(首次请求->基础材料.来源材料);
        L1所有者范围一致当前读取请求 当前请求;
        当前请求.合同版本 = L1所有者范围一致当前读取合同版本;
        当前请求.期望事实代次 = 截止;
        当前请求.所有者 = {身份来源定位_.所有者};
        当前请求.源关系组 = {
            {来源任务.值, 正式选择定位_.任务当前正式选择关系类型},
            {来源任务.值, 类型定位_.实例方法当前关系类型}};
        const auto 当前读取 = L1_.尝试读取所有者范围一致当前投影(当前请求);
        if (当前读取.状态 != L1所有者范围一致当前读取状态::成功
            || 当前读取.读取事实代次 != 截止 || 当前读取.源关系组.size() != 2) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::引用冲突;
            return 结果;
        }
        const L1所有者范围一致源关系组读取结果项* 当前选择 = nullptr;
        const L1所有者范围一致源关系组读取结果项* 当前实例 = nullptr;
        for (const auto& 组 : 当前读取.源关系组) {
            if (组.关系类型节点 == 正式选择定位_.任务当前正式选择关系类型)
                当前选择 = &组;
            else if (组.关系类型节点 == 类型定位_.实例方法当前关系类型)
                当前实例 = &组;
        }
        if (!当前选择 || !当前实例 || 当前选择->成员.size() != 1
            || 当前实例->成员.size() != 1
            || 当前选择->成员.front().关系.目标节点 != 选择记录.值) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::引用冲突;
            return 结果;
        }
        const auto 实例 = L2任务结构内部::读取实例方法_v2当前投影(
            L1_, 身份来源定位_, 类型定位_,
            L2实例方法身份{当前实例->成员.front().关系.目标节点}, 截止,
            来源任务, 路径身份, static_cast<std::uint64_t>(*轮次), 方法,
            L2方法内容版本{static_cast<std::uint64_t>(*版本)});
        if (!实例 || 实例->当前选中路径 != 路径身份) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::引用冲突;
            return 结果;
        }
        const auto 创建代次 = 路径节点->事实->创建事实代次;
        L2任务方法路径事实_v2 路径事实;
        路径事实.身份 = 路径身份; 路径事实.任务 = 来源任务;
        路径事实.筹办轮次 = static_cast<std::uint64_t>(*轮次);
        路径事实.来源方法 = L2方法身份{稳定编码{
            static_cast<std::uint64_t>(*路径方法)}};
        路径事实.来源方法内容版本 = {
            static_cast<std::uint64_t>(*版本)};
        路径事实.完整路径材料.条件绑定 = 冻结身份.值;
        路径事实.完整路径材料.输入绑定 = 冻结身份.值;
        路径事实.完整路径材料.参数绑定 = 冻结身份.值;
        路径事实.完整路径材料.完整有序动作组 = std::move(动作组);
        路径事实.完整路径材料.预期结果 = *预期;
        路径事实.完整路径材料.验证合同 = *验证;
        路径事实.完整路径材料.归因合同 = 选择记录.值;
        路径事实.生命周期 = {创建代次, std::nullopt};
        L2任务方法选择记录事实 记录;
        记录.记录版本 = 选择读取.节点.front().事实->创建事实代次;
        记录.身份 = 选择记录;
        记录.原筹办来源幂等身份 = L2取得任务筹办来源幂等身份(
            首次请求->基础材料.来源材料);
        记录.筹办轮次 = L2取得任务筹办轮次(
            首次请求->基础材料.来源材料);
        记录.选择规则版本 = 首次请求->基础材料.选择规则版本;
        记录.召回规则版本 = 首次请求->基础材料.召回规则版本;
        记录.方法内容版本 = 首次请求->冻结材料.方法内容版本;
        记录.任务身份来源 = 首次请求->基础材料.任务身份来源;
        记录.方法生命周期 = 首次请求->基础材料.方法生命周期;
        记录.生命周期 = {记录.记录版本, std::nullopt};
        L2任务方法选择完整事实_v2 选择;
        选择.记录 = std::move(记录); 选择.任务 = 来源任务; 选择.方法 = 方法;
        选择.路径 = std::move(路径事实);
        选择.来源需求组 = 首次请求->基础材料.来源需求组;
        选择.稳定证据组 = 首次请求->基础材料.稳定证据组;
        选择.冻结材料 = 冻结身份; 选择.当前 = true;
        L2任务执行绑定冻结材料事实_v2 冻结事实;
        冻结事实.身份 = 冻结身份; 冻结事实.材料 = 首次请求->冻结材料;
        冻结事实.生命周期 = {创建代次, std::nullopt};
        if (!L2任务筹办正式选择三事实_v2闭合(
                选择, 冻结事实, *实例, 截止)) {
            结果.状态 = L2任务筹办正式选择读取状态_v2::引用冲突;
            return 结果;
        }
        结果.状态 = L2任务筹办正式选择读取状态_v2::已读取;
        结果.正式选择 = std::move(选择); 结果.冻结材料 = std::move(冻结事实);
        结果.实例方法 = *实例; 结果.本次正式读回截止 = 截止;
        return 结果;
    }

    L2任务方法选择读取结果 读取正式选择当前投影(
        L2任务方法选择记录身份 选择记录,
        std::uint64_t 截止) const {
        L2任务方法选择读取结果 结果;
        结果.结果头 = {L2结构合同版本, L2结构状态::内部不一致,
            截止, std::nullopt};
        结果.读取类别 = L2读取类别::当前;
        if (!有效(选择记录.值) || 截止 == 0) {
            结果.结果头.状态 = L2结构状态::入口拒绝;
            return 结果;
        }
        L1所有者范围一致当前读取请求 读取请求;
        读取请求.合同版本 = L1所有者范围一致当前读取合同版本;
        读取请求.期望事实代次 = 截止;
        读取请求.所有者 = {身份来源定位_.所有者};
        读取请求.节点 = {选择记录.值};
        读取请求.属性值 = {{选择记录.值, 正式选择定位_.完整请求属性类型}};
        读取请求.源关系组 = {
            {选择记录.值, 正式选择定位_.选择记录方法引用关系类型},
            {选择记录.值, 正式选择定位_.选择记录路径引用关系类型},
            {选择记录.值, 正式选择定位_.选择记录来源需求关系类型},
            {选择记录.值, 正式选择定位_.选择记录稳定证据关系类型},
            {选择记录.值, 正式选择定位_.选择记录冻结材料关系类型}};
        const auto 读取 = L1_.尝试读取所有者范围一致当前投影(读取请求);
        if (读取.状态 != L1所有者范围一致当前读取状态::成功
            || 读取.读取事实代次 != 截止 || 读取.节点.size() != 1
            || 读取.属性值.size() != 1 || 读取.源关系组.size() != 5)
            return 结果;
        if (!读取.节点.front().事实
            || 读取.节点.front().状态
                != L1所有者范围一致当前读取项目状态::成功
            || !读取.属性值.front().投影)
            return 结果;
        const auto* 完整请求值 = std::get_if<std::vector<std::uint64_t>>(
            &读取.属性值.front().投影->当前值事实.材料);
        if (!完整请求值) return 结果;
        const auto 首次请求 = L2任务结构内部::解码正式选择完整请求(*完整请求值);
        if (!首次请求) return 结果;
        const auto 找关系组 = [&](稳定编码 类型)
            -> const L1所有者范围一致源关系组读取结果项* {
            for (const auto& 组 : 读取.源关系组)
                if (组.关系类型节点 == 类型) return &组;
            return nullptr;
        };
        const auto* 方法组 = 找关系组(正式选择定位_.选择记录方法引用关系类型);
        const auto* 路径组 = 找关系组(正式选择定位_.选择记录路径引用关系类型);
        const auto* 需求组 = 找关系组(正式选择定位_.选择记录来源需求关系类型);
        const auto* 证据组 = 找关系组(正式选择定位_.选择记录稳定证据关系类型);
        const auto* 冻结组 = 找关系组(正式选择定位_.选择记录冻结材料关系类型);
        if (!方法组 || !路径组 || !需求组 || !证据组 || !冻结组
            || 方法组->成员.size() != 1 || 路径组->成员.size() != 1
            || 冻结组->成员.size() != 1
            || 需求组->成员.size() != 首次请求->来源需求组.size()
            || 证据组->成员.size() != 首次请求->稳定证据组.size())
            return 结果;
        const auto 方法 = L2方法身份{方法组->成员.front().关系.目标节点};
        const auto 路径身份 = L2任务方法路径身份{路径组->成员.front().关系.目标节点};
        const auto 冻结身份 = L2任务执行绑定冻结材料身份{冻结组->成员.front().关系.目标节点};
        if (方法 != 首次请求->冻结材料.方法) return 结果;
        for (std::size_t 索引 = 0; 索引 < 需求组->成员.size(); ++索引) {
            const auto& 关系 = 需求组->成员[索引].关系;
            if (关系.角色或顺序 != static_cast<std::int64_t>(索引 + 1)
                || L2需求身份{关系.目标节点} != 首次请求->来源需求组[索引])
                return 结果;
        }
        for (std::size_t 索引 = 0; 索引 < 证据组->成员.size(); ++索引) {
            const auto& 关系 = 证据组->成员[索引].关系;
            const auto& 证据 = 首次请求->稳定证据组[索引];
            if (关系.角色或顺序 != static_cast<std::int64_t>(证据.类别)
                || 关系.目标节点 != 证据.身份) return 结果;
        }
        const auto 路径 = L2任务结构内部::读取任务方法路径当前投影(
            L1_, 身份来源定位_, 类型定位_,
            {{L2结构合同版本, 截止}, L2读取类别::当前, 路径身份, 0},
            截止, &正式选择定位_);
        if (!路径.成功() || !路径.路径) return 结果;
        L1所有者范围一致当前读取请求 当前请求;
        当前请求.合同版本 = L1所有者范围一致当前读取合同版本;
        当前请求.期望事实代次 = 截止;
        当前请求.所有者 = {身份来源定位_.所有者};
        const auto 来源任务 = L2取得任务筹办任务(首次请求->来源材料);
        当前请求.源关系组 = {
            {来源任务.值,
                正式选择定位_.任务当前正式选择关系类型},
            {来源任务.值,
                类型定位_.实例方法当前关系类型}};
        const auto 当前读取 = L1_.尝试读取所有者范围一致当前投影(当前请求);
        if (当前读取.状态 != L1所有者范围一致当前读取状态::成功
            || 当前读取.读取事实代次 != 截止
            || 当前读取.源关系组.size() != 2) return 结果;
        const L1所有者范围一致源关系组读取结果项* 当前选择组 = nullptr;
        const L1所有者范围一致源关系组读取结果项* 当前实例组 = nullptr;
        for (const auto& 组 : 当前读取.源关系组) {
            if (组.关系类型节点 == 正式选择定位_.任务当前正式选择关系类型)
                当前选择组 = &组;
            else if (组.关系类型节点 == 类型定位_.实例方法当前关系类型)
                当前实例组 = &组;
        }
        if (!当前选择组 || !当前实例组 || 当前选择组->成员.size() != 1
            || 当前实例组->成员.size() != 1
            || 当前选择组->成员.front().关系.目标节点 != 选择记录.值)
            return 结果;
        const auto 实例 = L2任务结构内部::读取实例方法当前投影(
            L1_, 身份来源定位_, 类型定位_,
            L2实例方法身份{当前实例组->成员.front().关系.目标节点}, 截止,
            &正式选择定位_);
        if (!实例 || 实例->当前选中路径 != 路径身份) return 结果;
        const auto 创建代次 = 读取.节点.front().事实->创建事实代次;
        const auto& 请求 = *首次请求;
        L2任务方法选择记录事实 记录;
        记录.记录版本 = 创建代次; 记录.身份 = 选择记录;
        记录.原筹办来源幂等身份 =
            L2取得任务筹办来源幂等身份(请求.来源材料);
        记录.筹办轮次 = L2取得任务筹办轮次(请求.来源材料);
        记录.选择规则版本 = 请求.选择规则版本;
        记录.召回规则版本 = 请求.召回规则版本;
        记录.方法内容版本 = 请求.冻结材料.方法内容版本;
        记录.任务身份来源 = 请求.任务身份来源;
        记录.方法生命周期 = 请求.方法生命周期;
        记录.生命周期 = {创建代次, std::nullopt};
        L2任务方法选择完整事实 选择;
        选择.记录 = std::move(记录); 选择.任务 = 来源任务;
        选择.方法 = 方法; 选择.路径 = *路径.路径;
        选择.来源需求组 = 请求.来源需求组;
        选择.稳定证据组 = 请求.稳定证据组;
        选择.冻结材料 = 冻结身份; 选择.当前 = true;
        const auto& 材料 = 选择.路径.完整路径材料;
        if (材料.条件绑定 != 冻结身份.值 || 材料.输入绑定 != 冻结身份.值
            || 材料.参数绑定 != 冻结身份.值
            || 材料.动作入口 != 请求.冻结材料.动作入口.身份.值
            || 材料.预期结果 != 请求.冻结材料.主轴结果.来源稳定编码
            || 材料.验证合同 != 请求.冻结材料.验证合同.值
            || 材料.归因合同 != 选择记录.值) return 结果;
        结果.正式选择 = std::move(选择);
        结果.冻结材料 = L2任务执行绑定冻结材料事实{
            冻结身份, 请求.冻结材料, {创建代次, std::nullopt}};
        结果.实例方法 = *实例;
        结果.结果头.状态 = L2结构状态::已读取;
        return 结果;
    }

    static L1所有者范围写端口&& 验证并移动交付(
        const L1事实基座服务& 第一层服务,
        L2任务所有者交付& 交付) {
        if (!交付.有效() || !交付.绑定于(第一层服务))
            throw std::runtime_error("L2 task owner delivery is invalid");
        return std::move(交付.写入端口_);
    }

    const L1事实基座服务& L1_;
    const L2需求结构服务& 需求结构服务_;
    L1所有者范围写端口 第一层写入端口_;
    L2任务结构内部::任务身份来源定位 身份来源定位_;
    L2任务结构内部::任务结构类型定位 类型定位_;
    L2任务结构内部::任务轮次核心定位 任务轮次核心定位_;
    L2任务结构内部::任务初次筹办治理定位 初次筹办治理定位_;
    L2任务结构内部::任务轮次结算登记定位 轮次结算定位_;
    L2任务结构内部::任务初次筹办准备记录定位 初次筹办记录定位_;
    L2任务结构内部::任务筹办轮次定位 筹办轮次定位_;
    L2任务结构内部::任务子目标承接记录定位 子目标记录定位_;
    L2任务结构内部::任务正式选择登记定位 正式选择定位_;
    mutable std::mutex 互斥体_;
#ifdef ARCH_L4_SELF_GOVERNANCE_TEST_SEAM
    bool ARCH_任务实际结果提交后首次读回失败一次_ = false;
#endif
};

inline std::optional<L2任务方法路径写入端口>
尝试形成L2任务方法路径写入端口(
    L2任务结构服务& 任务服务,
    L2任务方法路径写入能力交付&& 交付) noexcept {
    const auto 凭据有效 = 交付.有效_;
    const auto 凭据所有者 = 交付.所有者_;
    交付.有效_ = false;
    if (!凭据有效 || !任务服务.第一层写入端口_.有效()
        || 凭据所有者 != 任务服务.第一层写入端口_.所有者身份())
        return std::nullopt;
    return L2任务方法路径写入端口{任务服务};
}

inline L2新增任务方法路径结果
L2任务方法路径写入端口::写入已验证任务方法路径(
    L2新增任务方法路径请求 请求) noexcept {
    if (服务_)
        return 服务_->写入已验证任务方法路径(std::move(请求));
    L2新增任务方法路径结果 结果;
    结果.结果头.合同版本 = L2结构合同版本;
    结果.结果头.状态 = L2结构状态::内部不一致;
    return 结果;
}

inline std::optional<L2任务子目标承接记录写入端口>
尝试形成L2任务子目标承接记录写入端口(
    L2任务结构服务& 任务服务,
    L2任务子目标承接记录写入能力交付&& 交付) noexcept {
    const auto 凭据有效 = 交付.有效_;
    const auto 凭据所有者 = 交付.所有者_;
    交付.有效_ = false;
    if (!凭据有效 || !任务服务.第一层写入端口_.有效()
        || 凭据所有者 != 任务服务.第一层写入端口_.所有者身份())
        return std::nullopt;
    return L2任务子目标承接记录写入端口{任务服务};
}

inline L2任务子目标承接记录写入结果
L2任务子目标承接记录写入端口::写入已验证新增记录(
    L2新增任务子目标承接记录请求 请求) noexcept {
    if (服务_)
        return 服务_->写入已验证新增任务子目标承接记录(std::move(请求));
    L2任务子目标承接记录写入结果 结果;
    结果.结果头.状态 = L2结构状态::内部不一致;
    return 结果;
}

inline L2任务子目标承接记录写入结果
L2任务子目标承接记录写入端口::写入已验证绑定子需求(
    L2绑定任务子目标承接记录子需求请求 请求) noexcept {
    if (服务_)
        return 服务_->写入已验证绑定任务子目标承接记录子需求(
            std::move(请求));
    L2任务子目标承接记录写入结果 结果;
    结果.结果头.状态 = L2结构状态::内部不一致;
    return 结果;
}

inline L2任务子目标承接记录写入结果
L2任务子目标承接记录写入端口::写入已验证登记回流(
    L2登记任务子目标承接记录回流请求 请求) noexcept {
    if (服务_)
        return 服务_->写入已验证登记任务子目标承接记录回流(
            std::move(请求));
    L2任务子目标承接记录写入结果 结果;
    结果.结果头.状态 = L2结构状态::内部不一致;
    return 结果;
}

inline L2任务子目标承接记录写入结果
L2任务子目标承接记录写入端口::写入已验证退出记录(
    L2退出任务子目标承接记录请求 请求) noexcept {
    if (服务_)
        return 服务_->写入已验证退出任务子目标承接记录(std::move(请求));
    L2任务子目标承接记录写入结果 结果;
    结果.结果头.状态 = L2结构状态::内部不一致;
    return 结果;
}

} // namespace 海中鱼巣
