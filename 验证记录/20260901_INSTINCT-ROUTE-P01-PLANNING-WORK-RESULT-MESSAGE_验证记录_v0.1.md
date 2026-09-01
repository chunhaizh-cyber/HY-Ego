# INSTINCT-ROUTE-P01 筹办工作包与结果消息验证记录 v0.1

计划身份：`INSTINCT-ROUTE-P01-PLANNING-WORK-RESULT-MESSAGE`

验证代码提交：`d792cd7a79ceee133494c132c82b4f9c02fca92d`

隔离候选：`D:\TEMP\INSTINCT-ROUTE-P01-PLANNING-WORK-RESULT-MESSAGE\candidate-13288612`

## 1. 构建

| 验证 | 命令 | 结果 |
| --- | --- | --- |
| Debug 全量重建 | `msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m /v:minimal /nologo` | PASS；生成 `x64\Debug\海中鱼巣.exe` |
| Release 全量重建 | `msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m /v:minimal /nologo` | PASS；生成 `x64\Release\海中鱼巣.exe` |
| 最终生产工程增量复核 | Debug/Release `/t:Build` | PASS |
| 阶段二 P01 专项编译 | 在隔离候选中临时把 `端到端测试.本能路线阶段二.ixx`登记为 ClCompile 后执行 Debug/Release 构建 | PASS；随后按严格规则撤销临时登记，正式工程中登记数为 0 |

## 2. 静态门禁

最终候选扫描结果：

```text
manager_provider_calls = 0
worker_provider_calls = 1
worker_deques = 1
worker_threads = 1
worker_variant_queues = 1
protocol_result_ports = 1
project_protocol = 1
filters_protocol = 1
project_test = 0
filters_test = 0
```

另行扫描六个生产文件未发现 `outbox`、持久筹办账、服务合同账、五类历史账等新增体系。最终九文件以 `git diff --no-index --check`检查，没有空白错误；只出现 Git 的 LF/CRLF 工作副本提示。

## 3. 专项覆盖与未运行边界

阶段二测试源码新增的 P01 专项已通过编译，代码断言：

1. 构造一个完整工作包消息；
2. 现有任务工作线程消费一次；
3. 结果端前两次返回 `资源等待/队列已满`，第三次返回 `已接收`；
4. 快照要求已消费工作包数、provider 调用数、已确认消息数均为 1；
5. 三次接收的消息逐字段相同，当前处理中槽和待上交槽最终为空。

按用户“先完成整条链路，再统一做业务/集成验收”的口径，本轮没有把阶段二专项接入生产命令行，也没有运行整条业务集成验收。因此上述源码断言记为 `COMPILE_PASS / RUNTIME_NOT_RUN`，不得写成运行 PASS。

## 4. 严格规范检查

命令：

`python .\tools\check_specs.py --strict`

结果：`FAIL（3 项既有基线错误，0 警告）`：

```text
tools/l1_frozen_surface_manifest.json:
acceptance_record 尚未进入 HEAD
acceptance_record 尚未进入 origin/main
acceptance_record 工作树字节与已提交 blob 不一致
```

临时登记端到端测试时额外出现两项“生产工程不得登记端到端测试”错误；撤销登记后两项消失。剩余三项在 P01 起点候选中已经存在，且不属于 P01 白名单，本轮未扩大范围修复。

## 5. 结论

可以声明：

- P01 代码已形成单线程、单队列、每包一次 provider、唯一结果消息、管理端接收/重复确认与独立读回链；
- Debug/Release 生产工程全量构建通过；
- 停机派发竞态和内部失败误报已在计划范围内修正。

不能声明：

- 阶段二专项已运行；
- 严格规范检查全绿；
- 真实任务方法、P02/P03、需求满足、跨进程恢复或 INSTINCT-ROUTE 全链已完成。
