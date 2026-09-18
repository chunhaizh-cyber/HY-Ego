# U64组值事实保存与完整读回验证记录

计划：`U64-GROUP-VALUE-FACT-PERSISTENCE` v0.4

## 已执行

| 验证 | 结果 |
| --- | --- |
| `git diff --check`（本计划允许文件） | 通过 |
| Debug x64 `msbuild 海中鱼巣.vcxproj /t:Build /p:Configuration=Debug /p:Platform=x64 /m:1` | 通过，生成 `x64/Debug/海中鱼巣.exe` |
| Release x64 `msbuild 海中鱼巣.vcxproj /t:Build /p:Configuration=Release /p:Platform=x64 /m:1` | 通过，生成 `x64/Release/海中鱼巣.exe` |
| 隔离专项 `u64_group_value_probe` | 编译并运行退出码 0；验证 owner 建立、两节点结构登记、首次保存、相同 U64 序列复用、空序列入口拒绝、按身份完整读回。 |

专项目录：`D:\TEMP\海中鱼巣\U64-GROUP-VALUE-FACT-PERSISTENCE`。

## 边界

构建与专项运行证明本计划的 U64 值事实叶；不证明普通应用长时运行、持久恢复、局部二值格业务格式、非 U64 表示或上层特征/概念业务闭环。
