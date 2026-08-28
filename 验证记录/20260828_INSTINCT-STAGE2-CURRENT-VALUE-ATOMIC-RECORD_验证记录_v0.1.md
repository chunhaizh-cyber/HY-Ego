# INSTINCT-STAGE2-CURRENT-VALUE-ATOMIC-RECORD 验证记录

日期：2026-08-29
计划：`INSTINCT-STAGE2-CURRENT-VALUE-ATOMIC-RECORD` v0.6
起点：`8a7696694652cfd62894fb1c357072298837825b`
结果提交：本记录与阶段二结果同提交形成；精确提交哈希以 Git 发布回传为准。

## 1. 环境与入口

- 编译器：Visual Studio 2022 Professional，MSVC v143 / 14.44.35207。
- Debug 输出：`D:\TEMP\海中鱼巣\INSTINCT-STAGE2-CURRENT-VALUE-ATOMIC-RECORD\final-debug`。
- Release 输出：`D:\TEMP\海中鱼巣\INSTINCT-STAGE2-CURRENT-VALUE-ATOMIC-RECORD\final-release`。
- V13 宏输出：`D:\TEMP\海中鱼巣\INSTINCT-STAGE2-CURRENT-VALUE-ATOMIC-RECORD\final-v13`。
- 普通专项：`driver\运行阶段二测试-final.exe`。
- V13 专项：`driver\运行阶段二测试-final-v13.exe`。
- ABI consumer：`driver\阶段二ABI消费者.cpp`，只 import `海中鱼巣.领域.服务.L2特征结构` 与 `海中鱼巣.领域.服务.L2状态动态原子发布` 并取得三个公开成员函数指针。
- 精确提交树：`D:\TEMP\海中鱼巣\INSTINCT-STAGE2-CURRENT-VALUE-ATOMIC-RECORD\commit-tree-20260829-02`；Debug、Release 与 V13 各自使用独立输出根。

## 2. 构建与静态门禁

| 门禁 | 结果 | 边界 |
| --- | --- | --- |
| 精确提交树 v143 Debug 根工程 Rebuild | PASS，exit 0 | 只消费 index 展开的提交树。 |
| 精确提交树 v143 Release 根工程 Rebuild | PASS，exit 0 | 同上。 |
| 精确提交树 V13 宏 Debug Rebuild | PASS，exit 0 | 通过 `ForceImportBeforeCppTargets` 注入验证宏，独立输出。 |
| 精确提交树 ABI consumer 编译 | PASS，exit 0 | 手工 consumer 只 import 两个公开模块；C5050 为手工 consumer 与 Debug IFC 的环境警告，无错误。 |
| `rg INSTINCT-STAGE2-DIAG 海中鱼巣` | PASS，零命中 | 临时诊断已退出生产和测试源码；验证记录中的命令文字不属于源码命中。 |
| `git diff --check` | PASS | 仅输出既有 LF→CRLF 工作树提示。 |
| `python .\tools\check_specs.py --strict` | PASS，116/116 | 证明正式规范目录一致，不证明代码业务闭环。 |
| `git diff --cached --check` | PASS | 精确 index 只覆盖阶段二提交树。 |

## 3. 运行矩阵

| 编号 | 结果 | 机器证据 |
| --- | --- | --- |
| V01 | PASS | 普通应用唯一构造和 getter。 |
| V01A | PASS | 同一自我可同时承担需求所属存在与目标宿主。 |
| V02 | PASS | legacy 基线、新状态和迁移动能一次形成。 |
| V03 | PASS | 关系式前态退出并建立唯一新当前选择。 |
| V04 | PASS | 无变化零新增值/状态/动态/事实代次。 |
| V05 | NOT_RUN | 没有正式生产方法动作 fixture；不得伪造来源方法。 |
| V06 | PASS | 无来源方法时不形成动作致变动态。 |
| V07 | PASS | 精确重复返回首次 G1 且零新代次。 |
| V08 | PASS | 同组合键异请求返回幂等冲突。 |
| V09 | PASS | 新键旧 G0 返回当前性漂移且零写。 |
| V10 | PASS | 旧值字段错配结构化阻断且零写。 |
| V11 | PASS | 坏上下文 / 前状态上下文错配阻断。 |
| V12 | PASS | 三 owner 任一正式引用缺失即阻断。 |
| V13 | PASS | 宏配置下提交后首次读回失败返回已可能发布，只有原请求重放收束为精确重复。 |
| V14 | PASS | 写结果与独立历史读回逐字段一致。 |
| V15 | PASS | 三状态连续迁移、末态唯一当前、全部历史稳定可读。 |
| V16 | PASS | 第二普通应用上下文恢复固定登记和本能根，并消费中性当前选择；未重跑系统根跨进程认领链。 |
| V16A 主路径 | PASS | 隔离空库首次形成，持久恢复后只读收敛。 |
| V16A 故障子项 | NOT_RUN | 许可/资源/内部故障、首次账缺项和持久账同键异义需要专属 L1 恢复故障 fixture。 |
| V17 | PASS | 制造历史后继后当前值仍从特征实例当前槽读取，动态后状态不替代当前值。 |
| V18 | PASS | Debug/Release 两配置根工程 Rebuild。 |
| V19 | PASS | 独立 ABI consumer 可编译。 |

普通专项与 V13 宏专项均 exit 0。普通配置中的 V13 明确输出 `NOT_RUN：验证宏未启用`，不与宏专项的 V13 PASS 混淆。

## 4. 提交树复核与结论

- 第一份提交树复核识别到专项曾隐式依赖共享工作树的 L4 `普通应用配置` 删除增量；它在基线配置有效性处失败，未被计作 PASS。
- 精确 index 的阶段二测试随后显式填入既有 `形成普通应用任务筹办等待合同登记()` 结果，不修改生产配置 ABI；第二份提交树 Debug / Release / V13 Rebuild 均 exit 0。
- 第二份提交树普通专项 exit 0，V01、V01A、V02—V04、V06—V17、V16A 可运行项全部 PASS；V05 与 V16A 故障子项继续按冻结边界 `NOT_RUN`。
- 第二份提交树 V13 宏专项 exit 0，V13 实际 PASS；ABI consumer 编译 exit 0；`check_specs.py --strict` 为 116/116 PASS；`git diff --cached --check` PASS。

因此，精确提交树已经证明阶段二基础能力候选可发布。该结论仍不证明阶段三至阶段九或完整本能路线完成。
