#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "../核心/基础数据集.h"

namespace 海中鱼巣 {

// 自然语言世界树与现实世界树并列。它只组织彼此不保证关联的语言场景，
// 不保存概念副本，也不把语言场景自动解释为现实场景。
extern 稳定编码 自然语言世界树根节点;

struct 新自然语言场景信息 final {
    稳定编码 节点;
    friend bool operator==(const 新自然语言场景信息&,
        const 新自然语言场景信息&) = default;
};

enum class 新自然语言世界初始化状态 : std::uint8_t {
    已完成 = 1,
    无变化 = 2,
    结构不一致 = 3,
    资源失败 = 4
};

struct 新自然语言世界初始化结果 final {
    新自然语言世界初始化状态 状态 =
        新自然语言世界初始化状态::结构不一致;
    std::optional<稳定编码> 根节点;

    bool 成功() const noexcept;
};

enum class 新自然语言场景操作状态 : std::uint8_t {
    已建立 = 1,
    未初始化 = 2,
    入口拒绝 = 3,
    结构不一致 = 4,
    资源失败 = 5
};

struct 新自然语言场景建立结果 final {
    新自然语言场景操作状态 状态 =
        新自然语言场景操作状态::入口拒绝;
    std::optional<稳定编码> 场景节点;
};

// 该类只拥有自然语言世界根和空语言场景的结构身份。场景中的存在、
// 关系、特征及来源字段在相应合同冻结前不在这里猜测建立。
class 新_自然语言世界树类 final {
public:
    新自然语言世界初始化结果 初始化() noexcept;
    bool 已初始化() const noexcept;
    std::optional<稳定编码> 获取根节点() const noexcept;

    // 每次调用建立一个彼此独立的空语言场景；根下父子关系只用于组织。
    新自然语言场景建立结果 建立空场景() noexcept;
    std::optional<新自然语言场景信息> 获取场景(
        稳定编码 场景节点) const noexcept;
    std::vector<稳定编码> 查询全部场景() const noexcept;
    bool 是自然语言场景节点(稳定编码 节点) const noexcept;
};

} // namespace 海中鱼巣
