#pragma once
#include "../领域/本能语言场景写回.运行接口.h"
#include "转换.语言场景基础.h"
#include <memory>
namespace 海中鱼巣 {
class 新_世界树类;class 新_外设世界树类;class 新_自然语言世界树类;
class 新_方法类;class 新_存在类;class 概念_存在类;class 概念_特征类;class 新_场景类;
// 只做值式形状投影，唯一解释及来源保护仍由调用方交付；不自行绑定真实对象。
std::optional<新语言场景主张内容> 投影语言场景主张(
    const 语言场景基础含义&,std::vector<新语言场景主张对象> 逐出现对象);
class 语言场景写回本能运行服务 final : public 新语言场景写回本能端口 {
public:
    语言场景写回本能运行服务(新_世界树类&,新_外设世界树类&,
        新_自然语言世界树类&,新_方法类&,稳定编码 通用存在概念);
    ~语言场景写回本能运行服务();
    void 初始化();
    void 关闭() noexcept;
    新本能函数本次条件结果对 声明() override;
    新本能函数本次条件结果对 执行(const 方法参数场景&,const 新语言场景写回调用上下文&) override;
    // 供未来语言采用/task消费者使用；通过正式方法调用本能，不直接调用写入owner。
    新本能函数本次条件结果对 调用(const 新语言场景写回调用上下文&);
private:
    struct 实现;std::unique_ptr<实现> 实现_;
};
} // namespace 海中鱼巣
