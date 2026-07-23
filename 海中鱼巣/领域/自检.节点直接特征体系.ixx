// 文件规则：本模块只保存 P2A 隔离新域的源码级合同检查点，不进入生产依赖，不登记统一运行器。
module;

#include "../核心/句柄.h"
#include "节点直接领域合同.数据.h"

#include <array>
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

}

export namespace 海中鱼巣 {

struct 节点直接特征体系源码自检结果 {
    std::array<bool, 12> 检查点{};

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
    return 输出;
}

}
