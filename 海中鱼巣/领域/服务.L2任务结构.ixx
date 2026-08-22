module;

#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <limits>
#include <mutex>
#include <new>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#define L2_TASK_STRUCTURE_NO_INCLUDES
#define L2_TASK_SUBGOAL_ACCEPTANCE_RECORD_NO_INCLUDES

export module 海中鱼巣.领域.服务.L2任务结构;

export import 海中鱼巣.领域.合同.L2结构公共;
import 海中鱼巣.核心.服务.L1事实基座;
import 海中鱼巣.领域.服务.L2需求结构;
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

// ===== 固定幂等身份 =====
inline constexpr L1所有者范围写入幂等身份 任务身份来源登记幂等身份{
    0x4C32'5441'534B'4944ULL};
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

inline constexpr std::uint32_t 任务族归属关系类型键值 = 0x0301'2111U;
inline constexpr std::uint32_t 任务需求列表项引用关系类型键值 = 0x0301'2112U;
inline constexpr std::uint32_t 任务虚拟存在归属关系类型键值 = 0x0301'2113U;
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

// ===== 写入本地键值（新增任务） =====
inline constexpr std::uint32_t 任务节点写入本地键值 = 0x0301'2201U;
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
    const auto 代次 = 读取中性当前事实代次(第一层服务);
    if (代次 == 0)
        throw std::runtime_error("L2 task type init: no current fact");

    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 代次;
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

// 任务实际结果的登记独立于既有任务登记；不得复用旧幂等身份重放。
void 初始化任务实际结果登记(
    const L1事实基座服务& 第一层服务,
    L1所有者范围写端口& 写入端口,
    任务身份来源定位& 来源定位,
    任务结构类型定位& 类型定位) {
    const auto 当前代次 = 读取中性当前事实代次(第一层服务);
    if (当前代次 == 0) throw std::runtime_error("task result registry: no fact");
    const auto 键 = [](std::uint32_t 值) noexcept {
        return L1所有者范围写集本地键{值};
    };
    L1所有者范围写集请求 身份写集;
    身份写集.合同版本 = L1所有者范围CRUD合同版本;
    身份写集.期望事实代次 = 当前代次;
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
    const auto 类型代次 = 读取中性当前事实代次(第一层服务);
    L1所有者范围写集请求 类型写集;
    类型写集.合同版本 = L1所有者范围CRUD合同版本;
    类型写集.期望事实代次 = 类型代次;
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
    身份写集.期望事实代次 = 读取中性当前事实代次(第一层服务);
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
    类型写集.期望事实代次 = 读取中性当前事实代次(第一层服务);
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
    std::uint64_t 期望代次) {
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
        || !读取.目标关系组.empty())
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
    if (!轮次项 || !方法项 || !版本项
        || !条件项 || !输入项 || !参数项
        || !动作项 || !预期项 || !验证项 || !归因项)
        return 失败(L2结构状态::内部不一致);
    if (轮次项->状态 != L1所有者范围一致当前读取项目状态::成功
        || 方法项->状态 != L1所有者范围一致当前读取项目状态::成功
        || 版本项->状态 != L1所有者范围一致当前读取项目状态::成功
        || 条件项->状态 != L1所有者范围一致当前读取项目状态::成功
        || 输入项->状态 != L1所有者范围一致当前读取项目状态::成功
        || 参数项->状态 != L1所有者范围一致当前读取项目状态::成功
        || 动作项->状态 != L1所有者范围一致当前读取项目状态::成功
        || 预期项->状态 != L1所有者范围一致当前读取项目状态::成功
        || 验证项->状态 != L1所有者范围一致当前读取项目状态::成功
        || 归因项->状态 != L1所有者范围一致当前读取项目状态::成功
        || !轮次项->投影 || !方法项->投影 || !版本项->投影
        || !条件项->投影 || !输入项->投影 || !参数项->投影
        || !动作项->投影 || !预期项->投影 || !验证项->投影
        || !归因项->投影)
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
        || !验证值(*条件项->投影, 请求.路径.值,
        来源定位.路径条件绑定属性类型, 来源定位.所有者)
        || !验证值(*输入项->投影, 请求.路径.值,
        来源定位.路径输入绑定属性类型, 来源定位.所有者)
        || !验证值(*参数项->投影, 请求.路径.值,
        来源定位.路径参数绑定属性类型, 来源定位.所有者)
        || !验证值(*动作项->投影, 请求.路径.值,
        来源定位.路径动作入口属性类型, 来源定位.所有者)
        || !验证值(*预期项->投影, 请求.路径.值,
        来源定位.路径预期结果属性类型, 来源定位.所有者)
        || !验证值(*验证项->投影, 请求.路径.值,
        来源定位.路径验证合同属性类型, 来源定位.所有者)
        || !验证值(*归因项->投影, 请求.路径.值,
        来源定位.路径归因合同属性类型, 来源定位.所有者))
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
    const auto 条件绑定 = 取引用(*条件项->投影);
    const auto 输入绑定 = 取引用(*输入项->投影);
    const auto 参数绑定 = 取引用(*参数项->投影);
    const auto 动作入口 = 取引用(*动作项->投影);
    const auto 预期结果 = 取引用(*预期项->投影);
    const auto 验证合同 = 取引用(*验证项->投影);
    const auto 归因合同 = 取引用(*归因项->投影);
    if (!条件绑定 || !输入绑定 || !参数绑定
        || !动作入口 || !预期结果 || !验证合同 || !归因合同)
        return 失败(L2结构状态::内部不一致);

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
    const 任务结构类型定位& 类型, L2实例方法身份 身份, std::uint64_t G0) {
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
    const auto 路径结果=读取任务方法路径当前投影(L1,来源,类型,路径请求,G0);
    if(!路径结果.成功()||!路径结果.路径||路径结果.路径->任务!=L2任务身份{*任务}) return std::nullopt;
    return L2实例方法事实{身份,L2任务身份{*任务},L2任务方法路径身份{*路径},路径结果.路径->筹办轮次,static_cast<std::uint64_t>(*轮),路径结果.路径->来源方法,路径结果.路径->来源方法内容版本,{读.节点[0].事实->创建事实代次,std::nullopt}};
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
    std::uint64_t 写入代次) {
    const auto 失败 = [&](L2结构状态 状态) noexcept {
        L2新增任务结果 结果;
        结果.结果头.合同版本 = L2结构合同版本;
        结果.结果头.状态 = 状态;
        结果.结果头.事实截止代次 = 写入代次;
        结果.结果头.变更事实代次 = 写入代次;
        return 结果;
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
        || 任务节点->退出事实代次
        || 任务节点->创建事实代次 != 写入代次)
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
        || 虚拟存在节点->退出事实代次
        || 虚拟存在节点->创建事实代次 != 写入代次)
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

    const auto 族归属 = 读回关系(编码映射.任务族归属关系);
    const auto 列表项引用 = 读回关系(编码映射.任务需求列表项引用关系);
    const auto 虚拟存在归属 = 读回关系(编码映射.任务虚拟存在归属关系);
    if (!族归属 || !列表项引用 || !虚拟存在归属)
        return 失败(L2结构状态::内部不一致);

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
        任务节点->退出事实代次};
    身份来源.族归属关系生命周期 = {族归属->创建事实代次,
        族归属->退出事实代次};
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
    结果.结果头.状态 = L2结构状态::已提交;
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
    const auto 当前代次 = 读取中性当前事实代次(第一层服务);
    if (当前代次 == 0)
        throw std::runtime_error("L2 task subgoal record init has no fact generation");
    L1所有者范围写集请求 写集;
    写集.合同版本 = L1所有者范围CRUD合同版本;
    写集.期望事实代次 = 当前代次;
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
          子目标记录定位_(
              L2任务结构内部::初始化任务子目标承接记录登记(
                  L1_, 第一层写入端口_)) {
        L2任务结构内部::初始化任务实际结果登记(
            L1_, 第一层写入端口_, 身份来源定位_, 类型定位_);
        L2任务结构内部::初始化任务治理状态登记(
            L1_, 第一层写入端口_, 身份来源定位_, 类型定位_);
    }
    L2任务结构服务(const L2任务结构服务&) = delete;
    L2任务结构服务& operator=(const L2任务结构服务&) = delete;

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
                提交.编码映射, 提交.事实截止代次);
        } catch (const std::bad_alloc&) {
            return 失败(L2结构状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L2结构状态::资源失败);
        } catch (...) {
            return 失败(L2结构状态::内部不一致);
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

            // 构造退出写集：退出 2 节点 + 3 关系
            L1所有者范围写集请求 写集;
            写集.合同版本 = L1所有者范围CRUD合同版本;
            写集.期望事实代次 = 请求.请求头.期望事实代次;
            写集.写入幂等身份 = L2任务结构内部::映射任务幂等键(
                请求.幂等身份);
            写集.退出事实 = {请求.任务.值, 虚拟存在编码,
                族归属编码, 列表项引用编码, 虚拟存在归属编码};

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
                L1_, 身份来源定位_, 类型定位_, 请求, 截止);
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

    // ===== 发布当前选中路径 =====
    L2发布当前选中路径结果 发布当前选中路径(
        L2发布当前选中路径请求 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [&](L2结构状态 状态, std::uint64_t 截止 = 0) {
            L2发布当前选中路径结果 结果;
            结果.结果头 = {L2结构合同版本, 状态, 截止, std::nullopt};
            return 结果;
        };
        try {
            if (!L2发布当前选中路径请求有效(请求)) return 失败(L2结构状态::入口拒绝);
            const auto 代次 = L2任务结构内部::检查任务写入当前代次(
                L1_, 请求.请求头.期望事实代次);
            if (!代次.成功() && !代次.直接提交) return 失败(代次.状态, 代次.事实截止代次);
            L2任务方法路径读取请求 路径请求{{L2结构合同版本, 请求.请求头.期望事实代次}, L2读取类别::当前, 请求.新路径};
            const auto 路径 = L2任务结构内部::读取任务方法路径当前投影(
                L1_, 身份来源定位_, 类型定位_, 路径请求, 请求.请求头.期望事实代次);
            if (!路径.成功() || !路径.路径 || 路径.路径->任务 != 请求.任务)
                return 失败(L2结构状态::引用冲突, 路径.结果头.事实截止代次);
            L2读取当前选中路径请求 当前请求{{L2结构合同版本, 请求.请求头.期望事实代次}, L2读取类别::当前, 请求.任务};
            const auto 当前 = L2任务结构内部::读取当前选中路径当前投影(
                L1_, 身份来源定位_, 类型定位_, 当前请求, 请求.请求头.期望事实代次);
            if (!当前.成功()) return 失败(当前.结果头.状态, 当前.结果头.事实截止代次);
            if (当前.当前选中路径) {
                if (*当前.当前选中路径 == 请求.新路径) {
                    L2发布当前选中路径结果 结果;
                    结果.结果头 = {L2结构合同版本, L2结构状态::精确重复,
                        请求.请求头.期望事实代次, std::nullopt};
                    结果.旧当前选中路径 = *当前.当前选中路径;
                    结果.新当前选中路径 = *当前.当前选中路径;
                    return 结果;
                }
                return 失败(L2结构状态::引用冲突, 请求.请求头.期望事实代次);
            }
            L1所有者范围写集请求 写集;
            写集.合同版本 = L1所有者范围CRUD合同版本;
            写集.期望事实代次 = 请求.请求头.期望事实代次;
            写集.写入幂等身份 = {请求.幂等身份.值};
            写集.关系 = {{{L2任务结构内部::当前选中路径关系写入本地键值},
                请求.任务.值, 请求.新路径.值,
                类型定位_.当前选中路径关系类型, 1}};
            const auto 写入 = 第一层写入端口_.提交所有者范围中性写集(写集);
            if (写入.状态 != L1所有者范围写入状态::成功) return 失败(
                L2任务结构内部::映射任务写入状态(写入.状态), 写入.事实代次);
            L2发布当前选中路径结果 结果;
            结果.结果头 = {L2结构合同版本, L2结构状态::已提交,
                写入.事实代次, 写入.事实代次};
            结果.新当前选中路径 = 请求.新路径;
            return 结果;
        } catch (const std::bad_alloc&) { return 失败(L2结构状态::资源失败); }
        catch (...) { return 失败(L2结构状态::内部不一致); }
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
            const auto 路径 = L2任务结构内部::读取任务方法路径当前投影(L1_, 身份来源定位_, 类型定位_, 路径请求, 请求.请求头.期望事实代次);
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
        try { 结果.实例方法=L2任务结构内部::读取实例方法当前投影(L1_,身份来源定位_,类型定位_,请求.实例方法,截止); if(!结果.实例方法) 结果.结果头.状态=L2结构状态::未找到; return 结果; } catch (...) {结果.结果头.状态=L2结构状态::内部不一致;return 结果;}
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
        try { L1所有者范围一致当前读取请求 查询; 查询.合同版本=L1所有者范围一致当前读取合同版本;查询.期望事实代次=截止;查询.所有者={身份来源定位_.所有者};查询.源关系组={{请求.任务.值,类型定位_.实例方法当前关系类型}};const auto 读=L1_.尝试读取所有者范围一致当前投影(查询);if(读.状态!=L1所有者范围一致当前读取状态::成功||读.读取事实代次!=截止||读.源关系组.size()!=1){结果.结果头.状态=L2结构状态::内部不一致;return 结果;}if(读.源关系组[0].成员.empty())return 结果;if(读.源关系组[0].成员.size()!=1){结果.结果头.状态=L2结构状态::内部不一致;return 结果;}结果.当前实例方法=L2任务结构内部::读取实例方法当前投影(L1_,身份来源定位_,类型定位_,L2实例方法身份{读.源关系组[0].成员[0].关系.目标节点},截止);if(!结果.当前实例方法)结果.结果头.状态=L2结构状态::内部不一致;return 结果;}catch(...){结果.结果头.状态=L2结构状态::内部不一致;return 结果;}
    }

    // ===== 记录任务实际结果 =====
    L2记录任务实际结果结果 记录任务实际结果(
        L2记录任务实际结果请求 请求) noexcept {
        std::lock_guard<std::mutex> 锁(互斥体_);
        const auto 失败 = [&](L2结构状态 状态, std::uint64_t 截止 = 0) {
            L2记录任务实际结果结果 结果; 结果.结果头 = {L2结构合同版本, 状态, 截止, std::nullopt}; return 结果; };
        try {
            if (!L2记录任务实际结果请求有效(请求)) return 失败(L2结构状态::入口拒绝);
            const auto 代次 = L2任务结构内部::检查任务写入当前代次(L1_, 请求.请求头.期望事实代次);
            if (!代次.成功() && !代次.直接提交) return 失败(代次.状态, 代次.事实截止代次);
            const auto G0 = 请求.请求头.期望事实代次;
            const auto 任务读取 = L2任务结构内部::读取任务当前投影(
                L1_, 身份来源定位_, 类型定位_,
                {{L2结构合同版本, G0}, L2读取类别::当前, 请求.任务}, G0);
            const auto 实例读取 = L2任务结构内部::读取实例方法当前投影(
                L1_, 身份来源定位_, 类型定位_, 请求.实例方法, G0);
            if (!任务读取.成功() || !任务读取.任务
                || 任务读取.任务->身份 != 请求.任务 || !实例读取
                || 实例读取->任务 != 请求.任务
                || 实例读取->执行轮次 != 请求.执行轮次
                || 实例读取->来源方法 != 请求.方法)
                return 失败(L2结构状态::引用冲突, G0);
            L2任务方法路径读取请求 路径请求{
                {L2结构合同版本, G0}, L2读取类别::当前,
                实例读取->当前选中路径};
            const auto 路径读取 = L2任务结构内部::读取任务方法路径当前投影(
                L1_, 身份来源定位_, 类型定位_, 路径请求, G0);
            L2读取当前选中路径请求 当前请求{
                {L2结构合同版本, G0}, L2读取类别::当前, 请求.任务};
            const auto 当前读取 = L2任务结构内部::读取当前选中路径当前投影(
                L1_, 身份来源定位_, 类型定位_, 当前请求, G0);
            if (!路径读取.成功() || !路径读取.路径
                || 路径读取.路径->任务 != 请求.任务
                || 路径读取.路径->来源方法 != 请求.方法
                || 路径读取.路径->完整路径材料.动作入口 != 请求.动作入口.值
                || !当前读取.成功() || !当前读取.当前选中路径
                || *当前读取.当前选中路径 != 实例读取->当前选中路径)
                return 失败(L2结构状态::引用冲突, G0);
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
            const auto 事实 = 有效(编码) ? L2任务结构内部::读取任务实际结果当前投影(L1_, 身份来源定位_, 类型定位_, L2任务实际结果身份{编码}, 写入.事实代次) : std::nullopt;
            if (!事实) return 失败(L2结构状态::内部不一致, 写入.事实代次);
            L2记录任务实际结果结果 结果; 结果.结果头 = {L2结构合同版本, L2结构状态::已提交, 写入.事实代次, 写入.事实代次}; 结果.实际结果 = *事实; return 结果;
        } catch (const std::bad_alloc&) { return 失败(L2结构状态::资源失败); }
        catch (...) { return 失败(L2结构状态::内部不一致); }
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
            const auto 当前代次 = L2任务结构内部::读取中性当前事实代次(L1_);
            if (当前代次 == 0) return 失败(L2结构状态::内部不一致);

            // 旧截止只允许原幂等身份的精确重复。首次规范化写集由 L1 保存，
            // 先以首次结果读回完整业务请求，再原样重放该写集。
            if (当前代次 != 请求.请求头.期望事实代次) {
                const auto 首次 = 第一层写入端口_.读取首次写入材料(
                    {L1所有者范围首次写入读取合同版本,
                        {请求.幂等身份.值}});
                if (首次.状态 != L1所有者范围读取状态::成功
                    || !首次.首次规范化写集 || !首次.首次写入结果)
                    return 失败(L2结构状态::事实代次漂移, 当前代次);
                const auto& 首次结果 = *首次.首次写入结果;
                const auto 状态编码 = L2任务结构内部::查找治理写入编码(
                    首次结果, L2任务结构内部::新治理状态节点写入本地键值);
                const auto 证据编码 = L2任务结构内部::查找治理写入编码(
                    首次结果, L2任务结构内部::新目标裁决证据节点写入本地键值);
                const auto 投影 = 状态编码
                    ? L2任务结构内部::读取任务治理状态历史投影(
                        L1_, 身份来源定位_, 类型定位_,
                        L2任务治理状态身份{*状态编码}, 首次结果.事实代次,
                        当前代次)
                    : std::nullopt;
                if (!投影 || !证据编码 || 投影->证据.身份.值 != *证据编码
                    || 投影->状态.任务 != 请求.任务
                    || 投影->状态.任务虚拟存在 != 请求.任务虚拟存在
                    || 投影->状态.生命周期发生时间 != 请求.生命周期发生时间
                    || 投影->状态.来源方法路径 != 请求.来源方法路径
                    || 投影->状态.来源实例方法 != 请求.来源实例方法
                    || 投影->状态.来源任务实际结果 != 请求.来源任务实际结果
                    || 投影->证据.材料 != 请求.目标裁决)
                    return 失败(L2结构状态::幂等冲突, 当前代次);
                const auto 重放 = 第一层写入端口_.提交所有者范围中性写集(
                    *首次.首次规范化写集);
                if (重放.状态 != L1所有者范围写入状态::精确重复
                    || 重放.事实代次 != 首次结果.事实代次
                    || 重放.新编码映射 != 首次结果.新编码映射)
                    return 失败(L2任务结构内部::映射任务写入状态(重放.状态),
                        重放.事实代次);
                L2新增执行结果待验证状态结果 结果;
                结果.结果头 = {L2结构合同版本, L2结构状态::精确重复,
                    首次结果.事实代次, 首次结果.事实代次};
                结果.已登记状态 = 投影->状态;
                结果.目标裁决证据 = 投影->证据;
                return 结果;
            }

            const auto Gc = 当前代次;
            const auto 任务 = L2任务结构内部::读取任务当前投影(
                L1_, 身份来源定位_, 类型定位_,
                {{L2结构合同版本, Gc}, L2读取类别::当前, 请求.任务}, Gc);
            const auto 虚拟 = L2任务结构内部::读取任务虚拟存在当前投影(
                L1_, 身份来源定位_,
                {{L2结构合同版本, Gc}, L2读取类别::当前, 请求.任务}, Gc);
            const auto 路径 = L2任务结构内部::读取任务方法路径当前投影(
                L1_, 身份来源定位_, 类型定位_,
                {{L2结构合同版本, Gc}, L2读取类别::当前,
                    请求.来源方法路径}, Gc);
            const auto 实例 = L2任务结构内部::读取实例方法当前投影(
                L1_, 身份来源定位_, 类型定位_, 请求.来源实例方法, Gc);
            const auto 实际结果 = L2任务结构内部::读取任务实际结果当前投影(
                L1_, 身份来源定位_, 类型定位_, 请求.来源任务实际结果, Gc);
            const auto 当前路径 = L2任务结构内部::读取当前选中路径当前投影(
                L1_, 身份来源定位_, 类型定位_,
                {{L2结构合同版本, Gc}, L2读取类别::当前, 请求.任务}, Gc);
            const auto 目标来源 = 需求结构服务_.读取需求列表项完整(
                {{L2结构合同版本, Gc}, L2读取类别::当前,
                    请求.目标裁决.任务目标来源, 0});
            if (!任务.成功() || !任务.任务 || !虚拟.成功() || !虚拟.任务虚拟存在
                || !路径.成功() || !路径.路径 || !实例 || !实际结果
                || !当前路径.成功() || !当前路径.当前选中路径
                || !目标来源.成功() || !目标来源.列表项)
                return 失败(L2结构状态::未找到, Gc);
            if (任务.任务->任务虚拟存在 != 请求.任务虚拟存在
                || 任务.任务->需求列表项 != 请求.目标裁决.任务目标来源
                || 虚拟.任务虚拟存在->身份 != 请求.任务虚拟存在
                || 虚拟.任务虚拟存在->任务 != 请求.任务
                || 路径.路径->任务 != 请求.任务
                || 路径.路径->身份 != 请求.来源方法路径
                || 路径.路径->来源方法 != 请求.目标裁决.方法
                || 路径.路径->完整路径材料.预期结果
                    != 请求.目标裁决.路径预期结果
                || 路径.路径->完整路径材料.验证合同
                    != 请求.目标裁决.路径验证合同
                || 实例->任务 != 请求.任务
                || 实例->身份 != 请求.来源实例方法
                || 实例->当前选中路径 != 请求.来源方法路径
                || 实例->执行轮次 != 请求.目标裁决.执行轮次
                || 实例->来源方法 != 请求.目标裁决.方法
                || *当前路径.当前选中路径 != 请求.来源方法路径
                || 实际结果->任务 != 请求.任务
                || 实际结果->实例方法 != 请求.来源实例方法
                || 实际结果->执行轮次 != 请求.目标裁决.执行轮次
                || 实际结果->方法 != 请求.目标裁决.方法
                || 实际结果->动作入口.值
                    != 路径.路径->完整路径材料.动作入口
                || 实际结果->实际状态 != 请求.目标裁决.实际状态
                || 实际结果->动态证据 != 请求.目标裁决.动态证据
                || 目标来源.列表项->目标宿主存在 != 请求.目标裁决.目标宿主
                || 目标来源.列表项->目标特征定义 != 请求.目标裁决.目标特征
                || 目标来源.列表项->目标状态合同 != 请求.目标裁决.目标状态合同)
                return 失败(L2结构状态::引用冲突, Gc);

            L1所有者范围一致当前读取请求 当前状态请求;
            当前状态请求.合同版本 = L1所有者范围一致当前读取合同版本;
            当前状态请求.期望事实代次 = Gc;
            当前状态请求.所有者 = {身份来源定位_.所有者};
            当前状态请求.源关系组 = {{请求.任务.值,
                类型定位_.任务当前治理状态关系类型}};
            const auto 当前状态 = L1_.尝试读取所有者范围一致当前投影(
                当前状态请求);
            if (当前状态.状态 != L1所有者范围一致当前读取状态::成功
                || 当前状态.读取事实代次 != Gc
                || 当前状态.源关系组.size() != 1)
                return 失败(L2结构状态::内部不一致, Gc);
            if (!当前状态.源关系组.front().成员.empty())
                return 失败(L2结构状态::引用冲突, Gc);

            const auto 写集 = L2任务结构内部::形成执行结果待验证首态写集(
                请求, 实际结果->场景, 身份来源定位_, 类型定位_);
            const auto 写入 = 第一层写入端口_.提交所有者范围中性写集(写集);
            if (写入.状态 != L1所有者范围写入状态::成功)
                return 失败(L2任务结构内部::映射任务写入状态(写入.状态),
                    写入.事实代次);
            const auto 状态编码 = L2任务结构内部::查找治理写入编码(
                写入, L2任务结构内部::新治理状态节点写入本地键值);
            const auto 证据编码 = L2任务结构内部::查找治理写入编码(
                写入, L2任务结构内部::新目标裁决证据节点写入本地键值);
            const auto 投影 = 状态编码
                ? L2任务结构内部::读取任务治理状态历史投影(
                    L1_, 身份来源定位_, 类型定位_,
                    L2任务治理状态身份{*状态编码}, 写入.事实代次,
                    写入.事实代次)
                : std::nullopt;
            if (!投影 || !证据编码 || 投影->证据.身份.值 != *证据编码)
                return 失败(L2结构状态::内部不一致, 写入.事实代次);
            L2新增执行结果待验证状态结果 结果;
            结果.结果头 = {L2结构合同版本, L2结构状态::已提交,
                写入.事实代次, 写入.事实代次};
            结果.已登记状态 = 投影->状态;
            结果.目标裁决证据 = 投影->证据;
            return 结果;
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
            const auto 当前代次 = L2任务结构内部::读取中性当前事实代次(L1_);
            if (当前代次 == 0) return 失败(L2结构状态::内部不一致);
            if (当前代次 != 请求.请求头.期望事实代次) {
                const auto 首次 = 第一层写入端口_.读取首次写入材料(
                    {L1所有者范围首次写入读取合同版本,
                        {请求.幂等身份.值}});
                if (首次.状态 != L1所有者范围读取状态::成功
                    || !首次.首次规范化写集 || !首次.首次写入结果)
                    return 失败(L2结构状态::事实代次漂移, 当前代次);
                const auto& 首次结果 = *首次.首次写入结果;
                const auto 状态编码 = L2任务结构内部::查找治理写入编码(
                    首次结果, L2任务结构内部::新治理状态节点写入本地键值);
                const auto 目标投影 = 状态编码
                    ? L2任务结构内部::读取任务治理状态历史投影(
                        L1_, 身份来源定位_, 类型定位_,
                        L2任务治理状态身份{*状态编码}, 首次结果.事实代次,
                        当前代次)
                    : std::nullopt;
                const auto 前态投影 = L2任务结构内部::读取任务治理状态历史投影(
                    L1_, 身份来源定位_, 类型定位_, 请求.前一治理状态,
                    请求.请求头.期望事实代次, 当前代次);
                if (!目标投影 || !前态投影
                    || 目标投影->状态.任务 != 请求.任务
                    || 目标投影->状态.任务虚拟存在 != 请求.任务虚拟存在
                    || 目标投影->状态.生命周期发生时间 != 请求.生命周期发生时间
                    || 目标投影->状态.前一治理状态 !=请求.前一治理状态
                    || 目标投影->状态.来源方法路径 != 请求.旧当前路径
                    || 目标投影->状态.来源实例方法 != 请求.旧当前实例
                    || 目标投影->证据.身份 != 请求.目标裁决证据)
                    return 失败(L2结构状态::幂等冲突, 当前代次);
                const auto 重放 = 第一层写入端口_.提交所有者范围中性写集(
                    *首次.首次规范化写集);
                if (重放.状态 != L1所有者范围写入状态::精确重复
                    || 重放.事实代次 != 首次结果.事实代次
                    || 重放.新编码映射 != 首次结果.新编码映射)
                    return 失败(L2任务结构内部::映射任务写入状态(重放.状态),
                        重放.事实代次);
                L2提交目标未达成待重筹办迁移结果 结果;
                结果.结果头 = {L2结构合同版本, L2结构状态::精确重复,
                    首次结果.事实代次, 首次结果.事实代次};
                结果.目标状态 = 目标投影->状态;
                结果.前驱状态 = 前态投影->状态;
                结果.目标裁决证据 = 目标投影->证据;
                const auto 旧路径读取 = L2任务结构内部::读取任务方法路径当前投影(
                    L1_, 身份来源定位_, 类型定位_,
                    {{L2结构合同版本, 当前代次}, L2读取类别::当前,
                        请求.旧当前路径}, 当前代次);
                结果.旧路径 = 旧路径读取.路径;
                结果.旧实例 = L2任务结构内部::读取实例方法当前投影(
                    L1_, 身份来源定位_, 类型定位_, 请求.旧当前实例,
                    当前代次);
                if (!结果.旧路径 || !结果.旧实例)
                    return 失败(L2结构状态::内部不一致, 当前代次);
                return 结果;
            }
            const auto Gs = 当前代次;
            const auto 前态 = L2任务结构内部::读取任务治理状态历史投影(
                L1_, 身份来源定位_, 类型定位_, 请求.前一治理状态, Gs, Gs);
            if (!前态) return 失败(L2结构状态::未找到, Gs);
            if (前态->状态.任务 != 请求.任务
                || 前态->状态.任务虚拟存在 != 请求.任务虚拟存在
                || 前态->证据.身份 != 请求.目标裁决证据
                || 前态->状态.业务状态段 != L2任务业务状态段::执行结果待验证
                || 前态->状态.目标达成状态 != L2任务目标达成状态::未达成
                || 前态->证据.材料.后继准备 !=
                    L2任务目标裁决后继准备::目标未达成待重筹办
                || 前态->状态.来源方法路径 != 请求.旧当前路径
                || 前态->状态.来源实例方法 != 请求.旧当前实例
                || 请求.生命周期发生时间 < 前态->状态.生命周期发生时间)
                return 失败(L2结构状态::引用冲突, Gs);
            const auto 实际结果 = L2任务结构内部::读取任务实际结果当前投影(
                L1_, 身份来源定位_, 类型定位_,
                前态->状态.来源任务实际结果, Gs);
            if (!实际结果 || 实际结果->任务 != 请求.任务
                || 实际结果->实例方法 != 前态->状态.来源实例方法
                || 实际结果->执行轮次 != 前态->状态.来源执行轮次)
                return 失败(L2结构状态::引用冲突, Gs);
            const auto 当前关系组 = L1_.读取所有者范围历史关系组(
                {L1所有者范围CRUD合同版本,
                    L1所有者范围关系端点方向::源, 请求.任务.值,
                    类型定位_.任务当前治理状态关系类型, Gs});
            if (当前关系组.状态 != L1所有者范围读取状态::成功
                || 当前关系组.读取事实代次 != Gs
                || 当前关系组.关系组.size() != 1)
                return 失败(L2结构状态::内部不一致, Gs);
            const auto& 当前关系 = 当前关系组.关系组.front();
            if (当前关系.目标节点 != 请求.前一治理状态.值
                || 当前关系.角色或顺序 !=
                    static_cast<std::int64_t>(前态->状态.生命周期版本))
                return 失败(L2结构状态::引用冲突, Gs);

            const auto 当前路径关系组 = L1_.读取所有者范围历史关系组(
                {L1所有者范围CRUD合同版本,
                    L1所有者范围关系端点方向::源, 请求.任务.值,
                    类型定位_.当前选中路径关系类型, Gs});
            const auto 当前实例关系组 = L1_.读取所有者范围历史关系组(
                {L1所有者范围CRUD合同版本,
                    L1所有者范围关系端点方向::源, 请求.任务.值,
                    类型定位_.实例方法当前关系类型, Gs});
            if (当前路径关系组.状态 != L1所有者范围读取状态::成功
                || 当前实例关系组.状态 != L1所有者范围读取状态::成功
                || 当前路径关系组.读取事实代次 != Gs
                || 当前实例关系组.读取事实代次 != Gs
                || 当前路径关系组.关系组.size() != 1
                || 当前实例关系组.关系组.size() != 1)
                return 失败(L2结构状态::内部不一致, Gs);
            const auto& 当前路径关系 = 当前路径关系组.关系组.front();
            const auto& 当前实例关系 = 当前实例关系组.关系组.front();
            if (当前路径关系.目标节点 != 请求.旧当前路径.值
                || 当前路径关系.角色或顺序 != 1
                || 当前实例关系.目标节点 != 请求.旧当前实例.值
                || 当前实例关系.角色或顺序 != 1)
                return 失败(L2结构状态::引用冲突, Gs);

            L2任务方法路径读取请求 旧路径请求{
                {L2结构合同版本, Gs}, L2读取类别::当前,
                请求.旧当前路径};
            const auto 旧路径 = L2任务结构内部::读取任务方法路径当前投影(
                L1_, 身份来源定位_, 类型定位_, 旧路径请求, Gs);
            const auto 旧实例 = L2任务结构内部::读取实例方法当前投影(
                L1_, 身份来源定位_, 类型定位_, 请求.旧当前实例, Gs);
            if (!旧路径.成功() || !旧路径.路径 || !旧实例
                || 旧路径.路径->任务 != 请求.任务
                || 旧实例->任务 != 请求.任务
                || 旧实例->当前选中路径 != 请求.旧当前路径)
                return 失败(L2结构状态::引用冲突, Gs);
            const auto 写集 = L2任务结构内部::形成目标未达成迁移写集(
                请求, 前态->状态, 前态->证据, 当前关系.编码,
                当前路径关系.编码, 当前实例关系.编码,
                身份来源定位_, 类型定位_);
            const auto 写入 = 第一层写入端口_.提交所有者范围中性写集(写集);
            if (写入.状态 != L1所有者范围写入状态::成功)
                return 失败(L2任务结构内部::映射任务写入状态(写入.状态),
                    写入.事实代次);
            const auto 状态编码 = L2任务结构内部::查找治理写入编码(
                写入, L2任务结构内部::新治理状态节点写入本地键值);
            const auto 目标投影 = 状态编码
                ? L2任务结构内部::读取任务治理状态历史投影(
                    L1_, 身份来源定位_, 类型定位_,
                    L2任务治理状态身份{*状态编码}, 写入.事实代次,
                    写入.事实代次)
                : std::nullopt;
            const auto 前态读回 = L2任务结构内部::读取任务治理状态历史投影(
                L1_, 身份来源定位_, 类型定位_, 请求.前一治理状态, Gs,
                写入.事实代次);
            if (!目标投影 || !前态读回)
                return 失败(L2结构状态::内部不一致, 写入.事实代次);
            L2提交目标未达成待重筹办迁移结果 结果;
            结果.结果头 = {L2结构合同版本, L2结构状态::已提交,
                写入.事实代次, 写入.事实代次};
            结果.目标状态 = 目标投影->状态;
            结果.前驱状态 = 前态读回->状态;
            结果.目标裁决证据 = 目标投影->证据;
            结果.旧路径 = 旧路径.路径;
            结果.旧实例 = 旧实例;
            return 结果;
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
    L2任务结构内部::任务子目标承接记录定位 子目标记录定位_;
    mutable std::mutex 互斥体_;
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
    结果.结果头.状态 = L2结构状态::许可拒绝;
    return 结果;
}

inline L2任务子目标承接记录写入结果
L2任务子目标承接记录写入端口::写入已验证绑定子需求(
    L2绑定任务子目标承接记录子需求请求 请求) noexcept {
    if (服务_)
        return 服务_->写入已验证绑定任务子目标承接记录子需求(
            std::move(请求));
    L2任务子目标承接记录写入结果 结果;
    结果.结果头.状态 = L2结构状态::许可拒绝;
    return 结果;
}

inline L2任务子目标承接记录写入结果
L2任务子目标承接记录写入端口::写入已验证登记回流(
    L2登记任务子目标承接记录回流请求 请求) noexcept {
    if (服务_)
        return 服务_->写入已验证登记任务子目标承接记录回流(
            std::move(请求));
    L2任务子目标承接记录写入结果 结果;
    结果.结果头.状态 = L2结构状态::许可拒绝;
    return 结果;
}

inline L2任务子目标承接记录写入结果
L2任务子目标承接记录写入端口::写入已验证退出记录(
    L2退出任务子目标承接记录请求 请求) noexcept {
    if (服务_)
        return 服务_->写入已验证退出任务子目标承接记录(std::move(请求));
    L2任务子目标承接记录写入结果 结果;
    结果.结果头.状态 = L2结构状态::许可拒绝;
    return 结果;
}

} // namespace 海中鱼巣
