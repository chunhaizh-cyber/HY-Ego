module;

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <variant>
#include <vector>

export module 海中鱼巣.领域.合同.因果结构;

export import 海中鱼巣.领域.合同.动态结构;

export namespace 海中鱼巣 {

inline constexpr std::uint32_t 因果数据合同版本 = 1;

struct 因果信息身份 final {
  稳定编码 编码{};
  friend bool operator==(const 因果信息身份 &, const 因果信息身份 &) = default;
};
struct 因果存在概念引用 final {
  稳定编码 编码{};
  friend bool operator==(const 因果存在概念引用 &,
                         const 因果存在概念引用 &) = default;
};
struct 因果特征概念引用 final {
  稳定编码 编码{};
  friend bool operator==(const 因果特征概念引用 &,
                         const 因果特征概念引用 &) = default;
};
struct 因果关系概念引用 final {
  稳定编码 编码{};
  friend bool operator==(const 因果关系概念引用 &,
                         const 因果关系概念引用 &) = default;
};
struct 因果动作概念引用 final {
  稳定编码 编码{};
  friend bool operator==(const 因果动作概念引用 &,
                         const 因果动作概念引用 &) = default;
};
struct 因果链概念引用 final {
  稳定编码 编码{};
  friend bool operator==(const 因果链概念引用 &,
                         const 因果链概念引用 &) = default;
};
inline bool 有效(因果信息身份 v) noexcept { return 有效(v.编码); }
inline bool 有效(因果存在概念引用 v) noexcept { return 有效(v.编码); }
inline bool 有效(因果特征概念引用 v) noexcept { return 有效(v.编码); }
inline bool 有效(因果关系概念引用 v) noexcept { return 有效(v.编码); }
inline bool 有效(因果动作概念引用 v) noexcept { return 有效(v.编码); }
inline bool 有效(因果链概念引用 v) noexcept { return 有效(v.编码); }

enum class 因果类型 : std::uint8_t { 主动 = 1, 被动 = 2 };
enum class 因果角色同异 : std::uint8_t { 同一 = 1, 不同 = 2 };
struct 因果角色项 final {
  std::uint32_t 角色 = 0;
  因果存在概念引用 存在概念;
  friend bool operator==(const 因果角色项 &, const 因果角色项 &) = default;
};
struct 因果角色关系 final {
  std::uint32_t 左角色 = 0, 右角色 = 0;
  因果角色同异 关系 = 因果角色同异::不同;
  friend bool operator==(const 因果角色关系 &, const 因果角色关系 &) = default;
};
struct 因果参与者定义 final {
  std::vector<因果角色项> 角色组;
  std::vector<因果角色关系> 同异约束组;
  friend bool operator==(const 因果参与者定义 &,
                         const 因果参与者定义 &) = default;
};
struct 因果概念域约束 final {
  friend bool operator==(const 因果概念域约束 &,
                         const 因果概念域约束 &) = default;
};
struct 因果准确值约束 final {
  特征准确值 值;
  friend bool operator==(const 因果准确值约束 &,
                         const 因果准确值约束 &) = default;
};
struct 因果I64区间约束 final {
  std::int64_t 下界 = 0, 上界 = 0;
  friend bool operator==(const 因果I64区间约束 &,
                         const 因果I64区间约束 &) = default;
};
struct 因果二元关系约束 final {
  因果关系概念引用 关系概念;
  std::uint32_t 被描述角色A = 0, 参照角色B = 0;
  friend bool operator==(const 因果二元关系约束 &,
                         const 因果二元关系约束 &) = default;
};
using 因果条件约束 = std::variant<因果概念域约束, 因果准确值约束,
                                  因果I64区间约束, 因果二元关系约束>;
struct 因果条件项 final {
  std::uint32_t 序号 = 0, 参与者角色 = 0;
  因果特征概念引用 特征概念;
  因果条件约束 约束;
  friend bool operator==(const 因果条件项 &, const 因果条件项 &) = default;
};
struct 因果动作参数约束 final {
  std::uint32_t 参数角色 = 0;
  因果特征概念引用 特征概念;
  因果条件约束 约束;
  friend bool operator==(const 因果动作参数约束 &,
                         const 因果动作参数约束 &) = default;
};
struct 因果动作项 final {
  因果动作概念引用 动作概念;
  std::uint32_t 执行者角色 = 0;
  std::optional<std::uint32_t> 作用对象角色;
  std::vector<因果动作参数约束> 参数组;
  friend bool operator==(const 因果动作项 &, const 因果动作项 &) = default;
};
struct 因果相对结果约束 final {
  std::uint32_t 基准条件序号 = 0;
  因果关系概念引用 变化关系概念;
  friend bool operator==(const 因果相对结果约束 &,
                         const 因果相对结果约束 &) = default;
};
using 因果结果约束 = std::variant<因果概念域约束, 因果准确值约束,
                                  因果I64区间约束, 因果相对结果约束>;
enum class 因果时序端类别 : std::uint8_t {
  条件 = 1,
  动作开始 = 2,
  结果 = 3,
  动作完成 = 4
};
struct 因果时序端 final {
  因果时序端类别 类别 = 因果时序端类别::条件;
  std::uint32_t 序号 = 0;
  friend bool operator==(const 因果时序端 &, const 因果时序端 &) = default;
};
struct 因果形成时限 final {
  因果时序端 起点;
  std::int64_t 最迟形成纳秒 = 0;
  friend bool operator==(const 因果形成时限 &, const 因果形成时限 &) = default;
};
struct 因果结果项 final {
  std::uint32_t 序号 = 0, 承担者角色 = 0;
  因果特征概念引用 特征概念;
  因果结果约束 约束;
  std::optional<因果形成时限> 时限;
  friend bool operator==(const 因果结果项 &, const 因果结果项 &) = default;
};
struct 因果相对时序项 final {
  因果时序端 前端, 后端;
  std::int64_t 最小间隔纳秒 = 0, 最大间隔纳秒 = 0;
  friend bool operator==(const 因果相对时序项 &,
                         const 因果相对时序项 &) = default;
};
enum class 因果来源用途 : std::uint8_t { 简化 = 1, 补完 = 2 };
using 因果定义来源目标 = std::variant<因果信息身份, 因果链概念引用>;
struct 因果定义来源项 final {
  因果来源用途 用途 = 因果来源用途::简化;
  因果定义来源目标 目标;
  friend bool operator==(const 因果定义来源项 &,
                         const 因果定义来源项 &) = default;
};
struct 因果定义 final {
  因果类型 类型 = 因果类型::被动;
  因果参与者定义 参与者;
  std::vector<因果条件项> 条件组;
  std::optional<因果动作项> 动作;
  std::vector<因果结果项> 结果组;
  std::vector<因果相对时序项> 时序组;
  std::vector<因果定义来源项> 来源组;
  friend bool operator==(const 因果定义 &, const 因果定义 &) = default;
};
struct 因果信息 final {
  因果信息身份 身份;
  因果定义 定义;
  friend bool operator==(const 因果信息 &, const 因果信息 &) = default;
};
struct 因果读取预算 final {
  std::uint64_t 最大候选数 = 0, 最大定义项数 = 0, 最大关系数 = 0,
                最大值元素数 = 0, 最大来源材料数 = 0, 最大展开深度 = 0;
  friend bool operator==(const 因果读取预算 &, const 因果读取预算 &) = default;
};

enum class 因果数据状态 : std::uint8_t {
  已创建 = 1,
  已复用 = 2,
  精确重复 = 3,
  已读取 = 4,
  已关联 = 5,
  已退出 = 6,
  入口拒绝 = 7,
  未找到 = 8,
  目标已退出 = 9,
  概念不相容 = 10,
  约束不支持 = 11,
  角色冲突 = 12,
  时序冲突 = 13,
  来源不足 = 14,
  动作来源未实现 = 15,
  历史材料已清理 = 16,
  数量预算不足 = 17,
  事实代次漂移 = 18,
  幂等冲突 = 19,
  引用冲突 = 20,
  旧格式不支持 = 21,
  依赖未实现 = 22,
  资源失败 = 23,
  内部不一致 = 24,
  已可能发布 = 25
};
enum class 因果发布阶段 : std::uint8_t {
  无写入 = 0,
  已提交待读回 = 1,
  已读回 = 2,
  未知 = 3
};
struct 因果操作结果 final {
  因果数据状态 状态 = 因果数据状态::入口拒绝;
  std::uint32_t 合同版本 = 1;
  std::uint64_t Gread = 0, H = 0;
  std::optional<std::uint64_t> 发布代次;
  因果发布阶段 阶段 = 因果发布阶段::无写入;
  friend bool operator==(const 因果操作结果 &, const 因果操作结果 &) = default;
};
struct 因果内容事实 final {
  std::uint64_t Gread = 0, H = 0;
  因果信息 信息;
  稳定编码 族归属关系{}, 定义值{};
  std::vector<稳定编码> 定义引用关系组;
  std::uint64_t 创建事实代次 = 0;
  std::optional<std::uint64_t> 退出事实代次;
  friend bool operator==(const 因果内容事实 &, const 因果内容事实 &) = default;
};
struct 因果证据角色绑定 final {
  std::uint32_t 角色 = 0;
  稳定编码 存在{};
  friend bool operator==(const 因果证据角色绑定 &,
                         const 因果证据角色绑定 &) = default;
};
enum class 因果证据项类别 : std::uint8_t { 条件 = 1, 结果 = 2 };
struct 因果状态证据项 final {
  因果证据项类别 类别 = 因果证据项类别::条件;
  std::uint32_t 定义项序号 = 0;
  动态信息身份 原子来源;
  状态使用绑定身份 绑定;
  friend bool operator==(const 因果状态证据项 &,
                         const 因果状态证据项 &) = default;
};
struct 因果动作来源引用 final {
  稳定编码 关系身份{};
  friend bool operator==(const 因果动作来源引用 &,
                         const 因果动作来源引用 &) = default;
};
struct 因果证据提交 final {
  动态信息身份 发生锚点;
  std::uint64_t 证据截止 = 0;
  std::vector<因果证据角色绑定> 角色绑定组;
  std::vector<因果状态证据项> 状态证据组;
  std::optional<因果动作来源引用> 动作来源;
  friend bool operator==(const 因果证据提交 &, const 因果证据提交 &) = default;
};
struct 因果证据定位 final {
  因果信息身份 因果;
  动态信息身份 发生锚点;
  friend bool operator==(const 因果证据定位 &, const 因果证据定位 &) = default;
};
struct 因果证据事实 final {
  std::uint64_t Gread = 0, H = 0;
  因果信息身份 因果;
  因果证据提交 内容;
  稳定编码 锚点关系{};
  std::vector<稳定编码> 来源关系组;
  std::uint64_t 创建事实代次 = 0;
  std::optional<std::uint64_t> 退出事实代次;
  friend bool operator==(const 因果证据事实 &, const 因果证据事实 &) = default;
};
struct 因果发布请求 final {
  std::uint32_t 合同版本 = 1;
  std::uint64_t G0 = 0;
  L1所有者范围写入幂等身份 幂等身份{};
  因果定义 定义;
  因果读取预算 预算;
  friend bool operator==(const 因果发布请求 &, const 因果发布请求 &) = default;
};
struct 因果当前读取请求 final {
  std::uint32_t 合同版本 = 1;
  std::uint64_t G0 = 0;
  因果信息身份 身份;
  因果读取预算 预算;
};
struct 因果历史读取请求 final {
  std::uint32_t 合同版本 = 1;
  std::uint64_t Gread = 0, H = 0;
  因果信息身份 身份;
  因果读取预算 预算;
};
struct 因果精确定义查询请求 final {
  std::uint32_t 合同版本 = 1;
  std::uint64_t Gread = 0, H = 0;
  因果定义 定义;
  因果读取预算 预算;
};
enum class 因果特征召回方向 : std::uint8_t { 条件 = 1, 结果 = 2 };
struct 因果按特征查询请求 final {
  std::uint32_t 合同版本 = 1;
  std::uint64_t Gread = 0, H = 0;
  因果特征概念引用 特征概念;
  因果特征召回方向 方向 = 因果特征召回方向::结果;
  因果读取预算 预算;
};
struct 因果证据关联请求 final {
  std::uint32_t 合同版本 = 1;
  std::uint64_t G0 = 0;
  L1所有者范围写入幂等身份 幂等身份{};
  因果信息身份 因果;
  因果证据提交 证据;
  因果读取预算 预算;
  friend bool operator==(const 因果证据关联请求 &,
                         const 因果证据关联请求 &) = default;
};
struct 因果证据读取请求 final {
  std::uint32_t 合同版本 = 1;
  std::uint64_t Gread = 0, H = 0;
  因果信息身份 因果;
  std::optional<动态信息身份> 发生锚点;
  因果读取预算 预算;
};
struct 因果证据退出请求 final {
  std::uint32_t 合同版本 = 1;
  std::uint64_t G0 = 0;
  L1所有者范围写入幂等身份 幂等身份{};
  因果证据定位 定位;
  因果读取预算 预算;
  friend bool operator==(const 因果证据退出请求 &,
                         const 因果证据退出请求 &) = default;
};
struct 因果退出请求 final {
  std::uint32_t 合同版本 = 1;
  std::uint64_t G0 = 0;
  L1所有者范围写入幂等身份 幂等身份{};
  因果信息身份 身份;
  因果读取预算 预算;
  friend bool operator==(const 因果退出请求 &, const 因果退出请求 &) = default;
};
struct 因果当前身份请求 final {
  std::uint32_t 合同版本 = 1;
  std::uint64_t G0 = 0;
  因果信息身份 身份;
};
struct 因果历史身份请求 final {
  std::uint32_t 合同版本 = 1;
  std::uint64_t Gread = 0, H = 0;
  因果信息身份 身份;
};
struct 因果单项结果 final {
  因果操作结果 操作;
  std::optional<因果内容事实> 内容;
  bool 成功() const noexcept;
};
struct 因果组结果 final {
  因果操作结果 操作;
  std::vector<因果内容事实> 因果组;
  bool 成功() const noexcept;
};
struct 因果证据结果 final {
  因果操作结果 操作;
  std::vector<因果证据事实> 证据组;
  bool 成功() const noexcept;
};
struct 因果身份历史事实 final {
  std::uint64_t Gread = 0, H = 0;
  因果信息身份 身份;
  稳定编码 族锚点{}, 族归属关系类型{}, 族归属关系{};
  std::uint64_t 创建事实代次 = 0;
  std::optional<std::uint64_t> 退出事实代次;
  friend bool operator==(const 因果身份历史事实 &,
                         const 因果身份历史事实 &) = default;
};
struct 因果身份历史结果 final {
  因果操作结果 操作;
  std::optional<因果身份历史事实> 见证;
  bool 成功() const noexcept;
};

enum class 因果概念类别 : std::uint8_t {
  存在 = 1,
  特征域 = 2,
  二次关系 = 3,
  动作 = 4,
  因果链 = 5
};
struct 因果概念核验项 final {
  std::uint32_t 引用序号 = 0;
  稳定编码 概念{};
  因果概念类别 类别 = 因果概念类别::存在;
  稳定编码 本体根{}, 根归属关系{};
  std::optional<特征类型身份> 正式特征类型;
  std::uint64_t 创建事实代次 = 0;
  std::optional<std::uint64_t> 退出事实代次;
  friend bool operator==(const 因果概念核验项 &,
                         const 因果概念核验项 &) = default;
};
struct 因果概念核验请求 final {
  std::uint32_t 合同版本 = 1;
  std::uint64_t Gread = 0, H = 0;
  因果定义 定义;
  因果读取预算 预算;
};
struct 因果概念核验结果 final {
  因果操作结果 操作;
  std::optional<因果定义> 已核验定义;
  std::vector<因果概念核验项> 概念组;
  std::uint64_t 已读取关系数 = 0, 已读取值元素数 = 0, 已读取材料数 = 0;
  bool 成功() const noexcept;
};
class 因果概念核验提供者 {
public:
  virtual ~因果概念核验提供者() = default;
  virtual bool 绑定于(const L1事实基座服务 &) const noexcept = 0;
  virtual 因果概念核验结果 核验因果定义概念(const 因果概念核验请求 &) const = 0;
};
class 因果结构只读提供者 {
public:
  virtual ~因果结构只读提供者() = default;
  virtual bool 绑定于(const L1事实基座服务 &) const noexcept = 0;
  virtual 因果单项结果 读取当前因果(const 因果当前读取请求 &) const = 0;
  virtual 因果单项结果 读取因果历史(const 因果历史读取请求 &) const = 0;
  virtual 因果组结果 查询精确定义(const 因果精确定义查询请求 &) const = 0;
  virtual 因果组结果 按特征概念查询因果(const 因果按特征查询请求 &) const = 0;
  virtual 因果证据结果 读取因果证据(const 因果证据读取请求 &) const = 0;
  virtual 因果操作结果 确认当前因果结构身份(const 因果当前身份请求 &) const = 0;
  virtual 因果身份历史结果 读取因果身份历史(const 因果历史身份请求 &) const = 0;
};

struct 因果结构交付 final {
  稳定编码 格式锚点{}, 字段登记关系类型{};
  稳定编码 因果族锚点{}, 族归属关系类型{}, 定义属性类型{};
  稳定编码 参与者EC关系类型{}, 条件FC关系类型{}, 结果FC关系类型{};
  稳定编码 参数FC关系类型{}, 约束RC关系类型{}, 动作DC关系类型{};
  稳定编码 原因果关系类型{}, 来源链关系类型{};
  稳定编码 证据索引属性类型{}, 证据锚点关系类型{};
  稳定编码 证据存在关系类型{}, 证据绑定关系类型{};
  friend bool operator==(const 因果结构交付 &, const 因果结构交付 &) = default;
};

} // namespace 海中鱼巣

namespace 因果合同细节 {
using namespace 海中鱼巣;
inline bool 读取头(const 因果操作结果 &o) noexcept {
  return o.状态 == 因果数据状态::已读取 && o.合同版本 == 1 && o.Gread && o.H &&
         o.H <= o.Gread && !o.发布代次 && o.阶段 == 因果发布阶段::无写入;
}
inline bool 内容完整(const 因果内容事实 &v) noexcept {
  try {
    if (!v.Gread || !v.H || v.H > v.Gread || !有效(v.信息.身份) ||
        !v.创建事实代次 || v.创建事实代次 > v.H ||
        (v.退出事实代次 && v.H >= *v.退出事实代次) || !有效(v.族归属关系) ||
        !有效(v.定义值))
      return false;
    std::vector<稳定编码> ids{v.信息.身份.编码, v.族归属关系, v.定义值};
    ids.insert(ids.end(), v.定义引用关系组.begin(), v.定义引用关系组.end());
    for (std::size_t i = 0; i < ids.size(); ++i) {
      if (!有效(ids[i]))
        return false;
      for (std::size_t j = 0; j < i; ++j)
        if (ids[i] == ids[j])
          return false;
    }
    return true;
  } catch (...) {
    return false;
  }
}
inline bool 证据完整(const 因果证据事实 &v) noexcept {
  try {
    if (!v.Gread || !v.H || v.H > v.Gread || !有效(v.因果) ||
        !有效(v.内容.发生锚点) || !v.内容.证据截止 || !有效(v.锚点关系) ||
        !v.创建事实代次 || v.创建事实代次 > v.H ||
        (v.退出事实代次 && v.H >= *v.退出事实代次))
      return false;
    std::vector<稳定编码> ids{v.锚点关系};
    ids.insert(ids.end(), v.来源关系组.begin(), v.来源关系组.end());
    std::sort(ids.begin(), ids.end());
    return std::adjacent_find(ids.begin(), ids.end()) == ids.end() &&
           std::all_of(ids.begin(), ids.end(), [](auto x) { return 有效(x); });
  } catch (...) {
    return false;
  }
}
} // namespace 因果合同细节

export namespace 海中鱼巣 {
inline bool 因果单项结果::成功() const noexcept {
  const bool repeated = 操作.状态 == 因果数据状态::精确重复;
  const bool create =
      操作.状态 == 因果数据状态::已创建 || 操作.状态 == 因果数据状态::已复用 ||
      (repeated && 内容 && 操作.发布代次 && 操作.H == *操作.发布代次);
  const bool read = 操作.状态 == 因果数据状态::已读取;
  const bool exit = 操作.状态 == 因果数据状态::已退出 ||
                    (repeated && 内容 && 操作.发布代次 && *操作.发布代次 > 1 &&
                     操作.H == *操作.发布代次 - 1);
  if (!内容 || !因果合同细节::内容完整(*内容) || 内容->Gread != 操作.Gread ||
      内容->H != 操作.H)
    return false;
  if (操作.状态 == 因果数据状态::已复用)
    return 操作.合同版本 == 1 && !操作.发布代次 &&
           操作.阶段 == 因果发布阶段::无写入 && 操作.Gread == 操作.H;
  if (create)
    return 操作.合同版本 == 1 && 操作.发布代次 && *操作.发布代次 &&
           操作.阶段 == 因果发布阶段::已读回 && 操作.Gread >= *操作.发布代次 &&
           操作.H == *操作.发布代次 && 内容->创建事实代次 == *操作.发布代次 &&
           !内容->退出事实代次;
  if (read)
    return 因果合同细节::读取头(操作);
  if (exit)
    return 操作.合同版本 == 1 && 操作.发布代次 && *操作.发布代次 > 1 &&
           操作.阶段 == 因果发布阶段::已读回 && 操作.Gread >= *操作.发布代次 &&
           操作.H == *操作.发布代次 - 1 && 内容->退出事实代次 &&
           *内容->退出事实代次 == *操作.发布代次;
  return false;
}
inline bool 因果组结果::成功() const noexcept {
  if (!因果合同细节::读取头(操作))
    return false;
  稳定编码 previous{};
  for (const auto &v : 因果组) {
    if (!因果合同细节::内容完整(v) || v.Gread != 操作.Gread || v.H != 操作.H ||
        (有效(previous) && !(previous < v.信息.身份.编码)))
      return false;
    previous = v.信息.身份.编码;
  }
  return true;
}
inline bool 因果证据结果::成功() const noexcept {
  const bool read = 操作.状态 == 因果数据状态::已读取;
  const bool reuse = 操作.状态 == 因果数据状态::已复用;
  const bool write = 操作.状态 == 因果数据状态::已关联 ||
                     操作.状态 == 因果数据状态::已退出 ||
                     操作.状态 == 因果数据状态::精确重复;
  if (read && !因果合同细节::读取头(操作))
    return false;
  if (reuse && (操作.发布代次 || 操作.阶段 != 因果发布阶段::无写入 ||
                操作.Gread != 操作.H))
    return false;
  if (write && (!操作.发布代次 || 操作.阶段 != 因果发布阶段::已读回 ||
                操作.Gread < *操作.发布代次))
    return false;
  if (!read && !reuse && !write)
    return false;
  if ((reuse || write) && 证据组.size() != 1)
    return false;
  动态信息身份 previous{};
  for (const auto &v : 证据组) {
    if (!因果合同细节::证据完整(v) || v.Gread != 操作.Gread ||
        v.H != 操作.H ||
        (有效(previous) && !(previous.编码 < v.内容.发生锚点.编码)))
      return false;
    previous = v.内容.发生锚点;
  }
  if (reuse && 证据组.front().退出事实代次)
    return false;
  if (操作.状态 == 因果数据状态::已关联)
    return 操作.H == *操作.发布代次 &&
           证据组.front().创建事实代次 == *操作.发布代次 &&
           !证据组.front().退出事实代次;
  if (操作.状态 == 因果数据状态::已退出)
    return *操作.发布代次 > 1 && 操作.H == *操作.发布代次 - 1 &&
           证据组.front().退出事实代次 == 操作.发布代次;
  if (操作.状态 == 因果数据状态::精确重复)
    return 证据组.front().退出事实代次
               ? (*操作.发布代次 > 1 && 操作.H == *操作.发布代次 - 1 &&
                  证据组.front().退出事实代次 == 操作.发布代次)
               : (操作.H == *操作.发布代次 &&
                  证据组.front().创建事实代次 == *操作.发布代次);
  return true;
}
inline bool 因果身份历史结果::成功() const noexcept {
  if (!因果合同细节::读取头(操作) || !见证 || 见证->Gread != 操作.Gread ||
      见证->H != 操作.H || !有效(见证->身份) || !有效(见证->族锚点) ||
      !有效(见证->族归属关系类型) || !有效(见证->族归属关系) ||
      !见证->创建事实代次 || 见证->创建事实代次 > 见证->H ||
      (见证->退出事实代次 && 见证->H >= *见证->退出事实代次))
    return false;
  const 稳定编码 ids[]{见证->身份.编码, 见证->族锚点, 见证->族归属关系类型,
                       见证->族归属关系};
  for (std::size_t i = 0; i < 4; ++i)
    for (std::size_t j = 0; j < i; ++j)
      if (ids[i] == ids[j])
        return false;
  return true;
}
inline bool 因果概念核验结果::成功() const noexcept {
  if (!因果合同细节::读取头(操作) || !已核验定义 || 概念组.empty())
    return false;
  std::uint32_t expected = 1;
  for (const auto &v : 概念组) {
    if (v.引用序号 != expected++ || !有效(v.概念) || !有效(v.本体根) ||
        !有效(v.根归属关系) || !v.创建事实代次 || v.创建事实代次 > 操作.H ||
        (v.退出事实代次 && 操作.H >= *v.退出事实代次))
      return false;
    if (v.类别 == 因果概念类别::特征域) {
      if (!v.正式特征类型 || !有效(*v.正式特征类型))
        return false;
    } else if (v.正式特征类型)
      return false;
  }
  return true;
}
} // namespace 海中鱼巣
