# TASK-RESULT-STABILITY-S2 JY-462 集成归档与行尾无关诊断修订

日期：2026-07-19

状态：正式设计记录 / v0.2 已归档 / v0.3 重新待执行 / WT-313-R3 已预留待创建

## 1. 集成事实

```text
任务提交：17855611b0b0b7d157d06fec1a01df59e74d2f25
集成 cherry-pick：5d672204fed9b9d3ddf3fc2e5352218f31769e97
main 发布：74925a892dfd36ba3c28d99d92e7ddb2889e6e4f
任务记录 blob：2ad7577980cdd346fd2dc4cf66c087084557b9c4
范围：任务记录 + 唯一集成记录
验证：strict、diff、单父 / 单文件 / blob、禁止范围通过
```

主线发布占用已释放，WT-MAIN、集成 worktree 和 WT-313-R2 均 clean。本批纯文档，未构建、未运行、未占共享目录。

## 2. v0.2 归档

v0.2 的补丁和 Debug Rebuild 通过，但第 43 轮在产品进程启动前停止。无新有效轮、无产品退出码，历史有效证据仍只有 v0.1 的 1—42。该结果已独立集成，可以正式归档。

## 3. v0.3 裁决

状态文件是六键 `key=value` 材料，行尾不是语义。v0.3 固定：

```text
使用 Get-Content -Raw -Encoding UTF8 + ConvertFrom-StringData
按六个键精确读取，不用多行正则或手工移除 CR
产品启动前建立 LF / CRLF 两份同内容样本
两份样本由同一解析器逐键读回相等后才初始化正式状态
预检不计第 43 轮，不启动产品进程
第 43—60 轮仍为 18 次独立工具调用，每次一个进程
```

## 4. 新批次预留

```text
批次：WB-DIAG-05
worktree：WT-313-R3
路径：C:\Users\zhchh\.codex\worktrees\313-task-result-subcondition-r3\海中鱼巢
分支：codex/313-task-result-subcondition-r3
冻结候选：0890464414a8ffd5dfc71dcfc80641d9e4f18181
最终记录：实施记录/20260719_TASK-RESULT-STABILITY-S2_任务结果结算行尾无关逐轮可续接逐子条件证据诊断_Codex断点清单.md
```

创建前目标路径、本地分支、远端同名分支和 worktree 登记均不存在。E207 发布后才创建并核对；创建、S0 消息和共享时隙必须分别登记，预留不等于派发。

## 5. 边界

本次不修改生产代码、工程或入口，不修复 TASK-RESULT-S1，不解释候选稳定性，不恢复 #310，不解除 #305 / #301 / #306。流程图和业务详细设计仍由既有 TASK-RESULT-S1 产物定义；本轮只修订诊断编排。
