# INSTINCT-BOOTSTRAP-ROOT-ANCHOR 验证记录 v0.1

日期：2026-08-28

计划身份：`INSTINCT-BOOTSTRAP-ROOT-ANCHOR`

代码起点：`518d7b54b44caaf1fa6a0ea3b9bfbe6281490604`

验证候选：`D:\TEMP\海中鱼巣\INSTINCT-ROOT-ANCHOR\candidate-final-01`

## 1. 验证边界

本轮按计划只执行：

```text
正式规范目录 strict
+ 精确候选差异检查
+ 静态不变量扫描
+ VS 2022 v143 Debug/Release x64 根工程隔离 Rebuild
```

用户已要求业务测试留到实际使用阶段，因此 18 项外部业务验收全部为 `NOT_RUN`。
构建 PASS 不替代运行、重启、幂等、故障或业务结果验收。

## 2. 文档与静态门禁

| 检查 | 结果 | 证据 |
| --- | --- | --- |
| `python .\tools\check_specs.py --strict` | PASS | `116` 份目录项全部通过 |
| 起点 HEAD 与精确候选 `diff --check` 等价检查 | PASS | 无空白错误；仅曾出现换行转换提示，关闭临时 autocrlf 诊断后无输出 |
| 固定身份表 | PASS | 安全 `0x494E'5354'0101'0001..000A`、服务 `0x494E'5354'0102'0001..000A` 均为 constexpr 字面量 |
| 跨用途身份复用 | PASS | 两组十项逐槽互异；共享 M-X 派生规则身份独立于幂等身份 |
| 来源稳定编码 | PASS | 定义、实例、首值首态、比较、合同和需求逐层取正式上游身份，不由运行期哈希或名称派生 |
| 首次形成动态写 | PASS | provider 动态写调用 `0`，动态服务 import `0` |
| 旧 `新增状态_v2` | PASS | provider 和启动调用均为 `0` |
| 本能根初始化生产调用 | PASS | `启动.应用程序.ixx` 恰一处调用 |
| 自我线程生产创建调用 | PASS | 排除专属端到端测试文件后，生产树恰一处调用，位于启动文件 |
| 锚点交付 | PASS | 调用前恰一处 `线程请求.本能根锚点 = *本能根.锚点` |
| 治理运行门开放 | PASS | 启动成功链恰一处开放；初始化失败在创建前返回阶段 21 |
| 工程登记 | PASS | project 和 filters 各只增加数据头与 provider 两条对应登记 |
| 专项/测试污染 | PASS | 本计划增量无生产专项、参数、测试宏、故障注入或验收运行器 |

`L2特征结构.数据.h` 的候选与 HEAD 只有换行表示差异，按 Git 归一化无语义差异，
因此不属于结果代码增量。

## 3. 根工程构建

工具链由计划冻结为 Visual Studio 2022 `v143`。根工程从隔离候选执行，IntDir/OutDir
均位于仓库外：

```powershell
$验证根 = 'D:\TEMP\海中鱼巣\INSTINCT-ROOT-ANCHOR'

& $msbuild .\海中鱼巣.vcxproj /m /t:Rebuild `
    /p:Configuration=Debug /p:Platform=x64 /p:PlatformToolset=v143 `
    /p:IntDir="$验证根\Debug\obj\" `
    /p:OutDir="$验证根\Debug\bin\"

& $msbuild .\海中鱼巣.vcxproj /m /t:Rebuild `
    /p:Configuration=Release /p:Platform=x64 /p:PlatformToolset=v143 `
    /p:IntDir="$验证根\Release\obj\" `
    /p:OutDir="$验证根\Release\bin\"
```

| 配置 | 结果 | 警告/错误 | 用时 | 产物 |
| --- | --- | --- | --- | --- |
| Debug x64 Rebuild | PASS | 0 / 0 | 56.61 秒 | `D:\TEMP\海中鱼巣\INSTINCT-ROOT-ANCHOR\Debug\bin\海中鱼巣.exe` |
| Release x64 Rebuild | PASS | 0 / 0 | 53.74 秒 | `D:\TEMP\海中鱼巣\INSTINCT-ROOT-ANCHOR\Release\bin\海中鱼巣.exe` |

产物复核：Debug EXE `15,149,056` 字节，Release EXE `2,068,992` 字节。

## 4. 早期失败与修复记录

| 轮次 | 结果 | 根因 | 修复 |
| --- | --- | --- | --- |
| 早期候选 1 | FAIL | 类作用域内 namespace alias 不符合 MSVC 语法 | alias 移到 namespace 作用域 |
| 早期候选 2 | FAIL | 调用了不存在的实例读取 `成功()` 和不存在的二参数状态完整性重载 | 改查正式结果头，并显式核对生命周期 |
| 最终 Debug | PASS | 上述编译问题已闭合 | 0 警告、0 错误 |
| 最终 Release | PASS | 与 Debug 同一候选 | 0 警告、0 错误 |

VS 2026 `v145` 对同一候选的非白名单既有模块触发 MSVC `C1001`。本计划没有接管或
修改该模块，正式门禁按计划冻结为 `v143`；因此 `v145` 仍是未证明边界，不写为 PASS。

## 5. 外部业务验收矩阵

| 编号 | 用例 | 本轮结果 | 原因 |
| --- | --- | --- | --- |
| 01 | 空库首次形成 | NOT_RUN | 留待实际使用阶段 |
| 02 | 同进程相同请求零新事实 | NOT_RUN | 留待实际使用阶段 |
| 03 | 进程重启恢复同一稳定身份 | NOT_RUN | 留待实际使用阶段 |
| 04 | 当前值变化后重启不重置 | NOT_RUN | 留待实际使用阶段 |
| 05 | 下中点与余数舍弃 | NOT_RUN | 仅完成常量静态核对 |
| 06 | 两根比较注册与目标合同逐字段一致 | NOT_RUN | 仅完成代码静态核对 |
| 07 | 首值、初态、选择同 G1 | NOT_RUN | 留待权威读回验收 |
| 08 | 首次动态新增量为 0 | NOT_RUN | 仅证明调用边为 0 |
| 09 | 目标合同后故障幂等恢复 | NOT_RUN | 未执行故障场景 |
| 10 | 列表项后故障恢复根需求 | NOT_RUN | 未执行故障场景 |
| 11 | 根实例 0 项/多项不任选 | NOT_RUN | 未执行数据场景 |
| 12 | 安全/服务角色交叉拒绝 | NOT_RUN | 未执行数据场景 |
| 13 | 原子提交已可能发布收敛 | NOT_RUN | 未执行故障与重放 |
| 14 | final read 漂移空锚点 | NOT_RUN | 未执行并发漂移 |
| 15 | 二次概念零实例/值/状态/动态 | NOT_RUN | 仅完成代码静态核对 |
| 16 | 二次值 0、M/2、M 和负值边界 | NOT_RUN | 未运行纯值调用者 |
| 17 | 初始化失败零线程创建/开门 | NOT_RUN | 仅完成控制流静态核对 |
| 18 | Debug/Release 业务结果逐项一致 | NOT_RUN | 两配置只构建，未运行业务验收 |

## 6. 证据分账

```text
静态合同和调用边：PASS
Debug/Release 根工程编译闭包：PASS
生产程序运行：NOT_RUN
业务矩阵：NOT_RUN 18/18
跨进程恢复：NOT_RUN
故障注入与已可能发布收敛：NOT_RUN
集成验收：NOT_RUN
```

## 7. 结论边界

候选满足本轮计划要求的静态和双配置构建门禁，可以进入精确代码发布。该结论只证明
公开能力已编译并接入启动/停门线程创建路径，不证明业务运行、重启恢复、故障收敛、
根需求展开、任务、方法、结果结算或整体自我治理闭环已经通过验收。
