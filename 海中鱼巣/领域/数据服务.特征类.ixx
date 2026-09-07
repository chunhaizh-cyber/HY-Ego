module;

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <mutex>
#include <new>
#include <numeric>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

export module 海中鱼巣.领域.数据服务.特征类;

export import 海中鱼巣.领域.数据服务.特征值类;

export namespace 海中鱼巣 {

struct 特征信息身份 final {
    稳定编码 编码{};
    friend bool operator==(const 特征信息身份&,
        const 特征信息身份&) = default;
};

inline bool 有效(特征信息身份 身份) noexcept {
    return 有效(身份.编码);
}

struct 特征类型身份 final {
    稳定编码 编码{};
    friend bool operator==(const 特征类型身份&,
        const 特征类型身份&) = default;
};

inline bool 有效(特征类型身份 身份) noexcept {
    return 有效(身份.编码);
}

// 活动分支就是承载方式；不另存可能漂移的表示枚举。
using 特征观察值 = std::variant<std::int64_t, 特征值身份>;

struct 特征值域项标识 final {
    std::uint64_t 值{};
    friend bool operator==(const 特征值域项标识&,
        const 特征值域项标识&) = default;
};

inline bool 有效(特征值域项标识 标识) noexcept {
    return 标识.值 != 0;
}

struct 特征I64闭区间 final {
    std::int64_t 下界{};
    std::int64_t 上界{};
    friend bool operator==(const 特征I64闭区间&,
        const 特征I64闭区间&) = default;
};

struct 特征比较规则身份 final {
    稳定编码 编码{};
    friend bool operator==(const 特征比较规则身份&,
        const 特征比较规则身份&) = default;
};

inline bool 有效(特征比较规则身份 身份) noexcept {
    return 有效(身份.编码);
}

struct 特征结构域 final {
    特征比较规则身份 正式比较规则{};
    特征I64闭区间 允许差异范围{};
    friend bool operator==(const 特征结构域&,
        const 特征结构域&) = default;
};

// 活动分支承载值域表示；值域表示不是第五个物理成员。
using 特征值域范围 = std::variant<特征I64闭区间, 特征结构域>;

enum class 特征值域表示 : std::uint8_t {
    I64区间 = 1,
    结构域 = 2
};

struct 特征值域项 final {
    特征值域项标识 标识{};
    特征观察值 代表值{};
    特征值域范围 范围{};
    std::uint64_t 命中次数{};
    friend bool operator==(const 特征值域项&,
        const 特征值域项&) = default;
};

// 历史成员显式区分具体值和本特征信息内的值域项引用。
using 特征历史值项 = std::variant<特征观察值, 特征值域项标识>;

struct 特征信息 final {
    特征信息身份 身份{};
    特征类型身份 类型{};
    特征观察值 准确当前观察值{};
    std::vector<特征历史值项> 历史值集合;
    std::vector<特征值域项> 值域集合;
    friend bool operator==(const 特征信息&,
        const 特征信息&) = default;
};

// 以下函数只校验无需外部服务即可确定的浅层结构。值引用的完整内容、
// 正式类型和比较规则仍须由后续组合数据操作在同一事实截止内读回确认。
inline bool 浅层结构有效(const 特征观察值& 值) noexcept {
    if (值.valueless_by_exception()) return false;
    const auto* 引用 = std::get_if<特征值身份>(&值);
    return 引用 == nullptr || 有效(*引用);
}

inline bool 浅层结构有效(特征I64闭区间 范围) noexcept {
    return 范围.下界 <= 范围.上界;
}

inline bool 浅层结构有效(const 特征结构域& 范围) noexcept {
    return 有效(范围.正式比较规则)
        && 浅层结构有效(范围.允许差异范围);
}

inline bool 浅层结构有效(const 特征值域范围& 范围) noexcept {
    if (范围.valueless_by_exception()) return false;
    return std::visit([](const auto& 内容) noexcept {
        return 浅层结构有效(内容);
    }, 范围);
}

inline bool 浅层结构有效(const 特征值域项& 值域) noexcept {
    if (!有效(值域.标识) || !浅层结构有效(值域.代表值)
        || !浅层结构有效(值域.范围))
        return false;

    const auto* 区间 = std::get_if<特征I64闭区间>(&值域.范围);
    const auto* 代表整数 = std::get_if<std::int64_t>(&值域.代表值);
    return 区间 == nullptr || 代表整数 == nullptr
        || (*代表整数 >= 区间->下界 && *代表整数 <= 区间->上界);
}

inline bool 浅层结构有效(const 特征信息& 信息) noexcept {
    if (!有效(信息.身份) || !有效(信息.类型)
        || !浅层结构有效(信息.准确当前观察值))
        return false;

    for (std::size_t i = 0; i < 信息.值域集合.size(); ++i) {
        const auto& 当前域 = 信息.值域集合[i];
        if (!浅层结构有效(当前域)) return false;
        for (std::size_t j = 0; j < i; ++j)
            if (信息.值域集合[j].标识 == 当前域.标识) return false;
    }

    for (std::size_t i = 0; i < 信息.历史值集合.size(); ++i) {
        const auto& 历史项 = 信息.历史值集合[i];
        if (历史项.valueless_by_exception()) return false;
        if (const auto* 具体值 = std::get_if<特征观察值>(&历史项)) {
            if (!浅层结构有效(*具体值)) return false;
            continue;
        }

        const auto* 标识项 = std::get_if<特征值域项标识>(&历史项);
        if (标识项 == nullptr) return false;
        const auto 标识 = *标识项;
        if (!有效(标识)) return false;
        bool 属于本信息 = false;
        for (const auto& 值域 : 信息.值域集合)
            if (值域.标识 == 标识) {
                属于本信息 = true;
                break;
            }
        if (!属于本信息) return false;
        for (std::size_t j = 0; j < i; ++j) {
            const auto* 已有标识 = std::get_if<特征值域项标识>(
                &信息.历史值集合[j]);
            if (已有标识 != nullptr && *已有标识 == 标识) return false;
        }
    }
    return true;
}

inline 特征信息身份 获取特征信息身份(const 特征信息& 信息) noexcept {
    return 信息.身份;
}

inline 特征类型身份 获取特征类型(const 特征信息& 信息) noexcept {
    return 信息.类型;
}

inline const 特征观察值& 获取准确当前观察值(
    const 特征信息& 信息) noexcept {
    return 信息.准确当前观察值;
}

inline const std::vector<特征历史值项>& 获取历史值集合(
    const 特征信息& 信息) noexcept {
    return 信息.历史值集合;
}

inline const std::vector<特征值域项>& 获取值域集合(
    const 特征信息& 信息) noexcept {
    return 信息.值域集合;
}

inline 特征值域项标识 获取值域项标识(
    const 特征值域项& 值域) noexcept {
    // 返回值仅在所属特征信息身份的上下文内有意义。
    return 值域.标识;
}

inline const 特征观察值& 获取值域代表值(
    const 特征值域项& 值域) noexcept {
    return 值域.代表值;
}

inline 特征值域表示 获取值域表示(
    const 特征值域项& 值域) noexcept {
    return std::holds_alternative<特征I64闭区间>(值域.范围)
        ? 特征值域表示::I64区间 : 特征值域表示::结构域;
}

inline const 特征值域范围& 获取值域范围(
    const 特征值域项& 值域) noexcept {
    return 值域.范围;
}

inline std::uint64_t 获取值域命中次数(
    const 特征值域项& 值域) noexcept {
    return 值域.命中次数;
}

enum class 特征数据错误 : std::uint8_t {
    未找到, 未设置, 入口拒绝, 类型不相容, 能力未提供, 规则缺失,
    引用冲突, 算术不可表示, 并发变化, 资源失败, 内部不一致,
    发布结果未确认, 前次写入待收敛
};

template<class 类型>
using 特征数据结果 = std::variant<类型, 特征数据错误>;

struct 先天I64特征类型规格 final {
    稳定编码 外设提供者{};
    稳定编码 单位{};
    std::uint64_t 缩放分子{};
    std::uint64_t 缩放分母{};
    std::vector<特征I64闭区间> 允许集合;
    std::int64_t 允许误差{};
    稳定编码 误差参数来源{};
    friend bool operator==(const 先天I64特征类型规格&,
        const 先天I64特征类型规格&) = default;
};

struct 先天I64特征类型信息 final {
    特征类型身份 身份{};
    先天I64特征类型规格 规格;
    特征比较规则身份 规则{};
    friend bool operator==(const 先天I64特征类型信息&,
        const 先天I64特征类型信息&) = default;
};

// 此类只组合 L1 公共合同和特征值类。构造的写端口必须由合法装配方
// 新建并专属交付本类，或为本类原分区重签发；来源真实性也由装配方保证。
// 普通数据 API 不接收 owner、代次、幂等或宿主。全部诊断向上送出。
class 特征类数据服务 final {
    template<class 类型> using 结果 = 特征数据结果<类型>;
    using 错误 = 特征数据错误;
    using 引用 = L1所有者范围事实引用;
    using 本地键 = L1所有者范围写集本地键;
    using 写集 = L1所有者范围写集请求;
    using 投影 = L1所有者范围一致当前读取结果;
    using 读取项状态 = L1所有者范围一致当前读取项目状态;

    enum 结构位置 : std::size_t {
        类型规格属性, 规则误差属性, 当前值属性, 历史集合属性, 域集合属性,
        外设来源关系, 单位关系, 域规则关系, 参数来源关系, 特征类型关系,
        细分关系, 结构数量
    };

    struct 节点副本 final {
        特征信息身份 身份;
        std::optional<特征类型身份> 类型;
        std::optional<std::int64_t> 当前;
        std::vector<特征历史值项> 历史;
        std::vector<特征值域项> 域;
        std::optional<稳定编码> 当前载荷;
        稳定编码 历史载荷;
        稳定编码 域载荷;
        std::optional<L1所有者范围关系事实> 类型关联;
        std::vector<L1所有者范围关系事实> 子关联;
        std::vector<L1所有者范围关系事实> 父关联;
    };

    struct 待确认写入 final {
        写集 请求;
        bool 初始化结构 = false;
    };

public:
    特征类数据服务(const L1事实基座服务& 第一层服务,
        L1所有者范围写端口&& 写端口, const 特征值类数据服务& 值服务,
        稳定编码 真实输入生产者)
        : 第一层服务_(第一层服务), 写端口_(std::move(写端口)),
          值服务_(值服务), 输入生产者_(真实输入生产者) {}

    特征类数据服务(const 特征类数据服务&) = delete;
    特征类数据服务& operator=(const 特征类数据服务&) = delete;
    特征类数据服务(特征类数据服务&&) = delete;
    特征类数据服务& operator=(特征类数据服务&&) = delete;

    结果<std::monostate> 初始化();
    结果<std::monostate> 收敛待确认写入();
    结果<特征类型身份> 创建先天I64特征类型(const 先天I64特征类型规格&);
    结果<先天I64特征类型信息> 读取先天I64特征类型(特征类型身份) const;
    结果<特征信息身份> 创建特征信息(特征类型身份, const 特征观察值&);
    结果<特征信息> 读取特征信息(特征信息身份) const;
    结果<特征信息> 修改特征信息(特征信息身份, 特征类型身份,
        const 特征观察值&);
    结果<std::monostate> 删除特征信息(特征信息身份);
    结果<std::vector<特征信息>> 查询特征信息(特征类型身份,
        const 特征观察值&) const;
    结果<特征类型身份> 读取特征类型(特征信息身份) const;
    结果<特征类型身份> 设置特征类型(特征信息身份, 特征类型身份);
    结果<std::monostate> 移除特征类型(特征信息身份);
    结果<特征观察值> 读取准确当前观察值(特征信息身份) const;
    结果<特征信息> 接收观察值(特征信息身份, const 特征观察值&);
    结果<std::monostate> 移除准确当前观察值(特征信息身份);
    结果<std::vector<特征历史值项>> 读取历史值集合(特征信息身份) const;
    结果<std::monostate> 添加历史值项(特征信息身份, const 特征历史值项&);
    结果<std::monostate> 修改历史值项(特征信息身份,
        const 特征历史值项&, const 特征历史值项&);
    结果<std::monostate> 删除历史值项(特征信息身份, const 特征历史值项&);
    结果<bool> 查询历史值项(特征信息身份, const 特征历史值项&) const;
    结果<std::vector<特征值域项>> 读取值域集合(特征信息身份) const;
    结果<特征值域项> 读取值域项(特征信息身份, 特征值域项标识) const;
    结果<特征值域项> 添加值域项(特征信息身份,
        const 特征观察值&, const 特征值域范围&);
    结果<特征值域项> 修改值域项(特征信息身份, 特征值域项标识,
        const 特征观察值&, const 特征值域范围&);
    结果<std::monostate> 删除值域项(特征信息身份, 特征值域项标识);
    结果<特征观察值> 读取值域代表值(特征信息身份, 特征值域项标识) const;
    结果<特征值域项> 修改值域代表值(特征信息身份, 特征值域项标识,
        const 特征观察值&);
    结果<特征值域范围> 读取值域范围(特征信息身份, 特征值域项标识) const;
    结果<特征值域项> 修改值域范围(特征信息身份, 特征值域项标识,
        const 特征值域范围&);
    结果<std::uint64_t> 读取值域命中次数(特征信息身份, 特征值域项标识) const;
    结果<std::vector<特征信息身份>> 读取细分子节点(特征信息身份) const;
    结果<特征信息身份> 读取细分父节点(特征信息身份) const;
    结果<特征信息身份> 读取值域生成子节点(特征信息身份,
        特征值域项标识) const;

private:
    // 同实例入口串行，读取不维护隐式事实。内部 helper 不再取锁。
    template<class 类型, class 操作>
    结果<类型> 保护执行(操作&& 函数) const {
        static_assert(std::is_nothrow_move_constructible_v<类型>);
        try {
            std::lock_guard<std::mutex> 锁(入口锁_);
            return 结果<类型>{std::in_place_index<0>, 函数()};
        } catch (错误 状态) {
            return 状态;
        } catch (const std::bad_alloc&) {
            return 错误::资源失败;
        } catch (const std::length_error&) {
            return 错误::资源失败;
        } catch (...) {
            return 错误::内部不一致;
        }
    }

    static void 要求(bool 条件, 错误 状态 = 错误::内部不一致) {
        if (!条件) throw 状态;
    }
    static void 检查读取状态(L1所有者范围一致当前读取状态 状态) {
        switch (状态) {
        case L1所有者范围一致当前读取状态::成功: return;
        case L1所有者范围一致当前读取状态::事实代次漂移: throw 错误::并发变化;
        case L1所有者范围一致当前读取状态::资源失败: throw 错误::资源失败;
        case L1所有者范围一致当前读取状态::入口拒绝: throw 错误::入口拒绝;
        default: throw 错误::内部不一致;
        }
    }
    static void 检查项目状态(读取项状态 状态) {
        if (状态 == 读取项状态::成功) return;
        if (状态 == 读取项状态::未找到 || 状态 == 读取项状态::已退出)
            throw 错误::未找到;
        if (状态 == 读取项状态::属性未设置) throw 错误::未设置;
        if (状态 == 读取项状态::种类不匹配) throw 错误::类型不相容;
        throw 错误::内部不一致;
    }

    投影 取得投影(L1所有者范围一致当前读取请求 请求, std::uint64_t& 截止) const {
        请求.期望事实代次 = 截止;
        auto 读取 = 第一层服务_.尝试读取所有者范围一致当前投影(请求);
        检查读取状态(读取.状态);
        要求(读取.合同版本 == L1所有者范围一致当前读取合同版本
            && 读取.期望事实代次 == 截止 && 读取.读取事实代次 != 0
            && (截止 == 0 || 读取.读取事实代次 == 截止)
            && 读取.所有者.size() == 请求.所有者.size()
            && 读取.节点.size() == 请求.节点.size()
            && 读取.关系.size() == 请求.关系.size()
            && 读取.值.size() == 请求.值.size()
            && 读取.属性值.size() == 请求.属性值.size()
            && 读取.源关系组.size() == 请求.源关系组.size()
            && 读取.目标关系组.size() == 请求.目标关系组.size());
        截止 = 读取.读取事实代次;
        return 读取;
    }

    std::uint64_t 开始读取(bool 需要初始化 = true) const {
        要求(写端口_.绑定于(第一层服务_) && 值服务_.绑定于(第一层服务_));
        要求(有效(输入生产者_), 错误::入口拒绝);
        if (需要初始化) 要求(已初始化_, 错误::未设置);
        L1所有者范围一致当前读取请求 请求;
        请求.所有者.push_back(写端口_.所有者身份());
        请求.节点.push_back(输入生产者_);
        std::uint64_t 截止 = 0;
        const auto 读取 = 取得投影(std::move(请求), 截止);
        检查项目状态(读取.所有者.front().状态);
        要求(读取.所有者.front().所有者事实.has_value()
            && 读取.所有者.front().所有者事实->所有者 == 写端口_.所有者身份()
            && 读取.所有者.front().所有者事实->范围种类
                == L1所有者范围种类::独占结构范围
            && !读取.所有者.front().所有者事实->退出事实代次);
        检查项目状态(读取.节点.front().状态);
        要求(读取.节点.front().事实.has_value()
            && 读取.节点.front().事实->编码 == 输入生产者_
            && !读取.节点.front().事实->退出事实代次);
        return 截止;
    }

    std::uint64_t 开始写入(bool 需要初始化 = true) const {
        要求(!待确认_, 错误::前次写入待收敛);
        return 开始读取(需要初始化);
    }

    static 本地键 分配本地键(const 写集& 请求) {
        const auto 数量 = 请求.节点.size() + 请求.关系.size() + 请求.值.size();
        要求(数量 < std::numeric_limits<std::uint32_t>::max(), 错误::资源失败);
        return {static_cast<std::uint32_t>(数量 + 1)};
    }

    static 本地键 加节点(写集& 请求,
        std::optional<L1所有者范围值表示种类> 表示 = std::nullopt) {
        const auto 键 = 分配本地键(请求);
        请求.节点.push_back({键, 表示 ? 节点种类::属性类型 : 节点种类::普通, 表示});
        return 键;
    }

    void 加关系(写集& 请求, 引用 源, 引用 目标,
        结构位置 类型, std::int64_t 角色 = 0) const {
        请求.关系.push_back({分配本地键(请求), std::move(源), std::move(目标),
            结构_[类型], 角色});
    }

    void 写字段(写集& 请求, 引用 所属, 结构位置 属性,
        L1所有者范围原始值材料 材料, std::optional<稳定编码> 旧载荷 = std::nullopt) const {
        const auto 键 = 分配本地键(请求);
        请求.值.push_back({键, 所属, 结构_[属性], std::move(材料), 输入生产者_});
        请求.属性槽变更.push_back({std::move(所属), 结构_[属性], 键});
        if (旧载荷) 请求.退出事实.push_back(*旧载荷);
    }

    static 写集 新写集(std::uint64_t 截止) {
        要求(截止 > 0 && 截止 < std::numeric_limits<std::uint64_t>::max(),
            错误::算术不可表示);
        写集 请求;
        请求.期望事实代次 = 截止;
        请求.写入幂等身份 = {截止 + 1};
        return 请求;
    }

    static bool 区间包含(特征I64闭区间 区间, std::int64_t 值) noexcept {
        return 值 >= 区间.下界 && 值 <= 区间.上界;
    }
    static bool 集合包含(const std::vector<特征I64闭区间>& 集合,
        特征I64闭区间 范围) {
        auto 排序 = 集合;
        std::sort(排序.begin(), 排序.end(), [](const auto& 左, const auto& 右) {
            return 左.下界 < 右.下界;
        });
        auto 尚需覆盖 = 范围.下界;
        for (const auto& 区间 : 排序) {
            if (区间.上界 < 尚需覆盖) continue;
            if (区间.下界 > 尚需覆盖) return false;
            if (区间.上界 >= 范围.上界) return true;
            尚需覆盖 = 区间.上界 + 1; // 此处严格小于目标上界，故不溢出。
        }
        return false;
    }

    static void 检查类型规格(const 先天I64特征类型规格& 规格) {
        要求(有效(规格.外设提供者) && 有效(规格.单位)
            && 有效(规格.误差参数来源) && 规格.缩放分子 != 0
            && 规格.缩放分母 != 0 && !规格.允许集合.empty()
            && 规格.允许误差 >= 0, 错误::入口拒绝);
        for (const auto& 区间 : 规格.允许集合)
            要求(浅层结构有效(区间), 错误::入口拒绝);
    }

    static const 特征I64闭区间& 整数域(const 特征值域项& 域) {
        const auto* 范围 = std::get_if<特征I64闭区间>(&域.范围);
        要求(范围 != nullptr, 错误::能力未提供);
        return *范围;
    }
    static std::int64_t 内联值(const 特征观察值& 值) {
        const auto* 整数 = std::get_if<std::int64_t>(&值);
        要求(整数 != nullptr);
        return *整数;
    }
    static std::uint64_t 位型(std::int64_t 值) noexcept {
        return std::bit_cast<std::uint64_t>(值);
    }
    static std::int64_t 整数(std::uint64_t 值) noexcept {
        return std::bit_cast<std::int64_t>(值);
    }

    static std::vector<std::uint64_t> 编码历史(const std::vector<特征历史值项>& 历史) {
        std::vector<std::uint64_t> 材料{static_cast<std::uint64_t>(历史.size())};
        for (const auto& 项 : 历史) {
            if (const auto* 值 = std::get_if<特征观察值>(&项)) {
                材料.push_back(0); 材料.push_back(位型(内联值(*值)));
            } else {
                材料.push_back(1); 材料.push_back(std::get<特征值域项标识>(项).值);
            }
        }
        return 材料;
    }
    static std::vector<std::uint64_t> 编码域(std::vector<特征值域项> 域组) {
        std::sort(域组.begin(), 域组.end(), [](const auto& 左, const auto& 右) {
            return 左.标识.值 < 右.标识.值;
        });
        std::vector<std::uint64_t> 材料{static_cast<std::uint64_t>(域组.size())};
        for (const auto& 域 : 域组) {
            const auto& 范围 = 整数域(域);
            材料.insert(材料.end(), {域.标识.值, 位型(内联值(域.代表值)),
                位型(范围.下界), 位型(范围.上界), 域.命中次数});
        }
        return 材料;
    }

    static const 特征值域项& 找域(const 节点副本& 节点, 特征值域项标识 标识) {
        要求(有效(标识), 错误::入口拒绝);
        const auto 项 = std::find_if(节点.域.begin(), 节点.域.end(),
            [&](const auto& 域) { return 域.标识 == 标识; });
        要求(项 != 节点.域.end(), 错误::未找到);
        return *项;
    }
    static bool 有稳定覆盖(const 节点副本& 节点, std::int64_t 值) {
        return std::any_of(节点.域.begin(), 节点.域.end(), [&](const auto& 域) {
            return 域.命中次数 >= 3 && 区间包含(整数域(域), 值);
        });
    }
    static void 检查字段(const 节点副本& 节点);
    static void 检查相容(const 节点副本&, const 先天I64特征类型规格&);
    static 特征信息 组成完整信息(const 节点副本&);
    static 特征值域项标识 新域键(const 节点副本&);
    static bool 相同域定义(const 特征值域项&, const 特征值域项&);
    static void 规范化写集(写集&);
    static 稳定编码 映射编码(const L1所有者范围写入结果&, 本地键);
    static 稳定编码 解析引用(const L1所有者范围写入结果&, const 引用&);
    static 写集 初始化写集(std::uint64_t);
    void 确认发布载荷(const 写集&, const L1所有者范围写入结果&) const;
    L1所有者范围写入结果 提交并读回(写集, bool 初始化结构 = false);
    L1所有者范围写入结果 收敛原请求();
    void 接受初始化映射(const L1所有者范围写入结果&);
    先天I64特征类型信息 读类型(特征类型身份, std::uint64_t&) const;
    节点副本 读节点(特征信息身份, std::uint64_t&) const;
    void 检查细分(const 节点副本&, std::uint64_t&) const;
    std::optional<特征值域项> 读继承域(const 节点副本&, std::uint64_t&) const;
    std::int64_t 解析观察值(const 特征观察值&, std::uint64_t&) const;
    特征历史值项 规范化历史项(const 节点副本&, const 特征历史值项&,
        std::uint64_t&) const;
    void 处理输入(节点副本&, std::int64_t, const 先天I64特征类型信息&,
        写集&, std::uint64_t&);
    void 保存节点字段(const 节点副本&, 写集&) const;
    void 修改类型副本(节点副本&, 特征类型身份, 写集&, std::uint64_t&) const;
    void 检查域可改义(const 节点副本&, 特征值域项标识) const;
    特征值域项 修改域副本(节点副本&, 特征值域项标识,
        const 特征观察值&, const 特征值域范围&, std::uint64_t&) const;

    const L1事实基座服务& 第一层服务_;
    L1所有者范围写端口 写端口_;
    const 特征值类数据服务& 值服务_;
    稳定编码 输入生产者_;
    std::array<稳定编码, 结构数量> 结构_{};
    bool 已初始化_ = false;
    std::optional<待确认写入> 待确认_;
    mutable std::mutex 入口锁_;
};

inline 先天I64特征类型信息 特征类数据服务::读类型(特征类型身份 身份,
    std::uint64_t& 截止) const {
    要求(有效(身份), 错误::入口拒绝);
    L1所有者范围一致当前读取请求 请求;
    请求.节点.push_back(身份.编码);
    请求.属性值.push_back({身份.编码, 结构_[类型规格属性]});
    for (const auto 关系 : {外设来源关系, 单位关系, 域规则关系})
        请求.源关系组.push_back({身份.编码, 结构_[关系]});
    const auto 读取 = 取得投影(std::move(请求), 截止);
    检查项目状态(读取.节点.front().状态);
    要求(读取.节点.front().事实.has_value()
        && 读取.节点.front().事实->写入所有者 == 写端口_.所有者身份()
        && 读取.节点.front().事实->种类 == 节点种类::普通, 错误::类型不相容);
    检查项目状态(读取.属性值.front().状态);
    要求(读取.属性值.front().投影.has_value());
    const auto& 值事实 = 读取.属性值.front().投影->当前值事实;
    要求(值事实.所属节点 == 身份.编码 && 值事实.属性类型节点 == 结构_[类型规格属性]
        && 值事实.写入所有者 == 写端口_.所有者身份() && !值事实.退出事实代次);
    const auto* 材料 = std::get_if<std::vector<std::uint64_t>>(&值事实.材料);
    要求(材料 != nullptr && 材料->size() >= 3);
    const auto 数量 = (*材料)[2];
    要求(数量 > 0 && 数量 <= (材料->size() - 3) / 2
        && 数量 * 2 == 材料->size() - 3);
    先天I64特征类型信息 类型;
    类型.身份 = 身份;
    类型.规格.缩放分子 = (*材料)[0];
    类型.规格.缩放分母 = (*材料)[1];
    for (std::size_t 序号 = 0; 序号 < 数量; ++序号)
        类型.规格.允许集合.push_back({整数((*材料)[3 + 序号 * 2]),
            整数((*材料)[4 + 序号 * 2])});
    for (std::size_t 序号 = 0; 序号 < 读取.源关系组.size(); ++序号) {
        const auto& 组 = 读取.源关系组[序号];
        要求(组.成员.size() == 1, 错误::规则缺失);
        const auto& 项 = 组.成员.front();
        要求(项.关系.源节点 == 身份.编码 && 项.关系.目标节点 == 项.对端节点.编码
            && 项.关系.关系类型节点 == 组.关系类型节点 && 项.关系.角色或顺序 == 0
            && 项.关系.写入所有者 == 写端口_.所有者身份()
            && !项.关系.退出事实代次 && !项.对端节点.退出事实代次);
        if (序号 == 0) 类型.规格.外设提供者 = 项.对端节点.编码;
        if (序号 == 1) 类型.规格.单位 = 项.对端节点.编码;
        if (序号 == 2) 类型.规则 = {项.对端节点.编码};
    }
    L1所有者范围一致当前读取请求 规则请求;
    规则请求.节点.push_back(类型.规则.编码);
    规则请求.属性值.push_back({类型.规则.编码, 结构_[规则误差属性]});
    规则请求.源关系组.push_back({类型.规则.编码, 结构_[参数来源关系]});
    const auto 规则 = 取得投影(std::move(规则请求), 截止);
    检查项目状态(规则.节点.front().状态);
    要求(规则.节点.front().事实.has_value()
        && 规则.节点.front().事实->写入所有者 == 写端口_.所有者身份()
        && 规则.节点.front().事实->种类 == 节点种类::普通);
    要求(规则.属性值.front().状态 == 读取项状态::成功
        && 规则.属性值.front().投影.has_value(), 错误::规则缺失);
    const auto& 误差事实 = 规则.属性值.front().投影->当前值事实;
    const auto* 误差 = std::get_if<std::int64_t>(&误差事实.材料);
    要求(误差 != nullptr && 误差事实.所属节点 == 类型.规则.编码
        && 误差事实.属性类型节点 == 结构_[规则误差属性]
        && 误差事实.写入所有者 == 写端口_.所有者身份());
    类型.规格.允许误差 = *误差;
    要求(规则.源关系组.front().成员.size() == 1, 错误::规则缺失);
    const auto& 来源 = 规则.源关系组.front().成员.front();
    要求(来源.关系.源节点 == 类型.规则.编码
        && 来源.关系.关系类型节点 == 结构_[参数来源关系]
        && 来源.关系.角色或顺序 == 0
        && 来源.关系.写入所有者 == 写端口_.所有者身份()
        && 来源.关系.目标节点 == 来源.对端节点.编码 && !来源.对端节点.退出事实代次);
    类型.规格.误差参数来源 = 来源.对端节点.编码;
    检查类型规格(类型.规格);
    return 类型;
}

inline 特征类数据服务::节点副本 特征类数据服务::读节点(
    特征信息身份 身份, std::uint64_t& 截止) const {
    要求(有效(身份), 错误::入口拒绝);
    L1所有者范围一致当前读取请求 请求;
    请求.节点.push_back(身份.编码);
    for (const auto 属性 : {当前值属性, 历史集合属性, 域集合属性})
        请求.属性值.push_back({身份.编码, 结构_[属性]});
    请求.源关系组.push_back({身份.编码, 结构_[特征类型关系]});
    请求.源关系组.push_back({身份.编码, 结构_[细分关系]});
    请求.目标关系组.push_back({身份.编码, 结构_[细分关系]});
    const auto 读取 = 取得投影(std::move(请求), 截止);
    检查项目状态(读取.节点.front().状态);
    要求(读取.节点.front().事实.has_value()
        && 读取.节点.front().事实->编码 == 身份.编码
        && 读取.节点.front().事实->写入所有者 == 写端口_.所有者身份()
        && 读取.节点.front().事实->种类 == 节点种类::普通, 错误::类型不相容);
    节点副本 节点;
    节点.身份 = 身份;
    for (std::size_t 序号 = 0; 序号 < 3; ++序号) {
        const auto& 项 = 读取.属性值[序号];
        if (序号 == 0 && 项.状态 == 读取项状态::属性未设置) continue;
        要求(项.状态 == 读取项状态::成功 && 项.投影.has_value(), 错误::类型不相容);
        const auto& 值 = 项.投影->当前值事实;
        要求(值.所属节点 == 身份.编码 && 值.属性类型节点 == 项.属性类型
            && 值.写入所有者 == 写端口_.所有者身份() && !值.退出事实代次
            && 项.投影->属性槽.当前值 == 值.编码);
        if (序号 == 0) {
            const auto* 内容 = std::get_if<std::int64_t>(&值.材料);
            要求(内容 != nullptr);
            节点.当前 = *内容;
            节点.当前载荷 = 值.编码;
            continue;
        }
        const auto* 材料 = std::get_if<std::vector<std::uint64_t>>(&值.材料);
        要求(材料 != nullptr && !材料->empty());
        const auto 数量 = 材料->front();
        const std::size_t 宽度 = 序号 == 1 ? 2 : 5;
        要求(数量 <= (材料->size() - 1) / 宽度
            && 数量 * 宽度 == 材料->size() - 1);
        if (序号 == 1) {
            节点.历史载荷 = 值.编码;
            for (std::size_t 索引 = 0; 索引 < 数量; ++索引) {
                const auto 标签 = (*材料)[1 + 索引 * 2];
                const auto 内容 = (*材料)[2 + 索引 * 2];
                要求(标签 <= 1);
                if (标签 == 0) 节点.历史.push_back(特征观察值{整数(内容)});
                else 节点.历史.push_back(特征值域项标识{内容});
            }
        } else {
            节点.域载荷 = 值.编码;
            for (std::size_t 索引 = 0; 索引 < 数量; ++索引) {
                const auto 位置 = 1 + 索引 * 5;
                节点.域.push_back({{(*材料)[位置]}, 特征观察值{整数((*材料)[位置 + 1])},
                    特征I64闭区间{整数((*材料)[位置 + 2]), 整数((*材料)[位置 + 3])},
                    (*材料)[位置 + 4]});
            }
        }
    }
    要求(读取.源关系组[0].成员.size() <= 1 && 读取.目标关系组[0].成员.size() <= 1);
    const auto 检查关系 = [&](const L1所有者范围一致关系对端投影& 项,
        结构位置 类型, bool 作为源) {
        要求(项.关系.写入所有者 == 写端口_.所有者身份()
            && 项.关系.关系类型节点 == 结构_[类型] && !项.关系.退出事实代次
            && !项.对端节点.退出事实代次
            && 项.对端节点.写入所有者 == 写端口_.所有者身份()
            && (作为源 ? 项.关系.源节点 : 项.关系.目标节点) == 身份.编码
            && (作为源 ? 项.关系.目标节点 : 项.关系.源节点) == 项.对端节点.编码
            && (类型 == 特征类型关系 ? 项.关系.角色或顺序 == 0 : 项.关系.角色或顺序 > 0));
    };
    if (!读取.源关系组[0].成员.empty()) {
        const auto& 项 = 读取.源关系组[0].成员.front();
        检查关系(项, 特征类型关系, true);
        节点.类型 = 特征类型身份{项.关系.目标节点};
        节点.类型关联 = 项.关系;
    }
    for (const auto& 项 : 读取.源关系组[1].成员) {
        检查关系(项, 细分关系, true);
        要求(std::none_of(节点.子关联.begin(), 节点.子关联.end(), [&](const auto& 已有) {
            return 已有.角色或顺序 == 项.关系.角色或顺序 || 已有.目标节点 == 项.关系.目标节点;
        }));
        节点.子关联.push_back(项.关系);
    }
    for (const auto& 项 : 读取.目标关系组[0].成员) {
        检查关系(项, 细分关系, false);
        节点.父关联.push_back(项.关系);
    }
    检查字段(节点);
    if (节点.类型) 检查相容(节点, 读类型(*节点.类型, 截止).规格);
    return 节点;
}

inline std::optional<特征值域项> 特征类数据服务::读继承域(
    const 节点副本& 节点, std::uint64_t& 截止) const {
    if (节点.父关联.empty()) return std::nullopt;
    要求(节点.父关联.size() == 1 && 节点.类型.has_value());
    const auto& 关联 = 节点.父关联.front();
    要求(关联.源节点 != 节点.身份.编码 && 关联.角色或顺序 > 0);
    const auto 父 = 读节点({关联.源节点}, 截止);
    要求(父.类型 == 节点.类型);
    const 特征值域项标识 标识{static_cast<std::uint64_t>(关联.角色或顺序)};
    const auto& 父域 = 找域(父, 标识);
    const auto& 本域 = 找域(节点, 标识);
    要求(父域.命中次数 >= 3 && 相同域定义(父域, 本域));
    return 本域;
}

inline void 特征类数据服务::检查细分(const 节点副本& 节点,
    std::uint64_t& 截止) const {
    (void)读继承域(节点, 截止);
    for (const auto& 关联 : 节点.子关联) {
        要求(关联.目标节点 != 节点.身份.编码 && 节点.类型.has_value());
        const auto 子 = 读节点({关联.目标节点}, 截止);
        要求(子.类型 == 节点.类型 && 子.父关联.size() == 1
            && 子.父关联.front() == 关联);
        const 特征值域项标识 标识{static_cast<std::uint64_t>(关联.角色或顺序)};
        const auto& 父域 = 找域(节点, 标识);
        要求(父域.命中次数 >= 3 && 相同域定义(父域, 找域(子, 标识)));
    }
    std::vector<稳定编码> 已见{节点.身份.编码};
    auto 祖先关系 = 节点.父关联;
    while (!祖先关系.empty()) {
        const auto 祖先 = 祖先关系.front().源节点;
        要求(std::find(已见.begin(), 已见.end(), 祖先) == 已见.end());
        已见.push_back(祖先);
        const auto 父 = 读节点({祖先}, 截止);
        要求(父.类型 == 节点.类型);
        祖先关系 = 父.父关联;
    }
}

inline std::int64_t 特征类数据服务::解析观察值(const 特征观察值& 观察,
    std::uint64_t& 截止) const {
    要求(浅层结构有效(观察), 错误::入口拒绝);
    if (const auto* 值 = std::get_if<std::int64_t>(&观察)) return *值;
    const auto 身份 = std::get<特征值身份>(观察);
    const auto 读取 = 值服务_.获取特征值(身份);
    if (const auto* 状态 = std::get_if<特征值读取错误>(&读取)) {
        switch (*状态) {
        case 特征值读取错误::资源失败: throw 错误::资源失败;
        case 特征值读取错误::未找到: throw 错误::未找到;
        case 特征值读取错误::材料已清理:
        case 特征值读取错误::能力未提供: throw 错误::能力未提供;
        case 特征值读取错误::入口拒绝: throw 错误::入口拒绝;
        default: throw 错误::内部不一致;
        }
    }
    const auto& 值信息 = std::get<特征值信息>(读取);
    const auto* 整数值 = std::get_if<std::int64_t>(&值信息.值内容);
    要求(整数值 != nullptr, 错误::类型不相容);
    const auto 正式 = 第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, 身份.编码});
    if (正式.状态 == L1所有者范围读取状态::资源失败) throw 错误::资源失败;
    要求(正式.读取事实代次 == 截止, 错误::并发变化);
    要求(正式.状态 == L1所有者范围读取状态::成功 && 正式.事实.has_value(), 错误::能力未提供);
    const auto* 事实 = std::get_if<L1所有者范围值事实>(&*正式.事实);
    要求(事实 != nullptr && 事实->编码 == 身份.编码 && 事实->创建事实代次 <= 截止
        && std::holds_alternative<std::int64_t>(事实->材料)
        && std::get<std::int64_t>(事实->材料) == *整数值);
    return *整数值;
}

inline 特征历史值项 特征类数据服务::规范化历史项(const 节点副本& 节点,
    const 特征历史值项& 项, std::uint64_t& 截止) const {
    要求(!项.valueless_by_exception(), 错误::入口拒绝);
    if (const auto* 值 = std::get_if<特征观察值>(&项))
        return 特征观察值{解析观察值(*值, 截止)};
    const auto 标识 = std::get<特征值域项标识>(项);
    要求(找域(节点, 标识).命中次数 >= 3, 错误::入口拒绝);
    return 标识;
}

inline 特征数据结果<特征类型身份> 特征类数据服务::创建先天I64特征类型(
    const 先天I64特征类型规格& 规格) {
    return 保护执行<特征类型身份>([&] {
        auto 截止 = 开始写入();
        检查类型规格(规格);
        L1所有者范围一致当前读取请求 读取请求;
        for (const auto 编码 : {规格.外设提供者, 规格.单位, 规格.误差参数来源})
            if (std::find(读取请求.节点.begin(), 读取请求.节点.end(), 编码) == 读取请求.节点.end())
                读取请求.节点.push_back(编码);
        const auto 读取 = 取得投影(std::move(读取请求), 截止);
        for (const auto& 项 : 读取.节点) {
            检查项目状态(项.状态);
            要求(项.事实.has_value() && !项.事实->退出事实代次);
        }
        auto 请求 = 新写集(截止);
        const auto 类型 = 加节点(请求);
        const auto 规则 = 加节点(请求);
        std::vector<std::uint64_t> 材料{规格.缩放分子, 规格.缩放分母,
            static_cast<std::uint64_t>(规格.允许集合.size())};
        for (const auto& 区间 : 规格.允许集合) {
            材料.push_back(位型(区间.下界)); 材料.push_back(位型(区间.上界));
        }
        写字段(请求, 类型, 类型规格属性, std::move(材料));
        写字段(请求, 规则, 规则误差属性, 规格.允许误差);
        加关系(请求, 类型, 规格.外设提供者, 外设来源关系);
        加关系(请求, 类型, 规格.单位, 单位关系);
        加关系(请求, 类型, 规则, 域规则关系);
        加关系(请求, 规则, 规格.误差参数来源, 参数来源关系);
        const auto 发布 = 提交并读回(std::move(请求));
        return 特征类型身份{映射编码(发布, 类型)};
    });
}

inline 特征数据结果<先天I64特征类型信息> 特征类数据服务::读取先天I64特征类型(
    特征类型身份 身份) const {
    return 保护执行<先天I64特征类型信息>([&] {
        auto 截止 = 开始读取();
        return 读类型(身份, 截止);
    });
}

inline void 特征类数据服务::保存节点字段(const 节点副本& 节点, 写集& 请求) const {
    检查字段(节点);
    if (节点.当前) 写字段(请求, 节点.身份.编码, 当前值属性, *节点.当前, 节点.当前载荷);
    写字段(请求, 节点.身份.编码, 历史集合属性, 编码历史(节点.历史), 节点.历史载荷);
    写字段(请求, 节点.身份.编码, 域集合属性, 编码域(节点.域), 节点.域载荷);
}

inline void 特征类数据服务::处理输入(节点副本& 节点, std::int64_t 输入,
    const 先天I64特征类型信息& 类型, 写集& 请求, std::uint64_t& 截止) {
    要求(节点.类型 == 类型.身份);
    要求(集合包含(类型.规格.允许集合, {输入, 输入}), 错误::类型不相容);
    节点.当前 = 输入;
    bool 有命中 = false;
    for (auto& 域 : 节点.域) {
        if (!区间包含(整数域(域), 输入)) continue;
        要求(域.命中次数 != std::numeric_limits<std::uint64_t>::max(), 错误::算术不可表示);
        ++域.命中次数;
        有命中 = true;
    }
    if (!有命中) {
        const auto 误差 = 类型.规格.允许误差;
        要求(输入 >= std::numeric_limits<std::int64_t>::min() + 误差
            && 输入 <= std::numeric_limits<std::int64_t>::max() - 误差, 错误::算术不可表示);
        const 特征I64闭区间 范围{输入 - 误差, 输入 + 误差};
        要求(集合包含(类型.规格.允许集合, 范围), 错误::类型不相容);
        节点.域.push_back({新域键(节点), 特征观察值{输入}, 范围, 1});
        std::sort(节点.域.begin(), 节点.域.end(), [](const auto& 左, const auto& 右) {
            return 左.标识.值 < 右.标识.值;
        });
    }
    const 特征历史值项 准确项{特征观察值{输入}};
    if (!有稳定覆盖(节点, 输入)
        && std::find(节点.历史.begin(), 节点.历史.end(), 准确项) == 节点.历史.end())
        节点.历史.push_back(准确项);
    const auto 继承 = 读继承域(节点, 截止);
    for (const auto& 域 : 节点.域) {
        if (域.命中次数 < 3) continue;
        const 特征历史值项 域项{域.标识};
        if (std::find(节点.历史.begin(), 节点.历史.end(), 域项) == 节点.历史.end())
            节点.历史.push_back(域项);
        std::erase_if(节点.历史, [&](const auto& 项) {
            const auto* 值 = std::get_if<特征观察值>(&项);
            return 值 != nullptr && 区间包含(整数域(域), 内联值(*值));
        });
        const bool 已建子 = std::any_of(节点.子关联.begin(), 节点.子关联.end(),
            [&](const auto& 关系) { return static_cast<std::uint64_t>(关系.角色或顺序) == 域.标识.值; });
        if (已建子 || (继承 && 相同域定义(*继承, 域))) continue;
        要求(有效(节点.身份)); // 首次创建最多一次输入，不会在这里生成子节点。
        const auto 子 = 加节点(请求);
        auto 子域 = 域;
        子域.命中次数 = 0;
        写字段(请求, 子, 当前值属性, 输入);
        写字段(请求, 子, 历史集合属性, 编码历史({准确项}));
        写字段(请求, 子, 域集合属性, 编码域({子域}));
        加关系(请求, 子, 类型.身份.编码, 特征类型关系);
        要求(域.标识.值 <= static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max()),
            错误::算术不可表示);
        加关系(请求, 节点.身份.编码, 子, 细分关系, static_cast<std::int64_t>(域.标识.值));
    }
    检查字段(节点);
}

inline 特征数据结果<特征信息身份> 特征类数据服务::创建特征信息(
    特征类型身份 类型身份, const 特征观察值& 输入) {
    return 保护执行<特征信息身份>([&] {
        auto 截止 = 开始写入();
        const auto 类型 = 读类型(类型身份, 截止);
        const auto 值 = 解析观察值(输入, 截止);
        节点副本 节点;
        节点.类型 = 类型身份;
        auto 请求 = 新写集(截止);
        处理输入(节点, 值, 类型, 请求, 截止);
        const auto 键 = 加节点(请求);
        写字段(请求, 键, 当前值属性, 值);
        写字段(请求, 键, 历史集合属性, 编码历史(节点.历史));
        写字段(请求, 键, 域集合属性, 编码域(节点.域));
        加关系(请求, 键, 类型身份.编码, 特征类型关系);
        const auto 发布 = 提交并读回(std::move(请求));
        return 特征信息身份{映射编码(发布, 键)};
    });
}

inline 特征数据结果<特征信息> 特征类数据服务::读取特征信息(特征信息身份 身份) const {
    return 保护执行<特征信息>([&] {
        auto 截止 = 开始读取();
        const auto 节点 = 读节点(身份, 截止);
        检查细分(节点, 截止);
        return 组成完整信息(节点);
    });
}

inline void 特征类数据服务::修改类型副本(节点副本& 节点, 特征类型身份 新类型,
    写集& 请求, std::uint64_t& 截止) const {
    const auto 新规格 = 读类型(新类型, 截止);
    检查相容(节点, 新规格.规格);
    if (节点.类型 == 新类型) return;
    要求(节点.父关联.empty() && 节点.子关联.empty(), 错误::引用冲突);
    std::optional<特征类型身份> 旧类型 = 节点.类型;
    if (!旧类型 && (节点.当前 || !节点.历史.empty() || !节点.域.empty())) {
        // 所有能改写历史/域的函数都先取得正式类型；移除类型不换载荷。
        // 以仍存非空字段的创建截止定位其解释，不回到节点最初类型。
        const auto 载荷 = !节点.域.empty() ? 节点.域载荷
            : (!节点.历史.empty() ? 节点.历史载荷 : *节点.当前载荷);
        const auto 字段 = 第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, 载荷});
        要求(字段.读取事实代次 == 截止, 错误::并发变化);
        要求(字段.状态 == L1所有者范围读取状态::成功 && 字段.事实.has_value());
        const auto* 值 = std::get_if<L1所有者范围值事实>(&*字段.事实);
        要求(值 != nullptr && 值->所属节点 == 节点.身份.编码 && 值->创建事实代次 != 0);
        const auto 关系 = 第一层服务_.读取所有者范围历史关系组({L1所有者范围CRUD合同版本,
            L1所有者范围关系端点方向::源, 节点.身份.编码, 结构_[特征类型关系], 值->创建事实代次});
        要求(关系.读取事实代次 == 截止, 错误::并发变化);
        要求(关系.状态 == L1所有者范围读取状态::成功 && 关系.关系组.size() == 1,
            错误::类型不相容);
        const auto& 项 = 关系.关系组.front();
        要求(项.源节点 == 节点.身份.编码 && 项.关系类型节点 == 结构_[特征类型关系]
            && 项.写入所有者 == 写端口_.所有者身份() && 项.角色或顺序 == 0);
        旧类型 = 特征类型身份{项.目标节点};
    }
    if (旧类型 && (节点.当前 || !节点.历史.empty() || !节点.域.empty())) {
        const auto 旧规格 = 读类型(*旧类型, 截止);
        const auto 旧约数 = std::gcd(旧规格.规格.缩放分子, 旧规格.规格.缩放分母);
        const auto 新约数 = std::gcd(新规格.规格.缩放分子, 新规格.规格.缩放分母);
        要求(旧规格.规格.单位 == 新规格.规格.单位
            && 旧规格.规格.缩放分子 / 旧约数 == 新规格.规格.缩放分子 / 新约数
            && 旧规格.规格.缩放分母 / 旧约数 == 新规格.规格.缩放分母 / 新约数,
            错误::类型不相容);
    }
    if (节点.类型关联) 请求.退出事实.push_back(节点.类型关联->编码);
    加关系(请求, 节点.身份.编码, 新类型.编码, 特征类型关系);
    节点.类型 = 新类型;
}

inline 特征数据结果<特征信息> 特征类数据服务::修改特征信息(特征信息身份 身份,
    特征类型身份 类型, const 特征观察值& 输入) {
    return 保护执行<特征信息>([&] {
        auto 截止 = 开始写入();
        auto 节点 = 读节点(身份, 截止);
        检查细分(节点, 截止);
        auto 请求 = 新写集(截止);
        修改类型副本(节点, 类型, 请求, 截止);
        处理输入(节点, 解析观察值(输入, 截止), 读类型(类型, 截止), 请求, 截止);
        保存节点字段(节点, 请求);
        auto 信息 = 组成完整信息(节点); // 成功结果载荷先分配，再发布。
        (void)提交并读回(std::move(请求));
        return 信息;
    });
}

inline 特征数据结果<特征信息> 特征类数据服务::接收观察值(特征信息身份 身份,
    const 特征观察值& 输入) {
    return 保护执行<特征信息>([&] {
        auto 截止 = 开始写入();
        auto 节点 = 读节点(身份, 截止);
        要求(节点.类型.has_value(), 错误::未设置);
        检查细分(节点, 截止);
        const auto 类型 = 读类型(*节点.类型, 截止);
        auto 请求 = 新写集(截止);
        处理输入(节点, 解析观察值(输入, 截止), 类型, 请求, 截止);
        保存节点字段(节点, 请求);
        auto 信息 = 组成完整信息(节点);
        (void)提交并读回(std::move(请求));
        return 信息;
    });
}

inline 特征数据结果<特征类型身份> 特征类数据服务::读取特征类型(特征信息身份 身份) const {
    return 保护执行<特征类型身份>([&] {
        auto 截止 = 开始读取();
        const auto 节点 = 读节点(身份, 截止);
        要求(节点.类型.has_value(), 错误::未设置);
        return *节点.类型;
    });
}

inline 特征数据结果<特征类型身份> 特征类数据服务::设置特征类型(
    特征信息身份 身份, 特征类型身份 类型) {
    return 保护执行<特征类型身份>([&] {
        auto 截止 = 开始写入();
        auto 节点 = 读节点(身份, 截止);
        检查细分(节点, 截止);
        auto 请求 = 新写集(截止);
        修改类型副本(节点, 类型, 请求, 截止);
        if (!请求.关系.empty()) (void)提交并读回(std::move(请求));
        return 类型;
    });
}

inline 特征数据结果<std::monostate> 特征类数据服务::移除特征类型(特征信息身份 身份) {
    return 保护执行<std::monostate>([&] {
        auto 截止 = 开始写入();
        const auto 节点 = 读节点(身份, 截止);
        要求(节点.父关联.empty() && 节点.子关联.empty(), 错误::引用冲突);
        if (节点.类型关联) {
            auto 请求 = 新写集(截止);
            请求.退出事实.push_back(节点.类型关联->编码);
            (void)提交并读回(std::move(请求));
        }
        return std::monostate{};
    });
}

inline 特征数据结果<特征观察值> 特征类数据服务::读取准确当前观察值(特征信息身份 身份) const {
    return 保护执行<特征观察值>([&] {
        auto 截止 = 开始读取();
        const auto 节点 = 读节点(身份, 截止);
        要求(节点.当前.has_value(), 错误::未设置);
        return 特征观察值{*节点.当前};
    });
}

inline 特征数据结果<std::monostate> 特征类数据服务::移除准确当前观察值(特征信息身份 身份) {
    return 保护执行<std::monostate>([&] {
        auto 截止 = 开始写入();
        const auto 节点 = 读节点(身份, 截止);
        if (节点.当前载荷) {
            auto 请求 = 新写集(截止);
            请求.退出事实.push_back(*节点.当前载荷);
            (void)提交并读回(std::move(请求));
        }
        return std::monostate{};
    });
}

inline 特征数据结果<std::monostate> 特征类数据服务::删除特征信息(特征信息身份 身份) {
    return 保护执行<std::monostate>([&] {
        auto 截止 = 开始写入();
        const auto 节点 = 读节点(身份, 截止);
        要求(节点.父关联.empty() && 节点.子关联.empty(), 错误::引用冲突);
        auto 请求 = 新写集(截止);
        请求.退出事实 = {节点.历史载荷, 节点.域载荷, 身份.编码};
        if (节点.当前载荷) 请求.退出事实.push_back(*节点.当前载荷);
        if (节点.类型关联) 请求.退出事实.push_back(节点.类型关联->编码);
        (void)提交并读回(std::move(请求));
        return std::monostate{};
    });
}

inline 特征数据结果<std::vector<特征信息>> 特征类数据服务::查询特征信息(
    特征类型身份 类型, const 特征观察值& 条件) const {
    return 保护执行<std::vector<特征信息>>([&] {
        auto 截止 = 开始读取();
        const auto 规格 = 读类型(类型, 截止);
        const auto 值 = 解析观察值(条件, 截止);
        要求(集合包含(规格.规格.允许集合, {值, 值}), 错误::类型不相容);
        L1所有者范围一致当前读取请求 请求;
        请求.目标关系组.push_back({类型.编码, 结构_[特征类型关系]});
        const auto 读取 = 取得投影(std::move(请求), 截止);
        std::vector<特征信息> 匹配;
        for (const auto& 项 : 读取.目标关系组.front().成员) {
            要求(项.关系.目标节点 == 类型.编码 && 项.关系.源节点 == 项.对端节点.编码
                && 项.关系.写入所有者 == 写端口_.所有者身份());
            const auto 节点 = 读节点({项.关系.源节点}, 截止);
            要求(节点.类型 == 类型 && 节点.类型关联 == 项.关系);
            检查细分(节点, 截止);
            if (节点.当前 && *节点.当前 == 值) 匹配.push_back(组成完整信息(节点));
        }
        std::sort(匹配.begin(), 匹配.end(), [](const auto& 左, const auto& 右) {
            return 左.身份.编码 < 右.身份.编码;
        });
        return 匹配;
    });
}

inline 特征数据结果<std::vector<特征历史值项>> 特征类数据服务::读取历史值集合(
    特征信息身份 身份) const {
    return 保护执行<std::vector<特征历史值项>>([&] {
        auto 截止 = 开始读取();
        return 读节点(身份, 截止).历史;
    });
}

inline 特征数据结果<std::monostate> 特征类数据服务::添加历史值项(特征信息身份 身份,
    const 特征历史值项& 输入) {
    return 保护执行<std::monostate>([&] {
        auto 截止 = 开始写入();
        auto 节点 = 读节点(身份, 截止);
        要求(节点.类型.has_value(), 错误::未设置);
        const auto 项 = 规范化历史项(节点, 输入, 截止);
        if (std::find(节点.历史.begin(), 节点.历史.end(), 项) != 节点.历史.end())
            return std::monostate{};
        节点.历史.push_back(项);
        检查字段(节点);
        检查相容(节点, 读类型(*节点.类型, 截止).规格);
        auto 请求 = 新写集(截止);
        写字段(请求, 身份.编码, 历史集合属性, 编码历史(节点.历史), 节点.历史载荷);
        (void)提交并读回(std::move(请求));
        return std::monostate{};
    });
}

inline 特征数据结果<std::monostate> 特征类数据服务::修改历史值项(特征信息身份 身份,
    const 特征历史值项& 旧输入, const 特征历史值项& 新输入) {
    return 保护执行<std::monostate>([&] {
        auto 截止 = 开始写入();
        auto 节点 = 读节点(身份, 截止);
        要求(节点.类型.has_value(), 错误::未设置);
        const auto 旧项 = 规范化历史项(节点, 旧输入, 截止);
        const auto 新项 = 规范化历史项(节点, 新输入, 截止);
        const auto 位置 = std::find(节点.历史.begin(), 节点.历史.end(), 旧项);
        要求(位置 != 节点.历史.end(), 错误::未找到);
        if (旧项 == 新项) return std::monostate{};
        节点.历史.erase(位置);
        if (std::find(节点.历史.begin(), 节点.历史.end(), 新项) == 节点.历史.end())
            节点.历史.push_back(新项);
        检查字段(节点);
        检查相容(节点, 读类型(*节点.类型, 截止).规格);
        auto 请求 = 新写集(截止);
        写字段(请求, 身份.编码, 历史集合属性, 编码历史(节点.历史), 节点.历史载荷);
        (void)提交并读回(std::move(请求));
        return std::monostate{};
    });
}

inline 特征数据结果<std::monostate> 特征类数据服务::删除历史值项(特征信息身份 身份,
    const 特征历史值项& 输入) {
    return 保护执行<std::monostate>([&] {
        auto 截止 = 开始写入();
        auto 节点 = 读节点(身份, 截止);
        要求(节点.类型.has_value(), 错误::未设置);
        const auto 项 = 规范化历史项(节点, 输入, 截止);
        const auto 位置 = std::find(节点.历史.begin(), 节点.历史.end(), 项);
        要求(位置 != 节点.历史.end(), 错误::未找到);
        节点.历史.erase(位置);
        检查字段(节点);
        auto 请求 = 新写集(截止);
        写字段(请求, 身份.编码, 历史集合属性, 编码历史(节点.历史), 节点.历史载荷);
        (void)提交并读回(std::move(请求));
        return std::monostate{};
    });
}

inline 特征数据结果<bool> 特征类数据服务::查询历史值项(特征信息身份 身份,
    const 特征历史值项& 输入) const {
    return 保护执行<bool>([&] {
        auto 截止 = 开始读取();
        const auto 节点 = 读节点(身份, 截止);
        const auto 项 = 规范化历史项(节点, 输入, 截止);
        return std::find(节点.历史.begin(), 节点.历史.end(), 项) != 节点.历史.end();
    });
}

inline 特征数据结果<std::vector<特征值域项>> 特征类数据服务::读取值域集合(
    特征信息身份 身份) const {
    return 保护执行<std::vector<特征值域项>>([&] {
        auto 截止 = 开始读取();
        return 读节点(身份, 截止).域;
    });
}

inline 特征数据结果<特征值域项> 特征类数据服务::读取值域项(特征信息身份 身份,
    特征值域项标识 标识) const {
    return 保护执行<特征值域项>([&] {
        auto 截止 = 开始读取();
        return 找域(读节点(身份, 截止), 标识);
    });
}

inline void 特征类数据服务::检查域可改义(const 节点副本& 节点,
    特征值域项标识 标识) const {
    const 特征历史值项 历史引用{标识};
    要求(std::find(节点.历史.begin(), 节点.历史.end(), 历史引用) == 节点.历史.end(),
        错误::引用冲突);
    const auto 引用该域 = [&](const auto& 关系) {
        return static_cast<std::uint64_t>(关系.角色或顺序) == 标识.值;
    };
    要求(std::none_of(节点.子关联.begin(), 节点.子关联.end(), 引用该域)
        && std::none_of(节点.父关联.begin(), 节点.父关联.end(), 引用该域), 错误::引用冲突);
}

inline 特征数据结果<特征值域项> 特征类数据服务::添加值域项(特征信息身份 身份,
    const 特征观察值& 代表, const 特征值域范围& 范围) {
    return 保护执行<特征值域项>([&] {
        auto 截止 = 开始写入();
        auto 节点 = 读节点(身份, 截止);
        要求(节点.类型.has_value(), 错误::未设置);
        检查细分(节点, 截止);
        要求(!范围.valueless_by_exception(), 错误::入口拒绝);
        要求(std::holds_alternative<特征I64闭区间>(范围), 错误::能力未提供);
        const auto 准确代表 = 解析观察值(代表, 截止);
        特征值域项 新域{新域键(节点), 特征观察值{准确代表}, 范围, 0};
        要求(浅层结构有效(新域), 错误::入口拒绝);
        节点.域.push_back(新域);
        std::sort(节点.域.begin(), 节点.域.end(), [](const auto& 左, const auto& 右) {
            return 左.标识.值 < 右.标识.值;
        });
        检查字段(节点);
        检查相容(节点, 读类型(*节点.类型, 截止).规格);
        auto 请求 = 新写集(截止);
        写字段(请求, 身份.编码, 域集合属性, 编码域(节点.域), 节点.域载荷);
        (void)提交并读回(std::move(请求));
        return 新域;
    });
}

inline 特征值域项 特征类数据服务::修改域副本(节点副本& 节点,
    特征值域项标识 标识, const 特征观察值& 代表,
    const 特征值域范围& 范围, std::uint64_t& 截止) const {
    要求(节点.类型.has_value(), 错误::未设置);
    要求(!范围.valueless_by_exception(), 错误::入口拒绝);
    要求(std::holds_alternative<特征I64闭区间>(范围), 错误::能力未提供);
    const auto 旧域 = 找域(节点, 标识);
    特征值域项 新域{标识, 特征观察值{解析观察值(代表, 截止)}, 范围, 0};
    要求(浅层结构有效(新域), 错误::入口拒绝);
    if (相同域定义(旧域, 新域)) return 旧域;
    检查域可改义(节点, 标识);
    新域.标识 = 新域键(节点);
    const auto 位置 = std::find_if(节点.域.begin(), 节点.域.end(),
        [&](const auto& 域) { return 域.标识 == 标识; });
    *位置 = 新域;
    std::sort(节点.域.begin(), 节点.域.end(), [](const auto& 左, const auto& 右) {
        return 左.标识.值 < 右.标识.值;
    });
    检查字段(节点);
    检查相容(节点, 读类型(*节点.类型, 截止).规格);
    return 新域;
}

inline 特征数据结果<特征值域项> 特征类数据服务::修改值域项(特征信息身份 身份,
    特征值域项标识 标识, const 特征观察值& 代表, const 特征值域范围& 范围) {
    return 保护执行<特征值域项>([&] {
        auto 截止 = 开始写入();
        auto 节点 = 读节点(身份, 截止);
        检查细分(节点, 截止);
        const auto 新域 = 修改域副本(节点, 标识, 代表, 范围, 截止);
        if (新域.标识 != 标识) {
            auto 请求 = 新写集(截止);
            写字段(请求, 身份.编码, 域集合属性, 编码域(节点.域), 节点.域载荷);
            (void)提交并读回(std::move(请求));
        }
        return 新域;
    });
}

inline 特征数据结果<std::monostate> 特征类数据服务::删除值域项(特征信息身份 身份,
    特征值域项标识 标识) {
    return 保护执行<std::monostate>([&] {
        auto 截止 = 开始写入();
        auto 节点 = 读节点(身份, 截止);
        要求(节点.类型.has_value(), 错误::未设置);
        检查细分(节点, 截止);
        (void)找域(节点, 标识);
        检查域可改义(节点, 标识);
        std::erase_if(节点.域, [&](const auto& 域) { return 域.标识 == 标识; });
        检查字段(节点);
        auto 请求 = 新写集(截止);
        写字段(请求, 身份.编码, 域集合属性, 编码域(节点.域), 节点.域载荷);
        (void)提交并读回(std::move(请求));
        return std::monostate{};
    });
}

inline 特征数据结果<特征观察值> 特征类数据服务::读取值域代表值(特征信息身份 身份,
    特征值域项标识 标识) const {
    return 保护执行<特征观察值>([&] {
        auto 截止 = 开始读取();
        return 找域(读节点(身份, 截止), 标识).代表值;
    });
}

inline 特征数据结果<特征值域项> 特征类数据服务::修改值域代表值(特征信息身份 身份,
    特征值域项标识 标识, const 特征观察值& 代表) {
    return 保护执行<特征值域项>([&] {
        auto 截止 = 开始写入();
        auto 节点 = 读节点(身份, 截止);
        检查细分(节点, 截止);
        const auto 范围 = 找域(节点, 标识).范围;
        const auto 新域 = 修改域副本(节点, 标识, 代表, 范围, 截止);
        if (新域.标识 != 标识) {
            auto 请求 = 新写集(截止);
            写字段(请求, 身份.编码, 域集合属性, 编码域(节点.域), 节点.域载荷);
            (void)提交并读回(std::move(请求));
        }
        return 新域;
    });
}

inline 特征数据结果<特征值域范围> 特征类数据服务::读取值域范围(特征信息身份 身份,
    特征值域项标识 标识) const {
    return 保护执行<特征值域范围>([&] {
        auto 截止 = 开始读取();
        return 找域(读节点(身份, 截止), 标识).范围;
    });
}

inline 特征数据结果<特征值域项> 特征类数据服务::修改值域范围(特征信息身份 身份,
    特征值域项标识 标识, const 特征值域范围& 范围) {
    return 保护执行<特征值域项>([&] {
        auto 截止 = 开始写入();
        auto 节点 = 读节点(身份, 截止);
        检查细分(节点, 截止);
        const auto 代表 = 找域(节点, 标识).代表值;
        const auto 新域 = 修改域副本(节点, 标识, 代表, 范围, 截止);
        if (新域.标识 != 标识) {
            auto 请求 = 新写集(截止);
            写字段(请求, 身份.编码, 域集合属性, 编码域(节点.域), 节点.域载荷);
            (void)提交并读回(std::move(请求));
        }
        return 新域;
    });
}

inline 特征数据结果<std::uint64_t> 特征类数据服务::读取值域命中次数(特征信息身份 身份,
    特征值域项标识 标识) const {
    return 保护执行<std::uint64_t>([&] {
        auto 截止 = 开始读取();
        return 找域(读节点(身份, 截止), 标识).命中次数;
    });
}

inline 特征数据结果<std::vector<特征信息身份>> 特征类数据服务::读取细分子节点(
    特征信息身份 身份) const {
    return 保护执行<std::vector<特征信息身份>>([&] {
        auto 截止 = 开始读取();
        const auto 节点 = 读节点(身份, 截止);
        检查细分(节点, 截止);
        std::vector<特征信息身份> 子节点;
        for (const auto& 关联 : 节点.子关联) 子节点.push_back({关联.目标节点});
        std::sort(子节点.begin(), 子节点.end(), [](const auto& 左, const auto& 右) {
            return 左.编码 < 右.编码;
        });
        return 子节点;
    });
}

inline 特征数据结果<特征信息身份> 特征类数据服务::读取细分父节点(特征信息身份 身份) const {
    return 保护执行<特征信息身份>([&] {
        auto 截止 = 开始读取();
        const auto 节点 = 读节点(身份, 截止);
        检查细分(节点, 截止);
        要求(!节点.父关联.empty(), 错误::未找到);
        return 特征信息身份{节点.父关联.front().源节点};
    });
}

inline 特征数据结果<特征信息身份> 特征类数据服务::读取值域生成子节点(
    特征信息身份 身份, 特征值域项标识 标识) const {
    return 保护执行<特征信息身份>([&] {
        auto 截止 = 开始读取();
        const auto 节点 = 读节点(身份, 截止);
        检查细分(节点, 截止);
        (void)找域(节点, 标识);
        const auto 关系 = std::find_if(节点.子关联.begin(), 节点.子关联.end(),
            [&](const auto& 项) { return static_cast<std::uint64_t>(项.角色或顺序) == 标识.值; });
        要求(关系 != 节点.子关联.end(), 错误::未找到);
        return 特征信息身份{关系->目标节点};
    });
}

inline void 特征类数据服务::检查字段(const 节点副本& 节点) {
    std::uint64_t 前键 = 0;
    for (const auto& 域 : 节点.域) {
        要求(域.标识.值 > 前键 && 浅层结构有效(域));
        前键 = 域.标识.值;
        要求(区间包含(整数域(域), 内联值(域.代表值)));
    }
    for (std::size_t 序号 = 0; 序号 < 节点.历史.size(); ++序号) {
        const auto& 项 = 节点.历史[序号];
        要求(!项.valueless_by_exception());
        if (const auto* 域键 = std::get_if<特征值域项标识>(&项))
            要求(找域(节点, *域键).命中次数 >= 3);
        else
            (void)内联值(std::get<特征观察值>(项));
        要求(std::find(节点.历史.begin(), 节点.历史.begin() + 序号, 项)
            == 节点.历史.begin() + 序号);
    }
    if (节点.当前) {
        const 特征历史值项 项{特征观察值{*节点.当前}};
        要求(std::find(节点.历史.begin(), 节点.历史.end(), 项) != 节点.历史.end()
            || std::any_of(节点.历史.begin(), 节点.历史.end(), [&](const auto& 历史) {
                const auto* 标识 = std::get_if<特征值域项标识>(&历史);
                return 标识 != nullptr && 区间包含(整数域(找域(节点, *标识)), *节点.当前);
            }), 错误::引用冲突);
    }
}

inline void 特征类数据服务::检查相容(const 节点副本& 节点,
    const 先天I64特征类型规格& 规格) {
    const auto 检查整数 = [&](std::int64_t 值) {
        要求(集合包含(规格.允许集合, {值, 值}), 错误::类型不相容);
    };
    if (节点.当前) 检查整数(*节点.当前);
    for (const auto& 项 : 节点.历史)
        if (const auto* 值 = std::get_if<特征观察值>(&项)) 检查整数(内联值(*值));
    for (const auto& 域 : 节点.域) {
        检查整数(内联值(域.代表值));
        要求(集合包含(规格.允许集合, 整数域(域)), 错误::类型不相容);
    }
}

inline 特征信息 特征类数据服务::组成完整信息(const 节点副本& 节点) {
    要求(节点.类型.has_value() && 节点.当前.has_value(), 错误::未设置);
    return {节点.身份, *节点.类型, 特征观察值{*节点.当前}, 节点.历史, 节点.域};
}

inline 特征值域项标识 特征类数据服务::新域键(const 节点副本& 节点) {
    const auto 键 = 有效(节点.域载荷) ? 节点.域载荷.值
        : (节点.类型 ? 节点.类型->编码.值 : 0);
    要求(键 != 0 && 键 <= static_cast<std::uint64_t>(
        std::numeric_limits<std::int64_t>::max()), 错误::算术不可表示);
    要求(std::none_of(节点.域.begin(), 节点.域.end(),
        [&](const auto& 域) { return 域.标识.值 == 键; }));
    return {键};
}

inline bool 特征类数据服务::相同域定义(const 特征值域项& 左,
    const 特征值域项& 右) {
    return 左.代表值 == 右.代表值 && 左.范围 == 右.范围;
}

inline 特征类数据服务::写集 特征类数据服务::初始化写集(std::uint64_t 截止) {
    写集 请求;
    请求.期望事实代次 = 截止;
    请求.写入幂等身份 = {1};
    for (std::size_t 序号 = 0; 序号 < 结构数量; ++序号) {
        std::optional<L1所有者范围值表示种类> 表示;
        if (序号 <= 域集合属性)
            表示 = 序号 == 规则误差属性 || 序号 == 当前值属性
                ? L1所有者范围值表示种类::I64 : L1所有者范围值表示种类::U64组;
        (void)加节点(请求, 表示);
    }
    return 请求;
}

inline void 特征类数据服务::规范化写集(写集& 请求) {
    const auto 按键 = [](const auto& 左, const auto& 右) { return 左.本地键 < 右.本地键; };
    std::sort(请求.节点.begin(), 请求.节点.end(), 按键);
    std::sort(请求.关系.begin(), 请求.关系.end(), 按键);
    std::sort(请求.值.begin(), 请求.值.end(), 按键);
    const auto 引用键 = [](const 引用& 值) {
        if (const auto* 编码 = std::get_if<稳定编码>(&值)) return 编码->值;
        return (std::uint64_t{1} << 63) | std::get<本地键>(值).值;
    };
    std::sort(请求.属性槽变更.begin(), 请求.属性槽变更.end(), [&](const auto& 左,
        const auto& 右) {
        if (引用键(左.所属节点) != 引用键(右.所属节点))
            return 引用键(左.所属节点) < 引用键(右.所属节点);
        if (引用键(左.属性类型节点) != 引用键(右.属性类型节点))
            return 引用键(左.属性类型节点) < 引用键(右.属性类型节点);
        return 左.新当前值 < 右.新当前值;
    });
    std::sort(请求.退出事实.begin(), 请求.退出事实.end());
}

inline 稳定编码 特征类数据服务::映射编码(const L1所有者范围写入结果& 结果,
    本地键 键) {
    const auto 项 = std::find_if(结果.新编码映射.begin(), 结果.新编码映射.end(),
        [&](const auto& 成员) { return 成员.first == 键; });
    要求(项 != 结果.新编码映射.end() && 有效(项->second));
    return 项->second;
}

inline 稳定编码 特征类数据服务::解析引用(const L1所有者范围写入结果& 结果,
    const 引用& 值) {
    if (const auto* 编码 = std::get_if<稳定编码>(&值)) return *编码;
    return 映射编码(结果, std::get<本地键>(值));
}

inline void 特征类数据服务::接受初始化映射(const L1所有者范围写入结果& 结果) {
    std::array<稳定编码, 结构数量> 候选{};
    for (std::size_t 序号 = 0; 序号 < 结构数量; ++序号)
        候选[序号] = 映射编码(结果, {static_cast<std::uint32_t>(序号 + 1)});
    结构_ = 候选;
    已初始化_ = true;
}

// 原首次结果证明 G1 发布；当前读回重新选择 G，不要求其它分区停止推进。
// 此处逐项比较原写集，不以“提交成功”代替字段、槽、关系及退出读回。
inline void 特征类数据服务::确认发布载荷(const 写集& 请求,
    const L1所有者范围写入结果& 结果) const {
    要求(结果.状态 == L1所有者范围写入状态::成功
        && 结果.合同版本 == L1所有者范围CRUD合同版本
        && 结果.所有者 == 写端口_.所有者身份()
        && 结果.写入幂等身份 == 请求.写入幂等身份
        && 结果.是否形成内存权威发布 && 结果.事实代次 != 0
        && 结果.事实代次 == 请求.期望事实代次 + 1
        && 结果.新编码映射.size() == 请求.节点.size() + 请求.关系.size() + 请求.值.size());
    for (std::size_t 序号 = 0; 序号 < 结果.新编码映射.size(); ++序号)
        for (std::size_t 前 = 0; 前 < 序号; ++前)
            要求(结果.新编码映射[前].first != 结果.新编码映射[序号].first
                && 结果.新编码映射[前].second != 结果.新编码映射[序号].second);

    L1所有者范围一致当前读取请求 读取请求;
    读取请求.所有者.push_back(写端口_.所有者身份());
    for (const auto& 项 : 请求.节点) 读取请求.节点.push_back(映射编码(结果, 项.本地键));
    for (const auto& 项 : 请求.关系) 读取请求.关系.push_back(映射编码(结果, 项.本地键));
    for (const auto& 项 : 请求.值) 读取请求.值.push_back(映射编码(结果, 项.本地键));
    for (const auto& 项 : 请求.属性槽变更)
        读取请求.属性值.push_back({解析引用(结果, 项.所属节点),
            解析引用(结果, 项.属性类型节点)});
    std::uint64_t 截止 = 0;
    const auto 读取 = 取得投影(std::move(读取请求), 截止);
    检查项目状态(读取.所有者.front().状态);
    要求(读取.所有者.front().所有者事实.has_value()
        && 读取.所有者.front().所有者事实->所有者 == 写端口_.所有者身份());
    for (std::size_t 序号 = 0; 序号 < 请求.节点.size(); ++序号) {
        const auto& 项 = 请求.节点[序号];
        const auto& 实际 = 读取.节点[序号];
        要求(实际.状态 == 读取项状态::成功 && 实际.事实.has_value());
        要求(实际.事实->编码 == 映射编码(结果, 项.本地键)
            && 实际.事实->种类 == 项.种类 && 实际.事实->属性类型表示 == 项.属性类型表示
            && 实际.事实->写入所有者 == 写端口_.所有者身份()
            && 实际.事实->创建事实代次 == 结果.事实代次 && !实际.事实->退出事实代次);
    }
    for (std::size_t 序号 = 0; 序号 < 请求.关系.size(); ++序号) {
        const auto& 项 = 请求.关系[序号];
        const auto& 实际 = 读取.关系[序号];
        要求(实际.状态 == 读取项状态::成功 && 实际.事实.has_value());
        要求(*实际.事实 == L1所有者范围关系事实{映射编码(结果, 项.本地键),
            解析引用(结果, 项.源节点), 解析引用(结果, 项.目标节点),
            解析引用(结果, 项.关系类型节点), 项.角色或顺序,
            写端口_.所有者身份(), 结果.事实代次, std::nullopt});
    }
    for (std::size_t 序号 = 0; 序号 < 请求.值.size(); ++序号) {
        const auto& 项 = 请求.值[序号];
        const auto& 实际 = 读取.值[序号];
        要求(实际.状态 == 读取项状态::成功 && 实际.事实.has_value());
        要求(*实际.事实 == L1所有者范围值事实{映射编码(结果, 项.本地键),
            解析引用(结果, 项.所属节点), 解析引用(结果, 项.属性类型节点), 项.材料,
            解析引用(结果, 项.来源节点), 写端口_.所有者身份(), 结果.事实代次, std::nullopt});
    }
    for (std::size_t 序号 = 0; 序号 < 请求.属性槽变更.size(); ++序号) {
        const auto& 项 = 请求.属性槽变更[序号];
        const auto& 实际 = 读取.属性值[序号];
        要求(实际.状态 == 读取项状态::成功 && 实际.投影.has_value()
            && 实际.投影->属性槽.当前值 == 映射编码(结果, 项.新当前值));
    }
    for (const auto 编码 : 请求.退出事实) {
        const auto 历史 = 第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, 编码});
        要求(历史.读取事实代次 == 截止, 错误::并发变化);
        要求(历史.状态 == L1所有者范围读取状态::成功 && 历史.事实.has_value());
        std::visit([&](const auto& 事实) {
            要求(事实.编码 == 编码 && 事实.写入所有者 == 写端口_.所有者身份()
                && 事实.退出事实代次 == 结果.事实代次);
        }, *历史.事实);
    }
}

inline L1所有者范围写入结果 特征类数据服务::收敛原请求() {
    try {
    要求(待确认_.has_value());
    const auto& 请求 = 待确认_->请求;
    auto 首次 = 写端口_.读取首次写入材料({L1所有者范围首次写入读取合同版本, 请求.写入幂等身份});
    if (首次.状态 == L1所有者范围读取状态::未找到) {
        // 原请求重放不重新计算输入，不分配新键，也不再次增加命中。
        const auto 重放 = 写端口_.提交所有者范围中性写集(请求);
        if (重放.状态 != L1所有者范围写入状态::成功
            && 重放.状态 != L1所有者范围写入状态::精确重复) {
            if (重放.状态 == L1所有者范围写入状态::事实代次漂移
                || 重放.状态 == L1所有者范围写入状态::引用冲突
                || 重放.状态 == L1所有者范围写入状态::入口拒绝
                || 重放.状态 == L1所有者范围写入状态::未找到
                || 重放.状态 == L1所有者范围写入状态::已退出) {
                const auto 状态 = 重放.状态;
                待确认_.reset(); // 先证实原键无发布，再证实重放在提交前拒绝。
                if (状态 == L1所有者范围写入状态::事实代次漂移) throw 错误::并发变化;
                if (状态 == L1所有者范围写入状态::引用冲突) throw 错误::引用冲突;
                if (状态 == L1所有者范围写入状态::入口拒绝) throw 错误::入口拒绝;
                throw 错误::未找到;
            }
            throw 错误::发布结果未确认;
        }
        首次 = 写端口_.读取首次写入材料({L1所有者范围首次写入读取合同版本, 请求.写入幂等身份});
    }
    要求(首次.状态 == L1所有者范围读取状态::成功
        && 首次.首次规范化写集.has_value() && 首次.首次写入结果.has_value(),
        错误::发布结果未确认);
    要求(首次.所有者 == 写端口_.所有者身份()
        && 首次.写入幂等身份 == 请求.写入幂等身份
        && *首次.首次规范化写集 == 请求);
    确认发布载荷(请求, *首次.首次写入结果);
    if (待确认_->初始化结构) 接受初始化映射(*首次.首次写入结果);
    auto 结果 = std::move(*首次.首次写入结果);
    待确认_.reset();
    return 结果;
    } catch (...) {
        // 包含已经取得首次成功、但当前读回漂移或分配失败的情形。
        // 只有已经证明原键零发布并清除 pending 的拒绝可以保持原错误。
        if (待确认_) throw 错误::发布结果未确认;
        throw;
    }
}

inline L1所有者范围写入结果 特征类数据服务::提交并读回(写集 请求,
    bool 初始化结构) {
    要求(!待确认_, 错误::前次写入待收敛);
    规范化写集(请求);
    // 必须在调用 L1 前拥有完整原请求；发布后的分配异常不能丢弃该上下文。
    待确认_.emplace(待确认写入{std::move(请求), 初始化结构});
    try {
        (void)写端口_.提交所有者范围中性写集(待确认_->请求);
        return 收敛原请求();
    } catch (...) {
        if (待确认_) throw 错误::发布结果未确认;
        throw;
    }
}

inline 特征数据结果<std::monostate> 特征类数据服务::初始化() {
    return 保护执行<std::monostate>([&] {
        const auto 截止 = 开始写入(false);
        auto 首次 = 写端口_.读取首次写入材料({L1所有者范围首次写入读取合同版本, {1}});
        if (首次.状态 == L1所有者范围读取状态::成功) {
            要求(首次.首次规范化写集.has_value() && 首次.首次写入结果.has_value());
            const auto 预期 = 初始化写集(首次.首次规范化写集->期望事实代次);
            要求(*首次.首次规范化写集 == 预期
                && 首次.所有者 == 写端口_.所有者身份());
            确认发布载荷(预期, *首次.首次写入结果);
            接受初始化映射(*首次.首次写入结果);
        } else if (首次.状态 == L1所有者范围读取状态::未找到) {
            要求(!已初始化_);
            (void)提交并读回(初始化写集(截止), true);
        } else if (首次.状态 == L1所有者范围读取状态::资源失败) {
            throw 错误::资源失败;
        } else {
            throw 错误::内部不一致;
        }
        return std::monostate{};
    });
}

inline 特征数据结果<std::monostate> 特征类数据服务::收敛待确认写入() {
    return 保护执行<std::monostate>([&] {
        if (待确认_) (void)收敛原请求();
        return std::monostate{};
    });
}

} // namespace 海中鱼巣
