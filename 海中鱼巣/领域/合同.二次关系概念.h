#pragma once

#include "数据服务.特征类.h"
#include "合同.相关概念添加参与.h"
#include "合同.场景状态使用绑定.h"

#include <cstdint>
#include <optional>
#include <utility>
#include <variant>
#include <vector>

namespace 海中鱼巣 {

struct 二次关系结果域 final {
  std::uint8_t 掩码 = 0;
  friend bool operator==(const 二次关系结果域 &,
                         const 二次关系结果域 &) = default;
};

enum class 二次关系约束角色 : std::uint8_t { 共同 = 1, A = 2, B = 3 };

struct 二次关系概念约束 final {
  二次关系约束角色 角色 = 二次关系约束角色::共同;
  概念树概念身份 概念;
  friend bool operator==(const 二次关系概念约束 &,
                         const 二次关系概念约束 &) = default;
};

struct 二次关系约束组 final {
  std::vector<二次关系概念约束> FC;
  std::vector<二次关系概念约束> EC;
  friend bool operator==(const 二次关系约束组 &,
                         const 二次关系约束组 &) = default;
};

struct 二次关系原子定义 final {
  二次关系结果域 D;
  二次关系约束组 约束;
  特征I64比较绑定身份 K;
  特征类标量结果角色 输出 = 特征类标量结果角色::差异;
  friend bool operator==(const 二次关系原子定义 &,
                         const 二次关系原子定义 &) = default;
};

struct 二次关系合取项 final {
  概念树概念身份 子RC;
  二次关系约束组 附加约束;
  friend bool operator==(const 二次关系合取项 &,
                         const 二次关系合取项 &) = default;
};

struct 二次关系合取定义 final {
  二次关系约束组 共同约束;
  std::vector<二次关系合取项> 条件组;
  friend bool operator==(const 二次关系合取定义 &,
                         const 二次关系合取定义 &) = default;
};

using 二次关系定义 = std::variant<二次关系原子定义, 二次关系合取定义>;

struct 二次关系规范形 final {
  std::uint32_t 版本 = 1;
  std::vector<二次关系原子定义> 原子组;
  friend bool operator==(const 二次关系规范形 &,
                         const 二次关系规范形 &) = default;
};

struct 二次关系结构类型 final {
  稳定编码 规范化规则归属{}, 规则版本{}, 定义种类{}, 定义格式{}, 域掩码{}, 输出角色{},
      固定K{}, 约束成员{}, 约束FC{}, 约束EC{}, 合取成员{}, 子RC{}, 来源成员{},
      来源F{}, 来源B{}, 来源概念{}, 用途成员{}, 用途目标{},
      用途业务依据{}, 用途业务标识{}, 用途角色{}, 用途时间{};
  friend bool operator==(const 二次关系结构类型 &,
                         const 二次关系结构类型 &) = default;
};

struct 二次关系结构交付 final {
  std::uint32_t 版本 = 1;
  稳定编码 锚点{};
  概念树规则身份 规范化规则;
  二次关系结构类型 类型;
  friend bool operator==(const 二次关系结构交付 &,
                         const 二次关系结构交付 &) = default;
};

enum class 二次关系引用种类 : std::uint8_t { FC = 1, EC = 2, K = 3, 子RC = 4 };

struct 二次关系定义引用事实 final {
  二次关系引用种类 种类 = 二次关系引用种类::FC;
  稳定编码 记录{}, 所属关系{}, 引用关系{};
  std::uint64_t 项顺序 = 0, 约束顺序 = 0;
  二次关系约束角色 角色 = 二次关系约束角色::共同;
  std::variant<概念树概念身份, 特征I64比较绑定身份> 目标;
  friend bool operator==(const 二次关系定义引用事实 &,
                         const 二次关系定义引用事实 &) = default;
};

struct 二次关系节点见证 final {
  稳定编码 编码{};
  概念树生命周期 生命周期;
  friend bool operator==(const 二次关系节点见证 &,
                         const 二次关系节点见证 &) = default;
};

struct 二次关系关系见证 final {
  稳定编码 编码{}, 源{}, 目标{}, 类型{};
  std::int64_t 角色 = 0;
  概念树生命周期 生命周期;
  friend bool operator==(const 二次关系关系见证 &,
                         const 二次关系关系见证 &) = default;
};

struct 二次关系值见证 final {
  稳定编码 编码{}, 所属节点{}, 属性类型{}, 来源{};
  std::variant<std::int64_t, std::vector<std::uint64_t>> 材料;
  概念树生命周期 生命周期;
  friend bool operator==(const 二次关系值见证 &,
                         const 二次关系值见证 &) = default;
};

struct 二次关系发布见证 final {
  std::uint32_t 版本 = 1;
  std::uint64_t 幂等身份 = 0, 发布H = 0;
  bool 已确认发布 = false, 精确重放 = false;
  std::vector<std::pair<std::uint64_t, 稳定编码>> 新编码映射;
  friend bool operator==(const 二次关系发布见证 &,
                         const 二次关系发布见证 &) = default;
};

struct 二次关系概念事实 final {
  std::uint64_t Gread = 0;
  概念树概念身份 身份;
  相关概念类别 类别 = 相关概念类别::特征;
  概念树规则身份 规则;
  二次关系定义 定义;
  二次关系规范形 规范形;
  概念树生命周期 生命周期;
  概念树生命周期状态 治理状态 = 概念树生命周期状态::活跃;
  std::vector<二次关系定义引用事实> 定义引用;
  std::vector<二次关系节点见证> 自有节点;
  std::vector<二次关系关系见证> 自有关系;
  std::vector<二次关系值见证> 自有值;
  friend bool operator==(const 二次关系概念事实 &,
                         const 二次关系概念事实 &) = default;
};

enum class 二次关系数据状态 : std::uint8_t {
  已读取 = 1, 已创建 = 2, 已复用 = 3, 精确重放 = 4, 已变更 = 5,
  同义缺失 = 6, 冷却命中 = 7, 退役命中 = 8,
  入口拒绝 = 10, 格式不支持 = 11, 规则不支持 = 12, 来源不足 = 13,
  引用冲突 = 14, 定义矛盾 = 15, 形成环 = 16, 旧预算不足 = 17,
  事实代次漂移 = 18, 幂等冲突 = 20,
  资源失败 = 21, 内部不一致 = 22, 发布未知 = 23, 未找到 = 24
};

struct 二次关系初始化请求 final {
  std::uint32_t 版本 = 1;
  std::uint64_t G0 = 0;
  L1所有者范围写入幂等身份 幂等身份;
  纯概念结构交付_v2 纯概念结构;
  std::uint64_t 最大首次材料项数 = 0;
  friend bool operator==(const 二次关系初始化请求 &,
                         const 二次关系初始化请求 &) = default;
};

struct 二次关系初始化结果 final {
  std::uint32_t 版本 = 1;
  二次关系数据状态 状态 = 二次关系数据状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<二次关系发布见证> 正式回执;
  std::optional<二次关系结构交付> 交付;
  bool 成功() const noexcept;
};

struct 二次关系概念读取请求 final {
  std::uint32_t 版本 = 2;
  概念树读取头 读取头;
  概念树概念身份 RC;
  friend bool operator==(const 二次关系概念读取请求 &,
                         const 二次关系概念读取请求 &) = default;
};

struct 二次关系定义查找请求 final {
  std::uint32_t 版本 = 2;
  概念树读取头 读取头;
  二次关系定义 定义;
  friend bool operator==(const 二次关系定义查找请求 &,
                         const 二次关系定义查找请求 &) = default;
};

struct 二次关系定义核验结果 final {
  std::uint32_t 版本 = 2;
  二次关系数据状态 状态 = 二次关系数据状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<二次关系规范形> 规范形;
  bool 成功() const noexcept;
};

struct 二次关系形成来源 final {
  std::variant<特征信息身份, 状态使用绑定身份, 概念树概念身份> 来源;
  friend bool operator==(const 二次关系形成来源 &,
                         const 二次关系形成来源 &) = default;
};

struct 二次关系概念建立请求 final {
  std::uint32_t 版本 = 2;
  概念树写入头 写入头;
  二次关系定义 定义;
  概念初始组织指定 组织 = 概念初始组织指定::未指定;
  std::vector<概念树概念身份> 初始父组;
  std::vector<二次关系形成来源> 形成来源;
  friend bool operator==(const 二次关系概念建立请求 &,
                         const 二次关系概念建立请求 &) = default;
};

struct 二次关系概念读取结果 final {
  std::uint32_t 版本 = 2;
  二次关系数据状态 状态 = 二次关系数据状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<二次关系概念事实> 事实;
  bool 成功() const noexcept;
};

struct 二次关系概念写入结果 final {
  std::uint32_t 版本 = 2;
  二次关系数据状态 状态 = 二次关系数据状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<std::uint64_t> 首次H;
  std::optional<二次关系发布见证> 正式回执;
  std::optional<二次关系概念事实> 事实;
  std::optional<二次关系概念建立请求> 建立原请求;
  bool 成功() const noexcept;
};

class 二次关系概念只读提供者 {
public:
  virtual ~二次关系概念只读提供者() = default;
  virtual bool 绑定于(const L1事实基座服务 &) const noexcept = 0;
  virtual 二次关系概念读取结果 读取二次关系概念(
      const 二次关系概念读取请求 &) const = 0;
};

struct 二次关系来源写入请求 final {
  std::uint32_t 版本 = 2;
  概念树写入头 写入头;
  概念树概念身份 RC;
  std::vector<二次关系形成来源> 来源;
  friend bool operator==(const 二次关系来源写入请求 &,
                         const 二次关系来源写入请求 &) = default;
};

struct 二次关系来源事实 final {
  稳定编码 记录{}, 成员关系{}, 目标关系{};
  概念树概念身份 RC;
  二次关系形成来源 来源;
  概念树生命周期 生命周期;
  friend bool operator==(const 二次关系来源事实 &,
                         const 二次关系来源事实 &) = default;
};

struct 二次关系用途请求 final {
  std::uint32_t 版本 = 2;
  概念树写入头 写入头;
  概念树概念身份 RC;
  std::uint64_t 业务标识 = 0;
  std::uint32_t 用途角色 = 0;
  std::int64_t 时间 = 0;
  稳定编码 业务依据{};
  friend bool operator==(const 二次关系用途请求 &,
                         const 二次关系用途请求 &) = default;
};

struct 二次关系用途事实 final {
  稳定编码 记录{}, 成员关系{}, 概念关系{}, 依据关系{};
  概念树概念身份 RC;
  std::uint64_t 业务标识 = 0;
  std::uint32_t 用途角色 = 0;
  std::int64_t 时间 = 0;
  稳定编码 业务依据{};
  概念树生命周期 生命周期;
  friend bool operator==(const 二次关系用途事实 &,
                         const 二次关系用途事实 &) = default;
};

struct 二次关系关联读取请求 final {
  std::uint32_t 版本 = 2;
  概念树读取头 读取头;
  概念树概念身份 RC;
  friend bool operator==(const 二次关系关联读取请求 &,
                         const 二次关系关联读取请求 &) = default;
};

struct 二次关系关联结果 final {
  std::uint32_t 版本 = 2;
  二次关系数据状态 状态 = 二次关系数据状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<std::uint64_t> 首次H;
  std::optional<二次关系发布见证> 正式回执;
  std::vector<二次关系来源事实> 来源组;
  std::vector<二次关系用途事实> 用途组;
  bool 成功() const noexcept;
};

struct 二次关系图读取请求 final {
  std::uint32_t 版本 = 2;
  概念树读取头 读取头;
  friend bool operator==(const 二次关系图读取请求 &,
                         const 二次关系图读取请求 &) = default;
};

struct 二次关系图结果 final {
  std::uint32_t 版本 = 2;
  二次关系数据状态 状态 = 二次关系数据状态::入口拒绝;
  std::uint64_t Gread = 0;
  相关概念类别 类别 = 相关概念类别::特征;
  std::vector<二次关系概念事实> RC组;
  std::vector<二次关系关系见证> 直接边;
  bool 成功() const noexcept;
};

struct 二次关系父组请求 final {
  std::uint32_t 版本 = 2;
  概念树写入头 写入头;
  概念树概念身份 RC;
  std::vector<二次关系关系见证> 预期直接父边;
  std::vector<概念树概念身份> 新父组;
  friend bool operator==(const 二次关系父组请求 &,
                         const 二次关系父组请求 &) = default;
};

struct 二次关系生命周期请求 final {
  std::uint32_t 版本 = 2;
  概念树写入头 写入头;
  概念树概念身份 RC;
  概念树生命周期状态 预期 = 概念树生命周期状态::活跃;
  概念树生命周期状态 目标 = 概念树生命周期状态::冷却;
  稳定编码 治理依据{};
  friend bool operator==(const 二次关系生命周期请求 &,
                         const 二次关系生命周期请求 &) = default;
};

struct 二次关系来源释放请求 final {
  std::uint32_t 版本 = 2;
  概念树写入头 写入头;
  概念树概念身份 RC;
  std::vector<二次关系来源事实> 预期来源;
  friend bool operator==(const 二次关系来源释放请求 &,
                         const 二次关系来源释放请求 &) = default;
};

struct 二次关系退出请求 final {
  std::uint32_t 版本 = 2;
  概念树写入头 写入头;
  概念树概念身份 RC;
  friend bool operator==(const 二次关系退出请求 &,
                         const 二次关系退出请求 &) = default;
};

struct 二次关系治理结果 final {
  std::uint32_t 版本 = 2;
  二次关系数据状态 状态 = 二次关系数据状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<std::uint64_t> 首次H;
  std::optional<二次关系发布见证> 正式回执;
  std::optional<二次关系概念事实> 事实;
  std::vector<二次关系关系见证> 新直接边;
  bool 成功() const noexcept;
};

} // namespace 海中鱼巣
