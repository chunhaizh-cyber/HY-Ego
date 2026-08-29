# INSTINCT-STAGE3-PASSIVE-MAINTENANCE-RULE-KERNEL 单完整秒被动维护规则内核代码实施计划 v0.1

日期：2026-08-29

计划身份：`INSTINCT-STAGE3-PASSIVE-MAINTENANCE-RULE-KERNEL`

版本：v0.1

状态：待创建侧发布与计划索引登记

## 1. 目标与完成条件

实现 6170 v0.6 的单完整秒纯值规则内核，使后继服务生存治理聚合入口可以在完整材料已经裁决后，确定计算 V 衰减、服务比例和 A 被动回归候选。

完成必须同时满足 P00—P08、两配置构建、专项两配置运行、ABI consumer、严格规范检查、精确提交与普通推送。只编译通过或只验证常量不能退出本计划。

## 2. 正式依据与起点

- 0050 v2.1、6100 v0.5、6120 v0.10、6170 v0.6。
- `20260829_INSTINCT-STAGE3-PASSIVE-MAINTENANCE-RULE-KERNEL_计算单完整秒被动维护候选函数流程图_v0.1.md`。
- `20260829_INSTINCT-STAGE3-PASSIVE-MAINTENANCE-RULE-KERNEL_单完整秒被动维护规则内核详细设计_v0.1.md`。
- 直接前置结果提交：`2c850a8b5f8ee4959817d2df509d3900e7eb5e73`。
- 创建侧观察基线：`f5bc1c8e377e5516ba84e54cbb61d9bdb46cfa25`；执行 S0 必须重读实际 HEAD。

## 3. 允许文件

生产与专项：

```text
海中鱼巣/领域/本能单完整秒被动维护.数据.h
海中鱼巣/领域/算法.本能单完整秒被动维护.ixx
海中鱼巣/端到端测试.本能单完整秒被动维护.ixx
海中鱼巣.vcxproj
海中鱼巣.vcxproj.filters
```

专属记录：

```text
施工记录/20260829_INSTINCT-STAGE3-PASSIVE-MAINTENANCE-RULE-KERNEL_施工记录_v0.1.md
验证记录/20260829_INSTINCT-STAGE3-PASSIVE-MAINTENANCE-RULE-KERNEL_验证记录_v0.1.md
```

禁止修改规范、流程图、详细设计、计划、计划索引、普通应用、启动、线程、时钟、游标、L1、L2特征 / 状态 / 动态、安全定义、合同或自我治理 WIP。执行只读消费它们。

## 4. 实施步骤

1. S0 核对 plan blob、HEAD、index、混合工程文件 WIP 和 6170 常量。
2. 新增数据头，固定常量、枚举值、请求 / 载荷 / 结果和严格成功谓词。
3. 新增纯值算法模块，实现请求形状、V、P、A 及安全算术；不得读取外部状态。
4. 新增 P00—P07 专项；对边界值和每个具名分支逐项断言。
5. 精确登记三个新源码；从 index 导出隔离提交树构建并修复计划内纯代码错误。
6. 构造仓库外 P08 ABI consumer，分别以 Debug / Release 导入并调用。
7. 形成施工 / 验证记录和函数级知识变更清单。
8. 只暂存本计划增量，运行 cached diff-check、两配置与全部门禁后提交并普通推送。
9. 发布后退出计划索引并归档；立即按新 HEAD 形成下一最早叶。

## 5. 非成功与失败收口

- 合同、值域、阈值、需求形状或门禁组合非法：`入口拒绝`，空载荷。
- 规则版本不等于 v1：`版本漂移`，空载荷。
- 低位回升需要门禁但材料不足：`材料不足`，空载荷；禁止推进游标。
- 检查乘除或构造不满足不变量：`算术拒绝`或`内部错误`，空载荷。
- 任何失败不得产生默认候选、默认比例、当前值写入、状态、动态或持久事实。

## 6. 验证与成功条件

仓库外验证根固定在 `D:\TEMP\海中鱼巣\INSTINCT-STAGE3-PASSIVE-MAINTENANCE-RULE-KERNEL\<本轮>`。

至少执行：

```text
普通 Debug / Release Rebuild
专项 Debug / Release Rebuild + 运行
Debug / Release ABI consumer（Release含LTCG证据）
python .\tools\check_specs.py --strict
git diff --check
git diff --cached --check
```

成功条件：全部命令退出 0，P00—P08 全 PASS，精确提交树只含允许文件和专属记录，不依赖工作树其它 WIP。正式工程声明的工具集不可用时可以用本机已安装 v143 覆盖验证，但必须记录，不得修改工程声明冒充正式工具集。

## 7. 知识变更与完成边界

施工记录至少列出：

```text
本能单完整秒被动维护结果_v1::成功
计算本能单完整秒被动维护候选_v1
运行本能单完整秒被动维护端到端测试
```

完成最多证明单完整秒纯值规则内核已实现。合同 / 活动 / 到期 / 安全门禁生产 provider、批量分段、游标推进、A/V 原子发布、状态 / 动态、调度和整个阶段三仍为后继。
