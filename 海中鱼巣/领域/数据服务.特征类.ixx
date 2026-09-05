module;

#include <algorithm>
#include <cstdint>
#include <exception>
#include <limits>
#include <new>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

export module 海中鱼巣.领域.数据服务.特征类;

export import 海中鱼巣.领域.数据服务.特征值类;

namespace 海中鱼巣::特征类数据内部 {

inline constexpr L1所有者范围写集本地键 特征结点本地键{1};
inline constexpr L1所有者范围写集本地键 特征值本地键{2};
inline constexpr L1所有者范围写集本地键 派生定义本地键{1};
inline constexpr L1所有者范围写集本地键 实际阶次值本地键{2};
inline constexpr L1所有者范围写集本地键 派生规则值本地键{3};
inline constexpr L1所有者范围写集本地键 第一来源关系本地键{0x0001'0001U};
inline constexpr L1所有者范围写集本地键 第二来源关系本地键{0x0001'0002U};
inline constexpr L1所有者范围写集本地键 宿主关系本地键{0x0002'0001U};
inline constexpr L1所有者范围写集本地键 比较注册本地键{0x0003'0001U};
inline constexpr L1所有者范围写集本地键 注册归属关系本地键{0x0003'0002U};
inline constexpr L1所有者范围写集本地键 注册U64值本地键{0x0003'0003U};
inline constexpr L1所有者范围写集本地键 注册I64值本地键{0x0003'0004U};
inline constexpr L1所有者范围写集本地键 集合身份本地键{0x0010'0001U};
inline constexpr L1所有者范围写集本地键 集合版本本地键{0x0010'0002U};
inline constexpr L1所有者范围写集本地键 集合身份归属本地键{0x0010'0011U};
inline constexpr L1所有者范围写集本地键 集合版本归属本地键{0x0010'0012U};
inline constexpr L1所有者范围写集本地键 集合当前版本本地键{0x0010'0013U};
inline constexpr L1所有者范围写集本地键 集合前版本本地键{0x0010'0014U};
inline constexpr std::uint32_t 集合成员本地键起点 = 0x1000'0000U;

bool L1材料形状有效(const L1所有者范围原始值材料& 材料) noexcept {
    return std::visit([](const auto& 值) noexcept {
        using 类型 = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<类型, std::int64_t>) return true;
        else if constexpr (std::is_same_v<类型,
            L1所有者范围独立材料引用>) return 有效(值.编码);
        else return !值.empty();
    }, 材料);
}

L1所有者范围值表示种类 L1材料表示(
    const L1所有者范围原始值材料& 材料) noexcept {
    return std::visit([](const auto& 值) noexcept {
        using 类型 = std::decay_t<decltype(值)>;
        if constexpr (std::is_same_v<类型, std::int64_t>)
            return L1所有者范围值表示种类::I64;
        else if constexpr (std::is_same_v<类型, std::vector<std::int64_t>>)
            return L1所有者范围值表示种类::I64组;
        else if constexpr (std::is_same_v<类型, std::vector<std::uint64_t>>)
            return L1所有者范围值表示种类::U64组;
        else
            return L1所有者范围值表示种类::独立材料引用;
    }, 材料);
}

std::optional<稳定编码> 查找唯一编码(
    const L1所有者范围写入结果& 结果,
    L1所有者范围写集本地键 本地键) noexcept {
    std::optional<稳定编码> 找到;
    for (const auto& [键, 编码] : 结果.新编码映射) {
        if (键 != 本地键) continue;
        if (找到 || !有效(编码)) return std::nullopt;
        找到 = 编码;
    }
    return 找到;
}

} // namespace 特征类数据内部

export namespace 海中鱼巣 {

inline constexpr std::uint32_t 特征类数据合同版本 = 1;
inline constexpr std::uint32_t 特征类派生数据合同版本 = 1;

enum class 特征类派生数据状态 : std::uint8_t {
    已创建 = 1, 精确重复 = 2, 已读取 = 3, 已退出 = 4, 已比较 = 5,
    入口拒绝 = 6, 未找到 = 7, 目标已退出 = 8, 来源未找到 = 9,
    来源已退出 = 10, 宿主未找到 = 11, 宿主已退出 = 12,
    宿主读取失败 = 13, 事实代次漂移 = 14, 幂等冲突 = 15,
    引用冲突 = 16, 未注册 = 17, 算法版本漂移 = 18,
    不支持 = 19, 资源失败 = 20, 内部不一致 = 21, 已可能发布 = 22
};

enum class 特征类比较状态 : std::uint8_t {
    已比较 = 1, 未注册 = 2, 入口拒绝 = 3, 版本漂移 = 4,
    许可拒绝 = 5, 资源失败 = 6, 内部不一致 = 7, 未实现 = 8
};

enum class 特征类比较拒绝原因 : std::uint8_t {
    无 = 0, 未注册 = 1, 注册不唯一 = 2, 注册失效 = 3,
    算法版本不匹配 = 4, 原始类型不匹配 = 5, 单位不匹配 = 6,
    维度不匹配 = 7, 左右角色不合法 = 8, 值域不允许 = 9,
    坐标或位姿不成立 = 10, 时间基准不成立 = 11, 样本不足 = 12,
    差异不可表示 = 13, 距离不可表示 = 14, 误差合同不成立 = 15,
    输入版本失效 = 16, 请求合同版本不匹配 = 17,
    请求身份或特征定义不完整 = 18, 要求结果组合不支持 = 19,
    未实现 = 20
};

enum class 特征类比较用途 : std::uint8_t { 目标判断 = 1, 状态迁移 = 2 };
enum class 特征类比较角色 : std::uint8_t {
    当前事实 = 1, 目标状态 = 2, 前状态 = 3, 后当前事实 = 4
};
enum class 特征类比较方向 : std::uint8_t { 右减左 = 1 };
enum class 特征类比较具名关系 : std::uint8_t {
    当前低于目标 = 1, 当前达到目标 = 2, 当前高于目标 = 3,
    状态迁移增加 = 4, 状态迁移等价 = 5, 状态迁移减少 = 6
};

struct 特征类派生结构类型 final {
    稳定编码 实际阶次属性类型{};
    稳定编码 派生规则属性类型{};
    稳定编码 直接来源关系类型{};
    稳定编码 宿主关系类型{};
    稳定编码 比较注册归属关系类型{};
    稳定编码 比较注册U64属性类型{};
    稳定编码 比较注册I64属性类型{};
    friend bool operator==(const 特征类派生结构类型&,
        const 特征类派生结构类型&) = default;
};

struct 特征类定义身份 final {
    稳定编码 值{};
    friend bool operator==(const 特征类定义身份&,
        const 特征类定义身份&) = default;
};

struct 特征类比较注册身份 final {
    稳定编码 值{};
    friend bool operator==(const 特征类比较注册身份&,
        const 特征类比较注册身份&) = default;
};

struct 特征类派生来源提交项 final {
    std::uint32_t 顺序 = 0;
    std::uint32_t 输入角色 = 0;
    特征类定义身份 来源特征定义;
    friend bool operator==(const 特征类派生来源提交项&,
        const 特征类派生来源提交项&) = default;
};

struct 特征类派生来源事实 final {
    稳定编码 关系{};
    std::uint32_t 顺序 = 0;
    std::uint32_t 输入角色 = 0;
    特征类定义身份 来源特征定义;
    friend bool operator==(const 特征类派生来源事实&,
        const 特征类派生来源事实&) = default;
};

struct 特征类派生规则 final {
    稳定编码 规则身份{};
    std::uint32_t 规则版本 = 0;
    friend bool operator==(const 特征类派生规则&,
        const 特征类派生规则&) = default;
};

struct 特征类I64比较注册合同 final {
    特征类比较用途 用途 = 特征类比较用途::目标判断;
    std::uint32_t 算法版本 = 0;
    特征类比较角色 左角色 = 特征类比较角色::当前事实;
    特征类比较角色 右角色 = 特征类比较角色::目标状态;
    稳定编码 单位{};
    稳定编码 维度{};
    稳定编码 分量角色{};
    std::uint8_t 允许结果位 = 0;
    std::uint32_t 误差合同版本 = 0;
    std::optional<std::int64_t> 误差预算;
    std::optional<std::int64_t> 相等容差;
    friend bool operator==(const 特征类I64比较注册合同&,
        const 特征类I64比较注册合同&) = default;
};

struct 特征类比较注册事实 final {
    特征类比较注册身份 身份;
    特征类I64比较注册合同 合同;
    稳定编码 归属关系{};
    稳定编码 U64合同值{};
    稳定编码 I64合同值{};
    friend bool operator==(const 特征类比较注册事实&,
        const 特征类比较注册事实&) = default;
};

struct 特征类派生定义事实 final {
    特征类定义身份 身份;
    稳定编码 实际阶次值{};
    std::uint32_t 实际阶次 = 0;
    std::vector<特征类派生来源事实> 直接来源;
    特征类派生规则 派生规则;
    std::optional<稳定编码> 宿主存在;
    std::optional<稳定编码> 宿主关系;
    特征类比较注册事实 比较注册;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    friend bool operator==(const 特征类派生定义事实&,
        const 特征类派生定义事实&) = default;
};

struct 特征类派生定义新增请求 final {
    std::uint32_t 合同版本 = 特征类派生数据合同版本;
    std::uint64_t 期望事实代次 = 0;
    L1所有者范围写入幂等身份 幂等身份{};
    std::vector<特征类派生来源提交项> 直接来源;
    特征类派生规则 派生规则;
    std::optional<稳定编码> 宿主存在;
    特征类I64比较注册合同 比较合同;
    friend bool operator==(const 特征类派生定义新增请求&,
        const 特征类派生定义新增请求&) = default;
};

struct 特征类派生定义查询请求 final {
    std::uint32_t 合同版本 = 特征类派生数据合同版本;
    std::uint64_t 期望事实代次 = 0;
    特征类定义身份 特征定义;
    friend bool operator==(const 特征类派生定义查询请求&,
        const 特征类派生定义查询请求&) = default;
};

struct 特征类派生定义退出请求 final {
    std::uint32_t 合同版本 = 特征类派生数据合同版本;
    std::uint64_t 期望事实代次 = 0;
    L1所有者范围写入幂等身份 幂等身份{};
    特征类定义身份 特征定义;
    friend bool operator==(const 特征类派生定义退出请求&,
        const 特征类派生定义退出请求&) = default;
};

struct 特征类派生定义结果 final {
    特征类派生数据状态 状态 = 特征类派生数据状态::入口拒绝;
    std::uint32_t 合同版本 = 特征类派生数据合同版本;
    std::uint64_t 事实代次 = 0;
    std::optional<特征类派生定义事实> 定义;
    bool 成功() const noexcept {
        if ((状态 != 特征类派生数据状态::已创建
                && 状态 != 特征类派生数据状态::精确重复
                && 状态 != 特征类派生数据状态::已读取
                && 状态 != 特征类派生数据状态::已退出)
            || 合同版本 != 特征类派生数据合同版本
            || 事实代次 == 0 || !定义 || 定义->实际阶次 <= 1
            || 定义->直接来源.size() != 2
            || 定义->创建事实代次 == 0)
            return false;
        if (状态 == 特征类派生数据状态::已退出)
            return 定义->退出事实代次 == 事实代次;
        if (状态 == 特征类派生数据状态::精确重复)
            return !定义->退出事实代次
                || 定义->退出事实代次 == 事实代次;
        return !定义->退出事实代次;
    }
};

struct 特征类比较输入 final {
    稳定编码 事实身份{};
    std::int64_t 值 = 0;
    稳定编码 来源稳定编码{};
    std::uint64_t 输入版本 = 0;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    稳定编码 单位{};
    稳定编码 维度{};
    稳定编码 分量角色{};
    特征类比较角色 角色 = 特征类比较角色::当前事实;
    friend bool operator==(const 特征类比较输入&,
        const 特征类比较输入&) = default;
};

struct 特征类比较请求 final {
    std::uint32_t 合同版本 = 特征类派生数据合同版本;
    std::uint64_t 期望事实代次 = 0;
    std::uint64_t 请求身份 = 0;
    特征类定义身份 特征定义;
    特征类比较用途 用途 = 特征类比较用途::目标判断;
    std::optional<std::uint32_t> 预期算法版本;
    特征类比较输入 左;
    特征类比较输入 右;
    std::uint8_t 要求结果位 = 0;
    friend bool operator==(const 特征类比较请求&,
        const 特征类比较请求&) = default;
};

struct 特征类I64差异材料 final {
    std::int64_t 值 = 0;
    稳定编码 单位{};
    稳定编码 维度{};
    稳定编码 分量角色{};
    特征类比较方向 方向 = 特征类比较方向::右减左;
    std::uint32_t 算法版本 = 0;
    friend bool operator==(const 特征类I64差异材料&,
        const 特征类I64差异材料&) = default;
};

struct 特征类比较结果 final {
    std::uint64_t 请求身份 = 0;
    特征类比较状态 状态 = 特征类比较状态::入口拒绝;
    特征类比较拒绝原因 拒绝原因 = 特征类比较拒绝原因::无;
    std::uint64_t 事实代次 = 0;
    特征类比较注册身份 注册身份;
    std::uint32_t 算法版本 = 0;
    std::uint8_t 实际结果位 = 0;
    std::optional<std::int8_t> 排序三态;
    std::optional<特征类比较具名关系> 具名关系;
    std::optional<特征类I64差异材料> 差异材料;
    std::optional<std::int64_t> 误差预算;
    std::optional<std::int64_t> 相等容差;
    std::uint32_t 误差合同版本 = 0;
    稳定编码 单位{};
    稳定编码 维度{};
    稳定编码 分量角色{};
    特征类比较角色 实际左角色 = 特征类比较角色::当前事实;
    特征类比较角色 实际右角色 = 特征类比较角色::目标状态;
    稳定编码 左事实身份{};
    稳定编码 左来源稳定编码{};
    std::uint64_t 左输入版本 = 0;
    std::uint64_t 左创建事实代次 = 0;
    稳定编码 右事实身份{};
    稳定编码 右来源稳定编码{};
    std::uint64_t 右输入版本 = 0;
    std::uint64_t 右创建事实代次 = 0;
    bool 成功() const noexcept {
        const bool 角色闭合 =
            (实际左角色 == 特征类比较角色::当前事实
                && 实际右角色 == 特征类比较角色::目标状态)
            || (实际左角色 == 特征类比较角色::前状态
                && 实际右角色 == 特征类比较角色::后当前事实);
        if (状态 != 特征类比较状态::已比较
            || 拒绝原因 != 特征类比较拒绝原因::无
            || 请求身份 == 0 || 事实代次 == 0 || !有效(注册身份.值)
            || 算法版本 == 0 || 实际结果位 == 0 || 实际结果位 > 7
            || 排序三态.has_value() != ((实际结果位 & 1U) != 0)
            || 具名关系.has_value() != ((实际结果位 & 2U) != 0)
            || 差异材料.has_value() != ((实际结果位 & 4U) != 0)
            || 误差合同版本 == 0 || !有效(单位) || !有效(维度)
            || !有效(分量角色) || !有效(左事实身份)
            || !有效(左来源稳定编码) || 左输入版本 == 0
            || 左创建事实代次 == 0 || !有效(右事实身份)
            || !有效(右来源稳定编码) || 右输入版本 == 0
            || 右创建事实代次 == 0 || 左创建事实代次 > 左输入版本
            || 右创建事实代次 > 右输入版本 || !角色闭合
            || (误差预算 && *误差预算 < 0)
            || (相等容差 && *相等容差 < 0)
            || (排序三态 && (*排序三态 < -1 || *排序三态 > 1)))
            return false;
        if (具名关系) {
            const auto 数值 = static_cast<std::uint8_t>(*具名关系);
            if (数值 < 1 || 数值 > 6) return false;
            const bool 目标关系 = 数值 <= 3;
            if (目标关系 != (实际左角色 == 特征类比较角色::当前事实))
                return false;
        }
        return !差异材料 || (差异材料->单位 == 单位
            && 差异材料->维度 == 维度
            && 差异材料->分量角色 == 分量角色
            && 差异材料->方向 == 特征类比较方向::右减左
            && 差异材料->算法版本 == 算法版本);
    }
};

using 特征直接值 = std::variant<std::int64_t,
    std::vector<std::int64_t>, std::vector<std::uint64_t>>;

struct 特征值结点引用 final {
    稳定编码 结点{};
    friend bool operator==(const 特征值结点引用&,
        const 特征值结点引用&) = default;
};

using 特征类值 = std::variant<特征直接值, 特征值结点引用>;

enum class 特征类数据状态 : std::uint8_t {
    已创建 = 1,
    精确重复 = 2,
    已读取 = 3,
    已换代 = 4,
    已退出 = 5,
    入口拒绝 = 6,
    未找到 = 7,
    目标已退出 = 8,
    特征类型未找到 = 9,
    特征类型已退出 = 10,
    事实代次漂移 = 11,
    幂等冲突 = 12,
    引用冲突 = 13,
    数量预算不足 = 14,
    资源失败 = 15,
    内部不一致 = 16
};

struct 特征类结点 final {
    稳定编码 结点{};
    稳定编码 值事实{};
    稳定编码 特征类型{};
    特征类值 特征值{特征直接值{std::int64_t{0}}};
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    friend bool operator==(const 特征类结点&, const 特征类结点&) = default;
};

struct 特征类新增请求 final {
    std::uint32_t 合同版本 = 特征类数据合同版本;
    std::uint64_t 期望事实代次 = 0;
    L1所有者范围写入幂等身份 幂等身份{};
    稳定编码 特征类型{};
    特征类值 特征值{特征直接值{std::int64_t{0}}};
    friend bool operator==(const 特征类新增请求&,
        const 特征类新增请求&) = default;
};

struct 特征类修改请求 final {
    std::uint32_t 合同版本 = 特征类数据合同版本;
    std::uint64_t 期望事实代次 = 0;
    L1所有者范围写入幂等身份 幂等身份{};
    稳定编码 原特征结点{};
    稳定编码 新特征类型{};
    特征类值 新特征值{特征直接值{std::int64_t{0}}};
    friend bool operator==(const 特征类修改请求&,
        const 特征类修改请求&) = default;
};

struct 特征类删除请求 final {
    std::uint32_t 合同版本 = 特征类数据合同版本;
    std::uint64_t 期望事实代次 = 0;
    L1所有者范围写入幂等身份 幂等身份{};
    稳定编码 特征结点{};
    friend bool operator==(const 特征类删除请求&,
        const 特征类删除请求&) = default;
};

struct 特征类查询请求 final {
    std::uint32_t 合同版本 = 特征类数据合同版本;
    std::uint64_t 期望事实代次 = 0;
    稳定编码 特征结点{};
    friend bool operator==(const 特征类查询请求&,
        const 特征类查询请求&) = default;
};

struct 特征类类型值查询请求 final {
    std::uint32_t 合同版本 = 特征类数据合同版本;
    std::uint64_t 期望事实代次 = 0;
    稳定编码 特征类型{};
    特征类值 特征值{特征直接值{std::int64_t{0}}};
    std::uint64_t 最大候选数量 = 0;
    friend bool operator==(const 特征类类型值查询请求&,
        const 特征类类型值查询请求&) = default;
};

struct 特征类结点结果 final {
    特征类数据状态 状态 = 特征类数据状态::入口拒绝;
    std::uint32_t 合同版本 = 特征类数据合同版本;
    std::uint64_t 事实代次 = 0;
    std::optional<特征类结点> 特征;
    std::optional<稳定编码> 被换代特征结点;

    bool 成功() const noexcept {
        return (状态 == 特征类数据状态::已创建
                || 状态 == 特征类数据状态::精确重复
                || 状态 == 特征类数据状态::已读取
                || 状态 == 特征类数据状态::已换代
                || 状态 == 特征类数据状态::已退出)
            && 合同版本 == 特征类数据合同版本
            && 事实代次 != 0 && 特征.has_value();
    }
};

struct 特征类组查询结果 final {
    特征类数据状态 状态 = 特征类数据状态::入口拒绝;
    std::uint32_t 合同版本 = 特征类数据合同版本;
    std::uint64_t 事实代次 = 0;
    稳定编码 特征类型{};
    特征类值 特征值{特征直接值{std::int64_t{0}}};
    std::vector<特征类结点> 特征组;

    bool 成功() const noexcept {
        return 状态 == 特征类数据状态::已读取
            && 合同版本 == 特征类数据合同版本
            && 事实代次 != 0 && 有效(特征类型);
    }
};

inline constexpr std::uint32_t 特征类事实集合数据合同版本 = 1;

enum class 特征类事实集合状态 : std::uint8_t {
    已创建=1, 精确重复=2, 已换代=3, 已退出=4, 已读取=5,
    入口拒绝=6, 未找到=7, 目标已退出=8, 成员未找到=9,
    成员已退出=10, 当前版本漂移=11, 事实代次漂移=12,
    幂等冲突=13, 引用冲突=14, 数量超限=15, 结构未配置=16,
    资源失败=17, 内部不一致=18, 已可能发布=19
};

struct 特征类事实集合身份 final {
    稳定编码 值{};
    friend bool operator==(const 特征类事实集合身份&, const 特征类事实集合身份&) = default;
};
struct 特征类事实集合版本身份 final {
    稳定编码 值{};
    friend bool operator==(const 特征类事实集合版本身份&, const 特征类事实集合版本身份&) = default;
};
struct 特征类事实集合结构类型 final {
    稳定编码 集合锚点{}, 集合族归属关系类型{}, 当前版本关系类型{}, 成员关系类型{}, 前版本关系类型{};
    std::size_t 最大成员数 = 0;
    friend bool operator==(const 特征类事实集合结构类型&, const 特征类事实集合结构类型&) = default;
};
struct 特征类事实集合成员提交项 final {
    std::uint32_t 顺序 = 0, 成员角色 = 0;
    稳定编码 特征{};
    friend bool operator==(const 特征类事实集合成员提交项&, const 特征类事实集合成员提交项&) = default;
};
struct 特征类事实集合成员事实 final {
    稳定编码 关系{};
    std::uint32_t 顺序 = 0, 成员角色 = 0;
    特征类结点 特征;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    friend bool operator==(const 特征类事实集合成员事实&, const 特征类事实集合成员事实&) = default;
};
struct 特征类事实集合版本事实 final {
    特征类事实集合版本身份 版本身份;
    特征类事实集合身份 集合身份;
    std::optional<特征类事实集合版本身份> 前版本身份;
    std::optional<稳定编码> 前版本关系;
    std::vector<特征类事实集合成员事实> 成员;
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    friend bool operator==(const 特征类事实集合版本事实&, const 特征类事实集合版本事实&) = default;
};
struct 特征类事实集合事实 final {
    特征类事实集合身份 集合身份;
    特征类事实集合版本事实 版本事实;
    稳定编码 当前版本关系{};
    std::uint64_t 创建事实代次 = 0;
    std::optional<std::uint64_t> 退出事实代次;
    friend bool operator==(const 特征类事实集合事实&, const 特征类事实集合事实&) = default;
};
struct 特征类事实集合新增请求 final {
    std::uint32_t 合同版本 = 特征类事实集合数据合同版本;
    std::uint64_t 期望事实代次 = 0;
    L1所有者范围写入幂等身份 幂等身份{};
    std::vector<特征类事实集合成员提交项> 成员;
    friend bool operator==(const 特征类事实集合新增请求&, const 特征类事实集合新增请求&) = default;
};
struct 特征类事实集合换代请求 final {
    std::uint32_t 合同版本 = 特征类事实集合数据合同版本;
    std::uint64_t 期望事实代次 = 0;
    L1所有者范围写入幂等身份 幂等身份{};
    特征类事实集合身份 集合身份;
    特征类事实集合版本身份 预期当前版本;
    std::vector<特征类事实集合成员提交项> 新成员;
    friend bool operator==(const 特征类事实集合换代请求&, const 特征类事实集合换代请求&) = default;
};
struct 特征类事实集合退出请求 final {
    std::uint32_t 合同版本 = 特征类事实集合数据合同版本;
    std::uint64_t 期望事实代次 = 0;
    L1所有者范围写入幂等身份 幂等身份{};
    特征类事实集合身份 集合身份;
    特征类事实集合版本身份 预期当前版本;
    friend bool operator==(const 特征类事实集合退出请求&, const 特征类事实集合退出请求&) = default;
};
struct 特征类事实集合查询请求 final {
    std::uint32_t 合同版本 = 特征类事实集合数据合同版本;
    std::uint64_t 期望事实代次 = 0;
    特征类事实集合身份 集合身份;
    friend bool operator==(const 特征类事实集合查询请求&, const 特征类事实集合查询请求&) = default;
};
struct 特征类事实集合历史查询请求 final {
    std::uint32_t 合同版本 = 特征类事实集合数据合同版本;
    std::uint64_t 读取事实代次 = 0, 历史截止事实代次 = 0;
    特征类事实集合身份 集合身份;
    friend bool operator==(const 特征类事实集合历史查询请求&, const 特征类事实集合历史查询请求&) = default;
};
struct 特征类事实集合版本查询请求 final {
    std::uint32_t 合同版本 = 特征类事实集合数据合同版本;
    std::uint64_t 读取事实代次 = 0, 历史截止事实代次 = 0;
    特征类事实集合版本身份 版本身份;
    friend bool operator==(const 特征类事实集合版本查询请求&, const 特征类事实集合版本查询请求&) = default;
};
struct 特征类事实集合成员反查请求 final {
    std::uint32_t 合同版本 = 特征类事实集合数据合同版本;
    std::uint64_t 期望事实代次 = 0;
    稳定编码 成员特征{};
    std::size_t 数量预算 = 0;
    friend bool operator==(const 特征类事实集合成员反查请求&, const 特征类事实集合成员反查请求&) = default;
};
struct 特征类事实集合结果 final {
    特征类事实集合状态 状态 = 特征类事实集合状态::入口拒绝;
    std::uint64_t 事实截止代次 = 0;
    std::optional<std::uint64_t> 首次提交事实代次;
    std::optional<特征类事实集合事实> 集合;
    friend bool operator==(const 特征类事实集合结果&, const 特征类事实集合结果&) = default;
};
struct 特征类事实集合版本结果 final {
    特征类事实集合状态 状态 = 特征类事实集合状态::入口拒绝;
    std::uint64_t 事实截止代次 = 0;
    std::optional<特征类事实集合版本事实> 版本;
    friend bool operator==(const 特征类事实集合版本结果&, const 特征类事实集合版本结果&) = default;
};
struct 特征类事实集合组结果 final {
    特征类事实集合状态 状态 = 特征类事实集合状态::入口拒绝;
    std::uint64_t 事实截止代次 = 0;
    std::vector<特征类事实集合事实> 集合组;
    friend bool operator==(const 特征类事实集合组结果&, const 特征类事实集合组结果&) = default;
};

struct 特征类型确认请求 final {
    std::uint32_t 合同版本 = 1;
    std::uint64_t Gread = 0;
    稳定编码 特征类型{};
};
struct 特征类型历史请求 final {
    std::uint32_t 合同版本 = 1;
    std::uint64_t Gread = 0;
    std::uint64_t H = 0;
    稳定编码 特征类型{};
};
struct 特征类型历史结果 final {
    特征引用读取状态 状态 = 特征引用读取状态::入口拒绝;
    std::uint32_t 合同版本 = 1;
    std::uint64_t Gread = 0;
    std::uint64_t H = 0;
    稳定编码 特征类型{};
    std::optional<L1所有者范围值表示种类> 表示;
    bool 成功() const noexcept {
        return 状态 == 特征引用读取状态::已读取 && 合同版本 == 1 && H && Gread >= H && 有效(特征类型)
               && 表示.has_value();
    }
};
using 特征类型确认结果 = 特征类型历史结果;
struct 特征按实例历史读取请求 final {
    std::uint32_t 合同版本 = 1;
    std::uint64_t Gread = 0;
    std::uint64_t H = 0;
    稳定编码 实例{};
    std::uint64_t 最大属性值数 = 0;
};
struct 特征历史读取请求 final {
    std::uint32_t 合同版本 = 1;
    std::uint64_t Gread = 0;
    std::uint64_t H = 0;
    稳定编码 实例{}, 值事实{};
};
struct 特征历史读取结果 final {
    特征引用读取状态 状态 = 特征引用读取状态::入口拒绝;
    std::uint32_t 合同版本 = 1;
    std::uint64_t Gread = 0;
    std::uint64_t H = 0;
    std::optional<特征类结点> 特征;
    bool 成功() const noexcept {
        return 状态 == 特征引用读取状态::已读取 && 合同版本 == 1 && H && Gread >= H && 特征
               && 有效(特征->结点) && 有效(特征->值事实) && 有效(特征->特征类型)
               && 特征->创建事实代次 && 特征->创建事实代次 <= H && !特征->退出事实代次;
    }
};

class 特征类数据服务 final {
public:
    bool 绑定于(const L1事实基座服务& s) const noexcept {
        return &s == &第一层服务_;
    }
    特征类型确认结果 确认特征类型(const 特征类型确认请求& r) const {
        return 读取特征类型历史事实({r.合同版本, r.Gread, r.Gread, r.特征类型});
    }
    特征类型历史结果 读取特征类型历史事实(const 特征类型历史请求& r) const {
        using S = 特征引用读取状态;
        特征类型历史结果 o;
        o.Gread = r.Gread;
        o.H = r.H;
        o.特征类型 = r.特征类型;
        if (r.合同版本 != 1 || !r.H || r.H > r.Gread || !有效(r.特征类型))
            return o;
        try {
            auto x = 第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, r.特征类型});
            if (x.状态 != L1所有者范围读取状态::成功) {
                o.状态 = 特征引用历史状态(x.状态);
                return o;
            }
            if (x.读取事实代次 != r.Gread) {
                o.状态 = S::事实代次漂移;
                return o;
            }
            auto n = x.事实 ? std::get_if<L1所有者范围节点事实>(&*x.事实) : nullptr;
            if (x.合同版本 != L1所有者范围CRUD合同版本 || x.查询编码 != r.特征类型 || !n
                || n->编码 != r.特征类型 || n->写入所有者 != 所有者_ || n->种类 != 节点种类::属性类型
                || !n->属性类型表示) {
                o.状态 = S::引用冲突;
                return o;
            }
            if (!n->创建事实代次 || n->创建事实代次 > r.H) {
                o.状态 = S::未找到;
                return o;
            }
            if (n->退出事实代次 && *n->退出事实代次 <= r.H) {
                o.状态 = S::目标已退出;
                return o;
            }
            auto last = 第一层服务_.读取中性当前事实代次({L1中性CRUD合同版本});
            if (last.状态 != L1中性读取状态::成功) {
                o.状态 = last.状态 == L1中性读取状态::资源失败 ? S::资源失败 : S::内部不一致;
                return o;
            }
            if (last.事实代次 != r.Gread) {
                o.状态 = S::事实代次漂移;
                return o;
            }
            o.状态 = S::已读取;
            o.表示 = n->属性类型表示;
            return o;
        } catch (const std::bad_alloc&) {
            o.状态 = S::资源失败;
            return o;
        } catch (...) {
            o.状态 = S::内部不一致;
            return o;
        }
    }
    特征历史读取结果 按实例读取特征历史事实(const 特征按实例历史读取请求& r) const {
        using S = 特征引用读取状态;
        特征历史读取结果 out;
        out.Gread = r.Gread;
        out.H = r.H;
        const auto fail = [&](S status) {
            out.状态 = status;
            return out;
        };
        if (r.合同版本 != 1 || !有效(r.实例) || !r.H || r.H > r.Gread || !r.最大属性值数)
            return out;
        try {
            const auto first = 第一层服务_.读取中性当前事实代次({L1中性CRUD合同版本});
            if (first.状态 != L1中性读取状态::成功)
                return fail(first.状态 == L1中性读取状态::资源失败 ? S::资源失败 : S::内部不一致);
            if (first.事实代次 != r.Gread)
                return fail(S::事实代次漂移);
            const auto nr = 第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, r.实例});
            if (nr.状态 != L1所有者范围读取状态::成功)
                return fail(特征引用历史状态(nr.状态));
            if (nr.读取事实代次 != r.Gread)
                return fail(S::事实代次漂移);
            const auto* n = nr.事实 ? std::get_if<L1所有者范围节点事实>(&*nr.事实) : nullptr;
            if (nr.合同版本 != L1所有者范围CRUD合同版本 || nr.查询编码 != r.实例)
                return fail(S::内部不一致);
            if (!n || n->编码 != r.实例 || n->写入所有者 != 所有者_ || n->种类 != 节点种类::普通
                || n->属性类型表示)
                return fail(S::引用冲突);
            if (!n->创建事实代次)
                return fail(S::内部不一致);
            if (n->创建事实代次 > r.H)
                return fail(S::未找到);
            if (n->退出事实代次 && *n->退出事实代次 <= r.H)
                return fail(S::目标已退出);
            const auto rows = 第一层服务_.读取所有者范围历史属性值组({L1所有者范围CRUD合同版本, r.实例, r.H});
            if (rows.状态 != L1所有者范围读取状态::成功)
                return fail(特征引用历史状态(rows.状态));
            if (rows.读取事实代次 != r.Gread)
                return fail(S::事实代次漂移);
            if (rows.合同版本 != L1所有者范围CRUD合同版本 || rows.所属节点 != r.实例
                || rows.历史截止事实代次 != r.H)
                return fail(S::内部不一致);
            if (rows.属性值组.size() > r.最大属性值数)
                return fail(S::数量预算不足);
            if (rows.属性值组.empty())
                return fail(S::历史材料不可用);
            if (rows.属性值组.size() != 1)
                return fail(S::内部不一致);
            const auto& value = rows.属性值组.front();
            if (!有效(value.编码) || value.写入所有者 != 所有者_ || value.所属节点 != r.实例
                || value.来源节点 != r.实例 || !有效(value.属性类型节点))
                return fail(S::引用冲突);
            if (!value.创建事实代次 || value.创建事实代次 > r.H
                || (value.退出事实代次 && *value.退出事实代次 <= r.H) || value.创建事实代次 != n->创建事实代次
                || value.退出事实代次 != n->退出事实代次)
                return fail(S::内部不一致);
            auto result = 读取特征历史事实({1, r.Gread, r.H, r.实例, value.编码});
            if (!result.成功())
                return fail(result.状态);
            if (result.Gread != r.Gread || result.H != r.H || result.特征->结点 != r.实例
                || result.特征->值事实 != value.编码)
                return fail(S::内部不一致);
            const auto last = 第一层服务_.读取中性当前事实代次({L1中性CRUD合同版本});
            if (last.状态 != L1中性读取状态::成功)
                return fail(last.状态 == L1中性读取状态::资源失败 ? S::资源失败 : S::内部不一致);
            if (last.事实代次 != r.Gread)
                return fail(S::事实代次漂移);
            return result;
        } catch (const std::bad_alloc&) {
            return fail(S::资源失败);
        } catch (...) {
            return fail(S::内部不一致);
        }
    }

    特征历史读取结果 读取特征历史事实(const 特征历史读取请求& r) const {

        using S = 特征引用读取状态;
        特征历史读取结果 out;
        out.Gread = r.Gread;
        out.H = r.H;
        auto fail = [&](S s) {
            out.状态 = s;
            return out;
        };
        if (r.合同版本 != 1 || !r.H || r.H > r.Gread || !有效(r.实例) || !有效(r.值事实))
            return out;
        try {
            auto first = 第一层服务_.读取中性当前事实代次({L1中性CRUD合同版本});
            if (first.状态 != L1中性读取状态::成功)
                return fail(first.状态 == L1中性读取状态::资源失败 ? S::资源失败 : S::内部不一致);
            if (first.事实代次 != r.Gread)
                return fail(S::事实代次漂移);
            auto nr = 第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, r.实例});
            auto vr = 第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, r.值事实});
            if (nr.状态 != L1所有者范围读取状态::成功)
                return fail(特征引用历史状态(nr.状态));
            if (vr.状态 != L1所有者范围读取状态::成功)
                return fail(特征引用历史状态(vr.状态));
            if (nr.读取事实代次 != r.Gread || vr.读取事实代次 != r.Gread)
                return fail(S::事实代次漂移);
            auto n = nr.事实 ? std::get_if<L1所有者范围节点事实>(&*nr.事实) : nullptr;
            auto v = vr.事实 ? std::get_if<L1所有者范围值事实>(&*vr.事实) : nullptr;
            if (nr.合同版本 != L1所有者范围CRUD合同版本 || vr.合同版本 != L1所有者范围CRUD合同版本
                || nr.查询编码 != r.实例 || vr.查询编码 != r.值事实 || !n || !v || n->编码 != r.实例
                || v->编码 != r.值事实 || n->写入所有者 != 所有者_ || v->写入所有者 != 所有者_
                || n->种类 != 节点种类::普通 || n->属性类型表示 || v->所属节点 != n->编码
                || n->创建事实代次 != v->创建事实代次 || n->退出事实代次 != v->退出事实代次)
                return fail(S::引用冲突);
            if (!n->创建事实代次 || !v->创建事实代次 || n->创建事实代次 > r.H || v->创建事实代次 > r.H)
                return fail(S::未找到);
            if ((n->退出事实代次 && *n->退出事实代次 <= r.H) || (v->退出事实代次 && *v->退出事实代次 <= r.H))
                return fail(S::目标已退出);
            if (v->来源节点 != n->编码 || !特征类数据内部::L1材料形状有效(v->材料))
                return fail(S::引用冲突);
            auto t = 读取特征类型历史事实({1, r.Gread, r.H, v->属性类型节点});
            if (!t.成功())
                return fail(t.状态);
            if (*t.表示 != 特征类数据内部::L1材料表示(v->材料))
                return fail(S::引用冲突);
            auto last = 第一层服务_.读取中性当前事实代次({L1中性CRUD合同版本});
            if (last.状态 != L1中性读取状态::成功)
                return fail(last.状态 == L1中性读取状态::资源失败 ? S::资源失败 : S::内部不一致);
            if (last.事实代次 != r.Gread)
                return fail(S::事实代次漂移);
            out.状态 = S::已读取;
            out.特征 = 特征类结点{n->编码,         v->编码,     v->属性类型节点, 转换为特征类值(v->材料),
                                  n->创建事实代次, std::nullopt};
            return out;
        } catch (const std::bad_alloc&) {
            return fail(S::资源失败);
        } catch (...) {
            return fail(S::内部不一致);
        }
    }

    特征类数据服务() = delete;
    特征类数据服务(const 特征类数据服务&) = delete;
    特征类数据服务& operator=(const 特征类数据服务&) = delete;
    特征类数据服务(特征类数据服务&&) = delete;
    特征类数据服务& operator=(特征类数据服务&&) = delete;

    // 特征结点有自己的 L1 owner；特征值类只验证被引用的特征值结点。
    特征类数据服务(const L1事实基座服务& 第一层服务,
        const 特征值类数据服务& 特征值服务,
        L1所有者范围写端口&& 写入端口,
        特征类派生结构类型 派生结构类型)
        : 第一层服务_(第一层服务), 特征值服务_(特征值服务),
          写入端口_(std::move(写入端口)),
          所有者_(写入端口_.所有者身份()),
          派生结构类型_(派生结构类型) {
        if (!写入端口_.有效() || !写入端口_.绑定于(第一层服务_)
            || !有效(所有者_))
            throw std::invalid_argument("invalid feature data owner port");
        const auto 读取 = 第一层服务_.读取当前结构所有者(
            {L1所有者范围CRUD合同版本, 所有者_});
        if (读取.状态 != L1所有者范围读取状态::成功
            || 读取.合同版本 != L1所有者范围CRUD合同版本
            || !读取.所有者事实 || 读取.所有者事实->所有者 != 所有者_
            || 读取.所有者事实->范围种类
                != L1所有者范围种类::独占结构范围
            || 读取.所有者事实->退出事实代次)
            throw std::invalid_argument("invalid feature data owner");
        const std::vector<稳定编码> 类型组{
            派生结构类型_.实际阶次属性类型,
            派生结构类型_.派生规则属性类型,
            派生结构类型_.直接来源关系类型,
            派生结构类型_.宿主关系类型,
            派生结构类型_.比较注册归属关系类型,
            派生结构类型_.比较注册U64属性类型,
            派生结构类型_.比较注册I64属性类型};
        if (std::any_of(类型组.begin(), 类型组.end(),
                [](稳定编码 编码) noexcept { return !有效(编码); }))
            throw std::invalid_argument("invalid derived feature types");
        auto 排序类型 = 类型组;
        std::sort(排序类型.begin(), 排序类型.end());
        if (std::adjacent_find(排序类型.begin(), 排序类型.end())
                != 排序类型.end())
            throw std::invalid_argument("duplicate derived feature types");
        if (!类型结点有效(派生结构类型_.实际阶次属性类型,
                节点种类::属性类型, L1所有者范围值表示种类::I64)
            || !类型结点有效(派生结构类型_.派生规则属性类型,
                节点种类::属性类型, L1所有者范围值表示种类::U64组)
            || !类型结点有效(派生结构类型_.直接来源关系类型,
                节点种类::普通, std::nullopt)
            || !类型结点有效(派生结构类型_.宿主关系类型,
                节点种类::普通, std::nullopt)
            || !类型结点有效(派生结构类型_.比较注册归属关系类型,
                节点种类::普通, std::nullopt)
            || !类型结点有效(派生结构类型_.比较注册U64属性类型,
                节点种类::属性类型, L1所有者范围值表示种类::U64组)
            || !类型结点有效(派生结构类型_.比较注册I64属性类型,
                节点种类::属性类型, L1所有者范围值表示种类::I64组))
            throw std::invalid_argument("invalid derived feature type shape");
    }

    特征类数据服务(const L1事实基座服务& 第一层服务,
        const 特征值类数据服务& 特征值服务,
        L1所有者范围写端口&& 写入端口,
        特征类派生结构类型 派生结构类型,
        特征类事实集合结构类型 事实集合结构类型)
        : 特征类数据服务(第一层服务, 特征值服务,
            std::move(写入端口), 派生结构类型) {
        if (事实集合结构配置有效(事实集合结构类型))
            事实集合结构类型_ = 事实集合结构类型;
    }

    // 诊断责任：向上送出；一份 L1 写集创建独立特征结点及其类型值槽。
    特征类结点结果 新增特征(const 特征类新增请求& 请求) {
        if (!新增请求有效(请求)) return 失败(特征类数据状态::入口拒绝);
        try {
            if (const auto 重放 = 尝试重放新特征(请求.期望事实代次,
                    请求.幂等身份, 请求.特征类型, 请求.特征值,
                    std::nullopt, 特征类数据状态::已创建))
                return *重放;
            if (const auto 类型状态 = 验证特征类型(
                    请求.特征类型, 请求.特征值, 请求.期望事实代次))
                return 失败(*类型状态, 请求.期望事实代次);
            if (const auto 引用状态 = 验证特征值结点引用(
                    请求.特征值, 请求.期望事实代次))
                return 失败(*引用状态, 请求.期望事实代次);
            const auto 写集 = 形成新特征写集(请求.期望事实代次,
                请求.幂等身份, 请求.特征类型, 请求.特征值, {});
            return 提交新特征(写集, 请求.幂等身份, std::nullopt,
                特征类数据状态::已创建);
        } catch (const std::bad_alloc&) {
            return 失败(特征类数据状态::资源失败);
        } catch (...) {
            return 失败(特征类数据状态::内部不一致);
        }
    }

    // 诊断责任：向上送出；修改生成新结点并在同一写集退出旧结点和值。
    特征类结点结果 修改特征(const 特征类修改请求& 请求) {
        if (!修改请求有效(请求)) return 失败(特征类数据状态::入口拒绝);
        try {
            if (const auto 重放 = 尝试重放新特征(请求.期望事实代次,
                    请求.幂等身份, 请求.新特征类型, 请求.新特征值,
                    请求.原特征结点, 特征类数据状态::已换代))
                return *重放;
            const auto 原特征 = 读取当前特征(
                请求.原特征结点, 请求.期望事实代次);
            if (!原特征.成功()) return 原特征;
            if (const auto 类型状态 = 验证特征类型(
                    请求.新特征类型, 请求.新特征值,
                    请求.期望事实代次))
                return 失败(*类型状态, 请求.期望事实代次);
            if (const auto 引用状态 = 验证特征值结点引用(
                    请求.新特征值, 请求.期望事实代次))
                return 失败(*引用状态, 请求.期望事实代次);
            const auto 写集 = 形成新特征写集(请求.期望事实代次,
                请求.幂等身份, 请求.新特征类型, 请求.新特征值,
                {原特征.特征->结点, 原特征.特征->值事实});
            return 提交新特征(写集, 请求.幂等身份,
                请求.原特征结点, 特征类数据状态::已换代);
        } catch (const std::bad_alloc&) {
            return 失败(特征类数据状态::资源失败);
        } catch (...) {
            return 失败(特征类数据状态::内部不一致);
        }
    }

    // 诊断责任：向上送出；删除只退出生命周期，不物理删除事实。
    特征类结点结果 删除特征(const 特征类删除请求& 请求) {
        if (!删除请求有效(请求)) return 失败(特征类数据状态::入口拒绝);
        try {
            if (const auto 重放 = 尝试重放删除(请求)) return *重放;
            const auto 当前 = 读取当前特征(
                请求.特征结点, 请求.期望事实代次);
            if (!当前.成功()) return 当前;
            L1所有者范围写集请求 写集;
            写集.合同版本 = L1所有者范围CRUD合同版本;
            写集.期望事实代次 = 请求.期望事实代次;
            写集.写入幂等身份 = 请求.幂等身份;
            写集.退出事实 = {当前.特征->结点, 当前.特征->值事实};
            const auto 写入 = 写入端口_.提交所有者范围中性写集(写集);
            const auto 状态 = 映射写入状态(
                写入.状态, 特征类数据状态::已退出);
            if (状态 != 特征类数据状态::已退出
                && 状态 != 特征类数据状态::精确重复)
                return 失败(状态, 写入.事实代次);
            if (!写入结果头完整(写入, 请求.幂等身份))
                return 失败(特征类数据状态::内部不一致,
                    写入.事实代次);
            auto 读回 = 读取历史已退出特征(
                请求.特征结点, 写入.事实代次);
            if (!读回.成功()) return 读回;
            读回.状态 = 状态;
            return 读回;
        } catch (const std::bad_alloc&) {
            return 失败(特征类数据状态::资源失败);
        } catch (...) {
            return 失败(特征类数据状态::内部不一致);
        }
    }

    // 诊断责任：向上送出；按稳定结点身份读取同截止特征数据。
    特征类结点结果 查询特征(const 特征类查询请求& 请求) const {
        if (!查询请求有效(请求)) return 失败(特征类数据状态::入口拒绝);
        try {
            return 读取当前特征(请求.特征结点, 请求.期望事实代次);
        } catch (const std::bad_alloc&) {
            return 失败(特征类数据状态::资源失败);
        } catch (...) {
            return 失败(特征类数据状态::内部不一致);
        }
    }

    // 诊断责任：向上送出；按类型枚举 L1 当前值后精确匹配值材料。
    特征类组查询结果 按特征类型和值查询(
        const 特征类类型值查询请求& 请求) const {
        if (!类型值查询请求有效(请求)) return 组失败(
            特征类数据状态::入口拒绝, 请求);
        if (const auto 类型状态 = 验证特征类型(
                请求.特征类型, 请求.特征值, 请求.期望事实代次))
            return 组失败(*类型状态, 请求, 请求.期望事实代次);
        if (const auto 引用状态 = 验证特征值结点引用(
                请求.特征值, 请求.期望事实代次))
            return 组失败(*引用状态, 请求, 请求.期望事实代次);
        try {
            const auto 读取 = 第一层服务_.按属性类型读取所有者范围全部当前值(
                {L1所有者范围属性类型当前值组读取合同版本, 所有者_,
                    请求.特征类型, 请求.期望事实代次,
                    请求.最大候选数量});
            const auto 状态 = 映射组读取状态(读取.状态);
            if (状态 != 特征类数据状态::已读取)
                return 组失败(状态, 请求, 读取.读取事实代次);
            if (读取.合同版本
                    != L1所有者范围属性类型当前值组读取合同版本
                || 读取.所有者 != 所有者_
                || 读取.属性类型节点 != 请求.特征类型
                || 读取.期望事实代次 != 请求.期望事实代次
                || 读取.读取事实代次 != 请求.期望事实代次)
                return 组失败(特征类数据状态::内部不一致,
                    请求, 读取.读取事实代次);

            特征类组查询结果 结果{特征类数据状态::已读取,
                特征类数据合同版本, 读取.读取事实代次,
                请求.特征类型, 请求.特征值, {}};
            const auto 查询材料 = 转换为L1材料(请求.特征值);
            for (const auto& 值 : 读取.当前值) {
                if (值.写入所有者 != 所有者_
                    || 值.属性类型节点 != 请求.特征类型
                    || 值.创建事实代次 == 0
                    || 值.创建事实代次 > 读取.读取事实代次
                    || 值.退出事实代次 || !有效(值.编码)
                    || !有效(值.所属节点)
                    || 值.来源节点 != 值.所属节点
                    || !特征类数据内部::L1材料形状有效(值.材料))
                    return 组失败(特征类数据状态::内部不一致,
                        请求, 读取.读取事实代次);
                if (值.材料 != 查询材料) continue;
                const auto 特征读取 = 读取当前特征(
                    值.所属节点, 请求.期望事实代次);
                if (!特征读取.成功()
                    || 特征读取.特征->值事实 != 值.编码
                    || 特征读取.特征->特征类型 != 请求.特征类型
                    || 特征读取.特征->特征值 != 请求.特征值)
                    return 组失败(特征类数据状态::内部不一致,
                        请求, 读取.读取事实代次);
                结果.特征组.push_back(*特征读取.特征);
            }
            std::sort(结果.特征组.begin(), 结果.特征组.end(),
                [](const 特征类结点& 左, const 特征类结点& 右) noexcept {
                    return 左.结点.值 < 右.结点.值;
                });
            if (std::adjacent_find(结果.特征组.begin(), 结果.特征组.end(),
                    [](const 特征类结点& 左,
                        const 特征类结点& 右) noexcept {
                        return 左.结点 == 右.结点;
                    }) != 结果.特征组.end())
                return 组失败(特征类数据状态::内部不一致,
                    请求, 读取.读取事实代次);
            return 结果;
        } catch (const std::bad_alloc&) {
            return 组失败(特征类数据状态::资源失败, 请求);
        } catch (...) {
            return 组失败(特征类数据状态::内部不一致, 请求);
        }
    }

    特征类派生定义结果 新增派生特征定义(
        const 特征类派生定义新增请求& 请求) {
        if (!派生新增请求有效(请求))
            return 派生失败(特征类派生数据状态::入口拒绝);
        try {
            if (const auto 重放 = 尝试重放派生新增(请求)) return *重放;
            std::vector<稳定编码> 路径;
            const auto 左 = 读取统一定义(
                请求.直接来源[0].来源特征定义, 请求.期望事实代次, 路径);
            if (!左.成功)
                return 派生失败(映射来源状态(左.状态), 左.事实代次);
            路径.clear();
            const auto 右 = 读取统一定义(
                请求.直接来源[1].来源特征定义, 请求.期望事实代次, 路径);
            if (!右.成功)
                return 派生失败(映射来源状态(右.状态), 右.事实代次);
            const auto 最大阶次 = std::max(左.实际阶次, 右.实际阶次);
            if (最大阶次 == std::numeric_limits<std::uint32_t>::max())
                return 派生失败(特征类派生数据状态::不支持,
                    请求.期望事实代次);
            if (请求.宿主存在) {
                const auto 宿主 = 第一层服务_.读取所有者范围当前节点(
                    {L1所有者范围CRUD合同版本, *请求.宿主存在});
                if (宿主.读取事实代次 != 请求.期望事实代次)
                    return 派生失败(特征类派生数据状态::事实代次漂移,
                        宿主.读取事实代次);
                const auto* 宿主事实 = 宿主.事实
                    ? std::get_if<L1所有者范围节点事实>(&*宿主.事实) : nullptr;
                if (宿主.状态 == L1所有者范围读取状态::未找到)
                    return 派生失败(特征类派生数据状态::宿主未找到,
                        宿主.读取事实代次);
                if (宿主.状态 == L1所有者范围读取状态::已退出)
                    return 派生失败(特征类派生数据状态::宿主已退出,
                        宿主.读取事实代次);
                if (宿主.状态 != L1所有者范围读取状态::成功
                    || !宿主事实 || 宿主事实->编码 != *请求.宿主存在
                    || 宿主事实->种类 != 节点种类::普通
                    || 宿主事实->属性类型表示 || 宿主事实->退出事实代次)
                    return 派生失败(特征类派生数据状态::宿主读取失败,
                        宿主.读取事实代次);
            }
            return 提交派生新增(形成派生新增写集(
                请求, 最大阶次 + 1), 请求, 特征类派生数据状态::已创建);
        } catch (const std::bad_alloc&) {
            return 派生失败(特征类派生数据状态::资源失败);
        } catch (const std::length_error&) {
            return 派生失败(特征类派生数据状态::资源失败);
        } catch (...) {
            return 派生失败(特征类派生数据状态::内部不一致);
        }
    }

    特征类派生定义结果 查询派生特征定义(
        const 特征类派生定义查询请求& 请求) const {
        if (!派生查询请求有效(请求))
            return 派生失败(特征类派生数据状态::入口拒绝);
        try {
            std::vector<稳定编码> 路径;
            const auto 读取 = 读取统一定义(
                请求.特征定义, 请求.期望事实代次, 路径);
            if (!读取.成功 || !读取.派生)
                return 派生失败(读取.成功
                        ? 特征类派生数据状态::未找到 : 读取.状态,
                    读取.事实代次);
            return {特征类派生数据状态::已读取,
                特征类派生数据合同版本, 读取.事实代次, 读取.派生};
        } catch (const std::bad_alloc&) {
            return 派生失败(特征类派生数据状态::资源失败);
        } catch (const std::length_error&) {
            return 派生失败(特征类派生数据状态::资源失败);
        } catch (...) {
            return 派生失败(特征类派生数据状态::内部不一致);
        }
    }

    特征类派生定义结果 退出派生特征定义(
        const 特征类派生定义退出请求& 请求) {
        if (!派生退出请求有效(请求))
            return 派生失败(特征类派生数据状态::入口拒绝);
        try {
            if (const auto 重放 = 尝试重放派生退出(请求)) return *重放;
            const auto 当前 = 查询派生特征定义({特征类派生数据合同版本,
                请求.期望事实代次, 请求.特征定义});
            if (!当前.成功() || 当前.状态 != 特征类派生数据状态::已读取)
                return 当前;
            const auto 规则值 = 读取派生规则值身份(
                请求.特征定义.值, 请求.期望事实代次);
            if (!规则值)
                return 派生失败(特征类派生数据状态::内部不一致,
                    请求.期望事实代次);
            const auto 写集 = 形成派生退出写集(
                请求, *当前.定义, *规则值);
            const auto 写入 = 写入端口_.提交所有者范围中性写集(写集);
            const auto 状态 = 映射派生写入状态(
                写入.状态, 特征类派生数据状态::已退出);
            if (状态 != 特征类派生数据状态::已退出
                && 状态 != 特征类派生数据状态::精确重复)
                return 派生失败(状态, 写入.事实代次);
            if (!写入结果头完整(写入, 请求.幂等身份)
                || !写入.新编码映射.empty())
                return 派生失败(写入.状态 == L1所有者范围写入状态::成功
                        ? 特征类派生数据状态::已可能发布
                        : 特征类派生数据状态::内部不一致,
                    写入.事实代次);
            if (!退出事实同代闭合(写集.退出事实, 写入.事实代次))
                return 派生失败(特征类派生数据状态::已可能发布,
                    写入.事实代次);
            auto 定义 = *当前.定义;
            定义.退出事实代次 = 写入.事实代次;
            return {状态, 特征类派生数据合同版本,
                写入.事实代次, std::move(定义)};
        } catch (const std::bad_alloc&) {
            return 派生失败(特征类派生数据状态::资源失败);
        } catch (const std::length_error&) {
            return 派生失败(特征类派生数据状态::资源失败);
        } catch (...) {
            return 派生失败(特征类派生数据状态::内部不一致);
        }
    }

    特征类比较结果 比较派生特征(const 特征类比较请求& 请求) const {
        if (!比较请求有效(请求))
            return 比较失败(请求.请求身份, 特征类比较状态::入口拒绝,
                请求.合同版本 != 特征类派生数据合同版本
                    ? 特征类比较拒绝原因::请求合同版本不匹配
                    : 特征类比较拒绝原因::请求身份或特征定义不完整);
        try {
            const auto 读取 = 查询派生特征定义({特征类派生数据合同版本,
                请求.期望事实代次, 请求.特征定义});
            if (!读取.成功()) {
                if (读取.状态 == 特征类派生数据状态::未找到
                    || 读取.状态 == 特征类派生数据状态::目标已退出)
                    return 比较失败(请求.请求身份, 特征类比较状态::未注册,
                        特征类比较拒绝原因::未注册, 读取.事实代次);
                if (读取.状态 == 特征类派生数据状态::资源失败)
                    return 比较失败(请求.请求身份, 特征类比较状态::资源失败,
                        特征类比较拒绝原因::无, 读取.事实代次);
                return 比较失败(请求.请求身份, 特征类比较状态::内部不一致,
                    特征类比较拒绝原因::注册失效, 读取.事实代次);
            }
            const auto& 定义 = *读取.定义;
            const auto& 注册 = 定义.比较注册;
            if (请求.预期算法版本
                && *请求.预期算法版本 != 注册.合同.算法版本)
                return 比较失败(请求.请求身份, 特征类比较状态::版本漂移,
                    特征类比较拒绝原因::算法版本不匹配, 读取.事实代次);
            if (请求.用途 != 注册.合同.用途)
                return 比较失败(请求.请求身份, 特征类比较状态::入口拒绝,
                    特征类比较拒绝原因::左右角色不合法, 读取.事实代次);
            if (请求.要求结果位 == 0 || 请求.要求结果位 > 7
                || (请求.要求结果位 & ~注册.合同.允许结果位) != 0)
                return 比较失败(请求.请求身份, 特征类比较状态::入口拒绝,
                    特征类比较拒绝原因::要求结果组合不支持, 读取.事实代次);
            if (请求.左.角色 != 注册.合同.左角色
                || 请求.右.角色 != 注册.合同.右角色)
                return 比较失败(请求.请求身份, 特征类比较状态::入口拒绝,
                    特征类比较拒绝原因::左右角色不合法, 读取.事实代次);
            if (请求.左.单位 != 注册.合同.单位
                || 请求.右.单位 != 注册.合同.单位)
                return 比较失败(请求.请求身份, 特征类比较状态::入口拒绝,
                    特征类比较拒绝原因::单位不匹配, 读取.事实代次);
            if (请求.左.维度 != 注册.合同.维度
                || 请求.右.维度 != 注册.合同.维度)
                return 比较失败(请求.请求身份, 特征类比较状态::入口拒绝,
                    特征类比较拒绝原因::维度不匹配, 读取.事实代次);
            if (请求.左.分量角色 != 注册.合同.分量角色
                || 请求.右.分量角色 != 注册.合同.分量角色)
                return 比较失败(请求.请求身份, 特征类比较状态::入口拒绝,
                    特征类比较拒绝原因::原始类型不匹配, 读取.事实代次);
            if (请求.左.来源稳定编码
                    != 定义.直接来源[0].来源特征定义.值
                || 请求.右.来源稳定编码
                    != 定义.直接来源[1].来源特征定义.值)
                return 比较失败(请求.请求身份, 特征类比较状态::入口拒绝,
                    特征类比较拒绝原因::请求身份或特征定义不完整,
                    读取.事实代次);
            if (!比较输入当前(请求.左, 请求.期望事实代次)
                || !比较输入当前(请求.右, 请求.期望事实代次))
                return 比较失败(请求.请求身份, 特征类比较状态::入口拒绝,
                    特征类比较拒绝原因::输入版本失效, 读取.事实代次);
            if (!验证比较输入事实(请求.左, 请求.期望事实代次)
                || !验证比较输入事实(请求.右, 请求.期望事实代次))
                return 比较失败(请求.请求身份, 特征类比较状态::入口拒绝,
                    特征类比较拒绝原因::输入版本失效, 读取.事实代次);
            if (定义.派生规则.规则版本 != 注册.合同.算法版本)
                return 比较失败(请求.请求身份, 特征类比较状态::内部不一致,
                    特征类比较拒绝原因::注册失效, 读取.事实代次);

            特征类比较结果 结果;
            结果.请求身份 = 请求.请求身份;
            结果.状态 = 特征类比较状态::已比较;
            结果.事实代次 = 读取.事实代次;
            结果.注册身份 = 注册.身份;
            结果.算法版本 = 注册.合同.算法版本;
            结果.实际结果位 = 请求.要求结果位;
            const std::int8_t 排序 = 请求.左.值 < 请求.右.值 ? -1
                : 请求.左.值 > 请求.右.值 ? 1 : 0;
            if ((请求.要求结果位 & 1U) != 0) 结果.排序三态 = 排序;
            const auto 差异幅度 = I64差异幅度(请求.左.值, 请求.右.值);
            const bool 容差相等 = 注册.合同.相等容差
                && 差异幅度 <= static_cast<std::uint64_t>(*注册.合同.相等容差);
            if ((请求.要求结果位 & 2U) != 0) {
                if (请求.用途 == 特征类比较用途::目标判断)
                    结果.具名关系 = 容差相等 || 排序 == 0
                        ? 特征类比较具名关系::当前达到目标
                        : 排序 < 0 ? 特征类比较具名关系::当前低于目标
                                   : 特征类比较具名关系::当前高于目标;
                else
                    结果.具名关系 = 容差相等 || 排序 == 0
                        ? 特征类比较具名关系::状态迁移等价
                        : 排序 < 0 ? 特征类比较具名关系::状态迁移增加
                                   : 特征类比较具名关系::状态迁移减少;
            }
            if ((请求.要求结果位 & 4U) != 0) {
                if (!I64差异可表示(请求.左.值, 请求.右.值))
                    return 比较失败(请求.请求身份, 特征类比较状态::入口拒绝,
                        特征类比较拒绝原因::差异不可表示, 读取.事实代次);
                结果.差异材料 = 特征类I64差异材料{
                    请求.右.值 - 请求.左.值, 注册.合同.单位,
                    注册.合同.维度, 注册.合同.分量角色,
                    特征类比较方向::右减左, 注册.合同.算法版本};
            }
            填充比较回显(结果, 请求, 注册.合同);
            return 结果.成功() ? 结果
                : 比较失败(请求.请求身份, 特征类比较状态::内部不一致,
                    特征类比较拒绝原因::注册失效, 读取.事实代次);
        } catch (const std::bad_alloc&) {
            return 比较失败(请求.请求身份, 特征类比较状态::资源失败,
                特征类比较拒绝原因::无);
        } catch (const std::length_error&) {
            return 比较失败(请求.请求身份, 特征类比较状态::资源失败,
                特征类比较拒绝原因::无);
        } catch (...) {
            return 比较失败(请求.请求身份, 特征类比较状态::内部不一致,
                特征类比较拒绝原因::注册失效);
        }
    }

    特征类事实集合结果 新增特征事实集合(const 特征类事实集合新增请求& 请求) noexcept {
        if (!事实集合结构类型_) return 集合失败(特征类事实集合状态::结构未配置);
        if (!集合新增请求有效(请求)) return 集合失败(特征类事实集合状态::入口拒绝);
        try { return 执行新增事实集合(请求); }
        catch (const std::bad_alloc&) { return 集合失败(特征类事实集合状态::资源失败); }
        catch (const std::length_error&) { return 集合失败(特征类事实集合状态::资源失败); }
        catch (...) { return 集合失败(特征类事实集合状态::内部不一致); }
    }

    特征类事实集合结果 换代特征事实集合成员(const 特征类事实集合换代请求& 请求) noexcept {
        if (!事实集合结构类型_) return 集合失败(特征类事实集合状态::结构未配置);
        if (!集合换代请求有效(请求)) return 集合失败(特征类事实集合状态::入口拒绝);
        try { return 执行换代事实集合(请求); }
        catch (const std::bad_alloc&) { return 集合失败(特征类事实集合状态::资源失败); }
        catch (const std::length_error&) { return 集合失败(特征类事实集合状态::资源失败); }
        catch (...) { return 集合失败(特征类事实集合状态::内部不一致); }
    }

    特征类事实集合结果 退出特征事实集合(const 特征类事实集合退出请求& 请求) noexcept {
        if (!事实集合结构类型_) return 集合失败(特征类事实集合状态::结构未配置);
        if (!集合退出请求有效(请求)) return 集合失败(特征类事实集合状态::入口拒绝);
        try { return 执行退出事实集合(请求); }
        catch (const std::bad_alloc&) { return 集合失败(特征类事实集合状态::资源失败); }
        catch (const std::length_error&) { return 集合失败(特征类事实集合状态::资源失败); }
        catch (...) { return 集合失败(特征类事实集合状态::内部不一致); }
    }

    特征类事实集合结果 查询当前特征事实集合(const 特征类事实集合查询请求& 请求) const noexcept {
        if (!事实集合结构类型_) return 集合失败(特征类事实集合状态::结构未配置);
        if (请求.合同版本 != 特征类事实集合数据合同版本 || 请求.期望事实代次 == 0 || !有效(请求.集合身份.值))
            return 集合失败(特征类事实集合状态::入口拒绝);
        try { return 读取集合截止(请求.集合身份.值, 请求.期望事实代次, 请求.期望事实代次, false); }
        catch (const std::bad_alloc&) { return 集合失败(特征类事实集合状态::资源失败); }
        catch (const std::length_error&) { return 集合失败(特征类事实集合状态::资源失败); }
        catch (...) { return 集合失败(特征类事实集合状态::内部不一致); }
    }

    特征类事实集合结果 查询历史特征事实集合(const 特征类事实集合历史查询请求& 请求) const noexcept {
        if (!事实集合结构类型_) return 集合失败(特征类事实集合状态::结构未配置);
        if (请求.合同版本 != 特征类事实集合数据合同版本 || 请求.读取事实代次 == 0
            || 请求.历史截止事实代次 == 0 || 请求.历史截止事实代次 > 请求.读取事实代次
            || !有效(请求.集合身份.值)) return 集合失败(特征类事实集合状态::入口拒绝);
        try { return 读取集合截止(请求.集合身份.值, 请求.读取事实代次, 请求.历史截止事实代次, true); }
        catch (const std::bad_alloc&) { return 集合失败(特征类事实集合状态::资源失败); }
        catch (const std::length_error&) { return 集合失败(特征类事实集合状态::资源失败); }
        catch (...) { return 集合失败(特征类事实集合状态::内部不一致); }
    }

    特征类事实集合版本结果 查询特征事实集合版本(const 特征类事实集合版本查询请求& 请求) const noexcept {
        if (!事实集合结构类型_) return 版本失败(特征类事实集合状态::结构未配置);
        if (请求.合同版本 != 特征类事实集合数据合同版本 || 请求.读取事实代次 == 0
            || 请求.历史截止事实代次 == 0 || 请求.历史截止事实代次 > 请求.读取事实代次
            || !有效(请求.版本身份.值)) return 版本失败(特征类事实集合状态::入口拒绝);
        try { return 读取版本截止(请求.版本身份.值, 请求.读取事实代次, 请求.历史截止事实代次); }
        catch (const std::bad_alloc&) { return 版本失败(特征类事实集合状态::资源失败); }
        catch (const std::length_error&) { return 版本失败(特征类事实集合状态::资源失败); }
        catch (...) { return 版本失败(特征类事实集合状态::内部不一致); }
    }

    特征类事实集合组结果 按成员特征查询当前事实集合组(const 特征类事实集合成员反查请求& 请求) const noexcept {
        if (!事实集合结构类型_) return 集合组失败(特征类事实集合状态::结构未配置);
        if (请求.合同版本 != 特征类事实集合数据合同版本 || 请求.期望事实代次 == 0
            || !有效(请求.成员特征) || 请求.数量预算 == 0)
            return 集合组失败(特征类事实集合状态::入口拒绝);
        try { return 反查当前事实集合(请求); }
        catch (const std::bad_alloc&) { return 集合组失败(特征类事实集合状态::资源失败); }
        catch (const std::length_error&) { return 集合组失败(特征类事实集合状态::资源失败); }
        catch (...) { return 集合组失败(特征类事实集合状态::内部不一致); }
    }

private:
    struct 集合节点读取值 final {
        特征类事实集合状态 状态 = 特征类事实集合状态::内部不一致;
        std::uint64_t 读取事实代次 = 0;
        std::optional<L1所有者范围节点事实> 节点;
    };

    struct 集合关系组读取值 final {
        特征类事实集合状态 状态 = 特征类事实集合状态::内部不一致;
        std::uint64_t 读取事实代次 = 0;
        std::vector<L1所有者范围关系事实> 关系组;
    };

    static 特征类事实集合结果 集合失败(特征类事实集合状态 状态,
        std::uint64_t 事实截止代次 = 0,
        std::optional<std::uint64_t> 首次提交事实代次 = std::nullopt) noexcept {
        return {状态, 事实截止代次, 首次提交事实代次, std::nullopt};
    }

    static 特征类事实集合版本结果 版本失败(
        特征类事实集合状态 状态, std::uint64_t 事实截止代次 = 0) noexcept {
        return {状态, 事实截止代次, std::nullopt};
    }

    static 特征类事实集合组结果 集合组失败(
        特征类事实集合状态 状态, std::uint64_t 事实截止代次 = 0) noexcept {
        return {状态, 事实截止代次, {}};
    }

    bool 事实集合结构配置有效(const 特征类事实集合结构类型& 配置) const {
        if (配置.最大成员数 == 0) return false;
        std::vector<稳定编码> 编码组{配置.集合锚点, 配置.集合族归属关系类型,
            配置.当前版本关系类型, 配置.成员关系类型, 配置.前版本关系类型};
        if (std::any_of(编码组.begin(), 编码组.end(),
                [](稳定编码 编码) noexcept { return !有效(编码); })) return false;
        std::vector<稳定编码> 排序{配置.集合族归属关系类型,
            配置.当前版本关系类型, 配置.成员关系类型, 配置.前版本关系类型};
        std::sort(排序.begin(), 排序.end());
        if (std::adjacent_find(排序.begin(), 排序.end()) != 排序.end()) return false;
        return std::all_of(编码组.begin(), 编码组.end(), [this](稳定编码 编码) {
            return 类型结点有效(编码, 节点种类::普通, std::nullopt);
        });
    }

    bool 成员提交组有效(const std::vector<特征类事实集合成员提交项>& 成员) const {
        if (!事实集合结构类型_ || 成员.empty()
            || 成员.size() > 事实集合结构类型_->最大成员数
            || 成员.size() > std::numeric_limits<std::uint32_t>::max()
                - 特征类数据内部::集合成员本地键起点) return false;
        std::vector<稳定编码> 身份;
        身份.reserve(成员.size());
        for (std::size_t i = 0; i < 成员.size(); ++i) {
            if (成员[i].顺序 != i + 1 || 成员[i].成员角色 == 0
                || !有效(成员[i].特征)) return false;
            身份.push_back(成员[i].特征);
        }
        std::sort(身份.begin(), 身份.end());
        return std::adjacent_find(身份.begin(), 身份.end()) == 身份.end();
    }

    bool 集合新增请求有效(const 特征类事实集合新增请求& 请求) const {
        return 请求.合同版本 == 特征类事实集合数据合同版本
            && 请求.期望事实代次 != 0 && 请求.幂等身份.值 != 0
            && 成员提交组有效(请求.成员);
    }
    bool 集合换代请求有效(const 特征类事实集合换代请求& 请求) const {
        return 请求.合同版本 == 特征类事实集合数据合同版本
            && 请求.期望事实代次 != 0 && 请求.幂等身份.值 != 0
            && 有效(请求.集合身份.值) && 有效(请求.预期当前版本.值)
            && 成员提交组有效(请求.新成员);
    }
    static bool 集合退出请求有效(const 特征类事实集合退出请求& 请求) noexcept {
        return 请求.合同版本 == 特征类事实集合数据合同版本
            && 请求.期望事实代次 != 0 && 请求.幂等身份.值 != 0
            && 有效(请求.集合身份.值) && 有效(请求.预期当前版本.值);
    }

    static std::int64_t 编码成员角色顺序(std::uint32_t 顺序,
        std::uint32_t 角色) noexcept {
        return static_cast<std::int64_t>((static_cast<std::uint64_t>(顺序) << 32U) | 角色);
    }
    static bool 解码成员角色顺序(std::int64_t 编码,
        std::uint32_t& 顺序, std::uint32_t& 角色) noexcept {
        const auto 位 = static_cast<std::uint64_t>(编码);
        顺序 = static_cast<std::uint32_t>(位 >> 32U);
        角色 = static_cast<std::uint32_t>(位);
        return 顺序 != 0 && 角色 != 0;
    }

    static 特征类事实集合状态 映射集合读取状态(L1所有者范围读取状态 状态) noexcept {
        switch (状态) {
        case L1所有者范围读取状态::未找到: return 特征类事实集合状态::未找到;
        case L1所有者范围读取状态::已退出: return 特征类事实集合状态::目标已退出;
        case L1所有者范围读取状态::资源失败: return 特征类事实集合状态::资源失败;
        case L1所有者范围读取状态::入口拒绝: return 特征类事实集合状态::入口拒绝;
        default: return 特征类事实集合状态::内部不一致;
        }
    }

    static 特征类事实集合状态 映射集合写入状态(L1所有者范围写入状态 状态,
        特征类事实集合状态 成功状态) noexcept {
        switch (状态) {
        case L1所有者范围写入状态::成功: return 成功状态;
        case L1所有者范围写入状态::精确重复: return 特征类事实集合状态::精确重复;
        case L1所有者范围写入状态::入口拒绝: return 特征类事实集合状态::入口拒绝;
        case L1所有者范围写入状态::未找到: return 特征类事实集合状态::未找到;
        case L1所有者范围写入状态::已退出: return 特征类事实集合状态::目标已退出;
        case L1所有者范围写入状态::事实代次漂移: return 特征类事实集合状态::事实代次漂移;
        case L1所有者范围写入状态::幂等冲突: return 特征类事实集合状态::幂等冲突;
        case L1所有者范围写入状态::引用冲突: return 特征类事实集合状态::引用冲突;
        case L1所有者范围写入状态::资源失败: return 特征类事实集合状态::资源失败;
        default: return 特征类事实集合状态::内部不一致;
        }
    }

    集合节点读取值 读取集合节点(稳定编码 编码, std::uint64_t 读取代次,
        std::uint64_t 截止, bool 历史) const {
        L1所有者范围历史读取结果 读取;
        if (历史) {
            读取 = 第一层服务_.读取所有者范围历史事实(
                {L1所有者范围CRUD合同版本, 编码});
        } else {
            const auto 当前 = 第一层服务_.读取所有者范围当前节点(
                {L1所有者范围CRUD合同版本, 编码});
            读取 = {当前.状态, 当前.合同版本, 当前.查询编码,
                当前.读取事实代次, 当前.事实, 当前.物理清理事实代次,
                当前.物理清理墓碑};
        }
        if (读取.状态 != L1所有者范围读取状态::成功)
            return {映射集合读取状态(读取.状态), 读取.读取事实代次, std::nullopt};
        if (读取.合同版本 != L1所有者范围CRUD合同版本 || 读取.查询编码 != 编码
            || 读取.读取事实代次 != 读取代次 || 读取.物理清理事实代次 || 读取.物理清理墓碑)
            return {读取.读取事实代次 != 读取代次 ? 特征类事实集合状态::事实代次漂移
                : 特征类事实集合状态::内部不一致, 读取.读取事实代次, std::nullopt};
        const auto* 节点 = 读取.事实 ? std::get_if<L1所有者范围节点事实>(&*读取.事实) : nullptr;
        if (!节点 || 节点->编码 != 编码 || 节点->写入所有者 != 所有者_
            || 节点->种类 != 节点种类::普通 || 节点->属性类型表示
            || 节点->创建事实代次 == 0 || 节点->创建事实代次 > 截止
            || !节点->当前属性.empty())
            return {特征类事实集合状态::内部不一致, 读取.读取事实代次, std::nullopt};
        if (节点->退出事实代次 && *节点->退出事实代次 <= 截止)
            return {特征类事实集合状态::目标已退出, 读取.读取事实代次, std::nullopt};
        return {特征类事实集合状态::已读取, 读取.读取事实代次, *节点};
    }

    集合关系组读取值 读取集合关系组(L1所有者范围关系端点方向 方向,
        稳定编码 端点, 稳定编码 类型, std::uint64_t 读取代次,
        std::uint64_t 截止, bool 历史) const {
        if (历史) {
            const auto 读取 = 第一层服务_.读取所有者范围历史关系组(
                {L1所有者范围CRUD合同版本, 方向, 端点, 类型, 截止});
            if (读取.状态 != L1所有者范围读取状态::成功)
                return {映射集合读取状态(读取.状态), 读取.读取事实代次, {}};
            if (读取.合同版本 != L1所有者范围CRUD合同版本 || 读取.方向 != 方向
                || 读取.端点节点 != 端点 || 读取.关系类型节点 != 类型
                || 读取.历史截止事实代次 != 截止 || 读取.读取事实代次 != 读取代次)
                return {读取.读取事实代次 != 读取代次 ? 特征类事实集合状态::事实代次漂移
                    : 特征类事实集合状态::内部不一致, 读取.读取事实代次, {}};
            return {特征类事实集合状态::已读取, 读取.读取事实代次, 读取.关系组};
        }
        if (方向 == L1所有者范围关系端点方向::源) {
            const auto 读取 = 第一层服务_.读取所有者范围当前源关系组(
                {L1所有者范围CRUD合同版本, 端点, 类型});
            if (读取.状态 != L1所有者范围读取状态::成功)
                return {映射集合读取状态(读取.状态), 读取.读取事实代次, {}};
            if (读取.合同版本 != L1所有者范围CRUD合同版本 || 读取.源节点 != 端点
                || 读取.关系类型节点 != 类型 || 读取.读取事实代次 != 读取代次)
                return {读取.读取事实代次 != 读取代次 ? 特征类事实集合状态::事实代次漂移
                    : 特征类事实集合状态::内部不一致, 读取.读取事实代次, {}};
            return {特征类事实集合状态::已读取, 读取.读取事实代次, 读取.关系组};
        }
        const auto 读取 = 第一层服务_.读取所有者范围当前目标关系组(
            {L1所有者范围CRUD合同版本, 端点, 类型});
        if (读取.状态 != L1所有者范围读取状态::成功)
            return {映射集合读取状态(读取.状态), 读取.读取事实代次, {}};
        if (读取.合同版本 != L1所有者范围CRUD合同版本 || 读取.目标节点 != 端点
            || 读取.关系类型节点 != 类型 || 读取.读取事实代次 != 读取代次)
            return {读取.读取事实代次 != 读取代次 ? 特征类事实集合状态::事实代次漂移
                : 特征类事实集合状态::内部不一致, 读取.读取事实代次, {}};
        return {特征类事实集合状态::已读取, 读取.读取事实代次, 读取.关系组};
    }

    bool 集合关系有效(const L1所有者范围关系事实& 关系, 稳定编码 源,
        稳定编码 目标, 稳定编码 类型, std::int64_t 角色,
        std::uint64_t 截止) const noexcept {
        return 有效(关系.编码) && 关系.源节点 == 源 && 关系.目标节点 == 目标
            && 关系.关系类型节点 == 类型 && 关系.写入所有者 == 所有者_
            && 关系.角色或顺序 == 角色 && 关系.创建事实代次 != 0
            && 关系.创建事实代次 <= 截止
            && (!关系.退出事实代次 || *关系.退出事实代次 > 截止);
    }

    std::optional<特征类结点> 读取截止普通特征(稳定编码 编码,
        std::uint64_t 读取代次, std::uint64_t 截止, bool 历史,
        特征类事实集合状态& 失败状态) const {
        if (!历史) {
            const auto 结果 = 读取当前特征(编码, 读取代次);
            if (结果.成功() && 结果.状态 == 特征类数据状态::已读取 && 结果.特征)
                return 结果.特征;
            if (结果.状态 == 特征类数据状态::未找到) 失败状态 = 特征类事实集合状态::成员未找到;
            else if (结果.状态 == 特征类数据状态::目标已退出) 失败状态 = 特征类事实集合状态::成员已退出;
            else if (结果.状态 == 特征类数据状态::事实代次漂移) 失败状态 = 特征类事实集合状态::事实代次漂移;
            else if (结果.状态 == 特征类数据状态::资源失败) 失败状态 = 特征类事实集合状态::资源失败;
            else 失败状态 = 特征类事实集合状态::内部不一致;
            return std::nullopt;
        }
        const auto 节点读取 = 第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, 编码});
        if (节点读取.状态 != L1所有者范围读取状态::成功 || 节点读取.读取事实代次 != 读取代次) {
            失败状态 = 节点读取.读取事实代次 != 读取代次 ? 特征类事实集合状态::事实代次漂移
                : (节点读取.状态 == L1所有者范围读取状态::未找到 ? 特征类事实集合状态::成员未找到
                    : 节点读取.状态 == L1所有者范围读取状态::资源失败 ? 特征类事实集合状态::资源失败
                    : 特征类事实集合状态::内部不一致);
            return std::nullopt;
        }
        const auto* 节点 = 节点读取.事实 ? std::get_if<L1所有者范围节点事实>(&*节点读取.事实) : nullptr;
        if (!节点 || 节点->编码 != 编码 || 节点->写入所有者 != 所有者_
            || 节点->种类 != 节点种类::普通 || 节点->属性类型表示
            || 节点->创建事实代次 == 0 || 节点->创建事实代次 > 截止
            || (节点->退出事实代次 && *节点->退出事实代次 <= 截止)
            || 节点->当前属性.size() != 1) {
            失败状态 = 节点 && 节点->退出事实代次 && *节点->退出事实代次 <= 截止
                ? 特征类事实集合状态::成员已退出 : 特征类事实集合状态::内部不一致;
            return std::nullopt;
        }
        const auto& 槽 = 节点->当前属性.front();
        const auto 值读取 = 第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, 槽.当前值});
        const auto* 值 = 值读取.事实 ? std::get_if<L1所有者范围值事实>(&*值读取.事实) : nullptr;
        if (值读取.状态 != L1所有者范围读取状态::成功 || 值读取.读取事实代次 != 读取代次
            || !值 || 值->编码 != 槽.当前值 || 值->所属节点 != 编码
            || 值->属性类型节点 != 槽.属性类型节点 || 值->来源节点 != 编码
            || 值->写入所有者 != 所有者_ || 值->创建事实代次 != 节点->创建事实代次
            || (值->退出事实代次 && *值->退出事实代次 <= 截止)
            || !特征类数据内部::L1材料形状有效(值->材料)) {
            失败状态 = 特征类事实集合状态::内部不一致; return std::nullopt;
        }
        失败状态 = 特征类事实集合状态::已读取;
        return 特征类结点{编码, 值->编码, 值->属性类型节点,
            转换为特征类值(值->材料), 节点->创建事实代次, std::nullopt};
    }

    特征类事实集合结果 读取集合截止(稳定编码 集合编码,
        std::uint64_t 读取代次, std::uint64_t 截止, bool 历史) const {
        const auto& 配置 = *事实集合结构类型_;
        const auto 集合节点 = 读取集合节点(集合编码, 读取代次, 截止, 历史);
        if (!集合节点.节点) return 集合失败(集合节点.状态);
        const auto 归属 = 读取集合关系组(L1所有者范围关系端点方向::源,
            集合编码, 配置.集合族归属关系类型, 读取代次, 截止, 历史);
        if (归属.状态 != 特征类事实集合状态::已读取) return 集合失败(归属.状态);
        if (归属.关系组.size() != 1 || !集合关系有效(归属.关系组.front(),
                集合编码, 配置.集合锚点, 配置.集合族归属关系类型, 1, 截止))
            return 集合失败(特征类事实集合状态::内部不一致);
        const auto 当前 = 读取集合关系组(L1所有者范围关系端点方向::源,
            集合编码, 配置.当前版本关系类型, 读取代次, 截止, 历史);
        if (当前.状态 != 特征类事实集合状态::已读取) return 集合失败(当前.状态);
        if (当前.关系组.size() != 1) return 集合失败(特征类事实集合状态::内部不一致);
        const auto& 当前关系 = 当前.关系组.front();
        if (!集合关系有效(当前关系, 集合编码, 当前关系.目标节点,
                配置.当前版本关系类型, 1, 截止))
            return 集合失败(特征类事实集合状态::内部不一致);
        const auto 版本编码 = 当前关系.目标节点;
        const auto 版本节点 = 读取集合节点(版本编码, 读取代次, 截止, 历史);
        if (!版本节点.节点) return 集合失败(版本节点.状态);
        const auto 版本归属 = 读取集合关系组(L1所有者范围关系端点方向::源,
            版本编码, 配置.集合族归属关系类型, 读取代次, 截止, 历史);
        if (版本归属.状态 != 特征类事实集合状态::已读取) return 集合失败(版本归属.状态);
        if (版本归属.关系组.size() != 1 || !集合关系有效(版本归属.关系组.front(),
                版本编码, 配置.集合锚点, 配置.集合族归属关系类型, 2, 截止))
            return 集合失败(特征类事实集合状态::内部不一致);
        const auto 前版 = 读取集合关系组(L1所有者范围关系端点方向::源,
            版本编码, 配置.前版本关系类型, 读取代次, 截止, 历史);
        if (前版.状态 != 特征类事实集合状态::已读取) return 集合失败(前版.状态);
        if (前版.关系组.size() > 1) return 集合失败(特征类事实集合状态::内部不一致);
        特征类事实集合版本事实 版本;
        版本.版本身份 = {版本编码};
        版本.集合身份 = {集合编码};
        版本.创建事实代次 = 版本节点.节点->创建事实代次;
        if (!前版.关系组.empty()) {
            const auto& 关系 = 前版.关系组.front();
            if (!集合关系有效(关系, 版本编码, 关系.目标节点,
                    配置.前版本关系类型, 1, 截止))
                return 集合失败(特征类事实集合状态::内部不一致);
            版本.前版本身份 = 特征类事实集合版本身份{关系.目标节点};
            版本.前版本关系 = 关系.编码;
        }
        const auto 成员 = 读取集合关系组(L1所有者范围关系端点方向::源,
            版本编码, 配置.成员关系类型, 读取代次, 截止, 历史);
        if (成员.状态 != 特征类事实集合状态::已读取) return 集合失败(成员.状态);
        if (成员.关系组.empty() || 成员.关系组.size() > 配置.最大成员数)
            return 集合失败(成员.关系组.size() > 配置.最大成员数
                ? 特征类事实集合状态::数量超限 : 特征类事实集合状态::内部不一致);
        for (const auto& 关系 : 成员.关系组) {
            std::uint32_t 顺序 = 0, 角色 = 0;
            if (!解码成员角色顺序(关系.角色或顺序, 顺序, 角色)
                || !集合关系有效(关系, 版本编码, 关系.目标节点,
                    配置.成员关系类型, 关系.角色或顺序, 截止))
                return 集合失败(特征类事实集合状态::内部不一致);
            特征类事实集合状态 失败 = 特征类事实集合状态::内部不一致;
            auto 特征 = 读取截止普通特征(关系.目标节点, 读取代次, 截止, 历史, 失败);
            if (!特征) return 集合失败(失败);
            版本.成员.push_back({关系.编码, 顺序, 角色, std::move(*特征),
                关系.创建事实代次, std::nullopt});
        }
        std::sort(版本.成员.begin(), 版本.成员.end(), [](const auto& 左, const auto& 右) {
            return 左.顺序 != 右.顺序 ? 左.顺序 < 右.顺序 : 左.关系 < 右.关系;
        });
        std::vector<稳定编码> 成员身份;
        for (std::size_t i = 0; i < 版本.成员.size(); ++i) {
            if (版本.成员[i].顺序 != i + 1) return 集合失败(特征类事实集合状态::内部不一致);
            成员身份.push_back(版本.成员[i].特征.结点);
        }
        std::sort(成员身份.begin(), 成员身份.end());
        if (std::adjacent_find(成员身份.begin(), 成员身份.end()) != 成员身份.end())
            return 集合失败(特征类事实集合状态::内部不一致);
        特征类事实集合事实 集合{{集合编码}, std::move(版本), 当前关系.编码,
            集合节点.节点->创建事实代次, std::nullopt};
        return {特征类事实集合状态::已读取, 截止, std::nullopt, std::move(集合)};
    }

    特征类事实集合版本结果 读取版本截止(稳定编码 版本编码,
        std::uint64_t 读取代次, std::uint64_t 截止) const {
        const auto& 配置 = *事实集合结构类型_;
        const auto 原版本节点 = 读取集合节点(版本编码, 读取代次, 截止, true);
        if (!原版本节点.节点) return 版本失败(原版本节点.状态);
        const auto 原归属 = 读取集合关系组(L1所有者范围关系端点方向::源,
            版本编码, 配置.集合族归属关系类型, 读取代次, 截止, true);
        if (原归属.状态 != 特征类事实集合状态::已读取)
            return 版本失败(原归属.状态);
        if (原归属.关系组.size() != 1 || !集合关系有效(原归属.关系组.front(),
                版本编码, 配置.集合锚点, 配置.集合族归属关系类型, 2, 截止))
            return 版本失败(特征类事实集合状态::内部不一致);

        std::vector<稳定编码> 已见;
        稳定编码 链尾 = 版本编码;
        std::uint64_t 成员截止 = 截止;
        while (true) {
            if (std::find(已见.begin(), 已见.end(), 链尾) != 已见.end())
                return 版本失败(特征类事实集合状态::内部不一致);
            已见.push_back(链尾);
            const auto 后继 = 读取集合关系组(L1所有者范围关系端点方向::目标,
                链尾, 配置.前版本关系类型, 读取代次, 截止, true);
            if (后继.状态 != 特征类事实集合状态::已读取) return 版本失败(后继.状态);
            if (后继.关系组.size() > 1) return 版本失败(特征类事实集合状态::内部不一致);
            if (后继.关系组.empty()) break;
            const auto& 关系 = 后继.关系组.front();
            if (!集合关系有效(关系, 关系.源节点, 链尾,
                    配置.前版本关系类型, 1, 截止))
                return 版本失败(特征类事实集合状态::内部不一致);
            if (链尾 == 版本编码) {
                if (关系.创建事实代次 <= 1) return 版本失败(特征类事实集合状态::内部不一致);
                成员截止 = 关系.创建事实代次 - 1;
            }
            链尾 = 关系.源节点;
        }
        const auto 链尾节点 = 读取集合节点(链尾, 读取代次, 截止, true);
        if (!链尾节点.节点) return 版本失败(链尾节点.状态);
        const auto 集合识别截止 = 链尾节点.节点->创建事实代次;
        const auto 当前 = 读取集合关系组(L1所有者范围关系端点方向::目标,
            链尾, 配置.当前版本关系类型, 读取代次, 集合识别截止, true);
        if (当前.状态 != 特征类事实集合状态::已读取) return 版本失败(当前.状态);
        if (当前.关系组.size() != 1) return 版本失败(特征类事实集合状态::内部不一致);
        const auto& 当前关系 = 当前.关系组.front();
        if (!集合关系有效(当前关系, 当前关系.源节点, 链尾,
                配置.当前版本关系类型, 1, 集合识别截止))
            return 版本失败(特征类事实集合状态::内部不一致);
        if (版本编码 == 链尾 && 当前关系.退出事实代次
            && *当前关系.退出事实代次 <= 截止) {
            if (*当前关系.退出事实代次 <= 1)
                return 版本失败(特征类事实集合状态::内部不一致);
            成员截止 = *当前关系.退出事实代次 - 1;
        }
        const auto 集合验证 = 读取集合截止(当前关系.源节点, 读取代次,
            集合识别截止, true);
        if (!集合验证.集合) return 版本失败(集合验证.状态);

        特征类事实集合版本事实 版本;
        版本.版本身份 = {版本编码};
        版本.集合身份 = {当前关系.源节点};
        版本.创建事实代次 = 原版本节点.节点->创建事实代次;
        const auto 前版 = 读取集合关系组(L1所有者范围关系端点方向::源,
            版本编码, 配置.前版本关系类型, 读取代次, 成员截止, true);
        if (前版.状态 != 特征类事实集合状态::已读取 || 前版.关系组.size() > 1)
            return 版本失败(前版.状态 == 特征类事实集合状态::已读取
                ? 特征类事实集合状态::内部不一致 : 前版.状态);
        if (!前版.关系组.empty()) {
            const auto& 关系 = 前版.关系组.front();
            if (!集合关系有效(关系, 版本编码, 关系.目标节点,
                    配置.前版本关系类型, 1, 成员截止))
                return 版本失败(特征类事实集合状态::内部不一致);
            版本.前版本身份 = 特征类事实集合版本身份{关系.目标节点};
            版本.前版本关系 = 关系.编码;
        }
        const auto 成员 = 读取集合关系组(L1所有者范围关系端点方向::源,
            版本编码, 配置.成员关系类型, 读取代次, 成员截止, true);
        if (成员.状态 != 特征类事实集合状态::已读取 || 成员.关系组.empty()
            || 成员.关系组.size() > 配置.最大成员数)
            return 版本失败(成员.关系组.size() > 配置.最大成员数
                ? 特征类事实集合状态::数量超限
                : 成员.状态 == 特征类事实集合状态::已读取
                    ? 特征类事实集合状态::内部不一致 : 成员.状态);
        for (const auto& 关系 : 成员.关系组) {
            std::uint32_t 顺序 = 0, 角色 = 0;
            if (!解码成员角色顺序(关系.角色或顺序, 顺序, 角色)
                || !集合关系有效(关系, 版本编码, 关系.目标节点,
                    配置.成员关系类型, 关系.角色或顺序, 成员截止))
                return 版本失败(特征类事实集合状态::内部不一致);
            特征类事实集合状态 失败 = 特征类事实集合状态::内部不一致;
            auto 特征 = 读取截止普通特征(关系.目标节点, 读取代次, 成员截止, true, 失败);
            if (!特征) return 版本失败(失败);
            版本.成员.push_back({关系.编码, 顺序, 角色, std::move(*特征),
                关系.创建事实代次, std::nullopt});
        }
        std::sort(版本.成员.begin(), 版本.成员.end(), [](const auto& 左, const auto& 右) {
            return 左.顺序 != 右.顺序 ? 左.顺序 < 右.顺序 : 左.关系 < 右.关系;
        });
        std::vector<稳定编码> 身份;
        for (std::size_t i = 0; i < 版本.成员.size(); ++i) {
            if (版本.成员[i].顺序 != i + 1) return 版本失败(特征类事实集合状态::内部不一致);
            身份.push_back(版本.成员[i].特征.结点);
        }
        std::sort(身份.begin(), 身份.end());
        if (std::adjacent_find(身份.begin(), 身份.end()) != 身份.end())
            return 版本失败(特征类事实集合状态::内部不一致);
        return {特征类事实集合状态::已读取, 截止, std::move(版本)};
    }

    特征类事实集合组结果 反查当前事实集合(
        const 特征类事实集合成员反查请求& 请求) const {
        特征类事实集合状态 失败 = 特征类事实集合状态::内部不一致;
        if (!读取截止普通特征(请求.成员特征, 请求.期望事实代次,
                请求.期望事实代次, false, 失败)) return 集合组失败(失败);
        const auto 关系组 = 读取集合关系组(L1所有者范围关系端点方向::目标,
            请求.成员特征, 事实集合结构类型_->成员关系类型,
            请求.期望事实代次, 请求.期望事实代次, false);
        if (关系组.状态 != 特征类事实集合状态::已读取) return 集合组失败(关系组.状态);
        if (关系组.关系组.size() > 请求.数量预算)
            return 集合组失败(特征类事实集合状态::数量超限);
        std::vector<特征类事实集合事实> 集合组;
        std::vector<稳定编码> 已见;
        for (const auto& 成员关系 : 关系组.关系组) {
            std::uint32_t 顺序 = 0, 角色 = 0;
            if (!解码成员角色顺序(成员关系.角色或顺序, 顺序, 角色)
                || !集合关系有效(成员关系, 成员关系.源节点, 请求.成员特征,
                    事实集合结构类型_->成员关系类型, 成员关系.角色或顺序,
                    请求.期望事实代次)) return 集合组失败(特征类事实集合状态::内部不一致);
            const auto 当前 = 读取集合关系组(L1所有者范围关系端点方向::目标,
                成员关系.源节点, 事实集合结构类型_->当前版本关系类型,
                请求.期望事实代次, 请求.期望事实代次, false);
            if (当前.状态 != 特征类事实集合状态::已读取 || 当前.关系组.size() != 1)
                return 集合组失败(特征类事实集合状态::内部不一致);
            const auto 集合编码 = 当前.关系组.front().源节点;
            if (std::find(已见.begin(), 已见.end(), 集合编码) != 已见.end()) continue;
            auto 集合 = 读取集合截止(集合编码, 请求.期望事实代次,
                请求.期望事实代次, false);
            if (!集合.集合) return 集合组失败(集合.状态);
            已见.push_back(集合编码);
            集合组.push_back(std::move(*集合.集合));
            if (集合组.size() > 请求.数量预算)
                return 集合组失败(特征类事实集合状态::数量超限);
        }
        std::sort(集合组.begin(), 集合组.end(), [](const auto& 左, const auto& 右) {
            return 左.集合身份.值 < 右.集合身份.值;
        });
        return {特征类事实集合状态::已读取, 请求.期望事实代次, std::move(集合组)};
    }

    std::optional<特征类事实集合状态> 验证集合成员提交组(
        const std::vector<特征类事实集合成员提交项>& 成员,
        std::uint64_t 事实代次) const {
        for (const auto& 项 : 成员) {
            const auto 读取 = 读取当前特征(项.特征, 事实代次);
            if (读取.成功() && 读取.状态 == 特征类数据状态::已读取
                && 读取.特征 && 读取.特征->结点 == 项.特征) continue;
            if (读取.状态 == 特征类数据状态::未找到)
                return 特征类事实集合状态::成员未找到;
            if (读取.状态 == 特征类数据状态::目标已退出)
                return 特征类事实集合状态::成员已退出;
            if (读取.状态 == 特征类数据状态::事实代次漂移)
                return 特征类事实集合状态::事实代次漂移;
            if (读取.状态 == 特征类数据状态::资源失败)
                return 特征类事实集合状态::资源失败;
            return 特征类事实集合状态::内部不一致;
        }
        return std::nullopt;
    }

    L1所有者范围写集请求 形成新增集合写集(
        const 特征类事实集合新增请求& 请求) const {
        const auto& 配置 = *事实集合结构类型_;
        L1所有者范围写集请求 写集;
        写集.合同版本 = L1所有者范围CRUD合同版本;
        写集.期望事实代次 = 请求.期望事实代次;
        写集.写入幂等身份 = 请求.幂等身份;
        写集.节点 = {{特征类数据内部::集合身份本地键, 节点种类::普通, std::nullopt},
            {特征类数据内部::集合版本本地键, 节点种类::普通, std::nullopt}};
        写集.关系 = {
            {特征类数据内部::集合身份归属本地键, 特征类数据内部::集合身份本地键,
                配置.集合锚点, 配置.集合族归属关系类型, 1},
            {特征类数据内部::集合版本归属本地键, 特征类数据内部::集合版本本地键,
                配置.集合锚点, 配置.集合族归属关系类型, 2},
            {特征类数据内部::集合当前版本本地键, 特征类数据内部::集合身份本地键,
                特征类数据内部::集合版本本地键, 配置.当前版本关系类型, 1}};
        for (std::size_t i = 0; i < 请求.成员.size(); ++i)
            写集.关系.push_back({{特征类数据内部::集合成员本地键起点
                    + static_cast<std::uint32_t>(i)},
                特征类数据内部::集合版本本地键, 请求.成员[i].特征,
                配置.成员关系类型, 编码成员角色顺序(请求.成员[i].顺序,
                    请求.成员[i].成员角色)});
        return 写集;
    }

    L1所有者范围写集请求 形成换代集合写集(
        const 特征类事实集合换代请求& 请求,
        const 特征类事实集合事实& 当前) const {
        const auto& 配置 = *事实集合结构类型_;
        L1所有者范围写集请求 写集;
        写集.合同版本 = L1所有者范围CRUD合同版本;
        写集.期望事实代次 = 请求.期望事实代次;
        写集.写入幂等身份 = 请求.幂等身份;
        写集.节点 = {{特征类数据内部::集合版本本地键, 节点种类::普通, std::nullopt}};
        写集.关系 = {
            {特征类数据内部::集合版本归属本地键, 特征类数据内部::集合版本本地键,
                配置.集合锚点, 配置.集合族归属关系类型, 2},
            {特征类数据内部::集合前版本本地键, 特征类数据内部::集合版本本地键,
                当前.版本事实.版本身份.值, 配置.前版本关系类型, 1},
            {特征类数据内部::集合当前版本本地键, 请求.集合身份.值,
                特征类数据内部::集合版本本地键, 配置.当前版本关系类型, 1}};
        for (std::size_t i = 0; i < 请求.新成员.size(); ++i)
            写集.关系.push_back({{特征类数据内部::集合成员本地键起点
                    + static_cast<std::uint32_t>(i)},
                特征类数据内部::集合版本本地键, 请求.新成员[i].特征,
                配置.成员关系类型, 编码成员角色顺序(请求.新成员[i].顺序,
                    请求.新成员[i].成员角色)});
        写集.退出事实.push_back(当前.当前版本关系);
        for (const auto& 项 : 当前.版本事实.成员) 写集.退出事实.push_back(项.关系);
        return 写集;
    }

    L1所有者范围写集请求 形成退出集合写集(
        const 特征类事实集合退出请求& 请求,
        const 特征类事实集合事实& 当前, 稳定编码 集合族归属关系) const {
        L1所有者范围写集请求 写集;
        写集.合同版本 = L1所有者范围CRUD合同版本;
        写集.期望事实代次 = 请求.期望事实代次;
        写集.写入幂等身份 = 请求.幂等身份;
        写集.退出事实 = {请求.集合身份.值, 集合族归属关系, 当前.当前版本关系};
        for (const auto& 项 : 当前.版本事实.成员) 写集.退出事实.push_back(项.关系);
        return 写集;
    }

    std::optional<稳定编码> 读取当前集合族归属(
        稳定编码 集合, std::uint64_t 事实代次) const {
        const auto 读取 = 读取集合关系组(L1所有者范围关系端点方向::源,
            集合, 事实集合结构类型_->集合族归属关系类型,
            事实代次, 事实代次, false);
        if (读取.状态 != 特征类事实集合状态::已读取 || 读取.关系组.size() != 1
            || !集合关系有效(读取.关系组.front(), 集合,
                事实集合结构类型_->集合锚点,
                事实集合结构类型_->集合族归属关系类型, 1, 事实代次)) return std::nullopt;
        return 读取.关系组.front().编码;
    }

    特征类事实集合结果 提交集合写集(const L1所有者范围写集请求& 写集,
        L1所有者范围写入幂等身份 幂等身份, 特征类事实集合状态 成功状态,
        std::optional<稳定编码> 已知集合 = std::nullopt) {
        const auto 写入 = 写入端口_.提交所有者范围中性写集(写集);
        const auto 状态 = 映射集合写入状态(写入.状态, 成功状态);
        if (状态 != 成功状态 && 状态 != 特征类事实集合状态::精确重复)
            return 集合失败(状态);
        if (!写入结果头完整(写入, 幂等身份))
            return 集合失败(特征类事实集合状态::已可能发布, 0, 写入.事实代次);
        const auto 首次 = 写入端口_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本, 幂等身份});
        if (首次.状态 != L1所有者范围读取状态::成功
            || 首次.合同版本 != L1所有者范围首次写入读取合同版本
            || 首次.所有者 != 所有者_ || 首次.写入幂等身份 != 幂等身份
            || !首次.首次规范化写集 || !首次.首次写入结果)
            return 集合失败(特征类事实集合状态::已可能发布, 0, 写入.事实代次);
        auto 集合 = 已知集合;
        if (!集合) 集合 = 特征类数据内部::查找唯一编码(
            写入, 特征类数据内部::集合身份本地键);
        if (!集合) return 集合失败(特征类事实集合状态::已可能发布, 0, 写入.事实代次);

        if (成功状态 == 特征类事实集合状态::已退出) {
            if (写入.新编码映射.size() != 0 || 写入.事实代次 <= 1)
                return 集合失败(特征类事实集合状态::已可能发布, 0, 写入.事实代次);
            auto 读回 = 读取集合截止(*集合, 首次.读取事实代次,
                写入.事实代次 - 1, true);
            if (!读回.集合) return 集合失败(特征类事实集合状态::已可能发布, 0, 写入.事实代次);
            for (const auto 编码 : 写集.退出事实) {
                const auto 事实 = 第一层服务_.读取所有者范围历史事实(
                    {L1所有者范围CRUD合同版本, 编码});
                if (事实.状态 != L1所有者范围读取状态::成功
                    || 事实.读取事实代次 != 首次.读取事实代次 || !事实.事实)
                    return 集合失败(特征类事实集合状态::已可能发布, 0, 写入.事实代次);
                const auto 退出 = std::visit([](const auto& 项) { return 项.退出事实代次; }, *事实.事实);
                if (退出 != 写入.事实代次)
                    return 集合失败(特征类事实集合状态::已可能发布, 0, 写入.事实代次);
            }
            读回.集合->退出事实代次 = 写入.事实代次;
            for (auto& 项 : 读回.集合->版本事实.成员) 项.退出事实代次 = 写入.事实代次;
            读回.状态 = 状态;
            读回.事实截止代次 = 写入.事实代次;
            读回.首次提交事实代次 = 写入.事实代次;
            return 读回;
        }

        const std::size_t 期望映射 = 成功状态 == 特征类事实集合状态::已创建
            ? 写集.节点.size() + 写集.关系.size()
            : 写集.节点.size() + 写集.关系.size();
        if (写入.新编码映射.size() != 期望映射)
            return 集合失败(特征类事实集合状态::已可能发布, 0, 写入.事实代次);
        auto 读回 = 读取集合截止(*集合, 首次.读取事实代次, 写入.事实代次, true);
        if (!读回.集合) return 集合失败(特征类事实集合状态::已可能发布, 0, 写入.事实代次);
        const auto 新版本 = 特征类数据内部::查找唯一编码(
            写入, 特征类数据内部::集合版本本地键);
        if (!新版本 || 读回.集合->版本事实.版本身份.值 != *新版本)
            return 集合失败(特征类事实集合状态::已可能发布, 0, 写入.事实代次);
        读回.状态 = 状态;
        读回.首次提交事实代次 = 写入.事实代次;
        return 读回;
    }

    std::optional<特征类事实集合结果> 尝试重放新增集合(
        const 特征类事实集合新增请求& 请求) {
        const auto 首次 = 写入端口_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本, 请求.幂等身份});
        if (首次.状态 == L1所有者范围读取状态::未找到) return std::nullopt;
        if (首次.状态 != L1所有者范围读取状态::成功)
            return 集合失败(映射集合读取状态(首次.状态));
        if (!首次.首次规范化写集
            || !首次.首次写入结果 || 首次.所有者 != 所有者_
            || *首次.首次规范化写集 != 形成新增集合写集(请求)
            || 首次.首次写入结果->状态 != L1所有者范围写入状态::成功)
            return 集合失败(特征类事实集合状态::幂等冲突);
        return 提交集合写集(*首次.首次规范化写集, 请求.幂等身份,
            特征类事实集合状态::已创建);
    }

    std::optional<特征类事实集合结果> 尝试重放换代集合(
        const 特征类事实集合换代请求& 请求) {
        const auto 首次 = 写入端口_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本, 请求.幂等身份});
        if (首次.状态 == L1所有者范围读取状态::未找到) return std::nullopt;
        if (首次.状态 != L1所有者范围读取状态::成功)
            return 集合失败(映射集合读取状态(首次.状态));
        if (!首次.首次规范化写集
            || !首次.首次写入结果 || 首次.所有者 != 所有者_)
            return 集合失败(特征类事实集合状态::内部不一致);
        const auto 旧 = 读取集合截止(请求.集合身份.值, 首次.读取事实代次,
            请求.期望事实代次, true);
        if (!旧.集合 || 旧.集合->版本事实.版本身份 != 请求.预期当前版本
            || *首次.首次规范化写集 != 形成换代集合写集(请求, *旧.集合)
            || 首次.首次写入结果->状态 != L1所有者范围写入状态::成功)
            return 集合失败(特征类事实集合状态::幂等冲突);
        return 提交集合写集(*首次.首次规范化写集, 请求.幂等身份,
            特征类事实集合状态::已换代, 请求.集合身份.值);
    }

    std::optional<特征类事实集合结果> 尝试重放退出集合(
        const 特征类事实集合退出请求& 请求) {
        const auto 首次 = 写入端口_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本, 请求.幂等身份});
        if (首次.状态 == L1所有者范围读取状态::未找到) return std::nullopt;
        if (首次.状态 != L1所有者范围读取状态::成功)
            return 集合失败(映射集合读取状态(首次.状态));
        if (!首次.首次规范化写集
            || !首次.首次写入结果 || 首次.所有者 != 所有者_)
            return 集合失败(特征类事实集合状态::内部不一致);
        const auto 旧 = 读取集合截止(请求.集合身份.值, 首次.读取事实代次,
            请求.期望事实代次, true);
        const auto 归属 = 读取集合关系组(L1所有者范围关系端点方向::源,
            请求.集合身份.值, 事实集合结构类型_->集合族归属关系类型,
            首次.读取事实代次, 请求.期望事实代次, true);
        if (!旧.集合 || 旧.集合->版本事实.版本身份 != 请求.预期当前版本
            || 归属.关系组.size() != 1
            || *首次.首次规范化写集 != 形成退出集合写集(请求, *旧.集合,
                归属.关系组.front().编码)
            || 首次.首次写入结果->状态 != L1所有者范围写入状态::成功)
            return 集合失败(特征类事实集合状态::幂等冲突);
        return 提交集合写集(*首次.首次规范化写集, 请求.幂等身份,
            特征类事实集合状态::已退出, 请求.集合身份.值);
    }

    特征类事实集合结果 执行新增事实集合(const 特征类事实集合新增请求& 请求) {
        if (const auto 重放 = 尝试重放新增集合(请求)) return *重放;
        if (const auto 失败 = 验证集合成员提交组(请求.成员, 请求.期望事实代次))
            return 集合失败(*失败);
        return 提交集合写集(形成新增集合写集(请求), 请求.幂等身份,
            特征类事实集合状态::已创建);
    }

    特征类事实集合结果 执行换代事实集合(const 特征类事实集合换代请求& 请求) {
        if (const auto 重放 = 尝试重放换代集合(请求)) return *重放;
        auto 当前 = 读取集合截止(请求.集合身份.值, 请求.期望事实代次,
            请求.期望事实代次, false);
        if (!当前.集合) return 当前;
        if (当前.集合->版本事实.版本身份 != 请求.预期当前版本)
            return 集合失败(特征类事实集合状态::当前版本漂移);
        if (const auto 失败 = 验证集合成员提交组(请求.新成员, 请求.期望事实代次))
            return 集合失败(*失败);
        return 提交集合写集(形成换代集合写集(请求, *当前.集合),
            请求.幂等身份, 特征类事实集合状态::已换代, 请求.集合身份.值);
    }

    特征类事实集合结果 执行退出事实集合(const 特征类事实集合退出请求& 请求) {
        if (const auto 重放 = 尝试重放退出集合(请求)) return *重放;
        auto 当前 = 读取集合截止(请求.集合身份.值, 请求.期望事实代次,
            请求.期望事实代次, false);
        if (!当前.集合) return 当前;
        if (当前.集合->版本事实.版本身份 != 请求.预期当前版本)
            return 集合失败(特征类事实集合状态::当前版本漂移);
        const auto 归属 = 读取当前集合族归属(请求.集合身份.值, 请求.期望事实代次);
        if (!归属) return 集合失败(特征类事实集合状态::内部不一致);
        return 提交集合写集(形成退出集合写集(请求, *当前.集合, *归属),
            请求.幂等身份, 特征类事实集合状态::已退出, 请求.集合身份.值);
    }






    struct 统一定义读取结果 final {
        bool 成功 = false;
        特征类派生数据状态 状态 = 特征类派生数据状态::内部不一致;
        std::uint64_t 事实代次 = 0;
        std::uint32_t 实际阶次 = 0;
        std::optional<特征类派生定义事实> 派生;
    };

    struct 关系组结果 final {
        bool 成功 = false;
        特征类派生数据状态 状态 = 特征类派生数据状态::内部不一致;
        std::uint64_t 事实代次 = 0;
        std::vector<L1所有者范围关系事实> 关系组;
    };

    bool 类型结点有效(稳定编码 编码, 节点种类 种类,
        std::optional<L1所有者范围值表示种类> 表示) const {
        const auto 读取 = 第一层服务_.读取所有者范围当前节点(
            {L1所有者范围CRUD合同版本, 编码});
        const auto* 事实 = 读取.事实
            ? std::get_if<L1所有者范围节点事实>(&*读取.事实) : nullptr;
        return 读取.状态 == L1所有者范围读取状态::成功
            && 读取.合同版本 == L1所有者范围CRUD合同版本
            && 读取.查询编码 == 编码 && !读取.物理清理事实代次
            && !读取.物理清理墓碑 && 事实 && 事实->编码 == 编码
            && 事实->写入所有者 == 所有者_ && 事实->种类 == 种类
            && 事实->属性类型表示 == 表示 && 事实->创建事实代次 != 0
            && !事实->退出事实代次 && 事实->当前属性.empty();
    }

    static bool 派生新增请求有效(
        const 特征类派生定义新增请求& 请求) noexcept {
        if (请求.合同版本 != 特征类派生数据合同版本
            || 请求.期望事实代次 == 0 || !有效(请求.幂等身份)
            || 请求.直接来源.size() != 2
            || 请求.直接来源[0].顺序 != 1
            || 请求.直接来源[1].顺序 != 2
            || 请求.直接来源[0].输入角色 == 0
            || 请求.直接来源[1].输入角色 == 0
            || 请求.直接来源[0].输入角色 == 请求.直接来源[1].输入角色
            || !有效(请求.直接来源[0].来源特征定义.值)
            || !有效(请求.直接来源[1].来源特征定义.值)
            || 请求.直接来源[0].来源特征定义
                == 请求.直接来源[1].来源特征定义
            || !有效(请求.派生规则.规则身份)
            || 请求.派生规则.规则版本 == 0
            || (请求.宿主存在 && !有效(*请求.宿主存在)))
            return false;
        return 比较合同有效(请求.比较合同, 请求.派生规则);
    }

    static bool 派生查询请求有效(
        const 特征类派生定义查询请求& 请求) noexcept {
        return 请求.合同版本 == 特征类派生数据合同版本
            && 请求.期望事实代次 != 0 && 有效(请求.特征定义.值);
    }

    static bool 派生退出请求有效(
        const 特征类派生定义退出请求& 请求) noexcept {
        return 请求.合同版本 == 特征类派生数据合同版本
            && 请求.期望事实代次 != 0 && 有效(请求.幂等身份)
            && 有效(请求.特征定义.值);
    }

    static bool 比较合同有效(const 特征类I64比较注册合同& 合同,
        const 特征类派生规则& 规则) noexcept {
        const bool 角色匹配 = 合同.用途 == 特征类比较用途::目标判断
            ? 合同.左角色 == 特征类比较角色::当前事实
                && 合同.右角色 == 特征类比较角色::目标状态
            : 合同.用途 == 特征类比较用途::状态迁移
                && 合同.左角色 == 特征类比较角色::前状态
                && 合同.右角色 == 特征类比较角色::后当前事实;
        return 角色匹配 && 合同.算法版本 != 0
            && 合同.算法版本 == 规则.规则版本
            && 有效(合同.单位) && 有效(合同.维度)
            && 有效(合同.分量角色) && 合同.允许结果位 >= 1
            && 合同.允许结果位 <= 7 && 合同.误差合同版本 != 0
            && (!合同.误差预算 || *合同.误差预算 >= 0)
            && (!合同.相等容差 || *合同.相等容差 >= 0);
    }

    static bool 比较输入当前(const 特征类比较输入& 输入,
        std::uint64_t 截止) noexcept {
        return 有效(输入.事实身份) && 有效(输入.来源稳定编码)
            && 输入.输入版本 != 0 && 输入.输入版本 <= 截止
            && 输入.创建事实代次 != 0
            && 输入.创建事实代次 <= 输入.输入版本
            && (!输入.退出事实代次 || *输入.退出事实代次 > 截止)
            && 有效(输入.单位) && 有效(输入.维度)
            && 有效(输入.分量角色);
    }

    bool 验证比较输入事实(const 特征类比较输入& 输入,
        std::uint64_t 截止) const {
        const auto 读取 = 第一层服务_.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 输入.事实身份});
        const auto* 事实 = 读取.事实
            ? std::get_if<L1所有者范围值事实>(&*读取.事实) : nullptr;
        const auto* 值 = 事实
            ? std::get_if<std::int64_t>(&事实->材料) : nullptr;
        return 读取.状态 == L1所有者范围读取状态::成功
            && 读取.合同版本 == L1所有者范围CRUD合同版本
            && 读取.查询编码 == 输入.事实身份
            && 读取.读取事实代次 == 截止 && !读取.物理清理事实代次
            && !读取.物理清理墓碑 && 事实 && 值 && *值 == 输入.值
            && 事实->编码 == 输入.事实身份
            && 事实->创建事实代次 == 输入.创建事实代次
            && 事实->创建事实代次 <= 输入.输入版本
            && (!事实->退出事实代次 || *事实->退出事实代次 > 截止);
    }

    static bool 比较请求有效(const 特征类比较请求& 请求) noexcept {
        return 请求.合同版本 == 特征类派生数据合同版本
            && 请求.期望事实代次 != 0 && 请求.请求身份 != 0
            && 有效(请求.特征定义.值)
            && (请求.用途 == 特征类比较用途::目标判断
                || 请求.用途 == 特征类比较用途::状态迁移)
            && 请求.要求结果位 >= 1 && 请求.要求结果位 <= 7
            && 比较输入当前(请求.左, 请求.期望事实代次)
            && 比较输入当前(请求.右, 请求.期望事实代次);
    }

    static std::uint64_t I64差异幅度(
        std::int64_t 左, std::int64_t 右) noexcept {
        if (左 == 右) return 0;
        if (左 < 0 && 右 >= 0)
            return static_cast<std::uint64_t>(-(左 + 1)) + 1U
                + static_cast<std::uint64_t>(右);
        if (右 < 0 && 左 >= 0)
            return static_cast<std::uint64_t>(-(右 + 1)) + 1U
                + static_cast<std::uint64_t>(左);
        return 左 < 右 ? static_cast<std::uint64_t>(右 - 左)
                       : static_cast<std::uint64_t>(左 - 右);
    }

    static bool I64差异可表示(std::int64_t 左, std::int64_t 右) noexcept {
        if (左 > 0 && 右 < std::numeric_limits<std::int64_t>::min() + 左)
            return false;
        if (左 < 0 && 右 > std::numeric_limits<std::int64_t>::max() + 左)
            return false;
        return true;
    }

    static 特征类比较结果 比较失败(std::uint64_t 请求身份,
        特征类比较状态 状态, 特征类比较拒绝原因 原因,
        std::uint64_t 事实代次 = 0) noexcept {
        特征类比较结果 结果;
        结果.请求身份 = 请求身份;
        结果.状态 = 状态;
        结果.拒绝原因 = 原因;
        结果.事实代次 = 事实代次;
        return 结果;
    }

    static void 填充比较回显(特征类比较结果& 结果,
        const 特征类比较请求& 请求,
        const 特征类I64比较注册合同& 合同) noexcept {
        结果.误差预算 = 合同.误差预算;
        结果.相等容差 = 合同.相等容差;
        结果.误差合同版本 = 合同.误差合同版本;
        结果.单位 = 合同.单位;
        结果.维度 = 合同.维度;
        结果.分量角色 = 合同.分量角色;
        结果.实际左角色 = 请求.左.角色;
        结果.实际右角色 = 请求.右.角色;
        结果.左事实身份 = 请求.左.事实身份;
        结果.左来源稳定编码 = 请求.左.来源稳定编码;
        结果.左输入版本 = 请求.左.输入版本;
        结果.左创建事实代次 = 请求.左.创建事实代次;
        结果.右事实身份 = 请求.右.事实身份;
        结果.右来源稳定编码 = 请求.右.来源稳定编码;
        结果.右输入版本 = 请求.右.输入版本;
        结果.右创建事实代次 = 请求.右.创建事实代次;
    }

    static 特征类派生定义结果 派生失败(
        特征类派生数据状态 状态, std::uint64_t 事实代次 = 0) noexcept {
        return {状态, 特征类派生数据合同版本, 事实代次, std::nullopt};
    }

    static 特征类派生数据状态 映射派生读取状态(
        L1所有者范围读取状态 状态) noexcept {
        switch (状态) {
        case L1所有者范围读取状态::未找到:
            return 特征类派生数据状态::未找到;
        case L1所有者范围读取状态::已退出:
            return 特征类派生数据状态::目标已退出;
        case L1所有者范围读取状态::事实代次漂移:
            return 特征类派生数据状态::事实代次漂移;
        case L1所有者范围读取状态::入口拒绝:
            return 特征类派生数据状态::入口拒绝;
        case L1所有者范围读取状态::资源失败:
            return 特征类派生数据状态::资源失败;
        default:
            return 特征类派生数据状态::内部不一致;
        }
    }

    static 特征类派生数据状态 映射派生写入状态(
        L1所有者范围写入状态 状态,
        特征类派生数据状态 成功状态) noexcept {
        switch (状态) {
        case L1所有者范围写入状态::成功: return 成功状态;
        case L1所有者范围写入状态::精确重复:
            return 特征类派生数据状态::精确重复;
        case L1所有者范围写入状态::入口拒绝:
            return 特征类派生数据状态::入口拒绝;
        case L1所有者范围写入状态::未找到:
        case L1所有者范围写入状态::已退出:
            return 特征类派生数据状态::内部不一致;
        case L1所有者范围写入状态::事实代次漂移:
            return 特征类派生数据状态::事实代次漂移;
        case L1所有者范围写入状态::幂等冲突:
            return 特征类派生数据状态::幂等冲突;
        case L1所有者范围写入状态::引用冲突:
            return 特征类派生数据状态::引用冲突;
        case L1所有者范围写入状态::资源失败:
            return 特征类派生数据状态::资源失败;
        default:
            return 特征类派生数据状态::内部不一致;
        }
    }

    static 特征类派生数据状态 映射来源状态(
        特征类派生数据状态 状态) noexcept {
        if (状态 == 特征类派生数据状态::未找到)
            return 特征类派生数据状态::来源未找到;
        if (状态 == 特征类派生数据状态::目标已退出)
            return 特征类派生数据状态::来源已退出;
        return 状态;
    }

    关系组结果 读取当前源关系组(稳定编码 源, 稳定编码 类型,
        std::uint64_t 截止) const {
        const auto 读取 = 第一层服务_.读取所有者范围当前源关系组(
            {L1所有者范围CRUD合同版本, 源, 类型});
        if (读取.状态 != L1所有者范围读取状态::成功)
            return {false, 映射派生读取状态(读取.状态),
                读取.读取事实代次, {}};
        if (读取.合同版本 != L1所有者范围CRUD合同版本
            || 读取.源节点 != 源 || 读取.关系类型节点 != 类型
            || 读取.读取事实代次 != 截止)
            return {false, 读取.读取事实代次 != 截止
                    ? 特征类派生数据状态::事实代次漂移
                    : 特征类派生数据状态::内部不一致,
                读取.读取事实代次, {}};
        return {true, 特征类派生数据状态::已读取,
            读取.读取事实代次, 读取.关系组};
    }

    const L1所有者范围属性槽* 查找属性槽(
        const L1所有者范围节点事实& 节点, 稳定编码 类型) const noexcept {
        const L1所有者范围属性槽* 找到 = nullptr;
        for (const auto& 槽 : 节点.当前属性) {
            if (槽.属性类型节点 != 类型) continue;
            if (找到) return nullptr;
            找到 = &槽;
        }
        return 找到;
    }

    std::optional<L1所有者范围值事实> 读取当前值事实(
        const L1所有者范围属性槽& 槽, 稳定编码 所属节点,
        std::uint64_t 截止) const {
        const auto 读取 = 第一层服务_.读取所有者范围当前值(
            {L1所有者范围CRUD合同版本, 槽.当前值});
        const auto* 值 = 读取.事实
            ? std::get_if<L1所有者范围值事实>(&*读取.事实) : nullptr;
        if (读取.状态 != L1所有者范围读取状态::成功
            || 读取.合同版本 != L1所有者范围CRUD合同版本
            || 读取.查询编码 != 槽.当前值 || 读取.读取事实代次 != 截止
            || 读取.物理清理事实代次 || 读取.物理清理墓碑 || !值
            || 值->编码 != 槽.当前值 || 值->所属节点 != 所属节点
            || 值->属性类型节点 != 槽.属性类型节点
            || 值->来源节点 != 所属节点 || 值->写入所有者 != 所有者_
            || 值->创建事实代次 == 0 || 值->创建事实代次 > 截止
            || 值->退出事实代次)
            return std::nullopt;
        return *值;
    }

    统一定义读取结果 读取统一定义(特征类定义身份 身份,
        std::uint64_t 截止, std::vector<稳定编码>& 路径) const {
        if (std::find(路径.begin(), 路径.end(), 身份.值) != 路径.end())
            return {false, 特征类派生数据状态::内部不一致, 截止, 0, {}};
        const auto 节点读取 = 第一层服务_.读取所有者范围当前节点(
            {L1所有者范围CRUD合同版本, 身份.值});
        if (节点读取.状态 != L1所有者范围读取状态::成功)
            return {false, 映射派生读取状态(节点读取.状态),
                节点读取.读取事实代次, 0, {}};
        const auto* 节点 = 节点读取.事实
            ? std::get_if<L1所有者范围节点事实>(&*节点读取.事实) : nullptr;
        if (节点读取.合同版本 != L1所有者范围CRUD合同版本
            || 节点读取.查询编码 != 身份.值
            || 节点读取.读取事实代次 != 截止)
            return {false, 节点读取.读取事实代次 != 截止
                    ? 特征类派生数据状态::事实代次漂移
                    : 特征类派生数据状态::内部不一致,
                节点读取.读取事实代次, 0, {}};
        if (!节点 || 节点->编码 != 身份.值
            || 节点->写入所有者 != 所有者_
            || 节点->种类 != 节点种类::普通 || 节点->属性类型表示
            || 节点->创建事实代次 == 0 || 节点->创建事实代次 > 截止
            || 节点->退出事实代次)
            return {false, 特征类派生数据状态::内部不一致,
                截止, 0, {}};

        const auto 来源组 = 读取当前源关系组(身份.值,
            派生结构类型_.直接来源关系类型, 截止);
        const auto 宿主组 = 读取当前源关系组(身份.值,
            派生结构类型_.宿主关系类型, 截止);
        const auto 注册组 = 读取当前源关系组(身份.值,
            派生结构类型_.比较注册归属关系类型, 截止);
        if (!来源组.成功 || !宿主组.成功 || !注册组.成功) {
            const auto& 失败项 = !来源组.成功 ? 来源组
                : !宿主组.成功 ? 宿主组 : 注册组;
            return {false, 失败项.状态, 失败项.事实代次, 0, {}};
        }
        const bool 有派生属性 = std::any_of(节点->当前属性.begin(),
            节点->当前属性.end(), [&](const auto& 槽) noexcept {
                return 槽.属性类型节点 == 派生结构类型_.实际阶次属性类型
                    || 槽.属性类型节点 == 派生结构类型_.派生规则属性类型;
            });
        const bool 有派生关系 = !来源组.关系组.empty()
            || !宿主组.关系组.empty() || !注册组.关系组.empty();
        if (!有派生属性 && !有派生关系) {
            const auto 基础 = 查询特征({特征类数据合同版本, 截止, 身份.值});
            if (基础.成功() && 基础.状态 == 特征类数据状态::已读取)
                return {true, 特征类派生数据状态::已读取,
                    基础.事实代次, 1, {}};
            return {false, 基础.状态 == 特征类数据状态::未找到
                    ? 特征类派生数据状态::未找到
                    : 基础.状态 == 特征类数据状态::目标已退出
                        ? 特征类派生数据状态::目标已退出
                        : 基础.状态 == 特征类数据状态::事实代次漂移
                            ? 特征类派生数据状态::事实代次漂移
                            : 基础.状态 == 特征类数据状态::资源失败
                                ? 特征类派生数据状态::资源失败
                                : 特征类派生数据状态::内部不一致,
                基础.事实代次, 0, {}};
        }
        路径.push_back(身份.值);
        auto 结果 = 读取当前派生闭包(*节点, 来源组.关系组,
            宿主组.关系组, 注册组.关系组, 截止, 路径);
        路径.pop_back();
        return 结果;
    }

    统一定义读取结果 读取当前派生闭包(
        const L1所有者范围节点事实& 节点,
        std::vector<L1所有者范围关系事实> 来源关系,
        std::vector<L1所有者范围关系事实> 宿主关系,
        std::vector<L1所有者范围关系事实> 注册关系,
        std::uint64_t 截止, std::vector<稳定编码>& 路径) const {
        if (节点.当前属性.size() != 2 || 来源关系.size() != 2
            || 宿主关系.size() > 1 || 注册关系.size() != 1)
            return {false, 特征类派生数据状态::内部不一致, 截止, 0, {}};
        const auto* 阶次槽 = 查找属性槽(
            节点, 派生结构类型_.实际阶次属性类型);
        const auto* 规则槽 = 查找属性槽(
            节点, 派生结构类型_.派生规则属性类型);
        if (!阶次槽 || !规则槽)
            return {false, 特征类派生数据状态::内部不一致, 截止, 0, {}};
        const auto 阶次值 = 读取当前值事实(*阶次槽, 节点.编码, 截止);
        const auto 规则值 = 读取当前值事实(*规则槽, 节点.编码, 截止);
        if (!阶次值 || !规则值
            || 阶次值->创建事实代次 != 节点.创建事实代次
            || 规则值->创建事实代次 != 节点.创建事实代次)
            return {false, 特征类派生数据状态::内部不一致, 截止, 0, {}};
        const auto* 阶次 = std::get_if<std::int64_t>(&阶次值->材料);
        const auto* 规则 = std::get_if<std::vector<std::uint64_t>>(&规则值->材料);
        if (!阶次 || *阶次 <= 1
            || static_cast<std::uint64_t>(*阶次)
                > std::numeric_limits<std::uint32_t>::max()
            || !规则 || 规则->size() != 2 || (*规则)[0] == 0 || (*规则)[1] == 0
            || (*规则)[1] > std::numeric_limits<std::uint32_t>::max())
            return {false, 特征类派生数据状态::内部不一致, 截止, 0, {}};

        std::sort(来源关系.begin(), 来源关系.end(),
            [](const auto& 左, const auto& 右) noexcept {
                return 左.角色或顺序 < 右.角色或顺序;
            });
        特征类派生定义事实 定义;
        定义.身份 = {节点.编码};
        定义.实际阶次值 = 阶次值->编码;
        定义.实际阶次 = static_cast<std::uint32_t>(*阶次);
        定义.派生规则 = {稳定编码{(*规则)[0]},
            static_cast<std::uint32_t>((*规则)[1])};
        定义.创建事实代次 = 节点.创建事实代次;
        std::uint32_t 最大来源阶次 = 0;
        for (std::size_t i = 0; i < 来源关系.size(); ++i) {
            const auto& 关系 = 来源关系[i];
            const auto 角色编码 = static_cast<std::uint64_t>(关系.角色或顺序);
            const auto 顺序 = static_cast<std::uint32_t>(角色编码 >> 32U);
            const auto 输入角色 = static_cast<std::uint32_t>(角色编码);
            if (!有效(关系.编码) || 关系.源节点 != 节点.编码
                || !有效(关系.目标节点)
                || 关系.关系类型节点 != 派生结构类型_.直接来源关系类型
                || 关系.写入所有者 != 所有者_ || 关系.退出事实代次
                || 关系.创建事实代次 != 节点.创建事实代次
                || 顺序 != i + 1 || 输入角色 == 0
                || (i != 0 && 输入角色 == 定义.直接来源.front().输入角色))
                return {false, 特征类派生数据状态::内部不一致, 截止, 0, {}};
            const auto 来源 = 读取统一定义({关系.目标节点}, 截止, 路径);
            if (!来源.成功)
                return {false, 映射来源状态(来源.状态),
                    来源.事实代次, 0, {}};
            最大来源阶次 = std::max(最大来源阶次, 来源.实际阶次);
            定义.直接来源.push_back({关系.编码, 顺序, 输入角色,
                {关系.目标节点}});
        }
        if (定义.直接来源[0].来源特征定义
                == 定义.直接来源[1].来源特征定义
            || 最大来源阶次 == std::numeric_limits<std::uint32_t>::max()
            || 定义.实际阶次 != 最大来源阶次 + 1)
            return {false, 特征类派生数据状态::内部不一致, 截止, 0, {}};
        if (!宿主关系.empty()) {
            const auto& 关系 = 宿主关系.front();
            if (!有效(关系.编码) || 关系.源节点 != 节点.编码
                || !有效(关系.目标节点)
                || 关系.关系类型节点 != 派生结构类型_.宿主关系类型
                || 关系.写入所有者 != 所有者_ || 关系.角色或顺序 != 1
                || 关系.创建事实代次 != 节点.创建事实代次
                || 关系.退出事实代次)
                return {false, 特征类派生数据状态::内部不一致, 截止, 0, {}};
            定义.宿主存在 = 关系.目标节点;
            定义.宿主关系 = 关系.编码;
        }
        if (!读取比较注册(节点, 注册关系.front(), 截止, 定义))
            return {false, 特征类派生数据状态::内部不一致, 截止, 0, {}};
        return {true, 特征类派生数据状态::已读取,
            截止, 定义.实际阶次, std::move(定义)};
    }

    bool 读取比较注册(const L1所有者范围节点事实& 定义节点,
        const L1所有者范围关系事实& 归属关系, std::uint64_t 截止,
        特征类派生定义事实& 定义) const {
        if (!有效(归属关系.编码) || 归属关系.源节点 != 定义节点.编码
            || !有效(归属关系.目标节点)
            || 归属关系.关系类型节点
                != 派生结构类型_.比较注册归属关系类型
            || 归属关系.角色或顺序 != 1
            || 归属关系.写入所有者 != 所有者_
            || 归属关系.创建事实代次 != 定义节点.创建事实代次
            || 归属关系.退出事实代次)
            return false;
        const auto 注册读取 = 第一层服务_.读取所有者范围当前节点(
            {L1所有者范围CRUD合同版本, 归属关系.目标节点});
        const auto* 注册节点 = 注册读取.事实
            ? std::get_if<L1所有者范围节点事实>(&*注册读取.事实) : nullptr;
        if (注册读取.状态 != L1所有者范围读取状态::成功
            || 注册读取.合同版本 != L1所有者范围CRUD合同版本
            || 注册读取.查询编码 != 归属关系.目标节点
            || 注册读取.读取事实代次 != 截止 || !注册节点
            || 注册节点->编码 != 归属关系.目标节点
            || 注册节点->写入所有者 != 所有者_
            || 注册节点->种类 != 节点种类::普通
            || 注册节点->属性类型表示 || 注册节点->当前属性.size() != 2
            || 注册节点->创建事实代次 != 定义节点.创建事实代次
            || 注册节点->退出事实代次)
            return false;
        const auto* U64槽 = 查找属性槽(
            *注册节点, 派生结构类型_.比较注册U64属性类型);
        const auto* I64槽 = 查找属性槽(
            *注册节点, 派生结构类型_.比较注册I64属性类型);
        if (!U64槽 || !I64槽) return false;
        const auto U64值 = 读取当前值事实(*U64槽, 注册节点->编码, 截止);
        const auto I64值 = 读取当前值事实(*I64槽, 注册节点->编码, 截止);
        if (!U64值 || !I64值
            || U64值->创建事实代次 != 定义节点.创建事实代次
            || I64值->创建事实代次 != 定义节点.创建事实代次)
            return false;
        const auto* U64 = std::get_if<std::vector<std::uint64_t>>(&U64值->材料);
        const auto* I64 = std::get_if<std::vector<std::int64_t>>(&I64值->材料);
        if (!U64 || U64->size() != 16 || !I64 || I64->size() != 4
            || (*U64)[0] != 特征类派生数据合同版本 || (*U64)[2] != 2
            || (*U64)[4] != 1 || (*U64)[7] != 1 || (*U64)[12] != 1
            || (*U64)[13] != 1 || (*U64)[1] < 1 || (*U64)[1] > 2
            || (*U64)[3] == 0
            || (*U64)[3] > std::numeric_limits<std::uint32_t>::max()
            || (*U64)[5] < 1 || (*U64)[5] > 4
            || (*U64)[6] < 1 || (*U64)[6] > 4
            || (*U64)[8] == 0 || (*U64)[9] == 0 || (*U64)[10] == 0
            || (*U64)[11] < 1 || (*U64)[11] > 7 || (*U64)[14] == 0
            || (*U64)[14] > std::numeric_limits<std::uint32_t>::max()
            || (*U64)[15] != 定义.派生规则.规则身份.值
            || ((*I64)[0] != 0 && (*I64)[0] != 1)
            || ((*I64)[2] != 0 && (*I64)[2] != 1)
            || ((*I64)[0] == 0 && (*I64)[1] != 0)
            || ((*I64)[2] == 0 && (*I64)[3] != 0)
            || ((*I64)[0] == 1 && (*I64)[1] < 0)
            || ((*I64)[2] == 1 && (*I64)[3] < 0))
            return false;
        特征类I64比较注册合同 合同;
        合同.用途 = static_cast<特征类比较用途>((*U64)[1]);
        合同.算法版本 = static_cast<std::uint32_t>((*U64)[3]);
        合同.左角色 = static_cast<特征类比较角色>((*U64)[5]);
        合同.右角色 = static_cast<特征类比较角色>((*U64)[6]);
        合同.单位 = 稳定编码{(*U64)[8]};
        合同.维度 = 稳定编码{(*U64)[9]};
        合同.分量角色 = 稳定编码{(*U64)[10]};
        合同.允许结果位 = static_cast<std::uint8_t>((*U64)[11]);
        合同.误差合同版本 = static_cast<std::uint32_t>((*U64)[14]);
        if ((*I64)[0] == 1) 合同.误差预算 = (*I64)[1];
        if ((*I64)[2] == 1) 合同.相等容差 = (*I64)[3];
        if (!比较合同有效(合同, 定义.派生规则)) return false;
        定义.比较注册 = {{注册节点->编码}, std::move(合同),
            归属关系.编码, U64值->编码, I64值->编码};
        return true;
    }

    static std::vector<std::uint64_t> 编码比较U64合同(
        const 特征类派生规则& 规则,
        const 特征类I64比较注册合同& 合同) {
        return {特征类派生数据合同版本,
            static_cast<std::uint64_t>(合同.用途), 2, 合同.算法版本, 1,
            static_cast<std::uint64_t>(合同.左角色),
            static_cast<std::uint64_t>(合同.右角色), 1,
            合同.单位.值, 合同.维度.值, 合同.分量角色.值,
            合同.允许结果位, 1, 1, 合同.误差合同版本, 规则.规则身份.值};
    }

    static std::vector<std::int64_t> 编码比较I64合同(
        const 特征类I64比较注册合同& 合同) {
        return {合同.误差预算 ? 1 : 0, 合同.误差预算.value_or(0),
            合同.相等容差 ? 1 : 0, 合同.相等容差.value_or(0)};
    }

    L1所有者范围写集请求 形成派生新增写集(
        const 特征类派生定义新增请求& 请求,
        std::uint32_t 实际阶次) const {
        using namespace 特征类数据内部;
        L1所有者范围写集请求 写集;
        写集.合同版本 = L1所有者范围CRUD合同版本;
        写集.期望事实代次 = 请求.期望事实代次;
        写集.写入幂等身份 = 请求.幂等身份;
        写集.节点 = {
            {派生定义本地键, 节点种类::普通, std::nullopt},
            {比较注册本地键, 节点种类::普通, std::nullopt}};
        写集.关系 = {
            {第一来源关系本地键, 派生定义本地键,
                请求.直接来源[0].来源特征定义.值,
                派生结构类型_.直接来源关系类型,
                static_cast<std::int64_t>(
                    (static_cast<std::uint64_t>(请求.直接来源[0].顺序) << 32U)
                    | 请求.直接来源[0].输入角色)},
            {第二来源关系本地键, 派生定义本地键,
                请求.直接来源[1].来源特征定义.值,
                派生结构类型_.直接来源关系类型,
                static_cast<std::int64_t>(
                    (static_cast<std::uint64_t>(请求.直接来源[1].顺序) << 32U)
                    | 请求.直接来源[1].输入角色)},
            {注册归属关系本地键, 派生定义本地键, 比较注册本地键,
                派生结构类型_.比较注册归属关系类型, 1}};
        if (请求.宿主存在)
            写集.关系.push_back({宿主关系本地键, 派生定义本地键,
                *请求.宿主存在, 派生结构类型_.宿主关系类型, 1});
        写集.值 = {
            {实际阶次值本地键, 派生定义本地键,
                派生结构类型_.实际阶次属性类型,
                static_cast<std::int64_t>(实际阶次), 派生定义本地键},
            {派生规则值本地键, 派生定义本地键,
                派生结构类型_.派生规则属性类型,
                std::vector<std::uint64_t>{请求.派生规则.规则身份.值,
                    请求.派生规则.规则版本}, 派生定义本地键},
            {注册U64值本地键, 比较注册本地键,
                派生结构类型_.比较注册U64属性类型,
                编码比较U64合同(请求.派生规则, 请求.比较合同),
                比较注册本地键},
            {注册I64值本地键, 比较注册本地键,
                派生结构类型_.比较注册I64属性类型,
                编码比较I64合同(请求.比较合同), 比较注册本地键}};
        写集.属性槽变更 = {
            {派生定义本地键, 派生结构类型_.实际阶次属性类型,
                实际阶次值本地键},
            {派生定义本地键, 派生结构类型_.派生规则属性类型,
                派生规则值本地键},
            {比较注册本地键, 派生结构类型_.比较注册U64属性类型,
                注册U64值本地键},
            {比较注册本地键, 派生结构类型_.比较注册I64属性类型,
                注册I64值本地键}};
        return 写集;
    }

    特征类派生定义结果 提交派生新增(
        const L1所有者范围写集请求& 写集,
        const 特征类派生定义新增请求& 请求,
        特征类派生数据状态 成功状态) {
        const auto 写入 = 写入端口_.提交所有者范围中性写集(写集);
        const auto 状态 = 映射派生写入状态(写入.状态, 成功状态);
        if (状态 != 成功状态 && 状态 != 特征类派生数据状态::精确重复)
            return 派生失败(状态, 写入.事实代次);
        const std::size_t 期望映射数量 = 请求.宿主存在 ? 10 : 9;
        if (!写入结果头完整(写入, 请求.幂等身份)
            || 写入.新编码映射.size() != 期望映射数量)
            return 派生失败(写入.状态 == L1所有者范围写入状态::成功
                    ? 特征类派生数据状态::已可能发布
                    : 特征类派生数据状态::内部不一致,
                写入.事实代次);
        const auto 定义身份 = 特征类数据内部::查找唯一编码(
            写入, 特征类数据内部::派生定义本地键);
        if (!定义身份)
            return 派生失败(特征类派生数据状态::已可能发布,
                写入.事实代次);
        auto 读回 = 查询派生特征定义({特征类派生数据合同版本,
            写入.事实代次, {*定义身份}});
        if (!读回.成功() || 读回.状态 != 特征类派生数据状态::已读取
            || !派生定义符合请求(*读回.定义, 请求))
            return 派生失败(特征类派生数据状态::已可能发布,
                写入.事实代次);
        读回.状态 = 状态;
        return 读回;
    }

    static bool 派生定义符合请求(const 特征类派生定义事实& 定义,
        const 特征类派生定义新增请求& 请求) noexcept {
        return 定义.实际阶次 > 1 && 定义.直接来源.size() == 2
            && 定义.直接来源[0].顺序 == 请求.直接来源[0].顺序
            && 定义.直接来源[0].输入角色 == 请求.直接来源[0].输入角色
            && 定义.直接来源[0].来源特征定义
                == 请求.直接来源[0].来源特征定义
            && 定义.直接来源[1].顺序 == 请求.直接来源[1].顺序
            && 定义.直接来源[1].输入角色 == 请求.直接来源[1].输入角色
            && 定义.直接来源[1].来源特征定义
                == 请求.直接来源[1].来源特征定义
            && 定义.派生规则 == 请求.派生规则
            && 定义.宿主存在 == 请求.宿主存在
            && 定义.比较注册.合同 == 请求.比较合同
            && 定义.创建事实代次 != 0 && !定义.退出事实代次;
    }

    std::optional<特征类派生定义结果> 尝试重放派生新增(
        const 特征类派生定义新增请求& 请求) {
        const auto 首次 = 写入端口_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本, 请求.幂等身份});
        if (首次.状态 == L1所有者范围读取状态::未找到)
            return std::nullopt;
        if (首次.状态 != L1所有者范围读取状态::成功
            || 首次.合同版本 != L1所有者范围首次写入读取合同版本
            || 首次.所有者 != 所有者_ || 首次.写入幂等身份 != 请求.幂等身份
            || !首次.首次规范化写集 || !首次.首次写入结果)
            return 派生失败(特征类派生数据状态::内部不一致,
                首次.读取事实代次);
        const auto& 首次写集 = *首次.首次规范化写集;
        const auto& 首次结果 = *首次.首次写入结果;
        const auto 阶次项 = std::find_if(首次写集.值.begin(), 首次写集.值.end(),
            [](const auto& 项) noexcept {
                return 项.本地键 == 特征类数据内部::实际阶次值本地键;
            });
        const auto* 阶次 = 阶次项 == 首次写集.值.end()
            ? nullptr : std::get_if<std::int64_t>(&阶次项->材料);
        if (!阶次 || *阶次 <= 1
            || static_cast<std::uint64_t>(*阶次)
                > std::numeric_limits<std::uint32_t>::max()
            || 首次结果.状态 != L1所有者范围写入状态::成功
            || !写入结果头完整(首次结果, 请求.幂等身份)
            || 首次写集 != 形成派生新增写集(
                请求, static_cast<std::uint32_t>(*阶次)))
            return 派生失败(特征类派生数据状态::幂等冲突,
                首次结果.事实代次);
        return 提交派生新增(首次写集, 请求,
            特征类派生数据状态::已创建);
    }

    static L1所有者范围写集请求 形成派生退出写集(
        const 特征类派生定义退出请求& 请求,
        const 特征类派生定义事实& 定义, 稳定编码 派生规则值) {
        L1所有者范围写集请求 写集;
        写集.合同版本 = L1所有者范围CRUD合同版本;
        写集.期望事实代次 = 请求.期望事实代次;
        写集.写入幂等身份 = 请求.幂等身份;
        写集.退出事实 = {定义.身份.值, 定义.实际阶次值, 派生规则值,
            定义.比较注册.身份.值, 定义.比较注册.归属关系,
            定义.比较注册.U64合同值, 定义.比较注册.I64合同值};
        for (const auto& 来源 : 定义.直接来源)
            写集.退出事实.push_back(来源.关系);
        if (定义.宿主关系) 写集.退出事实.push_back(*定义.宿主关系);
        return 写集;
    }

    std::optional<稳定编码> 读取派生规则值身份(
        稳定编码 定义身份, std::uint64_t 截止) const {
        const auto 读取 = 第一层服务_.读取所有者范围当前节点(
            {L1所有者范围CRUD合同版本, 定义身份});
        const auto* 节点 = 读取.事实
            ? std::get_if<L1所有者范围节点事实>(&*读取.事实) : nullptr;
        if (读取.状态 != L1所有者范围读取状态::成功
            || 读取.读取事实代次 != 截止 || !节点)
            return std::nullopt;
        const auto* 槽 = 查找属性槽(*节点,
            派生结构类型_.派生规则属性类型);
        return 槽 && 有效(槽->当前值)
            ? std::optional<稳定编码>{槽->当前值} : std::nullopt;
    }

    std::optional<特征类派生定义结果> 尝试重放派生退出(
        const 特征类派生定义退出请求& 请求) {
        const auto 首次 = 写入端口_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本, 请求.幂等身份});
        if (首次.状态 == L1所有者范围读取状态::未找到)
            return std::nullopt;
        if (首次.状态 != L1所有者范围读取状态::成功
            || 首次.合同版本 != L1所有者范围首次写入读取合同版本
            || 首次.所有者 != 所有者_ || 首次.写入幂等身份 != 请求.幂等身份
            || !首次.首次规范化写集 || !首次.首次写入结果)
            return 派生失败(特征类派生数据状态::内部不一致,
                首次.读取事实代次);
        const auto& 写集 = *首次.首次规范化写集;
        const auto& 首次结果 = *首次.首次写入结果;
        if (!写集.节点.empty() || !写集.关系.empty() || !写集.值.empty()
            || !写集.属性槽变更.empty()
            || (写集.退出事实.size() != 9
                && 写集.退出事实.size() != 10)
            || std::count(写集.退出事实.begin(), 写集.退出事实.end(),
                请求.特征定义.值) != 1
            || 写集.期望事实代次 != 请求.期望事实代次
            || 写集.写入幂等身份 != 请求.幂等身份
            || 首次结果.状态 != L1所有者范围写入状态::成功
            || !写入结果头完整(首次结果, 请求.幂等身份))
            return 派生失败(特征类派生数据状态::幂等冲突,
                首次结果.事实代次);
        const auto 写入 = 写入端口_.提交所有者范围中性写集(写集);
        if (写入.状态 != L1所有者范围写入状态::精确重复
            || !写入结果头完整(写入, 请求.幂等身份)
            || !写入.新编码映射.empty())
            return 派生失败(特征类派生数据状态::内部不一致,
                写入.事实代次);
        auto 历史 = 读取历史派生定义(
            请求.特征定义, 写入.事实代次);
        if (!历史.成功())
            return 派生失败(特征类派生数据状态::已可能发布,
                写入.事实代次);
        const auto 规则值 = 读取历史派生规则值身份(
            请求.特征定义.值, 写入.事实代次);
        if (!规则值)
            return 派生失败(特征类派生数据状态::已可能发布,
                写入.事实代次);
        auto 期望写集 = 形成派生退出写集(请求, *历史.定义, *规则值);
        std::sort(期望写集.退出事实.begin(), 期望写集.退出事实.end());
        if (写集 != 期望写集)
            return 派生失败(特征类派生数据状态::幂等冲突,
                写入.事实代次);
        历史.状态 = 特征类派生数据状态::精确重复;
        return 历史;
    }

    std::optional<稳定编码> 读取历史派生规则值身份(
        稳定编码 定义身份, std::uint64_t 退出代次) const {
        const auto 读取 = 第一层服务_.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 定义身份});
        const auto* 节点 = 读取.事实
            ? std::get_if<L1所有者范围节点事实>(&*读取.事实) : nullptr;
        if (读取.状态 != L1所有者范围读取状态::成功 || !节点
            || 节点->编码 != 定义身份 || 节点->写入所有者 != 所有者_
            || 节点->退出事实代次 != 退出代次)
            return std::nullopt;
        const auto* 槽 = 查找属性槽(*节点,
            派生结构类型_.派生规则属性类型);
        return 槽 && 有效(槽->当前值)
            ? std::optional<稳定编码>{槽->当前值} : std::nullopt;
    }

    bool 退出事实同代闭合(const std::vector<稳定编码>& 编码组,
        std::uint64_t 退出代次) const {
        if (编码组.empty() || 退出代次 == 0) return false;
        for (const auto 编码 : 编码组) {
            const auto 读取 = 第一层服务_.读取所有者范围历史事实(
                {L1所有者范围CRUD合同版本, 编码});
            if (读取.状态 != L1所有者范围读取状态::成功
                || 读取.合同版本 != L1所有者范围CRUD合同版本
                || 读取.查询编码 != 编码 || !读取.事实)
                return false;
            const auto 检查 = [&](const auto& 事实) noexcept {
                return 事实.编码 == 编码 && 事实.写入所有者 == 所有者_
                    && 事实.退出事实代次 == 退出代次;
            };
            bool 闭合 = false;
            if (const auto* 节点 = std::get_if<L1所有者范围节点事实>(&*读取.事实))
                闭合 = 检查(*节点);
            else if (const auto* 关系 = std::get_if<L1所有者范围关系事实>(&*读取.事实))
                闭合 = 检查(*关系);
            else if (const auto* 值 = std::get_if<L1所有者范围值事实>(&*读取.事实))
                闭合 = 检查(*值);
            if (!闭合) return false;
        }
        return true;
    }

    特征类派生定义结果 读取历史派生定义(
        特征类定义身份 身份, std::uint64_t 退出代次) const {
        if (退出代次 <= 1) return 派生失败(
            特征类派生数据状态::内部不一致, 退出代次);
        const auto 节点读取 = 第一层服务_.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 身份.值});
        const auto* 节点 = 节点读取.事实
            ? std::get_if<L1所有者范围节点事实>(&*节点读取.事实) : nullptr;
        if (节点读取.状态 != L1所有者范围读取状态::成功 || !节点
            || 节点->编码 != 身份.值 || 节点->写入所有者 != 所有者_
            || 节点->种类 != 节点种类::普通 || 节点->属性类型表示
            || 节点->创建事实代次 == 0
            || 节点->创建事实代次 >= 退出代次
            || 节点->退出事实代次 != 退出代次 || 节点->当前属性.size() != 2)
            return 派生失败(特征类派生数据状态::内部不一致, 退出代次);
        const auto* 阶次槽 = 查找属性槽(*节点,
            派生结构类型_.实际阶次属性类型);
        const auto* 规则槽 = 查找属性槽(*节点,
            派生结构类型_.派生规则属性类型);
        if (!阶次槽 || !规则槽)
            return 派生失败(特征类派生数据状态::内部不一致, 退出代次);
        const auto 阶次读取 = 第一层服务_.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 阶次槽->当前值});
        const auto 规则读取 = 第一层服务_.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 规则槽->当前值});
        const auto* 阶次值 = 阶次读取.事实
            ? std::get_if<L1所有者范围值事实>(&*阶次读取.事实) : nullptr;
        const auto* 规则值 = 规则读取.事实
            ? std::get_if<L1所有者范围值事实>(&*规则读取.事实) : nullptr;
        const auto* 阶次 = 阶次值
            ? std::get_if<std::int64_t>(&阶次值->材料) : nullptr;
        const auto* 规则 = 规则值
            ? std::get_if<std::vector<std::uint64_t>>(&规则值->材料) : nullptr;
        if (!阶次值 || !规则值 || !阶次 || *阶次 <= 1
            || static_cast<std::uint64_t>(*阶次)
                > std::numeric_limits<std::uint32_t>::max() || !规则
            || 规则->size() != 2 || (*规则)[0] == 0 || (*规则)[1] == 0
            || (*规则)[1] > std::numeric_limits<std::uint32_t>::max()
            || 阶次值->所属节点 != 身份.值 || 规则值->所属节点 != 身份.值
            || 阶次值->写入所有者 != 所有者_
            || 规则值->写入所有者 != 所有者_
            || 阶次值->创建事实代次 != 节点->创建事实代次
            || 规则值->创建事实代次 != 节点->创建事实代次
            || 阶次值->退出事实代次 != 退出代次
            || 规则值->退出事实代次 != 退出代次)
            return 派生失败(特征类派生数据状态::内部不一致, 退出代次);
        const auto 截止 = 退出代次 - 1;
        const auto 读组 = [&](稳定编码 类型) {
            return 第一层服务_.读取所有者范围历史关系组({
                L1所有者范围CRUD合同版本,
                L1所有者范围关系端点方向::源, 身份.值, 类型, 截止});
        };
        const auto 来源读 = 读组(派生结构类型_.直接来源关系类型);
        const auto 宿主读 = 读组(派生结构类型_.宿主关系类型);
        const auto 注册读 = 读组(派生结构类型_.比较注册归属关系类型);
        if (来源读.状态 != L1所有者范围读取状态::成功
            || 宿主读.状态 != L1所有者范围读取状态::成功
            || 注册读.状态 != L1所有者范围读取状态::成功
            || 来源读.关系组.size() != 2 || 宿主读.关系组.size() > 1
            || 注册读.关系组.size() != 1)
            return 派生失败(特征类派生数据状态::内部不一致, 退出代次);
        auto 来源关系 = 来源读.关系组;
        std::sort(来源关系.begin(), 来源关系.end(),
            [](const auto& 左, const auto& 右) noexcept {
                return 左.角色或顺序 < 右.角色或顺序;
            });
        特征类派生定义事实 定义;
        定义.身份 = 身份;
        定义.实际阶次值 = 阶次值->编码;
        定义.实际阶次 = static_cast<std::uint32_t>(*阶次);
        定义.派生规则 = {稳定编码{(*规则)[0]},
            static_cast<std::uint32_t>((*规则)[1])};
        定义.创建事实代次 = 节点->创建事实代次;
        定义.退出事实代次 = 退出代次;
        for (std::size_t i = 0; i < 来源关系.size(); ++i) {
            const auto& 关系 = 来源关系[i];
            const auto 角色编码 = static_cast<std::uint64_t>(关系.角色或顺序);
            const auto 顺序 = static_cast<std::uint32_t>(角色编码 >> 32U);
            const auto 输入角色 = static_cast<std::uint32_t>(角色编码);
            if (关系.源节点 != 身份.值
                || 关系.关系类型节点 != 派生结构类型_.直接来源关系类型
                || 关系.写入所有者 != 所有者_ || 关系.退出事实代次 != 退出代次
                || 关系.创建事实代次 != 节点->创建事实代次
                || 顺序 != i + 1 || 输入角色 == 0)
                return 派生失败(特征类派生数据状态::内部不一致, 退出代次);
            定义.直接来源.push_back({关系.编码, 顺序, 输入角色,
                {关系.目标节点}});
        }
        if (!宿主读.关系组.empty()) {
            const auto& 关系 = 宿主读.关系组.front();
            if (关系.写入所有者 != 所有者_
                || 关系.源节点 != 身份.值
                || 关系.关系类型节点 != 派生结构类型_.宿主关系类型
                || !有效(关系.目标节点)
                || 关系.创建事实代次 != 节点->创建事实代次
                || 关系.退出事实代次 != 退出代次 || 关系.角色或顺序 != 1)
                return 派生失败(特征类派生数据状态::内部不一致, 退出代次);
            定义.宿主存在 = 关系.目标节点;
            定义.宿主关系 = 关系.编码;
        }
        if (!读取历史比较注册(注册读.关系组.front(), 定义, 退出代次))
            return 派生失败(特征类派生数据状态::内部不一致, 退出代次);
        if (定义.直接来源[0].来源特征定义
                == 定义.直接来源[1].来源特征定义
            || 定义.直接来源[0].输入角色 == 定义.直接来源[1].输入角色)
            return 派生失败(特征类派生数据状态::内部不一致, 退出代次);
        return {特征类派生数据状态::已退出,
            特征类派生数据合同版本, 退出代次, std::move(定义)};
    }

    bool 读取历史比较注册(const L1所有者范围关系事实& 归属关系,
        特征类派生定义事实& 定义, std::uint64_t 退出代次) const {
        if (归属关系.源节点 != 定义.身份.值
            || 归属关系.关系类型节点
                != 派生结构类型_.比较注册归属关系类型
            || 归属关系.写入所有者 != 所有者_
            || 归属关系.创建事实代次 != 定义.创建事实代次
            || 归属关系.退出事实代次 != 退出代次
            || 归属关系.角色或顺序 != 1)
            return false;
        const auto 节点读取 = 第一层服务_.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 归属关系.目标节点});
        const auto* 节点 = 节点读取.事实
            ? std::get_if<L1所有者范围节点事实>(&*节点读取.事实) : nullptr;
        if (节点读取.状态 != L1所有者范围读取状态::成功 || !节点
            || 节点->编码 != 归属关系.目标节点
            || 节点->写入所有者 != 所有者_
            || 节点->种类 != 节点种类::普通 || 节点->属性类型表示
            || 节点->创建事实代次 != 定义.创建事实代次
            || 节点->退出事实代次 != 退出代次
            || 节点->当前属性.size() != 2)
            return false;
        const auto* U64槽 = 查找属性槽(*节点,
            派生结构类型_.比较注册U64属性类型);
        const auto* I64槽 = 查找属性槽(*节点,
            派生结构类型_.比较注册I64属性类型);
        if (!U64槽 || !I64槽) return false;
        const auto U64读取 = 第一层服务_.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, U64槽->当前值});
        const auto I64读取 = 第一层服务_.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, I64槽->当前值});
        const auto* U64值 = U64读取.事实
            ? std::get_if<L1所有者范围值事实>(&*U64读取.事实) : nullptr;
        const auto* I64值 = I64读取.事实
            ? std::get_if<L1所有者范围值事实>(&*I64读取.事实) : nullptr;
        const auto* U64 = U64值
            ? std::get_if<std::vector<std::uint64_t>>(&U64值->材料) : nullptr;
        const auto* I64 = I64值
            ? std::get_if<std::vector<std::int64_t>>(&I64值->材料) : nullptr;
        if (!U64值 || !I64值
            || U64值->所属节点 != 节点->编码
            || I64值->所属节点 != 节点->编码
            || U64值->写入所有者 != 所有者_
            || I64值->写入所有者 != 所有者_
            || U64值->创建事实代次 != 定义.创建事实代次
            || I64值->创建事实代次 != 定义.创建事实代次
            || U64值->退出事实代次 != 退出代次
            || I64值->退出事实代次 != 退出代次 || !U64 || U64->size() != 16
            || !I64 || I64->size() != 4 || (*U64)[0] != 特征类派生数据合同版本
            || (*U64)[2] != 2 || (*U64)[4] != 1 || (*U64)[7] != 1
            || (*U64)[12] != 1 || (*U64)[13] != 1 || (*U64)[1] < 1
            || (*U64)[1] > 2 || (*U64)[3] == 0
            || (*U64)[3] > std::numeric_limits<std::uint32_t>::max()
            || (*U64)[5] < 1
            || (*U64)[5] > 4 || (*U64)[6] < 1 || (*U64)[6] > 4
            || (*U64)[8] == 0 || (*U64)[9] == 0 || (*U64)[10] == 0
            || (*U64)[11] < 1 || (*U64)[11] > 7 || (*U64)[14] == 0
            || (*U64)[14] > std::numeric_limits<std::uint32_t>::max()
            || (*U64)[15] != 定义.派生规则.规则身份.值
            || ((*I64)[0] != 0 && (*I64)[0] != 1)
            || ((*I64)[2] != 0 && (*I64)[2] != 1)
            || ((*I64)[0] == 0 && (*I64)[1] != 0)
            || ((*I64)[2] == 0 && (*I64)[3] != 0)
            || ((*I64)[0] == 1 && (*I64)[1] < 0)
            || ((*I64)[2] == 1 && (*I64)[3] < 0))
            return false;
        特征类I64比较注册合同 合同;
        合同.用途 = static_cast<特征类比较用途>((*U64)[1]);
        合同.算法版本 = static_cast<std::uint32_t>((*U64)[3]);
        合同.左角色 = static_cast<特征类比较角色>((*U64)[5]);
        合同.右角色 = static_cast<特征类比较角色>((*U64)[6]);
        合同.单位 = 稳定编码{(*U64)[8]};
        合同.维度 = 稳定编码{(*U64)[9]};
        合同.分量角色 = 稳定编码{(*U64)[10]};
        合同.允许结果位 = static_cast<std::uint8_t>((*U64)[11]);
        合同.误差合同版本 = static_cast<std::uint32_t>((*U64)[14]);
        if ((*I64)[0] == 1) 合同.误差预算 = (*I64)[1];
        if ((*I64)[2] == 1) 合同.相等容差 = (*I64)[3];
        if (!比较合同有效(合同, 定义.派生规则)) return false;
        定义.比较注册 = {{节点->编码}, std::move(合同), 归属关系.编码,
            U64值->编码, I64值->编码};
        return true;
    }





    static bool 特征值形状有效(const 特征类值& 特征值) noexcept {
        if (const auto* 直接值 = std::get_if<特征直接值>(&特征值)) {
            return std::visit([](const auto& 值) noexcept {
                using 类型 = std::decay_t<decltype(值)>;
                if constexpr (std::is_same_v<类型, std::int64_t>) return true;
                else return !值.empty();
            }, *直接值);
        }
        const auto* 引用 = std::get_if<特征值结点引用>(&特征值);
        return 引用 && 有效(引用->结点);
    }

    static L1所有者范围原始值材料 转换为L1材料(
        const 特征类值& 特征值) {
        if (const auto* 直接值 = std::get_if<特征直接值>(&特征值)) {
            return std::visit([](const auto& 值)
                -> L1所有者范围原始值材料 { return 值; }, *直接值);
        }
        return L1所有者范围独立材料引用{
            std::get<特征值结点引用>(特征值).结点};
    }

    static 特征类值 转换为特征类值(
        const L1所有者范围原始值材料& 材料) {
        return std::visit([](const auto& 值) -> 特征类值 {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型,
                L1所有者范围独立材料引用>)
                return 特征值结点引用{值.编码};
            else
                return 特征直接值{值};
        }, 材料);
    }

    static L1所有者范围值表示种类 特征值表示(
        const 特征类值& 特征值) noexcept {
        if (std::holds_alternative<特征值结点引用>(特征值))
            return L1所有者范围值表示种类::独立材料引用;
        return std::visit([](const auto& 值) noexcept {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, std::int64_t>)
                return L1所有者范围值表示种类::I64;
            else if constexpr (std::is_same_v<类型,
                std::vector<std::int64_t>>)
                return L1所有者范围值表示种类::I64组;
            else
                return L1所有者范围值表示种类::U64组;
        }, std::get<特征直接值>(特征值));
    }

    static bool 新增请求有效(const 特征类新增请求& 请求) noexcept {
        return 请求.合同版本 == 特征类数据合同版本
            && 请求.期望事实代次 != 0 && 有效(请求.幂等身份)
            && 有效(请求.特征类型)
            && 特征值形状有效(请求.特征值);
    }

    static bool 修改请求有效(const 特征类修改请求& 请求) noexcept {
        return 请求.合同版本 == 特征类数据合同版本
            && 请求.期望事实代次 != 0 && 有效(请求.幂等身份)
            && 有效(请求.原特征结点) && 有效(请求.新特征类型)
            && 特征值形状有效(请求.新特征值);
    }

    static bool 删除请求有效(const 特征类删除请求& 请求) noexcept {
        return 请求.合同版本 == 特征类数据合同版本
            && 请求.期望事实代次 != 0 && 有效(请求.幂等身份)
            && 有效(请求.特征结点);
    }

    static bool 查询请求有效(const 特征类查询请求& 请求) noexcept {
        return 请求.合同版本 == 特征类数据合同版本
            && 请求.期望事实代次 != 0 && 有效(请求.特征结点);
    }

    static bool 类型值查询请求有效(
        const 特征类类型值查询请求& 请求) noexcept {
        return 请求.合同版本 == 特征类数据合同版本
            && 请求.期望事实代次 != 0 && 有效(请求.特征类型)
            && 请求.最大候选数量 != 0
            && 特征值形状有效(请求.特征值);
    }

    std::optional<特征类数据状态> 验证特征类型(
        稳定编码 特征类型,
        const 特征类值& 特征值,
        std::uint64_t 期望事实代次) const {
        L1所有者范围一致当前读取请求 请求;
        请求.合同版本 = L1所有者范围一致当前读取合同版本;
        请求.期望事实代次 = 期望事实代次;
        请求.所有者 = {所有者_};
        请求.节点 = {特征类型};
        const auto 读取 = 第一层服务_.尝试读取所有者范围一致当前投影(请求);
        if (读取.状态 != L1所有者范围一致当前读取状态::成功) {
            if (读取.状态 == L1所有者范围一致当前读取状态::事实代次漂移)
                return 特征类数据状态::事实代次漂移;
            if (读取.状态 == L1所有者范围一致当前读取状态::资源失败)
                return 特征类数据状态::资源失败;
            if (读取.状态 == L1所有者范围一致当前读取状态::入口拒绝)
                return 特征类数据状态::入口拒绝;
            return 特征类数据状态::内部不一致;
        }
        if (读取.合同版本 != L1所有者范围一致当前读取合同版本
            || 读取.期望事实代次 != 期望事实代次
            || 读取.读取事实代次 != 期望事实代次
            || 读取.所有者.size() != 1 || 读取.节点.size() != 1
            || !读取.关系.empty() || !读取.值.empty()
            || !读取.属性值.empty() || !读取.源关系组.empty()
            || !读取.目标关系组.empty()
            || 读取.所有者.front().查询所有者 != 所有者_
            || 读取.所有者.front().状态
                != L1所有者范围一致当前读取项目状态::成功
            || !读取.所有者.front().所有者事实
            || 读取.所有者.front().所有者事实->所有者 != 所有者_
            || 读取.所有者.front().所有者事实->范围种类
                != L1所有者范围种类::独占结构范围
            || 读取.所有者.front().所有者事实->退出事实代次
            || 读取.节点.front().查询编码 != 特征类型)
            return 特征类数据状态::内部不一致;
        const auto& 类型项 = 读取.节点.front();
        if (类型项.状态 == L1所有者范围一致当前读取项目状态::未找到)
            return 特征类数据状态::特征类型未找到;
        if (类型项.状态 == L1所有者范围一致当前读取项目状态::已退出)
            return 特征类数据状态::特征类型已退出;
        if (类型项.状态 != L1所有者范围一致当前读取项目状态::成功
            || 类型项.物理清理事实代次 || 类型项.物理清理墓碑
            || !类型项.事实)
            return 特征类数据状态::入口拒绝;
        const auto& 类型事实 = *类型项.事实;
        if (类型事实.编码 != 特征类型
            || 类型事实.种类 != 节点种类::属性类型
            || !类型事实.属性类型表示
            || *类型事实.属性类型表示
                != 特征值表示(特征值)
            || 类型事实.创建事实代次 == 0
            || 类型事实.创建事实代次 > 读取.读取事实代次
            || 类型事实.退出事实代次)
            return 特征类数据状态::入口拒绝;
        return std::nullopt;
    }

    std::optional<特征类数据状态> 验证特征值结点引用(
        const 特征类值& 特征值,
        std::uint64_t 期望事实代次) const {
        const auto* 引用 = std::get_if<特征值结点引用>(&特征值);
        if (!引用) return std::nullopt;
        const auto 读取 = 特征值服务_.查询不可变材料(
            {特征值类数据合同版本, 期望事实代次, 引用->结点});
        if (读取.状态 == 特征值类数据状态::已读取 && 读取.成功()
            && 读取.事实代次 == 期望事实代次
            && 读取.材料结点 && 读取.材料结点->结点 == 引用->结点)
            return std::nullopt;
        switch (读取.状态) {
        case 特征值类数据状态::入口拒绝:
            return 特征类数据状态::入口拒绝;
        case 特征值类数据状态::事实代次漂移:
            return 特征类数据状态::事实代次漂移;
        case 特征值类数据状态::资源失败:
            return 特征类数据状态::资源失败;
        case 特征值类数据状态::未找到:
        case 特征值类数据状态::目标已退出:
        case 特征值类数据状态::引用冲突:
            return 特征类数据状态::引用冲突;
        default:
            return 特征类数据状态::内部不一致;
        }
    }

    static L1所有者范围写集请求 形成新特征写集(
        std::uint64_t 期望事实代次,
        L1所有者范围写入幂等身份 幂等身份,
        稳定编码 特征类型,
        const 特征类值& 特征值,
        std::vector<稳定编码> 退出事实) {
        L1所有者范围写集请求 写集;
        写集.合同版本 = L1所有者范围CRUD合同版本;
        写集.期望事实代次 = 期望事实代次;
        写集.写入幂等身份 = 幂等身份;
        写集.节点 = {{特征类数据内部::特征结点本地键,
            节点种类::普通, std::nullopt}};
        写集.值 = {{特征类数据内部::特征值本地键,
            特征类数据内部::特征结点本地键, 特征类型,
            转换为L1材料(特征值),
            特征类数据内部::特征结点本地键}};
        写集.属性槽变更 = {{特征类数据内部::特征结点本地键,
            特征类型, 特征类数据内部::特征值本地键}};
        写集.退出事实 = std::move(退出事实);
        return 写集;
    }

    static 特征类结点结果 失败(特征类数据状态 状态,
        std::uint64_t 事实代次 = 0) noexcept {
        return {状态, 特征类数据合同版本, 事实代次,
            std::nullopt, std::nullopt};
    }

    static 特征类组查询结果 组失败(特征类数据状态 状态,
        const 特征类类型值查询请求& 请求,
        std::uint64_t 事实代次 = 0) {
        return {状态, 特征类数据合同版本, 事实代次,
            请求.特征类型, 请求.特征值, {}};
    }

    static 特征类数据状态 映射读取状态(
        L1所有者范围读取状态 状态) noexcept {
        switch (状态) {
        case L1所有者范围读取状态::成功:
            return 特征类数据状态::已读取;
        case L1所有者范围读取状态::入口拒绝:
            return 特征类数据状态::入口拒绝;
        case L1所有者范围读取状态::许可拒绝:
            return 特征类数据状态::内部不一致;
        case L1所有者范围读取状态::未找到:
            return 特征类数据状态::未找到;
        case L1所有者范围读取状态::已退出:
            return 特征类数据状态::目标已退出;
        case L1所有者范围读取状态::事实代次漂移:
            return 特征类数据状态::事实代次漂移;
        case L1所有者范围读取状态::资源失败:
            return 特征类数据状态::资源失败;
        case L1所有者范围读取状态::属性未设置:
        case L1所有者范围读取状态::历史材料已清理:
        case L1所有者范围读取状态::内部不一致:
            return 特征类数据状态::内部不一致;
        }
        return 特征类数据状态::内部不一致;
    }

    static 特征类数据状态 映射写入状态(
        L1所有者范围写入状态 状态,
        特征类数据状态 成功状态) noexcept {
        switch (状态) {
        case L1所有者范围写入状态::成功: return 成功状态;
        case L1所有者范围写入状态::精确重复:
            return 特征类数据状态::精确重复;
        case L1所有者范围写入状态::入口拒绝:
            return 特征类数据状态::入口拒绝;
        case L1所有者范围写入状态::许可拒绝:
            return 特征类数据状态::内部不一致;
        case L1所有者范围写入状态::未找到:
            return 特征类数据状态::特征类型未找到;
        case L1所有者范围写入状态::已退出:
            return 特征类数据状态::特征类型已退出;
        case L1所有者范围写入状态::事实代次漂移:
            return 特征类数据状态::事实代次漂移;
        case L1所有者范围写入状态::幂等冲突:
            return 特征类数据状态::幂等冲突;
        case L1所有者范围写入状态::引用冲突:
            return 特征类数据状态::引用冲突;
        case L1所有者范围写入状态::资源失败:
            return 特征类数据状态::资源失败;
        case L1所有者范围写入状态::内部不一致:
            return 特征类数据状态::内部不一致;
        }
        return 特征类数据状态::内部不一致;
    }

    static 特征类数据状态 映射组读取状态(
        L1所有者范围属性类型当前值组读取状态 状态) noexcept {
        switch (状态) {
        case L1所有者范围属性类型当前值组读取状态::成功:
            return 特征类数据状态::已读取;
        case L1所有者范围属性类型当前值组读取状态::未找到:
            return 特征类数据状态::内部不一致;
        case L1所有者范围属性类型当前值组读取状态::入口拒绝:
            return 特征类数据状态::入口拒绝;
        case L1所有者范围属性类型当前值组读取状态::已退出:
            return 特征类数据状态::特征类型已退出;
        case L1所有者范围属性类型当前值组读取状态::事实代次漂移:
            return 特征类数据状态::事实代次漂移;
        case L1所有者范围属性类型当前值组读取状态::数量预算不足:
            return 特征类数据状态::数量预算不足;
        case L1所有者范围属性类型当前值组读取状态::资源失败:
            return 特征类数据状态::资源失败;
        case L1所有者范围属性类型当前值组读取状态::内部不一致:
            return 特征类数据状态::内部不一致;
        }
        return 特征类数据状态::内部不一致;
    }

    bool 写入结果头完整(const L1所有者范围写入结果& 写入,
        L1所有者范围写入幂等身份 幂等身份) const noexcept {
        const bool 首次完整 = 写入.状态 == L1所有者范围写入状态::成功
            && 写入.是否形成内存权威发布
            && 写入.重试边界 == L1所有者范围重试边界::不适用;
        const bool 重复完整 =
            写入.状态 == L1所有者范围写入状态::精确重复
            && !写入.是否形成内存权威发布
            && 写入.重试边界
                == L1所有者范围重试边界::原幂等身份读回收敛;
        return 写入.合同版本 == L1所有者范围CRUD合同版本
            && 写入.所有者 == 所有者_
            && 写入.写入幂等身份 == 幂等身份
            && 写入.事实代次 != 0 && (首次完整 || 重复完整);
    }

    std::optional<特征类结点结果> 尝试重放新特征(
        std::uint64_t 期望事实代次,
        L1所有者范围写入幂等身份 幂等身份,
        稳定编码 特征类型,
        const 特征类值& 特征值,
        std::optional<稳定编码> 被换代特征结点,
        特征类数据状态 首次成功状态) {
        const auto 首次 = 写入端口_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本, 幂等身份});
        if (首次.状态 == L1所有者范围读取状态::未找到)
            return std::nullopt;
        if (首次.状态 != L1所有者范围读取状态::成功)
            return 失败(映射读取状态(首次.状态), 首次.读取事实代次);
        if (首次.合同版本 != L1所有者范围首次写入读取合同版本
            || 首次.所有者 != 所有者_
            || 首次.写入幂等身份 != 幂等身份
            || !首次.首次规范化写集 || !首次.首次写入结果)
            return 失败(特征类数据状态::内部不一致,
                首次.读取事实代次);

        const auto& 首次写集 = *首次.首次规范化写集;
        const auto& 首次结果 = *首次.首次写入结果;
        if (首次写集.合同版本 != L1所有者范围CRUD合同版本
            || 首次写集.期望事实代次 != 期望事实代次
            || 首次写集.写入幂等身份 != 幂等身份
            || 首次结果.状态 != L1所有者范围写入状态::成功
            || !写入结果头完整(首次结果, 幂等身份))
            return 失败(特征类数据状态::幂等冲突,
                首次结果.事实代次);

        if (被换代特征结点) {
            if (首次写集.退出事实.size() != 2
                || std::count(首次写集.退出事实.begin(),
                    首次写集.退出事实.end(), *被换代特征结点) != 1)
                return 失败(特征类数据状态::幂等冲突,
                    首次结果.事实代次);
        } else if (!首次写集.退出事实.empty()) {
            return 失败(特征类数据状态::幂等冲突,
                首次结果.事实代次);
        }

        const auto 期望写集 = 形成新特征写集(期望事实代次,
            幂等身份, 特征类型, 特征值, 首次写集.退出事实);
        if (首次写集 != 期望写集)
            return 失败(特征类数据状态::幂等冲突,
                首次结果.事实代次);
        return 提交新特征(首次写集, 幂等身份,
            被换代特征结点, 首次成功状态);
    }

    std::optional<特征类结点结果> 尝试重放删除(
        const 特征类删除请求& 请求) {
        const auto 首次 = 写入端口_.读取首次写入材料(
            {L1所有者范围首次写入读取合同版本, 请求.幂等身份});
        if (首次.状态 == L1所有者范围读取状态::未找到)
            return std::nullopt;
        if (首次.状态 != L1所有者范围读取状态::成功)
            return 失败(映射读取状态(首次.状态), 首次.读取事实代次);
        if (首次.合同版本 != L1所有者范围首次写入读取合同版本
            || 首次.所有者 != 所有者_
            || 首次.写入幂等身份 != 请求.幂等身份
            || !首次.首次规范化写集 || !首次.首次写入结果)
            return 失败(特征类数据状态::内部不一致,
                首次.读取事实代次);

        const auto& 首次写集 = *首次.首次规范化写集;
        const auto& 首次结果 = *首次.首次写入结果;
        if (首次写集.合同版本 != L1所有者范围CRUD合同版本
            || 首次写集.期望事实代次 != 请求.期望事实代次
            || 首次写集.写入幂等身份 != 请求.幂等身份
            || !首次写集.节点.empty() || !首次写集.关系.empty()
            || !首次写集.值.empty() || !首次写集.属性槽变更.empty()
            || 首次写集.退出事实.size() != 2
            || std::count(首次写集.退出事实.begin(),
                首次写集.退出事实.end(), 请求.特征结点) != 1
            || 首次结果.状态 != L1所有者范围写入状态::成功
            || !写入结果头完整(首次结果, 请求.幂等身份)
            || !首次结果.新编码映射.empty())
            return 失败(特征类数据状态::幂等冲突,
                首次结果.事实代次);

        const auto 写入 = 写入端口_.提交所有者范围中性写集(首次写集);
        if (写入.状态 != L1所有者范围写入状态::精确重复
            || !写入结果头完整(写入, 请求.幂等身份)
            || !写入.新编码映射.empty())
            return 失败(写入.状态 == L1所有者范围写入状态::精确重复
                    ? 特征类数据状态::内部不一致
                    : 映射写入状态(写入.状态,
                        特征类数据状态::已退出),
                写入.事实代次);
        auto 读回 = 读取历史已退出特征(
            请求.特征结点, 写入.事实代次);
        if (!读回.成功()) return 读回;
        读回.状态 = 特征类数据状态::精确重复;
        return 读回;
    }

    特征类结点结果 提交新特征(
        const L1所有者范围写集请求& 写集,
        L1所有者范围写入幂等身份 幂等身份,
        std::optional<稳定编码> 被换代特征结点,
        特征类数据状态 成功状态) {
        const auto 写入 = 写入端口_.提交所有者范围中性写集(写集);
        const auto 状态 = 映射写入状态(写入.状态, 成功状态);
        if (状态 != 成功状态 && 状态 != 特征类数据状态::精确重复)
            return 失败(状态, 写入.事实代次);
        if (!写入结果头完整(写入, 幂等身份)
            || 写入.新编码映射.size() != 2)
            return 失败(特征类数据状态::内部不一致, 写入.事实代次);
        const auto 结点 = 特征类数据内部::查找唯一编码(
            写入, 特征类数据内部::特征结点本地键);
        const auto 值 = 特征类数据内部::查找唯一编码(
            写入, 特征类数据内部::特征值本地键);
        if (!结点 || !值)
            return 失败(特征类数据状态::内部不一致, 写入.事实代次);
        auto 读回 = 读取历史新特征(*结点, *值, 写入.事实代次);
        if (!读回.成功()) return 读回;
        if (被换代特征结点) {
            const auto 退出读回 = 读取历史已退出特征(
                *被换代特征结点, 写入.事实代次);
            if (!退出读回.成功()) return 退出读回;
        }
        读回.状态 = 状态;
        读回.被换代特征结点 = 被换代特征结点;
        return 读回;
    }

    特征类结点结果 读取历史新特征(稳定编码 结点,
        稳定编码 值, std::uint64_t 创建事实代次) const {
        const auto 结点读取 = 第一层服务_.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 结点});
        const auto 值读取 = 第一层服务_.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 值});
        if (结点读取.状态 != L1所有者范围读取状态::成功)
            return 失败(映射读取状态(结点读取.状态),
                结点读取.读取事实代次);
        if (值读取.状态 != L1所有者范围读取状态::成功)
            return 失败(映射读取状态(值读取.状态),
                值读取.读取事实代次);
        const auto* 结点事实 = 结点读取.事实
            ? std::get_if<L1所有者范围节点事实>(&*结点读取.事实) : nullptr;
        const auto* 值事实 = 值读取.事实
            ? std::get_if<L1所有者范围值事实>(&*值读取.事实) : nullptr;
        if (!结点事实 || !值事实 || 结点事实->编码 != 结点
            || 值事实->编码 != 值 || 结点事实->写入所有者 != 所有者_
            || 值事实->写入所有者 != 所有者_
            || 结点事实->创建事实代次 != 创建事实代次
            || 值事实->创建事实代次 != 创建事实代次
            || 结点事实->退出事实代次 || 值事实->退出事实代次
            || 结点事实->种类 != 节点种类::普通
            || 结点事实->属性类型表示
            || 结点事实->当前属性.size() != 1
            || 结点事实->当前属性.front().属性类型节点
                != 值事实->属性类型节点
            || 结点事实->当前属性.front().当前值 != 值事实->编码
            || 值事实->所属节点 != 结点 || 值事实->来源节点 != 结点
            || !特征类数据内部::L1材料形状有效(值事实->材料))
            return 失败(特征类数据状态::内部不一致, 创建事实代次);
        特征类结点 特征{结点事实->编码, 值事实->编码,
            值事实->属性类型节点, 转换为特征类值(值事实->材料),
            创建事实代次, std::nullopt};
        return {特征类数据状态::已读取, 特征类数据合同版本,
            创建事实代次, std::move(特征), std::nullopt};
    }

    特征类结点结果 读取历史已退出特征(稳定编码 结点,
        std::uint64_t 退出事实代次) const {
        const auto 结点读取 = 第一层服务_.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 结点});
        const auto* 结点事实 = 结点读取.事实
            ? std::get_if<L1所有者范围节点事实>(&*结点读取.事实) : nullptr;
        if (结点读取.状态 != L1所有者范围读取状态::成功
            || 结点读取.合同版本 != L1所有者范围CRUD合同版本
            || 结点读取.查询编码 != 结点 || 结点读取.物理清理事实代次
            || 结点读取.物理清理墓碑 || !结点事实
            || 结点事实->编码 != 结点
            || 结点事实->写入所有者 != 所有者_
            || 结点事实->种类 != 节点种类::普通
            || 结点事实->属性类型表示
            || 结点事实->创建事实代次 == 0
            || 结点事实->创建事实代次 >= 退出事实代次
            || 结点事实->退出事实代次 != 退出事实代次
            || 结点事实->当前属性.size() != 1)
            return 失败(特征类数据状态::内部不一致,
                结点读取.读取事实代次);

        const auto& 属性槽 = 结点事实->当前属性.front();
        if (!有效(属性槽.属性类型节点) || !有效(属性槽.当前值))
            return 失败(特征类数据状态::内部不一致,
                结点读取.读取事实代次);
        const auto 值读取 = 第一层服务_.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 属性槽.当前值});
        const auto* 值事实 = 值读取.事实
            ? std::get_if<L1所有者范围值事实>(&*值读取.事实) : nullptr;
        if (值读取.状态 != L1所有者范围读取状态::成功
            || 值读取.合同版本 != L1所有者范围CRUD合同版本
            || 值读取.查询编码 != 属性槽.当前值
            || 值读取.物理清理事实代次 || 值读取.物理清理墓碑
            || !值事实 || 值事实->编码 != 属性槽.当前值
            || 值事实->写入所有者 != 所有者_
            || 值事实->所属节点 != 结点
            || 值事实->属性类型节点 != 属性槽.属性类型节点
            || 值事实->来源节点 != 结点
            || 值事实->创建事实代次 != 结点事实->创建事实代次
            || 值事实->退出事实代次 != 退出事实代次
            || !特征类数据内部::L1材料形状有效(值事实->材料))
            return 失败(特征类数据状态::内部不一致,
                值读取.读取事实代次);

        特征类结点 特征{结点, 值事实->编码,
            属性槽.属性类型节点, 转换为特征类值(值事实->材料),
            结点事实->创建事实代次, 退出事实代次};
        return {特征类数据状态::已退出, 特征类数据合同版本,
            退出事实代次, std::move(特征), std::nullopt};
    }

    特征类结点结果 读取当前特征(稳定编码 结点,
        std::uint64_t 期望事实代次) const {
        const auto 初读 = 第一层服务_.读取所有者范围当前节点(
            {L1所有者范围CRUD合同版本, 结点});
        if (初读.状态 != L1所有者范围读取状态::成功)
            return 失败(映射读取状态(初读.状态), 初读.读取事实代次);
        if (初读.合同版本 != L1所有者范围CRUD合同版本
            || 初读.查询编码 != 结点 || 初读.物理清理事实代次
            || 初读.物理清理墓碑)
            return 失败(特征类数据状态::内部不一致,
                初读.读取事实代次);
        if (初读.读取事实代次 != 期望事实代次)
            return 失败(特征类数据状态::事实代次漂移,
                初读.读取事实代次);
        const auto* 初读结点 = 初读.事实
            ? std::get_if<L1所有者范围节点事实>(&*初读.事实) : nullptr;
        if (!初读结点 || 初读结点->编码 != 结点
            || 初读结点->写入所有者 != 所有者_
            || 初读结点->种类 != 节点种类::普通
            || 初读结点->属性类型表示
            || 初读结点->创建事实代次 == 0
            || 初读结点->创建事实代次 > 初读.读取事实代次
            || 初读结点->退出事实代次
            || 初读结点->当前属性.size() != 1
            || !有效(初读结点->当前属性.front().属性类型节点)
            || !有效(初读结点->当前属性.front().当前值))
            return 失败(特征类数据状态::内部不一致,
                初读.读取事实代次);
        const auto 特征类型 = 初读结点->当前属性.front().属性类型节点;
        L1所有者范围一致当前读取请求 请求;
        请求.合同版本 = L1所有者范围一致当前读取合同版本;
        请求.期望事实代次 = 期望事实代次;
        请求.所有者 = {所有者_};
        请求.节点 = {结点};
        请求.属性值 = {{结点, 特征类型}};
        const auto 读取 = 第一层服务_.尝试读取所有者范围一致当前投影(请求);
        if (读取.状态 != L1所有者范围一致当前读取状态::成功) {
            if (读取.状态 == L1所有者范围一致当前读取状态::事实代次漂移)
                return 失败(特征类数据状态::事实代次漂移,
                    读取.读取事实代次);
            if (读取.状态 == L1所有者范围一致当前读取状态::资源失败)
                return 失败(特征类数据状态::资源失败,
                    读取.读取事实代次);
            if (读取.状态 == L1所有者范围一致当前读取状态::入口拒绝)
                return 失败(特征类数据状态::入口拒绝,
                    读取.读取事实代次);
            return 失败(特征类数据状态::内部不一致,
                读取.读取事实代次);
        }
        if (读取.合同版本 != L1所有者范围一致当前读取合同版本
            || 读取.期望事实代次 != 期望事实代次
            || 读取.读取事实代次 != 期望事实代次
            || 读取.所有者.size() != 1 || 读取.节点.size() != 1
            || 读取.属性值.size() != 1 || !读取.关系.empty()
            || !读取.值.empty() || !读取.源关系组.empty()
            || !读取.目标关系组.empty()
            || 读取.所有者.front().查询所有者 != 所有者_
            || 读取.所有者.front().状态
                != L1所有者范围一致当前读取项目状态::成功
            || !读取.所有者.front().所有者事实
            || 读取.所有者.front().所有者事实->所有者 != 所有者_
            || 读取.所有者.front().所有者事实->范围种类
                != L1所有者范围种类::独占结构范围
            || 读取.所有者.front().所有者事实->退出事实代次
            || 读取.节点.front().状态
                != L1所有者范围一致当前读取项目状态::成功
            || 读取.节点.front().查询编码 != 结点
            || 读取.节点.front().物理清理事实代次
            || 读取.节点.front().物理清理墓碑
            || !读取.节点.front().事实
            || 读取.属性值.front().节点 != 结点
            || 读取.属性值.front().属性类型 != 特征类型
            || 读取.属性值.front().状态
                != L1所有者范围一致当前读取项目状态::成功
            || !读取.属性值.front().投影)
            return 失败(特征类数据状态::内部不一致,
                读取.读取事实代次);
        const auto& 结点事实 = *读取.节点.front().事实;
        const auto& 投影 = *读取.属性值.front().投影;
        const auto& 值事实 = 投影.当前值事实;
        if (结点事实.编码 != 结点 || 结点事实.写入所有者 != 所有者_
            || 结点事实.种类 != 节点种类::普通
            || 结点事实.属性类型表示 || 结点事实.创建事实代次 == 0
            || 结点事实.创建事实代次 > 读取.读取事实代次
            || 结点事实.退出事实代次 || 结点事实.当前属性.size() != 1
            || 结点事实.当前属性.front() != 投影.属性槽
            || 投影.属性槽.属性类型节点 != 特征类型
            || 投影.属性槽.当前值 != 值事实.编码
            || 值事实.所属节点 != 结点
            || 值事实.属性类型节点 != 特征类型
            || 值事实.写入所有者 != 所有者_
            || 值事实.创建事实代次 != 结点事实.创建事实代次
            || 值事实.来源节点 != 结点 || 值事实.退出事实代次
            || !特征类数据内部::L1材料形状有效(值事实.材料))
            return 失败(特征类数据状态::内部不一致,
                读取.读取事实代次);
        const auto 特征值 = 转换为特征类值(值事实.材料);
        if (const auto 引用状态 = 验证特征值结点引用(
                特征值, 期望事实代次))
            return 失败(*引用状态, 读取.读取事实代次);
        特征类结点 特征{结点事实.编码, 值事实.编码,
            特征类型, 特征值, 结点事实.创建事实代次,
            std::nullopt};
        return {特征类数据状态::已读取, 特征类数据合同版本,
            读取.读取事实代次, std::move(特征), std::nullopt};
    }

    const L1事实基座服务& 第一层服务_;
    const 特征值类数据服务& 特征值服务_;
    L1所有者范围写端口 写入端口_;
    L1结构所有者身份 所有者_{};
    特征类派生结构类型 派生结构类型_{};
    std::optional<特征类事实集合结构类型> 事实集合结构类型_;
};

} // namespace 海中鱼巣
