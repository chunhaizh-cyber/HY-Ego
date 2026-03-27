#include "framework.h"
#include "鱼巢Headless入口.h"

#include <algorithm>
#include <bit>
#include <atomic>
#include <chrono>
#include <deque>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <optional>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <thread>
#include <unordered_set>
#include <variant>
#include <vector>
#include <winsvc.h>

#pragma comment(lib, "odbc32.lib")

import 基础数据类型模块;
import 因果信息模块;
import 日志模块;
import 后台前端通道模块;
import 场景模块;
import 数据仓库模块;
import 动态模块;
import 本能动作管理模块;
import 主信息定义模块;
import 方法环境模块;
import 任务控制总控模块_v0;
import 自我模块;
import 自我本能方法_最小试探模块;
import 自我线程模块;
import 自治宿主模块;
import 相机观测运行时模块;
import 世界树环境模块;
import 需求环境模块;
import 语素环境模块;
import 特征类型定义模块;

namespace {
constexpr wchar_t 默认服务名[] = L"鱼巢";
constexpr wchar_t 单实例互斥名[] = L"Global\\鱼巢.后台单实例";
constexpr wchar_t 默认SQL实例[] = L".\\SQLEXPRESS";
constexpr wchar_t 默认SQL数据库[] = L"FishNestRuntime";
constexpr wchar_t 默认SQL快照键[] = L"headless.default";
constexpr std::size_t 快照枚举最大节点数 = 262144;
constexpr DWORD 服务等待提示毫秒 = 3000;
constexpr auto 停机宽限时长 = std::chrono::seconds(8);

struct 结构_启动参数 {
    enum class 枚举_运行模式 {
        控制台,
        作为服务,
        安装服务,
        卸载服务,
        任务控制总控回归测试,
        安全服务因果回归测试,
        安全服务物理长时回归测试,
        SQL运行时快照测试,
        帮助
    };

    枚举_运行模式 模式 = 枚举_运行模式::控制台;
    结构_自治宿主配置 宿主配置{};
    结构_相机观测运行时配置 相机配置{};
    std::chrono::seconds 心跳周期{ 5 };
    std::optional<std::chrono::seconds> 最大运行时长;
    std::chrono::seconds SQL写入周期{ 1 };
    std::wstring 服务名 = 默认服务名;
    std::wstring SQL实例 = 默认SQL实例;
    std::wstring SQL数据库 = 默认SQL数据库;
    std::wstring SQL快照键 = 默认SQL快照键;
    bool 启用相机观测 = false;
    bool 启用SQL运行时快照 = false;
};

struct 结构_前端人类输入 {
    std::uint64_t 序号 = 0;
    时间戳 提交时间 = 0;
    I64 来源码 = 1001;
    std::string 文本;
    std::shared_ptr<std::string> 文本对象;
};

struct 结构_快照诊断状态 {
    std::mutex 互斥{};
    std::chrono::steady_clock::time_point 上次日志时间{};
    std::uint64_t 调用序号 = 0;

    bool 应记录() {
        std::lock_guard<std::mutex> 锁(互斥);
        const auto 当前 = std::chrono::steady_clock::now();
        const auto 当前调用序号 = ++调用序号;
        if (当前调用序号 <= 3 || 当前 - 上次日志时间 >= std::chrono::seconds(2)) {
            上次日志时间 = 当前;
            return true;
        }
        return false;
    }
};

struct 结构_完整快照生成诊断状态 {
    std::mutex 互斥{};
    std::chrono::steady_clock::time_point 上次日志时间{};
    std::uint64_t 调用序号 = 0;

    bool 应记录() {
        std::lock_guard<std::mutex> 锁(互斥);
        const auto 当前 = std::chrono::steady_clock::now();
        const auto 当前调用序号 = ++调用序号;
        if (当前调用序号 <= 3 || 当前 - 上次日志时间 >= std::chrono::seconds(2)) {
            上次日志时间 = 当前;
            return true;
        }
        return false;
    }
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

std::string 取词文本_(const 词性节点类* 节点);

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

bool 取Vec3I64_(const std::optional<特征快照值>& 快照, I64& x, I64& y, I64& z) {
    if (!快照 || !std::holds_alternative<VecU句柄>(*快照)) return false;
    const auto 句柄 = std::get<VecU句柄>(*快照);
    const auto* 值 = 世界树.值池().取VecU只读指针(句柄);
    if (!值 || 值->size() < 3) return false;
    x = std::bit_cast<std::int64_t>((*值)[0]);
    y = std::bit_cast<std::int64_t>((*值)[1]);
    z = std::bit_cast<std::int64_t>((*值)[2]);
    return true;
}

std::size_t 取VecU长度_(const std::optional<特征快照值>& 快照) {
    if (!快照 || !std::holds_alternative<VecU句柄>(*快照)) return 0;
    const auto 句柄 = std::get<VecU句柄>(*快照);
    const auto* 值 = 世界树.值池().取VecU只读指针(句柄);
    return 值 ? 值->size() : 0;
}

const 词性节点类* 特征_中心X_() {
    static const 词性节点类* v = nullptr;
    if (!v) v = 语素集.添加词性词("中心X", "名词");
    return v;
}

const 词性节点类* 特征_中心Y_() {
    static const 词性节点类* v = nullptr;
    if (!v) v = 语素集.添加词性词("中心Y", "名词");
    return v;
}

const 词性节点类* 特征_中心Z_() {
    static const 词性节点类* v = nullptr;
    if (!v) v = 语素集.添加词性词("中心Z", "名词");
    return v;
}

const 词性节点类* 特征_尺寸X_() {
    static const 词性节点类* v = nullptr;
    if (!v) v = 语素集.添加词性词("尺寸X", "名词");
    return v;
}

const 词性节点类* 特征_尺寸Y_() {
    static const 词性节点类* v = nullptr;
    if (!v) v = 语素集.添加词性词("尺寸Y", "名词");
    return v;
}

const 词性节点类* 特征_尺寸Z_() {
    static const 词性节点类* v = nullptr;
    if (!v) v = 语素集.添加词性词("尺寸Z", "名词");
    return v;
}

const 词性节点类* 场景名_观察存在集_() {
    static const 词性节点类* v = nullptr;
    if (!v) v = 语素集.添加词性词("观察存在集", "名词");
    return v;
}

const 词性节点类* 场景名_主窗口自我观察输入_() {
    static const 词性节点类* v = nullptr;
    if (!v) v = 语素集.添加词性词("主窗口_自我观察输入", "名词");
    return v;
}

const 词性节点类* 场景名_候选存在集_() {
    static const 词性节点类* v = nullptr;
    if (!v) v = 语素集.添加词性词("候选存在集", "名词");
    return v;
}

const 词性节点类* 场景型_候选集场景_() {
    static const 词性节点类* v = nullptr;
    if (!v) v = 语素集.添加词性词("候选集场景", "名词");
    return v;
}

场景节点类* 查找直属子场景_按名称或类型_(
    基础信息节点类* 父节点,
    const 词性节点类* 名称,
    const 词性节点类* 类型,
    const std::string& 调用点) {
    if (!父节点) return nullptr;
    for (auto* 场景 : 世界树.获取子场景(父节点, 调用点)) {
        auto* 信息 = (场景 && 场景->主信息) ? dynamic_cast<场景节点主信息类*>(场景->主信息) : nullptr;
        if (!信息) continue;
        if (名称 && 信息->名称 == 名称) return 场景;
        if (类型 && 信息->类型 == 类型) return 场景;
    }
    return nullptr;
}

std::vector<存在节点类*> 提取直属存在_(基础信息节点类* 父节点, const std::string& 调用点) {
    if (!父节点) return {};
    return 世界树.获取子存在(父节点, 调用点);
}

struct 结构_自我再现场景解析结果 {
    场景节点类* 场景 = nullptr;
    std::vector<存在节点类*> 存在列表{};
};

结构_自我再现场景解析结果 解析自我再现场景_() {
    结构_自我再现场景解析结果 结果{};

    auto 尝试场景 = [&](场景节点类* 场景, bool 回写到自我所在场景) -> bool {
        if (!场景) return false;
        auto 临时存在 = 提取直属存在_(场景, "鱼巢::自我场景再现/提取直属存在");
        if (!结果.场景) {
            结果.场景 = 场景;
        }
        if (临时存在.empty()) {
            return false;
        }
        结果.场景 = 场景;
        结果.存在列表 = std::move(临时存在);
        if (回写到自我所在场景) {
            世界树.自我所在场景 = 场景;
        }
        return true;
    };

    auto* 世界根 = 世界树.世界根();
    auto* 当前自我场景 = 世界树.获取自我所在场景();
    auto* 观察存在集 = 查找直属子场景_按名称或类型_(
        世界根,
        场景名_观察存在集_(),
        nullptr,
        "鱼巢::自我场景再现/查观察存在集");
    if (尝试场景(观察存在集, true)) {
        return 结果;
    }

    if (尝试场景(当前自我场景, false)) {
        return 结果;
    }
    if (当前自我场景) {
        auto* 当前场景候选存在集 = 查找直属子场景_按名称或类型_(
            当前自我场景,
            场景名_候选存在集_(),
            场景型_候选集场景_(),
            "鱼巢::自我场景再现/查当前场景候选存在集");
        if (尝试场景(当前场景候选存在集, true)) {
            return 结果;
        }
    }

    auto* 内部世界 = 世界树.取内部世界()
        ? static_cast<基础信息节点类*>(世界树.取内部世界())
        : 世界根;
    auto* 自我观察输入 = 查找直属子场景_按名称或类型_(
        内部世界,
        场景名_主窗口自我观察输入_(),
        nullptr,
        "鱼巢::自我场景再现/查自我观察输入");
    auto* 候选存在集 = 查找直属子场景_按名称或类型_(
        自我观察输入,
        场景名_候选存在集_(),
        场景型_候选集场景_(),
        "鱼巢::自我场景再现/查候选存在集");
    if (尝试场景(候选存在集, true)) {
        return 结果;
    }

    (void)尝试场景(自我观察输入, true);
    return 结果;
}

struct 结构_自我场景再现共享状态 {
    mutable std::mutex 互斥;
    bool 已启用 = false;
    std::uint64_t 帧序号 = 0;
    std::string 最近状态摘要 = "自我场景再现未启动";

    void 启动() {
        std::lock_guard<std::mutex> 锁(互斥);
        已启用 = true;
        最近状态摘要 = "自我场景再现已启动";
    }

    void 停止() {
        std::lock_guard<std::mutex> 锁(互斥);
        已启用 = false;
        最近状态摘要 = "自我场景再现已停止";
    }

    结构_后台自我场景快照 生成快照(const 相机观测运行时类* 相机运行时 = nullptr) {
        结构_后台自我场景快照 输出{};
        bool 已启用本次 = false;
        {
            std::lock_guard<std::mutex> 锁(互斥);
            已启用本次 = 已启用;
            输出.再现运行中 = 已启用;
            输出.帧序号 = 帧序号;
            输出.状态摘要 = 最近状态摘要;
        }
        输出.后台在线 = true;
        if (!已启用本次) {
            return 输出;
        }

        if (!相机运行时) {
            std::lock_guard<std::mutex> 锁(互斥);
            最近状态摘要 = "当前未启用相机桥接缓存";
            输出.状态摘要 = 最近状态摘要;
            return 输出;
        }

        const auto 相机快照 = 相机运行时->快照();
        const auto 桥接存在列表 = 相机运行时->取最近桥接存在列表();
        输出.场景主键 = "主窗口_自我观察输入/候选存在集";
        输出.存在列表.reserve(桥接存在列表.size());

        for (const auto& 存在 : 桥接存在列表) {
            结构_后台自我场景存在快照 项{};
            项.存在主键 = "bridge_candidate_" + std::to_string(static_cast<long long>(存在.跟踪ID));
            项.有位置 = true;
            项.中心X_mm = 存在.中心X_mm;
            项.中心Y_mm = 存在.中心Y_mm;
            项.中心Z_mm = 存在.中心Z_mm;
            项.有尺寸 = true;
            项.尺寸X_mm = 存在.尺寸X_mm;
            项.尺寸Y_mm = 存在.尺寸Y_mm;
            项.尺寸Z_mm = 存在.尺寸Z_mm;
            项.轮廓块数 = 存在.轮廓块数;
            输出.存在列表.push_back(std::move(项));
        }

        {
            std::lock_guard<std::mutex> 锁(互斥);
            输出.帧序号 = ++帧序号;
            if (!相机快照.运行中) {
                最近状态摘要 = "相机运行时未运行";
            }
            else if (相机快照.已处理帧数 == 0) {
                最近状态摘要 = "相机已启动，尚未处理到可回放帧";
            }
            else if (输出.存在列表.empty()) {
                最近状态摘要 = "相机桥接已运行，但当前没有存在";
            }
            else {
                最近状态摘要 = "已提取 " + std::to_string(输出.存在列表.size()) + " 个存在（来自相机桥接）";
            }
            输出.状态摘要 = 最近状态摘要;
        }
        return 输出;
    }
};

std::atomic_bool 控制台请求退出{ false };
HANDLE 服务停止事件 = nullptr;
SERVICE_STATUS_HANDLE 服务状态句柄 = nullptr;
SERVICE_STATUS 服务状态{};
结构_启动参数 全局启动参数{};

HANDLE 创建单实例守卫_()
{
    ::SetLastError(ERROR_SUCCESS);
    HANDLE 互斥句柄 = ::CreateMutexW(nullptr, FALSE, 单实例互斥名);
    if (!互斥句柄) {
        return nullptr;
    }
    if (::GetLastError() == ERROR_ALREADY_EXISTS) {
        ::CloseHandle(互斥句柄);
        return nullptr;
    }
    return 互斥句柄;
}

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

std::string 取词文本_(const 词性节点类* 节点)
{
    if (!节点) return "<null>";
    try {
        const auto 文本 = 语素集.获取词(节点);
        if (!文本.empty()) return 文本;
    }
    catch (...) {
    }
    const auto 主键 = 节点->获取主键();
    return 主键.empty() ? "<unnamed>" : 主键;
}

const 词性节点类* 取特征节点类型_(const 特征节点类* 特征)
{
    auto* 特征信息 = (特征 && 特征->主信息)
        ? dynamic_cast<特征节点主信息类*>(特征->主信息)
        : nullptr;
    return 特征信息 ? 特征信息->类型 : nullptr;
}

const 词性节点类* 取状态特征类型_(const 状态节点类* 状态)
{
    auto* 状态信息 = (状态 && 状态->主信息) ? dynamic_cast<状态节点主信息类*>(状态->主信息) : nullptr;
    return 状态信息 ? 取特征节点类型_(状态信息->状态特征) : nullptr;
}

const 词性节点类* 取动态特征类型_(const 动态节点类* 动态)
{
    auto* 动态信息 = (动态 && 动态->主信息) ? dynamic_cast<动态节点主信息类*>(动态->主信息) : nullptr;
    return 动态信息 ? 取特征节点类型_(动态信息->动态特征) : nullptr;
}

bool 状态列表含特征_(const std::vector<状态节点类*>& 状态列表, const 词性节点类* 特征类型)
{
    if (!特征类型) return false;
    for (auto* 状态 : 状态列表) {
        if (取状态特征类型_(状态) == 特征类型) return true;
    }
    return false;
}

bool 状态含特征_(const 状态节点类* 状态, const 词性节点类* 特征类型)
{
    return 特征类型 && 取状态特征类型_(状态) == 特征类型;
}

bool 动态列表含特征_(const std::vector<动态节点类*>& 动态列表, const 词性节点类* 特征类型)
{
    if (!特征类型) return false;
    for (auto* 动态 : 动态列表) {
        if (取动态特征类型_(动态) == 特征类型) return true;
    }
    return false;
}

std::string 汇总状态特征_(const std::vector<状态节点类*>& 状态列表)
{
    std::vector<std::string> 去重{};
    for (auto* 状态 : 状态列表) {
        const auto* 特征类型 = 取状态特征类型_(状态);
        if (!特征类型) continue;
        const auto 文本 = 取词文本_(特征类型);
        if (std::find(去重.begin(), 去重.end(), 文本) == 去重.end()) {
            去重.push_back(文本);
        }
    }

    if (去重.empty()) return "<none>";

    std::string out;
    for (std::size_t i = 0; i < 去重.size(); ++i) {
        if (i != 0) out += ",";
        out += 去重[i];
    }
    return out;
}

std::string 汇总状态特征_(const 状态节点类* 状态)
{
    const auto* 特征类型 = 取状态特征类型_(状态);
    if (!特征类型) return "<none>";
    const auto 文本 = 取词文本_(特征类型);
    return 文本.empty() ? "<none>" : 文本;
}

std::string 取状态值文本_(const 特征快照值& 值);

std::string 节点显示文本_(const 基础信息节点类* 节点)
{
    if (!节点) return "<未知主体>";
    const auto 主键 = 节点->获取主键();
    return 主键.empty() ? "<无主键主体>" : 主键;
}

std::string 取状态主体显示文本_(const 状态节点类* 状态)
{
    auto* 状态信息 = (状态 && 状态->主信息) ? dynamic_cast<状态节点主信息类*>(状态->主信息) : nullptr;
    return 状态信息 ? 节点显示文本_(状态信息->状态主体) : std::string("<未知主体>");
}

std::string 生成状态描述键_(const 状态节点类* 状态)
{
    if (!状态) return "<none>";
    return 取状态主体显示文本_(状态) + "." + 汇总状态特征_(状态);
}

std::string 生成状态标准描述_(const 状态节点类* 状态)
{
    auto* 状态信息 = (状态 && 状态->主信息) ? dynamic_cast<状态节点主信息类*>(状态->主信息) : nullptr;
    if (!状态信息) return "<none>";
    return 生成状态描述键_(状态) + "=" + 取状态值文本_(状态信息->状态值);
}

std::string 单状态静态短语_(const 状态节点类* 状态, bool 强调保持 = false)
{
    auto out = 生成状态标准描述_(状态);
    if (强调保持) out += "（保持）";
    return out;
}

std::string 取状态原因类别_(const 状态节点类* 状态)
{
    auto* 状态信息 = (状态 && 状态->主信息) ? dynamic_cast<状态节点主信息类*>(状态->主信息) : nullptr;
    return 状态信息 ? 状态信息->变化原因类别 : std::string{};
}

std::string 取状态原因显示文本_(const 状态节点类* 状态)
{
    auto* 状态信息 = (状态 && 状态->主信息) ? dynamic_cast<状态节点主信息类*>(状态->主信息) : nullptr;
    if (!状态信息) return {};
    if (!状态信息->变化原因说明.empty()) return 状态信息->变化原因说明;
    return 状态信息->变化原因类别;
}

bool 是显式动作动态_(const 动态节点类* 动态)
{
    auto* 动态信息 = (动态 && 动态->主信息)
        ? dynamic_cast<动态节点主信息类*>(动态->主信息)
        : nullptr;
    if (!动态信息) return false;
    if (动态信息->来源类型 != 枚举_动态来源类型::未定义) return true;
    if (动态信息->来源动作名 || 动态信息->来源方法首节点) return true;
    return 动态信息->初始状态 == nullptr
        && 动态信息->结果状态 == nullptr
        && 动态信息->动态主体 != nullptr
        && 动态信息->动态特征 != nullptr;
}

动态节点类* 取因动作动态_(const 因果主信息类* 信息)
{
    if (!信息) return nullptr;
    auto 打分 = [&](const 动态节点类* 动态) -> int {
        if (!是显式动作动态_(动态)) return -1;
        auto* 动态信息 = (动态 && 动态->主信息)
            ? dynamic_cast<动态节点主信息类*>(动态->主信息)
            : nullptr;
        if (!动态信息) return -1;
        switch (动态信息->来源动作相位) {
        case 枚举_动作事件相位::开始运行:
            return 3;
        case 枚举_动作事件相位::未定义:
            return 2;
        case 枚举_动作事件相位::完成:
        case 枚举_动作事件相位::失败:
            return 1;
        default:
            return 0;
        }
    };
    动态节点类* 最优 = nullptr;
    int 最优分 = -1;
    auto 更新最优 = [&](动态节点类* 动态) {
        const int 分 = 打分(动态);
        if (分 > 最优分) {
            最优 = 动态;
            最优分 = 分;
        }
    };
    更新最优(信息->因信息);
    for (auto* 动态 : 信息->过程动态列表) {
        更新最优(动态);
    }
    if (最优) return 最优;
    if (信息->因信息) return 信息->因信息;
    return 信息->过程动态列表.empty() ? nullptr : 信息->过程动态列表.front();
}

std::string 动作事件相位显示文本_(枚举_动作事件相位 相位)
{
    switch (相位) {
    case 枚举_动作事件相位::开始运行:
        return "开始运行";
    case 枚举_动作事件相位::完成:
        return "完成";
    case 枚举_动作事件相位::失败:
        return "失败";
    default:
        return {};
    }
}

std::vector<动态节点类*> 提取过程动态_不含因动作_(const 因果主信息类* 信息)
{
    std::vector<动态节点类*> out{};
    if (!信息) return out;
    auto* 因动作 = 取因动作动态_(信息);
    bool 已跳过因动作 = false;
    for (auto* 动态 : 信息->过程动态列表) {
        if (!已跳过因动作 && 动态 == 因动作) {
            已跳过因动作 = true;
            continue;
        }
        out.push_back(动态);
    }
    return out;
}

std::string 取动态来源动作显示文本_(const 动态节点类* 动态)
{
    auto* 动态信息 = (动态 && 动态->主信息)
        ? dynamic_cast<动态节点主信息类*>(动态->主信息)
        : nullptr;
    if (!动态信息) return {};
    std::string 动作名{};
    if (动态信息->来源动作名) {
        动作名 = 取词文本_(动态信息->来源动作名);
    }
    if (动作名.empty() && 动态信息->来源方法首节点 && 动态信息->来源方法首节点->主信息) {
        if (auto* 方法首信息 = dynamic_cast<方法首节点主信息类*>(动态信息->来源方法首节点->主信息)) {
            if (方法首信息->动作名) {
                动作名 = 取词文本_(方法首信息->动作名);
            }
        }
    }
    if (动作名.empty()) return {};
    const auto 相位 = 动作事件相位显示文本_(动态信息->来源动作相位);
    return 相位.empty() ? 动作名 : (动作名 + 相位);
}

std::string 汇总因动作详情_(const 因果主信息类* 信息)
{
    auto* 动作 = 取因动作动态_(信息);
    if (!动作) return "<none>";
    if (!是显式动作动态_(动作)) {
        return 取词文本_(取动态特征类型_(动作));
    }
    auto 文本 = 取动态来源动作显示文本_(动作);
    if (文本.empty()) {
        文本 = 取词文本_(取动态特征类型_(动作));
    }
    return 文本.empty() ? "<none>" : 文本;
}

std::string 取因动作标签_(const 因果主信息类* 信息)
{
    auto* 动作 = 取因动作动态_(信息);
    if (!动作) return {};
    return 是显式动作动态_(动作) ? "动作" : "状态变化";
}

std::string 生成因动作模式键_(const 因果主信息类* 信息)
{
    auto* 动作 = 取因动作动态_(信息);
    auto* 动态信息 = (动作 && 动作->主信息)
        ? dynamic_cast<动态节点主信息类*>(动作->主信息)
        : nullptr;
    if (!动态信息) return "none";
    return 取词文本_(取动态特征类型_(动作)) + "|path=" + std::to_string(动态信息->动态路径签名);
}

std::string 方向显示文本_(bool 可比较, std::int64_t 方向值)
{
    if (!可比较) return "不可比较";
    if (方向值 > 0) return "上升(+1)";
    if (方向值 < 0) return "下降(-1)";
    return "持平(0)";
}

std::string 方向自然语言文本_(bool 可比较, std::int64_t 方向值)
{
    if (!可比较) return "发生变化";
    if (方向值 > 0) return "变大";
    if (方向值 < 0) return "变小";
    return "保持不变";
}

std::string 生成状态结果描述_(const 状态节点类* 状态, bool 可比较, std::int64_t 方向值)
{
    if (!状态) return "<none>";
    auto out = 生成状态标准描述_(状态);
    if (可比较) {
        out += "（" + 方向自然语言文本_(可比较, 方向值) + "）";
    }
    return out;
}

std::string 果方向文本_(const 因果主信息类* 信息)
{
    if (!信息) return "不可比较";
    return 方向显示文本_(信息->结果方向可比较, 信息->结果方向值);
}

状态节点类* 取因果主结果状态_(const 因果主信息类* 信息)
{
    if (!信息) return nullptr;
    return 信息->主结果状态 ? 信息->主结果状态 : 信息->结果状态;
}

std::string 汇总因果结果_(const 因果主信息类* 信息)
{
    auto* 结果状态 = 取因果主结果状态_(信息);
    if (!结果状态) return "<none>";
    return 生成状态结果描述_(结果状态, 信息->结果方向可比较, 信息->结果方向值)
        + "|" + 果方向文本_(信息);
}

bool 条件方向表示变化_(const 因果主信息类* 信息, std::size_t 序号)
{
    if (!信息 || 序号 >= 信息->条件状态列表.size()) return false;
    const bool 可比较 = 序号 < 信息->条件方向可比较列表.size()
        && 信息->条件方向可比较列表[序号] != 0;
    if (!可比较) return false;
    const std::int64_t 方向值 = 序号 < 信息->条件方向值列表.size()
        ? 信息->条件方向值列表[序号]
        : 0;
    return 方向值 != 0;
}

std::string 生成因果抽象组键_(const 因果主信息类* 信息)
{
    auto* 结果状态 = 取因果主结果状态_(信息);
    if (!结果状态) return {};
    std::string key = "result:";
    key += 生成状态描述键_(结果状态);
    key += "|dir:";
    key += 信息->结果方向可比较 ? std::to_string(信息->结果方向值) : std::string{ "na" };
    key += "|reason:";
    {
        const auto 原因类别 = 取状态原因类别_(结果状态);
        key += 原因类别.empty() ? std::string{ "none" } : 原因类别;
    }
    for (std::size_t i = 0; i < 信息->条件状态列表.size(); ++i) {
        key += "|cause:";
        key += 生成状态描述键_(信息->条件状态列表[i]);
        key += "#reason:";
        {
            const auto 原因类别 = 取状态原因类别_(信息->条件状态列表[i]);
            key += 原因类别.empty() ? std::string{ "none" } : 原因类别;
        }
        if (条件方向表示变化_(信息, i)) {
            key += "@dynamic";
        }
        else {
            auto* 状态信息 = (信息->条件状态列表[i] && 信息->条件状态列表[i]->主信息)
                ? dynamic_cast<状态节点主信息类*>(信息->条件状态列表[i]->主信息)
                : nullptr;
            key += "@static:";
            key += 状态信息 ? 取状态值文本_(状态信息->状态值) : std::string{ "<none>" };
        }
    }
    key += "|action:";
    key += 生成因动作模式键_(信息);
    return key;
}

std::string 生成因果条件模式键_(const 因果主信息类* 信息)
{
    if (!信息) return {};
    std::string key = "action:" + 生成因动作模式键_(信息);
    for (std::size_t i = 0; i < 信息->条件状态列表.size(); ++i) {
        key += "|";
        key += "reason:";
        {
            const auto 原因类别 = 取状态原因类别_(信息->条件状态列表[i]);
            key += 原因类别.empty() ? std::string{ "none" } : 原因类别;
        }
        key += "#";
        if (条件方向表示变化_(信息, i)) {
            const std::int64_t 方向值 = i < 信息->条件方向值列表.size()
                ? 信息->条件方向值列表[i]
                : 0;
            key += "dynamic:";
            key += std::to_string(方向值);
        }
        else {
            const bool 可比较 = i < 信息->条件方向可比较列表.size()
                && 信息->条件方向可比较列表[i] != 0;
            auto* 状态信息 = (信息->条件状态列表[i] && 信息->条件状态列表[i]->主信息)
                ? dynamic_cast<状态节点主信息类*>(信息->条件状态列表[i]->主信息)
                : nullptr;
            key += 可比较 ? "hold:" : "static:";
            key += 状态信息 ? 取状态值文本_(状态信息->状态值) : std::string{ "<none>" };
        }
    }
    return key;
}

struct 结构_因果状态判定结果
{
    std::unordered_map<const 基础信息节点类*, std::string> 节点标签{};
};

结构_因果状态判定结果 构建因果状态判定结果_(const std::vector<基础信息节点类*>& 节点列表)
{
    结构_因果状态判定结果 out{};
    std::unordered_map<std::string, std::unordered_map<std::string, std::size_t>> 分组模式频次{};
    for (auto* 节点 : 节点列表) {
        auto* 信息 = (节点 && 节点->主信息) ? dynamic_cast<因果主信息类*>(节点->主信息) : nullptr;
        if (!信息 || 信息->条件状态列表.empty() || !取因果主结果状态_(信息)) continue;
        ++分组模式频次[生成因果抽象组键_(信息)][生成因果条件模式键_(信息)];
    }

    for (auto* 节点 : 节点列表) {
        auto* 信息 = (节点 && 节点->主信息) ? dynamic_cast<因果主信息类*>(节点->主信息) : nullptr;
        std::string 标签 = "待细化";
        if (信息 && !信息->条件状态列表.empty() && 取因果主结果状态_(信息)) {
            const auto it = 分组模式频次.find(生成因果抽象组键_(信息));
            const bool 是否冲突 = it != 分组模式频次.end() && it->second.size() > 1;
            if (是否冲突) {
                标签 = "冲突";
            }
            else if (信息->观察次数 >= 2) {
                标签 = "稳定";
            }
        }
        out.节点标签[节点] = 标签;
    }
    return out;
}

std::string 查询因果状态标签_(
    const 基础信息节点类* 节点,
    const 结构_因果状态判定结果* 判定结果)
{
    if (!节点 || !判定结果) return "待细化";
    const auto it = 判定结果->节点标签.find(节点);
    return it == 判定结果->节点标签.end() ? "待细化" : it->second;
}

std::string 汇总因果条件_(const 因果主信息类* 信息)
{
    if (!信息 || 信息->条件状态列表.empty()) return "<none>";
    std::string out;
    for (std::size_t i = 0; i < 信息->条件状态列表.size(); ++i) {
        if (i != 0) out += ",";
        out += 生成状态标准描述_(信息->条件状态列表[i]);
    }
    return out;
}

struct 结构_因果条件短语分组
{
    std::vector<std::string> 变化条件{};
    std::vector<std::string> 背景条件{};
};

结构_因果条件短语分组 收集因果条件短语分组_(const 因果主信息类* 信息)
{
    结构_因果条件短语分组 out{};
    if (!信息) return out;
    for (std::size_t i = 0; i < 信息->条件状态列表.size(); ++i) {
        out.背景条件.push_back(生成状态标准描述_(信息->条件状态列表[i]));
    }
    return out;
}

std::string 翻译因果语句_(const 因果主信息类* 信息)
{
    auto* 结果状态 = 取因果主结果状态_(信息);
    if (!信息 || 信息->条件状态列表.empty() || !结果状态) return "因果信息不完整。";
    const auto 条件分组 = 收集因果条件短语分组_(信息);
    auto 拼接短语 = [](const std::vector<std::string>& 列表) {
        std::string out;
        for (std::size_t i = 0; i < 列表.size(); ++i) {
            if (i != 0) out += "、";
            out += 列表[i];
        }
        return out;
    };
    const auto 条件 = 拼接短语(条件分组.背景条件);
    const auto 动作标签 = 取因动作标签_(信息);
    const auto 动作 = 汇总因动作详情_(信息);

    const auto 结果 = 生成状态结果描述_(结果状态, 信息->结果方向可比较, 信息->结果方向值);
    const bool 有动作短语 = !动作标签.empty() && 动作 != "<none>";
    if (有动作短语) {
        return std::string("当状态{") + 条件 + "}成立时，发生" + 动作标签 + "{" + 动作 + "}，结果状态为" + 结果 + "。";
    }
    return std::string("当状态{") + 条件 + "}成立时，结果状态为" + 结果 + "。";
}

std::string 汇总动态特征_(const std::vector<动态节点类*>& 动态列表)
{
    std::vector<std::string> 去重{};
    for (auto* 动态 : 动态列表) {
        auto 文本 = 取动态来源动作显示文本_(动态);
        if (文本.empty()) {
            const auto* 特征类型 = 取动态特征类型_(动态);
            if (!特征类型) continue;
            文本 = 取词文本_(特征类型);
        }
        if (std::find(去重.begin(), 去重.end(), 文本) == 去重.end()) {
            去重.push_back(文本);
        }
    }

    if (去重.empty()) return "<none>";

    std::string out;
    for (std::size_t i = 0; i < 去重.size(); ++i) {
        if (i != 0) out += ",";
        out += 去重[i];
    }
    return out;
}

std::vector<基础信息节点类*> 收集因果节点列表_(std::size_t 上限 = 0)
{
    return 因果集.获取实例因果列表(上限);
}

std::vector<基础信息节点类*> 收集抽象因果节点列表_(std::size_t 上限 = 0)
{
    return 因果集.获取抽象因果列表(上限);
}

std::vector<基础信息节点类*> 收集因果链节点列表_(std::size_t 上限 = 0)
{
    return 因果集.获取因果链列表(上限);
}

std::string 生成因果链详细摘要_(std::size_t 展示上限 = 24)
{
    const auto 全部因果 = 收集因果节点列表_();
    const auto 抽象因果 = 收集抽象因果节点列表_();
    const auto 因果链列表 = 收集因果链节点列表_();
    const auto 判定结果 = 构建因果状态判定结果_(全部因果);
    std::ostringstream os;
    os << "实例因果节点数: " << 全部因果.size() << "\n";
    os << "抽象因果节点数: " << 抽象因果.size() << "\n";
    os << "因果链节点数: " << 因果链列表.size() << "\n";

    if (全部因果.empty() && 抽象因果.empty() && 因果链列表.empty()) {
        os << "\n当前没有因果信息。";
        return os.str();
    }

    if (!全部因果.empty()) {
        os << "\n[实例因果]\n";
        const auto 实际展示数 = (std::min)(展示上限, 全部因果.size());
        for (std::size_t i = 0; i < 实际展示数; ++i) {
            auto* 节点 = 全部因果[i];
            auto* 信息 = (节点 && 节点->主信息) ? dynamic_cast<因果主信息类*>(节点->主信息) : nullptr;
            if (!信息) continue;

            os << "因果[" << (i + 1) << "] "
               << "状态=" << 查询因果状态标签_(节点, &判定结果)
               << " | 观察=" << 信息->观察次数
               << " | 稳定度Q=" << 信息->稳定度Q
               << " | 因={状态集:" << 汇总因果条件_(信息) << "; 动作:" << 汇总因动作详情_(信息) << "}"
               << " | 过程={" << 汇总动态特征_(提取过程动态_不含因动作_(信息)) << "}"
               << " | 结果={" << 汇总因果结果_(信息) << "}"
               << " | 说明=" << 翻译因果语句_(信息)
               << "\n";
        }
        if (全部因果.size() > 实际展示数) {
            os << "其余 " << (全部因果.size() - 实际展示数) << " 条实例因果已省略。\n";
        }
    }

    if (!抽象因果.empty()) {
        os << "\n[抽象因果]\n";
        const auto 实际展示数 = (std::min)(展示上限, 抽象因果.size());
        for (std::size_t i = 0; i < 实际展示数; ++i) {
            auto* 节点 = 抽象因果[i];
            auto* 信息 = (节点 && 节点->主信息) ? dynamic_cast<因果模板主信息类*>(节点->主信息) : nullptr;
            if (!信息) continue;
            os << "抽象[" << (i + 1) << "] 动作模板={" << 节点显示文本_(信息->动作模板)
               << "} | 条件模板=" << 信息->条件模板列表.size()
               << " | 结果模板=" << 信息->结果模板列表.size()
               << " | 证据=" << 信息->证据次数
               << " | 稳定度=" << 信息->稳定度
               << "\n";
        }
        if (抽象因果.size() > 实际展示数) {
            os << "其余 " << (抽象因果.size() - 实际展示数) << " 条抽象因果已省略。\n";
        }
    }

    if (!因果链列表.empty()) {
        os << "\n[因果链]\n";
        const auto 实际展示数 = (std::min)(展示上限, 因果链列表.size());
        for (std::size_t i = 0; i < 实际展示数; ++i) {
            auto* 节点 = 因果链列表[i];
            auto* 信息 = (节点 && 节点->主信息) ? dynamic_cast<因果链主信息类*>(节点->主信息) : nullptr;
            if (!信息) continue;
            os << "链[" << (i + 1) << "] 长度=" << 信息->因果实例序列.size()
               << " | 时间=" << 信息->起始时间 << "->" << 信息->结束时间 << "\n";
            const auto 链内展示数 = (std::min)(static_cast<std::size_t>(6), 信息->因果实例序列.size());
            for (std::size_t j = 0; j < 链内展示数; ++j) {
                auto* 实例 = 信息->因果实例序列[j];
                auto* 实例信息 = (实例 && 实例->主信息) ? dynamic_cast<因果主信息类*>(实例->主信息) : nullptr;
                if (!实例信息) continue;
                os << "  -> 条件={" << 汇总因果条件_(实例信息)
                   << "} | 动作={" << 汇总因动作详情_(实例信息)
                   << "} | 结果={" << 汇总因果结果_(实例信息) << "}\n";
            }
            if (信息->因果实例序列.size() > 链内展示数) {
                os << "  其余 " << (信息->因果实例序列.size() - 链内展示数) << " 跳已省略\n";
            }
        }
        if (因果链列表.size() > 实际展示数) {
            os << "其余 " << (因果链列表.size() - 实际展示数) << " 条因果链已省略。\n";
        }
    }

    return os.str();
}

std::size_t 计数同层节点_(const 基础信息节点类* 头节点)
{
    if (!头节点) return 0;
    std::size_t 数量 = 0;
    auto* 当前 = 头节点;
    do {
        ++数量;
        当前 = 当前->下;
    } while (当前 && 当前 != 头节点);
    return 数量;
}

I64 当前UTC毫秒_()
{
    return static_cast<I64>(std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count());
}

bool 尝试取当前自我特征I64_(
    const 词性节点类* 特征类型,
    I64* 输出值)
{
    if (!输出值 || !特征类型) return false;
    auto* 自我存在 = 世界树.自我指针;
    if (!自我存在) return false;

    const auto 快照 = 世界树.读取特征快照(
        自我存在,
        特征类型,
        "鱼巢Headless::尝试取当前自我特征I64");
    if (!快照.has_value()) return false;

    if (const auto* 值 = std::get_if<I64>(&*快照)) {
        *输出值 = *值;
        return true;
    }
    return false;
}

std::string 取状态值文本_(const 特征快照值& 值)
{
    if (const auto* p = std::get_if<I64>(&值)) return std::to_string(*p);
    if (const auto* p = std::get_if<指针句柄>(&值)) {
        return p->有效() ? ("ptr:" + std::to_string(p->指针)) : "ptr:0";
    }
    if (const auto* p = std::get_if<VecU句柄>(&值)) {
        return p->有效() ? ("vec:" + std::to_string(p->主信息指针)) : "vec:0";
    }
    return "<none>";
}

std::string 取状态摘要_(const 状态节点类* 状态)
{
    auto* 状态信息 = (状态 && 状态->主信息) ? dynamic_cast<状态节点主信息类*>(状态->主信息) : nullptr;
    if (!状态信息) return "<invalid>";
    return 生成状态标准描述_(状态)
        + "@t" + std::to_string(状态信息->发生时间);
}

std::string 取动态摘要_(const 动态节点类* 动态)
{
    auto* 动态信息 = (动态 && 动态->主信息) ? dynamic_cast<动态节点主信息类*>(动态->主信息) : nullptr;
    if (!动态信息) return "<invalid>";

    const auto 节点摘要 = [](const 基础信息节点类* 节点) -> std::string {
        if (!节点) return "<空>";
        const auto 主键 = 节点->获取主键();
        return 主键.empty() ? "<无主键>" : 主键;
        };

    std::string 动作名 = 取动态来源动作显示文本_(动态);
    if (动作名.empty()) {
        动作名 = 取词文本_(取动态特征类型_(动态));
    }

    std::string 摘要 = 动作名
        + "@[" + std::to_string(动态信息->开始时间)
        + "->" + std::to_string(动态信息->结束时间) + "]";
    if (动态信息->来源类型 == 枚举_动态来源类型::方法调用) {
        摘要 += "{in=" + 节点摘要(动态信息->来源输入场景)
            + ", out=" + 节点摘要(动态信息->来源输出场景);
        if (动态信息->来源动作相位 != 枚举_动作事件相位::开始运行) {
            摘要 += ", result=" + std::string(动态信息->来源执行成功 ? "ok" : "fail");
        }
        if (!动态信息->来源执行成功) {
            摘要 += ":" + std::to_string(动态信息->来源错误码);
        }
        摘要 += "}";
    }
    return 摘要;
}

std::string 取运行时动态摘要_(const 运行时动态主信息记录* 动态)
{
    if (!动态) return "<invalid>";

    const auto 节点摘要 = [](const 基础信息节点类* 节点) -> std::string {
        if (!节点) return "<空>";
        const auto 主键 = 节点->获取主键();
        return 主键.empty() ? "<无主键>" : 主键;
    };
    const auto 词摘要 = [](const 词性节点类* 词) -> std::string {
        if (!词) return {};
        auto 文本 = 取词文本_(词);
        if (!文本.empty()) return 文本;
        return 词->获取主键();
    };
    const auto 特征类型摘要 = [&](const 特征节点类* 特征) -> std::string {
        auto* 特征信息 = (特征 && 特征->主信息)
            ? dynamic_cast<特征节点主信息类*>(特征->主信息)
            : nullptr;
        return 特征信息 ? 词摘要(特征信息->类型) : std::string{};
    };

    std::string 动作名 = 词摘要(动态->来源动作名);
    if (动作名.empty()) {
        动作名 = 特征类型摘要(动态->动态特征);
    }
    if (动作名.empty()) 动作名 = "<运行时动态>";

    std::string 摘要 = 动作名
        + "@[" + std::to_string(动态->开始时间)
        + "->" + std::to_string(动态->结束时间) + "]";
    if (动态->来源类型 == 枚举_动态来源类型::方法调用) {
        摘要 += "{in=" + 节点摘要(动态->来源输入场景)
            + ", out=" + 节点摘要(动态->来源输出场景);
        if (动态->来源动作相位 != 枚举_动作事件相位::开始运行) {
            摘要 += ", result=" + std::string(动态->来源执行成功 ? "ok" : "fail");
        }
        if (!动态->来源执行成功) {
            摘要 += ":" + std::to_string(动态->来源错误码);
        }
        摘要 += "}";
    }
    return 摘要;
}

std::vector<状态节点类*> 收集指定特征状态_(
    const std::vector<状态节点类*>& 状态列表,
    const 词性节点类* 特征类型)
{
    std::vector<状态节点类*> out{};
    for (auto* 状态 : 状态列表) {
        if (取状态特征类型_(状态) == 特征类型) out.push_back(状态);
    }
    return out;
}

void 短暂等待状态链路_()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
}

DWORD 运行安全服务因果回归测试_()
{
    初始化日志_();
    日志::运行("[HY-Ego Headless] 开始运行安全服务因果回归测试");

    自我类 自我{};
    自我.初始化自我("鱼巢Headless::运行安全服务因果回归测试");
    自我本能方法类_最小试探模块{}.注册默认本能动作_自我最小试探();

    auto* 自我存在 = 世界树.自我指针;
    if (!自我存在) {
        std::cout << "[HY-Ego Headless] 安全服务因果回归测试: 失败\n"
            << "  错误摘要: 自我存在未初始化\n";
        日志::运行_错误("[HY-Ego Headless] 安全服务因果回归测试失败: 自我存在未初始化");
        日志::关闭();
        return 1;
    }

    const auto* 安全特征类型 = 特征类型定义类::类型_自我_安全值;
    const auto* 服务特征类型 = 特征类型定义类::类型_自我_服务值;
    const auto* 待完成任务数特征 = 语素集.添加词性词("待完成任务数", "名词");
    const auto* 待学习任务数特征 = 语素集.添加词性词("待学习任务数", "名词");

    auto 查询因果 = [&]() {
        return 因果集.按动作主体查询因果(自我存在, 256);
    };

    auto 创建回归场景 = [&](const std::string& 名称, const std::string& 类型) -> 场景节点类* {
        auto* 父场景 = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
        if (!父场景) return nullptr;

        auto* mi = new 场景节点主信息类();
        mi->名称 = 语素集.添加词性词(名称, "名词");
        mi->类型 = 语素集.添加词性词(类型, "名词");
        mi->最后观测时间 = 结构体_时间戳::当前_微秒();
        return 世界树.创建场景(
            父场景,
            mi,
            "鱼巢Headless::运行安全服务因果回归测试/创建场景/" + 名称);
    };

    auto 调用显式物理本能 = [&](枚举_本能动作ID 本能ID,
                            const std::string& 标签,
                            I64 待完成任务数 = 0,
                            I64 待学习任务数 = 0,
                            std::optional<I64> 调整量 = std::nullopt) -> bool {
        日志::运行("[HY-Ego Headless] 显式物理本能调用开始: " + 标签);
        const auto now = 结构体_时间戳::当前_微秒();
        auto* 输入场景 = 创建回归场景(
            "回归_" + 标签 + "_输入_" + std::to_string(now),
            "安全服务因果回归输入场景");
        auto* 输出场景 = 创建回归场景(
            "回归_" + 标签 + "_输出_" + std::to_string(now),
            "安全服务因果回归输出场景");
        if (!输入场景 || !输出场景) {
            std::cout << "  [" << 标签 << "] 显式物理本能调用失败：输入或输出场景创建失败\n";
            日志::运行_错误("[HY-Ego Headless] 显式物理本能调用失败: " + 标签 + ", 输入或输出场景创建失败");
            return false;
        }

        if (待完成任务数特征) {
            (void)世界树.写入特征_I64(
                输入场景,
                待完成任务数特征,
                待完成任务数,
                {},
                "鱼巢Headless::运行安全服务因果回归测试/写待完成任务数");
        }
        if (待学习任务数特征) {
            (void)世界树.写入特征_I64(
                输入场景,
                待学习任务数特征,
                待学习任务数,
                {},
                "鱼巢Headless::运行安全服务因果回归测试/写待学习任务数");
        }
        if (调整量.has_value()) {
            const auto* 调整量特征 = 语素集.添加词性词("调整量", "名词");
            if (调整量特征) {
                (void)世界树.写入特征_I64(
                    输入场景,
                    调整量特征,
                    *调整量,
                    {},
                    "鱼巢Headless::运行安全服务因果回归测试/写调整量");
            }
        }

        const auto 调用结果 = 方法集.调用本能函数(本能ID, 输入场景, 输出场景);
        日志::运行f(
            "[HY-Ego Headless] 显式物理本能调用完成: label={}, ok={}, code={}",
            标签,
            调用结果.成功,
            调用结果.成功码);
        if (!调用结果.成功) {
            std::cout << "  [" << 标签 << "] 显式物理本能执行失败"
                << ", 错误码=" << 调用结果.成功码
                << "\n";
            日志::运行_错误(
                "[HY-Ego Headless] 显式物理本能执行失败: "
                + 标签
                + ", 错误码="
                + std::to_string(调用结果.成功码));
        }
        return 调用结果.成功;
    };

    auto 打印快照 = [&](const char* 标签) {
        日志::运行(std::string("[HY-Ego Headless] 打印快照开始: ") + 标签);
        const auto 列表 = 查询因果();
        auto* 内部世界 = 世界树.虚拟世界;
        auto* 场景信息 = (内部世界 && 内部世界->主信息) ? dynamic_cast<场景节点主信息类*>(内部世界->主信息) : nullptr;
        const auto 状态数 = 场景信息 ? 场景信息->状态总数() : 0;
        const auto 动态数 = 场景信息 ? 场景信息->动态总数() : 0;
        const auto 事件动态数 = 场景信息 ? 场景信息->事件动态总数() : 0;
        const auto 实例因果数 = 收集因果节点列表_().size();
        const auto 抽象因果数 = 收集抽象因果节点列表_().size();
        const auto 因果链数 = 收集因果链节点列表_().size();
        std::cout << "  [" << 标签 << "] 实例因果数=" << 实例因果数
            << ", 场景状态数=" << 状态数
            << ", 场景动态数=" << 动态数
            << ", 事件动态数=" << 事件动态数
            << ", 抽象因果数=" << 抽象因果数
            << ", 因果链数=" << 因果链数
            << "\n";
        if (场景信息 && !场景信息->动态列表.empty()) {
            const auto 起始 = 场景信息->动态列表.size() > 3 ? 场景信息->动态列表.size() - 3 : 0;
            for (std::size_t i = 起始; i < 场景信息->动态列表.size(); ++i) {
                std::cout << "    动态[" << i << "] " << 取动态摘要_(场景信息->动态列表[i]) << "\n";
            }
        }
        if (场景信息 && !场景信息->运行时动态列表.empty()) {
            const auto 起始 = 场景信息->运行时动态列表.size() > 3 ? 场景信息->运行时动态列表.size() - 3 : 0;
            for (std::size_t i = 起始; i < 场景信息->运行时动态列表.size(); ++i) {
                std::cout << "    运行时动态[" << i << "] " << 取运行时动态摘要_(&场景信息->运行时动态列表[i]) << "\n";
            }
        }
        if (场景信息 && !场景信息->状态列表.empty()) {
            const auto 起始 = 场景信息->状态列表.size() > 3 ? 场景信息->状态列表.size() - 3 : 0;
            for (std::size_t i = 起始; i < 场景信息->状态列表.size(); ++i) {
                std::cout << "    状态[" << i << "] " << 取状态摘要_(场景信息->状态列表[i]) << "\n";
            }
        }
        日志::运行f(
            "[HY-Ego Headless] 打印快照完成: label={}, causality={}, state={}, dynamic={}, event_dynamic={}",
            标签,
            列表.size(),
            状态数,
            动态数,
            事件动态数);
        return 列表;
    };

    const auto 初始因果 = 打印快照("初始化后");
    const auto 安全上升后 = 初始因果;
    const auto 安全下降后 = 初始因果;
    const auto 服务上升后 = 初始因果;

    短暂等待状态链路_();
    const bool 定时衰减成功 = 调用显式物理本能(
        枚举_本能动作ID::自我_定时衰减服务值,
        "定时衰减服务值");
    const auto 服务下降后 = 打印快照("服务值减少后");

    const auto 周期维护后 = 打印快照("周期维护后");

    auto* 内部世界 = 世界树.虚拟世界;
    auto* 场景信息 = (内部世界 && 内部世界->主信息) ? dynamic_cast<场景节点主信息类*>(内部世界->主信息) : nullptr;
    std::size_t 手工补提后因果数 = 周期维护后.size();
    bool 手工安全序列成功 = false;
    bool 手工服务序列成功 = false;

    if (内部世界 && 场景信息) {
        const auto 安全状态列表 = 收集指定特征状态_(场景信息->状态列表, 安全特征类型);
        const auto 服务状态列表 = 收集指定特征状态_(场景信息->状态列表, 服务特征类型);

        auto 手工提炼 = [&](const char* 标签, const std::vector<状态节点类*>& 状态序列) {
            if (状态序列.size() < 3) {
                std::cout << "  [手工" << 标签 << "序列] 状态不足，无法直接提炼因果\n";
                return false;
            }

            auto* d0 = 动态集.创建叶子动态主信息(内部世界, 状态序列[0], 状态序列[1], 结构体_时间戳::当前_微秒());
            auto* d1 = 动态集.创建叶子动态主信息(内部世界, 状态序列[1], 状态序列[2], 结构体_时间戳::当前_微秒());
            if (!d0 || !d1) {
                std::cout << "  [手工" << 标签 << "序列] 叶子动态创建失败\n";
                return false;
            }

            std::cout << "  [手工" << 标签 << "序列] "
                << "动态1=" << 取运行时动态摘要_(d0)
                << ", 动态2=" << 取运行时动态摘要_(d1)
                << ", 因果=已禁用"
                << "\n";
            return false;
        };

        手工安全序列成功 = 手工提炼("安全", 安全状态列表);
        手工服务序列成功 = 手工提炼("服务", 服务状态列表);
        const auto 手工补提后列表 = 查询因果();
        const auto 手工判定结果 = 构建因果状态判定结果_(手工补提后列表);
        手工补提后因果数 = 手工补提后列表.size();
        std::cout << "  [手工补提后] 因果节点数=" << 手工补提后因果数 << "\n";
        const auto 展示上限 = (std::min)(std::size_t{ 4 }, 手工补提后列表.size());
        for (std::size_t i = 0; i < 展示上限; ++i) {
            auto* 节点 = 手工补提后列表[i];
            auto* 信息 = (节点 && 节点->主信息) ? dynamic_cast<因果主信息类*>(节点->主信息) : nullptr;
            if (!信息) continue;
            std::cout
                << "    手工因果[" << i << "] 状态=" << 查询因果状态标签_(节点, &手工判定结果)
                << ", 观察次数=" << 信息->观察次数
                << ", 稳定度Q=" << 信息->稳定度Q
                << ", 条件={" << 汇总因果条件_(信息) << "}"
                << ", 过程={" << 汇总动态特征_(信息->过程动态列表) << "}"
                << ", 结果={" << 汇总因果结果_(信息) << "}"
                << ", 说明=" << 翻译因果语句_(信息)
                << "\n";
            日志::运行("[HY-Ego Headless] 手工因果语句[" + 查询因果状态标签_(节点, &手工判定结果) + "]: " + 翻译因果语句_(信息));
        }
    }

    std::size_t 条件含安全 = 0;
    std::size_t 条件含服务 = 0;
    std::size_t 过程含安全 = 0;
    std::size_t 过程含服务 = 0;
    std::size_t 结果含安全 = 0;
    std::size_t 结果含服务 = 0;

    for (auto* 节点 : 周期维护后) {
        auto* 信息 = (节点 && 节点->主信息) ? dynamic_cast<因果主信息类*>(节点->主信息) : nullptr;
        if (!信息) continue;

        if (状态列表含特征_(信息->条件状态列表, 安全特征类型)) ++条件含安全;
        if (状态列表含特征_(信息->条件状态列表, 服务特征类型)) ++条件含服务;
        if (动态列表含特征_(信息->过程动态列表, 安全特征类型)) ++过程含安全;
        if (动态列表含特征_(信息->过程动态列表, 服务特征类型)) ++过程含服务;
        if (状态含特征_(取因果主结果状态_(信息), 安全特征类型)) ++结果含安全;
        if (状态含特征_(取因果主结果状态_(信息), 服务特征类型)) ++结果含服务;
    }

    std::cout
        << "  条件含安全=" << 条件含安全
        << ", 条件含服务=" << 条件含服务
        << ", 过程含安全=" << 过程含安全
        << ", 过程含服务=" << 过程含服务
        << ", 结果含安全=" << 结果含安全
        << ", 结果含服务=" << 结果含服务
        << "\n";

    const std::size_t 展示上限 = (std::min)(std::size_t{ 6 }, 周期维护后.size());
    const auto 自动判定结果 = 构建因果状态判定结果_(周期维护后);
    for (std::size_t i = 0; i < 展示上限; ++i) {
        auto* 节点 = 周期维护后[i];
        auto* 信息 = (节点 && 节点->主信息) ? dynamic_cast<因果主信息类*>(节点->主信息) : nullptr;
        if (!信息) continue;

        std::cout
            << "  因果[" << i << "] 状态=" << 查询因果状态标签_(节点, &自动判定结果)
            << ", 观察次数=" << 信息->观察次数
            << ", 稳定度Q=" << 信息->稳定度Q
            << ", 条件={" << 汇总因果条件_(信息) << "}"
            << ", 过程={" << 汇总动态特征_(信息->过程动态列表) << "}"
            << ", 结果={" << 汇总因果结果_(信息) << "}"
            << ", 说明=" << 翻译因果语句_(信息)
            << "\n";
        日志::运行("[HY-Ego Headless] 因果语句[" + 查询因果状态标签_(节点, &自动判定结果) + "]: " + 翻译因果语句_(信息));
    }

    const bool 因果节点有增长 = 周期维护后.size() > 初始因果.size();
    const bool 含安全服务因果 =
        (条件含安全 + 条件含服务 + 过程含安全 + 过程含服务 + 结果含安全 + 结果含服务) > 0;
    const bool 显式物理本能成功 =
        定时衰减成功;

    const bool 自动链成功 = 显式物理本能成功 && 因果节点有增长 && 含安全服务因果;
    const bool 成功 = 自动链成功 || 手工安全序列成功 || 手工服务序列成功;

    std::cout << "[HY-Ego Headless] 安全服务因果回归测试: "
        << (成功 ? "成功" : "失败") << "\n";
    if (!成功) {
        std::cout
            << "  错误摘要: 因果节点未按预期增长，或未观察到安全/服务相关因果\n"
            << "  基线=" << 初始因果.size()
            << ", 安全增后=" << 安全上升后.size()
            << ", 安全减后=" << 安全下降后.size()
            << ", 服务增后=" << 服务上升后.size()
            << ", 服务减后=" << 服务下降后.size()
            << ", 周期维护后=" << 周期维护后.size()
            << ", 手工补提后=" << 手工补提后因果数
            << ", 显式物理本能=" << (显式物理本能成功 ? "成功" : "失败")
            << "\n";
        日志::运行_错误("[HY-Ego Headless] 安全服务因果回归测试失败");
    }
    else {
        std::cout
            << "  自动链路=" << (自动链成功 ? "已生成因果" : "未生成因果")
            << ", 手工安全序列=" << (手工安全序列成功 ? "已生成因果" : "未生成因果")
            << ", 手工服务序列=" << (手工服务序列成功 ? "已生成因果" : "未生成因果")
            << "\n";
        日志::运行f(
            "[HY-Ego Headless] 安全服务因果回归测试通过: baseline={}, safety_up={}, safety_down={}, service_up={}, service_down={}, final={}, manual_final={}, explicit_ok={}, auto_ok={}, manual_safety_ok={}, manual_service_ok={}, cond_s={}, cond_v={}, proc_s={}, proc_v={}, result_s={}, result_v={}",
            初始因果.size(),
            安全上升后.size(),
            安全下降后.size(),
            服务上升后.size(),
            服务下降后.size(),
            周期维护后.size(),
            手工补提后因果数,
            显式物理本能成功,
            自动链成功,
            手工安全序列成功,
            手工服务序列成功,
            条件含安全,
            条件含服务,
            过程含安全,
            过程含服务,
            结果含安全,
            结果含服务);
    }

    日志::关闭();
    return 成功 ? 0 : 1;
}

DWORD 运行安全服务物理长时回归测试_()
{
    初始化日志_();
    日志::运行("[HY-Ego Headless] 开始运行安全服务物理长时回归测试");

    struct 结构_回中位结果 {
        std::size_t 步数 = 0;
        I64 初始安全值 = 0;
        I64 最终安全值 = 0;
        I64 服务值 = 0;
        I64 中位值 = 0;
        I64 正向步长 = 0;
        I64 反向步长 = 0;
        bool 已到达中位 = false;
    };

    auto 取自我时序动作词 = [](const 自我类& 自我) -> const 词性节点类* {
        (void)自我;
        return 语素集.添加词性词(
            "服务归零待机保护",
            "动词");
    };

    auto 执行自我时序动作 = [&](
        自我类& 自我,
        I64 待完成任务数,
        I64 待学习任务数,
        时间戳 now,
        const std::string& 调用点) -> bool
    {
        const auto* 动作词 = 取自我时序动作词(自我);
        (void)自我.记录自我动作动态(
            动作词,
            nullptr,
            nullptr,
            true,
            0,
            now,
            now,
            枚举_动作事件相位::开始运行,
            调用点 + "/开始");
        const I64 旧安全值 = 自我.获取安全值();
        const I64 旧服务值 = 自我.获取服务值();
        const bool 旧待机状态 = 自我.是否待机状态();
        状态节点类* 主结果状态 = nullptr;
        自我.按时序规则更新安全值(
            待完成任务数,
            待学习任务数,
            now,
            调用点,
            &主结果状态);
        const bool 成功 =
            旧安全值 != 自我.获取安全值()
            || 旧服务值 != 自我.获取服务值()
            || 旧待机状态 != 自我.是否待机状态();
        const 时间戳 结束时间 = 结构体_时间戳::当前_微秒();
        (void)自我.记录自我动作动态(
            动作词,
            nullptr,
            nullptr,
            成功,
            成功 ? 0 : -1,
            结束时间,
            结束时间,
            成功 ? 枚举_动作事件相位::完成 : 枚举_动作事件相位::失败,
            调用点 + "/结束",
            nullptr,
            主结果状态);
        return 成功;
    };

    自我类 基准自我{};
    基准自我.初始化自我("鱼巢Headless::运行安全服务物理长时回归测试/基准");
    const I64 初始安全值 = 基准自我.获取安全值();
    const I64 初始服务值 = 基准自我.获取服务值();
    const I64 初始正向步长 = 基准自我.时序正向步长();
    const I64 初始反向步长 = 基准自我.时序反向步长();

    const bool 定时衰减成功 = 基准自我.应用定时衰减服务值(
        1,
        结构体_时间戳::当前_微秒(),
        "鱼巢Headless::运行安全服务物理长时回归测试/基准/定时衰减");
    const I64 衰减后服务值 = 基准自我.获取服务值();

    const bool 安全时序成功 = 执行自我时序动作(
        基准自我,
        3,
        1,
        结构体_时间戳::当前_微秒(),
        "鱼巢Headless::运行安全服务物理长时回归测试/基准/安全时序");
    const I64 时序后安全值 = 基准自我.获取安全值();

    自我类::结构_自我配置 无需求配置{};
    无需求配置.工作者数量 = 0;
    无需求配置.Tick间隔_微秒 = 1000;
    无需求配置.调试_最大循环次数 = 256;
    无需求配置.退出事件文件 = "self_exit_event.physics.txt";
    自我类 无需求自测{ 无需求配置 };
    无需求自测.初始化自我("鱼巢Headless::运行安全服务物理长时回归测试/无需求");
    无需求自测.根任务权重().学习 = 0;
    const I64 无需求初始服务 = 无需求自测.获取服务值();
    const I64 无需求初始安全 = 无需求自测.获取安全值();
    无需求自测.启动();
    无需求自测.等待停止();
    const I64 无需求结束服务 = 无需求自测.获取服务值();
    const I64 无需求结束安全 = 无需求自测.获取安全值();
    const bool 规则_无需求服务持续下降 = 无需求结束服务 < 无需求初始服务;
    const bool 规则_无需求阶段安全未自动回中 =
        无需求结束安全 == 无需求初始安全;

    const bool 规则_默认步长为1 =
        初始正向步长 == 1
        && 初始反向步长 == 1;
    const bool 规则_显式衰减生效 =
        定时衰减成功
        && 衰减后服务值 < 初始服务值;
    const bool 规则_安全时序生效 =
        安全时序成功
        && 时序后安全值 != 初始安全值;

    const bool 全部达成 =
        规则_无需求服务持续下降
        && 规则_默认步长为1
        && 规则_显式衰减生效
        && 规则_安全时序生效;

    std::cout
        << "[HY-Ego Headless] 安全服务物理长时回归测试: "
        << (全部达成 ? "成功" : "未完全达成") << "\n"
        << "  [基准] 初始安全=" << 初始安全值
        << ", 初始服务=" << 初始服务值
        << ", 初始正向步长=" << 初始正向步长
        << ", 初始反向步长=" << 初始反向步长 << "\n"
        << "  [显式衰减] 衰减后服务=" << 衰减后服务值
        << ", 生效=" << (规则_显式衰减生效 ? "是" : "否") << "\n"
        << "  [安全时序] 时序后安全=" << 时序后安全值
        << ", 生效=" << (规则_安全时序生效 ? "是" : "否") << "\n"
        << "  [无需求256Tick] 服务变化=" << (无需求结束服务 - 无需求初始服务)
        << ", 安全变化=" << (无需求结束安全 - 无需求初始安全) << "\n"
        << "  规则1_默认步长为1=" << (规则_默认步长为1 ? "是" : "否")
        << ", 规则2_显式衰减生效=" << (规则_显式衰减生效 ? "是" : "否") << "\n"
        << "  规则3_安全时序生效=" << (规则_安全时序生效 ? "是" : "否")
        << ", 规则4_无需求服务持续下降=" << (规则_无需求服务持续下降 ? "是" : "否") << "\n"
        << "  注: 无需求阶段安全变化=" << (规则_无需求阶段安全未自动回中 ? "未发生" : "发生了") << "\n";

    if (全部达成) {
        日志::运行("[HY-Ego Headless] 安全服务物理长时回归测试通过");
    }
    else {
        日志::运行_错误("[HY-Ego Headless] 安全服务物理长时回归测试未完全达成");
    }

    日志::关闭();
    return 全部达成 ? 0 : 1;
}

void 填充SQL快照测试样本_(结构_后台前端快照* 快照) {
    if (!快照) return;

    快照->后台在线 = true;
    快照->宿主运行中 = true;
    快照->请求停止 = false;
    快照->自我线程运行中 = true;
    快照->初始化完成 = true;
    快照->健康运行 = true;
    快照->请求安全重启 = false;
    快照->致命错误计数 = 0;
    快照->已执行重启次数 = 2;
    快照->相机已启用 = false;
    快照->相机运行中 = false;
    快照->相机使用虚拟回退 = false;
    快照->相机已处理帧数 = 0;
    快照->最近桥接候选数 = 0;
    快照->世界根子节点数 = 9;
    快照->需求数 = 4;
    快照->任务数 = 6;
    快照->安全任务数 = 2;
    快照->服务任务数 = 2;
    快照->叶子任务数 = 3;
    快照->方法数 = 5;
    快照->状态数 = 13;
    快照->动态数 = 4;
    快照->事件动态数 = 8;
    快照->因果数 = 0;
    快照->有当前安全值 = true;
    快照->当前安全值 = 9223372036854775LL;
    快照->有当前服务值 = true;
    快照->当前服务值 = 9223372036854000LL;
    快照->快照生成UTC毫秒 = 当前UTC毫秒_();
    快照->最近输入序号 = 42;
    快照->主要需求权重 = 88;
    快照->主要需求已满足 = false;
    快照->主要需求类型 = "服务";
    快照->最近输入文本 = "sql snapshot synthetic sample";
    快照->最近命令摘要 = "sql_snapshot_test";
    快照->最近故障摘要.clear();
    快照->宿主故障摘要.clear();
    快照->相机状态摘要 = "not_enabled";
}

template<class TNode, class Fn>
void 枚举根子节点_(TNode* 根, Fn&& fn) {
    if (!根) return;

    std::unordered_set<TNode*> 已访问{};
    已访问.reserve(256);
    const auto 安全遍历同层 = [&](TNode* 起点, TNode* 根哨兵 = nullptr) {
        auto* 当前 = 起点;
        while (当前 && 当前 != 根哨兵) {
            if (!已访问.insert(当前).second) break;
            if (已访问.size() > 快照枚举最大节点数) break;
            fn(当前);
            当前 = static_cast<TNode*>(当前->下);
        }
    };

    if (根->子) {
        安全遍历同层(static_cast<TNode*>(根->子));
        return;
    }

    // 顶层需求链/任务链的根节点使用 根->下 维护同层环，而不是挂在 根->子 上。
    安全遍历同层(static_cast<TNode*>(根->下), 根);
}

std::string 词键_(const 词性节点类* 词) {
    return 词 ? 词->获取主键() : std::string{};
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
    std::unordered_set<任务节点类*> 已访问节点{};
    已访问节点.reserve(512);

    auto 递归枚举子树 = [&](auto&& self, 任务节点类* 父节点) -> void {
        auto* 首子节点 = 父节点 ? static_cast<任务节点类*>(父节点->子) : nullptr;
        if (!首子节点) return;
        auto* 当前 = 首子节点;
        std::unordered_set<任务节点类*> 已访问同层{};
        已访问同层.reserve(64);
        while (当前) {
            if (!已访问同层.insert(当前).second) break;
            if (!已访问节点.insert(当前).second) break;
            if (已访问节点.size() > 快照枚举最大节点数) break;
            if (auto* 头信息 = (当前 && 当前->主信息) ? dynamic_cast<任务头结点信息*>(当前->主信息) : nullptr) {
                fn(当前, 头信息);
            }
            self(self, 当前);
            当前 = static_cast<任务节点类*>(当前->下);
        }
    };

    枚举根子节点_(static_cast<任务节点类*>(数据仓库模块::任务链.根指针), [&](任务节点类* 顶层节点) {
        if (!已访问节点.insert(顶层节点).second) return;
        if (已访问节点.size() > 快照枚举最大节点数) return;
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
    const 结构_前端命令共享状态* 前端命令状态 = nullptr,
    bool 仅快速状态 = false) {
    static 结构_完整快照生成诊断状态 完整快照诊断{};
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

    if (仅快速状态) {
        return 结果;
    }

    const bool 输出诊断 = 完整快照诊断.应记录();
    const auto 统计开始时间 = std::chrono::steady_clock::now();
    if (输出诊断) {
        日志::运行f(
            "[HY-Ego Headless][完整快照诊断] 开始生成: host={}, self={}, initialized={}, healthy={}",
            宿主快照.宿主运行中,
            宿主快照.自我线程运行中,
            宿主快照.初始化完成,
            宿主快照.健康运行);
    }
    枚举根子节点_(static_cast<基础信息节点类*>(数据仓库模块::世界链.根指针), [&](基础信息节点类*) {
        ++结果.世界根子节点数;
    });
    if (输出诊断) {
        日志::运行f("[HY-Ego Headless][完整快照诊断] 世界节点统计完成: world={}", static_cast<unsigned long long>(结果.世界根子节点数));
    }
    枚举根子节点_(static_cast<需求节点类*>(数据仓库模块::需求链.根指针), [&](需求节点类* n) {
        ++结果.需求数;
        auto* mi = n ? n->主信息 : nullptr;
        if (!mi) return;
        if (结果.需求数 == 1 || static_cast<std::int64_t>(mi->权重) > 结果.主要需求权重) {
            结果.主要需求权重 = static_cast<std::int64_t>(mi->权重);
            结果.主要需求类型 = 需求类型显示文本_(mi->类型);
        }
    });
    if (输出诊断) {
        日志::运行f(
            "[HY-Ego Headless][完整快照诊断] 需求统计完成: need={}, top_need={}, top_weight={}",
            static_cast<unsigned long long>(结果.需求数),
            结果.主要需求类型.empty() ? std::string("<none>") : 结果.主要需求类型,
            static_cast<long long>(结果.主要需求权重));
    }
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
    if (输出诊断) {
        日志::运行f(
            "[HY-Ego Headless][完整快照诊断] 任务统计完成: task={}, leaf={}, safety={}, service={}",
            static_cast<unsigned long long>(结果.任务数),
            static_cast<unsigned long long>(结果.叶子任务数),
            static_cast<unsigned long long>(结果.安全任务数),
            static_cast<unsigned long long>(结果.服务任务数));
    }
    枚举根子节点_(static_cast<方法节点类*>(数据仓库模块::方法链.根指针), [&](方法节点类*) {
        ++结果.方法数;
    });
    if (输出诊断) {
        日志::运行f("[HY-Ego Headless][完整快照诊断] 方法统计完成: method={}", static_cast<unsigned long long>(结果.方法数));
    }

    auto* 内部世界 = 世界树.取内部世界() ? 世界树.取内部世界() : 世界树.虚拟世界;
    auto* 内部场景信息 = (内部世界 && 内部世界->主信息) ? dynamic_cast<场景节点主信息类*>(内部世界->主信息) : nullptr;
    if (内部场景信息) {
        if (输出诊断) {
            日志::运行("[HY-Ego Headless][完整快照诊断] 场景清理开始");
        }
        内部场景信息->清理空指针();
        if (输出诊断) {
            日志::运行("[HY-Ego Headless][完整快照诊断] 场景清理完成");
        }
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
        收集因果节点列表_().size()
        + 收集抽象因果节点列表_().size()
        + 收集因果链节点列表_().size());
    结果.因果链摘要 = 生成因果链详细摘要_(24);
    if (输出诊断) {
        日志::运行f(
            "[HY-Ego Headless][完整快照诊断] 场景统计完成: state={}, dynamic={}, event_dynamic={}, causality={}",
            static_cast<unsigned long long>(结果.状态数),
            static_cast<unsigned long long>(结果.动态数),
            static_cast<unsigned long long>(结果.事件动态数),
            static_cast<unsigned long long>(结果.因果数));
    }

    if (结果.需求数 > 0) {
        auto* 最佳需求 = [&]() -> 需求节点类* {
            需求节点类* 最佳 = nullptr;
            I64 最佳权重 = std::numeric_limits<I64>::min();
            枚举根子节点_(static_cast<需求节点类*>(数据仓库模块::需求链.根指针), [&](需求节点类* n) {
                auto* mi = n ? n->主信息 : nullptr;
                if (!mi) return;
                const auto 当前权重 = static_cast<I64>(mi->权重);
                if (!最佳 || 当前权重 > 最佳权重) {
                    最佳 = n;
                    最佳权重 = 当前权重;
                }
            });
            return 最佳;
        }();
        if (最佳需求) {
            枚举_需求运行状态 缓存状态 = 枚举_需求运行状态::未知;
            if (输出诊断) {
                日志::运行("[HY-Ego Headless][完整快照诊断] 主要需求状态缓存查询开始");
            }
            if (需求状态管理器.尝试查询缓存需求状态(最佳需求, &缓存状态)) {
                结果.主要需求已满足 = (缓存状态 == 枚举_需求运行状态::已满足);
            }
            if (输出诊断) {
                日志::运行f(
                    "[HY-Ego Headless][完整快照诊断] 主要需求状态缓存查询完成: hit={}, satisfied={}",
                    缓存状态 != 枚举_需求运行状态::未知,
                    结果.主要需求已满足);
            }
        }
    }

    const auto 统计耗时毫秒 = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - 统计开始时间).count();
    if (输出诊断) {
        日志::运行f(
            "[HY-Ego Headless][完整快照诊断] 生成完成: cost={} ms",
            static_cast<long long>(统计耗时毫秒));
    }
    if (统计耗时毫秒 >= 200) {
        日志::运行f(
            "[HY-Ego Headless] 完整快照统计耗时较长: {} ms, world={}, need={}, task={}, method={}, state={}, dynamic={}, event_dynamic={}",
            static_cast<long long>(统计耗时毫秒),
            static_cast<unsigned long long>(结果.世界根子节点数),
            static_cast<unsigned long long>(结果.需求数),
            static_cast<unsigned long long>(结果.任务数),
            static_cast<unsigned long long>(结果.方法数),
            static_cast<unsigned long long>(结果.状态数),
            static_cast<unsigned long long>(结果.动态数),
            static_cast<unsigned long long>(结果.事件动态数));
    }

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

std::string 收集ODBC诊断_(SQLSMALLINT 句柄类型, SQLHANDLE 句柄) {
    if (!句柄) return "无诊断句柄";

    std::string 汇总;
    SQLWCHAR 状态[16]{};
    SQLWCHAR 消息[512]{};
    SQLINTEGER 原生错误 = 0;
    SQLSMALLINT 消息长度 = 0;

    for (SQLSMALLINT 序号 = 1;; ++序号) {
        const auto 结果 = ::SQLGetDiagRecW(
            句柄类型,
            句柄,
            序号,
            状态,
            &原生错误,
            消息,
            static_cast<SQLSMALLINT>(std::size(消息)),
            &消息长度);
        if (结果 == SQL_NO_DATA) break;
        if (!SQL_SUCCEEDED(结果)) break;

        if (!汇总.empty()) 汇总 += " | ";
        汇总 += 转UTF8(std::wstring(reinterpret_cast<const wchar_t*>(状态)));
        汇总 += ":";
        汇总 += std::to_string(static_cast<long long>(原生错误));
        汇总 += ":";
        汇总 += 转UTF8(std::wstring(reinterpret_cast<const wchar_t*>(消息), static_cast<std::size_t>(消息长度)));
    }

    return 汇总.empty() ? "无 ODBC 诊断信息" : 汇总;
}

I64 安全转换到I64_(std::uint64_t 值) {
    constexpr auto 上限 = static_cast<std::uint64_t>(std::numeric_limits<I64>::max());
    return static_cast<I64>(值 > 上限 ? 上限 : 值);
}

std::wstring 裁剪宽字符串_(std::wstring 文本, std::size_t 最大长度 = 0) {
    if (最大长度 > 0 && 文本.size() > 最大长度) {
        文本.resize(最大长度);
    }
    return 文本;
}

std::wstring 转SQL文本字面量_(const std::wstring& 文本, std::size_t 最大长度 = 0) {
    auto 处理后 = 裁剪宽字符串_(文本, 最大长度);
    std::wstring 转义;
    转义.reserve(处理后.size() + 8);
    for (const wchar_t ch : 处理后) {
        if (ch == L'\'') {
            转义 += L"''";
        }
        else {
            转义.push_back(ch);
        }
    }
    return L"N'" + 转义 + L"'";
}

std::wstring 转SQL可空文本字面量_(const std::wstring& 文本, std::size_t 最大长度 = 0) {
    if (文本.empty()) return L"NULL";
    return 转SQL文本字面量_(文本, 最大长度);
}

std::wstring 转SQL可空文本字面量_UTF8_(const std::string& 文本, std::size_t 最大长度 = 0) {
    if (文本.empty()) return L"NULL";
    return 转SQL文本字面量_(从UTF8(文本), 最大长度);
}

std::wstring 取当前主机名_() {
    wchar_t 缓冲[MAX_COMPUTERNAME_LENGTH + 1]{};
    DWORD 长度 = static_cast<DWORD>(std::size(缓冲));
    if (!::GetComputerNameW(缓冲, &长度)) return {};
    return std::wstring(缓冲, 长度);
}

class SQL运行时快照写入器类 {
public:
    SQL运行时快照写入器类(std::wstring 实例, std::wstring 数据库, std::wstring 快照键)
        : 实例_(std::move(实例)),
        数据库_(std::move(数据库)),
        快照键_(裁剪宽字符串_(std::move(快照键), 64)),
        主机名_(裁剪宽字符串_(取当前主机名_(), 128)),
        进程ID_(::GetCurrentProcessId()) {
    }

    ~SQL运行时快照写入器类() {
        释放连接_();
    }

    bool 写入(const 结构_后台前端快照& 快照, std::string_view 调用点) {
        if (!确保连接_(调用点)) return false;

        std::vector<std::wstring> 参数列表{};
        参数列表.reserve(41);

        const auto 追加布尔 = [&](const wchar_t* 名称, bool 值) {
            参数列表.push_back(std::wstring(名称) + L"=" + (值 ? L"1" : L"0"));
        };
        const auto 追加I64 = [&](const wchar_t* 名称, I64 值) {
            参数列表.push_back(std::wstring(名称) + L"=" + std::to_wstring(static_cast<long long>(值)));
        };
        const auto 追加文本 = [&](const wchar_t* 名称, const std::wstring& 值, std::size_t 最大长度 = 0) {
            参数列表.push_back(std::wstring(名称) + L"=" + 转SQL可空文本字面量_(值, 最大长度));
        };
        const auto 追加UTF8文本 = [&](const wchar_t* 名称, const std::string& 值, std::size_t 最大长度 = 0) {
            参数列表.push_back(std::wstring(名称) + L"=" + 转SQL可空文本字面量_UTF8_(值, 最大长度));
        };

        追加文本(L"@snapshot_key", 快照键_, 64);
        参数列表.push_back(L"@captured_at=@__captured_at");
        追加文本(L"@source_host", 主机名_, 128);
        追加I64(L"@source_process_id", static_cast<I64>(进程ID_));
        追加布尔(L"@backend_online", 快照.后台在线);
        追加布尔(L"@host_running", 快照.宿主运行中);
        追加布尔(L"@stop_requested", 快照.请求停止);
        追加布尔(L"@self_running", 快照.自我线程运行中);
        追加布尔(L"@initialized", 快照.初始化完成);
        追加布尔(L"@healthy", 快照.健康运行);
        追加布尔(L"@restart_requested", 快照.请求安全重启);
        追加I64(L"@fatal_count", static_cast<I64>(快照.致命错误计数));
        追加I64(L"@restart_count", static_cast<I64>(快照.已执行重启次数));
        追加布尔(L"@camera_enabled", 快照.相机已启用);
        追加布尔(L"@camera_running", 快照.相机运行中);
        追加布尔(L"@camera_virtual", 快照.相机使用虚拟回退);
        追加I64(L"@camera_frames", 安全转换到I64_(快照.相机已处理帧数));
        追加I64(L"@camera_candidates", 安全转换到I64_(快照.最近桥接候选数));
        追加I64(L"@world_count", 安全转换到I64_(快照.世界根子节点数));
        追加I64(L"@need_count", 安全转换到I64_(快照.需求数));
        追加I64(L"@task_count", 安全转换到I64_(快照.任务数));
        追加I64(L"@safe_task_count", 安全转换到I64_(快照.安全任务数));
        追加I64(L"@service_task_count", 安全转换到I64_(快照.服务任务数));
        追加I64(L"@leaf_task_count", 安全转换到I64_(快照.叶子任务数));
        追加I64(L"@method_count", 安全转换到I64_(快照.方法数));
        追加I64(L"@state_count", 安全转换到I64_(快照.状态数));
        追加I64(L"@dynamic_count", 安全转换到I64_(快照.动态数));
        追加I64(L"@event_dynamic_count", 安全转换到I64_(快照.事件动态数));
        追加I64(L"@causality_count", 安全转换到I64_(快照.因果数));
        追加布尔(L"@has_self_safety", 快照.有当前安全值);
        追加I64(L"@self_safety_value", 快照.当前安全值);
        追加布尔(L"@has_self_service", 快照.有当前服务值);
        追加I64(L"@self_service_value", 快照.当前服务值);
        追加I64(L"@snapshot_utc_ms", 快照.快照生成UTC毫秒);
        追加I64(L"@last_input_seq", 安全转换到I64_(快照.最近输入序号));
        追加I64(L"@top_need_weight", 快照.主要需求权重);
        追加布尔(L"@top_need_satisfied", 快照.主要需求已满足);
        追加UTF8文本(L"@top_need_type", 快照.主要需求类型, 256);
        参数列表.push_back(L"@last_input_text=" + 转SQL可空文本字面量_UTF8_(快照.最近输入文本));
        追加UTF8文本(L"@last_command", 快照.最近命令摘要, 1024);
        追加UTF8文本(L"@recent_fault", 快照.最近故障摘要, 1024);
        追加UTF8文本(L"@host_fault", 快照.宿主故障摘要, 1024);
        追加UTF8文本(L"@camera_status", 快照.相机状态摘要, 1024);

        std::wstring SQL文本 = L"DECLARE @__captured_at DATETIME2(3)=SYSUTCDATETIME(); EXEC dbo.upsert_latest_runtime_snapshot ";
        for (std::size_t i = 0; i < 参数列表.size(); ++i) {
            if (i != 0) SQL文本 += L", ";
            SQL文本 += 参数列表[i];
        }

        SQLHSTMT 语句句柄 = SQL_NULL_HSTMT;
        if (!SQL_SUCCEEDED(::SQLAllocHandle(SQL_HANDLE_STMT, 连接句柄_, &语句句柄))) {
            记录失败_("分配 SQL 语句句柄失败", {});
            return false;
        }

        const auto 执行结果 = ::SQLExecDirectW(
            语句句柄,
            const_cast<SQLWCHAR*>(reinterpret_cast<const SQLWCHAR*>(SQL文本.c_str())),
            SQL_NTS);
        if (!SQL_SUCCEEDED(执行结果)) {
            const auto 诊断 = 收集ODBC诊断_(SQL_HANDLE_STMT, 语句句柄);
            ::SQLFreeHandle(SQL_HANDLE_STMT, 语句句柄);
            释放连接_();
            记录失败_("SQL 快照写入失败", 诊断);
            return false;
        }

        ::SQLFreeHandle(SQL_HANDLE_STMT, 语句句柄);
        记录恢复_(调用点);
        return true;
    }

private:
    bool 确保连接_(std::string_view 调用点) {
        if (连接句柄_ != SQL_NULL_HDBC) return true;

        if (!SQL_SUCCEEDED(::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &环境句柄_))) {
            记录失败_("分配 SQL 环境句柄失败", {});
            return false;
        }
        if (!SQL_SUCCEEDED(::SQLSetEnvAttr(
            环境句柄_,
            SQL_ATTR_ODBC_VERSION,
            reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3),
            0))) {
            记录失败_("设置 ODBC 版本失败", 收集ODBC诊断_(SQL_HANDLE_ENV, 环境句柄_));
            释放连接_();
            return false;
        }
        if (!SQL_SUCCEEDED(::SQLAllocHandle(SQL_HANDLE_DBC, 环境句柄_, &连接句柄_))) {
            记录失败_("分配 SQL 连接句柄失败", 收集ODBC诊断_(SQL_HANDLE_ENV, 环境句柄_));
            释放连接_();
            return false;
        }

        const std::vector<std::wstring> 驱动列表 = {
            L"ODBC Driver 18 for SQL Server",
            L"ODBC Driver 17 for SQL Server",
            L"SQL Server Native Client 11.0",
            L"SQL Server"
        };

        std::string 最后错误;
        for (const auto& 驱动名 : 驱动列表) {
            const std::wstring 连接串 =
                L"Driver={" + 驱动名 + L"};"
                L"Server=" + 实例_ + L";"
                L"Database=" + 数据库_ + L";"
                L"Trusted_Connection=Yes;"
                L"Encrypt=No;"
                L"TrustServerCertificate=Yes;"
                L"APP=FishNestHeadless;";
            const auto 连接结果 = ::SQLDriverConnectW(
                连接句柄_,
                nullptr,
                const_cast<SQLWCHAR*>(reinterpret_cast<const SQLWCHAR*>(连接串.c_str())),
                SQL_NTS,
                nullptr,
                0,
                nullptr,
                SQL_DRIVER_NOPROMPT);
            if (SQL_SUCCEEDED(连接结果)) {
                记录恢复_(调用点);
                return true;
            }
            最后错误 = "[" + 转UTF8(驱动名) + "] " + 收集ODBC诊断_(SQL_HANDLE_DBC, 连接句柄_);
            ::SQLDisconnect(连接句柄_);
        }

        释放连接_();
        记录失败_("连接 SQL Server 失败", 最后错误);
        return false;
    }

    void 释放连接_() {
        if (连接句柄_ != SQL_NULL_HDBC) {
            ::SQLDisconnect(连接句柄_);
            ::SQLFreeHandle(SQL_HANDLE_DBC, 连接句柄_);
            连接句柄_ = SQL_NULL_HDBC;
        }
        if (环境句柄_ != SQL_NULL_HENV) {
            ::SQLFreeHandle(SQL_HANDLE_ENV, 环境句柄_);
            环境句柄_ = SQL_NULL_HENV;
        }
    }

    void 记录失败_(std::string_view 阶段, const std::string& 详情) {
        const auto 当前 = std::chrono::steady_clock::now();
        if (当前 < 下次允许记录失败时间_) return;

        std::string 消息 = "[HY-Ego Headless] ";
        消息 += std::string(阶段);
        消息 += ": instance=" + 转UTF8(实例_);
        消息 += ", database=" + 转UTF8(数据库_);
        消息 += ", snapshot_key=" + 转UTF8(快照键_);
        if (!详情.empty()) {
            消息 += ", detail=" + 详情;
        }
        日志::运行_错误(消息);
        上次写入失败_ = true;
        下次允许记录失败时间_ = 当前 + std::chrono::seconds(30);
    }

    void 记录恢复_(std::string_view 调用点) {
        if (!上次写入失败_) return;

        std::string 消息 = "[HY-Ego Headless] SQL 运行时快照写入已恢复";
        if (!调用点.empty()) {
            消息 += ": ";
            消息 += 调用点;
        }
        消息 += ", instance=" + 转UTF8(实例_);
        消息 += ", database=" + 转UTF8(数据库_);
        消息 += ", snapshot_key=" + 转UTF8(快照键_);
        日志::运行(消息);
        上次写入失败_ = false;
        下次允许记录失败时间_ = std::chrono::steady_clock::time_point{};
    }

    std::wstring 实例_;
    std::wstring 数据库_;
    std::wstring 快照键_;
    std::wstring 主机名_;
    DWORD 进程ID_ = 0;
    SQLHENV 环境句柄_ = SQL_NULL_HENV;
    SQLHDBC 连接句柄_ = SQL_NULL_HDBC;
    bool 上次写入失败_ = false;
    std::chrono::steady_clock::time_point 下次允许记录失败时间_{};
};

DWORD 运行SQL运行时快照测试_(const 结构_启动参数& 参数) {
    初始化日志_();
    日志::运行("[HY-Ego Headless] 开始运行 SQL 运行时快照测试");

    结构_后台前端快照 快照{};
    后台前端客户端类 客户端{};
    const bool 使用实时后台快照 = 客户端.查询快照(&快照, 1200);
    if (!使用实时后台快照) {
        填充SQL快照测试样本_(&快照);
        日志::运行("[HY-Ego Headless] SQL 快照测试未查询到在线后台，改用内置样本快照");
    }

    SQL运行时快照写入器类 写入器(
        参数.SQL实例,
        参数.SQL数据库,
        参数.SQL快照键);
    const bool 写入成功 = 写入器.写入(快照, "sql-snapshot-test");

    std::cout << "[HY-Ego Headless] SQL 运行时快照测试: "
        << (写入成功 ? "成功" : "失败") << "\n"
        << "  数据源=" << (使用实时后台快照 ? "live_backend" : "synthetic")
        << ", instance=" << 转UTF8(参数.SQL实例)
        << ", database=" << 转UTF8(参数.SQL数据库)
        << ", snapshot_key=" << 转UTF8(参数.SQL快照键)
        << ", tasks=" << 快照.任务数
        << ", needs=" << 快照.需求数
        << ", states=" << 快照.状态数
        << ", dynamics=" << 快照.动态数
        << ", event_dynamics=" << 快照.事件动态数
        << ", safety=" << (快照.有当前安全值 ? std::to_string(快照.当前安全值) : std::string("<unknown>"))
        << ", service=" << (快照.有当前服务值 ? std::to_string(快照.当前服务值) : std::string("<unknown>"))
        << "\n";

    if (!写入成功) {
        日志::运行_错误("[HY-Ego Headless] SQL 运行时快照测试失败");
    }
    else {
        日志::运行("[HY-Ego Headless] SQL 运行时快照测试通过");
    }

    日志::关闭();
    return 写入成功 ? 0 : 1;
}

void 打印帮助() {
    std::wcout
        << L"HY-Ego Headless\n"
        << L"  --help                    显示帮助\n"
        << L"  --run-task-control-regression  运行任务控制总控回归套件（全链+自动生长）后退出\n"
        << L"  --run-causality-regression     运行安全/服务值因果链回归测试后退出\n"
        << L"  --run-safety-physics-regression  运行安全/服务物理规则长时回归测试后退出\n"
        << L"  --run-sql-snapshot-test        运行 SQL 运行时快照写库自检后退出\n"
        << L"  --no-restart              禁用自治线程自动拉起\n"
        << L"  --monitor-ms=N            设置宿主监控周期（毫秒）\n"
        << L"  --restart-backoff-ms=N    设置拉起退避（毫秒）\n"
        << L"  --max-restarts=N          设置最大自动拉起次数，0 表示不限\n"
        << L"  --heartbeat-seconds=N     控制台生命体征输出周期（秒）\n"
        << L"  --duration-seconds=N      到时后自动退出\n"
        << L"  --sql-runtime             启用 SQL Server 运行时快照写入\n"
        << L"  --sql-instance=NAME       指定 SQL Server 实例，默认 .\\SQLEXPRESS\n"
        << L"  --sql-database=NAME       指定运行时数据库，默认 FishNestRuntime\n"
        << L"  --sql-snapshot-key=KEY    指定 latest_runtime_snapshot 主键，默认 headless.default\n"
        << L"  --sql-period-seconds=N    设置 SQL 运行时快照写入周期（秒）\n"
        << L"  --camera                  同时启动相机观测运行时（D455 失败时回退虚拟相机）\n"
        << L"  --camera-required         强制启动真实相机，失败则整体退出\n"
        << L"  --camera-virtual          强制使用虚拟相机（用于闭环验证）\n"
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

bool 解析文本参数(const std::wstring& 当前, const wchar_t* 前缀, std::wstring* 输出) {
    if (!输出) return false;
    const std::wstring_view 视图(当前);
    const std::wstring_view 目标前缀(前缀);
    if (!视图.starts_with(目标前缀)) return false;

    *输出 = std::wstring(视图.substr(目标前缀.size()));
    return true;
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
        if (当前 == L"--run-causality-regression") {
            结果.模式 = 结构_启动参数::枚举_运行模式::安全服务因果回归测试;
            continue;
        }
        if (当前 == L"--run-safety-physics-regression") {
            结果.模式 = 结构_启动参数::枚举_运行模式::安全服务物理长时回归测试;
            continue;
        }
        if (当前 == L"--run-sql-snapshot-test") {
            结果.模式 = 结构_启动参数::枚举_运行模式::SQL运行时快照测试;
            continue;
        }
        if (当前 == L"--no-restart") {
            结果.宿主配置.允许自动重启 = false;
            continue;
        }
        if (当前 == L"--sql-runtime") {
            结果.启用SQL运行时快照 = true;
            continue;
        }
        if (当前 == L"--camera") {
            结果.启用相机观测 = true;
            结果.相机配置.允许回退到虚拟相机 = true;
            结果.相机配置.强制虚拟相机 = false;
            continue;
        }
        if (当前 == L"--camera-required") {
            结果.启用相机观测 = true;
            结果.相机配置.允许回退到虚拟相机 = false;
            结果.相机配置.强制虚拟相机 = false;
            continue;
        }
        if (当前 == L"--camera-virtual") {
            结果.启用相机观测 = true;
            结果.相机配置.强制虚拟相机 = true;
            结果.相机配置.允许回退到虚拟相机 = true;
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
        if (解析文本参数(当前, L"--sql-instance=", &结果.SQL实例)) {
            if (结果.SQL实例.empty()) {
                if (错误) *错误 = L"sql-instance 不能为空";
                return false;
            }
            continue;
        }
        if (解析文本参数(当前, L"--sql-database=", &结果.SQL数据库)) {
            if (结果.SQL数据库.empty()) {
                if (错误) *错误 = L"sql-database 不能为空";
                return false;
            }
            continue;
        }
        if (解析文本参数(当前, L"--sql-snapshot-key=", &结果.SQL快照键)) {
            if (结果.SQL快照键.empty()) {
                if (错误) *错误 = L"sql-snapshot-key 不能为空";
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
        if (解析无符号参数(当前, L"--sql-period-seconds=", &数值)) {
            结果.启用SQL运行时快照 = true;
            结果.SQL写入周期 = std::chrono::seconds(数值);
            continue;
        }

        if (错误) *错误 = L"未知参数: " + 当前;
        return false;
    }

    *输出 = 结果;
    return true;
}

bool 是Headless参数_(std::wstring_view 当前) {
    if (当前.empty()) return false;

    constexpr std::wstring_view 精确参数[] = {
        L"--help",
        L"-h",
        L"/?",
        L"--run-task-control-regression",
        L"--run-causality-regression",
        L"--run-safety-physics-regression",
        L"--run-sql-snapshot-test",
        L"--no-restart",
        L"--sql-runtime",
        L"--camera",
        L"--camera-required",
        L"--camera-virtual",
        L"--service",
        L"--install-service",
        L"--uninstall-service"
    };
    for (const auto 参数 : 精确参数) {
        if (当前 == 参数) return true;
    }

    constexpr std::wstring_view 前缀参数[] = {
        L"--service-name=",
        L"--sql-instance=",
        L"--sql-database=",
        L"--sql-snapshot-key=",
        L"--monitor-ms=",
        L"--restart-backoff-ms=",
        L"--max-restarts=",
        L"--heartbeat-seconds=",
        L"--duration-seconds=",
        L"--sql-period-seconds="
    };
    for (const auto 前缀 : 前缀参数) {
        if (当前.starts_with(前缀)) return true;
    }
    return false;
}

}

bool 鱼巢Headless_命令行应接管(int argc, wchar_t* argv[]) {
    for (int i = 1; i < argc; ++i) {
        const std::wstring_view 当前 = argv[i] ? argv[i] : L"";
        if (是Headless参数_(当前)) {
            return true;
        }
    }
    return false;
}

bool 鱼巢Headless_命令行需要控制台(int argc, wchar_t* argv[]) {
    if (!鱼巢Headless_命令行应接管(argc, argv)) {
        return false;
    }
    for (int i = 1; i < argc; ++i) {
        const std::wstring_view 当前 = argv[i] ? argv[i] : L"";
        if (当前 == L"--service") {
            return false;
        }
    }
    return true;
}

namespace {

bool 初始化基础运行时() {
    static bool 已初始化 = false;
    if (已初始化) return true;

    WSADATA Winsock数据{};
    const int 结果 = ::WSAStartup(MAKEWORD(2, 2), &Winsock数据);
    if (结果 != 0) {
        std::wcerr << L"[HY-Ego Headless] WSAStartup 失败，错误码=" << 结果 << L"\n";
        return false;
    }
    已初始化 = true;
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
    std::atomic_bool 相机观测已启用{ 参数.启用相机观测 };
    std::unique_ptr<SQL运行时快照写入器类> SQL写入器;
    结构_前端命令共享状态 前端命令状态{};
    结构_自我场景再现共享状态 自我场景再现状态{};
    结构_快照诊断状态 快照诊断{};
    std::mutex 前端统计缓存互斥;
    结构_后台前端快照 前端统计缓存{};
    bool 前端统计缓存有效 = false;
    const auto 合并前端统计缓存 = [&](结构_后台前端快照* 输出快照) {
        if (!输出快照) return;
        std::lock_guard<std::mutex> 锁(前端统计缓存互斥);
        if (!前端统计缓存有效) return;
        输出快照->世界根子节点数 = 前端统计缓存.世界根子节点数;
        输出快照->需求数 = 前端统计缓存.需求数;
        输出快照->任务数 = 前端统计缓存.任务数;
        输出快照->安全任务数 = 前端统计缓存.安全任务数;
        输出快照->服务任务数 = 前端统计缓存.服务任务数;
        输出快照->叶子任务数 = 前端统计缓存.叶子任务数;
        输出快照->方法数 = 前端统计缓存.方法数;
        输出快照->状态数 = 前端统计缓存.状态数;
        输出快照->动态数 = 前端统计缓存.动态数;
        输出快照->事件动态数 = 前端统计缓存.事件动态数;
        输出快照->主要需求权重 = 前端统计缓存.主要需求权重;
        输出快照->主要需求已满足 = 前端统计缓存.主要需求已满足;
        输出快照->主要需求类型 = 前端统计缓存.主要需求类型;
    };
    后台前端服务类 前端通道([&宿主, &相机运行时, &相机观测已启用, &参数, &前端命令状态, &合并前端统计缓存, &自我场景再现状态](
        const std::unordered_map<std::string, std::string>& 请求,
        结构_后台前端快照* 输出快照,
        bool* 成功,
        std::string* 消息,
        结构_后台自我场景快照* 输出自我场景) {
        const auto 生成快照 = [&](bool 仅快速状态) {
            const auto 宿主快照 = 宿主->快照();
            const auto 相机快照 = 相机运行时 ? std::make_optional(相机运行时->快照()) : std::nullopt;
            return 生成后台快照(
                宿主快照,
                相机快照 ? &*相机快照 : nullptr,
                相机观测已启用.load(),
                &前端命令状态,
                仅快速状态);
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
            控制台请求退出.store(true);
            if (相机运行时) {
                相机运行时->请求退出();
            }
            宿主->请求停止();
            日志::运行("[HY-Ego Headless] 前端停机命令已直接触发宿主停机请求");
            if (成功) *成功 = true;
            if (消息) *消息 = "后台已收到安全停机请求";
        }
        else if (命令 == "snapshot") {
            if (成功) *成功 = true;
            if (消息) *消息 = "ok";
        }
        else if (命令 == "camera_start") {
            if (相机运行时 && 相机运行时->是否正在运行()) {
                相机观测已启用.store(true);
                if (成功) *成功 = true;
                if (消息) *消息 = "相机观测已在运行";
            }
            else {
                if (!相机运行时) {
                    相机运行时 = std::make_unique<相机观测运行时类>(参数.相机配置);
                }
                if (!相机运行时->启动()) {
                    相机观测已启用.store(false);
                    if (成功) *成功 = false;
                    if (消息) *消息 = "相机观测启动失败";
                }
                else {
                    相机观测已启用.store(true);
                    if (成功) *成功 = true;
                    if (消息) *消息 = "相机观测已启动";
                    日志::运行("[HY-Ego Headless] 已处理前端相机启动命令");
                }
            }
        }
        else if (命令 == "camera_stop") {
            相机观测已启用.store(false);
            if (!相机运行时) {
                if (成功) *成功 = true;
                if (消息) *消息 = "相机观测未启动";
            }
            else {
                相机运行时->停止();
                if (成功) *成功 = true;
                if (消息) *消息 = "相机观测已停止";
                日志::运行("[HY-Ego Headless] 已处理前端相机停止命令");
            }
        }
        else if (命令 == "scene_replay_start") {
            自我场景再现状态.启动();
            if (成功) *成功 = true;
            if (消息) *消息 = "自我场景再现已启动";
        }
        else if (命令 == "scene_replay_stop") {
            自我场景再现状态.停止();
            if (成功) *成功 = true;
            if (消息) *消息 = "自我场景再现已停止";
        }
        else if (命令 == "scene_replay_snapshot") {
            if (输出自我场景) {
                *输出自我场景 = 自我场景再现状态.生成快照(相机运行时.get());
            }
            if (成功) *成功 = true;
            if (消息) *消息 = "ok";
        }
        else if (命令 == "scene_observe_lock") {
            const auto 目标 = 请求.find("target_key");
            if (目标 == 请求.end() || 目标->second.empty()) {
                if (成功) *成功 = false;
                if (消息) *消息 = "观测锁定失败：target_key 为空";
            }
            else if (!相机运行时) {
                if (成功) *成功 = false;
                if (消息) *消息 = "观测锁定失败：相机运行时未启用";
            }
            else {
                相机运行时->锁定观测目标(目标->second);
                if (成功) *成功 = true;
                if (消息) *消息 = "已锁定观测目标: " + 目标->second;
                日志::运行("[HY-Ego Headless] 已处理前端观测锁定命令: " + 目标->second);
            }
        }
        else if (命令 == "scene_observe_unlock") {
            if (!相机运行时) {
                if (成功) *成功 = false;
                if (消息) *消息 = "观测解锁失败：相机运行时未启用";
            }
            else {
                相机运行时->解除锁定观测();
                if (成功) *成功 = true;
                if (消息) *消息 = "已解除观测目标锁定";
                日志::运行("[HY-Ego Headless] 已处理前端观测解锁命令");
            }
        }
        else {
            if (成功) *成功 = false;
            if (消息) *消息 = "未知前端命令: " + 命令;
        }

        if (输出快照) {
            const bool 需要完整统计 = (命令 == "snapshot");
            *输出快照 = 生成快照(!需要完整统计);
            if (!需要完整统计) {
                合并前端统计缓存(输出快照);
            }
        }
        if (输出自我场景) {
            输出自我场景->后台在线 = true;
        }
    });
    前端通道.启动();
    auto 下次前端统计刷新时间 = std::chrono::steady_clock::now();
    auto 下次SQL写入时间 = std::chrono::steady_clock::time_point::min();
    if (参数.启用SQL运行时快照) {
        SQL写入器 = std::make_unique<SQL运行时快照写入器类>(
            参数.SQL实例,
            参数.SQL数据库,
            参数.SQL快照键);
        日志::运行(
            "[HY-Ego Headless] 已启用 SQL 运行时快照写入: instance=" + 转UTF8(参数.SQL实例)
            + ", database=" + 转UTF8(参数.SQL数据库)
            + ", snapshot_key=" + 转UTF8(参数.SQL快照键)
            + ", period_s=" + std::to_string(参数.SQL写入周期.count()));
    }
    const auto 刷新前端统计缓存 = [&]() {
        const bool 输出诊断 = 快照诊断.应记录();
        const auto 诊断起点 = std::chrono::steady_clock::now();
        if (输出诊断) {
            日志::运行("[HY-Ego Headless][快照诊断] 开始刷新完整快照");
        }
        const auto 宿主快照 = 宿主->快照();
        if (输出诊断) {
            日志::运行f(
                "[HY-Ego Headless][快照诊断] 宿主快照完成: host={}, self={}, initialized={}, healthy={}",
                宿主快照.宿主运行中,
                宿主快照.自我线程运行中,
                宿主快照.初始化完成,
                宿主快照.健康运行);
        }
        const auto 相机快照 = 相机运行时 ? std::make_optional(相机运行时->快照()) : std::nullopt;
        if (输出诊断 && 相机快照.has_value()) {
            日志::运行f(
                "[HY-Ego Headless][快照诊断] 相机快照完成: running={}, frames={}, candidates={}",
                相机快照->运行中,
                static_cast<unsigned long long>(相机快照->已处理帧数),
                static_cast<unsigned long long>(相机快照->最近桥接候选数));
        }
        auto 完整快照 = 生成后台快照(
            宿主快照,
            相机快照 ? &*相机快照 : nullptr,
            相机观测已启用.load(),
            &前端命令状态,
            false);
        if (输出诊断) {
            日志::运行f(
                "[HY-Ego Headless][快照诊断] 完整快照生成完成: task={}, need={}, state={}, dynamic={}, event_dynamic={}",
                static_cast<unsigned long long>(完整快照.任务数),
                static_cast<unsigned long long>(完整快照.需求数),
                static_cast<unsigned long long>(完整快照.状态数),
                static_cast<unsigned long long>(完整快照.动态数),
                static_cast<unsigned long long>(完整快照.事件动态数));
        }
        if (SQL写入器) {
            const auto 当前时间 = std::chrono::steady_clock::now();
            if (当前时间 >= 下次SQL写入时间) {
                (void)SQL写入器->写入(完整快照, "runtime-loop");
                下次SQL写入时间 = 当前时间 + 参数.SQL写入周期;
                if (输出诊断) {
                    日志::运行("[HY-Ego Headless][快照诊断] SQL 快照写入完成");
                }
            }
        }
        std::lock_guard<std::mutex> 锁(前端统计缓存互斥);
        前端统计缓存 = std::move(完整快照);
        前端统计缓存有效 = true;
        if (输出诊断) {
            const auto 总耗时毫秒 = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - 诊断起点).count();
            日志::运行f("[HY-Ego Headless][快照诊断] 刷新完整快照完成，总耗时={} ms", static_cast<long long>(总耗时毫秒));
        }
    };
    刷新前端统计缓存();
    下次前端统计刷新时间 = std::chrono::steady_clock::now() + std::chrono::seconds(1);
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
    if (相机观测已启用.load()) {
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
        if (std::chrono::steady_clock::now() >= 下次前端统计刷新时间) {
            刷新前端统计缓存();
            下次前端统计刷新时间 = std::chrono::steady_clock::now() + std::chrono::seconds(1);
        }
        if (!快照.宿主故障摘要.empty() && !快照.宿主运行中) {
            if (输出心跳) {
                输出控制台生命体征(快照, 相机快照 ? &*相机快照 : nullptr);
            }
            请求所有停机();
            等待所有停机();
            return ERROR_SERVICE_SPECIFIC_ERROR;
        }

        if (相机观测已启用.load() && 相机运行时 && 相机快照 && !相机快照->运行中) {
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

    if (!初始化基础运行时()) {
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

std::wstring 命令行值加引号_(const std::wstring& 值) {
    if (值.find_first_of(L" \t\"") == std::wstring::npos) return 值;

    std::wstring 结果 = L"\"";
    for (const auto ch : 值) {
        if (ch == L'"') {
            结果 += L"\\\"";
        }
        else {
            结果.push_back(ch);
        }
    }
    结果 += L"\"";
    return 结果;
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
        if (参数.相机配置.强制虚拟相机) {
            二进制路径 += L" --camera-virtual";
        }
        else {
            二进制路径 += 参数.相机配置.允许回退到虚拟相机 ? L" --camera" : L" --camera-required";
        }
    }
    if (参数.启用SQL运行时快照) {
        二进制路径 += L" --sql-runtime";
        二进制路径 += L" --sql-instance=" + 命令行值加引号_(参数.SQL实例);
        二进制路径 += L" --sql-database=" + 命令行值加引号_(参数.SQL数据库);
        二进制路径 += L" --sql-snapshot-key=" + 命令行值加引号_(参数.SQL快照键);
        二进制路径 += L" --sql-period-seconds=" + std::to_wstring(参数.SQL写入周期.count());
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

int 鱼巢Headless_入口(int argc, wchar_t* argv[]) {
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
        if (!初始化基础运行时()) {
            return 3;
        }
        return static_cast<int>(运行任务控制总控回归测试套件_());
    }
    if (全局启动参数.模式 == 结构_启动参数::枚举_运行模式::安全服务因果回归测试) {
        if (!初始化基础运行时()) {
            return 3;
        }
        return static_cast<int>(运行安全服务因果回归测试_());
    }
    if (全局启动参数.模式 == 结构_启动参数::枚举_运行模式::安全服务物理长时回归测试) {
        if (!初始化基础运行时()) {
            return 3;
        }
        return static_cast<int>(运行安全服务物理长时回归测试_());
    }
    if (全局启动参数.模式 == 结构_启动参数::枚举_运行模式::SQL运行时快照测试) {
        if (!初始化基础运行时()) {
            return 3;
        }
        return static_cast<int>(运行SQL运行时快照测试_(全局启动参数));
    }

    if (全局启动参数.模式 == 结构_启动参数::枚举_运行模式::作为服务) {
        HANDLE 单实例守卫 = 创建单实例守卫_();
        if (!单实例守卫) {
            std::wcerr << L"[鱼巢] 已检测到另一个后台实例正在运行，当前服务实例不再启动。\n";
            return static_cast<int>(ERROR_ALREADY_EXISTS);
        }
        SERVICE_TABLE_ENTRYW 服务表[] = {
            { const_cast<LPWSTR>(全局启动参数.服务名.c_str()), 服务主函数 },
            { nullptr, nullptr }
        };
        if (!::StartServiceCtrlDispatcherW(服务表)) {
            const DWORD 错误码 = ::GetLastError();
            ::CloseHandle(单实例守卫);
            std::wcerr << L"连接 Service Control Manager 失败: " << 取Win32错误文本(错误码) << L"\n";
            return static_cast<int>(错误码);
        }
        ::CloseHandle(单实例守卫);
        return 0;
    }

    if (!初始化基础运行时()) {
        return 3;
    }

    HANDLE 单实例守卫 = 创建单实例守卫_();
    if (!单实例守卫) {
        std::wcerr << L"[鱼巢] 已检测到另一个后台实例正在运行，当前实例直接退出。\n";
        return static_cast<int>(ERROR_ALREADY_EXISTS);
    }

    ::SetConsoleCtrlHandler(控制台控制处理, TRUE);
    日志::运行("[HY-Ego Headless] 控制台模式启动");
    const DWORD 退出码 = 运行宿主循环(全局启动参数, nullptr, true);
    ::CloseHandle(单实例守卫);
    return static_cast<int>(退出码);
}

#ifdef HY_HEADLESS_STANDALONE
int wmain(int argc, wchar_t* argv[]) {
    return 鱼巢Headless_入口(argc, argv);
}
#endif
