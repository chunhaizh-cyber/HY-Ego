# INSTINCT-STAGE3-SAFETY-ROOT-DEFINITION-CURRENT-PROVIDER 安全根定义与当前值提供者代码实施计划 v0.1

日期：2026-08-29

版本：v0.1

状态：已退出；结果提交 `2fdcc4bf598922d273108bd455ef59a26d7c63b1`

## 1. 目标与完成条件

实现安全根定义专属 owner、版本化发布 / 当前读回、与现有安全根当前值 `A` 的同截止组合读取，并由普通应用唯一装配。完成要求真实合法消费者可取得服务，显式 fixture 可发布并独立读回；装配后不得自动发布默认 `L/H`。

完成不包括具体生产阈值选择、启动发布者、安全事件结算、服务被动回归、状态 / 动态变化、控制态、调度配额、FRESH 或双根自检。

## 2. 正式依据和当前事实

- 详细设计：`规范/详细设计/20260829_INSTINCT-STAGE3-SAFETY-ROOT-DEFINITION-CURRENT-PROVIDER_安全根定义与当前值提供者详细设计_v0.1.md`。
- 直接上游：本能根锚点、阶段二统一当前值变化入口、绝对 UTC 时间证据、完整秒和被动维护游标均已发布。
- 当前 `L/H` 无正式生产数值，运行期只读查询的向量位置猜测是占位，禁止复用。
- 当前 L1 owner-scoped CRUD、游标 owner 和 `L2特征结构服务::读取特征当前值` 提供所需机械能力。
- 当前工作树含大量 SELF-GOVERNANCE 异主 WIP；只允许提交本计划精确增量。

## 3. 精确白名单

```text
海中鱼巣/领域/安全根定义与当前值.数据.h
海中鱼巣/领域/服务.安全根定义与当前值.ixx
海中鱼巣/装配.普通应用.ixx
海中鱼巣/端到端测试.安全根定义与当前值.ixx
海中鱼巣.vcxproj
海中鱼巣.vcxproj.filters
施工记录/20260829_INSTINCT-STAGE3-SAFETY-ROOT-DEFINITION-CURRENT-PROVIDER_施工记录_v0.1.md
验证记录/20260829_INSTINCT-STAGE3-SAFETY-ROOT-DEFINITION-CURRENT-PROVIDER_验证记录_v0.1.md
```

普通应用、工程和 filters 仅暂存本计划 hunk。禁止修改或带入 L1、特征 / 状态 / 动态、游标、启动、FRESH、安全硬否决、规范、流程图及 SELF-GOVERNANCE WIP。

## 4. 实施步骤

### S0 当前事实和所有权

重新核对计划 blob、索引状态、HEAD / origin / index、白名单 dirty、目标常量零命中、现有 owner / 装配 / 当前值 ABI。保存三个混合文件的计划前 patch。

### S1 数据和 owner 服务

按详细设计新增 v1 事实、请求、结果、固定状态数值和成功谓词。实现 owner 交付、结构登记、规范化编码 / 解码、幂等重放、连续版本发布、当前定义独立读回和已可能发布见证。

### S2 同截止组合读取和装配

调用现有 `读取特征当前值` 形成 `定义 + 当前值事实 + A + G0` 快照。普通应用建立唯一 owner / 服务并提供 getter；不得调用发布入口。

### S3 专项、ABI 和记录

实现 S00—S12；新增两份记录。施工记录列出函数级知识变更清单，验证记录分账静态、构建、专项、ABI、恢复、提交树和 NOT_RUN。

### S4 精确发布

混合文件按 `HEAD + 本计划 hunk` 构造 index。执行 cached diff-check、strict、Debug / Release 根工程、两配置 ABI consumer 和专项；从最终 index tree 复验后提交并普通推送。

## 5. 失败收口

- 合同、owner、当前值 ABI 或常量发生实质漂移：只停止本切片并修订设计。
- 无定义：结构化“未发布”，不自动发布默认值。
- 非连续版本、坏锚点、坏当前值：结构化拒绝且零部分载荷。
- 交换后读回失败：只返回已可能发布见证，不补写第二次。
- 代码 / 编译错误：只在白名单内修复。
- 混合 WIP：保留工作树，以 HEAD 和本计划 patch 构造 index，不覆盖异主内容。

## 6. 验证命令和成功条件

```powershell
git diff --check -- <精确白名单>
python .\tools\check_specs.py --strict
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v143
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v143
```

另运行 Debug / Release ABI consumer 和 S00—S12 专项。成功要求：两配置编译、链接和运行均为 0；合法普通应用消费者可取得唯一服务；显式发布与独立读回成功；空装配保持未发布；提交树只含本计划增量并普通推送。

## 7. 完成声明边界

完成最多声明“安全根定义及其与当前值的同截止正式提供者已实现”。不得声明具体 `L/H` 已生产发布、安全被动维护或阶段三已完成。
