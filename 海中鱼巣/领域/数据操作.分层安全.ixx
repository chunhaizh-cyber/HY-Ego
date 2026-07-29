// 文件规则：本模块唯一拥有分层安全非值式外部参与载荷、执行器编排和发布后读回，不把参与者暴露给公开服务。
module;

#include "../核心/句柄.h"

#include <cstdint>
#include <memory>
#include <optional>
#include <span>
#include <type_traits>
#include <utility>
#include <vector>

export module 海中鱼巣.领域.数据操作.分层安全;

import 海中鱼巣.领域.协议.分层安全维护;
import 海中鱼巣.领域.参与者.分层安全维护记录;
import 海中鱼巣.核心.执行器.节点直接身份结构写入;

export namespace 海中鱼巣 {

struct 安全维护外部事实参与载荷 final {
    std::vector<std::unique_ptr<节点直接身份结构写入事务参与者>> 参与者所有权组;
    std::uint64_t 候选代次 = 0;
    安全输入版本快照 来源版本;

    安全维护外部事实参与载荷() = default;
    安全维护外部事实参与载荷(安全维护外部事实参与载荷&&) noexcept = default;
    安全维护外部事实参与载荷& operator=(安全维护外部事实参与载荷&&) noexcept = default;
    安全维护外部事实参与载荷(const 安全维护外部事实参与载荷&) = delete;
    安全维护外部事实参与载荷& operator=(const 安全维护外部事实参与载荷&) = delete;
};

struct 安全维护外部事实参与结果 final {
    安全结果分类 结果分类 = 安全结果分类::内部不一致;
    std::optional<安全维护外部事实参与载荷> 载荷;

    安全维护外部事实参与结果() = default;
    安全维护外部事实参与结果(安全维护外部事实参与结果&&) noexcept = default;
    安全维护外部事实参与结果& operator=(安全维护外部事实参与结果&&) noexcept = default;
    安全维护外部事实参与结果(const 安全维护外部事实参与结果&) = delete;
    安全维护外部事实参与结果& operator=(const 安全维护外部事实参与结果&) = delete;
};

class 安全维护外部事实参与提供者 {
public:
    virtual ~安全维护外部事实参与提供者() = default;
    virtual 安全维护外部事实参与结果 形成安全维护外部事实参与包(
        const 安全维护外部事实规格& 规格) = 0;
};

class 分层安全数据操作 final {
public:
    分层安全数据操作(
        节点直接身份结构写入执行器& 执行器,
        分层安全维护记录仓库& 安全记录仓库) noexcept
        : 执行器_(执行器), 安全记录仓库_(安全记录仓库) {
    }

    分层安全数据操作(const 分层安全数据操作&) = delete;
    分层安全数据操作& operator=(const 分层安全数据操作&) = delete;

    安全维护结果 提交安全维护候选(
        const 安全维护请求& 请求,
        const 安全维护载荷& 候选,
        std::optional<安全维护外部事实参与载荷>&& 外部参与) noexcept {
        try {
            const bool 有值变化 = 候选.整数变化量 != 0;
            if (有值变化 != 外部参与.has_value()) {
                return {安全结果分类::入口拒绝, std::nullopt};
            }
            if (请求.维护族 != 候选.维护族
                || 请求.期望当前值版本 != 候选.当前值版本
                || 请求.事实截止版本 != 候选.消费事实截止版本
                || 请求.维护规则版本 != 候选.维护规则版本
                || 候选.发布代次 == 0) {
                return {安全结果分类::版本漂移, std::nullopt};
            }
            if (外部参与) {
                if (外部参与->候选代次 != 候选.发布代次
                    || 外部参与->来源版本.事实截止版本
                        != 候选.消费事实截止版本
                    || 外部参与->来源版本.维护规则版本
                        != 候选.维护规则版本
                    || 外部参与->参与者所有权组.empty()) {
                    return {安全结果分类::版本漂移, std::nullopt};
                }
                for (std::size_t 左 = 0; 左 < 外部参与->参与者所有权组.size(); ++左) {
                    if (!外部参与->参与者所有权组[左]) {
                        return {安全结果分类::入口拒绝, std::nullopt};
                    }
                    for (std::size_t 右 = 左 + 1;
                         右 < 外部参与->参与者所有权组.size(); ++右) {
                        if (外部参与->参与者所有权组[左].get()
                            == 外部参与->参与者所有权组[右].get()) {
                            return {安全结果分类::入口拒绝, std::nullopt};
                        }
                    }
                }
            }

            auto 记录结果 = 形成分层安全维护记录参与者(
                安全记录仓库_, 请求, 候选);
            if (记录结果.结果分类 != 安全结果分类::已形成 || !记录结果.参与者) {
                return {记录结果.结果分类, std::nullopt};
            }
            std::vector<节点直接身份结构写入事务参与者*> 参与者组;
            参与者组.reserve(1 + (外部参与 ? 外部参与->参与者所有权组.size() : 0));
            参与者组.push_back(记录结果.参与者.get());
            if (外部参与) {
                for (auto& 参与者 : 外部参与->参与者所有权组) {
                    参与者组.push_back(参与者.get());
                }
            }
            const auto 执行结果 = 执行器_.执行(
                [](节点直接身份结构写入会话& 会话) {
                    会话.请求提交();
                },
                std::span<节点直接身份结构写入事务参与者* const>(参与者组));
            switch (执行结果.状态) {
            case 节点直接身份结构写入状态::已提交:
            case 节点直接身份结构写入状态::幂等读回:
                break;
            case 节点直接身份结构写入状态::入口拒绝:
            case 节点直接身份结构写入状态::许可拒绝:
            case 节点直接身份结构写入状态::身份冲突:
                return {安全结果分类::入口拒绝, std::nullopt};
            case 节点直接身份结构写入状态::版本漂移:
            case 节点直接身份结构写入状态::候选已撤销:
                return {安全结果分类::版本漂移, std::nullopt};
            case 节点直接身份结构写入状态::资源失败:
                return {安全结果分类::资源失败, std::nullopt};
            case 节点直接身份结构写入状态::候选已确认:
            case 节点直接身份结构写入状态::内部不一致:
            default:
                return {安全结果分类::内部不一致, std::nullopt};
            }
            auto 读回 = 读取已发布分层安全维护结果(安全记录仓库_, 请求);
            if (!读回.载荷
                || 读回.载荷->维护族 != 候选.维护族
                || 读回.载荷->前值 != 候选.前值
                || 读回.载荷->后值 != 候选.后值
                || 读回.载荷->当前值版本 != 候选.当前值版本
                || 读回.载荷->新值版本 != 候选.新值版本
                || 读回.载荷->维护账版本 != 候选.维护账版本
                || 读回.载荷->发布代次 != 候选.发布代次
                || 读回.载荷->消费事实截止版本 != 候选.消费事实截止版本) {
                return {安全结果分类::内部不一致, std::nullopt};
            }
            if (执行结果.状态 == 节点直接身份结构写入状态::幂等读回) {
                读回.结果分类 = 安全结果分类::精确重复;
            }
            return 读回;
        } catch (...) {
            return {安全结果分类::资源失败, std::nullopt};
        }
    }

private:
    节点直接身份结构写入执行器& 执行器_;
    分层安全维护记录仓库& 安全记录仓库_;
};

static_assert(!std::is_copy_constructible_v<安全维护外部事实参与载荷>);
static_assert(!std::is_copy_assignable_v<安全维护外部事实参与载荷>);
static_assert(std::is_move_constructible_v<安全维护外部事实参与载荷>);
static_assert(!std::is_copy_constructible_v<安全维护外部事实参与结果>);

}
