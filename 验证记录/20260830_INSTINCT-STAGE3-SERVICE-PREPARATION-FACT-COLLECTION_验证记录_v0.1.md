# INSTINCT-STAGE3-SERVICE-PREPARATION-FACT-COLLECTION 验证记录 v0.1

## 1. 基线与环境

- 计划 blob：`377e6ab08a259af992731945558b8d32abe758ac`
- 执行起点：`01deea80fc22642650dbbcf3b5a9bc9092733e8b`
- 工程入口：`海中鱼巣.vcxproj`
- 工具链：Visual Studio Community 安装根 `18`，`PlatformToolset=v143`，x64。
- 隔离候选：`D:/TEMP/海中鱼巣/INSTINCT-STAGE3-SERVICE-PREPARATION-FACT-COLLECTION/6b16cb9ec8874da5aadb1e127f2a1895/source`。
- 结果提交：`0c170f397dd41da00f43ff9b73e3a641bd9cd5b0`。

## 2. 构建、consumer 与恢复

| 项目 | 结果 | 证据摘要 |
| --- | --- | --- |
| v143 Debug x64 根工程 Rebuild | PASS | 干净 `HEAD` 加本计划三代码文件，退出码 0 |
| v143 Debug x64 验证宏 Build | PASS | `ServiceContractFactAuthorityValidation=true`，退出码 0 |
| v143 Release x64 根工程 Rebuild / Build | PASS | 生产宏关闭，完成代码生成并链接 |
| Debug 独立公开入口 consumer | PASS | 临时入口只 import L1 与服务合同事实权威模块，构造唯一 owner 并调用 `读取当前服务准备完整集合_v1`，退出 0 |
| Release `/GL + /LTCG` consumer | PASS | 运行退出 0，`link.command.1.tlog` 明确含 `/LTCG` |
| 持久恢复 | PASS | P10 关闭首个持久运行包后重建，合同 / 事件 / 进展 / 准备四类结果逐字段一致 |
| 共享工作区 Debug 根工程 | BLOCKED_BY_FOREIGN_WIP | 异主三个既有测试引用已删除的普通应用等待合同接口；本计划模块已进入编译且未报本计划错误，不修改异主文件 |

## 3. 专项矩阵

| 编号 | 结果 | 证明内容 |
| --- | --- | --- |
| P00—P16 | PASS | 既有合同、到期事件和进展集合全部回归通过；P10 扩展为四类持久恢复 |
| P17 | PASS | 非法准备请求精确空失败；0 项由基础 / 进展 / 准备三层登记完整见证证明 |
| P18 | PASS | 单个需求来源准备正式读回并保留非零 `T→D` |
| P19 | PASS | N=9，九种状态、需求 / 能力缺口两种来源、完成结果与验证形状、规范集合和精确重放全部成立 |
| P20 | PASS | 坏准备载荷映射引用冲突并整体空失败 |
| P21 | PASS | 需求来源缺少 `T→D` 被拒绝，不形成部分成功 |
| P22 | PASS | 来源关系目标与载荷不一致被拒绝 |
| P23 | PASS | 读中 `G0` 漂移返回当前性漂移、空组、空见证、截止 0 |
| P24 | PASS | 资源失败不复用旧载荷，返回空组、空见证、截止 0 |

## 4. 静态与差异门禁

| 项目 | 结果 | 证据摘要 |
| --- | --- | --- |
| 三文件工作区 / 隔离树 SHA-256 | PASS | 三个 SHA-256 逐项相同 |
| Release 服务对象 fixture 符号扫描 | PASS | `dumpbin /symbols` 无 `ARCH_`、验证样本和故障注入符号 |
| 准备登记幂等身份扫描 | PASS | `0x494E'5354'5350'5250` 在生产源码唯一命中 |
| `3020..302D` key 扫描 | PASS | 十四个键各在唯一服务模块定义一次，`302E..302F` 未占用 |
| 生产准备写入口扫描 | PASS | 没有导出发布 / 提交 / 写入服务准备入口；fixture 位于验证宏内 |
| L/H 改动扫描 | PASS | 本计划差异没有当前 L/H 定义或数值改动；`INT64_MAX` 仅用于持久时间字段解码上界 |
| `git diff --check` | PASS | 三代码文件无 whitespace error；仅换行转换提示 |
| `check_specs.py --strict` | PASS | 116 份正式规范目录项全部通过 |
| 精确 staged `git diff --cached --check` | PASS | index 恰含三个代码文件与两份专属记录，无 whitespace error |

## 5. 结论与未证明边界

候选已证明既有唯一服务合同 owner 能建立 / 恢复第三层服务准备登记，并在共同非零 `G0` 返回指定自我的 0/N 完整规范准备事实组；九种状态和两种来源均保真，所有非成功均无部分载荷。

当前没有生产准备发布入口，三类外部见证也未由本计划独立裁决。合法空组不证明有效服务活动，不得升级为准备完成、补回、安全门禁、阶段三或 `INSTINCT-ROUTE` 完成。
