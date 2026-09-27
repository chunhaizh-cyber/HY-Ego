#pragma once

#include <optional>
#include <variant>

#include "合同.世界树根.h"
#include "数据服务.绑定存在.h"
#include "数据服务.定位特征.h"
#include "数据服务.特征类.h"

namespace 海中鱼巣 {

enum class 存在类数据状态 : std::uint8_t {
  已创建 = 1,
  精确重复 = 2,
  已读取 = 3,
  入口拒绝 = 7,
  未找到 = 8,
  幂等冲突 = 19,
  引用冲突 = 20,
  资源失败 = 22,
  内部不一致 = 23,
  已可能发布 = 24,
  已建立当前采用 = 25,
  已替换当前采用 = 26,
  已解除当前采用 = 27,
  未实现 = 30
};

struct 存在当前采用结构交付 final { 稳定编码 当前采用关系类型{}; };

struct 存在当前采用事实 final {
  稳定编码 关系{}, 存在{};
  特征类型身份 特征类型;
  特征信息身份 特征;
  friend bool operator==(const 存在当前采用事实 &,
                         const 存在当前采用事实 &) = default;
};

struct 存在当前采用读取请求 final {
  稳定编码 存在{};
  特征类型身份 特征类型;
};

struct 存在当前采用建立 final { 特征信息身份 特征; };
struct 存在当前采用替换 final {
  存在当前采用事实 预期;
  特征信息身份 新特征;
};
struct 存在当前采用解除 final { 存在当前采用事实 预期; };
using 存在当前采用操作 =
    std::variant<存在当前采用建立, 存在当前采用替换, 存在当前采用解除>;

struct 存在当前采用写请求 final {
  L1所有者范围写入幂等身份 幂等身份;
  稳定编码 存在{};
  特征类型身份 特征类型;
  存在当前采用操作 操作;
};

struct 存在当前采用结果 final {
  存在类数据状态 状态 = 存在类数据状态::入口拒绝;
  std::optional<存在当前采用事实> 采用;
  std::optional<存在当前采用写请求> 原请求;
  bool 成功() const noexcept;
};

struct 实例特征结构交付 final {
  稳定编码 存在到容器{}, 容器到特征{}, 容器到R集合{}, R集合到版本{},
      版本到R项{}, R项到特征{}, R项材料属性类型{};
};

enum class 实例特征结构状态 : std::uint8_t {
  已登记 = 1,
  精确重复 = 2,
  入口拒绝 = 3,
  幂等冲突 = 5,
  结构冲突 = 6,
  已可能发布 = 7,
  资源失败 = 8,
  内部不一致 = 9,
  未实现 = 10
};

struct 实例特征结构登记请求 final {
  L1所有者范围写入幂等身份 幂等身份{};
};

struct 实例特征结构登记结果 final {
  实例特征结构状态 状态 = 实例特征结构状态::入口拒绝;
  std::optional<实例特征结构交付> 交付;
  bool 成功(const 实例特征结构登记请求 &) const noexcept;
};

class 存在类数据服务 final : public 存在结构身份只读提供者,
                             public 存在组成结构只读提供者,
                             public 原子I64特征holder参与者,
                             public 绑定存在内容参与者,
                             public 世界树根存在参与者 {
public:
  存在类数据服务() = delete;
  存在类数据服务(const 存在类数据服务 &) = delete;
  存在类数据服务 &operator=(const 存在类数据服务 &) = delete;
  存在类数据服务(存在类数据服务 &&) = delete;
  存在类数据服务 &operator=(存在类数据服务 &&) = delete;

  存在类数据服务(const L1事实基座服务 &, const 特征类数据服务 &,
                 L1所有者范围写端口 &&, 稳定编码 子存在关系类型,
                 稳定编码 特征关系类型,
                 const 存在当前采用结构交付 &,
                 const 实例特征结构交付 &);
  存在类数据服务(const L1事实基座服务 &, const 特征类数据服务 &,
                 L1所有者范围写端口 &&, 稳定编码 子存在关系类型,
                 稳定编码 特征关系类型,
                 const 存在当前采用结构交付 &,
                 const 实例特征结构交付 &,
                 const 存在单例角色结构交付 &);

  bool 绑定于(const L1事实基座服务 &) const noexcept override;

  const L1事实基座服务 &世界树根底座() const noexcept override;
  L1结构所有者身份 世界树根所有者() const noexcept override;
  bool 世界树根幂等键可用(
      L1所有者范围写入幂等身份) const noexcept override;
  bool 世界树根结构已就绪() const noexcept override;
  世界树根存在片段
  准备世界树根存在片段(const 世界树根初始化请求 &) const override;
  L1所有者范围首次写入读取结果
  读取世界树根存在首次材料(
      L1所有者范围写入幂等身份) const override;
  std::optional<世界树根存在来源>
  读取世界树根存在来源(稳定编码) const override;

  存在组成读取结果
  读取当前组成父(const 存在组成父读取请求 &) const override;
  存在组成读取结果
  读取当前组成子组(const 存在组成子组读取请求 &) const override;
  存在当前身份确认结果
  确认当前存在结构身份(稳定编码) const override;
  存在身份来源当前见证读取结果
  读取当前存在身份来源见证(稳定编码) const override;
  存在已知准确特征读取结果
  确认当前已知准确特征(
      const 存在已知准确特征当前请求 &) const override;

  static 存在单例角色结构登记结果
  登记单例角色结构(const L1事实基座服务 &,
                   L1所有者范围写端口 &,
                   const 存在单例角色结构登记请求 &) noexcept;
  static 实例特征结构登记结果
  登记实例特征结构(const L1事实基座服务 &,
                   L1所有者范围写端口 &,
                   const 实例特征结构登记请求 &) noexcept;

  存在单例角色读取结果
  读取单例角色(const 存在单例角色读取请求 &) const noexcept;
  存在当前采用结果
  读取当前采用(const 存在当前采用读取请求 &) const noexcept;
  存在当前采用结果
  变更当前采用(const 存在当前采用写请求 &) noexcept;

  已发布概念绑定创建结果
  添加存在节点(const 已发布概念绑定创建请求 &,
               绑定存在数据服务 &,
               已发布概念引用参与者 &) noexcept;

private:
  friend class 世界树根数据服务;
  L1所有者范围写端口 &世界树根协调端口() noexcept;

  const L1事实基座服务 &绑定存在底座() const noexcept override;
  L1所有者范围写端口 &绑定存在端口() noexcept override;
  bool 绑定存在结构已就绪() const noexcept override;
  bool 绑定存在幂等键可用(
      L1所有者范围写入幂等身份) const noexcept override;
  const 存在组成结构只读提供者 &
  绑定存在组成提供者() const noexcept override;
  绑定存在参与者结果<L1有限N分区原子参与者写集>
  准备存在出生片段(
      const 绑定存在创建请求 &,
      const std::optional<存在单例角色身份> &) const override;
  存在单例角色读取结果
  读取绑定单例角色(
      const 存在单例角色读取请求 &) const noexcept override;
  绑定存在参与者结果<L1所有者范围首次写入读取结果>
  读取存在出生首次材料(
      L1所有者范围写入幂等身份) const override;
  绑定存在参与者结果<存在绑定出生见证>
  读取存在绑定出生(稳定编码,
                   const 存在初始绑定 &) const override;

  const L1事实基座服务 &原子I64底座() const noexcept override;
  L1所有者范围写端口 &原子I64端口() noexcept override;
  bool 原子I64结构已就绪() const noexcept override;
  原子I64特征参与结果<L1有限N分区原子参与者写集>
  准备原子I64出生片段(
      const 原子I64特征出生请求 &) const override;
  原子I64特征窄读取结果<原子I64特征holder事实>
  读取原子I64holder(
      const 原子I64特征holder读取请求 &) const override;

  void 初始化存在族来源();
  void 接受存在族首次材料(
      const L1所有者范围首次写入读取结果 &);
  static L1所有者范围写集请求 形成存在族初始化写集();
  static bool 关系类型组有效(稳定编码, 稳定编码,
                            const 存在当前采用结构交付 &) noexcept;
  static bool 实例特征结构有效(
      const 实例特征结构交付 &) noexcept;

  inline static constexpr L1所有者范围写入幂等身份
      存在族来源初始化幂等身份{0x455849535446414DULL};

  const L1事实基座服务 &第一层服务_;
  const 特征类数据服务 &特征服务_;
  L1所有者范围写端口 写入端口_;
  L1结构所有者身份 所有者_;
  稳定编码 子存在关系类型_{}, 特征关系类型_{}, 当前采用关系类型_{};
  实例特征结构交付 实例特征结构_;
  std::optional<存在单例角色结构交付> 角色结构_;
  稳定编码 存在族锚点_{}, 存在族归属关系类型_{};
};

} // namespace 海中鱼巣
