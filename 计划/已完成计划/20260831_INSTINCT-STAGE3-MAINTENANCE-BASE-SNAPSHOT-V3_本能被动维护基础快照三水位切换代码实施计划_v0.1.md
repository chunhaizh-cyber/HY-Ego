# INSTINCT-STAGE3-MAINTENANCE-BASE-SNAPSHOT-V3 本能被动维护基础快照三水位切换代码实施计划 v0.1

日期：2026-08-31

计划身份：`INSTINCT-STAGE3-MAINTENANCE-BASE-SNAPSHOT-V3`

版本：v0.1

状态：已完成；结果提交 `d232e8036b30484d383acd8f564ab4d706b97ffe`

## 1. 目标与完成条件

实现基础快照 v2，使后继阶段三维护在同一 `G0` 取得正式 A/V、安全根定义、完整秒观察和 v3 三水位游标；v1 完整保留。

完成必须同时满足：

1. v2 DTO、完整性/成功谓词和 `读取本能被动维护基础快照_v2` 进入正式代码；
2. v2 只消费 `读取当前本能被动维护游标_v3`，两个事实代次水位逐字段回显且不晚于 `G0`；
3. 两次 v3 游标、A/V、定义和时间全部绑定同一 `G0`，非成功空载荷/截止0；
4. v1 ABI 与现有专项回归保持；
5. Debug/Release、专项、外部 ABI consumer、diff-check 和 strict 通过；
6. 形成专属施工/验证记录并精确提交、普通推送。

## 2. 正式依据与创建事实

依据：0050 v2.1、6120 v0.10、6170 v0.9、4230 v0.20，以及：

```text
规范/详细设计/20260831_INSTINCT-STAGE3-MAINTENANCE-BASE-SNAPSHOT-V3_本能被动维护基础快照三水位切换详细设计_v0.1.md
流程图/20260831_INSTINCT-STAGE3-MAINTENANCE-BASE-SNAPSHOT-V3_读取三水位本能被动维护基础快照函数流程图_v0.1.md
```

创建时确认：生产启动已经建立/迁移 v3；现有基础快照只承载并读取 v1，无法携带服务历史水位。该缺口与 L/H 数值无关；版本 1 阈值保持 `2767011611056432742 / 7378697629483820645`。

## 3. 精确施工范围

修改生产：

```text
海中鱼巣/领域/本能被动维护基础快照.数据.h
海中鱼巣/领域/组合.本能被动维护基础快照.ixx
```

修改专项：

```text
海中鱼巣/端到端测试.本能被动维护基础快照.ixx
```

新增记录：

```text
施工记录/20260831_INSTINCT-STAGE3-MAINTENANCE-BASE-SNAPSHOT-V3_施工记录_v0.1.md
验证记录/20260831_INSTINCT-STAGE3-MAINTENANCE-BASE-SNAPSHOT-V3_验证记录_v0.1.md
```

禁止修改：游标 owner/数据、启动、普通应用、工程登记、L1、L2特征/状态/动态、服务合同 owner、硬否决/FRESH、线程和 SELF-GOVERNANCE WIP。

## 4. 实施步骤

1. 在现有数据头追加详细设计第3节 v2 常量、DTO、枚举和完整性/成功谓词；不得修改 v1 定义。
2. 在现有组合器类追加 v2 读取函数，复用现有 A/V/定义/时钟读取，但游标首末读固定调用 v3。
3. 对 v3 未建立、待迁移、漂移、引用、资源和内部结果执行冻结映射。
4. 专项追加 v3 成功、无新秒、1/N秒、两个水位、待迁移、漂移和 v1 回归用例；不得用测试默认值绕过完整性谓词。
5. 形成函数级知识变更清单，但本计划不直接修改共享知识库。

## 5. 验证

至少执行：

- `git diff --check`、精确 staged `git diff --cached --check`；
- `python .\tools\check_specs.py --strict`；
- x64 Debug/Release 根工程 `ClCompile` 或更强构建门禁；
- 专项 Debug/Release；
- 仓库外 Debug/Release ABI consumer 编译运行，证明 v1/v2 同时可消费；
- 静态扫描确认 v2 零 v1 游标读取、零 L/H 重算、零游标推进、零生产写入。

若根链接仍受正式 HEAD 的无关未解析符号影响，必须分账记录，不能用 `/FORCE:UNRESOLVED` 宣称根链接通过；本切片自身仍须以隔离 consumer/专项证明链接与运行。

## 6. 完成声明边界

本计划完成只证明 v3 三水位基础快照 provider 可用。不得声明历史事实段、完整维护事务、A/V 被动变化、阶段三或 `INSTINCT-ROUTE` 完成。
