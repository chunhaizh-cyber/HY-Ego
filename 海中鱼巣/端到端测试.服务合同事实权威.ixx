module;

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <cstdint>
#include <exception>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <system_error>
#include <utility>

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

bool 进展空失败(const 服务进展完整集合读取结果_v1& 结果,
    服务合同事实权威读取状态_v1 状态) {
    return 结果.状态 == 状态 && !结果.成功()
        && 结果.完整进展事实组.empty() && !结果.完整集合见证
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

struct 隔离根清理 final {
    std::filesystem::path 根;
    ~隔离根清理() {
        std::error_code 错误;
        std::filesystem::remove_all(根, 错误);
    }
};

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
            服务进展完整集合读取结果_v1 进展期望;
            {
                auto 建立 = 建立L1事实基座持久运行包_v1(配置);
                if (!建立.成功() || !建立.运行包)
                    return 失败("P10", "first persistent package");
                auto 运行包 = std::move(*建立.运行包);
                auto 服务 = 建立服务(运行包);
                const auto 自我结果 = 服务
                    ? 服务->ARCH_建立验证样本_v1(
                        0x5343'0000'0000'0070ULL, 2, 2, false, false, 2)
                    : std::optional<L2存在身份>{};
                const auto G0 = 当前代次(运行包);
                if (!服务 || !自我结果 || !G0)
                    return 失败("P10", "persistent fixture");
                自我 = *自我结果;
                合同期望 = 服务->读取当前有效未满足服务合同完整集合_v1(
                    合同请求(自我, *G0));
                事件期望 = 服务->读取到期未满足事件完整集合_v1(
                    事件请求(自我, *G0));
                进展期望 = 服务->读取当前服务合同关联进展完整集合_v1(
                    进展请求(自我, *G0));
                if (!合同期望.成功() || !事件期望.成功() || !进展期望.成功())
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
                    || 服务->读取当前服务合同关联进展完整集合_v1(
                        进展请求(自我, *G0)) != 进展期望)
                    return 失败("P10", "recovery exact readback");
            }
            通过("P10", "persistent recovery preserves all three complete collections");
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
