# DATA-L2 特征实例当前值独立退出施工检查记录

日期：2026-08-15

计划：`DATA-L2-FEATURE-CURRENT-VALUE-EXIT v0.1`

施工起点：`9e90e8b0d122dd470ae6089d705bf76372e2e229`

结论名称：施工检查；不是独立 DATA 验收或正式消费者适用性证明。

## 1. 已运行构建与程序

以下最终构建和程序运行均以退出码 0 完成：

```powershell
MSBuild.exe .\海中鱼巣.vcxproj /m:1 /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /verbosity:minimal
MSBuild.exe .\海中鱼巣.vcxproj /m:1 /t:Rebuild /p:Configuration=Release /p:Platform=x64 /verbosity:minimal

MSBuild.exe .\验证工具\DATA-L2特征当前值独立退出施工检查.vcxproj /m:1 /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /p:OutDir=<临时目录> /p:IntDir=<临时目录>
& <临时目录>\Debug\bin\DATA-L2特征当前值独立退出施工检查.exe

MSBuild.exe .\验证工具\DATA-L2特征当前值独立退出施工检查.vcxproj /m:1 /t:Rebuild /p:Configuration=Release /p:Platform=x64 /p:OutDir=<临时目录> /p:IntDir=<临时目录>
& <临时目录>\Release\bin\DATA-L2特征当前值独立退出施工检查.exe
```

根工程 Debug / Release 均编译并链接成功。专项使用系统临时隔离输出；MSBuild 报告 `MSB8029` 临时目录增量构建警告，但两轮均使用 `Rebuild` 且没有编译或链接错误。

第一次 Debug 专项编译因临时驱动调用三个不存在的 `成功()` helper 失败；改为按现行公开结果头和载荷判断后重跑通过。生产模块在该轮已经编译通过，失败不属于生产代码。

## 2. Debug / Release 动态矩阵

两配置均为 `9/9 PASS`、失败数 `0`：

1. 请求 helper 接受完整请求，拒绝零守卫；入口拒绝结果保持三载荷为空、无变更代次。
2. 新增当前值后，独立退出只推进一个事实代次，返回 `已提交` 和完整三载荷。
3. 实例在退出截止仍为当前且当前值为空，宿主 / 定义自有关系与退出前逐字段相同。
4. 退出值的编码、实例、类型化材料、来源和创建代次保持，退出代次等于写入截止，并可由具名历史入口再次读回。
5. 原完整请求重放返回 `精确重复`、首次事实截止和逐字段相同载荷，当前事实代次不推进。
6. 同幂等身份改变具名退出值返回 `幂等冲突`，零变化。
7. 无当前值返回 `属性未设置`；另一实例具名错误当前值返回 `引用冲突`；两者均零变化。
8. 陈旧守卫的新请求返回 `事实代次漂移`，零变化且不取得首次结果。
9. 另一实例的当前值和自有关系保持不变；成功谓词拒绝缺载荷及“实例仍有当前值”的矛盾成功形状。

## 3. 静态门禁与清理

以下检查通过：

- `git diff --check` 对两个生产文件通过；
- `python .\tools\check_specs.py --strict` 为 `113/113`；
- 两个生产文件 UTF-8 严格解码通过且无 BOM；
- 新增生产差异中 `PASS`、`FAIL`、测试、专项、验证、sleep 和线程等待痕迹零命中；
- 生产差异只增加公开 DTO / helper、空载荷失败辅助和同一特征服务公开退出函数；
- 临时两文件、临时 Debug / Release 输出和路径标记均在发布前删除；
- 最终暂存范围只允许两个生产文件和两份永久记录。

## 4. NOT_RUN 与结论边界

以下没有合法稳定的公开注入路径，记为 `NOT_RUN`：

- 真实内存分配耗尽与 `bad_alloc`；
- 私有 owner、类型登记、关系或历史事实损坏；
- L1 发布结果未知或发布后持续高频并发导致两次历史读回都无法取得稳定守卫；
- 跨进程恢复、故障注入、正式普通应用调用和 BIZ 消费。

没有为这些分支增加生产 seam。施工检查不构成独立验收，也不证明 DATA-EXT-03 全部能力、世界树、概念树或《数据服务最终需求清单》整体完成；具体适用性由后继合法消费者实际使用时判断。
