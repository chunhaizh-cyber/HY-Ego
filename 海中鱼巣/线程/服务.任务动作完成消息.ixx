// 文件规则：本服务只维护进程内不可变任务动作完成消息账；不提供更新、替换、删除或崩溃恢复。
module;

#include <cstdint>
#include <map>
#include <mutex>
#include <new>
#include <optional>
#include <tuple>
#include <utility>

export module 海中鱼巣.线程.服务.任务动作完成消息;

export import 海中鱼巣.线程.协议.任务动作完成消息;

export namespace 海中鱼巣 {

class 任务动作完成消息账 final : public 任务动作完成消息端口 {
public:
    任务动作完成消息账() = default;
    任务动作完成消息账(const 任务动作完成消息账&) = delete;
    任务动作完成消息账& operator=(const 任务动作完成消息账&) = delete;

    任务动作完成消息结果 发布或读取动作完成消息(
        const 任务动作完成消息& 消息) noexcept override {
        if (!任务动作完成消息有效(消息)) {
            return {任务动作完成消息状态::入口拒绝, std::nullopt};
        }
        try {
            std::lock_guard<std::mutex> 锁(互斥体_);
            const 消息键 键{
                消息.运行代次, 消息.执行幂等身份.值, 消息.动作顺序};
            const auto 位置 = 消息账_.find(键);
            if (位置 != 消息账_.end()) {
                if (位置->second == 消息) {
                    return {任务动作完成消息状态::精确重复, 位置->second};
                }
                return {任务动作完成消息状态::幂等冲突, std::nullopt};
            }
            const auto [新位置, 已插入] = 消息账_.emplace(键, 消息);
            if (!已插入) {
                return {任务动作完成消息状态::内部错误, std::nullopt};
            }
            return {任务动作完成消息状态::已发布, 新位置->second};
        } catch (const std::bad_alloc&) {
            return {任务动作完成消息状态::资源失败, std::nullopt};
        } catch (...) {
            return {任务动作完成消息状态::内部错误, std::nullopt};
        }
    }

    任务动作完成消息结果 读取动作完成消息(
        稳定编码 执行幂等身份,
        std::uint64_t 运行代次,
        std::uint64_t 动作顺序) const noexcept override {
        if (!有效(执行幂等身份) || 运行代次 == 0 || 动作顺序 == 0) {
            return {任务动作完成消息状态::入口拒绝, std::nullopt};
        }
        try {
            std::lock_guard<std::mutex> 锁(互斥体_);
            const auto 位置 = 消息账_.find(
                消息键{运行代次, 执行幂等身份.值, 动作顺序});
            if (位置 == 消息账_.end()) {
                return {任务动作完成消息状态::未找到, std::nullopt};
            }
            return {任务动作完成消息状态::已独立读取, 位置->second};
        } catch (const std::bad_alloc&) {
            return {任务动作完成消息状态::资源失败, std::nullopt};
        } catch (...) {
            return {任务动作完成消息状态::内部错误, std::nullopt};
        }
    }

private:
    using 消息键 = std::tuple<std::uint64_t, std::uint64_t, std::uint64_t>;

    mutable std::mutex 互斥体_;
    std::map<消息键, 任务动作完成消息> 消息账_;
};

} // namespace 海中鱼巣
