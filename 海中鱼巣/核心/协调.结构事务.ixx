// 文件规则：结构事务协调只提供运行期许可，不承载业务事实、回滚或持久化。
module;

#include "结构事务接线.数据.h"

#include <cstdint>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <unordered_map>

export module 海中鱼巣.核心.协调.结构事务;

export namespace 海中鱼巣 {

namespace 结构事务内部 {

struct 结构事务活动记录 {
    结构许可类型 类型 = 结构许可类型::无效;
    std::thread::id 线程;
    std::unique_ptr<std::shared_lock<std::shared_mutex>> 共享锁;
    std::unique_ptr<std::unique_lock<std::shared_mutex>> 独占锁;
};

struct 结构事务运行期状态 {
    std::uint64_t 域编号 = 0;
    std::uint64_t 纪元 = 0;
    std::uint64_t 下个许可序号 = 1;
    std::shared_mutex 结构锁;
    std::mutex 活动锁;
    std::unordered_map<std::uint64_t, std::unique_ptr<结构事务活动记录>> 活动表;
};

std::uint64_t 下个运行期纪元 = 1;
std::mutex 纪元锁;

std::shared_ptr<结构事务运行期状态> 转换状态(const std::shared_ptr<void>& 状态) {
    return std::static_pointer_cast<结构事务运行期状态>(状态);
}

bool 验证令牌(const std::shared_ptr<void>& 状态, const 结构事务令牌& 令牌, bool 需要独占) {
    const auto 运行期状态 = 转换状态(状态);
    if (!运行期状态 || 令牌.域编号 != 运行期状态->域编号 || 令牌.运行期纪元 != 运行期状态->纪元
        || 令牌.许可序号 == 0 || (需要独占 && 令牌.类型 != 结构许可类型::独占)
        || (!需要独占 && 令牌.类型 == 结构许可类型::无效)) {
        return false;
    }
    std::lock_guard<std::mutex> 锁(运行期状态->活动锁);
    const auto 位置 = 运行期状态->活动表.find(令牌.许可序号);
    return 位置 != 运行期状态->活动表.end() && 位置->second != nullptr
        && 位置->second->类型 == 令牌.类型
        && 位置->second->线程 == std::this_thread::get_id();
}

void 释放许可(const std::shared_ptr<void>& 状态, const 结构事务令牌& 令牌) noexcept {
    const auto 运行期状态 = 转换状态(状态);
    if (!运行期状态 || 令牌.域编号 != 运行期状态->域编号 || 令牌.运行期纪元 != 运行期状态->纪元) {
        return;
    }
    std::unique_ptr<结构事务活动记录> 待释放记录;
    {
        std::lock_guard<std::mutex> 锁(运行期状态->活动锁);
        const auto 位置 = 运行期状态->活动表.find(令牌.许可序号);
        if (位置 == 运行期状态->活动表.end()) {
            return;
        }
        待释放记录 = std::move(位置->second);
        运行期状态->活动表.erase(位置);
    }
    待释放记录.reset();
}

结构事务许可 取得许可(const std::shared_ptr<void>& 状态, 结构许可类型 类型) {
    const auto 运行期状态 = 转换状态(状态);
    if (!运行期状态 || 类型 == 结构许可类型::无效) {
        return {};
    }
    结构事务令牌 令牌;
    auto 待登记记录 = std::make_unique<结构事务活动记录>();
    待登记记录->类型 = 类型;
    待登记记录->线程 = std::this_thread::get_id();
    结构事务活动记录* 记录 = nullptr;
    {
        std::lock_guard<std::mutex> 活动锁(运行期状态->活动锁);
        for (const auto& [_, 记录] : 运行期状态->活动表) {
            if (记录 != nullptr && 记录->线程 == std::this_thread::get_id()) {
                return {};
            }
        }
        令牌 = {运行期状态->域编号, 运行期状态->纪元, 运行期状态->下个许可序号++, 类型};
        const auto [位置, 已登记] = 运行期状态->活动表.emplace(令牌.许可序号, std::move(待登记记录));
        if (!已登记) return {};
        记录 = 位置->second.get();
    }
    try {
        if (类型 == 结构许可类型::独占) {
            记录->独占锁 = std::make_unique<std::unique_lock<std::shared_mutex>>(运行期状态->结构锁);
        } else {
            记录->共享锁 = std::make_unique<std::shared_lock<std::shared_mutex>>(运行期状态->结构锁);
        }
    } catch (...) {
        std::lock_guard<std::mutex> 活动锁(运行期状态->活动锁);
        运行期状态->活动表.erase(令牌.许可序号);
        return {};
    }
    return 结构事务许可::创建(状态, 令牌, 释放许可);
}

}

class 结构事务协调器 {
public:
    explicit 结构事务协调器(std::uint64_t 域编号) {
        if (域编号 == 0) {
            return;
        }
        std::lock_guard<std::mutex> 锁(结构事务内部::纪元锁);
        状态_ = std::make_shared<结构事务内部::结构事务运行期状态>();
        状态_->域编号 = 域编号;
        状态_->纪元 = 结构事务内部::下个运行期纪元++;
    }

    结构事务接线 生成接线() {
        if (!状态_) {
            return {};
        }
        return {状态_->域编号, 状态_->纪元, 状态_,
            [](const std::shared_ptr<void>& 状态) { return 结构事务内部::取得许可(状态, 结构许可类型::共享); },
            [](const std::shared_ptr<void>& 状态) { return 结构事务内部::取得许可(状态, 结构许可类型::独占); },
            [](const std::shared_ptr<void>& 状态, const 结构事务令牌& 令牌) { return 结构事务内部::验证令牌(状态, 令牌, false); },
            [](const std::shared_ptr<void>& 状态, const 结构事务令牌& 令牌) { return 结构事务内部::验证令牌(状态, 令牌, true); }};
    }

    std::uint64_t 读取域编号() const {
        return 状态_ ? 状态_->域编号 : 0;
    }

private:
    std::shared_ptr<结构事务内部::结构事务运行期状态> 状态_;
};

}
