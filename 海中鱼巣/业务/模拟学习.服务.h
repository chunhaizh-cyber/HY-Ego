#pragma once
#include <functional>
#include <exception>
#include <string>
#include <map>
#include <memory>
#include <mutex>
#include "../领域/本能交互.运行接口.h"
#include "../领域/本能字段询问.运行接口.h"

namespace 海中鱼巣 {
class 新_方法类;
class 新_存在类;
class 概念_存在类;
struct 新方法信息;
class 概念_特征类;
class 概念_二次特征类;
class 动作方法定义只读提供者;
class 概念_动态类;
class 新_动态类;
class 模拟任务需求来源读取接口;
struct 新学习结束通知 final {
    稳定编码 方法节点;
};
enum class 新学习运行异常原因 : std::uint8_t {
    停止取消=1, 输入不合法, 资源不足, 结构不一致,
    本能运行异常, 发布结果未知, 内部错误, 规格未支持, 定义前提已变化
};
struct 新学习运行异常记录 final {
    稳定编码 方法节点;
    新学习运行异常原因 原因=新学习运行异常原因::内部错误;
    std::string 诊断;
};
class 新学习运行异常 final : public std::exception {
public:
    新学习运行异常(稳定编码 方法节点, 新学习运行异常原因 原因) noexcept;
    新学习运行异常(稳定编码 方法节点, 新学习运行异常原因 原因, std::string 诊断);
    const 新学习运行异常记录& 记录() const noexcept;
    const char* what() const noexcept override;
private:
    新学习运行异常记录 记录_;
};
class 模拟学习服务 final {
public:
    模拟学习服务(新_方法类&, 新_特征类&, 概念_特征类&, 概念_二次特征类&,
        const 模拟任务需求来源读取接口&) noexcept;
    新学习结束通知 学习(稳定编码 方法节点,
        const std::function<bool()>& 停止检查 = {});
    void 配置场景事实构成读取(const 新方法场景事实构成读取接口&);
    void 配置隔离动作定义(const 动作方法定义只读提供者&,新_动态类&,const 概念_动态类&);
    void 配置需求执行来源(稳定编码 自我存在,const 新_存在类&,const 概念_存在类&);
    void 配置字段询问语言(std::shared_ptr<const 新字段询问语言接口>,
        std::optional<新字段询问登记配置>);
private:
    void 学习有序本能(const 新方法信息&,const std::function<bool()>&,
        const std::map<std::uint32_t,稳定编码>& 原型选择={},
        std::optional<稳定编码> 执行分支={},bool 已选路径=false,
        std::optional<std::uint32_t> 消息方案={});
    新_方法类& 方法_;
    概念_特征类& 概念_;
    概念_二次特征类& 二次概念_;
    新_二次特征类 二次计算_;
    const 模拟任务需求来源读取接口& 业务来源_;
    const 新方法场景事实构成读取接口* 场景构成_=nullptr;
    const 动作方法定义只读提供者* 动作定义_=nullptr;
    新_动态类* 动态定义_=nullptr;
    const 概念_动态类* 动态概念_=nullptr;
    稳定编码 需求执行来源_{};
    const 新_存在类* 来源身份_=nullptr;
    const 概念_存在类* 存在概念_=nullptr;
    std::mutex 字段询问装配锁_;
    std::size_t 活动学习数_=0;
    std::shared_ptr<const 新字段询问语言接口> 字段询问语言_;
    std::optional<新字段询问登记配置> 字段询问配置_;
};
} // namespace 海中鱼巣
