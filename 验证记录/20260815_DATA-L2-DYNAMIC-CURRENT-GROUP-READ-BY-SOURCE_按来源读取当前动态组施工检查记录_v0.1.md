# 按来源读取当前动态组施工检查记录 v0.1

日期：2026-08-15

计划身份：`DATA-L2-DYNAMIC-CURRENT-GROUP-READ-BY-SOURCE`

结论：计划范围内的双配置构建、公开入口参数矩阵和静态清理门禁通过；未运行项保持 `NOT_RUN`。本文件不是独立验收结论。

## 1. 动态矩阵

| 检查项 | Debug | Release |
| --- | --- | --- |
| 错合同、零守卫、零来源和零预算均入口拒绝空载荷 | PASS | PASS |
| 当前来源没有动态时成功返回空组 | PASS | PASS |
| 同一来源多个动态完整、同截止并严格升序读回 | PASS | PASS |
| 不同来源严格隔离 | PASS | PASS |
| 数量预算精确时成功 | PASS | PASS |
| 数量预算少一时整组拒绝且零载荷 | PASS | PASS |
| 来源未找到分账 | PASS | PASS |
| 来源已退出分账 | PASS | PASS |
| 陈旧守卫返回事实代次漂移 | PASS | PASS |
| 动态退出后不再命中当前来源组 | PASS | PASS |
| 已退出动态在退出截止仍可历史完整读取 | PASS | PASS |
| 既有主体 / 前状态 / 后状态组与单动态读取保持 | PASS | PASS |
| 读取前后事实代次不推进 | PASS | PASS |

两配置汇总均为 `13/13 PASS`。

## 2. 构建

```powershell
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m:1
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m:1
```

临时检查清理后两条命令均再次执行成功，退出码均为 0。根工程当前还消费共享工作区中异主的工程登记与 EVENT WIP；该事实只说明本次根构建没有被动态来源查询变更破坏，不构成 EVENT、SELF 或普通应用业务闭环结论。

## 3. 静态门禁

- `python .\tools\check_specs.py --strict`：`113/113 PASS`；
- 本切片四个最终结果文件 `git diff --check`：PASS；
- 四文件 UTF-8 可解码且无 BOM：PASS；
- 请求 DTO / helper、const 服务签名、状态 15 复用和两个生产文件范围静态核对：PASS；
- 临时源码、工程、exe、pdb 和专属中间文件均不存在，生产工程没有专项登记：PASS；
- 两个生产文件不存在专项名、`13/13`、`PASS` / `FAIL` 计数或故障注入 seam：PASS；
- 最终生产范围只有两个既有 L2 动态文件，永久证据只有本记录和配对施工记录：PASS。

## 4. NOT_RUN

真实资源耗尽、私有 owner / 索引 / 动态损坏、共享锁长期公平、跨进程恢复、时间范围查询以及正式 BIZ 消费没有稳定合法注入或不属于本叶，均为 `NOT_RUN`。未用静态检查或构建通过替代这些动态证据。

## 5. 声明边界

检查结果只支持 `DATA-L2-DYNAMIC-CURRENT-GROUP-READ-BY-SOURCE` 这一 provider 的计划内结论。动态时间范围和因果来源查询仍须分别实现；本记录不证明 DATA-EXT-05 或最终需求清单整体完成。
