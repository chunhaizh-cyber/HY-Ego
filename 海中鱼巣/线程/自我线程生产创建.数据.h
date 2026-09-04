#pragma once

// 预冻结 BIZ 候选：真实自我形成 DTO 发布后必须机械匹配。
// 线程生命周期 DTO 与端口由通用线程投影模块唯一拥有；本文件不重复声明。
// 本文件不定义 DATA 事实，也不把候选源码视为已编译。
namespace 海中鱼巣 {

inline constexpr std::uint32_t 自我线程生产创建合同版本 = 3;

inline constexpr std::uint32_t 自我线程治理端口合同版本 = 1;

enum class 自我线程外部调用状态 : std::uint8_t {
  已形成 = 1,
  精确重复 = 2,
  合法等待 = 3,
  当前性漂移 = 4,
  引用冲突 = 5,
  不可比较 = 6,
  资源失败 = 7,
  入口拒绝 = 8,
  内部错误 = 9
};

struct 自我线程治理端口请求 final {
  std::uint32_t 合同版本 = 自我线程治理端口合同版本;
  std::uint64_t 运行代次 = 0;
  std::uint64_t 治理批次 = 0;
  std::uint64_t 治理消息编号 = 0;
  std::uint64_t 消息幂等键 = 0;
  std::uint64_t 共同事实截止G0 = 0;
  L2存在身份 唯一自我;
  L2特征定义身份 安全根特征定义;
  L2特征定义身份 服务根特征定义;
  std::optional<L2需求身份> 具体需求;
  std::optional<L2任务子目标承接记录身份> 子目标承接记录;
  std::optional<自我治理消息> 来源消息;
};

struct 自我线程治理端口结果 final {
  自我线程外部调用状态 状态 = 自我线程外部调用状态::入口拒绝;
  std::uint64_t 本次正式读回截止 = 0;
  std::optional<L2需求身份> 具体需求;
  std::optional<L2任务子目标承接记录身份> 子目标承接记录;

  bool 成功() const noexcept {
    return (状态 == 自我线程外部调用状态::已形成 ||
            状态 == 自我线程外部调用状态::精确重复) &&
           本次正式读回截止 != 0;
  }
};

class 自我线程需求治理端口 {
public:
  virtual ~自我线程需求治理端口() = default;
  virtual 自我线程治理端口结果 读取需求治理当前快照(
      const 自我线程治理端口请求 &) noexcept = 0;
  virtual 自我线程治理端口结果 确认后继目标(
      const 自我线程治理端口请求 &) noexcept = 0;
  virtual 自我线程治理端口结果 发布或复用已承接需求(
      const 自我线程治理端口请求 &) noexcept = 0;
};

class 自我线程任务管理提交端口 {
public:
  virtual ~自我线程任务管理提交端口() = default;
  virtual 自我线程治理端口结果 提交已确认子目标承接记录意图(
      const 自我线程治理端口请求 &) noexcept = 0;
  virtual 自我线程治理端口结果 提交任务承接意图(
      const 自我线程治理端口请求 &) noexcept = 0;
  virtual 自我线程治理端口结果 提交自我任务后继决议(
      const 自我线程治理端口请求 &) noexcept = 0;
};

class 自我线程安全治理端口 {
public:
  virtual ~自我线程安全治理端口() = default;
  virtual 自我线程治理端口结果 读取安全治理当前快照(
      const 自我线程治理端口请求 &) noexcept = 0;
  virtual 自我线程治理端口结果 请求执行安全治理(
      const 自我线程治理端口请求 &) noexcept = 0;
  virtual 自我线程治理端口结果 请求现实执行授权(
      const 自我线程治理端口请求 &) noexcept = 0;
};

enum class 自我线程治理端口绑定状态 : std::uint8_t {
  已绑定 = 1,
  精确重复 = 2,
  请求无效 = 3,
  选择冲突 = 4,
  已停止 = 5
};

struct 自我线程治理端口绑定结果 final {
  自我线程治理端口绑定状态 状态 =
      自我线程治理端口绑定状态::请求无效;
  bool 成功() const noexcept {
    return 状态 == 自我线程治理端口绑定状态::已绑定 ||
           状态 == 自我线程治理端口绑定状态::精确重复;
  }
};

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
  本能根运行锚点_v1 本能根锚点;
  std::uint64_t 治理邮箱容量 = 0;
  std::uint64_t 最大任务结果来源需求成员数 = 64;
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

// 自我治理循环退出结果（对齐 BIZ-L1-T02 v0.2 函数合同）
enum class 自我治理循环退出结果 : std::uint8_t {
  邮箱已停止 = 1,
  批次组合终止 = 2,
  一致事实终止 = 3,
  领域治理终止 = 4,
  下行提交终止 = 5,
  内部不一致 = 6,
  任务目标裁决已形成 = 8
};

enum class 任务结果治理处理槽状态 : std::uint8_t {
  待复核 = 1,
  后继已形成 = 2,
  合法等待 = 3,
  当前性终止 = 4,
  输入拒绝 = 5,
  数量预算不足 = 6,
  资源等待 = 7,
  内部终止 = 8,
  目标待裁决 = 9,
  不可比较终止 = 10,
  目标裁决已形成 = 11
};

struct 任务结果治理后继输入 final {
  std::uint64_t 治理批次身份 = 0;
  std::uint64_t 治理消息编号 = 0;
  std::uint64_t 治理输入幂等键 = 0;
  L2任务身份 任务;
  L2实例方法身份 实例方法;
  std::uint64_t 执行轮次 = 0;
  L2任务实际结果身份 任务实际结果;
  L2结构幂等身份 任务治理首态写入幂等身份;
  std::uint64_t 共享事实截止G0 = 0;
  治理输入事实复核状态 复核状态 = 治理输入事实复核状态::入口拒绝;
  friend bool operator==(const 任务结果治理后继输入&,
                         const 任务结果治理后继输入&) = default;
};

struct 任务结果治理处理槽快照 final {
  任务结果治理处理槽状态 状态 = 任务结果治理处理槽状态::待复核;
  std::uint64_t 治理批次身份 = 0;
  std::uint64_t 治理消息编号 = 0;
  std::uint64_t 治理输入幂等键 = 0;
  L2任务身份 任务;
  std::uint64_t 共享事实截止G0 = 0;
  std::uint64_t 复核次数 = 0;
  std::uint64_t 目标裁决次数 = 0;
  治理输入事实复核状态 复核状态 = 治理输入事实复核状态::入口拒绝;
  std::optional<任务结果治理后继输入> 后继输入;
  std::optional<任务目标达成裁决快照> 目标裁决;
  friend bool operator==(const 任务结果治理处理槽快照&,
                         const 任务结果治理处理槽快照&) = default;
};

// 治理批次处理阶段（对齐 BIZ-L1-T02 N28-N29 重试恢复分派）
enum class 治理批次处理阶段 : std::uint8_t {
  一致事实 = 1,
  安全维护 = 2,
  权限计算 = 3,
  输入分类 = 4,
  输入处理 = 5,
  父链重判 = 6,
  正差距确认 = 7,
  后继裁决 = 8,
  消息提交 = 9,
  已完成 = 10
};

} // namespace 海中鱼巣
