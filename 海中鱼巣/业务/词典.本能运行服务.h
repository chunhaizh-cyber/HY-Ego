#pragma once

#include "新_世界树类.h"
#include "../领域/本能词典.运行接口.h"

namespace 海中鱼巣 {

class 离线词典数据源;
class 新_外设世界树类;
class 新_任务管理线程类;
class 新_任务类;
class 新_需求类;
class 词典材料类;
class 新_模拟世界类;
struct 新词典任务执行依赖 final {
    新_任务类& 任务;
    新_需求类& 需求;
    新_词类& 词条;
    新_语言类& 语言;
    新_动态类& 动态;
    新_场景类& 场景;
    词典材料类& 材料;
    稳定编码 材料组织, 释义语言;
};

// 应用持有来源及运行服务；解除场景接收并等待调用退出后才能销毁所借用owner。
class 词典本能运行服务 final : public 新自我场景资源接收接口,
                              public 新词典登记端口,
                              public 新本能词典任务参数工厂 {
public:
    词典本能运行服务(新_世界树类&, 新_外设世界树类&, 新_方法类&,
        新_特征类&, 新_特征值类&, 新_任务管理线程类&, 新词典任务执行依赖);
    ~词典本能运行服务();
    词典本能运行服务(const 词典本能运行服务&) = delete;
    词典本能运行服务& operator=(const 词典本能运行服务&) = delete;

    // 仅装配期使用；空来源仍允许登记方法规格，不创建可用入口。
    void 初始化来源(const 新词典登记配置&, std::shared_ptr<const 离线词典数据源>, std::string 原包标识);
    新词典运行状态 保障当前场景词典入口();
    void 接收自我场景就绪() noexcept override;
    void 关闭() noexcept;
    std::optional<新词典登记配置> 读取登记材料() const noexcept override;
    新词典运行状态 配置隔离词典运行(const std::shared_ptr<新_模拟世界类>&,概念_特征类&);
    新方法建立结果 确保方法登记(const 新方法知识建立请求&) noexcept override;

    bool 支持任务方法(稳定编码) const noexcept override;
    新词典任务参数场景结果 建立任务参数场景(const 新本能词典任务参数请求&,
        const 新内部治理对象读取接口*, std::optional<新方法任务成员绑定>) override;
    新词典任务参数场景结果 取得任务续接参数场景(稳定编码) override;
    新词典推进收束读取结果 读取任务推进收束(稳定编码) const noexcept override;

private:
    struct 实现;
    std::shared_ptr<实现> 实现_;
};

} // namespace 海中鱼巣
