#pragma once

#include <array>
#include "../领域/概念_存在类.h"

namespace 海中鱼巣 {
class 新_需求类;
class 新_语言类;

// 只保存具名范围身份；定义与实例内容每次由各自owner读取。
class 正式信息参数读取服务 final : public 正式信息参数读取接口 {
public:
    正式信息参数读取服务(const 新_需求类&, const 新_语言类&,
        std::array<稳定编码,3> 需求范围, 新词典归类信息概念组) noexcept;
    正式信息参数读取结果 读取正式信息参数保护(稳定编码 信息, 稳定编码 范围) const noexcept override;
private:
    const 新_需求类& 需求_;
    const 新_语言类& 语言_;
    std::array<稳定编码,3> 需求范围_;
    新词典归类信息概念组 归类范围_;
};
} // namespace 海中鱼巣
