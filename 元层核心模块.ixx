module;

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <format>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

export module 元层核心模块;

import 基础数据类型模块;
import 日志模块;

export enum class 枚举_元实体类型 : std::uint8_t
{
    需求 = 0,
    任务 = 1,
    方法 = 2,
    反馈 = 3,
    经验 = 4,
    资源 = 5,
    规则 = 6
};

export enum class 枚举_元关系类型 : std::uint8_t
{
    产生 = 0,
    分解 = 1,
    绑定 = 2,
    依赖 = 3,
    替代 = 4,
    约束 = 5,
    反馈到 = 6,
    学习回写 = 7
};

export enum class 枚举_元对象状态 : std::uint8_t
{
    新建 = 0,
    待处理 = 1,
    待执行 = 2,
    执行中 = 3,
    已完成 = 4,
    已失败 = 5,
    已取消 = 6,
    已阻塞 = 7,
    已失效 = 8
};

export using 元字段表 = std::unordered_map<std::string, std::string>;
export using 元标签集 = std::unordered_set<std::string>;

export struct 结构_元实体
{
    std::string 标识;
    枚举_元实体类型 类型 = 枚举_元实体类型::任务;
    枚举_元对象状态 状态 = 枚举_元对象状态::新建;
    元字段表 内容;
    元标签集 标签;
    std::uint64_t 版本号 = 0;
    时间戳 创建时间 = 0;
    时间戳 更新时间 = 0;
    bool 是否失效 = false;
};

export struct 结构_元关系
{
    std::string 标识;
    std::string 起点标识;
    std::string 终点标识;
    枚举_元关系类型 类型 = 枚举_元关系类型::产生;
    double 权重 = 1.0;
    枚举_元对象状态 状态 = 枚举_元对象状态::新建;
    元字段表 内容;
    std::uint64_t 版本号 = 0;
    时间戳 创建时间 = 0;
    时间戳 更新时间 = 0;
    bool 是否失效 = false;
};

export struct 结构_元事件
{
    std::string 标识;
    std::string 事件类型;
    std::string 对象标识;
    元字段表 内容;
    时间戳 发生时间 = 0;
};

export inline 时间戳 元层当前时间() noexcept
{
    return 结构体_时间戳::当前_微秒();
}

export inline std::string 生成元层标识(std::string_view 前缀 = "meta")
{
    static std::mutex 计数锁;
    static std::uint64_t 计数 = 0;
    std::lock_guard<std::mutex> 锁(计数锁);
    return std::format("{}_{}_{}", 前缀, 元层当前时间(), ++计数);
}

export inline const char* 元实体类型文本(枚举_元实体类型 类型) noexcept
{
    switch (类型) {
    case 枚举_元实体类型::需求: return "需求";
    case 枚举_元实体类型::任务: return "任务";
    case 枚举_元实体类型::方法: return "方法";
    case 枚举_元实体类型::反馈: return "反馈";
    case 枚举_元实体类型::经验: return "经验";
    case 枚举_元实体类型::资源: return "资源";
    case 枚举_元实体类型::规则: return "规则";
    default: return "未知实体";
    }
}

export inline const char* 元关系类型文本(枚举_元关系类型 类型) noexcept
{
    switch (类型) {
    case 枚举_元关系类型::产生: return "产生";
    case 枚举_元关系类型::分解: return "分解";
    case 枚举_元关系类型::绑定: return "绑定";
    case 枚举_元关系类型::依赖: return "依赖";
    case 枚举_元关系类型::替代: return "替代";
    case 枚举_元关系类型::约束: return "约束";
    case 枚举_元关系类型::反馈到: return "反馈到";
    case 枚举_元关系类型::学习回写: return "学习回写";
    default: return "未知关系";
    }
}

export inline const char* 元对象状态文本(枚举_元对象状态 状态) noexcept
{
    switch (状态) {
    case 枚举_元对象状态::新建: return "新建";
    case 枚举_元对象状态::待处理: return "待处理";
    case 枚举_元对象状态::待执行: return "待执行";
    case 枚举_元对象状态::执行中: return "执行中";
    case 枚举_元对象状态::已完成: return "已完成";
    case 枚举_元对象状态::已失败: return "已失败";
    case 枚举_元对象状态::已取消: return "已取消";
    case 枚举_元对象状态::已阻塞: return "已阻塞";
    case 枚举_元对象状态::已失效: return "已失效";
    default: return "未知状态";
    }
}

namespace 元层核心模块_内部 {
    inline bool 允许状态迁移(枚举_元对象状态 旧状态, 枚举_元对象状态 新状态) noexcept
    {
        if (旧状态 == 新状态) return true;
        if (旧状态 == 枚举_元对象状态::已失效) return false;
        if (新状态 == 枚举_元对象状态::已失效) return true;

        switch (旧状态) {
        case 枚举_元对象状态::新建:
            return 新状态 == 枚举_元对象状态::待处理
                || 新状态 == 枚举_元对象状态::待执行
                || 新状态 == 枚举_元对象状态::已取消;
        case 枚举_元对象状态::待处理:
            return 新状态 == 枚举_元对象状态::待执行
                || 新状态 == 枚举_元对象状态::已阻塞
                || 新状态 == 枚举_元对象状态::已取消
                || 新状态 == 枚举_元对象状态::已失败;
        case 枚举_元对象状态::待执行:
            return 新状态 == 枚举_元对象状态::执行中
                || 新状态 == 枚举_元对象状态::已阻塞
                || 新状态 == 枚举_元对象状态::已取消
                || 新状态 == 枚举_元对象状态::已失败;
        case 枚举_元对象状态::执行中:
            return 新状态 == 枚举_元对象状态::已完成
                || 新状态 == 枚举_元对象状态::已失败
                || 新状态 == 枚举_元对象状态::已阻塞
                || 新状态 == 枚举_元对象状态::已取消;
        case 枚举_元对象状态::已阻塞:
            return 新状态 == 枚举_元对象状态::待处理
                || 新状态 == 枚举_元对象状态::待执行
                || 新状态 == 枚举_元对象状态::已取消
                || 新状态 == 枚举_元对象状态::已失败;
        case 枚举_元对象状态::已完成:
        case 枚举_元对象状态::已失败:
        case 枚举_元对象状态::已取消:
            return false;
        default:
            return false;
        }
    }
}

export class 元层接口
{
public:
    virtual ~元层接口() = default;

    virtual bool 写实体(const 结构_元实体& 实体对象) = 0;
    virtual std::optional<结构_元实体> 读实体(const std::string& 标识) const = 0;
    virtual std::vector<结构_元实体> 查询实体(枚举_元实体类型 类型, bool 包含失效 = false) const = 0;
    virtual bool 失效实体(const std::string& 标识, std::string_view 原因 = {}) = 0;

    virtual bool 写关系(const 结构_元关系& 关系对象) = 0;
    virtual std::optional<结构_元关系> 读关系(const std::string& 标识) const = 0;
    virtual std::vector<结构_元关系> 查询关系(
        const std::string& 起点标识,
        std::optional<枚举_元关系类型> 类型 = std::nullopt,
        bool 包含失效 = false) const = 0;
    virtual std::vector<结构_元关系> 查询关系_按终点(
        const std::string& 终点标识,
        std::optional<枚举_元关系类型> 类型 = std::nullopt,
        bool 包含失效 = false) const = 0;
    virtual std::vector<结构_元关系> 查询关系_双向(
        const std::string& 对象标识,
        std::optional<枚举_元关系类型> 类型 = std::nullopt,
        bool 包含失效 = false) const = 0;
    virtual bool 失效关系(const std::string& 标识, std::string_view 原因 = {}) = 0;

    virtual bool 迁移状态(const std::string& 对象标识, 枚举_元对象状态 新状态, std::string_view 原因 = {}) = 0;
    virtual bool 记录事件(const 结构_元事件& 事件对象) = 0;
    virtual std::vector<结构_元事件> 查询事件(std::string_view 对象标识 = {}, std::string_view 事件类型 = {}) const = 0;
};

export class 内存元层 final : public 元层接口
{
public:
    bool 写实体(const 结构_元实体& 实体对象) override
    {
        if (实体对象.标识.empty()) {
            日志::运行_错误("[元层核心] 写实体失败：标识为空");
            return false;
        }

        const 时间戳 now = 元层当前时间();
        std::lock_guard<std::mutex> 锁(互斥_);
        结构_元实体 待保存 = 实体对象;
        if (auto it = 实体仓库_.find(待保存.标识); it != 实体仓库_.end()) {
            if (待保存.创建时间 == 0) 待保存.创建时间 = it->second.创建时间;
            待保存.版本号 = it->second.版本号 + 1;
        }
        else {
            if (待保存.创建时间 == 0) 待保存.创建时间 = now;
            if (待保存.版本号 == 0) 待保存.版本号 = 1;
        }
        if (待保存.更新时间 == 0) 待保存.更新时间 = now;
        实体仓库_[待保存.标识] = 待保存;
        私有_附记系统事件_已加锁("写实体", 待保存.标识, {
            {"类型", 元实体类型文本(待保存.类型)},
            {"状态", 元对象状态文本(待保存.状态)},
            {"版本号", std::to_string(待保存.版本号)}
            });
        日志::运行f(
            "[元层核心] 写实体成功: id={}, type={}, state={}, version={}",
            待保存.标识,
            元实体类型文本(待保存.类型),
            元对象状态文本(待保存.状态),
            待保存.版本号);
        return true;
    }

    std::optional<结构_元实体> 读实体(const std::string& 标识) const override
    {
        std::lock_guard<std::mutex> 锁(互斥_);
        if (auto it = 实体仓库_.find(标识); it != 实体仓库_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    std::vector<结构_元实体> 查询实体(枚举_元实体类型 类型, bool 包含失效 = false) const override
    {
        std::vector<结构_元实体> 结果;
        std::lock_guard<std::mutex> 锁(互斥_);
        for (const auto& [_, 实体] : 实体仓库_) {
            if (实体.类型 != 类型) continue;
            if (!包含失效 && 实体.是否失效) continue;
            结果.push_back(实体);
        }
        return 结果;
    }

    bool 失效实体(const std::string& 标识, std::string_view 原因 = {}) override
    {
        const 时间戳 now = 元层当前时间();
        std::lock_guard<std::mutex> 锁(互斥_);
        auto it = 实体仓库_.find(标识);
        if (it == 实体仓库_.end()) {
            日志::运行_错误(std::format("[元层核心] 失效实体失败：未找到 id={}", 标识));
            return false;
        }
        auto& 实体 = it->second;
        实体.是否失效 = true;
        实体.状态 = 枚举_元对象状态::已失效;
        实体.更新时间 = now;
        ++实体.版本号;
        私有_附记系统事件_已加锁("失效实体", 标识, {
            {"原因", std::string(原因)},
            {"版本号", std::to_string(实体.版本号)}
            });
        日志::运行f("[元层核心] 实体已失效: id={}, reason={}", 标识, std::string(原因));
        return true;
    }

    bool 写关系(const 结构_元关系& 关系对象) override
    {
        if (关系对象.标识.empty() || 关系对象.起点标识.empty() || 关系对象.终点标识.empty()) {
            日志::运行_错误("[元层核心] 写关系失败：标识或端点为空");
            return false;
        }

        const 时间戳 now = 元层当前时间();
        std::lock_guard<std::mutex> 锁(互斥_);
        const auto 校验端点_ = [&](const std::string& 端点标识, std::string_view 端点名称) -> bool {
            const auto it = 实体仓库_.find(端点标识);
            if (it == 实体仓库_.end()) {
                日志::运行_错误(std::format(
                    "[元层核心] 写关系失败：{}不存在 id={}",
                    std::string(端点名称),
                    端点标识));
                return false;
            }
            if (it->second.是否失效) {
                日志::运行_错误(std::format(
                    "[元层核心] 写关系失败：{}已失效 id={}",
                    std::string(端点名称),
                    端点标识));
                return false;
            }
            return true;
        };
        if (!校验端点_(关系对象.起点标识, "起点") || !校验端点_(关系对象.终点标识, "终点")) {
            return false;
        }

        结构_元关系 待保存 = 关系对象;
        if (auto it = 关系仓库_.find(待保存.标识); it != 关系仓库_.end()) {
            if (待保存.创建时间 == 0) 待保存.创建时间 = it->second.创建时间;
            待保存.版本号 = it->second.版本号 + 1;
        }
        else {
            if (待保存.创建时间 == 0) 待保存.创建时间 = now;
            if (待保存.版本号 == 0) 待保存.版本号 = 1;
        }
        if (待保存.更新时间 == 0) 待保存.更新时间 = now;
        关系仓库_[待保存.标识] = 待保存;
        私有_附记系统事件_已加锁("写关系", 待保存.标识, {
            {"类型", 元关系类型文本(待保存.类型)},
            {"起点", 待保存.起点标识},
            {"终点", 待保存.终点标识},
            {"版本号", std::to_string(待保存.版本号)}
            });
        日志::运行f(
            "[元层核心] 写关系成功: id={}, type={}, from={}, to={}, version={}",
            待保存.标识,
            元关系类型文本(待保存.类型),
            待保存.起点标识,
            待保存.终点标识,
            待保存.版本号);
        return true;
    }

    std::optional<结构_元关系> 读关系(const std::string& 标识) const override
    {
        std::lock_guard<std::mutex> 锁(互斥_);
        if (auto it = 关系仓库_.find(标识); it != 关系仓库_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    std::vector<结构_元关系> 查询关系(
        const std::string& 起点标识,
        std::optional<枚举_元关系类型> 类型 = std::nullopt,
        bool 包含失效 = false) const override
    {
        std::vector<结构_元关系> 结果;
        std::lock_guard<std::mutex> 锁(互斥_);
        for (const auto& [_, 关系] : 关系仓库_) {
            if (关系.起点标识 != 起点标识) continue;
            if (类型.has_value() && 关系.类型 != *类型) continue;
            if (!包含失效 && 关系.是否失效) continue;
            结果.push_back(关系);
        }
        return 结果;
    }

    std::vector<结构_元关系> 查询关系_按终点(
        const std::string& 终点标识,
        std::optional<枚举_元关系类型> 类型 = std::nullopt,
        bool 包含失效 = false) const override
    {
        std::vector<结构_元关系> 结果;
        std::lock_guard<std::mutex> 锁(互斥_);
        for (const auto& [_, 关系] : 关系仓库_) {
            if (关系.终点标识 != 终点标识) continue;
            if (类型.has_value() && 关系.类型 != *类型) continue;
            if (!包含失效 && 关系.是否失效) continue;
            结果.push_back(关系);
        }
        return 结果;
    }

    std::vector<结构_元关系> 查询关系_双向(
        const std::string& 对象标识,
        std::optional<枚举_元关系类型> 类型 = std::nullopt,
        bool 包含失效 = false) const override
    {
        std::vector<结构_元关系> 结果;
        std::lock_guard<std::mutex> 锁(互斥_);
        for (const auto& [_, 关系] : 关系仓库_) {
            if (关系.起点标识 != 对象标识 && 关系.终点标识 != 对象标识) continue;
            if (类型.has_value() && 关系.类型 != *类型) continue;
            if (!包含失效 && 关系.是否失效) continue;
            结果.push_back(关系);
        }
        return 结果;
    }

    bool 失效关系(const std::string& 标识, std::string_view 原因 = {}) override
    {
        const 时间戳 now = 元层当前时间();
        std::lock_guard<std::mutex> 锁(互斥_);
        auto it = 关系仓库_.find(标识);
        if (it == 关系仓库_.end()) {
            日志::运行_错误(std::format("[元层核心] 失效关系失败：未找到 id={}", 标识));
            return false;
        }
        auto& 关系 = it->second;
        关系.是否失效 = true;
        关系.状态 = 枚举_元对象状态::已失效;
        关系.更新时间 = now;
        ++关系.版本号;
        私有_附记系统事件_已加锁("失效关系", 标识, {
            {"原因", std::string(原因)},
            {"版本号", std::to_string(关系.版本号)}
            });
        日志::运行f("[元层核心] 关系已失效: id={}, reason={}", 标识, std::string(原因));
        return true;
    }

    bool 迁移状态(const std::string& 对象标识, 枚举_元对象状态 新状态, std::string_view 原因 = {}) override
    {
        const 时间戳 now = 元层当前时间();
        std::lock_guard<std::mutex> 锁(互斥_);
        if (auto it = 实体仓库_.find(对象标识); it != 实体仓库_.end()) {
            return 私有_迁移实体状态_已加锁(it->second, 新状态, 原因, now);
        }
        if (auto it = 关系仓库_.find(对象标识); it != 关系仓库_.end()) {
            return 私有_迁移关系状态_已加锁(it->second, 新状态, 原因, now);
        }
        日志::运行_错误(std::format("[元层核心] 状态迁移失败：未找到对象 id={}", 对象标识));
        return false;
    }

    bool 记录事件(const 结构_元事件& 事件对象) override
    {
        if (事件对象.事件类型.empty()) {
            日志::运行_错误("[元层核心] 记录事件失败：事件类型为空");
            return false;
        }

        std::lock_guard<std::mutex> 锁(互斥_);
        结构_元事件 待记录 = 事件对象;
        if (待记录.标识.empty()) {
            待记录.标识 = 生成元层标识("event");
        }
        if (待记录.发生时间 == 0) {
            待记录.发生时间 = 元层当前时间();
        }
        if (事件仓库_.contains(待记录.标识)) {
            日志::运行_错误(std::format("[元层核心] 记录事件失败：重复事件 id={}", 待记录.标识));
            return false;
        }
        事件仓库_[待记录.标识] = 待记录;
        日志::运行f(
            "[元层核心] 记录事件成功: id={}, type={}, object={}",
            待记录.标识,
            待记录.事件类型,
            待记录.对象标识);
        return true;
    }

    std::vector<结构_元事件> 查询事件(std::string_view 对象标识 = {}, std::string_view 事件类型 = {}) const override
    {
        std::vector<结构_元事件> 结果;
        std::lock_guard<std::mutex> 锁(互斥_);
        for (const auto& [_, 事件] : 事件仓库_) {
            if (!对象标识.empty() && 事件.对象标识 != 对象标识) continue;
            if (!事件类型.empty() && 事件.事件类型 != 事件类型) continue;
            结果.push_back(事件);
        }
        return 结果;
    }

    std::size_t 实体数量() const
    {
        std::lock_guard<std::mutex> 锁(互斥_);
        return 实体仓库_.size();
    }

    std::size_t 关系数量() const
    {
        std::lock_guard<std::mutex> 锁(互斥_);
        return 关系仓库_.size();
    }

    std::size_t 事件数量() const
    {
        std::lock_guard<std::mutex> 锁(互斥_);
        return 事件仓库_.size();
    }

private:
    bool 私有_迁移实体状态_已加锁(
        结构_元实体& 实体,
        枚举_元对象状态 新状态,
        std::string_view 原因,
        时间戳 now)
    {
        if (!元层核心模块_内部::允许状态迁移(实体.状态, 新状态)) {
            日志::运行_错误(std::format(
                "[元层核心] 非法实体状态迁移: id={}, from={}, to={}",
                实体.标识,
                元对象状态文本(实体.状态),
                元对象状态文本(新状态)));
            return false;
        }

        const auto 旧状态 = 实体.状态;
        实体.状态 = 新状态;
        实体.更新时间 = now;
        ++实体.版本号;
        私有_附记系统事件_已加锁("实体状态迁移", 实体.标识, {
            {"从", 元对象状态文本(旧状态)},
            {"到", 元对象状态文本(新状态)},
            {"原因", std::string(原因)}
            });
        日志::运行f(
            "[元层核心] 实体状态迁移: id={}, from={}, to={}, reason={}",
            实体.标识,
            元对象状态文本(旧状态),
            元对象状态文本(新状态),
            std::string(原因));
        return true;
    }

    bool 私有_迁移关系状态_已加锁(
        结构_元关系& 关系,
        枚举_元对象状态 新状态,
        std::string_view 原因,
        时间戳 now)
    {
        if (!元层核心模块_内部::允许状态迁移(关系.状态, 新状态)) {
            日志::运行_错误(std::format(
                "[元层核心] 非法关系状态迁移: id={}, from={}, to={}",
                关系.标识,
                元对象状态文本(关系.状态),
                元对象状态文本(新状态)));
            return false;
        }

        const auto 旧状态 = 关系.状态;
        关系.状态 = 新状态;
        关系.更新时间 = now;
        ++关系.版本号;
        私有_附记系统事件_已加锁("关系状态迁移", 关系.标识, {
            {"从", 元对象状态文本(旧状态)},
            {"到", 元对象状态文本(新状态)},
            {"原因", std::string(原因)}
            });
        日志::运行f(
            "[元层核心] 关系状态迁移: id={}, from={}, to={}, reason={}",
            关系.标识,
            元对象状态文本(旧状态),
            元对象状态文本(新状态),
            std::string(原因));
        return true;
    }

    void 私有_附记系统事件_已加锁(
        std::string_view 事件类型,
        const std::string& 对象标识,
        元字段表 内容)
    {
        结构_元事件 事件{};
        事件.标识 = 生成元层标识("audit");
        事件.事件类型 = std::string(事件类型);
        事件.对象标识 = 对象标识;
        事件.内容 = std::move(内容);
        事件.发生时间 = 元层当前时间();
        事件仓库_[事件.标识] = std::move(事件);
    }

    mutable std::mutex 互斥_;
    std::unordered_map<std::string, 结构_元实体> 实体仓库_;
    std::unordered_map<std::string, 结构_元关系> 关系仓库_;
    std::unordered_map<std::string, 结构_元事件> 事件仓库_;
};

export struct 结构_元层贯通演示结果
{
    bool 成功 = false;
    std::string 错误摘要;
    std::string 需求标识;
    std::string 任务标识;
    std::string 方法标识;
    std::string 反馈标识;
    std::string 经验标识;
    std::string 学习事件标识;
    std::size_t 实体数 = 0;
    std::size_t 关系数 = 0;
    std::size_t 事件数 = 0;
};

export inline 结构_元层贯通演示结果 运行元层核心_v0_贯通演示()
{
    结构_元层贯通演示结果 输出{};
    内存元层 元层{};

    结构_元实体 需求{};
    需求.标识 = 生成元层标识("need");
    需求.类型 = 枚举_元实体类型::需求;
    需求.状态 = 枚举_元对象状态::待处理;
    需求.内容 = {
        {"目标", "保持服务连续并收集外界信息"},
        {"满足条件", "存在可执行任务链"}
    };

    结构_元实体 任务{};
    任务.标识 = 生成元层标识("task");
    任务.类型 = 枚举_元实体类型::任务;
    任务.状态 = 枚举_元对象状态::待处理;
    任务.内容 = {
        {"说明", "处理一条来自外界的人类输入"},
        {"执行者", "自我线程"}
    };

    结构_元实体 方法{};
    方法.标识 = 生成元层标识("method");
    方法.类型 = 枚举_元实体类型::方法;
    方法.状态 = 枚举_元对象状态::待处理;
    方法.内容 = {
        {"动作", "确认收到"},
        {"风险", "低"}
    };

    结构_元实体 反馈{};
    反馈.标识 = 生成元层标识("feedback");
    反馈.类型 = 枚举_元实体类型::反馈;
    反馈.状态 = 枚举_元对象状态::已完成;
    反馈.内容 = {
        {"结果", "用户确认回应有效"},
        {"服务变化", "+1"}
    };

    结构_元实体 经验{};
    经验.标识 = 生成元层标识("exp");
    经验.类型 = 枚举_元实体类型::经验;
    经验.状态 = 枚举_元对象状态::已完成;
    经验.内容 = {
        {"摘要", "确认收到方法在当前任务上下文中有效"},
        {"写回方向", "提升任务-方法适配置信"}
    };

    const auto 写关系_ = [&](std::string_view 前缀, const std::string& 起点, const std::string& 终点, 枚举_元关系类型 类型, const 元字段表& 内容 = {}) {
        结构_元关系 关系{};
        关系.标识 = 生成元层标识(前缀);
        关系.起点标识 = 起点;
        关系.终点标识 = 终点;
        关系.类型 = 类型;
        关系.状态 = 枚举_元对象状态::待处理;
        关系.内容 = 内容;
        return 元层.写关系(关系);
    };

    if (!元层.写实体(需求)
        || !元层.写实体(任务)
        || !元层.写实体(方法)
        || !写关系_("rel", 需求.标识, 任务.标识, 枚举_元关系类型::产生, { {"说明", "需求触发执行任务"} })
        || !写关系_("rel", 任务.标识, 方法.标识, 枚举_元关系类型::绑定, { {"说明", "任务绑定确认收到方法"} })
        || !元层.迁移状态(任务.标识, 枚举_元对象状态::待执行, "元层演示：任务进入待执行")
        || !元层.迁移状态(任务.标识, 枚举_元对象状态::执行中, "开始执行演示任务")
        || !元层.迁移状态(任务.标识, 枚举_元对象状态::已完成, "演示任务执行完成")
        || !元层.写实体(反馈)
        || !写关系_("rel", 反馈.标识, 任务.标识, 枚举_元关系类型::反馈到, { {"说明", "反馈回到任务"} })
        || !写关系_("rel", 反馈.标识, 方法.标识, 枚举_元关系类型::反馈到, { {"说明", "反馈回到方法"} })) {
        输出.错误摘要 = "需求到反馈阶段写入失败";
        return 输出;
    }

    if (!元层.写实体(经验)
        || !写关系_("rel", 经验.标识, 任务.标识, 枚举_元关系类型::学习回写, { {"目标", "任务"} })
        || !写关系_("rel", 经验.标识, 方法.标识, 枚举_元关系类型::学习回写, { {"目标", "方法"} })) {
        输出.错误摘要 = "学习写回阶段失败";
        return 输出;
    }

    结构_元事件 学习事件{};
    学习事件.标识 = 生成元层标识("event");
    学习事件.事件类型 = "学习写回";
    学习事件.对象标识 = 经验.标识;
    学习事件.内容 = {
        {"来源反馈", 反馈.标识},
        {"写回任务", 任务.标识},
        {"写回方法", 方法.标识}
    };
    if (!元层.记录事件(学习事件)) {
        输出.错误摘要 = "记录学习写回事件失败";
        return 输出;
    }

    输出.成功 = true;
    输出.需求标识 = 需求.标识;
    输出.任务标识 = 任务.标识;
    输出.方法标识 = 方法.标识;
    输出.反馈标识 = 反馈.标识;
    输出.经验标识 = 经验.标识;
    输出.学习事件标识 = 学习事件.标识;
    输出.实体数 = 元层.实体数量();
    输出.关系数 = 元层.关系数量();
    输出.事件数 = 元层.事件数量();
    日志::运行f(
        "[元层核心] v0贯通演示完成: entities={}, relations={}, events={}",
        输出.实体数,
        输出.关系数,
        输出.事件数);
    return 输出;
}
