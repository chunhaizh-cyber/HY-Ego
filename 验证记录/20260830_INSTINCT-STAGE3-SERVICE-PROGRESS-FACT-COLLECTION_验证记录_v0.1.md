# INSTINCT-STAGE3-SERVICE-PROGRESS-FACT-COLLECTION 验证记录 v0.1

## 1. 基线与环境

- 计划 blob：`dcff19c020c9b614babbcf2161bead663f26e97e`
- 执行起点：`80c508f82a7c210bc29aa70a1ec66663166c2587`
- 工程入口：`海中鱼巣.vcxproj`
- 工具链：Visual Studio 2022 Professional，MSVC 14.44.35207，`PlatformToolset=v143`，x64。
- 隔离候选：`D:/TEMP/海中鱼巣/INSTINCT-STAGE3-SERVICE-PROGRESS-FACT-COLLECTION/wip-v143-20260830-01/source`。
- 专项 / ABI runner：同一隔离根下 `runner/`。
- 结果提交：待本切片精确提交形成；由计划收口提交补录。

## 2. 构建与外部 consumer

| 项目 | 结果 | 证据摘要 |
| --- | --- | --- |
| v143 Debug x64 根工程 Rebuild | PASS | `ServiceContractFactAuthorityValidation=true`，退出码 0，生成 Debug 可执行文件 |
| v143 Release x64 根工程 Rebuild | PASS | 验证宏关闭，退出码 0，完整代码生成并链接 |
| Debug ABI consumer | PASS | 仓库外 consumer import L1 服务与服务合同事实权威模块，构造唯一 owner，调用三个公开完整集合入口并运行退出 0 |
| Release `/GL + /LTCG` ABI consumer | PASS | 仓库外 consumer 完成代码生成、链接并运行退出 0 |
| 持久恢复 | PASS | P10 在新持久运行包关闭后恢复，合同、事件、进展三个完整集合逐字段一致 |

## 3. 专项矩阵

| 编号 | 结果 | 证明内容 |
| --- | --- | --- |
| P00—P09 | PASS | 既有合同 / 到期事件请求、0/1/N、精确重放、终态干扰、坏载荷、漂移、资源失败和同进程重构全部保持通过 |
| P10 | PASS | 扩展登记和三类完整集合跨持久运行包恢复后逐字段一致 |
| P11 | PASS | 非法进展请求精确空失败；0 项和 1 项进展集合均有完整见证 |
| P12 | PASS | N=8，八种运行状态全部保留；状态 only、动态 only、状态 + 动态三种合法形状均正式读回 |
| P13 | PASS | 进展 fixture 精确重放，事实代次不增加且结果逐字段相同 |
| P14 | PASS | 坏进展载荷映射引用冲突，整体空失败，不返回部分集合 |
| P15 | PASS | 读中 `G0` 漂移映射当前性漂移，空组、空见证、截止 0 |
| P16 | PASS | 资源失败不复用旧候选，空组、空见证、截止 0 |

## 4. 静态与差异门禁

| 项目 | 结果 | 证据摘要 |
| --- | --- | --- |
| 三文件工作区 / 隔离树 SHA-256 | PASS | 数据头、服务模块、专项三文件逐项一致 |
| Release 服务对象 fixture 符号扫描 | PASS | `dumpbin /symbols` 无 `ARCH_`、验证样本或故障注入符号 |
| 扩展 owner 身份扫描 | PASS | `0x494E'5354'5350'5247` 仅在既有服务模块命中一次 |
| `3014..301F` 扩展 key 扫描 | PASS | 十二个 key 仅在既有服务模块定义，无第二 owner |
| 生产写入口扫描 | PASS | 没有导出发布 / 提交 / 写入服务进展入口；fixture 全部位于验证宏内 |
| `git diff --check` | PASS | 三代码文件及记录无 whitespace error；LF/CRLF 提示不作为错误 |
| `check_specs.py --strict` | PASS | `116` 份正式规范目录项全部通过 |
| 精确 staged `git diff --cached --check` | PASS | index 恰含本计划三代码文件和两份专属记录，无 whitespace error |

## 5. 结论与未证明边界

当前候选已证明同一 `服务合同事实权威` owner 能建立 / 恢复进展扩展登记，并在共同非零 `G0` 返回 0/N 完整规范进展集合；进行中和七种非进行状态都由 provider 保留。Debug consumer 的手工编译出现 MSVC 模块环境 C5050 警告，但编译、链接和运行均成功；根工程双配置没有本计划编译或链接错误。

当前没有生产进展发布者。合法空组不证明有效服务活动，也不得升级为安全门禁、服务值衰减、生存安全回归、阶段三、SERVICE-C1 或 `INSTINCT-ROUTE` 完成。
