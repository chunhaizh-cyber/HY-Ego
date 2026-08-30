# INSTINCT-STAGE3-ACTIVE-SAFETY-SOURCE-CATALOG 验证记录 v0.1

日期：2026-08-30

计划：`INSTINCT-STAGE3-ACTIVE-SAFETY-SOURCE-CATALOG v0.1`

计划 blob：`6d03a5333ee57d988bae5e0f8f5b996c7662084f`

施工起点：`6e4f549bd923340d1d96465f681f2286ac6f9979`

结果提交：本记录所在提交；发布后以 Git 返回为准。

## 1. 环境与隔离

- 根工程：`海中鱼巣.vcxproj`。
- 工具链：Visual Studio 2022 Professional，MSVC 14.44.35207，`v143`，x64。
- 隔离根：`D:\TEMP\海中鱼巣\INSTINCT-STAGE3-ACTIVE-SAFETY-SOURCE-CATALOG\20260830-01`。
- 隔离树形状：当前 `HEAD` 的 Git archive 加本计划六个生产候选文件；不含共享工作区其它异主 WIP。
- 验证宏：`ARCH_INSTINCT_SAFETY_DEFINITION_VALIDATION`、`ARCH_INSTINCT_PRODUCTION_SAFETY_DEFINITION_VALIDATION`，只由仓库外 `validation.props` 注入。

## 2. 构建与 ABI consumer

| 检查 | Debug | Release |
| --- | --- | --- |
| 普通根工程 Rebuild | PASS | PASS，含 LTCG |
| 验证宏根工程 Rebuild | PASS | PASS，含 LTCG |
| 两专项实际运行 | PASS，exit 0 | PASS，exit 0 |
| 仓库外 ABI consumer `/GL` 编译、`/LTCG` 链接、运行 | PASS，exit 0 | PASS，exit 0 |
| 精确暂存树根工程 Rebuild | PASS | PASS，含 LTCG |

ABI consumer 只 import 安全根定义服务和生产初始化 provider，消费两个新公开入口、DTO / 成功谓词、生产初始化入口以及 L/H / 定义版本不变量。Debug 手工编译对 `_DEBUG` 空宏形状产生 C5050 环境提示，但编译、`/GL`、`/LTCG`、链接和运行均成功；Release 无该提示。

精确暂存树 Release 复验使用计划固定的 Visual Studio 2022、MSVC 14.44 与 `PlatformToolset=v143`。一次额外的 `v145` 试跑在与本切片无关的既有 `服务.需求初次筹办准备.ixx` 触发编译器 C1001，未作为本计划工具链结果；切回已冻结的 `v143` 后完整 Rebuild 通过。

## 3. 运行矩阵

| 分组 | 结果 |
| --- | --- |
| S00 | PASS：普通装配后定义和来源规则均未发布；未发布不是合法空集 |
| S01—S10 | PASS：既有定义发布、版本、快照、漂移和已可能发布回归保持 |
| S17 | PASS：空方法组规范；非 1、非空组、坏身份拒绝；乱序 / 重复 / 零方法身份谓词拒绝 |
| S18 | PASS：规则 1 显式空集首次发布、提交后读回失败见证、原键收敛和独立读回 |
| S19 | PASS：同键重复、同义异键零第二事实、同键异义和 G0 漂移 |
| S12—S16 | PASS：既有永久变化账登记、0/N、连续变化、来源绑定、清理和恢复回归保持 |
| S11 | PASS：持久重装配后定义、来源规则、登记和永久变化账均可读 |
| P00—P03 | PASS：L/H 精确不变量、定义 + 规则首次生产发布、同进程和跨进程恢复 |
| P04A | PASS：定义存在但规则缺失时补发规则；裸规则版本号不冒充完整集合 |
| P04B / P04 | PASS：来源规则异义或定义异义均 fail-closed，零第二写入 |
| P05 | PASS：定义与来源规则读取 / 发布非成功映射均保持空定义、空规则、截止 0 |

## 4. 静态与文档门禁

- 固定登记身份与 `0x0307'1205..1208` 生产源码仅在现有安全根服务 module-private 定义一次。
- 固定生产发布幂等身份和来源编码仅在生产初始化数据头定义一次。
- `git diff --check`（六个生产文件）：PASS。
- `python .\tools\check_specs.py --strict`：PASS，116 / 116。
- `git diff --cached --check`：PASS。
- 精确 staged 范围：PASS，恰为计划白名单六个生产文件与两份专属记录。
- 提交树复验：PASS，Debug / Release 根工程均由精确暂存树重建，Release 含 LTCG。

## 5. NOT_RUN 与声明边界

- 未运行正式主动安全结算方法，因为当前生产规则版本 1 的完整方法组按规范显式为空。
- 未验证规则版本 2、非空生产方法 owner 同截止读回或定义 / 规则联合原子切换；这些需要后继公开合同。
- 未运行永久变化账窗口分类、低位回升、UTC / 单调秒时间轴绑定或完整阶段三。
- 未执行发布后的独立集成验收；本记录是计划内施工验证，不替代集成验收。
