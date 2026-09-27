#pragma once

#include <algorithm>
#include <cstdint>
#include <optional>
#include <vector>

#include "数据服务.存在类.h"

namespace 海中鱼巣 {

enum class 方法类数据状态 : std::uint8_t {
  已创建 = 1,
  精确重复 = 2,
  已读取 = 3,
  已添加条件项 = 4,
  已读取条件组 = 5,
  已添加结果方向项 = 6,
  已读取结果方向组 = 7,
  入口拒绝 = 8,
  未找到 = 9,
  来源节点未找到 = 10,
  来源节点不是普通节点 = 12,
  精确记录已存在 = 13,
  条件特征未找到 = 14,
  条件特征不可用 = 16,
  条件项已存在 = 17,
  结果方向未找到 = 18,
  结果方向不是二次特征 = 20,
  结果方向不可用 = 21,
  结果方向项已存在 = 22,
  幂等冲突 = 25,
  引用冲突 = 26,
  资源失败 = 27,
  内部不一致 = 28,
  已可能发布 = 29,
  已初始化方法虚拟存在 = 30,
  方法虚拟存在已初始化 = 31,
  方法虚拟存在未初始化 = 32,
  方法虚拟存在规格失败 = 33,
  方法虚拟存在提供者不兼容 = 34,
  已添加参数规格 = 35,
  已读取参数规格组 = 36,
  参数特征类型未找到 = 37,
  参数特征类型不是属性类型 = 39,
  参数规格已存在 = 40,
  参数顺序冲突 = 41,
  参数类别无效 = 42,
  结果方向格式不支持 = 43,
  结果方向来源不匹配 = 44,
  未实现 = 45
};

enum class 方法参数类别 : std::uint8_t { 场景 = 1, 存在 = 2, 特征 = 3 };

enum class 方法虚拟存在专用状态 : std::uint8_t {
  已形成写集 = 1,
  已读取 = 2,
  未初始化 = 3,
  已初始化 = 4,
  入口拒绝 = 5,
  方法所有者未找到 = 6,
  方法节点未找到 = 8,
  关系类型未找到 = 10,
  所有者不匹配 = 12,
  结构冲突 = 13,
  资源失败 = 15,
  内部不一致 = 16,
  未实现 = 17
};

struct 方法类结构类型 final {
  稳定编码 方法来源关系类型{};
  稳定编码 方法虚拟存在关系类型{};
  稳定编码 方法条件关系类型{};
  稳定编码 条件特征关系类型{};
  稳定编码 方法结果方向关系类型{};
  稳定编码 结果行动方向关系类型{};
  稳定编码 结果结果方向关系类型{};
  稳定编码 场景参数特征类型关系类型{};
  稳定编码 存在参数特征类型关系类型{};
  稳定编码 特征参数特征类型关系类型{};
  friend bool operator==(const 方法类结构类型 &,
                         const 方法类结构类型 &) = default;
};

struct 方法类记录身份 final {
  稳定编码 值{};
  friend bool operator==(const 方法类记录身份 &,
                         const 方法类记录身份 &) = default;
};

struct 方法类条件项身份 final {
  稳定编码 值{};
  friend bool operator==(const 方法类条件项身份 &,
                         const 方法类条件项身份 &) = default;
};

struct 方法类结果方向项身份 final {
  稳定编码 值{};
  friend bool operator==(const 方法类结果方向项身份 &,
                         const 方法类结果方向项身份 &) = default;
};

struct 方法类条件项 final {
  方法类条件项身份 身份;
  方法类记录身份 方法;
  稳定编码 条件特征{};
  稳定编码 方法条件关系{};
  稳定编码 条件特征关系{};
  friend bool operator==(const 方法类条件项 &,
                         const 方法类条件项 &) = default;
};

struct 方法类结果方向项 final {
  方法类结果方向项身份 身份;
  方法类记录身份 方法;
  特征类定义身份 行动变化方向;
  特征类定义身份 结果方向;
  稳定编码 方法结果方向关系{};
  稳定编码 结果行动方向关系{};
  稳定编码 结果结果方向关系{};
  friend bool operator==(const 方法类结果方向项 &,
                         const 方法类结果方向项 &) = default;
};

struct 方法类参数规格项 final {
  方法类记录身份 方法;
  稳定编码 方法虚拟存在节点{};
  方法参数类别 类别 = 方法参数类别::场景;
  稳定编码 参数特征类型{};
  稳定编码 关系{};
  std::uint64_t 顺序 = 0;
  friend bool operator==(const 方法类参数规格项 &,
                         const 方法类参数规格项 &) = default;
};

struct 方法虚拟存在投影 final {
  稳定编码 方法节点{};
  稳定编码 虚拟存在节点{};
  稳定编码 方法虚拟存在关系{};
  friend bool operator==(const 方法虚拟存在投影 &,
                         const 方法虚拟存在投影 &) = default;
};

struct 方法类记录 final {
  方法类记录身份 身份;
  稳定编码 来源节点{};
  稳定编码 来源关系{};
  std::optional<方法虚拟存在投影> 虚拟存在;
  std::vector<方法类条件项> 条件组;
  std::vector<方法类结果方向项> 结果方向组;
  std::vector<方法类参数规格项> 参数规格组;
  friend bool operator==(const 方法类记录 &, const 方法类记录 &) = default;
};

inline bool 方法类条件项完整(const 方法类条件项 &item) noexcept {
  return 有效(item.身份.值) && 有效(item.方法.值)
      && 有效(item.条件特征) && 有效(item.方法条件关系)
      && 有效(item.条件特征关系);
}

inline bool 方法类结果方向项完整(
    const 方法类结果方向项 &item) noexcept {
  return 有效(item.身份.值) && 有效(item.方法.值)
      && 有效(item.行动变化方向.结点) && 有效(item.结果方向.结点)
      && 有效(item.方法结果方向关系)
      && 有效(item.结果行动方向关系)
      && 有效(item.结果结果方向关系);
}

inline bool 方法类参数规格项完整(
    const 方法类参数规格项 &item) noexcept {
  return 有效(item.方法.值) && 有效(item.参数特征类型)
      && 有效(item.方法虚拟存在节点) && 有效(item.关系) && item.顺序 != 0
      && (item.类别 == 方法参数类别::场景
          || item.类别 == 方法参数类别::存在
          || item.类别 == 方法参数类别::特征);
}

struct 方法类新增请求 final {
  L1所有者范围写入幂等身份 幂等身份{};
  稳定编码 来源节点{};
  friend bool operator==(const 方法类新增请求 &,
                         const 方法类新增请求 &) = default;
};

struct 方法类身份查询请求 final { 方法类记录身份 方法; };
struct 方法类精确查询请求 final { 稳定编码 来源节点{}; };

struct 方法类记录结果 final {
  方法类数据状态 状态 = 方法类数据状态::入口拒绝;
  std::optional<方法类记录> 记录;
  bool 成功() const noexcept;
  friend bool operator==(const 方法类记录结果 &,
                         const 方法类记录结果 &) = default;
};

struct 方法类虚拟存在初始化请求 final {
  L1所有者范围写入幂等身份 幂等身份{};
  方法类记录身份 方法;
  friend bool operator==(const 方法类虚拟存在初始化请求 &,
                         const 方法类虚拟存在初始化请求 &) = default;
};

struct 方法类虚拟存在结果 final {
  方法类数据状态 状态 = 方法类数据状态::入口拒绝;
  方法虚拟存在专用状态 提供者状态 = 方法虚拟存在专用状态::入口拒绝;
  方法类记录身份 方法;
  std::optional<方法虚拟存在投影> 虚拟存在;
  bool 成功() const noexcept;
  friend bool operator==(const 方法类虚拟存在结果 &,
                         const 方法类虚拟存在结果 &) = default;
};

struct 方法类条件添加请求 final {
  L1所有者范围写入幂等身份 幂等身份{};
  方法类记录身份 方法;
  稳定编码 条件特征{};
  friend bool operator==(const 方法类条件添加请求 &,
                         const 方法类条件添加请求 &) = default;
};

struct 方法类条件组读取请求 final { 方法类记录身份 方法; };

struct 方法类条件结果 final {
  方法类数据状态 状态 = 方法类数据状态::入口拒绝;
  std::optional<方法类条件项> 条件项;
  bool 成功() const noexcept;
  friend bool operator==(const 方法类条件结果 &,
                         const 方法类条件结果 &) = default;
};

struct 方法类条件组结果 final {
  方法类数据状态 状态 = 方法类数据状态::入口拒绝;
  方法类记录身份 方法;
  std::vector<方法类条件项> 条件组;
  bool 成功() const noexcept;
  friend bool operator==(const 方法类条件组结果 &,
                         const 方法类条件组结果 &) = default;
};

struct 方法类结果方向添加请求 final {
  L1所有者范围写入幂等身份 幂等身份{};
  方法类记录身份 方法;
  特征类定义身份 行动变化方向;
  特征类定义身份 结果方向;
  friend bool operator==(const 方法类结果方向添加请求 &,
                         const 方法类结果方向添加请求 &) = default;
};

struct 方法类结果方向组读取请求 final { 方法类记录身份 方法; };

struct 方法类结果方向结果 final {
  方法类数据状态 状态 = 方法类数据状态::入口拒绝;
  std::optional<方法类结果方向项> 结果方向项;
  bool 成功() const noexcept;
  friend bool operator==(const 方法类结果方向结果 &,
                         const 方法类结果方向结果 &) = default;
};

struct 方法类结果方向组结果 final {
  方法类数据状态 状态 = 方法类数据状态::入口拒绝;
  方法类记录身份 方法;
  std::vector<方法类结果方向项> 结果方向组;
  bool 成功() const noexcept;
  friend bool operator==(const 方法类结果方向组结果 &,
                         const 方法类结果方向组结果 &) = default;
};

struct 方法类参数规格添加请求 final {
  L1所有者范围写入幂等身份 幂等身份{};
  方法类记录身份 方法;
  方法参数类别 类别 = 方法参数类别::场景;
  稳定编码 参数特征类型{};
  std::uint64_t 顺序 = 0;
  friend bool operator==(const 方法类参数规格添加请求 &,
                         const 方法类参数规格添加请求 &) = default;
};

struct 方法类参数规格组读取请求 final { 方法类记录身份 方法; };

struct 方法类参数规格结果 final {
  方法类数据状态 状态 = 方法类数据状态::入口拒绝;
  std::optional<方法类参数规格项> 参数规格;
  bool 成功() const noexcept;
  friend bool operator==(const 方法类参数规格结果 &,
                         const 方法类参数规格结果 &) = default;
};

struct 方法类参数规格组结果 final {
  方法类数据状态 状态 = 方法类数据状态::入口拒绝;
  方法类记录身份 方法;
  稳定编码 方法虚拟存在节点{};
  std::vector<方法类参数规格项> 参数规格组;
  bool 成功() const noexcept;
  friend bool operator==(const 方法类参数规格组结果 &,
                         const 方法类参数规格组结果 &) = default;
};

class 方法类数据服务 final {
public:
  方法类数据服务() = delete;
  方法类数据服务(const 方法类数据服务 &) = delete;
  方法类数据服务 &operator=(const 方法类数据服务 &) = delete;
  方法类数据服务(方法类数据服务 &&) = delete;
  方法类数据服务 &operator=(方法类数据服务 &&) = delete;

  方法类数据服务(const L1事实基座服务 &,
                   const 特征类数据服务 &,
                   const 存在类数据服务 &,
                   L1所有者范围写端口 &&,
                   方法类结构类型);

  bool 绑定于(const L1事实基座服务 &) const noexcept;

  方法类记录结果 新增方法记录(const 方法类新增请求 &);
  方法类记录结果 查询方法记录(const 方法类身份查询请求 &) const;
  方法类记录结果 按来源节点精确查询(
      const 方法类精确查询请求 &) const;
  方法类虚拟存在结果 初始化方法虚拟存在(
      const 方法类虚拟存在初始化请求 &);
  方法类条件结果 添加条件项(const 方法类条件添加请求 &);
  方法类条件组结果 读取条件项组(
      const 方法类条件组读取请求 &) const;
  方法类结果方向结果 添加结果方向项(
      const 方法类结果方向添加请求 &);
  方法类结果方向组结果 读取结果方向项组(
      const 方法类结果方向组读取请求 &) const;
  方法类参数规格结果 添加方法参数规格(
      const 方法类参数规格添加请求 &);
  方法类参数规格组结果 读取方法参数规格组(
      const 方法类参数规格组读取请求 &) const;

private:
  bool 结构类型有效() const noexcept;
  bool 当前结构有效() const;

  static 方法类记录结果 记录失败(方法类数据状态) noexcept;
  static 方法类条件结果 条件失败(方法类数据状态) noexcept;
  static 方法类虚拟存在结果 虚拟存在失败(
      方法类数据状态, 方法虚拟存在专用状态,
      方法类记录身份 = {}) noexcept;
  static 方法类条件组结果 条件组失败(
      方法类数据状态, 方法类记录身份 = {}) noexcept;
  static 方法类结果方向结果 结果失败(方法类数据状态) noexcept;
  static 方法类结果方向组结果 结果组失败(
      方法类数据状态, 方法类记录身份 = {}) noexcept;
  static 方法类参数规格结果 参数失败(方法类数据状态) noexcept;
  static 方法类参数规格组结果 参数组失败(
      方法类数据状态, 方法类记录身份 = {}) noexcept;

  const L1事实基座服务 &第一层服务_;
  const 特征类数据服务 &特征服务_;
  const 存在类数据服务 &存在服务_;
  L1所有者范围写端口 写入端口_;
  L1结构所有者身份 所有者_{};
  方法类结构类型 结构类型_;
};

} // namespace 海中鱼巣
