# INSTINCT-STAGE3-MAINTENANCE-BASE-SNAPSHOT-COMPOSER 本能被动维护基础快照组合器代码实施计划 v0.1

日期：2026-08-29

计划身份：`INSTINCT-STAGE3-MAINTENANCE-BASE-SNAPSHOT-COMPOSER`

版本：v0.1

状态：待创建侧发布与计划索引登记

## 1. 目标与完成条件

实现同 G0 读取版本化安全根定义及 A、服务根 V、本能被动维护游标和单调完整秒证据的只读基础快照组合器，为阶段三后继完整输入集合提供稳定基础。

完成必须同时满足 P00—P08、Debug / Release 普通工程与专项、仓库外 ABI consumer、strict、精确提交和普通推送。只存在 DTO、只读单项成功或只验证版本 1 常量不能退出计划。

## 2. 正式依据与起点

- 0050 v2.1、6100 v0.5、6120 v0.10、6170 v0.6。
- `20260829_INSTINCT-STAGE3-MAINTENANCE-BASE-SNAPSHOT-COMPOSER_本能被动维护基础快照组合器详细设计_v0.1.md`。
- `20260829_INSTINCT-STAGE3-MAINTENANCE-BASE-SNAPSHOT-COMPOSER_读取本能被动维护基础快照函数流程图_v0.1.md`。
- 直接前置：规则内核结果提交 `78ed50c754c298993f8b51d87bbc398732c701fa`；安全根定义、当前值、游标和完整秒时钟公开 ABI 均已发布。
- 创建侧观察基线：`03afa4772decd194e70143d126518176c2c5df60`；执行 S0 必须重读实际 HEAD。

## 3. 允许文件

生产与专项：

```text
海中鱼巣/领域/本能被动维护基础快照.数据.h
海中鱼巣/领域/组合.本能被动维护基础快照.ixx
海中鱼巣/端到端测试.本能被动维护基础快照.ixx
海中鱼巣.vcxproj
海中鱼巣.vcxproj.filters
```

专属记录：

```text
施工记录/20260829_INSTINCT-STAGE3-MAINTENANCE-BASE-SNAPSHOT-COMPOSER_施工记录_v0.1.md
验证记录/20260829_INSTINCT-STAGE3-MAINTENANCE-BASE-SNAPSHOT-COMPOSER_验证记录_v0.1.md
```

禁止修改规范、流程图、详细设计、计划、计划索引、启动、普通应用、线程、运行期只读查询、现有 L1 / L2 / 安全定义 / 游标 / 时钟服务、服务合同和自我治理 WIP。执行只读消费它们。

## 4. 实施步骤

1. S0 核对 plan blob、HEAD、index、混合工程文件和现有四项服务公开 ABI。
2. 新增数据头，固定请求、快照、状态、完整性和 `成功/可维护` 谓词。
3. 新增只读组合器模块，按设计顺序调用游标、时钟、安全定义与当前值、服务根当前值并末次复核游标。
4. 新增 P00—P07 专项，显式覆盖后继安全根定义版本改变 L/H；不得把版本 1 常量写入生产组合器。
5. 新增 P08 仓库外 ABI consumer；专项把快照投影到单秒算法请求，证明 A/V/L/H/定义版本来自快照。
6. 精确登记三个源码，形成施工 / 验证记录和函数级知识变更清单。
7. 从精确 index 导出隔离树，完成两配置构建、专项和 consumer 后提交并普通推送。
8. 发布后退出索引并归档，按新 HEAD 形成服务合同 / 活动 / 到期 / 安全门禁完整集合的下一最早提供者。

## 5. 非成功与失败收口

- 所有非成功状态必须空快照、截止 0，禁止补默认 A/V/L/H、游标、时间或空集合。
- `无新完整秒` 是完整成功读回，不是错误，但 `可维护()==false`。
- 定义版本变化只通过正式安全根定义读回体现；旧 G、新 G 不得混合。
- 资源异常与内部异常分账；组合器零写入，无撤销、重放或已可能发布状态。

## 6. 验证与完成边界

仓库外验证根：`D:\TEMP\海中鱼巣\INSTINCT-STAGE3-MAINTENANCE-BASE-SNAPSHOT-COMPOSER\<本轮>`。

至少执行：普通 Debug / Release Rebuild、专项两配置、ABI consumer 两配置（Release 含 LTCG）、`python .\tools\check_specs.py --strict`、`git diff --check`、`git diff --cached --check`。

完成最多证明基础快照组合器已实现。服务合同、活动、到期事件、安全低位门禁完整集合，批量维护、游标推进、A/V 原子发布、状态 / 动态和生产接线仍未证明。

施工记录至少列出：

```text
本能被动维护基础快照完整_v1
本能被动维护基础快照读取结果_v1::成功
本能被动维护基础快照读取结果_v1::可维护
本能被动维护基础快照组合器::读取本能被动维护基础快照_v1
运行本能被动维护基础快照端到端测试
```
