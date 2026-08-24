# ARCH-L2-GENERIC-VIRTUAL-EXISTENCE-AUTHORITY-CORRECTION 通用虚拟存在唯一权威纠偏代码实施切片

计划身份：`ARCH-L2-GENERIC-VIRTUAL-EXISTENCE-AUTHORITY-CORRECTION`
版本：v0.1
状态：设计就绪候选；取得真实 plan blob 并登记前不得施工
目标方案：`规范/详细设计/20260824_SELF-GOVERNANCE-CLOSURE_自我内部治理闭环纠偏与目标达成方案_v0.3.md`
详细设计：`规范/详细设计/20260824_ARCH-L2-GENERIC-VIRTUAL-EXISTENCE-AUTHORITY-CORRECTION_通用虚拟存在唯一权威纠偏详细设计_v0.1.md`
流程图：`流程图/20260824_ARCH-L2-GENERIC-VIRTUAL-EXISTENCE-AUTHORITY-CORRECTION_通用虚拟存在适配与状态动态依赖纠偏流程图_v0.1.md`

## 1. 唯一目标与 S0

删除无 ARCH-L1 owner、使用私有 map / 身份 / 事实代次且无生产消费者的 `L2通用虚拟存在结构服务`，让状态动态 provider 只消费由现有唯一 `L2存在结构服务`发布的正式 `L2存在身份`。

执行 S0 必须重新确认：

1. `ARCH-FIVE-LAYER-FOUNDATION v0.3`代码提交仍可达，当前索引已由本计划登记动作退出。
2. 全仓生产调用只有装配、工程和 provider 未使用持有；不存在后来新增的真实消费者。
3. `L2状态动态原子发布服务`方法体未调用冗余服务，状态 / 动态参与者仍各自验证正式存在身份。
4. 白名单文件无他主 tracked dirty，index 和发布租约可用。

若出现真实消费者、公开 ABI 使用或所有权冲突，返回具名设计漂移，不删除。

## 2. 精确实施

1. 从 `服务.L2状态动态原子发布.ixx`删除冗余 import、构造参数、初始化项和成员；保留请求 / 结果 DTO及双 owner 原子事务。
2. 从 `装配.普通应用.ixx`删除冗余 import、两个 getter、构造参数、成员、`make_unique`和向 provider 传参；保留原 `L2存在结构服务`装配和 getter。
3. 从 `.vcxproj`和 `.filters`删除两个冗余文件登记。
4. 删除两个冗余源码文件，不建立兼容壳或新 owner。
5. 新增专属施工 / 验证记录，列出函数级知识变更清单和完成边界。

## 3. 白名单

修改：

```text
海中鱼巣/领域/服务.L2状态动态原子发布.ixx
海中鱼巣/装配.普通应用.ixx
海中鱼巣.vcxproj
海中鱼巣.vcxproj.filters
```

删除：

```text
海中鱼巣/领域/L2通用虚拟存在结构.数据.h
海中鱼巣/领域/服务.L2通用虚拟存在结构.ixx
```

新增：

```text
施工记录/20260824_ARCH-L2-GENERIC-VIRTUAL-EXISTENCE-AUTHORITY-CORRECTION_施工记录.md
验证记录/20260824_ARCH-L2-GENERIC-VIRTUAL-EXISTENCE-AUTHORITY-CORRECTION_验证记录.md
```

禁止修改任务、需求、方法、线程、自我、状态 / 动态 / 特征 DTO、ARCH-L1 事务、计划 / 索引、知识库和其它异主 WIP。

## 4. 验证

```powershell
rg -n "L2通用虚拟存在结构" 海中鱼巣 海中鱼巣.vcxproj 海中鱼巣.vcxproj.filters
python .\tools\check_specs.py --strict
git diff --check
git diff --cached --check
```

根工程使用 VS 18 Professional 在独立输出目录执行 x64 Debug / Release fresh Rebuild；两配置必须 0 错误。不得使用仓库 `.codex-build`，不得把专项模块接入生产目标。

## 5. 完成条件和声明边界

完成条件：

- 两个冗余源码已删除且生产零引用；
- provider 和普通应用依赖收敛；
- Debug / Release、strict 和 diff 门禁通过；
- 只提交并推送白名单。

本计划不证明 G1 状态动态发布、任务消费者、任务轮次、授权、结算或自我内部治理闭环。发布后直接后继是 `TASK-ROUND-INITIAL-STATE-ARCH-L2-MIGRATION` 的设计和实施。
