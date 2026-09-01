# INSTINCT-ROUTE-P00 任务轮次正式生产与同 G0 读回验证记录

## 1. 对象与结论

- 候选基线：相对指定基线 `db505ff4` 的五文件候选切片。
- 运行环境：Windows x64，MSBuild 18.5.4，隔离持久根由专项按进程建立在 `D:\TEMP\海中鱼巣` 并在退出时清理。
- 核心实现与四条成功链：PASS。
- 详细设计 §8 全故障矩阵：DRIFT；自然可达路径已运行，需 L1 故障注入或坏账 fixture 的项目仍为 NOT_RUN。
- strict：DRIFT，原因是白名单外既有 frozen-surface manifest Git 状态，不归本叶所有。
- 发布状态：待发布；未执行 Git、提交或推送。

## 2. 实际命令与退出码

| 验证 | 命令摘要 | 退出码 | 日志 |
| --- | --- | ---: | --- |
| 候选 Debug | `MSBuild 海中鱼巣.vcxproj /t:Build /p:Configuration=Debug /p:Platform=x64` | 0 | `D:\TEMP\INSTINCT-ROUTE-P00-TASK-ROUND-PROVIDER\final-debug\build.log` |
| 候选 Release | `MSBuild 海中鱼巣.vcxproj /t:Build /p:Configuration=Release /p:Platform=x64` | 0 | `D:\TEMP\INSTINCT-ROUTE-P00-TASK-ROUND-PROVIDER\final-release\build.log` |
| coverage Debug | 同项目临时加入 `P00Runner.cpp` 与阶段二专项后 Debug 构建 | 0 | `D:\TEMP\INSTINCT-ROUTE-P00-TASK-ROUND-PROVIDER\coverage-build-debug4\build7.log` |
| coverage 运行 | `coverage-build-debug4\bin\海中鱼巣.exe` | 0 | `D:\TEMP\INSTINCT-ROUTE-P00-TASK-ROUND-PROVIDER\coverage-build-debug4\run7.log` |
| 规范严格检查 | `python tools/check_specs.py --strict` | 1 | `D:\TEMP\INSTINCT-ROUTE-P00-TASK-ROUND-PROVIDER\strict.log` |
| 共享 WIP 整合后 Debug | `MSBuild 海中鱼巣.vcxproj /t:Build /p:Configuration=Debug /p:Platform=x64` | 1 | P00 数据头与 `服务.L2任务结构.ixx` 已通过扫描和编译；随后白名单外 `服务.需求初次筹办准备.ixx` 两处未适配 P00 请求合同版本字段的旧聚合初始化失败 |

strict 的三条错误均指向 `tools/l1_frozen_surface_manifest.json`：`acceptance_record` 尚未进入 HEAD、尚未进入 origin/main、工作树字节与已提交 blob 不一致。P00 五文件未修改该文件，故只记录，不越权修复。

## 3. P00 专项矩阵

| 项目 | 结果 | 证据或边界 |
| --- | --- | --- |
| 首次提交完整返回 T、身份来源、`T→D`、`T→E`、R1 | PASS | `run7.log` 的 `[P00] PASS`；fixture 逐项核对五项载荷 |
| task-owner 事实同 G1 | PASS | fixture 核对 T、族关系、`T→D`、`T→E`、R1 创建代次均等于 G1 |
| 精确重复完整读回且零新代次 | PASS | 同一请求、同一键重放返回`精确重复`，五项载荷等于首次结果，当前代次仍为 G1 |
| 按建立键成功读回 | PASS | 同 G1 五项载荷与首次结果相等 |
| 按任务同 G1 成功读回 | PASS | T 与当前 R1 等于首次结果；owner-aware task-family 互证通过 |
| 按建立键未找到 | PASS | 返回`未找到`且五项载荷、正式读回截止均为零 |
| 任务未找到 | PASS | 返回专用`任务未找到`且 T/R/截止均为零 |
| 旧 G 漂移 | PASS | 返回`事实代次漂移`且五项载荷、截止均为零 |
| 同键异义 | PASS | 使用另一具名根需求的完整裁决材料，同键返回`幂等冲突`且零载荷 |
| E 未找到 | PASS | 新键引用未知 E 返回`未找到`且零载荷 |
| 入口拒绝 | PASS | 既有 `验证P00入口拒绝零载荷` 运行通过 |
| 已有当前任务查询锚点 | NOT_RUN | 当前专项未建立不会污染后续断言的专属隔离子会话 |
| E 已退出、D/L 漂移 | NOT_RUN | 需要合法退出或并发漂移 fixture；未伪造生产事实 |
| 许可、资源、内部错误 | NOT_RUN | 需要 L1 故障注入；公共生产接口无自然注入点 |
| 按建立键坏写集、退出事实 | NOT_RUN | 需要专属坏账/恢复 fixture；未直接篡改生产事实 |
| 当前关系 0、>1、错 owner/端点/角色/生命周期 | NOT_RUN | 需要专属坏账 fixture；正常 1 条路径已运行 |
| 读后 G 漂移 | NOT_RUN | 需要并发闸门或测试注入；代码已执行读后 G 校验 |
| 旧新建入口零生产调用、零消息/回执/线程队列 | 静态有界 PASS | P00 fixture 只调用三个新入口；五文件候选未新增消息、回执、线程或队列合同 |

## 4. 运行输出摘要

`run7.log` 明确包含：

```text
[INSTINCT-STAGE2][P00] PASS：任务轮次首次提交、精确重复、按建立键与按任务同代读回
[INSTINCT-STAGE2] V01、V01A、V02-V17、V16A 可运行项全部通过
```

原阶段二专项既有 `V13`、`V05`、`V16A` 的 NOT_RUN 文本保持原边界；它们不属于 P00 成功链完成证据。

## 5. 剩余风险与声明边界

1. §8 的故障注入/坏账矩阵尚未全部运行，因此本记录不把 P00 全矩阵升级为完成。
2. strict 受白名单外 manifest 既有 Git 状态阻断，必须由其所有者在发布工作区裁决。
3. coverage 工程、runner、build 与 log 仅位于任务临时根，不进入候选五文件。
4. 共享 WIP 的两个白名单外消费者仍需由后继或其所有者按已发布 P00 ABI 补入合同版本；本叶未越权修改或暂存它们。
5. 本记录不证明 P01 结果消息或任务管理线程接收；该语义属于紧邻后继。

提交：待发布

推送：待发布
