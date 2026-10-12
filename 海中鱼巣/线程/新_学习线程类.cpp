#include "新_学习线程类.h"
#include "../核心/日志系统.h"
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <map>
#include <mutex>
#include <new>
#include <sstream>
#include <thread>

namespace 海中鱼巣 {
namespace {
std::chrono::milliseconds 学习等待时长(std::uint64_t 值) noexcept {
    const auto 最大=std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::duration::max()/4).count();
    return std::chrono::milliseconds((std::min)(值,static_cast<std::uint64_t>(最大)));
}
void 记录学习观察(稳定编码 方法, const wchar_t* 阶段) noexcept {
    try {
        std::wostringstream 文本;
        文本 << L"方法=" << 方法.值;
        (void)记录运行日志(L"模拟学习",阶段,文本.str());
    } catch (...) {}
}
}
struct 新_学习线程类::实现 final {
    实现(模拟学习服务& 值,std::function<void()> 通知) noexcept
        : 服务(值),完成通知(std::move(通知)) {}
    模拟学习服务& 服务;
    方法问题学习服务* 问题服务=nullptr;
    std::function<void()> 完成通知;
    mutable std::mutex 互斥;
    std::mutex 加入互斥;
    std::condition_variable 变化;
    std::thread 线程;
    std::thread::id 工作线程标识;
    新学习线程状态 状态=新学习线程状态::未启动;
    bool 创建已尝试=false,已进入=false,停止=false,已结束=false,已加入=false;
    struct 工作 final {
        稳定编码 方法;
        std::promise<新学习结束通知> 承诺;
        std::shared_future<新学习结束通知> 结果=承诺.get_future().share();
        bool 已开始=false,已完成=false,已确认=false;
        std::optional<新方法问题学习消息> 问题消息;
    };
    std::map<稳定编码,std::shared_ptr<工作>> 工作组;
    // 两种请求共用FIFO；两个索引只负责各自身份去重，不决定调度次序。
    std::map<稳定编码,std::shared_ptr<工作>> 问题工作组;
    std::deque<std::shared_ptr<工作>> 待处理;
    std::optional<稳定编码> 当前方法;
    void 通知() noexcept { try { if (完成通知) 完成通知(); } catch (...) {} }
    bool 要停止() noexcept { std::lock_guard 锁(互斥); return 停止; }
    void 运行() noexcept {
        try {
            std::unique_lock 锁(互斥);
            工作线程标识=std::this_thread::get_id();
            已进入=true; 状态=新学习线程状态::等待工作; 变化.notify_all();
            while (!停止) {
                if (待处理.empty()) {
                    状态=新学习线程状态::等待工作;
                    变化.wait(锁,[this] {
                        return 停止 || !待处理.empty();
                    });
                    continue;
                }
                auto 项=待处理.front(); 待处理.pop_front();
                项->已开始=true; 当前方法=项->方法; 状态=新学习线程状态::正在学习;
                锁.unlock();
                记录学习观察(项->方法,项->问题消息 ? L"问题材料接收开始" : L"开始");
                std::optional<新学习结束通知> 结束;
                std::exception_ptr 异常;
                try {
                    if (项->问题消息 && 问题服务)
                        结束=问题服务->学习(*项->问题消息,[this] { return 要停止(); });
                    else if (!项->问题消息) 结束=服务.学习(项->方法,[this] { return 要停止(); });
                    else throw 新学习运行异常(项->方法,新学习运行异常原因::结构不一致);
                }
                catch (...) { 异常=std::current_exception(); }
                if (结束) 记录学习观察(项->方法,项->问题消息 ? L"问题材料接收结束" : L"结束");
                锁.lock();
                if (异常) 项->承诺.set_exception(异常);
                else 项->承诺.set_value(*结束);
                项->已完成=true;
                当前方法.reset(); 变化.notify_all();
                锁.unlock(); 通知(); 锁.lock();
            }
            状态=新学习线程状态::停止中;
            for (auto& [方法,项]:工作组) {
                if (项->已完成) continue;
                项->承诺.set_exception(std::make_exception_ptr(
                    新学习运行异常(方法,新学习运行异常原因::停止取消)));
                项->已完成=true; 项->已开始=true;
            }
            for (auto& [归档,项]:问题工作组) {
                (void)归档;
                if (项->已完成) continue;
                项->承诺.set_exception(std::make_exception_ptr(
                    新学习运行异常(项->方法,新学习运行异常原因::停止取消)));
                项->已完成=true; 项->已开始=true;
            }
            待处理.clear();
            当前方法.reset(); 已结束=true; 状态=新学习线程状态::已停止;
            变化.notify_all(); 锁.unlock(); 通知();
        } catch (...) {
            const auto 异常=std::current_exception();
            try {
                std::lock_guard 锁(互斥);
                停止=true; 已结束=true; 状态=新学习线程状态::内部错误;
                for (auto& [方法,项]:工作组) if (!项->已完成) {
                    // promise不变量破损不能伪装为已经交付的正常工作。
                    项->承诺.set_exception(异常);
                    项->已完成=true; 项->已开始=true;
                }
                for (auto& [归档,项]:问题工作组) if (!项->已完成) {
                    (void)归档;
                    项->承诺.set_exception(异常);
                    项->已完成=true; 项->已开始=true;
                }
                待处理.clear();
                当前方法.reset(); 变化.notify_all();
            } catch (...) { std::terminate(); }
            通知();
        }
    }
};
新_学习线程类::新_学习线程类(模拟学习服务& 服务,std::function<void()> 通知) noexcept
    : 实现_(new(std::nothrow) 实现(服务,std::move(通知))) {}
新_学习线程类::新_学习线程类(模拟学习服务& 服务,方法问题学习服务& 问题服务,
    std::function<void()> 通知) noexcept
    : 新_学习线程类(服务,std::move(通知)) {
    if (实现_) 实现_->问题服务=&问题服务;
}
新_学习线程类::~新_学习线程类() noexcept {
    if (!实现_) return;
    (void)请求停止();
    try {
        std::lock_guard 锁(实现_->加入互斥);
        if (实现_->线程.joinable()) {
            if (实现_->线程.get_id()==std::this_thread::get_id()) std::terminate();
            实现_->线程.join();
        }
    } catch (...) { std::terminate(); }
}
新学习操作状态 新_学习线程类::启动(std::uint64_t 毫秒) noexcept {
    if (!实现_) return 新学习操作状态::资源失败;
    if (!毫秒) return 新学习操作状态::入口拒绝;
    try {
        std::lock_guard 加入锁(实现_->加入互斥);
        std::unique_lock 锁(实现_->互斥);
        if (实现_->创建已尝试) return 实现_->已进入 && !实现_->停止 && !实现_->已结束
            ? 新学习操作状态::无变化 : 新学习操作状态::已停止接收;
        实现_->创建已尝试=true;
        try { 实现_->线程=std::thread([p=实现_.get()] { p->运行(); }); }
        catch (...) { 实现_->已结束=true; 实现_->状态=新学习线程状态::内部错误;
            return 新学习操作状态::资源失败; }
        if (!实现_->变化.wait_for(锁,学习等待时长(毫秒),[this] {
            return 实现_->已进入 || 实现_->已结束;
        })) return 新学习操作状态::等待超时;
        return 实现_->已结束 ? 新学习操作状态::内部错误 : 新学习操作状态::已完成;
    } catch (...) { return 新学习操作状态::内部错误; }
}
新学习接收结果 新_学习线程类::提交学习(稳定编码 方法) noexcept {
    新学习接收结果 结果;
    if (!实现_ || !有效(方法)) return 结果;
    try {
        std::unique_lock 锁(实现_->互斥);
        if (实现_->停止 || 实现_->已结束) { 结果.状态=新学习操作状态::已停止接收; return 结果; }
        if (!实现_->已进入) { 结果.状态=新学习操作状态::尚未启动; return 结果; }
        auto 位置=实现_->工作组.find(方法);
        if (位置!=实现_->工作组.end()) {
            结果.状态=新学习操作状态::精确重复; 结果.处理结果=位置->second->结果;
        } else {
            auto 项=std::make_shared<实现::工作>(); 项->方法=方法;
            实现_->工作组.emplace(方法,项);
            try { 实现_->待处理.push_back(项); }
            catch (...) { 实现_->工作组.erase(方法); throw; }
            结果.状态=新学习操作状态::已完成; 结果.处理结果=项->结果;
            实现_->变化.notify_all();
        }
        锁.unlock();
        记录学习观察(方法,结果.状态==新学习操作状态::精确重复 ? L"重复接收" : L"接收");
    } catch (...) { 结果.状态=新学习操作状态::资源失败; }
    return 结果;
}
新学习接收结果 新_学习线程类::提交方法问题学习(const 新方法问题学习消息& 消息) noexcept {
    新学习接收结果 结果;
    if (!实现_ || !实现_->问题服务 || !有效(消息.方法节点) || !有效(消息.任务节点)
        || !有效(消息.执行实例节点) || !有效(消息.归档节点)) return 结果;
    try {
        std::lock_guard 锁(实现_->互斥);
        if (实现_->停止 || 实现_->已结束) { 结果.状态=新学习操作状态::已停止接收; return 结果; }
        if (!实现_->已进入) { 结果.状态=新学习操作状态::尚未启动; return 结果; }
        const auto 位置=实现_->问题工作组.find(消息.归档节点);
        if (位置!=实现_->问题工作组.end()) {
            if (位置->second->问题消息!=消息) return 结果;
            结果.状态=新学习操作状态::精确重复;
            结果.处理结果=位置->second->结果;
        } else {
            auto 项=std::make_shared<实现::工作>();
            项->方法=消息.方法节点; 项->问题消息=消息;
            实现_->问题工作组.emplace(消息.归档节点,项);
            try { 实现_->待处理.push_back(项); }
            catch (...) { 实现_->问题工作组.erase(消息.归档节点); throw; }
            结果.状态=新学习操作状态::已完成; 结果.处理结果=项->结果;
            实现_->变化.notify_all();
        }
    } catch (...) { 结果.状态=新学习操作状态::资源失败; }
    return 结果;
}
新学习操作状态 新_学习线程类::确认方法问题结果已接收(稳定编码 归档) noexcept {
    if (!实现_ || !有效(归档)) return 新学习操作状态::入口拒绝;
    try {
        std::lock_guard 锁(实现_->互斥);
        const auto 位置=实现_->问题工作组.find(归档);
        if (位置==实现_->问题工作组.end()) return 新学习操作状态::无变化;
        if (!位置->second->已完成) return 新学习操作状态::入口拒绝;
        if (位置->second->已确认) return 新学习操作状态::无变化;
        // 保留紧凑消息和已就绪future，确认后重放不能再次处理同一原始样本。
        位置->second->已确认=true;
        return 新学习操作状态::已完成;
    } catch (...) { return 新学习操作状态::内部错误; }
}
std::optional<新方法问题学习收集结果> 新_学习线程类::读取方法问题收集结果(稳定编码 归档) const noexcept {
    if (!实现_ || !实现_->问题服务) return std::nullopt;
    return 实现_->问题服务->读取收集结果(归档);
}
新学习操作状态 新_学习线程类::确认结果已接收(稳定编码 方法) noexcept {
    if (!实现_ || !有效(方法)) return 新学习操作状态::入口拒绝;
    try {
        std::lock_guard 锁(实现_->互斥);
        const auto 位置=实现_->工作组.find(方法);
        if (位置==实现_->工作组.end()) return 新学习操作状态::无变化;
        if (!位置->second->已完成) return 新学习操作状态::入口拒绝;
        实现_->工作组.erase(位置); return 新学习操作状态::已完成;
    } catch (...) { return 新学习操作状态::内部错误; }
}
新学习操作状态 新_学习线程类::请求停止() noexcept {
    if (!实现_) return 新学习操作状态::资源失败;
    try {
        std::lock_guard 锁(实现_->互斥);
        if (!实现_->创建已尝试) return 新学习操作状态::尚未启动;
        if (实现_->停止 || 实现_->已结束) return 新学习操作状态::无变化;
        实现_->停止=true; 实现_->状态=新学习线程状态::停止中;
        实现_->变化.notify_all(); return 新学习操作状态::已完成;
    } catch (...) { return 新学习操作状态::内部错误; }
}
新学习操作状态 新_学习线程类::等待停止(std::uint64_t 毫秒) noexcept {
    if (!实现_) return 新学习操作状态::资源失败;
    try {
        {
            std::lock_guard 锁(实现_->互斥);
            if (实现_->工作线程标识==std::this_thread::get_id())
                return 新学习操作状态::不能等待自身;
        }
        std::lock_guard 加入锁(实现_->加入互斥);
        std::unique_lock 锁(实现_->互斥);
        if (!实现_->线程.joinable()) return 实现_->已加入
            ? 新学习操作状态::无变化 : 新学习操作状态::尚未启动;
        if (实现_->线程.get_id()==std::this_thread::get_id()) return 新学习操作状态::不能等待自身;
        if (!实现_->变化.wait_for(锁,学习等待时长(毫秒),[this] { return 实现_->已结束; }))
            return 新学习操作状态::等待超时;
        锁.unlock(); 实现_->线程.join(); 锁.lock(); 实现_->已加入=true;
        return 实现_->状态==新学习线程状态::内部错误 ? 新学习操作状态::内部错误 : 新学习操作状态::已完成;
    } catch (...) { return 新学习操作状态::内部错误; }
}
新学习线程快照 新_学习线程类::读取快照() const noexcept {
    新学习线程快照 结果;
    if (!实现_) { 结果.状态=新学习线程状态::内部错误; return 结果; }
    try {
        std::lock_guard 锁(实现_->互斥);
        结果.状态=实现_->状态; 结果.当前方法=实现_->当前方法;
        结果.停止已请求=实现_->停止; 结果.待处理数量=实现_->工作组.size()
            + static_cast<std::uint64_t>(std::count_if(实现_->问题工作组.begin(),实现_->问题工作组.end(),
                [](const auto& 项) { return !项.second->已确认; }));
    } catch (...) { 结果.状态=新学习线程状态::内部错误; }
    return 结果;
}
}
