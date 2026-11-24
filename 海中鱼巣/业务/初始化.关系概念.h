#pragma once
#include "../领域/关系概念类.h"
#include "../领域/新_词类.h"
#include <string>

namespace 海中鱼巣 {

struct 初始化关系概念请求 final {
    std::string 种子文本;
    稳定编码 中文语言节点{};
    std::optional<稳定编码> 英文语言节点;
};

enum class 初始化关系概念状态 : std::uint8_t {
    已完成 = 1,
    前置拒绝,
    语法拒绝,
    写入失败,
    读回失败
};

struct 初始化关系概念结果 final {
    初始化关系概念状态 状态 = 初始化关系概念状态::前置拒绝;
    std::vector<关系概念信息> 项组;
    std::string 诊断;
};

// 预置种子 → 建立或取得；重复调用形状相同则复用且不覆盖后天表达关联。
初始化关系概念结果 初始化世界关系概念(
    const 初始化关系概念请求&,
    关系概念类&,
    新_词类&);

初始化关系概念结果 读取世界关系概念种子(
    const 初始化关系概念请求&,
    const 关系概念类&,
    const 新_词类&);

} // namespace 海中鱼巣
