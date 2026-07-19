# TASK-RESULT-STABILITY-S2 JY-466 集成归档与独立单轮脚本诊断修订

日期：2026-07-19

状态：正式设计记录 / v0.3 已归档 / v0.4 重新待执行 / WT-313-R4 已预留待创建

## 1. 集成事实

```text
任务提交：2ee669a24f78cd6dab694c15e63c2b390fa4ea41
集成 cherry-pick：5596af6bcff2831af014b4ac5e89e79edd990083
main 发布：7ae0efe7b97ce9ff9e4c21ff166f6def364271b3
任务记录 blob：a4e9e7c0c38413ce90e17453f69bf6b2ba05eedf
范围：任务记录 + 唯一集成记录
验证：strict、diff、单父 / 单文件 / blob、禁止范围通过
```

主线发布占用已释放，WT-MAIN、集成 worktree 和 WT-313-R3 均 clean。本批纯文档，未构建、未运行、未占共享目录。

## 2. v0.3 归档

v0.3 的临时补丁、Debug Rebuild、六键结构化解析和 LF / CRLF 双样本预检均通过。第 43 轮在实际 shell 和产品进程启动前，因外层工具 JavaScript 模板中的 PowerShell 制表符写法未转义而语法失败。无产品退出码、无轮日志、无新有效轮，历史证据仍只有 v0.1 的第 1—42 轮。

该异常已经由独立纯文档集成进入 main，可正式归档；不得解释为 TASK-RESULT-S1 新失败或 #310 因果。

## 3. v0.4 裁决

不再把完整 PowerShell 编排嵌入工具调用字符串。v0.4 固定：

```text
完整单轮逻辑写入非 Git run-one-round.ps1
外层调用只保留 powershell.exe -File、脚本绝对路径和 -Round N
脚本一次最多启动一个产品进程，不循环多个轮次
产品启动前先做 AST 语法零错误检查并冻结脚本 SHA-256
用同一脚本 -ValidateOnly 路径复核六键状态、LF / CRLF 和进程 0
43—60 仍为 18 次独立最小工具调用
首异常停止，不补跑，不现场修脚本后续跑
```

脚本、状态和原始日志只写 `D:\海中鱼巣\日志\诊断\TASK-RESULT-STABILITY-S2-R4`，不进入 Git。临时 C++ 观察合同、冻结 blob 和最终单记录范围不变。

## 4. 新批次预留

```text
批次：WB-DIAG-06
worktree：WT-313-R4
路径：C:\Users\zhchh\.codex\worktrees\313-task-result-subcondition-r4\海中鱼巢
分支：codex/313-task-result-subcondition-r4
冻结候选：0890464414a8ffd5dfc71dcfc80641d9e4f18181
最终记录：实施记录/20260719_TASK-RESULT-STABILITY-S2_任务结果结算独立单轮脚本可续接逐子条件证据诊断_Codex断点清单.md
```

创建前目标路径、本地分支、远端同名分支和 worktree 命中均为 0。E211 发布后才创建并核对；创建、S0 消息和共享时隙必须分别登记，预留不等于派发。

## 5. 边界

本次不修改生产代码、工程或入口，不修复 TASK-RESULT-S1，不解释候选稳定性，不恢复 #310，不解除 #305 / #301 / #306。流程图和业务详细设计仍由既有 TASK-RESULT-S1 产物定义；本轮只修订诊断编排。
