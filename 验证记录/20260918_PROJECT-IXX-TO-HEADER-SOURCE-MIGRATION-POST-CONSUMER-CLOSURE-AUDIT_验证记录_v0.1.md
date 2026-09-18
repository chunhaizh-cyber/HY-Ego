# 迁移后消费者闭包审计验证记录

计划身份：`PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-POST-CONSUMER-CLOSURE-AUDIT`
计划版本：v0.1
正式输入基线：`f28ac086ecf3db403945945e92ac05d606a1161a`

## 1. 验证矩阵

| 检查 | 命令或输入 | 结果 |
| --- | --- | --- |
| 输入稳定性 | `main == origin/main == f28ac086`，index 为空 | `PASS` |
| archive 输入 | `git archive --format=tar f28ac086...` 解包到本计划 `D:\TEMP` 隔离目录 | `PASS` |
| 输入 JSON 可解析 | 10 个附件 JSON 逐一 `ConvertFrom-Json` | `PASS` |
| Git blob 与 archive SHA-256 回读 | 2501 项与 `git ls-tree -r f28ac086` 和 archive 逐项复核 | `PASS`（`blobBad=0`，`hashBad=0`） |
| P 旧格式逐行扫描 | 108 个 P 输入，扫描 `.ixx/.cppcpp/module/import` | `PASS`（0 命中） |
| V 逐行扫描及人工分类 | 17 个 V 输入，22 条命中逐项分类 | `FAIL`（22 `active-consumer`，0 `inactive-text`，0 `uncertain`） |
| H 历史文字扫描 | 2376 个 H 输入 | `PASS`（11896 命中被保留为历史证据，不参与通过谓词） |
| 主工程 Debug x64 求值 | `MSBuild.exe archive/海中鱼巣.vcxproj -getItem:ClCompile/-getItem:ClInclude /p:Configuration=Debug /p:Platform=x64 /p:EnableD455RealSense=false /nologo` | `PASS`（27 `.cpp`、69 `.h`，无旧/非允许后缀） |
| 主工程 Release x64 求值 | 同上，`Configuration=Release` | `PASS`（27 `.cpp`、69 `.h`，无旧/非允许后缀） |
| 词法头源候选 | 28 个 P `.cpp` 对同基本名 `.h` | `PASS`（仅 `海中鱼巣/入口.cpp` 未匹配；已按设计单列，不作语义证明） |
| 暂存区格式 | `git diff --cached --check` | `PASS` |
| 正式规范目录 | `python .\tools\check_specs.py --strict` | `PASS`（126/126） |

## 2. 分类证据

| 活动消费者 | 命中 | 分类依据 |
| --- | ---: | --- |
| `L1中性当前引用闭包保护参数验证` | 6 | 一个 `import` 和五个工程 `ClCompile` `.ixx` 项。 |
| `L1历史关系组参数验证` | 6 | 一个 `import` 和五个工程 `ClCompile` `.ixx` 项。 |
| `L1历史属性值组参数验证` | 6 | 一个 `import` 和五个工程 `ClCompile` `.ixx` 项。 |
| `运行世界树根启动专项验证.ps1` | 4 | 脚本在其构建/运行前读取四个旧格式路径。 |

精确路径、行号、原文、blob、SHA-256 和分类理由见[old-format-hits.json](../施工记录/附件/20260918_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-POST-CONSUMER-CLOSURE-AUDIT/v0.1/old-format-hits.json)。工程求值完整输出与规范化条目见[project-item-evaluations.json](../施工记录/附件/20260918_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-POST-CONSUMER-CLOSURE-AUDIT/v0.1/project-item-evaluations.json)。

## 3. 结论与边界

`P=PASS`、`V=FAIL`、总迁移后消费者闭包=`FAIL`。失败原因是仍存在 22 条活动验证消费者引用 C++ Modules 旧格式，而非主工程或生产树仍保留旧格式。

全工作区 `git diff --check` 返回 2，唯一报告项为异主 `AGENTS.md:12` 尾随空白；该文件不在本计划允许范围，未修改。计划专属输出须在发布前另作目标路径 `git diff --check -- <allowed paths>` 复核。本次检查没有构建、链接或运行程序；MSBuild 仅以 `-getItem` 求值 archive 工程。

本验证不证明任何运行或业务能力；更不授权删除 V 消费者。后继应先建立逐消费者迁移/退役计划，固定验证合同和唯一处理方式。
