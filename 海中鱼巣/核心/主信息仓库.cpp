// 文件规则：主信息仓库只维护主信息记录生命周期；业务字段必须先经规范确认。
#include "主信息仓库.h"

namespace 海中鱼巣 {

主信息仓库::主信息仓库(std::uint64_t 仓库编号)
    : 仓库编号_(仓库编号 == 0 ? 1 : 仓库编号) {
}

主信息句柄 主信息仓库::创建主信息() {
    const std::uint64_t 编号 = 下个主信息编号_++;
    主信息记录 记录;
    记录.主信息编号 = 编号;
    记录.版本号 = 1;
    记录.状态 = 记录状态::有效;
    主信息表_.emplace(编号, 记录);
    return 主信息句柄{仓库编号_, 编号, 记录.版本号};
}

std::optional<主信息记录> 主信息仓库::读取主信息(主信息句柄 主信息) const {
    const auto 位置 = 主信息表_.find(主信息.主信息编号);
    if (位置 == 主信息表_.end()) {
        return std::nullopt;
    }
    const 主信息记录& 记录 = 位置->second;
    if (主信息.仓库编号 != 仓库编号_ || 主信息.版本号 != 记录.版本号 || 记录.状态 != 记录状态::有效) {
        return std::nullopt;
    }
    return 记录;
}

bool 主信息仓库::删除主信息(主信息句柄 主信息) {
    auto 位置 = 主信息表_.find(主信息.主信息编号);
    if (位置 == 主信息表_.end()) {
        return false;
    }
    主信息记录& 记录 = 位置->second;
    if (主信息.仓库编号 != 仓库编号_ || 主信息.版本号 != 记录.版本号 || 记录.状态 != 记录状态::有效) {
        return false;
    }
    记录.状态 = 记录状态::已删除;
    ++记录.版本号;
    return true;
}

bool 主信息仓库::主信息是否有效(主信息句柄 主信息) const {
    return 读取主信息(主信息).has_value();
}

bool 主信息仓库::写入I64值(主信息句柄 主信息, std::int64_t 值) {
    auto 位置 = 主信息表_.find(主信息.主信息编号);
    if (位置 == 主信息表_.end()) {
        return false;
    }
    主信息记录& 记录 = 位置->second;
    if (主信息.仓库编号 != 仓库编号_ || 主信息.版本号 != 记录.版本号 || 记录.状态 != 记录状态::有效) {
        return false;
    }
    if (记录.值容器.empty()) {
        记录.值容器.push_back(值);
    } else {
        记录.值容器[0] = 值;
    }
    return true;
}

std::optional<std::int64_t> 主信息仓库::读取I64值(主信息句柄 主信息) const {
    const auto 记录 = 读取主信息(主信息);
    if (!记录.has_value() || 记录->值容器.empty()) {
        return std::nullopt;
    }
    return 记录->值容器[0];
}

}
