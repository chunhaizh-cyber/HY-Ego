# 世界树根启动专项脚本迁移验证记录

计划身份：`PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-WORLD-TREE-ROOT-SCRIPT`

日期：2026-09-18

## 已执行检查

| 检查 | 命令或证据 | 退出码 | 结果 |
| --- | --- | ---: | --- |
| PowerShell 语法 | `[System.Management.Automation.Language.Parser]::ParseFile(...)` | 0 | 通过 |
| 目标 diff 格式 | `git diff --check -- 验证工具/运行世界树根启动专项验证.ps1` | 0 | 通过 |
| 正式规范目录 | `python .\\tools\\check_specs.py --strict` | 0 | 126/126 通过 |
| 旧格式脚本字面量 | `rg -n -i '\\.ixx|\\.cppcpp|^\\s*(export\\s+)?module\\b|^\\s*(export\\s+)?import\\b' -- 验证工具/运行世界树根启动专项验证.ps1` | 1（rg 无匹配） | 通过：无旧格式路径或旧语法字面量 |
| Debug 专项 | `powershell -NoProfile -ExecutionPolicy Bypass -File .\\验证工具\\运行世界树根启动专项验证.ps1 -Configuration Debug` | 1 | 失败，见下文 |

## Debug 失败证据

隔离输出根为：

```text
D:\TEMP\海中鱼巣\WORLD-TREE-ROOT-BOOTSTRAP\01a06f6b-20260912\special\Debug
```

`build.log` 记录三个独立的计划外阻断：

1. `采集器.D455相机.cpp` 不能找到 `librealsense2/h/rs_context.h`；
2. `世界树根启动专项验证.cpp` 仍使用旧世界树请求/结果 ABI，和现有 `世界树场景创建请求_v4`、`世界树存在创建请求_v4`、`建立世界树应用服务` 签名不匹配；
3. `数据服务.概念树类.cpp` 报 C1128，当前专项工程未以 `/bigobj` 编译。

本计划禁止修改专项 C++、专项工程及生产头源；且详细设计要求路径、结构或运行事实不一致时停止。因此未运行 Release：Debug 的构建前提已经确定不满足，不能用同一错误输入继续生成无效的 Release 结论。

没有生成任何配置的成功 `result.json`，故下列合同尚未获得验证：`RebuildExit=0`、`CoreExit=0`、`OrdinaryExit=0`、`InterfaceSplit=PASS`、`PublicRootPublisher=ABSENT`。

## 声明边界

本记录只证明脚本已迁移到现行 `.h/.cpp` 探针且静态格式检查通过；不证明 Debug/Release 专项构建、core/ordinary 运行、JSON 成功合同、世界树根逻辑、恢复或其它消费者。
