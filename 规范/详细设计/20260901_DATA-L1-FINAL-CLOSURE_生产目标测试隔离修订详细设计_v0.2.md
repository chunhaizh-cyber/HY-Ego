# DATA-L1-FINAL-CLOSURE 生产目标测试隔离修订详细设计 v0.2

日期：2026-09-01

状态：执行后具名退回修订设计；设计包发布并精确登记后可执行

## 1. 修订来源与结论

本设计只处理 `DATA-L1-FINAL-CLOSURE` 候选提交 `63849adaa916ef69123d3e9b7c3c2c43069d593f` 经独立集成验收形成的具名失败：

```text
验收记录提交 = 63bc332bdadb6620c949101358606065beafc590
验收记录路径 = 验证记录/集成验收/DATA-L1-FINAL-CLOSURE/**
结构化结论 = INTEGRATION-ACCEPTANCE-FAIL
失败门禁 = ACCEPTANCE-01 第6节“生产目标零检查专用代码”
```

候选的 43 项冻结公开操作、五个冻结文件、三类事实恢复、载荷格式 1 / 2 / 3、严格规范检查、Debug / Release 构建和生产外统一 driver 已取得通过证据。本次失败不要求改变 4015、4070、4080、L1 DTO、服务、仓库、fixture 或两个专项源码；它证明 v0.1 对生产目标隔离及其机器防回流门禁的施工范围不足。

最小 PASS 范围固定为：

1. 生产 `海中鱼巣/入口.cpp` 不再 import、识别或分派任何测试入口；
2. 生产 `海中鱼巣.vcxproj` 不再登记现有全部 9 个 `端到端测试.*` 编译项；
3. `海中鱼巣.vcxproj.filters` 同步删除同一 9 个项目显示项；
4. 9 个测试源码保留在仓库但不属于生产目标；
5. DATA-L1 两个专项只由仓库外临时项目机械加入和运行；
6. 既有冻结检查器永久拒绝入口测试开关及根工程 / filters 的任意 `端到端测试.*` 登记回流。

仅移除两个 L1 专项不满足 ACCEPTANCE-01；其余七个测试模块即使当前入口不可达，也仍是生产工程中的检查专用工程项。

DATA-L1 通用操作 / 目标公开合同继续为 `FROZEN`，整体能力继续为 `PENDING_ACCEPTANCE`。只有新候选提交通过新的独立集成验收并完成第二次治理发布后，整体状态才能升级。

## 2. 当前事实与精确基线

失败记录基线为 `main@63bc332bdadb6620c949101358606065beafc590`；候选与失败记录之间没有修改以下三个生产文件：

| 文件 | 候选 blob | 已证实问题 |
| --- | --- | --- |
| `海中鱼巣/入口.cpp` | `2a3c2cd857a50225d7e0c92dc07889a4c732238f` | import L1 持久恢复专项并暴露 `--test-l1-persistent-recovery` |
| `海中鱼巣.vcxproj` | `9874a5272e4893d1630c7f2b60e5a2fdb07e2a3b` | 登记 9 个 `端到端测试.*` 编译项 |
| `海中鱼巣.vcxproj.filters` | `baa604d4e26b6143c219f089ccacca1450895e37` | 登记对应 9 个显示项 |

当前工作区的根 `vcxproj` 与 `filters` 另有 SELF-GOVERNANCE 未提交修改。它们和本设计删除目标不是同一语义，但共享物理文件。顶层交互智能体已核对当前计划索引、活动任务与所有权：没有其它活动代码所有者主张第 4 节删除 hunk，并在用户明确的 L1 收口目标内接管该精确 hunk。执行者可以只向 index 应用删除补丁，不修改 foreign 工作树内容；若 S0 出现新的所有者或重叠修改，重新进入停止门禁。

## 3. 入口目标形状

`海中鱼巣/入口.cpp` 固定删除：

```text
#include <string_view>
import 海中鱼巣.端到端测试.L1事实基座持久恢复;
识别 --test-l1-persistent-recovery 并调用专项的整个 if 分支
```

删除后 `main` 只执行现行正式流程：

```cpp
int main(int 参数数量, char* 参数组[]) {
    const auto 解析 = 海中鱼巣::解析并验证启动选项(参数数量, 参数组);
    if (!解析.成功()) {
        return 海中鱼巣::映射进程退出码(解析, nullptr);
    }
    const auto 运行 = 海中鱼巣::运行海中鱼巣(解析.选项);
    return 海中鱼巣::映射进程退出码(解析, &运行);
}
```

不得用隐藏宏、环境变量、未记录别名、另一个参数或条件编译保留测试入口。生产入口也不得新增其它测试 / 自检 import。

## 4. 生产工程精确删除集

`海中鱼巣.vcxproj` 必须且只能删除下列 9 个完整 `ClCompile` 项：

```text
海中鱼巣\端到端测试.需求服务.ixx
海中鱼巣\端到端测试.自我治理一致事实.ixx
海中鱼巣\端到端测试.L1事实基座持久恢复.ixx
海中鱼巣\端到端测试.本能路线阶段二.ixx
海中鱼巣\端到端测试.DATA-L1有限N分区原子事务.ixx
海中鱼巣\端到端测试.本能UTC时间证据.ixx
海中鱼巣\端到端测试.安全根定义与当前值.ixx
海中鱼巣\端到端测试.安全根生产定义初始化.ixx
海中鱼巣\端到端测试.任务现实执行硬否决规则宇宙.ixx
```

`海中鱼巣.vcxproj.filters` 必须删除同一路径的 9 个完整 `ClCompile + Filter` 块。不得删除测试源码，不得把测试登记迁到另一生产 `ItemGroup`，不得新建第二生产 exe、仓内测试工程或测试宏。

删除后对根 `vcxproj` 与 `filters` 搜索 `ClCompile Include="海中鱼巣\端到端测试.` 必须零命中；生产 C++ 入口搜索 `import 海中鱼巣.端到端测试.` 和 `--test-` 必须零命中。

## 5. 共享工程文件的所有权和暂存

本设计只取得第 4 节 9 项删除 hunk，不取得根工程与 filters 中其它 SELF-GOVERNANCE 增删改。执行者必须：

1. S0 记录新 HEAD、三个目标文件 blob、异主工作区 diff 和具名 hunk 移交；
2. 在工作树删除目标块时保留其它 WIP 原文；
3. 为 `vcxproj` 与 `filters` 生成相对当前 HEAD、只含第 4 节删除项的显式补丁；
4. 先执行 `git apply --cached --check <补丁>`，再只向 index 应用该补丁；不得对这两个文件执行整文件 `git add`；
5. `入口.cpp` 只有在 S0 仍 clean 且无其它所有者时才可整文件暂存；
6. cached diff 中工程文件只能出现第 4 节删除，工作区其它 WIP 不得进入 index 或精确候选树。

若目标项已改变、删除 hunk 与异主修改重叠、无法只向 index 形成精确差异，或 S0 出现新的所有者，立即停止本切片，不得覆盖、清理、stash、reset 或吸收其它 WIP。

提交删除后，foreign 工作树若仍保留这 9 项，它们会相对新 HEAD 表现为未提交新增。执行者不得改写该 foreign 内容；下节机器门禁必须使任何包含这些回流项的后续候选失败，直到其所有者在自己的切片中保留新 HEAD 的删除。

## 5.1 冻结检查器防回流

代码白名单加入既有 `tools/check_l1_frozen_surface.py`，只扩展 `检查规范与生产边界(...)` 或新增由 `检查冻结面()` 无条件调用的私有检查函数，不改变清单 schema、43 项操作、五文件指纹或状态轴。

检查器固定读取 `海中鱼巣/入口.cpp`、`海中鱼巣.vcxproj` 和 `海中鱼巣.vcxproj.filters`，并增加以下错误条件：

1. 生产入口的有效代码出现任何 `import 海中鱼巣.端到端测试.*;`；
2. 生产入口的有效字符串字面量出现任何 `--test-*` 开关；
3. 根 `vcxproj` 出现路径含 `端到端测试.` 的任意 `ClCompile Include`；
4. filters 出现路径含 `端到端测试.` 的任意 `ClCompile Include`。

注释不形成命中；字符串检查保留字符串字面量但排除注释。文件缺失或无法读取同样失败。现有 `tools/check_specs.py` 已无条件消费 `检查冻结面()`，因此不修改 `check_specs.py`、冻结 manifest 或 4080。

仓库外负例在精确候选副本运行：

- 重新加入入口测试 import、旧 `--test-*` 分支和专项调用，冻结检查器必须非零；
- 重新加入一个 `端到端测试.*` 工程项，分别对 `vcxproj` 与 filters 做两个变体，两次都必须非零。

现有十二类冻结面负例继续通过；本次新增两类后共十四类，其中工程项类含两个文件变体。负例不得修改正式工作树，负例完成后正式精确 index 树检查必须恢复为 0。

## 6. 仓库外统一 driver 临时项目

生产项目删除全部测试登记后，统一黑盒不能再通过替换生产入口而自动取得专项模块。执行者必须在精确 index 导出树中生成临时项目，固定根为：

```text
D:\TEMP\海中鱼巣\DATA-L1-FINAL-CLOSURE\v0.2\<轮次>\
  exact-candidate\
    DATA-L1-FINAL-CLOSURE.unified-driver.vcxproj
    DATA-L1-FINAL-CLOSURE.unified-driver.cpp
```

临时项目的唯一生成算法：

1. 从 index 的精确 tree 导出完整候选到 `exact-candidate`；
2. 复制候选根 `海中鱼巣.vcxproj` 为 `DATA-L1-FINAL-CLOSURE.unified-driver.vcxproj`；
3. 在临时副本中把唯一 `海中鱼巣\入口.cpp` 项替换为 `DATA-L1-FINAL-CLOSURE.unified-driver.cpp`；
4. 在临时副本中只补入两个专项模块：
   - `海中鱼巣\端到端测试.L1事实基座持久恢复.ixx`
   - `海中鱼巣\端到端测试.DATA-L1有限N分区原子事务.ixx`
5. 不复制或修改 filters，不把临时项目、driver、脚本、构建产物和日志加入 Git。

临时 driver 源固定为等价于：

```cpp
import 海中鱼巣.端到端测试.L1事实基座持久恢复;
import 海中鱼巣.端到端测试.DATA_L1有限N分区原子事务;

int main() {
    const int 恢复结果 = 海中鱼巣::运行L1事实基座持久恢复端到端测试();
    if (恢复结果 != 0) {
        return 恢复结果;
    }
    return 海中鱼巣::运行DATA_L1有限N分区原子事务端到端测试();
}
```

生成器必须机械断言临时项目恰好新增两个 `端到端测试.*` 项、生产项目仍为零项、入口替换恰好一次；任一计数不符即失败，不得手工补工程项继续。

## 7. 构建与验证闭环

正式构建与统一黑盒必须来自同一个只含三个生产文件和一个冻结检查器修改的精确代码 index tree。验证完成后才形成两份记录；最终发布 tree 可以只比已验证代码 tree 多这两份记录，四个代码文件 blob 必须逐项相等，并在最终 tree 重跑静态隔离、冻结面、严格规范与 diff 检查：

1. `python .\tools\check_l1_frozen_surface.py --strict` 退出 0；
2. `python .\tools\check_specs.py --strict` 退出 0；
3. `git diff --cached --check` 退出 0，cached 名单只含计划允许文件；
4. 静态隔离扫描及第 5.1 节机器门禁证明生产入口无测试 import / 开关，根 `vcxproj` / filters 无 9 项或任何其它 `端到端测试.*` 登记；
5. 生产根工程以 VS 2022 `v143`、Debug / Release x64、仓库外独立 `IntDir` / `OutDir` fresh Rebuild，均成功；
6. 两配置生产 exe 用旧 `--test-l1-persistent-recovery` 调用时不得运行专项、不得输出专项 PASS，且必须沿正式参数解析返回非成功；
7. 第 6 节临时项目以 Debug / Release x64 fresh Rebuild，两配置统一 driver 均退出 0；
8. 统一 driver 输出并证明 v0.1 已有的三类事实恢复、格式 1 / 2 / 3、v1 / v2 / v3、幂等和 fail-closed 矩阵继续通过；
9. 第 5.1 节新增两类负例及既有十二类负例全部非零，正式精确 index 树复查为 0；
10. 写入并暂存 v0.2 记录后，核对最终 index 中四个代码文件 blob 与已验证代码 tree 相同，并重复第 1—4 项；
11. 新候选提交并普通推送后，由集成验收与回归智能体绑定该精确 commit 重做 `DATA-L1-FINAL-CLOSURE` 验收。

正式生产构建和临时 driver 构建必须使用不同 `IntDir` / `OutDir`，不得复用仓库内 `.codex-build` 或上一轮结果。

## 8. 文件范围

代码执行允许修改：

```text
海中鱼巣/入口.cpp
海中鱼巣.vcxproj                         # 只删第4节9项
海中鱼巣.vcxproj.filters                 # 只删第4节9块
tools/check_l1_frozen_surface.py          # 只增加生产目标测试隔离检查
施工记录/20260901_DATA-L1-FINAL-CLOSURE_施工记录_v0.2.md
验证记录/20260901_DATA-L1-FINAL-CLOSURE_验证记录_v0.2.md
```

禁止修改：9 个测试源码、全部 L1 核心 / 公共 DTO / 服务、冻结 manifest、`check_specs.py`、fixture、4015 / 4070 / 4080、普通装配、其它生产模块、v0.1 记录、详细设计、计划和计划索引。

## 9. 失败收口与声明边界

- 删除测试登记后生产构建失败：先证明是否存在生产代码非法 import 测试模块；不得把测试项加回生产工程。若修复需要白名单外生产文件，具名退回计划支撑。
- 临时项目不能机械加入两个专项：修正仓库外生成逻辑；不得修改测试源码或生产项目取得通过。
- 统一 driver 失败：若事实位于既有专项、L1 核心、工具或 fixture，保持失败并具名退回，不扩大本计划。
- S0 出现新的工程 hunk 所有者、重叠修改或 cached diff 混入异主 WIP：停止发布并保留现场。
- 新独立验收非 `PASS`：DATA-L1 保持 `PENDING_ACCEPTANCE`，不得执行第二次治理发布。

代码候选最多声明：

> DATA-L1 收口候选已移除生产目标中的检查专用入口和全部端到端测试工程项；正式双配置构建与仓库外统一 driver 在同一精确提交树通过。DATA-L1 整体仍为 `PENDING_ACCEPTANCE`。

只有新的独立集成验收 `PASS` 和后续治理发布完成后，才能声明整体 `FROZEN`。
