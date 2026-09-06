module;

#include <algorithm>
#include <cstdint>
#include <exception>
#include <limits>
#include <array>
#include <bit>
#include <map>
#include <set>
#include <numeric>
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
    稳定编码 输出类型关系类型{};
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


inline constexpr std::uint32_t 标量派生合同版本 = 2;
enum class 特征类标量结果角色 : std::uint8_t { 排序 = 1, 关系 = 2, 差异 = 3 };
enum class 特征类标量量纲 : std::uint8_t { 无量纲 = 1, 有量纲 = 2 };
enum class 特征类标量舍入 : std::uint8_t { 不转换 = 1 };
enum class 特征类标量溢出 : std::uint8_t { 拒绝 = 1 };
enum class 特征类标量方向 : std::uint8_t { 左对右 = 1, 左到右 = 2, 右减左 = 3 };
enum class 特征类标量状态 : std::uint8_t {
    已读取 = 1, 已比较, 入口拒绝, 未找到, 已退出, 格式不支持, 算法不支持,
    类型不匹配, 来源不匹配, 单位量化不匹配, 结果范围不满足, 运算溢出,
    预算不足, 历史材料不可用, 事实代次漂移, 资源失败, 内部不一致,
    已创建, 精确重复, 幂等冲突, 引用冲突, 已可能发布
};
enum class 特征类标量发布确定性 : std::uint8_t {
    未派发 = 1, 确认未发布, 确认已发布, 可能已发布
};
struct 特征类标量量化合同 final {
    稳定编码 单位{}, 维度{}, 分量角色{};
    std::uint64_t 缩放分子 = 1, 缩放分母 = 1;
    std::int64_t 下界 = 0, 上界 = 0;
    特征类标量量纲 量纲类别 = 特征类标量量纲::无量纲;
    特征类标量舍入 舍入 = 特征类标量舍入::不转换;
    特征类标量溢出 溢出 = 特征类标量溢出::拒绝;
    bool 完整() const noexcept {
        return 有效(单位) && 有效(维度) && 有效(分量角色) && 缩放分子 && 缩放分母
            && std::gcd(缩放分子, 缩放分母) == 1 && 下界 <= 上界
            && (量纲类别 == 特征类标量量纲::无量纲 || 量纲类别 == 特征类标量量纲::有量纲)
            && 舍入 == 特征类标量舍入::不转换 && 溢出 == 特征类标量溢出::拒绝;
    }
    friend bool operator==(const 特征类标量量化合同&, const 特征类标量量化合同&) = default;
};
struct 特征类标量输出提交项 final {
    特征类标量结果角色 角色 = 特征类标量结果角色::排序;
    特征类标量量化合同 量化;
    friend bool operator==(const 特征类标量输出提交项&, const 特征类标量输出提交项&) = default;
};
struct 特征类标量输出事实 final {
    特征类标量输出提交项 声明;
    稳定编码 特征类型{}, 归属关系{}, 格式标记值事实{};
    friend bool operator==(const 特征类标量输出事实&, const 特征类标量输出事实&) = default;
};
struct 特征类标量派生来源提交项 final {
    std::uint32_t 顺序 = 0, 输入角色 = 0;
    特征类定义身份 来源;
    std::uint8_t 上游输出角色 = 0;
    friend bool operator==(const 特征类标量派生来源提交项&, const 特征类标量派生来源提交项&) = default;
};
struct 特征类标量派生来源事实 final {
    特征类标量派生来源提交项 内容;
    稳定编码 关系{};
    friend bool operator==(const 特征类标量派生来源事实&, const 特征类标量派生来源事实&) = default;
};
struct 特征类标量比较注册合同 final {
    特征类比较用途 用途 = 特征类比较用途::目标判断;
    std::uint32_t 算法版本 = 1;
    特征类比较角色 左角色 = 特征类比较角色::当前事实;
    特征类比较角色 右角色 = 特征类比较角色::目标状态;
    std::uint8_t 允许结果位 = 0;
    std::uint32_t 误差合同版本 = 0;
    std::optional<std::int64_t> 误差预算, 相等容差;
    特征类标量量化合同 输入量化;
    std::vector<特征类标量输出提交项> 输出组;
    friend bool operator==(const 特征类标量比较注册合同&, const 特征类标量比较注册合同&) = default;
};
struct 特征类派生读取预算 final {
    std::uint64_t 最大定义数 = 0, 最大关系数 = 0, 最大叶数 = 0, 最大属性值数 = 0, 最大深度 = 0;
    bool 有效() const noexcept {
        return 最大定义数 && 最大关系数 && 最大叶数 && 最大属性值数 && 最大深度;
    }
    friend bool operator==(const 特征类派生读取预算&, const 特征类派生读取预算&) = default;
};
inline constexpr 特征类派生读取预算 标量业务准入预算{1024, 8192, 2048, 16384, 64};
struct 特征类标量派生建立请求 final {
    std::uint32_t 版本 = 标量派生合同版本;
    std::uint64_t G = 0;
    L1所有者范围写入幂等身份 幂等身份;
    std::vector<特征类标量派生来源提交项> 来源组;
    特征类派生规则 派生规则;
    std::optional<稳定编码> 宿主E;
    特征类标量比较注册合同 标量注册;
    特征类派生读取预算 预算;
    friend bool operator==(const 特征类标量派生建立请求&, const 特征类标量派生建立请求&) = default;
};
struct 特征类标量派生读取请求 final {
    std::uint32_t 版本 = 标量派生合同版本;
    std::uint64_t Gread = 0, H = 0;
    特征类定义身份 定义身份;
    特征类派生读取预算 预算;
};
struct 特征类标量派生退出请求 final {
    std::uint32_t 版本 = 标量派生合同版本;
    std::uint64_t G = 0;
    L1所有者范围写入幂等身份 幂等身份;
    特征类定义身份 定义身份;
    特征类派生读取预算 预算;
};
struct 特征类标量比较请求 final {
    std::uint32_t 版本 = 标量派生合同版本;
    std::uint64_t G = 0, 请求身份 = 0;
    特征类定义身份 根定义;
    特征类比较用途 用途 = 特征类比较用途::目标判断;
    std::optional<std::uint32_t> 预期算法版本;
    std::uint8_t 要求结果位 = 0;
    特征类派生读取预算 预算;
};
struct 特征类标量派生事实 final {
    特征类定义身份 定义身份;
    std::uint32_t 真实阶次 = 0;
    std::vector<特征类标量派生来源事实> 来源组;
    特征类派生规则 派生规则;
    std::optional<稳定编码> 宿主E, 宿主关系;
    特征类比较注册身份 注册身份;
    稳定编码 注册归属{}, 阶次值{}, 规则值{}, 注册U64值{}, 注册I64值{};
    特征类标量比较注册合同 注册;
    std::vector<特征类标量输出事实> 输出组;
    std::uint64_t 创建G = 0;
    std::optional<std::uint64_t> 退出G;
    bool 完整(std::uint64_t h) const noexcept {
        if (!有效(定义身份.值) || 真实阶次 <= 1 || !创建G || 创建G > h
            || (退出G && (*退出G <= 创建G || *退出G <= h)) || 来源组.size() != 2
            || !有效(派生规则.规则身份) || 派生规则.规则版本 != 1 || !有效(注册身份.值)
            || !有效(注册归属) || !有效(阶次值) || !有效(规则值) || !有效(注册U64值) || !有效(注册I64值)
            || 宿主E.has_value() != 宿主关系.has_value() || (宿主E && (!有效(*宿主E) || !有效(*宿主关系)))
            || 注册.算法版本 != 1 || !注册.误差合同版本 || !注册.输入量化.完整()
            || (注册.误差预算 && *注册.误差预算 < 0) || (注册.相等容差 && *注册.相等容差 < 0)
            || 输出组.empty() || 输出组.size() > 3 || 输出组.size() != 注册.输出组.size()) return false;
        const auto left = static_cast<unsigned>(注册.左角色), right = static_cast<unsigned>(注册.右角色);
        if (注册.用途 == 特征类比较用途::目标判断 ? left != 1 || right != 2
            : 注册.用途 != 特征类比较用途::状态迁移 || left != 3 || right != 4) return false;
        for (unsigned i = 0; i < 2; ++i) {
            const auto& x = 来源组[i];
            if (!有效(x.关系) || !有效(x.内容.来源.值) || x.内容.顺序 != i + 1
                || x.内容.输入角色 != (i ? right : left) || x.内容.上游输出角色 > 3) return false;
        }
        if (来源组[0].内容.来源 == 来源组[1].内容.来源 || 来源组[0].关系 == 来源组[1].关系) return false;
        unsigned mask = 0, previous = 0;
        for (std::size_t i = 0; i < 输出组.size(); ++i) {
            const auto& x = 输出组[i]; const auto role = static_cast<unsigned>(x.声明.角色);
            if (role < 1 || role > 3 || role <= previous || !x.声明.量化.完整()
                || x.声明 != 注册.输出组[i] || !有效(x.特征类型) || !有效(x.归属关系) || !有效(x.格式标记值事实)) return false;
            for (std::size_t j = 0; j < i; ++j)
                if (x.特征类型 == 输出组[j].特征类型 || x.归属关系 == 输出组[j].归属关系
                    || x.格式标记值事实 == 输出组[j].格式标记值事实) return false;
            mask |= 1U << (role - 1); previous = role;
        }
        return mask == 注册.允许结果位;
    }
    friend bool operator==(const 特征类标量派生事实&, const 特征类标量派生事实&) = default;
};
struct 特征类标量叶回执 final {
    稳定编码 F{}, FT{}, 值事实{};
    std::int64_t 值 = 0;
    std::uint64_t G = 0, 创建G = 0;
    std::optional<std::uint64_t> 退出G;
    friend bool operator==(const 特征类标量叶回执&, const 特征类标量叶回执&) = default;
};
struct 特征类标量基础输入回执 final {
    特征类标量叶回执 叶;
    特征类比较角色 角色 = 特征类比较角色::当前事实;
};
struct 特征类标量上游输入回执 final {
    特征类定义身份 定义;
    稳定编码 输出FT{};
    特征类标量结果角色 输出角色 = 特征类标量结果角色::排序;
    std::int64_t 值 = 0;
    std::uint32_t 实际阶次 = 0;
    特征类比较角色 角色 = 特征类比较角色::当前事实;
};
using 特征类标量输入回执 = std::variant<特征类标量基础输入回执, 特征类标量上游输入回执>;
struct 特征类标量结果项 final {
    稳定编码 输出FT{};
    特征类标量结果角色 角色 = 特征类标量结果角色::排序;
    std::int64_t 值 = 0;
    特征类标量量化合同 量化;
    特征类标量方向 方向 = 特征类标量方向::左对右;
};
struct 特征类标量派生读取结果 final {
    std::uint32_t 版本 = 标量派生合同版本;
    特征类标量状态 状态 = 特征类标量状态::入口拒绝;
    std::uint64_t Gread = 0, H = 0;
    std::optional<特征类标量派生事实> 定义事实;
    std::vector<特征类标量叶回执> 基础叶组;
    std::vector<稳定编码> 左叶组, 右叶组;
    bool 成功() const noexcept {
        if (版本 != 2 || 状态 != 特征类标量状态::已读取 || !H || Gread < H
            || !定义事实 || !定义事实->完整(H) || 基础叶组.empty() || 左叶组.empty() || 右叶组.empty()) return false;
        稳定编码 previous{};
        for (const auto& x : 基础叶组) {
            if (!有效(x.F) || !有效(x.FT) || !有效(x.值事实) || x.G != H || !x.创建G || x.创建G > H
                || (x.退出G && *x.退出G <= H) || (有效(previous) && !(previous < x.F))) return false;
            if (std::find(左叶组.begin(), 左叶组.end(), x.F) == 左叶组.end()
                && std::find(右叶组.begin(), 右叶组.end(), x.F) == 右叶组.end()) return false;
            previous = x.F;
        }
        for (const auto* group : {&左叶组, &右叶组}) {
            稳定编码 prior{};
            for (auto id : *group) {
                if (!有效(id) || (有效(prior) && !(prior < id))
                    || std::none_of(基础叶组.begin(), 基础叶组.end(), [&](const auto& x) { return x.F == id; })) return false;
                prior = id;
            }
        }
        return true;
    }
};
struct 特征类标量派生写结果 final {
    std::uint32_t 版本 = 标量派生合同版本;
    特征类标量状态 状态 = 特征类标量状态::入口拒绝;
    std::uint64_t Gread = 0;
    std::optional<std::uint64_t> 首次发布H;
    特征类标量发布确定性 发布确定性 = 特征类标量发布确定性::未派发;
    std::optional<特征类标量派生事实> 定义事实;
    std::optional<L1所有者范围写入结果> 正式回执;
    bool 成功() const noexcept {
        return 版本 == 2 && (状态 == 特征类标量状态::已创建 || 状态 == 特征类标量状态::已退出
                || 状态 == 特征类标量状态::精确重复)
            && 首次发布H && *首次发布H && Gread >= *首次发布H && 定义事实
            && 定义事实->完整(定义事实->创建G)
            && (*首次发布H == 定义事实->创建G || 定义事实->退出G == 首次发布H)
            && (状态 != 特征类标量状态::已创建 || *首次发布H == 定义事实->创建G)
            && (状态 != 特征类标量状态::已退出 || 定义事实->退出G == 首次发布H)
            && 发布确定性 == 特征类标量发布确定性::确认已发布 && 正式回执
            && 正式回执->合同版本 == L1所有者范围CRUD合同版本 && 正式回执->事实代次 == *首次发布H
            && (正式回执->状态 == L1所有者范围写入状态::成功 || 正式回执->状态 == L1所有者范围写入状态::精确重复);
    }
};
struct 特征类标量比较结果 final {
    std::uint32_t 版本 = 标量派生合同版本;
    特征类标量状态 状态 = 特征类标量状态::入口拒绝;
    特征类标量状态 拒绝原因 = 特征类标量状态::入口拒绝;
    std::uint64_t G = 0, 请求身份 = 0;
    特征类定义身份 根定义;
    特征类比较注册身份 注册身份;
    std::uint32_t 算法版本 = 0, 真实阶次 = 0;
    std::uint8_t 实际结果位 = 0;
    特征类标量量化合同 输入量化;
    std::uint32_t 误差合同版本 = 0;
    std::optional<std::int64_t> 误差预算, 相等容差;
    std::optional<std::array<特征类标量输入回执, 2>> 直接输入回执;
    std::vector<特征类标量叶回执> 基础叶回执组;
    std::vector<特征类标量结果项> 结果组;
    bool 成功() const noexcept {
        if (版本 != 2 || 状态 != 特征类标量状态::已比较 || 拒绝原因 != 状态 || !G || !请求身份
            || !有效(根定义.值) || !有效(注册身份.值) || 算法版本 != 1 || 真实阶次 <= 1
            || 实际结果位 == 0 || 实际结果位 > 7 || 基础叶回执组.empty() || !直接输入回执
            || !输入量化.完整() || !误差合同版本 || (误差预算 && *误差预算 < 0) || (相等容差 && *相等容差 < 0)) return false;
        稳定编码 previousLeaf{};
        for (const auto& leaf : 基础叶回执组) {
            if (!有效(leaf.F) || !有效(leaf.FT) || !有效(leaf.值事实) || leaf.G != G
                || !leaf.创建G || leaf.创建G > G || (leaf.退出G && *leaf.退出G <= G)
                || (有效(previousLeaf) && !(previousLeaf < leaf.F))) return false;
            previousLeaf = leaf.F;
        }
        for (unsigned i = 0; i < 2; ++i) {
            const auto& input = (*直接输入回执)[i];
            if (input.valueless_by_exception()) return false;
            const bool valid = std::visit([&](const auto& x) noexcept {
                using T = std::decay_t<decltype(x)>;
                const auto role = static_cast<unsigned>(x.角色);
                if (i == 0 ? (role != 1 && role != 3) : (role != 2 && role != 4)) return false;
                if constexpr (std::is_same_v<T, 特征类标量基础输入回执>) {
                    const auto it = std::find(基础叶回执组.begin(), 基础叶回执组.end(), x.叶);
                    return it != 基础叶回执组.end();
                } else {
                    const auto selected = static_cast<unsigned>(x.输出角色);
                    return 有效(x.定义.值) && 有效(x.输出FT) && x.实际阶次 > 1 && x.实际阶次 < 真实阶次
                        && selected >= 1 && selected <= 3;
                }
            }, input);
            if (!valid) return false;
        }
        const auto leftRole = std::visit([](const auto& x) { return static_cast<unsigned>(x.角色); }, (*直接输入回执)[0]);
        const auto rightRole = std::visit([](const auto& x) { return static_cast<unsigned>(x.角色); }, (*直接输入回执)[1]);
        if (rightRole != leftRole + 1) return false;
        unsigned bits = 0, last = 0;
        for (const auto& x : 结果组) {
            const auto role = static_cast<unsigned>(x.角色);
            if (role < 1 || role > 3 || role <= last || !有效(x.输出FT) || !x.量化.完整()
                || x.值 < x.量化.下界 || x.值 > x.量化.上界
                || static_cast<unsigned>(x.方向) != role) return false;
            bits |= 1U << (role - 1); last = role;
        }
        return bits == 实际结果位;
    }
};

using 特征直接值 = std::variant<std::int64_t, std::vector<std::int64_t>, std::vector<std::uint64_t>>;

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
    内部不一致 = 16,
    已可能发布 = 17
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

// 长期特征的类型域和观测事实，独立于既有标量派生格式。
#define HYC_LONG_ID(Name) \
struct Name final { \
    稳定编码 值{}; \
    Name() = default; \
    explicit Name(稳定编码 x) : 值(x) {} \
    friend bool operator==(const Name&, const Name&) = default; \
};
HYC_LONG_ID(特征类型域身份)
HYC_LONG_ID(特征校正身份)
HYC_LONG_ID(特征跟踪身份)
HYC_LONG_ID(特征区间身份)
HYC_LONG_ID(特征长期观测身份)
HYC_LONG_ID(特征长期类型引用)
HYC_LONG_ID(特征存在上下文引用)
#undef HYC_LONG_ID

struct 特征I64闭区间 final {
    std::int64_t 下界 = 0, 上界 = 0;
    friend bool operator==(const 特征I64闭区间&, const 特征I64闭区间&) = default;
};
struct 特征二值图格式 final {
    std::uint64_t 宽 = 0, 高 = 0;
    稳定编码 坐标系{};
    std::uint64_t 轴约定 = 1, 分辨率分子 = 1, 分辨率分母 = 1;
    std::int64_t 原点X = 0, 原点Y = 0;
    friend bool operator==(const 特征二值图格式&, const 特征二值图格式&) = default;
};
struct 特征二值图 final {
    特征二值图格式 格式;
    std::vector<std::uint64_t> 像素;
    friend bool operator==(const 特征二值图&, const 特征二值图&) = default;
};
struct 特征二值半径域 final {
    特征二值图 代表;
    std::int64_t 半径 = 0;
    friend bool operator==(const 特征二值半径域&, const 特征二值半径域&) = default;
};
enum class 特征长期算法 : std::uint8_t { I64幅度 = 1, 二值边界平均 = 2 };
struct 特征类型域内容 final {
    std::uint32_t 版本 = 1;
    稳定编码 FT{};
    特征长期算法 算法 = 特征长期算法::I64幅度;
    稳定编码 单位{}, 坐标系{};
    std::int64_t 缩放 = 1;
    std::variant<特征I64闭区间, 特征二值图格式> 完整域;
    friend bool operator==(const 特征类型域内容&, const 特征类型域内容&) = default;
};
struct 特征类型化原子域 final {
    特征类型域内容 类型;
    std::variant<特征I64闭区间, 特征二值半径域> 域;
    friend bool operator==(const 特征类型化原子域&, const 特征类型化原子域&) = default;
};
struct 特征长期结构交付 final {
    稳定编码 格式锚点{};
    std::uint32_t 格式 = 1;
    std::array<稳定编码, 18> 类型{};
    friend bool operator==(const 特征长期结构交付&, const 特征长期结构交付&) = default;
};
struct 特征长期读头 final {
    std::uint32_t 版本 = 1;
    std::uint64_t Gread = 0, H = 0;
    friend bool operator==(const 特征长期读头&, const 特征长期读头&) = default;
};
struct 特征长期写头 final {
    std::uint32_t 版本 = 1;
    std::uint64_t 期望G = 0;
    L1所有者范围写入幂等身份 幂等键{};
    friend bool operator==(const 特征长期写头&, const 特征长期写头&) = default;
};
struct 特征长期预算 final {
    std::uint64_t 最大记录数 = 0, 最大关系数 = 0, 最大属性数 = 0, 最大样本数 = 0;
    std::uint64_t 最大像素数 = 0, 最大边界点对数 = 0, 最大历史事实数 = 0;
    friend bool operator==(const 特征长期预算&, const 特征长期预算&) = default;
};
struct 特征观察见证 final {
    std::uint64_t 观察标识 = 0;
    std::int64_t 时间纳秒 = 0;
    std::uint64_t H = 0;
    稳定编码 存在上下文{}, F{}, FT{}, 值事实{}, 成员关系{};
    friend bool operator==(const 特征观察见证&, const 特征观察见证&) = default;
};
struct 特征长期来源事实 final {
    稳定编码 记录身份{}, 关系{}, F{};
    std::uint64_t 创建G = 0;
    std::optional<std::uint64_t> 退出G;
    friend bool operator==(const 特征长期来源事实&, const 特征长期来源事实&) = default;
};
struct 特征长期自有事实 final {
    std::vector<L1所有者范围节点事实> 节点;
    std::vector<L1所有者范围关系事实> 关系;
    std::vector<L1所有者范围值事实> 值;
    friend bool operator==(const 特征长期自有事实&, const 特征长期自有事实&) = default;
};
struct 特征类型域事实 final {
    特征类型域身份 身份;
    特征类型域内容 内容;
    稳定编码 输出FT{};
    std::uint64_t 创建G = 0;
    特征长期自有事实 自有事实;
    friend bool operator==(const 特征类型域事实&, const 特征类型域事实&) = default;
};
struct 特征差异标量 final {
    稳定编码 输出FT{};
    特征类型域身份 类型域身份;
    std::uint32_t 真实阶次 = 2;
    稳定编码 单位{};
    std::int64_t 缩放 = 1;
    std::variant<特征观察见证, 特征区间身份> 左来源;
    特征观察见证 右观察见证;
    std::int64_t 值 = 0;
    friend bool operator==(const 特征差异标量&, const 特征差异标量&) = default;
};
enum class 特征长期状态 : std::uint8_t {
    已读取 = 1, 已发布, 精确重复, 无须变更, 未启用, 入口拒绝, 未找到,
    类型不支持, 预算不足, 历史材料不可用, 事实代次漂移, 幂等冲突,
    引用冲突, 资源失败, 内部不一致, 可能已发布, 差异不可表示 = 17
};
enum class 特征长期发布确定性 : std::uint8_t { 未派发, 已确认未发布, 已确认发布, 未知 };
template<class T> struct 特征长期读取结果 final {
    std::uint32_t 版本 = 1;
    特征长期状态 状态 = 特征长期状态::入口拒绝;
    std::uint64_t Gread = 0, H = 0;
    std::optional<T> 数据;
    bool 成功() const noexcept {
        return 版本 == 1 && 状态 == 特征长期状态::已读取 && Gread && H && H <= Gread && 数据.has_value();
    }
};
struct 特征类型域登记请求 final {
    特征长期写头 写头;
    特征类型域内容 内容;
    特征长期预算 预算;
    friend bool operator==(const 特征类型域登记请求&, const 特征类型域登记请求&) = default;
};
struct 特征类型域读取请求 final {
    特征长期读头 读头;
    std::variant<特征类型域身份, 特征长期类型引用> 选择;
    特征长期预算 预算;
};
struct 特征长期比较请求 final {
    特征长期读头 读头;
    特征类型域身份 类型域;
    特征观察见证 左, 右;
    特征长期预算 预算;
};
struct 特征校正内容 final {
    特征类型域身份 类型域;
    std::uint64_t 参数版本 = 0;
    稳定编码 外设来源{};
    std::int64_t 裕量 = 0;
    std::vector<特征观察见证> 样本;
    bool 静止声明 = false;
    friend bool operator==(const 特征校正内容&, const 特征校正内容&) = default;
};
enum class 特征长期证据状态 : std::uint8_t { 未展开 = 1, 已核验 = 2 };
struct 特征校正事实 final {
    特征校正身份 身份;
    特征校正内容 内容;
    std::int64_t 阈值 = 0;
    std::optional<std::vector<特征差异标量>> 样本对比较;
    特征长期证据状态 证据状态 = 特征长期证据状态::未展开;
    std::uint64_t 创建G = 0;
    特征长期自有事实 自有事实;
    std::vector<特征长期来源事实> 来源组;
    friend bool operator==(const 特征校正事实&, const 特征校正事实&) = default;
};
struct 特征跟踪内容 final {
    特征存在上下文引用 存在上下文;
    特征类型域身份 类型域;
    特征校正身份 校正;
    friend bool operator==(const 特征跟踪内容&, const 特征跟踪内容&) = default;
};
struct 特征跟踪事实 final {
    特征跟踪身份 身份;
    特征跟踪内容 内容;
    std::uint64_t 创建G = 0;
    特征长期自有事实 自有事实;
    friend bool operator==(const 特征跟踪事实&, const 特征跟踪事实&) = default;
};
struct 特征区间事实 final {
    特征区间身份 身份;
    特征跟踪身份 跟踪;
    特征观察见证 首观察;
    std::variant<std::int64_t, 特征二值图> 固定代表;
    std::int64_t 半径 = 0;
    特征类型化原子域 原子域;
    std::uint64_t 创建G = 0;
    特征长期自有事实 自有事实;
    std::vector<特征长期来源事实> 来源组;
    friend bool operator==(const 特征区间事实&, const 特征区间事实&) = default;
};
struct 特征区间命中 final {
    特征区间身份 区间;
    特征差异标量 差异;
    friend bool operator==(const 特征区间命中&, const 特征区间命中&) = default;
};
struct 特征长期观测事实 final {
    特征长期观测身份 身份;
    特征观察见证 见证;
    特征跟踪身份 跟踪;
    std::vector<特征区间命中> 全部命中;
    std::optional<特征区间身份> 本次新建;
    std::optional<std::int64_t> 实际I64值;
    std::uint64_t 创建G = 0;
    特征长期证据状态 证据状态 = 特征长期证据状态::未展开;
    特征长期自有事实 自有事实;
    std::vector<特征长期来源事实> 来源组;
    friend bool operator==(const 特征长期观测事实&, const 特征长期观测事实&) = default;
};
struct 特征完整跟踪投影 final {
    特征跟踪事实 跟踪事实;
    特征类型域事实 类型域事实;
    特征校正事实 校正事实;
    std::vector<特征区间事实> 区间组;
    std::vector<特征长期观测事实> 观察组;
    std::uint64_t 观察总数 = 0;
    std::vector<std::pair<特征区间身份, std::uint64_t>> 各区间命中数;
    std::int64_t 首时间 = 0, 末时间 = 0;
    std::optional<特征I64闭区间> 实际范围;
};
struct 特征校正建立请求 final {
    特征长期写头 写头;
    特征校正内容 内容;
    特征长期预算 预算;
    friend bool operator==(const 特征校正建立请求&, const 特征校正建立请求&) = default;
};
struct 特征校正读取请求 final {
    特征长期读头 读头;
    特征校正身份 校正;
    bool 核验原始样本 = false;
    特征长期预算 预算;
};
struct 特征跟踪建立请求 final {
    特征长期写头 写头;
    特征跟踪内容 内容;
    特征长期预算 预算;
    friend bool operator==(const 特征跟踪建立请求&, const 特征跟踪建立请求&) = default;
};
struct 特征跟踪范围 final {
    特征存在上下文引用 E;
    特征长期类型引用 FT;
    friend bool operator==(const 特征跟踪范围&, const 特征跟踪范围&) = default;
};
struct 特征跟踪读取请求 final {
    特征长期读头 读头;
    std::variant<特征跟踪身份, 特征跟踪范围> 选择;
    特征长期预算 预算;
};
struct 特征长期观察积累请求 final {
    特征长期写头 写头;
    特征跟踪身份 跟踪;
    特征观察见证 观察;
    特征长期预算 预算;
    friend bool operator==(const 特征长期观察积累请求&, const 特征长期观察积累请求&) = default;
};
struct 特征长期区间匹配请求 final {
    特征长期读头 读头;
    特征跟踪身份 跟踪;
    特征观察见证 观察;
    特征长期预算 预算;
};
struct 特征长期区间读取请求 final {
    特征长期读头 读头;
    特征跟踪身份 跟踪;
    特征长期预算 预算;
};
struct 特征长期观察读取请求 final {
    特征长期读头 读头;
    特征跟踪身份 跟踪;
    bool 核验原始证据 = false;
    特征长期预算 预算;
};
struct 特征长期来源释放请求 final {
    特征长期写头 写头;
    std::vector<特征长期来源事实> 预期组;
    特征长期预算 预算;
    friend bool operator==(const 特征长期来源释放请求&, const 特征长期来源释放请求&) = default;
};
using 特征长期原请求 = std::variant<特征类型域登记请求, 特征校正建立请求, 特征跟踪建立请求,
    特征长期观察积累请求, 特征长期来源释放请求>;
template<class T> struct 特征长期写入结果 final {
    std::uint32_t 版本 = 1;
    特征长期状态 状态 = 特征长期状态::入口拒绝;
    std::uint64_t Gread = 0, H = 0;
    std::optional<T> 数据;
    std::optional<特征长期原请求> 原请求;
    std::optional<std::uint64_t> 首次H;
    特征长期发布确定性 发布确定性 = 特征长期发布确定性::未派发;
    bool 成功() const noexcept {
        if (版本 != 1 || !Gread || !H || H > Gread || !数据 || !原请求 || 原请求->valueless_by_exception()) return false;
        if (状态 == 特征长期状态::无须变更)
            return !首次H && 发布确定性 == 特征长期发布确定性::已确认未发布;
        return (状态 == 特征长期状态::已发布 || 状态 == 特征长期状态::精确重复)
            && 首次H && *首次H && *首次H <= Gread && H == *首次H
            && 发布确定性 == 特征长期发布确定性::已确认发布;
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

    特征类数据服务(const L1事实基座服务& l1, const 特征值类数据服务& values,
        L1所有者范围写端口&& port, 特征类派生结构类型 derived, 特征长期结构交付 layout)
        : 特征类数据服务(l1, values, std::move(port), derived) {
        if (!values.绑定于(l1)) throw std::invalid_argument("long term feature value provider mismatch");
        长期校验交付(layout);
    }
    特征类数据服务(const L1事实基座服务& l1, const 特征值类数据服务& values,
        L1所有者范围写端口&& port, 特征类派生结构类型 derived,
        特征类事实集合结构类型 collections, 特征长期结构交付 layout)
        : 特征类数据服务(l1, values, std::move(port), derived, collections) {
        if (!values.绑定于(l1)) throw std::invalid_argument("long term feature value provider mismatch");
        长期校验交付(layout);
    }
    特征长期读取结果<特征类型域事实> 读取特征类型域(const 特征类型域读取请求& r) const {
        return 长期读包装<特征类型域事实>(r.读头, r.预算, [&](auto& c) {
            长期要求(!r.选择.valueless_by_exception(), LS::入口拒绝);
            if (const auto* id = std::get_if<特征类型域身份>(&r.选择)) {
                长期要求(有效(id->值), LS::入口拒绝); return 长期读类型(id->值, c);
            }
            const auto ft = std::get<特征长期类型引用>(r.选择).值;
            长期要求(有效(ft), LS::入口拒绝);
            std::optional<特征类型域事实> found;
            for (const auto& record : 长期目录(c)) if (record.种类 == 1) {
                auto type = 长期读类型(record.节点.编码, c);
                if (type.内容.FT == ft) { 长期要求(!found); found = std::move(type); }
            }
            长期要求(found.has_value(), LS::未找到); return std::move(*found);
        });
    }
    特征长期读取结果<特征差异标量> 比较长期特征(const 特征长期比较请求& r) const {
        return 长期读包装<特征差异标量>(r.读头, r.预算, [&](auto& c) {
            长期要求(有效(r.类型域.值), LS::入口拒绝);
            const auto type = 长期读类型(r.类型域.值, c);
            const auto a = 长期实际(type, r.左, c), b = 长期实际(type, r.右, c);
            return 长期差异事实(type, r.左, r.右, 长期差异(type.内容, a, b, c));
        });
    }
    特征长期写入结果<特征类型域事实> 登记特征类型域(const 特征类型域登记请求& r) {
        return 长期执行<特征类型域事实>(r, [&](auto& c) {
            长期要求(长期类型有效(r.内容), LS::入口拒绝);
            const auto ft = 长期节点(r.内容.FT, c);
            长期要求(ft.种类 == 节点种类::属性类型 && ft.属性类型表示, LS::类型不支持);
            const auto rep = *ft.属性类型表示;
            长期要求(rep == L1所有者范围值表示种类::独立材料引用
                || (r.内容.算法 == 特征长期算法::I64幅度 ? rep == L1所有者范围值表示种类::I64 : rep == L1所有者范围值表示种类::U64组), LS::类型不支持);
            (void)长期节点(r.内容.单位, c, false);
            if (有效(r.内容.坐标系)) (void)长期节点(r.内容.坐标系, c, false);
            for (const auto& record : 长期目录(c)) if (record.种类 == 1) {
                auto old = 长期读类型(record.节点.编码, c);
                if (old.内容.FT == r.内容.FT) {
                    长期要求(old.内容 == r.内容, LS::幂等冲突);
                    return 长期准备<特征类型域事实>{{}, std::move(old)};
                }
            }
            return 长期准备<特征类型域事实>{长期类型写集(r), {}};
        }, [&](auto& c, const auto& saved) {
            auto result = 长期读类型(长期映射键(saved, SK{1}), c);
            长期要求(result.内容 == r.内容, LS::幂等冲突);
            return std::pair{长期类型写集(r), std::move(result)};
        });
    }
    特征长期写入结果<特征校正事实> 建立特征校正(const 特征校正建立请求& r) {
        return 长期执行<特征校正事实>(r, [&](auto& c) {
            const auto input = 长期规范校正(r.内容);
            长期扣量(c.样本数, input.样本.size(), c.限额.最大样本数);
            const auto type = 长期读类型(input.类型域.值, c);
            (void)长期节点(input.外设来源, c, false);
            const auto computed = 长期计算校正(input, type, c);
            std::vector<std::uint64_t> states;
            for (const auto& sample : input.样本) states.push_back(长期源状态(sample, c));
            return 长期准备<特征校正事实>{长期校正写集(r, input, computed.first, states), {}};
        }, [&](auto& c, const auto& saved) {
            auto out = 长期读校正(长期映射键(saved, SK{1}), false, c);
            const auto input = 长期规范校正(r.内容);
            长期要求(out.内容 == input, LS::幂等冲突);
            std::vector<std::uint64_t> states;
            for (std::size_t i = 0; i < input.样本.size(); ++i)
                states.push_back(长期读记录(长期映射键(saved, SK{static_cast<std::uint32_t>(i + 2)}), c).U.back());
            return std::pair{长期校正写集(r, input, out.阈值, states), std::move(out)};
        });
    }
    特征长期读取结果<特征校正事实> 读取特征校正(const 特征校正读取请求& r) const {
        return 长期读包装<特征校正事实>(r.读头, r.预算, [&](auto& c) {
            长期要求(有效(r.校正.值), LS::入口拒绝);
            return 长期读校正(r.校正.值, r.核验原始样本, c);
        });
    }
    特征长期写入结果<特征跟踪事实> 建立特征跟踪(const 特征跟踪建立请求& r) {
        return 长期执行<特征跟踪事实>(r, [&](auto& c) {
            长期要求(有效(r.内容.存在上下文.值) && 有效(r.内容.类型域.值) && 有效(r.内容.校正.值), LS::入口拒绝);
            const auto type = 长期读类型(r.内容.类型域.值, c);
            const auto correction = 长期读校正(r.内容.校正.值, false, c);
            长期要求(correction.内容.类型域 == type.身份, LS::引用冲突);
            for (const auto& item : 长期目录(c)) if (item.种类 == 4) {
                auto old = 长期读跟踪(item.节点.编码, c);
                if (old.内容 == r.内容) return 长期准备<特征跟踪事实>{{}, std::move(old)};
            }
            return 长期准备<特征跟踪事实>{长期跟踪写集(r), {}};
        }, [&](auto& c, const auto& saved) {
            auto out = 长期读跟踪(长期映射键(saved, SK{1}), c);
            长期要求(out.内容 == r.内容, LS::幂等冲突);
            return std::pair{长期跟踪写集(r), std::move(out)};
        });
    }
    特征长期读取结果<std::vector<特征完整跟踪投影>> 读取特征跟踪(const 特征跟踪读取请求& r) const {
        return 长期读包装<std::vector<特征完整跟踪投影>>(r.读头, r.预算, [&](auto& c) {
            长期要求(!r.选择.valueless_by_exception(), LS::入口拒绝);
            std::vector<特征完整跟踪投影> out;
            if (const auto* id = std::get_if<特征跟踪身份>(&r.选择)) {
                长期要求(有效(id->值), LS::入口拒绝);
                out.push_back(长期投影(id->值, false, c));
            } else {
                const auto& range = std::get<特征跟踪范围>(r.选择);
                长期要求(有效(range.E.值) && 有效(range.FT.值), LS::入口拒绝);
                for (const auto& record : 长期目录(c)) if (record.种类 == 4) {
                    const auto track = 长期读跟踪(record.节点.编码, c);
                    const auto type = 长期读类型(track.内容.类型域.值, c);
                    if (track.内容.存在上下文 == range.E && type.内容.FT == range.FT.值)
                        out.push_back(长期投影(track.身份.值, false, c));
                }
            }
            std::sort(out.begin(), out.end(), [](const auto& a, const auto& b) {
                return std::pair{a.跟踪事实.创建G, a.跟踪事实.身份.值} < std::pair{b.跟踪事实.创建G, b.跟踪事实.身份.值};
            });
            return out;
        });
    }
    特征长期写入结果<特征长期观测事实> 积累特征观察(const 特征长期观察积累请求& r) {
        return 长期执行<特征长期观测事实>(r, [&](auto& c) {
            长期要求(有效(r.跟踪.值) && 长期见证有效(r.观察), LS::入口拒绝);
            auto projection = 长期投影(r.跟踪.值, false, c);
            长期要求(r.观察.存在上下文 == projection.跟踪事实.内容.存在上下文.值
                && r.观察.FT == projection.类型域事实.内容.FT, LS::引用冲突);
            if (auto shared = 长期跨跟踪重复(r, projection.跟踪事实, c))
                return 长期准备<特征长期观测事实>{{}, std::move(shared)};
            for (auto& old : projection.观察组) {
                if (old.见证.观察标识 == r.观察.观察标识) {
                    长期要求(old.见证 == r.观察, LS::幂等冲突);
                    return 长期准备<特征长期观测事实>{{}, std::move(old)};
                }
                if (长期同次样本(old.见证, r.观察)) {
                    auto witness = r.观察; witness.观察标识 = old.见证.观察标识;
                    长期要求(witness == old.见证, LS::幂等冲突);
                    return 长期准备<特征长期观测事实>{{}, std::move(old)};
                }
            }
            const auto value = 长期实际(projection.类型域事实, r.观察, c);
            const auto matches = 长期匹配(projection.区间组, projection.类型域事实, r.观察, value, c);
            std::optional<特征区间事实> created;
            if (matches.empty()) {
                created.emplace(); created->固定代表 = value; created->半径 = projection.校正事实.阈值;
            }
            std::optional<std::int64_t> scalar;
            if (const auto* n = std::get_if<std::int64_t>(&value)) scalar = *n;
            return 长期准备<特征长期观测事实>{长期观察写集(r, created, matches, scalar, 长期源状态(r.观察, c)), {}};
        }, [&](auto& c, const auto& saved) {
            长期要求(有效(r.跟踪.值), LS::幂等冲突);
            auto projection = 长期投影(r.跟踪.值, false, c);
            std::optional<特征长期观测事实> out;
            for (auto& item : projection.观察组) if (item.创建G == saved.事实代次 && item.见证 == r.观察) {
                长期要求(!out); out = std::move(item);
            }
            长期要求(out.has_value(), LS::幂等冲突);
            const auto raw = 长期读记录(out->身份.值, c);
            std::optional<特征区间事实> created;
            if (out->本次新建) for (const auto& interval : projection.区间组)
                if (interval.身份 == *out->本次新建) created = interval;
            长期要求(bool(created) == bool(out->本次新建));
            return std::pair{长期观察写集(r, created, out->全部命中, out->实际I64值, raw.U.back()), std::move(*out)};
        });
    }
    特征长期读取结果<std::vector<特征区间命中>> 匹配长期区间(const 特征长期区间匹配请求& r) const {
        return 长期读包装<std::vector<特征区间命中>>(r.读头, r.预算, [&](auto& c) {
            长期要求(有效(r.跟踪.值), LS::入口拒绝);
            const auto p = 长期投影(r.跟踪.值, false, c);
            长期要求(r.观察.存在上下文 == p.跟踪事实.内容.存在上下文.值, LS::引用冲突);
            const auto value = 长期实际(p.类型域事实, r.观察, c);
            return 长期匹配(p.区间组, p.类型域事实, r.观察, value, c);
        });
    }
    特征长期读取结果<std::vector<特征区间事实>> 读取长期区间(const 特征长期区间读取请求& r) const {
        return 长期读包装<std::vector<特征区间事实>>(r.读头, r.预算, [&](auto& c) {
            长期要求(有效(r.跟踪.值), LS::入口拒绝);
            return 长期投影(r.跟踪.值, false, c).区间组;
        });
    }
    特征长期读取结果<std::vector<特征长期观测事实>> 读取长期观察(const 特征长期观察读取请求& r) const {
        return 长期读包装<std::vector<特征长期观测事实>>(r.读头, r.预算, [&](auto& c) {
            长期要求(有效(r.跟踪.值), LS::入口拒绝);
            return 长期投影(r.跟踪.值, r.核验原始证据, c).观察组;
        });
    }
    特征长期写入结果<std::vector<特征长期来源事实>> 释放长期来源引用(const 特征长期来源释放请求& r) {
        auto reconstruct = [&](auto& c, bool replay) {
            std::vector<特征长期来源事实> actual;
            std::set<稳定编码> seen;
            for (const auto& expected : r.预期组) {
                长期要求(有效(expected.记录身份) && 有效(expected.关系) && 有效(expected.F)
                    && expected.创建G && expected.创建G <= r.写头.期望G && !expected.退出G
                    && seen.insert(expected.关系).second, LS::入口拒绝);
                const auto record = 长期读记录(expected.记录身份, c);
                长期要求(record.种类 == 3 || record.种类 == 5 || record.种类 == 6, LS::引用冲突);
                const auto w = 长期解析见证(record.U, record.I[0]);
                const auto source = 长期来源(record, w);
                长期要求(source.size() == 1, LS::引用冲突);
                auto before = source[0];
                if (before.退出G && *before.退出G > r.写头.期望G) before.退出G.reset();
                长期要求(before == expected, replay ? LS::幂等冲突 : LS::引用冲突);
                if (replay) 长期要求(source[0].退出G == c.H);
                else 长期要求(!source[0].退出G || *source[0].退出G > c.H, LS::引用冲突);
                actual.push_back(source[0]);
            }
            std::sort(actual.begin(), actual.end(), [](const auto& a, const auto& b) { return a.关系 < b.关系; });
            return actual;
        };
        auto write = [&](const auto& sources) {
            L1所有者范围写集请求 w;
            w.合同版本 = L1所有者范围CRUD合同版本; w.期望事实代次 = r.写头.期望G; w.写入幂等身份 = r.写头.幂等键;
            for (const auto& e : sources) w.退出事实.push_back(e.关系);
            return w;
        };
        return 长期执行<std::vector<特征长期来源事实>>(r, [&](auto& c) {
            auto sources = reconstruct(c, false);
            if (sources.empty()) return 长期准备<std::vector<特征长期来源事实>>{{}, std::move(sources)};
            return 长期准备<std::vector<特征长期来源事实>>{write(sources), {}};
        }, [&](auto& c, const auto& saved) {
            auto sources = reconstruct(c, true);
            return std::pair{write(sources), std::move(sources)};
        });
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
        if (有效(派生结构类型_.输出类型关系类型)
            && (std::find(类型组.begin(), 类型组.end(), 派生结构类型_.输出类型关系类型) != 类型组.end()
                || !类型结点有效(派生结构类型_.输出类型关系类型, 节点种类::普通, std::nullopt)))
            throw std::invalid_argument("invalid scalar output relation type");
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
        static_assert(std::is_nothrow_move_constructible_v<特征类结点结果>);
        try {
            if (auto 重放 = 尝试重放新特征(请求.期望事实代次,
                    请求.幂等身份, 请求.特征类型, 请求.特征值,
                    std::nullopt, 特征类数据状态::已创建))
                return std::move(*重放);
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
        static_assert(std::is_nothrow_move_constructible_v<特征类结点结果>);
        try {
            if (auto 重放 = 尝试重放新特征(请求.期望事实代次,
                    请求.幂等身份, 请求.新特征类型, 请求.新特征值,
                    请求.原特征结点, 特征类数据状态::已换代))
                return std::move(*重放);
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
            return 派生失败(特征类派生数据状态::不支持, 请求.期望事实代次);
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
    using LS = 特征长期状态;
    struct 长期失败 { LS 状态; };
    static void 长期要求(bool ok, LS s = LS::内部不一致) {
        if (!ok) throw 长期失败{s};
    }
    using LN = L1所有者范围节点事实;
    using LV = L1所有者范围值事实;
    using LE = L1所有者范围关系事实;
    struct 长期记录 {
        LN 节点;
        std::int64_t 种类 = 0;
        std::vector<std::uint64_t> U;
        std::vector<std::int64_t> I;
        std::array<std::vector<LE>, 18> 边;
        特征长期自有事实 事实;
    };
    struct 长期上下文 {
        std::uint64_t G = 0, H = 0;
        特征长期预算 限额;
        std::uint64_t 记录数 = 0, 关系数 = 0, 属性数 = 0, 样本数 = 0;
        std::uint64_t 像素数 = 0, 点对数 = 0, 历史数 = 0;
        std::map<std::pair<稳定编码, std::uint64_t>, 长期记录> 已读记录;
    };
    static void 长期扣量(std::uint64_t& used, std::uint64_t amount, std::uint64_t limit) {
        长期要求(limit && used <= limit && amount <= limit - used, LS::预算不足);
        used += amount;
    }
    static bool 长期格式有效(const 特征二值图格式& f) noexcept {
        return f.宽 && f.高 && 有效(f.坐标系) && f.轴约定 == 1 && f.分辨率分子 && f.分辨率分母
            && std::gcd(f.分辨率分子, f.分辨率分母) == 1;
    }
    static bool 长期类型有效(const 特征类型域内容& t) noexcept {
        if (t.版本 != 1 || !有效(t.FT) || !有效(t.单位) || t.缩放 <= 0 || t.完整域.valueless_by_exception()) return false;
        if (t.算法 == 特征长期算法::I64幅度) {
            const auto* d = std::get_if<特征I64闭区间>(&t.完整域);
            return d && d->下界 <= d->上界 && t.缩放 == 1 && !有效(t.坐标系);
        }
        if (t.算法 != 特征长期算法::二值边界平均) return false;
        const auto* d = std::get_if<特征二值图格式>(&t.完整域);
        return d && 长期格式有效(*d) && d->坐标系 == t.坐标系;
    }
    static std::int64_t 长期I64位模式(std::uint64_t x) noexcept {
        return std::bit_cast<std::int64_t>(x);
    }
    static 特征二值图 长期解析二值(const std::vector<std::uint64_t>& u, 长期上下文& c) {
        长期要求(u.size() >= 10 && u[0] == 0x42494E4152590001ULL, LS::类型不支持);
        特征二值图 out;
        out.格式 = {u[1], u[2], 稳定编码{u[3]}, u[4], u[5], u[6], 长期I64位模式(u[7]), 长期I64位模式(u[8])};
        长期要求(长期格式有效(out.格式), LS::类型不支持);
        长期要求(u[1] <= UINT64_MAX / u[2] && u[1] * u[2] == u.size() - 9, LS::类型不支持);
        长期扣量(c.像素数, u.size() - 9, c.限额.最大像素数);
        bool foreground = false;
        for (std::size_t i = 9; i < u.size(); ++i) {
            长期要求(u[i] <= 1, LS::类型不支持);
            foreground |= u[i] == 1;
        }
        长期要求(foreground, LS::类型不支持);
        out.像素.assign(u.begin() + 9, u.end());
        return out;
    }
    static std::vector<std::uint64_t> 长期图载荷(const 特征二值图& x) {
        const auto& f = x.格式;
        std::vector<std::uint64_t> out{0x42494E4152590001ULL, f.宽, f.高, f.坐标系.值, f.轴约定,
            f.分辨率分子, f.分辨率分母, std::bit_cast<std::uint64_t>(f.原点X), std::bit_cast<std::uint64_t>(f.原点Y)};
        out.insert(out.end(), x.像素.begin(), x.像素.end());
        return out;
    }
    static bool 长期见证有效(const 特征观察见证& w) noexcept {
        return w.观察标识 && w.时间纳秒 > 0 && w.H && 有效(w.存在上下文) && 有效(w.F)
            && 有效(w.FT) && 有效(w.值事实) && 有效(w.成员关系);
    }
    static bool 长期同次样本(const 特征观察见证& a, const 特征观察见证& b) noexcept {
        return a.存在上下文 == b.存在上下文 && a.FT == b.FT && a.F == b.F && a.值事实 == b.值事实
            && a.H == b.H && a.时间纳秒 == b.时间纳秒;
    }
    static std::vector<std::uint64_t> 长期见证U64(const 特征观察见证& w) {
        return {w.观察标识, w.H, w.存在上下文.值, w.F.值, w.FT.值, w.值事实.值, w.成员关系.值};
    }
    static 特征观察见证 长期解析见证(const std::vector<std::uint64_t>& u, std::int64_t time) {
        长期要求(u.size() >= 7);
        特征观察见证 w{u[0], time, u[1], 稳定编码{u[2]}, 稳定编码{u[3]}, 稳定编码{u[4]}, 稳定编码{u[5]}, 稳定编码{u[6]}};
        长期要求(长期见证有效(w));
        return w;
    }
    std::uint64_t 长期当前() const {
        const auto r = 第一层服务_.读取中性当前事实代次({L1中性CRUD合同版本});
        长期要求(r.状态 == L1中性读取状态::成功,
            r.状态 == L1中性读取状态::资源失败 ? LS::资源失败 : LS::内部不一致);
        长期要求(r.合同版本 == L1中性CRUD合同版本 && r.事实代次);
        return r.事实代次;
    }
    void 长期守卫(std::uint64_t g) const { 长期要求(长期当前() == g, LS::事实代次漂移); }
    static LS 长期映射(L1所有者范围读取状态 s) noexcept {
        switch (s) {
        case L1所有者范围读取状态::未找到: return LS::未找到;
        case L1所有者范围读取状态::历史材料已清理: return LS::历史材料不可用;
        case L1所有者范围读取状态::资源失败: return LS::资源失败;
        default: return LS::内部不一致;
        }
    }
    static LS 长期映射(特征引用读取状态 s) noexcept {
        switch (s) {
        case 特征引用读取状态::数量预算不足: return LS::预算不足;
        case 特征引用读取状态::资源失败: return LS::资源失败;
        case 特征引用读取状态::事实代次漂移: return LS::事实代次漂移;
        case 特征引用读取状态::历史材料不可用: return LS::历史材料不可用;
        case 特征引用读取状态::未找到: return LS::未找到;
        case 特征引用读取状态::内部不一致: return LS::内部不一致;
        default: return LS::引用冲突;
        }
    }
    长期上下文 长期开始(const 特征长期读头& h, const 特征长期预算& b) const {
        长期要求(长期交付_.has_value(), LS::未启用);
        长期要求(h.版本 == 1 && h.Gread && h.H <= h.Gread, LS::入口拒绝);
        长期守卫(h.Gread);
        return {h.Gread, h.H ? h.H : h.Gread, b};
    }
    template<class T> static bool 长期活动(const T& f, std::uint64_t h) noexcept {
        return f.创建事实代次 && f.创建事实代次 <= h && (!f.退出事实代次 || *f.退出事实代次 > h);
    }
    LN 长期节点(稳定编码 id, 长期上下文& c, bool own = true, bool active = true) const {
        长期扣量(c.历史数, 1, c.限额.最大历史事实数);
        auto r = 第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, id});
        if (r.状态 != L1所有者范围读取状态::成功) {
            长期守卫(c.G);
            throw 长期失败{长期映射(r.状态)};
        }
        长期要求(r.读取事实代次 == c.G, LS::事实代次漂移);
        const auto* n = r.事实 ? std::get_if<LN>(&*r.事实) : nullptr;
        长期要求(r.合同版本 == L1所有者范围CRUD合同版本 && r.查询编码 == id && n && n->编码 == id
            && n->创建事实代次 && (!own || n->写入所有者 == 所有者_));
        长期要求(n->创建事实代次 <= c.H && (!active || 长期活动(*n, c.H)), LS::未找到);
        return *n;
    }
    std::vector<LV> 长期属性(const LN& n, 长期上下文& c) const {
        auto r = 第一层服务_.读取所有者范围历史属性值组({L1所有者范围CRUD合同版本, n.编码, c.H});
        if (r.状态 != L1所有者范围读取状态::成功) {
            长期守卫(c.G); throw 长期失败{长期映射(r.状态)};
        }
        长期要求(r.读取事实代次 == c.G, LS::事实代次漂移);
        长期要求(r.合同版本 == L1所有者范围CRUD合同版本 && r.所属节点 == n.编码 && r.历史截止事实代次 == c.H);
        长期扣量(c.属性数, r.属性值组.size(), c.限额.最大属性数);
        长期扣量(c.历史数, r.属性值组.size(), c.限额.最大历史事实数);
        for (const auto& v : r.属性值组)
            长期要求(有效(v.编码) && v.所属节点 == n.编码 && v.写入所有者 == 所有者_
                && v.来源节点 == n.编码 && 长期活动(v, c.H)
                && v.创建事实代次 == n.创建事实代次 && v.退出事实代次 == n.退出事实代次);
        return r.属性值组;
    }
    std::vector<LE> 长期关系(稳定编码 id, unsigned role, bool incoming, 长期上下文& c,
        std::optional<std::uint64_t> at = {}) const {
        长期要求(role >= 1 && role <= 18);
        const auto h = at.value_or(c.H);
        const auto direction = incoming ? L1所有者范围关系端点方向::目标 : L1所有者范围关系端点方向::源;
        auto r = 第一层服务_.读取所有者范围历史关系组(
            {L1所有者范围CRUD合同版本, direction, id, 长期交付_->类型[role - 1], h});
        if (r.状态 != L1所有者范围读取状态::成功) {
            长期守卫(c.G); throw 长期失败{长期映射(r.状态)};
        }
        长期要求(r.读取事实代次 == c.G, LS::事实代次漂移);
        长期要求(r.合同版本 == L1所有者范围CRUD合同版本 && r.方向 == direction
            && r.端点节点 == id && r.关系类型节点 == 长期交付_->类型[role - 1] && r.历史截止事实代次 == h);
        长期扣量(c.关系数, r.关系组.size(), c.限额.最大关系数);
        长期扣量(c.历史数, r.关系组.size(), c.限额.最大历史事实数);
        for (const auto& e : r.关系组)
            长期要求(有效(e.编码) && e.写入所有者 == 所有者_ && e.关系类型节点 == 长期交付_->类型[role - 1]
                && (incoming ? e.目标节点 : e.源节点) == id && 长期活动(e, h));
        std::sort(r.关系组.begin(), r.关系组.end(), [](const auto& a, const auto& b) {
            return std::pair{a.角色或顺序, a.编码} < std::pair{b.角色或顺序, b.编码};
        });
        return r.关系组;
    }
    void 长期校验交付(const 特征长期结构交付& d) {
        长期要求(d.格式 == 1 && 有效(d.格式锚点), LS::入口拒绝);
        长期交付_ = d;
        特征长期预算 b;
        b.最大记录数 = b.最大关系数 = b.最大属性数 = b.最大历史事实数 = 1000;
        auto c = 长期开始({1, 长期当前(), 0}, b);
        const auto a = 长期节点(d.格式锚点, c);
        长期要求(a.种类 == 节点种类::普通 && !a.属性类型表示);
        std::set<稳定编码> ids;
        for (unsigned i = 0; i < 18; ++i) {
            长期要求(有效(d.类型[i]) && d.类型[i] != d.格式锚点 && ids.insert(d.类型[i]).second);
            const auto n = 长期节点(d.类型[i], c);
            const auto rep = i == 1 || i == 2 ? std::optional{L1所有者范围值表示种类::I64}
                : i == 3 ? std::optional{L1所有者范围值表示种类::U64组}
                : i == 4 ? std::optional{L1所有者范围值表示种类::I64组} : std::nullopt;
            长期要求(n.种类 == (rep ? 节点种类::属性类型 : 节点种类::普通) && n.属性类型表示 == rep);
        }
        const auto values = 长期属性(a, c);
        长期要求(values.size() == 1 && values[0].属性类型节点 == d.类型[2]
            && std::get_if<std::int64_t>(&values[0].材料) && std::get<std::int64_t>(values[0].材料) == 1);
        const auto edges = 长期关系(a.编码, 18, false, c);
        长期要求(edges.size() == 18);
        for (unsigned i = 0; i < 18; ++i)
            长期要求(edges[i].角色或顺序 == i + 1 && edges[i].目标节点 == d.类型[i] && !edges[i].退出事实代次);
        长期守卫(c.G);
    }
    长期记录 长期读记录(稳定编码 id, 长期上下文& c) const {
        const auto cached = c.已读记录.find({id, c.H});
        if (cached != c.已读记录.end()) return cached->second;
        长期扣量(c.记录数, 1, c.限额.最大记录数);
        长期记录 r;
        r.节点 = 长期节点(id, c);
        长期要求(r.节点.种类 == 节点种类::普通 && !r.节点.属性类型表示 && !r.节点.退出事实代次);
        r.事实.节点.push_back(r.节点);
        r.事实.值 = 长期属性(r.节点, c);
        长期要求(r.事实.值.size() == 4);
        auto value = [&](unsigned role) -> const L1所有者范围原始值材料& {
            const LV* found = nullptr;
            for (const auto& v : r.事实.值) if (v.属性类型节点 == 长期交付_->类型[role - 1]) {
                长期要求(!found); found = &v;
            }
            长期要求(found != nullptr); return found->材料;
        };
        const auto* kind = std::get_if<std::int64_t>(&value(2));
        const auto* ver = std::get_if<std::int64_t>(&value(3));
        const auto* u = std::get_if<std::vector<std::uint64_t>>(&value(4));
        const auto* i = std::get_if<std::vector<std::int64_t>>(&value(5));
        长期要求(kind && *kind >= 1 && *kind <= 6 && ver && *ver == 1 && u && !u->empty() && i && !i->empty());
        r.种类 = *kind; r.U = *u; r.I = *i;
        r.边[0] = 长期关系(id, 1, true, c);
        长期要求(r.边[0].size() == 1 && r.边[0][0].源节点 == 长期交付_->格式锚点
            && r.边[0][0].角色或顺序 == 1 && r.边[0][0].创建事实代次 == r.节点.创建事实代次
            && !r.边[0][0].退出事实代次);
        r.事实.关系 = r.边[0];
        for (unsigned role = 6; role <= 17; ++role) {
            r.边[role - 1] = 长期关系(id, role, false, c, role == 16 ? std::optional{r.节点.创建事实代次} : std::nullopt);
            std::int64_t order = 0;
            for (const auto& e : r.边[role - 1]) {
                长期要求(e.创建事实代次 == r.节点.创建事实代次 && (role == 16 || !e.退出事实代次)
                    && e.角色或顺序 == ++order);
                r.事实.关系.push_back(e);
            }
        }
        c.已读记录.emplace(std::pair{id, c.H}, r);
        return r;
    }
    static 稳定编码 长期单边(const 长期记录& r, unsigned role) {
        长期要求(r.边[role - 1].size() == 1);
        return r.边[role - 1][0].目标节点;
    }
    static void 长期角色(const 长期记录& r, std::initializer_list<unsigned> allowed) {
        for (unsigned role = 6; role <= 17; ++role)
            if (std::find(allowed.begin(), allowed.end(), role) == allowed.end())
                长期要求(r.边[role - 1].empty());
    }
    std::vector<长期记录> 长期目录(长期上下文& c) const {
        const auto edges = 长期关系(长期交付_->格式锚点, 1, false, c);
        std::vector<长期记录> out;
        std::set<稳定编码> ids;
        for (const auto& e : edges) {
            长期要求(e.角色或顺序 == 1 && !e.退出事实代次 && ids.insert(e.目标节点).second);
            auto r = 长期读记录(e.目标节点, c);
            长期要求(r.边[0][0] == e);
            out.push_back(std::move(r));
        }
        return out;
    }
    // 有界128位整数；每个中间量均精确，超过本实现宽度显式拒绝。
    struct 长期宽整数 {
        std::array<std::uint32_t, 4> w{};
        长期宽整数() = default;
        explicit 长期宽整数(std::uint64_t x) : w{static_cast<std::uint32_t>(x), static_cast<std::uint32_t>(x >> 32), 0, 0} {}
        friend bool operator==(const 长期宽整数&, const 长期宽整数&) = default;
        bool operator<(const 长期宽整数& b) const noexcept {
            for (int i = 3; i >= 0; --i) if (w[i] != b.w[i]) return w[i] < b.w[i];
            return false;
        }
        bool bit(unsigned i) const noexcept { return (w[i / 32] >> (i % 32)) & 1U; }
        void set(unsigned i) noexcept { w[i / 32] |= 1U << (i % 32); }
        bool shift() noexcept {
            bool carry = w[3] >> 31;
            for (int i = 3; i >= 0; --i) w[i] = (w[i] << 1) | (i ? w[i - 1] >> 31 : 0);
            return carry;
        }
        void subtract(const 长期宽整数& b) noexcept {
            std::uint64_t borrow = 0;
            for (unsigned i = 0; i < 4; ++i) {
                const auto rhs = static_cast<std::uint64_t>(b.w[i]) + borrow;
                const auto lhs = static_cast<std::uint64_t>(w[i]);
                w[i] = static_cast<std::uint32_t>(lhs - rhs);
                borrow = lhs < rhs;
            }
        }
        std::uint64_t u64() const {
            长期要求(!w[2] && !w[3], LS::差异不可表示);
            return w[0] | (static_cast<std::uint64_t>(w[1]) << 32);
        }
    };
    static 长期宽整数 长期加(长期宽整数 a, const 长期宽整数& b) {
        std::uint64_t carry = 0;
        for (unsigned i = 0; i < 4; ++i) {
            const auto x = static_cast<std::uint64_t>(a.w[i]) + b.w[i] + carry;
            a.w[i] = static_cast<std::uint32_t>(x); carry = x >> 32;
        }
        长期要求(!carry, LS::差异不可表示); return a;
    }
    static 长期宽整数 长期乘(const 长期宽整数& a, const 长期宽整数& b) {
        std::array<std::uint32_t, 8> v{};
        for (unsigned i = 0; i < 4; ++i) {
            std::uint64_t carry = 0;
            for (unsigned j = 0; j < 4; ++j) {
                const auto x = static_cast<std::uint64_t>(a.w[i]) * b.w[j] + v[i + j] + carry;
                v[i + j] = static_cast<std::uint32_t>(x); carry = x >> 32;
            }
            v[i + 4] = static_cast<std::uint32_t>(carry);
        }
        长期要求(!v[4] && !v[5] && !v[6] && !v[7], LS::差异不可表示);
        长期宽整数 out; std::copy_n(v.begin(), 4, out.w.begin()); return out;
    }
    static 长期宽整数 长期除(const 长期宽整数& a, const 长期宽整数& b) {
        长期要求(b != 长期宽整数{});
        长期宽整数 q, rem;
        for (int i = 127; i >= 0; --i) {
            const bool carry = rem.shift();
            if (a.bit(static_cast<unsigned>(i))) rem.w[0] |= 1;
            if (carry || !(rem < b)) { rem.subtract(b); q.set(static_cast<unsigned>(i)); }
        }
        return q;
    }
    static std::uint64_t 长期开方(const 长期宽整数& n) {
        std::uint64_t lo = 0, hi = UINT64_MAX;
        while (lo != hi) {
            const auto mid = lo + (hi - lo) / 2 + (hi - lo) % 2;
            const auto square = 长期乘(长期宽整数{mid}, 长期宽整数{mid});
            if (n < square) hi = mid - 1; else lo = mid;
        }
        return lo;
    }
    using 长期实际值 = std::variant<std::int64_t, 特征二值图>;
    static std::int64_t 长期差异(const 特征类型域内容& t, const 长期实际值& a,
        const 长期实际值& b, 长期上下文& c) {
        长期要求(a.index() == b.index());
        if (const auto* x = std::get_if<std::int64_t>(&a)) {
            const auto y = std::get<std::int64_t>(b);
            const auto distance = *x >= y ? static_cast<std::uint64_t>(*x) - static_cast<std::uint64_t>(y)
                : static_cast<std::uint64_t>(y) - static_cast<std::uint64_t>(*x);
            长期要求(distance <= INT64_MAX, LS::差异不可表示);
            return static_cast<std::int64_t>(distance);
        }
        const auto& x = std::get<特征二值图>(a);
        const auto& y = std::get<特征二值图>(b);
        长期要求(x.格式 == y.格式 && x.格式 == std::get<特征二值图格式>(t.完整域), LS::类型不支持);
        if (x == y) return 0;
        using Point = std::pair<std::uint64_t, std::uint64_t>;
        auto boundary = [](const 特征二值图& image) {
            std::vector<Point> out;
            const auto w = image.格式.宽, h = image.格式.高;
            for (std::uint64_t i = 0; i < image.像素.size(); ++i) if (image.像素[i]) {
                const auto px = i % w, py = i / w;
                if (!px || px + 1 == w || !py || py + 1 == h || !image.像素[i - 1]
                    || !image.像素[i + 1] || !image.像素[i - w] || !image.像素[i + w]) out.emplace_back(px, py);
            }
            return out;
        };
        const auto A = boundary(x), B = boundary(y);
        长期要求(!A.empty() && !B.empty());
        const auto scale2 = 长期乘(长期宽整数{static_cast<std::uint64_t>(t.缩放)}, 长期宽整数{static_cast<std::uint64_t>(t.缩放)});
        auto directed = [&](const auto& first, const auto& second) {
            长期宽整数 sum;
            for (const auto& p : first) {
                std::optional<长期宽整数> nearest;
                for (const auto& q : second) {
                    长期扣量(c.点对数, 1, c.限额.最大边界点对数);
                    const auto dx = p.first > q.first ? p.first - q.first : q.first - p.first;
                    const auto dy = p.second > q.second ? p.second - q.second : q.second - p.second;
                    const auto d2 = 长期加(长期乘(长期宽整数{dx}, 长期宽整数{dx}), 长期乘(长期宽整数{dy}, 长期宽整数{dy}));
                    if (!nearest || d2 < *nearest) nearest = d2;
                }
                sum = 长期加(sum, 长期宽整数{长期开方(长期乘(*nearest, scale2))});
            }
            return sum;
        };
        const auto sa = directed(A, B), sb = directed(B, A);
        const auto numerator = 长期加(长期乘(sa, 长期宽整数{B.size()}), 长期乘(sb, 长期宽整数{A.size()}));
        const auto denominator = 长期乘(长期宽整数{2}, 长期乘(长期宽整数{A.size()}, 长期宽整数{B.size()}));
        const auto result = 长期除(numerator, denominator).u64();
        长期要求(result <= INT64_MAX, LS::差异不可表示);
        return static_cast<std::int64_t>(result);
    }
    特征类型域事实 长期读类型(稳定编码 id, 长期上下文& c) const {
        auto r = 长期读记录(id, c);
        长期要求(r.种类 == 1 && r.U.size() == 6 && r.I.size() == 5);
        长期角色(r, {6, 7, 8, 17});
        特征类型域事实 out;
        out.身份 = 特征类型域身份{id}; out.创建G = r.节点.创建事实代次;
        auto& t = out.内容;
        t.FT = 长期单边(r, 6); t.单位 = 长期单边(r, 8); t.缩放 = r.I[0];
        长期要求(r.U[0] == 1 || r.U[0] == 2);
        t.算法 = static_cast<特征长期算法>(r.U[0]);
        if (r.U[0] == 1) {
            长期要求(std::all_of(r.U.begin() + 1, r.U.end(), [](auto x) { return x == 0; })
                && !r.I[3] && !r.I[4] && r.边[6].empty());
            t.完整域 = 特征I64闭区间{r.I[1], r.I[2]};
        } else {
            t.坐标系 = 长期单边(r, 7);
            长期要求(!r.I[1] && !r.I[2]);
            t.完整域 = 特征二值图格式{r.U[1], r.U[2], t.坐标系, r.U[3], r.U[4], r.U[5], r.I[3], r.I[4]};
        }
        长期要求(长期类型有效(t));
        const auto originalH = c.H;
        c.H = out.创建G;
        const auto ft = 长期节点(t.FT, c);
        长期要求(ft.种类 == 节点种类::属性类型 && ft.属性类型表示);
        const auto rep = *ft.属性类型表示;
        长期要求(rep == L1所有者范围值表示种类::独立材料引用
            || (t.算法 == 特征长期算法::I64幅度 ? rep == L1所有者范围值表示种类::I64 : rep == L1所有者范围值表示种类::U64组));
        (void)长期节点(t.单位, c, false);
        if (有效(t.坐标系)) (void)长期节点(t.坐标系, c, false);
        out.输出FT = 长期单边(r, 17);
        const auto output = 长期节点(out.输出FT, c);
        长期要求(output.种类 == 节点种类::属性类型 && output.属性类型表示 == L1所有者范围值表示种类::I64
            && output.创建事实代次 == out.创建G && !output.退出事实代次);
        const auto values = 长期属性(output, c);
        长期要求(values.size() == 1 && values[0].属性类型节点 == output.编码
            && std::get_if<std::int64_t>(&values[0].材料)
            && std::get<std::int64_t>(values[0].材料) == 0x4C54444946460001LL);
        const auto reverse = 长期关系(output.编码, 17, true, c);
        长期要求(reverse.size() == 1 && reverse[0] == r.边[16][0]);
        r.事实.节点.push_back(output); r.事实.值.push_back(values[0]);
        out.自有事实 = std::move(r.事实);
        c.H = originalH;
        return out;
    }
    长期实际值 长期实际(const 特征类型域事实& type, const 特征观察见证& w, 长期上下文& c) const {
        长期要求(长期见证有效(w) && w.H <= c.H && w.FT == type.内容.FT, LS::引用冲突);
        长期扣量(c.样本数, 1, c.限额.最大样本数);
        长期扣量(c.历史数, 3, c.限额.最大历史事实数);
        长期要求(c.属性数 < c.限额.最大属性数, LS::预算不足);
        auto whole = 按实例读取特征历史事实({1, c.G, w.H, w.F, c.限额.最大属性数 - c.属性数});
        if (!whole.成功()) { 长期守卫(c.G); throw 长期失败{长期映射(whole.状态)}; }
        长期扣量(c.属性数, 1, c.限额.最大属性数);
        长期扣量(c.历史数, 3, c.限额.最大历史事实数);
        长期要求(whole.Gread == c.G && whole.H == w.H && whole.特征 && whole.特征->结点 == w.F
            && whole.特征->特征类型 == w.FT && whole.特征->值事实 == w.值事实, LS::引用冲突);
        auto f = 读取特征历史事实({1, c.G, w.H, w.F, w.值事实});
        if (!f.成功()) { 长期守卫(c.G); throw 长期失败{长期映射(f.状态)}; }
        长期要求(f.Gread == c.G && f.H == w.H && f.特征 && f.特征->特征类型 == w.FT);
        // 派生输出FT不能借普通历史形状伪装为第一阶来源。
        auto oldH = c.H; c.H = w.H;
        const auto ft = 长期节点(w.FT, c);
        长期要求(std::none_of(ft.当前属性.begin(), ft.当前属性.end(), [&](const auto& x) { return x.属性类型节点 == ft.编码; }), LS::类型不支持);
        c.H = oldH;
        L1所有者范围原始值材料 raw = 转换为L1材料(f.特征->特征值);
        if (const auto* ref = std::get_if<L1所有者范围独立材料引用>(&raw)) {
            长期扣量(c.历史数, 4, c.限额.最大历史事实数);
            长期要求(c.属性数 < c.限额.最大属性数, LS::预算不足);
            auto material = 特征值服务_.按结点读取不可变材料历史事实({1, c.G, w.H, ref->编码, c.限额.最大属性数 - c.属性数});
            if (!material.成功()) { 长期守卫(c.G); throw 长期失败{长期映射(material.状态)}; }
            长期要求(material.Gread == c.G && material.H == w.H && material.材料 && material.材料->结点 == ref->编码);
            长期扣量(c.属性数, 1, c.限额.最大属性数);
            raw = material.材料->特征值;
        }
        if (type.内容.算法 == 特征长期算法::I64幅度) {
            const auto* v = std::get_if<std::int64_t>(&raw);
            长期要求(v != nullptr, LS::类型不支持);
            const auto domain = std::get<特征I64闭区间>(type.内容.完整域);
            长期要求(*v >= domain.下界 && *v <= domain.上界, LS::类型不支持);
            return *v;
        }
        const auto* v = std::get_if<std::vector<std::uint64_t>>(&raw);
        长期要求(v != nullptr, LS::类型不支持);
        auto image = 长期解析二值(*v, c);
        长期要求(image.格式 == std::get<特征二值图格式>(type.内容.完整域), LS::类型不支持);
        return image;
    }
    static 特征差异标量 长期差异事实(const 特征类型域事实& t,
        std::variant<特征观察见证, 特征区间身份> left, const 特征观察见证& right, std::int64_t value) {
        return {t.输出FT, t.身份, 2, t.内容.单位, t.内容.缩放, std::move(left), right, value};
    }
    template<class T, class F> 特征长期读取结果<T> 长期读包装(
        const 特征长期读头& h, const 特征长期预算& budget, F&& action) const {
        特征长期读取结果<T> out;
        out.Gread = h.Gread; out.H = h.H ? h.H : h.Gread;
        try {
            auto c = 长期开始(h, budget);
            auto data = action(c);
            长期守卫(c.G);
            out.数据 = std::move(data); out.状态 = LS::已读取;
        } catch (const 长期失败& e) { out.状态 = e.状态; }
        catch (const std::bad_alloc&) { out.状态 = LS::资源失败; }
        catch (const std::length_error&) { out.状态 = LS::资源失败; }
        catch (...) { out.状态 = LS::内部不一致; }
        if (out.状态 != LS::已读取) out.数据.reset();
        return out;
    }
    struct 长期草稿 {
        std::int64_t 种类 = 0;
        std::vector<std::uint64_t> U;
        std::vector<std::int64_t> I;
        struct Edge {
            unsigned 角色 = 0;
            L1所有者范围事实引用 目标;
            std::int64_t 顺序 = 1;
        };
        std::vector<Edge> 边;
    };
    L1所有者范围写集请求 长期写集(const 特征长期写头& h, const std::vector<长期草稿>& records,
        bool output = false) const {
        L1所有者范围写集请求 w;
        w.合同版本 = L1所有者范围CRUD合同版本; w.期望事实代次 = h.期望G; w.写入幂等身份 = h.幂等键;
        长期要求(!records.empty() && records.size() + output <= 0xFFFF, LS::预算不足);
        std::uint32_t edgeKey = 0x10000, valueKey = 0x20000;
        for (std::size_t i = 0; i < records.size(); ++i) {
            const SK key{static_cast<std::uint32_t>(i + 1)};
            const auto& r = records[i];
            长期要求(r.种类 >= 1 && r.种类 <= 6 && !r.U.empty() && !r.I.empty());
            w.节点.push_back({key, 节点种类::普通, std::nullopt});
            w.关系.push_back({SK{edgeKey++}, 长期交付_->格式锚点, key, 长期交付_->类型[0], 1});
            auto edges = r.边;
            std::sort(edges.begin(), edges.end(), [](const auto& a, const auto& b) {
                return std::pair{a.角色, a.顺序} < std::pair{b.角色, b.顺序};
            });
            for (const auto& e : edges) {
                长期要求(e.角色 >= 6 && e.角色 <= 17 && edgeKey < 0x1FFFF, LS::预算不足);
                w.关系.push_back({SK{edgeKey++}, key, e.目标, 长期交付_->类型[e.角色 - 1], e.顺序});
            }
            std::array<L1所有者范围原始值材料, 4> payload{r.种类, std::int64_t{1}, r.U, r.I};
            for (unsigned j = 0; j < 4; ++j) {
                长期要求(valueKey < 0x2FFFF, LS::预算不足);
                const SK vk{valueKey++};
                w.值.push_back({vk, key, 长期交付_->类型[j + 1], std::move(payload[j]), key});
                w.属性槽变更.push_back({key, 长期交付_->类型[j + 1], vk});
            }
        }
        if (output) {
            const SK key{static_cast<std::uint32_t>(records.size() + 1)}, vk{valueKey++};
            w.节点.push_back({key, 节点种类::属性类型, L1所有者范围值表示种类::I64});
            w.值.push_back({vk, key, key, std::int64_t{0x4C54444946460001LL}, key});
            w.属性槽变更.push_back({key, key, vk});
        }
        标量排序写集(w);
        return w;
    }
    L1所有者范围写集请求 长期类型写集(const 特征类型域登记请求& r) const {
        长期草稿 d;
        d.种类 = 1;
        d.U = {static_cast<std::uint64_t>(r.内容.算法), 0, 0, 0, 0, 0};
        d.I = {r.内容.缩放, 0, 0, 0, 0};
        d.边 = {{6, r.内容.FT}, {8, r.内容.单位}, {17, SK{2}}};
        if (const auto* interval = std::get_if<特征I64闭区间>(&r.内容.完整域)) {
            d.I[1] = interval->下界; d.I[2] = interval->上界;
        } else {
            const auto& f = std::get<特征二值图格式>(r.内容.完整域);
            d.U = {2, f.宽, f.高, f.轴约定, f.分辨率分子, f.分辨率分母};
            d.I[3] = f.原点X; d.I[4] = f.原点Y;
            d.边.push_back({7, r.内容.坐标系});
        }
        return 长期写集(r.写头, {d}, true);
    }
    static 稳定编码 长期映射键(const L1所有者范围写入结果& saved, L1所有者范围写集本地键 key) {
        const auto id = 特征类数据内部::查找唯一编码(saved, key);
        长期要求(id.has_value()); return *id;
    }
    static 稳定编码 长期引用解码(const L1所有者范围写入结果& saved, const L1所有者范围事实引用& ref) {
        if (const auto* id = std::get_if<稳定编码>(&ref)) return *id;
        return 长期映射键(saved, std::get<SK>(ref));
    }
    void 长期核验写后(const L1所有者范围写入结果& saved, const L1所有者范围写集请求& w, 长期上下文& c) const {
        长期要求(写入结果头完整(saved, w.写入幂等身份) && saved.新编码映射.size() == w.节点.size() + w.关系.size() + w.值.size());
        auto resolve = [&](const auto& ref) { return 长期引用解码(saved, ref); };
        std::set<稳定编码> ids;
        for (const auto& [key, id] : saved.新编码映射) 长期要求(有效(id) && ids.insert(id).second);
        for (const auto& n : w.节点) {
            const auto actual = 长期节点(长期映射键(saved, n.本地键), c);
            长期要求(actual.种类 == n.种类 && actual.属性类型表示 == n.属性类型表示 && actual.创建事实代次 == saved.事实代次);
        }
        auto read = [&](稳定编码 id) {
            长期扣量(c.历史数, 1, c.限额.最大历史事实数);
            auto raw = 第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, id});
            if (raw.状态 != L1所有者范围读取状态::成功) { 长期守卫(c.G); throw 长期失败{长期映射(raw.状态)}; }
            长期要求(raw.读取事实代次 == c.G, LS::事实代次漂移);
            长期要求(raw.合同版本 == L1所有者范围CRUD合同版本 && raw.查询编码 == id && raw.事实);
            return *raw.事实;
        };
        for (const auto& e : w.关系) {
            const auto raw = read(长期映射键(saved, e.本地键));
            const auto* x = std::get_if<LE>(&raw);
            长期要求(x && x->写入所有者 == 所有者_ && x->源节点 == resolve(e.源节点) && x->目标节点 == resolve(e.目标节点)
                && x->关系类型节点 == resolve(e.关系类型节点) && x->角色或顺序 == e.角色或顺序
                && x->创建事实代次 == saved.事实代次 && 长期活动(*x, saved.事实代次));
        }
        for (const auto& v : w.值) {
            const auto raw = read(长期映射键(saved, v.本地键));
            const auto* x = std::get_if<LV>(&raw);
            长期要求(x && x->写入所有者 == 所有者_ && x->所属节点 == resolve(v.所属节点)
                && x->属性类型节点 == resolve(v.属性类型节点) && x->来源节点 == resolve(v.来源节点)
                && x->材料 == v.材料 && x->创建事实代次 == saved.事实代次 && 长期活动(*x, saved.事实代次));
        }
        for (const auto& slot : w.属性槽变更) {
            const auto n = 长期节点(resolve(slot.所属节点), c);
            const auto found = std::find_if(n.当前属性.begin(), n.当前属性.end(), [&](const auto& x) {
                return x.属性类型节点 == resolve(slot.属性类型节点);
            });
            长期要求(found != n.当前属性.end() && found->当前值 == 长期映射键(saved, slot.新当前值));
        }
        for (const auto id : w.退出事实) {
            const auto raw = read(id); const auto* e = std::get_if<LE>(&raw);
            长期要求(e && e->写入所有者 == 所有者_ && e->退出事实代次 == saved.事实代次);
        }
    }
    template<class T> struct 长期准备 {
        L1所有者范围写集请求 写集;
        std::optional<T> 复用;
    };
    template<class T, class R, class Prepare, class Recover>
    特征长期写入结果<T> 长期执行(const R& r, Prepare&& prepare, Recover&& recover) {
        特征长期写入结果<T> out;
        bool uncertain = false, dispatched = false, prior = false;
        auto fail = [&](LS s) {
            out.数据.reset();
            if (uncertain || dispatched || prior) {
                out.发布确定性 = 特征长期发布确定性::未知;
                out.状态 = s == LS::幂等冲突 ? s : LS::可能已发布;
            } else out.状态 = s;
            return std::move(out);
        };
        try {
            out.原请求 = 特征长期原请求{r};
            长期要求(长期交付_.has_value(), LS::未启用);
            长期要求(r.写头.版本 == 1 && r.写头.期望G && 有效(r.写头.幂等键)
                && (r.写头.幂等键.值 >> 48) != 0x4E43, LS::入口拒绝);
            if constexpr (std::is_same_v<R, 特征校正建立请求>) 长期要求(r.内容.静止声明, LS::入口拒绝);
            out.Gread = 长期当前(); uncertain = true;
            auto first = 写入端口_.读取首次写入材料({L1所有者范围首次写入读取合同版本, r.写头.幂等键});
            长期要求(first.合同版本 == L1所有者范围首次写入读取合同版本 && first.所有者 == 所有者_
                && first.写入幂等身份 == r.写头.幂等键);
            L1所有者范围写集请求 w;
            if (first.状态 == L1所有者范围读取状态::成功) {
                长期要求(first.首次写入结果 && first.首次规范化写集);
                const auto& saved = *first.首次写入结果;
                if (saved.事实代次 && saved.事实代次 <= out.Gread) out.首次H = saved.事实代次;
                prior = true; uncertain = false;
                长期要求(first.读取事实代次 == out.Gread, LS::事实代次漂移);
                长期要求(out.首次H && saved.状态 == L1所有者范围写入状态::成功 && 写入结果头完整(saved, r.写头.幂等键));
                长期守卫(out.Gread);
                长期要求(first.首次规范化写集->期望事实代次 == r.写头.期望G, LS::幂等冲突);
                长期上下文 c{out.Gread, *out.首次H, r.预算};
                auto recovered = recover(c, saved);
                长期要求(recovered.first == *first.首次规范化写集, LS::幂等冲突);
                长期核验写后(saved, recovered.first, c);
                长期守卫(c.G);
                out.数据 = std::move(recovered.second);
                out.H = *out.首次H; out.状态 = LS::精确重复;
                out.发布确定性 = 特征长期发布确定性::已确认发布;
                return out;
            }
            if (first.状态 != L1所有者范围读取状态::未找到) throw 长期失败{长期映射(first.状态)};
            长期要求(first.读取事实代次 == out.Gread, LS::事实代次漂移);
            uncertain = false; out.发布确定性 = 特征长期发布确定性::已确认未发布;
            长期守卫(r.写头.期望G); out.Gread = r.写头.期望G;
            长期上下文 c{out.Gread, out.Gread, r.预算};
            auto plan = prepare(c);
            长期守卫(c.G);
            if (plan.复用) {
                out.数据 = std::move(plan.复用); out.H = c.H; out.状态 = LS::无须变更;
                return out;
            }
            w = std::move(plan.写集);
            长期扣量(c.记录数, w.节点.size(), c.限额.最大记录数);
            长期扣量(c.关系数, w.关系.size(), c.限额.最大关系数);
            长期扣量(c.属性数, w.值.size(), c.限额.最大属性数);
            dispatched = true;
            const auto saved = 写入端口_.提交所有者范围中性写集(w);
            if (saved.状态 != L1所有者范围写入状态::成功 && saved.状态 != L1所有者范围写入状态::精确重复) {
                if (saved.状态 == L1所有者范围写入状态::事实代次漂移 || saved.状态 == L1所有者范围写入状态::引用冲突
                    || saved.状态 == L1所有者范围写入状态::入口拒绝) {
                    dispatched = false;
                    return fail(saved.状态 == L1所有者范围写入状态::事实代次漂移 ? LS::事实代次漂移
                        : saved.状态 == L1所有者范围写入状态::引用冲突 ? LS::引用冲突 : LS::入口拒绝);
                }
                return fail(saved.状态 == L1所有者范围写入状态::幂等冲突 ? LS::幂等冲突 : LS::可能已发布);
            }
            if (saved.事实代次) out.首次H = saved.事实代次;
            长期要求(out.首次H && 写入结果头完整(saved, r.写头.幂等键));
            out.Gread = 长期当前();
            长期上下文 after{out.Gread, *out.首次H, r.预算};
            auto result = recover(after, saved);
            长期要求(result.first == w);
            长期核验写后(saved, w, after);
            长期守卫(after.G);
            out.数据 = std::move(result.second); out.H = *out.首次H;
            out.状态 = saved.状态 == L1所有者范围写入状态::精确重复 ? LS::精确重复 : LS::已发布;
            out.发布确定性 = 特征长期发布确定性::已确认发布;
            return out;
        } catch (const 长期失败& e) { return fail(e.状态); }
        catch (const std::bad_alloc&) { return fail(LS::资源失败); }
        catch (const std::length_error&) { return fail(LS::资源失败); }
        catch (...) { return fail(LS::内部不一致); }
    }
    static void 长期合并(特征长期自有事实& a, const 特征长期自有事实& b) {
        a.节点.insert(a.节点.end(), b.节点.begin(), b.节点.end());
        a.关系.insert(a.关系.end(), b.关系.begin(), b.关系.end());
        a.值.insert(a.值.end(), b.值.begin(), b.值.end());
    }
    static std::vector<特征长期来源事实> 长期来源(const 长期记录& r, const 特征观察见证& w) {
        长期要求(r.U.back() == 1 || r.U.back() == 2);
        const auto& edges = r.边[15];
        if (r.U.back() == 2) { 长期要求(edges.empty()); return {}; }
        长期要求(edges.size() == 1 && edges[0].目标节点 == w.F);
        const auto& e = edges[0];
        return {{r.节点.编码, e.编码, e.目标节点, e.创建事实代次, e.退出事实代次}};
    }
    std::uint64_t 长期源状态(const 特征观察见证& w, 长期上下文& c) const {
        const auto h = c.H; c.H = c.G;
        const auto n = 长期节点(w.F, c, true, false);
        c.H = h;
        return n.退出事实代次 && *n.退出事实代次 <= c.G ? 2 : 1;
    }
    static 特征校正内容 长期规范校正(特征校正内容 input) {
        长期要求(input.静止声明 && 有效(input.类型域.值) && input.参数版本 && 有效(input.外设来源)
            && input.裕量 >= 0 && input.样本.size() >= 2, LS::入口拒绝);
        std::sort(input.样本.begin(), input.样本.end(), [](const auto& a, const auto& b) { return a.观察标识 < b.观察标识; });
        for (std::size_t i = 0; i < input.样本.size(); ++i) {
            const auto& a = input.样本[i];
            长期要求(长期见证有效(a) && a.存在上下文 == input.样本[0].存在上下文 && a.FT == input.样本[0].FT, LS::入口拒绝);
            if (i) 长期要求(a.观察标识 != input.样本[i - 1].观察标识, LS::入口拒绝);
            for (std::size_t j = 0; j < i; ++j) 长期要求(!长期同次样本(a, input.样本[j]), LS::引用冲突);
        }
        return input;
    }
    std::pair<std::int64_t, std::vector<特征差异标量>> 长期计算校正(
        const 特征校正内容& input, const 特征类型域事实& type, 长期上下文& c) const {
        std::vector<长期实际值> values;
        for (const auto& w : input.样本) values.push_back(长期实际(type, w, c));
        std::int64_t maximum = 0;
        std::vector<特征差异标量> comparisons;
        for (std::size_t i = 0; i < values.size(); ++i) for (std::size_t j = i + 1; j < values.size(); ++j) {
            const auto d = 长期差异(type.内容, values[i], values[j], c);
            maximum = std::max(maximum, d);
            comparisons.push_back(长期差异事实(type, input.样本[i], input.样本[j], d));
        }
        长期要求(input.裕量 <= INT64_MAX - maximum, LS::差异不可表示);
        return {maximum + input.裕量, std::move(comparisons)};
    }
    特征校正事实 长期读校正(稳定编码 id, bool audit, 长期上下文& c) const {
        auto r = 长期读记录(id, c);
        长期要求(r.种类 == 2 && r.U.size() == 1 && r.I.size() == 3 && r.I[0] >= 0 && r.I[1] >= r.I[0] && r.I[2] == 1);
        长期角色(r, {8, 9, 10});
        特征校正事实 out;
        out.身份 = 特征校正身份{id}; out.创建G = r.节点.创建事实代次;
        out.内容.类型域 = 特征类型域身份{长期单边(r, 9)};
        out.内容.外设来源 = 长期单边(r, 8); out.内容.参数版本 = r.U[0];
        out.内容.裕量 = r.I[0]; out.阈值 = r.I[1]; out.内容.静止声明 = true;
        const auto type = 长期读类型(out.内容.类型域.值, c);
        const auto h = c.H; c.H = out.创建G;
        (void)长期节点(out.内容.外设来源, c, false); c.H = h;
        长期要求(r.边[9].size() >= 2);
        长期扣量(c.样本数, r.边[9].size(), c.限额.最大样本数);
        for (const auto& edge : r.边[9]) {
            auto sample = 长期读记录(edge.目标节点, c);
            长期角色(sample, {16});
            长期要求(sample.种类 == 3 && sample.U.size() == 8 && sample.I.size() == 1
                && sample.节点.创建事实代次 == out.创建G);
            const auto parent = 长期关系(sample.节点.编码, 10, true, c);
            长期要求(parent.size() == 1 && parent[0] == edge);
            auto w = 长期解析见证(sample.U, sample.I[0]);
            长期要求(w.H < out.创建G && w.FT == type.内容.FT);
            auto sources = 长期来源(sample, w);
            out.来源组.insert(out.来源组.end(), sources.begin(), sources.end());
            out.内容.样本.push_back(w);
            长期合并(r.事实, sample.事实);
        }
        长期要求(长期规范校正(out.内容) == out.内容);
        out.自有事实 = std::move(r.事实);
        if (audit) {
            auto calculated = 长期计算校正(out.内容, type, c);
            长期要求(calculated.first == out.阈值);
            out.样本对比较 = std::move(calculated.second);
            out.证据状态 = 特征长期证据状态::已核验;
        }
        return out;
    }
    特征跟踪事实 长期读跟踪(稳定编码 id, 长期上下文& c) const {
        auto r = 长期读记录(id, c);
        长期要求(r.种类 == 4 && r.U.size() == 1 && r.U[0] && r.I == std::vector<std::int64_t>{0});
        长期角色(r, {11, 12});
        特征跟踪事实 out;
        out.身份 = 特征跟踪身份{id}; out.创建G = r.节点.创建事实代次;
        out.内容 = {特征存在上下文引用{稳定编码{r.U[0]}}, 特征类型域身份{长期单边(r, 11)}, 特征校正身份{长期单边(r, 12)}};
        const auto type = 长期读类型(out.内容.类型域.值, c);
        const auto correction = 长期读校正(out.内容.校正.值, false, c);
        长期要求(correction.内容.类型域 == type.身份 && correction.创建G < out.创建G && type.创建G < out.创建G);
        out.自有事实 = std::move(r.事实); return out;
    }
    static 特征类型化原子域 长期原子域(const 特征类型域内容& t, const 长期实际值& value, std::int64_t radius) {
        长期要求(radius >= 0);
        特征类型化原子域 out; out.类型 = t;
        if (const auto* v = std::get_if<std::int64_t>(&value)) {
            const auto domain = std::get<特征I64闭区间>(t.完整域);
            const auto low = *v < INT64_MIN + radius ? INT64_MIN : *v - radius;
            const auto high = *v > INT64_MAX - radius ? INT64_MAX : *v + radius;
            out.域 = 特征I64闭区间{std::max(domain.下界, low), std::min(domain.上界, high)};
        } else out.域 = 特征二值半径域{std::get<特征二值图>(value), radius};
        return out;
    }
    特征区间事实 长期读区间(稳定编码 id, const 特征跟踪事实& track, const 特征类型域事实& type,
        const 特征校正事实& correction, 长期上下文& c) const {
        auto r = 长期读记录(id, c);
        长期要求(r.种类 == 5 && r.U.size() >= 10 && r.I.size() == 3 && r.U[8] == r.U.size() - 10);
        长期角色(r, {13, 16});
        长期要求(长期单边(r, 13) == track.身份.值 && track.创建G < r.节点.创建事实代次);
        特征区间事实 out;
        out.身份 = 特征区间身份{id}; out.跟踪 = track.身份; out.创建G = r.节点.创建事实代次;
        out.首观察 = 长期解析见证(r.U, r.I[0]); out.半径 = r.I[1];
        长期要求(out.首观察.H < out.创建G && out.首观察.存在上下文 == track.内容.存在上下文.值
            && out.首观察.FT == type.内容.FT && out.半径 == correction.阈值);
        if (type.内容.算法 == 特征长期算法::I64幅度) {
            长期要求(r.U[7] == 1 && !r.U[8]); out.固定代表 = r.I[2];
            const auto domain = std::get<特征I64闭区间>(type.内容.完整域);
            长期要求(r.I[2] >= domain.下界 && r.I[2] <= domain.上界);
        } else {
            长期要求(r.U[7] == 2 && r.I[2] == 0);
            std::vector<std::uint64_t> payload(r.U.begin() + 9, r.U.end() - 1);
            auto image = 长期解析二值(payload, c);
            长期要求(image.格式 == std::get<特征二值图格式>(type.内容.完整域));
            out.固定代表 = std::move(image);
        }
        out.原子域 = 长期原子域(type.内容, out.固定代表, out.半径);
        out.来源组 = 长期来源(r, out.首观察); out.自有事实 = std::move(r.事实);
        return out;
    }
    std::vector<特征区间事实> 长期区间组(const 特征跟踪事实& track, const 特征类型域事实& type,
        const 特征校正事实& correction, 长期上下文& c) const {
        std::vector<特征区间事实> out;
        for (const auto& edge : 长期关系(track.身份.值, 13, true, c)) {
            const auto record = 长期读记录(edge.源节点, c);
            长期要求(record.种类 == 5 && record.边[12].size() == 1 && record.边[12][0] == edge);
            out.push_back(长期读区间(edge.源节点, track, type, correction, c));
        }
        std::sort(out.begin(), out.end(), [](const auto& a, const auto& b) { return std::pair{a.创建G, a.身份.值} < std::pair{b.创建G, b.身份.值}; });
        return out;
    }
    std::vector<特征区间命中> 长期匹配(const std::vector<特征区间事实>& intervals, const 特征类型域事实& type,
        const 特征观察见证& observation, const 长期实际值& value, 长期上下文& c) const {
        std::vector<特征区间命中> out;
        for (const auto& x : intervals) {
            const auto d = 长期差异(type.内容, x.固定代表, value, c);
            if (d <= x.半径) out.push_back({x.身份, 长期差异事实(type, x.身份, observation, d)});
        }
        return out;
    }
    特征长期观测事实 长期读观察(稳定编码 id, const 特征跟踪事实& track, const 特征类型域事实& type,
        const std::vector<特征区间事实>& intervals, bool audit, 长期上下文& c) const {
        auto r = 长期读记录(id, c);
        长期要求(r.种类 == 6 && r.U.size() == 9 && r.U[7] <= 1 && r.I.size() >= 4
            && r.I[1] > 0 && static_cast<std::uint64_t>(r.I[1]) == r.I.size() - 3);
        长期角色(r, {14, 15, 16});
        长期要求(长期单边(r, 14) == track.身份.值 && r.边[14].size() == r.I.size() - 3);
        特征长期观测事实 out;
        out.身份 = 特征长期观测身份{id}; out.跟踪 = track.身份; out.创建G = r.节点.创建事实代次;
        out.见证 = 长期解析见证(r.U, r.I[0]);
        长期要求(out.见证.H < out.创建G && out.见证.FT == type.内容.FT && out.见证.存在上下文 == track.内容.存在上下文.值);
        if (type.内容.算法 == 特征长期算法::I64幅度) {
            out.实际I64值 = r.I[2];
            const auto domain = std::get<特征I64闭区间>(type.内容.完整域);
            长期要求(r.I[2] >= domain.下界 && r.I[2] <= domain.上界);
        } else 长期要求(r.I[2] == 0);
        std::optional<std::pair<std::uint64_t, 稳定编码>> previous;
        for (std::size_t i = 0; i < r.边[14].size(); ++i) {
            const auto id = r.边[14][i].目标节点;
            const auto it = std::find_if(intervals.begin(), intervals.end(), [&](const auto& x) { return x.身份.值 == id; });
            长期要求(it != intervals.end() && r.I[i + 3] >= 0 && r.I[i + 3] <= it->半径 && it->创建G <= out.创建G);
            const auto order = std::pair{it->创建G, id};
            长期要求(!previous || *previous < order); previous = order;
            out.全部命中.push_back({it->身份, 长期差异事实(type, it->身份, out.见证, r.I[i + 3])});
            if (r.U[7]) {
                长期要求(r.边[14].size() == 1 && it->创建G == out.创建G && it->首观察 == out.见证 && r.I[i + 3] == 0);
                out.本次新建 = it->身份;
            }
        }
        out.来源组 = 长期来源(r, out.见证); out.自有事实 = std::move(r.事实);
        if (audit) {
            const auto value = 长期实际(type, out.见证, c);
            if (out.实际I64值) 长期要求(std::get<std::int64_t>(value) == *out.实际I64值);
            std::vector<特征区间事实> originalIntervals;
            for (const auto& interval : intervals) if (interval.创建G <= out.创建G)
                originalIntervals.push_back(interval);
            const auto complete = 长期匹配(originalIntervals, type, out.见证, value, c);
            长期要求(complete == out.全部命中);
            out.证据状态 = 特征长期证据状态::已核验;
        }
        return out;
    }
    特征完整跟踪投影 长期投影(稳定编码 id, bool audit, 长期上下文& c) const {
        特征完整跟踪投影 out;
        out.跟踪事实 = 长期读跟踪(id, c);
        out.类型域事实 = 长期读类型(out.跟踪事实.内容.类型域.值, c);
        out.校正事实 = 长期读校正(out.跟踪事实.内容.校正.值, false, c);
        out.区间组 = 长期区间组(out.跟踪事实, out.类型域事实, out.校正事实, c);
        for (const auto& x : out.区间组) out.各区间命中数.emplace_back(x.身份, 0);
        for (const auto& edge : 长期关系(id, 14, true, c)) {
            const auto r = 长期读记录(edge.源节点, c);
            长期要求(r.种类 == 6 && r.边[13].size() == 1 && r.边[13][0] == edge);
            auto observation = 长期读观察(r.节点.编码, out.跟踪事实, out.类型域事实, out.区间组, audit, c);
            for (const auto& old : out.观察组)
                长期要求(old.见证.观察标识 != observation.见证.观察标识 && !长期同次样本(old.见证, observation.见证));
            const auto time = observation.见证.时间纳秒;
            out.首时间 = out.首时间 ? std::min(out.首时间, time) : time;
            out.末时间 = std::max(out.末时间, time);
            if (observation.实际I64值) {
                if (!out.实际范围) out.实际范围 = 特征I64闭区间{*observation.实际I64值, *observation.实际I64值};
                else { out.实际范围->下界 = std::min(out.实际范围->下界, *observation.实际I64值); out.实际范围->上界 = std::max(out.实际范围->上界, *observation.实际I64值); }
            }
            for (const auto& match : observation.全部命中)
                for (auto& counter : out.各区间命中数) if (counter.first == match.区间) ++counter.second;
            out.观察组.push_back(std::move(observation));
        }
        std::sort(out.观察组.begin(), out.观察组.end(), [](const auto& a, const auto& b) { return std::pair{a.创建G, a.身份.值} < std::pair{b.创建G, b.身份.值}; });
        out.观察总数 = out.观察组.size();
        return out;
    }
    L1所有者范围写集请求 长期校正写集(const 特征校正建立请求& r,
        const 特征校正内容& input, std::int64_t threshold, const std::vector<std::uint64_t>& states) const {
        长期要求(states.size() == input.样本.size());
        长期草稿 correction{2, {input.参数版本}, {input.裕量, threshold, 1},
            {{8, input.外设来源}, {9, input.类型域.值}}};
        std::vector<长期草稿> records;
        records.push_back(std::move(correction));
        for (std::size_t i = 0; i < input.样本.size(); ++i) {
            const auto& w = input.样本[i];
            records[0].边.push_back({10, SK{static_cast<std::uint32_t>(i + 2)}, static_cast<std::int64_t>(i + 1)});
            auto u = 长期见证U64(w); u.push_back(states[i]);
            长期草稿 sample{3, std::move(u), {w.时间纳秒}, {}};
            if (states[i] == 1) sample.边.push_back({16, w.F});
            else 长期要求(states[i] == 2);
            records.push_back(std::move(sample));
        }
        return 长期写集(r.写头, records);
    }
    L1所有者范围写集请求 长期跟踪写集(const 特征跟踪建立请求& r) const {
        return 长期写集(r.写头, {{4, {r.内容.存在上下文.值.值}, {0},
            {{11, r.内容.类型域.值}, {12, r.内容.校正.值}}}});
    }
    L1所有者范围写集请求 长期观察写集(const 特征长期观察积累请求& r,
        const std::optional<特征区间事实>& created, const std::vector<特征区间命中>& matches,
        std::optional<std::int64_t> scalar, std::uint64_t state) const {
        std::vector<长期草稿> records;
        if (created) {
            auto u = 长期见证U64(r.观察);
            std::vector<std::uint64_t> image;
            std::int64_t value = 0;
            if (const auto* n = std::get_if<std::int64_t>(&created->固定代表)) { u.push_back(1); value = *n; }
            else { u.push_back(2); image = 长期图载荷(std::get<特征二值图>(created->固定代表)); }
            u.push_back(image.size()); u.insert(u.end(), image.begin(), image.end()); u.push_back(state);
            records.push_back({5, std::move(u), {r.观察.时间纳秒, created->半径, value}, {{13, r.跟踪.值}}});
            if (state == 1) records.back().边.push_back({16, r.观察.F});
        }
        auto u = 长期见证U64(r.观察); u.push_back(created ? 1 : 0); u.push_back(state);
        长期草稿 observation{6, std::move(u), {r.观察.时间纳秒,
            static_cast<std::int64_t>(created ? 1 : matches.size()), scalar.value_or(0)}, {{14, r.跟踪.值}}};
        if (created) { observation.边.push_back({15, SK{1}, 1}); observation.I.push_back(0); }
        else for (std::size_t i = 0; i < matches.size(); ++i) {
            observation.边.push_back({15, matches[i].区间.值, static_cast<std::int64_t>(i + 1)});
            observation.I.push_back(matches[i].差异.值);
        }
        if (state == 1) observation.边.push_back({16, r.观察.F});
        else 长期要求(state == 2);
        records.push_back(std::move(observation));
        return 长期写集(r.写头, records);
    }
    std::optional<特征长期观测事实> 长期跨跟踪重复(const 特征长期观察积累请求& r,
        const 特征跟踪事实& current, 长期上下文& c) const {
        // 类型域按 FT 唯一；反向原生关系完整枚举各校正跟踪，不扫描其它类型记录。
        for (const auto& trackEdge : 长期关系(current.内容.类型域.值, 11, true, c)) {
            const auto ownerTrack = trackEdge.源节点;
            const auto rawTrack = 长期读记录(ownerTrack, c);
            长期要求(rawTrack.种类 == 4 && rawTrack.边[10].size() == 1 && rawTrack.边[10][0] == trackEdge);
            const auto track = 长期读跟踪(ownerTrack, c);
            if (ownerTrack == r.跟踪.值 || track.内容.存在上下文 != current.内容.存在上下文) continue;
            for (const auto& observationEdge : 长期关系(ownerTrack, 14, true, c)) {
                const auto record = 长期读记录(observationEdge.源节点, c);
                长期要求(record.种类 == 6 && record.边[13].size() == 1 && record.边[13][0] == observationEdge
                    && record.U.size() == 9 && record.I.size() >= 4);
                const auto witness = 长期解析见证(record.U, record.I[0]);
                if (!长期同次样本(witness, r.观察)) continue;
                const auto other = 长期投影(ownerTrack, false, c);
                const auto found = std::find_if(other.观察组.begin(), other.观察组.end(),
                    [&](const auto& x) { return x.身份.值 == record.节点.编码; });
                长期要求(found != other.观察组.end());
                长期要求(other.跟踪事实.内容 == current.内容, LS::幂等冲突);
                auto normalized = r.观察; normalized.观察标识 = witness.观察标识;
                长期要求(normalized == witness, LS::幂等冲突);
                return *found;
            }
        }
        return std::nullopt;
    }
    std::optional<特征长期结构交付> 长期交付_;
    using SS = 特征类标量状态;
    using SP = 特征类标量发布确定性;
    using SN = L1所有者范围节点事实;
    using SV = L1所有者范围值事实;
    using SE = L1所有者范围关系事实;
    using SK = L1所有者范围写集本地键;
    static constexpr std::int64_t 标量格式标记 = 0x5343414C41520002LL;
    struct 标量失败 { SS 状态; };
    struct 标量读取上下文 {
        std::uint64_t G = 0, H = 0;
        特征类派生读取预算 预算;
        std::set<稳定编码> 定义计数, 关系计数, 叶计数, 值计数;
        std::map<稳定编码, 特征类标量派生事实> 定义;
        std::map<稳定编码, 特征类标量叶回执> 叶;
        std::map<稳定编码, std::set<稳定编码>> 闭包;
        std::map<稳定编码, std::uint64_t> 高度;
        std::vector<稳定编码> 后序;
    };
    static void 标量要求(bool ok, SS e = SS::内部不一致) {
        if (!ok) throw 标量失败{e};
    }
    static SS 标量映射(L1所有者范围读取状态 s) noexcept {
        switch (s) {
        case L1所有者范围读取状态::成功: return SS::已读取;
        case L1所有者范围读取状态::未找到: return SS::未找到;
        case L1所有者范围读取状态::已退出: return SS::已退出;
        case L1所有者范围读取状态::历史材料已清理: return SS::历史材料不可用;
        case L1所有者范围读取状态::事实代次漂移: return SS::事实代次漂移;
        case L1所有者范围读取状态::资源失败: return SS::资源失败;
        case L1所有者范围读取状态::入口拒绝: return SS::入口拒绝;
        default: return SS::内部不一致;
        }
    }
    std::uint64_t 标量当前G() const {
        const auto r = 第一层服务_.读取中性当前事实代次({L1中性CRUD合同版本});
        标量要求(r.状态 == L1中性读取状态::成功,
            r.状态 == L1中性读取状态::资源失败 ? SS::资源失败 : SS::内部不一致);
        标量要求(r.合同版本 == L1中性CRUD合同版本 && r.事实代次);
        return r.事实代次;
    }
    void 标量守卫(std::uint64_t g) const { 标量要求(标量当前G() == g, SS::事实代次漂移); }
    template<class T> static bool 标量活动(const T& f, std::uint64_t h) noexcept {
        return f.创建事实代次 && f.创建事实代次 <= h
            && (!f.退出事实代次 || *f.退出事实代次 > h);
    }
    static void 标量计数(std::set<稳定编码>& ids, 稳定编码 id, std::uint64_t limit) {
        if (ids.contains(id)) return;
        标量要求(ids.size() < limit, SS::预算不足);
        ids.insert(id);
    }
    SN 标量节点(稳定编码 id, 标量读取上下文& c, bool own = true) const {
        const auto r = 第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, id});
        if (r.状态 != L1所有者范围读取状态::成功) {
            标量守卫(c.G);
            throw 标量失败{标量映射(r.状态)};
        }
        标量要求(r.读取事实代次 == c.G, SS::事实代次漂移);
        const auto* n = r.事实 ? std::get_if<SN>(&*r.事实) : nullptr;
        标量要求(r.合同版本 == L1所有者范围CRUD合同版本 && r.查询编码 == id
            && n && n->编码 == id && (!own || n->写入所有者 == 所有者_));
        标量要求(标量活动(*n, c.H), n->退出事实代次 && *n->退出事实代次 <= c.H ? SS::已退出 : SS::未找到);
        return *n;
    }
    SV 标量属性(const SN& n, 稳定编码 type, 标量读取上下文& c) const {
        const L1所有者范围属性槽* slot = nullptr;
        for (const auto& x : n.当前属性) if (x.属性类型节点 == type) {
            标量要求(!slot); slot = &x;
        }
        标量要求(slot != nullptr);
        标量计数(c.值计数, slot->当前值, c.预算.最大属性值数);
        const auto r = 第一层服务_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, slot->当前值});
        if (r.状态 != L1所有者范围读取状态::成功) {
            标量守卫(c.G); throw 标量失败{标量映射(r.状态)};
        }
        标量要求(r.读取事实代次 == c.G, SS::事实代次漂移);
        const auto* v = r.事实 ? std::get_if<SV>(&*r.事实) : nullptr;
        标量要求(r.合同版本 == L1所有者范围CRUD合同版本 && r.查询编码 == slot->当前值
            && v && v->编码 == slot->当前值 && v->所属节点 == n.编码 && v->来源节点 == n.编码
            && v->属性类型节点 == type && v->写入所有者 == 所有者_ && 标量活动(*v, c.H)
            && v->创建事实代次 == n.创建事实代次 && v->退出事实代次 == n.退出事实代次);
        return *v;
    }
    std::vector<SE> 标量关系(稳定编码 id, 稳定编码 type, bool incoming, 标量读取上下文& c) const {
        const auto direction = incoming ? L1所有者范围关系端点方向::目标 : L1所有者范围关系端点方向::源;
        const auto r = 第一层服务_.读取所有者范围历史关系组({L1所有者范围CRUD合同版本, direction, id, type, c.H});
        if (r.状态 != L1所有者范围读取状态::成功) {
            标量守卫(c.G); throw 标量失败{标量映射(r.状态)};
        }
        标量要求(r.读取事实代次 == c.G, SS::事实代次漂移);
        标量要求(r.合同版本 == L1所有者范围CRUD合同版本 && r.方向 == direction
            && r.端点节点 == id && r.关系类型节点 == type && r.历史截止事实代次 == c.H);
        std::uint64_t extra = 0;
        for (const auto& e : r.关系组) if (!c.关系计数.contains(e.编码)) ++extra;
        标量要求(extra <= c.预算.最大关系数 - c.关系计数.size(), SS::预算不足);
        std::set<稳定编码> unique;
        auto out = r.关系组;
        for (const auto& e : out) {
            标量要求(有效(e.编码) && e.写入所有者 == 所有者_ && e.关系类型节点 == type
                && (incoming ? e.目标节点 : e.源节点) == id && 标量活动(e, c.H)
                && unique.insert(e.编码).second);
            c.关系计数.insert(e.编码);
        }
        std::sort(out.begin(), out.end(), [](const auto& a, const auto& b) { return a.角色或顺序 < b.角色或顺序; });
        return out;
    }
    static bool 标量量化有效(const 特征类标量量化合同& q) noexcept {
        return q.完整();
    }
    static bool 标量同量纲(const 特征类标量量化合同& a, const 特征类标量量化合同& b) noexcept {
        return a.单位 == b.单位 && a.维度 == b.维度 && a.分量角色 == b.分量角色
            && a.缩放分子 == b.缩放分子 && a.缩放分母 == b.缩放分母 && a.量纲类别 == b.量纲类别
            && a.舍入 == b.舍入 && a.溢出 == b.溢出;
    }
    static bool 标量注册有效(const 特征类标量比较注册合同& x) noexcept {
        const bool roles = x.用途 == 特征类比较用途::目标判断
            ? x.左角色 == 特征类比较角色::当前事实 && x.右角色 == 特征类比较角色::目标状态
            : x.用途 == 特征类比较用途::状态迁移
                && x.左角色 == 特征类比较角色::前状态 && x.右角色 == 特征类比较角色::后当前事实;
        if (!roles || x.算法版本 != 1 || !x.误差合同版本 || !标量量化有效(x.输入量化)
            || (x.误差预算 && *x.误差预算 < 0) || (x.相等容差 && *x.相等容差 < 0)
            || x.输出组.empty() || x.输出组.size() > 3) return false;
        unsigned mask = 0, previous = 0;
        for (const auto& o : x.输出组) {
            const auto role = static_cast<unsigned>(o.角色);
            if (role < 1 || role > 3 || role <= previous || !标量量化有效(o.量化)) return false;
            if (role == 3) {
                if (!标量同量纲(o.量化, x.输入量化)) return false;
            } else {
                if (o.量化.量纲类别 != 特征类标量量纲::无量纲 || o.量化.缩放分子 != 1 || o.量化.缩放分母 != 1) return false;
                const auto lo = role == 1 ? -1 : x.用途 == 特征类比较用途::目标判断 ? 1 : 4;
                const auto hi = role == 1 ? 1 : x.用途 == 特征类比较用途::目标判断 ? 3 : 6;
                if (o.量化.下界 != lo || o.量化.上界 != hi) return false;
            }
            mask |= 1U << (role - 1); previous = role;
        }
        return mask == x.允许结果位;
    }
    static void 标量建立形状(const 特征类标量派生建立请求& r) {
        标量要求(r.版本 == 2 && r.G && 有效(r.幂等身份) && r.预算.有效()
            && 有效(r.派生规则.规则身份) && r.派生规则.规则版本 == 1
            && (!r.宿主E || 有效(*r.宿主E)) && 标量注册有效(r.标量注册), SS::入口拒绝);
        标量要求(r.来源组.size() == 2 && r.来源组[0].来源 != r.来源组[1].来源, SS::来源不匹配);
        for (unsigned i = 0; i < 2; ++i) {
            const auto& x = r.来源组[i];
            标量要求(x.顺序 == i + 1 && 有效(x.来源.值) && x.上游输出角色 <= 3
                && x.输入角色 == static_cast<unsigned>(i ? r.标量注册.右角色 : r.标量注册.左角色), SS::来源不匹配);
        }
    }
    static std::pair<std::vector<std::uint64_t>, std::vector<std::int64_t>> 标量编码(
        const 特征类标量派生建立请求& r) {
        const auto& x = r.标量注册; const auto& q = x.输入量化;
        std::vector<std::uint64_t> u{2, static_cast<std::uint64_t>(x.用途), 2, x.算法版本, 1,
            static_cast<std::uint64_t>(x.左角色), static_cast<std::uint64_t>(x.右角色), 1,
            q.单位.值, q.维度.值, q.分量角色.值, x.允许结果位, 1, 1, x.误差合同版本,
            r.派生规则.规则身份.值, r.来源组[0].上游输出角色, r.来源组[1].上游输出角色,
            q.缩放分子, q.缩放分母, 1, 1, x.输出组.size(), static_cast<std::uint64_t>(q.量纲类别)};
        std::vector<std::int64_t> v{x.误差预算 ? 1 : 0, x.误差预算.value_or(0),
            x.相等容差 ? 1 : 0, x.相等容差.value_or(0), q.下界, q.上界};
        for (const auto& o : x.输出组) {
            const auto& z = o.量化;
            u.insert(u.end(), {static_cast<std::uint64_t>(o.角色), z.单位.值, z.维度.值,
                z.分量角色.值, z.缩放分子, z.缩放分母, static_cast<std::uint64_t>(z.量纲类别), 1, 1});
            v.insert(v.end(), {z.下界, z.上界});
        }
        return {std::move(u), std::move(v)};
    }
    static void 标量解码(const std::vector<std::uint64_t>& u, const std::vector<std::int64_t>& v,
        特征类标量派生事实& d) {
        标量要求(!u.empty());
        标量要求(u[0] == 2, SS::格式不支持);
        标量要求(u.size() >= 24 && v.size() >= 6 && u[22] >= 1 && u[22] <= 3
            && u.size() == 24 + 9 * u[22] && v.size() == 6 + 2 * u[22]);
        标量要求(u[1] >= 1 && u[1] <= 2 && u[2] == 2 && u[4] == 1 && u[7] == 1
            && u[12] == 1 && u[13] == 1 && u[5] >= 1 && u[5] <= 4 && u[6] >= 1 && u[6] <= 4
            && u[11] >= 1 && u[11] <= 7 && u[14] && u[14] <= UINT32_MAX
            && u[15] == d.派生规则.规则身份.值 && u[16] <= 3 && u[17] <= 3
            && u[20] == 1 && u[21] == 1 && u[23] >= 1 && u[23] <= 2);
        标量要求(u[3] == 1 && d.派生规则.规则版本 == 1, SS::算法不支持);
        标量要求((v[0] == 0 || v[0] == 1) && (v[2] == 0 || v[2] == 1)
            && (v[0] ? v[1] >= 0 : v[1] == 0) && (v[2] ? v[3] >= 0 : v[3] == 0));
        auto& x = d.注册;
        x.用途 = static_cast<特征类比较用途>(u[1]); x.算法版本 = 1;
        x.左角色 = static_cast<特征类比较角色>(u[5]); x.右角色 = static_cast<特征类比较角色>(u[6]);
        x.允许结果位 = static_cast<std::uint8_t>(u[11]); x.误差合同版本 = static_cast<std::uint32_t>(u[14]);
        if (v[0]) x.误差预算 = v[1]; if (v[2]) x.相等容差 = v[3];
        x.输入量化 = {{u[8]}, {u[9]}, {u[10]}, u[18], u[19], v[4], v[5], static_cast<特征类标量量纲>(u[23])};
        d.来源组[0].内容.上游输出角色 = static_cast<std::uint8_t>(u[16]);
        d.来源组[1].内容.上游输出角色 = static_cast<std::uint8_t>(u[17]);
        for (std::size_t j = 0; j < u[22]; ++j) {
            const auto k = 24 + 9 * j;
            标量要求(u[k] >= 1 && u[k] <= 3 && u[k + 6] >= 1 && u[k + 6] <= 2 && u[k + 7] == 1 && u[k + 8] == 1);
            x.输出组.push_back({static_cast<特征类标量结果角色>(u[k]),
                {{u[k + 1]}, {u[k + 2]}, {u[k + 3]}, u[k + 4], u[k + 5],
                    v[6 + 2 * j], v[7 + 2 * j], static_cast<特征类标量量纲>(u[k + 6])}});
        }
        标量要求(标量注册有效(x));
    }
    特征类标量派生事实 标量自有定义(稳定编码 id, 标量读取上下文& c) const {
        标量要求(有效(派生结构类型_.输出类型关系类型), SS::格式不支持);
        标量计数(c.定义计数, id, c.预算.最大定义数);
        const auto n = 标量节点(id, c);
        标量要求(n.种类 == 节点种类::普通 && !n.属性类型表示 && n.当前属性.size() == 2, SS::类型不匹配);
        const auto order = 标量属性(n, 派生结构类型_.实际阶次属性类型, c);
        const auto rule = 标量属性(n, 派生结构类型_.派生规则属性类型, c);
        const auto* level = std::get_if<std::int64_t>(&order.材料);
        const auto* rules = std::get_if<std::vector<std::uint64_t>>(&rule.材料);
        标量要求(level && *level > 1 && static_cast<std::uint64_t>(*level) <= UINT32_MAX
            && rules && rules->size() == 2 && (*rules)[0] && (*rules)[1] && (*rules)[1] <= UINT32_MAX);
        特征类标量派生事实 d;
        d.定义身份 = {id}; d.真实阶次 = static_cast<std::uint32_t>(*level);
        d.阶次值 = order.编码; d.规则值 = rule.编码;
        d.派生规则 = {{(*rules)[0]}, static_cast<std::uint32_t>((*rules)[1])};
        d.创建G = n.创建事实代次; d.退出G = n.退出事实代次;
        const auto sources = 标量关系(id, 派生结构类型_.直接来源关系类型, false, c);
        const auto host = 标量关系(id, 派生结构类型_.宿主关系类型, false, c);
        const auto reg = 标量关系(id, 派生结构类型_.比较注册归属关系类型, false, c);
        标量要求(sources.size() == 2 && host.size() <= 1 && reg.size() == 1);
        auto sameLife = [&](const SE& e) { 标量要求(e.创建事实代次 == n.创建事实代次 && e.退出事实代次 == n.退出事实代次); };
        for (unsigned i = 0; i < 2; ++i) {
            const auto& e = sources[i]; sameLife(e);
            const auto bits = static_cast<std::uint64_t>(e.角色或顺序);
            标量要求(bits >> 32U == i + 1 && static_cast<std::uint32_t>(bits) != 0);
            d.来源组.push_back({{i + 1, static_cast<std::uint32_t>(bits), {e.目标节点}, 0}, e.编码});
        }
        标量要求(d.来源组[0].内容.来源 != d.来源组[1].内容.来源);
        if (!host.empty()) {
            sameLife(host[0]); 标量要求(host[0].角色或顺序 == 1);
            const auto hn = 标量节点(host[0].目标节点, c, false);
            标量要求(hn.种类 == 节点种类::普通 && !hn.属性类型表示, SS::来源不匹配);
            d.宿主E = hn.编码; d.宿主关系 = host[0].编码;
        }
        sameLife(reg[0]); 标量要求(reg[0].角色或顺序 == 1);
        const auto rn = 标量节点(reg[0].目标节点, c);
        标量要求(rn.种类 == 节点种类::普通 && !rn.属性类型表示 && rn.当前属性.size() == 2
            && rn.创建事实代次 == n.创建事实代次 && rn.退出事实代次 == n.退出事实代次);
        const auto uv = 标量属性(rn, 派生结构类型_.比较注册U64属性类型, c);
        const auto iv = 标量属性(rn, 派生结构类型_.比较注册I64属性类型, c);
        const auto* u = std::get_if<std::vector<std::uint64_t>>(&uv.材料);
        const auto* v = std::get_if<std::vector<std::int64_t>>(&iv.材料);
        标量要求(u && v); 标量解码(*u, *v, d);
        d.注册身份 = {rn.编码}; d.注册归属 = reg[0].编码; d.注册U64值 = uv.编码; d.注册I64值 = iv.编码;
        const auto outputs = 标量关系(id, 派生结构类型_.输出类型关系类型, false, c);
        标量要求(outputs.size() == d.注册.输出组.size());
        for (std::size_t i = 0; i < outputs.size(); ++i) {
            const auto& e = outputs[i]; sameLife(e);
            标量要求(e.角色或顺序 == static_cast<std::int64_t>(d.注册.输出组[i].角色));
            const auto ft = 标量节点(e.目标节点, c);
            标量要求(ft.种类 == 节点种类::属性类型 && ft.属性类型表示 == L1所有者范围值表示种类::I64
                && ft.当前属性.size() == 1 && ft.创建事实代次 == n.创建事实代次 && ft.退出事实代次 == n.退出事实代次);
            const auto marker = 标量属性(ft, ft.编码, c);
            const auto* value = std::get_if<std::int64_t>(&marker.材料);
            标量要求(value && *value == 标量格式标记);
            const auto reverse = 标量关系(ft.编码, 派生结构类型_.输出类型关系类型, true, c);
            标量要求(reverse.size() == 1 && reverse[0] == e);
            d.输出组.push_back({d.注册.输出组[i], ft.编码, e.编码, marker.编码});
        }
        for (unsigned i = 0; i < 2; ++i)
            标量要求(d.来源组[i].内容.输入角色 == static_cast<unsigned>(i ? d.注册.右角色 : d.注册.左角色));
        return d;
    }
    特征类标量叶回执 标量读叶(稳定编码 id, 标量读取上下文& c) const {
        标量计数(c.叶计数, id, c.预算.最大叶数);
        const auto n = 标量节点(id, c);
        标量要求(n.种类 == 节点种类::普通 && !n.属性类型表示 && n.当前属性.size() == 1, SS::类型不匹配);
        const auto ft = 标量节点(n.当前属性[0].属性类型节点, c);
        标量要求(ft.种类 == 节点种类::属性类型 && ft.属性类型表示 == L1所有者范围值表示种类::I64
            && std::none_of(ft.当前属性.begin(), ft.当前属性.end(), [&](const auto& x) { return x.属性类型节点 == ft.编码; }), SS::类型不匹配);
        const auto v = 标量属性(n, ft.编码, c);
        const auto* value = std::get_if<std::int64_t>(&v.材料);
        标量要求(value != nullptr, SS::类型不匹配);
        const auto exact = 读取特征历史事实({1, c.G, c.H, id, v.编码});
        if (!exact.成功()) {
            标量守卫(c.G);
            switch (exact.状态) {
            case 特征引用读取状态::资源失败: throw 标量失败{SS::资源失败};
            case 特征引用读取状态::事实代次漂移: throw 标量失败{SS::事实代次漂移};
            case 特征引用读取状态::历史材料不可用: throw 标量失败{SS::历史材料不可用};
            default: throw 标量失败{SS::来源不匹配};
            }
        }
        标量要求(exact.Gread == c.G && exact.H == c.H && exact.特征
            && exact.特征->结点 == id && exact.特征->值事实 == v.编码 && exact.特征->特征类型 == ft.编码);
        // 普通 F 不可借只有形状相似的派生结构进入叶路径。
        for (auto type : {派生结构类型_.直接来源关系类型, 派生结构类型_.宿主关系类型,
                派生结构类型_.比较注册归属关系类型, 派生结构类型_.输出类型关系类型})
            if (有效(type)) 标量要求(标量关系(id, type, false, c).empty(), SS::类型不匹配);
        return {id, ft.编码, v.编码, *value, c.H, n.创建事实代次, n.退出事实代次};
    }
    const 特征类标量输出事实& 标量输出(const 特征类标量派生事实& d, unsigned role) const {
        const auto it = std::find_if(d.输出组.begin(), d.输出组.end(), [&](const auto& x) { return static_cast<unsigned>(x.声明.角色) == role; });
        标量要求(it != d.输出组.end(), SS::来源不匹配); return *it;
    }
    void 标量展开(稳定编码 root, bool derived, 标量读取上下文& c) const {
        struct Frame { 稳定编码 id; bool derived; bool finish; std::uint64_t depth; };
        std::vector<Frame> stack{{root, derived, false, 1}};
        std::set<稳定编码> gray;
        while (!stack.empty()) {
            const auto f = stack.back(); stack.pop_back();
            标量要求(f.depth <= c.预算.最大深度, SS::预算不足);
            if (f.finish) {
                const auto& d = c.定义.at(f.id);
                std::uint32_t maxOrder = 0; std::uint64_t height = 0;
                std::set<稳定编码> leaves;
                for (const auto& source : d.来源组) {
                    const auto& x = source.内容;
                    if (x.上游输出角色) {
                        const auto& child = c.定义.at(x.来源.值);
                        const auto& output = 标量输出(child, x.上游输出角色);
                        标量要求(标量同量纲(output.声明.量化, d.注册.输入量化), SS::单位量化不匹配);
                        maxOrder = std::max(maxOrder, child.真实阶次);
                    } else { 标量要求(c.叶.contains(x.来源.值), SS::来源不匹配); maxOrder = std::max(maxOrder, 1U); }
                    const auto& set = c.闭包.at(x.来源.值); leaves.insert(set.begin(), set.end());
                    height = std::max(height, c.高度.at(x.来源.值));
                }
                标量要求(maxOrder != UINT32_MAX && d.真实阶次 == maxOrder + 1);
                标量要求(height < c.预算.最大深度, SS::预算不足);
                c.高度[f.id] = height + 1; c.闭包[f.id] = std::move(leaves);
                c.后序.push_back(f.id); gray.erase(f.id); continue;
            }
            标量要求(!gray.contains(f.id));
            if (c.闭包.contains(f.id)) {
                标量要求(f.derived == c.定义.contains(f.id), SS::来源不匹配);
                标量要求(c.高度.at(f.id) <= c.预算.最大深度 - f.depth + 1, SS::预算不足); continue;
            }
            if (!f.derived) {
                c.叶.emplace(f.id, 标量读叶(f.id, c)); c.闭包[f.id] = {f.id}; c.高度[f.id] = 1; continue;
            }
            auto d = 标量自有定义(f.id, c);
            gray.insert(f.id); stack.push_back({f.id, true, true, f.depth});
            for (auto it = d.来源组.rbegin(); it != d.来源组.rend(); ++it)
                stack.push_back({it->内容.来源.值, it->内容.上游输出角色 != 0, false, f.depth + 1});
            c.定义.emplace(f.id, std::move(d));
        }
    }
    特征类标量派生读取结果 标量读取结果(稳定编码 id, 标量读取上下文& c) const {
        特征类标量派生读取结果 out;
        out.状态 = SS::已读取; out.Gread = c.G; out.H = c.H; out.定义事实 = c.定义.at(id);
        for (const auto& leaf : c.闭包.at(id)) out.基础叶组.push_back(c.叶.at(leaf));
        const auto& d = *out.定义事实;
        const auto& left = c.闭包.at(d.来源组[0].内容.来源.值);
        const auto& right = c.闭包.at(d.来源组[1].内容.来源.值);
        out.左叶组.assign(left.begin(), left.end()); out.右叶组.assign(right.begin(), right.end());
        return out;
    }
public:
    特征类标量派生读取结果 读取标量派生定义(const 特征类标量派生读取请求& r) const {
        特征类标量派生读取结果 out;
        auto fail = [&](SS e) { 特征类标量派生读取结果 f; f.状态 = e; f.Gread = out.Gread; f.H = r.H; return f; };
        try {
            标量要求(r.版本 == 2 && r.H && r.Gread >= r.H && 有效(r.定义身份.值) && r.预算.有效(), SS::入口拒绝);
            标量守卫(r.Gread); out.Gread = r.Gread;
            标量读取上下文 c{r.Gread, r.H, r.预算};
            标量展开(r.定义身份.值, true, c);
            out = 标量读取结果(r.定义身份.值, c); 标量守卫(r.Gread);
            标量要求(out.成功()); return out;
        } catch (const 标量失败& e) { return fail(e.状态); }
        catch (const std::bad_alloc&) { return fail(SS::资源失败); }
        catch (const std::length_error&) { return fail(SS::资源失败); }
        catch (...) { return fail(SS::内部不一致); }
    }


private:
    static void 标量排序写集(L1所有者范围写集请求& w) {
        auto local = [](const auto& a, const auto& b) { return a.本地键 < b.本地键; };
        std::sort(w.节点.begin(), w.节点.end(), local);
        std::sort(w.关系.begin(), w.关系.end(), local);
        std::sort(w.值.begin(), w.值.end(), local);
        auto less = [](const L1所有者范围事实引用& a, const L1所有者范围事实引用& b) {
            if (a.index() != b.index()) return a.index() < b.index();
            auto key = [](const auto& x) { return static_cast<std::uint64_t>(x.值); };
            return std::visit(key, a) < std::visit(key, b);
        };
        std::sort(w.属性槽变更.begin(), w.属性槽变更.end(), [&](const auto& a, const auto& b) {
            return a.所属节点 != b.所属节点 ? less(a.所属节点, b.所属节点) : less(a.属性类型节点, b.属性类型节点);
        });
        std::sort(w.退出事实.begin(), w.退出事实.end());
        标量要求(std::adjacent_find(w.退出事实.begin(), w.退出事实.end()) == w.退出事实.end());
    }
    L1所有者范围写集请求 标量建立写集(const 特征类标量派生建立请求& r, std::uint32_t level) const {
        特征类派生定义新增请求 base;
        base.期望事实代次 = r.G; base.幂等身份 = r.幂等身份; base.派生规则 = r.派生规则; base.宿主存在 = r.宿主E;
        for (const auto& x : r.来源组) base.直接来源.push_back({x.顺序, x.输入角色, x.来源});
        auto w = 形成派生新增写集(base, level);
        const auto [u, v] = 标量编码(r);
        for (auto& value : w.值) {
            if (value.本地键 == SK{0x30003}) value.材料 = u;
            if (value.本地键 == SK{0x30004}) value.材料 = v;
        }
        for (const auto& x : r.标量注册.输出组) {
            const auto role = static_cast<std::uint32_t>(x.角色);
            const SK ft{0x40000U + role}, edge{0x41000U + role}, marker{0x42000U + role};
            w.节点.push_back({ft, 节点种类::属性类型, L1所有者范围值表示种类::I64});
            w.关系.push_back({edge, SK{1}, ft, 派生结构类型_.输出类型关系类型, static_cast<std::int64_t>(role)});
            w.值.push_back({marker, ft, ft, 标量格式标记, ft});
            w.属性槽变更.push_back({ft, ft, marker});
        }
        标量排序写集(w); return w;
    }
    L1所有者范围写集请求 标量退出写集(const 特征类标量派生退出请求& r,
        const 特征类标量派生事实& d) const {
        L1所有者范围写集请求 w;
        w.合同版本 = L1所有者范围CRUD合同版本; w.期望事实代次 = r.G; w.写入幂等身份 = r.幂等身份;
        w.退出事实 = {d.定义身份.值, d.注册身份.值, d.阶次值, d.规则值,
            d.注册U64值, d.注册I64值, d.注册归属};
        for (const auto& x : d.来源组) w.退出事实.push_back(x.关系);
        if (d.宿主关系) w.退出事实.push_back(*d.宿主关系);
        // L1 退出节点时保存完整历史属性；不能再向退出节点提交槽变更。
        for (const auto& x : d.输出组) {
            w.退出事实.insert(w.退出事实.end(), {x.特征类型, x.归属关系, x.格式标记值事实});
        }
        标量排序写集(w); return w;
    }
    std::uint32_t 标量准入来源(const 特征类标量派生建立请求& r, 标量读取上下文& c) const {
        std::uint32_t maximum = 0; std::uint64_t height = 0;
        for (const auto& x : r.来源组) {
            标量展开(x.来源.值, x.上游输出角色 != 0, c);
            if (x.上游输出角色) {
                const auto& d = c.定义.at(x.来源.值);
                const auto& o = 标量输出(d, x.上游输出角色);
                标量要求(标量同量纲(o.声明.量化, r.标量注册.输入量化), SS::单位量化不匹配);
                const auto computed = 标量复算(x.来源.值, 1U << (x.上游输出角色 - 1), c);
                const auto value = computed.值.at(x.来源.值).at(x.上游输出角色).值;
                标量要求(value >= r.标量注册.输入量化.下界 && value <= r.标量注册.输入量化.上界, SS::结果范围不满足);
                maximum = std::max(maximum, d.真实阶次);
            } else {
                const auto value = c.叶.at(x.来源.值).值;
                标量要求(value >= r.标量注册.输入量化.下界 && value <= r.标量注册.输入量化.上界, SS::结果范围不满足);
                maximum = std::max(maximum, 1U);
            }
            height = std::max(height, c.高度.at(x.来源.值));
        }
        标量要求(maximum != UINT32_MAX, SS::运算溢出);
        标量要求(height < r.预算.最大深度, SS::预算不足);
        const auto n = r.标量注册.输出组.size();
        标量要求(c.定义计数.size() < r.预算.最大定义数
            && 4 + n <= r.预算.最大属性值数 - c.值计数.size()
            && 3 + (r.宿主E ? 1U : 0U) + n <= r.预算.最大关系数 - c.关系计数.size(), SS::预算不足);
        if (r.宿主E) {
            const auto e = 标量节点(*r.宿主E, c, false);
            标量要求(e.种类 == 节点种类::普通 && !e.属性类型表示, SS::来源不匹配);
        }
        return maximum + 1;
    }
    void 标量映射互证(const L1所有者范围写入结果& receipt, const 特征类标量派生事实& d) const {
        auto id = [&](std::uint32_t k) {
            const auto x = 特征类数据内部::查找唯一编码(receipt, SK{k});
            标量要求(x.has_value()); return *x;
        };
        标量要求(receipt.新编码映射.size() == 9 + (d.宿主E ? 1U : 0U) + 3 * d.输出组.size()
            && id(1) == d.定义身份.值 && id(2) == d.阶次值 && id(3) == d.规则值
            && id(0x10001) == d.来源组[0].关系 && id(0x10002) == d.来源组[1].关系
            && id(0x30001) == d.注册身份.值 && id(0x30002) == d.注册归属
            && id(0x30003) == d.注册U64值 && id(0x30004) == d.注册I64值);
        if (d.宿主关系) 标量要求(id(0x20001) == *d.宿主关系);
        for (const auto& x : d.输出组) {
            const auto r = static_cast<std::uint32_t>(x.声明.角色);
            标量要求(id(0x40000 + r) == x.特征类型 && id(0x41000 + r) == x.归属关系
                && id(0x42000 + r) == x.格式标记值事实);
        }
    }
    template<class R> 特征类标量派生写结果 标量执行写(const R& r) {
        constexpr bool create = std::is_same_v<R, 特征类标量派生建立请求>;
        特征类标量派生写结果 out;
        bool keyUnknown = false, dispatched = false, prior = false;
        auto fail = [&](SS e) {
            out.定义事实.reset();
            if (dispatched || prior || keyUnknown) {
                out.发布确定性 = SP::可能已发布;
                out.状态 = e == SS::幂等冲突 ? e : SS::已可能发布;
            } else { out.状态 = e; }
            return std::move(out);
        };
        try {
            if constexpr (create) 标量建立形状(r);
            else 标量要求(r.版本 == 2 && r.G && 有效(r.幂等身份) && 有效(r.定义身份.值) && r.预算.有效(), SS::入口拒绝);
            标量要求(有效(派生结构类型_.输出类型关系类型), SS::格式不支持);
            out.Gread = 标量当前G(); keyUnknown = true;
            const auto first = 写入端口_.读取首次写入材料({L1所有者范围首次写入读取合同版本, r.幂等身份});
            L1所有者范围写集请求 w;
            稳定编码 target{};
            std::uint64_t readH = r.G;
            if (first.状态 == L1所有者范围读取状态::成功) {
                标量要求(first.合同版本 == L1所有者范围首次写入读取合同版本
                    && first.所有者 == 所有者_ && first.写入幂等身份 == r.幂等身份
                    && first.首次规范化写集 && first.首次写入结果);
                const auto& saved = *first.首次写入结果;
                if (saved.事实代次 && saved.事实代次 <= out.Gread) out.首次发布H = saved.事实代次;
                out.正式回执 = saved; prior = true; keyUnknown = false;
                标量要求(saved.状态 == L1所有者范围写入状态::成功 && 写入结果头完整(saved, r.幂等身份)
                    && out.首次发布H && first.读取事实代次 == out.Gread);
                标量守卫(out.Gread);
                标量要求(first.首次规范化写集->期望事实代次 == r.G, SS::幂等冲突);
                标量读取上下文 c{out.Gread, create ? *out.首次发布H : r.G, r.预算};
                if constexpr (create) {
                    const auto id = 特征类数据内部::查找唯一编码(saved, SK{1});
                    标量要求(id.has_value()); target = *id;
                    标量展开(target, true, c);
                    w = 标量建立写集(r, c.定义.at(target).真实阶次);
                    标量映射互证(saved, c.定义.at(target));
                } else {
                    target = r.定义身份.值; 标量展开(target, true, c);
                    w = 标量退出写集(r, c.定义.at(target));
                    标量要求(saved.新编码映射.empty());
                }
                标量要求(w == *first.首次规范化写集, SS::幂等冲突);
            } else if (first.状态 == L1所有者范围读取状态::未找到) {
                标量要求(first.合同版本 == L1所有者范围首次写入读取合同版本
                    && first.所有者 == 所有者_ && first.写入幂等身份 == r.幂等身份);
                keyUnknown = false; out.发布确定性 = SP::确认未发布;
                标量守卫(r.G); out.Gread = r.G;
                标量读取上下文 c{r.G, r.G, r.预算};
                if constexpr (create) w = 标量建立写集(r, 标量准入来源(r, c));
                else {
                    target = r.定义身份.值; 标量展开(target, true, c);
                    const auto& d = c.定义.at(target);
                    // 下游定义与输出类型上的真实外部引用仍由 L1 原子闭包守卫最终裁决。
                    标量要求(标量关系(target, 派生结构类型_.直接来源关系类型, true, c).empty(), SS::引用冲突);
                    w = 标量退出写集(r, d);
                }
                标量守卫(r.G);
            } else { throw 标量失败{标量映射(first.状态)}; }
            dispatched = true;
            auto receipt = 写入端口_.提交所有者范围中性写集(w);
            out.正式回执 = std::move(receipt);
            const auto& committed = *out.正式回执;
            const bool ok = committed.状态 == L1所有者范围写入状态::成功 || committed.状态 == L1所有者范围写入状态::精确重复;
            if (!ok) {
                if (!prior && (committed.状态 == L1所有者范围写入状态::事实代次漂移
                        || committed.状态 == L1所有者范围写入状态::引用冲突
                        || committed.状态 == L1所有者范围写入状态::入口拒绝)) {
                    dispatched = false; out.发布确定性 = SP::确认未发布;
                    return fail(committed.状态 == L1所有者范围写入状态::事实代次漂移 ? SS::事实代次漂移
                        : committed.状态 == L1所有者范围写入状态::引用冲突 ? SS::引用冲突 : SS::入口拒绝);
                }
                return fail(committed.状态 == L1所有者范围写入状态::幂等冲突 ? SS::幂等冲突 : SS::已可能发布);
            }
            if (committed.事实代次) out.首次发布H = committed.事实代次;
            标量要求(写入结果头完整(committed, r.幂等身份) && out.首次发布H);
            out.Gread = 标量当前G();
            标量要求(out.Gread >= *out.首次发布H);
            if constexpr (create) {
                const auto id = 特征类数据内部::查找唯一编码(committed, SK{1});
                标量要求(id.has_value()); target = *id; readH = *out.首次发布H;
            }
            标量读取上下文 c{out.Gread, readH, r.预算};
            标量展开(target, true, c);
            auto d = c.定义.at(target);
            if constexpr (create) {
                标量映射互证(committed, d);
                标量要求(标量建立写集(r, d.真实阶次) == w);
            } else {
                标量要求(committed.新编码映射.empty() && d.退出G == out.首次发布H && 标量退出写集(r, d) == w);
            }
            标量守卫(out.Gread);
            out.定义事实 = std::move(d); out.发布确定性 = SP::确认已发布;
            out.状态 = committed.状态 == L1所有者范围写入状态::精确重复 ? SS::精确重复 : create ? SS::已创建 : SS::已退出;
            标量要求(out.成功()); return out;
        } catch (const 标量失败& e) { return fail(e.状态); }
        catch (const std::bad_alloc&) { return fail(SS::资源失败); }
        catch (const std::length_error&) { return fail(SS::资源失败); }
        catch (...) { return fail(SS::内部不一致); }
    }
    struct 标量计算缓存 {
        std::map<稳定编码, std::map<unsigned, 特征类标量结果项>> 值;
        std::map<稳定编码, std::array<特征类标量输入回执, 2>> 输入;
    };
    标量计算缓存 标量复算(稳定编码 root, unsigned requested, 标量读取上下文& c) const {
        std::map<稳定编码, unsigned> needed;
        needed[root] = requested;
        for (auto it = c.后序.rbegin(); it != c.后序.rend(); ++it) {
            const auto& d = c.定义.at(*it);
            for (const auto& x : d.来源组)
                if (x.内容.上游输出角色) needed[x.内容.来源.值] |= 1U << (x.内容.上游输出角色 - 1);
        }
        标量计算缓存 cache;
        for (auto id : c.后序) {
            const auto& d = c.定义.at(id); const auto& r = d.注册;
            标量要求((needed[id] & ~r.允许结果位) == 0, SS::来源不匹配);
            std::array<std::int64_t, 2> values{};
            std::array<特征类标量输入回执, 2> inputs;
            for (unsigned i = 0; i < 2; ++i) {
                const auto& x = d.来源组[i].内容;
                const auto role = i ? r.右角色 : r.左角色;
                if (!x.上游输出角色) {
                    const auto& leaf = c.叶.at(x.来源.值); values[i] = leaf.值;
                    inputs[i] = 特征类标量基础输入回执{leaf, role};
                } else {
                    const auto& value = cache.值.at(x.来源.值).at(x.上游输出角色);
                    标量要求(标量同量纲(value.量化, r.输入量化), SS::单位量化不匹配);
                    values[i] = value.值;
                    inputs[i] = 特征类标量上游输入回执{x.来源, value.输出FT, value.角色,
                        value.值, c.定义.at(x.来源.值).真实阶次, role};
                }
                标量要求(values[i] >= r.输入量化.下界 && values[i] <= r.输入量化.上界, SS::结果范围不满足);
            }
            const std::int64_t order = values[0] < values[1] ? -1 : values[0] > values[1] ? 1 : 0;
            const bool equivalent = order == 0 || (r.相等容差
                && I64差异幅度(values[0], values[1]) <= static_cast<std::uint64_t>(*r.相等容差));
            for (const auto& o : d.输出组) {
                const auto role = static_cast<unsigned>(o.声明.角色);
                if (!(needed[id] & (1U << (role - 1)))) continue;
                std::int64_t value = 0;
                if (role == 1) value = order;
                else if (role == 2) value = r.用途 == 特征类比较用途::目标判断
                    ? (equivalent ? 2 : order < 0 ? 1 : 3) : (equivalent ? 5 : order < 0 ? 4 : 6);
                else {
                    标量要求(I64差异可表示(values[0], values[1]), SS::运算溢出);
                    value = values[1] - values[0];
                }
                标量要求(value >= o.声明.量化.下界 && value <= o.声明.量化.上界, SS::结果范围不满足);
                cache.值[id].emplace(role, 特征类标量结果项{o.特征类型, o.声明.角色, value,
                    o.声明.量化, static_cast<特征类标量方向>(role)});
            }
            cache.输入.emplace(id, std::move(inputs));
        }
        return cache;
    }
public:
    特征类标量派生写结果 建立标量派生定义(const 特征类标量派生建立请求& r) { return 标量执行写(r); }
    特征类标量派生写结果 退出标量派生定义(const 特征类标量派生退出请求& r) { return 标量执行写(r); }
    特征类标量比较结果 比较派生特征(const 特征类标量比较请求& r) const {
        特征类标量比较结果 out;
        auto fail = [&](SS e) {
            特征类标量比较结果 f; f.状态 = e; f.拒绝原因 = e; f.请求身份 = r.请求身份; f.G = out.G;
            return f;
        };
        try {
            标量要求(r.版本 == 2 && r.G && r.请求身份 && 有效(r.根定义.值) && r.预算.有效()
                && r.要求结果位 >= 1 && r.要求结果位 <= 7, SS::入口拒绝);
            标量守卫(r.G); out.G = r.G;
            标量读取上下文 c{r.G, r.G, r.预算}; 标量展开(r.根定义.值, true, c);
            const auto& d = c.定义.at(r.根定义.值);
            标量要求(r.用途 == d.注册.用途, SS::来源不匹配);
            标量要求(!r.预期算法版本 || *r.预期算法版本 == d.注册.算法版本, SS::算法不支持);
            标量要求((r.要求结果位 & ~d.注册.允许结果位) == 0, SS::入口拒绝);
            const auto cache = 标量复算(r.根定义.值, r.要求结果位, c);
            out.状态 = SS::已比较; out.拒绝原因 = SS::已比较; out.请求身份 = r.请求身份;
            out.根定义 = r.根定义; out.注册身份 = d.注册身份; out.算法版本 = d.注册.算法版本;
            out.真实阶次 = d.真实阶次; out.实际结果位 = r.要求结果位;
            out.输入量化 = d.注册.输入量化; out.误差合同版本 = d.注册.误差合同版本;
            out.误差预算 = d.注册.误差预算; out.相等容差 = d.注册.相等容差;
            out.直接输入回执 = cache.输入.at(r.根定义.值);
            for (const auto& id : c.闭包.at(r.根定义.值)) out.基础叶回执组.push_back(c.叶.at(id));
            for (const auto& [_, value] : cache.值.at(r.根定义.值)) out.结果组.push_back(value);
            标量守卫(r.G); 标量要求(out.成功()); return out;
        } catch (const 标量失败& e) { return fail(e.状态); }
        catch (const std::bad_alloc&) { return fail(SS::资源失败); }
        catch (const std::length_error&) { return fail(SS::资源失败); }
        catch (...) { return fail(SS::内部不一致); }
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
        if (std::any_of(类型事实.当前属性.begin(), 类型事实.当前属性.end(),
                [特征类型](const auto& 槽) { return 槽.属性类型节点 == 特征类型; }))
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

        // 从此处起原键已经发布；任何读回失败都不能再证明原键未绑定。
        const auto 未收敛 = [&] {
            return 失败(特征类数据状态::已可能发布, 写入.事实代次);
        };
        try {
            if (!写入结果头完整(写入, 幂等身份) || 写入.新编码映射.size() != 2)
                return 未收敛();
            const auto 结点 = 特征类数据内部::查找唯一编码(
                写入, 特征类数据内部::特征结点本地键);
            const auto 值 = 特征类数据内部::查找唯一编码(
                写入, 特征类数据内部::特征值本地键);
            if (!结点 || !值)
                return 未收敛();
            const auto 当前 = 第一层服务_.读取中性当前事实代次({L1中性CRUD合同版本});
            if (当前.状态 != L1中性读取状态::成功
                || 当前.合同版本 != L1中性CRUD合同版本 || 当前.事实代次 < 写入.事实代次)
                return 未收敛();
            auto 读回 = 读取历史新特征(*结点, *值, 写入.事实代次, 当前.事实代次);
            if (!读回.成功())
                return 未收敛();
            if (被换代特征结点) {
                const auto 退出读回 = 读取历史已退出特征(
                    *被换代特征结点, 写入.事实代次, 当前.事实代次);
                if (!退出读回.成功())
                    return 未收敛();
            }
            const auto 末读 = 第一层服务_.读取中性当前事实代次({L1中性CRUD合同版本});
            if (末读.状态 != L1中性读取状态::成功
                || 末读.合同版本 != L1中性CRUD合同版本 || 末读.事实代次 != 当前.事实代次)
                return 未收敛();
            读回.状态 = 状态;
            读回.被换代特征结点 = 被换代特征结点;
            return 读回;
        } catch (...) {
            return 未收敛();
        }
    }

    特征类结点结果 读取历史新特征(稳定编码 结点,
        稳定编码 值, std::uint64_t 创建事实代次, std::uint64_t 读取事实代次) const {
        const auto 当前 = 第一层服务_.读取中性当前事实代次({L1中性CRUD合同版本});
        if (当前.状态 != L1中性读取状态::成功
            || 当前.合同版本 != L1中性CRUD合同版本
            || !创建事实代次 || 当前.事实代次 < 创建事实代次
            || 当前.事实代次 != 读取事实代次)
            return 失败(特征类数据状态::内部不一致, 创建事实代次);
        // 身份来自首次映射，H 来自首次发布；不读取已经换代的当前槽。
        auto 历史 = 读取特征历史事实({1, 当前.事实代次, 创建事实代次, 结点, 值});
        if (!历史.成功() || 历史.Gread != 当前.事实代次 || 历史.H != 创建事实代次
            || 历史.特征->结点 != 结点 || 历史.特征->值事实 != 值
            || 历史.特征->创建事实代次 != 创建事实代次)
            return 失败(特征类数据状态::内部不一致, 创建事实代次);
        return {特征类数据状态::已读取, 特征类数据合同版本,
            创建事实代次, std::move(历史.特征), std::nullopt};
    }

    特征类结点结果 读取历史已退出特征(稳定编码 结点,
        std::uint64_t 退出事实代次, std::uint64_t 读取事实代次 = 0) const {
        const auto 结点读取 = 第一层服务_.读取所有者范围历史事实(
            {L1所有者范围CRUD合同版本, 结点});
        const auto* 结点事实 = 结点读取.事实
            ? std::get_if<L1所有者范围节点事实>(&*结点读取.事实) : nullptr;
        if (结点读取.状态 != L1所有者范围读取状态::成功
            || (读取事实代次 && 结点读取.读取事实代次 != 读取事实代次)
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
            || (读取事实代次 && 值读取.读取事实代次 != 读取事实代次)
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
