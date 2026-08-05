# L1-SIMPLIFY-P6-I64-COMPARE v0.3 验证记录

## 1. 验证对象

- 计划：`L1-SIMPLIFY-P6-I64-COMPARE v0.3`
- plan blob：`e0b62bbe955ba32c2aab140a1bcd995411906276`
- 正式恢复基线：`f9f6aeae52259749c778d4b8d47169d048ff7438`
- 范围：计划规定的八文件。

## 2. 构建与运行

| 门禁 | 实际结果 |
| --- | --- |
| Debug x64 Rebuild | PASS，0 warning / 0 error |
| Debug `--self-test-exit` | PASS，退出码 0，29/29 |
| Release x64 Rebuild | PASS |
| Release `--self-test-exit` | PASS，退出码 0，29/29 |
| `python .\tools\check_specs.py --strict` | PASS，110/110 |

Debug 与 Release 模块扫描均在 PowerShell 7 中先设置 `$PSNativeCommandUseErrorActionPreference = $false`，再运行 `tools/clang_ast/扫描.MSVC模块依赖.ps1`。两配置结果相同：`sources=192, modules=192, failed=0`，拓扑节点 192、循环 0、外部需求 0。

## 3. 静态与范围门禁

- `比较特征` 内 L1 完整快照调用计数：1。
- `比较特征` 内 L1 写调用计数：0。
- 五个公开函数签名各唯一出现一次。
- 三个新工程项在 `.vcxproj` 与 `.filters` 中各唯一登记一次。
- 计划禁止的 import/include：0 命中。
- 临时诊断标记 `iostream`、`cout`、`cerr`、`P6-EVIDENCE`：0 命中。
- L1、P2、P3、P4 上游文件相对正式基线：0 变更。
- `git diff --check`：PASS。

## 4. 语义矩阵结论

- 左值低于 P2 当前上界：`已比较`，排序三态 `-1`，具名关系 `当前低于目标`；请求差异时返回 checked `右减左`。
- 左值等于上界：`已比较`，排序三态 `0`，具名关系 `当前达到目标`，差异为 0。
- 左值高于上界：`入口拒绝/值域不允许`，排序、关系、差异载荷均为空。
- checked 差异不可表示：排序和具名关系仍可成功，差异材料明确不可表示。
- 目标身份、来源、创建代次、事实截止代次或合同身份不匹配：落入冻结的非成功状态，不返回伪造成功载荷。

## 5. 单快照与事实结构证据

比较入口对一次请求只获取一次 L1 完整快照，并在该快照中校准合同、当前实例值、P2 当前上界及全部见证。登记事实为五普通节点、一属性类型节点；合同事实为一注册节点、一配置值、一槽、八条节点目标关系和二十项配置材料。专项自检验证正式读回、幂等/冲突、P4 换代沿用和并发读取期间权威事实不变。

## 6. 未证明边界与剩余问题

未验证运行期生产装配、持久恢复、L2 消费者、STEP-3 状态迁移、跨服务集成验收或 SELF-GOV-CLOSURE 全链路完成；本结果也不证明高于 P2 当前上界存在合法成功分支。计划范围内未留已知失败项，是否接受 STEP-2 目标完成由交互智能体独立核验。
