#pragma once

#include "../领域/数据服务.概念树类.h"
#include "../领域/数据服务.需求类.h"
#include "../领域/算法.有序I64特征比较.h"

#include <cstdint>
#include <optional>
#include <variant>
#include <vector>

namespace 海中鱼巣 {

struct 二次关系准确F来源 final {
  特征信息身份 F;
  friend bool operator==(const 二次关系准确F来源 &,
                         const 二次关系准确F来源 &) = default;
};

struct 二次关系状态端点来源 final {
  状态使用绑定身份 B;
  friend bool operator==(const 二次关系状态端点来源 &,
                         const 二次关系状态端点来源 &) = default;
};

struct 二次关系本能根目标合同值来源 final {
  本能根角色 角色 = 本能根角色::安全;
  稳定编码 根需求{}, 根目标合同{}, 目标值事实{};
  特征信息身份 对应实际特征;
  friend bool operator==(const 二次关系本能根目标合同值来源 &,
                         const 二次关系本能根目标合同值来源 &) = default;
};

using 二次关系特征来源 =
    std::variant<二次关系准确F来源, 二次关系状态端点来源,
                 二次关系本能根目标合同值来源>;

struct 二次关系参与者材料 final {
  稳定编码 E{};
  std::vector<二次关系特征来源> 来源组;
  friend bool operator==(const 二次关系参与者材料 &,
                         const 二次关系参与者材料 &) = default;
};

struct 二次关系求值请求 final {
  std::uint32_t 版本 = 2;
  std::uint64_t Gread = 0, H = 0, 请求身份 = 0;
  概念树概念身份 RC;
  二次关系参与者材料 A, B;
};

struct 二次关系FC求值请求 final {
  std::uint32_t 版本 = 2;
  std::uint64_t Gread = 0, H = 0, 请求身份 = 0;
  概念树概念身份 FC;
  二次关系参与者材料 参与者;
  std::uint64_t 来源下标 = 0;
};

struct 二次关系EC求值请求 final {
  std::uint32_t 版本 = 2;
  std::uint64_t Gread = 0, H = 0, 请求身份 = 0;
  概念树概念身份 EC;
  二次关系参与者材料 参与者;
};

struct 二次关系候选求值请求 final {
  std::uint32_t 版本 = 2;
  std::uint64_t Gread = 0, H = 0, 请求身份 = 0;
  二次关系定义 定义;
  二次关系参与者材料 A, B;
};

enum class 二次关系判断状态 : std::uint8_t {
  命中 = 1,
  不命中 = 2,
  证据不足 = 3,
  规则不支持 = 4,
  入口拒绝 = 5,
  旧预算不足 = 6,
  事实代次漂移 = 7,
  历史材料不可用 = 8,
  运算溢出 = 9,
  资源失败 = 10,
  内部不一致 = 11,
  退役不可用 = 12,
  类型不相容 = 13,
  单位量化不相容 = 14,
  参照不相容 = 15
};

struct 二次关系本能根目标合同值见证 final {
  本能根材料 根材料;
  存在当前采用事实 根形成采用;
};

struct 二次关系准确来源见证 final {
  稳定编码 E{};
  二次关系特征来源 来源;
  存在身份来源历史见证 存在见证;
  特征类型身份 FT;
  std::int64_t 值 = 0;
  std::variant<准确特征读取事实, 状态内容事实,
               二次关系本能根目标合同值见证> 内容;
  std::optional<状态使用绑定事实> 绑定;
  std::optional<存在当前采用事实> 采用;
};

struct 二次关系条件见证 final {
  二次关系原子定义 定义;
  二次关系准确来源见证 A, B;
  特征I64比较绑定事实 K;
  特征比较执行结果 计算;
  bool FC命中 = false, EC命中 = false, 域命中 = false;
};

struct 二次关系判断结果 final {
  std::uint32_t 版本 = 2;
  二次关系判断状态 状态 = 二次关系判断状态::入口拒绝;
  std::uint64_t Gread = 0, H = 0, 定义H = 0, 请求身份 = 0;
  std::vector<二次关系条件见证> 条件组;
  std::vector<二次关系准确来源见证> 来源组;
  std::vector<概念树概念身份> 已核验概念;
};

class 二次关系求值应用服务 final {
public:
  二次关系求值应用服务(const L1事实基座服务 &,
                         const 概念树类数据服务 &,
                         const 状态使用绑定只读提供者 &,
                         const 状态类数据服务 &,
                         const 存在类数据服务 &,
                         const 特征类数据服务 &,
                         const 特征值类数据服务 &,
                         const 需求类数据服务 &,
                         const 有序I64特征比较提供者 &);

  二次关系判断结果 求值二次关系(const 二次关系求值请求 &) const noexcept;
  二次关系判断结果 求值特征概念(const 二次关系FC求值请求 &) const noexcept;
  二次关系判断结果 求值存在概念(const 二次关系EC求值请求 &) const noexcept;
  二次关系判断结果 求值候选定义(
      const 二次关系候选求值请求 &) const noexcept;

  bool 绑定于(const L1事实基座服务 &) const noexcept;
  bool 使用概念服务(const 概念树类数据服务 &) const noexcept;

  二次关系求值应用服务() = delete;
  二次关系求值应用服务(const 二次关系求值应用服务 &) = delete;
  二次关系求值应用服务 &operator=(const 二次关系求值应用服务 &) = delete;
  二次关系求值应用服务(二次关系求值应用服务 &&) = delete;
  二次关系求值应用服务 &operator=(二次关系求值应用服务 &&) = delete;

private:
  const L1事实基座服务 &l1_;
  const 概念树类数据服务 &概念_;
  const 状态使用绑定只读提供者 &绑定_;
  const 状态类数据服务 &状态_;
  const 存在类数据服务 &存在_;
  const 特征类数据服务 &特征_;
  const 特征值类数据服务 &特征值_;
  const 需求类数据服务 &需求_;
  const 有序I64特征比较提供者 &比较_;
};

} // namespace 海中鱼巣
