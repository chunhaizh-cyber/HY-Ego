# LANGUAGE-PREUNDERSTANDING-SOURCE 编译验证

日期：2026-11-03。当前状态：实际工程 Debug/x64 编译、链接通过；未运行测试。

## 输入与资源

- 工作树：`D:/worktree/language-preunderstanding-source-20261103/海中鱼巣`。
- 分支：`codex/language-preunderstanding-source-20261103`。
- 起点 HEAD：`78c41a97681c1048bb5d26da7eb98853095e37db`，参与构建为此 HEAD 加本叶七生产／工程文件 WIP，不包含异主改动。
- 计划：v0.1／`f3c0659a83903727a395078168902decd11eab93`，索引为可执行。
- 工具：Visual Studio 18 Professional MSBuild 18.11.1，平台工具集 v145，C++20；配置 Debug/x64。
- 唯一输出：`D:/TEMP/海中鱼巣/LANGUAGE-PREUNDERSTANDING-SOURCE/build-20261103-source-owner-01/`，仅本执行者构建使用。

## 真实命令

在上述工作树执行；目录创建仅容纳实际工程产物，无源码副本或临时工程：

```powershell
$buildRound='D:\TEMP\海中鱼巣\LANGUAGE-PREUNDERSTANDING-SOURCE\build-20261103-source-owner-01'
New-Item -ItemType Directory -Path $buildRound -Force | Out-Null
& 'C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe' '海中鱼巣.vcxproj' /m /t:Build /p:Configuration=Debug /p:Platform=x64 "/p:OutDir=$buildRound\bin\" "/p:IntDir=$buildRound\obj\" /fl "/flp:LogFile=$buildRound\msbuild.log;Encoding=UTF-8;Verbosity=normal" /v:minimal
```

## 静态检查

实现主体及收尾阶段 `git diff --check`、`git diff --cached --check` 均退出 0。七生产路径白名单相符，新头为第七个未跟踪生产文件；两专属记录另列，不改共享索引。root和独立审阅均未发现ABI或所有权漂移；root指出节点数上限检查会漏掉消息根，已改为恰五节点形状，不解析私有消息字段。

工程与 filters 以 XML 读取成功；`语言分界.数据.h` 各恰一 `ClInclude`，filters 为“领域”，无新 cpp 或配置。四配置 `/utf-8`、v145、C++20 保持。消息私有字段只由外设owner解释；语言类没有业务外设头依赖，数据头不依赖语言类／外设业务类。

## 构建读回

| 轮次 | 结果 | 耗时 | 输入边界 |
| --- | --- | --- | --- |
| 首轮实际工程完整Build | 退出0，0警告0错误 | 00:05:32.16 | 起点HEAD＋七文件初版WIP，构建中未改源码 |
| 最终实际工程增量Build | 退出0，0警告0错误 | 00:00:09.10 | 首轮结束后仅将端口投影节点数由上限五改为恰五；新语言cpp重编并重链接 |

最终增量命令与上方同工程／配置／OutDir／IntDir，单独日志名，实际为：

```powershell
$buildRound='D:\TEMP\海中鱼巣\LANGUAGE-PREUNDERSTANDING-SOURCE\build-20261103-source-owner-01'
& 'C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe' '海中鱼巣.vcxproj' /m /t:Build /p:Configuration=Debug /p:Platform=x64 "/p:OutDir=$buildRound\bin\" "/p:IntDir=$buildRound\obj\" /fl "/flp:LogFile=$buildRound\msbuild-final.log;Encoding=UTF-8;Verbosity=normal" /v:minimal
```

两份实际日志末尾均读回“已成功生成。0个警告，0个错误”。最终产物为上述轮次`bin/海中鱼巣.exe`，45,498,368字节，SHA256 `C2A75A64C43153B679D9D428C2C7B5BC180CD1833EF3C8012F76D74B6616A0D5`，没有启动。

最终构建输入源码blob：

| 文件 | blob |
| --- | --- |
| 海中鱼巣/领域/新_语言类.h | `28d69c7f2706fb8758ee24989f36f122ac0b236e` |
| 海中鱼巣/领域/新_语言类.cpp | `73f85813dd6ec996e7b8f10bde6d52cca84f7fff` |
| 海中鱼巣/领域/语言分界.数据.h | `960053e68c600f7dc568670063b16965bd0d6ae1` |
| 海中鱼巣/业务/新_外设世界树类.h | `5317eb36b01a5a390457293edeb9eef333366c37` |
| 海中鱼巣/业务/新_外设世界树类.cpp | `77d8845eee9fc80bc87f2bd61aeef2f2e70102c1` |
| 海中鱼巣.vcxproj | `08fdc2e0786cf685c625b3fb8d887bbb9f918e8c` |
| 海中鱼巣.vcxproj.filters | `6890f6429bc37ee5b23635aa9480c0e9353d4e74` |

结果代码提交为 `86b7a06d61b2090ed653e9184bcce3e035b276a0`，root只提交上述七文件，并读回blob与本次最终构建输入一致；以上是精确WIP构建后固定到提交的证据，不冒称重跑了已提交基线，更不是main集成验收。root全文复核及独立只读复核未发现新的合同问题；审查后将端口投影收紧为恰五端点，该修正已参与最终增量构建。

## 清理与残留

在产物／日志证据读取后，先解析绝对路径，确认精确目标仍在本任务根内，核对无MSBuild/cl/link/本程序进程，再请求删除自有精确轮次；执行工具以策略拒绝了该删除请求。未删除任何内容，不换其它手段绕过。只读复核残留精确为 `D:/TEMP/海中鱼巣/LANGUAGE-PREUNDERSTANDING-SOURCE/build-20261103-source-owner-01`，109文件、1,440,682,407字节，为实际工程生成物、部署复制材料和两日志。未占业务、数据库、端口或外设资源，交root后续清理；其它任务目录未动。工程验证事实不因普通生成物残留而失效。

## 未验证范围

未启动程序、自检、测试、性能或外部夹具。本轮无生产调用者，不证明实际初始化、来源保存／复用、并发唯一、资源失败分支、真实网页接线、分界有效性、提问、同D接续或总业务闭环。跨进程恢复不在本叶范围。
