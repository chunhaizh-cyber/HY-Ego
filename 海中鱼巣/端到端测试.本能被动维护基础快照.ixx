module;

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <system_error>
#include <thread>
#include <utility>

export module 海中鱼巣.端到端测试.本能被动维护基础快照;

import 海中鱼巣.装配.普通应用;
import 海中鱼巣.业务.提供者.真实自我形成;
import 海中鱼巣.业务.提供者.四本体根生产初始化;
import 海中鱼巣.业务.提供者.方法登记根生产初始化;
import 海中鱼巣.业务.提供者.本能根运行初始化;
import 海中鱼巣.适配.适配器.单调时钟;
import 海中鱼巣.领域.服务.完整秒时钟;
import 海中鱼巣.领域.组合.本能被动维护基础快照;
import 海中鱼巣.领域.算法.本能单完整秒被动维护;

export namespace 海中鱼巣 {
int 运行本能被动维护基础快照端到端测试() noexcept;
}

module :private;

namespace 海中鱼巣::本能被动维护基础快照测试内部 {

inline constexpr std::int64_t L1 = 2'767'011'611'056'432'742;
inline constexpr std::int64_t H1 = 7'378'697'629'483'820'645;

struct 隔离根清理 final {
    std::filesystem::path 根;
    ~隔离根清理() {
        std::error_code 错误;
        std::filesystem::remove_all(根, 错误);
    }
};

int 失败(const char* 编号, const char* 步骤) noexcept {
    std::cout << "[INSTINCT-BASE-SNAPSHOT][" << 编号 << "] FAIL: "
              << 步骤 << '\n';
    return 1;
}

void 通过(const char* 编号, const char* 说明) {
    std::cout << "[INSTINCT-BASE-SNAPSHOT][" << 编号 << "] PASS: "
              << 说明 << '\n';
}

struct 会话_v1 final {
    std::unique_ptr<普通应用上下文> 上下文;
    本能根运行锚点_v1 本能根;
};

std::optional<会话_v1> 建立会话(
    const 普通应用配置& 配置, std::uint64_t 根启动身份) {
    auto 装配 = 构造普通应用上下文(配置);
    if (!装配.成功() || !装配.上下文) return std::nullopt;
    auto 上下文 = std::move(装配.上下文);
    const auto 世界根 = 初始化并发布系统世界树根(
        *上下文, 系统世界树根初始化请求{});
    if (!世界根.成功() || !世界根.已发布根) return std::nullopt;
    const auto& 根 = *世界根.已发布根;
    const 自我世界树根消费请求 消费请求{
        自我世界树根消费合同版本, 根.选择, 根.首次建立请求,
        根.树, 根.根场景, 根.正式树读回.根标记};
    const auto 自我世界根 = 消费并发布自我世界树根(*上下文, 消费请求);
    if (!自我世界根.成功() || !自我世界根.已发布材料)
        return std::nullopt;
    真实自我形成提供者 自我形成者(上下文->取得L2结构聚合服务());
    真实自我形成请求 自我请求;
    自我请求.根消费材料 = *自我世界根.已发布材料;
    const auto 自我 = 自我形成者.形成(自我请求);
    if (!自我.成功() || !自我.已形成自我) return std::nullopt;
    四本体根生产初始化提供者 四本体根者(
        上下文->取得L2概念结构聚合服务());
    if (!四本体根者.初始化(四本体根生产初始化请求{}).成功())
        return std::nullopt;
    方法登记根生产初始化提供者 方法根者(
        上下文->取得L2方法结构聚合服务());
    if (!方法根者.初始化(方法登记根生产初始化请求{}).成功())
        return std::nullopt;
    本能根运行初始化提供者 本能根者(
        上下文->取得L2存在结构服务(), 上下文->取得L2特征结构服务(),
        上下文->取得L2状态结构服务(), 上下文->取得L2需求结构服务());
    const auto 本能根 = 本能根者.初始化并读取本能根运行锚点_v1({
        本能根运行初始化合同版本_v1,
        自我.已形成自我->自我存在.身份, 根启动身份});
    if (!本能根.成功() || !本能根.锚点) return std::nullopt;
    return 会话_v1{std::move(上下文), *本能根.锚点};
}

std::optional<std::uint64_t> 当前代次(const 会话_v1& 会话) {
    return 会话.上下文->取得L2状态结构服务().读取中性当前事实代次_v1();
}

bool 发布定义(会话_v1& 会话, std::uint32_t 版本,
    std::int64_t L, std::int64_t H, std::uint64_t 幂等) {
    const auto G0 = 当前代次(会话);
    if (!G0) return false;
    const auto 结果 = 会话.上下文->取得安全根定义与当前值服务()
        .发布安全根定义_v1({安全根定义合同版本_v1,
            {L2结构合同版本, *G0}, L2结构幂等身份{幂等}, 会话.本能根,
            L, H, 版本, 1, 1, 稳定编码{0x4241'5345'534E'4150ULL}});
    return 结果.成功();
}

bool 建立游标(会话_v1& 会话, std::uint64_t 运行代次,
    std::uint64_t 纪元, std::uint64_t 幂等) {
    const auto G0 = 当前代次(会话);
    if (!G0) return false;
    const auto 结果 = 会话.上下文->取得本能被动维护游标服务()
        .建立或恢复本能被动维护游标_v1({
            本能被动维护游标合同版本_v1,
            {L2结构合同版本, *G0}, L2结构幂等身份{幂等}, 会话.本能根,
            运行代次, 纪元, 0, 稳态时钟时间源版本,
            服务时间维护规则版本_v1});
    return 结果.成功();
}

本能被动维护基础快照读取请求_v1 形成请求(
    const 会话_v1& 会话, std::uint64_t 运行代次, std::uint64_t G0) {
    return {本能被动维护基础快照合同版本_v1,
        {L2结构合同版本, G0}, 会话.本能根, 运行代次};
}

bool 空失败(const 本能被动维护基础快照读取结果_v1& 结果,
    本能被动维护基础快照读取状态_v1 状态) {
    return 结果.状态 == 状态 && !结果.成功() && !结果.可维护()
        && !结果.快照 && 结果.本次正式读回截止 == 0;
}

} // namespace 海中鱼巣::本能被动维护基础快照测试内部

namespace 海中鱼巣 {

int 运行本能被动维护基础快照端到端测试() noexcept {
    using namespace 本能被动维护基础快照测试内部;
    try {
        const auto 隔离根 = std::filesystem::path{L"D:\\TEMP"}
            / L"海中鱼巣" / L"INSTINCT-STAGE3-MAINTENANCE-BASE-SNAPSHOT-COMPOSER"
            / (std::to_wstring(GetCurrentProcessId()) + L"-"
                + std::to_wstring(GetTickCount64()));
        隔离根清理 清理{隔离根};
        std::error_code 错误;
        std::filesystem::create_directories(隔离根 / L"l1", 错误);
        if (错误) return 失败("P00", "create L1 root");
        std::filesystem::create_directories(隔离根 / L"material", 错误);
        if (错误) return 失败("P00", "create material root");
        普通应用配置 配置;
        配置.L1事实基座持久存储 = {
            L1事实基座持久恢复合同版本_v1, 隔离根 / L"l1"};
        配置.不可变材料存储 = {
            L2结构合同版本, {隔离根 / L"material"}};
        配置.等待合同登记 = 形成普通应用任务筹办等待合同登记();
        auto 会话 = 建立会话(配置, 0x4241'5345'524F'4F54ULL);
        if (!会话) return 失败("P00", "assemble session");

        constexpr std::uint64_t 运行代次 = 701;
        constexpr std::uint64_t 纪元 = 702;
        单调时钟适配器 时钟适配器({运行代次, 纪元});
        完整秒时钟服务 时钟(时钟适配器,
            {运行代次, 纪元, 稳态时钟时间源版本});
        本能被动维护基础快照组合器 组合器(
            会话->上下文->取得L2特征结构服务(),
            会话->上下文->取得安全根定义与当前值服务(),
            会话->上下文->取得本能被动维护游标服务(), 时钟);

        const auto G初 = 当前代次(*会话);
        if (!G初) return 失败("P00", "read initial G");
        auto 坏请求 = 形成请求(*会话, 运行代次, *G初);
        坏请求.合同版本 = 0;
        if (!空失败(组合器.读取本能被动维护基础快照_v1(坏请求),
                本能被动维护基础快照读取状态_v1::入口拒绝))
            return 失败("P00", "invalid request shape");
        通过("P00", "invalid request returns exact empty failure");

        const auto 未建立 = 组合器.读取本能被动维护基础快照_v1(
            形成请求(*会话, 运行代次, *G初));
        if (!空失败(未建立,
                本能被动维护基础快照读取状态_v1::游标未建立))
            return 失败("P01", "cursor must be explicit");
        if (!建立游标(*会话, 运行代次, 纪元, 0x4241'5345'0000'0001ULL))
            return 失败("P01", "publish cursor");
        const auto G游标 = 当前代次(*会话);
        if (!G游标) return 失败("P01", "read cursor G");
        const auto 未定义 = 组合器.读取本能被动维护基础快照_v1(
            形成请求(*会话, 运行代次, *G游标));
        if (!空失败(未定义,
                本能被动维护基础快照读取状态_v1::定义未发布))
            return 失败("P01", "definition must be explicit");
        通过("P01", "missing cursor and definition never become defaults");

        if (!发布定义(*会话, 1, L1, H1, 0x4241'5345'0000'0002ULL))
            return 失败("P02", "publish definition v1");
        const auto G1 = 当前代次(*会话);
        if (!G1) return 失败("P02", "read v1 G");
        const auto 首版 = 组合器.读取本能被动维护基础快照_v1(
            形成请求(*会话, 运行代次, *G1));
        if (!首版.成功() || !首版.快照
            || 首版.快照->安全根.定义.定义版本 != 1
            || 首版.快照->安全根.定义.低位阈值 != L1
            || 首版.快照->安全根.定义.高位阈值 != H1)
            return 失败("P02", "read exact v1 thresholds");
        if (!发布定义(*会话, 2, L1 + 101, H1 + 101,
                0x4241'5345'0000'0003ULL))
            return 失败("P02", "publish definition v2");
        const auto G2 = 当前代次(*会话);
        if (!G2) return 失败("P02", "read v2 G");
        const auto 二版 = 组合器.读取本能被动维护基础快照_v1(
            形成请求(*会话, 运行代次, *G2));
        if (!二版.成功() || !二版.快照
            || 二版.快照->安全根.定义.定义版本 != 2
            || 二版.快照->安全根.定义.低位阈值 != L1 + 101
            || 二版.快照->安全根.定义.高位阈值 != H1 + 101)
            return 失败("P02", "read changed versioned thresholds");
        通过("P02", "L/H change only through a new formal definition version");

        if (二版.状态 != 本能被动维护基础快照读取状态_v1::无新完整秒
            || 二版.可维护() || !二版.快照
            || 二版.快照->待维护区间)
            return 失败("P03", "no-new-second full success shape");
        通过("P03", "no new second is complete but not maintainable");

        const auto 错代次 = 组合器.读取本能被动维护基础快照_v1(
            形成请求(*会话, 运行代次 + 1, *G2));
        if (!空失败(错代次,
                本能被动维护基础快照读取状态_v1::运行代次漂移))
            return 失败("P05", "runtime generation drift");
        const auto 旧G = 组合器.读取本能被动维护基础快照_v1(
            形成请求(*会话, 运行代次, *G2 + 1));
        if (!空失败(旧G,
                本能被动维护基础快照读取状态_v1::当前性漂移))
            return 失败("P06", "G0 drift");
        通过("P05-P06", "runtime and G0 drift fail closed");

        auto 坏锚点请求 = 形成请求(*会话, 运行代次, *G2);
        坏锚点请求.本能根锚点.服务根特征实例 =
            L2特征实例身份{稳定编码{0x4241'5345'4241'4456ULL}};
        const auto 坏实例 = 组合器.读取本能被动维护基础快照_v1(坏锚点请求);
        if (坏实例.成功() || 坏实例.快照)
            return 失败("P07", "service value instance binding");
        通过("P07", "service V must come from the anchored current instance");

        std::this_thread::sleep_for(std::chrono::milliseconds(2200));
        const auto GN = 当前代次(*会话);
        if (!GN) return 失败("P04", "read interval G");
        const auto 多秒 = 组合器.读取本能被动维护基础快照_v1(
            形成请求(*会话, 运行代次, *GN));
        if (!多秒.成功() || !多秒.可维护() || !多秒.快照
            || !多秒.快照->待维护区间
            || 多秒.快照->待维护区间->完整秒数量 < 2)
            return 失败("P04", "multi-second interval");
        通过("P04", "monotonic clock yields exact positive interval");

        本能单完整秒被动维护请求_v1 算法请求;
        算法请求.服务值前值 = 多秒.快照->当前服务值;
        算法请求.生存安全根前值 = 多秒.快照->安全根.当前安全根值;
        算法请求.低位阈值 = 多秒.快照->安全根.定义.低位阈值;
        算法请求.高位阈值 = 多秒.快照->安全根.定义.高位阈值;
        算法请求.安全根定义版本 = 多秒.快照->安全根.定义.定义版本;
        const auto 算法结果 = 计算本能单完整秒被动维护候选_v1(算法请求);
        if (!算法结果.成功() || !算法结果.载荷
            || 算法结果.载荷->低位阈值 != L1 + 101
            || 算法结果.载荷->高位阈值 != H1 + 101
            || 算法结果.载荷->安全根定义版本 != 2)
            return 失败("P08", "consumer projection into rule kernel");
        通过("P08", "consumer projects A/V/L/H/version without constants");
        return 0;
    } catch (...) {
        return 失败("EX", "unhandled exception");
    }
}

} // namespace 海中鱼巣
