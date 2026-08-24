module;

#include <cstdint>
#include <mutex>
#include <optional>
#include <unordered_map>

#define L2_VIRTUAL_EXISTENCE_NO_INCLUDES

export module 海中鱼巣.领域.服务.L2通用虚拟存在结构;
export import 海中鱼巣.领域.合同.L2结构公共;

export {
#include "L2通用虚拟存在结构.数据.h"
}

export namespace 海中鱼巣 {

class L2通用虚拟存在结构服务 final {
public:
    L2通用虚拟存在结构服务() = default;
    L2通用虚拟存在结构服务(const L2通用虚拟存在结构服务&) = delete;
    L2通用虚拟存在结构服务& operator=(
        const L2通用虚拟存在结构服务&) = delete;

    L2通用虚拟存在写入结果 新增(
        const L2通用虚拟存在新增请求& 请求) noexcept {
        if (!L2通用虚拟存在新增请求有效(请求))
            return {{L2结构合同版本, L2结构状态::入口拒绝, 0, std::nullopt},
                std::nullopt};
        try {
            std::lock_guard<std::mutex> 锁(锁_);
            const auto 既有 = 幂等账_.find(请求.幂等身份.值);
            if (既有 != 幂等账_.end()) {
                if (!(既有->second.请求 == 请求))
                    return {{L2结构合同版本, L2结构状态::幂等冲突,
                        事实代次_, std::nullopt}, std::nullopt};
                auto 结果 = 既有->second.结果;
                结果.结果头.状态 = L2结构状态::精确重复;
                结果.结果头.变更事实代次 = std::nullopt;
                return 结果;
            }
            const auto 事实代次 = 事实代次_ + 1;
            const auto 身份 = L2存在身份{稳定编码{下个身份_++}};
            L2通用虚拟存在事实 事实{身份, 请求.ARCHL4宿主引用,
                {事实代次, std::nullopt}};
            L2通用虚拟存在写入结果 结果{
                {L2结构合同版本, L2结构状态::已提交, 事实代次,
                    事实代次}, 事实};
            事实代次_ = 事实代次;
            当前_.emplace(身份.值.值, 事实);
            历史_.emplace(身份.值.值, 事实);
            幂等账_.emplace(请求.幂等身份.值, 记录{请求, 结果});
            return 结果;
        } catch (...) {
            return {{L2结构合同版本, L2结构状态::资源失败, 事实代次_,
                std::nullopt}, std::nullopt};
        }
    }

    L2通用虚拟存在完整读取结果 读取完整(
        const L2通用虚拟存在完整读取请求& 请求) const noexcept {
        if (!L2结构请求头合同有效(请求.请求头)
            || !有效(请求.存在.值))
            return {{L2结构合同版本, L2结构状态::入口拒绝, 0, std::nullopt},
                请求.读取类别, 请求.历史截止事实代次, std::nullopt};
        try {
            std::lock_guard<std::mutex> 锁(锁_);
            const auto& 源 = 请求.读取类别 == L2读取类别::当前 ? 当前_ : 历史_;
            const auto 项 = 源.find(请求.存在.值.值);
            if (项 == 源.end())
                return {{L2结构合同版本,
                    历史_.contains(请求.存在.值.值)
                        ? L2结构状态::已退出 : L2结构状态::未找到,
                    事实代次_, std::nullopt}, 请求.读取类别,
                    请求.历史截止事实代次, std::nullopt};
            if (请求.读取类别 == L2读取类别::当前
                && 请求.请求头.期望事实代次 != 事实代次_)
                return {{L2结构合同版本, L2结构状态::事实代次漂移,
                    事实代次_, std::nullopt}, 请求.读取类别,
                    请求.历史截止事实代次, std::nullopt};
            return {{L2结构合同版本, L2结构状态::已读取,
                请求.读取类别 == L2读取类别::当前
                    ? 请求.请求头.期望事实代次
                    : 请求.历史截止事实代次, std::nullopt},
                请求.读取类别, 请求.历史截止事实代次, 项->second};
        } catch (...) {
            return {{L2结构合同版本, L2结构状态::内部不一致, 事实代次_,
                std::nullopt}, 请求.读取类别, 请求.历史截止事实代次,
                std::nullopt};
        }
    }

    L2通用虚拟存在退出结果 退出(
        const L2通用虚拟存在退出请求& 请求) noexcept {
        if (!L2结构请求头合同有效(请求.请求头)
            || !L2结构幂等身份有效(请求.幂等身份)
            || !有效(请求.存在.值))
            return {{L2结构合同版本, L2结构状态::入口拒绝, 0, std::nullopt},
                std::nullopt};
        try {
            std::lock_guard<std::mutex> 锁(锁_);
            const auto 项 = 当前_.find(请求.存在.值.值);
            if (项 == 当前_.end())
                return {{L2结构合同版本,
                    历史_.contains(请求.存在.值.值)
                        ? L2结构状态::已退出 : L2结构状态::未找到,
                    事实代次_, std::nullopt}, std::nullopt};
            const auto 事实代次 = 事实代次_ + 1;
            auto 事实 = 项->second;
            事实.生命周期.退出事实代次 = 事实代次;
            事实代次_ = 事实代次;
            当前_.erase(项);
            历史_[请求.存在.值.值] = 事实;
            return {{L2结构合同版本, L2结构状态::已提交, 事实代次,
                事实代次}, 事实};
        } catch (...) {
            return {{L2结构合同版本, L2结构状态::内部不一致, 事实代次_,
                std::nullopt}, std::nullopt};
        }
    }

private:
    struct 记录 final {
        L2通用虚拟存在新增请求 请求;
        L2通用虚拟存在写入结果 结果;
    };
    mutable std::mutex 锁_;
    std::uint64_t 事实代次_ = 0;
    std::uint64_t 下个身份_ = 0xA200'0000'0000'0001ULL;
    std::unordered_map<std::uint64_t, L2通用虚拟存在事实> 当前_;
    std::unordered_map<std::uint64_t, L2通用虚拟存在事实> 历史_;
    std::unordered_map<std::uint64_t, 记录> 幂等账_;
};

} // namespace 海中鱼巣
