# MULTI-WORKTREE-S1 多工作树并发与集成规则治理计划 v0.1

日期：2026-07-18

队列编号：#308

设计队列：DQ-200

决策：JY-392

状态：规则治理完成 / 已归档 / 文档验证与提交推送待本轮闭合 / 不修改 C++ / 不构建不运行

## 1. 目标

撤销“只允许一个可写 worktree、只允许 main 承载任务提交”的旧限制，建立：

```text
main 主集成工作树
+ 多个独立任务 worktree
+ codex/ 任务分支
+ 预授权并行批次
+ 文件与结构所有权冻结
+ 集成暂存验证
+ origin/main 最终发布
```

## 2. 依据

用户明确授权修改规则并允许多 worktree。正式接管基线为 `main@4a89c16`；#296 尚未实施，代码与工程零修改。

本计划只治理协作、Git、计划路由和验证责任，不改变机器结构或业务调用路径，因此流程图与领域详细设计不适用。替代依据为用户口径、AGENTS、规则总纲、规则迁移清单、设计执行双窗口交互规范、JY-391 当前路由和执行前用户改向断点。

## 3. 实施范围

1. 建立多工作树并发与集成专项规范。
2. 修订 AGENTS、总纲、迁移清单和双窗口交互规范。
3. 修订 plan、execute、current-fact-scan、pre-implementation-review 四个项目技能。
4. 建立并行工作树登记表，初始只登记 WT-MAIN，不创建任务 worktree。
5. 同步计划索引、任务队列、当前状态、决策、实施 / 验收汇总和窗口事件。

## 4. 默认裁决

1. `D:\海中鱼巣` 继续是唯一 `main` 主集成工作树，不再是唯一可写 worktree。
2. 任务 worktree 必须使用独立 `codex/<队列号>-<主题>` 分支；任务分支不得直推 `origin/main`。
3. 同一 worktree 任一时刻只有一个写入角色；多个任务 worktree 只有在正式并行批次中才可并发。
4. 依赖、允许文件、结构所有权、公开接口或共享装配任一冲突即取消并行资格。
5. 任务分支完成只进入“分支完成待集成”；集成验证、main 发布和设计归档后三层齐备才完成。
6. 现有 #296 路线不因规则变化自动并行；必须另行登记 worktree、分支和冻结批次。

## 5. 允许与禁止范围

允许文件：本计划、AGENTS、四份规范、四个项目技能、计划索引、任务队列、当前状态、决策记录、实施 / 验收记录、窗口交互、并行工作树登记表和 JY-392 设计记录。

禁止修改 `海中鱼巣/**`、工程文件、流程图、领域详细设计、现有代码计划内容、日志、快照、数据库和任何 stash。禁止在本治理轮创建 worktree、任务分支、集成分支或运行构建 / 程序。

## 6. 验收

| 编号 | 验收项 |
| --- | --- |
| MULTI-WORKTREE-S1-A01 | main 保持唯一最终权威与 origin/main 发布入口 |
| MULTI-WORKTREE-S1-A02 | 任务 worktree 使用独立 codex/ 分支 |
| MULTI-WORKTREE-S1-A03 | 设计、执行、集成角色权限分离 |
| MULTI-WORKTREE-S1-A04 | 并行批次有共同基线、依赖、接口和文件所有权门禁 |
| MULTI-WORKTREE-S1-A05 | 共享治理、工程、入口和中央运行器具有唯一所有者 |
| MULTI-WORKTREE-S1-A06 | 任务完成与主线完成状态明确分离 |
| MULTI-WORKTREE-S1-A07 | 任务分支只推同名远端分支，不直推 main |
| MULTI-WORKTREE-S1-A08 | 集成失败不允许语义性临时消冲突 |
| MULTI-WORKTREE-S1-A09 | 多 worktree 不降低任务和集成验证等级 |
| MULTI-WORKTREE-S1-A10 | 漂移断点有正式纳入 main 的通道 |
| MULTI-WORKTREE-S1-A11 | 工作树登记表和完整状态机已建立 |
| MULTI-WORKTREE-S1-A12 | 回收前验证 main 已集成、worktree clean、状态可回收 |
| MULTI-WORKTREE-S1-A13 | 四个项目技能与正式规则一致 |
| MULTI-WORKTREE-S1-A14 | #296 未自动执行、未自动并行、890 未登记 |
| MULTI-WORKTREE-S1-A15 | C++、工程、流程图、领域详细设计和 stash 零变化 |
| MULTI-WORKTREE-S1-A16 | strict、skill 校验、diff、提交和非强制推送闭合 |

## 7. 验证

```powershell
git diff --check
python .\tools\check_specs.py --strict
python C:\Users\zhchh\.codex\skills\.system\skill-creator\scripts\quick_validate.py .\.codex\skills\hai-zhong-yu-chao-plan
python C:\Users\zhchh\.codex\skills\.system\skill-creator\scripts\quick_validate.py .\.codex\skills\hai-zhong-yu-chao-execute
python C:\Users\zhchh\.codex\skills\.system\skill-creator\scripts\quick_validate.py .\.codex\skills\hai-zhong-yu-chao-current-fact-scan
python C:\Users\zhchh\.codex\skills\.system\skill-creator\scripts\quick_validate.py .\.codex\skills\hai-zhong-yu-chao-pre-implementation-review
```

## 8. 完成边界

完成后只可声明项目已经允许在正式登记、文件和结构所有权互斥、接口冻结及集成验证约束下使用多个 worktree。不得声明已经创建并行批次、#296 已执行、迁移速度已经提升或任何业务能力已经完成。

## 9. 实施结果

1. 专项规范、工作树登记表、AGENTS、总纲、迁移清单、双窗口规范和四个项目技能已统一新口径。
2. #308 / DQ-200 已完成并直接归档；#296 未消费，890 未登记。
3. `git diff --check` 和 `python .\tools\check_specs.py --strict` 通过。
4. 四次 `quick_validate.py` 均因当前 Python 和 Codex bundled Python 缺少 `PyYAML` 而未能加载脚本；未为本轮文档治理修改全局 Python。已按同一脚本的 frontmatter 规则逐项人工校验四个技能，名称、必填字段、允许键、长度和字符约束全部通过。
5. C++、工程、流程图、领域详细设计和 stash 零修改；未构建、未运行、未创建任务 / 集成分支或 worktree。
