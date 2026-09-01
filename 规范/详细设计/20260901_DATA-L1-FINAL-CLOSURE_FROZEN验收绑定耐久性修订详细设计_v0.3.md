# DATA-L1-FINAL-CLOSURE FROZEN验收绑定耐久性修订详细设计 v0.3

日期：2026-09-01

状态：执行后具名退回修订设计；设计包发布并精确登记后可执行

## 1. 修订来源与裁决

`DATA-L1-FINAL-CLOSURE` v0.2 已形成生产目标隔离候选：

```text
C1 = 9b1649d91cacc1a9353bea5bddb161ebda510367
main == origin/main == C1（本次设计扫描时）
冻结公开操作 = 43 项
冻结源码 = 5 个文件
整体状态 = PENDING_ACCEPTANCE
```

当前独立复验以 C1 的 `git archive` 为唯一代码输入；其最终记录提交暂记为：

```text
R1_RECORD_COMMIT = 3e2e23be9a79d192618a20076603851af5de7297
R1_MANIFEST_PATH = 验证记录/集成验收/DATA-L1-FINAL-CLOSURE/manifest_v0.2.json
R1_RESULT = INTEGRATION-ACCEPTANCE-FAIL / 事实失败
```

R1 无论最终为 PASS 还是 FAIL，都只证明 C1。当前代码另存在两个会使第二次治理发布不耐久或失配的检查器缺陷：

1. `tools/check_integration_acceptance.py::验证_git()` 要求验收清单中的历史 `基线.远端提交` 永远等于当前 `origin/main` tip。验收记录提交、检查器修复提交或最终治理提交一旦推进远端，历史精确候选的合法验收会自动失效。
2. `tools/check_l1_frozen_surface.py::检查状态()` 只运行验收清单检查器，没有强制冻结清单的 `accepted_commit` 等于验收清单的 `基线.候选提交`；`检查规范与生产边界()` 又把规范目录摘要固定为“目标合同 FROZEN，整体 PENDING_ACCEPTANCE”，无法对最终 FROZEN 状态做精确同步。

上述问题属于 v0.1 原目标“验收绑定后整体 FROZEN”的门禁耐久性漂移，不改变 4015、4070、4080 已冻结的 L1 机器语义，也不要求修改任何 L1 DTO、服务、仓库、公开操作、持久格式或黑盒专项。

## 2. 目标与完成条件

本修订只建立从精确候选、独立验收记录到最终 FROZEN 治理提交的稳定绑定。完成必须同时满足：

1. 验收候选提交存在，并被验收清单记录的远端分支当前历史包含；
2. 远端分支可以在验收记录、检查器修复和最终治理发布后继续前进，不要求候选或记录远端等于当前远端 tip；
3. 验收清单的 `基线.远端提交` 是可核验的验收时远端锚点，必须存在，并满足 `候选 <= 记录远端 <= 当前远端分支 tip`；不再要求记录远端永远等于当前 tip；
4. FROZEN 清单的 `accepted_commit` 精确等于所引用 JSON 验收清单的 `基线.候选提交`；
5. `acceptance_record` 只能指向仓库内 `验证记录/集成验收/DATA-L1-FINAL-CLOSURE/` 下的 JSON 清单，不能指向 Markdown、仓库外文件或其它目标记录；
6. FROZEN 引用的验收清单必须绑定 DATA-L1-FINAL-CLOSURE 身份、当前仓库、`origin/main` 和精确服务切片，不能把其它服务的 PASS manifest 复制进目标目录；
7. 验收 JSON 自身必须已进入当前 HEAD 与 `origin/main`，且工作树、HEAD、远端三者的 record blob 相等；未跟踪或本地篡改的记录不能成为冻结依据；
8. FROZEN 引用的验收清单必须明确 `结论.类型 = 服务验收通过`；字段完整但结论为 `事实失败`、`环境阻断` 或其它较弱结论的旧记录不得成为冻结依据；
9. 4080 正文第 1 节与规范目录的当前摘要按清单状态精确匹配 `PENDING_ACCEPTANCE` 或 `FROZEN`，不能靠其它历史段落残留的旧短语通过；
10. 修改检查器后的精确候选 C2 重新执行完整独立验收，形成 R2；R1 不得替代 R2；
11. 最终治理提交 G3 只升级状态和退出计划，推送前、推送后冻结门禁都通过。

本修订不增加 L1 上层用途入口，不改变 43 项通用操作全集，不重做 4070 恢复审计裁决，不修改三类事实或格式 1 / 2 / 3 的业务矩阵。

## 3. 精确提交链与身份绑定

发布顺序冻结为：

```text
C1  = 9b1649d 生产目标隔离候选
R1  = 对 C1 的当前独立复验记录提交
D3  = 本 v0.3 详细设计与代码计划发布提交
I3  = 计划索引登记 v0.3 精确 plan blob 的提交
C2  = 两个检查器耐久性修复及专属施工/验证记录提交
R2  = 对 C2 的全矩阵独立验收记录提交
G3  = 4080 + 规范目录 + 冻结 manifest + 计划索引的最终治理提交
```

最终绑定必须是：

```text
tools/l1_frozen_surface_manifest.json.accepted_commit = C2
tools/l1_frozen_surface_manifest.json.acceptance_record
  = 验证记录/集成验收/DATA-L1-FINAL-CLOSURE/manifest_v0.3.json
manifest_v0.3.json.基线.候选提交 = C2
C2 是 origin/main 的祖先
R2 提交包含 manifest_v0.3.json
G3 推送后 origin/main 可以指向 G3，冻结门禁仍通过
```

不得把 `accepted_commit` 填为 C1、R1、R2 或 G3。R2 是证据发布提交，不是被验收代码候选；G3 是消费 R2 的状态治理提交，也不是验收候选。

## 4. 集成验收 Git 绑定修订

修改 `tools/check_integration_acceptance.py::验证_git()`，保持 CLI、JSON schema 和字段不变，冻结以下语义：

1. `基线.候选提交` 必须是本地存在的 commit；
2. `基线.远端提交` 必须是本地存在的 commit；
3. `基线.远端分支` 必须能被本地 Git 解析；
4. 候选必须是记录远端的祖先或等于记录远端；
5. 记录远端必须是当前远端分支 tip 的祖先或等于 tip；
6. 不再要求记录远端等于当前远端分支 tip，远端后来正常追加提交不是失败；
7. `基线.输入文件[]` 仍逐项用 `候选提交:路径` 解析并与记录 blob 精确比较；
8. 候选不存在、记录远端不存在、远端分支不存在、候选不被记录远端包含、记录远端不被当前远端包含、路径不存在或 blob 不同都失败。

`基线.远端提交` 表示验收开始时对远端 tip 的历史观察。机器事后证明的是记录远端存在、记录远端包含候选且当前远端仍包含记录远端；它不能证明该值当时确为 tip，因此既不能只校验 40 位文本，也不能用当前 tip 回等比较伪造“历史不变”。

允许在同文件新增私有 helper 或扩展 `自检()`，但不得增加新清单字段、放宽 `已推送=true`、允许 `使用未提交内容=true`，也不得降低任何结论升级门禁。

## 5. FROZEN 精确记录绑定修订

修改 `tools/check_l1_frozen_surface.py::检查状态()`，保持冻结 manifest schema 1 不变。状态规则固定为：

### 5.1 PENDING_ACCEPTANCE

- `accepted_commit` 和 `acceptance_record` 必须仍为 `null`；
- 不读取验收清单，不允许提前登记候选或记录。

### 5.2 FROZEN

1. `accepted_commit` 必须是 40 位小写十六进制提交；
2. `acceptance_record` 必须是仓库相对路径，规范化解析后仍位于仓库根内；
3. 路径必须位于 `验证记录/集成验收/DATA-L1-FINAL-CLOSURE/`，扩展名必须为 `.json`，目标文件必须存在；
4. JSON 必须可解析为对象，且 `基线.候选提交` 必须精确等于 `accepted_commit`；
5. JSON 目标身份必须精确等于：

```text
验收标识 = DATA-L1-FINAL-CLOSURE@<accepted_commit>
服务切片.名称 = DATA-L1 通用操作冻结面、三类事实持久恢复与旧格式兼容收口
服务切片.上级目标 = 冻结 L1 通用操作全集并使后续存在、场景、特征、需求、任务和方法应用只能组合 L1 能力
```

6. `基线.仓库` 必须是精确文本 `.`，由检查器相对当前仓库根解析；`基线.远端分支` 必须精确为 `origin/main`。禁止绝对外部仓库、其它相对仓库或任意本地 ref；
7. JSON 的 `结论.类型` 必须精确等于 `服务验收通过`；通用验收检查器允许合法保存 FAIL / BLOCKED 等历史清单并返回结构检查通过，L1 FROZEN 不能把“记录结构合法”误当成“验收结论 PASS”；
8. 记录文件必须已提交并推送：`HEAD:<acceptance_record>` 与 `origin/main:<acceptance_record>` 都必须存在，二者 blob 必须相等，且当前工作树文件的 Git blob 必须与二者相等；
9. 随后才调用：

```powershell
python .\tools\check_integration_acceptance.py `
  --manifest <acceptance_record> --verify-git --strict
```

10. 任一解析、路径、目标身份、仓库 / 分支、记录提交状态、候选相等性、结论类型或清单检查失败均使 FROZEN 失败；不能只用子进程退出 0 代替本地精确绑定和 PASS 结论检查。

FROZEN 检查不要求 `accepted_commit == HEAD`、`accepted_commit == origin/main tip` 或 `accepted_commit == 验收记录提交`。它只要求验收清单精确证明同一候选，且当前正式远端仍包含该候选。

## 6. 规范目录状态门禁修订

修改 `tools/check_l1_frozen_surface.py::检查规范与生产边界()` 或其必经私有 helper。不得再在全文中搜索固定短语“目标合同 FROZEN，整体 PENDING_ACCEPTANCE”。

检查器必须定位规范目录中编号为 4080 的当前表格行，且恰好一行。按冻结清单的 `data_l1_status` 形成唯一预期：

```text
PENDING_ACCEPTANCE -> 目标合同 FROZEN，整体 PENDING_ACCEPTANCE
FROZEN             -> 目标合同 FROZEN，整体 FROZEN
```

当前 4080 行必须包含对应预期并不得包含相反状态。把预期短语附加到历史段落、注释或其它编号行不能替代当前 4080 行。

检查器还必须定位“20260901 DATA-L1 最终收口”的当前完整性段落，恰好一段：

- PENDING 时，该段必须明确整体仍保持 `PENDING_ACCEPTANCE`；
- FROZEN 时，该段必须明确整体已升级为 `FROZEN`，并同时包含 manifest 的 `accepted_commit` 与 `acceptance_record`；
- 相反状态或候选/记录缺失时失败。

4080 正文检查不得使用全文 substring。检查器必须定位 `## 1. 定位与冻结状态` 到下一个二级标题之间的当前状态代码块，并要求下列两行各恰好一次：

```text
DATA-L1 通用操作全集 / 目标公开合同状态 = FROZEN
DATA-L1 整体能力状态 = <manifest状态>
```

保留当前 PENDING 行、再把正确 FROZEN 行附加到修订记录或其它历史段落必须失败。其它 4015、4070、有限 N v3、唯一仓库入口、许可拒绝和生产测试隔离门禁保持原样。

## 7. C2 代码执行范围

C2 只允许修改：

```text
tools/check_integration_acceptance.py
tools/check_l1_frozen_surface.py
施工记录/20260901_DATA-L1-FINAL-CLOSURE_FROZEN验收绑定耐久性修订施工记录_v0.3.md
验证记录/20260901_DATA-L1-FINAL-CLOSURE_FROZEN验收绑定耐久性修订验证记录_v0.3.md
```

C2 禁止修改：

- `tools/check_specs.py`、`tools/l1_frozen_surface_manifest.json`；
- 4080、规范目录、计划索引和其它规范 / 设计 / 计划；
- L1 核心、公共 DTO、服务、仓库、fixture、专项、生产工程和入口；
- R1 验收材料及既有 v0.1 / v0.2 记录。

检查器变更必须全部进入 C2。R2 开始后不得再改检查器；否则原 R2 立即失效，必须形成新候选并重新验收。

## 8. R2 独立验收合同

C2 提交并普通推送后，由集成验收与回归智能体以 C2 为唯一候选，使用 `git archive C2` 的干净树重新执行 DATA-L1 完整矩阵。R2 至少新增：

```text
验证记录/集成验收/DATA-L1-FINAL-CLOSURE/DATA-L1-FINAL-CLOSURE_独立集成验收记录_v0.3.md
验证记录/集成验收/DATA-L1-FINAL-CLOSURE/manifest_v0.3.json
验证记录/集成验收/DATA-L1-FINAL-CLOSURE/证据_v0.3/**
```

R2 manifest 必须绑定 C2、C2 中全部正式输入 blob、`基线.仓库="."`、`基线.远端分支="origin/main"` 和验收时观察到的远端提交；验收标识必须为 `DATA-L1-FINAL-CLOSURE@<C2>`，服务名称与上级目标必须等于第 5.2 节冻结文本。R2 只有结构化结论 `INTEGRATION-ACCEPTANCE-PASS` 才允许 G3；FAIL、DRIFT、BLOCKED 或任一必要项 NOT_RUN 都保持 PENDING。

R2 必须重跑 v0.2 的 43 项 / 五文件、三类事实、格式 1 / 2 / 3、幂等、拒绝、fail-closed、生产目标隔离、Debug / Release fresh Rebuild 与生产外统一 driver。另把第 10 节门禁耐久性矩阵作为检查器专项证据。R1 只保留为 C1 历史记录，不能复制结论或替代运行。

## 9. G3 最终治理发布

只有 R2 已提交、推送且 `manifest_v0.3.json` 对 C2 为 PASS 后，规范 / 计划索引所有者才取得 G3 切片。G3 只允许修改四个文件：

```text
规范/4080_子规范_DATA-L1通用操作全集与冻结门禁.md
规范/规范目录.md
tools/l1_frozen_surface_manifest.json
计划/计划索引.md
```

精确修改合同：

1. 4080 保持通用操作合同版本 `v1.0`、43 项操作和所有机器语义不变；把整体状态由 PENDING 升为 FROZEN，删除当前阶段的待验收解释，登记 C2 与 `manifest_v0.3.json`，并说明这是状态治理而非公开合同解冻或升版。
2. 规范目录的 4080 当前行改为“目标合同 FROZEN，整体 FROZEN”；DATA-L1 当前完整性段落改为已由 C2 + R2 绑定并经本次第二次治理发布 FROZEN。其它异主 WIP 原样保留；若同文件仍 dirty，只向 index 应用本节两个精确 hunk，不得整文件暂存。
3. 冻结 manifest 只修改：

```json
"data_l1_status": "FROZEN",
"accepted_commit": "<C2>",
"acceptance_record": "验证记录/集成验收/DATA-L1-FINAL-CLOSURE/manifest_v0.3.json"
```

4. 计划索引删除 `DATA-L1-FINAL-CLOSURE` 当前执行队列行，并在历史导航登记 v0.3 plan blob、C2、R2 记录路径和“整体 FROZEN / 已退出”。不得改写 v0.1—v0.3 计划正文或验收记录。

G3 不得修改两个检查器、验收记录或任何生产代码；否则被 R2 验收的候选不再等于最终检查器基线。

## 10. 验证矩阵

### 10.1 C2 正例与负例

所有变体在仓库外临时 Git 仓或精确候选副本执行，不修改正式工作树：

1. 候选不存在：集成验收检查失败；
2. 记录远端提交不存在：失败；
3. 远端分支不存在：失败；
4. 候选不被记录远端包含：失败；
5. 记录远端不被当前 `origin/main` 包含：失败；
6. `candidate <= recorded_remote <= origin/main` 且 `origin/main` 已推进到 R2 / G3：仍通过；
7. 输入路径不存在或 blob 与候选不符：失败；
8. `accepted_commit != manifest_v0.3.json.基线.候选提交`：冻结检查失败；
9. accepted 与 candidate 相等、记录结构合法，但 `结论.类型=事实失败` 或其它非 `服务验收通过`：失败；
10. 验收标识、服务名称或上级目标不是 DATA-L1-FINAL-CLOSURE 精确身份：失败；
11. `基线.仓库` 不是 `.` 或 `基线.远端分支` 不是 `origin/main`：失败；
12. record 未跟踪、只存在于工作树、工作树被篡改、HEAD 与 origin/main 的 record blob 不同：分别失败；
13. `acceptance_record` 为绝对路径、越出仓库、非 JSON、其它验收目标或损坏 JSON：失败；
14. manifest 状态 FROZEN，但 4080 第 1 节当前状态或规范目录 4080 行仍 PENDING：失败；
15. 把正确 FROZEN 状态只附加到 4080 修订记录或规范目录其它历史段落、当前状态仍错误：失败；
16. DATA-L1 当前完整性段落缺 C2 或 record：失败；
17. 候选、记录远端、当前远端、已提交推送的 PASS 记录、4080、目录和 manifest 精确一致：通过。

### 10.2 C2 必跑命令

```powershell
python .\tools\check_integration_acceptance.py --self-test
python .\tools\check_l1_frozen_surface.py --strict
python .\tools\check_specs.py --strict
git diff --cached --check
```

专属施工 / 验证记录必须列出每个负例的临时根、输入、退出码和预期错误；所有临时材料不得进入 Git。

### 10.3 G3 推送耐久性

G3 工作树、精确 staged tree、G3 提交形成后但推送前、普通推送后至少各核对相应阶段：

```powershell
python .\tools\check_l1_frozen_surface.py --strict
python .\tools\check_specs.py --strict
git diff --check
```

推送后必须从一个仓库外 fresh clone（具有真实 `origin/main` 和 `.git`，不使用 archive 代替 Git 绑定检查）再次运行两项检查；满足 `HEAD == origin/main == G3`、index 空，且检查器通过时所绑定的候选仍为 C2。不得让清单指向原工作区绝对路径，也不得把 G3 自身写入 `accepted_commit` 以取得通过。

## 11. 失败收口与声明边界

- R1 尚未发布：v0.3 设计可以形成草案，但不得发布、登记或占用 Git 租约。
- C2 负例不能稳定证明：保留 PENDING，不得删减负例或放宽路径 / 候选绑定。
- R2 非 PASS：不得执行 G3，不得把 R1 或旧 FAIL 记录改写为 PASS。
- G3 前远端前进：重新核对 C2 仍被 `origin/main` 包含、R2 输入 blob 与候选一致；不得 merge、rebase 或改 accepted_commit。
- 规范目录异主 WIP 与两个目标 hunk重叠：停止 G3 受影响切片并取得交接；不清理或吸收其它 WIP。

C2 最多声明“FROZEN 验收绑定门禁具备远端推进耐久性，仍待 C2 独立验收”；R2 PASS 最多声明“C2 已通过具名独立验收，仍待第二次治理发布”。只有 G3 推送后才能声明：

> DATA-L1 整体能力状态已 `FROZEN`；后续存在、场景、特征、状态、动态、因果、需求、任务、方法和动作应用只能组合冻结 L1 能力，不得反向修改 L1 公开合同。

该声明不证明上层应用完成，也不证明未执行的突然崩溃、断电、真实介质故障、资源耗尽、压力并发或长时运行。
