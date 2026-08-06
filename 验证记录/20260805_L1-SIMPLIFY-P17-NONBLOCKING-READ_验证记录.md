# L1-SIMPLIFY-P17-NONBLOCKING-READ v0.2 验证记录

日期：2026-08-07

计划身份：`L1-SIMPLIFY-P17-NONBLOCKING-READ`

plan blob：`8c830e9016553315f826d359e7f614af3d40d846`

完成代码基线起点：`b0df2d72ee2d95278f2819a130dec8a8c4e3b8a9`

最终范围复核基线：`5868970df2ff9f4819263a4f1b9c37ed1dce4553`

## 1. 正式执行结果

| 门禁 | 结果 |
| --- | --- |
| Debug x64串行Rebuild | PASS，退出0 |
| Debug同一生产程序`海中鱼巣.exe --self-test-exit` | PASS，退出0，stderr 0字节 |
| Debug MSVC模块扫描 | PASS，sources/modules=202/202，failed=0，拓扑202，循环0，外部需求0 |
| Release x64串行Rebuild | PASS，退出0 |
| Release同一生产程序`海中鱼巣.exe --self-test-exit` | PASS，退出0，stderr 0字节 |
| Release MSVC模块扫描 | PASS，sources/modules=202/202，failed=0，拓扑202，循环0，外部需求0 |
| `python .\tools\check_specs.py --strict` | PASS，110/110 |
| `git diff --check` | PASS；仅Git行尾策略提示，无whitespace错误 |
| Git index | 空；本轮未stage/commit/push |

构建使用Visual Studio 18 Community `MSBuild.exe`，Debug/Release、x64、`Rebuild`、`/m:1`。所有输出位于系统临时目录`C:\Users\zhchh\AppData\Local\Temp\hzy-p17-v02-20260807`，未使用仓库`.codex-build`。

## 2. P17矩阵

| 场景 | 结果 |
| --- | --- |
| 两入口合同版本错误 | PASS，入口拒绝、空载荷/代次0、仓库零调用 |
| 空初始L1 | PASS，快照成功且代次0；代次入口未找到0 |
| 正常非零代次 | PASS，两入口返回同一非零事实代次 |
| 快照稳定排序和同状态重复读取 | PASS，纯值逐字段一致 |
| 两个并发共享读者 | PASS，两个快照均成功且一致 |
| 独占许可竞争 | PASS，两个入口立即许可拒绝；耗时门禁小于100毫秒，释放后恢复成功 |
| 事实退出后的当前快照 | PASS，代次推进且已退出值不进入当前快照；历史入口仍可读取历史 |
| 隔离和状态损坏 | PASS，内部不一致、空快照/代次0，不返回部分成功 |
| 快照分配失败 | PASS，资源失败；下一次正常读取恢复成功 |
| 其它未知异常 | PASS，内部不一致；下一次正常读取恢复成功 |
| 既有P15自检回归 | PASS，双配置同一生产程序全量自检退出0 |

生产新入口只使用一次`std::try_to_lock`；`sleep`和内部重试均为0。静态搜索中的唯一`wait`位于Debug自检专用独占许可屏障，不属于生产读取入口。

## 3. 范围和所有权

目标代码diff精确为五个L1核心文件；加上本施工记录和本验证记录后，结果范围为计划允许的七路径。工程、filters、启动、计划、索引、R5/R4、P0-FIX、路线、WORLD-TREE、`.codex-build`及其它异主WIP均未纳入目标diff。

最终基线相对施工起点只新增R6三份设计文档和一条待激活索引行，P17七路径、接口和构建资源零重叠；该文档增量不被当作代码验证结果，也不改变上述双配置证据。

## 4. 声明边界

允许声明：P17 L1组件非阻塞完整快照与当前事实代次读取候选通过本轮双配置构建、自检、模块扫描、strict和静态门禁。

禁止声明：P15正式接线或正式独立读回通过；P17生产消费者、独立集成验收、指定根世界结构、完整世界快照、STEP-4、崩溃/跨进程恢复或治理闭环完成。
