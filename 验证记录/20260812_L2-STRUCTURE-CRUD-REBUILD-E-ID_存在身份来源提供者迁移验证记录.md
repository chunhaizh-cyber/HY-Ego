# L2 存在身份来源与 provider 迁移验证记录

日期：2026-08-12

计划身份：`L2-STRUCTURE-CRUD-REBUILD-E-ID-EXISTENCE-IDENTITY-SOURCE-PROVIDER-MIGRATION` v0.1

验证基线：`main == origin/main == 81067a4b2b4b127165db0bf107aaa7996907ed42` 加本计划十路径未提交 WIP

## 1. 主工程双配置重建

执行：

```powershell
MSBuild.exe .\海中鱼巣.vcxproj /m:1 /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /verbosity:minimal
MSBuild.exe .\海中鱼巣.vcxproj /m:1 /t:Rebuild /p:Configuration=Release /p:Platform=x64 /verbosity:minimal
```

删除零调用弱身份 helper 后重新运行，Debug 与 Release x64 均完整扫描模块、编译并链接成功，退出码均为 0。该结果只证明具名配置下生产工程可编译链接，不证明业务消费者、生产运行或独立集成验收。

## 2. 生产外 15 组参数矩阵

执行：

```powershell
powershell -ExecutionPolicy Bypass -File .\验证工具\运行L2存在身份来源提供者迁移参数验证.ps1 -配置 Debug -起始幂等身份 31001
powershell -ExecutionPolicy Bypass -File .\验证工具\运行L2存在身份来源提供者迁移参数验证.ps1 -配置 Release -起始幂等身份 41001
```

最终 Debug 与 Release 均输出 `CHECKS=15 FAILURES=0`。覆盖事实如下：

| 范围 | 已验证事实 |
| --- | --- |
| owner 与 provider | 错建立幂等身份、跨实例 raw 端口不能形成强类型交付；正确交付形成成功；普通应用两个 getter 返回同一已初始化服务 |
| 来源初始化 | 首次登记成功；释放端口并同 owner 精确重复重构后锚点和类型编码不变；固定登记同键异义冲突 |
| 创建与幂等 | 新节点与唯一非自环来源关系同写、同 owner、同创建代次；精确重复回首次两项映射；同键异义冲突 |
| 当前 / 历史读取 | 来源与完整存在共享正式身份闭环；当前与历史生命周期、截止和稳定编码逐字段核对 |
| 弱身份与畸形 | wrong-family、legacy、裸节点、属性类型、内部节点入口拒绝；重复、错锚点、错角色、混代、提前退出来源内部不一致 |
| 属性与退出 | 固定属性新增 / 换代只接受正式来源；显式来源关系、节点和属性同代退出；锚点 / 类型保留；外部引用由 L1 保护，闭合后允许退出 |
| 有界并发 | 前置探测只在取得有效代次后发起完整读取；完整投影只接受完整 `已读取`、空载荷 `事实代次漂移` 或 `许可拒绝`，未观察到混代或部分载荷 |

专项构建均出现 MSB8029，内容为 MSBuild 不建议把中间 / 输出目录放入系统临时目录。这是计划要求的工作区外隔离路径提示；实际 Rebuild、链接和程序运行成功，仓库内未生成专项构建物。

## 3. 静态、规范与工程门禁

- 十条授权路径通过严格 UTF-8 解码；专项 vcxproj XML 可解析。
- `git diff --check` 对授权路径通过；Git index 在申请发布前为空。
- `python .\tools\check_specs.py --strict`：`111 份目录项全部通过`。
- 主工程与 filters 对三份临时验证文件零登记；主工程和 filters 自基线零 diff。
- L1 目录、场景 / 特征 / 状态 / 动态引用服务及其它禁止生产路径自基线零 diff。
- 旧 `查找唯一存在编码映射`、`读取当前存在节点`、`读取历史存在节点` 和专用失败封装均为零残留；公开兼容节点读取收敛到正式完整来源读取。
- 生产代码未增加测试实体、第二仓、第二成员表、缓存权威、恢复扫描、日志判定或 L2 对 L1 的 friend / 私有状态访问。

## 4. NOT_RUN 与结论边界

以下均为 `NOT_RUN`：关系事实 owner 与正式归属类型 owner 不一致的不可公开注入损坏、私有索引损坏、分配故障注入、崩溃 / 恢复 / 重入、长时公平性、跨进程、Win32、F-ID / T1 / DYN2 / S-ID / A1、生产业务消费者和独立集成验收。

结论：E-ID 在 Debug / Release x64 根工程重建和具名 15 组生产外参数矩阵下通过；来源约束、强类型 owner 交付、唯一 provider、显式关系闭合与失败空载荷得到计划范围内证据。Git 提交、推送和三份临时验证文件的后续清理仍待顶层唯一发布者完成，不能据本记录宣称项目或业务闭环。
