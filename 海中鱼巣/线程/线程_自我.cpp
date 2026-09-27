#include "线程_自我.h"

#include <chrono>
#include <condition_variable>
#include <deque>
#include <limits>
#include <mutex>
#include <new>
#include <thread>

namespace 海中鱼巣 {

bool 自我线程消息身份::完整() const noexcept { return 值 != 0; }
bool 自我线程原请求身份::完整() const noexcept { return 值 != 0; }
bool 自我线程需求身份::完整() const noexcept { return 值 != 0; }

namespace {

std::chrono::milliseconds 有界等待时长(std::uint64_t value) noexcept {
    using rep = std::chrono::milliseconds::rep;
    constexpr auto maximum = (std::numeric_limits<rep>::max)();
    return std::chrono::milliseconds{
        value > static_cast<std::uint64_t>(maximum)
            ? maximum : static_cast<rep>(value)};
}

bool 触发根有效(自我线程复核触发根 value) noexcept {
    return value == 自我线程复核触发根::双根 ||
        value == 自我线程复核触发根::安全根 ||
        value == 自我线程复核触发根::服务根;
}

自我根复核触发根 转换触发根(自我线程复核触发根 value) noexcept {
    switch (value) {
    case 自我线程复核触发根::安全根: return 自我根复核触发根::安全根;
    case 自我线程复核触发根::服务根: return 自我根复核触发根::服务根;
    default: return 自我根复核触发根::双根;
    }
}

bool 初始化包完整(const 不可变本能根任务初始化包_v1& package) noexcept {
    const auto& request = package.原请求;
    return 有效(request.意图.值) &&
        (request.来源.根角色 == 本能根角色::安全 ||
         request.来源.根角色 == 本能根角色::服务) &&
        有效(request.来源.D.值) && 有效(request.来源.L) &&
        有效(request.来源.根形成F) && 有效(package.预留记录.值) &&
        package.预留序号 && 有效(package.控制幂等身份) &&
        有效(package.任务核心建立幂等身份) && 有效(package.P1建立幂等身份) &&
        有效(package.Vt首迁移幂等身份);
}

bool 来源匹配(const 不可变本能根任务初始化包_v1& package,
              本能根任务初始化意图身份_v1 intent,
              本能根角色 role,
              const 自我线程单根复核投影& root) noexcept {
    return 初始化包完整(package) && package.原请求.意图 == intent &&
        package.原请求.来源.根角色 == role &&
        package.原请求.来源.D.值 == root.根材料.根需求 &&
        package.原请求.来源.L == root.根材料.根列表项 &&
        package.原请求.来源.根形成F.编码 == root.根材料.实际特征;
}

enum class 消息处理结论 : std::uint8_t {
    成功, 可恢复, 根初始化代际已结束, 入口拒绝, 消息冲突,
    依赖未就绪, 资源失败, 内部错误,
};

自我线程操作状态 映射操作状态(消息处理结论 value) noexcept {
    switch (value) {
    case 消息处理结论::成功: return 自我线程操作状态::成功;
    case 消息处理结论::根初始化代际已结束:
        return 自我线程操作状态::根初始化代际已结束;
    case 消息处理结论::消息冲突: return 自我线程操作状态::消息冲突;
    case 消息处理结论::依赖未就绪:
    case 消息处理结论::可恢复: return 自我线程操作状态::依赖未就绪;
    case 消息处理结论::资源失败: return 自我线程操作状态::资源失败;
    case 消息处理结论::内部错误: return 自我线程操作状态::内部错误;
    default: return 自我线程操作状态::入口拒绝;
    }
}

自我线程外部调用状态 映射上下文状态(自我线程正式上下文状态 value) noexcept {
    switch (value) {
    case 自我线程正式上下文状态::成功: return 自我线程外部调用状态::成功;
    case 自我线程正式上下文状态::精确重复: return 自我线程外部调用状态::精确重复;
    case 自我线程正式上下文状态::未实现: return 自我线程外部调用状态::待实现;
    case 自我线程正式上下文状态::依赖未就绪: return 自我线程外部调用状态::依赖未就绪;
    case 自我线程正式上下文状态::引用冲突: return 自我线程外部调用状态::引用冲突;
    case 自我线程正式上下文状态::不可比较: return 自我线程外部调用状态::不可比较;
    case 自我线程正式上下文状态::资源失败: return 自我线程外部调用状态::资源失败;
    case 自我线程正式上下文状态::内部错误: return 自我线程外部调用状态::内部错误;
    default: return 自我线程外部调用状态::入口拒绝;
    }
}

} // namespace

bool 自我线程创建请求_v1::完整() const noexcept {
    if (!邮箱容量 || !本能根.完整() ||
        !有效(正式上下文.期望世界根) || !有效(正式上下文.期望自我) ||
        !有效(正式上下文.期望所在场景) || !有效(正式上下文.角色.值) ||
        !有效(正式上下文.安全实际特征) || !有效(正式上下文.服务实际特征))
        return false;
    return 本能根.自我 == 正式上下文.期望自我 &&
        本能根.安全根.实际特征 == 正式上下文.安全实际特征 &&
        本能根.服务根.实际特征 == 正式上下文.服务实际特征;
}

bool 自我线程停门见证_v1::完整() const noexcept {
    return 入口序号 && 停门序号 > 入口序号 && !治理运行门开启 &&
        已冻结批次数量 == 0;
}

bool 自我线程创建结果_v1::成功() const noexcept {
    return (状态 == 自我线程操作状态::成功 ||
            状态 == 自我线程操作状态::精确重复) &&
        生命周期 == 自我线程生命周期状态::已停门 && 见证 &&
        见证->完整() && !写业务事实;
}

bool 自我线程操作结果_v1::成功() const noexcept {
    return (状态 == 自我线程操作状态::成功 ||
            状态 == 自我线程操作状态::精确重复) && !写业务事实;
}

struct 自我线程::实现 final {
    mutable std::mutex 状态互斥;
    std::condition_variable 状态变化;
    std::mutex 加入互斥;
    std::deque<自我线程根需求复核消息_v2> 邮箱, 冻结批;
    std::optional<自我线程根需求复核消息_v2> 当前处理消息;
    std::optional<自我线程创建请求_v1> 锁定请求;
    自我线程正式上下文提供者* 正式上下文端口 = nullptr;
    自我线程根需求复核端口* 根需求复核端口 = nullptr;
    本能根任务核心端口_v1* 任务核心端口 = nullptr;
    自我线程根治理意图接收端口_v1* 根治理意图接收端口 = nullptr;
    std::optional<自我线程停门见证_v1> 停门见证;
    std::thread 工作线程;
    自我线程生命周期状态 生命周期 = 自我线程生命周期状态::未创建;
    自我线程外部调用状态 最近外部调用 = 自我线程外部调用状态::待实现;
    std::uint64_t 事件序号=0, 输出消息序号=0, 已冻结批次数量=0,
                  成功治理批次数量=0;
    bool 创建已尝试=false, 开门请求已登记=false, 开门结果已形成=false,
         治理运行门开启=false, 线程已进入=false, 线程已完成=false,
         停止已请求=false, 内部错误锁存=false;
    自我线程操作状态 开门结果状态 = 自我线程操作状态::入口拒绝;

    自我线程操作结果_v1 操作结果(自我线程操作状态 state) const noexcept {
        return {state, 生命周期, false};
    }

    bool 下一输出消息身份(自我线程消息身份& output) noexcept {
        if (输出消息序号 == (std::numeric_limits<std::uint64_t>::max)()) return false;
        output.值 = ++输出消息序号;
        return output.完整();
    }

    消息处理结论 交付单根(
        const 自我线程根需求复核消息_v2& message, 本能根角色 role,
        const 自我线程单根复核投影& root,
        本能根任务初始化意图身份_v1 intent,
        std::optional<自我线程消息身份>& outputIdentity,
        bool& delivered) noexcept {
        if (delivered) return 消息处理结论::成功;

        const 本能根任务初始化包按意图读取请求_v1 readRequest{intent};
        const auto read = 任务核心端口->按初始化意图读取不可变包(readRequest);
        std::optional<不可变本能根任务初始化包_v1> package;
        if (read.状态 == 本能根任务阶段状态_v1::当前任务不可复用) {
            if (read.意图 != intent || !read.包 ||
                !来源匹配(*read.包, intent, role, root))
                return 消息处理结论::内部错误;
            return 消息处理结论::根初始化代际已结束;
        }
        if (read.状态 == 本能根任务阶段状态_v1::资源失败 ||
            read.状态 == 本能根任务阶段状态_v1::已可能发布 ||
            read.状态 == 本能根任务阶段状态_v1::未实现)
            return 消息处理结论::可恢复;
        if (read.状态 == 本能根任务阶段状态_v1::已读取) {
            if (read.意图 != intent || !read.包 ||
                !来源匹配(*read.包, intent, role, root))
                return 消息处理结论::消息冲突;
            package = read.包;
        } else if (read.状态 == 本能根任务阶段状态_v1::未找到) {
            if (read.意图 != intent || read.包) return 消息处理结论::内部错误;
            if (root.业务结果 == 自我线程根复核业务结果::当前满足) {
                delivered = true;
                return 消息处理结论::成功;
            }
            if (root.业务结果 != 自我线程根复核业务结果::正差距)
                return 消息处理结论::内部错误;
            const 本能根任务初始化语义请求_v1 issueRequest{
                intent,
                {role, 需求类记录身份{root.根材料.根需求},
                 root.根材料.根列表项,
                 特征信息身份{root.根材料.实际特征}},
                std::nullopt};
            const auto issue = 任务核心端口->签发或恢复不可变初始化包(issueRequest);
            if (!issue.成功(issueRequest)) {
                if (issue.状态 == 本能根任务阶段状态_v1::资源失败 ||
                    issue.状态 == 本能根任务阶段状态_v1::已可能发布 ||
                    issue.状态 == 本能根任务阶段状态_v1::未实现)
                    return 消息处理结论::可恢复;
                return issue.状态 == 本能根任务阶段状态_v1::入口拒绝
                    ? 消息处理结论::入口拒绝 : 消息处理结论::内部错误;
            }
            if (!issue.包 || !来源匹配(*issue.包, intent, role, root))
                return 消息处理结论::消息冲突;
            package = issue.包;
        } else {
            return read.状态 == 本能根任务阶段状态_v1::入口拒绝
                ? 消息处理结论::入口拒绝 : 消息处理结论::内部错误;
        }

        if (!package) return 消息处理结论::内部错误;
        if (!outputIdentity) {
            自我线程消息身份 identity;
            if (!下一输出消息身份(identity)) return 消息处理结论::内部错误;
            outputIdentity = identity;
        }
        const 自我到任务管理本能根承接消息_v1 delivery{
            *outputIdentity, message.消息, message.原请求, *package};
        const auto accepted = 根治理意图接收端口->提交(delivery);
        if (accepted.消息 != *outputIdentity) return 消息处理结论::内部错误;
        if (accepted.状态 == 根治理意图投递状态_v1::已接收 ||
            accepted.状态 == 根治理意图投递状态_v1::精确重复) {
            delivered = true;
            return 消息处理结论::成功;
        }
        if (accepted.状态 == 根治理意图投递状态_v1::队列已满 ||
            accepted.状态 == 根治理意图投递状态_v1::尚未就绪 ||
            accepted.状态 == 根治理意图投递状态_v1::资源失败)
            return 消息处理结论::可恢复;
        if (accepted.状态 == 根治理意图投递状态_v1::消息冲突)
            return 消息处理结论::消息冲突;
        return accepted.状态 == 根治理意图投递状态_v1::正在停止
            ? 消息处理结论::依赖未就绪 : 消息处理结论::入口拒绝;
    }

    消息处理结论 处理消息(const 自我线程根需求复核消息_v2& message) noexcept {
        std::optional<自我线程消息身份> safetyOutput, serviceOutput;
        bool safetyDelivered=false, serviceDelivered=false;
        for (;;) {
            {
                std::lock_guard lock{状态互斥};
                if (停止已请求) return 消息处理结论::依赖未就绪;
            }
            const auto context = 正式上下文端口->读取正式上下文(锁定请求->正式上下文);
            {
                std::lock_guard lock{状态互斥};
                最近外部调用 = 映射上下文状态(context.状态);
            }
            if (!context.成功(锁定请求->正式上下文)) {
                if (context.状态 != 自我线程正式上下文状态::未实现 &&
                    context.状态 != 自我线程正式上下文状态::依赖未就绪 &&
                    context.状态 != 自我线程正式上下文状态::资源失败)
                    return context.状态 == 自我线程正式上下文状态::引用冲突
                        ? 消息处理结论::消息冲突
                        : (context.状态 == 自我线程正式上下文状态::内部错误
                            ? 消息处理结论::内部错误 : 消息处理结论::入口拒绝);
            } else {
                const auto& projection = *context.投影;
                const auto& anchor = 锁定请求->本能根;
                const bool anchorMatches = projection.世界 == 锁定请求->正式上下文.期望世界根 &&
                    projection.自我所在场景 == 锁定请求->正式上下文.期望所在场景 &&
                    projection.自我 == anchor.自我 &&
                    projection.安全根.根需求 == anchor.安全根.需求 &&
                    projection.安全根.根列表项 == anchor.安全根.列表项 &&
                    projection.安全根.实际特征 == anchor.安全根.实际特征.编码 &&
                    projection.安全根.根目标合同 == anchor.安全根.目标合同 &&
                    projection.服务根.根需求 == anchor.服务根.需求 &&
                    projection.服务根.根列表项 == anchor.服务根.列表项 &&
                    projection.服务根.实际特征 == anchor.服务根.实际特征.编码 &&
                    projection.服务根.根目标合同 == anchor.服务根.目标合同;
                if (!anchorMatches) return 消息处理结论::消息冲突;

                const 自我线程根需求复核请求 reviewRequest{
                    projection, 转换触发根(message.触发根),
                    稳定编码{message.正式需求定位.值},
                    稳定编码{message.消息.值}, 稳定编码{message.原请求.值}};
                const auto review = 根需求复核端口->复核双根当前需求(reviewRequest);
                if (review.成功(reviewRequest)) {
                    消息处理结论 outcome = 消息处理结论::成功;
                    if (message.触发根 == 自我线程复核触发根::双根 ||
                        message.触发根 == 自我线程复核触发根::安全根)
                        outcome = 交付单根(message, 本能根角色::安全,
                            *review.安全根, message.安全根任务意图,
                            safetyOutput, safetyDelivered);
                    if (outcome == 消息处理结论::成功 &&
                        (message.触发根 == 自我线程复核触发根::双根 ||
                         message.触发根 == 自我线程复核触发根::服务根))
                        outcome = 交付单根(message, 本能根角色::服务,
                            *review.服务根, message.服务根任务意图,
                            serviceOutput, serviceDelivered);
                    if (outcome == 消息处理结论::成功) return outcome;
                    if (outcome != 消息处理结论::可恢复) return outcome;
                } else if (review.状态 != 自我线程根需求复核状态::未实现 &&
                           review.状态 != 自我线程根需求复核状态::资源失败) {
                    return review.状态 == 自我线程根需求复核状态::内部不一致
                        ? 消息处理结论::内部错误 : 消息处理结论::入口拒绝;
                }
            }
            std::unique_lock lock{状态互斥};
            生命周期 = 治理运行门开启
                ? 自我线程生命周期状态::治理中
                : 自我线程生命周期状态::等待依赖;
            if (状态变化.wait_for(lock,
                    有界等待时长(根治理可恢复失败重试间隔毫秒_v1),
                    [this]{ return 停止已请求; }))
                return 消息处理结论::依赖未就绪;
        }
    }

    void 运行() noexcept {
        try {
            std::unique_lock lock{状态互斥};
            线程已进入=true;
            const auto enter=++事件序号;
            if (!停止已请求 && 锁定请求) {
                生命周期=自我线程生命周期状态::已停门;
                停门见证=自我线程停门见证_v1{enter,++事件序号,false,0};
                状态变化.notify_all();
            }
            for (;;) {
                状态变化.wait(lock,[this]{return 停止已请求 ||
                    (开门请求已登记&&!开门结果已形成) ||
                    (治理运行门开启&&!邮箱.empty());});
                if (停止已请求) break;
                const bool first=!治理运行门开启;
                if (first && (!开门请求已登记 || 开门结果已形成 ||
                    !根治理意图接收端口 || !根治理意图接收端口->已就绪() ||
                    邮箱.empty())) {
                    if (!根治理意图接收端口 || !根治理意图接收端口->已就绪()) {
                        最近外部调用=自我线程外部调用状态::依赖未就绪;
                        生命周期=自我线程生命周期状态::等待依赖;
                    } else if (邮箱.empty()) {
                        开门结果已形成=true;
                        开门结果状态=自我线程操作状态::入口拒绝;
                        生命周期=自我线程生命周期状态::已停门;
                    }
                    状态变化.notify_all();
                    if (!开门结果已形成)
                        状态变化.wait_for(lock,
                            有界等待时长(根治理可恢复失败重试间隔毫秒_v1),
                            [this]{return 停止已请求;});
                    continue;
                }
                if (first && (邮箱.size()!=1 ||
                    邮箱.front().触发根!=自我线程复核触发根::双根 ||
                    邮箱.front().正式需求定位.完整())) {
                    开门结果已形成=true;
                    开门结果状态=自我线程操作状态::入口拒绝;
                    生命周期=自我线程生命周期状态::已停门;
                    状态变化.notify_all();
                    continue;
                }
                冻结批.clear();
                冻结批.swap(邮箱);
                auto batch=冻结批;
                ++已冻结批次数量;
                lock.unlock();
                消息处理结论 outcome=消息处理结论::成功;
                while (!batch.empty()) {
                    { std::lock_guard stateLock{状态互斥}; 当前处理消息=batch.front(); }
                    outcome=处理消息(batch.front());
                    { std::lock_guard stateLock{状态互斥}; 当前处理消息.reset(); }
                    if (outcome!=消息处理结论::成功) break;
                    batch.pop_front();
                }
                lock.lock();
                if (停止已请求) break;
                if (outcome==消息处理结论::成功 && batch.empty()) {
                    ++成功治理批次数量;
                    最近外部调用=自我线程外部调用状态::成功;
                    if (first) {
                        治理运行门开启=true;
                        开门结果已形成=true;
                        开门结果状态=自我线程操作状态::成功;
                    }
                    生命周期=自我线程生命周期状态::治理中;
                } else {
                    // 未完成批次放回邮箱头部；不得因下层未实现或暂时不可用丢失消息。
                    while (!batch.empty()) { 邮箱.push_front(std::move(batch.back())); batch.pop_back(); }
                    开门结果已形成=first;
                    开门结果状态=映射操作状态(outcome);
                    if (outcome==消息处理结论::内部错误) {
                        内部错误锁存=true;
                        停止已请求=true;
                        生命周期=自我线程生命周期状态::内部错误;
                        最近外部调用=自我线程外部调用状态::内部错误;
                    } else {
                        生命周期=first ? 自我线程生命周期状态::已停门
                                        : 自我线程生命周期状态::等待依赖;
                    }
                }
                冻结批.clear();
                状态变化.notify_all();
            }
            if (!内部错误锁存) 生命周期=自我线程生命周期状态::正在停止;
            线程已完成=true;
            ++事件序号;
            if (!内部错误锁存) 生命周期=自我线程生命周期状态::已停止;
            状态变化.notify_all();
        } catch (...) {
            std::lock_guard lock{状态互斥};
            内部错误锁存=true;停止已请求=true;开门结果已形成=true;
            开门结果状态=自我线程操作状态::内部错误;
            线程已完成=true;生命周期=自我线程生命周期状态::内部错误;
            最近外部调用=自我线程外部调用状态::内部错误;
            状态变化.notify_all();
        }
    }

    bool 加入工作线程() noexcept {
        std::lock_guard joinLock{加入互斥};
        if (!工作线程.joinable()) return false;
        try { 工作线程.join(); return true; }
        catch (...) { return false; }
    }
};

自我线程::自我线程() noexcept : 实现_(new(std::nothrow) 实现{}) {}

自我线程::~自我线程() noexcept {
    if (!实现_) return;
    (void)请求停止();
    (void)实现_->加入工作线程();
    delete 实现_;
    实现_=nullptr;
}

自我线程创建结果_v1 自我线程::创建并停在治理运行门(
    const 自我线程创建请求_v1& request,
    自我线程正式上下文提供者& context,
    自我线程根需求复核端口& review,
    本能根任务核心端口_v1& task,
    自我线程根治理意图接收端口_v1& manager,
    std::uint64_t wait) noexcept {
    if (!实现_ || !request.完整() || !wait) return {};
    try {
        std::unique_lock lock{实现_->状态互斥};
        if (实现_->创建已尝试) {
            if (!实现_->锁定请求 || *实现_->锁定请求!=request ||
                实现_->正式上下文端口!=&context || 实现_->根需求复核端口!=&review ||
                实现_->任务核心端口!=&task || 实现_->根治理意图接收端口!=&manager)
                return {自我线程操作状态::选择冲突,实现_->生命周期,std::nullopt,false};
            if (实现_->停门见证 && 实现_->停门见证->完整())
                return {自我线程操作状态::精确重复,实现_->生命周期,
                        实现_->停门见证,false};
            return {};
        }
        实现_->创建已尝试=true;
        实现_->锁定请求=request;
        实现_->正式上下文端口=&context;
        实现_->根需求复核端口=&review;
        实现_->任务核心端口=&task;
        实现_->根治理意图接收端口=&manager;
        实现_->生命周期=自我线程生命周期状态::创建中;
        lock.unlock();
        try { 实现_->工作线程=std::thread([p=实现_]{p->运行();}); }
        catch (...) {
            lock.lock();
            实现_->生命周期=自我线程生命周期状态::启动失败;
            实现_->线程已完成=true;
            return {自我线程操作状态::资源失败,实现_->生命周期,std::nullopt,false};
        }
        lock.lock();
        if (!实现_->状态变化.wait_for(lock,有界等待时长(wait),[&]{
                return 实现_->停门见证.has_value() || 实现_->线程已完成;}))
            return {自我线程操作状态::等待超时,实现_->生命周期,std::nullopt,false};
        if (实现_->停门见证 && 实现_->停门见证->完整())
            return {自我线程操作状态::成功,实现_->生命周期,实现_->停门见证,false};
        return {实现_->内部错误锁存 ? 自我线程操作状态::内部错误
                                     : 自我线程操作状态::依赖未就绪,
                实现_->生命周期,std::nullopt,false};
    } catch (...) { return {自我线程操作状态::内部错误,
                            自我线程生命周期状态::内部错误,std::nullopt,false}; }
}

自我线程操作结果_v1 自我线程::复核前置并开放治理运行门(
    std::uint64_t wait) noexcept {
    if (!实现_ || !wait) return {};
    try {
        std::unique_lock lock{实现_->状态互斥};
        if (!实现_->线程已进入 || 实现_->线程已完成 || 实现_->停止已请求)
            return 实现_->操作结果(自我线程操作状态::依赖未就绪);
        if (实现_->治理运行门开启)
            return 实现_->操作结果(自我线程操作状态::精确重复);
        实现_->开门请求已登记=true;
        实现_->开门结果已形成=false;
        实现_->状态变化.notify_all();
        if (!实现_->状态变化.wait_for(lock,有界等待时长(wait),[&]{
                return 实现_->开门结果已形成 || 实现_->线程已完成;}))
            return 实现_->操作结果(自我线程操作状态::等待超时);
        return 实现_->操作结果(实现_->开门结果状态);
    } catch (...) { return {自我线程操作状态::内部错误,
                            自我线程生命周期状态::内部错误,false}; }
}

自我线程操作结果_v1 自我线程::提交根需求复核消息(
    const 自我线程根需求复核消息_v2& message) noexcept {
    if (!实现_ || !message.消息.完整() || !message.原请求.完整() ||
        !触发根有效(message.触发根)) return {};
    const bool needsSafety = message.触发根==自我线程复核触发根::双根 ||
                             message.触发根==自我线程复核触发根::安全根;
    const bool needsService = message.触发根==自我线程复核触发根::双根 ||
                              message.触发根==自我线程复核触发根::服务根;
    if ((needsSafety&&!有效(message.安全根任务意图.值)) ||
        (needsService&&!有效(message.服务根任务意图.值))) return {};
    try {
        std::lock_guard lock{实现_->状态互斥};
        if (!实现_->锁定请求 || 实现_->停止已请求 || 实现_->线程已完成)
            return 实现_->操作结果(自我线程操作状态::依赖未就绪);
        const auto same=[&](const auto& item){return item.消息==message.消息;};
        if (实现_->当前处理消息 && same(*实现_->当前处理消息))
            return 实现_->操作结果(*实现_->当前处理消息==message
                ? 自我线程操作状态::精确重复 : 自我线程操作状态::消息冲突);
        for (const auto& item:实现_->邮箱) if (same(item))
            return 实现_->操作结果(item==message
                ? 自我线程操作状态::精确重复 : 自我线程操作状态::消息冲突);
        for (const auto& item:实现_->冻结批) if (same(item))
            return 实现_->操作结果(item==message
                ? 自我线程操作状态::精确重复 : 自我线程操作状态::消息冲突);
        if (实现_->邮箱.size()>=实现_->锁定请求->邮箱容量)
            return 实现_->操作结果(自我线程操作状态::队列已满);
        实现_->邮箱.push_back(message);
        实现_->状态变化.notify_all();
        return 实现_->操作结果(自我线程操作状态::成功);
    } catch (const std::bad_alloc&) {
        return {自我线程操作状态::资源失败,自我线程生命周期状态::内部错误,false};
    } catch (...) {
        return {自我线程操作状态::内部错误,自我线程生命周期状态::内部错误,false};
    }
}

自我线程操作结果_v1 自我线程::请求停止() noexcept {
    if (!实现_) return {};
    try {
        std::lock_guard lock{实现_->状态互斥};
        if (!实现_->创建已尝试) return {};
        if (实现_->停止已请求)
            return 实现_->操作结果(自我线程操作状态::精确重复);
        实现_->停止已请求=true;
        if (!实现_->线程已完成)
            实现_->生命周期=自我线程生命周期状态::正在停止;
        实现_->状态变化.notify_all();
        return 实现_->操作结果(自我线程操作状态::成功);
    } catch (...) { return {自我线程操作状态::内部错误,
                            自我线程生命周期状态::内部错误,false}; }
}

自我线程操作结果_v1 自我线程::等待停止(std::uint64_t wait) noexcept {
    if (!实现_ || !wait) return {};
    try {
        std::unique_lock lock{实现_->状态互斥};
        if (!实现_->状态变化.wait_for(lock,有界等待时长(wait),[&]{
                return 实现_->线程已完成;}))
            return 实现_->操作结果(自我线程操作状态::等待超时);
        lock.unlock();
        if (!实现_->加入工作线程())
            return {自我线程操作状态::内部错误,
                    自我线程生命周期状态::内部错误,false};
        return {自我线程操作状态::成功,自我线程生命周期状态::已停止,false};
    } catch (...) { return {自我线程操作状态::内部错误,
                            自我线程生命周期状态::内部错误,false}; }
}

自我线程诊断快照_v1 自我线程::读取诊断快照() const noexcept {
    自我线程诊断快照_v1 out;
    if (!实现_) { out.生命周期=自我线程生命周期状态::内部错误;
                  out.内部错误锁存=true; return out; }
    try {
        std::lock_guard lock{实现_->状态互斥};
        out.生命周期=实现_->生命周期;
        out.治理运行门开启=实现_->治理运行门开启;
        out.线程已进入=实现_->线程已进入;
        out.线程已完成=实现_->线程已完成;
        out.邮箱数量=实现_->邮箱.size()+实现_->冻结批.size();
        out.邮箱容量=实现_->锁定请求 ? 实现_->锁定请求->邮箱容量 : 0;
        if (实现_->当前处理消息) out.当前消息=实现_->当前处理消息->消息;
        out.已冻结批次数量=实现_->已冻结批次数量;
        out.成功治理批次数量=实现_->成功治理批次数量;
        out.最近外部调用=实现_->最近外部调用;
        out.内部错误锁存=实现_->内部错误锁存;
        return out;
    } catch (...) { out.生命周期=自我线程生命周期状态::内部错误;
                    out.内部错误锁存=true; return out; }
}

} // namespace 海中鱼巣
