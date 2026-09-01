# DATA-L1-FINAL-CLOSURE 生产目标测试隔离验证记录 v0.2

日期：2026-09-01

## 1. 验证对象

- 计划：`DATA-L1-FINAL-CLOSURE v0.2`
- plan blob：`ebe68c86b9ff71d3b56cff71a71652b314bb7729`
- 起点：`c80e54b945e43756231079f12f89e7d1859f8ca6`
- 代码验证 tree：`6b1c85f72972736a378c502a66f5015b23c42620`
- 精确导出根：`D:\TEMP\海中鱼巣\DATA-L1-FINAL-CLOSURE\v0.2\run-20260901-c\exact-candidate`
- 工具链：Visual Studio 2022 MSBuild `18.8.2`，`PlatformToolset=v143`，x64。

所有构建和运行都消费精确 index tree，不消费共享工作树异主 WIP。

## 2. 静态隔离与规范门禁

精确候选树结果：

```text
python tools/check_l1_frozen_surface.py --strict -> PASS
python tools/check_specs.py --strict              -> PASS，115 / 115
生产入口、vcxproj、filters 搜索 端到端测试. / --test- -> 0 命中
git diff --cached --check                         -> PASS
```

cached 代码名单只有：

```text
tools/check_l1_frozen_surface.py
海中鱼巣.vcxproj
海中鱼巣.vcxproj.filters
海中鱼巣/入口.cpp
```

工程 cached diff 精确为 `vcxproj` 删除 9 行、filters 删除 9 个三行块；入口删除 5 行；未带入其它工程 WIP。

## 3. 冻结检查器负例

在仓库外隔离副本执行原 12 类变异，加本次 2 类防回流变异；工程项回流分别对 `vcxproj` 和 filters 各执行一个变体。共 14 类、15 个实际副本全部退出 1：

| 范围 | 结果 |
| --- | --- |
| 额外 / 缺失操作、同名重载、签名、DTO、枚举、合同版本、重导出 callable | 8 / 8 非零 |
| 领域标识、legacy 重复、surface 状态篡改、提前整体 FROZEN | 4 / 4 非零 |
| 入口恢复测试 import、旧 `--test-*` 分支和专项调用 | 非零，命中 2 个隔离错误 |
| 根 `vcxproj` 恢复一个端到端测试项 | 非零，命中 1 个隔离错误 |
| filters 恢复一个端到端测试项 | 非零，命中 1 个隔离错误 |

负例完成后精确候选树冻结检查和严格规范检查再次退出 0；正式源码未被负例修改。

## 4. 正式生产构建与旧开关

| 配置 | fresh Rebuild | 警告 / 错误 | 日志 SHA-256 |
| --- | --- | --- | --- |
| Debug x64 | PASS | `0 / 0` | `28BE6E51134360C4B6D7E235EC812C5BD5EB33F414E522A8171DC2852135424B` |
| Release x64 | PASS | `0 / 0` | `743F4CEF6A74442C3BB8E72A99EA27271AE8D9D943D30AFB64A643FD950EFDC1` |

Debug 与 Release 的正式 `海中鱼巣.exe --test-l1-persistent-recovery` 均退出 2，输出为空，且不含 L1 专项、persistent recovery 或统一 driver PASS 文本。旧字符串已经沿正式启动参数解析拒绝，不再进入测试函数。

## 5. 仓库外统一 driver

临时项目由生产项目副本机械形成：生产测试项数 0，临时项目测试项数恰为 2，正式入口替换恰为 1，原生产入口项为 0。临时项目不使用 filters。

| 配置 | fresh Rebuild | 警告 / 错误 | 日志 SHA-256 | 运行 |
| --- | --- | --- | --- | --- |
| Debug x64 | PASS | `0 / 0` | `DB7C306F25377AF13EB90C59612C90C8F6B3262575D9C887512FC97FF8042A50` | 退出 0 |
| Release x64 | PASS | `0 / 0` | `6BD557BD977590EF0AAD5273982F6C5CF4440917ABBADEA9FB711E25D531EB02` | 退出 0 |

两配置运行均实际输出：

```text
[L1持久恢复测试] 三类事实、当前/历史/墓碑、组/一致投影、幂等与 fail-closed 通过
[DATA-L1-N-ATOMIC][V01] ... [V08] PASS
[DATA-L1-N-ATOMIC] 可运行矩阵全部通过
```

这复验了节点 / 关系 / 值当前、历史与墓碑、属性槽、owner、事实代次、幂等、一致投影、v1 / v2 / v3、载荷格式 1 / 2 / 3 和既有 fail-closed 矩阵。9 个测试源码相对首轮候选 `63849adaa...` 的正式 Git 差异为零。

## 6. 未运行与结论边界

崩溃、断电、真实介质故障、资源耗尽和长时运行仍为 `NOT_RUN`；本轮不新增相应可靠性声明。

本轮已证明：生产目标移除检查专用入口和全部端到端测试工程项；永久防回流门禁、正式双配置构建和仓库外 DATA-L1 统一公开 driver 在同一精确代码 tree 通过。候选提交与推送尚需完成；完成后仍只形成待独立验收候选，DATA-L1 整体保持 `PENDING_ACCEPTANCE`。
