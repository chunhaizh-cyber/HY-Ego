# L2 存在节点新增与当前历史查询验证记录

日期：2026-08-11

计划身份：`L2-STRUCTURE-CRUD-REBUILD-E1-EXISTENCE-NODE-CREATE-READ` v0.1

验证基线：施工起点 `3ec9e183c2e7d866985978d9a95a6fca7f217a72` 加本计划九文件未提交 WIP

## 1. 主工程构建

执行：

```powershell
MSBuild.exe .\海中鱼巣.vcxproj /m:1 /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /verbosity:minimal
MSBuild.exe .\海中鱼巣.vcxproj /m:1 /t:Rebuild /p:Configuration=Release /p:Platform=x64 /verbosity:minimal
```

结果：Debug 与 Release x64 均完整扫描模块、编译并链接成功。该结果只证明生产工程在两项具名配置下可编译链接，不证明存在服务已经生产装配或被业务消费者调用。

## 2. 生产外参数矩阵

执行：

```powershell
powershell -ExecutionPolicy Bypass -File .\验证工具\运行L2存在节点参数验证.ps1 `
  -配置 Debug -期望事实代次 0 -幂等身份 301
powershell -ExecutionPolicy Bypass -File .\验证工具\运行L2存在节点参数验证.ps1 `
  -配置 Release -期望事实代次 0 -幂等身份 302
```

Debug 与 Release 均取得相同结构化事实形状：

| 验证项 | 结构化结果 |
| --- | --- |
| 合法新增 | `已提交(1)`；截止 / 变更 / 存在创建代次均为 1；存在编码非零，固定属性组为空 |
| 精确重复 | `精确重复(2)`；返回同一身份、截止和变更代次；L1 当前代次仍为 1 |
| 同键异义 | `幂等冲突(11)`；空存在、空变更代次；L1 当前代次仍为 1 |
| 当前读取 | `已读取(3)`；截止 1；身份和生命周期与首次结果逐字段一致 |
| 历史读取 | `已读取(3)`；前后代次夹取截止 1；身份和生命周期与首次结果一致 |
| 零幂等 / 零存在身份 | 均为 `入口拒绝(5)`、空存在；前后 L1 当前代次均为 1 |
| 当前读取期望代次 2 | `事实代次漂移(10)`；回显实际截止 1；空存在、空变更代次 |
| 合同版本错误 | `入口拒绝(5)`、空存在；前后 L1 当前代次均为 1 |

两项配置均打印 `验证结论：PASS，失败数=0`。外部项目构建出现 MSB8029 警告：MSBuild 不建议把中间 / 输出目录置于系统临时目录；这是计划要求的工作区外隔离路径提示，实际 Rebuild、链接和程序运行成功，且工作区没有验证生成物。

## 3. 静态与工程门禁

- `python .\tools\check_specs.py --strict`：`111 份目录项全部通过`。
- `海中鱼巣.vcxproj`、`海中鱼巣.vcxproj.filters`、`验证工具/L2存在节点参数验证.vcxproj` 均通过严格 UTF-8 解码和 XML 解析。
- 主工程登记：生产 `.ixx` 为 26、`ClInclude` 为 12、四配置 `/utf-8` 为 4 项。
- 外部验证项目在主工程和 filters 中均为零登记，且只含七个模块源与一个验证 `.cpp`。
- 入口、启动、普通应用装配和场景服务对存在模块零 import；存在模块及外验对场景服务零 import。
- 新存在 `.ixx` 以 `ClCompile` 登记，不存在把 `.ixx` 作为文本头 `#include` 的路径。
- 九文件均通过严格 UTF-8 与空白检查；最终 staged `git diff --cached --check` 由唯一发布者在精确暂存后复核。

## 4. 未验证范围与结论边界

未运行 Win32 构建、生产入口、真实消费者或独立集成验收；计划不要求这些范围。本记录不证明存在固定属性、存在退出、被引用结构查询、场景成员关系、特征、状态、动态、因果、聚合面、生产装配、存在真实性、业务资格、完整 L2 或项目闭环。

结论：E1 的无固定属性存在节点新增、精确重复 / 冲突、当前单节点和历史单节点读取，在 Debug / Release x64 构建及具名自定义参数矩阵下通过。Git 提交和推送事实仍待顶层唯一发布者完成。
