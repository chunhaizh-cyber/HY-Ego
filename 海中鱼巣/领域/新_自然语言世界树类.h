#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "../核心/基础数据集.h"

namespace 海中鱼巣 {

class 概念_特征类;
class 概念_存在类;

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

enum class 新自然语言概念实例种类 : std::uint8_t {
    存在概念 = 1,
    特征概念 = 2
};

struct 新自然语言概念实例信息 final {
    稳定编码 节点;
    稳定编码 所属场景节点;
    稳定编码 概念节点;
    新自然语言概念实例种类 种类 =
        新自然语言概念实例种类::存在概念;
    friend bool operator==(const 新自然语言概念实例信息&,
        const 新自然语言概念实例信息&) = default;
};

enum class 新自然语言概念实例操作状态 : std::uint8_t {
    已建立 = 1,
    未初始化 = 2,
    场景不存在 = 3,
    概念不存在 = 4,
    结构不一致 = 5,
    资源失败 = 6
};

struct 新自然语言概念实例建立结果 final {
    新自然语言概念实例操作状态 状态 =
        新自然语言概念实例操作状态::概念不存在;
    std::optional<新自然语言概念实例信息> 实例;

    bool 成功() const noexcept;
};

// 该类拥有自然语言世界根、语言场景及场景中的概念实例结构。概念实例
// 只引用已经选定的概念，不复制词条、概念定义、值域或现实准确值。
class 新_自然语言世界树类 final {
public:
    新_自然语言世界树类(
        const 概念_特征类& 特征概念服务,
        const 概念_存在类& 存在概念服务) noexcept;

    新自然语言世界初始化结果 初始化() noexcept;
    bool 已初始化() const noexcept;
    std::optional<稳定编码> 获取根节点() const noexcept;

    // 每次调用建立一个彼此独立的空语言场景；根下父子关系只用于组织。
    新自然语言场景建立结果 建立空场景() noexcept;
    std::optional<新自然语言场景信息> 获取场景(
        稳定编码 场景节点) const noexcept;
    std::vector<稳定编码> 查询全部场景() const noexcept;
    bool 是自然语言场景节点(稳定编码 节点) const noexcept;

    // 每次调用都建立新的语言概念实例；同一场景中的同一概念可以重复
    // 出现。词条召回和候选消歧由调用方在本函数之前完成。
    新自然语言概念实例建立结果 实例化概念(
        稳定编码 场景节点,
        稳定编码 概念节点) noexcept;
    std::optional<新自然语言概念实例信息> 获取概念实例(
        稳定编码 实例节点) const noexcept;
    std::vector<稳定编码> 查询场景概念实例(
        稳定编码 场景节点) const noexcept;
    bool 是自然语言概念实例节点(稳定编码 节点) const noexcept;

private:
    const 概念_特征类& 特征概念服务_;
    const 概念_存在类& 存在概念服务_;
};

} // namespace 海中鱼巣
