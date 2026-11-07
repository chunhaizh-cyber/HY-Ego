#pragma once

#include <memory>
#include "../领域/本能字段询问.运行接口.h"
#include "../领域/新_本能函数集.h"

namespace 海中鱼巣 {
class 新_世界树类; class 新_外设世界树类; class 新_方法类;
class 新_需求类; class 新_存在类; class 新_特征类; class 新_动态类;
class 概念_存在类; class 概念_特征类;
class 新_语言类; class 新_自然语言世界树类;
新本能函数本次条件结果对 执行指定需求字段询问核心(
    const 方法参数场景&,新本能字段询问运行接口&);

// 只实现问题入列，不等待、解释回复或写回需求。
class 字段询问本能运行服务 final : public 新本能字段询问运行接口,
    public 新字段询问登记端口 {
public:
    字段询问本能运行服务(新_世界树类&,新_外设世界树类&,新_方法类&,
        新_需求类&,新_存在类&,新_特征类&,新_动态类&,
        概念_存在类&,概念_特征类&,新_语言类&,新_自然语言世界树类&,
        std::shared_ptr<const 新字段询问语言接口>) noexcept;
    ~字段询问本能运行服务();
    bool 初始化(const 新字段询问登记配置&) noexcept;
    void 关闭() noexcept;
    std::optional<新字段询问登记配置> 读取登记材料() const noexcept override;
    新方法建立结果 确保方法登记(const 新方法知识建立请求&) noexcept override;
    新字段询问材料读回 读取询问材料(const 方法参数场景&) const override;
    新字段询问文字结果 生成询问文字(const 新正式字段询问目标&) const override;
    bool 已请求停止() const noexcept override;
    稳定编码 建立输出动作(const 方法参数场景&,const 新字段询问材料&) override;
    新出站表达留档状态 留档出站具体表达(const 方法参数场景&,
        const 新字段询问材料&,稳定编码,const 新字段询问文字结果&) override;
    新字段询问入列读回 提交询问输出(const 方法参数场景&,
        const 新字段询问材料&,稳定编码,std::string_view) override;

private:
    struct 实现;
    std::unique_ptr<实现> 实现_;
};
} // namespace 海中鱼巣
