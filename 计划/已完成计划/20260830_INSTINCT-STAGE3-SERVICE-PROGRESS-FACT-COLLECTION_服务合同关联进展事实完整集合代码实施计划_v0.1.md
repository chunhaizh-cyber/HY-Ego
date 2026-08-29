# INSTINCT-STAGE3-SERVICE-PROGRESS-FACT-COLLECTION 服务合同关联进展事实完整集合代码实施计划 v0.1

日期：2026-08-30

计划身份：`INSTINCT-STAGE3-SERVICE-PROGRESS-FACT-COLLECTION`

版本：v0.1

计划状态：已完成（结果提交 `3a49b40c207c8282c9f4b6499d2324449bd763d4`）

## 1. 目标与完成条件

扩展现有唯一 `服务合同事实权威服务`，实现服务合同关联进展事实的同 owner 承载、独立扩展登记和 `G0` 下 0/N 完整集合读取。

完成必须同时满足：

1. 三个生产 / 专项文件按白名单完成且没有第二 owner、第二服务或公开进展写入口；
2. 既有基础登记不改写，`3014..301F` 扩展登记首次形成、重构和持久恢复成立；
3. 0 / 1 / N 进展组全部有完整见证、规范排序和共同非零 `G0`；
4. 进行中与非进行状态完整保留，provider 不裁决有效活动；
5. 任一坏成员、缺关系、错引用、漂移或资源失败整体非成功且零载荷；
6. Debug / Release、专项、ABI consumer、持久恢复、diff-check 和 strict 通过；
7. 形成专属施工 / 验证记录并精确提交、普通推送；
8. 完成后计划归档并从当前索引删除。

## 2. 正式依据与基线

正式依据：0050 v2.1、6100 v0.5、6120 v0.10、6160 v0.4、6170 v0.6，以及：

```text
规范/详细设计/20260830_INSTINCT-STAGE3-SERVICE-PROGRESS-FACT-COLLECTION_服务合同关联进展事实完整集合详细设计_v0.1.md
流程图/20260830_INSTINCT-STAGE3-SERVICE-PROGRESS-FACT-COLLECTION_服务合同关联进展事实完整集合业务流程图_v0.1.md
```

创建时观察基线：`main == origin/main == de9fd384bc4b3841bfd25c43d216171c4d25ac4a`，index 空。工作树存在无关 SELF-GOVERNANCE WIP，施工不得覆盖、删除、清理、stash 或整体暂存。

当前复用裁决：现有服务合同 owner 和 L1 owner 范围一致关系类型闭包直接扩展；不复用线程、队列、日志、任务状态枚举或 `服务活动裁决_v1` 作为事实来源。

## 3. 精确施工范围

修改：

```text
海中鱼巣/领域/服务合同事实权威.数据.h
海中鱼巣/领域/服务.服务合同事实权威.ixx
海中鱼巣/端到端测试.服务合同事实权威.ixx
```

新增：

```text
施工记录/20260830_INSTINCT-STAGE3-SERVICE-PROGRESS-FACT-COLLECTION_施工记录_v0.1.md
验证记录/20260830_INSTINCT-STAGE3-SERVICE-PROGRESS-FACT-COLLECTION_验证记录_v0.1.md
```

禁止修改：

```text
全部 L1 源码
L2需求 / 任务 / 方法 / 特征 / 状态 / 动态源码
普通应用、工程与 filters
运行期只读查询、基础快照、规则内核、游标和线程入口
SELF-GOVERNANCE WIP
```

编译闭包若要求修改禁止文件，先退回设计，不得临场扩大范围。

## 4. 实施步骤

### 4.1 DTO 与成功谓词

按详细设计第 4 节增加身份、运行状态、事实、请求、见证、结果及完整性 / 成功谓词。所有公开值类型逐字段相等；成功谓词必须逐项复核，不得只看状态码。

### 4.2 扩展登记

在同模块中新增独立扩展登记幂等身份和 `3014..301F` 定位。构造期先恢复基础登记，再建立 / 恢复扩展登记。基础登记首次写入材料与既有数据保持字节级语义不变。

### 4.3 完整集合读取

实现：

```cpp
服务进展完整集合读取结果_v1
读取当前服务合同关联进展完整集合_v1(
    const 服务进展完整集合读取请求_v1&) const noexcept;
```

严格执行请求校验、G0 双守卫、同 owner 闭包、登记 / 关系 / 载荷 / 合同引用互证、规范排序和完整见证。非成功清空全部载荷。

### 4.4 验证 fixture

只在 `ARCH_INSTINCT_SERVICE_CONTRACT_FACT_AUTHORITY_VALIDATION` 下增加进展样本 fixture 和故障注入。生产构建不得导出或可调用写入口。

## 5. 失败收口

- 扩展登记建立 / 恢复失败：服务构造失败，不降级为空进展集合。
- G0 漂移：`当前性漂移`，空组、空见证、截止 0。
- 成员、载荷、关系或合同互证失败：`集合不闭合`或`引用冲突`，不跳过成员。
- L1 资源失败：`资源失败`；异常映射为资源失败或内部错误。
- 读后守卫失败：丢弃首次候选，不缓存、不复用旧结果。

## 6. 验证

至少执行：

1. `git diff --check`、精确 staged `git diff --cached --check`；
2. `python .\tools\check_specs.py --strict`；
3. 本机已验证工具集 `v143` 的 x64 Debug / Release 根工程 Rebuild；
4. 专项 P00—P09，覆盖详细设计第 7 节；
5. 仓库外 Debug ABI consumer；
6. 仓库外 Release `/GL + /LTCG` ABI consumer；
7. 静态扫描：生产无 fixture、无公开进展写入口、无第二 owner、无线程 / 日志 / 队列推断；
8. 持久恢复：扩展登记和进展 fixture 恢复后逐字段一致。

验证根使用：

```text
D:\TEMP\海中鱼巣\INSTINCT-STAGE3-SERVICE-PROGRESS-FACT-COLLECTION\<本轮唯一标识>
```

## 7. 记录与知识变更材料

施工记录必须列出新增稳定函数、DTO、扩展 key、调用边、事实读写和未实现生产者。验证记录逐项写明 PASS / FAIL / NOT_RUN。

完成后向知识维护交付：

```text
读取当前服务合同关联进展完整集合_v1
服务进展 DTO 完整性与成功谓词
服务合同事实权威构造的扩展登记增量
```

本计划不直接修改共享知识库。

## 8. 完成声明边界

本计划完成只证明服务合同关联进展事实完整集合 provider。不得声明生产进展发布已接线、有效服务活动已裁决、安全门禁已成立、服务值衰减 / 安全根回归已发布、阶段三、SERVICE-C1 或 `INSTINCT-ROUTE` 完成。
