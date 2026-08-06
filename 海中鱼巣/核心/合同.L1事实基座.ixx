module;

#include <cstdint>
#include <array>
#include <algorithm>
#include <optional>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#define L1_PUBLIC_FACT_NO_INCLUDES
#define L1_FACT_BASE_NO_INCLUDES

export module 海中鱼巣.核心.合同.L1事实基座;
export import 海中鱼巣.核心.合同.L1公共事实;

export {
#include "L1事实基座.数据.h"
}

export namespace 海中鱼巣 {

inline constexpr std::uint32_t L1事实基座合同版本 = 2;

struct L1幂等探测请求 final {
    std::uint32_t 合同版本 = L1事实基座合同版本;
    std::uint32_t 摘要合同版本 = L1幂等摘要合同版本;
    写集幂等键 幂等键;
    L1确定性摘要 请求意图摘要;
    L1领域意图凭证 领域意图凭证;
};
enum class L1幂等探测状态 : std::uint8_t {
    未找到 = 1, 同义已发布 = 2, 异义冲突 = 3,
    入口拒绝 = 4, 已隔离 = 5, 内部不一致 = 6
};
struct L1幂等首次结果副本 final {
    L1确定性摘要 首次执行证据摘要;
    std::uint64_t 首次发布事实代次 = 0;
    std::vector<std::pair<写集本地键, 稳定编码>> 首次新编码映射;
    L1通用发布后读回结果 首次完整读回;
    L1确定性摘要 确定性结果摘要;
    std::optional<L1失败见证身份> 发布后失败见证;
    bool 当前隔离 = false;
    std::uint32_t 确定性结果摘要版本 = L1确定性结果摘要版本;
    L1确定性摘要 领域结果见证摘要;
    std::optional<L1领域结果见证段> 领域结果见证;
};
struct L1幂等探测结果 final {
    L1幂等探测状态 状态 = L1幂等探测状态::入口拒绝;
    写集幂等键 幂等键;
    std::optional<L1幂等首次结果副本> 首次结果;
};

inline L1写入结果 映射L1幂等探测(const L1幂等探测结果& 探测) {
    if (探测.状态 == L1幂等探测状态::同义已发布 && 探测.首次结果) {
        return {L1写入状态::精确重复, 探测.首次结果->首次发布事实代次,
            探测.首次结果->首次新编码映射, 探测.首次结果->首次完整读回,
            探测.首次结果->发布后失败见证,
            探测.首次结果->确定性结果摘要版本,
            探测.首次结果->确定性结果摘要,
            探测.首次结果->领域结果见证};
    }
    if (探测.状态 == L1幂等探测状态::异义冲突) return {L1写入状态::幂等冲突};
    if (探测.状态 == L1幂等探测状态::已隔离
        || 探测.状态 == L1幂等探测状态::内部不一致) return {L1写入状态::内部不一致};
    return {};
}

struct L1事实读取请求 final {
    std::uint32_t 合同版本 = L1事实基座合同版本;
    稳定编码 编码;
};
struct L1属性读取请求 final {
    std::uint32_t 合同版本 = L1事实基座合同版本;
    稳定编码 节点;
    稳定编码 属性类型;
};
struct L1审计读取请求 final {
    std::uint32_t 合同版本 = L1事实基座合同版本;
    写集幂等键 幂等键;
};
struct L1完整快照读取请求 final {
    std::uint32_t 合同版本 = L1事实基座合同版本;
};
struct L1事实代次读取请求 final {
    std::uint32_t 合同版本 = L1事实基座合同版本;
    // 诊断责任：无适用错误分支；默认比较只比较纯值请求。
    friend bool operator==(const L1事实代次读取请求&,
        const L1事实代次读取请求&) = default;
};
struct L1恢复材料导出请求 final {
    std::uint32_t 合同版本 = L1事实基座合同版本;
};
struct L1恢复候选建立请求 final {
    std::uint32_t 合同版本 = L1事实基座合同版本;
    L1恢复材料 材料;
    std::uint64_t 期望事实代次 = 0;
};
struct L1恢复候选操作请求 final {
    std::uint32_t 合同版本 = L1事实基座合同版本;
};

} // namespace 海中鱼巣
