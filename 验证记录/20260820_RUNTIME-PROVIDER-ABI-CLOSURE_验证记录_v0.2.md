# RUNTIME-PROVIDER-ABI-CLOSURE 验证记录 v0.2

日期：2026-08-20

计划：`RUNTIME-PROVIDER-ABI-CLOSURE v0.2`

施工起点：`067b2d3ee06227ee655e46b34b56535db81162a5`

## 1. 构建与运行

| 验证 | 结果 |
| --- | --- |
| Debug x64 fresh Rebuild | PASS，最终 `0 warning / 0 error`。接入端到端辅助函数后首轮因匿名命名空间位于 export namespace 内触发 C2291/C2294；在计划范围内移到非导出命名空间后重构通过。 |
| Release x64 fresh Rebuild | PASS，生成 `x64/Release/海中鱼巣.exe`。 |
| Debug `海中鱼巣.exe --test-治理` | PASS，exit 0；先输出“运行期时间与事实提供者闭环读回通过”，随后原治理一致事实读回全部通过。 |
| Release `海中鱼巣.exe --test-治理` | PASS，exit 0；与 Debug 同一矩阵通过。 |

端到端新增动态覆盖：

- 坏单调时间、完整秒和事实截止请求均返回具名拒绝且零 optional 伪材料；
- 同一适配器连续两次证据完整且第二次纳秒不小于第一次；
- 完整秒实时结果为`已读取`或`无新完整秒`时，观察 / 区间 optional 形状守恒；若有区间则验证`[1, 当前]`和数量；
- 事实截止通过普通应用同一 L2 结构聚合合法取得，成功材料非零且运行代次 / 范围版本回显一致；
- 合法停止消息经现有邮箱进入批次路由，路由返回完整秒材料和完整共享事实截止材料。

## 2. 静态与工程门禁

| 验证 | 结果 |
| --- | --- |
| `python .\tools\check_specs.py --strict` | PASS，116/116。 |
| `git diff --check -- <精确白名单>` | PASS。 |
| 旧 ABI 扫描 | PASS：旧共享头引用、旧合同常量、旧状态枚举、旧纪元 wrapper、旧字段访问和裸当前批次截止均为 0。 |
| 治理时间源扫描 | PASS：运行期治理生产路径的 `steady_clock`只存在于新适配器；D455 采集器的独立采样时钟保持不变。 |
| 工程登记 | PASS：新适配器在 vcxproj / filters 各恰好 1 项；旧共享头均为 0。 |
| MSVC 模块依赖扫描 | PASS：`sources=56, modules=56, failed=0`，循环 0，外部需求 0。工具自身强制把候选 JSON 写入系统临时目录；尝试指定计划隔离的 `D:\TEMP`路径被工具明确拒绝，未据此扩大修改工具范围。 |

## 3. 未验证和声明边界

真实稳态时钟倒退、I64 极限、DATA 资源竞争 / 耗尽、跨进程连续性、并发压力、批次路由最终状态机、T02 最终 ownership 和停止边界、G0/G1 以及系统业务闭环为 `NOT_RUN`。本记录不能升级为完整自我治理循环或系统完成证明。
