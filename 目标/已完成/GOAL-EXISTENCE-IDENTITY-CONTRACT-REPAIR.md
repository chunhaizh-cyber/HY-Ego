# GOAL-EXISTENCE-IDENTITY-CONTRACT-REPAIR

## 1. 目标身份与终态

- 目标编号：`GOAL-EXISTENCE-IDENTITY-CONTRACT-REPAIR`
- 名称：存在信息身份公开合同修复
- 类型：代码实施目标
- 记录版本：v0.2
- 创建日期：2026-09-20
- 终态日期：2026-09-20
- 当前状态：完成
- 直接上位目标：`GOAL-INSTINCT-RUNTIME`
- 直接下位目标：无；完成后解除`GOAL-INSTINCT-ROOT-DEMAND-PROVIDER-IMPLEMENTATION`的唯一具名编译阻断
- 目标账所有者：交互智能体；代码切片所有者在S0通过后取得

## 2. 目标问题、结果与边界

`7c5cce844`发布的 IF/R ABI 在存在类和消费者中使用了`存在信息身份`，但当前正式头没有定义它，令合法包含链无法翻译。此目标补齐该已使用的公开值式身份，以恢复根材料 provider 的验证前提。

完成条件：`EXISTENCE-IDENTITY-CONTRACT-REPAIR`允许范围内的声明、专属施工/验证记录已发布；差异检查和独立头翻译验证已完成。结果只声明公开头重新可解析，不声明 IF/R 行为、根材料 provider、根工程构建、启动、恢复或本能运行完成。

明确排除：任何存在事实读写、IF/R算法、普通应用、需求服务、工程登记、根材料 WIP、正式规范和阶段21。

## 3. 设计承载、依赖与下一动作

- 设计承载：`规范/详细设计/20260915_INSTANCE-FEATURE-IF-R-CONCEPT-FOUNDATION_实例特征容器与区间概念汇聚详细设计_v0.1.md` v0.11，以及`计划/已完成计划/20260920_EXISTENCE-IDENTITY-CONTRACT-REPAIR_存在信息身份公开合同修复代码实施计划_v0.1.md` v0.1 / blob `089f49f4cce8ddf5b04f7e911d1a34595449cff4`。
- 依赖：1140、4230、4115、已发布 IF/R ABI和当前正式`数据服务.存在类.h`；不依赖根材料 WIP或特征组合 provider。
- 当前阻断：终态无；后续根材料验证不得重新打开本目标。
- 后继：`GOAL-INSTINCT-ROOT-DEMAND-PROVIDER-IMPLEMENTATION`已在同一事实事务恢复为当前，并必须独立重新S0。

## 4. 状态证据上下文

- 状态事实截止：`81aa94e3ae71bbfa846830ff38ba3ca334f74488`
- 关联计划身份：`EXISTENCE-IDENTITY-CONTRACT-REPAIR`
- 关联计划观察版本 / blob：v0.1 / `089f49f4cce8ddf5b04f7e911d1a34595449cff4`，已移入`计划/已完成计划/`。
- 完成证据：`81aa94e3ae71bbfa846830ff38ba3ca334f74488`；`施工记录/EXISTENCE-IDENTITY-CONTRACT-REPAIR/施工记录_v0.1.md`；`验证记录/EXISTENCE-IDENTITY-CONTRACT-REPAIR/验证记录_v0.1.md`。独立 MSVC C++20 包含翻译通过，原未声明身份首错消失。
- 未验证范围：IF/R 行为、根工程、根材料 provider、启动、恢复、线程和集成验收。
- 终态边界：本目标只完成已发布公开头的可解析修复；后续若发现不同的机器语义或行为缺口，必须建立新的目标身份，不能重新打开本记录。
