#pragma once
// 统一双向入口：出站=正式世界关系内容→成句；入站=句子→自然语言场景材料（不写业务事实）。
// 共享关系概念、角色槽与表达知识；入站复用 解释单句含义图，不按种子键分派专用解析函数。
#include <map>
#include <string>
#include <vector>
#include "../领域/关系概念.数据.h"
#include "解释.基础语言.h"
#include "转换.语言场景基础.h"

namespace 海中鱼巣 {

class 关系概念类;
class 新_语言类;
class 新_词类;
class 概念_名称关系类;
class 概念_存在类;
class 概念_特征类;
class 新_存在类;
class 新_需求类;
class 新_自然语言世界树类;

enum class 关系陈述状态 : std::uint8_t {
    已生成且唯一回读 = 1,
    缺关系概念,
    端点不合法,
    关系不成立,     // owner/角色/方向与事实不符（出站核验）
    方向或角色错误,
    缺语言知识,     // 目标语言无表达关联/知识；不暗退中文
    缺名称,
    范围未支持,     // 字段引用等尚未接线的连接种类
    多义,
    前提已变化,
    输入拒绝
};

struct 关系陈述请求 final {
    std::string 关系种子键;
    std::map<std::string, 稳定编码> 端点绑定; // 角色键 → 正式节点
    稳定编码 目标语言节点{};
    std::optional<稳定编码> 说话者;
    std::optional<稳定编码> 受话者;
};

struct 关系陈述结果 final {
    关系陈述状态 状态 = 关系陈述状态::输入拒绝;
    std::string UTF8正文;
    std::vector<稳定编码> 所用知识节点组;
    std::optional<关系概念信息> 所用关系概念;
    bool 所用表达为预置 = false;
    std::string 诊断;
};

enum class 关系陈述理解状态 : std::uint8_t {
    已形成场景材料 = 1,
    多义待解,
    缺语言知识,
    角色不相容,
    缺名称,
    场景建立未完成,
    材料保存失败,
    输入拒绝,
    前提已变化,
    范围未支持
};

struct 关系陈述理解请求 final {
    std::string UTF8正文;
    稳定编码 目标语言节点{};
    std::optional<稳定编码> 交互场景; // 自我衔接键；写入入站材料
    std::optional<语言图交互上下文> 交互上下文;
    std::vector<语言图对象绑定> 对象绑定组; // 优先复用调用方已有绑定，不任取同名
    const 语言图专名绑定读取接口* 专名绑定读取 = nullptr;
};

struct 关系陈述理解结果 final {
    关系陈述理解状态 状态 = 关系陈述理解状态::输入拒绝;
    std::optional<关系陈述入站材料> 材料;
    std::string 诊断;
};

class 关系陈述语言服务 final {
public:
    关系陈述语言服务(
        关系概念类&,
        const 新_语言类&,
        const 新_词类&,
        const 概念_名称关系类&,
        const 概念_存在类&,
        const 概念_特征类&,
        const 新_存在类&,
        const 新_需求类&,
        新_自然语言世界树类&) noexcept;

    // 出站：正式内容 + 目标语言 → 成句或明确缺口。
    关系陈述结果 生成关系陈述(const 关系陈述请求&) const;

    // 入站：句子 → 解释含义图 → 匹配关系表达知识 → 自然语言场景材料（不写业务边/需求）。
    关系陈述理解结果 理解关系陈述(const 关系陈述理解请求&);

private:
    关系概念类& 关系_;
    const 新_语言类& 语言_;
    const 新_词类& 词_;
    const 概念_名称关系类& 名称_;
    const 概念_存在类& 存在概念_;
    const 概念_特征类& 特征概念_;
    const 新_存在类& 存在_;
    const 新_需求类& 需求_;
    新_自然语言世界树类& 语言世界_;
};

} // namespace 海中鱼巣
