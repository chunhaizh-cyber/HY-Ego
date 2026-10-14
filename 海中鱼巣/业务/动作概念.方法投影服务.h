#pragma once
#include "../领域/动作概念.只读投影合同.h"

namespace 海中鱼巣 {
class 新_方法类;
class 新_任务类;
class 概念_存在类;
class 概念_特征类;
// 借用生命周期由组合根保证；只投影正式声明，不读取本次实参或写事实。
class 动作方法投影服务 final : public 动作方法定义只读提供者 {
public:
    动作方法投影服务(新_方法类&, const 新_任务类&,
        const 概念_存在类&, const 概念_特征类&) noexcept;
    动作概念定义 读取动作方法投影(const 动作方法投影请求&) const override;
private:
    新_方法类& 方法_;
    const 新_任务类& 任务_;
    const 概念_存在类& 存在概念_;
    const 概念_特征类& 特征概念_;
};
}
