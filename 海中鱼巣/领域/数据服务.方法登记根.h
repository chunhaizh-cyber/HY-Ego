#pragma once

#include "../核心/服务.L1事实基座.h"

#include <cstdint>
#include <optional>
#include <vector>

namespace 海中鱼巣 {

inline constexpr std::uint64_t 方法登记根初始化所有者建立键 =
    0x4D52'524F'4F54'4F31ULL;
inline constexpr L1所有者范围写入幂等身份 方法登记根首次写入幂等身份{
    0x4D52'524F'4F54'5731ULL};
inline constexpr L1所有者范围写集本地键 方法登记根本地键{1};

struct 方法登记根身份 final {
  稳定编码 结点{};
  friend bool operator==(const 方法登记根身份 &, const 方法登记根身份 &) = default;
};

struct 方法登记根事实 final {
  方法登记根身份 身份{};
  std::uint64_t 创建事实代次 = 0;
  friend bool operator==(const 方法登记根事实 &, const 方法登记根事实 &) = default;
};

enum class 方法登记根结构状态 : std::uint8_t {
  已建立 = 0, 精确重复 = 1, 已读取 = 2, 尚未建立 = 3, 入口拒绝 = 4,
  首次材料冲突 = 5, 首次材料不完整 = 6, 当前根缺失 = 7,
  当前根形状错误 = 9, 所有者非空冲突 = 10, 事实代次漂移 = 11,
  资源失败 = 12, 已可能发布 = 13, 内部不一致 = 14, 数量预算不足 = 15
};

struct L2方法登记根初始化请求 final {
  std::uint32_t 合同版本 = 1;
  std::uint64_t G0 = 0;
  L1所有者范围写入幂等身份 幂等身份{方法登记根首次写入幂等身份};
  std::uint64_t 最大空域核验事实数 = 0;
  friend bool operator==(const L2方法登记根初始化请求 &,
                         const L2方法登记根初始化请求 &) = default;
};
struct L2方法登记根读取请求 final {
  std::uint32_t 合同版本 = 1;
  std::uint64_t Gread = 0;
  friend bool operator==(const L2方法登记根读取请求 &,
                         const L2方法登记根读取请求 &) = default;
};
struct L2方法登记根全组读取请求 final {
  std::uint32_t 合同版本 = 1;
  std::uint64_t Gread = 0;
  friend bool operator==(const L2方法登记根全组读取请求 &,
                         const L2方法登记根全组读取请求 &) = default;
};

struct L2方法登记根初始化结果 final {
  方法登记根结构状态 状态 = 方法登记根结构状态::入口拒绝;
  L2方法登记根初始化请求 请求回显{};
  std::uint64_t Gread = 0, H = 0;
  std::optional<方法登记根事实> 根;
  bool 成功() const noexcept;
};
struct L2方法登记根读取结果 final {
  方法登记根结构状态 状态 = 方法登记根结构状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::optional<方法登记根事实> 根;
  bool 成功() const noexcept;
};
struct L2方法登记根全组读取结果 final {
  方法登记根结构状态 状态 = 方法登记根结构状态::入口拒绝;
  std::uint64_t Gread = 0;
  std::vector<方法登记根事实> 根组;
  bool 成功() const noexcept;
};

class L2方法结构服务 final {
 public:
  L2方法结构服务(const L1事实基座服务 &, L1所有者范围写端口 &);
  L2方法结构服务() = delete;
  L2方法结构服务(const L2方法结构服务 &) = delete;
  L2方法结构服务 &operator=(const L2方法结构服务 &) = delete;
  L2方法结构服务(L2方法结构服务 &&) = delete;
  L2方法结构服务 &operator=(L2方法结构服务 &&) = delete;

  bool 绑定于(const L1事实基座服务 &) const noexcept;
  L2方法登记根初始化结果 建立或读取方法登记根(
      const L2方法登记根初始化请求 &);
  L2方法登记根读取结果 读取当前方法登记根(
      const L2方法登记根读取请求 &) const;
  L2方法登记根全组读取结果 读取全部当前方法登记根(
      const L2方法登记根全组读取请求 &) const;

 private:
  const L1事实基座服务 &第一层服务_;
  L1所有者范围写端口 &写入端口_;
};

class L2方法结构聚合服务 final {
 public:
  L2方法结构聚合服务(const L1事实基座服务 &, L1所有者范围写端口 &&);
  L2方法结构聚合服务() = delete;
  L2方法结构聚合服务(const L2方法结构聚合服务 &) = delete;
  L2方法结构聚合服务 &operator=(const L2方法结构聚合服务 &) = delete;
  L2方法结构聚合服务(L2方法结构聚合服务 &&) = delete;
  L2方法结构聚合服务 &operator=(L2方法结构聚合服务 &&) = delete;
  L2方法结构服务 &取得L2方法结构服务() noexcept;
  const L2方法结构服务 &取得L2方法结构服务() const noexcept;

 private:
  L1所有者范围写端口 写入端口_;
  L2方法结构服务 方法结构服务_;
};

}  // namespace 海中鱼巣
