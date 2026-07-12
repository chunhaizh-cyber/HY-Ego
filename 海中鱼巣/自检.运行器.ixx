// 文件规则：中央自检运行器只管理测试登记、确定顺序、全量执行和值式汇总；不得写业务结构或启动生产设施。
module;

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
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

}
