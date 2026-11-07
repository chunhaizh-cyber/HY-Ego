#pragma once
#include <string>
#include "../领域/新_语言类.h"
namespace 海中鱼巣 {
// 整批预检后发布并逐条读回；只读重载不初始化、不修补。工程异常向装配上送。
void 初始化表达组织知识(const std::string&,新_语言类&);
void 读取表达组织知识种子(const std::string&,const 新_语言类&);
} // namespace 海中鱼巣
