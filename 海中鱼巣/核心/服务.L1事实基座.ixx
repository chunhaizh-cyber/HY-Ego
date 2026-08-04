module;

#include <cstdint>
#include <algorithm>
#include <optional>
#include <type_traits>
#include <variant>
#include <vector>
#include <utility>

#define L1_FACT_BASE_NO_INCLUDES

export module 海中鱼巣.核心.服务.L1事实基座;

export import 海中鱼巣.核心.仓库.L1事实基座;

export namespace 海中鱼巣 {

class L1事实基座服务 final {
public:
    L1事实基座服务() = default;
    L1事实基座服务(const L1事实基座服务&) = delete;
    L1事实基座服务& operator=(const L1事实基座服务&) = delete;

    L1写入结果 提交写集(const L1写集请求& 请求) { return 仓库_.提交(请求); }
    L1读取结果 读取当前节点(稳定编码 编码) const { return 仓库_.读取当前节点(编码); }
    L1读取结果 读取当前关系(稳定编码 编码) const { return 仓库_.读取当前关系(编码); }
    L1读取结果 读取当前值(稳定编码 编码) const { return 仓库_.读取当前值(编码); }
    L1属性读取结果 读取当前属性(稳定编码 节点, 稳定编码 属性类型) const { return 仓库_.读取当前属性(节点, 属性类型); }
    L1历史读取结果 读取历史事实(稳定编码 编码) const { return 仓库_.读取历史事实(编码); }
    L1审计读取结果 读取审计(写集幂等键 幂等键) const { return 仓库_.读取审计(幂等键); }
    L1完整快照结果 读取完整快照() const { return 仓库_.读取完整快照(); }
    L1恢复结果 建立恢复候选(const L1恢复材料& 材料, std::uint64_t 期望事实代次) { return 仓库_.建立恢复候选(材料, 期望事实代次); }
    L1恢复结果 确认恢复候选() { return 仓库_.确认恢复候选(); }
    L1恢复结果 撤销恢复候选() noexcept { return 仓库_.撤销恢复候选(); }

private:
    L1事实基座仓库 仓库_;
};

} // namespace 海中鱼巣
