# DATA-L3-WORLD-TREE-NEW-SERVICE-CUTOVER 验证记录 v0.1

日期：2026-09-12
结论：计划切片专项通过；根工程和 strict 存在范围外阻断；一项验证资源缺口为 NOT_RUN

## 1. 身份与基线

- 正式计划：`DATA-L3-WORLD-TREE-NEW-SERVICE-CUTOVER` v0.3。
- 计划 blob：`eb501042caa9dccadcf4c8cc470a4ebff3c53ff4`；详细设计 blob：`2cc2e9997d8a2f1f228fe8101e8a629fecea5cc6`。
- 起点及验证时正式 HEAD：`590bc3075019df7c2a92acab346ce9533b4bf100`；`HEAD == origin/main`，ahead/behind=`0/0`。
- index 为空；工作区含大量异主 WIP，未清理、覆盖或带入本切片。

## 2. Debug/Release 专项

仓库外驱动：

- `D:/TEMP/海中鱼巣/DATA-L3-WORLD-TREE-NEW-SERVICE-CUTOVER/budget-idempotency-probe/世界树预算幂等专项验证.cpp`
- `D:/TEMP/海中鱼巣/DATA-L3-WORLD-TREE-NEW-SERVICE-CUTOVER/budget-idempotency-probe/世界树预算幂等专项验证.vcxproj`

| 配置 | Rebuild | Run |
| --- | --- | --- |
| Debug x64 | PASS，0 warning，0 error | `WORLD_TREE_BUDGET_IDEMPOTENCY_OK 247 PASS` |
| Release x64 | PASS，0 warning，0 error | `WORLD_TREE_BUDGET_IDEMPOTENCY_OK 247 PASS` |

日志：

- `budget-idempotency-probe-debug-build.log`
- `budget-idempotency-probe-debug-run.log`
- `budget-idempotency-probe-release-build.log`
- `budget-idempotency-probe-release-run.log`

## 3. 专项覆盖

1. 扩展登记首次、精确重放、同键异义，以及五项身份互异和同 owner。
2. 两棵独立场景树、无父旧 C 不认根、只有根的空树和稳定排序。
3. 直接存在成员新增、当前组/父读取、迁移、退出和历史；直接子场景创建、六个具名读取、同树迁移、跨树拒绝和历史。
4. 存在组成、场景存在成员、场景子场景三类联合单父；包含跨类别并发竞争。
5. 混合成员/组成链、三类角色投影、未启用/未纳树/根/非根/已退出 C，以及存在子关系固定 4096 边界。
6. 三类树角色启用的单写与预算重放；预算不构成幂等同一性，低预算重放不改事实，恢复预算后精确重复。
7. 世界树六入口主要成功、精确重放、预算不足、错父、同父、事实代次漂移和后继覆盖。
8. 场景创建与存在创建的第二阶段交换前漂移、完整续跑请求和续跑成功。
9. 角色退出的根/非根树、成员依赖保护和“C 仅作为普通 E 被指向不误拦”；边退出前后历史读回。
10. 零身份、坏版本、超预算、不存在成员、多来源父、坏 Gread 等非成功结果零假成功载荷。

## 4. 根工程与静态门禁

| 验证 | 结果 | 证据边界 |
| --- | --- | --- |
| Debug x64 根工程隔离 Rebuild | FAIL（范围外） | 日志 138 条错误行均来自`数据服务.概念树类.ixx`；本切片五个生产模块无诊断 |
| Release x64 根工程隔离 Rebuild | FAIL（范围外） | 与 Debug 相同 |
| 七文件 scoped `git diff --check` | PASS | 本切片无空白错误 |
| 全仓 `git diff --check` | FAIL（范围外） | 异主 `AGENTS.md:12` 尾随空格 |
| `python .\tools\check_specs.py --strict` | FAIL（范围外） | 124 项；FROZEN 清单缺存在的 `acceptance_record` |
| 旧世界树符号扫描 | PASS | `场景类成员/世界树应用状态/验证世界/读取场景历史事实`零命中 |
| 入口扫描 | PASS | 六个世界树入口、六个具名 L2 读取入口均存在 |

根工程日志：

- `D:/TEMP/海中鱼巣/DATA-L3-WORLD-TREE-NEW-SERVICE-CUTOVER/root-rebuild-final-debug/build.log`
- `D:/TEMP/海中鱼巣/DATA-L3-WORLD-TREE-NEW-SERVICE-CUTOVER/root-rebuild-final-release/build.log`

## 5. NOT_RUN 与静态分账

- `M8 第二阶段真实已可能发布`：`NOT_RUN（验证资源缺口）`。当前确定性内存 L1 和具体 final 场景服务没有合法、稳定的提交后读回失败触发器；禁止为测试增加 getter、故障注入或生产接口。相应冻结原请求与发布未知代码路径仅完成静态复核。
- F/S/D/B 四类组织关系对场景角色退出的独立运行夹具未全部建立；现有运行覆盖与其余静态路径分开记录，不提升为完整运行 PASS。

## 6. 结论边界

本记录证明候选切片的 Debug/Release 专项矩阵通过，并证明目标模块在根工程扫描/编译推进到范围外概念树阻断前无诊断。它不证明根工程总体构建、概念树、普通应用生产装配、跨进程恢复、正式集成验收或业务闭环。提交、推送和远端 blob 读回以最终完成回执为准。
