# NT-P1 独立集成 Codex 断点清单

日期：2026-07-23

任务：`#339 / DQ-231 / NT-P1 v0.1`

集成身份：`INT-WB-NODE-TYPED-01-R6`

集成顶层任务：`019f8c83-ca8a-7293-a86e-d7ecc2b4d685`

## 1. 固定输入与身份

- 目标 worktree：`C:\Users\zhchh\.codex\worktrees\integration-node-typed-01-r6\海中鱼巢`
- 分支：`codex/integration-node-typed-01-r6`
- 内容冻结基线：JY-589 / `93cc9c8ab8e7753312a5ad5a05107ae9a62fc82d`
- R6 建立起点：JY-604 / `d06dbbbe2fb889b24e2e6d68d4d842c8ddd5af5e`
- S0 接受：JY-606/E347 / `256b7161570db30511a0b37e8b203186ecfd19d2`
- 计划 blob：`69ad59eec5b3cfa7e9b3643063c735543565cbb3`
- 唯一固定输入：`16dd9410ab08a36a0b0fc2f6d249de67299eb4ec`
- 固定输入唯一父：`bcf129ec742ecf86763fa651704f547cf326fe0a`

R6 在 S0 接受后第一项 Git 写为：

```text
git merge --ff-only origin/main
```

结果：无冲突 fast-forward，JY-604 -> JY-606，R6 HEAD 固定为 `256b7161570db30511a0b37e8b203186ecfd19d2`。

随后按正式补充口径执行：

```text
git cherry-pick 16dd9410ab08a36a0b0fc2f6d249de67299eb4ec
```

结果：无冲突，生成集成输入提交 `9140665b744e409bc77b4625e37594771aafd674`。

## 2. 汇入范围

相对 JY-606，固定输入精确带入十二项文件，未修改中央治理文件，未修改任务十二文件消冲突：

1. `实施记录/20260722_NODE-TYPED-MIGRATION_NT-P1_隔离节点直接身份事务底座代码实施_Codex断点清单.md`
2. `海中鱼巣.vcxproj`
3. `海中鱼巣.vcxproj.filters`
4. `海中鱼巣/入口.cpp`
5. `海中鱼巣/核心/句柄.h`
6. `海中鱼巣/核心/仓库.节点直接身份.ixx`
7. `海中鱼巣/核心/仓库.正式关系.ixx`
8. `海中鱼巣/核心/仓库.可重建索引.ixx`
9. `海中鱼巣/核心/会话.节点直接身份结构写入.ixx`
10. `海中鱼巣/核心/执行器.节点直接身份结构写入.ixx`
11. `海中鱼巣/核心/冻结.节点直接身份结构.ixx`
12. `海中鱼巣/核心/自检.节点直接身份结构写入.ixx`

七个新模块的 `export module` 固定字符串逐文件唯一命中；七个模块同时在 `.vcxproj` 与 `.filters` 各登记一次。四个配置块均含 `/utf-8`。

## 3. 验证结果

### 3.1 构建与运行

- Debug x64 Rebuild：通过，0 警告、0 错误。
- Debug `x64/Debug/海中鱼巣.exe --self-test-exit`：通过，退出码 0，运行后进程为 0。
- Release x64 Rebuild：通过，0 警告、0 错误。
- Release `x64/Release/海中鱼巣.exe --self-test-exit`：通过，退出码 0，运行后进程为 0。

### 3.2 目标验收与静态边界

- P1-A01—A15：Debug、Release 自检均通过。
- 关系 ABI 0—23：由 P1-A05 通过确认。
- Debug / Release 完整自检、最小闭环和运行线程收口：通过。
- 新旧域零互通、默认生产装配零可达、模块 import DAG 与禁止依赖扫描：通过。
- `python .\\tools\\check_specs.py --strict`：94/94 通过。
- `git diff --check` 与 `git diff --cached --check`：通过。
- 相对 JY-606 的分支范围：精确十二项允许文件，无越界文件。
- 任务进程门禁：验证前后目标进程均为 0。

首次综合正则扫描曾因命令转义误配退出；随后按固定字符串逐文件重跑并通过。一次冻结基线全树差异核对也错误包含了预期中央路由历史尾，未作为代码失败；按 JY-606 到集成 HEAD 的十二文件范围重核通过。源码未因上述扫描纠偏修改。

## 4. 当前收口状态

当前集成 HEAD：`9140665b744e409bc77b4625e37594771aafd674`。

当前已完成固定输入汇入与完整验证，尚未执行专属记录的暂存、提交、集成分支推送或 WT-MAIN 发布。本记录是本集成身份唯一新增 Markdown，下一步仅按合同精确暂存、提交、推送 R6 分支，再核对 WT-MAIN 与 JY-606 后非强制发布 main。

本集成结果只证明固定输入在 R6 集成分支上的汇入与本合同验证通过，不宣称 #339 已进入 main、旧能力等价重建或后继任务完成。
