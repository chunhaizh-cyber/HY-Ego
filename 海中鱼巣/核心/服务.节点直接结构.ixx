// 文件规则：本模块只在共享许可内把六仓当前值投影为稳定身份，不取得写入能力。
module;

#include "节点直接结构合同.数据.h"

#include <algorithm>
#include <atomic>
#include <new>
#include <optional>
#include <utility>

export module 海中鱼巣.核心.服务.节点直接结构;

import 海中鱼巣.核心.执行器.节点直接身份结构写入;
import 海中鱼巣.核心.仓库.节点直接类型合同;
import 海中鱼巣.核心.仓库.节点直接类型化值;

export namespace 海中鱼巣 {

#if defined(HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST)
struct 节点直接当前索引查询测试异常 {};

enum class 节点直接当前索引读取内部损坏测试种类 : std::uint8_t {
    节点目标权威读回缺失 = 1,
    关系目标权威读回缺失 = 2,
    关系端点见证不可形成 = 3
};
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
#endif
};

} // namespace 海中鱼巣
