# RUNTIME-GOVERNANCE-DEMAND-STRUCTURE-TASK-SNAPSHOT 验证记录 v0.1

日期：2026-08-20

计划 blob：`45ff8fa73a37ff96f7e7226b67e71119daa8d0d0`

起点：`1a8b2d560f3f457dc765709c70e17773859974d8`

## 已执行

| 验证 | 结果 | 证据 |
| --- | --- | --- |
| Debug x64 Rebuild | PASS | `msbuild .\海中鱼巣.slnx /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m`，0 警告、0 错误 |
| Debug `--test-治理` | PASS | 同一公开 L1 运行包签发 L2 owner，公开构造、公开写入与公开组合器读回：单根无任务、单根有任务、旧 G0 漂移、零根预算拒绝和父 N04 机械消费均通过 |
| Release x64 Rebuild | PASS | `msbuild .\海中鱼巣.slnx /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m`，0 警告、0 错误 |
| Release `--test-治理` | PASS | 与 Debug 同一公开消费者矩阵通过 |
| 规范 strict | PASS | `python .\tools\check_specs.py --strict`：`116/116` |
| diff 检查 | PASS | `git diff --check` 与 `git diff --cached --check` 退出 0 |

## 发布前待执行

- 获取唯一发布租约后，重新核对 HEAD/origin/index，精确暂存九个计划文件并提交、普通推送、远端读回。

## 未证明边界

当前测试未证明真实内部损坏、资源耗尽、真实并发代次漂移、跨进程恢复、生产普通应用装配，以及父级三子完整或 G1 后读。空根的合法成功由实现和首轮测试验证；本次扩展的端到端消费者重点覆盖单根无任务、单根有任务、旧 G0 与请求拒绝。
