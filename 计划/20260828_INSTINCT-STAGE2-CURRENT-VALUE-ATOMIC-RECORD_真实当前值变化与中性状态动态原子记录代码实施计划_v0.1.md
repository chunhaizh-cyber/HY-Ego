# INSTINCT-STAGE2-CURRENT-VALUE-ATOMIC-RECORD 代码实施计划

日期：2026-08-28
版本：v0.1
状态：待创建侧发布与索引登记

## 1. 计划身份与目标

计划身份：`INSTINCT-STAGE2-CURRENT-VALUE-ATOMIC-RECORD`。

目标：在已发布 L1 三分区原子事务和本能根锚点之上，实现 `发布特征当前值变化与中性记录_v1`、独立读回和本能根恢复适配，取得本能路线阶段二的第一个真实代码结果。

直接依赖：

```text
INSTINCT-BOOTSTRAP-ROOT-ANCHOR 结果 f0a83999185288ff3da657a6b61e118fe79c524e
DATA-L1-THREE-PARTITION-ATOMIC-V2 结果 ff2366d97b662cfc6c8cf16917c06d5da182bf4b
4230 v0.18
4210 v0.4
同名详细设计 v0.1
```

## 2. 完成条件

1. 公开 DTO、固定数值和两个入口与详细设计逐字段一致。
2. 三个 owner 只通过 L1 v2 一次原子事务发布，旧迁移入口继续零写。
3. legacy 第一次变化、正常变化、无变化、来源方法可选、重放和已可能发布闭合。
4. 普通应用唯一构造、getter、独立读回和本能根重启恢复接线完成。
5. v143 Debug/Release Rebuild、ABI consumer、V01—V19 取得具名结果；不可达项明确 `NOT_RUN`。
6. 代码、施工记录和验证记录精确提交并普通推送；计划退出不夸大阶段三或整个路线。

## 3. 允许文件

生产白名单严格等于详细设计第 10 节的十个生产文件；专项验证只允许新增：

```text
海中鱼巣/端到端测试.本能路线阶段二.ixx
海中鱼巣.vcxproj
海中鱼巣.vcxproj.filters
施工记录/20260828_INSTINCT-STAGE2-CURRENT-VALUE-ATOMIC-RECORD_施工记录_v0.1.md
验证记录/20260828_INSTINCT-STAGE2-CURRENT-VALUE-ATOMIC-RECORD_验证记录_v0.1.md
```

不修改 L1、SELF-GOVERNANCE 线程/治理文件、现有自我治理端到端测试、路线方案、正式规范、详细设计、流程图或计划索引。执行发现正式合同漂移时只停止受影响代码切片并回到本计划设计，不临场改语义。

## 4. 实施步骤

### S0 当前事实与知识核对

- 核对计划索引精确版本/blob、HEAD/origin/index、目标文件 dirty 和 Git 租约。
- 重读 L1 三分区 DTO/入口、三个服务现有物理编码、普通应用构造顺序和本能根恢复分支。
- 核对函数用途知识基线；过期条目只作候选，源码为准。

### S1 DTO 与上下文登记

- 在三个数据头增加公开请求、结果、读取和上下文事实，不复义旧类型。
- 状态/动态服务增加独立固定关系类型登记；构造失败结构化映射到既有装配失败，不自动修复异义登记。

### S2 三个参与者 helper

- 特征 helper：验证完整旧值，形成换代写集。
- 状态 helper：正常/legacy 基线两种形状，形成关系 19、中性选择替换。
- 动态 helper：形成迁移动能；可选形成动作致变动态与同源关系。
- helper 为 module-private，只向唯一组合服务开放，不形成第二公开写入口。

### S3 组合服务

- 建立三端口凭证，固定 1/2/3 owner。
- 实现无变化早退、读前守卫、三参与者提交、L1 状态映射、首次/重复映射解释、已可能发布和独立读回。
- 保持旧 v1/v2 入口零写拒绝。

### S4 恢复和装配

- 普通应用移动唯一组合服务构造点并注入存在/场景/特征/状态/动态同实例服务。
- 本能根恢复优先消费固定角色的中性选择；无中性选择且值仍为初值时保留 legacy 兼容。

### S5 专项验证

- 新增独立测试模块，从普通应用 getter 调用公开入口和独立读回。
- 使用受控临时持久根验证首次、重放、连续迁移和进程重建。
- 故障注入只在验证宏内触发提交后首次读回失败，不进入生产结果语义。

### S6 记录、发布与后继

- 施工记录包含代码知识变更清单和结果提交待填项。
- 验证记录逐项列 V01—V19、命令、配置、证据类别和 NOT_RUN。
- 精确暂存本计划文件，执行 cached diff-check 和提交树构建后提交、普通推送。
- 发布后把本计划从索引退出并归档；交互目标核验阶段二结果后才形成阶段三计划。

## 5. 失败收口

- 编译/代码错误：在白名单内修复。
- L1 v2 当前接口与计划漂移：停止组合实现并修订计划。
- 现行数据无法受约束迁移 legacy 首态：返回材料不足，不清理、不补写。
- 已可能发布：保留原完整请求和四幂等身份，只允许原请求重放。
- 异主 WIP：不覆盖、不整文件暂存；必要时以精确 hunk 构造 index。

## 6. 验证命令与证据边界

最低命令：

```powershell
git diff --check
python .\tools\check_specs.py --strict
MSBuild.exe .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v143
MSBuild.exe .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v143
```

再运行专项测试/ABI consumer 命令并记录实际入口。编译只证明构建；专项 provider 测试不自动证明普通应用接线；提交/推送只证明产物进入 Git；本计划完成不证明阶段三至九。
