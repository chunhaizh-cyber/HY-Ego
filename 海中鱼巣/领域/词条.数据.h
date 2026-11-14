#pragma once

#include <cstdint>
#include <optional>

#include "../核心/基础数据集.h"

namespace 海中鱼巣 {

enum class 新场景操作状态 : std::uint8_t;

// 只挂接已经发生的变化；实现者持有正确的自然语言场景owner。
class 新词条变化场景写入接口 {
public:
    virtual ~新词条变化场景写入接口() = default;
    virtual 新场景操作状态 挂接已发生词条变化动态(
        稳定编码 所属语言场景, 稳定编码 动态) noexcept = 0;
};

// 保持原动态原因的数值；纯结果材料不依赖词条服务。
enum class 新词条动态记录原因 : std::uint8_t {
    无值变化, 无前态, 时间不递增, 已形成
};

struct 新词条特征定位 final {
    稳定编码 特征类型根, 特征节点;
    friend bool operator==(const 新词条特征定位&, const 新词条特征定位&) = default;
};

enum class 新词条特征取得状态 : std::uint8_t {
    已建立 = 1, 已复用, 词条不存在, 类型未支持,
    场景未装配, 资源失败
};

struct 新词条特征取得结果 final {
    新词条特征取得状态 状态 = 新词条特征取得状态::词条不存在;
    std::optional<新词条特征定位> 特征;
};

class 新词条材料特征写入接口 {
public:
    virtual ~新词条材料特征写入接口() = default;
    virtual 新词条特征取得结果 建立或取得词条特征(
        稳定编码 词条, 稳定编码 特征类型) noexcept = 0;
};

} // namespace 海中鱼巣
