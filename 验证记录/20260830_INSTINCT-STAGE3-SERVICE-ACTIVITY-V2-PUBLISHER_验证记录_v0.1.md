# INSTINCT-STAGE3-SERVICE-ACTIVITY-V2-PUBLISHER 验证记录 v0.1

日期：2026-08-30

计划身份：`INSTINCT-STAGE3-SERVICE-ACTIVITY-V2-PUBLISHER`

候选基线：`2739d85387e3014f85b4428d43ec458e301ba90b` 加本计划五文件候选。

隔离根：`D:\TEMP\海中鱼巣\INSTINCT-STAGE3-SERVICE-ACTIVITY-V2-PUBLISHER\probe-20260830-a`

正式工具链：Visual Studio 2022 Professional 17.14，`PlatformToolset=v143`，x64。

## 1. 构建

| 项目 | 结果 | 证据边界 |
| --- | --- | --- |
| Debug v143 根工程 Rebuild，验证宏开启 | PASS | 隔离候选根；全工程模块扫描、编译和链接成功 |
| Release v143 根工程 Rebuild，验证宏开启 | PASS | 隔离候选根；`/GL` 生产优化配置成功 |
| 修复宏边界后 Debug / Release v143 增量 Build | PASS / PASS | 重新编译目标服务和专项并链接根工程 |
| Release v143 根工程 Rebuild，验证宏关闭 | PASS | 输出 `production-no-validation/bin/海中鱼巣.exe`；证明专项宏边界及生产配置可编译 |
| Release v143 根工程增量 Build，验证宏关闭 | PASS | 最终生产配置再次编译目标服务/专项并链接成功 |

验证宏关闭的首次门禁在修复前曾因专项 helper 直接引用 `ARCH_建立验证样本_v1` 失败。白名单测试文件增加条件编译边界后，完整 Rebuild 和最终 Build 均通过；该失败未被当作最终通过证据。

Visual Studio 2026 Community v145 曾在非白名单既有文件 `海中鱼巣/领域/内部治理/服务.需求初次筹办准备.ixx:251` 触发 C1001。计划要求 v143，本记录不修改该异主文件，也不把 v145 结果计入正式失败或通过。

## 2. 专项运行矩阵

仓库外 runner 分别链接 Debug 对象和 Release `/GL + /LTCG` 对象。最终运行：

```text
publisher-tests-v143-debug.exe   -> 0
publisher-tests-v143-release.exe -> 0
```

两配置均通过：

- P00—P24：既有 v1 合同、事件、进展、准备、失败和恢复回归；
- V00—V04（含 V02A/V02B）：既有 v2 登记、完整集合、坏绑定、重复成员、版本和 v1 零回落；
- W00：进展首次发布、同流替代、旧请求历史重放、外部端点保留、同键/跨入口冲突；
- W01：准备首次发布、完成替代、历史重放、需求来源与能力缺口来源；
- W02：进展八状态、准备九状态、状态 only / 动态 only / 两者；
- W03：坏材料、缺/错预期当前、合同-需求冲突、坏外部引用、旧 G0、资源失败；
- W04：提交后读回失败返回 `已可能发布`，原请求重试精确收束；
- W05：持久根释放/恢复后两个发布入口均精确重放；
- Q00—Q06：单完整秒时间裁决回归。

## 3. 独立 ABI consumer

仓库外 `abi-consumer.cpp` 仅导入公开模块 `海中鱼巣.领域.服务.服务合同事实权威`，在独立翻译单元中：

- 构造两个公开请求类型；
- 调用 `发布服务进展事实_v2` 与 `发布服务准备事实_v2`；
- 读取状态、事实、首次提交代次和正式读回截止；
- 调用两个结果的 `成功()`。

结果：

| 配置 | 编译 / 链接 | 运行 |
| --- | --- | --- |
| Debug v143 | PASS | `abi-consumer-v143-debug.exe -> 0` |
| Release v143 `/GL + /LTCG` | PASS | `abi-consumer-v143-release.exe -> 0` |

## 4. 生产对象验证符号隔离

对验证宏关闭后形成的 Release 生产对象执行 `dumpbin /symbols`：

```text
ARCH_注入发布后读回失败一次  -> 0 hits
ARCH_发布后读回失败一次_     -> 0 hits
```

正控：验证宏开启的 Debug 专项对象对 `ARCH_注入发布后读回失败一次` 为 `1 hit`，证明扫描能够识别验证专属入口。

## 5. 静态门禁与禁止范围

| 门禁 | 结果 |
| --- | --- |
| `python .\tools\check_specs.py --strict` | PASS，116/116 |
| 全工作区 `git diff --check` | PASS；仅报告既有 LF/CRLF 提示 |
| 三份目标源码 `git diff --check -- <paths>` | PASS |

生产 L/H 和安全根定义相关路径相对 HEAD 零差异，已核对的 index blob：

```text
海中鱼巣/业务/安全根生产定义初始化.数据.h              44e936d69ba611a5c04580054bb3267f0154b295
海中鱼巣/业务/提供者.安全根生产定义初始化.ixx          fba264f23b58ed9cb30d88b2ed494e73f8ea5d24
海中鱼巣/领域/安全根定义与当前值.数据.h                db19ef831f8bf06880002fcef4de8fbc714bef2e
海中鱼巣/领域/服务.安全根定义与当前值.ixx              de0d4ecf46cc3df2aa9f3681ec0708de2fa702f0
海中鱼巣/端到端测试.安全根生产定义初始化.ixx           447a94c9d255d1eb4fd61c758ed0c0f9ab324241
```

当前定义仍为版本 1、值域 `0..I64_MAX`、`L=2,767,011,611,056,432,742`、`H=7,378,697,629,483,820,645`；本切片没有运行期自动调整逻辑。

普通应用、线程、工程文件和安全治理存在 S0 前已有异主 WIP，因此不能声明全工作区干净；本计划未修改或暂存这些路径，只精确发布五文件白名单。

## 6. 未验证与完成边界

- 真实方法执行 / 服务准备生产者调用两个新入口：`NOT_RUN`，生产调用尚未接线；
- 同 G0 安全门禁 provider：`NOT_RUN`，属于后继切片；
- 真实生产规则宇宙、有效服务活动、阶段三和 `INSTINCT-ROUTE` 完成：`NOT_PROVEN`；
- v145 C1001 所在异主内部治理文件：未修复、未纳入本计划。

本记录证明的是服务 owner 的 v2 正式写读闭环及其专属 ABI/回归矩阵，不把编译、专项、日志或空集合升级为真实生产接线和整体业务完成。
