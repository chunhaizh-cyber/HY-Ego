# INSTINCT-STAGE3-GSEED-ACTIVATION-CURSOR-COMPAT-MIGRATION Gseed 验证闭合代码实施计划 v0.3

日期：2026-09-01

状态：正式代码实施计划候选；当前执行状态只由 `计划/计划索引.md` 的精确 blob 登记决定

计划身份：`INSTINCT-STAGE3-GSEED-ACTIVATION-CURSOR-COMPAT-MIGRATION`

恢复基线：`6bf847089bbf4e9af4261ed06dbd790e369decb1`

## 1. 正式依据

- 原计划 v0.2：`计划/20260831_INSTINCT-STAGE3-GSEED-ACTIVATION-CURSOR-COMPAT-MIGRATION_Gseed显式激活与维护游标兼容迁移代码实施计划_v0.2.md`，blob `08ba2aee3c352f95b8f4c9669af3f6085ebf3f30`；
- 修订详细设计 v0.2：`规范/详细设计/20260901_INSTINCT-STAGE3-GSEED-ACTIVATION-CURSOR-COMPAT-MIGRATION_Gseed显式激活与维护游标兼容迁移详细设计_v0.2.md`，blob `2a1f2045b81385c4944e85869b6812f8bde83004`；
- Gseed 安全代码 WIP 提交 `b38059b6b501c49705249f5d5587f6d1f9dba2e9`；
- ABI 结果提交 `497746760fada7c989d6747abf42b0f794355875`；
- 执行恢复证据根 `D:\TEMP\海中鱼巣\INSTINCT-STAGE3-GSEED-ACTIVATION-CURSOR-COMPAT-MIGRATION\20260901-gseed-resume-01`。

本计划以 v0.3 取代 v0.2 的当前执行候选身份；v0.2 只作原实现合同与历史证据。

## 2. 目标与完成边界

不改变已经发布的生产 DTO、服务实现、公开 ABI、owner、写集或启动接线，只完成：

1. 修复 Gseed 专项的隔离配置和持久子根，使已经写入源码的 G01—G05 能合法运行；
2. 按详细设计 v0.2 的 V01—V10 登记已取得的构建、专项、ABI 和静态证据；
3. 对没有合法夹具的 B、真实普通启动和故障场景保留 `NOT_RUN`；
4. 形成可精确发布的测试与两份专属记录结果。

完成最多证明 Gseed 代码结果以及 A 激活 / 恢复、U 安全拒绝、幂等、旧投影和持久恢复专项运行闭合；不证明 B 运行、真实普通启动、故障恢复、B 完整迁移、阶段三、本能路线或业务集成验收完成。

## 3. 允许与禁止范围

允许修改且只允许修改：

```text
海中鱼巣/端到端测试.本能被动维护游标.ixx
施工记录/20260831_INSTINCT-STAGE3-GSEED-ACTIVATION-CURSOR-COMPAT-MIGRATION_施工记录_v0.1.md
验证记录/20260831_INSTINCT-STAGE3-GSEED-ACTIVATION-CURSOR-COMPAT-MIGRATION_验证记录_v0.1.md
```

测试文件只允许修复：

```text
形成隔离配置：填入形成普通应用任务筹办等待合同登记()
Gseed v4 子场景：建立 l1 / material 目录
legacy-v1/v2/v3 子场景：分别建立 l1 / material 目录
```

禁止修改生产 DTO、游标服务、启动、工程、其它测试、规范、知识库、流程图和其它 owner 文件；计划状态维护由计划索引所有者另行处理。禁止为 B 或故障场景建立私有写入口、篡改 L1 持久材料或运行真实 AppData。

## 4. 混合文件和所有权保护

`海中鱼巣/端到端测试.本能被动维护游标.ixx` 仍包含异主 WIP：外层普通测试删除

```cpp
配置.等待合同登记 = 形成普通应用任务筹办等待合同登记();
```

该删除不得恢复、暂存或提交。本计划新增的同名配置位于独立 `形成隔离配置()` helper，语义和 hunk 均可分离。发布时只能把三个 Gseed 夹具修复 hunk写入 index；未暂存 diff 必须继续保留异主删除 hunk。`海中鱼巣/启动.应用程序.ixx` 的两个异主装配 hunk也必须原样保留。

## 5. 实施与验证步骤

1. 重新 S0：核对 `main`、HEAD / origin、index、v0.3 plan/design blob、两个结果提交祖先关系、三文件所有权和异主 hunk；
2. 复核专项夹具修复只发生在 Gseed 独立 helper / 子场景；
3. 核对 V01 / V02 当前工作树 Debug / Release fresh Rebuild 日志；
4. 核对 exact-HEAD 隔离源码、仓外入口投影和 G01—G05 Debug / Release 日志与 exit 0；
5. 核对全局模块 ABI consumer 双配置编译、链接、运行 exit 0；
6. 重做生产接线、B 分支、唯一 owner / 零写和旧投影静态审计；
7. 在两份专属记录中分账 PASS、NOT_RUN、证据路径和完成边界；
8. 运行 `git diff --check`、`git diff --cached --check`、`python .\tools\check_specs.py --strict`；
9. 精确暂存三文件并逐 hunk 审计，普通提交并推送；
10. 推送后核对提交、远端、index 和异主 WIP，再由计划索引所有者登记退出。

若新 HEAD 只增加本次设计 / 计划治理文件，而三份结果文件内容、四份已发布生产源码 blob、证据日志和工具链均未改变，V01—V10 可以在逐项复核后复用，不机械重跑昂贵构建。

## 6. 验证矩阵

必须满足修订详细设计 v0.2 的 V01—V10。特别是：

```text
Debug / Release fresh Rebuild 完整链接
G01—G05 Debug / Release 均 PASS / exit 0
外部 ABI consumer 双配置编译、链接、运行 exit 0
生产接线、B 零写分支、唯一 owner、其它 owner 零写和旧投影静态 PASS
diff-check / cached-check / strict PASS
异主 WIP 完整保留
```

以下必须写为 `NOT_RUN`，但在本计划不再作为退出阻断：

```text
B 成功维护提交序号 1/N 运行
真实普通启动 A/B/U
已可能发布、坏材料、部分 v4、双当前
资源失败、内部错误和并发故障注入
```

## 7. 记录和知识交接

施工记录与验证记录沿用第 3 节两份路径。施工记录必须更新函数级代码知识变更清单：本轮没有生产函数变化；测试函数 `形成隔离配置`、`运行Gseed显式激活专项`及 legacy lambda 只改变测试资源准备，不改变生产调用边。

本计划不授权修改共享知识索引；结果发布后只需把测试函数变化和 Gseed 生产函数既有知识清单交给能力证据 / 代码知识库维护者。

## 8. 失败收口

- 三个夹具修复仍不能使 G01—G05 双配置通过：在本计划白名单内修复测试错误；
- 生产代码、ABI 或机器语义漂移：停止并具名退回计划支撑；
- 混合文件 hunk 无法分离、index / 发布租约冲突：保留现场并停止发布；
- B、真实普通启动或故障场景缺夹具：保持 `NOT_RUN`，不得扩大范围。
