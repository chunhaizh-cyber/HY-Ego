#pragma once

#include "../领域/本能词条归类.运行接口.h"

namespace 海中鱼巣 {
class 新_方法类;
class 新_任务类;
class 新_场景类;
class 新_动态类;
class 概念_名称关系类;

struct 新词条归类执行依赖 final {
    新_方法类& 方法;
    新_任务类& 任务;
    概念_名称关系类& 名称;
    新_场景类& 场景;
    新_动态类& 动态;
};

class 词条归类本能运行服务 final : public 新词条归类登记端口,
    public 新本能词条归类任务参数工厂 {
public:
    词条归类本能运行服务(新词条归类执行依赖,新词条归类登记配置);
    ~词条归类本能运行服务();
    词条归类本能运行服务(const 词条归类本能运行服务&) = delete;
    词条归类本能运行服务& operator=(const 词条归类本能运行服务&) = delete;
    std::optional<新词条归类登记配置> 读取登记材料() const noexcept override;
    新方法建立结果 确保方法登记(const 新方法知识建立请求&) noexcept override;
    bool 支持任务方法(稳定编码) const noexcept override;
    新词条归类任务参数场景结果 建立任务参数场景(const 新本能词条归类任务参数请求&,
        const 新内部治理对象读取接口*,std::optional<新方法任务成员绑定>) override;
    新词条归类任务参数场景结果 取得任务续接参数场景(稳定编码) override;
    bool 关闭() noexcept;
private:
    struct 实现;
    std::shared_ptr<实现> 实现_;
};
} // namespace 海中鱼巣
