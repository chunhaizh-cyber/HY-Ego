# JY-537 #315 WT-315-R7 S2 临时观测接受与 S3 Debug 构建准备

日期：2026-07-20

## 1. 前置与实施

JY-536 / E281 已由 `3eb90d80c0c129b8542c4288c7e04c9be259247b` 非强制发布到 `origin/main`。发布后，设计窗口只授权替代唯一代理 `/root/wt315_r7_execution_v2` 在两份临时文件实现 S2，没有授权构建或运行。

## 2. S2 差异

```text
海中鱼巣/线程/路由.根需求筹办.ixx +47 / -0
海中鱼巣/线程/自检.根需求筹办.ixx +43 / -0
```

新增字段、赋值和 `REAL-GENERATION-LOOP-S1-R7-DIAG` 输出覆盖方法结果、动作入口、方法联合校验、调用前选择、召回调用与状态 / 候选 / 唯一建议、选择提交、调用后选择以及最终状态 / 选择 / 冻结。

静态门禁：

- 所有新增结构与赋值受 `HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST` 控制；自检输出位于既有完整宏块内。
- 移除宏块后，两份当前文件与 HEAD 对应文件逐字一致。
- A11 原表达式、A12 聚合表达式、16 项生产业务调用名称 / 数量 / 顺序不变。
- `git diff --check` 通过，diff 仅两文件，index 空。

## 3. 非 Git 证据

目录：`D:\海中鱼巣\日志\诊断\REAL-GENERATION-LOOP-S1-R7\E281-WT-315-R7`

```text
01-route-diagnostic.patch
SHA-256=27F1F7658C21778FDD3144603E5299958D1757016F20E9585C269FD2BAD7B75B

02-selftest-diagnostic.patch
SHA-256=7A6862645063D95FD120D13E707225C7116B43BCFBE7FA2C272955040A7B2FE5

03-s2-diagnostic-patch-manifest.txt
路由当前blob=8763e3aa2a28a33d7778c3e7bf7d6ea822c1bc12
自检当前blob=d32506dffe387bf9a69abbcedb237d9ae9997343
```

设计窗口独立验证两个 patch 与对应当前 `git diff` 逐行差异数均为 0，SHA、manifest、源码 blob、两文件状态和 index 空一致。

## 4. 裁决与 E282 后 S3 合同

正式接受 S2。E282 发布后只允许执行一次：

```powershell
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m
```

构建前后都必须确认产品 / 构建进程为 0、共享目录为空；构建日志保存到同一非 Git 诊断目录，要求退出 0、0 warning、0 error。不得运行产品、修改源码、写实施记录、暂存、提交或推送。

若构建失败，按计划首异常停止并恢复两份临时源码后回报，不得继续 S4。构建成功后保留两文件临时差异，等待设计窗口独立接受。设计窗口形成 JY-538 / E283 前不得授权 S4。

## 5. 六类证据

- 输入入口：E281 云端提交、S2两文件diff、宏等价检查和非Git patch证据。
- 核心处理：接受宏控临时观测，下一步只授权一次Debug构建。
- 结构承载：#315 计划、计划索引、任务队列、并行工作树登记表、当前状态、窗口交互记录和本设计记录。
- 输出变化：R7在`d36cb4c`上形成两文件未提交诊断差异；S3待执行。
- 调用路径：E281发布 -> S2两文件实现 -> 静态与patch证据 -> JY-537/E282 -> 一次Debug Rebuild。
- 验证方式：完整diff、宏边界、调用序列、A11/A12、patch SHA/逐行匹配、strict、diff和staged范围。

## 6. 完成边界

本记录不证明构建或运行通过，不授予 S4、产品运行、共享目录、实施记录、暂存、提交或推送权限。R7 不可集成，#316 继续依赖门控。
