module;

#include <cstdint>
#include <optional>
#include <span>
#include <variant>
#include <vector>

export module 海中鱼巣.领域.合同.相关概念添加参与;

export import 海中鱼巣.核心.服务.L1事实基座;

export namespace 海中鱼巣 {

struct 概念树概念身份 final {
  稳定编码 值{};
  概念树概念身份() = default;
  explicit 概念树概念身份(稳定编码 编码) : 值(编码) {}
  friend bool operator==(const 概念树概念身份 &,
                         const 概念树概念身份 &) = default;
};

struct 概念树存在引用 final {
  稳定编码 值{};
  概念树存在引用() = default;
  explicit 概念树存在引用(稳定编码 编码) : 值(编码) {}
  friend bool operator==(const 概念树存在引用 &,
                         const 概念树存在引用 &) = default;
};

struct 概念树特征类型引用 final {
  稳定编码 值{};
  概念树特征类型引用() = default;
  explicit 概念树特征类型引用(稳定编码 编码) : 值(编码) {}
  friend bool operator==(const 概念树特征类型引用 &,
                         const 概念树特征类型引用 &) = default;
};

struct 概念树特征引用 final {
  稳定编码 值{};
  概念树特征引用() = default;
  explicit 概念树特征引用(稳定编码 编码) : 值(编码) {}
  friend bool operator==(const 概念树特征引用 &,
                         const 概念树特征引用 &) = default;
};

struct 概念树场景引用 final {
  稳定编码 值{};
  概念树场景引用() = default;
  explicit 概念树场景引用(稳定编码 编码) : 值(编码) {}
  friend bool operator==(const 概念树场景引用 &,
                         const 概念树场景引用 &) = default;
};

using 概念树世界引用 = std::variant<概念树存在引用, 概念树特征引用>;
using 概念树形成世界引用 =
    std::variant<概念树存在引用, 概念树特征引用,
                 概念树特征类型引用, 概念树场景引用>;
using 概念树精确值 =
    std::variant<std::int64_t, std::vector<std::int64_t>,
                 std::vector<std::uint64_t>>;

struct 概念树I64区间 final {
  std::int64_t 下界 = 0;
  std::int64_t 上界 = 0;
  friend bool operator==(const 概念树I64区间 &,
                         const 概念树I64区间 &) = default;
};

using 概念树特征值域 = std::variant<概念树精确值, 概念树I64区间>;

struct 概念树特征定义 final {
  概念树存在引用 形成宿主;
  概念树特征类型引用 特征类型;
  概念树特征值域 值域;
  friend bool operator==(const 概念树特征定义 &,
                         const 概念树特征定义 &) = default;
};

struct 概念树存在定义 final {
  std::vector<概念树概念身份> 特征模板组;
  friend bool operator==(const 概念树存在定义 &,
                         const 概念树存在定义 &) = default;
};

using 概念树定义 = std::variant<概念树特征定义, 概念树存在定义>;

struct 概念树来源项 final {
  概念树世界引用 世界事实;
  std::uint64_t 证据截止 = 0;
  friend bool operator==(const 概念树来源项 &,
                         const 概念树来源项 &) = default;
};

struct 概念树预算 final {
  std::uint64_t 最大概念数 = 0;
  std::uint64_t 最大关系数 = 0;
  std::uint64_t 最大来源数 = 0;
  std::uint64_t 最大支持数 = 0;
  std::uint64_t 最大世界成员数 = 0;
  std::uint64_t 最大特征属性数 = 0;
  std::uint64_t 最大动态槽数 = 0;
  std::uint64_t 最大动态模板数 = 0;
  friend bool operator==(const 概念树预算 &,
                         const 概念树预算 &) = default;
};

struct 概念树生命周期 final {
  std::uint64_t 创建事实代次 = 0;
  std::optional<std::uint64_t> 退出事实代次;
  friend bool operator==(const 概念树生命周期 &,
                         const 概念树生命周期 &) = default;
};

struct 概念树直接上位事实 final {
  稳定编码 关系{};
  概念树概念身份 上位;
  概念树概念身份 下位;
  概念树生命周期 生命周期;
  friend bool operator==(const 概念树直接上位事实 &,
                         const 概念树直接上位事实 &) = default;
};

struct 概念树形成引用事实 final {
  稳定编码 关系{};
  稳定编码 记录{};
  概念树概念身份 所属概念;
  概念树形成世界引用 世界引用;
  概念树生命周期 关系生命周期;
  friend bool operator==(const 概念树形成引用事实 &,
                         const 概念树形成引用事实 &) = default;
};

enum class 相关概念类别 : std::uint8_t { 存在 = 1, 特征 = 2 };
enum class 相关概念方案种类 : std::uint8_t {
  精确复用 = 1,
  创建并引用 = 2
};
enum class 相关概念参与状态 : std::uint8_t {
  已准备 = 1,
  已读取 = 2,
  精确重复 = 3,
  入口拒绝 = 4,
  概念未找到 = 5,
  概念已退出 = 6,
  类别冲突 = 7,
  签名冲突 = 8,
  上位成环 = 9,
  挂靠无效 = 10,
  事实代次漂移 = 11,
  幂等冲突 = 12,
  数量预算不足 = 13,
  历史材料不可用 = 14,
  资源失败 = 15,
  内部不一致 = 16,
  已可能发布 = 17,
  旧格式不支持 = 18
};

using 相关概念读取预算 = 概念树预算;

struct 相关概念世界挂靠 final {
  概念树形成世界引用 世界事实;
  std::uint64_t 证据截止 = 0;
};
struct 相关概念精确复用方案 final {
  概念树概念身份 概念;
  相关概念类别 类别 = 相关概念类别::存在;
  概念树定义 预期定义;
};
struct 相关概念创建方案 final {
  相关概念类别 类别 = 相关概念类别::存在;
  概念树定义 定义;
  std::vector<概念树概念身份> 直接上位;
};
using 相关概念方案 =
    std::variant<相关概念精确复用方案, 相关概念创建方案>;
struct 相关概念参与请求 final {
  std::uint32_t 版本 = 1;
  std::uint64_t G0 = 0;
  L1所有者范围写入幂等身份 幂等身份;
  相关概念方案 方案;
  相关概念世界挂靠 挂靠;
  相关概念读取预算 预算;
};
struct 相关概念参与片段 final {
  相关概念参与状态 状态 = 相关概念参与状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<L1有限N分区原子参与者写集_v3> 写集;
};
struct 相关概念完整事实 final {
  概念树概念身份 概念;
  相关概念类别 类别 = 相关概念类别::存在;
  概念树定义 定义;
  std::vector<概念树直接上位事实> 直接上位;
  概念树形成引用事实 挂接;
  概念树生命周期 生命周期;
};
struct 相关概念参与读回 final {
  相关概念参与状态 状态 = 相关概念参与状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::uint64_t H = 0;
  std::optional<相关概念完整事实> 概念;
};
struct 相关概念组合提交请求 final {
  std::uint32_t 版本 = 1;
  std::uint64_t Gread = 0;
  L1所有者范围写入幂等身份 组合幂等身份;
  相关概念参与请求 概念请求;
  std::vector<L1有限N分区原子参与者写集_v3> 前序参与者写集组;
};
struct 相关概念组合提交结果 final {
  std::uint32_t 版本 = 1;
  相关概念参与状态 概念状态 = 相关概念参与状态::入口拒绝;
  std::uint64_t Gread = 0;
  bool 已进入L1 = false;
  L1有限N分区原子事务结果_v3 事务结果;
};

class 相关概念添加参与者 {
public:
  virtual bool 绑定于(const L1事实基座服务 &) const noexcept = 0;
  virtual 相关概念参与片段
  准备相关概念片段(const 相关概念参与请求 &,
                     std::uint64_t Gread,
                     L1有限N分区原子参与者身份_v3) const noexcept = 0;
  virtual 相关概念组合提交结果
  提交相关概念组合事务(const 相关概念组合提交请求 &,
      std::span<L1所有者范围写端口 *const> 前序参与者端口组) noexcept = 0;
  virtual 相关概念参与读回
  读取相关概念结果(const 相关概念参与请求 &, std::uint64_t Gread,
                     std::uint64_t H) const noexcept = 0;
};

struct 相关概念结构类型 final {
  稳定编码 类型登记, 概念族成员, 概念类别, 定义成员, 定义种类;
  稳定编码 定义宿主, 定义特征类型, 定义模板;
  稳定编码 精确I64, 精确I64组, 精确U64组, 区间下界, 区间上界;
  稳定编码 直接上位, 形成成员, 形成存在, 形成特征, 形成特征类型,
      形成场景;
  稳定编码 证据截止, 生命周期;
};
struct 相关概念结构交付 final {
  std::uint32_t 版本 = 1, 格式 = 1;
  稳定编码 格式锚点, 概念族锚点;
  相关概念结构类型 类型;
};

} // namespace 海中鱼巣
