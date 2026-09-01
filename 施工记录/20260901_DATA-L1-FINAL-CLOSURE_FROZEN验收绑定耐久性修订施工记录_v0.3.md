# DATA-L1-FINAL-CLOSURE FROZEN验收绑定耐久性修订施工记录 v0.3

日期：2026-09-01

计划身份：`DATA-L1-FINAL-CLOSURE`

计划版本 / blob：`v0.3` / `c6de76077fb0c85939352f67c5df9160f6d917ca`

施工起点：`main@979ababbebaa3029975851e16f04728634473696`，起点时 `HEAD == origin/main`、ahead / behind `0 / 0`、index 为空。

## 1. S0

1. 计划索引把 v0.3 精确计划登记为 `可执行`；设计提交 `1d33ad59703ab5e9d4fa17898be87ea7da8f0705`、R1 失败记录提交 `3e2e23be9a79d192618a20076603851af5de7297` 和 C1 `9b1649d91cacc1a9353bea5bddb161ebda510367` 都在起点 HEAD 历史中。
2. 冻结 manifest 仍为 `PENDING_ACCEPTANCE`，`accepted_commit` 与 `acceptance_record` 均为 `null`；4080 与规范目录当前摘要仍为 PENDING。
3. 两个目标检查器在 S0 时均无未提交差异，起点 blob 分别为：
   - `tools/check_integration_acceptance.py`：`97d287d50169a0bc5165465ee62e75d6bd1e0a2c`
   - `tools/check_l1_frozen_surface.py`：`4b48bd1d04b87261b440f641ec0b7bbec369ad14`
4. 工作区存在大量异主 SELF-GOVERNANCE WIP；本切片未覆盖、暂存或清理这些字节。根工程和 filters 的异主工作树字节会重新出现 9 个端到端测试登记，因此冻结面与 strict 只在 `HEAD` 精确归档加本切片白名单覆盖的隔离树中作正式判断。
5. 已读取执行技能、协作 / Git / 知识规则、计划索引、v0.3 计划与设计、4080、ACCEPTANCE-01、0600、CODE-FILE-01、知识库入口 / 复用方法 / 当前状态，并回读两个检查器当前源码和调用点。
6. 知识库、能力证据目录和当前调用图没有两个 Python 检查器的专属用途卡或稳定能力项；复用裁决为直接修改既有函数并增加文件内私有测试辅助，不新建 CLI、schema、生产入口或第二检查器。

S0 结论：`PASS`。允许范围、目标函数、当前接口与 v0.3 设计一致，没有设计漂移或所有权冲突。

## 2. 实际变更

### 2.1 集成验收 Git 绑定

`tools/check_integration_acceptance.py::验证_git()` 改为依次验证：

```text
候选 commit 存在
-> 记录远端 commit 存在
-> 当前远端分支可解析
-> candidate <= recorded_remote
-> recorded_remote <= current remote tip
-> 候选提交内输入路径与 blob 精确相等
```

删除历史 `基线.远端提交` 必须永远等于当前远端 tip 的约束。`基线.远端提交` 字段、schema、模板、`已推送=true`、`使用未提交内容=false` 和输入 blob 门禁均保留。

`自检()` 新增仓库外临时 Git 正负例，覆盖远端从 R 推进到 G 后仍通过，以及候选、记录远端、远端分支、两段祖先链、输入路径和输入 blob 的拒绝分账；临时轮次结束后删除自身目录。

### 2.2 DATA-L1 FROZEN 精确绑定

`tools/check_l1_frozen_surface.py::检查状态()` 新增：

- `acceptance_record` 的相对路径、路径穿越、仓库内解析、DATA-L1 专属目录和 `.json` 限制；
- JSON 对象解析，`accepted_commit == 基线.候选提交`；
- 验收标识、服务名称、上级目标、仓库 `.`、远端分支 `origin/main` 的精确绑定；
- `结论.类型 == 服务验收通过`；
- record 必须同时进入 HEAD 和 `origin/main`，且 HEAD / 远端 / 当前工作树 Git blob 相等；
- 上述本地绑定全部成立后才运行通用验收检查器 `--verify-git --strict`。

`检查规范与生产边界()` 改为定位 4080 第 1 节唯一当前状态代码块、规范目录唯一 4080 当前表格行和唯一 `20260901 DATA-L1 最终收口` 当前完整性段落。PENDING / FROZEN 按 manifest 状态分支检查；FROZEN 完整性段落还必须包含 accepted commit 和 acceptance record。4015、4070、有限 N v3、唯一仓库 import、许可拒绝与生产目标测试隔离检查原样保留。

未修改冻结 manifest、规范、计划、计划索引、L1 源码、生产工程或 R1 材料；DATA-L1 整体状态仍为 `PENDING_ACCEPTANCE`。

## 3. 代码知识变更清单

| 身份 | 类型 | 用途 / 合同变化 | 调用边变化 | 待更新用途知识 |
| --- | --- | --- | --- | --- |
| `tools/check_integration_acceptance.py::验证_git(清单)` | 直接修改 | 从候选直达当前远端及 tip 相等，改为候选—记录远端—当前远端两段祖先链；输入 blob 合同不变 | 仍只调用同文件 `运行_git()` | 当前无专属用途卡；结果发布后登记为待识别 Python 治理工具，不修改 C++ 用途卡 |
| `tools/check_integration_acceptance.py::自检()` | 直接修改 | 增加 9 项 Git 耐久性正负例；不改变 CLI | 新增两个局部测试闭包，调用 `验证_git()` 与 `运行_git()` | 当前无专属用途卡；记录本批验证职责变化 |
| `tools/check_l1_frozen_surface.py::运行_git(*参数)` | 新增私有 helper | 只为同文件 FROZEN record 的 HEAD / origin / worktree blob 检查提供结构化 Git 调用 | 被 `检查状态()` 调用 | 当前无专属用途卡；保持文件内私有辅助，不形成公开能力 |
| `tools/check_l1_frozen_surface.py::检查状态(清单, 错误)` | 直接修改 | FROZEN 从“文件存在 + 子检查器退出 0”收紧为候选、PASS、身份、路径、仓库 / 分支及三方 blob 精确绑定 | 新增对 `运行_git()` 的调用；对子检查器调用保持但后移 | 当前无专属用途卡；结果发布后待知识维护者按正式提交识别 |
| `tools/check_l1_frozen_surface.py::检查规范与生产边界(清单, 错误)` | 直接修改 | 从全文固定 PENDING substring 改为当前 4080 状态块、目录当前行和当前完整性段落的状态相关门禁 | 无新增跨文件函数调用 | 当前无专属用途卡；结果发布后待知识维护者按正式提交识别 |

诊断责任：两个检查器都是命令行最终责任边界；目标函数继续把错误累积到结构化错误清单，由 `主函数()` 统一输出并映射退出码。私有 Git helper 只向上送出退出码与文本，不单独记录。

结果提交：本记录与代码同属待发布 C2，精确提交在取得发布租约并普通推送后由完成回传登记；本记录不预造提交哈希。

## 4. 声明边界

本施工最多证明两个冻结治理检查器按 v0.3 计划完成实现并通过施工验证。它不把 DATA-L1 整体升级为 FROZEN，不替代 C2 的独立 R2，也不证明真实并发、突然崩溃、断电、介质故障、资源耗尽、长时运行或任何上层应用完成。
