---
name: hai-zhong-yu-chao-execute
description: Execute or resume design-ready plans in the single shared 海中鱼巣 repository workspace. Use when the current top-level task tree is the execution-agent type and the user says 执行, 继续执行, 按照计划索引执行, 继续执行计划, or 持续执行, when the permanent execution task receives PLAN-AVAILABLE-NOTICE or PLAN-SUPPORT-NOTICE, or when it is woken after EXECUTION-YIELD. The execution agent is the only code writer, performs one S0 per serial plan segment, validates, commits, and non-force pushes main, sends execution facts only to its paired plan-support agent, and stops only on a named terminal condition.
---

# 海中鱼巣执行

## 单工作区入口

只在当前顶层任务树为执行智能体类型时使用本技能。遵守 `AGENTS.md`；智能体 S0、施工记录、完成 / 退回消息读取 `.codex/rules/设计执行双窗口交互规则.md`；唯一主工作区、写入占用、Git 收口和发布读取 `.codex/rules/单工作区串行执行与发布规则.md`。

本项目不使用 Git / Codex worktree、任务分支、长期执行分支或集成分支。执行智能体固定在唯一主工作区的 `main` 上工作，是代码、工程、自检和计划专属施工 / 验证记录的唯一写入者。

不得硬编码盘符。用下列事实解析技术身份：

```powershell
git rev-parse --show-toplevel
git branch --show-current
git status --short --branch
git rev-parse HEAD
git rev-parse origin/main
git rev-list --left-right --count HEAD...origin/main
```

开始写入前必须确认：

```text
当前分支为 main
main == origin/main
工作区 / index clean
没有其它智能体持有写入占用
没有第二活动计划段
```

任一项不成立时，不 stash、不 reset、不 clean、不切分支、不覆盖现场，按具名阻断收口。

当前计划正常收口，或计划明确允许的代码 WIP 已形成具名断点提交、推送到 `origin/main` 且工作区 / index 恢复 clean 后，必须立即重读计划列表。只要存在候选就禁止输出最终答复，必须建立下一计划段并重新 S0。不等待计划智能体或计划支撑智能体接受 S0 或选择下一项。

完整详细设计是代码实施的直接依据，流程图只用于复核详细设计和代码是否覆盖完整过程。执行智能体不得根据流程图自行设计函数、结构、字段、类型、条件、版本、所有权、调用顺序或错误处理；如果详细设计仍要求作上述裁决，必须在第一写前返回具名 DRIFT。

## 通知触发

既有执行任务收到 `PLAN-AVAILABLE-NOTICE` 或 `PLAN-SUPPORT-NOTICE` 并产生新轮次时，必须使用本技能；不要求用户同时发送“继续”。通知只是定位线索，不是执行许可、计划选择或 S0 PASS。

```text
用户已在执行智能体自身任务明确暂停
-> USER-PAUSED，不由通知恢复施工

没有活动计划段、工作区 / index clean 且没有其它写入占用
-> 刷新并重读 origin/main、计划索引和全部可执行候选
-> 自主选择一份并当轮 S0，PASS 后直接实施

没有活动计划段但工作区 / index dirty
-> 不开始 S0
-> DIRTY-BLOCKED

已有活动计划段
-> 不暂停、不切换、不扩大当前白名单
-> 当前段合法提交推送并恢复 clean 后立即运行继续门禁
```

禁止只回复“已收到”“下次恢复时处理”或等待另一条“继续”后结束。

## S0

开始写入、构建或运行前：

```text
读取 AGENTS.md 要求的共同入口
-> 读取计划索引、拟选叶子计划、计划状态和具名依赖
-> 读取关联规范、流程图、详细设计和实际代码接口或预冻结接口合同
-> 核对详细设计的节点实现、函数、数据、调用 / 事务、失败、代码映射、验证和空未决项
-> 核对允许 / 禁止文件、依赖、合同版本、所有权、验证矩阵和完成边界
-> 核对唯一主工作区、main、HEAD、origin/main、dirty state 和写入占用
-> 确认 main == origin/main、没有第二写入者或第二活动计划段
-> 把当前 HEAD 固定为本段唯一起点
-> 返回 PASS 或具名 DRIFT
```

被选计划必须为 `可执行`，具名真实依赖必须满足。执行智能体自行完成 S0，向配对计划支撑智能体发送结构化事实并明确 `是否请求设计包修订`；S0 PASS 必须写 `否`。PASS 后直接施工，不等待其它智能体接受。

预冻结接口合同可以使提供者实现尚不存在不构成开发启动门控，但不再授权并行施工。提供者和消费者由唯一执行智能体按计划段顺序在 main 上串行实现。

历史计划中的 worktree、任务分支、长期执行分支、分支完成待集成或集成工作区字段不再是 S0 门禁，也不得据此创建额外工作区。业务合同、允许 / 禁止文件、接口、真实依赖和验证要求继续有效。

## 执行算法

```text
从计划索引自主选择一份可执行且真实依赖满足的计划
-> 取得单工作区写入占用
-> 冻结 main 当前 HEAD、计划 blob、白名单和验证矩阵
-> 复核具名前置产物与实际接口或预冻结合同
-> 按计划机械阶段实施最小闭合切片
-> 只修改允许的代码 / 工程 / 自检 / 唯一专属施工记录 / 验证记录
-> 运行计划明确要求的全部验证，包括本计划必须完成的集成级验证
-> 精确提交计划段结果，或按计划许可提交具名 WIP
-> 提交正文写入 Plan-Id / Plan-Blob / Plan-Segment-Start / Plan-Segment-Result
-> 非强制推送 origin/main
-> 确认 main == origin/main 且工作区 / index clean，释放写入占用
-> 只向配对计划支撑智能体返回结构化事实
-> 重读计划索引并运行继续门禁
```

代码计划必须明确允许文件、禁止文件和验证命令。新增文件或修改 `入口.cpp` 时，还必须符合 `规范/代码文件建立归属与模块命名规范.md` 及计划中的文件分类 / 装配合同。

前置已满足后，内部结构写入、绑定、读回、发布或验证不符合预期时，按计划的内部逻辑错误路径停止并追根因；不得擅自改为普通失败或扩充计划。

实施验收、自检、基准或专项检查时，人读过程信息和结果展示只能调用现行统一日志系统，不得新增或保留 `std::cout`、`std::cerr`、`printf`、弹窗、界面文本或另建文件旁路。必须按设计使用具名预编译宏，并验证宏关闭后不求值仅供日志使用的参数，同时自检执行、结构化结果、失败计数、退出码、机器事实和生产行为保持不变；日志写入失败不得改变验收裁决，日志关闭不得关闭自检。

## 允许写入

只允许当前选中计划列出的代码、工程、自检文件，以及该计划唯一专属的施工记录和验证记录。施工 / 验证记录只保存已发生的计划身份、S0、实际接口、变更文件、验证、提交、推送、残余风险和完成边界。

禁止修改或移动：

```text
AGENTS.md
.codex/rules/**
.codex/skills/**
规范/**
流程图/**
规范/详细设计/**
知识图谱/**
计划/**
计划/计划索引.md
当前计划未明确列出的其它 Markdown
```

计划未明确允许的文件一律禁止。无关 WIP 不得修改、暂存、stash、清理或带入提交。

## 漂移与退回

发生任一情况时立即停止扩大范围：

- 当前不在 `main`，或 main 与 origin/main 不一致。
- 工作区存在其它智能体、用户或未知来源的 dirty 改动。
- 计划状态不是 `可执行`，或具名真实依赖未满足。
- 实际接口与计划假定契约不一致。
- 预冻结合同缺项、版本不一致，或需要补造接口 / 修改提供者独占文件。
- 需要修改禁止文件或中央治理文件。
- 计划段起点、所有权、HEAD 或远端身份漂移。
- 有效流程图 / 详细设计缺失、撤回、覆盖不全或未决项不为空。
- 发现需要新的业务裁决。

只向配对计划支撑智能体发送结构化消息，写明计划编号 / DQ、计划版本与 blob、唯一主工作区、main、计划段起点、实际接口、具名差异、已改文件、验证、提交、dirty state、`是否请求设计包修订：是 / 否` 和完成边界。

计划选择、S0 PASS、正常完成、纯代码错误、Git / 环境 / 权限问题和 `RULE-BLOCKED` 必须写 `否`。只有现行计划或其上游设计合同确实需要改变时才写 `是`。纯代码问题由执行智能体在白名单内追根因，不得虚报计划缺口。

计划 DRIFT 后，只有计划明确允许保留 WIP 时才能形成断点提交。无法合法提交且工作区仍 dirty 时，保持当前现场并停止；不得 stash、覆盖或切换下一计划。

## Git 收口

至少检查：

```powershell
git diff --check
git diff --cached --check
```

只暂存当前计划段允许文件；一个结果 / WIP 提交只能属于一个计划段。验证通过后非强制推送 `origin/main`。禁止 force、force-with-lease，以及未经用户明确要求的 merge、rebase、cherry-pick。

历史计划把完整构建或运行后置到独立集成工作区的，执行智能体必须在当前计划段提交前补齐这些验证；无法补齐时返回计划 / 验证合同 DRIFT，不得提交未经规定验证的结果。

## 继续门禁与停止条件

计划段提交推送且工作区 / index clean 后，重读 `origin/main` 计划索引、索引 blob、当前 HEAD、dirty state、计划段 Git trailer 和全部剩余 `可执行` 计划：

```text
发现候选 -> CONTINUE-REQUIRED -> 立即选择下一计划并重新 S0
没有候选 -> NO-CANDIDATE
用户明确暂停 -> USER-PAUSED
无法形成 clean 断点 -> DIRTY-BLOCKED
main / origin/main / 段起点漂移 -> MAIN-DRIFT
另一智能体仍持有写入占用 -> WRITE-OCCUPIED
规则或角色冲突 -> RULE-BLOCKED
环境不可用且无其它候选 -> ENVIRONMENT-BLOCKED
```

最终回执必须列出终止码、索引 blob、HEAD / origin/main、dirty state、最近计划段起点—结果提交和全部剩余候选的选择 / 排除结论。单计划完成、已推送、完成回执已送达、计划支撑智能体没有回复、长时间执行或没有新消息都不是停止条件。

平台轮次或上下文上限迫使当前轮让出时，只发送 `EXECUTION-YIELD`，固定当前活动计划或下一候选、HEAD、dirty state 和精确续点；不得宣称执行任务停止或完成。

不得用本技能声明计划归档、完整业务闭环、旧能力等价迁移、外设接通、自我循环、自我苏醒或成熟验收。
