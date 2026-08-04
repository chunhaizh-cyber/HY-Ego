# L1-SIMPLIFY-P3-FEATURE-INSTANCE v0.2 验证记录

## 验证基线

- HEAD/origin：`648d50077bd71c9a9a3e3065ca2608168fb199b2`
- index：空；其它所有者文档 WIP 未触碰。

## 命令与结果

| 命令 | 结果 |
| --- | --- |
| `msbuild .\海中鱼巣.vcxproj /m:1 /t:Rebuild /p:Configuration=Debug /p:Platform=x64` | PASS |
| `x64\Debug\海中鱼巣.exe --self-test-exit` | PASS，退出 0；P2 非空组合与 P3 顺序自检通过 |
| `msbuild .\海中鱼巣.vcxproj /m:1 /t:Rebuild /p:Configuration=Release /p:Platform=x64` | PASS |
| `x64\Release\海中鱼巣.exe --self-test-exit` | PASS，退出 0 |
| `python .\tools\check_specs.py --strict` | PASS，110/110 |
| `扫描.MSVC模块依赖.ps1 -配置 Debug -平台 x64` | PASS，sources=187、modules=187、failed=0、循环模块=0 |
| 静态禁入项与 `git diff --check` | PASS；临时 iostream/cout/cerr/P2-EVIDENCE 清零 |

## 未验证范围

未进行跨编译器 ABI、生产初始化、集成验收、持久恢复或旧体系迁移删除验证。
