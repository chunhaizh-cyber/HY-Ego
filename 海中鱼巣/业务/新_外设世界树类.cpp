#include "新_外设世界树类.h"
#include "../核心/容错检查.h"
#include "../领域/世界挂接.全局根.h"
#include "../领域/数据服务.外设词典记录类.h"

#include <array>
#include <algorithm>
#include <chrono>
#include <cwchar>
#include <functional>
#include <limits>
#include <map>
#include <mutex>
#include <new>
#include <stdexcept>
#include <set>
#include <tuple>
#include <thread>

namespace 海中鱼巣 {
namespace {
稳定编码 外设根{};
std::array<稳定编码, 6> 类型{};
std::array<稳定编码, 2> 消息类型{};
稳定编码 输出队列类型{};
稳定编码 词典批次类型{};
// 先分配记录再发表C；此映射只保存尚未完成的创建责任，不是场景类别事实。
std::map<std::string, 稳定编码, std::less<>> 待完成词典场景;
// 私有消息结构的字段键；消息不是普通存在，不能借用存在身份。
std::array<稳定编码, 12> 消息字段{};
稳定编码 消息根{};
const 新_特征值类* 消息值服务 = nullptr;
std::mutex 外设互斥;

基础联合节点快照 读取消息快照(稳定编码 节点) {
    const auto 结果 = 全局基础数据集.查询节点组完整结构({节点});
    if (结果.状态 == 基础结构操作状态::资源失败) throw std::bad_alloc{};
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
    if (结构.状态 == 基础结构操作状态::资源失败) throw std::bad_alloc{};
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
    if (批.状态 == 基础结构操作状态::资源失败) throw std::bad_alloc{};
    if (批.状态 != 基础结构操作状态::已完成 || 批.节点组.size() != 5)
        throw std::runtime_error("peripheral message joint identity read failed");
    std::vector<基础联合节点快照> 快照;
    for (const auto& 项 : 批.节点组) {
        if (!项.快照) throw std::runtime_error("peripheral message identity endpoint disappeared");
        if (项.查询节点 == 节点 && 项.快照->结构.字段组 != 消息.结构.字段组)
            throw std::runtime_error("peripheral immutable message content changed");
        if (保护) 快照.push_back(*项.快照);
    }
    if (保护) 保护->insert(保护->end(), 快照.begin(), 快照.end());
    return 结果;
}
class 外设消息身份读取器 final : public 特征消息成员读取接口 {
public:
    特征消息成员读取结果 读取消息集合成员身份(稳定编码 节点) const noexcept override {
        try {
            const auto dictionary = 外设词典记录类::取得稳定消息身份读取接口().读取消息集合成员身份(节点);
            if (dictionary.状态 != 特征概念规则状态::入口拒绝) return dictionary;
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
        std::unique_lock 锁(外设互斥);
        if (!世界_.已初始化()) return {新世界初始化状态::依赖初始化失败, {}, {}};
        特征概念定义 批次定义;
        批次定义.材料类型 = 特征概念材料物理类型::UTF8字符串;
        批次定义.值域 = 特征概念全材料值域{};
        批次定义.比较规则 = 特征概念比较规则::完整材料精确;
        批次定义.聚合规则 = 特征概念聚合规则::不自动聚合;
        for (;;) {
            const auto ft = 服务_.特征概念服务.建立或取得先天特征类型槽(
                {先天特征类型槽::词典来源批次, 批次定义}, 服务_.特征值服务, {});
            if (ft.状态 == 特征概念规则状态::前提已变化) {
                锁.unlock(); std::this_thread::yield(); 锁.lock(); continue;
            }
            if (ft.状态 == 特征概念规则状态::资源失败)
                return {新世界初始化状态::资源失败, {}, {}};
            if ((ft.状态 != 特征概念规则状态::已完成 && ft.状态 != 特征概念规则状态::无变化)
                || !ft.类型根) throw std::runtime_error("dictionary batch type initialization failed");
            词典批次类型 = *ft.类型根;
            break;
        }
        for (auto& 字段 : 消息字段) {
            if (!有效(字段)) 字段 = 全局基础数据集.新建节点();
            if (!有效(字段)) throw std::runtime_error("peripheral message schema allocation failed");
        }
        if (!有效(消息根)) 消息根 = 全局基础数据集.新建节点();
        if (!有效(消息根)) throw std::runtime_error("peripheral message root allocation failed");
        if (消息值服务 && 消息值服务 != &服务_.特征值服务)
            throw std::runtime_error("peripheral message value owner changed");
        消息值服务 = &服务_.特征值服务;
        外设词典记录类 词典记录(服务_.特征值服务);
        const auto 词典初始化 = 词典记录.初始化();
        if (词典初始化 != 新词典记录操作状态::已完成 && 词典初始化 != 新词典记录操作状态::已复用)
            throw std::runtime_error("dictionary message schema initialization failed");
        服务_.特征概念服务.装配消息成员读取接口(&消息读取器);
        const bool 已有 = 有效(外设根);
        if (!场景_.初始化()) throw std::runtime_error("peripheral scene initialization failed");
        const auto 根 = 场景_.建立根场景();
        if (!根.场景节点 || !根.专属存在概念节点
            || (根.状态 != 新场景操作状态::已完成 && 根.状态 != 新场景操作状态::无变化)
            || 世界_.获取世界树根场景() == 根.场景节点)
            throw std::runtime_error("peripheral root invalid");
        const auto 挂接 = 新_全局根类::挂接世界根(世界类型登记键::外设, *根.场景节点);
        if (挂接 != 全局根操作状态::已完成 && 挂接 != 全局根操作状态::无变化)
            throw std::runtime_error("peripheral root global attachment failed");
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

std::optional<稳定编码> 新_外设世界树类::获取接收消息特征类型() const noexcept {
    std::lock_guard lock(外设互斥);
    if (!有效(外设根)) return std::nullopt;
    if (!有效(消息类型[0]))
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取接收消息类型", L"外设根已初始化但接收消息类型缺失");
    return 消息类型[0];
}

struct 新_外设世界树类::场景来源身份 final {
    enum class 类别 { 非本树, 网页, 词典, 正在装配 } 种类 = 类别::非本树;
    std::optional<新场景信息> 场景;
    std::optional<稳定编码> 批次F, 接收F;
};

新_外设世界树类::场景来源身份 新_外设世界树类::读取场景来源身份(稳定编码 C) const {
    for (;;) {
        场景来源身份 out;
        out.场景 = 场景_.获取场景(C);
        if (!out.场景 || out.场景->父场景节点 != 外设根) return out;
        bool webpage = false, refresh = false;
        for (const auto f : out.场景->特征节点组) {
            const auto type = 服务_.特征服务.读取当前特征类型(f);
            if (type.状态 == 新特征类型读取状态::特征不存在) {
                const auto latest = 场景_.获取场景(C);
                if (latest && std::find(latest->特征节点组.begin(), latest->特征节点组.end(), f)
                    != latest->特征节点组.end())
                    throw std::runtime_error("peripheral scene still contains a missing feature");
                refresh = true; break;
            }
            if (type.状态 == 新特征类型读取状态::资源失败) throw std::bad_alloc{};
            if (type.状态 != 新特征类型读取状态::已读取 || !type.特征类型根)
                throw std::runtime_error("peripheral scene feature type unavailable");
            if (*type.特征类型根 == 类型[0] || *type.特征类型根 == 类型[1]) webpage = true;
            auto save = [&](std::optional<稳定编码>& target) {
                if (target) throw std::runtime_error("peripheral scene has duplicate source feature types");
                target = f;
            };
            if (*type.特征类型根 == 词典批次类型) save(out.批次F);
            if (*type.特征类型根 == 消息类型[0]) save(out.接收F);
        }
        if (refresh) continue;
        if (webpage && out.批次F) throw std::runtime_error("peripheral dictionary and web identities overlap");
        if (webpage) out.种类 = 场景来源身份::类别::网页;
        else if (out.批次F) out.种类 = 场景来源身份::类别::词典;
        else if (std::any_of(待完成词典场景.begin(), 待完成词典场景.end(),
            [&](const auto& item) { return item.second == C; })) out.种类 = 场景来源身份::类别::正在装配;
        else throw std::runtime_error("peripheral child scene has no source identity");
        return out;
    }
}

新外设词典来源结果 新_外设世界树类::读取词典来源已加锁(稳定编码 C) const {
    using S = 新外设事实状态;
    for (;;) {
        auto source = 读取场景来源身份(C);
        if (source.种类 == 场景来源身份::类别::正在装配) return {S::未就绪, {}};
        if (source.种类 != 场景来源身份::类别::词典) return {S::未找到, {}};
        const auto batch = 服务_.特征服务.获取特征(*source.批次F);
        if (!batch) continue;
        if (!batch->当前值) return {S::未就绪, {}};
        const auto* V = std::get_if<稳定编码>(&*batch->当前值);
        if (!V) throw std::runtime_error("dictionary batch is not a text material reference");
        const auto value = 服务_.特征值服务.获取特征值(*V);
        if (!value) {
            const auto latest = 服务_.特征服务.获取特征(*source.批次F);
            if (!latest || latest->当前值 != batch->当前值) continue;
            throw std::runtime_error("dictionary batch still refers to missing material");
        }
        const auto* text = std::get_if<std::string>(&value->材料);
        if (!text || value->物理类型 != 材料物理类型::UTF8字符串 || text->empty())
            throw std::runtime_error("dictionary source batch material invalid");
        if (!source.场景->直接子场景组.empty() || source.场景->直接成员存在组.size() > 1)
            throw std::runtime_error("dictionary source scene has ambiguous speaker structure");
        if (!source.接收F || source.场景->直接成员存在组.empty()) return {S::未就绪, {}};
        const auto speaker = source.场景->直接成员存在组.front();
        // 唯一成员在本来源场景合同中明确承担词典说话者，不在多个候选中取首个。
        if (!服务_.存在服务.获取存在(speaker)) {
            const auto latest = 场景_.获取场景(C);
            if (!latest || latest->直接成员存在组 != source.场景->直接成员存在组) continue;
            throw std::runtime_error("dictionary source still contains a missing speaker");
        }
        return {S::已完成, 新外设词典来源定位{C, speaker, *source.接收F, *source.批次F, *text}};
    }
}

新外设词典来源结果 新_外设世界树类::按场景读取词典来源(稳定编码 C) const noexcept {
    try {
        std::lock_guard lock(外设互斥);
        if (!有效(外设根)) return {新外设事实状态::未初始化, {}};
        if (!有效(C)) return {新外设事实状态::入口拒绝, {}};
        return 读取词典来源已加锁(C);
    } catch (const std::bad_alloc&) { return {新外设事实状态::资源失败, {}}; }
    catch (const std::length_error&) { return {新外设事实状态::资源失败, {}}; }
    catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取词典来源场景", L"来源结构损坏，不伪装未配置或未命中。");
    }
}

新外设词典来源结果 新_外设世界树类::建立或取得词典来源场景(std::string_view batch) noexcept {
    using S = 新外设事实状态;
    try {
        if (batch.empty() || !新_语言类::严格UTF8字节串有效(std::string(batch))) return {S::入口拒绝, {}};
        std::unique_lock lock(外设互斥);
        if (!有效(外设根) || !有效(词典批次类型)) return {S::未初始化, {}};
        // map分配在任何C发表之前；后续记录C编码不分配，也不撤销既有事实。
        auto pending = 待完成词典场景.try_emplace(std::string(batch), 稳定编码{}).first;
        bool changed = false;
        const auto reread = [&] {
            lock.unlock(); std::this_thread::yield(); lock.lock();
            pending = 待完成词典场景.try_emplace(std::string(batch), 稳定编码{}).first;
        };
        for (;;) {
            const auto root = 场景_.获取场景(外设根);
            if (!root) throw std::runtime_error("dictionary peripheral root unavailable");
            std::optional<稳定编码> found;
            bool refresh = false;
            for (const auto C : root->直接子场景组) {
                const auto source = 读取场景来源身份(C);
                if (source.种类 != 场景来源身份::类别::词典) continue;
                const auto F = 服务_.特征服务.获取特征(*source.批次F);
                if (!F) { refresh = true; break; }
                if (!F->当前值) {
                    if (pending->second == C) continue;
                    throw std::runtime_error("dictionary source batch identity was lost");
                }
                if (读取文本特征(*source.批次F) != batch) continue;
                if (found && *found != C) throw std::runtime_error("dictionary batch has multiple source scenes");
                found = C;
            }
            if (refresh) {
                reread();
                continue;
            }
            if (found) {
                if (有效(pending->second) && pending->second != *found)
                    throw std::runtime_error("pending dictionary source conflicts with published batch");
                pending->second = *found;
            }
            if (!有效(pending->second)) {
                const auto made = 场景_.建立场景(外设根);
                if (made.场景节点) pending->second = *made.场景节点;
                if (made.状态 == 新场景操作状态::资源失败) return {S::资源失败, {}};
                if (made.状态 != 新场景操作状态::已完成 || !有效(pending->second))
                    throw std::runtime_error("dictionary source scene creation failed");
                changed = true;
            }
            const auto C = pending->second;
            auto source = 读取场景来源身份(C);
            if (!source.场景) throw std::runtime_error("pending dictionary source disappeared during assembly");
            const auto require = [](新场景操作状态 state) {
                if (state == 新场景操作状态::资源失败) throw std::bad_alloc{};
                if (state != 新场景操作状态::已完成 && state != 新场景操作状态::无变化)
                    throw std::runtime_error("dictionary source assembly step failed");
            };
            const auto featureAdded = [&](const 新场景特征添加结果& result) {
                // 场景层的存在建立失败会折叠底层原因；仅零发表的变化可以重读。
                if (result.存在结果 && !result.存在结果->特征节点 && !result.状态节点) {
                    const auto state = result.存在结果->状态;
                    if (state == 新存在操作状态::前提已变化
                        || state == 新存在操作状态::特征联合更新占用) return false;
                    if (state == 新存在操作状态::资源失败) throw std::bad_alloc{};
                }
                require(result.状态);
                return true;
            };
            if (!source.批次F) {
                const auto V = 服务_.特征值服务.保存或取得特征值(材料物理类型::UTF8字符串, std::string(batch));
                if (!有效(V)) throw std::bad_alloc{};
                if (!featureAdded(场景_.添加场景特征(C, 词典批次类型, V, 当前强时间()))) {
                    reread(); continue;
                }
                changed = true;
            }
            source = 读取场景来源身份(C);
            if (!source.接收F) {
                if (!featureAdded(场景_.添加场景特征区间值(C, 消息类型[0], 特征概念全材料值域{}))) {
                    reread(); continue;
                }
                changed = true;
            }
            source = 读取场景来源身份(C);
            if (source.场景->直接成员存在组.empty()) {
                require(场景_.建立区间成员存在(C, {std::nullopt, {}}).状态);
                changed = true;
            }
            auto ready = 读取词典来源已加锁(C);
            if (ready.状态 != S::已完成 || !ready.定位)
                throw std::runtime_error("dictionary source assembly did not yield a complete location");
            待完成词典场景.erase(pending);
            if (!changed) ready.状态 = S::无变化;
            return ready;
        }
    } catch (const std::bad_alloc&) { return {S::资源失败, {}}; }
    catch (const std::length_error&) { return {S::资源失败, {}}; }
    catch (const std::exception& error) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立词典来源场景", L"来源装配未完成，保留已发表场景及待完成责任。", error.what());
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"建立词典来源场景", L"来源装配工程异常，保留已发表事实。");
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

std::pair<稳定编码, std::optional<稳定编码>> 新_外设世界树类::区分窗口与交互者(
    const 新场景信息& 场景材料) const {
    const auto& 成员 = 场景材料.直接成员存在组;
    if (成员.empty() || 成员.size() > 2) throw std::runtime_error("peripheral dialogue member count invalid");
    const auto 持有窗口特征 = [&](稳定编码 存在节点, bool& 有接收数量) {
        const auto 存在材料 = 服务_.存在服务.获取存在(存在节点);
        if (!存在材料) throw std::runtime_error("peripheral dialogue member unreadable");
        bool 有窗口特征 = false;
        for (const auto 特征节点 : 存在材料->特征节点组) {
            const auto 特征 = 服务_.特征服务.获取特征(特征节点);
            const auto 根 = 特征 ? 服务_.特征概念服务.比较单特征概念(
                特征->特征概念节点, 特征->特征概念节点).特征类型根 : std::nullopt;
            if (!根) throw std::runtime_error("peripheral dialogue member feature type unreadable");
            if (*根 == 类型[2]) 有接收数量 = true;
            for (const auto 窗口类型 : {类型[2], 类型[3], 类型[4], 类型[5], 消息类型[0], 消息类型[1], 输出队列类型})
                if (*根 == 窗口类型) 有窗口特征 = true;
        }
        return 有窗口特征;
    };
    std::optional<稳定编码> 窗口, 交互者;
    for (const auto 存在节点 : 成员) {
        bool 有接收数量 = false;
        const bool 有窗口特征 = 持有窗口特征(存在节点, 有接收数量);
        if (有接收数量) {
            if (窗口) throw std::runtime_error("peripheral dialogue window duplicated");
            窗口 = 存在节点;
        } else {
            if (有窗口特征 || 交互者) throw std::runtime_error("peripheral dialogue interactor invalid");
            交互者 = 存在节点;
        }
    }
    if (!窗口) throw std::runtime_error("peripheral dialogue window missing");
    return {*窗口, 交互者};
}

新外设对话框定位 新_外设世界树类::读取窗口定位(稳定编码 场景节点) const {
    const auto 场景材料 = 场景_.获取场景(场景节点);
    if (!场景材料 || 场景材料->父场景节点 != 外设根
        || !场景材料->直接子场景组.empty() || 场景材料->特征节点组.size() != 2)
        throw std::runtime_error("peripheral dialogue scene incomplete");
    (void)读取文本特征(读取类型特征(场景材料->特征节点组, 类型[0]));
    (void)读取文本特征(读取类型特征(场景材料->特征节点组, 类型[1]));
    const auto 存在材料 = 服务_.存在服务.获取存在(区分窗口与交互者(*场景材料).first);
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
        if (读取场景来源身份(节点).种类 != 场景来源身份::类别::网页) continue;
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
            if (读取场景来源身份(场景节点).种类 != 场景来源身份::类别::网页) continue;
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

std::vector<稳定编码> 新_外设世界树类::列出交互场景() const noexcept {
    try {
        std::lock_guard 锁(外设互斥);
        if (!有效(外设根)) return {};
        const auto 根 = 场景_.获取场景(外设根);
        if (!根) throw std::runtime_error("peripheral root unreadable");
        std::vector<稳定编码> 结果;
        for (const auto 场景节点 : 根->直接子场景组) {
            if (读取场景来源身份(场景节点).种类 != 场景来源身份::类别::网页) continue;
            (void)读取窗口定位(场景节点);
            结果.push_back(场景节点);
        }
        return 结果;
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"列出外设交互场景", L"正式窗口定位结构不完整。");
    }
}

std::optional<新外设交互场景读取> 新_外设世界树类::按场景读取交互场景(
    稳定编码 场景节点) const noexcept {
    if (!有效(场景节点)) return std::nullopt;
    try {
        std::lock_guard 锁(外设互斥);
        if (!有效(外设根)) return std::nullopt;
        const auto 根 = 场景_.获取场景(外设根);
        if (!根) throw std::runtime_error("peripheral root unreadable");
        if (std::find(根->直接子场景组.begin(), 根->直接子场景组.end(), 场景节点) == 根->直接子场景组.end())
            return std::nullopt;
        if (读取场景来源身份(场景节点).种类 != 场景来源身份::类别::网页) return std::nullopt;
        const auto 定位 = 读取窗口定位(场景节点);
        const auto 场景材料 = 场景_.获取场景(场景节点);
        if (!场景材料) throw std::runtime_error("peripheral scene unreadable");
        return 新外设交互场景读取{
            {定位, 读取文本特征(读取类型特征(场景材料->特征节点组, 类型[0])),
                读取文本特征(读取类型特征(场景材料->特征节点组, 类型[1]))},
            区分窗口与交互者(*场景材料).second};
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"按场景读取外设交互场景", L"正式窗口定位结构不完整。");
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
        // INTERACT-SCENE-INSTINCT（2026-11-09用户裁决）：对方打开窗口即进入交互状态——窗口建立时同时派生
        // 交互者（存在根零特征，与窗口并列），不再等首条消息。接收路径的派生保留为补齐旧场景。
        const auto 交互者 = 场景_.建立区间成员存在(*场景材料.场景节点, {std::nullopt, {}});
        if (交互者.状态 != 新场景操作状态::已完成 || !交互者.存在节点)
            throw std::runtime_error("peripheral interactor derivation at window open failed");
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
    if ((前 && (!特征->当前值 || 前->固定准确值 != 特征->当前值))
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
    // 三场景分工（7240 §1）：来源动作作用于内部的交互窗口副本时，外设只记录执行事实（消息本体已保留来源动作），
    // 不跨场景建立“导致变化”关联；输出队列新增问话属顺带、不入结果组（5380 §3.6）。只有动作直接作用于外设窗口时关联。
    const auto 来源动作本体 = 来源动作 ? 服务_.动态服务.获取动态(*来源动作) : std::nullopt;
    if (来源动作 && 来源动作本体 && 来源动作本体->动作目标存在节点 == 定位.窗口节点) {
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
    if (!前 || !前->固定准确值 || !原特征 || 原特征->当前值 != 前->固定准确值
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
    新消息待发表凭据 凭据{{裸节点}, 请求.预期联合节点组, *封存.片段};
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

namespace {
std::mutex 外设事实变化端口互斥;
std::function<void(稳定编码)> 外设事实变化端口;
// 调用者已释放外设互斥；端口调用期间持端口互斥，解除绑定等待在途投递结束。
void 投递外设事实变化(const std::vector<稳定编码>& 状态组) {
    std::lock_guard 锁(外设事实变化端口互斥);
    if (!外设事实变化端口) return;
    for (const auto 节点 : 状态组) 外设事实变化端口(节点);
}
}

// DEP-READY-WAKE B（设计4.3②）：外设接收形成状态后投递“事实变化”的端口。装配阶段在治理线程
// 启动前绑定，上下文回收前以空函数解除；端口只转交状态身份，不携带结论。
void 绑定外设世界树事实变化端口(std::function<void(稳定编码)> 端口) noexcept {
    try {
        std::lock_guard 锁(外设事实变化端口互斥);
        外设事实变化端口 = std::move(端口);
    } catch (...) {
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"绑定外设事实变化端口", L"端口互斥或函数交换失败。");
    }
}

新外设事实状态 新_外设世界树类::更新接收(const 新外设对话框定位& 定位,
    std::uint64_t 来源序号, std::string_view 实际UTF8正文, const 新状态强时间& 接收时间) noexcept {
    if (实际UTF8正文.empty()) return 新外设事实状态::入口拒绝;
    try {
        std::vector<稳定编码> 新状态组;
        {
        std::lock_guard 锁(外设互斥);
        if (!有效(外设根)) return 新外设事实状态::未初始化;
        if (读取窗口定位(定位.场景节点) != 定位)
            throw std::runtime_error("peripheral receive location changed");
        // 首次收到消息时派生交互者：以存在根零特征建立，与窗口并列（6120 §11.1，Q2=A）。
        if (const auto 场景材料 = 场景_.获取场景(定位.场景节点);
            !场景材料 || !区分窗口与交互者(*场景材料).second) {
            if (!场景材料) throw std::runtime_error("peripheral receive scene unreadable");
            const auto 交互者 = 场景_.建立区间成员存在(定位.场景节点, {std::nullopt, {}});
            if (交互者.状态 != 新场景操作状态::已完成 || !交互者.存在节点)
                throw std::runtime_error("peripheral interactor derivation failed");
        }
        const auto 消息结果 = 记录消息(定位, 新外设消息方向::接收, 来源序号, 实际UTF8正文, 接收时间);
        if (消息结果 == 新外设事实状态::无变化) return 消息结果;
        const auto 特征材料 = 服务_.特征服务.获取特征(定位.接收数量特征);
        const auto* 数量 = 特征材料 && 特征材料->当前值 ? std::get_if<std::int64_t>(&*特征材料->当前值) : nullptr;
        if (!数量 || *数量 < 0 || *数量 == (std::numeric_limits<std::int64_t>::max)())
            throw std::runtime_error("peripheral receive count invalid");
        const auto 值材料 = 服务_.特征值服务.保存或取得特征值(材料物理类型::UTF8字符串, std::string(实际UTF8正文));
        if (!有效(值材料)) throw std::runtime_error("peripheral receive publication incomplete");
        const auto 内容变化 = 更新窗口特征并形成动态(定位, 定位.接收内容特征, 值材料, 接收时间);
        if (!更新完成(内容变化)) throw std::runtime_error("peripheral receive publication incomplete");
        const auto 数量变化 = 更新窗口特征并形成动态(定位, 定位.接收数量特征, *数量 + 1, 接收时间);
        if (!更新完成(数量变化)) throw std::runtime_error("peripheral receive publication incomplete");
        if (内容变化.状态节点) 新状态组.push_back(*内容变化.状态节点);
        if (数量变化.状态节点) 新状态组.push_back(*数量变化.状态节点);
        const auto 后 = 服务_.特征服务.获取特征(定位.接收数量特征);
        const auto 当前内容 = 读取文本特征(定位.接收内容特征);
        if (!后 || !后->当前值 || *后->当前值 != 新特征准确值{*数量 + 1}
            || 当前内容 != 实际UTF8正文)
            throw std::runtime_error("peripheral receive readback failed");
        }
        投递外设事实变化(新状态组);
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

新分界消息读取结果 新_外设世界树类::读取分界消息正文(稳定编码 节点) const {
    using 状态 = 新语言分界工程状态;
    try {
        std::lock_guard 锁(外设互斥);
        if (!有效(消息根)) return {状态::未初始化, {}, {}};
        if (!有效(节点)) return {状态::输入拒绝, {}, {}};
        std::vector<基础联合节点快照> 快照组;
        auto 消息 = 读取正式消息(节点, &快照组);
        if (!消息) return {状态::未找到, {}, {}};
        if (消息->来源.方向 != 新外设消息方向::接收) return {状态::未支持, {}, {}};
        新分界消息读取结果 结果{状态::已完成, std::move(消息), {}};
        for (const auto& 快照 : 快照组) {
            const auto 节点值 = 快照.结构.节点;
            if (std::find(结果.发布前提.节点组.begin(), 结果.发布前提.节点组.end(), 节点值)
                == 结果.发布前提.节点组.end()) 结果.发布前提.节点组.push_back(节点值);
            if (节点值.编码 == 节点)
                结果.发布前提.字段集合组.push_back({节点, std::nullopt, {}, 快照.结构.字段组});
        }
        if (结果.发布前提.字段集合组.size() != 1
            || 结果.发布前提.字段集合组.front().字段组.size() != 10)
            throw std::logic_error("boundary message final projection missing");
        return 结果;
    } catch (const std::bad_alloc&) {
        return {状态::资源失败, {}, {}};
    } catch (const std::length_error&) {
        return {状态::资源失败, {}, {}};
    } catch (const std::exception& 异常) {
        wchar_t 原因[160]{};
        (void)swprintf_s(原因, L"正式接收消息窄投影读取失败；M=%llu。",
            static_cast<unsigned long long>(节点.值));
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取分界消息正文",
            原因, 异常.what());
    } catch (...) {
        wchar_t 原因[160]{};
        (void)swprintf_s(原因, L"正式接收消息窄投影发生未知工程异常；M=%llu。",
            static_cast<unsigned long long>(节点.值));
        工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"读取分界消息正文",
            原因);
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
        if (!前 || !前->固定准确值 || !特征 || 特征->当前值 != 前->固定准确值
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
            const bool 直接作用窗口 = a && a->动作目标存在节点==请求.定位.窗口节点
                && a->动作目标特征节点==请求.定位.输出队列特征;
            bool 作用对应副本 = false;
            if (a && 请求.来源动作窗口副本 && a->动作目标存在节点==请求.来源动作窗口副本 && a->动作目标特征节点) {
                const auto 副本 = 服务_.存在服务.获取存在(*请求.来源动作窗口副本);
                作用对应副本 = 副本 && std::count(副本->特征节点组.begin(), 副本->特征节点组.end(), *a->动作目标特征节点) == 1;
            }
            if (!a || a->种类!=新动态种类::动作 || (!直接作用窗口 && !作用对应副本))
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
            return {新外设输出队列状态::目标不符, {}};
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
namespace {
using 词典记录状态 = 新词典记录操作状态;
using 词典提交状态 = 基础节点组追加状态;
[[noreturn]] void 词典接收故障(const wchar_t* why) noexcept {
    工程异常终止(HY_EGO_WIDEN(__FILE__), __LINE__, L"词典场景接收", why);
}
词典提交状态 词典特征准备结局(新特征I64准备状态 s) {
    switch (s) {
    case 新特征I64准备状态::资源失败: return 词典提交状态::资源失败;
    case 新特征I64准备状态::预期已变化:
    case 新特征I64准备状态::特征不存在: return 词典提交状态::预期不符;
    case 新特征I64准备状态::联合更新占用:
        std::this_thread::yield(); return 词典提交状态::预期不符;
    default: 词典接收故障(L"接收消息F准备遇到非正常变化故障");
    }
}

class 真实词典接收准备 final : public 新词典接收参与准备 {
public:
    真实词典接收准备(新世界事实服务引用 s, 新_场景类& scene, 稳定编码 C,
        新词典接收准备投影 v, std::optional<新特征准确值> old,
        新特征状态构造绑定 stateBinding, 新场景状态参与绑定 sceneBinding,
        新消息集合保持材料 retention)
        : 服务(s), 场景(scene), 场景节点(C), 投影(v), 原当前值(std::move(old)),
          状态绑定(std::move(stateBinding)), 场景绑定(std::move(sceneBinding)), 保持(std::move(retention)) {}
    ~真实词典接收准备() override { 取消未发表准备(); }
    新词典接收准备投影 读取接收投影() const override { return 投影; }
    新词典接收提交结果 提交接收(新消息待发表凭据&& pending,
        新状态结果引用参与接口& participant) noexcept override {
        try {
            if (已使用 || 已结束) 词典接收故障(L"同一词典接收准备不得重复提交");
            已使用 = true;
            // 凭据必须尚未被F消费；保持请求与新S同一片段，不单独发表。
            auto extra = 服务.特征概念服务.构造消息集合成员保持(std::move(保持), pending);
            if (投影.前状态) {
                auto count = 服务.状态服务.准备引用计数参与({{*投影.前状态, 1}});
                if (count.状态 == 新状态操作状态::状态不存在) return {词典提交状态::预期不符, {}, {}};
                if (count.状态 == 新状态操作状态::资源失败) return {词典提交状态::资源失败, {}, {}};
                if (count.状态 != 新状态操作状态::已完成 || !count.准备)
                    词典接收故障(L"接收前态保持准备不合法");
                前态保持 = std::move(count.准备);
            }
            auto f = 服务.特征服务.准备顶层消息集合更新(投影.接收特征, 投影.新消息集合值, std::move(pending));
            if (f.状态 != 新特征I64准备状态::已准备) return {词典特征准备结局(f.状态), {}, {}};
            if (!f.准备) 词典接收故障(L"接收F准备成功但缺少句柄");
            特征准备 = std::move(f.准备);
            if (服务.特征服务.读取准备原当前值(*特征准备) != 原当前值)
                return {词典提交状态::预期不符, {}, {}};
            const auto candidate = 服务.特征服务.读取准备当前值候选(*特征准备);
            auto state = 服务.状态服务.准备特征状态(状态绑定, candidate, 场景节点, 投影.本轮状态时间, std::move(extra));
            服务.状态服务.合并结果引用(state, participant);
            const auto sealed = 场景.封存场景状态挂接(场景绑定, std::move(state));
            if (sealed.封存.状态 != 词典提交状态::已完成) return {sealed.封存.状态, {}, {}};
            if (!sealed.封存.片段) 词典接收故障(L"接收状态共同封存缺片段");
            std::vector<基础封存追加片段> fragments{*sealed.封存.片段};
            if (前态保持) fragments.push_back(服务.状态服务.封存计数参与(*前态保持));
            auto published = 服务.特征服务.提交顶层引用集合更新(*特征准备, fragments);
            if (published.状态 != 词典提交状态::已完成) return {published.状态, {}, {}};
            if (published.附加片段回执组.size() != fragments.size()) 词典接收故障(L"词典接收缺共同回执");
            const auto& receipt = published.附加片段回执组.front();
            if (!receipt.对应片段(fragments.front()) || sealed.状态槽 >= receipt.新节点组().size())
                词典接收故障(L"词典接收回执未覆盖确切S/B片段");
            const auto newState = receipt.新节点组()[sealed.状态槽];
            // F已登记自身及R/M必需片段成功；此处先无抛登记B及前S，再做任何收尾。
            participant.登记共同发表回执(receipt);
            if (前态保持) 服务.状态服务.登记计数发表(*前态保持, published.附加片段回执组[1]);
            已发表 = true;
            const auto completed = 服务.特征服务.完成顶层引用集合更新(*特征准备);
            if (completed.状态 != 新特征写入状态::已完成 || !completed.当前值已改变)
                词典接收故障(L"已发表词典接收F收尾失败，保留事实");
            服务.特征服务.结束顶层引用集合更新(*特征准备);
            if (前态保持) 服务.状态服务.完成计数参与(*前态保持);
            已结束 = true;
            // 所有owner准备已结束，才投递实际状态变化。
            投递外设事实变化({newState});
            return {词典提交状态::已完成, std::move(published.附加片段回执组), newState};
        } catch (词典提交状态 s) { return {s, {}, {}}; }
        catch (const std::bad_alloc&) {
            if (已发表) 词典接收故障(L"词典接收已发表后的资源故障，保留事实");
            return {词典提交状态::资源失败, {}, {}};
        } catch (const std::length_error&) {
            if (已发表) 词典接收故障(L"词典接收已发表后的容量故障，保留事实");
            return {词典提交状态::资源失败, {}, {}};
        } catch (...) { 词典接收故障(L"词典接收共同发表工程异常"); }
    }
    void 取消未发表准备() noexcept override {
        if (已结束 || 已发表) return;
        if (特征准备) 服务.特征服务.取消顶层引用集合更新(*特征准备);
        if (前态保持) 服务.状态服务.取消计数参与(*前态保持);
        已结束 = true;
    }
private:
    新世界事实服务引用 服务;
    新_场景类& 场景;
    稳定编码 场景节点;
    新词典接收准备投影 投影;
    std::optional<新特征准确值> 原当前值;
    新特征状态构造绑定 状态绑定;
    新场景状态参与绑定 场景绑定;
    新消息集合保持材料 保持;
    std::optional<新状态计数准备> 前态保持;
    std::optional<新特征引用集合更新准备> 特征准备;
    bool 已使用 = false, 已发表 = false, 已结束 = false;
};

class 真实词典接收提供者 final : public 新词典接收场景提供接口 {
public:
    真实词典接收提供者(新世界事实服务引用 s, 新_场景类& scene, 稳定编码 ft)
        : 服务(s), 场景(scene), 接收FT(ft) {}
    新词典接收准备结果 准备接收(稳定编码 C, const std::vector<稳定编码>& messages) noexcept override {
        using S = 词典记录状态;
        try {
            if (!有效(C) || messages.empty()) return {S::输入拒绝, {}};
            std::set<稳定编码> batch;
            for (const auto m : messages) if (!有效(m) || !batch.insert(m).second) return {S::输入拒绝, {}};
            for (;;) {
                // 先捕获C依据，再读取C中的F/状态；其后任何组织变化均由L1零发表处理。
                const auto sceneBinding = 场景.取得场景状态参与绑定(C);
                const auto currentScene = 场景.获取场景(C);
                if (!currentScene) return {S::未找到, {}};
                std::optional<稳定编码> F;
                bool refresh = false;
                for (const auto f : currentScene->特征节点组) {
                    const auto type = 服务.特征服务.读取当前特征类型(f);
                    if (type.状态 == 新特征类型读取状态::特征不存在) { refresh = true; break; }
                    if (type.状态 == 新特征类型读取状态::资源失败) return {S::资源失败, {}};
                    if (type.状态 != 新特征类型读取状态::已读取 || !type.特征类型根) return {S::未装配, {}};
                    if (*type.特征类型根 == 接收FT) {
                        if (F) 词典接收故障(L"词典场景存在重复接收消息F");
                        F = f;
                    }
                }
                if (refresh) continue;
                if (!F) return {S::未装配, {}};
                const auto feature = 服务.特征服务.获取特征(*F);
                if (!feature) continue;
                std::vector<std::uint64_t> members;
                if (feature->当前值) {
                    const auto* V = std::get_if<稳定编码>(&*feature->当前值);
                    if (!V) 词典接收故障(L"接收消息当前值不是集合材料");
                    const auto value = 服务.特征值服务.获取特征值(*V);
                    if (!value) {
                        const auto latest = 服务.特征服务.获取特征(*F);
                        if (!latest || latest->当前值 != feature->当前值) continue;
                        词典接收故障(L"当前接收F仍引用不存在的集合材料");
                    }
                    const auto* ids = std::get_if<std::vector<std::uint64_t>>(&value->材料);
                    if (!ids || value->物理类型 != 材料物理类型::U64数组)
                        词典接收故障(L"接收消息集合物理类型不合法");
                    members = *ids;
                }
                for (const auto m : messages) members.push_back(m.值);
                std::sort(members.begin(), members.end());
                members.erase(std::unique(members.begin(), members.end()), members.end());
                const auto V = 服务.特征值服务.保存或取得特征值(材料物理类型::U64数组, members);
                if (!有效(V)) return {S::资源失败, {}};
                auto retention = 服务.特征概念服务.读取消息集合保持材料(V, 服务.特征值服务);
                if (retention.状态 == 特征概念规则状态::值不存在) {
                    const auto latest = 服务.特征服务.获取特征(*F);
                    if (!latest || latest->当前值 != feature->当前值) continue;
                    词典接收故障(L"最新集合仍包含不存在的接收消息或私有身份");
                }
                if (retention.状态 == 特征概念规则状态::资源失败) return {S::资源失败, {}};
                if (retention.状态 == 特征概念规则状态::读取接口未装配) return {S::未装配, {}};
                if (retention.状态 != 特征概念规则状态::已完成 || !retention.材料)
                    词典接收故障(L"接收集合保持材料不合法");
                std::optional<新状态信息> before;
                for (const auto id : currentScene->状态节点组) {
                    const auto state = 服务.状态服务.获取状态(id);
                    if (!state) { refresh = true; break; }
                    if (state->被描述存在节点 != C || state->来源实例特征节点 != *F
                        || state->固定准确值 != feature->当前值) continue;
                    if (state->强时间.语义 != 新状态时间语义::实例绝对UTC)
                        词典接收故障(L"真实词典场景含非绝对时间接收状态");
                    if (!before || state->强时间.纳秒 > before->强时间.纳秒) before = state;
                    else if (state->强时间.纳秒 == before->强时间.纳秒 && state->节点 != before->节点)
                        词典接收故障(L"接收F最新前态时间无法唯一确定");
                }
                if (refresh) continue;
                新词典接收准备投影 projection{*F, V, before ? std::optional<稳定编码>{before->节点} : std::nullopt, 当前强时间()};
                const auto stateBinding = 服务.状态服务.取得特征状态构造绑定();
                return {S::已完成, std::make_unique<真实词典接收准备>(服务, 场景, C, projection,
                    feature->当前值, stateBinding, sceneBinding, std::move(*retention.材料))};
            }
        } catch (词典提交状态 s) {
            if (s == 词典提交状态::节点不存在) return {S::未找到, {}};
            if (s == 词典提交状态::资源失败) return {S::资源失败, {}};
            词典接收故障(L"接收场景准备技术结局无法解释");
        } catch (const std::bad_alloc&) { return {S::资源失败, {}}; }
        catch (const std::length_error&) { return {S::资源失败, {}}; }
        catch (...) { 词典接收故障(L"准备词典场景接收发生工程异常"); }
    }

    新词典接收提交结果 续接变化(稳定编码 C, 新词典处理阶段参与接口& participant) noexcept override {
        try {
            const auto info = participant.读取本轮处理材料();
            if (info.阶段 == 新词典接收处理阶段::接收已发表) {
                if (!info.后状态) 词典接收故障(L"已接收B没有后状态");
                const auto after = 服务.状态服务.获取状态(*info.后状态);
                if (!after || after->被描述存在节点 != C) 词典接收故障(L"B保持的后态缺失或主体不符");
                if (!info.前状态) return 收束(participant, info, 新词典接收处理阶段::无需动态, 新词条动态记录原因::无前态);
                const auto before = 服务.状态服务.获取状态(*info.前状态);
                if (!before || before->被描述存在节点 != C || before->来源实例特征节点 != after->来源实例特征节点)
                    词典接收故障(L"B保持的前后状态不属于同一接收F");
                if (before->固定准确值 == after->固定准确值)
                    return 收束(participant, info, 新词典接收处理阶段::无需动态, 新词条动态记录原因::无值变化);
                if (before->强时间.语义 != 新状态时间语义::实例绝对UTC || after->强时间.语义 != 新状态时间语义::实例绝对UTC)
                    词典接收故障(L"真实接收两态时间语义不符");
                if (after->强时间.纳秒 <= before->强时间.纳秒)
                    return 收束(participant, info, 新词典接收处理阶段::无需动态, 新词条动态记录原因::时间不递增);
                const std::vector<稳定编码> states{*info.前状态, *info.后状态};
                std::optional<稳定编码> D;
                for (const auto id : 服务.动态服务.按状态查询动态(*info.后状态)) {
                    const auto dynamic = 服务.动态服务.获取动态(id);
                    if (!dynamic) return {词典提交状态::预期不符, {}, {}};
                    if (dynamic->种类 != 新动态种类::原子 || dynamic->主题存在节点 != C || dynamic->状态节点组 != states) continue;
                    if (D) 词典接收故障(L"同一词典接收状态对存在多个原子动态");
                    D = id;
                }
                if (!D) {
                    const auto made = 服务.动态服务.建立原子动态(C, *info.前状态, *info.后状态);
                    if (made.状态 == 新动态操作状态::资源失败) return {词典提交状态::资源失败, {}, {}};
                    if (made.状态 != 新动态操作状态::已完成 || !made.动态节点)
                        词典接收故障(L"词典接收两态动态建立失败，保留B");
                    D = made.动态节点;
                }
                auto count = 服务.动态服务.准备引用计数参与({{*D, 1}});
                if (count.状态 == 新动态操作状态::动态不存在) return {词典提交状态::预期不符, {}, {}};
                if (count.状态 == 新动态操作状态::资源失败) return {词典提交状态::资源失败, {}, {}};
                if (count.状态 != 新动态操作状态::已完成 || !count.准备) 词典接收故障(L"B动态保持准备失败");
                const auto dPart = 服务.动态服务.封存计数参与(*count.准备);
                const auto bPart = participant.构造动态已建立(*D);
                auto done = 全局基础数据集.原子追加封存片段组({dPart, bPart});
                if (done.状态 != 词典提交状态::已完成) {
                    服务.动态服务.取消计数参与(*count.准备);
                    return {done.状态, {}, {}};
                }
                if (done.片段回执组.size() != 2) 词典接收故障(L"动态阶段缺共同回执");
                participant.登记共同发表回执(done.片段回执组[1]);
                服务.动态服务.登记计数发表(*count.准备, done.片段回执组[0]);
                服务.动态服务.完成计数参与(*count.准备);
                return {词典提交状态::已完成, std::move(done.片段回执组), {}};
            }
            if (info.阶段 != 新词典接收处理阶段::动态已建立 || !info.动态)
                词典接收故障(L"词典变化续接收到非在途阶段");
            // 已挂场景的Δ保留，零发表再次到来只复用同Δ，不回滚或另造。
            const auto attached = 场景.添加动态(C, *info.动态);
            if (attached == 新场景操作状态::资源失败) return {词典提交状态::资源失败, {}, {}};
            if (attached != 新场景操作状态::已完成 && attached != 新场景操作状态::无变化)
                词典接收故障(L"接收动态场景挂接失败，保留B及动态");
            return 收束(participant, info, 新词典接收处理阶段::场景已挂接, {});
        } catch (词典提交状态 s) { return {s, {}, {}}; }
        catch (词典记录状态 s) {
            if (s == 词典记录状态::资源失败) return {词典提交状态::资源失败, {}, {}};
            词典接收故障(L"接收阶段参与者返回非法技术状态");
        } catch (const std::bad_alloc&) { return {词典提交状态::资源失败, {}, {}}; }
        catch (const std::length_error&) { return {词典提交状态::资源失败, {}, {}}; }
        catch (...) { 词典接收故障(L"词典变化续接工程异常，保留已发表事实"); }
    }
private:
    新词典接收提交结果 收束(新词典处理阶段参与接口& participant,
        const 新词典接收处理信息& info, 新词典接收处理阶段 stage,
        std::optional<新词条动态记录原因> reason) {
        std::optional<新动态计数准备> dynamicCount;
        if (info.动态) {
            auto count = 服务.动态服务.准备引用计数参与({{*info.动态, -1}});
            if (count.状态 == 新动态操作状态::资源失败) return {词典提交状态::资源失败, {}, {}};
            if (count.状态 != 新动态操作状态::已完成 || !count.准备) 词典接收故障(L"收束动态保持准备失败");
            dynamicCount = std::move(count.准备);
        }
        std::vector<新引用计数变化> states;
        if (info.前状态) states.push_back({*info.前状态, -1});
        if (info.后状态) states.push_back({*info.后状态, -1});
        std::optional<新状态计数准备> stateCount;
        if (!states.empty()) {
            auto count = 服务.状态服务.准备引用计数参与(states);
            if (count.状态 == 新状态操作状态::资源失败) return {词典提交状态::资源失败, {}, {}};
            if (count.状态 != 新状态操作状态::已完成 || !count.准备) 词典接收故障(L"收束状态保持准备失败");
            stateCount = std::move(count.准备);
        }
        std::vector<基础封存追加片段> parts;
        if (dynamicCount) parts.push_back(服务.动态服务.封存计数参与(*dynamicCount));
        if (stateCount) parts.push_back(服务.状态服务.封存计数参与(*stateCount));
        parts.push_back(participant.构造处理收束(stage, reason));
        auto done = 全局基础数据集.原子追加封存片段组(parts);
        if (done.状态 != 词典提交状态::已完成) {
            if (stateCount) 服务.状态服务.取消计数参与(*stateCount);
            if (dynamicCount) 服务.动态服务.取消计数参与(*dynamicCount);
            return {done.状态, {}, {}};
        }
        if (done.片段回执组.size() != parts.size()) 词典接收故障(L"收束缺共同回执");
        participant.登记共同发表回执(done.片段回执组.back());
        std::size_t i = 0;
        if (dynamicCount) 服务.动态服务.登记计数发表(*dynamicCount, done.片段回执组[i++]);
        if (stateCount) 服务.状态服务.登记计数发表(*stateCount, done.片段回执组[i]);
        if (stateCount) 服务.状态服务.完成计数参与(*stateCount);
        if (dynamicCount) 服务.动态服务.完成计数参与(*dynamicCount);
        return {词典提交状态::已完成, std::move(done.片段回执组), {}};
    }
    新世界事实服务引用 服务;
    新_场景类& 场景;
    稳定编码 接收FT;
};
} // namespace

std::shared_ptr<新词典接收场景提供接口> 新_外设世界树类::创建词典接收场景提供者() {
    const auto type = 获取接收消息特征类型();
    if (!type) throw std::logic_error("dictionary receiver requires initialized peripheral world");
    return std::make_shared<真实词典接收提供者>(服务_, 场景_, *type);
}
} // namespace 海中鱼巣
