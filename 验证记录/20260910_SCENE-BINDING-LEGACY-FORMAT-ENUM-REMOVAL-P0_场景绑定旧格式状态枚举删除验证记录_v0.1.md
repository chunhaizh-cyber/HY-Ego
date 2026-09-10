# 场景绑定旧格式状态枚举删除验证记录

计划身份：`SCENE-BINDING-LEGACY-FORMAT-ENUM-REMOVAL-P0`

验证日期：2026-09-10

输入基线：`0c9bbbd8409df533d33a4da79fa1a51779bd3978` 加本计划唯一生产候选

## 1. 验证环境与输入

- MSBuild：`C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe`，18.5.4.18101（.NET Framework MSBuild 18.5.4+cb4e32d21）。
- 工具集与选项：v145、C++20、UTF-8、x64、`bcrypt.lib`、`/m:1 /v:minimal`。
- 仓外独占目录：`D:/TEMP/海中鱼巣/SCENE-BINDING-LEGACY-FORMAT-ENUM-REMOVAL-P0/20260910-p0`。
- 工程：`legacy_enum_p0.vcxproj`，SHA256 `0CFBA7987EE0EB377DB87FD7FBFF895664A72077A8301A618B773D98D510C985`。
- 探针：`legacy_enum_p0_probe.cpp`，SHA256 `D25B57BBFD761052A0FC79EF7A9A789999D69BFC4E15AF74E9D154694F4D4334`。

关键生产输入 SHA256：

- B合同候选：`6E20377ACE094DB95BA1BCDCD6631ABC2C36665274AEB15F5684C169A3B163B0`；
- 场景类：`560267729F4DB6B893B5651C3934B1949488AB7917DE5ABE0885D6725F12A740`；
- 动态合同/服务：`A75C389CE3FEB0C66D16A6F61885CFC5181F530C717B965878C0B96B8DC8ED3E` / `9F57317051D64541137517FFB84C96D43E9FF7610FD2573021F9C59FF39C1995`；
- 因果合同/服务：`C5CE1625D32B93D75252561B5CE89EB95287F382C854A49D164AE149A5DC3CCD` / `FB2E8B918E3F0329BFCBD10DCCEB4D57A68C333E032497DA4038F1AB22E9A0D3`。

工程按实际模块依赖编译17个正式模块：L1六模块、特征值、特征、有序I64比较、状态、存在身份窄合同、B合同、场景、动态合同/服务、因果合同/服务，另编译一个仓外探针；没有修改生产工程登记。

## 2. 静态和形状检查

| 检查 | 结果 |
| --- | --- |
| 生产 `状态使用绑定数据状态::旧格式不支持` 与 `旧格式不支持=14` 扫描 | 零命中，PASS |
| 目标枚举其它 `=14` 成员 | 零命中，PASS |
| 合法成员数量与数值 | 22项；`1..13,15..23`，PASS |
| 底层类型 | `std::uint8_t`，编译期断言通过 |
| 合同版本 | 1，编译期断言通过 |
| `状态使用绑定信息` 字段类型与顺序 | 编译期断言通过 |
| 三字段 `状态使用绑定结构交付` 类型与顺序 | 编译期断言通过 |
| 只读 provider 三个签名 | 编译期断言通过 |
| 场景类七个 B 入口签名 | 编译和取成员指针通过 |
| 创建/读取/退出成功谓词无空载荷假成功 | Debug/Release运行通过 |
| 起点仅删除目标成员后的逐字比较 | True，PASS |
| 根工程 `海中鱼巣.vcxproj` 与 filters | 零差异 |

## 3. 构建与运行

Debug命令：

```powershell
& $msbuild $project /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m:1 /v:minimal
& D:/TEMP/海中鱼巣/SCENE-BINDING-LEGACY-FORMAT-ENUM-REMOVAL-P0/20260910-p0/Debug/bin/legacy_enum_p0.exe
```

结果：Rebuild、编译、链接退出0；探针退出0。可执行文件 SHA256 `89700084C981EFD94372CAF5C2C2CCF603BF08F4666C21EE9FB6BABA9E446FA5`。

Release命令同上，仅 `Configuration=Release`。结果：Rebuild、编译、链接退出0；探针退出0。可执行文件 SHA256 `983C598A55504B8B41A780B799354399497E4FF1B6C2CF0A27E1C8F6A19CB2F1`。

## 4. 差异与规范检查分账

- `git diff --check -- 海中鱼巣/领域/合同.场景状态使用绑定.ixx`：退出0。
- 全工作区 `git diff --check`：退出2，仅本次观察到异主 `AGENTS.md:12` 既有尾随空格；目标文件未产生空白错误，未修改异主文件。
- `python tools/check_specs.py --strict`：118份目录项，1错误，退出1；唯一错误为 `tools/l1_frozen_surface_manifest.json: FROZEN 必须登记存在的 acceptance_record`，属于范围外既有现场，未补造、恢复或暂存。
- 根工程未构建。上述仓外工程证明当前合法源消费者闭包可重新编译、链接并运行探针，不证明生产装配、真实绑定运行、恢复或业务验收。

## 5. 结论

本 P0 验证通过：B 合同旧格式死枚举已从候选源码删除，14未复用，22个合法枚举数值与其余受检源码 ABI 形状保持，场景/动态/因果真实 import 闭包在 Debug/Release 中重新编译、链接并运行通过。全工作区检查的两个范围外失败不由本计划掩盖，也不升级为本切片失败或全仓通过。
