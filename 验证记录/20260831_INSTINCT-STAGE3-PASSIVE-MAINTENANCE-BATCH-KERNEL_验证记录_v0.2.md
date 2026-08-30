# INSTINCT-STAGE3-PASSIVE-MAINTENANCE-BATCH-KERNEL 验证记录 v0.2

日期：2026-08-31

## 1. 验证对象与隔离根

- 正式起点：`fa174a3f784e156deb2c73822091360b449e4cd7`。
- 候选：起点提交树叠加本计划三个源码。
- 仓库外隔离根：`D:\TEMP\海中鱼巣\INSTINCT-STAGE3-PASSIVE-MAINTENANCE-BATCH-KERNEL\20260831-verify-01`。
- 隔离根由 `git archive HEAD` 形成并只覆盖本计划三个源码；没有包含共享工作区的 SELF-GOVERNANCE WIP。
- 临时 `batch_test_main.cpp` 与 `batch_abi_consumer.cpp` 只位于隔离根，不进入仓库或生产工程。

## 2. 构建和专项

| 验证 | 结果 |
| --- | --- |
| Debug x64 根工程 `Rebuild`，toolset v143 | PASS，exit 0，0 warning，0 error |
| Release x64 根工程 `Rebuild`，toolset v143 | PASS，exit 0 |
| Debug 独立专项入口直接调用 `运行本能单完整秒被动维护端到端测试()` | PASS，P00—P07、B00—B10 全部通过，exit 0 |
| Release 独立专项入口 | PASS，P00—P07、B00—B10 全部通过，exit 0 |
| Debug 外部 ABI consumer 同时调用单秒和批量入口 | PASS，编译、链接、运行 exit 0；独立 consumer 的模块编译环境提示 C5050 和 Debug 对象 LNK4075，不影响根工程 0 warning 结果或运行结论 |
| Release 外部 ABI consumer 同时调用单秒和批量入口 | PASS，编译、LTCG 链接、运行 exit 0 |

根构建命令：

```powershell
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v143 /m
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v143 /m
```

## 3. 专项矩阵

| 编号 | 覆盖 | 结果 |
| --- | --- | --- |
| B00 | N=0；合同 / 版本 / 固定阈值；空洞、重复键、相邻可合并未合并、非法到期段 | PASS |
| B01 | 无需求短区间与逐秒 reference | PASS |
| B02 | 最长未满足合同的确定性地板和与逐秒 reference | PASS |
| B03 | 跨 `t=T` 与同秒到期未满足事件 | PASS |
| B04 | 有效活动最低 1 保护 | PASS |
| B05 | 低位允许 / 禁止 / 材料不足及非适用分支不消费材料 | PASS |
| B06 | 零预算与少一预算均为空载荷`输出预算不足` | PASS |
| B07 | 服务比例跨档切段 | PASS |
| B08 | A=`0/1/L/H/INT64_MAX` | PASS |
| B09 | 区间 `(0, INT64_MAX]`，输出至多两个审计段 | PASS |
| B10 | 64 组确定性短随机区间逐秒 reference | PASS |

短区间 reference 逐秒比较最终 A/V、服务分支、安全分支、服务比例和低位门禁投影。B09 与同一进程内全部专项快速完成，证明该路径没有按 `INT64_MAX` 个完整秒线性执行；源码边界只依赖输入事实段、最多 99 个比例区间、每比例最多 10,000 个安全收敛步骤和固定次数二分。

## 4. 静态门禁

| 门禁 | 结果 |
| --- | --- |
| `git diff --check` | PASS；仅现有 LF/CRLF 提示，无空白错误 |
| `git diff --cached --check` | PASS；执行时 index 空 |
| `python .\tools\check_specs.py --strict` | PASS，116/116 |

共享工作区根工程曾受异主 SELF-GOVERNANCE 未提交接口 WIP 阻断，因此正式构建结论使用上述干净隔离提交树。隔离根中的生产程序 `--self-test-exit` 仍返回基线 exit 1 且不调用本专项；本计划按既有模块边界使用独立临时入口直接调用目标测试函数，不把全量自检退出码冒充本专项证据。

## 5. 结论边界

已证明多完整秒纯批量候选内核在 Debug / Release 中可构建、可由外部模块消费，并在短区间与单秒参考等价、在 I64 大跨度下保持有界执行。未证明历史事实材料、外部 A/V 调整 composer、四 owner 原子事务、生产接线、阶段三或 INSTINCT-ROUTE 完成。
