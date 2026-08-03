// 文件规则：本模块只在共享许可内把六仓当前值投影为稳定身份，不取得写入能力。
module;

#include "节点直接结构合同.数据.h"

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <limits>
#include <new>
#include <optional>
#include <stdexcept>
#include <utility>

export module 海中鱼巣.核心.服务.节点直接结构;

import 海中鱼巣.核心.执行器.节点直接身份结构写入;
import 海中鱼巣.核心.仓库.正式关系;
import 海中鱼巣.核心.仓库.节点直接类型合同;
import 海中鱼巣.核心.仓库.节点直接类型化值;

namespace 海中鱼巣 {

bool 类型化值来源完整_(const 类型化值来源见证& 来源) noexcept {
    if (const auto* 节点 = std::get_if<节点稳定身份见证>(&来源)) {
        return 节点稳定身份见证完整(*节点);
    }
    if (const auto* 服务 = std::get_if<服务稳定身份>(&来源)) {
        return 服务稳定身份完整(*服务);
    }
    return false;
}

bool 类型化值材料完整_(const 类型化值材料& 材料) noexcept {
    if (const auto* 区间 = std::get_if<I64区间材料>(&材料)) {
        return 区间->下界 <= 区间->上界;
    }
    if (const auto* 身份组 = std::get_if<稳定身份有序组材料>(&材料)) {
        return std::all_of(身份组->项目组.begin(), 身份组->项目组.end(),
            节点稳定身份见证完整);
    }
    if (const auto* 引用 = std::get_if<独立材料引用>(&材料)) {
        return 稳定主键有效(引用->材料身份)
            && 引用->格式版本 != 0
            && 引用->字节数 != 0
            && std::any_of(引用->SHA256.begin(), 引用->SHA256.end(),
                [](std::uint8_t 值) noexcept { return 值 != 0; });
    }
    return true;
}

bool 类型化值记录完整_(const 类型化值读回& 值) noexcept {
    return 节点稳定身份见证完整(值.所属身份)
        && 类型合同稳定身份完整(值.类型合同身份)
        && 值.类型合同版本 != 0
        && 类型化值记录稳定身份完整(值.值记录身份)
        && 值.值记录版本 != 0
        && 类型化值来源完整_(值.来源)
        && 类型化值材料完整_(值.材料)
        && 值.首次发布代次 != 0
        && 值.当前状态发布代次 >= 值.首次发布代次;
}

} // namespace 海中鱼巣

export namespace 海中鱼巣 {

#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
struct 节点直接当前索引查询测试异常 {};

enum class 节点直接当前索引读取内部损坏测试种类 : std::uint8_t {
    节点目标权威读回缺失 = 1,
    关系目标权威读回缺失 = 2,
    关系端点见证不可形成 = 3
};
#endif

class 节点直接结构查询服务;

#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
bool 自检验证两级投影类型化值完整性(const 类型化值读回& 值) noexcept;
void 自检注入下一次两级投影资源失败(节点直接结构查询服务& 服务) noexcept;
#endif

class 节点直接结构查询服务 final {
public:
    节点直接结构查询服务(
        节点直接身份结构事务域& 事务域,
        节点直接身份仓库& 节点,
        正式关系仓库& 关系,
        可重建索引仓库& 索引,
        节点直接类型合同仓库& 类型合同,
        节点直接类型化值仓库& 类型化值) noexcept
        : 事务域_(事务域), 节点_(节点), 关系_(关系), 索引_(索引),
          类型合同_(类型合同), 类型化值_(类型化值) {}

    节点直接代次读取结果 读取当前事实代次() const {
        auto 许可 = 事务域_.取得读取许可();
        if (!许可.有效()) return {{节点直接结构服务状态::许可拒绝, 0}};
        const auto 代次 = 许可.读取已发布代次();
        return {{代次 == 0 ? 节点直接结构服务状态::未找到
                           : 节点直接结构服务状态::成功, 代次}};
    }

    节点直接节点读取结果 读取当前节点(节点稳定主键 主键) const {
        auto 许可 = 事务域_.取得读取许可();
        if (!许可.有效()) return {{节点直接结构服务状态::许可拒绝, 0}, std::nullopt};
        const auto 代次 = 许可.读取已发布代次();
        if (代次 == 0) return {{节点直接结构服务状态::未找到, 0}, std::nullopt};
        const auto 读取 = 节点_.读取稳定主键当前身份(主键);
        if (读取.状态 == 稳定主键当前身份状态::当前有效 && 读取.当前记录) {
            return {{节点直接结构服务状态::成功, 代次}, 形成节点见证_(*读取.当前记录)};
        }
        if (读取.状态 == 稳定主键当前身份状态::未占用
            || 读取.状态 == 稳定主键当前身份状态::历史占用) {
            return {{节点直接结构服务状态::未找到, 代次}, std::nullopt};
        }
        return {{读取.状态 == 稳定主键当前身份状态::入口拒绝
                    ? 节点直接结构服务状态::入口拒绝
                    : 节点直接结构服务状态::内部不一致, 0}, std::nullopt};
    }

    节点直接关系组读取结果 读取按源当前关系组(
        节点稳定主键 源, 关系类型 类型) const {
        return 读取关系组_(源, 类型, 0);
    }
    节点直接关系组读取结果 读取按目标当前关系组(
        节点稳定主键 目标, 关系类型 类型) const {
        return 读取关系组_(目标, 类型, 1);
    }
    节点直接关系组读取结果 读取当前相关关系组(节点稳定主键 节点) const {
        return 读取关系组_(节点, 关系类型::普通父子, 2);
    }

    节点直接类型化值读取结果 读取所属身份当前类型化值组(
        节点稳定主键 所属身份) const {
        auto 许可 = 事务域_.取得读取许可();
        if (!许可.有效()) return {{节点直接结构服务状态::许可拒绝, 0}, {}};
        const auto 代次 = 许可.读取已发布代次();
        if (代次 == 0 || !稳定主键有效(所属身份)) {
            return {{代次 == 0 ? 节点直接结构服务状态::未找到
                               : 节点直接结构服务状态::入口拒绝,
                     std::uint64_t{0}}, {}};
        }
        auto 值组 = 类型化值_.读取所属身份当前值组(所属身份);
        return {{值组.empty() ? 节点直接结构服务状态::未找到
                              : 节点直接结构服务状态::成功, 代次}, std::move(值组)};
    }

    节点直接索引读取结果 读取目标当前索引组(节点稳定主键 目标身份) const {
        auto 许可 = 事务域_.取得读取许可();
        if (!许可.有效()) return {{节点直接结构服务状态::许可拒绝, 0}, {}};
        const auto 代次 = 许可.读取已发布代次();
        if (代次 == 0) return {{节点直接结构服务状态::未找到, 0}, {}};
        const auto 节点读取 = 节点_.读取稳定主键当前身份(目标身份);
        if (节点读取.状态 != 稳定主键当前身份状态::当前有效 || !节点读取.当前身份) {
            return {{节点读取.状态 == 稳定主键当前身份状态::未占用
                        || 节点读取.状态 == 稳定主键当前身份状态::历史占用
                    ? 节点直接结构服务状态::未找到
                    : 节点直接结构服务状态::入口拒绝, 代次}, {}};
        }
        可重建索引记录 目标;
        目标.目标种类 = 索引目标种类::节点;
        目标.节点 = *节点读取.当前身份;
        std::vector<可重建索引读回> 结果;
        for (const auto& 键 : 索引_.读取目标索引物理键组(目标)) {
            结果.push_back({键, true, 形成节点见证_(*节点读取.当前记录), std::nullopt});
        }
        return {{结果.empty() ? 节点直接结构服务状态::未找到
                              : 节点直接结构服务状态::成功, 代次}, std::move(结果)};
    }

    节点直接索引读取结果 读取当前索引(const 索引物理键& 键) const {
        if (!索引物理键完整(键)) {
            return {{节点直接结构服务状态::入口拒绝, 0}, {}};
        }
        try {
#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
            const auto 故障 = 测试_下次当前索引读取故障_.exchange(
                0, std::memory_order_acq_rel);
            if (故障 == 1) throw std::bad_alloc{};
            if (故障 == 2) throw 节点直接当前索引查询测试异常{};
#endif
            auto 许可 = 事务域_.取得读取许可();
            if (!许可.有效()) {
                return {{节点直接结构服务状态::许可拒绝, 0}, {}};
            }
            const auto 代次 = 许可.读取已发布代次();
            if (代次 == 0) {
                return {{节点直接结构服务状态::未找到, 0}, {}};
            }
            const auto 内部 = 索引_.读取索引物理键结构化(键);
            if (内部.状态 == 可重建索引精确读取状态::未找到) {
                return {{节点直接结构服务状态::未找到, 代次}, {}};
            }
            if (内部.状态 == 可重建索引精确读取状态::资源失败) {
                return {{节点直接结构服务状态::资源失败, 0}, {}};
            }
            if (内部.状态 != 可重建索引精确读取状态::已读取
                || !内部.当前记录 || !(内部.当前记录->物理键 == 键)) {
                return {{节点直接结构服务状态::内部不一致, 0}, {}};
            }

            可重建索引读回 读回;
            读回.键 = 键;
            读回.当前 = true;
            const auto& 记录 = *内部.当前记录;
            if (记录.目标种类 == 索引目标种类::节点
                && 句柄有效(记录.节点) && !句柄有效(记录.关系)) {
#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
                if (故障 == 4 || 故障 == 5) {
                    return {{节点直接结构服务状态::内部不一致, 0}, {}};
                }
#endif
                auto 节点记录 = 节点_.读取节点(记录.节点);
#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
                if (故障 == 3) 节点记录.reset();
#endif
                if (!节点记录) {
                    return {{节点直接结构服务状态::内部不一致, 0}, {}};
                }
                读回.节点目标 = 形成节点见证_(*节点记录);
            } else if (记录.目标种类 == 索引目标种类::关系
                && 句柄有效(记录.关系) && !句柄有效(记录.节点)) {
#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
                if (故障 == 3) {
                    return {{节点直接结构服务状态::内部不一致, 0}, {}};
                }
#endif
                auto 关系记录 = 关系_.读取关系(记录.关系);
#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
                if (故障 == 4) 关系记录.reset();
#endif
                if (!关系记录) {
                    return {{节点直接结构服务状态::内部不一致, 0}, {}};
                }
                读回.关系目标 = 形成关系见证_(*关系记录);
#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
                if (故障 == 5) 读回.关系目标.reset();
#endif
                if (!读回.关系目标) {
                    return {{节点直接结构服务状态::内部不一致, 0}, {}};
                }
            } else {
                return {{节点直接结构服务状态::内部不一致, 0}, {}};
            }
            return {{节点直接结构服务状态::成功, 代次}, {std::move(读回)}};
        } catch (const std::bad_alloc&) {
            return {{节点直接结构服务状态::资源失败, 0}, {}};
        } catch (...) {
            return {{节点直接结构服务状态::内部不一致, 0}, {}};
        }
    }

#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
    void 测试_令下次当前索引读取抛出资源异常() noexcept {
        测试_下次当前索引读取故障_.store(1, std::memory_order_release);
    }

    void 测试_令下次当前索引读取抛出其它异常() noexcept {
        测试_下次当前索引读取故障_.store(2, std::memory_order_release);
    }

    bool 测试_令下次当前索引读取模拟内部损坏(
        节点直接当前索引读取内部损坏测试种类 种类) noexcept {
        const auto 值 = static_cast<std::uint8_t>(种类);
        if (值 < 1 || 值 > 3) return false;
        测试_下次当前索引读取故障_.store(
            static_cast<std::uint8_t>(2 + 值), std::memory_order_release);
        return true;
    }
#endif

    节点直接类型合同读取结果 读取精确类型合同(
        类型合同稳定身份 合同身份, std::uint32_t 合同版本) const {
        auto 许可 = 事务域_.取得读取许可();
        if (!许可.有效()) return {{节点直接结构服务状态::许可拒绝, 0}, std::nullopt};
        const auto 代次 = 许可.读取已发布代次();
        if (代次 == 0) return {{节点直接结构服务状态::未找到, 0}, std::nullopt};
        auto 合同 = 类型合同_.读取精确合同(合同身份, 合同版本);
        return {{合同 ? 节点直接结构服务状态::成功 : 节点直接结构服务状态::未找到, 代次},
            std::move(合同)};
    }

    节点直接两级关系值投影结果 读取两级关系与目标当前类型化值(
        const 节点直接两级关系值投影请求& 请求) const noexcept;

private:
    static 节点稳定身份见证 形成节点见证_(const 节点直接身份记录& 记录) noexcept {
        return {记录.稳定主键, 记录.类型, 记录.版本号};
    }

    std::optional<关系稳定身份见证> 形成关系见证_(const 正式关系记录& 记录) const {
        const auto 源 = 节点_.读取节点(记录.源节点);
        const auto 目标 = 节点_.读取节点(记录.目标节点);
        if (!源 || !目标) return std::nullopt;
        return 关系稳定身份见证{记录.稳定主键, 记录.类型, 记录.版本号,
            形成节点见证_(*源), 形成节点见证_(*目标), 记录.顺序号};
    }

    std::optional<关系稳定身份见证> 形成当前关系见证_(
        const 正式关系记录& 记录) const;

    节点直接关系组读取结果 读取关系组_(
        节点稳定主键 主键, 关系类型 类型, std::uint8_t 方向) const {
        auto 许可 = 事务域_.取得读取许可();
        if (!许可.有效()) return {{节点直接结构服务状态::许可拒绝, 0}, {}};
        const auto 代次 = 许可.读取已发布代次();
        if (代次 == 0) return {{节点直接结构服务状态::未找到, 0}, {}};
        const auto 节点读取 = 节点_.读取稳定主键当前身份(主键);
        if (节点读取.状态 != 稳定主键当前身份状态::当前有效 || !节点读取.当前身份) {
            return {{节点直接结构服务状态::未找到, 代次}, {}};
        }
        const auto 内部 = 方向 == 0 ? 关系_.读取有效源关系组(*节点读取.当前身份, 类型)
            : 方向 == 1 ? 关系_.读取有效目标关系组(*节点读取.当前身份, 类型)
                         : 关系_.读取有效相关关系组(*节点读取.当前身份);
        if (内部.状态 != 稳定关系组读取状态::已读取) {
            return {{内部.状态 == 稳定关系组读取状态::入口拒绝
                        ? 节点直接结构服务状态::入口拒绝
                        : 节点直接结构服务状态::内部不一致, 0}, {}};
        }
        std::vector<关系稳定身份见证> 结果;
        for (const auto& 记录 : 内部.关系组) {
            auto 见证 = 形成关系见证_(记录);
            if (!见证) return {{节点直接结构服务状态::内部不一致, 0}, {}};
            结果.push_back(std::move(*见证));
        }
        return {{结果.empty() ? 节点直接结构服务状态::未找到
                              : 节点直接结构服务状态::成功, 代次}, std::move(结果)};
    }

    节点直接身份结构事务域& 事务域_;
    节点直接身份仓库& 节点_;
    正式关系仓库& 关系_;
    可重建索引仓库& 索引_;
    节点直接类型合同仓库& 类型合同_;
    节点直接类型化值仓库& 类型化值_;
#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
    mutable std::atomic<std::uint8_t> 测试_下次当前索引读取故障_{0};
    mutable bool 自检下一次两级投影资源失败_ = false;
    friend void 自检注入下一次两级投影资源失败(
        节点直接结构查询服务& 服务) noexcept;
#endif
};

std::optional<关系稳定身份见证> 节点直接结构查询服务::形成当前关系见证_(
    const 正式关系记录& 记录) const {
    if (!正式关系记录完整(记录) || 记录.状态 != 记录状态::有效
        || static_cast<std::uint32_t>(记录.类型) >= 正式关系类型ABI数量) {
        return std::nullopt;
    }
    const auto 源记录 = 节点_.读取节点(记录.源节点);
    const auto 目标记录 = 节点_.读取节点(记录.目标节点);
    if (!源记录 || !目标记录
        || 源记录->状态 != 记录状态::有效 || 目标记录->状态 != 记录状态::有效) {
        return std::nullopt;
    }
    const auto 源当前 = 节点_.读取稳定主键当前身份(源记录->稳定主键);
    const auto 目标当前 = 节点_.读取稳定主键当前身份(目标记录->稳定主键);
    if (源当前.状态 != 稳定主键当前身份状态::当前有效
        || 目标当前.状态 != 稳定主键当前身份状态::当前有效
        || !源当前.当前身份 || !源当前.当前记录
        || !目标当前.当前身份 || !目标当前.当前记录
        || *源当前.当前身份 != 记录.源节点
        || *目标当前.当前身份 != 记录.目标节点
        || 源当前.当前记录->稳定主键 != 源记录->稳定主键
        || 源当前.当前记录->类型 != 源记录->类型
        || 源当前.当前记录->版本号 != 源记录->版本号
        || 目标当前.当前记录->稳定主键 != 目标记录->稳定主键
        || 目标当前.当前记录->类型 != 目标记录->类型
        || 目标当前.当前记录->版本号 != 目标记录->版本号) {
        return std::nullopt;
    }
    return 关系稳定身份见证{记录.稳定主键, 记录.类型, 记录.版本号,
        形成节点见证_(*源当前.当前记录), 形成节点见证_(*目标当前.当前记录),
        记录.顺序号};
}

节点直接两级关系值投影结果 节点直接结构查询服务::读取两级关系与目标当前类型化值(
    const 节点直接两级关系值投影请求& 请求) const noexcept {
    static_assert(noexcept(节点直接两级关系值投影结果{
        {节点直接结构服务状态::资源失败, 0}, {}, {}, {}, {}, std::nullopt}));
    static_assert(noexcept(节点直接两级关系值投影结果{
        {节点直接结构服务状态::内部不一致, 0}, {}, {}, {}, {}, std::nullopt}));

    auto 空结果 = [](节点直接结构服务状态 状态, std::uint64_t 代次 = 0,
        std::optional<节点稳定身份见证> 未满足 = std::nullopt) noexcept {
        return 节点直接两级关系值投影结果{
            {状态, 代次}, {}, {}, {}, {}, std::move(未满足)};
    };
    auto 节点顺序 = [](const 节点稳定身份见证& 左,
        const 节点稳定身份见证& 右) noexcept {
        if (左.稳定主键.命名域 != 右.稳定主键.命名域)
            return 左.稳定主键.命名域 < 右.稳定主键.命名域;
        if (左.稳定主键.键值 != 右.稳定主键.键值)
            return 左.稳定主键.键值 < 右.稳定主键.键值;
        if (左.类型 != 右.类型)
            return static_cast<std::uint32_t>(左.类型) < static_cast<std::uint32_t>(右.类型);
        return 左.身份版本 < 右.身份版本;
    };
    auto 关系顺序 = [](const 关系稳定身份见证& 左,
        const 关系稳定身份见证& 右) noexcept {
        if (左.稳定主键.命名域 != 右.稳定主键.命名域)
            return 左.稳定主键.命名域 < 右.稳定主键.命名域;
        if (左.稳定主键.键值 != 右.稳定主键.键值)
            return 左.稳定主键.键值 < 右.稳定主键.键值;
        return 左.关系版本 < 右.关系版本;
    };
    auto 值顺序 = [](const 类型化值读回& 左, const 类型化值读回& 右) noexcept {
        if (左.所属身份.稳定主键.命名域 != 右.所属身份.稳定主键.命名域)
            return 左.所属身份.稳定主键.命名域 < 右.所属身份.稳定主键.命名域;
        if (左.所属身份.稳定主键.键值 != 右.所属身份.稳定主键.键值)
            return 左.所属身份.稳定主键.键值 < 右.所属身份.稳定主键.键值;
        if (左.类型合同身份.命名域 != 右.类型合同身份.命名域)
            return 左.类型合同身份.命名域 < 右.类型合同身份.命名域;
        if (左.类型合同身份.键值 != 右.类型合同身份.键值)
            return 左.类型合同身份.键值 < 右.类型合同身份.键值;
        if (左.类型合同版本 != 右.类型合同版本)
            return 左.类型合同版本 < 右.类型合同版本;
        if (左.值记录身份.命名域 != 右.值记录身份.命名域)
            return 左.值记录身份.命名域 < 右.值记录身份.命名域;
        if (左.值记录身份.键值 != 右.值记录身份.键值)
            return 左.值记录身份.键值 < 右.值记录身份.键值;
        return 左.值记录版本 < 右.值记录版本;
    };

    try {
        const auto 组合法 = [](const std::vector<节点稳定身份见证>& 组) {
            for (std::size_t 序号 = 0; 序号 < 组.size(); ++序号) {
                if (!节点稳定身份见证完整(组[序号])) return false;
                for (std::size_t 后序 = 序号 + 1; 后序 < 组.size(); ++后序) {
                    if (组[序号].稳定主键 == 组[后序].稳定主键) return false;
                }
            }
            return true;
        };
        const auto 关系数值 = static_cast<std::uint32_t>(请求.关系种类);
        if (请求.合同版本 != 节点直接两级关系值投影合同版本
            || 关系数值 >= 正式关系类型ABI数量
            || !组合法(请求.必须节点组) || !组合法(请求.根节点组)) {
            return 空结果(节点直接结构服务状态::入口拒绝);
        }
        for (const auto& 根 : 请求.根节点组) {
            if (std::none_of(请求.必须节点组.begin(), 请求.必须节点组.end(),
                [&](const auto& 必须) { return 必须 == 根; })) {
                return 空结果(节点直接结构服务状态::入口拒绝);
            }
        }
#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
        if (自检下一次两级投影资源失败_) {
            自检下一次两级投影资源失败_ = false;
            throw std::bad_alloc{};
        }
#endif
        auto 必须节点组 = 请求.必须节点组;
        auto 根节点组 = 请求.根节点组;
        std::sort(必须节点组.begin(), 必须节点组.end(), 节点顺序);
        std::sort(根节点组.begin(), 根节点组.end(), 节点顺序);

        auto 许可 = 事务域_.取得读取许可();
        if (!许可.有效()) return 空结果(节点直接结构服务状态::许可拒绝);
        const auto 代次 = 许可.读取已发布代次();
        if (代次 == 0) return 空结果(节点直接结构服务状态::未找到);

        struct 当前节点映射 { 节点稳定身份见证 见证; 节点句柄 句柄; };
        std::vector<当前节点映射> 当前节点组;
        std::vector<节点稳定身份见证> 已解析必须节点组;
        for (const auto& 请求节点 : 必须节点组) {
            const auto 当前 = 节点_.读取稳定主键当前身份(请求节点.稳定主键);
            if (当前.状态 == 稳定主键当前身份状态::未占用
                || 当前.状态 == 稳定主键当前身份状态::历史占用) {
                return 空结果(节点直接结构服务状态::未找到, 代次, 请求节点);
            }
            if (当前.状态 != 稳定主键当前身份状态::当前有效
                || !当前.当前身份 || !当前.当前记录) {
                return 空结果(节点直接结构服务状态::内部不一致);
            }
            if (!句柄有效(*当前.当前身份)
                || 当前.当前记录->节点编号 == 0
                || !稳定主键有效(当前.当前记录->稳定主键)
                || 当前.当前记录->类型 == 节点类型::未分类
                || 当前.当前记录->版本号 == 0
                || 当前.当前记录->状态 != 记录状态::有效
                || 当前.当前身份->节点编号 != 当前.当前记录->节点编号
                || 当前.当前身份->版本号 != 当前.当前记录->版本号) {
                return 空结果(节点直接结构服务状态::内部不一致);
            }
            const auto 当前见证 = 形成节点见证_(*当前.当前记录);
            if (当前见证 != 请求节点) {
                return 空结果(节点直接结构服务状态::版本漂移, 代次, 请求节点);
            }
            当前节点组.push_back({当前见证, *当前.当前身份});
            已解析必须节点组.push_back(当前见证);
        }
        auto 查找句柄 = [&](const 节点稳定身份见证& 见证) -> std::optional<节点句柄> {
            const auto 位置 = std::find_if(当前节点组.begin(), 当前节点组.end(),
                [&](const auto& 项) { return 项.见证.稳定主键 == 见证.稳定主键; });
            return 位置 == 当前节点组.end() ? std::nullopt
                                              : std::optional<节点句柄>{位置->句柄};
        };
        auto 加入关系 = [](std::vector<关系稳定身份见证>& 组,
            const 关系稳定身份见证& 见证) {
            const auto 位置 = std::find_if(组.begin(), 组.end(), [&](const auto& 项) {
                return 项.稳定主键 == 见证.稳定主键;
            });
            if (位置 == 组.end()) { 组.push_back(见证); return true; }
            return *位置 == 见证;
        };

        std::vector<关系稳定身份见证> 第一跳关系组;
        struct 中间节点项 { 节点稳定身份见证 见证; 节点句柄 句柄; };
        std::vector<中间节点项> 中间节点组;
        for (const auto& 根 : 根节点组) {
            const auto 根句柄 = 查找句柄(根);
            if (!根句柄) return 空结果(节点直接结构服务状态::内部不一致);
            const auto 读取 = 关系_.读取有效源关系组(*根句柄, 请求.关系种类);
            if (读取.状态 != 稳定关系组读取状态::已读取) {
                return 空结果(节点直接结构服务状态::内部不一致);
            }
            for (const auto& 记录 : 读取.关系组) {
                if (记录.源节点 != *根句柄 || 记录.顺序号 != 请求.第一跳角色) continue;
                const auto 见证 = 形成当前关系见证_(记录);
                if (!见证 || !加入关系(第一跳关系组, *见证)) {
                    return 空结果(节点直接结构服务状态::内部不一致);
                }
                const auto 中间位置 = std::find_if(中间节点组.begin(), 中间节点组.end(),
                    [&](const auto& 项) { return 项.见证.稳定主键 == 见证->目标端.稳定主键; });
                if (中间位置 == 中间节点组.end()) {
                    中间节点组.push_back({见证->目标端, 记录.目标节点});
                } else if (中间位置->见证 != 见证->目标端
                    || 中间位置->句柄 != 记录.目标节点) {
                    return 空结果(节点直接结构服务状态::内部不一致);
                }
            }
        }

        std::vector<关系稳定身份见证> 中间关系组;
        std::vector<关系稳定身份见证> 目标关系组;
        for (const auto& 中间 : 中间节点组) {
            const auto 源组 = 关系_.读取有效源关系组(中间.句柄, 请求.关系种类);
            const auto 目标组 = 关系_.读取有效目标关系组(中间.句柄, 请求.关系种类);
            if (源组.状态 != 稳定关系组读取状态::已读取
                || 目标组.状态 != 稳定关系组读取状态::已读取) {
                return 空结果(节点直接结构服务状态::内部不一致);
            }
            auto 消费关系组 = [&](const std::vector<正式关系记录>& 记录组) {
                for (const auto& 记录 : 记录组) {
                    const auto 见证 = 形成当前关系见证_(记录);
                    if (!见证 || !加入关系(中间关系组, *见证)) return false;
                    if (记录.源节点 == 中间.句柄
                        && 记录.顺序号 == 请求.类型化值目标角色
                        && !加入关系(目标关系组, *见证)) return false;
                }
                return true;
            };
            if (!消费关系组(源组.关系组) || !消费关系组(目标组.关系组)) {
                return 空结果(节点直接结构服务状态::内部不一致);
            }
        }

        std::vector<节点直接关系目标当前类型化值投影> 目标值组;
        for (const auto& 目标关系 : 目标关系组) {
            auto 当前值组 = 类型化值_.读取所属身份当前值组(目标关系.目标端.稳定主键);
            std::vector<类型化值读回> 已检查值组;
            for (const auto& 值 : 当前值组) {
                if (!类型化值记录完整_(值) || 值.所属身份 != 目标关系.目标端
                    || !值.当前 || 值.首次发布代次 > 代次
                    || 值.当前状态发布代次 > 代次) {
                    return 空结果(节点直接结构服务状态::内部不一致);
                }
                const auto 同身份 = std::find_if(已检查值组.begin(), 已检查值组.end(),
                    [&](const auto& 项) { return 项.值记录身份 == 值.值记录身份; });
                if (同身份 != 已检查值组.end()) {
                    if (*同身份 != 值) return 空结果(节点直接结构服务状态::内部不一致);
                    continue;
                }
                const auto 同合同 = std::find_if(已检查值组.begin(), 已检查值组.end(),
                    [&](const auto& 项) {
                        return 项.所属身份.稳定主键 == 值.所属身份.稳定主键
                            && 项.类型合同身份 == 值.类型合同身份
                            && 项.类型合同版本 == 值.类型合同版本;
                    });
                if (同合同 != 已检查值组.end()) {
                    return 空结果(节点直接结构服务状态::内部不一致);
                }
                已检查值组.push_back(值);
            }
            std::sort(已检查值组.begin(), 已检查值组.end(), 值顺序);
            目标值组.push_back({目标关系, std::move(已检查值组)});
        }
        std::sort(第一跳关系组.begin(), 第一跳关系组.end(), 关系顺序);
        std::sort(中间关系组.begin(), 中间关系组.end(), 关系顺序);
        std::sort(目标值组.begin(), 目标值组.end(), [&](const auto& 左, const auto& 右) {
            return 关系顺序(左.目标关系, 右.目标关系);
        });
        return {{节点直接结构服务状态::成功, 代次}, std::move(已解析必须节点组),
            std::move(第一跳关系组), std::move(中间关系组), std::move(目标值组),
            std::nullopt};
    } catch (const std::bad_alloc&) {
        return {{节点直接结构服务状态::资源失败, 0}, {}, {}, {}, {}, std::nullopt};
    } catch (const std::length_error&) {
        return {{节点直接结构服务状态::资源失败, 0}, {}, {}, {}, {}, std::nullopt};
    } catch (...) {
        return {{节点直接结构服务状态::内部不一致, 0}, {}, {}, {}, {}, std::nullopt};
    }
}

#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
bool 自检验证两级投影类型化值完整性(const 类型化值读回& 值) noexcept {
    return 类型化值记录完整_(值);
}

void 自检注入下一次两级投影资源失败(节点直接结构查询服务& 服务) noexcept {
    服务.自检下一次两级投影资源失败_ = true;
}
#endif

} // namespace 海中鱼巣
