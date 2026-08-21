# THREAD-LIFECYCLE-PROJECTION-SERVICE 线程生命周期投影服务代码实施切片 v0.1

日期：2026-08-15

状态：交互目标直接实施完成；计划索引未登记，本文件不构成队列执行许可

## 1. 目标

实现 8110 通用线程生命周期消息账、当前只读投影、发布端口和读取端口。该服务属于 BIZ 运行基础设施，不属于 DATA-L1—L3；SELF-THREAD 适配因失败候选重试语义未冻结而退出本切片。

## 2. 完成条件

1. 通用 DTO 和两个端口只有一个实体所有者；服务模块唯一写消息账与当前投影。
2. 消息身份、当前性、合法转换、迟到历史链、精确重复、身份冲突、系统线程绑定和终态守恒均结构化实现；`已接受/已成为当前` 不混淆。
3. 单运行代次消息账与当前投影有界，发布/读取竞争不等待；单锁事务失败不留下半消息或半投影。
4. 架构合同版本与事件模式版本分别定义、分别校验，当前相同数值不得形成同一版本域。
5. 普通应用未来先构造唯一服务、最后析构，全部发布线程必须在服务之前 join；本计划只冻结装配合同，不提前接线。
6. provider 工程登记唯一，根工程双配置通过；未接线范围保持 NOT_RUN。

## 3. 精确范围

```text
海中鱼巣/线程/线程生命周期.数据.h
海中鱼巣/线程/投影.线程生命周期.ixx
海中鱼巣.vcxproj
海中鱼巣.vcxproj.filters
规范/详细设计/20260815_THREAD-LIFECYCLE-PROJECTION-SERVICE_线程生命周期投影服务详细设计_v0.1.md
计划/过期计划/20260815_THREAD-LIFECYCLE-PROJECTION-SERVICE_线程生命周期投影服务代码实施切片_v0.1.md
```

禁止修改计划索引、规范目录、DATA 规范/provider、SELF-THREAD 源码/设计/计划、CF 图谱、路线和其它所有者 WIP。SELF-THREAD 当前现场原地保留，不得纳入本提交。

## 4. 实施顺序

1. 建立数据头和真实 provider 模块，登记 provider 两文件。
2. 完成请求校验、类别注册、转换矩阵、非阻塞发布造成的前向缺口/相邻严格互证、双账事务、迟到相邻链、迟到终态跨缺口拒绝、终态、有界配置、try-lock 和两个只读入口。
3. 将架构合同版本和事件模式版本拆为独立常量与独立校验，不改变 DTO 字段顺序或端口签名。
4. 只针对精确范围自审、构建与验证；等待唯一 Git 发布租约后再暂存、提交、普通 push 和远端读回。

## 5. 验证

```powershell
git diff --check -- <精确六文件>
python .\tools\check_specs.py --strict
msbuild .\海中鱼巣.slnx /t:Rebuild /p:Configuration=Debug /p:Platform=x64
msbuild .\海中鱼巣.slnx /t:Rebuild /p:Configuration=Release /p:Platform=x64
```

生产外专项至少覆盖详细设计第 8 节的拒绝、版本分域、转换、幂等、迟到链、当前性分账、错代、容量、终态、并发和异常安全矩阵；专项不得把故障 seam 或内部状态出口写入生产代码，并按施工规范在退出后独立清理。无法经公开边界确定性触发的分配失败和 try-lock 竞争保持 `NOT_RUN`，不得以概率命中冒充确定性证明。

发布前必须取得当时唯一 Git 租约；其它代码叶占用执行段或构建资源时，本切片只保留 unstaged 现场并做不竞争的文档/静态复核，不得并行构建或发布。

## 6. 激活与未证明边界

计划登记前必须从最新 `main == origin/main` 重做 S0，确认 DATA 规范治理没有改变 8110、CODE-FILE-01、线程归属或项目工程边界。若只改变 DATA-L1—L3，当前计划无需等待 DATA provider 实现。

`DRIFT-SELF-THREAD-FAILED-CANDIDATE-RETRY-LIFECYCLE-SEMANTICS-UNDEFINED` 只阻断 SELF-THREAD：失败候选回收后究竟永久锁存、在同运行代次分配新尝试身份，还是重建普通应用上下文，现行设计没有唯一裁决。本 provider 不依赖该裁决，也不得借 provider 提交修改 SELF 状态机。

本计划完成不证明任何具体线程已运行，不证明控制面板已接线，也不证明 SELF-THREAD 或 EVENT-LOG 业务叶完成。SELF-THREAD、事件日志和后继线程分别保留自己的业务成功、租约、停止和验证合同；尤其不证明 SELF 失败重试语义已经裁决。

## 7. 实际施工验证

- 根工程 x64 Debug / Release Rebuild：PASS，0 警告、0 错误；
- 生产外专项 Debug / Release：各 `19/19 PASS`；
- `python .\tools\check_specs.py --strict`：`113/113 PASS`；
- 精确六文件 `diff-check`、UTF-8 无 BOM、工程 XML：PASS；
- 临时专项已移出仓库，未登记生产工程，生产代码无测试 seam；
- 真实线程、普通应用装配、控制面板、分配失败、持续锁竞争和完整运行联调：`NOT_RUN`。
