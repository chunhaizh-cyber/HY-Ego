# DATA-L1-N-PARTITION-ATOMIC-V3 验证记录 v0.1

日期：2026-08-29

## 1. 验证对象

- 计划：`DATA-L1-N-PARTITION-ATOMIC-V3 v0.1`
- 起点：`a2f0f6a3c50a8e14180be365a1480e33f1b766c5`
- 候选范围：三个 L1 生产文件、一个专项、两个工程登记精确 hunk及两份专属记录
- 仓库外验证根：`D:\TEMP\海中鱼巣\DATA-L1-N-PARTITION-ATOMIC-V3\20260829-round1`
- 根工程工具集：`PlatformToolset=v143`

## 2. 已运行门禁

### 2.1 根工程 Debug/Release

```powershell
MSBuild.exe .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v143 /m
MSBuild.exe .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v143 /m
```

结果：两配置均退出码 0，完整工程完成编译和链接；输出未出现编译或链接错误。本结果包含当前工作区异主 L4 WIP 的编译闭包，只证明当前完整树可构建，不把异主代码计入本计划结果。

### 2.2 Debug/Release 专项

临时 `driver.cpp` 只 import `海中鱼巣.端到端测试.DATA_L1有限N分区原子事务`，重新链接两配置最终 OBJ 后运行。两配置均退出码 0：

```text
V01 PASS  2/3/4/8/255 参与者共同 G1 与逐 owner 正式读回
V02 PASS  精确重复与同键异义分账
V03 PASS  非法 N、身份、owner、内部幂等和端口矩阵写前拒绝
V04 PASS  G0、本地键、前向、自跨、缺失引用与属性槽边界零写
V05 PASS  第1/中间/末参与者失败后原组合键和内部幂等键修正提交
V06 PASS  既有 v1/v2 原子事务入口回归
V07 PASS  格式3 round-trip、恢复后 v3 精确重放
V08 NOT_RUN  缺少独立合法格式1/2固定样本
```

专项使用公开运行包、签发器、端口及正式当前读取；没有访问私有仓库或以日志、数据库行数、内存地址代替权威读回。

### 2.3 Debug/Release ABI consumer

临时 `abi_consumer.cpp` 用明确成员函数指针类型静态固定：

```cpp
L1有限N分区原子事务结果_v3 (L1所有者范围写端口::*)(
    const L1有限N分区原子事务请求_v3&,
    const std::vector<const L1所有者范围写端口*>&) noexcept
```

两配置均重新编译、链接并运行退出 0。Debug 导入 IFC 时 MSVC 给出 `_DEBUG` 环境 C5050 提示，未产生编译错误；根工程正式 Debug/Release Rebuild 没有该临时 consumer 提示。

### 2.4 规范与差异门禁

- `python .\tools\check_specs.py --strict`：`PASS`，116/116 份目录项通过。
- `git diff --check`：`PASS`；只有工作区既有 LF/CRLF 转换提示。
- 目标六个代码/工程对象的 `git diff --check -- <targets>`：`PASS`。
- v3 生产符号只位于计划允许的三个 L1 文件；专项只位于新测试模块和两个工程登记 hunk。
- v1/v2 DTO、入口、账和恢复分支未删除；内部编码只产生格式 3，解码仍接受格式 1/2/3。

## 3. NOT_RUN

- 独立合法内部格式 1、格式 2 固定样本恢复后继续 v3 提交：仓库没有可验证的已发布固定 fixture，未伪造样本。
- 资源耗尽、交换边界异常、进程崩溃、断电和介质故障：没有不污染生产合同的确定触发方法。
- 阶段三安全/服务被动维护、普通应用 consumer 和完整本能路线：不在本计划范围。

## 4. 精确暂存树复验

首次精确 index 的提交树为 `67438a06b206aa04179d19ec7d9010403af70c8b`，导出到：

```text
D:\TEMP\海中鱼巣\DATA-L1-N-PARTITION-ATOMIC-V3\20260829-staged-final
```

该树只含本计划 8 文件，工程登记只有新增专项的 1 行与 3 行；异主 L4 工程 hunk 未进入 index。该树重新执行：

- Debug/Release v143 根工程 Rebuild：均退出 0；
- Debug/Release ABI consumer：均编译、链接并运行退出 0；
- Debug/Release 专项：均再次输出 V01–V07 PASS、V08 NOT_RUN。

复验生产与工程 blob：

```text
cb676f15caef37144a427e1eff15bb27f190b42f  L1所有者范围CRUD.数据.h
983a03c9fb3b8b1d10c8ec9655d233cda0ec5e46  服务.L1事实基座.ixx
f742a6657f5c18dc00fee44291ca042054fc24c2  仓库.L1事实基座.ixx
f526317c8eedea6bcc9d7b9b4b10502f40b8cbb9  端到端测试.DATA-L1有限N分区原子事务.ixx
61d7aea879e599c7008e71591044d9e18cf56592  海中鱼巣.vcxproj
32bcb25898fb60079d5fd291da93fd7731203869  海中鱼巣.vcxproj.filters
```

本节写入后只改变本验证记录 blob；最终提交前再次运行 `git diff --cached --check` 和精确文件清单核对。

## 5. 结论

DATA-L1 有限 N 分区 v3 provider 已在当前候选上通过 v143 根工程双配置 Rebuild、双配置 ABI consumer 和 V01–V07 公开入口运行矩阵；V08 与故障注入边界保持明确 `NOT_RUN`。该结论不升级为阶段三被动安全/服务维护或本能路线完成。
