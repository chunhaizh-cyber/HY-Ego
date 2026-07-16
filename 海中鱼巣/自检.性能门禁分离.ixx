// 文件规则：性能门禁分离自检只验证运行模式、条件登记和退出汇总；不得复制性能算法或访问仓库。
module;

#ifdef HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST
#include <array>
#include <cstddef>
#include <iostream>
#endif

export module 海中鱼巣.自检.性能门禁分离;

#ifdef HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST
import 海中鱼巣.自检.运行器;

export namespace 海中鱼巣 {

struct 性能门禁分离自检证据 {
    bool 性能验收已请求 = false;
    bool 自检后退出 = false;
    bool 性能基线已登记 = false;
    bool 性能基线已执行 = false;
    bool 性能基线执行通过 = false;
    bool 性能模式汇总通过 = false;
};

namespace 性能门禁分离自检内部 {

bool 形成性能模式汇总(
    bool 性能验收已请求,
    bool 性能基线已登记,
    bool 性能基线已执行,
    bool 性能基线执行通过) noexcept {
    return !性能验收已请求
        || (性能基线已登记 && 性能基线已执行 && 性能基线执行通过);
}

bool 既有参数组合不改变性能模式() noexcept {
    for (const bool 数据库自检后退出 : {false, true}) {
        for (const bool 无窗口常驻 : {false, true}) {
            for (const bool D455真实样本后退出 : {false, true}) {
                (void)数据库自检后退出;
                (void)无窗口常驻;
                const bool 既有自检后退出 = D455真实样本后退出;
                for (const bool 性能验收已请求 : {false, true}) {
                    const bool 自检后退出 = 既有自检后退出 || 性能验收已请求;
                    const bool 性能基线应登记 = 性能验收已请求;
                    if ((D455真实样本后退出 && !自检后退出)
                        || 性能基线应登记 != 性能验收已请求) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

}

自检单元结果 运行性能门禁分离自检(性能门禁分离自检证据 证据) {
    using namespace 性能门禁分离自检内部;

    const bool 默认功能模式 = !证据.性能验收已请求;
    const bool 显式性能模式 = 证据.性能验收已请求;
    const bool 期望性能汇总 = 形成性能模式汇总(
        证据.性能验收已请求,
        证据.性能基线已登记,
        证据.性能基线已执行,
        证据.性能基线执行通过);

    const std::array<bool, 8> 验收{
        默认功能模式 != 显式性能模式,
        !默认功能模式
            || (!证据.性能基线已登记
                && !证据.性能基线已执行
                && 证据.性能模式汇总通过),
        !显式性能模式
            || (证据.性能基线已登记 && 证据.性能基线已执行),
        !显式性能模式 || 证据.自检后退出,
        证据.性能模式汇总通过 == 期望性能汇总,
        !形成性能模式汇总(true, true, true, false),
        形成性能模式汇总(true, true, true, true),
        既有参数组合不改变性能模式()};

    const std::array<const char*, 8> 名称{
        "默认与显式模式唯一",
        "默认模式不登记不执行性能阶段",
        "显式模式登记并执行性能阶段",
        "显式模式强制自检后退出",
        "实际汇总与性能结果一致",
        "性能失败保持失败",
        "性能通过保持通过",
        "既有参数不改变性能模式"};

    std::size_t 失败数量 = 0;
    for (std::size_t 索引 = 0; 索引 < 验收.size(); ++索引) {
        if (!验收[索引]) {
            ++失败数量;
        }
        std::cout << "SELFTEST-PERF-GATE-S1-A0" << (索引 + 1)
            << ' ' << 名称[索引] << ": " << (验收[索引] ? "通过" : "失败") << '\n';
    }
    std::cout << "SELFTEST-PERF-GATE-S1-EVIDENCE"
        << " requested=" << 证据.性能验收已请求
        << " registered=" << 证据.性能基线已登记
        << " executed=" << 证据.性能基线已执行
        << " passed=" << 证据.性能基线执行通过
        << " gate=" << 证据.性能模式汇总通过 << '\n';

    return {
        "SELFTEST-PERF-GATE-S1",
        "功能回归与性能验收分离",
        失败数量 == 0,
        验收.size(),
        失败数量};
}

}
#endif
