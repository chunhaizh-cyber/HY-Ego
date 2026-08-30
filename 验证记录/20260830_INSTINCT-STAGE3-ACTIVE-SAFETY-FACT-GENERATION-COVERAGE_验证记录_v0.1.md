# INSTINCT-STAGE3-ACTIVE-SAFETY-FACT-GENERATION-COVERAGE 验证记录 v0.1

日期：2026-08-30

## 1. 对象与隔离

- 正式起点：`16f44a5dcfad9cfd87de111aeea6bc57078a7390`。
- 计划 blob：`940d7cce26f160f98231f26d3f3a268560c140e9`。
- 隔离根：`D:/TEMP/海中鱼巣/INSTINCT-STAGE3-ACTIVE-SAFETY-FACT-GENERATION-COVERAGE/20260830-compile1`。
- 构建输入：从正式 HEAD 导出，只覆盖本计划六个源码；仓库其它未提交 WIP 未进入候选。
- 工具链：VS2022 Professional，MSVC 14.44.35207，`PlatformToolset=v143`，x64。
- 临时启动参数和 ABI consumer 只位于隔离根，不进入正式候选。
- 精确 staged tree 首轮固定为 `f9356cd8740163ffc2f7aecd161878a1b3f73ede`；该树的 Debug / Release Rebuild、四次专项运行和两配置 ABI consumer 全部通过。随后只更新本验证记录，六个代码 blob 未改变。

## 2. 构建、专项与 ABI

| 门禁 | Debug | Release |
| --- | --- | --- |
| 精确候选根工程 Rebuild | PASS | PASS，完整 `/GL + /LTCG` 代码生成 |
| `安全根定义与当前值` 专项 | PASS，exit 0 | PASS，exit 0 |
| `本能被动维护基础快照` 专项 | PASS，exit 0 | PASS，exit 0 |
| 独立 ABI consumer | PASS，`/GL + /LTCG`，exit 0 | PASS，`/GL + /LTCG /OPT:REF /OPT:ICF`，exit 0 |

ABI consumer 同时 import `海中鱼巣.领域.服务.L2状态动态原子发布` 和 `海中鱼巣.领域.组合.本能被动维护基础快照`，核对两个新公开入口的完整成员函数类型，并实际调用新结果成功谓词。手工 consumer 出现模块编译环境 C5050 提示，Debug 链接另有既有 LNK4075 提示；两配置编译、LTCG 链接和运行均成功。

VS 18 / MSVC 14.52 的首轮隔离 Debug 在无关正式基线文件 `服务.需求初次筹办准备.ixx:251` 触发 C1001。该工具链失败不归因本计划；同一精确候选用既有 v143 工具链两配置通过。

## 3. L2 provider 运行矩阵

| 用例 | 结果 | 覆盖 |
| --- | --- | --- |
| S12F | PASS | 未登记时返回账未登记，空组、空见证、截止 0 |
| S13F | PASS | 起点等于登记代次合法空；起点等于 G0 合法空；早于登记返回覆盖边界不可用；零起点、未来起点、零预算入口拒绝 |
| S15F | PASS | 三项 `(登记G,G0]` 完整组；排除首项后两项；G0 边界空组；按形成事实代次、账身份排序；预算不足空载荷 |
| S13 / S15 | PASS | 既有 UTC 登记后空组、三项半开区间、排序和预算回归不变 |
| S16 / S11 | PASS | 状态动态清理后永久账仍可读，重新装配后永久账恢复读回不变 |

坏锚点载荷、重复身份、同特征同 G1 多账及参与者错 G1 沿用统一 private 全量账完整性验证；本轮未新增可合法制造这些持久坏事实的故障注入入口，专项运行标记 `NOT_RUN`，对应 fail-closed 分支由编译和静态代码事实覆盖，不升级为运行证据。

## 4. 直接业务消费者运行矩阵

| 用例 | 结果 | 覆盖 |
| --- | --- | --- |
| A05 | PASS | 坏合同入口拒绝；起点等于 G0 的完整空组返回已读取、无尚未消费变化 |
| A06 | PASS | 登记前起点映射覆盖边界不可用；预算不足结构化映射；非空变化组返回来源未裁定 |
| A00—A04 | PASS | 既有 UTC 窗口坏请求、规则/账未发布、合法空组、预算、非空来源及下层错误映射无回归 |
| P00—P08 / Q00 | PASS | 基础快照、阈值版本投影、运行代次和旧 G 漂移、合法空服务裁决无回归 |
| Q01—Q06 | NOT_RUN | 普通构建未启用服务合同专项宏；不是本计划新增事实代次矩阵 |

两次基础快照之间的精确并发推进没有新增确定性注入 seam；旧 G 和稳定字段互证路径已运行，真正读中推进保持 `NOT_RUN`，不冒充运行覆盖。

## 5. 静态门禁

| 门禁 | 结果 |
| --- | --- |
| 六源码 `git diff --check` | PASS，仅行尾转换提示 |
| `python .\tools\check_specs.py --strict` | PASS，116 / 116 |
| 值域、L/H、定义版本、生产初始化 | PASS；对应生产文件零差异，仍为 `0..INT64_MAX`、`2767011611056432742`、`7378697629483820645`、版本 1 |
| 游标数据 / 服务、工程、普通应用、启动、线程和 SELF-GOVERNANCE | 本计划零修改 |
| cached diff 与精确 staged-tree 复验 | PASS；index 恰含计划八文件，`git diff --cached --check` 通过；`f9356cd8` 两配置 Rebuild、专项和 ABI consumer 通过 |

## 6. 结论边界

当前候选证明永久变化账可以按事实代次水位完整读取，且规则版本 1 只在完整空组时形成“无尚未消费变化”。没有证明游标已保存或推进该水位，没有实现非空主动安全来源、低位回升、A/V 写入、完整被动维护事务、阶段三或整个 `INSTINCT-ROUTE`。
