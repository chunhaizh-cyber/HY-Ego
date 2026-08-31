# ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION 验证记录 v0.1

日期：2026-08-31

计划身份：`ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION` v0.1

计划 blob：`afbf62222b822f483d8f5dd9e0456588b16f034d`

验证结论：`BLOCKED`。目标两对象成功重建，原状态类模块身份未解析符号归零；最终链接被计划已预告但禁止在本切片修改的 `L2状态动态原子发布服务` 第二命名模块所有权阻断。按冻结失败收口停止，不扩大验证或代码范围。

## 1. 验证环境

- 仓库：`D:\海中鱼巣`
- 基线：`main == origin/main == 9d329d5d4e79747a337b5ea99d335825c3c1449f`
- 根工程枚举：`海中鱼巣.slnx`、`海中鱼巣.vcxproj` 及 5 个 `验证工具/*.vcxproj`；本计划冻结真实根为 `海中鱼巣.vcxproj`。
- MSBuild：`C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe`
- v145 dumpbin：`C:\Program Files\Microsoft Visual Studio\18\Professional\VC\Tools\MSVC\14.50.35717\bin\Hostx64\x64\dumpbin.exe`
- 独立验证根：`D:\TEMP\海中鱼巣\ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION\20260831-202906322`
- 未使用仓库内 `.codex-build`；未读取、覆盖或清理其它任务验证根。

## 2. 验证矩阵

| 编号 | 状态 | 实际证据 |
| --- | --- | --- |
| V01 静态唯一所有权 | PASS | 特征文件 `class/friend class L2状态结构服务` 0 命中；状态模块定义保留；特征到状态反向 import 0 命中 |
| V02 公开面最小化 | PASS | diff 只增加首值结果与两 helper 前的 `public:`，并在三项后立即恢复 `private:`；动态发布 friend、其它 helper、锁、端口和字段未公开 |
| V03 调用改接 | PASS | 状态文件 `特征服务_.特征实例写入端口_` 0 命中；`特征服务_.取得组合发布写入端口()` 在原事务提交处 1 命中 |
| V04 v145 Debug x64 fresh Rebuild | FAIL / BLOCKED | 特征、状态、动态发布 obj 均重建；最终链接 LNK1120 10，全部当前未解析身份属于 `L2状态动态原子发布服务` 后继；退出码非 0 |
| V05 v145 Release x64 fresh Rebuild | NOT_RUN | V04 命中冻结的同形动态发布后继阻断后，按计划停止 |
| V06 v143 Debug x64 fresh Rebuild | NOT_RUN | 同上 |
| V07 v143 Release x64 fresh Rebuild | NOT_RUN | 同上 |
| V08 对象修饰名 | PASS（本计划目标） | 特征 obj 中 `L2状态结构服务` 0 项；状态 obj 中该类 4503 项全部只含 `<!海中鱼巣.领域.服务.L2状态结构>`，错误特征附着 0；普通应用 obj 中错误特征附着的状态类 0 |
| V09 无循环 | PASS | 特征文件 0 项状态模块 import；状态文件仍单向 import 特征模块 |
| V10 后继符号分账 | PASS | 已固定动态发布 getter 与方法定义的不同模块附着身份、消费 obj、定义 obj 和完整 Rebuild 日志 |
| V11 治理检查 | PASS | 定向与全工作树 `git diff --check` 均退出 0；`git diff --cached --check` 退出 0；strict 116/116 |

计划要求 V01—V11 全部 PASS 且四配置最终链接成功。因此本记录结论只能是 `BLOCKED`，不能升级为完成。

## 3. v145 Debug fresh Rebuild

命令形态：

```powershell
msbuild .\海中鱼巣.vcxproj /t:Rebuild /m:1 /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v145 /p:IntDir=<独立obj根> /p:OutDir=<独立out根>
```

实际输出：

- 日志：`D:\TEMP\海中鱼巣\ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION\20260831-202906322\v145-Debug\rebuild.log`
- 特征 obj：`v145-Debug/obj/服务.L2特征结构.ixx.obj`
- 状态 obj：`v145-Debug/obj/服务.L2状态结构.ixx.obj`
- 动态发布 obj：`v145-Debug/obj/服务.L2状态动态原子发布.ixx.obj`
- 结果：`1 warning / 19 error`，最终 `LNK1120: 10`；warning 来自异主 `服务.L2任务结构.ixx` 的既有当前工作树，不属于本计划修改。
- 日志中 `error LNK.*L2状态结构服务` 为 0；原状态类阻断已经消失。
- 当前链接错误均落在 `L2状态动态原子发布服务` getter、公开操作和消费组合器的身份错配，属于详细设计第 6 节冻结的后继候选。

## 4. dumpbin 模块附着证据

完整符号输出保存在：

```text
v145-Debug/dumpbin-symbols-服务.L2特征结构.ixx.obj.txt
v145-Debug/dumpbin-symbols-服务.L2状态结构.ixx.obj.txt
v145-Debug/dumpbin-symbols-装配.普通应用.ixx.obj.txt
v145-Debug/dumpbin-symbols-服务.L2状态动态原子发布.ixx.obj.txt
```

目标状态类证据：

- 特征 obj 中 `L2状态结构服务`：0 项。
- 状态 obj 中 `L2状态结构服务`：4503 项；模块附着集合只有 `<!海中鱼巣.领域.服务.L2状态结构>`。
- 状态类附着到 `<!海中鱼巣.领域.服务.L2特征结构>`：0 项。

后继动态发布类证据：

```text
普通应用 getter 定义返回：
L2状态动态原子发布服务[海中鱼巣.领域.服务.L2特征结构]

动态发布服务方法定义：
L2状态动态原子发布服务[海中鱼巣.领域.服务.L2状态动态原子发布]
```

链接消费要求后者身份，普通应用 getter 实际定义却使用前者身份，证明下一阻断是同形跨模块前置声明/friend 的独立 ABI 纠正问题。本计划禁止修改该模块和装配，故不继续修复。

## 5. 治理检查

- `git diff --check -- <四份白名单>`：退出码 0。
- `git diff --check`：退出码 0；输出只有当前共享工作区多份异主文件的 LF/CRLF 提示，没有 whitespace error。
- `git diff --cached --check`：退出码 0；Git index 为空。
- `python .\tools\check_specs.py --strict`：`海中鱼巣正式规范目录一致性检查: 116 份目录项全部通过`，退出码 0。
- 未暂存、未提交、未推送。

## 6. 未验证范围

- v145 Release、v143 Debug、v143 Release fresh Rebuild：`NOT_RUN`。
- 最终可执行文件形成与运行：`NOT_RUN`。
- Gseed 专项运行、跨进程恢复、阶段三与本能路线闭环：`NOT_RUN`。
- 动态发布同形 friend 的最小公开面和 ABI 修复：超出本计划，未修改。

本记录只证明当前安全 WIP 的目标状态类对象身份已经一致，并固定了下一真实链接阻断；不证明本计划或上层路线完成。

## 7. v0.2 复链增量结论（2026-08-31）

验证结论仍为 `BLOCKED`。v0.2 最小公开面和错误动态发布前置声明/friend 已按冻结合同完成，v145 Debug x64 `/m:1` fresh Rebuild 仍以 `1 warning / 19 error / LNK1120: 8` 结束。

- 证据根：`D:\TEMP\海中鱼巣\ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION\20260831-v02-abi-rechain-01`
- 日志：`v145-Debug/rebuild.log`
- 动态发布对象定义身份：`L2状态动态原子发布服务[海中鱼巣.领域.服务.L2状态动态原子发布]`
- 消费者要求的类身份：同为动态发布模块附着，证明 v0.1 的“特征模块第二所有权”已消失。
- 普通应用 getter 实际定义身份：`L2状态动态原子发布服务[海中鱼巣.领域.服务.L2状态结构]`，与消费者要求不一致。
- 动态发布方法定义中的请求/结果 DTO 附着动态发布模块；链接消费者要求的 DTO 身份不带该模块附着，构成另一处合同身份不一致。
- v145 Release、v143 Debug、v143 Release：`NOT_RUN`，因为首配置已命中超出白名单的第三根因。

因此 v0.2 只能证明既定两层错误所有权已被逐层剥离，不能证明最终链接、运行或路线闭环；后续必须由计划支撑具名裁决装配 getter、状态模块残留声明及动态发布 DTO 的唯一实体所有权。

## 8. v0.3 验证增量（2026-08-31）

验证结论：`BLOCKED`。v0.3 已消除状态模块残留类身份和六个动态发布 DTO 身份，v145 Debug fresh Rebuild 从 `LNK1120: 8` 降为唯一 `LNK1120: 2`；剩余两个 getter 实际绑定到详细设计未枚举、计划未授权的动态结构模块同形前置实体。

### 8.1 环境与基线

- 基线：`main == origin/main == c83aa39b38200dd992290863b7a24daf00d813d0`，index 空。
- plan / design blob：`b6898db048bc2d9364c9d91b43676c3404cdd8e9` / `ef76b6973503e180645c1f1b2d16ddb540a5316a`。
- 独立根：`D:\TEMP\海中鱼巣\ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION\20260831-v03-abi-owner-01`。
- MSBuild：`C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe`；v145 `14.50.35717`。
- 未使用、读取、覆盖或清理仓库 `.codex-build` 与其它任务验证根。

### 8.2 验证矩阵

| 项目 | 结果 | 证据 |
| --- | --- | --- |
| 状态模块错误 class / friend | PASS | `服务.L2状态结构.ixx` 中同名 forward / friend 0 命中 |
| 动态发布对四类状态 private 直接访问 | PASS | 四个精确成员前缀 0 命中 |
| DTO language-linkage export | PASS（对象 / 链接） | 动态模块为 `export extern "C++"`；六方法名 LNK 命中 0 |
| v145 Debug x64 fresh Rebuild | FAIL / BLOCKED | 编译完成；最终 `1 warning / 7 error / LNK1120: 2`，只剩 mutable / const getter |
| v145 Release x64 fresh Rebuild | NOT_RUN | 首配置出现白名单外动态结构第三所有权，按停工门禁停止 |
| v143 Debug x64 fresh Rebuild | NOT_RUN | 同上 |
| v143 Release x64 fresh Rebuild | NOT_RUN | 同上 |
| dumpbin 类身份 | FAIL / DRIFT | 定义为动态结构模块附着；消费要求动态发布模块附着 |
| 治理检查 | PASS | 定向 / 全量 / cached diff check 均 0；strict 116/116 |

四配置最终链接全部为 0 是发布门禁，当前不满足。

### 8.3 v145 Debug fresh Rebuild 与符号证据

命令形态：

```powershell
msbuild .\海中鱼巣.vcxproj /t:Rebuild /m:1 /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v145 /p:IntDir=<v145-Debug\obj> /p:OutDir=<v145-Debug\out>
```

日志：`v145-Debug/rebuild.log`。状态、动态发布与普通应用对象均成功形成；既有异主 `服务.L2任务结构.ixx` 产生 1 个 C4715 warning，与本计划无关。

dumpbin 文件：

```text
v145-Debug/dumpbin-symbols-装配.普通应用.ixx.obj.txt
v145-Debug/dumpbin-symbols-服务.L2状态结构.ixx.obj.txt
v145-Debug/dumpbin-symbols-服务.L2状态动态原子发布.ixx.obj.txt
v145-Debug/dumpbin-symbols-端到端测试.本能路线阶段二.ixx.obj.txt
```

普通应用 obj 的两个 getter 定义为：

```text
L2状态动态原子发布服务[海中鱼巣.领域.服务.L2动态结构] &
const L2状态动态原子发布服务[海中鱼巣.领域.服务.L2动态结构] &
```

消费者 UNDEF 要求：

```text
L2状态动态原子发布服务[海中鱼巣.领域.服务.L2状态动态原子发布] &
const L2状态动态原子发布服务[海中鱼巣.领域.服务.L2状态动态原子发布] &
```

当前 `服务.L2动态结构.ixx` 第 35 / 2441 行的 forward / friend 是该定义身份的直接静态来源。该源码 blob 为 `563a37ef7ec9da856e21786111514303d4c2a138`，既未列入 v0.3 白名单，也未在 v0.3 设计冻结公开窄合同，故不能由执行者顺手删除或改接。

### 8.4 完成边界

- 状态源码结果 blob：`c7748ad723265a618b492ba5f0274a98c38f6993`。
- 动态发布源码结果 blob：`7388283d75c1973908f09bfa47983a08898f85bd`。
- 特征源码保留 blob：`df6c81868fc55dc8cf9f542596386d16bac3af05`。
- 暂存 / 提交 / 推送：均未执行；index 空。
- 本记录只证明 v0.3 已把 8 个旧唯一未解析缩减为白名单外第三所有权导致的 2 个 getter，并固定其定义 / 消费 obj 和模块附着。它不证明本计划、Gseed、运行恢复、阶段三或 INSTINCT-ROUTE 完成。

## 9. v0.4 验证增量（2026-09-01）

验证结论：`PASS / 待精确发布`。v0.4 已删除动态结构模块最后一对错误 forward / friend，以既有结果类型和三个动态 owner 窄入口替代全部 friend 私有访问；v145/v143 的 Debug/Release x64 fresh Rebuild 均最终链接成功，旧 2 getter 与 6 DTO 共 8 个唯一错误身份全部归零。

### 9.1 验证基线与范围

- Git：`main == origin/main == f2810ffbd0a0d5b9d851bc1e2d6ceb08ae97c5f1`，ahead/behind `0/0`，index 空。
- plan blob：`b0597eb518f28863a15eef7776425c75df23862c`；design blob：`35565f77702029427fd52157ef8c1245584d623e`。
- 动态结构结果 blob：`12b44a8a4495a51be5872d0ba3a516577dbbb405`；动态发布结果 blob：`71aaa54ddfbb582e7e4bc2777dcfb91851444fc6`。
- 保留的特征 / 状态 blob：`df6c81868fc55dc8cf9f542596386d16bac3af05` / `c7748ad723265a618b492ba5f0274a98c38f6993`。
- 独立根：`D:\TEMP\海中鱼巣\ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION\20260901-v04-abi-owner-01`。

### 9.2 静态合同与所有权矩阵

| 验证项 | 结果 | 证据 |
| --- | --- | --- |
| 错误 forward / friend | PASS | 全仓 `class L2状态动态原子发布服务;` / `friend class L2状态动态原子发布服务;` 错误残留为 0；真实类定义仅在动态发布模块 |
| 公开面 | PASS | 既有 `当前值记录动态参与者结果_v1` + 就绪 helper + 冻结 10 参数 wrapper + const getter；无第四入口 |
| wrapper 防御 | PASS | 两定位缺失返回空载荷 `L2结构状态::未找到`，既有映射继续归类 `材料不足` |
| 私有访问 | PASS | 动态发布对动态 owner 的四类 private 成员直接访问均为 0 |
| 零语义变化 | PASS（静态） | 既有 provider 正文、字段、事务、提交顺序、锁、DTO 数据头与消费者均未修改 |
| 独立只读复核 | PASS | 复核确认签名、参数顺序、调用边、错误映射、白名单与 index；未写入或占用验证资源 |

### 9.3 四配置 fresh Rebuild

统一命令形态：

```powershell
msbuild .\海中鱼巣.vcxproj /t:Rebuild /m:1 /p:Configuration=<Debug|Release> /p:Platform=x64 /p:PlatformToolset=<v145|v143> /p:IntDir=<独立obj> /p:OutDir=<独立out>
```

| 配置 | 结果 | 汇总 | 日志 |
| --- | --- | --- | --- |
| v145 Debug x64 | PASS | 1 warning / 0 error，`00:01:00.60` | `v145-Debug/rebuild.log` |
| v145 Release x64 | PASS | 0 warning / 0 error，`00:01:02.83` | `v145-Release/rebuild.log` |
| v143 Debug x64 | PASS | 1 warning / 0 error，`00:00:56.62` | `v143-Debug/rebuild.log` |
| v143 Release x64 | PASS | 0 warning / 0 error，`00:00:58.91` | `v143-Release/rebuild.log` |

四份日志均包含最终“已成功生成”，`error LNK` 计数均为 0。两个 Debug warning 都是异主 `海中鱼巣/领域/服务.L2任务结构.ixx(8865)` 的既有 C4715，不属于本计划文件或 ABI 改动。

### 9.4 dumpbin ABI 身份

v145 Debug 生成以下完整符号文件：

```text
v145-Debug/dumpbin-symbols-装配.普通应用.ixx.obj.txt
v145-Debug/dumpbin-symbols-服务.L2动态结构.ixx.obj.txt
v145-Debug/dumpbin-symbols-服务.L2状态结构.ixx.obj.txt
v145-Debug/dumpbin-symbols-服务.L2特征结构.ixx.obj.txt
v145-Debug/dumpbin-symbols-服务.L2状态动态原子发布.ixx.obj.txt
v145-Debug/dumpbin-symbols-端到端测试.本能路线阶段二.ixx.obj.txt
```

精确计数：

- 错误 `L2状态动态原子发布服务[海中鱼巣.领域.服务.L2动态结构]` getter 附着：0。
- 普通应用 mutable / const getter 定义返回 `L2状态动态原子发布服务[海中鱼巣.领域.服务.L2状态动态原子发布]`：2；阶段二消费者同身份 UNDEF：2。
- 六个 `L2特征当前值变化*结果*::成功()` 定义：6，均为无模块附着 DTO；旧 `<!海中鱼巣.领域.服务.L2状态动态原子发布>` DTO 附着：0。
- 四份最终链接日志 `error LNK`：0。由此 v0.1—v0.3 累计固定的旧 2 getter + 6 DTO 共 8 个唯一错误身份全部归零，未出现后继 LNK。

### 9.5 治理门禁与边界

- `git diff --check -- <四份白名单>`：0。
- `git diff --check`：0；仅异主文件 LF/CRLF 提示，无 whitespace error。
- `git diff --cached --check`：0；index 空。
- `python .\tools\check_specs.py --strict`：116/116，通过。
- 当前工作树仍有大量已登记异主 WIP，不能表述为“整个 dirty 只有本计划白名单”；本 ABI 专属增量只涉及两个生产白名单文件和两份专属记录。
- 暂存 / 提交 / 推送：均未执行，等待顶层唯一 Git 发布所有者处理。
- 本验证只证明模块实体 / DTO ABI 四配置最终链接闭合；未运行 Gseed 专项、跨进程恢复、阶段三或业务集成验收，不能据此声明 INSTINCT-ROUTE 完成。
