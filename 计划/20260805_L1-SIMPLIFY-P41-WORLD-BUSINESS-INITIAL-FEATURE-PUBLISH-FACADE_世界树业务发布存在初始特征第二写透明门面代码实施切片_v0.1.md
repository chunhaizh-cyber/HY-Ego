# L1-SIMPLIFY-P41 世界树业务发布存在初始特征第二写透明门面代码实施切片 v0.1

日期：2026-08-05

状态：待激活

计划身份：`L1-SIMPLIFY-P41-WORLD-BUSINESS-INITIAL-FEATURE-PUBLISH-FACADE`

创建侧基线：`d20d9ba7a45cabd6e85bfeb650330f5658f84fd9`

## 1. 施工目标与完成条件

原位扩展 P32/P35 唯一 `世界树业务服务`，把构造从“快照读取 + P34存在发布”扩展为“快照读取 + P34存在发布 + P40初始特征批次发布”三个必需引用，新增第二写 `发布存在初始特征`，函数体只调用 P40 同一请求恰1次并逐字段透明返回。

完成必须同时满足：

- P32三读和 P35第一写签名、const、异常与一对一委托完全不变；
- P35双引用构造完整退出，三引用构造成为唯一合法构造；
- 第二写为const、非noexcept，请求/结果直接复用 P40，不新增 DTO 或状态映射；
- 第二写只调用 P40一次，快照/P34及下层provider调用0；
- 门面不读取或解释批次、规格、摘要、4170、事务、读回或失败，不保存状态、不加锁、不捕获改判；
- P32/P35回归、P41顺序500专项和完整验证全部通过。

## 2. 正式设计包与未来代码白名单

设计包：

```text
规范/详细设计/20260805_L1-SIMPLIFY-P41-WORLD-BUSINESS-INITIAL-FEATURE-PUBLISH-FACADE_世界树业务发布存在初始特征第二写透明门面详细设计_v0.1.md
流程图/20260805_L1-SIMPLIFY-P41-WORLD-BUSINESS-INITIAL-FEATURE-PUBLISH-FACADE_发布存在初始特征函数流程图_v0.1.md
计划/20260805_L1-SIMPLIFY-P41-WORLD-BUSINESS-INITIAL-FEATURE-PUBLISH-FACADE_世界树业务发布存在初始特征第二写透明门面代码实施切片_v0.1.md
```

未来代码白名单：

```text
海中鱼巣/领域/服务.世界树.ixx
海中鱼巣/领域/自检.世界树业务只读.ixx
海中鱼巣/领域/自检.世界树业务创建存在.ixx
海中鱼巣/领域/自检.世界树业务发布初始特征.ixx
海中鱼巣.vcxproj
海中鱼巣.vcxproj.filters
海中鱼巣/启动.应用程序.ixx
施工记录/20260805_L1-SIMPLIFY-P41-WORLD-BUSINESS-INITIAL-FEATURE-PUBLISH-FACADE_施工记录.md
验证记录/20260805_L1-SIMPLIFY-P41-WORLD-BUSINESS-INITIAL-FEATURE-PUBLISH-FACADE_验证记录.md
```

不得修改 P29—P31、P34、P36—P40 provider/数据头/账/参与者、旧 P1C服务、运行期装配、其它计划或其它所有者 WIP。白名单外出现正式构造调用点时停止并由计划支撑修订，不得漏改或保留半装配构造。

## 3. 固定目标合同

精确实现：

```cpp
世界树业务服务(
    const 世界树快照读取服务& 快照读取,
    L1当前场景实际存在发布服务& 当前场景实际存在发布,
    实际存在I64初始特征批次发布服务& 初始特征批次发布) noexcept;

实际存在I64初始特征批次发布结果 发布存在初始特征(
    const 实际存在I64初始特征批次发布请求& 请求) const;
```

第二写函数体唯一合法语义：

```cpp
return 初始特征批次发布_.发布实际存在I64初始特征批次(请求);
```

不允许额外入口、DTO、校验、映射、兼容构造、指针、optional、setter、函数对象或服务定位器。

## 4. 机械施工顺序

### S0：正式接口与调用点重绑定

1. 核对 `main==origin/main`、0/0、index空、目标 plan blob、状态为可执行和唯一活动代码段。
2. 读取 P32/P35 实际世界树模块、三读/第一写、构造和全部调用点，逐签名核对。
3. 读取 P40实际模块、类、请求、结果和公开函数，确认非const/非noexcept provider可由门面const函数经非const引用直接调用。
4. 确认 P32顺序460、P34顺序470、P35顺序480、P38/P39顺序490和本叶顺序500均与正式结果匹配且唯一。
5. 核对九文件白名单及所有共享验证资源已经释放。

任一接口、构造调用点、顺序、物理文件、plan blob或所有权漂移，停止并退回计划支撑。

### S1：构造与第二写门面

1. 在 `服务.世界树.ixx` import P40正式模块，不 import P40下层内部模块。
2. 用详细设计三引用构造替代 P35双引用构造；不保留 overload或空provider。
3. 保持 P32三读和 P35第一写源码语义不变，新增一行直接委托函数。
4. 静态断言四个合同版本为1、五个公开函数签名和特殊成员状态。

### S2：专项、工程与记录

1. P32只读专项和 P35创建存在专项只补入真实 P40 fixture引用；原矩阵不变。
2. 新建顺序500专项，覆盖详细设计第7节；无 P40测试 seam的分支按真实入口拒绝运行与源码闭包分账，不伪造成功 provider。
3. 工程/filters只登记新增专项；启动只增加 import、薄包装和顺序500项，不接生产消费者。
4. 形成专属施工/验证记录，记录构造调用点、签名、调用计数、结果全等、既有四根回归和静态扫描。

## 5. 禁止实现

```text
禁止重复实现或包装P40内部发布算法
禁止门面解析批次、顺序、宿主、定义、值域、摘要、4170记录或发布布尔
禁止门面调用P15/P36/P38/P39内部入口、L1、仓库、SQL、材料或旧栈
禁止保留双引用构造、空P40、未实现第二写、setter或兼容overload
禁止第二provider调用、快照预读、结果重组、缓存、日志、retry、fallback或try/catch改判
禁止顺带设计更新已有特征、其它四写、治理路由、装配、旧入口退出或STEP-5
```

## 6. 激活门禁

保持 `待激活`，直至：

1. P32、P34、P35真实结果进入正式 main 并经交互接受，三读、第一写、双引用构造和顺序460/470/480实际匹配；
2. P36—P40真实结果进入正式 main，P40经交互接受，其类/请求/结果/函数/异常和完整字段机械匹配；
3. P32/P35/P40释放全部白名单、工程/filters/启动、构建、自检和模块扫描资源；
4. 新 HEAD 扫描确认世界树服务构造调用点全在白名单，顺序500未占用，旧 P1C-F/FW未施工且无等价第二写；
5. 执行侧从新 HEAD 重做完整 S0，确认本计划是唯一可执行代码段。

冻结合同只支持本设计推演，不证明本计划当前可执行。

## 7. 验证命令与成功条件

至少执行：

```powershell
git diff --check -- <本计划白名单>
python .\tools\check_specs.py --strict
cmake --build --preset windows-msvc-debug --target 海中鱼巣 --clean-first
cmake --build --preset windows-msvc-release --target 海中鱼巣 --clean-first
& .\out\build\windows-msvc-debug\Debug\海中鱼巣.exe --self-test-exit
& .\out\build\windows-msvc-release\Release\海中鱼巣.exe --self-test-exit
pwsh -NoProfile -Command '$PSNativeCommandUseErrorActionPreference=$false; & ".\tools\clang_ast\扫描.MSVC模块依赖.ps1" -配置 Debug -平台 x64'
pwsh -NoProfile -Command '$PSNativeCommandUseErrorActionPreference=$false; & ".\tools\clang_ast\扫描.MSVC模块依赖.ps1" -配置 Release -平台 x64'
```

成功条件：P32三读和 P35第一写专项全部不退化；P41专项证明三引用构造、第二写签名、P40一次委托、结果全等和零旁路；完整自检两配置退出0；strict、双配置模块扫描、工程/filters一致性、目标 diff check和 staged白名单全部通过。

## 8. 停工与完成声明

以下情况停工：P32/P35/P40实际接口不匹配；出现白名单外构造调用点；正式顺序与460—500链不匹配或500已占用；必须保留空provider/兼容构造才能编译；需修改provider/装配/旧入口；验证持续失败且无法在本计划内归因；Git或所有权冲突。

完成后只可声明：P41白名单内源码候选使唯一世界树业务服务增加发布存在初始特征的第二写透明门面，并保持 P32三读和 P35第一写不变。

禁止声明：P32/P35/P40本身由本叶实现、更新已有特征、其它四写、治理白名单、生产装配、运行期接线、旧入口退出、集成验收、STEP-4、STEP-5或治理闭环完成。
