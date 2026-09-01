# DATA-L1-FINAL-CLOSURE 独立集成验收记录 v0.2

日期：2026-09-01

## 1. 结构化结论

```text
INTEGRATION-ACCEPTANCE-FAIL / 事实失败
候选提交 = 9b1649d91cacc1a9353bea5bddb161ebda510367
候选 tree = a3b406ec1923aa0ae1ae9b14fac6dc247d34080f
远端 = origin/main@9b1649d91cacc1a9353bea5bddb161ebda510367
唯一运行输入 = 上述提交的 git archive
```

候选的生产目标测试隔离修订本身成立：43 项冻结公开面和 5 个冻结文件一致，严格规范检查 115 / 115；生产入口不含测试 import 或 `--test-*`，根工程和 filters 不登记任何 `端到端测试.*`；Debug / Release x64 正式 fresh Rebuild、旧开关拒绝以及仓库外只含两个 L1 专项的统一 driver 均通过。

但是最终冻结门禁仍存在事实缺陷。隔离变体把冻结清单改为 `data_l1_status=FROZEN`，把 `accepted_commit` 故意填为 40 个零，并把 `acceptance_record` 指向一份结论仍为 `事实失败` 的结构合法验收清单；同时只把 4080 的整体状态行改为 `FROZEN`。`python tools/check_l1_frozen_surface.py --strict` 仍退出 0，并输出“43 项操作、5 个冻结文件与生产边界一致”。这证明当前门禁：

1. 没有要求验收清单结论必须为 `服务验收通过`；
2. 没有要求 `accepted_commit` 等于验收清单的候选提交；
3. 规范目录检查仍硬编码寻找 `目标合同 FROZEN，整体 PENDING_ACCEPTANCE`，没有按状态轴核对 FROZEN 摘要。

因此该候选不能保证“只有同一候选的独立 PASS 才能进入第二次治理发布”，不满足 4080 第 7、8 节和详细设计 v0.1 的最终收口条件。DATA-L1 必须继续保持 `PENDING_ACCEPTANCE`。

## 2. 基线与隔离

- Git 归档 SHA-256：`CA15F5F091DE595F395BB73D240332CA327249BE8712356DDF94D758ECEC863C`。
- 独立根：`D:\TEMP\海中鱼巣\DATA-L1-FINAL-CLOSURE-INTEGRATION-ACCEPTANCE\v0.2-9b1649d9-01`。
- 归档源码：`...\source`；正式构建、临时 driver 构建、持久材料和负例均使用本轮独立子目录。
- 未读取或使用共享工作区未提交源码作为验收输入；未修改代码、工程、规范、设计、计划、索引、旧 v0.1 FAIL 记录或旧 manifest。

## 3. 已通过事实

| 项目 | 结果 | 证据 |
| --- | --- | --- |
| 候选远端可达 | PASS | `origin/main` 当前为候选，候选为其祖先（自身） |
| 冻结公开面 | PASS | 43 项操作、5 个冻结文件一致 |
| 正式规范 | PASS | 115 / 115 |
| 标准冻结负例 | PASS | 14 类、15 个实际变体全部非零 |
| 生产入口隔离 | PASS | 测试 import 0、`--test-*` 0 |
| 生产工程隔离 | PASS | vcxproj / filters 端到端测试项均 0 |
| 正式 Debug / Release | PASS | x64、v143、独立 IntDir / OutDir、fresh Rebuild，均 exit 0 |
| 旧开关拒绝 | PASS | 两配置均 exit 2，专项 PASS 输出 0 |
| 临时项目形状 | PASS | 生产 0 项；临时恰 2 项；正式入口替换恰 1 次 |
| 临时 driver Debug / Release | PASS | fresh Rebuild 和运行均 exit 0 |
| 三类事实与旧格式 | PASS | 节点 / 关系 / 值当前、历史、墓碑、格式 1 / 2 / 3、V01—V08 输出通过 |

通过项只证明实际发生的构建、隔离、公开专项黑盒与读回范围；不抵消冻结门禁耐久性失败。

## 4. 八类矩阵分账

| 类型 | 状态 | 分账 |
| --- | --- | --- |
| 成功 | PASS | 三类事实、共同事实代次、owner 和一致投影按公开专项读回 |
| 精确重复 | PASS | V02、V07、V08 证明原身份 / 代次 / 结果收敛，不形成第二事实 |
| 合法拒绝 | PASS | V03、V04 的非法 N、身份、owner、端口、G0、引用和属性槽保持零写 |
| 外部失败 | PASS | 清单 / 槽缺失、长度、摘要、未知格式 / 标签、存储占用 fail-closed |
| 并发 | NOT_RUN | 覆盖多参与者原子失败点与 G0 漂移，但未运行真实跨线程竞争或压力并发 |
| 崩溃恢复 | NOT_RUN | 只运行受控新运行包恢复；未强杀进程或在清单 / 槽替换窗口崩溃 |
| 正式读回 | PASS | 格式 1 / 2 固定样本和格式 3 均恢复三类事实并继续 v3 |
| 正式构建 | PASS | 正式 Debug / Release fresh Rebuild，且生产目标零测试入口和工程项 |
| 冻结门禁耐久性（自定义） | FAIL | 结论为事实失败的验收清单与不匹配 `accepted_commit` 被 FROZEN 门禁接受 |

## 5. 允许与禁止声明

允许声明：

- 候选已修复首轮发现的生产目标测试隔离问题；
- 43 项冻结面、5 个冻结文件、15 个标准负例、正式双配置构建、旧开关拒绝和双配置统一 driver 在候选归档树通过；
- 三类事实、格式 1 / 2 / 3、V01—V08、拒绝与受控正式读回在本轮公开专项范围通过；
- 当前失败唯一由最终 FROZEN 结论绑定门禁缺陷触发。

禁止声明：

- 不得声明 DATA-L1 已整体 `FROZEN`；
- 不得声明 DATA-L1-FINAL-CLOSURE 通过独立集成验收；
- 不得执行第二次治理发布或将 `data_l1_status` 升级为 `FROZEN`；
- 不得把统一 driver、自检输出或构建通过升级为最终冻结门禁通过；
- 不得声明真实并发竞争、突然崩溃、断电、介质故障、资源耗尽或长时运行已验证。

## 6. 后继条件

修复候选至少必须让机器门禁同时证明：验收清单结论为 `服务验收通过`、验收清单候选提交与 `accepted_commit` 完全相等、候选远端可达、验收记录自身位于合法治理提交，并按 `PENDING_ACCEPTANCE / FROZEN` 当前状态核对规范目录摘要。修复必须形成新提交并重新独立验收；不得把本记录改写为 PASS。

精确命令、退出码、输出哈希与外部日志见 `证据/v0.2/命令与退出码.md`；失败复现见 `证据/v0.2/冻结门禁耐久性失败.md`。
