#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "新_语言类.h"

namespace 海中鱼巣 {

extern 稳定编码 词组织根节点;
extern 稳定编码 严格UTF8字节精确规范化合同节点;

struct 新词条建立请求 final {
    稳定编码 语言节点;
    稳定编码 规范化合同节点;
    std::string 规范化词面;
    std::optional<稳定编码> 创建来源节点;
};

struct 新词条信息 final {
    稳定编码 节点;
    稳定编码 语言节点;
    稳定编码 规范化合同节点;
    std::string 规范化词面;
    std::optional<稳定编码> 创建来源节点;
    friend bool operator==(const 新词条信息&, const 新词条信息&) = default;
};

enum class 新词条建立状态 : std::uint8_t {
    已建立 = 1,
    已复用 = 2,
    输入不合法 = 3,
    依赖不存在 = 4,
    结构不一致 = 5,
    资源失败 = 6
};

enum class 新词条查询状态 : std::uint8_t {
    已找到 = 1,
    未找到 = 2,
    输入不合法 = 3,
    结构不一致 = 4,
    资源失败 = 5
};

struct 新词条建立结果 final {
    新词条建立状态 状态 = 新词条建立状态::输入不合法;
    std::optional<稳定编码> 词条节点;
};

struct 新词条查询结果 final {
    新词条查询状态 状态 = 新词条查询状态::输入不合法;
    std::optional<新词条信息> 信息;
};

struct 新词条列表查询结果 final {
    新词条查询状态 状态 = 新词条查询状态::输入不合法;
    std::vector<稳定编码> 词条节点组;
};

// 词条只登记语言、规范化合同和规范化词面。它不分词、不规范化输入，
// 也不建立词义、概念或名称关系。
class 新_词类 final {
public:
    explicit 新_词类(新_语言类& 语言服务) noexcept;

    bool 初始化() noexcept;

    新词条建立结果 建立或取得词条(
        const 新词条建立请求& 请求) noexcept;
    新词条查询结果 获取词条(稳定编码 词条节点) const noexcept;
    新词条查询结果 按规范化词面查询(
        稳定编码 语言节点,
        稳定编码 规范化合同节点,
        const std::string& 规范化词面) const noexcept;
    新词条列表查询结果 查询语言词条(
        稳定编码 语言节点) const noexcept;
    bool 是词条节点(稳定编码 节点) const noexcept;

private:
    新_语言类& 语言服务_;
};

} // namespace 海中鱼巣
