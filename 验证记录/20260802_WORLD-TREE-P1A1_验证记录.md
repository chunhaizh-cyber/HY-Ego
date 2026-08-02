# WORLD-TREE-P1A1 验证记录

日期：2026-08-03

## 验证基线

- 计划：`WORLD-TREE-P1A1 v0.1`
- plan blob：`d6c102c67b3dd9f4d7807237cef818601c1be37a`
- 验证时正式基线：`main == origin/main == 6a8a06054bfb97dab38390886f9c7e4d496147d2`

## 机械结果

| 验证 | 结果 |
| --- | --- |
| Debug x64 `Rebuild` | PASS，exit 0；未输出 warning/error |
| Release x64 `Rebuild` | PASS，exit 0；未输出 warning/error |
| Debug `--self-test-exit` | PASS，exit 0 |
| Release `--self-test-exit` | PASS，exit 0 |
| `WORLD-P1A1-A01—A15` | 已接入完整结构自检聚合；两配置完整自检均 exit 0 |
| `git diff --check` | PASS |
| `python .\tools\check_specs.py --strict` | PASS，108/108 |
| 共享类型唯一声明扫描 | PASS；四个目标共享值只在 `节点直接结构合同.数据.h` 声明 |
| 三个新仓模块 import 扫描 | PASS；无命名模块 import |

## 矩阵覆盖

- 关系稳定键候选可见性、撤销恢复高水位、发布后永久占用。
- 源、目标、相关三组普通读取与本事务读取，合法空组、无效入口、稳定排序、无效端点和内部不一致分类。
- 关系权威材料的高水位、历史占用和记录组稳定排序。
- 类型合同候选读回、普通不可见、同义/异义、确认/撤销/发布。
- 类型化值初始版本 1、换代版本 2、前一版本、旧当前转历史、新当前、退役撤销和退役发布。
- 强类型来源联合的服务身份分支。
- 幂等候选读回、同义/异义、发布、不可变记录与持久证据侧账单调分离。

## 声明边界

本记录只证明 L1 稳定身份与类型记录仓代码候选通过本计划的构建、自检和静态矩阵；不证明统一查询、类型化结构固定事务提交、持久证据、恢复、世界树 L2 服务或生产接线完成。
