#pragma once

#include <cstdint>
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
    稳定编码 执行场景{}, 外设场景{}, 自我{}, 受询者{}, 窗口{}, 输出特征{};
};
struct 新字段询问材料读回 final {
    新字段询问材料状态 状态=新字段询问材料状态::定位未支持;
    std::optional<新字段询问材料> 材料;
};
struct 新字段询问入列读回 final {
    稳定编码 消息节点{}, 入列状态{}, 入列动态{};
    std::uint64_t 提交序号=0;
};
class 新本能字段询问运行接口 {
public:
    virtual ~新本能字段询问运行接口()=default;
    virtual 新字段询问材料读回 读取询问材料(const 方法参数场景&) const=0;
    virtual 新字段询问文字结果 生成询问文字(const 新正式字段询问目标&) const=0;
    virtual 新字段询问文字结果 提交表达需求(const 新字段询问表达需求& 目标) const {
        return 生成询问文字(目标);
    }
    virtual bool 已请求停止() const noexcept=0;
};
// task的独立发送工作消费；询问本能不取得此端口。
class 新字段询问发送运行接口 {
public:
    virtual ~新字段询问发送运行接口()=default;
    // 只有独立动作定义及语言结果.完整保护组（生成+唯一回读）已能原子提交时才可开放。
    // 语言侧交付非空完整保护后，由本能实现覆写为 true；默认保持关闭。
    virtual bool 原子发送合同已接通() const noexcept { return false; }
    virtual bool 复核发送会话(稳定编码,std::string_view,std::string_view) const { return false; }
    virtual 稳定编码 建立输出动作(const 方法参数场景&, const 新字段询问材料&)=0;
    virtual 新字段询问入列读回 提交询问输出(
        const 方法参数场景&, const 新字段询问材料&, 稳定编码 动作,
        std::string_view UTF8正文)=0;
};
struct 新字段询问登记配置 final {
    稳定编码 需求根概念{}, 自我范围概念{}, 人类概念{}, 通用存在概念{}, 输出队列类型{};
    稳定编码 语言节点{}, 第三方指代概念{}, 第三方别名关系{};
    std::vector<稳定编码> 支持字段类型组;
    稳定编码 需求主体类型{};
    friend bool operator==(const 新字段询问登记配置&, const 新字段询问登记配置&)=default;
};
新字段询问材料读回 读取通用询问材料(
    const 方法参数场景&, const 新字段询问登记配置&,
    const 正式信息参数读取接口&);
class 新字段询问登记端口 {
public:
    virtual ~新字段询问登记端口()=default;
    virtual std::optional<新字段询问登记配置> 读取登记材料() const noexcept=0;
    virtual 新方法建立结果 确保方法登记(const 新方法知识建立请求&) noexcept=0;
    virtual 新方法建立结果 读取或确保发送方法登记() noexcept=0;
};

} // namespace 海中鱼巣
