# DATA-L2-GOVERNANCE-SERVICE-ORDINARY-APP-ENTRY 验证记录 v0.2

日期：2026-08-20

计划：`DATA-L2-GOVERNANCE-SERVICE-ORDINARY-APP-ENTRY v0.2`

## 结果

| 验证 | 结果 |
| --- | --- |
| Debug x64 fresh Rebuild | PASS，0 warning / 0 error |
| Release x64 fresh Rebuild | PASS |
| Debug `--test-需求` | PASS，exit 0 |
| Release `--test-需求` | PASS，exit 0 |
| Debug `--test-治理` | PASS，exit 0 |
| Release `--test-治理` | PASS，exit 0 |
| `python .\tools\check_specs.py --strict` | PASS，116 / 116 |
| 精确 diff check | PASS |
| 静态入口与退出读回检查 | PASS；需求测试中手工 L1 / owner / 服务构造为 0，普通应用两组 getter 各 2、成员各 1，退出后内部读回零当前投影查询 |

## 合法消费者已发生路径

`--test-需求`只经普通应用公开 getter 取得同一运行包中的结构服务，实际完成：

```text
形成配置并装配普通应用
-> 建立目标状态合同和需求列表项
-> 新增任务
-> 读取同一任务核心
-> 按需求列表项反查同一任务
-> 退出任务
-> 当前读取返回已退出或未找到
-> 退出需求并读回
```

首轮任务退出曾返回 `内部不一致(14)`；根因是退出后的当前投影无法发现已退出关系。修复后 Debug / Release 均完整通过，证明本次错误不是被测试绕过或降级为允许状态。

## 未验证范围

真实资源耗尽、发布未知、长期并发、跨进程恢复、任务方法路径全矩阵、任务业务服务、筹办、任务线程和完整治理循环均为 `NOT_RUN`。治理回归中的需求活动快照仍按现状返回“材料缺失”，不能据此声明 DATA-EXT-12 或真实治理闭合。
