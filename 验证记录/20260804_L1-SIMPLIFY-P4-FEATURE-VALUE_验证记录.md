# L1-SIMPLIFY-P4-FEATURE-VALUE 验证记录

计划 blob：`c4e3dd2a30564a009e1374c8d75c9252aeaf027`
验证基线：`a201bcb21108bf99f763f5654fd472bdc7572c37`

## 结果

| 门禁 | 结果 |
| --- | --- |
| Debug x64 Rebuild | PASS |
| Release x64 Rebuild | PASS |
| Debug `--self-test-exit` | PASS，退出码 0 |
| Release `--self-test-exit` | PASS，退出码 0 |
| `check_specs.py --strict` | PASS，110/110 |
| MSVC 模块依赖扫描 | PASS，187/187，failed=0，循环模块=0，外部需求=0 |
| 目标 `git diff --check` | PASS |

专项自检新增覆盖：换代成功与代次推进、独立新当前读回、旧值已退出/历史读回、当前属性槽、精确重复、同键异义、旧基线竞争、错误当前/来源、越界值和未登记零写入。

## 未覆盖范围

其它宿主和值形状、关系22、批次、比较、状态动态、恢复、生产初始化、旧特征体系删除和集成验收未验证。
