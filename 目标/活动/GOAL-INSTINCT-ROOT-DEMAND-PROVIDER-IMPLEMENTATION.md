# GOAL-INSTINCT-ROOT-DEMAND-PROVIDER-IMPLEMENTATION

## 1. 目标身份

- 目标编号：`GOAL-INSTINCT-ROOT-DEMAND-PROVIDER-IMPLEMENTATION`
- 名称：本能根独立需求物理合同代码实施
- 类型：代码实施目标
- 记录版本：v0.8
- 创建与更新时间：2026-09-20
- 当前状态：阻断
- 直接上位目标：`GOAL-INSTINCT-ROOT-PHYSICAL-CONTRACT`
- 直接下位目标：无；技术前置`GOAL-IFR-FEATURE-IDENTITY-CONSUMER-REPAIR`不改变本目标层级，只在其完成前阻断根工程验证；阶段21后继必须等待本目标与特征组合 provider 都已发布后重新形成
- 目标账所有者：交互智能体；代码切片所有者在S0通过后取得

## 2. 目标结果与完成条件

按已发布计划`ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER`实现需求 owner 的根目标合同、独立根需求、根列表项和唯一当前成员 provider。

完成条件：计划允许范围的代码与专属施工记录已发布；计划静态检查和规定验证已完成；结果只声明计划范围内的根材料 provider，不宣称特征组合 provider、阶段21、启动、恢复、线程或持续运行已完成。

## 3. 范围、依赖与下一动作

- 设计承载：`计划/20260920_ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER_本能根独立需求物理合同代码实施计划_v0.1.md`，v0.6 / blob `0c856f92b70e5537c69226ebbbb971b9b9f53f3a`，计划索引登记为`可执行`。
- 依赖：8121、5170、6120、6130及本计划；`EXISTENCE-IDENTITY-CONTRACT-REPAIR`已由`81aa94e3a`满足，不依赖特征组合 provider 的代码结果即可实施根材料 provider，但阶段21依赖两者。
- 明确排除：阶段21、特征当前采用写入、首值/状态/动态、线程、普通需求和任务合同迁移。
- 当前阻断：根工程 Debug Rebuild 在`海中鱼巣/业务/应用服务.特征概念类.cpp:601`失败：IF/R 的`构造目标`期待`optional<特征信息身份>`，而 I64 出生事实`F`是裸`稳定编码`。该文件不在本计划白名单，不能由根材料切片修复。
- 下一动作：推进直接局部修复目标`GOAL-IFR-FEATURE-IDENTITY-CONSUMER-REPAIR`；其发布并通过同一隔离 Debug Rebuild 后，自动重新S0根材料 WIP。

## 4. 状态证据上下文

- 状态事实截止：`9597075a49f2dcc9e50eadf6c0d04d3216adbc40`
- 关联计划身份：`ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER`
- 关联计划观察版本 / blob：v0.6 / `0c856f92b70e5537c69226ebbbb971b9b9f53f3a`
- 最后实质进展：需求 owner WIP 的独立头翻译通过；隔离根工程 Debug Rebuild确认存在类身份首错已消失，但出现IF/R消费者类型不匹配。
- 当前阻断：`D:\TEMP\海中鱼巣\ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER\build-debug-rerun\msbuild.stdout.log`记录唯一编译错误 C2664，根因是`应用服务.特征概念类.cpp:602`将`原子I64特征出生事实.F`裸编码传入`optional<特征信息身份>`。
- 阻断解除证据：`GOAL-IFR-FEATURE-IDENTITY-CONSUMER-REPAIR`发布精确强类型包装并在同一隔离根工程 Debug Rebuild中不再出现该错误；随后必须重新S0、重建并完成本计划验证，未发布根材料 WIP 不得作为完成证据。
- 自动复核触发：相关依赖提交、计划/规范/接口/所有权变更、构建或验收回传，以及用户“同步”“继续”或“检查进展”。
