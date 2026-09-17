# LOCAL-BINARY-GRID-U64-PARSER 局部二值格 U64 组解析提供者验证记录

日期：2026-09-17
基线：`af0a97d8` 加本切片未提交生产源与工程登记

## 专项运行

临时驱动：`D:\TEMP\海中鱼巣\LOCAL-BINARY-GRID-U64-PARSER\20260917-root-u64grid\局部二值格解析专项.cpp`。

两次均直接编译该驱动与 `数据服务.局部二值格解析类.cpp`，不引入 L1 或其它领域源：

```powershell
cmd /c "vcvars64.bat && cl.exe /std:c++20 /EHsc /utf-8 /Od ..."
cmd /c "vcvars64.bat && cl.exe /std:c++20 /EHsc /utf-8 /O2 /DNDEBUG ..."
```

Debug：编译退出码 0，运行退出码 0。
Release：编译退出码 0，运行退出码 0。

驱动断言覆盖：

| 输入 | 断言结果 |
| --- | --- |
| 轮廓 1 / 4 块 | 成功；`N=8 / 16`，块数正确 |
| 轮廓空组、2 块、3 块、错误版本、空指针 | 对应拒绝且无格式载荷 |
| 体素 `N=4`，全零位图 | 成功；`B=1`、`C=0`、颜色尾段为空 |
| 体素 `N=4`，两个占用位及两个低 24 位颜色 | 成功；`C=2` |
| 体素 `N=8`，8 个全零位图块 | 成功；`B=8`、`C=0` |
| `N=0`、非 4 倍数、短/长载荷、颜色高 40 位非零 | `格式不合法` 且无格式载荷 |
| 可整除 4 的极大 N | `数量超限` 且无格式载荷 |
| 体素错误版本、空指针 | `入口拒绝` 且无格式载荷 |

静态扫描 `数据服务.局部二值格解析类.h/.cpp` 未发现 L1、写端口、写集、节点、关系、材料、概念、特征值类或特征类依赖标识。

## 主工程

```powershell
MSBuild.exe .\海中鱼巣.vcxproj /t:Build /p:Configuration=Debug /p:Platform=x64 /m
MSBuild.exe .\海中鱼巣.vcxproj /t:Build /p:Configuration=Release /p:Platform=x64 /m
```

两配置均退出码 0，0 warning、0 error；日志显示新 `数据服务.局部二值格解析类.cpp` 已进入编译和链接。

## 完整性检查

- `python .\tools\check_specs.py --strict`：126/126 通过。
- `git diff --cached --check`：通过（暂存前索引为空）。
- `git diff --check`：受异主既有 `AGENTS.md:12` 行尾空白阻断，未修改该文件；本切片文件没有报告格式错误。

本验证只证明纯 U64 组格式解析和主工程编译，不证明按 N 的全零体素持久化唯一性、非 I64 特征接线、比较、概念或运行期业务行为。

验证对应结果代码提交：`fc918c00 feat: add local binary grid parser`。

临时验证目录 `D:\TEMP\海中鱼巣\LOCAL-BINARY-GRID-U64-PARSER\20260917-root-u64grid\` 的递归清理被当前执行环境策略拒绝；目录不在仓库、不含生产或提交文件，本切片保留该残留路径以便后续环境允许时清理。
