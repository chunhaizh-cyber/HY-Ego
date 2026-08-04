# L1-SIMPLIFY-CONTRACT-F0 v0.1 验证记录

计划 blob：`f15e53a6b6a33b9f87e13b7e37e75c4112b7b24d`；代码施工基线：`697397a4f9ab66bc366460083b5ef7bf4a0c97c1`。

## 已执行

| 验证 | 结果 |
| --- | --- |
| Debug Rebuild | 通过；0 警告、0 错误 |
| Release Rebuild | 通过；0 警告、0 错误 |
| Debug `--self-test-exit` | 退出码 0；`L1-SIMPLIFY-P1-S01 checks=14 failed=0` |
| Release `--self-test-exit` | 退出码 0；完整专项输出通过 |
| `python .\tools\check_specs.py --strict` | 通过；110 份目录项全部通过 |
| `git diff --check` | 通过 |
| MSVC 模块扫描（Debug x64） | `sources=183, modules=183, failed=0`；循环模块 0、外部需求 0 |
| L2 数据头隔离 | `写集幂等键`、`写集本地键`、`L1写集请求`、`L1恢复材料`、`L1事实基座仓库` 命中 0 |
| 模块方向 | L1 服务唯一直接导入仓库；其它模块直接导入仓库为 0；领域服务不导出完整 L1 服务/合同 |
| 工程登记 | 两合同模块与公共事实头登记存在 |

## 完成边界与未验证范围

本记录只证明合同版本拒绝、服务/仓库单向依赖、L2 公开面隔离及当前既有行为矩阵在两配置下通过。未验证生产持久恢复、世界树完整业务、旧基座删除、跨编译器 ABI、性能或集成验收；提交/推送也不等同于上述能力完成。

提交哈希、唯一父、远端关系和 index 清洁状态以发布后 Git 回执为准。
