#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include "../核心/基础数据集.h"
#include "消息交互.数据.h"
#include "新_方法公共类型.h"
#include "新_存在类.h"
#include "新_二次特征类.h"

namespace 海中鱼巣 {
class 方法参数场景;
enum class 新隔离推进状态 : std::uint8_t { 已推进=1,等待外因,已耗尽,已停止 };
enum class 新隔离动作阶段 : std::uint8_t { 主动作=1,输出入列 };
struct 新隔离调用身份 final {
    稳定编码 调用节点,方法节点,场景节点;
    std::optional<稳定编码> 主动作;
};
struct 新隔离阶段动作请求 final {
    std::uint32_t 目标存在位置=0;
    稳定编码 目标特征类型;
    新隔离动作阶段 阶段=新隔离动作阶段::主动作;
};
struct 新隔离输出请求 final {
    std::uint32_t 窗口位置=0,对端位置=0,发送特征位置=0;
    稳定编码 队列特征类型,阶段动作;
    std::string UTF8正文;
};
struct 新隔离提交结果 final {
    std::uint64_t 提交序号=0;
    稳定编码 输出消息,入列状态,入列动态;
};
struct 新隔离变化证据 final {
    稳定编码 调用节点,特征节点,后状态,变化动态;
    std::optional<稳定编码> 前状态,动作节点;
    bool 方法直接写入=false;
};
struct 新隔离调用证据 final {
    新隔离调用身份 身份;
    bool 已请求取消=false;
    std::vector<稳定编码> 阶段动作组;
    std::vector<新隔离变化证据> 变化组;
};
struct 新隔离连接完成材料 final {
    新交互绑定 绑定;
    稳定编码 调用节点,主动作,基准状态,更新状态;
};
class 新方法隔离执行接口 {
public:
    virtual ~新方法隔离执行接口()=default;
    virtual 新隔离调用身份 读取当前调用身份() const=0;
    virtual 稳定编码 建立阶段动作(const 方法参数场景&,const 新隔离阶段动作请求&)=0;
    virtual 新隔离提交结果 提交输出(const 方法参数场景&,const 新隔离输出请求&)=0;
    virtual 新隔离推进状态 推进输出()=0;
    virtual std::uint64_t 读取进展序号() const=0;
    virtual bool 请求停止已成立()=0;
    virtual std::optional<新隔离调用证据> 读取已关闭调用证据() const=0;
    virtual std::optional<新状态信息> 读取本地状态(稳定编码) const=0;
    virtual std::optional<新状态信息> 读取最新特征状态(稳定编码 场景,稳定编码 存在,稳定编码 特征) const=0;
    virtual 新二次特征计算结果 比较接收数量状态(const 新状态信息& 最新,const 新状态信息& 上轮) const=0;
    virtual std::optional<新状态信息> 读取固定消息状态(稳定编码,稳定编码) const=0;
    virtual std::optional<新动态信息> 读取本地动作(稳定编码) const=0;
    virtual std::optional<新动态信息> 读取本地变化(稳定编码) const=0;
    virtual std::optional<新外设消息信息> 按提交读取发送消息(std::uint64_t) const=0;
    virtual bool 提交已完整写出(std::uint64_t) const=0;
    virtual std::optional<新外设消息信息> 读取消息(稳定编码) const=0;
    virtual std::vector<新外设消息信息> 读取消息组(稳定编码) const=0;
    virtual 新场景事实构成读取结果 读取连接构成() const=0;
    virtual 新交互写入读回 写入本次连接交互者(const 方法参数场景&,const 新隔离连接完成材料&)=0;
};
struct 新本能函数本次条件结果对;
struct 新方法参数角色绑定;
class 新方法场景事实构成读取接口 {
public:
    virtual ~新方法场景事实构成读取接口()=default;
    virtual 新场景事实构成读取结果 读取场景事实构成(稳定编码) const=0;
};
class 新方法场景事实读取接口 {
public:
    virtual ~新方法场景事实读取接口()=default;
    virtual 新交互求值状态 读取场景事实(const 新方法初始场景事实限定&,
        const std::vector<新方法参数角色绑定>&) const=0;
    virtual 新交互求值状态 读取本次结果场景事实(
        const 新方法本次场景事实引用&,
        const std::vector<新方法参数角色绑定>&,
        稳定编码, 稳定编码, 新交互连接过程& 输出) const {
        输出 = {};
        return 新交互求值状态::不支持;
    }
};
class 新本能交互运行接口 : public 新方法场景事实读取接口 {
public:
    virtual std::optional<新方法初始必要条件定义> 读取本次必要条件(const 方法参数场景&) const=0;
    virtual 新交互求值状态 读取绑定环境(const 方法参数场景&,新交互绑定&)=0;
    virtual 新存在概念匹配状态 核验绑定存在概念(稳定编码,稳定编码)=0;
    virtual std::optional<新特征信息> 读取交互者(稳定编码)=0;
    virtual 稳定编码 建立本次动作(const 方法参数场景&,const 新交互绑定&)=0;
    virtual 新交互写入读回 写入交互者(const 新交互绑定&,稳定编码)=0;
    virtual bool 已请求停止() const noexcept=0;
};
} // namespace 海中鱼巣
