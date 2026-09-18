# FEATURE-DOMAIN-COMPARISON-BINDING-ABI 施工记录

计划：`FEATURE-DOMAIN-COMPARISON-BINDING-ABI` v0.1
开工基线：`68ebb3cc54282e3ab6e18c5b47a6a648eaae73c0`
完成边界：仅发布特征值域比较服务的只读同 L1 绑定 ABI；不代表存在概念两组定义、值域比较、恢复或业务闭环完成。

## 实际变更

- `海中鱼巣/领域/数据服务.特征值域比较类.h`：新增公开只读谓词 `bool 绑定于(const L1事实基座服务&) const noexcept;`。
- `海中鱼巣/领域/数据服务.特征值域比较类.cpp`：唯一实现为概念树、特征、特征值三项既有 `绑定于` 谓词的逻辑与。
- 未新增字段、构造参数、写端口、写集、事实读取、值域读取或生产消费者。

## S0 与范围核对

当前 `main`、`HEAD` 和 `origin/main` 在本切片开工时均为 `f28ac086ecf3db403945945e92ac05d606a1161a`；目标两生产文件仅由本切片修改。工作区其余 WIP、删除项和后消费者审计记录均原地保留，未暂存、覆盖或修改。

三项被组合服务均已有同形 `绑定于(const L1事实基座服务&) const noexcept`。当前比较服务已持有其三个 `const` 引用，故本次是冻结计划指定的最小 ABI 扩展，不改变比较语义或依赖方向。

## 代码知识变更清单

| 类型 | 函数 | 用途与合同 | 调用边 / 副作用 | 知识维护 |
| --- | --- | --- | --- | --- |
| 新增 | `特征值域比较数据服务::绑定于(const L1事实基座服务&) const noexcept` | 仅当内部概念树、特征、特征值服务都绑定传入的同一 L1 时返回 true。 | 调用三项既有绑定谓词；零读取、零写入、零分配、无事务和无生命周期变化。 | 需为该新函数建立用途卡或在现有比较服务用途卡中补充。 |

既有 `读取特征值域`、`核验特征值域关系`、`核验实例值域命中` 未改动；没有退出函数或新增生产调用者。

## 隔离夹具与验证说明

夹具位于仓外 `D:\TEMP\海中鱼巣\FEATURE-DOMAIN-COMPARISON-BINDING-ABI\binding_probe.cpp`。它将普通应用装配与本模块 `.cpp` 作为自身翻译单元输入，并排除工程中的 `入口.obj`、`装配.普通应用.obj`、比较服务 `.obj`，以避免重复定义。

初次 Debug 链接因使用默认 Release CRT 出现 `LNK2038`；改为 `/MDd /D_DEBUG /D_ITERATOR_DEBUG_LEVEL=2` 后，Debug 夹具链接、运行均退出 0。Release 使用 `/MD /DNDEBUG /D_ITERATOR_DEBUG_LEVEL=0` 后链接、运行均退出 0。后续用已被超时中止的旧持久目录重复诊断时，Debug 装配停在持久恢复阶段；该仓外输入不属于生产模块，未将其作为通过证据或产品失败。Release 最终使用两套新的受控持久根完成验证。

详见 `验证记录/FEATURE-DOMAIN-COMPARISON-BINDING-ABI_验证记录_v0.1.md`。
