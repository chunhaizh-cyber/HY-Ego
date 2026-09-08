# FEATURE-CONCEPT-MODEL-CUTOVER 编译记录

时间：2026-09-07 起记录；最新回执为 2026-09-08 v0.5（第 7 节）。

结论：最终同一候选 Debug/Release x64 Rebuild 均 exit 0、0 警告/0 错误，14 个源码模块均编译并链接。历史失败轮次原样保留；未运行任何程序或测试，不作业务/恢复/集成验收声明。

## 1. 输入与工具

- 执行者：`/root/execute_feature_node_i64`；主线程要求不提交、不推送、不释放占用。
- 正式导出 HEAD：`748e81c5fb61ff477921f60778fde68682067a19`。
- 计划：`计划/20260907_FEATURE-CONCEPT-MODEL-CUTOVER_准确特征与特征概念模型切换代码实施计划_v0.2.md`，blob `1b2ffcb0c111b32b443636b430743a061832b326`。
- 详细设计：对应 v0.2，blob `81755b11f6ce772a7ce34a43109c140503925722`。
- 隔离根：`D:/TEMP/海中鱼巣/FEATURE-CONCEPT-MODEL-CUTOVER/compile-20260907-01/`。
- MSBuild：`C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/amd64/MSBuild.exe`；FileVersion `18.5.4.18101`，ProductVersion `18.5.4+cb4e32d21bd58cfed6b8599bb4780cf64bd15afc`。
- 编译器实际日志路径：`C:/Program Files/Microsoft Visual Studio/18/Professional/VC/Tools/MSVC/14.50.35717/bin/HostX64/x64/CL.exe`；项目保留 v145、C++20、`/utf-8`。

隔离目录创建前检查不存在；`git archive HEAD --format=zip --output=<隔离根>/head.zip` 后 `Expand-Archive` 到 `source`，仅 `Copy-Item -LiteralPath` 覆盖计划13源码与 `.vcxproj/.filters`。没有复制整个dirty工作树、没有 git worktree、没有测试源码或数据库。两配置输出分别为 `out/Debug`、`obj/Debug` 与 `out/Release`、`obj/Release`，日志在 `logs`。

实际命令模板（每轮 `<配置>/<日志名>` 见下表）：

```powershell
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/amd64/MSBuild.exe' 'D:/TEMP/海中鱼巣/FEATURE-CONCEPT-MODEL-CUTOVER/compile-20260907-01/source/海中鱼巣.vcxproj' /t:Rebuild /p:Configuration=<配置> /p:Platform=x64 /m:1 '/p:OutDir=D:/TEMP/海中鱼巣/FEATURE-CONCEPT-MODEL-CUTOVER/compile-20260907-01/out/<配置>/' '/p:IntDir=D:/TEMP/海中鱼巣/FEATURE-CONCEPT-MODEL-CUTOVER/compile-20260907-01/obj/<配置>/' /nologo /v:minimal /fl '/flp:LogFile=D:/TEMP/海中鱼巣/FEATURE-CONCEPT-MODEL-CUTOVER/compile-20260907-01/logs/<日志名>;Encoding=UTF-8;Verbosity=normal'
```

## 2. 实际各轮结果

| 配置/日志（均在隔离根 logs） | 退出码 | 观察与后续 |
| --- | --- | --- |
| Debug / `Debug-first.log` | 1 | 存在新采用代码中文标识紧邻三元 `?` 导致 C2838/C2653 等；按报错行补空格后下一轮该组错误消失。 |
| Debug / `Debug-second.log` | 1 | 存在已编译；首次暴露白名单外世界树历史枚举。概念树另有 if-init 变量重名、导出区 static helper、显式身份构造错误，均在白名单内修复。 |
| Debug / `Debug-third.log` | 1 | 世界树仍失败；概念树剩拓扑 visitor 旧字段与显式构造错误，继续精确修复。 |
| Debug / `Debug-fourth.log` | 1 | 报错只剩世界树旧历史枚举；概念树、特征、存在、状态、动态、需求、任务、方法、二次等领域对象已产生。 |
| Release / `Release-first.log` | 1 | 采用最终安全收口源码，仍只有世界树旧历史枚举组错误；日志末尾1警告、38错误，耗时21.93秒。 |

Debug第四轮之后，特征类进一步保存标量唯一原请求并加完整叶检查，特征概念应用进一步修错误映射、命名读取和规则缺失前置。最终Release包含这些改动；没有把第四轮Debug冒称为最终同一候选双配置成功。

最早越界错误：

```text
海中鱼巣/业务/应用服务.世界树类.ixx(550,9): error C2061: 语法错误: 标识符“特征引用读取状态”
海中鱼巣/业务/应用服务.世界树类.ixx(694,54): error C2653: “特征引用读取状态”: 不是类或命名空间名称
海中鱼巣/业务/应用服务.世界树类.ixx(696,21): error C2660: “海中鱼巣::世界树应用服务::映射存在历史状态”: 函数不接受 1 个参数
```

原因是存在历史状态已正式归属存在类、而世界树是漏列消费者。执行者没有修改该文件、没有旧别名、没有剔除模块。四个业务概念模块虽然都进入模块依赖扫描，但受世界树失败阻断未完成实际编译，不宣称它们无编译错误。

## 3. 最终候选一致性

2026-09-07 23:40:43，工作区与隔离 source 的15个允许输入用 `Get-FileHash -Algorithm SHA256` 逐项比较，全为 true；下表记录最终输入，不反推较早调试轮次的字节快照。

| 文件（仓库相对路径） | SHA256 |
| --- | --- |
| 海中鱼巣/领域/数据服务.特征类.ixx | 782889921B85A83CA044269AB25C31175AA659344E89C79285DF3E8B28A7A716 |
| 海中鱼巣/业务/应用服务.特征概念类.ixx | C5BBF362020590FA9025A5BC9CADD7792A57F8FFDBBEA1DDF7E77EF2C491C78C |
| 海中鱼巣/领域/数据服务.状态类.ixx | 6FE63FEA3FF15E890FE56C9F76F8EE98BCDC8DB94FA17BD4F989CDA6BA2F17E6 |
| 海中鱼巣/领域/数据服务.动态类.ixx | 1083FE22F4521E95949BFF8D7CCDFFB9DC2BBB33CECC578E33884F77A97C5253 |
| 海中鱼巣/领域/数据服务.存在类.ixx | E69A92AA73035614171D3F1069C526FFC4FFAF392A1EF857F6893258BE24AECA |
| 海中鱼巣/领域/数据服务.需求类.ixx | FC24C5003628ECF04F60CAC25B32099E4962D0F4D92C5DBE934E2DCDC6E9A685 |
| 海中鱼巣/领域/数据服务.任务类.ixx | 68591810F6C12782F0C51C28AC9378C72A247743C258AFD8AA5E3F08F4E1A018 |
| 海中鱼巣/领域/数据服务.方法类.ixx | 303F9A9EF370B806F4F88EA77DD98CC80A07F03E1A084C8F5B18CF447AA077EC |
| 海中鱼巣/领域/数据服务.二次特征类.ixx | 42A2C95052AB654B2C67C6AD2D4CFC977385A03D50AE7ACB282C45B824DBBF19 |
| 海中鱼巣/领域/数据服务.概念树类.ixx | F1A27F700513B39D55DC527E0D2A15E24129AB274C3372617760A6692BAC16EE |
| 海中鱼巣/业务/应用服务.存在概念树类.ixx | 5474576EFEA4B42EF678E81F07D0F216B80150A77F6ABFA8BF7E47C3BDB64DFF |
| 海中鱼巣/业务/应用服务.场景成员概念类.ixx | 28D31BAA7E26F12666E2E744BA3D2F0944A4FF9CD6F09A312BE67561D9A0F314 |
| 海中鱼巣/业务/应用服务.动态概念树类.ixx | E73AC93B2BCEDF58D427126D6C644DF402F743D3DFEC66E57629CB483A8F5E3A |
| 海中鱼巣.vcxproj | 36A682858A82774D7440B9B1356990A1B72D55DD78E5D82506AFDB78BB7D37B3 |
| 海中鱼巣.vcxproj.filters | 652224869F80258C82562AE13790F9F5FEB76D881194FA9AF8F87971017C05B7 |

## 4. 限定静态检查

对计划第5节退役符号正则，仅以该计划13源码为精确 `rg -n -- <files>` 输入。命中11行，全部在尚未修改的 `海中鱼巣/业务/应用服务.场景成员概念类.ixx`：32、47、109、117、148、163、178、182、183、208、220。涉及旧生成/关联请求、继续材料、共享定义及旧生成/关联调用；该范围因设计缺口暂停，不能视为通过。其余12个文件该模式零命中，不扫描历史文档和异主未跟踪副本来制造噪声。

`git diff --check -- <13源码> 海中鱼巣.vcxproj 海中鱼巣.vcxproj.filters` 无差异错误；换行CRLF提示不当作业务错误。`git diff --cached --check` 无错误，index为空。全仓 `git diff --check` 报异主 `AGENTS.md:12` 尾空格，保持不动。

`python .\tools\check_specs.py --strict` 在两专属记录写入后再次退出0，输出“海中鱼巣正式规范目录一致性检查: 101 份目录项全部通过”。精确13源码/工程 `diff --check` 再次退出0；两未跟踪记录各用 `git diff --no-index --check -- NUL <记录>` 检查，退出1表示相对空文件存在差异，输出仅CRLF提示、没有空白差错诊断；该退出码不写成0。最终index仍空，HEAD不变。

旧v1专项：原七角色/端口路由、独立初始化门禁、16/4解码、原H闭包、9/10项退出、零槽变更及未知请求保留已有实现；未逐项完成全部静态矩阵，更未运行旧格式恢复或数据验证。不得把领域编译成功替代专项矩阵PASS。

## 5. 剩余与边界

需支撑闭合的4项：名称历史flag过量承诺、场景公开DTO与恢复材料、EC抽象依据结构交付、世界树精确白名单扩充。代码所有者仍需完成通用新C治理转接、全阶段预算/历史/成功谓词审查、E共同串行保护，以及解阻后四个应用的实际编译和场景迁移。详见施工记录第5节。

没有运行exe、测试、数据库、故障注入、集成验收；没有stage、commit、push或清理现场。隔离目录/日志、11个源码WIP、两份记录和所有占用均保留。此次仅证明已做这些有限检查且存在具名阻断，不证明计划完成。

## 6. 2026-09-08 v0.3 中间隔离编译

当次正式导出 HEAD `2339ce0724164137d26438a3dce4587e2b84f3c9`；计划 v0.3 `ff2854ff02effbf42397b8c8944a020a33223687`，设计 v0.3 `646d00f15a8f6d6d863c1254147c07c318201521`。使用原MSBuild、v145/C++20/utf8；新隔离根 `D:/TEMP/海中鱼巣/FEATURE-CONCEPT-MODEL-CUTOVER/compile-20260908-01/` 创建前确认不存在。导出该精确 HEAD 后只覆盖14源码及2工程文件（相比上一隔离根增加世界树），不带异主WIP。Debug 的 OutDir/IntDir 与 Release 分开，所有日志保留；每轮均 `/t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m:1`，`/flp` 的日志为下表名称、UTF-8、normal。没有剔除场景成员或其它失败模块。

| 日志（新隔离根 logs） | exit | 结果及修复 |
| --- | --- | --- |
| `Debug-first.log` | 1 | 特征/状态/动态/存在/世界树等进入实际编译；新通用释放转换使用了不存在的 `生命周期` 字段，按既有 DTO 改为 `关系生命周期`。 |
| `Debug-second.log` | 1 | 概念树/世界树编译完成；动态概念应用引用不存在的 `动态概念应用状态::不支持`，改用现有 I64入口拒绝，不伪造未命中。 |
| `Debug-third.log` | 1 | 特征/动态概念应用编译完成；存在概念结果 visitor 对新特征C仍读旧 `概念` 字段，按 union 分支改读 `身份`。 |
| `Debug-fourth.log` | 1 | 所有领域模块及世界树、特征概念、动态概念、存在概念实际编译，随后仅场景成员旧DTO报错；汇总0警告/82错误，32.22秒。首错为场景成员第23行 `共享特征实例事实` 未声明。 |

第四轮已包含标量强类型来源与FT预算、Fapp两个provider恢复、通用新C治理/pending、存在共同写锁及本段所有编译修复。场景合同缺口尚未获正式支撑，本文件仍未修改；不能用其旧DTO退役连锁报错掩盖整个工程未通过。这里的“某模块实际编译”只指本轮CL编译并产生对象，不是链接、运行或业务验证。

本段截至09:13 +08:00没有运行Release。待场景/名称/EC支撑完成与代码迁移后，仍须对最终同一候选重新取得Debug/Release双配置证据，不能复用此处中间失败轮次当成功验收。

09:14，精确16输入与新隔离 source 的 SHA256 逐项相等。相比第3节，状态、动态、需求、任务、方法、二次特征、场景成员和两工程共9项哈希不变；其余7项如下（世界树为新增项）：

| 文件 | SHA256 |
| --- | --- |
| 海中鱼巣/领域/数据服务.特征类.ixx | 833B65898C708A525E947B74F30EFABB5474185B6779760BC442731132D339E4 |
| 海中鱼巣/领域/数据服务.存在类.ixx | C8C03BB64A5DED6FE6D4263B54553F24CF288841B4C272EAC6F5DD17D40232EA |
| 海中鱼巣/领域/数据服务.概念树类.ixx | 0F16E5A3EE21201236F3DBFA60C32706A9E9FBEE43ED2BE905949B8293EA3B91 |
| 海中鱼巣/业务/应用服务.特征概念类.ixx | 9B095304D5588B0DB60D7DC029DFC92E2438C25F1E5E7CF6B304EE5BE1317B26 |
| 海中鱼巣/业务/应用服务.存在概念树类.ixx | DC5FA37482A94DA8EF1CF3C9084EBAF44C6167BDF48316725C0EEAF44F573B16 |
| 海中鱼巣/业务/应用服务.动态概念树类.ixx | 1AC0F02B58C6D6A52EFAC7152671CDDF64F1B04E189A34A0610B64CAFB613B6A |
| 海中鱼巣/业务/应用服务.世界树类.ixx | AE73605F4E96B283B5EA456522B0F2AB29938F37FD01ED418BEA244B5BB2B00C |

该16输入 `git diff --check` 及 `git diff --cached --check` 无空白差错；index仍空。`python .\tools\check_specs.py --strict` 再次 exit0，101份目录项通过。规范一致性检查不代替代码、编译或业务结果。

### 6.1 09:27 +08:00 后续加固编译

| 日志 | 配置 | exit | 结果 |
| --- | --- | --- | --- |
| `Debug-fifth.log` | Debug/x64 Rebuild | 1 | 纳入旧 v1 深度/真实来源owner/首次材料形状/成功谓词与采用容量/读回/pending/结果谓词加固；仍只有场景旧DTO，0警告/82错误，31.18秒。 |
| `Release-first.log` | Release/x64 Rebuild | 1 | 在第五轮上再纳入旧原键误指退出集合内注册节点的确定冲突映射；仍只有场景旧DTO，0警告/82错误，27.26秒。 |

最后Release实际使用 `out/Release/`、`obj/Release/`，其它命令开关同第6节、Configuration改为Release。两轮所有领域/世界树及前三概念应用均实际编译；未形成完整工程链接成功。Release后源码未改，最后两个变化输入SHA256为：

- 特征类：`E02545331B0813D439EC93439894C2D8ED06753D4B94253BA77145FCCC684CAA`。
- 存在类：`3E6CA2144C2B8B20D45C88B1B34AE900B40C3BD9BD0AD61A50F87F8418FB9BB7`。

其余14输入保持第6节09:14核对值。最后旧原键边界小修只有Release编译证据，不能把第五轮Debug冒称同一最终字节候选。所有本段构建与此前2026-09-07失败日志均保留。未运行测试/二进制、未发布、未释放登记；本计划尚未完成。

## 7. 2026-09-08 v0.5 同候选最终编译

### 7.1 正式输入和隔离

正式 HEAD/origin：`5ee25943bdf3dc4c68d48645062023be5437e775`。计划 v0.5 blob `0b82aff3ba358534212ec561c61f976e81ddf33b`、设计 blob `5230be393d1a6cdfefeb95de6d248c0508b7f54c`；索引可执行，main、index 空，无分支/新 worktree。

隔离根 `D:/TEMP/海中鱼巣/FEATURE-CONCEPT-MODEL-CUTOVER/compile-20260908-02/` 创建前确认不存在。用 `git archive --format=zip --output=<根>/HEAD.zip HEAD` 导出后解压到 source，再精确复制下列 14 源码和两工程文件。构建开始期间设计发布完成，因此另读 ZIP comment 确认实际归档基底就是上述 `5ee25943...`，不凭后续 HEAD 输出猜基底。没有复制异主源码/文档 WIP 或根目录未跟踪副本；没有剔除工程模块。

### 7.2 本根全部编译轮次

| 日志，位于本根 logs | exit | 警告/错误 | 用时 | 实际结论 |
| --- | --- | --- | --- | --- |
| Debug-first.log | 1 | 0/1 | 28.29 秒 | DC 回执核验误读合法动态模板字段 `概念`；按真实 DTO 改为 `模板`。本轮场景恢复尚未最终冻结，不作完成证据。 |
| Debug-second.log | 1 | 0/44 | 30.87 秒 | 全部领域及 EC/DC/Fapp 已实际编译；场景缺 vector 头导致导入 DTO 比较删除，紧贴中文标识符的三元表达式被 MSVC 合并解析引出连锁错误。 |
| Debug-third.log | 1 | 0/5 | 29.17 秒 | 容器比较已修；剩一处 `特征应用?nullptr` 同类解析问题，补空格后修复。 |
| Debug-fourth.log | 0 | 0/0 | 31.72 秒 | 完整 Debug 编译链接第一次通过；之后另补 EC/DC 初始预算与反向状态分账，故不是最终字节证据。 |
| Debug-final.log | 0 | 0/0 | 33.44 秒 | 含最后 EC/DC 修复，全部 14 源码实际编译并链接。 |
| Release-final.log | 0 | 0/0 | 27.37 秒 | 与 Debug-final 同一 source 字节，无中间改代码；全部 14 源码实际编译并链接。 |

每次均 `Rebuild`，未使用跳过消费者、假接口或仅编译两模块的工程。MSBuild 固定为：

```powershell
$featureCompileRoot = 'D:/TEMP/海中鱼巣/FEATURE-CONCEPT-MODEL-CUTOVER/compile-20260908-02'
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/amd64/MSBuild.exe' "$featureCompileRoot/source/海中鱼巣.vcxproj" /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m:1 /nologo /v:minimal "/p:OutDir=$featureCompileRoot/out/Debug/" "/p:IntDir=$featureCompileRoot/obj/Debug/" /fl "/flp:logfile=$featureCompileRoot/logs/Debug-final.log;encoding=UTF-8;verbosity=normal"
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/amd64/MSBuild.exe' "$featureCompileRoot/source/海中鱼巣.vcxproj" /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m:1 /nologo /v:minimal "/p:OutDir=$featureCompileRoot/out/Release/" "/p:IntDir=$featureCompileRoot/obj/Release/" /fl "/flp:logfile=$featureCompileRoot/logs/Release-final.log;encoding=UTF-8;verbosity=normal"
```

工程自身 v145/C++20/`/utf-8`，未定义测试宏。Debug 和 Release 的 OutDir/IntDir 分开，日志与二进制保留但未执行。

### 7.3 退役符号与调用扫描

实际先逐条以完整 14 路径参数运行，再用以下同一参数数组逐条复验。四次 `rg` 均无文本命中、exit 1；外层 JSON 汇总脚本 exit 0 不代表有命中。

```powershell
$featureScanFiles = @(
    '海中鱼巣/领域/数据服务.特征类.ixx',
    '海中鱼巣/业务/应用服务.特征概念类.ixx',
    '海中鱼巣/领域/数据服务.状态类.ixx',
    '海中鱼巣/领域/数据服务.动态类.ixx',
    '海中鱼巣/领域/数据服务.存在类.ixx',
    '海中鱼巣/领域/数据服务.需求类.ixx',
    '海中鱼巣/领域/数据服务.任务类.ixx',
    '海中鱼巣/领域/数据服务.方法类.ixx',
    '海中鱼巣/领域/数据服务.二次特征类.ixx',
    '海中鱼巣/领域/数据服务.概念树类.ixx',
    '海中鱼巣/业务/应用服务.存在概念树类.ixx',
    '海中鱼巣/业务/应用服务.场景成员概念类.ixx',
    '海中鱼巣/业务/应用服务.动态概念树类.ixx',
    '海中鱼巣/业务/应用服务.世界树类.ixx'
)
$featurePatterns = @(
    '特征值域项标识|特征历史值项|历史值集合|值域集合|历史集合属性|域集合属性|细分关系|修改特征信息|设置特征类型|移除特征类型|接收观察值|移除准确当前观察值|读取细分子节点|读取细分父节点|读取值域生成子节点|读继承域|修改类型副本|新域键',
    '共享特征生成请求|共享特征关联请求|共享特征继续材料|共享步骤原请求|生成并关联共享特征|关联已有共享特征|推进实例|特征长期观察积累请求|特征原子实际值见证|特征原子域判定事实|特征长期只读用量|概念树共享定义事实|概念树共享特征定义|概念树共享实例见证',
    '特征类结点|特征引用读取状态|查询特征\(|按实例读取特征历史事实|共享特征应用预算|共享场景限制|共享特征业务状态|共享特征阶段状态',
    '特征概念应用预算|特征概念继续材料|存在当前采用请求_v2|存在当前采用结果_v2|struct 场景特征概念请求|struct 场景存在概念请求|struct 场景成员概念继续请求|struct 共享场景特征见证|struct 场景组合投影 final|共享特征实例事实'
)
foreach ($featurePattern in $featurePatterns) {
    rg -n $featurePattern @featureScanFiles
    [pscustomobject]@{ Pattern=$featurePattern; ExitCode=$LASTEXITCODE } | ConvertTo-Json -Compress
}
```

扫描范围仅工程实际源码白名单，不扫描历史文档或根目录异主未跟踪副本。合法保留：旧 v1 独立治理/原七角色审计、普通任务参数槽、存在已知关系、类型域 DTO/算法、合法动态模板和 L1 G/H/历史证据；以上不是退役五字段、按 E 跟踪或旧场景联合 ABI。四组精确模式本身没有合法保留命中。

附加实际命令：

```powershell
rg -n '共享特征应用预算|共享场景限制|共享特征业务状态|共享特征阶段状态' '海中鱼巣' -g '*.ixx' -g '*.cpp' -g '*.h'
rg -n '概念原请求|概念写入|收敛特征概念写入' '海中鱼巣/业务/应用服务.场景成员概念类.ixx'
rg -n 'struct 特征概念观察请求|struct 存在概念应用预算|struct 存在概念场景限制|enum class 存在概念业务状态|enum class 存在概念阶段状态' 海中鱼巣 -g '*.ixx'
rg -n '处理类型观察|收敛类型观察|收敛当前采用|读取当前采用|判定应用特征模板|读取特征当前名称' '海中鱼巣/业务/应用服务.场景成员概念类.ixx'
rg -n '数据服务\.(特征类|状态类|动态类|存在类|需求类|任务类|方法类|二次特征类|概念树类)|应用服务\.(特征概念类|存在概念树类|动态概念树类|场景成员概念类|世界树类)' 海中鱼巣.vcxproj
```

前两命令 exit 1、无输出；第三 exit 0、五个定义各恰一处，四个 EC 类型全部只定义在 EC 模块；第四 exit 0，首次/续行分别进入 Fapp 处理/收敛、E 仅调用既有采用入口，概念只读判定和名称；第五 exit 0、14 个 ClCompile 登记齐全。

### 7.4 最终 16 输入 SHA256

两配置完成后逐项比较工作区和 source，下列全部相等；源码自 Debug-final 开始没有改变。

| 文件 | SHA256 |
| --- | --- |
| 海中鱼巣/领域/数据服务.特征类.ixx | DA20B789818D8F167B5580E4C762DED7D697FC085ADA5ED53362D3134AA5C263 |
| 海中鱼巣/业务/应用服务.特征概念类.ixx | 6F7FEDD18E38F04A6E57CF275D065B6AF1BCE202500E15C25131AA4EFE0B8DBC |
| 海中鱼巣/领域/数据服务.状态类.ixx | 7B068BA2E0CB0388E3364D5B44924029255BF87D2F6E016CBAEEF8294BA54C8B |
| 海中鱼巣/领域/数据服务.动态类.ixx | 1083FE22F4521E95949BFF8D7CCDFFB9DC2BBB33CECC578E33884F77A97C5253 |
| 海中鱼巣/领域/数据服务.存在类.ixx | 3E6CA2144C2B8B20D45C88B1B34AE900B40C3BD9BD0AD61A50F87F8418FB9BB7 |
| 海中鱼巣/领域/数据服务.需求类.ixx | AE450F6820D57AB3CCE1174DDDC116EBA507FA14DA994AD94D3DFA29BD730BEC |
| 海中鱼巣/领域/数据服务.任务类.ixx | B40BA66CBBCA90F77ED97A633641FA172BD3556CD3BC1BAB38E0A5939139375E |
| 海中鱼巣/领域/数据服务.方法类.ixx | 303F9A9EF370B806F4F88EA77DD98CC80A07F03E1A084C8F5B18CF447AA077EC |
| 海中鱼巣/领域/数据服务.二次特征类.ixx | 42A2C95052AB654B2C67C6AD2D4CFC977385A03D50AE7ACB282C45B824DBBF19 |
| 海中鱼巣/领域/数据服务.概念树类.ixx | 23A0735F780E4EC970A954B3D01572214B3924B13FEB917325F42E8DA94FBFF8 |
| 海中鱼巣/业务/应用服务.存在概念树类.ixx | 7EF05C88475BED041E229DC5E77FD7907C75A7D77A9E7EE87A83D7CED8DF45ED |
| 海中鱼巣/业务/应用服务.场景成员概念类.ixx | 1D4BC2BE54F7C8C91786310876C8FF8D73C6DE5EB8484D93F474F237A2556423 |
| 海中鱼巣/业务/应用服务.动态概念树类.ixx | 0BB591FA23AB3B9DF62612C743DADE18573AB5F465511E0834A3DDEC056B8773 |
| 海中鱼巣/业务/应用服务.世界树类.ixx | AE73605F4E96B283B5EA456522B0F2AB29938F37FD01ED418BEA244B5BB2B00C |
| 海中鱼巣.vcxproj | 36A682858A82774D7440B9B1356990A1B72D55DD78E5D82506AFDB78BB7D37B3 |
| 海中鱼巣.vcxproj.filters | 652224869F80258C82562AE13790F9F5FEB76D881194FA9AF8F87971017C05B7 |

二次特征类与两个工程文件虽无差异，仍在输入清单并参与校验，不造空编辑。

### 7.5 文本门禁、发布与未验证范围

`git diff --check -- <上表16项精确路径>` exit 0；`git diff --cached --check` exit 0，index 空。全仓 `git diff --check` 仅报异主 `AGENTS.md:12` 既有尾空格，未修改该文件；本计划 v0.2 明确允许单列此基线问题。`python .\tools\check_specs.py --strict` exit 0、101 份目录项通过。

未运行程序、测试、数据库、故障注入、恢复或集成验收。本证据只证明当前切片静态迁移与编译/链接闭合，不证明生产装配、跨进程 F pending、历史数据迁移或业务闭环。源码与两份记录仍未提交；未取得 Git 发布租约前不 stage/commit/push，未释放登记、未清理旧隔离日志和异主 WIP。

## 8. 2026-09-08 场景只读续行中间候选

第 7 节代码及记录后来已由 `ea2c3f950d8e349d1738d73dc1a63eed491e6603` 提交推送；上节末的未发布描述是当时状态。退出复核发现后两阶段误入 Fapp 写流程，随后按原 v0.5 计划恢复场景单源码及两记录切片。本轮仍有准确值互证 ABI 缺口，不能宣称修复完成。

### 8.1 精确输入及命令

新根 `D:/TEMP/海中鱼巣/FEATURE-CONCEPT-MODEL-CUTOVER/compile-20260908-03/` 创建前确认不存在。执行：

```powershell
$featureCompileRoot = 'D:/TEMP/海中鱼巣/FEATURE-CONCEPT-MODEL-CUTOVER/compile-20260908-03'
git archive --format=zip "--output=$featureCompileRoot/HEAD.zip" ea2c3f950d8e349d1738d73dc1a63eed491e6603
Expand-Archive -LiteralPath "$featureCompileRoot/HEAD.zip" -DestinationPath "$featureCompileRoot/source"
Copy-Item -LiteralPath 'D:/海中鱼巣/海中鱼巣/业务/应用服务.场景成员概念类.ixx' -Destination "$featureCompileRoot/source/海中鱼巣/业务/应用服务.场景成员概念类.ixx"
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/amd64/MSBuild.exe' "$featureCompileRoot/source/海中鱼巣.vcxproj" /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m:1 /nologo /v:minimal "/p:OutDir=$featureCompileRoot/out/Debug/" "/p:IntDir=$featureCompileRoot/obj/Debug/" /fl "/flp:logfile=$featureCompileRoot/logs/Debug-first.log;encoding=UTF-8;verbosity=normal"
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/amd64/MSBuild.exe' "$featureCompileRoot/source/海中鱼巣.vcxproj" /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m:1 /nologo /v:minimal "/p:OutDir=$featureCompileRoot/out/Release/" "/p:IntDir=$featureCompileRoot/obj/Release/" /fl "/flp:logfile=$featureCompileRoot/logs/Release-first.log;encoding=UTF-8;verbosity=normal"
```

场景文件 source 与工作区 SHA256 均为 `563F94D89ABA6524F9E21DEEB49B880D4F91E2563A3EB396FAB71F6AD9B448B4`；规范化 Git blob 为 `270af2a258261f3e6a7ead5f05c149d4df38b9f3`，正式基底旧 blob `4acb35d55edfb3b3c6d0b1ba5bad5148e1d07512`。第 7.4 节 16 个输入分别以 `git hash-object --path=<相对路径> -- <source文件>` 对比 `git rev-parse ea2c3f95:<相对路径>`，恰只有场景这一项不同；其余 15 项等于正式基底，没有复制其它 WIP，也未删改工程输入。

### 8.2 结果与边界

| 日志 | exit | 警告/错误 | 用时 |
| --- | --- | --- | --- |
| logs/Debug-first.log | 0 | 0/0 | 32.81 秒 |
| logs/Release-first.log | 0 | 0/0 | 27.50 秒 |

两配置之间源码未改，同一个 source 候选完整编译链接，14 个计划源码实际编译。单场景 `git diff --check` exit 0；`python .\tools\check_specs.py --strict` exit 0、101 项通过。`rg -n '收敛特征概念写入|概念原请求' <场景文件>` 无命中、exit 1。当前阶段分支静态核对说明后两阶段没有 Fapp 写调用，但不能以这个零命中替代原准确值互证。

独立复核发现只读分支无法经既有公开 ABI 对原请求准确值进行解引用等值校验；具体原因与回传见施工记录 §8.6。当前保留安全 WIP，等待正式 v0.6 设计/计划；这两次通过仅为中间编译证据，后续改代码必须重新取得最终同字节双配置。未执行二进制、测试、数据库、恢复、故障注入或集成验收；未提交本段 WIP、不申请发布租约，保留占用和隔离日志。

## 9. 2026-09-08 v0.6 最终只读核验候选

### 9.1 冻结输入与构建

正式基底 `789a1859646f0e769d883ea8f11b3df5df48c94a`；v0.6 plan blob `060e01341288c033734d0c43c68b01ce8224aa88`、design blob `fa5dd374a3de7ddf8b1110aff25feb2054c7a38e`，索引可执行。新根 `D:/TEMP/海中鱼巣/FEATURE-CONCEPT-MODEL-CUTOVER/compile-20260908-04/` 创建前确认不存在，只从正式提交导出并覆盖两份获准源码：

```powershell
$featureCompileRoot = 'D:/TEMP/海中鱼巣/FEATURE-CONCEPT-MODEL-CUTOVER/compile-20260908-04'
git archive --format=zip "--output=$featureCompileRoot/HEAD.zip" 789a1859646f0e769d883ea8f11b3df5df48c94a
Expand-Archive -LiteralPath "$featureCompileRoot/HEAD.zip" -DestinationPath "$featureCompileRoot/source"
$featurePatchFiles = @('海中鱼巣/业务/应用服务.特征概念类.ixx','海中鱼巣/业务/应用服务.场景成员概念类.ixx')
foreach ($featureFile in $featurePatchFiles) { Copy-Item -LiteralPath "D:/海中鱼巣/$featureFile" -Destination "$featureCompileRoot/source/$featureFile" }
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/amd64/MSBuild.exe' "$featureCompileRoot/source/海中鱼巣.vcxproj" /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m:1 /nologo /v:minimal "/p:OutDir=$featureCompileRoot/out/Debug/" "/p:IntDir=$featureCompileRoot/obj/Debug/" /fl "/flp:logfile=$featureCompileRoot/logs/Debug-second.log;encoding=UTF-8;verbosity=normal"
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/amd64/MSBuild.exe' "$featureCompileRoot/source/海中鱼巣.vcxproj" /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m:1 /nologo /v:minimal "/p:OutDir=$featureCompileRoot/out/Release/" "/p:IntDir=$featureCompileRoot/obj/Release/" /fl "/flp:logfile=$featureCompileRoot/logs/Release-final.log;encoding=UTF-8;verbosity=normal"
```

首次 Debug 使用相同命令但日志为 `Debug-first.log`，新函数定义带 `inline`。该轮 MSVC 在场景模块 432 行导入 IFC 时触发 C1001（`symbols.c:5692`）；仅删除新函数定义的 `inline` 并精确重拷 Fapp 后重做 Debug，公开签名、可见性和语义未改。没有降低编译选项、跳过消费者或修改工程。

| 日志（本根 logs） | exit | 警告/错误 | 用时 | 结论 |
| --- | --- | --- | --- | --- |
| Debug-first.log | 1 | 0/1 | 29.16 秒 | 内联模块函数导入时的编译器内部错误，保留失败日志。 |
| Debug-second.log | 0 | 0/0 | 29.88 秒 | 最终普通模块函数定义；14 个计划源码实际编译并链接。 |
| Release-final.log | 0 | 0/0 | 27.43 秒 | 同一最终 source 字节，完整编译链接。 |

最终两源码 source 与工作区逐项 SHA256 和 Git blob 相等：

| 源码 | SHA256 | Git blob |
| --- | --- | --- |
| 海中鱼巣/业务/应用服务.特征概念类.ixx | `1A31C6FF89E7117E2787B8BE3BD6F7F0C08E0F3671E6678565DC2C98070ED3FC` | `5f6b390c100adf5cc12fc6a0804fa9caa81672b5` |
| 海中鱼巣/业务/应用服务.场景成员概念类.ixx | `0304DB65031D90B34928B03040C4F75DC07020BDEBD272D528765CA2C640514A` | `bff29970c61a0d769c491eeaea43450760df7e6c` |

第 7.4 节全部 16 输入按 §8.1 的 blob 比对方法复核，基底改为 `789a1859`：恰仅上表两项不同，其余 14 项均等于正式 HEAD；两源码没有在 Debug-second 与 Release-final 之间修改。项目 source 不含异主 WIP。

### 9.2 静态命令与结果

旧符号四组使用 §7.3 原完整模式，在同一 14 路径白名单重跑，均无输出、exit 1。曾尝试自动提取模式时嵌套 PowerShell `-match` 覆盖 `$Matches`，所得 null 模式结果已弃用；修正为先保存捕获字符串并断言 14 文件/4 模式，再逐条扫描，才计入上述结论。

新入口体扫描只取该函数定义，不把同模块正常写函数混入只读核验范围：

```powershell
$featureText = Get-Content -LiteralPath '海中鱼巣/业务/应用服务.特征概念类.ixx' -Encoding UTF8 -Raw
$featureStart = $featureText.IndexOf('特征概念处理结果 特征概念应用服务::读取已发布类型观察(')
$featureStop = $featureText.IndexOf('inline 概念树应用读取结果<std::vector<特征概念事实>>', $featureStart)
$featureReadBody = $featureText.Substring($featureStart,$featureStop-$featureStart)
$featureReadBody | rg -n 'pending_input_|pending_|pending_feature_|confirmed_|写头|写\(|推进\(|消费待确认写入|处理类型观察|收敛类型观察|待原请求确认'
$featureReadBody | rg -n '查询准确特征|读取类型观察|读取特征当前名称|读取应用特征模板|判定应用特征模板|守卫\(g\)'
rg -n '处理类型观察|收敛类型观察|读取已发布类型观察' '海中鱼巣/业务/应用服务.场景成员概念类.ixx'
rg -n '概念原请求|收敛特征概念写入|特征类数据服务|读取类型观察|读取应用特征模板' '海中鱼巣/业务/应用服务.场景成员概念类.ixx'
rg -n '共享特征应用预算|共享场景限制|共享特征业务状态|共享特征阶段状态|特征类结点|特征引用读取状态|查询特征\(' 海中鱼巣 -g '*.ixx' -g '*.cpp' -g '*.h'
rg -n 'struct 存在概念应用预算|struct 存在概念场景限制|enum class 存在概念业务状态|enum class 存在概念阶段状态' 海中鱼巣 -g '*.ixx'
rg -n '不支持=15|旧格式不支持=16|规则缺失=17|类型不相容=18|前次写入待收敛=19|case D::(不支持|旧格式不支持|规则缺失|类型不相容|前次写入待收敛)' '海中鱼巣/业务/应用服务.动态概念树类.ixx'
```

依次结果：禁止读写成员零命中/1；五项只读调用与前后守卫命中/0；场景首次/特征续行各一处，E/最终只读各一处/0；场景非法依赖/本地替代恢复零命中/1；旧 ABI 零命中/1；EC 四个定义各只在 EC 模块一处/0；DC 15..19 和五个显式 case 命中/0。排除范围与 §7.3 相同：旧文档、根目录异主副本不作生产扫描；合法 L1 G/H、原 v1 审计与普通业务槽不退役。

静态分支核对：准确查询解析外部材料完整 I64，持久观察固定 F 后只计该身份的恰一次命中，不使用输入 raw variant 相等；名称缺失保留 O/F，生命周期/FT/名称端点/完整判定回执和域适用逐项复核；失败没有待确认原请求，不伪造写完成。这里是源码证据，未构造运行测试输入。

### 9.3 文本与完成边界

两源码 `git diff --check` exit 0；严格规范检查 exit 0、101 项通过。两份记录追加后仍须随精确四文件进行发布前检查。未运行程序、数据库、测试、恢复、故障注入或集成验收；本段只证明静态实现与同候选两配置编译链接。未取得新的发布租约，不暂存或提交；4 项登记与日志保留，异主 WIP 未改。

## 10. v0.6 全返回路径守卫后的最终候选

§9 候选随后被独立审计指出异常捕获后的返回缺少 G 守卫，不能作为最终合同完成依据。新只读入口现已将错误映射提为局部 lambda，在唯一返回之前对全部非零 G 统一守卫；尾守卫失败清空局部结果并保留真实原因，零 G 保持入口拒绝。独立复核 PASS，不涉及运行验证。

新隔离根 `D:/TEMP/海中鱼巣/FEATURE-CONCEPT-MODEL-CUTOVER/compile-20260908-05/` 创建前确认不存在。仍使用 §9.1 同一个精确 `789a1859646f0e769d883ea8f11b3df5df48c94a` archive，加 Fapp/场景两个精确覆盖文件；最终命令为：

```powershell
$featureCompileRoot = 'D:/TEMP/海中鱼巣/FEATURE-CONCEPT-MODEL-CUTOVER/compile-20260908-05'
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/amd64/MSBuild.exe' "$featureCompileRoot/source/海中鱼巣.vcxproj" /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m:1 /nologo /v:minimal "/p:OutDir=$featureCompileRoot/out/Debug/" "/p:IntDir=$featureCompileRoot/obj/Debug/" /fl "/flp:logfile=$featureCompileRoot/logs/Debug-final.log;encoding=UTF-8;verbosity=normal"
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/amd64/MSBuild.exe' "$featureCompileRoot/source/海中鱼巣.vcxproj" /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m:1 /nologo /v:minimal "/p:OutDir=$featureCompileRoot/out/Release/" "/p:IntDir=$featureCompileRoot/obj/Release/" /fl "/flp:logfile=$featureCompileRoot/logs/Release-final.log;encoding=UTF-8;verbosity=normal"
```

| 日志（本根 logs） | exit | 警告/错误 | 用时 |
| --- | --- | --- | --- |
| Debug-final.log | 0 | 0/0 | 30.97 秒 |
| Release-final.log | 0 | 0/0 | 26.76 秒 |

两配置同一 source 字节、14 个计划源码实际编译并链接。Fapp 最终 blob `f95bebf99c3334828d43f5621462c9c5db7bc372`，SHA256 `7BB1C795C50C7D537FFD0E2E819AA2B31DADA711CC986CF9DB975C1FD836CBEA`；场景 blob `bff29970c61a0d769c491eeaea43450760df7e6c`、SHA256 `0304DB65031D90B34928B03040C4F75DC07020BDEBD272D528765CA2C640514A`。两源码工作区/source 字节与 blob 逐项一致；16 输入与精确正式基底相比只有上述两项变化，其余 14 项保持正式 blob。

重跑 §9.2 函数体禁止引用扫描和 §7.3 原四组旧符号/14 路径扫描，均无命中、exit 1；函数体额外执行 `rg -n 'if\(g\)try|out=特征概念处理结果|return out|守卫\(g\)|记录失败'`，exit 0，确认异常处理之后的唯一返回前守卫及四种尾失败清空分支。没有待确认载荷、pending 或写入口引用。两源码 diff check exit 0，严格规范检查 exit 0、101 项通过。

本轮未执行程序、测试、数据库、恢复、故障注入或集成验收。记录保留之前失败与中间结果；最终字节只采用本节证据，不复用旧候选。代码和记录尚待唯一 Git 发布租约、四文件精确暂存及发布前复核；四项登记保留，异主 WIP 未动。
