// 文件规则：特征值服务承载值节点入口；值裁决不得写入日志或显示文本。
#pragma once

#include "../核心/节点仓库.h"
#include "../核心/主信息仓库.h"

namespace 海中鱼巣 {

class 特征值服务 {
public:
    特征值服务(主信息仓库& 主信息, 节点仓库& 节点)
        : 主信息_(主信息), 节点_(节点) {
    }

    节点句柄 创建特征值() {
        return 节点_.创建节点(节点类型::特征值, 主信息_.创建主信息());
    }

    bool 写入I64值(节点句柄 特征值节点, std::int64_t 值) {
        const auto 记录 = 节点_.读取节点(特征值节点);
        if (!记录.has_value() || 记录->类型 != 节点类型::特征值) {
            return false;
        }
        return 主信息_.写入I64值(记录->主信息, 值);
    }

    std::optional<std::int64_t> 读取I64值(节点句柄 特征值节点) const {
        const auto 记录 = 节点_.读取节点(特征值节点);
        if (!记录.has_value() || 记录->类型 != 节点类型::特征值) {
            return std::nullopt;
        }
        return 主信息_.读取I64值(记录->主信息);
    }

private:
    主信息仓库& 主信息_;
    节点仓库& 节点_;
};

}
