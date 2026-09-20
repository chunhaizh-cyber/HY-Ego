# GOAL-INSTINCT-ROOT-DEMAND-PROVIDER-IMPLEMENTATION

## 1. 目标身份

- 目标编号：`GOAL-INSTINCT-ROOT-DEMAND-PROVIDER-IMPLEMENTATION`
- 名称：本能根独立需求物理合同代码实施
- 类型：代码实施目标
- 记录版本：v0.6
- 创建与更新时间：2026-09-20
- 当前状态：阻断
- 直接上位目标：`GOAL-INSTINCT-ROOT-PHYSICAL-CONTRACT`
- 直接下位目标：无；技术前置`GOAL-EXISTENCE-IDENTITY-CONTRACT-REPAIR`不改变本目标层级，只在其完成前阻断本计划；阶段21后继必须等待本目标与特征组合 provider 都已发布后重新形成
- 目标账所有者：交互智能体；代码切片所有者在S0通过后取得

## 2. 目标结果与完成条件

按已发布计划`ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER`实现需求 owner 的根目标合同、独立根需求、根列表项和唯一当前成员 provider。

完成条件：计划允许范围的代码与专属施工记录已发布；计划静态检查和规定验证已完成；结果只声明计划范围内的根材料 provider，不宣称特征组合 provider、阶段21、启动、恢复、线程或持续运行已完成。

## 3. 范围、依赖与下一动作

- 设计承载：`计划/20260920_ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER_本能根独立需求物理合同代码实施计划_v0.1.md`，v0.5 / blob `67b89666d0a1203fca3b9430bf74f4db2c24780d`，计划索引登记为`待激活`；其唯一直接前置是`EXISTENCE-IDENTITY-CONTRACT-REPAIR`。
- 依赖：8121、5170、6120、6130、`EXISTENCE-IDENTITY-CONTRACT-REPAIR`及本计划；不依赖特征组合 provider 的代码结果即可实施根材料 provider，但阶段21依赖两者。
- 明确排除：阶段21、特征当前采用写入、首值/状态/动态、线程、普通需求和任务合同迁移。
- 当前阻断：`海中鱼巣/领域/数据服务.存在类.h`在当前正式基线缺少`存在信息身份`定义；需求服务头的合法依赖链无法解析，根工程和隔离头编译均在该上游文件停止。该文件不在`ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER`允许写入范围，不能由本切片修复。
- 下一动作：推进当前小目标`GOAL-EXISTENCE-IDENTITY-CONTRACT-REPAIR`的唯一计划；其结果和独立头翻译证据发布后，自动重新S0并以新基线复核本计划。

## 4. 状态证据上下文

- 状态事实截止：`54682cadfb020f702eb2051aad6e98b80cdd4010`
- 关联计划身份：`ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER`
- 关联计划观察版本 / blob：v0.5 / `67b89666d0a1203fca3b9430bf74f4db2c24780d`
- 最后实质进展：已在计划范围内形成未发布 WIP：需求 owner 8类型登记、根材料 DTO/固定身份/写集和受限普通应用持有/accessor；它尚未构成代码完成证据。
- 当前阻断：2026-09-20 在`288f81d9f3c279132d1d6a72aabfb6b663fbf004`重做S0后，根工程首次构建受共享`Debug/vc145.pdb`占用；改用`D:\TEMP\海中鱼巣\ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER\build-debug\`隔离重建及独立头编译，均先在当前正式`数据服务.存在类.h:235/250/265`的未声明`存在信息身份`失败。`git diff`确认该依赖文件并非本工作区改动，且它不在本计划允许文件，不能以本切片越权修改。
- 阻断解除证据：`EXISTENCE-IDENTITY-CONTRACT-REPAIR`以公开值式身份声明和独立头翻译验证发布；随后必须重新S0、重建并完成本计划验证，未发布 WIP 不得作为解除证据。
- 自动复核触发：相关依赖提交、计划/规范/接口/所有权变更、构建或验收回传，以及用户“同步”“继续”或“检查进展”。
