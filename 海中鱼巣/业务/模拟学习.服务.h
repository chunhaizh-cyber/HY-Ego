#pragma once
#include <functional>
#include <exception>
#include "../领域/服务值结算.本能合同.h"

namespace 海中鱼巣 {
class 新_方法类;
class 概念_特征类;
struct 新学习结束通知 final {
    稳定编码 方法节点;
};
enum class 新学习运行异常原因 : std::uint8_t {
    停止取消=1, 输入不合法, 资源不足, 结构不一致,
    本能运行异常, 发布结果未知, 内部错误
};
struct 新学习运行异常记录 final {
    稳定编码 方法节点;
    新学习运行异常原因 原因=新学习运行异常原因::内部错误;
};
class 新学习运行异常 final : public std::exception {
public:
    新学习运行异常(稳定编码 方法节点, 新学习运行异常原因 原因) noexcept;
    const 新学习运行异常记录& 记录() const noexcept;
    const char* what() const noexcept override;
private:
    新学习运行异常记录 记录_;
};
class 模拟学习服务 final {
public:
    模拟学习服务(新_方法类&, 概念_特征类&, const 新服务值结算本能端口&) noexcept;
    新学习结束通知 学习(稳定编码 方法节点,
        const std::function<bool()>& 停止检查 = {});
private:
    新_方法类& 方法_;
    概念_特征类& 概念_;
    const 新服务值结算本能端口& 登记读取_;
};
} // namespace 海中鱼巣
