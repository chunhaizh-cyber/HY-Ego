#pragma once

#include <optional>
#include <vector>

#include "合同.场景角色组织.h"
#include "合同.世界树根.h"

namespace 海中鱼巣 {

class 状态类数据服务;

class 场景类数据服务 final : public 状态使用绑定只读提供者,
                             public 场景动态组织只读提供者,
                             public 场景直接包含只读提供者,
                             public 世界树根场景参与者 {
public:
  场景类数据服务() = delete;
  场景类数据服务(const 场景类数据服务 &) = delete;
  场景类数据服务 &operator=(const 场景类数据服务 &) = delete;
  场景类数据服务(场景类数据服务 &&) = delete;
  场景类数据服务 &operator=(场景类数据服务 &&) = delete;

  场景类数据服务(const L1事实基座服务 &, L1所有者范围写端口 &&,
                 const 场景角色结构交付 &,
                 const 存在结构身份只读提供者 &,
                 const 状态类数据服务 &,
                 const 场景特征组织扩展结构交付 &,
                 const 场景直接包含扩展结构交付 &);

  bool 绑定于(const L1事实基座服务 &) const noexcept override;

  const L1事实基座服务 &世界树根底座() const noexcept override;
  L1结构所有者身份 世界树根所有者() const noexcept override;
  bool 世界树根幂等键可用(
      L1所有者范围写入幂等身份) const noexcept override;
  bool 世界树根结构已就绪() const noexcept override;
  世界树根场景片段
  准备世界树根场景片段(const 世界树根初始化请求 &) const override;
  L1所有者范围首次写入读取结果
  读取世界树根场景首次材料(
      L1所有者范围写入幂等身份) const override;
  世界树根组读取结果
  读取当前世界树根组(const 世界树根组读取请求 &) const override;

  static 场景结构登记结果
  登记结构(const L1事实基座服务 &, L1所有者范围写端口 &,
           const 场景结构登记请求 &);
  static 场景特征组织扩展登记结果
  登记特征组织扩展(const L1事实基座服务 &,
                   L1所有者范围写端口 &,
                   const 场景特征组织扩展登记请求 &);
  static 场景直接包含扩展登记结果
  登记直接包含扩展(const L1事实基座服务 &,
                   L1所有者范围写端口 &,
                   const 场景直接包含扩展登记请求 &);

  场景当前身份结果
  确认当前场景角色(const 场景当前身份请求 &) const override;
  场景角色当前读取结果
  读取当前场景角色(const 场景角色当前读取请求 &) const override;
  场景角色写结果
  启用场景角色(const 场景角色启用请求 &,
               const 直接归属联合只读提供者 &);
  场景角色写结果 退出场景角色(const 场景角色退出请求 &);
  场景父语境读取结果
  读取当前父场景语境(const 场景父语境读取请求 &) const override;

  状态使用绑定读取结果
  读取当前状态使用绑定(
      const 状态使用绑定当前读取请求 &) const override;

  场景组织写结果 组织状态实例(const 场景状态组织请求 &);
  场景组织写结果
  组织动态实例(const 场景动态组织请求 &,
               const 动态结构只读提供者 &);
  场景组织当前读取结果
  读取当前实例组织(const 场景组织当前读取请求 &) const;
  场景动态组织当前读取结果
  读取当前动态场景组织(
      const 场景动态组织当前读取请求 &) const override;
  场景特征组织写结果
  组织特征实例(const 场景特征组织请求 &);
  场景特征组织当前读取结果
  读取当前特征组织(const 场景特征组织当前读取请求 &) const;

  场景直接包含组结果
  读取当前场景包含父组(
      const 场景直接包含反向读取请求 &) const override;
  场景直接包含组结果
  读取当前场景包含子组(
      const 场景直接包含组读取请求 &) const override;
  直接归属场景角色读取结果
  读取当前场景角色位置(
      const 直接归属场景角色读取请求 &) const override;

  场景树角色写结果
  启用并建立场景树根(const 场景树根启用请求 &,
                     const 直接归属联合只读提供者 &);
  场景树角色写结果
  启用并接纳直接子场景(const 场景直接子场景启用请求 &,
                       const 直接归属联合只读提供者 &);
  场景直接包含单项结果
  新增直接存在成员(const 场景直接包含写请求 &,
                   const 直接归属联合只读提供者 &);
  场景直接包含单项结果
  退出直接存在成员(const 场景直接包含写请求 &,
                   const 直接归属联合只读提供者 &);
  场景直接包含迁移结果
  迁移直接存在成员(const 场景直接包含迁移请求 &,
                   const 直接归属联合只读提供者 &);
  场景直接包含组结果
  读取当前直接存在成员组(const 场景直接包含组读取请求 &) const;
  场景直接包含单项结果
  读取当前直接存在成员父(const 场景直接包含反向读取请求 &) const;
  场景直接包含单项结果
  读取当前直接存在成员(const 场景直接包含当前读取请求 &) const;
  场景直接包含迁移结果
  迁移直接子场景(const 场景直接包含迁移请求 &,
                 const 直接归属联合只读提供者 &);
  场景直接包含组结果
  读取当前直接子场景组(const 场景直接包含组读取请求 &) const;
  场景直接包含单项结果
  读取当前直接子场景父(const 场景直接包含反向读取请求 &) const;
  场景直接包含单项结果
  读取当前直接子场景(const 场景直接包含当前读取请求 &) const;
  场景直接包含单项结果
  读取当前直接包含(const 场景直接包含当前读取请求 &) const;
  场景树当前结果
  读取当前场景树(const 场景树当前读取请求 &,
                 const 直接归属联合只读提供者 &) const;

  bool 使用存在提供者(
      const 存在结构身份只读提供者 &) const noexcept;

private:
  friend class 世界树根数据服务;
  L1所有者范围写端口 &世界树根协调端口() noexcept;

  static bool 布局形状有效(const 场景角色结构交付 &) noexcept;
  static bool 特征布局形状有效(
      const 场景特征组织扩展结构交付 &) noexcept;
  static bool 包含布局形状有效(
      const 场景直接包含扩展结构交付 &) noexcept;

  const L1事实基座服务 &第一层服务_;
  L1所有者范围写端口 写端口_;
  L1结构所有者身份 所有者_;
  场景角色结构交付 布局_;
  场景特征组织扩展结构交付 特征布局_;
  场景直接包含扩展结构交付 包含布局_;
  const 存在结构身份只读提供者 &存在提供者_;
};

} // namespace 海中鱼巣
