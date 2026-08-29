module;

#include <cstdint>
#include <iostream>

export module 海中鱼巣.端到端测试.本能单完整秒被动维护;

import 海中鱼巣.领域.算法.本能单完整秒被动维护;

export namespace 海中鱼巣 {
int 运行本能单完整秒被动维护端到端测试() noexcept;
}

module :private;

namespace 海中鱼巣::本能单完整秒被动维护测试内部 {

inline constexpr std::int64_t L = 2'767'011'611'056'432'742;
inline constexpr std::int64_t H = 7'378'697'629'483'820'645;

int 失败(const char* 编号, const char* 步骤) noexcept {
    std::cout << "[INSTINCT-SINGLE-SECOND-MAINTENANCE][" << 编号
              << "] FAIL: " << 步骤 << '\n';
    return 1;
}
void 通过(const char* 编号, const char* 说明) {
    std::cout << "[INSTINCT-SINGLE-SECOND-MAINTENANCE][" << 编号
              << "] PASS: " << 说明 << '\n';
}
本能单完整秒被动维护请求_v1 基础请求() {
    本能单完整秒被动维护请求_v1 请求;
    请求.服务值前值 = 根值上界_v1 / 2;
    请求.生存安全根前值 = L;
    请求.低位阈值 = L;
    请求.高位阈值 = H;
    请求.安全根定义版本 = 1;
    return 请求;
}
bool 空失败(const 本能单完整秒被动维护结果_v1& 结果,
    本能单完整秒被动维护状态_v1 状态) {
    return 结果.状态 == 状态 && !结果.成功() && !结果.载荷;
}

} // namespace 海中鱼巣::本能单完整秒被动维护测试内部

namespace 海中鱼巣 {

int 运行本能单完整秒被动维护端到端测试() noexcept {
    using namespace 本能单完整秒被动维护测试内部;
    try {
        auto 错合同 = 基础请求(); 错合同.合同版本 = 0;
        auto 错版本 = 基础请求(); 错版本.服务时间维护规则版本 = 2;
        auto 错阈值 = 基础请求(); 错阈值.高位阈值 = 错阈值.低位阈值;
        auto 错需求 = 基础请求();
        错需求.需求裁决 = 服务维护需求裁决_v1::存在未满观察窗需求;
        if (!空失败(计算本能单完整秒被动维护候选_v1(错合同),
                本能单完整秒被动维护状态_v1::入口拒绝)
            || !空失败(计算本能单完整秒被动维护候选_v1(错版本),
                本能单完整秒被动维护状态_v1::版本漂移)
            || !空失败(计算本能单完整秒被动维护候选_v1(错阈值),
                本能单完整秒被动维护状态_v1::入口拒绝)
            || !空失败(计算本能单完整秒被动维护候选_v1(错需求),
                本能单完整秒被动维护状态_v1::入口拒绝))
            return 失败("P00", "entry, version and demand-shape guards");
        通过("P00", "entry, version and demand shapes fail closed");

        for (const auto 前值 : {std::int64_t{0}, std::int64_t{1},
                 根值上界_v1 / 2, 根值上界_v1}) {
            auto 请求 = 基础请求(); 请求.服务值前值 = 前值;
            const auto 结果 = 计算本能单完整秒被动维护候选_v1(请求);
            const auto 期望 = 前值 == 0 ? 0 : 前值 - 1;
            if (!结果.成功() || 结果.载荷->服务值后值 != 期望
                || 结果.载荷->服务实际减少 != 前值 - 期望)
                return 失败("P01", "no-demand saturated decrement");
        }
        auto 活动保护 = 基础请求();
        活动保护.服务值前值 = 1;
        活动保护.活动裁决 = 服务活动裁决_v1::存在;
        const auto 活动保护结果 = 计算本能单完整秒被动维护候选_v1(活动保护);
        if (!活动保护结果.成功() || 活动保护结果.载荷->服务值后值 != 1)
            return 失败("P01", "positive activity floor");
        通过("P01", "no-demand decrement and activity floor are exact");

        auto 等待 = 基础请求();
        等待.需求裁决 = 服务维护需求裁决_v1::存在未满观察窗需求;
        等待.有效未满足需求数量 = 1;
        等待.最长等待完整秒 = 0;
        const auto 等待零 = 计算本能单完整秒被动维护候选_v1(等待);
        等待.最长等待完整秒 = 服务观察窗完整秒_v1 - 1;
        const auto 等待末 = 计算本能单完整秒被动维护候选_v1(等待);
        const auto 期望末 = 服务基础衰减_v1
            + (服务基础衰减_v1 * (服务观察窗完整秒_v1 - 1))
                / 服务观察窗完整秒_v1;
        if (!等待零.成功() || 等待零.载荷->服务实际减少 != 服务基础衰减_v1
            || !等待末.成功() || 等待末.载荷->服务实际减少 != 期望末)
            return 失败("P02", "longest-wait floor formula");
        通过("P02", "longest-wait floor formula is exact");

        auto 门禁 = 基础请求();
        门禁.需求裁决 = 服务维护需求裁决_v1::三十天门禁已成立;
        门禁.本秒到期未满足事件数量 = 1;
        const auto 归零 = 计算本能单完整秒被动维护候选_v1(门禁);
        门禁.活动裁决 = 服务活动裁决_v1::存在;
        const auto 最低一 = 计算本能单完整秒被动维护候选_v1(门禁);
        门禁.服务值前值 = 0;
        const auto 活动零 = 计算本能单完整秒被动维护候选_v1(门禁);
        if (!归零.成功() || 归零.载荷->服务值后值 != 0
            || !最低一.成功() || 最低一.载荷->服务值后值 != 1
            || !活动零.成功() || 活动零.载荷->服务值后值 != 0
            || 活动零.载荷->服务分支
                != 服务被动维护分支_v1::三十天门禁活动零保持)
            return 失败("P03", "30-day target");
        通过("P03", "30-day gate yields exact 0/1 targets");

        const auto 比例 = [](std::int64_t 后值, std::int64_t 期望) {
            auto 请求 = 基础请求(); 请求.服务值前值 = 后值 + 1;
            const auto 结果 = 计算本能单完整秒被动维护候选_v1(请求);
            return 结果.成功() && 结果.载荷->服务值后值 == 后值
                && 结果.载荷->服务比例 == 期望;
        };
        if (!比例(1, 1) || !比例(服务比例单位_v1 - 1, 1)
            || !比例(服务比例单位_v1, 2))
            return 失败("P04", "service ratio boundaries");
        auto 满值 = 基础请求(); 满值.服务值前值 = 根值上界_v1;
        满值.活动裁决 = 服务活动裁决_v1::存在;
        const auto 满值结果 = 计算本能单完整秒被动维护候选_v1(满值);
        if (!满值结果.成功() || 满值结果.载荷->服务比例 != 99)
            return 失败("P04", "M ratio");
        通过("P04", "service ratio is 1..99 at exact boundaries");

        auto 终止 = 基础请求(); 终止.生存安全根前值 = 0;
        const auto 终止结果 = 计算本能单完整秒被动维护候选_v1(终止);
        auto 主动 = 基础请求(); 主动.生存安全根前值 = H + 10;
        主动.主动安全裁决 = 同秒主动安全裁决_v1::已改变A;
        const auto 主动结果 = 计算本能单完整秒被动维护候选_v1(主动);
        auto 休眠 = 基础请求();
        休眠.需求裁决 = 服务维护需求裁决_v1::三十天门禁已成立;
        休眠.本秒到期未满足事件数量 = 1;
        const auto 休眠结果 = 计算本能单完整秒被动维护候选_v1(休眠);
        if (!终止结果.成功() || 终止结果.载荷->生存安全根后值 != 0
            || !主动结果.成功()
            || 主动结果.载荷->生存安全根后值 != 主动.生存安全根前值
            || !休眠结果.成功() || 休眠结果.载荷->生存安全根后值 != 1)
            return 失败("P05", "A branch ordering");
        通过("P05", "termination, active safety and sleep are ordered");

        auto 低位 = 基础请求(); 低位.生存安全根前值 = L - 1;
        低位.低位回升门禁 = 安全低位回升门禁_v1::允许;
        const auto 允许 = 计算本能单完整秒被动维护候选_v1(低位);
        低位.低位回升门禁 = 安全低位回升门禁_v1::禁止;
        const auto 禁止 = 计算本能单完整秒被动维护候选_v1(低位);
        低位.低位回升门禁 = 安全低位回升门禁_v1::材料不足;
        const auto 缺口 = 计算本能单完整秒被动维护候选_v1(低位);
        auto 高位 = 基础请求(); 高位.生存安全根前值 = H + 1;
        const auto 高位结果 = 计算本能单完整秒被动维护候选_v1(高位);
        if (!允许.成功() || 允许.载荷->生存安全根后值 != L
            || !禁止.成功() || 禁止.载荷->生存安全根后值 != L - 1
            || !空失败(缺口, 本能单完整秒被动维护状态_v1::材料不足)
            || !高位结果.成功() || 高位结果.载荷->生存安全根后值 != H)
            return 失败("P06", "threshold and gate branches");
        通过("P06", "threshold and low-gate branches are exact");

        const auto 请求 = 基础请求();
        const auto 一 = 计算本能单完整秒被动维护候选_v1(请求);
        const auto 二 = 计算本能单完整秒被动维护候选_v1(请求);
        if (!一.成功() || 一 != 二
            || !本能单完整秒被动维护载荷完整_v1(*一.载荷))
            return 失败("P07", "deterministic result");
        通过("P07", "same input yields the same complete result");
        return 0;
    } catch (...) {
        return 失败("EX", "unhandled exception");
    }
}

} // namespace 海中鱼巣
