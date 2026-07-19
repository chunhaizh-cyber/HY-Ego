# TASK-RESULT-STABILITY-S3 JY-482 任务分支完成核对与独立集成派发准备

日期：2026-07-19

对应：#314 / DQ-206 / v0.2 / WB-TASK-RESULT-01 / E227

## 1. 输入事实

```text
任务分支：origin/codex/314-task-result-idempotent-r1
任务提交：8111270126778230593a4bf20684bf91fa1e85bd
唯一父：dd9bf97297ee0b1b6da53b8a6135bfcb6c4aa274
提交数量：1
提交范围：精确三文件
任务 worktree / index：clean
本地 / 远端：0/0
产品进程：0
共享事件段目录：已释放
```

三文件为：

```text
海中鱼巣/领域/数据操作.需求任务方法.ixx
海中鱼巣/领域/自检.需求任务方法分层.ixx
实施记录/20260719_TASK-RESULT-STABILITY-S3_同义并发结算版本漂移幂等归并代码实施_Codex断点清单.md
```

## 2. 实现核对

`提交需求结算` 不再在权威当前态读回前直接返回版本漂移。版本漂移和并发占用复用同一个正式结算匹配器，读取当前需求、来源任务和正式结算后按以下三分支收口：

```text
无当前正式结算 -> 版本漂移保持版本漂移
完整同义 -> 幂等读回
完整异义 -> 幂等冲突
```

SERVICE-DATA-S5 的同义专项只在局部扩大到100路，固定1提交、99幂等、0其它；异义仍为8路和原语义。A34人读标签同步为“百路”，未新增公开ABI、机器字段、匹配器、重试或阶段号。

## 3. 任务级证据

```text
Debug x64 Rebuild：exit0，0 warning，0 error
Release x64 Rebuild：exit0，0 warning，0 error
Debug完整单轮：exit0；38/0、18/0、18/0、20/0
Release完整单轮：exit0；四组Debug专项输出0行
Debug严格连续：20/20；每轮exit0且四组失败0
连续摘要SHA-256：DA2628C6426B8B50F5922F94403A83D66AE67A1C79FA8AC4B6FB8283550E7079
strict / diff / 三文件范围 / 禁止范围 / 公开ABI / Release隔离：通过
```

E225第一次Debug单轮仍只作验证合同漂移证据，不混入v0.2完成样本。

## 4. 独立集成冻结

```text
集成批次：INT-WB-TASK-RESULT-01-R1
起点：E227发布后的origin/main精确提交，推送后现场读取
路径：C:\Users\zhchh\.codex\worktrees\integration-wb-task-result-01-r1\海中鱼巢
分支：codex/integration-wb-task-result-01-r1
唯一输入：8111270126778230593a4bf20684bf91fa1e85bd
目标角色：任务019f73ee-7d09-7a20-ad9f-1788ea62de23独立集成角色
```

创建前已确认目标路径、本地分支、远端同名分支和worktree登记均不存在。预留不等于派发；E227推送后必须用现场读取的完整起点发送正式消息，并读回集成身份核对。

## 5. 集成矩阵

1. 核对起点、唯一父、单提交、三文件、任务远端和两棵工作树clean。
2. 从精确E227起点创建集成worktree，不得复用任何旧路径或分支。
3. 无冲突cherry-pick `8111270`；任务三文件逐blob与输入一致。
4. Debug / Release x64 Rebuild均0 warning / 0 error。
5. Debug / Release完整自检均退出0；Release保持四组专项隔离。
6. Debug严格连续20/20，每轮完整日志先落盘，轮间2秒；四组精确38 / 18 / 18 / 20且失败0。
7. strict、diff、最终范围、禁止范围和公开ABI零变化通过。
8. 只新增唯一独立集成记录，随后非强制发布集成分支和WT-MAIN main。

完整 / 连续自检期间独占 `D:\海中鱼巣\日志\事件段\自检`；其它任务不得并发运行。首异常立即停止、释放目录、确认进程0，不得修改业务逻辑或测试合同消冲突。

## 6. 后继裁决

集成窗口成功发布后状态只到“已集成待设计同步”。设计窗口必须读取正式集成记录、代码和Git状态，归档#314后才能从新main生成#310新基线重验。

禁止：

```text
直接集成旧0890464 WIP
把任务分支完成写成#314已完成
在集成窗口生成#310计划或解除#305门控
复用旧WB-P0B-02集成候选身份
```
