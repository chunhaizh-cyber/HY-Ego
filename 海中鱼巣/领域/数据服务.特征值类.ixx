module;

#include <cstdint>
#include <new>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

export module 海中鱼巣.领域.数据服务.特征值类;

export import 海中鱼巣.核心.服务.L1事实基座;

export namespace 海中鱼巣 {

struct 特征值身份 final {
    稳定编码 编码{};
    friend bool operator==(const 特征值身份&,
        const 特征值身份&) = default;
};

inline bool 有效(特征值身份 身份) noexcept {
    return 有效(身份.编码);
}

struct 特征值独立材料引用 final {
    稳定编码 身份{};
    friend bool operator==(const 特征值独立材料引用&,
        const 特征值独立材料引用&) = default;
};

using 特征值内容 = std::variant<std::int64_t,
    std::vector<std::int64_t>, std::vector<std::uint64_t>,
    特征值独立材料引用>;

enum class 特征值表示类型 : std::uint8_t {
    I64 = 1,
    I64组 = 2,
    U64组 = 3,
    独立材料引用 = 4
};

struct 特征值信息 final {
    特征值身份 值身份{};
    特征值内容 值内容;
    friend bool operator==(const 特征值信息&,
        const 特征值信息&) = default;
};

enum class 特征值读取错误 : std::uint8_t {
    入口拒绝, 未找到, 材料已清理, 能力未提供, 资源失败, 内部不一致
};

using 特征值读取结果 = std::variant<特征值信息, 特征值读取错误>;

class 特征值类数据服务 final {
public:
    explicit 特征值类数据服务(const L1事实基座服务& 第一层服务) noexcept
        : 第一层服务_(第一层服务) {}

    特征值类数据服务() = delete;
    特征值类数据服务(const 特征值类数据服务&) = delete;
    特征值类数据服务& operator=(const 特征值类数据服务&) = delete;
    特征值类数据服务(特征值类数据服务&&) = delete;
    特征值类数据服务& operator=(特征值类数据服务&&) = delete;

    bool 绑定于(const L1事实基座服务& 第一层服务) const noexcept {
        return &第一层服务 == &第一层服务_;
    }

    static 特征值表示类型 获取值表示类型(
        const 特征值信息& 特征值) noexcept {
        return std::visit([](const auto& 值) noexcept {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, std::int64_t>)
                return 特征值表示类型::I64;
            else if constexpr (std::is_same_v<类型,
                    std::vector<std::int64_t>>)
                return 特征值表示类型::I64组;
            else if constexpr (std::is_same_v<类型,
                    std::vector<std::uint64_t>>)
                return 特征值表示类型::U64组;
            else
                return 特征值表示类型::独立材料引用;
        }, 特征值.值内容);
    }

    static 特征值身份 获取值身份(const 特征值信息& 特征值) noexcept {
        return 特征值.值身份;
    }

    static const 特征值内容& 获取值内容(
        const 特征值信息& 特征值) noexcept {
        return 特征值.值内容;
    }

    static bool 内容结构有效(const 特征值内容& 内容) noexcept {
        if (内容.valueless_by_exception()) return false;
        if (const auto* 引用 = std::get_if<特征值独立材料引用>(&内容))
            return 有效(引用->身份);
        return true;
    }

    // 诊断责任：向上送出。退出的不可变值仍可从 L1 历史事实读回；
    // 节点编码、材料墓碑和未实现材料 provider 均不会成为成功值。
    特征值读取结果 获取特征值(特征值身份 身份) const {
        if (!有效(身份)) return 特征值读取错误::入口拒绝;
        try {
            const L1所有者范围事实读取请求 请求{
                L1所有者范围CRUD合同版本, 身份.编码};
            auto 当前 = 第一层服务_.读取所有者范围当前值(请求);
            if (当前.状态 == L1所有者范围读取状态::成功)
                return 投影读取事实(身份, 当前.事实);
            if (当前.状态 == L1所有者范围读取状态::已退出) {
                auto 历史 = 第一层服务_.读取所有者范围历史事实(请求);
                if (历史.状态 == L1所有者范围读取状态::成功)
                    return 投影读取事实(身份, 历史.事实);
                return 映射读取错误(历史.状态);
            }
            return 映射读取错误(当前.状态);
        } catch (const std::bad_alloc&) {
            return 特征值读取错误::资源失败;
        } catch (const std::length_error&) {
            return 特征值读取错误::资源失败;
        } catch (...) {
            return 特征值读取错误::内部不一致;
        }
    }

private:
    static 特征值读取错误 映射读取错误(L1所有者范围读取状态 状态) noexcept {
        switch (状态) {
        case L1所有者范围读取状态::未找到:
            return 特征值读取错误::未找到;
        case L1所有者范围读取状态::历史材料已清理:
            return 特征值读取错误::材料已清理;
        case L1所有者范围读取状态::入口拒绝:
            return 特征值读取错误::入口拒绝;
        case L1所有者范围读取状态::资源失败:
            return 特征值读取错误::资源失败;
        default: return 特征值读取错误::内部不一致;
        }
    }

    static 特征值读取结果 投影读取事实(特征值身份 身份,
        const std::optional<L1所有者范围事实副本>& 事实) {
        if (!事实) return 特征值读取错误::内部不一致;
        const auto* 值 = std::get_if<L1所有者范围值事实>(&*事实);
        if (值 == nullptr) return 特征值读取错误::入口拒绝;
        if (值->编码 != 身份.编码 || !有效(值->所属节点)
            || !有效(值->属性类型节点) || !有效(值->来源节点)
            || !有效(值->写入所有者) || 值->创建事实代次 == 0
            || !L1所有者范围原始材料完整(值->材料))
            return 特征值读取错误::内部不一致;
        auto 内容 = 转换为特征值内容(值->材料);
        if (!内容) return 特征值读取错误::能力未提供;
        return 特征值信息{身份, std::move(*内容)};
    }

    // 独立材料分支必须由正式不可变材料服务强类型读回。当前正式代码
    // 没有该 provider，因此不能仅凭非零编码把它投影为已核验内容。
    static std::optional<特征值内容> 转换为特征值内容(
        const L1所有者范围原始值材料& 材料) {
        return std::visit([](const auto& 值)
                -> std::optional<特征值内容> {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型,
                    L1所有者范围独立材料引用>)
                return std::nullopt;
            else
                return 特征值内容{值};
        }, 材料);
    }

    const L1事实基座服务& 第一层服务_;
};

} // namespace 海中鱼巣
