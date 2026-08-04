module;

#include <cstdint>
#include <algorithm>
#include <optional>
#include <type_traits>
#include <variant>
#include <vector>
#include <utility>

export module 海中鱼巣.核心.服务.L1事实基座;

export import 海中鱼巣.核心.合同.L1事实基座;
import 海中鱼巣.核心.仓库.L1事实基座;

export namespace 海中鱼巣 {

class L1事实基座服务 final {
public:
    L1事实基座服务() = default;
    L1事实基座服务(const L1事实基座服务&) = delete;
    L1事实基座服务& operator=(const L1事实基座服务&) = delete;

    L1写入结果 提交写集(const L1写集请求& 请求) {
        if (请求.合同版本 != L1事实基座合同版本) return {};
        return 仓库_.提交(请求);
    }
    L1读取结果 读取当前节点(const L1事实读取请求& 请求) const {
        if (请求.合同版本 != L1事实基座合同版本) return {};
        return 仓库_.读取当前节点(请求.编码);
    }
    L1读取结果 读取当前关系(const L1事实读取请求& 请求) const {
        if (请求.合同版本 != L1事实基座合同版本) return {};
        return 仓库_.读取当前关系(请求.编码);
    }
    L1读取结果 读取当前值(const L1事实读取请求& 请求) const {
        if (请求.合同版本 != L1事实基座合同版本) return {};
        return 仓库_.读取当前值(请求.编码);
    }
    L1属性读取结果 读取当前属性(const L1属性读取请求& 请求) const {
        if (请求.合同版本 != L1事实基座合同版本) return {};
        return 仓库_.读取当前属性(请求.节点, 请求.属性类型);
    }
    L1历史读取结果 读取历史事实(const L1事实读取请求& 请求) const {
        if (请求.合同版本 != L1事实基座合同版本) return {};
        return 仓库_.读取历史事实(请求.编码);
    }
    L1审计读取结果 读取审计(const L1审计读取请求& 请求) const {
        if (请求.合同版本 != L1事实基座合同版本) return {};
        return 仓库_.读取审计(请求.幂等键);
    }
    L1完整快照结果 读取完整快照(const L1完整快照读取请求& 请求) const {
        if (请求.合同版本 != L1事实基座合同版本) return {};
        return 仓库_.读取完整快照();
    }
    L1恢复结果 建立恢复候选(const L1恢复候选建立请求& 请求) {
        if (请求.合同版本 != L1事实基座合同版本) return {};
        return 仓库_.建立恢复候选(请求.材料, 请求.期望事实代次);
    }
    L1恢复结果 确认恢复候选(const L1恢复候选操作请求& 请求) {
        if (请求.合同版本 != L1事实基座合同版本) return {};
        return 仓库_.确认恢复候选();
    }
    L1恢复结果 撤销恢复候选(const L1恢复候选操作请求& 请求) noexcept {
        if (请求.合同版本 != L1事实基座合同版本) return {};
        return 仓库_.撤销恢复候选();
    }

private:
    L1事实基座仓库 仓库_;
};

} // namespace 海中鱼巣
