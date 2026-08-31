# INSTINCT-STAGE3-SERVICE-HISTORY-BOUNDARY-SNAPSHOT-V2 验证记录 v0.1

日期：2026-08-31

计划：`INSTINCT-STAGE3-SERVICE-HISTORY-BOUNDARY-SNAPSHOT-V2 v0.2`

验证基线：工作区起点 `a063b7e2cd0f5a3603a98b562546d473490f4a48` 加本计划五文件候选；结果提交由本记录所在发布提交绑定。

隔离根：

```text
D:\TEMP\海中鱼巣\INSTINCT-STAGE3-SERVICE-HISTORY-BOUNDARY-SNAPSHOT-V2\20260831-round3
```

## 1. 结果

| 验证项 | 结果 | 实际证据 |
| --- | --- | --- |
| Debug x64 根工程，validation 宏启用 | PASS | v143 `Build`，完整链接成功 |
| Release x64 `/GL + /LTCG` 根工程，validation 宏启用 | PASS | v143 `Build`，完整链接和代码生成成功 |
| Debug 专项 | PASS | P00—P24、V00—V04、H-1/H00—H06、B00—B10、W00—W05、Q00—Q06 全通过 |
| Release `/GL + /LTCG` 专项 | PASS | 同一完整矩阵全通过 |
| 无 validation 宏 Debug / Release 正式根工程 | PASS | 两配置重新生成正式 IFC 并完整链接成功 |
| Debug 外部 ABI consumer | PASS | 导入正式模块，静态核对 v2 入口 member-function 完整签名和合同版本，链接并运行 0 |
| Release `/GL + /LTCG` 外部 ABI consumer | PASS | `/GL` 编译，`/LTCG` 链接并运行 0 |
| v2 持久恢复 | PASS | W05 在恢复前后以同一 self/Gseed/Gstart/G0 读取，完整结果逐字段相等；成功同时证明 provider-private G0 当前根互证通过 |
| validation hook 二次只读审查 | PASS | 当前成员闭包、owner、登记版本、role=1、活动性、载荷和引用完整性守卫齐全；历史事实不能被钩子选中 |
| `git diff --check` | PASS | 全工作区无空白错误；只有既存 LF/CRLF 提示 |
| `python .\tools\check_specs.py --strict` | PASS | 116 / 116 |
| 私有 ABI / 生产调用静态扫描 | PASS | `3060..306B`、私有结构版本和幂等身份未从公开头导出；validation hook 只命中 owner 与专项；新入口无上层生产消费者 |

## 2. 专项新增覆盖

- B00：空种子、`Gseed/Gstart/G0` 边界和合法空集合。
- B01：锚点前 incumbent、后继更新、预算边界、左开区间及 v1 回归。
- B02：非法版本 / 范围、预算、漂移、资源和空失败形状。
- B03：延迟升级只走一次生产登记路径，种子保持首次事实。
- B04：左边界终态、消费秒超额和 foreign self 拷贝均使 `成功()`失败。
- B05：状态 / 到期事件未知或异主合同引用整体失败。
- B06：双 self 交错事实分别读取，无载荷、声明数或预算泄漏。
- B07：历史重放 G0 终态与正式当前根不一致时 provider 返回集合不闭合、零载荷。
- B08：同形成代次的不同业务流后继合法；同业务流缺后继失败。
- B09：退出任一 v2 永久种子关系后，服务重建必须拒绝。
- B10：用正式 publisher 建立合法进展 / 准备，再分别破坏正式选择、状态或动态引用；延迟 v2 登记全部拒绝，正常对照成功。
- W05：publisher、v1 历史和 v2 边界快照在持久恢复后精确重放。

## 3. 外部 ABI consumer

仓库外源码导入：

```cpp
import 海中鱼巣.领域.服务.服务合同事实权威;
```

静态核对：

```text
服务维护历史边界读取结果_v2
(服务合同事实权威服务::*)(
    const 服务维护历史边界读取请求_v2&) const noexcept

服务维护历史边界合同版本_v2 == 2
```

带 validation-only IFC 的手工 consumer 曾触发 MSVC C1001；重新按生产边界生成无测试宏正式 IFC 后，Debug 和 Release consumer 均通过。该工具问题未通过放宽 ABI 或修改生产源码规避。

## 4. 静态边界

- 公开快照不含第二份 G0 当前合同 / 状态 / 进展 / 准备或同义终态投影。
- `0x0307'3060..306B`、`0x494E'5354'5348'5632`、私有登记结构和内部成员类型只存在于 owner 实现。
- validation-only 钩子没有生产调用。
- owner / 专项之外没有 v2 历史边界消费；上层既存其它 `G0` 工程材料不属于本 ABI，也未被本轮改写。

## 5. 未验证与不声明

- 未做后继维护 composer、普通应用线程消费或具名集成验收，因为本计划不实现这些消费者。
- 未声明覆盖种子前历史可恢复。
- 未声明工程完整性快照属于状态、动态、因果或其它认知事实。
- 未声明阶段三、本能路线或项目业务闭环完成。
