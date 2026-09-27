#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>
#include <optional>

#include "数据服务.存在类.h"

namespace 海中鱼巣 {

enum class 需求类数据状态 : std::uint8_t {
  已创建 = 1, 精确重复 = 2, 已读取 = 3, 已计算 = 4,
  入口拒绝 = 5, 未找到 = 6, 所属存在未找到 = 7,
  目标宿主未找到 = 9, 静态目标特征未找到 = 11,
  当前事实特征未找到 = 13, 目标特征不属于宿主 = 15,
  当前特征不属于宿主 = 16, 方向定义未找到 = 17,
  方向定义不匹配 = 19, 目标不是一阶I64静态特征 = 20,
  当前不是一阶I64特征 = 21, 精确目标已存在 = 22,
  幂等冲突 = 25, 引用冲突 = 26, 资源失败 = 27,
  内部不一致 = 28, 已可能发布 = 29, 方向格式不支持 = 30,
  方向来源不匹配 = 31, 方向比较失败 = 32, 未实现 = 33
};

struct 需求类结构类型 final {
  稳定编码 所属存在关系类型{};
  稳定编码 目标宿主关系类型{};
  稳定编码 静态目标特征关系类型{};
  稳定编码 方向二次特征关系类型{};
  friend bool operator==(const 需求类结构类型 &,
                         const 需求类结构类型 &) = default;
};

inline constexpr L1所有者范围写入幂等身份 需求结构登记固定幂等身份{
    0x415243484C345244ULL};

struct 本能根结构类型 final {
  稳定编码 根实际特征关系类型{};
  稳定编码 根目标合同关系类型{};
  稳定编码 根列表成员关系类型{};
  稳定编码 根目标值属性类型{};
  friend bool operator==(const 本能根结构类型 &,
                         const 本能根结构类型 &) = default;
};

struct 需求结构交付 final {
  需求类结构类型 普通结构{};
  本能根结构类型 根结构{};
  friend bool operator==(const 需求结构交付 &,
                         const 需求结构交付 &) = default;
};

struct 需求结构登记请求 final {
  friend bool operator==(const 需求结构登记请求 &,
                         const 需求结构登记请求 &) = default;
};

struct 需求结构登记结果 final {
  需求类数据状态 状态 = 需求类数据状态::入口拒绝;
  std::optional<需求结构交付> 交付;
  bool 成功(const 需求结构登记请求 &) const noexcept;
};

enum class 本能根角色 : std::uint8_t { 安全 = 1, 服务 = 2 };
enum class 本能根材料状态 : std::uint8_t {
  已形成 = 1, 已恢复 = 2, 已读取 = 3, 入口拒绝 = 4,
  实际特征未找到 = 5, 实际特征类型不匹配 = 7,
  幂等冲突 = 9, 引用冲突 = 10, 根材料未闭合 = 11,
  已可能发布 = 13, 资源失败 = 14, 内部不一致 = 15,
  未实现 = 16
};

struct 本能根材料请求 final {
  本能根角色 角色 = 本能根角色::安全;
  稳定编码 实际特征{};
  friend bool operator==(const 本能根材料请求 &,
                         const 本能根材料请求 &) = default;
};

struct 本能根材料 final {
  本能根角色 角色 = 本能根角色::安全;
  稳定编码 根目标合同{};
  稳定编码 根需求{};
  稳定编码 根列表项{};
  稳定编码 实际特征{};
  稳定编码 实际特征关系{};
  稳定编码 目标合同关系{};
  稳定编码 列表成员关系{};
  稳定编码 目标值{};
  std::int64_t 目标I64值 = 0;
  bool 完整() const noexcept;
  friend bool operator==(const 本能根材料 &,
                         const 本能根材料 &) = default;
};

struct 本能根材料结果 final {
  本能根材料状态 状态 = 本能根材料状态::入口拒绝;
  std::optional<本能根材料> 材料;
  bool 成功(const 本能根材料请求 &) const noexcept;
};

struct 需求类记录身份 final {
  稳定编码 值{};
  friend bool operator==(const 需求类记录身份 &,
                         const 需求类记录身份 &) = default;
};

struct 需求类记录 final {
  需求类记录身份 身份;
  稳定编码 所属存在{};
  稳定编码 目标宿主{};
  稳定编码 静态目标特征{};
  特征类定义身份 方向二次特征;
  稳定编码 所属存在关系{};
  稳定编码 目标宿主关系{};
  稳定编码 静态目标特征关系{};
  稳定编码 方向二次特征关系{};
  friend bool operator==(const 需求类记录 &,
                         const 需求类记录 &) = default;
};

struct 需求类新增请求 final {
  L1所有者范围写入幂等身份 幂等身份{};
  稳定编码 所属存在{};
  稳定编码 目标宿主{};
  稳定编码 静态目标特征{};
  特征类定义身份 方向二次特征;
  friend bool operator==(const 需求类新增请求 &,
                         const 需求类新增请求 &) = default;
};

struct 需求类身份查询请求 final { 需求类记录身份 需求; };

struct 需求类精确查询请求 final {
  稳定编码 所属存在{};
  稳定编码 目标宿主{};
  稳定编码 静态目标特征{};
  特征类定义身份 方向二次特征;
  friend bool operator==(const 需求类精确查询请求 &,
                         const 需求类精确查询请求 &) = default;
};

inline bool 需求类记录闭合(const 需求类记录 &record) noexcept {
  return 有效(record.身份.值) && 有效(record.所属存在)
      && 有效(record.目标宿主) && 有效(record.静态目标特征)
      && 有效(record.方向二次特征.结点) && 有效(record.所属存在关系)
      && 有效(record.目标宿主关系) && 有效(record.静态目标特征关系)
      && 有效(record.方向二次特征关系);
}

struct 需求类记录结果 final {
  需求类数据状态 状态 = 需求类数据状态::入口拒绝;
  std::optional<需求类记录> 记录;
  bool 成功() const noexcept;
  friend bool operator==(const 需求类记录结果 &,
                         const 需求类记录结果 &) = default;
};

class 需求类数据服务 final {
public:
  static 需求结构登记结果 登记需求结构(
      const L1事实基座服务 &, L1所有者范围写端口 &,
      const 需求结构登记请求 &);

  需求类数据服务() = delete;
  需求类数据服务(const 需求类数据服务 &) = delete;
  需求类数据服务 &operator=(const 需求类数据服务 &) = delete;
  需求类数据服务(需求类数据服务 &&) = delete;
  需求类数据服务 &operator=(需求类数据服务 &&) = delete;

  需求类数据服务(const L1事实基座服务 &,
                   const 特征类数据服务 &,
                   const 存在类数据服务 &,
                   L1所有者范围写端口 &&,
                   需求结构交付);

  bool 绑定于(const L1事实基座服务 &) const noexcept;
  bool 与需求服务同底座(const 需求类数据服务 &) const noexcept;
  bool 与存在服务同底座(const 存在类数据服务 &) const noexcept;
  bool 与特征服务同底座(const 特征类数据服务 &) const noexcept;

  需求类记录结果 新增需求记录(const 需求类新增请求 &);
  需求类记录结果 查询需求记录(const 需求类身份查询请求 &) const;
  需求类记录结果 按所属存在和目标精确查询(
      const 需求类精确查询请求 &) const;
  本能根材料结果 建立或读取本能根材料(const 本能根材料请求 &);
  本能根材料结果 读取本能根材料(const 本能根材料请求 &) const;

private:
  bool 结构类型有效() const noexcept;
  bool 当前结构有效() const;
  需求类记录结果 从当前投影读取记录(
      需求类记录身份,
      const L1所有者范围一致当前读取结果 &) const;

  static 需求类记录结果 记录失败(需求类数据状态) noexcept;
  static 本能根材料结果 根失败(本能根材料状态) noexcept;
  static L1所有者范围写入幂等身份 根写入身份(本能根角色) noexcept;

  const L1事实基座服务 &第一层服务_;
  const 特征类数据服务 &特征服务_;
  const 存在类数据服务 &存在服务_;
  L1所有者范围写端口 写入端口_;
  L1结构所有者身份 所有者_{};
  需求类结构类型 结构类型_{};
  本能根结构类型 根结构类型_{};
};

} // namespace 海中鱼巣
