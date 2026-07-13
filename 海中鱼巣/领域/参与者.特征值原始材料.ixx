// 文件规则：特征值原始材料参与者只协调新建候选的初始侧表材料，不承载特征业务裁决。
module;

#include "特征值服务.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <utility>
#include <vector>

export module 海中鱼巣.领域.参与者.特征值原始材料;

import 海中鱼巣.核心.执行器.结构写入;

export namespace 海中鱼巣 {

struct 特征值初始原始材料 {
    节点句柄 特征值节点;
    主信息句柄 主信息;
    特征值原始类型 原始类型 = 特征值原始类型::未建立;
    std::uint64_t 原始值版本 = 0;
    std::optional<std::int64_t> I64值;
    std::vector<std::int64_t> VecI64值;
    std::vector<std::uint64_t> VecU64值;

    bool 字段自洽() const {
        if (!句柄有效(特征值节点) || !句柄有效(主信息) || 原始值版本 != 1) return false;
        switch (原始类型) {
        case 特征值原始类型::I64:
            return I64值.has_value() && VecI64值.empty() && VecU64值.empty();
        case 特征值原始类型::VecI64:
            return !I64值.has_value() && !VecI64值.empty() && VecI64值.size() <= 特征值服务::最大序列元素数量
                && VecU64值.empty();
        case 特征值原始类型::VecU64:
            return !I64值.has_value() && VecI64值.empty() && !VecU64值.empty()
                && VecU64值.size() <= 特征值服务::最大序列元素数量;
        default:
            return false;
        }
    }
};

enum class 特征值侧表读取状态 : std::uint32_t {
    未建立 = 0,
    可读 = 1,
    许可竞争 = 2,
    内部不一致 = 3
};

struct 特征值侧表原始材料快照 {
    特征值侧表读取状态 状态 = 特征值侧表读取状态::未建立;
    节点句柄 特征值节点;
    特征值原始类型 原始类型 = 特征值原始类型::未建立;
    std::uint64_t 原始值版本 = 0;
    std::vector<std::int64_t> VecI64值;
    std::vector<std::uint64_t> VecU64值;

    bool 可读() const {
        if (状态 != 特征值侧表读取状态::可读 || !句柄有效(特征值节点)
            || 原始值版本 == 0) return false;
        if (原始类型 == 特征值原始类型::I64) return VecI64值.empty() && VecU64值.empty();
        if (原始类型 == 特征值原始类型::VecI64) return !VecI64值.empty() && VecU64值.empty();
        if (原始类型 == 特征值原始类型::VecU64) return VecI64值.empty() && !VecU64值.empty();
        return false;
    }
};

class 特征值原始材料事务参与者 final : public 结构写入事务参与者 {
public:
    特征值原始材料事务参与者() = delete;
    explicit 特征值原始材料事务参与者(特征值服务& 服务) : 服务_(服务) {}
    特征值原始材料事务参与者(const 特征值原始材料事务参与者&) = delete;
    特征值原始材料事务参与者& operator=(const 特征值原始材料事务参与者&) = delete;
    特征值原始材料事务参与者(特征值原始材料事务参与者&&) = delete;
    特征值原始材料事务参与者& operator=(特征值原始材料事务参与者&&) = delete;
    ~特征值原始材料事务参与者() override = default;

    bool 登记初始材料(特征值初始原始材料 材料) {
        if (阶段_ != 阶段::未准备 || !材料.字段自洽()) return false;
        const auto 重复 = std::find_if(待发布材料组_.cbegin(), 待发布材料组_.cend(), [&](const auto& 已有) {
            return 同一节点身份(已有.特征值节点, 材料.特征值节点);
        });
        if (重复 != 待发布材料组_.cend()) return false;
        try {
            待发布材料组_.push_back(std::move(材料));
        } catch (...) {
            return false;
        }
        return true;
    }

    std::size_t 待读取材料数量() const { return 待发布材料组_.size(); }

private:
    enum class 阶段 : std::uint8_t { 未准备, 已准备, 已提交, 已撤销 };

    static bool 同一节点身份(节点句柄 左, 节点句柄 右) {
        return 左.仓库编号 == 右.仓库编号 && 左.节点编号 == 右.节点编号;
    }

    结构写入结果 准备提交(const 结构提交准备只读视图& 视图) override {
        if (阶段_ != 阶段::未准备 || 待发布材料组_.empty()) {
            return {结构写入状态::入口拒绝, 0, 0, 0};
        }

        for (const auto& 材料 : 待发布材料组_) {
            const bool 候选匹配 = 视图.节点是本会话候选(材料.特征值节点)
                && 视图.主信息是本会话候选(材料.主信息)
                && 视图.读取候选节点类型(材料.特征值节点)
                    == std::optional<节点类型>{节点类型::特征值}
                && 视图.读取候选节点主信息(材料.特征值节点)
                    == std::optional<主信息句柄>{材料.主信息};
            if (!候选匹配) return {结构写入状态::内部不一致, 材料.特征值节点.节点编号, 0, 0};
            const auto I64值 = 视图.读取候选I64值(材料.主信息, 0);
            if ((材料.原始类型 == 特征值原始类型::I64 && I64值 != 材料.I64值)
                || (材料.原始类型 != 特征值原始类型::I64 && I64值.has_value())) {
                return {结构写入状态::内部不一致, 材料.特征值节点.节点编号, 0, 0};
            }
        }

        std::unique_lock<std::shared_mutex> 锁(服务_.原始值锁_, std::try_to_lock);
        if (!锁.owns_lock()) return {结构写入状态::许可拒绝, 0, 0, 0};

        for (const auto& 材料 : 待发布材料组_) {
            const auto Vec数量 = 服务_.计算Vec记录数量_已加锁(材料.特征值节点);
            const auto I64数量 = 服务_.计算I64版本记录数量_已加锁(材料.特征值节点);
            if (Vec数量 > 1 || I64数量 > 1 || (Vec数量 != 0 && I64数量 != 0)) {
                return {结构写入状态::内部不一致, 材料.特征值节点.节点编号, 0, 0};
            }
            if (Vec数量 != 0 || I64数量 != 0) {
                return {结构写入状态::版本漂移, 材料.特征值节点.节点编号, 1, 1};
            }
        }

        std::size_t I64数量 = 0;
        std::size_t Vec数量 = 0;
        for (const auto& 材料 : 待发布材料组_) {
            if (材料.原始类型 == 特征值原始类型::I64) ++I64数量;
            else ++Vec数量;
        }
        try {
            服务_.I64版本记录表_.reserve(服务_.I64版本记录表_.size() + I64数量);
            服务_.Vec记录表_.reserve(服务_.Vec记录表_.size() + Vec数量);
            已插入I64节点组_.reserve(I64数量);
            已插入Vec节点组_.reserve(Vec数量);
            for (const auto& 材料 : 待发布材料组_) {
                if (材料.原始类型 == 特征值原始类型::I64) {
                    服务_.I64版本记录表_.push_back({材料.特征值节点, 1});
                    已插入I64节点组_.push_back(材料.特征值节点);
                    continue;
                }
                特征值服务::Vec原始值记录 记录;
                记录.特征值节点 = 材料.特征值节点;
                记录.类型 = 材料.原始类型;
                记录.VecI64值 = 材料.VecI64值;
                记录.VecU64值 = 材料.VecU64值;
                记录.容器版本 = 1;
                服务_.Vec记录表_.push_back(std::move(记录));
                已插入Vec节点组_.push_back(材料.特征值节点);
            }
        } catch (...) {
            const bool 已回前态 = 撤销已插入记录_已加锁();
            return {结构写入状态::内部不一致, 0, 0, 已回前态 ? 0U : 1U};
        }

        原始值锁_ = std::move(锁);
        阶段_ = 阶段::已准备;
        return {结构写入状态::候选已确认, 0, 0, 0};
    }

    结构写入结果 完成提交() override {
        if (阶段_ != 阶段::已准备 || !原始值锁_.owns_lock()) {
            return {结构写入状态::内部不一致, 0, 0, 0};
        }
        阶段_ = 阶段::已提交;
        已插入I64节点组_.clear();
        已插入Vec节点组_.clear();
        原始值锁_.unlock();
        return {结构写入状态::已提交, 0, 0, 0};
    }

    结构写入结果 完成撤销() override {
        if (阶段_ == 阶段::已撤销) return {结构写入状态::候选已撤销, 0, 0, 0};
        if (阶段_ == 阶段::已提交) return {结构写入状态::内部不一致, 0, 0, 0};
        bool 完整 = true;
        if (阶段_ == 阶段::已准备) {
            if (!原始值锁_.owns_lock()) return {结构写入状态::内部不一致, 0, 0, 0};
            完整 = 撤销已插入记录_已加锁();
            原始值锁_.unlock();
        }
        阶段_ = 阶段::已撤销;
        return {完整 ? 结构写入状态::候选已撤销 : 结构写入状态::内部不一致, 0, 0, 0};
    }

    bool 撤销已插入记录_已加锁() {
        bool 完整 = true;
        for (auto 位置 = 已插入Vec节点组_.rbegin(); 位置 != 已插入Vec节点组_.rend(); ++位置) {
            const auto 记录位置 = 服务_.查找Vec记录_已加锁(*位置);
            if (记录位置 == 服务_.Vec记录表_.end() || 记录位置->容器版本 != 1) {
                完整 = false;
                continue;
            }
            服务_.Vec记录表_.erase(记录位置);
        }
        for (auto 位置 = 已插入I64节点组_.rbegin(); 位置 != 已插入I64节点组_.rend(); ++位置) {
            const auto 记录位置 = 服务_.查找I64版本记录_已加锁(*位置);
            if (记录位置 == 服务_.I64版本记录表_.end() || 记录位置->原始值版本 != 1) {
                完整 = false;
                continue;
            }
            服务_.I64版本记录表_.erase(记录位置);
        }
        已插入Vec节点组_.clear();
        已插入I64节点组_.clear();
        return 完整;
    }

    特征值服务& 服务_;
    阶段 阶段_ = 阶段::未准备;
    std::vector<特征值初始原始材料> 待发布材料组_;
    std::vector<节点句柄> 已插入I64节点组_;
    std::vector<节点句柄> 已插入Vec节点组_;
    std::unique_lock<std::shared_mutex> 原始值锁_;
};

class 特征值原始材料侧表访问器 final {
public:
    特征值原始材料侧表访问器() = delete;
    explicit 特征值原始材料侧表访问器(const 特征值服务& 服务) : 服务_(服务) {}
    特征值原始材料侧表访问器(const 特征值原始材料侧表访问器&) = delete;
    特征值原始材料侧表访问器& operator=(const 特征值原始材料侧表访问器&) = delete;
    特征值原始材料侧表访问器(特征值原始材料侧表访问器&&) = delete;
    特征值原始材料侧表访问器& operator=(特征值原始材料侧表访问器&&) = delete;

    特征值侧表原始材料快照 读取(节点句柄 特征值节点) const {
        特征值侧表原始材料快照 输出;
        输出.特征值节点 = 特征值节点;
        if (!句柄有效(特征值节点)) return 输出;

        std::shared_lock<std::shared_mutex> 锁(服务_.原始值锁_, std::try_to_lock);
        if (!锁.owns_lock()) {
            输出.状态 = 特征值侧表读取状态::许可竞争;
            return 输出;
        }
        const auto Vec数量 = 服务_.计算Vec记录数量_已加锁(特征值节点);
        const auto I64数量 = 服务_.计算I64版本记录数量_已加锁(特征值节点);
        if (Vec数量 > 1 || I64数量 > 1 || (Vec数量 != 0 && I64数量 != 0)) {
            输出.状态 = 特征值侧表读取状态::内部不一致;
            return 输出;
        }
        if (I64数量 == 1) {
            const auto 记录 = 服务_.查找I64版本记录_已加锁(特征值节点);
            if (记录 == 服务_.I64版本记录表_.cend() || !特征值服务::I64版本记录内部一致(*记录)) {
                输出.状态 = 特征值侧表读取状态::内部不一致;
                return 输出;
            }
            输出.状态 = 特征值侧表读取状态::可读;
            输出.原始类型 = 特征值原始类型::I64;
            输出.原始值版本 = 记录->原始值版本;
            return 输出;
        }
        if (Vec数量 == 1) {
            const auto 记录 = 服务_.查找Vec记录_已加锁(特征值节点);
            if (记录 == 服务_.Vec记录表_.cend() || !特征值服务::Vec记录内部一致(*记录)) {
                输出.状态 = 特征值侧表读取状态::内部不一致;
                return 输出;
            }
            输出.状态 = 特征值侧表读取状态::可读;
            输出.原始类型 = 记录->类型;
            输出.原始值版本 = 记录->容器版本;
            输出.VecI64值 = 记录->VecI64值;
            输出.VecU64值 = 记录->VecU64值;
        }
        return 输出;
    }

private:
    const 特征值服务& 服务_;
};

}
