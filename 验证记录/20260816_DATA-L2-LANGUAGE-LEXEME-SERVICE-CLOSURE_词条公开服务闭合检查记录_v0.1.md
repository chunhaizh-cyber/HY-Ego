# DATA-L2 词条公开服务闭合检查记录

日期：2026-08-16

计划：`DATA-L2-LANGUAGE-LEXEME-SERVICE-CLOSURE v0.1`

## 1. 结论

根工程 x64 Debug / Release fresh Rebuild 通过；仓库外 Debug / Release 合法消费者各 `35/35 PASS`。代码建立了词条公开 DTO、独立词条 owner、四个公开服务函数及权威当前 / 历史读回，但普通应用装配、真实生产消费者和完整 `DATA-EXT-10` 均未运行。

## 2. 根工程双配置

使用两个新的系统临时 `OutDir / IntDir` 对根 `海中鱼巣.vcxproj` 执行：

```text
MSBuild.exe .\海中鱼巣.vcxproj /t:Rebuild /m:1 /p:Configuration=Debug /p:Platform=x64 /p:OutDir=<Debug临时目录> /p:IntDir=<Debug临时目录>
MSBuild.exe .\海中鱼巣.vcxproj /t:Rebuild /m:1 /p:Configuration=Release /p:Platform=x64 /p:OutDir=<Release临时目录> /p:IntDir=<Release临时目录>
```

- Debug：退出 `0`，1 个预期 `MSB8029`，0 错误；
- Release：退出 `0`，1 个预期 `MSB8029`，0 错误；
- 两次均生成同一生产程序 `海中鱼巣.exe`；
- 未读取、写入或清理异主 `.codex-build`。

## 3. 仓库外双配置合法消费者

专项源码位于仓库外系统临时目录，直接导入正式 L1、不可变材料和语言结构模块，并链接同配置根工程模块对象；未登记生产工程，未修改启动、入口或普通应用。Release 使用 `/W4 /WX /utf-8 /permissive- /MD`，Debug 使用 `/W4 /WX /utf-8 /permissive- /MDd`，仅对 MSVC Debug IFC 环境提示 `C5050` 定点禁用，编译、链接和运行均退出 `0`：

```text
DATA-L2-LEXEME-SERVICE 35/35 PASS
```

矩阵覆盖：

- 同 L1 双 owner 成功构造、异 L1 交付拒绝；
- 材料服务和五份正式材料写入，错误 UTF-8 内容为空载荷拒绝；
- 语言未建立时词条新增失败，语言建立后词条新增和完整权威读回；
- 新增精确重放保持原身份与原截止、同幂等异义冲突、同键异幂等占用冲突；
- 按身份和规范化键的当前 / 历史读取；
- 语言、规范化合同、规范化词面三维键分账；
- 坏身份入口拒绝、事实代次漂移、所有非成功空载荷；
- 活动词条对语言和材料的跨 owner 双向引用保护；
- 词条退出、退出精确重放、退出后当前不可见、身份历史可读；
- 规范化键只选择具名截止活动词条，在退出截止不返回旧活动对象；
- 词条关系关闭后语言和材料可以分别退出。

## 4. 静态门禁

发布前执行并要求通过：

- `python .\tools\check_specs.py --strict`；
- 根工程 `vcxproj / filters` XML 解析与两个生产文件唯一登记；
- 两生产文件与两记录严格 UTF-8 无 BOM；
- `git diff --check`、`git diff --cached --check`；
- 允许范围、启动 / 入口 / 普通应用零本计划差异；
- 生产源码无专项标记、断言计数或验证分支；
- 仓库外专项源码、对象、程序、PDB、日志与材料实际文件归零。

发布前已在正式 `c0dae4e3d4b97db9a14916e47ef2338864a2b125` 基线上复核：四个本任务专属系统临时目录均不存在；HEAD 与 `origin/main` 一致、ahead / behind 为 `0 / 0`、Git index 为空。目录由用户人工删除，执行者未绕过工具安全策略。

## 5. NOT_RUN

资源耗尽、发布结果未知、长时间并发调度、跨进程恢复、普通应用装配、真实生产消费者、真实 BIZ 消费、语言记录、概念名称、语素、词性、分词、规范化算法及完整 `DATA-EXT-10` 保持 `NOT_RUN`。

本检查只证明具名词条公开服务切片及其生产外矩阵，不证明数据服务最终需求清单整体满足。
