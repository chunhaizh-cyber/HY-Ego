#pragma once

#include "计算.二次特征.h"
#include "../领域/数据服务.需求类.h"

#include <cstdint>
#include <optional>

namespace 海中鱼巣 {

enum class 需求目标方向状态 : std::uint8_t {
    已计算=1, 入口拒绝=2, 需求未找到=3,
    目标宿主未找到=5, 当前F未找到=7, 目标F未找到=9,
    当前F不属于宿主=11, 目标F不属于宿主=12, 当前采用不匹配=13,
    方向定义未找到=14, 方向定义不匹配=16,
    方向来源不匹配=17, 方向计算失败=18,
    // 20、21 对应已经退出的旧技术状态，稳定数值不复用。
    资源失败=22, 内部不一致=23, 未实现=24
};

struct 需求目标方向请求 final {
    std::uint64_t 请求身份 = 0;
    需求类记录身份 需求;
    特征信息身份 当前F;
    std::uint8_t 要求结果位 = 0;
    friend bool operator==(const 需求目标方向请求&, const 需求目标方向请求&) = default;
};

struct 需求目标方向结果 final {
    需求目标方向状态 状态 = 需求目标方向状态::入口拒绝;
    需求目标方向请求 原请求;
    需求类记录身份 需求;
    稳定编码 目标宿主{};
    特征信息身份 当前F, 目标F;
    特征类定义身份 方向定义;
    std::optional<二次准确计算结果> 计算;
    bool 成功() const noexcept;
    friend bool operator==(const 需求目标方向结果&, const 需求目标方向结果&) = default;
};

class 需求目标方向应用服务 final {
public:
    需求目标方向应用服务(const 需求类数据服务&,
                           const 存在类数据服务&,
                           const 特征类数据服务&,
                           const 二次特征计算应用服务&);
    需求目标方向应用服务() = delete;
    需求目标方向应用服务(const 需求目标方向应用服务&) = delete;
    需求目标方向应用服务& operator=(const 需求目标方向应用服务&) = delete;
    需求目标方向应用服务(需求目标方向应用服务&&) = delete;
    需求目标方向应用服务& operator=(需求目标方向应用服务&&) = delete;

    bool 与需求服务同底座(const 需求类数据服务&) const noexcept;
    需求目标方向结果 读取并计算(const 需求目标方向请求&) const noexcept;

private:
    const 需求类数据服务& demand_;
    const 存在类数据服务& existence_;
    const 特征类数据服务& feature_;
    const 二次特征计算应用服务& calculation_;
};

} // namespace 海中鱼巣
