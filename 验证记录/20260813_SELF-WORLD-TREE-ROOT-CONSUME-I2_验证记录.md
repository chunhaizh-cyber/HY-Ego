# SELF-WORLD-TREE-ROOT-CONSUME-I2 v0.2 验证记录

日期：2026-08-13

## 1. 验证结果

| 层次 | 命令 / 检查 | 结果 |
| --- | --- | --- |
| 根工程 Debug | `MSBuild.exe .\海中鱼巣.slnx /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m /nologo /v:minimal` | PASS；最终 fresh Rebuild 生成 `x64/Debug/海中鱼巣.exe` |
| 根工程 Release | 同命令，`Configuration=Release` | PASS；最终 fresh Rebuild 生成 `x64/Release/海中鱼巣.exe` |
| 专项 Debug | `powershell -ExecutionPolicy Bypass -File .\验证工具\运行自我世界树根正式再读回消费适配参数验证.ps1 -配置 Debug` | PASS，30/30，失败 0 |
| 专项 Release | 同命令，`-配置 Release` | PASS，30/30，失败 0 |
| 正式规范 | `python .\tools\check_specs.py --strict` | PASS，113/113 |
| 编码与工程 | 十二路径严格 UTF-8；root project / filters / 专项 project XML 解析 | PASS |
| Git 文本 | `git diff --check -- <十二路径中的现存路径>` | PASS |

专项构建持续出现 MSBuild `MSB8029`：隔离输出 / 中间目录位于系统临时 GUID 目录。该 warning 未形成编译、链接或运行失败。

## 2. 专项矩阵

真实生产边界覆盖：普通应用真实装配；A1 getter 与普通应用直接 getter 取得同址 R1；真实 I1 首次及精确重复成功且重复携带同次根材料；真实 I2 首次与重复正式两读成功；发布材料按值读回；真实 R1 登记与整树基线；正式 I1 / I2 wrapper 各自对合同版本为零请求返回具名非成功，`成功()==false` 且全部成功载荷为空；普通控制面板与无窗口模式均在宿主前返回 `初始化失败 / 自我初始化能力未实现 = 17`。

脚本 `.inl` 实例覆盖：请求无效时 A1 / R1 零调用；首次和重复各一次登记读与整树读；异请求在 A1 / R1 调用前冲突；登记失败、整树失败；已读取坏 optional、坏截止和坏完整性由 L2 自身拒绝；两个 L2 结果各自成功但树身份、根场景、根标记、事实截止或结构登记彼此漂移时由 I2 拒绝；`bad_alloc` 与其它异常分账；失败旧发布值守恒；返回值式隔离；同请求并发由 I2 锁串行且两次均完整两读。

最初 21 项验证记录存在证据缺口：没有分别具名正式 I1 / I2 provider 非成功动态，跨结果漂移矩阵也不完整。本次补验先扩至 28 项闭合 I1 重复、L2 结果形状和跨结果漂移，再增加两项正式 provider 非成功，最终双配置 30/30 PASS。该纠正仅增强生产外验证，未修改生产实现。

## 3. 静态合同

- `.inl` 零 module / export / include / 验证词，根工程和 filters 对两个新头片段各唯一登记。
- I2 DTO / `.inl` 零 `建立场景树根` 调用；正式代码零验证宏、故障选择、计数器或测试专用入口。
- 程序失败阶段存在 14、16、17，阶段 15 在 `程序失败阶段` 中零定义、零复用。
- 启动源码顺序为 I1 -> 阶段 14 短路 -> I2 -> 阶段 16 短路 -> 阶段 17，且位于模式宿主分支之前。
- 临时专项没有进入 root solution；异主流程图、路线输入和 `.codex-build/` 未进入 I2 施工范围。

## 4. 阶段证据分账

| 阶段 | 静态 / provider 证据 | 真实入口动态 |
| --- | --- | --- |
| 14 现实世界树根初始化 | PASS：启动源码把 I1 非成功映射到 14 并在形成 I2 请求前返回；正式 I1 wrapper 非成功动态 PASS | `NOT_RUN` |
| 15 | PASS：枚举零定义、数值零复用 | 不适用 |
| 16 自我世界树根消费 | PASS：启动源码把 I2 非成功映射到 16 并在阶段 17 前返回；正式 I2 wrapper 非成功动态 PASS | `NOT_RUN` |
| 17 自我初始化能力未实现 | 当前真实自我 provider 缺失 | PASS：普通控制面板与无窗口两种模式真实入口均返回阶段 17 |

## 5. 未验证范围

真实分配耗尽、真实自我 provider 成功 / 失败、阶段 18、概念维度根、方法、自我线程、投影、宿主长时运行、恢复、跨进程、生产发布后的独立消费者读回和独立集成验收均为 `NOT_RUN`。阶段 14 / 16 的生产故障注入未做，因为正式代码禁止验证故障选择；两阶段真实入口动态明确为 `NOT_RUN`。其静态短路接线与 I1 / I2 provider 非成功动态已分别验证，但不得升级为真实入口触发证据。
