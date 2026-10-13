#include "新_语言类.h"
#include "新_词类.h"

#include <algorithm>
#include <map>
#include <mutex>
#include <set>
#include <vector>

namespace 海中鱼巣 {

稳定编码 自然语言组织根节点{};
稳定编码 语言组织根节点{};

namespace {

// 仅接收已经通过严格UTF8校验的非空剩余字节；推进一个完整码点。
std::uint32_t 读取已校验码点(const std::string& 文本,
    std::size_t& 字节位置) noexcept {
    const auto 首字节 = static_cast<unsigned char>(文本[字节位置++]);
    if (首字节 <= 0x7F) return 首字节;
    const std::size_t 后续数量 = 首字节 <= 0xDF ? 1 : (首字节 <= 0xEF ? 2 : 3);
    std::uint32_t 码点 = 首字节 & (后续数量 == 1 ? 0x1F : (后续数量 == 2 ? 0x0F : 0x07));
    for (std::size_t i = 0; i < 后续数量; ++i) {
        码点 = (码点 << 6) | (static_cast<unsigned char>(文本[字节位置++]) & 0x3F);
    }
    return 码点;
}

struct 本次词面匹配节点 final {
    std::map<std::uint32_t, std::size_t> 后继;
    std::optional<稳定编码> 词条节点;
};

std::mutex 新语言互斥;
std::map<std::string, 稳定编码> 语言索引;

稳定编码 节点类型字段{};
稳定编码 自然语言组织根类型{};
稳定编码 语言组织根类型{};
稳定编码 语言节点类型{};
稳定编码 语言登记键字段{};
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
        自然语言组织根节点, 语言组织根节点, 节点类型字段,
        自然语言组织根类型, 语言组织根类型, 语言节点类型,
        语言登记键字段, 创建来源字段};
    return std::all_of(节点组.begin(), 节点组.end(), [](稳定编码 节点) {
        return 有效(节点);
    });
}

bool 结构仍存在() {
    if (!结构句柄完整()) return false;
    const std::vector<稳定编码> 节点组{
        自然语言组织根节点, 语言组织根节点, 节点类型字段,
        自然语言组织根类型, 语言组织根类型, 语言节点类型,
        语言登记键字段, 创建来源字段};
    return std::all_of(节点组.begin(), 节点组.end(), 节点仍存在)
        && 类型为(自然语言组织根节点, 自然语言组织根类型)
        && 类型为(语言组织根节点, 语言组织根类型)
        && 有唯一父节点(语言组织根节点, 自然语言组织根节点);
}

std::optional<新语言信息> 读取语言信息(稳定编码 节点) {
    if (!节点仍存在(节点) || !类型为(节点, 语言节点类型)
        || !有唯一父节点(节点, 语言组织根节点)) {
        return std::nullopt;
    }
    const auto 登记键 = 读取唯一字符串字段(节点, 语言登记键字段);
    if (!登记键 || !新_语言类::严格UTF8字节串有效(*登记键)) {
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
    return 新语言信息{节点, *登记键, 来源};
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

bool 重建语言索引() {
    std::map<std::string, 稳定编码> 新索引;
    for (const auto& 关系 : 全局基础数据集.查询源关系(
        语言组织根节点, 基础外部关系类型::父子)) {
        if (关系.角色或顺序 != 0) return false;
        const auto 信息 = 读取语言信息(关系.目标节点);
        if (!信息) return false;
        const auto [位置, 已插入] = 新索引.emplace(信息->登记键, 信息->节点);
        if (!已插入 && 位置->second != 信息->节点) return false;
    }
    语言索引.swap(新索引);
    return true;
}

void 回滚语言节点(稳定编码 节点,
    const std::vector<稳定编码>& 字段组) noexcept {
    for (auto 位置 = 字段组.rbegin(); 位置 != 字段组.rend(); ++位置) {
        if (有效(*位置)) (void)全局基础数据集.删除字段(*位置);
    }
    (void)删除父子关系并删除节点(语言组织根节点, 节点);
}

} // namespace

bool 新_语言类::严格UTF8字节串有效(const std::string& 文本) noexcept {
    if (文本.empty()) return false;
    const auto* 字节 = reinterpret_cast<const unsigned char*>(文本.data());
    std::size_t 位置 = 0;
    while (位置 < 文本.size()) {
        const auto 首字节 = 字节[位置];
        std::uint32_t 码点 = 0;
        std::size_t 长度 = 0;
        if (首字节 <= 0x7F) {
            码点 = 首字节;
            长度 = 1;
        } else if (首字节 >= 0xC2 && 首字节 <= 0xDF) {
            码点 =首字节 & 0x1F;
            长度 = 2;
        } else if (首字节 >= 0xE0 && 首字节 <= 0xEF) {
            码点 = 首字节 & 0x0F;
            长度 = 3;
        } else if (首字节 >= 0xF0 && 首字节 <= 0xF4) {
            码点 = 首字节 & 0x07;
            长度 = 4;
        } else {
            return false;
        }
        if (位置 + 长度 > 文本.size()) return false;
        for (std::size_t i = 1; i < 长度; ++i) {
            const auto 后续 = 字节[位置 + i];
            if ((后续 & 0xC0) != 0x80) return false;
            码点 = (码点 << 6) | (后续 & 0x3F);
        }
        if ((长度 == 2 && 码点 < 0x80)
            || (长度 == 3 && 码点 < 0x800)
            || (长度 == 4 && 码点 < 0x10000)
            || (码点 >= 0xD800 && 码点 <= 0xDFFF)
            || 码点 > 0x10FFFF
            || 码点 == 0xFEFF
            || 码点 == 0xFFFD) {
            return false;
        }
        位置 += 长度;
    }
    return true;
}

bool 新_语言类::初始化() noexcept {
    try {
        std::lock_guard 锁(新语言互斥);
        auto 建立独立节点 = [](稳定编码& 节点) {
            if (有效(节点)) return 节点仍存在(节点);
            节点 = 全局基础数据集.新建节点();
            return 有效(节点);
        };
        if (!建立独立节点(节点类型字段)
            || !建立独立节点(自然语言组织根类型)
            || !建立独立节点(语言组织根类型)
            || !建立独立节点(语言节点类型)
            || !建立独立节点(语言登记键字段)
            || !建立独立节点(创建来源字段)) {
            return false;
        }
        if (!有效(自然语言组织根节点)) {
            自然语言组织根节点 = 全局基础数据集.新建节点();
            if (!有效(自然语言组织根节点)) return false;
        } else if (!节点仍存在(自然语言组织根节点)) {
            return false;
        }
        if (!确保唯一类型字段(
            自然语言组织根节点, 自然语言组织根类型)) {
            return false;
        }
        if (!有效(语言组织根节点)) {
            语言组织根节点 = 全局基础数据集.新建节点(自然语言组织根节点);
            if (!有效(语言组织根节点)) return false;
        } else if (!节点仍存在(语言组织根节点)
            || !有唯一父节点(语言组织根节点, 自然语言组织根节点)) {
            return false;
        }
        if (!确保唯一类型字段(语言组织根节点, 语言组织根类型)) {
            return false;
        }
        return 结构仍存在() && 重建语言索引();
    } catch (...) {
        return false;
    }
}

新语言建立结果 新_语言类::建立或取得语言(
    const 新语言建立请求& 请求) noexcept {
    新语言建立结果 结果;
    try {
        std::lock_guard 锁(新语言互斥);
        if (!严格UTF8字节串有效(请求.登记键)) return 结果;
        if (!结构句柄完整()) {
            结果.状态 = 新语言建立状态::依赖不存在;
            return 结果;
        }
        if (!结构仍存在()) {
            结果.状态 = 新语言建立状态::结构不一致;
            return 结果;
        }
        if (请求.创建来源节点
            && !节点仍存在(*请求.创建来源节点)) {
            结果.状态 = 新语言建立状态::依赖不存在;
            return 结果;
        }
        const auto 已有 = 语言索引.find(请求.登记键);
        if (已有 != 语言索引.end()) {
            const auto 信息 = 读取语言信息(已有->second);
            if (!信息 || 信息->登记键 != 请求.登记键) {
                结果.状态 = 新语言建立状态::结构不一致;
                return 结果;
            }
            结果.状态 = 新语言建立状态::已复用;
            结果.语言节点 = 已有->second;
            return 结果;
        }

        const auto 节点 = 全局基础数据集.新建节点(语言组织根节点);
        if (!有效(节点)) {
            结果.状态 = 新语言建立状态::资源失败;
            return 结果;
        }
        std::vector<稳定编码> 字段组;
        字段组.push_back(全局基础数据集.添加字段节点(
            节点, 节点类型字段, 语言节点类型));
        if (有效(字段组.back())) {
            字段组.push_back(全局基础数据集.添加字段值(
                节点, 语言登记键字段, 基础值{请求.登记键}));
        }
        if (请求.创建来源节点
            && !字段组.empty() && 有效(字段组.back())) {
            字段组.push_back(全局基础数据集.添加字段节点(
                节点, 创建来源字段, *请求.创建来源节点));
        }
        if (字段组.empty()
            || std::any_of(字段组.begin(), 字段组.end(), [](稳定编码 字段) {
                return !有效(字段);
            })) {
            回滚语言节点(节点, 字段组);
            结果.状态 = 新语言建立状态::资源失败;
            return 结果;
        }
        const auto 信息 = 读取语言信息(节点);
        if (!信息 || 信息->登记键 != 请求.登记键
            || 信息->创建来源节点 != 请求.创建来源节点) {
            回滚语言节点(节点, 字段组);
            结果.状态 = 新语言建立状态::结构不一致;
            return 结果;
        }
        const auto [_, 已插入] = 语言索引.emplace(请求.登记键, 节点);
        if (!已插入) {
            回滚语言节点(节点, 字段组);
            结果.状态 = 新语言建立状态::结构不一致;
            return 结果;
        }
        结果.状态 = 新语言建立状态::已建立;
        结果.语言节点 = 节点;
        return 结果;
    } catch (...) {
        结果.状态 = 新语言建立状态::资源失败;
        return 结果;
    }
}

新语言查询结果 新_语言类::获取语言(稳定编码 语言节点) const noexcept {
    新语言查询结果 结果;
    try {
        std::lock_guard 锁(新语言互斥);
        if (!有效(语言节点)) return 结果;
        if (!结构仍存在()) {
            结果.状态 = 新语言查询状态::结构不一致;
            return 结果;
        }
        if (!节点仍存在(语言节点)) {
            结果.状态 = 新语言查询状态::未找到;
            return 结果;
        }
        if (!类型为(语言节点, 语言节点类型)) {
            结果.状态 = 新语言查询状态::输入不合法;
            return 结果;
        }
        const auto 信息 = 读取语言信息(语言节点);
        if (!信息) {
            结果.状态 = 新语言查询状态::结构不一致;
            return 结果;
        }
        结果.状态 = 新语言查询状态::已找到;
        结果.信息 = 信息;
        return 结果;
    } catch (...) {
        结果.状态 = 新语言查询状态::资源失败;
        return 结果;
    }
}

新语言查询结果 新_语言类::按登记键查询(
    const std::string& 登记键) const noexcept {
    新语言查询结果 结果;
    try {
        std::lock_guard 锁(新语言互斥);
        if (!严格UTF8字节串有效(登记键)) return 结果;
        if (!结构仍存在()) {
            结果.状态 = 新语言查询状态::结构不一致;
            return 结果;
        }
        const auto 位置 = 语言索引.find(登记键);
        if (位置 == 语言索引.end()) {
            结果.状态 = 新语言查询状态::未找到;
            return 结果;
        }
        const auto 信息 = 读取语言信息(位置->second);
        if (!信息 || 信息->登记键 != 登记键) {
            结果.状态 = 新语言查询状态::结构不一致;
            return 结果;
        }
        结果.状态 = 新语言查询状态::已找到;
        结果.信息 = 信息;
        return 结果;
    } catch (...) {
        结果.状态 = 新语言查询状态::资源失败;
        return 结果;
    }
}

bool 新_语言类::是语言节点(稳定编码 节点) const noexcept {
    const auto 结果 = 获取语言(节点);
    return 结果.状态 == 新语言查询状态::已找到;
}

新语言词条匹配结果 新_语言类::按已登记词条最长匹配(
    稳定编码 语言节点, const std::string& 文本,
    const 新_词类& 词服务) const noexcept {
    try {
        if (!有效(语言节点) || (!文本.empty() && !严格UTF8字节串有效(文本))) {
            return {};
        }
        // 各领域读取各自持锁并返回副本；此处不能持新语言互斥再调词服务。
        const auto 语言 = 获取语言(语言节点);
        if (语言.状态 != 新语言查询状态::已找到) return {语言.状态, {}};
        if (!语言.信息 || 语言.信息->节点 != 语言节点) {
            return {新语言查询状态::结构不一致, {}};
        }
        if (文本.empty()) return {新语言查询状态::已找到, {}};

        const auto 词组 = 词服务.查询语言词条(语言节点);
        switch (词组.状态) {
        case 新词条查询状态::已找到:
            if (词组.词条节点组.empty()) return {新语言查询状态::结构不一致, {}};
            break;
        case 新词条查询状态::未找到:
            if (!词组.词条节点组.empty()) return {新语言查询状态::结构不一致, {}};
            break;
        case 新词条查询状态::输入不合法:
            return {新语言查询状态::输入不合法, {}};
        case 新词条查询状态::资源失败:
            return {新语言查询状态::资源失败, {}};
        default:
            return {新语言查询状态::结构不一致, {}};
        }

        std::vector<本次词面匹配节点> 匹配树(1);
        std::set<稳定编码> 已读词条;
        for (const auto 词条 : 词组.词条节点组) {
            if (!有效(词条) || !已读词条.insert(词条).second) {
                return {新语言查询状态::结构不一致, {}};
            }
            const auto 词 = 词服务.获取词条(词条);
            if (词.状态 == 新词条查询状态::资源失败) return {新语言查询状态::资源失败, {}};
            if (词.状态 != 新词条查询状态::已找到 || !词.信息
                || 词.信息->节点 != 词条 || 词.信息->语言节点 != 语言节点
                || !严格UTF8字节串有效(词.信息->规范化词面)) {
                return {新语言查询状态::结构不一致, {}};
            }
            std::size_t 树位置 = 0;
            std::size_t 字节位置 = 0;
            while (字节位置 < 词.信息->规范化词面.size()) {
                const auto 码点 = 读取已校验码点(词.信息->规范化词面, 字节位置);
                const auto 已有 = 匹配树[树位置].后继.find(码点);
                if (已有 != 匹配树[树位置].后继.end()) {
                    树位置 = 已有->second;
                } else {
                    const auto 新位置 = 匹配树.size();
                    匹配树[树位置].后继.emplace(码点, 新位置);
                    匹配树.emplace_back(); // 扩容后不使用旧引用或迭代器。
                    树位置 = 新位置;
                }
            }
            if (匹配树[树位置].词条节点) return {新语言查询状态::结构不一致, {}};
            匹配树[树位置].词条节点 = 词条;
        }

        std::vector<std::uint32_t> 码点组;
        std::vector<std::size_t> 字节边界{0};
        std::size_t 字节位置 = 0;
        while (字节位置 < 文本.size()) {
            码点组.push_back(读取已校验码点(文本, 字节位置));
            字节边界.push_back(字节位置);
        }
        新语言词条匹配结果 结果{新语言查询状态::已找到, {}};
        std::size_t 起点 = 0;
        while (起点 < 码点组.size()) {
            std::size_t 树位置 = 0;
            std::size_t 终点 = 起点 + 1;
            std::optional<稳定编码> 命中;
            for (auto i = 起点; i < 码点组.size(); ++i) {
                const auto 后继 = 匹配树[树位置].后继.find(码点组[i]);
                if (后继 == 匹配树[树位置].后继.end()) break;
                树位置 = 后继->second;
                if (匹配树[树位置].词条节点) {
                    命中 = 匹配树[树位置].词条节点;
                    终点 = i + 1;
                }
            }
            if (!命中 && !结果.片段组.empty() && !结果.片段组.back().词条节点) {
                auto& 末段 = 结果.片段组.back();
                末段.原文.append(文本, 字节边界[起点], 字节边界[终点] - 字节边界[起点]);
                末段.原文字节长度 += 字节边界[终点] - 字节边界[起点];
                ++末段.原文码点数量;
            } else {
                const auto 字节长度 = 字节边界[终点] - 字节边界[起点];
                结果.片段组.push_back({命中, 字节边界[起点], 字节长度,
                    起点, 终点 - 起点, 文本.substr(字节边界[起点], 字节长度)});
            }
            起点 = 终点;
        }
        return 结果;
    } catch (...) {
        return {新语言查询状态::资源失败, {}};
    }
}

} // namespace 海中鱼巣
