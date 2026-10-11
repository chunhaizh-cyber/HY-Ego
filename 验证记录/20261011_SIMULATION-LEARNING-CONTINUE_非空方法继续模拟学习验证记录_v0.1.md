# 非空方法继续模拟学习验证记录

日期：2026-10-11。范围：SIMULATION-LEARNING-CONTINUE v0.1。

## 1. 授权、基线和所有权

用户要求纠正“方法只要已有条件结果对就直接精确重复、不再学习”。施工依据为[修正计划](../计划/20261011_SIMULATION-LEARNING-CONTINUE_非空方法继续模拟学习修正计划_v0.1.md)，正文blob `1803fcdd5272254e30ba06ced9523cf1e406c9b0`。S0起点为`c88a6f10edbf24a65a784bcb301e2fa9c679a7dc`，main与origin/main一致，index空。

主线拥有服务、方法owner/DTO及本记录，子代理只修改已有模式5自检文件。独立只读复核覆盖生产diff；主线独占实际工程Build、运行资源和Git发布。验证输入为上述基线加本轮六个源码文件WIP，无其它代码切片输入；三份异主目标流程图和`海中鱼巣/数据/**`保留且不暂存。

代码结果提交为本记录所在的同次提交，可用`git log --follow -- 本记录路径`定位；计划退出另由计划所有者登记，不混入代码执行提交。发布前重新fetch确认0/0。

## 2. 实际修正

- 服务不再因任何已有配对而提前返回；每次通过准入的请求都创建新物理模拟样例，运行原本能并fresh读取结果，再提交本次完整C/R。
- 方法owner先校验输入，再扫描全部已有配对。完整条件与结果都相同才精确重复；没有同条件则追加；同条件不同结果明确冲突，不覆盖旧知识、不增加命中。
- AND/OR条件按完整项多重集比较；有序规则保持顺序敏感。多条同条件旧配对按结构不一致拒绝，不随意选择。
- 新配对顺序读取现有物理顺序后取最大值+1；拒绝缺失、负值、重复及MAX无法递增。方法节点存在但完整读取失败映射结构不一致。
- 返回新增或准确匹配的已有配对ID，读回按身份而非`front()`；追加后核验旧配对完整不变。发布后读回失败保留已知ID并返回发布结果未知，不回滚已发布知识。
- DTO原值2改名精确重复，末尾新增同条件结果冲突和已有配对ID载荷；旧枚举引用已迁移。

保持：一次只发布一对；模拟命中0；旧配对/旧副本不修改；取消边界、现实只读登记端口、外设模拟拒绝不变。没有修改self、worker、manager、task、工程登记或本能函数。

## 3. 实际工程构建

使用唯一实际工程`D:/海中鱼巣/海中鱼巣.vcxproj`，没有临时工程、第二main或源码副本。输出隔离在本轮专属目录。

```powershell
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe' 'D:/海中鱼巣/海中鱼巣.vcxproj' /t:Build /p:Configuration=Debug /p:Platform=x64 /p:OutDir=D:/TEMP/海中鱼巣/SIMULATION-LEARNING-CONTINUE/20261011-v0.1/Debug/bin/ /p:IntDir=D:/TEMP/海中鱼巣/SIMULATION-LEARNING-CONTINUE/20261011-v0.1/Debug/obj/ /m:2 /nologo /verbosity:minimal /fl '/flp:logfile=D:/TEMP/海中鱼巣/SIMULATION-LEARNING-CONTINUE/20261011-v0.1/Debug/build.log;verbosity=normal'
```

首次Build退出0、0警告0错误，耗时73.87秒。首次编译期间主线补正服务已发布但返回载荷不一致时的未知状态映射，故首次产物不作为最终输入；源码冻结后以完全相同参数再次Build，仅日志名改为`build-final.log`，退出0、0警告0错误，耗时1.66秒。所有下列测试使用第二次产物。

最终exe SHA256：`27A034719FE85C3244E387AB2C1BBAF5A1EE849D1071242335F67E811E947CE5`。

## 4. 实测结果

在本轮`Debug`目录运行其`bin/海中鱼巣.exe`，正式自检入口及退出码如下：

| 参数 | 结果 | 退出码 |
| --- | --- | --- |
| `--self-test-learning` | 15 PASS / 0 FAIL | 0 |
| `--self-test-simulation` | 21 PASS / 0 FAIL | 0 |
| `--self-test-method-domain` | 29 PASS / 0 FAIL | 0 |
| `--self-test-learning`独立再运行5次 | 每次15 PASS / 0 FAIL | 每次0 |
| `--self-test-learning --self-test-learning` | 重复模式拒绝 | 2 |
| `--self-test-learning --headless` | 混合模式拒绝 | 2 |

模式5实际输出：

```text
PASS head-only registration and declared singleton inputs
PASS sample inputs reject wrong state and MAX before action
PASS learning rejects unsupported input and supports cancellation
PASS fake binding and missing simulated port reject without real fallback
PASS original builtin executes physical sample and fresh output
PASS all webpage builtins reject simulation before external dispatch
PASS worker duplicate, pending acknowledgement and stop cancellation
PASS owner seeds one unrelated CR and copies it before learning (not simulation evidence)
PASS worker appends observed CR to nonempty method and resumes after acknowledgement
PASS repeat learning and method copy preserve knowledge/hit zero
PASS same-condition conflicts include result, direction, confirmation and AND order
PASS nonempty method still rejects invalid definitions and invalid learning materials
PASS OR condition permutations merge but ordered conditions and multiplicities stay distinct
PASS ordinary scene/existence/feature and known concepts remain unchanged
PASS manager reads learned knowledge and waits without creating or advancing tasks
SUMMARY 15 PASS / 0 FAIL
```

关键可观察证据：通过正式owner预先放入`3/9→4`配对（只属承载测试材料，不是实际模拟所得）；随后正式服务实际模拟`4/9→5`，追加第二个CR，首个CR完整不变。再次请求返回第二个CR而非首个，登记读取次数增加，新模拟场景ID不同且节点存在，方法完整DTO不变。worker完成结果确认前保持合并，确认后新请求实际重跑。学习前复制的方法仍只有旧CR，新复制方法完整保留两对及命中0。

AND换序后仍精确重复；同条件下结果值、方向、确认要求分别不同均明确冲突并保持方法完整DTO不变。OR换序重复、有序换序不同、重复项数量不同仍分开。非空方法的非法域、非法角色、概念/私有双域及MAX登记材料不再误报重复。

物理状态证据边界：服务每次内部fresh校验动作、前后状态、结果动态及4→5；自检另外直接调用原本能核验物理样例。外层重复调用只能观察新场景身份、节点存在和端口计数，模拟owner没有按场景ID重新打开样例的公开入口，因此不宣称外层直接读到了每次重复样例的全部内部状态。

`git diff --check`通过；发布暂存后另运行`git diff --cached --check`。换行风格警告不等于编译警告或diff错误。

## 5. 未验证及能力边界

- NOT_RUN：物理坏序号/MAX顺序/重复同条件结构、分配失败及发布后未知故障注入；没有合法公开注入合同。只静态核对这些分支。
- NOT_RUN：全集零写计数、全部概念枚举比对；无公开探针。冲突零配对写入来自方法完整前后快照加写入前返回的静态核对，不冒称底座全量写审计。
- NOT_RUN：Release、Win32、恢复及本轮生产启动/self自动发现集成。旧生产启动证据不升级为本轮验收。
- 本轮只解除非空拒学，仍仅支持既有结算登记的单点输入。没有实现按已有区间继续分层枚举、任意方法模拟、同条件结果自动扩域、self主动重学、真实命中计数或任务执行闭环。

## 6. 代码知识变更清单

共享用途卡与能力图谱当前未覆盖这些函数，已读知识库入口并核对当前源码/调用点；本轮不写共享知识索引。待知识维护者依据本记录同次发布代码建立/更新：

| 完整函数身份与位置 | 用途、合同及调用变化 |
| --- | --- |
| `新模拟学习结果 模拟学习服务::学习(稳定编码,const std::function<bool()>&) noexcept`，业务/模拟学习.服务.cpp，公开 | 非空继续真实模拟后按内容判重；返回准确配对ID及冲突。调用原模拟owner、本能、数值细分器和方法owner不变；不触及现实动作或任务推进。发布后失败保留现场。 |
| `新方法学习发布结果 新_方法类::追加模拟条件结果(稳定编码,const std::vector<新方法条件结果定义>&) noexcept`，领域/新_方法类.cpp，公开 | 原方法互斥内合法性检查、全配对判重/冲突、顺序追加、发布前回滚及发布后读回。新增DTO已有配对ID，不新增owner、不扩大批事务、不修改旧知识/命中/副本。生产调用者仍为模拟学习服务。 |
| 两个cpp各自匿名命名空间的`bool 同一学习条件(const 新方法条件定义&,const 新方法条件定义&)`，私有 | 完整条件值式比较；AND/OR使用is_permutation，其它保持顺序。无事实写入、不公开新服务；分别被所属owner/service调用。 |
| `int 运行模拟学习闭环自检() noexcept`，业务/自检.模拟学习闭环.cpp，已有正式自检入口 | 扩展非空追加、真实重复求值、匹配ID、冲突、非法输入及旧副本保护断言；不成为生产入口。 |
| `新方法条件结果定义 承载测试配对(const 新服务值结算登记材料&,整数,整数)`，同自检cpp匿名空间，私有 | 只构造owner承载测试定义，不执行本能、不伪造模拟证据。 |

函数签名、owner生命周期和学习线程合并/确认合同保持；退出的是“任意已有配对即重复”分支及旧含糊枚举，不退出公开函数。

## 7. 输出清理与残留

本轮进程已退出，无MSBuild或项目进程。已核验精确目录`D:/TEMP/海中鱼巣/SIMULATION-LEARNING-CONTINUE/20261011-v0.1`，68文件、268290062字节（约256MiB）。按规则尝试删除该本轮目录，工具策略拒绝且未执行；没有更换工具绕过。随后只读确认目录仍存在，清理待当前交互所有者后续处理。包含实际工程产物/中间件/构建日志，无源码副本；不评价、不删除其它任务历史残留。未删除任何用户数据。
