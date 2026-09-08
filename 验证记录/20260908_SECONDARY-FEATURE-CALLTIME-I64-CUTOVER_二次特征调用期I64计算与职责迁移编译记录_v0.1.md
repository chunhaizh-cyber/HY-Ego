# 二次特征调用期 I64 计算与职责迁移编译记录

日期：2026-09-08（Asia/Shanghai）
计划：`SECONDARY-FEATURE-CALLTIME-I64-CUTOVER`，计划/设计 v0.1—v0.6 合读。
验证所有者：`/root/execute_feature_node_i64`
基线：`29e3e11d350c3dd1390ecb741038c93a4ec9f264`。
结果提交：与[施工记录](../施工记录/20260908_SECONDARY-FEATURE-CALLTIME-I64-CUTOVER_二次特征调用期I64计算与职责迁移施工记录_v0.1.md)及其候选源码同一提交；发布前尚未形成，发布后以施工记录首次加入 Git 的提交定位，不将本记录写成计划退出回执。

## 1. 隔离输入和工具

最终隔离根：`D:/TEMP/海中鱼巣/SECONDARY-FEATURE-CALLTIME-I64-CUTOVER/20260908-final-01/`。

输入由正式 HEAD archive 加本片精确 10 个现存候选文件覆盖、删除旧 `海中鱼巣/领域/数据服务.二次特征类.ixx` 构成，未复制整个 dirty 工作区。Debug/Release 分别使用独立 obj/out 子目录，顺序执行，不抢用其它任务资源。

MSBuild：`C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/amd64/MSBuild.exe`。

最终实际命令（变量仅表示上述精确隔离根）：

```powershell
$secondaryFinalRoot = 'D:/TEMP/海中鱼巣/SECONDARY-FEATURE-CALLTIME-I64-CUTOVER/20260908-final-01'
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/amd64/MSBuild.exe' "$secondaryFinalRoot/source/海中鱼巣.vcxproj" /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m:1 /nologo /v:minimal "/p:OutDir=$secondaryFinalRoot/out/Debug/" "/p:IntDir=$secondaryFinalRoot/obj/Debug/" /fl "/flp:logfile=$secondaryFinalRoot/logs/Debug-reviewed-final.log;encoding=UTF-8;verbosity=normal"
# Debug exit 0 后，同一 source 执行 Release。
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/amd64/MSBuild.exe' "$secondaryFinalRoot/source/海中鱼巣.vcxproj" /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m:1 /nologo /v:minimal "/p:OutDir=$secondaryFinalRoot/out/Release/" "/p:IntDir=$secondaryFinalRoot/obj/Release/" /fl "/flp:logfile=$secondaryFinalRoot/logs/Release-reviewed-final.log;encoding=UTF-8;verbosity=normal"
```

## 2. 最终实际结果

| 配置 | 目标 | 退出码 | 警告 | 错误 | 耗时 | 日志（隔离根相对路径） |
| --- | --- | --- | --- | --- | --- | --- |
| Debug x64 | Rebuild | 0 | 0 | 0 | 00:00:31.04 | logs/Debug-reviewed-final.log |
| Release x64 | Rebuild | 0 | 0 | 0 | 00:00:27.72 | logs/Release-reviewed-final.log |

两日志均明确出现“已成功生成”“0 个警告”“0 个错误”，并生成对应 out/Debug、out/Release 下的海中鱼巣.exe；没有启动这些程序。工程 XML 实际登记 44 ClCompile，其中43个 .ixx 模块和1个入口.cpp，另有13个头文件；filters集合一致，无重复/缺失，四新一删双登记一致。MSBuild 按真实 import 拓扑编译，XML行序不等于模块依赖拓扑。

日志 SHA256：

- Debug：`4BD20FE5DC017EDB9A7341CE57EA5E0D83A4F4A887E6C1AD10FE5363A3DED8DC`
- Release：`2BB26DBD752F9C68E764F44CC957B66F8DA68960F864DA9B059DFDD1B8C03F96`

## 3. 最终同字节互证

编译结束后，将工作区与最终 source 下的下列文件逐一 SHA256 比较，10/10相同，未再修改生产代码。Git blob 另见施工记录。

| 文件 | 工作区及隔离 source 共同 SHA256 |
| --- | --- |
| `海中鱼巣/领域/算法.有序I64特征比较.ixx` | `ACEBB285B5AB4F26ECD99E578436E4D4DEE6EB56BEAAF8B21EEE5AE31E83A9E4` |
| `海中鱼巣/业务/计算.二次特征.ixx` | `5A704E8517F05FE1A20806BAB0A4047AAFFB4AC31835CA36EC666199EF1F349F` |
| `海中鱼巣/业务/应用服务.需求目标方向.ixx` | `B8DC398A8C7FCDCBD8695A77186F3DF37D4A053F09D549A5CC205A7E08E14478` |
| `海中鱼巣/业务/应用服务.任务目标方向.ixx` | `290ABAA15E8C568BDF2465F2843FAB244AEC778B372A78056164B2E890611BD2` |
| `海中鱼巣/领域/数据服务.特征类.ixx` | `68D4493125EB2F20A82F9E12E6DFA6D495068617A140B63493D15A73EA80D315` |
| `海中鱼巣/领域/数据服务.需求类.ixx` | `D4BACDCE51CB5A78ACCAC8F8EFBF51A4249C313036F3DEEFF59E727E7AA0FC7B` |
| `海中鱼巣/领域/数据服务.任务类.ixx` | `1307A930EADA1A99F9ED4051D2F88BD81CDE9712AFF7659D981EC33B7A3329B6` |
| `海中鱼巣/领域/数据服务.方法类.ixx` | `D64EE98541560818E892A6476303AC964E787AB206CC72DC7D5E1BE2761C5025` |
| `海中鱼巣.vcxproj` | `B7912D5AC508A6C3A1A892064E510F1695E1D49A41F809434C0042A918B4FF0C` |
| `海中鱼巣.vcxproj.filters` | `618C7373F49F9F708383881BD4EB2CF446142161B2181E96EB5A03743562F278` |

旧二次 DATA 门面在工作区及最终隔离 source 均已删除；基线原 blob 为 `f745c2e41dfea340a58bbd9a3d896e816caacf6d`。

## 4. 静态核对与验证矩阵

V编号沿正式计划，不新增完成标准。下列“静态符合”只表示声明/调用/分支/返回包的源码核对，不表示运行样例、竞争、恢复或故障注入测试通过。

| 计划项 | 实际检查及结论 |
| --- | --- |
| V01 | v0.6 plan/design blob、索引可执行状态、生产白名单和公开签名逐项核对，静态符合。v0.1—v0.5继续有效，冲突按v0.6。 |
| V02 | 生产目录和两工程文件扫描旧门面/比较/方向入口与DTO，零命中；旧门面已物理删除。根目录异主未跟踪旧源码副本不在当前工程，不被误删。 |
| V03 | `海中鱼巣/领域/数据服务*.ixx` import `海中鱼巣.业务` 零命中。 |
| V04—V06 | 三来源/两类根、G/H/六预算/真实用量、完整 DAG/K与成功谓词、保留保存定义/旧治理、DATA职责退出及新方向应用，静态符合；新应用/provider无L1写入入口。 |
| V07 | 44 ClCompile/43 ixx/13头，四新一删与filters集合相同，无缺失或重复，import拓扑正确。 |
| V08—V09 | 上节同字节 Debug/Release x64 Rebuild 实际通过。 |
| V10 | 本片精确路径 `git diff --check -- <本片路径>`通过；全仓 `git diff --check`发现异主 AGENTS.md:12 尾空格，保留未修，不能声称全仓通过。Git LF→CRLF提示不计为编译warning。 |
| V11 | 主仓 `python .\\tools\\check_specs.py --strict` 实际通过，108份目录项全部通过。 |
| V12 | 发布前已核对index为空、11项生产/工程白名单及两记录；最终staged审计、提交/推送/远端/index事实以随后发布回执为准，不提前记录通过。 |
| V13—V15 | 旧初始化{1}/16角色写集、d_下标不变；独立固定扩展键与原写集恢复分支；provider状态5仅枚举历史洞，函数体无赋值/返回，静态符合。 |
| V16—V18 | 有界F/派生/当前绑定真实读量、按总材料扣费、K上下文要求位和旧固定K=0、方向原请求回显/逐位升序根/成功自包含，静态符合。 |
| V19—V23 | 扫描候选/用途匹配数分账、完整扫描后唯一裁决、非匹配坏格式/零匹配/预算失败、保留真实用量及只按材料总数扣费，静态符合；未构造实际三用途数据或运行冲突样例。 |
| V24—V28 | 建立原请求回显、首发回执/编码映射互证；同键变G/定义/写集拒绝、跨键同定义幂等冲突/异定义注册不唯一、零新发布分支，静态符合；没有运行幂等测试。 |
| V29—V31 | 不复活退出绑定、竞争失败恰一次最新G重扫、pending/重启只重放原键/G/定义/写集、首发材料恢复及最终活动读回，静态符合；没有运行竞争、未知发布、重启恢复或故障注入。 |
| V32—V36 | 批量ABI、单共享上下文、完整根左右闭包/联合叶/定义、A/B共享C缓存及全根失败原子清载荷，静态符合；未运行构图样例。 |
| V37 | 二次计算源码对单根 `读取标量派生定义`调用零，对批量入口静态调用点一处；无失败回退单根。旧需求/方法单根数据消费者保持。 |
| V38 | 批量N图项/2N来源边预查溢出及余额、联合叶/材料各一次、saved/leaves命中不重复扣费、全部局部图项预算预扣，静态符合。 |
| V39—V40 | 宿主成员/当前采用在G下入口核验及尾守卫，包内谓词不新增快照；任务尾G漂移顶层失败但保留完整方向子结果，按v0.5静态符合。 |
| V41—V44 | 额外结构/FT/单位/规则节点计入总材料，不污染定义/叶分类；总数不小于分类和、分类和溢出检查、批量原请求六上限、共享编码缓存去重，静态符合。 |
| V45—V47 | 读后格式失败计实际材料、读前预算不足不预占、失败清载荷无回退；生产派生用量不再等于四分类和，不用常数/上限冒充实际值；定义基本准入及准确F完整准入后才计逻辑类，静态符合。 |

关键扫描形式：

```powershell
rg -n '数据服务.二次特征类|比较派生特征|计算二次特征|读取二次特征概念材料|计算临时需求方向|读取任务目标方向|特征类标量比较请求|特征类标量比较结果' 海中鱼巣 海中鱼巣.vcxproj 海中鱼巣.vcxproj.filters
rg -n 'import.*海中鱼巣\.业务' 海中鱼巣/领域 -g '数据服务*.ixx'
rg -n '读取标量派生定义\(' 海中鱼巣/业务/计算.二次特征.ixx
rg -n '批量读取标量派生定义\(' 海中鱼巣/业务/计算.二次特征.ixx
python .\tools\check_specs.py --strict
```

独立只读子审计 `/root/execute_feature_node_i64/audit_secondary_s0` 最终核对特征 blob `515884d06f1be6c8481ded94b65d48ca5bb723ca`、计算 blob `72a8291c8bceb4c0d681555693bf29b25290680e`，发布范围/扫描/双登记通过；未替代主执行者构建，未运行程序，也未将未生成时的记录或最终暂存集提前审为通过。

## 5. 中间失败与纠正分账

- compile-01首稿因中文标识符紧邻三元问号发生MSVC词法/语法错误；统一三元空格后消除。
- 旧治理调用的三参数标量计数重载缺失，已恢复；绑定模板参数R与类内别名冲突改为BindingRequest。
- MSVC C1001曾在未修改的概念树导入IFC时出现；只移除本片新/修改函数非必要inline后重新构建通过，未改概念树或公开ABI。不将中间失败隐去，也不将其描述为最终仍失败。
- Debug-syntax03、Debug-ifc07、Debug-batch08等是中间候选通过，不作为最终同字节证据；final目录的Debug-final.log也早于最后闭包修正。最终仅采用reviewed-final两日志。
- 曾误在无.git的archive目录运行strict，出现3条acceptance_record HEAD/origin环境检查错误；随后在正式主仓正确运行，108项全部通过。不是未修规范缺陷。

## 6. 残留与未运行边界

最终验证进程已退出。本轮临时目录由执行者按 CODE-FILE-01 第5.1.2节在发布收口时清理；涉及的本片目录仅：
`D:/TEMP/海中鱼巣/SECONDARY-FEATURE-CALLTIME-I64-CUTOVER/20260908-compile-01/`、
`D:/TEMP/海中鱼巣/SECONDARY-FEATURE-CALLTIME-I64-CUTOVER/20260908-final-01/`。
清理的实际结果由发布回执明确说明，不能提前写为已清理；其它所有者目录、仓库内旧.codex-build和未跟踪源码不处理。

NOT_RUN：程序运行、单元/端到端测试、数据库写入/迁移、故障注入、真实竞争、跨进程恢复、真实外设、集成验收和线程业务链。没有证明生产可达、运行算法正确、RC、持久二次实例或整体二次特征体系完成。编译日志中的成功只证明编译链接。
