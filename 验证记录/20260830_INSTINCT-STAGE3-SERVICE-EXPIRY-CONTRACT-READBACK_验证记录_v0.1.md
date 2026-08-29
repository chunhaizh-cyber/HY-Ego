# INSTINCT-STAGE3-SERVICE-EXPIRY-CONTRACT-READBACK 验证记录 v0.1

## 1. 基线与环境

- 计划 blob：`d04fbf5c982ea84bc5fbb89fa35eeb3f1b5143a6`
- 执行起点：`ff6683ea12d62dc791e3f233582dafbb8644f59c`
- 工程入口：`海中鱼巣.vcxproj`
- 工具链：Visual Studio Professional 18，`PlatformToolset=v143`，x64。
- 隔离候选：`D:/TEMP/海中鱼巣/INSTINCT-STAGE3-SERVICE-EXPIRY-CONTRACT-READBACK/final-20260830-01/source`。
- 结果提交：本记录与代码同提交；以本文件所在 Git 提交为准。

## 2. 构建、consumer 与恢复

| 项目 | 结果 | 证据摘要 |
| --- | --- | --- |
| v143 Debug x64 根工程 Rebuild | PASS | 起点 HEAD 加本计划三个代码文件，退出码 0 |
| v143 Release x64 根工程 Rebuild | PASS | 完成全程序代码生成和链接，退出码 0 |
| Debug 验证宏 Rebuild + 专项 | PASS | `ServiceContractFactAuthorityValidation=true`，P00—P24 + Q00—Q06 全部通过 |
| Debug 独立公开入口 consumer | PASS | 仓库外候选入口只 import L1 与服务合同事实权威模块，构造唯一 owner 并调用 `读取到期未满足事件合同终态完整集合_v1`，退出码 0 |
| Release `/GL + /LTCG` consumer | PASS | 生产宏关闭，运行退出码 0；`link.command.1.tlog` 明确含 `/LTCG` |
| 持久恢复 | PASS | P10 关闭首个持久运行包后重建，五类完整集合逐字段一致 |
| 三个候选源码一致性 | PASS | 工作区与隔离候选 SHA-256 逐项相同 |

三个候选源码 SHA-256：

```text
服务合同事实权威.数据.h        A243C117226604ED7172D944427B98AD22157C17F27B38491564A523D539733B
服务.服务合同事实权威.ixx     A9D5D3DB28CA46D374FD78CBDE3B9746D861C66060BEC7936208DE0D958CB4D6
端到端测试.服务合同事实权威.ixx 4B4C054A94ECA43CDB85BC3F93230B105DD87413CE1E59B5DED2477332B9B78C
```

## 3. 专项矩阵

| 编号 | 结果 | 证明内容 |
| --- | --- | --- |
| P00—P09、P11—P24 | PASS | 既有合同、事件、进展和准备读取全部回归通过 |
| P10 | PASS | 合同、事件、事件合同终态、进展和准备五类集合持久恢复后逐字段一致 |
| Q00 | PASS | 非法请求精确空失败；0 项有登记 owner 完整见证 |
| Q01 | PASS | 单事件返回原合同正式时间纪元和当前已到期终态 |
| Q02 | PASS | N=3，事件身份规范排序、三个原合同纪元保真、精确重放零第二副作用 |
| Q03 | PASS | 事件对应合同当前状态不是已到期时整体引用冲突 |
| Q04 | PASS | 秒数、终态版本、遗漏 / 重复成员、错误合同、遗漏 / 重复当前状态分别 fail-closed |
| Q05 | PASS | 原合同载荷损坏时引用冲突，无部分结果 |
| Q06 | PASS | 读中 `G0` 漂移和资源失败均返回空投影、空见证、截止 0 |

## 4. 静态与差异门禁

| 项目 | 结果 | 证据摘要 |
| --- | --- | --- |
| Release fixture 符号扫描 | PASS | 生产 `服务.服务合同事实权威.ixx.obj` 中 `ARCH_`、验证样本和故障注入命中 0 |
| 新 owner / 登记 / 键段扫描 | PASS | 本计划差异没有所有者建立身份、`0x0307` 键或结构登记增量 |
| 生产写入口扫描 | PASS | 新入口为 `const noexcept` 只读；没有发布、提交或写入服务合同事实入口 |
| L/H 改动扫描 | PASS | 三文件差异没有 L/H 定义或当前生产数值改动 |
| `git diff --check` | PASS | 三代码文件无 whitespace error；只有换行转换提示 |
| `check_specs.py --strict` | PASS | 116 份正式规范目录项全部通过 |
| 精确 staged `git diff --cached --check` | PASS | index 恰含三个代码文件与两份专属记录，无 whitespace error |

## 5. 结论与未证明边界

当前候选已证明既有唯一服务合同 owner 能在指定自我和同一非零 `G0` 下，完整读取 0/N 到期未满足事件、原合同和合同当前已到期终态；原合同是时间纪元唯一来源，所有非成功均没有部分载荷。

本结论不证明事件属于当前完整秒，不计算最长等待、有效服务活动、服务值衰减或生存安全回归，不形成任何生产写入，也不证明阶段三或 `INSTINCT-ROUTE` 完成。
