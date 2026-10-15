# 任务执行组织原子增补施工与编译记录

## S0 与范围

- 计划：`TASK-EXECUTION-ORGANIZATION-APPEND` v0.1，blob `6d6937f03d30c61abad0a08a3649aea70857cf56`；正文 `4478af12`、索引 `c90d7a3a` 已进入 main 并普通推送。
- 代码起点：`c90d7a3ac90f772bf21594d4acf6cbf32e65d33f`，工作树 `D:/worktree/09fc/海中鱼巣`，分支 `codex/passive-causal-generation`；代码所有者 root。
- 修改范围：`核心/基础数据集.h/.cpp`、`领域/新_任务类.h/.cpp`（均在 `海中鱼巣/` 下）及本记录；未改规范、装配、工程或线程。
- 开工检查：index及本树clean；跨工作树四文件无WIP。语言S01切片不涉及上述四文件，main/索引短发布已协调通知并释放。正式规范与旧复制描述的冲突仅按用户已裁决的任务内部组织范围替代。
- 已读执行/计划技能、AGENTS、串行发布/工作树及知识规则、知识入口、0050/4015/3200/3300/5260和施工要求。知识库没有可直接采用的当前函数用途卡，以现行头源及调用点核实。

## 实际变更

1. 基础数据集新增原子追加：新节点组、既有节点追加字段、完整原始结构前置；锁内核验后一次交换节点/字段及双向索引，旧字段和外部关系不修改。
2. task新增增补入口，原建立或取得入口的精确同义合同不变；无组织时共用首次建立路径。
3. 同源M保留旧定义，增加新Pair；新M完整纳入，旧内部调用域可提升为根候选。参数/旧Pair/旧控制定义异义进入工程终止；合法纯新增控制结构整次零写拒绝。
4. 增量序列化将既有占位转为原节点引用，新增引用使用压缩后的组内索引；只允许根成员、根候选和作用域Pair成员追加。
5. 发布前保留原始组织结构；发布后完整图读回，并核对所有旧字段编码、内容及白名单新增字段。未触碰X、当前方法选择、实际输入或学习反馈。

## 验证

- `git diff --check`：通过。独立代码审查完成，未发现可证实P1/P2；核对原始快照、零新节点根候选提升、新旧引用、旧字段不变和整体拒绝。
- 实际项目 `海中鱼巣.vcxproj` Debug/x64 编译链接完成，退出码0；没有编译修复或重复构建。
- 仅编译链接；用户本轮没有测试指令，未启动程序或运行测试。
- 输出资源：`D:/TEMP/海中鱼巣/TASK-EXECUTION-ORGANIZATION-APPEND/20261015-append/`，root独占。

实际命令（工作目录为上述任务工作树）：

```powershell
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe' '海中鱼巣.vcxproj' /t:Build /p:Configuration=Debug /p:Platform=x64 /p:BuildProjectReferences=false '/p:OutDir=D:/TEMP/海中鱼巣/TASK-EXECUTION-ORGANIZATION-APPEND/20261015-append/bin/' '/p:IntDir=D:/TEMP/海中鱼巣/TASK-EXECUTION-ORGANIZATION-APPEND/20261015-append/obj/' /m:1 /nologo /verbosity:minimal /fl '/flp:LogFile=D:/TEMP/海中鱼巣/TASK-EXECUTION-ORGANIZATION-APPEND/20261015-append/build.log;Verbosity=normal;Encoding=UTF-8'
```

## 代码知识变更清单

- 新增公开 `基础数据集::原子追加节点组(const 基础节点组追加请求&) noexcept -> 基础节点组追加结果`：唯一底座结构owner、纯追加、完整节点结构CAS、一次候选交换；资源状态向上送出，内部异常工程终止。task增补入口是本次真实调用方。
- 新增公开 `新_任务类::增补任务方法执行组织(稳定编码,const std::vector<稳定编码>&) noexcept -> 新任务执行组织结果`：只增可选知识，不执行、不重新筹办、不改变X。
- 新增私有 `新_任务类::实施任务方法执行组织(稳定编码,const std::vector<稳定编码>&,bool) noexcept`：两个公开入口共用方法快照、task锁、首次建立/增补及读回；最终诊断责任。
- 修改公开 `建立或取得任务方法执行组织(...)`：委托上述私有实施入口，原公开合同保持。
- 新增文件私有 `任务执行组织存储::合并定义(...)`、`写器::预留(...)`、`写器::追加请求(...)`；修改写器构造和 `读取已加锁(...)`：纯值合并、旧/新引用映射、原始结构输出，工程异常向task最终边界送出。
- 待知识维护者基于最终提交建立/更新上述用途卡；本记录不写共享知识索引。

## 完成边界

本叶已实现并编译，提交事实待补。本叶只提供增量组织能力；W2/W3仍走旧方法复制路径，没有新增生产调度消费者。复杂方法运行位置、参数映射、分支选择、执行反馈身份迁移和服务闭环均未在本叶实现或验证。
