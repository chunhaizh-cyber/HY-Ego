// 文件规则：本文件只承载概念安全删除调用期提交能力，不持久化业务事实或结构状态。
#pragma once

#include "../核心/句柄.h"
#include "../核心/结构事务接线.数据.h"

#include <cstdint>
#include <memory>
#include <utility>

namespace 海中鱼巣::领域 {
class 概念安全删除编排器;
}

namespace 海中鱼巣 {

enum class 概念安全删除提交阶段 : std::uint8_t {
    无效 = 0,
    关系 = 1,
    索引 = 2,
    主信息 = 3,
    节点 = 4,
    概念登记 = 5,
    统计缓存 = 6,
    活动快照 = 7,
    已完成 = 8
};

enum class 概念结构所有者类型 : std::uint8_t {
    未定义 = 0,
    存在服务 = 1,
    动态服务 = 2,
    二次特征服务 = 3,
    因果服务 = 4
};

struct 概念结构所有者清理请求 {
    节点句柄 目标;
    主信息句柄 主信息;
    概念结构所有者类型 所有者 = 概念结构所有者类型::未定义;
    bool 非根 = false;
    bool 已退役 = false;

    bool 完整() const {
        return 句柄有效(目标) && 句柄有效(主信息)
            && 所有者 >= 概念结构所有者类型::存在服务
            && 所有者 <= 概念结构所有者类型::因果服务
            && 非根 && 已退役;
    }
};

struct 概念结构所有者清理授权 {
    节点句柄 目标;
    主信息句柄 主信息;
    概念结构所有者类型 所有者 = 概念结构所有者类型::未定义;
    std::uint64_t 写集身份 = 0;

    bool 完整() const {
        return 句柄有效(目标) && 句柄有效(主信息)
            && 所有者 >= 概念结构所有者类型::存在服务
            && 所有者 <= 概念结构所有者类型::因果服务
            && 写集身份 != 0;
    }
};

class 概念安全删除提交会话 {
public:
    概念安全删除提交会话(const 概念安全删除提交会话&) = delete;
    概念安全删除提交会话& operator=(const 概念安全删除提交会话&) = delete;
    概念安全删除提交会话(概念安全删除提交会话&&) noexcept = default;
    概念安全删除提交会话& operator=(概念安全删除提交会话&&) noexcept = default;

    bool 有效() const {
        return 运行期状态_
            && 句柄有效(目标_)
            && 写集身份_ != 0
            && 阶段_ >= 概念安全删除提交阶段::关系
            && 阶段_ <= 概念安全删除提交阶段::已完成;
    }

    const std::shared_ptr<void>& 读取运行期状态() const { return 运行期状态_; }
    节点句柄 读取目标() const { return 目标_; }
    std::uint64_t 读取写集身份() const { return 写集身份_; }
    概念安全删除提交阶段 读取阶段() const { return 阶段_; }

private:
    friend class 领域::概念安全删除编排器;

    概念安全删除提交会话(
        std::shared_ptr<void> 运行期状态,
        节点句柄 目标,
        const 结构事务令牌& 令牌)
        : 运行期状态_(std::move(运行期状态)),
          目标_(目标),
          写集身份_(令牌.许可序号),
          阶段_(概念安全删除提交阶段::关系) {
    }

    void 推进到(概念安全删除提交阶段 下一阶段) {
        const auto 当前 = static_cast<std::uint8_t>(阶段_);
        const auto 下一 = static_cast<std::uint8_t>(下一阶段);
        if (下一 == 当前 + 1) {
            阶段_ = 下一阶段;
        }
    }

    std::shared_ptr<void> 运行期状态_;
    节点句柄 目标_;
    std::uint64_t 写集身份_ = 0;
    概念安全删除提交阶段 阶段_ = 概念安全删除提交阶段::无效;
};

}
