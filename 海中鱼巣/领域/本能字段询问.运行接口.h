#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string_view>
#include <vector>
#include "字段询问.数据.h"
#include "新_方法公共类型.h"

namespace 海中鱼巣 {
class 方法参数场景;

class 新字段询问语言接口 {
public:
    virtual ~新字段询问语言接口()=default;
    virtual 新字段询问文字结果 生成字段询问(
        const 新正式字段询问目标&, const 正式信息参数读取接口&) const=0;
};
enum class 新字段询问材料状态 : std::uint8_t {
    已取得=1, 字段已有值, 字段不适用, 前提已变化, 定位未支持
};
struct 新字段询问材料 final {
    正式内容引用定位 定位;
    正式信息参数读取结果 定位需求, 待完善需求;
    正式内容值 字段内容;
    新正式字段询问目标 表达上下文;
    // 参数4/5：现实交互场景中的窗口副本及其外设场景标识特征（方案乙，2026-11-10用户裁决）；
    // 窗口/输出特征：经标识对应的外设对话框及其输出队列特征，由运行方对应后填入。
    // CONV-INSTINCT §6.6（2026-11-10用户裁决）：交互者＝参数2自我的“交互者”特征当前值，作为受话者，只作执行时材料。
    稳定编码 执行场景{}, 外设场景{}, 自我{}, 交互者{}, 窗口{}, 输出特征{};
    稳定编码 窗口副本{}, 标识特征{};
};
struct 新字段询问材料读回 final {
    新字段询问材料状态 状态=新字段询问材料状态::定位未支持;
    std::optional<新字段询问材料> 材料;
};
struct 新字段询问入列读回 final {
    稳定编码 消息节点{}, 入列状态{}, 入列动态{};
    std::uint64_t 提交序号=0;
};
enum class 新出站表达留档状态 : std::uint8_t { 已留档=1, 前提已变化, 模拟不留档 };
class 新本能字段询问运行接口 {
public:
    virtual ~新本能字段询问运行接口()=default;
    virtual 新字段询问材料读回 读取询问材料(const 方法参数场景&) const=0;
    virtual 新字段询问文字结果 生成询问文字(const 新正式字段询问目标&) const=0;
    virtual bool 已请求停止() const noexcept=0;
    virtual 稳定编码 建立输出动作(const 方法参数场景&, const 新字段询问材料&)=0;
    virtual 新出站表达留档状态 留档出站具体表达(
        const 方法参数场景&,const 新字段询问材料&,稳定编码 动作,const 新字段询问文字结果&)=0;
    virtual 新字段询问入列读回 提交询问输出(
        const 方法参数场景&, const 新字段询问材料&, 稳定编码 动作,
        std::string_view UTF8正文)=0;
};
struct 新字段询问登记配置 final {
    稳定编码 需求根概念{}, 自我范围概念{}, 交互者特征类型{}, 通用存在概念{}, 输出队列类型{}, 外设场景标识类型{};
    稳定编码 语言节点{}, 第三方指代概念{}, 第三方别名关系{};
    std::vector<稳定编码> 支持字段类型组;
    稳定编码 需求主体类型{};
    friend bool operator==(const 新字段询问登记配置&, const 新字段询问登记配置&)=default;
};
// 窗口副本→外设对话框对应：入参（外设场景, 窗口副本），返回（外设对话框存在, 输出队列特征）；
// 未提供或返回空时按定位未支持（模拟世界无外设）。
using 新字段询问外设窗口对应 = std::function<std::optional<std::pair<稳定编码,稳定编码>>(稳定编码,稳定编码)>;
新字段询问材料读回 读取通用询问材料(
    const 方法参数场景&, const 新字段询问登记配置&,
    const 正式信息参数读取接口&, const 新字段询问外设窗口对应& 对应外设窗口 = {},
    const std::function<std::optional<稳定编码>(稳定编码)>& 读取当前交互者 = {});
// 询问等待回应（ASK-READ-SCENE v0.4 §3.2）：窗口副本经外设场景标识对应的外设对话框及其接收数量特征。
// 对方消息经外设世界树接收后在该特征上形成状态并投递事实变化；任务按此定位冻结结果获取目标。
struct 新字段询问回应观察定位 final {
    稳定编码 外设窗口{}, 接收数量特征{};
    friend bool operator==(const 新字段询问回应观察定位&, const 新字段询问回应观察定位&)=default;
};
class 新字段询问登记端口 {
public:
    virtual ~新字段询问登记端口()=default;
    virtual std::optional<新字段询问登记配置> 读取登记材料() const noexcept=0;
    virtual 新方法建立结果 确保方法登记(const 新方法知识建立请求&) noexcept=0;
    // 入参（外设场景, 窗口副本）；副本不是该外设场景在现实世界的唯一副本或外设对话框已不在时为空。
    virtual std::optional<新字段询问回应观察定位> 读取回应观察定位(稳定编码 外设场景, 稳定编码 窗口副本) const noexcept=0;
};

} // namespace 海中鱼巣
