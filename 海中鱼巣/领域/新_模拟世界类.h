#pragma once
#include <memory>
#include "新_方法参数场景.h"
#include "服务值结算.本能合同.h"
#include "方法问题反馈.数据.h"

namespace 海中鱼巣 {
extern 稳定编码 模拟世界树根节点;
enum class 新模拟世界状态 : std::uint8_t { 已完成 = 1, 入口拒绝, 结构不一致, 资源失败 };
struct 新模拟世界建立结果 final {
    新模拟世界状态 状态 = 新模拟世界状态::入口拒绝;
    std::optional<稳定编码> 场景节点;
};
struct 新模拟服务结算样例读回 final {
    稳定编码 模拟根节点, 场景节点, 自我节点, 服务特征节点;
    新任务虚拟存在引用 来源任务;
    std::int64_t 服务当前值 = 0, 任务当前值 = 0;
    std::optional<稳定编码> 动作节点, 前状态节点, 后状态节点, 结果动态节点;
};
class 新_模拟世界类 final : public 新方法模拟运行接口, public 新服务值结算本能端口,
    public 新方法问题归档接口 {
public:
    explicit 新_模拟世界类(新_方法类&) noexcept;
    ~新_模拟世界类();
    新_模拟世界类(const 新_模拟世界类&) = delete;
    新_模拟世界类& operator=(const 新_模拟世界类&) = delete;
    新_模拟世界类(新_模拟世界类&&) = delete;
    新_模拟世界类& operator=(新_模拟世界类&&) = delete;
    新模拟世界建立结果 建立服务结算样例(稳定编码, const 新服务值结算登记材料&,
        std::int64_t 服务值, std::int64_t 任务状态) noexcept;
    std::optional<std::vector<新方法参数角色绑定>> 读取参数绑定() const noexcept;
    std::optional<新模拟服务结算样例读回> 读取样例() const noexcept;
    // 不可变的真实执行材料。来源编码按值保存，不对现实节点取得引用。
    // 这些归档不能被结算模拟写入口当作可变样例使用。
    新方法问题归档结果 保存方法问题(const 新方法问题反馈材料&) noexcept override;
    新方法问题归档读回结果 读取方法问题归档(稳定编码) const noexcept override;
    新方法问题归档读回结果 查询方法问题归档(稳定编码, 稳定编码) const noexcept override;
    稳定编码 模拟根节点() const noexcept override;
    bool 核验模拟参数(稳定编码, const std::vector<新方法参数角色绑定>&) const noexcept override;
    std::optional<新特征信息> 读取模拟特征(稳定编码) const noexcept override;
    std::optional<新特征值信息> 读取模拟特征值材料(稳定编码) const noexcept override;
    std::optional<新任务虚拟存在读回> 读取任务虚拟存在引用(const 新任务虚拟存在引用&) const noexcept override;
    新服务值结算本能端口* 获取服务值结算模拟端口() noexcept override;
    std::optional<新服务值结算登记材料> 读取登记材料() const noexcept override;
    新方法建立结果 确保方法登记(const 新方法知识建立请求&) noexcept override;
    新需求建立结果 确保首子需求(稳定编码) noexcept override;
    新服务值结算动作回执 建立普通动作(稳定编码, const 新任务虚拟存在引用&,
        稳定编码, 稳定编码, 稳定编码, std::int64_t) noexcept override;
    新服务值结算变化回执 更新普通结果(稳定编码, 稳定编码, 稳定编码,
        稳定编码, std::int64_t) noexcept override;
    void 报告并停止(std::string_view) noexcept override;
private:
    struct 实现;
    std::unique_ptr<实现> 实现_;
};
} // namespace 海中鱼巣
