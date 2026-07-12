// 文件规则：中央自检运行器只管理测试登记、确定顺序、全量执行和值式汇总；不得写业务结构或启动生产设施。
module;

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

export module 海中鱼巣.自检.运行器;

export namespace 海中鱼巣 {

struct 自检单元结果 {
    std::string 编号;
    std::string 名称;
    bool 总通过 = false;
    std::size_t 验收项数量 = 0;
    std::size_t 失败项数量 = 0;
};

struct 自检批次结果 {
    std::size_t 登记数量 = 0;
    std::size_t 执行数量 = 0;
    std::size_t 通过数量 = 0;
    std::size_t 失败数量 = 0;
    std::vector<std::string> 失败编号组;
    bool 运行器契约完整 = false;
    bool 总通过 = false;
};

using 自检执行回调 = std::function<自检单元结果()>;

class 自检运行器 {
public:
    bool 登记自检(
        std::uint32_t 顺序,
        std::string 编号,
        std::string 名称,
        自检执行回调 回调) {
        if (已冻结_ || 已执行_ || 顺序 == 0 || 编号.empty() || 名称.empty() || !回调) {
            合同完整_ = false;
            return false;
        }
        const auto 重复 = std::find_if(登记组_.begin(), 登记组_.end(), [&](const 自检登记& 已有) {
            return 已有.顺序 == 顺序 || 已有.编号 == 编号;
        });
        if (重复 != 登记组_.end()) {
            合同完整_ = false;
            return false;
        }
        登记组_.push_back({顺序, std::move(编号), std::move(名称), std::move(回调)});
        return true;
    }

    自检批次结果 运行全部() {
        if (已执行_) {
            return {登记组_.size(), 0, 0, 0, {}, false, false};
        }
        已冻结_ = true;
        已执行_ = true;
        自检批次结果 批次;
        批次.登记数量 = 登记组_.size();
        批次.运行器契约完整 = 合同完整_;
        if (!合同完整_) {
            return 批次;
        }

        std::stable_sort(登记组_.begin(), 登记组_.end(), [](const 自检登记& 左, const 自检登记& 右) {
            return 左.顺序 < 右.顺序;
        });
        for (const auto& 登记 : 登记组_) {
            自检单元结果 单元;
            try {
                单元 = 登记.回调();
            }
            catch (...) {
                单元 = {登记.编号, 登记.名称, false, 1, 1};
            }
            ++批次.执行数量;
            const bool 单元通过 = 单元.总通过 && 单元.失败项数量 == 0;
            if (单元通过) {
                ++批次.通过数量;
            }
            else {
                ++批次.失败数量;
                批次.失败编号组.push_back(登记.编号);
            }
        }
        批次.总通过 = 批次.运行器契约完整
            && 批次.执行数量 == 批次.登记数量
            && 批次.失败数量 == 0;
        return 批次;
    }

    static int 转换退出码(const 自检批次结果& 结果) noexcept {
        return 结果.运行器契约完整 && 结果.总通过 ? 0 : 1;
    }

private:
    struct 自检登记 {
        std::uint32_t 顺序 = 0;
        std::string 编号;
        std::string 名称;
        自检执行回调 回调;
    };

    std::vector<自检登记> 登记组_;
    bool 合同完整_ = true;
    bool 已冻结_ = false;
    bool 已执行_ = false;
};

enum class 自检阶段 : std::uint8_t {
    初始化闭环 = 1,
    线程协议 = 2,
    领域回执 = 3,
    最终回归 = 4
};

struct 自检阶段执行结果 {
    自检阶段 阶段 = 自检阶段::初始化闭环;
    bool 已执行 = false;
    自检批次结果 批次;
};

struct 阶段化自检结果 {
    std::size_t 登记数量 = 0;
    std::size_t 执行数量 = 0;
    std::size_t 通过数量 = 0;
    std::size_t 失败数量 = 0;
    std::size_t 已执行阶段数量 = 0;
    std::vector<std::string> 失败编号组;
    bool 总成契约完整 = false;
    bool 总通过 = false;
};

class 阶段化自检总成 {
public:
    bool 登记自检(
        自检阶段 阶段,
        std::uint32_t 阶段内顺序,
        std::string 编号,
        std::string 名称,
        自检执行回调 回调) {
        const auto 阶段索引 = 转换阶段索引(阶段);
        if (!阶段索引.has_value()
            || 阶段内顺序 == 0
            || 编号.empty()
            || 名称.empty()
            || !回调
            || 阶段槽组_[*阶段索引].已执行
            || std::find(全局编号组_.begin(), 全局编号组_.end(), 编号) != 全局编号组_.end()
            || std::find(
                阶段槽组_[*阶段索引].顺序组.begin(),
                阶段槽组_[*阶段索引].顺序组.end(),
                阶段内顺序) != 阶段槽组_[*阶段索引].顺序组.end()) {
            return false;
        }
        if (!阶段槽组_[*阶段索引].运行器.登记自检(
                阶段内顺序, 编号, std::move(名称), std::move(回调))) {
            合同完整_ = false;
            return false;
        }
        全局编号组_.push_back(std::move(编号));
        阶段槽组_[*阶段索引].顺序组.push_back(阶段内顺序);
        ++登记数量_;
        return true;
    }

    自检阶段执行结果 运行阶段(自检阶段 阶段) {
        const auto 阶段索引 = 转换阶段索引(阶段);
        if (!阶段索引.has_value()
            || *阶段索引 != 下一阶段索引_
            || 阶段槽组_[*阶段索引].已执行) {
            return {阶段, false, {}};
        }
        auto 批次 = 阶段槽组_[*阶段索引].运行器.运行全部();
        阶段槽组_[*阶段索引].已执行 = true;
        阶段槽组_[*阶段索引].结果 = 批次;
        ++下一阶段索引_;
        return {阶段, true, std::move(批次)};
    }

    阶段化自检结果 完成总成() const {
        阶段化自检结果 结果;
        结果.登记数量 = 登记数量_;
        结果.总成契约完整 = 合同完整_;
        for (const auto& 阶段槽 : 阶段槽组_) {
            if (!阶段槽.顺序组.empty() && !阶段槽.已执行) {
                结果.总成契约完整 = false;
            }
            if (!阶段槽.已执行) {
                continue;
            }
            ++结果.已执行阶段数量;
            结果.执行数量 += 阶段槽.结果.执行数量;
            结果.通过数量 += 阶段槽.结果.通过数量;
            结果.失败数量 += 阶段槽.结果.失败数量;
            结果.失败编号组.insert(
                结果.失败编号组.end(),
                阶段槽.结果.失败编号组.begin(),
                阶段槽.结果.失败编号组.end());
            结果.总成契约完整 = 结果.总成契约完整 && 阶段槽.结果.运行器契约完整;
        }
        结果.总通过 = 结果.总成契约完整
            && 结果.执行数量 == 结果.登记数量
            && 结果.失败数量 == 0;
        return 结果;
    }

    static int 转换退出码(const 阶段化自检结果& 结果) noexcept {
        return 结果.总成契约完整 && 结果.总通过 ? 0 : 1;
    }

private:
    struct 阶段槽 {
        自检运行器 运行器;
        std::vector<std::uint32_t> 顺序组;
        bool 已执行 = false;
        自检批次结果 结果;
    };

    static std::optional<std::size_t> 转换阶段索引(自检阶段 阶段) noexcept {
        const auto 数值 = static_cast<std::uint8_t>(阶段);
        if (数值 < static_cast<std::uint8_t>(自检阶段::初始化闭环)
            || 数值 > static_cast<std::uint8_t>(自检阶段::最终回归)) {
            return std::nullopt;
        }
        return static_cast<std::size_t>(数值 - 1);
    }

    std::array<阶段槽, 4> 阶段槽组_;
    std::vector<std::string> 全局编号组_;
    std::size_t 登记数量_ = 0;
    std::size_t 下一阶段索引_ = 0;
    bool 合同完整_ = true;
};

自检单元结果 运行自检运行器合同自检() {
    const auto 输出验收项 = [](const char* 编号, const char* 名称, bool 通过) {
        std::cout << 编号 << ' ' << 名称 << ": " << (通过 ? "通过" : "失败") << '\n';
    };
    const auto 通过单元 = [] {
        return 自检单元结果{"样本", "样本", true, 1, 0};
    };

    自检运行器 合法运行器;
    const bool A02合法登记 = 合法运行器.登记自检(10, "合法", "合法单元", 通过单元);

    const auto 非法登记被拒绝 = [&](std::uint32_t 顺序, std::string 编号, std::string 名称, 自检执行回调 回调) {
        自检运行器 运行器;
        const bool 已拒绝 = !运行器.登记自检(顺序, std::move(编号), std::move(名称), std::move(回调));
        const auto 结果 = 运行器.运行全部();
        return 已拒绝 && !结果.运行器契约完整 && 结果.执行数量 == 0;
    };
    自检运行器 重复编号运行器;
    const bool 首次编号登记 = 重复编号运行器.登记自检(10, "重复", "甲", 通过单元);
    const bool 重复编号拒绝 = !重复编号运行器.登记自检(20, "重复", "乙", 通过单元);
    自检运行器 重复顺序运行器;
    const bool 首次顺序登记 = 重复顺序运行器.登记自检(10, "甲", "甲", 通过单元);
    const bool 重复顺序拒绝 = !重复顺序运行器.登记自检(10, "乙", "乙", 通过单元);
    const bool A03非法登记拒绝 =
        非法登记被拒绝(0, "零顺序", "零顺序", 通过单元)
        && 非法登记被拒绝(10, "", "空编号", 通过单元)
        && 非法登记被拒绝(10, "空名称", "", 通过单元)
        && 非法登记被拒绝(10, "空回调", "空回调", {})
        && 首次编号登记 && 重复编号拒绝
        && 首次顺序登记 && 重复顺序拒绝;

    std::vector<int> 执行顺序;
    自检运行器 排序运行器;
    const bool 反序登记一 = 排序运行器.登记自检(20, "后", "后", [&] {
        执行顺序.push_back(20);
        return 自检单元结果{"后", "后", true, 1, 0};
    });
    const bool 反序登记二 = 排序运行器.登记自检(10, "前", "前", [&] {
        执行顺序.push_back(10);
        return 自检单元结果{"前", "前", true, 1, 0};
    });
    const auto 排序结果 = 排序运行器.运行全部();
    const bool A04确定顺序 = 反序登记一 && 反序登记二 && 排序结果.总通过
        && 执行顺序 == std::vector<int>{10, 20};

    std::size_t 执行次数 = 0;
    自检运行器 全量运行器;
    (void)全量运行器.登记自检(10, "通过甲", "通过甲", [&] {
        ++执行次数;
        return 自检单元结果{"通过甲", "通过甲", true, 1, 0};
    });
    (void)全量运行器.登记自检(20, "失败", "失败", [&] {
        ++执行次数;
        return 自检单元结果{"失败", "失败", false, 1, 1};
    });
    (void)全量运行器.登记自检(30, "通过乙", "通过乙", [&] {
        ++执行次数;
        return 自检单元结果{"通过乙", "通过乙", true, 1, 0};
    });
    const auto 全量结果 = 全量运行器.运行全部();
    const bool A05失败不短路 = 执行次数 == 3 && 全量结果.执行数量 == 3
        && 全量结果.通过数量 == 2 && 全量结果.失败数量 == 1;

    自检运行器 一次运行器;
    (void)一次运行器.登记自检(10, "一次", "一次", 通过单元);
    const auto 首次结果 = 一次运行器.运行全部();
    const bool 冻结后拒绝 = !一次运行器.登记自检(20, "迟到", "迟到", 通过单元);
    const auto 二次结果 = 一次运行器.运行全部();
    const bool A06冻结与一次执行 = 首次结果.总通过 && 冻结后拒绝
        && !二次结果.运行器契约完整 && 二次结果.执行数量 == 0;

    const bool A07汇总退出一致 = 全量结果.登记数量 == 3
        && 全量结果.失败编号组 == std::vector<std::string>{"失败"}
        && 自检运行器::转换退出码(全量结果) != 0
        && 自检运行器::转换退出码(首次结果) == 0;

    输出验收项("SELFTEST-RUNNER-S1-A02", "合法登记", A02合法登记);
    输出验收项("SELFTEST-RUNNER-S1-A03", "非法登记入口拒绝", A03非法登记拒绝);
    输出验收项("SELFTEST-RUNNER-S1-A04", "稳定确定顺序", A04确定顺序);
    输出验收项("SELFTEST-RUNNER-S1-A05", "失败不短路且恰好一次", A05失败不短路);
    输出验收项("SELFTEST-RUNNER-S1-A06", "冻结与一次执行", A06冻结与一次执行);
    输出验收项("SELFTEST-RUNNER-S1-A07", "汇总与退出码一致", A07汇总退出一致);

    const std::size_t 失败数量 = static_cast<std::size_t>(!A02合法登记)
        + static_cast<std::size_t>(!A03非法登记拒绝)
        + static_cast<std::size_t>(!A04确定顺序)
        + static_cast<std::size_t>(!A05失败不短路)
        + static_cast<std::size_t>(!A06冻结与一次执行)
        + static_cast<std::size_t>(!A07汇总退出一致);
    return {"SELFTEST-RUNNER-S1", "中央自检运行器合同", 失败数量 == 0, 6, 失败数量};
}

自检单元结果 运行阶段化自检总成合同自检() {
    const auto 输出验收项 = [](const char* 编号, const char* 名称, bool 通过) {
        std::cout << 编号 << ' ' << 名称 << ": " << (通过 ? "通过" : "失败") << '\n';
    };
    const auto 通过单元 = [](std::string 编号) {
        return [编号 = std::move(编号)] {
            return 自检单元结果{编号, 编号, true, 1, 0};
        };
    };

    const bool A02强类型阶段 =
        static_cast<std::uint8_t>(自检阶段::初始化闭环) == 1
        && static_cast<std::uint8_t>(自检阶段::线程协议) == 2
        && static_cast<std::uint8_t>(自检阶段::领域回执) == 3
        && static_cast<std::uint8_t>(自检阶段::最终回归) == 4;

    std::vector<int> 顺序观察;
    阶段化自检总成 顺序总成;
    const bool 顺序登记甲 = 顺序总成.登记自检(
        自检阶段::初始化闭环, 20, "顺序乙", "顺序乙", [&] {
            顺序观察.push_back(20);
            return 自检单元结果{"顺序乙", "顺序乙", true, 1, 0};
        });
    const bool 顺序登记乙 = 顺序总成.登记自检(
        自检阶段::初始化闭环, 10, "顺序甲", "顺序甲", [&] {
            顺序观察.push_back(10);
            return 自检单元结果{"顺序甲", "顺序甲", true, 1, 0};
        });
    const auto 顺序阶段一 = 顺序总成.运行阶段(自检阶段::初始化闭环);
    const bool 渐进登记 = 顺序总成.登记自检(
        自检阶段::线程协议, 10, "渐进", "渐进", 通过单元("渐进"));
    const auto 顺序阶段二 = 顺序总成.运行阶段(自检阶段::线程协议);
    const auto 逆序重跑 = 顺序总成.运行阶段(自检阶段::线程协议);
    const bool A03阶段顺序一次执行 = 顺序登记甲 && 顺序登记乙
        && 顺序阶段一.已执行 && 顺序阶段一.批次.总通过
        && 顺序观察 == std::vector<int>{10, 20}
        && 顺序阶段二.已执行 && !逆序重跑.已执行;
    const bool A04渐进登记 = 渐进登记 && 顺序阶段二.批次.总通过;

    阶段化自检总成 拒绝总成;
    const bool 首项登记 = 拒绝总成.登记自检(
        自检阶段::初始化闭环, 10, "唯一", "唯一", 通过单元("唯一"));
    const bool 全局重复拒绝 = !拒绝总成.登记自检(
        自检阶段::线程协议, 10, "唯一", "重复编号", 通过单元("重复编号"));
    const bool 同阶段顺序拒绝 = !拒绝总成.登记自检(
        自检阶段::初始化闭环, 10, "另一项", "重复顺序", 通过单元("重复顺序"));
    const bool 零顺序拒绝 = !拒绝总成.登记自检(
        自检阶段::初始化闭环, 0, "零", "零", 通过单元("零"));
    const bool 无效阶段拒绝 = !拒绝总成.登记自检(
        static_cast<自检阶段>(0), 20, "坏阶段", "坏阶段", 通过单元("坏阶段"));
    const auto 拒绝阶段一 = 拒绝总成.运行阶段(自检阶段::初始化闭环);
    const bool 已执行阶段登记拒绝 = !拒绝总成.登记自检(
        自检阶段::初始化闭环, 20, "迟到", "迟到", 通过单元("迟到"));
    const bool A05登记拒绝 = 首项登记 && 全局重复拒绝 && 同阶段顺序拒绝
        && 零顺序拒绝 && 无效阶段拒绝 && 拒绝阶段一.已执行 && 已执行阶段登记拒绝;

    std::size_t 失败后执行次数 = 0;
    阶段化自检总成 失败总成;
    (void)失败总成.登记自检(自检阶段::初始化闭环, 10, "阶段失败", "阶段失败", [&] {
        ++失败后执行次数;
        return 自检单元结果{"阶段失败", "阶段失败", false, 1, 1};
    });
    (void)失败总成.登记自检(自检阶段::线程协议, 10, "后续通过", "后续通过", [&] {
        ++失败后执行次数;
        return 自检单元结果{"后续通过", "后续通过", true, 1, 0};
    });
    const auto 失败阶段一 = 失败总成.运行阶段(自检阶段::初始化闭环);
    const auto 失败阶段二 = 失败总成.运行阶段(自检阶段::线程协议);
    const auto 失败聚合 = 失败总成.完成总成();
    const bool A06失败不阻断 = 失败阶段一.已执行 && 失败阶段二.已执行
        && 失败后执行次数 == 2 && 失败聚合.执行数量 == 2;

    std::size_t 未执行回调次数 = 0;
    阶段化自检总成 未完成总成;
    (void)未完成总成.登记自检(自检阶段::初始化闭环, 10, "已执行", "已执行", 通过单元("已执行"));
    (void)未完成总成.登记自检(自检阶段::线程协议, 10, "未执行", "未执行", [&] {
        ++未执行回调次数;
        return 自检单元结果{"未执行", "未执行", true, 1, 0};
    });
    (void)未完成总成.运行阶段(自检阶段::初始化闭环);
    const auto 未完成结果 = 未完成总成.完成总成();
    const bool A07不自动补跑 = !未完成结果.总成契约完整
        && !未完成结果.总通过 && 未执行回调次数 == 0;

    const bool A08聚合一致 = 失败聚合.登记数量 == 2
        && 失败聚合.执行数量 == 2
        && 失败聚合.通过数量 == 1
        && 失败聚合.失败数量 == 1
        && 失败聚合.失败编号组 == std::vector<std::string>{"阶段失败"}
        && 失败聚合.总成契约完整
        && !失败聚合.总通过
        && 阶段化自检总成::转换退出码(失败聚合) != 0;

    输出验收项("SELFTEST-RUNNER-S2-A02", "强类型四阶段", A02强类型阶段);
    输出验收项("SELFTEST-RUNNER-S2-A03", "阶段内顺序与单调一次执行", A03阶段顺序一次执行);
    输出验收项("SELFTEST-RUNNER-S2-A04", "后续阶段渐进登记", A04渐进登记);
    输出验收项("SELFTEST-RUNNER-S2-A05", "全局与阶段登记拒绝", A05登记拒绝);
    输出验收项("SELFTEST-RUNNER-S2-A06", "普通失败不阻断后续阶段", A06失败不阻断);
    输出验收项("SELFTEST-RUNNER-S2-A07", "完成总成不自动补跑", A07不自动补跑);
    输出验收项("SELFTEST-RUNNER-S2-A08", "最终聚合与退出码一致", A08聚合一致);

    const std::size_t 失败数量 = static_cast<std::size_t>(!A02强类型阶段)
        + static_cast<std::size_t>(!A03阶段顺序一次执行)
        + static_cast<std::size_t>(!A04渐进登记)
        + static_cast<std::size_t>(!A05登记拒绝)
        + static_cast<std::size_t>(!A06失败不阻断)
        + static_cast<std::size_t>(!A07不自动补跑)
        + static_cast<std::size_t>(!A08聚合一致);
    return {"SELFTEST-RUNNER-S2", "阶段化自检总成合同", 失败数量 == 0, 7, 失败数量};
}

}
