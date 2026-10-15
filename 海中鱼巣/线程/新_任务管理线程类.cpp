#include "新_任务管理线程类.h"
#include "../领域/新_本能函数集.h"
#include "../领域/新_方法类.h"
#include "../业务/新_世界树类.h"
#include "../核心/日志系统.h"
#include "../核心/容错检查.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <condition_variable>
#include <limits>
#include <map>
#include <mutex>
#include <new>
#include <set>
#include <stdexcept>
#include <thread>
#include <vector>

namespace 海中鱼巣 {

namespace {
bool 学习筹办安全状态(新任务状态 状态) noexcept {
    return 状态 == 新任务状态::已建立 || 状态 == 新任务状态::待查找方法
        || 状态 == 新任务状态::待检查条件 || 状态 == 新任务状态::等待条件;
}

bool 条件筹办为业务等待(const 新任务治理条件筹办结果& 结果) noexcept {
    if (结果.状态 != 新任务治理条件筹办状态::已完成
        || 结果.候选已完整核验 || !结果.方法选择状态) return false;
    switch (*结果.方法选择状态) {
    case 新任务方法选择状态::等待条件:
    case 新任务方法选择状态::匹配不唯一:
    case 新任务方法选择状态::目标已满足:
    case 新任务方法选择状态::无适用结果:
    case 新任务方法选择状态::幅度材料不足:
    case 新任务方法选择状态::结果定义暂不支持:
        return true;
    default: return false;
    }
}

bool 条件筹办有结构错误(const 新任务治理条件筹办结果& 结果) noexcept {
    return 结果.状态 == 新任务治理条件筹办状态::结构不一致
        || std::any_of(结果.方法筹办组.begin(), 结果.方法筹办组.end(), [](const auto& 方法) {
            return 方法.状态 == 新任务候选方法筹办状态::结构缺口
                || std::any_of(方法.条件结果组.begin(), 方法.条件结果组.end(), [](const auto& 配对) {
                    return 配对.状态 == 新任务方法条件项核验状态::结构缺口;
                });
        });
}

bool 扫描承接可资源重试(const 新任务承接处理结果& 结果) noexcept {
    if (结果.原消息.依据 != 新任务承接依据::自我全树扫描 || !结果.承接)
        return false;
    const auto& 承接 = *结果.承接;
    return 承接.状态 == 新任务治理承接状态::资源失败
        || (承接.状态 == 新任务治理承接状态::任务操作未完成
            && 承接.建立结果
            && 承接.建立结果->状态 == 新任务操作状态::资源失败);
}

void 记录需求任务承接观察(const 新任务承接处理结果& 结果) noexcept {
    try {
        if (!结果.承接) return;
        const auto& 承接 = *结果.承接;
        const auto* 建立 = 承接.建立结果 ? &*承接.建立结果 : nullptr;
        const bool 同步未完成 = 建立
            && 建立->状态 == 新任务操作状态::服务任务集合同步失败;
        const bool 结构异常 = 建立
            && (建立->状态 == 新任务操作状态::结构不一致
                || 建立->状态 == 新任务操作状态::需求已有多个任务);
        const bool 已建立 = 建立 && 建立->状态 == 新任务操作状态::已建立;
        const bool 资源未完成 = 承接.状态 == 新任务治理承接状态::资源失败
            || (建立 && 建立->状态 == 新任务操作状态::资源失败);
        if (!同步未完成 && !结构异常 && !已建立 && !资源未完成) return;
        std::wostringstream 文本;
        文本 << (同步未完成 ? L"任务集合未完成同步；返回值未区分来源读取、回调或写后失败"
            : 结构异常 ? L"需求任务结构异常"
            : 已建立 ? L"任务建立返回及承接摘要" : L"本次承接因资源问题未完成")
            << L"；消息=" << 结果.原消息.消息编号
            << L"；需求=" << 结果.原消息.需求节点.值
            << L"；消息依据=" << static_cast<int>(结果.原消息.依据)
            << L"；处理状态=" << static_cast<int>(结果.状态)
            << L"；承接状态=" << static_cast<int>(承接.状态)
            << L"；写入已尝试=" << 承接.写入已尝试 << L"；建立状态=";
        if (建立) 文本 << static_cast<int>(建立->状态); else 文本 << L"无";
        文本 << L"；任务=";
        if (建立 && 建立->任务节点) 文本 << 建立->任务节点->值; else 文本 << L"无";
        文本 << L"；任务虚拟存在=";
        if (建立 && 建立->任务虚拟存在节点) 文本 << 建立->任务虚拟存在节点->值;
        else 文本 << L"无";
        if (扫描承接可资源重试(结果))
            文本 << L"；扫描承接保留原消息及结果通道，延后重新读取并建立或复用任务";
        if (结构异常) {
            (void)记录逻辑错误日志(L"任务管理线程", L"需求任务承接", 文本.str());
        } else {
            (void)记录运行日志(L"任务管理线程", L"需求任务承接", 文本.str());
        }
    } catch (...) {
        // 日志不可用不改变已经形成的结构化结果。
    }
}
} // namespace

bool 新长期服务值结算配置::完整() const noexcept {
    return 世界树服务
        && 有效(自我所在场景)
        && 有效(自我存在)
        && 有效(服务值特征)
        && 有效(完成服务需求) && 有效(服务根需求)
        && 有效(登记方法校验节点) && 冻结前值 >= 0
        && 有效(目标值域概念节点) && 工程诊断端口;
}

bool 新任务筹办反馈定位::完整() const noexcept {
    return 有效(任务节点)
        && 原工作.工作编号 != 0
        && 原工作.任务节点 == 任务节点
        && (原工作.种类 == 新任务工作种类::查找方法
            || 原工作.种类 == 新任务工作种类::检查条件);
}

bool 新任务学习复核定位::完整() const noexcept {
    return 复核编号 && 学习交接编号 && 有效(方法节点)
        && 有效(任务节点) && 有效(需求节点)
        && (!候选材料节点 || 有效(*候选材料节点))
        && (!方法实例节点 || 有效(*方法实例节点))
        && (!方法执行组织节点 || 有效(*方法执行组织节点))
        && 学习筹办安全状态(任务状态);
}

bool 新任务等待复核定位::完整() const noexcept {
    return 复核编号 && 定位.完整()
        && 原因 > 新任务等待原因::无 && 原因 < 新任务等待原因::需核查;
}

bool 新任务结果反馈定位::完整() const noexcept {
    if (!有效(任务节点) || !有效(执行实例节点) || !有效(影响评估节点)
        || !原工作.工作编号 || 原工作.任务节点 != 任务节点) return false;
    if (原工作.种类 == 新任务工作种类::检测本能结果)
        return 原工作.本能结果检测请求
            && 原工作.本能结果检测请求->任务节点 == 任务节点
            && 原工作.本能结果检测请求->执行实例节点 == 执行实例节点
            && !原工作.本能执行请求 && !原工作.本能结果获取请求;
    return 原工作.种类 == 新任务工作种类::获取本能结果
        && 原工作.本能结果获取请求
        && 原工作.本能结果获取请求->任务节点 == 任务节点
        && 原工作.本能结果获取请求->执行实例节点 == 执行实例节点
        && !原工作.本能执行请求 && !原工作.本能结果检测请求;
}

namespace {
std::mutex 管理所有者互斥;
const 新_任务管理线程类* 管理所有者 = nullptr;

std::chrono::milliseconds 等待时长(std::uint64_t 毫秒) noexcept {
    const auto 最大 = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::duration::max() / 4).count();
    return std::chrono::milliseconds((std::min)(毫秒, static_cast<std::uint64_t>(最大)));
}

enum class 筹办派发状态 : std::uint8_t {
    已派发 = 1, 无可派发任务, 资源暂不可用, 已停止, 内部错误
};
}

struct 新_任务管理线程类::实现 final {
    explicit 实现(新_任务类& 任务) noexcept
        : 任务服务(&任务),
          工作线程(new (std::nothrow) 新_任务工作线程类(
              任务, [this] {
                  try {
                      std::lock_guard 锁(互斥);
                      变化.notify_all();
                  } catch (...) {}
              })) {}
    新_任务类* 任务服务;
    const 新_方法类* 学习方法读取 = nullptr;
    struct 学习任务项 final {
        稳定编码 任务节点;
        std::optional<std::uint64_t> 复核编号;
        bool 已终结 = false;
    };
    struct 学习事件 final {
        新学习结束通知 通知;
        bool 已展开 = false, 已终结 = false;
        std::vector<学习任务项> 任务组;
        std::chrono::steady_clock::time_point 可处理时点{};
    };
    struct 学习复核记录 final {
        新任务学习复核定位 定位;
        std::optional<新自我学习复核决议> 决议;
        bool 已反馈 = false, 已终结 = false, 准备已尝试 = false;
    };
    // 已终结项保留到实例销毁，仅用于同一进程交接去重，不是业务账。
    std::map<std::uint64_t, 学习事件> 学习事件组;
    std::optional<std::uint64_t> 当前学习交接;
    std::map<std::uint64_t, 学习复核记录> 学习复核组;
    std::set<std::uint64_t> 学习复核占用;
    std::map<std::uint64_t, std::uint64_t> 学习失效筹办工作;
    std::uint64_t 下一学习复核编号 = 1;
    std::optional<新学习结束通知> 最近方法学习结束;
    std::optional<新学习运行异常记录> 最近学习运行异常;
    mutable std::mutex 互斥;
    std::mutex 加入互斥;
    std::condition_variable 变化;
    std::thread 线程;
    新任务管理运行配置 配置;
    新任务管理生命周期 生命周期 = 新任务管理生命周期::未启动;
    bool 创建已尝试 = false, 已进入 = false, 停止 = false, 已完成 = false, 已加入 = false;
    bool 内部错误发生 = false;
    struct 消息项 final {
        新任务承接消息 消息;
        std::optional<新任务筹办反馈定位> 父筹办定位;
        std::promise<新任务承接处理结果> 承诺;
        std::shared_future<新任务承接处理结果> 结果 = 承诺.get_future().share();
        std::chrono::steady_clock::time_point 可处理时点;
        bool 扫描资源失败已记录 = false;
    };
    std::vector<std::shared_ptr<消息项>> 活动;
    struct 已完成自动决议 final {
        新任务承接消息 消息;
        std::shared_future<新任务承接处理结果> 结果;
    };
    // 普通自动治理当前只使用安全根；保留固定小容量槽位，仅保存最近
    // 已完成自动决议的共享结果，不建立历史消息账。
    std::array<std::optional<已完成自动决议>, 2> 最近自动决议;
    std::unique_ptr<新_任务工作线程类> 工作线程;
    struct 工作占用 final {
        新任务工作请求 请求;
        std::shared_future<新任务工作处理结果> 结果;
    };
    std::vector<工作占用> 活动工作;
    std::set<std::uint64_t> 阻塞任务;
    // 只承载可以被新的筹办输入替代的W1/W2来源；其它保护仍在阻塞任务。
    std::set<std::uint64_t> 筹办阻塞任务;
    // W0是完整只读快照，不授予执行许可；接受新工作后移出本轮就绪集合。
    std::map<std::uint64_t, 新任务调度材料项> 调度材料;
    bool 已有完整调度材料 = false, 调度待触发 = true;
    std::optional<新任务工作请求> 待派调度工作, 待派筹办工作;
    std::set<std::uint64_t> 零动作失效调度工作;
    std::chrono::steady_clock::time_point 下一调度时点{}, 最早事件调度时点{};
    struct 等待来源记录 final {
        新任务信息 任务;
        新任务等待原因 原因 = 新任务等待原因::未分类;
        std::optional<新任务工作请求> 原工作;
    };
    std::map<std::uint64_t, 等待来源记录> 等待来源;
    std::set<std::uint64_t> 等待目标复核;
    std::map<std::uint64_t, 新任务工作请求> 筹办资源重试;
    struct 等待复核记录 final {
        新任务等待复核定位 定位;
        std::optional<新自我等待复核决议> 决议;
        bool 已反馈 = false, 已终结 = false, 准备已尝试 = false;
    };
    std::map<std::uint64_t, 等待复核记录> 等待复核组;
    std::set<std::uint64_t> 等待复核占用;
    std::map<std::uint64_t, 新任务调度材料项> 上次等待材料;
    std::uint64_t 下一等待复核编号 = 1;
    std::chrono::steady_clock::time_point 下一条件复核时点{};
    std::chrono::steady_clock::time_point 下一工作消费时点{};
    std::optional<新任务调度读取状态> 最近调度读取状态;
    std::vector<稳定编码> 最近调度异常任务;
    std::map<std::uint64_t, 新任务筹办处理结果> 最近筹办结果;
    std::set<std::uint64_t> 待反馈任务;
    struct 自我筹办裁决记录 final {
        新自我筹办裁决 裁决;
        bool 已完成接收 = false;
        bool 执行工作已派发 = false;
        std::optional<新任务工作请求> 执行工作;
        std::vector<新任务已确认条件子需求> 已确认子需求组;
        // 与成功D一一对应；收到回执即预分配，尚未送达也占等待位置。
        std::vector<std::shared_ptr<消息项>> 子需求承接组;
        std::vector<bool> 子需求承接完成组;
        std::set<std::uint64_t> 来源变化子需求;
        bool 条件复核待触发 = false;
        bool 条件交接失败 = false;
    };
    std::map<std::uint64_t, 自我筹办裁决记录> 最近自我筹办裁决;
    std::map<std::uint64_t, 新任务执行处理结果> 最近执行结果;
    // 只有运行期定位；正式结果仍从同一T/X读取，不保存第二份状态材料。
    struct 结果等待记录 final {
        稳定编码 执行实例节点;
        std::chrono::steady_clock::time_point 下次检测时点{};
        bool 待检测 = true;
        bool 在途期间再次唤醒 = false;
        std::optional<新任务工作请求> 原检测工作;
        bool 已派发 = false;
    };
    std::map<std::uint64_t, 结果等待记录> 结果等待任务;
    bool 结果通知待处理 = false;
    bool 下次优先结果检测 = false;
    std::uint64_t 最近检测任务值 = 0;
    std::set<std::uint64_t> 待结果反馈任务;
    std::map<std::uint64_t, 新自我任务后继决议> 最近任务后继决议;
    std::optional<新长期服务值结算配置> 服务值结算配置;
    std::optional<稳定编码> 服务值结算任务;
    // 仅为进程内交接队列；绑定及重新筹办只由管理循环推进。
    std::vector<稳定编码> 待服务结算来源;
    bool 服务结算来源排空中 = false;
    const void* 自我反馈所有者 = nullptr;
    新_任务管理线程类::筹办反馈接收函数 自我反馈接收;
    新_任务管理线程类::结果反馈接收函数 自我结果反馈接收;
    新_任务管理线程类::学习复核接收函数 自我学习反馈接收;
    新_任务管理线程类::等待复核接收函数 自我等待反馈接收;
    bool 工程停止已报告 = false;
    std::uint64_t 下一工作编号 = 1;

    void 报告工程停止(std::string_view 原因) noexcept {
        try {
            新服务值结算确认接口* 诊断 = nullptr;
            {
                std::lock_guard 锁(互斥);
                if (工程停止已报告) return;
                内部错误发生 = true;
                停止 = true;
                工程停止已报告 = true;
                if (服务值结算配置) 诊断 = 服务值结算配置->工程诊断端口;
                变化.notify_all();
            }
            if (诊断) 诊断->报告并停止(原因);
        } catch (...) {}
    }

    新任务管理操作结果 操作结果(新任务管理操作状态 状态) const noexcept {
        return {状态, 生命周期};
    }

    bool 有服务结算来源待处理已加锁(std::uint64_t 任务值) const noexcept {
        return 服务值结算任务 && 服务值结算任务->值 == 任务值
            && !待服务结算来源.empty();
    }

    bool 学习占用或旧筹办已失效已加锁(
        const 新任务筹办反馈定位& 定位) const noexcept {
        const auto 位置 = 学习失效筹办工作.find(定位.任务节点.值);
        return 学习复核占用.contains(定位.任务节点.值)
            || 等待复核占用.contains(定位.任务节点.值)
            || 有服务结算来源待处理已加锁(定位.任务节点.值)
            || (位置 != 学习失效筹办工作.end()
                && 位置->second == 定位.原工作.工作编号);
    }

    // 仅登记运行进度；调用者已持邮箱锁，不在这里调用领域服务。
    void 触发条件子需求复核已加锁() noexcept {
        调度待触发 = true;
        for (auto& [任务值, 记录] : 最近自我筹办裁决) {
            (void)任务值;
            if (记录.裁决.子需求回执 && 记录.已完成接收)
                记录.条件复核待触发 = true;
        }
        变化.notify_all();
    }

    bool 子需求承接定位有效已加锁(const 消息项& 项) const noexcept {
        if (!项.父筹办定位
            || 学习占用或旧筹办已失效已加锁(*项.父筹办定位)) return false;
        const auto 任务值 = 项.父筹办定位->任务节点.值;
        const auto 筹办 = 最近筹办结果.find(任务值);
        const auto 裁决 = 最近自我筹办裁决.find(任务值);
        if (筹办 == 最近筹办结果.end()
            || 筹办->second.原工作 != 项.父筹办定位->原工作
            || 裁决 == 最近自我筹办裁决.end()
            || !裁决->second.已完成接收
            || 裁决->second.裁决.定位 != *项.父筹办定位
            || !裁决->second.裁决.子需求回执
            || 裁决->second.来源变化子需求.contains(项.消息.需求节点.值)) return false;
        return std::any_of(裁决->second.已确认子需求组.begin(),
            裁决->second.已确认子需求组.end(), [&](const auto& 子项) {
                return 子项.需求节点 == 项.消息.需求节点;
            });
    }

    void 尝试复核条件子需求() noexcept {
        struct 待复核 final {
            新任务筹办反馈定位 定位;
            std::vector<新任务已确认条件子需求> 子需求组;
        };
        try {
            std::vector<待复核> 待复核组;
            {
                std::lock_guard 锁(互斥);
                if (停止 || 已完成) return;
                const auto 此刻 = std::chrono::steady_clock::now();
                if (此刻 < 下一条件复核时点) return;
                下一条件复核时点 = 此刻 + 等待时长(配置.调度复核间隔毫秒);
                for (auto& [任务值, 记录] : 最近自我筹办裁决) {
                    (void)任务值;
                    if (学习占用或旧筹办已失效已加锁(记录.裁决.定位)
                        || 任务有执行保护已加锁(任务值)
                        || !记录.已完成接收 || !记录.裁决.子需求回执
                        || !记录.条件复核待触发 || 记录.条件交接失败
                        || !std::all_of(记录.子需求承接完成组.begin(),
                            记录.子需求承接完成组.end(), [](bool 完成) { return 完成; }))
                        continue;
                    待复核组.push_back({记录.裁决.定位, 记录.已确认子需求组});
                }
            }
            for (const auto& 项 : 待复核组) {
                const auto 就绪 = 任务服务->复核条件子需求就绪(
                    项.定位.任务节点, 项.子需求组);
                if (就绪.状态 != 新任务条件子需求就绪状态::已就绪
                    || !就绪.全部需求已满足 || !就绪.当前选择
                    || 就绪.当前选择->状态 != 新任务方法选择状态::已选定)
                    continue;
                std::lock_guard 锁(互斥);
                if (停止 || 已完成) return;
                const auto 任务值 = 项.定位.任务节点.值;
                const auto 筹办 = 最近筹办结果.find(任务值);
                const auto 裁决 = 最近自我筹办裁决.find(任务值);
                if (筹办 == 最近筹办结果.end()
                    || 任务有执行保护已加锁(任务值)
                    || 学习占用或旧筹办已失效已加锁(项.定位)
                    || 筹办->second.原工作 != 项.定位.原工作
                    || 裁决 == 最近自我筹办裁决.end()
                    || 裁决->second.裁决.定位 != 项.定位
                    || 裁决->second.已确认子需求组 != 项.子需求组
                    || 裁决->second.条件交接失败
                    || !std::all_of(裁决->second.子需求承接完成组.begin(),
                        裁决->second.子需求承接完成组.end(), [](bool 完成) { return 完成; })
                    || std::any_of(活动工作.begin(), 活动工作.end(),
                        [&](const auto& 占用) { return 占用.请求.任务节点.值 == 任务值; }))
                    continue;
                待反馈任务.erase(任务值);
                最近筹办结果.erase(筹办);
                最近自我筹办裁决.erase(裁决);
                // 只释放相同W2的软阻塞；独立读回/确认/执行保护绝不清空。
                筹办阻塞任务.erase(任务值);
                调度待触发 = true;
                // T仍是待检查条件，下一步只经普通调度形成新W2。
                变化.notify_all();
            }
        } catch (...) {
            // 保留正式D、父阻塞与原工作；后续真实事件可重新触发。
        }
    }

    void 终结待办(新任务承接处理状态 状态) noexcept {
        for (const auto& 项 : 活动) {
            try { 项->承诺.set_value({项->消息, 状态, std::nullopt}); }
            catch (...) {}
        }
        活动.clear();
        for (auto& [编号, 记录] : 学习复核组) {
            (void)编号;
            记录.已终结 = true;
        }
        学习复核占用.clear();
        for (auto& [编号, 记录] : 等待复核组) {
            (void)编号;
            记录.已终结 = true;
        }
        等待复核占用.clear();
        const auto 未交付 = std::find_if(学习事件组.begin(), 学习事件组.end(),
            [](const auto& 事件) { return !事件.second.已终结; });
        if (未交付 != 学习事件组.end())
            最近学习运行异常 = 新学习运行异常记录{
                未交付->second.通知.方法节点, 新学习运行异常原因::停止取消};
    }

    void 尝试反馈筹办结果() noexcept {
        新任务筹办反馈定位 定位;
        新_任务管理线程类::筹办反馈接收函数 接收;
        try {
            {
                std::lock_guard 锁(互斥);
                if (!自我反馈接收 || 待反馈任务.empty() || 停止 || 已完成) return;
                const auto 待反馈 = std::find_if(待反馈任务.begin(), 待反馈任务.end(),
                    [&](const auto 任务值) {
                        const auto 位置 = 最近筹办结果.find(任务值);
                        return 位置 == 最近筹办结果.end()
                            || !学习占用或旧筹办已失效已加锁(
                                {稳定编码{任务值}, 位置->second.原工作});
                    });
                if (待反馈 == 待反馈任务.end()) return;
                const auto 任务值 = *待反馈;
                const auto 结果位置 = 最近筹办结果.find(任务值);
                if (结果位置 == 最近筹办结果.end()) {
                    待反馈任务.erase(任务值);
                    return;
                }
                定位 = {稳定编码{任务值}, 结果位置->second.原工作};
                接收 = 自我反馈接收;
            }
            // 回调可能进入self邮箱，绝不持manager互斥调用。
            (void)接收(定位);
        } catch (...) {
            // 定位仍保留在待反馈集合；后续按具名重试间隔再次尝试。
        }
    }

    void 尝试反馈任务结果() noexcept {
        新任务结果反馈定位 定位;
        新_任务管理线程类::结果反馈接收函数 接收;
        try {
            {
                std::lock_guard 锁(互斥);
                if (!自我结果反馈接收 || 待结果反馈任务.empty()
                    || 停止 || 已完成) return;
                const auto 任务值 = *待结果反馈任务.begin();
                const auto 结果位置 = 最近执行结果.find(任务值);
                if (结果位置 == 最近执行结果.end()
                    || 结果位置->second.状态
                        != 新任务执行处理状态::已完成影响评估
                    || !结果位置->second.独立读回
                    || !结果位置->second.独立读回->执行
                    || !结果位置->second.独立读回->影响评估) {
                    待结果反馈任务.erase(任务值);
                    return;
                }
                定位 = {稳定编码{任务值}, 结果位置->second.原工作,
                    结果位置->second.独立读回->执行->节点,
                    结果位置->second.独立读回->影响评估->节点};
                接收 = 自我结果反馈接收;
            }
            (void)接收(定位);
        } catch (...) {
            // 保留正式定位，后续按运行配置再次尝试。
        }
    }

    // 仅在manager锁内登记已独立读回的当前执行，不做领域读取。
    void 登记结果等待已加锁(稳定编码 任务节点, 稳定编码 执行实例节点) {
        auto [位置, 新建] = 结果等待任务.try_emplace(任务节点.值);
        auto& 记录 = 位置->second;
        if (!新建 && 记录.执行实例节点 != 执行实例节点)
            throw std::logic_error("same task has another pending execution");
        记录.执行实例节点 = 执行实例节点;
        if (记录.已派发) 记录.在途期间再次唤醒 = true;
        else 记录.待检测 = true;
    }

    void 更新结果检测唤醒已加锁() noexcept {
        const bool 收到通知 = 结果通知待处理;
        结果通知待处理 = false;
        const auto 当前 = std::chrono::steady_clock::now();
        for (auto& [任务值, 记录] : 结果等待任务) {
            (void)任务值;
            if (记录.已派发) {
                if (收到通知) 记录.在途期间再次唤醒 = true;
            } else if (收到通知 || 当前 >= 记录.下次检测时点) {
                记录.待检测 = true;
            }
        }
    }

    筹办派发状态 尝试派发结果检测() noexcept {
        try {
            std::unique_lock 锁(互斥);
            if (停止 || 已完成) return 筹办派发状态::已停止;
            更新结果检测唤醒已加锁();
            if (结果等待任务.empty()) return 筹办派发状态::无可派发任务;
            auto 位置 = 结果等待任务.upper_bound(最近检测任务值);
            for (std::size_t 剩余 = 结果等待任务.size(); 剩余; --剩余) {
                if (位置 == 结果等待任务.end()) 位置 = 结果等待任务.begin();
                const auto 当前位置 = 位置++;
                const auto 任务值 = 当前位置->first;
                auto& 记录 = 当前位置->second;
                if (!记录.待检测 || 记录.已派发
                    || std::any_of(活动工作.begin(), 活动工作.end(),
                        [&](const auto& 项) { return 项.请求.任务节点.值 == 任务值; }))
                    continue;
                活动工作.reserve(活动工作.size() + 1);
                if (!记录.原检测工作) {
                    if (!下一工作编号 || 下一工作编号 == (std::numeric_limits<std::uint64_t>::max)())
                        return 筹办派发状态::内部错误;
                    新任务工作请求 请求;
                    请求.工作编号 = 下一工作编号++;
                    请求.种类 = 新任务工作种类::检测本能结果;
                    请求.任务节点 = 稳定编码{任务值};
                    请求.本能结果检测请求 = 新任务结果检测请求{请求.任务节点, 记录.执行实例节点};
                    记录.原检测工作 = std::move(请求);
                }
                工作占用 待接纳{*记录.原检测工作, {}};
                // 提交只在worker邮箱锁内接纳，不调用owner或完成回调。
                // 保持manager锁直到future安装，避免等待记录被并发清理。
                const auto 接收 = 工作线程->提交工作(待接纳.请求);
                if (接收.状态 == 新任务工作接收状态::已接收
                    || 接收.状态 == 新任务工作接收状态::精确重复) {
                    // 提交前已预留；即使停止恰好到达也必须保住future交给收束。
                    待接纳.结果 = 接收.处理结果;
                    活动工作.push_back(std::move(待接纳));
                    const auto 当前 = 结果等待任务.find(任务值);
                    if (当前 == 结果等待任务.end()
                        || 当前->second.原检测工作 != 活动工作.back().请求)
                        return 筹办派发状态::内部错误;
                    当前->second.已派发 = true;
                    当前->second.待检测 = false;
                    最近检测任务值 = 任务值;
                    return 筹办派发状态::已派发;
                }
                if (接收.状态 == 新任务工作接收状态::任务已占用) continue;
                if (接收.状态 == 新任务工作接收状态::邮箱已满
                    || 接收.状态 == 新任务工作接收状态::资源失败)
                    return 筹办派发状态::资源暂不可用;
                return 接收.状态 == 新任务工作接收状态::已停止接收
                    ? 筹办派发状态::已停止 : 筹办派发状态::内部错误;
            }
            return 筹办派发状态::无可派发任务;
        } catch (const std::bad_alloc&) { return 筹办派发状态::资源暂不可用; }
        catch (...) { return 筹办派发状态::内部错误; }
    }

    bool 任务有执行保护已加锁(std::uint64_t 任务值) const noexcept {
        return 阻塞任务.contains(任务值)
            || 结果等待任务.contains(任务值) || 待结果反馈任务.contains(任务值)
            || std::any_of(活动工作.begin(), 活动工作.end(), [&](const auto& 工作) {
                return 工作.请求.任务节点.值 == 任务值;
            });
    }

    void 登记等待来源已加锁(const 新任务信息& 任务, 新任务等待原因 原因,
        std::optional<新任务工作请求> 原工作 = {}) {
        等待来源.insert_or_assign(任务.节点.值, 等待来源记录{任务, 原因, std::move(原工作)});
        上次等待材料.erase(任务.节点.值);
        调度待触发 = true;
    }

    static bool 等待知识相同(新任务调度材料项 左, 新任务调度材料项 右) {
        for (auto* 项 : {&左, &右})
            for (auto& 方法 : 项->当前候选方法闭包)
                for (auto& 配对 : 方法.条件结果组) 配对.命中次数 = 0;
        return 左.定位 == 右.定位 && 左.需求 == 右.需求
            && 左.候选材料 == 右.候选材料 && 左.条件核验 == 右.条件核验
            && 左.当前候选方法闭包 == 右.当前候选方法闭包;
    }

    // 只消费完整W0。所有可能分配失败的副本先构造，最后在邮箱锁内交换。
    void 处理调度扫描结果(const 工作占用& 占用) noexcept {
        try {
            const auto& 返回 = 占用.结果.get();
            if (返回.原请求 != 占用.请求
                || (返回.状态 != 新任务工作处理状态::调度材料已返回
                    && 返回.状态 != 新任务工作处理状态::停止取消)
                || (返回.状态 == 新任务工作处理状态::调度材料已返回 && !返回.调度材料)) {
                报告工程停止("调度扫描工作返回结构不一致");
                return;
            }
            std::map<std::uint64_t, 新任务调度材料项> 新材料;
            std::vector<稳定编码> 异常任务;
            const bool 已读取 = 返回.原请求 == 占用.请求
                && 返回.状态 == 新任务工作处理状态::调度材料已返回 && 返回.调度材料
                && 返回.调度材料->状态 == 新任务调度读取状态::已读取;
            if (已读取) {
                for (const auto& 项 : 返回.调度材料->项组) {
                    if (!有效(项.定位.任务节点)
                        || !新材料.emplace(项.定位.任务节点.值, 项).second) {
                        报告工程停止("调度扫描返回重复或空任务身份");
                        return;
                    }
                    if (项.状态 == 新任务调度项状态::结构不一致)
                        异常任务.push_back(项.定位.任务节点);
                }
            }
            std::unique_lock 锁(互斥);
            if (零动作失效调度工作.contains(占用.请求.工作编号)) {
                if (工作线程->确认结果已接收(占用.请求.工作编号, {}) != 新任务工作操作状态::已完成) {
                    锁.unlock();
                    报告工程停止("条件失效后的陈旧调度结果无法确认回收");
                    return;
                }
                零动作失效调度工作.erase(占用.请求.工作编号);
                std::erase_if(活动工作, [&](const auto& 项) { return 项.请求.工作编号 == 占用.请求.工作编号; });
                调度材料.clear(); 已有完整调度材料 = false; 调度待触发 = true;
                变化.notify_all();
                return;
            }
            const bool 诊断变化 = 返回.调度材料
                && (最近调度读取状态 != 返回.调度材料->状态 || 最近调度异常任务 != 异常任务);
            auto 新上次 = 上次等待材料;
            auto 新记录 = 等待复核组;
            auto 新占用 = 等待复核占用;
            auto 新失效 = 学习失效筹办工作;
            auto 新编号 = 下一等待复核编号;
            if (已读取 && !停止) {
                for (const auto& [任务值, 项] : 新材料) {
                    const auto 状态 = 项.定位.任务状态;
                    if ((状态 != 新任务状态::等待条件 && 状态 != 新任务状态::待检查条件
                            && 状态 != 新任务状态::已建立)
                        || !项.任务 || !项.需求 || !项.定位.完整()
                        || 项.状态 == 新任务调度项状态::结构不一致
                        || 项.状态 == 新任务调度项状态::旧执行待结果
                        || 任务有执行保护已加锁(任务值)
                        || 有服务结算来源待处理已加锁(任务值)
                        || 学习复核占用.contains(任务值) || 新占用.contains(任务值)) continue;
                    const auto 裁决 = 最近自我筹办裁决.find(任务值);
                    if (待反馈任务.contains(任务值)
                        || (裁决 != 最近自我筹办裁决.end()
                            && (!裁决->second.已完成接收 || 裁决->second.执行工作
                                || 裁决->second.裁决.子需求回执))) continue;
                    auto 原因 = 项.等待原因;
                    const auto 来源 = 等待来源.find(任务值);
                    if (来源 != 等待来源.end() && 来源->second.任务 == *项.任务)
                        原因 = 来源->second.原因;
                    if (原因 == 新任务等待原因::需核查 || 原因 == 新任务等待原因::未分类
                        || 原因 == 新任务等待原因::无) continue;
                    const auto 上次 = 新上次.find(任务值);
                    bool 需复核 = 等待目标复核.contains(任务值);
                    if (原因 == 新任务等待原因::首次关联待许可
                        || 原因 == 新任务等待原因::需求材料未到
                        || 原因 == 新任务等待原因::执行后等待
                        || 原因 == 新任务等待原因::同次执行比较材料不足) 需复核 = true;
                    else if (原因 == 新任务等待原因::尚无方法) {
                        // 完整空召回不会因周期经过而重复W1。
                        需复核 = 需复核 || (!项.当前候选方法闭包.empty()
                            && (上次 == 新上次.end() || !等待知识相同(上次->second, 项)));
                    } else if (原因 == 新任务等待原因::候选条件未齐) {
                        需复核 = 需复核 || (上次 != 新上次.end() && !等待知识相同(上次->second, 项));
                        if (上次 == 新上次.end() && 项.条件核验) {
                            // 与进入等待时的W2材料比较，防止登记前已变好被吞掉，
                            // 也不能把一直具备但匹配不唯一等情况每轮重派W2。
                            const auto 原筹办 = 最近筹办结果.find(任务值);
                            if (原筹办 != 最近筹办结果.end() && 原筹办->second.独立条件筹办) {
                                auto 原核验 = *原筹办->second.独立条件筹办;
                                原核验.方法选择状态.reset();
                                auto 本次核验 = *项.条件核验;
                                本次核验.方法选择状态.reset();
                                需复核 = 需复核 || 原核验 != 本次核验;
                            }
                        }
                    }
                    新上次.insert_or_assign(任务值, 项);
                    if (!需复核) continue;
                    if (!新编号 || 新编号 == (std::numeric_limits<std::uint64_t>::max)()) {
                        内部错误发生 = true;
                        停止 = true;
                        break;
                    }
                    等待复核记录 记录;
                    记录.定位 = {新编号++, 项.定位, 原因};
                    新记录.emplace(记录.定位.复核编号, 记录);
                    新占用.insert(任务值);
                    const auto 筹办 = 最近筹办结果.find(任务值);
                    if (筹办 != 最近筹办结果.end())
                        新失效.insert_or_assign(任务值, 筹办->second.原工作.工作编号);
                }
                for (auto 位置 = 新上次.begin(); 位置 != 新上次.end();) {
                    const auto 当前 = 新材料.find(位置->first);
                    if (当前 == 新材料.end() || !学习筹办安全状态(当前->second.定位.任务状态))
                        位置 = 新上次.erase(位置);
                    else ++位置;
                }
            }
            const auto 确认 = 工作线程->确认结果已接收(占用.请求.工作编号, {});
            if (确认 != 新任务工作操作状态::已完成) {
                内部错误发生 = true;
                停止 = true;
                return; // future及容量保留，绝不伪装确认成功。
            }
            if (已读取 && !停止) {
                调度材料.swap(新材料);
                上次等待材料.swap(新上次);
                等待复核组.swap(新记录);
                等待复核占用.swap(新占用);
                学习失效筹办工作.swap(新失效);
                下一等待复核编号 = 新编号;
                已有完整调度材料 = true;
                触发条件子需求复核已加锁();
            } else if (!已读取 && 返回.调度材料
                && 返回.调度材料->状态 != 新任务调度读取状态::资源失败) {
                // 结构失败没有完整新视图，冻结旧普通候选；结果回流不受阻。
                调度材料.clear();
                已有完整调度材料 = false;
            }
            std::erase_if(活动工作, [&](const auto& 项) {
                return 项.请求.工作编号 == 占用.请求.工作编号;
            });
            const auto 此刻 = std::chrono::steady_clock::now();
            下一调度时点 = 此刻 + 等待时长(配置.调度复核间隔毫秒);
            最早事件调度时点 = 下一调度时点;
            if (返回.调度材料) {
                最近调度读取状态 = 返回.调度材料->状态;
                最近调度异常任务 = std::move(异常任务);
            }
            if (诊断变化) {
                try {
                    std::wostringstream 文本;
                    文本 << L"调度扫描读取状态=" << static_cast<int>(*最近调度读取状态)
                        << L"；完整视图=" << 已读取 << L"；工作并发上限=" << 新任务工作并发上限
                        << L"；结构异常任务=";
                    for (const auto 任务 : 最近调度异常任务) 文本 << 任务.值 << L",";
                    (void)记录运行日志(L"任务管理线程", L"任务调度扫描", 文本.str());
                } catch (...) {} // 诊断不能改变已ack工作或调度资格。
            }
            变化.notify_all();
        } catch (const std::bad_alloc&) {
            std::lock_guard 锁(互斥);
            下一工作消费时点 = std::chrono::steady_clock::now() + 等待时长(配置.资源重试间隔毫秒);
        } catch (...) { 报告工程停止("调度扫描结果无法读取"); }
    }

    筹办派发状态 尝试派发调度扫描() noexcept {
        try {
            std::unique_lock 锁(互斥);
            if (停止 || 已完成) return 筹办派发状态::已停止;
            if (服务结算来源排空中) return 筹办派发状态::无可派发任务;
            const auto 当前 = std::chrono::steady_clock::now();
            if (std::any_of(活动工作.begin(), 活动工作.end(), [](const auto& 项) {
                    return 项.请求.种类 == 新任务工作种类::读取任务调度材料;
                }) || (!待派调度工作 && 当前 < 下一调度时点
                    && (!调度待触发 || 当前 < 最早事件调度时点)))
                return 筹办派发状态::无可派发任务;
            if (活动工作.size() >= 配置.邮箱容量) return 筹办派发状态::资源暂不可用;
            活动工作.reserve(活动工作.size() + 1);
            if (!待派调度工作) {
                if (!下一工作编号 || 下一工作编号 == (std::numeric_limits<std::uint64_t>::max)())
                    return 筹办派发状态::内部错误;
                新任务工作请求 请求;
                请求.工作编号 = 下一工作编号++;
                请求.种类 = 新任务工作种类::读取任务调度材料;
                待派调度工作 = 请求;
            }
            工作占用 待接纳{*待派调度工作, {}};
            调度待触发 = false; // 只消费提交前事件；解锁后新事件不能被覆盖。
            锁.unlock();
            const auto 接收 = 工作线程->提交工作(待接纳.请求);
            锁.lock();
            if (接收.状态 == 新任务工作接收状态::已接收 || 接收.状态 == 新任务工作接收状态::精确重复) {
                待接纳.结果 = 接收.处理结果;
                活动工作.push_back(std::move(待接纳));
                待派调度工作.reset();
                return 筹办派发状态::已派发;
            }
            调度待触发 = true;
            if (接收.状态 == 新任务工作接收状态::邮箱已满
                || 接收.状态 == 新任务工作接收状态::资源失败
                || 接收.状态 == 新任务工作接收状态::任务已占用) return 筹办派发状态::资源暂不可用;
            return 接收.状态 == 新任务工作接收状态::已停止接收
                ? 筹办派发状态::已停止 : 筹办派发状态::内部错误;
        } catch (const std::bad_alloc&) { return 筹办派发状态::资源暂不可用; }
        catch (...) { return 筹办派发状态::内部错误; }
    }

    void 推进等待复核() noexcept {
        try {
            等待复核记录 记录;
            {
                std::lock_guard 锁(互斥);
                if (停止 || 已完成) return;
                const auto 位置 = std::find_if(等待复核组.begin(), 等待复核组.end(),
                    [&](const auto& 项) {
                        const auto 任务值 = 项.second.定位.定位.任务节点.值;
                        return !项.second.已终结 && (!项.second.已反馈 || 项.second.决议)
                            && !学习复核占用.contains(任务值) && !任务有执行保护已加锁(任务值);
                    });
                if (位置 == 等待复核组.end()) return;
                记录 = 位置->second;
            }
            const auto 编号 = 记录.定位.复核编号;
            const auto 任务值 = 记录.定位.定位.任务节点.值;
            const auto 终结 = [&](bool 硬错误) {
                std::lock_guard 锁(互斥);
                if (硬错误) 阻塞任务.insert(任务值);
                等待复核组.at(编号).已终结 = true;
                等待复核占用.erase(任务值);
            };
            {
                std::lock_guard 锁(互斥);
                if (学习复核占用.contains(任务值) || 任务有执行保护已加锁(任务值)) return;
            }
            if (!记录.决议) {
                等待复核接收函数 接收;
                { std::lock_guard 锁(互斥); 接收 = 自我等待反馈接收; }
                const auto 状态 = 接收 ? 接收(记录.定位) : 新任务等待复核反馈接收状态::尚未配置;
                if (状态 == 新任务等待复核反馈接收状态::已接收
                    || 状态 == 新任务等待复核反馈接收状态::精确重复) {
                    std::lock_guard 锁(互斥);
                    等待复核组.at(编号).已反馈 = true;
                } else if (状态 != 新任务等待复核反馈接收状态::资源失败
                    && 状态 != 新任务等待复核反馈接收状态::邮箱已满
                    && 状态 != 新任务等待复核反馈接收状态::尚未启动) 终结(true);
                return;
            }
            if (记录.决议->状态 != 新自我等待复核状态::允许重新筹办) {
                if (记录.决议->状态 == 新自我等待复核状态::等待材料
                    || 记录.决议->状态 == 新自我等待复核状态::当前已满足
                    || 记录.决议->状态 == 新自我等待复核状态::需求非活动
                    || 记录.决议->状态 == 新自我等待复核状态::等待同次执行比较) {
                    std::lock_guard 锁(互斥);
                    等待目标复核.insert(任务值);
                }
                终结(记录.决议->状态 == 新自我等待复核状态::需核查);
                return;
            }
            { std::lock_guard 锁(互斥); 等待复核组.at(编号).准备已尝试 = true; }
            const auto 准备 = 任务服务->准备等待任务重新筹办(记录.定位.定位);
            if (准备 == 新任务操作状态::资源失败) return;
            if (准备 != 新任务操作状态::已修改 && 准备 != 新任务操作状态::无变化) {
                终结(准备 != 新任务操作状态::任务状态不允许 && 准备 != 新任务操作状态::任务不存在);
                return;
            }
            {
                std::lock_guard 锁(互斥);
                最近筹办结果.erase(任务值);
                待反馈任务.erase(任务值);
                最近自我筹办裁决.erase(任务值);
                筹办阻塞任务.erase(任务值);
                等待来源.erase(任务值);
                上次等待材料.erase(任务值);
                等待目标复核.erase(任务值);
                筹办资源重试.erase(任务值);
                调度待触发 = true;
            }
            终结(false);
        } catch (const std::bad_alloc&) {
            // 原定位和已经接受的决定保留，重试不得签新动作。
        } catch (...) { 报告工程停止("等待复核交接内部错误"); }
    }

    筹办派发状态 尝试派发执行工作() noexcept {
        try {
            工作占用 待接纳;
            {
                std::lock_guard 锁(互斥);
                if (停止 || 已完成) return 筹办派发状态::已停止;
                if (!已有完整调度材料) return 筹办派发状态::无可派发任务;
                活动工作.reserve(活动工作.size() + 1);
                for (const auto& [任务值, 记录] : 最近自我筹办裁决) {
                    if (!记录.已完成接收 || 记录.执行工作已派发 || !记录.执行工作
                        || 任务有执行保护已加锁(任务值)
                        || 学习占用或旧筹办已失效已加锁(记录.裁决.定位)) continue;
                    if (记录.执行工作->种类 != 新任务工作种类::执行本能动作)
                        return 筹办派发状态::内部错误;
                    待接纳.请求 = *记录.执行工作;
                    break;
                }
            }
            if (!待接纳.请求.工作编号) return 筹办派发状态::无可派发任务;
            const auto 接收 = 工作线程->提交工作(待接纳.请求);
            if (接收.状态 == 新任务工作接收状态::已接收
                || 接收.状态 == 新任务工作接收状态::精确重复) {
                std::lock_guard 锁(互斥);
                待接纳.结果 = 接收.处理结果;
                活动工作.push_back(std::move(待接纳));
                const auto& 请求 = 活动工作.back().请求;
                调度材料.erase(请求.任务节点.值);
                const auto 位置 = 最近自我筹办裁决.find(请求.任务节点.值);
                if (位置 == 最近自我筹办裁决.end()
                    || !位置->second.执行工作 || *位置->second.执行工作 != 请求)
                    return 筹办派发状态::内部错误;
                位置->second.执行工作已派发 = true;
                最近执行结果.erase(请求.任务节点.值);
                变化.notify_all();
                return 筹办派发状态::已派发;
            }
            if (接收.状态 == 新任务工作接收状态::邮箱已满
                || 接收.状态 == 新任务工作接收状态::任务已占用
                || 接收.状态 == 新任务工作接收状态::资源失败)
                return 筹办派发状态::资源暂不可用;
            return 接收.状态 == 新任务工作接收状态::已停止接收
                ? 筹办派发状态::已停止 : 筹办派发状态::内部错误;
        } catch (const std::bad_alloc&) { return 筹办派发状态::资源暂不可用; }
        catch (...) { return 筹办派发状态::内部错误; }
    }

    筹办派发状态 尝试派发筹办工作() noexcept {
        try {
            std::vector<新任务调度定位> 就绪;
            {
                std::lock_guard 锁(互斥);
                if (停止 || 已完成) return 筹办派发状态::已停止;
                if (!已有完整调度材料) return 筹办派发状态::无可派发任务;
                if (活动工作.size() >= 配置.邮箱容量) return 筹办派发状态::资源暂不可用;
                活动工作.reserve(活动工作.size() + 1);
                for (const auto& [任务值, 项] : 调度材料) {
                    (void)任务值;
                    if (项.状态 == 新任务调度项状态::普通可筹办) 就绪.push_back(项.定位);
                }
            }
            for (const auto 工作种类 : {
                    新任务工作种类::检查条件,
                    新任务工作种类::查找方法}) {
                // 资源失败沿原工作号重入，不伪造新的筹办尝试或丢掉已形成材料。
                std::optional<工作占用> 重试;
                {
                    std::lock_guard 锁(互斥);
                    const auto 当前时点 = std::chrono::steady_clock::now();
                    if (当前时点 >= 下一工作消费时点) {
                        for (const auto& [任务值, 请求] : 筹办资源重试) {
                            if (请求.种类 != 工作种类 || 任务有执行保护已加锁(任务值)
                                || 有服务结算来源待处理已加锁(任务值)
                                || 学习复核占用.contains(任务值) || 等待复核占用.contains(任务值)) continue;
                            重试 = 工作占用{请求, {}};
                            break;
                        }
                    }
                }
                if (重试) {
                    const auto 接收 = 工作线程->提交工作(重试->请求);
                    std::lock_guard 锁(互斥);
                    if (接收.状态 == 新任务工作接收状态::已接收 || 接收.状态 == 新任务工作接收状态::精确重复) {
                        const auto 任务值 = 重试->请求.任务节点.值;
                        重试->结果 = 接收.处理结果;
                        活动工作.push_back(std::move(*重试));
                        筹办资源重试.erase(任务值);
                        调度材料.erase(任务值);
                        return 筹办派发状态::已派发;
                    }
                    if (接收.状态 == 新任务工作接收状态::资源失败
                        || 接收.状态 == 新任务工作接收状态::邮箱已满
                        || 接收.状态 == 新任务工作接收状态::任务已占用) return 筹办派发状态::资源暂不可用;
                    return 接收.状态 == 新任务工作接收状态::已停止接收
                        ? 筹办派发状态::已停止 : 筹办派发状态::内部错误;
                }
                for (const auto& 定位 : 就绪) {
                    const auto 任务节点 = 定位.任务节点;
                    if (工作种类 == 新任务工作种类::检查条件
                        ? 定位.任务状态 != 新任务状态::待检查条件
                        : (定位.任务状态 != 新任务状态::已建立 && 定位.任务状态 != 新任务状态::待查找方法)) continue;
                    {
                        std::lock_guard 锁(互斥);
                        if (停止 || 已完成 || 阻塞任务.contains(任务节点.值)
                            || 有服务结算来源待处理已加锁(任务节点.值)
                            || 筹办阻塞任务.contains(任务节点.值)
                            || 学习复核占用.contains(任务节点.值)
                            || 等待复核占用.contains(任务节点.值)
                            || 筹办资源重试.contains(任务节点.值)
                            || std::any_of(活动工作.begin(), 活动工作.end(),
                                [&](const auto& 占用) {
                                    return 占用.请求.任务节点 == 任务节点;
                                })) continue;
                    }
                    if (工作种类 == 新任务工作种类::查找方法) {
                        const auto 准备 = 任务服务->准备任务查找方法(任务节点);
                        if (准备 != 新任务操作状态::已修改
                            && 准备 != 新任务操作状态::无变化) {
                            if (准备 != 新任务操作状态::任务状态不允许
                                && 准备 != 新任务操作状态::资源失败) {
                                std::lock_guard 锁(互斥);
                                筹办阻塞任务.insert(任务节点.值);
                            }
                            continue;
                        }
                    }

                    工作占用 待接纳;
                    新任务工作请求 请求;
                    {
                        std::lock_guard 锁(互斥);
                        if (停止 || 已完成) return 筹办派发状态::已停止;
                        if (有服务结算来源待处理已加锁(任务节点.值)) continue;
                        if (!下一工作编号
                            || 下一工作编号
                                == (std::numeric_limits<std::uint64_t>::max)()) {
                            阻塞任务.insert(任务节点.值);
                            return 筹办派发状态::内部错误;
                        }
                        if (活动工作.size() >= 配置.邮箱容量) return 筹办派发状态::资源暂不可用;
                        if (待派筹办工作 && (待派筹办工作->任务节点 != 任务节点
                            || 待派筹办工作->种类 != 工作种类)) {
                            // 原未接纳请求所属任务已离开就绪集合，不能堵住其它T。
                            const auto 旧 = std::find_if(就绪.begin(), 就绪.end(), [&](const auto& 项) {
                                return 项.任务节点 == 待派筹办工作->任务节点;
                            });
                            if (旧 != 就绪.end() && !等待复核占用.contains(旧->任务节点.值)
                                && !有服务结算来源待处理已加锁(旧->任务节点.值)
                                && !学习复核占用.contains(旧->任务节点.值)
                                && !筹办阻塞任务.contains(旧->任务节点.值)
                                && !阻塞任务.contains(旧->任务节点.值)) continue;
                            待派筹办工作.reset();
                        }
                        if (!待派筹办工作) {
                            请求.工作编号 = 下一工作编号++;
                            请求.种类 = 工作种类;
                            请求.任务节点 = 任务节点;
                            待派筹办工作 = 请求;
                        } else 请求 = *待派筹办工作;
                        待接纳.请求 = 请求;
                    }
                    const auto 接收 = 工作线程->提交工作(请求);
                    if (接收.状态 == 新任务工作接收状态::已接收
                        || 接收.状态 == 新任务工作接收状态::精确重复) {
                        std::lock_guard 锁(互斥);
                        待接纳.结果 = 接收.处理结果;
                        活动工作.push_back(std::move(待接纳));
                        待派筹办工作.reset();
                        调度材料.erase(任务节点.值);
                        等待来源.erase(任务节点.值);
                        上次等待材料.erase(任务节点.值);
                        最近筹办结果.erase(任务节点.值);
                        待反馈任务.erase(任务节点.值);
                        待结果反馈任务.erase(任务节点.值);
                        最近自我筹办裁决.erase(任务节点.值);
                        变化.notify_all();
                        return 筹办派发状态::已派发;
                    }
                    if (接收.状态 != 新任务工作接收状态::邮箱已满
                        && 接收.状态 != 新任务工作接收状态::任务已占用
                        && 接收.状态 != 新任务工作接收状态::资源失败) {
                        std::lock_guard 锁(互斥);
                        待派筹办工作.reset();
                        if (接收.状态 == 新任务工作接收状态::已停止接收)
                            阻塞任务.insert(任务节点.值);
                        else 筹办阻塞任务.insert(任务节点.值);
                    }
                    if (接收.状态 == 新任务工作接收状态::已停止接收) {
                        return 筹办派发状态::已停止;
                    }
                    if (接收.状态 == 新任务工作接收状态::邮箱已满
                        || 接收.状态 == 新任务工作接收状态::任务已占用
                        || 接收.状态 == 新任务工作接收状态::资源失败) {
                        return 筹办派发状态::资源暂不可用;
                    }
                }
            }
            return 筹办派发状态::无可派发任务;
        } catch (const std::bad_alloc&) {
            return 筹办派发状态::资源暂不可用;
        } catch (...) {
            return 筹办派发状态::内部错误;
        }
    }

    void 处理已完成结果检测(const 工作占用& 占用) noexcept {
        try {
            const auto& 返回 = 占用.结果.get();
            if (返回.原请求 != 占用.请求 || !占用.请求.本能结果检测请求)
                throw std::logic_error("result detection reply identity mismatch");
            const auto 定位 = *占用.请求.本能结果检测请求;
            const auto t = 定位.任务节点.值;
            const bool 取消 = 返回.状态 == 新任务工作处理状态::停止取消;
            新任务执行处理结果 处理;
            处理.原工作 = 占用.请求;
            if (!取消) {
                if (返回.状态 != 新任务工作处理状态::本能结果检测已返回
                    || !返回.本能结果检测 || 返回.本能结果检测->原请求 != 定位
                    || 返回.本能执行 || 返回.本能结果获取 || 返回.条件子需求提议)
                    throw std::logic_error("result detection reply shape mismatch");
                const auto& 检测 = *返回.本能结果检测;
                const auto& 回读 = 检测.正式读回;
                switch (检测.状态) {
                case 新任务结果检测状态::等待结果材料:
                    if (回读.状态 != 新任务治理执行读回状态::已读回等待观察
                        && 回读.状态 != 新任务治理执行读回状态::已读回等待评估)
                        throw std::logic_error("waiting reply has no pending execution");
                    break;
                case 新任务结果检测状态::已完成影响评估:
                    if (回读.状态 != 新任务治理执行读回状态::已读回影响评估 || !回读.影响评估)
                        throw std::logic_error("completed detection has no assessment");
                    break;
                case 新任务结果检测状态::定位已变化:
                    if (回读.状态 != 新任务治理执行读回状态::执行不在当前位)
                        throw std::logic_error("stale detection readback mismatch");
                    break;
                case 新任务结果检测状态::正式记录不存在:
                    if (回读.状态 != 新任务治理执行读回状态::任务或执行不存在)
                        throw std::logic_error("missing detection readback mismatch");
                    break;
                default: throw std::logic_error("unknown detection status");
                }
                if (检测.状态 == 新任务结果检测状态::等待结果材料
                    || 检测.状态 == 新任务结果检测状态::已完成影响评估) {
                    if (!回读.任务 || !回读.执行
                        || 回读.任务->节点 != 定位.任务节点
                        || 回读.任务->最近执行实例节点 != 定位.执行实例节点
                        || 回读.执行->节点 != 定位.执行实例节点
                        || 回读.执行->任务节点 != 定位.任务节点)
                        throw std::logic_error("detection material belongs to another execution");
                }
                处理.工作结果 = 返回;
                处理.独立读回 = 任务服务->读回任务执行(定位.任务节点, 定位.执行实例节点);
                switch (处理.独立读回->状态) {
                case 新任务治理执行读回状态::已读回等待观察:
                case 新任务治理执行读回状态::已读回等待评估:
                    处理.状态 = 新任务执行处理状态::动作已发出等待结果;
                    break;
                case 新任务治理执行读回状态::已读回影响评估:
                    if (!处理.独立读回->影响评估)
                        throw std::logic_error("independent assessment missing");
                    处理.状态 = 新任务执行处理状态::已完成影响评估;
                    break;
                case 新任务治理执行读回状态::执行不在当前位:
                case 新任务治理执行读回状态::任务或执行不存在:
                    处理.状态 = 新任务执行处理状态::任务状态已变化;
                    break;
                default: throw std::logic_error("independent result readback failed");
                }
            }

            std::unique_lock 锁(互斥);
            const auto 匹配等待 = [&]() {
                const auto p = 结果等待任务.find(t);
                return p != 结果等待任务.end()
                    && p->second.执行实例节点 == 定位.执行实例节点
                    && p->second.原检测工作 == 占用.请求 && p->second.已派发;
            };
            const bool 当前交接 = 匹配等待();
            const bool 等待 = !取消 && 处理.状态 == 新任务执行处理状态::动作已发出等待结果;
            const bool 完成 = !取消 && 处理.状态 == 新任务执行处理状态::已完成影响评估;
            // 所有可能分配的交接位置先准备，ack之后只移动/清除现存位置。
            if (当前交接 && (等待 || 完成)) {
                最近执行结果.try_emplace(t);
                if (完成) 待结果反馈任务.insert(t);
            }
            锁.unlock();
            if (工作线程->确认结果已接收(占用.请求.工作编号, 定位.任务节点)
                != 新任务工作操作状态::已完成)
                throw std::logic_error("result detection acknowledgement failed");
            锁.lock();
            std::erase_if(活动工作, [&](const auto& 项) { return 项.请求 == 占用.请求; });
            if (!当前交接 || !匹配等待()) return; // 迟到回执不能清掉新的X。
            auto 位置 = 结果等待任务.find(t);
            if (取消) {
                位置->second.原检测工作.reset();
                位置->second.已派发 = false;
            } else if (等待) {
                auto& 记录 = 位置->second;
                记录.原检测工作.reset();
                记录.已派发 = false;
                记录.待检测 = 记录.在途期间再次唤醒;
                记录.在途期间再次唤醒 = false;
                记录.下次检测时点 = std::chrono::steady_clock::now()
                    + 等待时长(配置.资源重试间隔毫秒);
                最近执行结果.at(t) = std::move(处理);
            } else if (完成) {
                最近执行结果.at(t) = std::move(处理);
                结果等待任务.erase(位置);
            } else {
                结果等待任务.erase(位置);
                const auto 旧 = 最近执行结果.find(t);
                if (旧 != 最近执行结果.end()) {
                    const auto& 旧结果 = 旧->second;
                    const bool 同X = (旧结果.独立读回 && 旧结果.独立读回->执行
                            && 旧结果.独立读回->执行->节点 == 定位.执行实例节点)
                        || (旧结果.原工作.本能结果检测请求
                            && 旧结果.原工作.本能结果检测请求->执行实例节点 == 定位.执行实例节点);
                    if (同X) {
                        待结果反馈任务.erase(t);
                        const auto 后继 = 最近任务后继决议.find(t);
                        if (后继 != 最近任务后继决议.end()
                            && 后继->second.执行实例节点 == 定位.执行实例节点)
                            最近任务后继决议.erase(后继);
                        最近执行结果.erase(旧);
                    }
                }
            }
            变化.notify_all();
        } catch (...) {
            工程异常终止(L"新_任务管理线程类.cpp", __LINE__,
                L"处理已完成结果检测", L"结果检测交接、正式读回或确认发生工程错误");
        }
    }

    void 处理已完成执行工作(const 工作占用& 占用) noexcept {
        新任务执行处理结果 处理;
        bool 零动作已回筹办 = false;
        bool 旧执行回执已失效 = false;
        bool 零动作定位失效 = false;
        try {
            处理.原工作 = 占用.请求;
            处理.工作结果 = 占用.结果.get();
            if (处理.工作结果->原请求 != 占用.请求
                || 处理.工作结果->条件子需求提议) {
                处理.状态 = 新任务执行处理状态::内部错误;
            } else if (处理.工作结果->状态 == 新任务工作处理状态::停止取消) {
                处理.状态 = 新任务执行处理状态::停止取消;
            } else if (占用.请求.种类 == 新任务工作种类::执行本能动作
                && 处理.工作结果->本能执行) {
                const auto& 执行结果 = *处理.工作结果->本能执行;
                if (!占用.请求.本能执行请求
                    || 执行结果.原请求 != *占用.请求.本能执行请求
                    || 执行结果.原请求.任务节点 != 占用.请求.任务节点
                    || !执行结果.原请求.完整()
                    || (执行结果.零动作恢复.has_value()
                        != (执行结果.状态 == 新任务治理本能执行状态::零动作已回筹办)))
                    throw std::logic_error("action reply differs from its original request");
                if (执行结果.执行开始
                    && 执行结果.执行开始->执行实例节点) {
                    处理.独立读回 = 任务服务->读回任务执行(
                        占用.请求.任务节点,
                        *执行结果.执行开始->执行实例节点);
                }
                switch (执行结果.状态) {
                case 新任务治理本能执行状态::工程停止:
                    处理.状态 = 新任务执行处理状态::工程停止;
                    break;
                case 新任务治理本能执行状态::动作已发出且结果材料已具备:
                    if (!处理.独立读回
                        || 处理.独立读回->状态
                            != 新任务治理执行读回状态::已读回等待观察
                        || !执行结果.执行开始
                        || !执行结果.执行开始->执行实例节点
                        || !执行结果.方法调用
                        || !执行结果.方法调用->本次条件结果对
                        || 执行结果.方法调用->本次条件结果对->结果.
                            已形成状态节点组.empty()) {
                        处理.状态 = 新任务执行处理状态::独立读回失败;
                        break;
                    }
                    处理.状态 = 新任务执行处理状态::动作已发出等待结果;
                    break;
                case 新任务治理本能执行状态::动作已发出等待结果:
                    处理.状态 = 处理.独立读回
                            && 处理.独立读回->状态
                                == 新任务治理执行读回状态::已读回等待观察
                        ? 新任务执行处理状态::动作已发出等待结果
                        : 新任务执行处理状态::独立读回失败;
                    break;
                case 新任务治理本能执行状态::执行依赖未就绪:
                case 新任务治理本能执行状态::动作未进入:
                    // 没有正式恢复材料的旧拒绝不能解释为可重试的业务条件。
                    处理.状态 = 新任务执行处理状态::工程停止;
                    break;
                case 新任务治理本能执行状态::零动作已回筹办: {
                    const auto& 恢复 = *执行结果.零动作恢复;
                    if (恢复.原最近方法节点.has_value() != 恢复.原最近执行实例节点.has_value()
                        || (恢复.原最近方法节点 && !恢复.原最近方法节点->值)
                        || (恢复.原最近执行实例节点 && !恢复.原最近执行实例节点->值)
                        || 执行结果.动作输出记录状态)
                        throw std::logic_error("invalid zero-action recovery receipt");
                    std::optional<新任务信息> 当前任务;
                    if (恢复.已撤销执行实例节点) {
                        if (!恢复.已撤销执行实例节点->值
                            || 恢复.已撤销执行实例节点 == 恢复.原最近执行实例节点
                            || !执行结果.执行开始
                            || 执行结果.执行开始->状态 != 新任务操作状态::已开始执行
                            || 执行结果.执行开始->执行实例节点 != 恢复.已撤销执行实例节点
                            || (执行结果.未进入动作撤销状态 != 新任务操作状态::已修改
                                && 执行结果.未进入动作撤销状态 != 新任务操作状态::精确重复)
                            || !处理.独立读回
                            || 处理.独立读回->状态 != 新任务治理执行读回状态::任务或执行不存在
                            || 处理.独立读回->执行 || !处理.独立读回->任务)
                            throw std::logic_error("empty execution removal not confirmed");
                        当前任务 = 处理.独立读回->任务;
                        if (执行结果.方法调用) {
                            const auto& 调用 = *执行结果.方法调用;
                            if (调用.状态 == 新方法本能调用状态::已调用 && 调用.本次条件结果对) {
                                const auto& r = 调用.本次条件结果对->结果;
                                if (r.状态 != 新本能函数执行状态::参数不匹配
                                    || r.动作阶段 != 新本能函数动作阶段::未进入动作
                                    || r.结果取得状态 != 新本能函数结果取得状态::未取得
                                    || r.动作动态节点 || !r.已形成状态节点组.empty()
                                    || !r.已形成动态节点组.empty() || !r.已形成二次特征结果组.empty()
                                    || !r.建议观察特征节点组.empty()
                                    || r.服务需求信息包 || r.服务对象观察材料)
                                    throw std::logic_error("zero-action reply contains effects or engineering failure");
                            } else if (调用.状态 != 新方法本能调用状态::方法停用 || 调用.本次条件结果对) {
                                throw std::logic_error("invocation failure cannot prove zero action");
                            }
                        }
                    } else {
                        if ((执行结果.执行开始 && 执行结果.执行开始->执行实例节点)
                            || 执行结果.方法调用 || 执行结果.未进入动作撤销状态)
                            throw std::logic_error("pre-start recovery contains an execution");
                        当前任务 = 任务服务->获取任务(占用.请求.任务节点);
                    }
                    if (!当前任务 || 当前任务->节点 != 占用.请求.任务节点)
                        throw std::logic_error("recovered task cannot be read");
                    零动作已回筹办 = 当前任务->当前状态 == 新任务状态::待查找方法
                        && 当前任务->最近方法节点 == 恢复.原最近方法节点
                        && 当前任务->最近执行实例节点 == 恢复.原最近执行实例节点;
                    // 正式任务已前进时，只接收旧工作，不能清理新执行的材料。
                    旧执行回执已失效 = !零动作已回筹办;
                    处理.状态 = 零动作已回筹办 ? 新任务执行处理状态::动作未进入
                        : 新任务执行处理状态::任务状态已变化;
                    break;
                }
                case 新任务治理本能执行状态::任务状态已变化:
                    处理.状态 = 新任务执行处理状态::任务状态已变化;
                    旧执行回执已失效 = true;
                    break;
                case 新任务治理本能执行状态::定位已变化: {
                    // 这是动作前复核不再成立，不是执行失败，也不能重放旧请求。
                    if ((执行结果.执行开始 && 执行结果.执行开始->执行实例节点)
                        || 执行结果.方法调用)
                        throw std::logic_error("changed pre-action locator contains an action");
                    const auto 当前任务 = 任务服务->获取任务(占用.请求.任务节点);
                    if (!当前任务)
                        throw std::logic_error("pre-action task cannot be read");
                    零动作定位失效 = true;
                    处理.状态 = 新任务执行处理状态::任务状态已变化;
                    break;
                }
                default:
                    处理.状态 = 新任务执行处理状态::工作未完成;
                    break;
                }
            } else if (占用.请求.种类 == 新任务工作种类::获取本能结果
                && 占用.请求.本能结果获取请求
                && 处理.工作结果->本能结果获取) {
                const auto& 获取结果 = *处理.工作结果->本能结果获取;
                处理.独立读回 = 任务服务->读回任务执行(
                    占用.请求.任务节点,
                    占用.请求.本能结果获取请求->执行实例节点);
                if (获取结果.状态
                        == 新任务治理结果获取状态::已完成影响评估) {
                    处理.状态 = 处理.独立读回
                            && 处理.独立读回->状态
                                == 新任务治理执行读回状态::已读回影响评估
                        ? 新任务执行处理状态::已完成影响评估
                        : 新任务执行处理状态::独立读回失败;
                } else if (获取结果.状态 == 新任务治理结果获取状态::等待结果材料
                    && 处理.独立读回
                    && (处理.独立读回->状态 == 新任务治理执行读回状态::已读回等待观察
                        || 处理.独立读回->状态 == 新任务治理执行读回状态::已读回等待评估)) {
                    处理.状态 = 新任务执行处理状态::动作已发出等待结果;
                } else if (获取结果.状态
                        == 新任务治理结果获取状态::任务状态已变化) {
                    处理.状态 = 新任务执行处理状态::任务状态已变化;
                } else {
                    处理.状态 = 新任务执行处理状态::工作未完成;
                }
            } else {
                处理.状态 = 新任务执行处理状态::内部错误;
            }
        } catch (...) {
            处理.状态 = 新任务执行处理状态::内部错误;
        }

        if (处理.状态 == 新任务执行处理状态::内部错误
            || 处理.状态 == 新任务执行处理状态::独立读回失败
            || 处理.状态 == 新任务执行处理状态::工作未完成
            || 处理.状态 == 新任务执行处理状态::工程停止)
            工程异常终止(L"新_任务管理线程类.cpp", __LINE__,
                L"处理已完成执行工作", L"动作或结果工作工程失败，不转为业务等待");
        try {
        bool 是工程停止 = false;
        // 先准备可能分配的交接位置，再确认工作；不能丢掉已发生动作的future。
        try {
            std::lock_guard 锁(互斥);
            const auto t = 占用.请求.任务节点.值;
            if (!旧执行回执已失效) 最近执行结果.try_emplace(t);
            if (处理.状态 == 新任务执行处理状态::动作已发出等待结果) {
                if (!处理.独立读回 || !处理.独立读回->执行)
                    throw std::logic_error("pending action has no execution");
                登记结果等待已加锁(占用.请求.任务节点, 处理.独立读回->执行->节点);
            } else if (处理.状态 == 新任务执行处理状态::已完成影响评估) {
                待结果反馈任务.insert(t);
            }
        } catch (...) {
            工程异常终止(L"新_任务管理线程类.cpp", __LINE__,
                L"处理已完成执行工作", L"动作回执交接位置准备失败");
        }
        {
            std::lock_guard 锁(互斥);
            // 结算动作后的工程失败不得静默留在等待，也不得交给自我重筹办。
            // 原始工程停止不能被接收确认失败覆盖成普通非成功状态。
            const bool 是结算任务 = 服务值结算任务
                && *服务值结算任务 == 占用.请求.任务节点;
            if (是工程停止 || (是结算任务
                && (处理.状态 == 新任务执行处理状态::独立读回失败
                    || 处理.状态 == 新任务执行处理状态::工作未完成
                    || 处理.状态 == 新任务执行处理状态::内部错误
                    || 处理.状态 == 新任务执行处理状态::执行依赖未就绪))) {
                处理.状态 = 新任务执行处理状态::工程停止;
            }
            if (零动作定位失效 || 零动作已回筹办) {
                const auto t = 占用.请求.任务节点.值;
                const auto 裁决 = 最近自我筹办裁决.find(t);
                // 仅使本次零动作请求所属的筹办材料失效，不能清掉新裁决。
                if (裁决 != 最近自我筹办裁决.end()
                    && 裁决->second.执行工作 == 占用.请求) {
                    const auto 旧 = 最近筹办结果.find(t);
                    if (旧 != 最近筹办结果.end()
                        && 旧->second.原工作 == 裁决->second.裁决.定位.原工作) {
                        学习失效筹办工作.insert_or_assign(t, 旧->second.原工作.工作编号);
                        if (待派筹办工作 && *待派筹办工作 == 旧->second.原工作)
                            待派筹办工作.reset();
                        待反馈任务.erase(t);
                        最近筹办结果.erase(旧);
                        筹办阻塞任务.erase(t);
                    }
                    最近自我筹办裁决.erase(裁决);
                    零动作定位失效 = true;
                } else {
                    零动作定位失效 = false;
                }
                if (零动作已回筹办 || 零动作定位失效) {
                    for (const auto& w : 活动工作)
                        if (w.请求.种类 == 新任务工作种类::读取任务调度材料)
                            零动作失效调度工作.insert(w.请求.工作编号);
                    调度材料.clear(); 已有完整调度材料 = false;
                    待派调度工作.reset();
                    调度待触发 = true;
                }
            } else if (!旧执行回执已失效
                && 处理.状态 != 新任务执行处理状态::动作已发出等待结果
                && 处理.状态 != 新任务执行处理状态::已完成影响评估
                && 处理.状态 != 新任务执行处理状态::停止取消) {
                阻塞任务.insert(占用.请求.任务节点.值);
            }
            // 后继工作构造也可能失败；保持同一结算 X，不发布业务反馈。
            if (是结算任务 && 处理.状态 == 新任务执行处理状态::内部错误) {
                处理.状态 = 新任务执行处理状态::工程停止;
            }
            是工程停止 = 处理.状态 == 新任务执行处理状态::工程停止;
            const bool 需要结果反馈 = 处理.状态
                == 新任务执行处理状态::已完成影响评估;
            if (处理.状态 == 新任务执行处理状态::工程停止) {
                内部错误发生 = true;
                停止 = true;
            }
            const auto 最近 = 最近执行结果.find(占用.请求.任务节点.值);
            // 工作编号来自同一manager的单调分配器，迟到旧回执不能覆盖新回执。
            if (!旧执行回执已失效 && 最近 != 最近执行结果.end()
                && 最近->second.原工作.工作编号 <= 占用.请求.工作编号)
                最近->second = std::move(处理);
            if (需要结果反馈) {
                待结果反馈任务.insert(占用.请求.任务节点.值);
            }
            变化.notify_all();
        }
        if (是工程停止)
            throw std::logic_error("action handover requires engineering stop");
        if (工作线程->确认结果已接收(占用.请求.工作编号, 占用.请求.任务节点)
            != 新任务工作操作状态::已完成)
            throw std::logic_error("action reply acknowledgement failed");
        {
            std::lock_guard 锁(互斥);
            std::erase_if(活动工作, [&](const auto& 项) { return 项.请求 == 占用.请求; });
            变化.notify_all();
        }
        } catch (...) {
            工程异常终止(L"新_任务管理线程类.cpp", __LINE__,
                L"处理已完成执行工作", L"执行工作收束发生工程错误，保留既有事实");
        }
    }

    void 延后工作消费() noexcept {
        try {
            std::lock_guard 锁(互斥);
            下一工作消费时点 = std::chrono::steady_clock::now() + 等待时长(配置.资源重试间隔毫秒);
        } catch (...) { 报告工程停止("筹办结果资源延后失败"); }
    }

    // 来源重筹办不会取消worker。先取得旧future并确认回收，再丢弃其调度作用。
    bool 收回来源已替代工作(const 工作占用& 占用) noexcept {
        try {
            {
                std::lock_guard 锁(互斥);
                const bool 是旧扫描 = 服务结算来源排空中
                    && 占用.请求.种类 == 新任务工作种类::读取任务调度材料;
                const bool 是旧筹办 = 有服务结算来源待处理已加锁(占用.请求.任务节点.值)
                    && (占用.请求.种类 == 新任务工作种类::查找方法
                        || 占用.请求.种类 == 新任务工作种类::检查条件);
                if (!是旧扫描 && !是旧筹办) return false;
            }
            const auto& 返回 = 占用.结果.get();
            const bool 未发布资源失败 = 返回.方法召回 && !返回.方法召回->发布已尝试
                && (返回.方法召回->状态 == 新任务治理方法召回状态::资源失败
                    || (返回.方法召回->方法查询
                        && 返回.方法召回->方法查询->状态 == 新方法操作状态::资源失败));
            const bool 条件资源失败 = 返回.条件筹办
                && 返回.条件筹办->状态 == 新任务治理条件筹办状态::资源失败;
            const bool 缺少工作材料 = 返回.状态 != 新任务工作处理状态::停止取消
                && ((占用.请求.种类 == 新任务工作种类::查找方法 && !返回.方法召回)
                    || (占用.请求.种类 == 新任务工作种类::检查条件 && !返回.条件筹办
                        && !(返回.状态 == 新任务工作处理状态::组织能力暂不支持
                            && 返回.执行组织交付
                            && 返回.执行组织交付->状态 == 新任务执行组织状态::增补结构暂不支持))
                    || (占用.请求.种类 == 新任务工作种类::读取任务调度材料 && !返回.调度材料));
            if (返回.原请求 != 占用.请求
                || 缺少工作材料
                || 返回.状态 == 新任务工作处理状态::内部错误
                || (返回.状态 == 新任务工作处理状态::需核查 && !未发布资源失败 && !条件资源失败)
                || (返回.状态 == 新任务工作处理状态::查找未完成 && !未发布资源失败)
                || 返回.状态 == 新任务工作处理状态::候选发布未完成
                || 返回.状态 == 新任务工作处理状态::服务未就绪
                || (返回.条件筹办 && 条件筹办有结构错误(*返回.条件筹办))
                || (返回.方法召回 && 返回.方法召回->状态 == 新任务治理方法召回状态::结构不一致)
                || (返回.条件子需求提议 && 返回.条件子需求提议->状态 == 新任务条件子需求提议状态::结构不一致)
                || (返回.调度材料 && 返回.调度材料->状态 != 新任务调度读取状态::已读取
                    && 返回.调度材料->状态 != 新任务调度读取状态::资源失败)) {
                报告工程停止("来源重新筹办排空时发现旧工作工程异常");
                return true;
            }
            // 明确的资源失败没有业务结果；旧请求回收后由新的普通筹办再读，不升级为进程故障。
            const auto 确认 = 工作线程->确认结果已接收(
                占用.请求.工作编号, 占用.请求.任务节点);
            if (确认 != 新任务工作操作状态::已完成) {
                报告工程停止("来源重新筹办未能收回旧工作");
                return true;
            }
            std::lock_guard 锁(互斥);
            std::erase_if(活动工作, [&](const auto& 项) {
                return 项.请求.工作编号 == 占用.请求.工作编号;
            });
            调度待触发 = true;
            return true;
        } catch (...) {
            报告工程停止("来源重新筹办读取旧工作失败");
            return true;
        }
    }

    void 处理已完成工作(const 工作占用& 占用) noexcept {
        if (收回来源已替代工作(占用)) return;
        if (占用.请求.种类 == 新任务工作种类::读取任务调度材料) {
            处理调度扫描结果(占用);
            return;
        }
        if (占用.请求.种类 == 新任务工作种类::检测本能结果) {
            处理已完成结果检测(占用);
            return;
        }
        if (占用.请求.种类 == 新任务工作种类::执行本能动作
            || 占用.请求.种类 == 新任务工作种类::获取本能结果) {
            处理已完成执行工作(占用);
            return;
        }
        新任务筹办处理结果 处理;
        处理.原工作 = 占用.请求;
        try {
            处理.工作结果 = 占用.结果.get();
            const auto& 返回 = *处理.工作结果;
            bool 资源重入 = 返回.原请求 == 占用.请求 && (
                (返回.方法召回 && !返回.方法召回->发布已尝试
                    && (返回.方法召回->状态 == 新任务治理方法召回状态::资源失败
                        || (返回.方法召回->方法查询
                            && 返回.方法召回->方法查询->状态 == 新方法操作状态::资源失败)))
                || (返回.条件筹办 && (返回.条件筹办->状态 == 新任务治理条件筹办状态::资源失败
                    || 返回.条件筹办->方法选择状态 == 新任务方法选择状态::资源失败
                    || std::any_of(返回.条件筹办->方法筹办组.begin(), 返回.条件筹办->方法筹办组.end(),
                        [](const auto& 方法) { return 方法.状态 == 新任务候选方法筹办状态::资源失败
                            || std::any_of(方法.条件结果组.begin(), 方法.条件结果组.end(),
                                [](const auto& 配对) { return 配对.状态 == 新任务方法条件项核验状态::资源失败; }); })))
                || (返回.条件子需求提议 && 返回.条件子需求提议->状态 == 新任务条件子需求提议状态::资源失败));
            if (资源重入) {
                std::lock_guard 锁(互斥);
                if (!停止) 筹办资源重试.insert_or_assign(占用.请求.任务节点.值, 占用.请求);
                const auto 确认 = 工作线程->确认结果已接收(占用.请求.工作编号, 占用.请求.任务节点);
                if (确认 != 新任务工作操作状态::已完成) {
                    阻塞任务.insert(占用.请求.任务节点.值);
                    内部错误发生 = true;
                    停止 = true;
                    return;
                }
                std::erase_if(活动工作, [&](const auto& 项) { return 项.请求.工作编号 == 占用.请求.工作编号; });
                下一工作消费时点 = std::chrono::steady_clock::now() + 等待时长(配置.资源重试间隔毫秒);
                调度待触发 = true;
                return;
            }
            if (处理.工作结果->原请求 != 占用.请求) {
                处理.状态 = 新任务筹办处理状态::内部错误;
            } else if (返回.状态 == 新任务工作处理状态::组织能力暂不支持) {
                处理.状态 = 占用.请求.种类 == 新任务工作种类::检查条件
                    && 返回.执行组织交付
                    && 返回.执行组织交付->状态 == 新任务执行组织状态::增补结构暂不支持
                    && !返回.条件筹办 && !返回.条件子需求提议
                    ? 新任务筹办处理状态::组织能力暂不支持
                    : 新任务筹办处理状态::独立读回失败;
            } else if (占用.请求.种类 != 新任务工作种类::检查条件
                && 处理.工作结果->条件子需求提议) {
                处理.状态 = 新任务筹办处理状态::独立读回失败;
            } else if (占用.请求.种类 == 新任务工作种类::检查条件
                && 处理.工作结果->状态
                    == 新任务工作处理状态::已完成条件筹办) {
                处理.条件筹办 = 处理.工作结果->条件筹办;
                if (处理.条件筹办 && 处理.条件筹办->任务节点 != 占用.请求.任务节点) {
                    处理.状态 = 新任务筹办处理状态::独立读回失败;
                } else if (!处理.条件筹办
                    || 条件筹办有结构错误(*处理.条件筹办)
                    || 处理.条件筹办->状态
                        != 新任务治理条件筹办状态::已完成
                    || (!处理.条件筹办->候选已完整核验
                        && !条件筹办为业务等待(*处理.条件筹办))) {
                    处理.状态 = 新任务筹办处理状态::独立读回失败;
                } else {
                    处理.独立条件筹办 = 任务服务->筹办任务条件和参数(
                        占用.请求.任务节点);
                    if (处理.独立条件筹办->状态 == 新任务治理条件筹办状态::资源失败) {
                        延后工作消费();
                        return;
                    }
                    处理.状态 = 条件筹办有结构错误(*处理.独立条件筹办)
                        ? 新任务筹办处理状态::独立读回失败
                        : 处理.独立条件筹办->状态
                            == 新任务治理条件筹办状态::任务状态已变化
                        ? 新任务筹办处理状态::任务状态已变化
                        : (*处理.独立条件筹办 == *处理.条件筹办
                            && 处理.独立条件筹办->状态
                                == 新任务治理条件筹办状态::已完成
                            && (处理.独立条件筹办->候选已完整核验
                                || 条件筹办为业务等待(*处理.独立条件筹办))
                            ? 新任务筹办处理状态::条件筹办已完成
                            : (处理.独立条件筹办->状态 == 新任务治理条件筹办状态::已完成
                                ? 新任务筹办处理状态::任务状态已变化
                                : 新任务筹办处理状态::独立读回失败));
                    if (处理.状态 == 新任务筹办处理状态::条件筹办已完成) {
                        处理.独立子需求提议 = 任务服务->提议任务条件子需求(
                            占用.请求.任务节点);
                        if (处理.独立子需求提议->状态 == 新任务条件子需求提议状态::资源失败) {
                            延后工作消费();
                            return;
                        }
                        if (!处理.工作结果->条件子需求提议
                            || 处理.独立子需求提议->状态 == 新任务条件子需求提议状态::结构不一致) {
                            处理.状态 = 新任务筹办处理状态::独立读回失败;
                        } else if (*处理.独立子需求提议 != *处理.工作结果->条件子需求提议) {
                            处理.状态 = 新任务筹办处理状态::任务状态已变化;
                        }
                    }
                }
            } else if (处理.工作结果->状态
                    == 新任务工作处理状态::已发布有候选
                || 处理.工作结果->状态
                    == 新任务工作处理状态::已发布完整无候选) {
                if (!处理.工作结果->方法召回
                    || !处理.工作结果->方法召回->候选材料节点) {
                    处理.状态 = 新任务筹办处理状态::独立读回失败;
                } else {
                    处理.独立消费 = 任务服务->消费任务方法候选(
                        占用.请求.任务节点,
                        *处理.工作结果->方法召回->候选材料节点);
                    if (处理.独立消费->状态 == 新任务治理候选消费状态::资源失败
                        || 处理.独立消费->读回.状态 == 新任务治理候选读回状态::资源失败) {
                        延后工作消费();
                        return;
                    }
                    switch (处理.独立消费->状态) {
                    case 新任务治理候选消费状态::已进入条件检查:
                        处理.状态 = 新任务筹办处理状态::有候选并进入条件检查;
                        break;
                    case 新任务治理候选消费状态::已进入等待条件:
                        处理.状态 = 新任务筹办处理状态::完整无候选并进入等待;
                        break;
                    case 新任务治理候选消费状态::任务状态已变化:
                        处理.状态 = 新任务筹办处理状态::任务状态已变化;
                        break;
                    case 新任务治理候选消费状态::读回失败:
                    case 新任务治理候选消费状态::尚未初始化:
                        处理.状态 = 新任务筹办处理状态::独立读回失败;
                        break;
                    case 新任务治理候选消费状态::状态写入失败:
                    case 新任务治理候选消费状态::资源失败:
                        处理.状态 = 新任务筹办处理状态::状态推进失败;
                        break;
                    }
                }
            } else if (处理.工作结果->状态
                == 新任务工作处理状态::任务状态已变化) {
                处理.状态 = 新任务筹办处理状态::任务状态已变化;
            } else if (处理.工作结果->状态
                == 新任务工作处理状态::停止取消) {
                处理.状态 = 新任务筹办处理状态::停止取消;
            } else {
                处理.状态 = 新任务筹办处理状态::工作未完成;
            }
        } catch (const std::bad_alloc&) {
            延后工作消费();
            return;
        } catch (...) {
            处理.状态 = 新任务筹办处理状态::内部错误;
        }

        try {
        const auto 等待任务 = 处理.状态 == 新任务筹办处理状态::完整无候选并进入等待
            ? 任务服务->获取任务(占用.请求.任务节点) : std::optional<新任务信息>{};
        if (处理.状态 == 新任务筹办处理状态::完整无候选并进入等待 && !等待任务) {
            延后工作消费();
            return;
        }
        const bool 旧筹办已变化 = 处理.状态 == 新任务筹办处理状态::任务状态已变化;
        const bool 需要反馈 = 处理.状态 != 新任务筹办处理状态::有候选并进入条件检查
            && 处理.状态 != 新任务筹办处理状态::停止取消 && !旧筹办已变化;
        const bool 业务等待 = 处理.状态 == 新任务筹办处理状态::工作未完成
            && 处理.条件筹办 && 条件筹办为业务等待(*处理.条件筹办);
        const bool 技术失败 = (处理.状态 == 新任务筹办处理状态::工作未完成 && !业务等待)
            || 处理.状态 == 新任务筹办处理状态::独立读回失败
            || 处理.状态 == 新任务筹办处理状态::状态推进失败
            || 处理.状态 == 新任务筹办处理状态::内部错误;
        // 所有可分配的记录在ack之前预留，已接受工作不能因分配失败丢失future。
        {
            std::lock_guard 锁(互斥);
            最近筹办结果.try_emplace(占用.请求.任务节点.值);
            if (需要反馈) 待反馈任务.insert(占用.请求.任务节点.值);
            if (技术失败) 阻塞任务.insert(占用.请求.任务节点.值);
            if (占用.请求.种类 == 新任务工作种类::检查条件 && !旧筹办已变化)
                筹办阻塞任务.insert(占用.请求.任务节点.值);
            if (等待任务) 登记等待来源已加锁(*等待任务, 新任务等待原因::尚无方法, 占用.请求);
        }
        const auto 确认 = 工作线程->确认结果已接收(
            占用.请求.工作编号, 占用.请求.任务节点);
        std::lock_guard 锁(互斥);
        const auto 位置 = std::find_if(活动工作.begin(), 活动工作.end(),
            [&](const auto& 当前) {
                return 当前.请求.工作编号 == 占用.请求.工作编号;
            });
        if (确认 != 新任务工作操作状态::已完成) {
            处理.状态 = 新任务筹办处理状态::内部错误;
            内部错误发生 = true;
            停止 = true;
            return;
        }
        if (位置 != 活动工作.end()) 活动工作.erase(位置);
        最近筹办结果[占用.请求.任务节点.值] = std::move(处理);
        if (!需要反馈) 待反馈任务.erase(占用.请求.任务节点.值);
        调度待触发 = true;
        变化.notify_all();
        } catch (const std::bad_alloc&) { 延后工作消费(); }
        catch (...) { 报告工程停止("筹办工作结果交接失败"); }
    }

    bool 停止工作线程并收束() noexcept {
        if (!工作线程) return false;
        const auto 停止结果 = 工作线程->请求停止();
        const auto 等待结果 = 工作线程->等待停止(
            (std::numeric_limits<std::uint64_t>::max)());
        bool 完整 = (停止结果.状态 == 新任务工作操作状态::已完成
                || 停止结果.状态 == 新任务工作操作状态::无变化)
            && (等待结果.状态 == 新任务工作操作状态::已完成
                || 等待结果.状态 == 新任务工作操作状态::无变化);
        for (;;) {
            工作占用 占用;
            {
                std::lock_guard 锁(互斥);
                if (活动工作.empty()) break;
                占用 = 活动工作.front();
            }
            if (!占用.结果.valid()
                || 占用.结果.wait_for(std::chrono::milliseconds(0))
                    != std::future_status::ready) {
                完整 = false;
                break;
            }
            处理已完成工作(占用);
            {
                std::lock_guard 锁(互斥);
                if (std::any_of(活动工作.begin(), 活动工作.end(), [&](const auto& 项) {
                        return 项.请求.工作编号 == 占用.请求.工作编号;
                    })) {
                    完整 = false; // 收束失败保留证据，不在停止中无限重复同一ready项。
                    break;
                }
            }
        }
        {
            std::lock_guard 锁(互斥);
            if (!活动工作.empty()) 完整 = false;
        }
        return 完整;
    }

    // 每次只推进一个事件的一项；所有领域读取和self回调都在manager锁外。
    void 推进学习事件() noexcept {
        std::uint64_t 交接 = 0;
        新学习结束通知 通知;
        try {
            学习任务项 项;
            bool 需展开 = false;
            {
                std::lock_guard 锁(互斥);
                if (停止 || 已完成) return;
                if (当前学习交接 && 学习事件组.at(*当前学习交接).已终结)
                    当前学习交接.reset();
                const auto 位置 = 当前学习交接 ? 学习事件组.find(*当前学习交接)
                    : std::find_if(学习事件组.begin(), 学习事件组.end(),
                        [](const auto& 候选) { return !候选.second.已终结; });
                if (位置 == 学习事件组.end()) return;
                当前学习交接 = 位置->first; // 晚到的小编号不能抢占正在推进的批次。
                auto& 事件 = 位置->second;
                if (事件.可处理时点 > std::chrono::steady_clock::now()) return;
                交接 = 位置->first;
                通知 = 事件.通知;
                // 每次尝试先安排退避，异常和等待分支不形成紧循环。
                事件.可处理时点 = std::chrono::steady_clock::now()
                    + 等待时长(配置.资源重试间隔毫秒);
                需展开 = !事件.已展开;
                if (!需展开) {
                    const auto 待办 = std::find_if(事件.任务组.begin(), 事件.任务组.end(),
                        [](const auto& 当前) { return !当前.已终结; });
                    if (待办 == 事件.任务组.end()) {
                        事件.已终结 = true;
                        return;
                    }
                    项 = *待办;
                }
            }
            if (需展开) {
                const auto 方法 = 学习方法读取->获取方法(通知.方法节点);
                const bool 合法 = 方法 && 方法->节点 == 通知.方法节点
                    && !方法->来源方法节点 && !方法->实例根节点
                    && 方法->实现方式 == 新方法实现方式::本能
                    && 方法->本能函数登记名 && !方法->本能函数登记名->empty();
                新任务列表结果 相关;
                if (合法 && !方法->条件结果组.empty())
                    相关 = 任务服务->查询受方法学习影响任务(通知.方法节点);
                else if (合法) 相关.状态 = 新任务操作状态::已找到;
                if (合法 && 相关.状态 == 新任务操作状态::资源失败) return;
                std::vector<学习任务项> 新任务组;
                std::set<std::uint64_t> 新占用;
                if (合法 && 相关.状态 == 新任务操作状态::已找到) {
                    新任务组.reserve(相关.任务节点组.size());
                    for (const auto 任务 : 相关.任务节点组) {
                        if (新占用.insert(任务.值).second)
                            新任务组.push_back({任务, {}, false});
                    }
                }
                std::lock_guard 锁(互斥);
                if (停止 || 已完成) return;
                auto& 事件 = 学习事件组.at(交接);
                if (!合法 || 相关.状态 != 新任务操作状态::已找到) {
                    最近学习运行异常 = 新学习运行异常记录{
                        通知.方法节点, 新学习运行异常原因::结构不一致};
                    事件.已终结 = true;
                    return;
                }
                auto 新失效 = 学习失效筹办工作;
                // 等待复核先完成交接，学习事件保留同一批次延后，不同时占用T。
                if (std::any_of(新任务组.begin(), 新任务组.end(), [&](const auto& 项) {
                        return 等待复核占用.contains(项.任务节点.值)
                            || 有服务结算来源待处理已加锁(项.任务节点.值);
                    })) return;
                for (const auto& 当前 : 新任务组) {
                    const auto 旧 = 最近筹办结果.find(当前.任务节点.值);
                    if (旧 != 最近筹办结果.end())
                        新失效.insert_or_assign(当前.任务节点.值, 旧->second.原工作.工作编号);
                }
                // 整批构造完成才发布；事件串行推进，前一批的占用已全部释放。
                事件.任务组.swap(新任务组);
                学习复核占用.swap(新占用);
                学习失效筹办工作.swap(新失效);
                事件.已展开 = true;
                事件.已终结 = 事件.任务组.empty();
                最近方法学习结束 = 通知;
                return;
            }

            const auto 终结 = [&](bool 异常) {
                std::lock_guard 锁(互斥);
                auto& 事件 = 学习事件组.at(交接);
                for (auto& 当前 : 事件.任务组) {
                    if (当前.任务节点 != 项.任务节点) continue;
                    当前.已终结 = true;
                    if (当前.复核编号) 学习复核组.at(*当前.复核编号).已终结 = true;
                    break;
                }
                学习复核占用.erase(项.任务节点.值);
                if (异常) 最近学习运行异常 = 新学习运行异常记录{
                    通知.方法节点, 新学习运行异常原因::结构不一致};
                事件.已终结 = std::all_of(事件.任务组.begin(), 事件.任务组.end(),
                    [](const auto& 当前) { return 当前.已终结; });
            };
            {
                std::lock_guard 锁(互斥);
                if (停止 || 已完成
                    || std::any_of(活动工作.begin(), 活动工作.end(), [&](const auto& 工作) {
                        return 工作.请求.任务节点 == 项.任务节点;
                    })) return;
                // 活动W1/W2完成后才出现的定位同样失效，不能趁占用释放派旧W3。
                const auto 旧 = 最近筹办结果.find(项.任务节点.值);
                if (旧 != 最近筹办结果.end())
                    学习失效筹办工作.insert_or_assign(
                        项.任务节点.值, 旧->second.原工作.工作编号);
            }
            const auto 任务 = 任务服务->获取任务(项.任务节点);
            if (!任务) { 终结(true); return; }
            新任务学习复核定位 当前定位{0, 交接, 通知.方法节点,
                项.任务节点, 任务->需求节点, 任务->当前状态,
                任务->当前候选方法材料节点, 任务->方法实例节点, 任务->方法执行组织节点};
            if (!学习筹办安全状态(任务->当前状态)) { 终结(false); return; }
            if (!有效(任务->需求节点)) { 终结(true); return; }
            bool 有保护 = false;
            {
                std::lock_guard 锁(互斥);
                有保护 = 阻塞任务.contains(项.任务节点.值)
                    || 结果等待任务.contains(项.任务节点.值)
                    || 待结果反馈任务.contains(项.任务节点.值);
            }
            if (有保护) { 终结(false); return; }

            学习复核记录 记录;
            if (!项.复核编号) {
                std::lock_guard 锁(互斥);
                if (停止 || 已完成) return;
                if (!下一学习复核编号
                    || 下一学习复核编号 == (std::numeric_limits<std::uint64_t>::max)()) {
                    最近学习运行异常 = 新学习运行异常记录{
                        通知.方法节点, 新学习运行异常原因::内部错误};
                    // 编号耗尽是硬停止，不换编号空间重试。
                    停止 = true;
                    return;
                }
                当前定位.复核编号 = 下一学习复核编号++;
                记录.定位 = 当前定位;
                学习复核组.emplace(当前定位.复核编号, 记录);
                for (auto& 当前 : 学习事件组.at(交接).任务组)
                    if (当前.任务节点 == 项.任务节点) {
                        当前.复核编号 = 当前定位.复核编号;
                        break;
                    }
                项.复核编号 = 当前定位.复核编号;
            } else {
                std::lock_guard 锁(互斥);
                记录 = 学习复核组.at(*项.复核编号);
            }
            当前定位.复核编号 = 记录.定位.复核编号;
            auto 可比较定位 = 当前定位;
            if (记录.准备已尝试 && 当前定位.任务状态 == 新任务状态::待查找方法)
                可比较定位.任务状态 = 记录.定位.任务状态;
            if (可比较定位 != 记录.定位) {
                const auto 相关 = 任务服务->查询受方法学习影响任务(通知.方法节点);
                if (相关.状态 == 新任务操作状态::资源失败) return;
                if (相关.状态 != 新任务操作状态::已找到) { 终结(true); return; }
                if (std::find(相关.任务节点组.begin(), 相关.任务节点组.end(), 项.任务节点)
                    == 相关.任务节点组.end()) { 终结(false); return; }
                std::lock_guard 锁(互斥);
                学习复核组.at(*项.复核编号).已终结 = true;
                for (auto& 当前 : 学习事件组.at(交接).任务组)
                    if (当前.任务节点 == 项.任务节点) 当前.复核编号.reset();
                return; // 保留占用；下一次建立全新的fresh定位，不复用旧许可。
            }
            if (!记录.决议) {
                if (记录.已反馈) return;
                学习复核接收函数 接收;
                {
                    std::lock_guard 锁(互斥);
                    接收 = 自我学习反馈接收;
                }
                const auto 状态 = 接收 ? 接收(记录.定位)
                    : 新任务学习复核反馈接收状态::尚未配置;
                if (状态 == 新任务学习复核反馈接收状态::已接收
                    || 状态 == 新任务学习复核反馈接收状态::精确重复) {
                    std::lock_guard 锁(互斥);
                    学习复核组.at(*项.复核编号).已反馈 = true;
                } else if (状态 != 新任务学习复核反馈接收状态::邮箱已满
                    && 状态 != 新任务学习复核反馈接收状态::尚未启动
                    && 状态 != 新任务学习复核反馈接收状态::资源失败) 终结(true);
                return;
            }
            if (记录.决议->状态 != 新自我学习复核状态::允许重新筹办) {
                终结(记录.决议->状态 == 新自我学习复核状态::需核查);
                return;
            }
            {
                std::lock_guard 锁(互斥);
                if (停止 || 已完成) return;
                学习复核组.at(*项.复核编号).准备已尝试 = true;
            }
            const auto 准备 = 任务服务->准备学习后重新筹办(项.任务节点, 通知.方法节点);
            if (准备 == 新任务操作状态::资源失败) return;
            if (准备 != 新任务操作状态::已修改 && 准备 != 新任务操作状态::无变化) {
                终结(准备 != 新任务操作状态::任务状态不允许
                    && 准备 != 新任务操作状态::任务不存在);
                return;
            }
            const auto 读回 = 任务服务->获取任务(项.任务节点);
            if (!读回 || 读回->当前状态 != 新任务状态::待查找方法
                || 读回->需求节点 != 记录.定位.需求节点
                || 读回->方法实例节点 != 记录.定位.方法实例节点
                || 读回->方法执行组织节点 != 记录.定位.方法执行组织节点) {
                终结(true);
                return;
            }
            {
                std::lock_guard 锁(互斥);
                最近筹办结果.erase(项.任务节点.值);
                待反馈任务.erase(项.任务节点.值);
                最近自我筹办裁决.erase(项.任务节点.值);
                筹办阻塞任务.erase(项.任务节点.值);
                等待来源.erase(项.任务节点.值);
                等待目标复核.erase(项.任务节点.值);
                筹办资源重试.erase(项.任务节点.值);
                上次等待材料.erase(项.任务节点.值);
                调度待触发 = true;
                // 不清阻塞任务、执行/结果/恢复/结算材料。
            }
            终结(false);
        } catch (const std::bad_alloc&) {
            // 原事件、定位及已接纳决议仍在；下一次按同一交接重入。
        } catch (...) {
            std::lock_guard 锁(互斥);
            最近学习运行异常 = 新学习运行异常记录{
                通知.方法节点, 新学习运行异常原因::内部错误};
            if (交接) {
                auto& 事件 = 学习事件组.at(交接);
                事件.已终结 = true;
                for (const auto& 项 : 事件.任务组) {
                    学习复核占用.erase(项.任务节点.值);
                    if (项.复核编号) 学习复核组.at(*项.复核编号).已终结 = true;
                }
            }
        }
    }

    void 推进服务结算来源事件() noexcept {
        try {
            新长期服务值结算配置 配;
            稳定编码 目标, 来源节点;
            {
                std::lock_guard 锁(互斥);
                if (停止 || 已完成 || 待服务结算来源.empty()
                    || !服务值结算配置 || !服务值结算任务) return;
                目标 = *服务值结算任务;
                const auto 值 = 目标.值;
                // 先让已经取得占用的复核和真实执行收束，禁止把来源待办放入其保护谓词。
                if (学习复核占用.contains(值) || 等待复核占用.contains(值)
                    || 阻塞任务.contains(值) || 结果等待任务.contains(值)
                    || 待结果反馈任务.contains(值)) return;
                const auto 裁决 = 最近自我筹办裁决.find(值);
                if (裁决 != 最近自我筹办裁决.end() && 裁决->second.执行工作) {
                    // 已有授权的来源不可替换。收束本事件，不能让待来源门阻止旧W3派发。
                    待服务结算来源.erase(待服务结算来源.begin());
                    服务结算来源排空中 = false;
                    变化.notify_all();
                    return;
                }
                服务结算来源排空中 = true;
                待派调度工作.reset();
                if (待派筹办工作 && 待派筹办工作->任务节点 == 目标)
                    待派筹办工作.reset();
                调度材料.erase(值);
                if (std::any_of(活动工作.begin(), 活动工作.end(), [&](const auto& 项) {
                        return 项.请求.种类 == 新任务工作种类::读取任务调度材料
                            || 项.请求.任务节点 == 目标;
                    })) return;
                配 = *服务值结算配置;
                来源节点 = 待服务结算来源.front();
            }
            // 从这里至重新发布普通调度均在manager线程串行执行；外部入口只能排队。
            const auto 终结事件 = [&](bool 已重新筹办) {
                std::lock_guard 锁(互斥);
                if (待服务结算来源.empty() || 待服务结算来源.front() != 来源节点)
                    throw std::logic_error("结算来源队列身份变化");
                if (已重新筹办) {
                    const auto 旧 = 最近筹办结果.find(目标.值);
                    if (旧 != 最近筹办结果.end())
                        学习失效筹办工作.insert_or_assign(目标.值, 旧->second.原工作.工作编号);
                    待反馈任务.erase(目标.值);
                    最近筹办结果.erase(目标.值);
                    最近自我筹办裁决.erase(目标.值);
                    筹办阻塞任务.erase(目标.值);
                    筹办资源重试.erase(目标.值);
                    等待来源.erase(目标.值);
                    等待目标复核.erase(目标.值);
                    上次等待材料.erase(目标.值);
                }
                待服务结算来源.erase(待服务结算来源.begin());
                服务结算来源排空中 = false;
                调度待触发 = true;
                变化.notify_all();
            };
            const auto 既有 = 任务服务->读取服务结算来源(目标);
            if (既有) {
                if (既有->来源任务.任务节点 == 来源节点) {
                    const auto 读回 = 任务服务->读取任务虚拟存在引用(既有->来源任务);
                    if (!读回 || (读回->当前状态 != static_cast<std::int64_t>(新任务状态::已完成)
                            && 读回->当前状态 != static_cast<std::int64_t>(新任务状态::已结算))
                        || 既有->冻结前值 != 配.冻结前值
                        || 既有->目标值域概念节点 != 配.目标值域概念节点) {
                        报告工程停止("结算来源重复交接读回不一致");
                        return;
                    }
                }
                终结事件(false); // 精确重复或其它来源，绝不覆盖。
                return;
            }
            const auto 任务 = 任务服务->获取任务(目标);
            if (!任务) { 报告工程停止("结算来源接纳目标任务不可读"); return; }
            if (!学习筹办安全状态(任务->当前状态)) { 终结事件(false); return; }
            const auto 满足 = 任务服务->复核任务当前需求(目标);
            if (满足.状态 == 新需求当前满足状态::已满足) { 终结事件(false); return; }
            if (满足.状态 != 新需求当前满足状态::需增加) {
                报告工程停止("结算来源接纳需求读回失败或目标不一致"); return;
            }
            const auto 事实 = 配.世界树服务->获取事实服务引用();
            const auto 特征 = 事实.特征服务.获取特征(配.服务值特征);
            if (!特征 || !特征->当前值 || *特征->当前值 != 新特征准确值{配.冻结前值}) {
                报告工程停止("结算来源接纳服务值读回不一致"); return;
            }
            const auto 来源 = 任务服务->获取任务虚拟存在(来源节点);
            if (!来源) { 报告工程停止("结算来源任务虚拟存在不可读"); return; }
            if (来源->当前状态 != 新任务状态::已完成) { 终结事件(false); return; }
            const 新任务服务结算来源绑定 绑定{
                {来源节点, 来源->节点, 来源->任务状态特征节点}, 配.冻结前值, 配.目标值域概念节点};
            const auto 接纳 = 任务服务->接纳服务结算来源(目标, 绑定);
            if (接纳 != 新任务操作状态::已修改 && 接纳 != 新任务操作状态::精确重复) {
                报告工程停止("结算来源正式绑定失败"); return;
            }
            const auto 准备 = 任务服务->准备服务结算筹办(目标);
            if (准备 != 新任务操作状态::已修改 && 准备 != 新任务操作状态::无变化
                && 准备 != 新任务操作状态::精确重复) {
                报告工程停止("结算来源绑定后重新筹办失败"); return;
            }
            终结事件(true);
        } catch (...) { 报告工程停止("结算来源消息串行推进失败"); }
    }

    void 运行() noexcept {
        try {
            std::unique_lock 锁(互斥);
            已进入 = true;
            变化.notify_all();
            while (!停止) {
                // 先回收已就绪回执，避免它们占据邮箱和同T排他位置。
                while (!停止 && std::chrono::steady_clock::now() >= 下一工作消费时点) {
                    const auto 已完成工作 = std::find_if(
                        活动工作.begin(), 活动工作.end(), [](const auto& 占用) {
                            return 占用.结果.valid()
                                && 占用.结果.wait_for(std::chrono::milliseconds(0))
                                    == std::future_status::ready;
                        });
                    if (已完成工作 == 活动工作.end()) break;
                    const auto 占用 = *已完成工作;
                    生命周期 = 新任务管理生命周期::处理消息;
                    锁.unlock();
                    处理已完成工作(占用);
                    锁.lock();
                    调度待触发 = true;
                }
                if (停止) continue;

                锁.unlock();
                推进服务结算来源事件();
                推进学习事件();
                推进等待复核();
                尝试反馈筹办结果();
                尝试反馈任务结果();
                尝试复核条件子需求();
                锁.lock();

                if (停止) continue;
                std::shared_ptr<消息项> 项;
                if (!活动.empty()) {
                    const auto 最早 = std::min_element(活动.begin(), 活动.end(),
                        [](const auto& 左, const auto& 右) {
                            return 左->可处理时点 < 右->可处理时点;
                        });
                    if ((*最早)->可处理时点 <= std::chrono::steady_clock::now()) {
                        项 = *最早;
                    }
                }
                if (项) {
                    生命周期 = 新任务管理生命周期::处理消息;
                    锁.unlock();
                    新任务承接处理结果 结果;
                    结果.原消息 = 项->消息;
                    const bool 是扫描补齐 = 项->消息.依据
                        == 新任务承接依据::自我全树扫描;
                    const bool 是条件子需求 = 项->消息.依据
                        == 新任务承接依据::自我条件子需求;
                    const bool 自动决议 = 项->消息.依据
                        != 新任务承接依据::显式请求 && !是扫描补齐 && !是条件子需求;
                    if (是条件子需求) {
                        结果.承接 = 新任务治理承接结果{};
                        try {
                            std::vector<新任务已确认条件子需求> 单项组;
                            {
                                std::lock_guard 定位锁(互斥);
                                if (!停止 && 子需求承接定位有效已加锁(*项)) {
                                    const auto& 记录 = 最近自我筹办裁决.at(
                                        项->父筹办定位->任务节点.值);
                                    const auto 子项 = std::find_if(记录.已确认子需求组.begin(),
                                        记录.已确认子需求组.end(), [&](const auto& 候选) {
                                            return 候选.需求节点 == 项->消息.需求节点;
                                        });
                                    单项组.push_back(*子项);
                                }
                            }
                            if (!单项组.empty()) {
                                // 接收回执只收存已发布D；真正承接前仍按原域快照
                                // fresh核验，不能把随后改变的域作为旧self决议执行。
                                const auto 复核 = 任务服务->复核条件子需求就绪(
                                    项->父筹办定位->任务节点, 单项组);
                                if (复核.状态 == 新任务条件子需求就绪状态::已就绪
                                    || 复核.状态 == 新任务条件子需求就绪状态::等待条件) {
                                    结果.承接 = 任务服务->承接已确认条件子需求(
                                        项->父筹办定位->任务节点, 项->消息.需求节点);
                                } else if (复核.状态 == 新任务条件子需求就绪状态::资源失败) {
                                    结果.承接->状态 = 新任务治理承接状态::资源失败;
                                }
                            }
                        } catch (...) {
                            结果.承接->状态 = 新任务治理承接状态::资源失败;
                        }
                    } else {
                        结果.承接 = 是扫描补齐
                            ? 任务服务->建立需求任务关联(项->消息.需求节点)
                            : 任务服务->承接需求任务(
                                项->消息.需求节点, 自动决议);
                    }
                    switch (结果.承接->状态) {
                    case 新任务治理承接状态::已读回:
                        结果.状态 = 新任务承接处理状态::已承接; break;
                    case 新任务治理承接状态::已重新启用:
                        结果.状态 = 新任务承接处理状态::已重新启用; break;
                    case 新任务治理承接状态::既有终结任务:
                        结果.状态 = 新任务承接处理状态::既有终结; break;
                    case 新任务治理承接状态::需求当前已满足:
                        结果.状态 = 新任务承接处理状态::需求当前已满足; break;
                    case 新任务治理承接状态::需求不存在:
                        结果.状态 = 新任务承接处理状态::入口拒绝; break;
                    case 新任务治理承接状态::尚未初始化:
                        结果.状态 = 新任务承接处理状态::服务未就绪; break;
                    default:
                        结果.状态 = 新任务承接处理状态::需核查; break;
                    }
                    // task调用及其递归锁已退出，manager锁尚未重新取得。
                    const bool 扫描资源重试 = 扫描承接可资源重试(结果);
                    if (!扫描资源重试 || !项->扫描资源失败已记录) {
                        记录需求任务承接观察(结果);
                        if (扫描资源重试) 项->扫描资源失败已记录 = true;
                    }
                    锁.lock();
                    // 普通承接只重试零写资源失败；受限条件承接则通过一D一T
                    // 读回收敛。扫描只对具名资源失败fresh重入，保留原消息、
                    // promise和容量；同步bool的未知失败不能走资源重试。
                    if (扫描资源重试
                        || ((!结果.承接->写入已尝试 || 是条件子需求)
                            && 结果.承接->状态 == 新任务治理承接状态::资源失败)) {
                        if (!停止) {
                            项->可处理时点 = std::chrono::steady_clock::now()
                                + 等待时长(配置.资源重试间隔毫秒);
                            continue;
                        }
                        结果.状态 = 新任务承接处理状态::停止取消;
                    }
                    if (是条件子需求 && 项->父筹办定位) {
                        const auto 父记录 = 最近自我筹办裁决.find(
                            项->父筹办定位->任务节点.值);
                        if (父记录 != 最近自我筹办裁决.end()
                            && 父记录->second.裁决.定位 == *项->父筹办定位) {
                            auto& 记录 = 父记录->second;
                            for (std::size_t i = 0; i < 记录.子需求承接组.size(); ++i) {
                                if (记录.子需求承接组[i] != 项) continue;
                                记录.子需求承接完成组[i] = true;
                                if (结果.状态 != 新任务承接处理状态::已承接
                                    && 结果.状态 != 新任务承接处理状态::已重新启用
                                    && 结果.状态 != 新任务承接处理状态::既有终结
                                    && 结果.状态 != 新任务承接处理状态::需求当前已满足)
                                    记录.条件交接失败 = true;
                                break;
                            }
                        }
                        if (结果.承接->任务
                            && 结果.承接->状态 == 新任务治理承接状态::已重新启用) {
                            阻塞任务.erase(结果.承接->任务->节点.值);
                            筹办阻塞任务.erase(结果.承接->任务->节点.值);
                        }
                    }
                    if (结果.承接 && 结果.承接->任务
                        && 结果.状态 == 新任务承接处理状态::已承接
                        && 结果.承接->任务->当前状态 == 新任务状态::等待条件
                        && !结果.承接->任务->当前候选方法材料节点
                        && !结果.承接->任务->最近执行实例节点)
                        登记等待来源已加锁(*结果.承接->任务, 新任务等待原因::首次关联待许可);
                    调度待触发 = true;
                    项->承诺.set_value(std::move(结果));
                    if (自动决议) {
                        auto 位置 = std::find_if(最近自动决议.begin(),
                            最近自动决议.end(), [&](const auto& 记录) {
                                return 记录
                                    && 记录->消息.需求节点 == 项->消息.需求节点;
                            });
                        if (位置 == 最近自动决议.end()) {
                            位置 = std::find_if(最近自动决议.begin(),
                                最近自动决议.end(), [](const auto& 记录) {
                                    return !记录;
                                });
                        }
                        if (位置 != 最近自动决议.end()) {
                            *位置 = 已完成自动决议{项->消息, 项->结果};
                        }
                    }
                    活动.erase(std::find(活动.begin(), 活动.end(), 项));
                }

                锁.unlock();
                const auto 普通派发 = [&]() {
                    auto 状态 = 尝试派发调度扫描();
                    if (状态 == 筹办派发状态::无可派发任务)
                        状态 = 尝试派发执行工作();
                    if (状态 == 筹办派发状态::无可派发任务)
                        状态 = 尝试派发筹办工作();
                    return 状态;
                };
                bool 本次为结果检测 = 下次优先结果检测;
                auto 派发 = 本次为结果检测 ? 尝试派发结果检测() : 普通派发();
                if (派发 == 筹办派发状态::无可派发任务) {
                    本次为结果检测 = !本次为结果检测;
                    派发 = 本次为结果检测 ? 尝试派发结果检测() : 普通派发();
                }
                锁.lock();
                if (派发 == 筹办派发状态::已派发) {
                    下次优先结果检测 = !本次为结果检测;
                    continue;
                }
                if (派发 == 筹办派发状态::内部错误) {
                    锁.unlock();
                    工程异常终止(L"新_任务管理线程类.cpp", __LINE__,
                        L"任务工作派发", L"工作接纳或调度结构发生工程错误");
                }
                if (派发 == 筹办派发状态::已停止) {
                    停止 = true;
                    continue;
                }
                生命周期 = 新任务管理生命周期::等待消息;
                变化.wait_for(锁, (std::min)(等待时长(配置.资源重试间隔毫秒),
                    等待时长(配置.调度复核间隔毫秒)));
            }

            锁.unlock();
            const bool 工作已收束 = 停止工作线程并收束();
            锁.lock();
            终结待办(内部错误发生
                ? 新任务承接处理状态::内部错误
                : 新任务承接处理状态::停止取消);
            生命周期 = !内部错误发生 && 工作已收束
                ? 新任务管理生命周期::已停止
                : 新任务管理生命周期::内部错误;
            已完成 = true;
            变化.notify_all();
        } catch (...) {
            try {
                {
                    std::lock_guard 锁(互斥);
                    内部错误发生 = true;
                    停止 = true;
                    生命周期 = 新任务管理生命周期::停止中;
                    变化.notify_all();
                }
                (void)停止工作线程并收束();
                {
                    std::lock_guard 锁(互斥);
                    终结待办(新任务承接处理状态::内部错误);
                    生命周期 = 新任务管理生命周期::内部错误;
                    已完成 = true;
                    变化.notify_all();
                }
            } catch (...) { std::terminate(); }
        }
    }
};

新_任务管理线程类::新_任务管理线程类(新_任务类& 任务服务) noexcept
    : 实现_(new (std::nothrow) 实现(任务服务)) {}

新服务值结算协作状态 新_任务管理线程类::接纳服务值结算来源(
    稳定编码 已完成任务节点) noexcept {
    if (!实现_) return 新服务值结算协作状态::工程停止;
    try {
        std::lock_guard 锁(实现_->互斥);
        if (!有效(已完成任务节点) || !实现_->服务值结算配置 || !实现_->服务值结算任务
            || 实现_->停止 || 实现_->已完成) return 新服务值结算协作状态::工程停止;
        if (已完成任务节点 == *实现_->服务值结算任务) return 新服务值结算协作状态::非本方法;
        auto& 来源组 = 实现_->待服务结算来源;
        if (std::find(来源组.begin(), 来源组.end(), 已完成任务节点) == 来源组.end())
            来源组.push_back(已完成任务节点);
        实现_->变化.notify_all();
        return 新服务值结算协作状态::已接收待筹办;
    } catch (...) {
        实现_->报告工程停止("服务结算来源消息未能收存");
        return 新服务值结算协作状态::工程停止;
    }
}

bool 新_任务管理线程类::使用任务服务(const 新_任务类& 任务服务) const noexcept {
    return 实现_ && 实现_->任务服务 == &任务服务;
}

新任务管理操作状态 新_任务管理线程类::绑定自我筹办反馈(
    const void* 所有者,
    筹办反馈接收函数 筹办接收,
    结果反馈接收函数 结果接收,
    学习复核接收函数 学习接收,
    等待复核接收函数 等待接收,
    const 新长期服务值结算配置& 服务结算配置) noexcept {
    if (!实现_ || !所有者 || !筹办接收 || !结果接收 || !学习接收 || !等待接收
        || !服务结算配置.完整()) {
        return 新任务管理操作状态::入口拒绝;
    }
    try {
        std::lock_guard 锁(实现_->互斥);
        if (实现_->创建已尝试 || 实现_->停止 || 实现_->已完成)
            return 新任务管理操作状态::服务未就绪;
        if (实现_->自我反馈所有者) {
            return 实现_->自我反馈所有者 == 所有者
                    && 实现_->服务值结算配置 == 服务结算配置
                ? 新任务管理操作状态::无变化
                : 新任务管理操作状态::所有者冲突;
        }
        实现_->自我反馈接收 = std::move(筹办接收);
        实现_->自我结果反馈接收 = std::move(结果接收);
        实现_->自我学习反馈接收 = std::move(学习接收);
        实现_->自我等待反馈接收 = std::move(等待接收);
        实现_->自我反馈所有者 = 所有者;
        实现_->服务值结算配置 = 服务结算配置;
        实现_->变化.notify_all();
        return 新任务管理操作状态::已完成;
    } catch (const std::bad_alloc&) {
        return 新任务管理操作状态::资源失败;
    } catch (...) {
        return 新任务管理操作状态::内部错误;
    }
}

void 新_任务管理线程类::解除自我筹办反馈(const void* 所有者) noexcept {
    if (!实现_ || !所有者) return;
    try {
        std::lock_guard 锁(实现_->互斥);
        if (实现_->自我反馈所有者 != 所有者) return;
        实现_->自我反馈接收 = {};
        实现_->自我结果反馈接收 = {};
        实现_->自我学习反馈接收 = {};
        实现_->自我等待反馈接收 = {};
        实现_->自我反馈所有者 = nullptr;
        实现_->服务值结算配置.reset();
        实现_->服务值结算任务.reset();
        实现_->变化.notify_all();
    } catch (...) {}
}

新自我筹办裁决接收状态 新_任务管理线程类::提交自我筹办裁决(
    const 新自我筹办裁决& 裁决) noexcept {
    if (!实现_ || !裁决.定位.完整())
        return 新自我筹办裁决接收状态::入口拒绝;
    try {
        const auto 任务值 = 裁决.定位.任务节点.值;
        const bool 是子需求回执 = 裁决.状态 == 新自我筹办裁决状态::已处理条件子需求;
        if (是子需求回执 != 裁决.子需求回执.has_value())
            return 新自我筹办裁决接收状态::入口拒绝;
        if (是子需求回执) {
            if (裁决.定位.原工作.种类 != 新任务工作种类::检查条件
                || 裁决.唯一具备方法节点 || 裁决.唯一具备配对节点
                || 裁决.唯一对应结果节点 || 裁决.执行前判断 || 裁决.本能采用
                || !裁决.子需求回执->已完成处理)
                return 新自我筹办裁决接收状态::入口拒绝;
            新任务条件子需求提议 原提议;
            {
                std::lock_guard 锁(实现_->互斥);
                if (实现_->停止 || 实现_->已完成)
                    return 新自我筹办裁决接收状态::已停止接收;
                const auto 当前 = 实现_->最近筹办结果.find(任务值);
                if (实现_->学习占用或旧筹办已失效已加锁(裁决.定位)
                    || 当前 == 实现_->最近筹办结果.end()
                    || 当前->second.原工作 != 裁决.定位.原工作
                    || 当前->second.状态 != 新任务筹办处理状态::条件筹办已完成
                    || !当前->second.独立子需求提议)
                    return 新自我筹办裁决接收状态::定位已变化;
                const auto 已有 = 实现_->最近自我筹办裁决.find(任务值);
                if (已有 != 实现_->最近自我筹办裁决.end()) {
                    if (已有->second.裁决 != 裁决)
                        return 新自我筹办裁决接收状态::身份冲突;
                    return 已有->second.已完成接收
                        ? 新自我筹办裁决接收状态::精确重复
                        : 新自我筹办裁决接收状态::资源失败;
                }
                原提议 = *当前->second.独立子需求提议;
            }
            const auto& 回执 = *裁决.子需求回执;
            if (回执.原提议 != 原提议
                || 回执.项组.size() != 原提议.子需求项组.size())
                return 新自我筹办裁决接收状态::身份冲突;
            实现::自我筹办裁决记录 记录;
            记录.裁决 = 裁决;
            std::set<std::uint64_t> 成功需求;
            for (std::size_t i = 0; i < 回执.项组.size(); ++i) {
                const auto& 项 = 回执.项组[i];
                if (项.原提议 != 原提议.子需求项组[i])
                    return 新自我筹办裁决接收状态::身份冲突;
                switch (项.状态) {
                case 新子需求采用状态::已建立:
                case 新子需求采用状态::已复用:
                    if (!项.需求节点 || !有效(*项.需求节点)
                        || !成功需求.insert(项.需求节点->值).second)
                        return 新自我筹办裁决接收状态::入口拒绝;
                    记录.已确认子需求组.push_back({项.原提议, *项.需求节点});
                    break;
                case 新子需求采用状态::已无需要:
                    if (项.需求节点) return 新自我筹办裁决接收状态::入口拒绝;
                    break;
                case 新子需求采用状态::来源过期:
                case 新子需求采用状态::拒绝:
                case 新子需求采用状态::结构错误:
                    if (项.需求节点) return 新自我筹办裁决接收状态::入口拒绝;
                    记录.条件交接失败 = true;
                    break;
                default:
                    return 新自我筹办裁决接收状态::入口拒绝;
                }
            }
            for (const auto& 子项 : 记录.已确认子需求组) {
                // 按单D调用，避免前项的来源变化/材料缺失使后项身份漏检。
                // 两种非就绪返回都发生在该D父链、来源及完整目标核验以后，
                // 可以收存已发布前缀，不能据此宣称可推进。
                const auto 读回 = 实现_->任务服务->复核条件子需求就绪(
                    裁决.定位.任务节点, {子项});
                if (读回.状态 == 新任务条件子需求就绪状态::资源失败)
                    return 新自我筹办裁决接收状态::资源失败;
                if (读回.状态 == 新任务条件子需求就绪状态::来源已变化) {
                    记录.来源变化子需求.insert(子项.需求节点.值);
                    记录.条件交接失败 = true;
                    continue;
                }
                if (读回.状态 != 新任务条件子需求就绪状态::已就绪
                    && 读回.状态 != 新任务条件子需求就绪状态::等待条件
                    && 读回.状态 != 新任务条件子需求就绪状态::材料未到)
                    return 新自我筹办裁决接收状态::入口拒绝;
            }
            记录.子需求承接组.resize(记录.已确认子需求组.size());
            记录.子需求承接完成组.resize(记录.已确认子需求组.size(), false);
            记录.条件复核待触发 = true; // 首次收执后的一次补查，承接未收口先合并。
            记录.已完成接收 = true;
            {
                std::lock_guard 锁(实现_->互斥);
                if (实现_->停止 || 实现_->已完成)
                    return 新自我筹办裁决接收状态::已停止接收;
                const auto 当前 = 实现_->最近筹办结果.find(任务值);
                if (实现_->学习占用或旧筹办已失效已加锁(裁决.定位)
                    || 当前 == 实现_->最近筹办结果.end()
                    || 当前->second.原工作 != 裁决.定位.原工作
                    || 当前->second.独立子需求提议 != 原提议)
                    return 新自我筹办裁决接收状态::定位已变化;
                const auto 已有 = 实现_->最近自我筹办裁决.find(任务值);
                if (已有 != 实现_->最近自我筹办裁决.end())
                    return 已有->second.裁决 == 裁决
                        ? 新自我筹办裁决接收状态::精确重复
                        : 新自我筹办裁决接收状态::身份冲突;
                实现_->筹办阻塞任务.insert(任务值);
                实现_->最近自我筹办裁决.emplace(任务值, std::move(记录));
                实现_->待反馈任务.erase(任务值);
                实现_->变化.notify_all();
            }
            return 新自我筹办裁决接收状态::已接收;
        }
        const bool 可进入 = 裁决.状态
            == 新自我筹办裁决状态::可进入执行前判断;
        if (可进入) {
            if (!裁决.唯一具备方法节点 || !裁决.唯一具备配对节点
                || !裁决.唯一对应结果节点 || !裁决.执行前判断 || !裁决.本能采用
                || 裁决.本能采用->来源方法节点 != *裁决.唯一具备方法节点
                || 裁决.本能采用->来源配对节点 != *裁决.唯一具备配对节点
                || 裁决.本能采用->来源结果节点 != *裁决.唯一对应结果节点
                || 裁决.执行前判断->任务节点 != 裁决.定位.任务节点
                || 裁决.执行前判断->方法节点
                    != *裁决.唯一具备方法节点
                || 裁决.执行前判断->配对节点
                    != *裁决.唯一具备配对节点
                || 裁决.执行前判断->结果节点
                    != *裁决.唯一对应结果节点
                || (裁决.执行前判断->状态
                        == 新自我执行前判断状态::允许执行
                    && (!裁决.执行前判断->本能执行请求
                        || !裁决.执行前判断->本能执行请求->完整()
                        || 裁决.执行前判断->本能执行请求->本能采用 != 裁决.本能采用
                        || 裁决.执行前判断->本能执行请求->任务节点
                            != 裁决.定位.任务节点
                        || 裁决.执行前判断->本能执行请求->方法节点
                            != *裁决.唯一具备方法节点
                        || 裁决.执行前判断->本能执行请求->配对节点
                            != *裁决.唯一具备配对节点
                        || 裁决.执行前判断->本能执行请求->结果节点
                            != *裁决.唯一对应结果节点))
                || (裁决.执行前判断->状态
                        != 新自我执行前判断状态::允许执行
                    && 裁决.执行前判断->本能执行请求)) {
                return 新自我筹办裁决接收状态::入口拒绝;
            }
        } else if (裁决.唯一具备方法节点 || 裁决.唯一具备配对节点
            || 裁决.唯一对应结果节点 || 裁决.执行前判断 || 裁决.本能采用) {
            return 新自我筹办裁决接收状态::入口拒绝;
        }
        const bool 需要执行 = 可进入
            && 裁决.执行前判断->状态
                == 新自我执行前判断状态::允许执行;
        const auto 等待任务 = !需要执行
            ? 实现_->任务服务->获取任务(裁决.定位.任务节点) : std::optional<新任务信息>{};
        if (!需要执行 && !等待任务) return 新自我筹办裁决接收状态::资源失败;
        {
            std::lock_guard 锁(实现_->互斥);
            if (实现_->停止 || 实现_->已完成)
                return 新自我筹办裁决接收状态::已停止接收;
            const auto 当前 = 实现_->最近筹办结果.find(任务值);
            if (实现_->学习占用或旧筹办已失效已加锁(裁决.定位)
                || 当前 == 实现_->最近筹办结果.end()
                || 当前->second.原工作 != 裁决.定位.原工作) {
                return 新自我筹办裁决接收状态::定位已变化;
            }
            const auto 已有 = 实现_->最近自我筹办裁决.find(任务值);
            if (已有 != 实现_->最近自我筹办裁决.end()) {
                if (已有->second.裁决 != 裁决)
                    return 新自我筹办裁决接收状态::身份冲突;
                if (!已有->second.已完成接收)
                    return 新自我筹办裁决接收状态::资源失败;
                实现_->待反馈任务.erase(任务值);
                return 新自我筹办裁决接收状态::精确重复;
            }
            std::optional<新任务工作请求> 执行工作;
            if (需要执行) {
                if (!实现_->下一工作编号
                    || 实现_->下一工作编号
                        == (std::numeric_limits<std::uint64_t>::max)()) {
                    return 新自我筹办裁决接收状态::资源失败;
                }
                新任务工作请求 请求;
                请求.工作编号 = 实现_->下一工作编号;
                请求.种类 = 新任务工作种类::执行本能动作;
                请求.任务节点 = 裁决.定位.任务节点;
                请求.本能执行请求 =
                    裁决.执行前判断->本能执行请求;
                执行工作 = std::move(请求);
            }
            实现::自我筹办裁决记录 记录;
            记录.裁决 = 裁决;
            记录.已完成接收 = true;
            记录.执行工作 = std::move(执行工作);
            if (等待任务 && 学习筹办安全状态(等待任务->当前状态)) {
                auto 原因 = 新任务等待原因::需核查;
                if (裁决.状态 == 新自我筹办裁决状态::当前无可用方法
                    && 裁决.定位.原工作.种类 == 新任务工作种类::查找方法)
                    原因 = 新任务等待原因::尚无方法;
                else if (裁决.状态 == 新自我筹办裁决状态::等待材料)
                    原因 = 新任务等待原因::候选条件未齐;
                else if (可进入 && 裁决.执行前判断->状态 == 新自我执行前判断状态::暂缓执行)
                    原因 = 新任务等待原因::需求材料未到;
                else if (当前->second.条件筹办 && 条件筹办为业务等待(*当前->second.条件筹办))
                    原因 = 新任务等待原因::候选条件未齐;
                实现_->登记等待来源已加锁(*等待任务, 原因, 裁决.定位.原工作);
            }
            实现_->最近自我筹办裁决.emplace(任务值, std::move(记录));
            实现_->最近任务后继决议.erase(任务值);
            if (需要执行) ++实现_->下一工作编号;
            实现_->待反馈任务.erase(任务值);
            实现_->变化.notify_all();
        }
        return 新自我筹办裁决接收状态::已接收;
    } catch (const std::bad_alloc&) {
        return 新自我筹办裁决接收状态::资源失败;
    } catch (...) {
        return 新自我筹办裁决接收状态::内部错误;
    }
}

新任务结果状态接收状态 新_任务管理线程类::提交结果状态通知(
    稳定编码 状态节点) noexcept {
    if (!实现_ || !有效(状态节点)) return 新任务结果状态接收状态::内部错误;
    try {
        std::lock_guard 锁(实现_->互斥);
        if (实现_->停止 || 实现_->已完成)
            return 新任务结果状态接收状态::已停止接收;
        // 通知只提示重读，不在调用者线程查询、核验或接纳S。
        实现_->结果通知待处理 = true;
        实现_->触发条件子需求复核已加锁();
        实现_->变化.notify_all();
        return 新任务结果状态接收状态::已接收待异步检测;
    } catch (...) {
        工程异常终止(L"新_任务管理线程类.cpp", __LINE__,
            L"提交结果状态通知", L"结果检测通知接纳失败");
    }
}

新自我任务后继接收状态 新_任务管理线程类::提交自我任务后继决议(
    const 新任务结果反馈定位& 定位,
    const 新自我任务后继决议& 决议) noexcept {
    if (!实现_ || !定位.完整()
        || 决议.任务节点 != 定位.任务节点
        || 决议.执行实例节点 != 定位.执行实例节点
        || 决议.影响评估节点 != 定位.影响评估节点) {
        return 新自我任务后继接收状态::入口拒绝;
    }
    try {
        {
            std::lock_guard 锁(实现_->互斥);
            if (实现_->停止 || 实现_->已完成) {
                return 新自我任务后继接收状态::已停止接收;
            }
            const auto 已有决议 = 实现_->最近任务后继决议.find(
                定位.任务节点.值);
            if (已有决议 != 实现_->最近任务后继决议.end()) {
                return 已有决议->second == 决议
                    ? 新自我任务后继接收状态::精确重复
                    : 新自我任务后继接收状态::入口拒绝;
            }
            const auto 当前 = 实现_->最近执行结果.find(定位.任务节点.值);
            if (当前 == 实现_->最近执行结果.end()
                || 当前->second.状态
                    != 新任务执行处理状态::已完成影响评估
                || 当前->second.原工作 != 定位.原工作
                || !当前->second.独立读回
                || !当前->second.独立读回->执行
                || !当前->second.独立读回->影响评估
                || 当前->second.独立读回->执行->节点
                    != 定位.执行实例节点
                || 当前->second.独立读回->影响评估->节点
                    != 定位.影响评估节点) {
                return 新自我任务后继接收状态::定位已变化;
            }
        }
        const auto 发布 = 实现_->任务服务->发布自我任务后继决议(决议);
        新自我任务后继接收状态 状态 =
            新自我任务后继接收状态::内部错误;
        switch (发布.状态) {
        case 新任务治理后继发布状态::已发布并读回:
            状态 = 新自我任务后继接收状态::已发布并读回; break;
        case 新任务治理后继发布状态::精确重复:
            状态 = 新自我任务后继接收状态::精确重复; break;
        case 新任务治理后继发布状态::定位已变化:
            状态 = 新自我任务后继接收状态::定位已变化; break;
        case 新任务治理后继发布状态::任务状态已变化:
            状态 = 新自我任务后继接收状态::任务状态已变化; break;
        case 新任务治理后继发布状态::状态发布失败:
            状态 = 新自我任务后继接收状态::状态发布失败; break;
        case 新任务治理后继发布状态::资源失败:
            状态 = 新自我任务后继接收状态::资源失败; break;
        case 新任务治理后继发布状态::结构不一致:
        default:
            状态 = 新自我任务后继接收状态::内部错误; break;
        }
        std::optional<新任务信息> 等待任务;
        if ((状态 == 新自我任务后继接收状态::已发布并读回
                || 状态 == 新自我任务后继接收状态::精确重复)
            && 决议.类型 == 新自我任务后继类型::等待)
            等待任务 = 实现_->任务服务->获取任务(定位.任务节点);
        if ((状态 == 新自我任务后继接收状态::已发布并读回
                || 状态 == 新自我任务后继接收状态::精确重复)
            && 决议.类型 == 新自我任务后继类型::等待 && !等待任务)
            return 新自我任务后继接收状态::资源失败;
        if (状态 == 新自我任务后继接收状态::已发布并读回
            || 状态 == 新自我任务后继接收状态::精确重复) {
            std::lock_guard 锁(实现_->互斥);
            if (等待任务 && 等待任务->当前状态 == 新任务状态::等待条件
                && 等待任务->最近执行实例节点 == 定位.执行实例节点)
                实现_->登记等待来源已加锁(*等待任务,
                    新任务等待原因::执行后等待, 定位.原工作);
            实现_->最近任务后继决议[定位.任务节点.值] = 决议;
            实现_->调度待触发 = true;
            实现_->待结果反馈任务.erase(定位.任务节点.值);
            // 完成状态先于完成结算发布。完成分支必须保留原结果定位，直到
            // self确认结算已经由task owner正式建立；否则结算资源失败后
            // 无法按原请求恢复。其它后继已经没有待完成的self业务步骤。
            if (决议.类型 != 新自我任务后继类型::完成) {
                if (const auto 等待 = 实现_->结果等待任务.find(定位.任务节点.值);
                    等待 != 实现_->结果等待任务.end()
                    && 等待->second.执行实例节点 == 定位.执行实例节点
                    && !等待->second.已派发)
                    实现_->结果等待任务.erase(等待);
                实现_->阻塞任务.erase(定位.任务节点.值);
                实现_->筹办阻塞任务.erase(定位.任务节点.值);
                实现_->最近筹办结果.erase(定位.任务节点.值);
                实现_->最近自我筹办裁决.erase(定位.任务节点.值);
                实现_->最近执行结果.erase(定位.任务节点.值);
            }
            实现_->变化.notify_all();
        }
        return 状态;
    } catch (const std::bad_alloc&) {
        return 新自我任务后继接收状态::资源失败;
    } catch (...) {
        return 新自我任务后继接收状态::内部错误;
    }
}

新任务完成结算确认状态 新_任务管理线程类::确认任务完成结算(
    const 新任务结果反馈定位& 定位,
    稳定编码 结算节点) noexcept {
    if (!实现_ || !定位.完整() || !有效(结算节点)) {
        return 新任务完成结算确认状态::入口拒绝;
    }
    try {
        const auto 结算 = 实现_->任务服务->读取任务完成结算(定位.任务节点);
        if (!结算) return 新任务完成结算确认状态::尚未结算;
        if (结算->节点 != 结算节点
            || 结算->任务节点 != 定位.任务节点
            || 结算->执行实例节点 != 定位.执行实例节点
            || 结算->影响评估节点 != 定位.影响评估节点) {
            return 新任务完成结算确认状态::定位已变化;
        }

        std::lock_guard 锁(实现_->互斥);
        if (实现_->停止 || 实现_->已完成) {
            return 新任务完成结算确认状态::已停止接收;
        }
        const auto 决议 = 实现_->最近任务后继决议.find(定位.任务节点.值);
        if (决议 == 实现_->最近任务后继决议.end()) {
            return 新任务完成结算确认状态::精确重复;
        }
        if (决议->second.任务节点 != 定位.任务节点
            || 决议->second.执行实例节点 != 定位.执行实例节点
            || 决议->second.影响评估节点 != 定位.影响评估节点
            || 决议->second.类型 != 新自我任务后继类型::完成) {
            return 新任务完成结算确认状态::定位已变化;
        }
        const auto 执行结果 = 实现_->最近执行结果.find(定位.任务节点.值);
        if (执行结果 == 实现_->最近执行结果.end()
            || 执行结果->second.原工作 != 定位.原工作) {
            return 新任务完成结算确认状态::定位已变化;
        }
        实现_->触发条件子需求复核已加锁();
        if (const auto 等待 = 实现_->结果等待任务.find(定位.任务节点.值);
                    等待 != 实现_->结果等待任务.end()
                    && 等待->second.执行实例节点 == 定位.执行实例节点
                    && !等待->second.已派发)
                    实现_->结果等待任务.erase(等待);
        实现_->阻塞任务.erase(定位.任务节点.值);
        实现_->筹办阻塞任务.erase(定位.任务节点.值);
        实现_->最近筹办结果.erase(定位.任务节点.值);
        实现_->最近自我筹办裁决.erase(定位.任务节点.值);
        实现_->最近执行结果.erase(定位.任务节点.值);
        实现_->最近任务后继决议.erase(决议);
        实现_->变化.notify_all();
        return 新任务完成结算确认状态::已确认;
    } catch (const std::bad_alloc&) {
        return 新任务完成结算确认状态::资源失败;
    } catch (...) {
        return 新任务完成结算确认状态::内部错误;
    }
}

新_任务管理线程类::~新_任务管理线程类() noexcept {
    if (!实现_) return;
    (void)请求停止();
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        if (实现_->线程.joinable()) {
            if (实现_->线程.get_id() == std::this_thread::get_id()) std::terminate();
            实现_->线程.join();
        }
        {
            std::lock_guard 锁(实现_->互斥);
            实现_->自我反馈接收 = {};
            实现_->自我结果反馈接收 = {};
            实现_->自我学习反馈接收 = {};
            实现_->自我等待反馈接收 = {};
            实现_->自我反馈所有者 = nullptr;
        }
        std::lock_guard 所有者锁(管理所有者互斥);
        if (管理所有者 == this) 管理所有者 = nullptr;
    } catch (...) { std::terminate(); }
}

新任务管理操作结果 新_任务管理线程类::启动(
    const 新任务管理运行配置& 配置, std::uint64_t 等待毫秒) noexcept {
    if (!实现_ || !实现_->工作线程) return {
        新任务管理操作状态::资源失败, 新任务管理生命周期::内部错误};
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        std::unique_lock 锁(实现_->互斥);
        if (!配置.邮箱容量 || 配置.邮箱容量 > 实现_->活动.max_size()
            || !配置.资源重试间隔毫秒 || !等待毫秒 || !配置.调度复核间隔毫秒
            || static_cast<std::uint64_t>(等待时长(配置.资源重试间隔毫秒).count())
                != 配置.资源重试间隔毫秒
            || static_cast<std::uint64_t>(等待时长(配置.调度复核间隔毫秒).count())
                != 配置.调度复核间隔毫秒)
            return 实现_->操作结果(新任务管理操作状态::入口拒绝);
        if (实现_->创建已尝试) return 实现_->操作结果(
            实现_->已进入 && !实现_->停止 && !实现_->已完成 && 实现_->配置 == 配置
                ? 新任务管理操作状态::已在运行 : 新任务管理操作状态::入口拒绝);
        {
            std::lock_guard 所有者锁(管理所有者互斥);
            if (管理所有者 && 管理所有者 != this)
                return 实现_->操作结果(新任务管理操作状态::所有者冲突);
            管理所有者 = this;
        }
        实现_->创建已尝试 = true;
        实现_->配置 = 配置;
        实现_->生命周期 = 新任务管理生命周期::启动中;
        锁.unlock();
        const bool 就绪 = 实现_->任务服务->初始化治理();
        std::optional<新任务治理承接结果> 服务结算任务承接;
        bool 服务结算任务就绪 = 就绪;
        if (就绪 && 实现_->服务值结算配置) {
            服务结算任务承接 = 实现_->任务服务->承接需求任务(
                实现_->服务值结算配置->完成服务需求, false);
            服务结算任务就绪 = 服务结算任务承接->任务
                && 服务结算任务承接->虚拟存在
                && 服务结算任务承接->任务->需求节点
                    == 实现_->服务值结算配置->完成服务需求
                && 服务结算任务承接->虚拟存在->任务节点
                    == 服务结算任务承接->任务->节点
                && 服务结算任务承接->状态
                    == 新任务治理承接状态::已读回
                && (服务结算任务承接->任务->当前状态 == 新任务状态::已建立
                    || 服务结算任务承接->任务->当前状态 == 新任务状态::等待条件);
            if (服务结算任务就绪) {
                const auto& 配 = *实现_->服务值结算配置;
                const auto 配置状态 = 实现_->任务服务->配置服务结算任务({
                    服务结算任务承接->任务->节点, 配.登记方法校验节点,
                    配.服务根需求, 配.完成服务需求, 配.冻结前值, 配.目标值域概念节点});
                服务结算任务就绪 = 配置状态 == 新任务操作状态::已修改
                    || 配置状态 == 新任务操作状态::精确重复;
            }
            if (服务结算任务就绪) {
                const auto 准备 = 实现_->任务服务->准备服务结算筹办(服务结算任务承接->任务->节点);
                服务结算任务就绪 = 准备 == 新任务操作状态::已修改
                    || 准备 == 新任务操作状态::无变化 || 准备 == 新任务操作状态::精确重复;
            }
            if (服务结算任务就绪) {
                锁.lock();
                实现_->服务值结算任务 = 服务结算任务承接->任务->节点;
                实现_->调度待触发 = true;
                锁.unlock();
            }
        }
        const auto 恢复执行 = 就绪
            ? 实现_->任务服务->查询等待结果任务执行()
            : std::optional<std::vector<新任务治理执行读回结果>>{};
        bool 恢复完整 = 就绪 && 服务结算任务就绪
            && 恢复执行.has_value();
        if (就绪 && 恢复执行) {
            锁.lock();
            for (const auto& 读回 : *恢复执行) {
                if (!读回.任务 || !读回.执行
                    || 读回.任务->节点 != 读回.执行->任务节点
                    || 读回.任务->最近执行实例节点 != 读回.执行->节点
                    || (读回.状态 != 新任务治理执行读回状态::已读回等待观察
                        && 读回.状态 != 新任务治理执行读回状态::已读回等待评估
                        && 读回.状态 != 新任务治理执行读回状态::已读回影响评估)) {
                    恢复完整 = false;
                    break;
                }
                实现_->登记结果等待已加锁(读回.任务->节点, 读回.执行->节点);
            }
            锁.unlock();
        }
        const auto 工作启动 = 恢复完整
            ? 实现_->工作线程->启动({配置.邮箱容量}, 等待毫秒)
            : 新任务工作操作结果{
                新任务工作操作状态::尚未启动, 新任务工作生命周期::未启动};
        锁.lock();
        if (!恢复完整 || 实现_->停止
            || (工作启动.状态 != 新任务工作操作状态::已完成
                && 工作启动.状态 != 新任务工作操作状态::已在运行)) {
            实现_->生命周期 = 新任务管理生命周期::启动失败;
            实现_->已完成 = true;
            实现_->变化.notify_all();
            锁.unlock();
            (void)实现_->工作线程->请求停止();
            (void)实现_->工作线程->等待停止(等待毫秒);
            锁.lock();
            return 实现_->操作结果(新任务管理操作状态::服务未就绪);
        }
        try { 实现_->线程 = std::thread([p = 实现_.get()] { p->运行(); }); }
        catch (...) {
            实现_->生命周期 = 新任务管理生命周期::启动失败;
            实现_->已完成 = true;
            锁.unlock();
            (void)实现_->工作线程->请求停止();
            (void)实现_->工作线程->等待停止(等待毫秒);
            锁.lock();
            return 实现_->操作结果(新任务管理操作状态::资源失败);
        }
        if (!实现_->变化.wait_for(锁, 等待时长(等待毫秒), [this] {
            return 实现_->已进入 || 实现_->已完成;
        })) return 实现_->操作结果(新任务管理操作状态::等待超时);
        return 实现_->操作结果(实现_->已完成
            ? 新任务管理操作状态::内部错误 : 新任务管理操作状态::已完成);
    } catch (...) { return {新任务管理操作状态::内部错误, 新任务管理生命周期::内部错误}; }
}

新任务承接接收结果 新_任务管理线程类::提交已确认条件子需求(
    const 新任务筹办反馈定位& 父筹办定位,
    const 新任务承接消息& 消息) noexcept {
    新任务承接接收结果 结果;
    if (!实现_) { 结果.状态 = 新任务承接接收状态::资源失败; return 结果; }
    if (!父筹办定位.完整()
        || 父筹办定位.原工作.种类 != 新任务工作种类::检查条件
        || !消息.消息编号 || !有效(消息.需求节点)
        || 消息.依据 != 新任务承接依据::自我条件子需求
        || 消息.来源状态节点) return 结果;
    try {
        // 先分配整个消息，再取得邮箱锁；失败不占用子D交接槽。
        const auto 项 = std::make_shared<实现::消息项>();
        项->消息 = 消息;
        项->父筹办定位 = 父筹办定位;
        项->可处理时点 = std::chrono::steady_clock::now();
        std::lock_guard 锁(实现_->互斥);
        if (实现_->停止 || 实现_->已完成) {
            结果.状态 = 新任务承接接收状态::已停止接收; return 结果;
        }
        if (!实现_->已进入) {
            结果.状态 = 新任务承接接收状态::尚未启动; return 结果;
        }
        if (!实现_->子需求承接定位有效已加锁(*项)) {
            结果.状态 = 新任务承接接收状态::过期决议; return 结果;
        }
        auto& 记录 = 实现_->最近自我筹办裁决.at(父筹办定位.任务节点.值);
        const auto 位置 = std::find_if(记录.已确认子需求组.begin(),
            记录.已确认子需求组.end(), [&](const auto& 子项) {
                return 子项.需求节点 == 消息.需求节点;
            });
        const auto 序号 = static_cast<std::size_t>(
            std::distance(记录.已确认子需求组.begin(), 位置));
        if (序号 >= 记录.子需求承接组.size()) return 结果;
        if (const auto& 已有 = 记录.子需求承接组[序号]) {
            if (已有->消息 == 消息 && 已有->父筹办定位 == 父筹办定位) {
                结果.状态 = 新任务承接接收状态::精确重复;
                结果.处理结果 = 已有->结果;
            } else {
                结果.状态 = 新任务承接接收状态::身份冲突;
            }
            return 结果;
        }
        // 同种消息号不可在另一父任务或另一子D下复义。
        for (const auto& [任务值, 当前] : 实现_->最近自我筹办裁决) {
            (void)任务值;
            for (const auto& 已有 : 当前.子需求承接组) {
                if (已有 && 已有->消息.消息编号 == 消息.消息编号) {
                    结果.状态 = 新任务承接接收状态::身份冲突;
                    return 结果;
                }
            }
        }
        if (实现_->活动.size() >= 实现_->配置.邮箱容量) {
            结果.状态 = 新任务承接接收状态::邮箱已满; return 结果;
        }
        实现_->活动.push_back(项);
        记录.子需求承接组[序号] = 项;
        结果.状态 = 新任务承接接收状态::已接收;
        结果.处理结果 = 项->结果;
        实现_->变化.notify_all();
    } catch (const std::bad_alloc&) {
        结果.状态 = 新任务承接接收状态::资源失败;
    } catch (...) {
        结果.状态 = 新任务承接接收状态::内部错误;
    }
    return 结果;
}

新任务承接接收结果 新_任务管理线程类::提交消息(const 新任务承接消息& 消息) noexcept {
    新任务承接接收结果 结果;
    if (!实现_) { 结果.状态 = 新任务承接接收状态::资源失败; return 结果; }
    if (!消息.消息编号 || !有效(消息.需求节点)) return 结果;
    const bool 是首次自动 = 消息.依据 == 新任务承接依据::自我首次复核;
    const bool 是状态自动 = 消息.依据 == 新任务承接依据::自我状态变化复核;
    const bool 是扫描补齐 = 消息.依据 == 新任务承接依据::自我全树扫描;
    if ((消息.依据 == 新任务承接依据::显式请求 && 消息.来源状态节点)
        || (是首次自动 && 消息.来源状态节点)
        || (是状态自动 && (!消息.来源状态节点 || !有效(*消息.来源状态节点)))
        || (是扫描补齐 && 消息.来源状态节点)
        || (消息.依据 != 新任务承接依据::显式请求
            && !是首次自动 && !是状态自动 && !是扫描补齐)) return 结果;
    try {
        std::lock_guard 锁(实现_->互斥);
        if (实现_->停止 || 实现_->已完成) {
            结果.状态 = 新任务承接接收状态::已停止接收; return 结果;
        }
        if (!实现_->已进入) { 结果.状态 = 新任务承接接收状态::尚未启动; return 结果; }
        for (const auto& 项 : 实现_->活动) {
            if (项->消息.依据 != 消息.依据
                || 项->消息.消息编号 != 消息.消息编号) continue;
            if (项->消息 == 消息) {
                结果.状态 = 新任务承接接收状态::精确重复;
                结果.处理结果 = 项->结果;
            } else 结果.状态 = 新任务承接接收状态::身份冲突;
            return 结果;
        }
        if (是首次自动 || 是状态自动) {
            for (const auto& 记录 : 实现_->最近自动决议) {
                if (!记录 || 记录->消息.需求节点 != 消息.需求节点) continue;
                if (记录->消息.依据 == 消息.依据
                    && 记录->消息.来源状态节点 == 消息.来源状态节点) {
                    结果.状态 = 新任务承接接收状态::精确重复;
                    结果.处理结果 = 记录->结果;
                    return 结果;
                }
                if (是首次自动
                    || (记录->消息.来源状态节点 && 消息.来源状态节点
                        && 消息.来源状态节点->值 <= 记录->消息.来源状态节点->值)) {
                    结果.状态 = 新任务承接接收状态::过期决议;
                    return 结果;
                }
                break;
            }
            const auto 已占根数 = static_cast<std::size_t>(std::count_if(
                实现_->最近自动决议.begin(), 实现_->最近自动决议.end(),
                [](const auto& 记录) { return 记录.has_value(); }));
            const bool 已有本根 = std::any_of(
                实现_->最近自动决议.begin(), 实现_->最近自动决议.end(),
                [&](const auto& 记录) {
                    return 记录 && 记录->消息.需求节点 == 消息.需求节点;
                });
            if (!已有本根 && 已占根数 >= 实现_->最近自动决议.size()) {
                return 结果;
            }
        }
        if (实现_->活动.size() >= 实现_->配置.邮箱容量) {
            结果.状态 = 新任务承接接收状态::邮箱已满; return 结果;
        }
        const auto 项 = std::make_shared<实现::消息项>();
        项->消息 = 消息;
        项->可处理时点 = std::chrono::steady_clock::now();
        实现_->活动.push_back(项);
        结果.处理结果 = 项->结果;
        结果.状态 = 新任务承接接收状态::已接收;
        实现_->变化.notify_all();
        return 结果;
    } catch (const std::bad_alloc&) { 结果.状态 = 新任务承接接收状态::资源失败; }
    catch (...) { 结果.状态 = 新任务承接接收状态::内部错误; }
    return 结果;
}

新任务管理操作结果 新_任务管理线程类::请求停止() noexcept {
    if (!实现_) return {新任务管理操作状态::资源失败, 新任务管理生命周期::内部错误};
    try {
        std::lock_guard 锁(实现_->互斥);
        if (!实现_->创建已尝试) return 实现_->操作结果(新任务管理操作状态::尚未启动);
        if (实现_->停止 || 实现_->已完成) return 实现_->操作结果(新任务管理操作状态::无变化);
        实现_->停止 = true;
        实现_->生命周期 = 新任务管理生命周期::停止中;
        实现_->变化.notify_all();
        return 实现_->操作结果(新任务管理操作状态::已完成);
    } catch (...) { return {新任务管理操作状态::内部错误, 新任务管理生命周期::内部错误}; }
}

新任务管理操作结果 新_任务管理线程类::等待停止(std::uint64_t 等待毫秒) noexcept {
    if (!实现_) return {新任务管理操作状态::资源失败, 新任务管理生命周期::内部错误};
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        std::unique_lock 锁(实现_->互斥);
        if (!实现_->线程.joinable()) return 实现_->操作结果(实现_->已加入
            ? 新任务管理操作状态::无变化 : 新任务管理操作状态::尚未启动);
        if (实现_->线程.get_id() == std::this_thread::get_id())
            return 实现_->操作结果(新任务管理操作状态::不能等待自身);
        if (!实现_->变化.wait_for(锁, 等待时长(等待毫秒), [this] { return 实现_->已完成; }))
            return 实现_->操作结果(新任务管理操作状态::等待超时);
        锁.unlock();
        实现_->线程.join();
        锁.lock();
        实现_->已加入 = true;
        return 实现_->操作结果(实现_->生命周期 == 新任务管理生命周期::内部错误
            ? 新任务管理操作状态::内部错误 : 新任务管理操作状态::已完成);
    } catch (...) { return {新任务管理操作状态::内部错误, 新任务管理生命周期::内部错误}; }
}

bool 新_任务管理线程类::绑定方法学习读取(const 新_方法类& 方法) noexcept {
    if (!实现_) return false;
    try {
        std::lock_guard 锁(实现_->互斥);
        if (实现_->学习方法读取==&方法) return true;
        if (实现_->学习方法读取 || 实现_->创建已尝试) return false;
        实现_->学习方法读取=&方法;
        return true;
    } catch (...) { return false; }
}

新方法学习接收状态 新_任务管理线程类::提交方法学习结束(
    const 新学习结束通知& 结果, std::uint64_t 交接编号) noexcept {
    if (!实现_) return 新方法学习接收状态::资源失败;
    if (!有效(结果.方法节点) || !交接编号
        || 交接编号 == (std::numeric_limits<std::uint64_t>::max)())
        return 新方法学习接收状态::入口拒绝;
    try {
        std::lock_guard 锁(实现_->互斥);
        if (实现_->停止 || 实现_->已完成) return 新方法学习接收状态::已停止接收;
        if (!实现_->学习方法读取) return 新方法学习接收状态::入口拒绝;
        if (!实现_->已进入) return 新方法学习接收状态::尚未启动;
        const auto 旧 = 实现_->学习事件组.find(交接编号);
        if (旧 != 实现_->学习事件组.end())
            return 旧->second.通知.方法节点 == 结果.方法节点
                ? 新方法学习接收状态::精确重复 : 新方法学习接收状态::身份冲突;
        实现::学习事件 事件;
        事件.通知 = 结果;
        实现_->学习事件组.emplace(交接编号, std::move(事件));
        实现_->调度待触发 = true;
        实现_->变化.notify_all();
        return 新方法学习接收状态::已接收;
    } catch (const std::bad_alloc&) { return 新方法学习接收状态::资源失败; }
    catch (...) { return 新方法学习接收状态::内部错误; }
}

std::optional<新任务学习复核定位> 新_任务管理线程类::读取学习复核定位(
    std::uint64_t 复核编号) const noexcept {
    if (!实现_ || !复核编号) return std::nullopt;
    try {
        std::lock_guard 锁(实现_->互斥);
        const auto 位置 = 实现_->学习复核组.find(复核编号);
        if (实现_->停止 || 实现_->已完成 || 位置 == 实现_->学习复核组.end()
            || 位置->second.已终结) return std::nullopt;
        return 位置->second.定位;
    } catch (...) { return std::nullopt; }
}

std::optional<新任务等待复核定位> 新_任务管理线程类::读取等待复核定位(
    std::uint64_t 复核编号) const noexcept {
    if (!实现_ || !复核编号) return std::nullopt;
    try {
        std::lock_guard 锁(实现_->互斥);
        const auto 位置 = 实现_->等待复核组.find(复核编号);
        if (实现_->停止 || 实现_->已完成 || 位置 == 实现_->等待复核组.end()
            || 位置->second.已终结) return std::nullopt;
        return 位置->second.定位;
    } catch (...) { return std::nullopt; }
}

新自我等待复核接收状态 新_任务管理线程类::提交等待复核决议(
    const 新自我等待复核决议& 决议) noexcept {
    if (!实现_ || !决议.定位.完整()
        || 决议.状态 < 新自我等待复核状态::允许重新筹办
        || 决议.状态 > 新自我等待复核状态::需核查)
        return 新自我等待复核接收状态::入口拒绝;
    if (决议.状态 == 新自我等待复核状态::允许重新筹办
        && (!决议.当前满足状态
            || (*决议.当前满足状态 != 新需求当前满足状态::需增加
                && *决议.当前满足状态 != 新需求当前满足状态::需减少
                && *决议.当前满足状态 != 新需求当前满足状态::未满足但无有向差值
                && *决议.当前满足状态 != 新需求当前满足状态::需要执行前后比较)))
        return 新自我等待复核接收状态::入口拒绝;
    try {
        std::lock_guard 锁(实现_->互斥);
        if (实现_->停止 || 实现_->已完成) return 新自我等待复核接收状态::已停止接收;
        const auto 位置 = 实现_->等待复核组.find(决议.定位.复核编号);
        if (位置 == 实现_->等待复核组.end()) return 新自我等待复核接收状态::定位已变化;
        auto& 记录 = 位置->second;
        if (记录.定位 != 决议.定位) return 新自我等待复核接收状态::身份冲突;
        if (记录.决议) return *记录.决议 == 决议
            ? 新自我等待复核接收状态::精确重复 : 新自我等待复核接收状态::身份冲突;
        if (记录.已终结 || !实现_->等待复核占用.contains(决议.定位.定位.任务节点.值))
            return 新自我等待复核接收状态::定位已变化;
        记录.决议 = 决议;
        实现_->变化.notify_all();
        return 新自我等待复核接收状态::已接收;
    } catch (const std::bad_alloc&) { return 新自我等待复核接收状态::资源失败; }
    catch (...) { return 新自我等待复核接收状态::内部错误; }
}

新自我学习复核接收状态 新_任务管理线程类::提交学习复核决议(
    const 新自我学习复核决议& 决议) noexcept {
    if (!实现_ || !决议.定位.完整()) return 新自我学习复核接收状态::入口拒绝;
    if (决议.状态 < 新自我学习复核状态::允许重新筹办
        || 决议.状态 > 新自我学习复核状态::需核查)
        return 新自我学习复核接收状态::入口拒绝;
    if (决议.状态 == 新自我学习复核状态::允许重新筹办
        && (决议.当前满足状态 != 新需求当前满足状态::需增加
            && 决议.当前满足状态 != 新需求当前满足状态::需减少
            && 决议.当前满足状态 != 新需求当前满足状态::未满足但无有向差值
            && 决议.当前满足状态 != 新需求当前满足状态::需要执行前后比较))
        return 新自我学习复核接收状态::入口拒绝;
    try {
        std::lock_guard 锁(实现_->互斥);
        if (实现_->停止 || 实现_->已完成) return 新自我学习复核接收状态::已停止接收;
        const auto 位置 = 实现_->学习复核组.find(决议.定位.复核编号);
        if (位置 == 实现_->学习复核组.end()) return 新自我学习复核接收状态::定位已变化;
        auto& 记录 = 位置->second;
        if (记录.定位 != 决议.定位) return 新自我学习复核接收状态::身份冲突;
        if (记录.决议) return *记录.决议 == 决议
            ? 新自我学习复核接收状态::精确重复 : 新自我学习复核接收状态::身份冲突;
        if (记录.已终结 || !实现_->学习复核占用.contains(决议.定位.任务节点.值))
            return 新自我学习复核接收状态::定位已变化;
        记录.决议 = 决议;
        实现_->变化.notify_all();
        return 新自我学习复核接收状态::已接收;
    } catch (const std::bad_alloc&) { return 新自我学习复核接收状态::资源失败; }
    catch (...) { return 新自我学习复核接收状态::内部错误; }
}

新任务管理快照 新_任务管理线程类::读取快照() const noexcept {
    新任务管理快照 结果;
    if (!实现_) { 结果.生命周期 = 新任务管理生命周期::内部错误; return 结果; }
    try {
        std::lock_guard 锁(实现_->互斥);
        结果.生命周期 = 实现_->生命周期;
        结果.最近方法学习结束=实现_->最近方法学习结束;
        结果.最近学习运行异常=实现_->最近学习运行异常;
        结果.等待方法实例消费 = !实现_->已完成 && std::any_of(
            实现_->学习事件组.begin(), 实现_->学习事件组.end(),
            [](const auto& 项) { return !项.second.已终结; });
        结果.活动消息数 = 实现_->活动.size();
        结果.活动工作数 = 实现_->活动工作.size();
        结果.等待复核任务数 = 实现_->等待复核占用.size();
        结果.调度扫描在途 = std::any_of(实现_->活动工作.begin(), 实现_->活动工作.end(),
            [](const auto& 项) { return 项.请求.种类 == 新任务工作种类::读取任务调度材料; });
        for (const auto& [任务值, 项] : 实现_->调度材料) {
            if (项.状态 == 新任务调度项状态::普通可筹办
                && !实现_->筹办阻塞任务.contains(任务值)
                && !实现_->任务有执行保护已加锁(任务值)
                && !实现_->学习复核占用.contains(任务值)
                && !实现_->等待复核占用.contains(任务值)) ++结果.就绪工作数;
        }
        for (const auto& [任务值, 项] : 实现_->最近自我筹办裁决) {
            if (项.已完成接收 && 项.执行工作 && !项.执行工作已派发
                && !实现_->学习占用或旧筹办已失效已加锁(项.裁决.定位)
                && !std::any_of(实现_->活动工作.begin(), 实现_->活动工作.end(),
                    [&](const auto& 工作) { return 工作.请求.任务节点.值 == 任务值; })) ++结果.就绪工作数;
        }
        for (const auto& [任务值, 项] : 实现_->结果等待任务) {
            if (!项.已派发 && (项.待检测 || 实现_->结果通知待处理
                    || std::chrono::steady_clock::now() >= 项.下次检测时点)
                && !std::any_of(实现_->活动工作.begin(), 实现_->活动工作.end(),
                    [&](const auto& 工作) { return 工作.请求.任务节点.值 == 任务值; })) ++结果.就绪工作数;
        }
        结果.待反馈任务数 = 实现_->待反馈任务.size();
        结果.自我筹办裁决数 = static_cast<std::uint64_t>(std::count_if(
            实现_->最近自我筹办裁决.begin(),
            实现_->最近自我筹办裁决.end(), [](const auto& 项) {
                return 项.second.已完成接收;
            }));
        结果.执行结果数 = 实现_->最近执行结果.size();
        结果.停止已请求 = 实现_->停止;
        结果.线程已完成 = 实现_->已完成;
        if (实现_->服务值结算配置) {
            结果.服务值结算需求 =
                实现_->服务值结算配置->完成服务需求;
        }
        结果.服务值结算任务 = 实现_->服务值结算任务;
        结果.服务值结算方法已配置 =
            实现_->服务值结算配置.has_value();
    } catch (...) { 结果.生命周期 = 新任务管理生命周期::内部错误; }
    return 结果;
}

std::optional<新任务筹办处理结果> 新_任务管理线程类::读取最近筹办结果(
    稳定编码 任务节点) const noexcept {
    if (!实现_ || !有效(任务节点)) return std::nullopt;
    try {
        std::lock_guard 锁(实现_->互斥);
        const auto 位置 = 实现_->最近筹办结果.find(任务节点.值);
        return 位置 == 实现_->最近筹办结果.end()
                || 实现_->学习占用或旧筹办已失效已加锁({任务节点, 位置->second.原工作})
            ? std::nullopt
            : std::optional<新任务筹办处理结果>{位置->second};
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<新自我筹办裁决> 新_任务管理线程类::读取最近自我筹办裁决(
    稳定编码 任务节点) const noexcept {
    if (!实现_ || !有效(任务节点)) return std::nullopt;
    try {
        std::lock_guard 锁(实现_->互斥);
        const auto 位置 = 实现_->最近自我筹办裁决.find(任务节点.值);
        return 位置 == 实现_->最近自我筹办裁决.end()
                || !位置->second.已完成接收
            ? std::nullopt
            : std::optional<新自我筹办裁决>{位置->second.裁决};
    } catch (...) {
        return std::nullopt;
    }
}

std::optional<新任务执行处理结果> 新_任务管理线程类::读取最近执行结果(
    稳定编码 任务节点) const noexcept {
    if (!实现_ || !有效(任务节点)) return std::nullopt;
    try {
        std::lock_guard 锁(实现_->互斥);
        const auto 位置 = 实现_->最近执行结果.find(任务节点.值);
        return 位置 == 实现_->最近执行结果.end()
            ? std::nullopt
            : std::optional<新任务执行处理结果>{位置->second};
    } catch (...) {
        return std::nullopt;
    }
}

} // namespace 海中鱼巣
