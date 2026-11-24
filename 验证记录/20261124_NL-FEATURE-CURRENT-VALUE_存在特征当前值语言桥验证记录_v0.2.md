# 存在—特征—当前值语言桥 验证记录 v0.2

- 日期：2026-11-24
- 切片：`NL-FEATURE-CURRENT-VALUE` t171u
- 分支：`cursor/nl-feature-current-value-c720`
- 相对 v0.1 head：`e8aa2f1d…`

## 1. 同步证据

| 项 | 值 |
| --- | --- |
| `git fetch origin cursor/nl-feature-current-value-c720 main` | 成功 |
| `origin/main` | `f65fd474f8a3b43294277754eb900cb0163e46f0` |
| 远端分支 tip（开工） | `e8aa2f1dec2a0f37889615edb84cd0ee9ea8144d` |
| 破坏性操作 | 无（保留 WIP、不强推、不自动合并） |

## 2. 静态检查（提交前）

```text
git diff --check
```

## 3. 三层验证划分

| 层 | 内容 | 本云执行 | 预期取证 |
| --- | --- | --- | --- |
| L1 编译 | `海中鱼巣.vcxproj` Release\|x64 | 本 Linux 无 MSVC；已加 GitHub Actions `cloud-nl-feature-current-value.yml`（`windows-2025-vs2026`） | CI artifact `msbuild.log` |
| L2 单测/装配自检 | `--verify-nl-feature-current-value`（正式装配上下文） | 同左，随 CI 跑 | `verify-summary.txt` / `verify-stdout.txt` / 应用日志 |
| L3 正式数据端到端 | 用户机器未提交 WIP / 生产持久库 | **未跑**（禁止操作）；合成夹具仅 L2 并标注 | 用户独有 Windows 正式数据待验清单见 §5 |

### L2 用例覆盖（`运行特征当前值语言桥自检`）

| 标识 | 材料性质 | 期望 |
| --- | --- | --- |
| `读.输入拒绝` | 正式 API | 空编码 → 输入拒绝 |
| `读.特征未持有` | 正式装配对象 | 假特征 → 未持有/不可读 |
| `闭环.I64陈述回读` | 正式装配对象（若有 I64 有值特征） | 往返语义一致；否则合成跳过 |
| `闭环.概念条件陈述回读` | 正式装配对象（若有） | 保留概念节点身份往返 |
| `引用.存在节点陈述边界` | 正式装配对象（若有） | 陈述 `能力未支持` |
| `填项.未知角色` | 正式装配对象 | 缺规则如实返回/可登记缺口；不伪造成功问句 |
| `学习.新模式命中` | **合成夹具**（克隆 `SC-正式字段-填项问` 模式标识） | 登记后生成查询命中 |
| `纠正.旧模式过滤` | **合成夹具** | 纠正后旧模式不再生效 |
| `理解.空文拒绝` | 正式 API | 空文输入拒绝 |

## 4. 本环境已执行命令

```text
git fetch origin cursor/nl-feature-current-value-c720 main
git rev-parse HEAD origin/cursor/nl-feature-current-value-c720 origin/main
git diff --check
# 推送后由 Actions 执行：
# tools/ci/windows-nl-feature-current-value.ps1
```

本 Linux agent：**未**本地 MSVC 编译；**未**声称 L1/L2 已绿。通过/失败数以 CI 运行结果与 artifact 为准（本记录在推送瞬间写入，CI 结果后补）。

## 5. 用户独有 Windows 正式数据待验（若需 L3）

1. 用生产/本机正式库启动：`海中鱼巣.exe --verify-nl-feature-current-value`
2. 另开普通会话：对已知存在发 I64 值陈述与缺值填项，核对接收认知原因串含「领域核对」且世界当前值未被理解写回
3. 提供日志片段与存在/特征类型身份（勿提交未授权持久库导出）

## 6. CI 首跑失败与修复（bc27125c）

| 项 | 值 |
| --- | --- |
| 失败 job | `msvc-nl-feature-current-value`（push + pull_request 各一，同因） |
| 失败层 | **L1 编译**（未进入 L2 verify） |
| 原因 | `自检.特征当前值语言桥.cpp` 使用了不存在的枚举成员 `特征当前值语言桥状态::引用不可读`（MSVC C2838/C2065，连锁 C2737） |
| 修复 | 改为既有状态：`存在不可读` / `特征不可读` / `能力未支持` |
| 正式库 L3 | 仍未跑 |

## 7. 结论边界

- 本轮交付：可编译登记的双向闭环实现 + 生产核对入口 + 自检旗标 + CI 工作流。
- **不**自动合并 PR#18；**不**把合成学习夹具称为正式业务数据；**不**把 CI 未完成写成已通过。
