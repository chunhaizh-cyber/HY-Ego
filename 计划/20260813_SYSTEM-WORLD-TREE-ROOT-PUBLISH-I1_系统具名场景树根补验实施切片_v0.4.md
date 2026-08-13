# SYSTEM-WORLD-TREE-ROOT-PUBLISH-I1 系统具名场景树根补验实施切片 v0.4

日期：2026-08-13
状态：可执行（唯一运行状态以 `计划/计划索引.md` 为准）

## 1. 目标

从正式 `3fe1e9ba28e7c758de92e3ede7e584eb6c83912f` 按 CODE-FILE-01 v2.6 修复 I1 补验物理边界：把现有 I1 状态机机械迁入既有普通应用模块所属 `初始化.系统世界树根.inl`，生产 final R1 与生产外脚本访问器在不同 TU 实例化同一源码正文，完成 v0.4 七组动态矩阵及四层证据。v0.3 `.h` 物理合同失效且不得施工。

不得改变公开 ABI、机器语义、provider-only、A1 同实例、启动、阶段 15 或 I2。

## 2. 正式依据与 S0

- `规范/详细设计/20260813_SYSTEM-WORLD-TREE-ROOT-PUBLISH-I1_系统具名场景树根补验单源模板详细设计_v0.4.md`
- CODE-FILE-01 v2.6、规范目录 v4.9。
- I1 v0.2 未被 v0.4 改写的状态机合同；实现 `34b9cbcc`、清理 `6a58962a`。

执行者从本版本正式索引与最新 HEAD 重新核对 plan/design blob、R1/A1/I1 ABI、`.inl` 依赖可见性、index、dirty 所有权及构建资源。专项真实编译若证明 R1 DTO 不可见，具名退回，不得复制 DTO、扩大 export 或手填模块依赖。

## 3. 精确九条白名单

1. `海中鱼巣/业务/初始化.系统世界树根.inl`（新增，永久保留）
2. `海中鱼巣/装配.普通应用.ixx`
3. `海中鱼巣.vcxproj`
4. `海中鱼巣.vcxproj.filters`
5. `验证工具/系统世界树根建立与正式读回发布参数验证.cpp`（恢复并扩展，接受后清理）
6. `验证工具/系统世界树根建立与正式读回发布参数验证.vcxproj`（同上）
7. `验证工具/运行系统世界树根建立与正式读回发布参数验证.ps1`（同上）
8. `施工记录/20260812_SYSTEM-WORLD-TREE-ROOT-PUBLISH-I1_施工记录.md`
9. `验证记录/20260812_SYSTEM-WORLD-TREE-ROOT-PUBLISH-I1_验证记录.md`

DTO、R1/A1/L1、六服务、启动、程序结果、8120、I2、自我、概念、流程图、其它计划 / 设计及异主 WIP 禁止修改。

## 4. 机械实施

1. 从 `34b9cbcc` 精确恢复三件专项。
2. 新建详细设计冻结的 `.inl`；迁入所有状态机正文及其直接逐字段 helper，零 module/export/include/验证逻辑。
3. 普通应用在 imports 与 DTO 后、首次使用前唯一 include；公开 wrapper 只经 A1 取 final R1 并把三个状态引用、请求和服务交模板。
4. 根工程 / filters 各唯一 ClInclude 登记 `.inl`；零 ClCompile、scan、partition、手工依赖。
5. 专项按详细设计精确提供标准库并直接 import L2公共合同、L2场景结构和普通应用三个既有模块，随后唯一 include；脚本 provider、返回队列、调用计数、输出只写专项 `.cpp`。
6. 逐用例输出输入脚本、调用序列、结果六 optional 格、两状态槽前后；完成详细设计第 6—7 节全部矩阵。
7. 永久记录严格按 seam / 真实 R1 / 静态源码 / 构建工程四层分账，列出 PASS / FAIL / 合法 NOT_RUN。

## 5. 验证命令与门禁

```powershell
git diff --check -- <九条白名单>
python .\tools\check_specs.py --strict
msbuild .\海中鱼巣.slnx /t:Rebuild /p:Configuration=Debug /p:Platform=x64
msbuild .\海中鱼巣.slnx /t:Rebuild /p:Configuration=Release /p:Platform=x64
powershell -ExecutionPolicy Bypass -File .\验证工具\运行系统世界树根建立与正式读回发布参数验证.ps1 -配置 Debug
powershell -ExecutionPolicy Bypass -File .\验证工具\运行系统世界树根建立与正式读回发布参数验证.ps1 -配置 Release
```

成功必须同时满足：

- 七组动态强制矩阵双配置逐项 PASS，零聚合坏形状名，零用 `NOT_RUN` 覆盖；
- 真实普通应用 final R1 首次、重复、并发、值式隔离与直接/A1 getter 同址 PASS；
- 固定身份 / 四保留值差异、wrapper A1 引用传递、A1 零写、状态机唯一正文静态 PASS；
- 根工程和含完整真实模块依赖图的专项 Debug / Release fresh 重建、链接、运行 PASS；
- `.inl` 物理合同、工程登记、生产零验证逻辑、公开 ABI / 三路径零 diff、UTF-8/XML、范围及 staged 精确 PASS。

真实分配器耗尽、启动 / 阶段 14—18、恢复、跨进程、长时生产、I2、自我、概念和独立集成验收可 `NOT_RUN`；脚本 `bad_alloc` 只证明异常映射。

## 6. 发布、接受与清理

实现与永久记录形成一份结果提交并普通推送。交互侧按四层证据重新独立核验；接受后由索引所有者登记退出，再以独立提交只删除三件专项并复跑根工程与 strict。`.inl` 是永久生产模板实现片段，不得清理。

## 7. 完成边界

计划可执行只表示 v0.4 合同可机械施工，不证明实现或验证完成。最终结果最多证明 I1 provider，不证明正式启动、I2、自我、概念、恢复或完整系统闭环。
