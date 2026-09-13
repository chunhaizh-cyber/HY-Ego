module;

#include <algorithm>
#include <array>
#include <cstdint>
#include <numeric>
#include <optional>
#include <utility>
#include <variant>
#include <vector>

export module 海中鱼巣.领域.合同.特征解释;

export import 海中鱼巣.领域.合同.结构操作公共;
export import 海中鱼巣.领域.数据服务.特征值类;

export namespace 海中鱼巣 {

struct 分量解释_B1 final {
  稳定编码 角色{}, 单位{};
  std::uint64_t 缩放分子{}, 缩放分母{};
  friend bool operator==(const 分量解释_B1 &, const 分量解释_B1 &) = default;
};

using 完整向量点_B1 =
    std::variant<std::vector<std::int64_t>, std::vector<std::uint64_t>>;

struct I64有限区间域_B1 final {
  std::vector<std::pair<std::int64_t, std::int64_t>> 区间;
  friend bool operator==(const I64有限区间域_B1 &,
                         const I64有限区间域_B1 &) = default;
};
struct I64组有限完整域_B1 final {
  std::vector<std::vector<std::int64_t>> 点;
  friend bool operator==(const I64组有限完整域_B1 &,
                         const I64组有限完整域_B1 &) = default;
};
struct U64组有限完整域_B1 final {
  std::vector<std::vector<std::uint64_t>> 点;
  friend bool operator==(const U64组有限完整域_B1 &,
                         const U64组有限完整域_B1 &) = default;
};
struct 材料有限完整域_B1 final {
  std::vector<不可变材料身份_B1> 完整代表;
  friend bool operator==(const 材料有限完整域_B1 &,
                         const 材料有限完整域_B1 &) = default;
};
using 特征完整允许域_B1 =
    std::variant<I64有限区间域_B1, I64组有限完整域_B1,
                 U64组有限完整域_B1, 材料有限完整域_B1>;

enum class 完整材料解析族_B1 : std::uint8_t {
  有序I64完整载荷 = 1,
  有序U64完整载荷 = 2
};
enum class 完整内容判等规则_B1 : std::uint8_t {
  有符号标量 = 1,
  有序I64全分量 = 2,
  有序U64全分量 = 3,
  完整解析载荷 = 4
};

struct 先天完整特征类型规格_B1 final {
  稳定编码 外设提供者{};
  std::vector<分量解释_B1> 分量;
  特征完整允许域_B1 允许域;
  std::optional<完整材料解析族_B1> 材料解析;
  完整内容判等规则_B1 精确判等{完整内容判等规则_B1::有符号标量};
  friend bool operator==(const 先天完整特征类型规格_B1 &,
                         const 先天完整特征类型规格_B1 &) = default;
};

struct 材料解析见证_B1 final {
  不可变材料身份_B1 材料;
  不可变材料格式身份_B1 格式;
  std::array<std::uint8_t, 32> 摘要{};
  std::uint64_t 字节长度{};
  完整材料解析族_B1 解析族{完整材料解析族_B1::有序I64完整载荷};
  std::uint32_t 规则版本{1};
  完整向量点_B1 完整点;
  结构生命周期_B1 材料生命周期;
  friend bool operator==(const 材料解析见证_B1 &,
                         const 材料解析见证_B1 &) = default;
};

struct 完整属性见证_B1 final {
  稳定编码 编码{}, 所属{}, 属性类型{}, 来源{};
  特征值内容 内容;
  结构生命周期_B1 生命周期;
  friend bool operator==(const 完整属性见证_B1 &,
                         const 完整属性见证_B1 &) = default;
};

struct 完整值见证_B1 final {
  特征值内容 内容;
  std::optional<不可变材料身份_B1> 材料;
  std::optional<材料解析见证_B1> 解析;
  friend bool operator==(const 完整值见证_B1 &,
                         const 完整值见证_B1 &) = default;
};

struct 原标量域形成_B1 final {
  std::int64_t 允许误差{};
  稳定编码 参数来源{};
  friend bool operator==(const 原标量域形成_B1 &,
                         const 原标量域形成_B1 &) = default;
};
struct 原标量类型见证_B1 final {
  稳定编码 FT{}, 外设提供者{}, 单位{};
  std::uint64_t 缩放分子{}, 缩放分母{};
  std::vector<std::pair<std::int64_t, std::int64_t>> 允许区间;
  std::optional<原标量域形成_B1> 域形成;
  std::optional<稳定编码> 原比较规则;
  std::vector<结构关系见证_B1> 类型自有关系;
  std::vector<完整属性见证_B1> 类型自有属性;
  friend bool operator==(const 原标量类型见证_B1 &,
                         const 原标量类型见证_B1 &) = default;
};

struct 完整特征类型事实_B1 final {
  稳定编码 FT{};
  结构节点见证_B1 节点;
  结构关系见证_B1 族边;
  std::variant<原标量类型见证_B1, 先天完整特征类型规格_B1> 规格;
  std::optional<完整属性见证_B1> 规格值, 域值;
  std::optional<结构关系见证_B1> 来源边;
  std::vector<结构关系见证_B1> 分量角色边, 分量单位边, 材料代表边;
  std::vector<材料解析见证_B1> 材料代表;
  friend bool operator==(const 完整特征类型事实_B1 &,
                         const 完整特征类型事实_B1 &) = default;
};

struct 准确特征完整事实_B1 final {
  稳定编码 F{};
  结构节点见证_B1 节点;
  结构关系见证_B1 族边, 类型边;
  完整属性见证_B1 准确属性;
  完整特征类型事实_B1 类型;
  完整值见证_B1 准确内容;
  friend bool operator==(const 准确特征完整事实_B1 &,
                         const 准确特征完整事实_B1 &) = default;
};

struct 完整采用事实_B1 final {
  稳定编码 E{}, FT{};
  std::optional<结构关系见证_B1> 采用边, 已知边;
  std::optional<准确特征完整事实_B1> 当前F;
  friend bool operator==(const 完整采用事实_B1 &,
                         const 完整采用事实_B1 &) = default;
};

inline bool 分量解释有效(const 分量解释_B1 &v) noexcept {
  return 有效(v.角色) && 有效(v.单位) && v.缩放分子 != 0 &&
         v.缩放分母 != 0 && std::gcd(v.缩放分子, v.缩放分母) == 1;
}

} // namespace 海中鱼巣
