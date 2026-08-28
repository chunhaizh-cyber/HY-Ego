# DATA-L1-THREE-PARTITION-ATOMIC-V2 验证记录 v0.1

日期：2026-08-28

## 1. 验证对象

- 计划：`DATA-L1-THREE-PARTITION-ATOMIC-V2 v0.1`
- 起点：`4613d1a8d5f963b300f9682a0ca3807bdfb97564`
- 候选范围：三个 L1 核心文件及两份专属记录
- 隔离根：`D:\TEMP\海中鱼巣\DATA-L1-THREE-PARTITION-ATOMIC-V2\20260828-round1`
- 项目实际工具集：`PlatformToolset=v145`

## 2. 已运行门禁

### 2.1 L1 模块编译

使用 Visual Studio 2026 Community 18.8、MSVC 14.51，在从正式基线建立并只覆盖三个候选文件的隔离源码树中运行 Debug/Release `Build`。

结果：六个 L1 模块在两配置均成功生成 IFC/OBJ：

- `海中鱼巣.核心.合同.L1公共事实`
- `海中鱼巣.核心.合同.L1中性CRUD`
- `海中鱼巣.核心.合同.L1所有者范围CRUD`
- `海中鱼巣.核心.合同.L1事实基座`
- `海中鱼巣.核心.仓库.L1事实基座`
- `海中鱼巣.核心.服务.L1事实基座`

### 2.2 全项目 Debug/Release

```powershell
MSBuild.exe .\海中鱼巣.vcxproj /t:Build /m:1 /p:Configuration=Debug /p:Platform=x64
MSBuild.exe .\海中鱼巣.vcxproj /t:Build /m:1 /p:Configuration=Release /p:Platform=x64
```

结果：两配置均在未修改的 `海中鱼巣/领域/内部治理/服务.需求初次筹办准备.ixx:251` 触发 MSVC C1001，未完成根可执行文件链接。隔离树该文件与起点 HEAD 的 blob 同为 `49d5c31745cdd82eccaf577626706ed22a393409`；失败位置不在本计划白名单。

补充工具链复核：Visual Studio 2026 Professional 18.5、MSVC 14.50 能越过上述 C1001，但全项目最终因现有 L2/装配符号产生 22 个无法解析的外部符号而链接失败。该轮发生在最终锁内代次/完整性收紧前，只作为基线工具链诊断，不作为最终候选 PASS。

计划文字要求 v143，但当前工程正式值为 v145；本机 VS2022 按工程配置返回 `MSB8020: 找不到 v145`。因此没有伪称 v143 构建通过。

### 2.3 Debug/Release 生产外公开入口矩阵

临时 consumer 只 import `海中鱼巣.核心.服务.L1事实基座`，通过公开运行包、签发器、三个写端口、`提交三分区原子事务_v2` 及正式当前/历史读取执行；没有 friend 私有仓库、日志判定或生产故障注入。

最终 Debug 与 Release 均链接并运行成功：

```text
PASS V01-V10; V11 NOT_RUN (no published internal-format-1 fixture)
```

覆盖：

- V01 三 owner 正常提交与单一 G1
- V02 2→1、3→1/2 后向引用及正式当前读回
- V03 后继代次精确重复、首次 G1/三映射不变、历史事实可读
- V04 同组合键异义、锁内代次和不可复用原异义请求边界
- V05 G0 漂移与锁内当前代次
- V06 参与者 0/1/2/4、重复身份、重复 owner、重复内部幂等身份
- V07 前向、自跨参与者、缺失键及引用冲突锁内当前代次
- V08 值来源、属性槽、关系端点、退出闭包破坏
- V09 持久恢复、端口重签、三份首次账、恢复后重放
- V10 旧双参与者 v1 ABI、状态、映射和重放回归

MSVC 对临时 consumer 给出 C5050 环境提示（Debug 包含 `_DEBUG`/guard-overflow，Release 包含 guard-overflow），但零编译错误、零链接错误，程序退出码 0。

### 2.4 静态边界

- `git diff --check -- <三个核心文件>`：`PASS`，仅有 LF/CRLF 转换提示。
- 旧 v1 删除行仅为内部载荷编码版本 `1→2` 与解码接受 `1或2`；旧 v1 DTO、入口、账和调用点未删除或复义。
- `提交三分区原子事务_v2` / `L1三分区原子事务*` 的生产代码命中仅位于三个允许 L1 文件。
- 新 v2 没有特征/状态/动态业务参与者枚举；相邻注释中的“状态/动态双参与者 v1”只描述旧 ABI 隔离。
- 新生产代码没有专项参数、测试入口、故障注入或检查模块。
- 公开恢复见证仍为 `L1事实基座核心持久恢复见证{1,...}`。

### 2.5 暂存树复验

精确暂存三个生产文件和两份专属记录后，`git diff --cached --check` 通过，暂存文件清单恰为本计划五文件。复验所用暂存树为 `55ffe305211b6cb91ff2051f847f39c2c944cc19`，三个生产 blob 分别为：

```text
5e9f9e4b722e46da437600e4e5fc9676bc69412f  L1所有者范围CRUD.数据.h
ec7c3d464f4e44514115282b8d1e18a4631c511b  服务.L1事实基座.ixx
c8a65b4df099362c21992c14b528c9bbb4433921  仓库.L1事实基座.ixx
```

从该暂存树构造 `20260828-staged-final` 隔离源码根后：

- Debug / Release 六个 L1 模块均重新成功生成 IFC / OBJ；
- Debug / Release 公开入口矩阵均重新输出 `PASS V01-V10; V11 NOT_RUN (no published internal-format-1 fixture)`；
- 两配置全项目构建仍只在未修改的 `服务.需求初次筹办准备.ixx:251` 触发同一 C1001，结论边界不变。

本小节写入记录后只改变本验证记录 blob；最终提交前再次核对三个生产 blob 与上列复验对象完全相同。

## 3. NOT_RUN

- V11 旧内部格式 1 恢复：没有合法已发布内部格式 1 fixture，未伪造。
- 资源耗尽、交换结果未知、进程崩溃、断电和介质故障：没有合法且不污染生产的触发方法。
- L2 当前值变化、业务 consumer、装配和自我内部治理：不在本计划范围。

## 4. 结论

三分区 L1 provider 的最终候选已通过两配置模块编译和 V01–V10 公开入口运行矩阵；旧 v1 回归与持久 round-trip 实际发生。全项目 Debug/Release 仍被起点 HEAD 的编译/链接问题阻断，不能声明根项目构建 PASS，也不能据此声明任何上层业务闭环完成。
