module;

#include <cstdint>
#include <limits>
#include <map>
#include <mutex>
#include <new>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

export module 海中鱼巣.线程.投影.线程生命周期;

export {
#include "线程生命周期.数据.h"
}

namespace 海中鱼巣::线程生命周期投影内部 {

struct 投影键 final {
  std::uint64_t 运行代次 = 0;
  std::uint64_t 线程身份 = 0;

  friend bool operator<(const 投影键 &左, const 投影键 &右) noexcept {
    if (左.运行代次 != 右.运行代次)
      return 左.运行代次 < 右.运行代次;
    return 左.线程身份 < 右.线程身份;
  }

  friend bool operator==(const 投影键 &, const 投影键 &) = default;
};

struct 消息键 final {
  投影键 投影;
  std::uint64_t 事件序号 = 0;

  friend bool operator<(const 消息键 &左, const 消息键 &右) noexcept {
    if (左.投影 < 右.投影)
      return true;
    if (右.投影 < 左.投影)
      return false;
    return 左.事件序号 < 右.事件序号;
  }
};

using 消息账 = std::map<消息键, 线程生命周期消息>;

inline bool 投影有效(线程生命周期投影 投影) noexcept {
  switch (投影) {
  case 线程生命周期投影::创建:
  case 线程生命周期投影::启动中:
  case 线程生命周期投影::运行中:
  case 线程生命周期投影::等待:
  case 线程生命周期投影::暂停:
  case 线程生命周期投影::恢复:
  case 线程生命周期投影::停止请求中:
  case 线程生命周期投影::收尾中:
  case 线程生命周期投影::退出前:
  case 线程生命周期投影::已退出:
  case 线程生命周期投影::故障:
  case 线程生命周期投影::异常退出:
    return true;
  }
  return false;
}

inline bool 线程类别有效(项目线程类别 类别) noexcept {
  switch (类别) {
  case 项目线程类别::运行宿主:
  case 项目线程类别::任务管理:
  case 项目线程类别::任务工作:
  case 项目线程类别::缓存统计:
  case 项目线程类别::事件日志:
  case 项目线程类别::外设采样材料:
  case 项目线程类别::自我:
  case 项目线程类别::持久证据:
  case 项目线程类别::控制面板:
    return true;
  case 项目线程类别::未知:
    return false;
  }
  return false;
}

inline bool 线程模块有效(项目线程模块 模块) noexcept {
  switch (模块) {
  case 项目线程模块::运行宿主:
  case 项目线程模块::任务管理:
  case 项目线程模块::任务工作:
  case 项目线程模块::缓存统计:
  case 项目线程模块::事件日志:
  case 项目线程模块::外设采样材料:
  case 项目线程模块::自我治理:
  case 项目线程模块::持久证据:
  case 项目线程模块::控制面板:
    return true;
  case 项目线程模块::未知:
    return false;
  }
  return false;
}

inline bool 诊断引用类别有效(线程诊断引用类别 类别) noexcept {
  return 类别 == 线程诊断引用类别::任务 ||
         类别 == 线程诊断引用类别::工作项;
}

inline bool 终态(线程生命周期投影 投影) noexcept {
  return 投影 == 线程生命周期投影::已退出 ||
         投影 == 线程生命周期投影::异常退出;
}

inline bool 生命周期转换允许(线程生命周期投影 前一,
                               线程生命周期投影 当前) noexcept {
  switch (前一) {
  case 线程生命周期投影::创建:
    return 当前 == 线程生命周期投影::启动中 ||
           当前 == 线程生命周期投影::停止请求中 ||
           当前 == 线程生命周期投影::故障 ||
           当前 == 线程生命周期投影::异常退出;
  case 线程生命周期投影::启动中:
    return 当前 == 线程生命周期投影::运行中 ||
           当前 == 线程生命周期投影::等待 ||
           当前 == 线程生命周期投影::停止请求中 ||
           当前 == 线程生命周期投影::故障 ||
           当前 == 线程生命周期投影::异常退出;
  case 线程生命周期投影::运行中:
    return 当前 == 线程生命周期投影::等待 ||
           当前 == 线程生命周期投影::暂停 ||
           当前 == 线程生命周期投影::停止请求中 ||
           当前 == 线程生命周期投影::收尾中 ||
           当前 == 线程生命周期投影::故障 ||
           当前 == 线程生命周期投影::异常退出;
  case 线程生命周期投影::等待:
    return 当前 == 线程生命周期投影::运行中 ||
           当前 == 线程生命周期投影::暂停 ||
           当前 == 线程生命周期投影::停止请求中 ||
           当前 == 线程生命周期投影::收尾中 ||
           当前 == 线程生命周期投影::故障 ||
           当前 == 线程生命周期投影::异常退出;
  case 线程生命周期投影::暂停:
    return 当前 == 线程生命周期投影::恢复 ||
           当前 == 线程生命周期投影::停止请求中 ||
           当前 == 线程生命周期投影::故障 ||
           当前 == 线程生命周期投影::异常退出;
  case 线程生命周期投影::恢复:
    return 当前 == 线程生命周期投影::运行中 ||
           当前 == 线程生命周期投影::等待 ||
           当前 == 线程生命周期投影::停止请求中 ||
           当前 == 线程生命周期投影::故障 ||
           当前 == 线程生命周期投影::异常退出;
  case 线程生命周期投影::停止请求中:
    return 当前 == 线程生命周期投影::收尾中 ||
           当前 == 线程生命周期投影::退出前 ||
           当前 == 线程生命周期投影::已退出 ||
           当前 == 线程生命周期投影::故障 ||
           当前 == 线程生命周期投影::异常退出;
  case 线程生命周期投影::收尾中:
    return 当前 == 线程生命周期投影::退出前 ||
           当前 == 线程生命周期投影::已退出 ||
           当前 == 线程生命周期投影::故障 ||
           当前 == 线程生命周期投影::异常退出;
  case 线程生命周期投影::退出前:
    return 当前 == 线程生命周期投影::已退出 ||
           当前 == 线程生命周期投影::异常退出;
  case 线程生命周期投影::故障:
    return 当前 == 线程生命周期投影::停止请求中 ||
           当前 == 线程生命周期投影::收尾中 ||
           当前 == 线程生命周期投影::退出前 ||
           当前 == 线程生命周期投影::已退出 ||
           当前 == 线程生命周期投影::异常退出;
  case 线程生命周期投影::已退出:
  case 线程生命周期投影::异常退出:
    return false;
  }
  return false;
}

inline bool 配置有效(const 线程生命周期投影服务配置 &配置) noexcept {
  return 配置.合同版本 == 线程生命周期投影合同版本 &&
         配置.运行代次 != 0 && 配置.最大线程数量 != 0 &&
         配置.最大消息数量 >= 配置.最大线程数量 &&
         配置.最大线程数量 <= 线程生命周期最大线程配置上限 &&
         配置.最大消息数量 <= 线程生命周期最大消息配置上限;
}

inline bool 请求有效(const 线程生命周期发布请求 &请求) noexcept {
  if (请求.合同版本 != 线程生命周期投影合同版本 ||
      请求.消息版本 != 线程生命周期事件模式版本 ||
      请求.消息身份.运行代次 == 0 || 请求.消息身份.线程身份.值 == 0 ||
      请求.消息身份.事件序号 == 0 || !线程类别有效(请求.线程类别) ||
      !线程模块有效(请求.所属模块) || 请求.发生时间毫秒 == 0 ||
      !投影有效(请求.当前投影))
    return false;
  if (请求.系统线程身份 && *请求.系统线程身份 == 0)
    return false;
  if (请求.诊断引用 &&
      (!诊断引用类别有效(请求.诊断引用->引用类别) ||
       请求.诊断引用->引用身份 == 0))
    return false;
  if (请求.消息身份.事件序号 == 1)
    return 请求.当前投影 == 线程生命周期投影::创建 &&
           !请求.前一投影;
  return 请求.当前投影 != 线程生命周期投影::创建 &&
         请求.前一投影 && 投影有效(*请求.前一投影);
}

inline 投影键 形成投影键(const 线程生命周期消息身份 &身份) noexcept {
  return {身份.运行代次, 身份.线程身份.值};
}

inline 消息键 形成消息键(const 线程生命周期消息身份 &身份) noexcept {
  return {形成投影键(身份), 身份.事件序号};
}

inline bool 相邻历史一致(const 消息账 &账, const 消息键 &键,
                         const 线程生命周期发布请求 &请求) noexcept {
  if (键.事件序号 > 1) {
    const auto 前驱 = 账.find({键.投影, 键.事件序号 - 1});
    if (前驱 != 账.end() &&
        (!请求.前一投影 ||
         *请求.前一投影 != 前驱->second.发布请求.当前投影))
      return false;
  }
  if (键.事件序号 != std::numeric_limits<std::uint64_t>::max()) {
    const auto 后继 = 账.find({键.投影, 键.事件序号 + 1});
    if (后继 != 账.end() &&
        (!后继->second.发布请求.前一投影 ||
         *后继->second.发布请求.前一投影 != 请求.当前投影))
      return false;
  }
  return true;
}

inline bool 前置完整(const 消息账 &账, const 投影键 &投影,
                     std::uint64_t 当前序号) noexcept {
  auto 位置 = 账.lower_bound({投影, 1});
  std::uint64_t 期望序号 = 1;
  std::optional<线程生命周期投影> 前一;
  for (; 位置 != 账.end() && 位置->first.投影 == 投影 &&
         位置->first.事件序号 <= 当前序号;
       ++位置) {
    if (位置->first.事件序号 != 期望序号)
      return false;
    const auto &请求 = 位置->second.发布请求;
    if (期望序号 == 1) {
      if (请求.当前投影 != 线程生命周期投影::创建 || 请求.前一投影)
        return false;
    } else if (!请求.前一投影 || *请求.前一投影 != *前一) {
      return false;
    }
    前一 = 请求.当前投影;
    if (期望序号 == 当前序号)
      return true;
    ++期望序号;
  }
  return false;
}

inline 当前线程生命周期投影 形成当前投影(
    const 线程生命周期发布请求 &请求,
    std::optional<std::uint64_t> 系统线程身份) noexcept {
  return {线程生命周期投影合同版本,
          请求.消息身份.运行代次,
          请求.消息身份.线程身份,
          请求.线程类别,
          请求.所属模块,
          系统线程身份,
          {请求, 线程生命周期消息当前性::当前},
          请求.消息身份.事件序号 == 1,
          请求.消息身份.事件序号 != 1,
          终态(请求.当前投影)};
}

static_assert(
    std::is_nothrow_move_assignable_v<当前线程生命周期投影>);

} // namespace 海中鱼巣::线程生命周期投影内部

export namespace 海中鱼巣 {

class 线程生命周期发布端口 {
public:
  virtual ~线程生命周期发布端口() = default;
  virtual 线程生命周期发布结果 发布(
      const 线程生命周期发布请求 &请求) noexcept = 0;
};

class 线程生命周期投影读取端口 {
public:
  virtual ~线程生命周期投影读取端口() = default;
  virtual 当前线程投影读取结果 读取当前线程投影(
      const 当前线程投影读取请求 &请求) const noexcept = 0;
  virtual 当前线程投影组读取结果 读取当前线程投影组(
      const 当前线程投影组读取请求 &请求) const noexcept = 0;
};

class 线程生命周期投影服务 final : public 线程生命周期发布端口,
                                     public 线程生命周期投影读取端口 {
public:
  explicit 线程生命周期投影服务(
      线程生命周期投影服务配置 配置) noexcept
      : 配置_(配置) {}

  线程生命周期发布结果 发布(
      const 线程生命周期发布请求 &请求) noexcept override {
    using namespace 线程生命周期投影内部;
    if (!配置有效(配置_) || !请求有效(请求))
      return {线程生命周期发布状态::请求拒绝, std::nullopt,
              std::nullopt};
    if (请求.消息身份.运行代次 != 配置_.运行代次)
      return {线程生命周期发布状态::运行代次不匹配, std::nullopt,
              std::nullopt};
    if (请求.前一投影 &&
        !生命周期转换允许(*请求.前一投影, 请求.当前投影))
      return {线程生命周期发布状态::非法状态转换, std::nullopt,
              std::nullopt};
    try {
      std::unique_lock<std::mutex> 锁(锁_, std::try_to_lock);
      if (!锁.owns_lock())
        return {线程生命周期发布状态::投影暂不可用, std::nullopt,
                std::nullopt};

      const auto 消息身份 = 形成消息键(请求.消息身份);
      const auto 已有消息 = 消息_.find(消息身份);
      const auto 当前位置 = 当前_.find(消息身份.投影);
      if (已有消息 != 消息_.end()) {
        if (当前位置 == 当前_.end())
          return {线程生命周期发布状态::内部不一致, std::nullopt,
                  std::nullopt};
        if (已有消息->second.发布请求 == 请求)
          return {线程生命周期发布状态::精确重复, 已有消息->second,
                  当前位置->second};
        return {线程生命周期发布状态::消息身份冲突, std::nullopt,
                当前位置->second};
      }

      if (消息_.size() >= 配置_.最大消息数量 ||
          (当前位置 == 当前_.end() &&
           当前_.size() >= 配置_.最大线程数量))
        return {线程生命周期发布状态::容量已满, std::nullopt,
                当前位置 == 当前_.end()
                    ? std::optional<当前线程生命周期投影>{}
                    : std::optional<当前线程生命周期投影>{当前位置->second}};

      if (当前位置 != 当前_.end()) {
        auto &当前投影 = 当前位置->second;
        const auto 当前消息身份 =
            形成消息键(当前投影.当前消息.发布请求.消息身份);
        const auto 当前消息 = 消息_.find(当前消息身份);
        if (当前消息 == 消息_.end() ||
            当前消息->second != 当前投影.当前消息 ||
            当前消息->second.当前性 != 线程生命周期消息当前性::当前)
          return {线程生命周期发布状态::内部不一致, std::nullopt,
                  当前投影};
        if (请求.线程类别 != 当前投影.线程类别 ||
            请求.所属模块 != 当前投影.所属模块)
          return {线程生命周期发布状态::消息身份冲突, std::nullopt,
                  当前投影};
        if (当前投影.系统线程身份 && 请求.系统线程身份 &&
            当前投影.系统线程身份 != 请求.系统线程身份)
          return {线程生命周期发布状态::系统线程身份冲突,
                  std::nullopt, 当前投影};
        const auto 当前序号 = 当前消息身份.事件序号;
        if (请求.消息身份.事件序号 < 当前序号) {
          if (终态(请求.当前投影))
            return {线程生命周期发布状态::历史链冲突, std::nullopt,
                    当前投影};
          if (!相邻历史一致(消息_, 消息身份, 请求))
            return {线程生命周期发布状态::历史链冲突, std::nullopt,
                    当前投影};
          const 线程生命周期消息 迟到消息{
              请求, 线程生命周期消息当前性::历史};
          const auto [新位置, 已插入] = 消息_.emplace(消息身份, 迟到消息);
          if (!已插入)
            return {线程生命周期发布状态::内部不一致, std::nullopt,
                    当前投影};
          if (!当前投影.系统线程身份 && 请求.系统线程身份)
            当前投影.系统线程身份 = 请求.系统线程身份;
          当前投影.已见创建事件 =
              消息_.find({消息身份.投影, 1}) != 消息_.end();
          当前投影.前置不完整 =
              !前置完整(消息_, 消息身份.投影, 当前序号);
          return {线程生命周期发布状态::已接收迟到消息,
                  新位置->second, 当前投影};
        }
        if (请求.消息身份.事件序号 == 当前序号)
          return {线程生命周期发布状态::内部不一致, std::nullopt,
                  当前投影};
        if (当前投影.已进入终态)
          return {线程生命周期发布状态::终态回退, std::nullopt,
                  当前投影};
        const bool 与当前紧邻 =
            请求.消息身份.事件序号 == 当前序号 + 1;
        if (与当前紧邻 &&
            (!请求.前一投影 ||
             *请求.前一投影 != 当前投影.当前消息.发布请求.当前投影))
          return {线程生命周期发布状态::前一投影不一致, std::nullopt,
                  当前投影};
        if (!生命周期转换允许(*请求.前一投影, 请求.当前投影))
          return {线程生命周期发布状态::非法状态转换, std::nullopt,
                  当前投影};

        const auto 系统线程身份 = 请求.系统线程身份
                                        ? 请求.系统线程身份
                                        : 当前投影.系统线程身份;
        auto 新当前 = 形成当前投影(请求, 系统线程身份);
        const 线程生命周期消息 新消息{
            请求, 线程生命周期消息当前性::当前};
        const auto [新位置, 已插入] = 消息_.emplace(消息身份, 新消息);
        if (!已插入)
          return {线程生命周期发布状态::内部不一致, std::nullopt,
                  当前投影};
        当前消息->second.当前性 = 线程生命周期消息当前性::历史;
        新当前.已见创建事件 =
            消息_.find({消息身份.投影, 1}) != 消息_.end();
        新当前.前置不完整 =
            !前置完整(消息_, 消息身份.投影, 请求.消息身份.事件序号);
        当前投影 = std::move(新当前);
        return {线程生命周期发布状态::已发布, 新位置->second,
                当前投影};
      }

      auto 新当前 = 形成当前投影(请求, 请求.系统线程身份);
      const 线程生命周期消息 新消息{
          请求, 线程生命周期消息当前性::当前};
      const auto [新消息位置, 已插入消息] =
          消息_.emplace(消息身份, 新消息);
      if (!已插入消息)
        return {线程生命周期发布状态::内部不一致, std::nullopt,
                std::nullopt};
      try {
        const auto [新当前位置, 已插入当前] =
            当前_.emplace(消息身份.投影, 新当前);
        if (!已插入当前) {
          消息_.erase(新消息位置);
          return {线程生命周期发布状态::内部不一致, std::nullopt,
                  std::nullopt};
        }
        return {线程生命周期发布状态::已发布, 新消息位置->second,
                新当前位置->second};
      } catch (...) {
        消息_.erase(新消息位置);
        throw;
      }
    } catch (const std::bad_alloc &) {
      return {线程生命周期发布状态::资源失败, std::nullopt,
              std::nullopt};
    } catch (...) {
      return {线程生命周期发布状态::内部不一致, std::nullopt,
              std::nullopt};
    }
  }

  当前线程投影读取结果 读取当前线程投影(
      const 当前线程投影读取请求 &请求) const noexcept override {
    using namespace 线程生命周期投影内部;
    if (!配置有效(配置_) ||
        请求.合同版本 != 线程生命周期投影合同版本 ||
        请求.运行代次 == 0 || 请求.线程身份.值 == 0)
      return {当前线程投影读取状态::请求拒绝, std::nullopt};
    if (请求.运行代次 != 配置_.运行代次)
      return {当前线程投影读取状态::运行代次不匹配, std::nullopt};
    try {
      std::unique_lock<std::mutex> 锁(锁_, std::try_to_lock);
      if (!锁.owns_lock())
        return {当前线程投影读取状态::投影暂不可用, std::nullopt};
      const auto 位置 = 当前_.find({请求.运行代次, 请求.线程身份.值});
      if (位置 == 当前_.end())
        return {当前线程投影读取状态::未找到, std::nullopt};
      const auto 当前消息 =
          消息_.find(形成消息键(位置->second.当前消息.发布请求.消息身份));
      if (当前消息 == 消息_.end() ||
          当前消息->second != 位置->second.当前消息)
        return {当前线程投影读取状态::内部不一致, std::nullopt};
      return {当前线程投影读取状态::已读取, 位置->second};
    } catch (const std::bad_alloc &) {
      return {当前线程投影读取状态::资源失败, std::nullopt};
    } catch (...) {
      return {当前线程投影读取状态::内部不一致, std::nullopt};
    }
  }

  当前线程投影组读取结果 读取当前线程投影组(
      const 当前线程投影组读取请求 &请求) const noexcept override {
    using namespace 线程生命周期投影内部;
    if (!配置有效(配置_) ||
        请求.合同版本 != 线程生命周期投影合同版本 ||
        请求.运行代次 == 0)
      return {当前线程投影读取状态::请求拒绝, {}};
    if (请求.运行代次 != 配置_.运行代次)
      return {当前线程投影读取状态::运行代次不匹配, {}};
    try {
      std::unique_lock<std::mutex> 锁(锁_, std::try_to_lock);
      if (!锁.owns_lock())
        return {当前线程投影读取状态::投影暂不可用, {}};
      当前线程投影组读取结果 结果{当前线程投影读取状态::已读取,
                                      {}};
      结果.当前投影组.reserve(当前_.size());
      for (const auto &[键, 投影] : 当前_) {
        const auto 当前消息 =
            消息_.find(形成消息键(投影.当前消息.发布请求.消息身份));
        if (当前消息 == 消息_.end() || 当前消息->second != 投影.当前消息)
          return {当前线程投影读取状态::内部不一致, {}};
        结果.当前投影组.push_back(投影);
      }
      return 结果;
    } catch (const std::bad_alloc &) {
      return {当前线程投影读取状态::资源失败, {}};
    } catch (...) {
      return {当前线程投影读取状态::内部不一致, {}};
    }
  }

private:
  线程生命周期投影服务配置 配置_;
  mutable std::mutex 锁_;
  线程生命周期投影内部::消息账 消息_;
  std::map<线程生命周期投影内部::投影键, 当前线程生命周期投影>
      当前_;
};

bool 线程生命周期发布结果::已接受() const noexcept {
  return (状态 == 线程生命周期发布状态::已发布 ||
          状态 == 线程生命周期发布状态::精确重复 ||
          状态 == 线程生命周期发布状态::已接收迟到消息) &&
         消息 && 当前投影;
}

bool 线程生命周期发布结果::已成为当前() const noexcept {
  return 已接受() && 消息->当前性 == 线程生命周期消息当前性::当前 &&
         当前投影->当前消息 == *消息;
}

} // namespace 海中鱼巣
