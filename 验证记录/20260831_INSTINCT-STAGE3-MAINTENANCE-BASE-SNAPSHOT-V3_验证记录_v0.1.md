# INSTINCT-STAGE3-MAINTENANCE-BASE-SNAPSHOT-V3 验证记录 v0.1

日期：2026-08-31

## 1. 验证对象

- 计划：`INSTINCT-STAGE3-MAINTENANCE-BASE-SNAPSHOT-V3 v0.1`
- S0 起点：`4bc4eed084f2f8c2ca8a5754f03e32878bfef67a`
- 生产修改：基础快照数据头、组合器模块
- 专项修改：本能被动维护基础快照端到端测试
- 仓库外验证根：`D:/TEMP/海中鱼巣/INSTINCT-STAGE3-MAINTENANCE-BASE-SNAPSHOT-V3`

隔离候选由 S0 HEAD 导出，仅覆盖本计划三个源码候选；临时专项入口和 ABI consumer 不进入仓库候选。

## 2. 验证结果

| 门禁 | 配置 | 结果 |
| --- | --- | --- |
| 默认工具集根工程 `ClCompile` | Debug x64 | PASS |
| 默认工具集根工程 `ClCompile` | Release x64 | PASS |
| v143 根工程 Rebuild/链接 | Debug x64 | PASS |
| v143 根工程 Rebuild/链接 | Release x64 | PASS |
| `--test-instinct-base-snapshot` | Debug x64 | PASS；原 v1 P00-P08、A00-A06、Q00 与新增 V200-V209 通过 |
| `--test-instinct-base-snapshot` | Release x64 | PASS；同上 |
| 外部 v1/v2 ABI consumer 编译、链接、运行 | Debug x64 | PASS；exit 0 |
| 外部 v1/v2 ABI consumer 编译、链接、运行 | Release x64 | PASS；exit 0 |
| `python .\tools\check_specs.py --strict` | 当前工作树 | PASS；116/116 |
| `git diff --check` | 三源码候选 | PASS |

专项 v2 证明：

- v3 未建立与 v1 待迁移分别返回 `游标未建立`、`游标待迁移`；
- 成功快照逐字段保留主动安全变化事实水位、服务维护历史事实水位，且二者不晚于 G0；
- 无新完整秒、精确 1 秒和 N 秒区间形状正确；
- 运行代次、G0、时间纪元、时间源版本、时间倒退和坏服务实例均 fail-closed；
- v1 原专项未因 v2 fixture 提前登记账或迁移游标而改变。

## 3. 静态边界扫描

对 `读取本能被动维护基础快照_v2` 函数体扫描结果：

```text
读取 v1 游标命中 = 0
读取 v3 游标命中 = 2
L/H 百分比或赋值重算命中 = 0
游标写入/推进命中 = 0
历史变化组或服务历史范围读取命中 = 0
```

## 4. 分账与未证明

1. 当前共享工作树全编译在异主 SELF-GOVERNANCE WIP 处失败，不是本切片编译错误；隔离候选两配置通过。
2. 默认 v145 全链接仍受当前基线跨模块未解析符号影响；默认 Debug/Release 编译通过，v143 两配置完整链接和专项通过。未把失败隐藏为成功。
3. Q01-Q06 依赖的服务合同验证宏在普通工程未启用，仍显示 `NOT_RUN`；它属于既有 v1 扩展矩阵，不影响新增 v2 的 V200-V209 结果。
4. 未验证历史事实段、维护事务、游标推进、A/V 写入、状态/动态发布或生产线程接线。

因此本记录只支持“v3 三水位基础快照 v2 provider 已实现并通过两配置专项”，不支持阶段三或本能路线整体完成。
