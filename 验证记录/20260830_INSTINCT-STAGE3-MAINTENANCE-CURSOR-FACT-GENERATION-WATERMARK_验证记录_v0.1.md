# INSTINCT-STAGE3-MAINTENANCE-CURSOR-FACT-GENERATION-WATERMARK 验证记录 v0.1

日期：2026-08-30

计划：`INSTINCT-STAGE3-MAINTENANCE-CURSOR-FACT-GENERATION-WATERMARK` v0.1

起点：`eb4ecb3149aea7b47e7a175ea40b2e5e907c496b`

结果提交：本记录与实现候选同提交形成；精确哈希以发布回传为准。

## 1. 环境与隔离

- 根工程：`海中鱼巣.vcxproj`。
- 正式工具链：Visual Studio 2022 Professional，MSVC 14.44.35207，`PlatformToolset=v143`，x64。
- 隔离根：`D:\TEMP\海中鱼巣\INSTINCT-STAGE3-MAINTENANCE-CURSOR-FACT-GENERATION-WATERMARK\20260830-compile2`。
- 构建输入从正式 HEAD 导出，只覆盖本计划四个代码候选；仓库其它未提交 WIP 未进入隔离候选。
- 临时启动参数 `--test-instinct-maintenance-cursor`、ABI consumer 源码、构建中间件和运行材料只位于隔离根，不进入正式候选。
- 精确 index 树：`3669c3bd727d881e0329e308725da02a42bdc018`；独立展开根为同一计划目录下的 `20260830-staged1/src`。

## 2. 构建、ABI 与静态门禁

| 门禁 | 结果 | 边界 |
| --- | --- | --- |
| v143 Debug 根工程 Rebuild | PASS，0 warning / 0 error | 编译并链接完整隔离候选。 |
| v143 Release 根工程 Rebuild | PASS，0 warning / 0 error | 编译并链接完整隔离候选。 |
| 精确 staged tree v143 Debug / Release Rebuild | PASS / PASS，exit 0 | 排除异主启动 hunk 后，两配置均编译并链接完整工程；Release 完成全量 LTCG。 |
| Debug `/GL + /LTCG` ABI consumer | PASS | 静态核对两个 v2 公开成员函数签名及两个结果 `成功()` 的 `bool/noexcept` 合同；仅有 `/INCREMENTAL` 被 `/LTCG` 忽略的预期链接提示。 |
| Release `/GL + /LTCG` ABI consumer | PASS，0 warning / 0 error | 同上。 |
| 四候选文件 `git diff --check` | PASS | 只有工作树 LF→CRLF 提示。 |
| `python .\tools\check_specs.py --strict` | PASS，116 / 116 | 只证明正式规范目录一致。 |
| 固定参数禁止差异扫描 | PASS | 安全根生产定义、单秒规则、L1、普通应用和工程文件均无本计划差异。 |
| `git diff --cached --check` 与 staged hunk 审计 | PASS | index 恰含 6 文件；混合启动文件只含本计划 3 个 hunk，两个 SELF-GOVERNANCE hunk仍留在工作区。 |

补充：VS 18 / MSVC v145 首轮隔离构建在既有 `服务.需求初次筹办准备.ixx` 触发 C1001；同一候选改用计划要求的 v143 后 Debug / Release 均通过，因此不归因本计划代码。

## 3. v2 双水位运行矩阵

| 用例 | 结果 | 机器证据 |
| --- | --- | --- |
| A02W | PASS，Debug / Release | v1 当前使 v2 读取返回 `待迁移`；正式登记后迁移为 v2，首次事实水位等于登记代次；v2 正式读回与 v1 六字段投影一致；同纪元完全相同为零写恢复；事实水位变化返回引用冲突。 |
| A02W2 | PASS，Debug / Release | 只变化运行代次、只变化纪元均拒绝；二者共同变化建立新纪元，完整秒边界取新观察，事实水位继承；旧迁移幂等键在当前槽变化后仍返回首次游标和首次 G1。 |
| v2 首次无旧游标 | 静态闭合，未单列运行编号 | `未建立` 分支只接受目标水位等于登记首次代次并写 7 项材料；生产启动对 `未建立` 使用同一分支。没有另建测试私口。 |
| 同纪元边界 / 时间源 / 规则漂移 | 静态闭合；时间和规则由旧 v1 C03 / C04 双配置回归 | v2 `游标等于请求` 失败后，未发生双纪元变化即返回引用冲突；未开放推进入口。 |
| 坏登记见证、零 / 错水位、见证晚于 G0 | 静态闭合；A02W 运行覆盖错水位 | `请求有效(v2)` 与首次 / 迁移分支逐字段 fail-closed；非成功结果为空提交游标、截止 0。 |
| 同键异义 | 静态闭合；旧 v1 C03 双配置回归 | v2 在当前性裁决前读取首次规范化写集，材料不同即返回幂等冲突。 |
| 提交后读回失败 / L1 资源故障 | NOT_RUN | 没有获授权的生产故障注入口；代码保留 `已可能发布`、首次 G1 和原键重放收束，资源 / 内部分账独立。 |

## 4. 回归运行

| 入口 | Debug | Release | 覆盖 |
| --- | --- | --- | --- |
| `--test-instinct-maintenance-cursor` | PASS，exit 0 | PASS，exit 0 | 既有 v1 C00—C06 全部 PASS：首次建立、同义恢复、同键异义、同纪元推进拒绝、版本 / G0 漂移、新纪元和进程级持久恢复。 |
| `--test-instinct-maintenance-base-snapshot` | PASS，exit 0 | PASS，exit 0 | P00—P08、A00—A06、Q00 及新增 A02W / A02W2 全部 PASS；Q01—Q06 因普通构建未启用服务合同专项宏而保持 NOT_RUN。 |

基础快照输出中的 P02 再次证明 L/H 只通过正式新定义版本变化；本切片没有运行期自动漂移。A05 证明同 G 完整空事实覆盖仍可判定无尚未消费主动安全变化，A06 证明非空来源未裁定继续 fail-closed。

## 5. 发布前精确门禁

精确 index 已完成：

- `git diff --cached --check`：PASS；
- staged 文件 / hunk 审计：PASS；
- staged tree Debug / Release Rebuild：PASS；
- 固定参数、禁止文件和异主 WIP 零纳入：PASS。

尚待普通提交与推送；发布后核对 `HEAD == origin/main`、ahead/behind `0/0`、index 空。

## 6. 结论边界

候选已经证明双水位 DTO / ABI、v1 迁移、新纪元事实水位继承、精确历史重放、生产启动登记和独立读回的构建及直接消费者运行结果。

不能据此声明水位已经随维护事务推进、A/V 已被动变化、非空主动安全来源已可裁决、完整秒维护事务、阶段三或 `INSTINCT-ROUTE` 已完成。
