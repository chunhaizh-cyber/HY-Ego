# SERVICE-LAYER-BOUNDARY-CLEANUP-A 旧代码物理清理验证记录

日期：2026-09-10
验证基线：`f961547cf8b19faa3dc376566eb8ab7a74dfccab` 加本计划八项生产差异
工具链：MSBuild 18.8.2，x64 Debug/Release，`/m:1 /t:Rebuild /verbosity:minimal`

## 1. 静态与工程结果

| 检查 | 结果 |
| --- | --- |
| 六目标 `Test-Path` | 6/6 为 false |
| Git 删除摘要 | 恰为六项 `delete mode 100644` |
| 旧文件/include/五模块名扫描 | 剩余生产源码零命中 |
| 旧稳定主键/三句柄/有效性函数身份扫描 | 剩余生产源码零命中 |
| 工程及构建脚本扫描 | 零命中 |
| 五份安全模块工程登记 | 0 |
| `海中鱼巣.vcxproj` 旧句柄登记 | 0；只删除一行 |
| `海中鱼巣.vcxproj.filters` 旧句柄登记 | 0；只删除一个三行块 |
| 两份 XML 解析 | PASS |
| 目标 `git diff --check` | PASS |

扫描范围为剩余生产 `*.ixx/*.cpp/*.h/*.inl`，以及 `*.vcxproj/*.vcxproj.filters/*.props/*.targets/*.ps1/*.cmd/*.bat`。独立同名工具字段不属于旧头身份，本轮没有据短名误删代码。

## 2. 隔离构建因果对比

日志位于：

- `D:/TEMP/海中鱼巣/SERVICE-LAYER-BOUNDARY-CLEANUP-A/20260910-legacy-a-execute/before-debug.log`
- `D:/TEMP/海中鱼巣/SERVICE-LAYER-BOUNDARY-CLEANUP-A/20260910-legacy-a-execute/after-debug.log`
- `D:/TEMP/海中鱼巣/SERVICE-LAYER-BOUNDARY-CLEANUP-A/20260910-legacy-a-execute/before-release.log`
- `D:/TEMP/海中鱼巣/SERVICE-LAYER-BOUNDARY-CLEANUP-A/20260910-legacy-a-execute/after-release.log`

| 配置 | before | after | 首个确定性错误 | 规范化 error 行 |
| --- | ---: | ---: | --- | --- |
| Debug x64 | 非零 | 非零 | `应用服务.世界树类.ixx:126:17 C2065 场景类结点未声明` | 118 / 118，差异 0 |
| Release x64 | 非零 | 非零 | 同上 | 118 / 118，差异 0 |

四次均真实执行。before/after 的首错文件、行、错误码和核心文本相同；规范化 before/after 根路径后完整错误行逐项相等；after 没有更早错误、旧文件/模块缺失错误或工程登记异常。结论为：A 包没有改变根工程已经到达的既有失败前沿。根工程 Debug/Release 仍失败，不能声明编译或链接通过，也不覆盖失败点之后的编译单元。

## 3. 规范与差异检查

在隔离 before/after 副本补齐 Git 上下文后，`python tools/check_specs.py --strict` 均为“119份目录项全部通过”。不带 Git 上下文的导出副本会因 `tools/l1_frozen_surface_manifest.json` 无法读回 HEAD/origin 的 acceptance record 报三项环境型错误；该结果没有通过恢复异主删除记录来掩盖。

共享工作区只对本计划十路径运行差异检查；发布前还需精确暂存六项 D、两个工程 M、两份记录 A，运行 cached name/status 与 cached diff/check。

## 4. 验证结论

静态删除、生产消费者归零、旧身份消费归零、工程精确差异、XML 和隔离 strict 均满足 v0.2 门禁；构建采用正式基线失败分支，before/after错误同构且零新增删除相关错误。

本验证不证明根工程通过、程序运行、数据库恢复、故障注入、集成验收、安全业务实现、B 包迁移或业务闭环。
