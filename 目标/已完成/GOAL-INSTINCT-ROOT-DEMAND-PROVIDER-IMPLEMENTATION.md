# GOAL-INSTINCT-ROOT-DEMAND-PROVIDER-IMPLEMENTATION

## 1. 目标身份

- 目标编号：`GOAL-INSTINCT-ROOT-DEMAND-PROVIDER-IMPLEMENTATION`
- 名称：本能根独立需求物理合同代码实施
- 类型：代码实施目标
- 记录版本：v0.21
- 创建与更新时间：2026-09-20
- 当前状态：完成
- 直接上位目标：`GOAL-INSTINCT-ROOT-PHYSICAL-CONTRACT`
- 直接下位目标：`GOAL-IFR-INSTANCE-STRUCTURE-RESULT-VERSION-REPAIR`（完成）、`GOAL-IFR-INSTANCE-STRUCTURE-PERSISTENT-REPLAY-REPAIR`（完成）、`GOAL-IFR-INSTANCE-STRUCTURE-REPLAY-FIRST-WRITE-REPAIR`（完成）；阶段21后继必须等待本目标与特征组合 provider 都已发布后重新形成
- 目标账所有者：交互智能体；代码切片所有者在S0通过后取得

## 2. 目标结果与完成条件

按已发布计划`ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER`实现需求 owner 的根目标合同、独立根需求、根列表项和唯一当前成员 provider。

完成条件：计划允许范围的代码与专属施工记录已发布；计划静态检查和规定验证已完成；结果只声明计划范围内的根材料 provider，不宣称特征组合 provider、阶段21、启动、恢复、线程或持续运行已完成。

## 3. 范围、依赖与下一动作

- 设计承载：`计划/已完成计划/20260920_ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER_本能根独立需求物理合同代码实施计划_v0.1.md`，v0.7 / blob `d5316b7b3ef5f555e374768b7338939228ff45fb`，计划索引登记为`可执行`。
- 依赖：8121、5170、6120、6130及本计划；`EXISTENCE-IDENTITY-CONTRACT-REPAIR`已由`81aa94e3a`满足，不依赖特征组合 provider 的代码结果即可实施根材料 provider，但阶段21依赖两者。
- 明确排除：阶段21、特征当前采用写入、首值/状态/动态、线程、普通需求和任务合同迁移。
- 当前阻断：无；自身完成条件已满足。
- 下一动作：终态；阶段21仅可在根材料与特征组合 provider 都已发布后，由上位目标重新形成新计划。

## 4. 状态证据上下文

- 状态事实截止：`d428c55959397aa8b91d6f44902ab54f44da2fed`
- 关联计划身份：`ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER`
- 关联计划观察版本 / blob：v0.7 / `d5316b7b3ef5f555e374768b7338939228ff45fb`
- 最后实质进展：`f57e3513ad1fddee674101e81cf6bd2263e165a0`发布根自有退出分流和`T/D/L`有界引用闭包；`f6d22b135`、`d428c5595`发布施工/验证记录。
- 阻断解除证据：`GOAL-IFR-INSTANCE-STRUCTURE-REPLAY-FIRST-WRITE-REPAIR`已由`37c3d7ae7f3203c5c4d2abfbe43e783f0e677071`解除计划外实例阻断；`7ca1f17b73f721f3395797dc3a381ed10637b024`已收敛本计划内需求结构和根材料的持久读回缺口。
- 完成证据：代码`f57e3513a`；施工/验证记录`f6d22b135`并由`d428c5595`补齐根间交叉与目标值/属性槽分支。公共夹具通过退出、重复成员、普通关系污染、根间交叉、额外非最大目标值、槽改写与写前旧截止；新持久仓两进程双根恢复和实例恢复通过；隔离 Debug/Release Rebuild 通过。直接缺项、发布未知和运行期资源失败均按合法注入缺口记录为`NOT_RUN`，未伪称运行通过。
- 未验证范围：特征组合 provider、阶段21、普通应用启动、线程、连续运行和正式集成验收；不属于本代码实施目标完成范围。
- 自动复核触发：终态；若未来需要改变其完成事实，必须建立新目标身份。
