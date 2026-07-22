// 文件规则：节点直接运行期业务装配只借用冻结 C3 服务实体并形成隔离候选；不得拥有仓库、会话、令牌、锁或默认入口。
module;

#include "领域/节点直接P3合同.数据.h"

#include <functional>
#include <optional>
#include <type_traits>
#include <utility>

export module 海中鱼巣.装配.节点直接运行期业务;

import 海中鱼巣.领域.服务.节点直接系统角色;
import 海中鱼巣.领域.服务.节点直接存在场景扩展;
import 海中鱼巣.领域.服务.节点直接语素基础;
import 海中鱼巣.领域.服务.节点直接轻量因果;
import 海中鱼巣.领域.服务.节点直接概念结构;
import 海中鱼巣.领域.服务.节点直接需求;
import 海中鱼巣.领域.服务.节点直接任务;
import 海中鱼巣.领域.服务.节点直接方法;
import 海中鱼巣.领域.服务.节点直接用途观察;
import 海中鱼巣.领域.退役.节点直接对象;
import 海中鱼巣.领域.审计.节点直接退役;
import 海中鱼巣.领域.适配.节点直接运行期业务请求;
import 海中鱼巣.领域.组合.节点直接运行期业务操作;
import 海中鱼巣.领域.组合.节点直接运行期只读查询;

export namespace 海中鱼巣 {

struct 节点直接运行期业务装配请求 {
    std::uint32_t 合同版本 = 节点直接P3合同版本;
    std::reference_wrapper<const 节点直接系统角色服务> 系统角色;
    std::reference_wrapper<const 节点直接存在场景扩展服务> 存在场景扩展;
    std::reference_wrapper<const 节点直接语素基础服务> 语素基础;
    std::reference_wrapper<const 节点直接轻量因果服务> 轻量因果;
    std::reference_wrapper<const 节点直接概念结构服务> 概念结构;
    std::reference_wrapper<const 节点直接需求服务> 需求;
    std::reference_wrapper<const 节点直接任务服务> 任务;
    std::reference_wrapper<const 节点直接方法服务> 方法;
    std::reference_wrapper<const 节点直接用途观察服务> 用途观察;
    std::reference_wrapper<const 节点直接对象退役服务> 对象退役;
    std::reference_wrapper<const 节点直接退役审计服务> 退役审计;
    std::reference_wrapper<const 节点直接投影后验服务> 投影后验;
    std::reference_wrapper<const 节点直接运行期业务请求适配器> 请求适配;
    std::reference_wrapper<const 节点直接运行期业务操作组合> 业务操作;
    std::reference_wrapper<const 节点直接运行期只读查询组合> 只读查询;
};

class 节点直接运行期业务装配;
struct 节点直接运行期业务装配结果;

节点直接运行期业务装配结果 形成节点直接运行期业务装配(
    const 节点直接运行期业务装配请求& 请求);

class 节点直接运行期业务装配 {
public:
    节点直接运行期业务装配() = delete;
    节点直接运行期业务装配(const 节点直接运行期业务装配&) = delete;
    节点直接运行期业务装配& operator=(const 节点直接运行期业务装配&) = delete;
    节点直接运行期业务装配(节点直接运行期业务装配&&) noexcept = default;
    节点直接运行期业务装配& operator=(节点直接运行期业务装配&&) noexcept = default;
    ~节点直接运行期业务装配() = default;

    std::uint32_t 读取合同版本() const noexcept { return 合同版本_; }

private:
    friend 节点直接运行期业务装配结果 形成节点直接运行期业务装配(
        const 节点直接运行期业务装配请求& 请求);

    explicit 节点直接运行期业务装配(
        const 节点直接运行期业务装配请求& 请求) noexcept
        : 合同版本_(请求.合同版本),
          系统角色_(请求.系统角色),
          存在场景扩展_(请求.存在场景扩展),
          语素基础_(请求.语素基础),
          轻量因果_(请求.轻量因果),
          概念结构_(请求.概念结构),
          需求_(请求.需求),
          任务_(请求.任务),
          方法_(请求.方法),
          用途观察_(请求.用途观察),
          对象退役_(请求.对象退役),
          退役审计_(请求.退役审计),
          投影后验_(请求.投影后验),
          请求适配_(请求.请求适配),
          业务操作_(请求.业务操作),
          只读查询_(请求.只读查询) {}

    std::uint32_t 合同版本_ = 0;
    std::reference_wrapper<const 节点直接系统角色服务> 系统角色_;
    std::reference_wrapper<const 节点直接存在场景扩展服务> 存在场景扩展_;
    std::reference_wrapper<const 节点直接语素基础服务> 语素基础_;
    std::reference_wrapper<const 节点直接轻量因果服务> 轻量因果_;
    std::reference_wrapper<const 节点直接概念结构服务> 概念结构_;
    std::reference_wrapper<const 节点直接需求服务> 需求_;
    std::reference_wrapper<const 节点直接任务服务> 任务_;
    std::reference_wrapper<const 节点直接方法服务> 方法_;
    std::reference_wrapper<const 节点直接用途观察服务> 用途观察_;
    std::reference_wrapper<const 节点直接对象退役服务> 对象退役_;
    std::reference_wrapper<const 节点直接退役审计服务> 退役审计_;
    std::reference_wrapper<const 节点直接投影后验服务> 投影后验_;
    std::reference_wrapper<const 节点直接运行期业务请求适配器> 请求适配_;
    std::reference_wrapper<const 节点直接运行期业务操作组合> 业务操作_;
    std::reference_wrapper<const 节点直接运行期只读查询组合> 只读查询_;
};

struct 节点直接运行期业务装配结果 {
    std::uint32_t 合同版本 = 节点直接P3合同版本;
    std::uint16_t 结果ABI版本 = 节点直接P3结果ABI版本;
    节点直接P3结果状态 状态 = 节点直接P3结果状态::请求拒绝;
    std::optional<节点直接运行期业务装配> 装配;
    std::optional<节点直接P3内部错误材料> 内部错误;
};

节点直接运行期业务装配结果 形成节点直接运行期业务装配(
    const 节点直接运行期业务装配请求& 请求) {
    if (请求.合同版本 != 节点直接P3合同版本) {
        return {
            节点直接P3合同版本,
            节点直接P3结果ABI版本,
            节点直接P3结果状态::请求拒绝,
            std::nullopt,
            std::nullopt};
    }

    return {
        节点直接P3合同版本,
        节点直接P3结果ABI版本,
        节点直接P3结果状态::已完成,
        std::optional<节点直接运行期业务装配>{
            节点直接运行期业务装配{请求}},
        std::nullopt};
}

static_assert(!std::is_default_constructible_v<节点直接运行期业务装配>);
static_assert(!std::is_copy_constructible_v<节点直接运行期业务装配>);
static_assert(std::is_move_constructible_v<节点直接运行期业务装配>);

}
