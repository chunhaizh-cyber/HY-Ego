# RUNTIME-GOVERNANCE-INPUT-V3-ROUTING 验证记录

日期：2026-08-20

计划：`RUNTIME-GOVERNANCE-INPUT-V3-ROUTING v0.1`

计划 blob：`26503c1f29bd8b14e1491b238c605ba623d5213f`

## 已执行验证

| 命令 / 检查 | 结果 | 覆盖范围 |
| --- | --- | --- |
| `msbuild .\海中鱼巣.slnx /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m` | PASS，0 warning / 0 error | Debug x64 fresh Rebuild |
| `x64\Debug\海中鱼巣.exe --test-治理` | PASS，输出“全部通过” | v3、回执 v2、批次路由和既有治理专项 |
| `msbuild .\海中鱼巣.slnx /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m` | PASS，0 warning / 0 error | Release x64 fresh Rebuild |
| `x64\Release\海中鱼巣.exe --test-治理` | PASS，输出“全部通过” | 同上，Release 二进制 |
| legacy 扫描 | PASS，零匹配 | 六个源码/测试文件无句柄组、旧回执句柄、旧来源执行请求、旧冻结请求或 `核心/句柄.h` |
| `git diff --check` | PASS | 未暂存 diff 格式 |
| `python .\tools\check_specs.py --strict` | PASS，116 份目录项 | 正式规范目录一致性 |

## 专项断言

- 九类 v3 载荷均可准入；旧 v2、错 alternative、零身份、零运行代次和普通消息零 G0 均具名拒绝；停止可使用零 G0。
- 停止在批次路由中形成唯一截断；八类普通输入各形成唯一后继和 `合法等待`，同批等待不互相阻断。
- 回执 v2 覆盖合法、已过期、零运行代次、零读取截止代次、零身份、缺来源绑定、精确重复、幂等冲突、同任务序号倒退和准入上下文不一致。
- 路由代码仅形成等待项，专项没有调用或宣称 002-04/05/06 的真实消费或成功。

## 未验证范围

没有验证任务实际结果、6340 正式承接、002-04/05/06 的 provider/业务调用、需求活动重判、任务筹办、任务执行、跨进程恢复或完整自我治理循环；这些均不属于本计划的完成声明。
