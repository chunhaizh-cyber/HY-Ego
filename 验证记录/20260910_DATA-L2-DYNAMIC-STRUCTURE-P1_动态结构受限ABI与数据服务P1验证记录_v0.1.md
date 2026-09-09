# DATA-L2-DYNAMIC-STRUCTURE-P1 动态结构受限ABI与数据服务P1验证记录 v0.1

## 1. 验证基线与范围

- 代码起点：`3a9d4f2813f0620029a8644355e72b690bdda41f`。
- 验证对象：动态合同、动态类及工程登记。
- 隔离目录：`D:/TEMP/海中鱼巣/DATA-L2-DYNAMIC-STRUCTURE-P1/20260910-dynamic-p1`。
- MSBuild：`C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe`。

## 2. 已执行验证

| 验证 | 结果 | 证据边界 |
| --- | --- | --- |
| 隔离工程 Debug x64 `Rebuild` | PASS，退出码0 | 新合同、动态类及所需L1/特征/状态/绑定依赖形成IFC/OBJ并链接探针 |
| Debug公开探针 | PASS，退出码0 | 12个成员函数指针签名、合同版本、枚举边界、不可默认构造/复制/移动、创建/退出精确重复谓词、零候选清理谓词 |
| 隔离工程 Release x64 `Rebuild` | PASS，退出码0 | 同上，Release生成exe |
| Release公开探针 | PASS，退出码0 | 同上 |
| 三轮独立源码复核 | 最终PASS | 写前预算、K/B/S回显、G/H尾守卫、来源映射、双键清理与完整集合逐项复核 |
| `git diff --check`（四项生产文件） | PASS | 仅有Git行尾转换提示，无空白错误 |

隔离构建命令：

```powershell
& 'C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe' `
  'D:\TEMP\海中鱼巣\DATA-L2-DYNAMIC-STRUCTURE-P1\20260910-dynamic-p1\dynamic_p1.vcxproj' `
  /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m:1 /v:minimal
```

Release仅把`Configuration`改为`Release`；两份`dynamic_p1.exe`运行退出码均为0。

## 3. 根工程分账

- 根工程Debug此前已尝试：新动态合同和动态类能够形成，随后因白名单外旧消费者仍引用已退出的场景/动态ABI而失败。
- 根工程Release于本轮使用仓外`IntDir/OutDir`再次尝试，退出码1。首批确定错误位于 `应用服务.世界树类.ixx`，例如旧 `场景类结点`、`存在类数据服务`、`场景类数据状态` 等符号已不存在；随后 `数据服务.概念树类.ixx` 仍引用旧 `动态类数据状态` 等接口。
- 这些消费者、场景类和概念树类均在计划禁止范围内，因此未修复。根工程失败不归因为新动态合同/动态类自身编译失败，也不能以隔离成功冒充根工程成功。

## 4. 静态合同检查

- 合同模块只重导出 `合同.场景状态使用绑定` 与 `算法.有序I64特征比较`，未导入场景类、因果类、概念类或业务模块。
- 动态类提供4213规定的12个业务入口：2创建、2内容读取、3反查、1展开、2身份、1退出、1同步清理。
- 对旧公开DTO `动态类数据状态/状态位置/状态成员/变化特征成员/结点/新增请求/删除请求/查询请求/状态查询请求/特征查询请求/结点结果/组查询结果/历史缺失位置/历史读取请求/历史读取结果` 的新合同与动态类生产扫描为零。
- 未恢复状态迁移关系20、过程快照、自带场景/时间/当前值/动作字段或旧兼容转发。

## 5. 未验证范围

未建立真实生产B，未运行真实数据库重启/恢复、普通应用接线、范围外消费者迁移、正式集成验收或业务闭环。仓外探针是编译与合同谓词探针，不是完整运行场景矩阵；各业务错误分支当前证据来自实现复核而非生产运行注入。因而本记录只支持“4213受限ABI与动态数据服务P1候选通过隔离编译/链接/探针及独立源码复核”，不支持更高层结论。
