# REMOVE-LEGACY-ACCEPTANCE-CODE 生产内旧验收体系物理退出详细设计 v0.2

日期：2026-08-07

计划身份：`REMOVE-LEGACY-ACCEPTANCE-CODE`

设计状态：创建侧已冻结，待正式发布与计划索引登记

## 1. 目标与完成边界

本叶只把已经由正式规范裁决退出的旧生产内验收体系从生产源码、工程、入口、DTO、日志和私有实现中物理删除。完成后，生产目标不再编译自检运行器、验收专用模块、验收启动模式、验收宏、故障注入、私有访问器、计数器或验收结果聚合；不得把这些内容改名迁移成双目标、状态矩阵、执行证据矩阵或第二生产目标。

本叶不实现新 L1 中性 CRUD，不修改生产热路径仍在使用的 legacy 业务标签、意图组、领域结果见证或执行证据合同，不删除历史施工 / 验证 / 集成验收记录，也不宣称任何服务通过新黑盒验收。

## 2. 正式依据与当前基线

正式基线：`main == origin/main == 9c984b869c1dd32e2c0234c0c176b6214a48883a`，Git index 空。

直接依据：

- `规范/4015_子规范_L1简化事实基座.md` v0.4；
- `规范/8120_子规范_程序入口启动模式与运行宿主边界_20260726.md` v0.2；
- `规范/运行期服务导向代码验收与全函数入口巡检规范.md` v0.2；
- `规范/日志系统规范.md` v0.5；
- `规范/4030_子规范_基础信息服务分层与领域写授权.md`；
- `规范/4040_子规范_不透明结构事务候选确认撤销与最后发布.md`；
- `规范/4050_子规范_入口拒绝逻辑内结果与内部逻辑错误.md`；
- `规范/4070_子规范_权威结构快照恢复候选与运行期原子发布.md`；
- `规范/4200_子规范_世界树业务服务与同代次完整事实快照.md`。

当前事实：

1. 名称含“自检”的源码共 72 个；71 个进入 `海中鱼巣.vcxproj`，而 `海中鱼巣.vcxproj.filters` 当前只有 67 个对应现有节点；以下 4 个工程编译项没有 filters 节点，且不得由本叶补造：`海中鱼巣/领域/自检.L1特征定义.ixx`、`海中鱼巣/领域/自检.L1实例特征.ixx`、`海中鱼巣/领域/自检.世界登记.ixx`、`海中鱼巣/领域/自检.L1场景结构.ixx`。`海中鱼巣/领域/自检.分层安全维护与任务权限.ixx` 是未编译残留。
2. 非自检源码只有 `海中鱼巣/启动.应用程序.ixx` import 自检模块，共 21 个 import；没有其它生产模块 import 自检模块。
3. `启动.应用程序.ixx` 仍有 19 个验收包装函数、20 个 `登记自检` 项和完整 / 数据库 / 性能 / D455 四类验收路由。
4. `启动.程序入口.ixx` 仍解析四个旧验收 flag；`启动选项.数据.h` 与 `程序运行结果.数据.h` 仍承载验收模式、状态、阶段和摘要。
5. `HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST` 仍由 Debug x64 工程默认定义，并出现在 21 个非自检生产源码中；四个 `HY_EGO_DEBUG_LOG_SELF_TEST_*` 宏仍由工程、日志路径和启动包装共同承载。
6. `程序运行结果` 类型只被 `程序运行结果.数据.h`、`启动.应用程序.ixx`、`启动.程序运行宿主.ixx`、`启动.程序入口.ixx` 和 `入口.cpp` 引用；删除 `专项` 字段后，实际聚合初始化修改点只有前两个运行实现文件，入口两文件只读状态，不需要改动。

## 3. 完整删除文件表

以下 72 个文件全部删除。它们是验收运行器、夹具、包装或验收矩阵，不具有独立生产调用；生产源码对它们的 import 只有 `启动.应用程序.ixx` 的 21 项。

```text
海中鱼巣/核心/自检.仓库权威导出.ixx
海中鱼巣/核心/自检.仓库统一合同.ixx
海中鱼巣/核心/自检.关系仓库性能基线.ixx
海中鱼巣/核心/自检.节点直接结构查询.ixx
海中鱼巣/核心/自检.节点直接身份结构写入.ixx
海中鱼巣/核心/自检.节点直接统一冻结骨架.ixx
海中鱼巣/核心/自检.结构事务.ixx
海中鱼巣/核心/自检.结构写入会话.ixx
海中鱼巣/核心/自检.索引所有者声明.ixx
海中鱼巣/核心/自检.未发布身份候选.ixx
海中鱼巣/核心/自检.L1可重建索引.ixx
海中鱼巣/核心/自检.L1事实基座.ixx
海中鱼巣/领域/自检.存在场景分层.ixx
海中鱼巣/领域/自检.存在状态同会话参与.ixx
海中鱼巣/领域/自检.动态聚类.ixx
海中鱼巣/领域/自检.动态模式.ixx
海中鱼巣/领域/自检.方法动作场景关系.ixx
海中鱼巣/领域/自检.分层安全维护与任务权限.ixx
海中鱼巣/领域/自检.概念安全删除.ixx
海中鱼巣/领域/自检.概念活动所有权.ixx
海中鱼巣/领域/自检.概念命名目标.ixx
海中鱼巣/领域/自检.概念清理适配.ixx
海中鱼巣/领域/自检.概念删除后验.ixx
海中鱼巣/领域/自检.概念删除候选.ixx
海中鱼巣/领域/自检.概念生命周期.ixx
海中鱼巣/领域/自检.概念图结构分层.ixx
海中鱼巣/领域/自检.节点直接世界结构骨架.ixx
海中鱼巣/领域/自检.任务筹办选择.ixx
海中鱼巣/领域/自检.任务生命周期初态.ixx
海中鱼巣/领域/自检.任务生命周期迁移.ixx
海中鱼巣/领域/自检.世界登记.ixx
海中鱼巣/领域/自检.特征比较.ixx
海中鱼巣/领域/自检.特征体系分层.ixx
海中鱼巣/领域/自检.特征值原始材料事务.ixx
海中鱼巣/领域/自检.需求任务方法分层.ixx
海中鱼巣/领域/自检.因果模式.ixx
海中鱼巣/领域/自检.因果完整结构键.ixx
海中鱼巣/领域/自检.用途观察.ixx
海中鱼巣/领域/自检.用途观察结构.ixx
海中鱼巣/领域/自检.用途事件.ixx
海中鱼巣/领域/自检.语义基础分层.ixx
海中鱼巣/领域/自检.状态动态分层.ixx
海中鱼巣/领域/自检.状态动态原子发布.ixx
海中鱼巣/领域/自检.L1场景结构.ixx
海中鱼巣/领域/自检.L1动态.ixx
海中鱼巣/领域/自检.L1方法登记根当前读取.ixx
海中鱼巣/领域/自检.L1实际存在.ixx
海中鱼巣/领域/自检.L1实例特征.ixx
海中鱼巣/领域/自检.L1特征定义.ixx
海中鱼巣/领域/自检.L1状态.ixx
海中鱼巣/适配/自检.D455真实样本.ixx
海中鱼巣/线程/自检.概念命名需求.ixx
海中鱼巣/线程/自检.概念命名治理.ixx
海中鱼巣/线程/自检.任务结果结算.ixx
海中鱼巣/线程/自检.任务执行调度.ixx
海中鱼巣/线程/自检.运行线程.ixx
海中鱼巣/线程/自检.自我治理闭环.ixx
海中鱼巣/线程/自检.自我治理多轮.ixx
海中鱼巣/线程/自检.自我治理回执.ixx
海中鱼巣/线程/自检.自我治理协议.ixx
海中鱼巣/线程/自检.D455采样材料.ixx
海中鱼巣/自检.服务写入兼容边界.ixx
海中鱼巣/自检.控制面板按需投影.ixx
海中鱼巣/自检.入口初始化.ixx
海中鱼巣/自检.生产运行期首发.ixx
海中鱼巣/自检.系统角色初始化.ixx
海中鱼巣/自检.性能门禁分离.ixx
海中鱼巣/自检.运行期兼容请求.ixx
海中鱼巣/自检.运行期上下文.ixx
海中鱼巣/自检.运行期组合分层.ixx
海中鱼巣/自检.运行器.ixx
海中鱼巣/自检.主装配事务.ixx
```

## 4. 生产文件逐文件修改裁决

### 4.1 宏包裹验收探针：21 文件

以下每个文件删除全部由 `HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST` 控制的声明、定义、成员、分支、故障注入、调用计数和私有观测入口；删除相邻只为这些块存在的 include。宏外正式生产逻辑、返回状态、原子候选 / 确认 / 撤销 / 发布 / 读回和错误映射保持原义。

```text
海中鱼巣/启动.程序入口.ixx
海中鱼巣/启动.应用程序.ixx
海中鱼巣/核心/仓库.可重建索引.ixx
海中鱼巣/核心/服务.节点直接结构.ixx
海中鱼巣/核心/会话.结构写入.ixx
海中鱼巣/核心/节点仓库.cpp
海中鱼巣/核心/节点仓库.h
海中鱼巣/核心/索引仓库.cpp
海中鱼巣/核心/索引仓库.h
海中鱼巣/核心/协调.结构事务.ixx
海中鱼巣/核心/执行器.结构写入.ixx
海中鱼巣/领域/参与者.特征批次发布记录.ixx
海中鱼巣/领域/服务.方法.ixx
海中鱼巣/领域/服务.任务.ixx
海中鱼巣/领域/服务.需求.ixx
海中鱼巣/领域/控制面板服务.h
海中鱼巣/领域/数据操作.概念图结构.ixx
海中鱼巣/领域/数据操作.特征体系.ixx
海中鱼巣/领域/数据操作.需求任务方法.ixx
海中鱼巣/领域/数据操作.状态动态.ixx
海中鱼巣/领域/组合.状态动态.ixx
```

### 4.2 未由该宏保护的纯验收承载：8 文件

| 文件 | 必须删除 / 修订 | 必须保留 |
| --- | --- | --- |
| `海中鱼巣/核心/仓库.L1事实基座.ixx` | 全部 `_DEBUG` 自检入口、六个一次性损坏 / 异常标志、对应热路径注入分支和并发占锁夹具 | 三类事实仓、普通提交、当前 / 历史 / 审计 / 快照 / 恢复生产合同 |
| `海中鱼巣/核心/服务.L1事实基座.ixx` | 三个 `_DEBUG` 自检转发入口 | 全部公开生产服务入口 |
| `海中鱼巣/核心/索引.L1事实基座.ixx` | `_DEBUG` 的 `自检_损坏下一次重建候选` | 正式可重建索引逻辑 |
| `海中鱼巣/核心/执行器.节点直接身份结构写入.ixx` | `节点直接混合原子写自检阶段`、`节点直接混合原子写自检故障`、`自检设置下一次混合原子写故障`、friend、`下一次混合原子写故障_`、故障编码读取、`命中故障` lambda 及所有注入条件；删除条件时保留同一行原生产确认 / 撤销 / 发布 / 读回判断 | 节点直接身份结构事务域及正式原子写 |
| `海中鱼巣/核心/事件日志段服务.cpp` | 事件段 `自检` 子目录及其路径放行分支 | 正式事件段目录、路径越界拒绝和人读事件日志能力 |
| `海中鱼巣/领域/服务.L1方法登记根当前读取.ixx` | `L1方法登记根当前读取自检访问器` friend；把诊断责任注释收敛到公开结构化结果 | R1 已发布的正式当前读取服务和结构化结果 |
| `海中鱼巣/领域/服务.L1动态.ixx` | 仅把四处“当前自检边界”诊断注释改为“公开结构化结果或未来合法消费者” | P14 生产服务、DTO 和错误传播 |
| `海中鱼巣/领域/服务.状态动态原子发布.ixx` | 仅把“顺序400专项”诊断注释改为“公开结构化结果或未来合法消费者” | P16 生产服务、DTO 和错误传播 |

### 4.3 工程、入口、DTO、日志和聚合初始化：6 文件

| 文件 | 冻结修改 |
| --- | --- |
| `海中鱼巣.vcxproj` | 删除 71 个自检编译项；删除四个自检日志开关 / Definition 属性及四配置中的引用；删除 Debug x64 默认 `HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST`；保留 `EnableD455RealSense`、D455 生产预处理定义和正式生产源码项 |
| `海中鱼巣.vcxproj.filters` | 仅删除当前存在的 67 个自检 `ClCompile` filter 节点；上述 4 个无 filters 节点的工程项不补节点、不改其它 filter |
| `海中鱼巣/启动选项.数据.h` | `启动模式` 只保留普通控制面板、无窗口常驻、生产运行期；删除 `完整自检`、`数据库专项`、`关系仓库性能专项`、`D455真实样本专项`；删除只服务旧模式的 `构建能力未启用` |
| `海中鱼巣/程序运行结果.数据.h` | 删除 `自检失败`、`专项失败`、三个验收失败阶段、`程序专项类别`、`程序专项摘要` 和 `程序运行结果::专项`；生产状态显式数值不重排，`内部不一致` 保持值 8；结果只保留模式、状态、失败阶段和 `成功()` |
| `海中鱼巣/启动.程序运行宿主.ixx` | 所有 `程序运行结果` 聚合初始化由四字段改为三字段；控制面板 / 无窗口生产状态和失败阶段不变 |
| `海中鱼巣/核心/日志系统.h` | 删除四个 `SELF_TEST_*` 路径特判、四个 `HY_EGO_DEBUG_LOG_SELF_TEST_*` 宏和四个中文包装宏；保留通用具名调试日志、逻辑错误 / 运行 / 事件日志 |

`启动.应用程序.ixx` 与 `启动.程序入口.ixx` 已在 4.1 表内，同时还必须完成：

1. `启动.应用程序.ixx` 删除 21 个自检 import、19 个包装函数、20 个登记项、四类专项运行函数及四个 switch 路由；保留 `运行生产运行期模式`、`运行普通程序` 和 `运行海中鱼巣` 的三个生产路由。
2. `运行生产运行期模式` 失败时把旧 `程序运行状态::专项失败` 改为既有生产状态 `程序运行状态::初始化失败`，继续保留 `程序失败阶段::生产运行期首发`。这是对生产会话 `启动()` 失败的既有生产状态映射，不新增状态、不改变退出码 1。
3. `启动.应用程序.ixx` 中剩余 `程序运行结果` 聚合初始化全部改为三字段。
4. `启动.程序入口.ixx` 删除四个旧 flag 解析、两类构建能力门禁和旧宏分支；未知旧 flag 自然落入既有 `未知参数`，退出码仍为 2。
5. 类型调用点全扫描确认 `启动.程序入口.ixx` 与 `入口.cpp` 只读取 `程序运行结果`，不访问被删字段，故不修改这两个只读调用点中的聚合结构；前者仅因旧 flag 和宏分支进入白名单。

## 5. 明确保留的生产能力

| 文件 / 能力 | 保留裁决 |
| --- | --- |
| `海中鱼巣/核心/仓库快照服务.h` 的“恢复专项”确认 | 正式隔离恢复管理入口，不返回验收 PASS；整文件保留 |
| `海中鱼巣/领域/控制面板服务.h`、`海中鱼巣/领域/显示服务.h` 的“后续专项门禁” | 正式控制面板 / 显示请求分账，不属于自检运行器；除宏包裹计数器外保留 |
| `海中鱼巣/线程/自我治理消息协议.ixx` 的“自检缺口候选” | 正式治理消息类型、来源角色、服务角色和材料类型；不是验收执行入口，整文件保留 |
| SQL 启动审计、性能遥测、D455 生产采集、运行期健康 / 故障隔离、初始化后置条件 | 按公开生产调用继续保留；只删除其验收驱动、夹具、包装和 PASS 聚合 |
| 生产热路径的业务标签、意图组、领域结果见证和执行证据 | 当前仍属 legacy 生产合同，本叶不删；等待中性 CRUD 替代计划保持调用链连续退出 |
| `施工记录/**`、`验证记录/**`、历史集成验收记录 | 原样保留，不参与零残留源码扫描，不升级为新合同证据 |

## 6. 计划索引影响裁决

1. 当前唯一 `可执行` 的 `L1-SIMPLIFY-METHOD-ROOT-R1-CURRENT-READ` 已有代码结果 `be35ff14fa72adad91297225fa010ffc69ce26f7` 和旧验收补修 `743d451094849a50f4418c390a4090ab6a776ca8` 进入 main；旧验收合同已由 `40c110656c401d9bb2cd3d64e2764bb255427d19` 正式退出。索引登记本计划时，R1 必须改为 `已退出`，注明服务源码保留、验收模块 / friend / 聚合登记由本删除计划退出，旧验收结果不能解除新治理门禁。
2. R10 设计提交 `972e491f8671278aaec68d521e31439ab7e835fc` 未登记，且依赖已暂停的 L1 业务意图 / 标签探测语义；不得补登记或恢复。
3. 下列 89 个 `待激活` 计划在索引说明或计划正文中仍绑定旧自检模式、专项顺序、完整自检或共享验收资源。它们保持 `待激活`，且索引新增统一门禁：必须先按 4015 / 8120 / ACCEPTANCE-01 修订为生产二进制外公开入口黑盒验证，才能重新审查激活；不得因删除计划完成而自动激活。

```text
L1-SIMPLIFY-METHOD-ROOT-R2-FIRST-WRITESET-SPEC
L1-SIMPLIFY-METHOD-ROOT-R3-INTENT-GROUP
L1-SIMPLIFY-METHOD-ROOT-R4-REQUEST-INTENT
L1-SIMPLIFY-METHOD-ROOT-R5-IDEMPOTENCY-KEY
L1-SIMPLIFY-METHOD-ROOT-R6-PUBLISHED-INTENT-READ
L1-SIMPLIFY-METHOD-ROOT-R7-CURRENT-ROOT-ADMISSION
L1-SIMPLIFY-METHOD-ROOT-R8-FIRST-PUBLISH-PROVIDER-EVIDENCE
L1-SIMPLIFY-METHOD-ROOT-R9-EXECUTION-EVIDENCE-ASSEMBLY
L1-SIMPLIFY-P18-WORLD-REGISTRY-READ
L1-SIMPLIFY-P19-WORLD-STRUCTURE-READ
L1-SIMPLIFY-P20-EXISTENCE-REGISTRY-READ
L1-SIMPLIFY-P21-EXISTENCE-SET-READ
L1-SIMPLIFY-P22-FEATURE-DEFINITION-REGISTRY-READ
L1-SIMPLIFY-P23-FEATURE-INSTANCE-REGISTRY-LOCATOR
L1-SIMPLIFY-P24-EXISTENCE-FEATURE-SLOT-SET-READ
L1-SIMPLIFY-P25-FEATURE-DEFINITION-SET-READ
L1-SIMPLIFY-P26-EXISTENCE-FEATURE-VALUE-SET-READ
L1-SIMPLIFY-P27-WORLD-STATE-DYNAMIC-SET-READ
L1-SIMPLIFY-P28-COMPLETE-WORLD-SNAPSHOT-COMPOSE
L1-SIMPLIFY-P29-COMPLETE-WORLD-SNAPSHOT-READ
L1-SIMPLIFY-P30-EXISTENCE-COMPLETE-SNAPSHOT-READ
L1-SIMPLIFY-P31-SCENE-COMPLETE-SNAPSHOT-READ
L1-SIMPLIFY-P32-WORLD-BUSINESS-READ-FACADE
L1-SIMPLIFY-P33-CURRENT-SCENE-ADMISSION-SPEC
L1-SIMPLIFY-P34-CURRENT-SCENE-EXISTENCE-CREATE
L1-SIMPLIFY-P35-WORLD-BUSINESS-EXISTENCE-CREATE-FACADE
L1-SIMPLIFY-P36-EXISTENCE-I64-INITIAL-FEATURE-SPEC
L1-SIMPLIFY-P37-L1-INTERNAL-PARTICIPANT-BRIDGE
L1-SIMPLIFY-P38-FEATURE-BATCH-LEDGER-READ
L1-SIMPLIFY-P39-FEATURE-BATCH-LEDGER-PARTICIPANT
L1-SIMPLIFY-P40-EXISTENCE-I64-INITIAL-FEATURE-BATCH-PUBLISH
L1-SIMPLIFY-P41-WORLD-BUSINESS-INITIAL-FEATURE-PUBLISH-FACADE
L1-SIMPLIFY-P42-EXISTENCE-I64-FEATURE-REPLACEMENT-SPEC
L1-SIMPLIFY-P43-EXISTENCE-I64-FEATURE-REPLACEMENT-BATCH-PUBLISH
L1-SIMPLIFY-P44-WORLD-BUSINESS-FEATURE-REPLACEMENT-FACADE
L1-SIMPLIFY-P45-WORLD-BUSINESS-STATE-DYNAMIC-PUBLISH-FACADE
L1-SIMPLIFY-P46-WORLD-MEMBER-MOVE-SPEC
L1-SIMPLIFY-P47-WORLD-MEMBER-MOVE-PUBLISH
L1-SIMPLIFY-P48-WORLD-BUSINESS-MEMBER-MOVE-FACADE
L1-SIMPLIFY-P49-WORLD-EXISTENCE-REMOVE-SPEC
L1-SIMPLIFY-P50-WORLD-EXISTENCE-REMOVE-PUBLISH
L1-SIMPLIFY-P51-WORLD-BUSINESS-EXISTENCE-REMOVE-FACADE
L1-SIMPLIFY-P52-WORLD-BUSINESS-NINE-CAPABILITY-PREACCEPTANCE
L1-SIMPLIFY-P53-SELF-STRUCTURE-REGISTRY-READ
L1-SIMPLIFY-P54-SELF-STRUCTURE-REGISTRY-PARTICIPANT
L1-SIMPLIFY-P55-SELF-STRUCTURE-REGISTRY-PUBLISH
L1-SIMPLIFY-P56-COMPLETE-SELF-STRUCTURE-READ
L1-SIMPLIFY-P57-COMPLETE-SELF-EXISTENCE-SPEC
L1-SIMPLIFY-P58-COMPLETE-SELF-SCENE-SPEC
L1-SIMPLIFY-P59-COMPLETE-SELF-AGGREGATE-SPEC
L1-SIMPLIFY-P60-COMPLETE-SELF-L1-WRITESET-SPEC
L1-SIMPLIFY-P61-COMPLETE-SELF-PUBLISH
L1-SIMPLIFY-P62-COMPLETE-SELF-INITIALIZE
L1-SIMPLIFY-P63-SELF-ROOT-DEFINITION-SPEC
L1-SIMPLIFY-P64-SELF-ROOT-DEFINITION-L1-WRITESET-SPEC
L1-SIMPLIFY-P65-SELF-ROOT-DEFINITION-PUBLISH
L1-SIMPLIFY-P66-SELF-ROOT-INSTANCE-SPEC
L1-SIMPLIFY-P67-SELF-ROOT-INSTANCE-PUBLISH
L1-SIMPLIFY-P68-SELF-ROOT-CURRENT-VALUE-PROJECTION
L1-SIMPLIFY-P69-SELF-ROOT-CURRENT-VALUE-READ
L1-SIMPLIFY-P70-SELF-ROOT-DEMAND-SPEC
L1-SIMPLIFY-P71-SELF-ROOT-DEMAND-L1-WRITESET-SPEC
L1-SIMPLIFY-P72-SELF-ROOT-DEMAND-PUBLISH
L1-SIMPLIFY-P74-I64-EXPLICIT-TARGET-COMPARE
L1-SIMPLIFY-P75-SELF-ROOT-DEMAND-CURRENT-SATISFACTION
L1-SIMPLIFY-P76-DYNAMIC-CAUSAL-CANDIDATE
L1-SIMPLIFY-P77-DYNAMIC-CAUSAL-CANDIDATE-FIRST-WRITESET-SPEC
L1-SIMPLIFY-P78-DYNAMIC-CAUSAL-INTENT-GROUP
L1-SIMPLIFY-P79-DYNAMIC-CAUSAL-CANDIDATE-FIRST-PUBLISH
L1-SIMPLIFY-P80-DYNAMIC-CAUSAL-CANDIDATE-CURRENT-READ
L1-SIMPLIFY-P81-DYNAMIC-CAUSAL-CANDIDATE-APPLICATION-ADMISSION
L1-SIMPLIFY-P82-SELF-ROOT-DEMAND-GAP-QUALIFICATION
SELF-GOV-G05-F0
SELF-GOV-G05-F1
WORLD-TREE-COMPARE-F
WORLD-TREE-FEATURE-HQ
WORLD-TREE-FEATURE-Q
WORLD-TREE-FEATURE-W-DP
WORLD-TREE-FEATURE-W-F
WORLD-TREE-P1B-F
WORLD-TREE-P1B-Q
WORLD-TREE-P1B-W0
WORLD-TREE-P1BF
WORLD-TREE-P1C-F
WORLD-TREE-P1C-FW
WORLD-TREE-P1C-Q
WORLD-TREE-P1C-QF
WORLD-TREE-P1C-SDW
WORLD-TREE-P1C-SW
```

4. 第二笔索引正式发布后，`REMOVE-LEGACY-ACCEPTANCE-CODE v0.2` 直接成为唯一 `可执行` 计划；执行侧必须从新 HEAD 完整重做 S0。其它既有状态除 R1 退出外不改变。

## 7. 实施顺序与失败收口

实施顺序固定：

```text
删除72模块与工程引用
-> 删除启动 import / 包装 / 登记 / 路由
-> 收敛启动与程序结果 DTO、修正全部聚合初始化
-> 删除工程宏、日志切片和21文件宏块
-> 删除8文件未保护探针并恢复纯生产条件
-> 静态零残留
-> Debug / Release 生产构建
-> 三种普通生产模式启动与具名日志人工观察
```

任何阶段出现生产能力无法从正式调用证明、删除条件后改变正式事务判断、聚合初始化遗漏、生产构建失败或普通模式无法启动，立即停止受影响切片并回传具名 DRIFT；不得恢复自检代码绕过失败，也不得顺手修改 legacy 业务合同。

## 8. 验证合同

允许的验证只有：

1. `git diff --check`；
2. `python .\tools\check_specs.py --strict`；
3. `msbuild .\海中鱼巣.sln /m:1 /t:Rebuild /p:Configuration=Debug /p:Platform=x64`；
4. `msbuild .\海中鱼巣.sln /m:1 /t:Rebuild /p:Configuration=Release /p:Platform=x64`；
5. 源码、工程、filters 和编译模块清单的静态零残留扫描；
6. Debug / Release 生产二进制分别以普通控制面板、`--headless`、`--runtime-context` 三种生产模式启动，并通过各模式既有正常关闭 / 停止机制收口；
7. 人只观察具名生产日志是否符合预期，不解析日志形成机器 PASS。

硬性零残留：

```text
名称含“自检”的生产源码文件 = 0
海中鱼巣.vcxproj 与 filters 中自检编译项 = 0
HY_EGO_ENABLE_STRUCTURE_COMMIT_FAULT_SELF_TEST = 0
HY_EGO_DEBUG_LOG_SELF_TEST_ENTRY_INITIALIZATION = 0
HY_EGO_DEBUG_LOG_SELF_TEST_DATABASE = 0
HY_EGO_DEBUG_LOG_SELF_TEST_WAREHOUSE_PERFORMANCE = 0
HY_EGO_DEBUG_LOG_SELF_TEST_D455 = 0
四个旧验收 flag = 0
自检运行器 / 登记自检 / 程序专项摘要 / 程序专项类别 / 程序运行结果::专项 = 0
本设计具名的私有探针、故障注入、计数器和访问器 = 0
```

历史记录、正式规范和本设计材料不参加生产源码零残留计数。

## 9. 完成声明边界

计划完成最多证明：旧生产内验收体系已物理退出，生产程序仍可构建并以三个正式生产模式启动。它不证明新中性 CRUD 已实现、公开 CRUD 黑盒已通过、P15 / R1—R10 已重新设计、生产服务已正式接线或崩溃 / 跨进程恢复成立。
