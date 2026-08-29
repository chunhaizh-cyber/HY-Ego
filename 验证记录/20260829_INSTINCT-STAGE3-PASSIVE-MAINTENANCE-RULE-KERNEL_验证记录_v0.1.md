# INSTINCT-STAGE3-PASSIVE-MAINTENANCE-RULE-KERNEL 验证记录 v0.1

日期：2026-08-29

## 1. 验证对象

- 计划：`INSTINCT-STAGE3-PASSIVE-MAINTENANCE-RULE-KERNEL v0.1`
- S0 起点：`5e3d3040aaa758cd6178b9da0a1d6ef83a325a99`
- 两配置与专项实际使用的代码候选 tree：`e6a69fb40440831d98f47c8776bec357025b4341`
- 隔离树：`D:/TEMP/海中鱼巣/INSTINCT-STAGE3-PASSIVE-MAINTENANCE-RULE-KERNEL/20260829-190037-r3/src`
- 构建环境：VS 2022 Professional，MSVC 14.44.35207，`PlatformToolset=v143` 覆盖

隔离树由精确 index 通过 `git write-tree` 与 `git archive` 导出；工作树其它未提交 WIP 未进入构建输入。专项临时修改隔离树 `入口.cpp`，该修改不在 index 和提交候选中。

## 2. 结果矩阵

| 门禁 | 配置 | 结果 | 证据摘要 |
| --- | --- | --- | --- |
| 普通工程 Rebuild | Debug x64 | PASS | 生成 `x64/Debug/海中鱼巣.exe` |
| 普通工程 Rebuild | Release x64 | PASS | 生成 `x64/Release/海中鱼巣.exe`，完成代码生成 |
| P00 请求/版本/形状 | Debug/Release | PASS | 非成功状态空载荷，入口与版本分账 |
| P01 无需求衰减 | Debug/Release | PASS | 0/1/中值/M 饱和减一，活动只保护正值最低 1 |
| P02 最长等待衰减 | Debug/Release | PASS | t=0 与 t=T-1 的整数地板公式精确 |
| P03 30 天门禁 | Debug/Release | PASS | 无活动归零、有活动正值到 1、V=0 不补 1 |
| P04 服务比例 | Debug/Release | PASS | V=1、档位边界和 M 得到 1..99 |
| P05 A 分支顺序 | Debug/Release | PASS | 终止、主动安全优先和服务为零休眠顺序成立 |
| P06 L/H 与门禁 | Debug/Release | PASS | 低位允许/禁止/材料不足及高位封底精确 |
| P07 确定性 | Debug/Release | PASS | 同请求逐字段同结果，完整载荷成立 |
| P08 仓库外 ABI consumer | Debug x64 | PASS | 仓库外源文件直接 import 算法模块并调用根函数，退出 0 |
| P08 仓库外 ABI consumer | Release x64 | PASS | `/GL /LTCG` 编译链接并运行退出 0 |

## 3. 实际命令形状

普通与专项构建使用：

```text
MSBuild.exe 海中鱼巣.vcxproj /t:Rebuild /m
  /p:Configuration=<Debug|Release> /p:Platform=x64
  /p:PlatformToolset=v143
```

专项运行参数：

```text
--test-instinct-single-second-maintenance
```

P08 使用 `cl /std:c++20 /reference` 直接消费模块 IFC 和算法 OBJ；Release 明确使用 `/GL` 与链接器 `/LTCG`。

## 4. 尚未证明

- 没有验证真实合同、活动、到期事件或安全门禁 provider 的完整集合。
- 没有验证跨多个完整秒的分段等价、游标推进或事务重试。
- 没有写入或读回 A/V、状态、动态和合同事实。
- 没有接普通应用、线程和生产调度。
- v143 覆盖验证不证明本机缺失的正式 v145 工具集构建结果。

因此结论仅为：本计划的单完整秒纯值规则内核及其公开 ABI 在本次两配置矩阵内通过。
