# INSTINCT-STAGE3-PRODUCTION-SAFETY-DEFINITION 验证记录 v0.1

日期：2026-08-29

验证对象：本记录所在的精确代码结果提交。

验证根：

- `D:\TEMP\海中鱼巣\INSTINCT-STAGE3-PRODUCTION-SAFETY-DEFINITION\20260829-verify1`
- `D:\TEMP\海中鱼巣\INSTINCT-STAGE3-PRODUCTION-SAFETY-DEFINITION\20260829-verify2`
- `D:\TEMP\海中鱼巣\INSTINCT-STAGE3-PRODUCTION-SAFETY-DEFINITION\20260829-final`

工具链：Visual Studio 2022 Professional，MSVC `14.44.35207`，命令行覆盖 `PlatformToolset=v143`。正式工程当前声明 `v145`，本机未安装该工具集；未修改正式工程工具集。

## 1. P00—P07

| 项 | 证据 | 结果 |
| --- | --- | --- |
| P00 常量与入口 | Debug / Release 专项静态商余、精确常量、错合同空载荷拒绝 | PASS |
| P01 首次发布 | 专项返回`已发布`，完整定义与独立读回一致 | PASS |
| P02 同进程重复 | 返回`已恢复`，前后事实代次相等 | PASS |
| P03 持久恢复 | 销毁普通应用、同持久根重建、正式恢复本能根；初始化前后代次相等 | PASS |
| P04 异义 v1 | 预置异义定义，返回`定义冲突`且代次不前进 | PASS |
| P05 下层非成功 | 验证宏覆盖读取 / 发布的许可、漂移、资源、内部、冲突、已可能发布；全部空载荷 / 截止 0 | PASS |
| P06 正式启动消费者 | 对精确 index 检查：定义失败早退位于单调时钟、游标、自我线程之前；P00/P04/P05 证明 provider 失败不返回成功 | PASS（静态生产顺序 + 运行 fail-closed 分账） |
| P07 ABI consumer | 仓库外独立 `ABI消费者.cpp` 导入 provider 并编译公开调用；Debug / Release 构建和运行退出 0；Release `link.command.1.tlog` 含 `/LTCG` | PASS |

专项 Debug 与 Release 运行输出均为：P00、P01、P02、P03、P04、P05 全部 PASS，进程退出 0。

## 2. 构建与门禁

- 精确 index 提交树普通 Debug x64 `Rebuild`：PASS。
- 精确 index 提交树普通 Release x64 `Rebuild`：PASS；完成全程序代码生成。
- 专项 Debug x64 `Rebuild + 运行`：PASS。
- 专项 Release x64 `Rebuild + 运行`：PASS。
- 独立 ABI consumer Debug x64 `Rebuild + 运行`：PASS。
- 独立 ABI consumer Release x64 `Rebuild + 运行 + /LTCG 检查`：PASS。
- `python .\tools\check_specs.py --strict`：116 / 116 PASS。
- `git diff --check`：PASS（保留全部异主 WIP，仅检查格式）。
- `git diff --cached --check`：PASS。
- 工程四配置 `/utf-8` 静态检查：4 / 4 PASS。

## 3. 证据边界

- 没有把普通启动成功路径运行到常驻等待宿主，避免使用真实生产持久根；P06 由精确提交树调用顺序和专项 fail-closed 组合证明。
- 未用 VS 18 的 `v145` 重新验证；本轮按计划已知有效的 VS 2022 / v143 完成两配置。
- 本验证不证明主动安全结算、服务被动维护、双根自检、阶段三或 INSTINCT-ROUTE 整体完成。
