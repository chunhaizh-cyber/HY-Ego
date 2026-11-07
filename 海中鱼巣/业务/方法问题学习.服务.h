#pragma once
#include <map>
#include <mutex>
#include <string>
#include "模拟学习.服务.h"
#include "../领域/方法问题反馈.数据.h"
#include "../领域/新_方法公共类型.h"

namespace 海中鱼巣 {
class 新_方法类;
class 方法学习评审服务;

// TRIAL-LEARNING-REVIEW §7：收集与处理分账；待处理可重试，其余为处理已结束。
enum class 新方法问题处理状态 : std::uint8_t {
    待处理 = 1, 已发布, 评审拒绝, 前提已变化, 缺口, 逻辑错误
};
struct 新方法问题学习收集结果 final {
    新方法问题学习消息 消息;
    std::shared_ptr<const 新方法问题反馈材料> 原始材料;
    bool 材料完整 = false;
    新方法问题处理状态 处理状态 = 新方法问题处理状态::待处理;
    std::wstring 处理原因;
};

// TRIAL-LEARNING-REVIEW §4：只读导出“实际条件→实际结果”候选；学习与独立评审共用。
enum class 新尝试补全导出种类 : std::uint8_t { 候选 = 1, 缺口, 逻辑错误 };
struct 新尝试补全导出结果 final {
    新尝试补全导出种类 种类 = 新尝试补全导出种类::缺口;
    std::optional<新尝试补全候选> 候选;
    std::wstring 原因;
};
新尝试补全导出结果 形成尝试补全候选(const 新方法问题反馈材料& 材料,
    稳定编码 归档节点, const 新方法信息& 当前方法, const 新_方法类& 方法读取);

// 收集不可变原始问题样本；尝试执行的空结果配对按 TRIAL-LEARNING-REVIEW 形成候选、经独立评审、由方法owner发布。
class 方法问题学习服务 final {
public:
    方法问题学习服务(const 新方法问题归档接口&, 新_方法类&, const 方法学习评审服务&) noexcept;
    新学习结束通知 学习(const 新方法问题学习消息&,
        const std::function<bool()>& 停止检查 = {});
    std::optional<新方法问题学习收集结果> 读取收集结果(
        稳定编码 归档节点) const noexcept;
private:
    std::pair<新方法问题处理状态, std::wstring> 处理尝试补全(
        const 新方法问题学习消息&, const 新方法问题反馈材料&);
    const 新方法问题归档接口& 归档_;
    新_方法类& 方法_;
    const 方法学习评审服务& 评审_;
    mutable std::mutex 互斥_;
    std::map<稳定编码, 新方法问题学习收集结果> 收集组_;
};
}
