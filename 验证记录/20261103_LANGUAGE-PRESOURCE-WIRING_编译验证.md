# LANGUAGE-PRESOURCE-WIRING 编译验证

日期：2026-11-03。状态：实际项目Debug/x64编译链接通过；未运行程序或测试。

## 1. 精确输入与资源

工作树`D:/worktree/language-preunderstanding-source-20261103/海中鱼巣`，分支`codex/language-presource-wiring-20261103`，起点HEAD `a5fef7721666e9df04e82b1baae9c866454326bb`＋本叶两cpp WIP。计划v0.2／blob `43dddd4b850c767348f0b5442d2fad1397dcf0da`，索引可执行。未消费旧三树WIP。

实际MSBuild 18.11.1／VS18 Professional，平台工具集v145，C++20，Debug/x64。输出为本叶新独占轮次，不复用或改写首叶残留。

## 2. 实际命令

在上述工作树执行，目录只容纳实际工程生成物，没有另建工程或源码副本：

```powershell
$buildRound='D:\TEMP\海中鱼巣\LANGUAGE-PRESOURCE-WIRING\build-20261103-wiring-owner-01'
New-Item -ItemType Directory -Path $buildRound -Force | Out-Null
& 'C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe' '海中鱼巣.vcxproj' /m /t:Build /p:Configuration=Debug /p:Platform=x64 "/p:OutDir=$buildRound\bin\" "/p:IntDir=$buildRound\obj\" /fl "/flp:LogFile=$buildRound\msbuild.log;Encoding=UTF-8;Verbosity=normal" /v:minimal
```

## 3. 静态核对

主体 `git diff --check`及`git diff --cached --check`退出0。只改两个生产cpp，另建两专属记录。两初始化点已存在；解析正文、正式k和采用材料均取来源成功载荷；未调用重复纯读、未改变语言选择／未支持M终结／重试／观察DTO。新工程错误保持向既有self最终终止边界送出，初始化错误在辅助收口，不新增吞没catch。

## 4. 构建结果与产物

实际工程一次Build命令退出0。`msbuild.log`末尾读回“已成功生成。0个警告、0个错误”，用时 `00:05:19.44`。构建过程中没有修改两cpp；root及独立审阅均未发现阻断问题。没有增加无关重跑或运行。

| 最终源码 | blob |
| --- | --- |
| 海中鱼巣/装配.普通应用.cpp | `fe948d78336e92b9a89b1b2f893a18780fba463e` |
| 海中鱼巣/业务/接收需求认知.服务.cpp | `3f9a00c2e1788206426c2dbefc8648da4f3ae5dc` |

实际产物 `D:/TEMP/海中鱼巣/LANGUAGE-PRESOURCE-WIRING/build-20261103-wiring-owner-01/bin/海中鱼巣.exe`，45,504,000字节，SHA256 `6E7DEE5F2EAFF52CB7B908E9E304A63904374588B93B6608E0EDD580A8E4BC7A`，从未启动。

收尾格式检查 `git diff --check`及`git diff --cached --check`均退出0，交付时index空。构建当时输入为声明HEAD＋本叶两cpp WIP；root随后形成代码提交 `c11e6dc75638667af5918956c79e87c4fbe72733`，两cpp与上表blob逐项一致，不预称main集成验收。

## 4.1 输出保留

root在交付时明确要求保留本轮输出，故未删除；只读复核精确目录仍为本轮独占路径，108文件、1,404,785,798字节，无MSBuild/cl/link/本程序进程。该目录仅实际工程生成物、部署复制材料及日志，没有业务运行数据。交付后由root持有输出保留／后续清理职责；没有触碰或重试删除上一叶已被工具策略拒绝的目录，也未清理任何其它任务输出。普通构建残留不改变编译证据，不占业务、数据库、端口或外设资源。

## 5. 未运行及声明边界

未运行程序、专项自检、回归、性能或临时夹具。未验证首次／重复初始化、真实消息来源建立复用、故障分流、停止、原未知输入终结以及完整网页链。只有静态实际调用接线，不能证明语言闭环、按需状态消费或前U治理完成。
