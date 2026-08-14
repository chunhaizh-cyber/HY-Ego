# 所有者一致投影零守卫原子最新读取代码实施切片 v0.1

日期：2026-08-14

计划身份：`L1-OWNER-CURRENT-SNAPSHOT-LATEST-READ`

状态：设计就绪；待计划索引登记；不等于代码已实现

## 1. 目标

让既有 owner-aware 一致当前投影原子支持 `期望事实代次=0`：仓库取得本次共享锁后选择最新当前代次，并在同一锁域完成全部投影。消除 `DRIFT-C1-L1-ZERO-LATEST-ATOMICITY`，为 root-only C1 v0.2 提供无竞争窗口的首次当前截止。

## 2. 正式依据与完成条件

- 0050、4015 v1.3、4230、4240 v0.4；
- [详细设计](../规范/详细设计/20260814_L1-OWNER-CURRENT-SNAPSHOT-LATEST-READ_所有者一致投影零守卫原子最新读取详细设计_v0.1.md)；
- 当前正式函数与仓库实现基线 `f649250080377ccb851922b1619836661c8f91ec`。

完成必须证明零守卫在一个仓库共享锁内选择和投影，成功结果回显请求 0 与实际读取代次；非零守卫和全部非法请求不回归；临时专项最终独立清理。

## 3. 允许与禁止范围

允许：

1. `海中鱼巣/核心/仓库.L1事实基座.ixx`；
2. 临时专项 `验证工具/L1所有者一致当前零守卫参数验证.{cpp,vcxproj,vcxproj.filters}`；
3. `实施记录/L1-OWNER-CURRENT-SNAPSHOT-LATEST-READ_v0.1_施工记录.md`；
4. `验证记录/L1-OWNER-CURRENT-SNAPSHOT-LATEST-READ_v0.1_验证记录.md`。

禁止新增公开 DTO / 函数 / 枚举 / 模块，禁止修改 L2、BIZ、普通应用、计划外规范、写入事务、锁种类或现有非零守卫语义。禁止自动重试、sleep、yield、缓存、日志判成功或验证 seam。

## 4. 实施步骤

1. S0 核对计划 / 设计 / 索引版本、正式 HEAD、仓库函数、所有调用者、dirty / index 与构建资源。
2. 仅在 owner 一致投影函数中移除零守卫入口拒绝；持共享锁后以请求值裁决“零选择当前 / 非零严格比较”，保持结果请求值原样回显。
3. fresh Rebuild 根工程 x64 Debug；通过后 Release。
4. 建立专项并完成详细设计第 5 节矩阵，逐项记录 PASS / FAIL / NOT_RUN。
5. 运行 strict、diff、UTF-8、XML、静态范围、非零调用者回归和生产零验证痕迹；形成两份永久记录。
6. 精确提交 / push 结果；交互核验后单独登记索引退出。
7. 独立删除三份专项，重跑根工程双配置与清理门禁并发布清理提交。

## 5. 验证命令和边界

```powershell
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64
msbuild .\验证工具\L1所有者一致当前零守卫参数验证.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64
msbuild .\验证工具\L1所有者一致当前零守卫参数验证.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64
python .\tools\check_specs.py --strict
git diff --check
```

一次构建或一次零读成功不能替代矩阵。分配耗尽、私有状态损坏和锁饥饿没有合法稳定注入时精确 `NOT_RUN`，不得扩生产接口。

## 6. 完成声明

结果只证明 L1 owner 一致投影的零守卫原子最新读取；不证明 C1、A1、阶段 18 或整个 DATA-L1—L5 完成。C1 v0.2 必须等待本计划结果接受、索引退出、专项清理和共享工程 / 构建资源释放后重新 S0。
