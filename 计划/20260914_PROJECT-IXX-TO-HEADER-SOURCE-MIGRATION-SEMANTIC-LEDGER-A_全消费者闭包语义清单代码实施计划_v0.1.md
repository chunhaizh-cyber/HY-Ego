# 全消费者闭包语义清单代码实施计划

计划身份：`PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A`

日期：2026-09-14
版本：v0.1
状态：可执行（只生成迁移施工前的语义清单；不改生产源码）
起点：正式 `main` / `origin/main` 提交 `89697ac7220d17068f70da63cfb4a945824335f1`

## 1. 目标、完成条件与边界

本叶解决已暂停的 `PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-CORE-COMPONENT` 的两个设计缺口：词法候选定义不能决定 `.h/.cpp` 唯一落点，且原范围漏列了三个正式验证消费者。它不把任何 `.ixx` 改名、不修改项目文件、更不建立 `.ixx` 到 `.h` 的兼容桥。

完成条件是发布一份可复现的、基于 C++ 语义解析的迁移清单，供下一份实际迁移计划冻结逐实体施工合同。清单必须覆盖本节的 57 个模块、两个 `.cppcpp` 实现单元、所有活动消费者和六个工程；对每一可迁移实体列出完整签名、原始源锚点、声明/定义的 `.h/.cpp` 落点、直接调用方或“无静态直接调用方”的证据，以及唯一实体/ODR 理由。输出经复核后，计划智能体才可建立实际的全量 `.h/.cpp` 迁移计划。

本叶不证明 `.h/.cpp` 已存在、工程能编译、程序行为等价、验证程序通过或任何业务能力闭合。

允许写入仅为本计划的施工记录、验证记录及其受控清单；禁止修改：

- `海中鱼巣/**`、`验证工具/**` 的任何生产或验证源码；
- `海中鱼巣.vcxproj`、其 filters、五个验证工程；
- 正式规范、共享知识索引、其它计划和异主 WIP；
- 三个交接中的删除 WIP：`验证工具/L1中性当前引用闭包保护参数验证.cpp`、`验证工具/L1历史关系组参数验证.cpp`、`验证工具/L1历史属性值组参数验证.cpp`。

后三个文件只允许从 `HEAD` Git 对象读取其内容和 blob；不得在本叶恢复、覆盖、删除或暂存。用户已经授权其后续交接，含义是它们必须进入**下一份实际迁移计划**的 `.cpp` 消费者集合，而不是授权本叶修改它们。

## 2. 正式依据与已核对事实

- `CODE-FILE-01` v4.0 §2、§3.2、§3.4、§4.2、§4.3：最终形态是唯一同基本名 `.h/.cpp`；非 inline 定义不得留在头中；不得保留 `.ixx`、`.cppcpp`、`module`、`export`、`import` 或桥接形态；必须逐函数确定实体所有者和物理位置。
- `计划/计划索引.md`：`WORLD-TREE-STRUCTURE-B` 因旧模块施工形态暂停，旧 CORE-COMPONENT 因 `FUNCTION-PHYSICAL-LOCATION-DRIFT` 和 `CONSUMER-CLOSURE-DRIFT` 暂停。
- 在本起点 `git ls-files '*.ixx'` 为 **57**；全部是具名 C++20 模块。现存 `.cppcpp` 为 **2**：`海中鱼巣/装配.普通应用.cppcpp`、`海中鱼巣/业务/应用服务.自我形成.cppcpp`。
- 已核对组件内无循环的 104 条 import 只是模块可达关系，不能推出函数调用关系。全根 `海中鱼巣/**` 与 `验证工具/**` 的活动源码必须以本计划第 4 节算法重新扫描，不能沿用旧计划数字作为结果。
- 本机已实测 `LLVM clang++ 22.1.8` 在 `VsDevCmd.bat -arch=x64 -host_arch=x64` 环境可直接对现有 `.ixx` 执行 `-x c++ -std=c++20 -fsyntax-only -Xclang -ast-dump=json`；无 import 的 `合同.L1公共事实.ixx` 和其 BMI 均成功。故采用“原模块 + clang BMI + JSON AST”，不采用把源码伪改为头文件的文本投影。

## 3. 固定输入闭包

### 3.1 模块与实现单元

执行者以 `git ls-files '*.ixx'` 取得精确 57 行白名单，并以每行同基本名映射目标 `.h`。所有模块都必须进入后续物理迁移范围；本叶不得凭目录、当前 import 数或文件大小排除模块。两个历史 `.cppcpp` 的唯一后继为同基本名 `.cpp`：

| 旧实现 | 唯一目标实现 |
| --- | --- |
| `海中鱼巣/装配.普通应用.cppcpp` | `海中鱼巣/装配.普通应用.cpp` |
| `海中鱼巣/业务/应用服务.自我形成.cppcpp` | `海中鱼巣/业务/应用服务.自我形成.cpp` |

模块声明名、路径、Git blob 和完整 import 边都写入 `模块清单.json`。任何模块声明、缺失 provider、循环或 57 以外的已登记 `.ixx` 都是本叶失败，不能静默排除。

### 3.2 消费者闭包

消费者扫描根固定为 `海中鱼巣/**/*.ixx`、`海中鱼巣/**/*.cpp`、`海中鱼巣/**/*.cppcpp`、`验证工具/**/*.cpp`，排除 `验证记录/**` 等历史证据文件。匹配每一个 C++ token 行的 `import 海中鱼巣.*;` 和 `export import 海中鱼巣.*;`，并将提供模块、消费者路径、行号、import 形态和消费者所属工程写入 `消费者闭包.json`。

后续实际迁移不可出现“旧 `.ixx` 消费新 `.h`”的过渡。任何仍是 `.ixx` 的消费者只要消费本次已转换的模块，就必须被纳入同一原子迁移批次并改为 `.h/.cpp`；现有 `.cpp` 消费者只将 import 改为对应 `#include`。因此所有 57 个模块、`入口.cpp`、两个 `.cppcpp` 和扫描命中的验证 `.cpp` 是同一最终闭包的候选集，实际集合只能因扫描结果扩大，不能缩小。

三个已交接的验证消费者在后续实际计划中固定为普通 `.cpp` 消费者：从 `HEAD` 取其唯一原内容，改写其 import 为清单给出的 include，保留其独立验证入口；它们没有 `.vcxproj` 项时不得虚构工程登记。其构建方式由后续计划按原验证命令/调用点冻结。

### 3.3 工程闭包

必须逐 XML 节点读取并写入 `工程闭包.json` 的工程为：

1. `海中鱼巣.vcxproj`；
2. `海中鱼巣.vcxproj.filters`；
3. `验证工具/绑定存在创建专项验证.vcxproj`；
4. `验证工具/定位特征创建专项验证.vcxproj`；
5. `验证工具/相关概念窄参与专项验证.vcxproj`；
6. `验证工具/场景特征实例组织专项验证.vcxproj`；
7. `验证工具/世界树根启动专项验证.vcxproj`。

对每个 `<ClCompile Include>`、`<ClInclude Include>` 和 filter 条目记录：工程、条件、原路径、是否属于 57 模块/两个 `.cppcpp`/消费者、目标路径、预期项类型。最终迁移只能把 `.ixx/.cppcpp` 的 `ClCompile` 改为目标 `.cpp` 的 `ClCompile`，新增目标 `.h` 的 `ClInclude`，并保留原条件、Filter、`ExcludedFromBuild` 与未命中项。主工程和五个验证工程必须以该清单全闭合；filters 是主工程的独立一致性对象。

## 4. 可复现 C++ 语义分析合同

### 4.1 隔离与版本固定

所有中间物只存在于：

`D:/TEMP/海中鱼巣/PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A/<UTC轮次>/`

输入只能由 `git show 89697ac7:<相对路径>` 或等价 Git 对象导出取得；禁止从 dirty 工作区复制。开始时记录 `clang++ --version`、`VsDevCmd.bat` 绝对路径、MSVC 工具集版本、Windows 架构、所有输入 blob、命令行、退出码和 SHA-256。不得修改工作区源码以帮助解析。

使用固定编译前置环境：

```powershell
cmd /c 'call "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat" -arch=x64 -host_arch=x64 >nul && set'
```

从该环境取得系统 include；项目 include、预处理宏和语言开关从七份工程 XML 的 Debug/Release x64 实际条件合并读取。若项目条件给出相互冲突的解析开关，清单记录冲突并停止，不得任选一种。

### 4.2 BMI 与 AST 步骤

1. 用 §3.1 的模块声明和 import 建立有向图；import 指向 provider。拓扑排序以 provider 在前。发现环、未知 provider或重复模块名即失败。
2. 依拓扑顺序，对每一个 Git 导出的原 `.ixx` 运行 `clang++ -std=c++20 -fms-extensions --precompile -x c++-module`；它的每个已解析 provider 必须以 `-fmodule-file=<模块名>=<对应pcm>` 显式传入。每项的 `.pcm`、stderr 和 hash 留在隔离目录。
3. 对每个模块再次运行 `clang++ -std=c++20 -fms-extensions -fsyntax-only -x c++ -Xclang -ast-dump=json`，带同一组 `-fmodule-file`。为避免系统头淹没输出，可附加 `-Xclang -ast-dump-filter=海中鱼巣`，但只要过滤导致本源文件任一声明/定义缺失，就改用未过滤输出后再提取；不得因输出过大跳过模块。
4. 对 `入口.cpp`、两个 `.cppcpp`、所有扫描命中验证 `.cpp` 同样生成 AST；对三个删除 WIP 的输入必须先 `git show HEAD:<路径>` 到隔离目录，禁止访问工作区路径。
5. AST 后处理只接受 `kind`、限定名、类型、模板实参、`mangledName`（有）、source range、`isThisDeclarationADefinition`、inline/模板/可见性信息和调用表达式中的已解析引用。后处理脚本、版本、输入 hash 和输出 hash 均进入记录；词法扫描仅可作 AST 覆盖率交叉检查，不能决定实体身份、重载归属或调用关系。

### 4.3 逐实体迁移清单的强制字段

`语义实体迁移清单.json` 每行代表一个实际 C++ 实体，稳定键为“限定名 + 完整规范化签名 + declaration kind + 原文件 blob + 定义起始行”；同一实体的声明、定义与引用使用该键关联。至少含：

| 字段 | 含义 |
| --- | --- |
| `entity_key`、`kind`、`qualified_name`、`full_signature` | 可重放的语义身份；构造/析构、重载、模板、cv/ref/noexcept、返回型、参数及可见性均不可省略 |
| `source_path`、`source_blob`、`declaration_range`、`definition_range` | 原始锚点；无定义必须明确为 declaration-only |
| `exported`、`inline_or_template`、`linkage`、`owner_file` | 原模块可见性与唯一所有者 |
| `target_header`、`target_source`、`target_anchor` | `.h` 首次声明及 `.cpp` 唯一定义位置；header-only 时 `target_source=null` |
| `odr_reason` | `public-declaration`、`inline-or-template-required-visible`、`private-file-helper`、`out-of-line-single-definition` 中的一项及简短事实说明 |
| `include_requirements` | 每个原 import 是否变成公开头 include、私有 cpp include或无需 include；不得把 `export import` 丢失为私有依赖 |
| `direct_callers` | 语义解析到的调用方键、调用点、调用种类；没有时写空数组和 `no_static_direct_caller=true` |
| `indirect_or_implicit_uses` | 虚调用、函数指针、模板实例化、构造/析构、运算符和转换等不能伪装成普通直接调用的使用 |

落点规则已冻结：公开类型、公开自由函数声明、枚举、别名、常量、必须可见的类内定义、模板和 inline 定义进入唯一同基本名 `.h`；非 inline 类外成员/自由函数进入唯一同基本名 `.cpp`；只服务本实现且未导出的辅助可留同名 `.cpp` 最小可见域。每个 `.cpp` 第一项目 include 是同基本名 `.h`。清单不得要求复制 DTO、以文本 include 实现、前置声明替代完整值成员，或新增机器语义。

### 4.4 完整性判定与失败收口

清单只有同时满足才可发布：

- 57/57 模块 BMI 与 AST 语义解析成功；全部消费者 AST 成功或有具名、可复现的原编译失败证据；
- 每个 AST 定义都恰有一个落点；每个 `.cpp` 定义都能回指一个 `.h` 声明，例外只能是 private helper；
- 每个 public/import 可达实体保留公开声明；每个语义调用方/间接调用标识均可回读；
- 模块、消费者、工程三份闭包清单与 Git blob 逐项一致；
- `rg` 的词法候选定义只是较低保障的覆盖计数，不得比 AST 定义数更多而无逐行解释；
- 三个交接验证源均显示为 `HEAD` 读取、`.cpp` consumer、未写入工作区。

任一 module BMI 失败、AST 不能唯一标识实体、调用引用不能解析、布局须改变、未知消费者/工程项出现、或 WIP 与计划写入范围冲突时：停止在隔离目录，写失败记录与最小复现命令；不得改源码、工程、WIP、规范或当前暂停计划。

## 5. 实施与发布

1. 执行者先完成 S0：确认 `main == origin/main == 89697ac7`，index 空、正式输入 blob、外部占用和三项 handoff WIP 状态；任何源码/工程基线变化使本计划退出并由计划智能体重建。
2. 建立第 4 节隔离目录并生成三份闭包清单、BMI、AST 和实体清单；此时不建立源码备份，因为未修改源码。
3. 将可审计的 `模块清单.json`、`消费者闭包.json`、`工程闭包.json`、`语义实体迁移清单.json` 作为本计划施工记录附件提交，外部大体积 AST/BMI 只以 hash、命令、路径和保留期限记载；不得把数百 MB AST 无审查地纳入仓库。
4. 复核后由计划智能体建立下一份实际迁移计划。它必须把三份清单作为不可变输入，冻结所有 57 个 `.ixx` 的 `.h/.cpp` 映射、两个 `.cppcpp` 的承接、所有消费者和全部工程项。该后继才可以取得代码文件所有权、做 Git 白名单备份，并按用户确定的“顶层到低层编辑、全闭合后一次编译”的顺序施工。

本计划施工记录固定为 `施工记录/20260914_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A_施工记录_v0.1.md`，验证记录固定为 `验证记录/20260914_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A_验证记录_v0.1.md`。

验证命令为：对每个输出 JSON 执行 JSON 解析、hash 对照和表间引用一致性检查；`git diff --check`；`python .\tools\check_specs.py --strict`。本叶不运行根工程或验证工程构建，因为没有源码/工程改动，且构建不证明语义清单完整。

## 6. 完成声明边界

本叶完成仅证明：基于 `89697ac7` 的全消费者闭包已被语义分析并冻结为下一计划可消费的逐实体清单，三个删除 WIP 已按用户授权被纳入未来 `.cpp` 消费者范围但未被触碰。它不证明实际模块迁移、编译、链接、运行、恢复、业务语义或全项目无模块格式。
