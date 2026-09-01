# DATA-L1-FINAL-CLOSURE FROZEN验收绑定耐久性修订代码实施计划 v0.3

日期：2026-09-01

计划身份：`DATA-L1-FINAL-CLOSURE`

修订版本：`v0.3`

目标登记状态：设计包发布并以精确 blob 登记后为 `可执行`

## 1. 退回来源、目标与完成条件

v0.2 代码结果 `9b1649d91cacc1a9353bea5bddb161ebda510367` 已移除生产测试入口和工程登记，当前独立复验 R1 正绑定该精确提交。执行后审查确认最终 FROZEN 门禁存在两个计划级漂移：

1. 集成验收检查器把历史 `基线.远端提交` 与当前远端 tip 强绑定，后继正常提交会让历史验收失效；
2. L1 冻结检查器没有强制 `accepted_commit` 等于验收 manifest 的候选，并把规范目录摘要写死为 PENDING。

R1 精确事实：

```text
R1_RECORD_COMMIT = 3e2e23be9a79d192618a20076603851af5de7297
R1_MANIFEST_PATH = 验证记录/集成验收/DATA-L1-FINAL-CLOSURE/manifest_v0.2.json
R1_RESULT = INTEGRATION-ACCEPTANCE-FAIL / 事实失败
```

本计划目标只修复两个检查器并形成 C2。完成必须同时满足：

1. 候选、记录远端和当前远端分支形成 `candidate <= recorded_remote <= current remote tip` 的可核验提交链；远端正常前进不使记录失效；
2. 输入 blob 继续绑定精确候选；
3. FROZEN 的 accepted commit、JSON 验收清单候选和 DATA-L1 记录路径精确一致；
4. FROZEN 只能消费身份、服务切片、仓库、远端分支都精确属于 DATA-L1-FINAL-CLOSURE 的记录，且 record 已在 HEAD / origin/main 提交推送、工作树字节未篡改；
5. FROZEN 只能消费 `结论.类型=服务验收通过` 的记录；结构合法的旧 FAIL / BLOCKED / 较弱结论记录必须拒绝；
6. 4080 第 1 节当前状态、规范目录 4080 当前行及当前完整性段落按 manifest 状态精确匹配；
7. 全部正负例和严格规范检查通过；
8. 形成、提交并推送 C2；整体仍为 `PENDING_ACCEPTANCE`；
9. 将 C2 交给独立验收形成 R2，R2 PASS 后才允许四文件 G3 治理发布。

## 2. 正式依据与施工基线

- `规范/4080_子规范_DATA-L1通用操作全集与冻结门禁.md` v1.0
- `规范/运行期服务导向代码验收与全函数入口巡检规范.md`
- `规范/详细设计/20260901_DATA-L1-FINAL-CLOSURE_通用操作冻结与统一黑盒收口详细设计_v0.1.md`
- `规范/详细设计/20260901_DATA-L1-FINAL-CLOSURE_生产目标测试隔离修订详细设计_v0.2.md`
- `规范/详细设计/20260901_DATA-L1-FINAL-CLOSURE_FROZEN验收绑定耐久性修订详细设计_v0.3.md`
- `tools/l1_frozen_surface_manifest.json` schema 1
- R1 验收记录提交 `3e2e23be9a79d192618a20076603851af5de7297`

计划支撑扫描代码基线为 `main@9b1649d91cacc1a9353bea5bddb161ebda510367`，与 `origin/main` 一致；正式执行基线必须是 R1 与本设计 / 索引发布后的最新 `main`。v0.1 / v0.2 计划保留为历史，索引登记 v0.3 后执行只能使用本计划精确 blob。

本计划不依赖新的 L1 provider。R1 的结果不授权复用为 C2 验收；它只是冻结 C1 的历史复验事实和发布顺序。

## 3. 允许文件与所有权

只允许修改：

```text
tools/check_integration_acceptance.py
tools/check_l1_frozen_surface.py
施工记录/20260901_DATA-L1-FINAL-CLOSURE_FROZEN验收绑定耐久性修订施工记录_v0.3.md
验证记录/20260901_DATA-L1-FINAL-CLOSURE_FROZEN验收绑定耐久性修订验证记录_v0.3.md
```

允许修改的稳定函数身份：

| 文件 | 函数 | 允许变化 |
| --- | --- | --- |
| `tools/check_integration_acceptance.py` | `验证_git()` | 远端包含语义与历史远端观察分账 |
| 同上 | `自检()` | 增加 Git 耐久性正负例；不得降低既有结论门禁 |
| `tools/check_l1_frozen_surface.py` | `检查状态()` | JSON 记录路径与 accepted candidate 精确绑定 |
| 同上 | `检查规范与生产边界()` | 状态相关的当前 4080 行 / 完整性段落检查 |
| 两文件 | 新增私有 helper（如确需） | 只服务上述检查，不形成新 CLI 或 schema |

两个检查器在本计划开始前必须为 clean 且无其它所有者。施工 / 验证记录为 v0.3 执行者专属。其它工作区 SELF-GOVERNANCE WIP 原地保留，不得暂存或改写。

## 4. 精确实现合同

### 4.1 `check_integration_acceptance.py`

`验证_git()` 固定按以下顺序检查：

1. 解析仓库与候选；候选不存在则 `基线.候选提交` 错误；
2. 解析 `基线.远端提交` 为 commit；不存在则 `基线.远端提交` 错误；
3. 解析 `基线.远端分支`；不存在则 `基线.远端分支` 错误；
4. 用祖先检查证明候选是记录远端的祖先 / 自身，否则失败；
5. 用祖先检查证明记录远端是当前远端分支 tip 的祖先 / 自身，否则失败；
6. 删除“当前远端 tip 必须等于 `基线.远端提交`”比较；不得改为要求候选或记录远端等于 tip；
7. 逐项验证候选中的输入文件路径和 blob，保持现有错误分账。

`基线.远端提交` 不删除字段、不改模板、不改 schema。它既要满足字段完整性，也要作为存在且位于候选与当前远端之间的历史锚点；当前远端正常追加提交不是记录失效理由。

### 4.2 `check_l1_frozen_surface.py::检查状态()`

PENDING 规则不变。FROZEN 时：

1. 校验 40 位小写 `accepted_commit`；
2. 拒绝绝对路径、路径穿越、仓库外解析结果、非 `.json`、非 DATA-L1-FINAL-CLOSURE 集成验收目录、文件不存在；
3. 读取并解析 JSON 对象；
4. 取得 `基线.候选提交`，与 `accepted_commit` 精确比较；
5. 要求 `验收标识=DATA-L1-FINAL-CLOSURE@<accepted_commit>`，服务切片名称与上级目标精确等于详细设计第 5.2 节文本；
6. 要求 `基线.仓库` 精确为 `.`、`基线.远端分支` 精确为 `origin/main`；
7. 取得 `结论.类型`，必须精确等于 `服务验收通过`；不得把合法保存且结构检查退出 0 的 `事实失败` / `环境阻断` / 较弱结论记录升级为冻结依据；
8. 用 Git 证明 `HEAD:<record>` 与 `origin/main:<record>` 都存在且 blob 相等，当前工作树 record 的 Git blob 也相等；未跟踪、本地修改或未推送记录失败；
9. 上述绑定和 PASS 结论成立后再运行集成验收检查器的 `--manifest --verify-git --strict`；
10. 任一失败写入具名错误并返回，不抛出未处理异常。

禁止把 `HEAD`、验收记录提交或当前远端 tip 当作 accepted commit。

### 4.3 `check_l1_frozen_surface.py::检查规范与生产边界()`

1. 定位 4080 正文 `## 1. 定位与冻结状态` 到下一二级标题之间的当前状态代码块；合同 FROZEN 行和整体 `<manifest状态>` 行必须各恰好一次，禁止全文 substring；
2. 规范目录必须恰有一个 4080 当前表格行；只在该行检查 `目标合同 FROZEN，整体 <manifest状态>`，并拒绝相反状态；
3. 必须恰有一个以 `20260901 DATA-L1 最终收口` 开始的当前完整性段落；
4. PENDING 段落必须明确仍保持 PENDING；FROZEN 段落必须明确已 FROZEN 且包含 `accepted_commit` 与 `acceptance_record`；
5. 不得用目录全文任意位置的历史短语满足第 2—4 项；
6. 4015、4070、有限 N v3、仓库唯一 import、许可拒绝、生产目标测试隔离和其它既有检查原样保留。

## 5. 禁止范围

禁止修改：

- `tools/check_specs.py`、冻结 manifest 和 schema；
- 4015、4070、4080、规范目录、计划索引及其它治理材料；
- L1 核心、公共 DTO、服务、仓库、持久格式、fixture 和专项；
- 生产入口、根工程 / filters、普通装配、L2 / L3 / 业务层；
- R1 验收记录和既有 v0.1 / v0.2 施工 / 验证记录。

本计划不执行 R2 或 G3，不把 DATA-L1 整体状态改为 FROZEN。

## 6. S0 门禁

执行前必须全部满足：

1. 当前为唯一主工作区 `main`；HEAD、`origin/main`、ahead / behind、index 和 dirty 已记录；
2. R1 已以 `3e2e23be9a79d192618a20076603851af5de7297` 提交并推送，记录路径和结构化结论可读；
3. 计划索引登记 `DATA-L1-FINAL-CLOSURE` v0.3、精确 plan blob、状态 `可执行`，引用 v0.3 设计发布提交；
4. 两个检查器实际函数仍与本计划第 4 节事实一致，没有其它所有者或未提交差异；
5. 当前 manifest 仍为 `PENDING_ACCEPTANCE` 且 accepted / record 为 null；4080 与目录仍为 PENDING；
6. C1、R1、v0.3 设计提交和索引提交都在当前 `main` 历史中；
7. 仓库外临时 Git 根可用，不占用其它验收 / 构建资源。

任一目标函数、记录身份、所有权或状态漂移都只停止本切片并具名退回；不得先改 schema、规范或 manifest 规避。

## 7. 实施步骤

1. 在 `验证_git()` 去除历史远端提交与当前 tip 的相等约束，显式验证候选、记录远端和远端分支存在，验证 `candidate <= recorded_remote <= current remote tip` 两段祖先链，并保留输入 blob 检查。
2. 扩展 `自检()` 或同文件私有测试 helper，在仓库外临时 Git 仓覆盖第 8.1 节正负例。
3. 在 `检查状态()` 增加 DATA-L1 JSON record 路径、解析和候选相等检查；保留对子检查器的严格调用。
4. 把目录摘要检查改为定位当前 4080 行与当前完整性段落，按 manifest 状态检查；不得全文搜索固定 PENDING 短语。
5. 在精确候选副本执行全部状态、路径、候选、目录和远端推进负例，正式树复查通过。
6. 写入两份 v0.3 专属记录，记录函数级知识变更清单；本计划只改 Python 工具，不修改 C++ 函数用途知识。
7. 精确暂存第 3 节四个文件，核对 staged 名单、diff、行结束和异主 WIP隔离，取得唯一发布租约后提交并普通推送形成 C2。
8. 回传 C2、plan blob、验证证据和未覆盖范围；由交互智能体派发 R2，不自行修改 manifest 或索引状态。

## 8. 验证矩阵

### 8.1 `check_integration_acceptance.py`

在仓库外临时 Git 仓生成候选 A、记录远端 R、非祖先 B 和后继治理提交 G：

| 场景 | 预期 |
| --- | --- |
| `A <= R <= origin/main`，输入 blob 相等 | PASS |
| `origin/main` 从 R 推进到 G，记录远端仍为 R | PASS |
| A 不存在 | FAIL |
| R 不存在 | FAIL |
| 远端分支不存在 | FAIL |
| A 不在 R 历史 | FAIL |
| R 不在当前 `origin/main` 历史 | FAIL |
| 输入路径不存在 | FAIL |
| 输入 blob 与 A 不同 | FAIL |

不得通过修改正式 `origin/main`、创建仓库分支或使用当前工作区未提交内容完成负例。

### 8.2 `check_l1_frozen_surface.py`

从精确候选副本形成以下变体：

| 场景 | 预期 |
| --- | --- |
| PENDING 且 accepted / record 均 null | PASS |
| PENDING 但任一绑定非 null | FAIL |
| FROZEN，accepted 与 JSON candidate 不同 | FAIL |
| FROZEN，accepted 与 candidate 相同但 `结论.类型=事实失败` 或其它非服务验收通过 | FAIL |
| 验收标识 / 服务名称 / 上级目标不是 DATA-L1-FINAL-CLOSURE 精确值 | FAIL |
| `基线.仓库` 不是 `.` 或远端分支不是 `origin/main` | FAIL |
| record 未跟踪、只在工作树、被本地篡改、未进入 origin/main | FAIL |
| record 为 `.md`、绝对路径、`..` 越界、其它目标目录或损坏 JSON | FAIL |
| 4080 第 1 节或规范目录 4080 行 PENDING、manifest FROZEN | FAIL |
| 当前状态错误但 4080 修订记录 / 规范目录其它段落附加正确 FROZEN 短语 | FAIL |
| 当前完整性段落缺 accepted commit 或 record | FAIL |
| C2 + `manifest_v0.3.json` + 4080 / 目录 / manifest 全部一致，远端已推进 | PASS |

所有变体结束后正式精确候选树必须恢复 PASS。

### 8.3 必跑命令

```powershell
python .\tools\check_integration_acceptance.py --self-test
python .\tools\check_l1_frozen_surface.py --strict
python .\tools\check_specs.py --strict
git diff --cached --check
```

提交形成后从 C2 的 `git archive` 再运行前三项；此时整体仍 PENDING，不能用手工 FROZEN 变体冒充正式状态。

## 9. R2 与 G3 固定交接

C2 推送后，R2 必须以 C2 为唯一候选，新增 v0.3 人读记录、`manifest_v0.3.json` 和 `证据_v0.3/**`，重做完整 DATA-L1 统一黑盒与检查器耐久性矩阵。R2 manifest 必须使用仓库 `.`、远端分支 `origin/main` 和详细设计第 5.2 节精确目标身份。R1 不得替代。

R2 PASS 并提交 / 推送后，G3 只能修改：

```text
规范/4080_子规范_DATA-L1通用操作全集与冻结门禁.md
规范/规范目录.md
tools/l1_frozen_surface_manifest.json
计划/计划索引.md
```

G3 把 manifest 的 accepted commit 设为 C2、record 设为 `验证记录/集成验收/DATA-L1-FINAL-CLOSURE/manifest_v0.3.json`，同步 4080 / 目录并使本计划退出。G3 不得修改检查器或验收记录。G3 推送前后都必须运行冻结面与严格规范检查；推送后还必须在仓库外 fresh clone 复验，当前 `origin/main` 前进到 G3 后仍须通过。

## 10. 失败收口与声明边界

- R1 未发布、占位未替换或索引未登记 v0.3：禁止施工。
- 临时 Git 正负例不稳定：修复测试构造或检查器，不删减语义。
- 需要改变 manifest schema、4080 机器语义或 L1 核心：真实漂移，停止并退回计划支撑。
- cached diff 混入异主 WIP：停止发布，不 stash、reset、clean 或整文件覆盖。
- push 前远端前进：重新 S0；只要新远端包含当前候选，历史记录不因 tip 变化失效。
- R2 非 PASS：保持 PENDING，不执行 G3。

C2 最多证明：

> DATA-L1 FROZEN 验收绑定检查器已修复远端推进耐久性、accepted candidate 精确相等和规范目录状态同步门禁；整体仍为 `PENDING_ACCEPTANCE`，等待 C2 的独立验收。

不得声明 DATA-L1 整体 FROZEN、上层应用完成，或未运行的崩溃 / 断电 / 真实介质 / 资源耗尽 / 压力并发 / 长时运行已经验收。
