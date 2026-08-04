# L1-SIMPLIFY-P1B 验证记录

日期：2026-08-04

## 计划与范围

- 计划版本：`L1-SIMPLIFY-P1B-WORLD-REGISTRY v0.1`。
- 计划 blob：`5511c6944d6cb81dd89922c9fe7de54b8720b435`。
- 允许范围：七个白名单文件；其它 dirty 文件均未暂存。

## 验证命令与结果

- `msbuild .\\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m:1`：通过。
- `msbuild .\\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m:1`：通过。
- `x64\\Debug\\海中鱼巣.exe --self-test-exit`：退出码 0；完整自检登记 24 项，P1B 专项十项通过。
- `x64\\Release\\海中鱼巣.exe --self-test-exit`：退出码 0；完整自检登记 24 项，P1B 专项十项通过。
- `python .\\tools\\check_specs.py --strict`：110/110 通过。
- `git diff --check`：通过。
- `tools\\clang_ast\\扫描.MSVC模块依赖.ps1 -配置 Debug -平台 x64`：sources=179、modules=179、failed=0，循环模块 0，外部需求 0。

## 静态闭包

- `L1-SIMPLIFY-P1B-S01` 与顺序 `290` 各出现一次。
- 三个新模块、一个纯值头和工程登记路径一致。
- 新世界登记服务只导入 L1 服务；未导入节点直接、旧世界、SQL、材料或运行期上下文。
- 写集固定五节点一值一槽；成功前置为空快照、同代次完整读回和十项专项自检均已覆盖。

## 未验证与剩余风险

本次未做生产启动、持久恢复、运行期装配、世界业务闭环或集成验收；这些均不在 P1B 白名单和完成条件内。
