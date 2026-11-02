#pragma once
#include "因果.反向查询服务.h"
#include "../领域/因果筹办.只读合同.h"

namespace 海中鱼巣 {
class 新_方法类;
class 因果任务筹办服务 final : public 新因果筹办读取接口 {
public:
    因果任务筹办服务(const 因果反向查询服务&, const 新_因果类&,
        const 新_方法类&, const 概念_动态类&, const 概念_二次特征类&,
        const 概念_特征类&, const 新_特征值类&) noexcept;
    std::vector<新先天因果信息> 读取先天结果规则(稳定编码) const override;
    新因果筹办读回 逆推(const 新因果筹办请求&) const override;
    新来源前驱元数据读取结果 读取服务取得来源前驱元数据(
        const 新服务取得来源前驱配置&) const override;
    新来源前驱规则读取结果 读取来源前驱规则保护(稳定编码) const override;
    新来源前驱规则查询结果 查询来源前驱规则保护(
        稳定编码 服务任务类型,新来源前驱关系结果) const override;
private:
    新派生关系筹办读回 逆推派生源关系(const 新因果筹办请求&) const;
    新来源前驱路径读回 逆推服务取得来源前驱(const 新因果筹办请求&) const;
    const 因果反向查询服务& 反查_;
    const 新_因果类& 因果_;
    const 新_方法类& 方法_;
    const 概念_动态类& 动作概念_;
    const 概念_二次特征类& 二次概念_;
    const 概念_特征类& 特征概念_;
    const 新_特征值类& 特征值_;
};
} // namespace 海中鱼巣
