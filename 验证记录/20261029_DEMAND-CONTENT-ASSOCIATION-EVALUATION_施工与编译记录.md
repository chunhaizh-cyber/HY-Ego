# 整体需求目标关联判定：施工与编译记录

日期：2026-10-29
标识：DEMAND-CONTENT-ASSOCIATION-EVALUATION

## S0及范围

- 工作区：D:/海中鱼巣，main；起点 HEAD `8d00098c`（冻结设计/计划及本项索引），origin/main `c3b12234181fac0da3eb0224c36a14c239df4355`。旧09fc工作树不写。
- 计划 v0.1 blob `7de47683d5e54b383ee13554174d5ea27e6999c0`；详细设计 blob `0e0c02d5ec0ab348c2bf97ca0b3f38cdd059314f`。
- root 唯一两源、Git/index及构建资源 owner。原联合 WIP 保留。起点 h blob `82a3aeab933784fbe41a2448f0e6467d9d5a2968`，cpp blob `efeae44d8d33aabfdfea4dac96d9d9a93e72eaf3`；修改前核同，index空。
- 只写服务对象需求.发布服务.h/.cpp及本记录。不改公开DTO、需求/任务/方法、初始化、学习、候选或工程；不改严格当前读口。
- 本叶只读，不修改虚拟/现实存在的特征值，不生成动作、状态、动态。当前无运行测试指令；只实际工程编译链接。
- 复用：既有找特征完整扫描、存在/场景及FC正式读口、需求身份reader和本cpp联合保护helper。不使用依赖最新状态的世界写前置，不使用尚不支持需求引用的F完整读口。

## 实际修改

两源实现 private 整体关联reader，并由求值内容内部的整体空路径分支实际调用。捕获主体全部持有F及FC定义、世界根/场景父链、当前D身份；末尾重核定位与完整保护。无F、current空（允许历史保留）或current为另一同主体被认知D均为未关联，继续指定D内容判定；仅内容符合且已关联才整体已满足。纯内容入口不要求F；单字段及严格当前结构读取实现未改。

静态分支读回（不是运行测试）：

| 情况 | 实际分支 |
| --- | --- |
| 无新/旧F | 完整负事实保护下返回已读取、未关联、F空 |
| 只有旧F | 完整复核后未支持，未迁移 |
| current空且历史保留 | 不调用历史选择或状态读取，未关联，保留真实F |
| current为指定D | 同主体/被认知身份合法后关联成立 |
| current为另一同主体被认知D | 仅关联为假，不改选D内容 |
| 错误current类型、主体或执行域 | 先检查保护，稳定损坏进入原工程故障链 |
| E/F/FC/场景父链/当前D捕获后变化 | 合并冲突或末尾复核返回前提变化 |
| 未关联且指定D缺值/分支待定 | 原必要项目及分支保留，原非成功优先级不变 |
| 未关联且指定D符合 | 整体未满足，不冒充父目标完成 |

## 代码知识变更清单

1. 新增 private `服务对象需求发布服务::整体目标关联读取结果` 和 `整体目标关联读取结果 读取整体目标关联(稳定编码 场景节点, 稳定编码 主体节点, 稳定编码 指定需求节点, const std::vector<基础联合节点快照>& 初始保护) const`。h声明/cpp定义；只由既有求值内容内部的整体当前关联分支调用。组合owner公开读口与中性联合保护，区分未关联与漂移；无事实写入，不新增占用，不持跨owner锁，异常由原公开求值处理。
2. 修改 private `新引用内容目标判定 求值内容内部(const 新需求完整目标&, bool 要求当前引用) const noexcept`：整体当前关联改用上述helper；未关联继续指定D内容判断，仅将内容符合的整体已满足降为未满足。纯内容和非空路径原义保持；公开调用边不变。
3. 无退出函数；公开DTO、严格读取当前需求结构内容/实现::读取当前结构保持不变。待更新两函数用途卡；共享知识索引不写，未提交生产WIP不能作为已发布知识。

## 验证与发布边界

实际项目 `D:/海中鱼巣/海中鱼巣.vcxproj`，main `8d00098c55b78f23485cef43bc1ad2bfb6482aa8` 加 root 联合生产 WIP。2026-10-29 Debug/x64 编译链接退出码0；日志记录16.12秒、0警告、0错误。产物 `D:/TEMP/海中鱼巣/INNATE-CONCEPT-GROUP-READ/20261028-optimization/bin/海中鱼巣.exe`，36122624字节，修改时间14:47:22。没有启动或运行测试。

```powershell
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe' 'D:/海中鱼巣/海中鱼巣.vcxproj' /t:Build /m /p:Configuration=Debug /p:Platform=x64 /p:OutDir=D:/TEMP/海中鱼巣/INNATE-CONCEPT-GROUP-READ/20261028-optimization/bin/ /p:IntDir=D:/TEMP/海中鱼巣/INNATE-CONCEPT-GROUP-READ/20261028-optimization/obj/ /v:minimal /nologo /fl '/flp:logfile=D:/TEMP/海中鱼巣/INNATE-CONCEPT-GROUP-READ/20261028-optimization/demand-content-association-build.log;verbosity=normal'
```

编译时h blob `2b7a3d4b66b8a0789a8e9123204a4256fcd11cae`，cpp blob `6f9630fbf556838ba2a4751732333b1e6cc6f0b2`。两源diff --check及cached --check通过（只有既有换行提示）。设计/计划/本项索引精确本地提交 `8d00098c`；生产源码及本记录不提交、不推送；不推送既有main提交。

独立只读审查 `/root/demand_skeleton_audit`：当前增量未发现P1/P2阻断，逐项核对负事实、实际父链、current空及D0身份、false继续内容判定、严格口/纯内容隔离、错误与资源边界；未构建或运行，不外推到全体联合WIP。本叶完成后仅退出本项索引并删除已完成计划文件；设计保留，完整计划可由提交 `8d00098c` 追溯。代码仍为联合WIP。

只读语义修复不证明首次G/D来源、参数绑定、F→D知识、模拟学习、方法执行或服务闭环。内容完整未挂接时，既有复合提议仍可能生成空子目标组；后续方法候选及执行是独立未实现缺口。
