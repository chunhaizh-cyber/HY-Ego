# 按来源读取当前状态组施工检查记录 v0.1

日期：2026-08-15

计划身份：`DATA-L2-STATE-CURRENT-GROUP-READ-BY-SOURCE`

结论：计划范围内的双配置构建、公开入口参数矩阵和静态清理门禁通过；未运行项保持 `NOT_RUN`。本文件不是独立验收结论。

## 1. 动态矩阵

| 检查项 | Debug | Release |
| --- | --- | --- |
| 当前来源没有状态时成功返回空组 | PASS | PASS |
| 同一来源多个状态完整读回 | PASS | PASS |
| 不同来源严格隔离 | PASS | PASS |
| 状态身份严格升序且全部截止相同 | PASS | PASS |
| 数量预算精确时成功，少一时整组拒绝 | PASS | PASS |
| 错合同、零守卫、零来源和零预算均入口拒绝 | PASS | PASS |
| 来源未找到与来源已退出分账 | PASS | PASS |
| 陈旧守卫返回事实代次漂移 | PASS | PASS |
| 状态退出后不再命中且历史读取仍保留 | PASS | PASS |
| 既有按主体 / 特征实例组读取与单状态读取保持 | PASS | PASS |
| 读取前后事实代次不推进 | PASS | PASS |

专项程序内部对组合前置和结果形状做了 13 项独立断言，两配置汇总均为 `13/13 PASS`。表中按计划矩阵合并展示，不把断言总数冒充业务场景数。

## 2. 构建

```powershell
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m:1
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m:1
```

临时检查清理后两条命令均再次执行成功，退出码均为 0，未输出警告或错误。根工程当前还消费共享工作区中异主的工程登记与 EVENT WIP；该事实只说明本次根构建没有被状态来源查询变更破坏，不构成 EVENT、SELF 或普通应用业务闭环结论。

## 3. 静态门禁

- `python .\tools\check_specs.py --strict`：`113/113 PASS`；
- 本切片五个最终结果文件 `git diff --check`：PASS；
- 五文件 UTF-8 可解码且无 BOM：PASS；
- 既有 `L2结构状态` 1—14 数值、追加状态 15、请求 DTO / helper 与 const 服务签名静态核对：PASS；
- 临时源码、工程、exe、pdb 和专属中间文件均不存在，生产工程没有专项登记：PASS；
- 三个生产文件不存在专项名、`13/13`、`PASS` / `FAIL` 计数或故障注入 seam：PASS；
- 最终生产范围只有三个既有 L2 文件，永久证据只有本记录和配对施工记录：PASS。

## 4. NOT_RUN

真实资源耗尽、私有 owner / 索引 / 状态损坏、共享锁长期公平、跨进程恢复、时间范围查询以及正式 BIZ 消费没有稳定合法注入或不属于本叶，均为 `NOT_RUN`。未用静态检查或构建通过替代这些动态证据。

## 5. 声明边界

检查结果只支持 `DATA-L2-STATE-CURRENT-GROUP-READ-BY-SOURCE` 这一 provider 的计划内结论。动态 / 因果按来源查询和状态时间范围查询仍须分别实现；本记录不证明 DATA-EXT-04 或最终需求清单整体完成。
