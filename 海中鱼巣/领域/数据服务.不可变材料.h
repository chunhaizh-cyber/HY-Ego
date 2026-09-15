#pragma once

#include <algorithm>
#include <array>
#define NOMINMAX
#include <Windows.h>
#include <bcrypt.h>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <limits>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>
#include "合同.结构操作公共.h"

namespace 海中鱼巣 {



inline constexpr std::uint32_t 不可变材料合同版本_B1 = 1;

enum class 不可变材料格式种类_B1 : std::uint8_t {
  有序I64载荷 = 1, 有序U64载荷 = 2
};

enum class 材料状态_B1 : std::uint8_t {
  已读取=1, 已创建=2, 已复用=3, 精确重复=4, 已退出=6,
  未找到=8, 入口拒绝=9, 目标已退出=11, 规则未提供=17,
  引用保护=18, 事实代次漂移=22, 幂等冲突=23,
  数量预算不足=24, 历史材料不可用=25, 资源失败=26,
  内部不一致=27, 可能已发布=28, 旧格式不支持=29
};
enum class 材料发布状态_B1 : std::uint8_t {
  未进入=0, 确认未发布=1, 确认发布=2, 可能发布=3
};

struct 不可变材料布局_B1 final {
  稳定编码 格式锚点{}, 族锚点{}, 类型登记{}, 格式版本{}, 族成员{}, 来源{};
  稳定编码 材料格式{}, 字节长度{}, 摘要{}, A承接{}, 载荷格式代码{};
  不可变材料格式身份_B1 有序I64格式{}, 有序U64格式{};
  稳定编码 A承接锚点{};
  std::uint64_t 首次G0{}, 首次H{};
  friend bool operator==(const 不可变材料布局_B1&, const 不可变材料布局_B1&) = default;
};
struct 原A特征定义承接读取请求_B1 final {
  std::uint32_t 版本{1}; std::uint64_t Gread{}; 稳定编码 预期锚点{};
  世界结构预算_B1 预算{};
  friend bool operator==(const 原A特征定义承接读取请求_B1&,
                         const 原A特征定义承接读取请求_B1&) = default;
};
struct 原A特征定义承接事实_B1 final {
  std::uint64_t Gread{}, 首次G0{}, 首次H{};
  std::array<结构节点见证_B1,16> 元节点{};
  friend bool operator==(const 原A特征定义承接事实_B1&,
                         const 原A特征定义承接事实_B1&) = default;
};
struct 原A特征定义承接读取结果_B1 final {
  std::uint32_t 版本{1}; 材料状态_B1 状态{材料状态_B1::入口拒绝};
  std::uint64_t Gread{}; std::optional<原A特征定义承接事实_B1> 承接;
  世界结构用量_B1 用量{};
  bool 成功(const 原A特征定义承接读取请求_B1&) const noexcept;
};
struct 不可变材料登记请求_B1 final {
  std::uint32_t 版本{1}; std::uint64_t G0{};
  L1所有者范围写入幂等身份 幂等身份{}; 稳定编码 A承接锚点{};
  世界结构预算_B1 预算{};
  friend bool operator==(const 不可变材料登记请求_B1&, const 不可变材料登记请求_B1&) = default;
};
struct 不可变材料登记结果_B1 final {
  std::uint32_t 版本{1}; 材料状态_B1 状态{材料状态_B1::入口拒绝};
  std::uint64_t Gread{}; std::optional<std::uint64_t> 首次H;
  材料发布状态_B1 发布{材料发布状态_B1::未进入};
  std::optional<不可变材料布局_B1> 布局;
  std::optional<原A特征定义承接事实_B1> A承接;
  世界结构用量_B1 用量{};
  bool 成功(const 不可变材料登记请求_B1&) const noexcept;
};

class 材料构造失败_B1 final : public std::exception {
public:
  材料状态_B1 原因; std::uint64_t Gread; std::optional<std::uint64_t> 首次H;
  材料发布状态_B1 发布; 世界结构用量_B1 用量;
  材料构造失败_B1(材料状态_B1 s,std::uint64_t g,std::optional<std::uint64_t> h,
                   材料发布状态_B1 p,世界结构用量_B1 u) noexcept
      :原因(s),Gread(g),首次H(h),发布(p),用量(u){}
  const char* what() const noexcept override {
    return "B1 material construction validation failed";
  }
};
struct 材料发布请求_B1 final {
  std::uint32_t 版本{1}; std::uint64_t G0{};
  L1所有者范围写入幂等身份 幂等身份{};
  不可变材料格式身份_B1 格式{}; 稳定编码 来源{};
  std::vector<std::uint8_t> 完整载荷; 世界结构预算_B1 预算{};
  friend bool operator==(const 材料发布请求_B1&, const 材料发布请求_B1&) = default;
};
struct 材料读取请求_B1 final {
  std::uint32_t 版本{1}; std::uint64_t Gread{}, H{};
  不可变材料身份_B1 材料{}; 世界结构预算_B1 预算{};
  friend bool operator==(const 材料读取请求_B1&, const 材料读取请求_B1&) = default;
};
struct 材料退出请求_B1 final {
  std::uint32_t 版本{1}; std::uint64_t G0{};
  L1所有者范围写入幂等身份 幂等身份{};
  不可变材料身份_B1 材料{}; 世界结构预算_B1 预算{};
  friend bool operator==(const 材料退出请求_B1&, const 材料退出请求_B1&) = default;
};
struct 材料格式读取请求_B1 final {
  std::uint32_t 版本{1}; std::uint64_t Gread{}, H{};
  不可变材料格式种类_B1 种类{不可变材料格式种类_B1::有序I64载荷};
  世界结构预算_B1 预算{};
  friend bool operator==(const 材料格式读取请求_B1&, const 材料格式读取请求_B1&) = default;
};
struct 材料属性见证_B1 final {
  稳定编码 编码{}, 所属{}, 类型{}, 来源{};
  std::variant<std::int64_t,std::vector<std::uint64_t>> 内容;
  结构生命周期_B1 生命周期;
  friend bool operator==(const 材料属性见证_B1&, const 材料属性见证_B1&) = default;
};
struct 材料格式事实_B1 final {
  不可变材料格式身份_B1 格式{};
  不可变材料格式种类_B1 种类{不可变材料格式种类_B1::有序I64载荷};
  结构节点见证_B1 格式节点; 材料属性见证_B1 格式代码;
  结构生命周期_B1 登记生命周期;
  friend bool operator==(const 材料格式事实_B1&, const 材料格式事实_B1&) = default;
};
struct 材料事实_B1 final {
  不可变材料身份_B1 材料{}; 不可变材料格式身份_B1 格式{};
  稳定编码 族关系{}, 格式关系{}, 来源关系{}, 来源{}, 长度值{}, 摘要值{};
  std::uint64_t 字节长度{}; std::array<std::uint8_t,32> SHA256{};
  结构生命周期_B1 生命周期; std::vector<std::uint8_t> 完整载荷;
  材料格式事实_B1 格式见证;
  friend bool operator==(const 材料事实_B1&, const 材料事实_B1&) = default;
};
struct 材料读取结果_B1 final {
  std::uint32_t 版本{1}; 材料状态_B1 状态{材料状态_B1::入口拒绝};
  std::uint64_t Gread{},H{}; 世界结构用量_B1 用量{};
  std::optional<材料事实_B1> 材料;
  bool 成功(const 材料读取请求_B1&) const noexcept;
};
struct 材料格式读取结果_B1 final {
  std::uint32_t 版本{1}; 材料状态_B1 状态{材料状态_B1::入口拒绝};
  std::uint64_t Gread{},H{}; 世界结构用量_B1 用量{};
  std::optional<材料格式事实_B1> 格式;
  bool 成功(const 材料格式读取请求_B1&) const noexcept;
};
struct 材料发布结果_B1 final {
  std::uint32_t 版本{1}; 材料状态_B1 状态{材料状态_B1::入口拒绝};
  std::uint64_t Gread{}; 世界结构用量_B1 用量{};
  std::optional<std::uint64_t> 首次H; 材料发布状态_B1 发布{材料发布状态_B1::未进入};
  std::optional<材料发布请求_B1> 原请求; 当前终态_B1 当前终态{当前终态_B1::未读取};
  std::optional<材料事实_B1> 首次材料, 当前材料; bool 本次来源已记录{};
  bool 成功(const 材料发布请求_B1&) const noexcept;
  bool 原操作已确认() const noexcept;
};
struct 材料退出结果_B1 final {
  std::uint32_t 版本{1}; 材料状态_B1 状态{材料状态_B1::入口拒绝};
  std::uint64_t Gread{}; 世界结构用量_B1 用量{};
  std::optional<std::uint64_t> 首次H; 材料发布状态_B1 发布{材料发布状态_B1::未进入};
  std::optional<材料退出请求_B1> 原请求; 当前终态_B1 当前终态{当前终态_B1::未读取};
  std::optional<材料事实_B1> 退出前材料; std::optional<结构生命周期_B1> 当前生命周期;
  std::vector<结构关系见证_B1> 已退出关系; std::vector<材料属性见证_B1> 已退出属性;
  std::vector<结构关系见证_B1> 退出前关系; std::vector<材料属性见证_B1> 退出前属性;
  bool 成功(const 材料退出请求_B1&) const noexcept;
  bool 原操作已确认() const noexcept;
};

class 不可变材料数据服务 final {
public:
  不可变材料数据服务(const L1事实基座服务&,L1所有者范围写端口&&,
                       const 不可变材料布局_B1&,const std::filesystem::path&,
                       const L1所有者范围写端口& 原A定义端口);
  ~不可变材料数据服务() noexcept;
  bool 绑定于(const L1事实基座服务&) const noexcept;
  材料发布结果_B1 发布材料(const 材料发布请求_B1&) noexcept;
  材料读取结果_B1 读取材料(const 材料读取请求_B1&) const noexcept;
  材料退出结果_B1 退出材料(const 材料退出请求_B1&) noexcept;
  材料格式读取结果_B1 读取材料格式(const 材料格式读取请求_B1&) const noexcept;
  static 不可变材料登记结果_B1 登记不可变材料结构_B1(
      const 不可变材料登记请求_B1&,L1事实基座服务&,L1所有者范围写端口&,
      const L1所有者范围写端口& 原A定义端口) noexcept;
  static 原A特征定义承接读取结果_B1 读取原A特征定义承接_B1(
      const 原A特征定义承接读取请求_B1&,const L1事实基座服务&,
      const L1所有者范围写端口& 原A定义端口) noexcept;
private:
  struct 材料完整读取内部结果_B1 final {
    材料读取结果_B1 公开结果;
    std::vector<结构关系见证_B1> 关系;
    std::vector<材料属性见证_B1> 属性;
  };
  材料完整读取内部结果_B1 读取材料完整内部(
      const 材料读取请求_B1&) const noexcept;
  const L1事实基座服务& l1_; L1所有者范围写端口 port_;
  不可变材料布局_B1 layout_; std::filesystem::path directory_;
  void* directory_handle_{};
  mutable std::mutex mutex_;
};

} // namespace 海中鱼巣
