module;

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <algorithm>
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
#include <variant>

export module 海中鱼巣.端到端测试.本能被动维护基础快照;

import 海中鱼巣.装配.普通应用;
import 海中鱼巣.业务.提供者.真实自我形成;
import 海中鱼巣.业务.提供者.四本体根生产初始化;
import 海中鱼巣.业务.提供者.方法登记根生产初始化;
import 海中鱼巣.业务.提供者.本能根运行初始化;
import 海中鱼巣.适配.适配器.单调时钟;
import 海中鱼巣.领域.服务.完整秒时钟;
import 海中鱼巣.领域.服务.服务合同事实权威;
import 海中鱼巣.领域.服务.L2状态动态原子发布;
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
    L2场景身份 根场景{};
    L2存在身份 自我{};
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
    return 会话_v1{std::move(上下文), 根.根场景,
        自我.已形成自我->自我存在.身份, *本能根.锚点};
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

L2特征当前值变化账登记结果_v2 登记永久变化账(会话_v1& 会话);

服务维护历史覆盖起点读取结果_v1 读取服务历史覆盖起点(
    const 会话_v1& 会话) {
    const auto G0 = 当前代次(会话);
    if (!G0) return {};
    return 会话.上下文->取得服务合同事实权威服务()
        .读取服务维护历史事实账覆盖起点_v1({
            服务维护历史覆盖起点合同版本_v1,
            {L2结构合同版本, *G0}, 会话.本能根.唯一自我});
}

std::optional<本能被动维护游标事实_v3> 建立游标_v3(
    会话_v1& 会话, std::uint64_t 运行代次,
    std::uint64_t 纪元, std::uint64_t 幂等,
    std::int64_t 当前完整秒边界 = 0,
    std::uint32_t 时间源版本 = 稳态时钟时间源版本) {
    const auto 变化账登记 = 登记永久变化账(会话);
    const auto 服务历史起点 = 读取服务历史覆盖起点(会话);
    const auto G0 = 当前代次(会话);
    if (!G0 || !变化账登记.成功() || !服务历史起点.成功()
        || !服务历史起点.快照)
        return std::nullopt;
    const auto 结果 = 会话.上下文->取得本能被动维护游标服务()
        .建立或恢复本能被动维护游标_v3({
            本能被动维护游标合同版本_v3,
            {L2结构合同版本, *G0}, L2结构幂等身份{幂等}, 会话.本能根,
            运行代次, 纪元, 当前完整秒边界,
            变化账登记.首次登记事实代次, 变化账登记,
            服务历史起点.快照->历史账登记事实代次, 服务历史起点,
            时间源版本, 服务时间维护规则版本_v1});
    return 结果.成功() && 结果.提交游标
        ? 结果.提交游标 : std::nullopt;
}

本能被动维护基础快照读取请求_v1 形成请求(
    const 会话_v1& 会话, std::uint64_t 运行代次, std::uint64_t G0) {
    return {本能被动维护基础快照合同版本_v1,
        {L2结构合同版本, G0}, 会话.本能根, 运行代次};
}

本能被动维护基础快照读取请求_v2 形成请求_v2(
    const 会话_v1& 会话, std::uint64_t 运行代次, std::uint64_t G0) {
    return {本能被动维护基础快照合同版本_v2,
        {L2结构合同版本, G0}, 会话.本能根, 运行代次};
}

bool 空失败(const 本能被动维护基础快照读取结果_v1& 结果,
    本能被动维护基础快照读取状态_v1 状态) {
    return 结果.状态 == 状态 && !结果.成功() && !结果.可维护()
        && !结果.快照 && 结果.本次正式读回截止 == 0;
}

bool 空失败_v2(const 本能被动维护基础快照读取结果_v2& 结果,
    本能被动维护基础快照读取状态_v2 状态) {
    return 结果.状态 == 状态 && !结果.成功() && !结果.可维护()
        && !结果.快照 && 结果.本次正式读回截止 == 0;
}

本能单完整秒服务需求时间裁决读取请求_v1 形成裁决请求(
    const 会话_v1& 会话, std::uint64_t 运行代次, std::uint64_t G0) {
    return {本能单完整秒服务需求时间裁决合同版本_v1,
        形成请求(会话, 运行代次, G0)};
}

bool 裁决空失败(const 本能单完整秒服务需求时间裁决读取结果_v1& 结果,
    本能单完整秒服务需求时间裁决读取状态_v1 状态) {
    return 结果.状态 == 状态 && !结果.成功() && !结果.可维护()
        && !结果.快照 && 结果.本次正式读回截止 == 0;
}

bool 发布主动安全来源规则(会话_v1& 会话, std::uint64_t 幂等) {
    const auto G0 = 当前代次(会话);
    if (!G0) return false;
    const auto 结果 = 会话.上下文->取得安全根定义与当前值服务()
        .发布主动安全结算来源规则_v1({安全根定义合同版本_v1,
            {L2结构合同版本, *G0}, L2结构幂等身份{幂等}, 会话.本能根,
            1, {}, 稳定编码{0x4153'5749'4E44'4F57ULL}});
    return 结果.成功() && 结果.规则 && 结果.规则->规则版本 == 1
        && 结果.规则->完整来源方法组.empty();
}

L2特征当前值变化账登记结果_v2 登记永久变化账(会话_v1& 会话) {
    const auto G0 = 当前代次(会话);
    if (!G0) return {};
    return 会话.上下文->取得L2状态动态原子发布服务()
        .确保特征当前值变化永久账登记_v2({
            L2特征当前值变化永久账合同版本_v2,
            {L2结构合同版本, *G0}});
}

std::optional<L2特征值事实> 读取安全根当前值(const 会话_v1& 会话) {
    const auto G0 = 当前代次(会话);
    if (!G0) return std::nullopt;
    const auto 结果 = 会话.上下文->取得L2特征结构服务().读取特征当前值(
        {{L2结构合同版本, *G0}, 会话.本能根.安全根特征实例});
    return 结果.成功() && 结果.结果头.事实截止代次 == *G0 && 结果.当前值
        ? 结果.当前值 : std::nullopt;
}

bool 发布安全根变化(会话_v1& 会话, std::int64_t UTC纳秒,
    std::uint64_t 身份基数) {
    const auto G0 = 当前代次(会话);
    const auto 旧值 = 读取安全根当前值(会话);
    if (!G0 || !旧值) return false;
    const auto* 数值 = std::get_if<std::int64_t>(&旧值->类型化不可变材料);
    if (!数值 || *数值 <= 0) return false;
    L2特征当前值变化记录请求_v2 请求;
    请求.请求头 = {L2结构合同版本, *G0};
    请求.组合幂等身份 = {身份基数};
    请求.特征参与者幂等身份 = {身份基数 + 1};
    请求.状态参与者幂等身份 = {身份基数 + 2};
    请求.动态参与者幂等身份 = {身份基数 + 3};
    请求.特征实例 = 会话.本能根.安全根特征实例;
    请求.预期旧当前值 = *旧值;
    请求.新值材料 = L2原始值材料{*数值 - 1};
    请求.变化UTC纳秒 = UTC纳秒;
    请求.主体存在 = 会话.自我;
    请求.共同场景 = 会话.根场景;
    请求.来源存在 = 会话.自我;
    return 会话.上下文->取得L2状态动态原子发布服务()
        .发布特征当前值变化与中性记录_v2(请求).成功();
}

本能主动安全UTC窗口裁决读取请求_v1 形成主动安全窗口请求(
    const 会话_v1& 会话, std::uint64_t 运行代次, std::uint64_t G0,
    std::uint64_t 数量预算 = 8) {
    return {本能主动安全UTC窗口裁决合同版本_v1,
        形成请求(会话, 运行代次, G0), 1, 1'000, 数量预算};
}

bool 主动安全空失败(const 本能主动安全UTC窗口裁决读取结果_v1& 结果,
    本能主动安全UTC窗口裁决读取状态_v1 状态) {
    return 结果.状态 == 状态 && !结果.成功() && !结果.快照
        && 结果.本次正式读回截止 == 0;
}

本能主动安全UTC窗口裁决读取结果_v1 读取稳定主动安全裁决(
    const 本能主动安全UTC窗口裁决组合器& 组合器,
    const 本能主动安全UTC窗口裁决读取请求_v1& 请求) {
    本能主动安全UTC窗口裁决读取结果_v1 结果;
    for (int i = 0; i < 4; ++i) {
        结果 = 组合器.读取UTC窗口主动安全变化裁决_v1(请求);
        if (结果.状态 != 本能主动安全UTC窗口裁决读取状态_v1::当前性漂移)
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    return 结果;
}

本能主动安全事实代次覆盖裁决读取请求_v1 形成主动安全事实代次请求(
    const 会话_v1& 会话, std::uint64_t 运行代次, std::uint64_t G0,
    std::uint64_t 排除起点, std::uint64_t 数量预算 = 8) {
    return {本能主动安全事实代次覆盖裁决合同版本_v1,
        形成请求(会话, 运行代次, G0), 排除起点, 数量预算};
}

bool 事实代次主动安全空失败(
    const 本能主动安全事实代次覆盖裁决读取结果_v1& 结果,
    本能主动安全事实代次覆盖裁决读取状态_v1 状态) {
    return 结果.状态 == 状态 && !结果.成功() && !结果.快照
        && 结果.本次正式读回截止 == 0;
}

本能主动安全事实代次覆盖裁决读取结果_v1
读取稳定事实代次主动安全裁决(
    const 本能主动安全事实代次覆盖裁决组合器& 组合器,
    const 本能主动安全事实代次覆盖裁决读取请求_v1& 请求) {
    本能主动安全事实代次覆盖裁决读取结果_v1 结果;
    for (int i = 0; i < 4; ++i) {
        结果 = 组合器.读取事实代次覆盖主动安全变化裁决_v1(请求);
        if (结果.状态
            != 本能主动安全事实代次覆盖裁决读取状态_v1::当前性漂移)
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    return 结果;
}

本能单完整秒服务需求时间裁决读取结果_v1 读取稳定裁决(
    const 本能单完整秒服务需求时间裁决组合器& 组合器,
    const 本能单完整秒服务需求时间裁决读取请求_v1& 请求) {
    本能单完整秒服务需求时间裁决读取结果_v1 结果;
    for (int i = 0; i < 4; ++i) {
        结果 = 组合器.读取当前维护秒服务需求时间裁决_v1(请求);
        if (结果.状态
            != 本能单完整秒服务需求时间裁决读取状态_v1::当前性漂移)
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    return 结果;
}

template<class 操作>
bool 运行裁决场景(const std::filesystem::path& 根, std::uint64_t 身份种子,
    操作&& 执行) {
    隔离根清理 清理{根};
    std::error_code 错误;
    std::filesystem::create_directories(根 / L"l1", 错误);
    if (错误) return false;
    std::filesystem::create_directories(根 / L"material", 错误);
    if (错误) return false;
    普通应用配置 配置;
    配置.L1事实基座持久存储 = {
        L1事实基座持久恢复合同版本_v1, 根 / L"l1"};
    配置.不可变材料存储 = {L2结构合同版本, {根 / L"material"}};
    配置.等待合同登记 = 形成普通应用任务筹办等待合同登记();
    auto 会话 = 建立会话(配置, 0x5449'4D45'0000'0000ULL + 身份种子);
    if (!会话) return false;
    const auto 运行代次 = 10'000ULL + 身份种子;
    const auto 纪元 = 20'000ULL + 身份种子;
    if (!建立游标(*会话, 运行代次, 纪元,
            0x5449'4D43'0000'0000ULL + 身份种子)
        || !发布定义(*会话, 1, L1, H1,
            0x5449'4D44'0000'0000ULL + 身份种子))
        return false;
    单调时钟适配器 时间适配器({运行代次, 纪元});
    完整秒时钟服务 时钟(时间适配器,
        {运行代次, 纪元, 稳态时钟时间源版本});
    本能被动维护基础快照组合器 基础组合器(
        会话->上下文->取得L2特征结构服务(),
        会话->上下文->取得安全根定义与当前值服务(),
        会话->上下文->取得本能被动维护游标服务(), 时钟);
    本能单完整秒服务需求时间裁决组合器 裁决组合器(
        基础组合器, 会话->上下文->取得服务合同事实权威服务());
    return std::forward<操作>(执行)(
        *会话, 运行代次, 纪元, 基础组合器, 裁决组合器);
}

bool 重写等待边界并验证(
    本能单完整秒服务需求时间裁决快照_v1 快照,
    std::int64_t 当前秒, 服务维护需求裁决_v1 期望裁决) {
    if (当前秒 <= 0) return false;
    快照.基础快照.时间状态 = 完整秒边界读取状态::已读取;
    快照.基础快照.时间观察.当前完整秒边界 = 当前秒;
    快照.基础快照.时间观察.当前经过纳秒 = 当前秒 * 1'000'000'000LL;
    快照.基础快照.待维护区间 = 完整秒区间{
        1, 当前秒, static_cast<std::uint64_t>(当前秒)};
    快照.当前维护完整秒边界 = 当前秒;
    快照.最长等待完整秒 = 当前秒;
    快照.并列最长来源组.clear();
    for (std::size_t i = 0;
        i < 快照.有效未满足合同完整集合.完整合同事实组.size(); ++i) {
        auto& 合同 = 快照.有效未满足合同完整集合.完整合同事实组[i];
        auto& 状态 = 快照.有效未满足合同完整集合.当前状态事实组[i];
        合同.提出完整秒边界 = 0;
        合同.有效开始完整秒边界 = 0;
        状态.最后已结算完整秒边界 = 0;
        快照.并列最长来源组.push_back({合同.身份, 合同.提出者,
            合同.需求, 0, 当前秒});
    }
    std::sort(快照.并列最长来源组.begin(),
        快照.并列最长来源组.end(), 服务需求最长等待来源严格小于_v1);
    快照.需求裁决 = 期望裁决;
    return 本能单完整秒服务需求时间裁决快照完整_v1(快照);
}

bool 运行v2基础快照场景(const std::filesystem::path& 根) {
    const auto 失败v2 = [](const char* 步骤) {
        std::cout << "[INSTINCT-BASE-SNAPSHOT][V2-DETAIL] FAIL: "
                  << 步骤 << '\n';
        return false;
    };
    隔离根清理 清理{根};
    std::error_code 错误;
    std::filesystem::create_directories(根 / L"l1", 错误);
    if (错误) return 失败v2("create L1 root");
    std::filesystem::create_directories(根 / L"material", 错误);
    if (错误) return 失败v2("create material root");
    普通应用配置 配置;
    配置.L1事实基座持久存储 = {
        L1事实基座持久恢复合同版本_v1, 根 / L"l1"};
    配置.不可变材料存储 = {L2结构合同版本, {根 / L"material"}};
    配置.等待合同登记 = 形成普通应用任务筹办等待合同登记();
    auto 会话 = 建立会话(配置, 0x5632'4241'5345'0001ULL);
    if (!会话) return 失败v2("assemble isolated session");

    constexpr std::uint64_t 运行代次 = 30'001;
    constexpr std::uint64_t 纪元 = 30'002;
    单调时钟适配器 前置适配器({运行代次, 纪元});
    完整秒时钟服务 前置时钟(前置适配器,
        {运行代次, 纪元, 稳态时钟时间源版本});
    本能被动维护基础快照组合器 前置组合器(
        会话->上下文->取得L2特征结构服务(),
        会话->上下文->取得安全根定义与当前值服务(),
        会话->上下文->取得本能被动维护游标服务(), 前置时钟);
    const auto G初 = 当前代次(*会话);
    if (!G初 || !空失败_v2(前置组合器.读取本能被动维护基础快照_v2(
                形成请求_v2(*会话, 运行代次, *G初)),
            本能被动维护基础快照读取状态_v2::游标未建立))
        return 失败v2("unestablished v3 cursor mapping");
    if (!建立游标(*会话, 运行代次, 纪元, 0x5632'4355'5253'0001ULL))
        return 失败v2("establish v1 cursor");
    const auto G待迁移 = 当前代次(*会话);
    if (!G待迁移 || !空失败_v2(
            前置组合器.读取本能被动维护基础快照_v2(
                形成请求_v2(*会话, 运行代次, *G待迁移)),
            本能被动维护基础快照读取状态_v2::游标待迁移))
        return 失败v2("pending-migration mapping");
    const auto 三水位游标 = 建立游标_v3(*会话, 运行代次, 纪元,
        0x5632'4355'5253'0002ULL);
    if (!三水位游标) return 失败v2("migrate to v3 cursor");

    单调时钟适配器 时间适配器({运行代次, 纪元});
    完整秒时钟服务 时钟(时间适配器,
        {运行代次, 纪元, 稳态时钟时间源版本});
    本能被动维护基础快照组合器 组合器(
        会话->上下文->取得L2特征结构服务(),
        会话->上下文->取得安全根定义与当前值服务(),
        会话->上下文->取得本能被动维护游标服务(), 时钟);
    const auto G未定义 = 当前代次(*会话);
    if (!G未定义 || !空失败_v2(
            组合器.读取本能被动维护基础快照_v2(
                形成请求_v2(*会话, 运行代次, *G未定义)),
            本能被动维护基础快照读取状态_v2::定义未发布))
        return 失败v2("definition-unpublished mapping");
    if (!发布定义(*会话, 1, L1, H1, 0x5632'4445'464E'0001ULL))
        return 失败v2("publish definition v1");
    const auto G1 = 当前代次(*会话);
    if (!G1) return 失败v2("read success G");
    const auto 首版 = 组合器.读取本能被动维护基础快照_v2(
        形成请求_v2(*会话, 运行代次, *G1));
    if (!首版.成功() || !首版.快照
        || 首版.状态 != 本能被动维护基础快照读取状态_v2::无新完整秒
        || 首版.可维护() || 首版.快照->待维护区间
        || 首版.快照->安全根.定义.定义版本 != 1
        || 首版.快照->安全根.定义.低位阈值 != L1
        || 首版.快照->安全根.定义.高位阈值 != H1
        || 首版.快照->游标.已消费主动安全变化事实代次
            != 三水位游标->已消费主动安全变化事实代次
        || 首版.快照->游标.已消费服务维护历史事实代次
            != 三水位游标->已消费服务维护历史事实代次
        || 首版.快照->游标.已消费主动安全变化事实代次 > *G1
        || 首版.快照->游标.已消费服务维护历史事实代次 > *G1)
        return 失败v2("success shape and watermarks");
    if (!空失败_v2(组合器.读取本能被动维护基础快照_v2(
                形成请求_v2(*会话, 运行代次 + 1, *G1)),
            本能被动维护基础快照读取状态_v2::运行代次漂移)
        || !空失败_v2(组合器.读取本能被动维护基础快照_v2(
                形成请求_v2(*会话, 运行代次, *G1 + 1)),
            本能被动维护基础快照读取状态_v2::当前性漂移))
        return 失败v2("runtime or G drift");
    auto 坏锚点 = 形成请求_v2(*会话, 运行代次, *G1);
    坏锚点.本能根锚点.服务根特征实例 =
        L2特征实例身份{稳定编码{0x5632'4241'4446'0001ULL}};
    const auto 坏实例 = 组合器.读取本能被动维护基础快照_v2(坏锚点);
    if (坏实例.成功() || 坏实例.快照)
        return 失败v2("bad service-instance binding");

    std::optional<本能被动维护基础快照读取结果_v2> 单秒;
    for (int i = 0; i < 240; ++i) {
        const auto G = 当前代次(*会话);
        if (!G) return 失败v2("read one-second G");
        单秒 = 组合器.读取本能被动维护基础快照_v2(
            形成请求_v2(*会话, 运行代次, *G));
        if (单秒->成功() && 单秒->快照 && 单秒->快照->待维护区间
            && 单秒->快照->待维护区间->完整秒数量 == 1)
            break;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    if (!单秒 || !单秒->成功() || !单秒->可维护() || !单秒->快照
        || !单秒->快照->待维护区间
        || 单秒->快照->待维护区间->完整秒数量 != 1)
        return 失败v2("exact one-second interval");
    std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    const auto GN = 当前代次(*会话);
    if (!GN) return 失败v2("read multi-second G");
    const auto 多秒 = 组合器.读取本能被动维护基础快照_v2(
        形成请求_v2(*会话, 运行代次, *GN));
    if (!多秒.成功() || !多秒.可维护() || !多秒.快照
        || !多秒.快照->待维护区间
        || 多秒.快照->待维护区间->完整秒数量 < 2)
        return 失败v2("multi-second interval");

    单调时钟适配器 错纪元适配器({运行代次, 纪元 + 1});
    完整秒时钟服务 错纪元时钟(错纪元适配器,
        {运行代次, 纪元 + 1, 稳态时钟时间源版本});
    本能被动维护基础快照组合器 错纪元组合器(
        会话->上下文->取得L2特征结构服务(),
        会话->上下文->取得安全根定义与当前值服务(),
        会话->上下文->取得本能被动维护游标服务(), 错纪元时钟);
    const auto G错纪元 = 当前代次(*会话);
    if (!G错纪元 || !空失败_v2(
            错纪元组合器.读取本能被动维护基础快照_v2(
                形成请求_v2(*会话, 运行代次, *G错纪元)),
            本能被动维护基础快照读取状态_v2::时间纪元漂移))
        return 失败v2("time epoch drift");

    const auto 建立时间异常会话 = [&](const wchar_t* 名称,
        std::uint64_t 根身份) -> std::optional<会话_v1> {
        const auto 子根 = 根 / 名称;
        std::error_code 子错误;
        std::filesystem::create_directories(子根 / L"l1", 子错误);
        if (子错误) return std::nullopt;
        std::filesystem::create_directories(子根 / L"material", 子错误);
        if (子错误) return std::nullopt;
        普通应用配置 子配置;
        子配置.L1事实基座持久存储 = {
            L1事实基座持久恢复合同版本_v1, 子根 / L"l1"};
        子配置.不可变材料存储 = {
            L2结构合同版本, {子根 / L"material"}};
        子配置.等待合同登记 = 形成普通应用任务筹办等待合同登记();
        return 建立会话(子配置, 根身份);
    };

    auto 错版本会话 = 建立时间异常会话(
        L"wrong-source", 0x5632'5449'4D45'0001ULL);
    constexpr std::uint64_t 错版本运行代次 = 30'003;
    constexpr std::uint64_t 错版本纪元 = 30'004;
    if (!错版本会话 || !建立游标_v3(*错版本会话,
            错版本运行代次, 错版本纪元,
            0x5632'4355'5253'0003ULL, 0, 稳态时钟时间源版本 + 1))
        return 失败v2("establish wrong-source-version cursor");
    单调时钟适配器 错版本适配器({错版本运行代次, 错版本纪元});
    完整秒时钟服务 错版本时钟(错版本适配器,
        {错版本运行代次, 错版本纪元, 稳态时钟时间源版本});
    本能被动维护基础快照组合器 错版本组合器(
        错版本会话->上下文->取得L2特征结构服务(),
        错版本会话->上下文->取得安全根定义与当前值服务(),
        错版本会话->上下文->取得本能被动维护游标服务(), 错版本时钟);
    const auto G错版本 = 当前代次(*错版本会话);
    if (!G错版本 || !空失败_v2(
            错版本组合器.读取本能被动维护基础快照_v2(
                形成请求_v2(*错版本会话, 错版本运行代次, *G错版本)),
            本能被动维护基础快照读取状态_v2::时间源版本漂移))
        return 失败v2("time-source version drift");

    auto 倒退会话 = 建立时间异常会话(
        L"rollback", 0x5632'5449'4D45'0002ULL);
    constexpr std::uint64_t 倒退运行代次 = 30'005;
    constexpr std::uint64_t 倒退纪元 = 30'006;
    if (!倒退会话 || !建立游标_v3(*倒退会话,
            倒退运行代次, 倒退纪元,
            0x5632'4355'5253'0004ULL, 1'000'000))
        return 失败v2("establish rollback cursor");
    单调时钟适配器 倒退适配器({倒退运行代次, 倒退纪元});
    完整秒时钟服务 倒退时钟(倒退适配器,
        {倒退运行代次, 倒退纪元, 稳态时钟时间源版本});
    本能被动维护基础快照组合器 倒退组合器(
        倒退会话->上下文->取得L2特征结构服务(),
        倒退会话->上下文->取得安全根定义与当前值服务(),
        倒退会话->上下文->取得本能被动维护游标服务(), 倒退时钟);
    const auto G倒退 = 当前代次(*倒退会话);
    if (!G倒退 || !空失败_v2(
            倒退组合器.读取本能被动维护基础快照_v2(
                形成请求_v2(*倒退会话, 倒退运行代次, *G倒退)),
            本能被动维护基础快照读取状态_v2::时间倒退))
        return 失败v2("time rollback mapping");
    return true;
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
        auto 坏请求_v2 = 形成请求_v2(*会话, 运行代次, *G初);
        坏请求_v2.合同版本 = 0;
        if (!空失败_v2(组合器.读取本能被动维护基础快照_v2(坏请求_v2),
                本能被动维护基础快照读取状态_v2::入口拒绝))
            return 失败("V200", "invalid v2 request shape");
        通过("P00", "invalid request returns exact empty failure");

        const auto 未建立 = 组合器.读取本能被动维护基础快照_v1(
            形成请求(*会话, 运行代次, *G初));
        if (!空失败(未建立,
                本能被动维护基础快照读取状态_v1::游标未建立))
            return 失败("P01", "cursor must be explicit");
        if (!空失败_v2(组合器.读取本能被动维护基础快照_v2(
                    形成请求_v2(*会话, 运行代次, *G初)),
                本能被动维护基础快照读取状态_v2::游标未建立))
            return 失败("V201", "v3 cursor must be explicit");
        if (!建立游标(*会话, 运行代次, 纪元, 0x4241'5345'0000'0001ULL))
            return 失败("P01", "publish cursor");
        const auto G待迁移 = 当前代次(*会话);
        if (!G待迁移) return 失败("V201", "read migration G");
        if (!空失败_v2(组合器.读取本能被动维护基础快照_v2(
                    形成请求_v2(*会话, 运行代次, *G待迁移)),
                本能被动维护基础快照读取状态_v2::游标待迁移))
            return 失败("V201", "v1 cursor must be reported as pending migration");
        const auto 未定义 = 组合器.读取本能被动维护基础快照_v1(
            形成请求(*会话, 运行代次, *G待迁移));
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

        本能主动安全UTC窗口裁决组合器 主动安全组合器(
            组合器, 会话->上下文->取得安全根定义与当前值服务(),
            会话->上下文->取得L2状态动态原子发布服务());
        const auto GA0 = 当前代次(*会话);
        if (!GA0) return 失败("A00", "read active-safety initial G");
        auto 主动坏请求 = 形成主动安全窗口请求(*会话, 运行代次, *GA0);
        主动坏请求.合同版本 = 0;
        if (!主动安全空失败(
                主动安全组合器.读取UTC窗口主动安全变化裁决_v1(主动坏请求),
                本能主动安全UTC窗口裁决读取状态_v1::入口拒绝))
            return 失败("A00", "invalid active-safety request");
        auto 反向窗口 = 形成主动安全窗口请求(*会话, 运行代次, *GA0);
        反向窗口.排除结束UTC纳秒 = 反向窗口.包含起始UTC纳秒;
        if (!主动安全空失败(
                主动安全组合器.读取UTC窗口主动安全变化裁决_v1(反向窗口),
                本能主动安全UTC窗口裁决读取状态_v1::入口拒绝))
            return 失败("A00", "invalid UTC half-open window");
        通过("A00", "invalid contract and UTC window fail closed");

        const auto 未发布规则 = 读取稳定主动安全裁决(
            主动安全组合器,
            形成主动安全窗口请求(*会话, 运行代次, *GA0));
        if (!主动安全空失败(未发布规则,
                本能主动安全UTC窗口裁决读取状态_v1::规则未发布))
            return 失败("A01", "source rule must be published");
        if (!发布主动安全来源规则(
                *会话, 0x4153'5749'0000'0001ULL))
            return 失败("A01", "publish source rule v1 empty set");
        const auto GA1 = 当前代次(*会话);
        if (!GA1) return 失败("A01", "read post-rule G");
        const auto 未登记账 = 读取稳定主动安全裁决(
            主动安全组合器,
            形成主动安全窗口请求(*会话, 运行代次, *GA1));
        if (!主动安全空失败(未登记账,
                本能主动安全UTC窗口裁决读取状态_v1::账未登记))
            return 失败("A01", "ledger registration must be explicit");
        通过("A01", "unpublished rule and unregistered ledger remain distinct");

        const auto 迁移前v2 = 会话->上下文->取得本能被动维护游标服务()
            .读取当前本能被动维护游标_v2({
                本能被动维护游标合同版本_v2,
                {L2结构合同版本, *GA1}, 会话->自我});
        if (迁移前v2.状态 != 本能被动维护游标读取状态_v2::待迁移
            || 迁移前v2.成功() || 迁移前v2.游标)
            return 失败("A02", "v1 cursor must require v2 migration");
        const auto 变化账登记 = 登记永久变化账(*会话);
        if (!变化账登记.成功())
            return 失败("A02", "register permanent change ledger");
        const auto GA2 = 当前代次(*会话);
        if (!GA2) return 失败("A02", "read registered-ledger G");
        const 建立或恢复本能被动维护游标请求_v2 迁移请求{
            本能被动维护游标合同版本_v2,
            {L2结构合同版本, *GA2},
            L2结构幂等身份{0x4241'5345'0000'0100ULL}, 会话->本能根,
            运行代次, 纪元, 0, 变化账登记.首次登记事实代次,
            变化账登记, 稳态时钟时间源版本,
            服务时间维护规则版本_v1};
        const auto 迁移 = 会话->上下文->取得本能被动维护游标服务()
            .建立或恢复本能被动维护游标_v2(迁移请求);
        if (!迁移.成功() || !迁移.提交游标
            || 迁移.状态 != 本能被动维护游标建立状态_v2::已迁移
            || 迁移.提交游标->已消费主动安全变化事实代次
                != 变化账登记.首次登记事实代次)
            return 失败("A02", "migrate v1 cursor with registration watermark");
        const auto G迁移 = 当前代次(*会话);
        if (!G迁移) return 失败("A02", "read migrated cursor G");
        const auto v2读回 = 会话->上下文->取得本能被动维护游标服务()
            .读取当前本能被动维护游标_v2({
                本能被动维护游标合同版本_v2,
                {L2结构合同版本, *G迁移}, 会话->自我});
        const auto v1投影 = 会话->上下文->取得本能被动维护游标服务()
            .读取当前本能被动维护游标_v1({
                本能被动维护游标合同版本_v1,
                {L2结构合同版本, *G迁移}, 会话->自我});
        if (!v2读回.成功() || !v2读回.游标
            || *v2读回.游标 != *迁移.提交游标
            || !v1投影.成功() || !v1投影.游标
            || v1投影.游标->上一已维护完整秒边界 != 0)
            return 失败("A02", "v2 authoritative readback and v1 projection");
        auto 同纪元恢复请求 = 迁移请求;
        同纪元恢复请求.请求头.期望事实代次 = *G迁移;
        同纪元恢复请求.幂等身份 = {0x4241'5345'0000'0101ULL};
        同纪元恢复请求.变化账登记见证.本次正式读回截止 = *G迁移;
        const auto 恢复前G = 当前代次(*会话);
        const auto 同纪元恢复 = 会话->上下文->取得本能被动维护游标服务()
            .建立或恢复本能被动维护游标_v2(同纪元恢复请求);
        const auto 恢复后G = 当前代次(*会话);
        if (!恢复前G || !恢复后G || *恢复前G != *恢复后G
            || !同纪元恢复.成功()
            || 同纪元恢复.状态 != 本能被动维护游标建立状态_v2::已恢复)
            return 失败("A02", "same-epoch v2 recovery must be zero-write");
        auto 坏水位请求 = 同纪元恢复请求;
        坏水位请求.幂等身份 = {0x4241'5345'0000'0102ULL};
        ++坏水位请求.目标已消费主动安全变化事实代次;
        const auto 坏水位 = 会话->上下文->取得本能被动维护游标服务()
            .建立或恢复本能被动维护游标_v2(坏水位请求);
        if (坏水位.状态 != 本能被动维护游标建立状态_v2::引用冲突
            || 坏水位.成功() || 坏水位.提交游标)
            return 失败("A02", "same-epoch watermark advance must be rejected");
        通过("A02W", "v1 migrates to dual watermark and same-epoch advance is rejected");
        if (!运行裁决场景(隔离根 / L"cursor-new-epoch-replay", 11,
                [](会话_v1& 场景, std::uint64_t 旧运行代次,
                    std::uint64_t 旧纪元,
                    const 本能被动维护基础快照组合器&,
                    const 本能单完整秒服务需求时间裁决组合器&) {
                    const auto 登记 = 登记永久变化账(场景);
                    const auto G0 = 当前代次(场景);
                    if (!登记.成功() || !G0) return false;
                    建立或恢复本能被动维护游标请求_v2 迁移请求{
                        本能被动维护游标合同版本_v2,
                        {L2结构合同版本, *G0},
                        L2结构幂等身份{0x4241'5345'0000'0110ULL},
                        场景.本能根, 旧运行代次, 旧纪元, 0,
                        登记.首次登记事实代次, 登记,
                        稳态时钟时间源版本, 服务时间维护规则版本_v1};
                    auto& 服务 = 场景.上下文->取得本能被动维护游标服务();
                    const auto 迁移 = 服务.建立或恢复本能被动维护游标_v2(
                        迁移请求);
                    const auto G迁移 = 当前代次(场景);
                    if (!迁移.成功() || !迁移.提交游标 || !G迁移
                        || 迁移.状态
                            != 本能被动维护游标建立状态_v2::已迁移)
                        return false;

                    auto 单变运行 = 迁移请求;
                    单变运行.请求头.期望事实代次 = *G迁移;
                    单变运行.幂等身份 = {0x4241'5345'0000'0111ULL};
                    ++单变运行.运行代次;
                    const auto 单变运行结果 =
                        服务.建立或恢复本能被动维护游标_v2(单变运行);
                    auto 单变纪元 = 迁移请求;
                    单变纪元.请求头.期望事实代次 = *G迁移;
                    单变纪元.幂等身份 = {0x4241'5345'0000'0112ULL};
                    ++单变纪元.时间纪元身份;
                    const auto 单变纪元结果 =
                        服务.建立或恢复本能被动维护游标_v2(单变纪元);
                    const auto G拒绝后 = 当前代次(场景);
                    if (!G拒绝后 || *G拒绝后 != *G迁移
                        || 单变运行结果.状态
                            != 本能被动维护游标建立状态_v2::引用冲突
                        || 单变纪元结果.状态
                            != 本能被动维护游标建立状态_v2::引用冲突)
                        return false;

                    auto 新纪元请求 = 迁移请求;
                    新纪元请求.请求头.期望事实代次 = *G迁移;
                    新纪元请求.幂等身份 = {0x4241'5345'0000'0113ULL};
                    ++新纪元请求.运行代次;
                    ++新纪元请求.时间纪元身份;
                    新纪元请求.当前完整秒边界 = 3;
                    const auto 新纪元 = 服务.建立或恢复本能被动维护游标_v2(
                        新纪元请求);
                    const auto G新纪元 = 当前代次(场景);
                    if (!新纪元.成功() || !新纪元.提交游标 || !G新纪元
                        || 新纪元.状态
                            != 本能被动维护游标建立状态_v2::已建立新纪元
                        || 新纪元.提交游标->上一已维护完整秒边界 != 3
                        || 新纪元.提交游标->已消费主动安全变化事实代次
                            != 迁移.提交游标->已消费主动安全变化事实代次)
                        return false;
                    const auto 正式 = 服务.读取当前本能被动维护游标_v2({
                        本能被动维护游标合同版本_v2,
                        {L2结构合同版本, *G新纪元}, 场景.自我});
                    const auto 迁移重放 =
                        服务.建立或恢复本能被动维护游标_v2(迁移请求);
                    return 正式.成功() && 正式.游标
                        && *正式.游标 == *新纪元.提交游标
                        && 迁移重放.成功() && 迁移重放.提交游标
                        && 迁移重放.状态
                            == 本能被动维护游标建立状态_v2::精确重复
                        && *迁移重放.提交游标 == *迁移.提交游标
                        && 迁移重放.首次提交事实代次
                            == 迁移.首次提交事实代次;
                }))
            return 失败("A02W2", "new epoch inheritance and historical replay");
        通过("A02W2", "new epoch inherits watermark and old key replays first cursor");
        const auto 合法空 = 读取稳定主动安全裁决(
            主动安全组合器,
            形成主动安全窗口请求(*会话, 运行代次, *G迁移));
        if (!合法空.成功() || !合法空.快照
            || 合法空.快照->裁决 != 本能主动安全UTC窗口裁决_v1::未改变A
            || !合法空.快照->永久变化账完整组.完整变化组.empty()
            || !合法空.快照->永久变化账完整组.完整集合见证
            || 合法空.快照->永久变化账完整组.完整集合见证->声明成员数 != 0)
            return 失败("A02", "rule-v1 and legal empty ledger result");
        const auto 旧G结果 = 主动安全组合器.读取UTC窗口主动安全变化裁决_v1(
            形成主动安全窗口请求(*会话, 运行代次, *GA1));
        if (!主动安全空失败(旧G结果,
                本能主动安全UTC窗口裁决读取状态_v1::当前性漂移))
            return 失败("A02", "old G must not reuse legal-empty result");
        通过("A02", "registered complete empty ledger proves unchanged A");

        if (!发布安全根变化(*会话, 100,
                0x4153'5749'1000'0000ULL)
            || !发布安全根变化(*会话, 200,
                0x4153'5749'2000'0000ULL))
            return 失败("A03", "publish two permanent safety changes");
        const auto GA3 = 当前代次(*会话);
        if (!GA3) return 失败("A03", "read post-change G");
        const auto 预算不足 = 读取稳定主动安全裁决(
            主动安全组合器,
            形成主动安全窗口请求(*会话, 运行代次, *GA3, 1));
        if (!主动安全空失败(预算不足,
                本能主动安全UTC窗口裁决读取状态_v1::数量预算不足))
            return 失败("A03", "ledger count budget mapping");
        const auto 未裁定来源 = 读取稳定主动安全裁决(
            主动安全组合器,
            形成主动安全窗口请求(*会话, 运行代次, *GA3, 2));
        if (!主动安全空失败(未裁定来源,
                本能主动安全UTC窗口裁决读取状态_v1::来源未裁定))
            return 失败("A03", "non-empty ledger must fail closed");
        通过("A03", "budget failure and non-empty unknown source remain structured");

        using 主动状态 = 本能主动安全UTC窗口裁决读取状态_v1;
        if (本能主动安全UTC窗口裁决内部::映射规则状态(
                主动安全结算来源规则读取状态_v1::当前性漂移)
                != 主动状态::当前性漂移
            || 本能主动安全UTC窗口裁决内部::映射规则状态(
                主动安全结算来源规则读取状态_v1::引用冲突)
                != 主动状态::引用冲突
            || 本能主动安全UTC窗口裁决内部::映射规则状态(
                主动安全结算来源规则读取状态_v1::资源失败)
                != 主动状态::资源失败
            || 本能主动安全UTC窗口裁决内部::映射永久账状态(
                L2特征当前值变化组读取状态_v2::集合不闭合)
                != 主动状态::集合不闭合
            || 本能主动安全UTC窗口裁决内部::映射永久账状态(
                L2特征当前值变化组读取状态_v2::引用冲突)
                != 主动状态::引用冲突
            || 本能主动安全UTC窗口裁决内部::映射永久账状态(
                L2特征当前值变化组读取状态_v2::资源失败)
                != 主动状态::资源失败
            || 本能主动安全UTC窗口裁决内部::映射永久账状态(
                L2特征当前值变化组读取状态_v2::内部错误)
                != 主动状态::内部错误)
            return 失败("A04", "lower provider status mapping");
        通过("A04", "lower drift, closure, reference, resource and internal states map exactly");

        本能主动安全事实代次覆盖裁决组合器 事实代次组合器(
            组合器, 会话->上下文->取得安全根定义与当前值服务(),
            会话->上下文->取得L2状态动态原子发布服务());
        auto 事实代次坏请求 = 形成主动安全事实代次请求(
            *会话, 运行代次, *GA3, *GA3);
        事实代次坏请求.合同版本 = 0;
        if (!事实代次主动安全空失败(
                事实代次组合器.读取事实代次覆盖主动安全变化裁决_v1(
                    事实代次坏请求),
                本能主动安全事实代次覆盖裁决读取状态_v1::入口拒绝))
            return 失败("A05", "invalid fact-generation coverage request");
        const auto 当前空覆盖 = 读取稳定事实代次主动安全裁决(
            事实代次组合器,
            形成主动安全事实代次请求(
                *会话, 运行代次, *GA3, *GA3));
        if (!当前空覆盖.成功() || !当前空覆盖.快照
            || 当前空覆盖.快照->裁决
                != 本能主动安全事实代次覆盖裁决_v1::无尚未消费变化
            || 当前空覆盖.快照->排除已消费变化事实代次 != *GA3
            || 当前空覆盖.快照->包含结束变化事实代次 != *GA3
            || !当前空覆盖.快照->永久变化账完整组.完整变化组.empty())
            return 失败("A05", "same-G complete empty fact-generation coverage");
        通过("A05", "same-G complete empty coverage proves no unconsumed change");

        const auto 覆盖边界不可用 = 读取稳定事实代次主动安全裁决(
            事实代次组合器,
            形成主动安全事实代次请求(
                *会话, 运行代次, *GA3, *GA1));
        if (!事实代次主动安全空失败(覆盖边界不可用,
                本能主动安全事实代次覆盖裁决读取状态_v1::覆盖边界不可用))
            return 失败("A06", "pre-registration start must fail closed");
        const auto 事实代次预算不足 = 读取稳定事实代次主动安全裁决(
            事实代次组合器,
            形成主动安全事实代次请求(
                *会话, 运行代次, *GA3, *GA2, 1));
        if (!事实代次主动安全空失败(事实代次预算不足,
                本能主动安全事实代次覆盖裁决读取状态_v1::数量预算不足))
            return 失败("A06", "fact-generation member budget mapping");
        const auto 事实代次来源未裁定 = 读取稳定事实代次主动安全裁决(
            事实代次组合器,
            形成主动安全事实代次请求(
                *会话, 运行代次, *GA3, *GA2, 2));
        if (!事实代次主动安全空失败(事实代次来源未裁定,
                本能主动安全事实代次覆盖裁决读取状态_v1::来源未裁定))
            return 失败("A06", "non-empty fact-generation coverage must fail closed");
        通过("A06", "coverage boundary, budget and non-empty source remain structured");

        单调时钟适配器 裁决时间适配器({运行代次, 纪元});
        完整秒时钟服务 裁决时钟(裁决时间适配器,
            {运行代次, 纪元, 稳态时钟时间源版本});
        本能被动维护基础快照组合器 裁决基础组合器(
            会话->上下文->取得L2特征结构服务(),
            会话->上下文->取得安全根定义与当前值服务(),
            会话->上下文->取得本能被动维护游标服务(), 裁决时钟);
        本能单完整秒服务需求时间裁决组合器 时间裁决组合器(
            裁决基础组合器,
            会话->上下文->取得服务合同事实权威服务());
        const auto G空 = 当前代次(*会话);
        if (!G空) return 失败("Q00", "read empty adjudication G");
        auto 裁决坏请求 = 形成裁决请求(*会话, 运行代次, *G空);
        裁决坏请求.合同版本 = 0;
        if (!裁决空失败(
                时间裁决组合器.读取当前维护秒服务需求时间裁决_v1(
                    裁决坏请求),
                本能单完整秒服务需求时间裁决读取状态_v1::入口拒绝))
            return 失败("Q00", "invalid adjudication request");
        const auto 空裁决 = 读取稳定裁决(
            时间裁决组合器, 形成裁决请求(*会话, 运行代次, *G空));
        if (!空裁决.成功() || 空裁决.可维护() || !空裁决.快照
            || 空裁决.状态
                != 本能单完整秒服务需求时间裁决读取状态_v1::无新完整秒
            || 空裁决.快照->有效未满足需求数量 != 0
            || 空裁决.快照->最长等待完整秒
            || !空裁决.快照->并列最长来源组.empty()
            || 空裁决.快照->本秒到期未满足事件数量 != 0
            || 空裁决.快照->需求裁决
                != 服务维护需求裁决_v1::无有效未满足且无到期事件)
            return 失败("Q00", "legal empty complete adjudication");
        通过("Q00", "empty result and same-second double-read succeed");

#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
        if (!运行裁决场景(隔离根 / L"time-ties", 1,
                [](会话_v1& 场景, std::uint64_t 场景代次,
                    std::uint64_t 场景纪元,
                    const 本能被动维护基础快照组合器& 基础,
                    const 本能单完整秒服务需求时间裁决组合器& 裁决) {
                    const auto G前 = 当前代次(场景);
                    if (!G前) return false;
                    const auto 基础结果 = 基础.读取本能被动维护基础快照_v1(
                        形成请求(场景, 场景代次, *G前));
                    if (!基础结果.成功() || !基础结果.快照) return false;
                    const auto 当前秒 = 基础结果.快照->时间观察.当前完整秒边界;
                    ARCH_服务需求时间裁决验证配置_v1 配置;
                    配置.固定自我 = 场景.本能根.唯一自我;
                    配置.共同时间纪元身份 = 场景纪元;
                    配置.提出起始完整秒边界 = 当前秒;
                    配置.最后已结算完整秒边界 = 当前秒;
                    配置.到期完整秒边界 = 当前秒 + 100;
                    auto& 服务 = 场景.上下文->取得服务合同事实权威服务();
                    if (!服务.ARCH_建立需求时间裁决验证样本_v1(
                            0x5449'4D45'1000'0001ULL, 3, 0, false, 配置))
                        return false;
                    const auto G0 = 当前代次(场景);
                    if (!G0) return false;
                    const auto 结果 = 读取稳定裁决(
                        裁决, 形成裁决请求(场景, 场景代次, *G0));
                    if (!结果.成功() || !结果.快照
                        || 结果.快照->有效未满足需求数量 != 3
                        || !结果.快照->最长等待完整秒
                        || 结果.快照->并列最长来源组.size() != 3
                        || 结果.快照->需求裁决
                            != 服务维护需求裁决_v1::存在未满观察窗需求)
                        return false;
                    for (std::size_t i = 1;
                        i < 结果.快照->并列最长来源组.size(); ++i)
                        if (!服务需求最长等待来源严格小于_v1(
                                结果.快照->并列最长来源组[i - 1],
                                结果.快照->并列最长来源组[i]))
                            return false;
                    return 重写等待边界并验证(*结果.快照,
                            服务观察窗完整秒_v1 - 1,
                            服务维护需求裁决_v1::存在未满观察窗需求)
                        && 重写等待边界并验证(*结果.快照,
                            服务观察窗完整秒_v1,
                            服务维护需求裁决_v1::三十天门禁已成立);
                }))
            return 失败("Q01", "t=0, T-1/T, ties and stable ordering");
        通过("Q01", "real t=0/ties plus recomputable T-1/T boundaries");

        const auto 未来材料场景 = [&](const wchar_t* 名称,
            std::uint64_t 种子, bool 未来提出) {
            return 运行裁决场景(隔离根 / 名称, 种子,
                [未来提出](会话_v1& 场景, std::uint64_t 场景代次,
                    std::uint64_t 场景纪元,
                    const 本能被动维护基础快照组合器& 基础,
                    const 本能单完整秒服务需求时间裁决组合器& 裁决) {
                    const auto G前 = 当前代次(场景);
                    if (!G前) return false;
                    const auto 基础结果 = 基础.读取本能被动维护基础快照_v1(
                        形成请求(场景, 场景代次, *G前));
                    if (!基础结果.成功() || !基础结果.快照) return false;
                    const auto 当前秒 = 基础结果.快照->时间观察.当前完整秒边界;
                    ARCH_服务需求时间裁决验证配置_v1 配置;
                    配置.固定自我 = 场景.本能根.唯一自我;
                    配置.共同时间纪元身份 = 场景纪元;
                    配置.提出起始完整秒边界 = 未来提出 ? 当前秒 + 10 : 当前秒;
                    配置.有效开始偏移 = 未来提出 ? 0 : 10;
                    配置.最后已结算完整秒边界 = 当前秒;
                    配置.到期完整秒边界 = 当前秒 + 100;
                    auto& 服务 = 场景.上下文->取得服务合同事实权威服务();
                    if (!服务.ARCH_建立需求时间裁决验证样本_v1(
                            0x5449'4D45'2000'0000ULL + (未来提出 ? 1 : 2),
                            1, 0, false, 配置))
                        return false;
                    const auto G0 = 当前代次(场景);
                    if (!G0) return false;
                    return 裁决空失败(读取稳定裁决(
                            裁决, 形成裁决请求(场景, 场景代次, *G0)),
                        本能单完整秒服务需求时间裁决读取状态_v1::时间材料不闭合);
                });
        };
        if (!未来材料场景(L"time-future-proposal", 2, true)
            || !未来材料场景(L"time-future-effective", 3, false))
            return 失败("Q02", "future proposal/effective-start rejection");
        通过("Q02", "future proposal and effective-start fail closed");

        const auto 事件场景 = [&](const wchar_t* 名称, std::uint64_t 种子,
            int 边界偏移, bool 其它纪元,
            本能单完整秒服务需求时间裁决读取状态_v1 期望状态,
            std::uint64_t 期望事件数, std::uint32_t 事件规则版本 = 1) {
            return 运行裁决场景(隔离根 / 名称, 种子,
                [=](会话_v1& 场景, std::uint64_t 场景代次,
                    std::uint64_t 场景纪元,
                    const 本能被动维护基础快照组合器& 基础,
                    const 本能单完整秒服务需求时间裁决组合器& 裁决) {
                    std::optional<本能被动维护基础快照读取结果_v1> 基础结果;
                    for (;;) {
                        const auto G前 = 当前代次(场景);
                        if (!G前) return false;
                        基础结果 = 基础.读取本能被动维护基础快照_v1(
                            形成请求(场景, 场景代次, *G前));
                        if (!基础结果->成功() || !基础结果->快照) return false;
                        if (边界偏移 >= 0
                            || 基础结果->快照->时间观察.当前完整秒边界 > 0)
                            break;
                        std::this_thread::sleep_for(std::chrono::milliseconds(1050));
                    }
                    const auto 当前秒 =
                        基础结果->快照->时间观察.当前完整秒边界;
                    ARCH_服务需求时间裁决验证配置_v1 配置;
                    配置.固定自我 = 场景.本能根.唯一自我;
                    配置.共同时间纪元身份 = 其它纪元
                        ? 场景纪元 + 1 : 场景纪元;
                    配置.提出起始完整秒边界 = 0;
                    配置.最后已结算完整秒边界 = 当前秒;
                    配置.到期完整秒边界 = 当前秒 + 边界偏移;
                    配置.事件规则版本 = 事件规则版本;
                    auto& 服务 = 场景.上下文->取得服务合同事实权威服务();
                    if (!服务.ARCH_建立需求时间裁决验证样本_v1(
                            0x5449'4D45'3000'0000ULL + 种子,
                            1, 1, true, 配置))
                        return false;
                    const auto G0 = 当前代次(场景);
                    if (!G0) return false;
                    const auto 结果 = 读取稳定裁决(
                        裁决, 形成裁决请求(场景, 场景代次, *G0));
                    if (期望状态
                        != 本能单完整秒服务需求时间裁决读取状态_v1::已读取
                        && 期望状态
                            != 本能单完整秒服务需求时间裁决读取状态_v1::无新完整秒)
                        return 裁决空失败(结果, 期望状态);
                    return 结果.成功() && 结果.快照
                        && 结果.快照->本秒到期未满足事件数量 == 期望事件数
                        && (期望事件数 == 0
                            ? 结果.快照->需求裁决
                                == 服务维护需求裁决_v1::无有效未满足且无到期事件
                            : 结果.快照->需求裁决
                                == 服务维护需求裁决_v1::三十天门禁已成立);
                });
        };
        if (!事件场景(L"time-event-current", 4, 0, false,
                本能单完整秒服务需求时间裁决读取状态_v1::无新完整秒, 1)
            || !事件场景(L"time-event-past", 5, -1, false,
                本能单完整秒服务需求时间裁决读取状态_v1::已读取, 0)
            || !事件场景(L"time-event-future", 6, 10, false,
                本能单完整秒服务需求时间裁决读取状态_v1::时间材料不闭合, 0)
            || !事件场景(L"time-event-other-epoch", 7, 0, true,
                本能单完整秒服务需求时间裁决读取状态_v1::无新完整秒, 0))
            return 失败("Q03", "current/past/future/other-epoch event matrix");
        通过("Q03", "only current event in current epoch is counted");

        if (!运行裁决场景(隔离根 / L"time-version", 8,
                [](会话_v1& 场景, std::uint64_t 场景代次,
                    std::uint64_t 场景纪元,
                    const 本能被动维护基础快照组合器& 基础,
                    const 本能单完整秒服务需求时间裁决组合器& 裁决) {
                    const auto G前 = 当前代次(场景);
                    if (!G前) return false;
                    const auto 基础结果 = 基础.读取本能被动维护基础快照_v1(
                        形成请求(场景, 场景代次, *G前));
                    if (!基础结果.成功() || !基础结果.快照) return false;
                    const auto 当前秒 = 基础结果.快照->时间观察.当前完整秒边界;
                    ARCH_服务需求时间裁决验证配置_v1 配置;
                    配置.固定自我 = 场景.本能根.唯一自我;
                    配置.共同时间纪元身份 = 场景纪元;
                    配置.提出起始完整秒边界 = 当前秒;
                    配置.最后已结算完整秒边界 = 当前秒;
                    配置.合同规则版本 = 2;
                    auto& 服务 = 场景.上下文->取得服务合同事实权威服务();
                    if (!服务.ARCH_建立需求时间裁决验证样本_v1(
                            0x5449'4D45'4000'0001ULL, 1, 0, false, 配置))
                        return false;
                    const auto G0 = 当前代次(场景);
                    return G0 && 裁决空失败(读取稳定裁决(
                            裁决, 形成裁决请求(场景, 场景代次, *G0)),
                        本能单完整秒服务需求时间裁决读取状态_v1::版本漂移);
                })
            || !事件场景(L"time-event-version", 11, 0, false,
                本能单完整秒服务需求时间裁决读取状态_v1::版本漂移,
                0, 2))
            return 失败("Q04", "unsupported rule version");
        通过("Q04", "unsupported contract and event rule versions fail closed");

        if (!运行裁决场景(隔离根 / L"time-duplicate-demand", 9,
                [](会话_v1& 场景, std::uint64_t 场景代次,
                    std::uint64_t 场景纪元,
                    const 本能被动维护基础快照组合器& 基础,
                    const 本能单完整秒服务需求时间裁决组合器& 裁决) {
                    const auto G前 = 当前代次(场景);
                    if (!G前) return false;
                    const auto 基础结果 = 基础.读取本能被动维护基础快照_v1(
                        形成请求(场景, 场景代次, *G前));
                    if (!基础结果.成功() || !基础结果.快照) return false;
                    const auto 当前秒 = 基础结果.快照->时间观察.当前完整秒边界;
                    ARCH_服务需求时间裁决验证配置_v1 配置;
                    配置.固定自我 = 场景.本能根.唯一自我;
                    配置.共同时间纪元身份 = 场景纪元;
                    配置.提出起始完整秒边界 = 当前秒;
                    配置.最后已结算完整秒边界 = 当前秒;
                    配置.重复首个需求 = true;
                    auto& 服务 = 场景.上下文->取得服务合同事实权威服务();
                    if (!服务.ARCH_建立需求时间裁决验证样本_v1(
                            0x5449'4D45'5000'0001ULL, 2, 0, false, 配置))
                        return false;
                    const auto G0 = 当前代次(场景);
                    return G0 && 裁决空失败(读取稳定裁决(
                            裁决, 形成裁决请求(场景, 场景代次, *G0)),
                        本能单完整秒服务需求时间裁决读取状态_v1::集合不闭合);
                }))
            return 失败("Q05", "duplicate demand conflict");
        通过("Q05", "duplicate demand never inflates active count");

        if (!运行裁决场景(隔离根 / L"time-provider-failures", 10,
                [](会话_v1& 场景, std::uint64_t 场景代次,
                    std::uint64_t,
                    const 本能被动维护基础快照组合器&,
                    const 本能单完整秒服务需求时间裁决组合器& 裁决) {
                    auto& 服务 = 场景.上下文->取得服务合同事实权威服务();
                    const auto G0 = 当前代次(场景);
                    if (!G0) return false;
                    服务.ARCH_注入资源失败一次();
                    if (!裁决空失败(读取稳定裁决(
                            裁决, 形成裁决请求(场景, 场景代次, *G0)),
                            本能单完整秒服务需求时间裁决读取状态_v1::资源失败))
                        return false;
                    const auto G1 = 当前代次(场景);
                    if (!G1) return false;
                    服务.ARCH_注入读中漂移一次();
                    return 裁决空失败(读取稳定裁决(
                            裁决, 形成裁决请求(场景, 场景代次, *G1)),
                        本能单完整秒服务需求时间裁决读取状态_v1::当前性漂移);
                }))
            return 失败("Q06", "provider resource/drift mapping");
        通过("Q06", "provider resource and G0 drift remain structured");
#else
        std::cout << "[INSTINCT-BASE-SNAPSHOT][Q01-Q06] NOT_RUN: "
                     "service-contract validation macro not enabled\n";
#endif
        if (!运行v2基础快照场景(隔离根 / L"v2"))
            return 失败("V200-V209", "isolated v2 matrix");
        通过("V200-V209", "v2 migration, watermarks and time matrix close");
        return 0;
    } catch (...) {
        return 失败("EX", "unhandled exception");
    }
}

} // namespace 海中鱼巣
