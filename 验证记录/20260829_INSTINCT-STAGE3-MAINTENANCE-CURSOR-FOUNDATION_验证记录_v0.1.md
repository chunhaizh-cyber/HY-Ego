# INSTINCT-STAGE3-MAINTENANCE-CURSOR-FOUNDATION 验证记录

日期：2026-08-29

计划：`INSTINCT-STAGE3-MAINTENANCE-CURSOR-FOUNDATION` v0.1

起点：`dd318c59f689479a3ae2bbd395b878094a0384c6`

结果提交：本记录与实现候选同提交形成；精确哈希以发布回传为准。

## 1. 环境与入口

- 根工程：`海中鱼巣.vcxproj`，Visual Studio 2022 Professional，MSVC v143 / 14.44.35207。
- Debug / Release 工程输出：`x64\Debug`、`x64\Release`。
- 仓库外专项与 ABI consumer：`D:\TEMP\海中鱼巣\INSTINCT-STAGE3-MAINTENANCE-CURSOR-FOUNDATION\runner`。
- 专项只 import `海中鱼巣.端到端测试.本能被动维护游标`；ABI consumer 只 import `海中鱼巣.领域.服务.本能被动维护游标` 并静态核对两个公开成员函数签名和合同版本。

## 2. 构建与静态门禁

| 门禁 | 结果 | 边界 |
| --- | --- | --- |
| v143 Debug 根工程 Rebuild | PASS，exit 0 | 工作树候选与精确 index 展开的提交树均编译、链接成功；提交树 0 warning / 0 error。 |
| v143 Release 根工程 Rebuild | PASS，exit 0 | 工作树候选与精确 index 展开的提交树均编译、链接成功；提交树 0 warning / 0 error。 |
| Debug / Release ABI consumer 编译、链接、运行 | PASS，exit 0 | 独立 consumer 在工作树与精确 index 提交树上均可消费公开类型和两个入口。 |
| Debug / Release 专项 runner | PASS，exit 0 | 精确 index 提交树两配置均重新编译、链接并调用公开专项入口。 |
| `git diff --check -- <本计划白名单>` | PASS | 只核对本计划候选；仅有既有 LF→CRLF 提示。 |
| `python .\tools\check_specs.py --strict` | PASS，116/116 | 证明规范目录一致，不证明代码业务闭环。 |
| `git diff --cached --check` | PASS | index 恰含本计划 9 文件；四个混合文件只含游标 hunk。 |

## 3. 运行矩阵

| 编号 | 结果 | 机器证据 |
| --- | --- | --- |
| C00 | PASS | 当前槽尚未形成时正式读取返回 `未建立`，空载荷、截止 0。 |
| C01 | PASS | 首次固定登记、边界 0 初始游标与独立当前读回。 |
| C02 | PASS | 同键同义重放和异键同义请求均返回 `已恢复`，形成代次不变。 |
| C03 | PASS | 同键异义返回 `幂等冲突`；同运行代次/纪元边界变化返回 `引用冲突`。 |
| C04 | PASS | 只变化运行代次、只变化纪元、时间源版本漂移、规则版本漂移均 fail-closed。 |
| C04-E | PASS | 请求 G0 与真实当前代次不一致返回 `当前性漂移`。 |
| C05 | PASS | 运行代次与纪元共同变化形成 `已建立新纪元`；新当前形成代次晚于旧游标。 |
| C06 | PASS | 销毁首个完整普通应用上下文后，从同一隔离持久根恢复 owner 和本能根锚点，建立新纪元并独立读回。 |
| 资源 / 内部故障 | NOT_RUN | 没有获授权的 L1 生产故障注入口，不使用假异常或私有出口。 |
| 提交后读回失败 | NOT_RUN | 没有本计划白名单内的正式故障 fixture；代码分支保留 `已可能发布` 和原请求重放边界。 |
| 并发竞态 | NOT_RUN | 服务用递归互斥保护定位缓存和两公开入口；没有专属并发调度 fixture。 |

Debug 与 Release 专项实际输出均为 C00、C01、C02、C03、C04、C04-E、C05、C06 全部 `PASS`。

提交树首轮专项在 S0 暴露测试配置遗漏现行 `任务筹办等待合同登记`，证明原测试候选隐式依赖共享工作树中未发布的配置删减；随后只在本计划 staged 测试投影中恢复 `形成普通应用任务筹办等待合同登记()`，重新展开第二棵 index 提交树并从头完成双配置 Rebuild、ABI consumer 和 C00—C06，全部通过。生产代码未因此扩大或消费异主 WIP。

## 4. 结论边界

精确 index 候选已经证明公开游标 ABI、普通应用唯一装配、启动静态接线、双配置构建、同义恢复、新纪元切换、结构化拒绝和进程级持久恢复；精确 index、cached diff-check 和提交树复验均已完成，尚待提交与普通推送。

该结论最多升级为“本能被动维护初始时间游标闭环已经实现并进入生产启动链”；不证明同纪元维护推进、安全值/服务值变化、状态/动态、合同/事件消费、双根自检或完整本能路线。
