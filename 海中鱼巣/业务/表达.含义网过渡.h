#pragma once
// 计划2完成时删除：只承接“询问需求字段”形状；不扩展旧生成器语义。
#include "正式表达需求.h"
#include "../领域/语言含义图.数据.h"
namespace 海中鱼巣 {
std::optional<语言图含义材料> 含义网转旧含义图(const 新语言含义网&,const 正式表达需求&);
} // namespace 海中鱼巣
