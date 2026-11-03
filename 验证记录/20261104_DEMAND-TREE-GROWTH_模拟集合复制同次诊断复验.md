# 模拟集合复制同次诊断后的双根需求树复验

2026-11-04；root独占运行、固定日志、浏览器、目标账及Git。沿用户确认的启动双根需求树生长范围，从实际项目正常入口验证；不手工补D/T、不直接调用本能、不使用专用测试工程。

## 输入、构建与正常入口

代码树`D:/worktree/demand-tree-growth-20261103/海中鱼巣`，分支`codex/dual-root-demand-preparation-growth-20261103`。原包v0.9计划blob`3ff2ac67f16e62110aae9445c8585ad73079452a`、详设blob`fd32a02edbaf2fa285a180d4ab8f9fd9d9558205`及唯一索引行经root/独审后发布`1506f2548590ac94c974bb47a4d67fc4951fe507`。单一生产增量为模拟学习cpp原失败消息的同次诊断，最终blob`497dd9920f184d969784dad49e08a01a805e74dc`；h保持`2f4fcbd50bbdbdff244fadec912368e1f4d14588`。原拒绝谓词、错误前缀、复制/学习次序及业务逻辑不变。代码及原施工记录精确提交、main快进普通推送`f05d7fc533e4065e3741e7930a719f77114e3e36`，运行输入无WIP。

实际工程Release/x64原session66250于02:37:31.3150509—02:38:26.5642381 +08完成，真实exit0、0警告0错误、54.79秒；单cpp重编并链接，见[施工记录§10](20261103_DUAL-ROOT-DEMAND-PREPARATION-GROWTH_v0.2_施工与编译记录.md)。root核最终exe SHA256`758F0B641391207D54BB7EE1C4DA6284ACAEAE4D7CDAA42AAB4EFC6FE3D1F3CD`，路径`D:/TEMP/海中鱼巣/DUAL-ROOT-DEMAND-PREPARATION-GROWTH/build-20261104-release-v04-01/bin/海中鱼巣.exe`。

02:40:06.2831709 +08正常入口启动PID52212，原Process句柄session57253，参数`--web-service-object-login=zch2005`；正式宿主54240监听核属于本PID。隔离目录`D:/TEMP/海中鱼巣/DEMAND-TREE-GROWTH/run-20261104-v09-copy-diagnostic-01`保存stdout/stderr和三份正式GET原始正文。IAB自建页9仅发送一次“你好。”启用采集，正式历史显示原文，不伪造服务请求。

## 需求生长与第二层匹配自我回执

- 双根安全D12326/T16655、服务D12347/T16825，先天子D12417/T16580保留。本轮新D17421(parent12417/sourceT16580)→T17605，新D17506(parent12347/sourceT16825)→T17892。
- 进一步新D18780(parent17421/sourceT17605，交互者值目标)→T19723，manager于02:40:43正式建立T19723。该第二层来自先天子分支D17421，不误写成D17506的子项。
- `control-snapshot-03.json`为sequence12，readStartedAt=`2026-11-03T18:42:19Z`、readCompletedAt=`2026-11-03T18:42:27Z`，任务区已取得。T17605的preparation.workId=15，selfCausalReview.workId=15、matchesPreparationWork=true、status=已处理因果子需求；receipt.complete=true，唯一items为status=已建立/demandId=18780。补齐上一轮未保存的第二层当拍匹配self确认回执，不以静态路径或后续空摘要替代。
- T19723已取得建立连接M13749/取得服务对象M13881候选，条件检查status=已完成、complete=true、selection=null；建立连接缺参数对象1/3/5/6，取得服务对象缺1/3。尚无必要条件子需求继续入树证据，不能把条件检查完成称为条件分解通过。
- 安全根仍完整无候选/无已知路径准确等待，不为证明树长而强造安全子需求。方法覆盖足够、满足停止及同义复用无环各分支仍未证明。

## 学习复制失败的同次定位

02:42:06建立连接、02:42:07取得服务对象均真实触发学习后失败；类别为规格未支持，发生在参数/隔离场景构造，尚未进入本能动作。#03最后自我学习异常的完整诊断为：

```text
simulation parameter 0: copied history/current task or message collection lacks complete typed mapping; methodId=13881; sourceFeatureId=13145; featureTypeRootId=168; featureConceptId=13259; copyEntry=fixed-source-feature; existingInputIndex0=8; ownerExistenceInput=2; languageSceneInput=0; valueSlot=current; slotIndex0=null; definitionMaterialTypeCode=2; comparisonRuleCode=4; materialId=19768; materialPhysicalTypeCode=1; payloadIsU64Array=true; payloadEmpty=false; memberCount=4
```

建立连接错误的同次F/FT/FC/材料/值槽相同，existingInputIndex0=10。0是私有复制上下文标记，不是第0业务参数。当前正式任务投影读到自我E12216/F13145/服务任务集合FT168/材料19768、成员[16825,17605,17892,19723]；故本次明确是非空服务任务集合当前值，不能继续泛称“可能消息集合”。rawAddedCount3而正式请求见证仍空，尚未取得服务需求，不以新增内部T冒充正式服务任务取得。

root源码读回：现`固定来源特征.emplace(k,F)`唯一位置来自范围概念必需特征遍历；专属对应存在存在时读取其F并固定为复制上下文。本次copyEntry明确为fixed-source-feature，不是后段来源E全F顺带扩张才首次带入。但入口标签本身不是完整必要性或FC域证明，仍须核正式范围和现有任务本地typed映射再冻结修法；禁止跳必需F、清空集合、保留现实T编码冒充隔离副本，或为单个方法定制参数。

本轮四本能pairCount仍0、活动学习归零，没有正式条件结果对或本能效果成功证据。v9诊断已命中，不等于复制/参数生成修复完成。

## 独立的候选发布与采样问题

#01 sequence1任务区在任务登记全集核同失败，不当空树；#02 sequence5与#03 sequence12任务区又完整取得，不据此判永久损坏或已修。

#02/#03均保存T17892/D17506、work16：查询已找到、publicationAttempted=true，但候选发布未完成；publicationResult.status=依赖未实现、statusCode=26、materialId=null。实际D为服务任务数量变化量并带新增成员来源见证，无引用内容定位。独审/root源码定位`新_任务类.cpp`约9924将见证读取除资源失败外多个非已读取状态折为26；原始前提变化/依赖未就绪/未支持仍未知。空新增任务或空见证本身可正常读取，不据此解释26；旧work摘要不能证明当前重试、CAS成功或活跃发布。此问题未改代码，独立保留，不冒称装配缺失。

## 正常退出、结论和后续

02:44:50正式结束按钮，宿主正常返回；02:44:52.5663756原session57253取得exit0，治理/self/manager及上下文最终回收结束。核无PID52212/54240监听，IAB自建页9关闭。固定日志未清空：末尾运行.log664964、逻辑错误.log29332、调试_RECEIVED_DEMAND_SCHED.log2373926字节；本轮新增两条学习异常，**不能称无错误**，未见本轮工程弹窗/工程终止。旧Release强收尾-1等反例保留，不由此次正常退出覆盖。

本次证明两层正式D/T、第二层匹配自我回执、候选/条件检查和真实学习触发，并将失败锁定到固定来源的非空服务任务集合当前值。未证明必要条件子需求、通用参数生成/正式配对、连接执行及整体目标完成。下一步原计划支撑只读核FC域及已有本地任务映射，冻结最小合法修订、审查登记后再实现；不修改本能、复制语义或清空真实材料绕过。本实测只改记录，代码知识变化无。
