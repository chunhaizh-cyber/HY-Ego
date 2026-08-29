# INSTINCT-STAGE3-MAINTENANCE-BASE-SNAPSHOT-COMPOSER 验证记录 v0.1

日期：2026-08-29

## 1. 验证对象

- 计划：`INSTINCT-STAGE3-MAINTENANCE-BASE-SNAPSHOT-COMPOSER v0.1`
- S0 起点：`5c6ec5519978314d201f37fc0b8f525b8eb432b5`
- 代码候选 tree：`1e990f5bf7e68742034f798fc10d2b17064505e5`
- 隔离树：`D:/TEMP/海中鱼巣/INSTINCT-STAGE3-MAINTENANCE-BASE-SNAPSHOT-COMPOSER/20260829-215000-r2/src`
- 仓库外 consumer：`D:/TEMP/海中鱼巣/INSTINCT-STAGE3-MAINTENANCE-BASE-SNAPSHOT-COMPOSER/20260829-215000-r2/abi-consumer/consumer.cpp`
- 构建环境：VS 2022 Professional，MSVC 14.44.35207，`PlatformToolset=v143` 覆盖，`/m:1`

隔离树由精确 index 导出；工作树其它未提交 WIP 未进入构建输入。专项临时入口和仓库外 ABI consumer 均位于 `D:/TEMP` 隔离根，不进入仓库候选。

## 2. 结果矩阵

| 门禁 | 配置 | 结果 | 证据摘要 |
| --- | --- | --- | --- |
| 普通工程 Rebuild | Debug x64 | PASS | 精确隔离树，v143 覆盖，引擎成功生成 |
| 普通工程 Rebuild | Release x64 | PASS | 精确隔离树，v143 覆盖，完成代码生成 |
| P00 请求失败空载荷 | Debug/Release | PASS | 入口失败不返回快照，截止 0 |
| P01 显式前置 | Debug/Release | PASS | 游标、定义缺失都保留具名失败，不补默认 |
| P02 版本化 L/H | Debug/Release | PASS | 版本 1 精确读取 30%/80%；发布版本 2 后快照读取改变后 L/H，不硬编码版本 1 |
| P03 无新完整秒 | Debug/Release | PASS | 完整成功快照，`可维护()==false`，无区间 |
| P04 单调完整秒区间 | Debug/Release | PASS | 真实稳态时钟产生至少 2 秒的正区间 |
| P05 运行代次漂移 | Debug/Release | PASS | 错代次 fail-closed，空载荷 |
| P06 G0/当前性漂移 | Debug/Release | PASS | 错 G0 及读后互证失败不形成快照 |
| P07 V 来源绑定 | Debug/Release | PASS | 服务值必须来自锚定服务根特征实例，错实例阻断 |
| P08 快照投影到单秒内核 | Debug/Release | PASS | 专项从快照投影 A/V/L/H/定义版本，版本 2 变更值进入内核，不用生产硬编码阈值 |
| 仓库外 ABI consumer | Debug x64 | PASS | 独立源文件 import 组合器模块，编译公开根入口调用，链接和运行 exit 0 |
| 仓库外 ABI consumer | Release x64 | PASS | `/GL` 编译、`/LTCG` 链接、运行 exit 0 |

P02 中版本 1 精确值为：

```text
L = 2,767,011,611,056,432,742
H = 7,378,697,629,483,820,645
```

测试随后发布定义版本 2，以不同 L/H 证明“浮动”是跨正式定义版本可变；同一版本内仍是确定的整数阈值。

## 3. 实际命令形状

普通与专项构建使用：

```text
MSBuild.exe 海中鱼巣.vcxproj /t:Rebuild /m:1
  /p:Configuration=<Debug|Release> /p:Platform=x64
  /p:PlatformToolset=v143
```

专项运行 P00—P08。仓库外 consumer 使用 `cl /std:c++20 /reference /ifcSearchDir`读取隔离树 IFC；Release 增加 `/GL`，链接器增加 `/LTCG /OPT:REF /OPT:ICF`。

## 4. 尚未证明

- 未读取服务合同、有效活动、到期未满足事件或安全低位回升门禁的完整集合。
- 未执行批量分段、游标推进、A/V 原子更新或状态/动态发布。
- 未接入普通应用、线程或生产被动维护循环。
- v143 覆盖验证不证明本机缺失的正式 v145 工具集构建结果。

因此结论仅为：本计划的阶段三基础快照组合器及公开 ABI 在本次两配置矩阵内通过。
