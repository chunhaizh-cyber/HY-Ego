# L1-SIMPLIFY-P16-PRE2-DYNAMIC-SPEC-PROVIDER-EVIDENCE v0.1 验证记录

日期：2026-08-07

计划身份：`L1-SIMPLIFY-P16-PRE2-DYNAMIC-SPEC-PROVIDER-EVIDENCE v0.1`

plan blob：`497375f4131bad0d8cef08775a6c1ab604dac932`

基线：`0eebbf4a57c5d27714ced827a463edb033eac55c`

## 1. 正式门禁结果

| 门禁 | 结果 |
| --- | --- |
| Debug x64 系统临时目录隔离 `Rebuild` | PASS，退出0 |
| Debug 同一生产程序 `--self-test-exit` | PASS，退出0 |
| Debug 顺序390正式运行日志增量 | PASS，新增1条；48/48，失败0，未分类异常0 |
| Debug MSVC 模块扫描 | PASS，sources/modules=200/200，failed=0，拓扑200，循环0，外部需求0 |
| Release x64 系统临时目录隔离 `Rebuild` | PASS，退出0 |
| Release 同一生产程序 `--self-test-exit` | PASS，退出0 |
| Release 顺序390正式运行日志增量 | PASS，新增1条；48/48，失败0，未分类异常0 |
| Release MSVC 模块扫描 | PASS，sources/modules=200/200，failed=0，拓扑200，循环0，外部需求0 |
| `python .\tools\check_specs.py --strict` | PASS，110/110 |
| 三代码文件 `git diff --check` | PASS；仅预期 LF→CRLF 提示 |
| plan blob / Git | PASS；plan blob匹配，`HEAD==origin/main==0eebbf4a...`，index空 |
| 短时诊断清理 | PASS；`PRE2_DIAG`、`P14_DIAG`、临时 `iostream`、测试钩子零命中 |

构建使用 Visual Studio 18 Community `MSBuild.exe`，配置为 Debug/Release、x64、`Rebuild`、`/m:1`；输出和中间文件分别位于系统临时目录的独立配置子目录。模块扫描输出为系统临时目录 `hzy-pre2-final/debug-modules.json` 与 `release-modules.json`，未形成仓库产物。

## 2. 48项专项覆盖

原 P14 登记、规格、发布与读取断言保持；新增覆盖包括：

- `0x4C31445000000001ULL`、合同/规则版本1、直接调用1、去重 provider 数量1；
- 增加/减少成功同时返回规格与 provider 证据，等价/无前状态/上游非成功两项全空；
- P13规格、PRE1凭证、P14规格、G0/G1共同截止相等且非零；
- P14十项登记当前节点、PRE1传递节点/关系/值三类当前副本、合法历史副本；
- 身份/副本一一对应、严格排序、精确数量和同身份同副本去重；
- 坏 provider 身份、坏合同版本、错截止、缺 provider、缺副本、乱序、错种类、当前来源携带已退出副本均确定性拒绝为内部不一致且载荷全空；
- 既有 `0x44` 写集的一节点、八值、六关系、八属性槽变更、角色顺序及发布后 P14 专属读回保持通过。

正式日志仅由同一生产程序既有顺序390包装边界形成。两配置均人读观察到 `结果=通过；验收项数=48；失败项数=0；未分类异常=0`，并继续明确 `正式接线=不适用；正式读回=不适用`。日志不是机器权威事实。

## 3. 未证明范围

本验证只证明 P14 组件候选能在本切片矩阵中形成同轮、同截止、排序去重且绑定完整事实副本的强类型提供者证据。没有合法生产消费者、正式业务入口或正式独立读回；不计服务集成 PASS。未验证 P16/P15、摘要、发布装配、因果、崩溃/重启/断电、跨进程恢复或 SELF-GOV 闭环。
