# 世界树根启动专项脚本迁移详细设计

设计身份：`PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-WORLD-TREE-ROOT-SCRIPT`

日期：2026-09-18
版本：v0.1
状态：施工设计

## 1. 目标与当前事实

迁移后消费者闭包审计确认 `验证工具/运行世界树根启动专项验证.ps1` 在构建前以 `ReadAllText` 读取四个旧格式路径，构成四条活动消费者依赖。该专项由已发布的世界树根启动计划建立，仍承担 Debug/Release 隔离重建与 core/ordinary 两种运行入口验证，因此处理方式冻结为迁移，不是删除。

旧路径与已发布替代一一对应：

| 旧路径 | 现行路径 |
| --- | --- |
| `海中鱼巣/装配.普通应用.ixx` | `海中鱼巣/装配.普通应用.h` |
| `海中鱼巣/装配.普通应用.cppcpp` | `海中鱼巣/装配.普通应用.cpp` |
| `海中鱼巣/领域/合同.世界树根.ixx` | `海中鱼巣/领域/合同.世界树根.h` |
| `海中鱼巣/领域/数据服务.世界树根.ixx` | `海中鱼巣/领域/数据服务.世界树根.h` |

完成条件是脚本改对现行 `.h/.cpp` 结构做等价的物理边界检查，并保留原有隔离构建、两种运行与 JSON 结果合同。

## 2. 唯一允许范围与冻结检查

唯一生产外变更文件：

```text
验证工具/运行世界树根启动专项验证.ps1
```

允许新增本计划专属施工/验证记录。禁止修改世界树根专项 `.vcxproj`、生产头/源、测试 EXE、其它验证工具、正式规范和当前异主 WIP。

脚本必须：

1. 读取 `.h`、`.cpp` 和两份世界树根公开头，而非旧格式路径；
2. 对 `装配.普通应用.h` 拒绝 `module`、`import`、`export module`，并拒绝 `构造普通应用上下文` 的函数体；
3. 对 `装配.普通应用.cpp` 要求 `#include "装配.普通应用.h"`，要求定义 `构造普通应用上下文`，并拒绝 `module 海中鱼巣.装配.普通应用;`；
4. 对两份世界树根公开头的拼接文本继续拒绝已退出符号 `发布世界树根节点`，并拒绝 module/import/export 旧格式；
5. 不改变既有 `$OutputRoot` 白名单、Debug/Release、`/m:1`、隔离 `OutDir/IntDir`、`world-tree-root-special.exe`、core 运行、`--ordinary` 运行、退出码或 JSON 的既有结果字段。

## 3. 实施顺序与失败收口

1. S0 从正式 HEAD 重读脚本和四个替代文件，确认当前 `.h/.cpp` 路径、函数名称和专项工程保持可读；若迁移事实已漂移，停止并退回计划支撑，不修改生产文件以迎合脚本。
2. 仅替换四个路径变量及其文本读取目标。
3. 用 §2 的 header/source 断言替代旧 module 断言；断言必须在 `New-Item`、MSBuild 和执行 EXE 之前失败，避免错误输入触发构建。
4. 保留并运行原有 Debug/Release 构建、core/ordinary 执行与 JSON 输出。路径不存在、文本结构不符或非零退出都抛出具名失败，不得转换为 PASS。

## 4. 验证与声明边界

脚本自身须在 Debug 与 Release 都从任务专属 `D:\TEMP\海中鱼巣\WORLD-TREE-ROOT-BOOTSTRAP\<轮次>\special` 运行成功，两个配置的 `result.json` 都记录 `RebuildExit=0`、`CoreExit=0`、`OrdinaryExit=0`、`InterfaceSplit=PASS`、`PublicRootPublisher=ABSENT`。另检查脚本不再含四个旧路径、`.ixx`、`.cppcpp` 或 module-specific 断言。

本迁移只证明专项脚本已消费当前头源物理形态且原专项仍可运行；不证明世界树根业务语义、恢复、其它验证消费者或全项目闭环。
