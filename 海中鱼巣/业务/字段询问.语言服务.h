#pragma once

#include "../领域/本能字段询问.运行接口.h"
#include "正式表达需求.h"

namespace 海中鱼巣 {
class 新_语言类;
class 新_词类;
class 概念_名称关系类;
class 概念_存在类;
class 概念_特征类;
class 新_存在类;
class 新_自然语言世界树类;

// 只读投影与统一语言组合；不写需求、不选择待问字段、不入列、不发送。
class 字段询问语言服务 final : public 新字段询问语言接口 {
public:
    字段询问语言服务(const 新_语言类&,const 新_词类&,
        const 概念_名称关系类&,const 概念_存在类&,const 概念_特征类&,
        const 新_存在类&,const 新_自然语言世界树类&) noexcept;
    // 正式入口：消费正式表达需求，只返回候选话语（已唯一回读，含完整保护组）。
    新字段询问文字结果 生成候选话语(
        const 正式表达需求&,const 正式信息参数读取接口&) const;
    // 投影到本地接缝并填写正式表达需求节点；正式节点可空（值式本地请求）。
    新字段询问候选 生成字段询问候选(
        const 正式表达需求&,const 正式信息参数读取接口&,
        std::optional<稳定编码> 正式表达需求节点 = std::nullopt) const;
    // 薄适配：转成正式表达需求后走生成候选话语；供本能侧尚未切换前继续编译。
    新字段询问文字结果 生成字段询问(
        const 新正式字段询问目标&,const 正式信息参数读取接口&) const override;
private:
    const 新_语言类& 语言_;
    const 新_词类& 词_;
    const 概念_名称关系类& 名称_;
    const 概念_存在类& 存在概念_;
    const 概念_特征类& 特征概念_;
    const 新_存在类& 存在_;
    const 新_自然语言世界树类& 语言世界_;
};
} // namespace 海中鱼巣
