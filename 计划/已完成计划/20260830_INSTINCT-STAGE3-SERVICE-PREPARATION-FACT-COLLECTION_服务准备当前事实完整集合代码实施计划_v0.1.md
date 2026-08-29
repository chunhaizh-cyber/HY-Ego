# INSTINCT-STAGE3-SERVICE-PREPARATION-FACT-COLLECTION 服务准备当前事实完整集合代码实施计划 v0.1

日期：2026-08-30

计划身份：`INSTINCT-STAGE3-SERVICE-PREPARATION-FACT-COLLECTION`

## 1. 目标与完成条件

扩展现有唯一 `服务合同事实权威服务`，实现 `读取当前服务准备完整集合_v1`，为 6160 / 6170 提供指定自我、指定 `G0` 下 0/N 当前服务准备事实的完整强类型集合和机器见证。

完成必须同时满足：

1. 同 owner 准备扩展登记建立、恢复和冲突拒绝；
2. DTO、完整性、见证和强成功谓词与详细设计一致；
3. 0/N 集合完整、规范排序、同 `G0`、非成功零载荷；
4. 需求 / 能力缺口来源二选一和九种运行状态保真；
5. 两配置根构建、专项、ABI consumer、持久恢复、strict 和 diff-check 通过；
6. 生产代码没有 fixture、公开准备写入口、有效活动裁决或 L/H 修改。

## 2. 正式依据与当前事实

正式依据：0050、6100、6120、6160、6170，以及：

```text
规范/详细设计/20260830_INSTINCT-STAGE3-SERVICE-PREPARATION-FACT-COLLECTION_服务准备当前事实完整集合详细设计_v0.1.md
流程图/20260830_INSTINCT-STAGE3-SERVICE-PREPARATION-FACT-COLLECTION_服务准备当前事实完整集合业务流程图_v0.1.md
```

创建基线：`main == origin/main == 7a187802c89edb1ec2d22c73900b5d9a527ac0aa`，index 空。当前服务合同 owner 已发布合同、到期事件和合同关联进展完整集合；服务准备完整集合不存在。未提交 SELF-GOVERNANCE 文件及语义不属于本计划。

当前阈值定义保持：值域 `0..I64_MAX`，`L=2,767,011,611,056,432,742`，`H=7,378,697,629,483,820,645`；未来只能通过新定义版本调整。

## 3. 实施白名单

允许修改：

```text
海中鱼巣/领域/服务合同事实权威.数据.h
海中鱼巣/领域/服务.服务合同事实权威.ixx
海中鱼巣/端到端测试.服务合同事实权威.ixx
施工记录/20260830_INSTINCT-STAGE3-SERVICE-PREPARATION-FACT-COLLECTION_施工记录_v0.1.md
验证记录/20260830_INSTINCT-STAGE3-SERVICE-PREPARATION-FACT-COLLECTION_验证记录_v0.1.md
```

禁止修改：正式规范、详细设计、流程图、计划 / 索引、普通应用、工程文件、L1、任务 / 方法 / 安全 owner、线程、启动、现有 SELF-GOVERNANCE WIP 和生产 L/H 定义。

## 4. 实施步骤

1. 在数据头新增准备身份、来源 variant、运行状态、事实、读取请求 / 见证 / 结果和全部完整性谓词。
2. 在服务构造中追加 `0x494E'5354'5350'5250` 幂等登记和 `0x0307'3020..302D` 定位恢复；保留 `302E..302F`。
3. 实现准备载荷编码 / 解码、来源二选一关系闭合和全部可选引用闭合。
4. 实现 `读取当前服务准备完整集合_v1` 的读前守卫、完整闭包、规范排序、见证、读后守卫和结构化失败。
5. 只在验证宏内扩展 fixture，覆盖 0/N、全部状态、坏来源、坏引用、坏载荷、漂移、资源失败和恢复。
6. 补齐施工 / 验证记录，执行全部门禁并只发布本计划白名单。

## 5. 失败收口

- 登记建立 / 恢复失败：服务构造失败，不降级为空准备集合。
- G0 漂移：`当前性漂移`，空组、空见证、截止 0。
- 成员、载荷、来源或引用互证失败：`集合不闭合`或`引用冲突`。
- 资源或异常：映射为既有结构化状态，不返回部分组。
- 提交后读回失败：遵守 L1 已可能发布 / 幂等重放边界，不以日志或内存缓存补成功。

## 6. 验证

1. `git diff --check` 和精确 staged `git diff --cached --check`；
2. `python .\tools\check_specs.py --strict`；
3. v143 x64 Debug / Release 根工程 Rebuild；
4. 专项覆盖详细设计第 7 节；
5. 仓库外 Debug ABI consumer、Release `/GL + /LTCG` ABI consumer；
6. 持久恢复后逐字段一致；
7. 静态扫描确认无第二 owner、无公开准备写入口、无生产 fixture、无 L/H 改动。

验证根：

```text
D:\TEMP\海中鱼巣\INSTINCT-STAGE3-SERVICE-PREPARATION-FACT-COLLECTION\<本轮唯一标识>
```

## 7. 记录与完成边界

施工记录列出新增 DTO、稳定入口、登记幂等身份、键段、读写边和未实现生产者；验证记录逐项写明 PASS / FAIL / NOT_RUN。

本计划完成只证明服务准备当前事实完整集合 provider。不得声明生产准备发布、准备结算 / 补回、有效服务活动、安全门禁、阶段三或 `INSTINCT-ROUTE` 完成。
