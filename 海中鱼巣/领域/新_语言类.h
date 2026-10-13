#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "../核心/基础数据集.h"

namespace 海中鱼巣 {

class 新_词类;

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

struct 新语言词条匹配片段 final {
    std::optional<稳定编码> 词条节点;
    std::size_t 原文字节起点 = 0;
    std::size_t 原文字节长度 = 0;
    std::size_t 原文码点起点 = 0;
    std::size_t 原文码点数量 = 0;
    std::string 原文;
    friend bool operator==(const 新语言词条匹配片段&,
        const 新语言词条匹配片段&) = default;
};

struct 新语言词条匹配结果 final {
    新语言查询状态 状态 = 新语言查询状态::输入不合法;
    std::vector<新语言词条匹配片段> 片段组;
};

// 语言节点登记语言身份；词面匹配只组合公开读取，不解释词义或绑定概念。
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

    // UTF-8 字节精确：不折叠大小写/空白，不做 NFC/NFKC 或编码修复；
    // 非空输入沿用严格校验（包括拒绝 U+FEFF、U+FFFD，允许 NUL）。
    // 原文区间为零基起点+长度；未知连续码点合并，引用为空。
    // 已找到表示完整处理（含空文本/全未知），失败总是返回空片段组。
    // 词库以本次枚举到的 ID 为界，不含随后新登记词；不持语言锁调用词服务。
    新语言词条匹配结果 按已登记词条最长匹配(
        稳定编码 语言节点,
        const std::string& 文本,
        const 新_词类& 词服务) const noexcept;

};

} // namespace 海中鱼巣
