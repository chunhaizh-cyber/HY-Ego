#pragma once

#include "数据服务.需求类.h"

#include <cstdint>
#include <optional>

namespace 海中鱼巣 {

inline constexpr std::uint32_t 本能根任务核心合同版本_v1 = 1;

struct 本能根任务身份_v1 final { 稳定编码 值{}; friend bool operator==(const 本能根任务身份_v1&, const 本能根任务身份_v1&) = default; };
struct 本能根任务轮次身份_v1 final { 稳定编码 值{}; friend bool operator==(const 本能根任务轮次身份_v1&, const 本能根任务轮次身份_v1&) = default; };
struct 本能根任务准备身份_v1 final { 稳定编码 值{}; friend bool operator==(const 本能根任务准备身份_v1&, const 本能根任务准备身份_v1&) = default; };
struct 本能根任务初始化意图身份_v1 final { 稳定编码 值{}; friend bool operator==(const 本能根任务初始化意图身份_v1&, const 本能根任务初始化意图身份_v1&) = default; };
struct 本能根任务初始化预留身份_v1 final { 稳定编码 值{}; friend bool operator==(const 本能根任务初始化预留身份_v1&, const 本能根任务初始化预留身份_v1&) = default; };
struct 本能根任务初始化回执身份_v1 final { 稳定编码 值{}; friend bool operator==(const 本能根任务初始化回执身份_v1&, const 本能根任务初始化回执身份_v1&) = default; };
struct 本能根任务退出回执身份_v1 final { 稳定编码 值{}; friend bool operator==(const 本能根任务退出回执身份_v1&, const 本能根任务退出回执身份_v1&) = default; };

enum class 本能根任务生命周期_v1 : std::uint8_t { 当前可治理=1, 已完成=2, 已失败=3, 已取消=4, 已退出当前资格=5 };
enum class 本能根任务Vt状态_v1 : std::uint8_t { 已建立待首轮准备=1, 待找方法=2 };
enum class 本能根任务阶段状态_v1 : std::uint8_t {
  未执行=1, 不适用=2, 已发布=3, 精确重复=4, 已读取=5,
  入口拒绝=6, 未找到=7, 当前任务不可复用=8, 目标不一致=9,
  事实代次漂移=10, 幂等冲突=11, 引用冲突=12,
  已可能发布=14, 号段耗尽=15,
  待迁移=16, 资源失败=17, 内部不一致=18
};
enum class 本能根任务承接总状态_v1 : std::uint8_t {
  已完成=1, 精确重复=2, 已承接到当前任务=3, 入口拒绝=4,
  未找到=5, 当前任务不可复用=6, 目标不一致=7, 事实代次漂移=8,
  幂等冲突=9, 引用冲突=10, 已可能发布=12,
  号段耗尽=13, 待迁移=14, 资源失败=15, 内部不一致=16
};

struct 本能根任务目标来源定位_v1 final {
  本能根角色 根角色 = 本能根角色::安全;
  需求类记录身份 D;
  稳定编码 L{};
  特征信息身份 根形成F;
  friend bool operator==(const 本能根任务目标来源定位_v1&, const 本能根任务目标来源定位_v1&) = default;
};
struct 本能根任务目标投影_v1 final {
  本能根角色 根角色 = 本能根角色::安全;
  需求类记录身份 D;
  稳定编码 L{}, 目标宿主E{};
  特征类型身份 目标FT;
  稳定编码 目标状态合同{};
  std::int64_t 目标I64值 = 0;
  friend bool operator==(const 本能根任务目标投影_v1&, const 本能根任务目标投影_v1&) = default;
};

struct 本能根任务核心结构交付_v1 final {
  std::uint32_t 合同版本 = 本能根任务核心合同版本_v1;
  L1结构所有者身份 task_owner{};
  稳定编码 任务族根{}, 预留记录族根{}, 初始化回执族根{};
  稳定编码 任务族成员关系类型{}, 任务查询锚点关系类型{}, L当前任务关系类型{},
      任务来源D关系类型{}, 任务Vt关系类型{}, 任务R1关系类型{}, R1的P1关系类型{},
      预留记录成员关系类型{}, 初始化回执成员关系类型{};
  稳定编码 私有高水位属性类型{}, 任务生命周期属性类型{}, Vt状态属性类型{},
      预留材料属性类型{}, 根来源定位属性类型{}, 初始化回执材料属性类型{};
  friend bool operator==(const 本能根任务核心结构交付_v1&, const 本能根任务核心结构交付_v1&) = default;
};
struct 本能根任务核心结构登记请求_v1 final {
  std::uint32_t 合同版本 = 本能根任务核心合同版本_v1;
  std::uint64_t 期望事实代次 = 0;
  L1所有者范围写入幂等身份 幂等身份{};
  friend bool operator==(const 本能根任务核心结构登记请求_v1&, const 本能根任务核心结构登记请求_v1&) = default;
};
struct 本能根任务核心结构登记结果_v1 final {
  本能根任务阶段状态_v1 状态 = 本能根任务阶段状态_v1::入口拒绝;
  std::uint32_t 合同版本 = 本能根任务核心合同版本_v1;
  std::uint64_t 事实代次 = 0;
  std::optional<本能根任务核心结构交付_v1> 交付;
  bool 成功(const 本能根任务核心结构登记请求_v1&) const noexcept;
};

struct 本能根任务初始化语义请求_v1 final {
  std::uint32_t 合同版本 = 本能根任务核心合同版本_v1;
  本能根任务初始化意图身份_v1 意图;
  std::uint64_t 期望事实代次 = 0;
  本能根任务目标来源定位_v1 来源;
  std::optional<本能根任务身份_v1> 前序任务;
  friend bool operator==(const 本能根任务初始化语义请求_v1&, const 本能根任务初始化语义请求_v1&) = default;
};
struct 不可变本能根任务初始化包_v1 final {
  本能根任务初始化语义请求_v1 原请求;
  本能根任务初始化预留身份_v1 预留记录;
  std::uint64_t 预留序号 = 0;
  L1所有者范围写入幂等身份 控制幂等身份{}, 任务核心建立幂等身份{}, P1建立幂等身份{}, Vt首迁移幂等身份{};
  std::uint64_t 预留形成事实代次 = 0;
  friend bool operator==(const 不可变本能根任务初始化包_v1&, const 不可变本能根任务初始化包_v1&) = default;
};
struct 本能根任务初始化包结果_v1 final {
  本能根任务阶段状态_v1 状态 = 本能根任务阶段状态_v1::入口拒绝;
  std::uint32_t 合同版本 = 本能根任务核心合同版本_v1;
  std::uint64_t 事实代次 = 0;
  本能根任务初始化语义请求_v1 原请求;
  std::optional<不可变本能根任务初始化包_v1> 包;
  bool 成功(const 本能根任务初始化语义请求_v1&) const noexcept;
};
struct 本能根任务核心投影_v1 final {
  本能根任务身份_v1 T;
  稳定编码 L{};
  需求类记录身份 D;
  稳定编码 T到D关系{}, Vt{};
  本能根任务轮次身份_v1 R1;
  std::optional<本能根任务准备身份_v1> P1;
  本能根任务生命周期_v1 生命周期 = 本能根任务生命周期_v1::当前可治理;
  本能根任务Vt状态_v1 Vt状态 = 本能根任务Vt状态_v1::已建立待首轮准备;
  本能根任务目标来源定位_v1 首次来源;
  L1所有者范围写入幂等身份 核心键{}, P1键{}, 首迁移键{};
  std::uint64_t 核心首次发布代次=0, P1首次发布代次=0, 首迁移首次发布代次=0;
  friend bool operator==(const 本能根任务核心投影_v1&, const 本能根任务核心投影_v1&) = default;
};
struct 本能根任务承接结果_v1 final {
  本能根任务承接总状态_v1 状态 = 本能根任务承接总状态_v1::入口拒绝;
  std::uint32_t 合同版本 = 本能根任务核心合同版本_v1;
  不可变本能根任务初始化包_v1 原包;
  本能根任务阶段状态_v1 核心阶段=本能根任务阶段状态_v1::未执行,
      P1阶段=本能根任务阶段状态_v1::未执行,
      首迁移阶段=本能根任务阶段状态_v1::未执行;
  std::uint64_t 事实代次 = 0;
  std::optional<本能根任务核心投影_v1> 核心;
  bool 成功(const 不可变本能根任务初始化包_v1&) const noexcept;
};

struct 本能根任务身份读取请求_v1 final { std::uint32_t 合同版本=1; std::uint64_t Gread=0; 本能根任务身份_v1 T; };
struct 本能根任务锚点读取请求_v1 final { std::uint32_t 合同版本=1; std::uint64_t Gread=0; 稳定编码 L{}; };
struct 本能根任务目标投影读取请求_v1 final { std::uint32_t 合同版本=1; std::uint64_t Gread=0; 本能根任务身份_v1 T; };
struct 本能根任务核心读取结果_v1 final {
  本能根任务阶段状态_v1 状态=本能根任务阶段状态_v1::入口拒绝; std::uint32_t 合同版本=1; std::uint64_t Gread=0; std::optional<本能根任务核心投影_v1> 核心;
  bool 成功(const 本能根任务身份读取请求_v1&) const noexcept;
  bool 成功(const 本能根任务锚点读取请求_v1&) const noexcept;
};
struct 本能根任务目标投影结果_v1 final {
  本能根任务阶段状态_v1 状态=本能根任务阶段状态_v1::入口拒绝; std::uint32_t 合同版本=1; std::uint64_t Gread=0; std::optional<本能根任务目标投影_v1> 投影;
  bool 成功(const 本能根任务目标投影读取请求_v1&) const noexcept;
};
struct 本能根任务当前资格退出请求_v1 final {
  std::uint32_t 合同版本=1; std::uint64_t 期望事实代次=0; 本能根任务身份_v1 T; 稳定编码 L{}; 本能根任务生命周期_v1 期望前生命周期=本能根任务生命周期_v1::当前可治理,目标生命周期=本能根任务生命周期_v1::已退出当前资格; L1所有者范围写入幂等身份 幂等身份{};
};
struct 本能根任务当前资格退出结果_v1 final {
  本能根任务阶段状态_v1 状态=本能根任务阶段状态_v1::入口拒绝; std::uint32_t 合同版本=1; std::uint64_t 事实代次=0; 本能根任务身份_v1 T; 稳定编码 L{}; 本能根任务生命周期_v1 目标生命周期=本能根任务生命周期_v1::已退出当前资格; std::optional<稳定编码> 新生命周期值; std::optional<本能根任务退出回执身份_v1> 退出回执;
  bool 成功(const 本能根任务当前资格退出请求_v1&) const noexcept;
};

class 本能根任务核心端口_v1 {
public:
  virtual ~本能根任务核心端口_v1() = default;
  virtual 本能根任务初始化包结果_v1 签发或恢复不可变初始化包(const 本能根任务初始化语义请求_v1&) noexcept = 0;
  virtual 本能根任务承接结果_v1 承接或建立任务(const 不可变本能根任务初始化包_v1&) noexcept = 0;
  virtual 本能根任务承接结果_v1 恢复任务初始化(const 不可变本能根任务初始化包_v1&) noexcept = 0;
  virtual 本能根任务核心读取结果_v1 按任务读取核心(const 本能根任务身份读取请求_v1&) const noexcept = 0;
  virtual 本能根任务核心读取结果_v1 按查询锚点读取当前任务(const 本能根任务锚点读取请求_v1&) const noexcept = 0;
  virtual 本能根任务目标投影结果_v1 按任务读取目标投影(const 本能根任务目标投影读取请求_v1&) const noexcept = 0;
  virtual 本能根任务当前资格退出结果_v1 退出任务当前资格(const 本能根任务当前资格退出请求_v1&) noexcept = 0;
};

class 本能根任务核心服务_v1 final : public 本能根任务核心端口_v1 {
public:
  static 本能根任务核心结构登记结果_v1 登记结构(const L1事实基座服务&,L1所有者范围写端口&,const 本能根任务核心结构登记请求_v1&) noexcept;
  本能根任务核心服务_v1(const L1事实基座服务&,const 需求类数据服务&,const 存在类数据服务&,const 特征类数据服务&,L1所有者范围写端口&&,const 本能根任务核心结构交付_v1&,存在单例角色身份);
  本能根任务初始化包结果_v1 签发或恢复不可变初始化包(const 本能根任务初始化语义请求_v1&) noexcept override;
  本能根任务承接结果_v1 承接或建立任务(const 不可变本能根任务初始化包_v1&) noexcept override;
  本能根任务承接结果_v1 恢复任务初始化(const 不可变本能根任务初始化包_v1&) noexcept override;
  本能根任务核心读取结果_v1 按任务读取核心(const 本能根任务身份读取请求_v1&) const noexcept override;
  本能根任务核心读取结果_v1 按查询锚点读取当前任务(const 本能根任务锚点读取请求_v1&) const noexcept override;
  本能根任务目标投影结果_v1 按任务读取目标投影(const 本能根任务目标投影读取请求_v1&) const noexcept override;
  本能根任务当前资格退出结果_v1 退出任务当前资格(const 本能根任务当前资格退出请求_v1&) noexcept override;
  bool 绑定于(const L1事实基座服务&) const noexcept;
private:
  const L1事实基座服务& l1_; const 需求类数据服务& 需求_; const 存在类数据服务& 存在_; const 特征类数据服务& 特征_; mutable L1所有者范围写端口 写端口_; L1结构所有者身份 owner_{}; 本能根任务核心结构交付_v1 结构_{}; 存在单例角色身份 self角色_{};
};

} // namespace 海中鱼巣
