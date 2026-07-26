# ENTRY-ONLY 薄入口模式路由与初始化宿主分层验证记录

## 记录身份

- 计划：`#380 / DQ-272 / ENTRY-ONLY`
- 类型：`DESIGN-DRIFT-WIP` 安全断点验证
- 原 plan blob：`bdd98b3af026b1608bb3e923405c44c9eb977272`
- 计划段起点：`6041bd1bc9766e5404359dba317366eea63bc8f6`
- 本记录不证明计划完成。

## 已通过

1. `git diff --check`
   - 结果：通过；仅出现 Git 的 LF/CRLF 提示，无空白错误。
2. `python .\tools\check_specs.py --strict`
   - 结果：通过，正式规范目录 `99/99`。
3. 白名单比对
   - 结果：记录写入前的 14 个代码/工程 dirty 文件与计划第 4 节精确一致，无范围外文件。
4. 入口静态拒绝扫描
   - `入口.cpp` 的领域、UI、SQL、自检关键字扫描：零命中。
   - 入口相关五个文件的 `std::cout/std::cerr/std::wcerr/printf` 扫描：零命中。
5. `msbuild .\海中鱼巣.slnx /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m:1 /v:minimal`
   - 结果：通过，生成 `x64\Debug\海中鱼巣.exe`。
6. 关键退出码
   - `--database-self-test-exit`：`0`。
   - `--runtime-context`：`0`。
   - `--d455-real-sample-exit`：`2`，默认构建能力未启用。
   - `--unknown`：`2`。
   - 重复 `--self-test-exit`：`2`。
   - `--self-test-exit --database-self-test-exit`：`2`。
7. 诊断宏构建与 F27/F29 定位
   - `EnableEntryInitializationSelfTestDebugLog=true` 的 Debug x64 构建通过。
   - F27 九项均通过。
   - F29 第二批次通过；第一批次的 14 个编号和三项初始零计数通过，地址集合大小为 1。

## 未通过与具名原因

1. `.\x64\Debug\海中鱼巣.exe --self-test-exit`
   - 结果：`exit 1`。
   - 原因：F29/V21 要求顺序析构上下文的裸地址跨生命周期两两互异；实测合法地址复用使第一批次失败。
   - 归属：已确认设计缺口，不是代码实现错误。
2. 两次并行 fresh rebuild
   - 命令分别为直接 `.vcxproj /m` 和 `.slnx /m`。
   - 结果：MSVC 模块依赖扫描阶段失败，出现不完整 `.module.json`、`CL.exe` 退出和级联“找不到模块”。
   - 单节点 `/m:1` fresh rebuild 随后通过，证明当前源码可以完整编译；并行扫描故障不作为计划完成证据。

## 未执行或未闭合

- 无参数 UI 受控启动和关闭。
- `--headless` 的 SIGINT/SIGTERM/SIGBREAK 受控收口。
- 六个单 flag、六个重复 flag、15 个两两组合正反顺序的完整参数矩阵。
- 关系仓库性能专项完成验证。
- 五轮日志宏 fresh-build 矩阵。
- 完整覆盖对比、普通 SQL 不可用的 UI/headless 产品子进程验证和最终完成验收。

## 安全断点结论

当前代码可由单节点 fresh rebuild 完整生成，静态范围与规范检查通过，关键路由退出码可复现，设计漂移可由 `--self-test-exit` 稳定暴露。该断点可提交为 `DESIGN-DRIFT-WIP`，但不得宣称 #380 或 ENTRY-ONLY 完成。
