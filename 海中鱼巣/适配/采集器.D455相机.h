// 文件规则：D455 相机采集器是唯一 RealSense SDK 边界；导出接口不得暴露 SDK 类型或指向 SDK 缓冲区的视图。
#pragma once

#include <memory>
#include "协议.D455采样材料.h"

namespace 海中鱼巣 {

std::unique_ptr<D455帧来源> 创建并打开D455相机采集器(
    D455采集配置 配置,
    D455操作结果& 打开结果);

} // namespace 海中鱼巣
