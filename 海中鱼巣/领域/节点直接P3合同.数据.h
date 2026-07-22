// 文件规则：节点直接 P3 合同公共值式头只定义跨叶 ABI、结果和错误材料；不得声明模块、仓库、会话、令牌或线程能力。
#pragma once

#include "../核心/句柄.h"

#include <cstdint>
#include <optional>
#include <vector>

namespace 海中鱼巣 {

inline constexpr std::uint32_t 节点直接P3合同版本 = 0x0002'0000;
inline constexpr std::uint16_t 节点直接P3结果ABI版本 = 1;
inline constexpr std::uint32_t 节点直接P3对象规则版本 = 1;

enum class 节点直接P3结果状态 : std::uint16_t {
    已完成 = 0,
    幂等同义 = 1,
    合法未找到 = 2,
    合法空组 = 3,
    请求拒绝 = 4,
    身份或类型拒绝 = 5,
    旧域或跨域拒绝 = 6,
    业务前置拒绝 = 7,
    许可竞争 = 8,
    版本漂移 = 9,
    同键异义 = 10,
    候选过期 = 11,
    读取版本漂移 = 12,
    内部逻辑错误 = 0x8000
};

enum class 节点直接P3内部错误阶段 : std::uint16_t {
    候选创建 = 1,
    节点读回 = 2,
    关系读回 = 3,
    记录读回 = 4,
    索引读回 = 5,
    确认 = 6,
    撤销 = 7,
    发布 = 8,
    跨域隔离 = 9,
    投影后验 = 10,
    并发规则 = 11,
    合同接线 = 12
};

struct 节点直接P3请求头 {
    std::uint32_t 合同版本 = 节点直接P3合同版本;
    std::uint64_t 请求编号 = 0;
    std::uint32_t 规则版本 = 节点直接P3对象规则版本;
    节点类型 对象类型 = 节点类型::未分类;
};

struct 节点直接P3内部错误材料 {
    节点直接P3内部错误阶段 阶段 = 节点直接P3内部错误阶段::合同接线;
    std::uint32_t 错误编号 = 0;
    节点类型 对象类型 = 节点类型::未分类;
    std::optional<节点句柄> 对象;
    std::optional<std::uint64_t> 预期版本;
    std::optional<std::uint64_t> 实际版本;
};

struct 节点直接P3结果头 {
    std::uint32_t 合同版本 = 节点直接P3合同版本;
    std::uint16_t 结果ABI版本 = 节点直接P3结果ABI版本;
    std::uint64_t 请求编号 = 0;
    节点直接P3结果状态 状态 = 节点直接P3结果状态::请求拒绝;
    节点类型 对象类型 = 节点类型::未分类;
    std::optional<节点句柄> 当前对象;
    std::optional<std::uint64_t> 当前版本;
    std::optional<节点直接P3内部错误材料> 内部错误;
};

}
