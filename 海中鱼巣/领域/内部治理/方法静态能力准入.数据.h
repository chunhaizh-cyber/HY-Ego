#pragma once

#ifndef 方法静态能力准入数据_NO_INCLUDES
#include <cstdint>
#include <optional>
#include <vector>
#include "../L2普通方法结构.数据.h"
#endif

namespace 海中鱼巣 {

inline constexpr std::uint32_t 方法静态能力准入合同版本 = 1;

enum class 方法静态能力准入状态 : std::uint8_t {
    已读取 = 1,
    数量预算不足 = 2,
    当前性漂移 = 3,
    引用冲突 = 4,
    入口拒绝 = 5,
    资源失败 = 6,
    内部错误 = 7
};

enum class 方法静态能力准入结论 : std::uint8_t {
    有匹配候选 = 1,
    匹配为空 = 2,
    仅结构不完整 = 3,
    有匹配且有结构不完整 = 4
};

struct 方法静态能力准入请求 final {
    std::uint32_t 合同版本 = 方法静态能力准入合同版本;
    L2结构请求头 请求头;
    L2读取类别 读取类别 = L2读取类别::当前;
    std::uint64_t 历史截止事实代次 = 0;
    L2方法六项静态能力签名 需求;
    std::uint64_t 最大扫描用途事实数 = 0;
    std::uint64_t 最大粗召回方法数 = 0;
    friend bool operator==(const 方法静态能力准入请求&,
        const 方法静态能力准入请求&) = default;
};

struct 方法静态能力候选 final {
    L2方法身份 方法;
    L2方法内容版本 内容版本;
    L2方法规格版本 规格版本;
    L2方法结果身份 主轴结果;
    L2方法用途身份 匹配用途;
    L2方法六项静态能力签名 签名;
    friend bool operator==(const 方法静态能力候选&,
        const 方法静态能力候选&) = default;
};

struct 方法结构不完整粗召回项 final {
    L2方法身份 方法;
    std::vector<L2方法结构缺口> 结构缺口;
    friend bool operator==(const 方法结构不完整粗召回项&,
        const 方法结构不完整粗召回项&) = default;
};

struct 方法静态能力准入结果 final {
    方法静态能力准入状态 状态 = 方法静态能力准入状态::入口拒绝;
    std::optional<方法静态能力准入结论> 结论;
    std::uint64_t 共同事实截止 = 0;
    std::uint64_t 实际扫描用途事实数 = 0;
    std::vector<方法静态能力候选> 匹配候选;
    std::vector<方法结构不完整粗召回项> 结构不完整;
    bool 成功() const noexcept;
};

inline bool 方法静态能力准入请求有效(
    const 方法静态能力准入请求& 请求) noexcept {
    return 请求.合同版本 == 方法静态能力准入合同版本
        && L2读取截止请求有效(请求.请求头, 请求.读取类别,
            请求.历史截止事实代次)
        && L2方法六项静态能力签名完整(请求.需求)
        && 请求.最大扫描用途事实数 != 0
        && 请求.最大粗召回方法数 != 0;
}

inline bool 方法静态能力准入结果::成功() const noexcept {
    if (状态 != 方法静态能力准入状态::已读取 || !结论
        || 共同事实截止 == 0) return false;
    for (std::size_t i = 0; i < 匹配候选.size(); ++i) {
        const auto& 项 = 匹配候选[i];
        if (!有效(项.方法.值) || !L2方法内容版本有效(项.内容版本)
            || !L2方法规格版本有效(项.规格版本)
            || !L2方法结果身份有效(项.主轴结果)
            || !L2方法用途身份有效(项.匹配用途)
            || !L2方法六项静态能力签名完整(项.签名)) return false;
        if (i != 0) {
            const auto& 前 = 匹配候选[i - 1];
            if (前.方法.值.值 > 项.方法.值.值
                || (前.方法 == 项.方法
                    && 前.主轴结果.值.值 >= 项.主轴结果.值.值)) return false;
        }
    }
    for (std::size_t i = 0; i < 结构不完整.size(); ++i) {
        if (!有效(结构不完整[i].方法.值)
            || 结构不完整[i].结构缺口.empty()
            || (i != 0 && 结构不完整[i - 1].方法.值.值
                >= 结构不完整[i].方法.值.值)) return false;
    }
    const bool 有候选 = !匹配候选.empty();
    const bool 有缺口 = !结构不完整.empty();
    switch (*结论) {
    case 方法静态能力准入结论::有匹配候选:
        return 有候选 && !有缺口;
    case 方法静态能力准入结论::匹配为空:
        return !有候选 && !有缺口;
    case 方法静态能力准入结论::仅结构不完整:
        return !有候选 && 有缺口;
    case 方法静态能力准入结论::有匹配且有结构不完整:
        return 有候选 && 有缺口;
    }
    return false;
}

} // namespace 海中鱼巣
