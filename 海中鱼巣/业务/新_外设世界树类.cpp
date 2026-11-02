#include "新_外设世界树类.h"
#include "../核心/容错检查.h"

#include <array>
#include <algorithm>
#include <chrono>
#include <limits>
#include <mutex>
#include <stdexcept>
#include <set>
#include <tuple>

namespace 海中鱼巣 {
namespace {
稳定编码 外设根{};
std::array<稳定编码, 6> 类型{};
std::array<稳定编码, 2> 消息类型{};
稳定编码 输出队列类型{};
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
    if ((方向 != 1 && 方向 != 2 && 方向 != 3) || 序号.size() != 1 || !序号[0]
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
        特征概念定义 输出队列;
        输出队列.材料类型 = 特征概念材料物理类型::U64数组;
        输出队列.值域 = 特征概念全材料值域{};
        输出队列.比较规则 = 特征概念比较规则::完整材料精确;
        输出队列.聚合规则 = 特征概念聚合规则::完整材料精确集合;
        const auto 队列类型 = 确保具名类型(服务_, *语言.语言节点,
            "外设对话框输出队列", 输出队列);
        if (有效(输出队列类型) && 输出队列类型 != 队列类型)
            throw std::runtime_error("peripheral output queue type changed");
        输出队列类型 = 队列类型;
        return {已有 ? 新世界初始化状态::无变化 : 新世界初始化状态::已完成,
            根.场景节点, 根.专属存在概念节点};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"初始化外设世界", L"根或正式特征类型不完整。");
    }
}

std::optional<稳定编码> 新_外设世界树类::获取输出队列特征类型() const noexcept {
    try {
        std::lock_guard 锁(外设互斥);
        if (!有效(外设根)) return std::nullopt;
        if (!有效(输出队列类型)) throw std::runtime_error("peripheral output queue type unavailable");
        const auto 读取 = 服务_.特征概念服务.读取特征概念定义保护(输出队列类型);
        if (读取.状态 != 基础结构操作状态::已完成 || 读取.前提已变化
            || !读取.信息 || 读取.预期节点组.empty() || 读取.信息->类型根 != 输出队列类型)
            throw std::runtime_error("peripheral output queue definition unavailable");
        const auto& 定义 = 读取.信息->定义;
        if (定义.材料类型 != 特征概念材料物理类型::U64数组
            || !std::holds_alternative<特征概念全材料值域>(定义.值域)
            || 定义.比较规则 != 特征概念比较规则::完整材料精确
            || 定义.聚合规则 != 特征概念聚合规则::完整材料精确集合)
            throw std::runtime_error("peripheral output queue definition corrupt");
        return 输出队列类型;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取外设输出队列特征类型", L"已初始化的正式输出队列定义不完整。");
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
        || 存在材料->特征节点组.size() < 2 || 存在材料->特征节点组.size() > 7
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
        else if (*根 == 输出队列类型) 结果.输出队列特征 = 特征节点;
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
    if (!定位.输出队列特征) {
        const auto 添加 = 场景_.添加场景内存在特征(定位.场景节点, 定位.窗口节点,
            输出队列类型, 空材料, 当前强时间());
        if (添加.状态 != 新场景操作状态::已完成)
            throw std::runtime_error("peripheral output queue initialization failed");
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
        throw std::runtime_error("peripheral dynamic abstraction incomplete; status="
            + std::to_string(static_cast<unsigned>(处理.状态)));
    const auto 后 = 服务_.动态服务.获取动态(动态节点);
    const auto 后列表 = 场景_.查询动态列表(场景节点);
    if (!后 || 后->种类 != 前->种类 || 后->主题存在节点 != 前->主题存在节点
        || 后->状态节点组 != 前->状态节点组 || !后->动态概念节点
        || !服务_.动态概念服务.获取动态概念(*后->动态概念节点)
        || std::count(后列表.begin(), 后列表.end(), 动态节点) != 1)
        throw std::runtime_error("peripheral dynamic abstraction readback invalid");
}

新_外设世界树类::特征变化读回 新_外设世界树类::更新窗口特征并形成动态(
    const 新外设对话框定位& 定位, 稳定编码 特征节点,
    const 新特征准确值& 新值, const 新状态强时间& 时间, std::optional<稳定编码> 来源动作) {
    if (!场景_.是场景节点(定位.场景节点) || 读取窗口定位(定位.场景节点) != 定位
        || (特征节点 != 定位.接收内容特征 && 特征节点 != 定位.接收数量特征
            && 定位.发送数量特征 != 特征节点 && 定位.发送内容特征 != 特征节点
            && 定位.输出队列特征 != 特征节点)
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
        return {新外设事实状态::无变化,{},{}};
    }
    if (更新.状态 != 新场景操作状态::已完成 || !更新.状态节点) {
        std::string 原因 = "peripheral feature update incomplete; scene="
            + std::to_string(static_cast<unsigned>(更新.状态));
        if (更新.存在结果) {
            原因 += "; existence=" + std::to_string(static_cast<unsigned>(更新.存在结果->状态));
            if (更新.存在结果->特征结果)
                原因 += "; feature=" + std::to_string(static_cast<unsigned>(更新.存在结果->特征结果->状态));
            if (更新.存在结果->状态结果)
                原因 += "; state=" + std::to_string(static_cast<unsigned>(更新.存在结果->状态结果->状态));
            if (更新.存在结果->专属概念结果)
                原因 += "; concept=" + std::to_string(static_cast<unsigned>(*更新.存在结果->专属概念结果));
        }
        throw std::runtime_error(原因);
    }
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
    if (!前) return {新外设事实状态::已完成,后->节点,{}};
    const auto 动态 = 服务_.动态服务.建立原子动态(定位.窗口节点, 前->节点, 后->节点);
    if (动态.状态 != 新动态操作状态::已完成 || !动态.动态节点
        || 场景_.添加动态(定位.场景节点, *动态.动态节点) != 新场景操作状态::已完成)
        throw std::runtime_error("peripheral dynamic publication incomplete");
    if (来源动作) {
        if (服务_.动态服务.关联动作结果(*来源动作,*动态.动态节点,新动作结果关系类型::导致变化)!=新动态操作状态::已完成)
            throw std::runtime_error("peripheral explicit output action association failed");
        const auto a=服务_.动态服务.获取动态(*来源动作);
        if (!a || std::count(a->动作结果组.begin(),a->动作结果组.end(),
            新动作结果关联{新动作结果关系类型::导致变化,*动态.动态节点})!=1)
            throw std::runtime_error("peripheral explicit output action association readback failed");
    }
    核验并处理已发布动态(定位.场景节点, *动态.动态节点);
    return {新外设事实状态::已完成,后->节点,*动态.动态节点};
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
        || (方向 != 新外设消息方向::接收 && 方向 != 新外设消息方向::发送)
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
    // 封存要求每个既有引用目标都有完整预期或只读身份保护。
    // 消息类型和旧消息成员只被引用，不应为补保护而取得它们的写资格。
    std::set<稳定编码> 只读目标(成员.begin(), 成员.end());
    只读目标.insert(消息字段[11]);
    for (const auto 编码 : 只读目标) {
        const auto 节点 = 全局基础数据集.查询节点(编码);
        if (!节点) throw std::runtime_error("peripheral message referenced identity unavailable");
        请求.只读预期.节点组.push_back(*节点);
    }
    const auto 封存 = 全局基础数据集.封存原子追加片段(请求);
    if (封存.状态 != 基础节点组追加状态::已完成 || !封存.片段)
        throw std::runtime_error("peripheral message identity seal failed; status="
            + std::to_string(static_cast<unsigned>(封存.状态)));
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
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"记录外设接收",
            L"实际接收后特征发布或读回失败，保留已发生事实，不作为可重发否定。", e.what());
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
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"记录外设发送",
            L"完整写出已发生，正式发送事实发布失败，保留现场，不重发补记录。", e.what());
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

新外设接收记录完整结果 新_外设世界树类::完整读取仍保留的接收记录() const noexcept {
    try {
        std::lock_guard 锁(外设互斥);
        if (!有效(消息根)) return {};
        const auto 根 = 读取消息快照(消息根);
        新外设接收记录完整结果 结果;
        std::set<稳定编码> 已读;
        std::set<std::tuple<std::string, std::string, std::uint64_t>> 来源组;
        for (const auto& 字段 : 根.结构.字段组) {
            const auto* 节点 = std::get_if<稳定编码>(&字段.内容);
            if (字段.字段节点 != 消息字段[10] || !节点 || !已读.insert(*节点).second)
                throw std::logic_error("retained peripheral message registry corrupt");
            const auto 消息 = 读取正式消息(*节点);
            if (!消息) throw std::logic_error("retained peripheral message missing");
            if (消息->来源.方向 != 新外设消息方向::接收) continue;
            if (!来源组.emplace(消息->来源.公开宿主会话, 消息->来源.对话框标识,
                    消息->来源.来源序号).second)
                throw std::logic_error("retained inbound message source duplicated");
            结果.消息组.push_back(*消息);
        }
        if (!消息快照相同(根, 读取消息快照(消息根)))
            throw std::logic_error("retained message registry changed under peripheral lock");
        std::sort(结果.消息组.begin(), 结果.消息组.end(), [](const auto& 左, const auto& 右) {
            return std::tie(左.来源.公开宿主会话, 左.来源.对话框标识,
                       左.来源.来源序号, 左.消息节点)
                < std::tie(右.来源.公开宿主会话, 右.来源.对话框标识,
                       右.来源.来源序号, 右.消息节点);
        });
        结果.状态 = 新外设事实状态::已完成;
        return 结果;
    } catch (const std::bad_alloc&) {
        return {新外设事实状态::资源失败, {}};
    } catch (const std::length_error&) {
        return {新外设事实状态::资源失败, {}};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"完整读取保留的接收记录",
            L"正式消息登记或内容损坏，不交付部分记录。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"完整读取保留的接收记录",
            L"完整消息读取发生未知工程异常。");
    }
}

新外设消息保护结果 新_外设世界树类::读取消息保护(稳定编码 节点) const noexcept {
    try {
        std::lock_guard 锁(外设互斥);
        if (!有效(消息根)) return {};
        if (!有效(节点)) return {新外设事实状态::入口拒绝, {}, {}};
        新外设消息保护结果 结果;
        结果.消息 = 读取正式消息(节点, &结果.完整保护);
        if (!结果.消息) return {新外设事实状态::未找到, {}, {}};
        结果.状态 = 新外设事实状态::已完成;
        return 结果;
    } catch (const std::bad_alloc&) {
        return {新外设事实状态::资源失败, {}, {}};
    } catch (const std::length_error&) {
        return {新外设事实状态::资源失败, {}, {}};
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取外设消息保护",
            L"正式消息保护读取失败，不作为未找到处理。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取外设消息保护",
            L"消息保护读取发生未知工程异常。");
    }
}

std::optional<新外设消息信息> 新_外设世界树类::查找消息(const 新外设消息来源键& 来源) const noexcept {
    try {
        std::lock_guard 锁(外设互斥);
        if (!有效(消息根) || !来源.来源序号 || 来源.公开宿主会话.empty() || 来源.对话框标识.empty())
            return std::nullopt;
        if (来源.方向 != 新外设消息方向::接收 && 来源.方向 != 新外设消息方向::发送
            && 来源.方向 != 新外设消息方向::输出)
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

std::vector<新外设消息信息> 新_外设世界树类::读取输出队列成员(
    const 新外设对话框定位& 定位) const {
    if (!定位.输出队列特征 || 读取窗口定位(定位.场景节点) != 定位)
        throw std::runtime_error("peripheral output queue location invalid");
    const auto 特征 = 服务_.特征服务.获取特征(*定位.输出队列特征);
    const auto* 材料节点 = 特征 && 特征->当前值
        ? std::get_if<稳定编码>(&*特征->当前值) : nullptr;
    const auto 材料 = 材料节点 ? 服务_.特征值服务.获取特征值(*材料节点) : std::nullopt;
    const auto* 编码组 = 材料 ? std::get_if<std::vector<std::uint64_t>>(&材料->材料) : nullptr;
    if (!编码组 || 材料->物理类型 != 材料物理类型::U64数组)
        throw std::runtime_error("peripheral output queue current material invalid");
    const auto 场景 = 场景_.获取场景(定位.场景节点);
    if (!场景) throw std::runtime_error("peripheral output queue scene unavailable");
    const auto 会话 = 读取文本特征(读取类型特征(场景->特征节点组, 类型[0]));
    const auto 页面 = 读取文本特征(读取类型特征(场景->特征节点组, 类型[1]));
    const auto 保护字段 = 服务_.特征概念服务.消息集合成员保护字段();
    if (!有效(保护字段)) throw std::runtime_error("peripheral message protection key unavailable");
    const auto 材料快照 = 读取消息快照(*材料节点);
    std::set<std::uint64_t> 去重;
    std::vector<新外设消息信息> 结果;
    结果.reserve(编码组->size());
    for (const auto 编码 : *编码组) {
        if (!编码 || !去重.insert(编码).second)
            throw std::runtime_error("peripheral output queue member invalid or repeated");
        const auto 消息 = 读取正式消息(稳定编码{编码});
        if (!消息 || 消息->来源.方向 != 新外设消息方向::输出
            || 消息->场景节点 != 定位.场景节点 || 消息->窗口节点 != 定位.窗口节点
            || 消息->来源.公开宿主会话 != 会话 || 消息->来源.对话框标识 != 页面)
            throw std::runtime_error("peripheral output queue member crosses source");
        const auto 保护数 = std::count_if(材料快照.结构.字段组.begin(),
            材料快照.结构.字段组.end(), [&](const auto& 字段) {
                return 字段.字段节点 == 保护字段
                    && 字段.内容 == 基础字段内容{消息->消息节点};
            });
        if (保护数 != 1)
            throw std::runtime_error("peripheral output queue member lacks unique protection");
        结果.push_back(*消息);
    }
    return 结果;
}

新_外设世界树类::特征变化读回 新_外设世界树类::更新输出队列(const 新外设对话框定位& 定位,
    const std::vector<新外设消息信息>& 消息组, std::optional<稳定编码> 来源动作) {
    if (!定位.输出队列特征) throw std::runtime_error("peripheral output queue feature missing");
    const auto 场景 = 场景_.获取场景(定位.场景节点);
    if (!场景 || 读取窗口定位(定位.场景节点) != 定位)
        throw std::runtime_error("peripheral output queue location changed");
    const auto 会话 = 读取文本特征(读取类型特征(场景->特征节点组, 类型[0]));
    const auto 页面 = 读取文本特征(读取类型特征(场景->特征节点组, 类型[1]));
    std::vector<std::uint64_t> 编码组;
    编码组.reserve(消息组.size());
    std::set<稳定编码> 去重;
    for (const auto& 消息 : 消息组) {
        if (!有效(消息.消息节点) || !去重.insert(消息.消息节点).second)
            throw std::runtime_error("peripheral output queue member repeated");
        const auto 正式 = 读取正式消息(消息.消息节点);
        if (!正式 || 正式->来源 != 消息.来源 || 正式->UTF8正文 != 消息.UTF8正文
            || 正式->来源.方向 != 新外设消息方向::输出
            || 正式->场景节点 != 定位.场景节点 || 正式->窗口节点 != 定位.窗口节点
            || 正式->来源.公开宿主会话 != 会话 || 正式->来源.对话框标识 != 页面)
            throw std::runtime_error("peripheral output queue member source invalid");
        编码组.push_back(消息.消息节点.值);
    }
    const auto 材料节点 = 服务_.特征值服务.保存或取得特征值(
        材料物理类型::U64数组, 编码组);
    if (!有效(材料节点)) throw std::runtime_error("peripheral output queue material unavailable");
    const auto 保护字段 = 服务_.特征概念服务.消息集合成员保护字段();
    if (!有效(保护字段)) throw std::runtime_error("peripheral message protection key unavailable");
    const auto 快照 = 读取消息快照(材料节点);
    基础节点组追加请求 保护请求;
    保护请求.预期联合节点组.push_back(快照);
    for (const auto& 消息 : 消息组) {
        const auto 数量 = std::count_if(快照.结构.字段组.begin(), 快照.结构.字段组.end(),
            [&](const auto& 字段) {
                return 字段.字段节点 == 保护字段
                    && 字段.内容 == 基础字段内容{消息.消息节点};
            });
        if (数量 > 1) throw std::runtime_error("peripheral output queue protection duplicated");
        if (!数量) 保护请求.追加字段组.push_back({材料节点, 保护字段, 消息.消息节点});
    }
    if (!保护请求.追加字段组.empty()) {
        const auto 发布 = 全局基础数据集.原子追加节点组(保护请求);
        if (发布.状态 != 基础节点组追加状态::已完成)
            throw std::runtime_error("peripheral output queue member protection failed; status="
                + std::to_string(static_cast<unsigned>(发布.状态)));
    }
    const auto 时间 = 当前强时间();
    const auto change=更新窗口特征并形成动态(定位, *定位.输出队列特征,
            新特征准确值{材料节点}, 时间,来源动作);
    if (!更新完成(change))
        throw std::runtime_error("peripheral output queue update failed");
    const auto 读回 = 读取输出队列成员(定位);
    if (读回.size() != 消息组.size()
        || !std::equal(读回.begin(), 读回.end(), 消息组.begin(),
            [](const auto& 左, const auto& 右) { return 左.消息节点 == 右.消息节点; }))
        throw std::runtime_error("peripheral output queue readback mismatch");
    return change;
}

新外设消息信息 新_外设世界树类::建立输出消息(const 新外设对话框定位& 定位,
    const 新外设消息来源键& 来源, std::string_view 正文) {
    const auto 正文材料 = 服务_.特征值服务.保存或取得特征值(
        材料物理类型::UTF8字符串, std::string(正文));
    const auto 裸节点 = 全局基础数据集.新建节点();
    if (!有效(正文材料) || !有效(裸节点))
        throw std::runtime_error("peripheral output message allocation failed");
    const auto 场景 = 场景_.获取场景(定位.场景节点);
    if (!场景) throw std::runtime_error("peripheral output message scene unavailable");
    if (读取文本特征(读取类型特征(场景->特征节点组, 类型[0])) != 来源.公开宿主会话
        || 读取文本特征(读取类型特征(场景->特征节点组, 类型[1])) != 来源.对话框标识)
        throw std::runtime_error("peripheral output source identity changed");
    基础节点组追加请求 请求;
    std::set<稳定编码> 已保护;
    const auto 加入预期 = [&](稳定编码 节点) {
        if (已保护.insert(节点).second)
            请求.预期联合节点组.push_back(读取消息快照(节点));
    };
    for (const auto 节点 : {消息根, 裸节点, 定位.场景节点, 定位.窗口节点, 正文材料})
        加入预期(节点);
    for (const auto 身份特征 : 场景->特征节点组) {
        加入预期(身份特征);
        const auto 信息 = 服务_.特征服务.获取特征(身份特征);
        const auto* 当前材料 = 信息 && 信息->当前值
            ? std::get_if<稳定编码>(&*信息->当前值) : nullptr;
        if (当前材料) 加入预期(*当前材料);
    }
    const auto& 裸 = 请求.预期联合节点组[1];
    if (!裸.结构.字段组.empty() || !裸.入向字段组.empty()
        || !裸.入向外部关系组.empty() || !裸.出向外部关系组.empty())
        throw std::runtime_error("peripheral private output message unexpectedly visible");
    const auto 时间 = 当前强时间();
    const std::array<基础组内字段内容, 10> 值{消息字段[11], 消息根,
        定位.场景节点, 定位.窗口节点,
        基础值{基础原始值{来源.公开宿主会话}},
        基础值{基础原始值{来源.对话框标识}},
        基础值{基础原始值{std::int64_t{3}}},
        基础值{基础原始值{std::vector<std::uint64_t>{来源.来源序号}}}, 正文材料,
        基础值{基础原始值{时间.纳秒}}};
    for (std::size_t 序号 = 0; 序号 < 值.size(); ++序号)
        请求.追加字段组.push_back({裸节点, 消息字段[序号], 值[序号]});
    请求.追加字段组.push_back({消息根, 消息字段[10], 裸节点});
    const auto 发布 = 全局基础数据集.原子追加节点组(请求);
    if (发布.状态 != 基础节点组追加状态::已完成)
        throw std::runtime_error("peripheral output message publication failed; status="
            + std::to_string(static_cast<unsigned>(发布.状态)));
    const auto 消息 = 读取正式消息(裸节点);
    if (!消息 || 消息->来源 != 来源 || 消息->场景节点 != 定位.场景节点
        || 消息->窗口节点 != 定位.窗口节点 || 消息->UTF8正文 != 正文)
        throw std::runtime_error("peripheral output message readback failed");
    return *消息;
}

新外设输出队列结果 新_外设世界树类::输出入列(
    const 新外设输出入列请求& 请求) noexcept {
    if (!请求.提交序号 || 请求.UTF8正文.empty())
        return {新外设输出队列状态::目标不符, {}};
    try {
        if (!新_语言类::严格UTF8字节串有效(请求.UTF8正文))
            return {新外设输出队列状态::目标不符, {}};
        std::lock_guard 锁(外设互斥);
        return 输出入列_已加锁(请求);
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"外设输出入列",
            L"正式消息或输出队列发布失败，保留已发生事实，不伪称发送或重试。", 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"外设输出入列",
            L"正式消息或输出队列发布失败，保留已发生事实，不伪称发送或重试。");
    }
}

新外设原子发送结果 新_外设世界树类::原子提交发送(
    const 新外设原子发送请求& 请求, const std::function<bool()>& 复核来源) noexcept {
    try {
        if (请求.完整保护组.empty() || !复核来源)
            throw std::logic_error("atomic send requires complete language protection and source guard");
        std::unique_lock 锁(外设互斥);
        if (!有效(外设根) || !有效(消息根)) return {};
        if (!请求.定位.输出队列特征 || 读取窗口定位(请求.定位.场景节点) != 请求.定位)
            return {};
        if (!服务_.场景服务.存在属于场景范围(请求.执行场景, 请求.执行主体)
            || !服务_.场景服务.存在属于场景范围(请求.执行场景, 请求.定位.窗口节点)) return {};
        // 生成/独立唯一回读的原读集不得用发送时的fresh快照替换。
        基础节点组追加请求 消息请求;
        消息请求.预期联合节点组 = 请求.完整保护组;
        const auto 并入 = [&](const 基础联合节点快照& s) {
            const auto it = std::find_if(消息请求.预期联合节点组.begin(), 消息请求.预期联合节点组.end(),
                [&](const auto& x) { return x.结构.节点.编码 == s.结构.节点.编码; });
            if (it != 消息请求.预期联合节点组.end()) return 消息快照相同(*it, s);
            消息请求.预期联合节点组.push_back(s); return true;
        };
        const auto 加保护 = [&](稳定编码 n) {
            const auto r=全局基础数据集.查询节点组完整结构({n});
            if(r.状态!=基础结构操作状态::已完成 || r.节点组.size()!=1)
                throw std::logic_error("atomic send protection read failed");
            return r.节点组[0].快照 && 并入(*r.节点组[0].快照);
        };
        for (const auto& s : 请求.完整保护组) if (!加保护(s.结构.节点.编码)) return {};
        // 保护场景归属路径，迁移不能在条件核验与核心CAS之间漏过。
        for(const auto e:{请求.执行主体,请求.定位.窗口节点}) {
            auto parent=服务_.场景服务.查询所属场景(e);
            std::set<稳定编码> seen;
            while(parent) {
                if(!seen.insert(*parent).second)throw std::logic_error("atomic send scene ancestry cycle");
                if(!加保护(*parent))return {};
                if(*parent==请求.执行场景)break;
                parent=服务_.场景服务.查询父场景(*parent);
            }
            if(!parent)return {};
        }
        const auto 场景 = 场景_.获取场景(请求.定位.场景节点);
        if (!场景) return {};
        const auto 会话 = 读取文本特征(读取类型特征(场景->特征节点组, 类型[0]));
        const auto 页面 = 读取文本特征(读取类型特征(场景->特征节点组, 类型[1]));
        for (const auto n : 场景->特征节点组) {
            if (!加保护(n)) return {};
            const auto f = 服务_.特征服务.获取特征(n);
            if (!f) return {};
            if (f->当前值) if (const auto* v = std::get_if<稳定编码>(&*f->当前值))
                if (!加保护(*v)) return {};
        }
        const auto 根 = 读取消息快照(消息根);
        if (!并入(根)) return {};
        std::uint64_t 最大序号 = 0;
        std::set<std::uint64_t> 序号组;
        for (const auto& f : 根.结构.字段组) {
            if (f.字段节点 != 消息字段[10]) throw std::logic_error("atomic send message index corrupt");
            const auto m = 读取正式消息(std::get<稳定编码>(f.内容));
            if (!m) throw std::logic_error("atomic send indexed message unreadable");
            if (m->来源.方向 != 新外设消息方向::输出 || m->来源.公开宿主会话 != 会话
                || m->来源.对话框标识 != 页面) continue;
            if (m->场景节点 != 请求.定位.场景节点 || m->窗口节点 != 请求.定位.窗口节点
                || !序号组.insert(m->来源.来源序号).second)
                throw std::logic_error("atomic send source identity conflicts");
            最大序号 = (std::max)(最大序号, m->来源.来源序号);
        }
        if (最大序号 == (std::numeric_limits<std::uint64_t>::max)())
            throw std::logic_error("atomic send source sequence exhausted");
        const auto 原队列 = 读取输出队列成员(请求.定位);
        const auto F = 服务_.特征服务.获取特征(*请求.定位.输出队列特征);
        const auto 前 = 读取最近特征状态(请求.定位.场景节点, 请求.定位.窗口节点, *请求.定位.输出队列特征);
        if (!F || !F->当前值 || !前 || 前->固定准确值 != *F->当前值)
            throw std::logic_error("atomic send queue/current state mismatch");
        for (const auto n : {请求.执行场景, 请求.执行主体, 请求.动作定义, 请求.动作来源,
            请求.定位.场景节点, 请求.定位.窗口节点, *请求.定位.输出队列特征, 前->节点})
            if (!加保护(n)) return {};
        const auto 正文材料 = 服务_.特征值服务.保存或取得特征值(材料物理类型::UTF8字符串, 请求.UTF8正文);
        if (!有效(正文材料)) throw std::logic_error("atomic send body material unavailable");
        // 两个空节点只是未发布技术材料，直到CAS成功才取得消息/动作身份。
        const auto 裸消息 = 全局基础数据集.新建节点();
        if (!有效(裸消息)) throw std::logic_error("atomic send private message allocation failed");
        std::optional<稳定编码> 裸动作;
        bool 已提交 = false;
        struct 清理守卫 final {
            稳定编码 消息; std::optional<稳定编码>& 动作; bool& 提交;
            ~清理守卫() {
                if (提交) return;
                for (const auto n : {消息, 动作.value_or(稳定编码{})}) {
                    if (!有效(n)) continue;
                    const auto s = 读取消息快照(n);
                    if (!s.结构.字段组.empty() || !s.入向字段组.empty()
                        || !s.入向外部关系组.empty() || !s.出向外部关系组.empty()
                        || !全局基础数据集.删除节点(n))
                        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子发送取消", L"未发布技术节点释放失败。");
                }
            }
        } 清理{裸消息, 裸动作, 已提交};
        auto 动作时间 = 当前强时间();
        if (动作时间.纳秒 < 前->强时间.纳秒) throw std::logic_error("atomic send time moved backwards");
        const 新动作动态建立请求 动作请求{请求.执行主体, 请求.动作定义, 请求.动作来源,
            请求.定位.窗口节点, 请求.定位.输出队列特征, 动作时间};
        const auto 动作准备 = 服务_.动态服务.准备普通动作动态(动作请求);
        if (动作准备.状态 == 新动态操作状态::动作来源冲突) return {};
        if (动作准备.状态 != 新动态操作状态::已完成 || !动作准备.动作节点 || !动作准备.片段)
            throw std::logic_error("atomic send action preparation failed");
        裸动作 = 动作准备.动作节点;
        std::vector<std::uint64_t> 编码组;
        for (const auto& m : 原队列) 编码组.push_back(m.消息节点.值);
        编码组.push_back(裸消息.值);
        const auto 新材料 = 服务_.特征值服务.保存或取得特征值(材料物理类型::U64数组, 编码组);
        if (!有效(新材料)) throw std::logic_error("atomic send queue material unavailable");
        for (const auto n : {裸消息, 正文材料, 新材料}) if (!加保护(n)) return {};
        const 新外设消息来源键 来源{会话, 页面, 新外设消息方向::输出, 最大序号 + 1};
        const std::array<基础组内字段内容, 10> 值{消息字段[11], 消息根,
            请求.定位.场景节点, 请求.定位.窗口节点,
            基础值{基础原始值{会话}}, 基础值{基础原始值{页面}},
            基础值{基础原始值{std::int64_t{3}}},
            基础值{基础原始值{std::vector<std::uint64_t>{来源.来源序号}}}, 正文材料,
            基础值{基础原始值{动作时间.纳秒}}};
        for (std::size_t i = 0; i < 值.size(); ++i)
            消息请求.追加字段组.push_back({裸消息, 消息字段[i], 值[i]});
        消息请求.追加字段组.push_back({消息根, 消息字段[10], 裸消息});
        const auto 保护字段 = 服务_.特征概念服务.消息集合成员保护字段();
        if (!有效(保护字段)) throw std::logic_error("atomic send membership key unavailable");
        for (const auto n : 编码组) {
            消息请求.追加字段组.push_back({新材料, 保护字段, 稳定编码{n}});
            const auto node = 全局基础数据集.查询节点(稳定编码{n});
            if (!node) return {};
            消息请求.只读预期.节点组.push_back(*node);
        }
        const auto 消息标记 = 全局基础数据集.查询节点(消息字段[11]);
        if (!消息标记) throw std::logic_error("atomic send message type missing");
        消息请求.只读预期.节点组.push_back(*消息标记);
        const auto 封存 = 全局基础数据集.封存原子追加片段(消息请求);
        if (封存.状态 == 基础节点组追加状态::预期不符
            || 封存.状态 == 基础节点组追加状态::节点不存在
            || 封存.状态 == 基础节点组追加状态::引用不存在) return {};
        if (封存.状态 != 基础节点组追加状态::已完成 || !封存.片段)
            throw std::logic_error("atomic send message seal failed");
        // 来源核验会再次读取会话；不能持外设锁反向回调。
        // 释放期间的变化仍由下面同一CAS中的原定位/队列/语言快照拒绝。
        锁.unlock();
        const bool 来源有效 = 复核来源();
        锁.lock();
        if (!来源有效) return {};
        auto 准备 = 服务_.特征服务.准备顶层完整材料更新(*请求.定位.输出队列特征,
            新材料, 消息请求.预期联合节点组);
        if (准备.状态 == 新特征I64准备状态::预期已变化
            || 准备.状态 == 新特征I64准备状态::联合更新占用) return {};
        if (准备.状态 != 新特征I64准备状态::已准备 || !准备.准备)
            throw std::logic_error("atomic send queue preparation failed");
        const auto 提交 = 服务_.特征服务.提交顶层I64更新(*准备.准备, {*动作准备.片段, *封存.片段});
        if (提交.状态 != 基础节点组追加状态::已完成) {
            服务_.特征服务.取消顶层I64更新(*准备.准备);
            if (提交.状态 == 基础节点组追加状态::预期不符
                || 提交.状态 == 基础节点组追加状态::节点不存在
                || 提交.状态 == 基础节点组追加状态::引用不存在) return {};
            throw std::logic_error("atomic send combined publication failed");
        }
        已提交 = true;
        if (提交.附加片段回执组.size() != 2
            || !提交.附加片段回执组[0].对应片段(*动作准备.片段)
            || !提交.附加片段回执组[1].对应片段(*封存.片段))
            throw std::logic_error("atomic send combined receipt mismatch");
        const auto 特征结果 = 服务_.特征服务.完成顶层I64更新(*准备.准备);
        const auto 消息 = 读取正式消息(裸消息);
        if (!消息 || 消息->来源 != 来源 || 消息->UTF8正文 != 请求.UTF8正文)
            throw std::logic_error("atomic send formal message readback failed");
        const auto 动作 = 服务_.动态服务.获取动态(*裸动作);
        if (!动作 || 动作->种类 != 新动态种类::动作 || 动作->主题存在节点 != 请求.执行主体
            || 动作->动作定义节点 != 请求.动作定义 || 动作->动作来源节点 != 请求.动作来源
            || 动作->动作目标存在节点 != 请求.定位.窗口节点
            || 动作->动作目标特征节点 != 请求.定位.输出队列特征 || 动作->动作时间 != 动作时间
            || !动作->动作结果组.empty())
            throw std::logic_error("atomic send formal action readback failed");
        const auto 挂接 = 服务_.场景服务.添加动态(请求.执行场景, *裸动作);
        if (挂接 != 新场景操作状态::已完成) throw std::logic_error("atomic send action attachment failed");
        auto 后时间 = 当前强时间();
        if (后时间.纳秒 <= 动作时间.纳秒 || 后时间.纳秒 <= 前->强时间.纳秒)
            throw std::logic_error("atomic send completion time did not advance");
        const auto 后 = 服务_.存在服务.完成已提交完整材料特征变化(请求.定位.窗口节点,
            *请求.定位.输出队列特征, 特征结果, 输出队列类型, 新材料, 后时间);
        if (后.状态 != 新存在操作状态::已完成 || !后.状态结果 || !后.状态结果->状态节点)
            throw std::logic_error("atomic send state completion failed");
        const auto 后节点 = *后.状态结果->状态节点;
        if (场景_.添加状态(请求.定位.场景节点, 后节点) != 新场景操作状态::已完成)
            throw std::logic_error("atomic send state attachment failed");
        const auto 变化 = 服务_.动态服务.建立原子动态(请求.定位.窗口节点, 前->节点, 后节点);
        if (变化.状态 != 新动态操作状态::已完成 || !变化.动态节点
            || 场景_.添加动态(请求.定位.场景节点, *变化.动态节点) != 新场景操作状态::已完成
            || 服务_.动态服务.关联动作结果(*裸动作, *变化.动态节点, 新动作结果关系类型::导致变化)
                != 新动态操作状态::已完成)
            throw std::logic_error("atomic send actual change completion failed");
        核验并处理已发布动态(请求.定位.场景节点, *变化.动态节点);
        const auto 已入列 = 读取输出队列成员(请求.定位);
        if (已入列.size() != 原队列.size() + 1 || 已入列.back().消息节点 != 裸消息)
            throw std::logic_error("atomic send queue readback mismatch");
        服务_.特征服务.结束顶层I64更新(*准备.准备);
        return {{新外设输出队列状态::已入列, 消息, 后节点, 变化.动态节点}, 裸动作};
    } catch (const std::exception& e) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子字段发送", L"组合提交或后续治理工程故障，保留已提交事实。", e.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"原子字段发送", L"未知工程故障，保留已提交事实。");
    }
}

新外设输出队列结果 新_外设世界树类::提交输出(
    const 新外设输出提交请求& 请求) noexcept {
    if (请求.UTF8正文.empty()) return {新外设输出队列状态::目标不符, {}};
    const wchar_t* 阶段 = L"正式输出输入、依赖及窗口定位核验失败；保留已发生事实，不重试。";
    try {
        if (!新_语言类::严格UTF8字节串有效(请求.UTF8正文))
            return {新外设输出队列状态::目标不符, {}};
        std::lock_guard 锁(外设互斥);
        if (!有效(外设根) || !有效(消息根))
            return {新外设输出队列状态::未初始化, {}};
        if (!场景_.是场景节点(请求.定位.场景节点))
            return {新外设输出队列状态::来源不存在, {}};
        if (读取窗口定位(请求.定位.场景节点) != 请求.定位
            || !请求.定位.输出队列特征)
            return {新外设输出队列状态::目标不符, {}};
        阶段 = L"正式输出动作来源核验失败；保留已发生事实，不重试。";
        if (请求.来源动作) {
            const auto a=服务_.动态服务.获取动态(*请求.来源动作);
            if (!a || a->种类!=新动态种类::动作 || a->动作目标存在节点!=请求.定位.窗口节点
                || a->动作目标特征节点!=请求.定位.输出队列特征)
                throw std::runtime_error("peripheral output source action does not match queue target");
        }
        阶段 = L"正式输出的场景、会话及页面来源读取失败；保留已发生事实，不重试。";
        const auto 场景 = 场景_.获取场景(请求.定位.场景节点);
        if (!场景) throw std::runtime_error("peripheral output scene unavailable");
        const auto 会话 = 读取文本特征(读取类型特征(场景->特征节点组, 类型[0]));
        const auto 页面 = 读取文本特征(读取类型特征(场景->特征节点组, 类型[1]));
        // 消息本体保留来源身份；已领取、取消或清空的记录仍占用序号。
        // 高水位只从正式记录重建，与入列共用外设锁，不维护第二计数权威。
        std::uint64_t 最大序号 = 0;
        std::set<std::uint64_t> 已有序号;
        阶段 = L"正式输出既有消息身份与来源序号扫描失败；保留已发生事实，不重试。";
        const auto 根 = 读取消息快照(消息根);
        for (const auto& 字段 : 根.结构.字段组) {
            if (字段.字段节点 != 消息字段[10])
                throw std::runtime_error("peripheral message root corrupt");
            const auto 消息 = 读取正式消息(std::get<稳定编码>(字段.内容));
            if (!消息) throw std::runtime_error("peripheral message root holds invalid message");
            if (消息->来源.方向 != 新外设消息方向::输出
                || 消息->来源.公开宿主会话 != 会话 || 消息->来源.对话框标识 != 页面)
                continue;
            if (消息->场景节点 != 请求.定位.场景节点 || 消息->窗口节点 != 请求.定位.窗口节点
                || !已有序号.insert(消息->来源.来源序号).second)
                throw std::runtime_error("peripheral output source identity conflicts");
            最大序号 = (std::max)(最大序号, 消息->来源.来源序号);
        }
        阶段 = L"正式输出来源序号发布前复核失败；保留已发生事实，不重试。";
        if (!消息快照相同(根, 读取消息快照(消息根)))
            throw std::runtime_error("peripheral message index changed while allocating output source");
        if (最大序号 == (std::numeric_limits<std::uint64_t>::max)())
            throw std::runtime_error("peripheral output source sequence exhausted");
        阶段 = L"正式输出消息建立、队列更新或状态动态读回失败；保留已发生事实，不重试。";
        return 输出入列_已加锁({请求.定位, 最大序号 + 1, 请求.UTF8正文},请求.来源动作);
    } catch (const std::exception& 异常) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"提交外设输出", 阶段, 异常.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"提交外设输出", 阶段);
    }
}

新外设输出队列结果 新_外设世界树类::输出入列_已加锁(
    const 新外设输出入列请求& 请求, std::optional<稳定编码> 来源动作) {
    // 两个公开入口均已核验正文并持有外设互斥，此处不得再次获取同锁。
    if (!有效(外设根) || !有效(消息根))
        return {新外设输出队列状态::未初始化, {}};
    if (!场景_.是场景节点(请求.定位.场景节点))
        return {新外设输出队列状态::来源不存在, {}};
    if (读取窗口定位(请求.定位.场景节点) != 请求.定位
        || !请求.定位.输出队列特征)
        return {新外设输出队列状态::目标不符, {}};
    const auto 场景 = 场景_.获取场景(请求.定位.场景节点);
    if (!场景) throw std::runtime_error("peripheral output scene unavailable");
    const 新外设消息来源键 来源{
        读取文本特征(读取类型特征(场景->特征节点组, 类型[0])),
        读取文本特征(读取类型特征(场景->特征节点组, 类型[1])),
        新外设消息方向::输出, 请求.提交序号};
    auto 成员 = 读取输出队列成员(请求.定位);
    if (auto 旧 = 按来源查找消息(来源)) {
        if (旧->场景节点 != 请求.定位.场景节点
            || 旧->窗口节点 != 请求.定位.窗口节点
            || 旧->UTF8正文 != 请求.UTF8正文)
            throw std::runtime_error("peripheral output source conflicts with immutable record");
        const auto 在列 = std::find_if(成员.begin(), 成员.end(),
            [&](const auto& 项) { return 项.消息节点 == 旧->消息节点; });
        return {在列 == 成员.end() ? 新外设输出队列状态::不在队列
            : 新外设输出队列状态::已复用, std::move(*旧)};
    }
    auto 消息 = 建立输出消息(请求.定位, 来源, 请求.UTF8正文);
    成员.push_back(消息);
    const auto change=更新输出队列(请求.定位, 成员,来源动作);
    return {新外设输出队列状态::已入列, std::move(消息),change.状态节点,change.动态节点};
}

新外设输出队列结果 新_外设世界树类::领取输出队首(
    const 新外设对话框定位& 定位) noexcept {
    try {
        std::lock_guard 锁(外设互斥);
        if (!有效(外设根)) return {新外设输出队列状态::未初始化, {}};
        if (!场景_.是场景节点(定位.场景节点))
            return {新外设输出队列状态::来源不存在, {}};
        if (读取窗口定位(定位.场景节点) != 定位)
            return {新外设输出队列状态::目标不符, {}};
        auto 成员 = 读取输出队列成员(定位);
        if (成员.empty()) return {新外设输出队列状态::队列为空, {}};
        auto 消息 = std::move(成员.front());
        成员.erase(成员.begin());
        更新输出队列(定位, 成员);
        return {新外设输出队列状态::已领取, std::move(消息)};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"领取外设输出队首",
            L"正式输出队列出列或读回失败，保留已发生事实，不伪称空队列。");
    }
}

新外设输出队列结果 新_外设世界树类::移除待发输出(
    const 新外设对话框定位& 定位, std::uint64_t 提交序号) noexcept {
    if (!提交序号) return {新外设输出队列状态::目标不符, {}};
    try {
        std::lock_guard 锁(外设互斥);
        if (!有效(外设根)) return {新外设输出队列状态::未初始化, {}};
        if (!场景_.是场景节点(定位.场景节点))
            return {新外设输出队列状态::来源不存在, {}};
        if (读取窗口定位(定位.场景节点) != 定位)
            return {新外设输出队列状态::目标不符, {}};
        const auto 场景 = 场景_.获取场景(定位.场景节点);
        if (!场景) throw std::runtime_error("peripheral output scene unavailable");
        const 新外设消息来源键 来源{
            读取文本特征(读取类型特征(场景->特征节点组, 类型[0])),
            读取文本特征(读取类型特征(场景->特征节点组, 类型[1])),
            新外设消息方向::输出, 提交序号};
        auto 消息 = 按来源查找消息(来源);
        if (!消息) return {新外设输出队列状态::来源不存在, {}};
        if (消息->场景节点 != 定位.场景节点 || 消息->窗口节点 != 定位.窗口节点)
            throw std::runtime_error("peripheral output source location conflict");
        auto 成员 = 读取输出队列成员(定位);
        const auto 位置 = std::find_if(成员.begin(), 成员.end(),
            [&](const auto& 项) { return 项.消息节点 == 消息->消息节点; });
        if (位置 == 成员.end())
            return {新外设输出队列状态::不在队列, std::move(消息)};
        成员.erase(位置);
        更新输出队列(定位, 成员);
        return {新外设输出队列状态::已移除, std::move(消息)};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"移除待发外设输出",
            L"正式输出队列取消或读回失败，保留已发生事实。");
    }
}

新外设输出队列结果 新_外设世界树类::清空待发输出(
    const 新外设对话框定位& 定位) noexcept {
    try {
        std::lock_guard 锁(外设互斥);
        if (!有效(外设根)) return {新外设输出队列状态::未初始化, {}};
        if (!场景_.是场景节点(定位.场景节点))
            return {新外设输出队列状态::来源不存在, {}};
        if (读取窗口定位(定位.场景节点) != 定位)
            return {新外设输出队列状态::目标不符, {}};
        const auto 成员 = 读取输出队列成员(定位);
        if (!成员.empty()) 更新输出队列(定位, {});
        return {新外设输出队列状态::已清空, {}};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"清空待发外设输出",
            L"正式输出队列清空或读回失败，保留已发生事实。");
    }
}

新外设输出队列读取结果 新_外设世界树类::读取输出队列(
    const 新外设对话框定位& 定位) const noexcept {
    try {
        std::lock_guard 锁(外设互斥);
        if (!有效(外设根)) return {新外设输出队列状态::未初始化, {}, {}};
        if (!场景_.是场景节点(定位.场景节点))
            return {新外设输出队列状态::来源不存在, {}, {}};
        if (读取窗口定位(定位.场景节点) != 定位)
            return {新外设输出队列状态::目标不符, {}, {}};
        return {新外设输出队列状态::已读取, 定位.输出队列特征,
            读取输出队列成员(定位)};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取外设输出队列",
            L"有序队列材料损坏，不返回部分或伪空队列。");
    }
}
} // namespace 海中鱼巣
