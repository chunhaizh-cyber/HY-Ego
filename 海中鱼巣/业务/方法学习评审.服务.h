#pragma once
#include <optional>
#include "../领域/方法问题反馈.数据.h"
#include "../领域/新_方法公共类型.h"

namespace 海中鱼巣 {
class 新_方法类;

// TRIAL-LEARNING-REVIEW §5：独立评审结论；只有已授权时输出授权。
enum class 新学习评审结论 : std::uint8_t {
    已授权 = 1, 已发布, 来源不可重建, 依据不符, 方法入口已变化, 前提已变化, 结构不自足, 资源失败
};

// 5370 §6 独立评审：独立于学习服务构造，不持有学习侧状态；自行读取归档与方法，只形成授权或拒绝，不写方法知识。
class 方法学习评审服务 final {
public:
    方法学习评审服务(const 新方法问题归档接口&, const 新_方法类&) noexcept;
    // 诊断责任：向上送出（工程异常以异常抛出，由学习线程交自我线程记录）。
    新学习评审结论 评审尝试补全(const 新尝试补全候选& 候选,
        std::optional<新尝试补全授权>& 输出) const;
private:
    const 新方法问题归档接口& 归档_;
    const 新_方法类& 方法_;
};
}
