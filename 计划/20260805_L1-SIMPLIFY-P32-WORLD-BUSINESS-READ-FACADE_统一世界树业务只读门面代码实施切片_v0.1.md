# L1-SIMPLIFY-P32-WORLD-BUSINESS-READ-FACADE 统一世界树业务只读门面代码实施切片 v0.1

日期：2026-08-05

版本：v0.1

状态：待激活

创建侧代码基线：`30b17ce0952f7fbbf641f0158cf663757aca210f`

## 1. 目标与完成条件

新建唯一 `世界树业务服务` 三读真实门面，只保存 `const 世界树快照读取服务&`，把存在、场景和整树强类型读取一对一委托给 P30、P31、P29，并逐字段透明返回 provider 结果。

完成必须同时满足：

- 新模块/类唯一，复用 P28—P31 DTO，不新增读状态、请求、结果或诊断；
- 世界树业务服务合同版本固定为1并与本阶段完整世界快照合同版本一致；
- 三个函数各调用对应 provider 恰1，其它根调用0；
- 所有成功、合法空组、结构化非成功和未实现结果逐字段原样返回；
- 门面不校验、映射、重组、缓存、重试、捕获改判或访问事实底层；
- 只有一个 const 引用成员，无写 provider、写入口或可变状态；
- 旧 P1C-F/QF/Q 不施工、不适配、不形成第二门面。

本叶不实现六个世界写入口、状态身份轻量读取、治理白名单路由、恢复装配、运行期接线、旧入口删除、集成验收、比较、动作动态、因果、任务/结算或 STEP-5。

## 2. 正式依据、设计包与未来代码白名单

直接依据：0050、4015、4190、4200 §1—§6、4201，以及 P28/P29/P30/P31 v0.1。

设计包：

```text
规范/详细设计/20260805_L1-SIMPLIFY-P32-WORLD-BUSINESS-READ-FACADE_统一世界树业务只读门面详细设计_v0.1.md
流程图/20260805_L1-SIMPLIFY-P32-WORLD-BUSINESS-READ-FACADE_统一世界树业务只读门面函数流程图_v0.1.md
计划/20260805_L1-SIMPLIFY-P32-WORLD-BUSINESS-READ-FACADE_统一世界树业务只读门面代码实施切片_v0.1.md
```

未来代码白名单：

```text
海中鱼巣/领域/服务.世界树.ixx
海中鱼巣/领域/自检.世界树业务只读.ixx
海中鱼巣.vcxproj
海中鱼巣.vcxproj.filters
海中鱼巣/启动.应用程序.ixx
施工记录/20260805_L1-SIMPLIFY-P32-WORLD-BUSINESS-READ-FACADE_施工记录.md
验证记录/20260805_L1-SIMPLIFY-P32-WORLD-BUSINESS-READ-FACADE_验证记录.md
```

不得修改 P28/P29/P30/P31 数据、服务、自检或设计，不得触碰 P11、P15、P0-FIX、路线输入、旧 WORLD-TREE 或其它 WIP，不得新增 DTO/写骨架/兼容入口。

## 3. 目标接口与物理变更

新建模块 `海中鱼巣.领域.服务.世界树` 和 final 类 `世界树业务服务`。类只保存：

```cpp
const 世界树快照读取服务& 快照读取_;
```

服务模块唯一声明 `世界树业务服务合同版本=1`；构造、删除的特殊成员与三个公开函数严格采用详细设计第4节签名。三个函数体严格采用第5节单行直接委托，不增加 helper、结果工厂、状态映射或异常包装。

新建顺序460专项自检模块；工程、filters 和启动聚合只登记新模块、自检薄包装和一个顺序项，不接生产线程或旧消费者。

## 4. 待激活门禁

保持待激活，直至：

1. P29、P30、P31 真实结果进入 main 并经交互接受；
2. 三个 provider 实际请求、结果、状态、函数、const/非`noexcept`、全等和可见性机械匹配；
3. P29/P30/P31 服务/自检、工程/filters/启动、顺序460、完整构建/自检/模块扫描资源释放；
4. 从正式新 HEAD 重做 S0，确认 `服务.世界树.ixx` 不存在，旧 WORLD-TREE-P1C-F/QF/Q 未激活且白名单无所有权冲突。

P17—P28 的真实性由 P29—P31 门禁传递，不是 P32 直接调用依赖。治理白名单路由和六写门面不是本叶激活前置。

## 5. 实施步骤

1. 从新 HEAD 核对 P29/P30/P31 plan blob、索引状态、真实签名和资源所有权。
2. 新建唯一服务模块，只 import 世界树快照读取公开模块及签名所需 DTO。
3. 声明单 const 引用、显式构造、删除默认/复制/移动和三个公开读根。
4. 逐函数实现单行直接委托；禁止任何前后处理或其它调用。
5. 新建顺序460专项，覆盖构造、三根调用计数、结果透明、异常、并发和静态扫描矩阵。
6. 登记工程/filters/启动与顶层自检，形成专属施工/验证记录并执行完整门禁。

## 6. 失败、异常、并发和生命周期

- provider 的全部结构化状态和载荷原样返回，门面不分支；
- `未实现=11` 原样返回，不映射为成功、未找到或资源失败；
- provider 外泄异常原样传播，不重试、不调用其它根；
- 多线程调用无新共享可变状态；并发和许可语义完全属于 provider；
- 装配宿主保证 provider 生命周期，门面析构不释放 provider；
- 门面不保存请求、结果、截止、索引或缓存。

## 7. 验证

至少执行：

```powershell
cmake --build --preset windows-msvc-debug --target 海中鱼巣 --clean-first
cmake --build --preset windows-msvc-release --target 海中鱼巣 --clean-first
& .\out\build\windows-msvc-debug\Debug\海中鱼巣.exe --self-test-exit
& .\out\build\windows-msvc-release\Release\海中鱼巣.exe --self-test-exit
python .\tools\check_specs.py --strict
pwsh -NoProfile -Command '$PSNativeCommandUseErrorActionPreference=$false; & ".\tools\clang_ast\扫描.MSVC模块依赖.ps1" -配置 Debug -平台 x64'
pwsh -NoProfile -Command '$PSNativeCommandUseErrorActionPreference=$false; & ".\tools\clang_ast\扫描.MSVC模块依赖.ps1" -配置 Release -平台 x64'
git diff --check -- <本计划白名单>
```

专项必须覆盖详细设计第7节14项，重点证明：类和三签名唯一、每根对应provider恰1/其它根0、全部结果逐字段透明、未实现不改判、无缓存、异常不重试、并发无新状态和底层/旧栈调用0。

若模块扫描工具参数机械变化，只按正式工具帮助适配命令，不降低双配置全部通过标准。

## 8. 完成声明边界

允许声明：唯一 `世界树业务服务` 三读门面源码候选形成，能把存在、场景和整树强类型请求一对一委托给唯一快照读取服务并透明返回结果。

禁止声明：P29/P30/P31 已实现、六写门面、治理白名单路由、恢复装配、运行期接线、旧入口退出、集成验收、STEP-4、STEP-5 或治理闭环完成。
