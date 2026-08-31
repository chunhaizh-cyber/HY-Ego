# ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION L2 特征/状态模块实体所有权 ABI 纠正代码实施计划 v0.1

日期：2026-08-31

状态：正式代码实施计划候选；只有本文件与详细设计先发布、取得精确 blob，并由 `计划/计划索引.md` 登记为唯一 `可执行` 后方可施工。

计划身份：`ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION`

冻结起点：`b38059b6b501c49705249f5d5587f6d1f9dba2e9`

## 1. 目标与完成条件

消除特征模块对 `L2状态结构服务` 的第二命名模块所有权，改由特征 owner 的公开窄首值参与者合同支撑状态 owner 的既有原子事务，恢复 v145/v143 Debug/Release x64 fresh Rebuild 和最终链接。

完成条件以同身份详细设计第 1、7、8 节为准。计划可执行、源码编译通过或首个未解析符号变化都不等于完成。

## 2. 正式依据与已验证事实

- 详细设计：`规范/详细设计/20260831_ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION_L2特征状态模块实体所有权ABI纠正详细设计_v0.1.md`；精确 blob 在第一阶段发布后由索引登记前复核；
- CODE-FILE-01 的函数物理位置、知识复核和命名模块实体单一所有权规则；
- 当前 HEAD `b38059b6...`；特征 source blob `f3d22b89056c06ed7502468da65985e86ab01b42`，状态 source blob `f3b661089dfe8c58a8bae6ce4b5da162e0c0a362`；
- 当前工程已登记并生成两模块对象，不是缺 obj；现有 `状态 -> 特征` import 禁止用反向 import 修复；
- Gseed 安全 WIP 已发布于 `b38059b6...`，其运行验证被本 ABI 身份错误阻断；本计划只解除该门禁，不接管 Gseed 源码或测试。

## 3. 允许范围、所有权与禁止项

允许修改：

```text
海中鱼巣/领域/服务.L2特征结构.ixx
海中鱼巣/领域/服务.L2状态结构.ixx
施工记录/20260831_ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION_施工记录_v0.1.md
验证记录/20260831_ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION_验证记录_v0.1.md
```

计划状态的两阶段登记由计划索引所有者另行精确发布，不属于代码执行白名单。

禁止修改：Gseed 四份源码/测试和既有记录、工程/filters、L1、装配、入口、动态发布模块、规范、详细设计、计划、索引、知识库、流程图、其它测试及当前大量异主 WIP。禁止创建模块、第二类声明、反向 import、兼容 typedef、文本 include、假定义或链接器规避项。

目标两份生产源码当前均相对 HEAD clean；执行 S0 若发现任一文件出现未知或异主 dirty、目标语义已变化、index 被占用或活动代码段冲突，只停止本计划并交回所有权协调。

## 4. 冻结接口、文件与实施步骤

详细设计第 3—5 节是唯一施工合同。机械步骤固定为：

1. 在特征模块删除 `class L2状态结构服务;` 和 `friend class L2状态结构服务;`；
2. 仅把既有 `首值原子参与者形成结果`、`形成首值原子参与者写集_v1(...) const noexcept`、`首值编码映射完整_v1(...) noexcept` 放入 `L2特征结构服务` 的公开窄区；函数签名和函数体零语义变化；
3. 紧接三项公开合同后恢复 `private:`，不得公开其它 helper、锁、owner 端口或字段；
4. 在状态模块把事务提交的 `特征服务_.特征实例写入端口_` 改为 `特征服务_.取得组合发布写入端口()`；
5. 不改变既有 `形成首值原子参与者写集_v1` 和 `首值编码映射完整_v1` 两个调用的参数、顺序和失败处理；
6. 形成专属施工/验证记录及知识变更清单，不直接写共享知识库。

## 5. 失败收口

- 普通代码错误：只在两份生产源码白名单内修复；
- 任一函数合同、DTO、owner、事务或 import 与详细设计不一致：停止并具名退回计划支撑；
- 特征私有区无法只公开三项而不扩大其它 ABI：停止，不把整个 private 区改 public；
- v145/v143 工具集缺失：记 `BLOCKED`，不得降级另一工具集；
- 目标状态 ABI 已闭合但出现新首个 LNK：按详细设计 V10 分账；若属于 `L2状态动态原子发布服务` 同形 friend，形成后继候选，不在本计划扩写；
- 发布租约、index、目标文件所有权或验证根冲突：保留现场，只停止本计划。

## 6. 验证命令与成功标准

执行者先枚举 `*.sln`、`*.slnx`、`*.vcxproj`，当前冻结真实根为 `海中鱼巣.vcxproj`。每个配置使用 `D:\TEMP\海中鱼巣\ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION\<run>\<toolset-config>` 的独立 `IntDir/OutDir`，串行 fresh Rebuild：

```powershell
msbuild .\海中鱼巣.vcxproj /t:Rebuild /m:1 /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v145 /p:IntDir=<仓外Debug对象根> /p:OutDir=<仓外Debug输出根>
msbuild .\海中鱼巣.vcxproj /t:Rebuild /m:1 /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v145 /p:IntDir=<仓外Release对象根> /p:OutDir=<仓外Release输出根>
msbuild .\海中鱼巣.vcxproj /t:Rebuild /m:1 /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v143 /p:IntDir=<仓外v143Debug对象根> /p:OutDir=<仓外v143Debug输出根>
msbuild .\海中鱼巣.vcxproj /t:Rebuild /m:1 /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v143 /p:IntDir=<仓外v143Release对象根> /p:OutDir=<仓外v143Release输出根>
```

随后必须完成：

```text
特征文件中 class/friend class L2状态结构服务 零命中
状态文件直接访问 特征实例写入端口_ 零命中
dumpbin /symbols 核对状态类消费/定义修饰名的模块附着身份
模块依赖扫描确认零 特征 -> 状态 反向边
保留并分账首个后继未解析符号
git diff --check
git diff --cached --check
python .\tools\check_specs.py --strict
```

成功要求详细设计 V01—V11 全部 PASS；根 Rebuild 必须形成最终链接成功。任何 `NOT_RUN`、`BLOCKED` 或只编译不链接都不构成计划完成。

## 7. 发布与完成声明

发布时只精确暂存第 3 节四份允许文件，逐 hunk 审计 cached diff；提交与普通推送后复核 `HEAD == origin/main`、ahead/behind `0/0`、index 空，并报告仍存在的异主 dirty。

本计划最多声明模块实体所有权和对应 ABI 链接阻断已纠正。发布后通知交互智能体恢复 Gseed 原计划的运行验证；不得直接把 Gseed 索引改为完成，不得声明阶段三或本能路线完成。
