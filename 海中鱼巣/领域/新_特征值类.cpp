#include "新_特征值类.h"
#include "新_特征值.恢复.h"
#include "../核心/基础数据集.持久运行包.h"
#include "../核心/容错检查.h"

#include <algorithm>
#include <bit>
#include <limits>
#include <map>
#include <mutex>
#include <new>
#include <set>
#include <string_view>
#include <stdexcept>
#include <utility>

namespace 海中鱼巣 {

namespace {

constexpr std::string_view 旧压缩类型标记 = "HY-Ego/2100/compression-layer-type/v1";
constexpr std::array<std::string_view, 7> 恢复标记组{
    "HY-Ego/2100/value-node-type-field/v1",
    "HY-Ego/2100/value-node-type/v1",
    "HY-Ego/2100/compression-layer-2d-type/v1",
    "HY-Ego/2100/compression-layer-3d-type/v1",
    "HY-Ego/2100/material-physical-type-field/v1",
    "HY-Ego/2100/original-material-field/v1",
    "HY-Ego/2100/compressed-material-field/v1"
};

using 特征值结构键组 = std::array<稳定编码, 恢复标记组.size()>;

bool 是特征值恢复标记(const 基础字段关系& 字段, 稳定编码 键, std::string_view 标记) noexcept {
    const auto* 值 = std::get_if<基础值>(&字段.内容);
    const auto* 字符串 = 值 ? std::get_if<std::string>(&值->材料) : nullptr;
    return 字段.所属节点 == 键 && 字段.字段节点 == 键 && 字符串 && *字符串 == 标记;
}

[[noreturn]] void 特征值恢复工程失败(const wchar_t* 入口, const wchar_t* 原因) noexcept {
    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, 入口, 原因);
}

struct 材料键 final {
    材料物理类型 类型 = 材料物理类型::U64数组;
    新特征值材料 材料;

    bool operator<(const 材料键& 另一项) const noexcept {
        if (类型 != 另一项.类型) {
            return static_cast<std::int64_t>(类型)
                < static_cast<std::int64_t>(另一项.类型);
        }
        return 材料 < 另一项.材料;
    }
};

std::optional<std::size_t> 二维边长(std::size_t 总位数) noexcept;
std::optional<std::size_t> 三维边长(std::size_t 总位数) noexcept;

bool 合法UTF8(std::string_view 文本) noexcept {
    for (std::size_t i = 0; i < 文本.size();) {
        const auto 首 = static_cast<unsigned char>(文本[i++]);
        if (首 < 0x80) continue;
        if (首 < 0xC2 || 首 > 0xF4) return false;
        const std::size_t 续数 = 首 < 0xE0 ? 1 : 首 < 0xF0 ? 2 : 3;
        if (文本.size() - i < 续数) return false;
        const auto 第二 = static_cast<unsigned char>(文本[i]);
        if ((首 == 0xE0 && 第二 < 0xA0) || (首 == 0xED && 第二 >= 0xA0)
            || (首 == 0xF0 && 第二 < 0x90) || (首 == 0xF4 && 第二 >= 0x90)) return false;
        for (std::size_t j = 0; j < 续数; ++j) {
            if ((static_cast<unsigned char>(文本[i++]) & 0xC0) != 0x80) return false;
        }
    }
    return true;
}

bool 是已知物理类型(材料物理类型 类型) noexcept {
    switch (类型) {
    case 材料物理类型::U64数组:
    case 材料物理类型::I64数组:
    case 材料物理类型::UTF8字符串:
    case 材料物理类型::二维二值格:
    case 材料物理类型::三维二值格:
        return true;
    }
    return false;
}

bool 材料形态匹配(材料物理类型 类型, const 新特征值材料& 材料) noexcept {
    switch (类型) {
    case 材料物理类型::U64数组:
        return std::holds_alternative<std::vector<std::uint64_t>>(材料);
    case 材料物理类型::二维二值格:
    case 材料物理类型::三维二值格: {
        const auto* 位图 = std::get_if<std::vector<std::uint64_t>>(&材料);
        if (!位图) return false;
        if (特征值内部::解析全零格字数(类型, *位图)) return true;
        if (位图->size() > (std::numeric_limits<std::size_t>::max)() / 64) return false;
        const auto 位数 = 位图->size() * 64;
        const auto 边长 = 类型 == 材料物理类型::二维二值格 ? 二维边长(位数) : 三维边长(位数);
        return 边长 && std::ranges::any_of(*位图, [](std::uint64_t 字) { return 字 != 0; });
    }
    case 材料物理类型::I64数组:
        return std::holds_alternative<std::vector<std::int64_t>>(材料);
    case 材料物理类型::UTF8字符串: {
        const auto* 文本 = std::get_if<std::string>(&材料);
        return 文本 && 合法UTF8(*文本);
    }
    }
    return false;
}

bool 安全平方(std::size_t 值, std::size_t& 结果) noexcept {
    if (值 != 0 && 值 > (std::numeric_limits<std::size_t>::max)() / 值) return false;
    结果 = 值 * 值;
    return true;
}

bool 安全立方(std::size_t 值, std::size_t& 结果) noexcept {
    std::size_t 平方 = 0;
    if (!安全平方(值, 平方)
        || (值 != 0 && 平方 > (std::numeric_limits<std::size_t>::max)() / 值)) {
        return false;
    }
    结果 = 平方 * 值;
    return true;
}

std::optional<std::size_t> 二维边长(std::size_t 总位数) noexcept {
    std::size_t 边长 = 8;
    for (;;) {
        std::size_t 当前位数 = 0;
        if (!安全平方(边长, 当前位数)) return std::nullopt;
        if (当前位数 == 总位数) return 边长;
        if (当前位数 > 总位数
            || 边长 > (std::numeric_limits<std::size_t>::max)() / 2) {
            return std::nullopt;
        }
        边长 *= 2;
    }
}

std::optional<std::size_t> 三维边长(std::size_t 总位数) noexcept {
    std::size_t 边长 = 4;
    for (;;) {
        std::size_t 当前位数 = 0;
        if (!安全立方(边长, 当前位数)) return std::nullopt;
        if (当前位数 == 总位数) return 边长;
        if (当前位数 > 总位数
            || 边长 > (std::numeric_limits<std::size_t>::max)() / 2) {
            return std::nullopt;
        }
        边长 *= 2;
    }
}

bool 读取位(const std::vector<std::uint64_t>& 材料, std::size_t 位置) noexcept {
    return (材料[位置 / 64] & (std::uint64_t{1} << (位置 % 64))) != 0;
}

void 写入位(std::vector<std::uint64_t>& 材料, std::size_t 位置) noexcept {
    材料[位置 / 64] |= std::uint64_t{1} << (位置 % 64);
}

std::vector<std::uint64_t> 压缩二维一层(
    const std::vector<std::uint64_t>& 下层, std::size_t 下层边长) {
    const auto 上层边长 = 下层边长 / 2;
    std::vector<std::uint64_t> 上层(上层边长 * 上层边长 / 64, 0);
    for (std::size_t y = 0; y < 下层边长; ++y) {
        for (std::size_t x = 0; x < 下层边长; ++x) {
            if (读取位(下层, y * 下层边长 + x))
                写入位(上层, (y / 2) * 上层边长 + x / 2);
        }
    }
    return 上层;
}

std::vector<std::uint64_t> 压缩三维一层(
    const std::vector<std::uint64_t>& 下层, std::size_t 下层边长) {
    const std::size_t 上层边长 = 下层边长 / 2;
    const std::size_t 上层位数 = 上层边长 * 上层边长 * 上层边长;
    std::vector<std::uint64_t> 上层(上层位数 / 64, 0);

    for (std::size_t 上层Z = 0; 上层Z < 上层边长; ++上层Z) {
        for (std::size_t 上层Y = 0; 上层Y < 上层边长; ++上层Y) {
            for (std::size_t 上层X = 0; 上层X < 上层边长; ++上层X) {
                bool 占用 = false;
                for (std::size_t 偏移Z = 0; 偏移Z < 2 && !占用; ++偏移Z) {
                    for (std::size_t 偏移Y = 0; 偏移Y < 2 && !占用; ++偏移Y) {
                        for (std::size_t 偏移X = 0; 偏移X < 2; ++偏移X) {
                            const std::size_t 下层X = 上层X * 2 + 偏移X;
                            const std::size_t 下层Y = 上层Y * 2 + 偏移Y;
                            const std::size_t 下层Z = 上层Z * 2 + 偏移Z;
                            const std::size_t 下层位置 =
                                (下层Z * 下层边长 + 下层Y) * 下层边长 + 下层X;
                            if (读取位(下层, 下层位置)) {
                                占用 = true;
                                break;
                            }
                        }
                    }
                }
                if (占用) {
                    const std::size_t 上层位置 =
                        (上层Z * 上层边长 + 上层Y) * 上层边长 + 上层X;
                    写入位(上层, 上层位置);
                }
            }
        }
    }
    return 上层;
}

std::optional<std::vector<std::vector<std::uint64_t>>> 生成压缩层(
    材料物理类型 类型, const 新特征值材料& 材料) {
    if (类型 != 材料物理类型::二维二值格
        && 类型 != 材料物理类型::三维二值格) {
        return std::vector<std::vector<std::uint64_t>>{};
    }

    const auto* 原始材料 = std::get_if<std::vector<std::uint64_t>>(&材料);
    if (!原始材料
        || 原始材料->size() > (std::numeric_limits<std::size_t>::max)() / 64) {
        return std::nullopt;
    }
    if (特征值内部::解析全零格字数(类型, *原始材料)) return std::vector<std::vector<std::uint64_t>>{};
    const std::size_t 总位数 = 原始材料->size() * 64;
    const auto 边长 = 类型 == 材料物理类型::二维二值格
        ? 二维边长(总位数) : 三维边长(总位数);
    if (!边长) return std::nullopt;

    std::vector<std::vector<std::uint64_t>> 层{*原始材料};
    const bool 二维 = 类型 == 材料物理类型::二维二值格;
    const std::size_t 根边长 = 二维 ? 8 : 4;
    for (auto 当前边长 = *边长; 当前边长 > 根边长; 当前边长 /= 2) {
        层.push_back(二维 ? 压缩二维一层(层.back(), 当前边长)
            : 压缩三维一层(层.back(), 当前边长));
    }
    return 层;
}

std::optional<稳定编码> 读取唯一节点字段(
    const 基础数据集& 数据集, 稳定编码 节点, 稳定编码 字段节点) {
    const auto 字段 = 数据集.查询字段(节点, 字段节点);
    if (字段.size() != 1) return std::nullopt;
    const auto* 目标 = std::get_if<稳定编码>(&字段.front().内容);
    if (!目标 || !有效(*目标)) return std::nullopt;
    return *目标;
}

std::optional<基础值> 读取唯一值字段(
    const 基础数据集& 数据集, 稳定编码 节点, 稳定编码 字段节点) {
    const auto 字段 = 数据集.查询字段(节点, 字段节点);
    if (字段.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&字段.front().内容);
    return 值 ? std::optional<基础值>{*值} : std::nullopt;
}

}

std::optional<std::size_t> 特征值内部::解析全零格字数(材料物理类型 类型,
    std::span<const std::uint64_t> 材料) noexcept {
    const bool 二维 = 类型 == 材料物理类型::二维二值格;
    if ((!二维 && 类型 != 材料物理类型::三维二值格) || 材料.size() != 3
        || 材料[0] != 0 || 材料[1] != (二维 ? 1u : 2u)
        || 材料[2] < (二维 ? 8u : 4u) || !std::has_single_bit(材料[2])
        || 材料[2] > (std::numeric_limits<std::size_t>::max)()) return std::nullopt;

    const auto 边长 = static_cast<std::size_t>(材料[2]);
    std::size_t 位数 = 0;
    if (!(二维 ? 安全平方(边长, 位数) : 安全立方(边长, 位数))) return std::nullopt;
    return 位数 / 64;
}

namespace {
using 值恢复状态 = 新特征值运行恢复状态;
using 材料索引 = std::map<材料键, std::vector<稳定编码>>;

struct 恢复材料候选 final {
    值恢复状态 状态 = 值恢复状态::结构不一致;
    特征值结构键组 键{};
    材料索引 普通, 根;
};

std::optional<稳定编码> 读取材料父节点(const 基础联合节点快照& 快照) {
    std::optional<稳定编码> 父;
    for (const auto& 边 : 快照.入向外部关系组) {
        if (边.类型 != 基础外部关系类型::父子) continue;
        if (父 || 边.角色或顺序 != 0 || 边.目标节点 != 快照.结构.节点.编码)
            throw 值恢复状态::结构不一致;
        父 = 边.源节点;
    }
    return 父;
}

恢复材料候选 构造恢复材料候选(const 基础持久视图& 视图) {
    using P = 新特征值保护读取状态;
    using B = 基础结构操作状态;
    const auto 结构 = 读取恢复特征值结构(视图);
    if (结构.状态 == P::格式不支持) return {值恢复状态::格式不支持};
    if (结构.状态 == P::资源失败) return {值恢复状态::资源失败};
    if (结构.状态 != P::已读取) return {};

    struct 压缩层 final {
        材料物理类型 类型;
        std::vector<std::uint64_t> 材料;
        std::optional<稳定编码> 父;
        std::size_t 边长;
    };
    const auto& 键 = 结构.键;
    const auto& 数据 = 视图.结构();
    const auto 读取类型成员 = [&](稳定编码 类型) {
        std::vector<稳定编码> 成员;
        for (const auto& 项 : 数据.查询目标字段(类型, 键[0])) 成员.push_back(项.所属节点);
        if (成员.empty()) return std::vector<基础联合节点读取项>{};
        auto 批 = 数据.查询节点组完整结构(成员);
        if (批.状态 == B::资源失败) throw 值恢复状态::资源失败;
        if (批.状态 != B::已完成 || 批.节点组.size() != 成员.size())
            throw 值恢复状态::结构不一致;
        return 批.节点组;
    };
    恢复材料候选 候选;
    候选.键 = 键;
    std::map<稳定编码, 压缩层> 层组;
    std::map<std::pair<std::optional<稳定编码>, 材料键>, 稳定编码> 层位置;
    for (const auto 类型 : {材料物理类型::二维二值格, 材料物理类型::三维二值格}) {
        const bool 二维 = 类型 == 材料物理类型::二维二值格;
        const auto 类型节点 = 键[二维 ? 2 : 3];
        for (const auto& 项 : 读取类型成员(类型节点)) {
            if (!项.快照) return {};
            const auto& 快照 = *项.快照;
            const auto& 字段 = 快照.结构.字段组;
            if (快照.结构.节点.编码 != 项.查询节点 || 字段.size() != 2 || !快照.入向字段组.empty()
                || std::ranges::any_of(快照.入向外部关系组, [](const auto& 边) {
                    return 边.类型 != 基础外部关系类型::父子;
                })) return {};
            const auto 类型项 = std::ranges::find(字段, 键[0], &基础字段关系::字段节点);
            const auto 材料项 = std::ranges::find(字段, 键[6], &基础字段关系::字段节点);
            if (类型项 == 字段.end() || 类型项->内容 != 基础字段内容{类型节点} || 材料项 == 字段.end()) return {};
            const auto* 值 = std::get_if<基础值>(&材料项->内容);
            const auto* 材料 = 值 ? std::get_if<std::vector<std::uint64_t>>(&值->材料) : nullptr;
            if (!材料 || 材料->size() > (std::numeric_limits<std::size_t>::max)() / 64) return {};
            const auto 位数 = 材料->size() * 64;
            const auto 边长 = 二维 ? 二维边长(位数) : 三维边长(位数);
            if (!边长 || std::ranges::none_of(*材料, [](auto 字) { return 字 != 0; })) return {};
            const auto 父 = 读取材料父节点(快照);
            const auto 节点 = 项.查询节点;
            const auto 索引键 = 材料键{类型, *材料};
            if (!层位置.emplace(std::pair{父, 索引键}, 节点).second
                || !层组.emplace(节点, 压缩层{类型, *材料, 父, *边长}).second) return {};
            if (!父) {
                if (材料->size() != 1) return {};
                候选.根.emplace(索引键, std::vector{节点});
            }
        }
    }

    for (const auto& [节点, 层] : 层组) {
        if (!层.父) continue;
        const auto 上级 = 层组.find(*层.父);
        if (上级 == 层组.end() || 上级->second.类型 != 层.类型
            || 上级->second.边长 != 层.边长 / 2 || 层.材料.size() == 1) return {};
        const auto 汇总 = 层.类型 == 材料物理类型::二维二值格
            ? 压缩二维一层(层.材料, 层.边长) : 压缩三维一层(层.材料, 层.边长);
        if (汇总 != 上级->second.材料) return {};
    }

    std::map<稳定编码, std::optional<稳定编码>> 原值父组;
    std::set<材料键> 已见原值;
    for (const auto& 项 : 读取类型成员(键[1])) {
        if (!项.快照 || 项.快照->结构.节点.编码 != 项.查询节点
            || 项.快照->结构.字段组.size() != 3) return {};
        const auto 解码 = 新_特征值类::解码特征值快照(项.快照->结构, 结构.解码结构());
        if (解码.状态 != 新特征值快照解码状态::已解码 || !解码.信息) return {};
        const auto& 原值 = *解码.信息;
        const auto 索引键 = 材料键{原值.物理类型, 原值.材料};
        const auto 父 = 读取材料父节点(*项.快照);
        if (!已见原值.insert(索引键).second || !原值父组.emplace(原值.节点, 父).second) return {};
        const auto 层材料 = 生成压缩层(原值.物理类型, 原值.材料);
        if (!层材料) return {};
        if (层材料->empty()) {
            if (父) return {};
            候选.普通.emplace(索引键, std::vector{原值.节点});
            continue;
        }
        auto 当前 = 父;
        for (std::size_t i = 层材料->size() == 1 ? 0 : 1; i < 层材料->size(); ++i) {
            if (!当前) return {};
            const auto 层 = 层组.find(*当前);
            if (层 == 层组.end() || 层->second.类型 != 原值.物理类型
                || 层->second.材料 != (*层材料)[i]) return {};
            当前 = 层->second.父;
        }
        if (当前) return {};
    }
    for (const auto& [节点, 层] : 层组) {
        const auto 读 = 数据.查询节点组完整结构({节点});
        if (读.状态 == B::资源失败) throw 值恢复状态::资源失败;
        if (读.状态 != B::已完成 || 读.节点组.size() != 1 || !读.节点组.front().快照) return {};
        for (const auto& 边 : 读.节点组.front().快照->出向外部关系组) {
            if (边.类型 != 基础外部关系类型::父子 || 边.角色或顺序 != 0) return {};
            const auto 子层 = 层组.find(边.目标节点);
            const auto 子原值 = 原值父组.find(边.目标节点);
            if ((子层 == 层组.end() || 子层->second.父 != 节点)
                && (子原值 == 原值父组.end() || 子原值->second != 节点)) return {};
        }
    }
    候选.状态 = 值恢复状态::已恢复;
    return 候选;
}
}

class 新_特征值类::实现 final {
public:
    explicit 实现(基础数据集& 数据集) : 数据集_(数据集) {}

    bool 初始化() noexcept {
        特征值结构键组 已建节点{};
        bool 已发表 = false;
        const auto 清理未发表 = [&]() noexcept {
            for (auto 位置 = 已建节点.rbegin(); 位置 != 已建节点.rend(); ++位置) {
                if (有效(*位置)) 数据集_.删除节点(*位置);
            }
        };
        try {
            std::scoped_lock 锁(互斥_);
            if (已初始化_) {
                const 特征值结构键组 已有{节点类型字段_, 特征值类型节点_, 二维压缩类型节点_,
                    三维压缩类型节点_, 材料物理类型字段_, 原始材料字段_, 压缩材料字段_};
                for (std::size_t i = 0; i < 已有.size(); ++i) {
                    const auto 字段 = 数据集_.查询字段(已有[i], 已有[i]);
                    if (字段.size() != 1 || !是特征值恢复标记(字段.front(), 已有[i], 恢复标记组[i]))
                        return false;
                }
                return true;
            }
            if (基础持久运行包::读取已绑定会话(数据集_)) return false;

            for (auto& 节点 : 已建节点) {
                节点 = 数据集_.新建节点();
                if (!有效(节点)) {
                    清理未发表();
                    return false;
                }
            }

            const auto 键结构 = 数据集_.查询节点组完整结构(
                std::vector<稳定编码>(已建节点.begin(), 已建节点.end()));
            if (键结构.状态 != 基础结构操作状态::已完成
                || 键结构.节点组.size() != 已建节点.size()) {
                清理未发表();
                return false;
            }

            基础节点组追加请求 请求;
            for (const auto& 项 : 键结构.节点组) {
                if (!项.快照) {
                    清理未发表();
                    return false;
                }
                请求.预期联合节点组.push_back(*项.快照);
            }
            for (std::size_t i = 0; i < 已建节点.size(); ++i) {
                const auto 键 = 已建节点[i];
                请求.只读预期.节点组.push_back({键});
                请求.只读预期.字段集合组.push_back({键, 键, {}, {}});
                请求.追加字段组.push_back({键, 键, 基础值{std::string(恢复标记组[i])}});
            }
            const auto 写入 = 数据集_.原子追加节点组(请求);
            if (写入.状态 != 基础节点组追加状态::已完成) {
                清理未发表();
                return false;
            }
            已发表 = true;
            for (std::size_t i = 0; i < 已建节点.size(); ++i) {
                const auto 键 = 已建节点[i];
                const auto 字段 = 数据集_.查询字段(键, 键);
                if (字段.size() != 1 || !是特征值恢复标记(字段.front(), 键, 恢复标记组[i]))
                    特征值恢复工程失败(L"特征值结构初始化", L"结构标记完整发表后读回不符。");
            }
            节点类型字段_ = 已建节点[0];
            特征值类型节点_ = 已建节点[1];
            二维压缩类型节点_ = 已建节点[2];
            三维压缩类型节点_ = 已建节点[3];
            材料物理类型字段_ = 已建节点[4];
            原始材料字段_ = 已建节点[5];
            压缩材料字段_ = 已建节点[6];
            已初始化_ = true;
            return true;
        } catch (const std::bad_alloc&) {
            if (已发表) 特征值恢复工程失败(L"特征值结构初始化", L"结构发表后无法完整读回。");
            清理未发表();
            return false;
        } catch (const std::length_error&) {
            if (已发表) 特征值恢复工程失败(L"特征值结构初始化", L"结构发表后读回超出资源限制。");
            清理未发表();
            return false;
        } catch (...) {
            特征值恢复工程失败(L"特征值结构初始化", L"结构标记发表发生未预期工程异常。");
        }
    }

    值恢复状态 恢复当前权威() noexcept {
        try {
            const auto 会话 = 基础持久运行包::读取已绑定会话(数据集_);
            const auto 视图 = 会话 ? 会话->读取() : nullptr;
            if (!视图) return 值恢复状态::未绑定;
            auto 候选 = 构造恢复材料候选(*视图);
            if (候选.状态 != 值恢复状态::已恢复) return 候选.状态;
            std::scoped_lock 锁(互斥_);
            if (基础持久运行包::读取已绑定会话(数据集_) != 会话 || 会话->读取() != 视图)
                return 值恢复状态::前提已变化;
            if (已初始化_) {
                if (恢复会话_ != 会话 || 当前结构键() != 候选.键) return 值恢复状态::结构不一致;
                if (索引视图_ == 视图) return 值恢复状态::精确复用;
            }
            const auto& 键 = 候选.键;
            节点类型字段_ = 键[0];
            特征值类型节点_ = 键[1];
            二维压缩类型节点_ = 键[2];
            三维压缩类型节点_ = 键[3];
            材料物理类型字段_ = 键[4];
            原始材料字段_ = 键[5];
            压缩材料字段_ = 键[6];
            普通材料索引_.swap(候选.普通);
            根索引_.swap(候选.根);
            恢复会话_ = 会话;
            索引视图_ = 视图;
            已初始化_ = true;
            return 值恢复状态::已恢复;
        } catch (值恢复状态 状态) {
            return 状态;
        } catch (const std::bad_alloc&) {
            return 值恢复状态::资源失败;
        } catch (const std::length_error&) {
            return 值恢复状态::资源失败;
        } catch (...) {
            特征值恢复工程失败(L"恢复当前特征值权威", L"恢复运行期材料索引发生工程异常。");
        }
    }

    稳定编码 保存或取得特征值(
        材料物理类型 类型, const 新特征值材料& 材料) noexcept {
        try {
            std::unique_lock 锁(互斥_);
            if (!已初始化_ || !材料形态匹配(类型, 材料)) {
                return {};
            }
            if (恢复会话_) {
                锁.unlock();
                const auto 既有 = 查询特征值(类型, 材料);
                return 既有.size() == 1 ? 既有.front() : 稳定编码{};
            }
            const auto 层 = 生成压缩层(类型, 材料);
            if (!层) return {};
            return 层->empty()
                ? 保存或取得普通材料(类型, 材料)
                : 保存或取得二值格(类型, 材料, *层);
        } catch (...) {
            return {};
        }
    }

    std::optional<新特征值信息> 获取特征值(稳定编码 节点) const noexcept {
        try {
            std::scoped_lock 锁(互斥_);
            const auto 视图 = 恢复会话_ ? 恢复会话_->读取() : nullptr;
            return 获取特征值已加锁(节点, 视图 ? 视图->结构() : 数据集_);
        } catch (const std::exception& 异常) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"获取特征值", L"材料读取异常。", 异常.what());
        } catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"获取特征值", L"材料读取未知异常。");
        }
    }

    新特征值保护读取结果 读取特征值保护(稳定编码 节点) const {
        using 状态 = 新特征值保护读取状态;
        using 底座状态 = 基础结构操作状态;
        using 解码状态 = 新特征值快照解码状态;

        if (!有效(节点)) return {状态::输入拒绝, {}, {}};
        try {
            std::scoped_lock 锁(互斥_);
            if (!已初始化_) return {状态::未初始化, {}, {}};

            const auto 视图 = 恢复会话_ ? 恢复会话_->读取() : nullptr;
            const auto 读取 = (视图 ? 视图->结构() : 数据集_).查询完整节点结构(节点);
            if (读取.状态 == 底座状态::节点不存在) return {状态::节点不存在, {}, {}};
            if (读取.状态 == 底座状态::资源失败) return {状态::资源失败, {}, {}};
            if (读取.状态 != 底座状态::已完成 || !读取.结构
                || 读取.结构->节点.编码 != 节点) {
                return {状态::结构不一致, {}, {}};
            }

            auto 解码 = 解码特征值快照已加锁(*读取.结构);
            if (解码.状态 == 解码状态::非本类节点) return {状态::非本类节点, {}, {}};
            if (解码.状态 != 解码状态::已解码 || !解码.信息) {
                return {状态::结构不一致, {}, {}};
            }
            解码.窄预期.节点组.push_back(基础节点{特征值类型节点_});
            return {状态::已读取, std::move(解码.信息), std::move(解码.窄预期)};
        } catch (const std::bad_alloc&) {
            return {状态::资源失败, {}, {}};
        } catch (const std::length_error&) {
            return {状态::资源失败, {}, {}};
        }
    }

    新特征值快照解码结果 解码特征值快照(const 基础节点结构& 结构) const {
        std::scoped_lock 锁(互斥_);
        return 解码特征值快照已加锁(结构);
    }

    std::optional<新特征值快照解码结构> 读取快照解码结构() const {
        std::scoped_lock 锁(互斥_);
        if (!已初始化_) return {};
        return 新特征值快照解码结构{节点类型字段_,特征值类型节点_,材料物理类型字段_,原始材料字段_};
    }

    std::vector<稳定编码> 查询特征值(
        材料物理类型 类型, const 新特征值材料& 材料) const noexcept {
        try {
            std::unique_lock 锁(互斥_);
            if (!已初始化_ || !材料形态匹配(类型, 材料)) {
                return {};
            }
            const auto 层 = 生成压缩层(类型, 材料);
            if (!层) return {};
            const auto 会话 = 恢复会话_;
            const auto 视图 = 会话 ? 会话->读取() : nullptr;
            if (视图 && 索引视图_ != 视图) {
                锁.unlock();
                auto 候选 = 构造恢复材料候选(*视图);
                锁.lock();
                if (候选.状态 != 值恢复状态::已恢复 || 候选.键 != 当前结构键()
                    || 会话->读取() != 视图) return {};
                普通材料索引_.swap(候选.普通);
                根索引_.swap(候选.根);
                索引视图_ = 视图;
            }
            const auto& 数据 = 视图 ? 视图->结构() : 数据集_;
            return 层->empty()
                ? 查询普通材料(类型, 材料, 数据)
                : 查询二值格(类型, 材料, *层, 数据);
        } catch (...) {
            return {};
        }
    }

    bool 是特征值节点(稳定编码 节点) const noexcept {
        try {
            std::scoped_lock 锁(互斥_);
            const auto 视图 = 恢复会话_ ? 恢复会话_->读取() : nullptr;
            return 已初始化_ && 是指定类型节点(节点, 特征值类型节点_, 视图 ? 视图->结构() : 数据集_);
        } catch (...) {
            return false;
        }
    }

private:
    特征值结构键组 当前结构键() const noexcept {
        return {节点类型字段_, 特征值类型节点_, 二维压缩类型节点_, 三维压缩类型节点_,
            材料物理类型字段_, 原始材料字段_, 压缩材料字段_};
    }

    bool 是指定类型节点(稳定编码 节点, 稳定编码 类型节点, const 基础数据集& 数据) const {
        const auto 实际类型 = 读取唯一节点字段(数据, 节点, 节点类型字段_);
        return 实际类型 && *实际类型 == 类型节点;
    }

    稳定编码 压缩类型节点(材料物理类型 类型) const noexcept {
        return 类型 == 材料物理类型::二维二值格 ? 二维压缩类型节点_ : 三维压缩类型节点_;
    }

    bool 压缩材料相同(稳定编码 节点, 材料物理类型 类型,
        const std::vector<std::uint64_t>& 材料, const 基础数据集& 数据) const {
        if (!是指定类型节点(节点, 压缩类型节点(类型), 数据)) return false;
        const auto 实际材料 = 读取唯一值字段(数据, 节点, 压缩材料字段_);
        if (!实际材料) return false;
        const auto* 值 = std::get_if<std::vector<std::uint64_t>>(&实际材料->材料);
        return 值 && *值 == 材料;
    }

    新特征值快照解码结果 解码特征值快照已加锁(const 基础节点结构& 结构) const {
        using 状态=新特征值快照解码状态;
        if (!已初始化_) return {状态::未初始化,{}};
        return 新_特征值类::解码特征值快照(结构,
            {节点类型字段_,特征值类型节点_,材料物理类型字段_,原始材料字段_});
    }

public:
    static 新特征值快照解码结果 解码快照(
        const 基础节点结构& 结构, const 新特征值快照解码结构& keys) {
        using 状态=新特征值快照解码状态;
        const auto 节点类型字段_=keys.节点类型字段, 特征值类型节点_=keys.特征值类型节点;
        const auto 材料物理类型字段_=keys.材料物理类型字段, 原始材料字段_=keys.原始材料字段;
        if (!有效(节点类型字段_) || !有效(特征值类型节点_)
            || !有效(材料物理类型字段_) || !有效(原始材料字段_)) return {状态::未初始化,{}};
        const 基础字段关系* 类型字段=nullptr;
        const 基础字段关系* 物理字段=nullptr;
        const 基础字段关系* 材料字段=nullptr;
        for (const auto& 字段 : 结构.字段组) {
            const 基础字段关系** 目标=nullptr;
            if (字段.字段节点==节点类型字段_) 目标=&类型字段;
            else if (字段.字段节点==材料物理类型字段_) 目标=&物理字段;
            else if (字段.字段节点==原始材料字段_) 目标=&材料字段;
            if (目标) {
                if (*目标) return {状态::结构不可解码,{}};
                *目标=&字段;
            }
        }
        if (!有效(结构.节点.编码) || !类型字段) return {状态::结构不可解码,{}};
        const auto* 节点类型=std::get_if<稳定编码>(&类型字段->内容);
        if (!节点类型) return {状态::结构不可解码,{}};
        if (*节点类型!=特征值类型节点_) return {状态::非本类节点,{}};
        const auto* 物理值=物理字段 ? std::get_if<基础值>(&物理字段->内容) : nullptr;
        const auto* 原始值=材料字段 ? std::get_if<基础值>(&材料字段->内容) : nullptr;
        const auto* 类型值=物理值 ? std::get_if<std::int64_t>(&物理值->材料) : nullptr;
        if (!类型值 || !原始值) return {状态::结构不可解码,{}};
        const auto 类型=static_cast<材料物理类型>(*类型值);
        if (!是已知物理类型(类型)) return {状态::结构不可解码,{}};
        新特征值材料 材料;
        if (const auto* U64值=std::get_if<std::vector<std::uint64_t>>(&原始值->材料)) 材料=*U64值;
        else if (const auto* I64值=std::get_if<std::vector<std::int64_t>>(&原始值->材料)) 材料=*I64值;
        else if (const auto* 字符串值=std::get_if<std::string>(&原始值->材料)) 材料=*字符串值;
        else return {状态::结构不可解码,{}};
        if (!材料形态匹配(类型,材料)) return {状态::结构不可解码,{}};
        基础只读预期 预期;
        预期.节点组.push_back(结构.节点);
        for(const auto key:{节点类型字段_,材料物理类型字段_,原始材料字段_}) {
            预期.节点组.push_back(基础节点{key});
            基础只读字段集合预期 item{结构.节点.编码,key,{},{}};
            for(const auto& field:结构.字段组)if(field.字段节点==key)item.字段组.push_back(field);
            预期.字段集合组.push_back(std::move(item));
        }
        return {状态::已解码,新特征值信息{结构.节点.编码,类型,std::move(材料)},std::move(预期)};
    }

private:

    std::optional<新特征值信息> 获取特征值已加锁(稳定编码 节点, const 基础数据集& 数据) const {
        if (!已初始化_ || !有效(节点)) return std::nullopt;
        const auto 读取=数据.查询完整节点结构(节点);
        if (读取.状态==基础结构操作状态::资源失败) throw std::bad_alloc{};
        if (读取.状态!=基础结构操作状态::已完成 || !读取.结构) return std::nullopt;
        return 解码特征值快照已加锁(*读取.结构).信息;
    }

    using 父引用 = std::variant<稳定编码, 基础组内节点引用>;

    基础组内节点引用 准备材料节点(基础节点组追加请求& 请求,
        基础组内新节点 节点, const std::optional<父引用>& 上级 = std::nullopt) const {
        const 基础组内节点引用 槽{请求.新节点组.size()};
        请求.新节点组.push_back(std::move(节点));
        if (上级) 请求.追加外部关系组.push_back({*上级, 槽, 基础外部关系类型::父子, 0});
        return 槽;
    }

    基础组内节点引用 准备原始材料节点(基础节点组追加请求& 请求,
        材料物理类型 类型, const 新特征值材料& 材料,
        const std::optional<父引用>& 上级 = std::nullopt) const {
        基础值 原始;
        std::visit([&原始](const auto& 值) { 原始.材料 = 值; }, 材料);
        return 准备材料节点(请求, {{{节点类型字段_, 特征值类型节点_},
            {材料物理类型字段_, 基础值{static_cast<std::int64_t>(类型)}},
            {原始材料字段_, std::move(原始)}}}, 上级);
    }

    void 复核新发表原值(稳定编码 节点, 材料物理类型 类型, const 新特征值材料& 材料) const noexcept {
        try {
            const auto 读回 = 获取特征值已加锁(节点, 数据集_);
            if (读回 && 读回->节点 == 节点 && 读回->物理类型 == 类型 && 读回->材料 == 材料) return;
        } catch (...) {
            特征值恢复工程失败(L"复核新发表原值", L"已发表原值读取发生工程异常。");
        }
        特征值恢复工程失败(L"复核新发表原值", L"已发表原值的身份、类型或材料不符。");
    }

    std::vector<稳定编码> 子节点(稳定编码 上级节点, const 基础数据集& 数据) const {
        std::vector<稳定编码> 结果;
        for (const auto& 关系 :
            数据.查询源关系(上级节点, 基础外部关系类型::父子)) {
            if (关系.角色或顺序 == 0) 结果.push_back(关系.目标节点);
        }
        return 结果;
    }

    稳定编码 查找压缩子节点(稳定编码 上级节点, 材料物理类型 类型,
        const std::vector<std::uint64_t>& 材料) const {
        稳定编码 命中;
        for (const auto 节点 : 子节点(上级节点, 数据集_)) {
            if (!压缩材料相同(节点, 类型, 材料, 数据集_)) continue;
            if (有效(命中)) 特征值恢复工程失败(L"核验压缩层", L"同一父层存在重复压缩节点。");
            命中 = 节点;
        }
        return 命中;
    }

    稳定编码 查找原始子节点(稳定编码 上级节点,
        材料物理类型 类型, const 新特征值材料& 材料) const {
        稳定编码 命中;
        for (const auto 节点 : 子节点(上级节点, 数据集_)) {
            const auto 信息 = 获取特征值已加锁(节点, 数据集_);
            if (!信息 || 信息->物理类型 != 类型 || 信息->材料 != 材料) continue;
            if (有效(命中)) 特征值恢复工程失败(L"核验原始材料", L"同一父层存在重复原始材料。");
            命中 = 节点;
        }
        return 命中;
    }

    稳定编码 保存或取得普通材料(
        材料物理类型 类型, const 新特征值材料& 材料) {
        const 材料键 键{类型, 材料};
        auto& 候选 = 普通材料索引_[键];
        for (const auto 节点 : 候选) {
            const auto 信息 = 获取特征值已加锁(节点, 数据集_);
            if (信息 && 信息->物理类型 == 类型 && 信息->材料 == 材料) return 节点;
        }

        候选.reserve(候选.size() + 1);
        基础节点组追加请求 请求;
        const auto 原始 = 准备原始材料节点(请求, 类型, 材料);
        const auto 提交 = 数据集_.原子追加节点组(请求);
        if (提交.状态 != 基础节点组追加状态::已完成) return {};
        if (提交.新节点组.size() != 请求.新节点组.size())
            特征值恢复工程失败(L"保存原始特征值", L"发布回执节点数量不符。");
        const auto 节点 = 提交.新节点组[原始.索引];
        复核新发表原值(节点, 类型, 材料);
        候选.push_back(节点);
        return 节点;
    }

    稳定编码 保存或取得二值格(材料物理类型 类型,
        const 新特征值材料& 原始材料,
        const std::vector<std::vector<std::uint64_t>>& 层) {
        auto& 根候选 = 根索引_[材料键{类型, 新特征值材料{层.back()}}];
        稳定编码 已有父;
        for (const auto 节点 : 根候选) {
            if (!压缩材料相同(节点, 类型, 层.back(), 数据集_)) continue;
            if (有效(已有父)) 特征值恢复工程失败(L"保存二值格", L"同一压缩根存在重复身份。");
            已有父 = 节点;
        }

        基础节点组追加请求 请求;
        std::optional<基础组内节点引用> 新根;
        std::optional<父引用> 父;
        const auto 压缩类型 = 压缩类型节点(类型);
        const auto 准备压缩层 = [&](std::size_t i) {
            return 准备材料节点(请求, {{{节点类型字段_, 压缩类型},
                {压缩材料字段_, 基础值{层[i]}}}}, 父);
        };
        for (std::size_t i = 层.size(); i > 0; --i) {
            const auto 层号 = i - 1;
            if (层号 == 0 && i != 层.size()) break;
            if (有效(已有父) && i == 层.size()) {
                父 = 已有父;
                continue;
            }
            if (有效(已有父)) {
                auto 读 = 数据集_.查询节点组完整结构({已有父});
                if (读.状态 != 基础结构操作状态::已完成 || 读.节点组.size() != 1
                    || !读.节点组.front().快照) return {};
                const auto 子 = 查找压缩子节点(已有父, 类型, 层[层号]);
                if (有效(子)) {
                    已有父 = 子;
                    父 = 子;
                    continue;
                }
                请求.预期联合节点组.push_back(std::move(*读.节点组.front().快照));
            }
            const auto 槽 = 准备压缩层(层号);
            if (!父) {
                新根 = 槽;
                根候选.reserve(根候选.size() + 1);
            }
            父 = 槽;
            已有父 = {};
        }

        if (有效(已有父)) {
            auto 读 = 数据集_.查询节点组完整结构({已有父});
            if (读.状态 != 基础结构操作状态::已完成 || 读.节点组.size() != 1
                || !读.节点组.front().快照) return {};
            const auto 已有 = 查找原始子节点(已有父, 类型, 原始材料);
            if (有效(已有)) return 已有;
            请求.预期联合节点组.push_back(std::move(*读.节点组.front().快照));
        }
        const auto 原始 = 准备原始材料节点(请求, 类型, 原始材料, 父);
        const auto 提交 = 数据集_.原子追加节点组(请求);
        if (提交.状态 != 基础节点组追加状态::已完成) return {};
        if (提交.新节点组.size() != 请求.新节点组.size())
            特征值恢复工程失败(L"保存二值格", L"发布回执节点数量不符。");
        const auto 节点 = 提交.新节点组[原始.索引];
        复核新发表原值(节点, 类型, 原始材料);
        if (新根) 根候选.push_back(提交.新节点组[新根->索引]);
        return 节点;
    }

    std::vector<稳定编码> 查询普通材料(
        材料物理类型 类型, const 新特征值材料& 材料, const 基础数据集& 数据) const {
        std::vector<稳定编码> 结果;
        const auto 位置 = 普通材料索引_.find(材料键{类型, 材料});
        if (位置 == 普通材料索引_.end()) return 结果;
        for (const auto 节点 : 位置->second) {
            const auto 信息 = 获取特征值已加锁(节点, 数据);
            if (信息 && 信息->物理类型 == 类型 && 信息->材料 == 材料) {
                结果.push_back(节点);
            }
        }
        return 结果;
    }

    std::vector<稳定编码> 查询二值格(材料物理类型 类型,
        const 新特征值材料& 原始材料,
        const std::vector<std::vector<std::uint64_t>>& 层, const 基础数据集& 数据) const {
        std::vector<稳定编码> 当前层;
        const auto 根位置 = 根索引_.find(材料键{类型, 新特征值材料{层.back()}});
        if (根位置 == 根索引_.end()) return {};
        for (const auto 节点 : 根位置->second) {
            if (压缩材料相同(节点, 类型, 层.back(), 数据)) 当前层.push_back(节点);
        }

        for (std::size_t 位置 = 层.size(); 位置 > 1; --位置) {
            const auto& 细层材料 = 层[位置 - 2];
            if (位置 - 2 == 0) break;
            std::vector<稳定编码> 下一层;
            for (const auto 上级节点 : 当前层) {
                for (const auto 节点 : 子节点(上级节点, 数据)) {
                    if (压缩材料相同(节点, 类型, 细层材料, 数据)) 下一层.push_back(节点);
                }
            }
            当前层 = std::move(下一层);
            if (当前层.empty()) return {};
        }

        std::vector<稳定编码> 结果;
        for (const auto 上级节点 : 当前层) {
            for (const auto 节点 : 子节点(上级节点, 数据)) {
                const auto 信息 = 获取特征值已加锁(节点, 数据);
                if (信息 && 信息->物理类型 == 类型 && 信息->材料 == 原始材料) {
                    结果.push_back(节点);
                }
            }
        }
        std::sort(结果.begin(), 结果.end(),
            [](稳定编码 左, 稳定编码 右) { return 左.值 < 右.值; });
        结果.erase(std::unique(结果.begin(), 结果.end()), 结果.end());
        return 结果;
    }

    基础数据集& 数据集_;
    mutable std::mutex 互斥_;
    bool 已初始化_ = false;

    稳定编码 节点类型字段_;
    稳定编码 特征值类型节点_;
    稳定编码 二维压缩类型节点_;
    稳定编码 三维压缩类型节点_;
    稳定编码 材料物理类型字段_;
    稳定编码 原始材料字段_;
    稳定编码 压缩材料字段_;

    std::shared_ptr<基础持久运行包> 恢复会话_;
    mutable 基础持久运行包::视图 索引视图_;
    mutable 材料索引 普通材料索引_;
    mutable 材料索引 根索引_;
};

新_特征值类::新_特征值类(基础数据集& 数据集)
    : 实现_(std::make_unique<实现>(数据集)) {}

新_特征值类::~新_特征值类() = default;

bool 新_特征值类::初始化() noexcept {
    return 实现_->初始化();
}

新特征值运行恢复状态 新_特征值类::恢复当前权威() noexcept {
    return 实现_->恢复当前权威();
}

稳定编码 新_特征值类::保存或取得特征值(
    材料物理类型 物理类型, const 新特征值材料& 材料) noexcept {
    return 实现_->保存或取得特征值(物理类型, 材料);
}

std::optional<新特征值信息> 新_特征值类::获取特征值(
    稳定编码 节点) const noexcept {
    return 实现_->获取特征值(节点);
}

新特征值保护读取结果 新_特征值类::读取特征值保护(稳定编码 节点) const {
    return 实现_->读取特征值保护(节点);
}

新特征值快照解码结果 新_特征值类::解码特征值快照(const 基础节点结构& 结构) const {
    return 实现_->解码特征值快照(结构);
}

std::optional<新特征值快照解码结构> 新_特征值类::读取快照解码结构() const {
    return 实现_->读取快照解码结构();
}

新特征值快照解码结果 新_特征值类::解码特征值快照(
    const 基础节点结构& 结构, const 新特征值快照解码结构& keys) {
    return 实现::解码快照(结构, keys);
}

std::vector<稳定编码> 新_特征值类::查询特征值(
    材料物理类型 物理类型, const 新特征值材料& 材料) const noexcept {
    return 实现_->查询特征值(物理类型, 材料);
}

新服务权限值结果 新_特征值类::保存或取得服务权限值(std::uint64_t 数值) {
    using S = 新服务权限值状态;
    try {
        const auto 节点 = 保存或取得特征值(
            材料物理类型::U64数组, std::vector<std::uint64_t>{数值});
        if (!有效(节点)) return {S::保存失败};
        auto 读回 = 读取服务权限值(节点);
        if (读回.状态 == S::已读取 && 读回.数值 != 数值) return {S::结构不一致};
        return 读回;
    } catch (const std::bad_alloc&) {
        return {S::资源失败};
    }
}

新服务权限值结果 新_特征值类::读取服务权限值(稳定编码 节点) const {
    using S = 新服务权限值状态;
    using R = 新特征值保护读取状态;
    auto 保护 = 读取特征值保护(节点);
    switch (保护.状态) {
    case R::资源失败: return {S::资源失败};
    case R::结构不一致: return {S::结构不一致};
    case R::非本类节点: return {S::材料形状不相容};
    case R::输入拒绝:
    case R::未初始化:
    case R::节点不存在: return {S::节点不可读};
    case R::已读取: break;
    default: return {S::结构不一致};
    }
    if (!保护.信息 || 保护.信息->节点 != 节点) return {S::结构不一致};
    const auto* 项组 = std::get_if<std::vector<std::uint64_t>>(&保护.信息->材料);
    if (保护.信息->物理类型 != 材料物理类型::U64数组 || !项组 || 项组->size() != 1)
        return {S::材料形状不相容};
    return {S::已读取, 节点, 项组->front(), std::move(保护.窄预期)};
}

bool 新_特征值类::是特征值节点(稳定编码 节点) const noexcept {
    return 实现_->是特征值节点(节点);
}

新特征值恢复结构读回 读取恢复特征值结构(const 基础持久视图& 视图) noexcept {
    using S = 新特征值保护读取状态;
    try {
        const auto 旧标记 = 视图.查询自键标记(旧压缩类型标记);
        const bool 旧格式 = !旧标记.empty();
        if (旧格式 && (旧标记.size() != 1 || !视图.查询自键标记(恢复标记组[2]).empty()
            || !视图.查询自键标记(恢复标记组[3]).empty())) return {S::结构不一致};
        特征值结构键组 键组{};
        for (std::size_t i = 0; i < 键组.size(); ++i) {
            if (旧格式 && i == 3) continue;
            const auto 标记 = 旧格式 && i == 2 ? 旧压缩类型标记 : 恢复标记组[i];
            const auto 命中 = 视图.查询自键标记(标记);
            if (命中.size() != 1) return {S::结构不一致};
            const auto 键 = 命中.front().所属节点;
            if (!有效(键) || std::find(键组.begin(), 键组.end(), 键) != 键组.end())
                return {S::结构不一致};
            const auto 字段 = 视图.结构().查询字段(键, 键);
            if (字段.size() != 1 || 字段.front() != 命中.front()
                || !是特征值恢复标记(字段.front(), 键, 标记))
                return {S::结构不一致};
            键组[i] = 键;
        }
        return 旧格式 ? 新特征值恢复结构读回{S::格式不支持}
            : 新特征值恢复结构读回{S::已读取, 键组};
    } catch (const std::bad_alloc&) {
        return {S::资源失败};
    } catch (const std::length_error&) {
        return {S::资源失败};
    } catch (...) {
        特征值恢复工程失败(L"恢复特征值结构", L"只读结构重取发生未预期工程异常。");
    }
}

新特征值运行恢复状态 核验恢复特征值索引(const 基础持久视图& 视图) noexcept {
    try {
        return 构造恢复材料候选(视图).状态;
    } catch (值恢复状态 状态) {
        return 状态;
    } catch (const std::bad_alloc&) {
        return 值恢复状态::资源失败;
    } catch (const std::length_error&) {
        return 值恢复状态::资源失败;
    } catch (...) {
        特征值恢复工程失败(L"核验恢复特征值索引", L"恢复材料索引核验发生工程异常。");
    }
}

新特征值保护读取结果 读取恢复特征值材料(const 基础持久视图& 视图,
    const 新特征值恢复结构读回& 结构, 材料物理类型 类型, const 新特征值材料& 材料) noexcept {
    using S = 新特征值保护读取状态;
    using B = 基础结构操作状态;
    using D = 新特征值快照解码状态;
    if (!是已知物理类型(类型) || !材料形态匹配(类型, 材料)) return {S::输入拒绝};
    if (结构.状态 != S::已读取) return {结构.状态};
    const auto 实际结构 = 读取恢复特征值结构(视图);
    if (实际结构.状态 != S::已读取) return {实际结构.状态};
    if (实际结构.键 != 结构.键) return {S::结构不一致};
    try {
        const auto 解码结构 = 实际结构.解码结构();
        const auto& 数据 = 视图.结构();
        新特征值保护读取结果 结果{S::节点不存在};
        for (const auto& 引用 : 数据.查询目标字段(解码结构.特征值类型节点, 解码结构.节点类型字段)) {
            const auto 节点 = 引用.所属节点;
            const auto 读回 = 数据.查询完整节点结构(节点);
            if (读回.状态 == B::资源失败) return {S::资源失败};
            if (读回.状态 != B::已完成 || !读回.结构 || 读回.结构->节点.编码 != 节点)
                return {S::结构不一致};
            auto 解码 = 新_特征值类::解码特征值快照(*读回.结构, 解码结构);
            if (解码.状态 != D::已解码 || !解码.信息 || 解码.信息->节点 != 节点)
                return {S::结构不一致};
            if (解码.信息->物理类型 != 类型 || 解码.信息->材料 != 材料) continue;
            if (结果.状态 == S::已读取) return {S::结构不一致};
            解码.窄预期.节点组.push_back({解码结构.特征值类型节点});
            结果 = {S::已读取, std::move(解码.信息), std::move(解码.窄预期)};
        }
        return 结果;
    } catch (const std::bad_alloc&) {
        return {S::资源失败};
    } catch (const std::length_error&) {
        return {S::资源失败};
    } catch (...) {
        特征值恢复工程失败(L"恢复特征值材料", L"只读材料重取发生未预期工程异常。");
    }
}

} // namespace 海中鱼巣
