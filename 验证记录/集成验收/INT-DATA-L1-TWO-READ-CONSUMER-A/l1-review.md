# INT-DATA-L1-TWO-READ-CONSUMER-A：L1 独立复核记录

日期：2026-09-13

复核者：`/root/world_tree_execute`（未编写本次复核的 L1 实现）

结论：**L1 子范围服务验收通过**。精确候选 `f5dda0a5ec920e8ebe72f441c813bf8151b2f946` 的原 43 项 DATA-L1 操作、`读取所有者范围空域`、`读取节点全部当前引用`，在独立 Git 快照、独立输出根中通过同一公开黑盒驱动的 Debug/Release 完整重建和运行。该结论不包含概念、世界、自我消费者，也不把 4080 整体状态升级为 `FROZEN`；整体仍为 `PENDING_ACCEPTANCE`，由根整合其它复核和正式工程证据。

## 1. 输入身份与独立性

- 候选提交：`f5dda0a5ec920e8ebe72f441c813bf8151b2f946`。
- 候选 tree：`6e55b4059ee56c93c8ffe64f16f9ecb5f304d5ca`。
- 开始时 `HEAD == origin/main == f5dda0a5ec920e8ebe72f441c813bf8151b2f946`。
- 源码取得方式：`git archive` 导出精确候选；构建没有读取工作区生产源码，也没有读取后续 B 的 WIP。
- 独占验证根：`D:/TEMP/海中鱼巣/INT-DATA-L1-TWO-READ-CONSUMER-A/01a06f6b-l1-review-a913`。
- 11 个 L1 输入文件逐项记录 Git blob、SHA256 与字节数，见 `l1-evidence.json` 和临时根 `l1-evidence.bundle.json`。
- 既有统一驱动只读取自 `D:/TEMP/海中鱼巣/WORLD-TREE-CONCEPT-FIRST-A/01a06f6b-l1-regression-9fb37420/revision-owner-value-01`。`regression.cpp`、工程文件和 45 操作覆盖表逐字节一致；`new_reads.cpp` 只有独占运行根替换。两个历史驱动从 `f8805cb3d0142aaaacce87b6351b4b7cac1c22c1` 的 Git blob `3e936dad8c444a40fe0e7d3304f898c4e4fbae87`、`9cdd6ffec7d3e5e8b6c596ee35d6590efb2143bb` 重新导出，只替换绝对运行根。三份机械差异均保存在临时根。

固定载荷格式 1/2 的八个样本直接来自候选 Git 树，SHA256 与其 `provenance.json` 完全一致。格式 1 来源提交为 `db65bc99c48310935509e3fdf05b22e71ae9b6b1`，活动载荷 SHA256 为 `eca97e5aa51146590097ed5c1d2ef397d839e1482b1cd8fd2b421b0f17f5330a`；格式 2 来源提交为 `ff2366d97b662cfc6c8cf16917c06d5da182bf4b`，活动载荷 SHA256 为 `cc3a4a3f99eb4865cb573aff461188aef4c6fa2fb7c19c27e3dec3a35661cfb6`。本次没有用当前编码器重造旧样本。

## 2. 实际命令与结果

构建工具为 `C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe`，MSVC `14.50.35717`、PlatformToolset `v145`、C++20、UTF-8、`bcrypt.lib`。两配置使用同一个 `l1_regression.vcxproj` 和同一组四个驱动源。

| 操作 | 实际命令摘要 | 退出码 | 结果 |
| --- | --- | ---: | --- |
| Debug 构建 | `MSBuild.exe l1_regression.vcxproj /t:Rebuild /m:1 /p:Configuration=Debug /p:Platform=x64` | 0 | 0 警告，0 错误 |
| Debug 运行 | `bin/Debug/l1_regression.exe` | 0 | 历史恢复 0；有限 N 0；两读口 0；322 断言 |
| Release 构建 | `MSBuild.exe l1_regression.vcxproj /t:Rebuild /m:1 /p:Configuration=Release /p:Platform=x64` | 0 | 0 警告，0 错误 |
| Release 运行 | `bin/Release/l1_regression.exe` | 0 | 历史恢复 0；有限 N 0；两读口 0；322 断言 |
| L1 冻结检查 | `py -3.14 -B tools/check_l1_frozen_surface.py --strict`（候选归档） | 0 | 45 项操作、5 个冻结文件一致 |
| 规范检查 | `py -3.14 -B tools/check_specs.py --strict`（候选归档） | 0 | 124 项目录项通过 |

完整命令、日志、退出码和二进制哈希位于独占验证根。Release 第一次构建因调用方前台等待上限未取得父进程退出码，虽日志完成成功，但不计作最终证据；随后在同一独占根重新执行完整 Rebuild，并以独立退出文件取得退出码 0，上表仅引用后一次。

## 3. 两个新增读取入口

验收对象和诊断责任如下：

- `L1所有者范围空域读取结果 L1事实基座服务::读取所有者范围空域(const L1所有者范围空域读取请求&) const noexcept`：服务声明/实现位于候选 `海中鱼巣/核心/服务.L1事实基座.ixx` 第 633—635、1106—1114 行；仓库实现从第 2611 行开始。诊断责任为“向上送出”，以结构化状态返回，不写日志。
- `L1节点当前引用读取结果 L1事实基座服务::读取节点全部当前引用(const L1节点当前引用读取请求&) const noexcept`：服务声明/实现位于同文件第 637—639、1116—1123 行；仓库实现从第 2748 行开始。诊断责任同样为“向上送出”。

`4015` 第 9.5 节要求的实际覆盖包括：未使用 owner 在无关写入后按 fresh G 仍为空；异键当前事实、历史、墓碑和 owner 幂等账均保护为非空；旧共享范围返回范围不支持；入口、预算和代次漂移失败均清空载荷且不改变 G；七类引用位置、同事实去重、跨 owner、未知关系类型、零引用；退出节点、物理墓碑和非节点身份；真实两线程使旧空域证明失效并使同 G 写入只有一个发布；受控销毁和重新打开后从权威事实恢复反向索引。Debug 与 Release 都执行同一断言集合。

## 4. 4080 九组统一矩阵

| 组 | 结果 | 实际证据 |
| ---: | --- | --- |
| 1 | PASS | 历史恢复与 R02：节点、关系、值创建/读取/替换/退出/历史/墓碑。 |
| 2 | PASS | 历史恢复、V02—V05、R01、C01：属性槽、引用闭包、代次、精确重复、同键异义、失败零变化和并发。 |
| 3 | PASS | O01、L01、历史恢复：owner 建立、唯一端口、重签、退出及 owner-scoped 当前/历史/组/一致投影。 |
| 4 | PASS | V01、V06：v1/v2/v3 与 N=2/3/4/8/255。 |
| 5 | PASS | 历史恢复、V07、P02：格式 3、当前/历史/槽/墓碑/幂等账和反向索引恢复。 |
| 6 | PASS | V08：候选内固定格式 1/2 样本恢复、旧账重放、继续 v3、再恢复。 |
| 7 | PASS | 历史损坏矩阵和 P03：未知清单/载荷格式及已知损坏分账。 |
| 8 | PASS | 历史损坏矩阵、P01、P03：清单/槽/长度/摘要/标签错误、真实第二进程占用、坏活动槽不回退完整非活动槽。 |
| 9 | L1 子范围 PASS | 45 项冻结检查、124 项规范检查、Debug/Release 完整 Rebuild 和同一驱动均通过；主工程与生产隔离由根整合。 |

## 5. ACCEPTANCE-01 八类场景

| 场景 | 结果 | 边界与证据 |
| --- | --- | --- |
| 成功 | PASS | E01、R01、V01、V07、V08 的完整结构化结果与公开权威读回。 |
| 精确重复 | PASS | V02、V06—V08、owner 重签；原身份/账/H 收敛，G 不增加。 |
| 合法拒绝 | PASS | E01、R01、E02、V03—V05；具名状态、空载荷及失败前后 G 一致。 |
| 外部失败 | 已执行存储失败 PASS | P01 真实第二进程占用、P03 未知格式和损坏；均不交付运行包。真实资源耗尽未运行。 |
| 并发 | PASS | C01 使用真实线程和 barrier；旧空域证明失效，同 G 竞争恰一发布，最终 G 只增加 1。 |
| 崩溃恢复 | NOT_RUN | P02 是受控析构后重新打开，不是 kill、崩溃、断电或介质故障。 |
| 正式读回 | PASS | 历史恢复、L01、P02、V08 使用公开当前/历史/首次账和新运行包读回。 |
| 正式构建 | L1 隔离目标 PASS | 两配置完整 Rebuild 通过；候选主工程和生产目标不携带自检由根的整合证据负责。 |

## 6. 未覆盖与结论边界

以下均为 `NOT_RUN`：真实进程 kill/崩溃、断电、物理介质故障、真实资源耗尽、长时可靠性/性能压力、私有状态或索引破坏、任意未登记旧布局。概念合法消费者、世界、自我和主工程验收不属于本 L1 子任务。

临时根结构化证据 `l1-evidence.bundle.json` 的 SHA256 为 `7f876e77848cf99eb40014d923fc72cb1acc3f100062f6d3403e82bda92d7220`；8691 项证据索引 `artifact-index.json` 的 SHA256 为 `91b0288b3ac7bbfc9197031552c4e332d78972ad816f3c043edef97ae22bf146`。两者、运行日志、二进制和受控数据根均已冻结在本记录第 1 节的独占验证根中。

因此，本记录只给出 `L1 子范围服务验收通过`。它不替代概念消费者复核、根的主工程/生产隔离证据或最终整合清单，也不自行修改 4080 的 `PENDING_ACCEPTANCE`、冻结表、计划或 Git。
