#pragma once

#include "启动选项.数据.h"
#include "程序运行结果.数据.h"

namespace 海中鱼巣 {

启动选项解析结果 解析并验证启动选项(
    int 参数数量,
    char* const 参数组[]) noexcept;

int 映射进程退出码(
    const 启动选项解析结果& 解析,
    const 程序运行结果* 运行) noexcept;

} // namespace 海中鱼巣
