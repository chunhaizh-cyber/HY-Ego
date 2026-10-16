#include "自检.模拟学习.h"

#include "模拟学习.算法.h"
#include "../启动.程序入口.h"
#include "../启动选项.数据.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <new>
#include <set>
#include <stdexcept>
#include <string>

namespace 海中鱼巣 {
namespace {
using 状态 = 数值模拟操作状态;
using 整数 = std::int64_t;
using 输入 = std::vector<整数>;
constexpr auto 最小 = (std::numeric_limits<整数>::min)();
constexpr auto 最大 = (std::numeric_limits<整数>::max)();

void 要求(bool 成立, const char* 说明) {
    if (!成立) throw std::runtime_error(说明);
}

数值模拟样例结果 数值(std::initializer_list<整数> 值) {
    return {数值模拟样例状态::已求值, 值};
}

数值模拟推进结果 推到终态(数值模拟细分器& 器, const std::size_t& 调用数) {
    // 仅测试驱动的失控保护；超限必须FAIL，不能伪装成算法完成。
    for (std::size_t 步 = 0; 步 < 100000; ++步) {
        const auto 前 = 调用数;
        auto 结果 = 器.推进();
        要求(调用数 - 前 <= 1, "more than one callback per step");
        if (结果.状态 != 状态::就绪 && 结果.状态 != 状态::推进中) return 结果;
    }
    throw std::runtime_error("test step guard reached before terminal state");
}

数值模拟材料读取结果 检验完成(const 数值模拟细分请求& 请求,
    const 数值模拟求值函数& 函数, std::size_t 样例数, std::size_t 区域数, std::size_t 实际次数) {
    数值模拟细分器 器;
    std::size_t 调用数 = 0;
    std::set<输入> 已调;
    std::set<输入> 已交付;
    要求(器.初始化(请求, [&](const 输入& 值) {
        ++调用数;
        已调.insert(值);
        return 函数(值);
    }) == 状态::就绪, "initialization rejected");
    要求(调用数 == 0, "initialization invoked callback");
    for (std::size_t 步 = 0; ; ++步) {
        要求(步 < 100000, "success case step guard");
        const auto 前 = 调用数;
        const auto 结果 = 器.推进();
        要求(调用数 - 前 <= 1, "multiple callbacks");
        要求(结果.本次求值.has_value() == (调用数 != 前), "actual evaluation delivery mismatch");
        if (结果.本次新样例) {
            要求(已交付.insert(结果.本次新样例->输入值组).second, "duplicate sample delivery");
        }
        if (结果.状态 == 状态::已完成) break;
        要求(结果.状态 == 状态::推进中, "unexpected terminal state");
    }
    const auto 材料 = 器.读取材料();
    const auto 进度 = 器.读取进度();
    要求(材料.状态 == 状态::已完成 && 进度.状态 == 状态::已完成, "read status");
    要求(材料.样例组.size() == 样例数 && 材料.完成区域组.size() == 区域数, "unexpected sample/region counts");
    要求(进度.已求值样例数 == 样例数 && 进度.已完成区域数 == 区域数, "progress counts");
    要求(调用数 == 实际次数 && 材料.实际求值组.size() == 实际次数
        && 进度.实际求值次数 == 实际次数 && 已交付 == 已调, "callback/delivery mismatch");
    要求(材料.未完善区域组.empty() && 材料.未知输入组.empty() && 材料.不合法输入组.empty()
        && 材料.同输入差异组.empty() && 进度.未完善区域数 == 0, "unexpected incomplete materials");
    std::set<输入> 读回;
    for (const auto& 样例 : 材料.样例组) {
        要求(读回.insert(样例.输入值组).second, "duplicate readback");
        要求(函数(样例.输入值组).输出值组 == 样例.输出值组, "wrong sample output");
    }
    要求(读回 == 已调, "sample identity mismatch");
    const auto 再推进 = 器.推进();
    要求(再推进.状态 == 状态::已完成 && !再推进.本次新样例 && !再推进.本次完成区域
        && !再推进.本次求值 && 调用数 == 实际次数, "terminal replay");
    return 材料;
}

void 检验失败(const 数值模拟求值函数& 函数, 状态 预期,
    std::optional<数值模拟样例状态> 失败原因 = {}) {
    数值模拟细分器 器;
    std::size_t 次数 = 0;
    要求(器.初始化({{{0, 8}}, 1}, [&](const 输入& 值) {
        ++次数;
        return 函数(值);
    }) == 状态::就绪, "failure-case initialization");
    const auto 结果 = 推到终态(器, 次数);
    要求(结果.状态 == 预期 && 结果.求值失败状态 == 失败原因 && 次数 == 1, "wrong failure mapping");
    const auto 重复 = 器.推进();
    要求(重复.状态 == 预期 && 重复.求值失败状态 == 失败原因 && 次数 == 1, "failed sample retried");
    要求(器.读取材料().完成区域组.empty() && 器.读取材料().实际求值组.empty()
        && 器.读取进度().实际求值次数 == 1, "engineering failure became normal material");
}

void 检验解析() {
    char 名称[] = "program";
    char 自检[] = "--self-test-simulation";
    char 无窗[] = "--headless";
    char 运行[] = "--runtime-context";
    char 未知[] = "--unknown";
    char* 参数[]{名称, 自检, 自检};
    要求(解析并验证启动选项(1, 参数).选项.模式 == 启动模式::普通控制面板, "default mode changed");
    const auto 单个 = 解析并验证启动选项(2, 参数);
    要求(单个.成功() && 单个.选项.模式 == 启动模式::数值模拟自检, "selftest parsing");
    要求(解析并验证启动选项(3, 参数).状态 == 启动选项解析状态::重复模式参数, "duplicate mode");
    for (auto 模式 : {无窗, 运行}) {
        参数[1] = 自检; 参数[2] = 模式;
        要求(解析并验证启动选项(3, 参数).状态 == 启动选项解析状态::模式组合冲突, "forward conflict");
        std::swap(参数[1], 参数[2]);
        要求(解析并验证启动选项(3, 参数).状态 == 启动选项解析状态::模式组合冲突, "reverse conflict");
    }
    参数[1] = 未知;
    要求(解析并验证启动选项(2, 参数).状态 == 启动选项解析状态::未知参数, "unknown argument");
}

void 检验初始化() {
    数值模拟细分器 器;
    要求(器.推进().状态 == 状态::尚未初始化 && 器.读取进度().状态 == 状态::尚未初始化
        && 器.读取材料().状态 == 状态::尚未初始化, "uninitialized object");
    std::size_t 次数 = 0;
    const 数值模拟求值函数 函数 = [&](const 输入& 值) { ++次数; return 数值({值[0]}); };
    for (const auto& 请求 : std::vector<数值模拟细分请求>{
        {{}, 1}, {{{0, 1}}, 0}, {{{1, 0}}, 1}, {{{最小, 0}}, 1}, {{{0, 最大}}, 1}}) {
        要求(器.初始化(请求, 函数) == 状态::输入不合法 && 次数 == 0, "invalid request accepted");
    }
    要求(器.初始化({{{0, 1}}, 1}, {}) == 状态::输入不合法, "empty callback accepted");
    要求(器.初始化({{{0, 8}}, 1}, 函数) == 状态::就绪, "valid request rejected");
    while (次数 == 0) (void)器.推进();
    const auto 暂停 = 器.读取进度();
    const auto 快照 = 器.读取材料();
    要求(器.初始化({{}, 1}, 函数) == 状态::输入不合法, "invalid reinit accepted");
    要求(器.读取进度().已求值样例数 == 暂停.已求值样例数 && 次数 == 1
        && 快照.样例组.size() == 1, "pause/read/rejection modified job");
    要求(推到终态(器, 次数).状态 == 状态::已完成 && 次数 == 6, "resume failed");
    数值模拟细分请求 不合法续学{{{0,8}},1};
    for (const auto& 域 : std::vector<数值模拟起始区域>{数值模拟起始区域{},
            {{{-1,2}}}, {{{0,9}}}, {{{2,1}}}, {{{0,2},{0,1}}}}) {
        不合法续学.已有区域组 = {域};
        要求(器.初始化(不合法续学, 函数) == 状态::输入不合法
            && 器.读取进度().状态 == 状态::已完成 && 次数 == 6, "invalid restart replaced prior job");
    }
    要求(器.初始化({{{4, 4}}, 1}, 函数) == 状态::就绪 && 次数 == 6
        && 器.读取进度().已求值样例数 == 0, "valid reinit did not replace job");
    要求(推到终态(器, 次数).状态 == 状态::已完成 && 次数 == 8, "new job failed");
}

void 检验重入() {
    数值模拟细分器 器;
    std::size_t 次数 = 0;
    要求(器.初始化({{{0, 8}}, 1}, [&](const 输入& 值) {
        ++次数;
        要求(器.读取进度().状态 == 状态::推进中, "reentrant progress");
        要求(器.读取材料().实际求值组.size() + 1 == 次数, "unfinished evaluation leaked");
        要求(器.推进().状态 == 状态::输入不合法, "reentrant advance accepted");
        要求(器.初始化({{{0, 0}}, 1}, [](const 输入&) { return 数值({0}); })
            == 状态::输入不合法, "reentrant init accepted");
        return 数值({值[0]});
    }) == 状态::就绪, "reentrant case initialization");
    要求(推到终态(器, 次数).状态 == 状态::已完成 && 次数 == 6, "outer job corrupted");
}

数值模拟材料读取结果 采样材料(const 数值模拟细分请求& 请求,
    const 数值模拟求值函数& 函数, std::size_t& 次数) {
    数值模拟细分器 器;
    要求(器.初始化(请求, [&](const 输入& 值) { ++次数; return 函数(值); }) == 状态::就绪,
        "material initialization");
    要求(推到终态(器, 次数).状态 == 状态::已完成, "normal material did not finish");
    const auto 材料 = 器.读取材料();
    const auto 进度 = 器.读取进度();
    要求(材料.状态 == 状态::已完成 && 材料.实际求值组.size() == 次数
        && 进度.实际求值次数 == 次数 && 进度.已求值样例数 == 材料.样例组.size()
        && 进度.未完善区域数 == 材料.未完善区域组.size(), "material progress mismatch");
    const auto 前 = 次数;
    要求(器.推进().状态 == 状态::已完成 && 次数 == 前, "normal terminal re-evaluated");
    return 材料;
}

void 检验未知与关系() {
    for (const auto 原因 : {数值模拟样例状态::输入不可求值, 数值模拟样例状态::结果未知}) {
        std::size_t 次数 = 0;
        const auto 材料 = 采样材料({{{0,2}},1}, [&](const 输入&) { return 数值模拟样例结果{原因,{}}; }, 次数);
        要求(次数 == 6 && 材料.未知输入组.size() == 3 && 材料.样例组.empty()
            && 材料.完成区域组.empty() && 材料.未完善区域组.size() == 1, "unknown collapsed to failure/completion");
    }
    for (const bool 首次未知 : {false, true}) {
        std::size_t 次数 = 0;
        const auto 材料 = 采样材料({{{0,2}},1}, [&](const 输入& x) {
            const bool 未知 = 首次未知 ? 次数 <= 3 : 次数 > 3;
            return 未知 ? 数值模拟样例结果{数值模拟样例状态::结果未知,{}} : 数值({x[0]});
        }, 次数);
        要求(次数 == 6 && 材料.样例组.size() == 3, "known history lost");
        要求(首次未知 ? 材料.未知输入组.empty() && 材料.完成区域组.size() == 1
            : 材料.未知输入组.size() == 3 && 材料.完成区域组.empty(), "latest unknown was masked by cache");
    }
    for (const bool 全部非法 : {false, true}) {
        std::size_t 次数 = 0, 原方法次数 = 0;
        const auto 材料 = 采样材料({{{0,2},{0,2}},1}, [&](const 输入& x) {
            if (全部非法 || x[0] + x[1] > 2)
                return 数值模拟样例结果{数值模拟样例状态::输入组合不合法,{}};
            ++原方法次数;
            return 数值({x[0] + x[1]});
        }, 次数);
        要求(次数 == 18 && 原方法次数 == (全部非法 ? 0U : 12U)
            && 材料.不合法输入组.size() == (全部非法 ? 9U : 3U)
            && 材料.未知输入组.empty() && 材料.完成区域组.empty()
            && 材料.未完善区域组.size() == 1, "sparse/invalid combinations invented complete lines");
    }
}

void 检验收尾新信息与差异() {
    std::size_t 次数 = 0;
    const auto 细分 = 采样材料({{{0,2}},1}, [&](const 输入& x) {
        if (次数 <= 3) return 数值模拟样例结果{数值模拟样例状态::结果未知,{}};
        return 数值({x[0] * x[0]});
    }, 次数);
    要求(次数 == 10 && 细分.完成区域组.size() == 2 && 细分.未知输入组.empty()
        && std::count_if(细分.实际求值组.begin(), 细分.实际求值组.end(),
            [](const auto& 项) { return 项.收尾复测; }) == 7, "new tail deviation skipped another full tail");
    for (const bool 中间未知 : {false, true}) {
        数值模拟细分请求 请求{{{0,4}},1};
        请求.已有区域组 = {{{{0,2}}}, {{{2,4}}}};
        std::map<整数, unsigned> 命中;
        次数 = 0;
        const auto 材料 = 采样材料(请求, [&](const 输入& x) {
            const auto 第几次 = ++命中[x[0]];
            if (x[0] == 2 && 第几次 == 2 && 中间未知)
                return 数值模拟样例结果{数值模拟样例状态::结果未知,{}};
            return 数值({x[0] == 2 && 第几次 == 3 ? 3 : x[0]});
        }, 次数);
        要求(次数 == 9 && 材料.同输入差异组.size() == 1
            && 材料.同输入差异组.front().输入值组 == 输入{2}
            && 材料.同输入差异组.front().不同输出组 == std::vector<输入>{{2},{3}}
            && 材料.完成区域组.empty() && 材料.未完善区域组.size() == 2
            && 材料.未知输入组.empty()
            && std::none_of(材料.样例组.begin(), 材料.样例组.end(),
                [](const auto& 项) { return 项.输入值组 == 输入{2}; }), "same-input difference lost/stayed in single samples");
    }
}

} // namespace

int 运行模拟学习算法自检() noexcept {
    std::size_t 通过 = 0, 失败 = 0;
    const auto 用例 = [&](const char* 名称, auto&& 函数) {
        try { 函数(); ++通过; std::cout << "PASS " << 名称 << '\n'; }
        catch (const std::exception& 异常) { ++失败; std::cerr << "FAIL " << 名称 << ": " << 异常.what() << '\n'; }
        catch (...) { ++失败; std::cerr << "FAIL " << 名称 << ": unknown exception\n"; }
    };
    try {
        const 数值模拟求值函数 恒等 = [](const 输入& 值) { return 数值({值[0]}); };
        用例("argument-parser", 检验解析);
        用例("initialization-pause-resume", 检验初始化);
        用例("single-point", [&] { 检验完成({{{3, 3}}, 1}, 恒等, 1, 1, 2); });
        用例("adjacent-points", [&] { 检验完成({{{3, 4}}, 1}, 恒等, 2, 1, 4); });
        用例("identity-three-points", [&] {
            const auto 材料 = 检验完成({{{0, 8}}, 1}, 恒等, 3, 1, 6);
            for (std::size_t i = 0; i < 3; ++i)
                要求(材料.样例组[i].输入值组[0] == static_cast<整数>(i * 4), "wrong three points");
        });
        用例("affine", [&] { 检验完成({{{0, 8}}, 1}, [](const 输入& x) { return 数值({2 * x[0] + 1}); }, 3, 1, 6); });
        用例("odd-width", [&] { 检验完成({{{0, 3}}, 1}, 恒等, 3, 1, 6); });
        用例("fractional-deviation", [&] { 检验完成({{{0, 3}}, 1}, [](const 输入& x) { return 数值({x[0] / 2}); }, 4, 3, 10); });
        用例("quadratic", [&] { 检验完成({{{0, 8}}, 1}, [](const 输入& x) { return 数值({x[0] * x[0]}); }, 9, 8, 25); });
        用例("midpoint-hidden-deviation", [&] {
            检验完成({{{0, 8}}, 1}, [](const 输入& x) { return 数值({x[0] * (x[0] - 4) * (x[0] - 8)}); }, 3, 1, 6);
        });
        用例("cartesian-bilinear", [&] { 检验完成({{{0, 4}, {0, 4}}, 1}, [](const 输入& x) { return 数值({x[0] * x[1]}); }, 9, 1, 18); });
        用例("cartesian-three-inputs", [&] { 检验完成({{{0,2},{0,2},{0,2}},1},
            [](const 输入& x) { return 数值({x[0]+x[1]+x[2]}); },27,1,54); });
        用例("multiple-outputs", [&] { 检验完成({{{0, 8}}, 2}, [](const 输入& x) { return 数值({x[0], x[0] * x[0]}); }, 9, 8, 25); });
        用例("wide-identity", [&] { 检验完成({{{最小 + 1, 最大 - 1}}, 1}, 恒等, 3, 1, 6); });
        用例("wide-constant", [&] { 检验完成({{{最小 + 1, 最大 - 1}}, 1}, [](const 输入&) { return 数值({7}); }, 3, 1, 6); });
        用例("wide-descending", [&] { 检验完成({{{最小 + 2, 最大 - 1}}, 1}, [](const 输入& x) { return 数值({-x[0]}); }, 3, 1, 6); });
        用例("existing-disconnected-and-overlapping-domains", [&] {
            数值模拟细分请求 请求{{{0,10}},1};
            请求.已有区域组 = {{{{0,2}}}, {{{8,10}}}, {{{0,2}}}};
            const auto 间断 = 检验完成(请求,恒等,6,2,12);
            要求(std::none_of(间断.样例组.begin(),间断.样例组.end(),
                [](const auto& 项) { return 项.输入值组[0] > 2 && 项.输入值组[0] < 8; }), "filled domain gap");
            请求.已有区域组 = {{{{0,2}}}, {{{2,4}}}};
            检验完成(请求,恒等,5,2,11);
        });
        用例("unknown-and-joint-constraints", 检验未知与关系);
        用例("tail-new-information-and-same-input-difference", 检验收尾新信息与差异);
        用例("failed-results", [&] {
            检验失败([](const 输入&) { return 数值模拟样例结果{}; },
                状态::样例求值失败, 数值模拟样例状态::求值失败);
        });
        用例("output-dimension", [&] { 检验失败([](const 输入&) { return 数值({}); }, 状态::输出维数不符); });
        用例("unknown-with-output-is-engineering-error", [&] { 检验失败([](const 输入&) {
            return 数值模拟样例结果{数值模拟样例状态::结果未知,{0}};
        }, 状态::输出维数不符); });
        用例("infinite-output", [&] {
            for (auto 极值 : {最小, 最大}) 检验失败([&](const 输入&) { return 数值({极值}); }, 状态::输出包含无穷哨兵);
        });
        用例("callback-exception", [&] { 检验失败([](const 输入&) -> 数值模拟样例结果 { throw std::runtime_error("expected"); }, 状态::求值抛异常); });
        用例("callback-bad-alloc", [&] { 检验失败([](const 输入&) -> 数值模拟样例结果 { throw std::bad_alloc(); }, 状态::资源失败); });
        用例("reentrancy", 检验重入);
        std::cout << "SIMULATION_SELFTEST PASS=" << 通过 << " FAIL=" << 失败 << '\n';
        return 失败 == 0 ? 0 : 1;
    } catch (...) {
        return 1;
    }
}

} // namespace 海中鱼巣
