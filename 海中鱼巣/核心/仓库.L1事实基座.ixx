module;

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <bcrypt.h>
#include <algorithm>
#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <limits>
#include <memory>
#include <mutex>
#include <new>
#include <optional>
#include <shared_mutex>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>
#include <type_traits>
#include <tuple>

#pragma comment(lib, "bcrypt.lib")

#define L1_FACT_BASE_NO_INCLUDES

export module 海中鱼巣.核心.仓库.L1事实基座;
import 海中鱼巣.核心.合同.L1事实基座;
import 海中鱼巣.核心.合同.L1中性CRUD;
import 海中鱼巣.核心.合同.L1所有者范围CRUD;

export namespace 海中鱼巣 {

enum class L1事实基座核心持久恢复状态 : std::uint8_t {
    已建立空仓 = 1, 已恢复 = 2, 入口拒绝 = 3, 存储占用 = 4,
    材料不完整 = 5, 格式不支持 = 6, 摘要不一致 = 7,
    编码或所有者冲突 = 8, 事实代次漂移 = 9, 资源失败 = 10,
    持久证据未知 = 11, 内部不一致 = 12
};

struct L1事实基座核心持久恢复见证 final {
    std::uint32_t 格式版本 = 1;
    std::uint64_t 快照序号 = 0;
    std::uint64_t 事实代次 = 0;
    std::array<std::uint8_t, 32> 载荷SHA256{};
};

struct L1事实基座核心持久恢复结果 final {
    L1事实基座核心持久恢复状态 状态 =
        L1事实基座核心持久恢复状态::内部不一致;
    std::optional<L1事实基座核心持久恢复见证> 恢复见证;
};

class L1事实基座仓库 final {
    struct 持久会话;
    struct 所有者范围一致投影内部请求 final {
        std::uint64_t 期望事实代次 = 0;
        const std::vector<L1结构所有者身份>& 所有者;
        const std::vector<稳定编码>& 节点;
        const std::vector<稳定编码>& 关系;
        const std::vector<稳定编码>& 值;
        const std::vector<L1所有者范围一致属性值选择项>& 属性值;
        const std::vector<L1所有者范围一致源关系组选择项>& 源关系组;
        const std::vector<L1所有者范围一致目标关系组选择项>& 目标关系组;
        const std::vector<L1所有者范围一致关系类型闭包选择项>*
            关系类型闭包 = nullptr;
    };
    struct 物理清理幂等记录 final {
        L1物理清理请求 首次规范请求;
        L1物理清理状态 首次状态 = L1物理清理状态::内部错误;
        std::uint64_t 首次物理清理事实代次 = 0;
        std::vector<std::pair<L1物理清理事实身份, 稳定编码>>
            首次稳定编码映射;
        bool 已物理清理 = false;
    };
public:
    enum class 一致当前事实种类 : std::uint8_t {
        节点 = 1, 关系 = 2, 值 = 3
    };

    struct 一致具名事实内部结果项 final {
        稳定编码 查询编码;
        L1中性一致当前读取项目状态 状态 =
            L1中性一致当前读取项目状态::未找到;
        std::optional<L1事实副本> 事实;
        std::optional<std::uint64_t> 物理清理事实代次;
        std::optional<L1物理清理墓碑> 物理清理墓碑;
    };

    struct 一致属性值内部投影 final {
        属性槽 属性槽值;
        值事实 当前值事实;
    };

    struct 一致属性值内部结果项 final {
        稳定编码 节点;
        稳定编码 属性类型;
        L1中性一致当前读取项目状态 状态 =
            L1中性一致当前读取项目状态::未找到;
        std::optional<一致属性值内部投影> 投影;
    };

    struct 一致关系对端内部投影 final {
        关系事实 关系;
        节点事实 对端节点;
    };

    struct 一致源关系组内部结果项 final {
        稳定编码 源节点;
        稳定编码 关系类型节点;
        std::vector<一致关系对端内部投影> 成员;
    };

    struct 一致目标关系组内部结果项 final {
        稳定编码 目标节点;
        稳定编码 关系类型节点;
        std::vector<一致关系对端内部投影> 成员;
    };

    struct 一致当前读取内部结果 final {
        L1中性一致当前读取状态 状态 =
            L1中性一致当前读取状态::入口拒绝;
        std::uint64_t 读取事实代次 = 0;
        std::vector<一致具名事实内部结果项> 节点;
        std::vector<一致具名事实内部结果项> 关系;
        std::vector<一致具名事实内部结果项> 值;
        std::vector<一致属性值内部结果项> 属性值;
        std::vector<一致源关系组内部结果项> 源关系组;
        std::vector<一致目标关系组内部结果项> 目标关系组;
    };

    L1事实基座仓库() = default;
    L1事实基座仓库(const L1事实基座仓库&) = delete;
    L1事实基座仓库& operator=(const L1事实基座仓库&) = delete;

    L1事实基座核心持久恢复结果 初始化持久恢复(
        const std::filesystem::path& 绝对受控根) noexcept {
        return 初始化持久恢复实现(绝对受控根);
    }

    L1所有者范围建立结果 建立所有者范围(
        const L1所有者范围建立请求& 请求) {
        const auto 失败 = [&](L1所有者范围管理状态 状态,
            std::uint64_t 代次 = 0) {
            return L1所有者范围建立结果{状态,
                L1所有者范围CRUD合同版本, 请求.建立幂等身份,
                std::nullopt, 代次, false,
                L1所有者范围重试边界::修正请求后可重试};
        };
        if (请求.合同版本 != L1所有者范围CRUD合同版本
            || !有效(请求.建立幂等身份)
            || 请求.范围种类 != L1所有者范围种类::独占结构范围)
            return 失败(L1所有者范围管理状态::入口拒绝);
        try {
            std::unique_lock<std::shared_mutex> 锁(锁_);
            if (状态_.隔离 || !状态完整(状态_))
                return 失败(L1所有者范围管理状态::内部不一致,
                    状态_.事实代次);
            const auto 既有 = 状态_.所有者建立幂等账.find(
                请求.建立幂等身份.值);
            if (既有 != 状态_.所有者建立幂等账.end()) {
                if (既有->second.首次请求 != 请求)
                    return 失败(L1所有者范围管理状态::幂等冲突,
                        状态_.事实代次);
                auto 结果 = 既有->second.首次结果;
                结果.状态 = L1所有者范围管理状态::精确重复;
                结果.是否形成内存权威发布 = false;
                结果.重试边界 =
                    L1所有者范围重试边界::原幂等身份读回收敛;
                return 结果;
            }
            状态 候选 = 状态_;
            const auto 编码 = 分配稳定编码(候选);
            if (!编码) return 失败(L1所有者范围管理状态::资源失败,
                状态_.事实代次);
            const auto 新代次 = 状态_.事实代次 + 1;
            const L1结构所有者事实 事实{{*编码}, 请求.范围种类,
                新代次, std::nullopt};
            候选.当前所有者.emplace(编码->值, 事实);
            候选.永久占用.insert(编码->值);
            候选.事实代次 = 新代次;
            L1所有者范围建立结果 结果{
                L1所有者范围管理状态::成功,
                L1所有者范围CRUD合同版本, 请求.建立幂等身份,
                事实, 新代次, true, L1所有者范围重试边界::不适用};
            候选.所有者建立幂等账.emplace(请求.建立幂等身份.值,
                所有者建立幂等记录{请求, 结果});
            if (!状态完整(候选))
                return 失败(L1所有者范围管理状态::内部不一致,
                    状态_.事实代次);
            const auto 持久准备 = 准备持久发布(候选);
            if (持久准备 != 持久准备状态::成功)
                return 失败(持久准备 == 持久准备状态::资源失败
                    ? L1所有者范围管理状态::资源失败
                    : L1所有者范围管理状态::内部不一致, 状态_.事实代次);
            std::swap(状态_, 候选);
            const auto 读回 = 状态_.当前所有者.find(编码->值);
            if (读回 == 状态_.当前所有者.end() || 读回->second != 事实) {
                状态_.隔离 = true;
                return 失败(L1所有者范围管理状态::内部不一致, 新代次);
            }
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(L1所有者范围管理状态::资源失败);
        } catch (...) {
            return 失败(L1所有者范围管理状态::内部不一致);
        }
    }

    // 诊断责任：内部消化；只读互证将仓库事实、建立账和不变量映射为管理状态。
    L1所有者范围重入结果 验证所有者范围重入(
        const L1所有者范围重入请求& 请求) const {
        const auto 失败 = [&](L1所有者范围管理状态 管理状态,
            std::uint64_t 代次 = 0,
            std::optional<L1结构所有者事实> 所有者事实 = std::nullopt,
            L1所有者范围重试边界 重试边界 =
                L1所有者范围重试边界::修正请求后可重试) {
            return L1所有者范围重入结果{管理状态,
                L1所有者范围CRUD合同版本, 请求.建立幂等身份,
                std::move(所有者事实), 代次, false, 重试边界};
        };
        if (请求.合同版本 != L1所有者范围CRUD合同版本
            || !有效(请求.所有者) || !有效(请求.建立幂等身份))
            return 失败(L1所有者范围管理状态::入口拒绝);
        try {
            std::shared_lock<std::shared_mutex> 锁(锁_);
            if (状态_.隔离 || 状态_.事实代次 == 0 || !状态完整(状态_))
                return 失败(L1所有者范围管理状态::内部不一致,
                    状态_.事实代次);

            const auto 建立 = 状态_.所有者建立幂等账.find(
                请求.建立幂等身份.值);
            if (建立 == 状态_.所有者建立幂等账.end())
                return 失败(L1所有者范围管理状态::未找到,
                    状态_.事实代次);
            if (!建立->second.首次结果.所有者事实)
                return 失败(L1所有者范围管理状态::内部不一致,
                    状态_.事实代次);
            const auto& 建立事实 = *建立->second.首次结果.所有者事实;
            if (建立事实.所有者 != 请求.所有者)
                return 失败(L1所有者范围管理状态::幂等冲突,
                    状态_.事实代次);

            const auto 当前 = 状态_.当前所有者.find(请求.所有者.编码.值);
            if (当前 == 状态_.当前所有者.end()) {
                const auto 历史 = 状态_.历史所有者.find(
                    请求.所有者.编码.值);
                if (历史 != 状态_.历史所有者.end())
                    return 失败(L1所有者范围管理状态::已退出,
                        状态_.事实代次, 历史->second,
                        L1所有者范围重试边界::不适用);
                return 失败(L1所有者范围管理状态::未找到,
                    状态_.事实代次);
            }
            if (当前->second.范围种类 != L1所有者范围种类::独占结构范围
                || 当前->second.退出事实代次
                || 建立->second.首次请求.范围种类
                    != L1所有者范围种类::独占结构范围
                || 建立事实 != 当前->second)
                return 失败(L1所有者范围管理状态::引用冲突,
                    状态_.事实代次, 当前->second);

            return {L1所有者范围管理状态::成功,
                L1所有者范围CRUD合同版本, 请求.建立幂等身份,
                当前->second, 状态_.事实代次, false,
                L1所有者范围重试边界::不适用};
        } catch (const std::bad_alloc&) {
            return 失败(L1所有者范围管理状态::资源失败, 0,
                std::nullopt, L1所有者范围重试边界::原请求可重试);
        } catch (...) {
            return 失败(L1所有者范围管理状态::内部不一致);
        }
    }

    L1所有者范围退出结果 退出所有者范围(
        const L1所有者范围退出请求& 请求) {
        const auto 失败 = [&](L1所有者范围管理状态 状态,
            std::uint64_t 代次 = 0) {
            return L1所有者范围退出结果{状态,
                L1所有者范围CRUD合同版本, 请求.建立幂等身份,
                std::nullopt, 代次, false,
                L1所有者范围重试边界::修正请求后可重试};
        };
        if (请求.合同版本 != L1所有者范围CRUD合同版本
            || !有效(请求.所有者) || !有效(请求.建立幂等身份)
            || 请求.期望事实代次 == 0)
            return 失败(L1所有者范围管理状态::入口拒绝);
        try {
            std::unique_lock<std::shared_mutex> 锁(锁_);
            if (状态_.隔离 || !状态完整(状态_))
                return 失败(L1所有者范围管理状态::内部不一致,
                    状态_.事实代次);
            const auto 建立 = 状态_.所有者建立幂等账.find(
                请求.建立幂等身份.值);
            if (建立 == 状态_.所有者建立幂等账.end())
                return 失败(L1所有者范围管理状态::未找到, 状态_.事实代次);
            if (!建立->second.首次结果.所有者事实
                || 建立->second.首次结果.所有者事实->所有者 != 请求.所有者)
                return 失败(L1所有者范围管理状态::幂等冲突,
                    状态_.事实代次);
            const auto 当前 = 状态_.当前所有者.find(请求.所有者.编码.值);
            if (当前 == 状态_.当前所有者.end())
                return 失败(状态_.历史所有者.contains(请求.所有者.编码.值)
                    ? L1所有者范围管理状态::已退出
                    : L1所有者范围管理状态::未找到, 状态_.事实代次);
            if (请求.期望事实代次 != 状态_.事实代次)
                return 失败(L1所有者范围管理状态::事实代次漂移,
                    状态_.事实代次);
            const auto 属于所有者 = [&](const auto& 表) {
                return std::any_of(表.begin(), 表.end(), [&](const auto& 项) {
                    return 项.second.写入所有者 == 请求.所有者;
                });
            };
            if (属于所有者(状态_.当前节点) || 属于所有者(状态_.当前关系)
                || 属于所有者(状态_.当前值))
                return 失败(L1所有者范围管理状态::引用冲突,
                    状态_.事实代次);
            状态 候选 = 状态_;
            auto 事实 = 当前->second;
            const auto 新代次 = 状态_.事实代次 + 1;
            事实.退出事实代次 = 新代次;
            候选.当前所有者.erase(请求.所有者.编码.值);
            候选.历史所有者.emplace(请求.所有者.编码.值, 事实);
            候选.事实代次 = 新代次;
            if (!状态完整(候选))
                return 失败(L1所有者范围管理状态::内部不一致,
                    状态_.事实代次);
            const auto 持久准备 = 准备持久发布(候选);
            if (持久准备 != 持久准备状态::成功)
                return 失败(持久准备 == 持久准备状态::资源失败
                    ? L1所有者范围管理状态::资源失败
                    : L1所有者范围管理状态::内部不一致, 状态_.事实代次);
            std::swap(状态_, 候选);
            return {L1所有者范围管理状态::成功,
                L1所有者范围CRUD合同版本, 请求.建立幂等身份,
                事实, 新代次, true, L1所有者范围重试边界::不适用};
        } catch (const std::bad_alloc&) {
            return 失败(L1所有者范围管理状态::资源失败);
        } catch (...) {
            return 失败(L1所有者范围管理状态::内部不一致);
        }
    }

    // 诊断责任：向上送出；资源与局部结构矛盾均由中性结构化状态携带。
    一致当前读取内部结果 尝试读取一致当前内部投影(
        const L1中性一致当前读取请求& 请求) const {
        const auto 失败 = [](L1中性一致当前读取状态 状态,
            std::uint64_t 代次 = 0) {
            一致当前读取内部结果 结果;
            结果.状态 = 状态;
            结果.读取事实代次 = 代次;
            return 结果;
        };
        try {
            if (!一致当前请求有效(请求))
                return 失败(L1中性一致当前读取状态::入口拒绝);

            std::shared_lock<std::shared_mutex> 锁(锁_, std::try_to_lock);
            if (!锁.owns_lock())
                return 失败(L1中性一致当前读取状态::资源失败);
            if (状态_.隔离 || 状态_.事实代次 == 0)
                return 失败(L1中性一致当前读取状态::内部不一致);
            if (状态_.事实代次 != 请求.期望事实代次)
                return 失败(L1中性一致当前读取状态::事实代次漂移,
                    状态_.事实代次);

            一致当前读取内部结果 结果;
            结果.状态 = L1中性一致当前读取状态::成功;
            结果.读取事实代次 = 状态_.事实代次;
            结果.节点.reserve(请求.节点.size());
            结果.关系.reserve(请求.关系.size());
            结果.值.reserve(请求.值.size());
            结果.属性值.reserve(请求.属性值.size());
            结果.源关系组.reserve(请求.源关系组.size());
            结果.目标关系组.reserve(请求.目标关系组.size());

            for (const auto 编码 : 请求.节点) {
                auto 项 = 读取一致具名当前事实(状态_, 编码,
                    一致当前事实种类::节点);
                if (!项) return 失败(L1中性一致当前读取状态::内部不一致);
                结果.节点.push_back(std::move(*项));
            }
            for (const auto 编码 : 请求.关系) {
                auto 项 = 读取一致具名当前事实(状态_, 编码,
                    一致当前事实种类::关系);
                if (!项) return 失败(L1中性一致当前读取状态::内部不一致);
                结果.关系.push_back(std::move(*项));
            }
            for (const auto 编码 : 请求.值) {
                auto 项 = 读取一致具名当前事实(状态_, 编码,
                    一致当前事实种类::值);
                if (!项) return 失败(L1中性一致当前读取状态::内部不一致);
                结果.值.push_back(std::move(*项));
            }

            for (const auto& 选择 : 请求.属性值) {
                一致属性值内部结果项 项{选择.节点, 选择.属性类型,
                    L1中性一致当前读取项目状态::未找到, std::nullopt};
                const auto 节点项 = 读取一致具名当前事实(状态_, 选择.节点,
                    一致当前事实种类::节点);
                if (!节点项)
                    return 失败(L1中性一致当前读取状态::内部不一致);
                if (节点项->状态 != L1中性一致当前读取项目状态::成功) {
                    项.状态 = 节点项->状态;
                    结果.属性值.push_back(std::move(项));
                    continue;
                }
                const auto* 节点 = 节点项->事实
                    ? std::get_if<节点事实>(&*节点项->事实) : nullptr;
                if (!节点)
                    return 失败(L1中性一致当前读取状态::内部不一致);
                const auto 槽 = std::lower_bound(节点->当前属性.begin(),
                    节点->当前属性.end(), 选择.属性类型,
                    [](const 属性槽& 左, 稳定编码 右) {
                        return 左.属性类型节点 < 右;
                    });
                if (槽 == 节点->当前属性.end()
                    || 槽->属性类型节点 != 选择.属性类型) {
                    项.状态 = L1中性一致当前读取项目状态::属性未设置;
                    结果.属性值.push_back(std::move(项));
                    continue;
                }
                const auto 当前值 = 状态_.当前值.find(槽->当前值.值);
                if (当前值 == 状态_.当前值.end()
                    || !一致当前值局部完整(状态_, 当前值->first, 当前值->second)
                    || 当前值->second.所属节点 != 选择.节点
                    || 当前值->second.属性类型节点 != 选择.属性类型)
                    return 失败(L1中性一致当前读取状态::内部不一致);
                项.状态 = L1中性一致当前读取项目状态::成功;
                项.投影 = 一致属性值内部投影{*槽, 当前值->second};
                结果.属性值.push_back(std::move(项));
            }

            for (const auto& 选择 : 请求.源关系组) {
                auto 项 = 读取一致源关系组(状态_, 选择);
                if (!项) return 失败(L1中性一致当前读取状态::内部不一致);
                结果.源关系组.push_back(std::move(*项));
            }
            for (const auto& 选择 : 请求.目标关系组) {
                auto 项 = 读取一致目标关系组(状态_, 选择);
                if (!项) return 失败(L1中性一致当前读取状态::内部不一致);
                结果.目标关系组.push_back(std::move(*项));
            }

            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(L1中性一致当前读取状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L1中性一致当前读取状态::资源失败);
        } catch (...) {
            return 失败(L1中性一致当前读取状态::内部不一致);
        }
    }

    // 诊断责任：向上送出；v1 保持原 DTO，仓库在首笔写时同代建立 legacy owner。
    L1中性写入结果 提交中性写集(const L1中性写集请求& 请求) {
        return 提交中性写集实现(请求, std::nullopt);
    }

    L1中性写入首次结果读取结果 读取中性写入首次结果(
        const L1中性写入首次结果读取请求& 请求) const noexcept {
        std::uint64_t 读取代次 = 0;
        const auto 失败 = [&](L1中性写入首次结果读取状态 状态) {
            L1中性写入首次结果读取结果 结果;
            结果.状态 = 状态;
            结果.幂等键 = 请求.幂等键;
            结果.读取事实代次 = 读取代次;
            return 结果;
        };
        if (请求.合同版本 != L1中性写入首次结果读取合同版本
            || !有效(请求.幂等键))
            return 失败(L1中性写入首次结果读取状态::入口拒绝);
        try {
            std::shared_lock<std::shared_mutex> 锁(锁_, std::try_to_lock);
            if (!锁.owns_lock())
                return 失败(L1中性写入首次结果读取状态::资源失败);
            读取代次 = 状态_.事实代次;
            if (状态_.隔离 || !状态完整(状态_))
                return 失败(L1中性写入首次结果读取状态::内部错误);
            const auto 记录 = 状态_.中性幂等账.find(请求.幂等键.值);
            if (记录 == 状态_.中性幂等账.end())
                return 失败(L1中性写入首次结果读取状态::未找到);
            const auto& 首次 = 记录->second;
            if (首次.首次规范化写集.幂等键 != 请求.幂等键
                || 首次.首次发布事实代次 == 0
                || 首次.首次发布事实代次 > 读取代次
                || 首次.首次结果.状态 != L1中性写入状态::成功
                || 首次.首次结果.事实代次 != 首次.首次发布事实代次
                || 首次.首次结果.新编码映射 != 首次.首次新编码映射)
                return 失败(L1中性写入首次结果读取状态::内部错误);
            L1中性写入首次结果读取结果 结果;
            结果.状态 = L1中性写入首次结果读取状态::已读取;
            结果.幂等键 = 请求.幂等键;
            结果.读取事实代次 = 读取代次;
            结果.首次规范请求等价材料 = 首次.首次规范化写集;
            结果.首次状态 = 首次.首次结果.状态;
            结果.首次事实代次 = 首次.首次发布事实代次;
            结果.首次稳定编码映射 = 首次.首次新编码映射;
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(L1中性写入首次结果读取状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L1中性写入首次结果读取状态::资源失败);
        } catch (...) {
            return 失败(L1中性写入首次结果读取状态::内部错误);
        }
    }

    L1物理清理结果 执行L1物理清理(
        const L1物理清理请求& 请求) noexcept {
        const auto 失败 = [&](L1物理清理状态 状态,
            std::uint64_t 清理代次 = 0) {
            L1物理清理结果 结果;
            结果.状态 = 状态;
            结果.幂等身份 = 请求.幂等身份;
            结果.物理清理事实代次 = 清理代次;
            return 结果;
        };
        if (!L1物理清理请求规范有序(请求))
            return 失败(L1物理清理状态::入口拒绝);
        try {
            std::unique_lock<std::shared_mutex> 锁(锁_, std::try_to_lock);
            if (!锁.owns_lock()) return 失败(L1物理清理状态::资源失败);
            if (状态_.隔离 || !状态完整(状态_))
                return 失败(L1物理清理状态::内部错误);
            if (const auto 既有 = 状态_.物理清理幂等账.find(请求.幂等身份.值);
                既有 != 状态_.物理清理幂等账.end()) {
                if (既有->second.首次规范请求 != 请求)
                    return 失败(L1物理清理状态::同键冲突);
                if (既有->second.首次状态 != L1物理清理状态::已清理
                    || !既有->second.已物理清理
                    || 既有->second.首次物理清理事实代次 == 0
                    || 既有->second.首次稳定编码映射.size()
                        != 请求.待清理事实身份组.size())
                    return 失败(L1物理清理状态::内部错误);
                L1物理清理结果 结果;
                结果.状态 = L1物理清理状态::精确重复;
                结果.幂等身份 = 请求.幂等身份;
                结果.物理清理事实代次 =
                    既有->second.首次物理清理事实代次;
                结果.稳定编码映射 = 既有->second.首次稳定编码映射;
                结果.墓碑组.reserve(结果.稳定编码映射.size());
                for (const auto& [身份, 编码] : 结果.稳定编码映射) {
                    if (身份.编码 != 编码)
                        return 失败(L1物理清理状态::内部错误);
                    const auto 墓碑 = 状态_.物理清理墓碑.find(编码.值);
                    if (墓碑 == 状态_.物理清理墓碑.end()
                        || 墓碑->second.事实种类 != 身份.事实种类
                        || 墓碑->second.物理清理事实代次
                            != 结果.物理清理事实代次)
                        return 失败(L1物理清理状态::内部错误);
                    结果.墓碑组.push_back(墓碑->second);
                }
                return 结果;
            }
            if (状态_.事实代次 != 请求.期望事实代次)
                return 失败(L1物理清理状态::事实代次漂移,
                    状态_.事实代次);
            if (状态_.事实代次 == std::numeric_limits<std::uint64_t>::max())
                return 失败(L1物理清理状态::内部错误);

            std::unordered_set<std::uint64_t> 清理编码;
            清理编码.reserve(请求.待清理事实身份组.size());
            std::vector<L1物理清理墓碑> 墓碑组;
            std::vector<std::pair<L1物理清理事实身份, 稳定编码>> 映射;
            墓碑组.reserve(请求.待清理事实身份组.size());
            映射.reserve(请求.待清理事实身份组.size());
            const auto 清理代次 = 状态_.事实代次 + 1;
            for (const auto& 身份 : 请求.待清理事实身份组) {
                if (!清理编码.insert(身份.编码.值).second)
                    return 失败(L1物理清理状态::入口拒绝);
                if (状态_.当前节点.contains(身份.编码.值)
                    || 状态_.当前关系.contains(身份.编码.值)
                    || 状态_.当前值.contains(身份.编码.值))
                    return 失败(L1物理清理状态::尚未退出);
                const auto 历史 = 状态_.历史.find(身份.编码.值);
                if (历史 == 状态_.历史.end())
                    return 失败(L1物理清理状态::未找到);
                const auto 墓碑 = std::visit([&](const auto& 事实)
                    -> std::optional<L1物理清理墓碑> {
                    using T = std::decay_t<decltype(事实)>;
                    constexpr auto 种类 = std::is_same_v<T, 节点事实>
                        ? L1物理清理事实种类::节点
                        : std::is_same_v<T, 关系事实>
                            ? L1物理清理事实种类::关系
                            : L1物理清理事实种类::值;
                    if (身份.事实种类 != 种类 || 事实.编码 != 身份.编码
                        || !事实.退出事实代次
                        || 事实.创建事实代次 == 0
                        || 事实.创建事实代次 > *事实.退出事实代次
                        || *事实.退出事实代次 > 状态_.事实代次
                        || !有效(事实.写入所有者)) return std::nullopt;
                    return L1物理清理墓碑{事实.编码, 种类,
                        事实.写入所有者.编码, 事实.创建事实代次,
                        *事实.退出事实代次, 清理代次};
                }, 历史->second.事实);
                if (!墓碑) return 失败(L1物理清理状态::未找到);
                墓碑组.push_back(*墓碑);
                映射.push_back({身份, 身份.编码});
            }

            const auto 引用待清理 = [&](稳定编码 编码) noexcept {
                return 清理编码.contains(编码.值);
            };
            const auto 事实引用待清理 = [&](const auto& 事实) noexcept {
                using T = std::decay_t<decltype(事实)>;
                if constexpr (std::is_same_v<T, 节点事实>) {
                    for (const auto& 槽 : 事实.当前属性)
                        if (引用待清理(槽.属性类型节点)
                            || 引用待清理(槽.当前值)) return true;
                    return false;
                } else if constexpr (std::is_same_v<T, 关系事实>) {
                    return 引用待清理(事实.源节点)
                        || 引用待清理(事实.目标节点)
                        || 引用待清理(事实.关系类型节点);
                } else {
                    if (引用待清理(事实.所属节点)
                        || 引用待清理(事实.属性类型节点)
                        || 引用待清理(事实.来源节点)) return true;
                    const auto* 独立 = std::get_if<独立材料引用>(&事实.材料);
                    return 独立 && 引用待清理(独立->编码);
                }
            };
            for (const auto& [_, 事实] : 状态_.当前节点)
                if (事实引用待清理(事实))
                    return 失败(L1物理清理状态::引用冲突);
            for (const auto& [_, 事实] : 状态_.当前关系)
                if (事实引用待清理(事实))
                    return 失败(L1物理清理状态::引用冲突);
            for (const auto& [_, 事实] : 状态_.当前值)
                if (事实引用待清理(事实))
                    return 失败(L1物理清理状态::引用冲突);
            for (const auto& [编码, 历史] : 状态_.历史)
                if (!清理编码.contains(编码)
                    && std::visit(事实引用待清理, 历史.事实))
                    return 失败(L1物理清理状态::引用冲突);

            状态 候选 = 状态_;
            const auto 删除候选索引 = [](auto& 索引, std::uint64_t 第一,
                std::uint64_t 第二, std::uint64_t 编码) noexcept {
                const auto 一级 = 索引.find(第一);
                if (一级 == 索引.end()) return;
                const auto 二级 = 一级->second.find(第二);
                if (二级 == 一级->second.end()) return;
                const auto 位置 = std::lower_bound(
                    二级->second.begin(), 二级->second.end(), 编码);
                if (位置 != 二级->second.end() && *位置 == 编码)
                    二级->second.erase(位置);
                if (二级->second.empty()) 一级->second.erase(二级);
                if (一级->second.empty()) 索引.erase(一级);
            };
            for (const auto& 身份 : 请求.待清理事实身份组) {
                const auto 历史 = 候选.历史.find(身份.编码.值);
                if (历史 == 候选.历史.end())
                    return 失败(L1物理清理状态::内部错误);
                std::visit([&](const auto& 事实) {
                    using T = std::decay_t<decltype(事实)>;
                    if constexpr (std::is_same_v<T, 关系事实>) {
                        删除候选索引(候选.历史源关系候选索引,
                            事实.源节点.值, 事实.关系类型节点.值, 事实.编码.值);
                        删除候选索引(候选.历史目标关系候选索引,
                            事实.目标节点.值, 事实.关系类型节点.值, 事实.编码.值);
                    } else if constexpr (std::is_same_v<T, 值事实>) {
                        删除候选索引(候选.历史属性槽值候选索引,
                            事实.所属节点.值, 事实.属性类型节点.值, 事实.编码.值);
                    }
                }, 历史->second.事实);
                候选.历史.erase(历史);
            }
            for (const auto& 墓碑 : 墓碑组)
                if (!候选.物理清理墓碑.emplace(墓碑.编码.值, 墓碑).second)
                    return 失败(L1物理清理状态::内部错误);
            候选.事实代次 = 清理代次;
            if (!候选.物理清理幂等账.emplace(请求.幂等身份.值,
                    物理清理幂等记录{请求, L1物理清理状态::已清理,
                        清理代次, 映射, true}).second)
                return 失败(L1物理清理状态::内部错误);
            if (!状态完整(候选))
                return 失败(L1物理清理状态::内部错误);
            const auto 持久准备 = 准备持久发布(候选);
            if (持久准备 != 持久准备状态::成功)
                return 失败(持久准备 == 持久准备状态::资源失败
                    ? L1物理清理状态::资源失败
                    : L1物理清理状态::内部错误);
            std::swap(状态_, 候选);

            L1物理清理结果 结果;
            结果.状态 = L1物理清理状态::已清理;
            结果.幂等身份 = 请求.幂等身份;
            结果.物理清理事实代次 = 清理代次;
            结果.是否形成内存权威发布 = true;
            结果.稳定编码映射 = std::move(映射);
            结果.墓碑组 = std::move(墓碑组);
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(L1物理清理状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L1物理清理状态::资源失败);
        } catch (...) {
            return 失败(L1物理清理状态::内部错误);
        }
    }

    // 诊断责任：向上送出；owner 由实例绑定端口注入，请求不含可写 owner 字段。
    L1所有者范围写入结果 提交所有者范围中性写集(
        L1结构所有者身份 所有者,
        const L1所有者范围写集请求& 请求) {
        const auto 失败 = [&](L1所有者范围写入状态 状态,
            std::uint64_t 代次 = 0) {
            return L1所有者范围写入结果{状态,
                L1所有者范围CRUD合同版本, 所有者, 请求.写入幂等身份,
                代次, false,
                L1所有者范围重试边界::修正请求后可重试, {}};
        };
        if (请求.合同版本 != L1所有者范围CRUD合同版本
            || !有效(所有者) || !有效(请求.写入幂等身份))
            return 失败(L1所有者范围写入状态::入口拒绝);
        try {
            L1中性写集请求 中性;
            中性.合同版本 = L1中性CRUD合同版本;
            中性.期望事实代次 = 请求.期望事实代次;
            中性.幂等键 = {请求.写入幂等身份.值};
            const auto 转换引用 = [](const L1所有者范围事实引用& 引用)
                -> L1中性事实引用 {
                return std::visit([](const auto& 值) -> L1中性事实引用 {
                    using 类型 = std::decay_t<decltype(值)>;
                    if constexpr (std::is_same_v<类型, 稳定编码>) return 值;
                    else return L1中性写集本地键{值.值};
                }, 引用);
            };
            const auto 转换表示 = [](std::optional<L1所有者范围值表示种类> 表示)
                -> std::optional<L1中性值表示种类> {
                if (!表示) return std::nullopt;
                return static_cast<L1中性值表示种类>(static_cast<std::uint8_t>(*表示));
            };
            const auto 转换材料 = [](const L1所有者范围原始值材料& 材料)
                -> L1中性原始值材料 {
                return std::visit([](const auto& 值) -> L1中性原始值材料 {
                    using 类型 = std::decay_t<decltype(值)>;
                    if constexpr (std::is_same_v<类型,
                        L1所有者范围独立材料引用>)
                        return L1中性独立材料引用{值.编码};
                    else return 值;
                }, 材料);
            };
            中性.节点.reserve(请求.节点.size());
            for (const auto& 项 : 请求.节点)
                中性.节点.push_back({{项.本地键.值}, 项.种类,
                    转换表示(项.属性类型表示)});
            中性.关系.reserve(请求.关系.size());
            for (const auto& 项 : 请求.关系)
                中性.关系.push_back({{项.本地键.值}, 转换引用(项.源节点),
                    转换引用(项.目标节点), 转换引用(项.关系类型节点),
                    项.角色或顺序});
            中性.值.reserve(请求.值.size());
            for (const auto& 项 : 请求.值)
                中性.值.push_back({{项.本地键.值}, 转换引用(项.所属节点),
                    转换引用(项.属性类型节点), 转换材料(项.材料),
                    转换引用(项.来源节点)});
            中性.属性槽变更.reserve(请求.属性槽变更.size());
            for (const auto& 项 : 请求.属性槽变更)
            中性.属性槽变更.push_back({转换引用(项.所属节点),
                    转换引用(项.属性类型节点), {项.新当前值.值}});
            中性.退出事实 = 请求.退出事实;
            const auto 内部 = 提交中性写集实现(中性, 所有者);
            const auto 映射状态 = [&](L1中性写入状态 状态) {
                switch (状态) {
                case L1中性写入状态::成功: return L1所有者范围写入状态::成功;
                case L1中性写入状态::精确重复:
                    return L1所有者范围写入状态::精确重复;
                case L1中性写入状态::入口拒绝:
                    return L1所有者范围写入状态::入口拒绝;
                case L1中性写入状态::未找到:
                    return L1所有者范围写入状态::未找到;
                case L1中性写入状态::已退出:
                    return L1所有者范围写入状态::已退出;
                case L1中性写入状态::事实代次漂移:
                    return L1所有者范围写入状态::事实代次漂移;
                case L1中性写入状态::幂等冲突:
                    return L1所有者范围写入状态::幂等冲突;
                case L1中性写入状态::引用冲突:
                    return L1所有者范围写入状态::引用冲突;
                case L1中性写入状态::资源失败:
                    return L1所有者范围写入状态::资源失败;
                case L1中性写入状态::内部不一致:
                    return L1所有者范围写入状态::内部不一致;
                }
                return L1所有者范围写入状态::内部不一致;
            };
            L1所有者范围写入结果 结果{映射状态(内部.状态),
                L1所有者范围CRUD合同版本, 所有者, 请求.写入幂等身份,
                内部.事实代次, 内部.是否形成内存权威发布,
                static_cast<L1所有者范围重试边界>(
                    static_cast<std::uint8_t>(内部.重试边界)), {}};
            结果.新编码映射.reserve(内部.新编码映射.size());
            for (const auto& [键, 编码] : 内部.新编码映射)
                结果.新编码映射.push_back({{键.值}, 编码});
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(L1所有者范围写入状态::资源失败);
        } catch (...) {
            return 失败(L1所有者范围写入状态::内部不一致);
        }
    }

    // ARCH-L2 唯一跨 owner 仓库提交点。公开请求以具名状态写集和动态写集
    // 固定参与者身份，不接受调用者提供的参与者顺序；本实现唯一规范顺序为
    // 状态参与者先、动态参与者后。两个参与者只在私有候选状态中形成，
    // 交换前统一为状态写入形成的单一 G1；普通 owner 入口仍保持一次一 owner 语义。
    L1跨所有者原子事务结果 提交跨所有者原子事务(
        const L1跨所有者原子事务请求& 请求) {
        const auto 失败 = [&](L1跨所有者原子事务状态 状态,
            std::uint64_t 代次 = 0) {
            return L1跨所有者原子事务结果{状态,
                L1跨所有者原子事务合同版本, 代次, false,
                L1所有者范围重试边界::修正请求后可重试, {}, {}};
        };
        if (请求.合同版本 != L1跨所有者原子事务合同版本
            || 请求.共同期望事实代次 == 0
            || !有效(请求.组合写入幂等身份)
            || 请求.状态写集.参与者
                != L1跨所有者原子事务参与者序号::状态
            || 请求.动态写集.参与者
                != L1跨所有者原子事务参与者序号::动态
            || !有效(请求.状态写集.所有者)
            || !有效(请求.动态写集.所有者)
            || 请求.状态写集.所有者 == 请求.动态写集.所有者)
            return 失败(L1跨所有者原子事务状态::入口拒绝);
        try {
            std::unique_lock<std::shared_mutex> 锁(锁_);
            if (状态_.隔离 || !状态完整(状态_))
                return 失败(L1跨所有者原子事务状态::内部不一致,
                    状态_.事实代次);
            if (const auto 既有 = 状态_.跨所有者原子事务幂等账.find(
                    请求.组合写入幂等身份.值);
                既有 != 状态_.跨所有者原子事务幂等账.end()) {
                if (!(既有->second.请求 == 请求))
                    return 失败(L1跨所有者原子事务状态::幂等冲突,
                        状态_.事实代次);
                // 精确重复只投影首次结果；在任何候选构造前返回，因此零应用、
                // 零新代次，并完整保留首次 G1 与两份稳定编码映射。
                auto 结果 = 既有->second.结果;
                结果.状态 = L1跨所有者原子事务状态::精确重复;
                结果.是否形成内存权威发布 = false;
                结果.重试边界 = L1所有者范围重试边界::原幂等身份读回收敛;
                return 结果;
            }
            if (请求.共同期望事实代次 != 状态_.事实代次)
                return 失败(L1跨所有者原子事务状态::事实代次漂移,
                    状态_.事实代次);

            // 在同一仓库锁保护下使用候选副本执行两个 owner 写集。
            // 跨参与者引用必须带有参与者序号和局部键，并在候选副本
            // 中解析为本次事务产生的正式稳定编码；普通 owner CRUD
            // 永远不会接收这种引用。
            const auto 转换写集 = [&](const L1跨所有者原子写集请求& 原,
                const std::vector<std::pair<L1所有者范围写集本地键,
                    稳定编码>>& 状态映射,
                const std::vector<std::pair<L1所有者范围写集本地键,
                    稳定编码>>& 动态映射,
                L1跨所有者原子事务参与者序号 当前)
                -> std::optional<L1所有者范围写集请求> {
                auto 查局部 = [&](L1跨所有者原子事务参与者序号 参与者,
                    L1所有者范围写集本地键 键) -> std::optional<稳定编码> {
                    const auto& 映射 = 参与者 == L1跨所有者原子事务参与者序号::状态
                        ? 状态映射 : 动态映射;
                    for (const auto& [本地, 编码] : 映射)
                        if (本地 == 键) return 编码;
                    return std::nullopt;
                };
                auto 转换引用 = [&](const L1跨所有者原子事实引用值& 引用)
                    -> std::optional<L1所有者范围事实引用> {
                    return std::visit([&](const auto& 值)
                        -> std::optional<L1所有者范围事实引用> {
                        using 类型 = std::decay_t<decltype(值)>;
                        if constexpr (std::is_same_v<类型, 稳定编码>)
                            return 值;
                        else if constexpr (std::is_same_v<类型,
                            L1所有者范围写集本地键>) return 值;
                        else {
                            if (值.参与者 == 当前) return std::nullopt;
                            const auto 编码 = 查局部(值.参与者, 值.本地键);
                            return 编码 ? std::optional<L1所有者范围事实引用>{*编码}
                                : std::nullopt;
                        }
                    }, 引用);
                };
                L1所有者范围写集请求 结果;
                结果.合同版本 = 原.合同版本;
                结果.期望事实代次 = 原.期望事实代次;
                结果.写入幂等身份 = 原.写入幂等身份;
                结果.退出事实 = 原.退出事实;
                for (const auto& 项 : 原.节点)
                    结果.节点.push_back({项.本地键, 项.种类, 项.属性类型表示});
                for (const auto& 项 : 原.关系) {
                    const auto 源 = 转换引用(项.源节点);
                    const auto 目标 = 转换引用(项.目标节点);
                    const auto 类型 = 转换引用(项.关系类型节点);
                    if (!源 || !目标 || !类型) return std::nullopt;
                    结果.关系.push_back({项.本地键, *源, *目标, *类型,
                        项.角色或顺序});
                }
                for (const auto& 项 : 原.值) {
                    const auto 所属 = 转换引用(项.所属节点);
                    const auto 类型 = 转换引用(项.属性类型节点);
                    const auto 来源 = 转换引用(项.来源节点);
                    if (!所属 || !类型 || !来源) return std::nullopt;
                    结果.值.push_back({项.本地键, *所属, *类型,
                        项.材料, *来源});
                }
                for (const auto& 项 : 原.属性槽变更) {
                    const auto 所属 = 转换引用(项.所属节点);
                    const auto 类型 = 转换引用(项.属性类型节点);
                    if (!所属 || !类型) return std::nullopt;
                    结果.属性槽变更.push_back({*所属, *类型,
                        项.新当前值});
                }
                const auto 按本地键 = [](const auto& 左, const auto& 右) {
                    return 左.本地键.值 < 右.本地键.值;
                };
                std::sort(结果.节点.begin(), 结果.节点.end(), 按本地键);
                std::sort(结果.关系.begin(), 结果.关系.end(), 按本地键);
                std::sort(结果.值.begin(), 结果.值.end(), 按本地键);
                std::sort(结果.退出事实.begin(), 结果.退出事实.end());
                return 结果;
            };
            L1事实基座仓库 候选仓库;
            候选仓库.状态_ = 状态_;
            // 具名参与者和入口校验已经冻结唯一顺序。状态映射先形成，动态写集
            // 只能据此解析跨参与者引用；调用者无法通过排列输入改变编码分配顺序。
            const auto 状态请求 = 转换写集(请求.状态写集.写集, {}, {},
                L1跨所有者原子事务参与者序号::状态);
            if (!状态请求) return 失败(L1跨所有者原子事务状态::引用冲突,
                状态_.事实代次);
            const auto 状态写入 = 候选仓库.提交所有者范围中性写集(
                请求.状态写集.所有者, *状态请求);
            if (状态写入.状态 != L1所有者范围写入状态::成功
                && 状态写入.状态 != L1所有者范围写入状态::精确重复)
                return 失败(static_cast<L1跨所有者原子事务状态>(
                    static_cast<std::uint8_t>(状态写入.状态)),
                    状态_.事实代次);
            auto 动态请求 = 转换写集(请求.动态写集.写集,
                状态写入.新编码映射, {},
                L1跨所有者原子事务参与者序号::动态);
            if (!动态请求) return 失败(L1跨所有者原子事务状态::引用冲突,
                状态_.事实代次);
            // 第二参与者在候选仓库内紧随状态参与者应用；其外部请求仍绑定
            // 原始共同 G0，内部候选写入则必须期望状态参与者刚形成的 G1。
            动态请求->期望事实代次 = 状态写入.事实代次;
            const auto 动态写入 = 候选仓库.提交所有者范围中性写集(
                请求.动态写集.所有者, *动态请求);
            if (动态写入.状态 != L1所有者范围写入状态::成功
                && 动态写入.状态 != L1所有者范围写入状态::精确重复)
                return 失败(static_cast<L1跨所有者原子事务状态>(
                    static_cast<std::uint8_t>(动态写入.状态)),
                    状态_.事实代次);
            if (状态写入.事实代次 == 0 || 动态写入.事实代次 == 0
                || 动态写入.事实代次 != 状态写入.事实代次 + 1)
                return 失败(L1跨所有者原子事务状态::内部不一致,
                    状态_.事实代次);

            const auto 首次代次 = 状态写入.事实代次;
            const auto 二次代次 = 动态写入.事实代次;
            // 两次候选应用只用于获得完整映射和闭合引用。交换前把第二次候选
            // 形成或退出的全部事实投影回首次代次，使对外事务仅形成单一 G1。
            auto 调整代次 = [&](auto& 事实) {
                if (事实.创建事实代次 == 二次代次)
                    事实.创建事实代次 = 首次代次;
                if (事实.退出事实代次 && *事实.退出事实代次 == 二次代次)
                    *事实.退出事实代次 = 首次代次;
            };
            for (auto& [_, 事实] : 候选仓库.状态_.当前所有者) 调整代次(事实);
            for (auto& [_, 事实] : 候选仓库.状态_.历史所有者) 调整代次(事实);
            for (auto& [_, 事实] : 候选仓库.状态_.当前节点) 调整代次(事实);
            for (auto& [_, 事实] : 候选仓库.状态_.当前关系) 调整代次(事实);
            for (auto& [_, 事实] : 候选仓库.状态_.当前值) 调整代次(事实);
            for (auto& [_, 副本] : 候选仓库.状态_.历史)
                std::visit([&](auto& 事实) { 调整代次(事实); }, 副本.事实);
            // 候选仓库借用普通 owner 写入算法形成两份局部映射；正式交换是
            // 一个跨 owner 事务，因此两份 owner 首次写入账也必须投影到同一
            // 原始 G0 -> G1，而不能泄漏候选内部的 G2。
            const auto 规范化参与者账 = [&](const auto& 参与者) {
                const auto 账组 = 候选仓库.状态_.所有者范围幂等账.find(
                    参与者.所有者.编码.值);
                if (账组 == 候选仓库.状态_.所有者范围幂等账.end())
                    return false;
                const auto 记录 = 账组->second.find(
                    参与者.写集.写入幂等身份.值);
                if (记录 == 账组->second.end()) return false;
                记录->second.首次规范化写集.期望事实代次 =
                    请求.共同期望事实代次;
                记录->second.首次发布事实代次 = 首次代次;
                记录->second.首次结果.事实代次 = 首次代次;
                return true;
            };
            if (!规范化参与者账(请求.状态写集)
                || !规范化参与者账(请求.动态写集))
                return 失败(L1跨所有者原子事务状态::内部不一致,
                    状态_.事实代次);
            候选仓库.状态_.事实代次 = 首次代次;

            L1跨所有者原子事务结果 结果{
                状态写入.状态 == L1所有者范围写入状态::精确重复
                    && 动态写入.状态 == L1所有者范围写入状态::精确重复
                    ? L1跨所有者原子事务状态::精确重复
                    : L1跨所有者原子事务状态::已提交,
                L1跨所有者原子事务合同版本, 首次代次,
                状态写入.是否形成内存权威发布
                    || 动态写入.是否形成内存权威发布,
                L1所有者范围重试边界::不适用,
                状态写入.新编码映射, 动态写入.新编码映射};
            if (!状态完整(候选仓库.状态_))
                return 失败(L1跨所有者原子事务状态::内部不一致,
                    状态_.事实代次);
            候选仓库.状态_.跨所有者原子事务幂等账.emplace(
                请求.组合写入幂等身份.值,
                跨所有者原子事务幂等记录{请求, 结果});
            const auto 持久准备 = 准备持久发布(候选仓库.状态_);
            if (持久准备 != 持久准备状态::成功)
                return 失败(持久准备 == 持久准备状态::资源失败
                    ? L1跨所有者原子事务状态::资源失败
                    : L1跨所有者原子事务状态::内部不一致);
            状态_ = std::move(候选仓库.状态_);
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(L1跨所有者原子事务状态::资源失败, 状态_.事实代次);
        } catch (...) {
            return 失败(L1跨所有者原子事务状态::内部不一致, 状态_.事实代次);
        }
    }

    L1三分区原子事务结果_v2 提交三分区原子事务_v2(
        const L1三分区原子事务请求_v2& 请求) noexcept {
        bool 已进入交换边界 = false;
        std::uint64_t 候选共同代次 = 0;
        const auto 失败 = [&](L1三分区原子事务状态_v2 失败状态,
            std::uint64_t 代次 = 0,
            L1所有者范围重试边界 重试边界 =
                L1所有者范围重试边界::修正请求后可重试) {
            return L1三分区原子事务结果_v2{失败状态,
                L1三分区原子事务合同版本_v2, 请求.组合写入幂等身份,
                代次, false, 重试边界, {}};
        };
        try {
            if (请求.合同版本 != L1三分区原子事务合同版本_v2
                || 请求.共同期望事实代次 == 0
                || !有效(请求.组合写入幂等身份)
                || 请求.参与者写集组.size() != 3)
                return 失败(L1三分区原子事务状态_v2::入口拒绝);

            auto 规范请求 = 请求;
            const auto 引用排序键 = [](const L1三分区原子事实引用值_v2& 引用) {
                std::array<std::uint64_t, 3> 键{};
                if (const auto* 稳定项 = std::get_if<稳定编码>(&引用))
                    键 = {1, 稳定项->值, 0};
                else if (const auto* 本地项 =
                    std::get_if<L1所有者范围写集本地键>(&引用))
                    键 = {2, 本地项->值, 0};
                else {
                    const auto& 跨项 = std::get<L1三分区原子事实引用_v2>(引用);
                    键 = {3, 跨项.参与者.值, 跨项.本地键.值};
                }
                return 键;
            };
            std::sort(规范请求.参与者写集组.begin(),
                规范请求.参与者写集组.end(), [](const auto& 左, const auto& 右) {
                    return 左.参与者.值 < 右.参与者.值;
                });
            for (auto& 参与者 : 规范请求.参与者写集组) {
                const auto 按本地键 = [](const auto& 左, const auto& 右) {
                    return 左.本地键.值 < 右.本地键.值;
                };
                std::sort(参与者.写集.节点.begin(), 参与者.写集.节点.end(), 按本地键);
                std::sort(参与者.写集.关系.begin(), 参与者.写集.关系.end(), 按本地键);
                std::sort(参与者.写集.值.begin(), 参与者.写集.值.end(), 按本地键);
                std::sort(参与者.写集.退出事实.begin(),
                    参与者.写集.退出事实.end());
                std::sort(参与者.写集.属性槽变更.begin(),
                    参与者.写集.属性槽变更.end(), [&](const auto& 左, const auto& 右) {
                        const auto 左所属 = 引用排序键(左.所属节点);
                        const auto 右所属 = 引用排序键(右.所属节点);
                        if (左所属 != 右所属) return 左所属 < 右所属;
                        const auto 左类型 = 引用排序键(左.属性类型节点);
                        const auto 右类型 = 引用排序键(右.属性类型节点);
                        if (左类型 != 右类型) return 左类型 < 右类型;
                        return 左.新当前值.值 < 右.新当前值.值;
                    });
            }

            std::array<std::unordered_set<std::uint32_t>, 3> 节点键组;
            std::array<std::unordered_set<std::uint32_t>, 3> 值键组;
            std::unordered_set<std::uint64_t> 所有者组;
            std::unordered_set<std::uint64_t> 内部幂等组;
            for (std::size_t 序号 = 0; 序号 != 3; ++序号) {
                const auto& 参与者 = 规范请求.参与者写集组[序号];
                const auto& 写集 = 参与者.写集;
                if (参与者.参与者.值 != 序号 + 1
                    || !有效(参与者.所有者)
                    || !所有者组.insert(参与者.所有者.编码.值).second
                    || 写集.合同版本 != L1所有者范围CRUD合同版本
                    || 写集.期望事实代次 != 请求.共同期望事实代次
                    || !有效(写集.写入幂等身份)
                    || !内部幂等组.insert(写集.写入幂等身份.值).second
                    || (写集.节点.empty() && 写集.关系.empty()
                        && 写集.值.empty() && 写集.属性槽变更.empty()
                        && 写集.退出事实.empty()))
                    return 失败(L1三分区原子事务状态_v2::入口拒绝);

                std::unordered_set<std::uint32_t> 全部本地键;
                for (const auto& 项 : 写集.节点) {
                    if (项.本地键.值 == 0
                        || !全部本地键.insert(项.本地键.值).second
                        || !节点键组[序号].insert(项.本地键.值).second)
                        return 失败(L1三分区原子事务状态_v2::入口拒绝);
                }
                for (const auto& 项 : 写集.关系)
                    if (项.本地键.值 == 0
                        || !全部本地键.insert(项.本地键.值).second)
                        return 失败(L1三分区原子事务状态_v2::入口拒绝);
                for (const auto& 项 : 写集.值) {
                    if (项.本地键.值 == 0
                        || !全部本地键.insert(项.本地键.值).second
                        || !值键组[序号].insert(项.本地键.值).second)
                        return 失败(L1三分区原子事务状态_v2::入口拒绝);
                }
                for (std::size_t i = 0; i < 写集.退出事实.size(); ++i)
                    if (!有效(写集.退出事实[i])
                        || (i != 0 && 写集.退出事实[i - 1] == 写集.退出事实[i]))
                        return 失败(L1三分区原子事务状态_v2::入口拒绝);
                for (std::size_t i = 1; i < 写集.属性槽变更.size(); ++i) {
                    const auto& 前 = 写集.属性槽变更[i - 1];
                    const auto& 后 = 写集.属性槽变更[i];
                    if (引用排序键(前.所属节点) == 引用排序键(后.所属节点)
                        && 引用排序键(前.属性类型节点)
                            == 引用排序键(后.属性类型节点))
                        return 失败(L1三分区原子事务状态_v2::入口拒绝);
                }
            }

            std::unique_lock<std::shared_mutex> 锁(锁_);
            const auto 引用有效 = [&](const L1三分区原子事实引用值_v2& 引用,
                std::size_t 当前序号) {
                if (const auto* 稳定项 = std::get_if<稳定编码>(&引用))
                    return 有效(*稳定项);
                if (const auto* 本地项 =
                    std::get_if<L1所有者范围写集本地键>(&引用))
                    return 本地项->值 != 0
                        && 节点键组[当前序号].contains(本地项->值);
                const auto& 跨项 = std::get<L1三分区原子事实引用_v2>(引用);
                return 跨项.参与者.值 >= 1
                    && 跨项.参与者.值 <= 3
                    && 跨项.参与者.值 < 当前序号 + 1
                    && 跨项.本地键.值 != 0
                    && 节点键组[跨项.参与者.值 - 1].contains(跨项.本地键.值);
            };
            for (std::size_t 序号 = 0; 序号 != 3; ++序号) {
                const auto& 写集 = 规范请求.参与者写集组[序号].写集;
                for (const auto& 项 : 写集.关系)
                    if (!引用有效(项.源节点, 序号)
                        || !引用有效(项.目标节点, 序号)
                        || !引用有效(项.关系类型节点, 序号))
                        return 失败(L1三分区原子事务状态_v2::引用冲突,
                            状态_.事实代次);
                for (const auto& 项 : 写集.值)
                    if (!引用有效(项.所属节点, 序号)
                        || !引用有效(项.属性类型节点, 序号)
                        || !引用有效(项.来源节点, 序号))
                        return 失败(L1三分区原子事务状态_v2::引用冲突,
                            状态_.事实代次);
                for (const auto& 项 : 写集.属性槽变更)
                    if (!引用有效(项.所属节点, 序号)
                        || !引用有效(项.属性类型节点, 序号)
                        || !值键组[序号].contains(项.新当前值.值))
                        return 失败(L1三分区原子事务状态_v2::引用冲突,
                            状态_.事实代次);
            }

            if (状态_.隔离 || !状态完整(状态_))
                return 失败(L1三分区原子事务状态_v2::内部不一致,
                    状态_.事实代次, L1所有者范围重试边界::不适用);
            if (const auto 既有 = 状态_.三分区原子事务幂等账_v2.find(
                    请求.组合写入幂等身份.值);
                既有 != 状态_.三分区原子事务幂等账_v2.end()) {
                if (!(既有->second.请求 == 规范请求))
                    return 失败(L1三分区原子事务状态_v2::幂等冲突,
                        状态_.事实代次, L1所有者范围重试边界::不适用);
                auto 结果 = 既有->second.结果;
                结果.状态 = L1三分区原子事务状态_v2::精确重复;
                结果.是否已确认形成内存权威发布 = false;
                结果.重试边界 =
                    L1所有者范围重试边界::原幂等身份读回收敛;
                return 结果;
            }
            if (规范请求.共同期望事实代次 != 状态_.事实代次)
                return 失败(L1三分区原子事务状态_v2::事实代次漂移,
                    状态_.事实代次);

            const auto 转换写集 = [&](const L1三分区原子写集请求_v2& 原,
                const std::array<std::vector<std::pair<
                    L1所有者范围写集本地键, 稳定编码>>, 3>& 映射组,
                std::size_t 当前序号) -> std::optional<L1所有者范围写集请求> {
                const auto 查映射 = [&](std::size_t 参与者序号,
                    L1所有者范围写集本地键 本地键) -> std::optional<稳定编码> {
                    const auto& 映射 = 映射组[参与者序号];
                    const auto 位置 = std::lower_bound(映射.begin(), 映射.end(),
                        本地键, [](const auto& 项, const auto 键值) {
                            return 项.first < 键值;
                        });
                    return 位置 != 映射.end() && 位置->first == 本地键
                        ? std::optional<稳定编码>{位置->second} : std::nullopt;
                };
                const auto 转换引用 = [&](const L1三分区原子事实引用值_v2& 引用)
                    -> std::optional<L1所有者范围事实引用> {
                    if (const auto* 稳定项 = std::get_if<稳定编码>(&引用))
                        return *稳定项;
                    if (const auto* 本地项 =
                        std::get_if<L1所有者范围写集本地键>(&引用))
                        return *本地项;
                    const auto& 跨项 = std::get<L1三分区原子事实引用_v2>(引用);
                    if (跨项.参与者.值 == 0
                        || 跨项.参与者.值 > 当前序号)
                        return std::nullopt;
                    const auto 编码 = 查映射(跨项.参与者.值 - 1, 跨项.本地键);
                    return 编码 ? std::optional<L1所有者范围事实引用>{*编码}
                        : std::nullopt;
                };
                L1所有者范围写集请求 结果;
                结果.合同版本 = 原.合同版本;
                结果.期望事实代次 = 原.期望事实代次;
                结果.写入幂等身份 = 原.写入幂等身份;
                结果.退出事实 = 原.退出事实;
                for (const auto& 项 : 原.节点)
                    结果.节点.push_back({项.本地键, 项.种类, 项.属性类型表示});
                for (const auto& 项 : 原.关系) {
                    const auto 源 = 转换引用(项.源节点);
                    const auto 目标 = 转换引用(项.目标节点);
                    const auto 类型 = 转换引用(项.关系类型节点);
                    if (!源 || !目标 || !类型) return std::nullopt;
                    结果.关系.push_back({项.本地键, *源, *目标, *类型,
                        项.角色或顺序});
                }
                for (const auto& 项 : 原.值) {
                    const auto 所属 = 转换引用(项.所属节点);
                    const auto 类型 = 转换引用(项.属性类型节点);
                    const auto 来源 = 转换引用(项.来源节点);
                    if (!所属 || !类型 || !来源) return std::nullopt;
                    结果.值.push_back({项.本地键, *所属, *类型, 项.材料, *来源});
                }
                for (const auto& 项 : 原.属性槽变更) {
                    const auto 所属 = 转换引用(项.所属节点);
                    const auto 类型 = 转换引用(项.属性类型节点);
                    if (!所属 || !类型) return std::nullopt;
                    结果.属性槽变更.push_back({*所属, *类型, 项.新当前值});
                }
                return 结果;
            };
            const auto 映射写入失败 = [](L1所有者范围写入状态 写入状态) {
                switch (写入状态) {
                case L1所有者范围写入状态::入口拒绝:
                    return L1三分区原子事务状态_v2::入口拒绝;
                case L1所有者范围写入状态::事实代次漂移:
                    return L1三分区原子事务状态_v2::事实代次漂移;
                case L1所有者范围写入状态::精确重复:
                case L1所有者范围写入状态::幂等冲突:
                    return L1三分区原子事务状态_v2::幂等冲突;
                case L1所有者范围写入状态::引用冲突:
                    return L1三分区原子事务状态_v2::引用冲突;
                case L1所有者范围写入状态::资源失败:
                    return L1三分区原子事务状态_v2::资源失败;
                default:
                    return L1三分区原子事务状态_v2::内部不一致;
                }
            };
            const auto 映射写入重试边界 = [](L1所有者范围写入状态 写入状态) {
                switch (写入状态) {
                case L1所有者范围写入状态::精确重复:
                case L1所有者范围写入状态::幂等冲突:
                case L1所有者范围写入状态::内部不一致:
                    return L1所有者范围重试边界::不适用;
                case L1所有者范围写入状态::资源失败:
                    return L1所有者范围重试边界::原请求可重试;
                default:
                    return L1所有者范围重试边界::修正请求后可重试;
                }
            };

            L1事实基座仓库 候选仓库;
            候选仓库.状态_ = 状态_;
            std::array<std::vector<std::pair<
                L1所有者范围写集本地键, 稳定编码>>, 3> 映射组;
            std::array<L1所有者范围写入结果, 3> 写入组;
            for (std::size_t 序号 = 0; 序号 != 3; ++序号) {
                auto 普通请求 = 转换写集(
                    规范请求.参与者写集组[序号].写集, 映射组, 序号);
                if (!普通请求)
                    return 失败(L1三分区原子事务状态_v2::引用冲突,
                        状态_.事实代次);
                if (序号 != 0)
                    普通请求->期望事实代次 = 写入组[序号 - 1].事实代次;
                写入组[序号] = 候选仓库.提交所有者范围中性写集(
                    规范请求.参与者写集组[序号].所有者, *普通请求);
                if (写入组[序号].状态 != L1所有者范围写入状态::成功)
                    return 失败(映射写入失败(写入组[序号].状态),
                        状态_.事实代次,
                        映射写入重试边界(写入组[序号].状态));
                映射组[序号] = 写入组[序号].新编码映射;
                if (写入组[序号].事实代次 == 0
                    || (序号 == 0
                        && 写入组[序号].事实代次
                            != 规范请求.共同期望事实代次 + 1)
                    || (序号 != 0
                        && 写入组[序号].事实代次
                            != 写入组[序号 - 1].事实代次 + 1))
                    return 失败(L1三分区原子事务状态_v2::内部不一致,
                        状态_.事实代次, L1所有者范围重试边界::不适用);
            }

            候选共同代次 = 写入组[0].事实代次;
            const auto 第二临时代次 = 写入组[1].事实代次;
            const auto 第三临时代次 = 写入组[2].事实代次;
            const auto 调整代次 = [&](auto& 事实) {
                if (事实.创建事实代次 == 第二临时代次
                    || 事实.创建事实代次 == 第三临时代次)
                    事实.创建事实代次 = 候选共同代次;
                if (事实.退出事实代次
                    && (*事实.退出事实代次 == 第二临时代次
                        || *事实.退出事实代次 == 第三临时代次))
                    *事实.退出事实代次 = 候选共同代次;
            };
            for (auto& [_, 事实] : 候选仓库.状态_.当前所有者) 调整代次(事实);
            for (auto& [_, 事实] : 候选仓库.状态_.历史所有者) 调整代次(事实);
            for (auto& [_, 事实] : 候选仓库.状态_.当前节点) 调整代次(事实);
            for (auto& [_, 事实] : 候选仓库.状态_.当前关系) 调整代次(事实);
            for (auto& [_, 事实] : 候选仓库.状态_.当前值) 调整代次(事实);
            for (auto& [_, 副本] : 候选仓库.状态_.历史)
                std::visit([&](auto& 事实) { 调整代次(事实); }, 副本.事实);

            for (const auto& 参与者 : 规范请求.参与者写集组) {
                const auto 账组 = 候选仓库.状态_.所有者范围幂等账.find(
                    参与者.所有者.编码.值);
                if (账组 == 候选仓库.状态_.所有者范围幂等账.end())
                    return 失败(L1三分区原子事务状态_v2::内部不一致,
                        状态_.事实代次, L1所有者范围重试边界::不适用);
                const auto 记录 = 账组->second.find(参与者.写集.写入幂等身份.值);
                if (记录 == 账组->second.end())
                    return 失败(L1三分区原子事务状态_v2::内部不一致,
                        状态_.事实代次, L1所有者范围重试边界::不适用);
                记录->second.首次规范化写集.期望事实代次 =
                    规范请求.共同期望事实代次;
                记录->second.首次发布事实代次 = 候选共同代次;
                记录->second.首次结果.事实代次 = 候选共同代次;
            }
            候选仓库.状态_.事实代次 = 候选共同代次;

            L1三分区原子事务结果_v2 结果{
                L1三分区原子事务状态_v2::已提交,
                L1三分区原子事务合同版本_v2,
                规范请求.组合写入幂等身份,
                候选共同代次, true, L1所有者范围重试边界::不适用,
                {
                    {规范请求.参与者写集组[0].参与者,
                        规范请求.参与者写集组[0].所有者, 映射组[0]},
                    {规范请求.参与者写集组[1].参与者,
                        规范请求.参与者写集组[1].所有者, 映射组[1]},
                    {规范请求.参与者写集组[2].参与者,
                        规范请求.参与者写集组[2].所有者, 映射组[2]}
                }};
            if (!候选仓库.状态_.三分区原子事务幂等账_v2.emplace(
                    规范请求.组合写入幂等身份.值,
                    三分区原子事务幂等记录_v2{规范请求, 结果}).second
                || !状态完整(候选仓库.状态_))
                return 失败(L1三分区原子事务状态_v2::内部不一致,
                    状态_.事实代次, L1所有者范围重试边界::不适用);
            const auto 持久准备 = 准备持久发布(候选仓库.状态_);
            if (持久准备 != 持久准备状态::成功)
                return 失败(持久准备 == 持久准备状态::资源失败
                        ? L1三分区原子事务状态_v2::资源失败
                        : L1三分区原子事务状态_v2::内部不一致,
                    状态_.事实代次,
                    持久准备 == 持久准备状态::资源失败
                        ? L1所有者范围重试边界::原请求可重试
                        : L1所有者范围重试边界::不适用);
            已进入交换边界 = true;
            状态_ = std::move(候选仓库.状态_);
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(已进入交换边界
                    ? L1三分区原子事务状态_v2::已可能发布
                    : L1三分区原子事务状态_v2::资源失败,
                已进入交换边界 ? 候选共同代次 : 0,
                已进入交换边界
                    ? L1所有者范围重试边界::原幂等身份读回收敛
                    : L1所有者范围重试边界::原请求可重试);
        } catch (...) {
            return 失败(已进入交换边界
                    ? L1三分区原子事务状态_v2::已可能发布
                    : L1三分区原子事务状态_v2::内部不一致,
                已进入交换边界 ? 候选共同代次 : 0,
                已进入交换边界
                    ? L1所有者范围重试边界::原幂等身份读回收敛
                    : L1所有者范围重试边界::不适用);
        }
    }

    L1有限N分区原子事务结果_v3 提交有限N分区原子事务_v3(
        const L1有限N分区原子事务请求_v3& 请求) noexcept {
        bool 已进入交换边界 = false;
        std::uint64_t 候选共同代次 = 0;
        const auto 失败 = [&](L1有限N分区原子事务状态_v3 失败状态,
            std::uint64_t 代次 = 0,
            L1所有者范围重试边界 重试边界 =
                L1所有者范围重试边界::修正请求后可重试) {
            return L1有限N分区原子事务结果_v3{失败状态,
                L1有限N分区原子事务合同版本_v3, 请求.组合写入幂等身份,
                代次, false, 重试边界, {}};
        };
        try {
            const auto 参与者数 = 请求.参与者写集组.size();
            if (请求.合同版本 != L1有限N分区原子事务合同版本_v3
                || 请求.共同期望事实代次 == 0
                || !有效(请求.组合写入幂等身份)
                || 参与者数 < L1有限N分区原子事务最小参与者数_v3
                || 参与者数 > L1有限N分区原子事务最大参与者数_v3)
                return 失败(L1有限N分区原子事务状态_v3::入口拒绝);

            auto 规范请求 = 请求;
            const auto 引用排序键 = [](const L1有限N分区原子事实引用值_v3& 引用) {
                std::array<std::uint64_t, 3> 键{};
                if (const auto* 稳定项 = std::get_if<稳定编码>(&引用))
                    键 = {1, 稳定项->值, 0};
                else if (const auto* 本地项 =
                    std::get_if<L1所有者范围写集本地键>(&引用))
                    键 = {2, 本地项->值, 0};
                else {
                    const auto& 跨项 = std::get<L1有限N分区原子事实引用_v3>(引用);
                    键 = {3, 跨项.参与者.值, 跨项.本地键.值};
                }
                return 键;
            };
            std::sort(规范请求.参与者写集组.begin(),
                规范请求.参与者写集组.end(), [](const auto& 左, const auto& 右) {
                    return 左.参与者.值 < 右.参与者.值;
                });
            for (auto& 参与者 : 规范请求.参与者写集组) {
                const auto 按本地键 = [](const auto& 左, const auto& 右) {
                    return 左.本地键.值 < 右.本地键.值;
                };
                std::sort(参与者.写集.节点.begin(), 参与者.写集.节点.end(), 按本地键);
                std::sort(参与者.写集.关系.begin(), 参与者.写集.关系.end(), 按本地键);
                std::sort(参与者.写集.值.begin(), 参与者.写集.值.end(), 按本地键);
                std::sort(参与者.写集.退出事实.begin(),
                    参与者.写集.退出事实.end());
                std::sort(参与者.写集.属性槽变更.begin(),
                    参与者.写集.属性槽变更.end(), [&](const auto& 左, const auto& 右) {
                        const auto 左所属 = 引用排序键(左.所属节点);
                        const auto 右所属 = 引用排序键(右.所属节点);
                        if (左所属 != 右所属) return 左所属 < 右所属;
                        const auto 左类型 = 引用排序键(左.属性类型节点);
                        const auto 右类型 = 引用排序键(右.属性类型节点);
                        if (左类型 != 右类型) return 左类型 < 右类型;
                        return 左.新当前值.值 < 右.新当前值.值;
                    });
            }

            std::vector<std::unordered_set<std::uint32_t>> 节点键组(参与者数);
            std::vector<std::unordered_set<std::uint32_t>> 值键组(参与者数);
            std::unordered_set<std::uint64_t> 所有者组;
            std::unordered_set<std::uint64_t> 内部幂等组;
            for (std::size_t 序号 = 0; 序号 != 参与者数; ++序号) {
                const auto& 参与者 = 规范请求.参与者写集组[序号];
                const auto& 写集 = 参与者.写集;
                if (参与者.参与者.值 != 序号 + 1
                    || !有效(参与者.所有者)
                    || !所有者组.insert(参与者.所有者.编码.值).second
                    || 写集.合同版本 != L1所有者范围CRUD合同版本
                    || 写集.期望事实代次 != 请求.共同期望事实代次
                    || !有效(写集.写入幂等身份)
                    || !内部幂等组.insert(写集.写入幂等身份.值).second
                    || (写集.节点.empty() && 写集.关系.empty()
                        && 写集.值.empty() && 写集.属性槽变更.empty()
                        && 写集.退出事实.empty()))
                    return 失败(L1有限N分区原子事务状态_v3::入口拒绝);

                std::unordered_set<std::uint32_t> 全部本地键;
                for (const auto& 项 : 写集.节点) {
                    if (项.本地键.值 == 0
                        || !全部本地键.insert(项.本地键.值).second
                        || !节点键组[序号].insert(项.本地键.值).second)
                        return 失败(L1有限N分区原子事务状态_v3::入口拒绝);
                }
                for (const auto& 项 : 写集.关系)
                    if (项.本地键.值 == 0
                        || !全部本地键.insert(项.本地键.值).second)
                        return 失败(L1有限N分区原子事务状态_v3::入口拒绝);
                for (const auto& 项 : 写集.值) {
                    if (项.本地键.值 == 0
                        || !全部本地键.insert(项.本地键.值).second
                        || !值键组[序号].insert(项.本地键.值).second)
                        return 失败(L1有限N分区原子事务状态_v3::入口拒绝);
                }
                for (std::size_t i = 0; i < 写集.退出事实.size(); ++i)
                    if (!有效(写集.退出事实[i])
                        || (i != 0 && 写集.退出事实[i - 1] == 写集.退出事实[i]))
                        return 失败(L1有限N分区原子事务状态_v3::入口拒绝);
                for (std::size_t i = 1; i < 写集.属性槽变更.size(); ++i) {
                    const auto& 前 = 写集.属性槽变更[i - 1];
                    const auto& 后 = 写集.属性槽变更[i];
                    if (引用排序键(前.所属节点) == 引用排序键(后.所属节点)
                        && 引用排序键(前.属性类型节点)
                            == 引用排序键(后.属性类型节点))
                        return 失败(L1有限N分区原子事务状态_v3::入口拒绝);
                }
            }

            std::unique_lock<std::shared_mutex> 锁(锁_);
            const auto 引用有效 = [&](const L1有限N分区原子事实引用值_v3& 引用,
                std::size_t 当前序号) {
                if (const auto* 稳定项 = std::get_if<稳定编码>(&引用))
                    return 有效(*稳定项);
                if (const auto* 本地项 =
                    std::get_if<L1所有者范围写集本地键>(&引用))
                    return 本地项->值 != 0
                        && 节点键组[当前序号].contains(本地项->值);
                const auto& 跨项 = std::get<L1有限N分区原子事实引用_v3>(引用);
                return 跨项.参与者.值 >= 1
                    && 跨项.参与者.值 < 当前序号 + 1
                    && 跨项.本地键.值 != 0
                    && 节点键组[跨项.参与者.值 - 1].contains(跨项.本地键.值);
            };
            for (std::size_t 序号 = 0; 序号 != 参与者数; ++序号) {
                const auto& 写集 = 规范请求.参与者写集组[序号].写集;
                for (const auto& 项 : 写集.关系)
                    if (!引用有效(项.源节点, 序号)
                        || !引用有效(项.目标节点, 序号)
                        || !引用有效(项.关系类型节点, 序号))
                        return 失败(L1有限N分区原子事务状态_v3::引用冲突,
                            状态_.事实代次);
                for (const auto& 项 : 写集.值)
                    if (!引用有效(项.所属节点, 序号)
                        || !引用有效(项.属性类型节点, 序号)
                        || !引用有效(项.来源节点, 序号))
                        return 失败(L1有限N分区原子事务状态_v3::引用冲突,
                            状态_.事实代次);
                for (const auto& 项 : 写集.属性槽变更)
                    if (!引用有效(项.所属节点, 序号)
                        || !引用有效(项.属性类型节点, 序号)
                        || !值键组[序号].contains(项.新当前值.值))
                        return 失败(L1有限N分区原子事务状态_v3::引用冲突,
                            状态_.事实代次);
            }

            if (状态_.隔离 || !状态完整(状态_))
                return 失败(L1有限N分区原子事务状态_v3::内部不一致,
                    状态_.事实代次, L1所有者范围重试边界::不适用);
            if (const auto 既有 = 状态_.有限N分区原子事务幂等账_v3.find(
                    请求.组合写入幂等身份.值);
                既有 != 状态_.有限N分区原子事务幂等账_v3.end()) {
                if (!(既有->second.请求 == 规范请求))
                    return 失败(L1有限N分区原子事务状态_v3::幂等冲突,
                        状态_.事实代次, L1所有者范围重试边界::不适用);
                auto 结果 = 既有->second.结果;
                结果.状态 = L1有限N分区原子事务状态_v3::精确重复;
                结果.是否已确认形成内存权威发布 = false;
                结果.重试边界 =
                    L1所有者范围重试边界::原幂等身份读回收敛;
                return 结果;
            }
            if (规范请求.共同期望事实代次 != 状态_.事实代次)
                return 失败(L1有限N分区原子事务状态_v3::事实代次漂移,
                    状态_.事实代次);

            const auto 转换写集 = [&](const L1有限N分区原子写集请求_v3& 原,
                const std::vector<std::vector<std::pair<
                    L1所有者范围写集本地键, 稳定编码>>>& 映射组,
                std::size_t 当前序号) -> std::optional<L1所有者范围写集请求> {
                const auto 查映射 = [&](std::size_t 参与者序号,
                    L1所有者范围写集本地键 本地键) -> std::optional<稳定编码> {
                    const auto& 映射 = 映射组[参与者序号];
                    const auto 位置 = std::lower_bound(映射.begin(), 映射.end(),
                        本地键, [](const auto& 项, const auto 键值) {
                            return 项.first < 键值;
                        });
                    return 位置 != 映射.end() && 位置->first == 本地键
                        ? std::optional<稳定编码>{位置->second} : std::nullopt;
                };
                const auto 转换引用 = [&](const L1有限N分区原子事实引用值_v3& 引用)
                    -> std::optional<L1所有者范围事实引用> {
                    if (const auto* 稳定项 = std::get_if<稳定编码>(&引用))
                        return *稳定项;
                    if (const auto* 本地项 =
                        std::get_if<L1所有者范围写集本地键>(&引用))
                        return *本地项;
                    const auto& 跨项 = std::get<L1有限N分区原子事实引用_v3>(引用);
                    if (跨项.参与者.值 == 0 || 跨项.参与者.值 > 当前序号)
                        return std::nullopt;
                    const auto 编码 = 查映射(跨项.参与者.值 - 1, 跨项.本地键);
                    return 编码 ? std::optional<L1所有者范围事实引用>{*编码}
                        : std::nullopt;
                };
                L1所有者范围写集请求 结果;
                结果.合同版本 = 原.合同版本;
                结果.期望事实代次 = 原.期望事实代次;
                结果.写入幂等身份 = 原.写入幂等身份;
                结果.退出事实 = 原.退出事实;
                for (const auto& 项 : 原.节点)
                    结果.节点.push_back({项.本地键, 项.种类, 项.属性类型表示});
                for (const auto& 项 : 原.关系) {
                    const auto 源 = 转换引用(项.源节点);
                    const auto 目标 = 转换引用(项.目标节点);
                    const auto 类型 = 转换引用(项.关系类型节点);
                    if (!源 || !目标 || !类型) return std::nullopt;
                    结果.关系.push_back({项.本地键, *源, *目标, *类型,
                        项.角色或顺序});
                }
                for (const auto& 项 : 原.值) {
                    const auto 所属 = 转换引用(项.所属节点);
                    const auto 类型 = 转换引用(项.属性类型节点);
                    const auto 来源 = 转换引用(项.来源节点);
                    if (!所属 || !类型 || !来源) return std::nullopt;
                    结果.值.push_back({项.本地键, *所属, *类型, 项.材料, *来源});
                }
                for (const auto& 项 : 原.属性槽变更) {
                    const auto 所属 = 转换引用(项.所属节点);
                    const auto 类型 = 转换引用(项.属性类型节点);
                    if (!所属 || !类型) return std::nullopt;
                    结果.属性槽变更.push_back({*所属, *类型, 项.新当前值});
                }
                return 结果;
            };
            const auto 映射写入失败 = [](L1所有者范围写入状态 写入状态) {
                switch (写入状态) {
                case L1所有者范围写入状态::入口拒绝:
                    return L1有限N分区原子事务状态_v3::入口拒绝;
                case L1所有者范围写入状态::事实代次漂移:
                    return L1有限N分区原子事务状态_v3::事实代次漂移;
                case L1所有者范围写入状态::精确重复:
                case L1所有者范围写入状态::幂等冲突:
                    return L1有限N分区原子事务状态_v3::幂等冲突;
                case L1所有者范围写入状态::引用冲突:
                    return L1有限N分区原子事务状态_v3::引用冲突;
                case L1所有者范围写入状态::资源失败:
                    return L1有限N分区原子事务状态_v3::资源失败;
                default:
                    return L1有限N分区原子事务状态_v3::内部不一致;
                }
            };
            const auto 映射写入重试边界 = [](L1所有者范围写入状态 写入状态) {
                switch (写入状态) {
                case L1所有者范围写入状态::精确重复:
                case L1所有者范围写入状态::幂等冲突:
                case L1所有者范围写入状态::内部不一致:
                    return L1所有者范围重试边界::不适用;
                case L1所有者范围写入状态::资源失败:
                    return L1所有者范围重试边界::原请求可重试;
                default:
                    return L1所有者范围重试边界::修正请求后可重试;
                }
            };

            L1事实基座仓库 候选仓库;
            候选仓库.状态_ = 状态_;
            std::vector<std::vector<std::pair<
                L1所有者范围写集本地键, 稳定编码>>> 映射组(参与者数);
            std::vector<L1所有者范围写入结果> 写入组(参与者数);
            for (std::size_t 序号 = 0; 序号 != 参与者数; ++序号) {
                auto 普通请求 = 转换写集(
                    规范请求.参与者写集组[序号].写集, 映射组, 序号);
                if (!普通请求)
                    return 失败(L1有限N分区原子事务状态_v3::引用冲突,
                        状态_.事实代次);
                if (序号 != 0)
                    普通请求->期望事实代次 = 写入组[序号 - 1].事实代次;
                写入组[序号] = 候选仓库.提交所有者范围中性写集(
                    规范请求.参与者写集组[序号].所有者, *普通请求);
                if (写入组[序号].状态 != L1所有者范围写入状态::成功)
                    return 失败(映射写入失败(写入组[序号].状态),
                        状态_.事实代次,
                        映射写入重试边界(写入组[序号].状态));
                映射组[序号] = 写入组[序号].新编码映射;
                if (写入组[序号].事实代次 == 0
                    || (序号 == 0
                        && 写入组[序号].事实代次
                            != 规范请求.共同期望事实代次 + 1)
                    || (序号 != 0
                        && 写入组[序号].事实代次
                            != 写入组[序号 - 1].事实代次 + 1))
                    return 失败(L1有限N分区原子事务状态_v3::内部不一致,
                        状态_.事实代次, L1所有者范围重试边界::不适用);
            }

            候选共同代次 = 写入组[0].事实代次;
            std::unordered_set<std::uint64_t> 后序临时代次;
            后序临时代次.reserve(参与者数 - 1);
            for (std::size_t 序号 = 1; 序号 != 参与者数; ++序号)
                后序临时代次.insert(写入组[序号].事实代次);
            const auto 调整代次 = [&](auto& 事实) {
                if (后序临时代次.contains(事实.创建事实代次))
                    事实.创建事实代次 = 候选共同代次;
                if (事实.退出事实代次
                    && 后序临时代次.contains(*事实.退出事实代次))
                    *事实.退出事实代次 = 候选共同代次;
            };
            for (auto& [_, 事实] : 候选仓库.状态_.当前所有者) 调整代次(事实);
            for (auto& [_, 事实] : 候选仓库.状态_.历史所有者) 调整代次(事实);
            for (auto& [_, 事实] : 候选仓库.状态_.当前节点) 调整代次(事实);
            for (auto& [_, 事实] : 候选仓库.状态_.当前关系) 调整代次(事实);
            for (auto& [_, 事实] : 候选仓库.状态_.当前值) 调整代次(事实);
            for (auto& [_, 副本] : 候选仓库.状态_.历史)
                std::visit([&](auto& 事实) { 调整代次(事实); }, 副本.事实);

            for (const auto& 参与者 : 规范请求.参与者写集组) {
                const auto 账组 = 候选仓库.状态_.所有者范围幂等账.find(
                    参与者.所有者.编码.值);
                if (账组 == 候选仓库.状态_.所有者范围幂等账.end())
                    return 失败(L1有限N分区原子事务状态_v3::内部不一致,
                        状态_.事实代次, L1所有者范围重试边界::不适用);
                const auto 记录 = 账组->second.find(参与者.写集.写入幂等身份.值);
                if (记录 == 账组->second.end())
                    return 失败(L1有限N分区原子事务状态_v3::内部不一致,
                        状态_.事实代次, L1所有者范围重试边界::不适用);
                记录->second.首次规范化写集.期望事实代次 =
                    规范请求.共同期望事实代次;
                记录->second.首次发布事实代次 = 候选共同代次;
                记录->second.首次结果.事实代次 = 候选共同代次;
            }
            候选仓库.状态_.事实代次 = 候选共同代次;

            std::vector<L1有限N分区原子参与者结果_v3> 参与者结果组;
            参与者结果组.reserve(参与者数);
            for (std::size_t 序号 = 0; 序号 != 参与者数; ++序号)
                参与者结果组.push_back({
                    规范请求.参与者写集组[序号].参与者,
                    规范请求.参与者写集组[序号].所有者,
                    映射组[序号]});
            L1有限N分区原子事务结果_v3 结果{
                L1有限N分区原子事务状态_v3::已提交,
                L1有限N分区原子事务合同版本_v3,
                规范请求.组合写入幂等身份,
                候选共同代次, true, L1所有者范围重试边界::不适用,
                std::move(参与者结果组)};
            if (!候选仓库.状态_.有限N分区原子事务幂等账_v3.emplace(
                    规范请求.组合写入幂等身份.值,
                    有限N分区原子事务幂等记录_v3{规范请求, 结果}).second
                || !状态完整(候选仓库.状态_))
                return 失败(L1有限N分区原子事务状态_v3::内部不一致,
                    状态_.事实代次, L1所有者范围重试边界::不适用);
            const auto 持久准备 = 准备持久发布(候选仓库.状态_);
            if (持久准备 != 持久准备状态::成功)
                return 失败(持久准备 == 持久准备状态::资源失败
                        ? L1有限N分区原子事务状态_v3::资源失败
                        : L1有限N分区原子事务状态_v3::内部不一致,
                    状态_.事实代次,
                    持久准备 == 持久准备状态::资源失败
                        ? L1所有者范围重试边界::原请求可重试
                        : L1所有者范围重试边界::不适用);
            已进入交换边界 = true;
            状态_ = std::move(候选仓库.状态_);
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(已进入交换边界
                    ? L1有限N分区原子事务状态_v3::已可能发布
                    : L1有限N分区原子事务状态_v3::资源失败,
                已进入交换边界 ? 候选共同代次 : 0,
                已进入交换边界
                    ? L1所有者范围重试边界::原幂等身份读回收敛
                    : L1所有者范围重试边界::原请求可重试);
        } catch (...) {
            return 失败(已进入交换边界
                    ? L1有限N分区原子事务状态_v3::已可能发布
                    : L1有限N分区原子事务状态_v3::内部不一致,
                已进入交换边界 ? 候选共同代次 : 0,
                已进入交换边界
                    ? L1所有者范围重试边界::原幂等身份读回收敛
                    : L1所有者范围重试边界::不适用);
        }
    }

    // 诊断责任：向上送出；只读本 owner 所选幂等记录并返回结构化状态。
    L1所有者范围首次写入读取结果 读取所有者范围首次写入材料(
        L1结构所有者身份 所有者,
        const L1所有者范围首次写入读取请求& 请求) const {
        std::uint64_t 读取代次 = 0;
        const auto 失败 = [&](L1所有者范围读取状态 状态) {
            return L1所有者范围首次写入读取结果{状态,
                L1所有者范围首次写入读取合同版本, 所有者,
                请求.写入幂等身份, 读取代次, std::nullopt, std::nullopt};
        };
        if (请求.合同版本 != L1所有者范围首次写入读取合同版本
            || !有效(所有者) || !有效(请求.写入幂等身份))
            return 失败(L1所有者范围读取状态::入口拒绝);
        try {
            std::shared_lock<std::shared_mutex> 锁(锁_);
            读取代次 = 状态_.事实代次;
            if (状态_.隔离)
                return 失败(L1所有者范围读取状态::内部不一致);

            const auto 当前所有者 = 状态_.当前所有者.find(所有者.编码.值);
            if (当前所有者 == 状态_.当前所有者.end()
                || 当前所有者->second.所有者 != 所有者
                || 当前所有者->second.范围种类
                    != L1所有者范围种类::独占结构范围
                || 当前所有者->second.退出事实代次)
                return 失败(L1所有者范围读取状态::内部不一致);

            const auto 账组 = 状态_.所有者范围幂等账.find(所有者.编码.值);
            if (账组 == 状态_.所有者范围幂等账.end())
                return 失败(L1所有者范围读取状态::未找到);
            const auto 记录 = 账组->second.find(请求.写入幂等身份.值);
            if (记录 == 账组->second.end())
                return 失败(L1所有者范围读取状态::未找到);
            if (!所有者范围首次记录完整(状态_, 所有者,
                    请求.写入幂等身份, 记录->second))
                return 失败(L1所有者范围读取状态::内部不一致);

            auto 首次写集 = 转换所有者范围写集(记录->second.首次规范化写集);
            auto 首次结果 = 转换所有者范围写入结果(
                所有者, 记录->second.首次结果);
            if (首次写集.合同版本 != L1所有者范围CRUD合同版本
                || 首次写集.写入幂等身份 != 请求.写入幂等身份
                || 首次结果.状态 != L1所有者范围写入状态::成功
                || 首次结果.所有者 != 所有者
                || 首次结果.写入幂等身份 != 请求.写入幂等身份
                || 首次结果.事实代次 == 0
                || 首次结果.事实代次 > 读取代次
                || !首次结果.是否形成内存权威发布
                || 首次结果.重试边界 != L1所有者范围重试边界::不适用)
                return 失败(L1所有者范围读取状态::内部不一致);

            L1所有者范围首次写入读取结果 结果;
            结果.状态 = L1所有者范围读取状态::成功;
            结果.所有者 = 所有者;
            结果.写入幂等身份 = 请求.写入幂等身份;
            结果.读取事实代次 = 读取代次;
            结果.首次规范化写集 = std::move(首次写集);
            结果.首次写入结果 = std::move(首次结果);
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(L1所有者范围读取状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L1所有者范围读取状态::资源失败);
        } catch (...) {
            return 失败(L1所有者范围读取状态::内部不一致);
        }
    }

    // 诊断责任：向上送出；v1/v2 共用同一候选和最后发布点。
    L1中性写入结果 提交中性写集实现(const L1中性写集请求& 请求,
        std::optional<L1结构所有者身份> 指定所有者) {
        try {
            const auto 规范化 = 规范化中性写集(请求);
            if (!规范化 || 请求.合同版本 != L1中性CRUD合同版本
                || !有效(请求.幂等键)
                || (请求.节点.empty() && 请求.关系.empty() && 请求.值.empty()
                    && 请求.属性槽变更.empty() && 请求.退出事实.empty()))
                return 中性写入结果(请求, L1中性写入状态::入口拒绝, 0, false,
                    L1中性重试边界::修正请求后可重试);

            std::unordered_set<std::uint32_t> 本地键;
            if (!中性请求结构有效(*规范化, 本地键))
                return 中性写入结果(请求, L1中性写入状态::入口拒绝, 0, false,
                    L1中性重试边界::修正请求后可重试);

            std::unique_lock<std::shared_mutex> 锁(锁_);
            if (状态_.隔离 || !状态完整(状态_))
                return 中性写入结果(请求, L1中性写入状态::内部不一致,
                    状态_.事实代次, false, L1中性重试边界::原幂等键读回收敛);

            L1结构所有者身份 写入所有者;
            if (指定所有者) {
                const auto 所有者 = 状态_.当前所有者.find(指定所有者->编码.值);
                if (所有者 == 状态_.当前所有者.end()
                    || 所有者->second.所有者 != *指定所有者
                    || 所有者->second.范围种类 != L1所有者范围种类::独占结构范围
                    || 所有者->second.退出事实代次) {
                    return 中性写入结果(请求, L1中性写入状态::内部不一致,
                        状态_.事实代次, false,
                        L1中性重试边界::原幂等键读回收敛);
                }
                写入所有者 = *指定所有者;
                const auto 所有者账 = 状态_.所有者范围幂等账.find(
                    写入所有者.编码.值);
                if (所有者账 != 状态_.所有者范围幂等账.end()) {
                    const auto 既有 = 所有者账->second.find(请求.幂等键.值);
                    if (既有 != 所有者账->second.end()) {
                        if (既有->second.首次规范化写集 == *规范化) {
                            auto 结果 = 既有->second.首次结果;
                            结果.状态 = L1中性写入状态::精确重复;
                            结果.是否形成内存权威发布 = false;
                            结果.重试边界 = L1中性重试边界::原幂等键读回收敛;
                            return 结果;
                        }
                        return 中性写入结果(请求, L1中性写入状态::幂等冲突,
                            状态_.事实代次, false,
                            L1中性重试边界::修正请求后可重试);
                    }
                }
            } else {
                if (状态_.旧共享所有者定位) {
                    const auto 所有者 = 状态_.当前所有者.find(
                        状态_.旧共享所有者定位->编码.值);
                    if (所有者 == 状态_.当前所有者.end()
                        || 所有者->second.所有者 != *状态_.旧共享所有者定位
                        || 所有者->second.范围种类 != L1所有者范围种类::旧共享范围
                        || 所有者->second.退出事实代次)
                        return 中性写入结果(请求,
                            L1中性写入状态::内部不一致, 状态_.事实代次,
                            false, L1中性重试边界::原幂等键读回收敛);
                    写入所有者 = *状态_.旧共享所有者定位;
                }
                const auto 中性既有 = 状态_.中性幂等账.find(请求.幂等键.值);
                if (中性既有 != 状态_.中性幂等账.end()) {
                    if (中性既有->second.首次规范化写集 == *规范化) {
                        auto 结果 = 中性既有->second.首次结果;
                        结果.状态 = L1中性写入状态::精确重复;
                        结果.是否形成内存权威发布 = false;
                        结果.重试边界 = L1中性重试边界::原幂等键读回收敛;
                        return 结果;
                    }
                    return 中性写入结果(请求, L1中性写入状态::幂等冲突,
                        状态_.事实代次, false,
                        L1中性重试边界::修正请求后可重试);
                }
            }
            if (请求.期望事实代次 != 状态_.事实代次)
                return 中性写入结果(请求, L1中性写入状态::事实代次漂移,
                    状态_.事实代次, false, L1中性重试边界::原请求可重试);

            const auto 稳定节点属于 = [&](const L1中性事实引用& 引用,
                bool 必须同所有者) {
                const auto* 编码 = std::get_if<稳定编码>(&引用);
                if (!编码) return true;
                const auto 节点 = 状态_.当前节点.find(编码->值);
                if (节点 == 状态_.当前节点.end()) return true;
                if (!指定所有者)
                    return 有效(写入所有者)
                        && 节点->second.写入所有者 == 写入所有者;
                return !必须同所有者
                    || 节点->second.写入所有者 == 写入所有者;
            };
            const auto 事实属于写者 = [&](稳定编码 编码) {
                if (const auto 节点 = 状态_.当前节点.find(编码.值);
                    节点 != 状态_.当前节点.end())
                    return 节点->second.写入所有者 == 写入所有者;
                if (const auto 关系 = 状态_.当前关系.find(编码.值);
                    关系 != 状态_.当前关系.end())
                    return 关系->second.写入所有者 == 写入所有者;
                if (const auto 值 = 状态_.当前值.find(编码.值);
                    值 != 状态_.当前值.end())
                    return 值->second.写入所有者 == 写入所有者;
                return true;
            };
            bool 所有权不符 = false;
            for (const auto& 项 : 规范化->关系)
                所有权不符 = 所有权不符
                    || !稳定节点属于(项.关系类型节点, true)
                    || !稳定节点属于(项.源节点, false)
                    || !稳定节点属于(项.目标节点, false);
            for (const auto& 项 : 规范化->值)
                所有权不符 = 所有权不符
                    || !稳定节点属于(项.所属节点, true)
                    || !稳定节点属于(项.属性类型节点, true)
                    || !稳定节点属于(项.来源节点, false);
            for (const auto& 项 : 规范化->属性槽变更)
                所有权不符 = 所有权不符
                    || !稳定节点属于(项.所属节点, true)
                    || !稳定节点属于(项.属性类型节点, true);
            for (const auto 编码 : 规范化->退出事实)
                所有权不符 = 所有权不符 || !事实属于写者(编码);
            if (所有权不符) {
                return 中性写入结果(请求, L1中性写入状态::引用冲突,
                    状态_.事实代次, false,
                    L1中性重试边界::修正请求后可重试);
            }

            std::unordered_set<std::uint64_t> 退出集合;
            退出集合.reserve(规范化->退出事实.size());
            for (const auto 编码 : 规范化->退出事实) {
                if (!状态_.当前节点.contains(编码.值)
                    && !状态_.当前关系.contains(编码.值)
                    && !状态_.当前值.contains(编码.值))
                    return 中性写入结果(请求,
                        状态_.历史.contains(编码.值)
                            ? L1中性写入状态::已退出 : L1中性写入状态::未找到,
                        状态_.事实代次, false, L1中性重试边界::修正请求后可重试);
                if (!退出集合.insert(编码.值).second)
                    return 中性写入结果(请求, L1中性写入状态::入口拒绝,
                        状态_.事实代次, false, L1中性重试边界::修正请求后可重试);
            }
            const auto 稳定引用指向退出节点 = [&](const L1中性事实引用& 引用) noexcept {
                const auto* 编码 = std::get_if<稳定编码>(&引用);
                return 编码 && 退出集合.contains(编码->值);
            };
            for (const auto& 项 : 规范化->关系) {
                if (稳定引用指向退出节点(项.源节点)
                    || 稳定引用指向退出节点(项.目标节点)
                    || 稳定引用指向退出节点(项.关系类型节点))
                    return 中性写入结果(请求, L1中性写入状态::入口拒绝,
                        状态_.事实代次, false, L1中性重试边界::修正请求后可重试);
            }
            for (const auto& 项 : 规范化->值) {
                const auto* 材料引用 = std::get_if<L1中性独立材料引用>(&项.材料);
                if (稳定引用指向退出节点(项.所属节点)
                    || 稳定引用指向退出节点(项.属性类型节点)
                    || 稳定引用指向退出节点(项.来源节点)
                    || (材料引用 && 退出集合.contains(材料引用->编码.值)))
                    return 中性写入结果(请求, L1中性写入状态::入口拒绝,
                        状态_.事实代次, false, L1中性重试边界::修正请求后可重试);
            }
            for (const auto& 项 : 规范化->属性槽变更) {
                if (稳定引用指向退出节点(项.所属节点)
                    || 稳定引用指向退出节点(项.属性类型节点))
                    return 中性写入结果(请求, L1中性写入状态::入口拒绝,
                        状态_.事实代次, false, L1中性重试边界::修正请求后可重试);
            }
            for (const auto 编码 : 规范化->退出事实) {
                if (!状态_.当前节点.contains(编码.值)) continue;
                const auto 闭包 = 检查当前节点引用闭包(状态_, 编码, 退出集合);
                if (闭包 == 当前节点引用闭包检查结果::引用冲突)
                    return 中性写入结果(请求, L1中性写入状态::引用冲突,
                        状态_.事实代次, false, L1中性重试边界::修正请求后可重试);
                if (闭包 == 当前节点引用闭包检查结果::内部不一致)
                    return 中性写入结果(请求, L1中性写入状态::内部不一致,
                        状态_.事实代次, false, L1中性重试边界::原幂等键读回收敛);
            }

            状态 候选 = 状态_;
            std::vector<std::pair<L1中性写集本地键, 稳定编码>> 映射;
            映射.reserve(规范化->节点.size() + 规范化->关系.size() + 规范化->值.size());
            auto 分配 = [&]() -> std::optional<稳定编码> {
                while (候选.下个编码 != 0 && 候选.永久占用.contains(候选.下个编码))
                    ++候选.下个编码;
                if (候选.下个编码 == 0) return std::nullopt;
                const 稳定编码 编码{候选.下个编码++};
                return 编码;
            };
            const auto 新代次 = 状态_.事实代次 + 1;
            if (!指定所有者 && !有效(写入所有者)) {
                const auto 所有者编码 = 分配();
                if (!所有者编码)
                    return 中性写入结果(请求, L1中性写入状态::资源失败,
                        状态_.事实代次, false, L1中性重试边界::原请求可重试);
                写入所有者 = L1结构所有者身份{*所有者编码};
                候选.当前所有者.emplace(所有者编码->值,
                    L1结构所有者事实{写入所有者,
                        L1所有者范围种类::旧共享范围, 新代次, std::nullopt});
                候选.永久占用.insert(所有者编码->值);
                候选.旧共享所有者定位 = 写入所有者;
            }
            for (const auto& 项 : 规范化->节点) {
                const auto 编码 = 分配();
                if (!编码) return 中性写入结果(请求, L1中性写入状态::资源失败,
                    状态_.事实代次, false, L1中性重试边界::原请求可重试);
                映射.emplace_back(项.本地键, *编码);
            }
            for (const auto& 项 : 规范化->关系) {
                const auto 编码 = 分配();
                if (!编码) return 中性写入结果(请求, L1中性写入状态::资源失败,
                    状态_.事实代次, false, L1中性重试边界::原请求可重试);
                映射.emplace_back(项.本地键, *编码);
            }
            for (const auto& 项 : 规范化->值) {
                const auto 编码 = 分配();
                if (!编码) return 中性写入结果(请求, L1中性写入状态::资源失败,
                    状态_.事实代次, false, L1中性重试边界::原请求可重试);
                映射.emplace_back(项.本地键, *编码);
            }

            auto 解析节点 = [&](const L1中性事实引用& 引用,
                L1中性写入状态& 失败) -> std::optional<稳定编码> {
                return std::visit([&](const auto& 值) -> std::optional<稳定编码> {
                    using 类型 = std::decay_t<decltype(值)>;
                    if constexpr (std::is_same_v<类型, 稳定编码>) {
                        if (!有效(值)) { 失败 = L1中性写入状态::入口拒绝; return std::nullopt; }
                        if (状态_.当前节点.contains(值.值)) return 值;
                        if (状态_.历史.contains(值.值)) 失败 = L1中性写入状态::已退出;
                        else if (状态_.当前关系.contains(值.值) || 状态_.当前值.contains(值.值))
                            失败 = L1中性写入状态::入口拒绝;
                        else 失败 = L1中性写入状态::未找到;
                        return std::nullopt;
                    } else {
                        for (std::size_t i = 0; i < 规范化->节点.size(); ++i)
                            if (规范化->节点[i].本地键 == 值) return 映射[i].second;
                        失败 = L1中性写入状态::入口拒绝;
                        return std::nullopt;
                    }
                }, 引用);
            };
            auto 查映射 = [&](L1中性写集本地键 键) -> std::optional<稳定编码> {
                for (const auto& [本地, 编码] : 映射) if (本地 == 键) return 编码;
                return std::nullopt;
            };
            for (std::size_t i = 0; i < 规范化->节点.size(); ++i) {
                const auto& 项 = 规范化->节点[i];
                if ((项.种类 == 节点种类::属性类型) != 项.属性类型表示.has_value())
                    return 中性写入结果(请求, L1中性写入状态::入口拒绝,
                        状态_.事实代次, false, L1中性重试边界::修正请求后可重试);
                候选.当前节点.emplace(映射[i].second.值, 节点事实{映射[i].second,
                    项.种类, 转换值表示种类(项.属性类型表示), 新代次,
                    std::nullopt, {}, 写入所有者});
                候选.永久占用.insert(映射[i].second.值);
            }

            const std::size_t 关系偏移 = 规范化->节点.size();
            for (std::size_t i = 0; i < 规范化->关系.size(); ++i) {
                const auto& 项 = 规范化->关系[i];
                L1中性写入状态 失败 = L1中性写入状态::入口拒绝;
                const auto 源 = 解析节点(项.源节点, 失败);
                if (!源) return 中性写入结果(请求, 失败, 状态_.事实代次, false,
                    L1中性重试边界::修正请求后可重试);
                const auto 目标 = 解析节点(项.目标节点, 失败);
                if (!目标) return 中性写入结果(请求, 失败, 状态_.事实代次, false,
                    L1中性重试边界::修正请求后可重试);
                const auto 类型 = 解析节点(项.关系类型节点, 失败);
                if (!类型) return 中性写入结果(请求, 失败, 状态_.事实代次, false,
                    L1中性重试边界::修正请求后可重试);
                const auto 编码 = 映射[关系偏移 + i].second;
                const 关系事实 事实{编码, *源, *目标, *类型,
                    项.角色或顺序, 新代次, std::nullopt, 写入所有者};
                if (!候选.当前关系.emplace(编码.值, 事实).second
                    || !插入当前源关系索引(候选, 事实)
                    || !插入当前目标关系索引(候选, 事实)
                    || !插入当前关系类型索引(候选, 事实)
                    || !插入历史源关系候选索引(候选, 事实)
                    || !插入历史目标关系候选索引(候选, 事实)
                    || !插入当前节点引用索引(候选, 事实))
                    return 中性写入结果(请求, L1中性写入状态::内部不一致,
                        状态_.事实代次, false, L1中性重试边界::原幂等键读回收敛);
                候选.永久占用.insert(编码.值);
            }

            const std::size_t 值偏移 = 关系偏移 + 规范化->关系.size();
            for (std::size_t i = 0; i < 规范化->值.size(); ++i) {
                const auto& 项 = 规范化->值[i];
                L1中性写入状态 失败 = L1中性写入状态::入口拒绝;
                const auto 所属 = 解析节点(项.所属节点, 失败);
                if (!所属) return 中性写入结果(请求, 失败, 状态_.事实代次, false,
                    L1中性重试边界::修正请求后可重试);
                const auto 类型编码 = 解析节点(项.属性类型节点, 失败);
                if (!类型编码) return 中性写入结果(请求, 失败, 状态_.事实代次, false,
                    L1中性重试边界::修正请求后可重试);
                const auto 来源 = 解析节点(项.来源节点, 失败);
                if (!来源) return 中性写入结果(请求, 失败, 状态_.事实代次, false,
                    L1中性重试边界::修正请求后可重试);
                const auto 类型节点 = 候选.当前节点.find(类型编码->值);
                const auto 材料 = 转换原始材料(项.材料);
                if (!材料 || 类型节点 == 候选.当前节点.end()
                    || 类型节点->second.种类 != 节点种类::属性类型
                    || !类型节点->second.属性类型表示
                    || !表示匹配(*类型节点->second.属性类型表示, *材料))
                    return 中性写入结果(请求, L1中性写入状态::入口拒绝,
                        状态_.事实代次, false, L1中性重试边界::修正请求后可重试);
                if (const auto* 引用 = std::get_if<独立材料引用>(&*材料);
                    引用 && !状态_.当前节点.contains(引用->编码.值))
                    return 中性写入结果(请求,
                        状态_.历史.contains(引用->编码.值)
                            ? L1中性写入状态::已退出 : L1中性写入状态::未找到,
                        状态_.事实代次, false, L1中性重试边界::修正请求后可重试);
                const auto 编码 = 映射[值偏移 + i].second;
                const 值事实 事实{编码, *所属, *类型编码,
                    *材料, *来源, 新代次, std::nullopt, 写入所有者};
                if (!候选.当前值.emplace(编码.值, 事实).second
                    || !插入当前节点引用索引(候选, 事实))
                    return 中性写入结果(请求, L1中性写入状态::内部不一致,
                        状态_.事实代次, false, L1中性重试边界::原幂等键读回收敛);
                候选.永久占用.insert(编码.值);
            }

            auto 退出一个 = [&](稳定编码 编码) {
                if (auto it = 候选.当前节点.find(编码.值); it != 候选.当前节点.end()) {
                    auto 事实 = it->second; 事实.退出事实代次 = 新代次;
                    候选.历史[编码.值] = {编码, 事实, false};
                    候选.当前节点.erase(it); return true;
                }
                if (auto it = 候选.当前关系.find(编码.值); it != 候选.当前关系.end()) {
                    const auto 当前事实 = it->second;
                    if (!删除当前源关系索引(候选, 当前事实)
                        || !删除当前目标关系索引(候选, 当前事实)
                        || !删除当前关系类型索引(候选, 当前事实)
                        || !删除当前节点引用索引(候选, 当前事实)) return false;
                    auto 事实 = 当前事实; 事实.退出事实代次 = 新代次;
                    候选.历史[编码.值] = {编码, 事实, false};
                    候选.当前关系.erase(it); return true;
                }
                if (auto 当前值 = 候选.当前值.find(编码.值);
                    当前值 != 候选.当前值.end()) {
                    if (!删除当前节点引用索引(候选, 当前值->second)) return false;
                    auto 事实 = 当前值->second; 事实.退出事实代次 = 新代次;
                    候选.历史[编码.值] = {编码, 事实, false};
                    候选.当前值.erase(当前值);
                    for (auto& [_, 节点] : 候选.当前节点)
                        节点.当前属性.erase(std::remove_if(节点.当前属性.begin(),
                            节点.当前属性.end(), [&](const 属性槽& 槽) {
                                return 槽.当前值 == 编码;
                            }), 节点.当前属性.end());
                    return true;
                }
                return false;
            };
            for (const auto 编码 : 规范化->退出事实)
                if (!退出一个(编码))
                    return 中性写入结果(请求, L1中性写入状态::内部不一致,
                        状态_.事实代次, false, L1中性重试边界::原幂等键读回收敛);

            for (const auto& 项 : 规范化->属性槽变更) {
                L1中性写入状态 失败 = L1中性写入状态::入口拒绝;
                const auto 所属 = 解析节点(项.所属节点, 失败);
                if (!所属) return 中性写入结果(请求, 失败, 状态_.事实代次, false,
                    L1中性重试边界::修正请求后可重试);
                const auto 类型 = 解析节点(项.属性类型节点, 失败);
                if (!类型) return 中性写入结果(请求, 失败, 状态_.事实代次, false,
                    L1中性重试边界::修正请求后可重试);
                const auto 新值编码 = 查映射(项.新当前值);
                const auto 新值 = 新值编码 ? 候选.当前值.find(新值编码->值)
                    : 候选.当前值.end();
                auto 节点 = 候选.当前节点.find(所属->值);
                if (!新值编码 || 新值 == 候选.当前值.end()
                    || 节点 == 候选.当前节点.end()
                    || 新值->second.所属节点 != *所属
                    || 新值->second.属性类型节点 != *类型)
                    return 中性写入结果(请求, L1中性写入状态::入口拒绝,
                        状态_.事实代次, false, L1中性重试边界::修正请求后可重试);
                if (!插入历史属性槽值候选索引(候选, 新值->second))
                    return 中性写入结果(请求, L1中性写入状态::内部不一致,
                        状态_.事实代次, false, L1中性重试边界::原幂等键读回收敛);
                auto& 槽组 = 节点->second.当前属性;
                auto 槽 = std::find_if(槽组.begin(), 槽组.end(), [&](const 属性槽& 值) {
                    return 值.属性类型节点 == *类型;
                });
                if (槽 != 槽组.end()) {
                    const auto 旧值编码 = 槽->当前值;
                    if (auto 旧值 = 候选.当前值.find(旧值编码.值);
                        旧值 != 候选.当前值.end()) {
                        if (!删除当前节点引用索引(候选, 旧值->second))
                            return 中性写入结果(请求,
                                L1中性写入状态::内部不一致, 状态_.事实代次,
                                false, L1中性重试边界::原幂等键读回收敛);
                        auto 历史值 = 旧值->second;
                        历史值.退出事实代次 = 新代次;
                        候选.历史[旧值编码.值] = {旧值编码, 历史值, false};
                        候选.当前值.erase(旧值);
                    }
                    槽->当前值 = *新值编码;
                } else 槽组.push_back({*类型, *新值编码});
                std::sort(槽组.begin(), 槽组.end(), [](const 属性槽& 左, const 属性槽& 右) {
                    return 左.属性类型节点 < 右.属性类型节点;
                });
            }

            候选.事实代次 = 新代次;
            std::sort(映射.begin(), 映射.end(), [](const auto& 左, const auto& 右) {
                return 左.first < 右.first;
            });
            if (!中性候选读回完整(
                    候选, *规范化, 映射, 新代次, 写入所有者))
                return 中性写入结果(请求, L1中性写入状态::入口拒绝,
                    状态_.事实代次, false, L1中性重试边界::修正请求后可重试);

            auto 结果 = 中性写入结果(请求, L1中性写入状态::成功, 新代次, true,
                L1中性重试边界::不适用, 映射);
            if (指定所有者)
                候选.所有者范围幂等账[写入所有者.编码.值].emplace(
                    请求.幂等键.值,
                    所有者范围幂等记录{*规范化, 新代次, 映射, 结果});
            else
                候选.中性幂等账.emplace(请求.幂等键.值,
                    中性幂等记录{*规范化, 新代次, 映射, 结果});
            if (!状态完整(候选))
                return 中性写入结果(请求, L1中性写入状态::内部不一致,
                    状态_.事实代次, false, L1中性重试边界::原幂等键读回收敛);

            const auto 持久准备 = 准备持久发布(候选);
            if (持久准备 != 持久准备状态::成功)
                return 中性写入结果(请求,
                    持久准备 == 持久准备状态::资源失败
                        ? L1中性写入状态::资源失败
                        : L1中性写入状态::内部不一致,
                    状态_.事实代次, false,
                    持久准备 == 持久准备状态::资源失败
                        ? L1中性重试边界::原请求可重试
                        : L1中性重试边界::原幂等键读回收敛);
            std::swap(状态_, 候选);
            if (!中性候选读回完整(
                    状态_, *规范化, 映射, 新代次, 写入所有者)) {
                状态_.隔离 = true;
                return 中性写入结果(请求, L1中性写入状态::内部不一致,
                    新代次, true, L1中性重试边界::原幂等键读回收敛);
            }
            return 结果;
        } catch (const std::bad_alloc&) {
            return 中性写入结果(请求, L1中性写入状态::资源失败, 0, false,
                L1中性重试边界::原请求可重试);
        } catch (...) {
            return 中性写入结果(请求, L1中性写入状态::内部不一致, 0, false,
                L1中性重试边界::原幂等键读回收敛);
        }
    }

    L1读取结果 读取当前节点(稳定编码 编码) const { return 读取当前(编码, 状态_.当前节点); }
    L1读取结果 读取当前关系(稳定编码 编码) const { return 读取当前(编码, 状态_.当前关系); }
    L1读取结果 读取当前值(稳定编码 编码) const { return 读取当前(编码, 状态_.当前值); }
    L1中性源关系读取结果 读取中性当前源关系组(
        const L1中性源关系读取请求& 请求) const {
        L1中性源关系读取结果 结果{L1中性读取状态::入口拒绝,
            L1中性CRUD合同版本, 请求.源节点, 请求.关系类型节点, 0, {}};
        if (请求.合同版本 != L1中性CRUD合同版本
            || !有效(请求.源节点) || !有效(请求.关系类型节点)) return 结果;
        std::shared_lock<std::shared_mutex> 锁(锁_);
        if (状态_.隔离) {
            结果.状态 = L1中性读取状态::内部不一致;
            return 结果;
        }
        const auto 源 = 状态_.当前源关系索引.find(请求.源节点.值);
        if (源 == 状态_.当前源关系索引.end()) {
            结果.状态 = L1中性读取状态::成功;
            结果.读取事实代次 = 状态_.事实代次;
            return 结果;
        }
        const auto 类型 = 源->second.find(请求.关系类型节点.值);
        if (类型 == 源->second.end()) {
            结果.状态 = L1中性读取状态::成功;
            结果.读取事实代次 = 状态_.事实代次;
            return 结果;
        }
        if (状态_.事实代次 == 0 || 类型->second.empty()) {
            结果.状态 = L1中性读取状态::内部不一致;
            return 结果;
        }
        结果.关系组.reserve(类型->second.size());
        std::uint64_t 前一编码 = 0;
        for (const auto 编码 : 类型->second) {
            const auto 关系 = 状态_.当前关系.find(编码);
            if (编码 == 0 || 编码 <= 前一编码 || 关系 == 状态_.当前关系.end()
                || 关系->second.编码.值 != 编码
                || 关系->second.源节点 != 请求.源节点
                || 关系->second.关系类型节点 != 请求.关系类型节点
                || 关系->second.退出事实代次.has_value()) {
                结果.状态 = L1中性读取状态::内部不一致;
                结果.关系组.clear();
                return 结果;
            }
            const auto& 事实 = 关系->second;
            结果.关系组.push_back({事实.编码, 事实.源节点, 事实.目标节点,
                事实.关系类型节点, 事实.角色或顺序, 事实.创建事实代次,
                事实.退出事实代次});
            前一编码 = 编码;
        }
        结果.状态 = L1中性读取状态::成功;
        结果.读取事实代次 = 状态_.事实代次;
        return 结果;
    }
    L1中性目标关系读取结果 读取中性当前目标关系组(
        const L1中性目标关系读取请求& 请求) const {
        L1中性目标关系读取结果 结果{L1中性读取状态::入口拒绝,
            L1中性CRUD合同版本, 请求.目标节点, 请求.关系类型节点, 0, {}};
        if (请求.合同版本 != L1中性CRUD合同版本
            || !有效(请求.目标节点) || !有效(请求.关系类型节点)) return 结果;
        std::shared_lock<std::shared_mutex> 锁(锁_);
        if (状态_.隔离) {
            结果.状态 = L1中性读取状态::内部不一致;
            return 结果;
        }
        const auto 目标 = 状态_.当前目标关系索引.find(请求.目标节点.值);
        if (目标 == 状态_.当前目标关系索引.end()) {
            结果.状态 = L1中性读取状态::成功;
            结果.读取事实代次 = 状态_.事实代次;
            return 结果;
        }
        const auto 类型 = 目标->second.find(请求.关系类型节点.值);
        if (类型 == 目标->second.end()) {
            结果.状态 = L1中性读取状态::成功;
            结果.读取事实代次 = 状态_.事实代次;
            return 结果;
        }
        if (状态_.事实代次 == 0 || 类型->second.empty()) {
            结果.状态 = L1中性读取状态::内部不一致;
            return 结果;
        }
        结果.关系组.reserve(类型->second.size());
        std::uint64_t 前一编码 = 0;
        for (const auto 编码 : 类型->second) {
            const auto 关系 = 状态_.当前关系.find(编码);
            if (编码 == 0 || 编码 <= 前一编码 || 关系 == 状态_.当前关系.end()
                || 关系->second.编码.值 != 编码
                || 关系->second.目标节点 != 请求.目标节点
                || 关系->second.关系类型节点 != 请求.关系类型节点
                || 关系->second.退出事实代次.has_value()) {
                结果.状态 = L1中性读取状态::内部不一致;
                结果.关系组.clear();
                return 结果;
            }
            const auto& 事实 = 关系->second;
            结果.关系组.push_back({事实.编码, 事实.源节点, 事实.目标节点,
                事实.关系类型节点, 事实.角色或顺序, 事实.创建事实代次,
                事实.退出事实代次});
            前一编码 = 编码;
        }
        结果.状态 = L1中性读取状态::成功;
        结果.读取事实代次 = 状态_.事实代次;
        return 结果;
    }

    // 诊断责任：向上送出；索引或权威事实矛盾由中性结构化状态携带。
    L1中性历史关系组读取结果 读取中性历史关系组(
        const L1中性历史关系组读取请求& 请求) const {
        const auto 失败 = [&](L1中性历史关系组读取状态 状态,
            std::uint64_t 读取代次 = 0) {
            return L1中性历史关系组读取结果{状态, L1中性CRUD合同版本,
                请求.方向, 请求.端点节点, 请求.关系类型节点,
                请求.历史截止事实代次, 读取代次, {}};
        };
        const bool 方向有效 = 请求.方向 == L1中性关系端点方向::源
            || 请求.方向 == L1中性关系端点方向::目标;
        if (请求.合同版本 != L1中性CRUD合同版本 || !方向有效
            || !有效(请求.端点节点) || !有效(请求.关系类型节点)
            || 请求.历史截止事实代次 == 0)
            return 失败(L1中性历史关系组读取状态::入口拒绝);

        std::shared_lock<std::shared_mutex> 锁(锁_);
        if (状态_.隔离)
            return 失败(L1中性历史关系组读取状态::内部不一致);
        if (状态_.事实代次 < 请求.历史截止事实代次)
            return 失败(L1中性历史关系组读取状态::事实代次漂移,
                状态_.事实代次);

        const auto& 端点索引 = 请求.方向 == L1中性关系端点方向::源
            ? 状态_.历史源关系候选索引 : 状态_.历史目标关系候选索引;
        const auto 端点 = 端点索引.find(请求.端点节点.值);
        if (端点 == 端点索引.end())
            return 失败(L1中性历史关系组读取状态::成功, 状态_.事实代次);
        const auto 类型 = 端点->second.find(请求.关系类型节点.值);
        if (类型 == 端点->second.end())
            return 失败(L1中性历史关系组读取状态::成功, 状态_.事实代次);
        if (类型->second.empty())
            return 失败(L1中性历史关系组读取状态::内部不一致);

        L1中性历史关系组读取结果 结果{
            L1中性历史关系组读取状态::成功, L1中性CRUD合同版本,
            请求.方向, 请求.端点节点, 请求.关系类型节点,
            请求.历史截止事实代次, 状态_.事实代次, {}};
        结果.关系组.reserve(类型->second.size());
        std::uint64_t 前一编码 = 0;
        for (const auto 编码 : 类型->second) {
            const auto 当前 = 状态_.当前关系.find(编码);
            const auto 历史 = 状态_.历史.find(编码);
            const bool 当前命中 = 当前 != 状态_.当前关系.end();
            const bool 历史命中 = 历史 != 状态_.历史.end();
            const auto* 历史关系 = 历史命中
                ? std::get_if<关系事实>(&历史->second.事实) : nullptr;
            const 关系事实* 事实 = 当前命中 ? &当前->second : 历史关系;
            if (编码 == 0 || 编码 <= 前一编码 || 当前命中 == 历史命中
                || !事实 || 事实->编码.值 != 编码
                || 事实->关系类型节点 != 请求.关系类型节点
                || (请求.方向 == L1中性关系端点方向::源
                    ? 事实->源节点 != 请求.端点节点
                    : 事实->目标节点 != 请求.端点节点)
                || 事实->创建事实代次 == 0
                || 事实->创建事实代次 > 状态_.事实代次
                || (当前命中 && 事实->退出事实代次)
                || (历史命中 && (!事实->退出事实代次
                    || 事实->创建事实代次 > *事实->退出事实代次
                    || *事实->退出事实代次 > 状态_.事实代次)))
                return 失败(L1中性历史关系组读取状态::内部不一致);

            if (事实->创建事实代次 <= 请求.历史截止事实代次
                && (!事实->退出事实代次
                    || 请求.历史截止事实代次 < *事实->退出事实代次))
                结果.关系组.push_back({事实->编码, 事实->源节点,
                    事实->目标节点, 事实->关系类型节点, 事实->角色或顺序,
                    事实->创建事实代次, 事实->退出事实代次});
            前一编码 = 编码;
        }
        return 结果;
    }

    // 诊断责任：向上送出；一次共享许可内按全生命周期候选回读权威值事实。
    L1中性历史属性值组读取结果 读取中性历史属性值组(
        const L1中性历史属性值组读取请求& 请求) const {
        const auto 失败 = [&](L1中性历史属性值组读取状态 状态,
            std::uint64_t 读取代次 = 0) {
            return L1中性历史属性值组读取结果{状态, L1中性CRUD合同版本,
                请求.所属节点, 请求.历史截止事实代次, 读取代次, {}};
        };
        if (请求.合同版本 != L1中性CRUD合同版本 || !有效(请求.所属节点)
            || 请求.历史截止事实代次 == 0)
            return 失败(L1中性历史属性值组读取状态::入口拒绝);

        std::shared_lock<std::shared_mutex> 锁(锁_);
        if (状态_.隔离)
            return 失败(L1中性历史属性值组读取状态::内部不一致);
        if (状态_.事实代次 < 请求.历史截止事实代次)
            return 失败(L1中性历史属性值组读取状态::事实代次漂移,
                状态_.事实代次);

        const auto 所属 = 状态_.历史属性槽值候选索引.find(请求.所属节点.值);
        if (所属 == 状态_.历史属性槽值候选索引.end())
            return 失败(L1中性历史属性值组读取状态::成功, 状态_.事实代次);
        if (所属->second.empty())
            return 失败(L1中性历史属性值组读取状态::内部不一致);

        L1中性历史属性值组读取结果 结果{
            L1中性历史属性值组读取状态::成功, L1中性CRUD合同版本,
            请求.所属节点, 请求.历史截止事实代次, 状态_.事实代次, {}};
        for (const auto& [类型编码, 编码组] : 所属->second) {
            if (类型编码 == 0 || 编码组.empty())
                return 失败(L1中性历史属性值组读取状态::内部不一致);
            std::uint64_t 前一编码 = 0;
            bool 已有有效值 = false;
            for (const auto 编码 : 编码组) {
                const auto 当前 = 状态_.当前值.find(编码);
                const auto 历史 = 状态_.历史.find(编码);
                const bool 当前命中 = 当前 != 状态_.当前值.end();
                const bool 历史命中 = 历史 != 状态_.历史.end();
                const auto* 历史值 = 历史命中
                    ? std::get_if<值事实>(&历史->second.事实) : nullptr;
                const 值事实* 事实 = 当前命中 ? &当前->second : 历史值;
                if (编码 == 0 || 编码 <= 前一编码 || 当前命中 == 历史命中
                    || !事实 || 事实->编码.值 != 编码
                    || 事实->所属节点 != 请求.所属节点
                    || 事实->属性类型节点.值 != 类型编码
                    || !有效(事实->来源节点) || 事实->创建事实代次 == 0
                    || 事实->创建事实代次 > 状态_.事实代次
                    || (当前命中 && 事实->退出事实代次)
                    || (历史命中 && (历史->second.查询编码.值 != 编码
                        || 历史->second.当前有效 || !事实->退出事实代次
                        || 事实->创建事实代次 > *事实->退出事实代次
                        || *事实->退出事实代次 > 状态_.事实代次)))
                    return 失败(L1中性历史属性值组读取状态::内部不一致);

                if (事实->创建事实代次 <= 请求.历史截止事实代次
                    && (!事实->退出事实代次
                        || 请求.历史截止事实代次 < *事实->退出事实代次)) {
                    if (已有有效值)
                        return 失败(L1中性历史属性值组读取状态::内部不一致);
                    const auto 材料 = 转换中性原始材料(事实->材料);
                    if (!材料)
                        return 失败(L1中性历史属性值组读取状态::内部不一致);
                    结果.属性值组.push_back({事实->编码, 事实->所属节点,
                        事实->属性类型节点, *材料, 事实->来源节点,
                        事实->创建事实代次, 事实->退出事实代次});
                    已有有效值 = true;
                }
                前一编码 = 编码;
            }
        }
        std::sort(结果.属性值组.begin(), 结果.属性值组.end(),
            [](const L1中性值事实& 左, const L1中性值事实& 右) noexcept {
                return 左.属性类型节点 < 右.属性类型节点
                    || (左.属性类型节点 == 右.属性类型节点 && 左.编码 < 右.编码);
            });
        return 结果;
    }
    L1属性读取结果 读取当前属性(稳定编码 节点, 稳定编码 类型) const {
        if (!有效(节点) || !有效(类型)) return {};
        std::shared_lock<std::shared_mutex> 锁(锁_);
        if (状态_.隔离) return {L1读取状态::内部不一致, 状态_.事实代次, std::nullopt};
        const auto it = 状态_.当前节点.find(节点.值);
        if (it == 状态_.当前节点.end()) return {状态_.历史.contains(节点.值) ? L1读取状态::已退出 : L1读取状态::未找到, 0, std::nullopt};
        const auto 槽 = std::find_if(it->second.当前属性.begin(), it->second.当前属性.end(), [&](const 属性槽& 值) { return 值.属性类型节点 == 类型; });
        if (槽 == it->second.当前属性.end()) return {L1读取状态::属性未设置, 状态_.事实代次, std::nullopt};
        return {L1读取状态::成功, 状态_.事实代次, L1属性读取副本{节点, 类型, 槽->当前值}};
    }
    L1历史读取结果 读取历史事实(稳定编码 编码) const {
        if (!有效(编码)) return {};
        std::shared_lock<std::shared_mutex> 锁(锁_);
        if (状态_.隔离) return {L1读取状态::内部不一致, std::nullopt};
        if (auto it = 状态_.当前节点.find(编码.值); it != 状态_.当前节点.end()) return {L1读取状态::成功, L1历史事实副本{编码, it->second, true}};
        if (auto it = 状态_.当前关系.find(编码.值); it != 状态_.当前关系.end()) return {L1读取状态::成功, L1历史事实副本{编码, it->second, true}};
        if (auto it = 状态_.当前值.find(编码.值); it != 状态_.当前值.end()) return {L1读取状态::成功, L1历史事实副本{编码, it->second, true}};
        if (auto it = 状态_.历史.find(编码.值); it != 状态_.历史.end()) return {L1读取状态::成功, it->second};
        if (auto it = 状态_.物理清理墓碑.find(编码.值);
            it != 状态_.物理清理墓碑.end())
            return {L1读取状态::历史材料已清理, std::nullopt, it->second};
        return {L1读取状态::未找到, std::nullopt};
    }
    // 诊断责任：向上送出；只复制同一共享锁内的权威事实代次。
    L1事实代次读取结果 尝试读取当前事实代次() const {
        try {
            std::shared_lock<std::shared_mutex> 锁(锁_, std::try_to_lock);
            if (!锁.owns_lock()) return {L1读取状态::资源失败, 0};
            if (状态_.隔离 || !状态完整(状态_))
                return {L1读取状态::内部不一致, 0};
            if (状态_.事实代次 == 0) return {L1读取状态::未找到, 0};
            return {L1读取状态::成功, 状态_.事实代次};
        } catch (...) {
            return {L1读取状态::内部不一致, 0};
        }
    }

    L1所有者范围当前读取结果 读取所有者范围当前节点(
        const L1所有者范围事实读取请求& 请求) const {
        return 读取所有者范围当前事实(请求, 状态_.当前节点);
    }

    L1所有者范围当前读取结果 读取所有者范围当前关系(
        const L1所有者范围事实读取请求& 请求) const {
        return 读取所有者范围当前事实(请求, 状态_.当前关系);
    }

    L1所有者范围当前读取结果 读取所有者范围当前值(
        const L1所有者范围事实读取请求& 请求) const {
        return 读取所有者范围当前事实(请求, 状态_.当前值);
    }

    L1所有者范围来源当前值组读取结果
    按来源读取所有者范围全部当前值(
        const L1所有者范围来源当前值组读取请求& 请求) const {
        L1所有者范围来源当前值组读取结果 结果{
            L1所有者范围来源当前值组读取状态::入口拒绝,
            L1所有者范围来源当前值组读取合同版本,
            请求.所有者, 请求.来源节点, 请求.期望事实代次, 0, {}};
        if (请求.合同版本 != L1所有者范围来源当前值组读取合同版本
            || !有效(请求.所有者) || !有效(请求.来源节点)
            || 请求.期望事实代次 == 0 || 请求.最大数量 == 0)
            return 结果;
        try {
            std::shared_lock<std::shared_mutex> 锁(锁_);
            结果.读取事实代次 = 状态_.事实代次;
            if (状态_.隔离 || !状态完整(状态_)) {
                结果.状态 =
                    L1所有者范围来源当前值组读取状态::内部不一致;
                return 结果;
            }
            if (请求.期望事实代次 != 状态_.事实代次) {
                结果.状态 =
                    L1所有者范围来源当前值组读取状态::事实代次漂移;
                return 结果;
            }
            const auto 所有者 = 状态_.当前所有者.find(
                请求.所有者.编码.值);
            if (所有者 == 状态_.当前所有者.end()) {
                结果.状态 = 状态_.历史所有者.contains(
                    请求.所有者.编码.值)
                    ? L1所有者范围来源当前值组读取状态::已退出
                    : L1所有者范围来源当前值组读取状态::未找到;
                return 结果;
            }
            if (所有者->second.所有者 != 请求.所有者
                || 所有者->second.范围种类
                    != L1所有者范围种类::独占结构范围
                || 所有者->second.创建事实代次 == 0
                || 所有者->second.创建事实代次 > 状态_.事实代次
                || 所有者->second.退出事实代次) {
                结果.状态 =
                    L1所有者范围来源当前值组读取状态::内部不一致;
                return 结果;
            }
            const auto 来源 = 状态_.当前节点.find(请求.来源节点.值);
            if (来源 == 状态_.当前节点.end()) {
                结果.状态 = 状态_.历史.contains(请求.来源节点.值)
                    ? L1所有者范围来源当前值组读取状态::已退出
                    : L1所有者范围来源当前值组读取状态::未找到;
                return 结果;
            }
            if (来源->second.编码 != 请求.来源节点
                || 来源->second.创建事实代次 == 0
                || 来源->second.创建事实代次 > 状态_.事实代次
                || 来源->second.退出事实代次) {
                结果.状态 =
                    L1所有者范围来源当前值组读取状态::内部不一致;
                return 结果;
            }
            const auto 候选 = 状态_.当前节点引用索引.find(
                请求.来源节点.值);
            if (候选 == 状态_.当前节点引用索引.end()) {
                结果.状态 = L1所有者范围来源当前值组读取状态::成功;
                return 结果;
            }
            if (候选->second.empty()) {
                结果.状态 =
                    L1所有者范围来源当前值组读取状态::内部不一致;
                return 结果;
            }
            std::uint64_t 前一编码 = 0;
            for (const auto 编码 : 候选->second) {
                if (编码 == 0 || 编码 <= 前一编码) {
                    结果.当前值.clear();
                    结果.状态 =
                        L1所有者范围来源当前值组读取状态::内部不一致;
                    return 结果;
                }
                前一编码 = 编码;
                const auto 当前值 = 状态_.当前值.find(编码);
                if (当前值 == 状态_.当前值.end()) continue;
                const auto& 值 = 当前值->second;
                if (值.来源节点 != 请求.来源节点
                    || 值.写入所有者 != 请求.所有者) continue;
                if (值.编码.值 != 编码 || 值.退出事实代次
                    || 值.创建事实代次 == 0
                    || 值.创建事实代次 > 状态_.事实代次
                    || !有效(值.所属节点) || !有效(值.属性类型节点)) {
                    结果.当前值.clear();
                    结果.状态 =
                        L1所有者范围来源当前值组读取状态::内部不一致;
                    return 结果;
                }
                if (结果.当前值.size() >= 请求.最大数量) {
                    结果.当前值.clear();
                    结果.状态 =
                        L1所有者范围来源当前值组读取状态::数量预算不足;
                    return 结果;
                }
                结果.当前值.push_back(转换所有者范围值(值));
            }
            结果.状态 = L1所有者范围来源当前值组读取状态::成功;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.当前值.clear();
            结果.状态 = L1所有者范围来源当前值组读取状态::资源失败;
            return 结果;
        } catch (const std::length_error&) {
            结果.当前值.clear();
            结果.状态 = L1所有者范围来源当前值组读取状态::资源失败;
            return 结果;
        } catch (...) {
            结果.当前值.clear();
            结果.状态 =
                L1所有者范围来源当前值组读取状态::内部不一致;
            return 结果;
        }
    }

    L1所有者范围属性类型当前值组读取结果
    按属性类型读取所有者范围全部当前值(
        const L1所有者范围属性类型当前值组读取请求& 请求) const {
        L1所有者范围属性类型当前值组读取结果 结果{
            L1所有者范围属性类型当前值组读取状态::入口拒绝,
            L1所有者范围属性类型当前值组读取合同版本,
            请求.所有者, 请求.属性类型节点, 请求.期望事实代次, 0, {}};
        if (请求.合同版本 != L1所有者范围属性类型当前值组读取合同版本
            || !有效(请求.所有者) || !有效(请求.属性类型节点)
            || 请求.期望事实代次 == 0 || 请求.最大数量 == 0)
            return 结果;
        try {
            std::shared_lock<std::shared_mutex> 锁(锁_);
            结果.读取事实代次 = 状态_.事实代次;
            if (状态_.隔离 || !状态完整(状态_)) {
                结果.状态 =
                    L1所有者范围属性类型当前值组读取状态::内部不一致;
                return 结果;
            }
            if (请求.期望事实代次 != 状态_.事实代次) {
                结果.状态 =
                    L1所有者范围属性类型当前值组读取状态::事实代次漂移;
                return 结果;
            }
            const auto 所有者 = 状态_.当前所有者.find(
                请求.所有者.编码.值);
            if (所有者 == 状态_.当前所有者.end()) {
                结果.状态 = 状态_.历史所有者.contains(
                    请求.所有者.编码.值)
                    ? L1所有者范围属性类型当前值组读取状态::已退出
                    : L1所有者范围属性类型当前值组读取状态::未找到;
                return 结果;
            }
            if (所有者->second.所有者 != 请求.所有者
                || 所有者->second.范围种类
                    != L1所有者范围种类::独占结构范围
                || 所有者->second.创建事实代次 == 0
                || 所有者->second.创建事实代次 > 状态_.事实代次
                || 所有者->second.退出事实代次) {
                结果.状态 =
                    L1所有者范围属性类型当前值组读取状态::内部不一致;
                return 结果;
            }
            const auto 属性类型 = 状态_.当前节点.find(
                请求.属性类型节点.值);
            if (属性类型 == 状态_.当前节点.end()) {
                结果.状态 = 状态_.历史.contains(请求.属性类型节点.值)
                    ? L1所有者范围属性类型当前值组读取状态::已退出
                    : L1所有者范围属性类型当前值组读取状态::未找到;
                return 结果;
            }
            if (属性类型->second.编码 != 请求.属性类型节点
                || 属性类型->second.创建事实代次 == 0
                || 属性类型->second.创建事实代次 > 状态_.事实代次
                || 属性类型->second.退出事实代次) {
                结果.状态 =
                    L1所有者范围属性类型当前值组读取状态::内部不一致;
                return 结果;
            }
            if (属性类型->second.种类 != 节点种类::属性类型
                || !属性类型->second.属性类型表示) {
                结果.状态 =
                    L1所有者范围属性类型当前值组读取状态::入口拒绝;
                return 结果;
            }
            const auto 候选 = 状态_.当前节点引用索引.find(
                请求.属性类型节点.值);
            if (候选 == 状态_.当前节点引用索引.end()) {
                结果.状态 =
                    L1所有者范围属性类型当前值组读取状态::成功;
                return 结果;
            }
            if (候选->second.empty()) {
                结果.状态 =
                    L1所有者范围属性类型当前值组读取状态::内部不一致;
                return 结果;
            }
            std::uint64_t 前一编码 = 0;
            for (const auto 编码 : 候选->second) {
                if (编码 == 0 || 编码 <= 前一编码) {
                    结果.当前值.clear();
                    结果.状态 =
                        L1所有者范围属性类型当前值组读取状态::内部不一致;
                    return 结果;
                }
                前一编码 = 编码;
                const auto 当前值 = 状态_.当前值.find(编码);
                if (当前值 == 状态_.当前值.end()) continue;
                const auto& 值 = 当前值->second;
                if (值.属性类型节点 != 请求.属性类型节点
                    || 值.写入所有者 != 请求.所有者) continue;
                if (值.编码.值 != 编码 || 值.退出事实代次
                    || 值.创建事实代次 == 0
                    || 值.创建事实代次 > 状态_.事实代次
                    || !有效(值.所属节点) || !有效(值.来源节点)) {
                    结果.当前值.clear();
                    结果.状态 =
                        L1所有者范围属性类型当前值组读取状态::内部不一致;
                    return 结果;
                }
                if (结果.当前值.size() >= 请求.最大数量) {
                    结果.当前值.clear();
                    结果.状态 =
                        L1所有者范围属性类型当前值组读取状态::数量预算不足;
                    return 结果;
                }
                结果.当前值.push_back(转换所有者范围值(值));
            }
            结果.状态 = L1所有者范围属性类型当前值组读取状态::成功;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.当前值.clear();
            结果.状态 =
                L1所有者范围属性类型当前值组读取状态::资源失败;
            return 结果;
        } catch (const std::length_error&) {
            结果.当前值.clear();
            结果.状态 =
                L1所有者范围属性类型当前值组读取状态::资源失败;
            return 结果;
        } catch (...) {
            结果.当前值.clear();
            结果.状态 =
                L1所有者范围属性类型当前值组读取状态::内部不一致;
            return 结果;
        }
    }

    L1结构所有者当前读取结果 读取当前结构所有者(
        const L1结构所有者读取请求& 请求) const {
        L1结构所有者当前读取结果 结果{
            L1所有者范围读取状态::入口拒绝,
            L1所有者范围CRUD合同版本, 请求.所有者, 0, std::nullopt};
        if (请求.合同版本 != L1所有者范围CRUD合同版本
            || !有效(请求.所有者)) return 结果;
        std::shared_lock<std::shared_mutex> 锁(锁_);
        if (状态_.隔离 || !状态完整(状态_)) {
            结果.状态 = L1所有者范围读取状态::内部不一致;
            return 结果;
        }
        结果.读取事实代次 = 状态_.事实代次;
        if (const auto 当前 = 状态_.当前所有者.find(请求.所有者.编码.值);
            当前 != 状态_.当前所有者.end()) {
            结果.状态 = L1所有者范围读取状态::成功;
            结果.所有者事实 = 当前->second;
        } else 结果.状态 = 状态_.历史所有者.contains(请求.所有者.编码.值)
            ? L1所有者范围读取状态::已退出
            : L1所有者范围读取状态::未找到;
        return 结果;
    }

    L1结构所有者历史读取结果 读取历史结构所有者(
        const L1结构所有者读取请求& 请求) const {
        L1结构所有者历史读取结果 结果{
            L1所有者范围读取状态::入口拒绝,
            L1所有者范围CRUD合同版本, 请求.所有者, 0, std::nullopt};
        if (请求.合同版本 != L1所有者范围CRUD合同版本
            || !有效(请求.所有者)) return 结果;
        std::shared_lock<std::shared_mutex> 锁(锁_);
        if (状态_.隔离 || !状态完整(状态_)) {
            结果.状态 = L1所有者范围读取状态::内部不一致;
            return 结果;
        }
        结果.读取事实代次 = 状态_.事实代次;
        if (const auto 当前 = 状态_.当前所有者.find(请求.所有者.编码.值);
            当前 != 状态_.当前所有者.end()) {
            结果.状态 = L1所有者范围读取状态::成功;
            结果.所有者事实 = 当前->second;
        } else if (const auto 历史 = 状态_.历史所有者.find(
                请求.所有者.编码.值); 历史 != 状态_.历史所有者.end()) {
            结果.状态 = L1所有者范围读取状态::成功;
            结果.所有者事实 = 历史->second;
        } else 结果.状态 = L1所有者范围读取状态::未找到;
        return 结果;
    }

    L1所有者范围历史读取结果 读取所有者范围历史事实(
        const L1所有者范围事实读取请求& 请求) const {
        L1所有者范围历史读取结果 结果{
            L1所有者范围读取状态::入口拒绝,
            L1所有者范围CRUD合同版本, 请求.编码, 0, std::nullopt};
        if (请求.合同版本 != L1所有者范围CRUD合同版本
            || !有效(请求.编码)) return 结果;
        try {
            std::shared_lock<std::shared_mutex> 锁(锁_);
            if (状态_.隔离 || !状态完整(状态_)) {
                结果.状态 = L1所有者范围读取状态::内部不一致;
                return 结果;
            }
            结果.读取事实代次 = 状态_.事实代次;
            if (const auto 节点 = 状态_.当前节点.find(请求.编码.值);
                节点 != 状态_.当前节点.end())
                结果.事实 = 转换所有者范围节点(节点->second);
            else if (const auto 关系 = 状态_.当前关系.find(请求.编码.值);
                关系 != 状态_.当前关系.end())
                结果.事实 = 转换所有者范围关系(关系->second);
            else if (const auto 值 = 状态_.当前值.find(请求.编码.值);
                值 != 状态_.当前值.end())
                结果.事实 = 转换所有者范围值(值->second);
            else if (const auto 历史 = 状态_.历史.find(请求.编码.值);
                历史 != 状态_.历史.end())
                结果.事实 = 转换所有者范围事实(历史->second.事实);
            else if (const auto 墓碑 = 状态_.物理清理墓碑.find(请求.编码.值);
                墓碑 != 状态_.物理清理墓碑.end()) {
                结果.状态 = L1所有者范围读取状态::历史材料已清理;
                结果.物理清理事实代次 = 墓碑->second.物理清理事实代次;
                结果.物理清理墓碑 =
                    转换所有者范围物理清理墓碑(墓碑->second);
                return 结果;
            }
            else {
                结果.状态 = L1所有者范围读取状态::未找到;
                return 结果;
            }
            结果.状态 = L1所有者范围读取状态::成功;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.状态 = L1所有者范围读取状态::资源失败;
            结果.事实.reset(); return 结果;
        } catch (...) {
            结果.状态 = L1所有者范围读取状态::内部不一致;
            结果.事实.reset(); return 结果;
        }
    }

    L1所有者范围源关系组读取结果 读取所有者范围当前源关系组(
        const L1所有者范围源关系组读取请求& 请求) const {
        L1所有者范围源关系组读取结果 结果{
            L1所有者范围读取状态::入口拒绝,
            L1所有者范围CRUD合同版本, 请求.源节点,
            请求.关系类型节点, 0, {}};
        if (请求.合同版本 != L1所有者范围CRUD合同版本
            || !有效(请求.源节点) || !有效(请求.关系类型节点)) return 结果;
        try {
            std::shared_lock<std::shared_mutex> 锁(锁_);
            if (状态_.隔离 || !状态完整(状态_)) {
                结果.状态 = L1所有者范围读取状态::内部不一致; return 结果;
            }
            结果.读取事实代次 = 状态_.事实代次;
            const auto 源 = 状态_.当前源关系索引.find(请求.源节点.值);
            if (源 != 状态_.当前源关系索引.end()) {
                const auto 类型 = 源->second.find(请求.关系类型节点.值);
                if (类型 != 源->second.end())
                    for (const auto 编码 : 类型->second) {
                        const auto 关系 = 状态_.当前关系.find(编码);
                        if (关系 == 状态_.当前关系.end()) {
                            结果.状态 = L1所有者范围读取状态::内部不一致;
                            结果.关系组.clear(); return 结果;
                        }
                        结果.关系组.push_back(转换所有者范围关系(关系->second));
                    }
            }
            结果.状态 = L1所有者范围读取状态::成功;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.状态 = L1所有者范围读取状态::资源失败;
            结果.关系组.clear(); return 结果;
        } catch (...) {
            结果.状态 = L1所有者范围读取状态::内部不一致;
            结果.关系组.clear(); return 结果;
        }
    }

    L1所有者范围目标关系组读取结果 读取所有者范围当前目标关系组(
        const L1所有者范围目标关系组读取请求& 请求) const {
        L1所有者范围目标关系组读取结果 结果{
            L1所有者范围读取状态::入口拒绝,
            L1所有者范围CRUD合同版本, 请求.目标节点,
            请求.关系类型节点, 0, {}};
        if (请求.合同版本 != L1所有者范围CRUD合同版本
            || !有效(请求.目标节点) || !有效(请求.关系类型节点)) return 结果;
        try {
            std::shared_lock<std::shared_mutex> 锁(锁_);
            if (状态_.隔离 || !状态完整(状态_)) {
                结果.状态 = L1所有者范围读取状态::内部不一致; return 结果;
            }
            结果.读取事实代次 = 状态_.事实代次;
            const auto 目标 = 状态_.当前目标关系索引.find(请求.目标节点.值);
            if (目标 != 状态_.当前目标关系索引.end()) {
                const auto 类型 = 目标->second.find(请求.关系类型节点.值);
                if (类型 != 目标->second.end())
                    for (const auto 编码 : 类型->second) {
                        const auto 关系 = 状态_.当前关系.find(编码);
                        if (关系 == 状态_.当前关系.end()) {
                            结果.状态 = L1所有者范围读取状态::内部不一致;
                            结果.关系组.clear(); return 结果;
                        }
                        结果.关系组.push_back(转换所有者范围关系(关系->second));
                    }
            }
            结果.状态 = L1所有者范围读取状态::成功;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.状态 = L1所有者范围读取状态::资源失败;
            结果.关系组.clear(); return 结果;
        } catch (...) {
            结果.状态 = L1所有者范围读取状态::内部不一致;
            结果.关系组.clear(); return 结果;
        }
    }

    L1所有者范围历史关系组读取结果 读取所有者范围历史关系组(
        const L1所有者范围历史关系组读取请求& 请求) const {
        L1所有者范围历史关系组读取结果 结果{
            L1所有者范围读取状态::入口拒绝,
            L1所有者范围CRUD合同版本, 请求.方向, 请求.端点节点,
            请求.关系类型节点, 请求.历史截止事实代次, 0, {}};
        const bool 方向有效 = 请求.方向 == L1所有者范围关系端点方向::源
            || 请求.方向 == L1所有者范围关系端点方向::目标;
        if (请求.合同版本 != L1所有者范围CRUD合同版本 || !方向有效
            || !有效(请求.端点节点) || !有效(请求.关系类型节点)
            || 请求.历史截止事实代次 == 0) return 结果;
        try {
            std::shared_lock<std::shared_mutex> 锁(锁_);
            if (状态_.隔离 || !状态完整(状态_)) {
                结果.状态 = L1所有者范围读取状态::内部不一致; return 结果;
            }
            结果.读取事实代次 = 状态_.事实代次;
            if (请求.历史截止事实代次 > 状态_.事实代次) {
                结果.状态 = L1所有者范围读取状态::事实代次漂移; return 结果;
            }
            const auto& 索引 = 请求.方向 == L1所有者范围关系端点方向::源
                ? 状态_.历史源关系候选索引 : 状态_.历史目标关系候选索引;
            const auto 端点 = 索引.find(请求.端点节点.值);
            if (端点 != 索引.end()) {
                const auto 类型 = 端点->second.find(请求.关系类型节点.值);
                if (类型 != 端点->second.end())
                    for (const auto 编码 : 类型->second) {
                        const 关系事实* 事实 = nullptr;
                        if (const auto 当前 = 状态_.当前关系.find(编码);
                            当前 != 状态_.当前关系.end()) 事实 = &当前->second;
                        else if (const auto 历史 = 状态_.历史.find(编码);
                            历史 != 状态_.历史.end())
                            事实 = std::get_if<关系事实>(&历史->second.事实);
                        if (!事实) {
                            结果.状态 = L1所有者范围读取状态::内部不一致;
                            结果.关系组.clear(); return 结果;
                        }
                        if (事实->创建事实代次 <= 请求.历史截止事实代次
                            && (!事实->退出事实代次
                                || 请求.历史截止事实代次 < *事实->退出事实代次))
                            结果.关系组.push_back(转换所有者范围关系(*事实));
                    }
            }
            结果.状态 = L1所有者范围读取状态::成功;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.状态 = L1所有者范围读取状态::资源失败;
            结果.关系组.clear(); return 结果;
        } catch (...) {
            结果.状态 = L1所有者范围读取状态::内部不一致;
            结果.关系组.clear(); return 结果;
        }
    }

    L1所有者范围历史属性值组读取结果 读取所有者范围历史属性值组(
        const L1所有者范围历史属性值组读取请求& 请求) const {
        L1所有者范围历史属性值组读取结果 结果{
            L1所有者范围读取状态::入口拒绝,
            L1所有者范围CRUD合同版本, 请求.所属节点,
            请求.历史截止事实代次, 0, {}};
        if (请求.合同版本 != L1所有者范围CRUD合同版本
            || !有效(请求.所属节点) || 请求.历史截止事实代次 == 0)
            return 结果;
        try {
            std::shared_lock<std::shared_mutex> 锁(锁_);
            if (状态_.隔离 || !状态完整(状态_)) {
                结果.状态 = L1所有者范围读取状态::内部不一致; return 结果;
            }
            结果.读取事实代次 = 状态_.事实代次;
            if (请求.历史截止事实代次 > 状态_.事实代次) {
                结果.状态 = L1所有者范围读取状态::事实代次漂移; return 结果;
            }
            const auto 所属 = 状态_.历史属性槽值候选索引.find(
                请求.所属节点.值);
            if (所属 != 状态_.历史属性槽值候选索引.end())
                for (const auto& [_, 编码组] : 所属->second)
                    for (const auto 编码 : 编码组) {
                        const 值事实* 事实 = nullptr;
                        if (const auto 当前 = 状态_.当前值.find(编码);
                            当前 != 状态_.当前值.end()) 事实 = &当前->second;
                        else if (const auto 历史 = 状态_.历史.find(编码);
                            历史 != 状态_.历史.end())
                            事实 = std::get_if<值事实>(&历史->second.事实);
                        if (!事实) {
                            结果.状态 = L1所有者范围读取状态::内部不一致;
                            结果.属性值组.clear(); return 结果;
                        }
                        if (事实->创建事实代次 <= 请求.历史截止事实代次
                            && (!事实->退出事实代次
                                || 请求.历史截止事实代次 < *事实->退出事实代次))
                            结果.属性值组.push_back(转换所有者范围值(*事实));
                    }
            std::sort(结果.属性值组.begin(), 结果.属性值组.end(),
                [](const auto& 左, const auto& 右) noexcept {
                    return 左.属性类型节点 < 右.属性类型节点
                        || (左.属性类型节点 == 右.属性类型节点
                            && 左.编码 < 右.编码);
                });
            结果.状态 = L1所有者范围读取状态::成功;
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.状态 = L1所有者范围读取状态::资源失败;
            结果.属性值组.clear(); return 结果;
        } catch (...) {
            结果.状态 = L1所有者范围读取状态::内部不一致;
            结果.属性值组.clear(); return 结果;
        }
    }

    L1所有者范围一致当前读取结果 尝试读取所有者范围一致当前投影(
        const L1所有者范围一致当前读取请求& 请求) const {
        if (请求.合同版本 != L1所有者范围一致当前读取合同版本
            || (请求.所有者.empty() && 请求.节点.empty()
                && 请求.关系.empty() && 请求.值.empty()
                && 请求.属性值.empty() && 请求.源关系组.empty()
                && 请求.目标关系组.empty())) {
            L1所有者范围一致当前读取结果 结果;
            结果.期望事实代次 = 请求.期望事实代次;
            return 结果;
        }
        auto 内部 = 尝试读取所有者范围一致投影内部({
            请求.期望事实代次, 请求.所有者, 请求.节点, 请求.关系,
            请求.值, 请求.属性值, 请求.源关系组, 请求.目标关系组,
            nullptr});
        L1所有者范围一致当前读取结果 结果;
        结果.状态 = 内部.状态;
        结果.期望事实代次 = 内部.期望事实代次;
        结果.读取事实代次 = 内部.读取事实代次;
        结果.所有者 = std::move(内部.所有者);
        结果.节点 = std::move(内部.节点);
        结果.关系 = std::move(内部.关系);
        结果.值 = std::move(内部.值);
        结果.属性值 = std::move(内部.属性值);
        结果.源关系组 = std::move(内部.源关系组);
        结果.目标关系组 = std::move(内部.目标关系组);
        return 结果;
    }

    L1所有者范围一致关系类型闭包读取结果
    尝试读取所有者范围一致关系类型闭包投影(
        const L1所有者范围一致关系类型闭包读取请求& 请求) const {
        const auto 失败 = [&](L1所有者范围一致当前读取状态 状态) {
            L1所有者范围一致关系类型闭包读取结果 结果;
            结果.状态 = 状态;
            结果.期望事实代次 = 请求.期望事实代次;
            return 结果;
        };
        if (请求.合同版本 != L1所有者范围一致关系类型闭包读取合同版本
            || (请求.所有者.empty() && 请求.节点.empty()
                && 请求.关系.empty() && 请求.值.empty()
                && 请求.属性值.empty() && 请求.源关系组.empty()
                && 请求.目标关系组.empty() && 请求.关系类型闭包.empty()))
            return 失败(L1所有者范围一致当前读取状态::入口拒绝);
        try {
            if (!关系类型闭包选择有效(请求.关系类型闭包))
                return 失败(L1所有者范围一致当前读取状态::入口拒绝);
            return 尝试读取所有者范围一致投影内部({
                请求.期望事实代次, 请求.所有者, 请求.节点, 请求.关系,
                请求.值, 请求.属性值, 请求.源关系组, 请求.目标关系组,
                &请求.关系类型闭包});
        } catch (const std::bad_alloc&) {
            return 失败(L1所有者范围一致当前读取状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L1所有者范围一致当前读取状态::资源失败);
        } catch (...) {
            return 失败(L1所有者范围一致当前读取状态::内部不一致);
        }
    }
private:
    static bool 稳定编码组严格升序(const std::vector<稳定编码>& 编码组) noexcept {
        std::uint64_t 前一编码 = 0;
        for (const auto 编码 : 编码组) {
            if (!有效(编码) || 编码.值 <= 前一编码) return false;
            前一编码 = 编码.值;
        }
        return true;
    }

    static bool 关系类型闭包选择有效(
        const std::vector<L1所有者范围一致关系类型闭包选择项>& 选择组) {
        std::unordered_set<std::uint64_t> 入口关系类型;
        入口关系类型.reserve(选择组.size());
        for (const auto& 选择 : 选择组) {
            if (!有效(选择.入口关系类型节点)
                || !入口关系类型.insert(选择.入口关系类型节点.值).second
                || !稳定编码组严格升序(选择.源节点属性类型)
                || !稳定编码组严格升序(选择.源节点源关系类型)
                || !稳定编码组严格升序(选择.源节点目标关系类型)
                || !稳定编码组严格升序(选择.目标节点属性类型)
                || !稳定编码组严格升序(选择.目标节点源关系类型)
                || !稳定编码组严格升序(选择.目标节点目标关系类型))
                return false;
        }
        return true;
    }

    L1所有者范围一致关系类型闭包读取结果
    尝试读取所有者范围一致投影内部(
        const 所有者范围一致投影内部请求& 请求) const {
        const auto 失败 = [&](L1所有者范围一致当前读取状态 状态,
            std::uint64_t 代次 = 0) {
            L1所有者范围一致关系类型闭包读取结果 结果;
            结果.状态 = 状态;
            结果.期望事实代次 = 请求.期望事实代次;
            结果.读取事实代次 = 代次;
            return 结果;
        };
        try {
            std::shared_lock<std::shared_mutex> 锁(锁_, std::try_to_lock);
            if (!锁.owns_lock())
                return 失败(L1所有者范围一致当前读取状态::资源失败);
            if (状态_.隔离 || !状态完整(状态_))
                return 失败(L1所有者范围一致当前读取状态::内部不一致);
            if (请求.期望事实代次 != 0
                && 状态_.事实代次 != 请求.期望事实代次)
                return 失败(L1所有者范围一致当前读取状态::事实代次漂移,
                    状态_.事实代次);

            L1所有者范围一致关系类型闭包读取结果 结果;
            结果.状态 = L1所有者范围一致当前读取状态::成功;
            结果.期望事实代次 = 请求.期望事实代次;
            结果.读取事实代次 = 状态_.事实代次;
            结果.所有者.reserve(请求.所有者.size());
            结果.节点.reserve(请求.节点.size());
            结果.关系.reserve(请求.关系.size());
            结果.值.reserve(请求.值.size());
            结果.属性值.reserve(请求.属性值.size());
            结果.源关系组.reserve(请求.源关系组.size());
            结果.目标关系组.reserve(请求.目标关系组.size());
            if (请求.关系类型闭包)
                结果.关系类型闭包.reserve(请求.关系类型闭包->size());

            std::unordered_set<std::uint64_t> 唯一;
            for (const auto 所有者 : 请求.所有者) {
                if (!有效(所有者) || !唯一.insert(所有者.编码.值).second)
                    return 失败(L1所有者范围一致当前读取状态::入口拒绝);
                L1所有者范围一致所有者读取结果项 项{所有者,
                    L1所有者范围一致当前读取项目状态::未找到, std::nullopt};
                if (const auto 当前 = 状态_.当前所有者.find(所有者.编码.值);
                    当前 != 状态_.当前所有者.end()) {
                    项.状态 = L1所有者范围一致当前读取项目状态::成功;
                    项.所有者事实 = 当前->second;
                } else if (状态_.历史所有者.contains(所有者.编码.值))
                    项.状态 = L1所有者范围一致当前读取项目状态::已退出;
                结果.所有者.push_back(std::move(项));
            }

            const auto 添加具名 = [&](稳定编码 编码,
                一致当前事实种类 种类, auto& 输出) -> bool {
                if (!有效(编码) || !唯一.insert(编码.值).second) return false;
                const auto 内部 = 读取一致具名当前事实(状态_, 编码, 种类);
                if (!内部) return false;
                using 输出项 = typename std::decay_t<decltype(输出)>::value_type;
                输出项 项;
                项.查询编码 = 编码;
                项.状态 = static_cast<L1所有者范围一致当前读取项目状态>(
                    static_cast<std::uint8_t>(内部->状态));
                项.物理清理事实代次 = 内部->物理清理事实代次;
                if (内部->物理清理墓碑)
                    项.物理清理墓碑 = 转换所有者范围物理清理墓碑(
                        *内部->物理清理墓碑);
                if (内部->事实) {
                    if constexpr (std::is_same_v<输出项,
                        L1所有者范围一致节点读取结果项>) {
                        const auto* 事实 = std::get_if<节点事实>(&*内部->事实);
                        if (!事实) return false;
                        项.事实 = 转换所有者范围节点(*事实);
                    } else if constexpr (std::is_same_v<输出项,
                        L1所有者范围一致关系读取结果项>) {
                        const auto* 事实 = std::get_if<关系事实>(&*内部->事实);
                        if (!事实) return false;
                        项.事实 = 转换所有者范围关系(*事实);
                    } else {
                        const auto* 事实 = std::get_if<值事实>(&*内部->事实);
                        if (!事实) return false;
                        项.事实 = 转换所有者范围值(*事实);
                    }
                }
                输出.push_back(std::move(项));
                return true;
            };
            for (const auto 编码 : 请求.节点)
                if (!添加具名(编码, 一致当前事实种类::节点, 结果.节点))
                    return 失败(L1所有者范围一致当前读取状态::入口拒绝);
            for (const auto 编码 : 请求.关系)
                if (!添加具名(编码, 一致当前事实种类::关系, 结果.关系))
                    return 失败(L1所有者范围一致当前读取状态::入口拒绝);
            for (const auto 编码 : 请求.值)
                if (!添加具名(编码, 一致当前事实种类::值, 结果.值))
                    return 失败(L1所有者范围一致当前读取状态::入口拒绝);

            for (const auto& 选择 : 请求.属性值) {
                auto 项 = 读取所有者范围一致属性值(
                    状态_, 选择.节点, 选择.属性类型);
                if (!项)
                    return 失败(L1所有者范围一致当前读取状态::内部不一致);
                结果.属性值.push_back(std::move(*项));
            }
            for (const auto& 选择 : 请求.源关系组) {
                auto 项 = 读取所有者范围一致源关系组(状态_, 选择);
                if (!项)
                    return 失败(L1所有者范围一致当前读取状态::内部不一致);
                结果.源关系组.push_back(std::move(*项));
            }
            for (const auto& 选择 : 请求.目标关系组) {
                auto 项 = 读取所有者范围一致目标关系组(状态_, 选择);
                if (!项)
                    return 失败(L1所有者范围一致当前读取状态::内部不一致);
                结果.目标关系组.push_back(std::move(*项));
            }
            if (请求.关系类型闭包) {
                for (const auto& 选择 : *请求.关系类型闭包) {
                    auto 项 = 读取一致关系类型闭包(状态_, 选择);
                    if (!项)
                        return 失败(L1所有者范围一致当前读取状态::内部不一致);
                    结果.关系类型闭包.push_back(std::move(*项));
                }
            }
            return 结果;
        } catch (const std::bad_alloc&) {
            return 失败(L1所有者范围一致当前读取状态::资源失败);
        } catch (const std::length_error&) {
            return 失败(L1所有者范围一致当前读取状态::资源失败);
        } catch (...) {
            return 失败(L1所有者范围一致当前读取状态::内部不一致);
        }
    }

    static L1所有者范围节点事实 转换所有者范围节点(const 节点事实& 事实) {
        L1所有者范围节点事实 结果;
        结果.编码 = 事实.编码;
        结果.种类 = 事实.种类;
        if (事实.属性类型表示)
            结果.属性类型表示 = static_cast<L1所有者范围值表示种类>(
                static_cast<std::uint8_t>(*事实.属性类型表示));
        结果.写入所有者 = 事实.写入所有者;
        结果.创建事实代次 = 事实.创建事实代次;
        结果.退出事实代次 = 事实.退出事实代次;
        结果.当前属性.reserve(事实.当前属性.size());
        for (const auto& 槽 : 事实.当前属性)
            结果.当前属性.push_back({槽.属性类型节点, 槽.当前值});
        return 结果;
    }

    static L1所有者范围关系事实 转换所有者范围关系(
        const 关系事实& 事实) noexcept {
        return {事实.编码, 事实.源节点, 事实.目标节点,
            事实.关系类型节点, 事实.角色或顺序, 事实.写入所有者,
            事实.创建事实代次, 事实.退出事实代次};
    }

    static L1所有者范围原始值材料 转换所有者范围材料(
        const 原始值材料& 材料) {
        return std::visit([](const auto& 值) -> L1所有者范围原始值材料 {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, 独立材料引用>)
                return L1所有者范围独立材料引用{值.编码};
            else return 值;
        }, 材料);
    }

    static L1所有者范围值事实 转换所有者范围值(const 值事实& 事实) {
        return {事实.编码, 事实.所属节点, 事实.属性类型节点,
            转换所有者范围材料(事实.材料), 事实.来源节点,
            事实.写入所有者, 事实.创建事实代次, 事实.退出事实代次};
    }

    static L1所有者范围事实副本 转换所有者范围事实(
        const L1事实副本& 事实) {
        return std::visit([](const auto& 值) -> L1所有者范围事实副本 {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, 节点事实>)
                return 转换所有者范围节点(值);
            else if constexpr (std::is_same_v<类型, 关系事实>)
                return 转换所有者范围关系(值);
            else return 转换所有者范围值(值);
        }, 事实);
    }

    static L1所有者范围物理清理墓碑 转换所有者范围物理清理墓碑(
        const L1物理清理墓碑& 墓碑) noexcept {
        return {墓碑.编码,
            static_cast<L1所有者范围物理清理事实种类>(
                static_cast<std::uint8_t>(墓碑.事实种类)),
            墓碑.内部结构分区, 墓碑.创建事实代次, 墓碑.退出事实代次,
            墓碑.物理清理事实代次};
    }

    enum class 当前节点引用闭包检查结果 : std::uint8_t {
        已闭合 = 1,
        引用冲突 = 2,
        内部不一致 = 3
    };

    struct 中性幂等记录 {
        L1中性写集请求 首次规范化写集;
        std::uint64_t 首次发布事实代次 = 0;
        std::vector<std::pair<L1中性写集本地键, 稳定编码>> 首次新编码映射;
        L1中性写入结果 首次结果;
    };
    struct 所有者范围幂等记录 {
        L1中性写集请求 首次规范化写集;
        std::uint64_t 首次发布事实代次 = 0;
        std::vector<std::pair<L1中性写集本地键, 稳定编码>> 首次新编码映射;
        L1中性写入结果 首次结果;
    };
    struct 所有者建立幂等记录 {
        L1所有者范围建立请求 首次请求;
        L1所有者范围建立结果 首次结果;
    };
    struct 跨所有者原子事务幂等记录 {
        L1跨所有者原子事务请求 请求;
        L1跨所有者原子事务结果 结果;
    };
    struct 三分区原子事务幂等记录_v2 {
        L1三分区原子事务请求_v2 请求;
        L1三分区原子事务结果_v2 结果;
    };
    struct 有限N分区原子事务幂等记录_v3 {
        L1有限N分区原子事务请求_v3 请求;
        L1有限N分区原子事务结果_v3 结果;
    };
    struct 状态 {
        std::uint64_t 事实代次 = 0;
        std::uint64_t 下个编码 = 1;
        std::unordered_map<std::uint64_t, L1结构所有者事实> 当前所有者;
        std::unordered_map<std::uint64_t, L1结构所有者事实> 历史所有者;
        std::unordered_map<std::uint64_t, 节点事实> 当前节点;
        std::unordered_map<std::uint64_t, 关系事实> 当前关系;
        std::unordered_map<std::uint64_t,
            std::unordered_map<std::uint64_t, std::vector<std::uint64_t>>>
            当前源关系索引;
        std::unordered_map<std::uint64_t,
            std::unordered_map<std::uint64_t, std::vector<std::uint64_t>>>
            当前目标关系索引;
        std::unordered_map<std::uint64_t, std::vector<std::uint64_t>>
            当前关系类型索引;
        std::unordered_map<std::uint64_t,
            std::unordered_map<std::uint64_t, std::vector<std::uint64_t>>>
            历史源关系候选索引;
        std::unordered_map<std::uint64_t,
            std::unordered_map<std::uint64_t, std::vector<std::uint64_t>>>
            历史目标关系候选索引;
        std::unordered_map<std::uint64_t,
            std::unordered_map<std::uint64_t, std::vector<std::uint64_t>>>
            历史属性槽值候选索引;
        std::unordered_map<std::uint64_t, std::vector<std::uint64_t>>
            当前节点引用索引;
        std::unordered_map<std::uint64_t, 值事实> 当前值;
        std::unordered_map<std::uint64_t, L1历史事实副本> 历史;
        std::unordered_set<std::uint64_t> 永久占用;
        std::unordered_map<std::uint64_t, L1物理清理墓碑> 物理清理墓碑;
        std::unordered_map<std::uint64_t, 物理清理幂等记录> 物理清理幂等账;
        std::unordered_map<std::uint64_t, 中性幂等记录> 中性幂等账;
        std::unordered_map<std::uint64_t, 所有者建立幂等记录>
            所有者建立幂等账;
        std::unordered_map<std::uint64_t,
            std::unordered_map<std::uint64_t, 所有者范围幂等记录>>
            所有者范围幂等账;
        std::unordered_map<std::uint64_t, 跨所有者原子事务幂等记录>
            跨所有者原子事务幂等账;
        std::unordered_map<std::uint64_t, 三分区原子事务幂等记录_v2>
            三分区原子事务幂等账_v2;
        std::unordered_map<std::uint64_t, 有限N分区原子事务幂等记录_v3>
            有限N分区原子事务幂等账_v3;
        std::optional<L1结构所有者身份> 旧共享所有者定位;
        bool 隔离 = false;
    };

    enum class 持久准备状态 : std::uint8_t {
        成功 = 1, 资源失败 = 2, 内部不一致 = 3, 持久证据未知 = 4
    };

    struct 持久会话 final {
        std::filesystem::path 根;
        HANDLE 锁句柄 = INVALID_HANDLE_VALUE;
        std::uint64_t 快照序号 = 0;
        std::uint8_t 活动槽 = 0;
        bool 已毒化 = false;
        ~持久会话() {
            if (锁句柄 != INVALID_HANDLE_VALUE) CloseHandle(锁句柄);
        }
    };

    struct 规范编码器 final {
        std::vector<std::uint8_t> 字节;
        void U8(std::uint8_t 值) { 字节.push_back(值); }
        void U32(std::uint32_t 值) {
            for (unsigned i = 0; i != 4; ++i) U8(static_cast<std::uint8_t>(值 >> (i * 8)));
        }
        void U64(std::uint64_t 值) {
            for (unsigned i = 0; i != 8; ++i) U8(static_cast<std::uint8_t>(值 >> (i * 8)));
        }
        void I64(std::int64_t 值) { U64(static_cast<std::uint64_t>(值)); }
        void 布尔(bool 值) { U8(值 ? 1 : 0); }
    };

    struct 规范解码器 final {
        const std::vector<std::uint8_t>& 字节;
        std::size_t 位置 = 0;
        bool U8(std::uint8_t& 值) {
            if (位置 >= 字节.size()) return false;
            值 = 字节[位置++]; return true;
        }
        bool U32(std::uint32_t& 值) {
            值 = 0; std::uint8_t 一{};
            for (unsigned i = 0; i != 4; ++i) { if (!U8(一)) return false; 值 |= std::uint32_t{一} << (i * 8); }
            return true;
        }
        bool U64(std::uint64_t& 值) {
            值 = 0; std::uint8_t 一{};
            for (unsigned i = 0; i != 8; ++i) { if (!U8(一)) return false; 值 |= std::uint64_t{一} << (i * 8); }
            return true;
        }
        bool I64(std::int64_t& 值) { std::uint64_t 原{}; if (!U64(原)) return false; 值 = static_cast<std::int64_t>(原); return true; }
        bool 布尔(bool& 值) { std::uint8_t 原{}; if (!U8(原) || 原 > 1) return false; 值 = 原 != 0; return true; }
        bool 完结() const noexcept { return 位置 == 字节.size(); }
    };

    template<class T, class 写函数>
    static void 写组(规范编码器& 出, const std::vector<T>& 组, 写函数 写一) {
        出.U64(static_cast<std::uint64_t>(组.size()));
        for (const auto& 项 : 组) 写一(出, 项);
    }
    template<class T, class 读函数>
    static bool 读组(规范解码器& 入, std::vector<T>& 组, 读函数 读一) {
        std::uint64_t 数{};
        if (!入.U64(数) || 数 > 入.字节.size() - 入.位置) return false;
        组.clear(); 组.reserve(static_cast<std::size_t>(数));
        for (std::uint64_t i = 0; i != 数; ++i) { T 项{}; if (!读一(入, 项)) return false; 组.push_back(std::move(项)); }
        return true;
    }
    template<class T, class 写函数>
    static void 写可选(规范编码器& 出, const std::optional<T>& 值, 写函数 写一) {
        出.U8(值 ? 1 : 0); if (值) 写一(出, *值);
    }
    template<class T, class 读函数>
    static bool 读可选(规范解码器& 入, std::optional<T>& 值, 读函数 读一) {
        std::uint8_t 有{}; if (!入.U8(有) || 有 > 1) return false;
        if (!有) { 值.reset(); return true; }
        T 项{}; if (!读一(入, 项)) return false; 值 = std::move(项); return true;
    }
    template<class E>
    static void 写枚举(规范编码器& 出, E 值) { 出.U8(static_cast<std::uint8_t>(值)); }
    template<class E>
    static bool 读枚举(规范解码器& 入, E& 值, std::uint8_t 最大) {
        std::uint8_t 原{}; if (!入.U8(原) || 原 == 0 || 原 > 最大) return false;
        值 = static_cast<E>(原); return true;
    }

    static void 写(规范编码器& 出, 稳定编码 值) { 出.U64(值.值); }
    static bool 读(规范解码器& 入, 稳定编码& 值) { return 入.U64(值.值); }
    static void 写(规范编码器& 出, L1结构所有者身份 值) { 写(出, 值.编码); }
    static bool 读(规范解码器& 入, L1结构所有者身份& 值) { return 读(入, 值.编码); }
    static void 写(规范编码器& 出, L1中性写集本地键 值) { 出.U32(值.值); }
    static bool 读(规范解码器& 入, L1中性写集本地键& 值) { return 入.U32(值.值); }
    static void 写(规范编码器& 出, L1所有者范围写集本地键 值) { 出.U32(值.值); }
    static void 写(规范编码器& 出, L1中性写集幂等键 值) { 出.U64(值.值); }
    static bool 读(规范解码器& 入, L1中性写集幂等键& 值) { return 入.U64(值.值); }
    static void 写(规范编码器& 出, L1所有者范围建立幂等身份 值) { 出.U64(值.值); }
    static bool 读(规范解码器& 入, L1所有者范围建立幂等身份& 值) { return 入.U64(值.值); }
    static void 写(规范编码器& 出, L1所有者范围写入幂等身份 值) { 出.U64(值.值); }
    static bool 读(规范解码器& 入, L1所有者范围写入幂等身份& 值) { return 入.U64(值.值); }

    static void 写原始材料(规范编码器& 出, const 原始值材料& 值) {
        if (const auto* 项 = std::get_if<std::int64_t>(&值)) { 出.U8(1); 出.I64(*项); }
        else if (const auto* 项 = std::get_if<std::vector<std::int64_t>>(&值)) { 出.U8(2); 写组(出, *项, [](auto& e, auto v){ e.I64(v); }); }
        else if (const auto* 项 = std::get_if<std::vector<std::uint64_t>>(&值)) { 出.U8(3); 写组(出, *项, [](auto& e, auto v){ e.U64(v); }); }
        else { 出.U8(4); 写(出, std::get<独立材料引用>(值).编码); }
    }
    static bool 读原始材料(规范解码器& 入, 原始值材料& 值) {
        std::uint8_t 标签{}; if (!入.U8(标签)) return false;
        if (标签 == 1) { std::int64_t 项{}; if (!入.I64(项)) return false; 值 = 项; return true; }
        if (标签 == 2) { std::vector<std::int64_t> 项; if (!读组(入, 项, [](auto& d, auto& v){ return d.I64(v); })) return false; 值 = std::move(项); return true; }
        if (标签 == 3) { std::vector<std::uint64_t> 项; if (!读组(入, 项, [](auto& d, auto& v){ return d.U64(v); })) return false; 值 = std::move(项); return true; }
        if (标签 == 4) { 独立材料引用 项; if (!读(入, 项.编码)) return false; 值 = 项; return true; }
        return false;
    }
    static void 写中性材料(规范编码器& 出, const L1中性原始值材料& 值) {
        if (const auto* 项 = std::get_if<std::int64_t>(&值)) { 出.U8(1); 出.I64(*项); }
        else if (const auto* 项 = std::get_if<std::vector<std::int64_t>>(&值)) { 出.U8(2); 写组(出, *项, [](auto& e, auto v){ e.I64(v); }); }
        else if (const auto* 项 = std::get_if<std::vector<std::uint64_t>>(&值)) { 出.U8(3); 写组(出, *项, [](auto& e, auto v){ e.U64(v); }); }
        else { 出.U8(4); 写(出, std::get<L1中性独立材料引用>(值).编码); }
    }
    static bool 读中性材料(规范解码器& 入, L1中性原始值材料& 值) {
        原始值材料 临时; if (!读原始材料(入, 临时)) return false;
        值 = std::visit([](const auto& 项) -> L1中性原始值材料 {
            using T = std::decay_t<decltype(项)>;
            if constexpr (std::is_same_v<T, 独立材料引用>) return L1中性独立材料引用{项.编码};
            else return 项;
        }, 临时); return true;
    }
    static void 写所有者材料(规范编码器& 出, const L1所有者范围原始值材料& 值) {
        if (const auto* 项 = std::get_if<std::int64_t>(&值)) { 出.U8(1); 出.I64(*项); }
        else if (const auto* 项 = std::get_if<std::vector<std::int64_t>>(&值)) { 出.U8(2); 写组(出, *项, [](auto& e, auto v){ e.I64(v); }); }
        else if (const auto* 项 = std::get_if<std::vector<std::uint64_t>>(&值)) { 出.U8(3); 写组(出, *项, [](auto& e, auto v){ e.U64(v); }); }
        else { 出.U8(4); 写(出, std::get<L1所有者范围独立材料引用>(值).编码); }
    }
    static bool 读所有者材料(规范解码器& 入, L1所有者范围原始值材料& 值) {
        原始值材料 临时; if (!读原始材料(入, 临时)) return false;
        值 = std::visit([](const auto& 项) -> L1所有者范围原始值材料 {
            using T = std::decay_t<decltype(项)>;
            if constexpr (std::is_same_v<T, 独立材料引用>) return L1所有者范围独立材料引用{项.编码};
            else return 项;
        }, 临时); return true;
    }

    static void 写(规范编码器& 出, const 属性槽& 值) { 写(出, 值.属性类型节点); 写(出, 值.当前值); }
    static bool 读(规范解码器& 入, 属性槽& 值) { return 读(入, 值.属性类型节点) && 读(入, 值.当前值); }
    static void 写(规范编码器& 出, const 节点事实& 值) {
        写(出, 值.编码); 写枚举(出, 值.种类);
        写可选(出, 值.属性类型表示, [](auto& e, auto v){ 写枚举(e, v); });
        出.U64(值.创建事实代次); 写可选(出, 值.退出事实代次, [](auto& e, auto v){ e.U64(v); });
        写组(出, 值.当前属性, [](auto& e, const auto& v){ 写(e, v); }); 写(出, 值.写入所有者);
    }
    static bool 读(规范解码器& 入, 节点事实& 值) {
        return 读(入, 值.编码) && 读枚举(入, 值.种类, 3)
            && 读可选(入, 值.属性类型表示, [](auto& d, auto& v){ return 读枚举(d, v, 4); })
            && 入.U64(值.创建事实代次)
            && 读可选(入, 值.退出事实代次, [](auto& d, auto& v){ return d.U64(v); })
            && 读组(入, 值.当前属性, [](auto& d, auto& v){ return 读(d, v); }) && 读(入, 值.写入所有者);
    }
    static void 写(规范编码器& 出, const 关系事实& 值) {
        写(出, 值.编码); 写(出, 值.源节点); 写(出, 值.目标节点); 写(出, 值.关系类型节点);
        出.I64(值.角色或顺序); 出.U64(值.创建事实代次);
        写可选(出, 值.退出事实代次, [](auto& e, auto v){ e.U64(v); }); 写(出, 值.写入所有者);
    }
    static bool 读(规范解码器& 入, 关系事实& 值) {
        return 读(入, 值.编码) && 读(入, 值.源节点) && 读(入, 值.目标节点)
            && 读(入, 值.关系类型节点) && 入.I64(值.角色或顺序) && 入.U64(值.创建事实代次)
            && 读可选(入, 值.退出事实代次, [](auto& d, auto& v){ return d.U64(v); }) && 读(入, 值.写入所有者);
    }
    static void 写(规范编码器& 出, const 值事实& 值) {
        写(出, 值.编码); 写(出, 值.所属节点); 写(出, 值.属性类型节点); 写原始材料(出, 值.材料);
        写(出, 值.来源节点); 出.U64(值.创建事实代次);
        写可选(出, 值.退出事实代次, [](auto& e, auto v){ e.U64(v); }); 写(出, 值.写入所有者);
    }
    static bool 读(规范解码器& 入, 值事实& 值) {
        return 读(入, 值.编码) && 读(入, 值.所属节点) && 读(入, 值.属性类型节点)
            && 读原始材料(入, 值.材料) && 读(入, 值.来源节点) && 入.U64(值.创建事实代次)
            && 读可选(入, 值.退出事实代次, [](auto& d, auto& v){ return d.U64(v); }) && 读(入, 值.写入所有者);
    }
    static void 写事实副本(规范编码器& 出, const L1事实副本& 值) {
        if (const auto* 项 = std::get_if<节点事实>(&值)) { 出.U8(1); 写(出, *项); }
        else if (const auto* 项 = std::get_if<关系事实>(&值)) { 出.U8(2); 写(出, *项); }
        else { 出.U8(3); 写(出, std::get<值事实>(值)); }
    }
    static bool 读事实副本(规范解码器& 入, L1事实副本& 值) {
        std::uint8_t 标签{}; if (!入.U8(标签)) return false;
        if (标签 == 1) { 节点事实 项; if (!读(入, 项)) return false; 值 = std::move(项); return true; }
        if (标签 == 2) { 关系事实 项; if (!读(入, 项)) return false; 值 = std::move(项); return true; }
        if (标签 == 3) { 值事实 项; if (!读(入, 项)) return false; 值 = std::move(项); return true; }
        return false;
    }
    static void 写(规范编码器& 出, const L1历史事实副本& 值) { 写(出, 值.查询编码); 写事实副本(出, 值.事实); 出.布尔(值.当前有效); }
    static bool 读(规范解码器& 入, L1历史事实副本& 值) { return 读(入, 值.查询编码) && 读事实副本(入, 值.事实) && 入.布尔(值.当前有效); }
    static void 写(规范编码器& 出, const L1结构所有者事实& 值) {
        写(出, 值.所有者); 写枚举(出, 值.范围种类); 出.U64(值.创建事实代次);
        写可选(出, 值.退出事实代次, [](auto& e, auto v){ e.U64(v); });
    }
    static bool 读(规范解码器& 入, L1结构所有者事实& 值) {
        return 读(入, 值.所有者) && 读枚举(入, 值.范围种类, 2) && 入.U64(值.创建事实代次)
            && 读可选(入, 值.退出事实代次, [](auto& d, auto& v){ return d.U64(v); });
    }
    static void 写(规范编码器& 出, const L1物理清理事实身份& 值) { 写枚举(出, 值.事实种类); 写(出, 值.编码); }
    static bool 读(规范解码器& 入, L1物理清理事实身份& 值) { return 读枚举(入, 值.事实种类, 3) && 读(入, 值.编码); }
    static void 写(规范编码器& 出, const L1物理清理墓碑& 值) {
        写(出, 值.编码); 写枚举(出, 值.事实种类); 写(出, 值.内部结构分区);
        出.U64(值.创建事实代次); 出.U64(值.退出事实代次); 出.U64(值.物理清理事实代次);
    }
    static bool 读(规范解码器& 入, L1物理清理墓碑& 值) {
        return 读(入, 值.编码) && 读枚举(入, 值.事实种类, 3) && 读(入, 值.内部结构分区)
            && 入.U64(值.创建事实代次) && 入.U64(值.退出事实代次) && 入.U64(值.物理清理事实代次);
    }

    static void 写中性引用(规范编码器& 出, const L1中性事实引用& 值) {
        if (const auto* 项 = std::get_if<稳定编码>(&值)) { 出.U8(1); 写(出, *项); }
        else { 出.U8(2); 写(出, std::get<L1中性写集本地键>(值)); }
    }
    static bool 读中性引用(规范解码器& 入, L1中性事实引用& 值) {
        std::uint8_t 标签{}; if (!入.U8(标签)) return false;
        if (标签 == 1) { 稳定编码 项; if (!读(入, 项)) return false; 值 = 项; return true; }
        if (标签 == 2) { L1中性写集本地键 项; if (!读(入, 项)) return false; 值 = 项; return true; }
        return false;
    }
    static void 写(规范编码器& 出, const L1中性写集请求& 值) {
        出.U32(值.合同版本); 出.U64(值.期望事实代次); 写(出, 值.幂等键);
        写组(出, 值.节点, [](auto& e, const auto& v){ 写(e, v.本地键); 写枚举(e, v.种类); 写可选(e, v.属性类型表示, [](auto& x, auto y){ 写枚举(x, y); }); });
        写组(出, 值.关系, [](auto& e, const auto& v){ 写(e, v.本地键); 写中性引用(e, v.源节点); 写中性引用(e, v.目标节点); 写中性引用(e, v.关系类型节点); e.I64(v.角色或顺序); });
        写组(出, 值.值, [](auto& e, const auto& v){ 写(e, v.本地键); 写中性引用(e, v.所属节点); 写中性引用(e, v.属性类型节点); 写中性材料(e, v.材料); 写中性引用(e, v.来源节点); });
        写组(出, 值.属性槽变更, [](auto& e, const auto& v){ 写中性引用(e, v.所属节点); 写中性引用(e, v.属性类型节点); 写(e, v.新当前值); });
        写组(出, 值.退出事实, [](auto& e, auto v){ 写(e, v); });
    }
    static bool 读(规范解码器& 入, L1中性写集请求& 值) {
        return 入.U32(值.合同版本) && 入.U64(值.期望事实代次) && 读(入, 值.幂等键)
            && 读组(入, 值.节点, [](auto& d, auto& v){ return 读(d, v.本地键) && 读枚举(d, v.种类, 3) && 读可选(d, v.属性类型表示, [](auto& x, auto& y){ return 读枚举(x, y, 4); }); })
            && 读组(入, 值.关系, [](auto& d, auto& v){ return 读(d, v.本地键) && 读中性引用(d, v.源节点) && 读中性引用(d, v.目标节点) && 读中性引用(d, v.关系类型节点) && d.I64(v.角色或顺序); })
            && 读组(入, 值.值, [](auto& d, auto& v){ return 读(d, v.本地键) && 读中性引用(d, v.所属节点) && 读中性引用(d, v.属性类型节点) && 读中性材料(d, v.材料) && 读中性引用(d, v.来源节点); })
            && 读组(入, 值.属性槽变更, [](auto& d, auto& v){ return 读中性引用(d, v.所属节点) && 读中性引用(d, v.属性类型节点) && 读(d, v.新当前值); })
            && 读组(入, 值.退出事实, [](auto& d, auto& v){ return 读(d, v); });
    }
    static void 写(规范编码器& 出, const L1中性写入结果& 值) {
        写枚举(出, 值.状态); 出.U32(值.合同版本); 写(出, 值.幂等键); 出.U64(值.事实代次);
        出.布尔(值.是否形成内存权威发布); 写枚举(出, 值.重试边界);
        写组(出, 值.新编码映射, [](auto& e, const auto& v){ 写(e, v.first); 写(e, v.second); });
    }
    static bool 读(规范解码器& 入, L1中性写入结果& 值) {
        return 读枚举(入, 值.状态, 10) && 入.U32(值.合同版本) && 读(入, 值.幂等键)
            && 入.U64(值.事实代次) && 入.布尔(值.是否形成内存权威发布)
            && 读枚举(入, 值.重试边界, 4)
            && 读组(入, 值.新编码映射, [](auto& d, auto& v){ return 读(d, v.first) && 读(d, v.second); });
    }

    static void 写(规范编码器& 出, const L1所有者范围建立请求& 值) { 出.U32(值.合同版本); 写(出, 值.建立幂等身份); 写枚举(出, 值.范围种类); }
    static bool 读(规范解码器& 入, L1所有者范围建立请求& 值) { return 入.U32(值.合同版本) && 读(入, 值.建立幂等身份) && 读枚举(入, 值.范围种类, 2); }
    static void 写(规范编码器& 出, const L1所有者范围建立结果& 值) {
        写枚举(出, 值.状态); 出.U32(值.合同版本); 写(出, 值.建立幂等身份);
        写可选(出, 值.所有者事实, [](auto& e, const auto& v){ 写(e, v); });
        出.U64(值.事实代次); 出.布尔(值.是否形成内存权威发布); 写枚举(出, 值.重试边界);
    }
    static bool 读(规范解码器& 入, L1所有者范围建立结果& 值) {
        return 读枚举(入, 值.状态, 11) && 入.U32(值.合同版本) && 读(入, 值.建立幂等身份)
            && 读可选(入, 值.所有者事实, [](auto& d, auto& v){ return 读(d, v); })
            && 入.U64(值.事实代次) && 入.布尔(值.是否形成内存权威发布) && 读枚举(入, 值.重试边界, 4);
    }

    static void 写跨引用(规范编码器& 出, const L1跨所有者原子事实引用值& 值) {
        if (const auto* 稳定项 = std::get_if<稳定编码>(&值)) {
            出.U8(1); 写(出, *稳定项);
        } else if (const auto* 本地项 =
            std::get_if<L1所有者范围写集本地键>(&值)) {
            出.U8(2); 出.U32(本地项->值);
        } else {
            出.U8(3);
            const auto& 跨项 = std::get<L1跨所有者原子事实引用>(值);
            写枚举(出, 跨项.参与者);
            出.U32(跨项.本地键.值);
        }
    }
    static bool 读跨引用(规范解码器& 入, L1跨所有者原子事实引用值& 值) {
        std::uint8_t 标签{}; if (!入.U8(标签)) return false;
        if (标签 == 1) { 稳定编码 项; if (!读(入, 项)) return false; 值 = 项; return true; }
        if (标签 == 2) { L1所有者范围写集本地键 项; if (!入.U32(项.值)) return false; 值 = 项; return true; }
        if (标签 == 3) { L1跨所有者原子事实引用 项; if (!读枚举(入, 项.参与者, 2) || !入.U32(项.本地键.值)) return false; 值 = 项; return true; }
        return false;
    }
    static void 写(规范编码器& 出, const L1跨所有者原子写集请求& 值) {
        const auto& [合同, 代次, 幂等, 节点, 关系, 值组, 槽, 退出] = 值;
        出.U32(合同); 出.U64(代次); 写(出, 幂等);
        写组(出, 节点, [](auto& e, const auto& v){ 写(e, v.本地键); 写枚举(e, v.种类); 写可选(e, v.属性类型表示, [](auto& x, auto y){ 写枚举(x, y); }); });
        写组(出, 关系, [](auto& e, const auto& v){ 写(e, v.本地键); 写跨引用(e, v.源节点); 写跨引用(e, v.目标节点); 写跨引用(e, v.关系类型节点); e.I64(v.角色或顺序); });
        写组(出, 值组, [](auto& e, const auto& v){ 写(e, v.本地键); 写跨引用(e, v.所属节点); 写跨引用(e, v.属性类型节点); 写所有者材料(e, v.材料); 写跨引用(e, v.来源节点); });
        写组(出, 槽, [](auto& e, const auto& v){ 写跨引用(e, v.所属节点); 写跨引用(e, v.属性类型节点); 写(e, v.新当前值); });
        写组(出, 退出, [](auto& e, auto v){ 写(e, v); });
    }
    static bool 读(规范解码器& 入, L1跨所有者原子写集请求& 值) {
        std::uint32_t 合同{}; std::uint64_t 代次{}; L1所有者范围写入幂等身份 幂等;
        std::vector<L1跨所有者原子节点新建项> 节点; std::vector<L1跨所有者原子关系新建项> 关系;
        std::vector<L1跨所有者原子值新建项> 值组; std::vector<L1跨所有者原子属性槽变更项> 槽; std::vector<稳定编码> 退出;
        if (!入.U32(合同) || !入.U64(代次) || !读(入, 幂等)
            || !读组(入, 节点, [](auto& d, auto& v){ return d.U32(v.本地键.值) && 读枚举(d, v.种类, 3) && 读可选(d, v.属性类型表示, [](auto& x, auto& y){ return 读枚举(x, y, 4); }); })
            || !读组(入, 关系, [](auto& d, auto& v){ return d.U32(v.本地键.值) && 读跨引用(d, v.源节点) && 读跨引用(d, v.目标节点) && 读跨引用(d, v.关系类型节点) && d.I64(v.角色或顺序); })
            || !读组(入, 值组, [](auto& d, auto& v){ return d.U32(v.本地键.值) && 读跨引用(d, v.所属节点) && 读跨引用(d, v.属性类型节点) && 读所有者材料(d, v.材料) && 读跨引用(d, v.来源节点); })
            || !读组(入, 槽, [](auto& d, auto& v){ return 读跨引用(d, v.所属节点) && 读跨引用(d, v.属性类型节点) && d.U32(v.新当前值.值); })
            || !读组(入, 退出, [](auto& d, auto& v){ return 读(d, v); })) return false;
        值 = {合同, 代次, 幂等, std::move(节点), std::move(关系), std::move(值组), std::move(槽), std::move(退出)}; return true;
    }
    static void 写(规范编码器& 出, const L1跨所有者原子参与者写集& 值) { 写枚举(出, 值.参与者); 写(出, 值.所有者); 写(出, 值.写集); }
    static bool 读(规范解码器& 入, L1跨所有者原子参与者写集& 值) { return 读枚举(入, 值.参与者, 2) && 读(入, 值.所有者) && 读(入, 值.写集); }
    static void 写(规范编码器& 出, const L1跨所有者原子事务请求& 值) {
        出.U32(值.合同版本); 出.U64(值.共同期望事实代次); 写(出, 值.组合写入幂等身份); 写(出, 值.状态写集); 写(出, 值.动态写集);
    }
    static bool 读(规范解码器& 入, L1跨所有者原子事务请求& 值) {
        return 入.U32(值.合同版本) && 入.U64(值.共同期望事实代次) && 读(入, 值.组合写入幂等身份) && 读(入, 值.状态写集) && 读(入, 值.动态写集);
    }
    static void 写(规范编码器& 出, const L1跨所有者原子事务结果& 值) {
        写枚举(出, 值.状态); 出.U32(值.合同版本); 出.U64(值.共同事实代次); 出.布尔(值.是否形成内存权威发布); 写枚举(出, 值.重试边界);
        写组(出, 值.状态编码映射, [](auto& e, const auto& v){ e.U32(v.first.值); 写(e, v.second); });
        写组(出, 值.动态编码映射, [](auto& e, const auto& v){ e.U32(v.first.值); 写(e, v.second); });
    }
    static bool 读(规范解码器& 入, L1跨所有者原子事务结果& 值) {
        return 读枚举(入, 值.状态, 11) && 入.U32(值.合同版本) && 入.U64(值.共同事实代次)
            && 入.布尔(值.是否形成内存权威发布) && 读枚举(入, 值.重试边界, 4)
            && 读组(入, 值.状态编码映射, [](auto& d, auto& v){ return d.U32(v.first.值) && 读(d, v.second); })
            && 读组(入, 值.动态编码映射, [](auto& d, auto& v){ return d.U32(v.first.值) && 读(d, v.second); });
    }

    static void 写三分区引用_v2(规范编码器& 出,
        const L1三分区原子事实引用值_v2& 值) {
        if (const auto* 稳定项 = std::get_if<稳定编码>(&值)) {
            出.U8(1); 写(出, *稳定项);
        } else if (const auto* 本地项 =
            std::get_if<L1所有者范围写集本地键>(&值)) {
            出.U8(2); 出.U32(本地项->值);
        } else {
            出.U8(3);
            const auto& 跨项 = std::get<L1三分区原子事实引用_v2>(值);
            出.U8(跨项.参与者.值); 出.U32(跨项.本地键.值);
        }
    }
    static bool 读三分区引用_v2(规范解码器& 入,
        L1三分区原子事实引用值_v2& 值) {
        std::uint8_t 标签{};
        if (!入.U8(标签)) return false;
        if (标签 == 1) {
            稳定编码 项; if (!读(入, 项)) return false; 值 = 项; return true;
        }
        if (标签 == 2) {
            L1所有者范围写集本地键 项;
            if (!入.U32(项.值)) return false; 值 = 项; return true;
        }
        if (标签 == 3) {
            L1三分区原子事实引用_v2 项;
            if (!入.U8(项.参与者.值) || 项.参与者.值 < 1
                || 项.参与者.值 > 3 || !入.U32(项.本地键.值)) return false;
            值 = 项; return true;
        }
        return false;
    }
    static void 写(规范编码器& 出, const L1三分区原子写集请求_v2& 值) {
        出.U32(值.合同版本); 出.U64(值.期望事实代次);
        写(出, 值.写入幂等身份);
        写组(出, 值.节点, [](auto& e, const auto& v) {
            e.U32(v.本地键.值); 写枚举(e, v.种类);
            写可选(e, v.属性类型表示,
                [](auto& x, auto y) { 写枚举(x, y); });
        });
        写组(出, 值.关系, [](auto& e, const auto& v) {
            e.U32(v.本地键.值); 写三分区引用_v2(e, v.源节点);
            写三分区引用_v2(e, v.目标节点);
            写三分区引用_v2(e, v.关系类型节点); e.I64(v.角色或顺序);
        });
        写组(出, 值.值, [](auto& e, const auto& v) {
            e.U32(v.本地键.值); 写三分区引用_v2(e, v.所属节点);
            写三分区引用_v2(e, v.属性类型节点); 写所有者材料(e, v.材料);
            写三分区引用_v2(e, v.来源节点);
        });
        写组(出, 值.属性槽变更, [](auto& e, const auto& v) {
            写三分区引用_v2(e, v.所属节点);
            写三分区引用_v2(e, v.属性类型节点); e.U32(v.新当前值.值);
        });
        写组(出, 值.退出事实, [](auto& e, auto v) { 写(e, v); });
    }
    static bool 读(规范解码器& 入, L1三分区原子写集请求_v2& 值) {
        return 入.U32(值.合同版本) && 入.U64(值.期望事实代次)
            && 读(入, 值.写入幂等身份)
            && 读组(入, 值.节点, [](auto& d, auto& v) {
                return d.U32(v.本地键.值) && 读枚举(d, v.种类, 3)
                    && 读可选(d, v.属性类型表示,
                        [](auto& x, auto& y) { return 读枚举(x, y, 4); });
            })
            && 读组(入, 值.关系, [](auto& d, auto& v) {
                return d.U32(v.本地键.值)
                    && 读三分区引用_v2(d, v.源节点)
                    && 读三分区引用_v2(d, v.目标节点)
                    && 读三分区引用_v2(d, v.关系类型节点)
                    && d.I64(v.角色或顺序);
            })
            && 读组(入, 值.值, [](auto& d, auto& v) {
                return d.U32(v.本地键.值)
                    && 读三分区引用_v2(d, v.所属节点)
                    && 读三分区引用_v2(d, v.属性类型节点)
                    && 读所有者材料(d, v.材料)
                    && 读三分区引用_v2(d, v.来源节点);
            })
            && 读组(入, 值.属性槽变更, [](auto& d, auto& v) {
                return 读三分区引用_v2(d, v.所属节点)
                    && 读三分区引用_v2(d, v.属性类型节点)
                    && d.U32(v.新当前值.值);
            })
            && 读组(入, 值.退出事实,
                [](auto& d, auto& v) { return 读(d, v); });
    }
    static void 写(规范编码器& 出,
        const L1三分区原子参与者写集_v2& 值) {
        出.U8(值.参与者.值); 写(出, 值.所有者); 写(出, 值.写集);
    }
    static bool 读(规范解码器& 入,
        L1三分区原子参与者写集_v2& 值) {
        return 入.U8(值.参与者.值) && 值.参与者.值 >= 1
            && 值.参与者.值 <= 3 && 读(入, 值.所有者) && 读(入, 值.写集);
    }
    static void 写(规范编码器& 出, const L1三分区原子事务请求_v2& 值) {
        出.U32(值.合同版本); 出.U64(值.共同期望事实代次);
        写(出, 值.组合写入幂等身份);
        写组(出, 值.参与者写集组,
            [](auto& e, const auto& v) { 写(e, v); });
    }
    static bool 读(规范解码器& 入, L1三分区原子事务请求_v2& 值) {
        return 入.U32(值.合同版本) && 入.U64(值.共同期望事实代次)
            && 读(入, 值.组合写入幂等身份)
            && 读组(入, 值.参与者写集组,
                [](auto& d, auto& v) { return 读(d, v); });
    }
    static void 写(规范编码器& 出,
        const L1三分区原子参与者结果_v2& 值) {
        出.U8(值.参与者.值); 写(出, 值.所有者);
        写组(出, 值.新编码映射, [](auto& e, const auto& v) {
            e.U32(v.first.值); 写(e, v.second);
        });
    }
    static bool 读(规范解码器& 入,
        L1三分区原子参与者结果_v2& 值) {
        return 入.U8(值.参与者.值) && 值.参与者.值 >= 1
            && 值.参与者.值 <= 3 && 读(入, 值.所有者)
            && 读组(入, 值.新编码映射, [](auto& d, auto& v) {
                return d.U32(v.first.值) && 读(d, v.second);
            });
    }
    static void 写(规范编码器& 出, const L1三分区原子事务结果_v2& 值) {
        写枚举(出, 值.状态); 出.U32(值.合同版本);
        写(出, 值.组合写入幂等身份); 出.U64(值.共同事实代次);
        出.布尔(值.是否已确认形成内存权威发布); 写枚举(出, 值.重试边界);
        写组(出, 值.参与者结果组,
            [](auto& e, const auto& v) { 写(e, v); });
    }
    static bool 读(规范解码器& 入, L1三分区原子事务结果_v2& 值) {
        return 读枚举(入, 值.状态, 9) && 入.U32(值.合同版本)
            && 读(入, 值.组合写入幂等身份) && 入.U64(值.共同事实代次)
            && 入.布尔(值.是否已确认形成内存权威发布)
            && 读枚举(入, 值.重试边界, 4)
            && 读组(入, 值.参与者结果组,
                [](auto& d, auto& v) { return 读(d, v); });
    }

    static void 写有限N引用_v3(规范编码器& 出,
        const L1有限N分区原子事实引用值_v3& 值) {
        if (const auto* 稳定项 = std::get_if<稳定编码>(&值)) {
            出.U8(1); 写(出, *稳定项);
        } else if (const auto* 本地项 =
            std::get_if<L1所有者范围写集本地键>(&值)) {
            出.U8(2); 出.U32(本地项->值);
        } else {
            出.U8(3);
            const auto& 跨项 = std::get<L1有限N分区原子事实引用_v3>(值);
            出.U8(跨项.参与者.值); 出.U32(跨项.本地键.值);
        }
    }
    static bool 读有限N引用_v3(规范解码器& 入,
        L1有限N分区原子事实引用值_v3& 值) {
        std::uint8_t 标签{};
        if (!入.U8(标签)) return false;
        if (标签 == 1) {
            稳定编码 项; if (!读(入, 项)) return false; 值 = 项; return true;
        }
        if (标签 == 2) {
            L1所有者范围写集本地键 项;
            if (!入.U32(项.值)) return false; 值 = 项; return true;
        }
        if (标签 == 3) {
            L1有限N分区原子事实引用_v3 项;
            if (!入.U8(项.参与者.值) || 项.参与者.值 < 1
                || !入.U32(项.本地键.值)) return false;
            值 = 项; return true;
        }
        return false;
    }
    static void 写有限N写集_v3(规范编码器& 出,
        const L1有限N分区原子写集请求_v3& 值) {
        出.U32(值.合同版本); 出.U64(值.期望事实代次);
        写(出, 值.写入幂等身份);
        写组(出, 值.节点, [](auto& e, const auto& v) {
            e.U32(v.本地键.值); 写枚举(e, v.种类);
            写可选(e, v.属性类型表示,
                [](auto& x, auto y) { 写枚举(x, y); });
        });
        写组(出, 值.关系, [](auto& e, const auto& v) {
            e.U32(v.本地键.值); 写有限N引用_v3(e, v.源节点);
            写有限N引用_v3(e, v.目标节点);
            写有限N引用_v3(e, v.关系类型节点); e.I64(v.角色或顺序);
        });
        写组(出, 值.值, [](auto& e, const auto& v) {
            e.U32(v.本地键.值); 写有限N引用_v3(e, v.所属节点);
            写有限N引用_v3(e, v.属性类型节点); 写所有者材料(e, v.材料);
            写有限N引用_v3(e, v.来源节点);
        });
        写组(出, 值.属性槽变更, [](auto& e, const auto& v) {
            写有限N引用_v3(e, v.所属节点);
            写有限N引用_v3(e, v.属性类型节点); e.U32(v.新当前值.值);
        });
        写组(出, 值.退出事实, [](auto& e, auto v) { 写(e, v); });
    }
    static bool 读有限N写集_v3(规范解码器& 入,
        L1有限N分区原子写集请求_v3& 值) {
        return 入.U32(值.合同版本) && 入.U64(值.期望事实代次)
            && 读(入, 值.写入幂等身份)
            && 读组(入, 值.节点, [](auto& d, auto& v) {
                return d.U32(v.本地键.值) && 读枚举(d, v.种类, 3)
                    && 读可选(d, v.属性类型表示,
                        [](auto& x, auto& y) { return 读枚举(x, y, 4); });
            })
            && 读组(入, 值.关系, [](auto& d, auto& v) {
                return d.U32(v.本地键.值)
                    && 读有限N引用_v3(d, v.源节点)
                    && 读有限N引用_v3(d, v.目标节点)
                    && 读有限N引用_v3(d, v.关系类型节点)
                    && d.I64(v.角色或顺序);
            })
            && 读组(入, 值.值, [](auto& d, auto& v) {
                return d.U32(v.本地键.值)
                    && 读有限N引用_v3(d, v.所属节点)
                    && 读有限N引用_v3(d, v.属性类型节点)
                    && 读所有者材料(d, v.材料)
                    && 读有限N引用_v3(d, v.来源节点);
            })
            && 读组(入, 值.属性槽变更, [](auto& d, auto& v) {
                return 读有限N引用_v3(d, v.所属节点)
                    && 读有限N引用_v3(d, v.属性类型节点)
                    && d.U32(v.新当前值.值);
            })
            && 读组(入, 值.退出事实,
                [](auto& d, auto& v) { return 读(d, v); });
    }
    static void 写有限N参与者_v3(规范编码器& 出,
        const L1有限N分区原子参与者写集_v3& 值) {
        出.U8(值.参与者.值); 写(出, 值.所有者);
        写有限N写集_v3(出, 值.写集);
    }
    static bool 读有限N参与者_v3(规范解码器& 入,
        L1有限N分区原子参与者写集_v3& 值) {
        return 入.U8(值.参与者.值) && 值.参与者.值 >= 1
            && 读(入, 值.所有者) && 读有限N写集_v3(入, 值.写集);
    }
    static void 写有限N请求_v3(规范编码器& 出,
        const L1有限N分区原子事务请求_v3& 值) {
        出.U32(值.合同版本); 出.U64(值.共同期望事实代次);
        写(出, 值.组合写入幂等身份);
        写组(出, 值.参与者写集组,
            [](auto& e, const auto& v) { 写有限N参与者_v3(e, v); });
    }
    static bool 读有限N请求_v3(规范解码器& 入,
        L1有限N分区原子事务请求_v3& 值) {
        return 入.U32(值.合同版本) && 入.U64(值.共同期望事实代次)
            && 读(入, 值.组合写入幂等身份)
            && 读组(入, 值.参与者写集组,
                [](auto& d, auto& v) { return 读有限N参与者_v3(d, v); });
    }
    static void 写有限N参与者结果_v3(规范编码器& 出,
        const L1有限N分区原子参与者结果_v3& 值) {
        出.U8(值.参与者.值); 写(出, 值.所有者);
        写组(出, 值.新编码映射, [](auto& e, const auto& v) {
            e.U32(v.first.值); 写(e, v.second);
        });
    }
    static bool 读有限N参与者结果_v3(规范解码器& 入,
        L1有限N分区原子参与者结果_v3& 值) {
        return 入.U8(值.参与者.值) && 值.参与者.值 >= 1
            && 读(入, 值.所有者)
            && 读组(入, 值.新编码映射, [](auto& d, auto& v) {
                return d.U32(v.first.值) && 读(d, v.second);
            });
    }
    static void 写有限N结果_v3(规范编码器& 出,
        const L1有限N分区原子事务结果_v3& 值) {
        写枚举(出, 值.状态); 出.U32(值.合同版本);
        写(出, 值.组合写入幂等身份); 出.U64(值.共同事实代次);
        出.布尔(值.是否已确认形成内存权威发布); 写枚举(出, 值.重试边界);
        写组(出, 值.参与者结果组,
            [](auto& e, const auto& v) { 写有限N参与者结果_v3(e, v); });
    }
    static bool 读有限N结果_v3(规范解码器& 入,
        L1有限N分区原子事务结果_v3& 值) {
        return 读枚举(入, 值.状态, 9) && 入.U32(值.合同版本)
            && 读(入, 值.组合写入幂等身份) && 入.U64(值.共同事实代次)
            && 入.布尔(值.是否已确认形成内存权威发布)
            && 读枚举(入, 值.重试边界, 4)
            && 读组(入, 值.参与者结果组,
                [](auto& d, auto& v) { return 读有限N参与者结果_v3(d, v); });
    }

    template<class T, class 写函数>
    static void 写有序表(规范编码器& 出, const std::unordered_map<std::uint64_t, T>& 表, 写函数 写值) {
        std::vector<std::uint64_t> 键; 键.reserve(表.size()); for (const auto& [k, _] : 表) 键.push_back(k);
        std::sort(键.begin(), 键.end()); 出.U64(static_cast<std::uint64_t>(键.size()));
        for (auto k : 键) { 出.U64(k); 写值(出, 表.at(k)); }
    }
    template<class T, class 读函数>
    static bool 读有序表(规范解码器& 入, std::unordered_map<std::uint64_t, T>& 表, 读函数 读值) {
        std::uint64_t 数{}; if (!入.U64(数) || 数 > 入.字节.size() - 入.位置) return false;
        表.clear(); 表.reserve(static_cast<std::size_t>(数)); std::uint64_t 前{};
        for (std::uint64_t i=0;i!=数;++i) { std::uint64_t k{}; T v{}; if (!入.U64(k) || (i && k<=前) || !读值(入,v) || !表.emplace(k,std::move(v)).second) return false; 前=k; }
        return true;
    }
    static bool 是进程维护幂等键(std::uint64_t 键) noexcept { return (键 & 0xFFFF000000000000ULL) == 0x4E43000000000000ULL; }
    static bool 是进程维护跨所有者原子事务(
        std::uint64_t 账键,
        const 跨所有者原子事务幂等记录& 账) noexcept {
        const auto 参与者写入幂等身份 = [](const L1跨所有者原子写集请求& 写集) {
            const auto& [合同版本, 期望事实代次, 写入幂等身份,
                节点, 关系, 值, 属性槽变更, 退出事实] = 写集;
            return 写入幂等身份.值;
        };
        return 是进程维护幂等键(账键)
            || 是进程维护幂等键(账.请求.组合写入幂等身份.值)
            || 是进程维护幂等键(
                参与者写入幂等身份(账.请求.状态写集.写集))
            || 是进程维护幂等键(
                参与者写入幂等身份(账.请求.动态写集.写集));
    }
    static bool 是进程维护三分区原子事务_v2(
        std::uint64_t 账键,
        const 三分区原子事务幂等记录_v2& 账) noexcept {
        if (是进程维护幂等键(账键)
            || 是进程维护幂等键(账.请求.组合写入幂等身份.值))
            return true;
        for (const auto& 参与者 : 账.请求.参与者写集组)
            if (是进程维护幂等键(参与者.写集.写入幂等身份.值))
                return true;
        return false;
    }
    static bool 是进程维护有限N分区原子事务_v3(
        std::uint64_t 账键,
        const 有限N分区原子事务幂等记录_v3& 账) noexcept {
        if (是进程维护幂等键(账键)
            || 是进程维护幂等键(账.请求.组合写入幂等身份.值))
            return true;
        for (const auto& 参与者 : 账.请求.参与者写集组)
            if (是进程维护幂等键(参与者.写集.写入幂等身份.值))
                return true;
        return false;
    }

    static std::vector<std::uint8_t> 编码权威状态(const 状态& 值) {
        规范编码器 出; 出.U64(0x3150414E534C3148ULL); 出.U32(3); 出.U64(值.事实代次); 出.U64(值.下个编码);
        写有序表(出, 值.当前所有者, [](auto& e,const auto& v){写(e,v);}); 写有序表(出, 值.历史所有者, [](auto& e,const auto& v){写(e,v);});
        写有序表(出, 值.当前节点, [](auto& e,const auto& v){写(e,v);}); 写有序表(出, 值.当前关系, [](auto& e,const auto& v){写(e,v);});
        写有序表(出, 值.当前值, [](auto& e,const auto& v){写(e,v);}); 写有序表(出, 值.历史, [](auto& e,const auto& v){写(e,v);});
        std::vector<std::uint64_t> 占用(值.永久占用.begin(), 值.永久占用.end()); std::sort(占用.begin(),占用.end());
        写组(出, 占用, [](auto& e,auto v){e.U64(v);});
        写有序表(出, 值.物理清理墓碑, [](auto& e,const auto& v){写(e,v);});
        写有序表(出, 值.物理清理幂等账, [](auto& e,const auto& v){
            e.U32(v.首次规范请求.合同版本); e.U64(v.首次规范请求.期望事实代次); e.U64(v.首次规范请求.幂等身份.值);
            写组(e,v.首次规范请求.待清理事实身份组,[](auto& x,const auto& y){写(x,y);}); 写枚举(e,v.首次状态); e.U64(v.首次物理清理事实代次);
            写组(e,v.首次稳定编码映射,[](auto& x,const auto& y){写(x,y.first);写(x,y.second);}); e.布尔(v.已物理清理);
        });
        std::vector<std::uint64_t> 中性键; for(const auto& [k,_]:值.中性幂等账) if(!是进程维护幂等键(k)) 中性键.push_back(k); std::sort(中性键.begin(),中性键.end());
        出.U64(static_cast<std::uint64_t>(中性键.size())); for(auto k:中性键){出.U64(k);const auto& v=值.中性幂等账.at(k);写(出,v.首次规范化写集);出.U64(v.首次发布事实代次);写组(出,v.首次新编码映射,[](auto& e,const auto& x){写(e,x.first);写(e,x.second);});写(出,v.首次结果);}
        写有序表(出, 值.所有者建立幂等账, [](auto& e,const auto& v){写(e,v.首次请求);写(e,v.首次结果);});
        std::vector<std::uint64_t> owner; for(const auto& [k,_]:值.所有者范围幂等账) owner.push_back(k); std::sort(owner.begin(),owner.end()); 出.U64(owner.size());
        for(auto ok:owner){出.U64(ok);const auto& inner=值.所有者范围幂等账.at(ok);std::vector<std::uint64_t> keys;for(const auto&[k,_]:inner)if(!是进程维护幂等键(k))keys.push_back(k);std::sort(keys.begin(),keys.end());出.U64(keys.size());for(auto k:keys){出.U64(k);const auto&v=inner.at(k);写(出,v.首次规范化写集);出.U64(v.首次发布事实代次);写组(出,v.首次新编码映射,[](auto&e,const auto&x){写(e,x.first);写(e,x.second);});写(出,v.首次结果);}}
        std::vector<std::uint64_t> 跨所有者事务键;
        for (const auto& [键, 账] : 值.跨所有者原子事务幂等账)
            if (!是进程维护跨所有者原子事务(键, 账))
                跨所有者事务键.push_back(键);
        std::sort(跨所有者事务键.begin(), 跨所有者事务键.end());
        出.U64(static_cast<std::uint64_t>(跨所有者事务键.size()));
        for (const auto 键 : 跨所有者事务键) {
            出.U64(键);
            const auto& 账 = 值.跨所有者原子事务幂等账.at(键);
            写(出, 账.请求);
            写(出, 账.结果);
        }
        std::vector<std::uint64_t> 三分区事务键;
        for (const auto& [键, 账] : 值.三分区原子事务幂等账_v2)
            if (!是进程维护三分区原子事务_v2(键, 账))
                三分区事务键.push_back(键);
        std::sort(三分区事务键.begin(), 三分区事务键.end());
        出.U64(static_cast<std::uint64_t>(三分区事务键.size()));
        for (const auto 键 : 三分区事务键) {
            出.U64(键);
            const auto& 账 = 值.三分区原子事务幂等账_v2.at(键);
            写(出, 账.请求); 写(出, 账.结果);
        }
        std::vector<std::uint64_t> 有限N分区事务键;
        for (const auto& [键, 账] : 值.有限N分区原子事务幂等账_v3)
            if (!是进程维护有限N分区原子事务_v3(键, 账))
                有限N分区事务键.push_back(键);
        std::sort(有限N分区事务键.begin(), 有限N分区事务键.end());
        出.U64(static_cast<std::uint64_t>(有限N分区事务键.size()));
        for (const auto 键 : 有限N分区事务键) {
            出.U64(键);
            const auto& 账 = 值.有限N分区原子事务幂等账_v3.at(键);
            写有限N请求_v3(出, 账.请求);
            写有限N结果_v3(出, 账.结果);
        }
        写可选(出, 值.旧共享所有者定位, [](auto& e,auto v){写(e,v);}); return std::move(出.字节);
    }

    static bool 解码权威状态(const std::vector<std::uint8_t>& 字节, 状态& 值) {
        规范解码器 入{字节}; std::uint64_t magic{}; std::uint32_t 版本{};
        if(!入.U64(magic)||magic!=0x3150414E534C3148ULL||!入.U32(版本)
            ||(版本!=1&&版本!=2&&版本!=3)||!入.U64(值.事实代次)||!入.U64(值.下个编码))return false;
        if(!读有序表(入,值.当前所有者,[](auto&d,auto&v){return 读(d,v);})||!读有序表(入,值.历史所有者,[](auto&d,auto&v){return 读(d,v);})
            ||!读有序表(入,值.当前节点,[](auto&d,auto&v){return 读(d,v);})||!读有序表(入,值.当前关系,[](auto&d,auto&v){return 读(d,v);})
            ||!读有序表(入,值.当前值,[](auto&d,auto&v){return 读(d,v);})||!读有序表(入,值.历史,[](auto&d,auto&v){return 读(d,v);}))return false;
        std::vector<std::uint64_t> 占用;if(!读组(入,占用,[](auto&d,auto&v){return d.U64(v);}))return false;std::uint64_t 前{};for(auto k:占用){if(!k||k<=前||!值.永久占用.insert(k).second)return false;前=k;}
        if(!读有序表(入,值.物理清理墓碑,[](auto&d,auto&v){return 读(d,v);}))return false;
        if(!读有序表(入,值.物理清理幂等账,[](auto&d,auto&v){
            if(!d.U32(v.首次规范请求.合同版本)||!d.U64(v.首次规范请求.期望事实代次)||!d.U64(v.首次规范请求.幂等身份.值)
                ||!读组(d,v.首次规范请求.待清理事实身份组,[](auto&x,auto&y){return 读(x,y);})||!读枚举(d,v.首次状态,10)||!d.U64(v.首次物理清理事实代次)
                ||!读组(d,v.首次稳定编码映射,[](auto&x,auto&y){return 读(x,y.first)&&读(x,y.second);})||!d.布尔(v.已物理清理))return false;return true;}))return false;
        std::uint64_t 数{};if(!入.U64(数)||数>入.字节.size()-入.位置)return false;for(std::uint64_t i=0,prev=0;i!=数;++i){std::uint64_t k{};中性幂等记录 v;if(!入.U64(k)||(i&&k<=prev)||是进程维护幂等键(k)||!读(入,v.首次规范化写集)||!入.U64(v.首次发布事实代次)||!读组(入,v.首次新编码映射,[](auto&d,auto&x){return 读(d,x.first)&&读(d,x.second);})||!读(入,v.首次结果)||!值.中性幂等账.emplace(k,std::move(v)).second)return false;prev=k;}
        if(!读有序表(入,值.所有者建立幂等账,[](auto&d,auto&v){return 读(d,v.首次请求)&&读(d,v.首次结果);}))return false;
        if(!入.U64(数)||数>入.字节.size()-入.位置)return false;for(std::uint64_t i=0,prev=0;i!=数;++i){std::uint64_t ok{},inner{};if(!入.U64(ok)||(i&&ok<=prev)||!入.U64(inner)||inner>入.字节.size()-入.位置)return false;auto&表=值.所有者范围幂等账[ok];for(std::uint64_t j=0,p=0;j!=inner;++j){std::uint64_t k{};所有者范围幂等记录 v;if(!入.U64(k)||(j&&k<=p)||是进程维护幂等键(k)||!读(入,v.首次规范化写集)||!入.U64(v.首次发布事实代次)||!读组(入,v.首次新编码映射,[](auto&d,auto&x){return 读(d,x.first)&&读(d,x.second);})||!读(入,v.首次结果)||!表.emplace(k,std::move(v)).second)return false;p=k;}prev=ok;}
        if (!入.U64(数) || 数 > 入.字节.size() - 入.位置) return false;
        std::uint64_t 前一事务键 = 0;
        for (std::uint64_t i = 0; i != 数; ++i) {
            std::uint64_t 键{};
            跨所有者原子事务幂等记录 账;
            if (!入.U64(键) || (i != 0 && 键 <= 前一事务键)
                || !读(入, 账.请求) || !读(入, 账.结果)
                || 是进程维护跨所有者原子事务(键, 账)
                || !值.跨所有者原子事务幂等账.emplace(
                    键, std::move(账)).second)
                return false;
            前一事务键 = 键;
        }
        if (版本 >= 2) {
            if (!入.U64(数) || 数 > 入.字节.size() - 入.位置) return false;
            std::uint64_t 前一三分区键 = 0;
            for (std::uint64_t i = 0; i != 数; ++i) {
                std::uint64_t 键{};
                三分区原子事务幂等记录_v2 账;
                if (!入.U64(键) || (i != 0 && 键 <= 前一三分区键)
                    || !读(入, 账.请求) || !读(入, 账.结果)
                    || 是进程维护三分区原子事务_v2(键, 账)
                    || !值.三分区原子事务幂等账_v2.emplace(
                        键, std::move(账)).second)
                    return false;
                前一三分区键 = 键;
            }
        }
        if (版本 == 3) {
            if (!入.U64(数) || 数 > 入.字节.size() - 入.位置) return false;
            std::uint64_t 前一有限N分区键 = 0;
            for (std::uint64_t i = 0; i != 数; ++i) {
                std::uint64_t 键{};
                有限N分区原子事务幂等记录_v3 账;
                if (!入.U64(键) || (i != 0 && 键 <= 前一有限N分区键)
                    || !读有限N请求_v3(入, 账.请求)
                    || !读有限N结果_v3(入, 账.结果)
                    || 是进程维护有限N分区原子事务_v3(键, 账)
                    || !值.有限N分区原子事务幂等账_v3.emplace(
                        键, std::move(账)).second)
                    return false;
                前一有限N分区键 = 键;
            }
        }
        if (!读可选(入,值.旧共享所有者定位,
                [](auto&d,auto&v){return 读(d,v);}) || !入.完结())
            return false;
        return 重建恢复索引(值);
    }

    static bool 重建恢复索引(状态& 值) {
        值.当前源关系索引.clear(); 值.当前目标关系索引.clear(); 值.当前关系类型索引.clear();
        值.历史源关系候选索引.clear(); 值.历史目标关系候选索引.clear(); 值.历史属性槽值候选索引.clear(); 值.当前节点引用索引.clear();
        for(const auto&[_,r]:值.当前关系)if(!插入当前源关系索引(值,r)||!插入当前目标关系索引(值,r)||!插入当前关系类型索引(值,r)||!插入历史源关系候选索引(值,r)||!插入历史目标关系候选索引(值,r)||!插入当前节点引用索引(值,r))return false;
        for(const auto&[_,v]:值.当前值)if(!插入历史属性槽值候选索引(值,v)||!插入当前节点引用索引(值,v))return false;
        for(const auto&[_,h]:值.历史)if(!std::visit([&](const auto&f){using T=std::decay_t<decltype(f)>;if constexpr(std::is_same_v<T,关系事实>)return 插入历史源关系候选索引(值,f)&&插入历史目标关系候选索引(值,f);else if constexpr(std::is_same_v<T,值事实>)return 插入历史属性槽值候选索引(值,f);else return true;},h.事实))return false;
        return true;
    }

    static bool SHA256(const std::vector<std::uint8_t>& 数据, std::array<std::uint8_t,32>& 摘要) noexcept {
        BCRYPT_ALG_HANDLE 算法{}; BCRYPT_HASH_HANDLE 哈希{}; std::vector<std::uint8_t> 对象; DWORD 长度{},已取{}; bool 成功=false;
        if (摘要.size() > (std::numeric_limits<ULONG>::max)()) return false;
        const auto 摘要长度 = static_cast<ULONG>(摘要.size());
        if(BCryptOpenAlgorithmProvider(&算法,BCRYPT_SHA256_ALGORITHM,nullptr,0)<0)return false;
        if(BCryptGetProperty(算法,BCRYPT_OBJECT_LENGTH,reinterpret_cast<PUCHAR>(&长度),sizeof(长度),&已取,0)>=0){try{对象.resize(长度);}catch(...){BCryptCloseAlgorithmProvider(算法,0);return false;}
            if(BCryptCreateHash(算法,&哈希,对象.data(),长度,nullptr,0,0)>=0){const auto* p=数据.data();std::size_t n=数据.size();NTSTATUS s=0;while(n&&s>=0){const auto 块=static_cast<ULONG>((std::min)(n,static_cast<std::size_t>((std::numeric_limits<ULONG>::max)())));s=BCryptHashData(哈希,const_cast<PUCHAR>(p),块,0);p+=块;n-=块;}成功=s>=0&&BCryptFinishHash(哈希,摘要.data(),摘要长度,0)>=0;}}
        if(哈希)BCryptDestroyHash(哈希);BCryptCloseAlgorithmProvider(算法,0);return 成功;
    }
    static bool 读文件(const std::filesystem::path& 路径,std::vector<std::uint8_t>& 数据) noexcept {
        HANDLE h=CreateFileW(路径.c_str(),GENERIC_READ,FILE_SHARE_READ,nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr);if(h==INVALID_HANDLE_VALUE)return false;LARGE_INTEGER s{};bool ok=GetFileSizeEx(h,&s)&&s.QuadPart>=0&&s.QuadPart<=static_cast<LONGLONG>(std::numeric_limits<std::uint32_t>::max());if(ok)try{数据.resize(static_cast<std::size_t>(s.QuadPart));}catch(...){ok=false;}DWORD got{};std::size_t off=0;while(ok&&off<数据.size()){DWORD want=static_cast<DWORD>((std::min)(数据.size()-off,static_cast<std::size_t>(1u<<30)));if(!ReadFile(h,数据.data()+off,want,&got,nullptr)||got!=want)ok=false;off+=got;}CloseHandle(h);return ok;
    }
    static bool 写文件并刷新(const std::filesystem::path& 路径,const std::vector<std::uint8_t>& 数据) noexcept {
        HANDLE h=CreateFileW(路径.c_str(),GENERIC_WRITE,0,nullptr,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,nullptr);if(h==INVALID_HANDLE_VALUE)return false;bool ok=true;std::size_t off=0;while(ok&&off<数据.size()){DWORD done{},want=static_cast<DWORD>((std::min)(数据.size()-off,static_cast<std::size_t>(1u<<30)));if(!WriteFile(h,数据.data()+off,want,&done,nullptr)||done!=want)ok=false;off+=done;}if(ok)ok=FlushFileBuffers(h)!=0;CloseHandle(h);return ok;
    }
    static std::filesystem::path 槽路径(const 持久会话& 会话,std::uint8_t 槽){return 会话.根/(槽==1?L"snapshot-a.bin":L"snapshot-b.bin");}
    static std::vector<std::uint8_t> 编码清单(std::uint64_t 序号,std::uint64_t 代次,std::uint8_t 槽,std::uint64_t 长度,const std::array<std::uint8_t,32>& 摘要){规范编码器 e;e.U64(0x31464E414D314C48ULL);e.U32(1);e.U64(序号);e.U64(代次);e.U8(槽);e.U64(长度);for(auto b:摘要)e.U8(b);return std::move(e.字节);}
    static bool 解码清单(const std::vector<std::uint8_t>& 数据,std::uint64_t&序号,std::uint64_t&代次,std::uint8_t&槽,std::uint64_t&长度,std::array<std::uint8_t,32>&摘要){规范解码器 d{数据};std::uint64_t m{};std::uint32_t v{};if(!d.U64(m)||m!=0x31464E414D314C48ULL||!d.U32(v)||v!=1||!d.U64(序号)||序号==0||!d.U64(代次)||代次==0||!d.U8(槽)||(槽!=1&&槽!=2)||!d.U64(长度)||长度==0)return false;for(auto&b:摘要)if(!d.U8(b))return false;return d.完结();}

    持久准备状态 准备持久发布(const 状态& 候选) noexcept {
        if(!持久会话_)return 持久准备状态::成功;
        if(持久会话_->已毒化){状态_.隔离=true;return 持久准备状态::持久证据未知;}
        try{if(!状态完整(候选))return 持久准备状态::内部不一致;auto 载荷=编码权威状态(候选);std::array<std::uint8_t,32>摘要{};if(!SHA256(载荷,摘要))return 持久准备状态::资源失败;
            const std::uint8_t 新槽=持久会话_->活动槽==1?2:1;const auto 槽临时=持久会话_->根/L"snapshot.tmp";if(!写文件并刷新(槽临时,载荷))return 持久准备状态::资源失败;
            if(!MoveFileExW(槽临时.c_str(),槽路径(*持久会话_,新槽).c_str(),MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH)){持久会话_->已毒化=true;状态_.隔离=true;return 持久准备状态::持久证据未知;}
            const auto 新序号=持久会话_->快照序号+1;if(!新序号){持久会话_->已毒化=true;return 持久准备状态::内部不一致;}auto 清单=编码清单(新序号,候选.事实代次,新槽,载荷.size(),摘要);const auto 清单临时=持久会话_->根/L"manifest.tmp";
            if(!写文件并刷新(清单临时,清单))return 持久准备状态::资源失败;if(!MoveFileExW(清单临时.c_str(),(持久会话_->根/L"manifest.bin").c_str(),MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH)){持久会话_->已毒化=true;状态_.隔离=true;return 持久准备状态::持久证据未知;}
            持久会话_->快照序号=新序号;持久会话_->活动槽=新槽;return 持久准备状态::成功;
        }catch(const std::bad_alloc&){return 持久准备状态::资源失败;}catch(const std::filesystem::filesystem_error&){return 持久准备状态::资源失败;}catch(...){return 持久准备状态::内部不一致;}
    }

    L1事实基座核心持久恢复结果 初始化持久恢复实现(const std::filesystem::path& 根) noexcept {
        const auto 失败=[](auto s){return L1事实基座核心持久恢复结果{s,std::nullopt};};
        if(根.empty()||!根.is_absolute()||根!=根.lexically_normal())return 失败(L1事实基座核心持久恢复状态::入口拒绝);
        try{std::unique_lock<std::shared_mutex> 锁(锁_);if(持久会话_||状态_.事实代次!=0||状态_.下个编码!=1||!状态_.当前所有者.empty()||!状态_.历史所有者.empty()||!状态_.当前节点.empty()||!状态_.当前关系.empty()||!状态_.当前值.empty()||!状态_.历史.empty())return 失败(L1事实基座核心持久恢复状态::内部不一致);
            std::error_code ec;std::filesystem::create_directories(根,ec);if(ec)return 失败(L1事实基座核心持久恢复状态::资源失败);auto 会话=std::make_unique<持久会话>();会话->根=根;会话->锁句柄=CreateFileW((根/L"session.lock").c_str(),GENERIC_READ|GENERIC_WRITE,0,nullptr,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,nullptr);if(会话->锁句柄==INVALID_HANDLE_VALUE)return 失败(GetLastError()==ERROR_SHARING_VIOLATION?L1事实基座核心持久恢复状态::存储占用:L1事实基座核心持久恢复状态::资源失败);
            const auto 清单路径=根/L"manifest.bin";const auto 槽A=根/L"snapshot-a.bin";const auto 槽B=根/L"snapshot-b.bin";const bool 有清单=std::filesystem::exists(清单路径,ec);if(ec)return 失败(L1事实基座核心持久恢复状态::资源失败);const bool 有A=std::filesystem::exists(槽A,ec);if(ec)return 失败(L1事实基座核心持久恢复状态::资源失败);const bool 有B=std::filesystem::exists(槽B,ec);if(ec)return 失败(L1事实基座核心持久恢复状态::资源失败);
            if(!有清单){if(有A||有B)return 失败(L1事实基座核心持久恢复状态::材料不完整);持久会话_=std::move(会话);return 失败(L1事实基座核心持久恢复状态::已建立空仓);}
            std::vector<std::uint8_t> 清单; if(!读文件(清单路径,清单))return 失败(L1事实基座核心持久恢复状态::资源失败);std::uint64_t 序号{},代次{},长度{};std::uint8_t 活动槽{};std::array<std::uint8_t,32>摘要{};if(!解码清单(清单,序号,代次,活动槽,长度,摘要))return 失败(L1事实基座核心持久恢复状态::格式不支持);
            const auto 活动路径=活动槽==1?槽A:槽B;if(!std::filesystem::exists(活动路径,ec)||ec)return 失败(ec?L1事实基座核心持久恢复状态::资源失败:L1事实基座核心持久恢复状态::材料不完整);std::vector<std::uint8_t>载荷;if(!读文件(活动路径,载荷))return 失败(L1事实基座核心持久恢复状态::资源失败);if(载荷.size()!=长度)return 失败(L1事实基座核心持久恢复状态::摘要不一致);std::array<std::uint8_t,32>实际{};if(!SHA256(载荷,实际))return 失败(L1事实基座核心持久恢复状态::资源失败);if(实际!=摘要)return 失败(L1事实基座核心持久恢复状态::摘要不一致);
            状态 候选;if(!解码权威状态(载荷,候选))return 失败(L1事实基座核心持久恢复状态::编码或所有者冲突);if(候选.事实代次!=代次)return 失败(L1事实基座核心持久恢复状态::事实代次漂移);if(!状态完整(候选))return 失败(L1事实基座核心持久恢复状态::内部不一致);会话->快照序号=序号;会话->活动槽=活动槽;std::swap(状态_,候选);持久会话_=std::move(会话);return {L1事实基座核心持久恢复状态::已恢复,L1事实基座核心持久恢复见证{1,序号,代次,摘要}};
        }catch(const std::bad_alloc&){return 失败(L1事实基座核心持久恢复状态::资源失败);}catch(const std::filesystem::filesystem_error&){return 失败(L1事实基座核心持久恢复状态::资源失败);}catch(...){return 失败(L1事实基座核心持久恢复状态::内部不一致);}
    }

    // 诊断责任：向上送出；分配异常由首次写入材料读取入口统一映射。
    static L1所有者范围事实引用 转换所有者范围写集引用(
        const L1中性事实引用& 引用) {
        return std::visit([](const auto& 值) -> L1所有者范围事实引用 {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, 稳定编码>) return 值;
            else return L1所有者范围写集本地键{值.值};
        }, 引用);
    }

    // 诊断责任：无适用错误分支；纯值表示种类映射。
    static std::optional<L1所有者范围值表示种类> 转换所有者范围写集表示(
        std::optional<L1中性值表示种类> 表示) noexcept {
        if (!表示) return std::nullopt;
        return static_cast<L1所有者范围值表示种类>(
            static_cast<std::uint8_t>(*表示));
    }

    // 诊断责任：向上送出；分配异常由首次写入材料读取入口统一映射。
    static L1所有者范围原始值材料 转换所有者范围写集材料(
        const L1中性原始值材料& 材料) {
        return std::visit([](const auto& 值) -> L1所有者范围原始值材料 {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, L1中性独立材料引用>)
                return L1所有者范围独立材料引用{值.编码};
            else return 值;
        }, 材料);
    }

    // 诊断责任：向上送出；只做私有规范化写集到公共同构 DTO 的逐字段转换。
    static L1所有者范围写集请求 转换所有者范围写集(
        const L1中性写集请求& 请求) {
        L1所有者范围写集请求 结果;
        结果.期望事实代次 = 请求.期望事实代次;
        结果.写入幂等身份 = {请求.幂等键.值};
        结果.节点.reserve(请求.节点.size());
        for (const auto& 项 : 请求.节点)
            结果.节点.push_back({{项.本地键.值}, 项.种类,
                转换所有者范围写集表示(项.属性类型表示)});
        结果.关系.reserve(请求.关系.size());
        for (const auto& 项 : 请求.关系)
            结果.关系.push_back({{项.本地键.值},
                转换所有者范围写集引用(项.源节点),
                转换所有者范围写集引用(项.目标节点),
                转换所有者范围写集引用(项.关系类型节点), 项.角色或顺序});
        结果.值.reserve(请求.值.size());
        for (const auto& 项 : 请求.值)
            结果.值.push_back({{项.本地键.值},
                转换所有者范围写集引用(项.所属节点),
                转换所有者范围写集引用(项.属性类型节点),
                转换所有者范围写集材料(项.材料),
                转换所有者范围写集引用(项.来源节点)});
        结果.属性槽变更.reserve(请求.属性槽变更.size());
        for (const auto& 项 : 请求.属性槽变更)
            结果.属性槽变更.push_back({
                转换所有者范围写集引用(项.所属节点),
                转换所有者范围写集引用(项.属性类型节点),
                {项.新当前值.值}});
        结果.退出事实 = 请求.退出事实;
        return 结果;
    }

    // 诊断责任：向上送出；只做私有首次成功结果到公共同构 DTO 的逐字段转换。
    static L1所有者范围写入结果 转换所有者范围写入结果(
        L1结构所有者身份 所有者, const L1中性写入结果& 内部) {
        const auto 映射状态 = [](L1中性写入状态 状态) noexcept {
            switch (状态) {
            case L1中性写入状态::成功: return L1所有者范围写入状态::成功;
            case L1中性写入状态::精确重复: return L1所有者范围写入状态::精确重复;
            case L1中性写入状态::入口拒绝: return L1所有者范围写入状态::入口拒绝;
            case L1中性写入状态::未找到: return L1所有者范围写入状态::未找到;
            case L1中性写入状态::已退出: return L1所有者范围写入状态::已退出;
            case L1中性写入状态::事实代次漂移:
                return L1所有者范围写入状态::事实代次漂移;
            case L1中性写入状态::幂等冲突: return L1所有者范围写入状态::幂等冲突;
            case L1中性写入状态::引用冲突: return L1所有者范围写入状态::引用冲突;
            case L1中性写入状态::资源失败: return L1所有者范围写入状态::资源失败;
            case L1中性写入状态::内部不一致:
                return L1所有者范围写入状态::内部不一致;
            }
            return L1所有者范围写入状态::内部不一致;
        };
        L1所有者范围写入结果 结果{映射状态(内部.状态),
            L1所有者范围CRUD合同版本, 所有者, {内部.幂等键.值},
            内部.事实代次, 内部.是否形成内存权威发布,
            static_cast<L1所有者范围重试边界>(
                static_cast<std::uint8_t>(内部.重试边界)), {}};
        结果.新编码映射.reserve(内部.新编码映射.size());
        for (const auto& [键, 编码] : 内部.新编码映射)
            结果.新编码映射.push_back({{键.值}, 编码});
        return 结果;
    }

    // 诊断责任：向上送出；局部互证所选 owner 幂等记录，不扫描其它 owner 或账目。
    static bool 所有者范围首次记录完整(const 状态& 值,
        L1结构所有者身份 所有者, L1所有者范围写入幂等身份 幂等身份,
        const 所有者范围幂等记录& 记录) {
        if (!有效(所有者) || !有效(幂等身份)
            || 记录.首次规范化写集.合同版本 != L1中性CRUD合同版本
            || 记录.首次规范化写集.幂等键.值 != 幂等身份.值
            || 记录.首次发布事实代次 == 0
            || 记录.首次发布事实代次 > 值.事实代次
            || 记录.首次规范化写集.期望事实代次
                != 记录.首次发布事实代次 - 1
            || 记录.首次结果.状态 != L1中性写入状态::成功
            || 记录.首次结果.合同版本 != L1中性CRUD合同版本
            || 记录.首次结果.幂等键.值 != 幂等身份.值
            || 记录.首次结果.事实代次 != 记录.首次发布事实代次
            || !记录.首次结果.是否形成内存权威发布
            || 记录.首次结果.重试边界 != L1中性重试边界::不适用
            || 记录.首次结果.新编码映射 != 记录.首次新编码映射)
            return false;
        const auto 再规范化 = 规范化中性写集(记录.首次规范化写集);
        if (!再规范化 || *再规范化 != 记录.首次规范化写集) return false;
        std::unordered_set<std::uint32_t> 本地键;
        if (!中性请求结构有效(记录.首次规范化写集, 本地键)
            || 记录.首次新编码映射.size() != 本地键.size())
            return false;
        for (std::size_t 索引 = 0; 索引 < 记录.首次新编码映射.size(); ++索引) {
            const auto& [本地, 编码] = 记录.首次新编码映射[索引];
            if (!本地键.contains(本地.值) || !有效(编码)
                || (索引 != 0 && !(记录.首次新编码映射[索引 - 1].first < 本地)))
                return false;
            std::size_t 命中数 = 0;
            const auto 匹配 = [&](const auto& 事实) {
                return 事实.编码 == 编码 && 事实.写入所有者 == 所有者
                    && 事实.创建事实代次 == 记录.首次发布事实代次;
            };
            if (const auto 当前 = 值.当前节点.find(编码.值);
                当前 != 值.当前节点.end()) {
                if (!匹配(当前->second)) return false;
                ++命中数;
            }
            if (const auto 当前 = 值.当前关系.find(编码.值);
                当前 != 值.当前关系.end()) {
                if (!匹配(当前->second)) return false;
                ++命中数;
            }
            if (const auto 当前 = 值.当前值.find(编码.值);
                当前 != 值.当前值.end()) {
                if (!匹配(当前->second)) return false;
                ++命中数;
            }
            if (const auto 历史 = 值.历史.find(编码.值);
                历史 != 值.历史.end()) {
                if (!std::visit(匹配, 历史->second.事实)) return false;
                ++命中数;
            }
            if (命中数 != 1) return false;
        }
        return true;
    }

    static std::optional<稳定编码> 分配稳定编码(状态& 值) noexcept {
        while (值.下个编码 != 0 && 值.永久占用.contains(值.下个编码))
            ++值.下个编码;
        if (值.下个编码 == 0) return std::nullopt;
        return 稳定编码{值.下个编码++};
    }

    // 诊断责任：向上送出；临时唯一性集合分配异常由公开仓库入口映射。
    static bool 一致当前请求有效(const L1中性一致当前读取请求& 请求) {
        if (请求.合同版本 != L1中性一致当前读取合同版本
            || 请求.期望事实代次 == 0
            || (请求.节点.empty() && 请求.关系.empty() && 请求.值.empty()
                && 请求.属性值.empty() && 请求.源关系组.empty()
                && 请求.目标关系组.empty())) return false;

        std::unordered_set<std::uint64_t> 具名编码;
        const auto 登记编码 = [&](稳定编码 编码) {
            return 有效(编码) && 具名编码.insert(编码.值).second;
        };
        for (const auto 编码 : 请求.节点) if (!登记编码(编码)) return false;
        for (const auto 编码 : 请求.关系) if (!登记编码(编码)) return false;
        for (const auto 编码 : 请求.值) if (!登记编码(编码)) return false;

        std::vector<std::pair<std::uint64_t, std::uint64_t>> 属性键;
        std::vector<std::pair<std::uint64_t, std::uint64_t>> 源关系键;
        std::vector<std::pair<std::uint64_t, std::uint64_t>> 目标关系键;
        属性键.reserve(请求.属性值.size());
        源关系键.reserve(请求.源关系组.size());
        目标关系键.reserve(请求.目标关系组.size());
        const auto 登记组合 = [](auto& 组, 稳定编码 左, 稳定编码 右) {
            if (!有效(左) || !有效(右)) return false;
            const auto 键 = std::pair{左.值, 右.值};
            if (std::find(组.begin(), 组.end(), 键) != 组.end()) return false;
            组.push_back(键);
            return true;
        };
        for (const auto& 项 : 请求.属性值)
            if (!登记组合(属性键, 项.节点, 项.属性类型)) return false;
        for (const auto& 项 : 请求.源关系组)
            if (!登记组合(源关系键, 项.源节点, 项.关系类型节点)) return false;
        for (const auto& 项 : 请求.目标关系组)
            if (!登记组合(目标关系键, 项.目标节点, 项.关系类型节点)) return false;
        return true;
    }

    // 诊断责任：无适用错误分支；只检查一个编码在当前或历史账中的唯一占用。
    static bool 一致编码唯一当前(const 状态& 值, std::uint64_t 编码,
        一致当前事实种类 种类) noexcept {
        const std::size_t 当前数量 = (值.当前节点.contains(编码) ? 1U : 0U)
            + (值.当前关系.contains(编码) ? 1U : 0U)
            + (值.当前值.contains(编码) ? 1U : 0U);
        const bool 期望存在 = 种类 == 一致当前事实种类::节点
            ? 值.当前节点.contains(编码)
            : 种类 == 一致当前事实种类::关系
                ? 值.当前关系.contains(编码) : 值.当前值.contains(编码);
        return 当前数量 == 1 && 期望存在 && !值.历史.contains(编码);
    }

    // 诊断责任：无适用错误分支；只核对当前节点自身生命周期与结构种类。
    static bool 一致当前节点基本完整(const 状态& 值, std::uint64_t 键,
        const 节点事实& 事实) noexcept {
        const bool 种类和表示有效 = 事实.种类 == 节点种类::普通
            ? !事实.属性类型表示
            : 事实.种类 == 节点种类::属性类型 && 事实.属性类型表示
                && (*事实.属性类型表示 == 值表示种类::I64
                    || *事实.属性类型表示 == 值表示种类::I64组
                    || *事实.属性类型表示 == 值表示种类::U64组
                    || *事实.属性类型表示 == 值表示种类::独立材料引用);
        return 键 != 0 && 事实.编码.值 == 键 && 有效(事实.编码)
            && 事实.创建事实代次 != 0 && 事实.创建事实代次 <= 值.事实代次
            && !事实.退出事实代次 && 种类和表示有效;
    }

    // 诊断责任：无适用错误分支；只核对具名值及其直接节点和类型引用。
    static bool 一致当前值局部完整(const 状态& 值, std::uint64_t 键,
        const 值事实& 事实) noexcept {
        if (!一致编码唯一当前(值, 键, 一致当前事实种类::值)
            || 事实.编码.值 != 键 || 事实.创建事实代次 == 0
            || 事实.创建事实代次 > 值.事实代次 || 事实.退出事实代次
            || !有效(事实.所属节点) || !有效(事实.属性类型节点)
            || !有效(事实.来源节点)) return false;
        const auto 所属 = 值.当前节点.find(事实.所属节点.值);
        const auto 类型 = 值.当前节点.find(事实.属性类型节点.值);
        const auto 来源 = 值.当前节点.find(事实.来源节点.值);
        if (所属 == 值.当前节点.end() || 类型 == 值.当前节点.end()
            || 来源 == 值.当前节点.end()
            || !一致编码唯一当前(值, 所属->first, 一致当前事实种类::节点)
            || !一致编码唯一当前(值, 类型->first, 一致当前事实种类::节点)
            || !一致编码唯一当前(值, 来源->first, 一致当前事实种类::节点)
            || !一致当前节点基本完整(值, 所属->first, 所属->second)
            || !一致当前节点基本完整(值, 类型->first, 类型->second)
            || !一致当前节点基本完整(值, 来源->first, 来源->second)
            || 类型->second.种类 != 节点种类::属性类型
            || !类型->second.属性类型表示
            || !表示匹配(*类型->second.属性类型表示, 事实.材料)) return false;
        if (const auto* 引用 = std::get_if<独立材料引用>(&事实.材料)) {
            const auto 材料节点 = 值.当前节点.find(引用->编码.值);
            if (!有效(引用->编码) || 材料节点 == 值.当前节点.end()
                || !一致编码唯一当前(值, 材料节点->first,
                    一致当前事实种类::节点)
                || !一致当前节点基本完整(值, 材料节点->first,
                    材料节点->second)) return false;
        }
        return true;
    }

    // 诊断责任：无适用错误分支；只核对具名节点和其直接属性槽闭包。
    static bool 一致当前节点局部完整(const 状态& 值, std::uint64_t 键,
        const 节点事实& 事实) noexcept {
        if (!一致编码唯一当前(值, 键, 一致当前事实种类::节点)
            || !一致当前节点基本完整(值, 键, 事实)) return false;
        std::uint64_t 前一属性类型 = 0;
        for (const auto& 槽 : 事实.当前属性) {
            if (!有效(槽.属性类型节点) || !有效(槽.当前值)
                || 槽.属性类型节点.值 <= 前一属性类型) return false;
            const auto 类型 = 值.当前节点.find(槽.属性类型节点.值);
            const auto 当前值 = 值.当前值.find(槽.当前值.值);
            if (类型 == 值.当前节点.end() || 当前值 == 值.当前值.end()
                || 类型->second.种类 != 节点种类::属性类型
                || !类型->second.属性类型表示
                || !一致当前节点基本完整(值, 类型->first, 类型->second)
                || !一致编码唯一当前(值, 类型->first,
                    一致当前事实种类::节点)
                || !一致当前值局部完整(值, 当前值->first, 当前值->second)
                || 当前值->second.所属节点 != 事实.编码
                || 当前值->second.属性类型节点 != 槽.属性类型节点
                || !表示匹配(*类型->second.属性类型表示,
                    当前值->second.材料)) return false;
            前一属性类型 = 槽.属性类型节点.值;
        }
        return true;
    }

    // 诊断责任：无适用错误分支；只核对具名关系及直接端点和类型节点。
    static bool 一致当前关系局部完整(const 状态& 值, std::uint64_t 键,
        const 关系事实& 事实) noexcept {
        if (!一致编码唯一当前(值, 键, 一致当前事实种类::关系)
            || 事实.编码.值 != 键 || 事实.创建事实代次 == 0
            || 事实.创建事实代次 > 值.事实代次 || 事实.退出事实代次
            || !有效(事实.源节点) || !有效(事实.目标节点)
            || !有效(事实.关系类型节点)) return false;
        const auto 源 = 值.当前节点.find(事实.源节点.值);
        const auto 目标 = 值.当前节点.find(事实.目标节点.值);
        const auto 类型 = 值.当前节点.find(事实.关系类型节点.值);
        return 源 != 值.当前节点.end() && 目标 != 值.当前节点.end()
            && 类型 != 值.当前节点.end()
            && 一致编码唯一当前(值, 源->first, 一致当前事实种类::节点)
            && 一致编码唯一当前(值, 目标->first, 一致当前事实种类::节点)
            && 一致编码唯一当前(值, 类型->first, 一致当前事实种类::节点)
            && 一致当前节点基本完整(值, 源->first, 源->second)
            && 一致当前节点基本完整(值, 目标->first, 目标->second)
            && 一致当前节点基本完整(值, 类型->first, 类型->second)
            && 类型->second.种类 == 节点种类::普通
            && !类型->second.属性类型表示;
    }

    // 诊断责任：无适用错误分支；只按单编码历史记录判定退出或种类不匹配。
    static std::optional<L1中性一致当前读取项目状态> 一致当前缺失状态(
        const 状态& 值, 稳定编码 编码, 一致当前事实种类 期望种类) noexcept {
        const std::size_t 当前数量 = (值.当前节点.contains(编码.值) ? 1U : 0U)
            + (值.当前关系.contains(编码.值) ? 1U : 0U)
            + (值.当前值.contains(编码.值) ? 1U : 0U);
        const auto 历史 = 值.历史.find(编码.值);
        if (当前数量 != 0) {
            if (当前数量 != 1 || 历史 != 值.历史.end()) return std::nullopt;
            return L1中性一致当前读取项目状态::种类不匹配;
        }
        if (历史 == 值.历史.end())
            return L1中性一致当前读取项目状态::未找到;
        if (历史->second.当前有效 || 历史->second.查询编码 != 编码)
            return std::nullopt;
        一致当前事实种类 历史种类 = 一致当前事实种类::节点;
        const bool 历史完整 = std::visit([&](const auto& 事实) {
            using 类型 = std::decay_t<decltype(事实)>;
            if constexpr (std::is_same_v<类型, 节点事实>)
                历史种类 = 一致当前事实种类::节点;
            else if constexpr (std::is_same_v<类型, 关系事实>)
                历史种类 = 一致当前事实种类::关系;
            else 历史种类 = 一致当前事实种类::值;
            return 事实.编码 == 编码 && 事实.创建事实代次 != 0
                && 事实.退出事实代次
                && 事实.创建事实代次 <= *事实.退出事实代次
                && *事实.退出事实代次 <= 值.事实代次;
        }, 历史->second.事实);
        if (!历史完整) return std::nullopt;
        return 历史种类 == 期望种类
            ? L1中性一致当前读取项目状态::已退出
            : L1中性一致当前读取项目状态::种类不匹配;
    }

    // 诊断责任：向上送出；事实副本分配异常由公开仓库入口映射。
    static std::optional<一致具名事实内部结果项> 读取一致具名当前事实(
        const 状态& 值, 稳定编码 编码, 一致当前事实种类 种类) {
        一致具名事实内部结果项 结果{编码,
            L1中性一致当前读取项目状态::未找到, std::nullopt};
        if (种类 == 一致当前事实种类::节点) {
            const auto it = 值.当前节点.find(编码.值);
            if (it != 值.当前节点.end()) {
                if (!一致当前节点局部完整(值, it->first, it->second))
                    return std::nullopt;
                结果.状态 = L1中性一致当前读取项目状态::成功;
                结果.事实 = L1事实副本{it->second};
                return 结果;
            }
        } else if (种类 == 一致当前事实种类::关系) {
            const auto it = 值.当前关系.find(编码.值);
            if (it != 值.当前关系.end()) {
                if (!一致当前关系局部完整(值, it->first, it->second))
                    return std::nullopt;
                结果.状态 = L1中性一致当前读取项目状态::成功;
                结果.事实 = L1事实副本{it->second};
                return 结果;
            }
        } else {
            const auto it = 值.当前值.find(编码.值);
            if (it != 值.当前值.end()) {
                if (!一致当前值局部完整(值, it->first, it->second))
                    return std::nullopt;
                结果.状态 = L1中性一致当前读取项目状态::成功;
                结果.事实 = L1事实副本{it->second};
                return 结果;
            }
        }
        if (const auto 墓碑 = 值.物理清理墓碑.find(编码.值);
            墓碑 != 值.物理清理墓碑.end()) {
            const auto 期望种类 = 种类 == 一致当前事实种类::节点
                ? L1物理清理事实种类::节点
                : 种类 == 一致当前事实种类::关系
                    ? L1物理清理事实种类::关系
                    : L1物理清理事实种类::值;
            结果.状态 = 墓碑->second.事实种类 == 期望种类
                ? L1中性一致当前读取项目状态::历史材料已清理
                : L1中性一致当前读取项目状态::种类不匹配;
            if (结果.状态 == L1中性一致当前读取项目状态::历史材料已清理)
                结果.物理清理事实代次 = 墓碑->second.物理清理事实代次;
            if (结果.状态 == L1中性一致当前读取项目状态::历史材料已清理)
                结果.物理清理墓碑 = 墓碑->second;
            return 结果;
        }
        const auto 状态 = 一致当前缺失状态(值, 编码, 种类);
        if (!状态) return std::nullopt;
        结果.状态 = *状态;
        return 结果;
    }

    // 诊断责任：向上送出；关系与对端副本分配异常由公开仓库入口映射。
    static std::optional<一致源关系组内部结果项> 读取一致源关系组(
        const 状态& 值, const L1中性一致源关系组选择项& 选择) {
        一致源关系组内部结果项 结果{选择.源节点, 选择.关系类型节点, {}};
        const auto 源 = 值.当前源关系索引.find(选择.源节点.值);
        if (源 == 值.当前源关系索引.end()) return 结果;
        const auto 类型 = 源->second.find(选择.关系类型节点.值);
        if (类型 == 源->second.end()) return 结果;
        if (类型->second.empty()) return std::nullopt;
        结果.成员.reserve(类型->second.size());
        std::uint64_t 前一编码 = 0;
        for (const auto 编码 : 类型->second) {
            const auto 关系 = 值.当前关系.find(编码);
            if (编码 == 0 || 编码 <= 前一编码 || 关系 == 值.当前关系.end()
                || !一致当前关系局部完整(值, 编码, 关系->second)
                || 关系->second.源节点 != 选择.源节点
                || 关系->second.关系类型节点 != 选择.关系类型节点)
                return std::nullopt;
            const auto 对端 = 值.当前节点.find(关系->second.目标节点.值);
            if (对端 == 值.当前节点.end()
                || !一致当前节点局部完整(值, 对端->first, 对端->second))
                return std::nullopt;
            结果.成员.push_back({关系->second, 对端->second});
            前一编码 = 编码;
        }
        return 结果;
    }

    // 诊断责任：向上送出；关系与对端副本分配异常由公开仓库入口映射。
    static std::optional<一致目标关系组内部结果项> 读取一致目标关系组(
        const 状态& 值, const L1中性一致目标关系组选择项& 选择) {
        一致目标关系组内部结果项 结果{选择.目标节点, 选择.关系类型节点, {}};
        const auto 目标 = 值.当前目标关系索引.find(选择.目标节点.值);
        if (目标 == 值.当前目标关系索引.end()) return 结果;
        const auto 类型 = 目标->second.find(选择.关系类型节点.值);
        if (类型 == 目标->second.end()) return 结果;
        if (类型->second.empty()) return std::nullopt;
        结果.成员.reserve(类型->second.size());
        std::uint64_t 前一编码 = 0;
        for (const auto 编码 : 类型->second) {
            const auto 关系 = 值.当前关系.find(编码);
            if (编码 == 0 || 编码 <= 前一编码 || 关系 == 值.当前关系.end()
                || !一致当前关系局部完整(值, 编码, 关系->second)
                || 关系->second.目标节点 != 选择.目标节点
                || 关系->second.关系类型节点 != 选择.关系类型节点)
                return std::nullopt;
            const auto 对端 = 值.当前节点.find(关系->second.源节点.值);
            if (对端 == 值.当前节点.end()
                || !一致当前节点局部完整(值, 对端->first, 对端->second))
                return std::nullopt;
            结果.成员.push_back({关系->second, 对端->second});
            前一编码 = 编码;
        }
        return 结果;
    }

    static std::optional<L1所有者范围一致属性值读取结果项>
    读取所有者范围一致属性值(const 状态& 值, 稳定编码 节点编码,
        稳定编码 属性类型编码) {
        L1所有者范围一致属性值读取结果项 结果{节点编码, 属性类型编码,
            L1所有者范围一致当前读取项目状态::未找到, std::nullopt};
        const auto 节点项 = 读取一致具名当前事实(
            值, 节点编码, 一致当前事实种类::节点);
        if (!节点项) return std::nullopt;
        if (节点项->状态 != L1中性一致当前读取项目状态::成功) {
            结果.状态 = static_cast<L1所有者范围一致当前读取项目状态>(
                static_cast<std::uint8_t>(节点项->状态));
            return 结果;
        }
        const auto* 节点 = 节点项->事实
            ? std::get_if<节点事实>(&*节点项->事实) : nullptr;
        if (!节点) return std::nullopt;
        const auto 槽 = std::lower_bound(节点->当前属性.begin(),
            节点->当前属性.end(), 属性类型编码,
            [](const 属性槽& 左, 稳定编码 右) {
                return 左.属性类型节点 < 右;
            });
        if (槽 == 节点->当前属性.end()
            || 槽->属性类型节点 != 属性类型编码) {
            结果.状态 = L1所有者范围一致当前读取项目状态::属性未设置;
            return 结果;
        }
        const auto 当前值 = 值.当前值.find(槽->当前值.值);
        if (当前值 == 值.当前值.end()
            || !一致当前值局部完整(值, 当前值->first, 当前值->second)
            || 当前值->second.所属节点 != 节点编码
            || 当前值->second.属性类型节点 != 属性类型编码)
            return std::nullopt;
        结果.状态 = L1所有者范围一致当前读取项目状态::成功;
        结果.投影 = L1所有者范围一致属性值投影{
            {槽->属性类型节点, 槽->当前值},
            转换所有者范围值(当前值->second)};
        return 结果;
    }

    static std::optional<L1所有者范围一致源关系组读取结果项>
    读取所有者范围一致源关系组(const 状态& 值,
        const L1所有者范围一致源关系组选择项& 选择) {
        const auto 内部 = 读取一致源关系组(值,
            {选择.源节点, 选择.关系类型节点});
        if (!内部) return std::nullopt;
        L1所有者范围一致源关系组读取结果项 结果{
            选择.源节点, 选择.关系类型节点, {}};
        结果.成员.reserve(内部->成员.size());
        for (const auto& 成员 : 内部->成员)
            结果.成员.push_back({转换所有者范围关系(成员.关系),
                转换所有者范围节点(成员.对端节点)});
        return 结果;
    }

    static std::optional<L1所有者范围一致目标关系组读取结果项>
    读取所有者范围一致目标关系组(const 状态& 值,
        const L1所有者范围一致目标关系组选择项& 选择) {
        const auto 内部 = 读取一致目标关系组(值,
            {选择.目标节点, 选择.关系类型节点});
        if (!内部) return std::nullopt;
        L1所有者范围一致目标关系组读取结果项 结果{
            选择.目标节点, 选择.关系类型节点, {}};
        结果.成员.reserve(内部->成员.size());
        for (const auto& 成员 : 内部->成员)
            结果.成员.push_back({转换所有者范围关系(成员.关系),
                转换所有者范围节点(成员.对端节点)});
        return 结果;
    }

    static std::optional<L1所有者范围一致闭包端点关系组读取结果项>
    读取一致闭包端点关系组(const 状态& 值, 稳定编码 端点节点,
        稳定编码 关系类型节点, bool 源方向) {
        L1所有者范围一致闭包端点关系组读取结果项 结果{
            端点节点, 关系类型节点,
            L1所有者范围一致当前读取项目状态::未找到,
            std::nullopt, {}};
        const auto 类型项 = 读取一致具名当前事实(
            值, 关系类型节点, 一致当前事实种类::节点);
        if (!类型项) return std::nullopt;
        if (类型项->状态 != L1中性一致当前读取项目状态::成功) {
            结果.状态 = static_cast<L1所有者范围一致当前读取项目状态>(
                static_cast<std::uint8_t>(类型项->状态));
            return 结果;
        }
        const auto* 类型事实 = 类型项->事实
            ? std::get_if<节点事实>(&*类型项->事实) : nullptr;
        if (!类型事实) return std::nullopt;
        if (类型事实->种类 != 节点种类::普通) {
            结果.状态 = L1所有者范围一致当前读取项目状态::种类不匹配;
            return 结果;
        }
        结果.状态 = L1所有者范围一致当前读取项目状态::成功;
        结果.关系类型事实 = 转换所有者范围节点(*类型事实);
        if (源方向) {
            const auto 内部 = 读取一致源关系组(
                值, {端点节点, 关系类型节点});
            if (!内部) return std::nullopt;
            结果.成员.reserve(内部->成员.size());
            for (const auto& 成员 : 内部->成员)
                结果.成员.push_back({转换所有者范围关系(成员.关系),
                    转换所有者范围节点(成员.对端节点)});
        } else {
            const auto 内部 = 读取一致目标关系组(
                值, {端点节点, 关系类型节点});
            if (!内部) return std::nullopt;
            结果.成员.reserve(内部->成员.size());
            for (const auto& 成员 : 内部->成员)
                结果.成员.push_back({转换所有者范围关系(成员.关系),
                    转换所有者范围节点(成员.对端节点)});
        }
        return 结果;
    }

    static std::optional<L1所有者范围一致关系类型闭包读取结果项>
    读取一致关系类型闭包(const 状态& 值,
        const L1所有者范围一致关系类型闭包选择项& 选择) {
        L1所有者范围一致关系类型闭包读取结果项 结果{
            选择.入口关系类型节点,
            L1所有者范围一致当前读取项目状态::未找到,
            std::nullopt, {}};
        const auto 类型项 = 读取一致具名当前事实(
            值, 选择.入口关系类型节点, 一致当前事实种类::节点);
        if (!类型项) return std::nullopt;
        if (类型项->状态 != L1中性一致当前读取项目状态::成功) {
            结果.状态 = static_cast<L1所有者范围一致当前读取项目状态>(
                static_cast<std::uint8_t>(类型项->状态));
            return 结果;
        }
        const auto* 类型事实 = 类型项->事实
            ? std::get_if<节点事实>(&*类型项->事实) : nullptr;
        if (!类型事实) return std::nullopt;
        if (类型事实->种类 != 节点种类::普通) {
            结果.状态 = L1所有者范围一致当前读取项目状态::种类不匹配;
            return 结果;
        }
        结果.状态 = L1所有者范围一致当前读取项目状态::成功;
        结果.关系类型事实 = 转换所有者范围节点(*类型事实);

        const auto 类型索引 = 值.当前关系类型索引.find(
            选择.入口关系类型节点.值);
        if (类型索引 == 值.当前关系类型索引.end()) return 结果;
        if (类型索引->second.empty()) return std::nullopt;
        结果.成员.reserve(类型索引->second.size());
        std::uint64_t 前一关系编码 = 0;
        for (const auto 关系编码 : 类型索引->second) {
            const auto 关系 = 值.当前关系.find(关系编码);
            if (关系编码 == 0 || 关系编码 <= 前一关系编码
                || 关系 == 值.当前关系.end()
                || !一致当前关系局部完整(值, 关系编码, 关系->second)
                || 关系->second.关系类型节点 != 选择.入口关系类型节点)
                return std::nullopt;
            const auto 源节点 = 值.当前节点.find(关系->second.源节点.值);
            const auto 目标节点 = 值.当前节点.find(关系->second.目标节点.值);
            if (源节点 == 值.当前节点.end() || 目标节点 == 值.当前节点.end()
                || !一致当前节点局部完整(值, 源节点->first, 源节点->second)
                || !一致当前节点局部完整(值, 目标节点->first, 目标节点->second))
                return std::nullopt;

            L1所有者范围一致关系类型闭包成员 成员;
            成员.关系 = 转换所有者范围关系(关系->second);
            成员.源节点 = 转换所有者范围节点(源节点->second);
            成员.目标节点 = 转换所有者范围节点(目标节点->second);
            成员.源节点属性值.reserve(选择.源节点属性类型.size());
            成员.源节点源关系组.reserve(选择.源节点源关系类型.size());
            成员.源节点目标关系组.reserve(选择.源节点目标关系类型.size());
            成员.目标节点属性值.reserve(选择.目标节点属性类型.size());
            成员.目标节点源关系组.reserve(选择.目标节点源关系类型.size());
            成员.目标节点目标关系组.reserve(选择.目标节点目标关系类型.size());
            for (const auto 属性类型 : 选择.源节点属性类型) {
                auto 项 = 读取所有者范围一致属性值(
                    值, 关系->second.源节点, 属性类型);
                if (!项) return std::nullopt;
                成员.源节点属性值.push_back(std::move(*项));
            }
            for (const auto 关系类型 : 选择.源节点源关系类型) {
                auto 项 = 读取一致闭包端点关系组(
                    值, 关系->second.源节点, 关系类型, true);
                if (!项) return std::nullopt;
                成员.源节点源关系组.push_back(std::move(*项));
            }
            for (const auto 关系类型 : 选择.源节点目标关系类型) {
                auto 项 = 读取一致闭包端点关系组(
                    值, 关系->second.源节点, 关系类型, false);
                if (!项) return std::nullopt;
                成员.源节点目标关系组.push_back(std::move(*项));
            }
            for (const auto 属性类型 : 选择.目标节点属性类型) {
                auto 项 = 读取所有者范围一致属性值(
                    值, 关系->second.目标节点, 属性类型);
                if (!项) return std::nullopt;
                成员.目标节点属性值.push_back(std::move(*项));
            }
            for (const auto 关系类型 : 选择.目标节点源关系类型) {
                auto 项 = 读取一致闭包端点关系组(
                    值, 关系->second.目标节点, 关系类型, true);
                if (!项) return std::nullopt;
                成员.目标节点源关系组.push_back(std::move(*项));
            }
            for (const auto 关系类型 : 选择.目标节点目标关系类型) {
                auto 项 = 读取一致闭包端点关系组(
                    值, 关系->second.目标节点, 关系类型, false);
                if (!项) return std::nullopt;
                成员.目标节点目标关系组.push_back(std::move(*项));
            }
            结果.成员.push_back(std::move(成员));
            前一关系编码 = 关系编码;
        }
        return 结果;
    }

    // 诊断责任：向上送出；分配异常由调用方公开边界映射。
    static bool 插入当前源关系索引(状态& 值, const 关系事实& 事实) {
        if (!有效(事实.编码) || !有效(事实.源节点)
            || !有效(事实.关系类型节点) || 事实.退出事实代次) return false;
        auto& 编码组 = 值.当前源关系索引[事实.源节点.值][事实.关系类型节点.值];
        const auto 位置 = std::lower_bound(编码组.begin(), 编码组.end(), 事实.编码.值);
        if (位置 != 编码组.end() && *位置 == 事实.编码.值) return false;
        编码组.insert(位置, 事实.编码.值);
        return true;
    }

    // 诊断责任：无适用错误分支；只删除候选状态中的一个派生索引编码。
    static bool 删除当前源关系索引(状态& 值, const 关系事实& 事实) noexcept {
        const auto 源 = 值.当前源关系索引.find(事实.源节点.值);
        if (源 == 值.当前源关系索引.end()) return false;
        const auto 类型 = 源->second.find(事实.关系类型节点.值);
        if (类型 == 源->second.end()) return false;
        const auto 位置 = std::lower_bound(类型->second.begin(), 类型->second.end(), 事实.编码.值);
        if (位置 == 类型->second.end() || *位置 != 事实.编码.值) return false;
        类型->second.erase(位置);
        if (类型->second.empty()) 源->second.erase(类型);
        if (源->second.empty()) 值.当前源关系索引.erase(源);
        return true;
    }

    // 诊断责任：向上送出；只从本状态权威当前关系纯派生非权威索引。
    static bool 派生当前源关系索引(状态& 值) {
        值.当前源关系索引.clear();
        for (const auto& [_, 事实] : 值.当前关系)
            if (!插入当前源关系索引(值, 事实)) return false;
        return true;
    }

    // 诊断责任：向上送出；分配异常由调用方公开边界映射。
    static bool 插入当前目标关系索引(状态& 值, const 关系事实& 事实) {
        if (!有效(事实.编码) || !有效(事实.目标节点)
            || !有效(事实.关系类型节点) || 事实.退出事实代次) return false;
        auto& 编码组 = 值.当前目标关系索引[事实.目标节点.值][事实.关系类型节点.值];
        const auto 位置 = std::lower_bound(编码组.begin(), 编码组.end(), 事实.编码.值);
        if (位置 != 编码组.end() && *位置 == 事实.编码.值) return false;
        编码组.insert(位置, 事实.编码.值);
        return true;
    }

    // 诊断责任：无适用错误分支；只删除候选状态中的一个派生索引编码。
    static bool 删除当前目标关系索引(状态& 值, const 关系事实& 事实) noexcept {
        const auto 目标 = 值.当前目标关系索引.find(事实.目标节点.值);
        if (目标 == 值.当前目标关系索引.end()) return false;
        const auto 类型 = 目标->second.find(事实.关系类型节点.值);
        if (类型 == 目标->second.end()) return false;
        const auto 位置 = std::lower_bound(类型->second.begin(), 类型->second.end(), 事实.编码.值);
        if (位置 == 类型->second.end() || *位置 != 事实.编码.值) return false;
        类型->second.erase(位置);
        if (类型->second.empty()) 目标->second.erase(类型);
        if (目标->second.empty()) 值.当前目标关系索引.erase(目标);
        return true;
    }

    // 诊断责任：向上送出；分配异常由调用方公开边界映射。
    static bool 插入当前关系类型索引(状态& 值, const 关系事实& 事实) {
        if (!有效(事实.编码) || !有效(事实.关系类型节点)
            || 事实.退出事实代次) return false;
        auto& 编码组 = 值.当前关系类型索引[事实.关系类型节点.值];
        const auto 位置 = std::lower_bound(
            编码组.begin(), 编码组.end(), 事实.编码.值);
        if (位置 != 编码组.end() && *位置 == 事实.编码.值) return false;
        编码组.insert(位置, 事实.编码.值);
        return true;
    }

    // 诊断责任：无适用错误分支；只删除候选状态中的一个派生索引编码。
    static bool 删除当前关系类型索引(
        状态& 值, const 关系事实& 事实) noexcept {
        const auto 类型 = 值.当前关系类型索引.find(事实.关系类型节点.值);
        if (类型 == 值.当前关系类型索引.end()) return false;
        const auto 位置 = std::lower_bound(
            类型->second.begin(), 类型->second.end(), 事实.编码.值);
        if (位置 == 类型->second.end() || *位置 != 事实.编码.值) return false;
        类型->second.erase(位置);
        if (类型->second.empty()) 值.当前关系类型索引.erase(类型);
        return true;
    }

    // 诊断责任：向上送出；只从本状态权威当前关系纯派生非权威索引。
    static bool 派生当前关系类型索引(状态& 值) {
        值.当前关系类型索引.clear();
        for (const auto& [_, 事实] : 值.当前关系)
            if (!插入当前关系类型索引(值, 事实)) return false;
        return true;
    }

    // 诊断责任：向上送出；只按关系的去重当前节点角色登记事实编码。
    static bool 插入当前节点引用索引(状态& 值, const 关系事实& 事实) {
        if (!有效(事实.编码) || !有效(事实.源节点)
            || !有效(事实.目标节点) || !有效(事实.关系类型节点)
            || 事实.退出事实代次) return false;
        std::uint64_t 节点组[3]{
            事实.源节点.值, 事实.目标节点.值, 事实.关系类型节点.值};
        std::sort(节点组, 节点组 + 3);
        std::size_t 数量 = 0;
        for (const auto 编码 : 节点组)
            if (数量 == 0 || 节点组[数量 - 1] != 编码) 节点组[数量++] = 编码;
        for (std::size_t 索引 = 0; 索引 < 数量; ++索引) {
            if (!值.当前节点.contains(节点组[索引])) return false;
            auto& 编码组 = 值.当前节点引用索引[节点组[索引]];
            const auto 位置 = std::lower_bound(编码组.begin(), 编码组.end(), 事实.编码.值);
            if (位置 != 编码组.end() && *位置 == 事实.编码.值) return false;
            编码组.insert(位置, 事实.编码.值);
        }
        return true;
    }

    // 诊断责任：向上送出；只按值的去重当前节点角色登记事实编码。
    static bool 插入当前节点引用索引(状态& 值, const 值事实& 事实) {
        if (!有效(事实.编码) || !有效(事实.所属节点)
            || !有效(事实.属性类型节点) || !有效(事实.来源节点)
            || 事实.退出事实代次) return false;
        std::uint64_t 节点组[4]{
            事实.所属节点.值, 事实.属性类型节点.值, 事实.来源节点.值, 0};
        std::size_t 原数量 = 3;
        if (const auto* 引用 = std::get_if<独立材料引用>(&事实.材料)) {
            if (!有效(引用->编码)) return false;
            节点组[原数量++] = 引用->编码.值;
        }
        std::sort(节点组, 节点组 + 原数量);
        std::size_t 数量 = 0;
        for (std::size_t 索引 = 0; 索引 < 原数量; ++索引)
            if (数量 == 0 || 节点组[数量 - 1] != 节点组[索引])
                节点组[数量++] = 节点组[索引];
        for (std::size_t 索引 = 0; 索引 < 数量; ++索引) {
            if (!值.当前节点.contains(节点组[索引])) return false;
            auto& 编码组 = 值.当前节点引用索引[节点组[索引]];
            const auto 位置 = std::lower_bound(编码组.begin(), 编码组.end(), 事实.编码.值);
            if (位置 != 编码组.end() && *位置 == 事实.编码.值) return false;
            编码组.insert(位置, 事实.编码.值);
        }
        return true;
    }

    // 诊断责任：无适用错误分支；只删除一个当前关系的全部去重节点引用。
    static bool 删除当前节点引用索引(状态& 值, const 关系事实& 事实) noexcept {
        std::uint64_t 节点组[3]{
            事实.源节点.值, 事实.目标节点.值, 事实.关系类型节点.值};
        std::sort(节点组, 节点组 + 3);
        std::size_t 数量 = 0;
        for (const auto 编码 : 节点组)
            if (数量 == 0 || 节点组[数量 - 1] != 编码) 节点组[数量++] = 编码;
        for (std::size_t 索引 = 0; 索引 < 数量; ++索引) {
            const auto 节点 = 值.当前节点引用索引.find(节点组[索引]);
            if (节点 == 值.当前节点引用索引.end()) return false;
            const auto 位置 = std::lower_bound(
                节点->second.begin(), 节点->second.end(), 事实.编码.值);
            if (位置 == 节点->second.end() || *位置 != 事实.编码.值) return false;
            节点->second.erase(位置);
            if (节点->second.empty()) 值.当前节点引用索引.erase(节点);
        }
        return true;
    }

    // 诊断责任：无适用错误分支；只删除一个当前值的全部去重节点引用。
    static bool 删除当前节点引用索引(状态& 值, const 值事实& 事实) noexcept {
        std::uint64_t 节点组[4]{
            事实.所属节点.值, 事实.属性类型节点.值, 事实.来源节点.值, 0};
        std::size_t 原数量 = 3;
        if (const auto* 引用 = std::get_if<独立材料引用>(&事实.材料))
            节点组[原数量++] = 引用->编码.值;
        std::sort(节点组, 节点组 + 原数量);
        std::size_t 数量 = 0;
        for (std::size_t 索引 = 0; 索引 < 原数量; ++索引)
            if (数量 == 0 || 节点组[数量 - 1] != 节点组[索引])
                节点组[数量++] = 节点组[索引];
        for (std::size_t 索引 = 0; 索引 < 数量; ++索引) {
            const auto 节点 = 值.当前节点引用索引.find(节点组[索引]);
            if (节点 == 值.当前节点引用索引.end()) return false;
            const auto 位置 = std::lower_bound(
                节点->second.begin(), 节点->second.end(), 事实.编码.值);
            if (位置 == 节点->second.end() || *位置 != 事实.编码.值) return false;
            节点->second.erase(位置);
            if (节点->second.empty()) 值.当前节点引用索引.erase(节点);
        }
        return true;
    }

    // 诊断责任：无适用错误分支；只按反向候选回读权威当前事实并作三值闭包裁决。
    static 当前节点引用闭包检查结果 检查当前节点引用闭包(
        const 状态& 值, 稳定编码 节点编码,
        const std::unordered_set<std::uint64_t>& 退出集合) noexcept {
        if (!有效(节点编码) || !值.当前节点.contains(节点编码.值))
            return 当前节点引用闭包检查结果::内部不一致;
        const auto 节点 = 值.当前节点引用索引.find(节点编码.值);
        if (节点 == 值.当前节点引用索引.end())
            return 当前节点引用闭包检查结果::已闭合;
        if (节点->second.empty()) return 当前节点引用闭包检查结果::内部不一致;
        std::uint64_t 前一编码 = 0;
        for (const auto 事实编码 : 节点->second) {
            const auto 关系 = 值.当前关系.find(事实编码);
            const auto 当前值 = 值.当前值.find(事实编码);
            const bool 关系命中 = 关系 != 值.当前关系.end();
            const bool 值命中 = 当前值 != 值.当前值.end();
            if (事实编码 == 0 || 事实编码 <= 前一编码 || 关系命中 == 值命中)
                return 当前节点引用闭包检查结果::内部不一致;
            bool 真实引用 = false;
            if (关系命中) {
                const auto& 事实 = 关系->second;
                真实引用 = 事实.编码.值 == 事实编码 && !事实.退出事实代次
                    && (事实.源节点 == 节点编码 || 事实.目标节点 == 节点编码
                        || 事实.关系类型节点 == 节点编码);
            } else {
                const auto& 事实 = 当前值->second;
                const auto* 材料引用 = std::get_if<独立材料引用>(&事实.材料);
                真实引用 = 事实.编码.值 == 事实编码 && !事实.退出事实代次
                    && (事实.所属节点 == 节点编码
                        || 事实.属性类型节点 == 节点编码
                        || 事实.来源节点 == 节点编码
                        || (材料引用 && 材料引用->编码 == 节点编码));
            }
            if (!真实引用) return 当前节点引用闭包检查结果::内部不一致;
            if (!退出集合.contains(事实编码))
                return 当前节点引用闭包检查结果::引用冲突;
            前一编码 = 事实编码;
        }
        return 当前节点引用闭包检查结果::已闭合;
    }

    // 诊断责任：向上送出；只从本状态权威当前关系纯派生非权威索引。
    static bool 派生当前目标关系索引(状态& 值) {
        值.当前目标关系索引.clear();
        for (const auto& [_, 事实] : 值.当前关系)
            if (!插入当前目标关系索引(值, 事实)) return false;
        return true;
    }

    // 诊断责任：向上送出；分配异常由调用方公开边界映射。
    static bool 插入历史源关系候选索引(状态& 值, const 关系事实& 事实) {
        if (!有效(事实.编码) || !有效(事实.源节点)
            || !有效(事实.关系类型节点) || 事实.创建事实代次 == 0) return false;
        auto& 编码组 = 值.历史源关系候选索引[事实.源节点.值]
            [事实.关系类型节点.值];
        const auto 位置 = std::lower_bound(编码组.begin(), 编码组.end(), 事实.编码.值);
        if (位置 != 编码组.end() && *位置 == 事实.编码.值) return false;
        编码组.insert(位置, 事实.编码.值);
        return true;
    }

    // 诊断责任：向上送出；分配异常由调用方公开边界映射。
    static bool 插入历史目标关系候选索引(状态& 值, const 关系事实& 事实) {
        if (!有效(事实.编码) || !有效(事实.目标节点)
            || !有效(事实.关系类型节点) || 事实.创建事实代次 == 0) return false;
        auto& 编码组 = 值.历史目标关系候选索引[事实.目标节点.值]
            [事实.关系类型节点.值];
        const auto 位置 = std::lower_bound(编码组.begin(), 编码组.end(), 事实.编码.值);
        if (位置 != 编码组.end() && *位置 == 事实.编码.值) return false;
        编码组.insert(位置, 事实.编码.值);
        return true;
    }

    // 诊断责任：向上送出；只登记已经由成功属性槽变更采用的值稳定编码。
    static bool 插入历史属性槽值候选索引(状态& 值, const 值事实& 事实) {
        if (!有效(事实.编码) || !有效(事实.所属节点)
            || !有效(事实.属性类型节点) || 事实.创建事实代次 == 0) return false;
        auto& 编码组 = 值.历史属性槽值候选索引[事实.所属节点.值]
            [事实.属性类型节点.值];
        const auto 位置 = std::lower_bound(编码组.begin(), 编码组.end(), 事实.编码.值);
        if (位置 != 编码组.end() && *位置 == 事实.编码.值) return false;
        编码组.insert(位置, 事实.编码.值);
        return true;
    }

    // 诊断责任：向上送出；分配异常由提交中性写集统一映射。
    static L1中性写入结果 中性写入结果(const L1中性写集请求& 请求,
        L1中性写入状态 状态值, std::uint64_t 代次, bool 已发布,
        L1中性重试边界 重试,
        std::vector<std::pair<L1中性写集本地键, 稳定编码>> 映射 = {}) {
        return {状态值, L1中性CRUD合同版本, 请求.幂等键, 代次,
            已发布, 重试, std::move(映射)};
    }

    // 诊断责任：无适用错误分支；纯值排序键转换。
    static std::uint64_t 中性引用排序键(const L1中性事实引用& 引用) {
        return std::visit([](const auto& 值) -> std::uint64_t {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, 稳定编码>) return 值.值;
            else return (1ULL << 63) | 值.值;
        }, 引用);
    }

    // 诊断责任：无适用错误分支；纯值有效性判断。
    static bool 中性引用有效(const L1中性事实引用& 引用) noexcept {
        return std::visit([](const auto& 值) noexcept { return 有效(值); }, 引用);
    }

    // 诊断责任：向上送出；分配异常由提交中性写集统一映射。
    static std::optional<L1中性写集请求> 规范化中性写集(
        const L1中性写集请求& 请求) {
        L1中性写集请求 结果 = 请求;
        std::sort(结果.节点.begin(), 结果.节点.end(), [](const auto& 左, const auto& 右) {
            return 左.本地键 < 右.本地键;
        });
        std::sort(结果.关系.begin(), 结果.关系.end(), [](const auto& 左, const auto& 右) {
            return 左.本地键 < 右.本地键;
        });
        std::sort(结果.值.begin(), 结果.值.end(), [](const auto& 左, const auto& 右) {
            return 左.本地键 < 右.本地键;
        });
        std::sort(结果.属性槽变更.begin(), 结果.属性槽变更.end(), [](const auto& 左,
            const auto& 右) {
            const auto 左所属 = 中性引用排序键(左.所属节点);
            const auto 右所属 = 中性引用排序键(右.所属节点);
            if (左所属 != 右所属) return 左所属 < 右所属;
            const auto 左类型 = 中性引用排序键(左.属性类型节点);
            const auto 右类型 = 中性引用排序键(右.属性类型节点);
            if (左类型 != 右类型) return 左类型 < 右类型;
            return 左.新当前值 < 右.新当前值;
        });
        std::sort(结果.退出事实.begin(), 结果.退出事实.end());
        return 结果;
    }

    // 诊断责任：向上送出；分配异常由提交中性写集统一映射。
    static bool 中性请求结构有效(const L1中性写集请求& 请求,
        std::unordered_set<std::uint32_t>& 本地键) {
        auto 登记 = [&](L1中性写集本地键 键) {
            return 有效(键) && 本地键.insert(键.值).second;
        };
        for (const auto& 项 : 请求.节点) {
            if (!登记(项.本地键)
                || ((项.种类 == 节点种类::属性类型) != 项.属性类型表示.has_value()))
                return false;
        }
        for (const auto& 项 : 请求.关系)
            if (!登记(项.本地键) || !中性引用有效(项.源节点)
                || !中性引用有效(项.目标节点) || !中性引用有效(项.关系类型节点))
                return false;
        for (const auto& 项 : 请求.值)
            if (!登记(项.本地键) || !中性引用有效(项.所属节点)
                || !中性引用有效(项.属性类型节点) || !中性引用有效(项.来源节点)
                || !L1中性原始材料完整(项.材料)) return false;
        std::unordered_set<std::uint64_t> 退出;
        for (const auto 编码 : 请求.退出事实)
            if (!有效(编码) || !退出.insert(编码.值).second) return false;
        for (std::size_t i = 0; i < 请求.属性槽变更.size(); ++i) {
            const auto& 项 = 请求.属性槽变更[i];
            if (!中性引用有效(项.所属节点) || !中性引用有效(项.属性类型节点)
                || !有效(项.新当前值)) return false;
            if (i != 0) {
                const auto& 前 = 请求.属性槽变更[i - 1];
                if (前.所属节点 == 项.所属节点 && 前.属性类型节点 == 项.属性类型节点)
                    return false;
            }
        }
        return true;
    }

    // 诊断责任：无适用错误分支；纯值枚举转换。
    static std::optional<值表示种类> 转换值表示种类(
        const std::optional<L1中性值表示种类>& 值) noexcept {
        if (!值) return std::nullopt;
        return static_cast<值表示种类>(static_cast<std::uint8_t>(*值));
    }

    // 诊断责任：向上送出；分配异常由提交中性写集统一映射。
    static std::optional<原始值材料> 转换原始材料(const L1中性原始值材料& 材料) {
        return std::visit([](const auto& 值) -> std::optional<原始值材料> {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, L1中性独立材料引用>)
                return 原始值材料{独立材料引用{值.编码}};
            else return 原始值材料{值};
        }, 材料);
    }

    // 诊断责任：向上送出；只把权威内部材料复制为公开中性材料。
    static std::optional<L1中性原始值材料> 转换中性原始材料(
        const 原始值材料& 材料) {
        return std::visit([](const auto& 值) -> std::optional<L1中性原始值材料> {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, 独立材料引用>)
                return L1中性原始值材料{L1中性独立材料引用{值.编码}};
            else return L1中性原始值材料{值};
        }, 材料);
    }

    // 诊断责任：无适用错误分支；纯值材料比较。
    static bool 中性材料等于(const 原始值材料& 实际,
        const L1中性原始值材料& 期望) noexcept {
        return std::visit([&](const auto& 值) noexcept -> bool {
            using 类型 = std::decay_t<decltype(值)>;
            if constexpr (std::is_same_v<类型, L1中性独立材料引用>) {
                const auto* 实际引用 = std::get_if<独立材料引用>(&实际);
                return 实际引用 && 实际引用->编码 == 值.编码;
            } else {
                const auto* 实际值 = std::get_if<类型>(&实际);
                return 实际值 && *实际值 == 值;
            }
        }, 期望);
    }

    // 诊断责任：无适用错误分支；只读候选并返回结构化真假。
    static bool 中性候选读回完整(const 状态& 值,
        const L1中性写集请求& 请求,
        const std::vector<std::pair<L1中性写集本地键, 稳定编码>>& 映射,
        std::uint64_t 发布代次, L1结构所有者身份 写入所有者) noexcept {
        const auto 查映射 = [&](L1中性写集本地键 键) -> std::optional<稳定编码> {
            for (const auto& [本地, 编码] : 映射) if (本地 == 键) return 编码;
            return std::nullopt;
        };
        const auto 解析 = [&](const L1中性事实引用& 引用) -> std::optional<稳定编码> {
            return std::visit([&](const auto& 项) -> std::optional<稳定编码> {
                using 类型 = std::decay_t<decltype(项)>;
                if constexpr (std::is_same_v<类型, 稳定编码>) return 项;
                else return 查映射(项);
            }, 引用);
        };
        for (const auto& 项 : 请求.节点) {
            const auto 编码 = 查映射(项.本地键);
            if (!编码) return false;
            const auto it = 值.当前节点.find(编码->值);
            if (it == 值.当前节点.end() || it->second.编码 != *编码
                || it->second.写入所有者 != 写入所有者
                || it->second.种类 != 项.种类
                || it->second.属性类型表示 != 转换值表示种类(项.属性类型表示)
                || it->second.创建事实代次 != 发布代次 || it->second.退出事实代次)
                return false;
        }
        for (const auto& 项 : 请求.关系) {
            const auto 编码 = 查映射(项.本地键);
            const auto 源 = 解析(项.源节点), 目标 = 解析(项.目标节点),
                类型 = 解析(项.关系类型节点);
            if (!编码 || !源 || !目标 || !类型) return false;
            const auto it = 值.当前关系.find(编码->值);
            if (it == 值.当前关系.end() || it->second.源节点 != *源
                || it->second.写入所有者 != 写入所有者
                || it->second.目标节点 != *目标 || it->second.关系类型节点 != *类型
                || it->second.角色或顺序 != 项.角色或顺序
                || it->second.创建事实代次 != 发布代次 || it->second.退出事实代次)
                return false;
        }
        for (const auto& 项 : 请求.值) {
            const auto 编码 = 查映射(项.本地键);
            const auto 所属 = 解析(项.所属节点), 类型 = 解析(项.属性类型节点),
                来源 = 解析(项.来源节点);
            if (!编码 || !所属 || !类型 || !来源) return false;
            const auto it = 值.当前值.find(编码->值);
            if (it == 值.当前值.end() || it->second.所属节点 != *所属
                || it->second.写入所有者 != 写入所有者
                || it->second.属性类型节点 != *类型 || it->second.来源节点 != *来源
                || !中性材料等于(it->second.材料, 项.材料)
                || it->second.创建事实代次 != 发布代次 || it->second.退出事实代次)
                return false;
        }
        for (const auto& 项 : 请求.属性槽变更) {
            const auto 所属 = 解析(项.所属节点), 类型 = 解析(项.属性类型节点),
                当前 = 查映射(项.新当前值);
            if (!所属 || !类型 || !当前) return false;
            const auto 节点 = 值.当前节点.find(所属->值);
            if (节点 == 值.当前节点.end()) return false;
            const auto 槽 = std::find_if(节点->second.当前属性.begin(),
                节点->second.当前属性.end(), [&](const 属性槽& 值项) {
                    return 值项.属性类型节点 == *类型;
                });
            if (槽 == 节点->second.当前属性.end() || 槽->当前值 != *当前) return false;
        }
        for (const auto 编码 : 请求.退出事实) {
            const auto it = 值.历史.find(编码.值);
            if (it == 值.历史.end()) return false;
            const bool 匹配 = std::visit([&](const auto& 事实) {
                return 事实.编码 == 编码 && 事实.写入所有者 == 写入所有者
                    && 事实.退出事实代次 == 发布代次;
            }, it->second.事实);
            if (!匹配) return false;
        }
        return 值.事实代次 == 发布代次;
    }

    static bool 表示匹配(值表示种类 表示, const 原始值材料& 材料) {
        return (表示 == 值表示种类::I64 && std::holds_alternative<std::int64_t>(材料))
            || (表示 == 值表示种类::I64组 && std::holds_alternative<std::vector<std::int64_t>>(材料))
            || (表示 == 值表示种类::U64组 && std::holds_alternative<std::vector<std::uint64_t>>(材料))
            || (表示 == 值表示种类::独立材料引用 && std::holds_alternative<独立材料引用>(材料));
    }
    static bool 状态完整(const 状态& 值) {
        std::unordered_set<std::uint64_t> 全部;
        auto 插入 = [&](std::uint64_t 编码) { return 编码 != 0 && 全部.insert(编码).second; };
        for (const auto& [编码, 墓碑] : 值.物理清理墓碑) {
            if (!插入(编码) || 墓碑.编码.值 != 编码
                || !L1物理清理事实身份有效({墓碑.事实种类, 墓碑.编码})
                || !有效(墓碑.内部结构分区)
                || 墓碑.创建事实代次 == 0 || 墓碑.退出事实代次 == 0
                || 墓碑.物理清理事实代次 == 0
                || 墓碑.创建事实代次 > 墓碑.退出事实代次
                || 墓碑.退出事实代次 >= 墓碑.物理清理事实代次
                || 墓碑.物理清理事实代次 > 值.事实代次
                || 值.当前节点.contains(编码) || 值.当前关系.contains(编码)
                || 值.当前值.contains(编码) || 值.历史.contains(编码)
                || !值.永久占用.contains(编码)) return false;
        }
        for (const auto& [键, 账] : 值.物理清理幂等账) {
            if (键 == 0 || 账.首次规范请求.幂等身份.值 != 键
                || !L1物理清理请求规范有序(账.首次规范请求)
                || 账.首次状态 != L1物理清理状态::已清理
                || 账.首次物理清理事实代次 == 0
                || 账.首次物理清理事实代次 > 值.事实代次
                || !账.已物理清理
                || 账.首次稳定编码映射.size()
                    != 账.首次规范请求.待清理事实身份组.size()) return false;
            for (std::size_t 序号 = 0;
                序号 < 账.首次稳定编码映射.size(); ++序号) {
                const auto& 映射 = 账.首次稳定编码映射[序号];
                if (映射.first != 账.首次规范请求.待清理事实身份组[序号]
                    || 映射.first.编码 != 映射.second) return false;
                const auto 墓碑 = 值.物理清理墓碑.find(映射.second.值);
                if (墓碑 == 值.物理清理墓碑.end()
                    || 墓碑->second.事实种类 != 映射.first.事实种类
                    || 墓碑->second.物理清理事实代次
                        != 账.首次物理清理事实代次) return false;
            }
        }
        for (const auto& [编码, 所有者] : 值.当前所有者) {
            if (!插入(编码) || 所有者.所有者.编码.值 != 编码
                || 所有者.创建事实代次 == 0
                || 所有者.创建事实代次 > 值.事实代次
                || 所有者.退出事实代次
                || (所有者.范围种类 != L1所有者范围种类::旧共享范围
                    && 所有者.范围种类 != L1所有者范围种类::独占结构范围))
                return false;
        }
        for (const auto& [编码, 所有者] : 值.历史所有者) {
            if (!插入(编码) || 所有者.所有者.编码.值 != 编码
                || 所有者.创建事实代次 == 0 || !所有者.退出事实代次
                || 所有者.创建事实代次 > *所有者.退出事实代次
                || *所有者.退出事实代次 > 值.事实代次)
                return false;
        }
        if (值.旧共享所有者定位) {
            const auto 旧 = 值.当前所有者.find(
                值.旧共享所有者定位->编码.值);
            if (旧 == 值.当前所有者.end()
                || 旧->second.所有者 != *值.旧共享所有者定位
                || 旧->second.范围种类 != L1所有者范围种类::旧共享范围)
                return false;
            std::size_t 旧范围数量 = 0;
            for (const auto& [_, 所有者] : 值.当前所有者)
                if (所有者.范围种类 == L1所有者范围种类::旧共享范围)
                    ++旧范围数量;
            if (旧范围数量 != 1) return false;
        } else {
            for (const auto& [_, 所有者] : 值.当前所有者)
                if (所有者.范围种类 == L1所有者范围种类::旧共享范围)
                    return false;
        }
        for (const auto& [编码, 节点] : 值.当前节点) {
            if (节点.编码.值 != 编码 || 节点.创建事实代次 == 0 || 节点.创建事实代次 > 值.事实代次
                || (节点.种类 == 节点种类::属性类型) != 节点.属性类型表示.has_value()
                || 节点.退出事实代次 || !属性排序唯一(节点.当前属性) || !插入(编码)
                || !有效(节点.写入所有者)
                || !值.当前所有者.contains(节点.写入所有者.编码.值)) return false;
        }
        for (const auto& [编码, 关系] : 值.当前关系) {
            if (!插入(编码) || 关系.编码.值 != 编码 || 关系.创建事实代次 == 0
                || 关系.创建事实代次 > 值.事实代次 || 关系.退出事实代次
                || !值.当前节点.contains(关系.源节点.值)
                || !值.当前节点.contains(关系.目标节点.值)
                || !值.当前节点.contains(关系.关系类型节点.值)
                || !有效(关系.源节点) || !有效(关系.目标节点)
                || !有效(关系.关系类型节点) || !有效(关系.写入所有者)
                || !值.当前所有者.contains(关系.写入所有者.编码.值)
                || 值.当前节点.at(关系.关系类型节点.值).写入所有者
                    != 关系.写入所有者) return false;
        }
        std::unordered_set<std::uint64_t> 已源索引关系;
        for (const auto& [源编码, 类型组] : 值.当前源关系索引) {
            if (源编码 == 0 || 类型组.empty()) return false;
            for (const auto& [类型编码, 编码组] : 类型组) {
                if (类型编码 == 0 || 编码组.empty()) return false;
                std::uint64_t 前一编码 = 0;
                for (const auto 关系编码 : 编码组) {
                    const auto 关系 = 值.当前关系.find(关系编码);
                    if (关系编码 == 0 || 关系编码 <= 前一编码
                        || !已源索引关系.insert(关系编码).second
                        || 关系 == 值.当前关系.end()
                        || 关系->second.源节点.值 != 源编码
                        || 关系->second.关系类型节点.值 != 类型编码
                        || 关系->second.退出事实代次) return false;
                    前一编码 = 关系编码;
                }
            }
        }
        if (已源索引关系.size() != 值.当前关系.size()) return false;
        std::unordered_set<std::uint64_t> 已目标索引关系;
        for (const auto& [目标编码, 类型组] : 值.当前目标关系索引) {
            if (目标编码 == 0 || 类型组.empty()) return false;
            for (const auto& [类型编码, 编码组] : 类型组) {
                if (类型编码 == 0 || 编码组.empty()) return false;
                std::uint64_t 前一编码 = 0;
                for (const auto 关系编码 : 编码组) {
                    const auto 关系 = 值.当前关系.find(关系编码);
                    if (关系编码 == 0 || 关系编码 <= 前一编码
                        || !已目标索引关系.insert(关系编码).second
                        || 关系 == 值.当前关系.end()
                        || 关系->second.目标节点.值 != 目标编码
                        || 关系->second.关系类型节点.值 != 类型编码
                        || 关系->second.退出事实代次) return false;
                    前一编码 = 关系编码;
                }
            }
        }
        if (已目标索引关系.size() != 值.当前关系.size()) return false;
        std::unordered_set<std::uint64_t> 已类型索引关系;
        for (const auto& [类型编码, 编码组] : 值.当前关系类型索引) {
            const auto 类型节点 = 值.当前节点.find(类型编码);
            if (类型编码 == 0 || 编码组.empty()
                || 类型节点 == 值.当前节点.end()
                || !一致当前节点基本完整(
                    值, 类型节点->first, 类型节点->second)) return false;
            std::uint64_t 前一编码 = 0;
            for (const auto 关系编码 : 编码组) {
                const auto 关系 = 值.当前关系.find(关系编码);
                if (关系编码 == 0 || 关系编码 <= 前一编码
                    || !已类型索引关系.insert(关系编码).second
                    || 关系 == 值.当前关系.end()
                    || 关系->second.关系类型节点.值 != 类型编码
                    || 关系->second.退出事实代次) return false;
                前一编码 = 关系编码;
            }
        }
        if (已类型索引关系.size() != 值.当前关系.size()) return false;
        const auto 历史关系索引完整 = [&](const auto& 索引, bool 源端) {
            std::unordered_set<std::uint64_t> 已索引关系;
            for (const auto& [端点编码, 类型组] : 索引) {
                if (端点编码 == 0 || 类型组.empty()) return false;
                for (const auto& [类型编码, 编码组] : 类型组) {
                    if (类型编码 == 0 || 编码组.empty()) return false;
                    std::uint64_t 前一编码 = 0;
                    for (const auto 关系编码 : 编码组) {
                        const auto 当前 = 值.当前关系.find(关系编码);
                        const auto 历史 = 值.历史.find(关系编码);
                        const bool 当前命中 = 当前 != 值.当前关系.end();
                        const bool 历史命中 = 历史 != 值.历史.end();
                        const auto* 历史关系 = 历史命中
                            ? std::get_if<关系事实>(&历史->second.事实) : nullptr;
                        const 关系事实* 关系 = 当前命中 ? &当前->second : 历史关系;
                        if (关系编码 == 0 || 关系编码 <= 前一编码
                            || !已索引关系.insert(关系编码).second
                            || 当前命中 == 历史命中 || !关系
                            || 关系->编码.值 != 关系编码
                            || (源端 ? 关系->源节点.值 : 关系->目标节点.值)
                                != 端点编码
                            || 关系->关系类型节点.值 != 类型编码
                            || 关系->创建事实代次 == 0
                            || 关系->创建事实代次 > 值.事实代次
                            || (当前命中 && 关系->退出事实代次)
                            || (历史命中 && (!关系->退出事实代次
                                || 关系->创建事实代次 > *关系->退出事实代次
                                || *关系->退出事实代次 > 值.事实代次))) return false;
                        前一编码 = 关系编码;
                    }
                }
            }
            std::size_t 权威关系数 = 值.当前关系.size();
            for (const auto& [_, 历史] : 值.历史)
                if (std::holds_alternative<关系事实>(历史.事实)) ++权威关系数;
            return 已索引关系.size() == 权威关系数;
        };
        if (!历史关系索引完整(值.历史源关系候选索引, true)
            || !历史关系索引完整(值.历史目标关系候选索引, false)) return false;
        for (const auto& [编码, 事实] : 值.当前值) {
            if (!插入(编码) || 事实.编码.值 != 编码 || 事实.创建事实代次 == 0
                || 事实.创建事实代次 > 值.事实代次 || 事实.退出事实代次
                || !有效(事实.所属节点) || !有效(事实.属性类型节点) || !有效(事实.来源节点)
                || !值.当前节点.contains(事实.所属节点.值)
                || !值.当前节点.contains(事实.来源节点.值)
                || !有效(事实.写入所有者)
                || !值.当前所有者.contains(事实.写入所有者.编码.值)
                || 值.当前节点.at(事实.所属节点.值).写入所有者
                    != 事实.写入所有者) return false;
            const auto 类型 = 值.当前节点.find(事实.属性类型节点.值);
            if (类型 == 值.当前节点.end() || 类型->second.种类 != 节点种类::属性类型
                || 类型->second.写入所有者 != 事实.写入所有者
                || !类型->second.属性类型表示
                || !表示匹配(*类型->second.属性类型表示, 事实.材料)) return false;
            if (const auto* 引用 = std::get_if<独立材料引用>(&事实.材料); 引用 && !值.当前节点.contains(引用->编码.值)) return false;
        }
        for (const auto& [节点编码, 编码组] : 值.当前节点引用索引) {
            if (节点编码 == 0 || !值.当前节点.contains(节点编码) || 编码组.empty())
                return false;
            std::uint64_t 前一编码 = 0;
            for (const auto 事实编码 : 编码组) {
                const auto 关系 = 值.当前关系.find(事实编码);
                const auto 当前值 = 值.当前值.find(事实编码);
                const bool 关系命中 = 关系 != 值.当前关系.end();
                const bool 值命中 = 当前值 != 值.当前值.end();
                if (事实编码 == 0 || 事实编码 <= 前一编码
                    || 关系命中 == 值命中 || 值.历史.contains(事实编码)) return false;
                bool 真实引用 = false;
                if (关系命中) {
                    const auto& 事实 = 关系->second;
                    真实引用 = 事实.编码.值 == 事实编码 && !事实.退出事实代次
                        && (事实.源节点.值 == 节点编码
                            || 事实.目标节点.值 == 节点编码
                            || 事实.关系类型节点.值 == 节点编码);
                } else {
                    const auto& 事实 = 当前值->second;
                    const auto* 材料引用 = std::get_if<独立材料引用>(&事实.材料);
                    真实引用 = 事实.编码.值 == 事实编码 && !事实.退出事实代次
                        && (事实.所属节点.值 == 节点编码
                            || 事实.属性类型节点.值 == 节点编码
                            || 事实.来源节点.值 == 节点编码
                            || (材料引用 && 材料引用->编码.值 == 节点编码));
                }
                if (!真实引用) return false;
                前一编码 = 事实编码;
            }
        }
        for (const auto& [关系编码, 事实] : 值.当前关系) {
            std::uint64_t 节点组[3]{
                事实.源节点.值, 事实.目标节点.值, 事实.关系类型节点.值};
            std::sort(节点组, 节点组 + 3);
            std::size_t 数量 = 0;
            for (std::size_t 索引 = 0; 索引 < 3; ++索引)
                if (数量 == 0 || 节点组[数量 - 1] != 节点组[索引])
                    节点组[数量++] = 节点组[索引];
            for (std::size_t 索引 = 0; 索引 < 数量; ++索引) {
                const auto 节点 = 值.当前节点引用索引.find(节点组[索引]);
                if (节点 == 值.当前节点引用索引.end()
                    || !std::binary_search(节点->second.begin(), 节点->second.end(),
                        关系编码)) return false;
            }
        }
        for (const auto& [值编码, 事实] : 值.当前值) {
            std::uint64_t 节点组[4]{
                事实.所属节点.值, 事实.属性类型节点.值, 事实.来源节点.值, 0};
            std::size_t 原数量 = 3;
            if (const auto* 引用 = std::get_if<独立材料引用>(&事实.材料))
                节点组[原数量++] = 引用->编码.值;
            std::sort(节点组, 节点组 + 原数量);
            std::size_t 数量 = 0;
            for (std::size_t 索引 = 0; 索引 < 原数量; ++索引)
                if (数量 == 0 || 节点组[数量 - 1] != 节点组[索引])
                    节点组[数量++] = 节点组[索引];
            for (std::size_t 索引 = 0; 索引 < 数量; ++索引) {
                const auto 节点 = 值.当前节点引用索引.find(节点组[索引]);
                if (节点 == 值.当前节点引用索引.end()
                    || !std::binary_search(节点->second.begin(), 节点->second.end(),
                        值编码)) return false;
            }
        }
        for (const auto& [编码, 节点] : 值.当前节点) for (const auto& 槽 : 节点.当前属性) {
            const auto 类型 = 值.当前节点.find(槽.属性类型节点.值);
            const auto 当前值 = 值.当前值.find(槽.当前值.值);
            if (类型 == 值.当前节点.end() || 类型->second.种类 != 节点种类::属性类型
                || 当前值 == 值.当前值.end() || 当前值->second.所属节点.值 != 编码
                || 当前值->second.属性类型节点 != 槽.属性类型节点
                || 节点.写入所有者 != 类型->second.写入所有者
                || 节点.写入所有者 != 当前值->second.写入所有者) return false;
        }
        std::unordered_set<std::uint64_t> 节点全集;
        for (const auto& [编码, 事实] : 值.当前节点) 节点全集.insert(编码);
        const auto 所有者覆盖创建 = [&](L1结构所有者身份 所有者,
            std::uint64_t 创建代次) {
            if (!有效(所有者) || 创建代次 == 0) return false;
            if (const auto 当前 = 值.当前所有者.find(所有者.编码.值);
                当前 != 值.当前所有者.end())
                return 当前->second.创建事实代次 <= 创建代次;
            if (const auto 历史 = 值.历史所有者.find(所有者.编码.值);
                历史 != 值.历史所有者.end())
                return 历史->second.创建事实代次 <= 创建代次
                    && 历史->second.退出事实代次
                    && 创建代次 <= *历史->second.退出事实代次;
            return false;
        };
        for (const auto& [编码, 历史] : 值.历史) {
            const auto 历史编码 = std::visit([](const auto& 事实) { return 事实.编码.值; }, 历史.事实);
            if (编码 == 0 || 全部.contains(编码) || 历史编码 != 编码) return false;
            全部.insert(编码);
            bool 有效历史 = std::visit([&](const auto& 事实) {
                if (!事实.退出事实代次 || 事实.创建事实代次 == 0
                    || 事实.创建事实代次 > *事实.退出事实代次
                    || *事实.退出事实代次 > 值.事实代次 || 事实.编码.值 != 编码
                    || !所有者覆盖创建(事实.写入所有者,
                        事实.创建事实代次)) return false;
                using T = std::decay_t<decltype(事实)>;
                if constexpr (std::is_same_v<T, 节点事实>) { 节点全集.insert(编码); return true; }
                else return true;
            }, 历史.事实);
            if (!有效历史) return false;
        }
        auto 节点存在 = [&](稳定编码 编码) { return 有效(编码) && 节点全集.contains(编码.值); };
        const auto 查节点事实 = [&](稳定编码 编码) -> const 节点事实* {
            if (const auto 当前 = 值.当前节点.find(编码.值);
                当前 != 值.当前节点.end()) return &当前->second;
            if (const auto 历史 = 值.历史.find(编码.值);
                历史 != 值.历史.end())
                return std::get_if<节点事实>(&历史->second.事实);
            return nullptr;
        };
        const auto 查值事实 = [&](稳定编码 编码) -> const 值事实* {
            if (const auto 当前 = 值.当前值.find(编码.值);
                当前 != 值.当前值.end()) return &当前->second;
            if (const auto 历史 = 值.历史.find(编码.值);
                历史 != 值.历史.end())
                return std::get_if<值事实>(&历史->second.事实);
            return nullptr;
        };
        for (const auto& [_, 关系历史] : 值.历史) {
            if (const auto* 关系 = std::get_if<关系事实>(&关系历史.事实)) {
                const auto* 类型 = 查节点事实(关系->关系类型节点);
                if (!节点存在(关系->源节点) || !节点存在(关系->目标节点)
                    || !类型 || 类型->写入所有者 != 关系->写入所有者)
                    return false;
            }
            if (const auto* 事实 = std::get_if<值事实>(&关系历史.事实)) {
                const auto* 所属 = 查节点事实(事实->所属节点);
                const auto* 类型 = 查节点事实(事实->属性类型节点);
                if (!所属 || !类型 || !节点存在(事实->来源节点)
                    || 所属->写入所有者 != 事实->写入所有者
                    || 类型->写入所有者 != 事实->写入所有者
                    || 类型->种类 != 节点种类::属性类型
                    || !类型->属性类型表示
                    || !表示匹配(*类型->属性类型表示, 事实->材料)) return false;
            }
            if (const auto* 节点 = std::get_if<节点事实>(&关系历史.事实))
                for (const auto& 槽 : 节点->当前属性) {
                    const auto* 类型 = 查节点事实(槽.属性类型节点);
                    const auto* 当前值 = 查值事实(槽.当前值);
                    if (!类型 || !当前值
                        || 类型->写入所有者 != 节点->写入所有者
                        || 当前值->写入所有者 != 节点->写入所有者
                        || 当前值->所属节点 != 节点->编码
                        || 当前值->属性类型节点 != 槽.属性类型节点)
                        return false;
                }
        }
        for (const auto 编码 : 全部) if (!值.永久占用.contains(编码)) return false;
        if (值.永久占用.size() != 全部.size()) return false;
        for (const auto 编码 : 值.永久占用) if (编码 == 0 || !全部.contains(编码)) return false;
        std::uint64_t 最大编码 = 0;
        for (const auto 编码 : 值.永久占用) 最大编码 = std::max(最大编码, 编码);
        if (值.下个编码 == 0 || 值.下个编码 <= 最大编码) return false;
        struct 期望历史属性候选 final {
            std::uint64_t 所属节点 = 0;
            std::uint64_t 属性类型 = 0;
            std::uint64_t 创建事实代次 = 0;
        };
        std::unordered_map<std::uint64_t, 期望历史属性候选> 期望属性候选;
        for (const auto& [键, 账] : 值.中性幂等账) {
            if (键 == 0 || 账.首次规范化写集.合同版本 != L1中性CRUD合同版本
                || 账.首次规范化写集.幂等键.值 != 键
                || 账.首次发布事实代次 == 0 || 账.首次发布事实代次 > 值.事实代次
                || 规范化中性写集(账.首次规范化写集)
                    != std::optional<L1中性写集请求>(账.首次规范化写集)
                || 账.首次结果.状态 != L1中性写入状态::成功
                || 账.首次结果.合同版本 != L1中性CRUD合同版本
                || 账.首次结果.幂等键.值 != 键
                || 账.首次结果.事实代次 != 账.首次发布事实代次
                || !账.首次结果.是否形成内存权威发布
                || 账.首次结果.重试边界 != L1中性重试边界::不适用
                || 账.首次结果.新编码映射 != 账.首次新编码映射)
                return false;
            std::unordered_set<std::uint32_t> 本地;
            if (!中性请求结构有效(账.首次规范化写集, 本地)
                || 账.首次新编码映射.size() != 本地.size()) return false;
            for (std::size_t i = 0; i < 账.首次新编码映射.size(); ++i) {
                const auto& [本地键, 编码] = 账.首次新编码映射[i];
                if (!本地.contains(本地键.值) || !全部.contains(编码.值)
                    || (i != 0 && !(账.首次新编码映射[i - 1].first < 本地键))) return false;
                bool 创建匹配 = false;
                if (const auto it = 值.当前节点.find(编码.值); it != 值.当前节点.end())
                    创建匹配 = it->second.创建事实代次 == 账.首次发布事实代次;
                if (const auto it = 值.当前关系.find(编码.值); it != 值.当前关系.end())
                    创建匹配 = it->second.创建事实代次 == 账.首次发布事实代次;
                if (const auto it = 值.当前值.find(编码.值); it != 值.当前值.end())
                    创建匹配 = it->second.创建事实代次 == 账.首次发布事实代次;
                if (const auto it = 值.历史.find(编码.值); it != 值.历史.end())
                    创建匹配 = std::visit([&](const auto& 事实) {
                        return 事实.创建事实代次 == 账.首次发布事实代次;
                    }, it->second.事实);
                if (const auto it = 值.物理清理墓碑.find(编码.值);
                    it != 值.物理清理墓碑.end())
                    创建匹配 = it->second.创建事实代次
                        == 账.首次发布事实代次;
                if (!创建匹配) return false;
            }
            const auto 查映射 = [&](L1中性写集本地键 本地键)
                -> std::optional<稳定编码> {
                const auto 位置 = std::lower_bound(账.首次新编码映射.begin(),
                    账.首次新编码映射.end(), 本地键,
                    [](const auto& 项, L1中性写集本地键 键值) {
                        return 项.first < 键值;
                    });
                return 位置 != 账.首次新编码映射.end() && 位置->first == 本地键
                    ? std::optional<稳定编码>{位置->second} : std::nullopt;
            };
            const auto 解析 = [&](const L1中性事实引用& 引用)
                -> std::optional<稳定编码> {
                return std::visit([&](const auto& 项) -> std::optional<稳定编码> {
                    using 类型 = std::decay_t<decltype(项)>;
                    if constexpr (std::is_same_v<类型, 稳定编码>) return 项;
                    else return 查映射(项);
                }, 引用);
            };
            for (const auto& 槽变更 : 账.首次规范化写集.属性槽变更) {
                const auto 所属 = 解析(槽变更.所属节点);
                const auto 类型 = 解析(槽变更.属性类型节点);
                const auto 值编码 = 查映射(槽变更.新当前值);
                if (!所属 || !类型 || !值编码) return false;
                const auto 当前 = 值.当前值.find(值编码->值);
                const auto 历史 = 值.历史.find(值编码->值);
                const auto 墓碑 = 值.物理清理墓碑.find(值编码->值);
                if (当前 == 值.当前值.end() && 历史 == 值.历史.end()
                    && 墓碑 != 值.物理清理墓碑.end()
                    && 墓碑->second.事实种类 == L1物理清理事实种类::值
                    && 墓碑->second.创建事实代次 == 账.首次发布事实代次)
                    continue;
                const bool 当前命中 = 当前 != 值.当前值.end();
                const bool 历史命中 = 历史 != 值.历史.end();
                const auto* 历史值 = 历史命中
                    ? std::get_if<值事实>(&历史->second.事实) : nullptr;
                const 值事实* 事实 = 当前命中 ? &当前->second : 历史值;
                if (当前命中 == 历史命中 || !事实
                    || 事实->编码 != *值编码 || 事实->所属节点 != *所属
                    || 事实->属性类型节点 != *类型
                    || 事实->创建事实代次 != 账.首次发布事实代次
                    || (当前命中 && 事实->退出事实代次)
                    || (历史命中 && (历史->second.查询编码 != *值编码
                        || 历史->second.当前有效 || !事实->退出事实代次)))
                    return false;
                if (!期望属性候选.emplace(值编码->值,
                        期望历史属性候选{所属->值, 类型->值,
                            账.首次发布事实代次}).second)
                    return false;
            }
        }
        for (const auto& [所有者编码, 账组] : 值.所有者范围幂等账) {
            for (const auto& [_, 账] : 账组) {
                const auto 查映射 = [&](L1中性写集本地键 本地键)
                    -> std::optional<稳定编码> {
                    const auto 位置 = std::lower_bound(账.首次新编码映射.begin(),
                        账.首次新编码映射.end(), 本地键,
                        [](const auto& 项, L1中性写集本地键 键值) {
                            return 项.first < 键值;
                        });
                    return 位置 != 账.首次新编码映射.end()
                        && 位置->first == 本地键
                        ? std::optional<稳定编码>{位置->second} : std::nullopt;
                };
                const auto 解析 = [&](const L1中性事实引用& 引用)
                    -> std::optional<稳定编码> {
                    return std::visit([&](const auto& 项) -> std::optional<稳定编码> {
                        using 类型 = std::decay_t<decltype(项)>;
                        if constexpr (std::is_same_v<类型, 稳定编码>) return 项;
                        else return 查映射(项);
                    }, 引用);
                };
                for (const auto& 槽变更 : 账.首次规范化写集.属性槽变更) {
                    const auto 所属 = 解析(槽变更.所属节点);
                    const auto 类型 = 解析(槽变更.属性类型节点);
                    const auto 值编码 = 查映射(槽变更.新当前值);
                    if (!所属 || !类型 || !值编码) return false;
                    const auto 当前 = 值.当前值.find(值编码->值);
                    const auto 历史 = 值.历史.find(值编码->值);
                    const auto 墓碑 = 值.物理清理墓碑.find(值编码->值);
                    if (当前 == 值.当前值.end() && 历史 == 值.历史.end()
                        && 墓碑 != 值.物理清理墓碑.end()
                        && 墓碑->second.事实种类 == L1物理清理事实种类::值
                        && 墓碑->second.内部结构分区.值 == 所有者编码
                        && 墓碑->second.创建事实代次 == 账.首次发布事实代次)
                        continue;
                    const bool 当前命中 = 当前 != 值.当前值.end();
                    const bool 历史命中 = 历史 != 值.历史.end();
                    const auto* 历史值 = 历史命中
                        ? std::get_if<值事实>(&历史->second.事实) : nullptr;
                    const 值事实* 事实 = 当前命中 ? &当前->second : 历史值;
                    if (当前命中 == 历史命中 || !事实
                        || 事实->编码 != *值编码 || 事实->所属节点 != *所属
                        || 事实->属性类型节点 != *类型
                        || 事实->写入所有者.编码.值 != 所有者编码
                        || 事实->创建事实代次 != 账.首次发布事实代次)
                        return false;
                    if (!期望属性候选.emplace(值编码->值,
                            期望历史属性候选{所属->值, 类型->值,
                                账.首次发布事实代次}).second)
                        return false;
                }
            }
        }
        std::unordered_set<std::uint64_t> 已登记属性候选;
        for (const auto& [所属编码, 类型组] : 值.历史属性槽值候选索引) {
            if (所属编码 == 0 || 类型组.empty()) return false;
            for (const auto& [类型编码, 编码组] : 类型组) {
                if (类型编码 == 0 || 编码组.empty()) return false;
                std::uint64_t 前一编码 = 0;
                for (const auto 值编码 : 编码组) {
                    const auto 当前 = 值.当前值.find(值编码);
                    const auto 历史 = 值.历史.find(值编码);
                    const bool 当前命中 = 当前 != 值.当前值.end();
                    const bool 历史命中 = 历史 != 值.历史.end();
                    const auto* 历史值 = 历史命中
                        ? std::get_if<值事实>(&历史->second.事实) : nullptr;
                    const 值事实* 事实 = 当前命中 ? &当前->second : 历史值;
                    const auto 期望 = 期望属性候选.find(值编码);
                    if (值编码 == 0 || 值编码 <= 前一编码
                        || !已登记属性候选.insert(值编码).second
                        || 当前命中 == 历史命中 || !事实
                        || 事实->编码.值 != 值编码 || 事实->所属节点.值 != 所属编码
                        || 事实->属性类型节点.值 != 类型编码
                        || 事实->创建事实代次 == 0
                        || 事实->创建事实代次 > 值.事实代次
                        || (当前命中 && 事实->退出事实代次)
                        || (历史命中 && (历史->second.查询编码.值 != 值编码
                            || 历史->second.当前有效 || !事实->退出事实代次
                            || 事实->创建事实代次 > *事实->退出事实代次
                            || *事实->退出事实代次 > 值.事实代次))
                        || 期望 == 期望属性候选.end()
                        || 期望->second.所属节点 != 所属编码
                        || 期望->second.属性类型 != 类型编码
                        || 期望->second.创建事实代次 != 事实->创建事实代次)
                        return false;
                    前一编码 = 值编码;
                }
            }
        }
        if (已登记属性候选.size() != 期望属性候选.size()) return false;
        for (const auto& [编码, 节点] : 值.当前节点) {
            for (const auto& 槽 : 节点.当前属性) {
                const auto 所属 = 值.历史属性槽值候选索引.find(编码);
                if (所属 == 值.历史属性槽值候选索引.end()) return false;
                const auto 类型 = 所属->second.find(槽.属性类型节点.值);
                if (类型 == 所属->second.end()
                    || !std::binary_search(类型->second.begin(), 类型->second.end(),
                        槽.当前值.值)) return false;
            }
        }
        std::unordered_set<std::uint64_t> 已建立独占所有者;
        for (const auto& [键, 账] : 值.所有者建立幂等账) {
            if (键 == 0 || 账.首次请求.合同版本 != L1所有者范围CRUD合同版本
                || 账.首次请求.建立幂等身份.值 != 键
                || 账.首次请求.范围种类 != L1所有者范围种类::独占结构范围
                || 账.首次结果.状态 != L1所有者范围管理状态::成功
                || 账.首次结果.合同版本 != L1所有者范围CRUD合同版本
                || 账.首次结果.建立幂等身份.值 != 键
                || !账.首次结果.所有者事实
                || 账.首次结果.所有者事实->范围种类
                    != L1所有者范围种类::独占结构范围
                || 账.首次结果.所有者事实->退出事实代次
                || 账.首次结果.事实代次
                    != 账.首次结果.所有者事实->创建事实代次
                || !账.首次结果.是否形成内存权威发布
                || 账.首次结果.重试边界 != L1所有者范围重试边界::不适用)
                return false;
            const auto 所有者 = 账.首次结果.所有者事实->所有者;
            if (!有效(所有者)
                || !已建立独占所有者.insert(所有者.编码.值).second)
                return false;
            if (const auto 当前 = 值.当前所有者.find(所有者.编码.值);
                当前 != 值.当前所有者.end()) {
                if (当前->second != *账.首次结果.所有者事实) return false;
            } else {
                const auto 历史 = 值.历史所有者.find(所有者.编码.值);
                if (历史 == 值.历史所有者.end()
                    || 历史->second.所有者 != 所有者
                    || 历史->second.范围种类 != L1所有者范围种类::独占结构范围
                    || 历史->second.创建事实代次
                        != 账.首次结果.所有者事实->创建事实代次)
                    return false;
            }
        }
        for (const auto& [编码, 所有者] : 值.当前所有者)
            if (所有者.范围种类 == L1所有者范围种类::独占结构范围
                && !已建立独占所有者.contains(编码)) return false;
        for (const auto& [编码, 所有者] : 值.历史所有者)
            if (所有者.范围种类 == L1所有者范围种类::独占结构范围
                && !已建立独占所有者.contains(编码)) return false;

        for (const auto& [所有者编码, 账组] : 值.所有者范围幂等账) {
            if (所有者编码 == 0 || 账组.empty()
                || (!值.当前所有者.contains(所有者编码)
                    && !值.历史所有者.contains(所有者编码))) return false;
            for (const auto& [键, 账] : 账组) {
                if (键 == 0
                    || 账.首次规范化写集.合同版本 != L1中性CRUD合同版本
                    || 账.首次规范化写集.幂等键.值 != 键
                    || 账.首次发布事实代次 == 0
                    || 账.首次发布事实代次 > 值.事实代次
                    || 规范化中性写集(账.首次规范化写集)
                        != std::optional<L1中性写集请求>(账.首次规范化写集)
                    || 账.首次结果.状态 != L1中性写入状态::成功
                    || 账.首次结果.合同版本 != L1中性CRUD合同版本
                    || 账.首次结果.幂等键.值 != 键
                    || 账.首次结果.事实代次 != 账.首次发布事实代次
                    || !账.首次结果.是否形成内存权威发布
                    || 账.首次结果.重试边界 != L1中性重试边界::不适用
                    || 账.首次结果.新编码映射 != 账.首次新编码映射)
                    return false;
                std::unordered_set<std::uint32_t> 本地;
                if (!中性请求结构有效(账.首次规范化写集, 本地)
                    || 账.首次新编码映射.size() != 本地.size()) return false;
                for (const auto& [本地键, 编码] : 账.首次新编码映射) {
                    if (!本地.contains(本地键.值)) return false;
                    bool 匹配 = false;
                    if (const auto 当前 = 值.当前节点.find(编码.值);
                        当前 != 值.当前节点.end())
                        匹配 = 当前->second.创建事实代次 == 账.首次发布事实代次
                            && 当前->second.写入所有者.编码.值 == 所有者编码;
                    if (const auto 当前 = 值.当前关系.find(编码.值);
                        当前 != 值.当前关系.end())
                        匹配 = 当前->second.创建事实代次 == 账.首次发布事实代次
                            && 当前->second.写入所有者.编码.值 == 所有者编码;
                    if (const auto 当前 = 值.当前值.find(编码.值);
                        当前 != 值.当前值.end())
                        匹配 = 当前->second.创建事实代次 == 账.首次发布事实代次
                            && 当前->second.写入所有者.编码.值 == 所有者编码;
                    if (const auto 历史 = 值.历史.find(编码.值);
                        历史 != 值.历史.end())
                        匹配 = std::visit([&](const auto& 事实) {
                            return 事实.创建事实代次 == 账.首次发布事实代次
                                && 事实.写入所有者.编码.值 == 所有者编码;
                        }, 历史->second.事实);
                    if (const auto 墓碑 = 值.物理清理墓碑.find(编码.值);
                        墓碑 != 值.物理清理墓碑.end())
                        匹配 = 墓碑->second.创建事实代次
                                == 账.首次发布事实代次
                            && 墓碑->second.内部结构分区.值 == 所有者编码;
                    if (!匹配) return false;
                }
            }
        }
        const auto 三分区引用排序键 = [](const L1三分区原子事实引用值_v2& 引用) {
            std::array<std::uint64_t, 3> 键{};
            if (const auto* 稳定项 = std::get_if<稳定编码>(&引用))
                键 = {1, 稳定项->值, 0};
            else if (const auto* 本地项 =
                std::get_if<L1所有者范围写集本地键>(&引用))
                键 = {2, 本地项->值, 0};
            else {
                const auto& 跨项 = std::get<L1三分区原子事实引用_v2>(引用);
                键 = {3, 跨项.参与者.值, 跨项.本地键.值};
            }
            return 键;
        };
        for (const auto& [键, 账] : 值.三分区原子事务幂等账_v2) {
            const auto& 请求 = 账.请求;
            const auto& 结果 = 账.结果;
            if (键 == 0 || 请求.合同版本 != L1三分区原子事务合同版本_v2
                || 请求.组合写入幂等身份.值 != 键
                || 请求.共同期望事实代次 == 0
                || 请求.参与者写集组.size() != 3
                || 结果.状态 != L1三分区原子事务状态_v2::已提交
                || 结果.合同版本 != L1三分区原子事务合同版本_v2
                || 结果.组合写入幂等身份 != 请求.组合写入幂等身份
                || 结果.共同事实代次 != 请求.共同期望事实代次 + 1
                || 结果.共同事实代次 == 0 || 结果.共同事实代次 > 值.事实代次
                || !结果.是否已确认形成内存权威发布
                || 结果.重试边界 != L1所有者范围重试边界::不适用
                || 结果.参与者结果组.size() != 3)
                return false;
            std::unordered_set<std::uint64_t> 所有者组;
            std::unordered_set<std::uint64_t> 幂等组;
            std::array<std::unordered_set<std::uint32_t>, 3> 节点键组;
            std::array<std::unordered_set<std::uint32_t>, 3> 值键组;
            for (std::size_t 序号 = 0; 序号 != 3; ++序号) {
                const auto& 参与者 = 请求.参与者写集组[序号];
                const auto& 参与者结果 = 结果.参与者结果组[序号];
                const auto& 写集 = 参与者.写集;
                if (参与者.参与者.值 != 序号 + 1
                    || 参与者结果.参与者 != 参与者.参与者
                    || 参与者结果.所有者 != 参与者.所有者
                    || !有效(参与者.所有者)
                    || !所有者组.insert(参与者.所有者.编码.值).second
                    || 写集.合同版本 != L1所有者范围CRUD合同版本
                    || 写集.期望事实代次 != 请求.共同期望事实代次
                    || !有效(写集.写入幂等身份)
                    || !幂等组.insert(写集.写入幂等身份.值).second
                    || (写集.节点.empty() && 写集.关系.empty()
                        && 写集.值.empty() && 写集.属性槽变更.empty()
                        && 写集.退出事实.empty()))
                    return false;
                std::unordered_set<std::uint32_t> 全部本地键;
                std::uint32_t 前一本地键 = 0;
                for (const auto& 项 : 写集.节点) {
                    if (项.本地键.值 == 0 || 项.本地键.值 <= 前一本地键
                        || !全部本地键.insert(项.本地键.值).second
                        || !节点键组[序号].insert(项.本地键.值).second)
                        return false;
                    前一本地键 = 项.本地键.值;
                }
                前一本地键 = 0;
                for (const auto& 项 : 写集.关系) {
                    if (项.本地键.值 == 0 || 项.本地键.值 <= 前一本地键
                        || !全部本地键.insert(项.本地键.值).second) return false;
                    前一本地键 = 项.本地键.值;
                }
                前一本地键 = 0;
                for (const auto& 项 : 写集.值) {
                    if (项.本地键.值 == 0 || 项.本地键.值 <= 前一本地键
                        || !全部本地键.insert(项.本地键.值).second
                        || !值键组[序号].insert(项.本地键.值).second) return false;
                    前一本地键 = 项.本地键.值;
                }
                std::uint64_t 前一退出 = 0;
                for (const auto 退出 : 写集.退出事实) {
                    if (!有效(退出) || 退出.值 <= 前一退出) return false;
                    前一退出 = 退出.值;
                }
                for (std::size_t i = 1; i < 写集.属性槽变更.size(); ++i) {
                    const auto& 前 = 写集.属性槽变更[i - 1];
                    const auto& 后 = 写集.属性槽变更[i];
                    const auto 前所属 = 三分区引用排序键(前.所属节点);
                    const auto 后所属 = 三分区引用排序键(后.所属节点);
                    const auto 前类型 = 三分区引用排序键(前.属性类型节点);
                    const auto 后类型 = 三分区引用排序键(后.属性类型节点);
                    if ((前所属 == 后所属 && 前类型 == 后类型)
                        || !(std::tuple{前所属, 前类型, 前.新当前值.值}
                            < std::tuple{后所属, 后类型, 后.新当前值.值}))
                        return false;
                }
                if (参与者结果.新编码映射.size()
                    != 写集.节点.size() + 写集.关系.size() + 写集.值.size())
                    return false;
                const auto owner账组 = 值.所有者范围幂等账.find(
                    参与者.所有者.编码.值);
                if (owner账组 == 值.所有者范围幂等账.end()) return false;
                const auto owner账 = owner账组->second.find(写集.写入幂等身份.值);
                if (owner账 == owner账组->second.end()
                    || owner账->second.首次发布事实代次 != 结果.共同事实代次
                    || owner账->second.首次结果.事实代次 != 结果.共同事实代次
                    || owner账->second.首次新编码映射.size()
                        != 参与者结果.新编码映射.size())
                    return false;
                for (std::size_t i = 0;
                    i < 参与者结果.新编码映射.size(); ++i)
                    if (owner账->second.首次新编码映射[i].first.值
                            != 参与者结果.新编码映射[i].first.值
                        || owner账->second.首次新编码映射[i].second
                            != 参与者结果.新编码映射[i].second)
                        return false;
                std::uint32_t 前一映射键 = 0;
                for (const auto& [本地键, 编码] : 参与者结果.新编码映射) {
                    if (本地键.值 == 0 || 本地键.值 <= 前一映射键
                        || !全部本地键.contains(本地键.值) || !有效(编码))
                        return false;
                    前一映射键 = 本地键.值;
                }
            }
            const auto 引用有效 = [&](const L1三分区原子事实引用值_v2& 引用,
                std::size_t 当前序号) {
                if (const auto* 稳定项 = std::get_if<稳定编码>(&引用))
                    return 有效(*稳定项);
                if (const auto* 本地项 =
                    std::get_if<L1所有者范围写集本地键>(&引用))
                    return 本地项->值 != 0
                        && 节点键组[当前序号].contains(本地项->值);
                const auto& 跨项 = std::get<L1三分区原子事实引用_v2>(引用);
                return 跨项.参与者.值 >= 1
                    && 跨项.参与者.值 < 当前序号 + 1
                    && 节点键组[跨项.参与者.值 - 1].contains(跨项.本地键.值);
            };
            for (std::size_t 序号 = 0; 序号 != 3; ++序号) {
                const auto& 写集 = 请求.参与者写集组[序号].写集;
                for (const auto& 项 : 写集.关系)
                    if (!引用有效(项.源节点, 序号)
                        || !引用有效(项.目标节点, 序号)
                        || !引用有效(项.关系类型节点, 序号)) return false;
                for (const auto& 项 : 写集.值)
                    if (!引用有效(项.所属节点, 序号)
                        || !引用有效(项.属性类型节点, 序号)
                        || !引用有效(项.来源节点, 序号)) return false;
                for (const auto& 项 : 写集.属性槽变更)
                    if (!引用有效(项.所属节点, 序号)
                        || !引用有效(项.属性类型节点, 序号)
                        || !值键组[序号].contains(项.新当前值.值)) return false;
            }
            const auto 转换首次账写集 = [&](const L1三分区原子写集请求_v2& 原,
                std::size_t 当前序号) -> std::optional<L1中性写集请求> {
                const auto 查映射 = [&](std::size_t 参与者序号,
                    L1所有者范围写集本地键 本地键) -> std::optional<稳定编码> {
                    const auto& 映射 = 结果.参与者结果组[参与者序号].新编码映射;
                    const auto 位置 = std::lower_bound(映射.begin(), 映射.end(),
                        本地键, [](const auto& 项, const auto 键值) {
                            return 项.first < 键值;
                        });
                    return 位置 != 映射.end() && 位置->first == 本地键
                        ? std::optional<稳定编码>{位置->second} : std::nullopt;
                };
                const auto 转换引用 = [&](const L1三分区原子事实引用值_v2& 引用)
                    -> std::optional<L1中性事实引用> {
                    if (const auto* 稳定项 = std::get_if<稳定编码>(&引用))
                        return *稳定项;
                    if (const auto* 本地项 =
                        std::get_if<L1所有者范围写集本地键>(&引用))
                        return L1中性写集本地键{本地项->值};
                    const auto& 跨项 = std::get<L1三分区原子事实引用_v2>(引用);
                    if (跨项.参与者.值 == 0
                        || 跨项.参与者.值 > 当前序号)
                        return std::nullopt;
                    const auto 编码 = 查映射(
                        跨项.参与者.值 - 1, 跨项.本地键);
                    return 编码
                        ? std::optional<L1中性事实引用>{*编码}
                        : std::nullopt;
                };
                const auto 转换表示 = [](std::optional<L1所有者范围值表示种类> 表示)
                    -> std::optional<L1中性值表示种类> {
                    if (!表示) return std::nullopt;
                    return static_cast<L1中性值表示种类>(
                        static_cast<std::uint8_t>(*表示));
                };
                const auto 转换材料 = [](const L1所有者范围原始值材料& 材料)
                    -> L1中性原始值材料 {
                    return std::visit([](const auto& 值) -> L1中性原始值材料 {
                        using 类型 = std::decay_t<decltype(值)>;
                        if constexpr (std::is_same_v<类型,
                            L1所有者范围独立材料引用>)
                            return L1中性独立材料引用{值.编码};
                        else return 值;
                    }, 材料);
                };
                L1中性写集请求 预期;
                预期.合同版本 = L1中性CRUD合同版本;
                预期.期望事实代次 = 原.期望事实代次;
                预期.幂等键 = {原.写入幂等身份.值};
                预期.退出事实 = 原.退出事实;
                for (const auto& 项 : 原.节点)
                    预期.节点.push_back({{项.本地键.值}, 项.种类,
                        转换表示(项.属性类型表示)});
                for (const auto& 项 : 原.关系) {
                    const auto 源 = 转换引用(项.源节点);
                    const auto 目标 = 转换引用(项.目标节点);
                    const auto 类型 = 转换引用(项.关系类型节点);
                    if (!源 || !目标 || !类型) return std::nullopt;
                    预期.关系.push_back({{项.本地键.值}, *源, *目标, *类型,
                        项.角色或顺序});
                }
                for (const auto& 项 : 原.值) {
                    const auto 所属 = 转换引用(项.所属节点);
                    const auto 类型 = 转换引用(项.属性类型节点);
                    const auto 来源 = 转换引用(项.来源节点);
                    if (!所属 || !类型 || !来源) return std::nullopt;
                    预期.值.push_back({{项.本地键.值}, *所属, *类型,
                        转换材料(项.材料), *来源});
                }
                for (const auto& 项 : 原.属性槽变更) {
                    const auto 所属 = 转换引用(项.所属节点);
                    const auto 类型 = 转换引用(项.属性类型节点);
                    if (!所属 || !类型) return std::nullopt;
                    预期.属性槽变更.push_back(
                        {*所属, *类型, {项.新当前值.值}});
                }
                const auto 引用排序键 = [](const L1中性事实引用& 引用) {
                    if (const auto* 稳定项 = std::get_if<稳定编码>(&引用))
                        return 稳定项->值;
                    return (1ULL << 63)
                        | std::get<L1中性写集本地键>(引用).值;
                };
                std::sort(预期.属性槽变更.begin(), 预期.属性槽变更.end(),
                    [&](const auto& 左, const auto& 右) {
                        const auto 左所属 = 引用排序键(左.所属节点);
                        const auto 右所属 = 引用排序键(右.所属节点);
                        if (左所属 != 右所属) return 左所属 < 右所属;
                        const auto 左类型 = 引用排序键(左.属性类型节点);
                        const auto 右类型 = 引用排序键(右.属性类型节点);
                        if (左类型 != 右类型) return 左类型 < 右类型;
                        return 左.新当前值 < 右.新当前值;
                    });
                return 预期;
            };
            for (std::size_t 序号 = 0; 序号 != 3; ++序号) {
                const auto& 参与者 = 请求.参与者写集组[序号];
                const auto 预期 = 转换首次账写集(参与者.写集, 序号);
                const auto owner账组 = 值.所有者范围幂等账.find(
                    参与者.所有者.编码.值);
                if (!预期 || owner账组 == 值.所有者范围幂等账.end())
                    return false;
                const auto owner账 = owner账组->second.find(
                    参与者.写集.写入幂等身份.值);
                if (owner账 == owner账组->second.end()
                    || !(owner账->second.首次规范化写集 == *预期))
                    return false;
            }
        }
        for (const auto& [键, 账] : 值.有限N分区原子事务幂等账_v3) {
            const auto& 请求 = 账.请求;
            const auto& 结果 = 账.结果;
            const auto 参与者数 = 请求.参与者写集组.size();
            if (键 == 0 || 请求.合同版本 != L1有限N分区原子事务合同版本_v3
                || 请求.组合写入幂等身份.值 != 键
                || 请求.共同期望事实代次 == 0
                || 参与者数 < L1有限N分区原子事务最小参与者数_v3
                || 参与者数 > L1有限N分区原子事务最大参与者数_v3
                || 结果.状态 != L1有限N分区原子事务状态_v3::已提交
                || 结果.合同版本 != L1有限N分区原子事务合同版本_v3
                || 结果.组合写入幂等身份 != 请求.组合写入幂等身份
                || 结果.共同事实代次 != 请求.共同期望事实代次 + 1
                || 结果.共同事实代次 == 0 || 结果.共同事实代次 > 值.事实代次
                || !结果.是否已确认形成内存权威发布
                || 结果.重试边界 != L1所有者范围重试边界::不适用
                || 结果.参与者结果组.size() != 参与者数)
                return false;
            std::unordered_set<std::uint64_t> 所有者组;
            std::unordered_set<std::uint64_t> 幂等组;
            std::vector<std::unordered_set<std::uint32_t>> 节点键组(参与者数);
            std::vector<std::unordered_set<std::uint32_t>> 值键组(参与者数);
            for (std::size_t 序号 = 0; 序号 != 参与者数; ++序号) {
                const auto& 参与者 = 请求.参与者写集组[序号];
                const auto& 参与者结果 = 结果.参与者结果组[序号];
                const auto& 写集 = 参与者.写集;
                if (参与者.参与者.值 != 序号 + 1
                    || 参与者结果.参与者 != 参与者.参与者
                    || 参与者结果.所有者 != 参与者.所有者
                    || !有效(参与者.所有者)
                    || !所有者组.insert(参与者.所有者.编码.值).second
                    || 写集.合同版本 != L1所有者范围CRUD合同版本
                    || 写集.期望事实代次 != 请求.共同期望事实代次
                    || !有效(写集.写入幂等身份)
                    || !幂等组.insert(写集.写入幂等身份.值).second
                    || (写集.节点.empty() && 写集.关系.empty()
                        && 写集.值.empty() && 写集.属性槽变更.empty()
                        && 写集.退出事实.empty()))
                    return false;
                std::unordered_set<std::uint32_t> 全部本地键;
                std::uint32_t 前一本地键 = 0;
                for (const auto& 项 : 写集.节点) {
                    if (项.本地键.值 == 0 || 项.本地键.值 <= 前一本地键
                        || !全部本地键.insert(项.本地键.值).second
                        || !节点键组[序号].insert(项.本地键.值).second)
                        return false;
                    前一本地键 = 项.本地键.值;
                }
                前一本地键 = 0;
                for (const auto& 项 : 写集.关系) {
                    if (项.本地键.值 == 0 || 项.本地键.值 <= 前一本地键
                        || !全部本地键.insert(项.本地键.值).second) return false;
                    前一本地键 = 项.本地键.值;
                }
                前一本地键 = 0;
                for (const auto& 项 : 写集.值) {
                    if (项.本地键.值 == 0 || 项.本地键.值 <= 前一本地键
                        || !全部本地键.insert(项.本地键.值).second
                        || !值键组[序号].insert(项.本地键.值).second) return false;
                    前一本地键 = 项.本地键.值;
                }
                std::uint64_t 前一退出 = 0;
                for (const auto 退出 : 写集.退出事实) {
                    if (!有效(退出) || 退出.值 <= 前一退出) return false;
                    前一退出 = 退出.值;
                }
                for (std::size_t i = 1; i < 写集.属性槽变更.size(); ++i) {
                    const auto& 前 = 写集.属性槽变更[i - 1];
                    const auto& 后 = 写集.属性槽变更[i];
                    const auto 前所属 = 三分区引用排序键(前.所属节点);
                    const auto 后所属 = 三分区引用排序键(后.所属节点);
                    const auto 前类型 = 三分区引用排序键(前.属性类型节点);
                    const auto 后类型 = 三分区引用排序键(后.属性类型节点);
                    if ((前所属 == 后所属 && 前类型 == 后类型)
                        || !(std::tuple{前所属, 前类型, 前.新当前值.值}
                            < std::tuple{后所属, 后类型, 后.新当前值.值}))
                        return false;
                }
                if (参与者结果.新编码映射.size()
                    != 写集.节点.size() + 写集.关系.size() + 写集.值.size())
                    return false;
                const auto owner账组 = 值.所有者范围幂等账.find(
                    参与者.所有者.编码.值);
                if (owner账组 == 值.所有者范围幂等账.end()) return false;
                const auto owner账 = owner账组->second.find(写集.写入幂等身份.值);
                if (owner账 == owner账组->second.end()
                    || owner账->second.首次发布事实代次 != 结果.共同事实代次
                    || owner账->second.首次结果.事实代次 != 结果.共同事实代次
                    || owner账->second.首次新编码映射.size()
                        != 参与者结果.新编码映射.size())
                    return false;
                for (std::size_t i = 0;
                    i < 参与者结果.新编码映射.size(); ++i)
                    if (owner账->second.首次新编码映射[i].first.值
                            != 参与者结果.新编码映射[i].first.值
                        || owner账->second.首次新编码映射[i].second
                            != 参与者结果.新编码映射[i].second)
                        return false;
                std::uint32_t 前一映射键 = 0;
                for (const auto& [本地键, 编码] : 参与者结果.新编码映射) {
                    if (本地键.值 == 0 || 本地键.值 <= 前一映射键
                        || !全部本地键.contains(本地键.值) || !有效(编码))
                        return false;
                    前一映射键 = 本地键.值;
                }
            }
            const auto 引用有效 = [&](const L1有限N分区原子事实引用值_v3& 引用,
                std::size_t 当前序号) {
                if (const auto* 稳定项 = std::get_if<稳定编码>(&引用))
                    return 有效(*稳定项);
                if (const auto* 本地项 =
                    std::get_if<L1所有者范围写集本地键>(&引用))
                    return 本地项->值 != 0
                        && 节点键组[当前序号].contains(本地项->值);
                const auto& 跨项 = std::get<L1有限N分区原子事实引用_v3>(引用);
                return 跨项.参与者.值 >= 1
                    && 跨项.参与者.值 < 当前序号 + 1
                    && 节点键组[跨项.参与者.值 - 1].contains(跨项.本地键.值);
            };
            for (std::size_t 序号 = 0; 序号 != 参与者数; ++序号) {
                const auto& 写集 = 请求.参与者写集组[序号].写集;
                for (const auto& 项 : 写集.关系)
                    if (!引用有效(项.源节点, 序号)
                        || !引用有效(项.目标节点, 序号)
                        || !引用有效(项.关系类型节点, 序号)) return false;
                for (const auto& 项 : 写集.值)
                    if (!引用有效(项.所属节点, 序号)
                        || !引用有效(项.属性类型节点, 序号)
                        || !引用有效(项.来源节点, 序号)) return false;
                for (const auto& 项 : 写集.属性槽变更)
                    if (!引用有效(项.所属节点, 序号)
                        || !引用有效(项.属性类型节点, 序号)
                        || !值键组[序号].contains(项.新当前值.值)) return false;
            }
            const auto 转换首次账写集 = [&](const L1有限N分区原子写集请求_v3& 原,
                std::size_t 当前序号) -> std::optional<L1中性写集请求> {
                const auto 查映射 = [&](std::size_t 参与者序号,
                    L1所有者范围写集本地键 本地键) -> std::optional<稳定编码> {
                    const auto& 映射 = 结果.参与者结果组[参与者序号].新编码映射;
                    const auto 位置 = std::lower_bound(映射.begin(), 映射.end(),
                        本地键, [](const auto& 项, const auto 键值) {
                            return 项.first < 键值;
                        });
                    return 位置 != 映射.end() && 位置->first == 本地键
                        ? std::optional<稳定编码>{位置->second} : std::nullopt;
                };
                const auto 转换引用 = [&](const L1有限N分区原子事实引用值_v3& 引用)
                    -> std::optional<L1中性事实引用> {
                    if (const auto* 稳定项 = std::get_if<稳定编码>(&引用))
                        return *稳定项;
                    if (const auto* 本地项 =
                        std::get_if<L1所有者范围写集本地键>(&引用))
                        return L1中性写集本地键{本地项->值};
                    const auto& 跨项 = std::get<L1有限N分区原子事实引用_v3>(引用);
                    if (跨项.参与者.值 == 0 || 跨项.参与者.值 > 当前序号)
                        return std::nullopt;
                    const auto 编码 = 查映射(
                        跨项.参与者.值 - 1, 跨项.本地键);
                    return 编码 ? std::optional<L1中性事实引用>{*编码}
                        : std::nullopt;
                };
                const auto 转换表示 = [](std::optional<L1所有者范围值表示种类> 表示)
                    -> std::optional<L1中性值表示种类> {
                    if (!表示) return std::nullopt;
                    return static_cast<L1中性值表示种类>(
                        static_cast<std::uint8_t>(*表示));
                };
                const auto 转换材料 = [](const L1所有者范围原始值材料& 材料)
                    -> L1中性原始值材料 {
                    return std::visit([](const auto& 材料值) -> L1中性原始值材料 {
                        using 类型 = std::decay_t<decltype(材料值)>;
                        if constexpr (std::is_same_v<类型,
                            L1所有者范围独立材料引用>)
                            return L1中性独立材料引用{材料值.编码};
                        else return 材料值;
                    }, 材料);
                };
                L1中性写集请求 预期;
                预期.合同版本 = L1中性CRUD合同版本;
                预期.期望事实代次 = 原.期望事实代次;
                预期.幂等键 = {原.写入幂等身份.值};
                预期.退出事实 = 原.退出事实;
                for (const auto& 项 : 原.节点)
                    预期.节点.push_back({{项.本地键.值}, 项.种类,
                        转换表示(项.属性类型表示)});
                for (const auto& 项 : 原.关系) {
                    const auto 源 = 转换引用(项.源节点);
                    const auto 目标 = 转换引用(项.目标节点);
                    const auto 类型 = 转换引用(项.关系类型节点);
                    if (!源 || !目标 || !类型) return std::nullopt;
                    预期.关系.push_back({{项.本地键.值}, *源, *目标, *类型,
                        项.角色或顺序});
                }
                for (const auto& 项 : 原.值) {
                    const auto 所属 = 转换引用(项.所属节点);
                    const auto 类型 = 转换引用(项.属性类型节点);
                    const auto 来源 = 转换引用(项.来源节点);
                    if (!所属 || !类型 || !来源) return std::nullopt;
                    预期.值.push_back({{项.本地键.值}, *所属, *类型,
                        转换材料(项.材料), *来源});
                }
                for (const auto& 项 : 原.属性槽变更) {
                    const auto 所属 = 转换引用(项.所属节点);
                    const auto 类型 = 转换引用(项.属性类型节点);
                    if (!所属 || !类型) return std::nullopt;
                    预期.属性槽变更.push_back(
                        {*所属, *类型, {项.新当前值.值}});
                }
                const auto 引用排序键 = [](const L1中性事实引用& 引用) {
                    if (const auto* 稳定项 = std::get_if<稳定编码>(&引用))
                        return 稳定项->值;
                    return (1ULL << 63)
                        | std::get<L1中性写集本地键>(引用).值;
                };
                std::sort(预期.属性槽变更.begin(), 预期.属性槽变更.end(),
                    [&](const auto& 左, const auto& 右) {
                        const auto 左所属 = 引用排序键(左.所属节点);
                        const auto 右所属 = 引用排序键(右.所属节点);
                        if (左所属 != 右所属) return 左所属 < 右所属;
                        const auto 左类型 = 引用排序键(左.属性类型节点);
                        const auto 右类型 = 引用排序键(右.属性类型节点);
                        if (左类型 != 右类型) return 左类型 < 右类型;
                        return 左.新当前值 < 右.新当前值;
                    });
                return 预期;
            };
            for (std::size_t 序号 = 0; 序号 != 参与者数; ++序号) {
                const auto& 参与者 = 请求.参与者写集组[序号];
                const auto 预期 = 转换首次账写集(参与者.写集, 序号);
                const auto owner账组 = 值.所有者范围幂等账.find(
                    参与者.所有者.编码.值);
                if (!预期 || owner账组 == 值.所有者范围幂等账.end())
                    return false;
                const auto owner账 = owner账组->second.find(
                    参与者.写集.写入幂等身份.值);
                if (owner账 == owner账组->second.end()
                    || !(owner账->second.首次规范化写集 == *预期))
                    return false;
            }
        }
        return true;
    }
    template<class T> L1读取结果 读取当前(稳定编码 编码, const std::unordered_map<std::uint64_t, T>& 表) const {
        if (!有效(编码)) return {};
        std::shared_lock<std::shared_mutex> 锁(锁_);
        if (状态_.隔离) return {L1读取状态::内部不一致, 状态_.事实代次, std::nullopt};
        const auto it = 表.find(编码.值);
        if (it != 表.end()) return {L1读取状态::成功, 状态_.事实代次, L1事实副本{it->second}};
        if (const auto 墓碑 = 状态_.物理清理墓碑.find(编码.值);
            墓碑 != 状态_.物理清理墓碑.end())
            return {L1读取状态::历史材料已清理, 状态_.事实代次,
                std::nullopt, 墓碑->second};
        return {状态_.历史.contains(编码.值) ? L1读取状态::已退出 : L1读取状态::未找到, 0, std::nullopt};
    }
    template<class T>
    L1所有者范围当前读取结果 读取所有者范围当前事实(
        const L1所有者范围事实读取请求& 请求,
        const std::unordered_map<std::uint64_t, T>& 表) const {
        L1所有者范围当前读取结果 结果{
            L1所有者范围读取状态::入口拒绝,
            L1所有者范围CRUD合同版本, 请求.编码, 0, std::nullopt};
        if (请求.合同版本 != L1所有者范围CRUD合同版本
            || !有效(请求.编码)) return 结果;
        try {
            std::shared_lock<std::shared_mutex> 锁(锁_);
            if (状态_.隔离 || !状态完整(状态_)) {
                结果.状态 = L1所有者范围读取状态::内部不一致;
                return 结果;
            }
            结果.读取事实代次 = 状态_.事实代次;
            const auto it = 表.find(请求.编码.值);
            if (it != 表.end()) {
                if constexpr (std::is_same_v<T, 节点事实>)
                    结果.事实 = 转换所有者范围节点(it->second);
                else if constexpr (std::is_same_v<T, 关系事实>)
                    结果.事实 = 转换所有者范围关系(it->second);
                else 结果.事实 = 转换所有者范围值(it->second);
                结果.状态 = L1所有者范围读取状态::成功;
            } else 结果.状态 = 状态_.历史.contains(请求.编码.值)
                ? L1所有者范围读取状态::已退出
                : L1所有者范围读取状态::未找到;
            if (结果.状态 == L1所有者范围读取状态::未找到) {
                const auto 墓碑 = 状态_.物理清理墓碑.find(请求.编码.值);
                if (墓碑 != 状态_.物理清理墓碑.end()) {
                    结果.状态 = L1所有者范围读取状态::历史材料已清理;
                    结果.物理清理事实代次 = 墓碑->second.物理清理事实代次;
                    结果.物理清理墓碑 =
                        转换所有者范围物理清理墓碑(墓碑->second);
                }
            }
            return 结果;
        } catch (const std::bad_alloc&) {
            结果.状态 = L1所有者范围读取状态::资源失败;
            结果.事实.reset(); return 结果;
        } catch (...) {
            结果.状态 = L1所有者范围读取状态::内部不一致;
            结果.事实.reset(); return 结果;
        }
    }
    mutable std::shared_mutex 锁_;
    状态 状态_;
    std::unique_ptr<持久会话> 持久会话_;
};

} // namespace 海中鱼巣
