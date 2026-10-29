# 需求五目标字段真实特征承载实施计划

日期：2026-10-29

版本：v0.1

标识：DEMAND-TARGET-FEATURE-SKELETON

状态及精确blob只由 [计划索引](计划索引.md) 登记，完整合同见 [详细设计](../规范/详细设计/20261029_DEMAND-TARGET-FEATURE-SKELETON_需求五目标字段真实特征承载详细设计_v0.1.md)。

## 1. 结果和边界

把D本体目标FT/形式/范围/比较/方向五字段改为各持真正F，F是值唯一存处。五F始终存在，未知/不适用可空；复用同D/同F，全部D两域创建、legacy迁移、被认知完善及正式读取均闭合。同步提供F完整保护、当前值组原子设置/清空，以D五字段更新为真实消费者，不铺无消费者泛writer。

主体/E/权重活动/父D/来源/理解确认仍旧字段单权威，列表项和复合来源目标快照不迁；不新增E/D/U/C信息对象writer，不修需求来源I64漂移，不实现筹办创建/绑定或全D骨架。

## 2. 基线与所有权

`D:/海中鱼巣`、main、`288ad4e6abc802034d0dadbdbadb621dc5eddb26`加root联合WIP，旧09fc不施工。root唯一生产/Git/index/装配/构建owner；计划agent只写本计划和具名设计，root负责唯一索引登记及精确本地发布，不能推送既有main本地提交。生产WIP和施工记录不提交不推送，保留原改动，不stash/reset/clean。

执行前按AGENTS及详细设计第2节重做S0，核实际HEAD/index/全部目标WIP、四个owner接口及构建资源；源码指纹写root专属施工记录，不能把旧知识卡当当前事实。先天FT就绪前保留legacy引导，正常入口启用迁移后再开放线程。

## 3. 允许文件

仅五个生产文件：

- `海中鱼巣/领域/新_特征类.h`、`海中鱼巣/领域/新_特征类.cpp`：完整保护读口、move-only组设置、清current及原核心组合/收尾。
- `海中鱼巣/领域/新_需求类.h`、`海中鱼巣/领域/新_需求类.cpp`：私有D五字段格式/schema、显式读取上下文、创建/复用/迁移/更新及全部正式reader。
- `海中鱼巣/装配.普通应用.cpp`：先天及概念引用reader就绪后的启用/迁移、开放门处理。

专属记录为 `验证记录/20261029_DEMAND-TARGET-FEATURE-SKELETON_施工与编译记录.md`。

不新增工程项/文件，不改DATA-L1、概念定义/正式规范、任务/本能/语言、共享知识索引或未迁D字段的合同。禁止定位wrapper冒充F、raw值双写、重建F清current、删历史、按编码猜格式、列表/复合快照误迁，以及未挂F直接裸删除。

## 4. 实施顺序

1. S0后先实现详细设计第4节全部F公开DTO/完整读口及组准备/提交/完成/结束/取消。请求F+optional当前值，同值幂等，nullopt仅清顶层current；I64真变值保留原聚合/子F正常能力。组内所有核心与D附加片段一次组合，锁/回执/零写取消完整闭合，不另造事务协议。
2. 需求私有schema增加五字段承载marker=2；free decoder和内部读取helper显式传F/概念读取上下文，真实rawD和F保护不被投影替换。逐D五F引用校验、FT根匹配、完整保护及最终复核；列表/来源快照原样。
3. 新增公开 `新需求操作状态 启用目标特征实例承载() noexcept`及私有同D确保函数。legacyD保持原单权威，准备完整合法未挂F后一次D CAS挂五F/marker，失败正式回收本次未挂F并保留旧D；重复不新建不增加命中。
4. 覆盖全部D成功、复用、无变化出口，包括双根、普通、服务来源代理、外部、引用内容、待完善/完整被认知。原出生旧回滚先收口，启用后最终返回前确保新格式；迁移后工程故障不再拆D/F。运行期晚期迁移失败保留事实并工程中止，不伪成功。
5. 完善更新按原业务条件/认知来源核验后，用F组设置五current，并把目标E/阶段/理解确认等仍属D的raw片段合批；值/方向切换或未知清current不换F。完整读回后才返回修改。普通活动/权重等旧raw操作不误覆盖五字段。
6. 覆盖所有正式D内容/身份/查询/精确匹配/发布所用reader，特别lockless身份reader进入F完整读口及最终D映射复核，更新原无领域锁注释。不得D解析F私有schema。
7. app先天及reader就绪后调用启用函数，逐D迁移成功才开放线程，部分失败同身份续作；不声明全集原子。保持既有消息、任务和本能行为。
8. root统一工程构建，记录知识变更清单、静态覆盖、未运行及未迁范围。生产WIP不提交推送；状态退出由root按索引规则处理。

精确DTO/接口、事务、物理字段映射、锁、生命周期和失败以详细设计第3—8节为准。无法裁决的真实漂移具名交计划支撑；纯代码编译问题在本切片解决，不提前登记下游库存。

## 5. 验证

创建侧检查文档尾随空格、链接与实际接口，运行 `git diff --check` 和 `git diff --cached --check`。实施静态矩阵按详细设计第8节，尤其创建/复用出口枚举、锁less身份、F批次与D阶段同批、清current及旧回滚门。

root独占实际工程Debug/x64构建，输入是HEAD加保留联合WIP，不是纯提交验收。日志：`D:/TEMP/海中鱼巣/INNATE-CONCEPT-GROUP-READ/20261028-optimization/demand-target-feature-skeleton-build.log`。

```powershell
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe' 'D:/海中鱼巣/海中鱼巣.vcxproj' /t:Build /m /p:Configuration=Debug /p:Platform=x64 /p:OutDir='D:/TEMP/海中鱼巣/INNATE-CONCEPT-GROUP-READ/20261028-optimization/bin/' /p:IntDir='D:/TEMP/海中鱼巣/INNATE-CONCEPT-GROUP-READ/20261028-optimization/obj/' /fl /flp:'logfile=D:/TEMP/海中鱼巣/INNATE-CONCEPT-GROUP-READ/20261028-optimization/demand-target-feature-skeleton-build.log;verbosity=normal'
```

记录真实退出码、警告/错误、源指纹及联合输入。仅实施无测试指令，不启动、自检、模拟、冒烟或脚本测试；计划矩阵不自行授权运行。交付只说明代码与编译，不升级为真实运行、恢复、整个需求骨架或业务闭环。
