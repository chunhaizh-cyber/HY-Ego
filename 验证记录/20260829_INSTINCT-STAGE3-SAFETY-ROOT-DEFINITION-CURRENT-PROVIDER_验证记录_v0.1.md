# INSTINCT-STAGE3-SAFETY-ROOT-DEFINITION-CURRENT-PROVIDER 验证记录 v0.1

日期：2026-08-29

计划：`INSTINCT-STAGE3-SAFETY-ROOT-DEFINITION-CURRENT-PROVIDER v0.1`

计划 blob：`ea81403ccf9a8424ed02952985bb633207096d02`

施工起点：`e292e4d9a9d99ae2a995fac76592ca135ef98d76`

结果提交：`2fdcc4bf598922d273108bd455ef59a26d7c63b1`

## 1. 验证环境

- 根工程：`海中鱼巣.vcxproj`
- 工具链：Visual Studio 2022 Professional，MSVC 14.44.35207，`v143`，x64。
- 仓库外隔离根：`D:\TEMP\海中鱼巣\INSTINCT-STAGE3-SAFETY-ROOT-DEFINITION\20260829-01`。
- 验证宏：`ARCH_INSTINCT_SAFETY_DEFINITION_VALIDATION`，仅由仓库外 `validation.props` 注入隔离构建。
- ABI consumer：仓库外 `abi.cpp`，只 import `海中鱼巣.领域.服务.安全根定义与当前值`，消费版本 / owner 常量、DTO、结果谓词和三个公开成员函数地址。

## 2. 静态门禁

| 检查 | 结果 |
| --- | --- |
| 普通应用内 `发布安全根定义_v1` 调用扫描 | PASS，0 命中 |
| 固定 owner `0x494E'5354'5341'4645` 生产源码扫描 | PASS，仅数据头定义 1 处 |
| 本 owner 键 `0x0307'1201..1204` 生产源码扫描 | PASS，仅服务 module-private 定义 |
| 新服务 / DTO 工程和 filters 登记 | PASS，Debug / Release 模块扫描与编译均可达 |
| 非成功结果形状 | PASS，三个失败构造均保持 optional 空；发布除已可能发布外首次 G 为 0，所有非成功正式截止为 0 |

## 3. 构建

| 配置 | 命令形状 | 结果 |
| --- | --- | --- |
| 普通 Debug | `MSBuild 海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v143` | PASS，exit 0 |
| 普通 Release | 同上，`Configuration=Release` | PASS，exit 0 |
| 验证宏 Debug | 额外 `ForceImportBeforeCppTargets=validation.props`，隔离 OutDir / IntDir | PASS，exit 0 |
| 验证宏 Release | 同上，Release | PASS，exit 0 |

## 4. S00—S12 专项

Debug 与 Release 均编译仓库外专项 main、链接隔离对象并实际运行；两配置 exit 0。

| 编号 | 运行证据 | 结果 |
| --- | --- | --- |
| S00 | 普通应用装配后定义为未发布、无载荷 / 截止 | PASS |
| S01 | 版本 1 显式发布并独立读回 | PASS |
| S02 | 同键同义精确重复，首次提交 G 不变 | PASS |
| S03 | 同键异义幂等冲突且零部分载荷 | PASS |
| S04 | 首版非 1 与后继跳版均版本漂移 | PASS |
| S05 | 非法 L/H、零规则版本、坏角色锚点结构化拒绝 | PASS |
| S06 | 版本 2 成为唯一当前定义 | PASS |
| S06A | 版本 2 后版本 1 仍按原请求精确历史重放 | PASS |
| S07 | 定义和真实 `A` 同 G0，逐字段绑定 | PASS |
| S08 | 坏当前值材料 fail-closed、零快照 / 截止 | PASS |
| S09 | G0 读前漂移和读后漂移均 fail-closed、零快照 / 截止 | PASS |
| S10 | 提交后首次读回失败只返回已可能发布见证；原请求重放收束 | PASS |
| S11 | 持久重装配后恢复 owner、本能根锚点、当前定义和同 G 快照 | PASS |
| S12 | 两配置根工程、ABI consumer、普通应用唯一构造和稳定常量扫描 | PASS |

## 5. ABI consumer

| 配置 | 编译 | 链接 | 运行 |
| --- | --- | --- | --- |
| Debug | PASS | PASS | exit 0 |
| Release | PASS | PASS（`/LTCG`） | exit 0 |

初次诊断曾误用 VS 18 Community 编译器读取 VS 2022 Professional 14.44 IFC，触发 C5050 / C1001；改回与工程完全一致的 14.44 工具链后两配置无该问题。该诊断不归类为设计或代码失败。

## 6. 文档、index 与提交树门禁

- 精确 index tree：`D:\TEMP\海中鱼巣\INSTINCT-STAGE3-SAFETY-ROOT-DEFINITION\INDEX-20260829-102934`，内容为 `HEAD + 本计划 index`，不含 SELF-GOVERNANCE 工作树差异。
- 普通 Debug / Release 根工程从精确 index tree Rebuild：PASS，均 exit 0。
- 验证宏 Debug / Release 根工程从精确 index tree Rebuild：PASS，均 exit 0。
- Debug / Release S00—S12 专项从精确 index tree 的 IFC / OBJ 重新编译、链接并运行：PASS，均 exit 0。
- Debug / Release ABI consumer 从精确 index tree 的 IFC / OBJ 重新编译、链接并运行：PASS，均 exit 0；Release 使用 `/LTCG`。
- `python .\tools\check_specs.py --strict`：PASS，116 / 116。
- `git diff --check`、`git diff --cached --check` 和最终 staged 范围审计：提交前执行。
- 提交和普通推送：PASS，结果提交 `2fdcc4bf598922d273108bd455ef59a26d7c63b1` 已进入 `origin/main`。

提交树首次专项运行曾在普通应用装配处失败。根因不是生产 provider，而是测试 fixture 未显式填充现行正式 `任务筹办等待合同登记`，此前工作树中的异主配置简化掩盖了该问题。fixture 已在测试白名单内修正；修正后的普通 / 验证宏两配置根工程、专项和 ABI consumer 均重新通过。

## 7. NOT_RUN 与声明边界

- 没有生产阈值 / 规则版本发布者，因此未验证真实生产 `L/H` 发布。
- 未运行主动安全事件结算、服务值被动回归、控制态、调度、FRESH 或完整阶段三业务闭环。
- 未执行独立集成验收智能体的发布后服务验收；本记录是计划创建的施工验证，不替代发布后验收。
