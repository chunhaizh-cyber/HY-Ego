# 概念前置与世界添加代码实施计划

日期：2026-09-13

版本：v0.1

计划身份：`WORLD-TREE-CONCEPT-FIRST-A`。

当前状态只由[计划索引](计划索引.md)的精确版本与 blob 决定。本文设计基线为 `b0a27c4858c739a0c21273245aab5b00faac3033`；正式登记后执行者须针对当时 HEAD 重新 S0，不将当前旧 P1 WIP 纳入基线。

## 1. 目标与结果

落实[世界树总体方案](规划/20260913_WORLD-TREE-COMPLETE_世界树信息操作与概念自动完善方案_v0.1.md)批次 A：纯概念定义先独立发布；世界树再经 DATA-L2 原子建立 E、合法初始挂靠、对已发布 EC 的引用及可选单例角色。真实自我形成服务在普通启动中消费这条路径，形成同节点 E=C 的唯一自我，完整读回后如实到达尚未实现的方法登记根阶段19。

用户已明确先采用具名规则定义的通用存在概念，不预设特征，随后依真实信息细化；因此本计划不依赖尚不存在的自我特征模板。通用 EC 与普通漏填模板分支严格分离，概念不承担唯一自我角色。

本计划把概念提供者、出生引用、场景/存在公开添加、世界编排、唯一自我、普通装配和直接读回作为**一个**代码切片，不能拆成DTO、空provider或getter分别宣布完成。

## 2. 正式依据与冻结合同

机器语义依据：0050、1140、1150、2220、4015、4200、4202、4203、4204、4230、4240、7130、7140、7150、7160、8120，以及用户本轮具名通用EC裁决所同步的正式条款。

全部精确函数、类型、字段来源、物理映射、依赖方向、原子参与者次序、原请求恢复、失败结果和验收条件冻结在[详细设计 v0.1](../规范/详细设计/20260913_WORLD-TREE-CONCEPT-FIRST-A_概念前置与世界添加详细设计_v0.1.md)。本文不再复制其 DTO 与流程。

当前事实已经核对：L1 v3 多 owner 原子能力真实存在；绑定三分支、同节点场景角色、联合父链与正式 I64 域能力可复用。当前概念related v1不能独立创建纯定义，旧世界创建无概念输入，普通启动尚无真实SELF-FORM；这些都须由本计划实现，不能作为上游完成项。

## 3. 允许与禁止范围

允许生产文件：

1. `海中鱼巣/领域/合同.相关概念添加参与.ixx`
2. `海中鱼巣/领域/数据服务.概念树类.ixx`
3. `海中鱼巣/领域/数据服务.绑定存在.ixx`
4. `海中鱼巣/领域/数据服务.存在类.ixx`
5. `海中鱼巣/领域/数据服务.场景类.ixx`
6. `海中鱼巣/业务/应用服务.世界树类.ixx`
7. `海中鱼巣/业务/应用服务.自我形成.ixx`（新建唯一业务提供者）
8. `海中鱼巣/装配.普通应用.ixx`
9. `海中鱼巣/装配.普通应用.cppcpp`
10. `海中鱼巣/启动.应用程序.ixx`
11. `海中鱼巣.vcxproj`
12. `海中鱼巣.vcxproj.filters`

专属记录：

- `施工记录/20260913_WORLD-TREE-CONCEPT-FIRST-A_概念前置与世界添加施工记录_v0.1.md`
- `验证记录/20260913_WORLD-TREE-CONCEPT-FIRST-A_概念前置与世界添加验证记录_v0.1.md`

代码执行不修改本设计、正式规范、计划或共享知识索引；真实设计漂移按规则具名退回。L1源码、特征准确内容/定位事务、状态/动态/因果模块、语言名称、方法/本能根、自我线程运行、控制面板布局及其它所有者记录不在本计划范围。不新增持久进度树、新owner或第二世界/概念根。旧库物理迁移与删除不在本计划；零数据新上下文完成，旧域明确拒绝新写并保留原身份和历史，不宣称旧库迁移完成。

旧P1 WIP由其原所有者交接：逐项保存差异、确认纯挂靠和读回可复用片段；共同出生、待发布概念/存在方案和旧测试成功语义不进入新实现。不覆盖、不删除、不提交来源不明的 WIP。所有允许文件在登记和交接完成后才可写；整计划只占一个代码切片，工程和验证资源串行。

## 4. 实施顺序

1. 完成S0、旧WIP交接和知识基线回读；记录精确HEAD/来源blob、允许文件与验证根。新写在同一概念owner新格式上成立，不把旧布局改版本冒充迁移。
2. 实现纯概念 v2：元登记与构造、通用EC显式规则、正式I64 FC与非空EC、精确查询/创建/读回、生命周期及无引用叶退出。普通查询零写，旧入口不得进入新格式写入。
3. 实现概念owner的 E→EC 使用读取与private出生引用参与；扩展存在owner单例角色登记、读取和出生片段；保持只有现存EC进入世界出生事务。
4. 在绑定协调器闭合三分支和原键首次材料恢复，场景/存在公开入口选择本类合法分支。原无概念公开创建及旧P1共同出生入口退出新写面。
5. 迁移世界创建v4，完成定位→概念→刷新G→L2添加→共同截止读回及真实阶段结果；修复工厂先全局发布后分配的生命周期窗口，发布之后保有同一底座。
6. 新增真实自我形成服务，普通上下文装配同一概念/角色/世界/self并调用；先读唯一角色，创建同节点场景或按原请求收敛，完整读回。self未完成/部分发布时上下文保留；后续不能重造底座。
7. 正常入口按实际装配、概念就绪、SELF-FORM和后继门禁返回；self成功后到阶段19，不运行空方法provider、测试循环或线程。
8. 运行第5节验证，记录真实失败与修复；施工记录列明新增/改变/退出函数用途和影响知识条目。达到本范围条件后精确发布，通知交互智能体沿总体方案选择下一最早缺口。

## 5. 验证合同

任务资源为 `D:/TEMP/海中鱼巣/WORLD-TREE-CONCEPT-FIRST-A/<本次唯一标识>/`，禁止复用旧任务固定输出目录。验证工具和测试源码在该任务目录中建立，完成后记录源文本、SHA-256、编译输入和命令；生产项目不登记验收驱动。若需永久入库的新验证工具，须先具名声明精确文件扩展，由切片所有者按当前规则确认后实施，不能默默复制旧专项工程。

已核实MSBuild路径：`C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe`，v18.4；目标v145/C++20。使用该路径或`vswhere`在同主机解析的同版本安装，不能写旧Visual Studio路径假定可用。

执行命令形态（将唯一验证根写入记录，命令必须含实际绝对路径）：

```powershell
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe' '<验证根>/world_tree_concept_first_probe.vcxproj' /t:Rebuild /m:1 /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v145 /p:IntDir='<验证根>/obj/debug/' /p:OutDir='<验证根>/bin/debug/'
& '<验证根>/bin/debug/world_tree_concept_first_probe.exe'
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe' '<验证根>/world_tree_concept_first_probe.vcxproj' /t:Rebuild /m:1 /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v145 /p:IntDir='<验证根>/obj/release/' /p:OutDir='<验证根>/bin/release/'
& '<验证根>/bin/release/world_tree_concept_first_probe.exe'
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe' './海中鱼巣.vcxproj' /t:Rebuild /m:1 /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v145 /p:IntDir='<验证根>/main/obj/debug/' /p:OutDir='<验证根>/main/bin/debug/'
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe' './海中鱼巣.vcxproj' /t:Rebuild /m:1 /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v145 /p:IntDir='<验证根>/main/obj/release/' /p:OutDir='<验证根>/main/bin/release/'
git diff --check
python ./tools/check_specs.py --strict
```

探针复用根工程的实际生产编译项，排除入口并增加生产外驱动；不复制实现到夹具。若本机已有无法隔离的自动输出，必须把源输入快照和完整依赖放在同一验证根，记录各输入blob/hash；不得在结果中混入异主WIP。

详细设计第8节八组为必测，本批Debug/Release都须运行。代码构建不是业务验收：探针须从正常 `运行普通程序`/`初始化普通应用自我` 及公开数据服务进入，并通过 `读取普通应用自我`和各owner公开读回证明实际终态。普通二进制退出1且阶段19是后继未实现的真实输出；只有同时证明self角色/同节点/根/概念闭包，才能称A完成。

同时扫描生产调用图，证明旧世界v3无概念创建、相关概念共同出生、旧静态根和旧自我双身份入口不在新普通初始化链。它不证明所有旧格式代码已经物理删除。

跨进程恢复、旧域迁移、多进程、磁盘故障和完整世界树/自我线程运行明确NOT_RUN或范围外；真实L1发布未知若只能边界注入，单列注入位置与未覆盖层，不能提升为真实故障覆盖。未运行任何必测用例不能把计划标为代码完成。

## 6. 完成声明

本计划完成必须同时满足：纯概念独立发布；三个绑定分支的E/父/现存EC引用同原子；单例角色真实唯一；普通自我形成使用同节点、同根、通用EC且独立读回；概念与世界失败/重试无重复或悬空；工厂/上下文失败不留悬空底座；旧绕行入口退出当前新写；必测验证与主工程实际通过，专属记录完整并发布。

代码执行完成后不得停止总体用户目标。交互智能体据结果继续总体方案B及其它未实现项；本计划不提前建立多层待激活库存。正式集成验收只消费本计划已提交推送的精确结果，不用本地探针替代其独立结论。
