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

export module 海中鱼巣.端到端测试.安全根生产定义初始化;

import 海中鱼巣.装配.普通应用;
import 海中鱼巣.业务.提供者.真实自我形成;
import 海中鱼巣.业务.提供者.四本体根生产初始化;
import 海中鱼巣.业务.提供者.方法登记根生产初始化;
import 海中鱼巣.业务.提供者.本能根运行初始化;
import 海中鱼巣.业务.提供者.安全根生产定义初始化;
import 海中鱼巣.领域.服务.安全根定义与当前值;

export namespace 海中鱼巣 {
int 运行安全根生产定义初始化端到端测试() noexcept;
}

module :private;

namespace 海中鱼巣::安全根生产定义初始化测试内部 {

struct 隔离根清理 final {
    std::filesystem::path 根;
    ~隔离根清理() {
        std::error_code 错误;
        std::filesystem::remove_all(根, 错误);
    }
};

int 失败(const char* 编号, const char* 步骤) noexcept {
    std::cout << "[INSTINCT-PRODUCTION-SAFETY-DEFINITION][" << 编号
              << "] FAIL: " << 步骤 << '\n';
    return 1;
}

void 通过(const char* 编号, const char* 说明) {
    std::cout << "[INSTINCT-PRODUCTION-SAFETY-DEFINITION][" << 编号
              << "] PASS: " << 说明 << '\n';
}

普通应用配置 形成配置(const std::filesystem::path& 根) {
    普通应用配置 配置;
    配置.L1事实基座持久存储 = {
        L1事实基座持久恢复合同版本_v1, 根 / L"l1"};
    配置.不可变材料存储 = {
        L2结构合同版本, {根 / L"material"}};
    配置.等待合同登记 = 形成普通应用任务筹办等待合同登记();
    return 配置;
}

struct 会话_v1 final {
    std::unique_ptr<普通应用上下文> 上下文;
    本能根运行锚点_v1 本能根;
};

std::optional<会话_v1> 建立会话(
    const 普通应用配置& 配置, std::uint64_t 启动身份) {
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
        上下文->取得L2存在结构服务(),
        上下文->取得L2特征结构服务(),
        上下文->取得L2状态结构服务(),
        上下文->取得L2需求结构服务());
    const auto 本能根 = 本能根者.初始化并读取本能根运行锚点_v1({
        本能根运行初始化合同版本_v1,
        自我.已形成自我->自我存在.身份, 启动身份});
    if (!本能根.成功() || !本能根.锚点) return std::nullopt;
    return 会话_v1{std::move(上下文), *本能根.锚点};
}

安全根生产定义初始化结果_v1 初始化(会话_v1& 会话) {
    安全根生产定义初始化提供者 提供者(
        会话.上下文->取得L2状态结构服务(),
        会话.上下文->取得安全根定义与当前值服务());
    return 提供者.初始化并读取安全根生产定义_v1({
        安全根生产定义初始化合同版本_v1, 会话.本能根});
}

std::optional<std::uint64_t> 当前代次(const 会话_v1& 会话) {
    return 会话.上下文->取得L2状态结构服务().读取中性当前事实代次_v1();
}

bool 建立目录(const std::filesystem::path& 根) {
    std::error_code 错误;
    std::filesystem::create_directories(根 / L"l1", 错误);
    if (错误) return false;
    std::filesystem::create_directories(根 / L"material", 错误);
    return !错误;
}

} // namespace 海中鱼巣::安全根生产定义初始化测试内部

namespace 海中鱼巣 {

int 运行安全根生产定义初始化端到端测试() noexcept {
    using namespace 安全根生产定义初始化测试内部;
    try {
        const auto 隔离根 = std::filesystem::path{L"D:\\TEMP"}
            / L"海中鱼巣" / L"INSTINCT-STAGE3-PRODUCTION-SAFETY-DEFINITION"
            / (std::to_wstring(GetCurrentProcessId()) + L"-"
                + std::to_wstring(GetTickCount64()));
        隔离根清理 清理{隔离根};
        const auto 正常根 = 隔离根 / L"normal";
        if (!建立目录(正常根)) return 失败("P00", "create normal roots");
        const auto 配置 = 形成配置(正常根);
        auto 会话 = 建立会话(配置, 0x5052'4F44'524F'4F54ULL);
        if (!会话) return 失败("P00", "assemble normal session");

        安全根生产定义初始化提供者 提供者(
            会话->上下文->取得L2状态结构服务(),
            会话->上下文->取得安全根定义与当前值服务());
        auto 坏请求 = 安全根生产定义初始化请求_v1{
            0, 会话->本能根};
        const auto 坏结果 = 提供者.初始化并读取安全根生产定义_v1(坏请求);
        if (坏结果.状态 != 安全根生产定义初始化状态_v1::入口拒绝
            || 坏结果.成功() || 坏结果.定义 || 坏结果.来源规则
            || 坏结果.本次正式读回截止 != 0)
            return 失败("P00", "reject invalid contract");
        if (安全根生产低位阈值_v1 != 2767011611056432742LL
            || 安全根生产高位阈值_v1 != 7378697629483820645LL)
            return 失败("P00", "exact threshold constants");
        通过("P00", "integer thresholds and entry guard are exact");

        const auto 首次 = 初始化(*会话);
        if (!首次.成功()
            || 首次.状态 != 安全根生产定义初始化状态_v1::已发布
            || !首次.定义 || !首次.来源规则
            || !安全根生产定义匹配_v1(*首次.定义, 会话->本能根)
            || !主动安全结算生产来源规则匹配_v1(
                *首次.来源规则, 会话->本能根)
            || 首次.来源规则->规则版本
                != 首次.定义->主动安全结算规则版本)
            return 失败("P01", "first production publish and readback");
        通过("P01", "production definition is published and independently read");

        const auto 重复前G = 当前代次(*会话);
        const auto 重复 = 初始化(*会话);
        const auto 重复后G = 当前代次(*会话);
        if (!重复前G || !重复后G || *重复前G != *重复后G
            || !重复.成功()
            || 重复.状态 != 安全根生产定义初始化状态_v1::已恢复
            || 重复.定义 != 首次.定义
            || 重复.来源规则 != 首次.来源规则)
            return 失败("P02", "same-process exact recovery");
        通过("P02", "repeat has no second authority publication");

        const auto 恢复锚点 = 会话->本能根;
        会话.reset();
        auto 恢复装配 = 构造普通应用上下文(配置);
        if (!恢复装配.成功() || !恢复装配.上下文)
            return 失败("P03", "reassemble persistent ordinary application");
        auto 恢复上下文 = std::move(恢复装配.上下文);
        本能根运行初始化提供者 恢复本能根者(
            恢复上下文->取得L2存在结构服务(),
            恢复上下文->取得L2特征结构服务(),
            恢复上下文->取得L2状态结构服务(),
            恢复上下文->取得L2需求结构服务());
        const auto 恢复本能根 =
            恢复本能根者.初始化并读取本能根运行锚点_v1({
                本能根运行初始化合同版本_v1,
                恢复锚点.唯一自我, 0x5052'4F44'524F'4F55ULL});
        if (!恢复本能根.成功() || !恢复本能根.锚点
            || 恢复本能根.状态 != 本能根运行初始化状态_v1::已恢复)
            return 失败("P03", "recover persistent instinct anchor");
        会话_v1 恢复会话{std::move(恢复上下文), *恢复本能根.锚点};
        const auto 恢复前G = 当前代次(恢复会话);
        const auto 恢复 = 初始化(恢复会话);
        const auto 恢复后G = 当前代次(恢复会话);
        if (!恢复前G || !恢复后G || *恢复前G != *恢复后G
            || !恢复.成功()
            || 恢复.状态 != 安全根生产定义初始化状态_v1::已恢复
            || 恢复.定义 != 首次.定义
            || 恢复.来源规则 != 首次.来源规则)
            return 失败("P03", "cross-session recovery");
        通过("P03", "persistent definition recovers without rewrite");

        const auto 补规则根 = 隔离根 / L"missing-rule";
        if (!建立目录(补规则根))
            return 失败("P04A", "create missing-rule roots");
        auto 补规则会话 = 建立会话(
            形成配置(补规则根), 0x4D49'5353'5255'4C45ULL);
        if (!补规则会话)
            return 失败("P04A", "assemble missing-rule session");
        const auto 补规则G = 当前代次(*补规则会话);
        if (!补规则G) return 失败("P04A", "read missing-rule G0");
        const auto 预置生产定义 = 补规则会话->上下文
            ->取得安全根定义与当前值服务().发布安全根定义_v1({
                安全根定义合同版本_v1,
                {L2结构合同版本, *补规则G},
                安全根生产定义发布幂等身份_v1,
                补规则会话->本能根,
                安全根生产低位阈值_v1,
                安全根生产高位阈值_v1,
                安全根生产定义版本_v1,
                主动安全结算规则版本_v1,
                安全根生产定义来源_v1});
        if (!预置生产定义.成功())
            return 失败("P04A", "preseed production definition only");
        const auto 补发规则 = 初始化(*补规则会话);
        if (!补发规则.成功()
            || 补发规则.状态 != 安全根生产定义初始化状态_v1::已发布
            || !补发规则.定义 || !补发规则.来源规则
            || !主动安全结算生产来源规则匹配_v1(
                *补发规则.来源规则, 补规则会话->本能根))
            return 失败("P04A", "publish missing source rule after definition");
        通过("P04A", "existing definition does not impersonate a source set");

        const auto 规则冲突根 = 隔离根 / L"rule-conflict";
        if (!建立目录(规则冲突根))
            return 失败("P04B", "create rule-conflict roots");
        auto 规则冲突会话 = 建立会话(
            形成配置(规则冲突根), 0x5255'4C45'434F'4E46ULL);
        if (!规则冲突会话)
            return 失败("P04B", "assemble rule-conflict session");
        const auto 规则冲突G = 当前代次(*规则冲突会话);
        if (!规则冲突G) return 失败("P04B", "read rule-conflict G0");
        auto& 规则冲突服务 =
            规则冲突会话->上下文->取得安全根定义与当前值服务();
        const auto 规则冲突定义 = 规则冲突服务.发布安全根定义_v1({
            安全根定义合同版本_v1,
            {L2结构合同版本, *规则冲突G},
            安全根生产定义发布幂等身份_v1,
            规则冲突会话->本能根,
            安全根生产低位阈值_v1,
            安全根生产高位阈值_v1,
            安全根生产定义版本_v1,
            主动安全结算规则版本_v1,
            安全根生产定义来源_v1});
        const auto 规则冲突写G = 当前代次(*规则冲突会话);
        if (!规则冲突定义.成功() || !规则冲突写G)
            return 失败("P04B", "preseed rule-conflict definition");
        const auto 规则冲突事实 =
            规则冲突服务.发布主动安全结算来源规则_v1({
                安全根定义合同版本_v1,
                {L2结构合同版本, *规则冲突写G},
                L2结构幂等身份{0x5255'4C45'4346'0001ULL},
                规则冲突会话->本能根,
                主动安全结算规则版本_v1,
                {},
                稳定编码{0x5255'4C45'4346'0002ULL}});
        const auto 规则冲突前G = 当前代次(*规则冲突会话);
        const auto 规则冲突结果 = 初始化(*规则冲突会话);
        const auto 规则冲突后G = 当前代次(*规则冲突会话);
        if (!规则冲突事实.成功() || !规则冲突前G || !规则冲突后G
            || *规则冲突前G != *规则冲突后G
            || 规则冲突结果.状态
                != 安全根生产定义初始化状态_v1::定义冲突
            || 规则冲突结果.成功() || 规则冲突结果.定义
            || 规则冲突结果.来源规则
            || 规则冲突结果.本次正式读回截止 != 0)
            return 失败("P04B", "conflicting source rule must fail closed");
        通过("P04B", "definition and source rule are independently verified");

        const auto 冲突根 = 隔离根 / L"conflict";
        if (!建立目录(冲突根)) return 失败("P04", "create conflict roots");
        auto 冲突会话 = 建立会话(
            形成配置(冲突根), 0x434F'4E46'524F'4F54ULL);
        if (!冲突会话) return 失败("P04", "assemble conflict session");
        const auto 冲突G = 当前代次(*冲突会话);
        if (!冲突G) return 失败("P04", "read conflict G0");
        const auto 预置 = 冲突会话->上下文->取得安全根定义与当前值服务()
            .发布安全根定义_v1({
                安全根定义合同版本_v1,
                {L2结构合同版本, *冲突G},
                L2结构幂等身份{0x434F'4E46'4445'4601ULL},
                冲突会话->本能根, 3, 4, 1, 1,
                稳定编码{0x434F'4E46'5352'4301ULL}});
        if (!预置.成功()) return 失败("P04", "preseed conflicting definition");
        const auto 冲突前G = 当前代次(*冲突会话);
        const auto 冲突结果 = 初始化(*冲突会话);
        const auto 冲突后G = 当前代次(*冲突会话);
        if (!冲突前G || !冲突后G || *冲突前G != *冲突后G
            || 冲突结果.状态 != 安全根生产定义初始化状态_v1::定义冲突
            || 冲突结果.成功() || 冲突结果.定义
            || 冲突结果.来源规则
            || 冲突结果.本次正式读回截止 != 0)
            return 失败("P04", "conflicting definition must fail closed");
        通过("P04", "existing conflicting definition is never overwritten");

#if defined(ARCH_INSTINCT_PRODUCTION_SAFETY_DEFINITION_VALIDATION)
        const auto 空失败形状 = [](const auto& 结果,
                                   安全根生产定义初始化状态_v1 状态) {
            return 结果.状态 == 状态 && !结果.成功() && !结果.定义
                && !结果.来源规则
                && 结果.本次正式读回截止 == 0;
        };
        const auto 验证读取映射 = [&](安全根定义读取状态_v1 下层,
                                      std::uint8_t 次数,
                                      安全根生产定义初始化状态_v1 期望) {
            安全根生产定义初始化提供者 映射提供者(
                恢复会话.上下文->取得L2状态结构服务(),
                恢复会话.上下文->取得安全根定义与当前值服务());
            映射提供者.ARCH_注入下层读取状态(下层, 次数);
            return 空失败形状(
                映射提供者.初始化并读取安全根生产定义_v1({
                    安全根生产定义初始化合同版本_v1,
                    恢复会话.本能根}), 期望);
        };
        if (!验证读取映射(安全根定义读取状态_v1::许可拒绝, 1,
                安全根生产定义初始化状态_v1::许可拒绝)
            || !验证读取映射(安全根定义读取状态_v1::资源失败, 1,
                安全根生产定义初始化状态_v1::资源失败)
            || !验证读取映射(安全根定义读取状态_v1::内部错误, 1,
                安全根生产定义初始化状态_v1::内部错误)
            || !验证读取映射(安全根定义读取状态_v1::引用冲突, 1,
                安全根生产定义初始化状态_v1::定义冲突)
            || !验证读取映射(安全根定义读取状态_v1::当前性漂移, 2,
                安全根生产定义初始化状态_v1::当前性漂移))
            return 失败("P05", "lower read status mapping");

        const auto 映射根 = 隔离根 / L"mapping";
        if (!建立目录(映射根)) return 失败("P05", "create mapping roots");
        auto 映射会话 = 建立会话(
            形成配置(映射根), 0x4D41'5050'524F'4F54ULL);
        if (!映射会话) return 失败("P05", "assemble mapping session");
        const auto 验证发布映射 = [&](安全根定义发布状态_v1 下层,
                                      std::uint8_t 次数,
                                      安全根生产定义初始化状态_v1 期望) {
            安全根生产定义初始化提供者 映射提供者(
                映射会话->上下文->取得L2状态结构服务(),
                映射会话->上下文->取得安全根定义与当前值服务());
            映射提供者.ARCH_注入下层发布状态(下层, 次数);
            return 空失败形状(
                映射提供者.初始化并读取安全根生产定义_v1({
                    安全根生产定义初始化合同版本_v1,
                    映射会话->本能根}), 期望);
        };
        if (!验证发布映射(安全根定义发布状态_v1::许可拒绝, 1,
                安全根生产定义初始化状态_v1::许可拒绝)
            || !验证发布映射(安全根定义发布状态_v1::资源失败, 1,
                安全根生产定义初始化状态_v1::资源失败)
            || !验证发布映射(安全根定义发布状态_v1::内部错误, 1,
                安全根生产定义初始化状态_v1::内部错误)
            || !验证发布映射(安全根定义发布状态_v1::版本漂移, 1,
                安全根生产定义初始化状态_v1::定义冲突)
            || !验证发布映射(安全根定义发布状态_v1::幂等冲突, 1,
                安全根生产定义初始化状态_v1::定义冲突)
            || !验证发布映射(安全根定义发布状态_v1::引用冲突, 1,
                安全根生产定义初始化状态_v1::定义冲突)
            || !验证发布映射(安全根定义发布状态_v1::已可能发布, 2,
                安全根生产定义初始化状态_v1::已可能发布))
            return 失败("P05", "lower publish status mapping");

        const auto 验证来源读取映射 = [&](
            主动安全结算来源规则读取状态_v1 下层,
            std::uint8_t 次数,
            安全根生产定义初始化状态_v1 期望) {
            安全根生产定义初始化提供者 映射提供者(
                恢复会话.上下文->取得L2状态结构服务(),
                恢复会话.上下文->取得安全根定义与当前值服务());
            映射提供者.ARCH_注入下层来源规则读取状态(下层, 次数);
            return 空失败形状(
                映射提供者.初始化并读取安全根生产定义_v1({
                    安全根生产定义初始化合同版本_v1,
                    恢复会话.本能根}), 期望);
        };
        if (!验证来源读取映射(
                主动安全结算来源规则读取状态_v1::许可拒绝, 1,
                安全根生产定义初始化状态_v1::许可拒绝)
            || !验证来源读取映射(
                主动安全结算来源规则读取状态_v1::资源失败, 1,
                安全根生产定义初始化状态_v1::资源失败)
            || !验证来源读取映射(
                主动安全结算来源规则读取状态_v1::内部错误, 1,
                安全根生产定义初始化状态_v1::内部错误)
            || !验证来源读取映射(
                主动安全结算来源规则读取状态_v1::引用冲突, 1,
                安全根生产定义初始化状态_v1::定义冲突)
            || !验证来源读取映射(
                主动安全结算来源规则读取状态_v1::当前性漂移, 2,
                安全根生产定义初始化状态_v1::当前性漂移))
            return 失败("P05", "lower source-rule read status mapping");

        const auto 来源映射根 = 隔离根 / L"source-mapping";
        if (!建立目录(来源映射根))
            return 失败("P05", "create source-mapping roots");
        auto 来源映射会话 = 建立会话(
            形成配置(来源映射根), 0x534F'5552'4345'4D50ULL);
        if (!来源映射会话)
            return 失败("P05", "assemble source-mapping session");
        const auto 来源映射G = 当前代次(*来源映射会话);
        if (!来源映射G) return 失败("P05", "read source-mapping G0");
        const auto 来源映射定义 = 来源映射会话->上下文
            ->取得安全根定义与当前值服务().发布安全根定义_v1({
                安全根定义合同版本_v1,
                {L2结构合同版本, *来源映射G},
                安全根生产定义发布幂等身份_v1,
                来源映射会话->本能根,
                安全根生产低位阈值_v1,
                安全根生产高位阈值_v1,
                安全根生产定义版本_v1,
                主动安全结算规则版本_v1,
                安全根生产定义来源_v1});
        if (!来源映射定义.成功())
            return 失败("P05", "preseed source-mapping definition");
        const auto 验证来源发布映射 = [&](
            主动安全结算来源规则发布状态_v1 下层,
            std::uint8_t 次数,
            安全根生产定义初始化状态_v1 期望) {
            auto& 得 = *来源映射会话;
            安全根生产定义初始化提供者 映射提供者(
                得.上下文->取得L2状态结构服务(),
                得.上下文->取得安全根定义与当前值服务());
            映射提供者.ARCH_注入下层来源规则发布状态(下层, 次数);
            安全根生产定义初始化结果_v1 结果 =
                映射提供者.初始化并读取安全根生产定义_v1({
                    安全根生产定义初始化合同版本_v1, 得.本能根});
            return 结果.状态 == 期望 && !结果.成功() && !结果.定义
                && !结果.来源规则 && 结果.本次正式读回截止 == 0;
        };
        if (!验证来源发布映射(
                主动安全结算来源规则发布状态_v1::许可拒绝, 1,
                安全根生产定义初始化状态_v1::许可拒绝)
            || !验证来源发布映射(
                主动安全结算来源规则发布状态_v1::资源失败, 1,
                安全根生产定义初始化状态_v1::资源失败)
            || !验证来源发布映射(
                主动安全结算来源规则发布状态_v1::内部错误, 1,
                安全根生产定义初始化状态_v1::内部错误)
            || !验证来源发布映射(
                主动安全结算来源规则发布状态_v1::版本漂移, 1,
                安全根生产定义初始化状态_v1::定义冲突)
            || !验证来源发布映射(
                主动安全结算来源规则发布状态_v1::幂等冲突, 1,
                安全根生产定义初始化状态_v1::定义冲突)
            || !验证来源发布映射(
                主动安全结算来源规则发布状态_v1::引用冲突, 1,
                安全根生产定义初始化状态_v1::定义冲突)
            || !验证来源发布映射(
                主动安全结算来源规则发布状态_v1::已可能发布, 2,
                安全根生产定义初始化状态_v1::已可能发布))
            return 失败("P05", "lower source-rule publish status mapping");
        通过("P05", "all lower definition and source-rule failures stay empty");
#else
        std::cout << "[INSTINCT-PRODUCTION-SAFETY-DEFINITION][P05] NOT_RUN: "
                     "validation macro disabled\n";
#endif
        return 0;
    } catch (...) {
        return 失败("EX", "unhandled exception");
    }
}

} // namespace 海中鱼巣
