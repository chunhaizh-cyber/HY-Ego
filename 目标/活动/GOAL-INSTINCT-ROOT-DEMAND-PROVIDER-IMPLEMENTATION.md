# GOAL-INSTINCT-ROOT-DEMAND-PROVIDER-IMPLEMENTATION

## 1. 目标身份

- 目标编号：`GOAL-INSTINCT-ROOT-DEMAND-PROVIDER-IMPLEMENTATION`
- 名称：本能根独立需求物理合同代码实施
- 类型：代码实施目标
- 记录版本：v0.13
- 创建与更新时间：2026-09-20
- 当前状态：阻断
- 直接上位目标：`GOAL-INSTINCT-ROOT-PHYSICAL-CONTRACT`
- 直接下位目标：`GOAL-IFR-INSTANCE-STRUCTURE-RESULT-VERSION-REPAIR`（完成）、`GOAL-IFR-INSTANCE-STRUCTURE-PERSISTENT-REPLAY-REPAIR`（当前）；阶段21后继必须等待本目标与特征组合 provider 都已发布后重新形成
- 目标账所有者：交互智能体；代码切片所有者在S0通过后取得

## 2. 目标结果与完成条件

按已发布计划`ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER`实现需求 owner 的根目标合同、独立根需求、根列表项和唯一当前成员 provider。

完成条件：计划允许范围的代码与专属施工记录已发布；计划静态检查和规定验证已完成；结果只声明计划范围内的根材料 provider，不宣称特征组合 provider、阶段21、启动、恢复、线程或持续运行已完成。

## 3. 范围、依赖与下一动作

- 设计承载：`计划/20260920_ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER_本能根独立需求物理合同代码实施计划_v0.1.md`，v0.6 / blob `0c856f92b70e5537c69226ebbbb971b9b9f53f3a`，计划索引登记为`可执行`。
- 依赖：8121、5170、6120、6130及本计划；`EXISTENCE-IDENTITY-CONTRACT-REPAIR`已由`81aa94e3a`满足，不依赖特征组合 provider 的代码结果即可实施根材料 provider，但阶段21依赖两者。
- 明确排除：阶段21、特征当前采用写入、首值/状态/动态、线程、普通需求和任务合同迁移。
- 当前阻断：两进程持久L1探针已证明实例特征结构登记在根 provider 之前返回`已可能发布`，其首次材料可读但精确重复回执不再提供完整映射；该函数不属于本计划白名单。
- 下一动作：推进`GOAL-IFR-INSTANCE-STRUCTURE-PERSISTENT-REPLAY-REPAIR`；其发布后从新HEAD重新S0并恢复本目标的持久恢复专项验证。

## 4. 状态证据上下文

- 状态事实截止：`5d79014412e16cbfab3c6e2d2299a44ec4d4e6e3`
- 关联计划身份：`ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER`
- 关联计划观察版本 / blob：v0.6 / `0c856f92b70e5537c69226ebbbb971b9b9f53f3a`
- 最后实质进展：`f9febe7d7`发布登记/根材料重放收敛修复并以仓外真实L1驱动验证安全、服务双根与具名失败分支；Debug/Release根工程重建通过。
- 阻断解除证据：`GOAL-IFR-INSTANCE-STRUCTURE-PERSISTENT-REPLAY-REPAIR`发布后，以同一两进程持久L1探针证明实例特征结构成功重放，再从新HEAD重新S0。本目标此前`8ea62e5ed`的实例结果版本/进程内重放证据仍有效，但不覆盖本次持久恢复缺陷。
- 自动复核触发：相关依赖提交、计划/规范/接口/所有权变更、构建或验收回传，以及用户“同步”“继续”或“检查进展”。
