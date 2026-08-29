# INSTINCT-STAGE3-SERVICE-CONTRACT-FACT-OWNER 验证记录 v0.1

## 1. 基线与验证环境

- 计划 blob：`8d92a30ccd5ebdb107bcbbf2671cafe1bd5e5a4b`
- 起点：`6b55a9ef500c9b6b9159185d721412240cd2cff6`
- 工程入口：`海中鱼巣.vcxproj`
- 工具链：Visual Studio 2022 Professional，MSBuild Current；正式根工程门禁使用本机已安装的 `v143 / MSVC 14.44.35207`。另以项目默认 `v145 / MSVC 14.50.35717` 做兼容边界诊断。
- 专项与 ABI runner：`D:/TEMP/海中鱼巣/INSTINCT-STAGE3-SERVICE-CONTRACT-FACT-OWNER/runner-01`。
- 干净索引候选：`D:/TEMP/海中鱼巣/INSTINCT-STAGE3-SERVICE-CONTRACT-FACT-OWNER/candidate-index-01`。
- 当前 HEAD 精确提交树：`D:/TEMP/海中鱼巣/INSTINCT-STAGE3-SERVICE-CONTRACT-FACT-OWNER/candidate-head-v143-20260830-01/source`，对应 `35df49d0f8d7ec4e0c2efbb02ec76bbacaef040b`，归档 tar SHA-256 为 `88033FC82F2352F0403499D91DE9D0F8C9D9742DE4B69AB2698440BD036DB039`。

## 2. 门禁结果

| 项目 | 结果 | 证据摘要 |
| --- | --- | --- |
| `git diff --check` | PASS | 退出码 0；只有既有 LF/CRLF 提示 |
| `python .\tools\check_specs.py --strict` | PASS | `116` 份目录项全部通过 |
| cached diff-check | PASS | 精确索引候选六文件无 whitespace error；记录加入后须再次执行 |
| live Debug，验证宏开 | PARTIAL | 本计划服务与专项编译通过；根构建被三个异主 SELF 测试缺失 `等待合同登记` 接口阻断 |
| live Debug，验证宏关 | PARTIAL | 本计划生产模块与专项无宏形状编译通过；同一异主阻断 |
| live Release，验证宏关 | PARTIAL | 本计划生产模块编译通过；同一异主阻断 |
| 当前 HEAD 精确提交树 `v143` Debug Rebuild | PASS | 退出码 0；生成 `Debug/bin/海中鱼巣.exe` |
| 当前 HEAD 精确提交树 `v143` Release Rebuild | PASS | 退出码 0；完成全程序生成，生成 `Release/bin/海中鱼巣.exe` |
| 精确索引候选项目默认 `v145` Debug / Release Rebuild | COMPATIBILITY FAIL | 全部源码编译完成后出现 26 个 L2 跨模块 LNK2001/LNK2019；相同源码在阶段二与本轮 `v143` 双配置均可完整链接，故记录为 `v145` 模块工具链兼容边界，不归因为本计划代码缺口 |
| Debug ABI consumer | PASS | 仓库外编译、链接、运行，退出码 0 |
| Release `/GL + /LTCG` ABI consumer | PASS | 生成代码完成，运行退出码 0 |
| Release fixture 符号扫描 | PASS | 生产 `服务.服务合同事实权威.ixx.obj` 中无三个 `ARCH_` fixture 符号 |
| 运行期只读查询旧接线扫描 | PASS | 两个运行期只读查询文件无新 owner / 两个完整集合入口命中 |

## 3. 专项矩阵

命令形状：工程以 `/p:ServiceContractFactAuthorityValidation=true` 编译服务和专项；仓库外 runner 链接专项、服务、L1 与直接 L2 依赖对象并运行。

| 编号 | 结果 | 证明内容 |
| --- | --- | --- |
| P00 | PASS | 非法请求精确映射为空失败 |
| P01 | PASS | 0 成员合法空合同 / 事件集合有登记 owner 完整见证 |
| P02 | PASS | 单合同、单事件在同 G0 完整读回 |
| P03 | PASS | N 合同、N 事件完整且规范排序 |
| P04 | PASS | 两阶段 fixture 精确重放，零第二次权威副作用 |
| P05 | PASS | 终态合同混入当前索引时 fail-closed 为集合不闭合 |
| P06 | PASS | 损坏载荷 fail-closed 为引用冲突，无部分成功 |
| P07 | PASS | 读中 G0 漂移返回当前性漂移和空载荷 |
| P08 | PASS | 资源失败返回空载荷，不复用旧结果 |
| P09 | PASS | 同进程服务重构恢复登记并保持逐字段读回 |
| P10 | PASS | 持久运行包关闭再恢复后，两类完整集合逐字段一致 |

## 4. 静态边界

- 生产 owner 写端口只用于结构登记；合同、状态和事件写入只存在于验证宏区间。
- 普通应用只建立 owner / 服务，不自动发布空合同、示例合同或到期事件。
- 没有新增公开生产合同写入入口。
- 当前没有生产合同发布者；合法空集合只证明已发布结构 owner 下成员数为 0，不证明 SERVICE-C1 闭环完成。

## 5. 结论与未证明边界

当前 HEAD 精确提交树已经以本机实际可用的 `v143` 完成 Debug / Release 根工程 Rebuild，结合 P00—P10、两类仓库外 ABI consumer、持久恢复、strict 与差异检查，本计划第 6 节门禁全部满足，验证结论升级为 `PASS`。

项目默认 `v145 / MSVC 14.50` 的 26 个既有 L2 跨模块未解析符号仍是独立工具链兼容边界。它不降低本轮 `v143` 正式门禁，也不授权修改 L2 特征、状态、动态、需求、方法或概念源码；若以后要求把 `v145` 作为正式支持工具链，须单独形成兼容修复目标与验证矩阵。

本结论只证明服务合同事实权威 leaf 达到其计划完成条件；仍不得声明 SERVICE-C1、服务结算、有效活动、服务值衰减、生存安全回归、阶段三整体或 `INSTINCT-ROUTE` 完成。
