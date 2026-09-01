# DATA-L1-FINAL-CLOSURE 验证记录 v0.1

日期：2026-09-01

## 1. 验证对象

- 计划：`DATA-L1-FINAL-CLOSURE v0.1`
- plan blob：`fad9e1bc26517fd2f8ef9a4b348a45636609c6fc`
- 起点：`5e57055d053184822834f38d7f87d919e528be94`
- 验证形态：工作树定向检查、仓库外负例、从精确 index 导出的候选树双配置 fresh Rebuild 与同一统一公开 driver。
- 仓库外根：`D:\TEMP\海中鱼巣\DATA-L1-FINAL-CLOSURE\run-20260901-a` 及最终候选树验证目录。

## 2. 静态与冻结门禁

工作树定向复核结果：

```text
python .\tools\check_l1_frozen_surface.py --strict -> PASS
  43 项操作、5 个冻结文件与生产边界一致
python .\tools\check_specs.py --strict -> PASS
  115 份目录项全部通过
git diff --check -- <计划白名单> -> PASS
```

`git diff --check` 仅输出 Git 的 LF / CRLF 转换提示，无 whitespace error。

冻结检查器在 `D:\TEMP\海中鱼巣\DATA-L1-FINAL-CLOSURE\negative-checks-20260901-a` 的仓库外隔离副本执行十二类变异，全部按预期非零：

| 编号 | 变异 | 退出码 | 主要捕获 |
| --- | --- | --- | --- |
| 01 | 额外公开操作 | 1 | 服务文件长度与 SHA-256 |
| 02 | 缺失 `执行L1物理清理` | 1 | 缺冻结操作、长度与 SHA-256 |
| 03 | 新增同名重载 | 1 | 服务文件长度与 SHA-256 |
| 04 | 签名增加 `noexcept` | 1 | 服务文件长度与 SHA-256 |
| 05 | DTO 新增字段 | 1 | 数据头长度与 SHA-256 |
| 06 | 枚举稳定数值重排 | 1 | SHA-256 与 legacy 限定变种 |
| 07 | 合同版本变化 | 1 | 数据头 SHA-256 |
| 08 | 重导出数据头新增 callable | 1 | 数据头长度与 SHA-256 |
| 09 | 额外领域标识 `L1场景索引` | 1 | 未限定领域标识、长度与 SHA-256 |
| 10 | 同文件不同 owner 重复 legacy `状态=1` | 1 | legacy 缺失 / 移动 / 重复限定与 SHA-256 |
| 11 | 篡改 `surface_contract_status` | 1 | 状态必须为 `FROZEN` |
| 12 | 无验收提前设置 `data_l1_status=FROZEN` | 1 | 缺 40 位验收 commit 且与 4080 不一致 |

前八类都由整文件冻结指纹直接捕获。正式工作树随后执行 `python .\tools\check_l1_frozen_surface.py --strict` 退出 0；负例没有修改正式源。

## 3. 历史 fixture 与兼容矩阵

格式 1：

- source commit：`db65bc99c48310935509e3fdf05b22e71ae9b6b1`
- generator SHA-256：`be2c99945da20e7c999bd7f04f47ada2a0fb9720708dc898ebdf2f11fa8bb99d`
- `manifest.bin`：`4a63b5fa36cb1ffce7734d55435af26f09631b6eb3ee1ffcd12dc46985a6ff63`
- `snapshot-a.bin`：`eca97e5aa51146590097ed5c1d2ef397d839e1482b1cd8fd2b421b0f17f5330a`
- `snapshot-b.bin`：`3ef046fe80a59182cae7c506be8e3d4a8cb9b8a9043f03a4a99cf8e25a4791ea`
- 历史提交树公开恢复 / 读回：`PASS`。

格式 2：

- source commit：`ff2366d97b662cfc6c8cf16917c06d5da182bf4b`
- generator SHA-256：`c1f76956e6978b9b3904aa37b38064d60cfb128f6af4fcc7b285b8a493e47bb9`
- `manifest.bin`：`406866618be48117deceffc26fb0dea2cb6f545ff1d607996600310252fa60c6`
- `snapshot-a.bin`：`89675ef4807a62f898193b569c683b0065061de050d9168e9cdee3cc56c878e3`
- `snapshot-b.bin`：`cc3a4a3f99eb4865cb573aff461188aef4c6fa2fb7c19c27e3dec3a35661cfb6`
- 历史提交树公开恢复 / 读回：`PASS`。

当前实现工作树统一 driver 已实际覆盖并通过：

- 格式 1 恢复、owner / v1 旧账精确重放、首次 v3、重启后 v3 精确重放；
- 格式 2 恢复、三分区 v2 旧账精确重放、首次 v3、重启后 v3 精确重放；
- 格式 3 既有 V07 round-trip 与 v3 精确重放；
- 节点 / 关系 / 值当前、历史、墓碑，属性槽，当前源 / 目标关系组，历史关系 / 属性值组，一致投影，端口重签，首次材料，后继代次和编码不复用；
- 清单缺失、活动槽缺失、长度、SHA-256、未知载荷版本、未知标签、同根第二运行包占用和非活动槽诱导。

工作树 Debug 统一 driver 输出：

```text
[L1持久恢复测试] 三类事实、当前/历史/墓碑、组/一致投影、幂等与 fail-closed 通过
[DATA-L1-N-ATOMIC][V01-V08] 全部 PASS
[DATA-L1-FINAL-CLOSURE] unified driver PASS
```

工作树 Release `v143` fresh Rebuild 与同一统一 driver：`PASS`。工作树 Debug 在最后一次专项扩展后只做了增量 Build，不作为最终候选树 fresh Rebuild 证据；最终结论只采用精确 index 候选树双配置结果。

## 4. 精确 index 候选树

精确暂存 16 个计划白名单路径后，`git write-tree` 形成候选树：

```text
d463a831a3533eb40779691f2296ca449acb7732
```

该树由 `git archive` 导出到：

```text
D:\TEMP\海中鱼巣\DATA-L1-FINAL-CLOSURE\run-20260901-b\candidate-source
```

候选树不含根工程、filters、入口、装配、规范、计划、知识库、流程图或其它异主 WIP。实际结果：

| 门禁 | 结果 |
| --- | --- |
| `python .\tools\check_l1_frozen_surface.py --strict` | `PASS`，43 项操作、5 个冻结文件与生产边界一致 |
| `python .\tools\check_specs.py --strict` | `PASS`，115 份目录项全部通过 |
| Debug x64 `v143` exact-tree fresh Rebuild | `PASS` |
| Release x64 `v143` exact-tree fresh Rebuild | `PASS` |
| Debug x64 `v143` production-outside unified driver fresh Rebuild + run | `PASS` |
| Release x64 `v143` production-outside unified driver fresh Rebuild + run | `PASS` |

exact-tree 输出分别位于 `run-20260901-b\exact-debug` 和 `exact-release`；只把临时 `入口.cpp` 替换为统一 driver 的隔离副本位于 `candidate-driver`，其构建输出分别位于 `driver-debug` 和 `driver-release`。该 driver 只 import 两个既有公开专项，不进入 Git。两配置运行都输出：

```text
[L1持久恢复测试] 三类事实、当前/历史/墓碑、组/一致投影、幂等与 fail-closed 通过
[DATA-L1-N-ATOMIC][V01] ... [V08] PASS
[DATA-L1-N-ATOMIC] 可运行矩阵全部通过
[DATA-L1-FINAL-CLOSURE] unified driver PASS
```

本记录在上述验证后补写结果；最终 index 与 `d463a831...` 的编译和运行输入保持逐 blob 相等，只允许本验证记录 blob 变化。发布前再次执行冻结检查、严格规范检查、cached 路径白名单和 cached diff-check。

## 5. 未运行矩阵

以下均为 `NOT_RUN`：

- 进程在清单 / 槽原子替换中崩溃；
- 操作系统或硬件断电；
- 介质损坏及真实磁盘故障；
- 内存、句柄或存储资源耗尽；
- 长时运行、长期保留与真实生产负载。

这些未运行项不阻止通用操作 / 目标公开合同冻结，但禁止声明对应可靠性已验证，也禁止把 DATA-L1 整体状态从 `PENDING_ACCEPTANCE` 升级为 `FROZEN`。

## 6. 结论边界

冻结面、十二类负例、三类事实恢复、格式 1 / 2 / 3、双配置 exact-tree fresh Rebuild 和双配置统一公开 driver 均已通过，可以形成“待独立验收的 DATA-L1 收口候选”。该结论仍不等于独立集成验收；候选提交、推送和统一黑盒都不能替代第二所有者验收或第二次治理发布，整体状态保持 `PENDING_ACCEPTANCE`。
