// 文件规则：二次特征业务服务只裁决长期组成项和有序请求，不接触仓库或事务能力。
module;

#include "../核心/句柄.h"

#include <cstdint>
#include <vector>

export module 海中鱼巣.领域.服务.二次特征;

import 海中鱼巣.领域.数据操作.特征体系;

export namespace 海中鱼巣 {

struct 创建组合二次特征请求 {
    std::uint64_t 幂等主键 = 0;
    std::vector<节点句柄> 有序组成项组;
};

class 二次特征业务服务 final {
public:
    explicit 二次特征业务服务(特征体系数据操作& 数据操作) : 数据操作_(数据操作) {}

    特征体系业务结果 创建组合二次特征(
        const 创建组合二次特征请求& 请求) const {
        if (请求.幂等主键 == 0 || 请求.有序组成项组.empty()) return {};
        for (std::size_t 左 = 0; 左 < 请求.有序组成项组.size(); ++左) {
            const auto 组成项 = 请求.有序组成项组[左];
            if (!句柄有效(组成项)) return {};
            for (std::size_t 右 = 左 + 1; 右 < 请求.有序组成项组.size(); ++右) {
                if (组成项 == 请求.有序组成项组[右]) return {};
            }
            const auto 身份 = 数据操作_.读取节点身份(组成项);
            if (!身份.当前可读()) return 映射读取失败(身份.状态);
            if (!允许长期组成项(身份.类型)) return {};
        }
        return 数据操作_.创建组合二次特征(请求.幂等主键, 请求.有序组成项组);
    }

    二次特征值式材料 读取二次特征(节点句柄 二次特征) const {
        if (!句柄有效(二次特征)) return {};
        return 数据操作_.读取二次特征(二次特征);
    }

    二次特征值式材料 读取主键二次特征(std::uint64_t 主键) const {
        if (主键 == 0) return {};
        return 数据操作_.读取主键二次特征(主键);
    }

private:
    static bool 允许长期组成项(节点类型 类型) noexcept {
        return 类型 == 节点类型::基础信息 || 类型 == 节点类型::存在
            || 类型 == 节点类型::场景 || 类型 == 节点类型::特征
            || 类型 == 节点类型::二次特征 || 类型 == 节点类型::因果引用;
    }

    static 特征体系业务结果 映射读取失败(特征体系读取状态 状态) noexcept {
        switch (状态) {
        case 特征体系读取状态::许可拒绝:
            return {特征体系业务状态::许可拒绝};
        case 特征体系读取状态::版本漂移:
            return {特征体系业务状态::版本漂移};
        case 特征体系读取状态::内部不一致:
            return {特征体系业务状态::内部不一致};
        default:
            return {特征体系业务状态::入口拒绝};
        }
    }

    特征体系数据操作& 数据操作_;
};

}
