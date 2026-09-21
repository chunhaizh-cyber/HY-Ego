#pragma once

#include "数据服务.绑定存在.h"
#include "数据服务.存在类.h"
#include "数据服务.场景类.h"

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "../核心/容错检查.h"

#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <limits>
#include <map>
#include <mutex>
#include <numeric>
#include <optional>
#include <set>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>


#include "合同.相关概念添加参与.h"

namespace 海中鱼巣 {

class 特征值域比较数据服务;
struct 特征值域比较预算_v1;

enum class 概念树数据状态 : std::uint8_t {
    已创建 = 1,
    精确重复,
    已读取,
    已添加支持,
    已退出支持,
    已添加上位,
    已替换上位,
    已退出上位,
    已迁移生命周期,
    已退出概念,
    已释放引用,
    无须变更,
    入口拒绝,
    未找到,
    目标已退出,
    引用冲突,
    形成环,
    根不可退出,
    事实代次漂移,
    幂等冲突,
    数量预算不足,
    历史材料不可用,
    资源失败,
    内部不一致,
    已可能发布,
    已创建共享定义 = 26, 已绑定共享名称, 已退出共享名称, 已记录共享用途, 不支持, 差异不可表示,
    已登记类型观察 = 32, 已登记特征概念命中 = 33, 已变更特征概念组织 = 34,
    旧格式不支持 = 35, 规则缺失 = 36, 类型不相容 = 37, 前次写入待收敛 = 38
};
struct 概念树截止结果 final {
    std::uint32_t 合同版本 = 1;
    概念树数据状态 状态 = 概念树数据状态::入口拒绝;
    std::uint64_t Gread = 0;
    bool 成功() const noexcept {
        return 合同版本 == 1 && 状态 == 概念树数据状态::已读取 && Gread;
    }
};
struct 特征概念出生使用结构交付 final {
    稳定编码 锚点{};
    std::uint32_t 格式 = 1;
    稳定编码 F到FCv出生使用关系类型{};
    friend bool operator==(const 特征概念出生使用结构交付 &,
                           const 特征概念出生使用结构交付 &) = default;
};
struct 特征概念出生使用结构登记请求 final {
    std::uint32_t 版本 = 1;
    std::uint64_t G0 = 0;
    L1所有者范围写入幂等身份 幂等键{};
    纯概念结构交付_v2 纯概念结构{};
    std::uint64_t 最大首次材料项数 = 0;
    friend bool operator==(const 特征概念出生使用结构登记请求 &,
                           const 特征概念出生使用结构登记请求 &) = default;
};
struct 特征概念出生使用结构首次材料读回 final {
    L1所有者范围节点事实 锚点;
    L1所有者范围节点事实 F到FCv出生使用关系类型;
    L1所有者范围关系事实 类型登记关系;
    L1所有者范围值事实 格式值;
    friend bool operator==(const 特征概念出生使用结构首次材料读回 &,
                           const 特征概念出生使用结构首次材料读回 &) = default;
};
struct 特征概念出生使用结构登记结果 final {
    std::uint32_t 版本 = 1;
    纯概念状态 状态 = 纯概念状态::入口拒绝;
    纯概念发布状态 发布 = 纯概念发布状态::未进入;
    std::uint64_t Gread = 0;
    std::optional<std::uint64_t> 首次发布H;
    std::optional<特征概念出生使用结构登记请求> 原请求;
    std::optional<特征概念出生使用结构交付> 交付;
    std::optional<特征概念出生使用结构首次材料读回> 首次材料;
    bool 成功(const 特征概念出生使用结构登记请求 &) const noexcept;
};
enum class 特征概念值域基础读取状态_v1 : std::uint8_t {
    已读取 = 1, 未找到, 目标已退出, 类别冲突, 规则缺失, 未实现,
    事实代次漂移, 数量预算不足, 历史材料不可用, 资源失败,
    内部不一致, 入口拒绝
};
struct 特征概念值域基础读取请求_v1 final {
    std::uint32_t 版本 = 1;
    std::uint64_t Gread = 0, H = 0;
    概念树概念身份 FC;
    概念树预算 预算;
};
struct 特征概念值域基础事实_v1 final {
    概念树概念身份 FC;
    特征类型身份 FT;
    特征值表示类型 表示 = 特征值表示类型::I64;
    std::optional<特征规范I64域> I64域;
    概念树生命周期 生命周期;
    std::uint64_t Gread = 0, H = 0;
};
struct 特征概念值域基础读取结果_v1 final {
    std::uint32_t 版本 = 1;
    特征概念值域基础读取状态_v1 状态 = 特征概念值域基础读取状态_v1::入口拒绝;
    std::uint64_t Gread = 0, H = 0;
    std::optional<特征概念值域基础事实_v1> 事实;
    bool 成功(const 特征概念值域基础读取请求_v1&) const noexcept;
};

class 概念树类数据服务 final : public 相关概念添加参与者,
                              public 已发布概念引用参与者,
                              public 原子I64特征概念参与者 {
    using S = 概念树数据状态;
    using N = L1所有者范围节点事实;
    using E = L1所有者范围关系事实;
    using V = L1所有者范围值事实;
    using Ref = L1所有者范围事实引用;
    using Key = L1所有者范围写集本地键;
    struct 失败 {
        S 状态;
    };
    struct 相关失败 {
        相关概念参与状态 状态;
    };
    struct 纯失败 {
        纯概念状态 状态;
    };

  public:
    static 纯概念结构登记结果_v2 登记纯概念结构_v2(
        const L1事实基座服务 &, L1所有者范围写端口 &,
        const 纯概念结构登记请求_v2 &) noexcept;
    static 特征概念出生使用结构登记结果 登记特征概念出生使用结构(
        const L1事实基座服务 &, L1所有者范围写端口 &,
        const 特征概念出生使用结构登记请求 &) noexcept;
    static 存在概念两组结构登记结果_v1 登记存在概念两组结构_v1(
        const L1事实基座服务 &, L1所有者范围写端口 &,
        const 存在概念两组结构登记请求_v1 &) noexcept;
    概念树类数据服务(const L1事实基座服务 &, const 特征类数据服务 &,
                     const 存在类数据服务 &, const 特征值类数据服务 &,
                     const 场景类数据服务 &, L1所有者范围写端口 &&,
                     const 纯概念结构交付_v2 &,
                     const 特征概念出生使用结构交付 &,
                     const 存在概念两组结构交付_v1 &);
    bool 绑定于(const L1事实基座服务 &x) const noexcept override { return &l1_ == &x; }
    相关概念参与片段 准备相关概念片段(const 相关概念参与请求 &,
                                         std::uint64_t Gread,
                                         L1有限N分区原子参与者身份_v3) const noexcept override;
    相关概念组合提交结果 提交相关概念组合事务(
        const 相关概念组合提交请求 &,
        std::span<L1所有者范围写端口 *const>) noexcept override;
    相关概念参与读回 读取相关概念结果(const 相关概念参与请求 &,
                                         std::uint64_t Gread,
                                         std::uint64_t H) const noexcept override;
    概念树类数据服务(const L1事实基座服务 &, const 特征类数据服务 &,
                     const 存在类数据服务 &, const 特征值类数据服务 &,
                     const 场景类数据服务 &, L1所有者范围写端口 &&,
                     const 相关概念结构交付 &);
    概念树截止结果 读取当前事实代次() const;
    存在概念两组规范化结果_v1 规范化存在概念两组定义(
        const 存在概念两组规范化请求_v1&, const 特征值域比较数据服务&,
        const 特征值域比较预算_v1&) const noexcept;
    存在概念两组枚举结果_v1 枚举存在概念候选(
        const 存在概念两组枚举请求_v1&, const 特征值域比较数据服务&,
        const 特征值域比较预算_v1&) const noexcept;
    存在概念两组查询结果_v1 精确查询存在概念(
        const 存在概念两组查询请求_v1&, const 特征值域比较数据服务&,
        const 特征值域比较预算_v1&) const noexcept;
    存在概念两组写入结果_v1 创建或复用存在概念(
        const 存在概念两组创建请求_v1&, const 特征值域比较数据服务&,
        const 特征值域比较预算_v1&) noexcept;
    存在概念两组读取结果_v1 读取存在概念两组定义(
        const 存在概念两组读取请求_v1&, const 特征值域比较数据服务&,
        const 特征值域比较预算_v1&) const noexcept;
    纯概念查询结果 精确查询纯概念(const 纯概念查询请求&) const noexcept;
    I64特征概念组织读取结果 读取当前I64特征概念(
        const I64特征概念组织读取请求&) const noexcept;
    特征概念值域基础读取结果_v1 读取特征概念值域基础(
        const 特征概念值域基础读取请求_v1&) const noexcept;
    纯概念写入结果 创建或复用纯概念(const 纯概念创建请求&) noexcept;
    纯概念创建恢复结果 读取纯概念创建首次结果(
        const 纯概念创建恢复请求&) const noexcept;
    纯概念读取结果 读取纯概念(const 纯概念读取请求&) const noexcept;
    纯概念生命周期结果 迁移纯概念生命周期(
        const 纯概念生命周期请求&) noexcept;
    纯概念退出结果 退出无引用叶概念(const 纯概念退出请求&) noexcept;
    存在概念使用读取结果 读取存在概念使用(
        const 存在概念使用读取请求&) const noexcept override;
    const L1事实基座服务& 原子I64底座() const noexcept override;
    L1所有者范围写端口& 原子I64端口() noexcept override;
    bool 原子I64结构已就绪() const noexcept override;
    原子I64特征参与结果<L1有限N分区原子参与者写集_v3>
    准备原子I64出生片段(const 原子I64特征出生请求&,std::uint64_t) const override;
    原子I64特征出生使用读取结果 读取原子I64出生使用(
        const 原子I64特征出生使用读取请求&) const override;
    原子I64特征参与结果<L1有限N分区原子参与者写集_v3>
    准备原子I64出生使用退出片段(
        const 原子I64特征出生使用退出片段请求&,std::uint64_t) const override;


    概念树类数据服务() = delete;
    概念树类数据服务(const 概念树类数据服务 &) = delete;
    概念树类数据服务 &operator=(const 概念树类数据服务 &) = delete;
    概念树类数据服务(概念树类数据服务 &&) = delete;
    概念树类数据服务 &operator=(概念树类数据服务 &&) = delete;

  private:
    const L1事实基座服务 &l1_;
    const 特征类数据服务 &features_;
    const 存在类数据服务 &existences_;
    const 特征值类数据服务 &values_;
    L1所有者范围写端口 port_;
    const 场景类数据服务 *scenes_ = nullptr;
    std::optional<相关概念结构交付> related_layout_;
    std::optional<纯概念结构交付_v2> pure_layout_;
    std::optional<特征概念出生使用结构交付> feature_birth_layout_;
    std::optional<存在概念两组结构交付_v1> two_group_definition_layout_;
    纯概念定义 规范化纯概念定义(const 纯概念定义&, std::uint64_t,
                              std::uint64_t, const 概念树预算&) const;
    纯概念事实 读取纯概念内部(概念树概念身份, std::uint64_t,
                            std::uint64_t, const 概念树预算&) const;
    void 核验纯概念无保护引用(const 纯概念事实&, std::uint64_t,
                               const 概念树预算&) const;
    L1所有者范围写集请求 形成纯概念写集(
        const 纯概念创建请求&, const 纯概念定义&) const;
    存在概念两组定义_v3 规范化两组定义内部(
        const 存在概念两组定义_v3&, std::uint64_t, std::uint64_t,
        const 存在概念两组预算_v3&, const 特征值域比较数据服务&,
        const 特征值域比较预算_v1&) const;
    存在概念两组事实_v3 读取两组定义内部(
        概念树概念身份, std::uint64_t, std::uint64_t,
        const 存在概念两组预算_v3&, const 特征值域比较数据服务&,
        const 特征值域比较预算_v1&) const;
    L1所有者范围写端口& 借用存在概念引用写端口() noexcept override {
        return port_;
    }
    已发布概念引用片段结果 准备存在概念引用片段(
        const 已发布存在概念引用准备请求&, std::uint64_t) const noexcept override;
    已发布概念引用首次材料结果 读取存在概念引用首次材料(
        L1所有者范围写入幂等身份) const noexcept override;
    void 校验相关概念布局(std::uint64_t) const;
    struct 相关概念展开 final {
        概念树概念身份 概念;
        相关概念类别 类别 = 相关概念类别::存在;
        概念树定义 定义;
        std::vector<概念树直接上位事实> 直接上位;
        std::vector<概念树形成引用事实> 挂接;
        概念树生命周期 生命周期;
    };
    相关概念展开 展开相关概念(概念树概念身份, std::uint64_t,
                                  std::uint64_t, const 相关概念读取预算 &) const;
    相关概念完整事实 读取相关概念内部(const 相关概念参与请求 &,
                                      概念树概念身份, std::uint64_t,
                                      std::uint64_t) const;
    相关概念参与状态 映射相关状态(S) const noexcept;
    void 验证相关世界(const 相关概念世界挂靠 &, std::uint64_t,
                      const 相关概念读取预算 &) const;
    L1有限N分区原子参与者写集_v3 形成相关概念写集(
        const 相关概念参与请求 &, std::uint64_t,
        L1有限N分区原子参与者身份_v3,
        const std::optional<相关概念完整事实> &) const;

    static S 共享映射(特征数据错误);
    template<class T> static T 特征结果(特征数据结果<T> r) {
        if (auto* e = std::get_if<特征数据错误>(&r)) throw 失败{共享映射(*e)};
        return std::get<T>(std::move(r));
    }
    void 验证世界(const 概念树形成世界引用 &, std::uint64_t, std::uint64_t,
                  const 概念树预算 &) const;
    void 核验外部节点(稳定编码, std::uint64_t, std::uint64_t) const;
    mutable std::mutex mutex_;
    void 守卫代次(std::uint64_t Gread) const;
    概念树定义 规范化定义(const 概念树定义 &) const;
    std::uint64_t 当前代次() const;
    N 节点(稳定编码, std::uint64_t g, std::uint64_t h) const;
    L1所有者范围事实副本 原始事实(稳定编码, std::uint64_t g) const;
    std::vector<E> 关系(稳定编码 端点, 稳定编码 类型, bool 入边, std::uint64_t g, std::uint64_t h,
                        std::uint64_t 预算) const;
    std::vector<V> 属性(稳定编码, std::uint64_t g, std::uint64_t h) const;
    static const V &唯一属性(const std::vector<V> &, 稳定编码);
    static void 预算有效(const 概念树预算 &);
    static void 检查数量(std::size_t n, std::uint64_t max);
    static S 映射(L1所有者范围读取状态);
    static S 映射(存在历史读取状态);
    static bool 编码小于(稳定编码 a, 稳定编码 b) {
        return a.值 < b.值;
    }
    static bool 身份小于(概念树概念身份 a, 概念树概念身份 b) {
        return 编码小于(a.值, b.值);
    }
    template <class W> static 稳定编码 世界编码(const W &w) {
        return std::visit([](const auto &v) { return v.值; }, w);
    }
    template <class T> static 概念树生命周期 投影生命(const T &v, std::uint64_t h) {
        return {v.创建事实代次,
                v.退出事实代次 && *v.退出事实代次 <= h ? v.退出事实代次 : std::nullopt};
    }
    template <class T> static 概念树生命周期 投影纯生命(const T &v, std::uint64_t g) {
        return {v.创建事实代次, v.退出事实代次 && *v.退出事实代次 <= g ? v.退出事实代次 : std::nullopt};
    }
    template <class T> static bool 有效于(const T &v, std::uint64_t h) {
        return v.创建事实代次 && v.创建事实代次 <= h && (!v.退出事实代次 || h < *v.退出事实代次);
    }
};

} // namespace 海中鱼巣
