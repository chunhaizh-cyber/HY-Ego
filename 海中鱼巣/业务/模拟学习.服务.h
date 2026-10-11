#pragma once
#include <functional>
#include "../领域/服务值结算.本能合同.h"

namespace 海中鱼巣 {
class 新_方法类;
class 概念_特征类;
enum class 新模拟学习状态 : std::uint8_t {
    已形成知识 = 1, 精确重复, 输入不支持, 方法不可读,
    样例失败, 发布失败, 资源失败, 已取消, 发布结果未知, 同条件结果冲突
};
struct 新模拟学习结果 final {
    新模拟学习状态 状态 = 新模拟学习状态::输入不支持;
    稳定编码 方法节点;
    std::optional<稳定编码> 模拟场景节点, 配对节点;
};
class 模拟学习服务 final {
public:
    模拟学习服务(新_方法类&, 概念_特征类&, const 新服务值结算本能端口&) noexcept;
    新模拟学习结果 学习(稳定编码 方法节点,
        const std::function<bool()>& 停止检查 = {}) noexcept;
private:
    新_方法类& 方法_;
    概念_特征类& 概念_;
    const 新服务值结算本能端口& 登记读取_;
};
} // namespace 海中鱼巣
