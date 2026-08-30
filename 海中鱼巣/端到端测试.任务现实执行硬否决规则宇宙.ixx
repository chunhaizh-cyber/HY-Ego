module;

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <system_error>
#include <utility>
#include <variant>
#include <vector>

export module 海中鱼巣.端到端测试.任务现实执行硬否决规则宇宙;

import 海中鱼巣.核心.服务.L1事实基座;
import 海中鱼巣.领域.服务.L2存在结构;
import 海中鱼巣.领域.服务.L2场景结构;
import 海中鱼巣.领域.安全治理.服务.L2任务现实执行硬否决规则结构;
import 海中鱼巣.领域.安全治理.服务.任务现实执行硬否决;

export namespace 海中鱼巣 {
int 运行任务现实执行硬否决规则宇宙端到端测试() noexcept;
}

module :private;

namespace 海中鱼巣::任务现实执行硬否决规则宇宙测试内部 {

int 失败(const char* 编号, const char* 步骤) noexcept {
    std::cout << "[INSTINCT-HARD-DENIAL][" << 编号 << "] FAIL: "
              << 步骤 << '\n';
    return 1;
}

void 通过(const char* 编号, const char* 说明) {
    std::cout << "[INSTINCT-HARD-DENIAL][" << 编号 << "] PASS: "
              << 说明 << '\n';
}

std::optional<std::uint64_t> 当前代次(const L1事实基座运行包& 运行包) {
    const auto 结果 = 运行包.读取服务().读取中性当前事实代次(
        {L1中性CRUD合同版本});
    if (结果.状态 != L1中性读取状态::成功 || 结果.事实代次 == 0)
        return std::nullopt;
    return 结果.事实代次;
}

struct 测试上下文 final {
    L1事实基座运行包 运行包;
    std::unique_ptr<L2存在结构服务> 存在服务;
    std::unique_ptr<L2场景结构服务> 场景服务;
    std::unique_ptr<L2任务现实执行硬否决规则结构服务> 规则服务;

    测试上下文() : 运行包(建立L1事实基座运行包()) {}
    explicit 测试上下文(L1事实基座运行包&& 已建立运行包)
        : 运行包(std::move(已建立运行包)) {}
};

std::unique_ptr<测试上下文> 完成上下文装配(
    std::unique_ptr<测试上下文> c) {
    auto 存在原始交付 = c->运行包.所有者范围签发器().建立所有者范围(
        {L1所有者范围CRUD合同版本, 存在所有者建立身份,
            L1所有者范围种类::独占结构范围});
    if ((存在原始交付.建立结果.状态 != L1所有者范围管理状态::成功
            && 存在原始交付.建立结果.状态
                != L1所有者范围管理状态::精确重复)
        || !存在原始交付.写入端口)
        return {};
    auto 存在交付 = 尝试形成L2存在所有者交付(
        c->运行包.读取服务(), std::move(存在原始交付));
    if (!存在交付) return {};
    c->存在服务 = std::make_unique<L2存在结构服务>(
        c->运行包.读取服务(), std::move(*存在交付));

    auto 场景原始交付 = c->运行包.所有者范围签发器().建立所有者范围(
        {L1所有者范围CRUD合同版本, 场景所有者建立身份,
            L1所有者范围种类::独占结构范围});
    if ((场景原始交付.建立结果.状态 != L1所有者范围管理状态::成功
            && 场景原始交付.建立结果.状态
                != L1所有者范围管理状态::精确重复)
        || !场景原始交付.写入端口)
        return {};
    auto 场景交付 = 尝试形成L2场景所有者交付(
        c->运行包.读取服务(), std::move(场景原始交付));
    if (!场景交付) return {};
    c->场景服务 = std::make_unique<L2场景结构服务>(
        c->运行包.读取服务(), *c->存在服务, std::move(*场景交付));

    auto 规则原始交付 = c->运行包.所有者范围签发器().建立所有者范围(
        {L1所有者范围CRUD合同版本,
            任务现实执行硬否决规则所有者建立身份,
            L1所有者范围种类::独占结构范围});
    if ((规则原始交付.建立结果.状态 != L1所有者范围管理状态::成功
            && 规则原始交付.建立结果.状态
                != L1所有者范围管理状态::精确重复)
        || !规则原始交付.写入端口)
        return {};
    auto 规则交付 = 尝试形成L2任务现实执行硬否决规则结构所有者交付(
        c->运行包.读取服务(), std::move(规则原始交付));
    if (!规则交付) return {};
    c->规则服务 =
        std::make_unique<L2任务现实执行硬否决规则结构服务>(
            c->运行包.读取服务(), *c->场景服务, std::move(*规则交付));
    return c;
}

std::unique_ptr<测试上下文> 建立上下文() {
    return 完成上下文装配(std::make_unique<测试上下文>());
}

std::unique_ptr<测试上下文> 建立上下文(
    L1事实基座运行包&& 已建立运行包) {
    return 完成上下文装配(
        std::make_unique<测试上下文>(std::move(已建立运行包)));
}

struct 隔离根清理 final {
    std::filesystem::path 根;

    ~隔离根清理() {
        std::error_code 错误;
        std::filesystem::remove_all(根, 错误);
    }
};

std::optional<L2存在身份> 新增存在(
    测试上下文& c, std::uint64_t 幂等身份) {
    const auto G0 = 当前代次(c.运行包);
    if (!G0) return std::nullopt;
    const auto 结果 = c.存在服务->新增存在节点(
        {{L2结构合同版本, *G0}, {幂等身份}});
    if ((结果.结果头.状态 != L2结构状态::已提交
            && 结果.结果头.状态 != L2结构状态::精确重复)
        || !结果.存在)
        return std::nullopt;
    return 结果.存在->身份;
}

std::optional<L2场景身份> 建立场景(
    测试上下文& c, std::uint64_t 幂等身份) {
    const auto G0 = 当前代次(c.运行包);
    if (!G0) return std::nullopt;
    const auto 结果 = c.场景服务->建立场景树根(
        {{L2结构合同版本, *G0}, {幂等身份}});
    if ((结果.结果头.状态 != L2结构状态::已提交
            && 结果.结果头.状态 != L2结构状态::精确重复)
        || !结果.场景节点)
        return std::nullopt;
    return 结果.场景节点->场景.身份;
}

bool 新增成员(测试上下文& c, L2场景身份 场景, L2存在身份 存在,
    std::uint64_t 顺序, std::uint64_t 幂等身份) {
    const auto G0 = 当前代次(c.运行包);
    if (!G0) return false;
    const auto 结果 = c.场景服务->新增场景成员关系(
        {{L2结构合同版本, *G0}, {幂等身份}, 场景, 存在, 顺序});
    return (结果.结果头.状态 == L2结构状态::已提交
            || 结果.结果头.状态 == L2结构状态::精确重复)
        && 结果.当前关系.has_value();
}

struct 场景夹具 final {
    L2存在身份 自我;
    L2存在身份 主体;
    L2存在身份 其它存在;
    L2场景身份 场景;
    L2场景身份 其它场景;
};

std::optional<场景夹具> 建立场景夹具(测试上下文& c, std::uint64_t 基数,
    bool 自我入场 = true, bool 主体入场 = true) {
    const auto 自我 = 新增存在(c, 基数 + 1);
    const auto 主体 = 新增存在(c, 基数 + 2);
    const auto 其它存在 = 新增存在(c, 基数 + 3);
    const auto 场景 = 建立场景(c, 基数 + 4);
    const auto 其它场景 = 建立场景(c, 基数 + 5);
    if (!自我 || !主体 || !其它存在 || !场景 || !其它场景)
        return std::nullopt;
    std::uint64_t 顺序 = 1;
    if (自我入场 && !新增成员(c, *场景, *自我, 顺序++, 基数 + 6))
        return std::nullopt;
    if (主体入场 && !新增成员(c, *场景, *主体, 顺序++, 基数 + 7))
        return std::nullopt;
    if (!新增成员(c, *其它场景, *其它存在, 1, 基数 + 8))
        return std::nullopt;
    return 场景夹具{*自我, *主体, *其它存在, *场景, *其它场景};
}

任务冻结动作项 形成动作(std::uint64_t 顺序, L2存在身份 主体,
    任务动作作用范围 范围, std::optional<L2场景身份> 场景,
    std::uint64_t 基数) {
    return {顺序, L2方法动作入口身份{稳定编码{基数 + 1}},
        L2方法稳定动作键{基数 + 2}, 范围, 主体, 场景,
        稳定编码{基数 + 3}, 稳定编码{基数 + 4},
        稳定编码{基数 + 5}, 稳定编码{基数 + 6}};
}

L2任务现实执行硬否决规则定义材料_v2 形成规则(
    std::uint64_t 语义, std::vector<稳定编码> 来源,
    L2硬否决self约束_v2 self约束 = {},
    L2硬否决场景约束_v2 场景约束 = {},
    std::vector<任务动作作用范围> 范围 = {任务动作作用范围::现实改变}) {
    return {{稳定编码{语义}}, std::move(self约束), std::move(场景约束),
        {std::move(范围)}, std::move(来源)};
}

L2任务现实执行硬否决规则宇宙发布请求_v2 发布请求(
    std::uint64_t G0, std::uint64_t 幂等, std::uint64_t 版本,
    L2任务现实执行硬否决规则版本切换原因_v2 原因,
    std::vector<L2任务现实执行硬否决规则定义材料_v2> 规则组) {
    return {L2任务现实执行硬否决规则结构专属合同版本_v2,
        {L2结构合同版本, G0}, {幂等}, 版本, 版本, 原因,
        稳定编码{0x4844'4500'0000'0000ULL + 版本}, std::move(规则组)};
}

L2任务现实执行硬否决规则全集读取结果_v2 读取全集(
    测试上下文& c, L2存在身份 自我, std::vector<任务冻结动作项> 动作组) {
    const auto G0 = 当前代次(c.运行包);
    if (!G0) return {};
    return c.规则服务->读取任务现实执行硬否决规则全集_v2(
        {L2任务现实执行硬否决规则结构专属合同版本_v2,
            {L2结构合同版本, *G0}, 自我, std::move(动作组)});
}

任务现实执行硬否决请求 形成硬否决请求(L2存在身份 自我,
    const std::vector<任务冻结动作项>& 动作组, 任务现实影响分类 影响,
    L2任务现实执行硬否决规则全集读取结果_v2 全集,
    std::uint64_t G0) {
    return {任务现实执行硬否决合同版本,
        任务现实执行硬否决复核阶段::动作前最终复核, 自我,
        L2任务身份{稳定编码{0x5101}},
        L2任务轮次身份{稳定编码{0x5102}},
        L2任务筹办轮次权威记录身份{稳定编码{0x5103}},
        L2任务执行轮次身份{稳定编码{0x5104}},
        L2任务执行冻结身份{稳定编码{0x5105}}, 1,
        L2实例方法身份{稳定编码{0x5106}},
        L2方法身份{稳定编码{0x5107}}, 稳定编码{1}, 动作组,
        影响, std::move(全集), G0};
}

bool 退出于(const L1事实基座运行包& 运行包, 稳定编码 身份,
    std::uint64_t G1) {
    const auto 读取 = 运行包.读取服务().读取所有者范围历史事实(
        {L1所有者范围CRUD合同版本, 身份});
    if (读取.状态 != L1所有者范围读取状态::成功 || !读取.事实)
        return false;
    return std::visit([&](const auto& 事实) {
        return 事实.退出事实代次 && *事实.退出事实代次 == G1;
    }, *读取.事实);
}

} // namespace 海中鱼巣::任务现实执行硬否决规则宇宙测试内部

namespace 海中鱼巣 {

int 运行任务现实执行硬否决规则宇宙端到端测试() noexcept {
    using namespace 任务现实执行硬否决规则宇宙测试内部;
    try {
        {
            auto c = 建立上下文();
            const auto f = c ? 建立场景夹具(*c, 0x1000) : std::nullopt;
            if (!c || !f) return 失败("P00", "fixture");
            const auto 动作 = 形成动作(1, f->主体, 任务动作作用范围::现实改变,
                f->场景, 0x1100);
            const auto 未发布 = 读取全集(*c, f->自我, {动作});
            if (未发布.状态 != L2任务现实执行硬否决规则全集读取状态_v2::未发布
                || 未发布.成功() || 未发布.宇宙 || 未发布.完整集合见证
                || 未发布.本次正式读回截止 != 0)
                return 失败("P00", "unpublished universe must fail closed");
            通过("P00", "unpublished universe is distinct from an explicit empty universe");
        }

        {
            auto c = 建立上下文();
            const auto f = c ? 建立场景夹具(*c, 0x2000) : std::nullopt;
            const auto G0 = c ? 当前代次(c->运行包) : std::nullopt;
            if (!c || !f || !G0) return 失败("P01", "fixture");
            auto 请求 = 发布请求(*G0, 0x2100, 1,
                L2任务现实执行硬否决规则版本切换原因_v2::首次发布, {});
            const auto 首次 = c->规则服务->发布或读取任务现实执行硬否决规则宇宙_v2(请求);
            const auto 重复 = c->规则服务->发布或读取任务现实执行硬否决规则宇宙_v2(请求);
            auto 异义请求 = 请求;
            异义请求.切换依据事件 = {0x4844'4500'0000'0101ULL};
            const auto 异义 = c->规则服务->发布或读取任务现实执行硬否决规则宇宙_v2(异义请求);
            auto 跳版 = 发布请求(*当前代次(c->运行包), 0x2101, 3,
                L2任务现实执行硬否决规则版本切换原因_v2::替代, {});
            const auto 跳版结果 = c->规则服务->发布或读取任务现实执行硬否决规则宇宙_v2(跳版);
            if (!首次.成功()
                || 首次.状态 != L2任务现实执行硬否决规则宇宙发布状态_v2::已发布
                || !重复.成功()
                || 重复.状态 != L2任务现实执行硬否决规则宇宙发布状态_v2::精确重复
                || 异义.状态 != L2任务现实执行硬否决规则宇宙发布状态_v2::幂等冲突
                || 跳版结果.状态 != L2任务现实执行硬否决规则宇宙发布状态_v2::版本漂移)
                return 失败("P01", "publish replay conflict and version matrix");
            const auto 动作 = 形成动作(1, f->主体, 任务动作作用范围::现实改变,
                f->场景, 0x2200);
            const auto 全集 = 读取全集(*c, f->自我, {动作});
            if (!全集.成功() || !全集.宇宙 || !全集.完整集合见证
                || !全集.完整规则组.empty() || !全集.适用规则事实身份组.empty()
                || 全集.完整集合见证->声明成员数 != 0)
                return 失败("P01", "explicit empty universe readback");
            任务现实执行硬否决提供者 provider;
            const auto 裁决 = provider.复核任务现实执行硬否决(
                形成硬否决请求(f->自我, {动作},
                    任务现实影响分类::含现实改变, 全集,
                    全集.本次正式读回截止));
            if (裁决.状态 != 任务现实执行硬否决状态::未命中
                || !裁决.允许继续() || !裁决.结果形状有效())
                return 失败("P01", "empty universe hard-veto result");
            通过("P01", "explicit empty universe replay conflict and version gates");
        }

        {
            auto c = 建立上下文();
            const auto f = c ? 建立场景夹具(*c, 0x3000) : std::nullopt;
            const auto G0 = c ? 当前代次(c->运行包) : std::nullopt;
            if (!c || !f || !G0) return 失败("P02", "fixture");
            L2硬否决self约束_v2 self{L2硬否决约束模式::精确集合, {f->自我}};
            L2硬否决场景约束_v2 scene{L2硬否决约束模式::精确集合, {f->场景}};
            auto a = 形成规则(0x3001, {{0x3002}}, self, scene);
            auto b = 形成规则(0x3001, {{0x3003}}, self, scene);
            const auto 发布 = c->规则服务->发布或读取任务现实执行硬否决规则宇宙_v2(
                发布请求(*G0, 0x3100, 1,
                    L2任务现实执行硬否决规则版本切换原因_v2::首次发布,
                    {b, a}));
            if (!发布.成功() || 发布.完整规则组.size() != 1
                || 发布.完整规则组.front().定义.归一物理来源事实组
                    != std::vector<稳定编码>{{0x3002}, {0x3003}})
                return 失败("P02", "same-semantic source normalization");
            const auto 动作 = 形成动作(1, f->主体, 任务动作作用范围::现实改变,
                f->场景, 0x3200);
            const auto 全集 = 读取全集(*c, f->自我, {动作});
            if (!全集.成功() || 全集.适用规则事实身份组.size() != 1
                || !全集.完整集合见证
                || 全集.完整集合见证->覆盖项组.size() != 1
                || !全集.完整集合见证->覆盖项组.front().适用())
                return 失败("P02", "applicable rule coverage");
            任务现实执行硬否决提供者 provider;
            const auto 裁决 = provider.复核任务现实执行硬否决(
                形成硬否决请求(f->自我, {动作},
                    任务现实影响分类::含现实改变, 全集,
                    全集.本次正式读回截止));
            if (裁决.状态 != 任务现实执行硬否决状态::已命中
                || 裁决.允许继续() || 裁决.命中证据.size() != 1
                || !裁决.结果形状有效())
                return 失败("P02", "typed hard-veto hit");
            通过("P02", "same-semantic sources normalize and typed hard-veto hits");
        }

        {
            auto c = 建立上下文();
            const auto f = c ? 建立场景夹具(*c, 0x4000) : std::nullopt;
            const auto G0 = c ? 当前代次(c->运行包) : std::nullopt;
            if (!c || !f || !G0) return 失败("P03", "fixture");
            auto a = 形成规则(0x4001, {{0x4002}});
            auto b = 形成规则(0x4001, {{0x4003}},
                {L2硬否决约束模式::精确集合, {f->自我}});
            const auto 冲突 = c->规则服务->发布或读取任务现实执行硬否决规则宇宙_v2(
                发布请求(*G0, 0x4100, 1,
                    L2任务现实执行硬否决规则版本切换原因_v2::首次发布,
                    {a, b}));
            if (冲突.状态 != L2任务现实执行硬否决规则宇宙发布状态_v2::引用冲突
                || 冲突.成功() || 冲突.宇宙 || !冲突.完整规则组.empty())
                return 失败("P03", "same-semantic constraint conflict");
            通过("P03", "same-semantic different constraints fail without publishing");
        }

        {
            auto c = 建立上下文();
            const auto f = c ? 建立场景夹具(*c, 0x5000) : std::nullopt;
            const auto G0 = c ? 当前代次(c->运行包) : std::nullopt;
            if (!c || !f || !G0) return 失败("P04", "fixture");
            const auto v1 = c->规则服务->发布或读取任务现实执行硬否决规则宇宙_v2(
                发布请求(*G0, 0x5100, 1,
                    L2任务现实执行硬否决规则版本切换原因_v2::首次发布,
                    {形成规则(0x5001, {{0x5002}})}));
            const auto G1 = 当前代次(c->运行包);
            if (!v1.成功() || !v1.宇宙 || v1.完整规则组.size() != 1 || !G1)
                return 失败("P04", "version one publish");
            const auto v2 = c->规则服务->发布或读取任务现实执行硬否决规则宇宙_v2(
                发布请求(*G1, 0x5101, 2,
                    L2任务现实执行硬否决规则版本切换原因_v2::替代, {}));
            if (!v2.成功() || !v2.宇宙 || v2.宇宙->规则版本 != 2
                || v2.宇宙->成员集合版本 != 2
                || !退出于(c->运行包, v1.宇宙->宇宙.值,
                    v2.本次正式读回截止)
                || !退出于(c->运行包, v1.完整规则组.front().身份.值,
                    v2.本次正式读回截止))
                return 失败("P04", "old universe and rule historical exit boundary");
            const auto 动作 = 形成动作(1, f->主体, 任务动作作用范围::现实改变,
                f->场景, 0x5200);
            const auto 当前 = 读取全集(*c, f->自我, {动作});
            if (!当前.成功() || !当前.宇宙 || 当前.宇宙->规则版本 != 2
                || !当前.完整规则组.empty())
                return 失败("P04", "current version two closure");
            通过("P04", "successor version exits old closure and preserves history");
        }

        {
            auto c = 建立上下文();
            const auto f = c ? 建立场景夹具(*c, 0x6000) : std::nullopt;
            const auto G0 = c ? 当前代次(c->运行包) : std::nullopt;
            if (!c || !f || !G0) return 失败("P05", "fixture");
            const auto 发布 = c->规则服务->发布或读取任务现实执行硬否决规则宇宙_v2(
                发布请求(*G0, 0x6100, 1,
                    L2任务现实执行硬否决规则版本切换原因_v2::首次发布,
                    {形成规则(0x6001, {{0x6002}})}));
            if (!发布.成功()) return 失败("P05", "publish");
            const auto 缺场景动作 = 形成动作(1, f->主体,
                任务动作作用范围::现实改变, std::nullopt, 0x6200);
            const auto 不同场景动作1 = 形成动作(1, f->主体,
                任务动作作用范围::现实改变, f->场景, 0x6210);
            const auto 不同场景动作2 = 形成动作(2, f->其它存在,
                任务动作作用范围::现实改变, f->其它场景, 0x6220);
            const auto 缺场景 = 读取全集(*c, f->自我, {缺场景动作});
            const auto 不同场景 = 读取全集(
                *c, f->自我, {不同场景动作1, 不同场景动作2});
            const auto 非成员主体动作 = 形成动作(1, f->其它存在,
                任务动作作用范围::现实改变, f->场景, 0x6230);
            const auto 非成员主体 = 读取全集(*c, f->自我, {非成员主体动作});
            if (缺场景.状态
                    != L2任务现实执行硬否决规则全集读取状态_v2::场景范围不闭合
                || 不同场景.状态
                    != L2任务现实执行硬否决规则全集读取状态_v2::场景范围不闭合
                || 非成员主体.状态
                    != L2任务现实执行硬否决规则全集读取状态_v2::场景范围不闭合)
                return 失败("P05", "scene closure failures");
            通过("P05", "missing mixed and non-member scenes fail closed");
        }

        {
            auto c = 建立上下文();
            const auto f = c ? 建立场景夹具(*c, 0x7000) : std::nullopt;
            if (!c || !f) return 失败("P06", "fixture");
            const auto 动作 = 形成动作(1, f->主体, 任务动作作用范围::内部治理,
                std::nullopt, 0x7100);
            const auto G0 = 当前代次(c->运行包);
            if (!G0) return 失败("P06", "cutoff");
            任务现实执行硬否决提供者 provider;
            const auto 裁决 = provider.复核任务现实执行硬否决(
                形成硬否决请求(f->自我, {动作},
                    任务现实影响分类::零现实改变, {}, *G0));
            if (裁决.状态 != 任务现实执行硬否决状态::不适用
                || !裁决.允许继续() || !裁决.结果形状有效()
                || 裁决.安全读回见证)
                return 失败("P06", "zero reality hard-veto result");
            通过("P06", "internal governance does not require a rule universe");
        }

        {
            auto c = 建立上下文();
            const auto f = c ? 建立场景夹具(*c, 0x8000) : std::nullopt;
            const auto G0 = c ? 当前代次(c->运行包) : std::nullopt;
            if (!c || !f || !G0) return 失败("P07", "fixture");
            const auto 发布 = c->规则服务->发布或读取任务现实执行硬否决规则宇宙_v2(
                发布请求(*G0, 0x8100, 1,
                    L2任务现实执行硬否决规则版本切换原因_v2::首次发布,
                    {形成规则(0x8001, {{0x8002}}, {}, {},
                        {任务动作作用范围::内部治理})}));
            const auto 动作 = 形成动作(1, f->主体, 任务动作作用范围::现实改变,
                f->场景, 0x8200);
            const auto 全集 = 读取全集(*c, f->自我, {动作});
            if (!发布.成功() || !全集.成功()
                || !全集.适用规则事实身份组.empty()
                || !全集.完整集合见证
                || 全集.完整集合见证->覆盖项组.size() != 1
                || 全集.完整集合见证->覆盖项组.front().动作作用范围包含)
                return 失败("P07", "non-applicable scope coverage");
            auto 破损全集 = 全集;
           破损全集.完整集合见证->声明成员数 = 2;
            任务现实执行硬否决提供者 provider;
            const auto 材料不足 = provider.复核任务现实执行硬否决(
                形成硬否决请求(f->自我, {动作},
                    任务现实影响分类::含现实改变, std::move(破损全集),
                    全集.本次正式读回截止));
            if (材料不足.状态 != 任务现实执行硬否决状态::材料不足
                || 材料不足.允许继续() || !材料不足.结果形状有效())
                return 失败("P07", "incomplete witness must not become empty applicable set");
            通过("P07", "non-applicable scope is proven and damaged witness fails closed");
        }

        {
            const auto 隔离根 = std::filesystem::path{L"D:\\TEMP"}
                / L"海中鱼巣"
                / L"INSTINCT-STAGE3-HARD-DENIAL-RULE-UNIVERSE"
                / (std::to_wstring(GetCurrentProcessId()) + L"-"
                    + std::to_wstring(GetTickCount64()));
            隔离根清理 清理{隔离根};
            const L1事实基座持久存储配置_v1 配置{
                L1事实基座持久恢复合同版本_v1, 隔离根};

            std::optional<场景夹具> f;
            std::optional<任务冻结动作项> 动作;
            L2任务现实执行硬否决规则宇宙发布请求_v2 v1请求{};
            L2任务现实执行硬否决规则宇宙发布请求_v2 v2请求{};
            L2任务现实执行硬否决规则宇宙发布结果_v2 v1首次{};
            L2任务现实执行硬否决规则宇宙发布结果_v2 v2首次{};

            {
                auto 首会话 = 建立L1事实基座持久运行包_v1(配置);
                if (!首会话.成功() || !首会话.运行包
                    || 首会话.恢复.状态
                        != L1事实基座持久恢复状态_v1::已建立空仓)
                    return 失败("P08", "persistent root creation");
                auto c = 建立上下文(std::move(*首会话.运行包));
                const auto 场景材料 = c ? 建立场景夹具(*c, 0x9000) : std::nullopt;
                const auto G0 = c ? 当前代次(c->运行包) : std::nullopt;
                if (!c || !场景材料 || !G0)
                    return 失败("P08", "first session fixture");
                f = *场景材料;
                动作 = 形成动作(1, f->主体, 任务动作作用范围::现实改变,
                    f->场景, 0x9100);
                v1请求 = 发布请求(*G0, 0x9200, 1,
                    L2任务现实执行硬否决规则版本切换原因_v2::首次发布,
                    {形成规则(0x9001, {{0x9002}},
                        {L2硬否决约束模式::精确集合, {f->自我}},
                        {L2硬否决约束模式::精确集合, {f->场景}})});
                v1首次 = c->规则服务
                    ->发布或读取任务现实执行硬否决规则宇宙_v2(v1请求);
                const auto G1 = 当前代次(c->运行包);
                if (!v1首次.成功() || !G1)
                    return 失败("P08", "first persistent version publish");
                v2请求 = 发布请求(*G1, 0x9201, 2,
                    L2任务现实执行硬否决规则版本切换原因_v2::替代,
                    {形成规则(0x9003, {{0x9004}},
                        {L2硬否决约束模式::精确集合, {f->自我}},
                        {L2硬否决约束模式::精确集合, {f->场景}})});
                v2首次 = c->规则服务
                    ->发布或读取任务现实执行硬否决规则宇宙_v2(v2请求);
                const auto 当前 = 读取全集(*c, f->自我, {*动作});
                if (!v2首次.成功() || !当前.成功() || !当前.宇宙
                    || 当前.宇宙->规则版本 != 2
                    || 当前.适用规则事实身份组.size() != 1)
                    return 失败("P08", "second persistent version publish");
            }

            {
                auto 后会话 = 建立L1事实基座持久运行包_v1(配置);
                if (!后会话.成功() || !后会话.运行包
                    || 后会话.恢复.状态 != L1事实基座持久恢复状态_v1::已恢复
                    || !后会话.恢复.恢复见证)
                    return 失败("P08", "persistent recovery");
                auto c = 建立上下文(std::move(*后会话.运行包));
                if (!c) return 失败("P08", "recovered service assembly");

                const auto v1历史 = c->规则服务
                    ->发布或读取任务现实执行硬否决规则宇宙_v2(v1请求);
                const auto v2当前重放 = c->规则服务
                    ->发布或读取任务现实执行硬否决规则宇宙_v2(v2请求);
                if (v1历史.状态
                        != L2任务现实执行硬否决规则宇宙发布状态_v2::精确重复
                    || !v1历史.成功() || v1历史.宇宙 != v1首次.宇宙
                    || v1历史.完整规则组 != v1首次.完整规则组
                    || v1历史.本次正式读回截止
                        != v1首次.本次正式读回截止)
                    return 失败("P08", "recovered historical version readback");
                if (v2当前重放.状态
                        != L2任务现实执行硬否决规则宇宙发布状态_v2::精确重复
                    || !v2当前重放.成功()
                    || v2当前重放.宇宙 != v2首次.宇宙
                    || v2当前重放.完整规则组 != v2首次.完整规则组
                    || v2当前重放.本次正式读回截止
                        != v2首次.本次正式读回截止)
                    return 失败("P08", "recovered current version replay");

                const auto 当前 = 读取全集(*c, f->自我, {*动作});
                if (!当前.成功() || 当前.宇宙 != v2首次.宇宙
                    || 当前.完整规则组 != v2首次.完整规则组
                    || 当前.适用规则事实身份组.size() != 1
                    || !当前.完整集合见证
                    || 当前.完整集合见证->规则版本 != 2)
                    return 失败("P08", "recovered current universe readback");
                任务现实执行硬否决提供者 provider;
                const auto 裁决 = provider.复核任务现实执行硬否决(
                    形成硬否决请求(f->自我, {*动作},
                        任务现实影响分类::含现实改变, 当前,
                        当前.本次正式读回截止));
                if (裁决.状态 != 任务现实执行硬否决状态::已命中
                    || 裁决.允许继续() || 裁决.命中证据.size() != 1
                    || !裁决.结果形状有效())
                    return 失败("P08", "recovered hard-veto result");
            }
            通过("P08", "persistent recovery preserves historical and current universes");
        }

        return 0;
    } catch (const std::exception& e) {
        std::cout << "[INSTINCT-HARD-DENIAL] EXCEPTION: " << e.what() << '\n';
        return 1;
    } catch (...) {
        std::cout << "[INSTINCT-HARD-DENIAL] UNKNOWN EXCEPTION\n";
        return 1;
    }
}

} // namespace 海中鱼巣
