# SUPPORT-THREAD-CANDIDATE-LIFECYCLE v0.1 验证记录

日期：2026-08-15

验证基线：正式起点 `b523b0f245e359a15641fabc6faf3825086a7b43` 加本计划六文件未暂存候选。

## 1. 根工程构建

使用 Visual Studio 18 MSBuild 执行：

```powershell
msbuild .\海中鱼巣.slnx /t:Rebuild /p:Configuration=Debug /p:Platform=x64
msbuild .\海中鱼巣.slnx /t:Rebuild /p:Configuration=Release /p:Platform=x64
```

结果：Debug、Release fresh Rebuild 均 `PASS`，候选模块形成 IFC / OBJ 并进入最终链接；命令退出码均为 0。

## 2. 生产外公开面专项

临时专项：`D:/SUPPORT_THREAD_CANDIDATE_LIFECYCLE_验证临时`。专项独立编译正式 8110 provider、正式候选模块和生产外调用者，不修改生产公开面，不登记生产工程。

Debug 与 Release 各自 fresh Rebuild 后单次运行结果均为：

```text
EARLY_STOP_HIT=1
PASS=161
```

随后 Debug / Release 各连续运行 10 次：

```text
RUNS=20
PASS_RUNS=20
EARLY_STOP_HIT_RUNS=20
```

专项实际覆盖：

| 合同 | 结果 |
| --- | --- |
| 坏版本、零运行代次、零逻辑身份、未知支持范围和错配类别—模块 | PASS；请求拒绝、零投影 |
| 事件日志 / 事件日志、缓存统计 / 缓存统计、持久证据 / 持久证据三组合法配对 | PASS；均真实创建、进入并安全连接 |
| 真实 8110 provider、真实线程、真实入口、非零 `GetCurrentThreadId()`、进入见证 | PASS |
| 正常停止链及真实 provider 最终 `已退出`、事件序号 7、未接受次数 0 | PASS |
| 生命周期端口接受 / 暂不可用 / 拒绝时物理创建、进入、连接结果守恒 | PASS；非接受次数与实际发布调用数一致 |
| 同运行代次身份并发重复、成功回收后历史重用 | PASS；仅一条物理线程，后继均 `身份已使用` |
| 可注入入口故障回收后的历史身份重用 | PASS；仍 `身份已使用` |
| 入口抛出、未知枚举、零故障原因、停止前提前返回 | PASS；全部收束为入口内部故障，零异常越过线程边界 |
| 入口先故障并完成终态后再显式回收 | PASS；零新增发布、零新增序号、零新增未接受计数 |
| stop token 唤醒、预算内完成、预算外完成后仍 join | PASS |
| 有效 1ms 预算、延迟入口故障和实际超预算并存 | PASS；入口故障优先，仍安全连接 |
| 无效请求、延迟入口故障和实际历时超过 1ms 并存 | PASS；请求拒绝优先，仍安全连接 |
| 仅移动构造、移动赋值 / 复制编译期删除、移动源等待 / 回收拒绝 | PASS |
| 并发读取值式见证 | PASS；8 个读取线程未见形成 / 进入事实回退 |
| 析构安全网 | PASS；零 detach，最终形成 `已退出` |
| 创建返回后立即回收 | PASS；20 轮压力均合法，专项总计 20/20 实际命中“先停止并整体跳过启动对” |
| 最终完成见证同步停止锁存 | PASS；完成且安全回收后 `已请求停止=true` |

## 3. 静态与仓库门禁

- 精确 `git diff --check`：PASS。
- `python .\tools\check_specs.py --strict`：`113/113 PASS`。
- UTF-8 严格解码：六个计划白名单文件，`6/6 PASS`，均无 BOM。
- 工程 XML 解析：PASS。
- 工程 / filters：模块和数据头各唯一登记一次；模块为 `ClCompile`、数据头为 `ClInclude`。
- `export module 海中鱼巣.线程.候选.支持线程`：唯一一次；正式 8110 provider 只通过 `export import` 复用。
- 四配置 `/utf-8`：仍为 4 项。
- 生产源码检索：无 `detach`、测试宏、测试 / 验证 / 自检正文和生产日志写入。
- Git：复核时 `HEAD == origin/main == b523b0f245e359a15641fabc6faf3825086a7b43`、ahead / behind `0/0`、index 空；仅六个计划白名单文件属于本切片。

## 4. NOT_RUN 与完成声明边界

以下项目未通过生产公开面确定性触发，保持 `NOT_RUN`：

- `std::jthread` 物理资源创建失败及其同身份重试；
- 进程级身份守卫 / 控制块真实分配失败；
- 操作系统极端调度导致的合法进入等待超时；
- 防御性“已完成但未进入”内部不一致。

源码静态复核确认身份墓碑在控制块和线程创建之前保留、全模块无 erase 路径；创建前失败和线程形成后收口由不同代码区段承担。未增加生产故障注入或调度 seam 伪造上述动态覆盖。

本记录最多证明共享支持线程候选生命周期 provider 的候选实现通过本矩阵。EVENT-LOG、缓存统计、持久证据、支持线程组、正常停止刷新、普通应用装配、T01、控制面板和真实业务运行均为 `NOT_RUN`。暂存、提交、推送、结果发布、计划退出和临时专项清理也尚未执行。
