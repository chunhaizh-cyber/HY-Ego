# ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION 代码实施计划 v0.5

日期：2026-09-01

状态：执行期发布合同缺口修订候选；详细设计、计划发布并由计划索引登记精确 v0.5 blob 后才可重新 S0。

恢复起点：v0.4 验证起点为 `main == origin/main == f2810ffbd0a0d5b9d851bc1e2d6ceb08ae97c5f1`。v0.5 允许 HEAD 只因治理文档继续前进，不允许代码、工程或构建输入提交前进。

唯一详细设计：`规范/详细设计/20260901_ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION_L2特征状态模块实体所有权ABI纠正详细设计_v0.5.md`；精确 blob 由第一阶段发布后登记。

## 1. 来源问题、目标与完成条件

v0.4 已使用累计的特征、状态、动态结构、动态发布四份源码通过 v145/v143 × Debug/Release x64 fresh Rebuild 和 dumpbin ABI 身份核对，但 v0.4 最终发布白名单只允许动态结构、动态发布与两份记录。特征、状态仍是未提交 WIP；照原白名单发布会使远端缺失验证输入，不能复现四配置成功。

本计划不再实施代码。目标是把已验证的四生产源码和两份最终证据记录作为一个不可拆分快照，一次性精确提交并普通推送。完成条件为：六个 staged blob 全部与冻结值相同、没有第七个 staged 路径、治理检查通过、普通推送成功、`HEAD == origin/main`、ahead/behind `0/0`、index 空。

## 2. 零修改边界与唯一发布白名单

禁止修改、格式化、追加或重新生成任何生产源码与记录；本计划只有读取、hash、diff、暂存、提交和推送动作。唯一发布白名单为：

```text
海中鱼巣/领域/服务.L2特征结构.ixx
海中鱼巣/领域/服务.L2状态结构.ixx
海中鱼巣/领域/服务.L2动态结构.ixx
海中鱼巣/领域/服务.L2状态动态原子发布.ixx
施工记录/20260831_ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION_施工记录_v0.1.md
验证记录/20260831_ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION_验证记录_v0.1.md
```

精确 blob 依次必须为：

```text
df6c81868fc55dc8cf9f542596386d16bac3af05
c7748ad723265a618b492ba5f0274a98c38f6993
12b44a8a4495a51be5872d0ba3a516577dbbb405
71aaa54ddfbb582e7e4bc2777dcfb91851444fc6
14df2b487d92248a9619f95729ef4a506f6ad9cd
09186aefb9a426476c32c1cf94cbcddf02958bc9
```

禁止只发布 v0.4 的后两源码和两记录，禁止把六文件拆成多个结果提交，禁止带入装配、DTO、工程/filters、Gseed、规范、知识库、流程图、计划索引、v0.5 设计/计划或其它 WIP。v0.5 设计、计划和索引必须在结果发布前由治理阶段独立提交。

## 3. 重新 S0 与证据复用

1. 核对当前为 `main`、`HEAD == origin/main`、ahead/behind `0/0`、index 空、发布租约和六文件所有权可用。
2. 核对正式计划索引唯一活动项已经登记本计划 v0.5 的精确 plan/design blob。
3. 核对从 `f2810ffbd0a0d5b9d851bc1e2d6ceb08ae97c5f1` 到当前 HEAD 的提交只修改治理文档，不含代码、工程或构建输入。
4. 对第 2 节六个路径运行 `git hash-object`，逐一匹配冻结 blob；任何一项不符立即停止，不修改现场。
5. 在上述四项全部满足时，复用证据根 `D:\TEMP\海中鱼巣\ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION\20260901-v04-abi-owner-01` 的四配置 fresh Rebuild 和 dumpbin 结果，不重复构建、不占用新验证根。该证据只绑定第 2 节精确六元快照。

HEAD 出现非文档提交、源码或记录 blob 改变、计划索引未正确激活、index 被占用或所有权冲突时，本计划停止；不得以重新格式化、补记录、临场修复或局部重验绕过。

## 4. 精确暂存、验证与发布

取得唯一 Git 发布租约后，用显式六路径执行一次 `git add -- <六路径>`。随后必须核对：

```text
git diff --cached --name-only        恰为六路径
git rev-parse :<path>                六个 index blob 精确匹配
git diff --cached --check            退出 0
git diff --check                     退出 0，无 whitespace error
python .\tools\check_specs.py --strict  116/116 且退出 0
```

若 staged 集合出现第七个路径、任一 index blob 不符或检查失败，停止并保留现场，不能提交。全部通过后形成一个普通结果提交并推送 `origin main`；禁止 force、merge、rebase、cherry-pick、stash、reset 或 clean。

推送后复核提交只含六个路径、`HEAD == origin/main`、ahead/behind `0/0`、index 空，六个结果文件已进入正式基线。报告其它异主 dirty，但不暂存、不覆盖、不声称全工作树 clean。

## 5. 完成声明与后继

本计划完成只表示 v0.4 已验证的模块实体 / DTO ABI 四配置链接闭合结果，连同完整施工和验证证据，已经以可复现六元快照发布。它不新增代码验证，也不证明 Gseed、跨进程恢复、阶段三、本能路线或业务集成验收完成。

结果发布后通知交互智能体恢复 Gseed 原计划验证，并通知代码知识库维护智能体以结果提交为基线维护用途知识；不得直接把本计划发布等同于 INSTINCT-ROUTE 完成。
