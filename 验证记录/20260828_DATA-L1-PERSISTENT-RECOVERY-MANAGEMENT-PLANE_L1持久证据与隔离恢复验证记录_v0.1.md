# DATA-L1-PERSISTENT-RECOVERY-MANAGEMENT-PLANE 验证记录 v0.1

日期：2026-08-28

## 1. 验证对象

- 计划：`DATA-L1-PERSISTENT-RECOVERY-MANAGEMENT-PLANE v0.1`
- 起点：`a94662f92a622fdcf9aa04cbd8f46d049ea439b8`
- 范围：本计划八个生产/工程/专项文件及专属记录。
- 构建中间件和输出：`D:\TEMP\海中鱼巣\DATA-L1-PERSISTENT-RECOVERY-MANAGEMENT-PLANE\{Debug,Release}`。

## 2. 已运行门禁

### 2.1 Debug|x64 fresh Rebuild

```powershell
MSBuild.exe .\海中鱼巣.vcxproj /t:Rebuild /m:1 `
  /p:Configuration=Debug /p:Platform=x64 `
  /p:IntDir=D:\TEMP\海中鱼巣\DATA-L1-PERSISTENT-RECOVERY-MANAGEMENT-PLANE\Debug\obj\ `
  /p:OutDir=D:\TEMP\海中鱼巣\DATA-L1-PERSISTENT-RECOVERY-MANAGEMENT-PLANE\Debug\bin\
```

结果：`PASS`。根工程生成 `Debug\bin\海中鱼巣.exe`。

### 2.2 Release|x64 fresh Rebuild

```powershell
MSBuild.exe .\海中鱼巣.vcxproj /t:Rebuild /m:1 `
  /p:Configuration=Release /p:Platform=x64 `
  /p:IntDir=D:\TEMP\海中鱼巣\DATA-L1-PERSISTENT-RECOVERY-MANAGEMENT-PLANE\Release\obj\ `
  /p:OutDir=D:\TEMP\海中鱼巣\DATA-L1-PERSISTENT-RECOVERY-MANAGEMENT-PLANE\Release\bin\
```

结果：`PASS`。根工程生成 `Release\bin\海中鱼巣.exe`。

### 2.3 静态合同复核

独立只读复核覆盖：公开 12 态和成功谓词、完整权威字段、七类索引重建、五个交换点、双槽/清单/锁/SHA、毒化 fail-closed、服务空载荷失败、普通装配顺序、专项隔离根和工程登记。

首次结果发现一项 `DRIFT`：跨 owner 原子事务账漏剔除进程维护身份。修复后对该唯一差异定向复核结果：`FIXED`。其余项目首次复核均为 `PASS`。

### 2.4 Whitespace/diff-check

目标六个 tracked 文件执行 `git diff --check`，两个新增代码文件执行行尾空白与末尾换行检查。

结果：`PASS`。仅存在 Git 的 LF/CRLF 转换提示，不是 whitespace error。

## 3. 未运行矩阵

按用户“在实际使用中再进行测试”的决定，以下全部为 `NOT_RUN`：

- `--test-l1-persistent-recovery` 专项程序执行。
- 真实第二进程恢复与新端口重新签发。
- 同根双进程互斥。
- owner、节点、关系、值、属性槽、历史、墓碑全量运行读回。
- 五类幂等账跨进程重放。
- 清单、槽、长度、摘要和标签损坏注入。
- manifest 已提交后崩溃、断电和原子替换结果未知。
- 五个交换点 I/O 故障注入与内存零变化。
- 长时运行、存储耗尽、权限失败和介质故障。

编译和静态检查不能替代这些运行证据。

## 4. 结论

代码编译闭包和静态持久门禁达到本计划发布前要求；真实跨进程与故障行为仍为未证明边界。本结论只解除本能根计划的代码前置，不证明 `INSTINCT-BOOTSTRAP-ROOT-ANCHOR` 已实施。

## 发布树隔离复核

- 复核对象：从 staged tree 独立导出的源码树，不读取或携带工作区 L4 WIP。
- staged tree 初始树对象：78dd8ff7c4e9041d3d6a4682843bc70b82a35cd7。
- Debug|x64 fresh Rebuild：/m 与 /m:1 均在 海中鱼巣/领域/内部治理/服务.需求初次筹办准备.ixx:251 触发 MSVC C1001；该文件 staged blob 与 HEAD blob 相同（$indexBlob），不在本计划文件白名单内。
- Release|x64 staged-tree Rebuild：NOT_RUN；同一确定编译器阻断已在 Debug 串行复现，不重复消耗构建资源。
- 证据边界：不得把工作区 Debug/Release PASS 扩大为干净发布树 PASS；本计划变更的独立静态审查、目标文件 diff-check 与完整工作区双配置编译证据保持有效，实际程序、专项、跨进程、崩溃、断电与故障注入仍为 NOT_RUN。