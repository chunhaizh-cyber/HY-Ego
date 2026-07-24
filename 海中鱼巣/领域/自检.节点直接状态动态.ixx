// 文件规则：本模块只保存 P2B 隔离新域的源码级合同检查点，不进入生产依赖，不登记统一运行器。
module;

#include "../核心/句柄.h"
#include "节点直接领域合同.数据.h"

#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

export module 海中鱼巣.领域.自检.节点直接状态动态;

import 海中鱼巣.领域.组合.节点直接状态动态;

namespace 海中鱼巣 {

using 创建完整状态事实签名 = 完整状态写入结果 (
    节点直接状态服务::*)(const 完整实例状态请求&);
using 读取完整状态事实签名 = 完整状态读取结果 (
    节点直接状态服务::*)(节点句柄) const;
using 读取主体状态事实组签名 = 主体状态事实组读取结果 (
    节点直接状态服务::*)(节点句柄) const;
using 记录状态迁移动能签名 = 完整动态写入结果 (
    节点直接动态服务::*)(const 完整动态请求&);
using 读取完整动态事实签名 = 完整动态读取结果 (
    节点直接动态服务::*)(节点句柄) const;
using 读取主体动态事实组签名 = 主体动态事实组读取结果 (
    节点直接动态服务::*)(节点句柄) const;
using 形成状态退役规格签名 = 状态退役规格结果 (
    节点直接状态服务::*)(节点句柄, std::uint32_t) const;
using 形成动态退役规格签名 = 动态退役规格结果 (
    节点直接动态服务::*)(节点句柄, std::uint32_t) const;
using 读取状态退役材料签名 = 带值读取结果<状态退役材料> (
    节点直接状态服务::*)(节点句柄) const;
using 读取动态退役材料签名 = 带值读取结果<动态退役材料> (
    节点直接动态服务::*)(节点句柄) const;
using 复制全部状态记录签名 = 状态动态记录全量读取结果<状态域记录> (
    状态类型化记录只读访问器::*)() const;
using 复制全部动态记录签名 = 状态动态记录全量读取结果<动态域记录> (
    动态类型化记录只读访问器::*)() const;
using 形成状态记录退役参与包签名 = 状态记录退役参与包形成结果 (
    状态动态记录退役提供者::*)(const 状态退役规格&) const;
using 形成动态记录退役参与包签名 = 动态记录退役参与包形成结果 (
    状态动态记录退役提供者::*)(const 动态退役规格&) const;
using 状态记录退役执行一次签名 = 状态记录退役执行结果 (
    状态记录退役参与包::*)(
        节点直接身份结构写入执行器&,
        const std::function<void(节点直接身份结构写入会话&)>&);
using 动态记录退役执行一次签名 = 动态记录退役执行结果 (
    动态记录退役参与包::*)(
        节点直接身份结构写入执行器&,
        const std::function<void(节点直接身份结构写入会话&)>&);
using 复制同代次状态动态全量签名 = 状态动态同代次冻结结果 (
    状态动态冻结提供者::*)(const 节点直接统一冻结许可&) const;
using 形成记录退役提供者签名 = 状态动态记录退役提供者 (
    节点直接状态动态数据操作::*)() noexcept;
using 形成冻结提供者签名 = 状态动态冻结提供者 (
    节点直接状态动态数据操作::*)() const noexcept;
using 创建完整状态迁移签名 = 状态动态迁移写入结果 (
    节点直接状态动态组合::*)(const 创建完整状态迁移请求&);

static_assert(std::is_same_v<
    decltype(static_cast<创建完整状态事实签名>(
        &节点直接状态服务::创建完整状态事实)),
    创建完整状态事实签名>);
static_assert(std::is_same_v<
    decltype(static_cast<读取完整状态事实签名>(
        &节点直接状态服务::读取完整状态事实)),
    读取完整状态事实签名>);
static_assert(std::is_same_v<
    decltype(static_cast<读取主体状态事实组签名>(
        &节点直接状态服务::读取主体状态事实组)),
    读取主体状态事实组签名>);
static_assert(std::is_same_v<
    decltype(static_cast<记录状态迁移动能签名>(
        &节点直接动态服务::记录状态迁移动能)),
    记录状态迁移动能签名>);
static_assert(std::is_same_v<
    decltype(static_cast<读取完整动态事实签名>(
        &节点直接动态服务::读取完整动态事实)),
    读取完整动态事实签名>);
static_assert(std::is_same_v<
    decltype(static_cast<读取主体动态事实组签名>(
        &节点直接动态服务::读取主体动态事实组)),
    读取主体动态事实组签名>);
static_assert(std::is_same_v<
    decltype(static_cast<形成状态退役规格签名>(
        &节点直接状态服务::形成状态退役规格)),
    形成状态退役规格签名>);
static_assert(std::is_same_v<
    decltype(static_cast<形成动态退役规格签名>(
        &节点直接动态服务::形成动态退役规格)),
    形成动态退役规格签名>);
static_assert(std::is_same_v<
    decltype(static_cast<读取状态退役材料签名>(
        &节点直接状态服务::读取状态退役材料)),
    读取状态退役材料签名>);
static_assert(std::is_same_v<
    decltype(static_cast<读取动态退役材料签名>(
        &节点直接动态服务::读取动态退役材料)),
    读取动态退役材料签名>);
static_assert(std::is_same_v<
    decltype(static_cast<复制全部状态记录签名>(
        &状态类型化记录只读访问器::复制全部已发布记录)),
    复制全部状态记录签名>);
static_assert(std::is_same_v<
    decltype(static_cast<复制全部动态记录签名>(
        &动态类型化记录只读访问器::复制全部已发布记录)),
    复制全部动态记录签名>);
static_assert(std::is_same_v<
    decltype(static_cast<形成状态记录退役参与包签名>(
        &状态动态记录退役提供者::形成状态记录退役参与包)),
    形成状态记录退役参与包签名>);
static_assert(std::is_same_v<
    decltype(static_cast<形成动态记录退役参与包签名>(
        &状态动态记录退役提供者::形成动态记录退役参与包)),
    形成动态记录退役参与包签名>);
static_assert(std::is_same_v<
    decltype(static_cast<状态记录退役执行一次签名>(
        &状态记录退役参与包::执行一次)),
    状态记录退役执行一次签名>);
static_assert(std::is_same_v<
    decltype(static_cast<动态记录退役执行一次签名>(
        &动态记录退役参与包::执行一次)),
    动态记录退役执行一次签名>);
static_assert(std::is_same_v<
    decltype(static_cast<复制同代次状态动态全量签名>(
        &状态动态冻结提供者::复制同代次状态动态全量)),
    复制同代次状态动态全量签名>);
static_assert(std::is_same_v<
    decltype(static_cast<形成记录退役提供者签名>(
        &节点直接状态动态数据操作::形成记录退役提供者)),
    形成记录退役提供者签名>);
static_assert(std::is_same_v<
    decltype(static_cast<形成冻结提供者签名>(
        &节点直接状态动态数据操作::形成冻结提供者)),
    形成冻结提供者签名>);
static_assert(std::is_same_v<
    decltype(static_cast<创建完整状态迁移签名>(
        &节点直接状态动态组合::创建完整状态并记录迁移动能)),
    创建完整状态迁移签名>);

static_assert(状态动态类型化记录当前格式版本 == 1);
static_assert(static_cast<std::uint32_t>(关系类型::状态事实角色) == 19);
static_assert(static_cast<std::uint32_t>(关系类型::动态事实角色) == 20);
static_assert(static_cast<std::uint32_t>(状态记录模式::观察) == 1);
static_assert(static_cast<std::uint32_t>(状态记录模式::动作结果) == 2);
static_assert(static_cast<std::uint32_t>(状态记录模式::治理事实) == 3);
static_assert(static_cast<std::uint32_t>(动态种类::状态迁移动能) == 1);
static_assert(static_cast<std::uint32_t>(动态种类::动作致变) == 2);
static_assert(static_cast<std::uint32_t>(动态种类::观察动态) == 3);
static_assert(static_cast<std::uint32_t>(动态种类::外部事件动态) == 4);
static_assert(static_cast<std::uint32_t>(动态种类::聚合动态) == 5);
static_assert(static_cast<std::uint32_t>(退役参与包形成状态::已形成) == 1);
static_assert(static_cast<std::uint32_t>(
    退役参与包形成状态::内部不一致) == 0x8000);
static_assert(static_cast<std::uint32_t>(状态动态冻结状态::已形成) == 1);
static_assert(static_cast<std::uint32_t>(
    状态动态冻结状态::内部不一致) == 0x8000);

static_assert(std::is_same_v<
    decltype(状态域记录::格式版本), std::uint32_t>);
static_assert(std::is_same_v<
    decltype(状态域记录::记录版本), std::uint64_t>);
static_assert(std::is_same_v<
    decltype(动态域记录::记录版本), std::uint64_t>);
static_assert(std::is_same_v<
    decltype(状态事实值式材料::主体证据), 正式关系记录>);
static_assert(std::is_same_v<
    decltype(动态事实值式材料::来源动作证据),
    std::optional<正式关系记录>>);
static_assert(std::is_same_v<
    decltype(动态事实值式材料::来源低层动态证据组),
    std::vector<正式关系记录>>);
static_assert(std::is_same_v<
    decltype(状态动态同代次冻结材料::状态记录结构版本),
    std::uint64_t>);
static_assert(std::is_same_v<
    decltype(状态动态同代次冻结材料::动态记录结构版本),
    std::uint64_t>);
static_assert(std::is_same_v<
    decltype(状态动态同代次冻结结果::材料),
    std::optional<状态动态同代次冻结材料>>);

static_assert(!std::is_copy_constructible_v<状态记录退役参与包>);
static_assert(!std::is_copy_assignable_v<状态记录退役参与包>);
static_assert(std::is_nothrow_move_constructible_v<状态记录退役参与包>);
static_assert(std::is_nothrow_move_assignable_v<状态记录退役参与包>);
static_assert(!std::is_copy_constructible_v<动态记录退役参与包>);
static_assert(!std::is_copy_assignable_v<动态记录退役参与包>);
static_assert(std::is_nothrow_move_constructible_v<动态记录退役参与包>);
static_assert(std::is_nothrow_move_assignable_v<动态记录退役参与包>);
static_assert(!std::is_copy_constructible_v<状态动态冻结提供者>);
static_assert(!std::is_copy_assignable_v<状态动态冻结提供者>);
static_assert(std::is_nothrow_move_constructible_v<状态动态冻结提供者>);
static_assert(std::is_nothrow_move_assignable_v<状态动态冻结提供者>);

}

export namespace 海中鱼巣 {

struct 节点直接状态动态源码自检结果 final {
    std::array<bool, 20> 检查点{};

    bool 全部通过() const noexcept {
        for (const bool 检查点 : 检查点) {
            if (!检查点) return false;
        }
        return true;
    }
};

节点直接状态动态源码自检结果 执行节点直接状态动态源码自检() {
    节点直接状态动态源码自检结果 输出;
    const 节点句柄 状态句柄{1, 1, 1};
    const 节点句柄 动态句柄{1, 2, 1};

    状态域记录 状态记录{
        状态句柄, 1, 1, 记录状态::有效,
        10, std::optional<std::uint64_t>{11}, 状态记录模式::观察};
    输出.检查点[0] = 状态记录.完整();
    auto 未知格式状态记录 = 状态记录;
    未知格式状态记录.格式版本 = 2;
    输出.检查点[1] = !未知格式状态记录.完整();
    auto 已删除状态记录 = 状态记录;
    已删除状态记录.记录版本 = 2;
    已删除状态记录.状态 = 记录状态::已删除;
    输出.检查点[2] = 已删除状态记录.完整()
        && 已删除状态记录.记录版本 == 状态记录.记录版本 + 1;
    auto 已失效状态记录 = 状态记录;
    已失效状态记录.状态 = 记录状态::已失效;
    输出.检查点[3] = 已失效状态记录.完整();

    动态域记录 原子动态记录{
        动态句柄, 1, 1, 记录状态::有效,
        12, 动态种类::状态迁移动能, 0};
    输出.检查点[4] = 原子动态记录.完整();
    auto 原子带聚合版本 = 原子动态记录;
    原子带聚合版本.聚合规则版本 = 1;
    输出.检查点[5] = !原子带聚合版本.完整();
    auto 聚合动态记录 = 原子动态记录;
    聚合动态记录.种类 = 动态种类::聚合动态;
    聚合动态记录.聚合规则版本 = 1;
    输出.检查点[6] = 聚合动态记录.完整();
    聚合动态记录.聚合规则版本 = 0;
    输出.检查点[7] = !聚合动态记录.完整();

    状态动态同代次冻结材料 空冻结材料;
    空冻结材料.见证 = {1, 1, 1};
    空冻结材料.状态记录结构版本 = 1;
    空冻结材料.动态记录结构版本 = 1;
    输出.检查点[8] = 空冻结材料.状态记录组.empty()
        && 空冻结材料.动态记录组.empty()
        && 空冻结材料.状态记录数量 == 0
        && 空冻结材料.动态记录数量 == 0;
    状态动态同代次冻结结果 空冻结结果{
        状态动态冻结状态::已形成, 空冻结材料};
    输出.检查点[9] = 空冻结结果.材料.has_value()
        && 空冻结结果.材料->见证.冻结规则版本 == 1;
    状态动态同代次冻结结果 整体失败结果{
        状态动态冻结状态::内部不一致, std::nullopt};
    输出.检查点[10] = !整体失败结果.材料.has_value();

    状态动态迁移写入结果 空拒绝结果;
    输出.检查点[11] = 空拒绝结果.自洽()
        && !空拒绝结果.成功();
    状态动态迁移值式材料 迁移材料;
    状态动态迁移写入结果 成功形状{
        合同结果状态::成功,
        合同结果原因::无,
        迁移材料,
        2,
        2};
    输出.检查点[12] = 成功形状.自洽() && 成功形状.成功();
    成功形状.动态记录结构版本 = 0;
    输出.检查点[13] = !成功形状.自洽() && !成功形状.成功();

    输出.检查点[14] = 合同状态携带单项材料(合同结果状态::成功)
        && 合同状态携带单项材料(合同结果状态::幂等读回)
        && !合同状态携带单项材料(合同结果状态::写前漂移);
    输出.检查点[15] = static_cast<std::uint32_t>(
        退役参与包形成状态::许可竞争) == 4
        && static_cast<std::uint32_t>(
            退役参与包形成状态::版本漂移) == 5
        && static_cast<std::uint32_t>(
            退役参与包形成状态::资源失败) == 6;
    输出.检查点[16] = static_cast<std::uint32_t>(
        状态动态冻结状态::入口拒绝) == 2
        && static_cast<std::uint32_t>(
            状态动态冻结状态::资源失败) == 3;
    输出.检查点[17] = sizeof(decltype(
        状态记录退役审计材料::退役前记录版本))
        == sizeof(std::uint64_t)
        && sizeof(decltype(
            动态记录退役审计材料::退役后记录版本))
        == sizeof(std::uint64_t);
    输出.检查点[18] = sizeof(decltype(
        状态动态同代次冻结材料::状态记录数量))
        == sizeof(std::uint64_t)
        && sizeof(decltype(
            状态动态同代次冻结材料::动态记录数量))
        == sizeof(std::uint64_t);
    输出.检查点[19] =
        状态动态类型化记录当前格式版本 == 1
        && static_cast<std::uint32_t>(记录状态::有效) == 1
        && static_cast<std::uint32_t>(记录状态::已删除) == 2
        && static_cast<std::uint32_t>(记录状态::已失效) == 3;
    return 输出;
}

}
