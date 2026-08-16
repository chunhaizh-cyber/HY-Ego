# DATA 语言材料普通应用装配与首次权威读回检查记录

日期：2026-08-16

计划：`DATA-LANGUAGE-ORDINARY-APP-ASSEMBLY-READBACK v0.1`

执行 S0：`d1ba93f08bca5da7aa1cf7d02fd7c1c6a4775646`

## 1. 当前结论

三个生产文件已完成计划内实现。根工程 x64 Debug / Release fresh Rebuild 均退出 `0`；仓库外普通应用合法消费者 Debug / Release 各 `35/35 PASS`；strict、XML、编码、工程登记、diff、范围和生产零测试痕迹静态门禁通过。`D:\TEMP` 验证治理提交 `d61c0b6f4b1fc4b8cca331162ce0ee9a4cebdc2a` 已发布并从本次新 S0 生效；本次增量 S0 确认 `HEAD == origin/main`、ahead / behind `0/0`、index 空。旧系统临时目录的普通残留按 CODE-FILE-01 v2.8 分账为待清理债务，不否定既有验证，也不再阻断结果发布；发布前只剩最终精确 Git 范围复核。

## 2. 根工程双配置

使用系统临时 `OutDir / IntDir` 对根 `海中鱼巣.vcxproj` 执行 fresh Rebuild：

```text
MSBuild.exe .\海中鱼巣.vcxproj /t:Rebuild /m:1 /p:Configuration=Debug /p:Platform=x64 /p:OutDir=<系统临时Debug目录> /p:IntDir=<系统临时Debug目录>
MSBuild.exe .\海中鱼巣.vcxproj /t:Rebuild /m:1 /p:Configuration=Release /p:Platform=x64 /p:OutDir=<系统临时Release目录> /p:IntDir=<系统临时Release目录>
```

- Debug：退出 `0`；
- Release：退出 `0`；
- 两配置只有临时 `OutDir / IntDir` 引起的预期 `MSB8029`；
- 未使用、写入或清理异主 `.codex-build`。

## 3. 仓库外普通应用消费者

专项源码和全部产物位于：

```text
C:/Users/zhchh/AppData/Local/Temp/hzy-data-language-assembly-readback-b7f51d30a2b04a2a965986a97d116a7a
```

专项直接导入正式普通应用、材料、语言、概念和概念聚合模块，链接同配置根工程模块对象且不链接生产 `入口.obj`。Debug 使用 `/std:c++20 /W4 /WX /utf-8 /permissive- /MDd`，Release 使用 `/std:c++20 /W4 /WX /utf-8 /permissive- /MD /O2`；两配置编译、链接和运行均退出 `0`：

```text
DATA-LANGUAGE-ORDINARY-APP-ASSEMBLY-READBACK 35/35 PASS
```

实际覆盖：

- 生产配置成功、固定 LocalAppData 三段路径和重复形成；
- 坏合同、相对路径、含 `..` 路径纯值拒绝；
- 配置无效及新增 32—38 装配失败状态的空上下文形状；
- 普通应用完整装配，材料 / 语言 const 与 non-const getter 同实例且重复地址稳定；
- 三个固定 owner 建立身份两两不同，语言 / 词条实际 owner 身份不同；
- 从材料登记的结构化代次漂移回显取得首个当前代次；
- 四份严格 UTF-8 材料写入，首份精确重复返回原身份 / 原截止；
- 四材料按身份当前 / 历史读取和失败空载荷；
- 语言新增、按身份 / 登记键当前与历史读取；
- 词条新增、按身份 / 规范化键当前与历史读取；
- 材料、语言、词条身份、来源、规范化键、生命周期和事实截止互证；
- 概念聚合 const / non-const 同实例，完整概念服务当前空根组读取成功；
- 上下文销毁后专项运行期材料目录已不存在。

专项未直达 L1、owner 写端口、材料 / 语言内部 helper，未进入生产工程，未向生产源码增加断点调用、验证模式、条件分支、断言计数或测试状态出口。

## 4. 静态门禁

已执行：

- `python .\tools\check_specs.py --strict`：`113/113 PASS`；
- `海中鱼巣.vcxproj` 和 `.filters` XML：解析通过；
- 三个生产模块在 `.vcxproj / .filters` 均为唯一登记 `1/1`；
- 普通应用对材料、语言模块各一个正式 import；
- 五目标文件严格 UTF-8 无 BOM：`5/5`；
- 四配置 `/utf-8`：`4/4`；
- 全工作区 `git diff --check` 与五目标限定 diff-check：退出 `0`；
- 当前白名单恰为三个生产文件和两条专属记录；
- 三个生产文件中专项身份、幂等值、`35/35`、专项 / 测试标记：`0`。

发布前仍须执行：

- 只暂存五目标后的 `git diff --cached --check` 与 cached 白名单；
- 最新 HEAD、远端、ahead / behind、index 和远端读回。

清理债务：旧系统临时根 `C:/Users/zhchh/AppData/Local/Temp/hzy-data-language-assembly-readback-b7f51d30a2b04a2a965986a97d116a7a` 仍有 `367` 个普通构建 / 专项文件；无进程、锁、共享资源或敏感材料占用证据。工具安全策略禁止执行者代删，后续由环境所有者清理；该残留不写成清理 PASS，也不阻断本结果。

## 5. NOT_RUN

真实资源耗尽、Shell API 故障注入、目录权限长期变化、发布结果未知、长时间并发、跨进程恢复、真实语言记录、概念名称、默认语言、分词、规范化算法、真实 BIZ 消费、具名集成验收及完整 `DATA-EXT-08` / `DATA-EXT-10` 均保持 `NOT_RUN`。

本检查只证明本计划的普通应用装配与首次权威读回，不证明数据服务最终需求清单整体满足。
