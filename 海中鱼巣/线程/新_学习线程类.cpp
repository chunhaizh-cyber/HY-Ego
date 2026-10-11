#include "新_学习线程类.h"
#include "../核心/日志系统.h"
#include <algorithm>
#include <chrono>
#include <condition_variable>
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
void 记录学习观察(稳定编码 方法, const wchar_t* 阶段,
    const 新模拟学习结果* 结果=nullptr) noexcept {
    try {
        std::wostringstream 文本;
        文本 << L"方法=" << 方法.值;
        if (结果) {
            文本 << L"；状态=" << static_cast<int>(结果->状态);
            if (结果->配对节点) 文本 << L"；配对=" << 结果->配对节点->值;
            if (结果->模拟场景节点) 文本 << L"；模拟场景=" << 结果->模拟场景节点->值;
        }
        (void)记录运行日志(L"模拟学习",阶段,文本.str());
    } catch (...) {}
}
}
struct 新_学习线程类::实现 final {
    实现(模拟学习服务& 值,std::function<void()> 通知) noexcept
        : 服务(值),完成通知(std::move(通知)) {}
    模拟学习服务& 服务;
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
        std::promise<新模拟学习结果> 承诺;
        std::shared_future<新模拟学习结果> 结果=承诺.get_future().share();
        bool 已开始=false,已完成=false,停止已记录=false;
    };
    std::map<稳定编码,std::shared_ptr<工作>> 工作组;
    std::optional<稳定编码> 当前方法;
    void 通知() noexcept { try { if (完成通知) 完成通知(); } catch (...) {} }
    bool 要停止() noexcept { std::lock_guard 锁(互斥); return 停止; }
    void 运行() noexcept {
        try {
            std::unique_lock 锁(互斥);
            工作线程标识=std::this_thread::get_id();
            已进入=true; 状态=新学习线程状态::等待工作; 变化.notify_all();
            while (!停止) {
                const auto 位置=std::find_if(工作组.begin(),工作组.end(),
                    [](const auto& 项) { return !项.second->已开始; });
                if (位置==工作组.end()) {
                    状态=新学习线程状态::等待工作;
                    变化.wait(锁,[this] {
                        return 停止 || std::any_of(工作组.begin(),工作组.end(),
                            [](const auto& 项) { return !项.second->已开始; });
                    });
                    continue;
                }
                auto 项=位置->second;
                项->已开始=true; 当前方法=项->方法; 状态=新学习线程状态::正在学习;
                锁.unlock();
                记录学习观察(项->方法,L"开始");
                auto 结果=服务.学习(项->方法,[this] { return 要停止(); });
                记录学习观察(项->方法,L"完成",&结果);
                锁.lock();
                项->承诺.set_value(std::move(结果)); 项->已完成=true;
                当前方法.reset(); 变化.notify_all();
                锁.unlock(); 通知(); 锁.lock();
            }
            状态=新学习线程状态::停止中;
            for (auto& [方法,项]:工作组) {
                if (项->已完成) continue;
                新模拟学习结果 结果;
                结果.方法节点=方法; 结果.状态=新模拟学习状态::已取消;
                项->承诺.set_value(结果); 项->已完成=true; 项->已开始=true;
            }
            // 不把map迭代器带到锁外：接收者可以随时确认并删除已完成工作。
            for (;;) {
                const auto 位置=std::find_if(工作组.begin(),工作组.end(),
                    [](const auto& 项) { return !项.second->停止已记录; });
                if (位置==工作组.end()) break;
                auto 项=位置->second; 项->停止已记录=true;
                const auto 结果=项->结果.get();
                锁.unlock();
                if (结果.状态==新模拟学习状态::已取消) 记录学习观察(项->方法,L"停止取消",&结果);
                锁.lock();
            }
            当前方法.reset(); 已结束=true; 状态=新学习线程状态::已停止;
            变化.notify_all(); 锁.unlock(); 通知();
        } catch (...) {
            try {
                std::lock_guard 锁(互斥);
                停止=true; 已结束=true; 状态=新学习线程状态::内部错误;
                for (auto& [方法,项]:工作组) if (!项->已完成) {
                    新模拟学习结果 结果;
                    结果.方法节点=方法; 结果.状态=新模拟学习状态::资源失败;
                    try { 项->承诺.set_value(结果); } catch (...) {}
                    项->已完成=true; 项->已开始=true;
                }
                当前方法.reset(); 变化.notify_all();
            } catch (...) { std::terminate(); }
            通知();
        }
    }
};
新_学习线程类::新_学习线程类(模拟学习服务& 服务,std::function<void()> 通知) noexcept
    : 实现_(new(std::nothrow) 实现(服务,std::move(通知))) {}
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
            结果.状态=新学习操作状态::已完成; 结果.处理结果=项->结果;
            实现_->变化.notify_all();
        }
        锁.unlock();
        记录学习观察(方法,结果.状态==新学习操作状态::精确重复 ? L"重复接收" : L"接收");
    } catch (...) { 结果.状态=新学习操作状态::资源失败; }
    return 结果;
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
        结果.停止已请求=实现_->停止; 结果.待处理数量=实现_->工作组.size();
    } catch (...) { 结果.状态=新学习线程状态::内部错误; }
    return 结果;
}
}
