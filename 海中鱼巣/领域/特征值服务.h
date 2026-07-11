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

struct 特征值原始材料 {
    节点句柄 特征值节点;
    特征值原始类型 原始类型 = 特征值原始类型::未建立;
    std::uint64_t 原始值版本 = 0;
    std::optional<std::int64_t> I64值;
    std::vector<std::int64_t> VecI64值;
    std::vector<std::uint64_t> VecU64值;

    bool 完整() const {
        if (!句柄有效(特征值节点) || 原始值版本 == 0) {
            return false;
        }
        switch (原始类型) {
        case 特征值原始类型::I64:
            return I64值.has_value() && VecI64值.empty() && VecU64值.empty();
        case 特征值原始类型::VecI64:
            return !I64值.has_value() && !VecI64值.empty() && VecU64值.empty();
        case 特征值原始类型::VecU64:
            return !I64值.has_value() && VecI64值.empty() && !VecU64值.empty();
        default:
            return false;
        }
    }
};

enum class 特征值非权威恢复请求 : std::uint32_t {
    未携带 = 0,
    请求恢复 = 1
};

enum class 特征值序列化结果状态 : std::uint32_t {
    已拒绝 = 0,
    可用 = 1,
    材料过期 = 2
};

enum class 特征值恢复候选状态 : std::uint32_t {
    已拒绝 = 0,
    可作为恢复候选 = 1
};

enum class 特征值序列化拒绝原因 : std::uint32_t {
    无 = 0,
    格式版本不匹配 = 1,
    节点类型枚举版本不匹配 = 2,
    关系类型枚举版本不匹配 = 3,
    值类型枚举版本不匹配 = 4,
    仓库上下文不匹配 = 5,
    材料序号无效 = 6,
    句柄材料无效 = 7,
    原始版本无效 = 8,
    错误归属 = 9,
    原始值未建立 = 10,
    元素数量不一致 = 11,
    原始类型冲突 = 12,
    原始值内容不匹配 = 13,
    非权威材料请求 = 14,
    材料已过期 = 15
};

struct 特征值仓库上下文材料 {
    std::uint64_t 仓库集合编号 = 0;
    std::uint64_t 节点仓库编号 = 0;

    bool 完整() const {
        return 仓库集合编号 != 0 && 节点仓库编号 != 0;
    }
};

inline bool operator==(const 特征值仓库上下文材料& 左, const 特征值仓库上下文材料& 右) {
    return 左.仓库集合编号 == 右.仓库集合编号
        && 左.节点仓库编号 == 右.节点仓库编号;
}

struct 特征值序列化材料 {
    std::uint32_t 格式版本 = 0;
    std::uint32_t 节点类型枚举版本 = 0;
    std::uint32_t 关系类型枚举版本 = 0;
    std::uint32_t 值类型枚举版本 = 0;
    特征值仓库上下文材料 仓库上下文;
    std::uint64_t 材料序号 = 0;
    节点句柄 特征节点;
    节点句柄 特征值节点;
    特征值原始类型 原始类型 = 特征值原始类型::未建立;
    std::uint64_t 原始值版本 = 0;
    std::uint64_t 元素数量 = 0;
    std::optional<std::int64_t> I64值;
    std::vector<std::int64_t> VecI64值;
    std::vector<std::uint64_t> VecU64值;
    特征值非权威恢复请求 非权威恢复请求 = 特征值非权威恢复请求::未携带;

    bool 字段自洽() const {
        if (格式版本 == 0
            || 节点类型枚举版本 == 0
            || 关系类型枚举版本 == 0
            || 值类型枚举版本 == 0
            || !仓库上下文.完整()
            || 材料序号 == 0
            || !句柄有效(特征节点)
            || !句柄有效(特征值节点)
            || 特征节点 == 特征值节点
            || 特征节点.仓库编号 != 仓库上下文.节点仓库编号
            || 特征值节点.仓库编号 != 仓库上下文.节点仓库编号
            || 原始值版本 == 0
            || 原始值版本 == std::numeric_limits<std::uint64_t>::max()) {
            return false;
        }
        switch (原始类型) {
        case 特征值原始类型::I64:
            return 元素数量 == 1
                && I64值.has_value()
                && VecI64值.empty()
                && VecU64值.empty();
        case 特征值原始类型::VecI64:
            return !I64值.has_value()
                && !VecI64值.empty()
                && VecU64值.empty()
                && 元素数量 == static_cast<std::uint64_t>(VecI64值.size());
        case 特征值原始类型::VecU64:
            return !I64值.has_value()
                && VecI64值.empty()
                && !VecU64值.empty()
                && 元素数量 == static_cast<std::uint64_t>(VecU64值.size());
        default:
            return false;
        }
    }
};

struct 特征值序列化生成结果 {
    特征值序列化结果状态 状态 = 特征值序列化结果状态::已拒绝;
    特征值序列化拒绝原因 拒绝原因 = 特征值序列化拒绝原因::句柄材料无效;
    std::optional<特征值序列化材料> 材料;

    bool 完整() const {
        return 状态 == 特征值序列化结果状态::可用
            && 拒绝原因 == 特征值序列化拒绝原因::无
            && 材料.has_value()
            && 材料->字段自洽();
    }
};

struct 特征值恢复候选材料 {
    特征值恢复候选状态 状态 = 特征值恢复候选状态::已拒绝;
    特征值序列化拒绝原因 拒绝原因 = 特征值序列化拒绝原因::句柄材料无效;
    特征值仓库上下文材料 目标仓库上下文;
    std::uint64_t 材料序号 = 0;
    节点句柄 特征节点;
    std::optional<特征值原始材料> 规范化原始材料;

    bool 完整() const {
        return 状态 == 特征值恢复候选状态::可作为恢复候选
            && 拒绝原因 == 特征值序列化拒绝原因::无
            && 目标仓库上下文.完整()
            && 材料序号 != 0
            && 句柄有效(特征节点)
            && 规范化原始材料.has_value()
            && 规范化原始材料->完整();
    }
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
        const auto 写入前状态 = 读取原始值状态_已加锁(特征值节点, 节点记录值->主信息);
        if (!写入前状态.内部一致
            || 写入前状态.类型 == 特征值原始类型::VecI64
            || 写入前状态.类型 == 特征值原始类型::VecU64) {
            return false;
        }
        if (写入前状态.类型 == 特征值原始类型::I64
            && 写入前状态.原始值版本 == std::numeric_limits<std::uint64_t>::max()) {
            return false;
        }

        const auto 新版本 = 写入前状态.类型 == 特征值原始类型::I64
            ? 写入前状态.原始值版本 + 1
            : 1;
        auto 版本位置 = 查找I64版本记录_已加锁(特征值节点);
        if (!追根因检查(
            (写入前状态.类型 == 特征值原始类型::I64) == (版本位置 != I64版本记录表_.end()),
            L"写入 I64 前原始值与版本记录不对应。")) {
            return false;
        }
        if (!追根因检查(主信息_.写入I64值(节点记录值->主信息, 值), L"写入 I64 特征值时主信息写入不及预期。")) {
            return false;
        }
        if (版本位置 == I64版本记录表_.end()) {
            I64版本记录表_.push_back({特征值节点, 新版本});
        } else {
            版本位置->原始值版本 = 新版本;
        }

        const auto 写入后状态 = 读取原始值状态_已加锁(特征值节点, 节点记录值->主信息);
        return 追根因检查(
            写入后状态.内部一致
                && 写入后状态.类型 == 特征值原始类型::I64
                && 写入后状态.原始值版本 == 新版本
                && 写入后状态.I64值 == 值,
            L"写入 I64 特征值后值与原始值版本读回不及预期。");
    }

    std::optional<std::int64_t> 读取I64值(节点句柄 特征值节点) const {
        const auto 节点记录值 = 读取有效特征值节点(特征值节点);
        if (!节点记录值.has_value()) {
            return std::nullopt;
        }

        std::shared_lock<std::shared_mutex> 锁(原始值锁_);
        const auto 状态 = 读取原始值状态_已加锁(特征值节点, 节点记录值->主信息);
        if (!状态.内部一致 || 状态.类型 != 特征值原始类型::I64) {
            return std::nullopt;
        }
        return 状态.I64值;
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
        const auto 写入前状态 = 读取原始值状态_已加锁(特征值节点, 节点记录值->主信息);
        if (!写入前状态.内部一致
            || 写入前状态.类型 == 特征值原始类型::I64
            || 写入前状态.类型 == 特征值原始类型::VecU64) {
            return false;
        }
        if (写入前状态.类型 == 特征值原始类型::VecI64
            && 写入前状态.原始值版本 == std::numeric_limits<std::uint64_t>::max()) {
            return false;
        }

        auto Vec位置 = 查找Vec记录_已加锁(特征值节点);
        if (!追根因检查(
            (写入前状态.类型 == 特征值原始类型::VecI64) == (Vec位置 != Vec记录表_.end()),
            L"写入 VecI64 前原始值与记录不对应。")) {
            return false;
        }
        新记录.容器版本 = 写入前状态.类型 == 特征值原始类型::VecI64
            ? 写入前状态.原始值版本 + 1
            : 1;
        if (Vec位置 == Vec记录表_.end()) {
            Vec记录表_.push_back(std::move(新记录));
        } else {
            *Vec位置 = std::move(新记录);
        }

        const auto 写入后状态 = 读取原始值状态_已加锁(特征值节点, 节点记录值->主信息);
        return 追根因检查(
            写入后状态.内部一致
                && 写入后状态.类型 == 特征值原始类型::VecI64
                && 写入后状态.原始值版本
                    == (写入前状态.类型 == 特征值原始类型::VecI64 ? 写入前状态.原始值版本 + 1 : 1)
                && 写入后状态.Vec记录 != nullptr
                && 写入后状态.Vec记录->VecI64值 == 值,
            L"发布 VecI64 后值与原始值版本读回不及预期。");
    }

    std::optional<std::vector<std::int64_t>> 读取VecI64值(节点句柄 特征值节点) const {
        const auto 节点记录值 = 读取有效特征值节点(特征值节点);
        if (!节点记录值.has_value()) {
            return std::nullopt;
        }

        std::shared_lock<std::shared_mutex> 锁(原始值锁_);
        const auto 状态 = 读取原始值状态_已加锁(特征值节点, 节点记录值->主信息);
        if (!状态.内部一致 || 状态.类型 != 特征值原始类型::VecI64 || 状态.Vec记录 == nullptr) {
            return std::nullopt;
        }
        return 状态.Vec记录->VecI64值;
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
        const auto 写入前状态 = 读取原始值状态_已加锁(特征值节点, 节点记录值->主信息);
        if (!写入前状态.内部一致
            || 写入前状态.类型 == 特征值原始类型::I64
            || 写入前状态.类型 == 特征值原始类型::VecI64) {
            return false;
        }
        if (写入前状态.类型 == 特征值原始类型::VecU64
            && 写入前状态.原始值版本 == std::numeric_limits<std::uint64_t>::max()) {
            return false;
        }

        auto Vec位置 = 查找Vec记录_已加锁(特征值节点);
        if (!追根因检查(
            (写入前状态.类型 == 特征值原始类型::VecU64) == (Vec位置 != Vec记录表_.end()),
            L"写入 VecU64 前原始值与记录不对应。")) {
            return false;
        }
        新记录.容器版本 = 写入前状态.类型 == 特征值原始类型::VecU64
            ? 写入前状态.原始值版本 + 1
            : 1;
        if (Vec位置 == Vec记录表_.end()) {
            Vec记录表_.push_back(std::move(新记录));
        } else {
            *Vec位置 = std::move(新记录);
        }

        const auto 写入后状态 = 读取原始值状态_已加锁(特征值节点, 节点记录值->主信息);
        return 追根因检查(
            写入后状态.内部一致
                && 写入后状态.类型 == 特征值原始类型::VecU64
                && 写入后状态.原始值版本
                    == (写入前状态.类型 == 特征值原始类型::VecU64 ? 写入前状态.原始值版本 + 1 : 1)
                && 写入后状态.Vec记录 != nullptr
                && 写入后状态.Vec记录->VecU64值 == 值,
            L"发布 VecU64 后值与原始值版本读回不及预期。");
    }

    std::optional<std::vector<std::uint64_t>> 读取VecU64值(节点句柄 特征值节点) const {
        const auto 节点记录值 = 读取有效特征值节点(特征值节点);
        if (!节点记录值.has_value()) {
            return std::nullopt;
        }

        std::shared_lock<std::shared_mutex> 锁(原始值锁_);
        const auto 状态 = 读取原始值状态_已加锁(特征值节点, 节点记录值->主信息);
        if (!状态.内部一致 || 状态.类型 != 特征值原始类型::VecU64 || 状态.Vec记录 == nullptr) {
            return std::nullopt;
        }
        return 状态.Vec记录->VecU64值;
    }

    std::optional<特征值原始类型> 读取原始类型(节点句柄 特征值节点) const {
        const auto 节点记录值 = 读取有效特征值节点(特征值节点);
        if (!节点记录值.has_value()) {
            return std::nullopt;
        }

        std::shared_lock<std::shared_mutex> 锁(原始值锁_);
        const auto 状态 = 读取原始值状态_已加锁(特征值节点, 节点记录值->主信息);
        if (!状态.内部一致) {
            return std::nullopt;
        }
        return 状态.类型;
    }

    std::optional<std::uint64_t> 读取容器版本(节点句柄 特征值节点) const {
        const auto 节点记录值 = 读取有效特征值节点(特征值节点);
        if (!节点记录值.has_value()) {
            return std::nullopt;
        }

        std::shared_lock<std::shared_mutex> 锁(原始值锁_);
        const auto 状态 = 读取原始值状态_已加锁(特征值节点, 节点记录值->主信息);
        if (!状态.内部一致
            || (状态.类型 != 特征值原始类型::VecI64 && 状态.类型 != 特征值原始类型::VecU64)) {
            return std::nullopt;
        }
        return 状态.原始值版本;
    }

    std::optional<std::uint64_t> 读取原始值版本(节点句柄 特征值节点) const {
        const auto 节点记录值 = 读取有效特征值节点(特征值节点);
        if (!节点记录值.has_value()) {
            return std::nullopt;
        }

        std::shared_lock<std::shared_mutex> 锁(原始值锁_);
        const auto 状态 = 读取原始值状态_已加锁(特征值节点, 节点记录值->主信息);
        if (!状态.内部一致 || 状态.类型 == 特征值原始类型::未建立) {
            return std::nullopt;
        }
        return 状态.原始值版本;
    }

    std::optional<特征值原始材料> 读取原始值材料(节点句柄 特征值节点) const {
        const auto 节点记录值 = 读取有效特征值节点(特征值节点);
        if (!节点记录值.has_value()) {
            return std::nullopt;
        }

        std::shared_lock<std::shared_mutex> 锁(原始值锁_);
        const auto 状态 = 读取原始值状态_已加锁(特征值节点, 节点记录值->主信息);
        if (!状态.内部一致 || 状态.类型 == 特征值原始类型::未建立) {
            return std::nullopt;
        }

        特征值原始材料 材料;
        材料.特征值节点 = 特征值节点;
        材料.原始类型 = 状态.类型;
        材料.原始值版本 = 状态.原始值版本;
        材料.I64值 = 状态.I64值;
        if (状态.类型 == 特征值原始类型::VecI64 && 状态.Vec记录 != nullptr) {
            材料.VecI64值 = 状态.Vec记录->VecI64值;
        } else if (状态.类型 == 特征值原始类型::VecU64 && 状态.Vec记录 != nullptr) {
            材料.VecU64值 = 状态.Vec记录->VecU64值;
        }
        if (!追根因检查(材料.完整(), L"读取特征值原始材料后材料不完整。")) {
            return std::nullopt;
        }
        return 材料;
    }

private:
    struct Vec原始值记录 {
        节点句柄 特征值节点;
        特征值原始类型 类型 = 特征值原始类型::未建立;
        std::vector<std::int64_t> VecI64值;
        std::vector<std::uint64_t> VecU64值;
        std::uint64_t 容器版本 = 0;
    };

    struct I64版本记录 {
        节点句柄 特征值节点;
        std::uint64_t 原始值版本 = 0;
    };

    struct 原始值状态 {
        bool 内部一致 = false;
        特征值原始类型 类型 = 特征值原始类型::未建立;
        std::uint64_t 原始值版本 = 0;
        std::optional<std::int64_t> I64值;
        const Vec原始值记录* Vec记录 = nullptr;
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

    static bool I64版本记录内部一致(const I64版本记录& 记录) {
        return 句柄有效(记录.特征值节点) && 记录.原始值版本 != 0;
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

    原始值状态 读取原始值状态_已加锁(节点句柄 特征值节点, 主信息句柄 主信息句柄值) const {
        原始值状态 结果;
        const auto Vec记录数量 = 计算Vec记录数量_已加锁(特征值节点);
        const auto I64版本记录数量 = 计算I64版本记录数量_已加锁(特征值节点);
        if (!追根因检查(Vec记录数量 <= 1, L"同一特征值节点存在重复 Vec 原始值记录。")
            || !追根因检查(I64版本记录数量 <= 1, L"同一特征值节点存在重复 I64 版本记录。")) {
            return 结果;
        }

        const auto Vec位置 = 查找Vec记录_已加锁(特征值节点);
        const auto I64版本位置 = 查找I64版本记录_已加锁(特征值节点);
        if (Vec位置 != Vec记录表_.cend()
            && !追根因检查(Vec记录内部一致(*Vec位置), L"特征值 Vec 原始值记录内部不一致。")) {
            return 结果;
        }
        if (I64版本位置 != I64版本记录表_.cend()
            && !追根因检查(I64版本记录内部一致(*I64版本位置), L"特征值 I64 版本记录内部不一致。")) {
            return 结果;
        }

        const auto I64值 = 主信息_.读取I64值(主信息句柄值);
        if (I64值.has_value() && Vec位置 != Vec记录表_.cend()) {
            (void)追根因检查(false, L"同一特征值节点存在多个原始类型。" );
            return 结果;
        }
        if (Vec位置 != Vec记录表_.cend() && I64版本位置 != I64版本记录表_.cend()) {
            (void)追根因检查(false, L"Vec 特征值节点残留 I64 版本记录。" );
            return 结果;
        }
        if (I64值.has_value()) {
            if (!追根因检查(I64版本位置 != I64版本记录表_.cend(), L"I64 特征值缺少原始值版本。")) {
                return 结果;
            }
            结果.内部一致 = true;
            结果.类型 = 特征值原始类型::I64;
            结果.原始值版本 = I64版本位置->原始值版本;
            结果.I64值 = I64值;
            return 结果;
        }
        if (Vec位置 != Vec记录表_.cend()) {
            结果.内部一致 = true;
            结果.类型 = Vec位置->类型;
            结果.原始值版本 = Vec位置->容器版本;
            结果.Vec记录 = &(*Vec位置);
            return 结果;
        }
        if (!追根因检查(I64版本位置 == I64版本记录表_.cend(), L"未建立 I64 原始值却存在版本记录。")) {
            return 结果;
        }
        结果.内部一致 = true;
        return 结果;
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

    std::vector<I64版本记录>::iterator 查找I64版本记录_已加锁(节点句柄 特征值节点) {
        return std::find_if(I64版本记录表_.begin(), I64版本记录表_.end(), [特征值节点](const auto& 记录) {
            return 记录.特征值节点 == 特征值节点;
        });
    }

    std::vector<I64版本记录>::const_iterator 查找I64版本记录_已加锁(节点句柄 特征值节点) const {
        return std::find_if(I64版本记录表_.cbegin(), I64版本记录表_.cend(), [特征值节点](const auto& 记录) {
            return 记录.特征值节点 == 特征值节点;
        });
    }

    std::size_t 计算Vec记录数量_已加锁(节点句柄 特征值节点) const {
        return static_cast<std::size_t>(std::count_if(
            Vec记录表_.cbegin(), Vec记录表_.cend(), [特征值节点](const auto& 记录) {
                return 记录.特征值节点 == 特征值节点;
            }));
    }

    std::size_t 计算I64版本记录数量_已加锁(节点句柄 特征值节点) const {
        return static_cast<std::size_t>(std::count_if(
            I64版本记录表_.cbegin(), I64版本记录表_.cend(), [特征值节点](const auto& 记录) {
                return 记录.特征值节点 == 特征值节点;
            }));
    }

    主信息仓库& 主信息_;
    节点仓库& 节点_;
    mutable std::shared_mutex 原始值锁_;
    std::vector<Vec原始值记录> Vec记录表_;
    std::vector<I64版本记录> I64版本记录表_;
};

}
