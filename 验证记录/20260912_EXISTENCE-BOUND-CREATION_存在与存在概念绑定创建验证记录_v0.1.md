# 存在与存在概念绑定创建验证记录

身份：EXISTENCE-BOUND-CREATION；日期：2026-09-12；版本：v0.1。

## 1. 范围与隔离

候选基线为 `main` / `f9e3090bd96e91e9fd76444e41d2aaaf66dae2ec` 加本切片未提交源码。仅使用 `D:/TEMP/海中鱼巣/EXISTENCE-BOUND-CREATION/01a06f6b-20260912-code`。本记录是代码施工专项验证，不是已提交基线的正式集成验收，不升级主程序、跨进程恢复或持续治理能力。

## 2. 验证方法

新增专项以真实 L1、存在/场景 owner 和联合只读提供者为主。固定根及一组存量坏图只在唯一 owner 端口交付前布置；生产入口没有引导开关或无绑定后门。读回同时检查结构化结果、唯一联合父、首次 H、当前 G 和具体身份。失败前后 G 相同用于核对零权威发布。

只读故障参与者只在专项中替换读取回执，存在 owner 与真实 L1 写入不替换。它覆盖资源/漂移/历史不可读/包含冲突传播和已确认发布后读失败的原请求收敛，不冒充真实磁盘故障或正式集成验收。真正 L1 提交未知的持久化故障未主动制造；其状态形状和重试边界守卫由源码及失败谓词检查。

## 3. 矩阵

| 场景 | 验证方式 |
| --- | --- |
| 场景成员、直接子场景、父存在组成 | 真实首次创建、同请求重复、当前唯一联合父、关系身份与共同 H |
| 根绑定、子场景绑定 | 根角色与空路径；完整子场景四根和现实树最终读回 |
| 空绑定、非法枚举、未找到、错类型、错树、已退出、旧 G、零预算 | 结构化拒绝、事实为空、G 不变 |
| 存量组成环 | 交付前真实坏图夹具，循环上溯拒绝零写 |
| owner 保留键 | 对应 owner 拒绝，另一 owner 同数值合法；独立组合账键 |
| 原账异常 | 原生部分首次材料、两 owner H 不一致、映射缺项拒绝补写；同键换绑定幂等冲突 |
| 预算、资源与读取漂移 | 有界祖先预算真实路径；可控只读错误回执检查精确状态 |
| 后继覆盖 | 真实合法移动后重放；绑定父脱离场景位置后子直接父不变的重放；保留首次 H、事实为空且不重建；首建脱树绑定仍精确拒绝 |
| 发布后当前联合父读回失败 | 从只读场景父组注入资源/漂移/历史清理/预算/内部错误，核对确切联合父分支一次失败、权威 G 已加 1、状态精确、原请求重放零增量收敛 |
| 世界树创建 v3 | 场景/存在原子创建及旧 G 精确重放；最终位置/现实树确认 |
| 存在概念 | 新旧空父零写；v2 合法父、多同根父、错根父、精确重放、退役/退出父拒绝 |
| 系统引导与旧入口退出 | 原族登记与四概念根夹具成功；旧新增存在生产/验证符号扫描归零 |

退出概念父的既有应用定义流程先读取属性；已退出概念的种类属性缺失后，`唯一属性`返回`内部不一致`（状态24），而非`目标已退出`。本次按当前实现保留该状态并验证零写，未扩张为概念生命周期语义修订。

## 4. 正式运行结果

以下检查均为 x64。根工程和绑定存在专项按最终冻结源码重建；定位及场景既有回归在最终发布后错误分账补充之前完成，其工程、夹具和业务成功路径随后未修改。最后调整的联合父错误映射及后继覆盖由新增专项的精确失败分支验证，未重复既有完整概念续跑。

| 检查 | 实际结果 | 专用根内证据 |
| --- | --- | --- |
| 根工程 Debug Rebuild | exit 0 | `root-final-Debug-build.log` |
| 根工程 Release Rebuild | exit 0 | `root-final-Release-build.log` |
| 绑定存在专项 Debug Rebuild / Run | exit 0 / 0；`BOUND_EXISTENCE_OK 372 G=56` | `core-Debug-build.log`、`core-Debug-run.log` |
| 绑定存在专项 Release Rebuild / Run | exit 0 / 0；`BOUND_EXISTENCE_OK 372 G=56` | `core-Release-build.log`、`core-Release-run.log` |
| 迁移后的定位特征专项 Debug Rebuild / 全量 Run | exit 0 / 0；`PARENT_BOUND_CORE_OK 685 G=59` | `located-final-build.log`、`located-final-run.log` |
| 场景特征专项 SceneOrganization Debug Rebuild / scene-preflight | exit 0 / 0；`SCENE_PREFLIGHT_OK 609 G=38` | `scene-final-build.log`、`scene-final-run.log` |
| 场景特征专项 ExistenceProvider Debug Rebuild / scene-preflight | exit 0 / 0；`SCENE_PREFLIGHT_OK 609 G=38` | `existence-provider-final-build.log`、`existence-provider-final-run.log` |
| `git diff --check` | 通过 | 工作区差异检查 |
| `py -3.14 tools/check_specs.py --strict` | 124 份目录项全部通过 | 正式规范目录检查 |
| 旧 `新增存在(` 扫描 | 0 处匹配 | 生产与验证目录扫描 |

正式专项命令为 `pwsh -NoProfile -File 验证工具/运行绑定存在创建专项验证.ps1 -Configuration Both -OutputRoot D:/TEMP/海中鱼巣/EXISTENCE-BOUND-CREATION/01a06f6b-20260912-code`。脚本固定源码、模块及本地头文件的 SHA256 输入闭包，共 33 项；每次构建/运行前后核对，最终复核 0 项变化，记录于 `core-inputs.json`、`core-results.json`。根构建结果为 `root-final-results.json`，定位迁移验证为 `migration-final-results.json`；其中保留首次场景工程失败，修复后的两个模式独立结果以 `scene-final-results.json` 为准。脚本 PowerShell AST 解析通过。迭代日志及早期被后续守卫修订替代的构建，不替代以上最终源码证据。

场景专项源码前序已改为包含定位专项的同一完整夹具，显式工程表仍按旧模式裁剪模块。首轮独立构建暴露现有定位模块和概念应用依赖缺失；本轮仅在获准工程补齐实际导入闭包。两个 ProbeMode 的快速运行使用既有 `scene-preflight` 参数，共用夹具的完整概念续跑由定位专项 685 项结果覆盖。

## 5. 完成边界

本切片尚未提交/推送。诊断宏打开后的模态弹窗未自动触发；Debug 编译覆盖入口与诊断调用，默认关闭诊断的拒绝路径实测。未做磁盘损坏、真实发布未知、跨进程恢复或主程序业务全链集成验收。
