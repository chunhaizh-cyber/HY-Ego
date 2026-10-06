#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include "../核心/基础数据集.h"

namespace 海中鱼巣 {

// 自然语言材料只在全局基础数据集中组织，不进入世界树。
extern 稳定编码 自然语言组织根节点;
extern 稳定编码 语言组织根节点;

struct 新语言建立请求 final {
    std::string 登记键;
    std::optional<稳定编码> 创建来源节点;
};

struct 新语言信息 final {
    稳定编码 节点;
    std::string 登记键;
    std::optional<稳定编码> 创建来源节点;
    friend bool operator==(const 新语言信息&, const 新语言信息&) = default;
};

enum class 新语言建立状态 : std::uint8_t {
    已建立 = 1,
    已复用 = 2,
    输入不合法 = 3,
    依赖不存在 = 4,
    结构不一致 = 5,
    资源失败 = 6
};

enum class 新语言查询状态 : std::uint8_t {
    已找到 = 1,
    未找到 = 2,
    输入不合法 = 3,
    结构不一致 = 4,
    资源失败 = 5
};

struct 新语言建立结果 final {
    新语言建立状态 状态 = 新语言建立状态::输入不合法;
    std::optional<稳定编码> 语言节点;
};

struct 新语言查询结果 final {
    新语言查询状态 状态 = 新语言查询状态::输入不合法;
    std::optional<新语言信息> 信息;
};

// 语言节点只登记语言身份。它不执行分词、词义理解或概念绑定。
class 新_语言类 final {
public:
    // 只做第一阶段 UTF-8 合法性校验，不改变输入字节。
    static bool 严格UTF8字节串有效(const std::string& 文本) noexcept;

    bool 初始化() noexcept;

    新语言建立结果 建立或取得语言(
        const 新语言建立请求& 请求) noexcept;
    新语言查询结果 获取语言(稳定编码 语言节点) const noexcept;
    新语言查询结果 按登记键查询(
        const std::string& 登记键) const noexcept;
    bool 是语言节点(稳定编码 节点) const noexcept;

};

} // namespace 海中鱼巣
