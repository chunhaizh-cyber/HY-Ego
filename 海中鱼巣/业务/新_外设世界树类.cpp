#include "新_外设世界树类.h"
#include "../核心/容错检查.h"

#include <array>
#include <algorithm>
#include <chrono>
#include <limits>
#include <mutex>
#include <stdexcept>

namespace 海中鱼巣 {
namespace {
稳定编码 外设根{};
std::array<稳定编码, 6> 类型{};
std::mutex 外设互斥;

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
        || (存在材料->特征节点组.size() != 2 && 存在材料->特征节点组.size() != 4)
        || !存在材料->子存在节点组.empty())
        throw std::runtime_error("peripheral window structure incomplete");
    新外设对话框定位 结果{场景节点, 存在材料->节点, 读取类型特征(存在材料->特征节点组, 类型[2]),
        读取类型特征(存在材料->特征节点组, 类型[3])};
    if (存在材料->特征节点组.size() == 4) {
        结果.发送数量特征 = 读取类型特征(存在材料->特征节点组, 类型[4]);
        结果.发送内容特征 = 读取类型特征(存在材料->特征节点组, 类型[5]);
    }
    return 结果;
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
        if (命中) return {新外设事实状态::无变化, 命中};
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
        return {新外设事实状态::已完成, 读取窗口定位(*场景材料.场景节点)};
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

新外设事实状态 新_外设世界树类::更新接收(const 新外设对话框定位& 定位,
    std::string_view 实际UTF8正文, const 新状态强时间& 接收时间) noexcept {
    if (实际UTF8正文.empty()) return 新外设事实状态::入口拒绝;
    try {
        std::lock_guard 锁(外设互斥);
        if (!有效(外设根)) return 新外设事实状态::未初始化;
        if (读取窗口定位(定位.场景节点) != 定位)
            throw std::runtime_error("peripheral receive location changed");
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
        if (!后 || !后->当前值 || *后->当前值 != 新特征准确值{*数量 + 1}
            || 读取文本特征(定位.接收内容特征) != 实际UTF8正文)
            throw std::runtime_error("peripheral receive readback failed");
        return 新外设事实状态::已完成;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"记录外设接收",
            L"实际接收后特征发布或读回失败，保留已发生事实，不作为可重发否定。");
    }
}
新外设事实状态 新_外设世界树类::更新发送(const 新外设对话框定位& 定位,
    std::string_view 实际UTF8正文, const 新状态强时间& 发布时间) noexcept {
    if (实际UTF8正文.empty()) return 新外设事实状态::入口拒绝;
    try {
        if (!新_语言类::严格UTF8字节串有效(std::string(实际UTF8正文)))
            return 新外设事实状态::入口拒绝;
        std::lock_guard 锁(外设互斥);
        if (!有效(外设根)) return 新外设事实状态::未初始化;
        if (读取窗口定位(定位.场景节点) != 定位 || !定位.发送数量特征 || !定位.发送内容特征)
            throw std::runtime_error("outbound current page missing formal send features");
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
} // namespace 海中鱼巣
