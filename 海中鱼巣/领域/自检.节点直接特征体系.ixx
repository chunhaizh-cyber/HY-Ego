// 文件规则：本模块只保存 P2A 隔离新域的源码级合同检查点，不进入生产依赖，不登记统一运行器。
module;

#include "../核心/句柄.h"
#include "节点直接领域合同.数据.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

export module 海中鱼巣.领域.自检.节点直接特征体系;

import 海中鱼巣.领域.服务.节点直接特征;

namespace 海中鱼巣 {

using 复核特征值归属签名 = 特征值归属读取结果 (节点直接特征服务::*)(
    节点句柄,
    std::optional<节点句柄>,
    特征值归属读取口径) const;
using 读取宿主实例特征槽位组签名 =
    宿主实例特征槽位组读取结果 (节点直接特征服务::*)(节点句柄) const;
using 形成特征值退役规格签名 = 特征值退役规格结果 (节点直接特征服务::*)(
    节点句柄,
    std::uint32_t,
    std::uint64_t) const;
using 读取特征值退役材料签名 =
    带值读取结果<特征值退役材料> (节点直接特征服务::*)(节点句柄) const;
using 复制全部已发布记录签名 =
    特征值类型化记录全量读取结果 (特征值类型化记录只读访问器::*)() const;
using 复制同代次特征值与批次全量签名 =
    节点直接特征同代次冻结结果 (节点直接特征体系数据操作::*)(
        const 节点直接统一冻结许可&) const;

static_assert(std::is_same_v<
    decltype(static_cast<复核特征值归属签名>(
        &节点直接特征服务::复核特征值归属)),
    复核特征值归属签名>);
static_assert(std::is_same_v<
    decltype(static_cast<读取宿主实例特征槽位组签名>(
        &节点直接特征服务::读取宿主实例特征槽位组)),
    读取宿主实例特征槽位组签名>);
static_assert(std::is_same_v<
    decltype(static_cast<形成特征值退役规格签名>(
        &节点直接特征服务::形成特征值退役规格)),
    形成特征值退役规格签名>);
static_assert(std::is_same_v<
    decltype(static_cast<读取特征值退役材料签名>(
        &节点直接特征服务::读取特征值退役材料)),
    读取特征值退役材料签名>);
static_assert(std::is_same_v<
    decltype(static_cast<复制全部已发布记录签名>(
        &特征值类型化记录只读访问器::复制全部已发布记录)),
    复制全部已发布记录签名>);
static_assert(std::is_same_v<
    decltype(static_cast<复制同代次特征值与批次全量签名>(
        &节点直接特征体系数据操作::复制同代次特征值与批次全量)),
    复制同代次特征值与批次全量签名>);

static_assert(static_cast<std::uint32_t>(关系类型::特征实例角色) == 22);
static_assert(特征值类型化记录当前格式版本 == 1);
static_assert(std::is_same_v<
    decltype(特征值归属值式材料::节点版本), std::uint32_t>);
static_assert(std::is_same_v<
    decltype(特征值归属值式材料::原始值版本), std::uint64_t>);
static_assert(std::is_same_v<
    decltype(特征值归属值式材料::定义模板证据), 正式关系记录>);
static_assert(std::is_same_v<
    decltype(特征值批次引用值式材料::项目顺序号), std::uint64_t>);
static_assert(std::is_same_v<
    decltype(特征值批次引用值式材料::写前当前关系),
    std::optional<关系句柄>>);
static_assert(!std::is_copy_constructible_v<节点直接统一冻结许可>);
static_assert(!std::is_copy_assignable_v<节点直接统一冻结许可>);
static_assert(std::is_nothrow_move_constructible_v<节点直接统一冻结许可>);
static_assert(std::is_nothrow_move_assignable_v<节点直接统一冻结许可>);
static_assert(std::is_same_v<
    decltype(节点直接统一冻结见证::冻结域身份), std::uint64_t>);
static_assert(std::is_same_v<
    decltype(节点直接统一冻结见证::已发布代次), std::uint64_t>);
static_assert(std::is_same_v<
    decltype(节点直接统一冻结见证::冻结规则版本), std::uint32_t>);
static_assert(std::is_same_v<
    decltype(节点直接特征批次冻结记录::记录格式版本), std::uint32_t>);
static_assert(std::is_same_v<
    decltype(节点直接特征同代次冻结材料::批次结构版本), std::uint64_t>);
static_assert(std::is_same_v<
    decltype(节点直接特征同代次冻结结果::材料),
    std::optional<节点直接特征同代次冻结材料>>);
static_assert(std::is_copy_constructible_v<节点直接特征同代次冻结材料>);
static_assert(节点直接特征批次身份命名域ABI == 1);
static_assert(static_cast<std::uint32_t>(
    特征值类型化记录全量读取状态::内部不一致) == 0x8000);
static_assert(static_cast<std::uint32_t>(
    节点直接特征冻结状态::内部不一致) == 0x8000);

bool 允许冻结自检关系(
    节点类型,
    节点类型,
    std::int64_t) noexcept {
    return true;
}

完整正式关系准入表 形成冻结自检准入表() {
    完整正式关系准入表 表;
    for (std::size_t 序号 = 0; 序号 < 表.规则组.size(); ++序号) {
        表.规则组[序号] = {
            static_cast<关系类型>(序号),
            0x4E54'5032'5100'0001ULL + 序号,
            1,
            true,
            true,
            true,
            &允许冻结自检关系};
    }
    return 表;
}

struct 统一冻结事务夹具 final {
    统一冻结事务夹具()
        : 节点(0x3751),
          关系(节点, 形成冻结自检准入表(), 0x3752),
          索引(节点, 关系),
          执行器(&事务域, &节点, &关系, &索引) {
    }

    节点直接身份仓库 节点;
    正式关系仓库 关系;
    可重建索引仓库 索引;
    节点直接身份结构事务域 事务域;
    节点直接身份结构写入执行器 执行器;
};

struct 统一冻结事务证据 final {
    bool 冻结阻断写入 = false;
    bool 真实提交推进一次 = false;
    bool 非提交不推进 = false;
};

统一冻结事务证据 形成统一冻结事务证据() {
    统一冻结事务夹具 夹具;
    auto 首次许可 = 夹具.事务域.取得统一冻结许可(1);
    const auto 初始见证 = 首次许可.读取见证();
    const auto 冻结中写入 = 夹具.执行器.执行(
        [](节点直接身份结构写入会话& 会话) { 会话.请求撤销(); });
    首次许可 = 节点直接统一冻结许可{};

    const 节点稳定主键 主键{
        static_cast<std::uint64_t>(节点稳定主键命名域::文章身份), 1};
    const auto 提交结果 = 夹具.执行器.执行(
        [&主键](节点直接身份结构写入会话& 会话) {
            const auto 创建 = 会话.创建节点候选(节点类型::文章, 主键);
            if (创建.成功()) 会话.请求提交(); else 会话.请求撤销();
        });
    auto 提交后许可 = 夹具.事务域.取得统一冻结许可(1);
    const auto 提交后见证 = 提交后许可.读取见证();
    提交后许可 = 节点直接统一冻结许可{};

    const auto 幂等结果 = 夹具.执行器.执行(
        [&主键](节点直接身份结构写入会话& 会话) {
            const auto 创建 = 会话.创建节点候选(节点类型::文章, 主键);
            if (创建.成功()) 会话.请求提交(); else 会话.请求撤销();
        });
    const auto 拒绝结果 = 夹具.执行器.执行(
        [](节点直接身份结构写入会话&) {});
    const auto 撤销结果 = 夹具.执行器.执行(
        [](节点直接身份结构写入会话& 会话) {
            const 节点稳定主键 撤销主键{
                static_cast<std::uint64_t>(节点稳定主键命名域::文章身份), 2};
            (void)会话.创建节点候选(节点类型::文章, 撤销主键);
            会话.请求撤销();
        });
    auto 非提交后许可 = 夹具.事务域.取得统一冻结许可(1);
    const auto 非提交后见证 = 非提交后许可.读取见证();

    统一冻结事务证据 证据;
    证据.冻结阻断写入 = 初始见证.冻结域身份 != 0
        && 冻结中写入.状态 == 节点直接身份结构写入状态::许可拒绝;
    证据.真实提交推进一次 =
        提交结果.状态 == 节点直接身份结构写入状态::已提交
        && 提交后见证.已发布代次 == 初始见证.已发布代次 + 1;
    证据.非提交不推进 =
        幂等结果.状态 == 节点直接身份结构写入状态::幂等读回
        && 拒绝结果.状态 == 节点直接身份结构写入状态::入口拒绝
        && 撤销结果.状态 == 节点直接身份结构写入状态::候选已撤销
        && 非提交后见证.已发布代次 == 提交后见证.已发布代次;
    return 证据;
}

}

export namespace 海中鱼巣 {

struct 节点直接特征体系源码自检结果 {
    std::array<bool, 27> 检查点{};

    bool 全部通过() const noexcept {
        for (const bool 检查点 : 检查点) {
            if (!检查点) return false;
        }
        return true;
    }
};

节点直接特征体系源码自检结果 执行节点直接特征体系源码自检() {
    节点直接特征体系源码自检结果 输出;

    特征值原始值式材料 I64材料;
    I64材料.类型 = 特征值原始类型::I64;
    I64材料.I64值 = 7;

    特征值原始值式材料 VecI64材料;
    VecI64材料.类型 = 特征值原始类型::VecI64;
    VecI64材料.VecI64值 = {1, 2};

    特征值原始值式材料 VecU64材料;
    VecU64材料.类型 = 特征值原始类型::VecU64;
    VecU64材料.VecU64值 = {3, 4};

    auto 多载荷材料 = I64材料;
    多载荷材料.VecI64值 = {1};

    输出.检查点[0] = I64材料.完整();
    输出.检查点[1] = VecI64材料.完整();
    输出.检查点[2] = VecU64材料.完整();
    输出.检查点[3] = !多载荷材料.完整();
    输出.检查点[4] = static_cast<std::uint32_t>(特征值原始类型::I64) == 1
        && static_cast<std::uint32_t>(特征值原始类型::VecI64) == 2
        && static_cast<std::uint32_t>(特征值原始类型::VecU64) == 3;
    输出.检查点[5] = static_cast<std::uint32_t>(合同结果状态::成功) == 1
        && static_cast<std::uint32_t>(合同结果状态::内部错误) == 9;
    输出.检查点[6] = 合同状态携带单项材料(合同结果状态::成功)
        && 合同状态携带单项材料(合同结果状态::幂等读回)
        && !合同状态携带单项材料(合同结果状态::合法未找到);
    输出.检查点[7] = 合同状态携带具名组材料(合同结果状态::成功)
        && 合同状态携带具名组材料(合同结果状态::合法空组)
        && !合同状态携带具名组材料(合同结果状态::入口拒绝);
    输出.检查点[8] = sizeof(decltype(特征值退役规格::预期节点版本))
        == sizeof(std::uint32_t);
    输出.检查点[9] = sizeof(decltype(特征值退役规格::预期原始值版本))
        == sizeof(std::uint64_t);
    输出.检查点[10] = static_cast<std::int64_t>(1) != static_cast<std::int64_t>(2)
        && static_cast<std::int64_t>(2) != static_cast<std::int64_t>(3);
    输出.检查点[11] = 特征值最大序列元素数量 != 0;
    节点直接身份结构事务域 第一冻结域;
    节点直接身份结构事务域 第二冻结域;
    auto 第一许可 = 第一冻结域.取得统一冻结许可(1);
    const auto 第一见证 = 第一许可.读取见证();
    输出.检查点[12] = 第一许可.有效()
        && 第一见证.冻结域身份 != 0 && 第一见证.已发布代次 != 0
        && 第一见证.冻结规则版本 == 1
        && 第一冻结域.冻结许可属于本域(第一许可);
    auto 第二许可 = 第二冻结域.取得统一冻结许可(1);
    const auto 第二见证 = 第二许可.读取见证();
    输出.检查点[13] = 第二许可.有效()
        && 第二见证.冻结域身份 != 第一见证.冻结域身份
        && !第一冻结域.冻结许可属于本域(第二许可)
        && !第二冻结域.冻结许可属于本域(第一许可);
    auto 移动后许可 = std::move(第一许可);
    输出.检查点[14] = !第一许可.有效() && 移动后许可.有效()
        && 第一冻结域.冻结许可属于本域(移动后许可);
    auto 错规则许可 = 节点直接身份结构事务域{}.取得统一冻结许可(2);
    输出.检查点[15] = !错规则许可.有效()
        && 错规则许可.读取见证().冻结域身份 == 0;
    节点直接特征同代次冻结材料 空材料;
    空材料.见证 = 第一见证;
    空材料.特征值结构版本 = 1;
    空材料.批次结构版本 = 1;
    输出.检查点[16] = 空材料.特征值记录组.empty()
        && 空材料.批次记录组.empty()
        && 空材料.特征值记录数量 == 0 && 空材料.批次记录数量 == 0;
    节点直接特征同代次冻结结果 原子空组结果{
        节点直接特征冻结状态::已形成, 空材料};
    输出.检查点[17] = 原子空组结果.材料.has_value()
        && 原子空组结果.材料->特征值记录组.empty()
        && 原子空组结果.材料->批次记录组.empty();
    节点直接特征同代次冻结结果 零材料结果{
        节点直接特征冻结状态::内部不一致, std::nullopt};
    输出.检查点[18] = !零材料结果.材料.has_value();
    特征值批次引用值式材料 部分写前项目;
    部分写前项目.写前当前关系 = 关系句柄{1, 1, 1};
    const bool 三项全空 = !部分写前项目.写前当前关系.has_value()
        && !部分写前项目.写前当前值.has_value()
        && !部分写前项目.写前原始值版本.has_value();
    const bool 三项全有 = 部分写前项目.写前当前关系.has_value()
        && 部分写前项目.写前当前值.has_value()
        && 部分写前项目.写前原始值版本.has_value();
    输出.检查点[19] = !三项全空 && !三项全有;
    输出.检查点[20] = static_cast<std::uint32_t>(
        特征值类型化记录全量读取状态::已形成) == 1
        && static_cast<std::uint32_t>(
            特征值类型化记录全量读取状态::许可竞争) == 2
        && static_cast<std::uint32_t>(
            特征值类型化记录全量读取状态::资源失败) == 3;
    输出.检查点[21] = static_cast<std::uint32_t>(
        节点直接特征冻结状态::已形成) == 1
        && static_cast<std::uint32_t>(
            节点直接特征冻结状态::入口拒绝) == 2
        && static_cast<std::uint32_t>(
            节点直接特征冻结状态::资源失败) == 3;
    auto 材料副本 = *原子空组结果.材料;
    材料副本.特征值结构版本 = 2;
    输出.检查点[22] = 原子空组结果.材料->特征值结构版本 == 1
        && 材料副本.特征值结构版本 == 2;
    输出.检查点[23] = sizeof(节点直接特征批次身份命名域ABI)
        == sizeof(std::uint64_t)
        && sizeof(decltype(节点直接特征批次冻结记录::记录格式版本))
            == sizeof(std::uint32_t);
    const auto 冻结事务证据 = 形成统一冻结事务证据();
    输出.检查点[24] = 冻结事务证据.冻结阻断写入;
    输出.检查点[25] = 冻结事务证据.真实提交推进一次;
    输出.检查点[26] = 冻结事务证据.非提交不推进;
    return 输出;
}

}
