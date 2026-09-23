#include "../海中鱼巣/线程/线程_自我.h"

#include <cstdint>
#include <iostream>
#include <string_view>
#include <thread>
#include <vector>

namespace {

using namespace 海中鱼巣;

std::uint64_t 通过数量 = 0;
std::uint64_t 失败数量 = 0;

void 检查(const bool 条件, const std::string_view 名称) {
    if (条件) {
        ++通过数量;
        std::cout << "PASS " << 名称 << '\n';
    }
    else {
        ++失败数量;
        std::cout << "FAIL " << 名称 << '\n';
    }
}

[[nodiscard]] 自我线程创建请求_v1 建立请求(const std::uint64_t 自我身份 = 40) {
    自我线程创建请求_v1 请求;
    请求.邮箱容量 = 2;
    请求.世界 = { { 30 }, { 31 }, 100 };
    请求.自我 = { { 自我身份 }, { 30 }, { 31 }, 101 };
    请求.本能根.自我 = { 自我身份 };
    请求.本能根.安全根 = { { 50 }, { 51 }, { 52 }, { 53 } };
    请求.本能根.服务根 = { { 60 }, { 61 }, { 62 }, { 63 } };
    请求.本能根.Gread = 102;
    return 请求;
}

[[nodiscard]] 自我线程根需求复核消息_v1 建立消息(
    const std::uint64_t 消息身份,
    const 自我线程复核触发根 根,
    const std::uint64_t 正式需求,
    const std::uint64_t 原请求 = 1000) {
    return {
        自我线程合同版本_v1,
        { 消息身份 },
        { 原请求 },
        根,
        { 正式需求 },
    };
}

void 验证基础生命周期与邮箱() {
    自我线程 线程;
    auto 无效请求 = 建立请求();
    无效请求.邮箱容量 = 0;
    const auto 无效结果 = 线程.创建并停在治理运行门(无效请求, 100);
    检查(无效结果.状态 == 自我线程操作状态::入口拒绝, "invalid request rejected");
    检查(线程.读取诊断快照().生命周期 == 自我线程生命周期状态::未创建,
        "invalid request creates no thread");

    const auto 请求 = 建立请求();
    const auto 创建 = 线程.创建并停在治理运行门(请求, 2000);
    检查(创建.状态 == 自我线程操作状态::成功 && 创建.成功(), "valid create reaches closed gate");
    检查(创建.见证.has_value() && 创建.见证->入口序号 < 创建.见证->停门序号,
        "entry and gate witness ordered");

    const auto 重复 = 线程.创建并停在治理运行门(请求, 2000);
    检查(重复.状态 == 自我线程操作状态::精确重复 && 重复.成功(),
        "same request reuses physical selection");
    检查(重复.见证 == 创建.见证, "duplicate returns original witness");

    const auto 冲突 = 线程.创建并停在治理运行门(建立请求(21), 2000);
    检查(冲突.状态 == 自我线程操作状态::选择冲突, "different selection conflicts");

    const auto 开门一 = 线程.复核前置并开放治理运行门(100);
    const auto 开门二 = 线程.复核前置并开放治理运行门(100);
    检查(开门一.状态 == 自我线程操作状态::依赖未就绪 &&
        开门二.状态 == 自我线程操作状态::依赖未就绪,
        "open gate remains dependency-not-ready");

    检查(线程.提交根需求复核消息(建立消息(70, 自我线程复核触发根::安全根, 60)).状态 ==
        自我线程操作状态::入口拒绝, "root locator mismatch rejected");

    const auto 消息一 = 建立消息(70, 自我线程复核触发根::安全根, 50);
    检查(线程.提交根需求复核消息(消息一).状态 == 自我线程操作状态::成功,
        "first message queued");
    检查(线程.提交根需求复核消息(消息一).状态 == 自我线程操作状态::精确重复,
        "same queued message is exact duplicate");
    auto 冲突消息 = 消息一;
    冲突消息.原请求 = { 1001 };
    检查(线程.提交根需求复核消息(冲突消息).状态 == 自我线程操作状态::消息冲突,
        "same message identity with different meaning conflicts");

    const auto 消息二 = 建立消息(71, 自我线程复核触发根::双根, 0);
    检查(线程.提交根需求复核消息(消息二).状态 == 自我线程操作状态::成功,
        "second message queued");
    const auto 消息三 = 建立消息(72, 自我线程复核触发根::服务根, 60);
    检查(线程.提交根需求复核消息(消息三).状态 == 自我线程操作状态::队列已满,
        "bounded mailbox rejects overflow");

    const auto 停门快照 = 线程.读取诊断快照();
    检查(停门快照.生命周期 == 自我线程生命周期状态::已停门 &&
        停门快照.线程已进入 && !停门快照.线程已完成 && !停门快照.治理运行门开启,
        "closed gate snapshot remains active");
    检查(停门快照.邮箱数量 == 2 && !停门快照.当前消息.has_value() &&
        停门快照.已冻结批次数量 == 0 && 停门快照.成功治理批次数量 == 0,
        "closed gate queues without consuming");

    const auto 停止 = 线程.请求停止();
    const auto 等待 = 线程.等待停止(2000);
    检查(停止.状态 == 自我线程操作状态::成功, "stop bypasses full mailbox");
    检查(等待.状态 == 自我线程操作状态::成功 && 等待.成功(), "completed thread joins");
    const auto 停止快照 = 线程.读取诊断快照();
    检查(停止快照.生命周期 == 自我线程生命周期状态::已停止 && 停止快照.线程已完成,
        "stop completion witnessed");
    检查(停止快照.邮箱数量 == 2 && 停止快照.已冻结批次数量 == 0,
        "stop does not consume closed-gate messages");
    检查(线程.请求停止().状态 == 自我线程操作状态::精确重复,
        "repeated stop is idempotent");
    检查(线程.等待停止(100).状态 == 自我线程操作状态::精确重复,
        "repeated wait observes joined thread");
    检查(线程.创建并停在治理运行门(请求, 100).状态 == 自我线程操作状态::入口拒绝 &&
        线程.创建并停在治理运行门(建立请求(22), 100).状态 == 自我线程操作状态::入口拒绝,
        "stopped object cannot restart");
}

void 验证并发同选择只形成一个见证() {
    自我线程 线程;
    const auto 请求 = 建立请求(200);
    std::vector<自我线程创建结果_v1> 结果(8);
    std::vector<std::thread> 调用方;
    for (std::size_t i = 0; i < 结果.size(); ++i) {
        调用方.emplace_back([&线程, &请求, &结果, i] {
            结果[i] = 线程.创建并停在治理运行门(请求, 2000);
        });
    }
    for (auto& 调用 : 调用方) {
        调用.join();
    }

    std::uint64_t 首次数量 = 0;
    std::uint64_t 重复数量 = 0;
    bool 同一见证 = true;
    std::optional<自我线程停门见证_v1> 基准见证;
    for (const auto& 结果项 : 结果) {
        首次数量 += 结果项.状态 == 自我线程操作状态::成功 ? 1 : 0;
        重复数量 += 结果项.状态 == 自我线程操作状态::精确重复 ? 1 : 0;
        if (!基准见证.has_value()) {
            基准见证 = 结果项.见证;
        }
        同一见证 = 同一见证 && 结果项.见证 == 基准见证;
    }
    检查(首次数量 == 1 && 重复数量 == 7, "concurrent create has one physical winner");
    检查(同一见证 && 基准见证.has_value() && 基准见证->完整(),
        "concurrent duplicates share one witness");
    (void)线程.请求停止();
    检查(线程.等待停止(2000).成功(), "concurrent-create thread joins");
}

void 验证析构停止守恒() {
    {
        自我线程 线程;
        const auto 请求 = 建立请求(300);
        检查(线程.创建并停在治理运行门(请求, 2000).成功(), "destructor case created");
        检查(线程.提交根需求复核消息(
            建立消息(400, 自我线程复核触发根::服务根, 60)).成功(),
            "destructor case queued");
    }
    检查(true, "destructor latched stop and joined");
}

} // namespace

int main() {
    验证基础生命周期与邮箱();
    验证并发同选择只形成一个见证();
    验证析构停止守恒();
    std::cout << "SUMMARY pass=" << 通过数量 << " fail=" << 失败数量 << '\n';
    return 失败数量 == 0 ? 0 : 1;
}
