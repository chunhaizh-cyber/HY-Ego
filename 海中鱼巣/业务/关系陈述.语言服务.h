#pragma once
// 统一出站入口：正式世界关系内容 + 目标语言 → 成句或明确缺口。
// 内部：读关系概念 → owner 正式核验边/方向 → 选目标语言表达槽 → 按槽统一构图 → 成文回读。
// 差异由表达关联的含义操作+角色槽知识提供；不按关系种子键/opcode 分派专用构图函数。
#include <map>
#include <string>
#include <vector>
#include "../领域/关系概念.数据.h"
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
    关系不成立,     // owner/角色/方向与事实不符
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

class 关系陈述语言服务 final {
public:
    关系陈述语言服务(
        const 关系概念类&,
        const 新_语言类&,
        const 新_词类&,
        const 概念_名称关系类&,
        const 概念_存在类&,
        const 概念_特征类&,
        const 新_存在类&,
        const 新_需求类&,
        const 新_自然语言世界树类&) noexcept;

    // 生产入口：只接收要表达的正式内容与目标语言；构图差异读表达角色槽知识。
    关系陈述结果 生成关系陈述(const 关系陈述请求&) const;

private:
    const 关系概念类& 关系_;
    const 新_语言类& 语言_;
    const 新_词类& 词_;
    const 概念_名称关系类& 名称_;
    const 概念_存在类& 存在概念_;
    const 概念_特征类& 特征概念_;
    const 新_存在类& 存在_;
    const 新_需求类& 需求_;
    const 新_自然语言世界树类& 语言世界_;
};

} // namespace 海中鱼巣
