# ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION 代码实施计划 v0.4

日期：2026-08-31

状态：执行期具名漂移修订候选；详细设计/计划发布并由计划索引登记精确 v0.4 blob 后才可重新 S0。

恢复基线：`main == origin/main == c83aa39b38200dd992290863b7a24daf00d813d0`。v0.3 未暂存安全 WIP 原样保留，不撤销、不覆盖、不夹带。

## 1. 来源问题、目标与完成条件

v0.3 v145 Debug fresh Rebuild 已把旧 6 个 DTO UNDEF 归零，当前 `LNK1120: 2` 只剩普通应用 mutable/const getter。定义返回类错误附着 `海中鱼巣.领域.服务.L2动态结构`，消费者要求 `海中鱼巣.领域.服务.L2状态动态原子发布`；根因是动态结构模块仅剩的一对同名 forward/friend。

本计划在同一 ABI 纠正目标内删除该对错误声明，以动态 owner 的 1 个既有结果类型和 3 个最小公开窄入口替代全部 friend 私有访问，保留 v0.1—v0.3 所有安全 WIP和证据，并完成 v145/v143 Debug/Release x64 fresh Rebuild 最终链接。

唯一详细设计：`规范/详细设计/20260831_ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION_L2特征状态模块实体所有权ABI纠正详细设计_v0.4.md`；精确 blob 由第一阶段发布后登记。

## 2. 完整白名单与禁止项

允许修改：

```text
海中鱼巣/领域/服务.L2动态结构.ixx
海中鱼巣/领域/服务.L2状态动态原子发布.ixx
施工记录/20260831_ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION_施工记录_v0.1.md
验证记录/20260831_ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION_验证记录_v0.1.md
```

动态结构当前 clean blob 必须为 `563a37ef7ec9da856e21786111514303d4c2a138`；动态发布、特征、状态和两记录必须保持详细设计 v0.4 第 3 节冻结的 WIP 起点。禁止修改 DTO 数据头、装配、消费者、工程/filters、Gseed、规范、旧设计/计划、索引、知识库、流程图或其它 WIP；禁止把其它现场带入提交。

明确排除：不得复用、改造或适配 `海中鱼巣/领域/L2动态结构.数据.h:418` 的 `L2动态原子参与者形成结果` 替代本计划的既有 `当前值记录动态参与者结果_v1`。前者使用 `L1跨所有者原子参与者写集`，后者要求 `L1三分区原子参与者写集_v2`，且前者没有 `状态迁移动能本地键` 与可选 `动作致变动态本地键`；两者语义和事务形状不等价。

## 3. 机械施工步骤

1. 重新 S0：核对 main、HEAD/origin、index、v0.3 plan/design blob、详细设计冻结的五份 WIP blob、动态结构 clean blob、正式索引和目标所有权。
2. 动态结构删除 namespace 顶层 `class L2状态动态原子发布服务;` 与类内 `friend class L2状态动态原子发布服务;`。
3. 只把既有 `当前值记录动态参与者结果_v1` 变为 public；字段、默认值和顺序不变。
4. 新增 `组合发布当前值动态材料就绪_v1() const noexcept`、详细设计冻结的 10 参数 `形成组合发布当前值记录动态参与者_v1(...) const noexcept` 与 const `取得组合发布写入端口()`。10 个显式参数的数量、顺序、传值 / 引用形态和 const 约束均不可改变，尤其保留 `const std::optional<L2方法身份>&` 与两个 `const L1三分区原子事实引用值_v2&`；owner 身份和上下文关系类型只能由 wrapper 内部注入。
5. 就绪 helper 只检查两个定位；动态发布必须先调用并把 false 保持为 v1 `材料不足`。形成 wrapper 内部注入上下文关系类型和 owner 身份，且自身在定位缺失时返回空载荷 `L2结构状态::未找到`，禁止解引用或返回 `入口拒绝`。现有私有 provider 正文原地不动，现有非 const 端口 getter保留。
6. 动态发布只机械改接：两个定位检查 -> 就绪 helper；私有 provider 调用及两个内部参数 -> 形成 wrapper；`第一层写入端口_` 首次完整读回 -> const getter。公开读回、DTO export、事务和其它正文不动。
7. 追加两份专属记录：v0.4 plan/design blob、S0、精确 diff、代码知识变更清单、四配置、dumpbin、旧 8 唯一符号归零和后继 LNK 分账。

需要修改签名、结果字段、私有 helper 正文、参数来源、owner、事务、锁、错误映射或白名单外文件时，立即按新设计漂移停止。

## 4. 验证、失败收口与发布

构建使用新的仓外唯一根：

```text
D:\TEMP\海中鱼巣\ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION\<new-run>\<toolset-config>
```

枚举 `*.sln`、`*.slnx`、`*.vcxproj` 后，以真实根 `海中鱼巣.vcxproj` 串行执行 v145/v143 × Debug/Release x64 `/t:Rebuild /m:1`，四项都须最终链接退出码 0。另须完成：

```text
全仓动态发布类错误 class/friend 归零且无第四模块
动态发布对动态 owner 四类 private 成员零直接访问
公开面精确为既有 1 结果类型 + 3 窄入口
缺定位分支仍映射材料不足
两个 getter 仅返回动态发布模块附着类
六个 DTO 方法继续使用无附着 DTO
dumpbin 逐项核对旧 8 个唯一符号
git diff --check -- <四份白名单>
git diff --check
git diff --cached --check
python .\tools\check_specs.py --strict
```

四配置任一工具链不可用记 `BLOCKED`，不得用另一配置替代。旧 2 getter 和 6 DTO 身份归零后出现新 LNK：仍属于本次类/窄合同复链的，在白名单内修复；其它符号固定首因、对象和附着后停止，不扩大。

全部通过后只精确暂存第 2 节四份白名单，普通提交并推送；复核 `HEAD == origin/main`、ahead/behind `0/0`、index 空并报告全部剩余异主 WIP。本计划最多证明模块实体/DTO ABI 链接闭合；发布后通知交互智能体恢复 Gseed 原计划验证，不得直接声明 Gseed、阶段三或本能路线完成。
