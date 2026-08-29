module;

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <cstdint>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <system_error>
#include <utility>

export module 海中鱼巣.端到端测试.安全根定义与当前值;

import 海中鱼巣.装配.普通应用;
import 海中鱼巣.业务.提供者.真实自我形成;
import 海中鱼巣.业务.提供者.四本体根生产初始化;
import 海中鱼巣.业务.提供者.方法登记根生产初始化;
import 海中鱼巣.业务.提供者.本能根运行初始化;
import 海中鱼巣.领域.服务.安全根定义与当前值;

export namespace 海中鱼巣 {
int 运行安全根定义与当前值端到端测试() noexcept;
}

module :private;

namespace 海中鱼巣::安全根定义测试内部 {

struct 隔离根清理 final {
    std::filesystem::path 根;
    ~隔离根清理() {
        std::error_code 错误;
        std::filesystem::remove_all(根, 错误);
    }
};

int 失败(const char* 编号, const char* 步骤) noexcept {
    std::cout << "[INSTINCT-SAFETY-DEFINITION][" << 编号 << "] FAIL: "
              << 步骤 << '\n';
    return 1;
}

void 通过(const char* 编号, const char* 说明) {
    std::cout << "[INSTINCT-SAFETY-DEFINITION][" << 编号 << "] PASS: "
              << 说明 << '\n';
}

struct 会话_v1 final {
    std::unique_ptr<普通应用上下文> 上下文;
    本能根运行锚点_v1 本能根;
};

std::optional<会话_v1> 建立会话(
    const 普通应用配置& 配置, std::uint64_t 根启动身份) {
    const auto 失败 = [](const char* 步骤) -> std::optional<会话_v1> {
        std::cout << "[INSTINCT-SAFETY-DEFINITION][SESSION] FAIL: "
                  << 步骤 << '\n';
        return std::nullopt;
    };
    auto 装配 = 构造普通应用上下文(配置);
    if (!装配.成功()) return 失败("assemble ordinary application");
    auto 上下文 = std::move(装配.上下文);
    const auto 世界根 = 初始化并发布系统世界树根(
        *上下文, 系统世界树根初始化请求{});
    if (!世界根.成功() || !世界根.已发布根)
        return 失败("initialize system world root");
    const auto& 根 = *世界根.已发布根;
    const 自我世界树根消费请求 消费请求{
        自我世界树根消费合同版本, 根.选择, 根.首次建立请求,
        根.树, 根.根场景, 根.正式树读回.根标记};
    const auto 自我世界根 = 消费并发布自我世界树根(*上下文, 消费请求);
    if (!自我世界根.成功() || !自我世界根.已发布材料)
        return 失败("consume self world root");
    真实自我形成提供者 自我形成者(上下文->取得L2结构聚合服务());
    真实自我形成请求 自我请求;
    自我请求.根消费材料 = *自我世界根.已发布材料;
    const auto 自我 = 自我形成者.形成(自我请求);
    if (!自我.成功() || !自我.已形成自我)
        return 失败("form real self");
    四本体根生产初始化提供者 四本体根者(
        上下文->取得L2概念结构聚合服务());
    const auto 四本体根 = 四本体根者.初始化(四本体根生产初始化请求{});
    if (!四本体根.成功()) return 失败("initialize ontology roots");
    方法登记根生产初始化提供者 方法根者(
        上下文->取得L2方法结构聚合服务());
    const auto 方法根 = 方法根者.初始化(方法登记根生产初始化请求{});
    if (!方法根.成功()) return 失败("initialize method root");
    本能根运行初始化提供者 本能根者(
        上下文->取得L2存在结构服务(), 上下文->取得L2特征结构服务(),
        上下文->取得L2状态结构服务(), 上下文->取得L2需求结构服务());
    const auto 本能根 = 本能根者.初始化并读取本能根运行锚点_v1({
        本能根运行初始化合同版本_v1,
        自我.已形成自我->自我存在.身份, 根启动身份});
    if (!本能根.成功() || !本能根.锚点)
        return 失败("initialize instinct root anchor");
    return 会话_v1{std::move(上下文), *本能根.锚点};
}

std::optional<std::uint64_t> 当前代次(const 会话_v1& 会话) {
    return 会话.上下文->取得L2状态结构服务().读取中性当前事实代次_v1();
}

std::optional<发布安全根定义请求_v1> 形成发布请求(
    const 会话_v1& 会话, std::uint64_t 幂等, std::uint32_t 版本,
    std::int64_t L = 100, std::int64_t H = 1000) {
    const auto G = 当前代次(会话);
    if (!G) return std::nullopt;
    return 发布安全根定义请求_v1{
        安全根定义合同版本_v1, {L2结构合同版本, *G},
        L2结构幂等身份{幂等}, 会话.本能根, L, H, 版本,
        1, 1, 稳定编码{0x5341'4645'4445'4601ULL}};
}

安全根定义读取结果_v1 读取定义(const 会话_v1& 会话) {
    const auto G = 当前代次(会话);
    if (!G) return {};
    return 会话.上下文->取得安全根定义与当前值服务()
        .读取当前安全根定义_v1({安全根定义合同版本_v1,
            {L2结构合同版本, *G}, 会话.本能根});
}

} // namespace 海中鱼巣::安全根定义测试内部

namespace 海中鱼巣 {

int 运行安全根定义与当前值端到端测试() noexcept {
    using namespace 安全根定义测试内部;
    try {
        const auto 隔离根 = std::filesystem::path{L"D:\\TEMP"}
            / L"海中鱼巣" / L"INSTINCT-STAGE3-SAFETY-ROOT-DEFINITION"
            / (std::to_wstring(GetCurrentProcessId()) + L"-"
                + std::to_wstring(GetTickCount64()));
        隔离根清理 清理{隔离根};
        std::error_code 错误;
        std::filesystem::create_directories(隔离根 / L"l1", 错误);
        if (错误) return 失败("S00", "create L1 root");
        std::filesystem::create_directories(隔离根 / L"material", 错误);
        if (错误) return 失败("S00", "create material root");
        普通应用配置 配置;
        配置.L1事实基座持久存储 = {
            L1事实基座持久恢复合同版本_v1, 隔离根 / L"l1"};
        配置.不可变材料存储 = {
            L2结构合同版本, {隔离根 / L"material"}};
        配置.等待合同登记 = 形成普通应用任务筹办等待合同登记();
        auto 会话 = 建立会话(配置, 0x5341'4645'524F'4F54ULL);
        if (!会话) return 失败("S00", "assemble session");
        auto& 服务 = 会话->上下文->取得安全根定义与当前值服务();

        const auto 未发布 = 读取定义(*会话);
        if (未发布.状态 != 安全根定义读取状态_v1::未发布
            || 未发布.成功() || 未发布.定义
            || 未发布.本次正式读回截止 != 0)
            return 失败("S00", "must not auto publish defaults");
        通过("S00", "ordinary assembly leaves definition unpublished");

        auto 首版跳版请求 = 形成发布请求(*会话,
            0x5341'4645'0000'00F4ULL, 2);
        if (!首版跳版请求) return 失败("S04", "make non-v1 first request");
        const auto 首版跳版 = 服务.发布安全根定义_v1(*首版跳版请求);
        if (首版跳版.状态 != 安全根定义发布状态_v1::版本漂移
            || 首版跳版.成功() || 首版跳版.定义
            || 首版跳版.首次提交事实代次 != 0
            || 首版跳版.本次正式读回截止 != 0)
            return 失败("S04", "reject first version other than 1");

        auto 首次请求 = 形成发布请求(*会话, 0x5341'4645'0000'0001ULL, 1);
        if (!首次请求) return 失败("S01", "make v1 request");
        const auto 首次 = 服务.发布安全根定义_v1(*首次请求);
        const auto 首次读回 = 读取定义(*会话);
        if (!首次.成功() || 首次.状态 != 安全根定义发布状态_v1::已发布
            || !首次.定义 || !首次读回.成功()
            || 首次读回.定义 != 首次.定义)
            return 失败("S01", "publish and independent readback");
        通过("S01", "version 1 published and independently read");

        const auto 重放 = 服务.发布安全根定义_v1(*首次请求);
        if (!重放.成功() || 重放.状态 != 安全根定义发布状态_v1::精确重复
            || 重放.首次提交事实代次 != 首次.首次提交事实代次
            || 重放.定义 != 首次.定义)
            return 失败("S02", "exact replay");
        通过("S02", "exact replay has no second authority effect");

        auto 冲突请求 = *首次请求;
        冲突请求.高位阈值 = 1001;
        const auto 冲突 = 服务.发布安全根定义_v1(冲突请求);
        if (冲突.状态 != 安全根定义发布状态_v1::幂等冲突
            || 冲突.成功() || 冲突.定义
            || 冲突.首次提交事实代次 != 0
            || 冲突.本次正式读回截止 != 0)
            return 失败("S03", "same key conflict");
        通过("S03", "same idempotency key with different meaning rejected");

        auto 跳版请求 = 形成发布请求(*会话, 0x5341'4645'0000'0002ULL, 3);
        if (!跳版请求) return 失败("S04", "make skipped version");
        const auto 跳版 = 服务.发布安全根定义_v1(*跳版请求);
        if (跳版.状态 != 安全根定义发布状态_v1::版本漂移
            || 跳版.成功() || 跳版.定义
            || 跳版.首次提交事实代次 != 0
            || 跳版.本次正式读回截止 != 0)
            return 失败("S04", "reject skipped version");
        通过("S04", "first and successor versions must be contiguous");

        auto 非法请求 = 形成发布请求(*会话, 0x5341'4645'0000'0003ULL,
            2, 1, 1000);
        if (!非法请求) return 失败("S05", "make invalid thresholds");
        const auto 非法 = 服务.发布安全根定义_v1(*非法请求);
        if (非法.状态 != 安全根定义发布状态_v1::入口拒绝
            || 非法.成功() || 非法.定义
            || 非法.首次提交事实代次 != 0
            || 非法.本次正式读回截止 != 0)
            return 失败("S05", "reject invalid thresholds");
        auto 零规则请求 = 形成发布请求(*会话,
            0x5341'4645'0000'00F5ULL, 2);
        if (!零规则请求) return 失败("S05", "make zero-rule request");
        零规则请求->主动安全结算规则版本 = 0;
        const auto 零规则 = 服务.发布安全根定义_v1(*零规则请求);
        if (零规则.状态 != 安全根定义发布状态_v1::入口拒绝
            || 零规则.成功() || 零规则.定义
            || 零规则.首次提交事实代次 != 0
            || 零规则.本次正式读回截止 != 0)
            return 失败("S05", "reject zero rule version");
        auto 坏角色请求 = 形成发布请求(*会话,
            0x5341'4645'0000'00F6ULL, 2);
        if (!坏角色请求) return 失败("S05", "make bad-anchor request");
        坏角色请求->本能根锚点.唯一自我 =
            L2存在身份{稳定编码{0x5341'4645'4241'4453ULL}};
        const auto 坏角色 = 服务.发布安全根定义_v1(*坏角色请求);
        if (坏角色.状态 != 安全根定义发布状态_v1::引用冲突
            || 坏角色.成功() || 坏角色.定义
            || 坏角色.首次提交事实代次 != 0
            || 坏角色.本次正式读回截止 != 0)
            return 失败("S05", "reject bad role anchor");
        通过("S05", "invalid thresholds, rule versions and anchors rejected");

        auto 二版请求 = 形成发布请求(*会话, 0x5341'4645'0000'0004ULL,
            2, 200, 2000);
        if (!二版请求) return 失败("S06", "make v2 request");
        const auto 二版 = 服务.发布安全根定义_v1(*二版请求);
        const auto 二版读回 = 读取定义(*会话);
        if (!二版.成功() || !二版.定义 || 二版.定义->定义版本 != 2
            || !二版读回.成功() || 二版读回.定义 != 二版.定义)
            return 失败("S06", "publish v2 as unique current");
        通过("S06", "version 2 becomes unique current definition");

        const auto 历史重放 = 服务.发布安全根定义_v1(*首次请求);
        if (!历史重放.成功()
            || 历史重放.状态 != 安全根定义发布状态_v1::精确重复
            || !历史重放.定义 || 历史重放.定义->定义版本 != 1
            || 历史重放.首次提交事实代次 != 首次.首次提交事实代次)
            return 失败("S06A", "historical v1 replay after v2");
        通过("S06A", "historical version remains independently replayable");

        const auto G = 当前代次(*会话);
        if (!G) return 失败("S07", "read G0");
        const auto 快照 = 服务.读取安全根定义与当前值_v1({
            安全根定义合同版本_v1, {L2结构合同版本, *G}, 会话->本能根});
        if (!快照.成功() || !快照.快照
            || 快照.快照->定义.定义版本 != 2
            || 快照.快照->当前值事实.特征实例
                != 会话->本能根.安全根特征实例
            || 快照.快照->当前安全根值 != 本能根初始值_v1
            || 快照.本次正式读回截止 != *G)
            return 失败("S07", "same-G definition and current A snapshot");
        通过("S07", "definition and authoritative current A share one G0");

#if defined(ARCH_INSTINCT_SAFETY_DEFINITION_VALIDATION)
        服务.ARCH_注入当前值坏材料一次();
        const auto 坏材料 = 服务.读取安全根定义与当前值_v1({
            安全根定义合同版本_v1, {L2结构合同版本, *G}, 会话->本能根});
        if (坏材料.状态 != 安全根定义与当前值读取状态_v1::引用冲突
            || 坏材料.成功() || 坏材料.快照
            || 坏材料.本次正式读回截止 != 0)
            return 失败("S08", "bad current-value material must fail closed");
        通过("S08", "bad current-value material returns no partial payload");
#else
        std::cout << "[INSTINCT-SAFETY-DEFINITION][S08] NOT_RUN: "
                     "validation macro disabled\n";
#endif

        std::uint32_t 期望恢复版本 = 2;
#if defined(ARCH_INSTINCT_SAFETY_DEFINITION_VALIDATION)
        auto 读后漂移发布请求 = 形成发布请求(*会话,
            0x5341'4645'0000'00F9ULL, 3, 300, 3000);
        if (!读后漂移发布请求)
            return 失败("S09", "make post-read drift publish request");
        服务.ARCH_注入组合读取后守卫发布一次(*读后漂移发布请求);
        const auto 读后漂移 = 服务.读取安全根定义与当前值_v1({
            安全根定义合同版本_v1, {L2结构合同版本, *G}, 会话->本能根});
        if (读后漂移.状态 != 安全根定义与当前值读取状态_v1::当前性漂移
            || 读后漂移.成功() || 读后漂移.快照
            || 读后漂移.本次正式读回截止 != 0)
            return 失败("S09", "post-read G0 drift must have empty payload");
        期望恢复版本 = 3;
#endif
        const auto 漂移G = 当前代次(*会话);
        if (!漂移G) return 失败("S09", "read G0 for pre-read drift");
        auto 漂移请求 = 安全根定义与当前值读取请求_v1{
            安全根定义合同版本_v1, {L2结构合同版本, *漂移G + 1},
            会话->本能根};
        const auto 漂移 = 服务.读取安全根定义与当前值_v1(漂移请求);
        if (漂移.状态 != 安全根定义与当前值读取状态_v1::当前性漂移
            || 漂移.成功() || 漂移.快照
            || 漂移.本次正式读回截止 != 0)
            return 失败("S09", "G0 drift must have empty payload");
        通过("S09", "pre-read and post-read G0 drift fail closed");

#if defined(ARCH_INSTINCT_SAFETY_DEFINITION_VALIDATION)
        auto 三版请求 = 形成发布请求(*会话,
            0x5341'4645'0000'0005ULL, 期望恢复版本 + 1,
            400, 4000);
        if (!三版请求) return 失败("S10", "make next-version request");
        服务.ARCH_注入发布提交后首次读回失败一次();
        const auto 已可能 = 服务.发布安全根定义_v1(*三版请求);
        if (已可能.状态 != 安全根定义发布状态_v1::已可能发布
            || 已可能.成功() || 已可能.定义
            || 已可能.首次提交事实代次 == 0
            || 已可能.本次正式读回截止 != 0)
            return 失败("S10", "post-commit readback failure shape");
        const auto 已可能重放 = 服务.发布安全根定义_v1(*三版请求);
        const auto 三版读回 = 读取定义(*会话);
        if (!已可能重放.成功()
            || 已可能重放.状态 != 安全根定义发布状态_v1::精确重复
            || 已可能重放.首次提交事实代次 != 已可能.首次提交事实代次
            || !三版读回.成功() || !三版读回.定义
            || 三版读回.定义->定义版本 != 期望恢复版本 + 1)
            return 失败("S10", "original replay must settle committed version");
        ++期望恢复版本;
        通过("S10", "post-commit readback failure settles only by replay");
#else
        std::cout << "[INSTINCT-SAFETY-DEFINITION][S10] NOT_RUN: "
                     "validation macro disabled\n";
#endif

        const auto 恢复锚点 = 会话->本能根;
        会话.reset();
        auto 恢复装配 = 构造普通应用上下文(配置);
        if (!恢复装配.成功() || !恢复装配.上下文)
            return 失败("S11", "reassemble persistent ordinary application");
        auto 恢复上下文 = std::move(恢复装配.上下文);
        本能根运行初始化提供者 恢复本能根者(
            恢复上下文->取得L2存在结构服务(),
            恢复上下文->取得L2特征结构服务(),
            恢复上下文->取得L2状态结构服务(),
            恢复上下文->取得L2需求结构服务());
        const auto 恢复本能根 =
            恢复本能根者.初始化并读取本能根运行锚点_v1({
                本能根运行初始化合同版本_v1,
                恢复锚点.唯一自我, 0x5341'4645'524F'4F55ULL});
        if (!恢复本能根.成功() || !恢复本能根.锚点
            || 恢复本能根.状态 != 本能根运行初始化状态_v1::已恢复)
            return 失败("S11", "recover persistent instinct root anchor");
        会话_v1 恢复会话{std::move(恢复上下文), *恢复本能根.锚点};
        const auto 恢复定义 = 读取定义(恢复会话);
        const auto 恢复G = 当前代次(恢复会话);
        if (!恢复定义.成功() || !恢复定义.定义
            || 恢复定义.定义->定义版本 != 期望恢复版本 || !恢复G)
            return 失败("S11", "read current definition after reassembly");
        const auto 恢复快照 = 恢复会话.上下文
            ->取得安全根定义与当前值服务().读取安全根定义与当前值_v1({
                安全根定义合同版本_v1, {L2结构合同版本, *恢复G},
                恢复会话.本能根});
        if (!恢复快照.成功() || !恢复快照.快照
            || 恢复快照.快照->定义.定义版本 != 期望恢复版本)
            return 失败("S11", "read same-G snapshot after reassembly");
        通过("S11", "owner and current definition recover across reassembly");
        return 0;
    } catch (...) {
        return 失败("EX", "unhandled exception");
    }
}

} // namespace 海中鱼巣
