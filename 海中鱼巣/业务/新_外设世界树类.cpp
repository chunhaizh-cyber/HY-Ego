#include "新_外设世界树类.h"
#include "../核心/容错检查.h"

#include <array>
#include <algorithm>
#include <chrono>
#include <limits>
#include <mutex>
#include <stdexcept>
#include <set>

namespace 海中鱼巣 {
namespace {
稳定编码 外设根{};
std::array<稳定编码, 6> 类型{};
std::array<稳定编码, 2> 消息类型{};
// 私有消息结构的字段键；消息不是普通存在，不能借用存在身份。
std::array<稳定编码, 12> 消息字段{};
稳定编码 消息根{};
const 新_特征值类* 消息值服务 = nullptr;
std::mutex 外设互斥;

基础联合节点快照 读取消息快照(稳定编码 节点) {
    const auto 结果 = 全局基础数据集.查询节点组完整结构({节点});
    if (结果.状态 != 基础结构操作状态::已完成 || 结果.节点组.size() != 1 || !结果.节点组[0].快照)
        throw std::runtime_error("peripheral message snapshot unavailable");
    return *结果.节点组[0].快照;
}
bool 消息快照相同(const 基础联合节点快照& 左, const 基础联合节点快照& 右) {
    return 左.结构.节点 == 右.结构.节点 && 左.结构.字段组 == 右.结构.字段组
        && 左.入向字段组 == 右.入向字段组 && 左.入向外部关系组 == 右.入向外部关系组
        && 左.出向外部关系组 == 右.出向外部关系组;
}
std::optional<新外设消息信息> 读取正式消息(
    稳定编码 节点, std::vector<基础联合节点快照>* 保护 = nullptr) {
    const auto 结构 = 全局基础数据集.查询完整节点结构(节点);
    if (结构.状态 == 基础结构操作状态::节点不存在) return std::nullopt;
    if (结构.状态 != 基础结构操作状态::已完成 || !结构.结构)
        throw std::runtime_error("peripheral message node read failed");
    // 其它节点和私有空节点不是消息；有消息类型却结构不全是工程故障。
    const auto 类型字段 = std::find_if(结构.结构->字段组.begin(), 结构.结构->字段组.end(),
        [](const auto& 字段) { return 字段.字段节点 == 消息字段[0]; });
    if (类型字段 == 结构.结构->字段组.end() || 类型字段->内容 != 基础字段内容{消息字段[11]})
        return std::nullopt;
    const auto 消息 = 读取消息快照(节点);
    const auto 根 = 读取消息快照(消息根);
    const auto 取 = [&](std::size_t 位置) -> const 基础字段内容& {
        const 基础字段关系* 命中 = nullptr;
        for (const auto& 字段 : 消息.结构.字段组) if (字段.字段节点 == 消息字段[位置]) {
            if (命中) throw std::runtime_error("peripheral message field duplicated");
            命中 = &字段;
        }
        if (!命中) throw std::runtime_error("peripheral message field missing");
        return 命中->内容;
    };
    if (消息.结构.字段组.size() != 10 || std::get<稳定编码>(取(0)) != 消息字段[11]
        || std::get<稳定编码>(取(1)) != 消息根)
        throw std::runtime_error("peripheral message identity corrupt");
    std::size_t 持有数 = 0;
    for (const auto& 字段 : 根.结构.字段组)
        if (字段.字段节点 == 消息字段[10] && 字段.内容 == 基础字段内容{节点}) ++持有数;
    if (持有数 != 1) throw std::runtime_error("peripheral message source ownership corrupt");
    新外设消息信息 结果;
    结果.消息节点 = 节点;
    结果.场景节点 = std::get<稳定编码>(取(2));
    结果.窗口节点 = std::get<稳定编码>(取(3));
    结果.来源.公开宿主会话 = std::get<std::string>(std::get<基础值>(取(4)).材料);
    结果.来源.对话框标识 = std::get<std::string>(std::get<基础值>(取(5)).材料);
    const auto 方向 = std::get<std::int64_t>(std::get<基础值>(取(6)).材料);
    const auto& 序号 = std::get<std::vector<std::uint64_t>>(std::get<基础值>(取(7)).材料);
    if ((方向 != 1 && 方向 != 2) || 序号.size() != 1 || !序号[0]
        || 结果.来源.公开宿主会话.empty() || 结果.来源.对话框标识.empty())
        throw std::runtime_error("peripheral message source invalid");
    结果.来源.方向 = static_cast<新外设消息方向>(方向);
    结果.来源.来源序号 = 序号[0];
    结果.正文材料节点 = std::get<稳定编码>(取(8));
    结果.正式记录时间 = {新状态时间语义::实例绝对UTC,
        std::get<std::int64_t>(std::get<基础值>(取(9)).材料)};
    const auto 正文 = 消息值服务 ? 消息值服务->获取特征值(结果.正文材料节点) : std::nullopt;
    const auto* 文本 = 正文 ? std::get_if<std::string>(&正文->材料) : nullptr;
    if (!文本 || 正文->物理类型 != 材料物理类型::UTF8字符串 || 文本->empty()
        || !新_语言类::严格UTF8字节串有效(*文本) || 结果.正式记录时间.纳秒 < 0)
        throw std::runtime_error("peripheral message body/time corrupt");
    结果.UTF8正文 = *文本;
    // 入口读取仅定位不可变端点；全部保护来自一次L1联合读取，不能把正常的
    // 其它消息追加导致的根变化当作消息损坏，也不跨批拼接保护快照。
    const auto 批 = 全局基础数据集.查询节点组完整结构(
        {节点, 消息根, 结果.场景节点, 结果.窗口节点, 结果.正文材料节点});
    if (批.状态 != 基础结构操作状态::已完成 || 批.节点组.size() != 5)
        throw std::runtime_error("peripheral message joint identity read failed");
    std::vector<基础联合节点快照> 快照;
    for (const auto& 项 : 批.节点组) {
        if (!项.快照) throw std::runtime_error("peripheral message identity endpoint disappeared");
        if (项.查询节点 == 节点 && 项.快照->结构.字段组 != 消息.结构.字段组)
            throw std::runtime_error("peripheral immutable message content changed");
        快照.push_back(*项.快照);
    }
    if (保护) 保护->insert(保护->end(), 快照.begin(), 快照.end());
    return 结果;
}
class 外设消息身份读取器 final : public 特征消息成员读取接口 {
public:
    特征消息成员读取结果 读取消息集合成员身份(稳定编码 节点) const noexcept override {
        try {
            std::vector<基础联合节点快照> 保护;
            const auto 消息 = 读取正式消息(节点, &保护);
            if (!消息) return {特征概念规则状态::入口拒绝, {}, {}};
            return {特征概念规则状态::已完成, 节点, std::move(保护)};
        } catch (const std::bad_alloc&) { return {特征概念规则状态::资源失败, {}, {}}; }
        catch (...) {
            工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取外设消息身份", L"正式消息结构损坏，不作为普通非消息结果。");
        }
    }
};
外设消息身份读取器 消息读取器;

std::optional<新外设消息信息> 按来源查找消息(const 新外设消息来源键& 来源) {
    std::optional<新外设消息信息> 结果;
    const auto 根 = 读取消息快照(消息根);
    for (const auto& 字段 : 根.结构.字段组) {
        if (字段.字段节点 != 消息字段[10]) throw std::runtime_error("peripheral message root corrupt");
        const auto 消息 = 读取正式消息(std::get<稳定编码>(字段.内容));
        if (!消息) throw std::runtime_error("peripheral message root holds invalid message");
        if (消息->来源 != 来源) continue;
        if (结果) throw std::runtime_error("peripheral message source duplicated");
        结果 = 消息;
    }
    if (!消息快照相同(根, 读取消息快照(消息根)))
        throw std::runtime_error("peripheral message index changed while reading");
    return 结果;
}

新状态强时间 当前强时间() {
    return {新状态时间语义::实例绝对UTC,
        std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count()};
}
bool 更新完成(新外设事实状态 状态) {
    return 状态 == 新外设事实状态::已完成 || 状态 == 新外设事实状态::无变化;
}
稳定编码 确保具名类型(新世界事实服务引用& 服务, 稳定编码 语言,
    const char* 名称, const 特征概念定义& 定义) {
    const auto 词 = 服务.词服务.建立或取得词条({语言, 严格UTF8字节精确规范化合同节点, 名称, std::nullopt});
    if (!词.词条节点 || (词.状态 != 新词条建立状态::已建立 && 词.状态 != 新词条建立状态::已复用))
        throw std::runtime_error("peripheral type word unavailable");
    const auto 候选 = 服务.名称关系服务.查询词条候选概念(*词.词条节点);
    if (候选.状态 != 概念名称关系查询状态::已找到 && 候选.状态 != 概念名称关系查询状态::未找到)
        throw std::runtime_error("peripheral named type lookup failed");
    稳定编码 概念{};
    for (const auto 关系节点 : 候选.名称关系节点组) {
        if (有效(概念)) throw std::runtime_error("peripheral type name ambiguous");
        const auto 项 = 服务.名称关系服务.获取名称关系(关系节点);
        if (项.状态 != 概念名称关系查询状态::已找到 || !项.信息)
            throw std::runtime_error("peripheral type name relation unreadable");
        概念 = 项.信息->概念节点;
    }
    if (!有效(概念)) {
        概念 = 服务.特征概念服务.建立或取得特征概念(定义, 服务.特征值服务);
        const auto 名 = 服务.名称关系服务.建立或取得名称关系(概念, *词.词条节点);
        if (!有效(概念) || !名.名称关系节点
            || (名.状态 != 概念名称关系建立状态::已建立 && 名.状态 != 概念名称关系建立状态::已复用))
            throw std::runtime_error("peripheral named type publication failed");
    }
    const auto 已有 = 服务.特征概念服务.获取特征概念(概念);
    if (!已有 || 已有->材料类型 != 定义.材料类型 || 已有->值域 != 定义.值域
        || 已有->单位 != 定义.单位 || 已有->比较规则 != 定义.比较规则
        || 已有->聚合规则 != 定义.聚合规则
        || 服务.特征概念服务.比较单特征概念(概念, 概念).特征类型根 != 概念)
        throw std::runtime_error("peripheral named type definition mismatch");
    return 概念;
}
}

新_外设世界树类::新_外设世界树类(新_世界树类& 世界) noexcept
    : 世界_(世界), 服务_(世界.获取事实服务引用()),
      场景_(外设根, 服务_.存在服务, 服务_.状态服务, 服务_.动态服务) {}

新世界初始化结果 新_外设世界树类::初始化() noexcept {
    try {
        std::lock_guard 锁(外设互斥);
        if (!世界_.已初始化()) return {新世界初始化状态::依赖初始化失败, {}, {}};
        for (auto& 字段 : 消息字段) {
            if (!有效(字段)) 字段 = 全局基础数据集.新建节点();
            if (!有效(字段)) throw std::runtime_error("peripheral message schema allocation failed");
        }
        if (!有效(消息根)) 消息根 = 全局基础数据集.新建节点();
        if (!有效(消息根)) throw std::runtime_error("peripheral message root allocation failed");
        if (消息值服务 && 消息值服务 != &服务_.特征值服务)
            throw std::runtime_error("peripheral message value owner changed");
        消息值服务 = &服务_.特征值服务;
        服务_.特征概念服务.装配消息成员读取接口(&消息读取器);
        const bool 已有 = 有效(外设根);
        if (!场景_.初始化()) throw std::runtime_error("peripheral scene initialization failed");
        const auto 根 = 场景_.建立根场景();
        if (!根.场景节点 || !根.专属存在概念节点
            || (根.状态 != 新场景操作状态::已完成 && 根.状态 != 新场景操作状态::无变化)
            || 世界_.获取世界树根场景() == 根.场景节点)
            throw std::runtime_error("peripheral root invalid");
        const auto 语言 = 服务_.语言服务.建立或取得语言({"zh-CN", std::nullopt});
        if (!语言.语言节点) throw std::runtime_error("peripheral language unavailable");
        特征概念定义 文本;
        文本.材料类型 = 特征概念材料物理类型::UTF8字符串;
        文本.值域 = 特征概念全材料值域{};
        文本.比较规则 = 特征概念比较规则::完整材料精确;
        文本.聚合规则 = 特征概念聚合规则::完整材料精确集合;
        特征概念定义 数量;
        数量.值域 = std::vector<特征概念I64闭区间>{{0, (std::numeric_limits<std::int64_t>::max)()}};
        数量.聚合规则 = 特征概念聚合规则::I64连续值归组;
        const std::array<const char*, 6> 名称{"外设对话框公开会话", "外设对话框标识",
            "外设对话框接收数量", "外设对话框接收内容", "外设对话框发送数量", "外设对话框发送内容"};
        for (std::size_t 序号 = 0; 序号 < 类型.size(); ++序号) {
            const auto 节点 = 确保具名类型(服务_, *语言.语言节点, 名称[序号], 序号 == 2 || 序号 == 4 ? 数量 : 文本);
            if (有效(类型[序号]) && 类型[序号] != 节点) throw std::runtime_error("peripheral type identity changed");
            类型[序号] = 节点;
            for (std::size_t 前序号 = 0; 前序号 < 序号; ++前序号)
                if (类型[序号] == 类型[前序号]) throw std::runtime_error("peripheral distinct types collapsed");
        }
        特征概念定义 消息集合;
        消息集合.材料类型 = 特征概念材料物理类型::U64数组;
        消息集合.值域 = 特征概念全材料值域{};
        消息集合.比较规则 = 特征概念比较规则::消息集合身份判等;
        消息集合.聚合规则 = 特征概念聚合规则::完整材料精确集合;
        for (std::size_t 序号 = 0; 序号 < 消息类型.size(); ++序号) {
            const auto 节点 = 确保具名类型(服务_, *语言.语言节点,
                序号 == 0 ? "外设对话框接收消息" : "外设对话框发送消息", 消息集合);
            if (有效(消息类型[序号]) && 消息类型[序号] != 节点)
                throw std::runtime_error("peripheral message feature type changed");
            消息类型[序号] = 节点;
        }
        return {已有 ? 新世界初始化状态::无变化 : 新世界初始化状态::已完成,
            根.场景节点, 根.专属存在概念节点};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化外设世界", L"根或正式特征类型不完整。");
    }
}

稳定编码 新_外设世界树类::读取类型特征(const std::vector<稳定编码>& 特征组, 稳定编码 类型根) const {
    稳定编码 命中{};
    for (const auto 节点 : 特征组) {
        const auto 特征材料 = 服务_.特征服务.获取特征(节点);
        if (!特征材料) throw std::runtime_error("peripheral feature unreadable");
        const auto 操作结果 = 服务_.特征概念服务.比较单特征概念(特征材料->特征概念节点, 类型根);
        if (操作结果.状态 != 特征概念规则状态::已完成)
            throw std::runtime_error("peripheral feature type unreadable");
        if (操作结果.特征类型根 != 类型根) continue;
        if (有效(命中)) throw std::runtime_error("peripheral duplicate feature type");
        命中 = 节点;
    }
    if (!有效(命中)) throw std::runtime_error("peripheral required feature missing");
    return 命中;
}

std::string 新_外设世界树类::读取文本特征(稳定编码 特征) const {
    const auto 特征材料 = 服务_.特征服务.获取特征(特征);
    const auto* 节点 = 特征材料 && 特征材料->当前值 ? std::get_if<稳定编码>(&*特征材料->当前值) : nullptr;
    const auto 值材料 = 节点 ? 服务_.特征值服务.获取特征值(*节点) : std::nullopt;
    const auto* 文本 = 值材料 ? std::get_if<std::string>(&值材料->材料) : nullptr;
    if (!文本 || 值材料->物理类型 != 材料物理类型::UTF8字符串)
        throw std::runtime_error("peripheral text current value invalid");
    return *文本;
}

新外设对话框定位 新_外设世界树类::读取窗口定位(稳定编码 场景节点) const {
    const auto 场景材料 = 场景_.获取场景(场景节点);
    if (!场景材料 || 场景材料->父场景节点 != 外设根 || 场景材料->直接成员存在组.size() != 1
        || !场景材料->直接子场景组.empty() || 场景材料->特征节点组.size() != 2)
        throw std::runtime_error("peripheral dialogue scene incomplete");
    (void)读取文本特征(读取类型特征(场景材料->特征节点组, 类型[0]));
    (void)读取文本特征(读取类型特征(场景材料->特征节点组, 类型[1]));
    const auto 存在材料 = 服务_.存在服务.获取存在(场景材料->直接成员存在组[0]);
    if (!存在材料 || 存在材料->父存在节点
        || 存在材料->特征节点组.size() < 2 || 存在材料->特征节点组.size() > 6
        || !存在材料->子存在节点组.empty())
        throw std::runtime_error("peripheral window structure incomplete");
    新外设对话框定位 结果{场景节点, 存在材料->节点, 读取类型特征(存在材料->特征节点组, 类型[2]),
        读取类型特征(存在材料->特征节点组, 类型[3])};
    std::set<稳定编码> 已有类型;
    for (const auto 特征节点 : 存在材料->特征节点组) {
        const auto 特征 = 服务_.特征服务.获取特征(特征节点);
        const auto 根 = 特征 ? 服务_.特征概念服务.比较单特征概念(
            特征->特征概念节点, 特征->特征概念节点).特征类型根 : std::nullopt;
        if (!根) throw std::runtime_error("peripheral window feature type unreadable");
        if (!已有类型.insert(*根).second) throw std::runtime_error("peripheral window feature type duplicated");
        if (*根 == 类型[4]) 结果.发送数量特征 = 特征节点;
        else if (*根 == 类型[5]) 结果.发送内容特征 = 特征节点;
        else if (*根 == 消息类型[0]) 结果.接收消息特征 = 特征节点;
        else if (*根 == 消息类型[1]) 结果.发送消息特征 = 特征节点;
        else if (*根 != 类型[2] && *根 != 类型[3])
            throw std::runtime_error("peripheral window unexpected feature type");
    }
    if (结果.发送数量特征.has_value() != 结果.发送内容特征.has_value())
        throw std::runtime_error("peripheral window outbound feature pair incomplete");
    return 结果;
}

新外设对话框定位 新_外设世界树类::补齐消息集合(const 新外设对话框定位& 定位) {
    const auto 空材料 = 服务_.特征值服务.保存或取得特征值(
        材料物理类型::U64数组, std::vector<std::uint64_t>{});
    if (!有效(空材料)) throw std::runtime_error("peripheral empty message material unavailable");
    for (std::size_t 序号 = 0; 序号 < 2; ++序号) {
        if (序号 == 0 ? 定位.接收消息特征.has_value() : 定位.发送消息特征.has_value()) continue;
        const auto 添加 = 场景_.添加场景内存在特征(定位.场景节点, 定位.窗口节点,
            消息类型[序号], 空材料, 当前强时间());
        if (添加.状态 != 新场景操作状态::已完成)
            throw std::runtime_error("peripheral message feature initialization failed");
    }
    return 读取窗口定位(定位.场景节点);
}

std::optional<新外设对话框定位> 新_外设世界树类::查找正式窗口(
    std::string_view 公开宿主会话, std::string_view 对话框标识) const {
    const auto 根 = 场景_.获取场景(外设根);
    if (!根) throw std::runtime_error("peripheral root unreadable");
    std::optional<新外设对话框定位> 命中;
    for (const auto 节点 : 根->直接子场景组) {
        const auto 定位 = 读取窗口定位(节点);
        const auto 场景材料 = 场景_.获取场景(节点);
        if (!场景材料) throw std::runtime_error("peripheral scene disappeared");
        if (读取文本特征(读取类型特征(场景材料->特征节点组, 类型[0])) != 公开宿主会话
            || 读取文本特征(读取类型特征(场景材料->特征节点组, 类型[1])) != 对话框标识) continue;
        if (命中) throw std::runtime_error("peripheral dialogue identity duplicated");
        命中 = 定位;
    }
    return 命中;
}

std::optional<新外设对话框来源> 新_外设世界树类::读取对话框来源(
    稳定编码 窗口节点) const noexcept {
    if (!有效(窗口节点)) return std::nullopt;
    try {
        std::lock_guard 锁(外设互斥);
        if (!有效(外设根)) return std::nullopt;
        const auto 根 = 场景_.获取场景(外设根);
        if (!根) throw std::runtime_error("peripheral root unreadable");
        std::optional<新外设对话框来源> 命中;
        for (const auto 场景节点 : 根->直接子场景组) {
            const auto 定位 = 读取窗口定位(场景节点);
            if (定位.窗口节点 != 窗口节点) continue;
            const auto 场景材料 = 场景_.获取场景(场景节点);
            if (!场景材料) throw std::runtime_error("peripheral scene unreadable");
            if (命中) throw std::runtime_error("peripheral window identity duplicated");
            命中 = 新外设对话框来源{定位,
                读取文本特征(读取类型特征(场景材料->特征节点组, 类型[0])),
                读取文本特征(读取类型特征(场景材料->特征节点组, 类型[1]))};
        }
        return 命中;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取外设对话框来源", L"正式窗口定位结构不完整。");
    }
}

新外设对话框建立结果 新_外设世界树类::查找对话框场景(
    std::string_view 公开宿主会话, std::string_view 对话框标识) const noexcept {
    if (公开宿主会话.empty() || 对话框标识.empty()) return {};
    try {
        if (!新_语言类::严格UTF8字节串有效(std::string(公开宿主会话))
            || !新_语言类::严格UTF8字节串有效(std::string(对话框标识))) return {};
        std::lock_guard 锁(外设互斥);
        if (!有效(外设根)) return {新外设事实状态::未初始化, {}};
        const auto 命中 = 查找正式窗口(公开宿主会话, 对话框标识);
        return {命中 ? 新外设事实状态::已完成 : 新外设事实状态::未找到, 命中};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查询外设对话框", L"正式定位或窗口结构不完整，不伪装未找到。");
    }
}

新外设对话框建立结果 新_外设世界树类::建立或取得对话框场景(
    std::string_view 公开宿主会话, std::string_view 对话框标识) noexcept {
    if (公开宿主会话.empty() || 对话框标识.empty()) return {};
    try {
        std::lock_guard 锁(外设互斥);
        if (!有效(外设根)) return {新外设事实状态::未初始化, {}};
        const auto 命中 = 查找正式窗口(公开宿主会话, 对话框标识);
        if (命中) return {新外设事实状态::无变化, 补齐消息集合(*命中)};
        const auto 场景材料 = 场景_.建立场景(外设根);
        if (场景材料.状态 != 新场景操作状态::已完成 || !场景材料.场景节点)
            throw std::runtime_error("peripheral dialogue creation failed");
        const auto 时间 = 当前强时间();
        const std::array<std::string_view, 2> 值{公开宿主会话, 对话框标识};
        for (std::size_t 序号 = 0; 序号 < 值.size(); ++序号) {
            const auto 值材料 = 服务_.特征值服务.保存或取得特征值(材料物理类型::UTF8字符串, std::string(值[序号]));
            if (!有效(值材料) || 场景_.添加场景特征(*场景材料.场景节点, 类型[序号], 值材料, 时间).状态 != 新场景操作状态::已完成)
                throw std::runtime_error("peripheral scene identity publication failed");
        }
        新存在建立请求 请求;
        请求.初始特征组.push_back({类型[2], std::int64_t{0}});
        请求.初始特征组.push_back({类型[4], std::int64_t{0}});
        const auto 存在材料 = 场景_.建立成员存在(*场景材料.场景节点, 请求, 时间);
        if (存在材料.状态 != 新场景操作状态::已完成 || !存在材料.存在节点)
            throw std::runtime_error("peripheral window creation failed");
        const auto 特征材料 = 场景_.添加场景内存在特征区间值(*场景材料.场景节点, *存在材料.存在节点, 类型[3], 特征概念全材料值域{});
        if (特征材料.状态 != 新场景操作状态::已完成)
            throw std::runtime_error("peripheral unknown text feature creation failed");
        const auto 发送内容 = 场景_.添加场景内存在特征区间值(*场景材料.场景节点,
            *存在材料.存在节点, 类型[5], 特征概念全材料值域{});
        if (发送内容.状态 != 新场景操作状态::已完成)
            throw std::runtime_error("peripheral unknown outbound text feature creation failed");
        return {新外设事实状态::已完成, 补齐消息集合(读取窗口定位(*场景材料.场景节点))};
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"取得外设对话框", L"定位或场景成员不完整，保留现场。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"取得外设对话框", L"定位或场景成员不完整，保留现场。");
    }
}

std::optional<新状态信息> 新_外设世界树类::读取最近特征状态(
    稳定编码 场景节点, 稳定编码 存在节点, 稳定编码 特征节点) const {
    const auto 特征 = 服务_.特征服务.获取特征(特征节点);
    if (!特征) throw std::runtime_error("peripheral previous feature missing");
    const auto 类型定位 = 服务_.特征概念服务.比较单特征概念(特征->特征概念节点, 特征->特征概念节点);
    if (类型定位.状态 != 特征概念规则状态::已完成 || !类型定位.特征类型根)
        throw std::runtime_error("peripheral previous feature root invalid");
    std::optional<新状态信息> 最近;
    for (const auto 节点 : 场景_.查询状态列表(场景节点)) {
        const auto 状态 = 服务_.状态服务.获取状态(节点);
        if (!状态) throw std::runtime_error("peripheral scene state unreadable");
        if (状态->被描述存在节点 != 存在节点 || 状态->来源实例特征节点 != 特征节点) continue;
        const auto 状态类型 = 服务_.特征概念服务.比较单特征概念(状态->特征类型概念节点, 状态->特征类型概念节点);
        if (状态类型.状态 != 特征概念规则状态::已完成 || 状态类型.特征类型根 != 类型定位.特征类型根
            || 状态->强时间.语义 != 新状态时间语义::实例绝对UTC || 状态->强时间.纳秒 < 0)
            throw std::runtime_error("peripheral previous state invalid");
        if (最近 && 最近->强时间.纳秒 == 状态->强时间.纳秒 && 最近->节点 != 状态->节点)
            throw std::runtime_error("peripheral previous state ambiguous");
        if (!最近 || 最近->强时间.纳秒 < 状态->强时间.纳秒) 最近 = 状态;
    }
    return 最近;
}

void 新_外设世界树类::核验并处理已发布动态(稳定编码 场景节点, 稳定编码 动态节点) {
    const auto 前 = 服务_.动态服务.获取动态(动态节点);
    const auto 列表 = 场景_.查询动态列表(场景节点);
    if (!前 || 前->种类 != 新动态种类::原子 || !前->动态概念节点
        || !服务_.动态概念服务.获取动态概念(*前->动态概念节点)
        || std::count(列表.begin(), 列表.end(), 动态节点) != 1)
        throw std::runtime_error("peripheral published dynamic invalid");
    const auto 处理 = 世界_.处理已登记动态概念(场景_, 场景节点, 动态节点);
    if (处理.状态 != 新动态概念自动抽象状态::已采用已有概念
        && 处理.状态 != 新动态概念自动抽象状态::已形成并采用上位概念
        && 处理.状态 != 新动态概念自动抽象状态::已是聚合概念
        && 处理.状态 != 新动态概念自动抽象状态::无同结构候选)
        throw std::runtime_error("peripheral dynamic abstraction incomplete");
    const auto 后 = 服务_.动态服务.获取动态(动态节点);
    const auto 后列表 = 场景_.查询动态列表(场景节点);
    if (!后 || 后->种类 != 前->种类 || 后->主题存在节点 != 前->主题存在节点
        || 后->状态节点组 != 前->状态节点组 || !后->动态概念节点
        || !服务_.动态概念服务.获取动态概念(*后->动态概念节点)
        || std::count(后列表.begin(), 后列表.end(), 动态节点) != 1)
        throw std::runtime_error("peripheral dynamic abstraction readback invalid");
}

新外设事实状态 新_外设世界树类::更新窗口特征并形成动态(
    const 新外设对话框定位& 定位, 稳定编码 特征节点,
    const 新特征准确值& 新值, const 新状态强时间& 时间) {
    if (!场景_.是场景节点(定位.场景节点) || 读取窗口定位(定位.场景节点) != 定位
        || (特征节点 != 定位.接收内容特征 && 特征节点 != 定位.接收数量特征
            && 定位.发送数量特征 != 特征节点 && 定位.发送内容特征 != 特征节点)
        || 时间.语义 != 新状态时间语义::实例绝对UTC || 时间.纳秒 < 0)
        throw std::runtime_error("peripheral feature publication input invalid");
    const auto 特征 = 服务_.特征服务.获取特征(特征节点);
    if (!特征) throw std::runtime_error("peripheral feature publication missing feature");
    const bool 同值 = 特征->当前值 && *特征->当前值 == 新值;
    const auto 前 = 读取最近特征状态(定位.场景节点, 定位.窗口节点, 特征节点);
    if ((前 && (!特征->当前值 || 前->固定准确值 != *特征->当前值))
        || (!前 && 特征->当前值) || (!同值 && 前 && 时间.纳秒 <= 前->强时间.纳秒))
        throw std::runtime_error("peripheral state/current value or time mismatch");
    const auto 更新 = 场景_.更新场景内存在特征值(定位.场景节点, 定位.窗口节点, 特征节点, 新值, 时间);
    if (同值) {
        if (更新.状态 != 新场景操作状态::无变化 || 更新.状态节点)
            throw std::runtime_error("peripheral unchanged value unexpectedly published state");
        return 新外设事实状态::无变化;
    }
    if (更新.状态 != 新场景操作状态::已完成 || !更新.状态节点)
        throw std::runtime_error("peripheral feature update incomplete");
    const auto 后 = 服务_.状态服务.获取状态(*更新.状态节点);
    const auto 列表 = 场景_.查询状态列表(定位.场景节点);
    const auto 类型定位 = 服务_.特征概念服务.比较单特征概念(特征->特征概念节点, 特征->特征概念节点);
    const auto 后类型 = 后 ? 服务_.特征概念服务.比较单特征概念(后->特征类型概念节点, 后->特征类型概念节点)
        : 单特征概念关系结果{};
    if (!后 || 后->被描述存在节点 != 定位.窗口节点 || 后->来源实例特征节点 != 特征节点
        || 后->固定准确值 != 新值 || 后->强时间.语义 != 时间.语义 || 后->强时间.纳秒 != 时间.纳秒
        || 后类型.状态 != 特征概念规则状态::已完成 || 后类型.特征类型根 != 类型定位.特征类型根
        || std::count(列表.begin(), 列表.end(), 后->节点) != 1)
        throw std::runtime_error("peripheral new state readback invalid");
    if (!前) return 新外设事实状态::已完成;
    const auto 动态 = 服务_.动态服务.建立原子动态(定位.窗口节点, 前->节点, 后->节点);
    if (动态.状态 != 新动态操作状态::已完成 || !动态.动态节点
        || 场景_.添加动态(定位.场景节点, *动态.动态节点) != 新场景操作状态::已完成)
        throw std::runtime_error("peripheral dynamic publication incomplete");
    核验并处理已发布动态(定位.场景节点, *动态.动态节点);
    return 新外设事实状态::已完成;
}

std::vector<稳定编码> 新_外设世界树类::读取消息成员(稳定编码 特征节点) const {
    const auto 特征 = 服务_.特征服务.获取特征(特征节点);
    if (!特征 || !特征->当前值) throw std::runtime_error("peripheral message feature lacks current value");
    std::vector<稳定编码> 成员;
    if (服务_.特征概念服务.读取消息集合成员(*特征->当前值, 服务_.特征值服务, 成员)
        != 特征概念规则状态::已完成)
        throw std::runtime_error("peripheral message collection invalid");
    return 成员;
}

void 新_外设世界树类::完成消息集合变化(const 新外设对话框定位& 定位,
    稳定编码 特征节点, 稳定编码 新材料, const 新特征值添加结果& 特征结果,
    const std::optional<新状态信息>& 前, const 新状态强时间& 时间) {
    const auto 特征 = 服务_.特征服务.获取特征(特征节点);
    const auto 根 = 特征 ? 服务_.特征概念服务.比较单特征概念(
        特征->特征概念节点, 特征->特征概念节点).特征类型根 : std::nullopt;
    if (!根 || !前 || 时间.语义 != 新状态时间语义::实例绝对UTC
        || 时间.纳秒 <= 前->强时间.纳秒)
        throw std::runtime_error("peripheral message state predecessor/time invalid");
    const auto 更新 = 服务_.存在服务.完成已提交引用集合特征变化(
        定位.窗口节点, 特征节点, 特征结果, *根, 新材料, 时间);
    if (更新.状态 != 新存在操作状态::已完成 || !更新.状态结果 || !更新.状态结果->状态节点)
        throw std::runtime_error("peripheral message existence/state completion failed");
    const auto 后节点 = *更新.状态结果->状态节点;
    if (场景_.添加状态(定位.场景节点, 后节点) != 新场景操作状态::已完成)
        throw std::runtime_error("peripheral message state attachment failed");
    const auto 后 = 服务_.状态服务.获取状态(后节点);
    if (!后 || 后->被描述存在节点 != 定位.窗口节点 || 后->来源实例特征节点 != 特征节点
        || 后->固定准确值 != 新特征准确值{新材料} || 后->强时间 != 时间)
        throw std::runtime_error("peripheral message state readback failed");
    const auto 动态 = 服务_.动态服务.建立原子动态(定位.窗口节点, 前->节点, 后节点);
    if (动态.状态 != 新动态操作状态::已完成 || !动态.动态节点
        || 场景_.添加动态(定位.场景节点, *动态.动态节点) != 新场景操作状态::已完成)
        throw std::runtime_error("peripheral message dynamic completion failed");
    核验并处理已发布动态(定位.场景节点, *动态.动态节点);
}

新外设事实状态 新_外设世界树类::记录消息(const 新外设对话框定位& 定位,
    新外设消息方向 方向, std::uint64_t 来源序号, std::string_view 正文, const 新状态强时间& 时间) {
    if (!来源序号 || 时间.语义 != 新状态时间语义::实例绝对UTC || 时间.纳秒 < 0
        || !新_语言类::严格UTF8字节串有效(std::string(正文)))
        throw std::runtime_error("peripheral message source/time invalid");
    if (读取窗口定位(定位.场景节点) != 定位 || !定位.接收消息特征 || !定位.发送消息特征)
        throw std::runtime_error("peripheral message location changed/incomplete");
    const auto 场景 = 场景_.获取场景(定位.场景节点);
    if (!场景) throw std::runtime_error("peripheral message scene disappeared");
    新外设消息来源键 来源{读取文本特征(读取类型特征(场景->特征节点组, 类型[0])),
        读取文本特征(读取类型特征(场景->特征节点组, 类型[1])), 方向, 来源序号};
    if (const auto 旧 = 按来源查找消息(来源)) {
        if (旧->场景节点 != 定位.场景节点 || 旧->窗口节点 != 定位.窗口节点 || 旧->UTF8正文 != 正文)
            throw std::runtime_error("peripheral repeated message source has conflicting material");
        return 新外设事实状态::无变化;
    }
    const auto 特征节点 = 方向 == 新外设消息方向::接收 ? *定位.接收消息特征 : *定位.发送消息特征;
    const auto 前 = 读取最近特征状态(定位.场景节点, 定位.窗口节点, 特征节点);
    const auto 原特征 = 服务_.特征服务.获取特征(特征节点);
    if (!前 || !原特征 || 原特征->当前值 != std::optional<新特征准确值>{前->固定准确值}
        || 时间.纳秒 <= 前->强时间.纳秒)
        throw std::runtime_error("peripheral message predecessor invalid");
    auto 成员 = 读取消息成员(特征节点);
    const auto 正文材料 = 服务_.特征值服务.保存或取得特征值(材料物理类型::UTF8字符串, std::string(正文));
    const auto 裸节点 = 全局基础数据集.新建节点();
    if (!有效(正文材料) || !有效(裸节点)) throw std::runtime_error("peripheral message allocation failed");
    成员.push_back(裸节点);
    std::sort(成员.begin(), 成员.end());
    std::vector<std::uint64_t> 编码组;
    for (const auto 编码 : 成员) 编码组.push_back(编码.值);
    const auto 集合材料 = 服务_.特征值服务.保存或取得特征值(材料物理类型::U64数组, 编码组);
    if (!有效(集合材料)) throw std::runtime_error("peripheral message collection allocation failed");
    基础节点组追加请求 请求;
    for (const auto 节点 : {消息根, 裸节点, 定位.场景节点, 定位.窗口节点, 正文材料, 集合材料})
        请求.预期联合节点组.push_back(读取消息快照(节点));
    for (const auto 身份特征 : 场景->特征节点组) {
        请求.预期联合节点组.push_back(读取消息快照(身份特征));
        const auto 信息 = 服务_.特征服务.获取特征(身份特征);
        const auto* 当前材料 = 信息 && 信息->当前值 ? std::get_if<稳定编码>(&*信息->当前值) : nullptr;
        if (!当前材料) throw std::runtime_error("peripheral source identity feature changed");
        请求.预期联合节点组.push_back(读取消息快照(*当前材料));
    }
    if (读取文本特征(读取类型特征(场景->特征节点组, 类型[0])) != 来源.公开宿主会话
        || 读取文本特征(读取类型特征(场景->特征节点组, 类型[1])) != 来源.对话框标识)
        throw std::runtime_error("peripheral source identity changed before sealing");
    const auto& 裸 = 请求.预期联合节点组[1];
    if (!裸.结构.字段组.empty() || !裸.入向字段组.empty()
        || !裸.入向外部关系组.empty() || !裸.出向外部关系组.empty())
        throw std::runtime_error("peripheral private message node unexpectedly visible");
    const std::array<基础组内字段内容, 10> 值{消息字段[11], 消息根, 定位.场景节点, 定位.窗口节点,
        基础值{基础原始值{来源.公开宿主会话}}, 基础值{基础原始值{来源.对话框标识}},
        基础值{基础原始值{static_cast<std::int64_t>(方向)}},
        基础值{基础原始值{std::vector<std::uint64_t>{来源序号}}}, 正文材料,
        基础值{基础原始值{时间.纳秒}}};
    for (std::size_t 序号 = 0; 序号 < 值.size(); ++序号)
        请求.追加字段组.push_back({裸节点, 消息字段[序号], 值[序号]});
    请求.追加字段组.push_back({消息根, 消息字段[10], 裸节点});
    const auto 保护字段 = 服务_.特征概念服务.消息集合成员保护字段();
    if (!有效(保护字段)) throw std::runtime_error("peripheral message protection key unavailable");
    for (const auto 编码 : 成员) 请求.追加字段组.push_back({集合材料, 保护字段, 编码});
    const auto 封存 = 全局基础数据集.封存原子追加片段(请求);
    if (封存.状态 != 基础节点组追加状态::已完成 || !封存.片段)
        throw std::runtime_error("peripheral message identity seal failed");
    新消息待发表凭据 凭据{裸节点, 请求.预期联合节点组, *封存.片段};
    auto 准备 = 服务_.特征服务.准备顶层消息集合更新(特征节点, 集合材料, std::move(凭据));
    if (准备.状态 != 新特征I64准备状态::已准备 || !准备.准备)
        throw std::runtime_error("peripheral message feature preparation failed");
    // 从取得F占用到完成S/D不重新读取/调用宿主；定位及来源快照也参与唯一CAS。
    const auto 提交 = 服务_.特征服务.提交顶层引用集合更新(*准备.准备, {});
    if (提交.状态 != 基础节点组追加状态::已完成) {
        服务_.特征服务.取消顶层引用集合更新(*准备.准备);
        // 零发布已证明，仍只按原来源读回；不重新发送或分配第二业务事件。
        if (const auto 已有 = 按来源查找消息(来源);
            已有 && 已有->消息节点 == 裸节点 && 已有->UTF8正文 == 正文)
            throw std::runtime_error("peripheral message receipt contradicts formal publication");
        if (!全局基础数据集.删除节点(裸节点))
            throw std::runtime_error("peripheral unpublished message cleanup failed");
        throw std::runtime_error("peripheral message atomic publication rejected");
    }
    const auto 特征结果 = 服务_.特征服务.完成顶层引用集合更新(*准备.准备);
    if (特征结果.状态 != 新特征写入状态::已完成 || !特征结果.当前值已改变)
        throw std::runtime_error("peripheral message feature completion failed");
    完成消息集合变化(定位, 特征节点, 集合材料, 特征结果, 前, 时间);
    const auto 正式 = 读取正式消息(裸节点);
    if (!正式 || 正式->来源 != 来源 || 正式->UTF8正文 != 正文 || 正式->正式记录时间 != 时间
        || 读取消息成员(特征节点) != 成员)
        throw std::runtime_error("peripheral message publication readback failed");
    服务_.特征服务.结束顶层引用集合更新(*准备.准备);
    return 新外设事实状态::已完成;
}

新外设事实状态 新_外设世界树类::更新接收(const 新外设对话框定位& 定位,
    std::uint64_t 来源序号, std::string_view 实际UTF8正文, const 新状态强时间& 接收时间) noexcept {
    if (实际UTF8正文.empty()) return 新外设事实状态::入口拒绝;
    try {
        std::lock_guard 锁(外设互斥);
        if (!有效(外设根)) return 新外设事实状态::未初始化;
        if (读取窗口定位(定位.场景节点) != 定位)
            throw std::runtime_error("peripheral receive location changed");
        const auto 消息结果 = 记录消息(定位, 新外设消息方向::接收, 来源序号, 实际UTF8正文, 接收时间);
        if (消息结果 == 新外设事实状态::无变化) return 消息结果;
        const auto 特征材料 = 服务_.特征服务.获取特征(定位.接收数量特征);
        const auto* 数量 = 特征材料 && 特征材料->当前值 ? std::get_if<std::int64_t>(&*特征材料->当前值) : nullptr;
        if (!数量 || *数量 < 0 || *数量 == (std::numeric_limits<std::int64_t>::max)())
            throw std::runtime_error("peripheral receive count invalid");
        const auto 值材料 = 服务_.特征值服务.保存或取得特征值(材料物理类型::UTF8字符串, std::string(实际UTF8正文));
        if (!有效(值材料) || !更新完成(更新窗口特征并形成动态(定位,
                定位.接收内容特征, 值材料, 接收时间))
            || !更新完成(更新窗口特征并形成动态(定位,
                定位.接收数量特征, *数量 + 1, 接收时间)))
            throw std::runtime_error("peripheral receive publication incomplete");
        const auto 后 = 服务_.特征服务.获取特征(定位.接收数量特征);
        const auto 当前内容 = 读取文本特征(定位.接收内容特征);
        if (!后 || !后->当前值 || *后->当前值 != 新特征准确值{*数量 + 1}
            || 当前内容 != 实际UTF8正文)
            throw std::runtime_error("peripheral receive readback failed");
        return 新外设事实状态::已完成;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"记录外设接收",
            L"实际接收后特征发布或读回失败，保留已发生事实，不作为可重发否定。");
    }
}
新外设事实状态 新_外设世界树类::更新发送(const 新外设对话框定位& 定位,
    std::uint64_t 来源序号, std::string_view 实际UTF8正文, const 新状态强时间& 发布时间) noexcept {
    if (实际UTF8正文.empty()) return 新外设事实状态::入口拒绝;
    try {
        if (!新_语言类::严格UTF8字节串有效(std::string(实际UTF8正文)))
            return 新外设事实状态::入口拒绝;
        std::lock_guard 锁(外设互斥);
        if (!有效(外设根)) return 新外设事实状态::未初始化;
        if (读取窗口定位(定位.场景节点) != 定位 || !定位.发送数量特征 || !定位.发送内容特征)
            throw std::runtime_error("outbound current page missing formal send features");
        const auto 消息结果 = 记录消息(定位, 新外设消息方向::发送, 来源序号, 实际UTF8正文, 发布时间);
        if (消息结果 == 新外设事实状态::无变化) return 消息结果;
        const auto 特征 = 服务_.特征服务.获取特征(*定位.发送数量特征);
        const auto* 数量 = 特征 && 特征->当前值 ? std::get_if<std::int64_t>(&*特征->当前值) : nullptr;
        if (!数量 || *数量 < 0 || *数量 == (std::numeric_limits<std::int64_t>::max)())
            throw std::runtime_error("outbound send count invalid");
        const auto 值 = 服务_.特征值服务.保存或取得特征值(材料物理类型::UTF8字符串, std::string(实际UTF8正文));
        if (!有效(值) || !更新完成(更新窗口特征并形成动态(定位, *定位.发送内容特征, 值, 发布时间))
            || !更新完成(更新窗口特征并形成动态(定位, *定位.发送数量特征, *数量 + 1, 发布时间)))
            throw std::runtime_error("outbound formal publication incomplete");
        const auto 后 = 服务_.特征服务.获取特征(*定位.发送数量特征);
        if (!后 || !后->当前值 || *后->当前值 != 新特征准确值{*数量 + 1}
            || 读取文本特征(*定位.发送内容特征) != 实际UTF8正文)
            throw std::runtime_error("outbound formal publication readback failed");
        return 新外设事实状态::已完成;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"记录外设发送",
            L"完整写出已发生，正式发送事实发布失败，保留现场，不重发补记录。");
    }
}

std::optional<新外设消息信息> 新_外设世界树类::读取消息(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(外设互斥);
        if (!有效(消息根) || !有效(节点)) return std::nullopt;
        return 读取正式消息(节点);
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取外设消息", L"消息读取失败，不作为未找到处理。");
    }
}

std::optional<新外设消息信息> 新_外设世界树类::查找消息(const 新外设消息来源键& 来源) const noexcept {
    try {
        std::lock_guard 锁(外设互斥);
        if (!有效(消息根) || !来源.来源序号 || 来源.公开宿主会话.empty() || 来源.对话框标识.empty())
            return std::nullopt;
        if (来源.方向 != 新外设消息方向::接收 && 来源.方向 != 新外设消息方向::发送)
            throw std::runtime_error("peripheral message query direction invalid");
        return 按来源查找消息(来源);
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"查找外设消息", L"来源索引读取失败，不作为未找到处理。");
    }
}

std::vector<新外设消息信息> 新_外设世界树类::读取当前消息(
    稳定编码 窗口, 新外设消息方向 方向) const noexcept {
    try {
        const auto 来源 = 读取对话框来源(窗口);
        if (!来源) throw std::runtime_error("peripheral message query window missing");
        std::lock_guard 锁(外设互斥);
        const auto 定位 = 读取窗口定位(来源->定位.场景节点);
        if (定位 != 来源->定位 || (方向 != 新外设消息方向::接收 && 方向 != 新外设消息方向::发送))
            throw std::runtime_error("peripheral message query location/direction invalid");
        const auto 特征 = 方向 == 新外设消息方向::接收 ? 定位.接收消息特征 : 定位.发送消息特征;
        if (!特征) throw std::runtime_error("peripheral message feature not initialized");
        std::vector<新外设消息信息> 结果;
        for (const auto 成员 : 读取消息成员(*特征)) {
            const auto 消息 = 读取正式消息(成员);
            if (!消息 || 消息->窗口节点 != 窗口 || 消息->场景节点 != 定位.场景节点
                || 消息->来源.方向 != 方向 || 消息->来源.公开宿主会话 != 来源->公开宿主会话
                || 消息->来源.对话框标识 != 来源->对话框标识)
                throw std::runtime_error("peripheral message collection crosses source");
            结果.push_back(*消息);
        }
        return 结果;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取当前外设消息", L"当前集合不完整，不返回部分或伪空集合。");
    }
}

新外设事实状态 新_外设世界树类::移除当前消息引用(
    稳定编码 窗口, 新外设消息方向 方向, 稳定编码 消息节点, const 新状态强时间& 时间) noexcept {
    try {
        const auto 来源 = 读取对话框来源(窗口);
        if (!来源) return 新外设事实状态::未找到;
        std::lock_guard 锁(外设互斥);
        const auto 定位 = 读取窗口定位(来源->定位.场景节点);
        if (定位 != 来源->定位 || (方向 != 新外设消息方向::接收 && 方向 != 新外设消息方向::发送)
            || 时间.语义 != 新状态时间语义::实例绝对UTC || 时间.纳秒 < 0)
            return 新外设事实状态::入口拒绝;
        const auto 特征节点 = 方向 == 新外设消息方向::接收 ? 定位.接收消息特征 : 定位.发送消息特征;
        if (!特征节点) throw std::runtime_error("peripheral message remove feature missing");
        auto 成员 = 读取消息成员(*特征节点);
        const auto 位置 = std::find(成员.begin(), 成员.end(), 消息节点);
        if (位置 == 成员.end()) return 新外设事实状态::无变化;
        const auto 消息 = 读取正式消息(消息节点);
        if (!消息 || 消息->窗口节点 != 窗口 || 消息->来源.方向 != 方向)
            throw std::runtime_error("peripheral message removal source mismatch");
        const auto 前 = 读取最近特征状态(定位.场景节点, 窗口, *特征节点);
        const auto 特征 = 服务_.特征服务.获取特征(*特征节点);
        if (!前 || !特征 || 特征->当前值 != std::optional<新特征准确值>{前->固定准确值}
            || 时间.纳秒 <= 前->强时间.纳秒)
            throw std::runtime_error("peripheral message removal predecessor invalid");
        成员.erase(位置);
        std::vector<std::uint64_t> 编码组;
        for (const auto 编码 : 成员) 编码组.push_back(编码.值);
        const auto 材料 = 服务_.特征值服务.保存或取得特征值(材料物理类型::U64数组, 编码组);
        if (!有效(材料)) throw std::runtime_error("peripheral message removal material failed");
        基础节点组追加请求 请求;
        请求.预期联合节点组.push_back(读取消息快照(材料));
        请求.预期联合节点组.push_back(读取消息快照(消息根));
        const auto 保护字段 = 服务_.特征概念服务.消息集合成员保护字段();
        const auto 材料快照 = 请求.预期联合节点组.front();
        for (const auto 编码 : 成员) {
            const auto 已保护 = std::any_of(材料快照.结构.字段组.begin(), 材料快照.结构.字段组.end(),
                [&](const auto& 字段) { return 字段.字段节点 == 保护字段 && 字段.内容 == 基础字段内容{编码}; });
            if (!已保护) 请求.追加字段组.push_back({材料, 保护字段, 编码});
        }
        const auto 封存 = 全局基础数据集.封存原子追加片段(请求);
        if (封存.状态 != 基础节点组追加状态::已完成 || !封存.片段)
            throw std::runtime_error("peripheral message removal protection seal failed");
        auto 准备 = 服务_.特征服务.准备顶层引用集合更新(*特征节点, 材料);
        if (准备.状态 != 新特征I64准备状态::已准备 || !准备.准备)
            throw std::runtime_error("peripheral message removal preparation failed");
        const auto 提交 = 服务_.特征服务.提交顶层引用集合更新(*准备.准备, {*封存.片段});
        if (提交.状态 != 基础节点组追加状态::已完成) {
            服务_.特征服务.取消顶层引用集合更新(*准备.准备);
            throw std::runtime_error("peripheral message removal commit failed");
        }
        if (提交.附加片段回执组.size() != 1 || !提交.附加片段回执组[0].对应片段(*封存.片段))
            throw std::runtime_error("peripheral message removal protection receipt invalid");
        const auto 结果 = 服务_.特征服务.完成顶层引用集合更新(*准备.准备);
        完成消息集合变化(定位, *特征节点, 材料, 结果, 前, 时间);
        if (读取消息成员(*特征节点) != 成员 || !读取正式消息(消息节点))
            throw std::runtime_error("peripheral message removal readback failed");
        服务_.特征服务.结束顶层引用集合更新(*准备.准备);
        return 新外设事实状态::已完成;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"移除当前外设消息引用", L"引用更新失败，保留消息本体及历史。");
    }
}
} // namespace 海中鱼巣
