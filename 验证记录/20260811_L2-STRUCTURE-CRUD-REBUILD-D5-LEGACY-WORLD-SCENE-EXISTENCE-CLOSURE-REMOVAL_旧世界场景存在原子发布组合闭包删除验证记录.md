# 旧世界场景存在原子发布组合闭包删除验证记录

日期：2026-08-11

计划身份：`L2-STRUCTURE-CRUD-REBUILD-D5-LEGACY-WORLD-SCENE-EXISTENCE-CLOSURE-REMOVAL v0.1`

验证基线起点：`823aa990eef02e29139344b25751fae5e9ed5159`

## 1. 验证结果

| 验证项 | 结果 | 证据边界 |
| --- | --- | --- |
| 九个目标路径不存在 | 通过 | 九个冻结路径逐项 `Test-Path=False` |
| 模块、服务、DTO 和十二项工程 / filters 登记归零 | 通过 | 对生产源码、vcxproj、filters 的具名 `rg` 扫描零命中，退出码 1 |
| 禁止文件未进入差异 | 通过 | 实际代码差异仅含九个删除源码及两个工程聚合文件 |
| Debug x64 Rebuild | 通过 | MSBuild 18.5.4，退出码 0，生成 `x64/Debug/海中鱼巣.exe` |
| Release x64 Rebuild | 通过 | MSBuild 18.5.4，退出码 0，生成 `x64/Release/海中鱼巣.exe` |
| 工程 UTF-8 XML | 通过 | 两工程文件经严格 UTF-8 解码及 BOM 感知 XML 解析成功，声明均为 `encoding="utf-8"` |
| 四配置 `/utf-8` | 通过 | `海中鱼巣.vcxproj` 中 `/utf-8` 保持 4 项 |
| `git diff --check` | 通过 | 退出码 0；异主流程图和路线文件只产生既有换行提示 |
| `python .\tools\check_specs.py --strict` | 通过 | 110/110 正式规范目录项通过 |

## 2. 构建命令

```powershell
MSBuild.exe .\海中鱼巣.vcxproj /m:1 /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /verbosity:minimal
MSBuild.exe .\海中鱼巣.vcxproj /m:1 /t:Rebuild /p:Configuration=Release /p:Platform=x64 /verbosity:minimal
```

## 3. 明确未执行

依计划未运行程序、CRUD、生产模式、自检、功能测试、参数黑盒或集成验收。编译链接只证明目标闭包退出后的工程静态闭合。

## 4. 结论

D5 的九个旧世界 / 场景 / 存在 / 原子发布 / 组合源码及十二项工程登记已归零，Debug 和 Release x64 均完整重建成功。

结合 D1—D4 结果只可声明具名旧 L2 删除链退出；不证明 G1、新 L2、其它 legacy 或项目业务闭环完成。
