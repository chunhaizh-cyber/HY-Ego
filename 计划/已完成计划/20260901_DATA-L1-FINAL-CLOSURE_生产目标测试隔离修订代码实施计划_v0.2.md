# DATA-L1-FINAL-CLOSURE 生产目标测试隔离修订代码实施计划 v0.2

日期：2026-09-01

计划身份：`DATA-L1-FINAL-CLOSURE`

修订版本：`v0.2`

目标登记状态：设计包发布并以精确 blob 登记后为 `可执行`

## 1. 退回来源与目标

本计划由候选 `63849adaa916ef69123d3e9b7c3c2c43069d593f` 的独立验收失败修订：

```text
验收记录提交：63bc332bdadb6620c949101358606065beafc590
验收路径：验证记录/集成验收/DATA-L1-FINAL-CLOSURE/**
结构化结论：INTEGRATION-ACCEPTANCE-FAIL
违反门禁：ACCEPTANCE-01 生产目标零检查专用代码
```

目标是不改变 L1 冻结合同和已完成实现，只把全部测试代码从生产入口与生产工程中隔离，并在仓库外临时项目继续运行两个 L1 专项。完成后形成新的待独立验收候选；DATA-L1 整体保持 `PENDING_ACCEPTANCE`。

完成必须同时满足：

1. 生产入口无测试 import、测试参数、测试分派或替代别名；
2. 根 `vcxproj` 与 filters 对全部 9 个 `端到端测试.*` 项零登记；
3. 9 个测试源码保留且内容不变；
4. 冻结检查器使入口测试分支或任一生产工程测试登记回流都稳定失败；
5. 精确 index 树的 Debug / Release x64 正式根工程 fresh Rebuild 通过；
6. 旧 `--test-l1-persistent-recovery` 不再运行专项；
7. 仓库外临时项目只加入两个 L1 专项并替换临时入口，两配置统一 driver 通过；
8. cached diff 只含本计划白名单，不含 SELF-GOVERNANCE WIP；
9. 新候选提交、推送并经独立集成验收重新裁决。

## 2. 正式依据与基线

- `规范/运行期服务导向代码验收与全函数入口巡检规范.md`，工程治理标识 `ACCEPTANCE-01`
- `规范/4080_子规范_DATA-L1通用操作全集与冻结门禁.md` v1.0
- `规范/详细设计/20260901_DATA-L1-FINAL-CLOSURE_通用操作冻结与统一黑盒收口详细设计_v0.1.md`
- `规范/详细设计/20260901_DATA-L1-FINAL-CLOSURE_生产目标测试隔离修订详细设计_v0.2.md`
- `验证记录/集成验收/DATA-L1-FINAL-CLOSURE/**` @ `63bc332bdadb6620c949101358606065beafc590`

计划支撑扫描基线：`main@63bc332bdadb6620c949101358606065beafc590`，与 `origin/main` 一致。v0.1 文件保留为历史形成材料；计划索引登记 v0.2 后，执行只能读取 v0.2 精确 blob，不能继续使用 v0.1 白名单。

无代码提供者依赖。顶层交互智能体已核对当前计划索引、活动任务和所有权，在用户明确的 L1 收口目标内接管根 `vcxproj` / filters 的第 4 节精确删除 hunk；其它 SELF-GOVERNANCE WIP 不在本计划所有权内。S0 若出现新的重叠所有者，重新停止受影响切片。

## 3. 允许文件

生产文件：

```text
海中鱼巣/入口.cpp
海中鱼巣.vcxproj
海中鱼巣.vcxproj.filters
```

工程治理工具：

```text
tools/check_l1_frozen_surface.py
```

专属记录：

```text
施工记录/20260901_DATA-L1-FINAL-CLOSURE_施工记录_v0.2.md
验证记录/20260901_DATA-L1-FINAL-CLOSURE_验证记录_v0.2.md
```

其中根 `vcxproj` 和 filters 只允许删除第 4 节列出的 9 项；不取得整个文件的其它 hunk。检查器只允许增加第 4.3 节生产目标测试隔离检查。仓库外临时项目、driver、补丁文件、构建中间件、程序和日志固定放在 `D:\TEMP\海中鱼巣\DATA-L1-FINAL-CLOSURE\v0.2\<轮次>`，不得进入 Git。

## 4. 精确修改合同

### 4.1 生产入口

从 `海中鱼巣/入口.cpp` 删除：

- `#include <string_view>`；
- `import 海中鱼巣.端到端测试.L1事实基座持久恢复;`；
- `--test-l1-persistent-recovery` 判断及专项调用三行分支。

保留启动选项解析、解析失败映射、`运行海中鱼巣` 和正式结果映射原样。不得新增替代测试入口。

### 4.2 根工程和 filters

从 `海中鱼巣.vcxproj` 删除以下 9 个完整 `ClCompile` 单行项，并从 filters 删除同一路径的 9 个完整三行块：

```text
端到端测试.需求服务.ixx
端到端测试.自我治理一致事实.ixx
端到端测试.L1事实基座持久恢复.ixx
端到端测试.本能路线阶段二.ixx
端到端测试.DATA-L1有限N分区原子事务.ixx
端到端测试.本能UTC时间证据.ixx
端到端测试.安全根定义与当前值.ixx
端到端测试.安全根生产定义初始化.ixx
端到端测试.任务现实执行硬否决规则宇宙.ixx
```

路径前缀固定为 `海中鱼巣\`。测试源码不得删除、移动、改名或修改。

### 4.3 冻结检查器

修改 `tools/check_l1_frozen_surface.py`，在 `检查冻结面()` 必经路径增加生产目标测试隔离检查：

- `海中鱼巣/入口.cpp` 有效代码中的任意 `import 海中鱼巣.端到端测试.*;` 失败；
- 入口有效字符串字面量中的任意 `--test-*` 失败；
- 根 `海中鱼巣.vcxproj` 或 filters 中任意路径含 `端到端测试.` 的 `ClCompile Include` 失败；
- 三个文件缺失或不可读失败。

注释不产生命中。不得修改 manifest schema、冻结指纹、43 项操作、状态字段、`check_specs.py` 或 4080；现有严格规范入口已经自动消费该检查。

## 5. 禁止范围

禁止修改：

- 9 个 `海中鱼巣/端到端测试.*` 源码；
- L1 核心、服务、公共数据头、冻结 manifest、`check_specs.py` 和 fixture；
- 4015、4070、4080、规范目录及其它规范；
- 启动 / 装配、L2 / L3、业务、线程和 SELF-GOVERNANCE 文件；
- v0.1 施工 / 验证记录、v0.2 详细设计、计划正文和计划索引。

禁止建立仓内测试工程、第二 exe、测试宏、隐藏生产开关或新测试入口。禁止用整文件暂存根 `vcxproj` / filters 带入异主 WIP。

## 6. S0 门禁

执行前必须全部满足：

1. 当前唯一工作区、`main`、HEAD、`origin/main`、ahead / behind 和 index 已核对；
2. 计划索引登记身份 `DATA-L1-FINAL-CLOSURE`、版本 v0.2、精确 plan blob、状态 `可执行`、具名依赖无，并引用 v0.2 设计发布提交；
3. `入口.cpp` 无其它所有者和未提交差异；
4. 顶层交互对第 4.2 节删除 hunk 的接管仍有效，且没有新活动所有者；现有其它 WIP 的路径和语义已记录；
5. `tools/check_l1_frozen_surface.py` 无其它所有者和未提交差异；
6. 当前 HEAD 的三个生产目标文件仍含第 4 节精确旧形状；若 blob 已变化，逐项重读并证明删除合同仍唯一，不能直接沿用旧行号；
7. 候选 `63849adaa...` 与失败记录 `63bc332b...` 均可读取，验收失败仍唯一指向生产目标隔离；
8. VS 2022 `v143`、仓库外 `D:\TEMP` 验证根和精确 index 导出可用。

任一所有权、目标项或正式合同漂移立即停止；不得先改再补交接。

## 7. 实施步骤

1. 按第 4.1 节删除生产入口测试 include、import 和分派分支。
2. 在工作树只删除第 4.2 节根工程与 filters 项，保留其它所有者 WIP 原文。
3. 为两个共享工程文件生成相对 HEAD、只含目标删除的显式 patch；用 `git apply --cached --check` 与 `git apply --cached` 只写 index，禁止整文件 `git add`。
4. 在检查器必经路径增加第 4.3 节回流检查，并精确暂存 clean 的 `入口.cpp` 与检查器；检查此时 cached 名单只有四个代码文件，工程 hunk 只有目标删除。
5. 从 index tree 导出完整候选到仓库外根；该树不得包含其它工作区 WIP。
6. 在导出树复制生产项目为 `DATA-L1-FINAL-CLOSURE.unified-driver.vcxproj`，把唯一正式入口项替换为临时 driver，并只补入两个 L1 专项模块。
7. 运行静态隔离检查、严格规范检查、冻结面检查、现有十二类负例、本次两类负例、正式双配置 fresh Rebuild、旧开关负向运行、临时 driver 双配置 fresh Rebuild 和统一黑盒。
8. 写入并暂存 v0.2 施工 / 验证记录，绑定代码 tree、命令、退出码、生产项目零测试项和临时项目恰两项证据；核对最终 index 中四个代码文件 blob 与已验证代码 tree 相同，再重跑静态、冻结面、严格规范和 diff 检查。
9. 取得唯一发布租约，只提交第 3 节 cached 文件并普通推送；提交后确认共享工程文件只剩异主 WIP。
10. 交互智能体将新提交交给集成验收与回归智能体重做具名验收；执行者不得自行升级整体状态。

## 8. 临时项目构造

在精确 index 导出树根执行等价机械变换：

```text
复制 海中鱼巣.vcxproj
-> DATA-L1-FINAL-CLOSURE.unified-driver.vcxproj

替换唯一：海中鱼巣\入口.cpp
-> DATA-L1-FINAL-CLOSURE.unified-driver.cpp

只新增：
海中鱼巣\端到端测试.L1事实基座持久恢复.ixx
海中鱼巣\端到端测试.DATA-L1有限N分区原子事务.ixx
```

临时 driver 依次调用：

```text
海中鱼巣::运行L1事实基座持久恢复端到端测试()
海中鱼巣::运行DATA_L1有限N分区原子事务端到端测试()
```

第一项非零立即返回；两项都成功才返回 0。生成后机械断言生产项目测试项数为 0、临时项目测试项数为 2、入口替换次数为 1。临时项目不使用 filters，不改正式源码。

## 9. 验证矩阵

### 9.1 静态和 Git

```powershell
python .\tools\check_l1_frozen_surface.py --strict
python .\tools\check_specs.py --strict
git diff --cached --check
```

另以精确搜索断言：

- 生产 `入口.cpp` 对 `端到端测试`、`--test-` 零命中；
- 根 `vcxproj` / filters 对 `ClCompile Include="海中鱼巣\端到端测试.` 零命中；
- 临时项目对同模式恰好 2 项；
- 9 个测试源码与候选 `63849adaa...` blob 完全一致；
- cached 工程 diff 只有 9 项 / 9 块删除，无其它 WIP。

仓库外冻结检查器负例：

1. 在候选副本重新加入入口测试 import、旧 `--test-*` 分支和专项调用，检查器非零；
2. 在候选副本重新加入一个 `端到端测试.*` 工程项，分别注入 `vcxproj` 与 filters，两种变体都非零。

现有十二类负例继续非零，本次总计十四类；负例结束后正式精确 index 树的冻结检查器和严格规范检查都必须恢复为 0。

### 9.2 正式生产构建

从精确 index tree，以 VS 2022 `v143`、Debug / Release x64 和独立 `IntDir` / `OutDir` fresh Rebuild 根 `海中鱼巣.vcxproj`。两配置编译、链接退出 0。

分别运行：

```text
海中鱼巣.exe --test-l1-persistent-recovery
```

必须不输出 L1 专项 PASS、不调用测试函数，并沿正式参数解析返回非成功。不得把该旧字符串恢复为受支持参数。

### 9.3 仓库外统一黑盒

临时项目 Debug / Release x64 fresh Rebuild 和运行均退出 0，继续覆盖：

- 节点 / 关系 / 值当前、历史、墓碑和恢复；
- 属性槽、owner、事实代次、幂等、一致投影；
- v1 / v2 / v3 事务；
- 载荷格式 1 / 2 / 3；
- 清单、槽、长度、摘要、未知格式 / 标签、存储占用和非活动槽 fail-closed。

### 9.4 独立验收

代码提交并推送后必须形成新的验收轮次和记录，候选 commit 必须与远端一致。旧 `INTEGRATION-ACCEPTANCE-FAIL` 保留，不得覆写；只有新结论 `PASS` 才能进入第二次治理发布。

## 10. 失败收口

- 生产构建因移除测试模块失败：不得加回工程项；若存在白名单外生产代码 import 测试模块，具名退回计划支撑。
- 临时项目构造计数不符：修复仓库外生成步骤，不改生产项目或测试源码。
- 统一黑盒暴露 L1 / fixture / 工具问题：保持失败并退回，不扩大本工程隔离计划。
- cached diff 混入异主 WIP：停止提交，恢复精确 index patch；不得清理工作树。foreign 工作树若保留 9 项，冻结检查器必须使其无法作为后续候选通过严格门禁。
- push 前远端前进：重新 S0 和 staged 树验证，不自动 merge / rebase。
- 新独立验收失败或必要项 `NOT_RUN`：整体保持 `PENDING_ACCEPTANCE`。

## 11. 发布与声明

代码提交只允许第 3 节实际文件。发布后复核 `HEAD == origin/main`、index 空、入口和本轮记录 clean，根工程 / filters 的异主 WIP仍原样存在且不含本轮已提交删除。

本计划代码结果最多证明：

> 生产目标已移除检查专用入口和全部端到端测试工程项，正式双配置构建与仓库外 DATA-L1 统一 driver 在同一精确提交树通过；DATA-L1 整体仍为 `PENDING_ACCEPTANCE`。

不得证明独立验收已通过或 DATA-L1 整体已 `FROZEN`。
