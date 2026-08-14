# L1 所有者一致投影零守卫原子最新读取验证记录

日期：2026-08-14

计划身份：`L1-OWNER-CURRENT-SNAPSHOT-LATEST-READ v0.1`

验证基线：`main == origin/main == 48e712686c3ce9dc3ca707efdb671622d789a356` 加本计划六路径未提交候选；异主 WIP 不属于验证输入。

## 1. 构建与专项结果

| 验证 | 命令 | 实际结果 |
| --- | --- | --- |
| 根工程 x64 Debug fresh Rebuild | `msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64` | PASS |
| 根工程 x64 Release fresh Rebuild | `msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64` | PASS |
| 专项 Debug fresh Rebuild | `msbuild .\验证工具\L1所有者一致当前零守卫参数验证.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64` | PASS |
| 专项 Debug 运行 | `验证工具\x64\Debug\L1所有者一致当前零守卫参数验证.exe` | `9 PASS / 0 FAIL`；并发成功读取 11、许可拒绝 1013、漂移 0、坏形状 0 |
| 专项 Release fresh Rebuild | `msbuild .\验证工具\L1所有者一致当前零守卫参数验证.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64` | PASS |
| 专项 Release 运行 | `验证工具\x64\Release\L1所有者一致当前零守卫参数验证.exe` | `9 PASS / 0 FAIL`；并发成功读取 706、许可拒绝 318、漂移 0、坏形状 0 |
| 规范一致性 | `python .\tools\check_specs.py --strict` | `113 / 113 PASS` |
| diff | `git diff --check` | PASS |
| UTF-8 | `.NET UTF8Encoding(false, true)` 解码生产文件与三专项 | `4 / 4 PASS`，均无 BOM |
| XML | PowerShell `[xml]` 解析专项工程与 filters | `2 / 2 PASS` |

汇总 `9` 是九个具名断言组，不替代下述计划矩阵逐项对账。

## 2. 计划矩阵逐项证据

| 计划项 | 输入与预期 | 实际证据 | 结论 |
| --- | --- | --- | --- |
| 空库零守卫 | 全新运行包；零守卫读取有效但不存在的节点；预期顶层成功、期望回显 0、读取代次 0、项目未找到 | Debug / Release 均逐字段 PASS | PASS |
| 非空零守卫 | 建立独占 owner、四节点、一个值、一个关系和一个属性槽；零守卫同时读取 owner、节点、关系、值、属性值、源 / 目标关系组 | 两配置均返回非零同一截止，七类数量、项目状态和关系组完整 | PASS |
| 非零同代 | 使用前一零读返回的非零截止作为严格守卫 | 两配置均成功，期望 / 读取代次等于守卫，查询编码、项目状态和组形状完整 | PASS |
| 非零陈旧 | 使用当前截止减一 | 两配置均返回事实代次漂移，读取代次回显当前值，七类项目载荷全部为空 | PASS |
| 非法请求 | 错合同、七组全空、重复节点选择、零节点选择 | 两配置四类均入口拒绝 | PASS |
| 并发原子性 | 单写者连续 32 次各原子发布一个节点和一条同类型关系；读者连续使用零守卫读取 owner、主节点和增长中的源关系组 | Debug / Release 每个成功结果均为非零截止、零请求回显、关系与对端创建代次不晚于截止；漂移和坏形状均为 0；最终组为初始 1 + 新增 32 项 | PASS |
| 锁竞争 | 零守卫读取使用既有 try-lock 共享许可 | Debug 观察 1013 次、Release 观察 318 次许可拒绝，均未伪装为成功或漂移 | PASS |
| 既有消费者回归 | 代表性生产根工程双配置 fresh Rebuild | Debug / Release 均 PASS | PASS |
| 公开形状 / 物理隔离 | 不改 DTO、函数、枚举、模块、主工程或 filters；专项独立工程 | 生产 diff 仅两处条件变化；专项不进入生产工程 | PASS |

## 3. 精确 NOT_RUN

| 项目 | 未运行原因 | 保留证据 |
| --- | --- | --- |
| 真实 `bad_alloc` / `length_error` | 公开合同没有安全、确定的分配耗尽入口；禁止生产故障注入 | 既有异常到资源失败的源代码分支保持不变；未用静态证据冒充动态运行 |
| 私有仓库状态损坏 | 正式公开入口不能合法形成隔离标志或不完整私有状态；禁止暴露私有仓和状态出口 | 隔离 / `状态完整` 失败仍映射内部不一致；动态 NOT_RUN |
| 共享锁长期饥饿与公平性 | 有界专项只能观察实际 try-lock 竞争，不能安全证明调度器长期公平或构造无限饥饿 | 两配置实际观察许可拒绝并在写者结束后最终成功读回；长期公平 NOT_RUN |

上述三项没有覆盖任何可经公开入口稳定构造的强制动态分支，也没有增加生产 seam。

## 4. 静态范围与生产零验证痕迹

- 生产差异精确为：删除零守卫入口拒绝；把代次比较条件收紧为“请求守卫非零且不等于当前代次”。
- 对生产 diff 扫描 `PASS|FAIL|NOT_RUN|验证|测试|故障注入|sleep|yield|计数器` 为零命中。
- `海中鱼巣/领域/*.ixx` 对 `期望事实代次 = 0` 的显式赋值为零；现有生产调用保持非零守卫行为。C1 是后继待实现消费者，不属于本结果。
- 公共 DTO、服务转发、合同版本、L2、普通应用、启动和主工程未修改。
- 专项首次编译错误只在专项中逐字段修正；正式 DTO 未增加公开比较运算符。

## 5. 未证明范围与清理门禁

本验证只证明本计划的 L1 owner 一致投影零守卫原子最新读取，以及既有非零 / 非法分支没有在本矩阵中回归。它不证明 C1 根事实形状、A1 同实例 getter、阶段 18 消费、方法登记根、阶段 19、恢复、跨进程或完整 DATA-L1—L5。

三份专项仍为临时资产。结果提交被交互核验接受且计划索引退出后，必须独立删除并重新运行根工程 Debug / Release fresh Rebuild、strict、diff、工程零引用和生产零验证痕迹检查；永久记录、计划、设计、规范和生产实现保留。
