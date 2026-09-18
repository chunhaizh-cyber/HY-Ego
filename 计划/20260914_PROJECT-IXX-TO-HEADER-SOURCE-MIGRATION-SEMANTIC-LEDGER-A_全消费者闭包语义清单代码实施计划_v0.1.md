# 全消费者闭包迁移证据清单代码实施计划

计划身份：`PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A`

日期：2026-09-14
版本：v0.5
状态：已退出（原迁移前输入对象已经不存在；后继只审计现有迁移后的消费者闭包）

## 1. 目标、范围与完成条件

本叶原在“为全量 `.ixx` → `.h/.cpp` 迁移冻结逐实体施工输入”的目标内，替换 LLVM/Clang AST 硬门槛。`6d2d4040` 已证明 LLVM 22.1.8 不能接受当时 `服务.L1事实基座.ixx` 的合法 MSVC 模块导入布局；该工具失败不等于 MSVC 工程或迁移目标失败，且不得为迎合 LLVM 改旧模块源码。

本版本基于 `68ebb3cc54282e3ab6e18c5b47a6a648eaae73c0` 的正式 HEAD 裁决本叶退出：`海中鱼巣/**` 已为 70 个 `.h`、50 个 `.cpp`、0 个 `.ixx`、0 个 `.cppcpp`；主工程 Debug/Release x64 求值均为 27 个 `ClCompile`，其中 26 个未排除项均为 `.cpp`，唯一排除项为关闭 D455 时的 `.cpp`。因此，原合同冻结的“56 个 `.ixx`、两个 `.cppcpp` 与 `入口.cpp` 共 59 个待扫描 TU”已经不是当前输入；继续执行只会把历史迁移对象伪造为当前事实。

该事实不等于迁移已经完整闭合。正式 HEAD 仍有三个 `验证工具/L1*参数验证.vcxproj` 的 `.ixx` `ClCompile` 项及相应 `.cpp` 的 `import`，并有 `验证工具/运行世界树根启动专项验证.ps1` 读取旧 `.ixx/.cppcpp` 路径。这些是现存消费者，不得由本叶的旧输入清单忽略，也不得通过触碰工作区中的异主删除 WIP 擅自消除。后继 `PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-POST-CONSUMER-CLOSURE-AUDIT` 只审计并逐项分账这些现状；它不声明删除、改写或行为等价。

本叶完成时发布一组可审查的迁移准备材料：

1. 精确正式 HEAD、MSVC 编译器/工具集、命令行和输入 blob；
2. 由隔离成功构建实际产生的 MSVC `/scanDependencies` 与 `/sourceDependencies` JSON 依赖闭包；
3. 源码候选实体清单：原文签名、起止行、实体类型标记、原文件 blob 与 SHA-256；
4. 人工复核的 `h-only` 或 `h+cpp` 分类和理由；
5. 全量迁移完成后必须达到的 clean Debug/Release x64 编译闭包及扫描退出条件。

该组合为物理落点与迁移批次的审查输入，不是 C++ AST，也不产生函数调用图。它不得声称“所有直接调用方”“无静态调用方”、虚调用、模板实例化、隐式使用或行为等价已经被证明。

允许写入仅为本计划专属施工/验证记录及其 JSON 附件。禁止修改 `海中鱼巣/**`、`验证工具/**`、工程 XML、正式规范、其它计划及三项交接删除 WIP；不得恢复、暂存、覆盖或删除那些 WIP。

## 2. 已核对依据与裁决

- `CODE-FILE-01` v4.0 要求同基本名 `.h/.cpp`、唯一头文件所有者、唯一非 inline 定义位置，并禁止 `.ixx/.cppcpp/module/export/import` 终态残留。
- `6d2d4040` 的失败记录表明：原 LLVM BMI → JSON AST 路线在第 23 个模块停止；历史 module JSON 只是旧快照诊断，不能作为当前 HEAD 的代码证据。
- MSVC `/scanDependencies` 与 `/sourceDependencies` 能在隔离的成功编译中给出实际翻译单元依赖闭包；它们不能给出函数 AST、实体语义绑定或调用关系。
- PDB/DIA 只在对应配置成功构建后，才可用于交叉记录已产物中的符号名和行号；它不是必须输入，也不能补成源级调用图。

执行 S0 以当时 `main == origin/main` 的计划 blob 为准。`89697ac7` 及 `6d2d4040` 只保留为旧失败的可追溯证据，严禁把历史 JSON、旧 blob 或旧编译输出标作当前 HEAD 事实。

## 3. 固定输入、输出与字段合同

### 3.1 输入闭包

从当前正式 HEAD 读取，而不是从工作区 WIP 或历史提交导出：

- `git ls-files '*.ixx'` 的全部模块、两个 `.cppcpp`、`入口.cpp`；
- 所有源码中的 `import`/`export import` 消费者，以及主工程、filters、五个验证工程的相关 XML 项；
- 三项交接验证消费者只从其当前正式 Git 对象读取；若尚是异主 WIP 或正式对象缺失，逐项记录为阻断，不触碰工作区。

每个输入清单行必须含相对路径、Git blob、SHA-256、文件类型和发现命令。扫描结果扩大范围时扩大清单；不得因目录、大小或是否“低依赖”缩小。

### 3.2 专属仓内产物与附件路径

本叶唯一允许写入的仓内路径在 S0 前固定为以下集合。历史 v0.2 失败记录只读保留；本轮不得覆盖它们：

```text
施工记录/20260914_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A_施工记录_v0.4.md
验证记录/20260914_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A_验证记录_v0.4.md
施工记录/附件/20260914_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A/v0.4/输入闭包.json
施工记录/附件/20260914_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A/v0.4/工程求值/Debug-x64.ClCompile.json
施工记录/附件/20260914_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A/v0.4/工程求值/Release-x64.ClCompile.json
施工记录/附件/20260914_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A/v0.4/Debug-x64/依赖索引.json
施工记录/附件/20260914_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A/v0.4/Release-x64/依赖索引.json
施工记录/附件/20260914_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A/v0.4/源码候选实体清单.json
施工记录/附件/20260914_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A/v0.4/人工物理落点复核.json
施工记录/附件/20260914_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A/v0.4/执行元数据.json
施工记录/附件/20260914_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A/v0.4/失败分账.json
```

原始依赖 JSON 不得省略或改名：每个配置的 `依赖索引.json` 必须逐条引用同一附件根下的 `Debug-x64/scanDependencies/<tu_sha256>.json`、`Debug-x64/sourceDependencies/<tu_sha256>.json`、`Release-x64/scanDependencies/<tu_sha256>.json`、`Release-x64/sourceDependencies/<tu_sha256>.json`。`<tu_sha256>` 是 UTF-8、LF、仓库相对 `/` 路径的 SHA-256 小写十六进制；每个索引条目再记录原 JSON SHA-256、相对路径、TU、实际 `cl` 退出码。任何丢失、重复键、索引外 JSON 或 JSON 解析失败都是本叶失败，不能改以汇总表代替原始 JSON。

### 3.3 MSVC 工程入口、隔离与实际依赖闭包

唯一工程入口为当前 HEAD 的 `海中鱼巣.vcxproj`；不构建五个验证工程。本叶只扫描主工程的 `Debug|x64` 与 `Release|x64`，固定 `EnableD455RealSense=false`。工具入口必须由以下命令取得并记录其标准输出、退出码和 SHA-256：

```powershell
$vswhere = 'C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe'
$vs = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -property installationPath
$msbuild = Join-Path $vs 'MSBuild\Current\Bin\MSBuild.exe'
$vsdevcmd = Join-Path $vs 'Common7\Tools\VsDevCmd.bat'
cmd /c "call `"$vsdevcmd`" -arch=x64 -host_arch=x64 >nul && `"$msbuild`" -version"
```

`$vswhere`、`$msbuild`、`$vsdevcmd` 任一不存在、返回多于一个安装根、或 `MSBuild -version` 失败即安全失败；不得退回 PATH 中其它 `MSBuild.exe`。输入副本只能从 S0 所在 HEAD 用 `git archive --format=tar <HEAD> | tar -xf - -C <轮次>/source` 建立。实际构建只允许在 `D:/TEMP/海中鱼巣/PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A/<UTC轮次>/source/` 中运行；对象、二进制和 JSON 分别固定落入该轮次的 `obj/Debug-x64`、`obj/Release-x64`、`bin/Debug-x64`、`bin/Release-x64`，不得读写仓库构建目录。

每配置先在输入副本运行以下求值命令，完整 stdout 原样写入 §3.2 对应 `工程求值/*.ClCompile.json`：

```powershell
& $msbuild .\海中鱼巣.vcxproj -getItem:ClCompile -property:Configuration=Debug -property:Platform=x64 -property:EnableD455RealSense=false -nologo
& $msbuild .\海中鱼巣.vcxproj -getItem:ClCompile -property:Configuration=Release -property:Platform=x64 -property:EnableD455RealSense=false -nologo
```

要求范围是每个求值结果中 `ExcludedFromBuild != true` 的全部 `ClCompile.Identity`，按 `/` 规范化并按序去重。当前已冻结集合为两配置均相同的 **59** 个 TU：主工程 **56** 个未排除的 `.ixx`、`海中鱼巣/装配.普通应用.cppcpp`、`海中鱼巣/业务/应用服务.自我形成.cppcpp` 与 `海中鱼巣/入口.cpp`；`海中鱼巣/适配/采集器.D455相机.ixx` 因固定关闭 D455 而唯一 `ExcludedFromBuild=true`。两个配置的求值集合、数量或该 D455 排除事实与此合同不符时，停止并在失败分账中写实际集合差异；不得自行缩小、扩大或改用一边集合。

每配置只在预先创建的 `<轮次>/obj/<配置>/scanDependencies` 与 `<轮次>/obj/<配置>/sourceDependencies` 下构建。唯一构建命令形态如下；`<tu_sha256>` 通过项目项元数据 `%(ClCompile.Identity)` 的规范化路径计算，执行前先生成且记录 `TU -> sha256 -> 两个输出路径` 映射。若 MSBuild 项元数据不能产生该唯一映射，停止，不得使用 `%(Filename)` 或共享输出名。

```powershell
& $msbuild .\海中鱼巣.vcxproj -t:Rebuild -m:1 -nologo `
  -property:Configuration=<Debug|Release> -property:Platform=x64 -property:EnableD455RealSense=false `
  -property:BaseIntermediateOutputPath=<轮次>\obj\<配置>\ `
  -property:IntDir=<轮次>\obj\<配置>\ `
  -property:OutDir=<轮次>\bin\<配置>\ `
  -property:AdditionalOptions="/scanDependencies <轮次>\\obj\\<配置>\\scanDependencies\\<tu_sha256>.json /sourceDependencies <轮次>\\obj\\<配置>\\sourceDependencies\\<tu_sha256>.json %(AdditionalOptions)"
```

上式的逐 TU `AdditionalOptions` 必须通过输入副本外、但同一轮次内的临时 MSBuild `.targets` 注入：该 targets 仅以 `BeforeTargets=ClCompile` 逐 `ClCompile` 项计算 `<tu_sha256>` 并追加两开关，不修改输入副本工程或仓库工程；其完整 XML、hash 和注入命令写入 `执行元数据.json`。若该注入不能保证每个实际 `cl` 命令恰有一对唯一绝对 JSON 输出，停止并记录首个命令，不能尝试全局同名 `/scanDependencies` 或 `/sourceDependencies`。

只有构建返回 0、要求范围 59/59 均具有可解析的两类 JSON、且 JSON 的 source/TU 字段与映射相符时，该配置可标“成功依赖闭包”。任何失败只保留命令、首个真实诊断、退出码、已产生 JSON 与缺失 TU；不得用旧 JSON、词法 import 图或人工猜测补齐。Debug 与 Release 独立分账，任一失败即本叶“证据不完整”。

### 3.4 源码候选实体清单

对当前 HEAD 的每个模块、`.cppcpp` 和已有 `.cpp` 实现单元生成可人工审查的候选条目。候选可由受控源码提取产生，但提取器只定位文本，不能裁决 C++ 语义或调用关系。每条最少具有：

| 字段 | 固定含义 |
| --- | --- |
| `source_path`、`git_blob`、`sha256` | 当前正式输入身份 |
| `start_line`、`end_line`、`signature_text` | 原文锚点；签名保留原文，不作伪语义规范化 |
| `candidate_kind` | `type`、`function-declaration`、`function-definition`、`template-or-inline`、`data`、`alias-or-enum`、`unknown` 之一 |
| `module_or_translation_unit` | 原模块声明名或翻译单元路径 |
| `extraction_rule`、`review_state` | 提取规则版本；初始只能为 `pending-human-review` |
| `dependency_evidence_refs` | 可关联的 MSVC JSON 路径/哈希；为空必须如实为空 |

提取器无法可靠划分的宏、条件编译、模板或中文标识符条目必须标 `unknown` 并保留原行区间，等待人工复核；不得丢弃、假定 inline 或假定私有。

### 3.5 人工物理落点复核

每个候选实体由人工复核后追加以下字段：`reviewer`、`review_time`、`classification`、`reason`、`target_header`、`target_source`、`source_anchor`。

- `h-only` 仅可用于公开声明、类型/枚举/别名/常量以及必须可见的模板或 inline 正文；理由必须写明可见性或语言要求。
- `h+cpp` 用于需要公开声明且有唯一非 inline 定义的实体；声明落唯一同基本名 `.h`，定义落唯一同基本名 `.cpp`。
- 私有辅助仅在人工确认不导出且不被其它翻译单元需要时，可标 `cpp-private`；它仍属于同基本名 `.cpp`。
- 不能确定时为 `blocked`，不得以默认规则代替复核。

此分类是迁移物理组织裁决，不改变函数语义、可见性、所有权或公开 ABI。最终实际迁移计划只能消费已复核且不存在 `blocked` 的范围。

## 4. 实施步骤与失败收口

1. S0：核对 `main == origin/main`、index 空、计划索引本行与 `HEAD:<计划路径>` 的 v0.3 blob 一致；核对文件登记、当前输入对象与隔离验证根互不冲突。任何源码、工程或交接 WIP 不在本叶写入范围。
2. 从当前 HEAD 建立输入/消费者/工程清单，逐项写 blob 与 hash；以 `rg` 的 import 命中只形成消费者候选，不称为函数关系。
3. 运行隔离 Debug x64 和 Release x64 MSVC 构建，并保存每个成功翻译单元的两类依赖 JSON；失败配置如实收口。
4. 生成候选实体清单，进行 `h-only` / `h+cpp` / `cpp-private` / `blocked` 的人工复核；对每一 `blocked` 给出最小原文锚点和问题。
5. 可选：只有某一配置成功构建后，使用其 PDB/DIA 交叉记录符号和行号；DIA 与源码候选不一致时标阻断，不以其中一方覆盖另一方。
6. 将原始 JSON、候选清单、人工复核表、命令/哈希和失败分账作为附件提交。若 Debug/Release 任一配置无法产生其要求范围的成功依赖闭包，或候选仍有 `blocked`，本叶发布“证据不完整”结果，并保持实际迁移计划不可激活。

本叶不执行实际 `.h/.cpp` 改写。后继实际迁移计划必须在全闭合改写之后、没有旧模块依赖的 clean 隔离根中完成 Debug/Release x64 编译；两配置均成功，且主工程、filters、验证工程和源码扫描均无 `.ixx/.cppcpp/module/export/import` 生产依赖，才可声明迁移编译闭包完成。

## 5. 验证与完成声明边界

本叶验证：附件 JSON 均可解析；每项 hash 可读回；MSVC 依赖 JSON 只关联实际成功编译单元；候选条目均有原文与行区间；人工分类无未解释的空值；`git diff --check`；`python .\tools\check_specs.py --strict`。

本叶只证明迁移前证据的来源、范围和审查状态。它不证明函数调用关系、AST 完整性、代码迁移、编译、链接、运行、恢复、业务行为或业务闭环。实际迁移执行者必须在本计划结果提交后从新的正式 HEAD 重新 S0。

## 修订记录

| 日期 | 版本 | 修订内容 |
| --- | --- | --- |
| 2026-09-18 | v0.5 | 执行 S0 发现原冻结的 59 个模块迁移对象已由当前正式 HEAD 的 `.h/.cpp` 树取代，而验证工程和脚本仍残留活动旧格式消费者；退出不可再现的迁移前证据叶，改由迁移后消费者闭包审计后继分账，不把工作区删除 WIP 当作正式清理。 |
| 2026-09-14 | v0.3 | 根据 `6d2d4040` 退回：废除 LLVM BMI/AST 与调用方硬门槛；改为当前 HEAD 的 MSVC 成功构建依赖 JSON、源码候选原文清单和人工物理落点复核；DIA 仅作成功构建后的可选符号行号交叉证据。 |
| 2026-09-14 | v0.2 | 原 LLVM AST 方案；已由本版本退役。 |
