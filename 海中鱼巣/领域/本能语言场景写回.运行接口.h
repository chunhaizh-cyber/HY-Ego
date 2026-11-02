#pragma once
#include "新_本能函数集.h"
namespace 海中鱼巣 {
class 方法参数场景;
// typed调用接缝，不把语言场景、采用节点或正式表达节点伪装成存在参数。
namespace 语言写回调用期 {
inline thread_local const 方法参数场景* 场景=nullptr;
inline thread_local const 新语言场景写回调用上下文* 上下文=nullptr;
}
class 语言场景写回上下文守卫 final {
    const 方法参数场景* 原场景_;
    const 新语言场景写回调用上下文* 原上下文_;
public:
    语言场景写回上下文守卫(const 方法参数场景& s,const 新语言场景写回调用上下文& c) noexcept
        :原场景_(语言写回调用期::场景),原上下文_(语言写回调用期::上下文){语言写回调用期::场景=&s;语言写回调用期::上下文=&c;}
    ~语言场景写回上下文守卫(){语言写回调用期::场景=原场景_;语言写回调用期::上下文=原上下文_;}
    语言场景写回上下文守卫(const 语言场景写回上下文守卫&)=delete;
    语言场景写回上下文守卫& operator=(const 语言场景写回上下文守卫&)=delete;
};
inline const 新语言场景写回调用上下文* 读取语言场景写回上下文(const 方法参数场景& s) noexcept {
    return 语言写回调用期::场景==&s ? 语言写回调用期::上下文 : nullptr;
}
class 新语言场景写回本能端口 {
public:
    virtual ~新语言场景写回本能端口()=default;
    virtual 新本能函数本次条件结果对 声明()=0;
    virtual 新本能函数本次条件结果对 执行(const 方法参数场景&,const 新语言场景写回调用上下文&)=0;
};
} // namespace 海中鱼巣
