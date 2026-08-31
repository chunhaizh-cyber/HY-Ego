# ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION L2 特征/状态模块实体所有权 ABI 纠正代码实施计划 v0.2

日期：2026-08-31

状态：执行期具名漂移修订候选；详细设计/计划发布并由计划索引登记精确 v0.2 blob 后，执行智能体才可重新 S0。

计划身份：`ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION`

恢复基线：`main == origin/main == 9d329d5d4e79747a337b5ea99d335825c3c1449f`

## 1. 来源问题、目标与完成条件

v0.1 执行已在两份生产源码和两份记录形成未暂存安全 WIP。v145 Debug fresh Rebuild 证明 `L2状态结构服务` 模块身份错误归零，随后特征模块对 `L2状态动态原子发布服务` 的同形前置声明/friend 造成第二所有权，最终 `LNK1120: 10`。

本修订在原目标内：删除动态发布服务错误声明/friend，把其实际消费的 4 个特征 owner 结果类型和 5 个既有 helper 改为精确公开窄合同，保留 v0.1 状态/首值修改，完成 v145/v143 Debug/Release x64 fresh Rebuild 和最终链接。

详细完成条件、零语义变化和声明边界以同身份详细设计 v0.2 第 1、4、7、8 节为准。计划发布、对象编译或首个 LNK 消失都不等于完成。

## 2. 正式依据与冻结事实

- 详细设计：`规范/详细设计/20260831_ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION_L2特征状态模块实体所有权ABI纠正详细设计_v0.2.md`；精确 blob 由第一阶段发布后登记；
- CODE-FILE-01 的命名模块实体单一所有权、函数物理位置和知识复核规则；
- v0.1 计划 blob `afbf62222b822f483d8f5dd9e0456588b16f034d`、详细设计 blob `430405acfee96e5048c7d17b2ebe9946a598b804`；
- 执行记录已固定普通应用 getter 的错误特征模块附着和动态发布定义的真实模块附着；
- 特征 WIP blob `513028fab29c14d89ed15546e7d0207e0dc530a7`，状态 WIP blob `9ec9a496d0ec9d036c76f8495c87dd93fb18a6de`；
- 动态发布源码工作树与 HEAD blob 均为 `96c77acfe3bbf1a550a7e4208abcbac1585535c1`，实际没有需要改写的私有端口访问；
- Git index 空，异主 WIP 原地保留。

## 3. 完整写白名单、保留现场与禁止项

执行恢复后允许修改的完整白名单仍为：

```text
海中鱼巣/领域/服务.L2特征结构.ixx
海中鱼巣/领域/服务.L2状态结构.ixx
施工记录/20260831_ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION_施工记录_v0.1.md
验证记录/20260831_ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION_验证记录_v0.1.md
```

其中状态源码只保留并验证既有 WIP，不授权新的语义修改；两份记录追加 v0.2 证据，不删除 v0.1 BLOCKED 历史。

`海中鱼巣/领域/服务.L2状态动态原子发布.ixx` 是必须 fresh 重编译和 dumpbin 核对的验证输入，但不是写白名单：当前 6 类特征调用已具备正确名称、参数和依赖方向，只有被调用合同的可见性错误。禁止为了形式对该文件制造注释、重排、wrapper 或无依据调用变化。

禁止修改：Gseed 源码/测试/记录、动态发布源码、装配、工程/filters、L1、数据头、规范、v0.1/v0.2 设计与计划、索引、知识库、流程图、其它测试和其它 WIP。禁止第二类声明、反向 import、typedef、文本 include、假定义、链接器规避或把整个 private 区公开。

若恢复 S0 发现四份白名单出现来源不明的新变更、index 被占用、HEAD/正式索引已前进且与本合同重叠，停止本计划并协调所有权。

## 4. 冻结合同与机械实施步骤

详细设计 v0.2 第 3—5 节是唯一接口合同。机械步骤：

1. 保留 v0.1 已落盘的状态服务声明/friend 删除、首值 1 类型/2 helper 公开窄合同及状态模块端口 getter 改接；
2. 在特征模块删除 namespace 顶层 `class L2状态动态原子发布服务;`；
3. 删除 `friend class L2状态动态原子发布服务;`；
4. 只把以下 4 个既有嵌套结果类型改为 `public`：`当前值变化三分区参与者结果_v1`、`当前值变化账登记内部结果_v2`、`当前值变化三分区参与者结果_v2`、`当前值变化账扫描内部结果_v2`；
5. 只把以下 5 个既有 helper 改为 `public`：`形成当前值变化三分区参与者_v1`、`确保当前值变化账登记_v2`、`读取当前值变化账登记_v2`、`形成当前值变化三分区参与者_v2`、`扫描全部当前值变化账_v2`；
6. 函数定义原地保留，签名、函数体、字段、默认值、常量、本地键、错误映射和调用参数零修改；公开窄区结束后确保其它 helper、锁、交付、端口和字段仍为 private；
7. 不改动态发布源码；fresh Rebuild 自动重新形成动态发布与普通应用对象；
8. 在原两份记录追加 v0.2 计划/blob、S0、四配置、dumpbin、剩余 LNK 分账和函数级知识变更清单。

函数级知识变更清单至少记录：4 类型/5 helper 仅可见性改变、两个错误声明/friend 删除、动态发布调用边与 v0.1 首值/状态调用边零变化；执行者不直接修改共享知识库。

## 5. 失败收口

- 需要修改任一目标函数签名、函数体、字段、DTO、owner、事务、端口或动态发布调用参数：真实新漂移，立即停止并具名退回；
- 无法只公开 4 类型/5 helper：停止，不扩大整个 private 区；
- v145/v143 任一工具链缺失：`BLOCKED`，不得降级替代；
- 目标两类身份闭合后出现其它模块实体 LNK：固定首个符号、消费/定义 obj 和模块附着身份，另行分账；
- 发布租约、index、目标所有权或验证根冲突：保留现场，只停止本计划。

## 6. 验证命令与成功标准

先枚举 `*.sln`、`*.slnx`、`*.vcxproj`；真实根保持 `海中鱼巣.vcxproj`。使用新的仓外唯一根：

```text
D:\TEMP\海中鱼巣\ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION\<new-run>\<toolset-config>
```

串行执行：

```powershell
msbuild .\海中鱼巣.vcxproj /t:Rebuild /m:1 /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v145 /p:IntDir=<obj> /p:OutDir=<out>
msbuild .\海中鱼巣.vcxproj /t:Rebuild /m:1 /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v145 /p:IntDir=<obj> /p:OutDir=<out>
msbuild .\海中鱼巣.vcxproj /t:Rebuild /m:1 /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v143 /p:IntDir=<obj> /p:OutDir=<out>
msbuild .\海中鱼巣.vcxproj /t:Rebuild /m:1 /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v143 /p:IntDir=<obj> /p:OutDir=<out>
```

并完成：

```text
特征文件中状态/动态发布两个 class/friend class 零命中
公开面只有 v0.1 首值 1类型/2 helper 加本次 4类型/5 helper
动态发布源码直接访问特征私有字段/端口零命中，6类调用原样
dumpbin 证明状态类只附着状态模块、动态发布类只附着动态发布模块
模块依赖保持 状态->特征、动态发布->特征，零反向边
git diff --check -- <四份白名单>
git diff --check
git diff --cached --check
python .\tools\check_specs.py --strict
```

成功要求详细设计 v0.2 V01—V10 全部 PASS，四配置 Rebuild 最终链接退出码均为 0。只做 `/t:ClCompile`、只跑 v145 Debug 或只证明 10 个旧 LNK 消失均不能完成。

## 7. 重新 S0、发布与完成声明

本设计/计划由计划支撑第一阶段发布后，计划索引所有者必须把同一计划登记为 v0.2、精确 plan/design blob 和 `可执行`，执行者随后从正式仓库重新 S0；不能继续沿用 v0.1 聊天许可。

代码结果发布时只精确暂存第 3 节四份白名单，逐 hunk 审计 cached diff；普通提交并推送后复核 `HEAD == origin/main`、ahead/behind `0/0`、index 空，并报告全部剩余异主 WIP。

本计划最多声明两个服务类的模块实体所有权和对应 ABI 链接阻断闭合。完成后通知交互智能体恢复 Gseed 原计划专项运行验证；不得直接把 Gseed 标为完成，不得声明阶段三或本能路线完成。
