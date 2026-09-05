# DATA-L2-CONCEPT-TREE-CRUD 概念树类数据操作验证记录

计划 v0.1；最终验证日期 2026-09-06。执行者 `/root/feature_concept_execute`，证据由交互所有者独立复核。

## 1. 基线、构建与结论

S0/最终代码基础为 `76a00ae3500399618a17423512d138b0f3a136b1`（main=origin/main）；设计 blob `739e91fdc0280b68a6d4cdc58edac99e6f522a52`，计划 blob `d05db3f0bc286c95a9fcc8fd0bb81f9be68dd775`。唯一代码变更为新概念树模块及两个工程登记，未改 L1、新 world、旧 L2、应用或主程序。

最终八阶段均退出 0。Debug/Release 分别在全新 `ConceptDebug`、`ConceptRelease` 目录完整编译 L1 六模块、新 world 五模块、新概念模块及消费者。两配置各通过 521 项矩阵断言（G=66）；两次独立进程 seed/recover 各通过 22/41 项断言（G=17）。Release 使用真实 require/throw 校验，不受 NDEBUG 禁用断言影响。无编译错误、FAIL 或准备工程共享 OBJ 警告。

工具为 Visual Studio 18 Professional，MSBuild `18.4.0+6e61e96ac`，MSVC `14.50.35717`，v145、x64、C++20、/utf-8、/bigobj；Debug /MDd、Release /MD 和 MaxSpeed，链接 bcrypt.lib。临时工程只含本条列出的 12 模块和独立消费者；export import 仅新存在链，未引入旧概念、旧 DTO、旧材料或旧应用。

绝对隔离根（下文 R）：`D:\TEMP\DATA-L2-CONCEPT-TREE-CRUD\海中鱼巣-01a07099-concept_tree_execute`。

实际驱动：`R\run-final.ps1`，经 PowerShell AST 解析后运行；构建工具绝对路径 `C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe`。各阶段及其结果如下，命令中的 R 展开为上述唯一绝对根。每阶段前及结束后重算冻结输入，21 项 SHA256 全部一致。

| 阶段 | 退出码 | 实际结果 | 秒 |
| --- | --- | --- | --- |
| Debug-build | 0 | 12 modules + consumer build passed | 27.555 |
| Debug-matrix | 0 | PASS 521 assertions; concept matrix G=66 | 154.984 |
| Debug-seed | 0 | PASS 22 assertions; process=seed G=17 | 2.723 |
| Debug-recover | 0 | PASS 41 assertions; process=recover G=17 | 1.784 |
| Release-build | 0 | 12 modules + consumer build passed | 43.411 |
| Release-matrix | 0 | PASS 521 assertions; concept matrix G=66 | 5.007 |
| Release-seed | 0 | PASS 22 assertions; process=seed G=17 | 2.01 |
| Release-recover | 0 | PASS 41 assertions; process=recover G=17 | 0.076 |
实际命令（各进程单独启动、前一个退出后才执行下一个）：

```powershell
& 'C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe' 'R\concept.vcxproj' /m:1 /t:Build /p:Configuration=Debug /p:Platform=x64 /v:minimal
& 'R\ConceptDebug\out\concept.exe'
& 'R\ConceptDebug\out\concept.exe' seed 'R\final-Debug-database'
& 'R\ConceptDebug\out\concept.exe' recover 'R\final-Debug-database'
& 'C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe' 'R\concept.vcxproj' /m:1 /t:Build /p:Configuration=Release /p:Platform=x64 /v:minimal
& 'R\ConceptRelease\out\concept.exe'
& 'R\ConceptRelease\out\concept.exe' seed 'R\final-Release-database'
& 'R\ConceptRelease\out\concept.exe' recover 'R\final-Release-database'
```

原始日志名逐一为 `final-{Debug|Release}-{build|matrix|seed|recover}.log`；完整命令、启动时间、耗时、退出码和末行同时由 `final-results.json` 保存，哈希列于第 4 节。seed 使用真实 `建立L1事实基座持久运行包_v1`，recover 在新的 EXE 进程重新打开同一数据库并重新签发原 owner 端口；引用和原请求传输文件只是输入，布局、概念、支持、生命周期及首次材料全部重新由真实 L1/provider 验证。没有把同进程重建伪装成跨进程恢复。

## 2. 必需矩阵及实际证据

本表均由两配置最终消费者实际运行；括号为 `concept.cpp` 中可定位的 require 名称，代表对应断言组，不以累计断言数替代覆盖判断。

| 设计用例组 | 实际结果 |
| --- | --- |
| 外部装配 | 28 类型/根/规则真实初始化交付，构造前后 G 不变；缺类型登记、额外根成员、交换类型用途、错根角色、缺根成员、重复根、混合规则身份、外部 world 根和错 L1 绑定均拒绝（external initializer、constructor zero initialization、real malformed root layout rejected）。 |
| 有类型特征 | I64 点、区间、I64/U64 整向量真实创建与读回；零宽区间复用点；反向、空向量、错 FT 表示、独立材料引用 FT 拒绝；整向量顺序不同不会合并（interval typed roundtrip、whole vector not component merged）。 |
| 完整定义查询与存在组合 | 公开查找返回完整命中或空成功；模板排序复用、重复/空模板拒绝；不同宿主产生不同 FC 身份，同域的不同模板 ID 组合不合并（public complete definition query、different template IDs remain different templates）。 |
| 来源及双向支持 | 历史宿主/特征由新 provider 回读；多对多、跨根和根支持；同 pair 零写；按概念/按 world 取得相同记录及 H；支持退出后当前消失、旧 H 仍完整（support directions same facts、support current absent history retained）。 |
| 多父图 | 多层/多父图完整读回，单边增/替/退保持其它父；自环、环、跨根、错旧边、最后根路径拒绝；真实 L1 构造孤儿与未知下位出边均返回失败且零部分图（orphan is not omitted、unknown outgoing edge is not omitted）。 |
| 输出排序 | 构造关系分配 ID 次序不等于端点次序，图按上位/下位/关系排序；先建较大 world ID 的支持、后建较小 world ID，图仍按 world/概念/记录排序（graph endpoint order differs from allocation ID order、graph support endpoint sorting differs from record ID ordering）。 |
| 生命周期 | 四条合法迁移、退役恢复活跃、同状态零写、非法迁移和旧值拒绝；根不可迁移/退出。旧值退出与新值经发布结果及历史正式读回互证（lifecycle transition、same lifecycle zero write、old lifecycle value rejected）。 |
| 概念退出 | 活跃、有支持、被模板引用前置拒绝；中间 P×C 将两父接至下位，已有绕行边不重复添加；叶完整退出；旧 H 可查、同定义再建新 ID；旧创建 key 在图/治理改变后仍回读首次结果（P x C skips existing bypass edge、irreversible exit new identity）。 |
| 未知外部引用 | 真实第二 owner 建立未知类型引用，退出返回保守可能发布；独立确认 G 不变、首次 key 未找到、叶完整。外部 owner 解除自己的边后可退出（unknown reference L1 guard conservatively possible no facts、independent first key confirms atomic rejection）。 |
| 四类形成引用释放 | 定义宿主、定义 FT、来源存在、来源特征均真实释放；不完整预期组拒绝，空组零写；释放前 world 退出被挡，释放后两个 world owner 真实退出。概念仍可读、完整定义复用、新治理写和旧创建重放成立。模板或来源成员被真实 L1 部分删除不能解释为释放（template removal is not world release、source member removal is not world release）。 |
| 幂等 | 七类写均实际原 key/原 G 重放；同 key 改 G 及同 key 同 G 改有效业务字段均拒绝并保留首次 H；服务重建和首次响应丢弃后重放保持 G 不变（same key same G changed business rejected preserves H、service reconstructed from real L1 without private authority）。 |
| 当前/历史与清理 | 未来 H、G 漂移拒绝；source world 退出后继续按形成 H；真实 L1 物理清理已退出叶的完整历史闭包后，旧 H 读取明确历史材料不可用，原创建重放返回可能发布并保留原请求/首次 H（real L1 physically cleans exited complete historical leaf、cleaned replay preserves known first H and request）。 |
| 预算 | 六字段零边界均拒绝并清空载荷；概念数、关系数、累计来源、创建后累计边、支持 world 方向及正数世界成员预算真实超限；节点及关系本地键 65536 项请求在 world 查找前拒绝，无整数溢出或部分发布（six zero budgets clear all payloads、postcreate cumulative source budget before dispatch、positive world member budget excess、relation local key count 65536 rejected before world lookup）。 |
| 跨进程 | 每配置独立 seed/recover：当前概念、历史来源、生命周期、完整图、正反支持、释放记录和创建/支持/治理/释放 key 收敛；恢复与全部重放均不推进 G=17（all restored requests converge without new facts）。 |

特征实例的合法历史投影唯一值数量为 1，最大特征属性预算的正数可用边界由真实特征来源读取覆盖，零边界拒绝已运行；不制造不合法 world 节点多值来声称正常正数超限。预算乘积溢出使用先除界限和局部键容量的静态校验，运行覆盖可达超限，未进行 U64 最大规模分配压力测试。

## 3. 已知失败、未动态覆盖及完成边界

开发期保留的失败不作为最终通过证据：初轮 C++ 保留字/默认构造/条件表达式语法已修复；两次 PowerShell 文本补丁解析失败没有执行写入；坏布局 early matrix-02 在第 3 类未实际变造时误设失败断言，修正 fixture 后最终七类变造均真正运行。开发轮共用准备 OBJ 的警告仅属早轮，最终新目录完整构建无该警告。准备 11 模块/19 断言与 smoke 21 断言不抵扣本记录最终结果。

无法稳定诱发的资源分配异常、首次材料查询资源故障、已发布后读回遭并发 G 漂移/资源失败均未做运行时注入，按静态分支分账：`执行写入` 在 keyUnknown/dispatched 时统一保守可能发布，原请求和有效首次 H 先保存；`发布结果` 逐事实校验；读取封装异常清空载荷。真实 L1 原子拒绝的派发后非成功分支已实际运行，不能将此等同于全部资源故障覆盖。

本记录证明新 DATA-L2 概念提供者在真实外部交付下的上述公开操作。未验证全仓生产工程/主程序接线；未实现全局根初始化、自动成熟/语言名称、新旧特征/存在应用迁移、动态/因果普通定义、DATA-L3 或旧链删除。未暂存、提交或推送；发布事实由唯一交互发布者后续记录。

## 4. 冻结输入及证据 SHA256

以下 21 项为 run-final 每阶段验证的同一份输入，含最终 12 个生产模块、5 个直接包含的数据头、消费者、临时工程和两个生产 XML。路径为绝对路径；TEMP 文件在父级证据复核后尝试清理但被工具策略拒绝，本记录保存其内容身份，实际残留见第 5 节。

| 输入绝对路径 | SHA256 |
| --- | --- |
| D:\海中鱼巢\海中鱼巣.vcxproj | 9271AD35E123DC524E8C84C7092D31D43398C737FA5A394B3305288C44808307 |
| D:\海中鱼巢\海中鱼巣.vcxproj.filters | FF0CB17FA2BBCA5203EC397D5EBD9FE8A573E24B1E5748022236215817A44FF1 |
| D:\海中鱼巢\海中鱼巣\核心\仓库.L1事实基座.ixx | F61C9ADC728671FF0F77872D309496ECE88D4A609E829A7D1B0EDB23D569E98E |
| D:\海中鱼巢\海中鱼巣\核心\服务.L1事实基座.ixx | EC8F42BE063882A4B4C5BB8C69A3C441C33E9865A4B2605C78EAC82330809E6F |
| D:\海中鱼巢\海中鱼巣\核心\合同.L1公共事实.ixx | 59EEDD7061420A3A8ECF2744D8C8F2951B71B1DA36108FBA087E801384FC88BC |
| D:\海中鱼巢\海中鱼巣\核心\合同.L1事实基座.ixx | E30BBB5D938F21CCC8C3975367AC0E3758E12DE9D63FD7698AC27AA1EB8C4371 |
| D:\海中鱼巢\海中鱼巣\核心\合同.L1所有者范围CRUD.ixx | C743F9ACF6A65E2F6ABB3A2A47BEFE96B950E9C2FBE8DC272C949E2B049044A4 |
| D:\海中鱼巢\海中鱼巣\核心\合同.L1中性CRUD.ixx | BDF45ADF835442CDD1A1CB0B56E100EBD431A9B0F8A4BE60E82649BE52237B84 |
| D:\海中鱼巢\海中鱼巣\核心\L1公共事实.数据.h | 493531C9AE313B23B73D28F04F27F9885F3E7FFF647BD8C4A8C76A11A0C2B018 |
| D:\海中鱼巢\海中鱼巣\核心\L1事实基座.数据.h | 161BD43341D3C320902E361FD76918FF7E24A030D02B253BBDC608D5886FA4FD |
| D:\海中鱼巢\海中鱼巣\核心\L1事实基座持久恢复.数据.h | 1573DE06243C7FE63DE885302688E3754B90A72C06FDC0F2DC8F3418EAE0B478 |
| D:\海中鱼巢\海中鱼巣\核心\L1所有者范围CRUD.数据.h | 87D021D707E8D6F2A98705827D05B956F43CFA75548D59FD3B880D770444704E |
| D:\海中鱼巢\海中鱼巣\核心\L1中性CRUD.数据.h | BF394DC1BFF90CFD7EB23DE9FFD00810359B88DD9E69F1031267BEE1F9A3092E |
| D:\海中鱼巢\海中鱼巣\领域\数据服务.存在类.ixx | FB5FB065F22BD4854CF42D8E741F66CF55D1224EDF21CF9C69F9CF101160C98C |
| D:\海中鱼巢\海中鱼巣\领域\数据服务.动态类.ixx | F774B4C8F6ABD96CFC0736CF4D5D3D6C32FB015B6B8F1733CC26FEC855B8DF71 |
| D:\海中鱼巢\海中鱼巣\领域\数据服务.概念树类.ixx | A0562511DB1550AD1E75574EA718ADAA474B30AB5EB43DD179F4836A2F3540A6 |
| D:\海中鱼巢\海中鱼巣\领域\数据服务.特征类.ixx | BB23C86D99948E3478308C98256335CCEF2F70305CBA2AC3630543C031AB68EB |
| D:\海中鱼巢\海中鱼巣\领域\数据服务.特征值类.ixx | 65A2D13E05829503A278769F9CDB1B01BDD49A00A3C99D9FC44E84CA55C487AC |
| D:\海中鱼巢\海中鱼巣\领域\数据服务.状态类.ixx | 3C4B3FEE77D3F83C395AFF6BF034B14768FBDFAE8A44D5B05F38BEC8EDA72E5E |
| D:\TEMP\DATA-L2-CONCEPT-TREE-CRUD\海中鱼巣-01a07099-concept_tree_execute\concept.cpp | 96951FCA7E42B3909A546824575A0A547D27000C099575BCD45C9323F3A1B09C |
| D:\TEMP\DATA-L2-CONCEPT-TREE-CRUD\海中鱼巣-01a07099-concept_tree_execute\concept.vcxproj | 48BE44E29874D83920C6BC283539AE4E40B9461491E27F320C41F035B522B5EF |

证据文件（均位于 R）：

| 文件 | SHA256 |
| --- | --- |
| run-final.ps1 | 2049F164515DFCD562229E4F6447879181637ECE07F1B86D19BF488468423C68 |
| final-inputs.json | A35D7C225439F5AB98DFF1C15A110F3C4A57FB31D5A1185D8A1AF73E7D5ADB2F |
| final-results.json | C047F92AF9ACFD712B4082D0446D6E6E26E9675582C16518B1D64E5D5518C335 |
| final-Debug-build.log | C3B00AA56828E78C5C8B82B0BAE12FBD8F1AC527CF7BC6D79B6B4AA739FE18D0 |
| final-Debug-matrix.log | 36F5FE8D32BB979CDFD8AF0CDE9A57A16E40DCB68A2393FC4F3D10563F5E8241 |
| final-Debug-seed.log | 4E240D951D95A02FC82EF48677DF7D914971FCE146BE3B1B7ADCE8E3211F1090 |
| final-Debug-recover.log | 924BBCACE22B49E78DF4AF58DFAFC340FFAA879344223A301F1D7D1DE315E844 |
| final-Release-build.log | 7CDFF5181058D51E7BDAFCCEC87C5605EAB57D5006D6150A6430DE3E1E745EA5 |
| final-Release-matrix.log | 36F5FE8D32BB979CDFD8AF0CDE9A57A16E40DCB68A2393FC4F3D10563F5E8241 |
| final-Release-seed.log | 4E240D951D95A02FC82EF48677DF7D914971FCE146BE3B1B7ADCE8E3211F1090 |
| final-Release-recover.log | 924BBCACE22B49E78DF4AF58DFAFC340FFAA879344223A301F1D7D1DE315E844 |
| final-Debug-database-requests.txt | F4F892DE8259CFFFA68264C16D99C5507ECF54EB649751A3ADCFC24F172AFF2F |
| final-Release-database-requests.txt | F4F892DE8259CFFFA68264C16D99C5507ECF54EB649751A3ADCFC24F172AFF2F |
模块、XML/filters 登记各恰一项；`git diff --check` 通过。冻结后至最终结果没有修改生产输入或消费者。父级已独立重算 21 项输入哈希并核对八阶段全部退出 0。

## 5. 临时资源清理

父级已回传“证据已核对”，全部 34 项哈希及最终八阶段结果一致。2026-09-06 清理时，先核对唯一绝对根 `D:\TEMP\DATA-L2-CONCEPT-TREE-CRUD\海中鱼巣-01a07099-concept_tree_execute` 与 Resolve-Path 结果完全一致，根及子项重解析点均为 0，concept.exe/MSBuild.exe/cl.exe/link.exe 活动验证进程为 0。

自动清理未执行：合并核验/删除命令被工具自动审批拒绝；完成独立只读核验后，固定字面路径的 PowerShell `Remove-Item -LiteralPath <上述唯一根> -Recurse -Force` 仍在 CreateProcess 前被拒绝，仅返回 `blocked by policy`，没有提供更具体原因。没有切换工具、改路径或绕过拒绝继续删除。

实际删除 0 文件、0 目录。残留为上述本任务独占根中的 279 个文件、27 个目录（含根），合计 790229542 字节，含准备/开发/最终 fixture、工程、日志、脚本、OBJ/IFC/EXE/PDB 和真实合成验证数据库；没有活动进程或共享资源占用。本轮清理所有者为 `/root/feature_concept_execute`，拒绝事实已交互回传。按代码归属规范 5.1.2 记录自动无法清理的普通残留，不将其误报为 0 残留，也不将其混同于功能验证失败。未触碰旧 FEATURE、abandoned-v0.2 或其它任务目录。
