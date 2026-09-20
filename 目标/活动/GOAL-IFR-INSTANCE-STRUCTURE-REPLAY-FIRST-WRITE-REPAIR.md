# GOAL-IFR-INSTANCE-STRUCTURE-REPLAY-FIRST-WRITE-REPAIR

- 目标编号：`GOAL-IFR-INSTANCE-STRUCTURE-REPLAY-FIRST-WRITE-REPAIR`
- 名称：实例特征结构首次与持久重放截止分流修复
- 类型：代码实施目标；记录版本：v0.1；创建与更新时间：2026-09-20；当前状态：当前
- 直接上位目标：`GOAL-INSTINCT-ROOT-DEMAND-PROVIDER-IMPLEMENTATION`；直接下位目标：无；目标账与代码切片所有者：当前交互智能体。
- 问题：未发布的持久重放修复把`out.Gread`统一设为写前当前`G0`。重放应使用该当前截止，但首次写入后的历史读回必须使用新增写入的`H`；新持久仓seed证明首次请求`G0=6`、写入形成`H=7`后被错误以6核验，返回`已可能发布`。
- 目标结果与完成条件：重放使用当前读回截止，首次使用新写入H；两条路径保持既有成功谓词、版本、交付、首次H和无新写入重放语义。只改`海中鱼巣/领域/数据服务.存在类.h`及专属施工/验证记录；仓外新持久仓双根seed/recover均通过并完成实际风险构建。不得改DTO、L1、持久格式、需求类、普通应用、计划、规范、启动或线程。
- 依据与切片：起点`9f7a791542c7beb784775ffca01400de0aca54ac`；现行函数`数据服务.存在类.h:4916-4945`，目标仅修正首次与重放的读回截止选择，不新增机器语义或公开合同。
- 状态证据：`D:\TEMP\海中鱼巣\ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER\20260920-s0\root-provider-persistence-probe\`的新`data-v2` seed输出`instance status=7 version=1 G0=6 Gread=6 firstH=0`并退出5。发布后自动重新S0上位目标；未覆盖资源失败、发布未知、阶段21、启动与集成验收。
