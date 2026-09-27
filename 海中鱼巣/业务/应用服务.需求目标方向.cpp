#include "应用服务.需求目标方向.h"

#include <bit>
#include <stdexcept>

namespace 海中鱼巣 {

bool 需求目标方向结果::成功() const noexcept {
    const auto& request = 原请求;
    if (状态 != 需求目标方向状态::已计算 || !request.请求身份 ||
        !有效(request.需求.值) || !有效(request.当前F) ||
        request.要求结果位 < 1 || request.要求结果位 > 7 ||
        需求 != request.需求 || 当前F != request.当前F ||
        !有效(目标宿主) || !有效(目标F) || !有效(方向定义.结点) ||
        !计算 || !计算->成功() || 计算->请求身份 != request.请求身份 ||
        计算->上下文 != 二次计算上下文{} ||
        计算->根输出组.size() != std::popcount(request.要求结果位)) return false;

    std::size_t index = 0;
    for (unsigned role = 1; role <= 3; ++role) {
        if (!(request.要求结果位 & (1U << (role - 1)))) continue;
        const auto* root = std::get_if<二次已保存定义输出来源>(
            &计算->根输出组[index++].来源);
        if (!root || root->定义 != 方向定义 ||
            static_cast<unsigned>(root->输出角色) != role) return false;
    }
    return true;
}

需求目标方向应用服务::需求目标方向应用服务(
    const 需求类数据服务& demand,
    const 存在类数据服务& existence,
    const 特征类数据服务& feature,
    const 二次特征计算应用服务& calculation)
    : demand_(demand), existence_(existence), feature_(feature),
      calculation_(calculation) {
    if (!demand_.与存在服务同底座(existence_) ||
        !demand_.与特征服务同底座(feature_) ||
        !calculation_.与特征服务同底座(feature_))
        throw std::invalid_argument("需求方向数据绑定");
}

bool 需求目标方向应用服务::与需求服务同底座(
    const 需求类数据服务& demand) const noexcept {
    return demand_.与需求服务同底座(demand);
}

需求目标方向结果 需求目标方向应用服务::读取并计算(
    const 需求目标方向请求& request) const noexcept {
    需求目标方向结果 result;
    result.原请求 = request;
    result.需求 = request.需求;
    result.当前F = request.当前F;
    if (!request.请求身份 || !有效(request.需求.值) ||
        !有效(request.当前F) || request.要求结果位 < 1 ||
        request.要求结果位 > 7) return result;

    // 待实现：需求、宿主采用、目标F和方向定义尚无一次current-only组合读取。
    // 本入口不以多次独立读取伪造共同快照，也不写需求或特征事实。
    result.状态 = 需求目标方向状态::未实现;
    return result;
}

} // namespace 海中鱼巣
