# DATA-L1-FINAL-CLOSURE 生产目标测试隔离施工记录 v0.2

日期：2026-09-01

## 1. 计划与 S0

- 计划身份：`DATA-L1-FINAL-CLOSURE`
- 执行版本：`v0.2`
- plan blob：`ebe68c86b9ff71d3b56cff71a71652b314bb7729`
- 设计提交：`d3a13b7e4bd48bad7a06ebd63b423034544cdd84`
- 执行起点：`c80e54b945e43756231079f12f89e7d1859f8ca6`
- 起点状态：`main == origin/main`，ahead / behind `0 / 0`，index 空。
- 具名退回：候选 `63849adaa916ef69123d3e9b7c3c2c43069d593f` 的独立验收记录提交 `63bc332bdadb6620c949101358606065beafc590`，失败点为生产目标仍携带检查专用入口和 9 个端到端测试模块。

S0 确认 `入口.cpp` 与冻结检查器无异主修改。根 `vcxproj` / filters 存在 SELF-GOVERNANCE 异主 WIP，但本计划取得的 9 项删除 hunk 与其余修改不重叠；只向 index 应用相对起点 HEAD 的显式删除补丁，未整文件暂存、清理、覆盖、stash 或 reset 异主现场。

## 2. 实际修改

1. `海中鱼巣/入口.cpp`
   - 删除 `<string_view>`、L1 持久恢复专项 import 和 `--test-l1-persistent-recovery` 分派。
   - `main` 恢复为启动解析、正式顶层运行和退出码映射的薄入口。
2. `海中鱼巣.vcxproj`
   - 只在 index 删除全部 9 个 `端到端测试.*` `ClCompile` 项。
3. `海中鱼巣.vcxproj.filters`
   - 只在 index 删除对应 9 个三行显示块。
4. `tools/check_l1_frozen_surface.py`
   - 新增生产入口注释剥离检查；拒绝有效代码中的任意端到端测试 import 和任意 `--test-*` 字符串开关。
   - 拒绝根工程或 filters 中任意路径含 `端到端测试.` 的 `ClCompile Include`。
   - 三个生产文件缺失或无法读取时同样失败；检查由 `检查冻结面()` 无条件调用，因而继续进入 `check_specs.py --strict`。

未修改 9 个测试源码、L1 核心 / DTO / 服务、冻结 manifest、`check_specs.py`、fixture、4015 / 4070 / 4080 或 v0.1 记录。相对 `63849adaa...`，9 个测试源码的正式 Git 差异为零。

## 3. 精确 index 与验证输入

四个代码文件暂存后，`git write-tree` 形成代码验证树：

```text
6b1c85f72972736a378c502a66f5015b23c42620
```

对应 index blob：

| 文件 | blob |
| --- | --- |
| `海中鱼巣/入口.cpp` | `870fa4500eee2e013e60103ddc5d1a0b24cf299b` |
| `海中鱼巣.vcxproj` | `87a382dcabaea171e740ba0244a786e61ea67586` |
| `海中鱼巣.vcxproj.filters` | `3a00aa6107a5c4adc6c3c038818c65a5341a28f5` |
| `tools/check_l1_frozen_surface.py` | `4b48bd1d04b87261b440f641ec0b7bbec369ad14` |

候选树导出至 `D:\TEMP\海中鱼巣\DATA-L1-FINAL-CLOSURE\v0.2\run-20260901-c\exact-candidate`。生产根工程测试项数为 0；仓库外 driver 临时项目机械替换入口并只增加两个 L1 专项，测试项数为 2。临时项目、driver、补丁、构建物和日志均未进入 Git。

## 4. 代码知识变更清单

### 4.1 直接修改

- `int main(int 参数数量, char* 参数组[])`，`海中鱼巣/入口.cpp`
  - 用途恢复为正式进程薄入口；删除测试 import、测试参数识别和专项调用边。
  - 直接调用边只保留 `解析并验证启动选项`、`运行海中鱼巣`、`映射进程退出码`。
  - 不读写 L1 事实，不取得测试或验收职责；生命周期仍为进程入口单次调用。
  - 既有用途卡绑定旧 blob `870fa450...`，本次结果 blob 恢复为同一值；知识维护者仍应按结果提交复核生产调用语境。
- `tools.check_l1_frozen_surface.检查冻结面() -> list[str]`
  - 新增无条件生产目标测试隔离检查调用；不改变 43 项冻结操作、manifest 或 L1 机器事实。

### 4.2 新增

- `tools.check_l1_frozen_surface.去注释保留字符串(文本: str) -> str`
  - 纯工具函数；删除 C++ 行 / 块注释并保留字符串和字符字面量，供入口开关扫描使用；无外部副作用、事务或并发状态。
- `tools.check_l1_frozen_surface.检查生产目标测试隔离(错误: list[str]) -> None`
  - 读取入口、根工程和 filters，将缺失、读取失败或测试接线命中追加为机器检查错误；不修改生产文件，不解释 L1 业务语义。

### 4.3 退出与非函数变更

- 退出调用边：`main -> 运行L1事实基座持久恢复端到端测试`。
- 无退出公开函数、无新增生产公开函数、无 L1 ABI / DTO / 持久格式变化。
- 根工程与 filters 仅改变编译 / 显示登记，不形成函数知识。
- 结果提交：待包含本记录的候选提交形成后填写于 Git；本清单以该提交中的四个代码 blob 为准。

## 5. 剩余边界

共享工作树中的异主 `vcxproj` / filters 内容按设计保持原字节，因而在候选提交后会相对新 HEAD 表现为包含 9 项测试登记的未提交回流。该内容不在 index、验证树或候选提交中；新的冻结检查器会阻止它被后继候选误提交。DATA-L1 整体状态仍为 `PENDING_ACCEPTANCE`，必须由新候选的独立集成验收 `PASS` 后再治理升级。
