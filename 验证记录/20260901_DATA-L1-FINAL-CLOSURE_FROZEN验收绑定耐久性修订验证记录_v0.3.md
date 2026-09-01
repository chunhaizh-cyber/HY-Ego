# DATA-L1-FINAL-CLOSURE FROZEN验收绑定耐久性修订验证记录 v0.3

日期：2026-09-01

计划版本 / blob：`v0.3` / `c6de76077fb0c85939352f67c5df9160f6d917ca`

施工起点：`979ababbebaa3029975851e16f04728634473696`

隔离根：`D:\TEMP\海中鱼巣\DATA-L1-FINAL-CLOSURE\v0.3-C2-67c218f883eb4971b0a939fde3746e27`。该目录只消费 `git archive HEAD` 并覆盖两个授权检查器；临时 Git、manifest 变体、规范状态变体和运行输出均未进入项目 Git。

## 1. 语法、既有清单与 Git 自检

| 验证 | 结果 |
| --- | --- |
| 两个 Python 文件 `ast.parse` | `PASS` |
| `python .\tools\check_integration_acceptance.py --self-test` | exit `0`，包含 9 项 Git 耐久性正负例 |
| R1 `manifest_v0.2.json --verify-git --strict` | exit `0`；只证明旧 FAIL 清单结构与 `C1 <= recorded_remote <= current origin/main` 绑定仍合法，不升级其结论 |

Git 自检矩阵实际覆盖：

1. `A <= R <= origin/main`：PASS；
2. `origin/main` 从 R 推进到 G、记录远端仍为 R：PASS；
3. A 不存在：FAIL；
4. R 不存在：FAIL；
5. 远端分支不存在：FAIL；
6. A 不在 R 历史：FAIL；
7. R 不在当前远端历史：FAIL；
8. 输入路径不存在：FAIL；
9. 输入 blob 与 A 不同：FAIL。

## 2. FROZEN 状态与记录绑定矩阵

在隔离临时 Git 中，候选 A 为包含本批两个检查器的精确归档树，记录提交 R 包含具名 JSON 变体，后继本地提交 U 使 HEAD / 远端及未推送记录可独立构造。实际结果 24 / 24 符合预期：

| 编号 | 变体 | 预期 / 实际 |
| --- | --- | --- |
| P01 | PENDING，accepted / record 均 null | PASS / exit 0 |
| N01 | PENDING 提前绑定 | FAIL / exit 1 |
| N02 | accepted 与 JSON candidate 不同 | FAIL / exit 1 |
| N03 | 结论为事实失败 | FAIL / exit 1 |
| N04—N06 | 验收标识、服务名称、上级目标错误 | FAIL / exit 1 |
| N07—N08 | 仓库不是 `.`、远端分支不是 `origin/main` | FAIL / exit 1 |
| N09—N11 | record 未跟踪、工作树篡改、未进入 origin/main | FAIL / exit 1 |
| N12—N16 | `.md`、绝对路径、`..` 穿越、其它目标目录、损坏 JSON | FAIL / exit 1 |
| N17—N18 | 4080 当前状态仍 PENDING；仅历史附加正确 FROZEN | FAIL / exit 1 |
| N19—N20 | 规范目录当前 4080 行仍 PENDING；仅其它段落附加 FROZEN | FAIL / exit 1 |
| N21—N22 | 当前完整性段落缺 accepted commit / acceptance record | FAIL / exit 1 |
| P02 | accepted、PASS JSON、4080、目录、完整性段落一致，远端已推进 | PASS / exit 0 |

## 3. 正式精确树检查

直接在共享工作树运行冻结面会且只会命中异主根工程与 filters 各 9 个端到端测试 `ClCompile` WIP；这不是本切片候选结论。正式判断使用起点 HEAD 精确归档并只覆盖本计划两个授权检查器：

| 命令 | 结果 |
| --- | --- |
| `python <隔离根>\tools\check_l1_frozen_surface.py --strict` | `PASS`，43 项操作、5 个冻结文件与生产边界一致 |
| `python <隔离根>\tools\check_specs.py --strict` | `PASS`，115 / 115 |
| `git diff --check -- tools/check_integration_acceptance.py tools/check_l1_frozen_surface.py` | exit `0`；仅 Git 提示工作树 LF 后续暂存时按属性转为 CRLF，不是 diff 错误 |

取得发布租约后只暂存本计划四个白名单文件，staged 名单为两个检查器和两份 v0.3 专属记录；`git diff --cached --check` exit `0`。从该 staged tree 导出精确候选树后实际执行：

- `python tools/check_integration_acceptance.py --self-test`：PASS，9 / 9；
- `python tools/check_l1_frozen_surface.py --strict`：PASS；
- `python tools/check_specs.py --strict`：PASS，115 / 115。

上述结果绑定只含四文件 staged 变更的精确候选树，不消费当前共享工作树的异主工程字节。

## 4. 清理与未覆盖范围

本轮 Git 自检创建的逐轮目录已由工具自行清理。两次完整归档隔离根的递归清理在先用 `Resolve-Path` 核对其精确位于 `D:\TEMP\海中鱼巣\DATA-L1-FINAL-CLOSURE\` 后仍被主机安全策略拒绝，未改用跨 shell 或绕过策略删除：

- `v0.3-C2-67c218f883eb4971b0a939fde3746e27`：8276 个文件、196 个子目录；
- `v0.3-final-57b92c7cdb5246519c551688facf7799`：8266 个文件、195 个子目录。

两处残留只含当前项目归档、隔离 Git 与测试变体，不含凭据或个人材料，没有运行进程、端口、数据库、外设或共享锁占用；按 CODE-FILE-01 属于普通残留，不改变验证结果，也不阻断本切片发布。清理所有者为本计划执行切片，待主机允许对应 PowerShell 原生删除后清理。

未运行：R2 独立验收、G3 状态治理、真实跨线程压力并发、突然崩溃 / 强杀 / 断电、真实介质故障、资源耗尽和长时运行。DATA-L1 整体仍为 `PENDING_ACCEPTANCE`。
