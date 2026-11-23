#pragma once

#include <array>
#include "../领域/新_本能函数集.h"
#include "../领域/语言分界.数据.h"

namespace 海中鱼巣 {
class 新_语言类;
class 新_特征值类;
class 新_词类;
class 新_动态类;
class 新_场景类;
class 新_自然语言世界树类;
class 新_方法类;
class 新_任务类;

struct 新语言分界本能执行依赖 final {
    新_语言类& 语言;
    const 新_特征值类& 特征值;
    新_词类& 词条;
    新_动态类& 动态;
    新_场景类& 场景;
    新_自然语言世界树类& 语言世界;
    新_方法类& 方法;
    新_任务类& 任务;
};

class 语言分界本能运行服务 final : public 新本能语言分界运行接口 {
public:
    语言分界本能运行服务(新语言分界本能执行依赖, 稳定编码 范围, 稳定编码 自我范围,
        const std::array<稳定编码, 4>& 类型组) noexcept;
    新本能函数本次条件结果对 运行语言片段分词(const 方法参数场景&) override;
    新本能函数本次条件结果对 运行词条准入(const 方法参数场景&) override;

private:
    新本能函数本次条件结果对 登记方法(bool 准入, const 方法参数场景&);
    新本能函数本次条件结果对 执行分界分支(const 方法参数场景&, 新语言范围特征种类);
    bool 挂接并记录动作(std::optional<稳定编码> 当前场景, 稳定编码 任务, 稳定编码 执行, 稳定编码 动作);
    新语言分界本能执行依赖 依赖_;
    稳定编码 范围_, 自我范围_;
    std::array<稳定编码, 4> 类型组_;
};

} // namespace 海中鱼巣
