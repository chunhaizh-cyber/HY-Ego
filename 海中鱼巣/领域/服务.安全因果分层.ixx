// 文件规则：本模块只编排不可变安全因果图来源和纯值分层算法，不读写仓库或生成身份。
module;

#include "../核心/句柄.h"

#include <new>
#include <optional>

export module 海中鱼巣.领域.服务.安全因果分层;

import 海中鱼巣.领域.协议.分层安全维护;
import 海中鱼巣.领域.算法.安全因果层级;

export namespace 海中鱼巣 {

class 安全直接因果图提供者 {
public:
    virtual ~安全直接因果图提供者() = default;
    virtual 安全因果图来源结果 读取安全直接因果图(
        const 安全因果图来源请求& 请求) const = 0;
};

class 安全因果分层服务 final {
public:
    explicit 安全因果分层服务(
        安全直接因果图提供者& 因果图提供者) noexcept
        : 因果图提供者_(因果图提供者) {
    }

    安全因果分层服务(const 安全因果分层服务&) = delete;
    安全因果分层服务& operator=(const 安全因果分层服务&) = delete;

    安全分层读取结果 读取安全因果分层(
        const 安全分层读取请求& 请求) const {
        try {
            if (!句柄有效(请求.自我)
                || !句柄有效(请求.来源因果信息)
                || !句柄有效(请求.目标安全结果)
                || !句柄有效(请求.适用场景)
                || 请求.期望图版本 == 0
                || 请求.事实截止版本 == 0) {
                return {安全结果分类::入口拒绝, std::nullopt};
            }
            const 安全因果图来源请求 来源请求{
                请求.自我,
                请求.目标安全结果,
                请求.适用场景,
                请求.期望图版本,
                请求.事实截止版本};
            const auto 来源结果 = 因果图提供者_.读取安全直接因果图(来源请求);
            if (来源结果.结果分类 != 安全结果分类::已形成) {
                return {来源结果.结果分类, std::nullopt};
            }
            if (!来源结果.载荷) {
                return {安全结果分类::内部不一致, std::nullopt};
            }
            const auto& 图 = 来源结果.载荷->不可变安全因果图快照;
            if (图.自我 != 请求.自我
                || 图.目标安全结果 != 请求.目标安全结果
                || 图.适用场景 != 请求.适用场景) {
                return {安全结果分类::内部不一致, std::nullopt};
            }
            if (图.图版本 != 请求.期望图版本
                || 来源结果.载荷->来源版本 != 请求.期望图版本
                || 来源结果.载荷->事实截止版本 != 请求.事实截止版本) {
                return {安全结果分类::版本漂移, std::nullopt};
            }
            return 计算安全因果层级(图, 请求);
        } catch (const std::bad_alloc&) {
            return {安全结果分类::资源失败, std::nullopt};
        } catch (...) {
            return {安全结果分类::内部不一致, std::nullopt};
        }
    }

private:
    安全直接因果图提供者& 因果图提供者_;
};

}
