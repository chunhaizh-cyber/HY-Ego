# 按 I64 时间范围读取当前动态组施工检查记录 v0.1

日期：2026-08-16

计划身份：`DATA-L2-DYNAMIC-CURRENT-GROUP-READ-BY-I64-TIME-RANGE`

结论：计划范围内的根工程双配置构建、生产外公开入口参数矩阵和静态清理门禁通过；未运行项保持 `NOT_RUN`。本文件不是独立集成验收结论。

## 1. 动态矩阵

| 检查项 | Debug | Release |
| --- | --- | --- |
| 错合同、零守卫、逆序范围和两个零预算分别入口拒绝 | PASS | PASS |
| 当前 owner 与类型登记完整但无双端 I64 动态时成功空组 | PASS | PASS |
| 闭区间最小端、最大端、单点、左右交和互相包含按谓词命中 | PASS | PASS |
| 两侧不相交不命中，相同范围多个动态全部返回 | PASS | PASS |
| 输出按动态身份严格升序而非起始、结束或值编码排序 | PASS | PASS |
| 任一端为 I64 组、U64 组或独立材料引用时不参与且不报错 | PASS | PASS |
| 逆序动态不作 DATA 错误，只按两个不等式机械判断 | PASS | PASS |
| 两端候选预算精确时成功，任一端少一时整组数量预算不足 | PASS | PASS |
| 区间外 I64 候选仍计每端候选预算 | PASS | PASS |
| 返回预算精确时成功，少一时整组数量预算不足 | PASS | PASS |
| 陈旧守卫返回事实代次漂移和空载荷 | PASS | PASS |
| 起始与结束时间值换代后只按新当前 I64 值判断 | PASS | PASS |
| 动态退出后不再返回 | PASS | PASS |
| 两端候选与完整动态时间属性逐字段互证 | PASS | PASS |
| 既有主体、前状态、后状态、来源、身份来源和完整读取保持 | PASS | PASS |
| 全部读取前后事实代次不推进 | PASS | PASS |

专项程序内部同时建立了公开前置存在、统一特征定义、特征实例、前后状态和十个动态，并对参数与结果形状执行 23 项断言；两配置汇总均为 `23/23 PASS`。表中按计划 17 项矩阵合并展示，不把断言总数冒充业务场景数。

## 2. 构建

```powershell
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m:1
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m:1
```

临时专项清理前、清理后均执行两条根工程命令；四次构建全部退出码 0。最终清理后 Debug、Release 重建均未产生编译或链接错误。该结果只证明当前共享工作区中的根工程未被本次动态时间范围查询破坏，不构成 SELF、普通应用业务链或正式 BIZ 消费闭环结论。

## 3. 静态门禁

- `python .\tools\check_specs.py --strict`：`113/113 PASS`；
- 本切片四个最终结果文件 `git diff --check`：PASS；
- 四文件 UTF-8 可解码、无 BOM 且无行尾空白：PASS；
- 请求 DTO、纯值 helper、const 服务入口、双端候选预算、返回预算和完整读回互证静态核对：PASS；
- 临时源码、工程、filters、exe、pdb、ifc、tlog、recipe 和专属生成文件均不存在，生产工程没有专项登记：PASS；
- 两个生产文件不存在专项名、`23/23`、`PASS` / `FAIL`计数或故障注入 seam：PASS；
- 最终生产范围只有两个既有 L2 文件，永久证据只有本记录和配对施工记录：PASS。

## 4. NOT_RUN

真实资源耗尽、私有登记 / 索引 / 动态损坏、共享锁长期公平、极端并发调度、跨进程恢复和正式 BIZ 消费没有稳定合法注入或不属于本叶，均为 `NOT_RUN`。未用静态检查、构建或生产外专项替代这些动态证据。

## 5. 声明边界

检查结果只支持 `DATA-L2-DYNAMIC-CURRENT-GROUP-READ-BY-I64-TIME-RANGE` 这一 provider 的计划内结论。本记录不证明其它起止材料类型统一排序、历史时间范围、`DATA-EXT-05`整项或数据服务最终需求清单整体完成。
