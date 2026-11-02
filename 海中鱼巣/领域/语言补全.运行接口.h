#pragma once
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include "新_需求类.h"
#include "本能字段询问.运行接口.h"
namespace 海中鱼巣 {
class 方法参数场景;
enum class 新语言补全种类  :  std::uint8_t { 词条归类=1, 指代澄清=2 };
enum class 新语言补全动作  :  std::uint8_t { 询问=1, 发送=2, 回答写回=3 };
enum class 新语言补全状态  :  std::uint8_t {
    已完成=1, 无需补全, 等待材料, 来源已变化, 输入不适用, 结构不一致, 资源失败
};
struct 新语言补全目标 final {
    新语言补全种类 种类=新语言补全种类::词条归类;
    std::optional<新词条归类目标> 词条;
    // 指代澄清是实例E上的真实FT/值目标；没有新需求种类/布尔镜像。
    std::optional<新需求完整目标> 指代;
    friend bool operator==(const 新语言补全目标&,const 新语言补全目标&)=default;
};
struct 新语言补全缺口结果 final {
    新语言补全状态 状态=新语言补全状态::等待材料;
    std::vector<新语言补全目标> 目标组;
    std::vector<基础联合节点快照> 完整保护;
};
struct 新语言补全候选 final {
    新语言补全目标 目标;
    std::string UTF8正文;
    std::vector<稳定编码> 所用知识节点组;
    std::vector<基础联合节点快照> 完整保护;
    std::optional<稳定编码> 正式表达需求节点;
    bool 原句确认=false;
    std::optional<稳定编码> 发送调用身份;
};
struct 新语言补全询问结果 final {
    新语言补全状态 状态=新语言补全状态::等待材料;
    std::optional<新语言补全候选> 候选;
};
struct 新语言补全发送结果 final {
    新语言补全状态 状态=新语言补全状态::等待材料;
    std::optional<新字段询问入列读回> 入列;
};
struct 新语言补全回答写回结果 final {
    新语言补全状态 状态=新语言补全状态::等待材料;
    bool 目标当前已满足=false;
    std::vector<基础联合节点快照> 完整保护;
    std::optional<新语言补全候选> 原句确认候选;
    bool 仍待理解重述=false;
};
// 由原语言owner组合原存在/特征/概念/名称/外设公开服务。启动前装配，全部join后释放。
// 纯读取缺口不建D/T、不写词条；目标只来自原来源中的正式词条或指代实例。
class 新语言补全运行接口 {
public :
    virtual ~新语言补全运行接口()=default;
    virtual 新语言补全缺口结果 读取缺口(const 新需求字段追问来源&) const=0;
    virtual std::optional<稳定编码> 读取方法(新语言补全种类,新语言补全动作) const=0;
    // first角色是当前完整子D；其余S/H/W/Fout。方法内部仍经正式本能登记执行。
    virtual 新语言补全询问结果 询问(const 方法参数场景&,const 新语言补全目标&,
        const 新需求字段追问来源&)=0;
    // 发送判断fresh成功才记录动作并提交正式外设队列；候选生成从不调用此入口。
    virtual 新语言补全发送结果 发送(const 方法参数场景&,const 新语言补全候选&,
        const 新需求字段追问来源&)=0;
    // 只读真实入站答复并唯一解析/关联；未收到/多义保持等待，禁止制造答复。
    virtual 新语言补全回答写回结果 回答写回(const 方法参数场景&,const 新语言补全目标&,
        const 新需求字段追问来源&)=0;
};
}
