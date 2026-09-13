# WORLD-TREE-CONCEPT-FIRST-A 概念前置与世界添加验证记录

日期：2026-09-13

## 1. 验证身份

- S0 基线：`4cde0f5994850d1ed0452b8620b055595f557487`。
- 正式计划：v0.6，blob `2161774da6c891e16ac518a3ab7b79f8bcb94848`；详细设计 blob `b7f2f41236d105456ae142458bfffa4ad0abd215`。
- 主验证根：`D:/TEMP/海中鱼巣/WORLD-TREE-CONCEPT-FIRST-A/01a06f6b-code-a-7d83bf16`。
- 最终20个生产/冻结工具输入：`final-source-manifest.json`，其中 world SHA256 `1E0CA6035849CB87F5563C1C429D34FEF53F7751C700D1A8295A2C7B881B4FAE`、self 实现 SHA256 `7BE09100A0A5FACC1C55B25C06F12DE61B0FD81269D9031FD7768E8C5E9FC0B2`、concept SHA256 `54988DEB62DCCE09B91F069E1346A1667F39B56D43160A7C431E6CDF34F53DEF`。
- MSBuild：`C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe`，`PlatformToolset=v145`，`x64`，`/m:1`。

## 2. 最终验证矩阵

| 项目 | 结果 | 证据与边界 |
| --- | --- | --- |
| 主工程 Debug Rebuild | PASS | `logs/main-debug-rebuild-final-current.log`：0 警告、0 错误；使用计划专属 `main/obj/debug` 与 `main/bin/debug`。 |
| 主工程 Release Rebuild | PASS | `logs/main-release-rebuild-final-current.log`：0 警告、0 错误；使用计划专属 `main/obj/release` 与 `main/bin/release`。 |
| world 独立消费者 Debug/Release | PASS | `world-probe/build-debug-final-current.log`、`build-release-final-current.log` 均完整 Rebuild 成功；`run-debug-final-current.log`、`run-release-final-current.log` 均 exit 0，输出 `PASS world-only matrix`。 |
| world 三挂靠与 A4 恢复矩阵 | PASS | 同一 world 驱动覆盖直接子场景、场景成员、父存在组成，模板排序去重后的原键恢复，组织拒绝，组合键/子键冲突，预算增加、预算不足，后续写与部分子账下首次 H 保留；最终当前读逐项互证。 |
| world/self 结果谓词反例 | PASS | world 驱动篡改零代次、空根、直接父、EC 使用和场景四根均拒绝；持久 H 驱动另覆盖 self 四根/角色/父语境等完整投影篡改。 |
| 根协调私有端口负向编译 | PASS | `world-probe/negative-root-private-debug-final.log`、`negative-root-private-release-final.log`：普通消费者访问存在/场景两端口时每配置仅出现预期2个 C2248。 |
| 纯概念 v2 Debug/Release | PASS（独立复核） | 独立根 `D:/TEMP/海中鱼巣/WORLD-TREE-CONCEPT-FIRST-A/01a06f6b-pure-concept-regression-4c591208`；两配置各35/35，追加并发恢复各25/25。 |
| L1 独立公开回归 | PASS（L1 原代码所有者执行） | 独立根 `D:/TEMP/海中鱼巣/WORLD-TREE-CONCEPT-FIRST-A/01a06f6b-l1-regression-9fb37420/revision-owner-value-01`；两配置各322项断言。 |
| 持久普通装配、自我与前缀恢复 | PASS（独立复核） | `D:/TEMP/海中鱼巣/WORLD-TREE-CONCEPT-FIRST-A/01a06f6b-persistent-self-h-b627/revision07`；Debug/Release各基础15项+前缀34项，并各有1个持锁进程，共100次真实启动。 |
| 生产调用图静态扫描 | PASS | `logs/ordinary-callgraph-scan-final.txt`：普通启动链只调用 `初始化普通应用自我`，未命中旧 `创建绑定存在`、相关概念共同出生、待发布概念或静态世界根入口；旧 v3 DTO 声明仍保留，不据此宣称物理删除。 |
| L1 45项冻结检查 | PASS | `logs/l1-strict-final.txt`：45项操作、5个冻结文件与生产边界一致。 |
| 正式规范 strict | PASS | `logs/specs-strict-final.txt`：124份目录项通过。 |
| Git diff 检查 | PASS | `logs/diff-check-final.txt`；worktree 与 cached 检查均 exit 0，只有既有换行提示。 |
| 20个冻结源码 hash 回读 | PASS | `final-source-manifest.json` 生成后逐项回读，20/20 一致。 |

## 3. 独立验证证据分账

### 3.1 L1

独立根 `D:/TEMP/海中鱼巣/WORLD-TREE-CONCEPT-FIRST-A/01a06f6b-l1-regression-9fb37420/revision-owner-value-01` 的 Debug/Release 完整 Rebuild 均为0警告、0错误，同一驱动每配置322项断言、历史恢复和 V01—V08 均 exit 0。最终 `result.json` SHA256 为 `46FC7CC4991285B2AF159B32420DF7624FDAFA2E55E2D6BB2B7175762913827E`，`artifact-manifest.json` SHA256 为 `06172CE2F014692DE91A70F30E4D52ACCE5ADE33ABB261703BC2E756C21FEC7F`。

较早一轮 `80407B.../1191040...` 证据只保留为历史诊断，不是本记录的最终输入或最终结果。L1 新整体能力仍为 `PENDING_ACCEPTANCE`，本计划消费者回归不把它升级为正式 FROZEN。

### 3.2 纯概念

独立根 `D:/TEMP/海中鱼巣/WORLD-TREE-CONCEPT-FIRST-A/01a06f6b-pure-concept-regression-4c591208` 的 round10 Debug/Release 均完整编译、链接、运行，分别35/35；两配置追加并发终态恢复各25/25。五个私有参与者方法在两配置只出现预期 C2248，普通控制消费者编译成功。`README.md`、`result-matrix.json`、`input-manifest-10.json` 与 `evidence-manifest.json` 已冻结，1432份证据的 SHA 总表为 `d1edfb5d36fc0bacd831607cd3ef88e8eb5e0e715908b048332d6f2107bae7ea`。

其中27组为概念服务公开合同路径，8组是 L1 对公开不可构造位置的精确拒绝，后者没有冒充概念引用保护路径通过。50轮并发只实际观察到状态2/15；状态19/21未通过故障注入主动命中，因此不声明这两个分支已运行验证。

### 3.3 持久普通装配与真实自我

独立根 `D:/TEMP/海中鱼巣/WORLD-TREE-CONCEPT-FIRST-A/01a06f6b-persistent-self-h-b627/revision07` 使用68份与最终生产 SHA 逐项一致的输入。Debug/Release均完整构建、链接；每配置运行基础15项、前缀34项和1个持锁进程，共100次真实启动。覆盖首次形成、同配置重启，身份/首次 H/G/活动快照与首次账文件零变化，concept-only/world-only/根与元结构前缀续行，额外合法子场景恢复，原 G 守卫，owner/角色/旧 scene/旧 v1 concept 拒绝，以及14类完整成功谓词反例。

`H-final-report.md` SHA256 为 `CBFCEC69D156A32A4F6CC63D63FC993CAE631A00D9A14BB0E7D6A0D5F8EBA68E`，`H-final-result.json` SHA256 为 `D16BD5C6ADE6792141A766A5A0A5CA2545B347B271B85620842BB64A244D69FE`，`artifact-index.json` SHA256 为 `3B2D5F6BBA548FE200179BF8E832E086DC04AEC34038700A8DE3DED869790D8F`，`input-manifest.json` SHA256 为 `228B72E37BD6EE015A371AF0E1B860B3EBC5DBF7644423DD67C2421E95FDF991`。

revision03 的四根反例失败、revision04—05诊断和revision06旧 world 完整通过均保留为历史证据，没有替代最终 revision07。
## 4. C1001 与模块归属回证

L1 owner 默认比较改为等价底层稳定编码比较后，世界根和概念 importer 的原 C1001 消失。纯概念 DTO 判定成员回到声明它的合同模块，已发布概念参与者移入绑定模块，根服务采用 v0.6 冻结的 `extern "C++"` 共享归属。self 非内联实现放入同模块 `.cppcpp`；world 7个公开定义、存在角色读和绑定11个 v2 定义只移除显式 `inline`，函数体、签名、异常边界和结果互证不变。最终主工程与两个专项的 Debug/Release 完整构建证明原 C1001 不再出现。

## 5. NOT_RUN 与结论边界

- 真实磁盘硬件故障、进程崩溃/断电中断、资源耗尽、长时和压力测试：`NOT_RUN`。
- 旧域的物理迁移/删除、完整世界树后续治理线程运行：不在 A 计划范围。
- 本记录是未提交工作区的代码验证记录，不是正式集成验收；Git 提交和推送由交互根串行完成。
