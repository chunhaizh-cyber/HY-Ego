#pragma once
#include <cstdint>
#include <optional>
#include <utility>
#include <vector>
#include "../核心/基础数据集.h"
#include "新_方法公共类型.h"

namespace 海中鱼巣 {
enum class 动作能力定义类别 : std::int64_t { 方法能力=1, 内部治理能力=2 };
enum class 动作参数对象类别 : std::int64_t {
    普通世界=1, 任务虚拟存在=2, 服务要求来源人类=3,
    服务要求目标存在=4, 服务要求目标特征类型=5,
    服务要求目标定义=6, 服务要求确认=7,
    方法虚拟存在=8  // METHOD-LEARN-STATE（7190）：方法状态变化动作的作用对象。
};
struct 动作参数特征规格 {
    稳定编码 特征类型概念节点;
    std::optional<稳定编码> 值域概念节点;
    std::vector<稳定编码> 二次特征概念组;
    friend bool operator==(const 动作参数特征规格&, const 动作参数特征规格&)=default;
};
struct 动作参数规格 {
    稳定编码 参数角色节点;
    std::optional<稳定编码> 存在概念节点;
    bool 要求场景=false;
    std::vector<动作参数特征规格> 特征规格组;
    动作参数对象类别 对象类别=动作参数对象类别::普通世界;
    // 有序规格与旧角色/嵌套规格互斥；参数位置不是稳定节点编码。
    std::optional<新方法有序参数规格> 有序规格;
    friend bool operator==(const 动作参数规格&, const 动作参数规格&)=default;
};
struct 动作概念角色 {
    std::optional<稳定编码> 存在概念节点;
    std::optional<稳定编码> 参数角色节点;
    动作参数对象类别 对象类别=动作参数对象类别::普通世界;
    friend bool operator==(const 动作概念角色&, const 动作概念角色&)=default;
};
enum class 新信息关系治理操作 : std::int64_t {
    自我采用正式请求=1, 管理关联唯一任务=2
};
struct 新信息关系治理动作规格 final {
    新信息关系治理操作 操作{};
    稳定编码 结果关系概念;
    std::vector<std::pair<std::int64_t,稳定编码>> 角色范围组;
    std::vector<std::int64_t> 结果角色组;
    friend bool operator==(const 新信息关系治理动作规格&,
        const 新信息关系治理动作规格&)=default;
};
struct 动作概念定义 {
    稳定编码 能力定义节点;
    动作能力定义类别 能力类别 = 动作能力定义类别::方法能力;
    std::optional<稳定编码> 抽象方法节点;
    动作概念角色 执行主体;
    动作概念角色 作用目标;
    std::optional<稳定编码> 目标特征类型概念节点;
    std::vector<动作参数规格> 参数规格组;
    std::optional<新信息关系治理动作规格> 信息关系规格;
    friend bool operator==(const 动作概念定义&, const 动作概念定义&)=default;
};
struct 动作方法投影请求 {
    稳定编码 动作定义节点;
    std::optional<稳定编码> 执行主体概念节点;
    std::optional<稳定编码> 普通目标概念节点;
    std::optional<稳定编码> 目标特征类型概念节点;
    bool 任务虚拟目标=false;
    bool 方法虚拟目标=false;
    friend bool operator==(const 动作方法投影请求&, const 动作方法投影请求&)=default;
};
class 动作方法定义只读提供者 {
public:
    virtual ~动作方法定义只读提供者()=default;
    virtual 动作概念定义 读取动作方法投影(const 动作方法投影请求&) const=0;
    // 动态持锁读回专用：只读不可变治理定义，不取得task锁或调用完整方法投影。
    virtual bool 核验普通治理动作定义(const 动作概念定义&) const=0;
};
// 动态持锁时也可调用：仅查询已发表的不可变定义目录，不取得领域锁。
class 关系治理动作只读提供者 {
public:
    virtual ~关系治理动作只读提供者()=default;
    virtual std::optional<动作概念定义> 读取关系治理动作定义(稳定编码 治理定义) const=0;
};
}
