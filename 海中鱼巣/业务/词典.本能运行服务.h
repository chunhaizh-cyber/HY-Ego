#pragma once

#include "新_世界树类.h"
#include "../领域/本能词典.运行接口.h"

namespace 海中鱼巣 {

class 离线词典数据源;
class 新_外设世界树类;
class 新_任务管理线程类;

// 应用持有来源及运行服务；解除场景接收并等待调用退出后才能销毁所借用owner。
class 词典本能运行服务 final : public 新自我场景资源接收接口,
                              public 新词典登记端口 {
public:
    词典本能运行服务(新_世界树类&, 新_外设世界树类&, 新_方法类&,
        新_特征类&, 新_特征值类&, 新_任务管理线程类&);
    ~词典本能运行服务();
    词典本能运行服务(const 词典本能运行服务&) = delete;
    词典本能运行服务& operator=(const 词典本能运行服务&) = delete;

    // 仅装配期使用；空来源仍允许登记方法规格，不创建可用入口。
    void 初始化来源(const 新词典登记配置&, std::shared_ptr<const 离线词典数据源>);
    新词典运行状态 保障当前场景词典入口();
    void 接收自我场景就绪() noexcept override;
    void 关闭() noexcept;
    std::optional<新词典登记配置> 读取登记材料() const noexcept override;
    新方法建立结果 确保方法登记(const 新方法知识建立请求&) noexcept override;

private:
    struct 实现;
    std::unique_ptr<实现> 实现_;
};

} // namespace 海中鱼巣
