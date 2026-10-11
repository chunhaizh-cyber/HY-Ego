#include "自检.方法精确值域.h"

#include "../启动.程序入口.h"
#include "../启动选项.数据.h"
#include "../领域/新_方法类.h"
#include "../领域/新_场景类.h"
#include "../领域/概念_二次特征类.h"

#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

namespace 海中鱼巣 {
namespace {
using 区间 = 特征概念I64闭区间;
using 整数 = std::int64_t;

void 要求(bool 成立, const char* 说明) {
    if (!成立) throw std::runtime_error(说明);
}

// 真实服务依赖，仅在独立自检进程中构造；不初始化世界、线程或外设。
struct 自检服务 final {
    新_特征值类 特征值{全局基础数据集};
    概念_特征类 特征概念;
    新_特征类 特征{特征值, 特征概念};
    新_状态类 状态{特征};
    概念_存在类 存在概念{特征概念};
    新_存在类 存在{特征, 存在概念, 状态};
    概念_二次特征类 二次概念{特征概念, 特征值};
    概念_动态类 动态概念{特征概念, 特征值, 二次概念};
    新_动态类 动态{状态, 动态概念, 特征概念};
    稳定编码 场景根;
    新_场景类 场景{场景根, 存在, 状态, 动态};
    新_方法类 方法{存在, 场景, 特征值, 特征, 存在概念, 特征概念};
    稳定编码 数值类型;
    稳定编码 文本类型;
    稳定编码 数值子概念;
    稳定编码 单位;

    自检服务() {
        要求(特征值.初始化() && 特征概念.初始化(特征值)
            && 方法.初始化(), "service initialization");
        单位 = 全局基础数据集.新建节点();
        要求(有效(单位), "unit allocation");
        特征概念定义 数值定义;
        数值定义.值域 = std::vector<区间>{{-100, 100}};
        数值定义.单位 = 单位;
        数值类型 = 特征概念.建立或取得特征概念(数值定义, 特征值);
        数值定义.值域 = std::vector<区间>{{-10, 10}};
        数值子概念 = 特征概念.建立或取得特征概念(
            数值定义, 特征值, 数值类型);
        特征概念定义 文本定义;
        文本定义.材料类型 = 特征概念材料物理类型::UTF8字符串;
        文本定义.值域 = 特征概念全材料值域{};
        文本定义.比较规则 = 特征概念比较规则::完整材料精确;
        文本类型 = 特征概念.建立或取得特征概念(文本定义, 特征值);
        要求(有效(数值类型) && 有效(文本类型) && 有效(数值子概念),
            "feature concept allocation");
    }

    新方法知识建立请求 请求() {
        新方法知识建立请求 请求;
        请求.方法头.涉及特征类型概念组 = {数值类型};
        请求.方法头.可能结果概念组 = {数值类型};
        新方法参数规格 规格;
        规格.参数角色节点 = 全局基础数据集.新建节点();
        要求(有效(规格.参数角色节点), "role allocation");
        规格.特征规格组.push_back({数值类型, std::nullopt, {}});
        请求.参数规格组.push_back(规格);
        新方法条件项定义 条件;
        条件.参数角色节点 = 规格.参数角色节点;
        条件.特征类型概念节点 = 数值类型;
        条件.精确数值域 = 区间{2, 2};
        新方法结果项定义 结果;
        结果.参数角色节点 = 规格.参数角色节点;
        结果.特征类型概念节点 = 数值类型;
        结果.精确数值域 = 区间{3, 3};
        结果.主要结果 = true;
        auto 禁止 = 结果;
        禁止.精确数值域 = 区间{10, 12};
        禁止.主要结果 = false;
        新方法条件结果定义 配对;
        配对.条件.条件项组.push_back(条件);
        配对.结果.结果项组.push_back(结果);
        配对.结果.确认要求.禁止结果项组.push_back(禁止);
        请求.条件结果组.push_back(配对);
        return 请求;
    }

    void 建立并检查复制(const 新方法知识建立请求& 请求) {
        const auto 建立 = 方法.建立方法知识(请求);
        要求(建立.状态 == 新方法操作状态::已建立 && 建立.方法节点,
            "method creation");
        const auto 源 = 方法.获取方法(*建立.方法节点);
        要求(源 && 源->条件结果组.size() == 请求.条件结果组.size(),
            "method readback");
        const auto 校验材料 = [&](const 新方法信息& 信息) {
            要求(信息.方法头 == 请求.方法头
                && 信息.参数规格组 == 请求.参数规格组, "header/spec preservation");
            要求(信息.条件结果组.size() == 请求.条件结果组.size(), "pair count");
            for (std::size_t i = 0; i < 请求.条件结果组.size(); ++i) {
                要求(信息.条件结果组[i].条件 == 请求.条件结果组[i].条件
                    && 信息.条件结果组[i].结果 == 请求.条件结果组[i].结果,
                    "exact domain and confirmation preservation");
            }
        };
        校验材料(*源);
        const auto 再建 = 方法.建立方法知识(请求);
        要求(再建.状态 == 新方法操作状态::已找到 && 再建.方法节点 == 建立.方法节点,
            "equal material deduplication");
        const auto 复制 = 方法.复制方法(源->节点);
        要求(复制.状态 == 新方法复制状态::已复制 && 复制.方法实例节点,
            "method copy");
        const auto 副本 = 方法.获取方法(*复制.方法实例节点);
        要求(副本.has_value(), "copy readback");
        校验材料(*副本);
        要求(副本->来源方法节点 == 源->节点
            && 副本->实例根节点 == 副本->节点
            && 副本->节点 != 源->节点
            && 副本->方法头节点 != 源->方法头节点
            && 副本->方法虚拟存在节点 != 源->方法虚拟存在节点,
            "copy ownership and source");
        for (std::size_t i = 0; i < 源->条件结果组.size(); ++i) {
            const auto& 左 = 源->条件结果组[i];
            const auto& 右 = 副本->条件结果组[i];
            要求(左.配对节点 != 右.配对节点 && 左.条件节点 != 右.条件节点
                && 左.结果节点 != 右.结果节点, "internal pair identities");
        }
        要求(!复制.节点映射组.empty(), "copy mapping");
        for (const auto& 映射 : 复制.节点映射组) {
            要求(映射.源节点 != 映射.实例节点, "independent copied node");
        }
        要求(方法.获取方法(源->节点) == 源, "source unchanged by copy");
    }

    void 检查拒绝(const 新方法知识建立请求& 请求) {
        const auto 前 = 方法.查询候选方法({});
        要求(前.状态 == 新方法操作状态::已找到, "candidate snapshot");
        const auto 结果 = 方法.建立方法知识(请求);
        要求(结果.状态 == 新方法操作状态::条件结果不合法 && !结果.方法节点,
            "invalid domain must be rejected");
        const auto 后 = 方法.查询候选方法({});
        要求(后.状态 == 前.状态 && 后.方法节点组 == 前.方法节点组,
            "invalid request must not publish a method");
    }
};
} // namespace

int 运行方法精确值域自检() noexcept {
    std::size_t 通过 = 0, 失败 = 0;
    const auto 检查 = [&](const std::string& 名称, const auto& 函数) {
        try {
            函数();
            ++通过;
            std::cout << "PASS " << 名称 << '\n';
        } catch (const std::exception& 异常) {
            ++失败;
            std::cout << "FAIL " << 名称 << ": " << 异常.what() << '\n';
        } catch (...) {
            ++失败;
            std::cout << "FAIL " << 名称 << ": unknown exception\n";
        }
    };
    try {
        检查("startup mode", [] {
            char 程序[] = "self-test", 模式[] = "--self-test-method-domain";
            char 其它[] = "--self-test-simulation";
            char* 参数[] = {程序, 模式, 模式};
            const auto 单项 = 解析并验证启动选项(2, 参数);
            要求(单项.成功() && 单项.选项.模式 == 启动模式::方法精确值域自检,
                "mode accepted");
            要求(解析并验证启动选项(3, 参数).状态 == 启动选项解析状态::重复模式参数,
                "duplicate mode rejected");
            参数[2] = 其它;
            要求(解析并验证启动选项(3, 参数).状态 == 启动选项解析状态::模式组合冲突,
                "mixed mode rejected");
        });
        自检服务 服务;
        const auto 概念前 = 服务.特征概念.获取特征概念(服务.数值类型);
        const auto 子概念前 = 服务.特征概念.获取特征概念(服务.数值子概念);
        const auto 文本前 = 服务.特征概念.获取特征概念(服务.文本类型);
        要求(概念前 && 子概念前 && 文本前, "concept snapshots");
        检查("singleton and forbidden range roundtrip/copy", [&] {
            服务.建立并检查复制(服务.请求());
        });
        检查("finite intervals roundtrip/copy", [&] {
            auto 请求 = 服务.请求();
            auto& 配对 = 请求.条件结果组.front();
            配对.条件.条件项组.front().精确数值域 = 区间{-9, 8};
            配对.结果.结果项组.front().精确数值域 = 区间{-7, 9};
            服务.建立并检查复制(请求);
        });
        检查("legacy concept domains roundtrip/copy", [&] {
            auto 请求 = 服务.请求();
            auto& 配对 = 请求.条件结果组.front();
            auto 清除 = [&](auto& 项) {
                项.精确数值域.reset();
                项.特征值域概念节点 = 服务.数值子概念;
            };
            清除(配对.条件.条件项组.front());
            清除(配对.结果.结果项组.front());
            清除(配对.结果.确认要求.禁止结果项组.front());
            服务.建立并检查复制(请求);
        });
        检查("finite I64 endpoints preserved without unit conversion", [&] {
            auto 请求 = 服务.请求();
            请求.条件结果组.front().条件.条件项组.front().精确数值域 = 区间{
                (std::numeric_limits<整数>::min)() + 1,
                (std::numeric_limits<整数>::max)() - 1};
            服务.建立并检查复制(请求);
        });
        检查("domain participates in equality and deduplication", [&] {
            auto 请求 = 服务.请求();
            const auto 第一 = 服务.方法.建立方法知识(请求);
            auto 原条件 = 请求.条件结果组.front().条件;
            请求.条件结果组.front().条件.条件项组.front().精确数值域 = 区间{4, 4};
            要求(原条件 != 请求.条件结果组.front().条件, "domain ignored in equality");
            const auto 第二 = 服务.方法.建立方法知识(请求);
            要求(第一.状态 == 新方法操作状态::已建立
                && 第二.状态 == 新方法操作状态::已建立
                && 第一.方法节点 != 第二.方法节点, "different domains deduplicated");
        });
        检查("composite copy preserves child private domains", [&] {
            const auto 子请求 = 服务.请求();
            const auto 子 = 服务.方法.建立方法知识(子请求);
            要求(子.方法节点.has_value(), "child creation");
            auto 父请求 = 服务.请求();
            父请求.实现方式 = 新方法实现方式::组合;
            auto& 配对 = 父请求.条件结果组.front();
            配对.条件.条件项组.front().精确数值域.reset();
            配对.结果.结果项组.front().精确数值域.reset();
            配对.结果.确认要求.禁止结果项组.front().精确数值域.reset();
            const auto 父 = 服务.方法.建立方法知识(父请求);
            要求(父.方法节点.has_value(), "composite creation");
            const auto 步骤 = 服务.方法.添加组合步骤({*父.方法节点, *子.方法节点, 0});
            要求(步骤.节点.has_value(), "composite step");
            要求(服务.方法.设置组合入口步骤(*父.方法节点, *步骤.节点)
                == 新方法操作状态::无变化, "composite first step is already entry");
            const auto 源父 = 服务.方法.获取方法(*父.方法节点);
            const auto 源子 = 服务.方法.获取方法(*子.方法节点);
            const auto 复制 = 服务.方法.复制方法(*父.方法节点);
            要求(复制.状态 == 新方法复制状态::已复制 && 复制.方法实例节点,
                "composite copy");
            const auto 副本 = 服务.方法.获取方法(*复制.方法实例节点);
            要求(副本 && 副本->组合步骤组.size() == 1, "copied composite steps");
            const auto 子副本 = 服务.方法.获取方法(副本->组合步骤组.front().调用方法节点);
            要求(子副本 && 子副本->来源方法节点 == 子.方法节点
                && 子副本->实例根节点 == 副本->节点
                && 子副本->节点 != *子.方法节点
                && 子副本->条件结果组.size() == 1, "copied child identity");
            要求(子副本->条件结果组.front().条件 == 子请求.条件结果组.front().条件
                && 子副本->条件结果组.front().结果 == 子请求.条件结果组.front().结果,
                "child private domains lost in closure copy");
            要求(服务.方法.获取方法(*父.方法节点) == 源父
                && 服务.方法.获取方法(*子.方法节点) == 源子, "source graph changed");
        });
        const char* 位置名[] = {"condition", "result", "forbidden"};
        const char* 错误名[] = {"dual domain", "missing FT", "non-I64 FT",
            "inverted", "MIN sentinel", "MAX sentinel", "non-concept FT"};
        for (int 位置 = 0; 位置 < 3; ++位置) {
            for (int 错误 = 0; 错误 < 7; ++错误) {
                检查(std::string(位置名[位置]) + " rejects " + 错误名[错误], [&] {
                    auto 请求 = 服务.请求();
                    auto 修改 = [&](auto& 项) {
                        switch (错误) {
                        case 0: 项.特征值域概念节点 = 服务.数值子概念; break;
                        case 1: 项.特征类型概念节点.reset(); break;
                        case 2: 项.特征类型概念节点 = 服务.文本类型; break;
                        case 3: 项.精确数值域 = 区间{3, 2}; break;
                        case 4: 项.精确数值域 = 区间{(std::numeric_limits<整数>::min)(), 0}; break;
                        case 5: 项.精确数值域 = 区间{0, (std::numeric_limits<整数>::max)()}; break;
                        case 6: 项.特征类型概念节点 = 服务.单位; break;
                        }
                    };
                    auto& 配对 = 请求.条件结果组.front();
                    if (位置 == 0) 修改(配对.条件.条件项组.front());
                    else if (位置 == 1) 修改(配对.结果.结果项组.front());
                    else 修改(配对.结果.确认要求.禁止结果项组.front());
                    服务.检查拒绝(请求);
                });
            }
        }
        检查("referenced concepts unchanged", [&] {
            要求(服务.特征概念.获取特征概念(服务.数值类型) == 概念前
                && 服务.特征概念.获取特征概念(服务.数值子概念) == 子概念前
                && 服务.特征概念.获取特征概念(服务.文本类型) == 文本前,
                "method operation changed shared feature concepts");
        });
    } catch (const std::exception& 异常) {
        ++失败;
        std::cout << "FAIL initialization/driver: " << 异常.what() << '\n';
    } catch (...) {
        ++失败;
        std::cout << "FAIL initialization/driver: unknown exception\n";
    }
    std::cout << "SUMMARY " << 通过 << " PASS / " << 失败 << " FAIL\n";
    return 失败 == 0 ? 0 : 1;
}
} // namespace 海中鱼巣
