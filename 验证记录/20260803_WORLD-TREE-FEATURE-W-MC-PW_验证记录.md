# WORLD-TREE-FEATURE-W-MC-PW 验证记录

日期：2026-08-03

## 验证基线

- 计划：`WORLD-TREE-FEATURE-W-MC-PW v0.1`
- plan blob：`cca4a4170ddf7e2de02ee8987edf4d29a0634921`
- 验证时正式基线：`main == origin/main == b6192d2524404e0af5b6d6169c4ebbc58781150d`

## 验证结果

| 验证 | 结果 |
| --- | --- |
| Debug x64 `Rebuild` | PASS，exit 0，0 warning，0 error |
| Release x64 `Rebuild` | PASS，exit 0，0 warning，0 error |
| Debug `--self-test-exit` | PASS，113/113，exit 0 |
| Release `--self-test-exit` | PASS，113/113，exit 0 |
| `python .\tools\check_specs.py --strict` | PASS，109/109 |
| `git diff --check` 与两代码文件 scoped check | PASS |
| MSVC 全模块依赖扫描 | PASS；PowerShell 7 执行同一脚本，sources=174、modules=174、failed=0、循环=0、外部需求=0 |
| 六分支源码闭包 | PASS；六处组调用加一处定义，首业务候选前调用 |
| 派生类与旧计数扫描 | PASS；正式派生类唯一，旧 `已触及参与者数量` 为 0 |

Windows PowerShell 5.1 首次运行扫描脚本时，脚本的 `$ErrorActionPreference = 'Stop'` 把 `cl.exe` 常规诊断流包装为 `NativeCommandError` 并在第一文件终止，未形成扫描结论；随后以 PowerShell 7 原样执行同一脚本完成上述 174/174 结果。该首次结果只记为工具宿主兼容问题，不记为源码 PASS 或 FAIL。

## 矩阵覆盖

- 状态 10 追加、旧 1—9 数值保持、纯虚无默认实现、四字段只读视图。
- 六个非空业务分支的许可内完整预扫描和首候选前调用。
- 多参与者登记序；第一、中间、最后预复核失败；失败参与者自身进入撤销域，未调用后继不撤销。
- 入口拒绝、版本漂移、许可拒绝、资源失败和异常到内部不一致的映射。
- 预复核全组成功后，准备首项失败仍逆序撤销全部已预复核参与者；撤销失败继续清理并隔离。
- 类型合同精确占用和模拟高水位；两类索引创建分支的同义/异义占用均先于参与者拒绝。
- 原准备、确认、完成发布顺序以及顶层 113 项身份、顺序 170 和旧业务成功/失败结果零回归。

## 声明边界

本记录只证明核心类型化结构事务具备业务中性的第一写前复核阶段，并通过本计划的构建、自检和静态矩阵；不证明任何 4170 参与者、批次账、`WORLD-TREE-FEATURE-W-DP`、真实特征写入、生产装配、恢复或服务验收完成。
