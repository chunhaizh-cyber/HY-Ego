#pragma once
#include <cstdint>
#include <optional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include "../领域/本能交互.运行接口.h"
#include "../领域/新_本能函数集.h"
#include "../领域/新_动态类.h"
namespace 海中鱼巣 {
struct 新连接执行现场 final {
    std::vector<新方法参数角色绑定> 绑定;
    稳定编码 身份{}, 方法身份{};
    std::optional<稳定编码> 动作;
    std::optional<新状态信息> 基准状态, 更新状态;
    bool 关系发布许可已冻结=false;
    新连接状态 阶段=新连接状态::待接收状态更新;
    新本能函数本次条件结果对 运行;
};
struct 新连接动作读回 final {
    std::optional<稳定编码> 动作;
    bool 前提仍具备=false;
};
class 新连接执行后端 {
public:
    virtual ~新连接执行后端()=default;
    virtual bool 已请求停止() const=0;
    virtual 新连接动作读回 建立动作(const 方法参数场景&)=0;
    virtual std::optional<新状态信息> 读取最新接收数量状态()=0;
    virtual 新二次特征计算结果 比较状态(const 新状态信息& 最新,const 新状态信息& 上轮) const=0;
    virtual 新交互写入读回 写入交互者(稳定编码)=0;
    virtual 稳定编码 读取连接关系() const=0;
};
新本能函数本次条件结果对 推进连接核心(
    const 方法参数场景&,新连接执行现场&,新连接执行后端&);
新本能函数本次条件结果对 执行隔离连接核心(const 方法参数场景&);
新本能函数本次条件结果对 执行服务对象核心(const 方法参数场景&,
    新本能交互运行接口&,const 新方法初始必要条件定义&);
}
