# INSTINCT-STAGE3-UTC-TIME-EVIDENCE 本能阶段三绝对 UTC 时间证据代码实施计划 v0.1

日期：2026-08-29

版本：v0.1

状态：创建侧候选；发布并登记后才可执行

## 1. 目标与完成条件

实现公开强类型 UTC 时间证据适配器，用它替代生产启动中直接读取 `system_clock` 的本能根首次状态时间来源，并以专项、ABI consumer 和双配置构建证明真实可用。

完成不包括完整秒推进、维护游标推进、A/V、L/H、合同、安全事件、状态 / 动态或双根自检。

## 2. 正式依据与当前事实

- 详细设计：`规范/详细设计/20260829_INSTINCT-STAGE3-UTC-TIME-EVIDENCE_本能阶段三绝对UTC时间证据详细设计_v0.1.md`。
- 上游结果：本能根运行锚点、阶段二当前值变化记录、完整秒时钟和维护游标均已发布。
- 当前生产启动仍直接调用 `std::chrono::system_clock::now()`；现有单调时钟只提供同纪元经过时间，不能替代绝对 UTC。
- 当前工作树含 SELF-GOVERNANCE 异主 WIP；只允许提交本计划精确增量。

## 3. 精确白名单

```text
海中鱼巣/适配/适配器.UTC时钟.ixx
海中鱼巣/启动.应用程序.ixx
海中鱼巣/端到端测试.本能UTC时间证据.ixx
海中鱼巣.vcxproj
海中鱼巣.vcxproj.filters
施工记录/20260829_INSTINCT-STAGE3-UTC-TIME-EVIDENCE_施工记录_v0.1.md
验证记录/20260829_INSTINCT-STAGE3-UTC-TIME-EVIDENCE_验证记录_v0.1.md
```

启动、工程和 filters 是混合 WIP 文件，只暂存本计划 hunk。禁止修改或带入其它代码、规范、流程图、计划和测试。

## 4. 实施步骤

### S0 当前事实与复用裁决

核对计划 blob、main / HEAD / origin / index、白名单 dirty、工具链和当前 `system_clock` 调用。保存三个混合文件的计划前 patch。确认不存在等价公开 UTC 时间证据入口。

### S1 UTC 适配器

新增设计冻结的 v1 DTO、状态、成功谓词和 `UTC时钟适配器::读取当前UTC时间证据_v1(...)`。只用 `system_clock` 形成 Unix epoch 纳秒；错误分账和空载荷严格按设计。

### S2 生产启动消费

启动 import 新模块，用完整证据替换本能根初始化前的裸 `system_clock` 调用。失败保持现有初始化失败阶段；不改变运行代次、单调时钟、完整秒和游标合同。

### S3 专项与记录

新增 U00—U07 专项。施工记录列出函数级知识变更清单；验证记录分账静态、编译、专项、ABI、提交树和 NOT_RUN。

### S4 精确发布

对混合文件构造 `HEAD + 本计划 hunk` 的 index blob；运行 cached diff-check、strict、Debug / Release Rebuild、两配置 ABI consumer 和专项。提交树从头复验后普通推送。

## 5. 失败收口

- 合同或当前代码发生实质漂移：只停止本切片。
- UTC 值不可表示或非正：结构化非成功，不补造时间。
- 编译 / 实现错误：只在白名单内修复。
- 混合 WIP 重叠：保留工作树，以 HEAD 和本计划 patch 构造 index，不覆盖异主内容。
- 任何测试不得要求 UTC 严格递增，也不得用等待 / sleep 制造正确性。

## 6. 验证命令与成功条件

```powershell
git diff --check -- <精确白名单>
python .\tools\check_specs.py --strict
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v143
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v143
```

另运行 Debug / Release ABI consumer 和专项 U00—U07。成功要求：两配置编译、链接和运行均为 0；生产启动真实消费强类型证据；index 只含白名单增量；提交树复验和普通推送完成。

## 7. 完成声明边界

完成最多声明“正式 UTC 发生时间证据提供者已实现并进入生产启动”。不得声明完整秒维护、A/V 变化、状态 / 动态、游标推进或阶段三完成。
