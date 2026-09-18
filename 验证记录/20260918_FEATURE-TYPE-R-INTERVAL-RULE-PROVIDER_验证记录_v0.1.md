# FEATURE-TYPE-R-INTERVAL-RULE-PROVIDER 验证记录 v0.1

计划：`FEATURE-TYPE-R-INTERVAL-RULE-PROVIDER` v0.3。验证对象：特征类 R 规则物理扩展与 I64 只读规则入口。

## 已执行验证

| 检查 | 结果 |
| --- | --- |
| `git diff --check -- 海中鱼巣/领域/数据服务.特征类.h 海中鱼巣/领域/数据服务.特征类.cpp` | 退出 0 |
| `python .\tools\check_specs.py --strict` | 126/126 通过，退出 0 |
| 旧 ABI 搜索：`rg -n --glob '!*.md' '特征R归组用途\|特征R归组状态\|特征R归组比较请求\|特征R归组比较结果\|比较FT的R归组\|当前采用基准F' .` | 零命中（`rg` 退出 1 表示无匹配） |
| Debug x64 单翻译单元：`cl /std:c++20 /permissive- /EHsc /utf-8 /bigobj /D_DEBUG /D_CONSOLE /c 数据服务.特征类.cpp` | 退出 0 |
| Release x64 单翻译单元：`cl /std:c++20 /permissive- /EHsc /utf-8 /bigobj /O2 /DNDEBUG /D_CONSOLE /c 数据服务.特征类.cpp` | 退出 0 |

仓外专项夹具位于 `D:\TEMP\海中鱼巣\FEATURE-TYPE-R-INTERVAL-RULE-PROVIDER\20260918-04\r_rule_probe.cpp`，分别以 `/MDd /D_DEBUG` 和 `/MD /DNDEBUG /O2` 编译、链接并运行，均以 `ALL_PASS` 结束。夹具以四个独立 L1 owner 实际出生 F，并以真实 F 作为 R 项成员；两次运行均覆盖：

```text
新 I64 FT 同写集默认规则
空 R 项 -> 形成新 R 的单点材料 {42,42}
单点材料 -> 代表值 42
规则关系预算 2 < 实际 3 -> 数量预算不足
空 R 项携带非零 R 预算 -> 入口拒绝
没有域形成 -> 已归并零输出且无载荷，并拒绝未使用的规则预算
完整既有规则的补齐入口 -> 零写幂等成功
真实 F + 同点 R 材料 -> 唯一命中
真实 F + 不同候选值 -> 形成新 R
两个同材料 R 项 -> 内部不一致
真实 F + 有域 FT -> 已归并输出
尝试退出仍被出生关系引用的真实 F -> 引用冲突、零写
读回代次被另一 owner 写入推进 -> 事实代次漂移
旧 FT 的一条规则关系退出 -> 补齐入口返回内部不一致且事实代次不变
旧 FT 的全部规则关系退出 -> 补齐入口发布两条默认规则；相同请求重放零写成功
```

专项输出两次均以 `ALL_PASS` 结束。

完整工程也以 Visual Studio 18 Professional 的 `v145` 前台 `MSBuild /t:Rebuild /m:1` 分别构建 Debug x64 与 Release x64；两者均链接生成 `D:\海中鱼巣\x64\Debug\海中鱼巣.exe`、`D:\海中鱼巣\x64\Release\海中鱼巣.exe`。最初使用 VS2022 的一次构建报 `MSB8020`，原因是该安装不带 v145；它不构成最终失败结论。

## 未形成通过结论的验证

- 本记录不证明 IF/R 或 FCr 写入、概念 owner 的 `F→FCv` 核验、非 I64 规则、恢复、装配、运行或业务闭环。
