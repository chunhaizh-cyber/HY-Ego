#pragma once

// 预冻结 BIZ 候选：真实自我形成 DTO 发布后必须机械匹配。
// 线程生命周期 DTO 与端口由通用线程投影模块唯一拥有；本文件不重复声明。
// 本文件不定义 DATA 事实，也不把候选源码视为已编译。
namespace 海中鱼巣 {

inline constexpr std::uint32_t 自我线程生产创建合同版本 = 1;

struct 自我线程创建幂等身份 final {
  std::uint64_t 值 = 0;
  friend bool operator==(const 自我线程创建幂等身份 &,
                         const 自我线程创建幂等身份 &) = default;
};

inline constexpr 自我线程创建幂等身份 自我线程固定创建幂等身份{
    0x5345'4C46'5448'0001ULL};

struct 自我线程生产创建请求 final {
  std::uint32_t 合同版本 = 自我线程生产创建合同版本;
  自我线程创建幂等身份 创建幂等身份 = 自我线程固定创建幂等身份;
  线程逻辑身份 线程逻辑身份;
  std::uint64_t 运行代次 = 0;
  自我世界树根消费材料 根消费材料;
  std::uint64_t 治理邮箱容量 = 0;
  std::uint64_t 入口停门等待毫秒 = 0;
  std::uint64_t 失败回收诊断毫秒 = 0;
};

enum class 自我线程治理运行门状态 : std::uint8_t {
  关闭 = 1,
  开放 = 2
};

enum class 自我线程生产创建状态 : std::uint8_t {
  已创建并停门 = 1,
  精确重复 = 2,
  请求无效 = 3,
  已发布选择冲突 = 4,
  线程候选创建失败 = 5,
  线程入口未进入 = 6,
  治理运行门停驻失败 = 7,
  启动候选回收超时 = 8,
  资源失败 = 9,
  内部不一致 = 10
};

struct 自我线程停门见证 final {
  线程逻辑身份 线程逻辑身份;
  std::uint64_t 运行代次 = 0;
  std::uint64_t 入口见证序号 = 0;
  std::uint64_t 停门见证序号 = 0;
  自我线程治理运行门状态 运行门 = 自我线程治理运行门状态::关闭;
  自我治理邮箱快照 治理邮箱;
  friend bool operator==(const 自我线程停门见证 &,
                         const 自我线程停门见证 &) = default;
};

struct 自我线程生产创建结果 final {
  自我线程生产创建状态 状态 = 自我线程生产创建状态::请求无效;
  std::optional<自我线程停门见证> 停门见证;
  std::optional<线程生命周期发布结果> 创建投影;
  std::optional<线程生命周期发布结果> 入口投影;
  std::optional<线程生命周期发布结果> 停门投影;

  bool 成功() const noexcept;
};

enum class 自我线程启动候选回收状态 : std::uint8_t {
  已回收 = 1,
  无候选 = 2,
  等待完成超时 = 3,
  内部不一致 = 4
};

struct 自我线程启动候选回收结果 final {
  自我线程启动候选回收状态 状态 =
      自我线程启动候选回收状态::内部不一致;
};

} // namespace 海中鱼巣
