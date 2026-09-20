# GOAL-IFR-FEATURE-IDENTITY-CONSUMER-REPAIR

## 1. 目标身份

- 目标编号：`GOAL-IFR-FEATURE-IDENTITY-CONSUMER-REPAIR`
- 名称：IF/R 特征身份消费者类型修复
- 类型：代码实施目标（直接局部代码切片）
- 记录版本：v0.1
- 创建与更新时间：2026-09-20
- 当前状态：当前
- 直接上位目标：`GOAL-INSTINCT-RUNTIME`
- 直接下位目标：无；完成后解除`GOAL-INSTINCT-ROOT-DEMAND-PROVIDER-IMPLEMENTATION`的根工程编译阻断
- 目标账所有者：交互智能体；代码切片所有者在S0通过后取得

## 2. 目标结果与完成条件

修复 IF/R 观察服务中唯一的强类型消费者失配：I64 出生结果的裸特征编码必须显式形成`特征信息身份`，才能作为`构造目标`的可选新增 F 成员传递。

完成条件：唯一允许源码和专属施工/验证记录已发布；差异检查通过；隔离根工程 Debug Rebuild 不再出现 C2664，且没有由本行转换引发的新编译失败。结果不声明 IF/R 行为、根材料 provider、Release 构建、运行、恢复或集成通过。

明确排除：公开 DTO、函数签名、IF/R 读写/事务、I64 出生 provider、存在类、需求服务、普通应用、根材料 WIP、工程登记、阶段21与正式规范。

## 3. 直接代码切片声明

- 起点 HEAD：`9597075a49f2dcc9e50eadf6c0d04d3216adbc40`。
- 唯一代码所有者：当前交互智能体；`海中鱼巣/业务/应用服务.特征概念类.cpp`当前无工作区改动。
- 允许生产文件：`海中鱼巣/业务/应用服务.特征概念类.cpp`，仅第602行向`构造目标`的第四参数传递处；允许专属记录为`施工记录/IFR-FEATURE-IDENTITY-CONSUMER-REPAIR/**`和`验证记录/IFR-FEATURE-IDENTITY-CONSUMER-REPAIR/**`。
- 冻结不变合同：`构造目标`保持`optional<特征信息身份>`；I64 出生结果、IF/R DTO、函数签名、读写、事务、失败映射、所有权和生命周期均不变。
- 实施：将`out.F出生->事实->F`显式包装为`特征信息身份{out.F出生->事实->F}`；不得增加隐式转换、改变参数类型或改动相邻分支。
- 验证：`git diff --check`、`git diff --cached --check`；复用`D:\TEMP\海中鱼巣\ARCH-L4-INSTINCT-ROOT-DEMAND-PROVIDER\build-debug-rerun\`以相同 Debug x64 配置 Rebuild 根工程，记录首错或成功；不得占用仓库`Debug/`。

## 4. 状态证据上下文

- 状态事实截止：`9597075a49f2dcc9e50eadf6c0d04d3216adbc40`
- 关联计划：无；本目标满足直接局部代码切片门禁，不进入`计划/计划索引.md`。
- 当前证据：隔离 Debug Rebuild 于2026-09-20失败，唯一错误 C2664 位于`应用服务.特征概念类.cpp:601/602`；`构造目标`第471行的参数类型是`optional<特征信息身份>`，`原子I64特征出生事实.F`在`数据服务.定位特征.h:78`是`稳定编码`。
- 当前阻断：无；语义、物理位置和唯一改动由现行代码及编译器错误唯一裁决。
- 后继：本目标发布并完成规定 Debug Rebuild后，根材料目标按新正式HEAD自动重新S0；若出现不同于本行转换的错误，不在本切片处理。
