#pragma once

#include "新_本能函数集.h"
#include "新_需求类.h"
#include "新_特征类.h"

namespace 海中鱼巣 {

enum class 新本能需求材料状态 : std::uint8_t {
    已读取=1, 参数不合法, 材料待齐, 前提已变化, 需重新确认
};
struct 新本能需求材料 final {
    稳定编码 服务对象, 需求特征, 需求节点;
    新需求治理信息 动作前需求;
    新特征信息 动作前需求特征;
    std::vector<基础联合节点快照> 完整保护;
    std::vector<正式直接内容项> 需求内容;
};
struct 新本能需求材料读回 final {
    新本能需求材料状态 状态=新本能需求材料状态::参数不合法;
    std::optional<新本能需求材料> 材料;
};
enum class 新本能需求发布状态 : std::uint8_t {
    已落实并发布=1, 已复用并发布, 前提已变化, 需重新确认,
    已建需求尚未发布, 已发布但当前读回变化
};
struct 新本能需求发布读回 final {
    新本能需求发布状态 状态=新本能需求发布状态::前提已变化;
    std::optional<稳定编码> 需求节点;
    std::vector<稳定编码> 结果状态节点组;
    std::optional<新本能已发布需求结果> 已发布需求;
};

// 实例读写限于参数引用闭包；调用记录及动作来源由提供者的外层租约维护。
class 新本能需求运行接口 {
public:
    virtual ~新本能需求运行接口()=default;
    virtual 新本能需求材料读回 读取新需求材料(const 方法参数场景&) const=0;
    virtual 新本能需求材料状态 复核新需求材料(
        const 方法参数场景&, const 新本能需求材料&) const=0;
    virtual 稳定编码 开始本次需求动作(const 方法参数场景&)=0;
    virtual 新本能需求发布读回 发布新需求(
        const 方法参数场景&, const 新本能需求材料&, 稳定编码 动作)=0;
};

struct 新方法参数场景结果;
struct 新本能需求任务参数请求 final {
    稳定编码 方法节点, 执行实例节点;
    std::vector<新方法参数角色绑定> 参数绑定组;
    std::optional<稳定编码> 执行分支节点;
};
// 外层正式X定位发生漂移，不是本能业务结果或学习样本。
struct 新本能需求任务前提变化 final {};
class 新本能需求任务参数工厂 {
public:
    virtual ~新本能需求任务参数工厂()=default;
    virtual 新本能已发布需求核验结果 核验任务新需求结果(
        稳定编码 方法节点,const 新本能已发布需求结果&) const noexcept=0;
    virtual bool 支持任务方法(稳定编码 方法节点) const noexcept=0;
    virtual std::shared_ptr<新本能需求运行接口> 建立任务参数操作提供者(
        const 新本能需求任务参数请求&)=0;
    virtual 新方法参数场景结果 建立任务参数场景(
        const 新本能需求任务参数请求&,
        const 新内部治理对象读取接口* 治理对象读取,
        std::optional<新方法任务成员绑定> 任务成员绑定)=0;
};

} // namespace 海中鱼巣
