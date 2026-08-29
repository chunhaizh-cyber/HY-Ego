# INSTINCT-STAGE3-UTC-TIME-EVIDENCE 验证记录

日期：2026-08-29

计划：`INSTINCT-STAGE3-UTC-TIME-EVIDENCE` v0.1

起点：`5b9ad7fa82e3ea4fd4c5eacb3831b6a8f909e89b`

结果提交：本记录与实现候选同提交形成；精确哈希以发布回传为准。

## 1. 环境与入口

- 根工程：`海中鱼巣.vcxproj`，MSVC v143，目标平台 x64。
- 仓库外专项与 ABI consumer：`D:\TEMP\海中鱼巣\INSTINCT-STAGE3-UTC-TIME-EVIDENCE\20260829-01` 及提交树复验目录。
- 专项只 import `海中鱼巣.端到端测试.本能UTC时间证据`；ABI consumer 只 import `海中鱼巣.适配.适配器.UTC时钟` 并消费公开 DTO、版本常量和读取入口。

## 2. 构建与静态门禁

| 门禁 | 当前结果 | 边界 |
| --- | --- | --- |
| 工作树 v143 Debug 根工程 Build | PASS，exit 0 | 证明当前共享工作树候选可编译、链接；不替代精确提交树门禁。 |
| 工作树 Debug / Release UTC 专项 | PASS，exit 0 | 两配置均实际运行 U00—U04。 |
| 精确 index `git diff --cached --check` | PASS | index 恰含 7 个计划文件；三个混合文件只含 UTC import、启动读取替换和两项工程登记。 |
| `python .\tools\check_specs.py --strict` | PASS，116/116 | 只证明规范目录一致，不证明 UTC 业务能力。 |
| 精确提交树 v143 Debug / Release Rebuild | PASS，exit 0 | 从 `HEAD + 精确 index` 的仓库外隔离树从头编译、链接；两配置均 0 warning / 0 error。 |
| 精确提交树 Debug / Release ABI consumer | PASS，exit 0 | 独立 consumer 只 import、构造和调用公开适配器，两配置均成功。 |
| 精确提交树 Debug / Release UTC 专项 | PASS，exit 0 | 隔离提交树两配置均重新编译、链接并实际运行 U00—U04。 |
| 生产静态接线 | PASS | 启动已无 `system_clock::now()` 直接调用，且真实读取 `结果.证据->UTC纳秒`。 |

## 3. 已运行矩阵

| 编号 | 结果 | 机器证据 |
| --- | --- | --- |
| U00 | PASS | 默认请求返回 `已读取`、完整 v1 证据和正 UTC 纳秒。 |
| U01 | PASS | 错误合同版本返回 `请求拒绝`、空载荷、`成功()==false`。 |
| U02 | PASS | 零时间源版本返回 `请求拒绝`、空载荷、`成功()==false`。 |
| U03 | PASS | 未知非零时间源版本返回 `时间源版本漂移`、空载荷、`成功()==false`。 |
| U04 | PASS | 连续两次合法读取均完整；没有要求后值严格大于前值。 |
| 计数不可表示 | NOT_RUN | 当前生产系统时钟计数可表示；没有获授权的时间源注入 ABI，不伪造异常。 |
| UTC 非正 / 底层异常 | NOT_RUN | 当前系统时钟未产生该条件；代码保留结构化防御，运行证据不夸大。 |

Debug 与 Release 专项实际输出均为 U00、U01、U02、U03、U04 全部 `PASS`，进程 exit 0。

## 4. 结论边界

精确 index、strict、隔离提交树双配置 Rebuild、独立 ABI consumer、专项复验和生产静态接线核对均已通过。候选已经证明“正式 UTC 发生时间证据提供者已实现并进入生产启动”，尚待精确提交与普通推送。

即使全部门禁通过，也不证明完整秒维护、A/V 变化、状态/动态、游标推进、合同/安全输入、双根自检或完整阶段三。
