#pragma once

#include "../适配/词典.离线数据源.h"

namespace 海中鱼巣 {

// 语言业务的外部材料查询入口。不建立词条、名称、概念或语言事实，
// 不把词面前缀候选裁决为唯一分词，也不把释义当作已理解的含义。
class 词典查询服务 final {
public:
    explicit 词典查询服务(std::shared_ptr<const 离线词典数据源>) noexcept;

    词典精确查询结果 查询词义(const 词典精确查询请求&) const noexcept;
    词典前缀查询结果 查询原文词面候选(const 词典前缀查询请求&) const noexcept;

private:
    std::shared_ptr<const 离线词典数据源> 数据源_;
};

} // namespace 海中鱼巣
