module;
#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <limits>
#include <map>
#include <mutex>
#include <new>
#include <numeric>
#include <optional>
#include <set>
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
    friend bool operator==(const 特征信息身份&, const 特征信息身份&) = default;
};
struct 特征类型身份 final {
    稳定编码 编码{};
    friend bool operator==(const 特征类型身份&, const 特征类型身份&) = default;
};
struct 特征比较规则身份 final {
    稳定编码 编码{};
    friend bool operator==(const 特征比较规则身份&, const 特征比较规则身份&) = default;
};
inline bool 有效(特征信息身份 v) noexcept { return 有效(v.编码); }
inline bool 有效(特征类型身份 v) noexcept { return 有效(v.编码); }
inline bool 有效(特征比较规则身份 v) noexcept { return 有效(v.编码); }
struct 特征I64闭区间 final {
    std::int64_t 下界{}, 上界{};
    friend bool operator==(const 特征I64闭区间&, const 特征I64闭区间&) = default;
};
using 特征准确值 = std::variant<std::int64_t, 特征值身份>;
struct 特征信息 final {
    特征信息身份 身份;
    特征类型身份 类型;
    特征准确值 准确值;
    friend bool operator==(const 特征信息&, const 特征信息&) = default;
};
inline bool 浅层结构有效(特征I64闭区间 v) noexcept { return v.下界 <= v.上界; }
inline bool 浅层结构有效(const 特征准确值& v) noexcept {
    return !v.valueless_by_exception()
        && (!std::holds_alternative<特征值身份>(v) || 有效(std::get<特征值身份>(v)));
}
inline bool 浅层结构有效(const 特征信息& v) noexcept {
    return 有效(v.身份) && 有效(v.类型) && 浅层结构有效(v.准确值);
}
enum class 特征数据错误 : std::uint8_t {
    未找到 = 0, 未设置 = 1, 入口拒绝 = 2, 类型不相容 = 3, 能力未提供 = 4,
    规则缺失 = 5, 引用冲突 = 6, 算术不可表示 = 7, 并发变化 = 8,
    资源失败 = 9, 内部不一致 = 10, 发布结果未确认 = 11, 前次写入待收敛 = 12,
    旧格式不支持 = 13, 已退出 = 14, 历史材料不可用 = 15, 幂等冲突 = 16, 数量预算不足 = 17
};
template<class T> using 特征数据结果 = std::variant<T, 特征数据错误>;
struct 准确特征读取请求 final {
    std::uint32_t 合同版本 = 1;
    std::uint64_t Gread = 0, H = 0;
    特征信息身份 身份;
    friend bool operator==(const 准确特征读取请求&, const 准确特征读取请求&) = default;
};
struct 准确特征读取事实 final {
    std::uint64_t Gread = 0, H = 0;
    特征信息 信息;
    std::variant<std::int64_t, 特征值信息> 完整值;
    稳定编码 类型关系;
    std::optional<稳定编码> 准确值事实;
    std::uint64_t 创建G = 0;
    std::optional<std::uint64_t> 退出G;
    friend bool operator==(const 准确特征读取事实&, const 准确特征读取事实&) = default;
};
struct I64特征域形成参数 final {
    std::int64_t 允许误差{};
    稳定编码 参数来源;
    friend bool operator==(const I64特征域形成参数&, const I64特征域形成参数&) = default;
};
struct 先天I64特征类型规格 final {
    稳定编码 外设提供者, 单位;
    std::uint64_t 缩放分子{}, 缩放分母{};
    std::vector<特征I64闭区间> 允许集合;
    std::optional<I64特征域形成参数> 域形成;
    friend bool operator==(const 先天I64特征类型规格&, const 先天I64特征类型规格&) = default;
};
struct 先天I64特征类型信息 final {
    特征类型身份 身份;
    先天I64特征类型规格 规格;
    std::optional<特征比较规则身份> 规则;
    friend bool operator==(const 先天I64特征类型信息&, const 先天I64特征类型信息&) = default;
};
struct 特征规范I64域 final {
    std::vector<特征I64闭区间> 区间;
    friend bool operator==(const 特征规范I64域&, const 特征规范I64域&) = default;
};
struct 特征域形成事实 final {
    特征类型身份 类型;
    特征规范I64域 域;
    特征信息身份 准确来源;
    特征比较规则身份 规则;
    std::uint64_t Gread = 0, H = 0;
};
struct 特征类型截止请求 final {
    std::uint32_t 版本 = 1;
    std::uint64_t Gread = 0, H = 0;
    特征类型身份 类型;
};
struct 特征I64域判定请求 final { 特征类型截止请求 类型; 特征规范I64域 域; };
struct 特征I64域包含请求 final { 特征类型截止请求 类型; 特征规范I64域 外, 内; };
struct 准确特征域命中请求 final { 准确特征读取请求 特征; 特征规范I64域 域; };
template<class T> struct 特征截止事实 final { std::uint64_t Gread = 0, H = 0; T 数据; };

inline constexpr std::uint32_t 标量派生合同版本 = 2;
enum class 特征类比较用途 : std::uint8_t { 目标判断 = 1, 状态迁移 = 2 };
enum class 特征类比较角色 : std::uint8_t { 当前事实 = 1, 目标状态 = 2, 前状态 = 3, 后当前事实 = 4 };
struct 特征类定义身份 final {
    稳定编码 结点{};
    friend bool operator==(const 特征类定义身份&, const 特征类定义身份&) = default;
};
struct 特征类比较注册身份 final {
    稳定编码 值{};
    friend bool operator==(const 特征类比较注册身份&, const 特征类比较注册身份&) = default;
};
struct 特征类派生规则 final {
    稳定编码 规则身份{};
    std::uint32_t 规则版本 = 0;
    friend bool operator==(const 特征类派生规则&, const 特征类派生规则&) = default;
};
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
struct 特征类标量基础来源 final {
    特征信息身份 F;
    friend bool operator==(const 特征类标量基础来源&, const 特征类标量基础来源&) = default;
};
struct 特征类标量派生来源 final {
    特征类定义身份 定义;
    特征类标量结果角色 上游输出角色 = 特征类标量结果角色::排序;
    friend bool operator==(const 特征类标量派生来源&, const 特征类标量派生来源&) = default;
};
inline 稳定编码 标量来源编码(const std::variant<特征类标量基础来源, 特征类标量派生来源>& v) {
    return std::visit([](const auto& x) {
        if constexpr (std::is_same_v<std::decay_t<decltype(x)>, 特征类标量基础来源>) return x.F.编码;
        else return x.定义.结点;
    }, v);
}
inline unsigned 标量来源输出角色(const std::variant<特征类标量基础来源, 特征类标量派生来源>& v) {
    if (const auto* x = std::get_if<特征类标量派生来源>(&v)) return static_cast<unsigned>(x->上游输出角色);
    return 0;
}
inline bool 标量来源形状有效(const std::variant<特征类标量基础来源, 特征类标量派生来源>& v) noexcept {
    if (const auto* x = std::get_if<特征类标量基础来源>(&v)) return 有效(x->F);
    if (const auto* x = std::get_if<特征类标量派生来源>(&v)) {
        const auto role = static_cast<unsigned>(x->上游输出角色);
        return 有效(x->定义.结点) && role >= 1 && role <= 3;
    }
    return false;
}
struct 特征类标量派生来源提交项 final {
    std::uint32_t 顺序 = 0;
    特征类比较角色 输入角色 = 特征类比较角色::当前事实;
    std::variant<特征类标量基础来源, 特征类标量派生来源> 来源;
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
    friend bool operator==(const 特征类标量派生退出请求&, const 特征类标量派生退出请求&) = default;
};
struct 旧v1派生治理结构交付 final {
    std::uint32_t 格式 = 1;
    稳定编码 实际阶次属性类型, 派生规则属性类型, 直接来源关系类型, 宿主关系类型,
        比较注册归属关系类型, 比较注册U64属性类型, 比较注册I64属性类型;
};
struct 旧派生治理事实 final {
    特征类定义身份 身份;
    std::uint32_t 实际阶次 = 0;
    std::optional<稳定编码> 宿主;
    特征类比较用途 用途 = 特征类比较用途::目标判断;
    特征类比较角色 左角色 = 特征类比较角色::当前事实, 右角色 = 特征类比较角色::目标状态;
    std::uint64_t 创建G = 0;
    std::optional<std::uint64_t> 退出G;
};
inline bool 旧治理投影完整(const 旧派生治理事实& f, std::uint64_t h) noexcept {
    return h && 有效(f.身份.结点) && f.实际阶次 > 1 && f.创建G && f.创建G <= h
        && (!f.退出G || *f.退出G > h) && (!f.宿主 || 有效(*f.宿主))
        && ((f.用途 == 特征类比较用途::目标判断 && f.左角色 == 特征类比较角色::当前事实
                && f.右角色 == 特征类比较角色::目标状态)
            || (f.用途 == 特征类比较用途::状态迁移 && f.左角色 == 特征类比较角色::前状态
                && f.右角色 == 特征类比较角色::后当前事实));
}
struct 旧派生治理读取结果 final {
    std::uint32_t 版本 = 2;
    特征类标量状态 状态 = 特征类标量状态::入口拒绝;
    std::uint64_t Gread = 0, H = 0;
    std::optional<旧派生治理事实> 事实;
    bool 成功() const noexcept {
        if (版本 != 2 || 状态 != 特征类标量状态::已读取 || !H || H > Gread || !事实) return false;
        return 旧治理投影完整(*事实, H);
    }
};
struct 旧派生治理退出事实 final {
    旧派生治理事实 定义;
    std::vector<稳定编码> 已退出事实;
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
        if (!有效(定义身份.结点) || 真实阶次 <= 1 || !创建G || 创建G > h
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
            if (!有效(x.关系) || !标量来源形状有效(x.内容.来源) || x.内容.顺序 != i + 1
                || static_cast<unsigned>(x.内容.输入角色) != (i ? right : left)) return false;
        }
        if (标量来源编码(来源组[0].内容.来源) == 标量来源编码(来源组[1].内容.来源) || 来源组[0].关系 == 来源组[1].关系) return false;
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
    稳定编码 F{}, FT{};
    std::optional<稳定编码> 值事实;
    std::variant<std::int64_t, 特征值信息> 完整值;
    稳定编码 类型关系;
    std::uint64_t Gread = 0;
    std::int64_t 值 = 0;
    std::uint64_t G = 0, 创建G = 0;
    std::optional<std::uint64_t> 退出G;
    friend bool operator==(const 特征类标量叶回执&, const 特征类标量叶回执&) = default;
};
inline bool 标量叶完整(const 特征类标量叶回执& leaf, std::uint64_t g, std::uint64_t h) noexcept {
    if (!h || g < h || !有效(leaf.F) || !有效(leaf.FT) || !有效(leaf.类型关系)
        || (leaf.值事实 && !有效(*leaf.值事实)) || leaf.G != h || leaf.Gread != g
        || !leaf.创建G || leaf.创建G > h || (leaf.退出G && *leaf.退出G <= h)) return false;
    if (const auto* direct = std::get_if<std::int64_t>(&leaf.完整值)) {
        // 内联值可同时回显真实 L1 属性值事实，该事实不是材料引用表示。
        return *direct == leaf.值;
    }
    const auto* material = std::get_if<特征值信息>(&leaf.完整值);
    if (!material || !leaf.值事实 || material->值身份.编码 != *leaf.值事实) return false;
    const auto* scalar = std::get_if<std::int64_t>(&material->值内容);
    return scalar && *scalar == leaf.值;
}
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
            if (!标量叶完整(x, Gread, H) || (有效(previous) && !(previous < x.F))) return false;
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
    std::optional<特征类标量派生退出请求> 旧治理原请求;
    std::optional<旧派生治理退出事实> 旧治理退出;
    bool 成功() const noexcept {
        if (旧治理原请求 || 旧治理退出) {
            if (定义事实 || !旧治理原请求 || !旧治理退出 || !正式回执 || 版本 != 2
                || !首次发布H || !*首次发布H || Gread < *首次发布H
                || 发布确定性 != 特征类标量发布确定性::确认已发布
                || (状态 != 特征类标量状态::已退出 && 状态 != 特征类标量状态::精确重复)) return false;
            const auto& r = *旧治理原请求; const auto& f = 旧治理退出->定义; const auto& w = *正式回执;
            if (r.版本 != 2 || !r.G || r.G >= *首次发布H || !有效(r.幂等身份) || !r.预算.有效()
                || !旧治理投影完整(f, r.G)
                || w.合同版本 != L1所有者范围CRUD合同版本 || w.写入幂等身份 != r.幂等身份
                || w.事实代次 != *首次发布H || !w.新编码映射.empty()
                || (状态 == 特征类标量状态::已退出
                    ? w.状态 != L1所有者范围写入状态::成功 || !w.是否形成内存权威发布
                    : w.状态 != L1所有者范围写入状态::精确重复 || w.是否形成内存权威发布)
                || f.身份 != r.定义身份 || !f.创建G || f.创建G >= *首次发布H
                || f.退出G != 首次发布H || f.实际阶次 <= 1
                || 旧治理退出->已退出事实.size() != 9 + (f.宿主 ? 1 : 0)) return false;
            稳定编码 previous{}; std::size_t targets = 0;
            for (auto id : 旧治理退出->已退出事实) {
                if (!有效(id) || (有效(previous) && !(previous < id))) return false;
                if (id == r.定义身份.结点) ++targets; previous = id;
            }
            return targets == 1;
        }
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
            || !有效(根定义.结点) || !有效(注册身份.值) || 算法版本 != 1 || 真实阶次 <= 1
            || 实际结果位 == 0 || 实际结果位 > 7 || 基础叶回执组.empty() || !直接输入回执
            || !输入量化.完整() || !误差合同版本 || (误差预算 && *误差预算 < 0) || (相等容差 && *相等容差 < 0)) return false;
        稳定编码 previousLeaf{};
        for (const auto& leaf : 基础叶回执组) {
            if (!标量叶完整(leaf, G, G) || (有效(previousLeaf) && !(previousLeaf < leaf.F))) return false;
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
                    return 有效(x.定义.结点) && 有效(x.输出FT) && x.实际阶次 > 1 && x.实际阶次 < 真实阶次
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


// 定义和准确内容使用两个既有技术分区；本类不保存名称、观察或当前采用。
class 特征类数据服务 final {
    template<class T> using R = 特征数据结果<T>;
    using S = 特征数据错误;
    using N = L1所有者范围节点事实;
    using E = L1所有者范围关系事实;
    using V = L1所有者范围值事实;
    using Ref = L1所有者范围事实引用;
    using Key = L1所有者范围写集本地键;
    using WS = L1所有者范围写集请求;
    enum class 分区 : std::uint8_t { 定义, 信息 };
    enum 定义角色 : std::size_t {
        定义锚点, 定义归属, 类型规格属性, 规则误差属性, 外设来源关系, 单位关系,
        域规则关系, 参数来源关系, 阶次属性, 派生规则属性, 直接来源关系,
        派生宿主关系, 注册归属关系, 注册U64属性, 注册I64属性, 输出FT归属关系, 定义角色数
    };
    enum 信息角色 : std::size_t { 信息锚点, 信息归属, 准确内联属性, 准确引用属性, 准确类型关系, 信息角色数 };
    struct 待确认写入 final {
        分区 区; WS 请求; bool 初始化 = false;
        std::optional<特征类标量派生退出请求> 旧治理;
    };
    struct 待确认标量业务 final {
        bool 旧治理=false;
        std::variant<特征类标量派生建立请求,特征类标量派生退出请求> 原请求;
    };
public:
    特征类数据服务(const L1事实基座服务& l1, L1所有者范围写端口&& definitions,
        L1所有者范围写端口&& information, const 特征值类数据服务& values, 稳定编码 producer)
        : l1_(l1), definitions_(std::move(definitions)), information_(std::move(information)),
          values_(values), producer_(producer) {
        if(!绑定于(l1)||!definitions_.有效()||!information_.有效())
            throw std::invalid_argument("特征数据端口绑定");
    }
    特征类数据服务(const L1事实基座服务& l1, L1所有者范围写端口&& definitions,
        L1所有者范围写端口&& information, const 特征值类数据服务& values, 稳定编码 producer,
        const 旧v1派生治理结构交付& legacy)
        : 特征类数据服务(l1, std::move(definitions), std::move(information), values, producer) {
        legacy_ = legacy;
    }
    特征类数据服务(const 特征类数据服务&) = delete;
    特征类数据服务& operator=(const 特征类数据服务&) = delete;
    bool 绑定于(const L1事实基座服务& l1) const noexcept {
        return &l1 == &l1_ && definitions_.绑定于(l1) && information_.绑定于(l1)
            && values_.绑定于(l1) && definitions_.所有者身份() != information_.所有者身份();
    }
    R<std::monostate> 初始化特征定义结构();
    R<std::monostate> 初始化准确特征结构();
    R<std::monostate> 收敛待确认写入();
    R<特征类型身份> 创建先天I64特征类型(const 先天I64特征类型规格&);
    R<先天I64特征类型信息> 读取先天I64特征类型(特征类型身份) const;
    R<特征信息身份> 创建准确特征(特征类型身份, const 特征准确值&);
    R<特征信息> 读取准确特征(特征信息身份) const;
    R<std::vector<特征信息>> 查询准确特征(特征类型身份, const 特征准确值&) const;
    R<std::monostate> 删除准确特征(特征信息身份);
    R<特征类型身份> 读取准确特征类型(特征信息身份) const;
    R<特征准确值> 读取准确特征值(特征信息身份) const;
    R<准确特征读取事实> 读取准确特征事实(const 准确特征读取请求&) const;
    R<特征截止事实<先天I64特征类型信息>> 读取先天I64特征类型事实(const 特征类型截止请求&) const;
    R<特征截止事实<特征规范I64域>> 读取I64类型完整域(const 特征类型截止请求&) const;
    R<特征域形成事实> 形成I64特征域(const 准确特征读取请求&) const;
    R<特征截止事实<特征规范I64域>> 规范化I64特征域(const 特征I64域判定请求&) const;
    R<特征截止事实<bool>> 判定准确特征命中域(const 准确特征域命中请求&) const;
    R<特征截止事实<bool>> 判定I64域包含(const 特征I64域包含请求&) const;
    旧派生治理读取结果 读取旧派生治理事实(const 特征类标量派生读取请求&) const;
    特征类标量派生写结果 退出旧派生治理定义(const 特征类标量派生退出请求&);
private:
    struct 旧治理布局 {
        旧派生治理事实 事实;
        std::vector<稳定编码> 自有, 来源;
    };
    分区 旧路由(std::uint64_t Gread, std::uint64_t H) const;
    旧治理布局 旧读取闭包(特征类定义身份, std::uint64_t Gread, std::uint64_t H,
        分区, const 特征类派生读取预算&) const;
    static void 要求(bool ok, S s = S::内部不一致) { if (!ok) throw s; }
    template<class T, class F> R<T> 保护(F&& f) const {
        try { std::lock_guard<std::mutex> lock(mutex_); return R<T>{std::in_place_index<0>, f()}; }
        catch (S s) { return s; }
        catch (const std::bad_alloc&) { return S::资源失败; }
        catch (const std::length_error&) { return S::资源失败; }
        catch (...) { return S::内部不一致; }
    }
    static S 映射(L1所有者范围读取状态);
    static S 映射(L1所有者范围写入状态);
    L1所有者范围写端口& 端口(分区 p) { return p == 分区::定义 ? definitions_ : information_; }
    const L1所有者范围写端口& 端口(分区 p) const { return p == 分区::定义 ? definitions_ : information_; }
    std::uint64_t 当前G() const;
    void 守卫(std::uint64_t g) const { 要求(当前G() == g, S::并发变化); }
    static void 截止有效(std::uint32_t v, std::uint64_t g, std::uint64_t h) { 要求(v == 1 && h && g >= h, S::入口拒绝); }
    template<class T> static void 生命周期(const T& f, std::uint64_t g, std::uint64_t h) {
        要求(f.创建事实代次 && f.创建事实代次 <= g
            && (!f.退出事实代次 || (*f.退出事实代次 >= f.创建事实代次 && *f.退出事实代次 <= g)));
        要求(f.创建事实代次 <= h, S::未找到);
        要求(!f.退出事实代次 || *f.退出事实代次 > h, S::已退出);
    }
    L1所有者范围事实副本 原始事实(稳定编码, std::uint64_t) const;
    N 节点(稳定编码, std::uint64_t, std::uint64_t, std::optional<分区> = std::nullopt) const;
    std::vector<E> 关系(稳定编码, 稳定编码, bool, std::uint64_t, std::uint64_t, 分区) const;
    std::vector<V> 属性(稳定编码, std::uint64_t, std::uint64_t, 分区) const;
    E 唯一关系(稳定编码, 稳定编码, std::uint64_t, std::uint64_t, 分区) const;
    static const V& 唯一属性(const std::vector<V>&, 稳定编码);
    void 结构就绪(分区, std::uint64_t, std::uint64_t) const;
    E 核对归属(稳定编码, std::uint64_t, std::uint64_t, 分区) const;
    static 特征规范I64域 规范域(特征规范I64域);
    static bool 包含(const 特征规范I64域&, const 特征规范I64域&);
    static void 检查规格(const 先天I64特征类型规格&);
    struct 标量读取上下文;
    先天I64特征类型信息 读类型(特征类型身份, std::uint64_t, std::uint64_t, 标量读取上下文* = nullptr) const;
    特征规范I64域 读完整域(特征类型身份, std::uint64_t, std::uint64_t) const;
    准确特征读取事实 读准确(特征信息身份, std::uint64_t, std::uint64_t, 标量读取上下文* = nullptr) const;
    std::int64_t 解析输入(const 特征准确值&, std::uint64_t, std::uint64_t) const;
    static std::int64_t 完整整数(const 准确特征读取事实&);
    static Key 新键(const WS&);
    static Key 加节点(WS&, std::optional<L1所有者范围值表示种类> = std::nullopt);
    static Key 加关系(WS&, Ref, Ref, Ref, std::int64_t = 1);
    Key 加值(WS&, Ref, Ref, L1所有者范围原始值材料, std::optional<Ref> = std::nullopt) const;
    WS 新写集(分区, std::uint64_t) const;
    static void 规范化写集(WS&);
    static 稳定编码 映射编码(const L1所有者范围写入结果&, Key);
    static 稳定编码 解析引用(const L1所有者范围写入结果&, const Ref&);
    static WS 初始化写集(分区, std::uint64_t);
    void 接受初始化(分区, const L1所有者范围写入结果&);
    void 确认发布(分区, const WS&, const L1所有者范围写入结果&) const;
    L1所有者范围写入结果 收敛原请求();
    L1所有者范围写入结果 提交(分区, WS, bool = false);
    void 初始化(分区);
    using SS = 特征类标量状态;
    static SS 标量映射(S s) noexcept {
        switch (s) {
        case S::未找到: return SS::未找到;
        case S::已退出: return SS::已退出;
        case S::历史材料不可用: return SS::历史材料不可用;
        case S::并发变化: return SS::事实代次漂移;
        case S::资源失败: return SS::资源失败;
        case S::入口拒绝: return SS::入口拒绝;
        case S::旧格式不支持: return SS::格式不支持;
        case S::能力未提供: return SS::算法不支持;
        case S::类型不相容: return SS::类型不匹配;
        case S::引用冲突: return SS::引用冲突;
        case S::幂等冲突: return SS::幂等冲突;
        case S::数量预算不足: return SS::预算不足;
        case S::算术不可表示: return SS::运算溢出;
        case S::发布结果未确认: case S::前次写入待收敛: return SS::已可能发布;
        default: return SS::内部不一致;
        }
    }
    static std::optional<稳定编码> 标量查找编码(const L1所有者范围写入结果& r, Key k) noexcept {
        std::optional<稳定编码> out;
        for (const auto& [key, id] : r.新编码映射) if (key == k) {
            if (out || !有效(id)) return std::nullopt; out = id;
        } return out;
    }
    bool 写入结果头完整(const L1所有者范围写入结果& r, L1所有者范围写入幂等身份 key) const noexcept {
        return r.合同版本 == L1所有者范围CRUD合同版本 && r.所有者 == definitions_.所有者身份()
            && r.写入幂等身份 == key && r.事实代次
            && ((r.状态 == L1所有者范围写入状态::成功 && r.是否形成内存权威发布
                    && r.重试边界 == L1所有者范围重试边界::不适用)
                || (r.状态 == L1所有者范围写入状态::精确重复 && !r.是否形成内存权威发布
                    && r.重试边界 == L1所有者范围重试边界::原幂等身份读回收敛));
    }
    static std::uint64_t I64差异幅度(std::int64_t a, std::int64_t b) noexcept {
        const auto ua = static_cast<std::uint64_t>(a), ub = static_cast<std::uint64_t>(b);
        return a < b ? ub - ua : ua - ub;
    }
    static bool I64差异可表示(std::int64_t a, std::int64_t b) noexcept {
        return (a >= 0 || b <= std::numeric_limits<std::int64_t>::max() + a)
            && (a <= 0 || b >= std::numeric_limits<std::int64_t>::min() + a);
    }
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
        const auto r = l1_.读取中性当前事实代次({L1中性CRUD合同版本});
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
        const auto r = l1_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, id});
        if (r.状态 != L1所有者范围读取状态::成功) {
            标量守卫(c.G);
            throw 标量失败{标量映射(r.状态)};
        }
        标量要求(r.读取事实代次 == c.G, SS::事实代次漂移);
        const auto* n = r.事实 ? std::get_if<SN>(&*r.事实) : nullptr;
        标量要求(r.合同版本 == L1所有者范围CRUD合同版本 && r.查询编码 == id
            && n && n->编码 == id && (!own || n->写入所有者 == definitions_.所有者身份()));
        标量要求(标量活动(*n, c.H), n->退出事实代次 && *n->退出事实代次 <= c.H ? SS::已退出 : SS::未找到);
        return *n;
    }
    SV 标量属性(const SN& n, 稳定编码 type, 标量读取上下文& c) const {
        const auto values = 属性(n.编码, c.G, c.H, 分区::定义);
        const auto& v = 唯一属性(values, type);
        标量计数(c.值计数, v.编码, c.预算.最大属性值数);
        标量要求(v.来源节点 == n.编码 && v.创建事实代次 == n.创建事实代次 && v.退出事实代次 == n.退出事实代次);
        return v;
    }
    std::vector<SE> 标量关系(稳定编码 id, 稳定编码 type, bool incoming, 标量读取上下文& c) const {
        const auto direction = incoming ? L1所有者范围关系端点方向::目标 : L1所有者范围关系端点方向::源;
        const auto r = l1_.读取所有者范围历史关系组({L1所有者范围CRUD合同版本, direction, id, type, c.H});
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
            标量要求(有效(e.编码) && e.写入所有者 == definitions_.所有者身份() && e.关系类型节点 == type
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
        标量要求(r.来源组.size() == 2 && !r.来源组[0].来源.valueless_by_exception()
            && !r.来源组[1].来源.valueless_by_exception()
            && 标量来源编码(r.来源组[0].来源) != 标量来源编码(r.来源组[1].来源), SS::来源不匹配);
        for (unsigned i = 0; i < 2; ++i) {
            const auto& x = r.来源组[i];
            标量要求(x.顺序 == i + 1 && 标量来源形状有效(x.来源)
                && x.输入角色 == (i ? r.标量注册.右角色 : r.标量注册.左角色), SS::来源不匹配);
        }
    }
    static std::pair<std::vector<std::uint64_t>, std::vector<std::int64_t>> 标量编码(
        const 特征类标量派生建立请求& r) {
        const auto& x = r.标量注册; const auto& q = x.输入量化;
        std::vector<std::uint64_t> u{2, static_cast<std::uint64_t>(x.用途), 2, x.算法版本, 1,
            static_cast<std::uint64_t>(x.左角色), static_cast<std::uint64_t>(x.右角色), 1,
            q.单位.值, q.维度.值, q.分量角色.值, x.允许结果位, 1, 1, x.误差合同版本,
            r.派生规则.规则身份.值, 标量来源输出角色(r.来源组[0].来源), 标量来源输出角色(r.来源组[1].来源),
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
        for (unsigned i = 0; i < 2; ++i) {
            const auto source = 标量来源编码(d.来源组[i].内容.来源);
            if (u[16 + i]) d.来源组[i].内容.来源 = 特征类标量派生来源{{source}, static_cast<特征类标量结果角色>(u[16 + i])};
            else d.来源组[i].内容.来源 = 特征类标量基础来源{{source}};
        }
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
        标量要求(有效(d_[输出FT归属关系]), SS::格式不支持);
        标量计数(c.定义计数, id, c.预算.最大定义数);
        const auto n = 标量节点(id, c);
        标量计数(c.关系计数, 核对归属(id, c.G, c.H, 分区::定义).编码, c.预算.最大关系数);
        标量要求(n.种类 == 节点种类::普通 && !n.属性类型表示 && n.当前属性.size() == 2, SS::类型不匹配);
        const auto order = 标量属性(n, d_[阶次属性], c);
        const auto rule = 标量属性(n, d_[派生规则属性], c);
        const auto* level = std::get_if<std::int64_t>(&order.材料);
        const auto* rules = std::get_if<std::vector<std::uint64_t>>(&rule.材料);
        标量要求(level && *level > 1 && static_cast<std::uint64_t>(*level) <= UINT32_MAX
            && rules && rules->size() == 2 && (*rules)[0] && (*rules)[1] && (*rules)[1] <= UINT32_MAX);
        特征类标量派生事实 d;
        d.定义身份 = {id}; d.真实阶次 = static_cast<std::uint32_t>(*level);
        d.阶次值 = order.编码; d.规则值 = rule.编码;
        d.派生规则 = {{(*rules)[0]}, static_cast<std::uint32_t>((*rules)[1])};
        d.创建G = n.创建事实代次; d.退出G = n.退出事实代次;
        (void)标量节点(d.派生规则.规则身份, c, false);
        const auto sources = 标量关系(id, d_[直接来源关系], false, c);
        const auto host = 标量关系(id, d_[派生宿主关系], false, c);
        const auto reg = 标量关系(id, d_[注册归属关系], false, c);
        标量要求(sources.size() == 2 && host.size() <= 1 && reg.size() == 1);
        auto sameLife = [&](const SE& e) { 标量要求(e.创建事实代次 == n.创建事实代次 && e.退出事实代次 == n.退出事实代次); };
        for (unsigned i = 0; i < 2; ++i) {
            const auto& e = sources[i]; sameLife(e);
            const auto bits = static_cast<std::uint64_t>(e.角色或顺序);
            标量要求(bits >> 32U == i + 1 && static_cast<std::uint32_t>(bits) != 0);
            d.来源组.push_back({{i + 1, static_cast<特征类比较角色>(static_cast<std::uint32_t>(bits)), 特征类标量基础来源{{e.目标节点}}}, e.编码});
        }
        标量要求(标量来源编码(d.来源组[0].内容.来源) != 标量来源编码(d.来源组[1].内容.来源));
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
        const auto uv = 标量属性(rn, d_[注册U64属性], c);
        const auto iv = 标量属性(rn, d_[注册I64属性], c);
        const auto* u = std::get_if<std::vector<std::uint64_t>>(&uv.材料);
        const auto* v = std::get_if<std::vector<std::int64_t>>(&iv.材料);
        标量要求(u && v); 标量解码(*u, *v, d);
        d.注册身份 = {rn.编码}; d.注册归属 = reg[0].编码; d.注册U64值 = uv.编码; d.注册I64值 = iv.编码;
        const auto outputs = 标量关系(id, d_[输出FT归属关系], false, c);
        标量要求(outputs.size() == d.注册.输出组.size());
        for (std::size_t i = 0; i < outputs.size(); ++i) {
            const auto& e = outputs[i]; sameLife(e);
            标量要求(e.角色或顺序 == static_cast<std::int64_t>(d.注册.输出组[i].角色));
            const auto ft = 标量节点(e.目标节点, c);
            标量计数(c.关系计数, 核对归属(ft.编码, c.G, c.H, 分区::定义).编码, c.预算.最大关系数);
            标量要求(ft.种类 == 节点种类::属性类型 && ft.属性类型表示 == L1所有者范围值表示种类::I64
                && ft.当前属性.size() == 1 && ft.创建事实代次 == n.创建事实代次 && ft.退出事实代次 == n.退出事实代次);
            const auto marker = 标量属性(ft, ft.编码, c);
            const auto* value = std::get_if<std::int64_t>(&marker.材料);
            标量要求(value && *value == 标量格式标记);
            const auto reverse = 标量关系(ft.编码, d_[输出FT归属关系], true, c);
            标量要求(reverse.size() == 1 && reverse[0] == e);
            d.输出组.push_back({d.注册.输出组[i], ft.编码, e.编码, marker.编码});
        }
        for (unsigned i = 0; i < 2; ++i)
            标量要求(d.来源组[i].内容.输入角色 == (i ? d.注册.右角色 : d.注册.左角色));
        return d;
    }
    特征类标量叶回执 标量读叶(稳定编码 id, 标量读取上下文& c) const {
        标量计数(c.叶计数, id, c.预算.最大叶数);
        const auto f = 读准确({id}, c.G, c.H, &c);
        特征类标量叶回执 out;
        out.F = id; out.FT = f.信息.类型.编码; out.值事实 = f.准确值事实;
        out.完整值 = f.完整值; out.类型关系 = f.类型关系; out.Gread = c.G;
        out.值 = 完整整数(f); out.G = c.H; out.创建G = f.创建G; out.退出G = f.退出G;
        return out;
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
                    if (std::holds_alternative<特征类标量派生来源>(x.来源)) {
                        const auto& child = c.定义.at(标量来源编码(x.来源));
                        const auto& output = 标量输出(child, 标量来源输出角色(x.来源));
                        标量要求(标量同量纲(output.声明.量化, d.注册.输入量化), SS::单位量化不匹配);
                        maxOrder = std::max(maxOrder, child.真实阶次);
                    } else { 标量要求(c.叶.contains(标量来源编码(x.来源)), SS::来源不匹配); maxOrder = std::max(maxOrder, 1U); }
                    const auto& set = c.闭包.at(标量来源编码(x.来源)); leaves.insert(set.begin(), set.end());
                    height = std::max(height, c.高度.at(标量来源编码(x.来源)));
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
                stack.push_back({标量来源编码(it->内容.来源), std::holds_alternative<特征类标量派生来源>(it->内容.来源), false, f.depth + 1});
            c.定义.emplace(f.id, std::move(d));
        }
    }
    特征类标量派生读取结果 标量读取结果(稳定编码 id, 标量读取上下文& c) const {
        特征类标量派生读取结果 out;
        out.状态 = SS::已读取; out.Gread = c.G; out.H = c.H; out.定义事实 = c.定义.at(id);
        for (const auto& leaf : c.闭包.at(id)) out.基础叶组.push_back(c.叶.at(leaf));
        const auto& d = *out.定义事实;
        const auto& left = c.闭包.at(标量来源编码(d.来源组[0].内容.来源));
        const auto& right = c.闭包.at(标量来源编码(d.来源组[1].内容.来源));
        out.左叶组.assign(left.begin(), left.end()); out.右叶组.assign(right.begin(), right.end());
        return out;
    }
public:
    特征类标量派生读取结果 读取标量派生定义(const 特征类标量派生读取请求& r) const {
        特征类标量派生读取结果 out;
        auto fail = [&](SS e) { 特征类标量派生读取结果 f; f.状态 = e; f.Gread = out.Gread; f.H = r.H; return f; };
        try {
            std::lock_guard<std::mutex> lock(mutex_);
            标量要求(r.版本 == 2 && r.H && r.Gread >= r.H && 有效(r.定义身份.结点) && r.预算.有效(), SS::入口拒绝);
            标量守卫(r.Gread); out.Gread = r.Gread;
            标量读取上下文 c{r.Gread, r.H, r.预算};
            标量展开(r.定义身份.结点, true, c);
            out = 标量读取结果(r.定义身份.结点, c); 标量守卫(r.Gread);
            标量要求(out.成功()); return out;
        } catch (const 标量失败& e) { return fail(e.状态); }
        catch (const std::bad_alloc&) { return fail(SS::资源失败); }
        catch (const std::length_error&) { return fail(SS::资源失败); }
        catch (S e) { return fail(标量映射(e)); }
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
        WS w; w.期望事实代次 = r.G; w.写入幂等身份 = r.幂等身份;
        w.节点 = {{SK{1}, 节点种类::普通, std::nullopt}, {SK{0x30001}, 节点种类::普通, std::nullopt}};
        const auto [u, v] = 标量编码(r);
        w.值 = {{SK{2}, SK{1}, d_[阶次属性], std::int64_t{level}, SK{1}},
            {SK{3}, SK{1}, d_[派生规则属性], std::vector<std::uint64_t>{r.派生规则.规则身份.值,r.派生规则.规则版本}, SK{1}},
            {SK{0x30003}, SK{0x30001}, d_[注册U64属性], u, SK{0x30001}},
            {SK{0x30004}, SK{0x30001}, d_[注册I64属性], v, SK{0x30001}}};
        w.属性槽变更 = {{SK{1}, d_[阶次属性], SK{2}}, {SK{1}, d_[派生规则属性], SK{3}},
            {SK{0x30001}, d_[注册U64属性], SK{0x30003}}, {SK{0x30001}, d_[注册I64属性], SK{0x30004}}};
        w.关系.push_back({SK{0x30002}, SK{1}, SK{0x30001}, d_[注册归属关系], 1});
        w.关系.push_back({SK{0x50001}, SK{1}, d_[定义锚点], d_[定义归属], 1});
        for (const auto& x : r.来源组)
            w.关系.push_back({SK{0x10000 + x.顺序}, SK{1}, 标量来源编码(x.来源), d_[直接来源关系],
                static_cast<std::int64_t>((std::uint64_t{x.顺序} << 32) | static_cast<std::uint32_t>(x.输入角色))});
        if (r.宿主E) w.关系.push_back({SK{0x20001}, SK{1}, *r.宿主E, d_[派生宿主关系], 1});
        for (const auto& x : r.标量注册.输出组) {
            const auto role = static_cast<std::uint32_t>(x.角色);
            const SK ft{0x40000U + role}, edge{0x41000U + role}, marker{0x42000U + role};
            w.节点.push_back({ft, 节点种类::属性类型, L1所有者范围值表示种类::I64});
            w.关系.push_back({edge, SK{1}, ft, d_[输出FT归属关系], static_cast<std::int64_t>(role)});
            w.关系.push_back({SK{0x51000U + role}, ft, d_[定义锚点], d_[定义归属], 1});
            w.值.push_back({marker, ft, ft, 标量格式标记, ft});
            w.属性槽变更.push_back({ft, ft, marker});
        }
        标量排序写集(w); return w;
    }
    L1所有者范围写集请求 标量退出写集(const 特征类标量派生退出请求& r,
        const 特征类标量派生事实& d, std::uint64_t g) const {
        L1所有者范围写集请求 w;
        w.合同版本 = L1所有者范围CRUD合同版本; w.期望事实代次 = r.G; w.写入幂等身份 = r.幂等身份;
        w.退出事实 = {d.定义身份.结点, d.注册身份.值, d.阶次值, d.规则值,
            d.注册U64值, d.注册I64值, d.注册归属};
        for (const auto& x : d.来源组) w.退出事实.push_back(x.关系);
        if (d.宿主关系) w.退出事实.push_back(*d.宿主关系);
        // L1 退出节点时保存完整历史属性；不能再向退出节点提交槽变更。
        for (const auto& x : d.输出组) {
            w.退出事实.insert(w.退出事实.end(), {x.特征类型, x.归属关系, x.格式标记值事实});
        }
        w.退出事实.push_back(核对归属(d.定义身份.结点, g, r.G, 分区::定义).编码);
        for (const auto& x : d.输出组) w.退出事实.push_back(核对归属(x.特征类型, g, r.G, 分区::定义).编码);
        标量排序写集(w); return w;
    }
    std::uint32_t 标量准入来源(const 特征类标量派生建立请求& r, 标量读取上下文& c) const {
        std::uint32_t maximum = 0; std::uint64_t height = 0;
        for (const auto& x : r.来源组) {
            标量展开(标量来源编码(x.来源), std::holds_alternative<特征类标量派生来源>(x.来源), c);
            if (std::holds_alternative<特征类标量派生来源>(x.来源)) {
                const auto& d = c.定义.at(标量来源编码(x.来源));
                const auto& o = 标量输出(d, 标量来源输出角色(x.来源));
                标量要求(标量同量纲(o.声明.量化, r.标量注册.输入量化), SS::单位量化不匹配);
                const auto computed = 标量复算(标量来源编码(x.来源), 1U << (标量来源输出角色(x.来源) - 1), c);
                const auto value = computed.值.at(标量来源编码(x.来源)).at(标量来源输出角色(x.来源)).值;
                标量要求(value >= r.标量注册.输入量化.下界 && value <= r.标量注册.输入量化.上界, SS::结果范围不满足);
                maximum = std::max(maximum, d.真实阶次);
            } else {
                const auto value = c.叶.at(标量来源编码(x.来源)).值;
                标量要求(value >= r.标量注册.输入量化.下界 && value <= r.标量注册.输入量化.上界, SS::结果范围不满足);
                maximum = std::max(maximum, 1U);
            }
            height = std::max(height, c.高度.at(标量来源编码(x.来源)));
        }
        标量要求(maximum != UINT32_MAX, SS::运算溢出);
        标量要求(height < r.预算.最大深度, SS::预算不足);
        const auto n = r.标量注册.输出组.size();
        标量要求(c.定义计数.size() < r.预算.最大定义数
            && 4 + n <= r.预算.最大属性值数 - c.值计数.size()
            && 4 + (r.宿主E ? 1U : 0U) + 2 * n <= r.预算.最大关系数 - c.关系计数.size(), SS::预算不足);
        if (r.宿主E) {
            const auto e = 标量节点(*r.宿主E, c, false);
            标量要求(e.种类 == 节点种类::普通 && !e.属性类型表示, SS::来源不匹配);
        }
        return maximum + 1;
    }
    void 标量映射互证(const L1所有者范围写入结果& receipt, const 特征类标量派生事实& d) const {
        auto id = [&](std::uint32_t k) {
            const auto x = 标量查找编码(receipt, SK{k});
            标量要求(x.has_value()); return *x;
        };
        标量要求(receipt.新编码映射.size() == 10 + (d.宿主E ? 1U : 0U) + 4 * d.输出组.size()
            && id(1) == d.定义身份.结点 && id(2) == d.阶次值 && id(3) == d.规则值
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
        std::lock_guard<std::mutex> lock(mutex_);
        bool keyUnknown = false, dispatched = false, prior = false, ownsRecovery=false;
        auto fail = [&](SS e) {
            out.定义事实.reset();
            if(e==SS::幂等冲突) {
                out.发布确定性=SP::确认未发布;out.状态=e;
                if(ownsRecovery)scalar_pending_.reset();
            } else if (dispatched || prior || keyUnknown) {
                out.发布确定性 = SP::可能已发布;
                out.状态 = SS::已可能发布;
            } else { out.状态 = e;if(ownsRecovery)scalar_pending_.reset(); }
            return std::move(out);
        };
        try {
            if constexpr (create) 标量建立形状(r);
            else 标量要求(r.版本 == 2 && r.G && 有效(r.幂等身份) && 有效(r.定义身份.结点) && r.预算.有效(), SS::入口拒绝);
            标量要求(有效(d_[输出FT归属关系]), SS::格式不支持);
            标量要求(!pending_, SS::已可能发布);
            if(scalar_pending_) {
                const auto* original=std::get_if<R>(&scalar_pending_->原请求);
                标量要求(!scalar_pending_->旧治理&&original&&*original==r,SS::已可能发布);
            }
            out.Gread = 标量当前G(); 结构就绪(分区::定义, out.Gread, out.Gread);
            if(!scalar_pending_)scalar_pending_.emplace(待确认标量业务{false,r});
            ownsRecovery=true;keyUnknown=true;
            const auto first = definitions_.读取首次写入材料({L1所有者范围首次写入读取合同版本, r.幂等身份});
            L1所有者范围写集请求 w;
            稳定编码 target{};
            std::uint64_t readH = r.G;
            if (first.状态 == L1所有者范围读取状态::成功) {
                标量要求(first.合同版本 == L1所有者范围首次写入读取合同版本
                    && first.所有者 == definitions_.所有者身份() && first.写入幂等身份 == r.幂等身份
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
                    const auto id = 标量查找编码(saved, SK{1});
                    标量要求(id.has_value()); target = *id;
                    标量展开(target, true, c);
                    w = 标量建立写集(r, c.定义.at(target).真实阶次);
                    标量映射互证(saved, c.定义.at(target));
                } else {
                    target = r.定义身份.结点; 标量展开(target, true, c);
                    w = 标量退出写集(r, c.定义.at(target), c.G);
                    标量要求(saved.新编码映射.empty());
                }
                标量要求(w == *first.首次规范化写集, SS::幂等冲突);
            } else if (first.状态 == L1所有者范围读取状态::未找到) {
                标量要求(first.合同版本 == L1所有者范围首次写入读取合同版本
                    && first.所有者 == definitions_.所有者身份() && first.写入幂等身份 == r.幂等身份);
                keyUnknown = false; out.发布确定性 = SP::确认未发布;
                标量守卫(r.G); out.Gread = r.G;
                标量读取上下文 c{r.G, r.G, r.预算};
                if constexpr (create) w = 标量建立写集(r, 标量准入来源(r, c));
                else {
                    target = r.定义身份.结点; 标量展开(target, true, c);
                    const auto& d = c.定义.at(target);
                    // 下游定义与输出类型上的真实外部引用仍由 L1 原子闭包守卫最终裁决。
                    标量要求(标量关系(target, d_[直接来源关系], true, c).empty(), SS::引用冲突);
                    w = 标量退出写集(r, d, c.G);
                }
                标量守卫(r.G);
            } else { throw 标量失败{标量映射(first.状态)}; }
            dispatched = true;
            auto receipt = definitions_.提交所有者范围中性写集(w);
            out.正式回执 = std::move(receipt);
            const auto& committed = *out.正式回执;
            const bool ok = committed.状态 == L1所有者范围写入状态::成功 || committed.状态 == L1所有者范围写入状态::精确重复;
            if (!ok) {
                if (!prior && (committed.状态 == L1所有者范围写入状态::事实代次漂移
                        || committed.状态 == L1所有者范围写入状态::引用冲突
                        || committed.状态 == L1所有者范围写入状态::入口拒绝)
                    && !committed.是否形成内存权威发布) {
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
                const auto id = 标量查找编码(committed, SK{1});
                标量要求(id.has_value()); target = *id; readH = *out.首次发布H;
            }
            标量读取上下文 c{out.Gread, readH, r.预算};
            标量展开(target, true, c);
            auto d = c.定义.at(target);
            if constexpr (create) {
                标量映射互证(committed, d);
                标量要求(标量建立写集(r, d.真实阶次) == w);
            } else {
                标量要求(committed.新编码映射.empty() && d.退出G == out.首次发布H && 标量退出写集(r, d, c.G) == w);
            }
            标量守卫(out.Gread);
            out.定义事实 = std::move(d); out.发布确定性 = SP::确认已发布;
            out.状态 = committed.状态 == L1所有者范围写入状态::精确重复 ? SS::精确重复 : create ? SS::已创建 : SS::已退出;
            标量要求(out.成功()); scalar_pending_.reset();return out;
        } catch (const 标量失败& e) { return fail(e.状态); }
        catch (const std::bad_alloc&) { return fail(SS::资源失败); }
        catch (const std::length_error&) { return fail(SS::资源失败); }
        catch (S e) { return fail(标量映射(e)); }
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
                if (std::holds_alternative<特征类标量派生来源>(x.内容.来源)) needed[标量来源编码(x.内容.来源)] |= 1U << (标量来源输出角色(x.内容.来源) - 1);
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
                if (std::holds_alternative<特征类标量基础来源>(x.来源)) {
                    const auto& leaf = c.叶.at(标量来源编码(x.来源)); values[i] = leaf.值;
                    inputs[i] = 特征类标量基础输入回执{leaf, role};
                } else {
                    const auto& value = cache.值.at(标量来源编码(x.来源)).at(标量来源输出角色(x.来源));
                    标量要求(标量同量纲(value.量化, r.输入量化), SS::单位量化不匹配);
                    values[i] = value.值;
                    inputs[i] = 特征类标量上游输入回执{std::get<特征类标量派生来源>(x.来源).定义, value.输出FT, value.角色,
                        value.值, c.定义.at(标量来源编码(x.来源)).真实阶次, role};
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
            std::lock_guard<std::mutex> lock(mutex_);
            标量要求(r.版本 == 2 && r.G && r.请求身份 && 有效(r.根定义.结点) && r.预算.有效()
                && r.要求结果位 >= 1 && r.要求结果位 <= 7, SS::入口拒绝);
            标量守卫(r.G); out.G = r.G;
            标量读取上下文 c{r.G, r.G, r.预算}; 标量展开(r.根定义.结点, true, c);
            const auto& d = c.定义.at(r.根定义.结点);
            标量要求(r.用途 == d.注册.用途, SS::来源不匹配);
            标量要求(!r.预期算法版本 || *r.预期算法版本 == d.注册.算法版本, SS::算法不支持);
            标量要求((r.要求结果位 & ~d.注册.允许结果位) == 0, SS::入口拒绝);
            const auto cache = 标量复算(r.根定义.结点, r.要求结果位, c);
            out.状态 = SS::已比较; out.拒绝原因 = SS::已比较; out.请求身份 = r.请求身份;
            out.根定义 = r.根定义; out.注册身份 = d.注册身份; out.算法版本 = d.注册.算法版本;
            out.真实阶次 = d.真实阶次; out.实际结果位 = r.要求结果位;
            out.输入量化 = d.注册.输入量化; out.误差合同版本 = d.注册.误差合同版本;
            out.误差预算 = d.注册.误差预算; out.相等容差 = d.注册.相等容差;
            out.直接输入回执 = cache.输入.at(r.根定义.结点);
            for (const auto& id : c.闭包.at(r.根定义.结点)) out.基础叶回执组.push_back(c.叶.at(id));
            for (const auto& [_, value] : cache.值.at(r.根定义.结点)) out.结果组.push_back(value);
            标量守卫(r.G); 标量要求(out.成功()); return out;
        } catch (const 标量失败& e) { return fail(e.状态); }
        catch (const std::bad_alloc&) { return fail(SS::资源失败); }
        catch (const std::length_error&) { return fail(SS::资源失败); }
        catch (S e) { return fail(标量映射(e)); }
        catch (...) { return fail(SS::内部不一致); }
    }
private:
    const L1事实基座服务& l1_;
    L1所有者范围写端口 definitions_, information_;
    const 特征值类数据服务& values_;
    稳定编码 producer_;
    std::array<稳定编码, 定义角色数> d_{};
    std::array<稳定编码, 信息角色数> f_{};
    bool definition_ready_ = false, information_ready_ = false;
    std::optional<旧v1派生治理结构交付> legacy_;
    std::optional<待确认写入> pending_;
    std::optional<待确认标量业务> scalar_pending_;
    mutable std::mutex mutex_;
};

inline 特征数据错误 特征类数据服务::映射(L1所有者范围读取状态 s) {
    switch (s) {
    case L1所有者范围读取状态::未找到: return S::未找到;
    case L1所有者范围读取状态::已退出: return S::已退出;
    case L1所有者范围读取状态::入口拒绝: return S::入口拒绝;
    case L1所有者范围读取状态::事实代次漂移: return S::并发变化;
    case L1所有者范围读取状态::历史材料已清理: return S::历史材料不可用;
    case L1所有者范围读取状态::资源失败: return S::资源失败;
    default: return S::内部不一致;
    }
}
inline 特征数据错误 特征类数据服务::映射(L1所有者范围写入状态 s) {
    switch (s) {
    case L1所有者范围写入状态::未找到: return S::未找到;
    case L1所有者范围写入状态::已退出: return S::已退出;
    case L1所有者范围写入状态::入口拒绝: return S::入口拒绝;
    case L1所有者范围写入状态::引用冲突: return S::引用冲突;
    case L1所有者范围写入状态::事实代次漂移: return S::并发变化;
    case L1所有者范围写入状态::幂等冲突: return S::幂等冲突;
    case L1所有者范围写入状态::资源失败: return S::资源失败;
    default: return S::内部不一致;
    }
}
inline std::uint64_t 特征类数据服务::当前G() const {
    要求(绑定于(l1_) && definitions_.有效() && information_.有效());
    auto r = l1_.读取中性当前事实代次({L1中性CRUD合同版本});
    if (r.状态 != L1中性读取状态::成功)
        throw r.状态 == L1中性读取状态::资源失败 ? S::资源失败 : S::内部不一致;
    要求(r.合同版本 == L1中性CRUD合同版本 && r.事实代次 != 0); return r.事实代次;
}
inline L1所有者范围事实副本 特征类数据服务::原始事实(稳定编码 id, std::uint64_t g) const {
    要求(有效(id), S::入口拒绝);
    auto r = l1_.读取所有者范围历史事实({L1所有者范围CRUD合同版本, id});
    要求(r.读取事实代次 == g, S::并发变化);
    if (r.状态 != L1所有者范围读取状态::成功) throw 映射(r.状态);
    要求(r.合同版本 == L1所有者范围CRUD合同版本 && r.查询编码 == id && r.事实
        && !r.物理清理墓碑 && !r.物理清理事实代次);
    std::visit([&](const auto& f) { 要求(f.编码 == id); }, *r.事实); return *r.事实;
}
inline 特征类数据服务::N 特征类数据服务::节点(稳定编码 id, std::uint64_t g,
    std::uint64_t h, std::optional<分区> p) const {
    auto raw = 原始事实(id, g); const auto* n = std::get_if<N>(&raw);
    要求(n != nullptr, S::类型不相容); 生命周期(*n, g, h);
    if (p) 要求(n->写入所有者 == 端口(*p).所有者身份(), S::引用冲突); return *n;
}
inline std::vector<特征类数据服务::E> 特征类数据服务::关系(稳定编码 id, 稳定编码 type,
    bool incoming, std::uint64_t g, std::uint64_t h, 分区 p) const {
    auto direction = incoming ? L1所有者范围关系端点方向::目标 : L1所有者范围关系端点方向::源;
    auto r = l1_.读取所有者范围历史关系组({L1所有者范围CRUD合同版本, direction, id, type, h});
    要求(r.读取事实代次 == g, S::并发变化);
    if (r.状态 != L1所有者范围读取状态::成功) throw 映射(r.状态);
    要求(r.合同版本 == L1所有者范围CRUD合同版本 && r.方向 == direction
        && r.端点节点 == id && r.关系类型节点 == type && r.历史截止事实代次 == h);
    std::set<std::uint64_t> seen;
    for (const auto& e : r.关系组) {
        生命周期(e, g, h);
        要求(有效(e.编码) && e.关系类型节点 == type && (incoming ? e.目标节点 : e.源节点) == id
            && e.写入所有者 == 端口(p).所有者身份() && seen.insert(e.编码.值).second);
    }
    std::sort(r.关系组.begin(), r.关系组.end(), [](const E& a, const E& b) {
        return a.角色或顺序 != b.角色或顺序 ? a.角色或顺序 < b.角色或顺序 : a.编码 < b.编码;
    }); return r.关系组;
}
inline std::vector<特征类数据服务::V> 特征类数据服务::属性(稳定编码 id,
    std::uint64_t g, std::uint64_t h, 分区 p) const {
    auto r = l1_.读取所有者范围历史属性值组({L1所有者范围CRUD合同版本, id, h});
    要求(r.读取事实代次 == g, S::并发变化);
    if (r.状态 != L1所有者范围读取状态::成功) throw 映射(r.状态);
    要求(r.合同版本 == L1所有者范围CRUD合同版本 && r.所属节点 == id && r.历史截止事实代次 == h);
    std::set<std::uint64_t> slots;
    for (const auto& v : r.属性值组) {
        生命周期(v, g, h);
        要求(有效(v.编码) && v.所属节点 == id && 有效(v.来源节点)
            && v.写入所有者 == 端口(p).所有者身份() && slots.insert(v.属性类型节点.值).second
            && L1所有者范围原始材料完整(v.材料));
        (void)节点(v.来源节点, g, h);
    } return r.属性值组;
}
inline 特征类数据服务::E 特征类数据服务::唯一关系(稳定编码 id, 稳定编码 type,
    std::uint64_t g, std::uint64_t h, 分区 p) const {
    auto es = 关系(id, type, false, g, h, p);
    要求(es.size() == 1 && es.front().角色或顺序 == 1, S::旧格式不支持); return es.front();
}
inline const 特征类数据服务::V& 特征类数据服务::唯一属性(const std::vector<V>& values, 稳定编码 type) {
    auto it = std::find_if(values.begin(), values.end(), [&](const V& v) { return v.属性类型节点 == type; });
    要求(it != values.end(), S::旧格式不支持); return *it;
}
inline void 特征类数据服务::结构就绪(分区 p, std::uint64_t g, std::uint64_t h) const {
    要求(definition_ready_ && (p == 分区::定义 || information_ready_), S::未设置);
    if (p == 分区::定义) { for (auto id : d_) (void)节点(id, g, h, p); }
    else { for (auto id : f_) (void)节点(id, g, h, p); }
}
inline 特征类数据服务::E 特征类数据服务::核对归属(稳定编码 id, std::uint64_t g,
    std::uint64_t h, 分区 p) const {
    结构就绪(p, g, h);
    auto e = 唯一关系(id, p == 分区::定义 ? d_[定义归属] : f_[信息归属], g, h, p);
    要求(e.目标节点 == (p == 分区::定义 ? d_[定义锚点] : f_[信息锚点])
        && e.源节点 != e.目标节点, S::旧格式不支持); return e;
}
inline 特征规范I64域 特征类数据服务::规范域(特征规范I64域 domain) {
    要求(!domain.区间.empty(), S::入口拒绝);
    for (auto x : domain.区间) 要求(浅层结构有效(x), S::入口拒绝);
    std::sort(domain.区间.begin(), domain.区间.end(), [](auto a, auto b) {
        return a.下界 != b.下界 ? a.下界 < b.下界 : a.上界 < b.上界;
    });
    std::vector<特征I64闭区间> out;
    for (auto x : domain.区间) {
        if (!out.empty() && (x.下界 <= out.back().上界
            || (out.back().上界 != std::numeric_limits<std::int64_t>::max() && x.下界 == out.back().上界 + 1)))
            out.back().上界 = std::max(out.back().上界, x.上界);
        else out.push_back(x);
    } return {std::move(out)};
}
inline bool 特征类数据服务::包含(const 特征规范I64域& outer, const 特征规范I64域& inner) {
    std::size_t i = 0;
    for (auto x : inner.区间) {
        while (i < outer.区间.size() && outer.区间[i].上界 < x.下界) ++i;
        if (i == outer.区间.size() || outer.区间[i].下界 > x.下界 || outer.区间[i].上界 < x.上界) return false;
    } return true;
}
inline void 特征类数据服务::检查规格(const 先天I64特征类型规格& s) {
    要求(有效(s.外设提供者) && 有效(s.单位) && s.缩放分子 && s.缩放分母
        && std::gcd(s.缩放分子, s.缩放分母) == 1 && !s.允许集合.empty(), S::入口拒绝);
    for (auto x : s.允许集合) 要求(浅层结构有效(x), S::入口拒绝);
    if (s.域形成) 要求(s.域形成->允许误差 >= 0 && 有效(s.域形成->参数来源), S::入口拒绝);
}
inline std::int64_t 特征类数据服务::完整整数(const 准确特征读取事实& f) {
    if (const auto* v = std::get_if<std::int64_t>(&f.完整值)) return *v;
    const auto* v = std::get_if<std::int64_t>(&std::get<特征值信息>(f.完整值).值内容);
    要求(v != nullptr, S::能力未提供); return *v;
}

inline 先天I64特征类型信息 特征类数据服务::读类型(特征类型身份 id, std::uint64_t g, std::uint64_t h,
    标量读取上下文* count) const {
    要求(有效(id), S::入口拒绝);
    const auto n = 节点(id.编码, g, h, 分区::定义);
    const auto family = 核对归属(id.编码, g, h, 分区::定义);
    if (count) 标量计数(count->关系计数, family.编码, count->预算.最大关系数);
    要求(n.种类 == 节点种类::属性类型 && n.属性类型表示 == L1所有者范围值表示种类::I64, S::能力未提供);
    auto attrs = 属性(id.编码, g, h, 分区::定义);
    if (count) for (const auto& value : attrs) 标量计数(count->值计数, value.编码, count->预算.最大属性值数);
    要求(attrs.size() == 1 && attrs.front().属性类型节点 == d_[类型规格属性], S::旧格式不支持);
    const auto* data = std::get_if<std::vector<std::uint64_t>>(&attrs.front().材料);
    要求(data && data->size() >= 5 && (*data)[2] && (*data)[2] <= (data->size() - 3) / 2
        && data->size() == 3 + 2 * (*data)[2], S::旧格式不支持);
    const auto source = 唯一关系(id.编码, d_[外设来源关系], g, h, 分区::定义);
    const auto unit = 唯一关系(id.编码, d_[单位关系], g, h, 分区::定义);
    if (count) {
        标量计数(count->关系计数, source.编码, count->预算.最大关系数);
        标量计数(count->关系计数, unit.编码, count->预算.最大关系数);
    }
    (void)节点(source.目标节点, g, h); (void)节点(unit.目标节点, g, h);
    先天I64特征类型信息 out; out.身份 = id;
    out.规格.外设提供者 = source.目标节点; out.规格.单位 = unit.目标节点;
    out.规格.缩放分子 = (*data)[0]; out.规格.缩放分母 = (*data)[1];
    for (std::size_t i = 3; i < data->size(); i += 2)
        out.规格.允许集合.push_back({std::bit_cast<std::int64_t>((*data)[i]), std::bit_cast<std::int64_t>((*data)[i + 1])});
    auto rules = 关系(id.编码, d_[域规则关系], false, g, h, 分区::定义);
    if (count) for (const auto& edge : rules) 标量计数(count->关系计数, edge.编码, count->预算.最大关系数);
    要求(rules.size() <= 1);
    if (!rules.empty()) {
        const auto& e = rules.front(); 要求(e.角色或顺序 == 1);
        const auto rule = 节点(e.目标节点, g, h, 分区::定义);
        要求(rule.种类 == 节点种类::普通 && rule.创建事实代次 == n.创建事实代次);
        const auto values = 属性(e.目标节点, g, h, 分区::定义);
        if (count) for (const auto& fact : values) 标量计数(count->值计数, fact.编码, count->预算.最大属性值数);
        要求(values.size() == 1);
        const auto& value = 唯一属性(values, d_[规则误差属性]);
        const auto* error = std::get_if<std::int64_t>(&value.材料); 要求(error && *error >= 0);
        const auto parameter = 唯一关系(e.目标节点, d_[参数来源关系], g, h, 分区::定义);
        if (count) 标量计数(count->关系计数, parameter.编码, count->预算.最大关系数);
        (void)节点(parameter.目标节点, g, h);
        out.规格.域形成 = I64特征域形成参数{*error, parameter.目标节点};
        out.规则 = 特征比较规则身份{e.目标节点};
    }
    检查规格(out.规格); return out;
}
inline 特征规范I64域 特征类数据服务::读完整域(特征类型身份 id, std::uint64_t g, std::uint64_t h) const {
    const auto n = 节点(id.编码, g, h, 分区::定义);
    const auto attrs = 属性(id.编码, g, h, 分区::定义);
    const auto marker = std::find_if(attrs.begin(), attrs.end(), [&](const V& v) { return v.属性类型节点 == id.编码; });
    if (marker == attrs.end()) return 规范域({读类型(id, g, h).规格.允许集合});
    const auto* value = std::get_if<std::int64_t>(&marker->材料);
    要求(value && *value == 标量格式标记 && attrs.size() == 1, S::旧格式不支持);
    try {
        标量读取上下文 c{g, h, 标量业务准入预算};
        const auto edges = 标量关系(id.编码, d_[输出FT归属关系], true, c);
        标量要求(edges.size() == 1 && edges.front().角色或顺序 >= 1 && edges.front().角色或顺序 <= 3);
        标量展开(edges.front().源节点, true, c);
        const auto& output = 标量输出(c.定义.at(edges.front().源节点), static_cast<unsigned>(edges.front().角色或顺序));
        要求(output.特征类型 == id.编码 && output.格式标记值事实 == marker->编码);
        return 特征规范I64域{{{output.声明.量化.下界, output.声明.量化.上界}}};
    } catch (const 标量失败& e) {
        switch (e.状态) {
        case SS::预算不足: throw S::数量预算不足;
        case SS::资源失败: throw S::资源失败;
        case SS::事实代次漂移: throw S::并发变化;
        case SS::历史材料不可用: throw S::历史材料不可用;
        case SS::未找到: throw S::未找到;
        case SS::已退出: throw S::已退出;
        case SS::格式不支持: throw S::旧格式不支持;
        case SS::类型不匹配: case SS::单位量化不匹配: throw S::类型不相容;
        case SS::算法不支持: throw S::能力未提供;
        default: throw S::内部不一致;
        }
    }
}
inline std::int64_t 特征类数据服务::解析输入(const 特征准确值& input, std::uint64_t g, std::uint64_t h) const {
    要求(浅层结构有效(input), S::入口拒绝);
    if (const auto* v = std::get_if<std::int64_t>(&input)) return *v;
    auto id = std::get<特征值身份>(input);
    auto raw = 原始事实(id.编码, g); const auto* fact = std::get_if<V>(&raw);
    要求(fact != nullptr, S::类型不相容); 生命周期(*fact, g, h);
    auto result = values_.获取特征值(id);
    if (const auto* error = std::get_if<特征值读取错误>(&result)) {
        switch (*error) {
        case 特征值读取错误::入口拒绝: throw S::入口拒绝;
        case 特征值读取错误::未找到: throw S::未找到;
        case 特征值读取错误::材料已清理: throw S::历史材料不可用;
        case 特征值读取错误::能力未提供: throw S::能力未提供;
        case 特征值读取错误::资源失败: throw S::资源失败;
        default: throw S::内部不一致;
        }
    }
    auto& full = std::get<特征值信息>(result);
    const auto* value = std::get_if<std::int64_t>(&full.值内容);
    const auto* stored = std::get_if<std::int64_t>(&fact->材料);
    要求(value && stored, S::能力未提供);
    要求(full.值身份 == id && *stored == *value); 守卫(g); return *value;
}
inline 准确特征读取事实 特征类数据服务::读准确(特征信息身份 id, std::uint64_t g, std::uint64_t h,
    标量读取上下文* count) const {
    const auto n = 节点(id.编码, g, h, 分区::信息);
    要求(n.种类 == 节点种类::普通, S::旧格式不支持);
    const auto family = 核对归属(id.编码, g, h, 分区::信息);
    const auto type = 唯一关系(id.编码, f_[准确类型关系], g, h, 分区::信息);
    if (count) {
        标量计数(count->关系计数, family.编码, count->预算.最大关系数);
        标量计数(count->关系计数, type.编码, count->预算.最大关系数);
    }
    const auto ft = 读类型({type.目标节点}, g, h, count);
    const auto values = 属性(id.编码, g, h, 分区::信息);
    if (count) for (const auto& value : values) 标量计数(count->值计数, value.编码, count->预算.最大属性值数);
    要求(values.size() == 1, S::旧格式不支持);
    const auto& v = values.front(); const auto* scalar = std::get_if<std::int64_t>(&v.材料);
    要求(scalar && (v.属性类型节点 == f_[准确内联属性] || v.属性类型节点 == f_[准确引用属性]), S::旧格式不支持);
    要求(v.创建事实代次 == n.创建事实代次 && type.创建事实代次 == n.创建事实代次
        && family.创建事实代次 == n.创建事实代次 && v.退出事实代次 == n.退出事实代次
        && type.退出事实代次 == n.退出事实代次 && family.退出事实代次 == n.退出事实代次);
    要求(包含(规范域({ft.规格.允许集合}), 特征规范I64域{{{*scalar, *scalar}}}), S::类型不相容);
    准确特征读取事实 out;
    out.Gread = g; out.H = h; out.信息.身份 = id; out.信息.类型 = ft.身份;
    out.类型关系 = type.编码; out.准确值事实 = v.编码;
    out.创建G = n.创建事实代次; out.退出G = n.退出事实代次;
    if (v.属性类型节点 == f_[准确内联属性]) { out.信息.准确值 = *scalar; out.完整值 = *scalar; }
    else {
        特征值身份 vid{v.编码}; 要求(解析输入(特征准确值{vid}, g, h) == *scalar);
        out.信息.准确值 = vid; out.完整值 = 特征值信息{vid, 特征值内容{*scalar}};
    } return out;
}
inline 特征类数据服务::Key 特征类数据服务::新键(const WS& ws) {
    std::uint32_t max = 0;
    for (const auto& x : ws.节点) max = std::max(max, x.本地键.值);
    for (const auto& x : ws.关系) max = std::max(max, x.本地键.值);
    for (const auto& x : ws.值) max = std::max(max, x.本地键.值);
    要求(max != std::numeric_limits<std::uint32_t>::max(), S::数量预算不足); return {max + 1};
}
inline 特征类数据服务::Key 特征类数据服务::加节点(WS& ws, std::optional<L1所有者范围值表示种类> repr) {
    auto k = 新键(ws); ws.节点.push_back({k, repr ? 节点种类::属性类型 : 节点种类::普通, repr}); return k;
}
inline 特征类数据服务::Key 特征类数据服务::加关系(WS& ws, Ref a, Ref b, Ref type, std::int64_t role) {
    auto k = 新键(ws); ws.关系.push_back({k, std::move(a), std::move(b), std::move(type), role}); return k;
}
inline 特征类数据服务::Key 特征类数据服务::加值(WS& ws, Ref node, Ref type,
    L1所有者范围原始值材料 content, std::optional<Ref> source) const {
    auto k = 新键(ws); ws.值.push_back({k, node, type, std::move(content), source ? *source : Ref{producer_}});
    ws.属性槽变更.push_back({std::move(node), std::move(type), k}); return k;
}
inline 特征类数据服务::WS 特征类数据服务::新写集(分区 p, std::uint64_t g) const {
    要求(!pending_&&!scalar_pending_, S::前次写入待收敛);
    要求(g && g < std::numeric_limits<std::uint64_t>::max(), S::算术不可表示);
    auto key = g + 1;
    for (;;) {
        if ((key >> 48) == 0x4E43) key = 0x4E44'0000'0000'0000;
        auto first = 端口(p).读取首次写入材料({L1所有者范围首次写入读取合同版本, {key}});
        要求(first.读取事实代次 == g, S::并发变化);
        if (first.状态 == L1所有者范围读取状态::未找到) break;
        if (first.状态 != L1所有者范围读取状态::成功) throw 映射(first.状态);
        要求(key != std::numeric_limits<std::uint64_t>::max(), S::算术不可表示); ++key;
    }
    WS ws; ws.期望事实代次 = g; ws.写入幂等身份 = {key}; return ws;
}
inline void 特征类数据服务::规范化写集(WS& ws) {
    const auto by_key = [](const auto& a, const auto& b) { return a.本地键 < b.本地键; };
    std::sort(ws.节点.begin(), ws.节点.end(), by_key); std::sort(ws.关系.begin(), ws.关系.end(), by_key);
    std::sort(ws.值.begin(), ws.值.end(), by_key);
    const auto order = [](const Ref& ref) {
        if (const auto* id = std::get_if<稳定编码>(&ref)) return id->值;
        return (std::uint64_t{1} << 63) | std::get<Key>(ref).值;
    };
    std::sort(ws.属性槽变更.begin(), ws.属性槽变更.end(), [&](const auto& a, const auto& b) {
        if (order(a.所属节点) != order(b.所属节点)) return order(a.所属节点) < order(b.所属节点);
        if (order(a.属性类型节点) != order(b.属性类型节点)) return order(a.属性类型节点) < order(b.属性类型节点);
        return a.新当前值 < b.新当前值;
    });
    std::sort(ws.退出事实.begin(), ws.退出事实.end());
    要求(std::adjacent_find(ws.退出事实.begin(), ws.退出事实.end()) == ws.退出事实.end());
}
inline 稳定编码 特征类数据服务::映射编码(const L1所有者范围写入结果& r, Key k) {
    auto it = std::find_if(r.新编码映射.begin(), r.新编码映射.end(), [&](const auto& x) { return x.first == k; });
    要求(it != r.新编码映射.end() && 有效(it->second)); return it->second;
}
inline 稳定编码 特征类数据服务::解析引用(const L1所有者范围写入结果& r, const Ref& ref) {
    if (const auto* id = std::get_if<稳定编码>(&ref)) return *id; return 映射编码(r, std::get<Key>(ref));
}
inline 特征类数据服务::WS 特征类数据服务::初始化写集(分区 p, std::uint64_t g) {
    WS ws; ws.期望事实代次 = g; ws.写入幂等身份 = {1};
    const auto count = p == 分区::定义 ? std::size_t{定义角色数} : std::size_t{信息角色数};
    for (std::size_t i = 0; i < count; ++i) {
        std::optional<L1所有者范围值表示种类> repr;
        if (p == 分区::定义) {
            if (i == 类型规格属性 || i == 注册U64属性 || i == 派生规则属性) repr = L1所有者范围值表示种类::U64组;
            if (i == 规则误差属性 || i == 阶次属性) repr = L1所有者范围值表示种类::I64;
            if (i == 注册I64属性) repr = L1所有者范围值表示种类::I64组;
        } else if (i == 准确内联属性 || i == 准确引用属性) repr = L1所有者范围值表示种类::I64;
        (void)加节点(ws, repr);
    } return ws;
}
inline void 特征类数据服务::接受初始化(分区 p, const L1所有者范围写入结果& result) {
    if (p == 分区::定义) {
        for (std::size_t i = 0; i < d_.size(); ++i) d_[i] = 映射编码(result, {static_cast<std::uint32_t>(i + 1)});
        definition_ready_ = true;
    } else {
        for (std::size_t i = 0; i < f_.size(); ++i) f_[i] = 映射编码(result, {static_cast<std::uint32_t>(i + 1)});
        information_ready_ = true;
    }
}

inline void 特征类数据服务::确认发布(分区 p, const WS& ws, const L1所有者范围写入结果& r) const {
    要求(r.状态 == L1所有者范围写入状态::成功 && r.合同版本 == L1所有者范围CRUD合同版本
        && r.所有者 == 端口(p).所有者身份() && r.写入幂等身份 == ws.写入幂等身份
        && r.是否形成内存权威发布 && r.事实代次 && r.事实代次 == ws.期望事实代次 + 1
        && r.新编码映射.size() == ws.节点.size() + ws.关系.size() + ws.值.size());
    std::set<std::uint32_t> keys; std::set<std::uint64_t> ids;
    for (const auto& [k, id] : r.新编码映射)
        要求(keys.insert(k.值).second && ids.insert(id.值).second && 有效(id));
    const auto g = 当前G(), h = r.事实代次; 要求(g >= h);
    for (const auto& x : ws.节点) {
        auto raw = 原始事实(映射编码(r, x.本地键), g); const auto* n = std::get_if<N>(&raw);
        要求(n && n->种类 == x.种类 && n->属性类型表示 == x.属性类型表示
            && n->创建事实代次 == h && n->写入所有者 == 端口(p).所有者身份());
    }
    for (const auto& x : ws.关系) {
        auto raw = 原始事实(映射编码(r, x.本地键), g); const auto* e = std::get_if<E>(&raw);
        要求(e && e->源节点 == 解析引用(r, x.源节点) && e->目标节点 == 解析引用(r, x.目标节点)
            && e->关系类型节点 == 解析引用(r, x.关系类型节点) && e->角色或顺序 == x.角色或顺序
            && e->写入所有者 == 端口(p).所有者身份() && e->创建事实代次 == h);
    }
    for (const auto& x : ws.值) {
        auto raw = 原始事实(映射编码(r, x.本地键), g); const auto* v = std::get_if<V>(&raw);
        要求(v && v->所属节点 == 解析引用(r, x.所属节点) && v->属性类型节点 == 解析引用(r, x.属性类型节点)
            && v->来源节点 == 解析引用(r, x.来源节点) && v->材料 == x.材料
            && v->写入所有者 == 端口(p).所有者身份() && v->创建事实代次 == h);
    }
    for (const auto& x : ws.属性槽变更) {
        const auto values = 属性(解析引用(r, x.所属节点), g, h, p);
        要求(唯一属性(values, 解析引用(r, x.属性类型节点)).编码 == 映射编码(r, x.新当前值));
    }
    for (auto id : ws.退出事实) {
        const auto raw = 原始事实(id, g);
        std::visit([&](const auto& f) { 要求(f.写入所有者 == 端口(p).所有者身份() && f.退出事实代次 == h); }, raw);
    } 守卫(g);
}
inline L1所有者范围写入结果 特征类数据服务::收敛原请求() {
    要求(pending_.has_value());
    try {
        const auto p = pending_->区; const auto& ws = pending_->请求;
        auto first = 端口(p).读取首次写入材料({L1所有者范围首次写入读取合同版本, ws.写入幂等身份});
        if (first.状态 == L1所有者范围读取状态::未找到) {
            const auto replay = 端口(p).提交所有者范围中性写集(ws);
            if (replay.状态 != L1所有者范围写入状态::成功 && replay.状态 != L1所有者范围写入状态::精确重复) {
                if (!replay.是否形成内存权威发布
                    && (replay.状态 == L1所有者范围写入状态::入口拒绝 || replay.状态 == L1所有者范围写入状态::引用冲突
                        || replay.状态 == L1所有者范围写入状态::未找到 || replay.状态 == L1所有者范围写入状态::已退出
                        || replay.状态 == L1所有者范围写入状态::事实代次漂移 || replay.状态 == L1所有者范围写入状态::幂等冲突)) {
                    const auto s = 映射(replay.状态); pending_.reset(); throw s;
                } throw S::发布结果未确认;
            }
            first = 端口(p).读取首次写入材料({L1所有者范围首次写入读取合同版本, ws.写入幂等身份});
        }
        要求(first.状态 == L1所有者范围读取状态::成功 && first.首次规范化写集 && first.首次写入结果, S::发布结果未确认);
        if(first.所有者 != 端口(p).所有者身份() || first.写入幂等身份 != ws.写入幂等身份
            || *first.首次规范化写集 != ws) {
            pending_.reset(); throw S::幂等冲突;
        }
        确认发布(p, ws, *first.首次写入结果);
        if (pending_->初始化) 接受初始化(p, *first.首次写入结果);
        auto result = std::move(*first.首次写入结果); pending_.reset(); return result;
    } catch (...) { if (pending_) throw S::发布结果未确认; throw; }
}
inline L1所有者范围写入结果 特征类数据服务::提交(分区 p, WS ws, bool init) {
    要求(!pending_&&!scalar_pending_, S::前次写入待收敛); 规范化写集(ws);
    pending_.emplace(待确认写入{p, std::move(ws), init});
    try { (void)端口(p).提交所有者范围中性写集(pending_->请求); return 收敛原请求(); }
    catch (...) { if (pending_) throw S::发布结果未确认; throw; }
}
inline void 特征类数据服务::初始化(分区 p) {
    要求(!pending_&&!scalar_pending_, S::前次写入待收敛);
    const auto g = 当前G(); (void)节点(producer_, g, g);
    if (p == 分区::信息) 结构就绪(分区::定义, g, g);
    const auto first = 端口(p).读取首次写入材料({L1所有者范围首次写入读取合同版本, {1}});
    要求(first.读取事实代次 == g, S::并发变化);
    if (first.状态 == L1所有者范围读取状态::成功) {
        要求(first.首次规范化写集 && first.首次写入结果);
        const auto expected = 初始化写集(p, first.首次规范化写集->期望事实代次);
        要求(*first.首次规范化写集 == expected, S::旧格式不支持);
        确认发布(p, expected, *first.首次写入结果); 接受初始化(p, *first.首次写入结果);
    } else if (first.状态 == L1所有者范围读取状态::未找到) (void)提交(p, 初始化写集(p, g), true);
    else throw 映射(first.状态);
}
inline 特征数据结果<std::monostate> 特征类数据服务::初始化特征定义结构() {
    return 保护<std::monostate>([&] { 初始化(分区::定义); return std::monostate{}; });
}
inline 特征数据结果<std::monostate> 特征类数据服务::初始化准确特征结构() {
    return 保护<std::monostate>([&] { 初始化(分区::信息); return std::monostate{}; });
}
inline 特征数据结果<std::monostate> 特征类数据服务::收敛待确认写入() {
    return 保护<std::monostate>([&] {
        要求(!scalar_pending_&&(!pending_ || !pending_->旧治理), S::前次写入待收敛);
        if (pending_) (void)收敛原请求(); return std::monostate{};
    });
}
inline 特征数据结果<特征类型身份> 特征类数据服务::创建先天I64特征类型(const 先天I64特征类型规格& spec) {
    return 保护<特征类型身份>([&] {
        检查规格(spec); const auto g = 当前G(); 结构就绪(分区::定义, g, g);
        (void)节点(spec.外设提供者, g, g); (void)节点(spec.单位, g, g); (void)节点(producer_, g, g);
        if (spec.域形成) (void)节点(spec.域形成->参数来源, g, g);
        auto ws = 新写集(分区::定义, g); const auto ft = 加节点(ws, L1所有者范围值表示种类::I64);
        (void)加关系(ws, ft, d_[定义锚点], d_[定义归属]);
        (void)加关系(ws, ft, spec.外设提供者, d_[外设来源关系]); (void)加关系(ws, ft, spec.单位, d_[单位关系]);
        std::vector<std::uint64_t> data{spec.缩放分子, spec.缩放分母, spec.允许集合.size()};
        for (auto x : spec.允许集合) { data.push_back(std::bit_cast<std::uint64_t>(x.下界)); data.push_back(std::bit_cast<std::uint64_t>(x.上界)); }
        (void)加值(ws, ft, d_[类型规格属性], std::move(data));
        if (spec.域形成) {
            const auto rule = 加节点(ws); (void)加关系(ws, ft, rule, d_[域规则关系]);
            (void)加关系(ws, rule, spec.域形成->参数来源, d_[参数来源关系]);
            (void)加值(ws, rule, d_[规则误差属性], spec.域形成->允许误差);
        }
        const auto result = 提交(分区::定义, std::move(ws)); return 特征类型身份{映射编码(result, ft)};
    });
}
inline 特征数据结果<特征信息身份> 特征类数据服务::创建准确特征(特征类型身份 type, const 特征准确值& input) {
    return 保护<特征信息身份>([&] {
        const auto g = 当前G(); 结构就绪(分区::信息, g, g);
        const auto ft = 读类型(type, g, g); const auto value = 解析输入(input, g, g);
        要求(包含(规范域({ft.规格.允许集合}), 特征规范I64域{{{value, value}}}), S::类型不相容);
        (void)节点(producer_, g, g); auto ws = 新写集(分区::信息, g); const auto f = 加节点(ws);
        (void)加关系(ws, f, f_[信息锚点], f_[信息归属]); (void)加关系(ws, f, type.编码, f_[准确类型关系]);
        // 引用输入复制完整 I64 为 F 自有不可变值；外部值身份不进入整数载荷。
        (void)加值(ws, f, f_[std::holds_alternative<std::int64_t>(input) ? 准确内联属性 : 准确引用属性], value);
        const auto result = 提交(分区::信息, std::move(ws)); return 特征信息身份{映射编码(result, f)};
    });
}
inline 特征数据结果<准确特征读取事实> 特征类数据服务::读取准确特征事实(const 准确特征读取请求& r) const {
    return 保护<准确特征读取事实>([&] {
        截止有效(r.合同版本, r.Gread, r.H); 守卫(r.Gread);
        auto out = 读准确(r.身份, r.Gread, r.H); 守卫(r.Gread); return out;
    });
}
inline 特征数据结果<特征信息> 特征类数据服务::读取准确特征(特征信息身份 id) const {
    return 保护<特征信息>([&] { const auto g = 当前G(); auto out = 读准确(id, g, g).信息; 守卫(g); return out; });
}
inline 特征数据结果<特征类型身份> 特征类数据服务::读取准确特征类型(特征信息身份 id) const {
    return 保护<特征类型身份>([&] { const auto g = 当前G(); auto out = 读准确(id, g, g).信息.类型; 守卫(g); return out; });
}
inline 特征数据结果<特征准确值> 特征类数据服务::读取准确特征值(特征信息身份 id) const {
    return 保护<特征准确值>([&] { const auto g = 当前G(); auto out = 读准确(id, g, g).信息.准确值; 守卫(g); return out; });
}
inline 特征数据结果<std::vector<特征信息>> 特征类数据服务::查询准确特征(特征类型身份 type, const 特征准确值& input) const {
    return 保护<std::vector<特征信息>>([&] {
        const auto g = 当前G(); 结构就绪(分区::信息, g, g);
        (void)读类型(type, g, g); const auto value = 解析输入(input, g, g);
        const auto es = 关系(type.编码, f_[准确类型关系], true, g, g, 分区::信息);
        std::vector<特征信息> out;
        for (const auto& e : es) {
            要求(e.角色或顺序 == 1); auto f = 读准确({e.源节点}, g, g);
            if (完整整数(f) == value) out.push_back(std::move(f.信息));
        }
        std::sort(out.begin(), out.end(), [](const auto& a, const auto& b) { return a.身份.编码 < b.身份.编码; });
        守卫(g); return out;
    });
}
inline 特征数据结果<std::monostate> 特征类数据服务::删除准确特征(特征信息身份 id) {
    return 保护<std::monostate>([&] {
        const auto g = 当前G(); const auto f = 读准确(id, g, g); auto ws = 新写集(分区::信息, g);
        ws.退出事实 = {id.编码, f.类型关系, 核对归属(id.编码, g, g, 分区::信息).编码};
        for (const auto& v : 属性(id.编码, g, g, 分区::信息)) ws.退出事实.push_back(v.编码);
        (void)提交(分区::信息, std::move(ws)); return std::monostate{};
    });
}
inline 特征数据结果<先天I64特征类型信息> 特征类数据服务::读取先天I64特征类型(特征类型身份 id) const {
    return 保护<先天I64特征类型信息>([&] { const auto g = 当前G(); auto out = 读类型(id, g, g); 守卫(g); return out; });
}
inline 特征数据结果<特征截止事实<先天I64特征类型信息>> 特征类数据服务::读取先天I64特征类型事实(const 特征类型截止请求& r) const {
    return 保护<特征截止事实<先天I64特征类型信息>>([&] {
        截止有效(r.版本, r.Gread, r.H); 守卫(r.Gread); auto ft = 读类型(r.类型, r.Gread, r.H); 守卫(r.Gread);
        return 特征截止事实<先天I64特征类型信息>{r.Gread, r.H, std::move(ft)};
    });
}
inline 特征数据结果<特征截止事实<特征规范I64域>> 特征类数据服务::读取I64类型完整域(const 特征类型截止请求& r) const {
    return 保护<特征截止事实<特征规范I64域>>([&] {
        截止有效(r.版本, r.Gread, r.H); 守卫(r.Gread);
        auto domain = 读完整域(r.类型, r.Gread, r.H); 守卫(r.Gread);
        return 特征截止事实<特征规范I64域>{r.Gread, r.H, std::move(domain)};
    });
}
inline 特征数据结果<特征域形成事实> 特征类数据服务::形成I64特征域(const 准确特征读取请求& r) const {
    return 保护<特征域形成事实>([&] {
        截止有效(r.合同版本, r.Gread, r.H); 守卫(r.Gread);
        const auto f = 读准确(r.身份, r.Gread, r.H); const auto ft = 读类型(f.信息.类型, r.Gread, r.H);
        要求(ft.规格.域形成 && ft.规则, S::规则缺失);
        const auto v = 完整整数(f), error = ft.规格.域形成->允许误差;
        要求(v >= std::numeric_limits<std::int64_t>::min() + error
            && v <= std::numeric_limits<std::int64_t>::max() - error, S::算术不可表示);
        特征规范I64域 domain{{{v - error, v + error}}};
        要求(包含(规范域({ft.规格.允许集合}), domain), S::类型不相容); 守卫(r.Gread);
        return 特征域形成事实{ft.身份, std::move(domain), r.身份, *ft.规则, r.Gread, r.H};
    });
}
inline 特征数据结果<特征截止事实<特征规范I64域>> 特征类数据服务::规范化I64特征域(const 特征I64域判定请求& r) const {
    return 保护<特征截止事实<特征规范I64域>>([&] {
        截止有效(r.类型.版本, r.类型.Gread, r.类型.H); 守卫(r.类型.Gread);
        const auto full = 读完整域(r.类型.类型, r.类型.Gread, r.类型.H);
        auto domain = 规范域(r.域); 要求(包含(full, domain), S::类型不相容); 守卫(r.类型.Gread);
        return 特征截止事实<特征规范I64域>{r.类型.Gread, r.类型.H, std::move(domain)};
    });
}
inline 特征数据结果<特征截止事实<bool>> 特征类数据服务::判定准确特征命中域(const 准确特征域命中请求& r) const {
    return 保护<特征截止事实<bool>>([&] {
        截止有效(r.特征.合同版本, r.特征.Gread, r.特征.H); 守卫(r.特征.Gread);
        const auto f = 读准确(r.特征.身份, r.特征.Gread, r.特征.H);
        const auto full = 规范域({读类型(f.信息.类型, r.特征.Gread, r.特征.H).规格.允许集合});
        const auto domain = 规范域(r.域); 要求(包含(full, domain), S::类型不相容);
        const auto value = 完整整数(f); const auto hit = 包含(domain, 特征规范I64域{{{value, value}}}); 守卫(r.特征.Gread);
        return 特征截止事实<bool>{r.特征.Gread, r.特征.H, hit};
    });
}
inline 特征数据结果<特征截止事实<bool>> 特征类数据服务::判定I64域包含(const 特征I64域包含请求& r) const {
    return 保护<特征截止事实<bool>>([&] {
        截止有效(r.类型.版本, r.类型.Gread, r.类型.H); 守卫(r.类型.Gread);
        const auto full = 读完整域(r.类型.类型, r.类型.Gread, r.类型.H);
        const auto outer = 规范域(r.外), inner = 规范域(r.内);
        要求(包含(full, outer) && 包含(full, inner), S::类型不相容);
        const auto hit = 包含(outer, inner); 守卫(r.类型.Gread);
        return 特征截止事实<bool>{r.类型.Gread, r.类型.H, hit};
    });
}

inline 特征类数据服务::分区 特征类数据服务::旧路由(std::uint64_t g, std::uint64_t h) const {
    标量要求(legacy_.has_value(), SS::格式不支持);
    const auto& t = *legacy_;
    const std::array<稳定编码, 7> ids{t.实际阶次属性类型, t.派生规则属性类型,
        t.直接来源关系类型, t.宿主关系类型, t.比较注册归属关系类型,
        t.比较注册U64属性类型, t.比较注册I64属性类型};
    标量要求(t.格式 == 1, SS::入口拒绝);
    std::set<稳定编码> seen;
    std::optional<L1结构所有者身份> owner;
    for (std::size_t i = 0; i < ids.size(); ++i) {
        标量要求(有效(ids[i]) && seen.insert(ids[i]).second, SS::入口拒绝);
        const auto raw = 原始事实(ids[i], g); const auto* n = std::get_if<N>(&raw);
        标量要求(n && n->编码 == ids[i], SS::格式不支持);
        const auto rep = i == 0 ? std::optional{L1所有者范围值表示种类::I64}
            : i == 1 || i == 5 ? std::optional{L1所有者范围值表示种类::U64组}
            : i == 6 ? std::optional{L1所有者范围值表示种类::I64组} : std::nullopt;
        标量要求(n->种类 == (rep ? 节点种类::属性类型 : 节点种类::普通)
            && n->属性类型表示 == rep && n->当前属性.empty(), SS::格式不支持);
        if (h) 生命周期(*n, g, h);
        if (!owner) owner = n->写入所有者;
        标量要求(n->写入所有者 == *owner, SS::来源不匹配);
    }
    const bool a = *owner == definitions_.所有者身份(), b = *owner == information_.所有者身份();
    标量要求(a != b && 绑定于(l1_), SS::格式不支持);
    return a ? 分区::定义 : 分区::信息;
}
inline 特征类数据服务::旧治理布局 特征类数据服务::旧读取闭包(
    特征类定义身份 root, std::uint64_t g, std::uint64_t h, 分区 p,
    const 特征类派生读取预算& budget) const {
    标量要求(budget.有效() && 有效(root.结点) && h && g >= h, SS::入口拒绝);
    标量要求(旧路由(g, h) == p, SS::来源不匹配);
    const auto& t = *legacy_;
    std::set<稳定编码> rels, vals, leaves, defs;
    auto rawnode = [&](稳定编码 id, bool own) {
        const auto raw = 原始事实(id, g); const auto* n = std::get_if<N>(&raw);
        标量要求(n && n->编码 == id, SS::格式不支持);
        if (own) 标量要求(n->写入所有者 == 端口(p).所有者身份(), SS::来源不匹配);
        生命周期(*n, g, h); return *n;
    };
    auto readattrs = [&](const N& n) {
        const auto result = l1_.读取所有者范围历史属性值组({L1所有者范围CRUD合同版本, n.编码, h});
        标量要求(result.读取事实代次 == g, SS::事实代次漂移);
        标量要求(result.状态 == L1所有者范围读取状态::成功, 标量映射(result.状态));
        标量要求(result.合同版本 == L1所有者范围CRUD合同版本 && result.所属节点 == n.编码
            && result.历史截止事实代次 == h);
        auto vs = result.属性值组; std::set<稳定编码> slots;
        for (const auto& v : vs) {
            标量计数(vals, v.编码, budget.最大属性值数);
            生命周期(v, g, h);
            标量要求(有效(v.编码) && v.写入所有者 == n.写入所有者 && v.所属节点 == n.编码
                && slots.insert(v.属性类型节点).second && L1所有者范围原始材料完整(v.材料)
                && v.来源节点 == n.编码 && v.创建事实代次 == n.创建事实代次
                && v.退出事实代次 == n.退出事实代次, SS::来源不匹配);
        }
        return vs;
    };
    auto readrels = [&](const N& n, 稳定编码 type) {
        const auto result = l1_.读取所有者范围历史关系组(
            {L1所有者范围CRUD合同版本, L1所有者范围关系端点方向::源, n.编码, type, h});
        标量要求(result.读取事实代次 == g, SS::事实代次漂移);
        标量要求(result.状态 == L1所有者范围读取状态::成功, 标量映射(result.状态));
        标量要求(result.合同版本 == L1所有者范围CRUD合同版本 && result.方向 == L1所有者范围关系端点方向::源
            && result.端点节点 == n.编码 && result.关系类型节点 == type && result.历史截止事实代次 == h);
        auto es = result.关系组; std::set<稳定编码> unique;
        for (const auto& e : es) {
            标量计数(rels, e.编码, budget.最大关系数);
            生命周期(e, g, h);
            标量要求(有效(e.编码) && unique.insert(e.编码).second && e.写入所有者 == n.写入所有者
                && e.源节点 == n.编码 && e.关系类型节点 == type && 有效(e.目标节点)
                && e.创建事实代次 == n.创建事实代次
                && e.退出事实代次 == n.退出事实代次, SS::来源不匹配);
        }
        std::sort(es.begin(), es.end(), [](const auto& a, const auto& b) {
            return a.角色或顺序 != b.角色或顺序 ? a.角色或顺序 < b.角色或顺序 : a.编码 < b.编码;
        });
        return es;
    };
    auto parse = [&](稳定编码 id) {
        旧治理布局 o;
        const auto n = rawnode(id, id == root.结点);
        标量要求(n.种类 == 节点种类::普通 && !n.属性类型表示, SS::格式不支持);
        auto vs = readattrs(n);
        auto sources = readrels(n, t.直接来源关系类型), host = readrels(n, t.宿主关系类型),
            reg = readrels(n, t.比较注册归属关系类型);
        o.事实.身份 = {id}; o.事实.创建G = n.创建事实代次; o.事实.退出G = n.退出事实代次;
        const bool any = std::any_of(vs.begin(), vs.end(), [&](const auto& v) {
            return v.属性类型节点 == t.实际阶次属性类型 || v.属性类型节点 == t.派生规则属性类型;
        }) || !sources.empty() || !host.empty() || !reg.empty();
        // 根只能是旧派生定义；注册节点、基础叶和其它退出集合成员不是同一业务目标。
        if(id==root.结点)标量要求(any
            &&std::count_if(vs.begin(),vs.end(),[&](const auto& v){return v.属性类型节点==t.实际阶次属性类型;})==1
            &&std::count_if(vs.begin(),vs.end(),[&](const auto& v){return v.属性类型节点==t.派生规则属性类型;})==1,
            SS::格式不支持);
        if (!any) {
            标量计数(leaves, id, budget.最大叶数);
            标量要求(vs.size() == 1, SS::格式不支持);
            const auto ft = rawnode(vs.front().属性类型节点, false);
            const auto rep = std::visit([](const auto& value) {
                using T = std::decay_t<decltype(value)>;
                if constexpr (std::is_same_v<T, std::int64_t>) return L1所有者范围值表示种类::I64;
                else if constexpr (std::is_same_v<T, std::vector<std::int64_t>>) return L1所有者范围值表示种类::I64组;
                else if constexpr (std::is_same_v<T, std::vector<std::uint64_t>>) return L1所有者范围值表示种类::U64组;
                else return L1所有者范围值表示种类::独立材料引用;
            }, vs.front().材料);
            标量要求(ft.种类 == 节点种类::属性类型 && ft.属性类型表示
                && ft.创建事实代次 <= n.创建事实代次
                && *ft.属性类型表示 == rep, SS::格式不支持);
            o.事实.实际阶次 = 1; return o; // 仅旧布局的治理证明，不产生新准确 F 或计算值。
        }
        标量计数(defs, id, budget.最大定义数);
        标量要求(vs.size() == 2 && sources.size() == 2 && host.size() <= 1 && reg.size() == 1, SS::格式不支持);
        const auto& order = 唯一属性(vs, t.实际阶次属性类型);
        const auto& rule = 唯一属性(vs, t.派生规则属性类型);
        const auto* k = std::get_if<std::int64_t>(&order.材料);
        const auto* rulebits = std::get_if<std::vector<std::uint64_t>>(&rule.材料);
        标量要求(k && *k > 1 && static_cast<std::uint64_t>(*k) <= UINT32_MAX
            && rulebits && rulebits->size() == 2 && (*rulebits)[0] && (*rulebits)[1]
            && (*rulebits)[1] <= UINT32_MAX, SS::格式不支持);
        o.事实.实际阶次 = static_cast<std::uint32_t>(*k);
        (void)rawnode({(*rulebits)[0]}, false);
        标量要求(reg.front().角色或顺序 == 1, SS::来源不匹配);
        const auto rn = rawnode(reg.front().目标节点, false);
        标量要求(rn.种类 == 节点种类::普通 && !rn.属性类型表示
            && rn.写入所有者 == n.写入所有者
            && rn.创建事实代次 == n.创建事实代次 && rn.退出事实代次 == n.退出事实代次, SS::来源不匹配);
        const auto rvs = readattrs(rn);
        标量要求(rvs.size() == 2, SS::格式不支持);
        const auto& uv = 唯一属性(rvs, t.比较注册U64属性类型);
        const auto& iv = 唯一属性(rvs, t.比较注册I64属性类型);
        const auto* u = std::get_if<std::vector<std::uint64_t>>(&uv.材料);
        const auto* v = std::get_if<std::vector<std::int64_t>>(&iv.材料);
        标量要求(u && v && u->size() == 16 && v->size() == 4, SS::格式不支持);
        标量要求((*u)[0] == 1 && (*u)[2] == 2 && (*u)[4] == 1 && (*u)[7] == 1
            && (*u)[12] == 1 && (*u)[13] == 1 && (*u)[1] >= 1 && (*u)[1] <= 2
            && (*u)[3] && (*u)[3] <= UINT32_MAX && (*u)[3] == (*rulebits)[1]
            && (*u)[8] && (*u)[9] && (*u)[10] && (*u)[11] >= 1 && (*u)[11] <= 7
            && (*u)[14] && (*u)[14] <= UINT32_MAX && (*u)[15] == (*rulebits)[0], SS::格式不支持);
        for (std::size_t i : {std::size_t{0}, std::size_t{2}})
            标量要求(((*v)[i] == 0 && (*v)[i + 1] == 0)
                || ((*v)[i] == 1 && (*v)[i + 1] >= 0), SS::格式不支持);
        const bool goal = (*u)[1] == 1;
        标量要求((*u)[5] == (goal ? 1 : 3) && (*u)[6] == (goal ? 2 : 4), SS::来源不匹配);
        o.事实.用途 = static_cast<特征类比较用途>((*u)[1]);
        o.事实.左角色 = static_cast<特征类比较角色>((*u)[5]);
        o.事实.右角色 = static_cast<特征类比较角色>((*u)[6]);
        for (std::size_t i : {std::size_t{8}, std::size_t{9}, std::size_t{10}}) (void)rawnode({(*u)[i]}, false);
        o.自有 = {id, rn.编码, order.编码, rule.编码, uv.编码, iv.编码, reg.front().编码};
        for (std::size_t i = 0; i < sources.size(); ++i) {
            const auto& e = sources[i];
            const auto encoded = static_cast<std::uint64_t>(e.角色或顺序);
            标量要求((encoded >> 32) == i + 1 && static_cast<std::uint32_t>(encoded) == (*u)[5 + i],
                SS::来源不匹配);
            o.来源.push_back(e.目标节点); o.自有.push_back(e.编码);
        }
        标量要求(o.来源[0] != o.来源[1], SS::来源不匹配);
        if (!host.empty()) {
            标量要求(host.front().角色或顺序 == 1, SS::来源不匹配);
            (void)rawnode(host.front().目标节点, false);
            o.事实.宿主 = host.front().目标节点; o.自有.push_back(host.front().编码);
        }
        std::sort(o.自有.begin(), o.自有.end());
        标量要求(o.自有.size() == 9 + host.size()
            && std::adjacent_find(o.自有.begin(), o.自有.end()) == o.自有.end());
        return o;
    };
    struct Frame { 稳定编码 id; std::uint64_t depth; bool after; };
    std::vector<Frame> stack{{root.结点, 1, false}};
    std::map<稳定编码, 旧治理布局> nodes; std::set<稳定编码> active, done;
    std::map<稳定编码, std::uint64_t> heights;
    while (!stack.empty()) {
        const auto f = stack.back(); stack.pop_back();
        标量要求(f.depth <= budget.最大深度, SS::预算不足);
        if (done.contains(f.id)) {
            标量要求(heights.at(f.id) <= budget.最大深度 - f.depth + 1, SS::预算不足);
            continue;
        }
        if (f.after) {
            auto& n = nodes.at(f.id); std::uint32_t max = 0; std::uint64_t height = 1;
            for (auto src : n.来源) {
                标量要求(done.contains(src), SS::内部不一致);
                max = std::max(max, nodes.at(src).事实.实际阶次);
                标量要求(heights.at(src) < budget.最大深度, SS::预算不足);
                height = std::max(height, heights.at(src) + 1);
            }
            if (!n.来源.empty()) 标量要求(max != UINT32_MAX && n.事实.实际阶次 == max + 1, SS::来源不匹配);
            标量要求(height <= budget.最大深度 - f.depth + 1, SS::预算不足);
            heights.emplace(f.id, height);
            active.erase(f.id); done.insert(f.id); continue;
        }
        标量要求(!active.contains(f.id), SS::来源不匹配);
        auto [it, inserted] = nodes.emplace(f.id, parse(f.id)); 标量要求(inserted);
        active.insert(f.id); stack.push_back({f.id, f.depth, true});
        if (!it->second.来源.empty()) 标量要求(f.depth < budget.最大深度, SS::预算不足);
        for (auto i = it->second.来源.rbegin(); i != it->second.来源.rend(); ++i)
            stack.push_back({*i, f.depth + 1, false});
    }
    auto out = nodes.at(root.结点); 标量要求(out.事实.实际阶次 > 1, SS::格式不支持);
    标量守卫(g); return out;
}
inline 旧派生治理读取结果 特征类数据服务::读取旧派生治理事实(
    const 特征类标量派生读取请求& r) const {
    旧派生治理读取结果 o; o.Gread = r.Gread; o.H = r.H;
    try {
        std::lock_guard<std::mutex> lock(mutex_);
        标量要求(r.版本 == 2 && r.H && r.H <= r.Gread && 有效(r.定义身份.结点) && r.预算.有效(), SS::入口拒绝);
        标量守卫(r.Gread);
        o.事实 = 旧读取闭包(r.定义身份, r.Gread, r.H, 旧路由(r.Gread, r.H), r.预算).事实;
        标量守卫(r.Gread); o.状态 = SS::已读取; 标量要求(o.成功()); return o;
    } catch (const 标量失败& e) { o.状态 = e.状态; }
    catch (S e) { o.状态 = 标量映射(e); }
    catch (const std::bad_alloc&) { o.状态 = SS::资源失败; }
    catch (const std::length_error&) { o.状态 = SS::资源失败; }
    catch (...) { o.状态 = SS::内部不一致; }
    o.事实.reset(); return o;
}
inline 特征类标量派生写结果 特征类数据服务::退出旧派生治理定义(
    const 特征类标量派生退出请求& r) {
    特征类标量派生写结果 o;bool dispatched=false,prior=false,keyUnknown=false,ownsRecovery=false;
    std::lock_guard<std::mutex> lock(mutex_);
    try {
        标量要求(r.版本 == 2 && r.G && 有效(r.定义身份.结点) && 有效(r.幂等身份)
            && r.预算.有效(), SS::入口拒绝);
        o.旧治理原请求 = r; o.Gread = 标量当前G();
        const auto p = 旧路由(o.Gread, 0);
        标量要求(!pending_ || (pending_->旧治理 && *pending_->旧治理 == r && pending_->区 == p),
            SS::已可能发布);
        if(scalar_pending_) {
            const auto* original=std::get_if<特征类标量派生退出请求>(&scalar_pending_->原请求);
            标量要求(scalar_pending_->旧治理&&original&&*original==r,SS::已可能发布);
        } else scalar_pending_.emplace(待确认标量业务{true,r});
        ownsRecovery=true;keyUnknown=true;
        const auto first = 端口(p).读取首次写入材料({L1所有者范围首次写入读取合同版本, r.幂等身份});
        标量要求(first.读取事实代次 == o.Gread, SS::事实代次漂移);
        标量要求(first.合同版本 == L1所有者范围首次写入读取合同版本
            && first.所有者 == 端口(p).所有者身份() && first.写入幂等身份 == r.幂等身份);
        WS ws; ws.合同版本 = L1所有者范围CRUD合同版本; ws.期望事实代次 = r.G; ws.写入幂等身份 = r.幂等身份;
        if (first.状态 == L1所有者范围读取状态::成功) {
            prior=true;keyUnknown=false;
            标量要求(first.首次规范化写集 && first.首次写入结果);
            const auto& recorded = *first.首次规范化写集;
            标量要求(recorded.合同版本 == L1所有者范围CRUD合同版本 && recorded.期望事实代次 == r.G
                && recorded.写入幂等身份 == r.幂等身份 && recorded.节点.empty() && recorded.关系.empty()
                && recorded.值.empty() && recorded.属性槽变更.empty()
                && (recorded.退出事实.size() == 9 || recorded.退出事实.size() == 10)
                && std::count(recorded.退出事实.begin(), recorded.退出事实.end(), r.定义身份.结点) == 1,
                SS::幂等冲突);
            const auto& saved = *first.首次写入结果;
            o.正式回执 = saved; if (saved.事实代次) o.首次发布H = saved.事实代次;
            标量要求(saved.合同版本 == L1所有者范围CRUD合同版本 && saved.所有者 == 端口(p).所有者身份()
                && saved.写入幂等身份 == r.幂等身份 && saved.状态 == L1所有者范围写入状态::成功
                && saved.是否形成内存权威发布 && saved.事实代次 > r.G && saved.事实代次 <= o.Gread);
            try {
                ws.退出事实 = 旧读取闭包(r.定义身份, o.Gread, r.G, p, r.预算).自有;
            } catch (const 标量失败& e) {
                // 退出集合内的注册等节点不是原目标；已绑定键的确定结构不符就是键冲突。
                if (e.状态 == SS::格式不支持 || e.状态 == SS::来源不匹配
                    || e.状态 == SS::未找到 || e.状态 == SS::已退出) throw 标量失败{SS::幂等冲突};
                throw;
            }
            标量要求(ws == *first.首次规范化写集 && saved.新编码映射.empty(), SS::幂等冲突);
        } else if (first.状态 == L1所有者范围读取状态::未找到) {
            标量要求(!first.首次规范化写集&&!first.首次写入结果);
            keyUnknown=false;
            标量守卫(r.G);
            ws.退出事实 = 旧读取闭包(r.定义身份, r.G, r.G, p, r.预算).自有;
        } else throw 标量失败{标量映射(first.状态)};
        if (pending_) 标量要求(pending_->请求 == ws, SS::幂等冲突);
        else pending_.emplace(待确认写入{p, ws, false, r});
        dispatched = true;
        const auto receipt = 端口(p).提交所有者范围中性写集(ws); o.正式回执 = receipt;
        const bool ok = receipt.状态 == L1所有者范围写入状态::成功
            || receipt.状态 == L1所有者范围写入状态::精确重复;
        if (!ok) {
            if (!prior && !receipt.是否形成内存权威发布
                && (receipt.状态 == L1所有者范围写入状态::事实代次漂移
                    || receipt.状态 == L1所有者范围写入状态::引用冲突
                    || receipt.状态 == L1所有者范围写入状态::入口拒绝)) {
                pending_.reset(); dispatched = false; o.发布确定性 = SP::确认未发布;
                throw 标量失败{标量映射(映射(receipt.状态))};
            }
            throw 标量失败{SS::已可能发布};
        }
        o.首次发布H = receipt.事实代次;
        标量要求(receipt.合同版本 == L1所有者范围CRUD合同版本 && receipt.所有者 == 端口(p).所有者身份()
            && receipt.写入幂等身份 == r.幂等身份 && receipt.事实代次 > r.G
            && receipt.新编码映射.empty() && (!prior || receipt.状态 == L1所有者范围写入状态::精确重复));
        o.Gread = 标量当前G();
        const auto after = 旧读取闭包(r.定义身份, o.Gread, r.G, p, r.预算);
        标量要求(after.自有 == ws.退出事实 && after.事实.退出G == o.首次发布H);
        for (auto id : ws.退出事实) {
            const auto fact = 原始事实(id, o.Gread);
            std::visit([&](const auto& f) {
                标量要求(f.写入所有者 == 端口(p).所有者身份() && f.退出事实代次 == o.首次发布H);
            }, fact);
        }
        标量守卫(o.Gread);
        o.旧治理退出 = 旧派生治理退出事实{after.事实, ws.退出事实};
        o.发布确定性 = SP::确认已发布;
        o.状态 = receipt.状态 == L1所有者范围写入状态::成功 ? SS::已退出 : SS::精确重复;
        标量要求(o.成功()); pending_.reset();scalar_pending_.reset();return o;
    } catch (const 标量失败& e) { o.状态 = e.状态; }
    catch (S e) { o.状态 = 标量映射(e); }
    catch (const std::bad_alloc&) { o.状态 = SS::资源失败; }
    catch (const std::length_error&) { o.状态 = SS::资源失败; }
    catch (...) { o.状态 = SS::内部不一致; }
    if(o.状态==SS::幂等冲突) {
        o.发布确定性=SP::确认未发布;
        if(ownsRecovery){pending_.reset();scalar_pending_.reset();}
    } else if (dispatched || prior || keyUnknown) {
        o.状态=SS::已可能发布;o.发布确定性=SP::可能已发布;
    } else if(ownsRecovery){pending_.reset();scalar_pending_.reset();}
    o.旧治理退出.reset(); o.定义事实.reset(); return o;
}

} // namespace 海中鱼巣
