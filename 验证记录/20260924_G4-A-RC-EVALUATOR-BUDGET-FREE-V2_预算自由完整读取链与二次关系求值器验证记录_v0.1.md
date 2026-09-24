# G4-A 预算自由完整读取链与二次关系求值器验证记录

日期：2026-09-25

计划：`G4-A-RC-EVALUATOR-BUDGET-FREE-V2` v0.6

验证源码基线：本记录所在代码提交前工作区；正式起点`6f44a7d81897f85a77a3b597e95a50acb7eed19a`

隔离输出根：`D:\TEMP\海中鱼巣\G4-A-RC-EVALUATOR-BUDGET-FREE-V2\delete-success-20260924-r2`及`delete-success-20260924-final`

## 1. 静态核验

| 项目 | 结果 |
| --- | --- |
| `git diff --check` | PASS |
| 求值头源及专项中的`二次关系判断结果::成功`、四处`out.成功()`、三处专项调用 | 0命中，PASS |
| 求值头源中的预算、计量会话、最大候选、`数量预算不足`、直接值读取和世界结构预算 | 0命中，PASS |
| 根工程与四个专项工程的`计算.二次关系求值.cpp`登记 | 各恰一条，PASS |
| 比较快照来源 | `当前独立绑定`，PASS |
| K时间上下文 | 左时间核物理左B状态来源，右时间核物理右A状态来源，PASS |
| 非空子EC | 在递归、扫描或补造前固定`规则不支持`；运行结果三组载荷为空，PASS/静态分账 |
| 固定K成功后的当前K零命中、唯一异身份、多命中 | 合法写链不可达或只由损坏夹具可达；按v0.6以源码映射和唯一写门静态核验，未增加测试钩子或mock ABI |

## 2. 构建与运行

MSBuild：`C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe`

| 工程 / 模式 | Debug | Release |
| --- | --- | --- |
| `二次关系概念结构专项验证.vcxproj` Rebuild | PASS | PASS |
| 结构专项运行 | 176 PASS / 0 FAIL | 176 PASS / 0 FAIL |
| `本能双根二次关系概念初始化专项验证.vcxproj` Rebuild | PASS；最终删除后再复验PASS | PASS |
| 双根专项无参数运行 | 34 PASS / 0 FAIL；最终复验相同 | 34 PASS / 0 FAIL |
| 双根普通应用首次运行 | 10 PASS / 0 FAIL；最终复验相同 | 10 PASS / 0 FAIL |
| 双根跨进程恢复 | 4 PASS / 0 FAIL；最终复验相同 | 4 PASS / 0 FAIL |
| `本能根运行生产消费专项验证.vcxproj` Rebuild | PASS | PASS |
| 配置、首次阶段、恢复阶段、普通控制面板、无窗口五模式 | 全部PASS | 全部PASS |
| `世界树根启动专项验证.vcxproj` Rebuild | PASS | PASS |
| 世界树根专项运行 | 205 PASS / 0 FAIL | 205 PASS / 0 FAIL |
| 根工程`海中鱼巣.vcxproj`隔离Rebuild | PASS | PASS |

第一次Debug构建发现私有校验局部变量误用C++20关键字`concept`，更名后复验通过。一次回归运行曾以无参数错误调用需要具名模式的可执行文件，得到usage失败；按程序正式参数矩阵重跑五种模式后全部通过。这两项均为验证过程中的已纠正问题，不是最终基线失败。

## 3. 已证明与未证明

已证明：公开`二次关系判断结果::成功()`已经从源级合同和实际代码消费中删除；四入口仍保持结构化成功后置条件；当前代码在列明工程的Debug/Release下可编译链接，专项和回归在隔离目录运行通过。

未运行或未证明：正式集成验收、G4-A双根业务复核、根治理意图sink/manager、治理门开放、4213/4340、动态/因果概念、连续多轮治理，以及非空子EC递归。提交和推送也不把这些范围升级为完成。
