#pragma once

#include <atomic>
#include <memory>
#include "../领域/动作概念.只读投影合同.h"
#include "../领域/新_因果公共类型.h"

namespace 海中鱼巣 {
class 新_因果类;
class 新_动态类;
class 概念_存在类;
class 概念_特征类;

struct 内部治理先天初始化回执 final {
    新关系先天元数据 元数据;
    std::vector<稳定编码> 动作概念组;
    std::vector<新关系先天因果信息> 规则组;
};

struct 内部治理来源前驱初始化回执 final {
    新服务取得来源前驱元数据 元数据;
    std::vector<稳定编码> 动作概念组;
    std::vector<新来源前驱规则信息> 规则组;
};

// 只预置已实现的治理规律；不执行治理，不发表动作发生或任务结果。
class 内部治理先天因果初始化服务 final : public 关系治理动作只读提供者 {
public:
    内部治理先天因果初始化服务(新_因果类&, 新_动态类&,
        概念_存在类&, 概念_特征类&) noexcept;
    内部治理先天初始化回执 初始化(const 新关系先天元数据请求&);
    内部治理来源前驱初始化回执 初始化来源前驱(const 新服务取得来源前驱配置&);
    std::optional<动作概念定义> 读取关系治理动作定义(稳定编码) const override;
private:
    std::vector<动作概念定义> 构造动作目录(const 新关系先天元数据&) const;
    std::vector<新关系先天因果定义> 构造规则(const 新关系先天元数据&,
        const std::vector<稳定编码>&) const;
    void 核验初始化读回(const 内部治理先天初始化回执&) const;
    std::vector<动作概念定义> 构造来源前驱动作目录(const 新服务取得来源前驱元数据&) const;
    std::vector<新来源前驱规则定义> 构造来源前驱规则(
        const 新服务取得来源前驱元数据&, const std::vector<稳定编码>& 动作概念组) const;
    void 核验来源前驱初始化读回(const 内部治理来源前驱初始化回执&) const;
    新_因果类& 因果_;
    新_动态类& 动态_;
    概念_存在类& 存在概念_;
    概念_特征类& 特征概念_;
    std::atomic<std::shared_ptr<const std::vector<动作概念定义>>> 动作目录_;
};
}
