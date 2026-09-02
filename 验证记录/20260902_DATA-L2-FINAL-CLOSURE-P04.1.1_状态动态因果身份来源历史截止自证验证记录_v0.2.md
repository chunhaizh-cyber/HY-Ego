# DATA-L2-FINAL-CLOSURE-P04.1.1 状态、动态、因果身份来源历史截止自证验证记录 v0.2

日期：2026-09-02

## 验证身份与结论

- 计划：`DATA-L2-FINAL-CLOSURE-P04.1.1-HISTORICAL-CUTOFF-SELF-PROOF` v0.2，plan blob `daf6d480263ab24c4482ce8c7ebb4b6eed257b3d`。
- 执行 S0 基线：`87f6fa12e7081c77e62592dae6f3aff8f4955157`，当时及本轮构建前均为 `main == origin/main`、ahead/behind `0/0`、Git index 空。
- 结论：本叶的三项纯值谓词、三服务零修改与 Debug / Release 隔离编译通过；正式规范严格检查存在两项异主工程 WIP 引入的全局失败，不能表述为 strict 全绿。

## 纯值谓词矩阵（源码级）

三份 `成功()` 均含：非零结果头截止、正确节点族、以结果头截止验证身份来源完整性，以及封闭的当前 / 历史二分支。于是，在其余基础条件均有效时：

| 构造 | 期望 | 状态 / 动态 / 因果 |
| --- | --- | --- |
| `读取类别 == 当前` 且历史截止为 `0` | 可成功 | PASS：三个谓词均显式接受。 |
| `读取类别 == 当前` 且历史截止非 `0` | 不可成功 | PASS：当前分支为假。 |
| `读取类别 == 历史` 且历史截止等于结果头截止 | 可成功 | PASS：历史分支显式要求该等式；结果头截止已要求非零。 |
| `读取类别 == 历史` 且历史截止不等于结果头截止（含 `0`） | 不可成功 | PASS：历史分支为假。 |
| 未知读取类别 | 不可成功 | PASS：既非当前也非历史，封闭分支为假。 |

对三个函数逐项做了七项必备文本条件核对，结果均为 `REQUIRED_TERMS=7, MISSING=0`。这是纯值谓词的源码级证明；没有伪造私有损坏或新增独立验收工程。

## 构造回归与范围核对

| 检查 | 结果 | 依据 |
| --- | --- | --- |
| 三份服务当前 / 历史构造 | PASS（零本地修改） | `服务.L2状态结构.ixx`、`服务.L2动态结构.ixx`、`服务.L2因果结构.ixx` 的 `git diff --quiet` 均返回 `0`；S0 已核对其当前回显 `0`、历史回显结果头截止。 |
| 共同 DTO | PASS（零本地修改） | `海中鱼巣/领域/L2结构公共.数据.h` 的 `git diff --quiet` 返回 `0`。 |
| 允许范围五文件空白检查 | PASS | 三个数据头的 `git diff --check` 返回 `0`；两份新记录以 `git diff --no-index --check` 核对，均无空白错误。 |

## 隔离编译

| 命令 | 结果 | 隔离边界 |
| --- | --- | --- |
| `MSBuild.exe <archive-source>\海中鱼巣.vcxproj /t:Rebuild /m:1 /v:minimal /p:Configuration=Debug /p:Platform=x64` | PASS，退出码 `0`，`0` 警告、`0` 错误。 | 私有 `debug-x64-int` / `debug-x64-out`。 |
| `MSBuild.exe <archive-source>\海中鱼巣.vcxproj /t:Rebuild /m:1 /v:minimal /p:Configuration=Release /p:Platform=x64` | PASS，退出码 `0`，成功链接私有 `release-x64-out\海中鱼巣.exe`。 | 私有 `release-x64-int` / `release-x64-out`。 |

两次构建都使用 `D:\TEMP\海中鱼巣\DATA-L2-FINAL-CLOSURE-P04.1.1-HISTORICAL-CUTOFF-SELF-PROOF\20260902T-P0411-01` 下的正式 HEAD archive 快照，只注入本计划三份数据头；未使用主工作区工程 WIP 作为构建输入。

## 规范严格检查分账

`python .\tools\check_specs.py --strict` 返回 `1`：115 份目录项中有 2 个错误，均为 `tools/l1_frozen_surface_manifest.json` 报告生产工程和 filters 各登记 9 个端到端测试 `ClCompile` 项。相关 `海中鱼巣.vcxproj` 与 `海中鱼巣.vcxproj.filters` 为异主 WIP，且不在本计划允许范围；本计划没有修改它们。因此该全局检查失败被如实保留，不能归为本叶引入的规范错误，也不能升级为 strict PASS。

## 未覆盖范围

未运行跨进程恢复、TTL 清理、墓碑、完整状态 / 动态 / 因果业务矩阵、运行期业务验证或集成验收；未证明 P04.2、P08 或 `DATA-L2 FROZEN`。本记录只证明本计划列出的三项结果谓词收紧及所列编译 / 静态检查结果。
