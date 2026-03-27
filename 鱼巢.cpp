#include "framework.h"

#include <atomic>
#include <chrono>
#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <vector>
#include <winsvc.h>

import 基础数据类型模块;
import 日志模块;
import 后台前端通道模块;
import 场景模块;
import 数据仓库模块;
import 主信息定义模块;
import 任务控制总控模块_v0;
import 自我线程模块;
import 自治宿主模块;
import 相机观测运行时模块;
import 世界树环境模块;
import 需求环境模块;
import 语素环境模块;

namespace {
constexpr wchar_t 默认服务名[] = L"鱼巢";
constexpr DWORD 服务等待提示毫秒 = 3000;
constexpr auto 停机宽限时长 = std::chrono::seconds(8);

struct 结构_启动参数 {
    enum class 枚举_运行模式 {
        控制台,
        作为服务,
        安装服务,
        卸载服务,
        任务控制总控回归测试,
        帮助
    };

    枚举_运行模式 模式 = 枚举_运行模式::控制台;
    结构_自治宿主配置 宿主配置{};
    结构_相机观测运行时配置 相机配置{};
    std::chrono::seconds 心跳周期{ 5 };
    std::optional<std::chrono::seconds> 最大运行时长;
    std::wstring 服务名 = 默认服务名;
    bool 启用相机观测 = false;
};

struct 结构_前端人类输入 {
    std::uint64_t 序号 = 0;
    时间戳 提交时间 = 0;
    I64 来源码 = 1001;
    std::string 文本;
    std::shared_ptr<std::string> 文本对象;
};

struct 结构_前端命令共享状态 {
    mutable std::mutex 互斥;
    std::deque<结构_前端人类输入> 待处理输入;
    std::shared_ptr<std::string> 最近输入对象;
    std::uint64_t 下一个输入序号 = 1;
    std::uint64_t 最近输入序号 = 0;
    std::string 最近输入文本;
    std::string 最近命令摘要;
    bool 请求后台停止 = false;

    static std::string 私有_裁剪摘要(std::string 文本, std::size_t 最大长度 = 120) {
        if (文本.size() <= 最大长度) return 文本;
        if (最大长度 < 3) return 文本.substr(0, 最大长度);
        return 文本.substr(0, 最大长度 - 3) + "...";
    }

    std::uint64_t 提交人类输入(const std::string& 文本) {
        std::lock_guard<std::mutex> 锁(互斥);
        结构_前端人类输入 输入{};
        输入.序号 = 下一个输入序号++;
        输入.提交时间 = 结构体_时间戳::当前_微秒();
        输入.文本 = 文本;
        输入.文本对象 = std::make_shared<std::string>(文本);
        待处理输入.push_back(输入);
        最近输入序号 = 输入.序号;
        最近输入文本 = 私有_裁剪摘要(文本);
        最近输入对象 = 输入.文本对象;
        最近命令摘要 = "已接收前端输入 #" + std::to_string(输入.序号);
        return 输入.序号;
    }

    bool 取出待处理输入(结构_前端人类输入* 输出) {
        if (!输出) return false;
        std::lock_guard<std::mutex> 锁(互斥);
        if (待处理输入.empty()) return false;
        *输出 = std::move(待处理输入.front());
        待处理输入.pop_front();
        return true;
    }

    void 请求停机() {
        std::lock_guard<std::mutex> 锁(互斥);
        请求后台停止 = true;
        最近命令摘要 = "已收到前端安全停机请求";
    }

    bool 领取停机请求() {
        std::lock_guard<std::mutex> 锁(互斥);
        if (!请求后台停止) return false;
        请求后台停止 = false;
        return true;
    }

    void 填充快照(结构_后台前端快照* 快照) const {
        if (!快照) return;
        std::lock_guard<std::mutex> 锁(互斥);
        快照->最近输入序号 = 最近输入序号;
        快照->最近输入文本 = 最近输入文本;
        快照->最近命令摘要 = 最近命令摘要;
    }
};

std::atomic_bool 控制台请求退出{ false };
HANDLE 服务停止事件 = nullptr;
SERVICE_STATUS_HANDLE 服务状态句柄 = nullptr;
SERVICE_STATUS 服务状态{};
结构_启动参数 全局启动参数{};

void 初始化日志_()
{
    日志参数 参数{};
	参数.文件前缀 = "鱼巢";
    参数.每条刷新 = true;
    日志::初始化(参数);
}

const char* 任务控制功能域文本_(枚举_任务控制功能域 功能域)
{
    switch (功能域) {
    case 枚举_任务控制功能域::筹办: return "筹办";
    case 枚举_任务控制功能域::维护: return "维护";
    case 枚举_任务控制功能域::纠偏: return "纠偏";
    case 枚举_任务控制功能域::收束: return "收束";
    default: return "未定义";
    }
}

const char* 总控结果文本_(枚举_总控结果 结果)
{
    switch (结果) {
    case 枚举_总控结果::已可执行: return "已可执行";
    case 枚举_总控结果::已等待学习: return "已等待学习";
    case 枚举_总控结果::需进入收束: return "需进入收束";
    case 枚举_总控结果::控制失败: return "控制失败";
    case 枚举_总控结果::需进入纠偏: return "需进入纠偏";
    case 枚举_总控结果::需回到筹办: return "需回到筹办";
    case 枚举_总控结果::已完成收束: return "已完成收束";
    case 枚举_总控结果::已失败收束: return "已失败收束";
    case 枚举_总控结果::已取消收束: return "已取消收束";
    default: return "未定义";
    }
}

const char* 方法沉淀来源文本_(枚举_方法沉淀来源 来源)
{
    switch (来源) {
    case 枚举_方法沉淀来源::本能: return "本能";
    case 枚举_方法沉淀来源::反推: return "反推";
    case 枚举_方法沉淀来源::组合: return "组合";
    case 枚举_方法沉淀来源::外部导入: return "外部导入";
    case 枚举_方法沉淀来源::运行期临时: return "运行期临时";
    default: return "未定义";
    }
}

DWORD 运行任务控制总控回归测试套件_()
{
    初始化日志_();
    日志::运行("[HY-Ego Headless] 开始运行任务控制总控回归测试套件");

    const auto 全链测试结果 = 任务控制总控回归测试_v0::测试_筹办维护纠偏收束全链();

    std::cout << "[HY-Ego Headless] 任务控制总控全链回归测试: "
        << (全链测试结果.成功 ? "成功" : "失败") << "\n";
    for (const auto& 项 : 全链测试结果.轨迹) {
        std::cout
            << "  [轮次 " << 项.轮次 << "] 当前域=" << 任务控制功能域文本_(项.当前功能域)
            << ", 下一域=" << 任务控制功能域文本_(项.下一功能域)
            << ", 结果=" << 总控结果文本_(项.结果)
            << ", 原因=" << 项.原因说明
            << "\n";
        日志::运行f(
            "[HY-Ego Headless] 回归轨迹: round={}, current={}, next={}, result={}, reason={}",
            项.轮次,
            任务控制功能域文本_(项.当前功能域),
            任务控制功能域文本_(项.下一功能域),
            总控结果文本_(项.结果),
            项.原因说明);
    }

    if (!全链测试结果.错误摘要.empty()) {
        std::cout << "  错误摘要: " << 全链测试结果.错误摘要 << "\n";
        日志::运行_错误("[HY-Ego Headless] 任务控制总控全链回归失败: " + 全链测试结果.错误摘要);
    }
    else {
        日志::运行("[HY-Ego Headless] 任务控制总控全链回归测试通过");
    }

    const auto 生长测试结果 = 任务控制总控回归测试_v0::测试_方法树自动生长升级链();
    std::cout << "[HY-Ego Headless] 方法树自动生长升级链回归测试: "
        << (生长测试结果.成功 ? "成功" : "失败") << "\n"
        << "  写回=" << (生长测试结果.已完成写回 ? "是" : "否")
        << ", 命中新枝=" << (生长测试结果.已命中写回枝条 ? "是" : "否")
        << ", 三次前未升级=" << (生长测试结果.已验证三次前不升级 ? "是" : "否")
        << ", 三次升候选=" << (生长测试结果.已升格候选枝 ? "是" : "否")
        << ", 五次升组合=" << (生长测试结果.已升格正式组合枝 ? "是" : "否")
        << ", 最终来源=" << 方法沉淀来源文本_(生长测试结果.最终来源)
        << "\n";
    日志::运行f(
        "[HY-Ego Headless] 方法树自动生长升级链: success={}, writeback={}, hit={}, stay_temp={}, promoted_candidate={}, promoted_combo={}, final_source={}",
        生长测试结果.成功,
        生长测试结果.已完成写回,
        生长测试结果.已命中写回枝条,
        生长测试结果.已验证三次前不升级,
        生长测试结果.已升格候选枝,
        生长测试结果.已升格正式组合枝,
        方法沉淀来源文本_(生长测试结果.最终来源));

    if (!生长测试结果.错误摘要.empty()) {
        std::cout << "  错误摘要: " << 生长测试结果.错误摘要 << "\n";
        日志::运行_错误("[HY-Ego Headless] 方法树自动生长升级链回归失败: " + 生长测试结果.错误摘要);
    }
    else {
        日志::运行("[HY-Ego Headless] 方法树自动生长升级链回归测试通过");
    }

    const auto 等待学习测试结果 = 自我线程类::测试_等待学习任务触发基础尝试学习();
    std::cout << "[HY-Ego Headless] 等待学习兜底回归测试: "
        << (等待学习测试结果.成功 ? "成功" : "失败") << "\n"
        << "  识别等待学习=" << (等待学习测试结果.已识别等待学习请求 ? "是" : "否")
        << ", 选中等待任务=" << (等待学习测试结果.已选中等待学习任务 ? "是" : "否")
        << ", 降级到基础尝试学习=" << (等待学习测试结果.已降级到基础尝试学习 ? "是" : "否")
        << ", 唤醒键一致=" << (等待学习测试结果.唤醒键一致 ? "是" : "否")
        << ", 优先级继承=" << (等待学习测试结果.学习任务优先级已继承 ? "是" : "否")
        << ", 已唤醒等待任务=" << (等待学习测试结果.已唤醒等待任务 ? "是" : "否")
        << "\n";
    日志::运行f(
        "[HY-Ego Headless] 等待学习兜底回归: success={}, pending={}, selected={}, fallback={}, wake_key={}, priority={}, resumed={}",
        等待学习测试结果.成功,
        等待学习测试结果.已识别等待学习请求,
        等待学习测试结果.已选中等待学习任务,
        等待学习测试结果.已降级到基础尝试学习,
        等待学习测试结果.唤醒键一致,
        等待学习测试结果.学习任务优先级已继承,
        等待学习测试结果.已唤醒等待任务);
    if (!等待学习测试结果.错误摘要.empty()) {
        std::cout << "  错误摘要: " << 等待学习测试结果.错误摘要 << "\n";
        日志::运行_错误("[HY-Ego Headless] 等待学习兜底回归失败: " + 等待学习测试结果.错误摘要);
    }
    else {
        日志::运行("[HY-Ego Headless] 等待学习兜底回归测试通过");
    }

    const auto 执行转学习测试结果 = 自我线程类::测试_任务执行器转入学习并唤醒等待任务();
    std::cout << "[HY-Ego Headless] 执行转学习回归测试: "
        << (执行转学习测试结果.成功 ? "成功" : "失败") << "\n"
        << "  执行器转入学习=" << (执行转学习测试结果.已通过任务执行器转入学习 ? "是" : "否")
        << ", 写等待学习标记=" << (执行转学习测试结果.已写等待学习标记 ? "是" : "否")
        << ", 写任务执行流水账=" << (执行转学习测试结果.已写任务执行流水账 ? "是" : "否")
        << ", 写任务失败账=" << (执行转学习测试结果.已写任务失败账 ? "是" : "否")
        << ", 写任务方法结果账=" << (执行转学习测试结果.已写任务方法结果账 ? "是" : "否")
        << ", 写任务转学习账=" << (执行转学习测试结果.已写任务转学习账 ? "是" : "否")
        << ", 写显式学习请求=" << (执行转学习测试结果.已写显式学习请求 ? "是" : "否")
        << ", 优先级继承=" << (执行转学习测试结果.学习任务优先级已继承 ? "是" : "否")
        << ", 切到基础尝试学习=" << (执行转学习测试结果.已切到等待任务基础尝试学习 ? "是" : "否")
        << ", 唤醒键保持来源方法=" << (执行转学习测试结果.唤醒键保持来源方法 ? "是" : "否")
        << ", 已唤醒原任务=" << (执行转学习测试结果.已唤醒原任务 ? "是" : "否")
        << "\n";
    日志::运行f(
        "[HY-Ego Headless] 执行转学习回归: success={}, transfer={}, waiting={}, task_ledger={}, task_fail={}, task_method_result={}, task_transfer_count={}, explicit={}, priority={}, fallback={}, wake_key={}, resumed={}",
        执行转学习测试结果.成功,
        执行转学习测试结果.已通过任务执行器转入学习,
        执行转学习测试结果.已写等待学习标记,
        执行转学习测试结果.已写任务执行流水账,
        执行转学习测试结果.已写任务失败账,
        执行转学习测试结果.已写任务方法结果账,
        执行转学习测试结果.已写任务转学习账,
        执行转学习测试结果.已写显式学习请求,
        执行转学习测试结果.学习任务优先级已继承,
        执行转学习测试结果.已切到等待任务基础尝试学习,
        执行转学习测试结果.唤醒键保持来源方法,
        执行转学习测试结果.已唤醒原任务);
    if (!执行转学习测试结果.错误摘要.empty()) {
        std::cout << "  错误摘要: " << 执行转学习测试结果.错误摘要 << "\n";
        日志::运行_错误("[HY-Ego Headless] 执行转学习回归失败: " + 执行转学习测试结果.错误摘要);
    }
    else {
        日志::运行("[HY-Ego Headless] 执行转学习回归测试通过");
    }

    const auto 学习效果测试结果 = 自我线程类::测试_尝试学习能够沉淀方法效果();
    std::cout << "[HY-Ego Headless] 学习效果回归测试: "
        << (学习效果测试结果.成功 ? "成功" : "失败") << "\n"
        << "  首次有进展=" << (学习效果测试结果.首次学习有进展 ? "是" : "否")
        << ", 首次补齐条件结果=" << (学习效果测试结果.首次已补齐条件和结果 ? "是" : "否")
        << ", 首次进入尝试中=" << (学习效果测试结果.首次已进入尝试中 ? "是" : "否")
        << ", 三次观察成功=" << (学习效果测试结果.三次观察均成功 ? "是" : "否")
        << ", 稳定次数达标=" << (学习效果测试结果.连续稳定次数达标 ? "是" : "否")
        << ", 已标记尝试完成=" << (学习效果测试结果.已标记尝试完成 ? "是" : "否")
        << "\n";
    日志::运行f(
        "[HY-Ego Headless] 学习效果回归: success={}, first_progress={}, first_fill={}, first_trying={}, observed_all={}, stable_ok={}, completed={}",
        学习效果测试结果.成功,
        学习效果测试结果.首次学习有进展,
        学习效果测试结果.首次已补齐条件和结果,
        学习效果测试结果.首次已进入尝试中,
        学习效果测试结果.三次观察均成功,
        学习效果测试结果.连续稳定次数达标,
        学习效果测试结果.已标记尝试完成);
    if (!学习效果测试结果.错误摘要.empty()) {
        std::cout << "  错误摘要: " << 学习效果测试结果.错误摘要 << "\n";
        日志::运行_错误("[HY-Ego Headless] 学习效果回归失败: " + 学习效果测试结果.错误摘要);
    }
    else {
        日志::运行("[HY-Ego Headless] 学习效果回归测试通过");
    }

    const auto 叶子修复测试结果 = 自我线程类::测试_叶子执行遇到无效方法时可通过学习兜底修复();
    std::cout << "[HY-Ego Headless] 叶子学习修复回归测试: "
        << (叶子修复测试结果.成功 ? "成功" : "失败") << "\n"
        << "  识别原方法不可执行=" << (叶子修复测试结果.已识别原方法不可执行 ? "是" : "否")
        << ", 已补出可执行方法=" << (叶子修复测试结果.已通过学习兜底补出可执行方法 ? "是" : "否")
        << ", 已切换到新方法=" << (叶子修复测试结果.已切换到新方法 ? "是" : "否")
        << ", 叶子执行成功=" << (叶子修复测试结果.叶子执行成功 ? "是" : "否")
        << "\n";
    日志::运行f(
        "[HY-Ego Headless] 叶子学习修复回归: success={}, invalid={}, repaired={}, switched={}, executed={}",
        叶子修复测试结果.成功,
        叶子修复测试结果.已识别原方法不可执行,
        叶子修复测试结果.已通过学习兜底补出可执行方法,
        叶子修复测试结果.已切换到新方法,
        叶子修复测试结果.叶子执行成功);
    if (!叶子修复测试结果.错误摘要.empty()) {
        std::cout << "  错误摘要: " << 叶子修复测试结果.错误摘要 << "\n";
        日志::运行_错误("[HY-Ego Headless] 叶子学习修复回归失败: " + 叶子修复测试结果.错误摘要);
    }
    else {
        日志::运行("[HY-Ego Headless] 叶子学习修复回归测试通过");
    }

    日志::关闭();
    return (全链测试结果.成功
        && 生长测试结果.成功
        && 等待学习测试结果.成功
        && 执行转学习测试结果.成功
        && 学习效果测试结果.成功
        && 叶子修复测试结果.成功) ? 0 : 1;
}

template<class TNode, class Fn>
void 枚举根子节点_(TNode* 根, Fn&& fn) {
    if (!根) return;

    if (根->子) {
        auto* first = static_cast<TNode*>(根->子);
        auto* it = first;
        do {
            fn(it);
            it = static_cast<TNode*>(it->下);
        } while (it && it != first);
        return;
    }

    // 顶层需求链/任务链的根节点使用 根->下 维护同层环，而不是挂在 根->子 上。
    auto* it = static_cast<TNode*>(根->下);
    while (it && it != 根) {
        fn(it);
        it = static_cast<TNode*>(it->下);
    }
}

std::string 词键_(const 词性节点类* 词) {
    return 词 ? 词->获取主键() : std::string{};
}

std::string 取词文本_(const 词性节点类* 词) {
    if (!词 || !词->父 || !词->父->主信息) return {};
    auto* 词信息 = dynamic_cast<词主信息类*>(词->父->主信息);
    return 词信息 ? 词信息->词 : std::string{};
}

std::string 需求类型显示文本_(const 词性节点类* 类型) {
    if (!类型) return {};
    auto 文本 = 取词文本_(类型);
    if (文本.empty()) {
        文本 = 类型->获取主键();
    }

    const auto 服务 = std::string("服务");
    const auto 安全 = std::string("安全");
    const auto 学习 = std::string("学习");
    if (文本.find(服务) != std::string::npos) return 服务;
    if (文本.find(安全) != std::string::npos) return 安全;
    if (文本.find(学习) != std::string::npos) return 学习;
    return 文本;
}

std::size_t 计数同层节点_(const 基础信息节点类* 头节点) {
    if (!头节点) return 0;
    std::size_t 数量 = 0;
    auto* 当前 = 头节点;
    do {
        ++数量;
        当前 = 当前->下;
    } while (当前 && 当前 != 头节点);
    return 数量;
}

const 词性节点类* 取状态特征类型_(const 状态节点类* 状态) {
    auto* 状态信息 = (状态 && 状态->主信息) ? dynamic_cast<状态节点主信息类*>(状态->主信息) : nullptr;
    auto* 特征信息 = (状态信息 && 状态信息->状态特征 && 状态信息->状态特征->主信息)
        ? dynamic_cast<特征主信息类*>(状态信息->状态特征->主信息)
        : nullptr;
    return 特征信息 ? 特征信息->特征类型 : nullptr;
}

I64 当前UTC毫秒_() {
    return static_cast<I64>(std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count());
}

bool 尝试取当前自我特征I64_(
    const 词性节点类* 特征类型,
    I64* 输出值) {
    if (!输出值 || !特征类型) return false;
    auto* 自我存在 = 世界树.自我指针;
    if (!自我存在) return false;

    const auto 快照 = 世界树.读取特征快照(
        自我存在,
        特征类型,
        "鱼巢::尝试取当前自我特征I64");
    if (!快照.has_value()) return false;

    if (const auto* 值 = std::get_if<I64>(&*快照)) {
        *输出值 = *值;
        return true;
    }
    return false;
}

任务节点类* 取主任务头节点_(任务节点类* 任务头节点) {
    auto* 当前 = 任务头节点;
    while (当前 && 当前->主信息) {
        auto* 头信息 = dynamic_cast<任务头结点信息*>(当前->主信息);
        if (!头信息 || !头信息->父任务头结点 || 头信息->父任务头结点 == 当前) break;
        当前 = 头信息->父任务头结点;
    }
    return 当前;
}

bool 主任务属于安全任务树_(任务节点类* 任务头节点) {
    auto* 主任务头 = 取主任务头节点_(任务头节点);
    auto* 头信息 = (主任务头 && 主任务头->主信息) ? dynamic_cast<任务头结点信息*>(主任务头->主信息) : nullptr;
    if (!头信息) return false;
    return 词键_(头信息->名称).find("安全任务") != std::string::npos;
}

bool 主任务属于服务任务树_(任务节点类* 任务头节点) {
    auto* 主任务头 = 取主任务头节点_(任务头节点);
    auto* 头信息 = (主任务头 && 主任务头->主信息) ? dynamic_cast<任务头结点信息*>(主任务头->主信息) : nullptr;
    if (!头信息) return false;
    return 词键_(头信息->名称).find("服务任务") != std::string::npos;
}

template<class Fn>
void 枚举任务头节点_(Fn&& fn) {
    auto 递归枚举子树 = [&](auto&& self, 任务节点类* 父节点) -> void {
        auto* 首子节点 = 父节点 ? static_cast<任务节点类*>(父节点->子) : nullptr;
        if (!首子节点) return;
        auto* 当前 = 首子节点;
        do {
            if (auto* 头信息 = (当前 && 当前->主信息) ? dynamic_cast<任务头结点信息*>(当前->主信息) : nullptr) {
                fn(当前, 头信息);
            }
            self(self, 当前);
            当前 = static_cast<任务节点类*>(当前->下);
        } while (当前 && 当前 != 首子节点);
    };

    枚举根子节点_(static_cast<任务节点类*>(数据仓库模块::任务链.根指针), [&](任务节点类* 顶层节点) {
        if (auto* 头信息 = (顶层节点 && 顶层节点->主信息) ? dynamic_cast<任务头结点信息*>(顶层节点->主信息) : nullptr) {
            fn(顶层节点, 头信息);
        }
        递归枚举子树(递归枚举子树, 顶层节点);
    });
}

结构_后台前端快照 生成后台快照(
    const 结构_自治宿主快照& 宿主快照,
    const 结构_相机观测运行时快照* 相机快照,
    bool 相机已启用,
    const 结构_前端命令共享状态* 前端命令状态 = nullptr) {
    结构_后台前端快照 结果{};
    结果.快照生成UTC毫秒 = 当前UTC毫秒_();
    结果.后台在线 = true;
    结果.宿主运行中 = 宿主快照.宿主运行中;
    结果.请求停止 = 宿主快照.请求停止;
    结果.自我线程运行中 = 宿主快照.自我线程运行中;
    结果.初始化完成 = 宿主快照.初始化完成;
    结果.健康运行 = 宿主快照.健康运行;
    结果.请求安全重启 = 宿主快照.请求安全重启;
    结果.致命错误计数 = 宿主快照.致命错误计数;
    结果.已执行重启次数 = 宿主快照.已执行重启次数;
    结果.最近故障摘要 = 宿主快照.最近故障摘要;
    结果.宿主故障摘要 = 宿主快照.宿主故障摘要;
    结果.相机已启用 = 相机已启用;
    if (相机快照) {
        结果.相机运行中 = 相机快照->运行中;
        结果.相机使用虚拟回退 = 相机快照->使用虚拟相机;
        结果.相机已处理帧数 = 相机快照->已处理帧数;
        结果.最近桥接候选数 = 相机快照->最近桥接候选数;
        结果.相机状态摘要 = 相机快照->最近状态摘要;
    }
    if (前端命令状态) {
        前端命令状态->填充快照(&结果);
    }

    枚举根子节点_(static_cast<基础信息节点类*>(数据仓库模块::世界链.根指针), [&](基础信息节点类*) {
        ++结果.世界根子节点数;
    });
    枚举根子节点_(static_cast<需求节点类*>(数据仓库模块::需求链.根指针), [&](需求节点类* n) {
        ++结果.需求数;
        auto* mi = n ? n->主信息 : nullptr;
        if (!mi) return;
        if (结果.需求数 == 1 || static_cast<std::int64_t>(mi->权重) > 结果.主要需求权重) {
            结果.主要需求权重 = static_cast<std::int64_t>(mi->权重);
            结果.主要需求类型 = 需求类型显示文本_(mi->类型);
            枚举_需求运行状态 缓存状态 = 枚举_需求运行状态::未知;
            if (需求状态管理器.尝试查询缓存需求状态(n, &缓存状态)) {
                结果.主要需求已满足 = (缓存状态 == 枚举_需求运行状态::已满足);
            }
        }
    });
    枚举任务头节点_([&](任务节点类* 任务头节点, 任务头结点信息* 头信息) {
        ++结果.任务数;
        if (头信息 && 头信息->任务树类型 == 枚举_任务树类型::叶子任务) {
            ++结果.叶子任务数;
        }
        if (主任务属于安全任务树_(任务头节点)) {
            ++结果.安全任务数;
        }
        else if (主任务属于服务任务树_(任务头节点)) {
            ++结果.服务任务数;
        }
    });
    枚举根子节点_(static_cast<方法节点类*>(数据仓库模块::方法链.根指针), [&](方法节点类*) {
        ++结果.方法数;
    });
    auto* 内部世界 = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
    auto* 内部场景信息 = (内部世界 && 内部世界->主信息) ? dynamic_cast<场景节点主信息类*>(内部世界->主信息) : nullptr;
    if (内部场景信息) {
        内部场景信息->清理空指针();
        结果.状态数 = static_cast<std::uint64_t>(内部场景信息->状态总数());
        结果.动态数 = static_cast<std::uint64_t>(内部场景信息->动态总数());
        结果.事件动态数 = static_cast<std::uint64_t>(内部场景信息->事件动态总数());
    }
    I64 当前值 = 0;
    if (尝试取当前自我特征I64_(特征类型定义类::类型_自我_安全值, &当前值)) {
        结果.有当前安全值 = true;
        结果.当前安全值 = 当前值;
    }
    if (尝试取当前自我特征I64_(特征类型定义类::类型_自我_服务值, &当前值)) {
        结果.有当前服务值 = true;
        结果.当前服务值 = 当前值;
    }
    结果.因果数 = static_cast<std::uint64_t>(
        计数同层节点_(因果集.获取因果根() ? 因果集.获取因果根()->子 : nullptr));

    return 结果;
}

场景节点类* 查找直系子场景_按名称(场景节点类* 父场景, const char* 名称) {
    if (!父场景 || !名称 || !*名称) return nullptr;
    const auto* 目标名称 = 语素集.添加词性词(名称, "名词");
    for (auto* 场景 : 场景类::获取子场景(父场景)) {
        auto* 信息 = (场景 && 场景->主信息) ? dynamic_cast<场景节点主信息类*>(场景->主信息) : nullptr;
        if (信息 && 信息->名称 == 目标名称) return 场景;
    }
    return nullptr;
}

场景节点类* 确保人类输入桥场景(const char* 名称, const char* 类型, 时间戳 now) {
    auto* 父场景 = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
    if (!父场景) return nullptr;
    if (auto* 已有 = 查找直系子场景_按名称(父场景, 名称)) {
        if (auto* 信息 = 已有->主信息 ? dynamic_cast<场景节点主信息类*>(已有->主信息) : nullptr) {
            信息->最后观测时间 = now;
        }
        return 已有;
    }

    auto* mi = new 场景节点主信息类();
    mi->名称 = 语素集.添加词性词(名称, "名词");
    mi->类型 = 语素集.添加词性词(类型, "名词");
    mi->最后观测时间 = now;
	return 世界树.创建场景(父场景, mi, std::string("鱼巢::确保人类输入桥场景/") + 名称);
}

bool 提交前端人类输入到自我桥(const 结构_前端人类输入& 输入) {
    auto* 输入场景 = 确保人类输入桥场景("主窗口_人类输入桥输入", "主窗口人类输入桥场景", 输入.提交时间);
    if (!输入场景) {
        日志::运行_错误("[HY-Ego Headless] 人类输入桥创建失败");
        return false;
    }

    (void)世界树.写入特征_I64(
        输入场景,
        语素集.添加词性词("人类输入_存在", "名词"),
        1,
        {},
		"鱼巢::提交前端人类输入到自我桥/写存在");
    (void)世界树.写入特征_I64(
        输入场景,
        语素集.添加词性词("人类输入_序号", "名词"),
        static_cast<I64>(输入.序号),
        {},
		"鱼巢::提交前端人类输入到自我桥/写序号");
    (void)世界树.写入特征_I64(
        输入场景,
        语素集.添加词性词("人类输入_来源码", "名词"),
        输入.来源码,
        {},
		"鱼巢::提交前端人类输入到自我桥/写来源码");
    if (输入.文本对象) {
        (void)世界树.写入特征_指针(
            输入场景,
            语素集.添加词性词("人类输入_对象指针", "名词"),
            reinterpret_cast<std::uintptr_t>(输入.文本对象.get()),
            {},
		"鱼巢::提交前端人类输入到自我桥/写对象指针");
    }

    日志::运行f(
        "[HY-Ego Headless] 前端输入已桥接: 序号={}, 字节数={}",
        static_cast<unsigned long long>(输入.序号),
        输入.文本.size());
    return true;
}

std::string 转UTF8(const std::wstring& 文本) {
    if (文本.empty()) return {};
    const int 需要长度 = ::WideCharToMultiByte(CP_UTF8, 0, 文本.c_str(), static_cast<int>(文本.size()), nullptr, 0, nullptr, nullptr);
    if (需要长度 <= 0) return {};
    std::string 结果(static_cast<std::size_t>(需要长度), '\0');
    ::WideCharToMultiByte(CP_UTF8, 0, 文本.c_str(), static_cast<int>(文本.size()), 结果.data(), 需要长度, nullptr, nullptr);
    return 结果;
}

std::wstring 从UTF8(const std::string& 文本) {
    if (文本.empty()) return {};
    const int 需要长度 = ::MultiByteToWideChar(CP_UTF8, 0, 文本.c_str(), static_cast<int>(文本.size()), nullptr, 0);
    if (需要长度 <= 0) return {};
    std::wstring 结果(static_cast<std::size_t>(需要长度), L'\0');
    ::MultiByteToWideChar(CP_UTF8, 0, 文本.c_str(), static_cast<int>(文本.size()), 结果.data(), 需要长度);
    return 结果;
}

std::wstring 取Win32错误文本(DWORD 错误码) {
    LPWSTR 缓冲 = nullptr;
    const DWORD 长度 = ::FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        错误码,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPWSTR>(&缓冲),
        0,
        nullptr);
    std::wstring 文本 = (长度 > 0 && 缓冲) ? std::wstring(缓冲, 长度) : L"未知错误";
    if (缓冲) ::LocalFree(缓冲);
    while (!文本.empty() && (文本.back() == L'\r' || 文本.back() == L'\n' || 文本.back() == L' ')) {
        文本.pop_back();
    }
    return 文本;
}

void 打印帮助() {
    std::wcout
        << L"HY-Ego Headless\n"
        << L"  --help                    显示帮助\n"
        << L"  --run-task-control-regression  运行任务控制总控回归套件（全链+自动生长）后退出\n"
        << L"  --no-restart              禁用自治线程自动拉起\n"
        << L"  --monitor-ms=N            设置宿主监控周期（毫秒）\n"
        << L"  --restart-backoff-ms=N    设置拉起退避（毫秒）\n"
        << L"  --max-restarts=N          设置最大自动拉起次数，0 表示不限\n"
        << L"  --heartbeat-seconds=N     控制台生命体征输出周期（秒）\n"
        << L"  --duration-seconds=N      到时后自动退出\n"
        << L"  --camera                  同时启动相机观测运行时（D455 失败时回退虚拟相机）\n"
        << L"  --camera-required         强制启动真实相机，失败则整体退出\n"
        << L"  --service                 以 Windows Service 方式运行（供 SCM 调用）\n"
        << L"  --install-service         安装 Windows Service（默认自动启动）\n"
        << L"  --uninstall-service       卸载 Windows Service\n"
        << L"  --service-name=NAME       指定服务名，默认 鱼巢\n";
}

bool 解析无符号参数(const std::wstring& 当前, const wchar_t* 前缀, std::uint64_t* 输出) {
    const std::wstring_view 视图(当前);
    const std::wstring_view 目标前缀(前缀);
    if (!视图.starts_with(目标前缀)) return false;

    try {
        *输出 = std::stoull(std::wstring(视图.substr(目标前缀.size())));
        return true;
    }
    catch (...) {
        return false;
    }
}

bool 解析启动参数(int argc, wchar_t* argv[], 结构_启动参数* 输出, std::wstring* 错误) {
    if (!输出) return false;

    结构_启动参数 结果{};
    for (int i = 1; i < argc; ++i) {
        const std::wstring 当前 = argv[i] ? argv[i] : L"";
        std::uint64_t 数值 = 0;

        if (当前 == L"--help" || 当前 == L"-h" || 当前 == L"/?") {
            结果.模式 = 结构_启动参数::枚举_运行模式::帮助;
            continue;
        }
        if (当前 == L"--run-task-control-regression") {
            结果.模式 = 结构_启动参数::枚举_运行模式::任务控制总控回归测试;
            continue;
        }
        if (当前 == L"--no-restart") {
            结果.宿主配置.允许自动重启 = false;
            continue;
        }
        if (当前 == L"--camera") {
            结果.启用相机观测 = true;
            结果.相机配置.允许回退到虚拟相机 = true;
            continue;
        }
        if (当前 == L"--camera-required") {
            结果.启用相机观测 = true;
            结果.相机配置.允许回退到虚拟相机 = false;
            continue;
        }
        if (当前 == L"--service") {
            结果.模式 = 结构_启动参数::枚举_运行模式::作为服务;
            continue;
        }
        if (当前 == L"--install-service") {
            结果.模式 = 结构_启动参数::枚举_运行模式::安装服务;
            continue;
        }
        if (当前 == L"--uninstall-service") {
            结果.模式 = 结构_启动参数::枚举_运行模式::卸载服务;
            continue;
        }
        if (const std::wstring_view 视图(当前); 视图.starts_with(L"--service-name=")) {
            结果.服务名 = std::wstring(视图.substr(std::wstring_view(L"--service-name=").size()));
            if (结果.服务名.empty()) {
                if (错误) *错误 = L"service-name 不能为空";
                return false;
            }
            continue;
        }
        if (解析无符号参数(当前, L"--monitor-ms=", &数值)) {
            结果.宿主配置.监控周期 = std::chrono::milliseconds(数值);
            continue;
        }
        if (解析无符号参数(当前, L"--restart-backoff-ms=", &数值)) {
            结果.宿主配置.重启退避 = std::chrono::milliseconds(数值);
            continue;
        }
        if (解析无符号参数(当前, L"--max-restarts=", &数值)) {
            结果.宿主配置.最大自动重启次数 = static_cast<std::uint32_t>(数值);
            continue;
        }
        if (解析无符号参数(当前, L"--heartbeat-seconds=", &数值)) {
            结果.心跳周期 = std::chrono::seconds(数值);
            continue;
        }
        if (解析无符号参数(当前, L"--duration-seconds=", &数值)) {
            结果.最大运行时长 = std::chrono::seconds(数值);
            continue;
        }

        if (错误) *错误 = L"未知参数: " + 当前;
        return false;
    }

    *输出 = 结果;
    return true;
}

bool 初始化Mfc运行时() {
    if (!::AfxWinInit(::GetModuleHandleW(nullptr), nullptr, ::GetCommandLineW(), 0)) {
        std::wcerr << L"[HY-Ego Headless] AfxWinInit 失败\n";
        return false;
    }
    if (!::AfxSocketInit()) {
        std::wcerr << L"[HY-Ego Headless] AfxSocketInit 失败\n";
        return false;
    }
    return true;
}

void 输出控制台生命体征(
    const 结构_自治宿主快照& 快照,
    const 结构_相机观测运行时快照* 相机快照 = nullptr) {
    std::wcout
        << L"[headless] 宿主=" << (快照.宿主运行中 ? L"运行" : L"停止")
        << L" 自我=" << (快照.自我线程运行中 ? L"运行" : L"停止")
        << L" 初始化=" << (快照.初始化完成 ? L"是" : L"否")
        << L" 健康=" << (快照.健康运行 ? L"是" : L"否")
        << L" 请求安全重启=" << (快照.请求安全重启 ? L"是" : L"否")
        << L" 致命错误计数=" << 快照.致命错误计数
        << L" 重启次数=" << 快照.已执行重启次数;
    std::wcout << L"\n";
    if (相机快照) {
        std::wcout
            << L"[headless] 相机=" << (相机快照->运行中 ? L"运行" : L"停止")
            << L" 虚拟回退=" << (相机快照->使用虚拟相机 ? L"是" : L"否")
            << L" 已处理帧数=" << 相机快照->已处理帧数
            << L" 最近桥接候选数=" << 相机快照->最近桥接候选数
            << L"\n";
        if (!相机快照->最近状态摘要.empty()) {
            std::wcout << L"[headless] 相机状态: " << 从UTF8(相机快照->最近状态摘要) << L"\n";
        }
    }
    if (!快照.最近故障摘要.empty()) {
        std::wcout << L"[headless] 最近故障摘要: " << 从UTF8(快照.最近故障摘要) << L"\n";
    }
    if (!快照.宿主故障摘要.empty()) {
        std::wcout << L"[headless] 宿主故障摘要: " << 从UTF8(快照.宿主故障摘要) << L"\n";
    }
}

BOOL WINAPI 控制台控制处理(DWORD 控制类型) {
    switch (控制类型) {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_SHUTDOWN_EVENT:
        控制台请求退出.store(true);
        日志::运行("[HY-Ego Headless] 收到控制台停止信号");
        return TRUE;
    default:
        return FALSE;
    }
}

void 更新服务状态(DWORD 当前状态, DWORD 退出码 = NO_ERROR, DWORD 等待提示 = 0) {
    if (!服务状态句柄) return;

    服务状态.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    服务状态.dwCurrentState = 当前状态;
    服务状态.dwWin32ExitCode = 退出码;
    服务状态.dwWaitHint = 等待提示;
    服务状态.dwControlsAccepted = (当前状态 == SERVICE_START_PENDING)
        ? 0
        : (SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN);

    ::SetServiceStatus(服务状态句柄, &服务状态);
}

DWORD 运行宿主循环(const 结构_启动参数& 参数, HANDLE 额外停止事件, bool 输出心跳) {
    auto 宿主 = std::make_unique<自治宿主类>(参数.宿主配置);
    宿主->启动();
    std::unique_ptr<相机观测运行时类> 相机运行时;
    结构_前端命令共享状态 前端命令状态{};
    后台前端服务类 前端通道([&宿主, &相机运行时, &参数, &前端命令状态](
        const std::unordered_map<std::string, std::string>& 请求,
        结构_后台前端快照* 输出快照,
        bool* 成功,
        std::string* 消息) {
        const auto 生成快照 = [&]() {
            const auto 宿主快照 = 宿主->快照();
            const auto 相机快照 = 相机运行时 ? std::make_optional(相机运行时->快照()) : std::nullopt;
            return 生成后台快照(
                宿主快照,
                相机快照 ? &*相机快照 : nullptr,
                参数.启用相机观测,
                &前端命令状态);
        };

        const auto 命令 = [&]() -> std::string {
            const auto it = 请求.find("command");
            return it == 请求.end() ? "snapshot" : it->second;
        }();

        if (命令 == "submit_input") {
            const auto it = 请求.find("text");
            if (it == 请求.end() || it->second.empty()) {
                if (成功) *成功 = false;
                if (消息) *消息 = "输入为空，未提交到后台";
            }
            else {
                const auto 序号 = 前端命令状态.提交人类输入(it->second);
                if (成功) *成功 = true;
                if (消息) *消息 = "已提交到后台，输入序号 #" + std::to_string(序号);
            }
        }
        else if (命令 == "request_stop") {
            前端命令状态.请求停机();
            if (成功) *成功 = true;
            if (消息) *消息 = "后台已收到安全停机请求";
        }
        else if (命令 == "snapshot") {
            if (成功) *成功 = true;
            if (消息) *消息 = "ok";
        }
        else {
            if (成功) *成功 = false;
            if (消息) *消息 = "未知前端命令: " + 命令;
        }

        if (输出快照) {
            *输出快照 = 生成快照();
        }
    });
    前端通道.启动();
    const auto 请求所有停机 = [&]() {
        if (相机运行时) {
            相机运行时->请求退出();
        }
        前端通道.请求退出();
        宿主->请求停止();
    };
    const auto 等待所有停机 = [&]() {
        if (相机运行时) {
            相机运行时->等待线程结束();
        }
        前端通道.等待线程结束();
        宿主->等待线程结束();
    };
    if (参数.启用相机观测) {
        日志::运行("[HY-Ego Headless] 启动相机观测运行时");
        相机运行时 = std::make_unique<相机观测运行时类>(参数.相机配置);
        if (!相机运行时->启动()) {
            日志::运行_错误("[HY-Ego Headless] 相机观测运行时启动失败");
            请求所有停机();
            等待所有停机();
            return ERROR_SERVICE_SPECIFIC_ERROR;
        }
    }

    const auto 开始时间 = std::chrono::steady_clock::now();
    auto 下次心跳时间 = 开始时间;

    while (true) {
        const auto 当前时间 = std::chrono::steady_clock::now();
        结构_前端人类输入 待处理输入{};
        while (前端命令状态.取出待处理输入(&待处理输入)) {
            (void)提交前端人类输入到自我桥(待处理输入);
        }
        if (前端命令状态.领取停机请求()) {
            日志::运行("[HY-Ego Headless] 收到前端安全停机请求，准备退出");
            break;
        }
        if (参数.最大运行时长.has_value() && 当前时间 - 开始时间 >= *参数.最大运行时长) {
            日志::运行("[HY-Ego Headless] 到达最大运行时长，准备退出");
            break;
        }
        if (控制台请求退出.load()) {
            break;
        }
        if (额外停止事件 && ::WaitForSingleObject(额外停止事件, 0) == WAIT_OBJECT_0) {
            break;
        }

        const auto 快照 = 宿主->快照();
        const auto 相机快照 = 相机运行时 ? std::make_optional(相机运行时->快照()) : std::nullopt;
        if (!快照.宿主故障摘要.empty() && !快照.宿主运行中) {
            if (输出心跳) {
                输出控制台生命体征(快照, 相机快照 ? &*相机快照 : nullptr);
            }
            请求所有停机();
            等待所有停机();
            return ERROR_SERVICE_SPECIFIC_ERROR;
        }

        if (参数.启用相机观测 && 相机运行时 && 相机快照 && !相机快照->运行中) {
            日志::运行_错误("[HY-Ego Headless] 相机运行时已退出，尝试重新启动");
            相机运行时->停止();
            if (!相机运行时->启动()) {
                日志::运行_错误("[HY-Ego Headless] 相机运行时重新启动失败");
                请求所有停机();
                等待所有停机();
                return ERROR_SERVICE_SPECIFIC_ERROR;
            }
        }

        if (输出心跳 && 当前时间 >= 下次心跳时间) {
            输出控制台生命体征(快照, 相机快照 ? &*相机快照 : nullptr);
            下次心跳时间 = 当前时间 + 参数.心跳周期;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    请求所有停机();
    const auto 停机截止时间 = std::chrono::steady_clock::now() + 停机宽限时长;
    while (std::chrono::steady_clock::now() < 停机截止时间) {
        const auto 快照 = 宿主->快照();
        if (!快照.宿主运行中) {
            等待所有停机();
            return 快照.宿主故障摘要.empty() ? NO_ERROR : ERROR_SERVICE_SPECIFIC_ERROR;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    日志::运行_错误("[HY-Ego Headless] 受控停机超时，准备直接结束进程");
    std::wcout << L"[headless] 受控停机超时，进程将直接退出\n";
    std::wcout.flush();
    std::wcerr.flush();
    (void)宿主.release();
    ::TerminateProcess(::GetCurrentProcess(), WAIT_TIMEOUT);
    return WAIT_TIMEOUT;
}

void WINAPI 服务控制处理器(DWORD 控制码) {
    switch (控制码) {
    case SERVICE_CONTROL_STOP:
    case SERVICE_CONTROL_SHUTDOWN:
        更新服务状态(SERVICE_STOP_PENDING, NO_ERROR, 服务等待提示毫秒);
        if (服务停止事件) {
            ::SetEvent(服务停止事件);
        }
        return;
    default:
        return;
    }
}

void WINAPI 服务主函数(DWORD, LPWSTR*) {
    服务状态句柄 = ::RegisterServiceCtrlHandlerW(全局启动参数.服务名.c_str(), 服务控制处理器);
    if (!服务状态句柄) {
        return;
    }

    更新服务状态(SERVICE_START_PENDING, NO_ERROR, 服务等待提示毫秒);
    服务停止事件 = ::CreateEventW(nullptr, TRUE, FALSE, nullptr);
    if (!服务停止事件) {
        更新服务状态(SERVICE_STOPPED, ::GetLastError(), 0);
        return;
    }

    if (!初始化Mfc运行时()) {
        更新服务状态(SERVICE_STOPPED, ERROR_DLL_INIT_FAILED, 0);
        ::CloseHandle(服务停止事件);
        服务停止事件 = nullptr;
        return;
    }

    日志::运行("[HY-Ego Headless] Service 模式启动: 服务名=" + 转UTF8(全局启动参数.服务名));
    更新服务状态(SERVICE_RUNNING, NO_ERROR, 0);
    const DWORD 退出码 = 运行宿主循环(全局启动参数, 服务停止事件, false);
    更新服务状态(SERVICE_STOPPED, 退出码, 0);

    ::CloseHandle(服务停止事件);
    服务停止事件 = nullptr;
}

std::wstring 当前模块路径() {
    std::vector<wchar_t> 缓冲(MAX_PATH, L'\0');
    while (true) {
        const DWORD 长度 = ::GetModuleFileNameW(nullptr, 缓冲.data(), static_cast<DWORD>(缓冲.size()));
        if (长度 == 0) return {};
        if (长度 < 缓冲.size() - 1) {
            return std::wstring(缓冲.data(), 长度);
        }
        缓冲.resize(缓冲.size() * 2, L'\0');
    }
}

bool 配置服务失败恢复策略(SC_HANDLE 服务句柄) {
    SC_ACTION 动作[3]{};
    动作[0].Type = SC_ACTION_RESTART;
    动作[0].Delay = 5000;
    动作[1].Type = SC_ACTION_RESTART;
    动作[1].Delay = 5000;
    动作[2].Type = SC_ACTION_RESTART;
    动作[2].Delay = 5000;

    SERVICE_FAILURE_ACTIONSW 失败恢复{};
    失败恢复.dwResetPeriod = 24 * 60 * 60;
    失败恢复.cActions = 3;
    失败恢复.lpsaActions = 动作;

    SERVICE_DELAYED_AUTO_START_INFO 延迟自动启动{};
    延迟自动启动.fDelayedAutostart = TRUE;

    return ::ChangeServiceConfig2W(服务句柄, SERVICE_CONFIG_FAILURE_ACTIONS, &失败恢复) != FALSE &&
        ::ChangeServiceConfig2W(服务句柄, SERVICE_CONFIG_DELAYED_AUTO_START_INFO, &延迟自动启动) != FALSE;
}

int 安装服务(const 结构_启动参数& 参数) {
    const std::wstring 模块路径 = 当前模块路径();
    if (模块路径.empty()) {
        std::wcerr << L"无法获取当前可执行文件路径\n";
        return 2;
    }

    std::wstring 二进制路径 = L"\"" + 模块路径 + L"\" --service --service-name=" + 参数.服务名;
    if (参数.启用相机观测) {
        二进制路径 += 参数.相机配置.允许回退到虚拟相机 ? L" --camera" : L" --camera-required";
    }

    SC_HANDLE SCM句柄 = ::OpenSCManagerW(nullptr, nullptr, SC_MANAGER_CREATE_SERVICE);
    if (!SCM句柄) {
        const DWORD 错误码 = ::GetLastError();
        std::wcerr << L"打开 SCM 失败: " << 取Win32错误文本(错误码) << L"\n";
        return static_cast<int>(错误码);
    }

    SC_HANDLE 服务句柄 = ::CreateServiceW(
        SCM句柄,
        参数.服务名.c_str(),
        参数.服务名.c_str(),
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_AUTO_START,
        SERVICE_ERROR_NORMAL,
        二进制路径.c_str(),
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr);

    if (!服务句柄) {
        const DWORD 错误码 = ::GetLastError();
        ::CloseServiceHandle(SCM句柄);
        std::wcerr << L"创建服务失败: " << 取Win32错误文本(错误码) << L"\n";
        return static_cast<int>(错误码);
    }

    (void)配置服务失败恢复策略(服务句柄);
    ::CloseServiceHandle(服务句柄);
    ::CloseServiceHandle(SCM句柄);

    std::wcout << L"已安装服务: " << 参数.服务名 << L"\n";
    return 0;
}

int 卸载服务(const 结构_启动参数& 参数) {
    SC_HANDLE SCM句柄 = ::OpenSCManagerW(nullptr, nullptr, SC_MANAGER_CONNECT);
    if (!SCM句柄) {
        const DWORD 错误码 = ::GetLastError();
        std::wcerr << L"打开 SCM 失败: " << 取Win32错误文本(错误码) << L"\n";
        return static_cast<int>(错误码);
    }

    SC_HANDLE 服务句柄 = ::OpenServiceW(SCM句柄, 参数.服务名.c_str(), DELETE | SERVICE_STOP | SERVICE_QUERY_STATUS);
    if (!服务句柄) {
        const DWORD 错误码 = ::GetLastError();
        ::CloseServiceHandle(SCM句柄);
        std::wcerr << L"打开服务失败: " << 取Win32错误文本(错误码) << L"\n";
        return static_cast<int>(错误码);
    }

    SERVICE_STATUS_PROCESS 状态{};
    DWORD 需要字节数 = 0;
    if (::QueryServiceStatusEx(服务句柄, SC_STATUS_PROCESS_INFO, reinterpret_cast<LPBYTE>(&状态), sizeof(状态), &需要字节数)) {
        if (状态.dwCurrentState != SERVICE_STOPPED && 状态.dwCurrentState != SERVICE_STOP_PENDING) {
            SERVICE_STATUS 停止状态{};
            ::ControlService(服务句柄, SERVICE_CONTROL_STOP, &停止状态);
        }
    }

    if (!::DeleteService(服务句柄)) {
        const DWORD 错误码 = ::GetLastError();
        ::CloseServiceHandle(服务句柄);
        ::CloseServiceHandle(SCM句柄);
        std::wcerr << L"删除服务失败: " << 取Win32错误文本(错误码) << L"\n";
        return static_cast<int>(错误码);
    }

    ::CloseServiceHandle(服务句柄);
    ::CloseServiceHandle(SCM句柄);
    std::wcout << L"已卸载服务: " << 参数.服务名 << L"\n";
    return 0;
}

}

int wmain(int argc, wchar_t* argv[]) {
    std::ios::sync_with_stdio(false);

    std::wstring 错误;
    if (!解析启动参数(argc, argv, &全局启动参数, &错误)) {
        std::wcerr << L"[HY-Ego Headless] 参数错误: " << 错误 << L"\n";
        打印帮助();
        return 2;
    }

    if (全局启动参数.模式 == 结构_启动参数::枚举_运行模式::帮助) {
        打印帮助();
        return 0;
    }

    if (全局启动参数.模式 == 结构_启动参数::枚举_运行模式::安装服务) {
        return 安装服务(全局启动参数);
    }
    if (全局启动参数.模式 == 结构_启动参数::枚举_运行模式::卸载服务) {
        return 卸载服务(全局启动参数);
    }
    if (全局启动参数.模式 == 结构_启动参数::枚举_运行模式::任务控制总控回归测试) {
        if (!初始化Mfc运行时()) {
            return 3;
        }
        return static_cast<int>(运行任务控制总控回归测试套件_());
    }

    if (全局启动参数.模式 == 结构_启动参数::枚举_运行模式::作为服务) {
        SERVICE_TABLE_ENTRYW 服务表[] = {
            { const_cast<LPWSTR>(全局启动参数.服务名.c_str()), 服务主函数 },
            { nullptr, nullptr }
        };
        if (!::StartServiceCtrlDispatcherW(服务表)) {
            const DWORD 错误码 = ::GetLastError();
            std::wcerr << L"连接 Service Control Manager 失败: " << 取Win32错误文本(错误码) << L"\n";
            return static_cast<int>(错误码);
        }
        return 0;
    }

    if (!初始化Mfc运行时()) {
        return 3;
    }

    ::SetConsoleCtrlHandler(控制台控制处理, TRUE);
    日志::运行("[HY-Ego Headless] 控制台模式启动");
    const DWORD 退出码 = 运行宿主循环(全局启动参数, nullptr, true);
    return static_cast<int>(退出码);
}
