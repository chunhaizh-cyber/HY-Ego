#include "模拟学习.算法.h"

#include <algorithm>
#include <bit>
#include <limits>
#include <map>
#include <new>
#include <utility>

namespace 海中鱼巣 {
namespace {

using 整数 = std::int64_t;
using 无符号整数 = std::uint64_t;
using 操作状态 = 数值模拟操作状态;

bool 是无穷哨兵(整数 值) noexcept {
    return 值 == (std::numeric_limits<整数>::min)()
        || 值 == (std::numeric_limits<整数>::max)();
}

无符号整数 有序坐标(整数 值) noexcept {
    return std::bit_cast<无符号整数>(值) ^ (无符号整数{1} << 63);
}

整数 区间中点(整数 左, 整数 右) noexcept {
    const auto 坐标 = 有序坐标(左) + (有序坐标(右) - 有序坐标(左)) / 2;
    return std::bit_cast<整数>(坐标 ^ (无符号整数{1} << 63));
}

struct 宽乘积 final {
    无符号整数 高位;
    无符号整数 低位;
    bool operator==(const 宽乘积&) const = default;
};

// 四个32位肢相乘，全部中间量均在U64内；适用于MSVC x64和Win32。
宽乘积 相乘(无符号整数 甲, 无符号整数 乙) noexcept {
    constexpr 无符号整数 掩码 = 0xffffffffULL;
    const auto 甲低 = 甲 & 掩码;
    const auto 甲高 = 甲 >> 32;
    const auto 乙低 = 乙 & 掩码;
    const auto 乙高 = 乙 >> 32;
    const auto 首积 = 甲低 * 乙低;
    const auto 次积 = 甲高 * 乙低 + (首积 >> 32);
    const auto 交积 = 甲低 * 乙高 + (次积 & 掩码);
    return {甲高 * 乙高 + (次积 >> 32) + (交积 >> 32),
        (交积 << 32) | (首积 & 掩码)};
}

无符号整数 差的幅度(整数 甲, 整数 乙) noexcept {
    return 甲 >= 乙 ? 有序坐标(甲) - 有序坐标(乙)
                    : 有序坐标(乙) - 有序坐标(甲);
}

bool 精确共线(整数 左, 整数 中, 整数 右,
    整数 左值, 整数 中值, 整数 右值) noexcept {
    // 不做除法，不饱和差值；0.5等非零偏差不能被整除截断。
    const auto 中差 = 差的幅度(中值, 左值);
    const auto 右差 = 差的幅度(右值, 左值);
    if (中差 == 0 || 右差 == 0) {
        return 中差 == 0 && 右差 == 0;
    }
    return (中值 < 左值) == (右值 < 左值)
        && 相乘(中差, 有序坐标(右) - 有序坐标(左))
            == 相乘(右差, 有序坐标(中) - 有序坐标(左));
}

struct 推进保护 final {
    bool& 标志;
    explicit 推进保护(bool& 值) noexcept : 标志(值) { 标志 = true; }
    ~推进保护() { 标志 = false; }
};

enum class 区域阶段 { 三点采样, 三点检查, 收束 };

struct 工作区域 final {
    std::vector<数值模拟输入区间> 区间组;
    std::vector<std::vector<整数>> 网格;
    std::vector<std::size_t> 游标;
    std::vector<bool> 偏差轴;
    区域阶段 阶段 = 区域阶段::三点采样;
    std::size_t 检查轴 = 0;
    bool 收尾 = false;
    std::size_t 叶序号 = 0;
    bool 未完善 = false;
    bool 有已知输入 = false;
    // 仅本区域本波次材料；旧已知值不能覆盖此次未知/非法结果。
    std::map<std::vector<整数>, 数值模拟样例结果> 点结果;

    void 准备网格() {
        网格.clear();
        网格.reserve(区间组.size());
        for (const auto& 区间 : 区间组) {
            const auto 中 = 区间中点(区间.下界, 区间.上界);
            std::vector<整数> 点{区间.下界, 中, 区间.上界};
            std::sort(点.begin(), 点.end());
            点.erase(std::unique(点.begin(), 点.end()), 点.end());
            网格.push_back(std::move(点));
        }
        游标.assign(区间组.size(), 0);
        检查轴 = 0;
    }

    std::vector<整数> 当前输入() const {
        std::vector<整数> 输入;
        输入.reserve(网格.size());
        for (std::size_t 轴 = 0; 轴 < 网格.size(); ++轴) {
            输入.push_back(网格[轴][游标[轴]]);
        }
        return 输入;
    }

    // 跳过轴==维数表示不跳过。混合基数游标不计算笛卡尔积大小。
    bool 下个坐标(std::size_t 跳过轴) noexcept {
        for (std::size_t 轴 = 游标.size(); 轴 != 0;) {
            --轴;
            if (轴 == 跳过轴) continue;
            if (++游标[轴] < 网格[轴].size()) return true;
            游标[轴] = 0;
        }
        return false;
    }
};

} // namespace

struct 数值模拟细分器::实现 final {
    std::size_t 输出维数 = 0;
    数值模拟求值函数 求值;
    操作状态 状态 = 操作状态::就绪;
    std::optional<数值模拟样例状态> 求值失败状态;
    std::map<std::vector<整数>, std::vector<整数>> 样例;
    std::map<std::vector<整数>, std::vector<std::vector<整数>>> 已知输出;
    std::map<std::vector<整数>, 数值模拟样例状态> 最近未知或非法;
    std::vector<数值模拟求值材料> 实际求值;
    std::size_t 实际求值次数 = 0;
    struct 叶区域 final {
        std::vector<数值模拟输入区间> 区间组;
        bool 完善 = false;
        bool 已收尾 = false;
    };
    std::vector<叶区域> 叶集合;
    std::vector<std::vector<数值模拟输入区间>> 待处理;
    std::optional<工作区域> 当前;
    bool 收尾中 = false;
    std::size_t 收尾游标 = 0;

    void 差异结束(const std::vector<整数>& 输入) {
        样例.erase(输入);
        for (auto& 叶 : 叶集合) {
            bool 包含 = true;
            for (std::size_t i = 0; i < 输入.size(); ++i)
                包含 = 包含 && 叶.区间组[i].下界 <= 输入[i] && 输入[i] <= 叶.区间组[i].上界;
            if (包含) { 叶.完善 = false; 叶.已收尾 = false; }
        }
        当前->未完善 = true;
        状态 = 操作状态::已完成;
    }

    void 采一个点(数值模拟推进结果& 返回) {
        auto& 区域 = *当前;
        auto 输入 = 区域.当前输入();
        数值模拟样例结果 结果;
        const auto 既有 = 样例.find(输入);
        if (!区域.收尾 && 既有 != 样例.end() && !最近未知或非法.contains(输入)) {
            结果 = {数值模拟样例状态::已求值, 既有->second};
        } else {
            ++实际求值次数;
            try {
                结果 = 求值(输入);
            } catch (const std::bad_alloc&) {
                状态 = 操作状态::资源失败;
                return;
            } catch (...) {
                状态 = 操作状态::求值抛异常;
                return;
            }
            const bool 已知 = 结果.状态 == 数值模拟样例状态::已求值;
            const bool 未知 = 结果.状态 == 数值模拟样例状态::输入不可求值
                || 结果.状态 == 数值模拟样例状态::结果未知;
            const bool 非法 = 结果.状态 == 数值模拟样例状态::输入组合不合法;
            if (!已知 && !未知 && !非法) {
                状态 = 操作状态::样例求值失败;
                求值失败状态 = 结果.状态;
                return;
            }
            if ((已知 && 结果.输出值组.size() != 输出维数)
                || (!已知 && !结果.输出值组.empty())) {
                状态 = 操作状态::输出维数不符;
                return;
            }
            if (std::any_of(结果.输出值组.begin(), 结果.输出值组.end(), 是无穷哨兵)) {
                状态 = 操作状态::输出包含无穷哨兵;
                return;
            }
            实际求值.push_back({输入, 结果, 区域.收尾});
            返回.本次求值 = 实际求值.back();
            if (已知) {
                最近未知或非法.erase(输入);
                auto& 输出组 = 已知输出[输入];
                const bool 首次已知 = 输出组.empty();
                if (std::find(输出组.begin(), 输出组.end(), 结果.输出值组) == 输出组.end())
                    输出组.push_back(结果.输出值组);
                if (输出组.size() > 1) { 差异结束(输入); return; }
                样例.insert_or_assign(输入, 结果.输出值组);
                if (首次已知) 返回.本次新样例 = 数值模拟样例{输入, 结果.输出值组};
            } else 最近未知或非法.insert_or_assign(输入, 结果.状态);
        }
        区域.有已知输入 = 区域.有已知输入 || 结果.状态 == 数值模拟样例状态::已求值;
        区域.未完善 = 区域.未完善 || 结果.状态 == 数值模拟样例状态::输入不可求值
            || 结果.状态 == 数值模拟样例状态::结果未知;
        区域.点结果.insert_or_assign(输入, std::move(结果));
        if (!区域.下个坐标(区域.游标.size())) {
            区域.阶段 = 区域阶段::三点检查;
        }
    }

    void 查一条线() {
        auto& 区域 = *当前;
        if (区域.检查轴 == 区域.区间组.size()) {
            区域.阶段 = 区域阶段::收束;
            return;
        }
        const auto 轴 = 区域.检查轴;
        const auto& 区间 = 区域.区间组[轴];
        const 整数 左 = 区间.下界;
        const 整数 右 = 区间.上界;
        const auto 中 = 区间中点(左, 右);
        if (左 < 中 && 中 < 右) {
            auto 输入 = 区域.当前输入();
            输入[轴] = 左;
            const auto 左项 = 区域.点结果.find(输入);
            输入[轴] = 中;
            const auto 中项 = 区域.点结果.find(输入);
            输入[轴] = 右;
            const auto 右项 = 区域.点结果.find(输入);
            if (左项 == 区域.点结果.end() || 中项 == 区域.点结果.end() || 右项 == 区域.点结果.end()
                || 左项->second.状态 != 数值模拟样例状态::已求值
                || 中项->second.状态 != 数值模拟样例状态::已求值
                || 右项->second.状态 != 数值模拟样例状态::已求值) {
                区域.未完善 = true;
            } else {
                for (std::size_t 分量 = 0; 分量 < 输出维数; ++分量) {
                    if (!精确共线(左, 中, 右, 左项->second.输出值组[分量],
                        中项->second.输出值组[分量], 右项->second.输出值组[分量])) {
                        区域.偏差轴[轴] = true;
                        break;
                    }
                }
            }
        }
        if (!区域.下个坐标(轴)) ++区域.检查轴;
    }

    void 收束区域(数值模拟推进结果& 返回) {
        const auto& 区域 = *当前;
        const auto 偏差 = std::find(区域.偏差轴.begin(), 区域.偏差轴.end(), true);
        if (偏差 == 区域.偏差轴.end()) {
            const bool 完善 = !区域.未完善 && 区域.有已知输入;
            if (区域.收尾) {
                auto& 叶 = 叶集合[区域.叶序号];
                叶.完善 = 完善; 叶.已收尾 = true;
                if (完善) 返回.本次完成区域 = 数值模拟完成区域{区域.区间组};
            } else {
                const auto 已有 = std::find_if(叶集合.begin(), 叶集合.end(),
                    [&](const auto& 叶) { return 叶.区间组 == 区域.区间组; });
                if (已有 == 叶集合.end()) 叶集合.push_back({区域.区间组, 完善, false});
            }
        } else {
            const auto 轴 = static_cast<std::size_t>(偏差 - 区域.偏差轴.begin());
            auto 左 = 区域.区间组;
            auto 右 = 区域.区间组;
            const auto 中 = 区间中点(左[轴].下界, 左[轴].上界);
            左[轴].上界 = 中;
            右[轴].下界 = 中;
            待处理.push_back(std::move(右));
            待处理.push_back(std::move(左));
            if (区域.收尾) {
                叶集合.erase(叶集合.begin() + static_cast<std::ptrdiff_t>(区域.叶序号));
                收尾中 = false;
                for (auto& 叶 : 叶集合) 叶.已收尾 = false;
            }
        }
        当前.reset();
    }

    void 前进一步(数值模拟推进结果& 返回) {
        状态 = 操作状态::推进中;
        if (!当前) {
            工作区域 新区域;
            if (!待处理.empty()) {
                新区域.区间组 = std::move(待处理.back());
                待处理.pop_back();
            } else {
                if (!收尾中) {
                    收尾中 = true; 收尾游标 = 0;
                    for (auto& 叶 : 叶集合) 叶.已收尾 = false;
                }
                if (收尾游标 == 叶集合.size()) { 状态 = 操作状态::已完成; return; }
                新区域.收尾 = true; 新区域.叶序号 = 收尾游标++;
                新区域.区间组 = 叶集合[新区域.叶序号].区间组;
            }
            新区域.偏差轴.assign(新区域.区间组.size(), false);
            新区域.准备网格();
            当前 = std::move(新区域);
            return;
        }
        switch (当前->阶段) {
        case 区域阶段::三点采样:
            采一个点(返回);
            break;
        case 区域阶段::三点检查:
            查一条线();
            break;
        case 区域阶段::收束:
            收束区域(返回);
            break;
        }
    }
};

数值模拟细分器::数值模拟细分器() noexcept = default;
数值模拟细分器::~数值模拟细分器() = default;

数值模拟操作状态 数值模拟细分器::初始化(
    const 数值模拟细分请求& 请求, const 数值模拟求值函数& 求值) noexcept {
    if (正在推进_ || 请求.输入区间组.empty() || 请求.输出维数 == 0 || !求值) {
        return 操作状态::输入不合法;
    }
    for (const auto& 区间 : 请求.输入区间组) {
        if (区间.下界 > 区间.上界 || 是无穷哨兵(区间.下界) || 是无穷哨兵(区间.上界)) {
            return 操作状态::输入不合法;
        }
    }
    try {
        std::vector<std::vector<数值模拟输入区间>> 起始域;
        if (请求.已有区域组.empty()) 起始域.push_back(请求.输入区间组);
        else for (const auto& 区域 : 请求.已有区域组) {
            if (区域.输入区间组.size() != 请求.输入区间组.size()) return 操作状态::输入不合法;
            for (std::size_t i = 0; i < 区域.输入区间组.size(); ++i) {
                const auto& 域 = 区域.输入区间组[i];
                if (域.下界 > 域.上界 || 域.下界 < 请求.输入区间组[i].下界
                    || 域.上界 > 请求.输入区间组[i].上界) return 操作状态::输入不合法;
            }
            if (std::find(起始域.begin(), 起始域.end(), 区域.输入区间组) == 起始域.end())
                起始域.push_back(区域.输入区间组);
        }
        auto 新实现 = std::make_unique<实现>();
        新实现->输出维数 = 请求.输出维数;
        新实现->求值 = 求值;
        新实现->待处理.assign(起始域.rbegin(), 起始域.rend());
        实现_.swap(新实现);
        return 操作状态::就绪;
    } catch (...) {
        return 操作状态::资源失败;
    }
}

数值模拟推进结果 数值模拟细分器::推进() noexcept {
    if (正在推进_) return {操作状态::输入不合法};
    if (!实现_) return {};
    if (实现_->状态 != 操作状态::就绪 && 实现_->状态 != 操作状态::推进中) {
        return {实现_->状态, {}, {}, 实现_->求值失败状态};
    }
    推进保护 保护(正在推进_);
    数值模拟推进结果 返回;
    try {
        实现_->前进一步(返回);
        返回.状态 = 实现_->状态;
        返回.求值失败状态 = 实现_->求值失败状态;
        return 返回;
    } catch (...) {
        // 求值后存储失败也锁定终态，不以重试隐式重复执行同一输入。
        实现_->状态 = 操作状态::资源失败;
        return {操作状态::资源失败};
    }
}

数值模拟进度 数值模拟细分器::读取进度() const noexcept {
    if (!实现_) return {};
    const auto 完成数 = static_cast<std::size_t>(std::count_if(实现_->叶集合.begin(), 实现_->叶集合.end(),
        [](const auto& 叶) { return 叶.已收尾 && 叶.完善; }));
    const auto 未完善数 = 实现_->叶集合.size() - 完成数 + 实现_->待处理.size()
        + (实现_->当前 && !实现_->当前->收尾 ? 1U : 0U);
    return {实现_->状态, 实现_->样例.size(), 完成数, 实现_->实际求值次数, 未完善数};
}

数值模拟材料读取结果 数值模拟细分器::读取材料() const noexcept {
    if (!实现_) return {};
    try {
        数值模拟材料读取结果 返回;
        返回.状态 = 实现_->状态;
        返回.样例组.reserve(实现_->样例.size());
        for (const auto& [输入, 输出] : 实现_->样例) {
            返回.样例组.push_back({输入, 输出});
        }
        for (const auto& 叶 : 实现_->叶集合) {
            if (叶.已收尾 && 叶.完善) 返回.完成区域组.push_back({叶.区间组});
            else 返回.未完善区域组.push_back({叶.区间组});
        }
        if (实现_->当前 && !实现_->当前->收尾) 返回.未完善区域组.push_back({实现_->当前->区间组});
        for (const auto& 域 : 实现_->待处理) 返回.未完善区域组.push_back({域});
        const auto 区域有序 = [](const auto& 左, const auto& 右) {
            return std::lexicographical_compare(左.输入区间组.begin(), 左.输入区间组.end(),
                右.输入区间组.begin(), 右.输入区间组.end(), [](const auto& a, const auto& b) {
                    return a.下界 < b.下界 || (a.下界 == b.下界 && a.上界 < b.上界);
                });
        };
        std::sort(返回.完成区域组.begin(), 返回.完成区域组.end(), 区域有序);
        std::sort(返回.未完善区域组.begin(), 返回.未完善区域组.end(), 区域有序);
        for (const auto& [输入, 原因] : 实现_->最近未知或非法) {
            if (原因 == 数值模拟样例状态::输入组合不合法) 返回.不合法输入组.push_back(输入);
            else 返回.未知输入组.push_back({输入, 原因});
        }
        for (const auto& [输入, 输出组] : 实现_->已知输出) if (输出组.size() > 1) {
            auto 有序输出 = 输出组;
            std::sort(有序输出.begin(), 有序输出.end());
            返回.同输入差异组.push_back({输入, std::move(有序输出)});
        }
        返回.实际求值组 = 实现_->实际求值;
        return 返回;
    } catch (...) {
        return {操作状态::资源失败};
    }
}

} // namespace 海中鱼巣
