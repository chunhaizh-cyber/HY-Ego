# CURRENT-SNAPSHOT-A3-RUNTIME 外设消息与确认结构持久自键及独立读回 v0.3

本版以 `c59f895db087280fda66c991ccd75a89ca0c7f9c` 为施工源码基线，由主工作区交互窗口独占代码实施与正式工程验证。施工以历史追溯章节之前的范围为准。原 A3 依赖链与 v0.2 外设事务保留供追溯。`7001a798c` 已回退 `ca50a167f` 的宿主与候选事务接线，本版按现行接口重新施工。

依据 [4015 §14–15](../规范/4015_子规范_基础数据集.md)、[4070 §4–5](../规范/4070_子规范_权威结构快照恢复候选与运行期原子发布.md)、[6120 §11.1](../规范/6120_子规范_自我存在服务与生存基本因果闭环.md)、[7241 §1–2](../规范/7241_子规范_句子转换与交互场景同步.md) 和[当前事实启动恢复详细设计](../规范/详细设计/20261128_CURRENT-SNAPSHOT-STARTUP_当前事实持久启动详细设计_v0.1.md)。既有恢复入口能在同一视图核验全局根、特征值、特征概念、服务登记所有者结构和自我单例。外设的 `消息字段`、`交互确认字段` 与 `消息根` 仍只持进程内注册，不能从旧受控快照推得唯一身份。先交付这个独立的外设所有者提供者。援引已回退快照的入站故障仅用于定位两处消息根解析和回归边界。

## 接口与结构

外设所有者在 `海中鱼巣/业务/新_外设世界树类.h` 公开声明下列静态函数，在同模块 `.cpp` 实现。持久视图类型前置声明并在实现使用现有底座接口。不加工程项。启动层是本版唯一调用者。

```cpp
static 基础结构操作状态 发表已初始化外设消息持久自键();
static 基础结构操作状态 核验外设消息持久自键(
    const 基础持久视图& 视图, bool 与当前初始化登记核同);
```

- 在外设所有者私有实现按现行 `消息字段[0..16]`、`交互确认字段[0..5]`、`消息根` 的顺序固定 24 项。标记分别为 `HY-Ego/7241/peripheral-message-{0..16}/v1`、`HY-Ego/6120/peripheral-confirm-{0..5}/v1` 和 `HY-Ego/7241/peripheral-message-root/v1`。序号使用十进制且无前导零，花括号表示取值范围。每条字段所属节点和字段键均等于该结构节点，内容为 `基础值` 中直接保存的精确字符串。所有代码依此表取序，字段角色仍由现有外设所有者定义。
- `发表已初始化外设消息持久自键` 只在持久空仓已装配且未启动外部消费者时调用。在现有外设互斥下核对外设根、值服务已初始化，以及 24 个现有注册编码均有效且互异，随后调用 `发表基础结构自键(全局基础数据集, 键组)` 一次组合发表。已有同宿主、同键、同值时正常复用。初始化不全、缺失、冲突或资源失败传回实际结构状态，禁止分配替代键或补造外设事实。
- `核验外设消息持久自键` 只从传入的同一不可变视图读取。调用 `读取基础结构自键`，严格检查 24 项完整存在、标记各自唯一、宿主、字段键、直接字符串及项间编码互异。按读回顺序用该视图的 `结构()` 读消息根并核验下述根字段规则，要求恰一枚自键。参数为 `true` 时在外设互斥下按位核同当前已初始化登记。参数为 `false` 时不读取当前登记，适用于独立暖恢复校验。函数不写回静态注册、不初始化外设、场景或特征服务，也不从全局数据当前视图补字段。缺节点或标记保留 `节点不存在`，结构错误、重复或登记不相等返回 `内部不一致`，分配失败返回 `资源失败`。
- 在外设 `.cpp` 私有实现共用消息根字段核验，先核快照的根编码与成员键有效且互异。允许已核验的精确根自键和 `消息字段[10]` 所指向的非零成员节点引用，拒绝其它键、直接值成员、重复自键或重复成员。冷建初始化未设置持久库的普通入口不调用自键发表。全根业务扫描接受零或一枚精确自键，持久视图核验要求一枚。`按来源查找消息` 和 `新_外设世界树类::提交输出` 的正式出站高水位扫描都先核验再只遍历成员，保留现有成员正式解码、同源唯一性、序号及完整根快照复核。直接按节点读取保持原成员持有校验。类型表、消息记录字段和既有来源合同保持原口径。

## 启动流程与错误边界

1. `启动.应用程序.cpp::引导持久空仓` 在 `构造普通应用上下文` 成功后、`准备首份` 前调用发表。在原首份候选及其它原有恢复校验通过后、`提交首份` 前，调用 `核验外设消息持久自键(候选->读取(), true)`。任一外设步骤失败即终止首份发布。沿已有 `持久库准入失败`，资源不足映射 `资源失败`，缺失或冲突映射 `结构冲突`。退出由现有普通上下文资源回收，禁止创建第二份快照或改用默认空库。`提交首份` 按现有状态回执和未知结果路径退出，发布结果未知不重试、不打开宿主。
2. `检查普通启动持久准入` 的已有快照分支先完成现有五组与自我单例复核，再在 `绑定恢复权威` 之前调用 `核验外设消息持久自键(*视图, false)`。失败同样使用 `持久库准入失败`；缺标记的旧受控快照保持原字节并拒绝继续，不做迁移。成功继续原有只读权威绑定和 `持久业务运行者未重绑` 出口，不构造业务 owner 或启动消费者。复用现有结果字段和故障收口，不新增日志调用、失败枚举、启动模式或调试路由。

后续仍需冻结实际外设根、场景与类型／值运行重绑及服务登记零写准备与业务组合。本版的外设自键不能证明真实开窗、入站、普通服务对象登记、权限分配、身份采用唤醒或 D/F/S/T 路径。`ca50a167f` 的线程私有候选、全局数据集入口、冷宿主开放、回调及普通装配写入口均超出本版。

## 施工与验证

生产允许写 `海中鱼巣/业务/新_外设世界树类.h`、`海中鱼巣/业务/新_外设世界树类.cpp` 和 `海中鱼巣/启动.应用程序.cpp`。证据只记 `验证记录/20261128_CURRENT-SNAPSHOT-A3-RUNTIME_外设消息确认自键与独立读回.md`。其余生产与共享目录归原所有者。

- 在主工作区实际构建 `MSBuild.exe 海中鱼巣.vcxproj /t:Build /p:Configuration=Release /p:Platform=x64 /m`，按默认 `D:/海中鱼巣/x64/Release` 产物收证，不重定向工程目录。静态回读全部 24 项、自键无效/重复、两处正式扫描、冷首份校验时序和独立暖入口。
- 若用户对当前提交明确授权普通默认入口回归，只在主工作区运行现有普通入口并记录启动、正常消息链及关闭的实际结果。当前授权未覆盖创建新的持久数据目录或运行持久空仓、拒绝样本和独立重启。此类验证记录 `NOT_RUN` 并保留无运行证据边界。取得具名授权后才可用受控新快照的真实普通入口验证首次 24 项与原注册一致、独立重启同码、缺键快照无写且拒绝、清单和活动槽无变化，以及普通业务写门保持关闭。不得把预期拒绝退出当成恢复运行成功。

## 历史 A3 v0.1（追溯）

基线 `5c23ac4a1`，本窗口唯一所有者。依据 4015 §15、4070 §4–5、6120 §11、6220、7240/7241、5170、5380。用户授权普通链实际施工和验证，真实库、第 1 组、旧格式迁移、隔离模式放宽均不属于此计划。

## 实际写入链与禁行边界

| 实际入口 | 现状与必须保留的共同发表边界 |
| --- | --- |
| `新_外设世界树类::建立或取得对话框场景/明确确认本次网页交互`，`新_场景类::建立或取得临时交互场景` | 外设场景、来源确认、本窗副本与双方角色。临时场景已有多步裸候选和状态留存，普通运行不得允许旧写端分次公开。 |
| `新_外设世界树类::记录消息`，`新_世界树类::准备原消息场景角色事实` | 正文外设消息、接收或发送集合、历史角色及现场回执共同发布。消息身份保留；跨窗口不认错。提交成功才通知下游或传输。 |
| 语言转换的解释、采用与 `关联源窗身份自述定义`，`新_世界树类::采用本窗身份认知` | 语言独立事实、外设关联、内部正式认知分属各 owner。当前／历史采用联合写，首次事务不冒充上游消息入账。 |
| `新_世界树类::登记活动数据集服务对象实现`，`新_特征类::提交顶层存在引用更新` | 先重新核本窗已知 E 的人类归属。首次目前先保存 U64 标准值，部分准备还会预建值／概念；须收进联合持久候选或受保护复用，正式权益、当前引用、状态和动作保持同一提交。不能把提前的内存写改成独立落盘。重投按原认知的动作身份，不按当前对象。 |
| `接收需求认知` 与需求 owner 场景初次/后答、正式 O/Send、回复关系及首次内部采用 | 保留外设、语言和内部需求各自身份；发送与认知先后按当前合同。这些仍有旧内存调用，相关 owner 未绑定之前持续关闭网页写门。 |

## 实施与验证

1. 给普通进程的现有基础数据集接线入口提供单一会话所有权。恢复候选先在原隔离视图完成旧门复核；仅空、从未发表过事实的进程入口可绑定已读持久会话，重复同会话幂等，不同会话、活动内存库或空会话拒绝。原实例只代理来自同一个会话的当前不可变快照，各公开查询在入口固定一次读视图；旧直接创建、修改、删除、比较变更和内存组合全部在写锁中拒绝。封存无写，按同一会话身份封存；禁止从旧内存片段切入。会话保持到进程静态出口，旧实例不得用在其背后维护镜像。未绑定实例保留原行为。
2. 先在真实普通重启入口经代理读回正式自我、父场景及所有者结构，确认与持久视图同源；使用现有受控新仓验证旧资料拒绝、独立重启、旧写门始终拒绝、原快照字节不动。进程作用域可通过既有具名持久自检对合法旧写入口拒绝、正式封存提交后实时可见、旧视图／错会话拒绝作验证。禁止用正常「启动退出 1」宣称已开放。允许在这个依赖链内接窗内正式采用与封存发布，但必须先具备真实窗、消息和所有者绑定，释放业务锁后使用捕获的同视图预期提交，全量互证旧快照并验证明确失败与未知终止。没有合格角色／来源时不造材料。
3. 后继在同一实施包优先改造业务释放锁交接，实际权限 V 的准备须零写，正常提交持久通知后才发送。逐段使用实际确认者和真实来源，能从普通入口经第 6 组原问和正式答、独立反读、重启同源重投及自然退出才计通过。第 1 步绝不开放普通写门。需要大范围恢复装配或事务提供者而本切片未完成时，保留真实代码和阶段证据，明确尚未实施或验收的 owner。

允许 `核心/基础数据集.{h,cpp}`、`核心/基础数据集.持久运行包.{h,cpp}`、原普通启动入口与状态、`业务/新_世界树类.{h,cpp}`、证明本包同权威的既有自检实现及正式验证记录。后继如代码落点超出现有文件须先收口设计并更新登记，不能用总目标授权跳过门。Release x64 单组编译；只用仓库外既有受控快照并以普通实际入口观察准入。

## 历史 v0.2 外设持久事务切片（已回退，供追溯）

Starting `94992dab2`. Same existing ordinary context only, no new mode/endpoint. 4015 §15 / 4070 §5 / 6120 §11.1 / 7241. No permission or new semantics.

Persist formal peripheral message/confirmation schema and message index root in their owner, use exact self-key recover, do not invent historical windows. Initially preserve closed warm host until scene/value/FT owners can be reattached; retained read verification uses original snapshot and does not allocate replacements. Cold host lives only after actual first snapshot matches stopped assembly completely, and relinquishes staging data under the same empty/transfer admission before supplying ordinary input. The same ordinary host may expose only the external window, explicit source confirmation and real receive once their transaction is complete. Service actions, send, complete ordinary startup and governance remain closed until covered. No test route.

Each external operation executes existing owner logic on a privately cloned current view, only on its calling thread, before external notification or receipt. Only the registered data entry routes to that private candidate during the scope. All other callers see published immutable state. Validate source, no-change, full ordinary owner readback before commit; drop candidate on rejection, retain current. Submit lock-free of owner locks through existing durable session, no receipt/notification before publication, stop on unknown. Session sequential; do not run unconverted control panel/governance writers. Read owner indices are derived and must verify candidate against actual persisted material. No use this scope for unrelated owners.

Scope core data set/runtime pack, external world .h/.cpp, ordinary startup/assembly and web adapter/host capability validation, plus exact project evidence. Run actual Release x64 with new isolated stored root. Prefer actual visible Chrome new window plus confirmed source and receive, then independent stored graph readback and restart. If partial, report exact operations and statuses; never label business or ordinary full success on a bootstrap check.
