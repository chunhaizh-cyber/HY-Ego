// 文件规则：基础信息业务服务只创建通用基础信息身份，不替代各基础信息原子业务服务。
module;

#include "../核心/句柄.h"

#include <cstdint>

export module 海中鱼巣.领域.服务.基础信息;

import 海中鱼巣.领域.数据操作.语素基础;

export namespace 海中鱼巣 {

struct 创建基础信息身份请求 {
    std::uint64_t 幂等主键 = 0;
};

class 基础信息业务服务 final {
public:
    explicit 基础信息业务服务(语素基础数据操作& 数据操作) : 数据操作_(数据操作) {}

    语义基础业务结果 创建基础信息身份(const 创建基础信息身份请求& 请求) const {
        if (请求.幂等主键 == 0) return {};
        return 数据操作_.创建基础信息(请求.幂等主键);
    }

    基础信息值式材料 读取基础信息(节点句柄 基础信息) const {
        if (!句柄有效(基础信息)) return {};
        return 数据操作_.读取基础信息(基础信息);
    }

    基础信息值式材料 读取主键基础信息(std::uint64_t 主键) const {
        if (主键 == 0) return {};
        return 数据操作_.读取主键基础信息(主键);
    }

private:
    语素基础数据操作& 数据操作_;
};

}
