# WORLD-TREE-P1D-MIG-SYSROLE-TEST 验证记录

日期：2026-08-04

## 验证基线

- 计划：`WORLD-TREE-P1D-MIG-SYSROLE-TEST v0.1`
- plan blob：`50477fe7cbaf6bbfdeb72d8f6adf2d06340dbb1e`
- 代码基线：`1b2b63d3b5e65c932c35977e74e3d5efea7fb05c`
- 目标文件修改：仅删除旧世界初始化 import 与默认构造断言。

## 待执行验证

- scoped `git diff --check`
- `python .\tools\check_specs.py --strict`
- Debug/Release Rebuild 与 `--self-test-exit`
- MSVC 模块依赖扫描
- 旧词零命中与 `运行系统角色初始化自检`、`INIT-MOD-S1`、`std::array<bool, 22>` 精确命中

## 实际结果与阻断

- scoped diff check、strict、旧词零命中和身份/22项精确命中：PASS。
- Debug x64 Rebuild：PASS，0 warning / 0 error。
- Debug `--self-test-exit`：PASS；完整自检进程退出码为 0。
- 首次 Release x64 Rebuild 曾出现 `核心.执行器.结构写入` 模块不可用及 364 个后续错误；目标文件不在错误路径中。原命令复验后通过，故该次失败按可恢复构建波动保留，不归因于本计划两处删除。
- Release x64 Rebuild 复验：PASS，0 warning / 0 error。
- Release `--self-test-exit`：PASS；完整自检进程退出码为 0。
- MSVC 模块依赖扫描：PASS，`sources=174`、`modules=174`、`failed=0`、循环 0、外部需求 0。PowerShell 7 的默认 native-stderr 提升会把 `cl.exe` 的普通文件名输出当作终止错误；以 `$PSNativeCommandUseErrorActionPreference = $false` 运行同一扫描脚本后取得上述真实扫描结果，不改变源码、项目或扫描参数。

## 完成边界

在上述验证完成前不声明计划完成。本叶只验证一个自检消费者退出旧世界类型纯编译依赖；不验证生产世界服务迁移、旧头删除、统一门面接线、P1D切换或端到端服务。
