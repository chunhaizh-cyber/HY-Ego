#include "基础数据集.快照格式.h"

#include <algorithm>
#include <bit>
#include <new>
#include <stdexcept>
#include <unordered_set>

namespace 海中鱼巣 {
namespace {

constexpr std::uint32_t 载荷版本 = 1;

enum class 材料标签 : std::uint8_t {
    节点引用 = 1,
    有符号整数 = 2,
    有符号数组 = 3,
    无符号数组 = 4,
    字节字符串 = 5
};

class 字节写入器 final {
public:
    template <typename T>
    void 写整数(T 值) {
        for (std::size_t i = 0; i < sizeof(T); ++i)
            字节.push_back(static_cast<std::uint8_t>(值 >> (i * 8)));
    }

    void 写有符号整数(std::int64_t 值) {
        写整数(std::bit_cast<std::uint64_t>(值));
    }

    void 写编码(稳定编码 编码) { 写整数(编码.值); }

    void 写字节(std::span<const std::uint8_t> 材料) {
        if (!材料.empty())
            字节.insert(字节.end(), 材料.begin(), 材料.end());
    }

    std::vector<std::uint8_t> 字节;
};

class 字节读取器 final {
public:
    explicit 字节读取器(std::span<const std::uint8_t> 字节) : 剩余(字节) {}

    std::span<const std::uint8_t> 读字节(std::size_t 长度) {
        if (长度 > 剩余.size())
            throw 基础持久状态::材料不完整;
        const auto 部分 = 剩余.first(长度);
        剩余 = 剩余.subspan(长度);
        return 部分;
    }

    template <typename T>
    T 读整数() {
        const auto 字节 = 读字节(sizeof(T));
        std::uint64_t 值 = 0;
        for (std::size_t i = 0; i < 字节.size(); ++i)
            值 |= static_cast<std::uint64_t>(字节[i]) << (i * 8);
        return static_cast<T>(值);
    }

    std::int64_t 读有符号整数() {
        return std::bit_cast<std::int64_t>(读整数<std::uint64_t>());
    }

    稳定编码 读编码() { return {读整数<std::uint64_t>()}; }

    std::size_t 读计数(std::size_t 单项最少字节) {
        const auto 数量 = 读整数<std::uint64_t>();
        if (数量 > 剩余.size() / 单项最少字节)
            throw 基础持久状态::材料不完整;
        return static_cast<std::size_t>(数量);
    }

    bool 已读完() const { return 剩余.empty(); }

private:
    std::span<const std::uint8_t> 剩余;
};

template <typename 节点类型>
bool 编码先于(const 节点类型& 左, const 节点类型& 右) {
    return 左.编码 < 右.编码;
}

void 核验映像(const 基础快照映像& 映像) {
    if (映像.下一个编码 == 0 || std::ranges::all_of(映像.数据集身份,
        [](std::uint8_t 字节) { return 字节 == 0; }))
        throw 基础持久状态::结构冲突;

    std::unordered_set<std::uint64_t> 全部编码;
    const auto 核验组编码 = [&](const auto& 组) {
        std::uint64_t 上一编码 = 0;
        for (const auto& 项 : 组) {
            const auto 编码 = 项.编码.值;
            if (编码 <= 上一编码 || 编码 >= 映像.下一个编码 || !全部编码.insert(编码).second)
                throw 基础持久状态::结构冲突;
            上一编码 = 编码;
        }
    };
    核验组编码(映像.节点组);
    核验组编码(映像.外部关系组);
    核验组编码(映像.字段关系组);

    const auto 核验节点 = [&](稳定编码 编码) {
        const auto 位置 = std::lower_bound(映像.节点组.begin(), 映像.节点组.end(),
            基础节点{编码}, 编码先于<基础节点>);
        if (位置 == 映像.节点组.end() || 位置->编码 != 编码)
            throw 基础持久状态::结构冲突;
    };
    for (const auto& 关系 : 映像.外部关系组) {
        if (关系.类型 != 基础外部关系类型::父子 && 关系.类型 != 基础外部关系类型::兄弟)
            throw 基础持久状态::结构冲突;
        核验节点(关系.源节点);
        核验节点(关系.目标节点);
    }
    for (const auto& 字段 : 映像.字段关系组) {
        核验节点(字段.所属节点);
        核验节点(字段.字段节点);
        if (const auto* 引用 = std::get_if<稳定编码>(&字段.内容))
            核验节点(*引用);
    }
}

void 写字段内容(字节写入器& 写, const 基础字段内容& 内容) {
    const auto 写标签 = [&](材料标签 标签) { 写.写整数(static_cast<std::uint8_t>(标签)); };
    if (const auto* 引用 = std::get_if<稳定编码>(&内容)) {
        写标签(材料标签::节点引用);
        写.写编码(*引用);
        return;
    }

    const auto& 材料 = std::get<基础值>(内容).材料;
    if (const auto* 整数 = std::get_if<std::int64_t>(&材料)) {
        写标签(材料标签::有符号整数);
        写.写有符号整数(*整数);
    } else if (const auto* 有符号数组 = std::get_if<std::vector<std::int64_t>>(&材料)) {
        写标签(材料标签::有符号数组);
        写.写整数<std::uint64_t>(有符号数组->size());
        for (const auto 值 : *有符号数组)
            写.写有符号整数(值);
    } else if (const auto* 无符号数组 = std::get_if<std::vector<std::uint64_t>>(&材料)) {
        写标签(材料标签::无符号数组);
        写.写整数<std::uint64_t>(无符号数组->size());
        for (const auto 值 : *无符号数组)
            写.写整数(值);
    } else {
        const auto& 字符串 = std::get<std::string>(材料);
        写标签(材料标签::字节字符串);
        写.写整数<std::uint64_t>(字符串.size());
        写.写字节({reinterpret_cast<const std::uint8_t*>(字符串.data()), 字符串.size()});
    }
}

基础字段内容 读字段内容(字节读取器& 读) {
    switch (static_cast<材料标签>(读.读整数<std::uint8_t>())) {
    case 材料标签::节点引用:
        return 读.读编码();
    case 材料标签::有符号整数:
        return 基础值{读.读有符号整数()};
    case 材料标签::有符号数组: {
        const auto 数量 = 读.读计数(sizeof(std::int64_t));
        std::vector<std::int64_t> 值;
        值.reserve(数量);
        for (std::size_t i = 0; i < 数量; ++i)
            值.push_back(读.读有符号整数());
        return 基础值{std::move(值)};
    }
    case 材料标签::无符号数组: {
        const auto 数量 = 读.读计数(sizeof(std::uint64_t));
        std::vector<std::uint64_t> 值;
        值.reserve(数量);
        for (std::size_t i = 0; i < 数量; ++i)
            值.push_back(读.读整数<std::uint64_t>());
        return 基础值{std::move(值)};
    }
    case 材料标签::字节字符串: {
        const auto 字节 = 读.读字节(读.读计数(1));
        return 基础值{std::string(reinterpret_cast<const char*>(字节.data()), 字节.size())};
    }
    }
    throw 基础持久状态::结构冲突;
}

} // namespace

std::pair<基础持久状态, std::vector<std::uint8_t>> 编码基础快照(const 基础快照映像& 映像) {
    try {
        auto 有序 = 映像;
        std::ranges::sort(有序.节点组, 编码先于<基础节点>);
        std::ranges::sort(有序.外部关系组, 编码先于<基础外部关系>);
        std::ranges::sort(有序.字段关系组, 编码先于<基础字段关系>);
        核验映像(有序);

        字节写入器 写;
        写.写整数(载荷版本);
        写.写字节(有序.数据集身份);
        写.写整数(有序.下一个编码);
        写.写整数<std::uint64_t>(有序.节点组.size());
        for (const auto& 节点 : 有序.节点组)
            写.写编码(节点.编码);
        写.写整数<std::uint64_t>(有序.外部关系组.size());
        for (const auto& 关系 : 有序.外部关系组) {
            写.写编码(关系.编码);
            写.写编码(关系.源节点);
            写.写编码(关系.目标节点);
            写.写整数(static_cast<std::uint8_t>(关系.类型));
            写.写有符号整数(关系.角色或顺序);
        }
        写.写整数<std::uint64_t>(有序.字段关系组.size());
        for (const auto& 字段 : 有序.字段关系组) {
            写.写编码(字段.编码);
            写.写编码(字段.所属节点);
            写.写编码(字段.字段节点);
            写字段内容(写, 字段.内容);
        }
        写.写整数<std::uint64_t>(有序.技术材料.size());
        写.写字节(有序.技术材料);
        return {基础持久状态::已读取, std::move(写.字节)};
    } catch (基础持久状态 状态) {
        return {状态, {}};
    } catch (const std::bad_alloc&) {
        return {基础持久状态::资源失败, {}};
    } catch (const std::length_error&) {
        return {基础持久状态::资源失败, {}};
    } catch (const std::bad_variant_access&) {
        return {基础持久状态::结构冲突, {}};
    }
}

std::pair<基础持久状态, std::optional<基础快照映像>> 解码基础快照(std::span<const std::uint8_t> 字节) {
    try {
        字节读取器 读(字节);
        if (读.读整数<std::uint32_t>() != 载荷版本)
            return {基础持久状态::格式不支持, std::nullopt};

        基础快照映像 映像;
        const auto 身份 = 读.读字节(映像.数据集身份.size());
        std::ranges::copy(身份, 映像.数据集身份.begin());
        映像.下一个编码 = 读.读整数<std::uint64_t>();

        const auto 节点数 = 读.读计数(8);
        映像.节点组.reserve(节点数);
        for (std::size_t i = 0; i < 节点数; ++i)
            映像.节点组.push_back({读.读编码()});

        const auto 关系数 = 读.读计数(33);
        映像.外部关系组.reserve(关系数);
        for (std::size_t i = 0; i < 关系数; ++i) {
            映像.外部关系组.push_back({读.读编码(), 读.读编码(), 读.读编码(),
                static_cast<基础外部关系类型>(读.读整数<std::uint8_t>()), 读.读有符号整数()});
        }

        const auto 字段数 = 读.读计数(33);
        映像.字段关系组.reserve(字段数);
        for (std::size_t i = 0; i < 字段数; ++i)
            映像.字段关系组.push_back({读.读编码(), 读.读编码(), 读.读编码(), 读字段内容(读)});

        const auto 技术材料 = 读.读字节(读.读计数(1));
        if (!读.已读完())
            return {基础持久状态::材料不完整, std::nullopt};
        if (!技术材料.empty())
            映像.技术材料.assign(技术材料.begin(), 技术材料.end());
        核验映像(映像);
        return {基础持久状态::已读取, std::move(映像)};
    } catch (基础持久状态 状态) {
        return {状态, std::nullopt};
    } catch (const std::bad_alloc&) {
        return {基础持久状态::资源失败, std::nullopt};
    } catch (const std::length_error&) {
        return {基础持久状态::资源失败, std::nullopt};
    }
}

} // namespace 海中鱼巣
