#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include "../核心/基础数据集.h"

namespace 海中鱼巣 {

// 来自只读词典的值式义项材料；不依赖加载服务或执行身份。
struct 词典义项材料 final {
    std::string 记录键, 词头语种, 词面, 义项键, 汉语释义;
    std::string 例句原文, 例句汉译, 读音;
    std::string 来源页URL, 来源版本, 许可标识, 署名, 原始来源定位;
    friend bool operator==(const 词典义项材料&, const 词典义项材料&) = default;
};

struct 新词典查询键 final {
    稳定编码 词条语言, 规范化合同;
    std::string 原规范化词面, 查询词面, 源语种;
    friend bool operator==(const 新词典查询键&, const 新词典查询键&) = default;
};

// 外设身份仅作追溯定位；不是内部世界对外设节点的强引用。
struct 新词典外设来源标识 final {
    std::uint64_t 场景 = 0, 查询记录 = 0;
    friend bool operator==(const 新词典外设来源标识&, const 新词典外设来源标识&) = default;
};

enum class 新词典材料操作状态 : std::uint8_t {
    已完成 = 1, 已复用, 未找到, 条件不具备,
    输入拒绝, 未装配, 未支持, 资源失败, 引用仍存在
};

enum class 新词典查阅结局 : std::uint8_t { 命中 = 1, 正式未命中 = 2 };

enum class 新词典学习缺口种类 : std::uint8_t {
    参数未定位 = 1, 参数特征未形成, 参数当前值未取得,
    来源不可用, 方法条件未具备
};

struct 新词典学习缺口 final {
    新词典学习缺口种类 种类{};
    std::uint32_t 参数序号 = 0;
    std::optional<稳定编码> 信息节点, 特征类型;
    std::optional<std::uint64_t> 条件项序号;
    friend bool operator==(const 新词典学习缺口&, const 新词典学习缺口&) = default;
};

} // namespace 海中鱼巣
