# 迁移后消费者闭包审计代码实施计划

计划身份：`PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-POST-CONSUMER-CLOSURE-AUDIT`

日期：2026-09-18
版本：v0.1
状态：可执行

## 1. 目标、依赖与完成条件

本计划承接已退出的 `PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A`，不再尝试从已经不存在的 `.ixx/.cppcpp` 源对象生成迁移前清单。它执行一次对正式 HEAD 的迁移后消费者闭包审计：P（生产树/主工程）与 V（活动验证消费者）分账，形成每一个旧格式引用的可复跑证据与后续裁决输入。

唯一设计依据为[迁移后消费者闭包审计详细设计](../规范/详细设计/20260918_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-POST-CONSUMER-CLOSURE-AUDIT_迁移后消费者闭包审计详细设计_v0.1.md) v0.1。无代码、ABI、机器语义或提供者依赖；本计划只读正式输入并写专属记录，因此可以独立执行。

完成条件：

1. 以 S0 的正式 `main == origin/main`、index 为空的 HEAD archive 为唯一输入；
2. P/V/H 范围、文件 blob/hash、扩展清单、工程项和旧格式逐行命中均完整写入附件；
3. 每项 V 命中已人工分类为 `active-consumer`、`inactive-text` 或 `uncertain`；
4. P、V 和总闭包结果按详细设计的谓词明确给出；
5. 任何未闭合项仅形成下一代码计划的精确输入，不修改、删除、暂存或覆盖生产/验证消费者/WIP。

## 2. 允许范围与禁止事项

允许写入：

```text
施工记录/20260918_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-POST-CONSUMER-CLOSURE-AUDIT_施工记录_v0.1.md
验证记录/20260918_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-POST-CONSUMER-CLOSURE-AUDIT_验证记录_v0.1.md
施工记录/附件/20260918_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-POST-CONSUMER-CLOSURE-AUDIT/v0.1/**
```

禁止修改 `海中鱼巣/**`、`验证工具/**`、工程/filters/solution、正式规范、其它计划和当前工作区 WIP；禁止构建到仓库目录、`reset`、`clean`、`stash`、合并、覆盖或删除。若需临时脚本，只能存在本计划的 `D:\TEMP\海中鱼巣\<计划身份>\<唯一轮次>\` 目录，结果以附件中的命令、hash和结构化输出保存。

## 3. 实施合同

1. S0：核对仓库、分支、HEAD/origin、index、目标记录路径无异主 WIP；以 `git archive` 建立唯一 archive 输入。发现不满足时停止并在验证记录标为 `BLOCKED`。
2. 依据详细设计枚举 P/V/H；使用 archive 的 Git blob、SHA-256 与逐行搜索证据生成输入和命中 JSON。禁止从当前工作区 WIP 得出“已删除”结论。
3. 对 `海中鱼巣.vcxproj` 做 Debug/Release x64、`EnableD455RealSense=false` 的 `-getItem:ClCompile` 求值，并记录每个项的后缀和排除状态；不构建工程。
4. 生成头源同名候选表、活动消费者分类表和 P/V/H 汇总。所有不能确定活动性的项为 `uncertain`，不得猜测为可删除。
5. 生成施工与验证记录，明确 P/V/总结果、真实阻断、后继需要的最小裁决或代码计划输入。

## 4. 验证与完成声明边界

验证命令和断言依详细设计第 5 节执行：附件 JSON 可解析、blob/hash 可读回、工程项求值记录完整、命中逐行可复核、分类无空值、`git diff --check`、`python .\tools\check_specs.py --strict`。执行者必须分账工具或范围外 WIP 阻断。

计划完成只表示迁移后格式/消费者审计结果已经发布。即使 P/V 均 PASS，也不证明迁移行为等价、根工程编译链接、验证程序运行、恢复、数据兼容或业务闭环；任何 FAIL/BLOCKED 都不授权直接删除或迁移消费者。
