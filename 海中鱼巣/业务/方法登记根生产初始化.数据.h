#pragma once

// 预冻结 BIZ 候选：下列 L2 方法结构类型必须在 DATA 最终 ABI 发布后机械匹配。
// 本文件不定义 DATA 事实，也不把候选类型视为已经实现。
namespace 海中鱼巣 {

inline constexpr std::uint32_t 方法登记根生产初始化合同版本 = 1;

inline constexpr L2结构幂等身份 方法登记根固定幂等身份{
    0x4D45'5448'524F'0001ULL};

struct 方法登记根生产初始化请求 final {
  std::uint32_t 合同版本 = 方法登记根生产初始化合同版本;
  L2结构幂等身份 根建立幂等身份 = 方法登记根固定幂等身份;
  friend bool operator==(const 方法登记根生产初始化请求 &,
                         const 方法登记根生产初始化请求 &) = default;
};

enum class 方法登记根生产初始化状态 : std::uint8_t {
  已发布 = 1,
  精确重复 = 2,
  请求无效 = 3,
  已发布选择冲突 = 4,
  当前根组预读失败 = 5,
  方法登记根形成失败 = 6,
  单根正式读回失败 = 7,
  最终根组读回失败 = 8,
  事实代次竞争 = 9,
  根读回不一致 = 10,
  资源失败 = 11,
  内部不一致 = 12
};

enum class 方法登记根生产初始化来源 : std::uint8_t {
  首次形成 = 1,
  已发布重复消费 = 2
};

struct 方法登记根发布材料 final {
  方法登记根生产初始化请求 选择;
  L2方法登记根事实 方法登记根;
  std::uint64_t 最终事实截止代次 = 0;
  friend bool operator==(const 方法登记根发布材料 &,
                         const 方法登记根发布材料 &) = default;
};

struct 方法登记根组读取调用证据 final {
  L2当前方法登记根组读取请求 请求;
  L2当前方法登记根组读取结果 结果;
  friend bool operator==(const 方法登记根组读取调用证据 &,
                         const 方法登记根组读取调用证据 &) = default;
};

struct 方法登记根漂移重读证据 final {
  std::uint64_t 调用前已知事实截止代次 = 0;
  L2当前方法登记根组读取请求 请求;
  L2当前方法登记根组读取结果 结果;
  friend bool operator==(const 方法登记根漂移重读证据 &,
                         const 方法登记根漂移重读证据 &) = default;
};

struct 方法登记根建立调用证据 final {
  L2方法登记根建立请求 请求;
  L2方法登记根写入结果 结果;
  friend bool operator==(const 方法登记根建立调用证据 &,
                         const 方法登记根建立调用证据 &) = default;
};

struct 方法登记根单根读取调用证据 final {
  L2当前方法登记根读取请求 请求;
  L2当前方法登记根读取结果 结果;
  friend bool operator==(const 方法登记根单根读取调用证据 &,
                         const 方法登记根单根读取调用证据 &) = default;
};

struct 方法登记根生产初始化结果 final {
  方法登记根生产初始化状态 状态 = 方法登记根生产初始化状态::请求无效;
  std::optional<方法登记根生产初始化来源> 来源;
  std::optional<方法登记根组读取调用证据> 当前根组预读;
  std::optional<L2方法登记根建立请求> 进入时待重放请求;
  std::vector<方法登记根建立调用证据> 根建立;
  std::optional<方法登记根漂移重读证据> 漂移根组重读;
  std::optional<方法登记根单根读取调用证据> 单根读取;
  std::optional<方法登记根组读取调用证据> 最终根组读取;
  std::optional<方法登记根发布材料> 已发布材料;

  bool 成功() const noexcept;
};

} // namespace 海中鱼巣
