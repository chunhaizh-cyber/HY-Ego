# L1-ATOMIC-APPEND-FIELD-DELETE 原子追加字段删除代码计划

日期：2026-10-17；版本：v0.1；状态只由计划索引维护。

## 1. 目标、依据和起点

代码起点`15e58ebfa1b1315376c8cafe540cecbcf9846de9`。按0050与4015组合原子发布合同，扩展现有原子追加请求，支持指定既有字段关系删除与新节点等变化同次提交。详细冻结合同见[原子追加字段删除详细设计](../规范/详细设计/20261017_L1-ATOMIC-APPEND-FIELD-DELETE_原子追加字段删除详细设计_v0.1.md)。

必要性为当前合法I64旧直接历史值向新命中节点转换需要同次新建+删除；现有两个L1入口不可串接冒充原子。它是服务值Q联合消费的最早提供者，不是完整消费者计划，也不提前登记feature后继。

## 2. 所有权及范围

唯一代码所有者root；工作树`D:/worktree/task-causal-preparation/海中鱼巣`，分支`codex/task-causal-preparation`。root同时独占本叶实际工程编译及Git发布。

允许文件：

- `海中鱼巣/核心/基础数据集.h`：在`基础节点组追加请求`尾部追加`std::vector<稳定编码> 删除字段关系组`，调整对应注释。
- `海中鱼巣/核心/基础数据集.cpp`：仅`原子追加节点组`及其本叶必要私有helper，实现删除输入校验、候选字段/双索引删除、混合事务及纯删除。
- `验证记录/20261017_L1-ATOMIC-APPEND-FIELD-DELETE_施工与编译记录_v0.1.md`：真实施工/编译证据、未验证边界和知识变更清单。

禁止修改其它生产代码、工程/装配入口、正式规范、共享知识索引及其它owner切片；不改变`原子比较变更字段组`公开或业务行为，不新增节点/外部边删除，不改现有生产调用为非空删除，不迁移任何旧历史，不开通最终服务值消费。

## 3. 施工及失败收口

1. S0确认当前HEAD、允许范围、无同文件/语义或构建资源冲突，重读详细设计与现有调用点。
2. 仅尾部新增请求成员，验证所有旧聚合初始化保持可编译且默认删除组为空。
3. 完整冻结纯删除/混合、删改互斥、完整联合快照、旧引用端点和无字段/预期变化语义；删除组空时保持旧路径。
4. 在候选中删除字段及双索引，沿用唯一最终交换点和异常保障，不在正式表上做前置删除。
5. 按详细设计§6完成独立静态复核，统一编译链接。纯工程错误在本范围修复；会改变合同的漂移回交计划支撑，不扩大L1公共职责。

## 4. 验证与发布边界

文档/代码差异检查：`git diff --check`及`git diff --cached --check`。实际项目命令在工作树根运行：

```powershell
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe' '海中鱼巣.vcxproj' /t:Build /p:Configuration=Debug /p:Platform=x64 '/p:OutDir=D:/TEMP/海中鱼巣/L1-ATOMIC-APPEND-FIELD-DELETE/20261017-code/Debug/bin/' '/p:IntDir=D:/TEMP/海中鱼巣/L1-ATOMIC-APPEND-FIELD-DELETE/20261017-code/Debug/obj/' /m:2 /nologo /verbosity:minimal /fl '/flp:logfile=D:/TEMP/海中鱼巣/L1-ATOMIC-APPEND-FIELD-DELETE/20261017-code/Debug/build.log;verbosity=normal;Encoding=UTF-8'
```

成功条件为本叶范围的代码/独立静态复核完成、实际工程编译链接0错误，并形成专属记录。未获明确测试指令，不启动程序、运行测试或新建测试载体；报告“已编译，未运行测试”。

只提交本叶允许文件与专属证据，普通推送由root执行。完成只证明L1提供者代码及编译，不证明删除运行、并发、故障恢复、feature联合消费、服务值更新或因果筹办闭环。
