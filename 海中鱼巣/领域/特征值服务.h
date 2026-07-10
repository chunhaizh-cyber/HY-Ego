// 文件规则：特征值服务承载值节点和运行期原始值；值裁决不得写入日志或显示文本。
#pragma once

#include "../核心/节点仓库.h"
#include "../核心/主信息仓库.h"
#include "../核心/容错检查.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <vector>

namespace 海中鱼巣 {

enum class 特征值原始类型 : std::uint32_t {
    未建立 = 0,
    I64 = 1,
    VecI64 = 2,
    VecU64 = 3
};

class 特征值服务 {
public:
    static constexpr std::size_t 最大序列元素数量 = 4096;

    特征值服务(主信息仓库& 主信息, 节点仓库& 节点)
        : 主信息_(主信息), 节点_(节点) {
    }

    节点句柄 创建特征值() {
        return 节点_.创建节点(节点类型::特征值, 主信息_.创建主信息());
    }

    bool 写入I64值(节点句柄 特征值节点, std::int64_t 值) {
        const auto 节点记录值 = 读取有效特征值节点(特征值节点);
        if (!节点记录值.has_value()) {
            return false;
        }

        std::unique_lock<std::shared_mutex> 锁(原始值锁_);
        const auto Vec记录数量 = 计算Vec记录数量_已加锁(特征值节点);
        if (!追根因检查(Vec记录数量 <= 1, L"写入 I64 特征值时发现重复 Vec 原始值记录。")) {
            return false;
        }
        const auto Vec位置 = 查找Vec记录_已加锁(特征值节点);
        if (Vec位置 != Vec记录表_.end()) {
            if (!追根因检查(Vec记录内部一致(*Vec位置), L"写入 I64 特征值时已有 Vec 原始值记录内部不一致。")) {
                return false;
            }
            return false;
        }
        if (!追根因检查(主信息_.写入I64值(节点记录值->主信息, 值), L"写入 I64 特征值时主信息写入不及预期。")) {
            return false;
        }
        const auto 读回值 = 主信息_.读取I64值(节点记录值->主信息);
        return 追根因检查(读回值.has_value() && 读回值.value() == 值, L"写入 I64 特征值后读回不及预期。");
    }

    std::optional<std::int64_t> 读取I64值(节点句柄 特征值节点) const {
        const auto 节点记录值 = 读取有效特征值节点(特征值节点);
        if (!节点记录值.has_value()) {
            return std::nullopt;
        }

        std::shared_lock<std::shared_mutex> 锁(原始值锁_);
        const auto Vec记录数量 = 计算Vec记录数量_已加锁(特征值节点);
        if (!追根因检查(Vec记录数量 <= 1, L"读取 I64 特征值时发现重复 Vec 原始值记录。")) {
            return std::nullopt;
        }
        const auto Vec位置 = 查找Vec记录_已加锁(特征值节点);
        if (Vec位置 != Vec记录表_.end()) {
            if (!追根因检查(Vec记录内部一致(*Vec位置), L"读取 I64 特征值时已有 Vec 原始值记录内部不一致。")) {
                return std::nullopt;
            }
            return std::nullopt;
        }
        return 主信息_.读取I64值(节点记录值->主信息);
    }

    bool 写入VecI64值(节点句柄 特征值节点, const std::vector<std::int64_t>& 值) {
        const auto 节点记录值 = 读取有效特征值节点(特征值节点);
        if (!节点记录值.has_value() || !序列元素数量有效(值.size())) {
            return false;
        }

        Vec原始值记录 新记录;
        新记录.特征值节点 = 特征值节点;
        新记录.类型 = 特征值原始类型::VecI64;
        新记录.VecI64值 = 值;

        std::unique_lock<std::shared_mutex> 锁(原始值锁_);
        const auto I64值 = 主信息_.读取I64值(节点记录值->主信息);
        const auto Vec记录数量 = 计算Vec记录数量_已加锁(特征值节点);
        if (!追根因检查(Vec记录数量 <= 1, L"写入 VecI64 时发现重复 Vec 原始值记录。")) {
            return false;
        }
        auto Vec位置 = 查找Vec记录_已加锁(特征值节点);
        if (Vec位置 != Vec记录表_.end()
            && !追根因检查(Vec记录内部一致(*Vec位置), L"写入 VecI64 时已有 Vec 原始值记录内部不一致。")) {
            return false;
        }
        if (I64值.has_value() && Vec位置 != Vec记录表_.end()) {
            (void)追根因检查(false, L"写入 VecI64 时同一特征值节点已同时存在 I64 与 Vec 原始值。" );
            return false;
        }
        if (I64值.has_value() || (Vec位置 != Vec记录表_.end() && Vec位置->类型 != 特征值原始类型::VecI64)) {
            return false;
        }

        if (Vec位置 == Vec记录表_.end()) {
            新记录.容器版本 = 1;
            Vec记录表_.push_back(std::move(新记录));
        } else {
            if (!追根因检查(Vec位置->容器版本 < std::numeric_limits<std::uint64_t>::max(),
                L"替换 VecI64 时容器版本已经达到上限。")) {
                return false;
            }
            新记录.容器版本 = Vec位置->容器版本 + 1;
            *Vec位置 = std::move(新记录);
        }

        const auto 发布位置 = 查找Vec记录_已加锁(特征值节点);
        return 追根因检查(
            发布位置 != Vec记录表_.end()
                && Vec记录内部一致(*发布位置)
                && 发布位置->类型 == 特征值原始类型::VecI64
                && 发布位置->VecI64值 == 值,
            L"发布 VecI64 后原始值读回不及预期。");
    }

    std::optional<std::vector<std::int64_t>> 读取VecI64值(节点句柄 特征值节点) const {
        const auto 节点记录值 = 读取有效特征值节点(特征值节点);
        if (!节点记录值.has_value()) {
            return std::nullopt;
        }

        std::shared_lock<std::shared_mutex> 锁(原始值锁_);
        const auto I64值 = 主信息_.读取I64值(节点记录值->主信息);
        const auto Vec记录数量 = 计算Vec记录数量_已加锁(特征值节点);
        if (!追根因检查(Vec记录数量 <= 1, L"读取 VecI64 时发现重复 Vec 原始值记录。")) {
            return std::nullopt;
        }
        const auto Vec位置 = 查找Vec记录_已加锁(特征值节点);
        if (I64值.has_value() && Vec位置 != Vec记录表_.end()) {
            (void)追根因检查(false, L"读取 VecI64 时同一特征值节点已同时存在 I64 与 Vec 原始值。" );
            return std::nullopt;
        }
        if (Vec位置 == Vec记录表_.end() || Vec位置->类型 != 特征值原始类型::VecI64) {
            return std::nullopt;
        }
        if (!追根因检查(Vec记录内部一致(*Vec位置), L"读取 VecI64 时原始值记录内部不一致。")) {
            return std::nullopt;
        }
        return Vec位置->VecI64值;
    }

    bool 写入VecU64值(节点句柄 特征值节点, const std::vector<std::uint64_t>& 值) {
        const auto 节点记录值 = 读取有效特征值节点(特征值节点);
        if (!节点记录值.has_value() || !序列元素数量有效(值.size())) {
            return false;
        }

        Vec原始值记录 新记录;
        新记录.特征值节点 = 特征值节点;
        新记录.类型 = 特征值原始类型::VecU64;
        新记录.VecU64值 = 值;

        std::unique_lock<std::shared_mutex> 锁(原始值锁_);
        const auto I64值 = 主信息_.读取I64值(节点记录值->主信息);
        const auto Vec记录数量 = 计算Vec记录数量_已加锁(特征值节点);
        if (!追根因检查(Vec记录数量 <= 1, L"写入 VecU64 时发现重复 Vec 原始值记录。")) {
            return false;
        }
        auto Vec位置 = 查找Vec记录_已加锁(特征值节点);
        if (Vec位置 != Vec记录表_.end()
            && !追根因检查(Vec记录内部一致(*Vec位置), L"写入 VecU64 时已有 Vec 原始值记录内部不一致。")) {
            return false;
        }
        if (I64值.has_value() && Vec位置 != Vec记录表_.end()) {
            (void)追根因检查(false, L"写入 VecU64 时同一特征值节点已同时存在 I64 与 Vec 原始值。" );
            return false;
        }
        if (I64值.has_value() || (Vec位置 != Vec记录表_.end() && Vec位置->类型 != 特征值原始类型::VecU64)) {
            return false;
        }

        if (Vec位置 == Vec记录表_.end()) {
            新记录.容器版本 = 1;
            Vec记录表_.push_back(std::move(新记录));
        } else {
            if (!追根因检查(Vec位置->容器版本 < std::numeric_limits<std::uint64_t>::max(),
                L"替换 VecU64 时容器版本已经达到上限。")) {
                return false;
            }
            新记录.容器版本 = Vec位置->容器版本 + 1;
            *Vec位置 = std::move(新记录);
        }

        const auto 发布位置 = 查找Vec记录_已加锁(特征值节点);
        return 追根因检查(
            发布位置 != Vec记录表_.end()
                && Vec记录内部一致(*发布位置)
                && 发布位置->类型 == 特征值原始类型::VecU64
                && 发布位置->VecU64值 == 值,
            L"发布 VecU64 后原始值读回不及预期。");
    }

    std::optional<std::vector<std::uint64_t>> 读取VecU64值(节点句柄 特征值节点) const {
        const auto 节点记录值 = 读取有效特征值节点(特征值节点);
        if (!节点记录值.has_value()) {
            return std::nullopt;
        }

        std::shared_lock<std::shared_mutex> 锁(原始值锁_);
        const auto I64值 = 主信息_.读取I64值(节点记录值->主信息);
        const auto Vec记录数量 = 计算Vec记录数量_已加锁(特征值节点);
        if (!追根因检查(Vec记录数量 <= 1, L"读取 VecU64 时发现重复 Vec 原始值记录。")) {
            return std::nullopt;
        }
        const auto Vec位置 = 查找Vec记录_已加锁(特征值节点);
        if (I64值.has_value() && Vec位置 != Vec记录表_.end()) {
            (void)追根因检查(false, L"读取 VecU64 时同一特征值节点已同时存在 I64 与 Vec 原始值。" );
            return std::nullopt;
        }
        if (Vec位置 == Vec记录表_.end() || Vec位置->类型 != 特征值原始类型::VecU64) {
            return std::nullopt;
        }
        if (!追根因检查(Vec记录内部一致(*Vec位置), L"读取 VecU64 时原始值记录内部不一致。")) {
            return std::nullopt;
        }
        return Vec位置->VecU64值;
    }

    std::optional<特征值原始类型> 读取原始类型(节点句柄 特征值节点) const {
        const auto 节点记录值 = 读取有效特征值节点(特征值节点);
        if (!节点记录值.has_value()) {
            return std::nullopt;
        }

        std::shared_lock<std::shared_mutex> 锁(原始值锁_);
        const auto I64值 = 主信息_.读取I64值(节点记录值->主信息);
        const auto Vec记录数量 = 计算Vec记录数量_已加锁(特征值节点);
        if (!追根因检查(Vec记录数量 <= 1, L"读取特征值原始类型时发现重复 Vec 原始值记录。")) {
            return std::nullopt;
        }
        const auto Vec位置 = 查找Vec记录_已加锁(特征值节点);
        if (I64值.has_value() && Vec位置 != Vec记录表_.end()) {
            (void)追根因检查(false, L"读取特征值原始类型时同一节点存在多个原始类型。" );
            return std::nullopt;
        }
        if (Vec位置 != Vec记录表_.end()) {
            if (!追根因检查(Vec记录内部一致(*Vec位置), L"读取特征值原始类型时 Vec 记录内部不一致。")) {
                return std::nullopt;
            }
            return Vec位置->类型;
        }
        return I64值.has_value() ? 特征值原始类型::I64 : 特征值原始类型::未建立;
    }

    std::optional<std::uint64_t> 读取容器版本(节点句柄 特征值节点) const {
        const auto 节点记录值 = 读取有效特征值节点(特征值节点);
        if (!节点记录值.has_value()) {
            return std::nullopt;
        }

        std::shared_lock<std::shared_mutex> 锁(原始值锁_);
        const auto I64值 = 主信息_.读取I64值(节点记录值->主信息);
        const auto Vec记录数量 = 计算Vec记录数量_已加锁(特征值节点);
        if (!追根因检查(Vec记录数量 <= 1, L"读取特征值容器版本时发现重复 Vec 原始值记录。")) {
            return std::nullopt;
        }
        const auto Vec位置 = 查找Vec记录_已加锁(特征值节点);
        if (I64值.has_value() && Vec位置 != Vec记录表_.end()) {
            (void)追根因检查(false, L"读取特征值容器版本时同一节点存在多个原始类型。" );
            return std::nullopt;
        }
        if (Vec位置 == Vec记录表_.end()) {
            return std::nullopt;
        }
        if (!追根因检查(Vec记录内部一致(*Vec位置), L"读取特征值容器版本时 Vec 记录内部不一致。")) {
            return std::nullopt;
        }
        return Vec位置->容器版本;
    }

private:
    struct Vec原始值记录 {
        节点句柄 特征值节点;
        特征值原始类型 类型 = 特征值原始类型::未建立;
        std::vector<std::int64_t> VecI64值;
        std::vector<std::uint64_t> VecU64值;
        std::uint64_t 容器版本 = 0;
    };

    static bool 序列元素数量有效(std::size_t 元素数量) {
        return 元素数量 > 0 && 元素数量 <= 最大序列元素数量;
    }

    static bool Vec记录内部一致(const Vec原始值记录& 记录) {
        if (!句柄有效(记录.特征值节点) || 记录.容器版本 == 0) {
            return false;
        }
        if (记录.类型 == 特征值原始类型::VecI64) {
            return 序列元素数量有效(记录.VecI64值.size()) && 记录.VecU64值.empty();
        }
        if (记录.类型 == 特征值原始类型::VecU64) {
            return 序列元素数量有效(记录.VecU64值.size()) && 记录.VecI64值.empty();
        }
        return false;
    }

    std::optional<节点记录> 读取有效特征值节点(节点句柄 特征值节点) const {
        const auto 记录 = 节点_.读取节点(特征值节点);
        if (!记录.has_value() || 记录->类型 != 节点类型::特征值) {
            return std::nullopt;
        }
        if (!追根因检查(主信息_.主信息是否有效(记录->主信息), L"有效特征值节点的主信息已经失效。")) {
            return std::nullopt;
        }
        return 记录;
    }

    std::vector<Vec原始值记录>::iterator 查找Vec记录_已加锁(节点句柄 特征值节点) {
        return std::find_if(Vec记录表_.begin(), Vec记录表_.end(), [特征值节点](const auto& 记录) {
            return 记录.特征值节点 == 特征值节点;
        });
    }

    std::vector<Vec原始值记录>::const_iterator 查找Vec记录_已加锁(节点句柄 特征值节点) const {
        return std::find_if(Vec记录表_.cbegin(), Vec记录表_.cend(), [特征值节点](const auto& 记录) {
            return 记录.特征值节点 == 特征值节点;
        });
    }

    std::size_t 计算Vec记录数量_已加锁(节点句柄 特征值节点) const {
        return static_cast<std::size_t>(std::count_if(
            Vec记录表_.cbegin(), Vec记录表_.cend(), [特征值节点](const auto& 记录) {
                return 记录.特征值节点 == 特征值节点;
            }));
    }

    主信息仓库& 主信息_;
    节点仓库& 节点_;
    mutable std::shared_mutex 原始值锁_;
    std::vector<Vec原始值记录> Vec记录表_;
};

}
