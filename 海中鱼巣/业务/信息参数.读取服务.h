#pragma once

#include <array>
#include "../领域/概念_存在类.h"
#include "../领域/信息内容.数据.h"

namespace 海中鱼巣 {
class 新_需求类;
class 新_语言类;

class 新_特征类;

class 稳定信息对象身份组合读取服务 final : public 新信息对象身份读取接口 {
public:
    稳定信息对象身份组合读取服务(新_特征类&,
        const 新信息对象身份读取接口& 词典, const 新信息对象身份读取接口& 语言);
    ~稳定信息对象身份组合读取服务() override;
    稳定信息对象身份组合读取服务(const 稳定信息对象身份组合读取服务&) = delete;
    稳定信息对象身份组合读取服务& operator=(const 稳定信息对象身份组合读取服务&) = delete;
    新信息对象身份读取结果 读取信息对象身份(稳定编码) const noexcept override;
private:
    新_特征类& 特征_;
    const 新信息对象身份读取接口& 词典_;
    const 新信息对象身份读取接口& 语言_;
};

// 只保存具名范围身份；定义与实例内容每次由各自owner读取。
class 正式信息参数读取服务 final : public 正式信息参数读取接口 {
public:
    正式信息参数读取服务(const 新_需求类&, const 新_语言类&,
        std::array<稳定编码,3> 需求范围, 新词典归类信息概念组,
        稳定编码 语言分界范围 = {}) noexcept;
    正式信息参数读取结果 读取正式信息参数保护(稳定编码 信息, 稳定编码 范围) const noexcept override;
private:
    const 新_需求类& 需求_;
    const 新_语言类& 语言_;
    std::array<稳定编码,3> 需求范围_;
    新词典归类信息概念组 归类范围_;
    稳定编码 语言分界范围_;
};
} // namespace 海中鱼巣
