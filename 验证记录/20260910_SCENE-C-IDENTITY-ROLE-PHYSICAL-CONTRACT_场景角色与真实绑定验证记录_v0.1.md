# SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT 场景角色与真实绑定验证记录

日期：2026-09-11

计划：`SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT` v0.8

输入HEAD：`6d5e35055ef11164a1df9757becb3cd5ab78ff32`

验证根：`D:\TEMP\海中鱼巣\SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT\run-20260911-v2`

工具：MSBuild `18.5.4.18101`

## 1. 精确输入

隔离副本由正式HEAD归档后仅覆盖本计划四个生产文件；4203及其它依赖取正式基线。最终覆盖SHA256：

| 文件 | SHA256 |
| --- | --- |
| `海中鱼巣/领域/合同.场景角色组织.ixx` | `359BD858C8CF9165362667A7786B8BCF10C8942C1ED733264001C90ECFBA9456` |
| `海中鱼巣/领域/数据服务.场景类.ixx` | `21614CDF7B380410678C3DC2E12E1512B42D173521ED3EFFF2148460C0AF7C13` |
| `海中鱼巣.vcxproj` | `6B735F3E59AF43518C658C89F7E9072C12BED6FCF6CC3DBF96A265EC2A55789B` |
| `海中鱼巣.vcxproj.filters` | `C506A5E9D70DEB66E553AAECAE2ECDD5B339CB0D6F599443FFC3A9D6A1493098` |

目标工程传递闭包实际包含L1合同/仓库/服务、特征值类、特征类、I64比较算法、存在只读合同/存在类、状态类、4203 B合同、4213动态合同、动态类、新场景合同和场景类。临时探针、工程、中间件、EXE和持久材料均只在上述验证根或同计划`D:\TEMP`目录，未进入仓库。

## 2. 构建与运行结果

实际八条构建命令完整列于§5.2。

| 项目/运行 | Debug x64 | Release x64 | 结论 |
| --- | --- | --- | --- |
| `compile-check/scene_compile.vcxproj` | Rebuild exit 0 | Rebuild exit 0 | 目标模块编译链接通过 |
| `probe/scene_probe.vcxproj` | Rebuild exit 0；Run exit 0；`SCENE_PROBE_OK` | 同左 | C/B/S组织、读取、退出和幂等公开路径通过 |
| `probe/dynamic_split_probe.vcxproj` | Rebuild exit 0；Run exit 0；`P01_RESTORE_AND_DYNAMIC_PROBE_OK` | 同左 | 真实原子/组合D、两层组织和同进程持久恢复通过 |
| `probe-cstage/cstage_probe.vcxproj` | Rebuild exit 0；Run exit 0；498 PASS/0 FAIL | 同左 | 合法失败面、映射、零写和结构破坏矩阵通过 |

动态探针在独立持久根分别执行。它实际证明：原子D组织成功；来源B.C属于E1的真实组合D可以直接组织到E2的动态根；两层路径可读；B仍受D引用保护；服务销毁后由公开L1持久恢复入口重建并读回C、B、动态路径及原C/B幂等键。该跨场景成功只证明4204 v1.1的场景服务不解释D来源，不证明E2是D的实际发生场景。

C阶段探针SHA256为`EBE08D8C9BBC9E0502F2262AAB2F2B3FB5DF28C108A64E72CDFF7726480009E8`。其真实动态provider转发计数证明`组织动态实例`成功路径调用`读取当前动态`恰好1次、调用`展开动态来源`0次；旧`G01-R-D-expand-map`断言已删除。

## 3. A/B/C矩阵分账

### A阶段

- `G01-M`：机械检查生产映射switch覆盖现行E窄、S、D当前读取和L1写结果枚举，均保留终端`default -> 内部不一致`；未知底层值不做非法运行注入。
- 工程XML可解析；新合同在vcxproj和filters中均恰好一项；无include伪装。
- 静态禁止项扫描：场景类无`未实现创建结果`、`未实现读取结果`、`未实现组查询结果`，无运行时未实现回退、无`展开动态来源`及逐原子B.C/请求C比较；七B签名与`绑定于`仍在。
- 锁序扫描：`write_`只在`串行提交`中取得，临界区内仅调用L1原子提交；外部provider、首次材料、前置读取和发布后读回均无服务锁。

### B阶段

`RUN_PASS`：C01、B01、O01-S、O02、R01、I01、X01、P01。真实E/S/D/L1均来自正式生产服务；原子D和组合D均经真实当前读取进入场景纯结构组织。C/B历史、路径、退出、引用保护、同键重放和同进程持久恢复均实际命中。

### C阶段

`RUN_PASS`：C02、B02、O01-F、F01、G01-R；Debug/Release均498 PASS/0 FAIL。可由公开provider替身表达的E/D失败状态、公开合法L1形成的代次漂移、预算、引用、幂等、恢复及结构损坏路径均实际运行并检查零写。

`UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M`：需要非法未知枚举注入、具体L1/写端口私有状态篡改或测试后门才能形成的来源状态。对应证据为A阶段生产switch机械穷尽及C阶段真实公开可达状态清单；未修改L1或增加注入缝。

## 4. 静态与仓库检查

- `git diff --check -- 海中鱼巣/领域/合同.场景角色组织.ixx 海中鱼巣/领域/数据服务.场景类.ixx 海中鱼巣.vcxproj 海中鱼巣.vcxproj.filters`：exit 0。
- `python .\tools\check_specs.py --strict`：验证复跑时exit 1；当时唯一既有范围外错误为`tools/l1_frozen_surface_manifest.json: FROZEN 必须登记存在的 acceptance_record`。发布前再次复跑时，另有两份异主未跟踪规范`规范/2340_根规范_自然语言知识.md`、`规范/7230_子规范_语言材料知识与交互字段.md`尚未登记目录，合计3个范围外错误。本片未修改manifest、规范目录或异主文件，不能将strict记为通过。
- 工程登记计数：vcxproj=1，filters=1。
- 根工程全量Rebuild：`NOT_RUN`。计划已确认世界树/概念树存在范围外悬空旧消费者；本片不通过恢复旧API、删除消费者或移除工程项来掩盖它们。

## 5. 本轮证据闭合（2026-09-11，结果提交后复跑）

本节只补录可从正式提交与仓库外探针重新取得的事实。复跑基线为
`HEAD == origin/main == 6ddbaf38473aaa9ae8b3aba9b9b5fb04705ffcec`，index为空。
验证副本中的17个生产编译单元及5个直接包含的数据头均以
对§5.1列出的每个生产相对路径，实际执行`git hash-object --path=该相对路径 对应副本文件`
与`git rev-parse HEAD:该相对路径`并逐项比较，22项全部相等；
这项比较使用Git清洁过滤后的blob身份，避免把Windows CRLF工作树字节差异误报成源码漂移。

### 5.1 完整输入源码与工程哈希

下表路径均为实际绝对路径，哈希算法为SHA256。生产文件是最终结果提交的字节一致外置副本；
探针文件只存在于验证根。动态恢复探针Debug与Release必须使用不同的新持久根，因此
`dynamic_main.cpp`的Debug输入为`persist-run-20260911-v15`、SHA256
`9CC1D03EB1D94D3B29B7233F3C0E3B11C3405AFD5C229CEF99D02E79AADECACF`；
Release输入为`persist-run-20260911-v16`、SHA256
`1A7FF61B0F01D4E2440C9549CBC287F0E6CD73ED7F9E583B207FD8B90BDA4805`。

| 实际输入路径 | SHA256 |
| --- | --- |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/核心/L1公共事实.数据.h` | `493531C9AE313B23B73D28F04F27F9885F3E7FFF647BD8C4A8C76A11A0C2B018` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/核心/L1中性CRUD.数据.h` | `BF394DC1BFF90CFD7EB23DE9FFD00810359B88DD9E69F1031267BEE1F9A3092E` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/核心/L1所有者范围CRUD.数据.h` | `87D021D707E8D6F2A98705827D05B956F43CFA75548D59FD3B880D770444704E` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/核心/L1事实基座.数据.h` | `161BD43341D3C320902E361FD76918FF7E24A030D02B253BBDC608D5886FA4FD` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/核心/L1事实基座持久恢复.数据.h` | `1573DE06243C7FE63DE885302688E3754B90A72C06FDC0F2DC8F3418EAE0B478` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/核心/合同.L1公共事实.ixx` | `59EEDD7061420A3A8ECF2744D8C8F2951B71B1DA36108FBA087E801384FC88BC` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/核心/合同.L1事实基座.ixx` | `E30BBB5D938F21CCC8C3975367AC0E3758E12DE9D63FD7698AC27AA1EB8C4371` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/核心/合同.L1中性CRUD.ixx` | `BDF45ADF835442CDD1A1CB0B56E100EBD431A9B0F8A4BE60E82649BE52237B84` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/核心/合同.L1所有者范围CRUD.ixx` | `C743F9ACF6A65E2F6ABB3A2A47BEFE96B950E9C2FBE8DC272C949E2B049044A4` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/核心/仓库.L1事实基座.ixx` | `F61C9ADC728671FF0F77872D309496ECE88D4A609E829A7D1B0EDB23D569E98E` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/核心/服务.L1事实基座.ixx` | `EC8F42BE063882A4B4C5BB8C69A3C441C33E9865A4B2605C78EAC82330809E6F` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/领域/数据服务.特征值类.ixx` | `254A88B6584A1BD65A8F7A262102B3D65F23E3A1662836CDB94D902E1036CCDB` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/领域/数据服务.特征类.ixx` | `EB5266A5CB4E7C20B8D781D59916E7314FDADEF80F7E9B94955FCBE1A53AB534` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/领域/算法.有序I64特征比较.ixx` | `BB3FA47ACE270C71D1AD007C821E37C55D8ABA62442251EDDD7C6A490E4FA293` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/领域/合同.存在结构身份只读.ixx` | `60070C509819D4E4B2DA65FA84D4EF50461305AEA2D1ADE194FDD6BAC781975D` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/领域/数据服务.存在类.ixx` | `455FF4A80D3BF6529013235C08A6FE2D7A30D49FF83E346873793FD0B9394765` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/领域/数据服务.状态类.ixx` | `8E44B288FACC7EA14398CDF0F609D0A9041387D1052CAB1197F33395AA5786FF` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/领域/合同.场景状态使用绑定.ixx` | `366278EE1A7EC881B0B1B65D456D2F8B8B38B545966BA146644183FD4FB44AE4` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/领域/合同.动态结构.ixx` | `7DF8F499BEBD67E66EC09567F19BB6712F88140B6228A8A4ECCE513346D12ED2` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/领域/数据服务.动态类.ixx` | `B9D60FD3B094BD241A7DF4DA4336821622A763CCA7901EC5F91FC988CDF625D9` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/领域/合同.场景角色组织.ixx` | `359BD858C8CF9165362667A7786B8BCF10C8942C1ED733264001C90ECFBA9456` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/海中鱼巣/领域/数据服务.场景类.ixx` | `21614CDF7B380410678C3DC2E12E1512B42D173521ED3EFFF2148460C0AF7C13` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/compile-check/scene_compile.cpp` | `D1205632D530CC85B4C5F81EFDA30873C9B89C1BA75FB4148D7557D6A1A0DF18` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe/scene_probe.cpp` | `467CA1D18DA5C950E88DD449DDEDCAA1B55D772DF35DF37BCB54FE96F421C16B` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe/dynamic_setup.cpp` | `3337DB7CFCEBB5F5ED8621158C84119D5FE5FAAC1C6B26889D641E84E92F6106` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe/dynamic_run.cpp` | `F875DA26716D86141EA791B4E306ADD0724C315E26BBE0084FDC26E0B09199CF` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe/dynamic_scene_call.cpp` | `D2FFD253FB30ED591271AB1348F506100D0F74133C043F68BFB76993219332FA` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe/dynamic_main.cpp`（Debug/v15） | `9CC1D03EB1D94D3B29B7233F3C0E3B11C3405AFD5C229CEF99D02E79AADECACF` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe/dynamic_main.cpp`（Release/v16） | `1A7FF61B0F01D4E2440C9549CBC287F0E6CD73ED7F9E583B207FD8B90BDA4805` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe-cstage/cstage_probe.cpp` | `EBE08D8C9BBC9E0502F2262AAB2F2B3FB5DF28C108A64E72CDFF7726480009E8` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/compile-check/scene_compile.vcxproj` | `8BAEFCF7851B75E354B32BAD21066845CFA950E724AA55777B855D2DA73A3B8A` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe/scene_probe.vcxproj` | `7281A7DB5CE51592832AF014551CA91A3D3FC0D82C79F5BC6D452856B7326508` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe/dynamic_split_probe.vcxproj` | `6971EC2B9534C8BAAEEAE7BC2F969B03D0228EEFCB126A621106CB36444DB459` |
| `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe-cstage/cstage_probe.vcxproj` | `36C74D164025FFF276D1502DB665854401FB274615C4218EBC5AE8C7D57773AE` |

工程依赖是上述共同生产闭包；`compile-check`不编译动态类，`scene_probe`和`cstage_probe`增加动态类，
`dynamic_split_probe`再增加`dynamic_setup.cpp`、`dynamic_run.cpp`、`dynamic_scene_call.cpp`和`dynamic_main.cpp`。
四份vcxproj均直接列出其完整ClCompile清单，没有从根工程删项或以替身成功替代E/S/D生产成功链。

### 5.2 实际构建命令、产物与运行输出

MSBuild绝对路径与版本：
`C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe`，`18.5.4.18101`。
实际执行的八条命令如下：

```powershell
& 'C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe' 'D:\TEMP\海中鱼巣\SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT\run-20260911-v2\compile-check\scene_compile.vcxproj' /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m /nologo /v:minimal
& 'C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe' 'D:\TEMP\海中鱼巣\SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT\run-20260911-v2\compile-check\scene_compile.vcxproj' /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m /nologo /v:minimal
& 'C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe' 'D:\TEMP\海中鱼巣\SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT\run-20260911-v2\probe\scene_probe.vcxproj' /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m /nologo /v:minimal
& 'C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe' 'D:\TEMP\海中鱼巣\SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT\run-20260911-v2\probe\scene_probe.vcxproj' /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m /nologo /v:minimal
& 'C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe' 'D:\TEMP\海中鱼巣\SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT\run-20260911-v2\probe\dynamic_split_probe.vcxproj' /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m /nologo /v:minimal
& 'C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe' 'D:\TEMP\海中鱼巣\SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT\run-20260911-v2\probe\dynamic_split_probe.vcxproj' /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m /nologo /v:minimal
& 'C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe' 'D:\TEMP\海中鱼巣\SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT\run-20260911-v2\probe-cstage\cstage_probe.vcxproj' /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m /nologo /v:minimal
& 'C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe' 'D:\TEMP\海中鱼巣\SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT\run-20260911-v2\probe-cstage\cstage_probe.vcxproj' /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m /nologo /v:minimal
```

八次Rebuild均返回0，MSBuild末行分别指向下表EXE。每个IntDir中的IFC/OBJ数量、场景模块产物、探针OBJ和EXE均在复跑后实际枚举：

| 工程/配置 | IFC/OBJ | 关键IFC、OBJ目录 | EXE及SHA256 | 构建/运行与输出 |
| --- | --- | --- | --- | --- |
| compile/Debug | 16/17 | `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/compile-check/obj/Debug/数据服务.场景类.ixx.ifc`、同目录`数据服务.场景类.ixx.obj`、`scene_compile.obj` | `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/compile-check/bin/Debug/scene_compile.exe`；`B4D119B730F977BF77AFCBCA175A3B87DA2C2F0BB8C84DE8ED4F2AE0EFAD0BCE` | build 0；run 0；stdout/stderr 0行 |
| compile/Release | 16/17 | `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/compile-check/obj/Release/数据服务.场景类.ixx.ifc`、同目录`数据服务.场景类.ixx.obj`、`scene_compile.obj` | `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/compile-check/bin/Release/scene_compile.exe`；`D9342550BE32A5D01BE36E6175CCC8500F70F9618FF51250A9DAF5326183CA77` | build 0；run 0；stdout/stderr 0行 |
| scene/Debug | 17/18 | `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe/obj/Debug/数据服务.场景类.ixx.ifc`、同目录`数据服务.场景类.ixx.obj`、`scene_probe.obj` | `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe/bin/Debug/scene_probe.exe`；`369080507AA18498DB0D23BBAE677EF65A8F8EAF41C565DC4DBD23D185A67136` | build 0；run 0；89 PASS/0 FAIL；`SCENE_PROBE_OK` |
| scene/Release | 17/18 | `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe/obj/Release/数据服务.场景类.ixx.ifc`、同目录`数据服务.场景类.ixx.obj`、`scene_probe.obj` | `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe/bin/Release/scene_probe.exe`；`0B184B12543640FADF71F4F9B6646A8905681F4F9E8F4DA12BF18636B5E55C96` | build 0；run 0；89 PASS/0 FAIL；`SCENE_PROBE_OK` |
| dynamic/Debug | 17/21 | `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe/obj-dynamic-split/Debug/数据服务.场景类.ixx.ifc`、同目录`数据服务.场景类.ixx.obj`、`dynamic_main.obj` | `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe/bin-dynamic-split/Debug/dynamic_split_probe.exe`；`3BC5423C96CCCF19310E8A2D9650D6E96677D2972ED630D0969785EDA236589A` | build 0；run 0；92 PASS/0 FAIL；`P01_RESTORE_AND_DYNAMIC_PROBE_OK`；持久根v15 |
| dynamic/Release | 17/21 | `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe/obj-dynamic-split/Release/数据服务.场景类.ixx.ifc`、同目录`数据服务.场景类.ixx.obj`、`dynamic_main.obj` | `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe/bin-dynamic-split/Release/dynamic_split_probe.exe`；`CF16721AB4FA2323C9D09797D0A5B7B64A03D0833298A2459F2619235C714ADA` | build 0；run 0；92 PASS/0 FAIL；`P01_RESTORE_AND_DYNAMIC_PROBE_OK`；持久根v16 |
| cstage/Debug | 17/18 | `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe-cstage/obj-cstage/Debug/数据服务.场景类.ixx.ifc`、同目录`数据服务.场景类.ixx.obj`、`cstage_probe.obj` | `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe-cstage/bin-cstage/Debug/cstage_probe.exe`；`DB825D546D6F8BA378D860038A5044953CE7D91AE796779A1757AFED2CC36520` | build 0；run 0；498 PASS/0 FAIL；`CSTAGE_PROBE_OK` |
| cstage/Release | 17/18 | `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe-cstage/obj-cstage/Release/数据服务.场景类.ixx.ifc`、同目录`数据服务.场景类.ixx.obj`、`cstage_probe.obj` | `D:/TEMP/海中鱼巣/SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT/run-20260911-v2/probe-cstage/bin-cstage/Release/cstage_probe.exe`；`0CC2BEBB702B4BD99BC28EA2ACDA834FB33B44E0145D6EF82A72CA7B0F6785D4` | build 0；run 0；498 PASS/0 FAIL；`CSTAGE_PROBE_OK` |

动态探针首次直接复用已存在的v12持久根时，Debug和Release均真实返回1，
输出止于`FAIL comparison-binding-create`及`dynamic probe aborted: probe failure`；该次失败证明固定旧根不能作为重复首轮输入，
不计入完成证据。确认v15/v16事前不存在后，分别重建对应配置并取得上表通过结果；未删除v12或任何其它历史验证材料。

### 5.3 G01-M逐项映射与证据位置

机械输入枚举位置：E为`合同.存在结构身份只读.ixx:13-22`，S为`数据服务.状态类.ixx:43-50`，
D为`合同.动态结构.ixx:91-100`，L1写为`L1所有者范围CRUD.数据.h:197-200`，
L1历史/首次读取为同文件`:567-570`，一致当前读取及项目状态为`:816-823`，L1中性读取为
`L1中性CRUD.数据.h:204-207`。映射实现位置均指验证副本中的
`海中鱼巣/领域/数据服务.场景类.ixx`。

本轮以PowerShell正则从上述精确输入的`enum class ... { ... };`实体机械提取成员，去重并与应有数量核对，
实际输出为`E=9`、`S=19`、`D=29`、`L1W=11`、`L1R=10`、`L1C=6`、`L1CI=6`、`L1N=9`，
随后对生产文件机械检查`状态到场景`、`状态到B`、`动态到场景`、`存在到场景`、
`映射场景写入状态`、`映射绑定写入状态`、`写入失败头完整`及两个终端default标记；
命令返回`G01M_STATIC_MARKERS|PASS`和`G01M_ENUM_EXTRACTION|PASS`。这两个PASS只证明枚举提取和生产映射标记存在；
以下表格再按载荷、写入头和发布阶段列出分流。它们不能替代合法并发可达分支的实际运行证据。

E映射位置为`:148`与调用点`:403`、`:568`：

| E来源状态 | 场景/B目标 | 运行分账 |
| --- | --- | --- |
| 已读取 | `成功(G0)`且载荷一致则继续；成功载荷矛盾则内部不一致 | RUN_PASS：真实E链`scene_probe.cpp:180`；矛盾载荷`cstage_probe.cpp:423-424` |
| 入口拒绝 | 入口拒绝 | RUN_PASS：`cstage_probe.cpp:412-422` |
| 未找到 | 未找到/存在未找到 | RUN_PASS：同上 |
| 目标已退出 | 目标已退出/存在已退出 | RUN_PASS：同上 |
| 事实代次漂移 | 事实代次漂移 | RUN_PASS：同上 |
| 历史材料已清理 | 历史材料已清理 | RUN_PASS：同上 |
| 资源失败 | 资源失败 | RUN_PASS：同上 |
| 内部不一致 | 内部不一致（default） | RUN_PASS：同上 |
| 未实现 | 内部不一致（default） | RUN_PASS：同上 |

S映射位置为`:146`（组织）和`:605`（B）：

| S来源状态 | 场景/B目标 | 运行分账 |
| --- | --- | --- |
| 已创建 | 内部不一致（读取入口不会合法返回写结果） | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M `:146/:605` |
| 精确重复 | 内部不一致 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M `:146/:605` |
| 已读取 | 状态结果头和内容与请求一致则继续；成功但内容缺失或身份/截止代次矛盾则内部不一致 | RUN_PASS：真实S，`scene_probe.cpp:191-203`；生产核验`:361,462,519` |
| 已退出 | 内部不一致（写结果，不等于目标已退出） | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M `:146/:605` |
| 已清理 | 内部不一致（写结果） | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M `:146/:605` |
| 入口拒绝 | 入口拒绝 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：场景固定合法读取请求 |
| 未找到 | 未找到/状态未找到 | RUN_PASS：`cstage_probe.cpp:342` |
| 目标已退出 | 目标已退出/状态已退出 | RUN_PASS：`cstage_probe.cpp:343` |
| 正式特征类型未找到 | 引用冲突 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M `:146/:605` |
| 正式特征类型已退出 | 引用冲突 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M `:146/:605` |
| 准确值不相容 | 入口拒绝 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M `:146/:605` |
| 旧格式不支持 | 入口拒绝 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M `:146/:605` |
| 事实代次漂移 | 事实代次漂移 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：本轮公开旧G0在场景/L1守卫处先收口，未把它误记为S provider返回值 |
| 幂等冲突 | 内部不一致 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：读取入口不会返回写幂等结果 |
| 引用冲突 | 内部不一致 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M `:146/:605` |
| 数量预算不足 | 数量预算不足 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：当前单项状态读取无外部数量预算 |
| 历史材料已清理 | 历史材料已清理 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：本计划不执行状态物理清理 |
| 资源失败 | 资源失败 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：无生产故障注入入口 |
| 内部不一致 | 内部不一致（default） | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：无私有状态篡改/测试后门 |

D映射位置为`:147`，调用点`:375`；除`已读取`使用真实动态外，其余28个现行成员逐项由
`cstage_probe.cpp:427-457`的公开只读provider失败替身输入并检查零写：

| D来源状态 | 场景目标 | 运行分账 |
| --- | --- | --- |
| 已创建 | 内部不一致 | RUN_PASS |
| 精确重复 | 内部不一致 | RUN_PASS |
| 已读取 | 当前动态结果与请求一致则继续；成功载荷矛盾则内部不一致 | RUN_PASS：真实D为`cstage_probe.cpp:331-336`及动态探针；矛盾载荷`cstage_probe.cpp:458-459` |
| 已退出 | 内部不一致 | RUN_PASS |
| 已清理 | 内部不一致 | RUN_PASS |
| 入口拒绝 | 入口拒绝 | RUN_PASS |
| 未找到 | 未找到 | RUN_PASS |
| 目标已退出 | 目标已退出 | RUN_PASS |
| 来源未找到 | 引用冲突 | RUN_PASS |
| 来源已退出 | 引用冲突 | RUN_PASS |
| 历史材料已清理 | 历史材料已清理 | RUN_PASS |
| 绑定不完整 | 内部不一致 | RUN_PASS |
| 主体不一致 | 内部不一致 | RUN_PASS |
| 正式特征类型不一致 | 内部不一致 | RUN_PASS |
| 时间不递增 | 内部不一致 | RUN_PASS |
| 无变化 | 内部不一致 | RUN_PASS |
| 比较依据不支持 | 内部不一致 | RUN_PASS |
| 组成发生期依据不足 | 内部不一致 | RUN_PASS |
| 重复成员 | 内部不一致 | RUN_PASS |
| 来源成环 | 内部不一致 | RUN_PASS |
| 数量预算不足 | 数量预算不足 | RUN_PASS |
| 事实代次漂移 | 事实代次漂移 | RUN_PASS |
| 幂等冲突 | 内部不一致 | RUN_PASS |
| 引用冲突 | 引用冲突 | RUN_PASS |
| 旧格式不支持 | 内部不一致 | RUN_PASS |
| 依赖未实现 | 内部不一致 | RUN_PASS |
| 资源失败 | 资源失败 | RUN_PASS |
| 内部不一致 | 内部不一致（default） | RUN_PASS |
| 已可能发布 | 内部不一致（default） | RUN_PASS |

L1写映射位置为`:150-176`；失败头/载荷先决为`:202-218`。`是否形成内存权威发布=true`
且状态不是成功时优先收口为已可能发布；正常失败头不完整则内部不一致：

| L1写来源状态 | 场景/B目标 | 运行分账 |
| --- | --- | --- |
| 成功 | 写入头、编码映射、发布后读回和结束守卫完整时给出调用点成功态；提交已开始后任一头/映射/读回/结束守卫矛盾均收口为已可能发布 | RUN_PASS：全部真实首发路径；G01-M：生产`:61-78,326-328,347-349,396-411,438-440,455-498` |
| 精确重复 | 原幂等身份、首次写集/首次结果、编码映射、发布后读回和结束守卫一致时为精确重复；已有原结果或提交已开始后任一互证失败均收口为已可能发布 | RUN_PASS：`scene_probe.cpp:183,204`及恢复原键；G01-M：生产`:52-78`及各写后收敛位置 |
| 入口拒绝 | 内部不一致；已发布标志存在则已可能发布 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：合法场景写集在提交前已完成形状核验 |
| 许可拒绝 | 内部不一致；已发布标志存在则已可能发布 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：已持有场景owner唯一端口 |
| 未找到 | 内部不一致；已发布标志存在则已可能发布 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：引用端点均在提交前真实读回 |
| 已退出 | 内部不一致；已发布标志存在则已可能发布 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：退出端点均在提交前真实读回 |
| 事实代次漂移 | 失败头完整时事实代次漂移；若错误声称已发布则已可能发布；失败头矛盾则内部不一致 | 未运行覆盖（计划证据缺口）：合法并发可在L1提交窗口形成；本轮O01-F只观察场景最终漂移，不能证明来源为L1写返回。G01-M仅证明生产`:151-158,202-218`映射存在 |
| 幂等冲突 | 失败头完整时幂等冲突；若错误声称已发布则已可能发布；失败头矛盾则内部不一致 | 未运行覆盖（计划证据缺口）：合法同键异义竞态可在L1提交形成；本轮未制造该提交瞬时竞争。G01-M仅证明生产`:151-158,202-218`映射存在 |
| 引用冲突 | 引用冲突 | RUN_PASS：B/D保护及有组织内容的C退出保护 |
| 资源失败 | 资源失败 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：无生产故障注入入口 |
| 内部不一致 | 内部不一致 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：无L1私有状态篡改/测试后门 |

L1读取并非集中switch，而是按用途在`:50-78`、`:182-192`、`:322-332`、`:391-401`、
`:450-460`、`:478-488`、`:556-564`逐项收口：

| L1读取来源 | 逐项映射 | 运行分账 |
| --- | --- | --- |
| `L1所有者范围读取状态::成功` | 校验合同、owner、编码、G/H、载荷后继续 | RUN_PASS：当前/历史/首次材料/恢复 |
| `::入口拒绝` | 内部不一致 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：固定合法版本和编码形状 |
| `::许可拒绝` | 内部不一致 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：owner内读取接口 |
| `::未找到` | 首次材料分流为新写；节点/关系为未找到 | RUN_PASS：首发与缺失对象 |
| `::已退出` | 内部不一致；生命周期由成功载荷的退出代次表达 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M |
| `::属性未设置` | 内部不一致 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：这里不调用属性读取 |
| `::事实代次漂移` | 载荷G不等请求G时映射事实代次漂移；直接枚举值否则内部不一致 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：公开旧G0由一致当前读取先收口，未把它误记为该枚举实际返回 |
| `::资源失败` | 资源失败 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：无生产故障注入入口 |
| `::内部不一致` | 内部不一致 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M |
| `::历史材料已清理` | 历史节点/关系映射历史材料已清理；首次原键读取不接受该状态 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：本计划不做物理清理 |
| `L1中性读取状态::成功` | 当前代次继续 | RUN_PASS：每项探针的`L1-current-generation` |
| `::资源失败` | 资源失败 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：无故障注入 |
| `::入口拒绝/未找到/已退出/属性未设置/内部不一致/许可拒绝/历史材料已清理` | 内部不一致 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：固定合法的当前代次读取不产生这些状态 |
| `L1所有者范围一致当前读取状态::成功` | 校验唯一owner项目后继续 | RUN_PASS：正常读回与路径读取 |
| `::事实代次漂移` | 事实代次漂移 | RUN_PASS：旧G0调用 |
| `::资源失败` | 资源失败 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：无故障注入 |
| `::入口拒绝/许可拒绝/内部不一致` | 内部不一致 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M |
| `L1所有者范围一致当前读取项目状态::成功` | 校验owner事实后继续 | RUN_PASS |
| `::未找到/已退出/属性未设置/种类不匹配/历史材料已清理` | 内部不一致 | UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M：场景只选择自身owner且不选择属性项目 |

所有上述枚举之外的非法底层数值统一由各映射末端`default -> 内部不一致`或
“非成功且非明确可映射状态→内部不一致”静态覆盖；它们标记为
`UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M`，没有通过构造非法enum、重解释内存或增加测试后门取得运行证据。

### 5.4 A/B/C运行矩阵的源码位置与结果

| 组 | 分账 | 实际源码证据位置 |
| --- | --- | --- |
| C01 | RUN_PASS | `probe/scene_probe.cpp:180,186-187` |
| B01 | RUN_PASS | `probe/scene_probe.cpp:191-198` |
| O01-S | RUN_PASS | `probe/scene_probe.cpp:203-208` |
| O02 | RUN_PASS | `probe/dynamic_scene_call.cpp:14,17`；真实原子/组合D |
| R01 | RUN_PASS | `probe/scene_probe.cpp:187,198-199,208`及恢复冷索引 |
| I01 | RUN_PASS | `probe/scene_probe.cpp:183,204`、`probe/dynamic_setup.cpp:113-114` |
| X01 | RUN_PASS | `probe/scene_probe.cpp:214-216` |
| P01 | RUN_PASS | `probe/dynamic_setup.cpp:113-114`、`probe/dynamic_main.cpp:7` |
| C02 | RUN_PASS | `probe-cstage/cstage_probe.cpp:285-296` |
| B02 | RUN_PASS | `probe-cstage/cstage_probe.cpp:303-337` |
| O01-F | RUN_PASS | `probe-cstage/cstage_probe.cpp:347-388` |
| F01 | RUN_PASS | `probe-cstage/cstage_probe.cpp:390-424,458-464` |
| G01-R | RUN_PASS | `probe-cstage/cstage_probe.cpp:285-296,335-344,412-468` |
| G01-M | PASS（静态枚举与条件分流） | 本记录§5.3；枚举定义、生产映射、载荷/发布阶段分流和default位置均绑定§5.1精确输入hash；不替代两项合法并发运行缺口 |

本轮没有把`UNREACHABLE_BY_PUBLIC_CONTRACT`写成运行通过，也没有用占位模板代替来源状态分账。
L1写入的`事实代次漂移`与`幂等冲突`是合法并发可达状态，但本轮未在L1提交窗口精确注入，已明确记为计划证据缺口。
唯一实际`RUN_FAIL`是复用旧持久根v12的动态探针前置条件失败，已在§5.2保留；使用事前不存在的v15/v16后相同生产链均RUN_PASS。

## 6. 结论与未证明范围

本轮已通过目标模块Debug/Release构建、真实成功链、公开失败面、结构破坏、退出/重放和同进程L1持久恢复；G01-M静态枚举与条件分流也已闭合。由于L1写入`事实代次漂移`与`幂等冲突`虽可由合法并发形成、但本轮未精确运行命中，计划v0.8要求的G01-R仍有两项运行证据缺口，因此本记录不宣布A/B/C全部通过或计划完成。strict另有具名范围外阻断，不改变已经取得的目标模块和公开探针结果。

已经证明的范围限于：C角色、四根、C/E/S绑定、S/D组织路径、退出/重放/同进程L1持久恢复、4204 v1.1动态当前读取边界，以及本记录逐项列出的运行分支和G01-M静态分支。

未证明：普通应用或根工程闭合、数据库跨进程恢复、完整七字段CRUD、直接成员/子场景、UTC发生期包含、动态实际发生场景/来源一致性、完整观察、全场景TTL、正式集成验收、生产业务运行或业务闭环。
