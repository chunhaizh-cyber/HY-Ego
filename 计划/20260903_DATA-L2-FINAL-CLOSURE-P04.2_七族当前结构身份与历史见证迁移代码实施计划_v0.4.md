# DATA-L2-FINAL-CLOSURE-P04.2 世界七族当前确认与历史见证迁移代码实施计划 v0.4

日期：2026-09-03

计划身份：`DATA-L2-FINAL-CLOSURE-P04.2-WORLD-SEVEN-CURRENT-CONFIRMATION-AND-HISTORICAL-WITNESS`

计划基线：`main == origin/main == 944d72ab47e15b390e40950c311b0d53018d78ce`。

状态：执行期具名 WIP 指纹漂移的最小修订。经本文件与计划索引同次发布后，v0.4 是本身份唯一当前`可执行`候选；它只要求执行智能体重新 S0，不证明代码、构建、恢复或集成验收已通过。

## 1. 绑定、范围与不变项

本版同时绑定下列已发布施工基：

```text
v0.2：计划/20260903_DATA-L2-FINAL-CLOSURE-P04.2_七族当前结构身份与历史见证迁移代码实施计划_v0.2.md
      plan blob 560b1031e91ac5602cf01e5df6152b2ca1fddbac
v0.3：计划/20260903_DATA-L2-FINAL-CLOSURE-P04.2_七族当前结构身份与历史见证迁移代码实施计划_v0.3.md
      plan blob 07da8114f7d21cd6c1676bb2640b6a3498517f17
```

除本版明示替代 v0.2 §2、§6 第 1/5 项和 §7.1 中“交接 WIP 指纹相等”的验证方法外，v0.2/v0.3 的目标、正式依据、七族 ABI、H 回显裁决、生产白名单、禁止范围、物理删除、调用分流、旧 ABI 精确扫描、隔离构建、记录路径和完成边界均原样继承。

本版不改变机器语义、规范、详细设计、公开 ABI、DTO、代码、所有权、WIP 白名单或验证资源。当前工作树内 C031 相关规范 WIP 不是本版依据；P04.2 的正式合同只读取本版基线 `HEAD` 中的 0050 §3.13、4230 §2.4、C032 和 P04.2 详细设计 v0.4。`a1a98f744cbd12461fa17a9522fa96c58638fabc..HEAD` 对两份交接文件、4230、P04.2 详细设计、v0.3 和计划索引均无差异，故已保存的 P04.2 交接证据仍可复核。

## 2. 已冻结的交接 WIP canonical 基线

v0.2 的两项 SHA-256 不是 Windows 工作树 CRLF 字节的 `Get-FileHash`；它们是 S0 二进制补丁 new-side 的 Git canonical UTF-8/LF blob 字节 SHA-256。不得用 CRLF checkout 或文本重定向的哈希否定、替换或放弃该基线。

| 交接文件 | S0/current HEAD old-side blob | S0 保存补丁与 SHA-256 | new-side canonical blob | canonical LF SHA-256 | 原 WIP 相对 HEAD |
| --- | --- | --- | --- | --- | --- |
| `海中鱼巣/领域/服务.L2需求结构.ixx` | `6d9dc9315ba59d45ee015f4451508a05bcbc68cc` | `20260903-101500-p042-executor/服务.L2需求结构.S0.binary.patch` / `9F8B65B5615E6ABFD362957BB6B003F515047F2162AA3D27DAE99E07D5E0E644` | `487b667924d628b26d34babce1fc59405d7ea125` | `2F968A7ADF791137945879E2F87C517DA5C9A218EC7FB4530F2E648EC659EDC1` | `+1/-0`，仅 `export import 海中鱼巣.领域.合同.任务结果消费身份;` |
| `海中鱼巣/端到端测试.自我治理一致事实.ixx` | `b71dda6bb27fcefc165e2f13e5ba8455243ce840` | `20260903-101500-p042-executor/端到端测试.自我治理一致事实.S0.binary.patch` / `FDA18D2AAA70FDF1CD75DEEC2CF75AE63754BF2C21D70068F2E9CA054765BFA3` | `4c95cf9c17a4496ef8dbd87a2757cd00dbf62d1a` | `D611182CA46D6481E4C650C83448C899BFF6C41B4EF60CD931FEF65062646920` | `+1610/-6659` |

补丁根固定为：`D:\TEMP\海中鱼巣\DATA-L2-FINAL-CLOSURE-P04.2-WORLD-SEVEN-CURRENT-CONFIRMATION-AND-HISTORICAL-WITNESS\20260903-101500-p042-executor\`。S0 首先必须验证：当前 `HEAD:<path>` 仍等于表中 old-side blob；补丁文件的物理 SHA-256 等于表值；`git cat-file -e <new-side>^{blob}` 成功；以二进制方式读取该 blob 的 SHA-256 等于表中 canonical 值。任一项失败，停止两份文件的施工、暂存和验证，保留现场并退回计划支撑；不得重记 SHA、重做补丁、接受 CRLF 变体或猜测合并。

## 3. 允许叠加的精确 P04.2 增量

在 `D:\TEMP` 下从届时 `HEAD` 的 `git archive` 建立 LF canonical 副本，先应用表中 S0 补丁并复核其 new-side blob/sha，再与工作树逐文件 `git diff --no-index --unified=0`。只有下列增量可叠加在原 WIP 上：

| 文件 | 相对原交接 WIP 的唯一允许 diff | 附加约束 |
| --- | --- | --- |
| `服务.L2需求结构.ixx` | 恰 `+16/-17`、一个 `需求目标身份验证` hunk：两处 `读取存在/特征定义身份来源(... L2读取类别::当前 ...)` 分别替为 `确认当前存在/特征定义结构身份(头, 身份)`，并仅以合同版本、`已读取`、截止=`期望代次`、空变更结果头判定成功。 | 该 diff 不得触及原 import WIP、其它函数、生命周期 payload 或 C031 内容。 |
| `端到端测试.自我治理一致事实.ixx` | 恰 `+6/-4`、一个 `建立并验证真实状态迁移测试夹具` 的首态前 hunk：`读取特征实例身份来源(... 当前 ...)` 替为 `确认当前特征实例结构身份({L2结构合同版本, G}, 特征实例)`，并仅增加合同版本、状态、截止=G、空变更断言。 | 不得触及其余 C031 WIP；本文件永不进入 P04.2 index、构建或验收输入。 |

每个 diff 还必须通过 `git diff --check`，且除表中单一 hunk 外无其它文件内容差异。将经审定的 P04 hunk 在 `D:\TEMP` 副本逆转后，副本必须重新得到表中 new-side canonical blob 与 canonical LF SHA-256；反转失败、hunk 数/numstat 不符、上下文漂移或任何额外行均为失败，不得以“整文件仍 dirty”放行。

这说明当前工作树总差异分别可为需求 `+17/-17`、端到端测试 `+1612/-6659`：它们等于原交接 WIP 加上表中唯一 P04.2 增量，绝不表示异主 WIP 被本叶取得、可暂存或可提交。

## 4. index-only 与最终验收门禁

需求文件只能由 clean `HEAD` 副本加第 3 节需求 P04 hunk 生成 index-only patch，并以 `git apply --cached` 暂存；禁止 `git add 海中鱼巣/领域/服务.L2需求结构.ixx`。暂存后必须同时成立：

1. `git diff --cached -- 海中鱼巣/领域/服务.L2需求结构.ixx` 只含第 3 节两处当前确认替换，且不含 `任务结果消费身份` import；
2. `git diff -- 海中鱼巣/领域/服务.L2需求结构.ixx` 恰为原 `+1/-0` import WIP，工作树与 index 不再出现额外 P04 行；
3. `git diff --cached --name-only` 不含 `海中鱼巣/端到端测试.自我治理一致事实.ixx`，该文件也不得作为 P04.2 隔离构建、专项或验收输入；
4. v0.2 §7.1 的 staged 与工作树旧函数/旧 DTO 精确扫描仍均为零命中，宽模式仍只作新历史见证说明性核查；14 份生产文件和两份记录之外不得进入 index。

执行记录必须分别保存：clean HEAD archive、两份经校验的 S0 补丁、canonical blob/sha 输出、原 WIP 到工作树的两个精确 diff、需求 index-only patch、staged/worktree diff、旧 ABI 精确扫描和排除 E2E 的证明。CRLF 物理变体只能作为验证器表示差异记录，不能替代 canonical 哈希或成为通过条件。

## 5. 重新 S0、完成边界与修订记录

v0.4 发布后，执行智能体必须同时读取 v0.2、v0.3 与本文件的精确 blob，以届时 HEAD、index、再用文件登记、交接补丁、两份文件的 canonical 基线、P04 增量、H 回显专项及验证资源重新 S0。任何后续 HEAD 变化都必须先重验第 2 节 old-side blob；本版不授予对异主 WIP 的覆盖、删除、整文件暂存或发布权。

本版只消除“原 WIP 整文件指纹必须不变”与“允许 P04.2 薄迁移”之间的计划内验证矛盾，不改变 DATA-L2 `FROZEN`、P08、4070、DATA-L3、ARCH-L4 或普通应用验收边界。

| 日期 | 版本 | 修订内容 |
| --- | --- | --- |
| 2026-09-03 | v0.4 | 以 S0 保存补丁的 old/new canonical blob 和 LF SHA-256重建交接 WIP；冻结需求两处 `+16/-17` 与 E2E 一处 `+6/-4` 的唯一 P04.2 增量、逆补丁复原、需求 index-only 与 E2E 永不入 index 门禁。 |
