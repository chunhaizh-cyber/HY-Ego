module;

#include <cstdint>
#include <optional>
#include <vector>

export module 海中鱼巣.领域.合同.世界树根;

export import 海中鱼巣.领域.合同.场景角色组织;

export namespace 海中鱼巣 {

inline constexpr std::uint32_t 世界树根合同版本 = 1;

enum class 世界树根状态 : std::uint8_t {
  已建立 = 1,
  精确重复 = 2,
  入口拒绝 = 3,
  已有不同根 = 4,
  事实代次漂移 = 5,
  幂等冲突 = 6,
  引用冲突 = 7,
  数量预算不足 = 8,
  历史材料已清理 = 9,
  资源失败 = 10,
  内部不一致 = 11,
  已可能发布 = 12
};

struct 世界树根初始化请求 final {
  std::uint32_t 版本 = 世界树根合同版本;
  std::uint64_t G0 = 0;
  L1所有者范围写入幂等身份 组合幂等身份{}, 存在幂等身份{}, 场景幂等身份{};
  std::uint64_t 最大关系数量 = 0, 最大场景数量 = 0;
  friend bool operator==(const 世界树根初始化请求 &,
                         const 世界树根初始化请求 &) = default;
};

struct 世界树根存在片段 final {
  L1有限N分区原子参与者写集_v3 写集;
};

struct 世界树根场景片段 final {
  L1有限N分区原子参与者写集_v3 写集;
};

struct 世界树根存在来源 final {
  std::uint64_t Gread = 0, H = 0;
  稳定编码 根{};
  存在身份来源历史见证 来源;
};

struct 世界树根组读取请求 final {
  std::uint32_t 版本 = 世界树根合同版本;
  std::uint64_t G0 = 0, 最大关系数量 = 0;
};

struct 世界树根组读取结果 final {
  世界树根状态 状态 = 世界树根状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::vector<稳定编码> 根组;
  bool 成功(const 世界树根组读取请求 &r) const noexcept {
    return r.版本 == 世界树根合同版本 && r.G0 &&
           状态 == 世界树根状态::精确重复 && Gread == r.G0 &&
           根组.size() <= r.最大关系数量;
  }
};

struct 世界树根事实 final {
  稳定编码 根{};
  std::uint64_t 首次H = 0, Gread = 0;
  场景树节点当前事实 根场景;
};

struct 世界树根初始化结果 final {
  世界树根状态 状态 = 世界树根状态::入口拒绝;
  std::uint32_t 版本 = 世界树根合同版本;
  std::uint64_t Gread = 0;
  std::optional<std::uint64_t> 首次发布H;
  std::optional<世界树根初始化请求> 原请求;
  std::optional<世界树根事实> 事实;
  bool 成功(const 世界树根初始化请求 &r) const noexcept;
};

class 世界树根存在参与者 {
public:
  virtual ~世界树根存在参与者() = default;
  virtual const L1事实基座服务 &世界树根底座() const noexcept = 0;
  virtual L1结构所有者身份 世界树根所有者() const noexcept = 0;
  virtual bool 世界树根幂等键可用(L1所有者范围写入幂等身份) const noexcept = 0;
  virtual bool 世界树根结构已就绪() const noexcept = 0;
  virtual 世界树根存在片段
  准备世界树根存在片段(const 世界树根初始化请求 &) const = 0;
  virtual L1所有者范围首次写入读取结果
  读取世界树根存在首次材料(L1所有者范围写入幂等身份) const = 0;
  virtual std::optional<世界树根存在来源>
  读取世界树根存在来源(std::uint64_t, std::uint64_t, 稳定编码) const = 0;
};

class 世界树根场景参与者 {
public:
  virtual ~世界树根场景参与者() = default;
  virtual const L1事实基座服务 &世界树根底座() const noexcept = 0;
  virtual L1结构所有者身份 世界树根所有者() const noexcept = 0;
  virtual bool 世界树根幂等键可用(L1所有者范围写入幂等身份) const noexcept = 0;
  virtual bool 世界树根结构已就绪() const noexcept = 0;
  virtual 世界树根场景片段
  准备世界树根场景片段(const 世界树根初始化请求 &) const = 0;
  virtual L1所有者范围首次写入读取结果
  读取世界树根场景首次材料(L1所有者范围写入幂等身份) const = 0;
  virtual 世界树根组读取结果
  读取当前世界树根组(const 世界树根组读取请求 &) const = 0;
};

inline bool 世界树根初始化结果::成功(const 世界树根初始化请求 &r) const noexcept {
  if (r.版本 != 世界树根合同版本 || 版本 != 世界树根合同版本 ||
      (状态 != 世界树根状态::已建立 && 状态 != 世界树根状态::精确重复) ||
      !首次发布H || !原请求 || *原请求 != r || !事实 ||
      事实->根 != 事实->根场景.场景角色.场景 || 事实->首次H != *首次发布H ||
      事实->Gread != Gread || !有效(事实->根) || !Gread ||
      !事实->首次H || 事实->首次H > Gread || 事实->根场景.直接父 ||
      事实->根场景.树证明.种类 != 场景树证明种类::根标记 ||
      事实->根场景.树证明.场景 != 事实->根 ||
      事实->根场景.树证明.树根 != 事实->根 ||
      !事实->根场景.从上游场景到本场景路径.empty() ||
      事实->根场景.父语境投影)
    return false;
  return 事实->根场景.场景角色.四根.size() == 4;
}

} // namespace 海中鱼巣
