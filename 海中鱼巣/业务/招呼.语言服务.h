#pragma once
// CONV-EXPR-TEXT §3.5 招呼成文：表达需求“招呼” → 表达组织知识 → 含义网 → 投影 → 生成并整段回读。
// 只读：不写场景、不建立动作、不留档、不入列；这些由建立连接本能（INSTINCT-CONTRACT 阶段6）承担。
#include <string>
#include <vector>
#include "正式表达需求.h"

namespace 海中鱼巣 {
class 新_语言类;
class 新_词类;
class 概念_名称关系类;
class 概念_存在类;
class 概念_特征类;
class 新_存在类;
class 新_自然语言世界树类;

enum class 新招呼话语状态 : std::uint8_t {
    已生成且唯一回读=1,
    缺知识,        // 缺描述结构或缺组织知识：建立连接“打招呼所需语言知识齐备”复核不通过，零动作
    多规则待选,    // 同一种类有多种可用说法；选用属于自我意识，本服务不选
    多义,          // 名称待选或回读多义
    前提已变化,    // 读取漂移或组织材料复读不一致
    范围未支持     // 投影或生成不支持该形状
};
struct 新招呼话语结果 final {
    新招呼话语状态 状态=新招呼话语状态::缺知识;
    std::string UTF8正文;
    std::vector<稳定编码> 所用知识节点组;
    新语言含义网 含义网;
    稳定编码 所用组织知识节点{};
};
class 新招呼语言接口 {
public:
    virtual ~新招呼语言接口()=default;
    // 说话者、受话者只作本次成文的参与者；招呼套语本身不携带角色。工程故障以异常上送。
    virtual 新招呼话语结果 生成招呼话语(新招呼种类,const 新绑定指称& 说话者,const 新绑定指称& 受话者,
        const 正式表达成文参数&) const=0;
};
class 招呼语言服务 final : public 新招呼语言接口 {
public:
    招呼语言服务(const 新_语言类&,const 新_词类&,const 概念_名称关系类&,const 概念_存在类&,
        const 概念_特征类&,const 新_存在类&,const 新_自然语言世界树类&) noexcept;
    新招呼话语结果 生成招呼话语(新招呼种类,const 新绑定指称&,const 新绑定指称&,
        const 正式表达成文参数&) const override;
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
