#pragma once

namespace 海中鱼巣 {

inline constexpr std::uint32_t 四本体根生产初始化合同版本 = 1;

inline constexpr L2结构幂等身份 存在本体根固定幂等身份{
    0x434F'4E43'5254'0001ULL};
inline constexpr L2结构幂等身份 特征本体根固定幂等身份{
    0x434F'4E43'5254'0002ULL};
inline constexpr L2结构幂等身份 动态本体根固定幂等身份{
    0x434F'4E43'5254'0003ULL};
inline constexpr L2结构幂等身份 因果链本体根固定幂等身份{
    0x434F'4E43'5254'0004ULL};

struct 四本体根生产初始化请求 final {
  std::uint32_t 合同版本 = 四本体根生产初始化合同版本;
  L2结构幂等身份 存在根幂等身份 = 存在本体根固定幂等身份;
  L2结构幂等身份 特征根幂等身份 = 特征本体根固定幂等身份;
  L2结构幂等身份 动态根幂等身份 = 动态本体根固定幂等身份;
  L2结构幂等身份 因果链根幂等身份 = 因果链本体根固定幂等身份;
  friend bool operator==(const 四本体根生产初始化请求 &,
                         const 四本体根生产初始化请求 &) = default;
};

enum class 四本体根生产初始化状态 : std::uint8_t {
  已发布 = 1,
  精确重复 = 2,
  请求无效 = 3,
  已发布选择冲突 = 4,
  当前根组预读失败 = 5,
  存在根形成失败 = 6,
  特征根形成失败 = 7,
  动态根形成失败 = 8,
  因果链根形成失败 = 9,
  单根正式读回失败 = 10,
  最终根组读回失败 = 11,
  事实代次竞争 = 12,
  根读回不一致 = 13,
  资源失败 = 14,
  内部不一致 = 15
};

enum class 四本体根生产初始化来源 : std::uint8_t {
  首次形成 = 1,
  已发布重复消费 = 2
};

struct 四本体根发布材料 final {
  四本体根生产初始化请求 选择;
  L2概念本体根事实 存在根;
  L2概念本体根事实 特征根;
  L2概念本体根事实 动态根;
  L2概念本体根事实 因果链根;
  std::uint64_t 最终事实截止代次 = 0;
  friend bool operator==(const 四本体根发布材料 &,
                         const 四本体根发布材料 &) = default;
};

struct 四本体根漂移重读证据 final {
  L2概念本体根角色 角色 = L2概念本体根角色::存在;
  std::uint64_t 调用前已知事实截止代次 = 0;
  L2当前概念本体根组读取请求 请求;
  L2当前概念本体根组读取结果 结果;
  friend bool operator==(const 四本体根漂移重读证据 &,
                         const 四本体根漂移重读证据 &) = default;
};

struct 四本体根组读取调用证据 final {
  L2当前概念本体根组读取请求 请求;
  L2当前概念本体根组读取结果 结果;
  friend bool operator==(const 四本体根组读取调用证据 &,
                         const 四本体根组读取调用证据 &) = default;
};

struct 四本体根单根读取调用证据 final {
  L2当前概念本体根读取请求 请求;
  L2当前概念本体根读取结果 结果;
  friend bool operator==(const 四本体根单根读取调用证据 &,
                         const 四本体根单根读取调用证据 &) = default;
};

struct 四本体根建立调用证据 final {
  L2概念本体根建立请求 请求;
  L2概念本体根写入结果 结果;
  friend bool operator==(const 四本体根建立调用证据 &,
                         const 四本体根建立调用证据 &) = default;
};

struct 四本体根生产初始化结果 final {
  四本体根生产初始化状态 状态 = 四本体根生产初始化状态::请求无效;
  std::optional<四本体根生产初始化来源> 来源;
  std::optional<四本体根组读取调用证据> 当前根组预读;
  std::vector<四本体根漂移重读证据> 漂移根组重读;
  std::optional<L2概念本体根建立请求> 存在根进入时待重放请求;
  std::vector<四本体根建立调用证据> 存在根建立;
  std::optional<四本体根单根读取调用证据> 存在根读取;
  std::optional<L2概念本体根建立请求> 特征根进入时待重放请求;
  std::vector<四本体根建立调用证据> 特征根建立;
  std::optional<四本体根单根读取调用证据> 特征根读取;
  std::optional<L2概念本体根建立请求> 动态根进入时待重放请求;
  std::vector<四本体根建立调用证据> 动态根建立;
  std::optional<四本体根单根读取调用证据> 动态根读取;
  std::optional<L2概念本体根建立请求> 因果链根进入时待重放请求;
  std::vector<四本体根建立调用证据> 因果链根建立;
  std::optional<四本体根单根读取调用证据> 因果链根读取;
  std::optional<四本体根组读取调用证据> 最终根组读取;
  std::optional<四本体根发布材料> 已发布材料;

  bool 成功() const noexcept;
};

} // namespace 海中鱼巣
