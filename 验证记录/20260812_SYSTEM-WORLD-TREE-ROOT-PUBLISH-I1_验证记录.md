# SYSTEM-WORLD-TREE-ROOT-PUBLISH-I1 验证记录

日期：2026-08-13

## 已运行

| 验证 | 命令 / 方法 | 结果 |
| --- | --- | --- |
| I1 专项 Debug fresh | `验证工具/运行系统世界树根建立与正式读回发布参数验证.ps1 -配置 Debug` | PASS；旧 10 / 10，动态 55 / 55，失败 0 |
| I1 专项 Release fresh | 同脚本 `-配置 Release` | PASS；旧 10 / 10，动态 55 / 55，失败 0 |
| 根工程 Debug fresh Rebuild | `MSBuild.exe .\海中鱼巣.slnx /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m /nologo /v:minimal` | PASS；生成 `x64/Debug/海中鱼巣.exe` |
| 根工程 Release fresh Rebuild | 同命令，`Configuration=Release` | PASS；生成 `x64/Release/海中鱼巣.exe` |
| 正式规范目录 | `python .\tools\check_specs.py --strict` | PASS，113 / 113 |
| diff | `git diff --check`、`git diff --cached --check` | PASS |
| UTF-8 / XML | 严格 UTF-8 解码九路径文本；PowerShell XML 解析根工程、filters、专项工程 | PASS |

专项双配置每轮均实际运行：真实普通应用旧 10 项全部通过；动态组 1—7 共 55 项全部通过。动态组覆盖准入与损坏槽防御、顺序/重放/重复、四阶段失败 optional 格、重组/保留、L2 与 I1 坏形状归层、首次/待收敛/已发布状态守恒、模板锁并发计数和真实来源分账。组 7 专项并发实际计数为根写 1、登记读取 5、整树读取 4，来源为 1 次首次建立和 3 次已发布重复。

## G5B-12 缺陷与修复证据

修复前 fresh Debug 中 G5B-01—11 PASS，G5B-12 首次且唯一 FAIL：坏旧发布独立截止被本轮正式读回纠正后返回 `精确重复 / 已发布重复消费`。在 `.inl` 私有既有发布互证分支补四项截止等式后，Debug/Release 均返回 `根读回不一致`；根写零调用，结果仅携带二次登记和整树读取 optional，待收敛槽保持空，旧发布值逐字段不变。随后根工程 Debug/Release fresh Rebuild 均通过。

## 四层证据

1. seam 动态层：专项脚本 specialization 的 55 / 55 证明同一 `.inl` 源码在预置 L2 结果、异常和并发下的分支、顺序、optional 和状态槽行为；不证明生产二进制实例或真实资源耗尽。
2. 真实 R1 / 普通应用层：旧 10 项及组 7 真实路径证明装配、A1/直接 getter 同址、首次来源、已发布重复来源、真实同请求并发和值式隔离。
3. 静态源码层：`.inl` 零 module/export/include/验证词；主接口与专项各唯一 include；工程/filters 各唯一 `ClInclude`；wrapper 仅 A1 getter 取引用并一次委托；A1 聚合服务零写；5 个固定身份值全异；独立 I1 `.cpp/.ixx` 与生产验证痕迹零命中。
4. 构建 / 工程层：专项与根工程完整真实依赖图 Debug/Release 均 fresh 编译链接运行通过。专项构建唯一持续 warning 为 MSBuild `MSB8029`：隔离输出/中间目录位于系统临时目录，未形成编译、链接或运行失败。

## 静态与范围门禁

- `git diff 34b9cbcc8e10429cfccba4d035df530f06c6ea51 --` 对 I1 DTO、`启动.应用程序.ixx`、`程序运行结果.数据.h` 三条精确路径零变化。
- I1 固定身份 `0x5359535754520001` 与场景身份来源、关系类型、属性类型、树结构登记四个保留值机械验证 5 / 5 唯一。
- `.inl` 只以 `ClInclude` 登记，不存在 `ClCompile`、模块扫描、分区或手填模块依赖；独立 I1 `.cpp/.ixx` 实现文件零命中。
- 暂存前 index 为空；最终只允许九条白名单进入结果提交，异主流程图、路线输入和 `.codex-build` 原地保留。

## NOT_RUN

- 真实分配器耗尽；专项 `bad_alloc` 只证明异常映射；
- 持久化、崩溃、恢复、跨进程和长时生产；
- 正式启动对 I1 的调用、阶段 14、I1 成功结果交给 I2、阶段 15—18；
- I2、真实自我、概念维度根；
- 独立集成验收。

因此本记录不证明正式入口可达、阶段接线、自我初始化、跨进程身份或完整系统闭环。
