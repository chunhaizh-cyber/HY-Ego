#ifndef L1_REBUILDABLE_INDEX_NO_INCLUDES
#include <cstdint>
#include <variant>
#include <vector>
#include "L1事实基座.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t L1可重建索引合同版本 = 1;
inline constexpr std::uint32_t L1可重建索引规则版本 = 1;

struct L1节点种类索引键 final {
    节点种类 种类 = 节点种类::普通;
    friend bool operator==(const L1节点种类索引键&, const L1节点种类索引键&) = default;
};
struct L1关系源索引键 final {
    稳定编码 源节点;
    稳定编码 关系类型节点;
    friend bool operator==(const L1关系源索引键&, const L1关系源索引键&) = default;
};
struct L1关系目标索引键 final {
    稳定编码 目标节点;
    稳定编码 关系类型节点;
    friend bool operator==(const L1关系目标索引键&, const L1关系目标索引键&) = default;
};
struct L1当前属性值索引键 final {
    稳定编码 所属节点;
    稳定编码 属性类型节点;
    friend bool operator==(const L1当前属性值索引键&, const L1当前属性值索引键&) = default;
};

using L1结构索引键 = std::variant<L1节点种类索引键, L1关系源索引键,
    L1关系目标索引键, L1当前属性值索引键>;

struct L1索引重建请求 final {
    std::uint32_t 合同版本 = L1可重建索引合同版本;
    std::uint32_t 规则版本 = L1可重建索引规则版本;
    std::uint64_t 来源事实代次 = 0;
    L1完整快照 指定快照;
    friend bool operator==(const L1索引重建请求&, const L1索引重建请求&) = default;
};
struct L1索引查询请求 final {
    std::uint32_t 合同版本 = L1可重建索引合同版本;
    std::uint32_t 规则版本 = L1可重建索引规则版本;
    L1结构索引键 键;
    friend bool operator==(const L1索引查询请求&, const L1索引查询请求&) = default;
};
struct L1索引清空请求 final {
    std::uint32_t 合同版本 = L1可重建索引合同版本;
    friend bool operator==(const L1索引清空请求&, const L1索引清空请求&) = default;
};
struct L1索引失效请求 final {
    std::uint32_t 合同版本 = L1可重建索引合同版本;
    std::uint32_t 规则版本 = L1可重建索引规则版本;
    std::uint64_t 被失效来源事实代次 = 0;
    friend bool operator==(const L1索引失效请求&, const L1索引失效请求&) = default;
};

enum class L1索引维护状态 : std::uint8_t {
    重建已发布 = 1, 精确重复 = 2, 已清空 = 3, 已失效 = 4,
    入口拒绝 = 5, 事实代次漂移 = 6, 索引视图漂移 = 7,
    资源失败 = 8, 内部不一致 = 9
};
struct L1索引维护结果 final {
    L1索引维护状态 状态 = L1索引维护状态::入口拒绝;
    std::uint64_t 来源事实代次 = 0;
    std::uint32_t 规则版本 = 0;
    std::uint64_t 索引视图序号 = 0;
    friend bool operator==(const L1索引维护结果&, const L1索引维护结果&) = default;
};

enum class L1索引读取状态 : std::uint8_t {
    命中 = 1, 确定未命中 = 2, 索引不可用 = 3,
    入口拒绝 = 4, 资源失败 = 5, 内部不一致 = 6
};
struct L1索引读取结果 final {
    L1索引读取状态 状态 = L1索引读取状态::入口拒绝;
    std::uint64_t 来源事实代次 = 0;
    std::uint32_t 规则版本 = 0;
    std::vector<L1事实副本> 当前事实;
    friend bool operator==(const L1索引读取结果&, const L1索引读取结果&) = default;
};

} // namespace 海中鱼巣
