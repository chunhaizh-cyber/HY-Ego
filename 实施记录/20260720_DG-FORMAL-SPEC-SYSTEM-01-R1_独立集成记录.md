# DG-FORMAL-SPEC-SYSTEM-01-R1 独立集成记录

日期：2026-07-20

集成编号：INT-DG-FORMAL-SPEC-SYSTEM-01-R1

状态：集成分支验证通过待主线发布

## 1. 冻结身份

```text
中央发布提交=54fc3dce5f20b98680a905520d9797be2129cd33
集成起点=3e76799059ee1fd8c50c5e11435a4f677876bdf0
集成起点父=1157d7d278e106b566d6b771d10c4f9c8a217120
集成worktree=C:\Users\zhchh\.codex\worktrees\integration-dg-formal-spec-system-01-r1\海中鱼巢
集成分支=codex/integration-dg-formal-spec-system-01-r1
任务远端=origin/codex/hr000-formal-spec-system-r1
冻结基线=2608aae814b313f344e91931bc7470547c16bc63
固定任务链=a9c21c9bee5875187ff734df0b9aeaf542a974db -> cc884b9597db9c251da64de24d34175919e7c30d
主线发布占用=未取得；当前已释放
```

S0 已核对：集成路径、分支、起点、父链、clean、无 upstream、中央 `main == origin/main`、任务远端和集成远端同名分支不存在均符合登记。错误中央哈希 `54fc3dcda1d8f85e1516196155c138a9e0ea6e13` 未使用。

WT-MAIN 存在派发前已知的重叠 dirty/WIP，包括 `AGENTS.md`、旧编号规范、计划索引、控制面板源码、历史报告、日志和 20260720 规范文件。集成窗口未读取其工作副本内容作为输入，未 stash、clean、checkout、reset、覆盖或修改 WT-MAIN。

## 2. 固定顺序汇入

先对集成分支执行中央发布提交的 fast-forward：

```text
54fc3dce5f20b98680a905520d9797be2129cd33
```

随后按固定任务链无冲突 cherry-pick：

```text
源提交=a9c21c9bee5875187ff734df0b9aeaf542a974db
集成提交=fbcf39d1f8afcb44cd48b4f95a6c85ee77519f0a

源提交=cc884b9597db9c251da64de24d34175919e7c30d
集成提交=d6ef2d31af3292364de4346e9872c4cccc1c5ea6
```

最终集成分支 HEAD：

```text
d6ef2d31af3292364de4346e9872c4cccc1c5ea6
```

## 3. 最终范围

相对中央发布提交 `54fc3dce5f20b98680a905520d9797be2129cd33`，任务汇入范围为既有 103 个任务路径；新增本集成记录后，本批次最终只允许为 103 个任务路径加本文件。

范围核对结果：

```text
任务路径数=103
任务范围外路径数=0
计划/项目记忆/代码/工程越界=0
```

任务 `AGENTS.md`、`tools/check_specs.py`、`规范/` 最终树与源提交 `cc884b9597db9c251da64de24d34175919e7c30d` 一致；中央路由文件保持中央发布提交内容。

## 4. 验证结果

```text
python .\tools\check_specs.py --strict=PASS（85 份目录项全部通过）
git diff --check=PASS
git source-tree consistency=PASS
git forbidden-tree consistency=PASS
任务范围越界检查=PASS（0）
```

本批仅集成正式规范文档和规范检查门禁，不修改 C++、工程、自检、计划、计划索引、任务队列、登记表或窗口交互记录；未运行构建和产品运行。

## 5. 主线发布门禁与交接

最终主线发布复核要求 WT-MAIN 仍为 `main == origin/main == 54fc3dce5f20b98680a905520d9797be2129cd33`，且存在登记的重叠 dirty/WIP。根据门禁，集成窗口不得 stash、clean、checkout、reset、覆盖或直接 fast-forward main；本记录不宣称 main 已集成，也不宣称正式规范迁移最终完成。

集成分支已完成固定汇入和验证，待设计窗口在 WT-MAIN 重叠 WIP 由所有者正式释放后，另行取得主线发布占用并按集成规范执行发布。当前主线发布占用未取得，已释放。

## 6. E296 释放后主线发布候选

E296 中央提交已重新核对为：

```text
E296=3434e9afa029302b0eaa188bd5457e05dab50f5c
E296父=54fc3dce5f20b98680a905520d9797be2129cd33
```

E296 的五份中央治理产物为计划索引、任务队列、并行工作树登记表、窗口交互记录和 `项目记忆/设计记录/20260720_DG-FORMAL-SPEC-SYSTEM-01_R1主线发布重叠WIP精确释放方案.md`。释放方案记录：已从 WT-MAIN 精确释放 69 项与集成范围重叠的任务 WIP，保留 6 项无关 WIP，释放后与集成范围交集为 0。

S0 重新核对结果：

```text
WT-MAIN main==origin/main==3434e9afa029302b0eaa188bd5457e05dab50f5c
WT-MAIN默认dirty=6项：控制面板、计划索引HR-000工作副本、三份复盘、日志
集成分支HEAD=0567ba4c63f5f3080745c6eb2062a60ebd9349ce
集成worktree=clean
集成远端与本地=0/0
```

直接比较 `3434e9a..0567ba4` 会包含计划索引这一中央路由差异；它属于 E296 中央侧变更，不是待覆盖的任务 WIP。以共同父 `54fc3dce..0567ba4` 计算的合入后候选预期范围为 104 个路径，与六项 WT-MAIN dirty 的交集为 0。

在取得本批主线发布占用后，已在 clean 集成 worktree 中无冲突合入 E296，形成：

```text
最终集成候选=102056e48ef06f45f421a6b923d3d30e3a8030ff
第一父=0567ba4c63f5f3080745c6eb2062a60ebd9349ce
第二父=3434e9afa029302b0eaa188bd5457e05dab50f5c
```

该提交当前仅是主线发布候选，尚未推送 `origin/main`，不得解释为 main 已发布或正式规范迁移最终完成。候选仍需完成最终严格规范、树一致性、范围和六项 dirty 保留复核；全部通过后才允许按门禁 fast-forward WT-MAIN main 并推送 `origin/main`。
