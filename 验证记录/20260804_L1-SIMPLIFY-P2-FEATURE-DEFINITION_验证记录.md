# L1-SIMPLIFY-P2-FEATURE-DEFINITION v0.1 验证记录

## 验证基线

- 计划 blob：`17200b9904118a5c96ee6360f8d64661105f96a5`
- 实施起点：`35dfd18385aa00685138d5f78a81ab166bd891c2`
- 发布前正式复核基线：`a6e0dc169211ce843e4f1463fa301acd0d1e0f83`
- Git index 在验证前为空；其它所有者 WIP 保留。

## 命令与结果

| 命令 | 结果 |
| --- | --- |
| `msbuild .\海中鱼巣.vcxproj /m:1 /t:Rebuild /p:Configuration=Debug /p:Platform=x64` | PASS |
| `x64\Debug\海中鱼巣.exe --self-test-exit` | PASS，完整自检 26/26 |
| `msbuild .\海中鱼巣.vcxproj /m:1 /t:Rebuild /p:Configuration=Release /p:Platform=x64` | PASS |
| `x64\Release\海中鱼巣.exe --self-test-exit` | PASS，完整自检 26/26 |
| `python .\tools\check_specs.py --strict` | PASS，110/110 |
| `git diff --check`（九文件范围） | PASS |
| `扫描.MSVC模块依赖.ps1 -配置 Debug -平台 x64` | PASS，sources=185、modules=185、failed=0、循环模块=0 |

## 静态门禁

- L2 数据头禁止的 L1 写机制类型命中为 0。
- 服务仅导出导入 `海中鱼巣.核心.合同.L1公共事实`，L1 服务为普通 import。
- 私有幂等域 `0x03`、`0x04` 各唯一；定义写集无关系、无索引。
- 工程登记数据头、服务模块和专项自检模块；顺序 310 唯一，完整自检登记/执行数量为 26。
- 临时 `iostream`、`cout`、`cerr` 和 `P2-EVIDENCE` 诊断已清零。

## 完成边界

验证证明本叶 I64 区间抽象特征定义的 L1 原子登记、定义、独立同代次读回和运行期持有；不证明实例槽、关系 22、特征批次、比较、恢复、生产初始化、旧特征体系迁移/删除或集成验收。
