# INSTINCT-STAGE3-SINGLE-SECOND-DEMAND-TIME-ADJUDICATION 验证记录 v0.1

日期：2026-08-30

## 1. 验证对象与环境

- 计划版本 / blob：v0.2 / `48b13e2097a1562630213f0d12a7f21e22e8d4c6`。
- 执行恢复基线：`966a1a3476598fef87d74e9a777f9461ff9682dc`。
- 工具链：Visual Studio 2022 Professional，MSVC 14.44.35207，`PlatformToolset=v143`，x64。
- 专项隔离根：`D:/TEMP/海中鱼巣/INSTINCT-STAGE3-SINGLE-SECOND-DEMAND-TIME-ADJUDICATION/compile-20260830-03`。
- 普通双配置与 ABI 隔离根：`D:/TEMP/海中鱼巣/INSTINCT-STAGE3-SINGLE-SECOND-DEMAND-TIME-ADJUDICATION/compile-20260830-04`。
- 结果提交：本记录与代码同一提交，以本文件所在 Git commit 为准。

隔离候选的四个源码 / 测试 SHA-256 与提交前工作区候选逐项相同：

```text
本能被动维护基础快照.数据.h        C2118B67D746EDF3BF95B059EC20BEA807CC51650F4BAD8E1E8CAE88F66F9075
组合.本能被动维护基础快照.ixx      605C757C7DF47CA36E2B44E12B4E176EC15A05E6724E05A80F0F8E6B0B9F6385
服务.服务合同事实权威.ixx          83345CD671534489E56F8BC73ABD905C3D0F47CE14754CC260E4C0416DC037C1
端到端测试.本能被动维护基础快照.ixx 5CCE8FC90253E6198BADFC7DAC9C1C201D4182B5CA955837E6A6C26431D8B8E9
```

## 2. 构建和 ABI consumer

| 门禁 | 结果 | 证据摘要 |
| --- | --- | --- |
| Debug x64 验证宏 Rebuild | PASS | `ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION` 开启，专项可构造真实组合材料 |
| v143 x64 Debug 根工程 Rebuild | PASS | 生产宏关闭，完整根工程成功 |
| v143 x64 Release 根工程 Rebuild | PASS | 生产宏关闭，完成全程序代码生成与链接 |
| Debug 独立 ABI consumer | PASS | 仓库外 consumer 只 import 组合模块，消费合同版本、结果谓词和新公开成员函数；编译、链接、运行 exit 0 |
| Release 独立 ABI consumer | PASS | `/GL` 编译、`/LTCG /OPT:REF /OPT:ICF` 链接并运行 exit 0 |

ABI consumer 的 IFC 由隔离根 `*.ifc.d.json` 的 `ProvidedModule` 生成 `模块全名.ifc` 映射，再用 `/ifcSearchDir` 导入；模块 OBJ 只在仓库外组成临时静态库供链接。编译器给出既有模块环境 C5050 警告，但没有编译或运行失败。

首轮逐项 `/reference` 尝试曾在 MSVC `module/reader.cpp:3056`、`optional(227)` 触发 C1001。改用仓库前序切片已验证的 IFC 映射形状后不再出现 C1001；最终门禁以上述 PASS 结果为准。

## 3. 专项矩阵

| 编号 | 结果 | 证明内容 |
| --- | --- | --- |
| P00—P08 | PASS | 既有基础快照请求、定义 / 游标前置、版本化 L/H、无新秒、单调区间、运行代次 / G0 漂移、V 来源和内核投影全部回归 |
| Q00 | PASS | 非法请求空失败；0 合同 / 0 事件是合法完整空集；同一完整秒内双读允许纳秒自然增长 |
| Q01 | PASS | 真实 t=0、三项并列最长、稳定排序；从正式快照重算 `T-1/T` 精确边界 |
| Q02 | PASS | 未来提出和未来生效开始均为 `时间材料不闭合`，空载荷 / 截止 0 |
| Q03 | PASS | 当前 / 过去 / 未来 / 其它纪元事件矩阵；只计同纪元且恰为当前秒的事件 |
| Q04 | PASS | 不支持的合同规则版本和事件规则版本均为 `版本漂移` |
| Q05 | PASS | 重复需求身份整体 `集合不闭合`，不膨胀活动需求数 |
| Q06 | PASS | provider 资源失败和读中 G0 漂移分别保持结构化失败，不返回部分载荷 |
| 服务合同 owner P00—P24 | PASS | fixture 扩展后既有完整合同 / 事件 / 进展 / 准备 / 持久恢复矩阵无回归 |
| 服务合同 owner Q00—Q06 | PASS | 到期事件合同终态完整集合既有专项无回归 |

`T = 2,592,000` 完整秒。三态结果为：

```text
本秒有到期未满足事件，或最长等待 >= T -> 三十天门禁已成立
无有效未满足合同且本秒无事件           -> 无有效未满足且无到期事件
其余                                     -> 存在未满观察窗需求
```

## 4. 静态、规范和差异门禁

| 门禁 | 结果 | 证据摘要 |
| --- | --- | --- |
| 当前生产 L/H 扫描 | PASS | 定义版本 1 仍为 `2767011611056432742 / 7378697629483820645` |
| 新 owner / 写入口 / 工程登记 | PASS | 差异只增加只读组合、DTO、验证宏 fixture 和专项；工程文件未改 |
| 四候选与隔离构建输入 SHA-256 | PASS | 四项逐一相同，见第 1 节 |
| `git diff --check` | PASS | 六个计划白名单文件无 whitespace error；只有工作树换行提示 |
| `python .\tools\check_specs.py --strict` | PASS | 116 份正式规范目录项全部通过 |
| `git diff --cached --check` | PASS | index 恰含四个源码 / 测试文件和两份专属记录，无 whitespace error |

## 5. PASS / NOT_RUN 边界

已证明：同一非零 `G0` 下，可以从基础快照、当前有效未满足合同完整集合和到期事件合同终态完整集合，形成可重算的单完整秒服务需求时间裁决；所有具名非成功均无部分载荷。

`NOT_RUN` / 未证明：生产普通应用消费者、线程循环、实际服务值衰减、安全值回升、有效活动 A、压力 P、游标推进、A/V/L/H 原子写入、状态 / 动态同步记录、多秒批量等价、跨进程恢复、资源耗尽长稳、阶段三和 `INSTINCT-ROUTE` 整体完成。
