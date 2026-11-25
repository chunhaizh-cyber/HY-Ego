#include "新_语言类.h"
#include "新_词类.h"
#include "概念_存在类.h"
#include "语言含义图.数据.h"
#include "新_特征类.h"
#include "新_特征值类.h"
#include "新_存在类.h"
#include "新_场景类.h"
#include "新_自然语言世界树类.h"
#include "数据操作.执行结果参与.h"
#include "../核心/容错检查.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <charconv>
#include <cwchar>
#include <initializer_list>
#include <limits>
#include <map>
#include <mutex>
#include <new>
#include <set>
#include <stdexcept>
#include <string_view>
#include <tuple>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace 海中鱼巣 {

稳定编码 自然语言组织根节点{};
稳定编码 语言组织根节点{};

namespace {

bool 新语言含义操作是招呼(新语言知识含义操作 操作) noexcept {
    return 操作==新语言知识含义操作::招呼发起||操作==新语言知识含义操作::招呼维持||操作==新语言知识含义操作::招呼结束;
}

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
稳定编码 单位名称语言字段{}, 单位名称词条字段{}, 单位名称单位字段{};

enum class 接收含义键 : std::size_t {
    采用类型, 定义图类型, 出现类型, 角色类型, 定位类型,
    来源消息, 来源范围, 来源路径, 语言, 场景, 现实根, 原文材料,
    根出现编号, 定义图, 出现成员, 定位成员, 语言采用登记,
    出现宿主, 出现编号, 出现知识, 出现定义编号, 出现作用域, 出现路径, 出现范围, 出现角色,
    角色宿主, 角色编号, 值标签, 引用类别, 实际角色特征, 引用节点, 来源概念, 选用名称,
    整数文本, 整数单位, 子出现,
    定位宿主, 定位出现, 定位角色, 定位范围, 总数
};
std::array<稳定编码, static_cast<std::size_t>(接收含义键::总数)> 接收含义句柄{};
enum class 语言分界键 : std::size_t {
    角色, 所属语言组织, 来源消息, 来源根范围, 来源根路径,
    来源正文材料, 来源登记, 范围登记, 范围来源, 范围语言, 范围合同, 范围场景,
    范围区间, 范围候选F, 范围依据F, 范围结果F, 范围准入F,
    范围材料登记, 材料范围, 材料定义, 范围采用登记,
    采用来源, 采用种类, 采用材料, 采用特征类型, 采用特征, 采用状态, 采用状态时间,
    接收候选采用登记, 正式依据采用登记, 接收准入采用登记, 总数
};
std::array<稳定编码, static_cast<std::size_t>(语言分界键::总数)> 语言分界句柄{};
std::atomic<std::uint64_t> 分界身份范围{0};
std::mutex 新语言分界采用互斥;
constexpr std::int64_t 首个分界材料角色 = 3;
constexpr std::int64_t 末个分界材料角色 = 6;

constexpr std::array 范围特征键组{语言分界键::范围候选F, 语言分界键::范围依据F,
    语言分界键::范围结果F, 语言分界键::范围准入F};

稳定编码 分界键(语言分界键 键) noexcept {
    return 语言分界句柄[static_cast<std::size_t>(键)];
}
稳定编码 采用键(接收含义键 键) noexcept {
    return 接收含义句柄[static_cast<std::size_t>(键)];
}
bool 初始化接收含义结构已加锁() {
    if (std::all_of(接收含义句柄.begin(), 接收含义句柄.end(), [](auto 节点) { return !有效(节点); })) {
        const auto 新建 = 全局基础数据集.原子建立节点组(
            std::vector<基础组内新节点>(接收含义句柄.size()));
        if (新建.状态 == 基础结构操作状态::资源失败) return false;
        if (新建.状态 != 基础结构操作状态::已完成 || 新建.节点组.size() != 接收含义句柄.size())
            throw std::logic_error("received meaning schema publication failed");
        std::copy(新建.节点组.begin(), 新建.节点组.end(), 接收含义句柄.begin());
    }
    const auto 读 = 全局基础数据集.查询节点组完整结构(
        {接收含义句柄.begin(), 接收含义句柄.end()});
    if (读.状态 == 基础结构操作状态::资源失败) return false;
    if (读.状态 != 基础结构操作状态::已完成 || 读.节点组.size() != 接收含义句柄.size())
        throw std::logic_error("received meaning schema complete read failed");
    for (const auto& 项 : 读.节点组)
        if (!项.快照 || !项.快照->结构.字段组.empty())
            throw std::logic_error("received meaning schema identity corrupt");
    return true;
}

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
    const std::array 节点组{
        自然语言组织根节点, 语言组织根节点, 节点类型字段,
        自然语言组织根类型, 语言组织根类型, 语言节点类型,
        语言登记键字段, 创建来源字段};
    return std::all_of(节点组.begin(), 节点组.end(), [](稳定编码 节点) {
        return 有效(节点);
    });
}

bool 结构仍存在() {
    if (!结构句柄完整()) return false;
    const std::array 节点组{
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

namespace {
struct 分界来源记录 final {
    稳定编码 节点, 正文材料;
    新接收认知来源键 来源;
};
struct 分界来源闭包 final {
    基础节点结构 语言组织;
    std::vector<分界来源记录> 来源组;
};
// 只在本文件传递正常的中性前提变化，不进入工程异常或业务值域。
struct 分界前提变化 final {};
struct 分界输入拒绝 final {};

[[noreturn]] void 分界来源工程终止(const wchar_t* 入口, 稳定编码 消息,
    std::optional<稳定编码> 来源, const char* 异常说明) noexcept {
    wchar_t 原因[192]{};
    (void)swprintf_s(原因, L"来源工程不变量破坏；M=%llu，已发表来源=%llu。",
        static_cast<unsigned long long>(消息.值),
        static_cast<unsigned long long>(来源 ? 来源->值 : 0));
    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, 入口, 原因, 异常说明);
}

const 基础联合节点快照& 取分界快照(
    const 基础联合节点查询结果& 批, 稳定编码 节点) {
    const auto 项 = std::find_if(批.节点组.begin(), 批.节点组.end(),
        [节点](const auto& 值) { return 值.查询节点 == 节点; });
    if (项 == 批.节点组.end() || !项->快照 || 项->快照->结构.节点.编码 != 节点)
        throw std::logic_error("preunderstanding source endpoint disappeared");
    return *项->快照;
}

基础联合节点查询结果 读取分界节点组(const std::vector<稳定编码>& 节点组) {
    auto 批 = 全局基础数据集.查询节点组完整结构(节点组);
    if (批.状态 == 基础结构操作状态::资源失败) throw std::bad_alloc{};
    if (批.状态 != 基础结构操作状态::已完成 || 批.节点组.size() != 节点组.size())
        throw std::logic_error("preunderstanding source joint read failed");
    for (const auto 节点 : 节点组) (void)取分界快照(批, 节点);
    return 批;
}

bool 分界句柄已建立() {
    const auto 数量 = std::count_if(语言分界句柄.begin(), 语言分界句柄.end(),
        [](auto 节点) { return 有效(节点); });
    if (!数量) return false;
    if (static_cast<std::size_t>(数量) != 语言分界句柄.size()
        || std::set<稳定编码>(语言分界句柄.begin(), 语言分界句柄.end()).size() != 语言分界句柄.size())
        throw std::logic_error("preunderstanding source schema handles incomplete");
    return true;
}

void 核验分界组织(const 基础联合节点快照& 根) {
    if (!结构句柄完整() || 根.结构.节点.编码 != 语言组织根节点)
        throw std::logic_error("preunderstanding source language organization unavailable");
    std::size_t 类型数 = 0, 父数 = 0;
    for (const auto& 字段 : 根.结构.字段组) if (字段.字段节点 == 节点类型字段) {
        if (字段.内容 != 基础字段内容{语言组织根类型})
            throw std::logic_error("preunderstanding source language organization type corrupt");
        ++类型数;
    }
    for (const auto& 边 : 根.入向外部关系组) if (边.类型 == 基础外部关系类型::父子) {
        if (边.源节点 != 自然语言组织根节点 || 边.角色或顺序 != 0)
            throw std::logic_error("preunderstanding source language organization parent corrupt");
        ++父数;
    }
    if (类型数 != 1 || 父数 != 1)
        throw std::logic_error("preunderstanding source language organization identity corrupt");
}

void 核验分界基础批(const 基础联合节点查询结果& 批) {
    核验分界组织(取分界快照(批, 语言组织根节点));
    for (const auto 键 : 语言分界句柄)
        if (!取分界快照(批, 键).结构.字段组.empty())
            throw std::logic_error("preunderstanding source schema key has own fields");
}

bool 分界结构就绪已加锁() {
    if (!分界句柄已建立()) return false;
    if (!结构句柄完整())
        throw std::logic_error("preunderstanding source base handles lost");
    std::vector<稳定编码> 节点组{语言组织根节点};
    节点组.insert(节点组.end(), 语言分界句柄.begin(), 语言分界句柄.end());
    核验分界基础批(读取分界节点组(节点组));
    return true;
}

std::uint64_t 计数分界原文标量(const std::string& 文本) {
    std::uint64_t 数量 = 0;
    for (std::size_t 位置 = 0; 位置 < 文本.size(); ++数量) {
        if (数量 == (std::numeric_limits<std::uint64_t>::max)())
            throw 分界输入拒绝{};
        (void)读取已校验码点(文本, 位置);
    }
    return 数量;
}

分界来源记录 解析分界来源(const 基础节点结构& 结构) {
    if (结构.字段组.size() != 6)
        throw std::logic_error("preunderstanding source requires six fields");
    const auto 取 = [&](语言分界键 键) -> const 基础字段内容& {
        const 基础字段关系* 命中 = nullptr;
        for (const auto& 字段 : 结构.字段组) if (字段.字段节点 == 分界键(键)) {
            if (命中) throw std::logic_error("preunderstanding source field duplicated");
            命中 = &字段;
        }
        if (!命中) throw std::logic_error("preunderstanding source field missing");
        return 命中->内容;
    };
    if (std::get<std::int64_t>(std::get<基础值>(取(语言分界键::角色)).材料) != 1
        || std::get<稳定编码>(取(语言分界键::所属语言组织)) != 语言组织根节点)
        throw std::logic_error("preunderstanding source role or organization corrupt");
    分界来源记录 结果{结构.节点.编码, std::get<稳定编码>(取(语言分界键::来源正文材料)), {}};
    结果.来源.接收消息节点 = std::get<稳定编码>(取(语言分界键::来源消息));
    const auto& 范围 = std::get<std::vector<std::uint64_t>>(
        std::get<基础值>(取(语言分界键::来源根范围)).材料);
    结果.来源.根出现路径 = std::get<std::vector<std::int64_t>>(
        std::get<基础值>(取(语言分界键::来源根路径)).材料);
    if (!有效(结果.节点) || !有效(结果.正文材料) || !有效(结果.来源.接收消息节点)
        || 范围.size() != 2 || 范围[0] != 0 || !范围[1] || !结果.来源.根出现路径.empty())
        throw std::logic_error("preunderstanding source root locator corrupt");
    结果.来源.原文起点 = 范围[0];
    结果.来源.原文终点 = 范围[1];
    return 结果;
}

bool 分界消息投影相符(const 基础只读预期& 预期, const 基础联合节点查询结果& 批) {
    for (const auto& 节点 : 预期.节点组) (void)取分界快照(批, 节点.编码);
    for (const auto& 选择 : 预期.字段集合组) {
        const auto& 字段组 = 取分界快照(批, 选择.节点).结构.字段组;
        std::vector<基础字段关系> 当前;
        for (const auto& 字段 : 字段组)
            if ((!选择.字段节点 || 字段.字段节点 == *选择.字段节点)
                && std::find(选择.排除字段键组.begin(), 选择.排除字段键组.end(), 字段.字段节点)
                    == 选择.排除字段键组.end()) 当前.push_back(字段);
        if (当前 != 选择.字段组) return false;
    }
    return true;
}

std::vector<稳定编码> 读取分界登记(const 基础节点结构& 根) {
    std::vector<稳定编码> 登记;
    for (const auto& 字段 : 根.字段组) if (字段.字段节点 == 分界键(语言分界键::来源登记)) {
        const auto* 节点 = std::get_if<稳定编码>(&字段.内容);
        if (!节点 || !有效(*节点)) throw std::logic_error("preunderstanding source registry corrupt");
        登记.push_back(*节点);
    }
    std::sort(登记.begin(), 登记.end());
    if (std::adjacent_find(登记.begin(), 登记.end()) != 登记.end())
        throw std::logic_error("preunderstanding source registered twice");
    return 登记;
}

分界来源闭包 读取分界来源闭包已加锁(const 基础只读预期& 预期) {
    const auto 发现 = 读取分界节点组({语言组织根节点});
    const auto 登记 = 读取分界登记(取分界快照(发现, 语言组织根节点).结构);
    std::vector<稳定编码> 节点组{语言组织根节点};
    节点组.insert(节点组.end(), 语言分界句柄.begin(), 语言分界句柄.end());
    节点组.insert(节点组.end(), 登记.begin(), 登记.end());
    for (const auto& 节点 : 预期.节点组) 节点组.push_back(节点.编码);
    std::sort(节点组.begin(), 节点组.end());
    节点组.erase(std::unique(节点组.begin(), 节点组.end()), 节点组.end());
    const auto 批 = 读取分界节点组(节点组);
    核验分界基础批(批);
    const auto& 根 = 取分界快照(批, 语言组织根节点).结构;
    if (读取分界登记(根) != 登记)
        throw std::logic_error("preunderstanding source registry changed under language lock");
    分界来源闭包 结果{根, {}};
    std::set<std::tuple<稳定编码, std::uint64_t, std::uint64_t, std::vector<std::int64_t>>> 来源键组;
    for (const auto 节点 : 登记) {
        auto 来源 = 解析分界来源(取分界快照(批, 节点).结构);
        const auto& 键 = 来源.来源;
        if (!来源键组.emplace(键.接收消息节点, 键.原文起点, 键.原文终点, 键.根出现路径).second)
            throw std::logic_error("preunderstanding source key duplicated");
        结果.来源组.push_back(std::move(来源));
    }
    if (!分界消息投影相符(预期, 批)) throw 分界前提变化{};
    return 结果;
}

void 核验分界消息端口(const 新分界消息读取结果& 读取, 稳定编码 消息节点) {
    switch (读取.状态) {
    case 新语言分界工程状态::已完成: case 新语言分界工程状态::未找到:
    case 新语言分界工程状态::未初始化: case 新语言分界工程状态::输入拒绝:
    case 新语言分界工程状态::未支持: case 新语言分界工程状态::前提已变化:
    case 新语言分界工程状态::资源失败: break;
    default: throw std::logic_error("boundary message unknown engineering state");
    }
    if (读取.状态 != 新语言分界工程状态::已完成) {
        if (读取.消息 || !读取.发布前提.节点组.empty() || !读取.发布前提.字段集合组.empty()
            || !读取.发布前提.外部关系集合组.empty())
            throw std::logic_error("boundary message failure carries success payload");
        return;
    }
    if (!读取.消息) throw std::logic_error("boundary message success without message");
    const auto& 消息 = *读取.消息;
    const auto& 预期 = 读取.发布前提;
    if (消息.消息节点 != 消息节点 || 消息.来源.方向 != 新外设消息方向::接收
        || !有效(消息.正文材料节点) || !有效(消息.场景节点) || !有效(消息.窗口节点)
        || !新_语言类::严格UTF8字节串有效(消息.UTF8正文)
        || 预期.字段集合组.size() != 1 || !预期.外部关系集合组.empty())
        throw std::logic_error("boundary message success contract corrupt");
    std::set<稳定编码> 节点组;
    for (const auto& 节点 : 预期.节点组)
        if (!有效(节点.编码) || !节点组.insert(节点.编码).second)
            throw std::logic_error("boundary message endpoint projection duplicated");
    for (const auto 节点 : {消息节点, 消息.正文材料节点, 消息.场景节点, 消息.窗口节点})
        if (!节点组.contains(节点)) throw std::logic_error("boundary message endpoint projection missing");
    const auto& 选择 = 预期.字段集合组.front();
    if (节点组.size() != 5 || 选择.节点 != 消息节点 || 选择.字段节点
        || !选择.排除字段键组.empty()
        || 选择.字段组.size() != 10 + static_cast<std::size_t>(消息.回应消息节点.has_value()))
        throw std::logic_error("boundary message field selector contract corrupt");
    std::set<稳定编码> 字段码组, 字段键组;
    for (const auto& 字段 : 选择.字段组)
        if (字段.所属节点 != 消息节点 || !有效(字段.编码) || !有效(字段.字段节点)
            || !字段码组.insert(字段.编码).second || !字段键组.insert(字段.字段节点).second
            || 字段.内容.valueless_by_exception())
            throw std::logic_error("boundary message neutral field projection corrupt");
}

void 访问分界来源(const 新接收认知来源键& 来源, const 新语言分界消息读取接口& 端口,
    bool 允许建立, 新语言分界调用结果<新语言前置来源信息>& 结果) {
    using 状态 = 新语言分界工程状态;
    if (!有效(来源.接收消息节点) || 来源.原文起点 != 0 || !来源.原文终点) return;
    if (!来源.根出现路径.empty()) { 结果.状态 = 状态::未支持; return; }
    {
        std::lock_guard 锁(新语言互斥);
        if (!分界结构就绪已加锁()) { 结果.状态 = 状态::未初始化; return; }
    }
    // 跨owner调用严格放在语言锁外；只借用本次调用，不保存端口。
    auto 读取 = 端口.读取分界消息正文(来源.接收消息节点);
    核验分界消息端口(读取, 来源.接收消息节点);
    if (读取.状态 != 状态::已完成 && 读取.状态 != 状态::未找到) {
        结果.状态 = 读取.状态;
        return;
    }
    if (读取.消息 && 计数分界原文标量(读取.消息->UTF8正文) != 来源.原文终点) return;
    std::lock_guard 锁(新语言互斥);
    if (!分界句柄已建立()) throw std::logic_error("preunderstanding source schema lost during callback");
    auto 闭包 = 读取分界来源闭包已加锁(读取.发布前提);
    const auto 查找 = [&](const 分界来源闭包& 值) {
        return std::find_if(值.来源组.begin(), 值.来源组.end(),
            [&](const auto& 记录) { return 记录.来源 == 来源; });
    };
    auto 已有 = 查找(闭包);
    if (!读取.消息) {
        if (已有 != 闭包.来源组.end()) throw std::logic_error("registered source message is no longer readable");
        结果.状态 = 状态::未找到;
        return;
    }
    const auto& 消息 = *读取.消息;
    for (const auto& 记录 : 闭包.来源组)
        if (记录.来源.接收消息节点 == 来源.接收消息节点
            && (记录.来源 != 来源 || 记录.正文材料 != 消息.正文材料节点))
            throw std::logic_error("preunderstanding source root/body disagrees with formal message");
    if (已有 != 闭包.来源组.end()) {
        if (已有->正文材料 != 消息.正文材料节点)
            throw std::logic_error("preunderstanding source immutable body identity changed");
        结果.信息 = 新语言前置来源信息{已有->节点, 已有->正文材料, 已有->来源, 消息.UTF8正文};
        结果.状态 = 状态::已完成;
        return;
    }
    if (!允许建立) { 结果.状态 = 状态::未找到; return; }
    // 发布前准备载荷；发表身份的无分配保存先于完整正式读回。
    新语言前置来源信息 信息{{}, 消息.正文材料节点, 来源, 消息.UTF8正文};
    基础节点组追加请求 请求;
    请求.新节点组.push_back({{
        {分界键(语言分界键::角色), 基础值{std::int64_t{1}}},
        {分界键(语言分界键::所属语言组织), 语言组织根节点},
        {分界键(语言分界键::来源消息), 来源.接收消息节点},
        {分界键(语言分界键::来源根范围), 基础值{std::vector<std::uint64_t>{0, 来源.原文终点}}},
        {分界键(语言分界键::来源根路径), 基础值{std::vector<std::int64_t>{}}},
        {分界键(语言分界键::来源正文材料), 消息.正文材料节点}
    }});
    请求.追加字段组.push_back({语言组织根节点, 分界键(语言分界键::来源登记), 基础组内节点引用{0}});
    请求.预期既有节点组.push_back(闭包.语言组织);
    请求.只读预期 = 读取.发布前提;
    for (const auto 键 : 语言分界句柄) {
        请求.只读预期.节点组.push_back({键});
        请求.只读预期.字段集合组.push_back({键, std::nullopt, {}, {}});
    }
    const auto 发表 = 全局基础数据集.原子追加节点组(请求);
    if (发表.状态 == 基础节点组追加状态::预期不符) { 结果.状态 = 状态::前提已变化; return; }
    if (发表.状态 == 基础节点组追加状态::资源失败) { 结果.状态 = 状态::资源失败; return; }
    if (发表.状态 != 基础节点组追加状态::已完成 || 发表.新节点组.size() != 1)
        throw std::logic_error("preunderstanding source atomic publication failed");
    结果.本次已发表身份 = 发表.新节点组.front();
    if (!有效(*结果.本次已发表身份) || 发表.新节点初始字段关系组.size() != 1
        || 发表.新节点初始字段关系组.front().size() != 6 || 发表.追加字段关系组.size() != 1)
        throw std::logic_error("preunderstanding source publication receipt corrupt");
    闭包 = 读取分界来源闭包已加锁(读取.发布前提);
    已有 = 查找(闭包);
    if (已有 == 闭包.来源组.end() || 已有->节点 != *结果.本次已发表身份
        || 已有->正文材料 != 信息.固定文本材料)
        throw std::logic_error("preunderstanding source formal readback failed");
    信息.节点 = 已有->节点;
    信息.来源 = 已有->来源;
    结果.信息 = std::move(信息);
    结果.状态 = 状态::已完成;
}

新语言材料范围信息 解码语言范围(const 基础节点结构& 结构) {
    const auto 取 = [&](语言分界键 键) -> const 基础字段内容* {
        const 基础字段内容* 结果 = nullptr;
        for (const auto& 项 : 结构.字段组) if (项.字段节点 == 分界键(键)) {
            if (结果) throw std::logic_error("language range field duplicated");
            结果 = &项.内容;
        }
        return 结果;
    };
    const auto 节点值 = [&](语言分界键 键) {
        const auto* 内容 = 取(键);
        if (!内容) throw std::logic_error("language range metadata missing");
        return std::get<稳定编码>(*内容);
    };
    const auto* 角色 = 取(语言分界键::角色);
    if (!角色 || std::get<std::int64_t>(std::get<基础值>(*角色).材料) != 2
        || 节点值(语言分界键::所属语言组织) != 语言组织根节点)
        throw 分界输入拒绝{};
    新语言材料范围信息 结果;
    结果.节点 = 结构.节点.编码;
    结果.定位.前置来源节点 = 节点值(语言分界键::范围来源);
    结果.定位.语言节点 = 节点值(语言分界键::范围语言);
    结果.定位.规范化合同节点 = 节点值(语言分界键::范围合同);
    结果.定位.所属语言场景 = 节点值(语言分界键::范围场景);
    const auto* 区间内容 = 取(语言分界键::范围区间);
    if (!区间内容) throw std::logic_error("language range interval missing");
    const auto& 区间 = std::get<std::vector<std::uint64_t>>(std::get<基础值>(*区间内容).材料);
    if (区间.size() != 2 || 区间[0] >= 区间[1])
        throw std::logic_error("language range interval corrupt");
    结果.定位.起始标量 = 区间[0];
    结果.定位.结束标量 = 区间[1];
    for (std::size_t i = 0; i < 范围特征键组.size(); ++i) {
        if (const auto* 内容 = 取(范围特征键组[i]))
            结果.特征节点组[i] = std::get<稳定编码>(*内容);
    }
    return 结果;
}

class 语言空特征持有 final : public 新空特征持有参与接口 {
public:
    语言空特征持有(稳定编码 R, 稳定编码 键) : 范围_(R), 键_(键) {}
    基础节点组追加请求 构造空特征持有(基础组内节点引用 F) const override {
        基础节点组追加请求 请求;
        请求.只读预期.节点组 = {{范围_}, {键_}};
        请求.只读预期.字段集合组.push_back({范围_, 键_, {}, {}});
        请求.当前追加字段键组.push_back({范围_, 键_});
        请求.追加字段组.push_back({范围_, 键_, F});
        return 请求;
    }
private:
    稳定编码 范围_, 键_;
};

class 语言分界稳定身份读取 final : public 新信息对象身份读取接口 {
public:
    新信息对象身份读取结果 读取信息对象身份(稳定编码 节点) const noexcept override {
        using S = 新信息对象身份读取状态;
        try {
            const 稳定编码 范围{分界身份范围.load(std::memory_order_acquire)};
            if (!有效(范围)) return {S::依赖未就绪, {}};
            if (!有效(节点)) return {S::信息不存在, {}};
            const auto 批 = 全局基础数据集.查询节点组完整结构({节点});
            if (批.状态 == 基础结构操作状态::资源失败) return {S::资源失败, {}};
            if (批.状态 == 基础结构操作状态::节点不存在) return {S::信息不存在, {}};
            if (批.状态 != 基础结构操作状态::已完成 || 批.节点组.size() != 1)
                throw std::logic_error("language range identity read failed");
            if (!批.节点组.front().快照) return {S::信息不存在, {}};
            const auto& 字段组 = 批.节点组.front().快照->结构.字段组;
            const auto 有字段 = [&](语言分界键 键, const 基础字段内容& 值) {
                return std::any_of(字段组.begin(), 字段组.end(), [&](const auto& 字段) {
                    return 字段.字段节点 == 分界键(键) && 字段.内容 == 值;
                });
            };
            if (!有字段(语言分界键::所属语言组织, 语言组织根节点))
                return {S::信息不存在, {}};
            const bool 是范围或材料 = std::any_of(字段组.begin(), 字段组.end(), [](const auto& 字段) {
                if (字段.字段节点 != 分界键(语言分界键::角色)) return false;
                const auto* 值 = std::get_if<基础值>(&字段.内容);
                const auto* 角色 = 值 ? std::get_if<std::int64_t>(&值->材料) : nullptr;
                return 角色 && *角色 >= 2 && *角色 <= 末个分界材料角色;
            });
            if (!是范围或材料) return {S::信息不存在, {}};
            return {S::已读取, 新信息对象身份投影{节点,
                {范围, 新信息引用范围形状::语言分界材料身份}}};
        } catch (const std::bad_alloc&) {
            return {S::资源失败, {}};
        } catch (const std::length_error&) {
            return {S::资源失败, {}};
        } catch (const std::exception& 异常) {
            分界来源工程终止(L"读取语言分界稳定身份", 节点, {}, 异常.what());
        } catch (...) {
            分界来源工程终止(L"读取语言分界稳定身份", 节点, {}, nullptr);
        }
    }
};

std::optional<新语言材料范围信息> 读取当前语言范围(
    稳定编码 节点, 基础联合节点快照* 范围快照 = nullptr) {
    if (!有效(节点)) return std::nullopt;
    const auto 批 = 全局基础数据集.查询节点组完整结构({节点});
    if (批.状态 == 基础结构操作状态::资源失败) throw std::bad_alloc{};
    if (批.状态 == 基础结构操作状态::节点不存在) return std::nullopt;
    if (批.状态 != 基础结构操作状态::已完成 || 批.节点组.size() != 1)
        throw std::logic_error("language range read failed");
    if (!批.节点组.front().快照) return std::nullopt;
    const auto& 快照 = 取分界快照(批, 节点);
    auto 内容 = 解码语言范围(快照.结构);
    if (范围快照) *范围快照 = 快照;
    return 内容;
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
            || !建立独立节点(创建来源字段)
            || !建立独立节点(单位名称语言字段)
            || !建立独立节点(单位名称词条字段)
            || !建立独立节点(单位名称单位字段)) {
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
        return 结构仍存在() && 重建语言索引() && 初始化接收含义结构已加锁();
    } catch (...) {
        return false;
    }
}

namespace {
新语言计量单位名称结果 读取计量单位关系(
    稳定编码 语言, 稳定编码 身份, 稳定编码 查询字段, 稳定编码 结果字段) {
    using S = 新语言知识状态;
    if (!有效(查询字段) || !有效(结果字段)) return {S::尚未初始化};
    if (!有效(语言) || !有效(身份)) return {S::输入拒绝};

    新语言计量单位名称结果 结果{S::未找到};
    for (const auto& 关系 : 全局基础数据集.查询目标字段(身份, 查询字段)) {
        const auto 所属语言 = 读取唯一节点字段(关系.所属节点, 单位名称语言字段);
        if (!所属语言) return {S::内部不一致};
        if (*所属语言 != 语言) continue;
        const auto 目标 = 读取唯一节点字段(关系.所属节点, 结果字段);
        if (!目标 || 结果.匹配节点) return {S::内部不一致};
        结果 = {S::已完成, *目标};
    }
    return 结果;
}
}

新语言计量单位名称结果 新_语言类::按词条读取计量单位(
    稳定编码 语言, 稳定编码 词条) const noexcept {
    try {
        std::lock_guard 锁(新语言互斥);
        return 读取计量单位关系(语言, 词条, 单位名称词条字段, 单位名称单位字段);
    } catch (const std::bad_alloc&) {
        return {新语言知识状态::工程未完成};
    } catch (...) {
        return {新语言知识状态::内部不一致};
    }
}

新语言计量单位名称结果 新_语言类::按计量单位读取词条(
    稳定编码 语言, 稳定编码 单位) const noexcept {
    try {
        std::lock_guard 锁(新语言互斥);
        return 读取计量单位关系(语言, 单位, 单位名称单位字段, 单位名称词条字段);
    } catch (const std::bad_alloc&) {
        return {新语言知识状态::工程未完成};
    } catch (...) {
        return {新语言知识状态::内部不一致};
    }
}

新语言知识状态 新_语言类::登记计量单位词条(稳定编码 语言, 稳定编码 单位,
    稳定编码 词条, const 概念_特征类& 特征概念, const 新_词类& 词) noexcept {
    using S = 新语言知识状态;
    using A = 基础节点组追加状态;
    try {
        for (;;) {
            const auto 单位身份 = 特征概念.获取先天特征概念();
            if (!单位身份) return S::尚未初始化;
            if (单位 != 单位身份->毫米单位 && 单位 != 单位身份->摄氏度单位)
                return S::输入拒绝;
            const auto W = 词.获取词条(词条);
            if (W.状态 == 新词条查询状态::资源失败) return S::工程未完成;
            if (!W.信息 || W.信息->语言节点 != 语言) return S::输入拒绝;

            std::lock_guard 锁(新语言互斥);
            if (!读取语言信息(语言)) return S::输入拒绝;
            const auto 正向 = 读取计量单位关系(语言, 词条, 单位名称词条字段, 单位名称单位字段);
            const auto 反向 = 读取计量单位关系(语言, 单位, 单位名称单位字段, 单位名称词条字段);
            if (正向.状态 != S::未找到 && 正向.状态 != S::已完成) return 正向.状态;
            if (反向.状态 != S::未找到 && 反向.状态 != S::已完成) return 反向.状态;
            if (正向.匹配节点 == 单位 && 反向.匹配节点 == 词条) return S::已完成;
            if (正向.匹配节点 || 反向.匹配节点) return S::输入拒绝;

            基础节点组追加请求 请求;
            请求.新节点组.push_back({{{单位名称语言字段, 语言},
                {单位名称词条字段, 词条}, {单位名称单位字段, 单位}}});
            请求.只读预期.节点组 = {{语言}, {词条}, {单位}};
            const auto 发表 = 全局基础数据集.原子追加节点组(请求);
            if (发表.状态 == A::已完成) return S::已完成;
            if (发表.状态 == A::预期不符 || 发表.状态 == A::节点不存在 || 发表.状态 == A::引用不存在)
                continue;
            if (发表.状态 == A::资源失败) return S::工程未完成;
            return S::内部不一致;
        }
    } catch (const std::bad_alloc&) {
        return S::工程未完成;
    } catch (...) {
        return S::内部不一致;
    }
}

新语言分界工程状态 新_语言类::初始化分界结构() {
    using 状态 = 新语言分界工程状态;
    try {
        std::lock_guard 锁(新语言互斥);
        const bool 已有 = 分界句柄已建立();
        if (!结构句柄完整()) {
            if (已有) throw std::logic_error("preunderstanding source base handles lost");
            return 状态::未初始化;
        }
        核验分界组织(取分界快照(读取分界节点组({语言组织根节点}), 语言组织根节点));
        if (!已有) {
            const auto 建立 = 全局基础数据集.原子建立节点组(
                std::vector<基础组内新节点>(语言分界句柄.size()));
            if (建立.状态 == 基础结构操作状态::资源失败) return 状态::资源失败;
            if (建立.状态 != 基础结构操作状态::已完成 || 建立.节点组.size() != 语言分界句柄.size())
                throw std::logic_error("preunderstanding source schema publication failed");
            // 首次发布后无分配保留整组句柄，后续读回资源失败不能重新建组。
            std::copy(建立.节点组.begin(), 建立.节点组.end(), 语言分界句柄.begin());
        }
        if (!分界结构就绪已加锁())
            throw std::logic_error("preunderstanding source schema initialization incomplete");
        return 状态::已完成;
    } catch (const std::bad_alloc&) { return 状态::资源失败; }
    catch (const std::length_error&) { return 状态::资源失败; }
    catch (const std::exception& 异常) {
        分界来源工程终止(L"初始化分界结构", {}, {}, 异常.what());
    } catch (...) { 分界来源工程终止(L"初始化分界结构", {}, {}, nullptr); }
}

新语言分界调用结果<新语言前置来源信息> 新_语言类::建立或取得前置来源(
    const 新语言前置来源请求& 请求, const 新语言分界消息读取接口& 端口) {
    新语言分界调用结果<新语言前置来源信息> 结果;
    try {
        访问分界来源(请求.来源, 端口, true, 结果);
    } catch (const 分界前提变化&) { 结果.状态 = 新语言分界工程状态::前提已变化; }
    catch (const 分界输入拒绝&) { 结果.状态 = 新语言分界工程状态::输入拒绝; }
    catch (const std::bad_alloc&) { 结果.状态 = 新语言分界工程状态::资源失败; }
    catch (const std::length_error&) { 结果.状态 = 新语言分界工程状态::资源失败; }
    catch (const std::exception& 异常) {
        分界来源工程终止(L"建立或取得前置来源", 请求.来源.接收消息节点, 结果.本次已发表身份, 异常.what());
    } catch (...) {
        分界来源工程终止(L"建立或取得前置来源", 请求.来源.接收消息节点, 结果.本次已发表身份, nullptr);
    }
    if (结果.状态 != 新语言分界工程状态::已完成) 结果.信息.reset();
    return 结果;
}

新语言分界调用结果<新语言前置来源信息> 新_语言类::读取前置来源(
    const 新接收认知来源键& 来源, const 新语言分界消息读取接口& 端口) const {
    新语言分界调用结果<新语言前置来源信息> 结果;
    try {
        访问分界来源(来源, 端口, false, 结果);
    } catch (const 分界前提变化&) { 结果.状态 = 新语言分界工程状态::前提已变化; }
    catch (const 分界输入拒绝&) { 结果.状态 = 新语言分界工程状态::输入拒绝; }
    catch (const std::bad_alloc&) { 结果.状态 = 新语言分界工程状态::资源失败; }
    catch (const std::length_error&) { 结果.状态 = 新语言分界工程状态::资源失败; }
    catch (const std::exception& 异常) {
        分界来源工程终止(L"读取前置来源", 来源.接收消息节点, {}, 异常.what());
    } catch (...) { 分界来源工程终止(L"读取前置来源", 来源.接收消息节点, {}, nullptr); }
    if (结果.状态 != 新语言分界工程状态::已完成) 结果.信息.reset();
    return 结果;
}

新语言分界工程状态 新_语言类::配置分界范围特征(
    const 新语言分界特征配置& 配置, 新_特征类& 特征) {
    using S = 新语言分界工程状态;
    try {
        std::lock_guard 锁(新语言互斥);
        if (!分界结构就绪已加锁()) return S::未初始化;
        auto 类型组 = 配置.特征类型组;
        std::sort(类型组.begin(), 类型组.end());
        if (!有效(配置.信息身份范围) || !有效(类型组.front())
            || std::adjacent_find(类型组.begin(), 类型组.end()) != 类型组.end())
            return S::输入拒绝;
        if (分界特征服务_)
            return 分界特征服务_ == &特征 && 分界特征配置_ == 配置 ? S::已完成 : S::输入拒绝;
        const 稳定编码 已登记范围{分界身份范围.load(std::memory_order_relaxed)};
        if (有效(已登记范围) && 已登记范围 != 配置.信息身份范围) return S::输入拒绝;
        分界特征配置_ = 配置;
        分界特征服务_ = &特征;
        分界身份范围.store(配置.信息身份范围.值, std::memory_order_release);
        分界配置就绪_.store(true, std::memory_order_release);
        return S::已完成;
    } catch (const std::bad_alloc&) {
        return S::资源失败;
    } catch (const std::length_error&) {
        return S::资源失败;
    } catch (const std::exception& 异常) {
        分界来源工程终止(L"配置分界范围特征", {}, {}, 异常.what());
    } catch (...) {
        分界来源工程终止(L"配置分界范围特征", {}, {}, nullptr);
    }
}

const 新信息对象身份读取接口& 新_语言类::取得分界稳定身份读取接口() noexcept {
    static const 语言分界稳定身份读取 读取;
    return 读取;
}

新语言分界调用结果<新语言范围特征信息> 新_语言类::读取材料范围特征(稳定编码 节点) const {
    using S = 新语言分界工程状态;
    try {
        if (!分界配置就绪_.load(std::memory_order_acquire)) return {S::未初始化, {}, {}};
        基础联合节点快照 范围快照;
        auto 当前 = 读取当前语言范围(节点, &范围快照);
        if (!当前) return {S::未找到, {}, {}};
        if (!std::all_of(当前->特征节点组.begin(), 当前->特征节点组.end(),
            [](稳定编码 F) { return 有效(F); })) return {S::材料未就绪, {}, {}};
        return {S::已完成, 新语言范围特征信息{std::move(*当前),
            分界特征配置_.特征类型组, std::move(范围快照)}, {}};
    } catch (const 分界输入拒绝&) {
        return {S::未找到, {}, {}};
    } catch (const std::bad_alloc&) {
        return {S::资源失败, {}, {}};
    } catch (const std::length_error&) {
        return {S::资源失败, {}, {}};
    } catch (const std::exception& 异常) {
        分界来源工程终止(L"读取语言材料范围", 节点, {}, 异常.what());
    } catch (...) {
        分界来源工程终止(L"读取语言材料范围", 节点, {}, nullptr);
    }
}

新语言分界调用结果<新语言材料范围信息> 新_语言类::读取材料范围(稳定编码 节点) const {
    auto 读 = 读取材料范围特征(节点);
    新语言分界调用结果<新语言材料范围信息> 结果{读.状态, {}, 读.本次已发表身份};
    if (读.信息) 结果.信息 = std::move(读.信息->范围);
    return 结果;
}

正式信息参数读取结果 新_语言类::读取分界参数保护(
    稳定编码 节点, 稳定编码 范围) const noexcept {
    using S = 正式信息参数读取状态;
    using R = 新语言分界工程状态;
    using F = 新特征完整读取状态;
    try {
        if (!分界配置就绪_.load(std::memory_order_acquire)) return {S::依赖未就绪, {}, {}};
        const auto 参数结构范围 = 分界特征配置_.参数结构范围;
        if (范围 != 分界特征配置_.信息身份范围
            && (!有效(参数结构范围) || 范围 != 参数结构范围)) return {S::范围未支持, {}, {}};
        auto 读 = 读取材料范围特征(节点);
        switch (读.状态) {
        case R::未找到: return {S::信息不存在, {}, {}};
        case R::未初始化:
        case R::材料未就绪: return {S::依赖未就绪, {}, {}};
        case R::资源失败: return {S::资源失败, {}, {}};
        case R::前提已变化: return {S::前提已变化, {}, {}};
        case R::已完成: break;
        default: throw std::logic_error("unexpected language range projection status");
        }
        if (!读.信息) throw std::logic_error("language range projection missing");
        auto& 范围信息 = *读.信息;
        正式信息参数投影 输出{节点, 范围, {}, {}};
        输出.直接内容组.reserve(范围信息.特征类型组.size());
        输出.特征定位组.reserve(范围信息.特征类型组.size());
        std::vector<基础联合节点快照> 保护;
        保护.push_back(std::move(范围信息.范围快照));
        for (std::size_t i = 0; i < 范围信息.特征类型组.size(); ++i) {
            auto 特征 = 分界特征服务_->读取特征完整保护(范围信息.范围.特征节点组[i]);
            switch (特征.状态) {
            case F::前提已变化: return {S::前提已变化, {}, {}};
            case F::资源失败: return {S::资源失败, {}, {}};
            case F::特征不存在:
            case F::依赖未就绪: return {S::依赖未就绪, {}, {}};
            case F::类型未支持: return {S::范围未支持, {}, {}};
            case F::已读取: break;
            }
            if (!特征.信息) throw std::logic_error("language range feature projection missing");
            正式内容值 值{正式内容值种类::信息对象引用, 正式内容值状态::当前未知, {}};
            if (const auto& 当前 = 特征.信息->特征.当前值; 当前) {
                值.值 = 正式内容准确值{std::get<稳定编码>(*当前)};
                值.状态 = 正式内容值状态::已知;
            }
            输出.直接内容组.push_back({范围信息.特征类型组[i], std::move(值)});
            输出.特征定位组.push_back({范围信息.特征类型组[i], 范围信息.范围.特征节点组[i]});
            // 保护材料只传给需要建立L1写入前提的调用者。
            for (auto& 项 : 特征.信息->完整保护组) {
                const auto 编码 = 项.结构.节点.编码;
                if (std::none_of(保护.begin(), 保护.end(), [编码](const auto& 已有) {
                        return 已有.结构.节点.编码 == 编码;
                    }))
                    保护.push_back(std::move(项));
            }
        }
        return {S::已读取, std::move(输出), std::move(保护)};
    } catch (const std::bad_alloc&) {
        return {S::资源失败, {}, {}};
    } catch (const std::length_error&) {
        return {S::资源失败, {}, {}};
    } catch (const std::exception& 异常) {
        分界来源工程终止(L"读取语言分界正式参数", 节点, {}, 异常.what());
    } catch (...) {
        分界来源工程终止(L"读取语言分界正式参数", 节点, {}, nullptr);
    }
}

新语言分界调用结果<新语言材料范围信息> 新_语言类::建立或取得材料范围(
    const 新语言材料范围请求& 输入, const 新_场景类& 场景) {
    using S = 新语言分界工程状态;
    using L1 = 基础节点组追加状态;
    新语言分界调用结果<新语言材料范围信息> 结果;
    try {
        if (!有效(输入.前置来源节点) || 输入.起始标量 >= 输入.结束标量
            || 输入.规范化合同节点 != 严格UTF8字节精确规范化合同节点) return 结果;
        const auto 语言 = 获取语言(输入.语言节点);
        if (语言.状态 == 新语言查询状态::资源失败) return {S::资源失败, {}, {}};
        if (语言.状态 != 新语言查询状态::已找到
            || !场景.是场景节点(输入.所属语言场景)) return 结果;
        新_特征类* 特征 = nullptr;
        新语言分界特征配置 配置;
        稳定编码 范围;
        // 同一范围的登记和唯一键读取由语言所有者在同一锁域内完成。
        for (int 尝试 = 0; 尝试 < 2; ++尝试) {
            std::lock_guard 锁(新语言互斥);
            if (!分界特征服务_) return {S::未初始化, {}, {}};
            if (输入.前置来源节点 == 语言组织根节点) return 结果;
            特征 = 分界特征服务_;
            配置 = 分界特征配置_;
            const auto 批 = 全局基础数据集.查询节点组完整结构({语言组织根节点, 输入.前置来源节点});
            if (批.状态 == 基础结构操作状态::资源失败) throw std::bad_alloc{};
            const auto& 根 = 取分界快照(批, 语言组织根节点).结构;
            const auto 来源 = std::find_if(批.节点组.begin(), 批.节点组.end(), [&](const auto& 项) {
                return 项.查询节点 == 输入.前置来源节点;
            });
            const auto 登记 = 读取分界登记(根);
            if (来源 == 批.节点组.end() || !来源->快照
                || !std::binary_search(登记.begin(), 登记.end(), 输入.前置来源节点))
                return {S::未找到, {}, {}};
            if (输入.结束标量 > 解析分界来源(来源->快照->结构).来源.原文终点) return 结果;
            for (const auto& 字段 : 根.字段组) {
                if (字段.字段节点 != 分界键(语言分界键::范围登记)) continue;
                const auto 已有 = 读取当前语言范围(std::get<稳定编码>(字段.内容));
                if (!已有 || 已有->定位.前置来源节点 != 输入.前置来源节点
                    || 已有->定位.起始标量 != 输入.起始标量 || 已有->定位.结束标量 != 输入.结束标量)
                    continue;
                if (已有->定位 != 输入) return 结果;
                范围 = 已有->节点;
                break;
            }
            if (有效(范围)) break;

            基础节点组追加请求 请求;
            基础组内新节点 新范围;
            新范围.字段组 = {
                {分界键(语言分界键::角色), 基础值{std::int64_t{2}}},
                {分界键(语言分界键::所属语言组织), 语言组织根节点},
                {分界键(语言分界键::范围来源), 输入.前置来源节点},
                {分界键(语言分界键::范围语言), 输入.语言节点},
                {分界键(语言分界键::范围合同), 输入.规范化合同节点},
                {分界键(语言分界键::范围场景), 输入.所属语言场景},
                {分界键(语言分界键::范围区间),
                    基础值{std::vector<std::uint64_t>{输入.起始标量, 输入.结束标量}}}};
            请求.新节点组.push_back(std::move(新范围));
            请求.追加字段组.push_back({
                语言组织根节点, 分界键(语言分界键::范围登记), 基础组内节点引用{0}});
            请求.当前追加字段键组.push_back({语言组织根节点, 分界键(语言分界键::范围登记)});
            for (const auto 节点 : {语言组织根节点, 输入.前置来源节点, 输入.语言节点,
                输入.规范化合同节点, 输入.所属语言场景})
                请求.只读预期.节点组.push_back({节点});
            for (const auto 键 : 语言分界句柄) 请求.只读预期.节点组.push_back({键});
            const auto 发布 = 全局基础数据集.原子追加节点组(请求);
            if (发布.状态 == L1::资源失败) throw std::bad_alloc{};
            if (发布.状态 == L1::预期不符 || 发布.状态 == L1::节点不存在
                || 发布.状态 == L1::引用不存在)
                continue;
            if (发布.状态 != L1::已完成 || 发布.新节点组.size() != 1)
                throw std::logic_error("language range publication failed");
            范围 = 发布.新节点组.front();
            break;
        }
        if (!有效(范围)) return {S::材料未就绪, {}, {}};

        结果.本次已发表身份 = 范围;
        for (std::size_t i = 0; i < 范围特征键组.size(); ++i) {
            const auto 当前 = 读取当前语言范围(范围);
            if (!当前) return {S::未找到, {}, {}};
            if (有效(当前->特征节点组[i])) continue;
            const 语言空特征持有 持有{范围, 分界键(范围特征键组[i])};
            const auto 建立 = 特征->建立并挂接空信息引用特征(配置.特征类型组[i], 持有);
            if (建立.状态 == 新特征写入状态::前提已变化) {
                const auto 重读 = 读取当前语言范围(范围);
                if (!重读) return {S::未找到, {}, {}};
                if (有效(重读->特征节点组[i])) continue;
                return {S::材料未就绪, {}, 范围};
            }
            if (建立.状态 == 新特征写入状态::资源失败) throw std::bad_alloc{};
            if (建立.状态 != 新特征写入状态::已完成 || !建立.特征节点)
                throw std::logic_error("language range empty feature publication failed");
        }
        auto 当前 = 读取材料范围(范围);
        if (当前.状态 != S::未找到) 当前.本次已发表身份 = 范围;
        return 当前;
    } catch (const std::bad_alloc&) {
        结果.状态 = S::资源失败;
    } catch (const std::length_error&) {
        结果.状态 = S::资源失败;
    } catch (const std::exception& 异常) {
        分界来源工程终止(L"建立或取得语言材料范围", {}, 结果.本次已发表身份, 异常.what());
    } catch (...) {
        分界来源工程终止(L"建立或取得语言材料范围", {}, 结果.本次已发表身份, nullptr);
    }
    return 结果;
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
        if (请求.创建来源节点 && 有效(字段组.back())) {
            字段组.push_back(全局基础数据集.添加字段节点(
                节点, 创建来源字段, *请求.创建来源节点));
        }
        if (std::any_of(字段组.begin(), 字段组.end(), [](稳定编码 字段) {
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

namespace {
新语言查询状态 读取本次词面匹配树(稳定编码 语言节点, const 新_词类& 词服务,
    std::vector<本次词面匹配节点>& 匹配树) {
    using S = 新语言查询状态;
    using W = 新词条查询状态;
    for (int 尝试 = 0; 尝试 < 2; ++尝试) {
        const auto 词组 = 词服务.查询语言词条(语言节点);
        switch (词组.状态) {
        case W::已找到:
            if (词组.词条节点组.empty()) return S::结构不一致;
            break;
        case W::未找到:
            if (!词组.词条节点组.empty()) return S::结构不一致;
            break;
        case W::输入不合法: return S::输入不合法;
        case W::资源失败: return S::资源失败;
        default: return S::结构不一致;
        }

        std::vector<本次词面匹配节点> 当前树(1);
        std::set<稳定编码> 已读词条;
        bool 需重读词表 = false;
        for (const auto W编号 : 词组.词条节点组) {
            if (!有效(W编号) || !已读词条.insert(W编号).second) return S::结构不一致;
            const auto 词 = 词服务.获取词条(W编号);
            if (词.状态 == W::未找到) {
                if (尝试 != 0) continue;
                需重读词表 = true;
                break;
            }
            if (词.状态 == W::资源失败) return S::资源失败;
            if (词.状态 != W::已找到 || !词.信息 || 词.信息->语言节点 != 语言节点)
                return S::结构不一致;

            std::size_t 树位置 = 0;
            std::size_t 字节位置 = 0;
            while (字节位置 < 词.信息->规范化词面.size()) {
                const auto 码点 = 读取已校验码点(词.信息->规范化词面, 字节位置);
                const auto 已有 = 当前树[树位置].后继.find(码点);
                if (已有 != 当前树[树位置].后继.end()) {
                    树位置 = 已有->second;
                } else {
                    const auto 新位置 = 当前树.size();
                    当前树[树位置].后继.emplace(码点, 新位置);
                    当前树.emplace_back();
                    树位置 = 新位置;
                }
            }
            if (当前树[树位置].词条节点) return S::结构不一致;
            当前树[树位置].词条节点 = W编号;
        }
        if (需重读词表) continue;
        匹配树 = std::move(当前树);
        return S::已找到;
    }
    return S::未找到;
}
} // namespace

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
        if (!语言.信息) {
            return {新语言查询状态::结构不一致, {}};
        }
        if (文本.empty()) return {新语言查询状态::已找到, {}};

        std::vector<本次词面匹配节点> 匹配树;
        const auto 词面状态 = 读取本次词面匹配树(语言节点, 词服务, 匹配树);
        if (词面状态 != 新语言查询状态::已找到) return {词面状态, {}};

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

namespace {

using 知识状态 = 新语言知识状态;
enum class 知识标识 : std::size_t {
    知识类型, 来源类型, 目标类型, 角色类型, 约束类型, 含义类型, 参数类型, 结构类型, 项类型, 关联类型,
    核心目标, 核心结构, 来源种类, 来源语言, 来源原文,
    目标目的, 目标角色, 目标含义, 目标条件,
    角色编号, 角色种类, 角色数量, 角色约束, 约束角色, 约束类别,
    含义编号, 含义操作, 含义作用域, 含义参数, 含义分支, 参数名, 参数角色,
    结构类别, 结构语言, 结构项, 项编号, 项词, 项角色, 项方式,
    关联知识, 关联来源, 关联起始, 关联结束,
    含义子项, 含义确认范围, 项子知识, 项子映射, 项指代定义,
    缺口类型, 缺口语言, 缺口编号, 缺口标签, 缺口词面, 缺口版本,
    // 结构概念 / 表达模式 / 语法 / 对应表 / 量词（LANGUAGE-STRUCTURE-CONCEPT）
    结构概念类型, 结构概念宿主, 结构概念标识, 结构概念种类, 结构概念细目, 结构概念操作,
    结构概念来源, 结构概念版本, 结构概念角色编号, 结构概念角色种类,
    结构概念角色参数, 结构概念角色类别, 结构概念角色可选,
    表达模式类型, 表达模式语言, 表达模式结构概念, 表达模式标识, 表达模式版本,
    表达模式槽字母, 表达模式槽可选, 表达模式组成种类, 表达模式组成词面,
    表达模式组成槽, 表达模式映射槽, 表达模式映射角色, 表达模式语法编号,
    表达模式修饰槽,
    语法规则类型, 语法规则语言, 语法规则编号, 语法规则种类, 语法规则条件,
    语法规则结果, 语法规则版本, 语法规则来源,
    量词搭配类型, 量词搭配语言, 量词搭配词面, 量词搭配回退, 量词搭配版本,
    量词缺口类型, 量词缺口语言, 量词缺口词面, 量词缺口样例, 量词缺口版本,
    对应表类型, 对应表语言, 对应表结构概念, 对应表模式, 对应表种类,
    对应表细目, 对应表状态, 对应表来源, 对应表版本, 对应表语法,
    对应表概念标识, 对应表模式标识,
    结构概念布局格式, 结构概念角色节点, 结构含义角色类型,
    结构含义角色宿主, 结构含义角色编号, 结构含义角色种类,
    结构含义角色参数, 结构含义角色可选, 结构含义角色类别组,
    表达模式固定词条,
    结构定义集类型, 结构定义集语言, 结构定义集版本, 结构定义集来源,
    结构定义集概念, 结构定义集模式, 结构定义集语法, 结构定义集量词, 结构定义集缺口,
    结构缺口声明类型, 结构缺口声明语言, 结构缺口声明种类, 结构缺口声明细目, 结构缺口声明版本,
    结构对应视图类型, 结构对应视图语言, 结构对应视图来源集, 结构对应视图行,
    当前结构定义集, 当前结构对应视图,
    目标图格式,目标图根编号,目标图成员,图含义类型,图含义宿主,图含义编号,图含义操作,图含义作用域,
    图含义角色,图含义参数,图含义分支,图含义确认,图含义方向,图子边类型,图子边宿主,图子边角色,
    图子边用途,图子边目标编号,图子边映射,图映射类型,图映射宿主,图映射源含义,图映射源角色,
    图映射目标含义,图映射目标角色,图角色宿主,图参数宿主,图项源含义,图项角色,图项子知识,
    图项映射,模式实现知识,模式知识角色映射,模式知识映射类型,模式知识映射宿主,模式知识映射含义角色,
    模式知识映射图角色,对应表实现知识,
    图项指代概念,图项指代名称,图项概念前词,图项概念后词,图项查询词,
    模式知识映射含义编号,
    总数
};
constexpr auto 知识句柄数 = static_cast<std::size_t>(知识标识::总数);
std::array<稳定编码, 知识句柄数> 知识句柄{};
// 已原子建立而尚未完整核验的技术现场，不作为可用schema，不重复建立第二组。
std::array<稳定编码, 知识句柄数> 待核知识句柄{};
稳定编码 知识键(知识标识 标识) noexcept { return 知识句柄[static_cast<std::size_t>(标识)]; }

struct 语言知识候选快照 final {
    std::vector<新语言知识信息> 信息组;
    std::map<稳定编码, 基础节点结构> 结构组;
};
struct 语言知识词端点快照 final {
    新词条信息 信息;
    基础节点结构 结构;
};
using 语言知识指代端点快照 = std::map<稳定编码, 指代概念信息>;

bool 是作用域说话者定义(const 指代概念定义& 定义) {
    return 定义.目标范围 == 指代目标范围::单个已登记强类型信息节点
        && 定义.规则 == 指代声明规则::作用域说话者
        && 定义.参数组 == std::vector<指代参数定义>{
            {指代参数角色::目标, 指代参数类型::强类型信息节点},
            {指代参数角色::作用域, 指代参数类型::交互作用域}}
        && 定义.特征约束组.empty();
}
bool 是作用域受话者定义(const 指代概念定义& 定义) {
    return 定义.目标范围 == 指代目标范围::单个已登记强类型信息节点
        && 定义.规则 == 指代声明规则::作用域受话者
        && 定义.参数组 == std::vector<指代参数定义>{
            {指代参数角色::目标, 指代参数类型::强类型信息节点},
            {指代参数角色::作用域, 指代参数类型::交互作用域}}
        && 定义.特征约束组.empty();
}
bool 基础结构相等(const 基础节点结构& 左, const 基础节点结构& 右) noexcept {
    return 左.节点 == 右.节点 && 左.字段组 == 右.字段组;
}
bool 知识信息相等(const 新语言知识信息& 左, const 新语言知识信息& 右) {
    return 左.节点 == 右.节点 && 左.目标节点 == 右.目标节点 && 左.结构节点 == 右.结构节点
        && 左.定义 == 右.定义 && 左.来源组 == 右.来源组;
}
知识状态 基础知识状态(基础结构操作状态 状态) noexcept {
    switch (状态) {
    case 基础结构操作状态::已完成: return 知识状态::已完成;
    case 基础结构操作状态::节点不存在:
    case 基础结构操作状态::引用不存在: return 知识状态::引用不可读;
    case 基础结构操作状态::资源失败: return 知识状态::工程未完成;
    default: return 知识状态::内部不一致;
    }
}
bool 知识来源UTF8有效(const std::string& 原文, std::int64_t& 标量数) noexcept {
    标量数 = 0;
    if (原文.empty()) return false;
    for (std::size_t 位置 = 0; 位置 < 原文.size();) {
        const auto 首 = static_cast<unsigned char>(原文[位置]);
        std::uint32_t 值 = 0;
        std::size_t 长度 = 0;
        if (首 < 0x80) { 值 = 首; 长度 = 1; }
        else if (首 >= 0xC2 && 首 <= 0xDF) { 值 = 首 & 0x1F; 长度 = 2; }
        else if (首 >= 0xE0 && 首 <= 0xEF) { 值 = 首 & 0x0F; 长度 = 3; }
        else if (首 >= 0xF0 && 首 <= 0xF4) { 值 = 首 & 0x07; 长度 = 4; }
        else return false;
        if (原文.size() - 位置 < 长度) return false;
        for (std::size_t i = 1; i < 长度; ++i) {
            const auto 后续 = static_cast<unsigned char>(原文[位置 + i]);
            if ((后续 & 0xC0) != 0x80) return false;
            值 = (值 << 6) | (后续 & 0x3F);
        }
        if ((长度 == 2 && 值 < 0x80) || (长度 == 3 && 值 < 0x800)
            || (长度 == 4 && 值 < 0x10000) || (值 >= 0xD800 && 值 <= 0xDFFF)
            || 值 > 0x10FFFF || 标量数 == (std::numeric_limits<std::int64_t>::max)()) return false;
        ++标量数;
        位置 += 长度;
    }
    return true;
}

新语言知识含义图 取知识含义图(const 新语言知识定义& 定义) {
    if(定义.目标.含义图)return *定义.目标.含义图;
    新语言知识含义图 图{1,{}};
    if(定义.目标.含义组.size()!=1)return 图;
    const auto& 原=定义.目标.含义组.front();
    新语言图含义节点 根{1,原.操作,原.作用域,定义.目标.角色组,原.参数组,原.特征值分支,原.确认范围,{},{}};
    if(原.被询含义) {
        const auto& 子=*原.被询含义;
        新语言图含义节点 内{2,子.操作,子.作用域,定义.目标.角色组,子.参数组,子.特征值分支,{},{},{}};
        根.角色组.clear();根.参数组={{新语言知识参数角色::被询内容,0}};
        根.角色组.push_back({0,新语言知识角色种类::子含义引用,新语言知识角色数量::单项,
            {{0,{新语言知识角色类别::子含义}}}});
        根.子含义组.push_back({0,2,新语言子含义用途::被询内容,{}});
        图.节点组.push_back(std::move(内));
    }
    图.节点组.push_back(std::move(根));
    std::sort(图.节点组.begin(),图.节点组.end(),[](const auto& 左,const auto& 右){return 左.局部编号<右.局部编号;});
    return 图;
}
bool 规范含义图(新语言知识定义& 定义,std::string& 诊断) {
    const auto 拒绝=[&](const char* 原因){诊断=原因;return false;};
    if((定义.目标.目的!=新语言知识表达目的::陈述&&定义.目标.目的!=新语言知识表达目的::提问
            &&定义.目标.目的!=新语言知识表达目的::提出需求&&定义.目标.目的!=新语言知识表达目的::招呼)
        ||!定义.目标.含义图||!定义.目标.角色组.empty()||!定义.目标.含义组.empty()
        ||!定义.目标.适用条件组.empty()||定义.结构.类别!=新语言知识结构类别::基础表达
        ||!有效(定义.结构.语言节点)||定义.结构.有序项组.empty())return 拒绝("图与旧目标载荷互斥");
    auto& 图=*定义.目标.含义图;
    std::map<std::int64_t,新语言图含义节点*> 节点组;
    std::map<std::pair<std::int64_t,std::int64_t>,新语言知识角色定义*> 角色组;
    using 参数=新语言知识参数角色;
    for(auto& 节点:图.节点组) {
        if(节点.局部编号<0||!节点组.emplace(节点.局部编号,&节点).second
            ||节点.作用域<新语言知识作用域::普通陈述||节点.作用域>新语言知识作用域::需求目标范围)return 拒绝("图节点重复或作用域非法");
        std::sort(节点.角色组.begin(),节点.角色组.end(),[](const auto& 左,const auto& 右){return 左.局部编号<右.局部编号;});
        for(auto& 角色:节点.角色组) {
            if(角色.局部编号<0||!角色组.emplace(std::pair{节点.局部编号,角色.局部编号},&角色).second
                ||角色.数量!=新语言知识角色数量::单项||角色.必要约束组.size()!=1)return 拒绝("图角色不完整");
            auto& 约束=角色.必要约束组.front();
            if(约束.被约束角色!=角色.局部编号||约束.允许类别组.empty())return 拒绝("图角色必须自约束");
            std::sort(约束.允许类别组.begin(),约束.允许类别组.end());
            if(std::adjacent_find(约束.允许类别组.begin(),约束.允许类别组.end())!=约束.允许类别组.end())return 拒绝("图类别重复");
            for(auto 类别:约束.允许类别组)if(类别<新语言知识角色类别::存在||类别>新语言知识角色类别::查询存在)return 拒绝("图角色类别非法");
            const auto& 类别=约束.允许类别组;
            const bool 相容=角色.种类==新语言知识角色种类::领域对象引用 ? (类别==std::vector{新语言知识角色类别::存在}||类别==std::vector{新语言知识角色类别::正式信息节点})
                :角色.种类==新语言知识角色种类::特征类型引用 ? 类别.size()==1&&(类别[0]==新语言知识角色类别::I64标量特征类型||类别[0]==新语言知识角色类别::正式特征类型)
                :角色.种类==新语言知识角色种类::准确值 ? 类别.size()==1&&(类别[0]==新语言知识角色类别::十进制整数||类别[0]==新语言知识角色类别::I64标量)
                :角色.种类==新语言知识角色种类::子含义引用 ? 类别==std::vector{新语言知识角色类别::子含义}
                :角色.种类==新语言知识角色种类::查询变量 ? (类别==std::vector{新语言知识角色类别::查询特征值}||类别==std::vector{新语言知识角色类别::查询存在})
                :角色.种类==新语言知识角色种类::概念引用 ? std::all_of(类别.begin(),类别.end(),[](auto 类){return 类==新语言知识角色类别::存在概念||类==新语言知识角色类别::特征概念;}) : false;
            if(!相容)return 拒绝("图角色种类与类别冲突");
        }
        std::map<参数,std::int64_t> 参数组;std::set<std::int64_t> 参数角色组;
        for(auto 参数项:节点.参数组)if(!角色组.contains({节点.局部编号,参数项.规则角色})
            ||!参数组.emplace(参数项.参数,参数项.规则角色).second||!参数角色组.insert(参数项.规则角色).second)return 拒绝("图参数引用非法");
        if(参数角色组.size()!=节点.角色组.size())return 拒绝("图含未使用的角色");
        std::set<参数> 必须;
        switch(节点.操作) {
        case 新语言知识含义操作::行动请求:必须={参数::受请求者,参数::目标内容};break;
        // 最小需求句式“Y要X”与目标栏问题：需求描述直接带目标存在，不带目标子含义（R120/R121用户裁决2026-11-12）。
        case 新语言知识含义操作::需求描述:必须=参数组.contains(参数::目标存在) ? std::set<参数>{参数::主体,参数::目标存在} : std::set<参数>{参数::主体,参数::目标内容};break;
        case 新语言知识含义操作::信息请求:必须={参数::被询内容};if(节点.确认范围!=新语言知识确认范围::主张成立与否&&节点.确认范围!=新语言知识确认范围::填项内容)return 拒绝("图询问范围未支持");break;
        case 新语言知识含义操作::概念归类:必须={参数::主体,参数::归类概念};break;
        case 新语言知识含义操作::组成描述:必须={参数::整体,参数::部分};break;
        case 新语言知识含义操作::组织隶属:必须={参数::上级,参数::下级};break;
        case 新语言知识含义操作::特征子类型定义:
            必须 = {参数::主体, 参数::特征类型, 参数::适用对象};
            if (参数组.contains(参数::下界) || 参数组.contains(参数::上界))
                必须.insert({参数::下界, 参数::上界});
            break;
        case 新语言知识含义操作::特征描述:
            必须={参数::主体,参数::特征类型};
            if (节点.作用域 != 新语言知识作用域::需求目标范围 || 节点.特征值分支)
                必须.insert(参数::特征值);
            if (节点.特征值分支 && 节点.特征值分支 != 新语言知识特征值分支::准确值
                && 节点.特征值分支 != 新语言知识特征值分支::概念条件) return 拒绝("特征图值分支非法");
            break;
        case 新语言知识含义操作::特征区间描述:必须={参数::主体,参数::特征类型,参数::下界,参数::上界};break;
        case 新语言知识含义操作::特征方向描述:必须={参数::主体,参数::特征类型};if(!节点.方向||(*节点.方向!=新语言知识特征方向::增加&&*节点.方向!=新语言知识特征方向::减少))return 拒绝("方向图非法");break;
        case 新语言知识含义操作::招呼发起:case 新语言知识含义操作::招呼维持:case 新语言知识含义操作::招呼结束:break; // 零角色、零参数。
        default:return 拒绝("本图含义操作未支持");
        }
        if(必须.size()!=参数组.size())return 拒绝("图参数数量非法");
        for(auto 参数名:必须)if(!参数组.contains(参数名))return 拒绝("图参数签名不完整");
        const auto 参数种类=[&](参数 参数名,新语言知识角色种类 种类) {
            return 参数组.contains(参数名)&&角色组.at({节点.局部编号,参数组.at(参数名)})->种类==种类;
        };
        const bool 归类陈述 = 节点.操作 == 新语言知识含义操作::概念归类;
        const bool 特征概念陈述 = 节点.操作 == 新语言知识含义操作::特征描述
            && 节点.特征值分支 == 新语言知识特征值分支::概念条件;
        if (归类陈述 || 特征概念陈述) {
            const auto 参数是 = [&](参数 名, 新语言知识角色种类 种类, 新语言知识角色类别 类别) {
                return 参数种类(名, 种类)
                    && 角色组.at({节点.局部编号, 参数组.at(名)})->必要约束组.front().允许类别组 == std::vector{类别};
            };
            using 种类 = 新语言知识角色种类;
            using 类别 = 新语言知识角色类别;
            if (定义.目标.目的 != 新语言知识表达目的::陈述 || 图.节点组.size() != 1
                || 节点.作用域 != 新语言知识作用域::普通陈述 || !节点.子含义组.empty()
                || !参数是(参数::主体, 种类::领域对象引用, 类别::存在)
                || (归类陈述 && !参数是(参数::归类概念, 种类::概念引用, 类别::存在概念))
                || (特征概念陈述 && (!参数是(参数::特征类型, 种类::特征类型引用, 类别::正式特征类型)
                    || !参数是(参数::特征值, 种类::概念引用, 类别::特征概念))))
                return 拒绝("静态陈述图签名非法");
        } else if (节点.操作 == 新语言知识含义操作::特征子类型定义) {
            using K = 新语言知识角色种类;
            using C = 新语言知识角色类别;
            const auto 参数是 = [&](参数 名, K 种类, C 类别) {
                return 参数种类(名, 种类)
                    && 角色组.at({节点.局部编号, 参数组.at(名)})->必要约束组.front().允许类别组
                        == std::vector{类别};
            };
            if (定义.目标.目的 != 新语言知识表达目的::陈述 || 图.节点组.size() != 1
                || 节点.作用域 != 新语言知识作用域::普通陈述 || !节点.子含义组.empty()
                || !参数是(参数::主体, K::概念引用, C::特征概念)
                || !参数是(参数::特征类型, K::特征类型引用, C::I64标量特征类型)
                || !参数是(参数::适用对象, K::概念引用, C::存在概念)
                || (参数组.contains(参数::下界)
                    && (!参数是(参数::下界, K::准确值, C::十进制整数)
                        || !参数是(参数::上界, K::准确值, C::十进制整数))))
                return 拒绝("特征子类型定义图签名非法");
        } else if (节点.操作 == 新语言知识含义操作::组成描述
            || 节点.操作 == 新语言知识含义操作::组织隶属) {
            const auto 左 = 节点.操作 == 新语言知识含义操作::组成描述 ? 参数::整体 : 参数::上级;
            const auto 右 = 节点.操作 == 新语言知识含义操作::组成描述 ? 参数::部分 : 参数::下级;
            if (定义.目标.目的 != 新语言知识表达目的::陈述 || 图.节点组.size() != 1
                || 节点.作用域 != 新语言知识作用域::普通陈述 || !节点.子含义组.empty()
                || !参数种类(左, 新语言知识角色种类::领域对象引用)
                || !参数种类(右, 新语言知识角色种类::领域对象引用)
                || 角色组.at({节点.局部编号, 参数组.at(左)})->必要约束组.front().允许类别组
                    != std::vector{新语言知识角色类别::存在}
                || 角色组.at({节点.局部编号, 参数组.at(右)})->必要约束组.front().允许类别组
                    != std::vector{新语言知识角色类别::存在})
                return 拒绝("组成/组织隶属图签名非法");
        } else if (节点.操作 == 新语言知识含义操作::行动请求) {
            if(节点.作用域!=新语言知识作用域::请求范围
                ||!参数种类(参数::受请求者,新语言知识角色种类::领域对象引用)
                ||角色组.at({节点.局部编号,参数组.at(参数::受请求者)})->必要约束组.front().允许类别组!=std::vector{新语言知识角色类别::存在}
                ||!参数种类(参数::目标内容,新语言知识角色种类::子含义引用)
                ||节点.子含义组.size()!=1||节点.子含义组[0].本节点角色编号!=参数组.at(参数::目标内容)
                ||节点.子含义组[0].用途!=新语言子含义用途::期望目标)return 拒绝("行动请求签名或期望目标边非法");
        }else if(节点.操作==新语言知识含义操作::需求描述&&参数组.contains(参数::目标存在)) {
            const auto& 目标=*角色组.at({节点.局部编号,参数组.at(参数::目标存在)});
            const bool 陈述目标=节点.作用域==新语言知识作用域::普通陈述&&目标.种类==新语言知识角色种类::领域对象引用
                &&目标.必要约束组.front().允许类别组==std::vector{新语言知识角色类别::存在};
            const bool 待求目标=节点.作用域==新语言知识作用域::提问范围&&目标.种类==新语言知识角色种类::查询变量
                &&目标.必要约束组.front().允许类别组==std::vector{新语言知识角色类别::查询存在};
            if(!参数种类(参数::主体,新语言知识角色种类::领域对象引用)
                ||角色组.at({节点.局部编号,参数组.at(参数::主体)})->必要约束组.front().允许类别组!=std::vector{新语言知识角色类别::存在}
                ||!(陈述目标||待求目标)||!节点.子含义组.empty())return 拒绝("目标存在需求图参数类型非法");
        }else if(节点.操作==新语言知识含义操作::需求描述) {
            if(!参数种类(参数::主体,新语言知识角色种类::领域对象引用)
                ||!参数种类(参数::目标内容,新语言知识角色种类::子含义引用)
                ||节点.子含义组.size()!=1||节点.子含义组[0].本节点角色编号!=参数组.at(参数::目标内容)
                ||节点.子含义组[0].用途!=新语言子含义用途::需求目标)return 拒绝("需求图参数类型或目标边非法");
        }else if(节点.操作==新语言知识含义操作::信息请求) {
            if(!参数种类(参数::被询内容,新语言知识角色种类::子含义引用)
                ||节点.子含义组.size()!=1||节点.子含义组[0].本节点角色编号!=参数组.at(参数::被询内容)
                ||节点.子含义组[0].用途!=新语言子含义用途::被询内容)return 拒绝("确认图参数类型或被询边非法");
        }else if(新语言含义操作是招呼(节点.操作)) {
            if(节点.作用域!=新语言知识作用域::普通陈述||!节点.角色组.empty()||!节点.子含义组.empty())return 拒绝("招呼图必须零角色且无子含义");
        }else {
            if(!参数种类(参数::主体,新语言知识角色种类::领域对象引用)
                ||!参数种类(参数::特征类型,新语言知识角色种类::特征类型引用)||!节点.子含义组.empty())return 拒绝("特征图主体或类型非法");
            const auto& 类型=*角色组.at({节点.局部编号,参数组.at(参数::特征类型)});
            const bool 待求=节点.操作==新语言知识含义操作::特征描述&&!节点.特征值分支
                &&参数种类(参数::特征值,新语言知识角色种类::查询变量);
            if(待求) {
                if(节点.作用域!=新语言知识作用域::提问范围
                    ||类型.必要约束组.front().允许类别组!=std::vector{新语言知识角色类别::正式特征类型}
                    ||角色组.at({节点.局部编号,参数组.at(参数::主体)})->必要约束组.front().允许类别组!=std::vector{新语言知识角色类别::正式信息节点})return 拒绝("待求必须位于正式特征填项问中");
            }else if(类型.必要约束组.front().允许类别组!=std::vector{新语言知识角色类别::I64标量特征类型})return 拒绝("H必须使用I64特征类型");
            for(auto 参数名:{参数::特征值,参数::下界,参数::上界})if(参数组.contains(参数名)
                &&!(待求&&参数名==参数::特征值)&&!参数种类(参数名,新语言知识角色种类::准确值))return 拒绝("H值参数必须是整数");
        }
        if((节点.方向.has_value())!=(节点.操作==新语言知识含义操作::特征方向描述)
            ||(节点.特征值分支.has_value())!=(节点.操作==新语言知识含义操作::特征描述 && 参数组.contains(参数::特征值)
                && !参数种类(参数::特征值,新语言知识角色种类::查询变量))
            ||(节点.确认范围.has_value())!=(节点.操作==新语言知识含义操作::信息请求))return 拒绝("图含义分支混用");
        std::sort(节点.参数组.begin(),节点.参数组.end(),[](auto 左,auto 右){return 左.参数<右.参数;});
    }
    if(!节点组.contains(图.根含义编号))return 拒绝("图根缺失");
    std::map<std::int64_t,int> 颜色;
    const auto 访问=[&](auto&& 自身,std::int64_t 编号)->bool {
        if(颜色[编号]==1)return false;if(颜色[编号]==2)return true;颜色[编号]=1;
        auto& 节点=*节点组.at(编号);std::set<std::int64_t> 边角色;
        for(const auto& 边:节点.子含义组) {
            if(!节点组.contains(边.子含义编号)||!边角色.insert(边.本节点角色编号).second
                ||!角色组.contains({编号,边.本节点角色编号})
                ||角色组.at({编号,边.本节点角色编号})->种类!=新语言知识角色种类::子含义引用)return false;
            const auto& 子=*节点组.at(边.子含义编号);
            if(边.用途==新语言子含义用途::需求目标) {
                if(节点.操作!=新语言知识含义操作::需求描述||子.作用域!=新语言知识作用域::需求目标范围)return false;
                if(子.操作!=新语言知识含义操作::特征描述&&子.操作!=新语言知识含义操作::特征区间描述&&子.操作!=新语言知识含义操作::特征方向描述)return false;
            }else if(边.用途==新语言子含义用途::期望目标) {
                if(节点.操作!=新语言知识含义操作::行动请求||子.操作!=新语言知识含义操作::需求描述
                    ||子.作用域!=新语言知识作用域::需求目标范围)return false;
            }else if(边.用途==新语言子含义用途::被询内容) {
                if(节点.操作!=新语言知识含义操作::信息请求||子.作用域!=新语言知识作用域::提问范围)return false;
                if(节点.确认范围==新语言知识确认范围::填项内容&&子.操作==新语言知识含义操作::需求描述) {
                    // 目标栏问题“Y需要什么？”：被询需求描述的目标存在为待求。
                    const auto 目标角色=std::find_if(子.参数组.begin(),子.参数组.end(),[](auto 项){return 项.参数==参数::目标存在;});
                    if(目标角色==子.参数组.end()||角色组.at({子.局部编号,目标角色->规则角色})->种类!=新语言知识角色种类::查询变量)return false;
                }else if(节点.确认范围==新语言知识确认范围::填项内容) {
                    if(子.操作!=新语言知识含义操作::特征描述||子.特征值分支)return false;
                    const auto 值角色=std::find_if(子.参数组.begin(),子.参数组.end(),[](auto 项){return 项.参数==参数::特征值;});
                    if(值角色==子.参数组.end()||角色组.at({子.局部编号,值角色->规则角色})->种类!=新语言知识角色种类::查询变量)return false;
                }
            }else return false;
            std::set<std::pair<std::int64_t,std::int64_t>> 目标组;
            for(const auto& 映射:边.显式映射组) {
                const auto 源键=std::pair{映射.来源.含义编号,映射.来源.角色编号},目标键=std::pair{映射.目标.含义编号,映射.目标.角色编号};
                if(!角色组.contains(源键)||!角色组.contains(目标键)||!目标组.insert(目标键).second
                    ||角色组.at(源键)->必要约束组.front().允许类别组!=角色组.at(目标键)->必要约束组.front().允许类别组)return false;
            }
            if(!自身(自身,边.子含义编号))return false;
        }
        for(const auto& 角色:节点.角色组)if((角色.种类==新语言知识角色种类::子含义引用)!=边角色.contains(角色.局部编号))return false;
        颜色[编号]=2;return true;
    };
    if(!访问(访问,图.根含义编号)||颜色.size()!=节点组.size())return 拒绝("图环、悬空或孤立节点");
    const auto& 根=*节点组.at(图.根含义编号);
    const auto 待求数=std::count_if(角色组.begin(),角色组.end(),[](const auto& 项){return 项.second->种类==新语言知识角色种类::查询变量;});
    if(待求数&&(待求数!=1||根.操作!=新语言知识含义操作::信息请求||根.确认范围!=新语言知识确认范围::填项内容
        ||根.子含义组.size()!=1))return 拒绝("查询变量只允许唯一填项被询内容");
    if((定义.目标.目的==新语言知识表达目的::提问)!=(根.操作==新语言知识含义操作::信息请求)
        ||(定义.目标.目的==新语言知识表达目的::提出需求)!=(根.操作==新语言知识含义操作::行动请求)
        ||(定义.目标.目的==新语言知识表达目的::招呼)!=新语言含义操作是招呼(根.操作)
        ||(定义.目标.目的==新语言知识表达目的::招呼&&(图.节点组.size()!=1||根.作用域!=新语言知识作用域::普通陈述)))return 拒绝("图目的与根操作冲突");
    for(std::size_t 位置=0;位置<定义.结构.有序项组.size();++位置) {
        auto& 项=定义.结构.有序项组[位置];项.局部编号=static_cast<std::int64_t>(位置);
        if(const auto* 词=std::get_if<新语言知识固定词项>(&项.载荷)){if(!有效(词->词条节点))return 拒绝("图固定词无身份");}
        else if(const auto* 表达=std::get_if<新语言知识图角色表达项>(&项.载荷)) {
            const auto 键=std::pair{表达->角色.含义编号,表达->角色.角色编号};
            if(!角色组.contains(键)||角色组.at(键)->种类==新语言知识角色种类::子含义引用
                ||表达->方式<新语言知识表达方式::名称表达||表达->方式>新语言知识表达方式::查询变量词面)return 拒绝("图表达项引用非法");
            const auto& 角色=*角色组.at(键);
            if(表达->方式==新语言知识表达方式::带概念指代) {
                if(角色.种类!=新语言知识角色种类::领域对象引用||角色.必要约束组.front().允许类别组!=std::vector{新语言知识角色类别::正式信息节点}
                    ||!表达->指代概念节点||!表达->指代名称关系节点||!表达->概念前词条节点||!表达->概念后词条节点||表达->查询词条节点)return 拒绝("带概念指代项不完整");
            }else if(表达->方式==新语言知识表达方式::已消解指代) {
                if(角色.种类!=新语言知识角色种类::领域对象引用
                    ||角色.必要约束组.front().允许类别组!=std::vector{新语言知识角色类别::存在}
                    ||!表达->指代概念节点||!表达->指代名称关系节点
                    ||表达->概念前词条节点||表达->概念后词条节点||表达->查询词条节点)return 拒绝("已消解指代图项不完整");
                const auto& 所在=*节点组.at(表达->角色.含义编号);
                const bool 受请求者=表达->角色.含义编号==图.根含义编号&&根.操作==新语言知识含义操作::行动请求
                    &&std::any_of(根.参数组.begin(),根.参数组.end(),[&](const auto& p){return p.参数==参数::受请求者&&p.规则角色==表达->角色.角色编号;});
                // 需求主体可用作用域中的说话者或受话者表达，包括通过子含义描述目标的陈述。
                const bool 需求主体 = 所在.操作 == 新语言知识含义操作::需求描述
                    && std::any_of(所在.参数组.begin(), 所在.参数组.end(), [&](const auto& p) {
                        return p.参数 == 参数::主体 && p.规则角色 == 表达->角色.角色编号;
                    })
                    && (所在.作用域 == 新语言知识作用域::普通陈述
                        || std::any_of(所在.参数组.begin(), 所在.参数组.end(), [](const auto& p) {
                            return p.参数 == 参数::目标存在;
                        }));
                if (!受请求者 && !需求主体) return 拒绝("已消解图指代需要行动请求的受请求者或需求描述的主体");
            }else if(表达->方式==新语言知识表达方式::查询变量词面) {
                if(角色.种类!=新语言知识角色种类::查询变量||!表达->查询词条节点||表达->指代概念节点||表达->指代名称关系节点||表达->概念前词条节点||表达->概念后词条节点)return 拒绝("查询词面项不完整");
            }else if(表达->指代概念节点||表达->指代名称关系节点||表达->概念前词条节点||表达->概念后词条节点||表达->查询词条节点)return 拒绝("普通图项混入查询表达材料");
            for(auto 端点:{表达->指代概念节点,表达->指代名称关系节点,表达->概念前词条节点,表达->概念后词条节点,表达->查询词条节点})if(端点&&!有效(*端点))return 拒绝("图项端点无效");
        }else if(const auto* 子=std::get_if<新语言知识图子规则项>(&项.载荷)) {
            if(!节点组.contains(子->子角色.含义编号)||子->允许子知识组.empty())return 拒绝("图子项没有知识");
            const auto& 边组=节点组.at(子->子角色.含义编号)->子含义组;
            if(std::none_of(边组.begin(),边组.end(),[&](const auto& 边){return 边.本节点角色编号==子->子角色.角色编号;}))return 拒绝("图子项无对应边");
            std::set<稳定编码> 知识组;
            for(auto 知识:子->允许子知识组)if(!有效(知识)||!知识组.insert(知识).second)return 拒绝("图子知识引用非法");
        }else return 拒绝("图项与旧项互斥");
    }
    std::sort(图.节点组.begin(),图.节点组.end(),[](const auto& 左,const auto& 右){return 左.局部编号<右.局部编号;});
    return true;
}
bool 图子规则相容(const 新语言知识定义& 外层,const 新语言知识图子规则项& 项,const 新语言知识定义& 子定义) {
    const auto 外图=取知识含义图(外层),子图=取知识含义图(子定义);
    std::map<std::int64_t,const 新语言图含义节点*> 外节点,子节点;
    for(const auto& 节点:外图.节点组)外节点.emplace(节点.局部编号,&节点);
    for(const auto& 节点:子图.节点组)子节点.emplace(节点.局部编号,&节点);
    if(!外节点.contains(项.子角色.含义编号)||!子节点.contains(子图.根含义编号))return false;
    const auto& 边组=外节点.at(项.子角色.含义编号)->子含义组;
    const auto 边=std::find_if(边组.begin(),边组.end(),[&](const auto& 值){return 值.本节点角色编号==项.子角色.角色编号;});
    if(边==边组.end()||!外节点.contains(边->子含义编号))return false;
    const auto& 外根=*外节点.at(边->子含义编号);const auto& 子根=*子节点.at(子图.根含义编号);
    if(外根.操作!=子根.操作||外根.特征值分支!=子根.特征值分支||外根.方向!=子根.方向
        ||外层.结构.语言节点!=子定义.结构.语言节点)return false;
    using 键=std::pair<std::int64_t,std::int64_t>;
    std::map<键,const 新语言知识角色定义*> 源角色,目标角色;
    const auto 加角色=[&](auto&& 自身,std::int64_t 编号,const auto& 节点表,auto& 角色表)->void {
        const auto& 节点=*节点表.at(编号);
        for(const auto& 角色:节点.角色组)if(角色.种类!=新语言知识角色种类::子含义引用)角色表.emplace(键{编号,角色.局部编号},&角色);
        for(const auto& 子边:节点.子含义组)自身(自身,子边.子含义编号,节点表,角色表);
    };
    加角色(加角色,外根.局部编号,外节点,源角色);加角色(加角色,子根.局部编号,子节点,目标角色);
    const auto 类别归一=[](auto 类别组){for(auto& 类别:类别组)if(类别==新语言知识角色类别::十进制整数)类别=新语言知识角色类别::I64标量;return 类别组;};
    std::set<键> 已映射;std::map<键,键> 反向映射;
    for(const auto& 映射:项.映射组) {
        const 键 源{映射.来源.含义编号,映射.来源.角色编号},目标{映射.目标.含义编号,映射.目标.角色编号};
        if(!源角色.contains(源)||!目标角色.contains(目标)||!已映射.insert(目标).second
            ||源角色.at(源)->种类!=目标角色.at(目标)->种类
            ||类别归一(源角色.at(源)->必要约束组.front().允许类别组)!=类别归一(目标角色.at(目标)->必要约束组.front().允许类别组))return false;
        反向映射.emplace(目标,源);
    }
    if(已映射.size()!=目标角色.size())return false;
    std::set<std::pair<std::int64_t,std::int64_t>> 已比较;
    const auto 比较子图=[&](auto&& 自身,std::int64_t 源编号,std::int64_t 目标编号)->bool {
        if(!已比较.emplace(源编号,目标编号).second)return true;
        const auto& 源=*外节点.at(源编号);const auto& 目标=*子节点.at(目标编号);
        const bool 域相容=源.作用域==目标.作用域||(目标.作用域==新语言知识作用域::普通陈述
            &&(源.作用域==新语言知识作用域::需求目标范围||源.作用域==新语言知识作用域::提问范围));
        if(源.操作!=目标.操作||源.特征值分支!=目标.特征值分支||源.方向!=目标.方向
            ||源.确认范围!=目标.确认范围||!域相容||源.参数组.size()!=目标.参数组.size()
            ||源.角色组.size()!=目标.角色组.size()||源.子含义组.size()!=目标.子含义组.size())return false;
        for(const auto& 目标参数:目标.参数组) {
            const auto 源参数=std::find_if(源.参数组.begin(),源.参数组.end(),[&](auto 参数){return 参数.参数==目标参数.参数;});
            if(源参数==源.参数组.end())return false;
            const auto 目标边=std::find_if(目标.子含义组.begin(),目标.子含义组.end(),[&](const auto& 边){return 边.本节点角色编号==目标参数.规则角色;});
            if(目标边!=目标.子含义组.end()) {
                const auto 源边=std::find_if(源.子含义组.begin(),源.子含义组.end(),[&](const auto& 边){return 边.本节点角色编号==源参数->规则角色;});
                if(源边==源.子含义组.end()||源边->用途!=目标边->用途||!自身(自身,源边->子含义编号,目标边->子含义编号))return false;
            }else {
                const auto 映射=反向映射.find({目标编号,目标参数.规则角色});
                if(映射==反向映射.end()||映射->second!=键{源编号,源参数->规则角色})return false;
            }
        }
        return true;
    };
    return 比较子图(比较子图,外根.局部编号,子根.局部编号);
}
知识状态 规范化知识定义(const 新语言知识定义& 输入, 新语言知识定义& 定稿, std::string& 诊断) {
    if(输入.目标.含义图){定稿=输入;return 规范含义图(定稿,诊断)?知识状态::已完成:知识状态::输入拒绝;}
    const auto 拒绝 = [&](const char* 原因, 知识状态 状态 = 知识状态::输入拒绝) {
        诊断 = 原因; return 状态;
    };
    using 种类 = 新语言知识角色种类;
    using 类别 = 新语言知识角色类别;
    using 参数 = 新语言知识参数角色;
    if (输入.目标.目的 != 新语言知识表达目的::陈述
            && 输入.目标.目的 != 新语言知识表达目的::提问
            && 输入.目标.目的 != 新语言知识表达目的::提出需求)
        return 拒绝("首叶不支持该目的、结构或附加条件", 知识状态::未支持);
    if ((输入.结构.类别 != 新语言知识结构类别::基础表达
            && 输入.结构.类别 != 新语言知识结构类别::缺口声明)
        || !输入.目标.适用条件组.empty())
        return 拒绝("首叶不支持该目的、结构或附加条件", 知识状态::未支持);
    if (!有效(输入.结构.语言节点) || 输入.目标.含义组.size() != 1
        || 输入.结构.有序项组.empty()) return 拒绝("目标或结构形状不完整");
    if (输入.结构.类别 == 新语言知识结构类别::缺口声明) {
        if (输入.目标.目的 != 新语言知识表达目的::陈述
            || !输入.目标.角色组.empty()) return 拒绝("缺口声明不携带角色");
        定稿 = 输入;
        auto& 含义 = 定稿.目标.含义组.front();
        if (含义.局部编号 != 0 || 含义.操作 != 新语言知识含义操作::缺口登记
            || 含义.作用域 != 新语言知识作用域::普通陈述
            || !含义.参数组.empty() || 含义.特征值分支 || 含义.被询含义 || 含义.确认范围)
            return 拒绝("缺口登记含义不合法");
        for (std::size_t i = 0; i < 定稿.结构.有序项组.size(); ++i) {
            auto& 项 = 定稿.结构.有序项组[i];
            if (项.局部编号 < 0 || !std::holds_alternative<新语言知识固定词项>(项.载荷)
                || !有效(std::get<新语言知识固定词项>(项.载荷).词条节点))
                return 拒绝("缺口声明只接受正式固定词提示");
            项.局部编号 = static_cast<std::int64_t>(i);
        }
        return 知识状态::已完成;
    }
    if (输入.目标.角色组.empty()) return 拒绝("目标或结构形状不完整");
    定稿 = 输入;
    std::map<std::int64_t, std::size_t> 角色位置;
    for (std::size_t i = 0; i < 定稿.目标.角色组.size(); ++i) {
        auto& 角色 = 定稿.目标.角色组[i];
        if (角色.局部编号 < 0 || !角色位置.emplace(角色.局部编号, i).second
            || 角色.数量 != 新语言知识角色数量::单项 || 角色.必要约束组.size() != 1)
            return 拒绝("角色编号、数量或约束基数不合法");
        auto& 约束 = 角色.必要约束组.front();
        if (约束.被约束角色 != 角色.局部编号 || 约束.允许类别组.empty()) return 拒绝("角色必须受自己的非空类别约束");
        for (const auto 值 : 约束.允许类别组)
            if (值 < 类别::存在 || 值 > 类别::查询特征值) return 拒绝("角色类别枚举不合法");
        std::sort(约束.允许类别组.begin(), 约束.允许类别组.end());
        约束.允许类别组.erase(std::unique(约束.允许类别组.begin(), 约束.允许类别组.end()), 约束.允许类别组.end());
        const auto& 类别组 = 约束.允许类别组;
        bool 合法 = false;
        switch (角色.种类) {
        case 种类::领域对象引用: 合法 = 类别组 == std::vector{类别::存在}; break;
        case 种类::概念引用: 合法 = std::all_of(类别组.begin(), 类别组.end(), [](auto 值) {
            return 值 == 类别::存在概念 || 值 == 类别::特征概念; }); break;
        case 种类::特征类型引用: 合法 = 类别组.size() == 1
            && (类别组.front() == 类别::正式特征类型 || 类别组.front() == 类别::I64标量特征类型); break;
        case 种类::准确值: 合法 = 类别组 == std::vector{类别::I64标量}; break;
        case 种类::查询变量: 合法 = 类别组 == std::vector{类别::查询特征值}; break;
        default: return 拒绝("角色种类枚举不合法");
        }
        if (!合法) return 拒绝("角色种类与允许类别不相容");
    }
    if (定稿.目标.目的 == 新语言知识表达目的::提问) {
        auto& 外层 = 定稿.目标.含义组.front();
        if (外层.局部编号 != 0 || 外层.操作 != 新语言知识含义操作::信息请求
            || 外层.作用域 != 新语言知识作用域::提问范围
            || 外层.特征值分支 || !外层.被询含义 || !外层.确认范围)
            return 拒绝("提问外层含义不合法");
        auto& 子 = *外层.被询含义;
        if (子.局部编号 != 1 || 子.作用域 != 新语言知识作用域::提问范围)
            return 拒绝("提问子含义作用域不合法");
        std::sort(子.参数组.begin(), 子.参数组.end(),
            [](const auto& 左, const auto& 右) { return 左.参数 < 右.参数; });

        const bool 是S09 = *外层.确认范围 == 新语言知识确认范围::主张成立与否
            && 子.操作 == 新语言知识含义操作::概念归类 && !子.特征值分支
            && 外层.参数组.empty() && 定稿.目标.角色组.size() == 2
            && 定稿.结构.有序项组.size() == 2
            && 角色位置.contains(0) && 角色位置.contains(1);
        const bool 是需求确认 = *外层.确认范围 == 新语言知识确认范围::主张成立与否
            && 子.操作 == 新语言知识含义操作::需求描述 && !子.特征值分支
            && 外层.参数组.empty() && 定稿.结构.有序项组.size() == 2;
        const bool 是S10 = *外层.确认范围 == 新语言知识确认范围::填项内容
            && 子.操作 == 新语言知识含义操作::特征描述 && 子.特征值分支
            && 外层.参数组.empty() && 子.参数组.size() == 3
            && 定稿.结构.有序项组.size() == 5;
        // 待审：需求侧信息请求（SC-需求-信息请求）与 S13 一并实现；共用信息请求外层校验。
        const bool 是S13 = *外层.确认范围 == 新语言知识确认范围::填项内容
            && 外层.参数组.size() == 2 && 子.参数组.size() >= 2
            && 定稿.结构.有序项组.size() >= 5;

        if (是S09) {
            for (const auto& 角色 : 定稿.目标.角色组)
                if (角色.种类 != 种类::概念引用
                    || 角色.必要约束组.front().允许类别组 != std::vector{类别::存在概念, 类别::特征概念})
                    return 拒绝("确认询问角色类别集合不合法");
            if (子.参数组.size() != 2
                || 子.参数组[0] != 新语言知识角色参数{参数::主体, 0}
                || 子.参数组[1] != 新语言知识角色参数{参数::归类概念, 1})
                return 拒绝("确认询问子含义角色方向不合法");
            auto& 第一 = 定稿.结构.有序项组[0];
            auto& 第二 = 定稿.结构.有序项组[1];
            if (第一.局部编号 < 0 || 第二.局部编号 < 0 || 第一.局部编号 == 第二.局部编号
                || !std::holds_alternative<新语言知识子规则项>(第一.载荷)
                || !std::holds_alternative<新语言知识固定词项>(第二.载荷))
                return 拒绝("确认询问结构必须为子规则后接固定词");
            第一.局部编号 = 0;
            第二.局部编号 = 1;
            auto& 子规则 = std::get<新语言知识子规则项>(第一.载荷);
            if (!有效(子规则.子知识节点) || 子规则.映射组.size() != 2)
                return 拒绝("子规则身份或映射基数不合法");
            std::sort(子规则.映射组.begin(), 子规则.映射组.end(),
                [](const auto& 左, const auto& 右) { return 左.外层角色编号 < 右.外层角色编号; });
            if (子规则.映射组[0] != 新语言知识子规则映射{0, 0}
                || 子规则.映射组[1] != 新语言知识子规则映射{1, 1}
                || !有效(std::get<新语言知识固定词项>(第二.载荷).词条节点))
                return 拒绝("子规则角色映射或固定词不合法");
            std::sort(定稿.目标.角色组.begin(), 定稿.目标.角色组.end(),
                [](const auto& 左, const auto& 右) { return 左.局部编号 < 右.局部编号; });
            return 知识状态::已完成;
        }

        if (是需求确认) {
            auto& 第一 = 定稿.结构.有序项组[0];
            auto& 第二 = 定稿.结构.有序项组[1];
            if (!std::holds_alternative<新语言知识子规则项>(第一.载荷)
                || !std::holds_alternative<新语言知识固定词项>(第二.载荷)
                || !有效(std::get<新语言知识固定词项>(第二.载荷).词条节点))
                return 拒绝("需求确认结构必须为子规则后接吗");
            第一.局部编号 = 0;
            第二.局部编号 = 1;
            auto& 子规则 = std::get<新语言知识子规则项>(第一.载荷);
            if (!有效(子规则.子知识节点) || 子规则.映射组.empty())
                return 拒绝("需求确认子规则不合法");
            std::sort(定稿.目标.角色组.begin(), 定稿.目标.角色组.end(),
                [](const auto& 左, const auto& 右) { return 左.局部编号 < 右.局部编号; });
            return 知识状态::已完成;
        }

        if (是S10) {
            if (子.参数组[0].参数 != 参数::主体 || 子.参数组[1].参数 != 参数::特征类型
                || 子.参数组[2].参数 != 参数::特征值)
                return 拒绝("S10子参数签名不合法");
            const auto& 主 = 定稿.目标.角色组[角色位置.at(子.参数组[0].规则角色)];
            const auto& 类 = 定稿.目标.角色组[角色位置.at(子.参数组[1].规则角色)];
            const auto& 询 = 定稿.目标.角色组[角色位置.at(子.参数组[2].规则角色)];
            if (主.种类 != 种类::领域对象引用 || 类.种类 != 种类::特征类型引用
                || 询.种类 != 种类::查询变量
                || 询.必要约束组.front().允许类别组 != std::vector{类别::查询特征值})
                return 拒绝("S10角色种类不相容");
            if (*子.特征值分支 == 新语言知识特征值分支::准确值) {
                if (类.必要约束组.front().允许类别组 != std::vector{类别::I64标量特征类型})
                    return 拒绝("S10准确值填项须I64特征类型");
            } else if (*子.特征值分支 == 新语言知识特征值分支::概念条件) {
                if (类.必要约束组.front().允许类别组.size() != 1
                    || 类.必要约束组.front().允许类别组.front() != 类别::正式特征类型)
                    return 拒绝("S10概念填项须正式特征类型");
            } else return 拒绝("S10特征值分支不合法");
            // 结构：角色0 的 角色1 是 什么；查询变量由“什么”覆盖，不出现角色表达。
            const auto& 项组 = 定稿.结构.有序项组;
            if (!std::holds_alternative<新语言知识角色表达项>(项组[0].载荷)
                || !std::holds_alternative<新语言知识固定词项>(项组[1].载荷)
                || !std::holds_alternative<新语言知识角色表达项>(项组[2].载荷)
                || !std::holds_alternative<新语言知识固定词项>(项组[3].载荷)
                || !std::holds_alternative<新语言知识固定词项>(项组[4].载荷))
                return 拒绝("S10结构项类别不合法");
            if (std::get<新语言知识角色表达项>(项组[0].载荷).规则角色 != 子.参数组[0].规则角色
                || std::get<新语言知识角色表达项>(项组[0].载荷).方式 != 新语言知识表达方式::明确对象描述
                || std::get<新语言知识角色表达项>(项组[2].载荷).规则角色 != 子.参数组[1].规则角色
                || std::get<新语言知识角色表达项>(项组[2].载荷).方式 != 新语言知识表达方式::名称表达)
                return 拒绝("S10角色表达不合法");
            for (std::size_t i = 0; i < 项组.size(); ++i)
                定稿.结构.有序项组[i].局部编号 = static_cast<std::int64_t>(i);
            std::sort(定稿.目标.角色组.begin(), 定稿.目标.角色组.end(),
                [](const auto& 左, const auto& 右) { return 左.局部编号 < 右.局部编号; });
            return 知识状态::已完成;
        }

        if (是S13) {
            std::sort(外层.参数组.begin(), 外层.参数组.end(),
                [](const auto& 左, const auto& 右) { return 左.参数 < 右.参数; });
            if (外层.参数组[0].参数 != 参数::受询者 || 外层.参数组[1].参数 != 参数::询求者)
                return 拒绝("S13外层参数须为受询者与询求者");
            auto& 项组 = 定稿.结构.有序项组;
            if (项组.size() < 5
                || !std::holds_alternative<新语言知识固定词项>(项组[0].载荷)
                || !std::holds_alternative<新语言知识角色表达项>(项组[1].载荷)
                || !std::holds_alternative<新语言知识固定词项>(项组[2].载荷)
                || !std::holds_alternative<新语言知识角色表达项>(项组[3].载荷))
                return 拒绝("S13结构前缀不合法");
            if (std::get<新语言知识角色表达项>(项组[1].载荷).规则角色 != 外层.参数组[0].规则角色
                || std::get<新语言知识角色表达项>(项组[1].载荷).方式 != 新语言知识表达方式::明确对象描述
                || std::get<新语言知识角色表达项>(项组[3].载荷).规则角色 != 外层.参数组[1].规则角色
                || std::get<新语言知识角色表达项>(项组[3].载荷).方式 != 新语言知识表达方式::已消解指代)
                return 拒绝("S13受询者或询求者表达不合法");
            if (!std::holds_alternative<新语言知识子规则项>(项组.back().载荷)
                && !(项组.size() == 5 && std::holds_alternative<新语言知识固定词项>(项组[4].载荷)))
                return 拒绝("S13查询内容结构不合法");
            for (std::size_t i = 0; i < 项组.size(); ++i)
                项组[i].局部编号 = static_cast<std::int64_t>(i);
            std::sort(定稿.目标.角色组.begin(), 定稿.目标.角色组.end(),
                [](const auto& 左, const auto& 右) { return 左.局部编号 < 右.局部编号; });
            return 知识状态::已完成;
        }
        return 拒绝("尚未支持的提问构式形状", 知识状态::未支持);
    }

    if (定稿.目标.目的 == 新语言知识表达目的::提出需求) {
        auto& 含义 = 定稿.目标.含义组.front();
        if (含义.局部编号 != 0 || 含义.操作 != 新语言知识含义操作::需求描述
            || 含义.作用域 != 新语言知识作用域::请求范围
            || 含义.确认范围 || 含义.特征值分支 || !含义.被询含义)
            return 拒绝("提出需求含义不合法");
        auto& 子 = *含义.被询含义;
        if (子.局部编号 != 1 || 子.作用域 != 新语言知识作用域::请求范围)
            return 拒绝("需求子含义作用域不合法");
        std::sort(含义.参数组.begin(), 含义.参数组.end(),
            [](const auto& 左, const auto& 右) { return 左.参数 < 右.参数; });
        if (含义.参数组.size() != 1 || 含义.参数组[0].参数 != 参数::主体)
            return 拒绝("需求描述须声明主体");
        const auto& 主 = 定稿.目标.角色组[角色位置.at(含义.参数组[0].规则角色)];
        if (主.种类 != 种类::领域对象引用) return 拒绝("需求主体须为存在");
        auto& 项组 = 定稿.结构.有序项组;
        if (项组.size() < 3
            || !std::holds_alternative<新语言知识角色表达项>(项组[0].载荷)
            || !std::holds_alternative<新语言知识固定词项>(项组[1].载荷)
            || !std::holds_alternative<新语言知识子规则项>(项组[2].载荷))
            return 拒绝("S11结构须为主体、需要、子规则");
        if (std::get<新语言知识角色表达项>(项组[0].载荷).规则角色 != 含义.参数组[0].规则角色
            || std::get<新语言知识角色表达项>(项组[0].载荷).方式 != 新语言知识表达方式::明确对象描述
            || !有效(std::get<新语言知识固定词项>(项组[1].载荷).词条节点)
            || !有效(std::get<新语言知识子规则项>(项组[2].载荷).子知识节点))
            return 拒绝("S11结构端点不合法");
        for (std::size_t i = 0; i < 项组.size(); ++i)
            项组[i].局部编号 = static_cast<std::int64_t>(i);
        std::sort(定稿.目标.角色组.begin(), 定稿.目标.角色组.end(),
            [](const auto& 左, const auto& 右) { return 左.局部编号 < 右.局部编号; });
        return 知识状态::已完成;
    }

    auto& 含义 = 定稿.目标.含义组.front();
    if (含义.局部编号 < 0 || 含义.作用域 != 新语言知识作用域::普通陈述
        || 含义.被询含义 || 含义.确认范围)
        return 拒绝("含义编号或作用域不支持", 知识状态::未支持);
    std::map<参数, std::int64_t> 参数角色;
    std::set<std::int64_t> 目标所用角色;
    for (const auto& 项 : 含义.参数组) {
        const bool 首叶参数 =
            (项.参数 >= 参数::主体 && 项.参数 <= 参数::询求者)
            || 项.参数 == 参数::上级 || 项.参数 == 参数::下级;
        if (!首叶参数
            || !角色位置.contains(项.规则角色) || !参数角色.emplace(项.参数, 项.规则角色).second)
            return 拒绝("含义参数重复、枚举不合法或角色悬空");
        目标所用角色.insert(项.规则角色);
    }
    const auto 角色是 = [&](参数 名, 种类 类型, std::optional<类别> 唯一类别 = std::nullopt) {
        const auto 参数位置 = 参数角色.find(名);
        if (参数位置 == 参数角色.end()) return false;
        const auto& 角色 = 定稿.目标.角色组[角色位置.at(参数位置->second)];
        return 角色.种类 == 类型 && (!唯一类别 || 角色.必要约束组.front().允许类别组 == std::vector{*唯一类别});
    };
    switch (含义.操作) {
    case 新语言知识含义操作::概念归类: {
        if (参数角色.size() != 2 || 含义.特征值分支 || !参数角色.contains(参数::主体)
            || !参数角色.contains(参数::归类概念)) return 拒绝("归类参数签名不完整");
        if (角色是(参数::主体, 种类::领域对象引用, 类别::存在)) {
            if (!角色是(参数::归类概念, 种类::概念引用, 类别::存在概念)) return 拒绝("存在归类必须使用存在概念");
        } else {
            if (!角色是(参数::主体, 种类::概念引用) || !角色是(参数::归类概念, 种类::概念引用))
                return 拒绝("上下位必须是同类别概念角色");
            const auto& 左 = 定稿.目标.角色组[角色位置.at(参数角色.at(参数::主体))].必要约束组.front().允许类别组;
            const auto& 右 = 定稿.目标.角色组[角色位置.at(参数角色.at(参数::归类概念))].必要约束组.front().允许类别组;
            if (左 != 右) return 拒绝("上下位概念允许类别必须一致");
        }
        break;
    }
    case 新语言知识含义操作::特征描述:
        if (参数角色.size() != 3 || !角色是(参数::主体, 种类::领域对象引用, 类别::存在)
            || !角色是(参数::特征类型, 种类::特征类型引用) || !含义.特征值分支) return 拒绝("特征描述参数签名不完整");
        if (*含义.特征值分支 == 新语言知识特征值分支::准确值) {
            if (!角色是(参数::特征类型, 种类::特征类型引用, 类别::I64标量特征类型)
                || !角色是(参数::特征值, 种类::准确值, 类别::I64标量)) return 拒绝("准确值分支必须使用I64类型与值");
        } else if (*含义.特征值分支 == 新语言知识特征值分支::概念条件) {
            if (!角色是(参数::特征值, 种类::概念引用, 类别::特征概念)) return 拒绝("概念条件分支必须使用特征概念");
        } else return 拒绝("特征值分支枚举不合法");
        break;
    case 新语言知识含义操作::组成描述:
        if (参数角色.size() != 2 || 含义.特征值分支
            || !角色是(参数::整体, 种类::领域对象引用, 类别::存在)
            || !角色是(参数::部分, 种类::领域对象引用, 类别::存在)) return 拒绝("组成参数必须明确存在整体和部分");
        break;
    case 新语言知识含义操作::组织隶属:
        if (参数角色.size() != 2 || 含义.特征值分支
            || !角色是(参数::上级, 种类::领域对象引用, 类别::存在)
            || !角色是(参数::下级, 种类::领域对象引用, 类别::存在))
            return 拒绝("组织隶属参数必须明确存在上级和下级");
        break;
    default: return 拒绝("首叶未支持该含义操作", 知识状态::未支持);
    }
    std::set<std::int64_t> 项编号;
    std::map<std::int64_t, std::int64_t> 新编号;
    std::size_t 指代项数 = 0;
    for (std::size_t i = 0; i < 定稿.结构.有序项组.size(); ++i) {
        auto& 项 = 定稿.结构.有序项组[i];
        if (项.局部编号 < 0 || !项编号.insert(项.局部编号).second || 项.载荷.valueless_by_exception())
            return 拒绝("结构项编号或载荷不合法");
        项.局部编号 = static_cast<std::int64_t>(i);
        if (const auto* 词 = std::get_if<新语言知识固定词项>(&项.载荷)) {
            if (!有效(词->词条节点)) return 拒绝("固定词不是正式非零词条引用");
        } else if (auto* 角色表达 = std::get_if<新语言知识角色表达项>(&项.载荷)) {
            auto& 表达 = *角色表达;
            const auto 位置 = 角色位置.find(表达.规则角色);
            if (位置 == 角色位置.end()) return 拒绝("结构引用悬空角色");
            const auto 类型 = 定稿.目标.角色组[位置->second].种类;
            const bool 合法 = (表达.方式 == 新语言知识表达方式::名称表达 && (类型 == 种类::概念引用 || 类型 == 种类::特征类型引用))
                || (表达.方式 == 新语言知识表达方式::明确对象描述 && 类型 == 种类::领域对象引用)
                || (表达.方式 == 新语言知识表达方式::类型化字面值 && 类型 == 种类::准确值)
                || (表达.方式 == 新语言知识表达方式::已消解指代
                    && 类型 == 种类::领域对象引用 && 含义.操作 == 新语言知识含义操作::概念归类
                    && 表达.规则角色 == 参数角色.at(参数::主体));
            if (!合法) return 拒绝("表达方式与角色不相容");
            if ((表达.方式 == 新语言知识表达方式::已消解指代)
                != (表达.指代概念节点 && 有效(*表达.指代概念节点)))
                return 拒绝("指代方式须恰有正式定义引用，旧方式不得携带引用");
            if (表达.方式 == 新语言知识表达方式::已消解指代) ++指代项数;
            if (!新编号.contains(表达.规则角色)) 新编号.emplace(表达.规则角色, static_cast<std::int64_t>(新编号.size()));
            表达.规则角色 = 新编号.at(表达.规则角色);
        } else return 拒绝("陈述知识不接受子规则项");
    }
    if (新编号.size() != 角色位置.size() || 目标所用角色.size() != 角色位置.size()) return 拒绝("角色未在目标及结构完整引用");
    if (指代项数) {
        if (指代项数 != 1 || 定稿.目标.角色组.size() != 2
            || 定稿.结构.有序项组.size() != 3
            || !std::holds_alternative<新语言知识角色表达项>(定稿.结构.有序项组.front().载荷)
            || !std::holds_alternative<新语言知识固定词项>(定稿.结构.有序项组[1].载荷)
            || !std::holds_alternative<新语言知识角色表达项>(定稿.结构.有序项组.back().载荷)
            || std::get<新语言知识角色表达项>(定稿.结构.有序项组.front().载荷).方式
                != 新语言知识表达方式::已消解指代
            || std::get<新语言知识角色表达项>(定稿.结构.有序项组.back().载荷).方式
                != 新语言知识表达方式::名称表达)
            return 拒绝("本叶指代知识只接受S01主体、固定词、概念名称三项结构");
    }
    auto 原角色组 = std::move(定稿.目标.角色组);
    定稿.目标.角色组.resize(原角色组.size());
    for (auto& 角色 : 原角色组) {
        角色.局部编号 = 新编号.at(角色.局部编号);
        角色.必要约束组.front().被约束角色 = 角色.局部编号;
        定稿.目标.角色组[static_cast<std::size_t>(角色.局部编号)] = std::move(角色);
    }
    for (auto& 项 : 含义.参数组) 项.规则角色 = 新编号.at(项.规则角色);
    std::sort(含义.参数组.begin(), 含义.参数组.end(), [](const auto& 左, const auto& 右) { return 左.参数 < 右.参数; });
    含义.局部编号 = 0;
    return 知识状态::已完成;
}

bool 子知识符合归类陈述形状(const 新语言知识定义& 外层,
    const 新语言知识定义& 子定义) {
    if (外层.目标.目的 != 新语言知识表达目的::提问
        && 外层.目标.目的 != 新语言知识表达目的::提出需求) return true;
    if (子定义.结构.语言节点 != 外层.结构.语言节点
        || 子定义.目标.含义组.size() != 1
        || 子定义.结构.类别 != 新语言知识结构类别::基础表达) return false;
    // S09 概念归类确认：子知识角色与外层完全同构。
    const bool 是S09子 = 外层.目标.目的 == 新语言知识表达目的::提问
        && 外层.目标.含义组.front().被询含义
        && 外层.目标.含义组.front().被询含义->操作 == 新语言知识含义操作::概念归类
        && 外层.结构.有序项组.size() == 2
        && std::holds_alternative<新语言知识子规则项>(外层.结构.有序项组.front().载荷);
    if (!是S09子) {
        const auto& 子含义 = 子定义.目标.含义组.front();
        if (外层.目标.目的 == 新语言知识表达目的::提出需求)
            return 子定义.目标.目的 == 新语言知识表达目的::陈述
                && 子含义.作用域 == 新语言知识作用域::普通陈述;
        if (外层.目标.目的 == 新语言知识表达目的::提问
            && 外层.目标.含义组.front().被询含义
            && 外层.目标.含义组.front().被询含义->操作 == 新语言知识含义操作::需求描述)
            return 子定义.目标.目的 == 新语言知识表达目的::提出需求
                && 子含义.操作 == 新语言知识含义操作::需求描述
                && 子含义.作用域 == 新语言知识作用域::请求范围;
        if (外层.目标.目的 == 新语言知识表达目的::提问
            && 外层.目标.含义组.front().确认范围
            && *外层.目标.含义组.front().确认范围 == 新语言知识确认范围::填项内容)
            return (子定义.目标.目的 == 新语言知识表达目的::陈述
                    && 子含义.作用域 == 新语言知识作用域::普通陈述)
                || (子定义.目标.目的 == 新语言知识表达目的::提问
                    && 子含义.操作 == 新语言知识含义操作::信息请求
                    && 子含义.作用域 == 新语言知识作用域::提问范围);
        return 子含义.作用域 == 新语言知识作用域::普通陈述;
    }
    if (子定义.目标.目的 != 新语言知识表达目的::陈述
        || 子定义.目标.含义组.front().操作 != 新语言知识含义操作::概念归类
        || 子定义.目标.含义组.front().作用域 != 新语言知识作用域::普通陈述
        || 子定义.目标.角色组 != 外层.目标.角色组
        || 子定义.目标.含义组.front().参数组 != 外层.目标.含义组.front().被询含义->参数组
        || 子定义.结构.有序项组.size() < 3
        || !std::holds_alternative<新语言知识角色表达项>(子定义.结构.有序项组.front().载荷)
        || !std::holds_alternative<新语言知识角色表达项>(子定义.结构.有序项组.back().载荷)
        || std::get<新语言知识角色表达项>(子定义.结构.有序项组.front().载荷)
            != 新语言知识角色表达项{0, 新语言知识表达方式::名称表达}
        || std::get<新语言知识角色表达项>(子定义.结构.有序项组.back().载荷)
            != 新语言知识角色表达项{1, 新语言知识表达方式::名称表达}) return false;
    for (std::size_t i = 1; i + 1 < 子定义.结构.有序项组.size(); ++i)
        if (!std::holds_alternative<新语言知识固定词项>(子定义.结构.有序项组[i].载荷)) return false;
    return true;
}

bool 子规则角色映射完整(const 新语言知识定义& 外层,
    const 新语言知识子规则项& 子项, const 新语言知识定义& 子定义) {
    if (子项.映射组.size() != 子定义.目标.角色组.size()) return false;
    std::set<std::int64_t> 已映射子角色;
    for (const auto& 映射 : 子项.映射组) {
        const auto 外角色 = std::find_if(外层.目标.角色组.begin(), 外层.目标.角色组.end(),
            [&](const auto& 角色) { return 角色.局部编号 == 映射.外层角色编号; });
        const auto 子角色 = std::find_if(子定义.目标.角色组.begin(), 子定义.目标.角色组.end(),
            [&](const auto& 角色) { return 角色.局部编号 == 映射.子规则角色编号; });
        if (外角色 == 外层.目标.角色组.end() || 子角色 == 子定义.目标.角色组.end()
            || 外角色->种类 != 子角色->种类
            || !已映射子角色.insert(映射.子规则角色编号).second) return false;
    }
    return true;
}

} // namespace

namespace {

知识状态 知识模式状态已加锁() {
    if (std::all_of(知识句柄.begin(), 知识句柄.end(), [](auto 值) { return !有效(值); })) return 知识状态::尚未初始化;
    if (!结构仍存在()) return 知识状态::内部不一致;
    for (const auto 句柄 : 知识句柄) {
        if (!有效(句柄)) return 知识状态::内部不一致;
        const auto 读取 = 全局基础数据集.查询完整节点结构(句柄);
        if (读取.状态 != 基础结构操作状态::已完成) return 基础知识状态(读取.状态);
        if (!读取.结构 || 读取.结构->节点.编码 != 句柄 || !读取.结构->字段组.empty()) return 知识状态::内部不一致;
    }
    return 知识状态::已完成;
}

// 仅在同一语言锁内使用；完整字段与逆向归属校验，不取得锁、不回调上层。
struct 知识读取现场 final {
    知识状态 状态 = 知识状态::已完成;
    std::string 诊断;
    std::map<稳定编码, 基础节点结构> 结构组;
    std::set<稳定编码> 活动知识;
    void 拒绝(const char* 原因, 知识状态 新状态 = 知识状态::内部不一致) {
        if (状态 == 知识状态::已完成) { 状态 = 新状态; 诊断 = 原因; }
    }
    const 基础节点结构* 节点(稳定编码 身份) {
        if (状态 != 知识状态::已完成) return nullptr;
        if (!有效(身份)) { 拒绝("结构含有零节点引用"); return nullptr; }
        const auto 已有 = 结构组.find(身份);
        if (已有 != 结构组.end()) return &已有->second;
        const auto 读取 = 全局基础数据集.查询完整节点结构(身份);
        if (读取.状态 != 基础结构操作状态::已完成) {
            拒绝("完整基础节点无法读回", 基础知识状态(读取.状态)); return nullptr;
        }
        if (!读取.结构 || 读取.结构->节点.编码 != 身份) { 拒绝("完整基础节点身份不一致"); return nullptr; }
        return &结构组.emplace(身份, *读取.结构).first->second;
    }
    std::vector<const 基础字段关系*> 字段(稳定编码 身份, 稳定编码 键) {
        std::vector<const 基础字段关系*> 组;
        if (const auto* 结构 = 节点(身份))
            for (const auto& 项 : 结构->字段组) if (项.字段节点 == 键) 组.push_back(&项);
        return 组;
    }
    bool 类型(稳定编码 身份, 知识标识 预期, std::initializer_list<知识标识> 允许) {
        const auto* 结构 = 节点(身份);
        if (!结构) return false;
        std::set<稳定编码> 字段编码;
        for (const auto& 项 : 结构->字段组) {
            if (!有效(项.编码) || 项.所属节点 != 身份 || !字段编码.insert(项.编码).second
                || (项.字段节点 != 节点类型字段 && std::none_of(允许.begin(), 允许.end(),
                    [&](auto 键) { return 项.字段节点 == 知识键(键); }))) {
                拒绝("知识节点有陌生字段、重复字段身份或错误归属"); return false;
            }
            if (const auto* 引用 = std::get_if<稳定编码>(&项.内容)) {
                const auto 存在 = 全局基础数据集.查询完整节点结构(*引用);
                if (存在.状态 != 基础结构操作状态::已完成) {
                    拒绝("知识强引用无法完整读取", 基础知识状态(存在.状态)); return false;
                }
                if (!存在.结构 || 存在.结构->节点.编码 != *引用) { 拒绝("知识强引用身份不一致"); return false; }
            }
        }
        const auto 类型组 = 字段(身份, 节点类型字段);
        const auto* 值 = 类型组.size() == 1 ? std::get_if<稳定编码>(&类型组.front()->内容) : nullptr;
        if (!值 || *值 != 知识键(预期)) { 拒绝("知识节点类型不相容"); return false; }
        return true;
    }
    std::vector<稳定编码> 引用组(稳定编码 身份, 知识标识 键, std::size_t 最少 = 0,
        std::size_t 最多 = (std::numeric_limits<std::size_t>::max)()) {
        const auto 字段组 = 字段(身份, 知识键(键));
        std::vector<稳定编码> 组;
        if (状态 != 知识状态::已完成) return 组;
        if (字段组.size() < 最少 || 字段组.size() > 最多) { 拒绝("知识引用字段基数不合法"); return 组; }
        std::set<稳定编码> 去重;
        for (const auto* 项 : 字段组) {
            const auto* 值 = std::get_if<稳定编码>(&项->内容);
            if (!值 || !有效(*值) || !去重.insert(*值).second) { 拒绝("知识引用字段载荷或重复组成不合法"); return {}; }
            组.push_back(*值);
        }
        return 组;
    }
    稳定编码 引用(稳定编码 身份, 知识标识 键) {
        const auto 组 = 引用组(身份, 键, 1, 1);
        return 组.empty() ? 稳定编码{} : 组.front();
    }
    template<class T> T 原始值(稳定编码 身份, 知识标识 键) {
        const auto 组 = 字段(身份, 知识键(键));
        const auto* 值 = 组.size() == 1 ? std::get_if<基础值>(&组.front()->内容) : nullptr;
        const auto* 材料 = 值 ? std::get_if<T>(&值->材料) : nullptr;
        if (!材料) { 拒绝("知识原始字段基数或物理类型不合法"); return {}; }
        return *材料;
    }
    template<class T> T 枚举(稳定编码 身份, 知识标识 键, std::int64_t 最大) {
        const auto 值 = 原始值<std::int64_t>(身份, 键);
        if (值 < 1 || 值 > 最大) { 拒绝("保存的知识枚举不合法"); return static_cast<T>(0); }
        return static_cast<T>(值);
    }
    bool 归属(稳定编码 子, 知识标识 键, 稳定编码 父) {
        if (状态 != 知识状态::已完成) return false;
        const auto 反查 = 全局基础数据集.查询字段引用节点结构(子, 知识键(键));
        if (反查.状态 != 基础结构操作状态::已完成) { 拒绝("组成反向完整读取未完成", 基础知识状态(反查.状态)); return false; }
        if (反查.引用字段组.size() != 1 || 反查.来源节点组.size() != 1
            || 反查.引用字段组.front().所属节点 != 父 || 反查.来源节点组.front().节点.编码 != 父) {
            拒绝("知识私有组成没有唯一所属节点"); return false;
        }
        const auto* 已读 = 节点(父);
        if (!已读 || !基础结构相等(*已读, 反查.来源节点组.front())) { 拒绝("组成正反完整结构不一致"); return false; }
        return true;
    }
};

新语言知识来源结果 读取知识来源细节已加锁(稳定编码 来源节点, 知识读取现场& 现场) {
    新语言知识来源结果 结果;
    if (!现场.类型(来源节点, 知识标识::来源类型,
        {知识标识::来源种类, 知识标识::来源语言, 知识标识::来源原文})) {
        结果.状态 = 现场.状态; return 结果;
    }
    新语言知识来源信息 信息{};
    信息.节点 = 来源节点;
    信息.种类 = 现场.枚举<新语言知识来源种类>(来源节点, 知识标识::来源种类, 1);
    信息.语言节点 = 现场.引用(来源节点, 知识标识::来源语言);
    信息.固定原文 = 现场.原始值<std::string>(来源节点, 知识标识::来源原文);
    std::int64_t 标量数 = 0;
    if (现场.状态 != 知识状态::已完成) { 结果.状态 = 现场.状态; return 结果; }
    if (!读取语言信息(信息.语言节点)) {
        现场.拒绝("先天来源语言身份不可读", 知识状态::引用不可读);
        结果.状态 = 现场.状态; return 结果;
    }
    if (!知识来源UTF8有效(信息.固定原文, 标量数)) {
        现场.拒绝("先天来源原文不是完整合法材料");
        结果.状态 = 现场.状态; return 结果;
    }
    if (!现场.节点(信息.语言节点)) { 结果.状态 = 现场.状态; return 结果; }
    结果.状态 = 知识状态::已完成;
    结果.信息 = std::move(信息);
    return 结果;
}
新语言知识来源结果 读取知识来源已加锁(稳定编码 来源节点) {
    知识读取现场 现场;
    return 读取知识来源细节已加锁(来源节点, 现场);
}

新语言子含义映射 读图映射(知识读取现场& 现场,稳定编码 身份,稳定编码 宿主,知识标识 成员键) {
    if(!现场.类型(身份,知识标识::图映射类型,{知识标识::图映射宿主,知识标识::图映射源含义,
        知识标识::图映射源角色,知识标识::图映射目标含义,知识标识::图映射目标角色})
        ||现场.引用(身份,知识标识::图映射宿主)!=宿主||!现场.归属(身份,成员键,宿主))return {};
    return {{现场.原始值<std::int64_t>(身份,知识标识::图映射源含义),现场.原始值<std::int64_t>(身份,知识标识::图映射源角色)},
        {现场.原始值<std::int64_t>(身份,知识标识::图映射目标含义),现场.原始值<std::int64_t>(身份,知识标识::图映射目标角色)}};
}
新语言知识含义图 读含义图(知识读取现场& 现场,稳定编码 目标) {
    新语言知识含义图 图;
    if(现场.原始值<std::int64_t>(目标,知识标识::目标图格式)!=1)现场.拒绝("未知目标图格式");
    图.根含义编号=现场.原始值<std::int64_t>(目标,知识标识::目标图根编号);
    for(auto 身份:现场.引用组(目标,知识标识::目标图成员,1)) {
        新语言图含义节点 节点;
        if(!现场.类型(身份,知识标识::图含义类型,{知识标识::图含义宿主,知识标识::图含义编号,知识标识::图含义操作,
            知识标识::图含义作用域,知识标识::图含义角色,知识标识::图含义参数,知识标识::图含义分支,
            知识标识::图含义确认,知识标识::图含义方向,知识标识::图子边宿主})
            ||现场.引用(身份,知识标识::图含义宿主)!=目标||!现场.归属(身份,知识标识::目标图成员,目标))return 图;
        节点.局部编号=现场.原始值<std::int64_t>(身份,知识标识::图含义编号);
        节点.操作=现场.枚举<新语言知识含义操作>(身份,知识标识::图含义操作,
            static_cast<std::int64_t>(新语言知识含义操作::特征子类型定义));
        节点.作用域=现场.枚举<新语言知识作用域>(身份,知识标识::图含义作用域,4);
        if(!现场.字段(身份,知识键(知识标识::图含义分支)).empty())节点.特征值分支=现场.枚举<新语言知识特征值分支>(身份,知识标识::图含义分支,2);
        if(!现场.字段(身份,知识键(知识标识::图含义确认)).empty())节点.确认范围=现场.枚举<新语言知识确认范围>(身份,知识标识::图含义确认,2);
        if(!现场.字段(身份,知识键(知识标识::图含义方向)).empty())节点.方向=现场.枚举<新语言知识特征方向>(身份,知识标识::图含义方向,2);
        for(auto 角色节点:现场.引用组(身份,知识标识::图含义角色)) {
            if(!现场.类型(角色节点,知识标识::角色类型,{知识标识::图角色宿主,知识标识::角色编号,
                知识标识::角色种类,知识标识::角色数量,知识标识::角色约束})
                ||现场.引用(角色节点,知识标识::图角色宿主)!=身份||!现场.归属(角色节点,知识标识::图含义角色,身份))return 图;
            新语言知识角色定义 角色{};
            角色.局部编号=现场.原始值<std::int64_t>(角色节点,知识标识::角色编号);
            角色.种类=现场.枚举<新语言知识角色种类>(角色节点,知识标识::角色种类,6);
            角色.数量=现场.枚举<新语言知识角色数量>(角色节点,知识标识::角色数量,1);
            const auto 约束=现场.引用(角色节点,知识标识::角色约束);
            if(!现场.类型(约束,知识标识::约束类型,{知识标识::约束角色,知识标识::约束类别})||!现场.归属(约束,知识标识::角色约束,角色节点))return 图;
            新语言知识角色类别约束 条件{现场.原始值<std::int64_t>(约束,知识标识::约束角色),{}};
            for(auto 类别:现场.原始值<std::vector<std::int64_t>>(约束,知识标识::约束类别))条件.允许类别组.push_back(static_cast<新语言知识角色类别>(类别));
            角色.必要约束组.push_back(std::move(条件));节点.角色组.push_back(std::move(角色));
        }
        for(auto 参数节点:现场.引用组(身份,知识标识::图含义参数)) {
            if(!现场.类型(参数节点,知识标识::参数类型,{知识标识::图参数宿主,知识标识::参数名,知识标识::参数角色})
                ||现场.引用(参数节点,知识标识::图参数宿主)!=身份||!现场.归属(参数节点,知识标识::图含义参数,身份))return 图;
            节点.参数组.push_back({现场.枚举<新语言知识参数角色>(参数节点,知识标识::参数名,
                static_cast<std::int64_t>(新语言知识参数角色::适用对象)),
                现场.原始值<std::int64_t>(参数节点,知识标识::参数角色)});
        }
        // 边以独立节点保存；宿主引用反查，避免把子目标编号当成生产节点编码。
        const auto 边读取=全局基础数据集.查询字段引用节点结构(身份,知识键(知识标识::图子边宿主));
        if(边读取.状态!=基础结构操作状态::已完成){现场.拒绝("图子边读取失败",基础知识状态(边读取.状态));return 图;}
        for(const auto& 边结构:边读取.来源节点组) {
            const auto 边节点=边结构.节点.编码;
            if(!现场.类型(边节点,知识标识::图子边类型,{知识标识::图子边宿主,知识标识::图子边角色,
                知识标识::图子边用途,知识标识::图子边目标编号,知识标识::图子边映射})||现场.引用(边节点,知识标识::图子边宿主)!=身份)return 图;
            新语言子含义边 边{现场.原始值<std::int64_t>(边节点,知识标识::图子边角色),现场.原始值<std::int64_t>(边节点,知识标识::图子边目标编号),
                现场.枚举<新语言子含义用途>(边节点,知识标识::图子边用途,3),{}};
            for(auto 映射:现场.引用组(边节点,知识标识::图子边映射))边.显式映射组.push_back(读图映射(现场,映射,边节点,知识标识::图子边映射));
            节点.子含义组.push_back(std::move(边));
        }
        std::sort(节点.子含义组.begin(),节点.子含义组.end(),[](const auto& 左,const auto& 右){return 左.本节点角色编号<右.本节点角色编号;});
        图.节点组.push_back(std::move(节点));
    }
    std::sort(图.节点组.begin(),图.节点组.end(),[](const auto& 左,const auto& 右){return 左.局部编号<右.局部编号;});
    return 图;
}
新语言知识结果 读取知识结构细节已加锁(稳定编码 知识节点, 知识读取现场& 现场,
    bool 允许子规则 = true) {
    新语言知识结果 结果;
    结果.阶段 = 新语言知识发布阶段::完整读回;
    const auto 返回未完成 = [&]() {
        结果.状态 = 现场.状态;
        结果.诊断说明 = 现场.诊断;
        return 结果;
    };
    if(!现场.活动知识.insert(知识节点).second){现场.拒绝("知识子规则形成环");return 返回未完成();}
    struct 活动知识离域 final {std::set<稳定编码>& 组;稳定编码 节点;~活动知识离域(){组.erase(节点);}} 离域{现场.活动知识,知识节点};
    if (!现场.类型(知识节点, 知识标识::知识类型, {知识标识::核心目标, 知识标识::核心结构})) return 返回未完成();
    新语言知识信息 信息{};
    信息.节点 = 知识节点;
    信息.目标节点 = 现场.引用(知识节点, 知识标识::核心目标);
    信息.结构节点 = 现场.引用(知识节点, 知识标识::核心结构);
    const auto 目标 = 信息.目标节点, 结构 = 信息.结构节点;
    if (!现场.类型(目标, 知识标识::目标类型,
        {知识标识::目标目的, 知识标识::目标角色, 知识标识::目标含义, 知识标识::目标条件,
            知识标识::目标图格式,知识标识::目标图根编号,知识标识::目标图成员})
        || !现场.归属(目标, 知识标识::核心目标, 知识节点)
        || !现场.类型(结构, 知识标识::结构类型,
            {知识标识::结构类别, 知识标识::结构语言, 知识标识::结构项})
        || !现场.归属(结构, 知识标识::核心结构, 知识节点)) return 返回未完成();
    信息.定义.目标.目的 = 现场.枚举<新语言知识表达目的>(目标, 知识标识::目标目的, 4);
    if(!现场.字段(目标,知识键(知识标识::目标图格式)).empty()) {
        for(auto 字段:{知识标识::目标角色,知识标识::目标含义,知识标识::目标条件})
            if(!现场.字段(目标,知识键(字段)).empty())现场.拒绝("图与旧目标布局混写");
        信息.定义.目标.含义图=读含义图(现场,目标);
    }else {
    if(!现场.字段(目标,知识键(知识标识::目标图根编号)).empty()||!现场.字段(目标,知识键(知识标识::目标图成员)).empty())现场.拒绝("图格式缺失");
    (void)现场.引用组(目标, 知识标识::目标条件, 0, 0);
    const auto 角色组 = 现场.引用组(目标, 知识标识::目标角色, 1);
    for (const auto 角色节点 : 角色组) {
        if (!现场.类型(角色节点, 知识标识::角色类型,
            {知识标识::角色编号, 知识标识::角色种类, 知识标识::角色数量, 知识标识::角色约束})
            || !现场.归属(角色节点, 知识标识::目标角色, 目标)) return 返回未完成();
        新语言知识角色定义 角色{};
        角色.局部编号 = 现场.原始值<std::int64_t>(角色节点, 知识标识::角色编号);
        角色.种类 = 现场.枚举<新语言知识角色种类>(角色节点, 知识标识::角色种类,
            static_cast<std::int64_t>(新语言知识角色种类::子含义引用));
        角色.数量 = 现场.枚举<新语言知识角色数量>(角色节点, 知识标识::角色数量, 1);
        const auto 约束节点 = 现场.引用(角色节点, 知识标识::角色约束);
        if (!现场.类型(约束节点, 知识标识::约束类型, {知识标识::约束角色, 知识标识::约束类别})
            || !现场.归属(约束节点, 知识标识::角色约束, 角色节点)) return 返回未完成();
        新语言知识角色类别约束 约束{};
        约束.被约束角色 = 现场.原始值<std::int64_t>(约束节点, 知识标识::约束角色);
        for (auto 值 : 现场.原始值<std::vector<std::int64_t>>(约束节点, 知识标识::约束类别)) {
            if (值 < 1 || 值 > static_cast<std::int64_t>(新语言知识角色类别::查询存在)) {
                现场.拒绝("保存的角色类别枚举不合法"); return 返回未完成();
            }
            约束.允许类别组.push_back(static_cast<新语言知识角色类别>(值));
        }
        角色.必要约束组.push_back(std::move(约束));
        信息.定义.目标.角色组.push_back(std::move(角色));
    }
    std::sort(信息.定义.目标.角色组.begin(), 信息.定义.目标.角色组.end(), [](const auto& 左, const auto& 右) { return 左.局部编号 < 右.局部编号; });
    for (std::size_t i = 0; i < 信息.定义.目标.角色组.size(); ++i)
        if (信息.定义.目标.角色组[i].局部编号 != static_cast<std::int64_t>(i)) { 现场.拒绝("保存的角色编号不连续"); return 返回未完成(); }
    const auto 含义节点 = 现场.引用(目标, 知识标识::目标含义);
    if (!现场.类型(含义节点, 知识标识::含义类型,
        {知识标识::含义编号, 知识标识::含义操作, 知识标识::含义作用域, 知识标识::含义参数,
            知识标识::含义分支, 知识标识::含义子项, 知识标识::含义确认范围})
        || !现场.归属(含义节点, 知识标识::目标含义, 目标)) return 返回未完成();
    新语言知识含义项 含义{};
    含义.局部编号 = 现场.原始值<std::int64_t>(含义节点, 知识标识::含义编号);
    if (含义.局部编号 != 0) { 现场.拒绝("保存的唯一含义编号必须为0"); return 返回未完成(); }
    // 旧目标布局读回上限取枚举末项（组织隶属/下级）；S组织隶属走此路径而非图布局。
    含义.操作 = 现场.枚举<新语言知识含义操作>(含义节点, 知识标识::含义操作,
        static_cast<std::int64_t>(新语言知识含义操作::组织隶属));
    含义.作用域 = 现场.枚举<新语言知识作用域>(含义节点, 知识标识::含义作用域,
        static_cast<std::int64_t>(新语言知识作用域::需求目标范围));
    const auto 分支字段 = 现场.字段(含义节点, 知识键(知识标识::含义分支));
    if (分支字段.size() > 1) { 现场.拒绝("特征分支字段重复"); return 返回未完成(); }
    if (!分支字段.empty()) 含义.特征值分支 = 现场.枚举<新语言知识特征值分支>(含义节点, 知识标识::含义分支, 2);
    const bool 允许空外层参数 = 含义.操作 == 新语言知识含义操作::信息请求
        || 含义.操作 == 新语言知识含义操作::缺口登记;
    for (const auto 参数节点 : 现场.引用组(含义节点, 知识标识::含义参数,
        允许空外层参数 ? 0 : 1, (std::numeric_limits<std::size_t>::max)())) {
        if (!现场.类型(参数节点, 知识标识::参数类型, {知识标识::参数名, 知识标识::参数角色})
            || !现场.归属(参数节点, 知识标识::含义参数, 含义节点)) return 返回未完成();
        含义.参数组.push_back({现场.枚举<新语言知识参数角色>(参数节点, 知识标识::参数名,
            static_cast<std::int64_t>(新语言知识参数角色::下级)),
            现场.原始值<std::int64_t>(参数节点, 知识标识::参数角色)});
    }
    const bool 需要子含义 = 含义.操作 == 新语言知识含义操作::信息请求
        || 含义.操作 == 新语言知识含义操作::需求描述;
    const auto 子项组 = 现场.引用组(含义节点, 知识标识::含义子项, 需要子含义 ? 1 : 0,
        需要子含义 ? 1 : 0);
    const auto 确认组 = 现场.字段(含义节点, 知识键(知识标识::含义确认范围));
    if (含义.操作 == 新语言知识含义操作::信息请求) {
        if (确认组.size() != 1 || 子项组.size() != 1) { 现场.拒绝("信息请求缺少唯一确认范围或子含义"); return 返回未完成(); }
        含义.确认范围 = 现场.枚举<新语言知识确认范围>(含义节点, 知识标识::含义确认范围, 2);
        const auto 子节点 = 子项组.front();
        if (!现场.类型(子节点, 知识标识::含义类型,
            {知识标识::含义编号, 知识标识::含义操作, 知识标识::含义作用域, 知识标识::含义参数,
                知识标识::含义分支})
            || !现场.归属(子节点, 知识标识::含义子项, 含义节点)) return 返回未完成();
        新语言知识被询含义 子{};
        子.局部编号 = 现场.原始值<std::int64_t>(子节点, 知识标识::含义编号);
        子.操作 = 现场.枚举<新语言知识含义操作>(子节点, 知识标识::含义操作,
            static_cast<std::int64_t>(新语言知识含义操作::组织隶属));
        子.作用域 = 现场.枚举<新语言知识作用域>(子节点, 知识标识::含义作用域,
            static_cast<std::int64_t>(新语言知识作用域::需求目标范围));
        const auto 子分支字段 = 现场.字段(子节点, 知识键(知识标识::含义分支));
        if (现场.状态 != 知识状态::已完成) return 返回未完成();
        if (!子分支字段.empty()) 子.特征值分支 = 现场.枚举<新语言知识特征值分支>(子节点, 知识标识::含义分支, 2);
        for (const auto 参数节点 : 现场.引用组(子节点, 知识标识::含义参数, 1, 8)) {
            if (!现场.类型(参数节点, 知识标识::参数类型,
                {知识标识::参数名, 知识标识::参数角色})
                || !现场.归属(参数节点, 知识标识::含义参数, 子节点)) return 返回未完成();
            子.参数组.push_back({现场.枚举<新语言知识参数角色>(参数节点, 知识标识::参数名,
                static_cast<std::int64_t>(新语言知识参数角色::下级)),
                现场.原始值<std::int64_t>(参数节点, 知识标识::参数角色)});
        }
        std::sort(子.参数组.begin(), 子.参数组.end(),
            [](const auto& 左, const auto& 右) { return 左.参数 < 右.参数; });
        含义.被询含义 = std::move(子);
    } else if (含义.操作 == 新语言知识含义操作::需求描述) {
        if (!确认组.empty() || 子项组.size() != 1) {
            现场.拒绝("需求描述须恰有子含义且无确认范围"); return 返回未完成();
        }
        const auto 子节点 = 子项组.front();
        if (!现场.类型(子节点, 知识标识::含义类型,
            {知识标识::含义编号, 知识标识::含义操作, 知识标识::含义作用域, 知识标识::含义参数,
                知识标识::含义分支})
            || !现场.归属(子节点, 知识标识::含义子项, 含义节点)) return 返回未完成();
        新语言知识被询含义 子{};
        子.局部编号 = 现场.原始值<std::int64_t>(子节点, 知识标识::含义编号);
        子.操作 = 现场.枚举<新语言知识含义操作>(子节点, 知识标识::含义操作,
            static_cast<std::int64_t>(新语言知识含义操作::组织隶属));
        子.作用域 = 现场.枚举<新语言知识作用域>(子节点, 知识标识::含义作用域,
            static_cast<std::int64_t>(新语言知识作用域::需求目标范围));
        const auto 子分支字段 = 现场.字段(子节点, 知识键(知识标识::含义分支));
        if (现场.状态 != 知识状态::已完成) return 返回未完成();
        if (!子分支字段.empty()) 子.特征值分支 = 现场.枚举<新语言知识特征值分支>(子节点, 知识标识::含义分支, 2);
        for (const auto 参数节点 : 现场.引用组(子节点, 知识标识::含义参数, 1, 8)) {
            if (!现场.类型(参数节点, 知识标识::参数类型,
                {知识标识::参数名, 知识标识::参数角色})
                || !现场.归属(参数节点, 知识标识::含义参数, 子节点)) return 返回未完成();
            子.参数组.push_back({现场.枚举<新语言知识参数角色>(参数节点, 知识标识::参数名,
                static_cast<std::int64_t>(新语言知识参数角色::下级)),
                现场.原始值<std::int64_t>(参数节点, 知识标识::参数角色)});
        }
        std::sort(子.参数组.begin(), 子.参数组.end(),
            [](const auto& 左, const auto& 右) { return 左.参数 < 右.参数; });
        含义.被询含义 = std::move(子);
    } else if (!确认组.empty() || !子项组.empty()) {
        现场.拒绝("普通含义混入确认范围或子含义"); return 返回未完成();
    }
    std::sort(含义.参数组.begin(), 含义.参数组.end(), [](const auto& 左, const auto& 右) { return 左.参数 < 右.参数; });
    信息.定义.目标.含义组.push_back(std::move(含义));
    }
    信息.定义.结构.类别 = 现场.枚举<新语言知识结构类别>(结构, 知识标识::结构类别, 2);
    信息.定义.结构.语言节点 = 现场.引用(结构, 知识标识::结构语言);
    if (现场.状态 != 知识状态::已完成) return 返回未完成();
    if (!读取语言信息(信息.定义.结构.语言节点) || !现场.节点(信息.定义.结构.语言节点)) { 现场.拒绝("知识语言引用不可读", 知识状态::引用不可读); return 返回未完成(); }
    for (const auto 项节点 : 现场.引用组(结构, 知识标识::结构项, 1)) {
        if (!现场.类型(项节点, 知识标识::项类型,
            {知识标识::项编号, 知识标识::项词, 知识标识::项角色, 知识标识::项方式,
                知识标识::项子知识, 知识标识::项子映射, 知识标识::项指代定义,
                知识标识::图项源含义,知识标识::图项角色,知识标识::图项子知识,知识标识::图项映射,
                知识标识::图项指代概念,知识标识::图项指代名称,知识标识::图项概念前词,知识标识::图项概念后词,知识标识::图项查询词})
            || !现场.归属(项节点, 知识标识::结构项, 结构)) return 返回未完成();
        新语言知识结构项 项{};
        项.局部编号 = 现场.原始值<std::int64_t>(项节点, 知识标识::项编号);
        const auto 固定词 = 现场.字段(项节点, 知识键(知识标识::项词));
        const auto 子规则 = 现场.字段(项节点, 知识键(知识标识::项子知识));
        if(!现场.字段(项节点,知识键(知识标识::图项源含义)).empty()) {
            if(!信息.定义.目标.含义图)现场.拒绝("旧目标含图项");
            for(auto 字段:{知识标识::项词,知识标识::项角色,知识标识::项子知识,知识标识::项子映射,知识标识::项指代定义})
                if(!现场.字段(项节点,知识键(字段)).empty())现场.拒绝("图项与旧项混写");
            新语言局部角色引用 角色{现场.原始值<std::int64_t>(项节点,知识标识::图项源含义),现场.原始值<std::int64_t>(项节点,知识标识::图项角色)};
            if(!现场.字段(项节点,知识键(知识标识::图项子知识)).empty()) {
                if(!现场.字段(项节点,知识键(知识标识::项方式)).empty())现场.拒绝("图子项混角色方式");
                新语言知识图子规则项 子{角色,现场.引用组(项节点,知识标识::图项子知识,1),{}};
                for(auto 映射:现场.引用组(项节点,知识标识::图项映射))子.映射组.push_back(读图映射(现场,映射,项节点,知识标识::图项映射));
                项.载荷=std::move(子);
            }else {
                if(!现场.字段(项节点,知识键(知识标识::图项映射)).empty())现场.拒绝("图角色项混映射");
                新语言知识图角色表达项 表达{角色,现场.枚举<新语言知识表达方式>(项节点,知识标识::项方式,6)};
                const auto 可选引用=[&](知识标识 键)->std::optional<稳定编码>{
                    if(现场.字段(项节点,知识键(键)).empty())return {};
                    return 现场.引用(项节点,键);
                };
                表达.指代概念节点=可选引用(知识标识::图项指代概念);表达.指代名称关系节点=可选引用(知识标识::图项指代名称);
                表达.概念前词条节点=可选引用(知识标识::图项概念前词);表达.概念后词条节点=可选引用(知识标识::图项概念后词);
                表达.查询词条节点=可选引用(知识标识::图项查询词);
                for(auto 端点:{表达.指代概念节点,表达.指代名称关系节点,表达.概念前词条节点,表达.概念后词条节点,表达.查询词条节点})
                    if(端点&&!现场.节点(*端点))return 返回未完成();
                项.载荷=std::move(表达);
            }
        } else if (!固定词.empty()) {
            if (!现场.字段(项节点, 知识键(知识标识::项角色)).empty()
                || !现场.字段(项节点, 知识键(知识标识::项方式)).empty()
                || !子规则.empty() || !现场.字段(项节点, 知识键(知识标识::项子映射)).empty()
                || !现场.字段(项节点, 知识键(知识标识::项指代定义)).empty()) { 现场.拒绝("知识项分支不能同时存在"); return 返回未完成(); }
            项.载荷 = 新语言知识固定词项{现场.引用(项节点, 知识标识::项词)};
        } else if (!子规则.empty()) {
            if (!允许子规则 || !现场.字段(项节点, 知识键(知识标识::项角色)).empty()
                || !现场.字段(项节点, 知识键(知识标识::项方式)).empty()
                || !现场.字段(项节点, 知识键(知识标识::项指代定义)).empty()
                || 子规则.size() != 1) { 现场.拒绝("子规则嵌套或项分支混用"); return 返回未完成(); }
            新语言知识子规则项 子{现场.引用(项节点, 知识标识::项子知识), {}};
            const auto 映射 = 现场.原始值<std::vector<std::int64_t>>(项节点, 知识标识::项子映射);
            if (映射.size() % 2 != 0) { 现场.拒绝("子规则角色映射物理长度不合法"); return 返回未完成(); }
            for (std::size_t i = 0; i < 映射.size(); i += 2) 子.映射组.push_back({映射[i], 映射[i + 1]});
            项.载荷 = std::move(子);
        } else {
            if (!现场.字段(项节点, 知识键(知识标识::项子映射)).empty()) { 现场.拒绝("角色项混入子规则映射"); return 返回未完成(); }
            新语言知识角色表达项 表达{现场.原始值<std::int64_t>(项节点, 知识标识::项角色),
                现场.枚举<新语言知识表达方式>(项节点, 知识标识::项方式, 4)};
            const auto 指代字段 = 现场.字段(项节点, 知识键(知识标识::项指代定义));
            if (表达.方式 == 新语言知识表达方式::已消解指代) {
                if (指代字段.size() != 1) { 现场.拒绝("指代项须有唯一正式定义引用"); return 返回未完成(); }
                表达.指代概念节点 = 现场.引用(项节点, 知识标识::项指代定义);
            } else if (!指代字段.empty()) {
                现场.拒绝("非指代项不能携带指代定义引用"); return 返回未完成();
            }
            项.载荷 = std::move(表达);
        }
        if(!std::holds_alternative<新语言知识图角色表达项>(项.载荷))
            for(auto 键:{知识标识::图项指代概念,知识标识::图项指代名称,知识标识::图项概念前词,知识标识::图项概念后词,知识标识::图项查询词})
                if(!现场.字段(项节点,知识键(键)).empty()){现场.拒绝("非图角色项混入查询材料");return 返回未完成();}
        信息.定义.结构.有序项组.push_back(std::move(项));
    }
    std::sort(信息.定义.结构.有序项组.begin(), 信息.定义.结构.有序项组.end(), [](const auto& 左, const auto& 右) { return 左.局部编号 < 右.局部编号; });
    for (std::size_t i = 0; i < 信息.定义.结构.有序项组.size(); ++i)
        if (信息.定义.结构.有序项组[i].局部编号 != static_cast<std::int64_t>(i)) { 现场.拒绝("保存的结构项编号不连续"); return 返回未完成(); }
    const auto 来源关联 = 全局基础数据集.查询字段引用节点结构(知识节点, 知识键(知识标识::关联知识));
    if (来源关联.状态 != 基础结构操作状态::已完成) { 现场.拒绝("来源关联全组读取未完成", 基础知识状态(来源关联.状态)); return 返回未完成(); }
    for (const auto& 关联 : 来源关联.来源节点组) {
        const auto 身份 = 关联.节点.编码;
        if (!现场.类型(身份, 知识标识::关联类型,
            {知识标识::关联知识, 知识标识::关联来源, 知识标识::关联起始, 知识标识::关联结束})
            || 现场.引用(身份, 知识标识::关联知识) != 知识节点) { 现场.拒绝("来源关联知识端点不一致"); return 返回未完成(); }
        新语言知识来源绑定 绑定{现场.引用(身份, 知识标识::关联来源),
            现场.原始值<std::int64_t>(身份, 知识标识::关联起始), 现场.原始值<std::int64_t>(身份, 知识标识::关联结束)};
        const auto 来源 = 读取知识来源细节已加锁(绑定.来源节点, 现场);
        if (来源.状态 != 知识状态::已完成 || !来源.信息) { 现场.拒绝("知识来源不可读", 来源.状态); return 返回未完成(); }
        std::int64_t 数量 = 0;
        if (!知识来源UTF8有效(来源.信息->固定原文, 数量)
            || 来源.信息->语言节点 != 信息.定义.结构.语言节点 || 绑定.起始标量 < 0
            || 绑定.结束标量 <= 绑定.起始标量 || 绑定.结束标量 > 数量) { 现场.拒绝("来源语言或标量范围不一致"); return 返回未完成(); }
        信息.来源组.push_back(绑定);
    }
    const auto 来源序 = [](const auto& 左, const auto& 右) { return std::tie(左.来源节点, 左.起始标量, 左.结束标量) < std::tie(右.来源节点, 右.起始标量, 右.结束标量); };
    std::sort(信息.来源组.begin(), 信息.来源组.end(), 来源序);
    if (std::adjacent_find(信息.来源组.begin(), 信息.来源组.end()) != 信息.来源组.end()) { 现场.拒绝("保存的来源关联重复"); return 返回未完成(); }
    if (现场.状态 != 知识状态::已完成) return 返回未完成();
    新语言知识定义 定稿;
    std::string 规范化诊断;
    if (规范化知识定义(信息.定义, 定稿, 规范化诊断) != 知识状态::已完成 || 定稿 != 信息.定义) {
        现场.拒绝("保存知识不是合法完整规范化定义"); return 返回未完成();
    }
    for (const auto& 项 : 信息.定义.结构.有序项组) {
        if(const auto* 图子=std::get_if<新语言知识图子规则项>(&项.载荷)) {
            for(auto 子身份:图子->允许子知识组) {
                if(子身份==知识节点){现场.拒绝("图知识不能自引用");return 返回未完成();}
                const auto 子读取=读取知识结构细节已加锁(子身份,现场,true);
                if(子读取.状态!=知识状态::已完成||!子读取.信息||!图子规则相容(信息.定义,*图子,子读取.信息->定义)) {
                    现场.拒绝("图子知识或全限定角色映射不相容");return 返回未完成();
                }
            }
            continue;
        }
        const auto* 子项 = std::get_if<新语言知识子规则项>(&项.载荷);
        if (!子项) continue;
        if (子项->子知识节点 == 知识节点) { 现场.拒绝("子规则不准自引用"); return 返回未完成(); }
        const auto 子读取 = 读取知识结构细节已加锁(子项->子知识节点, 现场, false);
        if (子读取.状态 != 知识状态::已完成 || !子读取.信息) {
            现场.拒绝("子规则完整定义不可读", 子读取.状态); return 返回未完成();
        }
        const auto& 子定义 = 子读取.信息->定义;
        if (!子知识符合归类陈述形状(信息.定义, 子定义)
            || !子规则角色映射完整(信息.定义, *子项, 子定义)) {
            现场.拒绝("子知识定义不相容或角色映射不完整"); return 返回未完成();
        }
    }
    结果.状态 = 知识状态::已完成;
    结果.信息 = std::move(信息);
    结果.阶段 = 新语言知识发布阶段::完成;
    return 结果;
}
新语言知识结果 读取知识结构已加锁(稳定编码 知识节点) {
    知识读取现场 现场;
    return 读取知识结构细节已加锁(知识节点, 现场);
}

} // namespace

namespace {

知识状态 读取语言知识候选已加锁(稳定编码 语言节点, 语言知识候选快照& 快照,
    std::string& 诊断, std::optional<std::size_t> 最大条目数 = std::nullopt) {
    const auto 模式 = 知识模式状态已加锁();
    if (模式 != 知识状态::已完成) { 诊断 = "语言知识schema未完整可用"; return 模式; }
    if (!读取语言信息(语言节点)) { 诊断 = "知识所属语言不可完整读取"; return 知识状态::引用不可读; }
    const auto 结构反查 = 全局基础数据集.查询字段引用节点结构(语言节点, 知识键(知识标识::结构语言));
    if (结构反查.状态 != 基础结构操作状态::已完成) { 诊断 = "知识语言全候选反查未完成"; return 基础知识状态(结构反查.状态); }
    知识读取现场 现场;
    std::set<稳定编码> 核心组;
    for (const auto& 结构 : 结构反查.来源节点组) {
        const auto 核心反查 = 全局基础数据集.查询字段引用节点结构(结构.节点.编码, 知识键(知识标识::核心结构));
        if (核心反查.状态 != 基础结构操作状态::已完成) { 诊断 = "结构所属知识反查未完成"; return 基础知识状态(核心反查.状态); }
        if (核心反查.引用字段组.size() != 1 || 核心反查.来源节点组.size() != 1
            || !核心组.insert(核心反查.来源节点组.front().节点.编码).second) {
            诊断 = "结构未唯一归属一个知识"; return 知识状态::内部不一致;
        }
    }
    // 反查本身返回完整身份组；在读取任一完整定义前拒绝超额请求，
    // 不把前缀候选伪装为该语言的全部知识。
    if (最大条目数 && 核心组.size() > *最大条目数) {
        诊断 = "语言知识候选数量超过请求额度";
        return 知识状态::额度不足;
    }
    语言知识候选快照 候选;
    for (const auto 核心 : 核心组) {
        const auto 读取 = 读取知识结构细节已加锁(核心, 现场);
        if (读取.状态 != 知识状态::已完成 || !读取.信息) { 诊断 = 读取.诊断说明; return 读取.状态; }
        if (读取.信息->定义.结构.语言节点 != 语言节点) { 诊断 = "候选结构语言端点不一致"; return 知识状态::内部不一致; }
        for (const auto& 已有 : 候选.信息组)
            if (已有.定义 == 读取.信息->定义) { 诊断 = "同规范化定义存在多个知识身份"; return 知识状态::内部不一致; }
        候选.信息组.push_back(*读取.信息);
    }
    if (!现场.节点(语言节点)) { 诊断 = 现场.诊断; return 现场.状态; }
    候选.结构组 = std::move(现场.结构组);
    快照 = std::move(候选);
    return 知识状态::已完成;
}

std::vector<稳定编码> 收集知识固定词(const 语言知识候选快照& 快照, const 新语言知识定义* 请求定义 = nullptr) {
    std::set<稳定编码> 词组;
    const auto 收集 = [&](const 新语言知识定义& 定义) {
        for (const auto& 项 : 定义.结构.有序项组) {
            if (const auto* 词 = std::get_if<新语言知识固定词项>(&项.载荷)) 词组.insert(词->词条节点);
            if(const auto* 表达=std::get_if<新语言知识图角色表达项>(&项.载荷))
                for(auto 词:{表达->概念前词条节点,表达->概念后词条节点,表达->查询词条节点})if(词)词组.insert(*词);
        }
    };
    for (const auto& 项 : 快照.信息组) 收集(项.定义);
    if (请求定义) 收集(*请求定义);
    return {词组.begin(), 词组.end()};
}

std::vector<稳定编码> 收集知识指代定义(const 语言知识候选快照& 快照,
    const 新语言知识定义* 请求定义 = nullptr) {
    std::set<稳定编码> 身份组;
    const auto 收集 = [&](const 新语言知识定义& 定义) {
        for (const auto& 项 : 定义.结构.有序项组) {
            if (const auto* 表达 = std::get_if<新语言知识角色表达项>(&项.载荷);
                表达 && 表达->指代概念节点) 身份组.insert(*表达->指代概念节点);
            if(const auto* 表达=std::get_if<新语言知识图角色表达项>(&项.载荷);表达&&表达->指代概念节点)身份组.insert(*表达->指代概念节点);
        }
    };
    for (const auto& 项 : 快照.信息组) 收集(项.定义);
    if (请求定义) 收集(*请求定义);
    return {身份组.begin(), 身份组.end()};
}

知识状态 读取知识指代端点快照(const std::vector<稳定编码>& 身份组,
    const 概念_存在类& 存在概念服务, 语言知识指代端点快照& 快照,
    std::string& 诊断) {
    语言知识指代端点快照 本次;
    for (const auto 身份 : 身份组) {
        const auto 读取 = 存在概念服务.获取指代概念(身份);
        if (读取.状态 != 指代概念查询状态::已找到 || !读取.信息) {
            诊断 = "指代定义正式端点不可完整读取";
            return 读取.状态 == 指代概念查询状态::资源失败 ? 知识状态::工程未完成
                : 读取.状态 == 指代概念查询状态::结构不一致 ? 知识状态::内部不一致
                : 知识状态::引用不可读;
        }
        if (!是作用域说话者定义(读取.信息->定义)&&!是作用域受话者定义(读取.信息->定义)
            && !(读取.信息->定义.规则==指代声明规则::作用域第三方
                &&读取.信息->定义.目标范围==指代目标范围::单个已登记强类型信息节点)) {
            诊断 = "指代定义不符合已支持的作用域规则";
            return 知识状态::引用不可读;
        }
        本次.emplace(身份, *读取.信息);
    }
    快照 = std::move(本次);
    return 知识状态::已完成;
}

知识状态 核图指代端点定义(const 语言知识候选快照& 知识,
    const 新语言知识定义* 请求定义,const 语言知识指代端点快照& 指代,
    std::string& 诊断) {
    const auto 核=[&](const 新语言知识定义& 定义) {
        for(const auto& 项:定义.结构.有序项组) {
            const auto* 表达=std::get_if<新语言知识图角色表达项>(&项.载荷);
            if(!表达||表达->方式!=新语言知识表达方式::已消解指代)continue;
            const auto p=表达->指代概念节点 ? 指代.find(*表达->指代概念节点) : 指代.end();
            // 图知识的已消解指代：作用域受话者（你）或作用域说话者（我，“Y要X”最小需求句式的主体）。
            if(p==指代.end()||(!是作用域受话者定义(p->second.定义)&&!是作用域说话者定义(p->second.定义)))return false;
        }
        return true;
    };
    for(const auto& 项:知识.信息组)if(!核(项.定义)) {
        诊断="行动请求图指代必须引用正式作用域受话者定义";return 知识状态::引用不可读;
    }
    if(请求定义&&!核(*请求定义)) {
        诊断="请求图指代不是正式作用域受话者定义";return 知识状态::输入拒绝;
    }
    return 知识状态::已完成;
}

知识状态 复核知识指代端点快照(const 语言知识指代端点快照& 原快照,
    const 概念_存在类& 存在概念服务, std::string& 诊断) {
    for (const auto& [身份, 原信息] : 原快照) {
        const auto 当前 = 存在概念服务.获取指代概念(身份);
        if (当前.状态 == 指代概念查询状态::资源失败) {
            诊断 = "指代定义复读资源失败"; return 知识状态::工程未完成;
        }
        if (当前.状态 != 指代概念查询状态::已找到 || !当前.信息
            || *当前.信息 != 原信息) {
            诊断 = "指代定义正式端点读取漂移"; return 知识状态::读取漂移;
        }
    }
    return 知识状态::已完成;
}

知识状态 读取知识词端点快照(const std::vector<稳定编码>& 词条组, 稳定编码 语言节点,
    const 新_词类& 词服务, std::vector<语言知识词端点快照>& 快照组, std::string& 诊断) {
    std::vector<语言知识词端点快照> 候选;
    for (const auto 词条 : 词条组) {
        const auto 前 = 词服务.获取词条(词条);
        if (前.状态 != 新词条查询状态::已找到 || !前.信息) {
            诊断 = "固定词正式端点未完整读取";
            return 前.状态 == 新词条查询状态::资源失败 ? 知识状态::工程未完成
                : (前.状态 == 新词条查询状态::结构不一致 ? 知识状态::内部不一致 : 知识状态::引用不可读);
        }
        if (前.信息->语言节点 != 语言节点) {
            诊断 = "固定词语言或规范化合同不相容"; return 知识状态::输入拒绝;
        }
        const auto 基础 = 全局基础数据集.查询完整节点结构(词条);
        if (基础.状态 != 基础结构操作状态::已完成) { 诊断 = "固定词基础结构未完整读取"; return 基础知识状态(基础.状态); }
        if (!基础.结构 || 基础.结构->节点.编码 != 词条) { 诊断 = "固定词基础身份不一致"; return 知识状态::内部不一致; }
        const auto 后 = 词服务.获取词条(词条);
        if (后.状态 != 新词条查询状态::已找到 || !后.信息) {
            诊断 = "固定词正式端点复读未完成";
            return 后.状态 == 新词条查询状态::资源失败 ? 知识状态::工程未完成
                : (后.状态 == 新词条查询状态::结构不一致 ? 知识状态::内部不一致 : 知识状态::引用不可读);
        }
        if (*前.信息 != *后.信息) { 诊断 = "固定词正式信息读取漂移"; return 知识状态::读取漂移; }
        候选.push_back({*后.信息, *基础.结构});
    }
    快照组 = std::move(候选);
    return 知识状态::已完成;
}
知识状态 复核知识准备已加锁(稳定编码 语言节点, const 语言知识候选快照& 原快照,
    const std::vector<语言知识词端点快照>& 词快照组, 语言知识候选快照& 当前快照,
    std::string& 诊断, std::optional<std::size_t> 最大条目数 = std::nullopt) {
    auto 状态 = 读取语言知识候选已加锁(语言节点, 当前快照, 诊断, 最大条目数);
    if (状态 != 知识状态::已完成) return 状态;
    bool 相等 = 原快照.信息组.size() == 当前快照.信息组.size()
        && 原快照.结构组.size() == 当前快照.结构组.size();
    if (相等) {
        相等 = std::equal(原快照.信息组.begin(), 原快照.信息组.end(), 当前快照.信息组.begin(), 知识信息相等)
            && std::equal(原快照.结构组.begin(), 原快照.结构组.end(), 当前快照.结构组.begin(),
                [](const auto& 左, const auto& 右) { return 左.first == 右.first && 基础结构相等(左.second, 右.second); });
    }
    if (!相等) { 诊断 = "知识全候选范围、定义或来源读取漂移"; return 知识状态::读取漂移; }
    for (const auto& 词 : 词快照组) {
        const auto 当前 = 全局基础数据集.查询完整节点结构(词.信息.节点);
        if (当前.状态 != 基础结构操作状态::已完成) { 诊断 = "固定词最终结构复核未完成"; return 基础知识状态(当前.状态); }
        if (!当前.结构 || !基础结构相等(*当前.结构, 词.结构)) { 诊断 = "固定词完整基础结构读取漂移"; return 知识状态::读取漂移; }
    }
    return 知识状态::已完成;
}

std::vector<基础组内新节点> 形成缺少来源节点组(稳定编码 已有知识,
    const std::vector<新语言知识来源绑定>& 缺少来源) {
    std::vector<基础组内新节点> 组;
    for (const auto& 来源 : 缺少来源) {
        组.push_back({{{节点类型字段, 知识键(知识标识::关联类型)},
            {知识键(知识标识::关联知识), 已有知识}, {知识键(知识标识::关联来源), 来源.来源节点},
            {知识键(知识标识::关联起始), 基础值{来源.起始标量}},
            {知识键(知识标识::关联结束), 基础值{来源.结束标量}}}});
    }
    return 组;
}
void 追加固定含义图(std::vector<基础组内新节点>& 组, std::size_t 目标,
    const 新语言知识含义图& 图) {
    const auto 新节点 = [&](知识标识 类型) {
        const auto 索引 = 组.size();
        组.push_back({{{节点类型字段, 知识键(类型)}}});
        return 索引;
    };
    const auto 加字段 = [&](std::size_t 索引, 知识标识 键, 基础组内字段内容 值) {
        组[索引].字段组.push_back({知识键(键), std::move(值)});
    };
    const auto 加整数 = [&](std::size_t 索引, 知识标识 键, auto 值) { 加字段(索引, 键, 基础值{static_cast<std::int64_t>(值)}); };
    const auto 加图映射=[&](std::size_t 宿主,知识标识 成员键,const 新语言子含义映射& 映射) {
        const auto 身份=新节点(知识标识::图映射类型);
        加字段(宿主,成员键,基础组内节点引用{身份});加字段(身份,知识标识::图映射宿主,基础组内节点引用{宿主});
        加整数(身份,知识标识::图映射源含义,映射.来源.含义编号);加整数(身份,知识标识::图映射源角色,映射.来源.角色编号);
        加整数(身份,知识标识::图映射目标含义,映射.目标.含义编号);加整数(身份,知识标识::图映射目标角色,映射.目标.角色编号);
    };
    加整数(目标,知识标识::目标图格式,1);加整数(目标,知识标识::目标图根编号,图.根含义编号);
    for(const auto& 节点:图.节点组) {
        const auto 身份=新节点(知识标识::图含义类型);
        加字段(目标,知识标识::目标图成员,基础组内节点引用{身份});加字段(身份,知识标识::图含义宿主,基础组内节点引用{目标});
        加整数(身份,知识标识::图含义编号,节点.局部编号);加整数(身份,知识标识::图含义操作,节点.操作);加整数(身份,知识标识::图含义作用域,节点.作用域);
        if(节点.特征值分支)加整数(身份,知识标识::图含义分支,*节点.特征值分支);
        if(节点.确认范围)加整数(身份,知识标识::图含义确认,*节点.确认范围);
        if(节点.方向)加整数(身份,知识标识::图含义方向,*节点.方向);
        for(const auto& 角色:节点.角色组) {
            const auto 角色身份=新节点(知识标识::角色类型),约束身份=新节点(知识标识::约束类型);
            加字段(身份,知识标识::图含义角色,基础组内节点引用{角色身份});加字段(角色身份,知识标识::图角色宿主,基础组内节点引用{身份});
            加整数(角色身份,知识标识::角色编号,角色.局部编号);加整数(角色身份,知识标识::角色种类,角色.种类);加整数(角色身份,知识标识::角色数量,角色.数量);
            加字段(角色身份,知识标识::角色约束,基础组内节点引用{约束身份});加整数(约束身份,知识标识::约束角色,角色.局部编号);
            std::vector<std::int64_t> 类别组;for(auto 类别:角色.必要约束组.front().允许类别组)类别组.push_back(static_cast<std::int64_t>(类别));
            加字段(约束身份,知识标识::约束类别,基础值{std::move(类别组)});
        }
        for(auto 参数:节点.参数组) {
            const auto 参数身份=新节点(知识标识::参数类型);
            加字段(身份,知识标识::图含义参数,基础组内节点引用{参数身份});加字段(参数身份,知识标识::图参数宿主,基础组内节点引用{身份});
            加整数(参数身份,知识标识::参数名,参数.参数);加整数(参数身份,知识标识::参数角色,参数.规则角色);
        }
        for(const auto& 边:节点.子含义组) {
            const auto 边身份=新节点(知识标识::图子边类型);加字段(边身份,知识标识::图子边宿主,基础组内节点引用{身份});
            加整数(边身份,知识标识::图子边角色,边.本节点角色编号);加整数(边身份,知识标识::图子边用途,边.用途);加整数(边身份,知识标识::图子边目标编号,边.子含义编号);
            for(const auto& 映射:边.显式映射组)加图映射(边身份,知识标识::图子边映射,映射);
        }
    }
}

std::vector<基础组内新节点> 形成知识完整节点组(const 新语言知识发布请求& 请求) {
    std::vector<基础组内新节点> 组;
    const auto 新节点 = [&](知识标识 类型) {
        const auto 索引 = 组.size();
        组.push_back({{{节点类型字段, 知识键(类型)}}});
        return 索引;
    };
    const auto 加字段 = [&](std::size_t 索引, 知识标识 键, 基础组内字段内容 值) {
        组[索引].字段组.push_back({知识键(键), std::move(值)});
    };
    const auto 加整数 = [&](std::size_t 索引, 知识标识 键, auto 值) { 加字段(索引, 键, 基础值{static_cast<std::int64_t>(值)}); };
    const auto 核心 = 新节点(知识标识::知识类型), 目标 = 新节点(知识标识::目标类型), 结构 = 新节点(知识标识::结构类型);
    加字段(核心, 知识标识::核心目标, 基础组内节点引用{目标});
    加字段(核心, 知识标识::核心结构, 基础组内节点引用{结构});
    加整数(目标, 知识标识::目标目的, 请求.定义.目标.目的);
    const auto 加图映射=[&](std::size_t 宿主,知识标识 成员键,const 新语言子含义映射& 映射) {
        const auto 身份=新节点(知识标识::图映射类型);
        加字段(宿主,成员键,基础组内节点引用{身份});加字段(身份,知识标识::图映射宿主,基础组内节点引用{宿主});
        加整数(身份,知识标识::图映射源含义,映射.来源.含义编号);加整数(身份,知识标识::图映射源角色,映射.来源.角色编号);
        加整数(身份,知识标识::图映射目标含义,映射.目标.含义编号);加整数(身份,知识标识::图映射目标角色,映射.目标.角色编号);
    };
    if(请求.定义.目标.含义图) {
        追加固定含义图(组, 目标, *请求.定义.目标.含义图);
    }else {
    for (const auto& 角色 : 请求.定义.目标.角色组) {
        const auto 角色节点 = 新节点(知识标识::角色类型), 约束节点 = 新节点(知识标识::约束类型);
        加字段(目标, 知识标识::目标角色, 基础组内节点引用{角色节点});
        加整数(角色节点, 知识标识::角色编号, 角色.局部编号);
        加整数(角色节点, 知识标识::角色种类, 角色.种类);
        加整数(角色节点, 知识标识::角色数量, 角色.数量);
        加字段(角色节点, 知识标识::角色约束, 基础组内节点引用{约束节点});
        const auto& 约束 = 角色.必要约束组.front();
        加整数(约束节点, 知识标识::约束角色, 约束.被约束角色);
        std::vector<std::int64_t> 类别组;
        for (const auto 值 : 约束.允许类别组) 类别组.push_back(static_cast<std::int64_t>(值));
        加字段(约束节点, 知识标识::约束类别, 基础值{std::move(类别组)});
    }
    const auto 含义节点 = 新节点(知识标识::含义类型);
    const auto& 含义 = 请求.定义.目标.含义组.front();
    加字段(目标, 知识标识::目标含义, 基础组内节点引用{含义节点});
    加整数(含义节点, 知识标识::含义编号, 含义.局部编号);
    加整数(含义节点, 知识标识::含义操作, 含义.操作);
    加整数(含义节点, 知识标识::含义作用域, 含义.作用域);
    if (含义.特征值分支) 加整数(含义节点, 知识标识::含义分支, *含义.特征值分支);
    if (含义.确认范围) 加整数(含义节点, 知识标识::含义确认范围, *含义.确认范围);
    if (含义.被询含义) {
        const auto 子节点 = 新节点(知识标识::含义类型);
        加字段(含义节点, 知识标识::含义子项, 基础组内节点引用{子节点});
        加整数(子节点, 知识标识::含义编号, 含义.被询含义->局部编号);
        加整数(子节点, 知识标识::含义操作, 含义.被询含义->操作);
        加整数(子节点, 知识标识::含义作用域, 含义.被询含义->作用域);
        if (含义.被询含义->特征值分支)
            加整数(子节点, 知识标识::含义分支, *含义.被询含义->特征值分支);
        for (const auto& 参数 : 含义.被询含义->参数组) {
            const auto 参数节点 = 新节点(知识标识::参数类型);
            加字段(子节点, 知识标识::含义参数, 基础组内节点引用{参数节点});
            加整数(参数节点, 知识标识::参数名, 参数.参数);
            加整数(参数节点, 知识标识::参数角色, 参数.规则角色);
        }
    }
    for (const auto& 参数 : 含义.参数组) {
        const auto 参数节点 = 新节点(知识标识::参数类型);
        加字段(含义节点, 知识标识::含义参数, 基础组内节点引用{参数节点});
        加整数(参数节点, 知识标识::参数名, 参数.参数);
        加整数(参数节点, 知识标识::参数角色, 参数.规则角色);
    }
    }
    加整数(结构, 知识标识::结构类别, 请求.定义.结构.类别);
    加字段(结构, 知识标识::结构语言, 请求.定义.结构.语言节点);
    for (const auto& 项 : 请求.定义.结构.有序项组) {
        const auto 项节点 = 新节点(知识标识::项类型);
        加字段(结构, 知识标识::结构项, 基础组内节点引用{项节点});
        加整数(项节点, 知识标识::项编号, 项.局部编号);
        if (const auto* 词 = std::get_if<新语言知识固定词项>(&项.载荷)) 加字段(项节点, 知识标识::项词, 词->词条节点);
        else if (const auto* 表达 = std::get_if<新语言知识角色表达项>(&项.载荷)) {
            加整数(项节点, 知识标识::项角色, 表达->规则角色);
            加整数(项节点, 知识标识::项方式, 表达->方式);
            if (表达->指代概念节点)
                加字段(项节点, 知识标识::项指代定义, *表达->指代概念节点);
        } else if(const auto* 图角色=std::get_if<新语言知识图角色表达项>(&项.载荷)) {
            加整数(项节点,知识标识::图项源含义,图角色->角色.含义编号);加整数(项节点,知识标识::图项角色,图角色->角色.角色编号);
            加整数(项节点,知识标识::项方式,图角色->方式);
            for(const auto& [键,值]:std::initializer_list<std::pair<知识标识,std::optional<稳定编码>>>{
                {知识标识::图项指代概念,图角色->指代概念节点},{知识标识::图项指代名称,图角色->指代名称关系节点},
                {知识标识::图项概念前词,图角色->概念前词条节点},{知识标识::图项概念后词,图角色->概念后词条节点},
                {知识标识::图项查询词,图角色->查询词条节点}})if(值)加字段(项节点,键,*值);
        } else if(const auto* 图子=std::get_if<新语言知识图子规则项>(&项.载荷)) {
            加整数(项节点,知识标识::图项源含义,图子->子角色.含义编号);加整数(项节点,知识标识::图项角色,图子->子角色.角色编号);
            for(auto 子知识:图子->允许子知识组)加字段(项节点,知识标识::图项子知识,子知识);
            for(const auto& 映射:图子->映射组)加图映射(项节点,知识标识::图项映射,映射);
        } else {
            const auto& 子规则 = std::get<新语言知识子规则项>(项.载荷);
            加字段(项节点, 知识标识::项子知识, 子规则.子知识节点);
            std::vector<std::int64_t> 映射;
            for (const auto& 一对 : 子规则.映射组) {
                映射.push_back(一对.外层角色编号);
                映射.push_back(一对.子规则角色编号);
            }
            加字段(项节点, 知识标识::项子映射, 基础值{std::move(映射)});
        }
    }
    for (const auto& 来源 : 请求.来源组) {
        const auto 关联 = 新节点(知识标识::关联类型);
        加字段(关联, 知识标识::关联知识, 基础组内节点引用{核心});
        加字段(关联, 知识标识::关联来源, 来源.来源节点);
        加整数(关联, 知识标识::关联起始, 来源.起始标量);
        加整数(关联, 知识标识::关联结束, 来源.结束标量);
    }
    return 组;
}

} // namespace

新语言知识状态 新_语言类::初始化语言知识结构() {
    std::lock_guard 锁(新语言互斥);
    const auto 当前 = 知识模式状态已加锁();
    if (当前 != 知识状态::尚未初始化) return 当前;
    if (!结构仍存在()) return 知识状态::尚未初始化;
    const bool 待核为空 = std::all_of(待核知识句柄.begin(), 待核知识句柄.end(), [](auto 值) { return !有效(值); });
    if (待核为空) {
        const auto 发布 = 全局基础数据集.原子建立节点组(std::vector<基础组内新节点>(知识句柄数));
        if (发布.状态 != 基础结构操作状态::已完成) return 基础知识状态(发布.状态);
        if (发布.节点组.size() != 知识句柄数) return 知识状态::内部不一致;
        std::copy(发布.节点组.begin(), 发布.节点组.end(), 待核知识句柄.begin());
    }
    std::set<稳定编码> 去重;
    for (const auto 句柄 : 待核知识句柄) {
        if (!有效(句柄) || !去重.insert(句柄).second) return 知识状态::内部不一致;
        const auto 读取 = 全局基础数据集.查询完整节点结构(句柄);
        if (读取.状态 != 基础结构操作状态::已完成) return 基础知识状态(读取.状态);
        if (!读取.结构 || 读取.结构->节点.编码 != 句柄 || !读取.结构->字段组.empty()) return 知识状态::内部不一致;
    }
    知识句柄 = 待核知识句柄;
    待核知识句柄 = {};
    return 知识状态::已完成;
}

新语言知识来源结果 新_语言类::获取知识先天来源(稳定编码 来源节点) const {
    新语言知识来源结果 结果;
    if (!有效(来源节点)) return 结果;
    std::lock_guard 锁(新语言互斥);
    结果.状态 = 知识模式状态已加锁();
    if (结果.状态 != 知识状态::已完成) return 结果;
    const auto 存在 = 全局基础数据集.查询完整节点结构(来源节点);
    if (存在.状态 != 基础结构操作状态::已完成) {
        结果.状态 = 存在.状态 == 基础结构操作状态::节点不存在 ? 知识状态::未找到 : 基础知识状态(存在.状态);
        return 结果;
    }
    return 读取知识来源已加锁(来源节点);
}

新语言知识来源保护结果 新_语言类::读取知识先天来源保护(稳定编码 来源节点) const {
    using S = 新语言知识来源保护状态;
    try {
        if (!有效(来源节点)) return {};
        std::lock_guard 锁(新语言互斥);
        if (std::all_of(知识句柄.begin(), 知识句柄.end(), [](auto 值) { return !有效(值); }))
            return {S::尚未初始化, {}, {}, {}};
        const auto 源读 = 全局基础数据集.查询完整节点结构(来源节点);
        if (源读.状态 == 基础结构操作状态::资源失败) return {S::资源失败, {}, {}, {}};
        if (源读.状态 == 基础结构操作状态::节点不存在) return {S::来源不存在, {}, {}, {}};
        if (源读.状态 != 基础结构操作状态::已完成 || !源读.结构)
            throw std::logic_error("language source identity read failed");
        知识读取现场 现场;
        现场.结构组.emplace(来源节点, *源读.结构);
        // 模式核验依赖也必须保留；不能只保护来源这一单节点。
        for (const auto 键 : 知识句柄) {
            if (!有效(键)) throw std::logic_error("language source schema incomplete");
            if (!现场.节点(键)) break;
        }
        if (现场.状态 == 知识状态::工程未完成) return {S::资源失败, {}, {}, {}};
        if (现场.状态 != 知识状态::已完成)
            throw std::logic_error("language source schema unreadable");
        const auto 模式 = 知识模式状态已加锁();
        if (模式 == 知识状态::工程未完成) return {S::资源失败, {}, {}, {}};
        if (模式 != 知识状态::已完成) throw std::logic_error("language source schema corrupt");
        const auto 来源 = 读取知识来源细节已加锁(来源节点, 现场);
        if (来源.状态 == 知识状态::工程未完成) return {S::资源失败, {}, {}, {}};
        if (来源.状态 != 知识状态::已完成 || !来源.信息)
            throw std::logic_error("language source content corrupt");
        const auto 语言 = 读取语言信息(来源.信息->语言节点);
        if (!语言) throw std::logic_error("language source language identity corrupt");
        for (const auto 节点 : {自然语言组织根节点, 语言组织根节点, 节点类型字段,
                 自然语言组织根类型, 语言组织根类型, 语言节点类型, 语言登记键字段, 创建来源字段})
            if (!现场.节点(节点)) break;
        if (语言->创建来源节点) (void)现场.节点(*语言->创建来源节点);
        if (现场.状态 == 知识状态::工程未完成) return {S::资源失败, {}, {}, {}};
        if (现场.状态 != 知识状态::已完成)
            throw std::logic_error("language source endpoint closure corrupt");
        std::vector<稳定编码> 节点组;
        for (const auto& [节点, _] : 现场.结构组) 节点组.push_back(节点);
        const auto 联合 = 全局基础数据集.查询节点组完整结构(节点组);
        if (联合.状态 == 基础结构操作状态::资源失败) return {S::资源失败, {}, {}, {}};
        if (联合.状态 != 基础结构操作状态::已完成 || 联合.节点组.size() != 节点组.size())
            throw std::logic_error("language source complete protection read failed");
        新语言知识来源保护结果 结果{S::已读取, 来源.信息, 语言, {}};
        for (const auto& 项 : 联合.节点组) {
            if (!项.快照 || !基础结构相等(现场.结构组.at(项.查询节点), 项.快照->结构))
                return {S::前提已变化, {}, {}, {}};
            结果.完整保护.push_back(*项.快照);
        }
        return 结果;
    } catch (const std::bad_alloc&) { return {S::资源失败, {}, {}, {}}; }
    catch (const std::length_error&) { return {S::资源失败, {}, {}, {}}; }
}

新语言知识来源结果 新_语言类::查询知识先天来源(const 新语言知识来源请求& 请求) const {
    新语言知识来源结果 结果;
    std::int64_t 标量数 = 0;
    if (请求.种类 != 新语言知识来源种类::经确认先天定义 || !有效(请求.语言节点)
        || !知识来源UTF8有效(请求.固定原文, 标量数)) return 结果;
    std::lock_guard 锁(新语言互斥);
    结果.状态 = 知识模式状态已加锁();
    if (结果.状态 != 知识状态::已完成) return 结果;
    if (!读取语言信息(请求.语言节点)) { 结果.状态 = 知识状态::引用不可读; return 结果; }
    const auto 全组 = 全局基础数据集.查询字段引用节点结构(请求.语言节点, 知识键(知识标识::来源语言));
    if (全组.状态 != 基础结构操作状态::已完成) { 结果.状态 = 基础知识状态(全组.状态); return 结果; }
    std::optional<新语言知识来源信息> 匹配;
    for (const auto& 来源 : 全组.来源节点组) {
        const auto 读取 = 读取知识来源已加锁(来源.节点.编码);
        if (读取.状态 != 知识状态::已完成 || !读取.信息) { 结果.状态 = 读取.状态; return 结果; }
        if (读取.信息->种类 == 请求.种类 && 读取.信息->语言节点 == 请求.语言节点 && 读取.信息->固定原文 == 请求.固定原文) {
            if (匹配) { 结果.状态 = 知识状态::内部不一致; return 结果; }
            匹配 = *读取.信息;
        }
    }
    结果.状态 = 匹配 ? 知识状态::已完成 : 知识状态::未找到;
    结果.信息 = std::move(匹配);
    return 结果;
}

新语言知识来源结果 新_语言类::建立或取得知识先天来源(const 新语言知识来源请求& 请求) {
    新语言知识来源结果 结果;
    std::int64_t 标量数 = 0;
    if (请求.种类 != 新语言知识来源种类::经确认先天定义 || !有效(请求.语言节点)
        || !知识来源UTF8有效(请求.固定原文, 标量数)) return 结果;
    std::lock_guard 锁(新语言互斥);
    结果.状态 = 知识模式状态已加锁();
    if (结果.状态 != 知识状态::已完成) return 结果;
    if (!读取语言信息(请求.语言节点)) { 结果.状态 = 知识状态::引用不可读; return 结果; }
    const auto 全组 = 全局基础数据集.查询字段引用节点结构(请求.语言节点, 知识键(知识标识::来源语言));
    if (全组.状态 != 基础结构操作状态::已完成) { 结果.状态 = 基础知识状态(全组.状态); return 结果; }
    std::optional<新语言知识来源信息> 匹配;
    for (const auto& 来源 : 全组.来源节点组) {
        const auto 读取 = 读取知识来源已加锁(来源.节点.编码);
        if (读取.状态 != 知识状态::已完成 || !读取.信息) { 结果.状态 = 读取.状态; return 结果; }
        if (读取.信息->种类 == 请求.种类 && 读取.信息->语言节点 == 请求.语言节点 && 读取.信息->固定原文 == 请求.固定原文) {
            if (匹配) { 结果.状态 = 知识状态::内部不一致; return 结果; }
            匹配 = *读取.信息;
        }
    }
    if (匹配) { 结果.信息 = std::move(匹配); 结果.发布种类 = 新语言知识发布种类::已复用; 结果.状态 = 知识状态::已完成; return 结果; }
    const auto 发布 = 全局基础数据集.原子建立节点组({{{
        {节点类型字段, 知识键(知识标识::来源类型)},
        {知识键(知识标识::来源种类), 基础值{static_cast<std::int64_t>(请求.种类)}},
        {知识键(知识标识::来源语言), 请求.语言节点}, {知识键(知识标识::来源原文), 基础值{请求.固定原文}}}}});
    if (发布.状态 != 基础结构操作状态::已完成) { 结果.状态 = 基础知识状态(发布.状态); return 结果; }
    if (发布.节点组.size() != 1) { 结果.状态 = 知识状态::内部不一致; return 结果; }
    结果 = 读取知识来源已加锁(发布.节点组.front());
    if (结果.状态 == 知识状态::已完成) 结果.发布种类 = 新语言知识发布种类::已建立;
    return 结果;
}

namespace {

新语言知识结果 读取知识请求(const 新语言知识定义* 定义, std::optional<稳定编码> 指定,
    const 新_词类& 词服务, const 概念_存在类& 存在概念服务) {
    新语言知识结果 结果;
    结果.阶段 = 新语言知识发布阶段::完整读回;
    稳定编码 语言节点{};
    语言知识候选快照 原快照;
    {
        std::lock_guard 锁(新语言互斥);
        结果.状态 = 知识模式状态已加锁();
        if (结果.状态 != 知识状态::已完成) return 结果;
        if (指定) {
            const auto 存在 = 全局基础数据集.查询完整节点结构(*指定);
            if (存在.状态 != 基础结构操作状态::已完成) {
                结果.状态 = 存在.状态 == 基础结构操作状态::节点不存在 ? 知识状态::未找到 : 基础知识状态(存在.状态);
                return 结果;
            }
            const auto 读取 = 读取知识结构已加锁(*指定);
            if (读取.状态 != 知识状态::已完成 || !读取.信息) return 读取;
            语言节点 = 读取.信息->定义.结构.语言节点;
        } else {
            if (!定义) throw std::logic_error("知识读取请求缺少固定身份或定义");
            语言节点 = 定义->结构.语言节点;
        }
        结果.状态 = 读取语言知识候选已加锁(语言节点, 原快照, 结果.诊断说明);
        if (结果.状态 != 知识状态::已完成) return 结果;
    }
    std::vector<语言知识词端点快照> 词快照;
    结果.状态 = 读取知识词端点快照(收集知识固定词(原快照, 定义), 语言节点, 词服务, 词快照, 结果.诊断说明);
    if (结果.状态 != 知识状态::已完成) return 结果;
    语言知识指代端点快照 指代快照;
    结果.状态 = 读取知识指代端点快照(收集知识指代定义(原快照, 定义),
        存在概念服务, 指代快照, 结果.诊断说明);
    if (结果.状态 != 知识状态::已完成) return 结果;
    结果.状态=核图指代端点定义(原快照,定义,指代快照,结果.诊断说明);
    if(结果.状态!=知识状态::已完成)return 结果;
    结果.状态 = 复核知识指代端点快照(指代快照, 存在概念服务, 结果.诊断说明);
    if (结果.状态 != 知识状态::已完成) return 结果;
    std::unique_lock 锁(新语言互斥);
    语言知识候选快照 当前快照;
    结果.状态 = 复核知识准备已加锁(语言节点, 原快照, 词快照, 当前快照, 结果.诊断说明);
    if (结果.状态 != 知识状态::已完成) return 结果;
    for (const auto& 信息 : 当前快照.信息组) {
        if ((指定 && 信息.节点 == *指定) || (!指定 && 信息.定义 == *定义)) {
            if (结果.信息) { 结果.信息.reset(); 结果.状态 = 知识状态::内部不一致; 结果.诊断说明 = "知识读取具有重复完整匹配"; return 结果; }
            结果.信息 = 信息;
        }
    }
    结果.状态 = 结果.信息 ? 知识状态::已完成 : 知识状态::未找到;
    if (结果.信息) 结果.阶段 = 新语言知识发布阶段::完成;
    锁.unlock();
    const auto 端点状态 = 复核知识指代端点快照(指代快照, 存在概念服务, 结果.诊断说明);
    if (端点状态 != 知识状态::已完成) {
        结果.状态 = 端点状态;
        结果.信息.reset();
        结果.阶段 = 新语言知识发布阶段::完整读回;
    }
    return 结果;
}

} // namespace

新语言知识结果 新_语言类::查询语言知识(const 新语言知识定义& 定义,
    const 新_词类& 词服务, const 概念_存在类& 存在概念服务) const {
    新语言知识结果 结果;
    新语言知识定义 定稿;
    结果.状态 = 规范化知识定义(定义, 定稿, 结果.诊断说明);
    if (结果.状态 != 知识状态::已完成) return 结果;
    return 读取知识请求(&定稿, std::nullopt, 词服务, 存在概念服务);
}

新语言知识结果 新_语言类::获取语言知识(稳定编码 知识节点,
    const 新_词类& 词服务, const 概念_存在类& 存在概念服务) const {
    if (!有效(知识节点)) return {};
    return 读取知识请求(nullptr, 知识节点, 词服务, 存在概念服务);
}

新语言知识候选列表结果 新_语言类::枚举语言知识候选(
    稳定编码 语言节点, std::size_t 最大条目数,
    const 新_词类& 词服务, const 概念_存在类& 存在概念服务) const {
    新语言知识候选列表结果 结果;
    if (!有效(语言节点) || 最大条目数 == 0) return 结果;
    语言知识候选快照 原快照;
    std::string 诊断;
    {
        std::lock_guard 锁(新语言互斥);
        结果.状态 = 读取语言知识候选已加锁(
            语言节点, 原快照, 诊断, 最大条目数);
        if (结果.状态 != 知识状态::已完成) return 结果;
    }
    std::vector<语言知识词端点快照> 词快照;
    结果.状态 = 读取知识词端点快照(
        收集知识固定词(原快照), 语言节点, 词服务, 词快照, 诊断);
    if (结果.状态 != 知识状态::已完成) return 结果;
    语言知识指代端点快照 指代快照;
    结果.状态 = 读取知识指代端点快照(收集知识指代定义(原快照),
        存在概念服务, 指代快照, 诊断);
    if (结果.状态 != 知识状态::已完成) return 结果;
    结果.状态=核图指代端点定义(原快照,nullptr,指代快照,诊断);
    if(结果.状态!=知识状态::已完成)return 结果;
    结果.状态 = 复核知识指代端点快照(指代快照, 存在概念服务, 诊断);
    if (结果.状态 != 知识状态::已完成) return 结果;
    std::unique_lock 锁(新语言互斥);
    语言知识候选快照 当前快照;
    结果.状态 = 复核知识准备已加锁(
        语言节点, 原快照, 词快照, 当前快照, 诊断, 最大条目数);
    if (结果.状态 != 知识状态::已完成) return 结果;
    std::sort(当前快照.信息组.begin(), 当前快照.信息组.end(),
        [](const auto& 左, const auto& 右) { return 左.节点 < 右.节点; });
    结果.信息组 = std::move(当前快照.信息组);
    锁.unlock();
    结果.状态 = 复核知识指代端点快照(指代快照, 存在概念服务, 诊断);
    if (结果.状态 != 知识状态::已完成) 结果.信息组.clear();
    return 结果;
}

新语言知识结果 新_语言类::建立或取得语言知识(const 新语言知识发布请求& 请求,
    const 新_词类& 词服务, const 概念_存在类& 存在概念服务) {
    新语言知识结果 结果;
    新语言知识发布请求 定稿请求;
    结果.状态 = 规范化知识定义(请求.定义, 定稿请求.定义, 结果.诊断说明);
    if (结果.状态 != 知识状态::已完成) return 结果;
    定稿请求.来源组 = 请求.来源组;
    std::sort(定稿请求.来源组.begin(), 定稿请求.来源组.end(), [](const auto& 左, const auto& 右) {
        return std::tie(左.来源节点, 左.起始标量, 左.结束标量) < std::tie(右.来源节点, 右.起始标量, 右.结束标量);
    });
    定稿请求.来源组.erase(std::unique(定稿请求.来源组.begin(), 定稿请求.来源组.end()), 定稿请求.来源组.end());
    const auto 语言节点 = 定稿请求.定义.结构.语言节点;
    语言知识候选快照 原快照;
    std::map<稳定编码, 基础节点结构> 请求来源快照;
    {
        std::lock_guard 锁(新语言互斥);
        结果.状态 = 读取语言知识候选已加锁(语言节点, 原快照, 结果.诊断说明);
        if (结果.状态 != 知识状态::已完成) return 结果;
        if(定稿请求.定义.目标.含义图)for(const auto& 项:定稿请求.定义.结构.有序项组)
            if(const auto* 子项=std::get_if<新语言知识图子规则项>(&项.载荷))for(auto 子身份:子项->允许子知识组) {
                const auto 子=std::find_if(原快照.信息组.begin(),原快照.信息组.end(),[&](const auto& 信息){return 信息.节点==子身份;});
                if(子==原快照.信息组.end()){结果.状态=知识状态::引用不可读;结果.诊断说明="图子知识尚未正式发布";return 结果;}
                if(!图子规则相容(定稿请求.定义,*子项,子->定义)){结果.状态=知识状态::输入拒绝;结果.诊断说明="图子知识拓扑或映射不相容";return 结果;}
            }
        if (定稿请求.定义.目标.目的 == 新语言知识表达目的::提问
            || 定稿请求.定义.目标.目的 == 新语言知识表达目的::提出需求) {
            for (const auto& 项 : 定稿请求.定义.结构.有序项组) {
                const auto* 子规则 = std::get_if<新语言知识子规则项>(&项.载荷);
                if (!子规则) continue;
                const auto 子 = std::find_if(原快照.信息组.begin(), 原快照.信息组.end(),
                    [&](const auto& 信息) { return 信息.节点 == 子规则->子知识节点; });
                if (子 == 原快照.信息组.end()) {
                    结果.状态 = 知识状态::引用不可读; 结果.诊断说明 = "子知识尚未正式发布"; return 结果;
                }
                if (!子知识符合归类陈述形状(定稿请求.定义, 子->定义)
                    || !子规则角色映射完整(定稿请求.定义, *子规则, 子->定义)) {
                    结果.状态 = 知识状态::输入拒绝;
                    结果.诊断说明 = "子知识定义不相容或角色映射不完整"; return 结果;
                }
            }
        }
        知识读取现场 来源现场;
        for (const auto& 绑定 : 定稿请求.来源组) {
            if (!有效(绑定.来源节点) || 绑定.起始标量 < 0 || 绑定.结束标量 <= 绑定.起始标量) {
                结果.状态 = 知识状态::输入拒绝; 结果.诊断说明 = "请求来源标量范围不合法"; return 结果;
            }
            const auto 来源 = 读取知识来源细节已加锁(绑定.来源节点, 来源现场);
            if (来源.状态 != 知识状态::已完成 || !来源.信息) { 结果.状态 = 来源.状态; 结果.诊断说明 = "请求来源无法完整读取"; return 结果; }
            std::int64_t 标量数 = 0;
            if (!知识来源UTF8有效(来源.信息->固定原文, 标量数) || 来源.信息->语言节点 != 语言节点 || 绑定.结束标量 > 标量数) {
                结果.状态 = 知识状态::输入拒绝; 结果.诊断说明 = "请求来源语言或范围不相容"; return 结果;
            }
        }
        请求来源快照 = std::move(来源现场.结构组);
    }
    std::vector<语言知识词端点快照> 词快照;
    结果.状态 = 读取知识词端点快照(收集知识固定词(原快照, &定稿请求.定义), 语言节点, 词服务, 词快照, 结果.诊断说明);
    if (结果.状态 != 知识状态::已完成) return 结果;
    语言知识指代端点快照 指代快照;
    结果.状态 = 读取知识指代端点快照(收集知识指代定义(原快照, &定稿请求.定义),
        存在概念服务, 指代快照, 结果.诊断说明);
    if (结果.状态 != 知识状态::已完成) return 结果;
    结果.状态=核图指代端点定义(原快照,&定稿请求.定义,指代快照,结果.诊断说明);
    if(结果.状态!=知识状态::已完成)return 结果;
    结果.状态 = 复核知识指代端点快照(指代快照, 存在概念服务, 结果.诊断说明);
    if (结果.状态 != 知识状态::已完成) return 结果;
    std::unique_lock 锁(新语言互斥);
    语言知识候选快照 当前快照;
    结果.状态 = 复核知识准备已加锁(语言节点, 原快照, 词快照, 当前快照, 结果.诊断说明);
    if (结果.状态 != 知识状态::已完成) return 结果;
    // 新来源可能尚未被候选引用，独立复核其固定原文、语言及全部字段。
    for (const auto& [身份, 原结构] : 请求来源快照) {
        const auto 当前 = 全局基础数据集.查询完整节点结构(身份);
        if (当前.状态 != 基础结构操作状态::已完成) {
            结果.状态 = 基础知识状态(当前.状态); 结果.诊断说明 = "请求来源最终完整读取未完成"; return 结果;
        }
        if (!当前.结构 || !基础结构相等(*当前.结构, 原结构)) {
            结果.状态 = 知识状态::读取漂移; 结果.诊断说明 = "请求来源固定原文或语言读取漂移"; return 结果;
        }
    }
    const 新语言知识信息* 已有 = nullptr;
    for (const auto& 信息 : 当前快照.信息组) {
        if (信息.定义 != 定稿请求.定义) continue;
        if (已有) { 结果.状态 = 知识状态::内部不一致; 结果.诊断说明 = "同完整定稿匹配多个身份"; return 结果; }
        已有 = &信息;
    }
    if (已有) {
        结果.已发生定义身份 = 已有->节点;
        结果.发布种类 = 新语言知识发布种类::已复用;
        std::vector<新语言知识来源绑定> 缺少来源;
        for (const auto& 来源 : 定稿请求.来源组)
            if (std::find(已有->来源组.begin(), 已有->来源组.end(), 来源) == 已有->来源组.end()) 缺少来源.push_back(来源);
        if (!缺少来源.empty()) {
            结果.阶段 = 新语言知识发布阶段::来源补充;
            const auto 发布 = 全局基础数据集.原子建立节点组(形成缺少来源节点组(已有->节点, 缺少来源));
            if (发布.状态 != 基础结构操作状态::已完成) { 结果.状态 = 基础知识状态(发布.状态); 结果.诊断说明 = "知识已复用，补充来源原子组未完成"; return 结果; }
            if (发布.节点组.size() != 缺少来源.size()) { 结果.状态 = 知识状态::内部不一致; 结果.诊断说明 = "来源发布身份数量不一致"; return 结果; }
        }
    } else {
        结果.阶段 = 新语言知识发布阶段::定义发布;
        const auto 发布 = 全局基础数据集.原子建立节点组(形成知识完整节点组(定稿请求));
        if (发布.状态 != 基础结构操作状态::已完成) { 结果.状态 = 基础知识状态(发布.状态); 结果.诊断说明 = "完整知识原子组未完成"; return 结果; }
        if (发布.节点组.empty() || !有效(发布.节点组.front())) { 结果.状态 = 知识状态::内部不一致; 结果.诊断说明 = "完整知识发布缺少真实身份"; return 结果; }
        结果.已发生定义身份 = 发布.节点组.front();
        结果.发布种类 = 新语言知识发布种类::已建立;
    }
    结果.阶段 = 新语言知识发布阶段::完整读回;
    const auto 读取 = 读取知识结构已加锁(*结果.已发生定义身份);
    if (读取.状态 != 知识状态::已完成 || !读取.信息) { 结果.状态 = 读取.状态; 结果.诊断说明 = 读取.诊断说明; return 结果; }
    if (读取.信息->定义 != 定稿请求.定义
        || std::any_of(定稿请求.来源组.begin(), 定稿请求.来源组.end(), [&](const auto& 来源) {
            return std::find(读取.信息->来源组.begin(), 读取.信息->来源组.end(), 来源) == 读取.信息->来源组.end();
        })) { 结果.状态 = 知识状态::内部不一致; 结果.诊断说明 = "知识定稿或请求来源发布后读回不一致"; return 结果; }
    结果.信息 = *读取.信息;
    结果.状态 = 知识状态::已完成;
    结果.阶段 = 新语言知识发布阶段::完成;
    锁.unlock();
    const auto 端点状态 = 复核知识指代端点快照(指代快照, 存在概念服务, 结果.诊断说明);
    if (端点状态 != 知识状态::已完成) {
        结果.状态 = 端点状态;
        结果.信息.reset();
        结果.阶段 = 新语言知识发布阶段::完整读回;
    }
    return 结果;
}


新语言知识缺口结果 新_语言类::建立或取得语言知识缺口(
    稳定编码 语言节点, const 新语言知识缺口定义& 定义,
    const std::string& 种子版本, const 新_词类& 词服务) {
    新语言知识缺口结果 结果;
    (void)词服务;
    if (!有效(语言节点) || !严格UTF8字节串有效(种子版本)
        || !严格UTF8字节串有效(定义.标签)
        || static_cast<std::uint8_t>(定义.编号) < 1
        || static_cast<std::uint8_t>(定义.编号) > 14)
        return 结果;
    for (const auto& 词面 : 定义.提示词面组)
        if (!严格UTF8字节串有效(词面)) return 结果;
    std::lock_guard 锁(新语言互斥);
    结果.状态 = 知识模式状态已加锁();
    if (结果.状态 != 知识状态::已完成) return 结果;
    if (!读取语言信息(语言节点)) { 结果.状态 = 知识状态::引用不可读; return 结果; }
    const auto 全组 = 全局基础数据集.查询字段引用节点结构(
        语言节点, 知识键(知识标识::缺口语言));
    if (全组.状态 != 基础结构操作状态::已完成) {
        结果.状态 = 基础知识状态(全组.状态); return 结果;
    }
    std::optional<新语言知识缺口信息> 匹配;
    for (const auto& 来源 : 全组.来源节点组) {
        知识读取现场 现场;
        if (!现场.类型(来源.节点.编码, 知识标识::缺口类型,
            {知识标识::缺口语言, 知识标识::缺口编号, 知识标识::缺口标签,
                知识标识::缺口词面, 知识标识::缺口版本})) {
            结果.状态 = 现场.状态; return 结果;
        }
        新语言知识缺口信息 信息{};
        信息.节点 = 来源.节点.编码;
        信息.语言节点 = 现场.引用(信息.节点, 知识标识::缺口语言);
        if (信息.语言节点 != 语言节点) {
            结果.状态 = 知识状态::内部不一致; return 结果;
        }
        信息.定义.编号 = 现场.枚举<新语言知识缺口编号>(
            信息.节点, 知识标识::缺口编号, 14);
        信息.定义.标签 = 现场.原始值<std::string>(信息.节点, 知识标识::缺口标签);
        信息.种子版本 = 现场.原始值<std::string>(信息.节点, 知识标识::缺口版本);
        for (const auto* 项 : 现场.字段(信息.节点, 知识键(知识标识::缺口词面))) {
            const auto* 值 = std::get_if<基础值>(&项->内容);
            const auto* 文本 = 值 ? std::get_if<std::string>(&值->材料) : nullptr;
            if (!文本) { 结果.状态 = 知识状态::内部不一致; return 结果; }
            信息.定义.提示词面组.push_back(*文本);
        }
        if (现场.状态 != 知识状态::已完成) { 结果.状态 = 现场.状态; return 结果; }
        if (信息.定义.编号 == 定义.编号 && 信息.种子版本 == 种子版本) {
            if (匹配) { 结果.状态 = 知识状态::内部不一致; return 结果; }
            匹配 = std::move(信息);
        }
    }
    if (匹配) {
        if (匹配->定义.标签 != 定义.标签
            || 匹配->定义.提示词面组 != 定义.提示词面组) {
            结果.状态 = 知识状态::内部不一致;
            return 结果;
        }
        结果.信息 = std::move(匹配);
        结果.发布种类 = 新语言知识发布种类::已复用;
        结果.状态 = 知识状态::已完成;
        return 结果;
    }
    基础组内新节点 节点{{{节点类型字段, 知识键(知识标识::缺口类型)},
        {知识键(知识标识::缺口语言), 语言节点},
        {知识键(知识标识::缺口编号), 基础值{static_cast<std::int64_t>(定义.编号)}},
        {知识键(知识标识::缺口标签), 基础值{定义.标签}},
        {知识键(知识标识::缺口版本), 基础值{种子版本}}}};
    for (const auto& 词面 : 定义.提示词面组)
        节点.字段组.push_back({知识键(知识标识::缺口词面), 基础值{词面}});
    const auto 发布 = 全局基础数据集.原子建立节点组({节点});
    if (发布.状态 != 基础结构操作状态::已完成) {
        结果.状态 = 基础知识状态(发布.状态); return 结果;
    }
    if (发布.节点组.size() != 1 || !有效(发布.节点组.front())) {
        结果.状态 = 知识状态::内部不一致; return 结果;
    }
    新语言知识缺口信息 信息{};
    信息.节点 = 发布.节点组.front();
    信息.语言节点 = 语言节点;
    信息.定义 = 定义;
    信息.种子版本 = 种子版本;
    结果.信息 = std::move(信息);
    结果.发布种类 = 新语言知识发布种类::已建立;
    结果.状态 = 知识状态::已完成;
    return 结果;
}

新语言知识缺口结果 新_语言类::获取语言知识缺口(稳定编码 缺口节点) const {
    新语言知识缺口结果 结果;
    if (!有效(缺口节点)) return 结果;
    std::lock_guard 锁(新语言互斥);
    结果.状态 = 知识模式状态已加锁();
    if (结果.状态 != 知识状态::已完成) return 结果;
    知识读取现场 现场;
    if (!现场.类型(缺口节点, 知识标识::缺口类型,
        {知识标识::缺口语言, 知识标识::缺口编号, 知识标识::缺口标签,
            知识标识::缺口词面, 知识标识::缺口版本})) {
        结果.状态 = 现场.状态 == 知识状态::已完成
            ? 知识状态::未找到 : 现场.状态;
        return 结果;
    }
    新语言知识缺口信息 信息{};
    信息.节点 = 缺口节点;
    信息.语言节点 = 现场.引用(缺口节点, 知识标识::缺口语言);
    if (!有效(信息.语言节点)) {
        结果.状态 = 知识状态::内部不一致; return 结果;
    }
    信息.定义.编号 = 现场.枚举<新语言知识缺口编号>(缺口节点, 知识标识::缺口编号, 14);
    信息.定义.标签 = 现场.原始值<std::string>(缺口节点, 知识标识::缺口标签);
    信息.种子版本 = 现场.原始值<std::string>(缺口节点, 知识标识::缺口版本);
    for (const auto* 项 : 现场.字段(缺口节点, 知识键(知识标识::缺口词面))) {
        const auto* 值 = std::get_if<基础值>(&项->内容);
        const auto* 文本 = 值 ? std::get_if<std::string>(&值->材料) : nullptr;
        if (!文本) { 结果.状态 = 知识状态::内部不一致; return 结果; }
        信息.定义.提示词面组.push_back(*文本);
    }
    if (现场.状态 != 知识状态::已完成) { 结果.状态 = 现场.状态; return 结果; }
    结果.信息 = std::move(信息);
    结果.状态 = 知识状态::已完成;
    return 结果;
}

新语言知识缺口列表结果 新_语言类::枚举语言知识缺口(
    稳定编码 语言节点, std::size_t 最大条目数) const {
    新语言知识缺口列表结果 结果;
    if (!有效(语言节点) || 最大条目数 == 0) return 结果;
    std::lock_guard 锁(新语言互斥);
    结果.状态 = 知识模式状态已加锁();
    if (结果.状态 != 知识状态::已完成) return 结果;
    if (!读取语言信息(语言节点)) { 结果.状态 = 知识状态::引用不可读; return 结果; }
    const auto 全组 = 全局基础数据集.查询字段引用节点结构(
        语言节点, 知识键(知识标识::缺口语言));
    if (全组.状态 != 基础结构操作状态::已完成) {
        结果.状态 = 基础知识状态(全组.状态); return 结果;
    }
    for (const auto& 来源 : 全组.来源节点组) {
        if (结果.信息组.size() >= 最大条目数) {
            结果.状态 = 知识状态::工程未完成;
            结果.信息组.clear(); return 结果;
        }
        知识读取现场 现场;
        if (!现场.类型(来源.节点.编码, 知识标识::缺口类型,
            {知识标识::缺口语言, 知识标识::缺口编号, 知识标识::缺口标签,
                知识标识::缺口词面, 知识标识::缺口版本})) {
            结果.状态 = 现场.状态; 结果.信息组.clear(); return 结果;
        }
        新语言知识缺口信息 信息{};
        信息.节点 = 来源.节点.编码;
        信息.语言节点 = 现场.引用(信息.节点, 知识标识::缺口语言);
        if (信息.语言节点 != 语言节点) {
            结果.状态 = 知识状态::内部不一致; 结果.信息组.clear(); return 结果;
        }
        信息.定义.编号 = 现场.枚举<新语言知识缺口编号>(
            信息.节点, 知识标识::缺口编号, 14);
        信息.定义.标签 = 现场.原始值<std::string>(信息.节点, 知识标识::缺口标签);
        信息.种子版本 = 现场.原始值<std::string>(信息.节点, 知识标识::缺口版本);
        for (const auto* 项 : 现场.字段(信息.节点, 知识键(知识标识::缺口词面))) {
            const auto* 值 = std::get_if<基础值>(&项->内容);
            const auto* 文本 = 值 ? std::get_if<std::string>(&值->材料) : nullptr;
            if (!文本) {
                结果.状态 = 知识状态::内部不一致; 结果.信息组.clear(); return 结果;
            }
            信息.定义.提示词面组.push_back(*文本);
        }
        if (现场.状态 != 知识状态::已完成) {
            结果.状态 = 现场.状态; 结果.信息组.clear(); return 结果;
        }
        结果.信息组.push_back(std::move(信息));
    }
    结果.状态 = 知识状态::已完成;
    return 结果;
}


// LANGUAGE-STRUCTURE-PROVIDER：本owner的不可变定义与可重建采用视图。
namespace {
using 结构键 = 知识标识;
using 结构快照 = std::map<稳定编码, 基础节点结构>;
bool 结构非空UTF8(const std::string& s) noexcept {
    return 新_语言类::严格UTF8字节串有效(s);
}
template<class T> bool 结构枚举合法(T v, std::int64_t max) {
    return static_cast<std::int64_t>(v) >= 1 && static_cast<std::int64_t>(v) <= max;
}
void 整理受阻(std::vector<稳定编码>& v) {
    std::sort(v.begin(), v.end()); v.erase(std::unique(v.begin(), v.end()), v.end());
}
void 加结构字段(基础组内新节点& n, 结构键 k, 基础组内字段内容 v) {
    n.字段组.push_back({知识键(k), std::move(v)});
}
template<class T> void 加结构整数(基础组内新节点& n, 结构键 k, T v) {
    加结构字段(n, k, 基础值{static_cast<std::int64_t>(v)});
}
基础组内新节点 新结构节点(结构键 t) {
    return {{{节点类型字段, 知识键(t)}}};
}
template<class T> std::vector<T> 读结构值组(知识读取现场& s, 稳定编码 id, 结构键 k) {
    std::vector<T> r;
    for (const auto* f : s.字段(id, 知识键(k))) {
        const auto* v = std::get_if<基础值>(&f->内容);
        const auto* p = v ? std::get_if<T>(&v->材料) : nullptr;
        if (!p) { s.拒绝("结构字段组物理类型不合法"); return {}; }
        r.push_back(*p);
    }
    return r;
}
bool 读结构布尔(知识读取现场& s, 稳定编码 id, 结构键 k) {
    const auto v = s.原始值<std::int64_t>(id, k);
    if (v != 0 && v != 1) s.拒绝("结构布尔值不是0/1");
    return v == 1;
}
知识状态 结构追加状态(基础节点组追加状态 s) {
    switch (s) {
    case 基础节点组追加状态::已完成: return 知识状态::已完成;
    case 基础节点组追加状态::预期不符: return 知识状态::读取漂移;
    case 基础节点组追加状态::资源失败: return 知识状态::工程未完成;
    case 基础节点组追加状态::节点不存在:
    case 基础节点组追加状态::引用不存在: return 知识状态::引用不可读;
    case 基础节点组追加状态::内部不一致: return 知识状态::内部不一致;
    default: return 知识状态::输入拒绝;
    }
}
// discovery只确定范围；同一底座共享锁重读该范围并逐节点对比，才形成完整材料。
知识状态 核结构闭包(const 结构快照& 原, std::vector<基础联合节点快照>* 联合 = nullptr) {
    std::vector<稳定编码> ids;
    for (const auto& [id, n] : 原) { (void)n; ids.push_back(id); }
    if (ids.empty()) return 知识状态::已完成;
    const auto r = 全局基础数据集.查询节点组完整结构(ids);
    if (r.状态 != 基础结构操作状态::已完成) return 基础知识状态(r.状态);
    if (r.节点组.size() != ids.size()) return 知识状态::内部不一致;
    std::set<稳定编码> seen;
    if (联合) 联合->clear();
    for (const auto& p : r.节点组) {
        const auto it = 原.find(p.查询节点);
        if (!p.快照 || it == 原.end() || !seen.insert(p.查询节点).second)
            return 知识状态::内部不一致;
        if (!基础结构相等(p.快照->结构, it->second)) return 知识状态::读取漂移;
        if (联合) 联合->push_back(*p.快照);
    }
    return 知识状态::已完成;
}
知识状态 复核结构词准备已加锁(const 结构快照& 原,
    const std::vector<语言知识词端点快照>& 词, std::vector<稳定编码>& 受阻, std::string& 诊断) {
    (void)受阻;
    结构快照 all = 原;
    for (const auto& p : 词) {
        const auto it = all.find(p.信息.节点);
        if (it != all.end() && !基础结构相等(it->second, p.结构)) {
            诊断 = "词端点与结构原始范围不一致"; return 知识状态::读取漂移;
        }
        all[p.信息.节点] = p.结构;
    }
    const auto s = 核结构闭包(all);
    if (s != 知识状态::已完成) 诊断 = "完整结构或正式词端点已漂移";
    return s;
}
bool 结构字段与声明相同(知识读取现场& s, 稳定编码 id, const 基础组内新节点& n) {
    const auto* old = s.节点(id);
    if (!old) return false;
    std::map<稳定编码, std::vector<基础字段内容>> a, b;
    for (const auto& f : n.字段组) {
        if (const auto* v = std::get_if<基础值>(&f.内容)) a[f.字段节点].push_back(*v);
        else if (const auto* v = std::get_if<稳定编码>(&f.内容)) a[f.字段节点].push_back(*v);
        else return false;
    }
    for (const auto& f : old->字段组) b[f.字段节点].push_back(f.内容);
    return a == b;
}
知识状态 查结构目录(知识读取现场& s, 稳定编码& id) {
    if (!s.节点(自然语言组织根节点)) return s.状态;
    const auto refs = s.引用组(自然语言组织根节点, 结构键::结构概念宿主, 0, 1);
    if (s.状态 != 知识状态::已完成) return s.状态;
    id = refs.empty() ? 稳定编码{} : refs.front();
    if (有效(id)) {
        if (!s.类型(id, 结构键::结构概念类型, {结构键::结构概念标识, 结构键::结构概念版本}))
            return s.状态;
        if (s.原始值<std::string>(id, 结构键::结构概念标识) != "#结构概念目录"
            || s.原始值<std::string>(id, 结构键::结构概念版本) != "catalog")
            s.拒绝("结构目录声明不一致");
    }
    return s.状态;
}
基础组内字段内容 形成结构目录(基础节点组追加请求& req, 知识读取现场& s) {
    稳定编码 id;
    if (查结构目录(s, id) != 知识状态::已完成) return 稳定编码{};
    if (有效(id)) return id;
    const auto i = req.新节点组.size();
    auto n = 新结构节点(结构键::结构概念类型);
    加结构字段(n, 结构键::结构概念标识, 基础值{std::string("#结构概念目录")});
    加结构字段(n, 结构键::结构概念版本, 基础值{std::string("catalog")});
    req.新节点组.push_back(std::move(n));
    req.追加字段组.push_back({自然语言组织根节点, 知识键(结构键::结构概念宿主), 基础组内节点引用{i}});
    return 基础组内节点引用{i};
}
bool 规范结构概念(新语言结构概念定义& d) {
    if (!结构非空UTF8(d.种子标识) || !结构枚举合法(d.种类, 14)
        || !结构枚举合法(d.细目, 4) || !结构枚举合法(d.操作, 9)
        || !结构枚举合法(d.来源, 3)) return false;
    std::set<std::string> ids;
    for (auto& r : d.含义角色组) {
        if (!结构非空UTF8(r.局部编号) || !ids.insert(r.局部编号).second
            || !结构枚举合法(r.种类, 6) || !结构枚举合法(r.参数角色, 14)) return false;
        for (auto c : r.允许类别组) if (!结构枚举合法(c, 11)) return false;
        std::sort(r.允许类别组.begin(), r.允许类别组.end());
        r.允许类别组.erase(std::unique(r.允许类别组.begin(), r.允许类别组.end()), r.允许类别组.end());
    }
    if((d.种类==新语言结构信息种类::行动请求)!=(d.操作==新语言知识含义操作::行动请求))return false;
    if(d.操作==新语言知识含义操作::行动请求) {
        if(d.细目!=新语言结构目的细目::指令||d.含义角色组.size()!=2)return false;
        std::set<新语言知识参数角色> 参数;
        for(const auto& r:d.含义角色组) {
            if(r.可选||!参数.insert(r.参数角色).second)return false;
            if(r.参数角色==新语言知识参数角色::受请求者) {
                if(r.种类!=新语言知识角色种类::领域对象引用||r.允许类别组!=std::vector{新语言知识角色类别::存在})return false;
            }else if(r.参数角色==新语言知识参数角色::目标内容) {
                if(r.种类!=新语言知识角色种类::子含义引用||r.允许类别组!=std::vector{新语言知识角色类别::子含义})return false;
            }else return false;
        }
    }
    return true;
}
bool 核结构模式(const 新语言表达模式定义& d, const 新语言结构概念定义& c) {
    if (!结构非空UTF8(d.模式标识) || d.组成项组.empty()) return false;
    std::map<std::string, bool> slots, roles;
    for (const auto& r : c.含义角色组) roles.emplace(r.局部编号, r.可选);
    for (const auto& x : d.槽位表)
        if (!结构非空UTF8(x.字母) || !slots.emplace(x.字母, x.可选).second) return false;
    std::set<std::string> used, mapped, bound;
    for (const auto& x : d.组成项组) {
        if (x.种类 == 新语言表达组成种类::固定词面) {
            if (!结构非空UTF8(x.词面) || !x.槽位字母.empty()) return false;
        } else if (x.种类 == 新语言表达组成种类::模式槽位) {
            if (!x.词面.empty() || !slots.contains(x.槽位字母)) return false;
            used.insert(x.槽位字母);
        } else return false;
    }
    for (const auto& x : d.映射组) {
        if (!slots.contains(x.槽位字母) || !roles.contains(x.含义角色局部编号)
            || !mapped.insert(x.槽位字母).second) return false;
        if (!slots.at(x.槽位字母)) bound.insert(x.含义角色局部编号);
    }
    if (used.size() != slots.size() || mapped.size() != slots.size()) return false;
    for (const auto& [id, optional] : roles) if (!optional && !bound.contains(id)) return false;
    for (const auto* group : {&d.适用语法编号组, &d.修饰槽标识组}) {
        std::set<std::string> unique;
        for (const auto& x : *group) if (!结构非空UTF8(x) || !unique.insert(x).second) return false;
    }
    if(!d.实现知识节点)return d.知识角色映射组.empty();
    if(!有效(*d.实现知识节点)||d.知识角色映射组.size()!=c.含义角色组.size())return false;
    std::set<std::string> 含义角色;std::set<std::pair<std::optional<std::int64_t>,std::int64_t>> 图角色;
    for(const auto& 映射:d.知识角色映射组)
        if(!roles.contains(映射.含义角色局部编号)||映射.知识图根角色编号<0
            ||(映射.知识图含义编号&&*映射.知识图含义编号<0)
            ||!含义角色.insert(映射.含义角色局部编号).second||!图角色.emplace(映射.知识图含义编号,映射.知识图根角色编号).second)return false;
    return true;
}
bool 规范模式含义映射(新语言表达模式定义& d) {
    if(!d.实现知识节点)return d.知识角色映射组.empty();
    知识读取现场 s;const auto k=读取知识结构细节已加锁(*d.实现知识节点,s);
    if(k.状态!=知识状态::已完成||!k.信息)return false;
    const auto 图=取知识含义图(k.信息->定义);
    for(auto& 项:d.知识角色映射组)if(项.知识图含义编号==图.根含义编号)项.知识图含义编号.reset();
    return true;
}
知识状态 核验结构定义集声明(新语言结构定义集声明& d, std::string& diag) {
    const auto fail = [&] { diag = "结构声明含非法值、重复键或不闭合的局部引用"; return 知识状态::输入拒绝; };
    if (!结构非空UTF8(d.种子版本)) return fail();
    std::map<std::string, 新语言结构概念定义> concepts;
    std::set<std::string> grammars, quantifiers;
    std::set<std::pair<std::string, std::string>> modes;
    std::set<std::pair<新语言结构信息种类, 新语言结构目的细目>> gaps;
    for (auto& c : d.概念组)
        if (!规范结构概念(c) || !concepts.emplace(c.种子标识, c).second) return fail();
    for (const auto& g : d.语法组)
        if (!结构非空UTF8(g.编号) || !结构枚举合法(g.种类, 3)
            || !新_语言类::严格UTF8字节串有效(g.条件) || !结构非空UTF8(g.结果文本)
            || !grammars.insert(g.编号).second) return fail();
    for (auto& m : d.模式组) {
        if(!规范模式含义映射(m.定义))return fail();
        const auto c = concepts.find(m.结构概念种子标识);
        if (m.实现知识来源标签||c == concepts.end() || !核结构模式(m.定义, c->second)
            || !modes.emplace(m.结构概念种子标识, m.定义.模式标识).second) return fail();
        for (const auto& g : m.定义.适用语法编号组) if (!grammars.contains(g)) return fail();
    }
    bool defaultSeen = false;
    for (const auto& q : d.量词组) {
        if (!结构非空UTF8(q.词面) || !quantifiers.insert(q.词面).second
            || (q.是回退默认 && defaultSeen)) return fail();
        defaultSeen = defaultSeen || q.是回退默认;
    }
    for (const auto& g : d.缺口组)
        if (!结构枚举合法(g.种类, 14) || !结构枚举合法(g.细目, 4)
            || !gaps.emplace(g.种类, g.细目).second) return fail();
    return 知识状态::已完成;
}
新语言结构概念信息 读结构概念(知识读取现场& s, 稳定编码 id, std::vector<稳定编码>& blocked) {
    新语言结构概念信息 r{}; r.节点 = id;
    const auto types = s.字段(id, 节点类型字段);
    if (s.状态 != 知识状态::已完成) return r;
    const auto* type = types.size() == 1 ? std::get_if<稳定编码>(&types.front()->内容) : nullptr;
    if (!type || *type != 知识键(结构键::结构概念类型)) {
        s.拒绝("结构概念正式类型不相容"); return r;
    }
    const auto formats = s.字段(id, 知识键(结构键::结构概念布局格式));
    if (s.状态 != 知识状态::已完成) return r;
    if (formats.empty()) { blocked.push_back(id); 整理受阻(blocked); s.拒绝("旧结构概念布局不能恢复角色类别分组", 知识状态::未支持); return r; }
    if (formats.size() != 1) { s.拒绝("结构布局格式重复"); return r; }
    for (const auto k : {结构键::结构概念角色编号, 结构键::结构概念角色种类,
        结构键::结构概念角色参数, 结构键::结构概念角色类别, 结构键::结构概念角色可选})
        if (!s.字段(id, 知识键(k)).empty()) { s.拒绝("结构布局新旧字段混写"); return r; }
    const auto format = s.原始值<std::int64_t>(id, 结构键::结构概念布局格式);
    if (s.状态 != 知识状态::已完成) return r;
    if (format != 2) { blocked.push_back(id); 整理受阻(blocked); s.拒绝("未支持的结构布局", 知识状态::未支持); return r; }
    if (!s.类型(id, 结构键::结构概念类型, {结构键::结构概念宿主, 结构键::结构概念标识,
        结构键::结构概念种类, 结构键::结构概念细目, 结构键::结构概念操作,
        结构键::结构概念来源, 结构键::结构概念版本, 结构键::结构概念布局格式,
        结构键::结构概念角色节点})) return r;
    稳定编码 catalog;
    if (查结构目录(s, catalog) != 知识状态::已完成) return r;
    if (!有效(catalog) || s.引用(id, 结构键::结构概念宿主) != catalog) s.拒绝("结构概念目录归属不一致");
    r.定义.种子标识 = s.原始值<std::string>(id, 结构键::结构概念标识);
    r.定义.种类 = s.枚举<新语言结构信息种类>(id, 结构键::结构概念种类, 14);
    r.定义.细目 = s.枚举<新语言结构目的细目>(id, 结构键::结构概念细目, 4);
    r.定义.操作 = s.枚举<新语言知识含义操作>(id, 结构键::结构概念操作, 9);
    r.定义.来源 = s.枚举<新语言结构来源种类>(id, 结构键::结构概念来源, 3);
    r.种子版本 = s.原始值<std::string>(id, 结构键::结构概念版本);
    for (const auto rid : s.引用组(id, 结构键::结构概念角色节点)) {
        if (!s.类型(rid, 结构键::结构含义角色类型, {结构键::结构含义角色宿主,
            结构键::结构含义角色编号, 结构键::结构含义角色种类, 结构键::结构含义角色参数,
            结构键::结构含义角色可选, 结构键::结构含义角色类别组})) break;
        if (s.引用(rid, 结构键::结构含义角色宿主) != id
            || !s.归属(rid, 结构键::结构概念角色节点, id)) { s.拒绝("结构角色宿主不一致"); break; }
        新语言结构含义角色 role{};
        role.局部编号 = s.原始值<std::string>(rid, 结构键::结构含义角色编号);
        role.种类 = s.枚举<新语言知识角色种类>(rid, 结构键::结构含义角色种类, 6);
        role.参数角色 = s.枚举<新语言知识参数角色>(rid, 结构键::结构含义角色参数, 14);
        role.可选 = 读结构布尔(s, rid, 结构键::结构含义角色可选);
        for (auto v : s.原始值<std::vector<std::int64_t>>(rid, 结构键::结构含义角色类别组)) {
            if (v < 1 || v > 11) s.拒绝("结构角色类别越界");
            role.允许类别组.push_back(static_cast<新语言知识角色类别>(v));
        }
        r.定义.含义角色组.push_back(std::move(role));
    }
    auto normalized = r.定义;
    if (!规范结构概念(normalized) || normalized != r.定义 || !结构非空UTF8(r.种子版本))
        s.拒绝("保存的结构概念不是完整规范化声明");
    return r;
}
基础组内字段内容 加结构概念(基础节点组追加请求& req, 基础组内字段内容 catalog,
    const 新语言结构概念定义& d, const std::string& version) {
    const auto ci = req.新节点组.size();
    req.新节点组.push_back(新结构节点(结构键::结构概念类型));
    auto& n = req.新节点组[ci];
    加结构字段(n, 结构键::结构概念宿主, catalog);
    加结构字段(n, 结构键::结构概念标识, 基础值{d.种子标识});
    加结构整数(n, 结构键::结构概念种类, d.种类); 加结构整数(n, 结构键::结构概念细目, d.细目);
    加结构整数(n, 结构键::结构概念操作, d.操作); 加结构整数(n, 结构键::结构概念来源, d.来源);
    加结构整数(n, 结构键::结构概念布局格式, 2);
    加结构字段(n, 结构键::结构概念版本, 基础值{version});
    for (const auto& r : d.含义角色组) {
        const auto ri = req.新节点组.size();
        auto role = 新结构节点(结构键::结构含义角色类型);
        加结构字段(role, 结构键::结构含义角色宿主, 基础组内节点引用{ci});
        加结构字段(role, 结构键::结构含义角色编号, 基础值{r.局部编号});
        加结构整数(role, 结构键::结构含义角色种类, r.种类);
        加结构整数(role, 结构键::结构含义角色参数, r.参数角色);
        加结构整数(role, 结构键::结构含义角色可选, r.可选);
        std::vector<std::int64_t> classes;
        for (auto c : r.允许类别组) classes.push_back(static_cast<std::int64_t>(c));
        加结构字段(role, 结构键::结构含义角色类别组, 基础值{std::move(classes)});
        req.新节点组.push_back(std::move(role));
        加结构字段(req.新节点组[ci], 结构键::结构概念角色节点, 基础组内节点引用{ri});
    }
    return 基础组内节点引用{ci};
}
void 核模式语法已加锁(知识读取现场& s, 稳定编码 language,
    const std::string& version, const std::vector<std::string>& names);
void 核模式知识已加锁(知识读取现场& 现场,const 新语言表达模式信息& 模式,const 新语言结构概念定义& 结构) {
    if(!模式.定义.实现知识节点)return;
    const auto 知识=读取知识结构细节已加锁(*模式.定义.实现知识节点,现场);
    if(知识.状态!=知识状态::已完成||!知识.信息){现场.拒绝("模式实现知识不可读",知识.状态);return;}
    const auto 图=取知识含义图(知识.信息->定义);
    const auto 根=std::find_if(图.节点组.begin(),图.节点组.end(),[&](const auto& 节点){return 节点.局部编号==图.根含义编号;});
    const 新语言图含义节点* 目标=根==图.节点组.end()?nullptr:&*根;
    const bool 填项=结构.细目==新语言结构目的细目::填项问;
    if(填项&&目标&&目标->操作==新语言知识含义操作::信息请求&&目标->确认范围==新语言知识确认范围::填项内容
        &&目标->子含义组.size()==1&&目标->子含义组.front().用途==新语言子含义用途::被询内容) {
        const auto 子=std::find_if(图.节点组.begin(),图.节点组.end(),[&](const auto& 节点){return 节点.局部编号==目标->子含义组.front().子含义编号;});
        目标=子==图.节点组.end()?nullptr:&*子;
        if(目标&&(目标->操作!=新语言知识含义操作::特征描述||目标->作用域!=新语言知识作用域::提问范围
            ||目标->特征值分支||std::count_if(目标->角色组.begin(),目标->角色组.end(),[](const auto& r){return r.种类==新语言知识角色种类::查询变量;})!=1))目标=nullptr;
    }else if(填项)目标=nullptr;
    if(!目标||目标->操作!=结构.操作||目标->角色组.size()!=结构.含义角色组.size()
        ||知识.信息->定义.结构.语言节点!=模式.语言节点
        ||(((结构.细目==新语言结构目的细目::是非问)||填项)!=(知识.信息->定义.目标.目的==新语言知识表达目的::提问))
        ||((结构.种类==新语言结构信息种类::行动请求)!=(目标->操作==新语言知识含义操作::行动请求))
        ||(目标->操作==新语言知识含义操作::行动请求
            &&(结构.细目!=新语言结构目的细目::指令||目标->作用域!=新语言知识作用域::请求范围
                ||知识.信息->定义.目标.目的!=新语言知识表达目的::提出需求))) {
        现场.拒绝("模式实现知识根签名不相容");return;
    }
    std::map<std::string,新语言局部角色引用> 映射;
    for(const auto& 项:模式.定义.知识角色映射组) {
        const auto 含义=项.知识图含义编号.value_or(图.根含义编号);
        if(含义!=目标->局部编号||项.知识图含义编号==图.根含义编号){现场.拒绝("模式含义映射非规范或指向非目标含义");return;}
        映射.emplace(项.含义角色局部编号,新语言局部角色引用{含义,项.知识图根角色编号});
    }
    for(const auto& 角色:结构.含义角色组) {
        if(!映射.contains(角色.局部编号)){现场.拒绝("模式知识缺角色映射");return;}
        const auto 图角色=std::find_if(目标->角色组.begin(),目标->角色组.end(),[&](const auto& 项){return 项.局部编号==映射.at(角色.局部编号).角色编号;});
        const auto 参数=std::find_if(目标->参数组.begin(),目标->参数组.end(),[&](auto 项){return 项.参数==角色.参数角色&&项.规则角色==映射.at(角色.局部编号).角色编号;});
        if(图角色==目标->角色组.end()||参数==目标->参数组.end()||角色.种类!=图角色->种类
            ||角色.允许类别组!=图角色->必要约束组.front().允许类别组){现场.拒绝("模式SC与图角色类型或参数不相容");return;}
    }
    const auto& 项组=知识.信息->定义.结构.有序项组;
    if(项组.size()!=模式.定义.组成项组.size()){现场.拒绝("模式与知识结构长度不一致");return;}
    std::size_t 固定位置=0;
    for(std::size_t 位置=0;位置<项组.size();++位置) {
        const auto& 组成=模式.定义.组成项组[位置];
        if(const auto* 词=std::get_if<新语言知识固定词项>(&项组[位置].载荷)) {
            if(组成.种类!=新语言表达组成种类::固定词面||固定位置>=模式.固定词条节点组.size()
                ||词->词条节点!=模式.固定词条节点组[固定位置++]){现场.拒绝("模式固定词与知识不一致");return;}
        }else {
            const auto 槽=std::find_if(模式.定义.映射组.begin(),模式.定义.映射组.end(),[&](const auto& 项){return 项.槽位字母==组成.槽位字母;});
            if(组成.种类!=新语言表达组成种类::模式槽位||槽==模式.定义.映射组.end()||!映射.contains(槽->含义角色局部编号)){现场.拒绝("模式槽没有知识映射");return;}
            const auto 预期=映射.at(槽->含义角色局部编号);
            const auto* 角色=std::get_if<新语言知识图角色表达项>(&项组[位置].载荷);
            const auto* 子=std::get_if<新语言知识图子规则项>(&项组[位置].载荷);
            if((!角色||角色->角色!=预期)&&(!子||子->子角色!=预期)){现场.拒绝("模式槽未指向本知识根角色");return;}
        }
    }
}
新语言表达模式信息 读结构模式(知识读取现场& s, 稳定编码 id, std::vector<稳定编码>& blocked) {
    新语言表达模式信息 r{}; r.节点 = id;
    if (!s.类型(id, 结构键::表达模式类型, {结构键::表达模式语言, 结构键::表达模式结构概念,
        结构键::表达模式标识, 结构键::表达模式版本, 结构键::表达模式槽字母, 结构键::表达模式槽可选,
        结构键::表达模式组成种类, 结构键::表达模式组成词面, 结构键::表达模式组成槽,
        结构键::表达模式映射槽, 结构键::表达模式映射角色, 结构键::表达模式语法编号,
        结构键::表达模式修饰槽, 结构键::表达模式固定词条,结构键::模式实现知识,结构键::模式知识角色映射})) return r;
    r.语言节点 = s.引用(id, 结构键::表达模式语言);
    if (!读取语言信息(r.语言节点)) s.拒绝("模式语言不可读", 知识状态::引用不可读);
    (void)s.节点(r.语言节点);
    r.结构概念节点 = s.引用(id, 结构键::表达模式结构概念);
    const auto c = 读结构概念(s, r.结构概念节点, blocked);
    r.定义.模式标识 = s.原始值<std::string>(id, 结构键::表达模式标识);
    r.种子版本 = s.原始值<std::string>(id, 结构键::表达模式版本);
    const auto names = 读结构值组<std::string>(s, id, 结构键::表达模式槽字母);
    const auto optional = 读结构值组<std::int64_t>(s, id, 结构键::表达模式槽可选);
    if (names.size() != optional.size()) s.拒绝("模式槽位组长度不一致");
    else for (std::size_t i = 0; i < names.size(); ++i) {
        if (optional[i] != 0 && optional[i] != 1) s.拒绝("模式可选标记不合法");
        r.定义.槽位表.push_back({names[i], optional[i] == 1});
    }
    const auto kinds = 读结构值组<std::int64_t>(s, id, 结构键::表达模式组成种类);
    const auto words = 读结构值组<std::string>(s, id, 结构键::表达模式组成词面);
    const auto slots = 读结构值组<std::string>(s, id, 结构键::表达模式组成槽);
    std::size_t wi = 0, si = 0;
    for (auto k : kinds) {
        if (k == 1 && wi < words.size()) r.定义.组成项组.push_back({新语言表达组成种类::固定词面, words[wi++], {}});
        else if (k == 2 && si < slots.size()) r.定义.组成项组.push_back({新语言表达组成种类::模式槽位, {}, slots[si++]});
        else s.拒绝("模式组成载荷不合法");
    }
    if (wi != words.size() || si != slots.size()) s.拒绝("模式组成字段多余");
    const auto ms = 读结构值组<std::string>(s, id, 结构键::表达模式映射槽);
    const auto mr = 读结构值组<std::string>(s, id, 结构键::表达模式映射角色);
    if (ms.size() != mr.size()) s.拒绝("模式映射组长度不一致");
    else for (std::size_t i = 0; i < ms.size(); ++i) r.定义.映射组.push_back({ms[i], mr[i]});
    r.定义.适用语法编号组 = 读结构值组<std::string>(s, id, 结构键::表达模式语法编号);
    r.定义.修饰槽标识组 = 读结构值组<std::string>(s, id, 结构键::表达模式修饰槽);
    const auto 实现=s.引用组(id,结构键::模式实现知识,0,1);
    if(!实现.empty())r.定义.实现知识节点=实现.front();
    for(auto 映射:s.引用组(id,结构键::模式知识角色映射)) {
        if(!s.类型(映射,结构键::模式知识映射类型,{结构键::模式知识映射宿主,结构键::模式知识映射含义角色,结构键::模式知识映射图角色,结构键::模式知识映射含义编号})
            ||s.引用(映射,结构键::模式知识映射宿主)!=id||!s.归属(映射,结构键::模式知识角色映射,id))return r;
        r.定义.知识角色映射组.push_back({s.原始值<std::string>(映射,结构键::模式知识映射含义角色),s.原始值<std::int64_t>(映射,结构键::模式知识映射图角色)});
        if(!s.字段(映射,知识键(结构键::模式知识映射含义编号)).empty())r.定义.知识角色映射组.back().知识图含义编号=s.原始值<std::int64_t>(映射,结构键::模式知识映射含义编号);
    }
    // 同一固定词可出现多次，故不能用去重引用组读取。
    for (const auto* f : s.字段(id, 知识键(结构键::表达模式固定词条))) {
        const auto* ref = std::get_if<稳定编码>(&f->内容);
        if (!ref || !有效(*ref)) s.拒绝("固定词引用不合法");
        else { r.固定词条节点组.push_back(*ref); (void)s.节点(*ref); }
    }
    if (r.固定词条节点组.size() != words.size()) {
        blocked.push_back(id); 整理受阻(blocked); s.拒绝("旧模式缺少正式固定词绑定", 知识状态::未支持);
    }
    if (!核结构模式(r.定义, c.定义) || r.种子版本 != c.种子版本)
        s.拒绝("模式定义或结构概念版本不一致");
    if (s.状态 == 知识状态::已完成)
        核模式语法已加锁(s, r.语言节点, r.种子版本, r.定义.适用语法编号组);
    if(s.状态==知识状态::已完成)核模式知识已加锁(s,r,c.定义);
    return r;
}
基础组内新节点 形成表达模式节点(稳定编码 language, 基础组内字段内容 conceptRef,
    const 新语言表达模式定义& d, const std::string& version, const std::vector<稳定编码>& words) {
    auto n = 新结构节点(结构键::表达模式类型);
    加结构字段(n, 结构键::表达模式语言, language); 加结构字段(n, 结构键::表达模式结构概念, conceptRef);
    加结构字段(n, 结构键::表达模式标识, 基础值{d.模式标识}); 加结构字段(n, 结构键::表达模式版本, 基础值{version});
    for (const auto& x : d.槽位表) {
        加结构字段(n, 结构键::表达模式槽字母, 基础值{x.字母});
        加结构整数(n, 结构键::表达模式槽可选, x.可选);
    }
    for (const auto& x : d.组成项组) {
        加结构整数(n, 结构键::表达模式组成种类, x.种类);
        if (x.种类 == 新语言表达组成种类::固定词面) 加结构字段(n, 结构键::表达模式组成词面, 基础值{x.词面});
        else 加结构字段(n, 结构键::表达模式组成槽, 基础值{x.槽位字母});
    }
    for (auto id : words) 加结构字段(n, 结构键::表达模式固定词条, id);
    for (const auto& x : d.映射组) {
        加结构字段(n, 结构键::表达模式映射槽, 基础值{x.槽位字母});
        加结构字段(n, 结构键::表达模式映射角色, 基础值{x.含义角色局部编号});
    }
    for (const auto& x : d.适用语法编号组) 加结构字段(n, 结构键::表达模式语法编号, 基础值{x});
    for (const auto& x : d.修饰槽标识组) 加结构字段(n, 结构键::表达模式修饰槽, 基础值{x});
    if(d.实现知识节点)加结构字段(n,结构键::模式实现知识,*d.实现知识节点);
    return n;
}
void 追加模式知识映射(基础节点组追加请求& 请求,std::size_t 模式索引,const 新语言表达模式定义& 定义) {
    for(const auto& 映射:定义.知识角色映射组) {
        const auto 索引=请求.新节点组.size();auto 节点=新结构节点(结构键::模式知识映射类型);
        加结构字段(节点,结构键::模式知识映射宿主,基础组内节点引用{模式索引});
        加结构字段(节点,结构键::模式知识映射含义角色,基础值{映射.含义角色局部编号});加结构整数(节点,结构键::模式知识映射图角色,映射.知识图根角色编号);
        if(映射.知识图含义编号)加结构整数(节点,结构键::模式知识映射含义编号,*映射.知识图含义编号);
        请求.新节点组.push_back(std::move(节点));加结构字段(请求.新节点组[模式索引],结构键::模式知识角色映射,基础组内节点引用{索引});
    }
}
bool 模式字段与声明相同(知识读取现场& 现场,稳定编码 身份,基础组内新节点 声明) {
    for(auto 映射:现场.引用组(身份,结构键::模式知识角色映射))加结构字段(声明,结构键::模式知识角色映射,映射);
    return 结构字段与声明相同(现场,身份,声明);
}
新语言语法规则信息 读结构语法(知识读取现场& s, 稳定编码 id) {
    新语言语法规则信息 r{}; r.节点 = id;
    if (!s.类型(id, 结构键::语法规则类型, {结构键::语法规则语言, 结构键::语法规则编号,
        结构键::语法规则种类, 结构键::语法规则条件, 结构键::语法规则结果,
        结构键::语法规则版本, 结构键::语法规则来源})) return r;
    r.语言节点 = s.引用(id, 结构键::语法规则语言);
    (void)s.节点(r.语言节点);
    if (!读取语言信息(r.语言节点)) s.拒绝("语法语言不可读", 知识状态::引用不可读);
    r.定义.编号 = s.原始值<std::string>(id, 结构键::语法规则编号);
    r.定义.种类 = s.枚举<新语言语法种类>(id, 结构键::语法规则种类, 3);
    r.定义.条件 = s.原始值<std::string>(id, 结构键::语法规则条件);
    r.定义.结果文本 = s.原始值<std::string>(id, 结构键::语法规则结果);
    r.种子版本 = s.原始值<std::string>(id, 结构键::语法规则版本);
    r.来源 = s.枚举<新语言结构来源种类>(id, 结构键::语法规则来源, 3);
    if (!结构非空UTF8(r.定义.编号) || !结构非空UTF8(r.定义.结果文本)
        || !新_语言类::严格UTF8字节串有效(r.定义.条件) || !结构非空UTF8(r.种子版本))
        s.拒绝("语法文本不合法");
    return r;
}
基础组内新节点 形成结构语法(稳定编码 lang, const 新语言语法规则定义& d, const std::string& v) {
    auto n = 新结构节点(结构键::语法规则类型);
    加结构字段(n, 结构键::语法规则语言, lang); 加结构字段(n, 结构键::语法规则编号, 基础值{d.编号});
    加结构整数(n, 结构键::语法规则种类, d.种类); 加结构字段(n, 结构键::语法规则条件, 基础值{d.条件});
    加结构字段(n, 结构键::语法规则结果, 基础值{d.结果文本}); 加结构字段(n, 结构键::语法规则版本, 基础值{v});
    加结构整数(n, 结构键::语法规则来源, 新语言结构来源种类::种子); return n;
}
新语言量词搭配信息 读结构量词(知识读取现场& s, 稳定编码 id) {
    新语言量词搭配信息 r{}; r.节点 = id;
    if (!s.类型(id, 结构键::量词搭配类型, {结构键::量词搭配语言, 结构键::量词搭配词面,
        结构键::量词搭配回退, 结构键::量词搭配版本})) return r;
    r.语言节点 = s.引用(id, 结构键::量词搭配语言); (void)s.节点(r.语言节点);
    r.量词词面 = s.原始值<std::string>(id, 结构键::量词搭配词面);
    r.种子版本 = s.原始值<std::string>(id, 结构键::量词搭配版本);
    r.是回退默认 = 读结构布尔(s, id, 结构键::量词搭配回退);
    if (!结构非空UTF8(r.量词词面) || !结构非空UTF8(r.种子版本)) s.拒绝("量词文本不合法");
    return r;
}
基础组内新节点 形成结构量词(稳定编码 lang, const 新语言结构量词声明& d, const std::string& v) {
    auto n = 新结构节点(结构键::量词搭配类型);
    加结构字段(n, 结构键::量词搭配语言, lang); 加结构字段(n, 结构键::量词搭配词面, 基础值{d.词面});
    加结构整数(n, 结构键::量词搭配回退, d.是回退默认); 加结构字段(n, 结构键::量词搭配版本, 基础值{v}); return n;
}
基础组内新节点 形成结构缺口声明(稳定编码 lang, const 新语言结构缺口声明& d, const std::string& v) {
    auto n = 新结构节点(结构键::结构缺口声明类型);
    加结构字段(n, 结构键::结构缺口声明语言, lang); 加结构整数(n, 结构键::结构缺口声明种类, d.种类);
    加结构整数(n, 结构键::结构缺口声明细目, d.细目); 加结构字段(n, 结构键::结构缺口声明版本, 基础值{v}); return n;
}
// 反向索引只发现身份；现场保留每个来源节点供最终原子预期。
std::vector<稳定编码> 查结构反向(知识读取现场& s, 稳定编码 target, 结构键 key) {
    std::vector<稳定编码> ids;
    const auto r = 全局基础数据集.查询字段引用节点结构(target, 知识键(key));
    if (r.状态 != 基础结构操作状态::已完成) { s.拒绝("结构反查失败", 基础知识状态(r.状态)); return ids; }
    std::set<稳定编码> seen;
    for (const auto& n : r.来源节点组) {
        if (!seen.insert(n.节点.编码).second) { s.拒绝("结构反查重复身份"); break; }
        const auto* prev = s.节点(n.节点.编码);
        if (!prev || !基础结构相等(*prev, n)) { s.拒绝("结构反查读取漂移", 知识状态::读取漂移); break; }
        ids.push_back(n.节点.编码);
    }
    return ids;
}
void 核模式语法已加锁(知识读取现场& s, 稳定编码 language,
    const std::string& version, const std::vector<std::string>& names) {
    if (s.状态 != 知识状态::已完成 || names.empty()) return;
    const auto ids = 查结构反向(s, language, 结构键::语法规则语言);
    if (s.状态 != 知识状态::已完成) return;
    for (const auto& name : names) {
        std::optional<稳定编码> found;
        for (auto id : ids) {
            const auto savedName = s.原始值<std::string>(id, 结构键::语法规则编号);
            const auto savedVersion = s.原始值<std::string>(id, 结构键::语法规则版本);
            if (s.状态 != 知识状态::已完成) return;
            if (savedName != name || savedVersion != version) continue;
            if (found) { s.拒绝("模式所用同语言同版本语法存在重复键"); return; }
            found = id;
        }
        if (!found) { s.拒绝("模式所用同语言同版本正式语法不可读", 知识状态::引用不可读); return; }
        const auto grammar = 读结构语法(s, *found);
        if (s.状态 != 知识状态::已完成) return;
        if (grammar.语言节点 != language || grammar.种子版本 != version || grammar.定义.编号 != name) {
            s.拒绝("模式所用正式语法与声明不一致"); return;
        }
    }
}
知识状态 核模式词(const std::vector<新语言表达模式信息>& modes, const 新_词类& service,
    std::vector<语言知识词端点快照>& snapshots, std::string& diag) {
    std::map<std::pair<稳定编码, 稳定编码>, std::string> expected;
    for (const auto& m : modes) {
        std::size_t i = 0;
        for (const auto& x : m.定义.组成项组) if (x.种类 == 新语言表达组成种类::固定词面) {
            if (i >= m.固定词条节点组.size()) return 知识状态::内部不一致;
            const auto key = std::make_pair(m.语言节点, m.固定词条节点组[i++]);
            const auto old = expected.find(key);
            if (old != expected.end() && old->second != x.词面) return 知识状态::内部不一致;
            expected[key] = x.词面;
        }
        if (i != m.固定词条节点组.size()) return 知识状态::内部不一致;
    }
    snapshots.clear();
    for (const auto& [key, word] : expected) {
        std::vector<语言知识词端点快照> one;
        auto state = 读取知识词端点快照({key.second}, key.first, service, one, diag);
        if (state != 知识状态::已完成) return state;
        if (one.size() != 1 || one.front().信息.规范化词面 != word) {
            diag = "正式固定词词面与组成不一致"; return 知识状态::内部不一致;
        }
        snapshots.push_back(one.front());
    }
    return 知识状态::已完成;
}
知识状态 准备声明词(稳定编码 lang, const std::vector<新语言结构模式声明>& modes,
    const 新_词类& service, std::vector<语言知识词端点快照>& snapshots, std::string& diag) {
    std::vector<新语言表达模式信息> inputs;
    for (const auto& m : modes) {
        新语言表达模式信息 info{}; info.语言节点 = lang; info.定义 = m.定义;
        for (const auto& x : m.定义.组成项组) if (x.种类 == 新语言表达组成种类::固定词面) {
            const auto r = service.按规范化词面查询(lang, 严格UTF8字节精确规范化合同节点, x.词面);
            if (r.状态 != 新词条查询状态::已找到 || !r.信息) {
                diag = "声明固定词不是可读正式词条";
                return r.状态 == 新词条查询状态::资源失败 ? 知识状态::工程未完成
                    : r.状态 == 新词条查询状态::结构不一致 ? 知识状态::内部不一致 : 知识状态::引用不可读;
            }
            info.固定词条节点组.push_back(r.信息->节点);
        }
        inputs.push_back(std::move(info));
    }
    return 核模式词(inputs, service, snapshots, diag);
}
std::vector<稳定编码> 声明词引用(const 新语言表达模式定义& d, const std::vector<语言知识词端点快照>& snapshots) {
    std::vector<稳定编码> ids;
    for (const auto& x : d.组成项组) if (x.种类 == 新语言表达组成种类::固定词面) {
        const auto it = std::find_if(snapshots.begin(), snapshots.end(),
            [&](const auto& w) { return w.信息.规范化词面 == x.词面; });
        if (it == snapshots.end()) throw std::logic_error("声明词准备缺少已核固定词");
        ids.push_back(it->信息.节点);
    }
    return ids;
}


知识状态 读取结构定义集原始已加锁(稳定编码 id, 新语言结构定义集信息& out,
    结构快照& raw, std::vector<稳定编码>& blocked, std::string& diag) {
    out = {}; raw.clear();
    知识读取现场 s;
    const auto fail = [&] {
        diag = s.诊断; raw = std::move(s.结构组); out = {}; 整理受阻(blocked); return s.状态;
    };
    if (!s.类型(id, 结构键::结构定义集类型, {结构键::结构定义集语言, 结构键::结构定义集版本,
        结构键::结构定义集来源, 结构键::结构定义集概念, 结构键::结构定义集模式,
        结构键::结构定义集语法, 结构键::结构定义集量词, 结构键::结构定义集缺口})) return fail();
    新语言结构定义集信息 r{}; r.节点 = id;
    r.语言节点 = s.引用(id, 结构键::结构定义集语言);
    if (!读取语言信息(r.语言节点)) s.拒绝("定义集语言不可读", 知识状态::引用不可读);
    (void)s.节点(r.语言节点);
    r.声明.种子版本 = s.原始值<std::string>(id, 结构键::结构定义集版本);
    r.原种子来源节点 = s.引用(id, 结构键::结构定义集来源);
    const auto source = 读取知识来源细节已加锁(r.原种子来源节点, s);
    if (source.状态 != 知识状态::已完成) {
        s.拒绝("结构定义集正式原始来源读取失败", source.状态); return fail();
    }
    if (!source.信息) { s.拒绝("结构定义集来源成功却无完整材料"); return fail(); }
    if (source.信息->语言节点 != r.语言节点
        || source.信息->种类 != 新语言知识来源种类::经确认先天定义)
        s.拒绝("定义集来源不是本语言正式先天来源");
    std::map<稳定编码, std::string> concepts;
    for (auto node : s.引用组(id, 结构键::结构定义集概念)) {
        auto c = 读结构概念(s, node, blocked);
        if (s.状态 != 知识状态::已完成) return fail();
        if (c.种子版本 != r.声明.种子版本) s.拒绝("集内概念内容版本不一致");
        concepts.emplace(node, c.定义.种子标识);
        r.声明.概念组.push_back(c.定义); r.概念组.push_back(std::move(c));
    }
    for (auto node : s.引用组(id, 结构键::结构定义集语法)) {
        auto g = 读结构语法(s, node);
        if (g.语言节点 != r.语言节点 || g.种子版本 != r.声明.种子版本) s.拒绝("集内语法语言或版本不一致");
        r.声明.语法组.push_back(g.定义); r.语法组.push_back(std::move(g));
    }
    for (auto node : s.引用组(id, 结构键::结构定义集模式)) {
        auto m = 读结构模式(s, node, blocked);
        if (s.状态 != 知识状态::已完成) return fail();
        const auto ci = concepts.find(m.结构概念节点);
        if (ci == concepts.end() || m.语言节点 != r.语言节点 || m.种子版本 != r.声明.种子版本) {
            s.拒绝("集内模式的概念、语言或内容版本不一致"); return fail();
        }
        r.声明.模式组.push_back({ci->second, m.定义}); r.模式组.push_back(std::move(m));
    }
    for (auto node : s.引用组(id, 结构键::结构定义集量词)) {
        auto q = 读结构量词(s, node);
        if (q.语言节点 != r.语言节点 || q.种子版本 != r.声明.种子版本) s.拒绝("集内量词语言或版本不一致");
        r.声明.量词组.push_back({q.量词词面, q.是回退默认}); r.量词组.push_back(std::move(q));
    }
    for (auto node : s.引用组(id, 结构键::结构定义集缺口)) {
        if (!s.类型(node, 结构键::结构缺口声明类型, {结构键::结构缺口声明语言,
            结构键::结构缺口声明种类, 结构键::结构缺口声明细目, 结构键::结构缺口声明版本})) return fail();
        if (s.引用(node, 结构键::结构缺口声明语言) != r.语言节点
            || s.原始值<std::string>(node, 结构键::结构缺口声明版本) != r.声明.种子版本)
            s.拒绝("缺口声明语言或版本不一致");
        r.声明.缺口组.push_back({s.枚举<新语言结构信息种类>(node, 结构键::结构缺口声明种类, 14),
            s.枚举<新语言结构目的细目>(node, 结构键::结构缺口声明细目, 4)});
    }
    auto normalized = r.声明;
    std::string why;
    if (核验结构定义集声明(normalized, why) != 知识状态::已完成 || normalized != r.声明)
        s.拒绝("定义集不是完整规范化闭合声明");
    if (s.状态 != 知识状态::已完成) return fail();
    const auto state = 核结构闭包(s.结构组);
    if (state != 知识状态::已完成) { s.拒绝("定义集完整范围读取漂移", state); return fail(); }
    out = std::move(r); raw = std::move(s.结构组); return 知识状态::已完成;
}
std::optional<稳定编码> 找同键定义集(知识读取现场& s, const 新语言结构定义集发布请求& q) {
    std::optional<稳定编码> found;
    for (const auto id : 查结构反向(s, q.语言节点, 结构键::结构定义集语言)) {
        const auto lang = s.引用(id, 结构键::结构定义集语言);
        const auto version = s.原始值<std::string>(id, 结构键::结构定义集版本);
        const auto source = s.引用(id, 结构键::结构定义集来源);
        if (s.状态 != 知识状态::已完成) return {};
        if (lang != q.语言节点 || version != q.声明.种子版本 || source != q.原种子来源节点) continue;
        if (found) { s.拒绝("同键存在多个结构定义集"); return {}; }
        found = id;
    }
    return found;
}
知识状态 形成结构定义集追加已加锁(const 新语言结构定义集发布请求& q,
    const std::vector<语言知识词端点快照>& words, 基础节点组追加请求& req,
    std::optional<std::size_t>& rootIndex, std::vector<稳定编码>& blocked, std::string& diag) {
    req = {}; rootIndex.reset();
    知识读取现场 s;
    const auto fail = [&](知识状态 state) { req = {}; rootIndex.reset(); diag = s.诊断.empty() ? diag : s.诊断; 整理受阻(blocked); return state; };
    if (!读取语言信息(q.语言节点)) { diag = "定义集语言不可读"; return fail(知识状态::引用不可读); }
    (void)s.节点(q.语言节点);
    const auto source = 读取知识来源细节已加锁(q.原种子来源节点, s);
    if (source.状态 != 知识状态::已完成) {
        s.拒绝("定义集发布前正式原始来源读取失败", source.状态); return fail(source.状态);
    }
    if (!source.信息) {
        s.拒绝("定义集发布前来源成功却无完整材料"); return fail(s.状态);
    }
    if (source.信息->语言节点 != q.语言节点 || source.信息->种类 != 新语言知识来源种类::经确认先天定义)
        return fail(知识状态::输入拒绝);
    const auto existing = 找同键定义集(s, q);
    if (s.状态 != 知识状态::已完成) return fail(s.状态);
    if (existing) {
        新语言结构定义集信息 info; 结构快照 raw;
        auto state = 读取结构定义集原始已加锁(*existing, info, raw, blocked, diag);
        if (state != 知识状态::已完成) return fail(state);
        if (info.声明 != q.声明) return fail(知识状态::输入拒绝);
        state = 复核结构词准备已加锁(raw, words, blocked, diag);
        if (state != 知识状态::已完成) return fail(state);
        return 知识状态::已完成;
    }
    稳定编码 catalogId;
    if (查结构目录(s, catalogId) != 知识状态::已完成) return fail(s.状态);
    std::optional<基础组内字段内容> catalog;
    if (有效(catalogId)) catalog = catalogId;
    std::vector<稳定编码> conceptIds;
    if (有效(catalogId)) {
        conceptIds = 查结构反向(s, catalogId, 结构键::结构概念宿主);
        conceptIds.erase(std::remove(conceptIds.begin(), conceptIds.end(), 自然语言组织根节点), conceptIds.end());
    }
    std::map<std::string, 基础组内字段内容> conceptRefs;
    std::vector<基础组内字段内容> cs, ms, gs, qs, gaps;
    for (const auto& d : q.声明.概念组) {
        std::optional<稳定编码> match;
        for (auto id : conceptIds) {
            const auto name = s.原始值<std::string>(id, 结构键::结构概念标识);
            const auto version = s.原始值<std::string>(id, 结构键::结构概念版本);
            if (name != d.种子标识 || version != q.声明.种子版本) continue;
            if (match) { s.拒绝("同键存在多个结构概念"); return fail(s.状态); }
            match = id;
        }
        基础组内字段内容 ref = 稳定编码{};
        if (match) {
            const auto c = 读结构概念(s, *match, blocked);
            if (s.状态 != 知识状态::已完成) return fail(s.状态);
            if (c.定义 != d) return fail(知识状态::输入拒绝);
            ref = *match;
        } else {
            if (!catalog) catalog = 形成结构目录(req, s);
            if (s.状态 != 知识状态::已完成) return fail(s.状态);
            ref = 加结构概念(req, *catalog, d, q.声明.种子版本);
        }
        conceptRefs.emplace(d.种子标识, ref); cs.push_back(ref);
    }
    // 成员同键全部读完后才选复用；没有首项胜出或异义覆盖。
    const auto reuse = [&](const std::vector<稳定编码>& ids, const auto& keyMatches,
        const auto& equal, const 基础组内新节点& n, std::vector<基础组内字段内容>& refs) -> 知识状态 {
        std::optional<稳定编码> found;
        for (auto id : ids) if (keyMatches(id)) {
            if (s.状态 != 知识状态::已完成) return s.状态;
            if (found) { s.拒绝("同键存在多个结构成员"); return s.状态; }
            found = id;
        }
        if (s.状态 != 知识状态::已完成) return s.状态;
        if (found) {
            if (!equal(*found)) return s.状态 == 知识状态::已完成 ? 知识状态::输入拒绝 : s.状态;
            refs.push_back(*found);
        } else {
            refs.push_back(基础组内节点引用{req.新节点组.size()}); req.新节点组.push_back(n);
        }
        return 知识状态::已完成;
    };
    const auto grammarIds = 查结构反向(s, q.语言节点, 结构键::语法规则语言);
    for (const auto& d : q.声明.语法组) {
        const auto n = 形成结构语法(q.语言节点, d, q.声明.种子版本);
        const auto state = reuse(grammarIds, [&](auto id) {
            return s.原始值<std::string>(id, 结构键::语法规则编号) == d.编号
                && s.原始值<std::string>(id, 结构键::语法规则版本) == q.声明.种子版本;
        }, [&](auto id) {
            const auto g = 读结构语法(s, id);
            return g.定义 == d && g.语言节点 == q.语言节点 && g.来源 == 新语言结构来源种类::种子
                && 结构字段与声明相同(s, id, n);
        }, n, gs);
        if (state != 知识状态::已完成) return fail(state);
    }
    const auto modeIds = 查结构反向(s, q.语言节点, 结构键::表达模式语言);
    for (const auto& d : q.声明.模式组) {
        const auto cref = conceptRefs.at(d.结构概念种子标识);
        const auto refs = 声明词引用(d.定义, words);
        const auto 概念=std::find_if(q.声明.概念组.begin(),q.声明.概念组.end(),[&](const auto& 项){return 项.种子标识==d.结构概念种子标识;});
        if(概念==q.声明.概念组.end())return fail(知识状态::输入拒绝);
        核模式知识已加锁(s,新语言表达模式信息{{},q.语言节点,{},d.定义,q.声明.种子版本,refs},*概念);
        if(s.状态!=知识状态::已完成)return fail(s.状态);
        const auto n = 形成表达模式节点(q.语言节点, cref, d.定义, q.声明.种子版本, refs);
        const auto state = reuse(modeIds, [&](auto id) {
            if (s.原始值<std::string>(id, 结构键::表达模式标识) != d.定义.模式标识
                || s.原始值<std::string>(id, 结构键::表达模式版本) != q.声明.种子版本) return false;
            const auto oldc = s.引用(id, 结构键::表达模式结构概念);
            if (const auto* c = std::get_if<稳定编码>(&cref)) return oldc == *c;
            return false;
        }, [&](auto id) {
            const auto m = 读结构模式(s, id, blocked);
            return m.定义 == d.定义 && m.固定词条节点组 == refs && 模式字段与声明相同(s, id, n);
        }, n, ms);
        if (state != 知识状态::已完成) return fail(state);
        if(const auto* 索引=std::get_if<基础组内节点引用>(&ms.back()))追加模式知识映射(req,索引->索引,d.定义);
    }
    const auto quantIds = 查结构反向(s, q.语言节点, 结构键::量词搭配语言);
    for (const auto& d : q.声明.量词组) {
        const auto n = 形成结构量词(q.语言节点, d, q.声明.种子版本);
        const auto state = reuse(quantIds, [&](auto id) {
            return s.原始值<std::string>(id, 结构键::量词搭配词面) == d.词面
                && s.原始值<std::string>(id, 结构键::量词搭配版本) == q.声明.种子版本;
        }, [&](auto id) {
            (void)读结构量词(s, id); return 结构字段与声明相同(s, id, n);
        }, n, qs);
        if (state != 知识状态::已完成) return fail(state);
    }
    const auto gapIds = 查结构反向(s, q.语言节点, 结构键::结构缺口声明语言);
    for (const auto& d : q.声明.缺口组) {
        const auto n = 形成结构缺口声明(q.语言节点, d, q.声明.种子版本);
        const auto state = reuse(gapIds, [&](auto id) {
            return s.原始值<std::string>(id, 结构键::结构缺口声明版本) == q.声明.种子版本
                && s.原始值<std::int64_t>(id, 结构键::结构缺口声明种类) == static_cast<std::int64_t>(d.种类)
                && s.原始值<std::int64_t>(id, 结构键::结构缺口声明细目) == static_cast<std::int64_t>(d.细目);
        }, [&](auto id) { return 结构字段与声明相同(s, id, n); }, n, gaps);
        if (state != 知识状态::已完成) return fail(state);
    }
    auto n = 新结构节点(结构键::结构定义集类型);
    加结构字段(n, 结构键::结构定义集语言, q.语言节点);
    加结构字段(n, 结构键::结构定义集来源, q.原种子来源节点);
    加结构字段(n, 结构键::结构定义集版本, 基础值{q.声明.种子版本});
    const auto addRefs = [&](结构键 k, const auto& refs) { for (const auto& ref : refs) 加结构字段(n, k, ref); };
    addRefs(结构键::结构定义集概念, cs); addRefs(结构键::结构定义集模式, ms);
    addRefs(结构键::结构定义集语法, gs); addRefs(结构键::结构定义集量词, qs); addRefs(结构键::结构定义集缺口, gaps);
    rootIndex = req.新节点组.size(); req.新节点组.push_back(std::move(n));
    for (const auto& w : words) s.结构组[w.信息.节点] = w.结构;
    auto state = 核结构闭包(s.结构组, &req.预期联合节点组);
    if (state != 知识状态::已完成) return fail(state);
    return 知识状态::已完成;
}


std::vector<新语言结构对应行> 派生集对应行(const 新语言结构定义集信息& set) {
    std::vector<新语言结构对应行> rows;
    std::map<稳定编码, const 新语言结构概念信息*> concepts;
    std::map<std::string, 稳定编码> grammar;
    for (const auto& c : set.概念组) concepts.emplace(c.节点, &c);
    for (const auto& g : set.语法组) grammar.emplace(g.定义.编号, g.节点);
    for (const auto& m : set.模式组) {
        const auto& c = *concepts.at(m.结构概念节点);
        新语言结构对应行 r{};
        r.语言节点 = set.语言节点; r.结构概念节点 = c.节点; r.表达模式节点 = m.节点;
        r.种类 = c.定义.种类; r.细目 = c.定义.细目; r.来源 = c.定义.来源;
        r.种子版本 = m.种子版本; r.结构概念种子标识 = c.定义.种子标识; r.模式标识 = m.定义.模式标识;
        r.实现知识节点=m.定义.实现知识节点;
        for (const auto& name : m.定义.适用语法编号组) r.语法规则节点组.push_back(grammar.at(name));
        rows.push_back(std::move(r));
    }
    for (const auto& g : set.声明.缺口组) {
        新语言结构对应行 r{}; r.语言节点 = set.语言节点; r.种类 = g.种类; r.细目 = g.细目;
        r.状态 = 新语言结构对应状态::缺口; r.种子版本 = set.声明.种子版本;
        rows.push_back(std::move(r));
    }
    return rows;
}
基础组内新节点 形成结构行(const 新语言结构对应行& r) {
    auto n = 新结构节点(结构键::对应表类型);
    加结构字段(n, 结构键::对应表语言, r.语言节点);
    if (r.结构概念节点) 加结构字段(n, 结构键::对应表结构概念, *r.结构概念节点);
    if (r.表达模式节点) 加结构字段(n, 结构键::对应表模式, *r.表达模式节点);
    加结构整数(n, 结构键::对应表种类, r.种类); 加结构整数(n, 结构键::对应表细目, r.细目);
    加结构整数(n, 结构键::对应表状态, r.状态); 加结构整数(n, 结构键::对应表来源, r.来源);
    加结构字段(n, 结构键::对应表版本, 基础值{r.种子版本});
    加结构字段(n, 结构键::对应表概念标识, 基础值{r.结构概念种子标识});
    加结构字段(n, 结构键::对应表模式标识, 基础值{r.模式标识});
    for (auto id : r.语法规则节点组) 加结构字段(n, 结构键::对应表语法, id);
    if(r.实现知识节点)加结构字段(n,结构键::对应表实现知识,*r.实现知识节点);
    return n;
}
知识状态 读采用引用已加锁(稳定编码 lang, 知识读取现场& s, std::optional<新语言结构采用引用>& ref) {
    ref.reset();
    if (!读取语言信息(lang)) return 知识状态::引用不可读;
    const auto a = s.引用组(lang, 结构键::当前结构定义集, 0, 1);
    const auto b = s.引用组(lang, 结构键::当前结构对应视图, 0, 1);
    if (s.状态 != 知识状态::已完成) return s.状态;
    if (a.empty() != b.empty()) { s.拒绝("当前采用缺少定义集或视图"); return s.状态; }
    if (a.empty()) return 知识状态::未找到;
    ref = 新语言结构采用引用{a.front(), b.front()}; return 知识状态::已完成;
}
知识状态 读采用原始已加锁(稳定编码 lang, std::optional<新语言结构采用引用>& ref,
    新语言结构定义集信息& set, std::vector<新语言结构对应行>& rows,
    结构快照& raw, std::vector<稳定编码>& blocked, std::string& diag) {
    rows.clear(); raw.clear(); set = {};
    知识读取现场 s;
    auto state = 读采用引用已加锁(lang, s, ref);
    if (state != 知识状态::已完成) { raw = std::move(s.结构组); diag = s.诊断; return state; }
    结构快照 source;
    state = 读取结构定义集原始已加锁(ref->定义集节点, set, source, blocked, diag);
    if (state != 知识状态::已完成) return state;
    s.结构组.insert(source.begin(), source.end());
    if (set.语言节点 != lang) { diag = "采用集语言不一致"; return 知识状态::内部不一致; }
    const auto view = ref->对应视图节点;
    if (!s.类型(view, 结构键::结构对应视图类型, {结构键::结构对应视图语言,
        结构键::结构对应视图来源集, 结构键::结构对应视图行})) { diag = s.诊断; return s.状态; }
    if (s.引用(view, 结构键::结构对应视图语言) != lang
        || s.引用(view, 结构键::结构对应视图来源集) != ref->定义集节点)
        s.拒绝("采用视图语言或来源集不一致");
    const auto ids = s.引用组(view, 结构键::结构对应视图行);
    auto expected = 派生集对应行(set);
    if (ids.size() != expected.size()) s.拒绝("采用视图不是完整源集对应行");
    else for (std::size_t i = 0; i < ids.size(); ++i) {
        if (!结构字段与声明相同(s, ids[i], 形成结构行(expected[i]))) {
            s.拒绝("采用行与源集声明不一致"); break;
        }
        expected[i].节点 = ids[i];
    }
    if (s.状态 != 知识状态::已完成) { diag = s.诊断; return s.状态; }
    state = 核结构闭包(s.结构组);
    if (state != 知识状态::已完成) { diag = "采用完整范围读取漂移"; return state; }
    rows = std::move(expected); raw = std::move(s.结构组); return 知识状态::已完成;
}
知识状态 完整读取采用(稳定编码 lang, const 新_词类& service,
    std::optional<新语言结构采用引用>& ref, 新语言结构定义集信息& set,
    std::vector<新语言结构对应行>& rows, 结构快照& raw,
    std::vector<稳定编码>& blocked, std::string& diag) {
    {
        std::lock_guard lock(新语言互斥);
        auto state = 知识模式状态已加锁();
        if (state != 知识状态::已完成) return state;
        state = 读采用原始已加锁(lang, ref, set, rows, raw, blocked, diag);
        if (state != 知识状态::已完成) { ref.reset(); rows.clear(); return state; }
    }
    std::vector<语言知识词端点快照> words;
    auto state = 核模式词(set.模式组, service, words, diag);
    if (state != 知识状态::已完成) { ref.reset(); rows.clear(); return state; }
    std::lock_guard lock(新语言互斥);
    state = 复核结构词准备已加锁(raw, words, blocked, diag);
    if (state != 知识状态::已完成) { ref.reset(); rows.clear(); return state; }
    for (const auto& w : words) raw[w.信息.节点] = w.结构;
    return 知识状态::已完成;
}
知识状态 形成结构采用追加已加锁(const 新语言结构采用请求& q,
    const 新语言结构定义集信息& target, const std::vector<语言知识词端点快照>& words,
    基础节点组追加请求& req, std::optional<std::size_t>& viewIndex,
    std::vector<稳定编码>& blocked, std::string& diag) {
    req = {}; viewIndex.reset();
    知识读取现场 s;
    const auto fail = [&](知识状态 state) { req = {}; viewIndex.reset(); if (!s.诊断.empty()) diag = s.诊断; return state; };
    std::optional<新语言结构采用引用> current;
    auto state = 读采用引用已加锁(q.语言节点, s, current);
    if (state != 知识状态::已完成 && state != 知识状态::未找到) return fail(state);
    if (current != q.预期当前采用) { diag = "当前采用与请求预期不符"; return fail(知识状态::读取漂移); }
    新语言结构定义集信息 fresh; 结构快照 raw;
    state = 读取结构定义集原始已加锁(q.目标定义集节点, fresh, raw, blocked, diag);
    if (state != 知识状态::已完成) return fail(state);
    if (fresh.语言节点 != q.语言节点 || fresh.节点 != target.节点 || fresh.声明 != target.声明)
        return fail(知识状态::读取漂移);
    s.结构组.insert(raw.begin(), raw.end());
    if (current) {
        新语言结构定义集信息 old; std::vector<新语言结构对应行> oldRows; 结构快照 oldRaw;
        std::optional<新语言结构采用引用> oldRef;
        state = 读采用原始已加锁(q.语言节点, oldRef, old, oldRows, oldRaw, blocked, diag);
        if (state != 知识状态::已完成) return fail(state);
        s.结构组.insert(oldRaw.begin(), oldRaw.end());
        if (current->定义集节点 == q.目标定义集节点) return 知识状态::已完成;
    }
    const auto rowIds = 查结构反向(s, q.语言节点, 结构键::对应表语言);
    const auto expected = 派生集对应行(fresh);
    std::vector<基础组内字段内容> refs;
    for (const auto& row : expected) {
        const auto n = 形成结构行(row);
        std::optional<稳定编码> found;
        for (auto id : rowIds) if (结构字段与声明相同(s, id, n)) {
            if (found) { s.拒绝("同义对应行存在重复身份"); return fail(s.状态); }
            found = id;
        }
        if (found) refs.push_back(*found);
        else { refs.push_back(基础组内节点引用{req.新节点组.size()}); req.新节点组.push_back(n); }
    }
    if (s.状态 != 知识状态::已完成) return fail(s.状态);
    auto view = 新结构节点(结构键::结构对应视图类型);
    加结构字段(view, 结构键::结构对应视图语言, q.语言节点);
    加结构字段(view, 结构键::结构对应视图来源集, q.目标定义集节点);
    for (const auto& ref : refs) 加结构字段(view, 结构键::结构对应视图行, ref);
    // 已有完整同义视图可以重用；新视图指针必须由同片组内引用追加。
    std::optional<稳定编码> reuseView;
    const auto views = 查结构反向(s, q.目标定义集节点, 结构键::结构对应视图来源集);
    bool allExisting = std::all_of(refs.begin(), refs.end(), [](const auto& r) { return std::holds_alternative<稳定编码>(r); });
    if (allExisting) for (auto id : views) if (结构字段与声明相同(s, id, view)) {
        if (reuseView) { s.拒绝("同义对应视图存在重复身份"); return fail(s.状态); }
        reuseView = id;
    }
    基础组内字段内容 viewRef = 稳定编码{};
    if (reuseView) viewRef = *reuseView;
    else {
        viewIndex = req.新节点组.size(); viewRef = 基础组内节点引用{*viewIndex};
        req.新节点组.push_back(std::move(view));
    }
    for (auto k : {结构键::当前结构定义集, 结构键::当前结构对应视图})
        for (const auto* f : s.字段(q.语言节点, 知识键(k))) req.删除字段关系组.push_back(f->编码);
    req.追加字段组.push_back({q.语言节点, 知识键(结构键::当前结构定义集), q.目标定义集节点});
    req.追加字段组.push_back({q.语言节点, 知识键(结构键::当前结构对应视图), viewRef});
    for (const auto& w : words) s.结构组[w.信息.节点] = w.结构;
    state = 核结构闭包(s.结构组, &req.预期联合节点组);
    if (state != 知识状态::已完成) return fail(state);
    return 知识状态::已完成;
}
} // namespace

新语言结构定义集结果 新_语言类::获取结构定义集(稳定编码 node, const 新_词类& words) const {
    新语言结构定义集结果 r; if (!有效(node)) return r;
    新语言结构定义集信息 info; 结构快照 raw;
    {
        std::lock_guard lock(新语言互斥);
        r.状态 = 知识模式状态已加锁(); if (r.状态 != 知识状态::已完成) return r;
        r.状态 = 读取结构定义集原始已加锁(node, info, raw, r.受阻节点组, r.诊断说明);
        if (r.状态 != 知识状态::已完成) return r;
    }
    std::vector<语言知识词端点快照> snapshots;
    r.状态 = 核模式词(info.模式组, words, snapshots, r.诊断说明);
    if (r.状态 != 知识状态::已完成) return r;
    std::lock_guard lock(新语言互斥);
    r.状态 = 复核结构词准备已加锁(raw, snapshots, r.受阻节点组, r.诊断说明);
    if (r.状态 == 知识状态::已完成) r.信息 = std::move(info);
    return r;
}
新语言结构定义集结果 新_语言类::建立或取得结构定义集(
    const 新语言结构定义集发布请求& request, const 新_词类& words) {
    新语言结构定义集结果 r; auto q = request;
    if (!有效(q.语言节点) || !有效(q.原种子来源节点)) return r;
    r.状态 = 核验结构定义集声明(q.声明, r.诊断说明);
    if (r.状态 != 知识状态::已完成) return r;
    std::vector<语言知识词端点快照> snapshots;
    r.状态 = 准备声明词(q.语言节点, q.声明.模式组, words, snapshots, r.诊断说明);
    if (r.状态 != 知识状态::已完成) return r;
    稳定编码 id;
    {
        std::lock_guard lock(新语言互斥);
        r.状态 = 知识模式状态已加锁(); if (r.状态 != 知识状态::已完成) return r;
        基础节点组追加请求 req; std::optional<std::size_t> index;
        r.状态 = 形成结构定义集追加已加锁(q, snapshots, req, index, r.受阻节点组, r.诊断说明);
        if (r.状态 != 知识状态::已完成) return r;
        if (!index) {
            知识读取现场 s; const auto existing = 找同键定义集(s, q);
            if (s.状态 != 知识状态::已完成 || !existing) {
                r.状态 = s.状态 == 知识状态::已完成 ? 知识状态::内部不一致 : s.状态; return r;
            }
            id = *existing; r.发布种类 = 新语言知识发布种类::已复用;
        } else {
            const auto published = 全局基础数据集.原子追加节点组(req);
            r.状态 = 结构追加状态(published.状态); if (r.状态 != 知识状态::已完成) return r;
            if (published.新节点组.size() != req.新节点组.size() || *index >= published.新节点组.size())
                throw std::logic_error("结构定义集已发布回执缺少实际集根");
            id = published.新节点组[*index]; r.已发布节点 = id; r.发布种类 = 新语言知识发布种类::已建立;
        }
    }
    const auto read = 获取结构定义集(id, words);
    r.状态 = read.状态; r.信息 = read.信息; r.受阻节点组 = read.受阻节点组; r.诊断说明 = read.诊断说明;
    if (r.状态 == 知识状态::已完成 && (!r.信息 || r.信息->声明 != q.声明)) {
        r.信息.reset(); r.状态 = 知识状态::内部不一致; r.诊断说明 = "发布后定义集读回与声明不一致";
    }
    return r;
}
新语言结构采用结果 新_语言类::读取当前结构采用(稳定编码 lang, const 新_词类& words) const {
    新语言结构采用结果 r; if (!有效(lang)) return r;
    新语言结构定义集信息 set; std::vector<新语言结构对应行> rows; 结构快照 raw;
    r.状态 = 完整读取采用(lang, words, r.采用, set, rows, raw, r.受阻节点组, r.诊断说明);
    return r;
}
新语言结构采用结果 新_语言类::采用结构定义集(const 新语言结构采用请求& q, const 新_词类& words) {
    新语言结构采用结果 r;
    if (!有效(q.语言节点) || !有效(q.目标定义集节点)) return r;
    // 当前采用完整读取和词owner核验均在发布锁外准备，锁内只复核中性材料。
    std::optional<新语言结构采用引用> oldRef;
    新语言结构定义集信息 oldSet; std::vector<新语言结构对应行> oldRows; 结构快照 oldRaw;
    r.状态 = 完整读取采用(q.语言节点, words, oldRef, oldSet, oldRows, oldRaw, r.受阻节点组, r.诊断说明);
    if (r.状态 != 知识状态::已完成 && r.状态 != 知识状态::未找到) return r;
    if (oldRef != q.预期当前采用) { r.状态 = 知识状态::读取漂移; return r; }
    const auto target = 获取结构定义集(q.目标定义集节点, words);
    if (target.状态 != 知识状态::已完成 || !target.信息) {
        r.状态 = target.状态; r.受阻节点组 = target.受阻节点组; r.诊断说明 = target.诊断说明; return r;
    }
    if (target.信息->语言节点 != q.语言节点) { r.状态 = 知识状态::输入拒绝; return r; }
    std::vector<语言知识词端点快照> snapshots;
    r.状态 = 核模式词(target.信息->模式组, words, snapshots, r.诊断说明);
    if (r.状态 != 知识状态::已完成) return r;
    {
        std::lock_guard lock(新语言互斥);
        r.状态 = 复核结构词准备已加锁(oldRaw, {}, r.受阻节点组, r.诊断说明);
        if (r.状态 != 知识状态::已完成) return r;
        基础节点组追加请求 req; std::optional<std::size_t> index;
        r.状态 = 形成结构采用追加已加锁(q, *target.信息, snapshots, req, index, r.受阻节点组, r.诊断说明);
        if (r.状态 != 知识状态::已完成) return r;
        if (req.追加字段组.empty()) { r.采用 = oldRef; return r; }
        结构快照 all = oldRaw;
        for (const auto& p : req.预期联合节点组) {
            const auto prev = all.find(p.结构.节点.编码);
            if (prev != all.end() && !基础结构相等(prev->second, p.结构)) { r.状态 = 知识状态::读取漂移; return r; }
            all[p.结构.节点.编码] = p.结构;
        }
        r.状态 = 核结构闭包(all, &req.预期联合节点组);
        if (r.状态 != 知识状态::已完成) return r;
        const auto published = 全局基础数据集.原子追加节点组(req);
        r.状态 = 结构追加状态(published.状态); if (r.状态 != 知识状态::已完成) return r;
        稳定编码 view;
        if (index) {
            if (*index >= published.新节点组.size()) throw std::logic_error("采用发布回执缺少实际视图");
            view = published.新节点组[*index];
        } else {
            const auto* ref = std::get_if<稳定编码>(&req.追加字段组.back().内容);
            if (!ref) throw std::logic_error("采用复用视图不是正式引用");
            view = *ref;
        }
        r.本次已发布 = true; r.采用 = 新语言结构采用引用{q.目标定义集节点, view};
    }
    const auto read = 读取当前结构采用(q.语言节点, words);
    r.状态 = read.状态; r.受阻节点组 = read.受阻节点组; r.诊断说明 = read.诊断说明;
    if (r.状态 == 知识状态::已完成 && read.采用 != r.采用) {
        r.状态 = 知识状态::读取漂移; r.诊断说明 = "发布后当前采用已变化";
    }
    return r;
}


新语言结构概念结果 新_语言类::获取结构概念(稳定编码 id) const {
    新语言结构概念结果 r; if (!有效(id)) return r;
    std::lock_guard lock(新语言互斥); r.状态 = 知识模式状态已加锁();
    if (r.状态 != 知识状态::已完成) return r;
    知识读取现场 s; auto info = 读结构概念(s, id, r.受阻节点组);
    r.状态 = s.状态;
    if (r.状态 == 知识状态::已完成) r.状态 = 核结构闭包(s.结构组);
    if (r.状态 == 知识状态::已完成) r.信息 = std::move(info);
    return r;
}
新语言结构概念列表结果 新_语言类::枚举全部结构概念() const {
    新语言结构概念列表结果 r;
    std::lock_guard lock(新语言互斥); r.状态 = 知识模式状态已加锁();
    if (r.状态 != 知识状态::已完成) return r;
    知识读取现场 s; 稳定编码 catalog;
    r.状态 = 查结构目录(s, catalog); if (r.状态 != 知识状态::已完成) return r;
    std::set<std::pair<std::string, std::string>> keys;
    if (有效(catalog)) for (auto id : 查结构反向(s, catalog, 结构键::结构概念宿主)) {
        if (id == 自然语言组织根节点) continue;
        auto c = 读结构概念(s, id, r.受阻节点组);
        if (!keys.emplace(c.定义.种子标识, c.种子版本).second) s.拒绝("概念枚举发现重复语义键");
        if (s.状态 != 知识状态::已完成) break;
        r.信息组.push_back(std::move(c));
    }
    r.状态 = s.状态;
    if (r.状态 == 知识状态::已完成) r.状态 = 核结构闭包(s.结构组);
    if (r.状态 != 知识状态::已完成) r.信息组.clear();
    return r;
}
新语言结构概念列表结果 新_语言类::枚举结构概念(std::size_t max) const {
    if (max == 0) return {};
    auto r = 枚举全部结构概念();
    if (r.状态 == 知识状态::已完成 && r.信息组.size() > max) { r.状态 = 知识状态::工程未完成; r.信息组.clear(); }
    return r;
}
新语言结构概念结果 新_语言类::建立或取得结构概念(const 新语言结构概念定义& input, const std::string& version) {
    新语言结构概念结果 r; auto d = input;
    if (!规范结构概念(d) || !结构非空UTF8(version)) return r;
    std::lock_guard lock(新语言互斥); r.状态 = 知识模式状态已加锁();
    if (r.状态 != 知识状态::已完成) return r;
    知识读取现场 s; 稳定编码 catalog;
    r.状态 = 查结构目录(s, catalog); if (r.状态 != 知识状态::已完成) return r;
    std::optional<稳定编码> match;
    if (有效(catalog)) for (auto id : 查结构反向(s, catalog, 结构键::结构概念宿主)) {
        if (id == 自然语言组织根节点) continue;
        if (s.原始值<std::string>(id, 结构键::结构概念标识) != d.种子标识
            || s.原始值<std::string>(id, 结构键::结构概念版本) != version) continue;
        if (match) { s.拒绝("结构概念同键重复"); break; }
        match = id;
    }
    if (s.状态 != 知识状态::已完成) { r.状态 = s.状态; return r; }
    稳定编码 id;
    if (match) {
        const auto old = 读结构概念(s, *match, r.受阻节点组);
        if (s.状态 != 知识状态::已完成) { r.状态 = s.状态; return r; }
        if (old.定义 != d) { r.状态 = 知识状态::输入拒绝; return r; }
        id = *match; r.发布种类 = 新语言知识发布种类::已复用;
    } else {
        基础节点组追加请求 req;
        const auto cref = 形成结构目录(req, s);
        const auto ref = 加结构概念(req, cref, d, version);
        if (s.状态 != 知识状态::已完成) { r.状态 = s.状态; return r; }
        r.状态 = 核结构闭包(s.结构组, &req.预期联合节点组);
        if (r.状态 != 知识状态::已完成) return r;
        const auto published = 全局基础数据集.原子追加节点组(req);
        r.状态 = 结构追加状态(published.状态); if (r.状态 != 知识状态::已完成) return r;
        const auto index = std::get<基础组内节点引用>(ref).索引;
        if (index >= published.新节点组.size()) throw std::logic_error("结构概念发布回执缺少节点");
        id = published.新节点组[index]; r.发布种类 = 新语言知识发布种类::已建立;
    }
    知识读取现场 read; auto info = 读结构概念(read, id, r.受阻节点组);
    r.状态 = read.状态;
    if (r.状态 == 知识状态::已完成) r.状态 = 核结构闭包(read.结构组);
    if (r.状态 == 知识状态::已完成) r.信息 = std::move(info);
    return r;
}
新语言表达模式结果 新_语言类::获取表达模式(稳定编码 id, const 新_词类& service) const {
    新语言表达模式结果 r; if (!有效(id)) return r;
    新语言表达模式信息 info; 结构快照 raw;
    {
        std::lock_guard lock(新语言互斥); r.状态 = 知识模式状态已加锁();
        if (r.状态 != 知识状态::已完成) return r;
        知识读取现场 s; info = 读结构模式(s, id, r.受阻节点组);
        r.状态 = s.状态; raw = std::move(s.结构组);
        if (r.状态 != 知识状态::已完成) return r;
    }
    std::string diag; std::vector<语言知识词端点快照> words;
    r.状态 = 核模式词({info}, service, words, diag); if (r.状态 != 知识状态::已完成) return r;
    std::lock_guard lock(新语言互斥);
    r.状态 = 复核结构词准备已加锁(raw, words, r.受阻节点组, diag);
    if (r.状态 == 知识状态::已完成) r.信息 = std::move(info);
    return r;
}
新语言表达模式列表结果 新_语言类::枚举全部表达模式(稳定编码 lang, const 新_词类& service) const {
    新语言表达模式列表结果 r; if (!有效(lang)) return r;
    结构快照 raw;
    {
        std::lock_guard lock(新语言互斥); r.状态 = 知识模式状态已加锁();
        if (r.状态 != 知识状态::已完成) return r;
        if (!读取语言信息(lang)) { r.状态 = 知识状态::引用不可读; return r; }
        知识读取现场 s; (void)s.节点(lang);
        std::set<std::tuple<std::string, std::string, 稳定编码>> keys;
        for (auto id : 查结构反向(s, lang, 结构键::表达模式语言)) {
            auto m = 读结构模式(s, id, r.受阻节点组);
            if (!keys.emplace(m.定义.模式标识, m.种子版本, m.结构概念节点).second) s.拒绝("模式枚举发现重复语义键");
            if (s.状态 != 知识状态::已完成) break;
            r.信息组.push_back(std::move(m));
        }
        r.状态 = s.状态; raw = std::move(s.结构组);
        if (r.状态 != 知识状态::已完成) { r.信息组.clear(); return r; }
    }
    std::vector<语言知识词端点快照> words; std::string diag;
    r.状态 = 核模式词(r.信息组, service, words, diag);
    if (r.状态 == 知识状态::已完成) {
        std::lock_guard lock(新语言互斥); r.状态 = 复核结构词准备已加锁(raw, words, r.受阻节点组, diag);
    }
    if (r.状态 != 知识状态::已完成) r.信息组.clear();
    return r;
}
新语言表达模式列表结果 新_语言类::枚举表达模式(稳定编码 lang, std::size_t max, const 新_词类& words) const {
    if (max == 0) return {};
    auto r = 枚举全部表达模式(lang, words);
    if (r.状态 == 知识状态::已完成 && r.信息组.size() > max) { r.状态 = 知识状态::工程未完成; r.信息组.clear(); }
    return r;
}
新语言表达模式结果 新_语言类::建立或取得表达模式(稳定编码 lang, 稳定编码 conceptId,
    const 新语言表达模式定义& 输入定义, const std::string& version, const 新_词类& service) {
    auto d=输入定义;
    新语言表达模式结果 r;
    if (!有效(lang) || !有效(conceptId) || !结构非空UTF8(version)) return r;
    if(!规范模式含义映射(d))return r;
    const auto conceptRead = 获取结构概念(conceptId);
    if (conceptRead.状态 != 知识状态::已完成 || !conceptRead.信息) {
        r.状态 = conceptRead.状态; r.受阻节点组 = conceptRead.受阻节点组; return r;
    }
    if (conceptRead.信息->种子版本 != version || !核结构模式(d, conceptRead.信息->定义)) return r;
    std::vector<语言知识词端点快照> words; std::string diag;
    r.状态 = 准备声明词(lang, {{"", d}}, service, words, diag);
    if (r.状态 != 知识状态::已完成) return r;
    const auto refs = 声明词引用(d, words);
    稳定编码 id;
    {
        std::lock_guard lock(新语言互斥); r.状态 = 知识模式状态已加锁();
        if (r.状态 != 知识状态::已完成) return r;
        if (!读取语言信息(lang)) { r.状态 = 知识状态::引用不可读; return r; }
        知识读取现场 s; (void)s.节点(lang);
        const auto c = 读结构概念(s, conceptId, r.受阻节点组);
        if (s.状态 != 知识状态::已完成) { r.状态 = s.状态; return r; }
        if (c.定义 != conceptRead.信息->定义 || c.种子版本 != version) { r.状态 = 知识状态::读取漂移; return r; }
        核模式语法已加锁(s, lang, version, d.适用语法编号组);
        if (s.状态 != 知识状态::已完成) { r.状态 = s.状态; return r; }
        const auto n = 形成表达模式节点(lang, conceptId, d, version, refs);
        std::optional<稳定编码> match;
        for (auto node : 查结构反向(s, lang, 结构键::表达模式语言)) {
            if (s.原始值<std::string>(node, 结构键::表达模式标识) != d.模式标识
                || s.原始值<std::string>(node, 结构键::表达模式版本) != version
                || s.引用(node, 结构键::表达模式结构概念) != conceptId) continue;
            if (match) { s.拒绝("模式同键重复"); break; } match = node;
        }
        if (s.状态 != 知识状态::已完成) { r.状态 = s.状态; return r; }
        if (match) {
            const auto old = 读结构模式(s, *match, r.受阻节点组);
            if (s.状态 != 知识状态::已完成) { r.状态 = s.状态; return r; }
            if (old.定义 != d || old.固定词条节点组 != refs || !模式字段与声明相同(s, *match, n)) {
                r.状态 = 知识状态::输入拒绝; return r;
            }
            r.状态 = 复核结构词准备已加锁(s.结构组, words, r.受阻节点组, diag);
            if (r.状态 != 知识状态::已完成) return r;
            id = *match; r.发布种类 = 新语言知识发布种类::已复用;
        } else {
            基础节点组追加请求 req; req.新节点组.push_back(n);追加模式知识映射(req,0,d);
            核模式知识已加锁(s,新语言表达模式信息{{},lang,conceptId,d,version,refs},c.定义);
            if(s.状态!=知识状态::已完成){r.状态=s.状态;return r;}
            for (const auto& w : words) s.结构组[w.信息.节点] = w.结构;
            r.状态 = 核结构闭包(s.结构组, &req.预期联合节点组);
            if (r.状态 != 知识状态::已完成) return r;
            const auto published = 全局基础数据集.原子追加节点组(req);
            r.状态 = 结构追加状态(published.状态); if (r.状态 != 知识状态::已完成) return r;
            if (published.新节点组.size() != req.新节点组.size()) throw std::logic_error("模式发布回执不完整");
            id = published.新节点组.front(); r.发布种类 = 新语言知识发布种类::已建立;
        }
    }
    const auto read = 获取表达模式(id, service);
    r.状态 = read.状态; r.信息 = read.信息; r.受阻节点组 = read.受阻节点组; return r;
}
新语言语法规则结果 新_语言类::获取语法规则(稳定编码 id) const {
    新语言语法规则结果 r; if (!有效(id)) return r;
    std::lock_guard lock(新语言互斥); r.状态 = 知识模式状态已加锁();
    if (r.状态 != 知识状态::已完成) return r;
    知识读取现场 s; auto g = 读结构语法(s, id); r.状态 = s.状态;
    if (r.状态 == 知识状态::已完成) r.状态 = 核结构闭包(s.结构组);
    if (r.状态 == 知识状态::已完成) r.信息 = std::move(g);
    return r;
}
新语言语法规则列表结果 新_语言类::枚举全部语法规则(稳定编码 lang) const {
    新语言语法规则列表结果 r; if (!有效(lang)) return r;
    std::lock_guard lock(新语言互斥); r.状态 = 知识模式状态已加锁();
    if (r.状态 != 知识状态::已完成) return r;
    if (!读取语言信息(lang)) { r.状态 = 知识状态::引用不可读; return r; }
    知识读取现场 s; (void)s.节点(lang); std::set<std::pair<std::string, std::string>> keys;
    for (auto id : 查结构反向(s, lang, 结构键::语法规则语言)) {
        auto g = 读结构语法(s, id);
        if (!keys.emplace(g.定义.编号, g.种子版本).second) s.拒绝("语法枚举发现重复语义键");
        if (s.状态 != 知识状态::已完成) break;
        r.信息组.push_back(std::move(g));
    }
    r.状态 = s.状态;
    if (r.状态 == 知识状态::已完成) r.状态 = 核结构闭包(s.结构组);
    if (r.状态 != 知识状态::已完成) r.信息组.clear();
    return r;
}
新语言语法规则列表结果 新_语言类::枚举语法规则(稳定编码 lang, std::size_t max) const {
    if (max == 0) return {};
    auto r = 枚举全部语法规则(lang);
    if (r.状态 == 知识状态::已完成 && r.信息组.size() > max) { r.状态 = 知识状态::工程未完成; r.信息组.clear(); } return r;
}
新语言语法规则结果 新_语言类::建立或取得语法规则(稳定编码 lang,
    const 新语言语法规则定义& d, const std::string& version) {
    新语言语法规则结果 r;
    if (!有效(lang) || !结构非空UTF8(d.编号) || !结构非空UTF8(version) || !结构枚举合法(d.种类, 3)
        || !新_语言类::严格UTF8字节串有效(d.条件) || !结构非空UTF8(d.结果文本)) return r;
    std::lock_guard lock(新语言互斥); r.状态 = 知识模式状态已加锁();
    if (r.状态 != 知识状态::已完成) return r;
    if (!读取语言信息(lang)) { r.状态 = 知识状态::引用不可读; return r; }
    知识读取现场 s; (void)s.节点(lang);
    const auto n = 形成结构语法(lang, d, version); std::optional<稳定编码> match;
    for (auto id : 查结构反向(s, lang, 结构键::语法规则语言)) {
        if (s.原始值<std::string>(id, 结构键::语法规则编号) != d.编号
            || s.原始值<std::string>(id, 结构键::语法规则版本) != version) continue;
        if (match) { s.拒绝("语法同键重复"); break; } match = id;
    }
    if (s.状态 != 知识状态::已完成) { r.状态 = s.状态; return r; }
    稳定编码 id;
    if (match) {
        (void)读结构语法(s, *match);
        if (s.状态 != 知识状态::已完成) { r.状态 = s.状态; return r; }
        if (!结构字段与声明相同(s, *match, n)) { r.状态 = 知识状态::输入拒绝; return r; }
        id = *match; r.发布种类 = 新语言知识发布种类::已复用;
    } else {
        基础节点组追加请求 req; req.新节点组.push_back(n);
        r.状态 = 核结构闭包(s.结构组, &req.预期联合节点组); if (r.状态 != 知识状态::已完成) return r;
        const auto published = 全局基础数据集.原子追加节点组(req); r.状态 = 结构追加状态(published.状态);
        if (r.状态 != 知识状态::已完成) return r;
        if (published.新节点组.size() != 1) throw std::logic_error("语法发布回执不完整");
        id = published.新节点组.front(); r.发布种类 = 新语言知识发布种类::已建立;
    }
    知识读取现场 read; auto g = 读结构语法(read, id); r.状态 = read.状态;
    if (r.状态 == 知识状态::已完成) r.状态 = 核结构闭包(read.结构组);
    if (r.状态 == 知识状态::已完成) r.信息 = std::move(g);
    return r;
}

新语言量词搭配结果 新_语言类::建立或取得量词搭配(
    稳定编码 语言节点, const std::string& 量词词面,
    const std::string& 种子版本, bool 是回退默认) {
    新语言量词搭配结果 结果;
    if (!有效(语言节点) || !结构非空UTF8(量词词面) || !结构非空UTF8(种子版本))
        return 结果;
    std::lock_guard 锁(新语言互斥);
    结果.状态 = 知识模式状态已加锁();
    if (结果.状态 != 知识状态::已完成) return 结果;
    if (!读取语言信息(语言节点)) { 结果.状态 = 知识状态::引用不可读; return 结果; }
    const auto 全组 = 全局基础数据集.查询字段引用节点结构(
        语言节点, 知识键(知识标识::量词搭配语言));
    if (全组.状态 != 基础结构操作状态::已完成) {
        结果.状态 = 基础知识状态(全组.状态); return 结果;
    }
    for (const auto& 来源 : 全组.来源节点组) {
        知识读取现场 现场;
        if (!现场.类型(来源.节点.编码, 知识标识::量词搭配类型, {
                知识标识::量词搭配语言, 知识标识::量词搭配词面,
                知识标识::量词搭配回退, 知识标识::量词搭配版本})) { 结果.状态 = 现场.状态; return 结果; }
        const auto 词面 = 现场.原始值<std::string>(来源.节点.编码, 知识标识::量词搭配词面);
        const auto 版本 = 现场.原始值<std::string>(来源.节点.编码, 知识标识::量词搭配版本);
        if (现场.状态 != 知识状态::已完成) { 结果.状态 = 现场.状态; return 结果; }
        if (词面 != 量词词面 || 版本 != 种子版本) continue;
        新语言量词搭配信息 信息{};
        信息.节点 = 来源.节点.编码;
        信息.语言节点 = 语言节点;
        信息.量词词面 = 词面;
        信息.是回退默认 = 现场.原始值<std::int64_t>(信息.节点, 知识标识::量词搭配回退) != 0;
        信息.种子版本 = 版本;
        if (信息.是回退默认 != 是回退默认) {
            结果.状态 = 知识状态::内部不一致; return 结果;
        }
        结果.信息 = std::move(信息);
        结果.发布种类 = 新语言知识发布种类::已复用;
        结果.状态 = 知识状态::已完成;
        return 结果;
    }
    基础组内新节点 节点{{{节点类型字段, 知识键(知识标识::量词搭配类型)},
        {知识键(知识标识::量词搭配语言), 语言节点},
        {知识键(知识标识::量词搭配词面), 基础值{量词词面}},
        {知识键(知识标识::量词搭配回退),
            基础值{static_cast<std::int64_t>(是回退默认 ? 1 : 0)}},
        {知识键(知识标识::量词搭配版本), 基础值{种子版本}}}};
    const auto 发布 = 全局基础数据集.原子建立节点组({节点});
    if (发布.状态 != 基础结构操作状态::已完成) {
        结果.状态 = 基础知识状态(发布.状态); return 结果;
    }
    if (发布.节点组.size() != 1 || !有效(发布.节点组.front())) {
        结果.状态 = 知识状态::内部不一致; return 结果;
    }
    新语言量词搭配信息 信息{};
    信息.节点 = 发布.节点组.front();
    信息.语言节点 = 语言节点;
    信息.量词词面 = 量词词面;
    信息.是回退默认 = 是回退默认;
    信息.种子版本 = 种子版本;
    结果.信息 = std::move(信息);
    结果.发布种类 = 新语言知识发布种类::已建立;
    结果.状态 = 知识状态::已完成;
    return 结果;
}

新语言量词搭配列表结果 新_语言类::枚举全部量词搭配(稳定编码 lang) const {
    新语言量词搭配列表结果 r; if (!有效(lang)) return r;
    std::lock_guard lock(新语言互斥); r.状态 = 知识模式状态已加锁();
    if (r.状态 != 知识状态::已完成) return r;
    if (!读取语言信息(lang)) { r.状态 = 知识状态::引用不可读; return r; }
    知识读取现场 s; (void)s.节点(lang);
    for (auto id : 查结构反向(s, lang, 结构键::量词搭配语言)) {
        auto q = 读结构量词(s, id);
        if (q.语言节点 != lang) s.拒绝("量词搭配归属不一致");
        if (s.状态 != 知识状态::已完成) break;
        r.信息组.push_back(std::move(q));
    }
    r.状态 = s.状态;
    if (r.状态 == 知识状态::已完成) r.状态 = 核结构闭包(s.结构组);
    if (r.状态 != 知识状态::已完成) r.信息组.clear();
    else std::sort(r.信息组.begin(), r.信息组.end(), [](const auto& a, const auto& b) { return a.节点 < b.节点; });
    return r;
}
新语言量词搭配列表结果 新_语言类::枚举量词搭配(稳定编码 lang, std::size_t max) const {
    if (max == 0) return {};
    auto r = 枚举全部量词搭配(lang);
    if (r.状态 == 知识状态::已完成 && r.信息组.size() > max) { r.状态 = 知识状态::工程未完成; r.信息组.clear(); }
    return r;
}

新语言量词缺口结果 新_语言类::建立或取得量词缺口(
    稳定编码 语言节点, const std::string& 量词词面,
    const std::string& 样例原文, const std::string& 种子版本) {
    新语言量词缺口结果 结果;
    if (!有效(语言节点) || !结构非空UTF8(量词词面)
        || !结构非空UTF8(样例原文) || !结构非空UTF8(种子版本))
        return 结果;
    std::lock_guard 锁(新语言互斥);
    结果.状态 = 知识模式状态已加锁();
    if (结果.状态 != 知识状态::已完成) return 结果;
    if (!读取语言信息(语言节点)) { 结果.状态 = 知识状态::引用不可读; return 结果; }
    const auto 全组 = 全局基础数据集.查询字段引用节点结构(
        语言节点, 知识键(知识标识::量词缺口语言));
    if (全组.状态 != 基础结构操作状态::已完成) {
        结果.状态 = 基础知识状态(全组.状态); return 结果;
    }
    for (const auto& 来源 : 全组.来源节点组) {
        知识读取现场 现场;
        if (!现场.类型(来源.节点.编码, 知识标识::量词缺口类型, {
                知识标识::量词缺口语言, 知识标识::量词缺口词面,
                知识标识::量词缺口样例, 知识标识::量词缺口版本})) { 结果.状态 = 现场.状态; return 结果; }
        const auto 词面 = 现场.原始值<std::string>(来源.节点.编码, 知识标识::量词缺口词面);
        const auto 版本 = 现场.原始值<std::string>(来源.节点.编码, 知识标识::量词缺口版本);
        if (现场.状态 != 知识状态::已完成) { 结果.状态 = 现场.状态; return 结果; }
        if (词面 != 量词词面 || 版本 != 种子版本) continue;
        新语言量词缺口信息 信息{};
        信息.节点 = 来源.节点.编码;
        信息.语言节点 = 语言节点;
        信息.量词词面 = 词面;
        信息.样例原文 = 现场.原始值<std::string>(信息.节点, 知识标识::量词缺口样例);
        信息.种子版本 = 版本;
        结果.信息 = std::move(信息);
        结果.发布种类 = 新语言知识发布种类::已复用;
        结果.状态 = 知识状态::已完成;
        return 结果;
    }
    基础组内新节点 节点{{{节点类型字段, 知识键(知识标识::量词缺口类型)},
        {知识键(知识标识::量词缺口语言), 语言节点},
        {知识键(知识标识::量词缺口词面), 基础值{量词词面}},
        {知识键(知识标识::量词缺口样例), 基础值{样例原文}},
        {知识键(知识标识::量词缺口版本), 基础值{种子版本}}}};
    const auto 发布 = 全局基础数据集.原子建立节点组({节点});
    if (发布.状态 != 基础结构操作状态::已完成) {
        结果.状态 = 基础知识状态(发布.状态); return 结果;
    }
    if (发布.节点组.size() != 1 || !有效(发布.节点组.front())) {
        结果.状态 = 知识状态::内部不一致; return 结果;
    }
    新语言量词缺口信息 信息{};
    信息.节点 = 发布.节点组.front();
    信息.语言节点 = 语言节点;
    信息.量词词面 = 量词词面;
    信息.样例原文 = 样例原文;
    信息.种子版本 = 种子版本;
    结果.信息 = std::move(信息);
    结果.发布种类 = 新语言知识发布种类::已建立;
    结果.状态 = 知识状态::已完成;
    return 结果;
}

新语言量词缺口列表结果 新_语言类::枚举全部量词缺口(稳定编码 lang) const {
    新语言量词缺口列表结果 r; if (!有效(lang)) return r;
    std::lock_guard lock(新语言互斥); r.状态 = 知识模式状态已加锁();
    if (r.状态 != 知识状态::已完成) return r;
    if (!读取语言信息(lang)) { r.状态 = 知识状态::引用不可读; return r; }
    知识读取现场 s; (void)s.节点(lang);
    for (auto id : 查结构反向(s, lang, 结构键::量词缺口语言)) {
        if (!s.类型(id, 结构键::量词缺口类型, {结构键::量词缺口语言, 结构键::量词缺口词面,
            结构键::量词缺口样例, 结构键::量词缺口版本})) break;
        新语言量词缺口信息 q{}; q.节点 = id; q.语言节点 = s.引用(id, 结构键::量词缺口语言);
        q.量词词面 = s.原始值<std::string>(id, 结构键::量词缺口词面);
        q.样例原文 = s.原始值<std::string>(id, 结构键::量词缺口样例);
        q.种子版本 = s.原始值<std::string>(id, 结构键::量词缺口版本);
        if (q.语言节点 != lang || !结构非空UTF8(q.量词词面)
            || !结构非空UTF8(q.样例原文) || !结构非空UTF8(q.种子版本)) s.拒绝("量词缺口字段或归属不合法");
        if (s.状态 != 知识状态::已完成) break;
        r.信息组.push_back(std::move(q));
    }
    r.状态 = s.状态;
    if (r.状态 == 知识状态::已完成) r.状态 = 核结构闭包(s.结构组);
    if (r.状态 != 知识状态::已完成) r.信息组.clear();
    else std::sort(r.信息组.begin(), r.信息组.end(), [](const auto& a, const auto& b) { return a.节点 < b.节点; });
    return r;
}
新语言量词缺口列表结果 新_语言类::枚举量词缺口(稳定编码 lang, std::size_t max) const {
    if (max == 0) return {};
    auto r = 枚举全部量词缺口(lang);
    if (r.状态 == 知识状态::已完成 && r.信息组.size() > max) { r.状态 = 知识状态::工程未完成; r.信息组.clear(); }
    return r;
}

std::string 新_语言类::解析生成用量词(
    稳定编码 语言节点, const std::optional<std::string>& 期望量词,
    bool& 采用了回退) const {
    采用了回退 = false;
    const auto 列表 = 枚举全部量词搭配(语言节点);
    if (列表.状态 != 知识状态::已完成)
        throw std::runtime_error("生成量词完整读取失败，不能解释为未知搭配");
    std::string 回退 = "个";
    for (const auto& 项 : 列表.信息组)
        if (项.是回退默认) 回退 = 项.量词词面;
    if (!期望量词) { 采用了回退 = true; return 回退; }
    for (const auto& 项 : 列表.信息组)
        if (项.量词词面 == *期望量词) return *期望量词;
    采用了回退 = true;
    return 回退;
}



新语言结构对应结果 新_语言类::建立或取得结构对应缺口(稳定编码 lang,
    新语言结构信息种类 kind, 新语言结构目的细目 detail, const std::string& version) {
    新语言结构对应结果 r;
    if (!有效(lang) || !结构非空UTF8(version) || !结构枚举合法(kind, 11) || !结构枚举合法(detail, 4)) return r;
    std::lock_guard lock(新语言互斥); r.状态 = 知识模式状态已加锁();
    if (r.状态 != 知识状态::已完成) return r;
    if (!读取语言信息(lang)) { r.状态 = 知识状态::引用不可读; return r; }
    知识读取现场 s; (void)s.节点(lang);
    基础节点组追加请求 req;
    const auto declaration = 形成结构缺口声明(lang, {kind, detail}, version);
    std::optional<稳定编码> gap;
    for (auto id : 查结构反向(s, lang, 结构键::结构缺口声明语言))
        if (结构字段与声明相同(s, id, declaration)) {
            if (gap) { s.拒绝("独立缺口声明同键重复"); break; } gap = id;
        }
    if (!gap) req.新节点组.push_back(declaration);
    新语言结构对应行 row{}; row.语言节点 = lang; row.种类 = kind; row.细目 = detail;
    row.状态 = 新语言结构对应状态::缺口; row.种子版本 = version;
    const auto n = 形成结构行(row); std::optional<稳定编码> match;
    for (auto id : 查结构反向(s, lang, 结构键::对应表语言)) if (结构字段与声明相同(s, id, n)) {
        if (match) { s.拒绝("对应缺口行同键重复"); break; } match = id;
    }
    if (s.状态 != 知识状态::已完成) { r.状态 = s.状态; return r; }
    std::optional<std::size_t> index;
    if (!match) { index = req.新节点组.size(); req.新节点组.push_back(n); }
    if (!req.新节点组.empty()) {
        r.状态 = 核结构闭包(s.结构组, &req.预期联合节点组); if (r.状态 != 知识状态::已完成) return r;
        const auto p = 全局基础数据集.原子追加节点组(req); r.状态 = 结构追加状态(p.状态);
        if (r.状态 != 知识状态::已完成) return r;
        if (index) {
            if (*index >= p.新节点组.size()) throw std::logic_error("对应缺口发布回执不完整");
            row.节点 = p.新节点组[*index];
        }
    }
    if (match) row.节点 = *match;
    知识读取现场 read;
    if (!结构字段与声明相同(read, row.节点, n)) { r.状态 = 知识状态::内部不一致; return r; }
    r.状态 = 核结构闭包(read.结构组);
    if (r.状态 == 知识状态::已完成) {
        r.发布种类 = index ? 新语言知识发布种类::已建立 : 新语言知识发布种类::已复用; r.信息 = std::move(row);
    }
    return r;
}
新语言结构对应列表结果 新_语言类::枚举结构对应行(稳定编码 lang, std::size_t max, const 新_词类& words) const {
    新语言结构对应列表结果 r; if (!有效(lang) || max == 0) return r;
    std::optional<新语言结构采用引用> ref; 新语言结构定义集信息 set; 结构快照 raw; std::string diag;
    r.状态 = 完整读取采用(lang, words, ref, set, r.信息组, raw, r.受阻节点组, diag);
    if (r.状态 == 知识状态::已完成 && r.信息组.size() > max) { r.状态 = 知识状态::工程未完成; r.信息组.clear(); }
    return r;
}
新语言结构对应查询结果 新_语言类::查询结构对应_生成(const 新语言结构生成查询& q, const 新_词类& words) const {
    新语言结构对应查询结果 r;
    if (!有效(q.语言节点) || !结构枚举合法(q.种类, 14) || !结构枚举合法(q.细目, 4)) return r;
    std::optional<新语言结构采用引用> ref; 新语言结构定义集信息 set; 结构快照 raw;
    std::vector<新语言结构对应行> rows; std::string diag;
    r.状态 = 完整读取采用(q.语言节点, words, ref, set, rows, raw, r.受阻节点组, diag);
    if (r.状态 != 知识状态::已完成) return r;
    for (const auto& row : rows) if (row.状态 == 新语言结构对应状态::已登记 && row.种类 == q.种类 && row.细目 == q.细目)
        r.信息组.push_back(row);
    return r;
}
新语言结构对应查询结果 新_语言类::查询结构对应_理解(const 新语言结构理解查询& q, const 新_词类& words) const {
    新语言结构对应查询结果 r;
    if (!有效(q.语言节点) || (!q.表达模式节点 && q.模式标识.empty())
        || (q.表达模式节点 && !有效(*q.表达模式节点))) return r;
    std::optional<新语言结构采用引用> ref; 新语言结构定义集信息 set; 结构快照 raw;
    std::vector<新语言结构对应行> rows; std::string diag;
    r.状态 = 完整读取采用(q.语言节点, words, ref, set, rows, raw, r.受阻节点组, diag);
    if (r.状态 != 知识状态::已完成) return r;
    for (const auto& row : rows) if (row.状态 == 新语言结构对应状态::已登记
        && (!q.表达模式节点 || row.表达模式节点 == q.表达模式节点)
        && (q.模式标识.empty() || row.模式标识 == q.模式标识)) r.信息组.push_back(row);
    return r;
}
新语言结构对应列表结果 新_语言类::查询结构对应_覆盖缺口(const 新语言结构覆盖查询& q, const 新_词类& words) const {
    新语言结构对应列表结果 r;
    if (!有效(q.语言节点) || (q.种类过滤 && !结构枚举合法(*q.种类过滤, 11))
        || (q.细目过滤 && !结构枚举合法(*q.细目过滤, 4))) return r;
    std::optional<新语言结构采用引用> ref; 新语言结构定义集信息 set; 结构快照 raw;
    std::vector<新语言结构对应行> rows; std::string diag;
    r.状态 = 完整读取采用(q.语言节点, words, ref, set, rows, raw, r.受阻节点组, diag);
    if (r.状态 != 知识状态::已完成) return r;
    for (const auto& row : rows) if (row.状态 == 新语言结构对应状态::缺口
        && (!q.种类过滤 || row.种类 == *q.种类过滤) && (!q.细目过滤 || row.细目 == *q.细目过滤))
        r.信息组.push_back(row);
    return r;
}
新语言结构对应列表结果 新_语言类::查询结构对应_跨语言(稳定编码 conceptId, const 新_词类& words) const {
    新语言结构对应列表结果 r; if (!有效(conceptId)) return r;
    const auto c = 获取结构概念(conceptId);
    if (c.状态 != 知识状态::已完成) { r.状态 = c.状态; r.受阻节点组 = c.受阻节点组; return r; }
    std::vector<稳定编码> langs; 结构快照 all;
    {
        std::lock_guard lock(新语言互斥); r.状态 = 知识模式状态已加锁();
        if (r.状态 != 知识状态::已完成) return r;
        知识读取现场 s; (void)s.节点(语言组织根节点);
        for (const auto& edge : 全局基础数据集.查询源关系(语言组织根节点, 基础外部关系类型::父子)) {
            if (!读取语言信息(edge.目标节点)) { r.状态 = 知识状态::内部不一致; return r; }
            langs.push_back(edge.目标节点); (void)s.节点(edge.目标节点);
        }
        all = std::move(s.结构组);
    }
    for (auto lang : langs) {
        std::optional<新语言结构采用引用> ref; 新语言结构定义集信息 set;
        std::vector<新语言结构对应行> rows; 结构快照 raw; std::string diag;
        r.状态 = 完整读取采用(lang, words, ref, set, rows, raw, r.受阻节点组, diag);
        if (r.状态 == 知识状态::未找到) continue;
        if (r.状态 != 知识状态::已完成) { r.信息组.clear(); return r; }
        for (const auto& [id, n] : raw) {
            const auto old = all.find(id);
            if (old != all.end() && !基础结构相等(old->second, n)) { r.状态 = 知识状态::读取漂移; r.信息组.clear(); return r; }
            all[id] = n;
        }
        for (const auto& row : rows) if (row.结构概念节点 == conceptId && row.状态 == 新语言结构对应状态::已登记)
            r.信息组.push_back(row);
    }
    std::lock_guard lock(新语言互斥);
    // 已登记语言清单也在语言锁内重核，不能遗漏读取期间新登记的语言。
    std::vector<稳定编码> finalLangs;
    for (const auto& e : 全局基础数据集.查询源关系(语言组织根节点, 基础外部关系类型::父子)) finalLangs.push_back(e.目标节点);
    if (finalLangs != langs) r.状态 = 知识状态::读取漂移;
    else r.状态 = 核结构闭包(all);
    if (r.状态 != 知识状态::已完成) r.信息组.clear(); return r;
}
新语言结构对应重建结果 新_语言类::重建语言结构对应表(稳定编码 lang, const std::string& version, const 新_词类& words) {
    新语言结构对应重建结果 r;
    if (!有效(lang) || !结构非空UTF8(version)) return r;
    std::optional<新语言结构采用引用> ref; 新语言结构定义集信息 set;
    std::vector<新语言结构对应行> rows; 结构快照 raw; std::string diag;
    r.状态 = 完整读取采用(lang, words, ref, set, rows, raw, r.受阻节点组, diag);
    if (r.状态 != 知识状态::已完成) return r;
    if (set.声明.种子版本 != version) { r.状态 = 知识状态::输入拒绝; return r; }
    for (const auto& row : rows) {
        if (row.状态 == 新语言结构对应状态::缺口) ++r.缺口行数;
        else ++r.已登记行数;
        if (row.来源 == 新语言结构来源种类::规则派生) ++r.派生行数;
    }
    return r;
}

新语言结构对应重建结果 新_语言类::派生任务询问对应行(
    稳定编码 语言节点, const std::string& 种子版本, const 新_词类& 词服务) {
    新语言结构对应重建结果 结果;
    if (!有效(语言节点) || !结构非空UTF8(种子版本)) return 结果;
    // 待审：派生句面闭集仅 句末吗 / 什么、怎么样 / 到哪一步了；其它登记缺口。
    // 待审：进度值 PROG1 从既有任务执行阶段信息读取，不在语言层计算。
    static const std::vector<std::string> 陈述骨架 = {
        "SC-任务-进度陈述", "SC-任务-失败陈述", "SC-任务-取消陈述",
    };
    const auto 概念列表 = 枚举全部结构概念();
    if (概念列表.状态 != 知识状态::已完成) {
        结果.状态 = 概念列表.状态; 结果.受阻节点组 = 概念列表.受阻节点组; return 结果;
    }
    std::map<std::string, 新语言结构概念信息> 概念索引;
    for (const auto& c : 概念列表.信息组) {
        if (c.种子版本 != 种子版本) continue;
        if (!概念索引.emplace(c.定义.种子标识, c).second) {
            结果.状态 = 知识状态::内部不一致; return 结果;
        }
    }

    for (const auto& 陈述标识 : 陈述骨架) {
        auto it = 概念索引.find(陈述标识);
        if (it == 概念索引.end()) continue;
        const bool 进度 = 陈述标识.find("进度") != std::string::npos;

        // 填项问：进度 →「到哪一步了」；失败/取消填项不在闭集 → 缺口
        if (进度) {
            新语言结构概念定义 填项 = it->second.定义;
            填项.种子标识 = 陈述标识 + "-填项问派生";
            填项.细目 = 新语言结构目的细目::填项问;
            填项.来源 = 新语言结构来源种类::规则派生;
            auto 填项概念 = 建立或取得结构概念(填项, 种子版本);
            if (填项概念.状态 != 知识状态::已完成 || !填项概念.信息) {
                结果.状态 = 填项概念.状态; 结果.受阻节点组 = 填项概念.受阻节点组; return 结果;
            }
            新语言表达模式定义 填项模式{};
            填项模式.模式标识 = 填项.种子标识 + ".到哪一步了";
            填项模式.槽位表.push_back({"T", false});
            填项模式.组成项组.push_back({新语言表达组成种类::模式槽位, "", "T"});
            填项模式.组成项组.push_back({新语言表达组成种类::固定词面, "进行到", ""});
            填项模式.组成项组.push_back({新语言表达组成种类::固定词面, "哪一步了", ""});
            填项模式.映射组.push_back({"T", "T1"});
            auto 模式发布 = 建立或取得表达模式(
                语言节点, 填项概念.信息->节点, 填项模式, 种子版本, 词服务);
            if (模式发布.状态 != 知识状态::已完成) {
                结果.状态 = 模式发布.状态; 结果.受阻节点组 = 模式发布.受阻节点组; return 结果;
            }
        } else {
            auto 缺口 = 建立或取得结构对应缺口(
                语言节点, 新语言结构信息种类::任务,
                新语言结构目的细目::填项问, 种子版本);
            if (缺口.状态 != 知识状态::已完成) {
                结果.状态 = 缺口.状态; 结果.受阻节点组 = 缺口.受阻节点组; return 结果;
            }
        }

        // 是非问：陈述骨架 + 句末吗
        新语言结构概念定义 是非 = it->second.定义;
        是非.种子标识 = 陈述标识 + "-是非问派生";
        是非.细目 = 新语言结构目的细目::是非问;
        是非.来源 = 新语言结构来源种类::规则派生;
        auto 是非概念 = 建立或取得结构概念(是非, 种子版本);
        if (是非概念.状态 != 知识状态::已完成 || !是非概念.信息) {
            结果.状态 = 是非概念.状态; 结果.受阻节点组 = 是非概念.受阻节点组; return 结果;
        }
        新语言表达模式定义 是非模式{};
        是非模式.模式标识 = 是非.种子标识 + ".吗";
        是非模式.槽位表.push_back({"T", false});
        是非模式.组成项组.push_back({新语言表达组成种类::模式槽位, "", "T"});
        if (进度) {
            是非模式.组成项组.push_back({新语言表达组成种类::固定词面, "进行到", ""});
            // 待审：PROG1 值从任务执行阶段读，语言层不计算
            是非模式.槽位表.push_back({"P", false});
            是非模式.组成项组.push_back({新语言表达组成种类::模式槽位, "", "P"});
            是非模式.映射组.push_back({"P", "PROG1"});
        } else if (陈述标识.find("失败") != std::string::npos) {
            是非模式.组成项组.push_back({新语言表达组成种类::固定词面, "失败", ""});
            是非模式.组成项组.push_back({新语言表达组成种类::固定词面, "了", ""});
        } else {
            是非模式.组成项组.push_back({新语言表达组成种类::固定词面, "取消", ""});
            是非模式.组成项组.push_back({新语言表达组成种类::固定词面, "了", ""});
        }
        是非模式.组成项组.push_back({新语言表达组成种类::固定词面, "吗", ""});
        是非模式.映射组.push_back({"T", "T1"});
        是非模式.适用语法编号组.push_back("H-ORD-05");
        是非模式.适用语法编号组.push_back("H-AGR-06");
        auto 是非模式发布 = 建立或取得表达模式(
            语言节点, 是非概念.信息->节点, 是非模式, 种子版本, 词服务);
        if (是非模式发布.状态 != 知识状态::已完成) {
            结果.状态 = 是非模式发布.状态; 结果.受阻节点组 = 是非模式发布.受阻节点组; return 结果;
        }
    }
    return 重建语言结构对应表(语言节点, 种子版本, 词服务);
}

namespace {
using 采用状态 = 新语言含义采用状态;
using U键 = 接收含义键;
struct 采用角色特征定位 final { std::int64_t 出现, 角色; 稳定编码 特征; };
struct 采用原始信息 final {
    新语言含义采用信息 信息;
    知识读取现场 现场;
    std::vector<采用角色特征定位> 角色特征组;
    std::vector<std::pair<std::int64_t,std::int64_t>> 受请求者定位组;
};

void 核采用技术状态(知识状态 状态) {
    if (状态 == 知识状态::工程未完成) throw 采用状态::资源失败;
    if (状态 == 知识状态::读取漂移 || 状态 == 知识状态::引用不可读)
        throw 采用状态::前提已变化;
    if (状态 != 知识状态::已完成) throw std::logic_error("adopted meaning private structure corrupt");
}
std::vector<const 基础字段关系*> 采用字段(知识读取现场& 现场, 稳定编码 节点, U键 键) {
    auto 结果 = 现场.字段(节点, 采用键(键));
    核采用技术状态(现场.状态);
    return 结果;
}
const 基础字段内容& 采用唯一内容(知识读取现场& 现场, 稳定编码 节点, U键 键) {
    const auto 组 = 采用字段(现场, 节点, 键);
    if (组.size() != 1) throw std::logic_error("adopted meaning field cardinality corrupt");
    return 组[0]->内容;
}
稳定编码 采用引用(知识读取现场& 现场, 稳定编码 节点, U键 键) {
    const auto* 值 = std::get_if<稳定编码>(&采用唯一内容(现场, 节点, 键));
    if (!值 || !有效(*值)) throw std::logic_error("adopted meaning reference invalid");
    (void)现场.节点(*值); 核采用技术状态(现场.状态);
    return *值;
}
template<class T> T 采用材料(知识读取现场& 现场, 稳定编码 节点, U键 键) {
    const auto* 值 = std::get_if<基础值>(&采用唯一内容(现场, 节点, 键));
    const auto* 材料 = 值 ? std::get_if<T>(&值->材料) : nullptr;
    if (!材料) throw std::logic_error("adopted meaning material type invalid");
    return *材料;
}
void 核采用字段白名单(知识读取现场& 现场, 稳定编码 节点, U键 类型,
    std::initializer_list<U键> 允许) {
    const auto* 原 = 现场.节点(节点); 核采用技术状态(现场.状态);
    std::size_t 类型数 = 0;
    std::set<稳定编码> 字段身份;
    for (const auto& 项 : 原->字段组) {
        if (!有效(项.编码) || 项.所属节点 != 节点 || !字段身份.insert(项.编码).second)
            throw std::logic_error("adopted meaning field ownership invalid");
        if (项.字段节点 == 节点类型字段) {
            ++类型数;
            if (项.内容 != 基础字段内容{采用键(类型)})
                throw std::logic_error("adopted meaning node type invalid");
        } else if (std::none_of(允许.begin(), 允许.end(), [&](auto 键) { return 项.字段节点 == 采用键(键); }))
            throw std::logic_error("adopted meaning contains foreign field");
    }
    if (类型数 != 1) throw std::logic_error("adopted meaning node type missing");
}
std::vector<稳定编码> 采用成员(知识读取现场& 现场, 稳定编码 宿主, U键 键) {
    std::vector<稳定编码> 组;
    for (const auto* 项 : 采用字段(现场, 宿主, 键)) {
        const auto* 节点 = std::get_if<稳定编码>(&项->内容);
        if (!节点 || !有效(*节点) || std::find(组.begin(), 组.end(), *节点) != 组.end())
            throw std::logic_error("adopted meaning member invalid or duplicated");
        const auto 反查 = 全局基础数据集.查询字段引用节点结构(*节点, 采用键(键));
        if (反查.状态 == 基础结构操作状态::资源失败) throw 采用状态::资源失败;
        if (反查.状态 != 基础结构操作状态::已完成 || 反查.引用字段组.size() != 1
            || 反查.来源节点组.size() != 1 || 反查.引用字段组[0].所属节点 != 宿主)
            throw std::logic_error("adopted meaning private member has no unique owner");
        const auto* 父 = 现场.节点(宿主); 核采用技术状态(现场.状态);
        if (!基础结构相等(*父, 反查.来源节点组[0])) throw 采用状态::前提已变化;
        组.push_back(*节点);
    }
    return 组;
}
std::pair<std::uint64_t,std::uint64_t> 采用范围(知识读取现场& 现场, 稳定编码 节点, U键 键) {
    const auto 组 = 采用材料<std::vector<std::uint64_t>>(现场, 节点, 键);
    if (组.size() != 2 || 组[0] >= 组[1]) throw std::logic_error("adopted meaning source range invalid");
    return {组[0], 组[1]};
}
bool 规范采用整数(const std::string& 值) {
    const std::size_t 起点 = !值.empty() && 值[0] == '-' ? 1 : 0;
    if (起点 == 值.size() || (值[起点] == '0' && 值.size() - 起点 != 1)
        || (起点 && 值 == "-0")) return false;
    return std::all_of(值.begin()+起点, 值.end(), [](char 字) { return 字 >= '0' && 字 <= '9'; });
}
const 语言图含义出现& 找采用出现(const 语言图含义材料& 图, std::int64_t 编号) {
    const auto 位置 = std::find_if(图.出现组.begin(), 图.出现组.end(), [&](const auto& 项) { return 项.出现编号 == 编号; });
    if (位置 == 图.出现组.end()) throw std::logic_error("adopted meaning occurrence missing");
    return *位置;
}
const 新语言图含义节点& 找采用定义(const 语言图含义材料& 图, std::int64_t 编号) {
    const auto 位置 = std::find_if(图.定义图.节点组.begin(), 图.定义图.节点组.end(), [&](const auto& 项) { return 项.局部编号 == 编号; });
    if (位置 == 图.定义图.节点组.end()) throw std::logic_error("adopted meaning definition missing");
    return *位置;
}
const 语言图绑定值& 采用参数(const 语言图含义材料& 图, const 语言图含义出现& 出现,
    新语言知识参数角色 参数) {
    const auto& 定义 = 找采用定义(图, 出现.定义含义编号);
    const auto p = std::find_if(定义.参数组.begin(), 定义.参数组.end(), [&](const auto& 项) { return 项.参数 == 参数; });
    if (p == 定义.参数组.end()) throw std::logic_error("adopted meaning parameter missing");
    const auto r = std::find_if(出现.角色组.begin(), 出现.角色组.end(), [&](const auto& 项) { return 项.角色编号 == p->规则角色; });
    if (r == 出现.角色组.end()) throw std::logic_error("adopted meaning role binding missing");
    return r->值;
}
bool 是采用行动请求(const 语言图含义材料& 图) {
    const auto& 根=找采用出现(图,图.根出现编号);
    return 找采用定义(图,根.定义含义编号).操作==新语言知识含义操作::行动请求;
}
const 语言图含义出现& 取采用需求出现(const 语言图含义材料& 图) {
    const auto& 根=找采用出现(图,图.根出现编号);
    const auto& 定义=找采用定义(图,根.定义含义编号);
    if(定义.操作==新语言知识含义操作::需求描述) {
        if(根.有效作用域!=新语言知识作用域::普通陈述||图.正式指称作用域)
            throw std::logic_error("plain adopted demand scope invalid");
        return 根;
    }
    if(定义.操作!=新语言知识含义操作::行动请求||定义.作用域!=新语言知识作用域::请求范围
        ||根.有效作用域!=新语言知识作用域::请求范围||定义.参数组.size()!=2
        ||定义.角色组.size()!=2||定义.子含义组.size()!=1
        ||图.定义图.节点组.size()!=3||图.出现组.size()!=3)
        throw std::logic_error("adopted request root profile invalid");
    const auto 参数=std::find_if(定义.参数组.begin(),定义.参数组.end(),[](const auto& p){return p.参数==新语言知识参数角色::目标内容;});
    const auto* 子引用=std::get_if<语言图子出现引用>(&采用参数(图,根,新语言知识参数角色::目标内容));
    if(参数==定义.参数组.end()||!子引用)throw std::logic_error("adopted request expected demand reference missing");
    const auto& 子=找采用出现(图,子引用->出现编号);
    const auto& 子定义=找采用定义(图,子.定义含义编号);
    const auto& 边=定义.子含义组.front();
    if(边.用途!=新语言子含义用途::期望目标||边.本节点角色编号!=参数->规则角色
        ||边.子含义编号!=子.定义含义编号||子定义.操作!=新语言知识含义操作::需求描述
        ||子定义.作用域!=新语言知识作用域::需求目标范围||子.有效作用域!=新语言知识作用域::需求目标范围
        ||!子.原文范围||子.结构项路径.empty()
        ||std::any_of(子.结构项路径.begin(),子.结构项路径.end(),[](auto n){return n<0;}))
        throw std::logic_error("adopted request expected demand edge or occurrence invalid");
    return 子;
}
新语言需求目标投影 投影采用需求(稳定编码 U, const 语言图含义材料& 图,
    const 新接收认知来源键& 来源) {
    using P = 新语言知识参数角色;
    const auto& 根 = 取采用需求出现(图);
    const auto& 定义 = 找采用定义(图, 根.定义含义编号);
    const auto* 主体 = std::get_if<语言图引用值>(&采用参数(图, 根, P::主体));
    const auto* 子引用 = std::get_if<语言图子出现引用>(&采用参数(图, 根, P::目标内容));
    if (!主体 || 主体->类别 != 新语言知识角色类别::存在 || !有效(主体->节点) || !子引用)
        throw std::logic_error("adopted demand subject or target reference invalid");
    const auto& 子 = 找采用出现(图, 子引用->出现编号);
    const auto& 子定义 = 找采用定义(图, 子.定义含义编号);
    const auto 目标参数 = std::find_if(定义.参数组.begin(), 定义.参数组.end(), [](const auto& 项) { return 项.参数 == P::目标内容; });
    if (子.有效作用域 != 新语言知识作用域::需求目标范围
        || std::count_if(定义.子含义组.begin(), 定义.子含义组.end(), [&](const auto& 边) {
            return 边.本节点角色编号 == 目标参数->规则角色 && 边.用途 == 新语言子含义用途::需求目标
                && 边.子含义编号 == 子.定义含义编号;
        }) != 1) throw std::logic_error("adopted demand target edge or scope invalid");
    const auto* E = std::get_if<语言图引用值>(&采用参数(图, 子, P::主体));
    const auto* FT = std::get_if<语言图引用值>(&采用参数(图, 子, P::特征类型));
    if (!E || E->类别 != 新语言知识角色类别::存在 || !有效(E->节点)
        || !FT || !有效(FT->节点) || (FT->类别 != 新语言知识角色类别::I64标量特征类型
            && FT->类别 != 新语言知识角色类别::正式特征类型))
        throw std::logic_error("adopted demand target identity invalid");
    新语言需求目标投影 结果{U, 主体->节点, E->节点, FT->节点};
    结果.需求来源=来源;
    if(是采用行动请求(图)) {
        结果.需求来源.原文起点=根.原文范围->first;
        结果.需求来源.原文终点=根.原文范围->second;
        结果.需求来源.根出现路径=根.结构项路径;
    }
    const auto 数 = [&](P 参数) -> const 语言图整数值& {
        const auto* 值 = std::get_if<语言图整数值>(&采用参数(图, 子, 参数));
        if (!值 || !规范采用整数(值->规范十进制)) throw std::logic_error("adopted demand integer invalid");
        return *值;
    };
    switch (子定义.操作) {
    case 新语言知识含义操作::特征描述: {
        if (!子定义.特征值分支) break;
        const auto& 值 = 数(P::特征值); 结果.准确整数 = 值.规范十进制; 结果.单位 = 值.单位; break;
    }
    case 新语言知识含义操作::特征区间描述: {
        const auto& 下 = 数(P::下界); const auto& 上 = 数(P::上界);
        if (下.单位 != 上.单位) throw std::logic_error("adopted demand interval units differ");
        const auto 比数=[](const std::string& a,const std::string& b) {
            const bool an=a[0]=='-',bn=b[0]=='-';
            if(an!=bn)return an;
            const auto av=std::string_view(a).substr(an?1:0),bv=std::string_view(b).substr(bn?1:0);
            const int c=av.size()!=bv.size()?(av.size()<bv.size()?-1:1):av.compare(bv);
            return an?c>0:c<0;
        };
        if(比数(上.规范十进制,下.规范十进制))throw std::logic_error("adopted demand interval reversed");
        结果.区间下界 = 下.规范十进制; 结果.区间上界 = 上.规范十进制; 结果.单位 = 下.单位; break;
    }
    case 新语言知识含义操作::特征方向描述:
        if (!子定义.方向) throw std::logic_error("adopted demand direction missing");
        结果.方向 = 子定义.方向; break;
    default: throw std::logic_error("adopted demand target operation unsupported");
    }
    return 结果;
}

bool 同采用图(const 语言图含义材料& a,const 语言图含义材料& b) {
    // 原文只保存M材料引用；不把瞬时显示封套误当第二份含义。
    return a.语言节点==b.语言节点 && a.定义图==b.定义图
        && a.根出现编号==b.根出现编号 && a.出现组==b.出现组
        && a.正式指称作用域==b.正式指称作用域;
}
bool 同采用定位(const std::vector<语言图角色原文定位>& a,const std::vector<语言图角色原文定位>& b) {
    return a.size()==b.size() && std::equal(a.begin(),a.end(),b.begin(),[](const auto& x,const auto& y){
        return std::tie(x.出现编号,x.角色编号,x.原文起点,x.原文终点)
            ==std::tie(y.出现编号,y.角色编号,y.原文起点,y.原文终点);
    });
}
bool 同采用请求(const 新语言含义采用信息& a,const 新语言含义采用请求& b) {
    return a.来源==b.来源 && a.场景节点==b.场景节点 && a.说话者节点==b.说话者节点
        && a.现实范围根==b.现实范围根 && a.原文材料节点==b.原文材料节点
        && 同采用图(a.含义,b.含义) && 同采用定位(a.角色原文定位组,b.角色原文定位组);
}
void 核采用图结构(const 新语言含义采用请求& q) {
    const auto& g=q.含义;
    if(!有效(q.来源.接收消息节点)||q.来源.原文起点!=0||q.来源.原文终点==0||!q.来源.根出现路径.empty()
        ||!有效(q.场景节点)||!有效(q.说话者节点)||!有效(q.现实范围根)||!有效(q.原文材料节点)
        ||!有效(g.语言节点)||g.出现组.empty()||g.定义图.节点组.empty()
        ||g.未核验来源语言记录节点||g.正式需求来源||g.被询正式内容)
        throw std::logic_error("adopted graph contains unsupported source envelope");
    std::set<std::int64_t> 定义号,出现号;
    for(const auto& d:g.定义图.节点组)if(d.局部编号<0||!定义号.insert(d.局部编号).second)
        throw std::logic_error("adopted graph definition number invalid");
    if(!定义号.contains(g.定义图.根含义编号))throw std::logic_error("adopted graph definition root missing");
    for(const auto& d:g.定义图.节点组) {
        using P=新语言知识参数角色;
        const bool 仅特征 = d.操作 == 新语言知识含义操作::特征描述
            && d.作用域 == 新语言知识作用域::需求目标范围 && !d.特征值分支;
        std::set<P> 必须;
        switch(d.操作) {
        case 新语言知识含义操作::行动请求:必须={P::受请求者,P::目标内容};break;
        case 新语言知识含义操作::需求描述:必须={P::主体,P::目标内容};break;
        case 新语言知识含义操作::特征描述:
            必须={P::主体,P::特征类型};
            if (!仅特征) 必须.insert(P::特征值);
            break;
        case 新语言知识含义操作::特征区间描述:必须={P::主体,P::特征类型,P::下界,P::上界};break;
        case 新语言知识含义操作::特征方向描述:必须={P::主体,P::特征类型};break;
        default:throw std::logic_error("adopted graph operation outside supported demand profiles");
        }
        if(d.参数组.size()!=必须.size()||d.角色组.size()!=必须.size()||d.确认范围
            ||d.子含义组.size()!=static_cast<std::size_t>(d.操作==新语言知识含义操作::行动请求||d.操作==新语言知识含义操作::需求描述)
            ||(d.特征值分支.has_value()!=(d.操作==新语言知识含义操作::特征描述 && !仅特征))
            ||(d.特征值分支&&d.特征值分支!=新语言知识特征值分支::准确值)
            ||(d.方向.has_value()!=(d.操作==新语言知识含义操作::特征方向描述))
            ||(d.方向&&d.方向!=新语言知识特征方向::增加&&d.方向!=新语言知识特征方向::减少))
            throw std::logic_error("adopted graph definition branch or cardinality corrupt");
        std::set<std::int64_t> 角色;
        for(const auto& p:d.参数组) {
            if(!必须.erase(p.参数)||!角色.insert(p.规则角色).second)throw std::logic_error("adopted graph parameter signature corrupt");
            const auto r=std::find_if(d.角色组.begin(),d.角色组.end(),[&](const auto& x){return x.局部编号==p.规则角色;});
            const auto 种类=p.参数==P::主体||p.参数==P::受请求者 ? 新语言知识角色种类::领域对象引用
                : p.参数==P::目标内容 ? 新语言知识角色种类::子含义引用
                : p.参数==P::特征类型 ? 新语言知识角色种类::特征类型引用 : 新语言知识角色种类::准确值;
            if(r==d.角色组.end()||r->种类!=种类)throw std::logic_error("adopted graph parameter role kind corrupt");
            if(r->必要约束组.size()!=1||r->必要约束组.front().被约束角色!=r->局部编号
                ||r->数量!=新语言知识角色数量::单项)throw std::logic_error("adopted graph role constraint corrupt");
            const auto& 类别=r->必要约束组.front().允许类别组;
            if((种类==新语言知识角色种类::领域对象引用&&类别!=std::vector{新语言知识角色类别::存在})
                ||(种类==新语言知识角色种类::子含义引用&&类别!=std::vector{新语言知识角色类别::子含义})
                ||(种类==新语言知识角色种类::特征类型引用&&类别!=std::vector{新语言知识角色类别::I64标量特征类型}
                    &&类别!=std::vector{新语言知识角色类别::正式特征类型})
                ||(种类==新语言知识角色种类::准确值&&类别!=std::vector{新语言知识角色类别::十进制整数}
                    &&类别!=std::vector{新语言知识角色类别::I64标量}))
                throw std::logic_error("adopted graph role category signature corrupt");
        }
    }
    for(const auto& o:g.出现组)if(o.出现编号<0||!有效(o.所用知识节点)
        ||!出现号.insert(o.出现编号).second||!定义号.contains(o.定义含义编号))
        throw std::logic_error("adopted graph occurrence identity invalid");
    if(!出现号.contains(g.根出现编号)||找采用出现(g,g.根出现编号).定义含义编号!=g.定义图.根含义编号)
        throw std::logic_error("adopted graph occurrence root differs");
    const bool 请求=是采用行动请求(g);
    if(请求) {
        if(g.正式指称作用域!=q.来源.接收消息节点)throw std::logic_error("adopted request reference scope differs from source message");
        const auto& 根=找采用出现(g,g.根出现编号);
        const auto* 受话者=std::get_if<语言图引用值>(&采用参数(g,根,新语言知识参数角色::受请求者));
        if(!受话者||受话者->类别!=新语言知识角色类别::存在||!有效(受话者->节点))
            throw std::logic_error("adopted request addressee binding invalid");
        if(受话者->来源概念||受话者->选用名称关系)
            throw std::logic_error("adopted request addressee is not a bare typed contextual binding");
        if(!根.结构项路径.empty()||!根.原文范围
            ||根.原文范围->first!=q.来源.原文起点||根.原文范围->second!=q.来源.原文终点)
            throw std::logic_error("adopted request root does not cover whole source message");
    }else if(g.正式指称作用域)throw std::logic_error("plain adopted demand contains request reference scope");
    std::set<std::pair<std::int64_t,std::int64_t>> 定位键;
    for(const auto& p:q.角色原文定位组) {
        if(!出现号.contains(p.出现编号)||p.原文起点>=p.原文终点
            ||p.原文起点<q.来源.原文起点||p.原文终点>q.来源.原文终点
            ||!定位键.emplace(p.出现编号,p.角色编号).second)
            throw std::logic_error("adopted graph alignment range invalid");
        const auto& o=找采用出现(g,p.出现编号);
        if(std::none_of(o.角色组.begin(),o.角色组.end(),[&](const auto& r){return r.角色编号==p.角色编号;})
            ||(o.原文范围&&(p.原文起点<o.原文范围->first||p.原文终点>o.原文范围->second)))
            throw std::logic_error("adopted graph alignment role differs");
    }
    std::set<std::int64_t> 活动,已到;
    const auto 访问=[&](auto&& self,std::int64_t n)->void {
        if(活动.contains(n))throw std::logic_error("adopted graph occurrence cycle");
        if(已到.contains(n))return;
        活动.insert(n);const auto& o=找采用出现(g,n);const auto& d=找采用定义(g,o.定义含义编号);
        if(o.有效作用域!=d.作用域||o.角色组.size()!=d.角色组.size()
            ||std::any_of(o.结构项路径.begin(),o.结构项路径.end(),[](auto i){return i<0;})
            ||(o.原文范围&&(o.原文范围->first>=o.原文范围->second
                ||o.原文范围->first<q.来源.原文起点||o.原文范围->second>q.来源.原文终点)))
            throw std::logic_error("adopted graph occurrence scope or shape invalid");
        std::set<std::int64_t> 角色号;
        for(const auto& r:o.角色组) {
            const auto rd=std::find_if(d.角色组.begin(),d.角色组.end(),[&](const auto& x){return x.局部编号==r.角色编号;});
            if(!角色号.insert(r.角色编号).second||rd==d.角色组.end())
                throw std::logic_error("adopted graph role cardinality invalid");
            新语言知识角色类别 类别;
            if(const auto* v=std::get_if<语言图引用值>(&r.值)) {
                类别=v->类别;
                if(!有效(v->节点)||类别<新语言知识角色类别::存在||类别>新语言知识角色类别::正式信息节点
                    ||(v->来源概念&&!有效(*v->来源概念))||(v->选用名称关系&&!有效(*v->选用名称关系))
                    ||(类别==新语言知识角色类别::存在&&!定位键.contains({n,r.角色编号})))
                    throw std::logic_error("adopted graph reference role invalid");
            } else if(const auto* v=std::get_if<语言图整数值>(&r.值)) {
                类别=新语言知识角色类别::十进制整数;
                if(!规范采用整数(v->规范十进制)||(v->单位&&!有效(*v->单位)))
                    throw std::logic_error("adopted graph integer role invalid");
            } else if(const auto* v=std::get_if<语言图子出现引用>(&r.值)) {
                类别=新语言知识角色类别::子含义;
                const auto e=std::find_if(d.子含义组.begin(),d.子含义组.end(),[&](const auto& x){return x.本节点角色编号==r.角色编号;});
                if(e==d.子含义组.end()||!出现号.contains(v->出现编号)
                    ||找采用出现(g,v->出现编号).定义含义编号!=e->子含义编号)
                    throw std::logic_error("adopted graph child edge invalid");
                const auto& 子=找采用出现(g,v->出现编号);
                if(请求&&(!o.原文范围||!子.原文范围
                    ||子.原文范围->first<o.原文范围->first||子.原文范围->second>o.原文范围->second
                    ||子.结构项路径.size()<=o.结构项路径.size()
                    ||!std::equal(o.结构项路径.begin(),o.结构项路径.end(),子.结构项路径.begin())))
                    throw std::logic_error("adopted request child range or structural path is outside parent");
                self(self,v->出现编号);
            } else throw std::logic_error("adopted graph unresolved value is not adoptable");
            const auto& allowed=rd->必要约束组[0].允许类别组;
            if(std::none_of(allowed.begin(),allowed.end(),[&](auto c){return c==类别
                ||(类别==新语言知识角色类别::十进制整数&&c==新语言知识角色类别::I64标量);}))
                throw std::logic_error("adopted graph role violates declared category");
        }
        活动.erase(n);已到.insert(n);
    };
    访问(访问,g.根出现编号);
    if(已到.size()!=g.出现组.size())throw std::logic_error("adopted graph orphan occurrence");
    std::set<std::int64_t> 已用定义;
    for(const auto& o:g.出现组)已用定义.insert(o.定义含义编号);
    if(已用定义!=定义号)throw std::logic_error("adopted graph contains unused definition");
    const auto p=投影采用需求({},g,q.来源);
    if(p.需求主体!=q.说话者节点)throw std::logic_error("adopted graph subject differs from session speaker");
}

bool 同采用联合(const 基础联合节点快照& a,const 基础联合节点快照& b) {
    return 基础结构相等(a.结构,b.结构)&&a.入向字段组==b.入向字段组
        &&a.入向外部关系组==b.入向外部关系组&&a.出向外部关系组==b.出向外部关系组;
}
void 合并采用保护(std::vector<基础联合节点快照>& out,const std::vector<基础联合节点快照>& in) {
    for(const auto& s:in) {
        const auto p=std::find_if(out.begin(),out.end(),[&](const auto& x){return x.结构.节点.编码==s.结构.节点.编码;});
        if(p==out.end())out.push_back(s);
        else if(!同采用联合(*p,s))throw 采用状态::前提已变化;
    }
}
void 复核采用保护(const std::vector<基础联合节点快照>& all) {
    std::vector<稳定编码> ids;for(const auto& x:all)ids.push_back(x.结构.节点.编码);
    const auto r=全局基础数据集.查询节点组完整结构(ids);
    if(r.状态==基础结构操作状态::资源失败)throw 采用状态::资源失败;
    if(r.状态!=基础结构操作状态::已完成||r.节点组.size()!=all.size())throw 采用状态::前提已变化;
    for(const auto& expected:all) {
        const auto p=std::find_if(r.节点组.begin(),r.节点组.end(),[&](const auto& x){return x.查询节点==expected.结构.节点.编码;});
        if(p==r.节点组.end()||!p->快照||!同采用联合(expected,*p->快照))throw 采用状态::前提已变化;
    }
}

std::optional<稳定编码> 查采用源已加锁(const 新接收认知来源键& k) {
    if(!有效(采用键(U键::来源消息)))return {};
    const auto r=全局基础数据集.查询字段引用节点结构(k.接收消息节点,采用键(U键::来源消息));
    if(r.状态==基础结构操作状态::资源失败)throw 采用状态::资源失败;
    if(r.状态!=基础结构操作状态::已完成)throw 采用状态::前提已变化;
    std::optional<稳定编码> found;
    for(const auto& s:r.来源节点组) {
        知识读取现场 t;t.结构组.emplace(s.节点.编码,s);
        if(采用引用(t,s.节点.编码,U键::来源消息)!=k.接收消息节点)
            throw std::logic_error("adopted source reverse reference differs");
        const auto range=采用范围(t,s.节点.编码,U键::来源范围);
        if(range!=std::pair{k.原文起点,k.原文终点}
            ||采用材料<std::vector<std::int64_t>>(t,s.节点.编码,U键::来源路径)!=k.根出现路径)continue;
        if(found)throw std::logic_error("adopted source has duplicate meaning roots");
        found=s.节点.编码;
    }
    return found;
}

采用原始信息 读取采用原始已加锁(稳定编码 U) {
    if (!有效(U) || !有效(采用键(U键::采用类型))) throw 采用状态::未找到;
    const auto 初 = 全局基础数据集.查询完整节点结构(U);
    if (初.状态 == 基础结构操作状态::节点不存在) throw 采用状态::未找到;
    if (初.状态 == 基础结构操作状态::资源失败) throw 采用状态::资源失败;
    if (初.状态 != 基础结构操作状态::已完成 || !初.结构) throw std::logic_error("adopted meaning root read failed");
    const auto 类型 = std::find_if(初.结构->字段组.begin(), 初.结构->字段组.end(), [](const auto& 项) {
        return 项.字段节点 == 节点类型字段 && 项.内容 == 基础字段内容{采用键(U键::采用类型)};
    });
    if (类型 == 初.结构->字段组.end()) throw 采用状态::未找到;
    采用原始信息 结果;
    auto& 信息 = 结果.信息; auto& s = 结果.现场;
    s.结构组.emplace(U, *初.结构); 信息.节点 = U;
    for(const auto k:接收含义句柄)(void)s.节点(k);
    for(const auto k:知识句柄)(void)s.节点(k);
    (void)s.节点(节点类型字段);核采用技术状态(s.状态);
    核采用字段白名单(s, U, U键::采用类型, {U键::来源消息,U键::来源范围,U键::来源路径,
        U键::语言,U键::场景,U键::现实根,U键::原文材料,U键::根出现编号,U键::定义图,U键::出现成员,U键::定位成员});
    信息.来源.接收消息节点 = 采用引用(s,U,U键::来源消息);
    const auto 范围 = 采用范围(s,U,U键::来源范围);
    信息.来源.原文起点 = 范围.first; 信息.来源.原文终点 = 范围.second;
    信息.来源.根出现路径 = 采用材料<std::vector<std::int64_t>>(s,U,U键::来源路径);
    信息.场景节点 = 采用引用(s,U,U键::场景); 信息.现实范围根 = 采用引用(s,U,U键::现实根);
    信息.原文材料节点 = 采用引用(s,U,U键::原文材料);
    auto& 图 = 信息.含义; 图.语言节点 = 采用引用(s,U,U键::语言);
    图.根出现编号 = 采用材料<std::int64_t>(s,U,U键::根出现编号);
    const auto 定义根 = 采用引用(s,U,U键::定义图);
    // 固定图复用知识图字段，但不是知识目标，不使用U字段的空白名单。
    const auto* 图结构 = s.节点(定义根); 核采用技术状态(s.状态);
    std::size_t 图类型数=0;
    for (const auto& 字段 : 图结构->字段组) {
        if (字段.字段节点==节点类型字段) {
            ++图类型数;
            if (字段.内容!=基础字段内容{采用键(U键::定义图类型)})
                throw std::logic_error("adopted definition graph type invalid");
        } else if (字段.字段节点!=知识键(知识标识::目标图格式)
            && 字段.字段节点!=知识键(知识标识::目标图根编号)
            && 字段.字段节点!=知识键(知识标识::目标图成员))
            throw std::logic_error("adopted definition graph contains foreign field");
    }
    if (图类型数!=1 || 采用成员(s,U,U键::定义图)!=std::vector{定义根})
        throw std::logic_error("adopted definition graph ownership invalid");
    图.定义图 = 读含义图(s, 定义根); 核采用技术状态(s.状态);
    const auto 出现节点 = 采用成员(s,U,U键::出现成员);
    std::map<稳定编码,std::int64_t> 编号;
    std::map<稳定编码,std::vector<std::pair<std::int64_t,稳定编码>>> 子引用组;
    for (const auto 节点 : 出现节点) {
        核采用字段白名单(s,节点,U键::出现类型,{U键::出现宿主,U键::出现编号,U键::出现知识,
            U键::出现定义编号,U键::出现作用域,U键::出现路径,U键::出现范围,U键::出现角色});
        if (采用引用(s,节点,U键::出现宿主) != U) throw std::logic_error("meaning occurrence owner differs");
        语言图含义出现 项;
        项.出现编号=采用材料<std::int64_t>(s,节点,U键::出现编号);
        if (!编号.emplace(节点,项.出现编号).second || std::any_of(图.出现组.begin(),图.出现组.end(),[&](const auto& 前){return 前.出现编号==项.出现编号;}))
            throw std::logic_error("meaning occurrence number duplicated");
        项.所用知识节点=采用引用(s,节点,U键::出现知识);
        项.定义含义编号=采用材料<std::int64_t>(s,节点,U键::出现定义编号);
        项.有效作用域=static_cast<新语言知识作用域>(采用材料<std::int64_t>(s,节点,U键::出现作用域));
        项.结构项路径=采用材料<std::vector<std::int64_t>>(s,节点,U键::出现路径);
        if (!采用字段(s,节点,U键::出现范围).empty()) {
            const auto r=采用范围(s,节点,U键::出现范围); 项.原文范围=std::pair<std::size_t,std::size_t>{r.first,r.second};
        }
        for (const auto 角色 : 采用成员(s,节点,U键::出现角色)) {
            核采用字段白名单(s,角色,U键::角色类型,{U键::角色宿主,U键::角色编号,U键::值标签,U键::引用类别,
                U键::实际角色特征,U键::引用节点,U键::来源概念,U键::选用名称,U键::整数文本,U键::整数单位,U键::子出现});
            if (采用引用(s,角色,U键::角色宿主)!=节点) throw std::logic_error("meaning role owner differs");
            语言图角色绑定 绑定; 绑定.角色编号=采用材料<std::int64_t>(s,角色,U键::角色编号);
            const auto 标签=采用材料<std::int64_t>(s,角色,U键::值标签);
            const auto 有=[&](U键 键){return !采用字段(s,角色,键).empty();};
            if (标签==1) {
                语言图引用值 值;
                值.类别=static_cast<新语言知识角色类别>(采用材料<std::int64_t>(s,角色,U键::引用类别));
                if (值.类别==新语言知识角色类别::存在) {
                    if (有(U键::引用节点)) throw std::logic_error("meaning existence role mixes raw reference and feature");
                    结果.角色特征组.push_back({项.出现编号,绑定.角色编号,采用引用(s,角色,U键::实际角色特征)});
                } else {
                    if (有(U键::实际角色特征)) throw std::logic_error("meaning nonexistence role owns existence feature");
                    值.节点=采用引用(s,角色,U键::引用节点);
                }
                if (有(U键::来源概念)) 值.来源概念=采用引用(s,角色,U键::来源概念);
                if (有(U键::选用名称)) 值.选用名称关系=采用引用(s,角色,U键::选用名称);
                if (有(U键::整数文本)||有(U键::整数单位)||有(U键::子出现)) throw std::logic_error("meaning reference mixes value tags");
                绑定.值=值;
            } else if (标签==4) {
                const auto 类别=采用材料<std::int64_t>(s,角色,U键::引用类别);
                if(类别!=static_cast<std::int64_t>(新语言知识角色类别::存在)
                    ||有(U键::实际角色特征)||有(U键::来源概念)||有(U键::选用名称)
                    ||有(U键::整数文本)||有(U键::整数单位)||有(U键::子出现))
                    throw std::logic_error("meaning addressee reference has illegal category or payload");
                const auto* e=std::get_if<稳定编码>(&采用唯一内容(s,角色,U键::引用节点));
                if(!e||!有效(*e))throw std::logic_error("meaning addressee reference invalid");
                // 历史绑定只恢复强类型引用，不在本读口反查E今日的存在结构。
                绑定.值=语言图引用值{新语言知识角色类别::存在,*e,{}, {}};
                结果.受请求者定位组.emplace_back(项.出现编号,绑定.角色编号);
            } else if (标签==2) {
                语言图整数值 值{采用材料<std::string>(s,角色,U键::整数文本),{}};
                if (!规范采用整数(值.规范十进制)) throw std::logic_error("meaning integer is not canonical");
                if (有(U键::整数单位)) 值.单位=采用引用(s,角色,U键::整数单位);
                if (有(U键::引用类别)||有(U键::实际角色特征)||有(U键::引用节点)||有(U键::来源概念)||有(U键::选用名称)||有(U键::子出现))
                    throw std::logic_error("meaning integer mixes value tags");
                绑定.值=std::move(值);
            } else if (标签==3) {
                if (有(U键::引用类别)||有(U键::实际角色特征)||有(U键::引用节点)||有(U键::来源概念)||有(U键::选用名称)||有(U键::整数文本)||有(U键::整数单位))
                    throw std::logic_error("meaning child mixes value tags");
                子引用组[节点].push_back({绑定.角色编号,采用引用(s,角色,U键::子出现)});
                绑定.值=语言图子出现引用{};
            } else throw std::logic_error("meaning role value tag unsupported");
            项.角色组.push_back(std::move(绑定));
        }
        图.出现组.push_back(std::move(项));
    }
    for (const auto& [父,子组] : 子引用组) for (const auto& [角色,子] : 子组) {
        if (!编号.contains(子)) throw std::logic_error("meaning child outside occurrence owner");
        auto& 出现=*std::find_if(图.出现组.begin(),图.出现组.end(),[&](const auto& x){return x.出现编号==编号.at(父);});
        auto& 绑定=*std::find_if(出现.角色组.begin(),出现.角色组.end(),[&](const auto& x){return x.角色编号==角色;});
        绑定.值=语言图子出现引用{编号.at(子)};
    }
    for (const auto 节点 : 采用成员(s,U,U键::定位成员)) {
        核采用字段白名单(s,节点,U键::定位类型,{U键::定位宿主,U键::定位出现,U键::定位角色,U键::定位范围});
        const auto 出现=采用引用(s,节点,U键::定位出现);
        if (采用引用(s,节点,U键::定位宿主)!=U || !编号.contains(出现)) throw std::logic_error("meaning alignment owner invalid");
        const auto r=采用范围(s,节点,U键::定位范围);
        信息.角色原文定位组.push_back({编号.at(出现),采用材料<std::int64_t>(s,节点,U键::定位角色),r.first,r.second});
    }
    const auto 登记=采用字段(s,图.语言节点,U键::语言采用登记);
    if (std::count_if(登记.begin(),登记.end(),[&](const auto* f){return f->内容==基础字段内容{U};})!=1)
        throw std::logic_error("meaning language registration missing");
    return 结果;
}
} // namespace

新语言含义采用结果 新_语言类::读取采用含义(稳定编码 U,const 新_特征类& 特征) const {
    try {
        采用原始信息 原;
        std::vector<基础联合节点快照> 保护;
        {
            std::lock_guard 锁(新语言互斥);
            原=读取采用原始已加锁(U);
            核采用技术状态(核结构闭包(原.现场.结构组,&保护));
        }
        auto& g=原.信息.含义;
        const auto& graphRoot=找采用出现(g,g.根出现编号);
        const auto& root=取采用需求出现(g);
        const auto* childPointer=std::get_if<语言图子出现引用>(&采用参数(g,root,新语言知识参数角色::目标内容));
        if(!childPointer)throw std::logic_error("adopted demand target is not a child occurrence");
        const auto& child=*childPointer;
        if(是采用行动请求(g)) {
            const auto& def=找采用定义(g,graphRoot.定义含义编号);
            const auto rp=std::find_if(def.参数组.begin(),def.参数组.end(),[](const auto& p){return p.参数==新语言知识参数角色::受请求者;});
            if(rp==def.参数组.end()||原.受请求者定位组!=std::vector{std::pair{graphRoot.出现编号,rp->规则角色}})
                throw std::logic_error("adopted request addressee tag missing or misplaced");
            // 历史采用只恢复已绑定强类型引用，不声称今日E身份仍可读。
            g.正式指称作用域=原.信息.来源.接收消息节点;
        }else if(!原.受请求者定位组.empty())throw std::logic_error("plain adopted demand contains addressee tag");
        const auto subjectRole=[&](std::int64_t n) {
            const auto& d=找采用定义(g,找采用出现(g,n).定义含义编号);
            const auto p=std::find_if(d.参数组.begin(),d.参数组.end(),[](const auto& x){return x.参数==新语言知识参数角色::主体;});
            if(p==d.参数组.end())throw std::logic_error("adopted existence subject role missing");
            return p->规则角色;
        };
        if(原.角色特征组.size()!=2)throw std::logic_error("adopted demand must own two existence role features");
        std::set<稳定编码> 已读F;
        for(const auto& p:原.角色特征组) {
            const bool isH=p.出现==root.出现编号&&p.角色==subjectRole(root.出现编号);
            const bool isE=p.出现==child.出现编号&&p.角色==subjectRole(child.出现编号);
            if((!isH&&!isE)||!已读F.insert(p.特征).second)throw std::logic_error("adopted existence role feature reused or misplaced");
            const auto f=特征.读取特征完整保护(p.特征);
            if(f.状态==新特征完整读取状态::资源失败)throw 采用状态::资源失败;
            if(f.状态==新特征完整读取状态::前提已变化)throw 采用状态::前提已变化;
            if(f.状态!=新特征完整读取状态::已读取||!f.信息||f.信息->完整保护组.empty())
                throw std::logic_error("adopted existence role feature unreadable");
            const auto& fi=f.信息->特征;
            const auto* e = fi.当前值 ? std::get_if<稳定编码>(&*fi.当前值) : nullptr;
            if(!e||!有效(*e)||fi.上级特征节点||fi.历史不同值!=std::vector<新特征准确值>{新特征准确值{*e}})
                throw std::logic_error("adopted existence role feature value changed");
            // FT语义由先天槽正式owner核验，下面使用其已初始化的槽身份。
            概念_特征类 槽读取;
            const auto ft=槽读取.读取先天特征类型槽(isH?先天特征类型槽::需求主体:先天特征类型槽::需求目标存在);
            if(ft.状态==特征概念规则状态::资源失败)throw 采用状态::资源失败;
            if(ft.状态!=特征概念规则状态::已完成||!ft.类型根||*ft.类型根!=f.信息->特征类型根)
                throw std::logic_error("adopted existence role feature type differs");
            合并采用保护(保护,ft.完整保护组);合并采用保护(保护,f.信息->完整保护组);
            auto& o=*std::find_if(g.出现组.begin(),g.出现组.end(),[&](const auto& x){return x.出现编号==p.出现;});
            auto& r=*std::find_if(o.角色组.begin(),o.角色组.end(),[&](const auto& x){return x.角色编号==p.角色;});
            std::get<语言图引用值>(r.值).节点=*e;
            if(isH)原.信息.说话者节点=*e;
        }
        新语言含义采用请求 q{原.信息.来源,原.信息.场景节点,原.信息.说话者节点,
            原.信息.现实范围根,原.信息.原文材料节点,g,原.信息.角色原文定位组};
        核采用图结构(q);
        {
            std::lock_guard 锁(新语言互斥);
            复核采用保护(保护);
            const auto same=查采用源已加锁(原.信息.来源);
            if(!same||*same!=U)throw std::logic_error("adopted source registration differs");
        }
        return {采用状态::已采用,std::move(原.信息),std::move(保护)};
    } catch(采用状态 s){return {s,{},{}};}
    catch(const std::bad_alloc&){return {采用状态::资源失败,{},{}};}
    catch(const std::length_error&){return {采用状态::资源失败,{},{}};}
}

新语言含义采用结果 新_语言类::读取接收采用含义(const 新接收认知来源键& k,const 新_特征类& f) const {
    try {
        if(!有效(k.接收消息节点)||k.原文起点!=0||k.原文终点==0||!k.根出现路径.empty())return {};
        std::optional<稳定编码> u;
        {std::lock_guard 锁(新语言互斥);u=查采用源已加锁(k);}
        if(!u)return {采用状态::未找到,{},{}};
        auto r=读取采用含义(*u,f);
        if(r.信息&&r.信息->来源!=k)throw std::logic_error("adopted source lookup returned another source");
        return r;
    } catch(采用状态 s){return {s,{},{}};}
    catch(const std::bad_alloc&){return {采用状态::资源失败,{},{}};}
    catch(const std::length_error&){return {采用状态::资源失败,{},{}};}
}

新语言需求目标投影结果 新_语言类::读取采用需求目标(稳定编码 u,const 新_特征类& f) const {
    auto r=读取采用含义(u,f);
    if(r.状态!=采用状态::已采用)return {r.状态,{},{}};
    if(!r.信息)throw std::logic_error("adopted demand read succeeded without payload");
    if(是采用行动请求(r.信息->含义))return {采用状态::输入拒绝,{},{}};
    return {r.状态,投影采用需求(u,r.信息->含义,r.信息->来源),std::move(r.完整保护)};
}

新语言需求目标投影结果 新_语言类::读取采用指定需求目标(
    稳定编码 u,const 新接收认知来源键& k,const 新_特征类& f) const {
    try {
        if(!有效(u)||!有效(k.接收消息节点)||k.原文起点>=k.原文终点
            ||std::any_of(k.根出现路径.begin(),k.根出现路径.end(),[](auto n){return n<0;}))return {};
        auto r=读取采用含义(u,f);
        if(r.状态!=采用状态::已采用)return {r.状态,{},{}};
        if(!r.信息||r.完整保护.empty())throw std::logic_error("specified adopted demand read lacks protected payload");
        auto p=投影采用需求(u,r.信息->含义,r.信息->来源);
        if(p.需求来源!=k)return {采用状态::输入拒绝,{},{}};
        return {采用状态::已采用,std::move(p),std::move(r.完整保护)};
    }catch(采用状态 s){return {s,{},{}};}
    catch(const std::bad_alloc&){return {采用状态::资源失败,{},{}};}
    catch(const std::length_error&){return {采用状态::资源失败,{},{}};}
}

新语言行动请求投影结果 新_语言类::读取采用行动请求保护(
    稳定编码 u,稳定编码 e,const 新_特征类& f,const 新_存在类& 存在) const {
    try {
        if(!有效(u)||!有效(e))return {};
        auto r=读取采用含义(u,f);
        if(r.状态!=采用状态::已采用)return {r.状态,{},{}};
        if(!r.信息||r.完整保护.empty())throw std::logic_error("adopted request read lacks protected payload");
        const auto& g=r.信息->含义;
        if(!是采用行动请求(g))return {采用状态::输入拒绝,{},{}};
        const auto& 根=找采用出现(g,g.根出现编号);
        const auto* 受话者=std::get_if<语言图引用值>(&采用参数(g,根,新语言知识参数角色::受请求者));
        if(!受话者||受话者->类别!=新语言知识角色类别::存在||!有效(受话者->节点))
            throw std::logic_error("adopted request addressee reference corrupt");
        if(受话者->节点!=e)return {采用状态::输入拒绝,{},{}};
        const auto 身份=存在.读取存在身份保护(e);
        if(身份.状态==基础结构操作状态::资源失败)return {采用状态::资源失败,{},{}};
        if(身份.前提已变化||身份.状态==基础结构操作状态::节点不存在)return {采用状态::前提已变化,{},{}};
        if(身份.状态!=基础结构操作状态::已完成||!身份.信息||身份.信息->存在节点!=e||身份.预期节点组.empty())
            throw std::logic_error("adopted request addressee identity unreadable");
        合并采用保护(r.完整保护,身份.预期节点组);
        auto p=投影采用需求(u,g,r.信息->来源);
        新语言行动请求投影 结果{u,r.信息->说话者节点,e,r.信息->来源,p.需求来源,std::move(p)};
        复核采用保护(r.完整保护);
        return {采用状态::已采用,std::move(结果),std::move(r.完整保护)};
    }catch(采用状态 s){return {s,{},{}};}
    catch(const std::bad_alloc&){return {采用状态::资源失败,{},{}};}
    catch(const std::length_error&){return {采用状态::资源失败,{},{}};}
}

新语言含义采用结果 新_语言类::建立或取得接收采用含义(
    const 新语言含义采用请求& q,新_特征类& f,概念_特征类& ft,
    const 新语言采用来源读取接口& source) {
    std::vector<稳定编码> candidates;
    bool publicationAttempted=false;
    const auto 清理未采用=[&]() {
        if(candidates.empty())return;
        // 只有确认没有采用这些F才清理；正式准备口检查全部外部引用。
        auto cleanup=f.准备特征组清理(candidates,{});
        if(cleanup.状态!=新特征清理准备状态::已准备||!cleanup.准备)
            throw std::logic_error("unadopted meaning role features cannot be safely reclaimed");
        auto prepared=std::move(*cleanup.准备);
        f.执行已准备特征组清理(prepared);candidates.clear();
    };
    const auto existing=[&]() {
        auto r=读取接收采用含义(q.来源,f);
        if(r.状态==采用状态::已采用) {
            if(!r.信息)throw std::logic_error("adopted meaning source read has no payload");
            r.状态=同采用请求(*r.信息,q)?采用状态::精确重复:采用状态::来源冲突;
        }
        return r;
    };
    try {
        if(!有效(q.来源.接收消息节点)||q.来源.原文起点!=0
            ||q.来源.原文终点==0||!q.来源.根出现路径.empty()
            ||!有效(q.场景节点)||!有效(q.说话者节点)||!有效(q.现实范围根)||!有效(q.原文材料节点))return {};
        核采用图结构(q);
        auto prior=existing();
        if(prior.状态!=采用状态::未找到)return prior;
        const auto 核来源=[&](const 新语言采用来源保护& s) {
            if(s.状态!=采用状态::已采用&&s.状态!=采用状态::精确重复)throw s.状态;
            if(s.来源!=q.来源||s.场景节点!=q.场景节点||s.说话者节点!=q.说话者节点
                ||s.现实范围根!=q.现实范围根||s.原文材料节点!=q.原文材料节点
                ||s.公开宿主会话.empty()||s.对话框标识.empty()||s.完整保护.empty()
                ||!同采用图(s.唯一解释,q.含义)||!同采用定位(s.角色原文定位组,q.角色原文定位组))
                throw 采用状态::前提已变化;
            std::int64_t count=0;
            if(!知识来源UTF8有效(s.固定原文,count)||static_cast<std::uint64_t>(count)!=q.来源.原文终点
                ||(q.含义.固定原文&&*q.含义.固定原文!=s.固定原文))
                throw 采用状态::前提已变化;
            if(s.特征概念依据组.empty())throw std::logic_error("adoption lacks actual feature definition protection");
            if(是采用行动请求(q.含义)) {
                const auto& 根=找采用出现(q.含义,q.含义.根出现编号);
                const auto& e=std::get<语言图引用值>(采用参数(q.含义,根,新语言知识参数角色::受请求者));
                if(s.受话者节点!=e.节点)throw 采用状态::前提已变化;
            }else if(s.受话者节点)throw 采用状态::前提已变化;
        };
        auto before=source.读取采用来源保护(q.来源,q.场景节点);核来源(before);
        const auto p=投影采用需求({},q.含义,q.来源);
        const auto& root=取采用需求出现(q.含义);
        const auto& child=std::get<语言图子出现引用>(采用参数(q.含义,root,新语言知识参数角色::目标内容));
        struct F角色 final {std::int64_t 出现,角色;稳定编码 节点,E,FT;};
        std::vector<F角色> roles;
        candidates.reserve(2);roles.reserve(2);
        for(const auto [n,e,slot]:{std::tuple{root.出现编号,p.需求主体,先天特征类型槽::需求主体},
                std::tuple{child.出现编号,p.目标存在,先天特征类型槽::需求目标存在}}) {
            const auto type=ft.读取先天特征类型槽(slot);
            if(type.状态==特征概念规则状态::资源失败)throw 采用状态::资源失败;
            if(type.状态!=特征概念规则状态::已完成||!type.类型根||type.完整保护组.empty())
                throw std::logic_error("adopted existence role innate feature slot unavailable");
            const auto created=f.建立特征强类型(*type.类型根,std::nullopt);
            if(created.状态==新特征写入状态::资源失败)throw 采用状态::资源失败;
            if(created.状态==新特征写入状态::前提已变化)throw 采用状态::前提已变化;
            if(created.状态==新特征写入状态::入口拒绝
                ||created.状态==新特征写入状态::材料类型不相容)throw 采用状态::输入拒绝;
            if(created.状态!=新特征写入状态::已完成||!created.特征节点)
                throw std::logic_error("adopted existence role feature creation failed");
            candidates.push_back(*created.特征节点);
            const auto written=f.添加特征值(*created.特征节点,新特征准确值{e});
            if(written.状态==新特征写入状态::资源失败)throw 采用状态::资源失败;
            if(written.状态==新特征写入状态::前提已变化)throw 采用状态::前提已变化;
            if(written.状态==新特征写入状态::入口拒绝
                ||written.状态==新特征写入状态::材料类型不相容)throw 采用状态::输入拒绝;
            if(written.状态!=新特征写入状态::已完成&&written.状态!=新特征写入状态::无变化)
                throw std::logic_error("adopted existence role feature value publication failed");
            const auto& def=找采用定义(q.含义,找采用出现(q.含义,n).定义含义编号);
            const auto r=std::find_if(def.参数组.begin(),def.参数组.end(),[](const auto& x){return x.参数==新语言知识参数角色::主体;});
            if(r==def.参数组.end())throw std::logic_error("adopted role subject definition missing");
            roles.push_back({n,r->规则角色,*created.特征节点,e,*type.类型根});
        }
        // S、F使入向引用发生自致变化；reader机械核原含义依据而不是重新解释。
        auto now=source.读取采用来源保护(q.来源,q.场景节点);核来源(now);
        基础节点组追加请求 req;
        合并采用保护(req.预期联合节点组,now.完整保护);
        for(const auto& basis:now.特征概念依据组)合并采用保护(req.预期联合节点组,basis.完整保护);
        for(const auto& r:roles) {
            const auto read=f.读取特征完整保护(r.节点);
            if(read.状态==新特征完整读取状态::资源失败)throw 采用状态::资源失败;
            if(read.状态==新特征完整读取状态::前提已变化)throw 采用状态::前提已变化;
            if(read.状态!=新特征完整读取状态::已读取||!read.信息
                ||read.信息->特征类型根!=r.FT||read.信息->特征.当前值!=std::optional<新特征准确值>{r.E})
                throw std::logic_error("adopted role feature complete read differs");
            合并采用保护(req.预期联合节点组,read.信息->完整保护组);
        }
        const auto 新节点=[&](U键 t){const auto n=req.新节点组.size();req.新节点组.push_back({{{节点类型字段,采用键(t)}}});return n;};
        const auto add=[&](std::size_t n,U键 k,基础组内字段内容 v){req.新节点组[n].字段组.push_back({采用键(k),std::move(v)});};
        const auto integer=[&](std::size_t n,U键 k,auto v){add(n,k,基础值{static_cast<std::int64_t>(v)});};
        const auto U=新节点(U键::采用类型),graph=新节点(U键::定义图类型);
        add(U,U键::来源消息,q.来源.接收消息节点);
        add(U,U键::来源范围,基础值{std::vector<std::uint64_t>{q.来源.原文起点,q.来源.原文终点}});
        add(U,U键::来源路径,基础值{q.来源.根出现路径});
        add(U,U键::语言,q.含义.语言节点);add(U,U键::场景,q.场景节点);
        add(U,U键::现实根,q.现实范围根);add(U,U键::原文材料,q.原文材料节点);
        integer(U,U键::根出现编号,q.含义.根出现编号);add(U,U键::定义图,基础组内节点引用{graph});
        追加固定含义图(req.新节点组,graph,q.含义.定义图);
        std::map<std::int64_t,std::size_t> occurrences;
        for(const auto& o:q.含义.出现组)occurrences.emplace(o.出现编号,新节点(U键::出现类型));
        for(const auto& o:q.含义.出现组) {
            const auto n=occurrences.at(o.出现编号);add(U,U键::出现成员,基础组内节点引用{n});
            add(n,U键::出现宿主,基础组内节点引用{U});integer(n,U键::出现编号,o.出现编号);
            add(n,U键::出现知识,o.所用知识节点);integer(n,U键::出现定义编号,o.定义含义编号);
            integer(n,U键::出现作用域,o.有效作用域);add(n,U键::出现路径,基础值{o.结构项路径});
            if(o.原文范围)add(n,U键::出现范围,基础值{std::vector<std::uint64_t>{o.原文范围->first,o.原文范围->second}});
            for(const auto& r:o.角色组) {
                const auto rn=新节点(U键::角色类型);add(n,U键::出现角色,基础组内节点引用{rn});
                add(rn,U键::角色宿主,基础组内节点引用{n});integer(rn,U键::角色编号,r.角色编号);
                if(const auto* v=std::get_if<语言图引用值>(&r.值)) {
                    const bool 受话者=是采用行动请求(q.含义)&&o.出现编号==q.含义.根出现编号
                        && (&r.值==&采用参数(q.含义,o,新语言知识参数角色::受请求者));
                    integer(rn,U键::值标签,受话者 ? 4 : 1);integer(rn,U键::引用类别,v->类别);
                    if(受话者) {
                        add(rn,U键::引用节点,v->节点);
                        continue;
                    }
                    if(v->类别==新语言知识角色类别::存在) {
                        const auto rp=std::find_if(roles.begin(),roles.end(),[&](const auto& x){return x.出现==o.出现编号&&x.角色==r.角色编号;});
                        if(rp==roles.end())throw std::logic_error("adopted role feature binding not prepared");
                        add(rn,U键::实际角色特征,rp->节点);
                    } else add(rn,U键::引用节点,v->节点);
                    if(v->来源概念)add(rn,U键::来源概念,*v->来源概念);
                    if(v->选用名称关系)add(rn,U键::选用名称,*v->选用名称关系);
                } else if(const auto* v=std::get_if<语言图整数值>(&r.值)) {
                    integer(rn,U键::值标签,2);add(rn,U键::整数文本,基础值{v->规范十进制});
                    if(v->单位)add(rn,U键::整数单位,*v->单位);
                } else if(const auto* v=std::get_if<语言图子出现引用>(&r.值)) {
                    integer(rn,U键::值标签,3);add(rn,U键::子出现,基础组内节点引用{occurrences.at(v->出现编号)});
                } else throw std::logic_error("adopted meaning unresolved role reached publication");
            }
        }
        for(const auto& p:q.角色原文定位组) {
            const auto n=新节点(U键::定位类型);add(U,U键::定位成员,基础组内节点引用{n});
            add(n,U键::定位宿主,基础组内节点引用{U});add(n,U键::定位出现,基础组内节点引用{occurrences.at(p.出现编号)});
            integer(n,U键::定位角色,p.角色编号);add(n,U键::定位范围,基础值{std::vector<std::uint64_t>{p.原文起点,p.原文终点}});
        }
        req.追加字段组.push_back({q.含义.语言节点,采用键(U键::语言采用登记),基础组内节点引用{U}});
        基础节点组追加结果 published;
        {
            std::lock_guard 锁(新语言互斥);
            if(!查采用源已加锁(q.来源)) {
                知识读取现场 schema;
                for(const auto id:接收含义句柄)(void)schema.节点(id);
                (void)schema.节点(节点类型字段);
                // 图追加器使用原知识schema，全部键纳入同次预期。
                for(const auto id:知识句柄)(void)schema.节点(id);
                核采用技术状态(schema.状态);
                std::vector<基础联合节点快照> schemaRead;
                核采用技术状态(核结构闭包(schema.结构组,&schemaRead));
                合并采用保护(req.预期联合节点组,schemaRead);
                publicationAttempted=true;published=全局基础数据集.原子追加节点组(req);
            }
        }
        auto result=existing();
        if(result.状态==采用状态::精确重复||result.状态==采用状态::来源冲突) {
            // 若本次F已被该U使用，不清理；并发输家的F经正式owner回收。
            bool adopted=false;
            if(result.信息) {
                std::lock_guard 锁(新语言互斥);
                const auto original=读取采用原始已加锁(result.信息->节点);
                adopted=std::all_of(candidates.begin(),candidates.end(),[&](auto c){return std::any_of(original.角色特征组.begin(),original.角色特征组.end(),[&](const auto& r){return r.特征==c;});});
            }
            if(adopted)candidates.clear();else 清理未采用();
            if(result.状态==采用状态::精确重复&&published.状态==基础节点组追加状态::已完成)result.状态=采用状态::已采用;
            return result;
        }
        if(result.状态!=采用状态::未找到) {
            // 发布后不能在未知读回状态下删除可能已采用的F。
            if(!publicationAttempted)清理未采用();
            return result;
        }
        清理未采用();
        if(published.状态==基础节点组追加状态::资源失败)return {采用状态::资源失败,{},{}};
        if(published.状态==基础节点组追加状态::预期不符)return {采用状态::前提已变化,{},{}};
        throw std::logic_error("meaning publication has no same-source readback");
    } catch(采用状态 s){if(!publicationAttempted)清理未采用();return {s,{},{}};}
    catch(const std::bad_alloc&){if(!publicationAttempted)清理未采用();return {采用状态::资源失败,{},{}};}
    catch(const std::length_error&){if(!publicationAttempted)清理未采用();return {采用状态::资源失败,{},{}};}
    catch(...){if(!publicationAttempted)清理未采用();throw;}
}

新语言解释资源保护结果 新_语言类::读取解释资源保护(
    稳定编码 语言节点, const 新_词类& 词, const 概念_存在类& 概念) const {
    using S = 新语言解释资源保护状态;
    try {
        if (!有效(语言节点)) return {};
        const auto 核状态 = [](知识状态 状态) {
            switch (状态) {
            case 知识状态::已完成: return;
            case 知识状态::尚未初始化: throw S::尚未初始化;
            case 知识状态::读取漂移:
            case 知识状态::引用不可读: throw S::前提已变化;
            // 本口只用无额度的实际有限集算法；此路径该状态只来自基础资源失败。
            case 知识状态::工程未完成: throw S::资源失败;
            default: throw std::logic_error("language resource complete reader rejected valid structure");
            }
        };
        const auto 同一联合 = [](const 基础联合节点快照& 左, const 基础联合节点快照& 右) {
            return 左.结构.节点 == 右.结构.节点 && 左.结构.字段组 == 右.结构.字段组
                && 左.入向字段组 == 右.入向字段组
                && 左.入向外部关系组 == 右.入向外部关系组
                && 左.出向外部关系组 == 右.出向外部关系组;
        };
        新语言解释资源保护结果 结果;
        新语言结构定义集信息 定义集;
        std::vector<新语言结构对应行> 对应行;
        结构快照 原;
        std::vector<稳定编码> 受阻;
        std::string 诊断;
        const auto 采用状态 = 完整读取采用(语言节点, 词, 结果.当前采用, 定义集,
            对应行, 原, 受阻, 诊断);
        if (采用状态 != 知识状态::未找到) 核状态(采用状态);
        if (结果.当前采用) 结果.当前定义集 = 定义集;
        语言知识候选快照 候选;
        const auto 并原始 = [&](const 结构快照& 组) {
            for (const auto& [节点, 内容] : 组) {
                const auto [位置, 新增] = 原.emplace(节点, 内容);
                if (!新增 && !基础结构相等(位置->second, 内容)) throw S::前提已变化;
            }
        };
        {
            std::lock_guard 锁(新语言互斥);
            核状态(读取语言知识候选已加锁(语言节点, 候选, 诊断));
            并原始(候选.结构组);
            知识读取现场 模式现场;
            for (const auto 节点 : 知识句柄) (void)模式现场.节点(节点);
            for (const auto 节点 : {自然语言组织根节点, 语言组织根节点,
                    节点类型字段, 自然语言组织根类型, 语言组织根类型,
                    语言节点类型, 语言登记键字段, 创建来源字段})
                (void)模式现场.节点(节点);
            核状态(模式现场.状态);
            并原始(模式现场.结构组);
        }
        std::map<稳定编码, 基础联合节点快照> 端点保护;
        const auto 并端点 = [&](const std::vector<基础联合节点快照>& 组) {
            if (组.empty()) throw std::logic_error("language resource endpoint protection missing");
            for (const auto& 快照 : 组) {
                const auto [位置, 新增] = 端点保护.emplace(快照.结构.节点.编码, 快照);
                if (!新增 && !同一联合(位置->second, 快照)) throw S::前提已变化;
                const auto [原位置, 原新增] = 原.emplace(快照.结构.节点.编码, 快照.结构);
                if (!原新增 && !基础结构相等(原位置->second, 快照.结构)) throw S::前提已变化;
            }
        };
        const auto 固定词 = 收集知识固定词(候选);
        std::set<稳定编码> 词节点组(固定词.begin(), 固定词.end());
        if (结果.当前定义集) for (const auto& 模式 : 结果.当前定义集->模式组)
            词节点组.insert(模式.固定词条节点组.begin(), 模式.固定词条节点组.end());
        for (const auto 节点 : 词节点组) {
            const auto 读 = 词.读取词条保护(节点);
            if (读.状态 == 新词条保护状态::资源失败) throw S::资源失败;
            if (读.状态 == 新词条保护状态::前提已变化
                || 读.状态 == 新词条保护状态::未找到) throw S::前提已变化;
            if (读.状态 != 新词条保护状态::已读取 || !读.信息
                || 读.信息->语言节点 != 语言节点)
                throw std::logic_error("language knowledge fixed word corrupt");
            并端点(读.完整保护);
        }
        语言知识指代端点快照 保护指代;
        for (const auto 节点 : 收集知识指代定义(候选)) {
            const auto 读 = 概念.读取指代概念保护(节点);
            if (读.状态 == 指代概念查询状态::资源失败) throw S::资源失败;
            if (读.状态 == 指代概念查询状态::读取漂移
                || 读.状态 == 指代概念查询状态::依赖已变化
                || 读.状态 == 指代概念查询状态::未找到) throw S::前提已变化;
            if (读.状态 != 指代概念查询状态::已找到 || !读.信息)
                throw std::logic_error("language knowledge pronoun definition corrupt");
            保护指代.emplace(节点,*读.信息);
            并端点(读.完整保护);
        }
        if(核图指代端点定义(候选,nullptr,保护指代,诊断)!=知识状态::已完成)
            throw std::logic_error("language knowledge graph contextual pronoun definition corrupt");
        std::lock_guard 锁(新语言互斥);
        // 锁外端点读取之后，重新读取本owner完整范围；不是从旧候选中截前缀。
        语言知识候选快照 现在;
        核状态(读取语言知识候选已加锁(语言节点, 现在, 诊断));
        if (现在.信息组.size() != 候选.信息组.size()) throw S::前提已变化;
        for (std::size_t i=0; i<候选.信息组.size(); ++i) {
            const auto& 前 = 候选.信息组[i]; const auto& 后 = 现在.信息组[i];
            if (前.节点 != 后.节点 || 前.目标节点 != 后.目标节点
                || 前.结构节点 != 后.结构节点 || 前.定义 != 后.定义
                || 前.来源组 != 后.来源组) throw S::前提已变化;
        }
        并原始(现在.结构组);
        std::optional<新语言结构采用引用> 再采用;
        新语言结构定义集信息 再定义;
        std::vector<新语言结构对应行> 再行;
        结构快照 再原始;
        const auto 再状态 = 读采用原始已加锁(语言节点, 再采用, 再定义,
            再行, 再原始, 受阻, 诊断);
        if (再状态 != 知识状态::未找到) 核状态(再状态);
        if (再采用 != 结果.当前采用) throw S::前提已变化;
        并原始(再原始);
        核状态(核结构闭包(原, &结果.完整保护));
        for (const auto& 快照 : 结果.完整保护) {
            const auto 旧 = 端点保护.find(快照.结构.节点.编码);
            if (旧 != 端点保护.end() && !同一联合(旧->second, 快照)) throw S::前提已变化;
        }
        结果.候选知识组 = std::move(现在.信息组);
        结果.状态 = S::已读取;
        return 结果;
    } catch (S 状态) { return {状态, {}, {}, {}, {}}; }
    catch (const std::bad_alloc&) { return {S::资源失败, {}, {}, {}, {}}; }
    catch (const std::length_error&) { return {S::资源失败, {}, {}, {}, {}}; }
}



namespace {
// CONV-EXPR-STRUCT：仅语言owner使用的结构字段编解码。引用始终是L1引用；
// 局部编号/顺序是直接值，未把稳定编码藏入文本或整数向量。
enum class 组织键 : std::size_t { 登记根,类型,序号,值,引用,子项,知识登记,出站登记,场景,来源,组织知识,原文,含义网,定义,场景信息登记,自我写入登记,问题登记,答案,答案来源,词典材料登记,词典材料组登记,词典归类解读,出站被询定位,采用篇章,篇章说话者,指代消解结果,指代当前依据,
    词头来源语言,词头规范化,词头批次,词头原包,词头语种,词头记录键,总数 };
std::array<稳定编码,static_cast<std::size_t>(组织键::总数)> 组织句柄{};
稳定编码 组键(组织键 k){return 组织句柄[static_cast<std::size_t>(k)];}
void 组织要求(bool ok){if(!ok)throw std::logic_error("表达组织知识或出站含义记录结构不一致");}
struct 组织读取现场 {
    std::map<稳定编码,基础联合节点快照> 快照;
    const 基础联合节点快照& 读(稳定编码 id){
        if(auto i=快照.find(id);i!=快照.end())return i->second;
        const auto r=全局基础数据集.查询节点组完整结构({id});
        if(r.状态==基础结构操作状态::资源失败)throw std::bad_alloc{};
        组织要求(r.状态==基础结构操作状态::已完成&&r.节点组.size()==1&&r.节点组.front().快照.has_value());
        return 快照.emplace(id,*r.节点组.front().快照).first->second;
    }
    std::vector<基础字段内容> 字段(稳定编码 id,组织键 k){
        std::vector<基础字段内容> r;for(const auto& f:读(id).结构.字段组)if(f.字段节点==组键(k))r.push_back(f.内容);return r;
    }
    基础字段内容 唯一(稳定编码 id,组织键 k){const auto r=字段(id,k);组织要求(r.size()==1);return r.front();}
    std::int64_t 整数(稳定编码 id,组织键 k){return std::get<std::int64_t>(std::get<基础值>(唯一(id,k)).材料);}
    稳定编码 引用(稳定编码 id,组织键 k){return std::get<稳定编码>(唯一(id,k));}
    std::vector<基础联合节点快照> 保护() const {std::vector<基础联合节点快照> r;for(const auto& [id,v]:快照){(void)id;r.push_back(v);}return r;}
};
auto 组织记录字段(新绑定指称& v){(void)v;return std::tie(v.类别,v.节点,v.范围概念);}
auto 组织记录字段(新指代实例引用& v){(void)v;return std::tie(v.节点,v.网内编号);}
auto 组织记录字段(新疑问代词实例& v){(void)v;return std::tie(v.网内编号,v.待求类型概念,v.询问内容,v.类型取材步骤);}
auto 组织记录字段(新名称描述指称& v){(void)v;return std::tie(v.名称,v.描述内容);}
auto 组织记录字段(新类型化表达值& v){(void)v;return std::tie(v.类型,v.单位,v.准确值);}
auto 组织记录字段(新匿名表达对象& v){(void)v;return std::tie(v.网内编号,v.范围概念);}
auto 组织记录字段(新明确未知& v){(void)v;return std::tie();}
auto 组织记录字段(新规则角色变量& v){(void)v;return std::tie(v.角色);}
auto 组织记录字段(新表达值域& v){return std::tie(v.值域概念);}
auto 组织记录字段(新表达概念条件& v){(void)v;return std::tie(v.概念);}
auto 组织记录字段(新特征值谓项& v){(void)v;return std::tie(v.特征类型,v.值);}
auto 组织记录字段(新概念归属谓项& v){(void)v;return std::tie(v.概念);}
auto 组织记录字段(新同一指认谓项& v){(void)v;return std::tie(v.另一指称);}
auto 组织记录字段(新静态含义单元& v){(void)v;return std::tie(v.主体,v.谓项,v.时间,v.范围);}
auto 组织记录字段(新含义具名角色& v){(void)v;return std::tie(v.名称,v.指称);}
auto 组织记录字段(新动作发生含义& v){(void)v;return std::tie(v.主体,v.动作概念,v.对象角色组,v.方式);}
auto 组织记录字段(新状态变化含义& v){(void)v;return std::tie(v.对象,v.特征类型,v.前值,v.后值或方向,v.时间);}
auto 组织记录字段(新关联主张含义& v){(void)v;return std::tie(v.动作发生引用,v.状态变化引用);}
auto 组织记录字段(新事件含义单元& v){(void)v;return std::tie(v.动作发生,v.状态变化,v.关联主张);}
auto 组织记录字段(新共享角色映射& v){(void)v;return std::tie(v.源角色,v.目标角色);}
auto 组织记录字段(新内容算子& v){(void)v;return std::tie(v.种类,v.角色组,v.内容引用组,v.共享角色映射,v.态度种类,v.确认范围,v.量化方式,v.数量值域);}
auto 组织记录字段(新篇章组合& v){(void)v;return std::tie(v.种类,v.有序子项组);}
auto 组织记录字段(新跨场景含义引用& v){(void)v;return std::tie(v.种类,v.目标语言场景,v.目标项或指代实例);}
auto 组织记录字段(新招呼含义单元& v){(void)v;return std::tie(v.种类);}
auto 组织记录字段(新含义原文范围& v){(void)v;return std::tie(v.起始,v.长度);}
auto 组织记录字段(新含义内容项& v){(void)v;return std::tie(v.编号,v.内容,v.原文范围组,v.所用知识组,v.角色映射);}
auto 组织记录字段(新含义顶层& v){(void)v;return std::tie(v.语言出现编号,v.表达用途,v.内容引用);}
auto 组织记录字段(新语言含义网& v){(void)v;return std::tie(v.身份,v.所属语言场景,v.顶层项组,v.内容项组);}
auto 组织记录字段(新组织取材步骤& v){(void)v;return std::tie(v.编号,v.算子,v.输出角色);}
auto 组织记录字段(新表达需求类型& v){(void)v;return std::tie(v.用途,v.对象种类);}
auto 组织记录字段(新表达组织知识定义& v){(void)v;return std::tie(v.适用类型,v.适用条件组,v.取材步骤组,v.含义网模板,v.缺材处理);}
auto 组织记录字段(新前置语言缺口& v){
    return std::tie(v.种类,v.起始标量,v.结束标量,v.已取得相关节点);
}
auto 组织记录字段(新语言词条类型依据& v){
    return std::tie(v.起始标量,v.结束标量,v.词条,v.所需类别,v.构式依据组);
}
auto 组织记录字段(接收语言主体来源& v){
    return std::tie(v.词条,v.名称关系,v.已绑定存在,v.已证冲突候选);
}
auto 组织记录字段(接收语言主体关系& v){
    return std::tie(v.起始标量,v.结束标量,v.对象起始标量,v.对象结束标量,v.所用构式,v.指代概念,v.已确定,v.来源);
}
auto 组织记录字段(语言图引用值& v){
    return std::tie(v.类别,v.节点,v.来源概念,v.选用名称关系);
}
auto 组织记录字段(语言图整数值& v){
    return std::tie(v.规范十进制,v.单位);
}
auto 组织记录字段(接收语言已解释角色& v){
    return std::tie(v.角色编号,v.参数,v.起始标量,v.结束标量,v.值);
}
auto 组织记录字段(接收语言静态结构& v){
    return std::tie(v.所用构式,v.操作,v.角色组);
}
auto 组织记录字段(接收语言关系端点& v){
    return std::tie(v.角色键,v.参数,v.起始标量,v.结束标量,v.指称);
}
auto 组织记录字段(接收语言关系候选& v){
    return std::tie(v.关系概念,v.所用构式,v.种子键,v.源角色键,v.目标角色键,v.端点组);
}
auto 组织记录字段(接收语言关系观察& v){
    return std::tie(v.读取未完成,v.候选组);
}
auto 组织记录字段(接收语言句段解释& v){
    return std::tie(v.起始标量,v.结束标量,v.状态,v.图状态,v.缺口组,v.构式,v.主体关系,v.词条类型依据组,
        v.静态结构,v.关系观察);
}
auto 组织记录字段(新接收语言解释内容& v){
    return std::tie(v.来源场景,v.前置来源,v.语言节点,v.原文起点,v.原文终点,v.句段组,v.含义候选);
}
auto 组织记录字段(新明确回复传输定位& v){return std::tie(v.关联问题,v.输出消息,v.实际发送);}
auto 组织记录字段(新接收场景信息& v){return std::tie(v.种类,v.来源消息,v.来源场景,v.交互场景,v.说话者,v.主体存在,v.主体概念,v.特征类型,
    v.值种类,v.准确整数,v.区间下界,v.区间上界,v.单位,v.特征概念,v.方向,v.招呼操作,v.目标存在,
    v.原文起点,v.原文终点,v.时刻,v.回复传输);}
auto 组织记录字段(新自我写入语言信息& v){return std::tie(v.写入来源,v.来源场景,v.自我,v.主体存在,v.特征类型,
    v.值种类,v.准确整数,v.区间下界,v.区间上界,v.单位,v.特征概念,v.方向,v.时刻);}
auto 组织记录字段(新出站正式被询定位& v){return std::tie(v.信息节点,v.特征类型);}
auto 组织记录字段(新语言原文分界被询目标& v){return std::tie(v.范围,v.特征类型,v.外设场景,v.外设窗口,v.公开会话,v.对话框);}
auto 组织记录字段(新问题整数短答规格& v){return std::tie(v.被问字段,v.单位);}
auto 组织记录字段(新语言问题定义& v){return std::tie(v.出站来源场景,v.出站来源,v.交互场景,v.交互者,
    v.被认知需求,v.主体,v.被问栏,v.提问时刻,v.被问种类,v.分界目标,v.整数短答规格);}
auto 组织记录字段(新词典语言词段& v) {
    return std::tie(v.起始标量, v.结束标量, v.规范化词面, v.依据);
}
auto 组织记录字段(新词典归类材料定义& v) {
    return std::tie(v.材料Q, v.义项I, v.来源场景, v.说话者, v.释义语言, v.词条X,
        v.原行号, v.原文起始标量, v.原文结束标量, v.释义原文, v.规范化释义,
        v.状态, v.结构编号, v.词段组, v.中心词, v.修饰起始标量, v.修饰结束标量, v.词条N, v.诊断);
}
auto 组织记录字段(新词典归类概念解读& v) {
    return std::tie(v.状态, v.依据词条N, v.候选概念组, v.采用概念);
}
auto 组织记录字段(新词典语言材料组定义& v) {
    return std::tie(v.来源场景, v.已处理行组);
}

auto 组织记录字段(新语言分界词段& v) { return std::tie(v.起始标量, v.结束标量, v.词条, v.用途); }
struct 分界回答定义 final { 稳定编码 问题, 回复, 说话者, 输出, 实际发送; };
struct 分界对齐定义 final { 稳定编码 范围; std::vector<新语言分界词段> 词段组; };
constexpr std::int64_t 分界回答组织类型 = 108, 分界对齐组织类型 = 109;
auto 组织记录字段(分界回答定义& v) { return std::tie(v.问题,v.回复,v.说话者,v.输出,v.实际发送); }
auto 组织记录字段(分界对齐定义& v) { return std::tie(v.范围,v.词段组); }
auto 组织记录字段(新语言分界候选路径& v) { return std::tie(v.词段组, v.规则应用记录组); }
auto 组织记录字段(新语言分界候选定义& v) { return std::tie(v.路径组); }
auto 组织记录字段(新语言规则分界来源& v) { return std::tie(v.正式匹配记录); }
auto 组织记录字段(新语言回答分界来源& v) { return std::tie(v.正式问题, v.正式回答, v.分段对齐材料); }
auto 组织记录字段(新语言正式分界取材& v) { return std::tie(v.已证明词段, v.来源); }
auto 组织记录字段(新语言分界依据定义& v) { return std::tie(v.候选材料, v.取材); }
auto 组织记录字段(新语言有效分界定义& v) { return std::tie(v.正式依据); }
auto 组织记录字段(新语言词条准入定义& v) { return std::tie(v.有效结果, v.词段组); }

template<class T> struct 组织可选 : std::false_type {};
template<class T> struct 组织可选<std::optional<T>> : std::true_type {};
template<class T> struct 组织数组 : std::false_type {};
template<class T> struct 组织数组<std::vector<T>> : std::true_type {};
template<class T> struct 组织变体 : std::false_type {};
template<class... T> struct 组织变体<std::variant<T...>> : std::true_type {};
struct 组织写入现场 {
    基础节点组追加请求 请求;
    std::set<稳定编码> 引用组;
    std::size_t 节点(std::int64_t tag,std::int64_t order){
        const auto n=请求.新节点组.size();
        请求.新节点组.push_back({{{组键(组织键::类型),基础值{tag}},{组键(组织键::序号),基础值{order}}}});return n;
    }
    void 值(std::size_t n,基础原始值 v){请求.新节点组[n].字段组.push_back({组键(组织键::值),基础值{std::move(v)}});}
    void 引用(std::size_t n,组织键 k,稳定编码 id){组织要求(有效(id));引用组.insert(id);请求.新节点组[n].字段组.push_back({组键(k),id});}
    void 子(std::size_t n,组织键 k,std::size_t child){请求.新节点组[n].字段组.push_back({组键(k),基础组内节点引用{child}});}
    template<class T> std::size_t 写(const T& v,std::int64_t order=0){
        std::size_t n=0;
        if constexpr(std::is_enum_v<T> || std::is_same_v<T,bool>){n=节点(2,order);值(n,static_cast<std::int64_t>(v));}
        else if constexpr(std::is_same_v<T,std::int64_t>){n=节点(1,order);值(n,v);}
        else if constexpr(std::is_same_v<T,std::uint64_t>){
            n=节点(9,order);
            值(n,std::vector<std::uint64_t>{v});
        }
        else if constexpr(std::is_same_v<T,std::string>){n=节点(3,order);值(n,v);}
        else if constexpr(std::is_same_v<T,稳定编码>){n=节点(4,order);if(有效(v))引用(n,组织键::引用,v);}
        else if constexpr(组织数组<T>::value){n=节点(6,order);std::int64_t i=0;for(const auto& c:v){const auto child=写(c,i++);子(n,组织键::子项,child);}}
        else if constexpr(组织可选<T>::value){n=节点(7,order);if(v){const auto child=写(*v);子(n,组织键::子项,child);}}
        else if constexpr(组织变体<T>::value){n=节点(8,order);值(n,static_cast<std::int64_t>(v.index()));std::visit([&](const auto& x){const auto child=写(x);子(n,组织键::子项,child);},v);}
        else {
            n=节点(5,order);auto copy=v;std::int64_t i=0;
            const auto 写项=[&](const auto& 项){
                if constexpr(std::is_same_v<T,新语言问题定义>){
                    using 字段=std::decay_t<decltype(项)>;
                    if constexpr(std::is_same_v<字段,std::optional<新语言原文分界被询目标>>)
                        if(!项 && !v.整数短答规格)return;
                    if constexpr(std::is_same_v<字段,std::optional<新问题整数短答规格>>)
                        if(!项)return;
                }
                子(n,组织键::子项,写(项,i++));
            };
            std::apply([&](const auto&... 项){(写项(项),...);},组织记录字段(copy));
        }
        return n;
    }
    bool 保护(组织读取现场& 现场){
        for(auto id:组织句柄)引用组.insert(id);
        for(auto id:引用组)现场.读(id);
        请求.预期联合节点组=现场.保护();return true;
    }
};
template<class T> T 组织读值(组织读取现场&,稳定编码,std::set<稳定编码>&);
template<std::size_t I=0,class T> void 组织读变体(T& v,std::size_t index,组织读取现场& r,稳定编码 child,std::set<稳定编码>& path){
    if constexpr(I<std::variant_size_v<T>){if(I==index)v=组织读值<std::variant_alternative_t<I,T>>(r,child,path);else 组织读变体<I+1>(v,index,r,child,path);}
    else 组织要求(false);
}
template<class T> T 组织读值(组织读取现场& r,稳定编码 id,std::set<稳定编码>& path){
    组织要求(path.insert(id).second);T v{};std::vector<稳定编码> children;
    for(const auto& f:r.字段(id,组织键::子项))children.push_back(std::get<稳定编码>(f));
    std::sort(children.begin(),children.end(),[&](auto a,auto b){return r.整数(a,组织键::序号)<r.整数(b,组织键::序号);});
    for(std::size_t i=0;i<children.size();++i)组织要求(r.整数(children[i],组织键::序号)==static_cast<std::int64_t>(i));
    const auto tag=r.整数(id,组织键::类型);
    if constexpr(std::is_enum_v<T>){
        组织要求(tag==2&&children.empty());
        const auto 原值=r.整数(id,组织键::值);
        if constexpr(std::is_same_v<T,新指称类别>)
            组织要求(原值>=1&&原值<=static_cast<std::int64_t>(新指称类别::正式信息));
        else if constexpr(std::is_same_v<T,新组织绑定角色>)
            组织要求(原值>=1&&原值<=static_cast<std::int64_t>(新组织绑定角色::正式信息));
        else if constexpr(std::is_same_v<T,新组织取材算子>)
            组织要求(原值>=1&&原值<=static_cast<std::int64_t>(新组织取材算子::正式字段值域概念));
        else if constexpr(std::is_same_v<T,新问题被问种类>)
            组织要求(原值>=1&&原值<=static_cast<std::int64_t>(新问题被问种类::原文分界));
        else if constexpr(std::is_same_v<T,新语言分界段用途>)
            组织要求(原值>=1&&原值<=static_cast<std::int64_t>(新语言分界段用途::分隔));
        v=static_cast<T>(原值);
    }
    else if constexpr(std::is_same_v<T,bool>){
        组织要求(tag==2&&children.empty());
        const auto 值=r.整数(id,组织键::值);
        组织要求(值==0||值==1);
        v=值!=0;
    }
    else if constexpr(std::is_same_v<T,std::int64_t>){组织要求(tag==1&&children.empty());v=r.整数(id,组织键::值);}
    else if constexpr(std::is_same_v<T,std::uint64_t>){
        组织要求(tag==9&&children.empty());
        const auto 字段=r.唯一(id,组织键::值);
        const auto& 数组=std::get<std::vector<std::uint64_t>>(std::get<基础值>(字段).材料);
        组织要求(数组.size()==1);
        v=数组.front();
    }
    else if constexpr(std::is_same_v<T,std::string>){组织要求(tag==3&&children.empty());v=std::get<std::string>(std::get<基础值>(r.唯一(id,组织键::值)).材料);}
    else if constexpr(std::is_same_v<T,稳定编码>){组织要求(tag==4&&children.empty());const auto a=r.字段(id,组织键::引用);组织要求(a.size()<=1);if(!a.empty()){v=std::get<稳定编码>(a.front());r.读(v);}}
    else if constexpr(组织数组<T>::value){组织要求(tag==6);for(auto c:children)v.push_back(组织读值<typename T::value_type>(r,c,path));}
    else if constexpr(组织可选<T>::value){组织要求(tag==7&&children.size()<=1);if(!children.empty())v=组织读值<typename T::value_type>(r,children.front(),path);}
    else if constexpr(组织变体<T>::value){组织要求(tag==8&&children.size()==1);const auto index=r.整数(id,组织键::值);组织要求(index>=0);组织读变体(v,static_cast<std::size_t>(index),r,children.front(),path);}
    else {
        组织要求(tag==5);auto fs=组织记录字段(v);
        constexpr auto 字段数=std::tuple_size_v<decltype(fs)>;
        if constexpr(std::is_same_v<T,接收语言句段解释>||std::is_same_v<T,新语言问题定义>)
            组织要求(children.size()>=字段数-2&&children.size()<=字段数);
        else if constexpr(std::is_same_v<T,新语言分界词段>
            ||std::is_same_v<T,接收语言主体来源>||std::is_same_v<T,接收语言主体关系>
            ||std::is_same_v<T,新接收语言解释内容>||std::is_same_v<T,新接收场景信息>)
            组织要求(children.size()==字段数||children.size()==字段数-1);
        else 组织要求(children.size()==字段数);
        std::size_t i=0;
        const auto 读项=[&](auto& 项){
            if(i<children.size())项=组织读值<std::decay_t<decltype(项)>>(r,children[i++],path);
        };
        std::apply([&](auto&... 项){(读项(项),...);},fs);
    }
    const auto scalars=r.字段(id,组织键::值).size(),refs=r.字段(id,组织键::引用).size();
    组织要求(r.读(id).结构.字段组.size()==2+children.size()+scalars+refs);
    组织要求(scalars==static_cast<std::size_t>(tag==1||tag==2||tag==3||tag==8||tag==9));
    组织要求(tag==4||refs==0);
    path.erase(id);return v;
}
template<class T> void 核验组织模板绑定(const T& v,const std::map<std::int64_t,新组织绑定角色>& steps,
    const std::set<新组织绑定角色>& roles){
    if constexpr(std::is_same_v<T,新规则角色变量>){
        组织要求(v.角色==新组织绑定角色::说话者||v.角色==新组织绑定角色::受话者||roles.contains(v.角色));
    } else if constexpr(std::is_same_v<T,新疑问代词实例>){
        组织要求(v.类型取材步骤&&steps.contains(*v.类型取材步骤)
            &&(steps.at(*v.类型取材步骤)==新组织绑定角色::字段值域概念||steps.at(*v.类型取材步骤)==新组织绑定角色::目标存在范围)
            &&!有效(v.待求类型概念));
    } else if constexpr(组织数组<T>::value){for(const auto& x:v)核验组织模板绑定(x,steps,roles);}
    else if constexpr(组织可选<T>::value){if(v)核验组织模板绑定(*v,steps,roles);}
    else if constexpr(组织变体<T>::value){std::visit([&](const auto& x){核验组织模板绑定(x,steps,roles);},v);}
    else if constexpr(!std::is_enum_v<T>&&!std::is_same_v<T,std::int64_t>&&!std::is_same_v<T,std::uint64_t>&&!std::is_same_v<T,std::string>&&!std::is_same_v<T,稳定编码>){
        auto copy=v;std::apply([&](const auto&... x){(核验组织模板绑定(x,steps,roles),...);},组织记录字段(copy));
    }
}
void 核验组织定义(const 新表达组织知识定义& d){
    // 对象种类：0 需求字段（陈述/疑问/祈使）；1 招呼（用途招呼，无条件、无取材，CONV-EXPR-TEXT §3.5）；
    // 2 需求目标栏、3 正式信息字段均用于疑问。
    组织要求((d.适用类型.对象种类==0&&d.适用类型.用途>=新表达用途::陈述&&d.适用类型.用途<=新表达用途::祈使)
        ||(d.适用类型.对象种类==2&&d.适用类型.用途==新表达用途::疑问)
        ||(d.适用类型.对象种类==3&&d.适用类型.用途==新表达用途::疑问)
        ||(d.适用类型.对象种类==1&&d.适用类型.用途==新表达用途::招呼&&d.适用条件组.empty()&&d.取材步骤组.empty()));
    组织要求(d.缺材处理==新组织缺材处理::必需不足);
    const bool 通用=d.适用类型.对象种类==3;
    if(通用)组织要求(d.适用条件组==std::vector{新组织取材算子::正式字段内容状态});
    for(auto c:d.适用条件组)组织要求(c==(通用
        ? 新组织取材算子::正式字段内容状态 : 新组织取材算子::需求字段内容状态));
    std::set<std::int64_t> ids;std::set<新组织绑定角色> roles;std::map<std::int64_t,新组织绑定角色> steps;
    for(const auto& t:d.取材步骤组){
        组织要求(t.编号>0&&ids.insert(t.编号).second&&roles.insert(t.输出角色).second);steps.emplace(t.编号,t.输出角色);
        组织要求((t.算子==新组织取材算子::绑定角色
                &&(t.输出角色==新组织绑定角色::字段特征类型||t.输出角色==新组织绑定角色::说话者||t.输出角色==新组织绑定角色::受话者))
            ||(t.算子==新组织取材算子::绑定角色&&t.输出角色==新组织绑定角色::需求D&&!通用)
            ||(t.算子==新组织取材算子::绑定角色&&t.输出角色==新组织绑定角色::正式信息&&通用)
            ||(t.算子==新组织取材算子::绑定角色&&t.输出角色==新组织绑定角色::问题主体&&d.适用类型.对象种类==2)
            ||(t.算子==新组织取材算子::需求字段值域概念&&t.输出角色==新组织绑定角色::字段值域概念&&!通用)
            ||(t.算子==新组织取材算子::正式字段值域概念&&t.输出角色==新组织绑定角色::字段值域概念&&通用)
            ||(t.算子==新组织取材算子::存在根概念&&t.输出角色==新组织绑定角色::目标存在范围&&!通用));
    }
    核验含义网结构(d.含义网模板,true);
    核验组织模板绑定(d.含义网模板,steps,roles);
    组织要求(d.含义网模板.顶层项组.front().表达用途==d.适用类型.用途);
}
新表达组织知识信息 读组织知识(组织读取现场& r,稳定编码 id){
    组织要求(r.整数(id,组织键::类型)==100);std::set<稳定编码> path;
    auto d=组织读值<新表达组织知识定义>(r,r.引用(id,组织键::定义),path);核验组织定义(d);
    return {id,std::move(d),r.保护()};
}
std::vector<新表达组织知识信息> 读全部组织知识(组织读取现场& r){
    组织要求(有效(组键(组织键::登记根)));
    for(auto k:组织句柄)r.读(k);
    std::vector<新表达组织知识信息> out;std::set<稳定编码> unique;
    for(const auto& f:r.字段(组键(组织键::登记根),组织键::知识登记)){
        const auto id=std::get<稳定编码>(f);组织要求(unique.insert(id).second);out.push_back(读组织知识(r,id));
    }
    return out;
}
std::optional<新出站具体表达信息> 读出站(组织读取现场& r,稳定编码 key){
    std::optional<新出站具体表达信息> out;
    for(const auto& f:r.字段(组键(组织键::登记根),组织键::出站登记)){
        const auto id=std::get<稳定编码>(f);组织要求(r.整数(id,组织键::类型)==101);
        const auto scene=r.引用(id,组织键::场景),source=r.引用(id,组织键::来源);
        if(scene!=key&&source!=key)continue;
        组织要求(!out);std::set<稳定编码> path;
        新出站具体表达信息 v{id,scene,r.引用(id,组织键::组织知识),source,
            组织读值<新语言含义网>(r,r.引用(id,组织键::含义网),path),
            std::get<std::string>(std::get<基础值>(r.唯一(id,组织键::原文)).材料)};
        const auto 被询=r.字段(id,组织键::出站被询定位);
        组织要求(被询.size()<=1);
        if(!被询.empty()){
            v.正式被询定位=组织读值<新出站正式被询定位>(r,std::get<稳定编码>(被询.front()),path);
            组织要求(v.正式被询定位->信息节点==source&&有效(v.正式被询定位->特征类型));
        }
        核验含义网结构(v.含义网);组织要求(!v.候选话语原文.empty());out=std::move(v);
    }
    return out;
}
} // namespace
void 新_语言类::初始化表达组织知识结构(){
    std::lock_guard lock(新语言互斥);组织要求(结构仍存在());
    if(有效(组织句柄.front())){
        for(auto k:组织句柄)组织要求(节点仍存在(k));return;
    }
    const auto r=全局基础数据集.原子建立节点组(std::vector<基础组内新节点>(组织句柄.size()));
    组织要求(r.状态==基础结构操作状态::已完成&&r.节点组.size()==组织句柄.size());
    std::copy(r.节点组.begin(),r.节点组.end(),组织句柄.begin());
}
新表达组织知识信息 新_语言类::建立或取得表达组织知识(const 新表达组织知识定义& 输入){
    auto d=输入;std::sort(d.含义网模板.内容项组.begin(),d.含义网模板.内容项组.end(),[](const auto& a,const auto& b){return a.编号<b.编号;});
    核验组织定义(d);std::lock_guard lock(新语言互斥);组织读取现场 r;
    const auto all=读全部组织知识(r);for(const auto& i:all)if(i.定义==d)return i;
    组织写入现场 w;const auto root=w.节点(100,0),def=w.写(d);w.子(root,组织键::定义,def);
    w.请求.追加字段组.push_back({组键(组织键::登记根),组键(组织键::知识登记),基础组内节点引用{root}});
    w.保护(r);const auto p=全局基础数据集.原子追加节点组(w.请求);
    组织要求(p.状态==基础节点组追加状态::已完成&&p.新节点组.size()==w.请求.新节点组.size());
    组织读取现场 back;auto out=读组织知识(back,p.新节点组[root]);组织要求(out.定义==d);return out;
}
新表达组织知识候选 新_语言类::查询适用表达组织知识(const 新表达需求类型& t) const {
    std::lock_guard lock(新语言互斥);组织读取现场 r;新表达组织知识候选 out;
    for(auto& i:读全部组织知识(r))if(i.定义.适用类型==t)out.候选组.push_back(std::move(i));out.完整保护=r.保护();return out;
}
新表达组织知识信息 新_语言类::读取表达组织知识(稳定编码 id) const {
    std::lock_guard lock(新语言互斥);组织读取现场 r;const auto all=读全部组织知识(r);
    for(const auto& i:all)if(i.节点==id)return i;throw std::logic_error("组织知识不在语言owner完整登记内");
}
std::optional<新出站具体表达信息> 新_语言类::读取出站具体表达(稳定编码 key) const {
    std::lock_guard lock(新语言互斥);组织读取现场 r;return 读出站(r,key);
}
namespace {
// 值陈述与自我写入信息共用的值形状核验（值种类与载荷一一对应）。
void 核值形状(新值陈述值种类 k,const std::optional<std::string>& 准确整数,const std::optional<std::string>& 下界,
    const std::optional<std::string>& 上界,const std::optional<稳定编码>& 单位,const std::optional<稳定编码>& 特征概念,
    const std::optional<新语言知识特征方向>& 方向){
    using K=新值陈述值种类;
    const bool 整数=准确整数.has_value(),区间=下界||上界;
    switch(k){
    case K::准确整数:组织要求(整数&&!区间&&!方向&&!特征概念);break;
    case K::整数闭区间:组织要求(!整数&&下界&&上界&&!方向&&!特征概念);break;
    case K::变化方向:组织要求(!整数&&!区间&&方向&&!特征概念&&!单位);break;
    case K::特征概念条件:组织要求(!整数&&!区间&&!方向&&特征概念&&!单位);break;
    case K::待求:组织要求(!整数&&!区间&&!方向&&!特征概念&&!单位);break;
    default:组织要求(false);
    }
}
void 核接收场景信息(const 新接收场景信息& v){
    组织要求(有效(v.来源消息)&&有效(v.来源场景)&&有效(v.交互场景)&&有效(v.说话者)
        &&v.原文起点>=0&&v.原文起点<v.原文终点&&(!v.主体存在||!v.主体概念));
    if(v.种类==新接收场景信息种类::明确整数短答){
        组织要求(v.回复传输 && 有效(v.回复传输->关联问题) && 有效(v.回复传输->输出消息)
            && 有效(v.回复传输->实际发送) && v.时刻>0 && v.值种类==新值陈述值种类::准确整数
            && v.单位 && 有效(*v.单位) && !v.主体存在 && !v.主体概念 && !v.特征类型
            && !v.招呼操作 && !v.目标存在);
        核值形状(*v.值种类,v.准确整数,v.区间下界,v.区间上界,v.单位,v.特征概念,v.方向);
        return;
    }
    组织要求(!v.回复传输);
    if(v.种类==新接收场景信息种类::招呼){
        组织要求(v.招呼操作&&新语言含义操作是招呼(*v.招呼操作)&&!v.值种类&&!v.主体存在&&!v.主体概念&&!v.特征类型
            &&!v.准确整数&&!v.区间下界&&!v.区间上界&&!v.单位&&!v.特征概念&&!v.方向&&!v.目标存在);
        return;
    }
    if(v.种类==新接收场景信息种类::需求信息){
        // R121：需求主体、目标存在、目标特征类型能确定的记、不能确定的记未知；记了的须为有效身份。
        组织要求(!v.招呼操作&&!v.主体概念&&(!v.主体存在||有效(*v.主体存在))&&(!v.目标存在||有效(*v.目标存在))
            &&(!v.特征类型||有效(*v.特征类型))&&v.时刻>0);
        if(v.值种类){组织要求(*v.值种类!=新值陈述值种类::待求);核值形状(*v.值种类,v.准确整数,v.区间下界,v.区间上界,v.单位,v.特征概念,v.方向);}
        else 组织要求(!v.准确整数&&!v.区间下界&&!v.区间上界&&!v.单位&&!v.特征概念&&!v.方向);
        return;
    }
    组织要求(v.种类==新接收场景信息种类::值陈述&&!v.招呼操作&&v.值种类&&v.特征类型&&!v.目标存在);
    核值形状(*v.值种类,v.准确整数,v.区间下界,v.区间上界,v.单位,v.特征概念,v.方向);
}
void 核自我写入信息(const 新自我写入语言信息& v){
    组织要求(有效(v.写入来源)&&有效(v.来源场景)&&有效(v.自我)&&有效(v.主体存在)&&有效(v.特征类型)
        &&v.时刻>0&&v.值种类!=新值陈述值种类::待求);
    核值形状(v.值种类,v.准确整数,v.区间下界,v.区间上界,v.单位,v.特征概念,v.方向);
}
void 核问题定义(const 新语言问题定义& v){
    组织要求(有效(v.出站来源场景)&&有效(v.出站来源)&&有效(v.交互场景)&&有效(v.交互者)&&v.提问时刻>0);
    if(v.被问种类==新问题被问种类::原文分界){
        组织要求(v.分界目标&&!v.整数短答规格&&!有效(v.被认知需求)&&!有效(v.主体)&&!有效(v.被问栏));
        const auto& 目标=*v.分界目标;
        组织要求(目标.范围==v.出站来源&&有效(目标.特征类型)&&有效(目标.外设场景)
            &&有效(目标.外设窗口)&&!目标.公开会话.empty()&&!目标.对话框.empty());
        return;
    }
    组织要求(!v.分界目标&&有效(v.被认知需求)&&有效(v.主体)&&有效(v.被问栏)
        &&(v.被问种类==新问题被问种类::载荷栏||v.被问种类==新问题被问种类::目标栏));
    if(v.整数短答规格)
        组织要求(v.被问种类==新问题被问种类::载荷栏&&有效(v.整数短答规格->被问字段)
            &&有效(v.整数短答规格->单位));
}
std::vector<新值陈述信息> 读全部场景信息(组织读取现场& r){
    组织要求(有效(组键(组织键::登记根)));
    for(auto k:组织句柄)r.读(k);
    std::vector<新值陈述信息> out;std::set<稳定编码> unique;
    for(const auto& f:r.字段(组键(组织键::登记根),组织键::场景信息登记)){
        const auto id=std::get<稳定编码>(f);组织要求(unique.insert(id).second&&r.整数(id,组织键::类型)==102);
        std::set<稳定编码> path;
        auto v=组织读值<新接收场景信息>(r,r.引用(id,组织键::定义),path);核接收场景信息(v);
        组织要求(r.引用(id,组织键::来源)==v.来源消息&&r.引用(id,组织键::场景)==v.来源场景);
        组织要求(std::none_of(out.begin(),out.end(),[&](const auto& x){return x.内容.来源消息==v.来源消息;}));
        out.push_back({id,std::move(v)});
    }
    return out;
}
struct 自我写入记录 final { 稳定编码 节点{}; 新自我写入语言信息 内容; };
std::vector<自我写入记录> 读全部自我写入(组织读取现场& r){
    组织要求(有效(组键(组织键::登记根)));
    for(auto k:组织句柄)r.读(k);
    std::vector<自我写入记录> out;std::set<稳定编码> unique;
    for(const auto& f:r.字段(组键(组织键::登记根),组织键::自我写入登记)){
        const auto id=std::get<稳定编码>(f);组织要求(unique.insert(id).second&&r.整数(id,组织键::类型)==105);
        std::set<稳定编码> path;
        auto v=组织读值<新自我写入语言信息>(r,r.引用(id,组织键::定义),path);核自我写入信息(v);
        组织要求(r.引用(id,组织键::来源)==v.写入来源&&r.引用(id,组织键::场景)==v.来源场景);
        组织要求(std::none_of(out.begin(),out.end(),[&](const auto& x){return x.内容.写入来源==v.写入来源;}));
        out.push_back({id,std::move(v)});
    }
    return out;
}
bool 明确整数短答与问题一致(const 新语言问题信息& q,const 新接收场景信息& v){
    const auto& 定义=q.定义;
    if(v.种类!=新接收场景信息种类::明确整数短答 || !v.回复传输 || !v.准确整数
        || v.回复传输->关联问题!=q.问题节点 || 定义.被问种类!=新问题被问种类::载荷栏
        || !定义.整数短答规格 || v.单位!=定义.整数短答规格->单位 || v.交互场景!=定义.交互场景
        || v.说话者!=定义.交互者 || v.时刻<=定义.提问时刻)
        return false;
    const auto& 文=*v.准确整数;
    std::int64_t 数值=0;
    const auto 解析=std::from_chars(文.data(),文.data()+文.size(),数值);
    return 解析.ec==std::errc{} && 解析.ptr==文.data()+文.size();
}
// 问题根节点：类型103；场景＝出站来源场景，来源＝出站来源，定义＝被问结构；答案与答案来源成对追加，至多一次。
std::vector<新语言问题信息> 读全部问题(组织读取现场& r){
    组织要求(有效(组键(组织键::登记根)));
    for(auto k:组织句柄)r.读(k);
    std::set<稳定编码> 场景信息,自我写入;
    const auto 已存信息=读全部场景信息(r);
    std::map<稳定编码,const 新接收场景信息*> 短答;
    for(const auto& x:已存信息){
        if(x.内容.种类==新接收场景信息种类::明确整数短答)短答.emplace(x.陈述节点,&x.内容);
        else if(x.内容.种类==新接收场景信息种类::值陈述||x.内容.种类==新接收场景信息种类::需求信息)
            场景信息.insert(x.陈述节点);
    }
    for(const auto& x:读全部自我写入(r))自我写入.insert(x.节点);
    std::vector<新语言问题信息> out;std::set<稳定编码> unique;
    for(const auto& f:r.字段(组键(组织键::登记根),组织键::问题登记)){
        const auto id=std::get<稳定编码>(f);组织要求(unique.insert(id).second&&r.整数(id,组织键::类型)==103);
        std::set<稳定编码> path;
        新语言问题信息 q{id,组织读值<新语言问题定义>(r,r.引用(id,组织键::定义),path),std::nullopt};核问题定义(q.定义);
        组织要求(r.引用(id,组织键::来源)==q.定义.出站来源&&r.引用(id,组织键::场景)==q.定义.出站来源场景);
        const auto a=r.字段(id,组织键::答案),k=r.字段(id,组织键::答案来源);
        组织要求(a.size()==k.size()&&a.size()<=1);
        if(!a.empty()){
            const auto kind=static_cast<新问题答案来源>(r.整数(id,组织键::答案来源));
            const auto info=std::get<稳定编码>(a.front());
            if(q.定义.被问种类==新问题被问种类::原文分界)
                组织要求(kind==新问题答案来源::原文分界回答&&r.整数(info,组织键::类型)==分界回答组织类型);
            else if(const auto 材料=短答.find(info);材料!=短答.end())
                组织要求(kind==新问题答案来源::消息转换&&明确整数短答与问题一致(q,*材料->second));
            else
                组织要求((kind==新问题答案来源::消息转换&&场景信息.contains(info))||(kind==新问题答案来源::自我写入&&自我写入.contains(info)));
            q.答案=新问题答案引用{info,kind};
        }
        组织要求(std::none_of(out.begin(),out.end(),[&](const auto& x){return x.定义.出站来源==q.定义.出站来源;}));
        out.push_back(std::move(q));
    }
    return out;
}
// 按结构比对：主体存在＝问题主体、特征类型＝被问栏、时刻严格晚于提问。不做文本匹配；主体待定不算一致。
bool 结构一致(const 新语言问题定义& q,const std::optional<稳定编码>& 主体,const std::optional<稳定编码>& 特征类型,std::int64_t 时刻){
    return 主体&&*主体==q.主体&&特征类型&&*特征类型==q.被问栏&&时刻>q.提问时刻;
}
bool 是值陈述答案(const 新接收场景信息& v){
    return v.种类==新接收场景信息种类::值陈述&&v.值种类&&*v.值种类!=新值陈述值种类::待求;
}
// 值陈述的结构比对（顶层裁决 2026-11-12，依计划既有规则放宽）：主体已绑定时按结构一致；主体待定时，
// 说话者是该问题所在交互的交互者（同一交互场景、说话者＝提问时的交互者）、特征类型＝被问栏、时刻晚于提问，也算答案。
bool 值陈述结构一致(const 新语言问题定义& q,const 新接收场景信息& v){
    if(q.被问种类==新问题被问种类::原文分界)return false;
    // R121（7240 §5.5第3项）：目标栏问题的答案是需求主体与问题主体一致、出现在提问之后的需求信息；载荷栏比对不变。
    if(q.被问种类==新问题被问种类::目标栏)
        return v.种类==新接收场景信息种类::需求信息&&v.主体存在==std::optional{q.主体}&&v.时刻>q.提问时刻;
    if(!是值陈述答案(v))return false;
    if(v.主体存在)return 结构一致(q,v.主体存在,v.特征类型,v.时刻);
    return v.交互场景==q.交互场景&&v.说话者==q.交互者&&v.特征类型&&*v.特征类型==q.被问栏&&v.时刻>q.提问时刻;
}
新问题答案查询结果 查答案已加锁(组织读取现场& r,const 新语言问题信息& q){
    const auto& 定义=q.定义;
    if(定义.被问种类==新问题被问种类::原文分界)return {};
    新问题答案查询结果 out;
    for(const auto& x:读全部场景信息(r))
        if(明确整数短答与问题一致(q,x.内容)||值陈述结构一致(定义,x.内容))
            out.答案组.push_back({x.陈述节点,新问题答案来源::消息转换});
    if(定义.被问种类==新问题被问种类::载荷栏)for(const auto& x:读全部自我写入(r))
        if(结构一致(定义,x.内容.主体存在,x.内容.特征类型,x.内容.时刻))
            out.答案组.push_back({x.节点,新问题答案来源::自我写入});
    return out;
}
// 新信息可能回答的尚无答案的问题（就绪通知键）。
std::vector<稳定编码> 待通知问题(组织读取现场& r,const std::optional<稳定编码>& 主体,const std::optional<稳定编码>& 特征类型,std::int64_t 时刻){
    std::vector<稳定编码> out;
    for(const auto& q:读全部问题(r))if(!q.答案&&q.定义.被问种类==新问题被问种类::载荷栏&&结构一致(q.定义,主体,特征类型,时刻))out.push_back(q.问题节点);
    return out;
}
std::vector<稳定编码> 待通知问题(组织读取现场& r,const 新接收场景信息& v){
    std::vector<稳定编码> out;
    for(const auto& q:读全部问题(r))if(!q.答案&&值陈述结构一致(q.定义,v))out.push_back(q.问题节点);
    return out;
}
// 问题答案就绪通知端口：独立互斥，不取语言锁；投递时持端口互斥，解除时等待在途投递结束。
std::mutex 问题答案通知互斥;
const void* 问题答案通知所有者=nullptr;
std::function<void(稳定编码)> 问题答案通知;
void 发出问题答案就绪通知(std::span<const 稳定编码> 问题组) noexcept {
    try {
        std::lock_guard lock(问题答案通知互斥);
        if(问题答案通知)for(const auto q:问题组)问题答案通知(q);
    } catch(...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"发出问题答案就绪通知",L"端口互斥或投递发生工程故障。");
    }
}
} // namespace
namespace {
constexpr std::int64_t 接收语言解释记录类型=112;
constexpr std::int64_t 接收篇章采用记录类型=113;
constexpr std::int64_t 接收指代实例记录类型=114;
using 接收指代范围 = std::pair<std::uint64_t, std::uint64_t>;

接收指代范围 读接收指代范围(组织读取现场&, 稳定编码);
void 核接收指代来源(组织读取现场&, 稳定编码, const 新语言前置来源信息&,
    const 新接收语言解释场景绑定&);

bool 是篇章主体指代(const 接收语言句段解释& 句) noexcept {
    return 句.主体关系 && 句.主体关系->指代概念.has_value();
}

新语言分界工程状态 接收语言解释场景状态(const 新接收语言解释场景结果& 结果) {
    using S = 新语言分界工程状态;
    switch (结果.状态) {
    case 新来源子场景状态::已存在:
        组织要求(结果.绑定.has_value());
        return S::已完成;
    case 新来源子场景状态::未建立: return S::未找到;
    case 新来源子场景状态::前提已变化: return S::前提已变化;
    case 新来源子场景状态::资源失败: return S::资源失败;
    default: return S::输入拒绝;
    }
}

const 接收语言已解释角色* 取静态角色(const 接收语言静态结构& 结构, 新语言知识参数角色 参数) {
    const auto 项=std::ranges::find(结构.角色组, 参数, &接收语言已解释角色::参数);
    return 项==结构.角色组.end() ? nullptr : &*项;
}

bool 可投影接收静态句(const 接收语言句段解释& 句) {
    using O = 新语言知识含义操作;
    return 句.静态结构 && (句.静态结构->操作 == O::概念归类 || 句.静态结构->操作 == O::特征描述);
}

template<class T>
const T* 静态角色值(const 接收语言已解释角色* 角色) {
    return 角色 && 角色->值 ? std::get_if<T>(&*角色->值) : nullptr;
}

新表达特征值 静态特征值(const 语言图引用值* 类型, const 接收语言已解释角色* 角色) {
    using C=新语言知识角色类别;
    if (const auto* 概念=静态角色值<语言图引用值>(角色); 概念 && 概念->类别==C::特征概念)
        return 新表达概念条件{概念->节点};
    if (const auto* 数=静态角色值<语言图整数值>(角色); 数 && 类型 && 类型->类别==C::I64标量特征类型) {
        std::int64_t 准确值=0;
        const auto& 文=数->规范十进制;
        const auto 解析=std::from_chars(文.data(),文.data()+文.size(),准确值);
        if (解析.ec==std::errc{} && 解析.ptr==文.data()+文.size())
            return 新类型化表达值{类型->节点,数->单位.value_or(稳定编码{}),准确值};
    }
    return 新表达指称{新明确未知{}};
}

std::optional<新语言含义网> 组织接收静态含义(const 新接收语言解释内容& 内容, const std::string& 固定原文) {
    using C=新语言知识角色类别;
    using P=新语言知识参数角色;
    using O=新语言知识含义操作;
    if (!std::in_range<std::int64_t>(内容.原文终点)) return {};

    新语言含义网 网;
    网.所属语言场景=内容.来源场景;
    std::map<std::uint64_t,新表达指称> 已称名对象;
    std::vector<std::size_t> 字节边界;
    const auto 称名原文=[&](std::uint64_t 起,std::uint64_t 末) {
        if (字节边界.empty()) {
            字节边界.push_back(0);
            for (std::size_t 位置=0;位置<固定原文.size();) {
                (void)读取已校验码点(固定原文,位置);
                字节边界.push_back(位置);
            }
        }
        if (末>=字节边界.size()) return std::string{};
        return 固定原文.substr(字节边界[起],字节边界[末]-字节边界[起]);
    };

    for (const auto& 句 : 内容.句段组) {
        if (!可投影接收静态句(句)) continue;
        const auto& 结构=*句.静态结构;
        const auto* 主体角色=取静态角色(结构,P::主体);
        const auto* 主体=静态角色值<语言图引用值>(主体角色);
        const auto* 归类=静态角色值<语言图引用值>(取静态角色(结构,P::归类概念));
        新静态含义单元 单元;
        单元.主体=新明确未知{};
        if (主体 && 主体->类别==C::存在) {
            单元.主体=新绑定指称{新指称类别::存在,主体->节点,主体->来源概念.value_or(稳定编码{})};
            if (句.主体关系 && !句.主体关系->指代概念 && 句.主体关系->来源
                && 句.主体关系->来源->已绑定存在==主体->节点)
                已称名对象.emplace(句.主体关系->对象起始标量,单元.主体);
        } else if (主体角色 && 句.主体关系 && 句.主体关系->已确定
            && 句.主体关系->所用构式==结构.所用构式
            && 句.主体关系->起始标量==主体角色->起始标量 && 句.主体关系->结束标量==主体角色->结束标量) {
            const auto& 关系=*句.主体关系;
            if (关系.指代概念) {
                const auto 前文=已称名对象.find(关系.对象起始标量);
                if (前文!=已称名对象.end()) 单元.主体=前文->second;
            } else if (归类 && 归类->类别==C::存在概念) {
                const 新匿名表达对象 对象{static_cast<std::int64_t>(关系.对象起始标量),归类->节点};
                已称名对象.emplace(关系.对象起始标量,对象);
                单元.主体=对象;
            } else if (auto 名称=称名原文(关系.起始标量,关系.结束标量); !名称.empty()) {
                单元.主体=新名称描述指称{std::move(名称),{}};
            }
        }

        if (结构.操作==O::概念归类) {
            新概念归属谓项 谓项{新明确未知{}};
            if (归类 && 归类->类别==C::存在概念)
                谓项.概念=新绑定指称{新指称类别::概念,归类->节点,{}};
            单元.谓项=std::move(谓项);
        } else {
            const auto* 类型=静态角色值<语言图引用值>(取静态角色(结构,P::特征类型));
            新特征值谓项 谓项{新明确未知{},静态特征值(类型,取静态角色(结构,P::特征值))};
            if (类型 && (类型->类别==C::正式特征类型 || 类型->类别==C::I64标量特征类型))
                谓项.特征类型=新绑定指称{新指称类别::特征类型,类型->节点,{}};
            单元.谓项=std::move(谓项);
        }
        const auto 编号=static_cast<std::int64_t>(网.内容项组.size());
        网.内容项组.push_back({编号,std::move(单元),
            {{static_cast<std::int64_t>(句.起始标量),static_cast<std::int64_t>(句.结束标量-句.起始标量)}},
            {结构.所用构式},{}});
    }
    if (网.内容项组.empty()) return {};
    if (网.内容项组.size()>1) {
        新篇章组合 顺序;
        for (const auto& 项 : 网.内容项组) 顺序.有序子项组.push_back(项.编号);
        网.内容项组.push_back({static_cast<std::int64_t>(网.内容项组.size()),std::move(顺序),{},{},{}});
    }
    网.顶层项组.push_back({{},新表达用途::陈述,网.内容项组.back().编号});
    核验含义网结构(网);
    return 网;
}

bool 接收语言解释内容有效(const 新接收语言解释内容& 内容, const 新语言前置来源信息& 来源,
    const 新接收语言解释场景绑定& 场景) {
    if (内容.来源场景 != 场景.场景() || 内容.前置来源 != 来源.节点 || !有效(内容.语言节点)
        || 内容.原文起点 != 来源.来源.原文起点 || 内容.原文终点 != 来源.来源.原文终点
        || 内容.句段组.empty()) return false;
    auto 前段末 = 内容.原文起点;
    std::map<std::pair<std::uint64_t,std::uint64_t>,const 接收语言主体关系*> 已称名;
    std::set<稳定编码> 正式前文对象;
    for (const auto& 句 : 内容.句段组) {
        if (句.起始标量 < 前段末 || 句.起始标量 >= 句.结束标量 || 句.结束标量 > 内容.原文终点)
            return false;
        前段末 = 句.结束标量;
        const auto 在段内 = [&](std::uint64_t 起, std::uint64_t 末) {
            return 句.起始标量 <= 起 && 起 < 末 && 末 <= 句.结束标量;
        };
        for (const auto& 缺口 : 句.缺口组) {
            if (!在段内(缺口.起始标量,缺口.结束标量)
                || 缺口.种类 < 新前置语言缺口种类::词面未登记 || 缺口.种类 > 新前置语言缺口种类::角色原文待确认)
                return false;
        }
        if (句.关系观察) for (const auto& 候选 : 句.关系观察->候选组) {
            if (!有效(候选.关系概念) || !有效(候选.所用构式) || 候选.种子键.empty()
                || 候选.源角色键.empty() || 候选.目标角色键.empty() || 候选.端点组.size()<2)
                return false;
            for (const auto& 端点 : 候选.端点组) {
                if (端点.角色键.empty()
                    || ((端点.起始标量 || 端点.结束标量) && !在段内(端点.起始标量,端点.结束标量))
                    || (端点.指称 && !有效(端点.指称->节点))) return false;
            }
        }
        for (const auto& 依据 : 句.词条类型依据组) {
            using C=新语言知识角色类别;
            if (!在段内(依据.起始标量,依据.结束标量) || !有效(依据.词条) || 依据.构式依据组.empty()
                || (依据.所需类别!=C::存在概念 && 依据.所需类别!=C::特征概念 && 依据.所需类别!=C::正式特征类型))
                return false;
        }
        if (句.静态结构) {
            const auto& 结构 = *句.静态结构;
            using O=新语言知识含义操作;
            if (!有效(结构.所用构式) || 结构.角色组.empty()
                || (!可投影接收静态句(句) && 结构.操作!=O::特征子类型定义))
                return false;
            std::set<std::int64_t> 角色编号;
            std::set<新语言知识参数角色> 参数组;
            for (const auto& 角色 : 结构.角色组) {
                using P=新语言知识参数角色;
                if (!在段内(角色.起始标量,角色.结束标量) || !角色编号.insert(角色.角色编号).second
                    || 角色.参数<P::主体 || 角色.参数>P::适用对象 || !参数组.insert(角色.参数).second)
                    return false;
                if (!角色.值) continue;
                if (const auto* 引用=std::get_if<语言图引用值>(&*角色.值)) {
                    using C=新语言知识角色类别;
                    if (引用->类别<C::存在 || 引用->类别>C::查询存在 || !有效(引用->节点)
                        || (引用->来源概念 && !有效(*引用->来源概念))
                        || (引用->选用名称关系 && !有效(*引用->选用名称关系)))
                        return false;
                } else {
                    const auto& 整数=std::get<语言图整数值>(*角色.值);
                    if (整数.规范十进制.empty() || (整数.单位 && !有效(*整数.单位))) return false;
                }
            }
            if (结构.操作==O::特征子类型定义) {
                using P=新语言知识参数角色;
                using C=新语言知识角色类别;
                const auto 类型相容 = [&](P 参数, C 类别) {
                    const auto* 角色 = 取静态角色(结构, 参数);
                    if (!角色 || !角色->值) return 角色 != nullptr;
                    const auto* 引用 = std::get_if<语言图引用值>(&*角色->值);
                    return 引用 && 引用->类别 == 类别;
                };
                const auto* 下 = 取静态角色(结构, P::下界);
                const auto* 上 = 取静态角色(结构, P::上界);
                const auto 是整数角色 = [](const 接收语言已解释角色* 角色) {
                    return !角色->值 || std::holds_alternative<语言图整数值>(*角色->值);
                };
                if (!类型相容(P::主体, C::特征概念) || !类型相容(P::特征类型, C::I64标量特征类型)
                    || !类型相容(P::适用对象, C::存在概念) || (下==nullptr)!=(上==nullptr)
                    || 参数组.size() != (下 ? 5 : 3)
                    || (下 && (!是整数角色(下) || !是整数角色(上)))) return false;
            }
        }
        if (句.主体关系) {
            const auto& 关系 = *句.主体关系;
            if (!在段内(关系.起始标量,关系.结束标量) || !有效(关系.所用构式)) return false;
            if (关系.来源) {
                const auto& 依据=*关系.来源;
                if ((依据.词条 && !有效(*依据.词条)) || (依据.已绑定存在 && !有效(*依据.已绑定存在))
                    || (依据.名称关系 && (!依据.词条 || !有效(*依据.名称关系)))
                    || (关系.指代概念 && !依据.名称关系) || (依据.已绑定存在 && !关系.已确定))
                    return false;
                const auto& 冲突 = 依据.已证冲突候选;
                if (!冲突.empty() && (!关系.指代概念 || 关系.已确定 || 关系.对象起始标量
                    || 关系.对象结束标量 || 冲突.size() != 2 || 冲突.front() == 冲突.back()
                    || !正式前文对象.contains(冲突.front()) || !正式前文对象.contains(冲突.back())))
                    return false;
            }
            const auto 绑定存在=关系.来源 ? 关系.来源->已绑定存在 : std::nullopt;
            const auto 对象范围 = std::pair{关系.对象起始标量,关系.对象结束标量};
            if (关系.指代概念) {
                if (!有效(*关系.指代概念)) return false;
                // 空半开范围表示仅取得出现依据，尚无唯一目标。
                if (关系.对象起始标量==0 && 关系.对象结束标量==0) {
                    if (关系.已确定 || 绑定存在 || !关系.来源 || !关系.来源->名称关系) return false;
                    continue;
                }
                const auto 前文=已称名.find(对象范围);
                if (前文==已称名.end()
                    || (关系.已确定 && !前文->second->已确定)) return false;
                const auto& 前文来源=前文->second->来源;
                if (绑定存在!=(前文来源 ? 前文来源->已绑定存在 : std::nullopt)) return false;
            } else {
                if (对象范围 != std::pair{关系.起始标量,关系.结束标量}) return false;
                if (绑定存在) {
                    using P=新语言知识参数角色;
                    using C=新语言知识角色类别;
                    if (!句.静态结构 || 句.静态结构->所用构式!=关系.所用构式) return false;
                    const auto* 角色=取静态角色(*句.静态结构,P::主体);
                    const auto* 引用=静态角色值<语言图引用值>(角色);
                    if (!引用 || 引用->类别!=C::存在 || 引用->节点!=*绑定存在
                        || 角色->起始标量!=关系.起始标量 || 角色->结束标量!=关系.结束标量) return false;
                    正式前文对象.insert(*绑定存在);
                }
                已称名.emplace(对象范围,&关系);
            }
        }
    }
    return true;
}

bool 具备接收篇章采用材料(const 新接收语言解释内容& 内容,const std::string& 固定原文) {
    if (!内容.含义候选 || !std::ranges::all_of(内容.句段组,[](const auto& 句) {
        return 可投影接收静态句(句) && 句.缺口组.empty();
    })) return false;

    auto 句=内容.句段组.begin();
    auto 标量=内容.原文起点;
    for (std::size_t 字节=0;字节<固定原文.size();++标量) {
        const auto 字=读取已校验码点(固定原文,字节);
        while (句!=内容.句段组.end() && 标量>=句->结束标量) ++句;
        if (句!=内容.句段组.end() && 句->起始标量<=标量) continue;
        if (字!=U' ' && 字!=U'\t' && 字!=U'\r' && 字!=U'\n' && 字!=0x3000) return false;
    }
    for (const auto& 项 : 内容.含义候选->内容项组) {
        if (std::holds_alternative<新篇章组合>(项.内容)) continue;
        const auto& 单元=std::get<新静态含义单元>(项.内容);
        const auto* 主体=std::get_if<新绑定指称>(&单元.主体);
        if (!主体 || 主体->类别!=新指称类别::存在) return false;
        if (const auto* 归类=std::get_if<新概念归属谓项>(&单元.谓项)) {
            const auto* 概念=std::get_if<新绑定指称>(&归类->概念);
            if (!概念 || 概念->类别!=新指称类别::概念) return false;
        } else {
            const auto& 特征=std::get<新特征值谓项>(单元.谓项);
            const auto* 类型=std::get_if<新绑定指称>(&特征.特征类型);
            if (!类型 || 类型->类别!=新指称类别::特征类型) return false;
            if (const auto* 值=std::get_if<新类型化表达值>(&特征.值)) {
                if (值->类型!=类型->节点 || !std::holds_alternative<std::int64_t>(值->准确值)) return false;
            } else if (!std::holds_alternative<新表达概念条件>(特征.值)) return false;
        }
    }
    return true;
}

新接收语言解释内容 读接收语言解释定义(组织读取现场& 读, 稳定编码 定义,
    const 新语言前置来源信息& 来源, const 新接收语言解释场景绑定& 场景) {
    std::set<稳定编码> 路径;
    auto 内容=组织读值<新接收语言解释内容>(读,定义,路径);
    组织要求(接收语言解释内容有效(内容,来源,场景));
    if (内容.含义候选) {
        const auto& 网=*内容.含义候选;
        组织要求(!网.身份 && 网.所属语言场景==场景.场景() && std::in_range<std::int64_t>(内容.原文终点));
        核验含义网结构(网);
        const auto& 顶=网.顶层项组.front();
        组织要求(顶.表达用途==新表达用途::陈述 && !顶.语言出现编号);
        const 新篇章组合* 文本顺序=nullptr;
        std::vector<std::int64_t> 静态项顺序;
        auto 句=内容.句段组.begin();
        for (const auto& 项 : 网.内容项组) {
            if (const auto* 顺序=std::get_if<新篇章组合>(&项.内容)) {
                组织要求(!文本顺序 && 顺序->种类==新篇章组合种类::文本顺序 && 项.编号==顶.内容引用);
                文本顺序=顺序;
                continue;
            }
            const auto* 静态=std::get_if<新静态含义单元>(&项.内容);
            组织要求(静态 && 项.原文范围组.size()==1 && 项.所用知识组.size()==1
                && (std::holds_alternative<新概念归属谓项>(静态->谓项)
                    || std::holds_alternative<新特征值谓项>(静态->谓项)));
            句=std::find_if(句,内容.句段组.end(),可投影接收静态句);
            组织要求(句!=内容.句段组.end()
                && 项.原文范围组.front()==新含义原文范围{static_cast<std::int64_t>(句->起始标量),
                    static_cast<std::int64_t>(句->结束标量-句->起始标量)}
                && 项.所用知识组.front()==句->静态结构->所用构式);
            静态项顺序.push_back(项.编号);
            ++句;
        }
        组织要求(std::none_of(句,内容.句段组.end(),可投影接收静态句));
        if (文本顺序) 组织要求(文本顺序->有序子项组==静态项顺序);
        else 组织要求(静态项顺序.size()==1 && 静态项顺序.front()==顶.内容引用);
    }
    return 内容;
}

新接收语言解释信息 读接收语言解释记录(组织读取现场& 读, 稳定编码 记录,
    const 新语言前置来源信息& 来源, const 新接收语言解释场景绑定& 场景) {
    组织要求(读.整数(记录,组织键::类型)==接收语言解释记录类型
        && 读.引用(记录,组织键::场景)==场景.场景()
        && 读.引用(记录,组织键::来源)==来源.节点);
    const auto 当前=读.引用(记录,组织键::定义);
    新接收语言解释信息 成果{记录,读接收语言解释定义(读,当前,来源,场景)};
    const auto 采用组=读.字段(记录,组织键::采用篇章);
    组织要求(采用组.size()<=1);
    if (采用组.empty()) return 成果;

    const auto 采用=std::get<稳定编码>(采用组.front());
    组织要求(读.整数(采用,组织键::类型)==接收篇章采用记录类型
        && 读.引用(采用,组织键::场景)==场景.场景()
        && 读.引用(采用,组织键::来源)==来源.节点);
    const auto 定义=读.引用(采用,组织键::定义);
    const auto 说话者=读.引用(采用,组织键::篇章说话者);
    const auto 定义历史=读.字段(记录,组织键::子项);
    组织要求(有效(说话者) && std::ranges::find(定义历史,基础字段内容{定义})!=定义历史.end());
    auto 已采用=定义==当前 ? 成果.内容 : 读接收语言解释定义(读,定义,来源,场景);
    组织要求(已采用.含义候选.has_value());
    std::vector<新接收篇章指代目标> 指代目标组;
    const auto 采用网组 = 读.字段(采用, 组织键::含义网);
    组织要求(采用网组.size() <= 1);
    if (!采用网组.empty()) {
        std::set<稳定编码> 路径;
        auto 采用网 = 组织读值<新语言含义网>(读, std::get<稳定编码>(采用网组.front()), 路径);
        核验含义网结构(采用网);
        auto 复原网 = 采用网;
        const auto& 原网 = *已采用.含义候选;
        组织要求(采用网.内容项组.size() == 原网.内容项组.size()
            && 已采用.句段组.size() <= 采用网.内容项组.size());
        for (std::size_t i = 0; i < 已采用.句段组.size(); ++i) {
            const auto& 句 = 已采用.句段组[i];
            if (!是篇章主体指代(句)) continue;
            auto& 主体 = std::get<新静态含义单元>(复原网.内容项组[i].内容).主体;
            const auto* 指代 = std::get_if<新指代实例引用>(&主体);
            const auto& 原主体 = std::get<新静态含义单元>(原网.内容项组[i].内容).主体;
            const auto* 目标 = std::get_if<新绑定指称>(&原主体);
            const auto& 关系 = *句.主体关系;
            组织要求(指代 && 指代->节点 && !指代->网内编号 && 目标 && 目标->类别 == 新指称类别::存在
                && 关系.来源 && 关系.来源->已绑定存在 == 目标->节点);
            const auto P = *指代->节点;
            // 采用提交已保护场景成员；历史读回只使用实例的固定来源。
            核接收指代来源(读, P, 来源, 场景);
            组织要求(读接收指代范围(读, P) == 接收指代范围{关系.起始标量, 关系.结束标量});
            指代目标组.push_back({复原网.内容项组[i].编号, P, *目标});
            主体 = 原主体;
        }
        组织要求(!指代目标组.empty() && 复原网 == 原网);
        已采用.含义候选 = std::move(采用网);
    }
    已采用.含义候选->身份=采用;
    成果.篇章采用=新接收篇章采用信息{采用,说话者,std::move(已采用),std::move(指代目标组)};
    return 成果;
}

新状态强时间 取得分界当前UTC();

bool 可保存主体指代(const 接收语言主体关系& 出现) {
    return 出现.指代概念 && 出现.来源 && 出现.来源->词条 && 出现.来源->名称关系;
}

bool 同一主体指代角色(const 接收语言主体关系& 一, const 接收语言主体关系& 二) {
    return 一.所用构式 == 二.所用构式 && 一.指代概念 == 二.指代概念
        && 一.来源->词条 == 二.来源->词条 && 一.来源->名称关系 == 二.来源->名称关系;
}

std::optional<稳定编码> 读接收指代当前依据(const 基础节点结构& P) {
    std::optional<稳定编码> 依据;
    for (const auto& 字段 : P.字段组) {
        if (字段.字段节点 != 组键(组织键::指代当前依据)) continue;
        const auto* 引用 = std::get_if<稳定编码>(&字段.内容);
        组织要求(!依据 && 引用 && 有效(*引用));
        依据 = *引用;
    }
    return 依据;
}

接收指代范围 读接收指代范围(组织读取现场& 读, 稳定编码 P) {
    const auto 字段 = 读.唯一(P, 组织键::原文);
    const auto& 范围 = std::get<std::vector<std::uint64_t>>(std::get<基础值>(字段).材料);
    组织要求(范围.size() == 2 && 范围.front() < 范围.back());
    return {范围.front(), 范围.back()};
}

const 接收语言主体关系* 查找接收主体指代(
    const 新接收语言解释内容& 内容, const 接收指代范围& 范围) {
    for (const auto& 句 : 内容.句段组) {
        if (!句.主体关系) continue;
        const auto& 出现 = *句.主体关系;
        if (可保存主体指代(出现) && 接收指代范围{出现.起始标量, 出现.结束标量} == 范围)
            return &出现;
    }
    return nullptr;
}

void 核接收指代来源(组织读取现场& 读, 稳定编码 P, const 新语言前置来源信息& 来源,
    const 新接收语言解释场景绑定& 场景) {
    组织要求(读.整数(P, 组织键::类型) == 接收指代实例记录类型
        && 读.引用(P, 组织键::来源) == 来源.节点
        && 读.引用(P, 组织键::场景) == 场景.场景());
}
} // namespace

新语言分界调用结果<新接收语言解释信息> 新_语言类::保存接收语言解释(
    const 新接收认知来源键& 来源键, const 新接收语言解释内容& 请求内容, 稳定编码 说话者,
    const 新_自然语言世界树类& 世界, const 新语言分界消息读取接口& 消息读取) {
    using S = 新语言分界工程状态;
    std::optional<稳定编码> 本次已发表;
    try {
        const auto 来源=读取前置来源(来源键,消息读取);
        if (来源.状态!=S::已完成 || !来源.信息) return {来源.状态,{},{}};
        const auto 场景结果=世界.读取接收语言解释场景(来源键.接收消息节点);
        const auto 场景状态=接收语言解释场景状态(场景结果);
        if (场景状态!=S::已完成) return {场景状态,{},{}};
        const auto& 场景=*场景结果.绑定;
        if (!有效(说话者) || !接收语言解释内容有效(请求内容,*来源.信息,场景) || !是语言节点(请求内容.语言节点))
            return {S::输入拒绝,{},{}};
        auto 内容=请求内容;
        内容.含义候选=组织接收静态含义(内容,来源.信息->固定UTF8原文);
        const bool 完整=!std::ranges::any_of(内容.句段组,是篇章主体指代)
            && 具备接收篇章采用材料(内容,来源.信息->固定UTF8原文);

        std::lock_guard 锁(新语言互斥);
        if (!有效(组织句柄.front())) return {S::未初始化,{},{}};
        组织读取现场 读;
        const auto 记录=场景.记录();
        std::optional<新接收语言解释信息> 已有;
        if (记录) {
            已有=读接收语言解释记录(读,*记录,*来源.信息,场景);
            if (已有->内容.语言节点!=内容.语言节点) return {S::输入拒绝,{},{}};
        }
        const bool 相同=已有 && 已有->内容==内容;
        const bool 首次采用=完整 && (!已有 || !已有->篇章采用);
        if (相同 && !首次采用) return {S::已完成,std::move(已有),{}};

        组织写入现场 写;
        std::optional<std::size_t> 新定义;
        if (!相同) 新定义=写.写(内容);
        std::optional<std::size_t> 新记录;
        if (记录) {
            写.请求.只读预期=场景.绑定预期();
            写.请求.预期联合节点组.push_back(读.读(*记录));
            if (新定义) {
                写.请求.当前字段赋值组.push_back(
                    {*记录,组键(组织键::定义),基础组内节点引用{*新定义}});
                写.请求.追加字段组.push_back(
                    {*记录,组键(组织键::子项),基础组内节点引用{*新定义}});
            }
        } else {
            新记录=写.节点(接收语言解释记录类型,0);
            写.引用(*新记录,组织键::场景,内容.来源场景);
            写.引用(*新记录,组织键::来源,内容.前置来源);
            写.子(*新记录,组织键::定义,*新定义);
            写.子(*新记录,组织键::子项,*新定义);
            组织要求(世界.合并接收语言解释首次挂接(场景,写.请求,基础组内节点引用{*新记录}));
        }
        if (首次采用) {
            const auto 采用=写.节点(接收篇章采用记录类型,0);
            写.引用(采用,组织键::场景,内容.来源场景);
            写.引用(采用,组织键::来源,内容.前置来源);
            写.引用(采用,组织键::篇章说话者,说话者);
            if (新定义) 写.子(采用,组织键::定义,*新定义);
            else 写.引用(采用,组织键::定义,读.引用(*记录,组织键::定义));
            if (新记录) 写.子(*新记录,组织键::采用篇章,采用);
            else 写.请求.追加字段组.push_back(
                {*记录,组键(组织键::采用篇章),基础组内节点引用{采用}});
        }
        // 新解释引用只在 L1 发布时核对仍然可读，失败后按同一消息重读。
        for (const auto 引用 : 写.引用组) 写.请求.只读预期.节点组.push_back({引用});
        const auto 发布=全局基础数据集.原子追加节点组(写.请求);
        using L1=基础节点组追加状态;
        if (发布.状态==L1::预期不符 || 发布.状态==L1::节点不存在 || 发布.状态==L1::引用不存在)
            return {S::前提已变化,{},{}};
        if (发布.状态==L1::资源失败) return {S::资源失败,{},{}};
        组织要求(发布.状态==L1::已完成 && 发布.新节点组.size()==写.请求.新节点组.size());
        const auto 节点=记录 ? *记录 : 发布.新节点组[*新记录];
        本次已发表=节点;
        组织读取现场 回读;
        return {S::已完成,读接收语言解释记录(回读,节点,*来源.信息,场景),本次已发表};
    } catch (const std::bad_alloc&) { return {S::资源失败,{},本次已发表}; }
      catch (const std::length_error&) { return {S::资源失败,{},本次已发表}; }
}

新语言分界调用结果<新接收语言解释信息> 新_语言类::采用当前接收指代篇章(
    const 新接收认知来源键& 来源键, 稳定编码 说话者,
    const 新_自然语言世界树类& 世界, const 新语言分界消息读取接口& 消息读取) {
    using S = 新语言分界工程状态;
    using L1 = 基础节点组追加状态;
    std::optional<稳定编码> 本次已发表;
    try {
        if (!有效(说话者)) return {S::输入拒绝, {}, {}};
        const auto 来源 = 读取前置来源(来源键, 消息读取);
        if (来源.状态 != S::已完成 || !来源.信息) return {来源.状态, {}, {}};
        const auto 场景结果 = 世界.读取接收语言解释场景(来源键.接收消息节点);
        const auto 状态 = 接收语言解释场景状态(场景结果);
        if (状态 != S::已完成) return {状态, {}, {}};
        const auto& 场景 = *场景结果.绑定;
        const auto 记录 = 场景.记录();
        if (!记录) return {S::未找到, {}, {}};
        struct 待核指代 final {
            std::size_t 内容位置;
            稳定编码 P, F, E;
        };
        std::vector<待核指代> 待核组;
        新语言含义网 采用网;
        组织写入现场 写;
        稳定编码 定义, 类型;
        const 新_特征类* 特征;
        {
            std::lock_guard 锁(新语言互斥);
            if (!指代特征服务_) return {S::未初始化, {}, {}};
            特征 = 指代特征服务_;
            类型 = 指代消解结果特征类型_;
            组织读取现场 读;
            auto 当前 = 读接收语言解释记录(读, *记录, *来源.信息, 场景);
            if (当前.篇章采用) return {S::已完成, std::move(当前), {}};
            if (!具备接收篇章采用材料(当前.内容, 来源.信息->固定UTF8原文))
                return {S::已完成, std::move(当前), {}};
            定义 = 读.引用(*记录, 组织键::定义);
            写.请求.预期联合节点组.push_back(读.读(*记录));
            写.请求.只读预期 = 场景.绑定预期();
            const auto& 成员预期 = 场景.指代成员预期();
            组织要求(成员预期.字段节点.has_value());
            写.请求.只读预期.节点组.push_back({*成员预期.字段节点});
            写.请求.只读预期.字段集合组.push_back(成员预期);
            std::map<接收指代范围, 稳定编码> 成员;
            for (const auto P : 场景.指代实例组()) {
                核接收指代来源(读, P, *来源.信息, 场景);
                组织要求(成员.emplace(读接收指代范围(读, P), P).second);
            }
            采用网 = *当前.内容.含义候选;
            for (std::size_t i = 0; i < 当前.内容.句段组.size(); ++i) {
                const auto& 句 = 当前.内容.句段组[i];
                if (!是篇章主体指代(句)) continue;
                const auto& 关系 = *句.主体关系;
                const auto 匹配 = 成员.find({关系.起始标量, 关系.结束标量});
                const auto& 主体 = std::get<新静态含义单元>(采用网.内容项组[i].内容).主体;
                const auto& 目标 = std::get<新绑定指称>(主体);
                if (匹配 == 成员.end() || !关系.来源 || 关系.来源->已绑定存在 != 目标.节点)
                    return {S::已完成, std::move(当前), {}};
                const auto P = 匹配->second;
                const auto& 快照 = 读.读(P);
                if (读接收指代当前依据(快照.结构) != 定义)
                    return {S::已完成, std::move(当前), {}};
                const auto F = 读.引用(P, 组织键::指代消解结果);
                写.请求.预期联合节点组.push_back(快照);
                待核组.push_back({i, P, F, 目标.节点});
            }
            if (待核组.empty()) return {S::已完成, std::move(当前), {}};
        }
        for (const auto& 项 : 待核组) {
            const auto 读取 = 特征->读取特征直接引用当前值(项.F, 类型, 项.P, true);
            if (读取.状态 == 新特征类型读取状态::资源失败) return {S::资源失败, {}, {}};
            if (读取.状态 != 新特征类型读取状态::已读取 || !读取.同批节点
                || 读接收指代当前依据(*读取.同批节点) != 定义 || 读取.当前引用 != 项.E)
                return {S::前提已变化, {}, {}};
            auto& 预期 = 写.请求.只读预期;
            const auto& F预期 = 读取.特征当前预期;
            预期.节点组.insert(预期.节点组.end(), F预期.节点组.begin(), F预期.节点组.end());
            预期.字段集合组.insert(预期.字段集合组.end(), F预期.字段集合组.begin(), F预期.字段集合组.end());
            std::get<新静态含义单元>(采用网.内容项组[项.内容位置].内容).主体 =
                新指代实例引用{项.P, {}};
        }
        核验含义网结构(采用网);
        const auto 采用 = 写.节点(接收篇章采用记录类型, 0);
        写.引用(采用, 组织键::场景, 场景.场景());
        写.引用(采用, 组织键::来源, 来源.信息->节点);
        写.引用(采用, 组织键::篇章说话者, 说话者);
        写.引用(采用, 组织键::定义, 定义);
        const auto 网槽 = 写.写(采用网);
        写.子(采用, 组织键::含义网, 网槽);
        写.请求.追加字段组.push_back({*记录, 组键(组织键::采用篇章), 基础组内节点引用{采用}});
        for (const auto 引用 : 写.引用组) 写.请求.只读预期.节点组.push_back({引用});
        const auto 发布 = 全局基础数据集.原子追加节点组(写.请求);
        if (发布.状态 == L1::预期不符 || 发布.状态 == L1::节点不存在 || 发布.状态 == L1::引用不存在)
            return {S::前提已变化, {}, {}};
        if (发布.状态 == L1::资源失败) return {S::资源失败, {}, {}};
        组织要求(发布.状态 == L1::已完成 && 发布.新节点组.size() == 写.请求.新节点组.size());
        本次已发表 = 发布.新节点组[采用];
        auto 回读 = 读取接收语言解释(来源键, 世界, 消息读取);
        回读.本次已发表身份 = 本次已发表;
        return 回读;
    } catch (const std::bad_alloc&) { return {S::资源失败, {}, 本次已发表}; }
      catch (const std::length_error&) { return {S::资源失败, {}, 本次已发表}; }
}

新语言分界调用结果<新接收语言解释信息> 新_语言类::读取接收语言解释(
    const 新接收认知来源键& 来源键, const 新_自然语言世界树类& 世界,
    const 新语言分界消息读取接口& 消息读取) const {
    using S=新语言分界工程状态;
    try {
        const auto 来源=读取前置来源(来源键,消息读取);
        if (来源.状态!=S::已完成 || !来源.信息) return {来源.状态,{},{}};
        const auto 场景=世界.读取接收语言解释场景(来源键.接收消息节点);
        const auto 状态=接收语言解释场景状态(场景);
        if (状态!=S::已完成) return {状态,{},{}};
        const auto 记录=场景.绑定->记录();
        if (!记录) return {S::未找到,{},{}};
        std::lock_guard 锁(新语言互斥);
        if (!有效(组织句柄.front())) return {S::未初始化,{},{}};
        组织读取现场 读;
        return {S::已完成,读接收语言解释记录(读,*记录,*来源.信息,*场景.绑定),{}};
    } catch (const std::bad_alloc&) { return {S::资源失败,{},{}}; }
      catch (const std::length_error&) { return {S::资源失败,{},{}}; }
}

新语言分界工程状态 新_语言类::配置指代消解结果(稳定编码 类型, 新_特征类& 特征) {
    using S = 新语言分界工程状态;
    if (!有效(类型)) return S::输入拒绝;
    std::lock_guard 锁(新语言互斥);
    if (!有效(组织句柄.front())) return S::未初始化;
    if (指代特征服务_)
        return 指代特征服务_ == &特征 && 指代消解结果特征类型_ == 类型 ? S::已完成 : S::输入拒绝;
    指代消解结果特征类型_ = 类型;
    指代特征服务_ = &特征;
    return S::已完成;
}

新语言分界调用结果<稳定编码> 新_语言类::同步接收指代实例(
    const 新接收认知来源键& 来源键, const 新_自然语言世界树类& 世界,
    const 新语言分界消息读取接口& 消息读取, 新_场景类& 场景服务, 新_状态类& 状态服务) {
    using S = 新语言分界工程状态;
    using L1 = 基础节点组追加状态;
    const auto 发布失败 = [](L1 状态) {
        if (状态 == L1::资源失败) return S::资源失败;
        if (状态 == L1::预期不符 || 状态 == L1::节点不存在 || 状态 == L1::引用不存在)
            return S::前提已变化;
        throw std::logic_error("reference resolution publication failed");
    };
    std::optional<稳定编码> 本次已发表;
    try {
        const auto 来源 = 读取前置来源(来源键, 消息读取);
        if (来源.状态 != S::已完成 || !来源.信息) return {来源.状态, {}, {}};
        for (;;) {
            const auto 场景结果 = 世界.读取接收语言解释场景(来源键.接收消息节点);
            const auto 状态 = 接收语言解释场景状态(场景结果);
            if (状态 != S::已完成) return {状态, {}, 本次已发表};
            const auto& 场景 = *场景结果.绑定;
            const auto 记录 = 场景.记录();
            if (!记录) return {S::未找到, {}, 本次已发表};

            组织写入现场 写;
            std::optional<std::size_t> 新宿主;
            std::optional<稳定编码> 实例, 结果特征, 目标;
            稳定编码 类型;
            新_特征类* 特征;
            {
                std::lock_guard 锁(新语言互斥);
                if (!指代特征服务_) return {S::未初始化, {}, 本次已发表};
                类型 = 指代消解结果特征类型_;
                特征 = 指代特征服务_;
                组织读取现场 读;
                const auto 当前定义 = 读.引用(*记录, 组织键::定义);
                const auto 内容 = 读接收语言解释定义(读, 当前定义, *来源.信息, 场景);
                std::map<接收指代范围, 稳定编码> 按范围;
                for (const auto P : 场景.指代实例组()) {
                    核接收指代来源(读, P, *来源.信息, 场景);
                    组织要求(按范围.emplace(读接收指代范围(读, P), P).second);
                }
                for (const auto& 句 : 内容.句段组) {
                    if (!句.主体关系 || !可保存主体指代(*句.主体关系)) continue;
                    const auto& 出现 = *句.主体关系;
                    const 接收指代范围 范围{出现.起始标量, 出现.结束标量};
                    const auto 既有 = 按范围.find(范围);
                    if (既有 == 按范围.end()) {
                        新宿主 = 写.节点(接收指代实例记录类型, 0);
                        写.引用(*新宿主, 组织键::场景, 场景.场景());
                        写.引用(*新宿主, 组织键::来源, 来源.信息->节点);
                        写.引用(*新宿主, 组织键::定义, 当前定义);
                        写.请求.新节点组[*新宿主].字段组.push_back({组键(组织键::原文),
                            基础值{std::vector<std::uint64_t>{范围.first, 范围.second}}});
                        break;
                    }
                    if (!出现.来源->已绑定存在 && 出现.来源->已证冲突候选.empty()) continue;
                    const auto P = 既有->second;
                    const auto& P快照 = 读.读(P);
                    if (读接收指代当前依据(P快照.结构) == 当前定义) continue;
                    const auto 初始定义 = 读.引用(P, 组织键::定义);
                    if (初始定义 != 当前定义) {
                        const auto 初始内容 = 读接收语言解释定义(读, 初始定义, *来源.信息, 场景);
                        const auto* 初始出现 = 查找接收主体指代(初始内容, 范围);
                        组织要求(初始出现 != nullptr);
                        if (!同一主体指代角色(*初始出现, 出现)) continue;
                    }
                    实例 = P;
                    结果特征 = 读.引用(P, 组织键::指代消解结果);
                    目标 = 出现.来源->已绑定存在;
                    // P 的完整 L1 预期同时保护 S 的入向成员引用、当前依据及 F 持有。
                    写.请求.预期联合节点组.push_back(P快照);
                    写.请求.只读预期 = 场景.绑定预期();
                    写.请求.当前字段赋值组.push_back({P, 组键(组织键::指代当前依据), 当前定义});
                    写.引用组.insert(当前定义);
                    break;
                }
                if (!新宿主 && !实例) return {S::已完成, 本次已发表, 本次已发表};
                const auto 定义键 = 组键(组织键::定义);
                写.请求.只读预期.节点组.push_back({*记录});
                写.请求.只读预期.节点组.push_back({定义键});
                基础只读字段集合预期 定义预期{*记录, 定义键, {}, {}};
                for (const auto& 字段 : 读.读(*记录).结构.字段组)
                    if (字段.字段节点 == 定义预期.字段节点) 定义预期.字段组.push_back(字段);
                写.请求.只读预期.字段集合组.push_back(std::move(定义预期));
            }
            if (新宿主) {
                组织要求(世界.合并接收指代实例挂接(场景, 写.请求, 基础组内节点引用{*新宿主}));
            }
            for (const auto 引用 : 写.引用组) 写.请求.只读预期.节点组.push_back({引用});
            if (新宿主) {
                const auto 发布 = 特征->建立新宿主及空信息引用特征(类型, std::move(写.请求),
                    基础组内节点引用{*新宿主}, 组键(组织键::指代消解结果));
                if (发布.状态 == 新特征写入状态::前提已变化) return {S::前提已变化, {}, 本次已发表};
                if (发布.状态 == 新特征写入状态::资源失败) return {S::资源失败, {}, 本次已发表};
                组织要求(发布.状态 == 新特征写入状态::已完成 && 发布.宿主节点 && 发布.特征节点);
                本次已发表 = 发布.宿主节点;
                continue;
            }
            const auto 场景绑定 = 场景服务.取得场景状态参与绑定(场景.场景());
            const auto 状态绑定 = 状态服务.取得特征状态构造绑定();
            const auto 值 = 目标 ? std::optional{新特征准确值{*目标}} : std::nullopt;
            auto 准备 = 特征->准备特征当前值组({{*结果特征, 值, false}});
            using Z = 新特征I64准备状态;
            if (准备.状态 == Z::资源失败) return {S::资源失败, {}, 本次已发表};
            if (准备.状态 == Z::预期已变化 || 准备.状态 == Z::特征不存在
                || 准备.状态 == Z::联合更新占用) return {S::前提已变化, {}, 本次已发表};
            if (准备.状态 == Z::材料类型不相容) return {S::材料未就绪, {}, 本次已发表};
            组织要求(准备.状态 == Z::已准备 && 准备.准备);
            const auto 候选 = 特征->读取准备当前值候选(*准备.准备, *结果特征);
            const auto 封存 = [&] {
                if (!候选.已改变_)
                    return 全局基础数据集.封存原子追加片段(写.请求);
                auto 材料 = 状态服务.准备特征状态(状态绑定, 候选, *实例,
                    取得分界当前UTC(), std::move(写.请求));
                return 场景服务.封存场景状态挂接(场景绑定, std::move(材料)).封存;
            }();
            if (封存.状态 != L1::已完成) {
                特征->取消特征当前值组(*准备.准备);
                return {发布失败(封存.状态), {}, 本次已发表};
            }
            组织要求(封存.片段.has_value());
            const auto 发布 = 特征->提交特征当前值组(*准备.准备, {*封存.片段});
            if (发布.状态 != L1::已完成) {
                特征->取消特征当前值组(*准备.准备);
                return {发布失败(发布.状态), {}, 本次已发表};
            }
            本次已发表 = 实例;
            (void)特征->完成特征当前值组(*准备.准备);
            特征->结束特征当前值组(*准备.准备);
        }
    } catch (L1 状态) { return {发布失败(状态), {}, 本次已发表}; }
      catch (const std::bad_alloc&) { return {S::资源失败, {}, 本次已发表}; }
      catch (const std::length_error&) { return {S::资源失败, {}, 本次已发表}; }
}

新语言分界调用结果<std::vector<新接收指代实例信息>> 新_语言类::读取接收指代实例组(
    const 新接收认知来源键& 来源键, const 新_自然语言世界树类& 世界,
    const 新语言分界消息读取接口& 消息读取) const {
    using S = 新语言分界工程状态;
    try {
        const auto 来源 = 读取前置来源(来源键, 消息读取);
        if (来源.状态 != S::已完成 || !来源.信息) return {来源.状态, {}, {}};
        const auto 场景结果 = 世界.读取接收语言解释场景(来源键.接收消息节点);
        const auto 状态 = 接收语言解释场景状态(场景结果);
        if (状态 != S::已完成) return {状态, {}, {}};
        const auto& 场景 = *场景结果.绑定;
        if (场景.指代实例组().empty()) return {S::已完成, std::vector<新接收指代实例信息>{}, {}};
        组织要求(场景.记录().has_value());

        std::vector<新接收指代实例信息> 结果;
        稳定编码 类型, 当前;
        新接收语言解释内容 当前内容;
        std::vector<基础字段内容> 历史定义;
        const 新_特征类* 特征;
        {
            std::lock_guard 锁(新语言互斥);
            if (!指代特征服务_) return {S::未初始化, {}, {}};
            特征 = 指代特征服务_;
            类型 = 指代消解结果特征类型_;
            组织读取现场 读;
            当前 = 读.引用(*场景.记录(), 组织键::定义);
            当前内容 = 读接收语言解释定义(读, 当前, *来源.信息, 场景);
            历史定义 = 读.字段(*场景.记录(), 组织键::子项);
            std::set<接收指代范围> 已保存范围;
            for (const auto P : 场景.指代实例组()) {
                核接收指代来源(读, P, *来源.信息, 场景);
                const auto 定义 = 读.引用(P, 组织键::定义);
                组织要求(std::ranges::find(历史定义, 基础字段内容{定义}) != 历史定义.end());
                const auto 范围 = 读接收指代范围(读, P);
                组织要求(已保存范围.insert(范围).second);
                const auto 保存内容 = 定义 == 当前 ? 当前内容 : 读接收语言解释定义(读, 定义, *来源.信息, 场景);
                const auto* 出现 = 查找接收主体指代(保存内容, 范围);
                组织要求(出现 != nullptr);
                const auto F = 读.引用(P, 组织键::指代消解结果);
                结果.push_back({P, 场景.场景(), 来源.信息->节点, 定义, F, {}, *出现});
            }
        }
        for (auto& 项 : 结果) {
            const auto F = 特征->读取特征直接引用当前值(项.消解结果特征, 类型, 项.节点);
            if (F.状态 == 新特征类型读取状态::资源失败) return {S::资源失败, {}, {}};
            组织要求(F.状态 == 新特征类型读取状态::已读取 && F.特征概念节点 && F.同批节点);
            项.特征类型 = *F.特征概念节点;
            项.当前目标 = F.当前引用;
            项.当前依据 = 读接收指代当前依据(*F.同批节点);
            const auto* 本轮出现 = 查找接收主体指代(当前内容, {项.出现.起始标量, 项.出现.结束标量});
            if (!本轮出现 || !同一主体指代角色(项.出现, *本轮出现)) continue;
            if (!本轮出现->来源->已绑定存在 && 本轮出现->来源->已证冲突候选.empty()) {
                项.当前适用 = !项.当前依据 && !项.当前目标 && *本轮出现 == 项.出现;
                continue;
            }
            if (!项.当前依据 || 项.当前目标 != 本轮出现->来源->已绑定存在) continue;
            if (项.当前依据 == 当前) {
                项.当前适用 = true;
                continue;
            }
            if (std::ranges::find(历史定义, 基础字段内容{*项.当前依据}) == 历史定义.end()) continue;
            std::lock_guard 锁(新语言互斥);
            组织读取现场 读;
            const auto 采用内容 = 读接收语言解释定义(读, *项.当前依据, *来源.信息, 场景);
            const auto* 采用出现 = 查找接收主体指代(采用内容, {项.出现.起始标量, 项.出现.结束标量});
            项.当前适用 = 采用出现 && *采用出现 == *本轮出现;
        }
        return {S::已完成, std::move(结果), {}};
    } catch (const std::bad_alloc&) { return {S::资源失败, {}, {}}; }
      catch (const std::length_error&) { return {S::资源失败, {}, {}}; }
}

namespace {
bool 明确回复传输一致(const 新语言明确回复事实& 事实) {
    const auto& M=事实.回复;
    const auto& O=事实.输出;
    const auto& Send=事实.实际发送;
    using 方向=新外设消息方向;
    if (!有效(事实.说话者) || !有效(M.消息节点) || !有效(O.消息节点) || !有效(Send.消息节点)
        || M.来源.方向!=方向::接收 || O.来源.方向!=方向::输出 || Send.来源.方向!=方向::发送
        || M.回应消息节点!=O.消息节点 || Send.对应输出消息节点!=O.消息节点
        || !O.关联问题 || !有效(*O.关联问题) || M.正式记录时间.纳秒<=Send.正式记录时间.纳秒)
        return false;
    auto 发送键=O.来源;
    发送键.方向=方向::发送;
    const auto 同一对话框=[&](const 新外设消息信息& 消息) {
        return 消息.场景节点==O.场景节点 && 消息.窗口节点==O.窗口节点
            && 消息.来源.公开宿主会话==O.来源.公开宿主会话 && 消息.来源.对话框标识==O.来源.对话框标识;
    };
    return Send.来源==发送键 && Send.UTF8正文==O.UTF8正文 && 同一对话框(M) && 同一对话框(Send);
}
} // namespace

新接收场景信息保存结果 新_语言类::建立或取得接收场景信息(const 新接收场景信息& v){
    if(v.回复传输 || v.种类==新接收场景信息种类::明确整数短答)
        return {新接收场景信息保存状态::输入拒绝,{}};
    return 保存接收场景信息(v,{});
}

新接收场景信息保存结果 新_语言类::建立或取得明确整数短答(稳定编码 来源场景,
    std::int64_t 原文标量数,const 语言图整数值& 数值,const 新语言明确回复事实& 事实) {
    using S=新接收场景信息保存状态;
    try {
        if(!明确回复传输一致(事实) || !有效(来源场景) || 原文标量数<=0
            || 数值.规范十进制.empty() || !数值.单位 || !有效(*数值.单位)) return {S::输入拒绝,{}};
        std::vector<基础联合节点快照> 传输保护;
        for (const auto id : {事实.回复.消息节点,事实.输出.消息节点,事实.实际发送.消息节点}) {
            const auto 项=std::ranges::find_if(事实.正式消息保护,[&](const auto& 快照) {
                return 快照.结构.节点.编码==id;
            });
            if(项==事实.正式消息保护.end()) return {S::输入拒绝,{}};
            传输保护.push_back(*项);
        }
        新接收场景信息 值;
        值.种类=新接收场景信息种类::明确整数短答;
        值.来源消息=事实.回复.消息节点;值.来源场景=来源场景;
        值.交互场景=事实.回复.场景节点;值.说话者=事实.说话者;
        值.值种类=新值陈述值种类::准确整数;值.准确整数=数值.规范十进制;值.单位=数值.单位;
        值.原文终点=原文标量数;值.时刻=事实.回复.正式记录时间.纳秒;
        值.回复传输=新明确回复传输定位{*事实.输出.关联问题,事实.输出.消息节点,事实.实际发送.消息节点};
        return 保存接收场景信息(值,传输保护);
    } catch (const std::bad_alloc&) {return {S::资源失败,{}};}
      catch (const std::length_error&) {return {S::资源失败,{}};}
}

新接收场景信息保存结果 新_语言类::保存接收场景信息(const 新接收场景信息& v,
    std::span<const 基础联合节点快照> 传输保护) {
    using S=新接收场景信息保存状态;
    核接收场景信息(v);
    新接收场景信息保存结果 out;std::vector<稳定编码> 通知组;
    {
        std::unique_lock lock(新语言互斥);组织读取现场 r;
        for(const auto& x:读全部场景信息(r))if(x.内容.来源消息==v.来源消息){
            const auto 状态=x.内容==v ? S::精确重复 : S::来源冲突;
            if(状态==S::精确重复&&v.回复传输){
                lock.unlock();
                发出问题答案就绪通知({&v.回复传输->关联问题,1});
            }
            return {状态,x.陈述节点};
        }
        组织写入现场 w;const auto root=w.节点(102,0),def=w.写(v);
        w.引用(root,组织键::场景,v.来源场景);w.引用(root,组织键::来源,v.来源消息);w.子(root,组织键::定义,def);
        w.请求.追加字段组.push_back({组键(组织键::登记根),组键(组织键::场景信息登记),基础组内节点引用{root}});
        w.保护(r);
        for(const auto& 项 : 传输保护) w.请求.预期既有节点组.push_back(项.结构);
        const auto p=全局基础数据集.原子追加节点组(w.请求);
        if(p.状态==基础节点组追加状态::预期不符)return {S::前提已变化,{}};
        if(!传输保护.empty()){
            if(p.状态==基础节点组追加状态::资源失败)return {S::资源失败,{}};
            if(p.状态==基础节点组追加状态::节点不存在 || p.状态==基础节点组追加状态::引用不存在)
                return {S::前提已变化,{}};
        }
        组织要求(p.状态==基础节点组追加状态::已完成&&p.新节点组.size()==w.请求.新节点组.size());
        组织读取现场 back;
        for(const auto& x:读全部场景信息(back))if(x.内容.来源消息==v.来源消息){
            组织要求(x.陈述节点==p.新节点组[root]&&x.内容==v);out={S::已保存,x.陈述节点};
        }
        组织要求(out.状态==S::已保存);
        if(!v.回复传输)通知组=待通知问题(back,v);
    }
    if(v.回复传输)发出问题答案就绪通知({&v.回复传输->关联问题,1});
    else 发出问题答案就绪通知(通知组);
    return out;
}
std::optional<新值陈述信息> 新_语言类::按来源消息读取接收场景信息(稳定编码 来源消息) const {
    组织要求(有效(来源消息));
    std::lock_guard lock(新语言互斥);组织读取现场 r;
    for(auto& x:读全部场景信息(r))if(x.内容.来源消息==来源消息)return std::move(x);
    return std::nullopt;
}
std::vector<新值陈述信息> 新_语言类::查询值陈述(稳定编码 交互场景,稳定编码 说话者,std::int64_t 起始时刻) const {
    组织要求(有效(交互场景)&&有效(说话者));
    std::lock_guard lock(新语言互斥);组织读取现场 r;std::vector<新值陈述信息> out;
    for(auto& x:读全部场景信息(r))
        if(x.内容.种类==新接收场景信息种类::值陈述&&x.内容.交互场景==交互场景
            &&x.内容.说话者==说话者&&x.内容.时刻>起始时刻)out.push_back(std::move(x));
    return out;
}
新自我写入信息保存结果 新_语言类::建立或取得自我写入信息(const 新自我写入语言信息& v){
    using S=新接收场景信息保存状态;
    核自我写入信息(v);
    新自我写入信息保存结果 out;std::vector<稳定编码> 通知组;
    {
        std::lock_guard lock(新语言互斥);组织读取现场 r;
        for(const auto& x:读全部自我写入(r))if(x.内容.写入来源==v.写入来源)
            return {x.内容==v ? S::精确重复 : S::来源冲突,x.节点};
        组织写入现场 w;const auto root=w.节点(105,0),def=w.写(v);
        w.引用(root,组织键::场景,v.来源场景);w.引用(root,组织键::来源,v.写入来源);w.子(root,组织键::定义,def);
        w.请求.追加字段组.push_back({组键(组织键::登记根),组键(组织键::自我写入登记),基础组内节点引用{root}});
        w.保护(r);const auto p=全局基础数据集.原子追加节点组(w.请求);
        if(p.状态==基础节点组追加状态::预期不符)return {S::前提已变化,{}};
        组织要求(p.状态==基础节点组追加状态::已完成&&p.新节点组.size()==w.请求.新节点组.size());
        组织读取现场 back;
        for(const auto& x:读全部自我写入(back))if(x.内容.写入来源==v.写入来源){
            组织要求(x.节点==p.新节点组[root]&&x.内容==v);out={S::已保存,x.节点};
        }
        组织要求(out.状态==S::已保存);
        通知组=待通知问题(back,v.主体存在,v.特征类型,v.时刻);
    }
    发出问题答案就绪通知(通知组);
    return out;
}
新语言问题建立结果 新_语言类::建立或取得出站问题(const 新语言问题定义& v){
    using S=新语言问题建立状态;
    核问题定义(v);
    std::lock_guard lock(新语言互斥);组织读取现场 r;
    for(const auto& q:读全部问题(r))if(q.定义.出站来源==v.出站来源)
        return {q.定义==v ? S::已存在 : S::来源冲突,q.问题节点};
    // 问题只随已留档的出站问句建立：出站来源场景须有同来源的出站具体表达。
    const auto 出站=读出站(r,v.出站来源场景);
    if(!出站||出站->场景!=v.出站来源场景||出站->来源!=v.出站来源)return {S::出站留档不符,{}};
    if(v.分界目标 && 出站->正式被询定位!=std::optional{新出站正式被询定位{
            v.分界目标->范围,v.分界目标->特征类型}})return {S::出站留档不符,{}};
    组织写入现场 w;const auto root=w.节点(103,0),def=w.写(v);
    w.引用(root,组织键::场景,v.出站来源场景);w.引用(root,组织键::来源,v.出站来源);w.子(root,组织键::定义,def);
    w.请求.追加字段组.push_back({组键(组织键::登记根),组键(组织键::问题登记),基础组内节点引用{root}});
    w.保护(r);const auto p=全局基础数据集.原子追加节点组(w.请求);
    if(p.状态==基础节点组追加状态::预期不符)return {S::前提已变化,{}};
    组织要求(p.状态==基础节点组追加状态::已完成&&p.新节点组.size()==w.请求.新节点组.size());
    组织读取现场 back;
    for(const auto& q:读全部问题(back))if(q.定义.出站来源==v.出站来源){
        组织要求(q.问题节点==p.新节点组[root]&&q.定义==v&&!q.答案);return {S::已建立,q.问题节点};
    }
    组织要求(false);return {};
}
std::optional<新语言问题信息> 新_语言类::读取问题(稳定编码 问题节点) const {
    std::lock_guard lock(新语言互斥);组织读取现场 r;
    for(auto& q:读全部问题(r))if(q.问题节点==问题节点)return std::move(q);
    return std::nullopt;
}
std::optional<新语言问题信息> 新_语言类::按出站来源读取问题(稳定编码 出站来源) const {
    组织要求(有效(出站来源));
    std::lock_guard lock(新语言互斥);组织读取现场 r;
    for(auto& q:读全部问题(r))if(q.定义.出站来源==出站来源)return std::move(q);
    return std::nullopt;
}
std::vector<新语言问题信息> 新_语言类::查询同结构问题(稳定编码 被认知需求,稳定编码 主体,稳定编码 被问栏) const {
    组织要求(有效(被认知需求)&&有效(主体)&&有效(被问栏));
    std::lock_guard lock(新语言互斥);组织读取现场 r;std::vector<新语言问题信息> out;
    for(auto& q:读全部问题(r))
        if(q.定义.被问种类!=新问题被问种类::原文分界&&q.定义.被认知需求==被认知需求&&q.定义.主体==主体&&q.定义.被问栏==被问栏)out.push_back(std::move(q));
    return out;
}
新问题答案查询结果 新_语言类::查找问题答案(稳定编码 问题节点) const {
    std::lock_guard lock(新语言互斥);组织读取现场 r;
    for(const auto& q:读全部问题(r))if(q.问题节点==问题节点)return 查答案已加锁(r,q);
    throw std::logic_error("问题节点不在语言owner问题登记内");
}
新问题答案登记结果 新_语言类::登记问题答案(稳定编码 问题节点,稳定编码 答案信息节点,新问题答案来源 来源种类){
    using S=新问题答案登记状态;
    组织要求(有效(问题节点)&&有效(答案信息节点)
        &&(来源种类==新问题答案来源::消息转换||来源种类==新问题答案来源::自我写入));
    const 新问题答案引用 want{答案信息节点,来源种类};
    std::lock_guard lock(新语言互斥);组织读取现场 r;
    std::optional<新语言问题信息> q;
    for(auto& x:读全部问题(r))if(x.问题节点==问题节点)q=std::move(x);
    if(!q)throw std::logic_error("问题节点不在语言owner问题登记内");
    if(q->答案)return {*q->答案==want ? S::精确重复 : S::已有其它答案,q->答案};
    const auto found=查答案已加锁(r,*q);
    if(std::find(found.答案组.begin(),found.答案组.end(),want)==found.答案组.end())return {S::不是该问题的答案,std::nullopt};
    组织写入现场 w;w.引用组.insert(答案信息节点);
    w.请求.追加字段组.push_back({问题节点,组键(组织键::答案),答案信息节点});
    w.请求.追加字段组.push_back({问题节点,组键(组织键::答案来源),基础值{static_cast<std::int64_t>(来源种类)}});
    w.保护(r);const auto p=全局基础数据集.原子追加节点组(w.请求);
    if(p.状态==基础节点组追加状态::预期不符)return {S::前提已变化,std::nullopt};
    组织要求(p.状态==基础节点组追加状态::已完成&&p.新节点组.empty());
    组织读取现场 back;
    for(const auto& x:读全部问题(back))if(x.问题节点==问题节点){
        组织要求(x.答案==std::optional{want});return {S::已登记,want};
    }
    组织要求(false);return {};
}
std::optional<新问题答案信息> 新_语言类::读取问题答案信息(稳定编码 信息节点) const {
    组织要求(有效(信息节点));
    std::lock_guard lock(新语言互斥);组织读取现场 r;
    for(const auto& x:读全部场景信息(r))if(x.陈述节点==信息节点){
        if(x.内容.种类==新接收场景信息种类::明确整数短答){
            for(const auto& q:读全部问题(r))
                if(明确整数短答与问题一致(q,x.内容))
                    return 新问题答案信息{信息节点,新问题答案来源::消息转换,q.定义.主体,q.定义.被问栏,
                        新值陈述值种类::准确整数,x.内容.准确整数,{},{},x.内容.单位,{},{}};
            return std::nullopt;
        }
        if(x.内容.种类==新接收场景信息种类::需求信息){
            const auto& v=x.内容;新问题答案信息 out{信息节点,新问题答案来源::消息转换,v.主体存在,v.特征类型.value_or(稳定编码{})};
            if(v.值种类)out.值种类=*v.值种类;
            out.准确整数=v.准确整数;out.区间下界=v.区间下界;out.区间上界=v.区间上界;out.单位=v.单位;out.特征概念=v.特征概念;out.方向=v.方向;
            out.需求信息=true;out.目标存在=v.目标存在;return out;
        }
        if(!是值陈述答案(x.内容))return std::nullopt;
        const auto& v=x.内容;
        return 新问题答案信息{信息节点,新问题答案来源::消息转换,v.主体存在,*v.特征类型,*v.值种类,
            v.准确整数,v.区间下界,v.区间上界,v.单位,v.特征概念,v.方向};
    }
    for(const auto& x:读全部自我写入(r))if(x.节点==信息节点){
        const auto& v=x.内容;
        return 新问题答案信息{信息节点,新问题答案来源::自我写入,v.主体存在,v.特征类型,v.值种类,
            v.准确整数,v.区间下界,v.区间上界,v.单位,v.特征概念,v.方向};
    }
    return std::nullopt;
}
bool 新_语言类::绑定问题答案就绪通知(const void* 所有者,std::function<void(稳定编码)> 通知) noexcept {
    try {
        if(!所有者||!通知)return false;
        std::lock_guard lock(问题答案通知互斥);
        if(问题答案通知所有者)return 问题答案通知所有者==所有者;
        问题答案通知=std::move(通知);问题答案通知所有者=所有者;return true;
    } catch(...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"绑定问题答案就绪通知",L"端口绑定发生工程故障。");
    }
}
void 新_语言类::解除问题答案就绪通知(const void* 所有者) noexcept {
    try {
        std::lock_guard lock(问题答案通知互斥);
        if(!所有者||问题答案通知所有者!=所有者)return;
        问题答案通知={};问题答案通知所有者=nullptr;
    } catch(...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"解除问题答案就绪通知",L"端口解除发生工程故障。");
    }
}
std::optional<新出站具体表达信息> 新_语言类::建立或取得出站具体表达(稳定编码 scene,const 新语言含义网& meaning,
    稳定编码 knowledge,const std::string& text,稳定编码 source,
    std::optional<新出站正式被询定位> 被询){
    组织要求(!被询||(被询->信息节点==source&&有效(被询->特征类型)));
    核验含义网结构(meaning);组织要求(有效(scene)&&有效(source)&&有效(knowledge)&&!text.empty());
    std::lock_guard lock(新语言互斥);组织读取现场 r;
    const auto prior=读出站(r,scene);
    if(prior){组织要求(prior->场景==scene&&prior->来源==source&&prior->所用组织知识==knowledge&&prior->含义网==meaning&&prior->候选话语原文==text&&prior->正式被询定位==被询);return prior;}
    (void)读组织知识(r,knowledge);
    组织写入现场 w;const auto root=w.节点(101,0),graph=w.写(meaning);
    w.引用(root,组织键::场景,scene);w.引用(root,组织键::来源,source);w.引用(root,组织键::组织知识,knowledge);
    w.子(root,组织键::含义网,graph);w.请求.新节点组[root].字段组.push_back({组键(组织键::原文),基础值{text}});
    if(被询)w.子(root,组织键::出站被询定位,w.写(*被询));
    w.请求.追加字段组.push_back({组键(组织键::登记根),组键(组织键::出站登记),基础组内节点引用{root}});
    w.保护(r);const auto p=全局基础数据集.原子追加节点组(w.请求);
    if(p.状态==基础节点组追加状态::预期不符)return std::nullopt;
    组织要求(p.状态==基础节点组追加状态::已完成&&p.新节点组.size()==w.请求.新节点组.size());
    组织读取现场 back;const auto out=读出站(back,scene);
    组织要求(out&&out->来源==source&&out->所用组织知识==knowledge&&out->含义网==meaning&&out->候选话语原文==text&&out->正式被询定位==被询);return out;
}

namespace {
新词典归类材料信息 读取词典语言行(组织读取现场& 读, 稳定编码 行根) {
    组织要求(读.整数(行根, 组织键::类型) == 106);
    std::set<稳定编码> 路径;
    新词典归类材料信息 结果{行根,
        组织读值<新词典归类材料定义>(读, 读.引用(行根, 组织键::定义), 路径), {}};
    const auto 解读 = 读.字段(行根, 组织键::词典归类解读);
    if (!解读.empty()) {
        组织要求(解读.size() == 1);
        结果.概念解读 = 组织读值<新词典归类概念解读>(读, std::get<稳定编码>(解读.front()), 路径);
    }
    return 结果;
}

bool 是正式词典语言行(稳定编码 R) {
    if (!有效(R) || !有效(组键(组织键::登记根))) return false;
    const auto 登记 = 全局基础数据集.查询目标字段(R, 组键(组织键::词典材料登记));
    return std::any_of(登记.begin(), 登记.end(), [](const auto& 字段) {
        return 字段.所属节点 == 组键(组织键::登记根);
    });
}

std::vector<新词典归类材料信息> 查询词典语言行(组织读取现场& 读, 稳定编码 Q) {
    std::vector<新词典归类材料信息> 结果;
    for (const auto& 字段 : 读.字段(组键(组织键::登记根), 组织键::词典材料登记)) {
        const auto 行根 = std::get<稳定编码>(字段);
        if (读.引用(行根, 组织键::来源) == Q)
            结果.push_back(读取词典语言行(读, 行根));
    }
    return 结果;
}

bool 补充词典语言引用身份(组织写入现场& 写) {
    const auto 结果 = 全局基础数据集.查询节点组完整结构({写.引用组.begin(), 写.引用组.end()});
    if (结果.状态 == 基础结构操作状态::资源失败) throw std::bad_alloc{};
    if (结果.状态 == 基础结构操作状态::节点不存在) return false;
    if (结果.状态 != 基础结构操作状态::已完成)
        throw std::logic_error("词典语言材料的引用身份读取失败");
    for (const auto& 项 : 结果.节点组) {
        if (!项.快照) return false;
        写.请求.只读预期.节点组.push_back(项.快照->结构.节点);
    }
    return true;
}

void 替换词典语言私有定义(组织读取现场& 读, 组织写入现场& 写,
    稳定编码 行根, std::size_t 新定义, 组织键 字段键 = 组织键::定义) {
    const auto 旧字段 = 读.字段(行根, 字段键);
    组织要求(旧字段.size() <= 1);
    std::set<稳定编码> 私有节点;
    std::vector<稳定编码> 待遍历;
    if (!旧字段.empty()) 待遍历.push_back(std::get<稳定编码>(旧字段.front()));
    while (!待遍历.empty()) {
        const auto 节点 = 待遍历.back();
        待遍历.pop_back();
        if (!私有节点.insert(节点).second) continue;
        for (const auto& 子项 : 读.字段(节点, 组织键::子项))
            待遍历.push_back(std::get<稳定编码>(子项));
    }
    std::set<稳定编码> 联合节点{行根};
    for (const auto 节点 : 私有节点) {
        联合节点.insert(节点);
        写.请求.删除节点组.push_back(节点);
        for (const auto& 字段 : 读.读(节点).结构.字段组) {
            写.请求.删除字段关系组.push_back(字段.编码);
            if (const auto* 目标 = std::get_if<稳定编码>(&字段.内容)) 联合节点.insert(*目标);
        }
    }
    for (const auto& 字段 : 读.读(行根).结构.字段组)
        if (字段.字段节点 == 组键(字段键)) 写.请求.删除字段关系组.push_back(字段.编码);
    for (const auto 节点 : 联合节点) {
        const auto& 已有 = 写.请求.预期联合节点组;
        if (std::none_of(已有.begin(), 已有.end(), [&](const auto& 项) {
            return 项.结构.节点.编码 == 节点;
        })) 写.请求.预期联合节点组.push_back(读.读(节点));
    }
    写.请求.追加字段组.push_back({行根, 组键(字段键), 基础组内节点引用{新定义}});
}
} // namespace

namespace {
constexpr std::int64_t 可信词头来源组织类型 = 115;

auto 可信词头来源键(const 新可信词头来源定义& 定义) {
    return std::tie(定义.语言节点, 定义.规范化合同, 定义.批次, 定义.原包, 定义.语种, 定义.词面);
}

bool 可信词头来源有效(const 新可信词头来源定义& 定义) {
    const auto 有效文本 = [](const std::string& 文本) {
        return !文本.empty() && 文本.find('\0') == std::string::npos && 新_语言类::严格UTF8字节串有效(文本);
    };
    return 有效(定义.语言节点) && 定义.规范化合同 == 严格UTF8字节精确规范化合同节点
        && 有效(定义.规范化合同) && 有效文本(定义.批次) && 有效文本(定义.原包)
        && 有效文本(定义.语种) && 有效文本(定义.词面) && !定义.记录键组.empty()
        && std::ranges::all_of(定义.记录键组, 有效文本);
}

std::optional<新可信词头来源信息> 解码可信词头来源(const 基础节点结构& 结构) {
    const auto 单字段 = [&](组织键 键) -> const 基础字段关系* {
        const auto id = 组键(键);
        const auto 项 = std::ranges::find(结构.字段组, id, &基础字段关系::字段节点);
        if (项 == 结构.字段组.end()) return nullptr;
        组织要求(std::count_if(结构.字段组.begin(), 结构.字段组.end(),
            [id](const auto& f) { return f.字段节点 == id; }) == 1);
        return &*项;
    };
    const auto 类型 = 单字段(组织键::类型);
    const auto 值 = 类型 ? std::get_if<基础值>(&类型->内容) : nullptr;
    const auto 编号 = 值 ? std::get_if<std::int64_t>(&值->材料) : nullptr;
    if (!编号 || *编号 != 可信词头来源组织类型) return std::nullopt;
    const auto 引用 = [&](组织键 键) {
        const auto f = 单字段(键);
        组织要求(f != nullptr);
        return std::get<稳定编码>(f->内容);
    };
    const auto 文本 = [&](组织键 键) {
        const auto f = 单字段(键);
        组织要求(f != nullptr);
        return std::get<std::string>(std::get<基础值>(f->内容).材料);
    };
    新可信词头来源定义 定义{引用(组织键::词头来源语言), 引用(组织键::词头规范化),
        文本(组织键::词头批次), 文本(组织键::词头原包), 文本(组织键::词头语种), 文本(组织键::原文), {}};
    for (const auto& 字段 : 结构.字段组)
        if (字段.字段节点 == 组键(组织键::词头记录键))
            定义.记录键组.push_back(std::get<std::string>(std::get<基础值>(字段.内容).材料));
    std::ranges::sort(定义.记录键组);
    组织要求(可信词头来源有效(定义) && 结构.字段组.size() == 7 + 定义.记录键组.size()
        && std::ranges::adjacent_find(定义.记录键组) == 定义.记录键组.end());
    return 新可信词头来源信息{结构.节点.编码, std::move(定义)};
}
} // namespace

新语言分界调用结果<新可信词头来源信息> 新_语言类::读取可信词头来源(稳定编码 节点) const {
    using S = 新语言分界工程状态;
    using L1 = 基础结构操作状态;
    if (!有效(节点)) return {};
    try {
        if (!有效(组键(组织键::词头来源语言))) return {S::未初始化};
        const auto 读取 = 全局基础数据集.查询节点组完整结构({节点});
        if (读取.状态 == L1::资源失败) return {S::资源失败};
        if (读取.状态 == L1::节点不存在) return {S::未找到};
        组织要求(读取.状态 == L1::已完成 && 读取.节点组.size() == 1);
        const auto& 项 = 读取.节点组.front();
        auto 结果 = 项.快照 ? 解码可信词头来源(项.快照->结构) : std::nullopt;
        return 结果 ? 新语言分界调用结果<新可信词头来源信息>{S::已完成, std::move(结果)}
                    : 新语言分界调用结果<新可信词头来源信息>{S::未找到};
    } catch (const std::bad_alloc&) { return {S::资源失败}; }
      catch (const std::length_error&) { return {S::资源失败}; }
}

新语言分界调用结果<新可信词头来源信息> 新_语言类::建立或取得可信词头来源(新可信词头来源定义 定义) {
    using S = 新语言分界工程状态;
    using L1 = 基础结构操作状态;
    try {
        std::lock_guard 锁(新语言互斥);
        if (!有效(组键(组织键::词头来源语言))) return {S::未初始化};
        if (!可信词头来源有效(定义)) return {S::输入拒绝};
        std::ranges::sort(定义.记录键组);
        定义.记录键组.erase(std::unique(定义.记录键组.begin(), 定义.记录键组.end()), 定义.记录键组.end());
        const auto 全组 = 全局基础数据集.查询字段引用节点结构(定义.语言节点, 组键(组织键::词头来源语言));
        if (全组.状态 == L1::资源失败) return {S::资源失败};
        if (全组.状态 == L1::节点不存在 || 全组.状态 == L1::引用不存在) return {S::前提已变化};
        组织要求(全组.状态 == L1::已完成);
        std::optional<新可信词头来源信息> 已有;
        for (const auto& 结构 : 全组.来源节点组) {
            auto 候选 = 解码可信词头来源(结构);
            组织要求(候选.has_value());
            if (可信词头来源键(候选->定义) != 可信词头来源键(定义)) continue;
            组织要求(!已有);
            已有 = std::move(候选);
        }
        if (已有) {
            if (已有->定义.记录键组 != 定义.记录键组) return {S::输入拒绝};
            return {S::已完成, std::move(已有)};
        }

        基础组内新节点 新来源{{
            {组键(组织键::类型), 基础值{可信词头来源组织类型}},
            {组键(组织键::词头来源语言), 定义.语言节点},
            {组键(组织键::词头规范化), 定义.规范化合同},
            {组键(组织键::词头批次), 基础值{定义.批次}},
            {组键(组织键::词头原包), 基础值{定义.原包}},
            {组键(组织键::词头语种), 基础值{定义.语种}},
            {组键(组织键::原文), 基础值{定义.词面}},
        }};
        for (const auto& 键 : 定义.记录键组)
            新来源.字段组.push_back({组键(组织键::词头记录键), 基础值{键}});
        const auto 发表 = 全局基础数据集.原子建立节点组({std::move(新来源)});
        if (发表.状态 == L1::资源失败) return {S::资源失败};
        if (发表.状态 == L1::节点不存在 || 发表.状态 == L1::引用不存在) return {S::前提已变化};
        组织要求(发表.状态 == L1::已完成 && 发表.节点组.size() == 1);
        auto 回执 = 读取可信词头来源(发表.节点组.front());
        回执.本次已发表身份 = 发表.节点组.front();
        if (回执.状态 == S::未找到) 回执.状态 = S::前提已变化;
        if (回执.状态 == S::已完成) 组织要求(回执.信息 && 回执.信息->定义 == 定义);
        return 回执;
    } catch (const std::bad_alloc&) { return {S::资源失败}; }
      catch (const std::length_error&) { return {S::资源失败}; }
}

std::optional<新词典归类材料信息> 新_语言类::保存词典归类材料(const 新词典归类材料定义& 定义) {
    for (;;) {
        {
            std::lock_guard 锁(新语言互斥);
            组织读取现场 读;
            const auto 全部 = 查询词典语言行(读, 定义.材料Q);
            const auto 旧 = std::find_if(全部.begin(), 全部.end(), [&](const auto& 项) {
                return 项.定义.义项I == 定义.义项I
                    && 项.定义.原文起始标量 == 定义.原文起始标量
                    && 项.定义.原文结束标量 == 定义.原文结束标量;
            });
            if (旧 != 全部.end() && 旧->定义 == 定义) return *旧;

            组织写入现场 写;
            const auto 新定义 = 写.写(定义);
            std::optional<std::size_t> 新行根;
            if (旧 == 全部.end()) {
                新行根 = 写.节点(106, 0);
                写.引用(*新行根, 组织键::来源, 定义.材料Q);
                写.引用(*新行根, 组织键::场景, 定义.来源场景);
                写.子(*新行根, 组织键::定义, 新定义);
                const auto 登记根 = 组键(组织键::登记根);
                写.请求.预期既有节点组.push_back(读.读(登记根).结构);
                写.请求.追加字段组.push_back({登记根, 组键(组织键::词典材料登记),
                    基础组内节点引用{*新行根}});
            } else {
                替换词典语言私有定义(读, 写, 旧->节点, 新定义);
                const auto 新解读 = 写.写(新词典归类概念解读{});
                替换词典语言私有定义(读, 写, 旧->节点, 新解读, 组织键::词典归类解读);
            }
            新词典归类材料信息 结果{{}, 定义, {}};
            if (!补充词典语言引用身份(写)) return std::nullopt;
            const auto 发布 = 全局基础数据集.原子追加节点组(写.请求);
            if (发布.状态 == 基础节点组追加状态::已完成) {
                const auto 行根 = 新行根 ? 发布.新节点组.at(*新行根) : 旧->节点;
                结果.节点 = 行根;
                return 结果;
            }
            if (发布.状态 == 基础节点组追加状态::资源失败) throw std::bad_alloc{};
            if (发布.状态 != 基础节点组追加状态::预期不符
                && 发布.状态 != 基础节点组追加状态::节点不存在
                && 发布.状态 != 基础节点组追加状态::引用不存在)
                throw std::logic_error("词典语言材料的组合发布失败");
        }
        std::this_thread::yield();
    }
}

namespace {
稳定编码 读取词典组织项(组织读取现场& 读, 稳定编码 记录, std::int64_t 序号) {
    for (const auto& 子项 : 读.字段(记录,组织键::子项)) {
        const auto 节点 = std::get<稳定编码>(子项);
        // 只用兄弟节点的直接序号定位，正文及引用对象不进入参数取材。
        const auto 字段 = 全局基础数据集.查询字段(节点,组键(组织键::序号));
        组织要求(字段.size() == 1);
        const auto 当前序号 = std::get<std::int64_t>(std::get<基础值>(字段.front().内容).材料);
        if (当前序号 == 序号) return 节点;
    }
    throw std::logic_error("dictionary claim formal field path missing");
}
} // namespace

新词典归类参数批次结果 新_语言类::解码词典归类参数批次(
    稳定编码 R,const 新词典归类信息概念组& 概念组,
    std::span<const 基础联合节点读取项> 批次) const {
    using 状态=新词典归类参数批次状态;
    struct 待补 final { std::vector<稳定编码> 节点; };
    struct 不可解码 final {};
    std::lock_guard 锁(新语言互斥);
    for(const auto key:{组键(组织键::登记根),组键(组织键::类型),组键(组织键::序号),组键(组织键::值),
        组键(组织键::引用),组键(组织键::子项),组键(组织键::定义),组键(组织键::词典材料登记),
        组键(组织键::词典归类解读),概念组.材料范围,概念组.目标词条FT,概念组.采用概念FT})
        if(!有效(key))return {状态::未初始化};
    if(!有效(R))return {状态::信息不存在};
    const auto find=[&](稳定编码 id)->const 基础联合节点读取项* {
        const auto it=std::find_if(批次.begin(),批次.end(),[&](const auto& item){return item.查询节点==id;});
        return it==批次.end() ? nullptr : &*it;
    };
    const auto root=find(R);
    if(!root)return {状态::需补快照,{}, {R}};
    if(!root->快照)return {状态::信息不存在};
    if(root->快照->结构.节点.编码!=R)return {状态::结构不可解码};
    const auto registered=std::any_of(root->快照->入向字段组.begin(),root->快照->入向字段组.end(),[&](const auto& field) {
        const auto* ref=std::get_if<稳定编码>(&field.内容);
        return field.所属节点==组键(组织键::登记根) && field.字段节点==组键(组织键::词典材料登记)
            && ref && *ref==R;
    });
    if(!registered)return {状态::信息不存在};
    try {
        const auto read=[&](稳定编码 id)->const 基础联合节点快照& {
            const auto item=find(id);
            if(!item)throw 待补{{id}};
            if(!item->快照 || item->快照->结构.节点.编码!=id)throw 不可解码{};
            return *item->快照;
        };
        const auto fields=[](const 基础联合节点快照& snapshot,组织键 key) {
            std::vector<const 基础字段关系*> out;
            for(const auto& field:snapshot.结构.字段组)if(field.字段节点==组键(key))out.push_back(&field);
            return out;
        };
        const auto single=[&](const 基础联合节点快照& snapshot,组织键 key)->const 基础字段关系& {
            const auto list=fields(snapshot,key);if(list.size()!=1)throw 不可解码{};return *list.front();
        };
        const auto number=[&](const 基础联合节点快照& snapshot,组织键 key) {
            const auto* raw=std::get_if<基础值>(&single(snapshot,key).内容);
            const auto* value=raw ? std::get_if<std::int64_t>(&raw->材料) : nullptr;
            if(!value)throw 不可解码{};return *value;
        };
        const auto reference=[&](const 基础字段关系& field) {
            const auto* value=std::get_if<稳定编码>(&field.内容);
            if(!value || !有效(*value))throw 不可解码{};return *value;
        };
        const auto recordItem=[&](稳定编码 id,std::int64_t wanted) {
            const auto& record=read(id);
            if(number(record,组织键::类型)!=5)throw 不可解码{};
            std::set<稳定编码> missing;
            std::optional<稳定编码> found;
            for(const auto* field:fields(record,组织键::子项)) {
                const auto child=reference(*field);
                const auto item=find(child);
                if(!item){missing.insert(child);continue;}
                if(!item->快照 || item->快照->结构.节点.编码!=child)throw 不可解码{};
                if(number(*item->快照,组织键::序号)==wanted) {
                    if(found)throw 不可解码{};found=child;
                }
            }
            if(!missing.empty())throw 待补{{missing.begin(),missing.end()}};
            if(!found)throw 不可解码{};return *found;
        };
        const auto directRef=[&](稳定编码 id) {
            const auto& node=read(id);
            if(number(node,组织键::类型)!=4 || !fields(node,组织键::子项).empty())throw 不可解码{};
            return reference(single(node,组织键::引用));
        };
        if(number(*root->快照,组织键::类型)!=106)return {状态::信息不存在};
        const auto definition=reference(single(*root->快照,组织键::定义));
        const auto W=directRef(recordItem(definition,5));
        std::optional<稳定编码> C;
        const auto interpretations=fields(*root->快照,组织键::词典归类解读);
        if(interpretations.size()>1)throw 不可解码{};
        if(!interpretations.empty()) {
            const auto interpretation=reference(*interpretations.front());
            const auto statusId=recordItem(interpretation,0);
            const auto& statusNode=read(statusId);
            if(number(statusNode,组织键::类型)!=2 || !fields(statusNode,组织键::子项).empty())throw 不可解码{};
            const auto v=number(statusNode,组织键::值);
            if(v<1 || v>7)throw 不可解码{};
            if(v==static_cast<std::int64_t>(新词典归类概念状态::已明确)) {
                const auto optionalId=recordItem(interpretation,3);
                const auto& optionalNode=read(optionalId);
                if(number(optionalNode,组织键::类型)!=7)throw 不可解码{};
                const auto children=fields(optionalNode,组织键::子项);
                if(children.size()>1)throw 不可解码{};
                if(!children.empty()) {
                    const auto child=reference(*children.front());
                    if(number(read(child),组织键::序号)!=0)throw 不可解码{};
                    C=directRef(child);
                }
            }
        }
        (void)read(W);if(C)(void)read(*C);
        for(const auto id:{概念组.材料范围,概念组.目标词条FT,概念组.采用概念FT})(void)read(id);
        正式内容值 adopted{正式内容值种类::概念引用,正式内容值状态::当前未知,{}};
        if(C)adopted={正式内容值种类::概念引用,正式内容值状态::已知,正式内容准确值{*C}};
        正式信息参数投影 out{R,概念组.材料范围,{
            {概念组.目标词条FT,{正式内容值种类::词条引用,正式内容值状态::已知,正式内容准确值{W}}},
            {概念组.采用概念FT,std::move(adopted)}},std::nullopt};
        return {状态::已解码,std::move(out),{}};
    } catch(const 待补& missing) {return {状态::需补快照,{},missing.节点};}
    catch(const 不可解码&) {return {状态::结构不可解码,{},{}};}
}

正式信息参数读取结果 新_语言类::读取词典归类参数保护(
    稳定编码 R, 稳定编码 请求范围, const 新词典归类信息概念组& 概念组) const noexcept {
    using S = 正式信息参数读取状态;
    try {
        if (请求范围 != 概念组.材料范围) return {S::范围未支持,{},{}};
        std::lock_guard 锁(新语言互斥);
        if (!有效(组键(组织键::登记根))) return {S::依赖未就绪,{},{}};
        if (!是正式词典语言行(R)) return {S::信息不存在,{},{}};
        组织读取现场 读;
        组织要求(读.整数(R,组织键::类型) == 106);
        std::set<稳定编码> 路径;
        const auto 定义 = 读.引用(R,组织键::定义);
        const auto X = 组织读值<稳定编码>(读,读取词典组织项(读,定义,5),路径);
        组织要求(有效(X));
        正式内容值 采用{正式内容值种类::概念引用,正式内容值状态::当前未知,{}};
        const auto 解读组 = 读.字段(R,组织键::词典归类解读);
        if (!解读组.empty()) {
            组织要求(解读组.size() == 1);
            const auto 解读 = std::get<稳定编码>(解读组.front());
            const auto 状态 = 组织读值<新词典归类概念状态>(读,读取词典组织项(读,解读,0),路径);
            if (状态 == 新词典归类概念状态::已明确) {
                const auto C = 组织读值<std::optional<稳定编码>>(读,读取词典组织项(读,解读,3),路径);
                if (C) 采用 = {正式内容值种类::概念引用,正式内容值状态::已知,正式内容准确值{*C}};
            }
        }
        for (const auto 节点 : {概念组.材料范围,概念组.目标词条FT,概念组.采用概念FT}) 读.读(节点);
        正式信息参数投影 输出{R,概念组.材料范围,{
            {概念组.目标词条FT,{正式内容值种类::词条引用,正式内容值状态::已知,正式内容准确值{X}}},
            {概念组.采用概念FT,std::move(采用)}},std::nullopt};
        return {S::已读取,std::move(输出),读.保护()};
    } catch (const std::bad_alloc&) { return {S::资源失败,{},{}}; }
    catch (const std::exception& 错误) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取词典归类参数",L"正式归类材料参数读取发生工程错误。",错误.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__),__LINE__,L"读取词典归类参数",L"正式归类材料参数读取发生未知工程错误。");
    }
}

std::optional<新词典归类材料信息> 新_语言类::读取词典归类材料(稳定编码 R) const {
    std::lock_guard 锁(新语言互斥);
    if (!是正式词典语言行(R)) return std::nullopt;
    组织读取现场 读;
    return 读取词典语言行(读, R);
}

std::optional<新词典归类材料信息> 新_语言类::更新词典归类概念(
    稳定编码 R, const 新词典归类概念解读& 解读) {
    for (;;) {
        {
            std::lock_guard 锁(新语言互斥);
            if (!是正式词典语言行(R)) return std::nullopt;
            组织读取现场 读;
            auto 当前 = 读取词典语言行(读, R);
            if (当前.定义.词条N != 解读.依据词条N) return 当前;
            const bool 缺规则 = 当前.定义.状态 == 新词典语言理解状态::缺规则;
            if (缺规则 != (解读.状态 == 新词典归类概念状态::缺规则)) return 当前;
            if (!缺规则 && !当前.定义.词条N
                && 解读.状态 != 新词典归类概念状态::中心词未取得) return 当前;
            if (当前.概念解读 == 解读) return 当前;
            组织写入现场 写;
            const auto 新解读 = 写.写(解读);
            替换词典语言私有定义(读, 写, R, 新解读, 组织键::词典归类解读);
            if (!补充词典语言引用身份(写)) return 当前;
            当前.概念解读 = 解读;
            const auto 发布 = 全局基础数据集.原子追加节点组(写.请求);
            if (发布.状态 == 基础节点组追加状态::已完成) return 当前;
            if (发布.状态 == 基础节点组追加状态::资源失败) throw std::bad_alloc{};
            if (发布.状态 != 基础节点组追加状态::预期不符
                && 发布.状态 != 基础节点组追加状态::节点不存在
                && 发布.状态 != 基础节点组追加状态::引用不存在)
                throw std::logic_error("词典归类概念解读的局部发布失败");
        }
        std::this_thread::yield();
    }
}

std::vector<新词典归类材料信息> 新_语言类::查询词典归类材料(稳定编码 Q) const {
    std::lock_guard 锁(新语言互斥);
    组织读取现场 读;
    return 查询词典语言行(读, Q);
}

namespace {
std::optional<稳定编码> 查找词典语言组(组织读取现场& 读, 稳定编码 Q) {
    for (const auto& 字段 : 读.字段(组键(组织键::登记根), 组织键::词典材料组登记)) {
        const auto 根 = std::get<稳定编码>(字段);
        if (读.引用(根, 组织键::来源) == Q) return 根;
    }
    return std::nullopt;
}

新词典语言材料组定义 读取词典语言组定义(组织读取现场& 读, 稳定编码 根) {
    组织要求(读.整数(根, 组织键::类型) == 107);
    std::set<稳定编码> 路径;
    return 组织读值<新词典语言材料组定义>(读, 读.引用(根, 组织键::定义), 路径);
}
} // namespace

std::optional<稳定编码> 新_语言类::发表词典语言材料组(稳定编码 Q, 稳定编码 场景,
    const std::vector<稳定编码>& 已处理行组) {
    const 新词典语言材料组定义 定义{场景, 已处理行组};
    for (;;) {
        {
            std::lock_guard 锁(新语言互斥);
            组织读取现场 读;
            const auto 已有 = 查找词典语言组(读, Q);
            if (已有 && 读取词典语言组定义(读, *已有) == 定义) return 已有;
            组织写入现场 写;
            const auto 新定义 = 写.写(定义);
            std::optional<std::size_t> 新根;
            if (已有) {
                替换词典语言私有定义(读, 写, *已有, 新定义);
            } else {
                新根 = 写.节点(107, 0);
                写.引用(*新根, 组织键::来源, Q);
                写.子(*新根, 组织键::定义, 新定义);
                const auto 登记根 = 组键(组织键::登记根);
                写.请求.预期既有节点组.push_back(读.读(登记根).结构);
                写.请求.追加字段组.push_back({登记根, 组键(组织键::词典材料组登记), 基础组内节点引用{*新根}});
            }
            if (!补充词典语言引用身份(写)) return std::nullopt;
            const auto 发布 = 全局基础数据集.原子追加节点组(写.请求);
            if (发布.状态 == 基础节点组追加状态::已完成)
                return 新根 ? 发布.新节点组.at(*新根) : *已有;
            if (发布.状态 == 基础节点组追加状态::资源失败) throw std::bad_alloc{};
            if (发布.状态 != 基础节点组追加状态::预期不符
                && 发布.状态 != 基础节点组追加状态::节点不存在
                && 发布.状态 != 基础节点组追加状态::引用不存在)
                throw std::logic_error("词典语言材料组发表失败");
        }
        std::this_thread::yield();
    }
}

std::optional<新词典语言材料组信息> 新_语言类::读取词典语言材料组(稳定编码 Q) const {
    std::lock_guard 锁(新语言互斥);
    组织读取现场 读;
    const auto 根 = 查找词典语言组(读, Q);
    if (!根) return std::nullopt;
    const auto 定义 = 读取词典语言组定义(读, *根);
    新词典语言材料组信息 结果{*根, Q, 定义.来源场景, {}};
    for (const auto 行根 : 定义.已处理行组) 结果.行组.push_back(读取词典语言行(读, 行根));
    return 结果;
}

namespace {
using 分界状态 = 新语言分界工程状态;
using 追加状态 = 基础节点组追加状态;

bool 分界追加需要重读(追加状态 状态) noexcept {
    return 状态 == 追加状态::预期不符 || 状态 == 追加状态::节点不存在
        || 状态 == 追加状态::引用不存在;
}

const 基础字段内容* 查分界材料字段(const 基础节点结构& 结构, 语言分界键 键) {
    const 基础字段内容* 结果 = nullptr;
    for (const auto& 项 : 结构.字段组) {
        if (项.字段节点 != 分界键(键)) continue;
        if (结果) throw std::logic_error("boundary material field duplicated");
        结果 = &项.内容;
    }
    return 结果;
}

const 基础字段内容& 取分界材料字段(const 基础节点结构& 结构, 语言分界键 键) {
    const auto* 项 = 查分界材料字段(结构, 键);
    if (!项) throw std::logic_error("boundary material field missing");
    return *项;
}

std::optional<新语言分界材料信息> 读分界材料已加锁(稳定编码 节点) {
    if (!有效(节点)) return {};
    const auto 批 = 全局基础数据集.查询节点组完整结构({节点});
    if (批.状态 == 基础结构操作状态::资源失败) throw std::bad_alloc{};
    if (批.状态 == 基础结构操作状态::节点不存在) return {};
    if (批.状态 != 基础结构操作状态::已完成 || 批.节点组.size() != 1)
        throw std::logic_error("boundary material read failed");
    if (!批.节点组.front().快照) return {};
    const auto& 结构 = 批.节点组.front().快照->结构;
    const auto* 角色项 = 查分界材料字段(结构, 语言分界键::角色);
    const auto* 值 = 角色项 ? std::get_if<基础值>(角色项) : nullptr;
    const auto* 角色 = 值 ? std::get_if<std::int64_t>(&值->材料) : nullptr;
    if (!角色 || *角色 < 首个分界材料角色 || *角色 > 末个分界材料角色) return {};

    const auto* 组织 = 查分界材料字段(结构, 语言分界键::所属语言组织);
    if (!组织 || *组织 != 基础字段内容{语言组织根节点}) return {};
    if (结构.字段组.size() != 4) throw std::logic_error("invalid boundary material header");

    组织读取现场 读;
    std::set<稳定编码> 路径;
    auto 定义 = 组织读值<新语言分界材料定义>(读,
        std::get<稳定编码>(取分界材料字段(结构, 语言分界键::材料定义)), 路径);
    if (定义.index() != static_cast<std::size_t>(*角色 - 首个分界材料角色))
        throw std::logic_error("boundary material role does not match definition");
    return 新语言分界材料信息{节点,
        std::get<稳定编码>(取分界材料字段(结构, 语言分界键::材料范围)), std::move(定义)};
}

bool 分界词段完整(const std::vector<新语言分界词段>& 词段, const 新语言材料范围请求& 范围) {
    auto 当前位置 = 范围.起始标量;
    for (const auto& 项 : 词段) {
        if (项.起始标量 != 当前位置 || 项.结束标量 <= 项.起始标量
            || 项.结束标量 > 范围.结束标量 || (项.词条 && !有效(*项.词条))) return false;
        if (项.用途 != 新语言分界段用途::词面
            && (项.用途 != 新语言分界段用途::分隔 || 项.词条)) return false;
        当前位置 = 项.结束标量;
    }
    return !词段.empty() && 当前位置 == 范围.结束标量;
}

class 分界回答字面读取 final {
public:
    explicit 分界回答字面读取(const std::string& 文本) : 文本_(文本) {}

    std::vector<新语言分界词段> 对齐(const 新语言范围原文信息& 原文) {
        std::vector<新语言分界词段> 词段组;
        std::size_t 原文字节 = 0;
        auto 标量 = 原文.范围.定位.起始标量;
        要求('[');
        do {
            auto 用途 = 新语言分界段用途::词面;
            if (取得('{')) {
                if (读字符串() != "分隔") throw 分界输入拒绝{};
                要求(':');
                用途 = 新语言分界段用途::分隔;
            }
            const auto 字面 = 读字符串();
            if (用途 == 新语言分界段用途::分隔) 要求('}');
            if (字面.empty() || !新_语言类::严格UTF8字节串有效(字面)
                || 原文.范围UTF8原文.compare(原文字节, 字面.size(), 字面) != 0)
                throw 分界输入拒绝{};
            const auto 起点 = 标量;
            for (std::size_t i=0; i<字面.size(); ++标量) (void)读取已校验码点(字面,i);
            词段组.push_back({起点,标量,{},用途});
            原文字节 += 字面.size();
        } while (取得(','));
        要求(']');
        跳过空白();
        if (位置_ != 文本_.size() || 原文字节 != 原文.范围UTF8原文.size()
            || !分界词段完整(词段组,原文.范围.定位)) throw 分界输入拒绝{};
        return 词段组;
    }

private:
    void 跳过空白() {
        while (位置_<文本_.size() && std::string_view(" \t\r\n").find(文本_[位置_])!=std::string_view::npos) ++位置_;
    }
    bool 取得(char 字符) {
        跳过空白();
        if (位置_==文本_.size() || 文本_[位置_]!=字符) return false;
        ++位置_;
        return true;
    }
    void 要求(char 字符) { if (!取得(字符)) throw 分界输入拒绝{}; }

    std::uint32_t 读转义码元() {
        if (文本_.size()-位置_<4) throw 分界输入拒绝{};
        std::uint32_t 码元=0;
        for (int i=0;i<4;++i) {
            constexpr std::string_view 字母表="0123456789abcdef";
            const auto c=文本_[位置_++];
            const auto digit=字母表.find(c>='A'&&c<='F' ? static_cast<char>(c-'A'+'a') : c);
            if (digit==std::string_view::npos) throw 分界输入拒绝{};
            码元=(码元<<4)|static_cast<std::uint32_t>(digit);
        }
        return 码元;
    }
    void 追加转义码点(std::string& 字面) {
        auto 码点=读转义码元();
        if (码点>=0xD800 && 码点<=0xDBFF) {
            if (std::string_view(文本_).substr(位置_,2)!="\\u") throw 分界输入拒绝{};
            位置_+=2;
            const auto 低位=读转义码元();
            if (低位<0xDC00 || 低位>0xDFFF) throw 分界输入拒绝{};
            码点=0x10000+((码点-0xD800)<<10)+(低位-0xDC00);
        } else if (码点>=0xDC00 && 码点<=0xDFFF) throw 分界输入拒绝{};
        if (码点<0x80) {
            字面.push_back(static_cast<char>(码点));
            return;
        }
        const int 后续字节数=码点<0x800 ? 1 : 码点<0x10000 ? 2 : 3;
        constexpr std::array<std::uint32_t,4> 首字节标识{0,0xC0,0xE0,0xF0};
        字面.push_back(static_cast<char>(首字节标识[后续字节数]|(码点>>(后续字节数*6))));
        for (int i=后续字节数-1;i>=0;--i) 字面.push_back(static_cast<char>(0x80|((码点>>(i*6))&0x3F)));
    }
    std::string 读字符串() {
        要求('"');
        std::string 字面;
        while (位置_<文本_.size()) {
            const auto c=static_cast<unsigned char>(文本_[位置_++]);
            if (c=='"') return 字面;
            if (c<0x20) throw 分界输入拒绝{};
            if (c!='\\') {
                字面.push_back(static_cast<char>(c));
                continue;
            }
            if (位置_==文本_.size()) throw 分界输入拒绝{};
            const auto 转义=文本_[位置_++];
            switch (转义) {
            case '"': case '\\': case '/': 字面.push_back(转义); break;
            case 'b': 字面.push_back('\b'); break;
            case 'f': 字面.push_back('\f'); break;
            case 'n': 字面.push_back('\n'); break;
            case 'r': 字面.push_back('\r'); break;
            case 't': 字面.push_back('\t'); break;
            case 'u': 追加转义码点(字面); break;
            default: throw 分界输入拒绝{};
            }
        }
        throw 分界输入拒绝{};
    }

    const std::string& 文本_;
    std::size_t 位置_=0;
};

std::optional<新语言分界回答信息> 读分界首答(组织读取现场& 读,const 新语言问题信息& 问题) {
    const auto& 定位=问题.定义.分界目标;
    if (!定位 || !问题.答案) return {};
    const auto A=问题.答案->答案信息;
    组织要求(问题.答案->来源==新问题答案来源::原文分界回答
        &&读.整数(A,组织键::类型)==分界回答组织类型);
    const auto I=读.引用(A,组织键::子项);
    组织要求(读.整数(I,组织键::类型)==分界对齐组织类型);
    std::set<稳定编码> 访问路径;
    const auto 答复=组织读值<分界回答定义>(读,读.引用(A,组织键::定义),访问路径);
    auto 对齐=组织读值<分界对齐定义>(读,读.引用(I,组织键::定义),访问路径);
    组织要求(答复.问题==问题.问题节点 && 答复.说话者==问题.定义.交互者
        &&对齐.范围==定位->范围 && 有效(答复.回复) && 有效(答复.输出)
        &&有效(答复.实际发送) && !对齐.词段组.empty());
    return 新语言分界回答信息{A,I,答复.问题,对齐.范围,答复.回复,答复.说话者,
        答复.输出,答复.实际发送,std::move(对齐.词段组)};
}

新语言分界候选路径 构造未知原文路径(const 新语言材料范围请求& 范围) {
    return {{{范围.起始标量, 范围.结束标量, {}}}, {}};
}

bool 分界证明细化候选(const std::vector<新语言分界词段>& 候选,
    const std::vector<新语言分界词段>& 证明) {
    std::size_t 下一段 = 0;
    for (const auto& 原段 : 候选) {
        if (下一段 == 证明.size()) return false;
        if (原段.词条 || 原段.用途 == 新语言分界段用途::分隔) {
            if (原段 != 证明[下一段++]) return false;
            continue;
        }
        auto 终点 = 原段.起始标量;
        while (下一段 < 证明.size() && 证明[下一段].起始标量 == 终点
            && 证明[下一段].结束标量 <= 原段.结束标量)
            终点 = 证明[下一段++].结束标量;
        if (终点 != 原段.结束标量) return false;
    }
    return 下一段 == 证明.size();
}

bool 分界各段相符(const std::vector<新语言分界词段>& 原词段,
    const std::vector<新语言分界词段>& 新词段) {
    return std::equal(原词段.begin(), 原词段.end(), 新词段.begin(), 新词段.end(),
        [](const auto& 原段, const auto& 新段) {
            return 原段.起始标量 == 新段.起始标量 && 原段.结束标量 == 新段.结束标量
                && 原段.用途 == 新段.用途 && (!原段.词条 || 原段.词条 == 新段.词条);
        });
}

bool 分界来源完整(const 新语言分界取材来源& 来源) {
    return std::visit([](const auto& 项) {
        if constexpr (std::is_same_v<std::decay_t<decltype(项)>, 新语言规则分界来源>)
            return 有效(项.正式匹配记录);
        else
            return 有效(项.正式问题) && 有效(项.正式回答) && 有效(项.分段对齐材料);
    }, 来源);
}

新语言范围特征信息 取得分界范围(const 新_语言类& 语言, 稳定编码 R) {
    auto 范围 = 语言.读取材料范围特征(R);
    if (范围.状态 != 分界状态::已完成) throw 范围.状态;
    if (!范围.信息) throw std::logic_error("boundary range missing on success");
    return std::move(*范围.信息);
}

新语言分界材料信息 取得分界材料已加锁(稳定编码 R, 稳定编码 节点, 新语言范围特征种类 种类) {
    auto 材料 = 读分界材料已加锁(节点);
    if (!材料) throw 分界状态::材料未就绪;
    if (材料->所属范围 != R || 材料->定义.index() != static_cast<std::size_t>(种类))
        throw 分界输入拒绝{};
    return std::move(*材料);
}

std::vector<新语言分界词段> 取得有效分界词段已加锁(稳定编码 R, 稳定编码 有效结果) {
    using 种类 = 新语言范围特征种类;
    const auto 有效材料 = 取得分界材料已加锁(R, 有效结果, 种类::有效分界结果);
    auto 依据 = 取得分界材料已加锁(R,
        std::get<新语言有效分界定义>(有效材料.定义).正式依据, 种类::分界依据);
    return std::move(std::get<新语言分界依据定义>(依据.定义).取材.已证明词段);
}

void 保存分界材料已加锁(稳定编码 R, 新语言分界材料定义 定义,
    新语言分界调用结果<新语言分界材料信息>& 结果) {
    if (!有效(组织句柄.front())) throw 分界状态::未初始化;
    // 语言锁维护同R同定义唯一，L1只核对本次追加使用的端点。
    for (int 尝试 = 0; 尝试 < 2; ++尝试) {
        for (const auto& 登记 : 全局基础数据集.查询字段(R, 分界键(语言分界键::范围材料登记))) {
            auto 已有 = 读分界材料已加锁(std::get<稳定编码>(登记.内容));
            if (!已有 || 已有->所属范围 != R) throw std::logic_error("invalid range material registration");
            if (已有->定义 == 定义) {
                结果.信息 = std::move(已有);
                结果.状态 = 分界状态::已完成;
                return;
            }
        }

        组织写入现场 写;
        const auto 定义槽 = 写.写(定义);
        const auto 材料槽 = 写.请求.新节点组.size();
        写.请求.新节点组.push_back({{
            {分界键(语言分界键::角色), 基础值{首个分界材料角色 + static_cast<std::int64_t>(定义.index())}},
            {分界键(语言分界键::所属语言组织), 语言组织根节点},
            {分界键(语言分界键::材料范围), R},
            {分界键(语言分界键::材料定义), 基础组内节点引用{定义槽}}}});
        const auto 登记键 = 分界键(语言分界键::范围材料登记);
        写.请求.当前追加字段键组.push_back({R, 登记键});
        写.请求.追加字段组.push_back({R, 登记键, 基础组内节点引用{材料槽}});
        for (const auto 引用 : 写.引用组) 写.请求.只读预期.节点组.push_back({引用});
        const auto 发布 = 全局基础数据集.原子追加节点组(写.请求);
        if (分界追加需要重读(发布.状态)) continue;
        if (发布.状态 == 追加状态::资源失败) throw std::bad_alloc{};
        if (发布.状态 != 追加状态::已完成 || 发布.新节点组.size() != 写.请求.新节点组.size())
            throw std::logic_error("boundary material publication failed");
        结果.本次已发表身份 = 发布.新节点组[材料槽];
        结果.信息 = 新语言分界材料信息{*结果.本次已发表身份, R, std::move(定义)};
        结果.状态 = 分界状态::已完成;
        return;
    }
    throw 分界状态::材料未就绪;
}

template<class 材料 = 新语言分界材料信息, class 操作>
新语言分界调用结果<材料> 调用分界材料入口(const wchar_t* 入口, 操作&& 执行) {
    新语言分界调用结果<材料> 结果;
    try { 执行(结果); }
    catch (const 分界输入拒绝&) { 结果.状态 = 分界状态::输入拒绝; }
    catch (分界状态 状态) { 结果.状态 = 状态; }
    catch (追加状态 状态) {
        if (状态 == 追加状态::资源失败) 结果.状态 = 分界状态::资源失败;
        else if (分界追加需要重读(状态)) 结果.状态 = 分界状态::材料未就绪;
        else 分界来源工程终止(入口, {}, 结果.本次已发表身份, "boundary write endpoint rejected");
    }
    catch (const std::bad_alloc&) { 结果.状态 = 分界状态::资源失败; }
    catch (const std::length_error&) { 结果.状态 = 分界状态::资源失败; }
    catch (const std::exception& 异常) { 分界来源工程终止(入口, {}, 结果.本次已发表身份, 异常.what()); }
    catch (...) { 分界来源工程终止(入口, {}, 结果.本次已发表身份, nullptr); }
    if (结果.状态 != 分界状态::已完成) 结果.信息.reset();
    return 结果;
}

struct 分界采用记录值 final {
    稳定编码 范围, 实际材料, 特征类型, 特征, 来源;
    std::optional<稳定编码> 新状态;
    std::optional<std::int64_t> 新状态UTC纳秒;
};

std::optional<分界采用记录值> 读已有分界采用已加锁(
    稳定编码 范围, 语言分界键 登记键, 新语言范围特征种类 目标种类, 稳定编码 来源) {
    for (const auto& 登记 : 全局基础数据集.查询字段(范围, 分界键(登记键))) {
        const auto 节点 = std::get<稳定编码>(登记.内容);
        const auto 批 = 读取分界节点组({节点});
        const auto& 结构 = 取分界快照(批, 节点).结构;
        const auto 取引用 = [&](语言分界键 键) {
            return std::get<稳定编码>(取分界材料字段(结构, 键));
        };
        const auto 种类 = std::get<std::int64_t>(
            std::get<基础值>(取分界材料字段(结构, 语言分界键::采用种类)).材料);
        if (取引用(语言分界键::采用来源) != 来源 || 种类 != static_cast<std::int64_t>(目标种类))
            continue;
        if (取引用(语言分界键::材料范围) != 范围)
            throw std::logic_error("boundary adoption registered on a different range");
        分界采用记录值 结果{范围,
            取引用(语言分界键::采用材料), 取引用(语言分界键::采用特征类型),
            取引用(语言分界键::采用特征), 来源, {}, {}};
        const auto* 状态 = 查分界材料字段(结构, 语言分界键::采用状态);
        const auto* 状态时间 = 查分界材料字段(结构, 语言分界键::采用状态时间);
        if ((状态 == nullptr) != (状态时间 == nullptr))
            throw std::logic_error("boundary adoption state and time must be recorded together");
        if (状态) {
            结果.新状态 = std::get<稳定编码>(*状态);
            结果.新状态UTC纳秒 = std::get<std::int64_t>(std::get<基础值>(*状态时间).材料);
        }
        return 结果;
    }
    return {};
}

新语言分界采用回执 投影执行分界采用(const 分界采用记录值& 记录) {
    return {记录.范围, 记录.实际材料, 记录.特征类型, 记录.特征, 记录.来源,
        记录.新状态, 记录.新状态UTC纳秒};
}

新语言分界依据采用回执 投影分界依据采用(const 分界采用记录值& 记录) {
    return {记录.范围, 记录.实际材料, 记录.特征类型, 记录.特征, 记录.新状态, 记录.新状态UTC纳秒};
}

class 分界采用记录参与 final : public 新状态结果引用参与接口 {
public:
    分界采用记录参与(分界采用记录值& 回执, 新语言范围特征种类 种类, 语言分界键 登记键)
        : 回执_(回执), 种类_(种类), 登记键_(登记键) {}

    基础节点组追加请求 构造记录() const {
        return 构造记录(0);
    }

    新状态结果追加构造 构造同组状态引用(const 新同组状态端点& 状态) const override {
        状态槽_ = std::get<基础组内节点引用>(状态.状态定位).索引;
        auto 请求 = 构造记录(状态.后续新节点起点);
        auto& 字段组 = 请求.新节点组.front().字段组;
        字段组.push_back({分界键(语言分界键::采用状态), 基础组内节点引用{*状态槽_}});
        字段组.push_back({分界键(语言分界键::采用状态时间), 基础值{状态.强时间.纳秒}});
        回执_.新状态UTC纳秒 = 状态.强时间.纳秒;
        return {std::move(请求), 1};
    }

    void 登记共同发表回执(const 基础片段发布回执& 发布) noexcept override {
        const auto& 节点组 = 发布.新节点组();
        if (记录槽_ >= 节点组.size() || (状态槽_ && *状态槽_ >= 节点组.size()))
            分界来源工程终止(L"登记分界采用回执", {}, {}, "boundary adoption receipt missing nodes");
        记录身份_ = 节点组[记录槽_];
        if (状态槽_) 回执_.新状态 = 节点组[*状态槽_];
    }

    稳定编码 记录身份() const noexcept { return 记录身份_; }

private:
    基础节点组追加请求 构造记录(std::size_t 记录槽) const {
        记录槽_ = 记录槽;
        基础节点组追加请求 请求;
        请求.新节点组.push_back({{
            {分界键(语言分界键::材料范围), 回执_.范围},
            {分界键(语言分界键::采用来源), 回执_.来源},
            {分界键(语言分界键::采用种类), 基础值{static_cast<std::int64_t>(种类_)}},
            {分界键(语言分界键::采用材料), 回执_.实际材料},
            {分界键(语言分界键::采用特征类型), 回执_.特征类型},
            {分界键(语言分界键::采用特征), 回执_.特征}}});
        const auto 登记键 = 分界键(登记键_);
        请求.追加字段组.push_back({回执_.范围, 登记键, 基础组内节点引用{记录槽}});
        请求.当前追加字段键组.push_back({回执_.范围, 登记键});
        const std::set<稳定编码> 引用{回执_.范围, 回执_.来源, 回执_.实际材料, 回执_.特征类型, 回执_.特征};
        for (const auto 节点 : 引用) 请求.只读预期.节点组.push_back({节点});
        return 请求;
    }

    分界采用记录值& 回执_;
    新语言范围特征种类 种类_;
    语言分界键 登记键_;
    mutable std::size_t 记录槽_ = 0;
    mutable std::optional<std::size_t> 状态槽_;
    稳定编码 记录身份_;
};

新状态强时间 取得分界当前UTC() {
    using namespace std::chrono;
    const auto 当前 = system_clock::now().time_since_epoch();
    if (当前 < system_clock::duration::zero()
        || 当前 > duration_cast<system_clock::duration>((nanoseconds::max)()))
        throw std::logic_error("boundary adoption time cannot be represented as UTC nanoseconds");
    return {新状态时间语义::实例绝对UTC, duration_cast<nanoseconds>(当前).count()};
}
} // namespace

auto 新_语言类::提交分界材料采用(
    稳定编码 R, 新语言范围特征种类 目标种类, 稳定编码 材料, 稳定编码 来源,
    分界采用来路 来路, 新_场景类& 场景, 新_状态类& 状态,
    std::optional<稳定编码>& 本次已发表身份, std::stop_token 停止, std::optional<稳定编码> 预期当前依据) {
    using 准备状态 = 新特征I64准备状态;
    const auto 种类 = static_cast<std::size_t>(目标种类);
    const auto 登记键 = [来路] {
        switch (来路) {
        case 分界采用来路::执行: return 语言分界键::范围采用登记;
        case 分界采用来路::接收候选: return 语言分界键::接收候选采用登记;
        case 分界采用来路::正式依据: return 语言分界键::正式依据采用登记;
        case 分界采用来路::接收准入: return 语言分界键::接收准入采用登记;
        }
        throw std::logic_error("unknown boundary adoption origin");
    }();
    auto& 特征 = *分界特征服务_;
    std::lock_guard 采用锁(新语言分界采用互斥);
    for (int 尝试 = 0; 尝试 < 2; ++尝试) {
        if (停止.stop_requested()) throw 分界状态::材料未就绪;
        std::optional<稳定编码> 依据候选;
        {
            std::lock_guard 锁(新语言互斥);
            if (!有效(组织句柄.front())) throw 分界状态::未初始化;
            if (const auto 已有 = 读已有分界采用已加锁(R, 登记键, 目标种类, 来源))
                return *已有;
            if (!有效(材料)) throw 分界输入拒绝{};
            const auto 材料信息 = 取得分界材料已加锁(R, 材料, 目标种类);
            if (来路 == 分界采用来路::正式依据)
                依据候选 = std::get<新语言分界依据定义>(材料信息.定义).候选材料;
            if (来路 == 分界采用来路::接收准入
                && std::get<新语言词条准入定义>(材料信息.定义).有效结果 != 来源)
                throw 分界输入拒绝{};
        }

        // 先取场景／状态构造绑定；取得特征准备后只调用纯构造入口。
        const auto 范围 = 取得分界范围(*this, R);
        if (依据候选) {
            using 读取状态 = 新特征完整读取状态;
            constexpr auto 候选槽 = static_cast<std::size_t>(新语言范围特征种类::分词候选材料);
            const auto 现候选 = 特征.读取特征完整保护(范围.范围.特征节点组[候选槽]);
            switch (现候选.状态) {
            case 读取状态::资源失败: throw std::bad_alloc{};
            case 读取状态::前提已变化: throw 分界状态::前提已变化;
            case 读取状态::特征不存在:
            case 读取状态::依赖未就绪: throw 分界状态::材料未就绪;
            case 读取状态::已读取: break;
            default: throw std::logic_error("boundary candidate feature is not supported");
            }
            if (!现候选.信息) throw std::logic_error("boundary candidate feature read is empty");
            const auto& 当前值 = 现候选.信息->特征.当前值;
            if (!当前值) throw 分界状态::材料未就绪;
            if (*当前值 != 新特征准确值{*依据候选}) throw 分界状态::前提已变化;
        }
        const auto F = 范围.范围.特征节点组[种类];
        分界采用记录值 回执{R, 材料, 范围.特征类型组[种类], F, 来源, {}, {}};
        std::optional<新场景状态参与绑定> 场景绑定;
        try {
            场景绑定 = 场景.取得场景状态参与绑定(范围.范围.定位.所属语言场景);
        } catch (追加状态 状态码) {
            if (分界追加需要重读(状态码)) continue;
            throw;
        }
        const auto 状态绑定 = 状态.取得特征状态构造绑定();
        std::vector<新特征当前值设置项> 设置组{{F, 新特征准确值{材料}, false}};
        std::optional<稳定编码> 有效结果特征;
        if (来路 == 分界采用来路::接收准入) {
            constexpr auto 结果槽 = static_cast<std::size_t>(新语言范围特征种类::有效分界结果);
            有效结果特征 = 范围.范围.特征节点组[结果槽];
            设置组.push_back({*有效结果特征, 新特征准确值{来源}, false});
        }
        auto 准备 = 特征.准备特征当前值组(设置组);
        if (准备.状态 == 准备状态::预期已变化 || 准备.状态 == 准备状态::特征不存在
            || 准备.状态 == 准备状态::概念不存在) continue;
        if (准备.状态 == 准备状态::联合更新占用) throw 分界状态::材料未就绪;
        if (准备.状态 == 准备状态::资源失败) throw std::bad_alloc{};
        if (准备.状态 != 准备状态::已准备 || !准备.准备)
            throw std::logic_error("boundary feature group preparation failed");

        if (有效结果特征 && 特征.读取准备原当前值(*准备.准备, *有效结果特征)
            != 新特征准确值{来源}) {
            特征.取消特征当前值组(*准备.准备);
            throw 分界状态::前提已变化;
        }

        if (来路 == 分界采用来路::正式依据) {
            const auto 原值 = 特征.读取准备原当前值(*准备.准备, F);
            std::optional<新特征准确值> 预期;
            if (预期当前依据) 预期 = 新特征准确值{*预期当前依据};
            if (原值 != 预期 && 原值 != 新特征准确值{材料}) {
                特征.取消特征当前值组(*准备.准备);
                throw 分界状态::前提已变化;
            }
        }
        const auto 候选 = 特征.读取准备当前值候选(*准备.准备, F);
        分界采用记录参与 参与(回执, 目标种类, 登记键);
        auto 封存 = [&] {
            if (!候选.已改变_)
                return 全局基础数据集.封存原子追加片段(参与.构造记录());
            const auto 时间 = 取得分界当前UTC();
            auto 状态材料 = 状态.准备特征状态(状态绑定, 候选, R, 时间);
            状态.合并结果引用(状态材料, 参与);
            return 场景.封存场景状态挂接(*场景绑定, std::move(状态材料)).封存;
        }();
        if (封存.状态 != 追加状态::已完成) {
            特征.取消特征当前值组(*准备.准备);
            if (分界追加需要重读(封存.状态)) continue;
            throw 封存.状态;
        }
        if (!封存.片段) throw std::logic_error("boundary adoption seal missing");
        if (停止.stop_requested()) {
            特征.取消特征当前值组(*准备.准备);
            throw 分界状态::材料未就绪;
        }
        const auto 发布 = 特征.提交特征当前值组(*准备.准备, {*封存.片段});
        if (发布.状态 != 追加状态::已完成) {
            特征.取消特征当前值组(*准备.准备);
            if (分界追加需要重读(发布.状态)) continue;
            throw 发布.状态;
        }
        if (发布.附加片段回执组.size() != 1)
            分界来源工程终止(L"采用分界材料", 来源, {}, "published boundary adoption receipt missing");
        参与.登记共同发表回执(发布.附加片段回执组.front());
        本次已发表身份 = 参与.记录身份();
        (void)特征.完成特征当前值组(*准备.准备);
        特征.结束特征当前值组(*准备.准备);
        return 回执;
    }
    throw 分界状态::材料未就绪;
}

新语言分界调用结果<新语言分界材料信息> 新_语言类::保存分界候选(
    稳定编码 R, const 新语言分界候选定义& 输入) {
    return 调用分界材料入口(L"保存分界候选", [&](auto& 结果) {
        const auto 范围 = 取得分界范围(*this, R);
        if (输入.路径组.empty()) throw 分界输入拒绝{};
        新语言分界候选定义 定义{{构造未知原文路径(范围.范围.定位)}};
        for (const auto& 路径 : 输入.路径组) {
            if (!分界词段完整(路径.词段组, 范围.范围.定位)
                || !std::all_of(路径.规则应用记录组.begin(), 路径.规则应用记录组.end(),
                    [](稳定编码 节点) { return 有效(节点); }))
                throw 分界输入拒绝{};
            if (std::find(定义.路径组.begin(), 定义.路径组.end(), 路径) == 定义.路径组.end())
                定义.路径组.push_back(路径);
        }
        std::lock_guard 锁(新语言互斥);
        保存分界材料已加锁(R, std::move(定义), 结果);
    });
}

新语言分界调用结果<新语言分界回答信息> 新_语言类::接纳原文分界回答(
    const 新语言明确回复事实& 事实, const 新_特征值类& 值服务) {
    return 调用分界材料入口<新语言分界回答信息>(L"接纳原文分界回答", [&](auto& 结果) {
        const auto& M=事实.回复;
        const auto& O=事实.输出;
        const auto& Send=事实.实际发送;
        if (!明确回复传输一致(事实)) throw 分界输入拒绝{};

        const auto Q=*O.关联问题;
        const auto 问题=读取问题(Q);
        if (!问题) throw 分界状态::前提已变化;
        if (问题->定义.被问种类!=新问题被问种类::原文分界 || !问题->定义.分界目标)
            throw 分界输入拒绝{};
        const auto& 目标=*问题->定义.分界目标;
        const auto 同一对话框=[&](const 新外设消息信息& 消息) {
            return 消息.场景节点==目标.外设场景 && 消息.窗口节点==目标.外设窗口
                &&消息.来源.公开宿主会话==目标.公开会话 && 消息.来源.对话框标识==目标.对话框;
        };
        if (!同一对话框(M) || !同一对话框(O) || !同一对话框(Send)
            || 问题->定义.交互者!=事实.说话者 || M.正式记录时间.纳秒<=问题->定义.提问时刻
            || M.正式记录时间.纳秒<=Send.正式记录时间.纳秒)
            throw 分界输入拒绝{};
        const auto R=目标.范围;
        const auto 原文=读取材料范围原文(R,值服务);
        if (原文.状态!=分界状态::已完成) throw 原文.状态;
        组织要求(原文.信息.has_value());
        const auto 词段组=分界回答字面读取(M.UTF8正文).对齐(*原文.信息);

        std::lock_guard 锁(新语言互斥);
        组织读取现场 读;
        const auto 问题组=读全部问题(读);
        const auto 当前=std::find_if(问题组.begin(),问题组.end(),[&](const auto& x){return x.问题节点==Q;});
        if (当前==问题组.end() || 当前->定义!=问题->定义) throw 分界状态::前提已变化;
        if (const auto 已有=读分界首答(读,*当前)) {
            if (已有->回复!=M.消息节点) throw 分界输入拒绝{};
            组织要求(已有->输出==O.消息节点 && 已有->实际发送==Send.消息节点 && 已有->词段组==词段组);
            结果.信息=已有;
            结果.状态=分界状态::已完成;
            return;
        }

        组织写入现场 写;
        const auto A=写.节点(分界回答组织类型,0), I=写.节点(分界对齐组织类型,0);
        const auto 回答体=写.写(分界回答定义{Q,M.消息节点,事实.说话者,O.消息节点,Send.消息节点});
        const auto 对齐体=写.写(分界对齐定义{R,词段组});
        写.子(A,组织键::定义,回答体);
        写.子(A,组织键::子项,I);
        写.子(I,组织键::定义,对齐体);
        写.请求.追加字段组.push_back({Q,组键(组织键::答案),基础组内节点引用{A}});
        写.请求.追加字段组.push_back({Q,组键(组织键::答案来源),
            基础值{static_cast<std::int64_t>(新问题答案来源::原文分界回答)}});
        写.请求.预期既有节点组.push_back(读.读(Q).结构);
        for (const auto 节点 : {M.消息节点,O.消息节点,Send.消息节点}) {
            const auto 快照=std::find_if(事实.正式消息保护.begin(),事实.正式消息保护.end(),
                [&](const auto& 项){return 项.结构.节点.编码==节点;});
            组织要求(快照!=事实.正式消息保护.end());
            写.请求.预期既有节点组.push_back(快照->结构);
        }
        for (const auto 引用 : 写.引用组) 写.请求.只读预期.节点组.push_back({引用});
        const auto 发布=全局基础数据集.原子追加节点组(写.请求);
        if (发布.状态==追加状态::资源失败) throw std::bad_alloc{};
        if (分界追加需要重读(发布.状态)) throw 分界状态::前提已变化;
        组织要求(发布.状态==追加状态::已完成 && 发布.新节点组.size()==写.请求.新节点组.size());
        结果.本次已发表身份=发布.新节点组.at(A);
        组织读取现场 读回;
        auto 已回答=*当前;
        已回答.答案=新问题答案引用{std::get<稳定编码>(读回.唯一(Q,组织键::答案)),
            static_cast<新问题答案来源>(读回.整数(Q,组织键::答案来源))};
        组织要求(已回答.答案->答案信息==*结果.本次已发表身份);
        结果.信息=读分界首答(读回,已回答);
        组织要求(结果.信息.has_value());
        结果.状态=分界状态::已完成;
    });
}

std::optional<新语言分界回答信息> 新_语言类::读取原文分界回答(稳定编码 Q) const {
    std::lock_guard 锁(新语言互斥);
    组织读取现场 读;
    for (const auto& 问题 : 读全部问题(读))
        if (问题.问题节点==Q) return 读分界首答(读,问题);
    return {};
}

新语言分界调用结果<新语言分界材料信息> 新_语言类::形成分界依据(
    稳定编码 R, 稳定编码 候选, const 新语言正式分界取材接口& 端口) {
    return 调用分界材料入口(L"形成分界依据", [&](auto& 结果) {
        const auto 范围 = 取得分界范围(*this, R);
        const auto 原材料 = [&] {
            std::lock_guard 锁(新语言互斥);
            if (!有效(组织句柄.front())) throw 分界状态::未初始化;
            return 取得分界材料已加锁(R, 候选, 新语言范围特征种类::分词候选材料);
        }();
        auto 取材 = 端口.读取已确认分界(R, 候选);
        if (取材.状态 != 分界状态::已完成) {
            结果.状态 = 取材.状态 == 分界状态::未找到 ? 分界状态::材料未就绪 : 取材.状态;
            return;
        }
        if (!取材.信息) throw std::logic_error("confirmed boundary source missing on success");
        const auto& 证明 = 取材.信息->已证明词段;
        const auto& 路径组 = std::get<新语言分界候选定义>(原材料.定义).路径组;
        if (!分界词段完整(证明, 范围.范围.定位) || !分界来源完整(取材.信息->来源)
            || std::none_of(路径组.begin(), 路径组.end(), [&](const auto& 路径) {
                return 分界证明细化候选(路径.词段组, 证明);
            }))
            throw 分界输入拒绝{};
        std::lock_guard 锁(新语言互斥);
        保存分界材料已加锁(R, 新语言分界依据定义{候选, std::move(*取材.信息)}, 结果);
    });
}

新语言分界调用结果<新语言分界材料信息> 新_语言类::保存有效分界结果(稳定编码 R, 稳定编码 依据) {
    return 调用分界材料入口(L"保存有效分界结果", [&](auto& 结果) {
        (void)取得分界范围(*this, R);
        std::lock_guard 锁(新语言互斥);
        if (!有效(组织句柄.front())) throw 分界状态::未初始化;
        (void)取得分界材料已加锁(R, 依据, 新语言范围特征种类::分界依据);
        保存分界材料已加锁(R, 新语言有效分界定义{依据}, 结果);
    });
}

新语言分界调用结果<新语言分界材料信息> 新_语言类::保存词条准入回执(
    稳定编码 R, const 新语言词条准入定义& 定义) {
    return 调用分界材料入口(L"保存词条准入回执", [&](auto& 结果) {
        (void)取得分界范围(*this, R);
        std::lock_guard 锁(新语言互斥);
        if (!有效(组织句柄.front())) throw 分界状态::未初始化;
        const auto 证明 = 取得有效分界词段已加锁(R, 定义.有效结果);
        if (!分界各段相符(证明, 定义.词段组)
            || !std::all_of(定义.词段组.begin(), 定义.词段组.end(),
                [](const auto& 词段) {
                    return 词段.用途 == 新语言分界段用途::分隔
                        ? !词段.词条 : 词段.词条 && 有效(*词段.词条);
                }))
            throw 分界输入拒绝{};
        保存分界材料已加锁(R, 定义, 结果);
    });
}

新语言分界调用结果<新语言分界材料信息> 新_语言类::读取分界材料(稳定编码 节点) const {
    return 调用分界材料入口(L"读取分界材料", [&](auto& 结果) {
        std::lock_guard 锁(新语言互斥);
        if (!分界配置就绪_.load(std::memory_order_acquire) || !有效(组织句柄.front()))
            throw 分界状态::未初始化;
        结果.信息 = 读分界材料已加锁(节点);
        结果.状态 = 结果.信息 ? 分界状态::已完成 : 分界状态::未找到;
    });
}

新语言分界调用结果<新语言分界采用回执> 新_语言类::读取执行分界采用(
    稳定编码 范围, 新语言范围特征种类 种类, 稳定编码 执行) const {
    return 调用分界材料入口<新语言分界采用回执>(L"读取执行分界采用", [&](auto& 结果) {
        if (!有效(范围) || !有效(执行) || static_cast<std::size_t>(种类) >= 范围特征键组.size())
            throw 分界输入拒绝{};
        std::lock_guard 锁(新语言互斥);
        if (!分界配置就绪_.load(std::memory_order_acquire) || !有效(组织句柄.front()))
            throw 分界状态::未初始化;
        const auto 记录 = 读已有分界采用已加锁(范围, 语言分界键::范围采用登记, 种类, 执行);
        if (记录) 结果.信息 = 投影执行分界采用(*记录);
        结果.状态 = 记录 ? 分界状态::已完成 : 分界状态::未找到;
    });
}

新语言分界调用结果<新语言分界采用回执> 新_语言类::读取执行分界采用(
    稳定编码 范围, 稳定编码 特征类型, 稳定编码 执行) const {
    using 状态 = 新语言分界工程状态;
    if (!有效(范围) || !有效(特征类型) || !有效(执行))
        return {状态::输入拒绝, {}, {}};
    if (!分界配置就绪_.load(std::memory_order_acquire))
        return {状态::未初始化, {}, {}};
    const auto& 类型组 = 分界特征配置_.特征类型组;
    const auto 槽 = std::find(类型组.begin(), 类型组.end(), 特征类型);
    if (槽 == 类型组.end()) return {状态::输入拒绝, {}, {}};
    return 读取执行分界采用(范围,
        static_cast<新语言范围特征种类>(槽 - 类型组.begin()), 执行);
}

新语言分界调用结果<新语言分界采用回执> 新_语言类::采用分界材料(
    const 新语言分界采用请求& 输入, 新_场景类& 场景, 新_状态类& 状态) {
    return 调用分界材料入口<新语言分界采用回执>(L"采用分界材料", [&](auto& 结果) {
        if (!有效(输入.范围) || !有效(输入.执行) || static_cast<std::size_t>(输入.种类) >= 范围特征键组.size())
            throw 分界输入拒绝{};
        if (!分界配置就绪_.load(std::memory_order_acquire)) throw 分界状态::未初始化;
        const auto 采用 = 提交分界材料采用(输入.范围, 输入.种类, 输入.材料, 输入.执行,
            分界采用来路::执行, 场景, 状态, 结果.本次已发表身份, {});
        结果.信息 = 投影执行分界采用(采用);
        结果.状态 = 分界状态::已完成;
    });
}

新语言分界调用结果<新语言分界依据采用回执> 新_语言类::采用已形成分界依据(
    const 新语言分界依据采用请求& 输入, 新_场景类& 场景, 新_状态类& 状态, std::stop_token 停止) {
    return 调用分界材料入口<新语言分界依据采用回执>(L"采用已形成分界依据", [&](auto& 结果) {
        if (!有效(输入.范围) || !有效(输入.依据) || (输入.预期当前依据 && !有效(*输入.预期当前依据)))
            throw 分界输入拒绝{};
        if (!分界配置就绪_.load(std::memory_order_acquire)) throw 分界状态::未初始化;
        const auto 采用 = 提交分界材料采用(输入.范围, 新语言范围特征种类::分界依据, 输入.依据, 输入.依据,
            分界采用来路::正式依据, 场景, 状态, 结果.本次已发表身份, 停止, 输入.预期当前依据);
        结果.信息 = 投影分界依据采用(采用);
        结果.状态 = 分界状态::已完成;
    });
}

新语言分界调用结果<新语言分界依据采用回执> 新_语言类::读取分界依据采用(
    稳定编码 R, 稳定编码 依据) const {
    return 调用分界材料入口<新语言分界依据采用回执>(L"读取分界依据采用", [&](auto& 结果) {
        if (!有效(R) || !有效(依据)) throw 分界输入拒绝{};
        std::lock_guard 锁(新语言互斥);
        if (!分界配置就绪_.load(std::memory_order_acquire) || !有效(组织句柄.front()))
            throw 分界状态::未初始化;
        const auto 记录 = 读已有分界采用已加锁(R, 语言分界键::正式依据采用登记,
            新语言范围特征种类::分界依据, 依据);
        if (记录) 结果.信息 = 投影分界依据采用(*记录);
        结果.状态 = 记录 ? 分界状态::已完成 : 分界状态::未找到;
    });
}

新语言分界调用结果<稳定编码> 新_语言类::采用接收分词候选(
    稳定编码 R, 稳定编码 前置来源, const 新_特征值类& 值, const 新_词类& 词条,
    新_场景类& 场景, 新_状态类& 状态, std::stop_token 停止) {
    return 调用分界材料入口<稳定编码>(L"采用接收分词候选", [&](auto& 结果) {
        if (!有效(R) || !有效(前置来源)) throw 分界输入拒绝{};
        if (停止.stop_requested()) throw 分界状态::材料未就绪;
        if (!分界配置就绪_.load(std::memory_order_acquire)) throw 分界状态::未初始化;
        const auto 范围 = 取得分界范围(*this, R);
        if (范围.范围.定位.前置来源节点 != 前置来源) throw 分界输入拒绝{};
        constexpr auto 登记键 = 语言分界键::接收候选采用登记;
        constexpr auto 种类 = 新语言范围特征种类::分词候选材料;
        {
            std::lock_guard 锁(新语言互斥);
            if (const auto 已有 = 读已有分界采用已加锁(R, 登记键, 种类, 前置来源)) {
                结果.信息 = 已有->实际材料;
                结果.状态 = 分界状态::已完成;
                return;
            }
        }

        const auto 候选 = 生成分词候选(R, 值, 词条, 停止);
        if (候选.状态 != 分界状态::已完成) throw 候选.状态;
        if (停止.stop_requested()) throw 分界状态::材料未就绪;
        const auto 材料 = 保存分界候选(R, *候选.信息);
        if (材料.状态 != 分界状态::已完成) throw 材料.状态;
        const auto 采用 = 提交分界材料采用(R, 种类, 材料.信息->节点, 前置来源,
            分界采用来路::接收候选, 场景, 状态, 结果.本次已发表身份, 停止);
        结果.信息 = 采用.实际材料;
        结果.状态 = 分界状态::已完成;
    });
}

namespace {
std::optional<稳定编码> 读取分界槽当前引用(const 新_特征类& 特征, 稳定编码 F) {
    using S = 新特征完整读取状态;
    const auto 读 = 特征.读取特征完整保护(F);
    switch (读.状态) {
    case S::资源失败: throw std::bad_alloc{};
    case S::前提已变化: throw 分界状态::前提已变化;
    case S::特征不存在:
    case S::依赖未就绪: throw 分界状态::材料未就绪;
    case S::已读取: break;
    default: throw std::logic_error("formal reception boundary feature unreadable");
    }
    if (!读.信息) throw std::logic_error("formal reception boundary feature read missing");
    const auto& 当前值 = 读.信息->特征.当前值;
    if (!当前值) return {};
    const auto* 引用 = std::get_if<稳定编码>(&*当前值);
    if (!引用 || !有效(*引用)) throw std::logic_error("formal reception boundary value is not a reference");
    return *引用;
}
}

新语言分界调用结果<新语言分界材料信息> 新_语言类::保存已确认词条准入(
    稳定编码 R, 稳定编码 有效结果, const 新语言词条准入取材& 取材, 新_词类& 词条服务,
    const std::function<bool()>& 停止已请求) {
    return 调用分界材料入口(L"保存已确认词条准入", [&](auto& 结果) {
        新语言词条准入定义 定义{有效结果, {}};
        定义.词段组.reserve(取材.词段材料组.size());
        for (const auto& 材料 : 取材.词段材料组) {
            if (停止已请求()) throw 分界状态::材料未就绪;
            auto 词段 = 材料.词段;
            if (词段.用途 == 新语言分界段用途::词面 && !词段.词条) {
                if (!材料.未登记原文词面) throw std::logic_error("confirmed source word surface missing");
                const auto 词条 = 词条服务.建立或取得词条({取材.语言节点,
                    取材.规范化合同节点, *材料.未登记原文词面, 有效结果});
                using S = 新词条建立状态;
                if (词条.状态 == S::依赖已变化) throw 分界状态::前提已变化;
                if (词条.状态 == S::依赖不存在) throw 分界状态::材料未就绪;
                if (词条.状态 == S::资源失败) throw std::bad_alloc{};
                if (!词条.词条节点) throw std::logic_error("confirmed source word was not admitted");
                词段.词条 = *词条.词条节点;
            }
            定义.词段组.push_back(std::move(词段));
        }
        if (停止已请求()) throw 分界状态::材料未就绪;
        结果 = 保存词条准入回执(R, 定义);
    });
}

新语言分界调用结果<新语言分界材料信息> 新_语言类::读取当前接收词条准入(
    稳定编码 R, 稳定编码 前置来源) const {
    return 调用分界材料入口(L"读取当前接收词条准入", [&](auto& 结果) {
        if (!分界配置就绪_.load(std::memory_order_acquire)) throw 分界状态::未初始化;
        std::lock_guard 采用锁(新语言分界采用互斥);
        const auto 范围 = 取得分界范围(*this, R);
        if (范围.范围.定位.前置来源节点 != 前置来源) throw 分界输入拒绝{};
        constexpr auto 结果槽 = static_cast<std::size_t>(新语言范围特征种类::有效分界结果);
        constexpr auto 准入槽 = static_cast<std::size_t>(新语言范围特征种类::词条准入结果);
        const auto 有效结果 = 读取分界槽当前引用(*分界特征服务_, 范围.范围.特征节点组[结果槽]);
        const auto 准入 = 读取分界槽当前引用(*分界特征服务_, 范围.范围.特征节点组[准入槽]);
        if (!有效结果 || !准入) throw 分界状态::未找到;
        std::lock_guard 锁(新语言互斥);
        auto 材料 = 取得分界材料已加锁(R, *准入, 新语言范围特征种类::词条准入结果);
        if (std::get<新语言词条准入定义>(材料.定义).有效结果 != *有效结果) throw 分界状态::未找到;
        结果.信息 = std::move(材料);
        结果.状态 = 分界状态::已完成;
    });
}

新语言分界调用结果<新语言分界材料信息> 新_语言类::采用接收词条准入(
    稳定编码 R, 稳定编码 前置来源, const 新_特征值类& 值, 新_词类& 词条,
    新_场景类& 场景, 新_状态类& 状态, std::stop_token 停止) {
    return 调用分界材料入口(L"采用接收词条准入", [&](auto& 结果) {
        if (停止.stop_requested()) throw 分界状态::材料未就绪;
        结果 = 读取当前接收词条准入(R, 前置来源);
        if (结果.状态 != 分界状态::未找到) return;
        const auto 范围 = 取得分界范围(*this, R);
        if (范围.范围.定位.前置来源节点 != 前置来源) throw 分界输入拒绝{};
        constexpr auto 结果槽 = static_cast<std::size_t>(新语言范围特征种类::有效分界结果);
        const auto 有效结果 = 读取分界槽当前引用(*分界特征服务_, 范围.范围.特征节点组[结果槽]);
        if (!有效结果) throw 分界状态::未找到;
        const auto 取材 = 读取词条准入取材(R, *有效结果, 值);
        if (取材.状态 != 分界状态::已完成) throw 取材.状态;
        if (!取材.信息) throw std::logic_error("formal reception admission source missing");
        const auto 保存 = 保存已确认词条准入(R, *有效结果, *取材.信息, 词条,
            [&] { return 停止.stop_requested(); });
        if (保存.状态 != 分界状态::已完成) throw 保存.状态;
        if (!保存.信息) throw std::logic_error("formal reception admission material missing");
        (void)提交分界材料采用(R, 新语言范围特征种类::词条准入结果, 保存.信息->节点,
            *有效结果, 分界采用来路::接收准入, 场景, 状态, 结果.本次已发表身份, 停止);
        auto 读回 = 读取当前接收词条准入(R, 前置来源);
        if (读回.状态 != 分界状态::已完成) throw 读回.状态;
        结果.信息 = std::move(读回.信息);
        结果.状态 = 分界状态::已完成;
    });
}

新语言分界调用结果<新语言范围原文信息> 新_语言类::读取材料范围原文(
    稳定编码 R, const 新_特征值类& 值服务) const {
    return 调用分界材料入口<新语言范围原文信息>(L"读取材料范围原文", [&](auto& 结果) {
        const auto 读快照 = [](稳定编码 节点) -> std::optional<基础联合节点快照> {
            auto 批 = 全局基础数据集.查询节点组完整结构({节点});
            if (批.状态 == 基础结构操作状态::资源失败) throw std::bad_alloc{};
            if (批.状态 == 基础结构操作状态::节点不存在) return {};
            if (批.状态 != 基础结构操作状态::已完成 || 批.节点组.size() != 1)
                throw std::logic_error("boundary fixed source read failed");
            return std::move(批.节点组.front().快照);
        };

        for (int 尝试 = 0; 尝试 < 2; ++尝试) {
            auto 范围读 = 读取材料范围(R);
            if (范围读.状态 != 分界状态::已完成) {
                if (尝试 > 0 && 范围读.状态 == 分界状态::未找到) break;
                throw 范围读.状态;
            }
            if (!范围读.信息) throw std::logic_error("boundary text range missing on success");
            auto& 范围 = *范围读.信息;
            const auto 来源快照 = 读快照(范围.定位.前置来源节点);
            if (!来源快照) continue;
            const auto 来源 = 解析分界来源(来源快照->结构);
            const auto 正文快照 = 读快照(来源.正文材料);
            if (!正文快照) continue;
            auto 正文 = 值服务.解码特征值快照(正文快照->结构);
            if (正文.状态 == 新特征值快照解码状态::未初始化) throw 分界状态::未初始化;
            if (正文.状态 != 新特征值快照解码状态::已解码 || !正文.信息
                || 正文.信息->物理类型 != 材料物理类型::UTF8字符串)
                throw std::logic_error("fixed boundary text material is not UTF8 text");
            auto 文本 = std::get<std::string>(std::move(正文.信息->材料));

            // 前置来源发表时已严格核验 UTF8，V 的正文保持不变。
            std::size_t 字节位置 = 0;
            std::size_t 起始字节 = 0;
            for (std::uint64_t 位置 = 0; 位置 < 范围.定位.结束标量; ++位置) {
                if (位置 == 范围.定位.起始标量) 起始字节 = 字节位置;
                if (字节位置 >= 文本.size())
                    throw std::logic_error("fixed boundary text does not cover its range");
                (void)读取已校验码点(文本, 字节位置);
            }
            auto 区间文本 = 文本.substr(起始字节, 字节位置 - 起始字节);
            结果.信息 = 新语言范围原文信息{std::move(范围), 来源.正文材料,
                std::move(文本), std::move(区间文本)};
            结果.状态 = 分界状态::已完成;
            return;
        }
        throw 分界状态::材料未就绪;
    });
}

新语言分界调用结果<新语言词条准入取材> 新_语言类::读取词条准入取材(
    稳定编码 R, 稳定编码 有效结果, const 新_特征值类& 值服务) const {
    return 调用分界材料入口<新语言词条准入取材>(L"读取词条准入取材", [&](auto& 结果) {
        const auto 原文 = 读取材料范围原文(R, 值服务);
        if (原文.状态 != 分界状态::已完成) throw 原文.状态;
        const auto 词段组 = [&] {
            std::lock_guard 锁(新语言互斥);
            if (!有效(组织句柄.front())) throw 分界状态::未初始化;
            return 取得有效分界词段已加锁(R, 有效结果);
        }();
        const auto& 定位 = 原文.信息->范围.定位;
        const auto& 文本 = 原文.信息->范围UTF8原文;
        新语言词条准入取材 材料{定位.语言节点, 定位.规范化合同节点, {}};
        材料.词段材料组.reserve(词段组.size());

        // 正式依据已经保证各词段连续覆盖同一范围。这里将标量位置换算成原文字节位置。
        auto 标量位置 = 定位.起始标量;
        std::size_t 字节位置 = 0;
        for (const auto& 词段 : 词段组) {
            const auto 起始字节 = 字节位置;
            while (标量位置 < 词段.结束标量) {
                if (字节位置 >= 文本.size())
                    throw std::logic_error("boundary admission segment exceeds fixed text");
                (void)读取已校验码点(文本, 字节位置);
                ++标量位置;
            }
            新语言词条准入词段材料 项{词段, {}};
            if (词段.用途 == 新语言分界段用途::词面 && !词段.词条)
                项.未登记原文词面 = 文本.substr(起始字节, 字节位置 - 起始字节);
            材料.词段材料组.push_back(std::move(项));
        }
        结果.信息 = std::move(材料);
        结果.状态 = 分界状态::已完成;
    });
}

新语言分界调用结果<新语言分界候选定义> 新_语言类::生成分词候选(
    稳定编码 R, const 新_特征值类& 值服务, const 新_词类& 词服务, std::stop_token 停止) const {
    return 调用分界材料入口<新语言分界候选定义>(L"生成分词候选", [&](auto& 结果) {
        if (停止.stop_requested()) throw 分界状态::材料未就绪;
        const auto 原文读 = 读取材料范围原文(R, 值服务);
        if (原文读.状态 != 分界状态::已完成) throw 原文读.状态;
        if (!原文读.信息) throw std::logic_error("boundary candidate text missing on success");
        const auto& 定位 = 原文读.信息->范围.定位;
        std::vector<本次词面匹配节点> 匹配树;
        switch (读取本次词面匹配树(定位.语言节点, 词服务, 匹配树)) {
        case 新语言查询状态::已找到: break;
        case 新语言查询状态::未找到: throw 分界状态::材料未就绪;
        case 新语言查询状态::资源失败: throw std::bad_alloc{};
        case 新语言查询状态::输入不合法: throw 分界输入拒绝{};
        default: throw std::logic_error("current lexical trie read failed");
        }
        const auto& 文本 = 原文读.信息->范围UTF8原文;
        std::vector<std::uint32_t> 码点组;
        for (std::size_t 字节位置 = 0; 字节位置 < 文本.size();)
            码点组.push_back(读取已校验码点(文本, 字节位置));

        struct 匹配边 final { std::size_t 结束; std::optional<稳定编码> 词条; };
        std::vector<std::vector<匹配边>> 后继组(码点组.size());
        for (std::size_t 起点 = 0; 起点 < 码点组.size(); ++起点) {
            if (停止.stop_requested()) throw 分界状态::材料未就绪;
            std::size_t 树位置 = 0;
            for (auto i = 起点; i < 码点组.size(); ++i) {
                const auto 后继 = 匹配树[树位置].后继.find(码点组[i]);
                if (后继 == 匹配树[树位置].后继.end()) break;
                树位置 = 后继->second;
                if (匹配树[树位置].词条节点)
                    后继组[起点].push_back({i + 1, 匹配树[树位置].词条节点});
            }
            if (后继组[起点].empty()) 后继组[起点].push_back({起点 + 1, {}});
        }

        新语言分界候选定义 定义{{构造未知原文路径(定位)}};
        struct 路径帧 final { std::size_t 起点, 下条边; };
        std::vector<路径帧> 路径栈{{0, 0}};
        std::vector<新语言分界词段> 当前词段;
        while (!路径栈.empty()) {
            if (停止.stop_requested()) throw 分界状态::材料未就绪;
            auto& 帧 = 路径栈.back();
            if (帧.起点 == 码点组.size()) {
                新语言分界候选路径 路径;
                for (const auto& 段 : 当前词段) {
                    if (!段.词条 && !路径.词段组.empty() && !路径.词段组.back().词条)
                        路径.词段组.back().结束标量 = 段.结束标量;
                    else
                        路径.词段组.push_back(段);
                }
                if (路径 != 定义.路径组.front()) 定义.路径组.push_back(std::move(路径));
            } else if (帧.下条边 < 后继组[帧.起点].size()) {
                const auto 起点 = 帧.起点;
                const auto& 边 = 后继组[起点][帧.下条边++];
                当前词段.push_back({定位.起始标量 + 起点, 定位.起始标量 + 边.结束, 边.词条});
                路径栈.push_back({边.结束, 0});
                continue;
            }
            路径栈.pop_back();
            if (!当前词段.empty()) 当前词段.pop_back();
        }
        结果.信息 = std::move(定义);
        结果.状态 = 分界状态::已完成;
    });
}

} // namespace 海中鱼巣
