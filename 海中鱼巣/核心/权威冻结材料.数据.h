// 文件规则：本文件只定义权威状态冻结的稳定值式材料，不取得许可、不读取仓库、不执行编码或恢复。
#pragma once

#include "句柄.h"
#include "索引所有权.数据.h"

#include <cstdint>
#include <optional>
#include <vector>

namespace 海中鱼巣 {

enum class 仓库权威导出状态 : std::uint8_t {
    已形成 = 1,
    入口拒绝 = 2,
    资源失败 = 3,
    内部不一致 = 4
};

template<class 材料类型>
struct 仓库权威导出结果 {
    仓库权威导出状态 状态 = 仓库权威导出状态::入口拒绝;
    材料类型 材料;

    bool 成功() const noexcept { return 状态 == 仓库权威导出状态::已形成; }
};

struct 冻结主信息记录 {
    std::uint64_t 主信息编号 = 0;
    std::uint32_t 版本号 = 0;
    记录状态 状态 = 记录状态::无效;
    std::uint64_t 拓扑锚点编号 = 0;
    std::vector<std::optional<std::int64_t>> 值容器;
};

inline bool operator==(const 冻结主信息记录& 左, const 冻结主信息记录& 右) {
    return 左.主信息编号 == 右.主信息编号 && 左.版本号 == 右.版本号
        && 左.状态 == 右.状态 && 左.拓扑锚点编号 == 右.拓扑锚点编号
        && 左.值容器 == 右.值容器;
}

struct 主信息仓库权威材料 {
    std::uint64_t 仓库编号 = 0;
    std::uint64_t 下个主信息编号 = 0;
    std::vector<冻结主信息记录> 记录组;
};

inline bool operator==(const 主信息仓库权威材料& 左, const 主信息仓库权威材料& 右) {
    return 左.仓库编号 == 右.仓库编号 && 左.下个主信息编号 == 右.下个主信息编号
        && 左.记录组 == 右.记录组;
}

struct 冻结节点记录 {
    std::uint64_t 节点编号 = 0;
    节点类型 类型 = 节点类型::未分类;
    主信息句柄 主信息;
    std::uint32_t 版本号 = 0;
    记录状态 状态 = 记录状态::无效;
    std::uint64_t 创建序号 = 0;
};

inline bool operator==(const 冻结节点记录& 左, const 冻结节点记录& 右) {
    return 左.节点编号 == 右.节点编号 && 左.类型 == 右.类型
        && 左.主信息 == 右.主信息 && 左.版本号 == 右.版本号
        && 左.状态 == 右.状态 && 左.创建序号 == 右.创建序号;
}

struct 节点仓库权威材料 {
    std::uint64_t 仓库编号 = 0;
    std::uint64_t 下个节点编号 = 0;
    std::uint64_t 下个创建序号 = 0;
    std::vector<冻结节点记录> 记录组;
};

inline bool operator==(const 节点仓库权威材料& 左, const 节点仓库权威材料& 右) {
    return 左.仓库编号 == 右.仓库编号 && 左.下个节点编号 == 右.下个节点编号
        && 左.下个创建序号 == 右.下个创建序号 && 左.记录组 == 右.记录组;
}

struct 冻结关系记录 {
    std::uint64_t 关系编号 = 0;
    关系类型 类型 = 关系类型::普通父子;
    节点句柄 源节点;
    节点句柄 目标节点;
    std::int64_t 顺序号 = 0;
    std::uint32_t 版本号 = 0;
    记录状态 状态 = 记录状态::无效;
};

inline bool operator==(const 冻结关系记录& 左, const 冻结关系记录& 右) {
    return 左.关系编号 == 右.关系编号 && 左.类型 == 右.类型
        && 左.源节点 == 右.源节点 && 左.目标节点 == 右.目标节点
        && 左.顺序号 == 右.顺序号 && 左.版本号 == 右.版本号 && 左.状态 == 右.状态;
}

struct 关系仓库权威材料 {
    std::uint64_t 仓库编号 = 0;
    std::uint64_t 下个关系编号 = 0;
    std::vector<冻结关系记录> 记录组;
};

inline bool operator==(const 关系仓库权威材料& 左, const 关系仓库权威材料& 右) {
    return 左.仓库编号 == 右.仓库编号 && 左.下个关系编号 == 右.下个关系编号
        && 左.记录组 == 右.记录组;
}

struct 冻结索引绑定记录 {
    std::uint64_t 主键 = 0;
    节点句柄 节点;
    索引所有者声明 所有者声明 = 形成未知兼容索引所有者声明();
    std::uint32_t 探测序号 = 0;
};

inline bool operator==(const 冻结索引绑定记录& 左, const 冻结索引绑定记录& 右) {
    return 左.主键 == 右.主键 && 左.节点 == 右.节点
        && 左.所有者声明 == 右.所有者声明 && 左.探测序号 == 右.探测序号;
}

struct 冻结节点主键组 {
    std::uint64_t 节点编号 = 0;
    std::vector<std::uint64_t> 主键组;
};

inline bool operator==(const 冻结节点主键组& 左, const 冻结节点主键组& 右) {
    return 左.节点编号 == 右.节点编号 && 左.主键组 == 右.主键组;
}

struct 索引仓库权威材料 {
    std::uint64_t 节点仓库编号 = 0;
    std::vector<冻结索引绑定记录> 当前绑定组;
    std::vector<冻结索引绑定记录> 永久保留绑定组;
    std::vector<冻结节点主键组> 反向绑定组;
};

inline bool operator==(const 索引仓库权威材料& 左, const 索引仓库权威材料& 右) {
    return 左.节点仓库编号 == 右.节点仓库编号
        && 左.当前绑定组 == 右.当前绑定组
        && 左.永久保留绑定组 == 右.永久保留绑定组
        && 左.反向绑定组 == 右.反向绑定组;
}

struct 权威冻结材料 {
    主信息仓库权威材料 主信息;
    节点仓库权威材料 节点;
    关系仓库权威材料 关系;
    索引仓库权威材料 索引;
};

inline bool operator==(const 权威冻结材料& 左, const 权威冻结材料& 右) {
    return 左.主信息 == 右.主信息 && 左.节点 == 右.节点
        && 左.关系 == 右.关系 && 左.索引 == 右.索引;
}

}
