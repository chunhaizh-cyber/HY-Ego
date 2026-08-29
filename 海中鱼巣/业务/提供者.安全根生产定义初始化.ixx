module;

#include <cstdint>
#include <new>
#include <optional>
#include <stdexcept>

#define INSTINCT_PRODUCTION_SAFETY_DEFINITION_INIT_NO_INCLUDES

export module 海中鱼巣.业务.提供者.安全根生产定义初始化;

import 海中鱼巣.业务.提供者.本能根运行初始化;
import 海中鱼巣.领域.服务.L2状态结构;
import 海中鱼巣.领域.服务.安全根定义与当前值;

export {
#include "安全根生产定义初始化.数据.h"
}

namespace 海中鱼巣 {

export class 安全根生产定义初始化提供者 final {
public:
    安全根生产定义初始化提供者(
        const L2状态结构服务& 状态服务,
        安全根定义与当前值服务& 定义服务) noexcept
        : 状态服务_(状态服务), 定义服务_(定义服务) {}

#if defined(ARCH_INSTINCT_PRODUCTION_SAFETY_DEFINITION_VALIDATION)
    void ARCH_注入下层读取状态(
        安全根定义读取状态_v1 状态, std::uint8_t 次数 = 1) noexcept {
        ARCH_读取状态_ = 状态;
        ARCH_读取剩余次数_ = 次数;
    }

    void ARCH_注入下层发布状态(
        安全根定义发布状态_v1 状态, std::uint8_t 次数 = 1) noexcept {
        ARCH_发布状态_ = 状态;
        ARCH_发布剩余次数_ = 次数;
    }
#endif

    安全根生产定义初始化结果_v1 初始化并读取安全根生产定义_v1(
        const 安全根生产定义初始化请求_v1& 请求) noexcept {
        const auto 失败 = [](安全根生产定义初始化状态_v1 状态) {
            安全根生产定义初始化结果_v1 结果;
            结果.状态 = 状态;
            return 结果;
        };
        if (请求.合同版本 != 安全根生产定义初始化合同版本_v1
            || !请求.本能根锚点.完整())
            return 失败(安全根生产定义初始化状态_v1::入口拒绝);
        try {
            bool 曾可能发布 = false;
            for (std::uint8_t 尝试 = 0; 尝试 != 2; ++尝试) {
                const auto G0 = 状态服务_.读取中性当前事实代次_v1();
                if (!G0)
                    return 失败(安全根生产定义初始化状态_v1::内部错误);
                const auto 当前 = 读取当前定义({
                    安全根定义合同版本_v1, {L2结构合同版本, *G0},
                    请求.本能根锚点});
                if (当前.成功()) {
                    if (!当前.定义
                        || !安全根生产定义匹配_v1(
                            *当前.定义, 请求.本能根锚点))
                        return 失败(安全根生产定义初始化状态_v1::定义冲突);
                    return 形成成功(
                        安全根生产定义初始化状态_v1::已恢复,
                        请求.本能根锚点);
                }
                if (当前.状态 == 安全根定义读取状态_v1::当前性漂移) {
                    if (尝试 == 0) continue;
                    return 失败(曾可能发布
                        ? 安全根生产定义初始化状态_v1::已可能发布
                        : 安全根生产定义初始化状态_v1::当前性漂移);
                }
                if (当前.状态 != 安全根定义读取状态_v1::未发布)
                    return 失败(映射读取失败(当前.状态));

                const 发布安全根定义请求_v1 发布请求{
                    安全根定义合同版本_v1,
                    {L2结构合同版本, *G0},
                    安全根生产定义发布幂等身份_v1,
                    请求.本能根锚点,
                    安全根生产低位阈值_v1,
                    安全根生产高位阈值_v1,
                    安全根生产定义版本_v1,
                    主动安全结算规则版本_v1,
                    被动服务维护规则版本_v1,
                    安全根生产定义来源_v1};
                const auto 发布 = 发布定义(发布请求);
                if (发布.成功()) {
                    if (!发布.定义
                        || !安全根生产定义匹配_v1(
                            *发布.定义, 请求.本能根锚点))
                        return 失败(安全根生产定义初始化状态_v1::定义冲突);
                    return 形成成功(
                        发布.状态 == 安全根定义发布状态_v1::已发布
                            ? 安全根生产定义初始化状态_v1::已发布
                            : 安全根生产定义初始化状态_v1::已恢复,
                        请求.本能根锚点);
                }
                if (发布.状态 == 安全根定义发布状态_v1::当前性漂移) {
                    if (尝试 == 0) continue;
                    return 失败(曾可能发布
                        ? 安全根生产定义初始化状态_v1::已可能发布
                        : 安全根生产定义初始化状态_v1::当前性漂移);
                }
                if (发布.状态 == 安全根定义发布状态_v1::已可能发布) {
                    曾可能发布 = true;
                    if (尝试 == 0) continue;
                    return 失败(安全根生产定义初始化状态_v1::已可能发布);
                }
                return 失败(映射发布失败(发布.状态));
            }
            return 失败(曾可能发布
                ? 安全根生产定义初始化状态_v1::已可能发布
                : 安全根生产定义初始化状态_v1::当前性漂移);
        } catch (const std::bad_alloc&) {
            return 失败(安全根生产定义初始化状态_v1::资源失败);
        } catch (const std::length_error&) {
            return 失败(安全根生产定义初始化状态_v1::资源失败);
        } catch (...) {
            return 失败(安全根生产定义初始化状态_v1::内部错误);
        }
    }

private:
    安全根生产定义初始化结果_v1 形成成功(
        安全根生产定义初始化状态_v1 状态,
        const 本能根运行锚点_v1& 锚点) const noexcept {
        const auto 失败 = [](安全根生产定义初始化状态_v1 失败状态) {
            安全根生产定义初始化结果_v1 结果;
            结果.状态 = 失败状态;
            return 结果;
        };
        const auto Gread = 状态服务_.读取中性当前事实代次_v1();
        if (!Gread)
            return 失败(安全根生产定义初始化状态_v1::内部错误);
        const auto 读回 = 读取当前定义({
            安全根定义合同版本_v1, {L2结构合同版本, *Gread}, 锚点});
        if (!读回.成功()) return 失败(映射读取失败(读回.状态));
        if (!读回.定义 || !安全根生产定义匹配_v1(*读回.定义, 锚点))
            return 失败(安全根生产定义初始化状态_v1::定义冲突);
        安全根生产定义初始化结果_v1 结果;
        结果.状态 = 状态;
        结果.定义 = *读回.定义;
        结果.本次正式读回截止 = *Gread;
        return 结果.成功() ? 结果
            : 失败(安全根生产定义初始化状态_v1::内部错误);
    }

    static 安全根生产定义初始化状态_v1 映射读取失败(
        安全根定义读取状态_v1 状态) noexcept {
        switch (状态) {
        case 安全根定义读取状态_v1::入口拒绝:
            return 安全根生产定义初始化状态_v1::入口拒绝;
        case 安全根定义读取状态_v1::许可拒绝:
            return 安全根生产定义初始化状态_v1::许可拒绝;
        case 安全根定义读取状态_v1::当前性漂移:
            return 安全根生产定义初始化状态_v1::当前性漂移;
        case 安全根定义读取状态_v1::引用冲突:
            return 安全根生产定义初始化状态_v1::定义冲突;
        case 安全根定义读取状态_v1::资源失败:
            return 安全根生产定义初始化状态_v1::资源失败;
        default:
            return 安全根生产定义初始化状态_v1::内部错误;
        }
    }

    static 安全根生产定义初始化状态_v1 映射发布失败(
        安全根定义发布状态_v1 状态) noexcept {
        switch (状态) {
        case 安全根定义发布状态_v1::入口拒绝:
            return 安全根生产定义初始化状态_v1::入口拒绝;
        case 安全根定义发布状态_v1::许可拒绝:
            return 安全根生产定义初始化状态_v1::许可拒绝;
        case 安全根定义发布状态_v1::当前性漂移:
            return 安全根生产定义初始化状态_v1::当前性漂移;
        case 安全根定义发布状态_v1::版本漂移:
        case 安全根定义发布状态_v1::幂等冲突:
        case 安全根定义发布状态_v1::引用冲突:
            return 安全根生产定义初始化状态_v1::定义冲突;
        case 安全根定义发布状态_v1::资源失败:
            return 安全根生产定义初始化状态_v1::资源失败;
        case 安全根定义发布状态_v1::已可能发布:
            return 安全根生产定义初始化状态_v1::已可能发布;
        default:
            return 安全根生产定义初始化状态_v1::内部错误;
        }
    }

    安全根定义读取结果_v1 读取当前定义(
        const 安全根定义读取请求_v1& 请求) const noexcept {
#if defined(ARCH_INSTINCT_PRODUCTION_SAFETY_DEFINITION_VALIDATION)
        if (ARCH_读取剩余次数_ != 0) {
            --ARCH_读取剩余次数_;
            安全根定义读取结果_v1 结果;
            结果.状态 = ARCH_读取状态_;
            return 结果;
        }
#endif
        return 定义服务_.读取当前安全根定义_v1(请求);
    }

    安全根定义发布结果_v1 发布定义(
        const 发布安全根定义请求_v1& 请求) noexcept {
#if defined(ARCH_INSTINCT_PRODUCTION_SAFETY_DEFINITION_VALIDATION)
        if (ARCH_发布剩余次数_ != 0) {
            --ARCH_发布剩余次数_;
            安全根定义发布结果_v1 结果;
            结果.状态 = ARCH_发布状态_;
            结果.幂等身份 = 请求.幂等身份;
            return 结果;
        }
#endif
        return 定义服务_.发布安全根定义_v1(请求);
    }

    const L2状态结构服务& 状态服务_;
    安全根定义与当前值服务& 定义服务_;
#if defined(ARCH_INSTINCT_PRODUCTION_SAFETY_DEFINITION_VALIDATION)
    mutable 安全根定义读取状态_v1 ARCH_读取状态_ =
        安全根定义读取状态_v1::内部错误;
    mutable std::uint8_t ARCH_读取剩余次数_ = 0;
    安全根定义发布状态_v1 ARCH_发布状态_ =
        安全根定义发布状态_v1::内部错误;
    std::uint8_t ARCH_发布剩余次数_ = 0;
#endif
};

} // namespace 海中鱼巣
