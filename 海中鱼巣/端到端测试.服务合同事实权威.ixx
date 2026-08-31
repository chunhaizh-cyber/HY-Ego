module;

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <system_error>
#include <utility>
#include <variant>
#include <vector>

export module 海中鱼巣.端到端测试.服务合同事实权威;

import 海中鱼巣.核心.服务.L1事实基座;
import 海中鱼巣.领域.服务.服务合同事实权威;

export namespace 海中鱼巣 {
int 运行服务合同事实权威端到端测试() noexcept;
}

module :private;

namespace 海中鱼巣::服务合同事实权威测试内部 {

int 失败(const char* 编号, const char* 步骤) noexcept {
    std::cout << "[INSTINCT-SERVICE-CONTRACT][" << 编号 << "] FAIL: "
              << 步骤 << '\n';
    return 1;
}

void 通过(const char* 编号, const char* 说明) {
    std::cout << "[INSTINCT-SERVICE-CONTRACT][" << 编号 << "] PASS: "
              << 说明 << '\n';
}

std::optional<std::uint64_t> 当前代次(const L1事实基座运行包& 运行包) {
    const auto 读取 = 运行包.读取服务().读取中性当前事实代次(
        {L1中性CRUD合同版本});
    if (读取.状态 != L1中性读取状态::成功 || 读取.事实代次 == 0)
        return std::nullopt;
    return 读取.事实代次;
}

std::unique_ptr<服务合同事实权威服务> 建立服务(L1事实基座运行包& 运行包) {
    auto 原始交付 = 运行包.所有者范围签发器().建立所有者范围(
        {L1所有者范围CRUD合同版本, 服务合同事实权威所有者建立身份_v1,
            L1所有者范围种类::独占结构范围});
    if ((原始交付.建立结果.状态 != L1所有者范围管理状态::成功
            && 原始交付.建立结果.状态 != L1所有者范围管理状态::精确重复)
        || !原始交付.写入端口)
        return {};
    auto 交付 = 尝试形成服务合同事实权威所有者交付_v1(
        运行包.读取服务(), std::move(原始交付));
    if (!交付) return {};
    return std::make_unique<服务合同事实权威服务>(
        运行包.读取服务(), std::move(*交付));
}

bool 合同空失败(const 服务合同完整集合读取结果_v1& 结果,
    服务合同事实权威读取状态_v1 状态) {
    return 结果.状态 == 状态 && !结果.成功()
        && 结果.完整合同事实组.empty() && 结果.当前状态事实组.empty()
        && !结果.完整集合见证 && 结果.本次正式读回截止 == 0;
}

bool 事件空失败(const 服务到期事件完整集合读取结果_v1& 结果,
    服务合同事实权威读取状态_v1 状态) {
    return 结果.状态 == 状态 && !结果.成功()
        && 结果.完整事件事实组.empty() && !结果.完整集合见证
        && 结果.本次正式读回截止 == 0;
}

bool 事件终态空失败(
    const 服务到期事件合同终态完整集合读取结果_v1& 结果,
    服务合同事实权威读取状态_v1 状态) {
    return 结果.状态 == 状态 && !结果.成功()
        && 结果.完整投影组.empty() && !结果.完整集合见证
        && 结果.本次正式读回截止 == 0;
}

bool 进展空失败(const 服务进展完整集合读取结果_v1& 结果,
    服务合同事实权威读取状态_v1 状态) {
    return 结果.状态 == 状态 && !结果.成功()
        && 结果.完整进展事实组.empty() && !结果.完整集合见证
        && 结果.本次正式读回截止 == 0;
}

bool 准备空失败(const 服务准备完整集合读取结果_v1& 结果,
    服务合同事实权威读取状态_v1 状态) {
    return 结果.状态 == 状态 && !结果.成功()
        && 结果.完整准备事实组.empty() && !结果.完整集合见证
        && 结果.本次正式读回截止 == 0;
}

bool 进展空失败_v2(const 服务进展完整集合读取结果_v2& 结果,
    服务合同事实权威读取状态_v2 状态) {
    return 结果.状态 == 状态 && !结果.成功()
        && 结果.完整进展事实组.empty() && !结果.完整集合见证
        && 结果.本次正式读回截止 == 0;
}

bool 准备空失败_v2(const 服务准备完整集合读取结果_v2& 结果,
    服务合同事实权威读取状态_v2 状态) {
    return 结果.状态 == 状态 && !结果.成功()
        && 结果.完整准备事实组.empty() && !结果.完整集合见证
        && 结果.本次正式读回截止 == 0;
}

bool 进展发布空失败_v2(const 服务进展事实发布结果_v2& 结果,
    服务活动事实发布状态_v2 状态) {
    const bool 允许见证 = 状态 == 服务活动事实发布状态_v2::已可能发布;
    return 结果.状态 == 状态 && !结果.成功() && !结果.事实
        && 结果.本次正式读回截止 == 0
        && (允许见证 ? 结果.首次提交事实代次 != 0
                     : 结果.首次提交事实代次 == 0);
}

bool 准备发布空失败_v2(const 服务准备事实发布结果_v2& 结果,
    服务活动事实发布状态_v2 状态) {
    const bool 允许见证 = 状态 == 服务活动事实发布状态_v2::已可能发布;
    return 结果.状态 == 状态 && !结果.成功() && !结果.事实
        && 结果.本次正式读回截止 == 0
        && (允许见证 ? 结果.首次提交事实代次 != 0
                     : 结果.首次提交事实代次 == 0);
}

bool 历史范围空失败(const 服务维护历史事实范围读取结果_v1& 结果,
    服务维护历史事实范围读取状态_v1 状态) {
    return 结果.状态 == 状态 && !结果.成功()
        && 结果.合同变化组.empty() && 结果.合同状态变化组.empty()
        && 结果.到期事件变化组.empty() && 结果.服务进展变化组.empty()
        && 结果.服务准备变化组.empty() && !结果.完整集合见证
        && 结果.本次正式读回截止 == 0;
}

bool 历史覆盖起点空失败(
    const 服务维护历史覆盖起点读取结果_v1& 结果,
    服务维护历史覆盖起点读取状态_v1 状态) {
    return 结果.状态 == 状态 && !结果.成功() && !结果.快照
        && 结果.本次正式读回截止 == 0;
}

bool 历史边界空失败_v2(
    const 服务维护历史边界读取结果_v2& 结果,
    服务维护历史边界读取状态_v2 状态) {
    return 结果.状态 == 状态 && !结果.成功() && !结果.快照
        && 结果.本次正式读回截止 == 0;
}

服务合同完整集合读取请求_v1 合同请求(L2存在身份 自我, std::uint64_t G0) {
    return {服务合同事实权威合同版本_v1,
        {L2结构合同版本, G0}, 自我};
}

服务到期事件完整集合读取请求_v1 事件请求(
    L2存在身份 自我, std::uint64_t G0) {
    return {服务合同事实权威合同版本_v1,
        {L2结构合同版本, G0}, 自我};
}

服务进展完整集合读取请求_v1 进展请求(
    L2存在身份 自我, std::uint64_t G0) {
    return {服务进展事实扩展合同版本_v1,
        {L2结构合同版本, G0}, 自我};
}

服务准备完整集合读取请求_v1 准备请求(
    L2存在身份 自我, std::uint64_t G0) {
    return {服务准备事实扩展合同版本_v1,
        {L2结构合同版本, G0}, 自我};
}

服务进展完整集合读取请求_v2 进展请求_v2(
    L2存在身份 自我, std::uint64_t G0) {
    return {服务进展事实扩展合同版本_v2,
        {L2结构合同版本, G0}, 自我};
}

服务准备完整集合读取请求_v2 准备请求_v2(
    L2存在身份 自我, std::uint64_t G0) {
    return {服务准备事实扩展合同版本_v2,
        {L2结构合同版本, G0}, 自我};
}

服务维护历史事实范围读取请求_v1 历史范围请求(
    L2存在身份 自我, std::uint64_t 起点, std::uint64_t G0,
    std::uint64_t 预算 = 1024) {
    return {服务维护历史事实账合同版本_v1,
        {L2结构合同版本, G0}, 自我, 起点, 预算};
}

服务维护历史边界读取请求_v2 历史边界请求_v2(
    L2存在身份 自我, std::uint64_t Gstart, std::uint64_t G0,
    std::uint64_t 左边界预算 = 1024,
    std::uint64_t 变化预算 = 1024) {
    return {服务维护历史边界合同版本_v2,
        {L2结构合同版本, G0}, 自我, Gstart,
        左边界预算, 变化预算};
}

struct 外部端点交付 final {
    L1所有者范围写端口 端口;
    std::array<稳定编码, 11> 节点{};

    外部端点交付(L1所有者范围写端口&& 写端口,
        std::array<稳定编码, 11> 节点组) noexcept
        : 端口(std::move(写端口)), 节点(std::move(节点组)) {}
    外部端点交付(const 外部端点交付&) = delete;
    外部端点交付& operator=(const 外部端点交付&) = delete;
    外部端点交付(外部端点交付&&) noexcept = default;
    外部端点交付& operator=(外部端点交付&&) noexcept = default;
};

std::optional<外部端点交付> 建立外部端点(
    L1事实基座运行包& 运行包, std::uint64_t 建立身份,
    std::uint64_t 写入身份) {
    auto 交付 = 运行包.所有者范围签发器().建立所有者范围(
        {L1所有者范围CRUD合同版本, {建立身份},
            L1所有者范围种类::独占结构范围});
    if ((交付.建立结果.状态 != L1所有者范围管理状态::成功
            && 交付.建立结果.状态 != L1所有者范围管理状态::精确重复)
        || !交付.写入端口)
        return std::nullopt;
    const auto G0 = 当前代次(运行包);
    if (!G0) return std::nullopt;
    L1所有者范围写集请求 写集;
    写集.期望事实代次 = *G0;
    写集.写入幂等身份 = {写入身份};
    for (std::uint32_t i = 1; i <= 11; ++i)
        写集.节点.push_back({{i}, 节点种类::普通, std::nullopt});
    const auto 写入 = 交付.写入端口->提交所有者范围中性写集(写集);
    if (写入.状态 != L1所有者范围写入状态::成功
        || 写入.事实代次 == 0 || 写入.新编码映射.size() != 11)
        return std::nullopt;
    std::array<稳定编码, 11> 节点{};
    for (std::uint32_t i = 1; i <= 11; ++i) {
        const auto it = std::find_if(写入.新编码映射.begin(),
            写入.新编码映射.end(), [i](const auto& 项) {
                return 项.first == L1所有者范围写集本地键{i};
            });
        if (it == 写入.新编码映射.end()) return std::nullopt;
        节点[i - 1] = it->second;
    }
    return 外部端点交付{std::move(*交付.写入端口), std::move(节点)};
}

bool 外部端点仍当前(
    const L1事实基座运行包& 运行包, const 外部端点交付& 端点) {
    for (const auto 身份 : 端点.节点) {
        const auto 读 = 运行包.读取服务().读取所有者范围当前节点(
            {L1所有者范围CRUD合同版本, 身份});
        if (读.状态 != L1所有者范围读取状态::成功 || !读.事实)
            return false;
    }
    return true;
}

struct 发布测试上下文 final {
    L2存在身份 自我{};
    服务合同身份_v1 服务合同{};
    L2需求身份 需求{};
    外部端点交付 外部端点;

    发布测试上下文(L2存在身份 self, 服务合同身份_v1 contract,
        L2需求身份 demand, 外部端点交付&& endpoints) noexcept
        : 自我(self), 服务合同(contract), 需求(demand),
          外部端点(std::move(endpoints)) {}
    发布测试上下文(const 发布测试上下文&) = delete;
    发布测试上下文& operator=(const 发布测试上下文&) = delete;
    发布测试上下文(发布测试上下文&&) noexcept = default;
    发布测试上下文& operator=(发布测试上下文&&) noexcept = default;
};

#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
std::optional<发布测试上下文> 建立发布测试上下文(
    L1事实基座运行包& 运行包, 服务合同事实权威服务& 服务,
    std::uint64_t 身份基数) {
    const auto 自我 = 服务.ARCH_建立历史到期事件样本_v1(
        身份基数 + 1, false);
    auto 外部端点 = 建立外部端点(
        运行包, 身份基数 + 2, 身份基数 + 3);
    const auto G0 = 当前代次(运行包);
    if (!自我 || !外部端点 || !G0) return std::nullopt;
    const auto 合同组 = 服务.读取当前有效未满足服务合同完整集合_v1(
        合同请求(*自我, *G0));
    if (!合同组.成功() || 合同组.完整合同事实组.size() != 1)
        return std::nullopt;
    const auto& 合同 = 合同组.完整合同事实组.front();
    return 发布测试上下文{
        *自我, 合同.身份, 合同.需求, std::move(*外部端点)};
}
#endif

发布服务进展事实请求_v2 形成进展发布请求(
    const 发布测试上下文& c, std::uint64_t G0,
    std::uint64_t 幂等身份, std::uint64_t 流序号,
    服务进展运行状态_v1 状态 = 服务进展运行状态_v1::进行中,
    bool 携带状态 = true, bool 携带动态 = true) {
    发布服务进展事实请求_v2 r;
    r.请求头 = {L2结构合同版本, G0};
    r.幂等身份 = {幂等身份};
    auto& m = r.材料;
    m.自我 = c.自我; m.服务合同 = c.服务合同; m.需求 = c.需求;
    m.任务 = L2任务身份{c.外部端点.节点[0]};
    m.方法 = L2方法身份{c.外部端点.节点[1]};
    m.T到D关系稳定编码 = {0x5055'4200'0000'0000ULL + 流序号};
    m.执行绑定 = {L2任务方法选择记录身份{c.外部端点.节点[2]},
        L2任务执行绑定冻结材料身份{c.外部端点.节点[3]},
        L2实例方法身份{c.外部端点.节点[4]}, 1, 1};
    if (携带状态) m.进展状态 = L2状态身份{c.外部端点.节点[5]};
    if (携带动态) m.进展动态 = L2动态身份{c.外部端点.节点[6]};
    m.运行状态 = 状态; m.运行代次 = 流序号;
    m.计量窗口开始完整秒边界 = 10;
    m.进展发生完整秒边界 = 11;
    m.计量窗口结束完整秒边界 = 12;
    m.方法内容版本 = 1; m.方法规格版本 = 1;
    m.方法生命周期版本 = 1; m.进展规则版本 = 1;
    return r;
}

发布服务准备事实请求_v2 形成准备发布请求(
    const 发布测试上下文& c, std::uint64_t G0,
    std::uint64_t 幂等身份, std::uint64_t 流序号,
    服务准备运行状态_v1 状态 = 服务准备运行状态_v1::进行中,
    bool 需求来源 = true, bool 携带状态 = true, bool 携带动态 = true) {
    发布服务准备事实请求_v2 r;
    r.请求头 = {L2结构合同版本, G0};
    r.幂等身份 = {幂等身份};
    auto& m = r.材料;
    m.自我 = c.自我;
    m.来源 = 需求来源
        ? 服务准备来源身份_v1{c.需求}
        : 服务准备来源身份_v1{
            服务能力缺口身份_v1{c.外部端点.节点[9]}};
    m.准备目标 = {{0x5052'4500'0000'0000ULL + 流序号}};
    m.适用服务范围 = {0x5052'5300'0000'0000ULL + 流序号};
    m.有效开始完整秒边界 = 10;
    m.有效结束完整秒边界 = 30;
    m.任务 = L2任务身份{c.外部端点.节点[0]};
    m.方法 = L2方法身份{c.外部端点.节点[1]};
    if (需求来源)
        m.T到D关系稳定编码 = 稳定编码{
            0x5052'5400'0000'0000ULL + 流序号};
    m.执行绑定 = {L2任务方法选择记录身份{c.外部端点.节点[2]},
        L2任务执行绑定冻结材料身份{c.外部端点.节点[3]},
        L2实例方法身份{c.外部端点.节点[4]}, 1, 1};
    if (携带状态) m.进展状态 = L2状态身份{c.外部端点.节点[5]};
    if (携带动态) m.进展动态 = L2动态身份{c.外部端点.节点[6]};
    if (状态 == 服务准备运行状态_v1::已完整完成) {
        m.准备结果 = 服务准备结果身份_v1{c.外部端点.节点[7]};
        m.完成验证 = 服务准备验证事实身份_v1{c.外部端点.节点[8]};
    }
    m.运行状态 = 状态; m.运行代次 = 流序号;
    m.计量窗口开始完整秒边界 = 10;
    m.进展发生完整秒边界 = 11;
    m.计量窗口结束完整秒边界 = 12;
    m.方法内容版本 = 1; m.方法规格版本 = 1;
    m.方法生命周期版本 = 1; m.准备规则版本 = 1;
    return r;
}

struct 隔离根清理 final {
    std::filesystem::path 根;
    ~隔离根清理() {
        std::error_code 错误;
        std::filesystem::remove_all(根, 错误);
    }
};

#if defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
int 验证历史边界v2_B00() {
    auto 运行包 = 建立L1事实基座运行包();
    auto 服务 = 建立服务(运行包);
    const auto Gseed = 当前代次(运行包);
    const auto 自我 = 服务 && Gseed
        ? 服务->ARCH_建立v2验证样本(
            0x5342'0000'0000'0001ULL, 0, 0)
        : std::optional<L2存在身份>{};
    const auto G0 = 当前代次(运行包);
    if (!服务 || !Gseed || !自我 || !G0 || *G0 < *Gseed)
        return 失败("B00", "empty v2 boundary fixture");
    const auto 从种子 = 服务->按事实代次边界读取服务维护历史完整快照_v2(
        历史边界请求_v2(*自我, *Gseed, *G0));
    if (!从种子.成功() || !从种子.快照
        || 从种子.快照->v2覆盖登记事实代次 != *Gseed
        || 从种子.快照->自我 != *自我
        || 从种子.快照->左边界事实代次 != *Gseed
        || 从种子.快照->包含结束事实代次 != *G0
        || 从种子.快照->声明左边界成员数 != 0
        || 从种子.快照->声明变化成员数 != 0
        || !从种子.快照->规范左边界成员身份组.empty()
        || !从种子.快照->规范变化成员身份组.empty()
        || !从种子.快照->左边界合同组.empty()
        || !从种子.快照->左边界进展组.empty()
        || !从种子.快照->左边界准备组.empty()
        || !从种子.快照->合同变化组.empty()
        || !从种子.快照->合同状态变化组.empty()
        || !从种子.快照->到期事件变化组.empty()
        || !从种子.快照->服务进展变化组.empty()
        || !从种子.快照->服务准备变化组.empty()
        || 从种子.本次正式读回截止 != *G0)
        return 失败("B00", "empty seed and complete zero-member witness");
    const auto 同点 = 服务->按事实代次边界读取服务维护历史完整快照_v2(
        历史边界请求_v2(*自我, *G0, *G0));
    if (!同点.成功() || !同点.快照
        || 同点.快照->左边界事实代次 != *G0
        || 同点.快照->包含结束事实代次 != *G0
        || 同点.快照->声明左边界成员数 != 0
        || 同点.快照->声明变化成员数 != 0)
        return 失败("B00", "Gstart equals G0 legal empty boundary");
    通过("B00", "empty seed and Gseed/Gstart/G0 boundaries are complete");
    return 0;
}

bool 历史边界B01头完整(
    const 服务维护历史边界完整范围快照_v2& s,
    std::uint64_t Gseed, std::uint64_t Gstart, std::uint64_t G0) {
    return s.v2覆盖登记事实代次 == Gseed
        && s.左边界事实代次 == Gstart && s.包含结束事实代次 == G0
        && s.声明左边界成员数 == 4 && s.声明变化成员数 == 2;
}

bool 历史边界B01左边界形状完整(
    const 服务维护历史边界完整范围快照_v2& s) {
    return s.左边界合同组.size() == 1
        && s.左边界进展组.size() == 1
        && s.左边界准备组.size() == 1;
}

bool 历史边界B01变化形状完整(
    const 服务维护历史边界完整范围快照_v2& s) {
    return s.合同变化组.empty() && s.合同状态变化组.empty()
        && s.到期事件变化组.empty() && s.服务进展变化组.size() == 1
        && s.服务准备变化组.size() == 1;
}

bool 历史边界B01执行绑定相同(
    const 服务活动任务执行绑定_v2& a,
    const 服务活动任务执行绑定_v2& b) {
    return a.正式选择 == b.正式选择
        && a.执行冻结材料 == b.执行冻结材料
        && a.实例方法 == b.实例方法
        && a.筹办轮次 == b.筹办轮次 && a.执行轮次 == b.执行轮次;
}

bool 历史边界B01进展身份绑定相同(
    const 服务合同关联进展事实_v2& a,
    const 服务合同关联进展事实_v2& b) {
    if (a.身份 != b.身份 || a.自我 != b.自我 || a.服务合同 != b.服务合同)
        return false;
    if (a.需求 != b.需求 || a.任务 != b.任务 || a.方法 != b.方法)
        return false;
    return a.T到D关系稳定编码 == b.T到D关系稳定编码
        && 历史边界B01执行绑定相同(a.执行绑定, b.执行绑定);
}

bool 历史边界B01进展运行证据相同(
    const 服务合同关联进展事实_v2& a,
    const 服务合同关联进展事实_v2& b) {
    if (a.进展状态 != b.进展状态 || a.进展动态 != b.进展动态
        || a.运行状态 != b.运行状态 || a.运行代次 != b.运行代次)
        return false;
    if (a.进展发生完整秒边界 != b.进展发生完整秒边界
        || a.计量窗口开始完整秒边界 != b.计量窗口开始完整秒边界
        || a.计量窗口结束完整秒边界 != b.计量窗口结束完整秒边界)
        return false;
    if (a.方法内容版本 != b.方法内容版本
        || a.方法规格版本 != b.方法规格版本
        || a.方法生命周期版本 != b.方法生命周期版本
        || a.进展规则版本 != b.进展规则版本)
        return false;
    return a.形成事实代次 == b.形成事实代次
        && a.生命周期.创建事实代次 == b.生命周期.创建事实代次
        && a.生命周期.退出事实代次 == b.生命周期.退出事实代次;
}

bool 历史边界B01准备来源相同(
    const 服务准备来源身份_v1& a, const 服务准备来源身份_v1& b) {
    if (a.index() != b.index()) return false;
    if (std::holds_alternative<L2需求身份>(a))
        return std::get<L2需求身份>(a) == std::get<L2需求身份>(b);
    return std::get<服务能力缺口身份_v1>(a)
        == std::get<服务能力缺口身份_v1>(b);
}

bool 历史边界B01准备身份绑定相同(
    const 服务准备当前事实_v2& a, const 服务准备当前事实_v2& b) {
    if (a.身份 != b.身份 || a.自我 != b.自我
        || !历史边界B01准备来源相同(a.来源, b.来源))
        return false;
    if (a.准备目标 != b.准备目标 || a.适用服务范围 != b.适用服务范围
        || a.有效开始完整秒边界 != b.有效开始完整秒边界
        || a.有效结束完整秒边界 != b.有效结束完整秒边界)
        return false;
    if (a.任务 != b.任务 || a.方法 != b.方法
        || a.T到D关系稳定编码 != b.T到D关系稳定编码)
        return false;
    return 历史边界B01执行绑定相同(a.执行绑定, b.执行绑定);
}

bool 历史边界B01准备运行证据相同(
    const 服务准备当前事实_v2& a, const 服务准备当前事实_v2& b) {
    if (a.进展状态 != b.进展状态 || a.进展动态 != b.进展动态
        || a.准备结果 != b.准备结果 || a.完成验证 != b.完成验证)
        return false;
    if (a.运行状态 != b.运行状态 || a.运行代次 != b.运行代次
        || a.进展发生完整秒边界 != b.进展发生完整秒边界
        || a.计量窗口开始完整秒边界 != b.计量窗口开始完整秒边界
        || a.计量窗口结束完整秒边界 != b.计量窗口结束完整秒边界)
        return false;
    if (a.方法内容版本 != b.方法内容版本
        || a.方法规格版本 != b.方法规格版本
        || a.方法生命周期版本 != b.方法生命周期版本
        || a.准备规则版本 != b.准备规则版本)
        return false;
    return a.形成事实代次 == b.形成事实代次
        && a.生命周期.创建事实代次 == b.生命周期.创建事实代次
        && a.生命周期.退出事实代次 == b.生命周期.退出事实代次;
}

bool 历史边界B01进展后继证据完整(
    const 服务维护历史边界完整范围快照_v2& s,
    const 服务合同关联进展事实_v2& 首进展,
    const 服务合同关联进展事实_v2& 次进展) {
    const auto& 左进展 = s.左边界进展组.front();
    return 历史边界B01进展身份绑定相同(左进展.事实, 首进展)
        && 历史边界B01进展运行证据相同(左进展.事实, 首进展)
        && 左进展.退出当前事实代次 == 次进展.形成事实代次
        && 历史边界B01进展身份绑定相同(
            s.服务进展变化组.front().事实, 次进展)
        && 历史边界B01进展运行证据相同(
            s.服务进展变化组.front().事实, 次进展);
}

bool 历史边界B01准备后继证据完整(
    const 服务维护历史边界完整范围快照_v2& s,
    const 服务准备当前事实_v2& 首准备,
    const 服务准备当前事实_v2& 次准备) {
    const auto& 左准备 = s.左边界准备组.front();
    return 历史边界B01准备身份绑定相同(左准备.事实, 首准备)
        && 历史边界B01准备运行证据相同(左准备.事实, 首准备)
        && 左准备.退出当前事实代次 == 次准备.形成事实代次
        && 历史边界B01准备身份绑定相同(
            s.服务准备变化组.front().事实, 次准备)
        && 历史边界B01准备运行证据相同(
            s.服务准备变化组.front().事实, 次准备);
}

bool 历史边界B01预算完整(服务合同事实权威服务& 服务,
    L2存在身份 自我, std::uint64_t Gstart, std::uint64_t G0) {
    return 历史边界空失败_v2(
            服务.按事实代次边界读取服务维护历史完整快照_v2(
                历史边界请求_v2(自我, Gstart, G0, 3, 2)),
            服务维护历史边界读取状态_v2::左边界预算不足)
        && 历史边界空失败_v2(
            服务.按事实代次边界读取服务维护历史完整快照_v2(
                历史边界请求_v2(自我, Gstart, G0, 4, 1)),
            服务维护历史边界读取状态_v2::变化预算不足);
}

bool 历史边界B01_v1回归完整(服务合同事实权威服务& 服务,
    L2存在身份 自我, std::uint64_t Gstart, std::uint64_t G0) {
    const auto r = 服务.按事实代次范围读取服务维护历史事实完整组_v1(
        历史范围请求(自我, Gstart, G0, 2));
    return r.成功() && r.完整集合见证 && r.合同变化组.empty()
        && r.合同状态变化组.empty() && r.到期事件变化组.empty()
        && r.服务进展变化组.size() == 1 && r.服务准备变化组.size() == 1;
}

int 验证历史边界v2_B01() {
    服务合同事实权威服务::ARCH_延迟下一实例v2历史覆盖登记();
    auto 运行包 = 建立L1事实基座运行包();
    auto 服务 = 建立服务(运行包);
    auto 上下文 = 服务 ? 建立发布测试上下文(
        运行包, *服务, 0x5342'0100'0000'0000ULL)
        : std::optional<发布测试上下文>{};
    const auto G准备 = 当前代次(运行包);
    const auto 首进展 = 服务 && 上下文 && G准备
        ? 服务->发布服务进展事实_v2(形成进展发布请求(
            *上下文, *G准备, 0x5342'0101'0000'0001ULL, 1))
        : 服务进展事实发布结果_v2{};
    const auto G进展 = 当前代次(运行包);
    const auto 首准备 = 服务 && 上下文 && G进展
        ? 服务->发布服务准备事实_v2(形成准备发布请求(
            *上下文, *G进展, 0x5342'0102'0000'0001ULL, 1))
        : 服务准备事实发布结果_v2{};
    const auto Glegacy = 当前代次(运行包);
    const auto Gseed = 服务 && 首进展.成功() && 首准备.成功()
        ? 服务->ARCH_完成延迟v2历史覆盖登记()
        : std::optional<std::uint64_t>{};
    const auto Gstart = 当前代次(运行包);
    if (!服务 || !上下文 || !首进展.成功() || !首进展.事实
        || !首准备.成功() || !首准备.事实 || !Glegacy
        || !Gseed || !Gstart || *Gseed <= *Glegacy || *Gstart != *Gseed)
        return 失败("B01", "left-boundary incumbent fixture");

    auto 次进展请求 = 形成进展发布请求(*上下文, *Gstart,
        0x5342'0101'0000'0002ULL, 1,
        服务进展运行状态_v1::等待条件, false, true);
    次进展请求.预期当前事实 = 首进展.事实->身份;
    次进展请求.材料.运行代次 = 2;
    const auto 次进展 = 服务->发布服务进展事实_v2(次进展请求);
    const auto G进展2 = 当前代次(运行包);
    auto 次准备请求 = G进展2
        ? 形成准备发布请求(*上下文, *G进展2,
            0x5342'0102'0000'0002ULL, 1,
            服务准备运行状态_v1::等待条件, true, false, true)
        : 发布服务准备事实请求_v2{};
    次准备请求.预期当前事实 = 首准备.事实->身份;
    次准备请求.材料.运行代次 = 2;
    const auto 次准备 = G进展2
        ? 服务->发布服务准备事实_v2(次准备请求)
        : 服务准备事实发布结果_v2{};
    const auto G0 = 当前代次(运行包);
    if (!次进展.成功() || !次进展.事实
        || !次准备.成功() || !次准备.事实 || !G0)
        return 失败("B01", "incumbent successor publication");

    const auto 完整 = 服务->按事实代次边界读取服务维护历史完整快照_v2(
        历史边界请求_v2(上下文->自我, *Gstart, *G0, 4, 2));
    if (!完整.成功() || !完整.快照
        || !历史边界B01头完整(*完整.快照, *Gseed, *Gstart, *G0)
        || !历史边界B01左边界形状完整(*完整.快照)
        || !历史边界B01变化形状完整(*完整.快照)
        || !历史边界B01进展后继证据完整(
            *完整.快照, *首进展.事实, *次进展.事实)
        || !历史边界B01准备后继证据完整(
            *完整.快照, *首准备.事实, *次准备.事实))
        return 失败("B01", "left boundary and two successors");
    if (!历史边界B01预算完整(*服务, 上下文->自我, *Gstart, *G0))
        return 失败("B01", "independent left and change budgets");
    if (!历史边界B01_v1回归完整(*服务, 上下文->自我, *Gstart, *G0))
        return 失败("B01", "v1 history range regression");
    通过("B01", "incumbent exits, successors, budgets and v1 range remain closed");
    return 0;
}

int 验证历史边界v2_B02() {
    auto 运行包 = 建立L1事实基座运行包();
    auto 服务 = 建立服务(运行包);
    const auto Gseed = 当前代次(运行包);
    const auto 自我 = 服务 && Gseed
        ? 服务->ARCH_建立v2验证样本(
            0x5342'0200'0000'0001ULL, 0, 0)
        : std::optional<L2存在身份>{};
    const auto G0 = 当前代次(运行包);
    if (!服务 || !Gseed || *Gseed <= 1 || !自我 || !G0
        || *G0 <= *Gseed)
        return 失败("B02", "boundary failure fixture");
    auto 坏版本 = 历史边界请求_v2(*自我, *Gseed, *G0);
    坏版本.合同版本 = 0;
    if (!历史边界空失败_v2(
            服务->按事实代次边界读取服务维护历史完整快照_v2(坏版本),
            服务维护历史边界读取状态_v2::入口拒绝)
        || !历史边界空失败_v2(
            服务->按事实代次边界读取服务维护历史完整快照_v2(
                历史边界请求_v2(*自我, *G0 + 1, *G0)),
            服务维护历史边界读取状态_v2::入口拒绝)
        || !历史边界空失败_v2(
            服务->按事实代次边界读取服务维护历史完整快照_v2(
                历史边界请求_v2(*自我, *Gseed, *G0, 0, 1)),
            服务维护历史边界读取状态_v2::入口拒绝)
        || !历史边界空失败_v2(
            服务->按事实代次边界读取服务维护历史完整快照_v2(
                历史边界请求_v2(*自我, *Gseed, *G0, 1, 0)),
            服务维护历史边界读取状态_v2::入口拒绝))
        return 失败("B02", "entry rejection matrix");
    if (!历史边界空失败_v2(
            服务->按事实代次边界读取服务维护历史完整快照_v2(
                历史边界请求_v2(*自我, *Gseed - 1, *G0)),
            服务维护历史边界读取状态_v2::覆盖边界不可用))
        return 失败("B02", "Gstart before Gseed");
    if (!历史边界空失败_v2(
            服务->按事实代次边界读取服务维护历史完整快照_v2(
                历史边界请求_v2(*自我, *Gseed, *G0 - 1)),
            服务维护历史边界读取状态_v2::当前性漂移))
        return 失败("B02", "read-before current generation drift");
    服务->ARCH_注入读中漂移一次();
    if (!历史边界空失败_v2(
            服务->按事实代次边界读取服务维护历史完整快照_v2(
                历史边界请求_v2(*自我, *Gseed, *G0)),
            服务维护历史边界读取状态_v2::当前性漂移))
        return 失败("B02", "read-middle generation drift");
    const auto G1 = 当前代次(运行包);
    if (!G1) return 失败("B02", "resource failure generation");
    服务->ARCH_注入资源失败一次();
    if (!历史边界空失败_v2(
            服务->按事实代次边界读取服务维护历史完整快照_v2(
                历史边界请求_v2(*自我, *Gseed, *G1)),
            服务维护历史边界读取状态_v2::资源失败))
        return 失败("B02", "resource failure empty result");
    通过("B02", "boundary, drift, resource and all empty failures are exact");
    return 0;
}

bool 历史边界B03种子头完整(
    const 服务维护历史边界完整范围快照_v2& s,
    std::uint64_t Gseed) {
    return s.v2覆盖登记事实代次 == Gseed
        && s.声明左边界成员数 == 4 && s.声明变化成员数 == 0;
}

bool 历史边界B03种子形状完整(
    const 服务维护历史边界完整范围快照_v2& s) {
    if (s.左边界合同组.size() != 1 || s.左边界进展组.size() != 1
        || s.左边界准备组.size() != 1 || !s.规范变化成员身份组.empty())
        return false;
    return s.合同变化组.empty() && s.合同状态变化组.empty()
        && s.到期事件变化组.empty() && s.服务进展变化组.empty()
        && s.服务准备变化组.empty();
}

bool 历史边界B03种子首事实完整(
    const 服务维护历史边界完整范围快照_v2& s,
    const 服务合同关联进展事实_v2& 首进展,
    const 服务准备当前事实_v2& 首准备) {
    const auto& 进展 = s.左边界进展组.front().事实;
    const auto& 准备 = s.左边界准备组.front().事实;
    return 历史边界B01进展身份绑定相同(进展, 首进展)
        && 历史边界B01进展运行证据相同(进展, 首进展)
        && 历史边界B01准备身份绑定相同(准备, 首准备)
        && 历史边界B01准备运行证据相同(准备, 首准备);
}

int 验证历史边界v2_B03() {
    服务合同事实权威服务::ARCH_延迟下一实例v2历史覆盖登记();
    auto 运行包 = 建立L1事实基座运行包();
    auto 服务 = 建立服务(运行包);
    auto 上下文 = 服务 ? 建立发布测试上下文(
        运行包, *服务, 0x5342'0300'0000'0000ULL)
        : std::optional<发布测试上下文>{};
    const auto G准备 = 当前代次(运行包);
    const auto 首进展 = 服务 && 上下文 && G准备
        ? 服务->发布服务进展事实_v2(形成进展发布请求(
            *上下文, *G准备, 0x5342'0301'0000'0001ULL, 1))
        : 服务进展事实发布结果_v2{};
    const auto G进展 = 当前代次(运行包);
    const auto 首准备 = 服务 && 上下文 && G进展
        ? 服务->发布服务准备事实_v2(形成准备发布请求(
            *上下文, *G进展, 0x5342'0302'0000'0001ULL, 1))
        : 服务准备事实发布结果_v2{};
    const auto Glegacy = 当前代次(运行包);
    const auto Gseed = 服务 && 上下文 && 首进展.成功() && 首准备.成功()
        ? 服务->ARCH_完成延迟v2历史覆盖登记()
        : std::optional<std::uint64_t>{};
    const auto G0 = 当前代次(运行包);
    if (!服务 || !上下文 || !首进展.事实 || !首准备.事实
        || !Glegacy || !Gseed || !G0
        || *Gseed <= *Glegacy || *G0 != *Gseed)
        return 失败("B03", "production registration seed fixture");
    const auto 种子 = 服务->按事实代次边界读取服务维护历史完整快照_v2(
        历史边界请求_v2(上下文->自我, *Gseed, *G0, 4, 1));
    if (!种子.成功() || !种子.快照
        || !历史边界B03种子头完整(*种子.快照, *Gseed)
        || !历史边界B03种子形状完整(*种子.快照)
        || !历史边界B03种子首事实完整(
            *种子.快照, *首进展.事实, *首准备.事实))
        return 失败("B03", "legacy incumbents frozen by production seed path");
    通过("B03", "delayed upgrade uses the production registration path once");
    return 0;
}

std::optional<服务维护历史边界读取结果_v2>
形成历史边界B04左边界快照() {
    服务合同事实权威服务::ARCH_延迟下一实例v2历史覆盖登记();
    auto 运行包 = 建立L1事实基座运行包();
    auto 服务 = 建立服务(运行包);
    auto 上下文 = 服务 ? 建立发布测试上下文(
        运行包, *服务, 0x5342'0400'0000'0000ULL)
        : std::optional<发布测试上下文>{};
    const auto G准备 = 当前代次(运行包);
    const auto 进展 = 服务 && 上下文 && G准备
        ? 服务->发布服务进展事实_v2(形成进展发布请求(
            *上下文, *G准备, 0x5342'0401'0000'0001ULL, 1))
        : 服务进展事实发布结果_v2{};
    const auto G进展 = 当前代次(运行包);
    const auto 准备 = 服务 && 上下文 && G进展
        ? 服务->发布服务准备事实_v2(形成准备发布请求(
            *上下文, *G进展, 0x5342'0402'0000'0001ULL, 1))
        : 服务准备事实发布结果_v2{};
    const auto Gseed = 服务 && 进展.成功() && 准备.成功()
        ? 服务->ARCH_完成延迟v2历史覆盖登记()
        : std::optional<std::uint64_t>{};
    const auto G0 = 当前代次(运行包);
    if (!服务 || !上下文 || !进展.成功() || !准备.成功()
        || !Gseed || !G0 || *Gseed != *G0)
        return std::nullopt;
    auto 结果 = 服务->按事实代次边界读取服务维护历史完整快照_v2(
        历史边界请求_v2(上下文->自我, *Gseed, *G0, 4, 1));
    if (!结果.成功() || !结果.快照
        || 结果.快照->左边界合同组.size() != 1
        || 结果.快照->左边界进展组.size() != 1
        || 结果.快照->左边界准备组.size() != 1)
        return std::nullopt;
    return 结果;
}

bool 历史边界B04左状态负例完整(
    const 服务维护历史边界读取结果_v2& 正常) {
    auto 终态 = 正常;
    终态.快照->左边界合同组.front().当前状态.事实.状态 =
        服务合同当前状态_v1::已完整完成;
    auto 超额 = 正常;
    auto& 超额合同 = 超额.快照->左边界合同组.front();
    超额合同.当前状态.事实.已消费有效秒 =
        超额合同.合同.事实.冻结有效总秒 + 1;
    return !终态.成功() && !超额.成功();
}

bool 历史边界B04自我负例完整(
    const 服务维护历史边界读取结果_v2& 正常) {
    const L2存在身份 异主{稳定编码{0x5342'04FF'FFFF'FFFFULL}};
    auto 合同异主 = 正常;
    合同异主.快照->左边界合同组.front().合同.事实.自我 = 异主;
    auto 进展异主 = 正常;
    进展异主.快照->左边界进展组.front().事实.自我 = 异主;
    auto 准备异主 = 正常;
    准备异主.快照->左边界准备组.front().事实.自我 = 异主;
    return !合同异主.成功() && !进展异主.成功() && !准备异主.成功();
}

int 验证历史边界v2_B04() {
    const auto 正常 = 形成历史边界B04左边界快照();
    if (!正常 || !正常->成功())
        return 失败("B04", "successful left-boundary predicate fixture");
    if (!历史边界B04左状态负例完整(*正常))
        return 失败("B04", "terminal or over-consumed left state");
    if (!历史边界B04自我负例完整(*正常))
        return 失败("B04", "foreign contract, progress or preparation self");
    通过("B04", "copied snapshot rejects terminal, over-consumed and foreign-self payloads");
    return 0;
}

std::optional<服务维护历史边界读取结果_v2>
形成历史边界B05变化快照() {
    auto 运行包 = 建立L1事实基座运行包();
    auto 服务 = 建立服务(运行包);
    const auto Gseed = 当前代次(运行包);
    const auto 自我 = 服务 && Gseed
        ? 服务->ARCH_建立历史到期事件样本_v1(
            0x5342'0500'0000'0001ULL)
        : std::optional<L2存在身份>{};
    const auto G0 = 当前代次(运行包);
    if (!服务 || !Gseed || !自我 || !G0 || *G0 <= *Gseed)
        return std::nullopt;
    auto 结果 = 服务->按事实代次边界读取服务维护历史完整快照_v2(
        历史边界请求_v2(*自我, *Gseed, *G0, 1, 4));
    if (!结果.成功() || !结果.快照
        || 结果.快照->合同变化组.size() != 1
        || 结果.快照->合同状态变化组.size() != 2
        || 结果.快照->到期事件变化组.size() != 1)
        return std::nullopt;
    return 结果;
}

int 验证历史边界v2_B05() {
    const auto 正常 = 形成历史边界B05变化快照();
    if (!正常 || !正常->成功())
        return 失败("B05", "successful state and expiry change fixture");
    auto 未知状态合同 = *正常;
    未知状态合同.快照->合同状态变化组.front().事实.合同 =
        服务合同身份_v1{稳定编码{0x5342'05FE'0000'0001ULL}};
    auto 异主事件合同 = *正常;
    异主事件合同.快照->到期事件变化组.front().事实.合同 =
        服务合同身份_v1{稳定编码{0x5342'05FF'0000'0001ULL}};
    if (未知状态合同.成功() || 异主事件合同.成功())
        return 失败("B05", "unknown or foreign contract reference");
    通过("B05", "copied snapshot rejects unclosed state and expiry contract references");
    return 0;
}

bool 历史边界B06归属完整(
    const 服务维护历史边界读取结果_v2& 结果,
    L2存在身份 自我) {
    if (!结果.成功() || !结果.快照
        || 结果.快照->自我 != 自我
        || 结果.快照->声明左边界成员数 != 0
        || 结果.快照->声明变化成员数 != 4
        || 结果.快照->合同变化组.size() != 1
        || 结果.快照->合同状态变化组.size() != 1
        || 结果.快照->服务进展变化组.size() != 1
        || 结果.快照->服务准备变化组.size() != 1)
        return false;
    const auto 合同 = 结果.快照->合同变化组.front().事实.身份;
    return 结果.快照->合同变化组.front().事实.自我 == 自我
        && 结果.快照->合同状态变化组.front().事实.合同 == 合同
        && 结果.快照->服务进展变化组.front().事实.自我 == 自我
        && 结果.快照->服务准备变化组.front().事实.自我 == 自我;
}

int 验证历史边界v2_B06() {
    auto 运行包 = 建立L1事实基座运行包();
    auto 服务 = 建立服务(运行包);
    const auto Gseed = 当前代次(运行包);
    auto 甲 = 服务 ? 建立发布测试上下文(
        运行包, *服务, 0x5342'0600'0000'0000ULL)
        : std::optional<发布测试上下文>{};
    auto 乙 = 服务 ? 建立发布测试上下文(
        运行包, *服务, 0x5342'1600'0000'0000ULL)
        : std::optional<发布测试上下文>{};
    const auto 发布进展 = [&](发布测试上下文& c, std::uint64_t 键值) {
        const auto G = 当前代次(运行包);
        return G && 服务 && 服务->发布服务进展事实_v2(
            形成进展发布请求(c, *G, 键值, 1)).成功();
    };
    const auto 发布准备 = [&](发布测试上下文& c, std::uint64_t 键值) {
        const auto G = 当前代次(运行包);
        return G && 服务 && 服务->发布服务准备事实_v2(
            形成准备发布请求(c, *G, 键值, 1)).成功();
    };
    const bool 交错发布 = 甲 && 乙
        && 发布进展(*甲, 0x5342'0601'0000'0001ULL)
        && 发布进展(*乙, 0x5342'1611'0000'0001ULL)
        && 发布准备(*甲, 0x5342'0602'0000'0001ULL)
        && 发布准备(*乙, 0x5342'1612'0000'0001ULL);
    const auto G0 = 当前代次(运行包);
    if (!服务 || !Gseed || !甲 || !乙 || !交错发布 || !G0)
        return 失败("B06", "two-self interleaved fixture");
    const auto 甲结果 = 服务->按事实代次边界读取服务维护历史完整快照_v2(
        历史边界请求_v2(甲->自我, *Gseed, *G0, 1, 4));
    const auto 乙结果 = 服务->按事实代次边界读取服务维护历史完整快照_v2(
        历史边界请求_v2(乙->自我, *Gseed, *G0, 1, 4));
    if (!历史边界B06归属完整(甲结果, 甲->自我)
        || !历史边界B06归属完整(乙结果, 乙->自我))
        return 失败("B06", "per-self history isolation");
    if (!历史边界空失败_v2(
            服务->按事实代次边界读取服务维护历史完整快照_v2(
                历史边界请求_v2(甲->自我, *Gseed, *G0, 1, 3)),
            服务维护历史边界读取状态_v2::变化预算不足)
        || !历史边界空失败_v2(
            服务->按事实代次边界读取服务维护历史完整快照_v2(
                历史边界请求_v2(乙->自我, *Gseed, *G0, 1, 3)),
            服务维护历史边界读取状态_v2::变化预算不足))
        return 失败("B06", "budget counts only requested self");
    通过("B06", "interleaved facts remain self-isolated and budgets count only that self");
    return 0;
}

int 验证历史边界v2_B07() {
    auto 运行包 = 建立L1事实基座运行包();
    auto 服务 = 建立服务(运行包);
    const auto Gseed = 当前代次(运行包);
    const auto 自我 = 服务 && Gseed
        ? 服务->ARCH_建立v2验证样本(
            0x5342'0700'0000'0001ULL, 0, 0)
        : std::optional<L2存在身份>{};
    const auto G0 = 当前代次(运行包);
    if (!服务 || !Gseed || !自我 || !G0)
        return 失败("B07", "G0 terminal mismatch fixture");
    const auto 请求 = 历史边界请求_v2(*自我, *Gseed, *G0);
    if (!服务->按事实代次边界读取服务维护历史完整快照_v2(请求).成功())
        return 失败("B07", "legal boundary before terminal mismatch");
    服务->ARCH_注入G0终态错配一次();
    if (!历史边界空失败_v2(
            服务->按事实代次边界读取服务维护历史完整快照_v2(请求),
            服务维护历史边界读取状态_v2::集合不闭合))
        return 失败("B07", "G0 terminal mismatch must be empty failure");
    通过("B07", "provider rejects a replay terminal that differs from G0 current roots");
    return 0;
}

int 验证历史边界v2_B08() {
    if (!服务合同事实权威服务::ARCH_验证历史后继按业务流分组_v2())
        return 失败("B08", "successor closure must be grouped by business flow");
    通过("B08", "same-generation successors of different flows remain legal and a missing same-flow successor fails");
    return 0;
}

int 验证历史边界v2_B09() {
    服务合同事实权威服务::ARCH_延迟下一实例v2历史覆盖登记();
    auto 运行包 = 建立L1事实基座运行包();
    auto 服务 = 建立服务(运行包);
    auto 上下文 = 服务 ? 建立发布测试上下文(
        运行包, *服务, 0x5342'0900'0000'0000ULL)
        : std::optional<发布测试上下文>{};
    const auto G发布 = 当前代次(运行包);
    const auto 进展 = 服务 && 上下文 && G发布
        ? 服务->发布服务进展事实_v2(形成进展发布请求(
            *上下文, *G发布, 0x5342'0901'0000'0001ULL, 1))
        : 服务进展事实发布结果_v2{};
    const auto Gseed = 服务 && 进展.成功()
        ? 服务->ARCH_完成延迟v2历史覆盖登记()
        : std::optional<std::uint64_t>{};
    if (!服务 || !上下文 || !进展.成功() || !进展.事实 || !Gseed)
        return 失败("B09", "seed progress registration fixture");
    if (!服务->ARCH_退出历史成员关系_v1(
            服务维护历史事实类别_v1::服务进展,
            进展.事实->身份.值, 进展.事实->形成事实代次, true))
        return 失败("B09", "exit v2 permanent seed progress relation");
    服务.reset();
    bool 重建已拒绝 = false;
    try {
        auto 重建 = 建立服务(运行包);
        重建已拒绝 = !重建;
    } catch (...) {
        重建已拒绝 = true;
    }
    if (!重建已拒绝)
        return 失败("B09", "service reconstruction accepted a missing seed relation");
    通过("B09", "service reconstruction rejects a missing v2 permanent seed relation");
    return 0;
}

bool 历史边界B10正式坏种子登记拒绝(
    std::uint64_t 身份基数, bool 准备, std::uint8_t 模式) {
    服务合同事实权威服务::ARCH_延迟下一实例v2历史覆盖登记();
    auto 运行包 = 建立L1事实基座运行包();
    auto 服务 = 建立服务(运行包);
    auto 上下文 = 服务 ? 建立发布测试上下文(运行包, *服务, 身份基数)
        : std::optional<发布测试上下文>{};
    const auto G0 = 当前代次(运行包);
    if (!服务 || !上下文 || !G0) return false;
    std::optional<稳定编码> 事实;
    if (准备) {
        const auto 发布 = 服务->发布服务准备事实_v2(形成准备发布请求(
            *上下文, *G0, 身份基数 + 0x100, 1,
            服务准备运行状态_v1::进行中, true, true, 模式 != 3));
        if (发布.成功() && 发布.事实) 事实 = 发布.事实->身份.值;
    } else {
        const auto 发布 = 服务->发布服务进展事实_v2(形成进展发布请求(
            *上下文, *G0, 身份基数 + 0x100, 1,
            服务进展运行状态_v1::进行中, true, 模式 != 3));
        if (发布.成功() && 发布.事实) 事实 = 发布.事实->身份.值;
    }
    if (!事实) return false;
    const auto 替代目标 = 模式 == 1 ? 上下文->外部端点.节点[1]
        : 模式 == 3 ? 上下文->外部端点.节点[6] : 稳定编码{};
    if (!服务->ARCH_损坏当前v2事实引用关系(
            准备, *事实, 模式, 替代目标))
        return false;
    return !服务->ARCH_完成延迟v2历史覆盖登记();
}

bool 历史边界B10正式正常种子登记成功() {
    服务合同事实权威服务::ARCH_延迟下一实例v2历史覆盖登记();
    auto 运行包 = 建立L1事实基座运行包();
    auto 服务 = 建立服务(运行包);
    auto 上下文 = 服务 ? 建立发布测试上下文(
        运行包, *服务, 0x5342'10F0'0000'0000ULL)
        : std::optional<发布测试上下文>{};
    const auto G进展 = 当前代次(运行包);
    const auto 进展 = 服务 && 上下文 && G进展
        ? 服务->发布服务进展事实_v2(形成进展发布请求(
            *上下文, *G进展, 0x5342'10F1'0000'0001ULL, 1))
        : 服务进展事实发布结果_v2{};
    const auto G准备 = 当前代次(运行包);
    const auto 准备 = 服务 && 上下文 && G准备
        ? 服务->发布服务准备事实_v2(形成准备发布请求(
            *上下文, *G准备, 0x5342'10F2'0000'0001ULL, 1))
        : 服务准备事实发布结果_v2{};
    return 服务 && 上下文 && 进展.成功() && 准备.成功()
        && 服务->ARCH_完成延迟v2历史覆盖登记().has_value();
}

int 验证历史边界v2_B10() {
    if (!历史边界B10正式坏种子登记拒绝(
            0x5342'1000'0000'0000ULL, false, 1))
        return 失败("B10", "progress required selection relation wrong target");
    if (!历史边界B10正式坏种子登记拒绝(
            0x5342'1010'0000'0000ULL, false, 2))
        return 失败("B10", "progress optional state relation missing");
    if (!历史边界B10正式坏种子登记拒绝(
            0x5342'1020'0000'0000ULL, false, 3))
        return 失败("B10", "progress empty optional dynamic has relation");
    if (!历史边界B10正式坏种子登记拒绝(
            0x5342'1030'0000'0000ULL, true, 1))
        return 失败("B10", "preparation required selection relation wrong target");
    if (!历史边界B10正式坏种子登记拒绝(
            0x5342'1040'0000'0000ULL, true, 2))
        return 失败("B10", "preparation optional state relation missing");
    if (!历史边界B10正式坏种子登记拒绝(
            0x5342'1050'0000'0000ULL, true, 3))
        return 失败("B10", "preparation empty optional dynamic has relation");
    if (!历史边界B10正式正常种子登记成功())
        return 失败("B10", "normal progress and preparation seed control");
    通过("B10", "seed registration validates required and optional progress/preparation references");
    return 0;
}
#endif

} // namespace 海中鱼巣::服务合同事实权威测试内部

namespace 海中鱼巣 {

int 运行服务合同事实权威端到端测试() noexcept {
    using namespace 服务合同事实权威测试内部;
#if !defined(ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION)
    std::cout << "[INSTINCT-SERVICE-CONTRACT] validation macro not enabled\n";
    return 2;
#else
    try {
        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            if (!服务) return 失败("P00", "owner and registration");
            auto 坏请求 = 合同请求(L2存在身份{稳定编码{1}}, 1);
            坏请求.合同版本 = 0;
            if (!合同空失败(服务->读取当前有效未满足服务合同完整集合_v1(坏请求),
                    服务合同事实权威读取状态_v1::入口拒绝))
                return 失败("P00", "invalid request shape");
            通过("P00", "invalid request is exact empty failure");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            if (!服务) return 失败("P01", "build service");
            const auto 自我 = 服务->ARCH_建立验证样本_v1(
                0x5343'0000'0000'0001ULL, 0, 0);
            const auto G0 = 当前代次(运行包);
            if (!自我 || !G0) return 失败("P01", "empty fixture");
            const auto 合同 = 服务->读取当前有效未满足服务合同完整集合_v1(
                合同请求(*自我, *G0));
            const auto 事件 = 服务->读取到期未满足事件完整集合_v1(
                事件请求(*自我, *G0));
            if (!合同.成功() || !事件.成功() || !合同.完整集合见证
                || !事件.完整集合见证 || 合同.完整集合见证->声明成员数 != 0
                || 事件.完整集合见证->声明成员数 != 0
                || 合同.本次正式读回截止 != *G0
                || 事件.本次正式读回截止 != *G0)
                return 失败("P01", "legal empty witness");
            通过("P01", "zero members is proven by registered owner closure");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'0010ULL, 1, 1)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0) return 失败("P02", "single fixture");
            const auto 合同 = 服务->读取当前有效未满足服务合同完整集合_v1(
                合同请求(*自我, *G0));
            const auto 事件 = 服务->读取到期未满足事件完整集合_v1(
                事件请求(*自我, *G0));
            if (!合同.成功() || !事件.成功()
                || 合同.完整合同事实组.size() != 1
                || 合同.当前状态事实组.size() != 1
                || 事件.完整事件事实组.size() != 1)
                return 失败("P02", "single contract and event readback");
            通过("P02", "one contract and one event close at the same G0");
        }

        服务合同完整集合读取结果_v1 N合同期望;
        服务到期事件完整集合读取结果_v1 N事件期望;
        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'0020ULL, 3, 2)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0) return 失败("P03", "N fixture");
            N合同期望 = 服务->读取当前有效未满足服务合同完整集合_v1(
                合同请求(*自我, *G0));
            N事件期望 = 服务->读取到期未满足事件完整集合_v1(
                事件请求(*自我, *G0));
            if (!N合同期望.成功() || !N事件期望.成功()
                || N合同期望.完整合同事实组.size() != 3
                || N事件期望.完整事件事实组.size() != 2)
                return 失败("P03", "N complete readback");
            通过("P03", "N members are complete and canonically ordered");

            const auto 重复自我 = 服务->ARCH_建立验证样本_v1(
                0x5343'0000'0000'0020ULL, 3, 2);
            const auto G重复 = 当前代次(运行包);
            if (!重复自我 || !G重复 || *重复自我 != *自我 || *G重复 != *G0)
                return 失败("P04", "fixture exact duplicate");
            const auto 重复合同 = 服务->读取当前有效未满足服务合同完整集合_v1(
                合同请求(*自我, *G重复));
            const auto 重复事件 = 服务->读取到期未满足事件完整集合_v1(
                事件请求(*自我, *G重复));
            if (重复合同 != N合同期望 || 重复事件 != N事件期望)
                return 失败("P04", "duplicate readback equality");
            通过("P04", "exact duplicate has zero second authoritative effect");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'0030ULL, 1, 0, true, false)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0) return 失败("P05", "terminal fixture");
            if (!合同空失败(服务->读取当前有效未满足服务合同完整集合_v1(
                    合同请求(*自我, *G0)),
                    服务合同事实权威读取状态_v1::集合不闭合))
                return 失败("P05", "terminal member interference");
            通过("P05", "terminal contract cannot remain in current index");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'0040ULL, 1, 0, false, true)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0) return 失败("P06", "damaged fixture");
            const auto 结果 = 服务->读取当前有效未满足服务合同完整集合_v1(
                合同请求(*自我, *G0));
            if (!合同空失败(结果, 服务合同事实权威读取状态_v1::引用冲突))
                return 失败("P06", "damaged payload fail closed");
            通过("P06", "damaged payload never becomes a partial success");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'0050ULL, 1, 1)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0) return 失败("P07", "drift fixture");
            服务->ARCH_注入读中漂移一次();
            if (!合同空失败(服务->读取当前有效未满足服务合同完整集合_v1(
                    合同请求(*自我, *G0)),
                    服务合同事实权威读取状态_v1::当前性漂移))
                return 失败("P07", "read drift");
            通过("P07", "G0 drift returns exact empty failure");

            const auto G1 = 当前代次(运行包);
            if (!G1) return 失败("P08", "resource G");
            服务->ARCH_注入资源失败一次();
            if (!事件空失败(服务->读取到期未满足事件完整集合_v1(
                    事件请求(*自我, *G1)),
                    服务合同事实权威读取状态_v1::资源失败))
                return 失败("P08", "resource failure");
            通过("P08", "resource failure has no stale payload");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'0060ULL, 2, 1)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0) return 失败("P09", "reconstruct fixture");
            const auto 期望 = 服务->读取当前有效未满足服务合同完整集合_v1(
                合同请求(*自我, *G0));
            服务.reset();
            auto 重构服务 = 建立服务(运行包);
            const auto G1 = 当前代次(运行包);
            if (!重构服务 || !G1 || *G1 != *G0
                || 重构服务->读取当前有效未满足服务合同完整集合_v1(
                    合同请求(*自我, *G1)) != 期望)
                return 失败("P09", "same-process reconstruction");
            通过("P09", "registration and facts survive service reconstruction");
        }

        {
            const auto 根 = std::filesystem::path{L"D:\\TEMP"} / L"海中鱼巣"
                / L"INSTINCT-STAGE3-SERVICE-CONTRACT-FACT-OWNER"
                / (std::to_wstring(GetCurrentProcessId()) + L"-"
                    + std::to_wstring(GetTickCount64()));
            隔离根清理 清理{根};
            std::error_code 错误;
            std::filesystem::create_directories(根, 错误);
            if (错误) return 失败("P10", "create persistent root");
            const L1事实基座持久存储配置_v1 配置{
                L1事实基座持久恢复合同版本_v1, 根};
            L2存在身份 自我{};
            服务合同完整集合读取结果_v1 合同期望;
            服务到期事件完整集合读取结果_v1 事件期望;
            服务到期事件合同终态完整集合读取结果_v1 事件终态期望;
            服务进展完整集合读取结果_v1 进展期望;
            服务准备完整集合读取结果_v1 准备期望;
            {
                auto 建立 = 建立L1事实基座持久运行包_v1(配置);
                if (!建立.成功() || !建立.运行包)
                    return 失败("P10", "first persistent package");
                auto 运行包 = std::move(*建立.运行包);
                auto 服务 = 建立服务(运行包);
                const auto 自我结果 = 服务
                    ? 服务->ARCH_建立验证样本_v1(
                        0x5343'0000'0000'0070ULL, 2, 2, false, false, 2,
                        false, 2, false, false, false, true)
                    : std::optional<L2存在身份>{};
                const auto G0 = 当前代次(运行包);
                if (!服务 || !自我结果 || !G0)
                    return 失败("P10", "persistent fixture");
                自我 = *自我结果;
                合同期望 = 服务->读取当前有效未满足服务合同完整集合_v1(
                    合同请求(自我, *G0));
                事件期望 = 服务->读取到期未满足事件完整集合_v1(
                    事件请求(自我, *G0));
                事件终态期望 =
                    服务->读取到期未满足事件合同终态完整集合_v1(
                        事件请求(自我, *G0));
                进展期望 = 服务->读取当前服务合同关联进展完整集合_v1(
                    进展请求(自我, *G0));
                准备期望 = 服务->读取当前服务准备完整集合_v1(
                    准备请求(自我, *G0));
                if (!合同期望.成功() || !事件期望.成功()
                    || !事件终态期望.成功()
                    || !进展期望.成功() || !准备期望.成功())
                    return 失败("P10", "persistent first readback");
            }
            {
                auto 恢复 = 建立L1事实基座持久运行包_v1(配置);
                if (!恢复.成功() || !恢复.运行包)
                    return 失败("P10", "recovery package");
                auto 运行包 = std::move(*恢复.运行包);
                auto 服务 = 建立服务(运行包);
                const auto G0 = 当前代次(运行包);
                if (!服务 || !G0
                    || 服务->读取当前有效未满足服务合同完整集合_v1(
                        合同请求(自我, *G0)) != 合同期望
                    || 服务->读取到期未满足事件完整集合_v1(
                        事件请求(自我, *G0)) != 事件期望
                    || 服务->读取到期未满足事件合同终态完整集合_v1(
                        事件请求(自我, *G0)) != 事件终态期望
                    || 服务->读取当前服务合同关联进展完整集合_v1(
                        进展请求(自我, *G0)) != 进展期望
                    || 服务->读取当前服务准备完整集合_v1(
                        准备请求(自我, *G0)) != 准备期望)
                    return 失败("P10", "recovery exact readback");
            }
            通过("P10", "persistent recovery preserves all five complete collections");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'0080ULL, 0, 0)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0) return 失败("P11", "empty progress fixture");
            auto 坏请求 = 进展请求(*自我, *G0);
            坏请求.合同版本 = 0;
            if (!进展空失败(服务->读取当前服务合同关联进展完整集合_v1(坏请求),
                    服务合同事实权威读取状态_v1::入口拒绝))
                return 失败("P11", "invalid progress request");
            const auto 空组 = 服务->读取当前服务合同关联进展完整集合_v1(
                进展请求(*自我, *G0));
            if (!空组.成功() || !空组.完整集合见证
                || 空组.完整集合见证->声明成员数 != 0
                || 空组.本次正式读回截止 != *G0)
                return 失败("P11", "legal empty progress witness");
            auto 单项运行包 = 建立L1事实基座运行包();
            auto 单项服务 = 建立服务(单项运行包);
            const auto 单项自我 = 单项服务
                ? 单项服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'0081ULL, 1, 0, false, false, 1)
                : std::optional<L2存在身份>{};
            const auto 单项G0 = 当前代次(单项运行包);
            if (!单项服务 || !单项自我 || !单项G0)
                return 失败("P11", "single progress fixture");
            const auto 单项组 = 单项服务->读取当前服务合同关联进展完整集合_v1(
                进展请求(*单项自我, *单项G0));
            if (!单项组.成功() || 单项组.完整进展事实组.size() != 1
                || !单项组.完整集合见证
                || 单项组.完整集合见证->声明成员数 != 1)
                return 失败("P11", "single progress readback");
            通过("P11", "zero and one progress sets have complete witnesses");
        }

        服务进展完整集合读取结果_v1 N进展期望;
        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'0090ULL, 8, 0, false, false, 8)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0) return 失败("P12", "N progress fixture");
            N进展期望 = 服务->读取当前服务合同关联进展完整集合_v1(
                进展请求(*自我, *G0));
            if (!N进展期望.成功() || N进展期望.完整进展事实组.size() != 8)
                return 失败("P12", "N progress complete readback");
            bool 状态组[8]{};
            for (std::size_t i = 0; i < N进展期望.完整进展事实组.size(); ++i) {
                const auto& 进展 = N进展期望.完整进展事实组[i];
                const auto 状态 = static_cast<std::uint8_t>(进展.运行状态);
                if (状态 < 1 || 状态 > 8) return 失败("P12", "progress state range");
                状态组[状态 - 1] = true;
                const bool 应有状态 = i % 3 != 1;
                const bool 应有动态 = i % 3 != 0;
                if (进展.进展状态.has_value() != 应有状态
                    || 进展.进展动态.has_value() != 应有动态)
                    return 失败("P12", "state and dynamic optional shapes");
            }
            for (const auto 已见 : 状态组)
                if (!已见) return 失败("P12", "all progress states retained");
            const auto 重复自我 = 服务->ARCH_建立验证样本_v1(
                0x5343'0000'0000'0090ULL, 8, 0, false, false, 8);
            const auto G重复 = 当前代次(运行包);
            if (!重复自我 || !G重复 || *重复自我 != *自我 || *G重复 != *G0
                || 服务->读取当前服务合同关联进展完整集合_v1(
                    进展请求(*自我, *G重复)) != N进展期望)
                return 失败("P13", "progress exact duplicate");
            通过("P12", "all eight current run states remain in the complete set");
            通过("P13", "progress fixture exact duplicate has zero second effect");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'00A0ULL, 1, 0, false, false, 1, true)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0) return 失败("P14", "damaged progress fixture");
            if (!进展空失败(服务->读取当前服务合同关联进展完整集合_v1(
                    进展请求(*自我, *G0)),
                    服务合同事实权威读取状态_v1::引用冲突))
                return 失败("P14", "damaged progress payload fail closed");
            通过("P14", "damaged progress cannot become partial success");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'00B0ULL, 1, 0, false, false, 1)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0) return 失败("P15", "progress drift fixture");
            服务->ARCH_注入读中漂移一次();
            if (!进展空失败(服务->读取当前服务合同关联进展完整集合_v1(
                    进展请求(*自我, *G0)),
                    服务合同事实权威读取状态_v1::当前性漂移))
                return 失败("P15", "progress read drift");
            const auto G1 = 当前代次(运行包);
            if (!G1) return 失败("P16", "progress resource G");
            服务->ARCH_注入资源失败一次();
            if (!进展空失败(服务->读取当前服务合同关联进展完整集合_v1(
                    进展请求(*自我, *G1)),
                    服务合同事实权威读取状态_v1::资源失败))
                return 失败("P16", "progress resource failure");
            通过("P15", "progress G0 drift fails with empty payload");
            通过("P16", "progress resource failure has no stale payload");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'00C0ULL, 0, 0)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0) return 失败("P17", "empty preparation fixture");
            auto 坏请求 = 准备请求(*自我, *G0);
            坏请求.合同版本 = 0;
            if (!准备空失败(服务->读取当前服务准备完整集合_v1(坏请求),
                    服务合同事实权威读取状态_v1::入口拒绝))
                return 失败("P17", "invalid preparation request");
            const auto 空组 = 服务->读取当前服务准备完整集合_v1(
                准备请求(*自我, *G0));
            if (!空组.成功() || !空组.完整集合见证
                || 空组.完整集合见证->声明成员数 != 0
                || 空组.本次正式读回截止 != *G0)
                return 失败("P17", "legal empty preparation witness");
            通过("P17", "zero preparations are proven by the three registrations");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'00D0ULL, 0, 0, false, false,
                    0, false, 1)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0) return 失败("P18", "single preparation fixture");
            const auto 单项 = 服务->读取当前服务准备完整集合_v1(
                准备请求(*自我, *G0));
            if (!单项.成功() || 单项.完整准备事实组.size() != 1
                || !std::holds_alternative<L2需求身份>(
                    单项.完整准备事实组.front().来源)
                || !单项.完整准备事实组.front().T到D关系稳定编码)
                return 失败("P18", "single demand preparation readback");
            通过("P18", "one demand preparation retains its T-to-D binding");
        }

        服务准备完整集合读取结果_v1 N准备期望;
        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'00E0ULL, 0, 0, false, false,
                    0, false, 9)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0) return 失败("P19", "N preparation fixture");
            N准备期望 = 服务->读取当前服务准备完整集合_v1(
                准备请求(*自我, *G0));
            if (!N准备期望.成功() || N准备期望.完整准备事实组.size() != 9)
                return 失败("P19", "N preparation complete readback");
            bool 状态组[9]{};
            bool 已见需求来源 = false;
            bool 已见能力缺口来源 = false;
            for (const auto& 准备 : N准备期望.完整准备事实组) {
                const auto 状态 = static_cast<std::uint8_t>(准备.运行状态);
                if (状态 < 1 || 状态 > 9) return 失败("P19", "preparation state range");
                状态组[状态 - 1] = true;
                if (std::holds_alternative<L2需求身份>(准备.来源)) {
                    已见需求来源 = true;
                    if (!准备.T到D关系稳定编码)
                        return 失败("P19", "demand source without T-to-D");
                } else {
                    已见能力缺口来源 = true;
                    if (准备.T到D关系稳定编码)
                        return 失败("P19", "gap source forged T-to-D");
                }
                const bool 已完成 = 准备.运行状态
                    == 服务准备运行状态_v1::已完整完成;
                if (准备.准备结果.has_value() != 已完成
                    || 准备.完成验证.has_value() != 已完成)
                    return 失败("P19", "completion evidence shape");
            }
            for (const auto 已见 : 状态组)
                if (!已见) return 失败("P19", "all preparation states retained");
            if (!已见需求来源 || !已见能力缺口来源)
                return 失败("P19", "both preparation source variants retained");
            const auto 重复自我 = 服务->ARCH_建立验证样本_v1(
                0x5343'0000'0000'00E0ULL, 0, 0, false, false,
                0, false, 9);
            const auto G重复 = 当前代次(运行包);
            if (!重复自我 || !G重复 || *重复自我 != *自我 || *G重复 != *G0
                || 服务->读取当前服务准备完整集合_v1(
                    准备请求(*自我, *G重复)) != N准备期望)
                return 失败("P19", "preparation exact duplicate");
            通过("P19", "nine states and both source variants are complete and replay-stable");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'00F0ULL, 0, 0, false, false,
                    0, false, 1, true)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0
                || !准备空失败(服务->读取当前服务准备完整集合_v1(
                    准备请求(*自我, *G0)),
                    服务合同事实权威读取状态_v1::引用冲突))
                return 失败("P20", "damaged preparation payload");
            通过("P20", "damaged preparation payload fails closed");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'0100ULL, 0, 0, false, false,
                    0, false, 1, false, true)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0
                || !准备空失败(服务->读取当前服务准备完整集合_v1(
                    准备请求(*自我, *G0)),
                    服务合同事实权威读取状态_v1::引用冲突))
                return 失败("P21", "invalid preparation source shape");
            通过("P21", "invalid demand source shape fails closed");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'0110ULL, 0, 0, false, false,
                    0, false, 1, false, false, true)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0
                || !准备空失败(服务->读取当前服务准备完整集合_v1(
                    准备请求(*自我, *G0)),
                    服务合同事实权威读取状态_v1::引用冲突))
                return 失败("P22", "invalid preparation relation");
            通过("P22", "source relation mismatch fails closed");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'0120ULL, 0, 0, false, false,
                    0, false, 1)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0) return 失败("P23", "preparation drift fixture");
            服务->ARCH_注入读中漂移一次();
            if (!准备空失败(服务->读取当前服务准备完整集合_v1(
                    准备请求(*自我, *G0)),
                    服务合同事实权威读取状态_v1::当前性漂移))
                return 失败("P23", "preparation read drift");
            const auto G1 = 当前代次(运行包);
            if (!G1) return 失败("P24", "preparation resource G");
            服务->ARCH_注入资源失败一次();
            if (!准备空失败(服务->读取当前服务准备完整集合_v1(
                    准备请求(*自我, *G1)),
                    服务合同事实权威读取状态_v1::资源失败))
                return 失败("P24", "preparation resource failure");
            通过("P23", "preparation G0 drift fails with empty payload");
            通过("P24", "preparation resource failure has no stale payload");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务 ? 服务->ARCH_建立v2验证样本(
                0x5343'0000'0000'1000ULL, 0, 0) : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0) return 失败("V00", "v2 empty fixture");
            auto 坏 = 进展请求_v2(*自我, *G0); 坏.合同版本 = 1;
            if (!进展空失败_v2(
                    服务->读取当前服务合同关联进展完整集合_v2(坏),
                    服务合同事实权威读取状态_v2::入口拒绝))
                return 失败("V00", "v2 invalid request");
            const auto p = 服务->读取当前服务合同关联进展完整集合_v2(
                进展请求_v2(*自我, *G0));
            const auto r = 服务->读取当前服务准备完整集合_v2(
                准备请求_v2(*自我, *G0));
            if (!p.成功() || !r.成功() || !p.完整集合见证 || !r.完整集合见证
                || p.完整集合见证->声明成员数 != 0
                || r.完整集合见证->声明成员数 != 0)
                return 失败("V00", "v2 legal zero-member witnesses");
            通过("V00", "v2 progress and preparation registrations prove legal empty sets");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务 ? 服务->ARCH_建立v2验证样本(
                0x5343'0000'0000'1010ULL, 8, 9) : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0) return 失败("V01", "v2 N fixture");
            const auto p = 服务->读取当前服务合同关联进展完整集合_v2(
                进展请求_v2(*自我, *G0));
            const auto r = 服务->读取当前服务准备完整集合_v2(
                准备请求_v2(*自我, *G0));
            if (!p.成功() || !r.成功() || p.完整进展事实组.size() != 8
                || r.完整准备事实组.size() != 9)
                return 失败("V01", "v2 N complete readback");
            for (const auto& f : p.完整进展事实组)
                if (!服务活动任务执行绑定完整_v2(f.执行绑定))
                    return 失败("V01", "v2 progress typed binding");
            for (const auto& f : r.完整准备事实组)
                if (!服务活动任务执行绑定完整_v2(f.执行绑定))
                    return 失败("V01", "v2 preparation typed binding");
            const auto 重放 = 服务->ARCH_建立v2验证样本(
                0x5343'0000'0000'1010ULL, 8, 9);
            const auto G重放 = 当前代次(运行包);
            if (!重放 || !G重放 || *G重放 != *G0
                || 服务->读取当前服务合同关联进展完整集合_v2(
                    进展请求_v2(*自我, *G重放)) != p
                || 服务->读取当前服务准备完整集合_v2(
                    准备请求_v2(*自我, *G重放)) != r)
                return 失败("V01", "v2 exact replay");
            通过("V01", "v2 N members are canonical, typed and replay-stable");
        }

        {
            const auto 验证损坏 = [](std::uint64_t id, bool 绑定, bool 关系,
                bool 载荷, bool 准备) {
                auto 运行包 = 建立L1事实基座运行包();
                auto 服务 = 建立服务(运行包);
                const auto 自我 = 服务 ? 服务->ARCH_建立v2验证样本(
                    id, 准备 ? 0 : 1, 准备 ? 1 : 0, 绑定, 关系, 载荷)
                    : std::optional<L2存在身份>{};
                const auto G0 = 当前代次(运行包);
                if (!服务 || !自我 || !G0) return false;
                return 准备
                    ? 准备空失败_v2(服务->读取当前服务准备完整集合_v2(
                        准备请求_v2(*自我, *G0)),
                        服务合同事实权威读取状态_v2::引用冲突)
                    : 进展空失败_v2(
                        服务->读取当前服务合同关联进展完整集合_v2(
                            进展请求_v2(*自我, *G0)),
                        服务合同事实权威读取状态_v2::引用冲突);
            };
            if (!验证损坏(0x5343'0000'0000'1020ULL, true, false, false, false)
                || !验证损坏(0x5343'0000'0000'1021ULL, false, true, false, false)
                || !验证损坏(0x5343'0000'0000'1022ULL, false, false, true, false)
                || !验证损坏(0x5343'0000'0000'1023ULL, true, false, false, true)
                || !验证损坏(0x5343'0000'0000'1024ULL, false, true, false, true)
                || !验证损坏(0x5343'0000'0000'1025ULL, false, false, true, true))
                return 失败("V02", "v2 corruption matrix");
            通过("V02", "invalid bindings, payloads and relation endpoints fail closed");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务 ? 服务->ARCH_建立v2验证样本(
                0x5343'0000'0000'1026ULL, 1, 0,
                false, false, false, true) : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0
                || !进展空失败_v2(
                    服务->读取当前服务合同关联进展完整集合_v2(
                        进展请求_v2(*自我, *G0)),
                    服务合同事实权威读取状态_v2::集合不闭合))
                return 失败("V02A", "v2 duplicate member closure");
            通过("V02A", "duplicate v2 members fail as an unclosed collection");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务 ? 服务->ARCH_建立v2验证样本(
                0x5343'0000'0000'1027ULL, 0, 0) : std::optional<L2存在身份>{};
            if (!服务 || !自我 || !服务->ARCH_损坏v2结构登记(false))
                return 失败("V02B", "v2 registration corruption fixture");
            const auto G0 = 当前代次(运行包);
            if (!G0 || !进展空失败_v2(
                    服务->读取当前服务合同关联进展完整集合_v2(
                        进展请求_v2(*自我, *G0)),
                    服务合同事实权威读取状态_v2::结构未登记))
                return 失败("V02B", "v2 registration version mismatch");
            通过("V02B", "v2 registration version mismatch fails closed");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务 ? 服务->ARCH_建立验证样本_v1(
                0x5343'0000'0000'1030ULL, 1, 0, false, false,
                1, false, 1) : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0) return 失败("V03", "v1 isolation fixture");
            const auto p = 服务->读取当前服务合同关联进展完整集合_v2(
                进展请求_v2(*自我, *G0));
            const auto r = 服务->读取当前服务准备完整集合_v2(
                准备请求_v2(*自我, *G0));
            if (!p.成功() || !r.成功() || !p.完整进展事实组.empty()
                || !r.完整准备事实组.empty())
                return 失败("V03", "v1 leaked into v2");
            服务->ARCH_注入读中漂移一次();
            if (!进展空失败_v2(
                    服务->读取当前服务合同关联进展完整集合_v2(
                        进展请求_v2(*自我, *G0)),
                    服务合同事实权威读取状态_v2::当前性漂移))
                return 失败("V04", "v2 drift");
            const auto G1 = 当前代次(运行包);
            if (!G1) return 失败("V04", "v2 resource cutoff");
            服务->ARCH_注入资源失败一次();
            if (!准备空失败_v2(服务->读取当前服务准备完整集合_v2(
                    准备请求_v2(*自我, *G1)),
                    服务合同事实权威读取状态_v2::资源失败))
                return 失败("V04", "v2 resource failure");
            通过("V03", "v1 facts do not project or fall back into v2");
            通过("V04", "v2 drift and resource failures expose no stale payload");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto Greg = 当前代次(运行包);
            const auto 自我 = 服务 && Greg
                ? 服务->ARCH_建立验证样本_v1(
                    0x5348'0000'0000'00F0ULL, 1, 1)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !Greg || *Greg <= 1 || !自我 || !G0)
                return 失败("H-1", "history coverage-point fixture");
            const auto 成功 = 服务->读取服务维护历史事实账覆盖起点_v1({
                服务维护历史覆盖起点合同版本_v1,
                {L2结构合同版本, *G0}, *自我});
            if (!成功.成功() || !成功.快照
                || 成功.快照->结构登记版本
                    != 服务维护历史事实账结构登记版本_v1
                || 成功.快照->历史账登记事实代次 == 0
                || 成功.快照->历史账登记事实代次 > *Greg
                || 成功.快照->自我 != *自我 || 成功.快照->G0 != *G0
                || 成功.本次正式读回截止 != *G0)
                return 失败("H-1", "history coverage-point readback");
            if (!历史覆盖起点空失败(
                    服务->读取服务维护历史事实账覆盖起点_v1({
                        0, {L2结构合同版本, *G0}, *自我}),
                    服务维护历史覆盖起点读取状态_v1::入口拒绝)
                || !历史覆盖起点空失败(
                    服务->读取服务维护历史事实账覆盖起点_v1({
                        服务维护历史覆盖起点合同版本_v1,
                        {L2结构合同版本, *G0},
                        L2存在身份{稳定编码{0}}}),
                    服务维护历史覆盖起点读取状态_v1::入口拒绝)
                || !历史覆盖起点空失败(
                    服务->读取服务维护历史事实账覆盖起点_v1({
                        服务维护历史覆盖起点合同版本_v1,
                        {L2结构合同版本, *Greg - 1}, *自我}),
                    服务维护历史覆盖起点读取状态_v1::当前性漂移))
                return 失败("H-1", "coverage-point entry and old-G matrix");
            服务->ARCH_注入读中漂移一次();
            if (!历史覆盖起点空失败(
                    服务->读取服务维护历史事实账覆盖起点_v1({
                        服务维护历史覆盖起点合同版本_v1,
                        {L2结构合同版本, *G0}, *自我}),
                    服务维护历史覆盖起点读取状态_v1::当前性漂移))
                return 失败("H-1", "coverage-point read drift");
            const auto G1 = 当前代次(运行包);
            if (!G1) return 失败("H-1", "coverage-point resource G");
            服务->ARCH_注入资源失败一次();
            if (!历史覆盖起点空失败(
                    服务->读取服务维护历史事实账覆盖起点_v1({
                        服务维护历史覆盖起点合同版本_v1,
                        {L2结构合同版本, *G1}, *自我}),
                    服务维护历史覆盖起点读取状态_v1::资源失败))
                return 失败("H-1", "coverage-point resource failure");
            通过("H-1", "history owner exposes a guarded registration cutoff");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto Greg = 当前代次(运行包);
            const auto 自我 = 服务 && Greg
                ? 服务->ARCH_建立历史到期事件样本_v1(
                    0x5348'0000'0000'0001ULL, false)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !Greg || *Greg <= 1 || !自我 || !G0)
                return 失败("H00", "history contract fixture");
            const auto 完整 = 服务->按事实代次范围读取服务维护历史事实完整组_v1(
                历史范围请求(*自我, *Greg, *G0));
            if (!完整.成功() || 完整.合同变化组.size() != 1
                || 完整.合同状态变化组.size() != 1
                || !完整.到期事件变化组.empty()
                || !完整.服务进展变化组.empty()
                || !完整.服务准备变化组.empty()
                || !完整.完整集合见证
                || 完整.完整集合见证->声明成员数 != 2
                || 完整.完整集合见证->历史账登记事实代次 <= 1
                || 完整.完整集合见证->历史账登记事实代次 > *Greg)
                return 失败("H00", "contract, state and expiry history range");
            if (!历史范围空失败(
                    服务->按事实代次范围读取服务维护历史事实完整组_v1(
                        历史范围请求(*自我,
                            完整.完整集合见证->历史账登记事实代次 - 1, *G0)),
                    服务维护历史事实范围读取状态_v1::覆盖边界不可用))
                return 失败("H00", "history coverage boundary");
            if (!历史范围空失败(
                    服务->按事实代次范围读取服务维护历史事实完整组_v1(
                        历史范围请求(*自我, *Greg, *G0, 1)),
                    服务维护历史事实范围读取状态_v1::数量预算不足))
                return 失败("H00", "history count budget");
            const auto 空组 = 服务->按事实代次范围读取服务维护历史事实完整组_v1(
                历史范围请求(*自我, *G0, *G0));
            if (!空组.成功() || !空组.完整集合见证
                || 空组.完整集合见证->声明成员数 != 0)
                return 失败("H00", "legal empty history range");
            通过("H00", "registration boundary, contract/state range, budget and empty witness");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto Greg = 当前代次(运行包);
            const auto 自我 = 服务 && Greg
                ? 服务->ARCH_建立历史到期事件样本_v1(
                    0x5348'0100'0000'0001ULL)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !Greg || !自我 || !G0 || *G0 <= *Greg)
                return 失败("H01", "expiry history fixture");
            const auto 完整 = 服务->按事实代次范围读取服务维护历史事实完整组_v1(
                历史范围请求(*自我, *Greg, *G0));
            if (!完整.成功() || !完整.完整集合见证
                || 完整.合同变化组.size() != 1
                || 完整.合同状态变化组.size() != 2
                || 完整.到期事件变化组.size() != 1
                || !完整.服务进展变化组.empty()
                || !完整.服务准备变化组.empty()
                || 完整.完整集合见证->声明成员数 != 4)
                return 失败("H01", "five-root expiry history shape");
            const auto& 合同 = 完整.合同变化组.front();
            const auto& 初态 = 完整.合同状态变化组.front();
            const auto& 终态 = 完整.合同状态变化组.back();
            const auto& 事件 = 完整.到期事件变化组.front();
            if (!合同.退出当前事实代次
                || 初态.退出当前事实代次 != 合同.退出当前事实代次
                || 终态.退出当前事实代次
                || 事件.退出当前事实代次
                || 初态.事实.状态 != 服务合同当前状态_v1::有效未满足
                || 终态.事实.状态 != 服务合同当前状态_v1::已到期
                || 事件.事实.合同 != 合同.事实.身份
                || 事件.事实.合同终态版本 != 终态.事实.状态版本
                || *合同.退出当前事实代次 != 终态.事实.形成事实代次
                || 完整.完整集合见证->规范成员身份组.size() != 4)
                return 失败("H01", "expiry current-exit boundary and references");
            通过("H01", "contract, two states and expiry event form permanent history");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto Greg = 当前代次(运行包);
            auto 上下文 = 服务 && Greg ? 建立发布测试上下文(
                运行包, *服务, 0x5348'0200'0000'0000ULL)
                : std::optional<发布测试上下文>{};
            bool 发布完整 = 服务 && 上下文;
            for (std::uint64_t i = 1; 发布完整 && i <= 3; ++i) {
                const auto G = 当前代次(运行包);
                if (!G || !服务->发布服务进展事实_v2(形成进展发布请求(
                        *上下文, *G, 0x5348'0201'0000'0000ULL + i, i)).成功())
                    发布完整 = false;
            }
            for (std::uint64_t i = 1; 发布完整 && i <= 2; ++i) {
                const auto G = 当前代次(运行包);
                if (!G || !服务->发布服务准备事实_v2(形成准备发布请求(
                        *上下文, *G, 0x5348'0202'0000'0000ULL + i, i)).成功())
                    发布完整 = false;
            }
            const auto G0 = 当前代次(运行包);
            if (!服务 || !Greg || !上下文 || !发布完整 || !G0)
                return 失败("H02", "five-category N fixture");
            const auto 完整 = 服务->按事实代次范围读取服务维护历史事实完整组_v1(
                历史范围请求(上下文->自我, *Greg, *G0, 7));
            if (!完整.成功() || !完整.完整集合见证
                || 完整.合同变化组.size() != 1
                || 完整.合同状态变化组.size() != 1
                || !完整.到期事件变化组.empty()
                || 完整.服务进展变化组.size() != 3
                || 完整.服务准备变化组.size() != 2
                || 完整.完整集合见证->声明成员数 != 7
                || 完整.完整集合见证->规范成员身份组.size() != 7)
                return 失败("H02", "five-category N range and exact budget");
            if (!历史范围空失败(
                    服务->按事实代次范围读取服务维护历史事实完整组_v1(
                        历史范围请求(上下文->自我, *Greg, *G0, 6)),
                    服务维护历史事实范围读取状态_v1::数量预算不足))
                return 失败("H02", "exact total budget minus one");
            const auto 左开空组 = 服务->按事实代次范围读取服务维护历史事实完整组_v1(
                历史范围请求(上下文->自我, *G0, *G0));
            if (!左开空组.成功() || !左开空组.完整集合见证
                || 左开空组.完整集合见证->声明成员数 != 0)
                return 失败("H02", "left-open right-closed boundary");
            通过("H02", "five categories, N members, exact budget and open-left boundary");
        }

        {
            const auto 验证坏载荷 = [&](std::uint64_t 幂等身份,
                std::uint8_t 类别) {
                auto 运行包 = 建立L1事实基座运行包();
                auto 服务 = 建立服务(运行包);
                const auto Greg = 当前代次(运行包);
                const auto 自我 = 服务 && Greg
                    ? 服务->ARCH_建立历史到期事件样本_v1(
                        幂等身份, 类别 == 3, 类别)
                    : std::optional<L2存在身份>{};
                const auto G0 = 当前代次(运行包);
                if (!服务 || !Greg || !自我 || !G0) return false;
                return 历史范围空失败(
                    服务->按事实代次范围读取服务维护历史事实完整组_v1(
                        历史范围请求(*自我, *Greg, *G0)),
                    服务维护历史事实范围读取状态_v1::引用冲突);
            };
            if (!验证坏载荷(0x5348'0300'0000'0001ULL, 1)
                || !验证坏载荷(0x5348'0300'0000'0002ULL, 2)
                || !验证坏载荷(0x5348'0300'0000'0003ULL, 3))
                return 失败("H03", "damaged contract/state/expiry payload");
            通过("H03", "damaged payload in three owner fact categories fails the whole history read");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto Greg = 当前代次(运行包);
            const auto 自我 = 服务 && Greg
                ? 服务->ARCH_建立验证样本_v1(
                    0x5348'0400'0000'0001ULL, 1, 1)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !Greg || !自我 || !G0)
                return 失败("H04", "failure injection fixture");
            服务->ARCH_注入读中漂移一次();
            if (!历史范围空失败(
                    服务->按事实代次范围读取服务维护历史事实完整组_v1(
                        历史范围请求(*自我, *Greg, *G0)),
                    服务维护历史事实范围读取状态_v1::当前性漂移))
                return 失败("H04", "history read drift");
            const auto G1 = 当前代次(运行包);
            if (!G1) return 失败("H04", "resource failure G");
            服务->ARCH_注入资源失败一次();
            if (!历史范围空失败(
                    服务->按事实代次范围读取服务维护历史事实完整组_v1(
                        历史范围请求(*自我, *Greg, *G1)),
                    服务维护历史事实范围读取状态_v1::资源失败))
                return 失败("H04", "history resource failure");
            通过("H04", "drift and resource failure return exact empty results");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto Greg = 当前代次(运行包);
            auto 上下文 = 服务 && Greg ? 建立发布测试上下文(
                运行包, *服务, 0x5348'0500'0000'0000ULL)
                : std::optional<发布测试上下文>{};
            const auto G0 = 当前代次(运行包);
            const auto 发布 = 服务 && 上下文 && G0
                ? 服务->发布服务进展事实_v2(形成进展发布请求(
                    *上下文, *G0, 0x5348'0501'0000'0001ULL, 1))
                : 服务进展事实发布结果_v2{};
            if (!服务 || !Greg || !上下文 || !发布.成功() || !发布.事实)
                return 失败("H05", "missing history member fixture");
            if (!服务->ARCH_退出历史成员关系_v1(
                    服务维护历史事实类别_v1::服务进展,
                    发布.事实->身份.值, 发布.事实->形成事实代次))
                return 失败("H05", "exit permanent history member");
            const auto G1 = 当前代次(运行包);
            if (!G1 || !历史范围空失败(
                    服务->按事实代次范围读取服务维护历史事实完整组_v1(
                        历史范围请求(上下文->自我, *Greg, *G1)),
                    服务维护历史事实范围读取状态_v1::集合不闭合))
                return 失败("H05", "current progress missing from permanent history");
            通过("H05", "a current post-registration fact cannot disappear from history");
        }

        {
            const auto 验证坏成员 = [&](std::uint64_t 基数, bool 重复) {
                auto 运行包 = 建立L1事实基座运行包();
                auto 服务 = 建立服务(运行包);
                const auto Greg = 当前代次(运行包);
                auto 上下文 = 服务 && Greg ? 建立发布测试上下文(
                    运行包, *服务, 基数)
                    : std::optional<发布测试上下文>{};
                const auto G0 = 当前代次(运行包);
                const auto 发布 = 服务 && 上下文 && G0
                    ? 服务->发布服务进展事实_v2(形成进展发布请求(
                        *上下文, *G0, 基数 + 0x100, 1))
                    : 服务进展事实发布结果_v2{};
                if (!服务 || !Greg || !上下文 || !发布.成功() || !发布.事实)
                    return false;
                const bool 已损坏 = 重复
                    ? 服务->ARCH_追加重复进展历史成员关系_v1(
                        发布.事实->身份.值)
                    : 服务->ARCH_追加无载荷进展历史成员_v1();
                const auto G1 = 当前代次(运行包);
                if (!已损坏 || !G1) return false;
                return 历史范围空失败(
                    服务->按事实代次范围读取服务维护历史事实完整组_v1(
                        历史范围请求(上下文->自我, *Greg, *G1)),
                    重复 ? 服务维护历史事实范围读取状态_v1::集合不闭合
                         : 服务维护历史事实范围读取状态_v1::引用冲突);
            };
            if (!验证坏成员(0x5348'0600'0000'0000ULL, true)
                || !验证坏成员(0x5348'0601'0000'0000ULL, false))
                return 失败("H06", "duplicate or unreferenced history member");
            通过("H06", "duplicate and payload-less members fail the whole history read");
        }

        if (const auto r = 验证历史边界v2_B00(); r != 0) return r;
        if (const auto r = 验证历史边界v2_B01(); r != 0) return r;
        if (const auto r = 验证历史边界v2_B02(); r != 0) return r;
        if (const auto r = 验证历史边界v2_B03(); r != 0) return r;
        if (const auto r = 验证历史边界v2_B04(); r != 0) return r;
        if (const auto r = 验证历史边界v2_B05(); r != 0) return r;
        if (const auto r = 验证历史边界v2_B06(); r != 0) return r;
        if (const auto r = 验证历史边界v2_B07(); r != 0) return r;
        if (const auto r = 验证历史边界v2_B08(); r != 0) return r;
        if (const auto r = 验证历史边界v2_B09(); r != 0) return r;
        if (const auto r = 验证历史边界v2_B10(); r != 0) return r;

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            auto 上下文 = 服务 ? 建立发布测试上下文(
                运行包, *服务, 0x5350'0000'0000'1000ULL)
                : std::optional<发布测试上下文>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !上下文 || !G0)
                return 失败("W00", "progress publisher fixture");
            const auto 首次请求 = 形成进展发布请求(*上下文, *G0,
                0x5350'1000'0000'0001ULL, 1,
                服务进展运行状态_v1::进行中, true, false);
            const auto 首次 = 服务->发布服务进展事实_v2(首次请求);
            const auto G1 = 当前代次(运行包);
            if (!首次.成功()
                || 首次.状态 != 服务活动事实发布状态_v2::已发布
                || !首次.事实 || !G1 || *G1 != 首次.首次提交事实代次
                || 首次.事实->进展状态 == std::nullopt
                || 首次.事实->进展动态 != std::nullopt)
                return 失败("W00", "progress first publish and readback");

            auto 更新请求 = 形成进展发布请求(*上下文, *G1,
                0x5350'1000'0000'0002ULL, 1,
                服务进展运行状态_v1::等待条件, false, true);
            更新请求.预期当前事实 = 首次.事实->身份;
            更新请求.材料.运行代次 = 2;
            const auto 更新 = 服务->发布服务进展事实_v2(更新请求);
            const auto G2 = 当前代次(运行包);
            if (!更新.成功()
                || 更新.状态 != 服务活动事实发布状态_v2::已发布
                || !更新.事实 || !G2 || *G2 != 更新.首次提交事实代次
                || 更新.事实->身份 == 首次.事实->身份)
                return 失败("W00", "progress atomic successor");
            const auto 当前组 = 服务->读取当前服务合同关联进展完整集合_v2(
                进展请求_v2(上下文->自我, *G2));
            if (!当前组.成功() || 当前组.完整进展事实组.size() != 1
                || 当前组.完整进展事实组.front().身份 != 更新.事实->身份)
                return 失败("W00", "progress unique current successor");

            const auto 首次重放 = 服务->发布服务进展事实_v2(首次请求);
            if (!首次重放.成功()
                || 首次重放.状态 != 服务活动事实发布状态_v2::精确重复
                || !首次重放.事实
                || 首次重放.事实->身份 != 首次.事实->身份
                || 首次重放.首次提交事实代次 != 首次.首次提交事实代次
                || 首次重放.事实->生命周期.退出事实代次
                || !外部端点仍当前(运行包, 上下文->外部端点))
                return 失败("W00", "progress historical replay after replacement");
            const auto 历史组 = 服务->按事实代次范围读取服务维护历史事实完整组_v1(
                历史范围请求(上下文->自我, *G0, *G2));
            if (!历史组.成功() || 历史组.服务进展变化组.size() != 2
                || !历史组.合同变化组.empty()
                || 历史组.服务进展变化组.front().事实.身份 != 首次.事实->身份
                || 历史组.服务进展变化组.front().退出当前事实代次
                    != std::optional<std::uint64_t>{更新.首次提交事实代次}
                || 历史组.服务进展变化组.back().事实.身份 != 更新.事实->身份
                || 历史组.服务进展变化组.back().退出当前事实代次)
                return 失败("W00", "progress permanent history ledger");

            auto 异义 = 首次请求;
            异义.材料.运行代次 = 99;
            if (!进展发布空失败_v2(服务->发布服务进展事实_v2(异义),
                    服务活动事实发布状态_v2::幂等冲突))
                return 失败("W00", "progress same-key conflict");
            auto 跨入口 = 形成准备发布请求(*上下文, *G2,
                首次请求.幂等身份.值, 1);
            if (!准备发布空失败_v2(服务->发布服务准备事实_v2(跨入口),
                    服务活动事实发布状态_v2::幂等冲突))
                return 失败("W00", "cross-entry idempotency conflict");
            通过("W00", "progress publish, replace and historical replay preserve external endpoints");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            auto 上下文 = 服务 ? 建立发布测试上下文(
                运行包, *服务, 0x5350'0000'0000'2000ULL)
                : std::optional<发布测试上下文>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !上下文 || !G0)
                return 失败("W01", "preparation publisher fixture");
            const auto 首次请求 = 形成准备发布请求(*上下文, *G0,
                0x5350'2000'0000'0001ULL, 1,
                服务准备运行状态_v1::进行中, true, true, false);
            const auto 首次 = 服务->发布服务准备事实_v2(首次请求);
            const auto G1 = 当前代次(运行包);
            if (!首次.成功()
                || 首次.状态 != 服务活动事实发布状态_v2::已发布
                || !首次.事实 || !G1 || *G1 != 首次.首次提交事实代次
                || !std::holds_alternative<L2需求身份>(首次.事实->来源)
                || !首次.事实->T到D关系稳定编码)
                return 失败("W01", "demand preparation first publish");

            auto 完成请求 = 形成准备发布请求(*上下文, *G1,
                0x5350'2000'0000'0002ULL, 1,
                服务准备运行状态_v1::已完整完成, true, true, true);
            完成请求.预期当前事实 = 首次.事实->身份;
            完成请求.材料.运行代次 = 2;
            const auto 完成 = 服务->发布服务准备事实_v2(完成请求);
            const auto G2 = 当前代次(运行包);
            if (!完成.成功() || !完成.事实 || !G2
                || !完成.事实->准备结果 || !完成.事实->完成验证)
                return 失败("W01", "preparation completed successor");
            const auto 首次重放 = 服务->发布服务准备事实_v2(首次请求);
            if (!首次重放.成功()
                || 首次重放.状态 != 服务活动事实发布状态_v2::精确重复
                || !首次重放.事实
                || 首次重放.事实->身份 != 首次.事实->身份
                || 首次重放.事实->生命周期.退出事实代次)
                return 失败("W01", "preparation historical replay");

            const auto 能力缺口请求 = 形成准备发布请求(*上下文, *G2,
                0x5350'2000'0000'0003ULL, 2,
                服务准备运行状态_v1::等待条件, false, false, false);
            const auto 能力缺口 = 服务->发布服务准备事实_v2(能力缺口请求);
            const auto G3 = 当前代次(运行包);
            if (!能力缺口.成功() || !能力缺口.事实 || !G3
                || !std::holds_alternative<服务能力缺口身份_v1>(
                    能力缺口.事实->来源)
                || 能力缺口.事实->T到D关系稳定编码)
                return 失败("W01", "gap preparation source shape");
            const auto 当前组 = 服务->读取当前服务准备完整集合_v2(
                准备请求_v2(上下文->自我, *G3));
            if (!当前组.成功() || 当前组.完整准备事实组.size() != 2
                || !外部端点仍当前(运行包, 上下文->外部端点))
                return 失败("W01", "preparation current closure");
            const auto 历史组 = 服务->按事实代次范围读取服务维护历史事实完整组_v1(
                历史范围请求(上下文->自我, *G0, *G3));
            if (!历史组.成功() || 历史组.服务准备变化组.size() != 3
                || 历史组.服务准备变化组.front().事实.身份 != 首次.事实->身份
                || 历史组.服务准备变化组.front().退出当前事实代次
                    != std::optional<std::uint64_t>{完成.首次提交事实代次})
                return 失败("W01", "preparation permanent history ledger");
            通过("W01", "demand and gap preparations publish with exact source shapes");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            auto 上下文 = 服务 ? 建立发布测试上下文(
                运行包, *服务, 0x5350'0000'0000'3000ULL)
                : std::optional<发布测试上下文>{};
            if (!服务 || !上下文) return 失败("W02", "state matrix fixture");
            for (std::uint64_t i = 1; i <= 8; ++i) {
                const auto G = 当前代次(运行包);
                if (!G) return 失败("W02", "progress state G0");
                const auto r = 服务->发布服务进展事实_v2(
                    形成进展发布请求(*上下文, *G,
                        0x5350'3000'0000'0000ULL + i, 100 + i,
                        static_cast<服务进展运行状态_v1>(i),
                        i % 3 != 1, i % 3 != 0));
                if (!r.成功()) return 失败("W02", "progress state publish");
            }
            bool 进展状态[8]{};
            const auto Gp = 当前代次(运行包);
            const auto p = Gp ? 服务->读取当前服务合同关联进展完整集合_v2(
                进展请求_v2(上下文->自我, *Gp))
                : 服务进展完整集合读取结果_v2{};
            if (!p.成功() || p.完整进展事实组.size() != 8)
                return 失败("W02", "progress state matrix readback");
            for (const auto& f : p.完整进展事实组) {
                const auto s = static_cast<std::uint8_t>(f.运行状态);
                if (s < 1 || s > 8) return 失败("W02", "progress state range");
                进展状态[s - 1] = true;
            }
            for (const bool 已见 : 进展状态)
                if (!已见) return 失败("W02", "all progress states");

            for (std::uint64_t i = 1; i <= 9; ++i) {
                const auto G = 当前代次(运行包);
                if (!G) return 失败("W02", "preparation state G0");
                const auto r = 服务->发布服务准备事实_v2(
                    形成准备发布请求(*上下文, *G,
                        0x5350'3100'0000'0000ULL + i, 100 + i,
                        static_cast<服务准备运行状态_v1>(i), i % 2 != 0,
                        i % 3 != 1, i % 3 != 0));
                if (!r.成功()) return 失败("W02", "preparation state publish");
            }
            bool 准备状态[9]{};
            const auto Gr = 当前代次(运行包);
            const auto r = Gr ? 服务->读取当前服务准备完整集合_v2(
                准备请求_v2(上下文->自我, *Gr))
                : 服务准备完整集合读取结果_v2{};
            if (!r.成功() || r.完整准备事实组.size() != 9)
                return 失败("W02", "preparation state matrix readback");
            for (const auto& f : r.完整准备事实组) {
                const auto s = static_cast<std::uint8_t>(f.运行状态);
                if (s < 1 || s > 9) return 失败("W02", "preparation state range");
                准备状态[s - 1] = true;
            }
            for (const bool 已见 : 准备状态)
                if (!已见) return 失败("W02", "all preparation states");
            通过("W02", "all progress and preparation states publish with optional evidence shapes");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            auto 上下文 = 服务 ? 建立发布测试上下文(
                运行包, *服务, 0x5350'0000'0000'4000ULL)
                : std::optional<发布测试上下文>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !上下文 || !G0)
                return 失败("W03", "publisher failure fixture");
            auto 首进展请求 = 形成进展发布请求(*上下文, *G0,
                0x5350'4000'0000'0001ULL, 1);
            const auto 首进展 = 服务->发布服务进展事实_v2(首进展请求);
            const auto G1 = 当前代次(运行包);
            if (!首进展.成功() || !首进展.事实 || !G1)
                return 失败("W03", "failure fixture progress current");
            auto 首准备请求 = 形成准备发布请求(*上下文, *G1,
                0x5350'4000'0000'0002ULL, 1);
            const auto 首准备 = 服务->发布服务准备事实_v2(首准备请求);
            const auto G2 = 当前代次(运行包);
            if (!首准备.成功() || !首准备.事实 || !G2)
                return 失败("W03", "failure fixture preparation current");

            auto 坏材料 = 形成进展发布请求(*上下文, *G2,
                0x5350'4000'0000'0010ULL, 2);
            坏材料.材料.运行代次 = 0;
            if (!进展发布空失败_v2(服务->发布服务进展事实_v2(坏材料),
                    服务活动事实发布状态_v2::入口拒绝))
                return 失败("W03", "bad progress material");
            auto 漏预期 = 形成进展发布请求(*上下文, *G2,
                0x5350'4000'0000'0011ULL, 1);
            if (!进展发布空失败_v2(服务->发布服务进展事实_v2(漏预期),
                    服务活动事实发布状态_v2::当前事实冲突))
                return 失败("W03", "missing expected current progress");
            auto 错预期 = 漏预期;
            错预期.幂等身份 = {0x5350'4000'0000'0012ULL};
            错预期.预期当前事实 = 服务进展事实身份_v2{
                上下文->外部端点.节点[10]};
            if (!进展发布空失败_v2(服务->发布服务进展事实_v2(错预期),
                    服务活动事实发布状态_v2::当前事实冲突))
                return 失败("W03", "wrong expected current progress");

            auto 错需求 = 形成进展发布请求(*上下文, *G2,
                0x5350'4000'0000'0013ULL, 3);
            错需求.材料.需求 = L2需求身份{上下文->外部端点.节点[10]};
            if (!进展发布空失败_v2(服务->发布服务进展事实_v2(错需求),
                    服务活动事实发布状态_v2::引用冲突))
                return 失败("W03", "contract self-demand mismatch");
            auto 坏引用 = 形成准备发布请求(*上下文, *G2,
                0x5350'4000'0000'0014ULL, 3);
            坏引用.材料.任务 = L2任务身份{{0xFFFF'FFFF'FFFF'FFF0ULL}};
            if (!准备发布空失败_v2(服务->发布服务准备事实_v2(坏引用),
                    服务活动事实发布状态_v2::引用冲突))
                return 失败("W03", "missing external endpoint");

            auto 推进 = 形成进展发布请求(*上下文, *G2,
                0x5350'4000'0000'0015ULL, 4);
            if (!服务->发布服务进展事实_v2(推进).成功())
                return 失败("W03", "advance generation");
            auto 旧G = 形成进展发布请求(*上下文, *G2,
                0x5350'4000'0000'0016ULL, 5);
            if (!进展发布空失败_v2(服务->发布服务进展事实_v2(旧G),
                    服务活动事实发布状态_v2::当前性漂移))
                return 失败("W03", "stale G0");
            const auto G3 = 当前代次(运行包);
            if (!G3) return 失败("W03", "resource G0");
            服务->ARCH_注入资源失败一次();
            auto 资源 = 形成进展发布请求(*上下文, *G3,
                0x5350'4000'0000'0017ULL, 6);
            if (!进展发布空失败_v2(服务->发布服务进展事实_v2(资源),
                    服务活动事实发布状态_v2::资源失败))
                return 失败("W03", "progress resource failure");
            const auto G4 = 当前代次(运行包);
            if (!G4) return 失败("W03", "preparation resource G0");
            服务->ARCH_注入资源失败一次();
            auto 准备资源 = 形成准备发布请求(*上下文, *G4,
                0x5350'4000'0000'0018ULL, 4);
            if (!准备发布空失败_v2(服务->发布服务准备事实_v2(准备资源),
                    服务活动事实发布状态_v2::资源失败))
                return 失败("W03", "preparation resource failure");
            通过("W03", "invalid, conflicting, stale and resource paths remain zero-payload");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            auto 上下文 = 服务 ? 建立发布测试上下文(
                运行包, *服务, 0x5350'0000'0000'5000ULL)
                : std::optional<发布测试上下文>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !上下文 || !G0)
                return 失败("W04", "post-commit failure fixture");
            auto 进展请求 = 形成进展发布请求(*上下文, *G0,
                0x5350'5000'0000'0001ULL, 1);
            服务->ARCH_注入发布后读回失败一次();
            const auto 进展可能 = 服务->发布服务进展事实_v2(进展请求);
            if (!进展发布空失败_v2(进展可能,
                    服务活动事实发布状态_v2::已可能发布))
                return 失败("W04", "progress possible publish witness");
            const auto 进展重试 = 服务->发布服务进展事实_v2(进展请求);
            if (!进展重试.成功()
                || 进展重试.状态 != 服务活动事实发布状态_v2::精确重复
                || 进展重试.首次提交事实代次
                    != 进展可能.首次提交事实代次)
                return 失败("W04", "progress retry convergence");

            const auto G1 = 当前代次(运行包);
            if (!G1) return 失败("W04", "preparation post-commit G0");
            auto 准备请求 = 形成准备发布请求(*上下文, *G1,
                0x5350'5000'0000'0002ULL, 1);
            服务->ARCH_注入发布后读回失败一次();
            const auto 准备可能 = 服务->发布服务准备事实_v2(准备请求);
            if (!准备发布空失败_v2(准备可能,
                    服务活动事实发布状态_v2::已可能发布))
                return 失败("W04", "preparation possible publish witness");
            const auto 准备重试 = 服务->发布服务准备事实_v2(准备请求);
            if (!准备重试.成功()
                || 准备重试.状态 != 服务活动事实发布状态_v2::精确重复
                || 准备重试.首次提交事实代次
                    != 准备可能.首次提交事实代次)
                return 失败("W04", "preparation retry convergence");
            通过("W04", "post-commit readback failure converges by original idempotent request");
        }

        {
            const auto 根 = std::filesystem::path{L"D:\\TEMP"} / L"海中鱼巣"
                / L"INSTINCT-STAGE3-SERVICE-ACTIVITY-V2-PUBLISHER"
                / (std::to_wstring(GetCurrentProcessId()) + L"-"
                    + std::to_wstring(GetTickCount64()));
            隔离根清理 清理{根};
            std::error_code 错误;
            std::filesystem::create_directories(根, 错误);
            if (错误) return 失败("W05", "create persistent publisher root");
            const L1事实基座持久存储配置_v1 配置{
                L1事实基座持久恢复合同版本_v1, 根};
            发布服务进展事实请求_v2 进展请求{};
            发布服务准备事实请求_v2 准备请求{};
            服务进展事实发布结果_v2 进展期望{};
            服务准备事实发布结果_v2 准备期望{};
            服务维护历史事实范围读取结果_v1 历史期望{};
            服务维护历史事实范围读取结果_v1 到期历史期望{};
            服务维护历史边界读取结果_v2 边界期望{};
            L2存在身份 自我{};
            L2存在身份 到期自我{};
            std::uint64_t 历史起点 = 0;
            std::uint64_t 边界Gseed = 0;
            std::uint64_t 边界Gstart = 0;
            std::uint64_t 边界G0 = 0;
            {
                auto 建立 = 建立L1事实基座持久运行包_v1(配置);
                if (!建立.成功() || !建立.运行包)
                    return 失败("W05", "first persistent publisher package");
                auto 运行包 = std::move(*建立.运行包);
                auto 服务 = 建立服务(运行包);
                const auto Greg = 当前代次(运行包);
                auto 上下文 = 服务 ? 建立发布测试上下文(
                    运行包, *服务, 0x5350'0000'0000'6000ULL)
                    : std::optional<发布测试上下文>{};
                const auto G0 = 当前代次(运行包);
                if (!服务 || !Greg || !上下文 || !G0)
                    return 失败("W05", "persistent publisher fixture");
                历史起点 = *Greg;
                自我 = 上下文->自我;
                进展请求 = 形成进展发布请求(*上下文, *G0,
                    0x5350'6000'0000'0001ULL, 1);
                进展期望 = 服务->发布服务进展事实_v2(进展请求);
                const auto G1 = 当前代次(运行包);
                if (!进展期望.成功() || !G1)
                    return 失败("W05", "persistent progress publish");
                准备请求 = 形成准备发布请求(*上下文, *G1,
                    0x5350'6000'0000'0002ULL, 1);
                准备期望 = 服务->发布服务准备事实_v2(准备请求);
                if (!准备期望.成功())
                    return 失败("W05", "persistent preparation publish");
                const auto 到期自我结果 = 服务->ARCH_建立历史到期事件样本_v1(
                    0x5350'6000'0000'1000ULL);
                if (!到期自我结果)
                    return 失败("W05", "persistent expiry history fixture");
                到期自我 = *到期自我结果;
                const auto G2 = 当前代次(运行包);
                历史期望 = G2
                    ? 服务->按事实代次范围读取服务维护历史事实完整组_v1(
                        历史范围请求(自我, 历史起点, *G2))
                    : 服务维护历史事实范围读取结果_v1{};
                到期历史期望 = G2
                    ? 服务->按事实代次范围读取服务维护历史事实完整组_v1(
                        历史范围请求(到期自我, 历史起点, *G2))
                    : 服务维护历史事实范围读取结果_v1{};
                const auto 边界定位 = G2
                    ? 服务->按事实代次边界读取服务维护历史完整快照_v2(
                        历史边界请求_v2(自我, *G2, *G2, 1024, 1024))
                    : 服务维护历史边界读取结果_v2{};
                if (!边界定位.成功() || !边界定位.快照)
                    return 失败("W05", "persistent boundary seed lookup");
                边界Gseed = 边界定位.快照->v2覆盖登记事实代次;
                边界Gstart = 边界Gseed;
                边界G0 = *G2;
                边界期望 = 服务->按事实代次边界读取服务维护历史完整快照_v2(
                    历史边界请求_v2(
                        自我, 边界Gstart, 边界G0, 1024, 1024));
                if (!历史期望.成功()
                    || 历史期望.合同变化组.size() != 1
                    || 历史期望.合同状态变化组.size() != 1
                    || 历史期望.服务进展变化组.size() != 1
                    || 历史期望.服务准备变化组.size() != 1
                    || !到期历史期望.成功()
                    || 到期历史期望.合同变化组.size() != 1
                    || 到期历史期望.合同状态变化组.size() != 2
                    || 到期历史期望.到期事件变化组.size() != 1
                    || !边界期望.成功() || !边界期望.快照
                    || 边界期望.快照->v2覆盖登记事实代次 != 边界Gseed
                    || 边界期望.快照->自我 != 自我
                    || 边界期望.快照->左边界事实代次 != 边界Gstart
                    || 边界期望.快照->包含结束事实代次 != 边界G0
                    || 边界期望.本次正式读回截止 != 边界G0)
                    return 失败("W05", "persistent history range before recovery");
            }
            {
                auto 恢复 = 建立L1事实基座持久运行包_v1(配置);
                if (!恢复.成功() || !恢复.运行包)
                    return 失败("W05", "recover persistent publisher package");
                auto 运行包 = std::move(*恢复.运行包);
                auto 服务 = 建立服务(运行包);
                const auto G0 = 当前代次(运行包);
                if (!服务 || !G0) return 失败("W05", "recovered publisher G0");
                const auto 进展重放 = 服务->发布服务进展事实_v2(进展请求);
                const auto 准备重放 = 服务->发布服务准备事实_v2(准备请求);
                const auto G1 = 当前代次(运行包);
                const auto 当前进展 = G1
                    ? 服务->读取当前服务合同关联进展完整集合_v2(
                        进展请求_v2(自我, *G1))
                    : 服务进展完整集合读取结果_v2{};
                const auto 当前准备 = G1
                    ? 服务->读取当前服务准备完整集合_v2(
                        准备请求_v2(自我, *G1))
                    : 服务准备完整集合读取结果_v2{};
                const auto 历史重放 = G1
                    ? 服务->按事实代次范围读取服务维护历史事实完整组_v1(
                        历史范围请求(自我, 历史起点, *G1))
                    : 服务维护历史事实范围读取结果_v1{};
                const auto 到期历史重放 = G1
                    ? 服务->按事实代次范围读取服务维护历史事实完整组_v1(
                        历史范围请求(到期自我, 历史起点, *G1))
                    : 服务维护历史事实范围读取结果_v1{};
                const auto 边界重放 = G1 && *G1 == 边界G0
                    ? 服务->按事实代次边界读取服务维护历史完整快照_v2(
                        历史边界请求_v2(
                            自我, 边界Gstart, 边界G0, 1024, 1024))
                    : 服务维护历史边界读取结果_v2{};
                if (!G1 || *G1 != *G0 || !进展重放.成功()
                    || !准备重放.成功()
                    || 进展重放.状态
                        != 服务活动事实发布状态_v2::精确重复
                    || 准备重放.状态
                        != 服务活动事实发布状态_v2::精确重复
                    || 进展重放.首次提交事实代次
                        != 进展期望.首次提交事实代次
                    || 准备重放.首次提交事实代次
                        != 准备期望.首次提交事实代次
                    || !当前进展.成功() || !当前准备.成功()
                    || 历史重放 != 历史期望
                    || 到期历史重放 != 到期历史期望
                    || !边界重放.成功() || !边界重放.快照
                    || 边界重放.快照->v2覆盖登记事实代次 != 边界Gseed
                    || 边界重放.快照->左边界事实代次 != 边界Gstart
                    || 边界重放.快照->包含结束事实代次 != 边界G0
                    || 边界重放 != 边界期望
                    || 当前进展.完整进展事实组.size() != 1
                    || 当前准备.完整准备事实组.size() != 1)
                    return 失败("W05", "persistent exact replay and current readback");
            }
            通过("W05", "publishers and history boundary replay exactly after persistent recovery");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'0200ULL, 0, 0)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0) return 失败("Q00", "empty terminal fixture");
            auto 坏请求 = 事件请求(*自我, *G0);
            坏请求.合同版本 = 0;
            if (!事件终态空失败(
                    服务->读取到期未满足事件合同终态完整集合_v1(坏请求),
                    服务合同事实权威读取状态_v1::入口拒绝))
                return 失败("Q00", "invalid terminal request");
            const auto 空组 =
                服务->读取到期未满足事件合同终态完整集合_v1(
                    事件请求(*自我, *G0));
            if (!空组.成功() || !空组.完整集合见证
                || 空组.完整集合见证->声明成员数 != 0
                || !空组.完整投影组.empty()
                || 空组.本次正式读回截止 != *G0)
                return 失败("Q00", "legal empty terminal witness");
            通过("Q00", "invalid request is empty failure and zero members are proven");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'0210ULL, 1, 1, false, false,
                    0, false, 0, false, false, false, true)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0) return 失败("Q01", "single terminal fixture");
            const auto 单项 =
                服务->读取到期未满足事件合同终态完整集合_v1(
                    事件请求(*自我, *G0));
            if (!单项.成功() || 单项.完整投影组.size() != 1
                || 单项.完整投影组.front().原合同.时间纪元身份.值
                    != 0xF3000001ULL
                || 单项.完整投影组.front().当前终态.状态
                    != 服务合同当前状态_v1::已到期)
                return 失败("Q01", "single terminal projection");
            通过("Q01", "one event retains the original contract epoch and expired state");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'0220ULL, 3, 3, false, false,
                    0, false, 0, false, false, false, true)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0) return 失败("Q02", "N terminal fixture");
            const auto N项 =
                服务->读取到期未满足事件合同终态完整集合_v1(
                    事件请求(*自我, *G0));
            bool 纪元[3]{};
            if (!N项.成功() || N项.完整投影组.size() != 3
                || !N项.完整集合见证
                || N项.完整集合见证->声明成员数 != 3)
                return 失败("Q02", "N terminal complete readback");
            for (const auto& 投影 : N项.完整投影组) {
                const auto 值 = 投影.原合同.时间纪元身份.值;
                if (值 < 0xF3000001ULL || 值 > 0xF3000003ULL)
                    return 失败("Q02", "original epoch range");
                纪元[值 - 0xF3000001ULL] = true;
            }
            for (const auto 已见 : 纪元)
                if (!已见) return 失败("Q02", "all original epochs retained");
            const auto 重放自我 = 服务->ARCH_建立验证样本_v1(
                0x5343'0000'0000'0220ULL, 3, 3, false, false,
                0, false, 0, false, false, false, true);
            const auto G重放 = 当前代次(运行包);
            if (!重放自我 || !G重放 || *G重放 != *G0
                || 服务->读取到期未满足事件合同终态完整集合_v1(
                    事件请求(*自我, *G重放)) != N项)
                return 失败("Q02", "N terminal exact replay");
            通过("Q02", "N projections are canonical, epoch-faithful and replay-stable");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'0230ULL, 1, 1)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0
                || !事件终态空失败(
                    服务->读取到期未满足事件合同终态完整集合_v1(
                        事件请求(*自我, *G0)),
                    服务合同事实权威读取状态_v1::引用冲突))
                return 失败("Q03", "non-expired terminal accepted");
            通过("Q03", "an event without an expired current state fails closed");
        }

        {
            const auto 验证损坏 = [](std::uint64_t 幂等身份,
                std::size_t 合同数, std::size_t 事件数,
                bool 坏秒, bool 坏版本, bool 漏成员, bool 重成员,
                bool 错合同, bool 漏状态, bool 重状态,
                服务合同事实权威读取状态_v1 期望状态) {
                auto 运行包 = 建立L1事实基座运行包();
                auto 服务 = 建立服务(运行包);
                const auto 自我 = 服务
                    ? 服务->ARCH_建立验证样本_v1(
                        幂等身份, 合同数, 事件数, false, false,
                        0, false, 0, false, false, false, true,
                        坏秒, 坏版本, 漏成员, 重成员, 错合同,
                        漏状态, 重状态)
                    : std::optional<L2存在身份>{};
                const auto G0 = 当前代次(运行包);
                return 服务 && 自我 && G0
                    && 事件终态空失败(
                        服务->读取到期未满足事件合同终态完整集合_v1(
                            事件请求(*自我, *G0)), 期望状态);
            };
            if (!验证损坏(0x5343'0000'0000'0240ULL, 1, 1,
                    true, false, false, false, false, false, false,
                    服务合同事实权威读取状态_v1::引用冲突)
                || !验证损坏(0x5343'0000'0000'0241ULL, 1, 1,
                    false, true, false, false, false, false, false,
                    服务合同事实权威读取状态_v1::引用冲突)
                || !验证损坏(0x5343'0000'0000'0242ULL, 1, 1,
                    false, false, true, false, false, false, false,
                    服务合同事实权威读取状态_v1::集合不闭合)
                || !验证损坏(0x5343'0000'0000'0243ULL, 1, 1,
                    false, false, false, true, false, false, false,
                    服务合同事实权威读取状态_v1::集合不闭合)
                || !验证损坏(0x5343'0000'0000'0244ULL, 2, 1,
                    false, false, false, false, true, false, false,
                    服务合同事实权威读取状态_v1::引用冲突)
                || !验证损坏(0x5343'0000'0000'0245ULL, 1, 1,
                    false, false, false, false, false, true, false,
                    服务合同事实权威读取状态_v1::集合不闭合)
                || !验证损坏(0x5343'0000'0000'0246ULL, 1, 1,
                    false, false, false, false, false, false, true,
                    服务合同事实权威读取状态_v1::集合不闭合))
                return 失败("Q04", "terminal closure corruption matrix");
            通过("Q04", "seconds, version, member, contract and state corruption fail closed");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'0250ULL, 1, 1, false, true,
                    0, false, 0, false, false, false, true)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0
                || !事件终态空失败(
                    服务->读取到期未满足事件合同终态完整集合_v1(
                        事件请求(*自我, *G0)),
                    服务合同事实权威读取状态_v1::引用冲突))
                return 失败("Q05", "damaged original contract payload");
            通过("Q05", "damaged original contract payload cannot become partial success");
        }

        {
            auto 运行包 = 建立L1事实基座运行包();
            auto 服务 = 建立服务(运行包);
            const auto 自我 = 服务
                ? 服务->ARCH_建立验证样本_v1(
                    0x5343'0000'0000'0260ULL, 1, 1, false, false,
                    0, false, 0, false, false, false, true)
                : std::optional<L2存在身份>{};
            const auto G0 = 当前代次(运行包);
            if (!服务 || !自我 || !G0) return 失败("Q06", "terminal drift fixture");
            服务->ARCH_注入读中漂移一次();
            if (!事件终态空失败(
                    服务->读取到期未满足事件合同终态完整集合_v1(
                        事件请求(*自我, *G0)),
                    服务合同事实权威读取状态_v1::当前性漂移))
                return 失败("Q06", "terminal read drift");
            const auto G1 = 当前代次(运行包);
            if (!G1) return 失败("Q06", "terminal resource G");
            服务->ARCH_注入资源失败一次();
            if (!事件终态空失败(
                    服务->读取到期未满足事件合同终态完整集合_v1(
                        事件请求(*自我, *G1)),
                    服务合同事实权威读取状态_v1::资源失败))
                return 失败("Q06", "terminal resource failure");
            通过("Q06", "terminal drift and resource failure return no stale payload");
        }
        return 0;
    } catch (const std::exception& 异常) {
        std::cout << "[INSTINCT-SERVICE-CONTRACT][PXX] exception: "
                  << 异常.what() << '\n';
        return 失败("PXX", "unexpected exception");
    } catch (...) {
        return 失败("PXX", "unexpected exception");
    }
#endif
}

} // namespace 海中鱼巣
