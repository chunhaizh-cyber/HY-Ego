# DATA-L2-FINAL-CLOSURE-P01-SCOPE 验证记录 v0.1

日期：2026-09-01

计划身份：`DATA-L2-FINAL-CLOSURE-P01-SCOPE`

编译候选基线：`45832e3796aa5002578dff46913d0703a4dd0016 + P01 精确覆盖`

最终规范候选基线：`aea4b16a529ddd7dafe8132cdb7405c0335edb76 + P01 精确覆盖`

当前结论：`PASS / P01 已落实`

## 1. 验证范围

本记录只验证 P01 的范围分类合同：

```text
当前 DATA-L2 活跃节点族 = 1—10、14
ARCH-L4 兼容节点族 = 11—13
两类互斥
未知值拒绝
既有工程仍可编译
```

不验证 P02—P12，不验证任一结构族 CRUD / 恢复 / 旧格式 / 生产消费者，也不升级为 L2 收口或 `FROZEN` 结论。

## 2. 隔离候选

隔离目录：

`D:\TEMP\海中鱼巣\DATA-L2-FINAL-CLOSURE-P01-SCOPE\81962f4d8216468da53948a806f1c8bb\repo`

形成方式：从施工起点 `git archive HEAD` 展开，然后只覆盖以下 P01 已跟踪候选：

```text
.codex/rules/层级命名空间规则.md
规范/0050_项目通用机器逻辑与禁止性规则总纲_20260721.md
规范/0300_规范冲突与前后矛盾清单_20260720.md
规范/4030_子规范_基础信息服务分层与领域写授权.md
规范/4230_子规范_L2六类纯结构统一合同.md
规范/4240_子规范_语言词条与概念空间纯结构统一合同.md
海中鱼巣/领域/L2结构公共.数据.h
```

共享工作区中的工程、装配和 ARCH-L4 WIP 未进入隔离候选。

## 3. 已执行结果

| 检查 | 结果 | 证据边界 |
| --- | --- | --- |
| P01 精确文件 `git diff --check` | PASS | 无空白错误；仅有 Git 的 LF→CRLF 工作区提示 |
| 定向冲突搜索 | PASS | 已消除 0050 当前正文仍把需求 / 任务 / 方法归入 DATA-L2 的冲突；历史修订条目不作为当前合同 |
| Debug x64 Rebuild，Visual Studio 18 Professional | PASS | 同一隔离候选重试后生成 `x64\Debug\海中鱼巣.exe` |
| Release x64 Rebuild，Visual Studio 18 Professional | PASS | 生成 `x64\Release\海中鱼巣.exe` |
| 工作区 `python .\tools\check_specs.py --strict` | FAIL（异主） | 115 项中 2 项错误，均为异主工程 WIP 把 9 个端到端测试登记到生产工程 / filters；P01 未修改这些文件 |
| 隔离 strict（目录交接前） | PASS | 以隔离目录为只读工作树、主仓 `.git` 为 HEAD / origin 证据运行，115 项全部通过；未带入异主工程 WIP |
| 目录同步后的最终隔离 strict | PASS | 基于 `aea4b16a... + P01 精确覆盖`，115 项全部通过 |

## 4. Debug 首次工具波动

第一次 Debug Rebuild 使用 Visual Studio 18 Community，在未触及的：

`海中鱼巣/领域/内部治理/服务.需求初次筹办准备.ixx:251`

触发 MSVC `C1001`，编译器内部位置为 `symbols.cpp:5758`。未修改任何源码，改用同机 Visual Studio 18 Professional 对同一隔离候选重新 Rebuild 后通过。Release 同样通过。因此本记录把首次失败限定为一次编译器 / 模块环境波动，不把它认定为 P01 缺陷，也不证明该业务模块不存在其它风险。

## 5. 编译期分类门禁

根工程成功编译包含公共头中的静态断言，已证明：

1. `概念 == 10`、`方法 == 11`、`需求 == 12`、`任务 == 13`、`不可变材料 == 14`。
2. 当前 DATA-L2 分类接受不可变材料，拒绝方法、需求和任务。
3. ARCH-L4 兼容分类接受方法、需求和任务，拒绝不可变材料。
4. `0` 与 `255` 等未定义枚举值被两类分类共同拒绝。
5. 现有方法、需求、任务模块仍能消费保持原值的兼容枚举并完成 Debug / Release 编译。

## 6. 最终门禁

以下事项已经全部完成：

1. 在 `aea4b16a...` 干净 HEAD 上同步 `规范/规范目录.md` 的 0050、0300、4030、4230、4240 版本和主题，并登记 v6.56。
2. 最终隔离候选执行 `python .\tools\check_specs.py --strict`，115 项全部通过。
3. 上游 `45832e... -> aea4b16a...` 只修改 23 份规范；生产代码树未变，因此先前 Debug / Release 通过的 P01 代码候选与最终候选相同。
4. 精确 staged diff 不包含异主 WIP；承载本记录的提交普通推送到 `origin/main` 后构成 P01 发布证据。
