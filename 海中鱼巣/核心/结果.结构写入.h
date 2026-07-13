// 文件规则：结构写入结果只表达仓库通用提交状态和值式载荷；不得承载领域业务语义。
#pragma once

#include <cstdint>
#include <optional>

namespace 海中鱼巣 {

enum class 结构写入状态 : std::uint32_t {
    已提交 = 1,
    幂等读回 = 2,
    入口拒绝 = 3,
    许可拒绝 = 4,
    版本漂移 = 5,
    候选已撤销 = 6,
    候选已确认 = 7,
    内部不一致 = 8
};

struct 结构写入结果 {
    结构写入状态 状态 = 结构写入状态::入口拒绝;
    std::uint64_t 结构编号 = 0;
    std::uint32_t 请求版本 = 0;
    std::uint32_t 当前版本 = 0;

    bool 成功() const noexcept {
        return 状态 == 结构写入状态::已提交
            || 状态 == 结构写入状态::幂等读回
            || 状态 == 结构写入状态::候选已撤销
            || 状态 == 结构写入状态::候选已确认;
    }

    bool 改变了结构() const noexcept {
        return 状态 == 结构写入状态::已提交;
    }
};

template <typename 值类型>
struct 带值结构写入结果 {
    结构写入结果 操作;
    std::optional<值类型> 值;

    bool 成功() const noexcept {
        return 操作.成功() && 值.has_value();
    }
};

}
