# 按 I64 时间范围读取当前状态组施工检查记录 v0.1

日期：2026-08-16

计划身份：`DATA-L2-STATE-CURRENT-GROUP-READ-BY-I64-TIME-RANGE`

结论：计划范围内的双配置根工程构建、生产外公开入口参数矩阵和静态清理门禁通过；未运行项保持 `NOT_RUN`。本文件不是独立验收结论。

## 1. 动态矩阵

| 检查项 | Debug | Release |
| --- | --- | --- |
| 错合同、零守卫、逆序范围和两个零预算分别入口拒绝 | PASS | PASS |
| 当前 owner 与类型登记完整但无 I64 时间值时成功空组 | PASS | PASS |
| 闭区间最小端、最大端和单点相等区间均包含边界 | PASS | PASS |
| 相同 I64 时间的多个状态全部返回 | PASS | PASS |
| 输出按状态身份严格升序而非时间值或值编码排序 | PASS | PASS |
| I64 组、U64 组和独立材料引用时间状态不参与 | PASS | PASS |
| 候选预算精确时成功，少一时整组数量预算不足 | PASS | PASS |
| 区间外 I64 候选仍计候选预算 | PASS | PASS |
| 返回预算精确时成功，少一时整组数量预算不足 | PASS | PASS |
| 陈旧守卫返回事实代次漂移和空载荷 | PASS | PASS |
| 时间值换代后只按新当前 I64 值命中 | PASS | PASS |
| 状态退出后不返回 | PASS | PASS |
| 候选与完整状态时间属性逐字段互证 | PASS | PASS |
| 既有按主体、特征实例、来源、完整状态和身份来源读取保持 | PASS | PASS |
| 全部读取前后事实代次不推进 | PASS | PASS |

专项程序内部对组合前置和结果形状执行 19 项独立断言，两配置汇总均为 `19/19 PASS`。表中按计划矩阵合并展示，不把断言总数冒充业务场景数。

## 2. 构建

```powershell
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m:1
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m:1
```

临时专项清理前、清理后均执行两条根工程命令；四次构建全部成功，退出码均为 0，且均为 0 警告、0 错误。该结果只证明当前共享工作区中的根工程未被本次状态时间范围查询破坏，不构成 SELF、普通应用或正式 BIZ 消费闭环结论。

## 3. 静态门禁

- `python .\tools\check_specs.py --strict`：`113/113 PASS`；
- 本切片四个最终结果文件 `git diff --check`：PASS；
- 四文件 UTF-8 可解码、无 BOM 且无行尾空白：PASS；
- 请求 DTO、纯值 helper 和 const 服务入口静态核对：PASS；
- 临时源码、工程、filters、exe、pdb 和专属生成文件均不存在，生产工程没有专项登记：PASS；
- 两个生产文件不存在专项名、`19/19`、`PASS` / `FAIL` 计数或故障注入 seam：PASS；
- 最终生产范围只有两个既有 L2 文件，永久证据只有本记录和配对施工记录：PASS。

## 4. NOT_RUN

真实资源耗尽、私有登记 / 索引 / 状态损坏、共享锁长期公平、极端并发调度、跨进程恢复和正式 BIZ 消费没有稳定合法注入或不属于本叶，均为 `NOT_RUN`。未用静态检查、构建或生产外专项替代这些动态证据。

## 5. 声明边界

检查结果只支持 `DATA-L2-STATE-CURRENT-GROUP-READ-BY-I64-TIME-RANGE` 这一 provider 的计划内结论。本记录不证明四类时间材料统一排序、动态时间范围、`DATA-EXT-04` 整项或数据服务最终需求清单整体完成。
