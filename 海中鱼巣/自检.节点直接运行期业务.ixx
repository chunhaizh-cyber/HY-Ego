// 文件规则：节点直接运行期业务自检只组织隔离 P3 全图候选的结构验收；不得进入生产依赖、写业务事实或接入统一运行器。
module;

#include "领域/节点直接P3合同.数据.h"

#include <array>
#include <cstddef>
#include <type_traits>
#include <vector>

export module 海中鱼巣.自检.节点直接运行期业务;

import 海中鱼巣.装配.节点直接运行期业务;
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

struct 节点直接运行期业务自检报告 {
    std::uint32_t 合同版本 = 节点直接P3合同版本;
    std::uint32_t 矩阵版本 = 1;
    std::size_t 用例总数 = 0;
    std::size_t 通过数 = 0;
    std::size_t 失败数 = 0;
    std::vector<节点直接P3内部错误材料> 内部错误组;
};

节点直接运行期业务自检报告 运行节点直接运行期业务自检(
    const 节点直接运行期业务装配& 装配) {
    const std::array<bool, 4> 验收{
        装配.读取合同版本() == 节点直接P3合同版本,
        !std::is_default_constructible_v<节点直接运行期业务装配>,
        !std::is_copy_constructible_v<节点直接运行期业务装配>,
        std::is_move_constructible_v<节点直接运行期业务装配>};

    节点直接运行期业务自检报告 报告;
    报告.用例总数 = 验收.size();
    for (const bool 通过 : 验收) {
        if (通过) {
            ++报告.通过数;
        } else {
            ++报告.失败数;
        }
    }
    return 报告;
}

static_assert(std::is_aggregate_v<节点直接运行期业务装配请求>);

}
