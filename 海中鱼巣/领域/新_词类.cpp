#include "新_词类.h"
#include "../核心/容错检查.h"

#include <algorithm>
#include <map>
#include <mutex>
#include <new>
#include <stdexcept>
#include <tuple>

namespace 海中鱼巣 {

稳定编码 词组织根节点{};
稳定编码 严格UTF8字节精确规范化合同节点{};

namespace {

std::mutex 新词互斥;

struct 词条键 final {
    稳定编码 语言节点;
    稳定编码 规范化合同节点;
    std::string 规范化词面;

    bool operator<(const 词条键& 另一项) const noexcept {
        return std::tie(语言节点, 规范化合同节点, 规范化词面)
            < std::tie(另一项.语言节点, 另一项.规范化合同节点,
                另一项.规范化词面);
    }
};

std::map<词条键, 稳定编码> 词条索引;

稳定编码 节点类型字段{};
稳定编码 词组织根类型{};
稳定编码 词条节点类型{};
稳定编码 规范化合同节点类型{};
稳定编码 所属语言字段{};
稳定编码 规范化合同字段{};
稳定编码 规范化词面字段{};
稳定编码 创建来源字段{};

bool 节点仍存在(稳定编码 节点) noexcept {
    return 有效(节点) && 全局基础数据集.查询节点(节点).has_value();
}

std::optional<稳定编码> 读取唯一节点字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<稳定编码>(&字段组.front().内容);
    return 值 ? std::optional<稳定编码>{*值} : std::nullopt;
}

std::optional<std::string> 读取唯一字符串字段(
    稳定编码 节点, 稳定编码 字段) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 字段);
    if (字段组.size() != 1) return std::nullopt;
    const auto* 值 = std::get_if<基础值>(&字段组.front().内容);
    if (!值) return std::nullopt;
    const auto* 文本 = std::get_if<std::string>(&值->材料);
    return 文本 ? std::optional<std::string>{*文本} : std::nullopt;
}

bool 有唯一父节点(稳定编码 节点, 稳定编码 父节点) {
    const auto 关系组 = 全局基础数据集.查询目标关系(
        节点, 基础外部关系类型::父子);
    return 关系组.size() == 1
        && 关系组.front().源节点 == 父节点
        && 关系组.front().角色或顺序 == 0;
}

bool 类型为(稳定编码 节点, 稳定编码 类型) {
    const auto 实际类型 = 读取唯一节点字段(节点, 节点类型字段);
    return 实际类型 && *实际类型 == 类型;
}

bool 结构句柄完整() noexcept {
    const std::vector<稳定编码> 节点组{
        自然语言组织根节点, 语言组织根节点, 词组织根节点,
        严格UTF8字节精确规范化合同节点, 节点类型字段,
        词组织根类型, 词条节点类型, 规范化合同节点类型,
        所属语言字段, 规范化合同字段, 规范化词面字段, 创建来源字段};
    return std::all_of(节点组.begin(), 节点组.end(), [](稳定编码 节点) {
        return 有效(节点);
    });
}

bool 结构仍存在() {
    if (!结构句柄完整()) return false;
    const std::vector<稳定编码> 节点组{
        自然语言组织根节点, 语言组织根节点, 词组织根节点,
        严格UTF8字节精确规范化合同节点, 节点类型字段,
        词组织根类型, 词条节点类型, 规范化合同节点类型,
        所属语言字段, 规范化合同字段, 规范化词面字段, 创建来源字段};
    return std::all_of(节点组.begin(), 节点组.end(), 节点仍存在)
        && 类型为(词组织根节点, 词组织根类型)
        && 类型为(严格UTF8字节精确规范化合同节点,
            规范化合同节点类型)
        && 有唯一父节点(词组织根节点, 自然语言组织根节点)
        && 有唯一父节点(严格UTF8字节精确规范化合同节点,
            自然语言组织根节点);
}

std::optional<新词条信息> 读取词条信息(
    稳定编码 节点, const 新_语言类& 语言服务) {
    if (!节点仍存在(节点) || !类型为(节点, 词条节点类型)
        || !有唯一父节点(节点, 词组织根节点)) {
        return std::nullopt;
    }
    const auto 语言 = 读取唯一节点字段(节点, 所属语言字段);
    const auto 合同 = 读取唯一节点字段(节点, 规范化合同字段);
    const auto 词面 = 读取唯一字符串字段(节点, 规范化词面字段);
    if (!语言 || !合同 || !词面
        || !语言服务.是语言节点(*语言)
        || *合同 != 严格UTF8字节精确规范化合同节点
        || !新_语言类::严格UTF8字节串有效(*词面)) {
        return std::nullopt;
    }

    const auto 来源组 = 全局基础数据集.查询字段(节点, 创建来源字段);
    std::optional<稳定编码> 来源;
    if (!来源组.empty()) {
        if (来源组.size() != 1) return std::nullopt;
        const auto* 值 = std::get_if<稳定编码>(&来源组.front().内容);
        if (!值 || !节点仍存在(*值)) return std::nullopt;
        来源 = *值;
    }
    return 新词条信息{节点, *语言, *合同, *词面, 来源};
}

bool 确保唯一类型字段(稳定编码 节点, 稳定编码 类型) {
    const auto 字段组 = 全局基础数据集.查询字段(节点, 节点类型字段);
    if (字段组.empty()) {
        return 有效(全局基础数据集.添加字段节点(
            节点, 节点类型字段, 类型));
    }
    if (字段组.size() != 1) return false;
    const auto* 值 = std::get_if<稳定编码>(&字段组.front().内容);
    return 值 && *值 == 类型;
}

bool 删除父子关系并删除节点(稳定编码 父节点, 稳定编码 节点) noexcept {
    bool 成功 = true;
    for (const auto& 关系 : 全局基础数据集.查询目标关系(
        节点, 基础外部关系类型::父子)) {
        if (关系.源节点 == 父节点) {
            成功 = 全局基础数据集.删除关系(关系.编码) && 成功;
        }
    }
    return 全局基础数据集.删除节点(节点) && 成功;
}

bool 重建词条索引(const 新_语言类& 语言服务) {
    std::map<词条键, 稳定编码> 新索引;
    for (const auto& 关系 : 全局基础数据集.查询源关系(
        词组织根节点, 基础外部关系类型::父子)) {
        if (关系.角色或顺序 != 0) return false;
        const auto 信息 = 读取词条信息(关系.目标节点, 语言服务);
        if (!信息) return false;
        词条键 键{信息->语言节点, 信息->规范化合同节点,
            信息->规范化词面};
        const auto [位置, 已插入] = 新索引.emplace(std::move(键), 信息->节点);
        if (!已插入 && 位置->second != 信息->节点) return false;
    }
    词条索引.swap(新索引);
    return true;
}

void 回滚词条节点(稳定编码 节点,
    const std::vector<稳定编码>& 字段组) noexcept {
    for (auto 位置 = 字段组.rbegin(); 位置 != 字段组.rend(); ++位置) {
        if (有效(*位置)) (void)全局基础数据集.删除字段(*位置);
    }
    (void)删除父子关系并删除节点(词组织根节点, 节点);
}

bool 词保护相同(const 基础联合节点快照& 左,
    const 基础联合节点快照& 右) {
    return 左.结构.节点 == 右.结构.节点
        && 左.结构.字段组 == 右.结构.字段组
        && 左.入向字段组 == 右.入向字段组
        && 左.入向外部关系组 == 右.入向外部关系组
        && 左.出向外部关系组 == 右.出向外部关系组;
}

// 栈上读集；索引仅召回，词条、组织登记及否定集合均由正式快照保护。
struct 词保护现场 final {
    std::map<稳定编码, 基础联合节点快照> 读集;

    std::optional<基础联合节点快照> 读取(稳定编码 节点) {
        const auto 结果 = 全局基础数据集.查询节点组完整结构({节点});
        if (结果.状态 == 基础结构操作状态::资源失败)
            throw 新词条保护状态::资源失败;
        if (结果.状态 != 基础结构操作状态::已完成
            || 结果.节点组.size() != 1 || 结果.节点组.front().查询节点 != 节点)
            throw std::logic_error("word protection read receipt invalid");
        if (!结果.节点组.front().快照) return std::nullopt;
        const auto& 快照 = *结果.节点组.front().快照;
        if (快照.结构.节点.编码 != 节点)
            throw std::logic_error("word protection identity mismatch");
        const auto [位置, 新增] = 读集.emplace(节点, 快照);
        if (!新增 && !词保护相同(位置->second, 快照))
            throw 新词条保护状态::前提已变化;
        return 快照;
    }

    基础联合节点快照 必需(稳定编码 节点) {
        if (!有效(节点)) throw std::logic_error("word protection dependency invalid");
        const auto 结果 = 读取(节点);
        if (!结果) throw std::logic_error("word protection dependency missing");
        return *结果;
    }

    std::vector<基础联合节点快照> 完成() const {
        std::vector<稳定编码> 身份;
        for (const auto& [节点, 快照] : 读集) 身份.push_back(节点);
        const auto 结果 = 全局基础数据集.查询节点组完整结构(身份);
        if (结果.状态 == 基础结构操作状态::资源失败)
            throw 新词条保护状态::资源失败;
        if (结果.状态 != 基础结构操作状态::已完成
            || 结果.节点组.size() != 身份.size())
            throw std::logic_error("word protection final read incomplete");
        std::vector<基础联合节点快照> 完整;
        for (std::size_t i = 0; i < 身份.size(); ++i) {
            const auto& 项 = 结果.节点组[i];
            if (项.查询节点 != 身份[i])
                throw std::logic_error("word protection final identity mismatch");
            if (!项.快照 || !词保护相同(读集.at(身份[i]), *项.快照))
                throw 新词条保护状态::前提已变化;
            完整.push_back(*项.快照);
        }
        return 完整;
    }
};

稳定编码 词保护唯一引用(const 基础联合节点快照& 快照, 稳定编码 键) {
    std::optional<稳定编码> 结果;
    for (const auto& 字段 : 快照.结构.字段组) if (字段.字段节点 == 键) {
        const auto* 值 = std::get_if<稳定编码>(&字段.内容);
        if (结果 || !值 || !有效(*值))
            throw std::logic_error("word protection reference malformed");
        结果 = *值;
    }
    if (!结果) throw std::logic_error("word protection reference missing");
    return *结果;
}

void 核词保护父边(const 基础联合节点快照& 子,
    const 基础联合节点快照& 父) {
    const 基础外部关系* 命中 = nullptr;
    for (const auto& 边 : 子.入向外部关系组)
        if (边.类型 == 基础外部关系类型::父子) {
            if (命中 || 边.源节点 != 父.结构.节点.编码
                || 边.目标节点 != 子.结构.节点.编码 || 边.角色或顺序 != 0)
                throw std::logic_error("word protection parent malformed");
            命中 = &边;
        }
    if (!命中 || std::count(父.出向外部关系组.begin(),
        父.出向外部关系组.end(), *命中) != 1)
        throw std::logic_error("word protection registration missing");
}

void 读取词保护结构(词保护现场& 现场) {
    if (!结构句柄完整()) throw 新词条保护状态::输入拒绝;
    const auto 自然根 = 现场.必需(自然语言组织根节点);
    const auto 词根 = 现场.必需(词组织根节点);
    const auto 合同 = 现场.必需(严格UTF8字节精确规范化合同节点);
    现场.必需(语言组织根节点);
    for (const auto 节点 : {节点类型字段, 词组织根类型, 词条节点类型,
        规范化合同节点类型, 所属语言字段, 规范化合同字段,
        规范化词面字段, 创建来源字段}) 现场.必需(节点);
    if (词根.结构.字段组.size() != 1 || 合同.结构.字段组.size() != 1
        || 词保护唯一引用(词根, 节点类型字段) != 词组织根类型
        || 词保护唯一引用(合同, 节点类型字段) != 规范化合同节点类型)
        throw std::logic_error("word protection schema malformed");
    核词保护父边(词根, 自然根);
    核词保护父边(合同, 自然根);
}

bool 读取词保护语言(词保护现场& 现场, 稳定编码 语言,
    const 新_语言类& 服务) {
    const auto 首读 = 现场.读取(语言);
    const auto 结果 = 服务.获取语言(语言);
    if (结果.状态 == 新语言查询状态::资源失败)
        throw 新词条保护状态::资源失败;
    if (结果.状态 == 新语言查询状态::结构不一致)
        throw std::logic_error("word language structure corrupt");
    if (结果.状态 != 新语言查询状态::已找到) return false;
    if (!结果.信息 || !首读)
        throw 新词条保护状态::前提已变化;
    // 语言owner解释其字段；此处只保留本次实际读到的端点完整性预期。
    for (const auto& 字段 : 首读->结构.字段组) {
        现场.必需(字段.字段节点);
        if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容)) 现场.必需(*引用);
    }
    return true;
}

新词条信息 解码词保护信息(const 基础联合节点快照& 快照,
    词保护现场& 现场, const 新_语言类& 服务) {
    if (词保护唯一引用(快照, 节点类型字段) != 词条节点类型)
        throw std::logic_error("registered word type corrupt");
    核词保护父边(快照, 现场.必需(词组织根节点));
    const auto 语言 = 词保护唯一引用(快照, 所属语言字段);
    const auto 合同 = 词保护唯一引用(快照, 规范化合同字段);
    if (合同 != 严格UTF8字节精确规范化合同节点)
        throw std::logic_error("word normalization contract corrupt");
    std::optional<std::string> 词面;
    std::optional<稳定编码> 来源;
    for (const auto& 字段 : 快照.结构.字段组) {
        if (字段.字段节点 == 规范化词面字段) {
            const auto* 值 = std::get_if<基础值>(&字段.内容);
            const auto* 文本 = 值 ? std::get_if<std::string>(&值->材料) : nullptr;
            if (词面 || !文本 || !新_语言类::严格UTF8字节串有效(*文本))
                throw std::logic_error("word normalized text corrupt");
            词面 = *文本;
        } else if (字段.字段节点 == 创建来源字段) {
            if (来源) throw std::logic_error("word source duplicated");
            来源 = 词保护唯一引用(快照, 创建来源字段);
            现场.必需(*来源);
        } else if (字段.字段节点 != 节点类型字段
            && 字段.字段节点 != 所属语言字段 && 字段.字段节点 != 规范化合同字段)
            throw std::logic_error("word field not allowed");
    }
    if (!词面 || 快照.结构.字段组.size() != (来源 ? 5u : 4u))
        throw std::logic_error("word fields incomplete");
    if (!读取词保护语言(现场, 语言, 服务))
        throw std::logic_error("word language dependency missing");
    const auto 索引 = 词条索引.find({语言, 合同, *词面});
    if (索引 == 词条索引.end() || 索引->second != 快照.结构.节点.编码)
        throw std::logic_error("word index and registration differ");
    return {快照.结构.节点.编码, 语言, 合同, *词面, 来源};
}

} // namespace

新_词类::新_词类(新_语言类& 语言服务) noexcept
    : 语言服务_(语言服务) {}

const 新_语言类& 新_词类::取得语言服务() const noexcept {
    return 语言服务_;
}

bool 新_词类::初始化() noexcept {
    try {
        if (!语言服务_.初始化()) return false;
        std::lock_guard 锁(新词互斥);
        auto 建立独立节点 = [](稳定编码& 节点) {
            if (有效(节点)) return 节点仍存在(节点);
            节点 = 全局基础数据集.新建节点();
            return 有效(节点);
        };
        if (!建立独立节点(节点类型字段)
            || !建立独立节点(词组织根类型)
            || !建立独立节点(词条节点类型)
            || !建立独立节点(规范化合同节点类型)
            || !建立独立节点(所属语言字段)
            || !建立独立节点(规范化合同字段)
            || !建立独立节点(规范化词面字段)
            || !建立独立节点(创建来源字段)) {
            return false;
        }
        if (!有效(词组织根节点)) {
            词组织根节点 = 全局基础数据集.新建节点(自然语言组织根节点);
            if (!有效(词组织根节点)) return false;
        } else if (!节点仍存在(词组织根节点)
            || !有唯一父节点(词组织根节点, 自然语言组织根节点)) {
            return false;
        }
        if (!确保唯一类型字段(词组织根节点, 词组织根类型)) return false;

        if (!有效(严格UTF8字节精确规范化合同节点)) {
            严格UTF8字节精确规范化合同节点 =
                全局基础数据集.新建节点(自然语言组织根节点);
            if (!有效(严格UTF8字节精确规范化合同节点)) return false;
        } else if (!节点仍存在(严格UTF8字节精确规范化合同节点)
            || !有唯一父节点(严格UTF8字节精确规范化合同节点,
                自然语言组织根节点)) {
            return false;
        }
        if (!确保唯一类型字段(严格UTF8字节精确规范化合同节点,
            规范化合同节点类型)) {
            return false;
        }
        return 结构仍存在() && 重建词条索引(语言服务_);
    } catch (...) {
        return false;
    }
}

新词条建立结果 新_词类::建立或取得词条(
    const 新词条建立请求& 请求) noexcept {
    新词条建立结果 结果;
    try {
        std::lock_guard 锁(新词互斥);
        if (!有效(请求.语言节点) || !有效(请求.规范化合同节点)
            || !新_语言类::严格UTF8字节串有效(请求.规范化词面)
            || 请求.规范化合同节点
                != 严格UTF8字节精确规范化合同节点) {
            return 结果;
        }
        if (!结构句柄完整()) {
            结果.状态 = 新词条建立状态::依赖不存在;
            return 结果;
        }
        if (!结构仍存在()) {
            结果.状态 = 新词条建立状态::结构不一致;
            return 结果;
        }
        if (!语言服务_.是语言节点(请求.语言节点)
            || (请求.创建来源节点
                && !节点仍存在(*请求.创建来源节点))) {
            结果.状态 = 新词条建立状态::依赖不存在;
            return 结果;
        }
        词条键 键{请求.语言节点, 请求.规范化合同节点,
            请求.规范化词面};
        const auto 已有 = 词条索引.find(键);
        if (已有 != 词条索引.end()) {
            const auto 信息 = 读取词条信息(已有->second, 语言服务_);
            if (!信息 || 信息->语言节点 != 请求.语言节点
                || 信息->规范化合同节点 != 请求.规范化合同节点
                || 信息->规范化词面 != 请求.规范化词面) {
                结果.状态 = 新词条建立状态::结构不一致;
                return 结果;
            }
            结果.状态 = 新词条建立状态::已复用;
            结果.词条节点 = 已有->second;
            return 结果;
        }

        const auto 节点 = 全局基础数据集.新建节点(词组织根节点);
        if (!有效(节点)) {
            结果.状态 = 新词条建立状态::资源失败;
            return 结果;
        }
        std::vector<稳定编码> 字段组;
        字段组.push_back(全局基础数据集.添加字段节点(
            节点, 节点类型字段, 词条节点类型));
        if (有效(字段组.back())) {
            字段组.push_back(全局基础数据集.添加字段节点(
                节点, 所属语言字段, 请求.语言节点));
        }
        if (有效(字段组.back())) {
            字段组.push_back(全局基础数据集.添加字段节点(
                节点, 规范化合同字段, 请求.规范化合同节点));
        }
        if (有效(字段组.back())) {
            字段组.push_back(全局基础数据集.添加字段值(
                节点, 规范化词面字段, 基础值{请求.规范化词面}));
        }
        if (请求.创建来源节点 && 有效(字段组.back())) {
            字段组.push_back(全局基础数据集.添加字段节点(
                节点, 创建来源字段, *请求.创建来源节点));
        }
        if (std::any_of(字段组.begin(), 字段组.end(), [](稳定编码 字段) {
            return !有效(字段);
        })) {
            回滚词条节点(节点, 字段组);
            结果.状态 = 新词条建立状态::资源失败;
            return 结果;
        }
        const auto 信息 = 读取词条信息(节点, 语言服务_);
        if (!信息 || 信息->语言节点 != 请求.语言节点
            || 信息->规范化合同节点 != 请求.规范化合同节点
            || 信息->规范化词面 != 请求.规范化词面
            || 信息->创建来源节点 != 请求.创建来源节点) {
            回滚词条节点(节点, 字段组);
            结果.状态 = 新词条建立状态::结构不一致;
            return 结果;
        }
        const auto [_, 已插入] = 词条索引.emplace(std::move(键), 节点);
        if (!已插入) {
            回滚词条节点(节点, 字段组);
            结果.状态 = 新词条建立状态::结构不一致;
            return 结果;
        }
        结果.状态 = 新词条建立状态::已建立;
        结果.词条节点 = 节点;
        return 结果;
    } catch (...) {
        结果.状态 = 新词条建立状态::资源失败;
        return 结果;
    }
}

新词条查询结果 新_词类::获取词条(稳定编码 词条节点) const noexcept {
    新词条查询结果 结果;
    try {
        std::lock_guard 锁(新词互斥);
        if (!有效(词条节点)) return 结果;
        if (!结构仍存在()) {
            结果.状态 = 新词条查询状态::结构不一致;
            return 结果;
        }
        if (!节点仍存在(词条节点)) {
            结果.状态 = 新词条查询状态::未找到;
            return 结果;
        }
        if (!类型为(词条节点, 词条节点类型)) {
            结果.状态 = 新词条查询状态::输入不合法;
            return 结果;
        }
        const auto 信息 = 读取词条信息(词条节点, 语言服务_);
        if (!信息) {
            结果.状态 = 新词条查询状态::结构不一致;
            return 结果;
        }
        结果.状态 = 新词条查询状态::已找到;
        结果.信息 = 信息;
        return 结果;
    } catch (...) {
        结果.状态 = 新词条查询状态::资源失败;
        return 结果;
    }
}

新词条查询结果 新_词类::按规范化词面查询(
    稳定编码 语言节点,
    稳定编码 规范化合同节点,
    const std::string& 规范化词面) const noexcept {
    新词条查询结果 结果;
    try {
        std::lock_guard 锁(新词互斥);
        if (!有效(语言节点) || !有效(规范化合同节点)
            || !新_语言类::严格UTF8字节串有效(规范化词面)
            || 规范化合同节点 != 严格UTF8字节精确规范化合同节点) {
            return 结果;
        }
        if (!结构仍存在()) {
            结果.状态 = 新词条查询状态::结构不一致;
            return 结果;
        }
        const auto 语言结果 = 语言服务_.获取语言(语言节点);
        if (语言结果.状态 == 新语言查询状态::资源失败) {
            结果.状态 = 新词条查询状态::资源失败;
            return 结果;
        }
        if (语言结果.状态 == 新语言查询状态::结构不一致) {
            结果.状态 = 新词条查询状态::结构不一致;
            return 结果;
        }
        if (语言结果.状态 != 新语言查询状态::已找到) return 结果;
        const 词条键 键{语言节点, 规范化合同节点, 规范化词面};
        const auto 位置 = 词条索引.find(键);
        if (位置 == 词条索引.end()) {
            结果.状态 = 新词条查询状态::未找到;
            return 结果;
        }
        const auto 信息 = 读取词条信息(位置->second, 语言服务_);
        if (!信息 || 信息->语言节点 != 语言节点
            || 信息->规范化合同节点 != 规范化合同节点
            || 信息->规范化词面 != 规范化词面) {
            结果.状态 = 新词条查询状态::结构不一致;
            return 结果;
        }
        结果.状态 = 新词条查询状态::已找到;
        结果.信息 = 信息;
        return 结果;
    } catch (...) {
        结果.状态 = 新词条查询状态::资源失败;
        return 结果;
    }
}

新词条列表查询结果 新_词类::查询语言词条(
    稳定编码 语言节点) const noexcept {
    新词条列表查询结果 结果;
    try {
        std::lock_guard 锁(新词互斥);
        if (!有效(语言节点)) return 结果;
        if (!结构仍存在()) {
            结果.状态 = 新词条查询状态::结构不一致;
            return 结果;
        }
        const auto 语言结果 = 语言服务_.获取语言(语言节点);
        if (语言结果.状态 == 新语言查询状态::资源失败) {
            结果.状态 = 新词条查询状态::资源失败;
            return 结果;
        }
        if (语言结果.状态 == 新语言查询状态::结构不一致) {
            结果.状态 = 新词条查询状态::结构不一致;
            return 结果;
        }
        if (语言结果.状态 != 新语言查询状态::已找到) return 结果;
        for (const auto& [键, 节点] : 词条索引) {
            if (键.语言节点 == 语言节点) 结果.词条节点组.push_back(节点);
        }
        std::sort(结果.词条节点组.begin(), 结果.词条节点组.end());
        结果.状态 = 结果.词条节点组.empty()
            ? 新词条查询状态::未找到 : 新词条查询状态::已找到;
        return 结果;
    } catch (...) {
        结果.状态 = 新词条查询状态::资源失败;
        return 结果;
    }
}

bool 新_词类::是词条节点(稳定编码 节点) const noexcept {
    const auto 结果 = 获取词条(节点);
    return 结果.状态 == 新词条查询状态::已找到;
}

新词条保护结果 新_词类::读取词条保护(稳定编码 词条) const noexcept {
    try {
        if (!有效(词条)) return {};
        std::lock_guard 锁(新词互斥);
        词保护现场 现场;
        读取词保护结构(现场);
        const auto 快照 = 现场.读取(词条);
        if (!快照) return {新词条保护状态::未找到, {}, 现场.完成()};
        const auto 根 = 现场.必需(词组织根节点);
        const bool 已登记 = std::any_of(根.出向外部关系组.begin(),
            根.出向外部关系组.end(), [&](const auto& 边) {
                return 边.类型 == 基础外部关系类型::父子 && 边.目标节点 == 词条;
            });
        if (!已登记) return {};
        const auto 信息 = 解码词保护信息(*快照, 现场, 语言服务_);
        return {新词条保护状态::已读取, 信息, 现场.完成()};
    } catch (新词条保护状态 状态) { return {状态, {}, {}}; }
    catch (const std::bad_alloc&) { return {新词条保护状态::资源失败, {}, {}}; }
    catch (const std::length_error&) { return {新词条保护状态::资源失败, {}, {}}; }
    catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取词条保护",
            L"词条完整保护读取发生工程故障。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取词条保护",
            L"词条完整保护读取发生未知工程故障。");
    }
}

新词条保护结果 新_词类::按规范化词面查询保护(稳定编码 语言,
    稳定编码 规范化合同, const std::string& 规范化词面) const noexcept {
    try {
        if (!有效(语言) || !有效(规范化合同)
            || 规范化合同 != 严格UTF8字节精确规范化合同节点
            || !新_语言类::严格UTF8字节串有效(规范化词面)) return {};
        std::lock_guard 锁(新词互斥);
        词保护现场 现场;
        读取词保护结构(现场);
        if (!读取词保护语言(现场, 语言, 语言服务_)) return {};
        const auto 位置 = 词条索引.find({语言, 规范化合同, 规范化词面});
        if (位置 == 词条索引.end())
            return {新词条保护状态::未找到, {}, 现场.完成()};
        const auto 信息 = 解码词保护信息(现场.必需(位置->second), 现场, 语言服务_);
        if (信息.语言节点 != 语言 || 信息.规范化合同节点 != 规范化合同
            || 信息.规范化词面 != 规范化词面)
            throw std::logic_error("word exact query and result differ");
        return {新词条保护状态::已读取, 信息, 现场.完成()};
    } catch (新词条保护状态 状态) { return {状态, {}, {}}; }
    catch (const std::bad_alloc&) { return {新词条保护状态::资源失败, {}, {}}; }
    catch (const std::length_error&) { return {新词条保护状态::资源失败, {}, {}}; }
    catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"按规范化词面查询保护",
            L"词面精确查询保护发生工程故障。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"按规范化词面查询保护",
            L"词面精确查询保护发生未知工程故障。");
    }
}

} // namespace 海中鱼巣
