#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace 海中鱼巣 {

enum class 词典操作状态 : std::uint8_t {
    已完成 = 1,
    未命中,
    未配置,
    输入不合法,
    数据错误,
    读取失败,
    资源失败
};

struct 词典义项材料 final {
    std::string 记录键, 词头语种, 词面, 义项键, 汉语释义;
    std::string 例句原文, 例句汉译, 读音;
    std::string 来源页URL, 来源版本, 许可标识, 署名, 原始来源定位;
};

struct 词典精确查询请求 final {
    std::string 词头语种, 词面;
};

struct 词典前缀查询请求 final {
    std::string 词头语种, 原文;
    std::size_t 起始标量 = 0;
};

struct 词典前缀命中 final {
    std::size_t 起始标量 = 0, 结束标量 = 0;
    std::vector<词典义项材料> 义项组;
};

struct 词典精确查询结果 final {
    词典操作状态 状态 = 词典操作状态::输入不合法;
    std::string 包标识, 诊断;
    std::vector<词典义项材料> 义项组;
};

struct 词典前缀查询结果 final {
    词典操作状态 状态 = 词典操作状态::输入不合法;
    std::string 包标识, 诊断;
    std::vector<词典前缀命中> 命中组;
};

class 离线词典数据源;

struct 词典打开结果 final {
    词典操作状态 状态 = 词典操作状态::输入不合法;
    std::string 包标识, 诊断;
    std::size_t 词面数 = 0, 义项数 = 0;
    std::shared_ptr<const 离线词典数据源> 数据源;
};

// 外部来源材料只读提供者；不登记正式词条、概念或世界事实。
// 完整准入后才交付不可变源；查询副本由调用方独占，寿命由 shared_ptr 保护。
class 离线词典数据源 final {
public:
    static 词典打开结果 打开(const std::filesystem::path& 目录) noexcept;
    词典精确查询结果 精确查询(const 词典精确查询请求&) const noexcept;
    // 返回指定标量起点的全部前缀候选，不裁决英语词边界或唯一分词。
    词典前缀查询结果 查询词面前缀(const 词典前缀查询请求&) const noexcept;

    // 读词典释义用辅助表（7250 §3.4第3、6项：词头表、繁简表〔OpenCC〕、词性表）。
    // 仅完整同批包启用；批次损坏使打开失败。无批次的旧包仅保留词义查询。
    bool 释义辅助表就绪() const noexcept;
    std::string 释义辅助表诊断() const;
    // 繁简规范化（OpenCC 词组优先、再逐字）：词组繁简等长，标量数不变，原文标量位置即规范化后位置；
    // 原文非法 UTF-8 或辅助表未就绪返回空。
    std::optional<std::string> 繁简规范化(std::string_view 原文) const;
    // 规范化后的词面是否为中文词头（读释义切分的边界依据，不登记词条）。
    bool 是规范化词头(std::string_view 规范化词面) const noexcept;
    std::size_t 最长词头标量数() const noexcept;
    // 按词典记录键读取该记录的原始词性；来源包不一致或无记录返回空。
    std::optional<std::string> 读取记录词性(std::string_view 记录键) const;

private:
    struct 实现;
    explicit 离线词典数据源(std::shared_ptr<const 实现>);
    std::shared_ptr<const 实现> 实现_;
};

} // namespace 海中鱼巣
