# 迁移后消费者闭包审计详细设计

> 2026-09-23 头源迁移治理：本文保留的业务与合同设计，仅在与现行正式规范一致时可作为重设计输入。正文中的旧模块路径、模块导入导出、旧行号、旧工程登记和旧构造链已经失效，不再是当前代码事实、现行物理设计或施工依据；实施前必须按现行 `.h/.cpp`、工程登记和生产调用点重新设计并登记计划。


设计身份：`PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-POST-CONSUMER-CLOSURE-AUDIT`

日期：2026-09-18
版本：v0.1
状态：施工设计；只读审计，不修改生产代码、工程或验证消费者。

## 1. 目标与边界

本设计承接已退出的 `PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A`。后者的迁移前 `.ixx/.cppcpp` 输入已不再存在，故不能再用“扫描 59 个模块 TU”的方式证明任何当前事实。

本审计的唯一目标是对正式 HEAD 形成可复跑的消费者闭包账：确认主生产树和主工程是否只使用传统 `.h/.cpp + #include`，并找出仍主动编译、包含、导入、读取或引用旧模块格式的验证工程、验证源码和执行脚本。它不推断函数调用关系、实体行为等价、数据兼容或业务正确性。

审计对象分为三类：

| 类别 | 路径范围 | 通过条件 |
| --- | --- | --- |
| P：生产闭包 | `海中鱼巣/**`、`海中鱼巣.vcxproj`、`海中鱼巣.vcxproj.filters`、解决方案中的主工程登记 | 源树无 `.ixx/.cppcpp`；全部主工程 `ClCompile` 为 `.cpp`；全部 `ClInclude` 为 `.h/.inl`；生产源和工程 XML 无 C++ module 声明、`import`、旧后缀。 |
| V：活动验证消费者 | `验证工具/**` 中的 `.vcxproj`、`.cpp`、`.h`、`.ps1`、`.cmd`、`.bat`，以及其由工程或脚本实际引用的项目源路径 | 不存在编译、`#include`、`import`、脚本路径或工程项对 `.ixx/.cppcpp/module/import/export` 旧格式的活动依赖。 |
| H：历史叙述 | `计划/**`、`规范/**`、`施工记录/**`、`验证记录/**`、历史对话 | 不纳入零命中条件；旧名称仅作为历史文字保留。 |

任何 V 命中都使“全消费者闭包”未通过，但不使 P 的已转换事实失效。审计必须逐命中给出原文路径、行号、命中类别、它是否为可执行消费者、以及只能由后续代码计划裁决的处理方式（迁移、删除或另作长期保留）。

## 2. 固定输入与复跑方式

审计只读取 S0 时 `HEAD` 的 Git 对象。执行者建立 `D:\TEMP\海中鱼巣\PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-POST-CONSUMER-CLOSURE-AUDIT\<唯一轮次>\source\`，以 `git archive --format=tar <HEAD>` 解包；不得将工作区 WIP、未跟踪文件、构建产物或异主删除当作审计输入。

输入清单逐文件保存：仓库相对路径、Git blob、SHA-256、类别 P/V/H、发现命令。源扩展计数也必须从该 archive 生成。本设计在 `68ebb3cc` 的已知起点仅记录观察值（70 `.h`、50 `.cpp`、0 `.ixx`、0 `.cppcpp`），不把它当成后续 HEAD 的硬编码数量门槛。

对主工程 Debug/Release x64 以 `EnableD455RealSense=false` 分别运行 `MSBuild -getItem:ClCompile`。每条项目项必须记录 `Identity`、实际后缀、`ExcludedFromBuild` 与条件。只有同配置全部未排除项为 `.cpp`、且排除项本身不指向旧格式时，P 工程登记通过。

## 3. 机制与结果合同

审计器可以是仓库外 PowerShell 或 C++/Python 一次性脚本，但其输出是值式 JSON/Markdown，不成为生产能力。它执行以下固定步骤：

1. 记录 Git 顶层、分支、HEAD、`origin/main`、index 状态、archive 命令、工具版本和 archive 输入哈希；若 `main != origin/main` 或 index 非空，安全停止，不读取工作区替代。
2. 从 archive 枚举 P、V、H；对 P/V 查找路径后缀 `.ixx/.cppcpp`，工程 XML 中的相同后缀，以及 C++ 语法 `module`、`export module`、`import`/`export import`。搜索结果必须保留逐行原文锚点，不能只保存数量。
3. 对每个 V 命中由人工复核为 `active-consumer`、`inactive-text` 或 `uncertain`。只有其内容不参与工程、脚本、构建或运行入口时才可为 `inactive-text`；`uncertain` 与 `active-consumer` 都阻断 V 通过。
4. 对每个 `.cpp` 检查同基本名 `.h` 是否存在；`入口.cpp` 或计划已具名说明的例外单列，不可静默豁免。对每个 `.h` 和 `.cpp` 的生产实体单一所有权仅作词法候选，不把文本搜索假称语义证明。
5. 输出 P/V/H 三个结果、源/工程/消费者逐项清单、所有阻断项、复跑命令和已知未覆盖边界。P 与 V 分别为 `PASS`、`FAIL` 或 `BLOCKED`；总迁移后消费者闭包只有 P=PASS、V=PASS、且无 `uncertain` 时才可为 PASS。

非成功收口：Git 输入不稳定、archive 失败、工程 XML 无法解析、工具未定位、搜索命令失败或任何分类未完成时为 `BLOCKED`；旧格式活动命中为 `FAIL`。两者均不得修改、删除或暂存目标文件，亦不得将工作区中已删除但未提交的文件当作已解决。

## 4. 后继裁决

本审计若总结果为 PASS，才可由计划智能体依据审计输出建立“迁移功能与行为回归”或宣布 CODE-FILE-01 §3.4 的物理消费者闭包通过；仍不证明业务行为。

若 V 存在 `active-consumer`，必须建立一个新的代码计划，逐消费者冻结唯一处理：

- 该验证消费者仍需长期使用：迁移其公开头、源码、工程登记和脚本调用，并按其原验证合同运行；
- 该验证消费者已经被正式替代或不再允许使用：在计划中证明替代者或退役理由后物理删除；
- 无法从现行材料判断是否仍需保留：保持为 `uncertain`，由用户裁决用途，不允许执行窗口自行删除。

因此本设计不为验证工具预先选择“迁移”或“删除”，也不触碰当前异主删除 WIP。

## 5. 验证、文件范围与声明边界

计划执行仅允许写：

```text
施工记录/20260918_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-POST-CONSUMER-CLOSURE-AUDIT_施工记录_v0.1.md
验证记录/20260918_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-POST-CONSUMER-CLOSURE-AUDIT_验证记录_v0.1.md
施工记录/附件/20260918_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-POST-CONSUMER-CLOSURE-AUDIT/v0.1/**
```

执行时验证 JSON 可解析、每个输入 blob 可回读、工程求值可复跑、所有 P/V 命中有逐行锚点与分类、`git diff --check` 以及 `python .\tools\check_specs.py --strict`。后两项若受范围外 WIP 阻断必须如实分账。

本审计只证明所选正式 HEAD 的格式与消费者扫描事实。它不修改或证明 C++ 行为、编译链接运行、恢复、数据兼容、调用关系、函数物理语义或业务闭环。
