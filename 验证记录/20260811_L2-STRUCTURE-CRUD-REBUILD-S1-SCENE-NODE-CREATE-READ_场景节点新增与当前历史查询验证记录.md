# L2 场景节点新增与当前历史查询验证记录

日期：2026-08-11

计划身份：`L2-STRUCTURE-CRUD-REBUILD-S1-SCENE-NODE-CREATE-READ` v0.1

验证基线：施工起点 `42e4389400f574faf9bfa181c8fa0ca4cba5a1f9` 加本计划十一文件未提交 WIP

## 1. 主工程构建

执行：

```powershell
MSBuild.exe .\海中鱼巣.vcxproj /m:1 /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /verbosity:minimal
MSBuild.exe .\海中鱼巣.vcxproj /m:1 /t:Rebuild /p:Configuration=Release /p:Platform=x64 /verbosity:minimal
```

结果：Debug 与 Release x64 均完整扫描模块、编译并链接成功，分别生成 `x64/Debug/海中鱼巣.exe` 与 `x64/Release/海中鱼巣.exe`。该结果只证明生产工程在两项具名配置下可编译链接，不证明新服务已经生产装配或被业务消费者调用。

## 2. 生产外参数矩阵

执行：

```powershell
powershell -ExecutionPolicy Bypass -File .\验证工具\运行L2场景节点参数验证.ps1 `
  -配置 Debug -期望事实代次 0 -幂等身份 101
powershell -ExecutionPolicy Bypass -File .\验证工具\运行L2场景节点参数验证.ps1 `
  -配置 Release -期望事实代次 0 -幂等身份 202
```

Debug 与 Release 均取得相同结构化事实形状：

| 验证项 | 结构化结果 |
| --- | --- |
| 合法新增 | `已提交(1)`；截止 / 变更 / 场景创建代次均为 1；场景编码非零，三个组为空 |
| 精确重复 | `精确重复(2)`；返回同一身份、截止和变更代次；L1 当前代次仍为 1 |
| 同键异义 | `幂等冲突(11)`；空场景、空变更代次；L1 当前代次仍为 1 |
| 当前读取 | `已读取(3)`；截止 1；身份和生命周期与首次结果逐字段一致 |
| 历史读取 | `已读取(3)`；前后代次夹取截止 1；身份和生命周期与首次结果一致 |
| 零幂等 / 零场景身份 | 均为 `入口拒绝(5)`、空场景；前后 L1 当前代次均为 1 |
| 当前读取期望代次 2 | `事实代次漂移(10)`；回显实际截止 1；空场景、空变更代次 |
| 合同版本错误 | `入口拒绝(5)`、空场景；前后 L1 当前代次均为 1 |

两项配置均打印 `验证结论：PASS，失败数=0`。外部项目构建出现 MSB8029 警告：MSBuild 不建议把中间 / 输出目录置于系统临时目录；这是计划要求的工作区外隔离路径提示，实际 Rebuild、链接和程序运行均成功，且工作区没有验证生成物。

## 3. 静态与工程门禁

- `python .\tools\check_specs.py --strict`：`111 份目录项全部通过`。
- `海中鱼巣.vcxproj`、`海中鱼巣.vcxproj.filters`、`验证工具/L2场景节点参数验证.vcxproj` 均通过严格 UTF-8 解码和 XML 解析。
- 主工程登记：生产 `.ixx` 为 25、`ClInclude` 为 11、四配置 `/utf-8` 为 4 项。
- 外部验证项目在主工程和 filters 中均为零登记。
- 入口、启动与普通应用装配对两个新 L2 模块零 import；没有生产消费者接线。
- 两个新 `.ixx` 都以 `ClCompile` 登记，且不存在把 `.ixx` 作为文本头 `#include` 的路径。
- `git diff --check` 对两个已跟踪工程文件无空白错误；未跟踪新文件逐文件检查除 Git 的 LF→CRLF 工作区提示外无空白错误。最终 staged `git diff --cached --check` 由唯一发布者在精确暂存后复核。

## 4. 未验证范围与结论边界

未运行 Win32 构建、生产入口运行、真实消费者调用或独立集成验收；计划不要求这些范围。本记录不证明父子 / 成员关系、固定属性、退出、引用保护、其它五类 L2 结构、聚合面、生产装配、业务真实性、完整 L2 或项目闭环。

结论：S1 的无属性、无关系场景节点新增、精确重复 / 冲突、当前单节点和历史单节点读取，在 Debug / Release x64 构建及具名自定义参数矩阵下通过。Git 提交和推送事实仍待顶层唯一发布者完成。
