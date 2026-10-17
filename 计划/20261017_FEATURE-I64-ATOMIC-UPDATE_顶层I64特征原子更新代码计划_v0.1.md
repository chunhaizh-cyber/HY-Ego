# FEATURE-I64-ATOMIC-UPDATE 顶层I64特征原子更新代码计划

日期：2026-10-17；版本：v0.1；状态只由计划索引维护。

## 1. 目标与合同

基线`a67e5e8379b1942069fa0f5f61908502f61243ea`，工作树`D:/worktree/task-causal-preparation/海中鱼巣`，分支`codex/task-causal-preparation`。

将既有`添加特征值`的顶层I64分支改为不可变准备、单次L1核心发布和一次原算法收尾；保持普通首次/重复/饱和及分组语义，以真实生产消费者闭合提供者。新L1能力只封存/组合现有中立写集，不加第二事务引擎。服务消费owner及runtime接线不在本计划。

实施合同读取[详细设计](../规范/详细设计/20261017_FEATURE-I64-ATOMIC-UPDATE_顶层I64特征原子更新详细设计_v0.1.md) v0.1全部，以及其具名引用的[消费设计输入§4.5](../规范/详细设计/20261017_SERVICE-CONTRIBUTION-CONSUME_服务值Q原子消费与收尾设计输入_v0.1.md)。其它消费输入段落不授权本叶施工。上游L1字段删除已由`f8db7cc9`发布，无未实现上游依赖。

## 2. 所有权、允许与禁止

root唯一顶层代码、构建及发布所有者，可在本计划冻结ABI下分配不重叠的核心/feature文件子切片；同计划整体仍算一个代码切片。构建/发布由root统一串行，子智能体不独立改变合同。

允许文件仅为：

- `海中鱼巣/核心/基础数据集.h/.cpp`：sealed片段、sealed回执、一次组合发布和所需私有技术帮助函数；旧追加事务继续为唯一候选交换实现。
- `海中鱼巣/领域/新_特征类.h/.cpp`：顶层I64准备/提交/完成/结束/取消，私有短锁占用、局部等价规范化、共享standard原子维护、原I64一次收尾及现有添加值生产消费；删除入口全子树占用预检。
- `验证记录/20261017_FEATURE-I64-ATOMIC-UPDATE_施工与编译记录_v0.1.md`：本叶实际改动、复核、编译、未验证边界及函数知识变更清单。

禁止修改task/self/manager/runtime、服务值算法、世界/状态/动态/因果、概念公开合同、正式规范、工程登记、共享知识索引和其它所有者WIP；不新增生产文件，不新增自检。不将直接子F/非I64正常处理迁到新算法，不做全仓历史迁移、持久恢复或任意半写修复。

## 3. 施工顺序

1. S0核对精确计划/设计、HEAD/index、文件/语义及构建所有权；查当前知识与调用点，旧能力优先复用。
2. 按详细设计§3冻结L1头公开声明，所有者确认后核心与feature可在不同文件并行实施；不让feature猜回执/封存布局。
3. 实现中立封存及组合：完整快照、冲突、局部偏移、发布前全部分配、唯一调用既有原子追加、封闭回执。
4. 实现feature准备和工程占用、局部旧历史转换、真实共享记录standard修正、空核心只读预期复核、取消/析构及一次收尾；保留原派生规则。父/直接子旧direct各按自身owner规范化，保留子独立历史；同值或次数相等不推定共享；接收组内按值复用自有记录，父覆盖按值、共享一致性按record，standard/聚合使用各子完整历史。不同子组独立同值不作为冲突，不改旧选组优先级、不合并区间。
5. 接入真实`添加特征值`顶层I64分支，检查所有公开写入口不会穿透占用；删除在首写前预检全子树。
6. 独立复核详细设计§8矩阵，完成一组相关改动后统一实际项目编译。纯工程错误原范围修；合同漂移具名返回，不现场发明恢复/分组算法。

## 4. 验证、发布与完成声明

运行`git diff --check`及`git diff --cached --check`，读回新增文档路径和链接。实际工程命令：

```powershell
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe' '海中鱼巣.vcxproj' /t:Build /p:Configuration=Debug /p:Platform=x64 '/p:OutDir=D:/TEMP/海中鱼巣/FEATURE-I64-ATOMIC-UPDATE/20261017-code/Debug/bin/' '/p:IntDir=D:/TEMP/海中鱼巣/FEATURE-I64-ATOMIC-UPDATE/20261017-code/Debug/obj/' /m:2 /nologo /verbosity:minimal /fl '/flp:logfile=D:/TEMP/海中鱼巣/FEATURE-I64-ATOMIC-UPDATE/20261017-code/Debug/build.log;verbosity=normal;Encoding=UTF-8'
```

成功条件为具名合同实现、独立静态复核无阻断和实际工程编译链接0错误。未获本轮测试指令，不运行测试/程序，不新增临时工程。只提交本叶允许文件和证据，由root普通发布；记录“已编译，未运行测试”。发布不证明运行、并发、故障恢复、服务贡献消费、因果筹办或内部治理业务闭环。
