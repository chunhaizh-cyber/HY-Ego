module;

#include "../领域/方法服务.h"
#include "../核心/句柄.h"

#include <chrono>
#include <cstdint>
#include <functional>
#include <optional>

export module 海中鱼巣.领域.初始化.系统;

import 海中鱼巣.领域.初始化.需求;
import 海中鱼巣.领域.初始化.概念图;
import 海中鱼巣.线程.自我线程;

export namespace 海中鱼巣 {

struct 系统初始化请求 {
    std::uint64_t 方法登记根稳定键 = 0;
    自我根需求初始化参数 根需求参数{};

    bool 有效() const noexcept {
        return 方法登记根稳定键 != 0 && 根需求参数.有效();
    }
};

struct 系统初始化端口 {
    std::function<std::optional<方法登记根材料>(std::uint64_t)> 初始化方法登记根;
    std::function<自我线程操作结果(const 自我根需求初始化参数&)> 启动自我线程;
    std::function<bool(std::chrono::milliseconds)> 等待自我线程初始化;
    std::function<std::optional<自我线程初始化快照>()> 读取自我线程初始化快照;
    std::function<概念图初始化结果()> 初始化概念图四根;
    std::function<std::optional<节点句柄>(节点句柄)> 确保存在实例根支持;

    bool 完整() const noexcept {
        return 初始化方法登记根
            && 启动自我线程
            && 等待自我线程初始化
            && 读取自我线程初始化快照
            && 初始化概念图四根
            && 确保存在实例根支持;
    }
};

enum class 系统初始化状态 : std::uint8_t {
    已初始化 = 0,
    请求无效 = 1,
    端口无效 = 2,
    方法登记根失败 = 3,
    自我线程启动失败 = 4,
    自我线程初始化失败 = 5,
    概念图初始化失败 = 6,
    自我存在根支持失败 = 7,
    内部不一致 = 8
};

struct 系统初始化结果 {
    系统初始化状态 状态 = 系统初始化状态::内部不一致;
    std::optional<方法登记根材料> 方法登记根;
    std::optional<自我线程初始化快照> 自我初始化;
    概念图初始化结果 概念图;
    std::optional<节点句柄> 自我存在根支持;

    bool 成功() const noexcept {
        return 状态 == 系统初始化状态::已初始化
            && 方法登记根.has_value()
            && 自我初始化.has_value()
            && 自我初始化->成功()
            && 概念图.成功()
            && 自我存在根支持.has_value();
    }
};

系统初始化结果 初始化普通系统(
    const 系统初始化端口& 端口,
    const 系统初始化请求& 请求) {
    系统初始化结果 结果;
    if (!请求.有效()) {
        结果.状态 = 系统初始化状态::请求无效;
        return 结果;
    }
    if (!端口.完整()) {
        结果.状态 = 系统初始化状态::端口无效;
        return 结果;
    }

    结果.方法登记根 = 端口.初始化方法登记根(请求.方法登记根稳定键);
    if (!结果.方法登记根.has_value()) {
        结果.状态 = 系统初始化状态::方法登记根失败;
        return 结果;
    }
    const auto 启动 = 端口.启动自我线程(请求.根需求参数);
    if (!启动.成功 || 启动.复用初始化结果) {
        结果.状态 = 系统初始化状态::自我线程启动失败;
        return 结果;
    }
    const bool 已等待完成 =
        端口.等待自我线程初始化(std::chrono::seconds(10));
    结果.自我初始化 = 端口.读取自我线程初始化快照();
    if (!已等待完成
        || !结果.自我初始化.has_value()
        || !结果.自我初始化->成功()) {
        结果.状态 = 系统初始化状态::自我线程初始化失败;
        return 结果;
    }
    结果.概念图 = 端口.初始化概念图四根();
    if (!结果.概念图.成功()) {
        结果.状态 = 系统初始化状态::概念图初始化失败;
        return 结果;
    }
    结果.自我存在根支持 =
        端口.确保存在实例根支持(结果.自我初始化->世界树.自我存在节点);
    if (!结果.自我存在根支持.has_value()
        || 结果.自我存在根支持.value() != 结果.概念图.存在根.根节点) {
        结果.状态 = 系统初始化状态::自我存在根支持失败;
        return 结果;
    }
    结果.状态 = 系统初始化状态::已初始化;
    return 结果;
}

}
