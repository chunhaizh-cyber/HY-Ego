# GOAL-INSTINCT-ROOT-DEMAND-PROVIDER-IMPLEMENTATION

## 1. 目标身份

- 目标编号：`GOAL-INSTINCT-ROOT-DEMAND-PROVIDER-IMPLEMENTATION`
- 名称：本能根独立需求物理合同代码实施
- 类型：代码实施目标
- 记录版本：v0.19
- 创建与更新时间：2026-09-20
- 当前状态：阻断
- 直接上位目标：`GOAL-INSTINCT-ROOT-PHYSICAL-CONTRACT`
- 直接下位目标：`GOAL-IFR-INSTANCE-STRUCTURE-RESULT-VERSION-REPAIR`（完成）、`GOAL-IFR-INSTANCE-STRUCTURE-PERSISTENT-REPLAY-REPAIR`（完成）、`GOAL-IFR-INSTANCE-STRUCTURE-REPLAY-FIRST-WRITE-REPAIR`（完成）；阶段21后继必须等待本目标与特征组合 provider 都已发布后重新形成
- 目标账所有者：交互智能体；代码切片所有者在S0通过后取得

## 2. 目标结果与完成条件

按已发布计划`ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER`实现需求 owner 的根目标合同、独立根需求、根列表项和唯一当前成员 provider。

完成条件：计划允许范围的代码与专属施工记录已发布；计划静态检查和规定验证已完成；结果只声明计划范围内的根材料 provider，不宣称特征组合 provider、阶段21、启动、恢复、线程或持续运行已完成。

## 3. 范围、依赖与下一动作

- 设计承载：`计划/20260920_ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER_本能根独立需求物理合同代码实施计划_v0.1.md`，v0.6 / blob `0c856f92b70e5537c69226ebbbb971b9b9f53f3a`，计划索引登记为`可执行`。
- 依赖：8121、5170、6120、6130及本计划；`EXISTENCE-IDENTITY-CONTRACT-REPAIR`已由`81aa94e3a`满足，不依赖特征组合 provider 的代码结果即可实施根材料 provider，但阶段21依赖两者。
- 明确排除：阶段21、特征当前采用写入、首值/状态/动态、线程、普通需求和任务合同迁移。
- 当前阻断：计划v0.6第7节要求原身份可能发布、资源失败、写后漂移与根不完整/污染/重复成员分支，但未冻结合法故障注入或测试夹具边界；需求服务独占持有根owner写端口，L1公开服务也无相应注入入口。执行者不能通过私有访问、伪造L1结果或修改非白名单合同补造验证。
- 下一动作：等待计划支撑在本计划原目标内补充合法注入/夹具设计；修订发布后从新HEAD重新S0并恢复故障矩阵验证。

## 4. 状态证据上下文

- 状态事实截止：`50c8a3cf3bffd4ac9ed57beceaa7bb67e359ce7a`
- 关联计划身份：`ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER`
- 关联计划观察版本 / blob：v0.6 / `0c856f92b70e5537c69226ebbbb971b9b9f53f3a`
- 最后实质进展：`7ca1f17b73f721f3395797dc3a381ed10637b024`发布需求结构与根材料持久恢复的首次H/当前G读回截止分流；新持久仓双根seed、第二进程根恢复、实例恢复以及根工程Debug/Release隔离Rebuild均通过。
- 阻断解除证据：`GOAL-IFR-INSTANCE-STRUCTURE-REPLAY-FIRST-WRITE-REPAIR`已由`37c3d7ae7f3203c5c4d2abfbe43e783f0e677071`解除计划外实例阻断；`7ca1f17b73f721f3395797dc3a381ed10637b024`已收敛本计划内需求结构和根材料的持久读回缺口。
- 阻断证据来源：`验证记录/ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER/安全WIP验证记录_v0.1.md`续行v0.4；仓外夹具的有效性边界和L1公开接口已读回。阻断解除条件：计划支撑发布原目标内的合法注入/夹具设计，明确允许文件、所有权、合同与验证方法。
- 自动复核触发：计划支撑修订、相关规范/接口/所有权变更、构建或验收回传，以及用户“同步”“继续”或“检查进展”。
