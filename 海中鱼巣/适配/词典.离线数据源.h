#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
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

private:
    struct 实现;
    explicit 离线词典数据源(std::shared_ptr<const 实现>);
    std::shared_ptr<const 实现> 实现_;
};

} // namespace 海中鱼巣
