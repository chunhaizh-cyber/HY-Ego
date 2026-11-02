#pragma once
#include "接收认知来源.数据.h"
#include <optional>
#include <string>
#include <vector>
namespace 海中鱼巣 {
// 语言主张始终保留claim身份；对象身份得到证明不代表断言已成为现实特征事实。
enum class 新语言场景主张种类 : std::int64_t { 特征概念=1, 整数特征, 组成 };
struct 新语言场景主张对象 final {
    std::int64_t 出现编号=0;
    稳定编码 来源概念{};
    std::optional<稳定编码> 真实存在;
    friend bool operator==(const 新语言场景主张对象&,const 新语言场景主张对象&)=default;
};
struct 新语言场景主张内容 final {
    新语言场景主张种类 种类=新语言场景主张种类::整数特征;
    std::vector<新语言场景主张对象> 对象组;
    std::int64_t 主体或部分出现编号=0, 整体出现编号=0;
    std::optional<稳定编码> 特征类型概念, 特征值概念;
    std::optional<std::string> 规范整数;
    friend bool operator==(const 新语言场景主张内容&,const 新语言场景主张内容&)=default;
};
struct 新语言场景主张请求 final {
    新接收认知来源键 来源;
    稳定编码 来源语言场景{}, 目标场景{}, 当前场景{}, 语言{}, 说话者{};
    std::string 固定原文;
    新语言场景主张内容 内容;
    friend bool operator==(const 新语言场景主张请求&,const 新语言场景主张请求&)=default;
};
enum class 新语言场景主张状态 : std::uint8_t {
    已记录=1, 已复用, 待材料, 前提已变化, 来源冲突
};
struct 新语言场景主张记录 final {
    稳定编码 节点{}, 动作节点{};
    新语言场景主张请求 请求;
    friend bool operator==(const 新语言场景主张记录&,const 新语言场景主张记录&)=default;
};
struct 新语言场景主张结果 final {
    新语言场景主张状态 状态=新语言场景主张状态::待材料;
    std::optional<新语言场景主张记录> 记录;
    friend bool operator==(const 新语言场景主张结果&,const 新语言场景主张结果&)=default;
};
// 调用者交付唯一解释及其完整读取证据；正式表达对象可投影至此，不要求它已经存在。
struct 新语言场景写回调用上下文 final {
    稳定编码 执行者{};
    新语言场景主张请求 主张;
    bool 已取得唯一主张=false;
    std::vector<基础联合节点快照> 候选保护;
};
} // namespace 海中鱼巣
