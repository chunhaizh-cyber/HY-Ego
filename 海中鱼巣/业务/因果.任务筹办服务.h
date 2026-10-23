#pragma once
#include "因果.反向查询服务.h"
#include "../领域/因果筹办.只读合同.h"

namespace 海中鱼巣 {
class 新_方法类;
class 因果任务筹办服务 final : public 新因果筹办读取接口 {
public:
    因果任务筹办服务(const 因果反向查询服务&, const 新_因果类&,
        const 新_方法类&, const 概念_动态类&) noexcept;
    std::vector<新先天因果信息> 读取先天结果规则(稳定编码) const override;
    新因果筹办读回 逆推(const 新因果筹办请求&) const override;
private:
    const 因果反向查询服务& 反查_;
    const 新_因果类& 因果_;
    const 新_方法类& 方法_;
    const 概念_动态类& 动作概念_;
};
} // namespace 海中鱼巣
