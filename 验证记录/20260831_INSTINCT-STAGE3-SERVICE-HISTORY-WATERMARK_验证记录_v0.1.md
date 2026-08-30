# INSTINCT-STAGE3-SERVICE-HISTORY-WATERMARK 验证记录 v0.1

日期：2026-08-31

计划：`INSTINCT-STAGE3-SERVICE-HISTORY-WATERMARK v0.1`

验证基线：工作区起点 `ced9acef11f26fe8f25a836d579a74a59b77d2c6` 加本计划九文件候选；结果提交 `49b7ca2abca7c379adf4c6b0224f33495c3eb809`。

隔离根：

```text
D:\TEMP\海中鱼巣\INSTINCT-STAGE3-SERVICE-HISTORY-WATERMARK
```

## 1. 结果表

| 验证项 | 结果 | 证据边界 |
| --- | --- | --- |
| Debug x64 生产模块与两专项模块 `ClCompile` | PASS | 隔离编译覆盖候选服务合同、游标和两专项模块 |
| 隔离 Debug 全工程 `ClCompile` | PASS | 计划外 ICE 文件仅在隔离副本加临时桩，仓库零改动；只证明编译闭包 |
| Debug 服务历史专项运行 | PASS | P00—Q06 及新增 H-1 全通过，返回 0 |
| Debug v3 最小真实运行夹具 | PASS | 真实 L1 / 存在 / 特征 owner 和游标 owner；v3 首建、重复、冲突、读回通过 |
| Release 服务合同、游标生产模块编译 | PASS | 以最近正式阶段 Release IFC 为依赖，从当前候选源码重新生成 v3 IFC / OBJ |
| Release 服务历史完整专项运行 | PASS | P00—Q06、H-1、H00—H06、W00—W05 全通过，返回 0 |
| Release 外部 ABI consumer | PASS | 外部源导入候选模块，具名使用覆盖起点 / v3 DTO，并调用三个公开入口；对象编译返回 0 |
| Release v3 最小真实运行夹具 | PASS | 首建、精确重复、同键异义和独立当前读回，返回 0 |
| Release v2 到 v3 迁移最小夹具 | PASS | 真实 owner 先写 v2，再以两个登记见证迁移 v3，三水位读回 / 重复 / 冲突通过 |
| Release v1 到 v3 迁移最小夹具 | PASS | 真实 owner 先写 v1，主动安全与服务水位分别采用正式登记起点，迁移后读回通过 |
| 当前生产阈值保持扫描 | PASS | 本计划没有修改值域、L/H 或定义版本；无运行期自动重算增量 |
| 本计划文件集 `git diff --check` | PASS | 九文件候选无空白错误；行尾转换提示不改变结果 |
| 全工作区 `git diff --check` | PASS | 异主 WIP 同样无空白错误；未据此取得其所有权 |
| `python .\tools\check_specs.py --strict` | PASS | 116 / 116 |
| 完整游标专项 Debug / Release 运行 | BLOCKED（异主） | 当前普通应用及状态模块存在未发布 SELF-GOVERNANCE 接口 / 链接漂移；Debug 强制链接映像触发未解析路径，Release 普通应用不能在本计划范围内重编译 |
| v3 三水位跨进程持久恢复 C10 | NOT_RUN | 测试代码已实现并在 Debug 模块编译；完整运行依赖上述普通应用闭包，未用最小夹具冒充跨进程恢复 |

## 2. 新增专项覆盖

- H-1：覆盖起点成功、旧 G、错 self、坏登记锚点、读中漂移、资源失败和非成功零载荷。
- C07：v1 到 v3 迁移，并验证 v3 对 v1 / v2 兼容读取投影。
- C08：v3 精确重复、同键异义和异键同义恢复分账。
- C09：新纪元继承两个 owner 水位，建立 / 恢复入口拒绝擅自推进服务历史水位。
- C10：三水位持久恢复逐字段一致；代码和 Debug 编译已覆盖，运行因普通应用异主漂移为 `NOT_RUN`。
- C11：v2 到 v3 保留主动安全水位，只从服务 owner 登记起点补服务历史水位。

## 3. 隔离运行与链接说明

Release 直接验证使用 MSVC 19.50、`/std:c++20 /MD /O2`。正式依赖 IFC 带有 MSBuild 内部保护宏，直接命令行 consumer / 夹具产生 C5050 环境提示，但编译和目标执行均返回 0。

共享基线的状态 / 动态模块仍存在四个计划外未解析符号。隔离目标映像以 `/FORCE:UNRESOLVED` 保留警告，目标路径实际执行并返回 0；因此证据只证明本计划可达路径，不替代完整根工程链接。完整游标专项依赖当前 `普通应用上下文`，其源码又导入大量异主未发布 SELF-GOVERNANCE 模块，本计划不得修改、桩化或发布这些依赖。

最小游标夹具中的两个登记结果是结构完整、字段逐项受约束的显式合同 fixture，用于隔离验证游标 owner；它不伪装两个登记服务的生产发布者。服务历史覆盖起点本身另由完整服务专项真实建立 owner 并运行验证。

## 4. 发布门禁与不声明

- 本计划文件集和全工作区 `git diff --check`、`python .\tools\check_specs.py --strict` 已通过；精确暂存后仍须执行 cached diff-check。
- 未证明共享 dirty 工作区完整 Debug / Release 链接。
- 未运行完整游标专项的跨进程三水位恢复，不把最小进程内读回升级为持久恢复。
- 未验证后继事实段 composer、完整维护事务或阶段三闭环。
- 未声明当前阈值会自行浮动；版本 1 实例保持不变，只有未来新定义版本可以调整。
