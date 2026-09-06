# 概念应用全局根引用编译运行记录

日期：20260906
版本：v0.1
结论：BUILD / RUN PASS；正式验收 NOT_RUN。

用户已解除此前暂停验证的限制，要求代码最低完成编译与运行。本记录补充既有历史未验证记录；不修改其当时状态。

## 1. 精确输入与工具

起点 HEAD `439444e2b24efaa56b6cdab5c0dddb43a34fec32`，加原三根应用 / 六份相关旧 WIP 和一行二次特征编译修复。三根应用 blob 分别为 FC `5c81203eff3fa1f667d116edb6e597228063b296`、EC `3992422e3f297769609402c3ec8c858b751a8b1b`、DC `18e5e98484d8ed44c7ca0004249082d1382c246b`；二次服务修后 blob `f745c2e41dfea340a58bbd9a3d896e816caacf6d`。源码尚未整体发布，不冒充已提交的正式验收基线。

全部材料位于 `D:/TEMP/海中鱼巣/CONCEPT-GLOBAL-ROOT/01a07099-20260906-build-run`，下表均为该目录相对路径。全量最终输入清单为 `final-source-inputs.json`，产物摘要为 `artifacts.json`；171 个快照文件均与当前工作区字节相同，项目只编译其中明确列出的 21 个模块及测试消费者。

构建命令（分别以 Debug / Release 替换配置）：

```powershell
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe' 'D:/TEMP/海中鱼巣/CONCEPT-GLOBAL-ROOT/01a07099-20260906-build-run/Roots.vcxproj' /m:1 /t:Build /p:Configuration=Debug /p:Platform=x64 /v:minimal
```

真实运行命令为对应 `RootsDebug/out/verify.exe roots` 和 `RootsRelease/out/verify.exe roots`。运行器隐藏启动，独立 stdout/stderr，单次最多 120 秒并分段等待，不使用旧 EXE。

## 2. 实际结果

| 配置 / 阶段 | 开始—结束（2026-09-06 +08） | 退出码 | 日志 / 结果 |
| --- | --- | --- | --- |
| Debug 最终构建 | 09:42:36—09:43:00 | 0 | Debug-build.log |
| Debug roots | 09:43:07—09:43:49 | 0 | Debug-roots.log；PASS 116 root/business assertions |
| Release 构建 | 09:43:41—09:44:44 | 0 | Release-build.log |
| Release roots | 09:44:56—09:44:58 | 0 | Release-roots.log；PASS 116 root/business assertions |

116 是每配置累计断言数，包括真实预置及业务检查，不等于 116 个独立业务场景。覆盖三根初始空、正式绑定、共享计数、异 provider / 异依赖拒绝、失败无覆盖、末实例清空、后续重绑及 L1 零写；FC/EC/场景实际业务和动态 F 值变化见证、形成支持与原键重放。两运行 stderr 为空。

首轮 Debug 在二次服务不存在的 `存在类数据状态::数量预算不足` case 上编译失败（C2838/C2065/C2131/C2051），已删除该无效引用并同步快照；失败原日志保留，不混入最终成功日志。

额外执行：三根源码 `git diff --check` 退出 0；`py -3.14 tools/check_specs.py --strict` 退出 0，98 份目录项全部通过。

## 3. 产物 SHA-256

| 文件 | SHA-256 |
| --- | --- |
| Roots.vcxproj | `4cf41b7c8d6813540c5877b68cb8e6785a24a13d8f67c1969014ff63dd7b6324` |
| consumer.cpp | `224b0c435000b024d8072918e17563831d1cb632143d5286502fd4a626b22129` |
| root_checks.cpp | `b78a72f2e2962aeada237f184d3510752d0e2d218eadf11a4b2d16943bb20820` |
| RootsDebug/out/verify.exe | `eae8cbf0d35f0337edf9c16949fd5bd348b8ace919c901b688dd6f38fd8d731b` |
| RootsRelease/out/verify.exe | `3264945fab5cfb28ddefa517712e5848c84151741b89105281646330e37abe89` |
| Debug-build.log | `59e8d46e444c10319737867df0eef0087fccb1d0db60a17265e456a701e2325e` |
| Release-build.log | `58535b2e055681e56bb9f9fd61fde75ca51dd76d738d56bee8def84cdee76e4f` |
| Debug-roots.log / Release-roots.log | `1fe9b8e26826f444eec38585542b3a52b42e442a208dd84faaa1e90437493aa5` |

## 4. 未验证范围

这不是独立集成验收。生产主程序 / UI 未启动，根初始化及正式接线未完成证明；并发压力、分配故障、重启恢复及全部二次标量分支未覆盖。三根应用源码仍是本地候选，不因构建运行通过自动进入 Git 或完成原大计划。
