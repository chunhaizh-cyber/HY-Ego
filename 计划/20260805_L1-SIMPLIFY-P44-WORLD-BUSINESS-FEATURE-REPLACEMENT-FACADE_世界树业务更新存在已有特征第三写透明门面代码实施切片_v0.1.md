# L1-SIMPLIFY-P44 世界树业务更新存在已有特征第三写透明门面代码实施切片 v0.1

日期：2026-08-05

状态：待激活

计划身份：`L1-SIMPLIFY-P44-WORLD-BUSINESS-FEATURE-REPLACEMENT-FACADE`

创建侧基线：`8a834fe30ba21a7fd3525ee313db3fd598e5f00d`

## 1. 施工目标与完成条件

原位扩展P32/P35/P41唯一`世界树业务服务`，把构造从“快照读取+P34存在发布+P40初始批次发布”扩展为再加P43换代批次发布的四个必需引用，新增第三写`更新存在已有特征`。函数体只调用P43同一请求恰1次并逐字段透明返回。

完成必须满足：既有五根完全不变；P41三引用构造完整退出；第三写直接复用P43 DTO且为const、非noexcept；门面不解释或映射P43任何字段；其它provider和下层内部入口调用0；P32/P35/P41回归、P44顺序530专项及完整验证通过。

## 2. 设计包与未来代码白名单

设计包：

```text
规范/详细设计/20260805_L1-SIMPLIFY-P44-WORLD-BUSINESS-FEATURE-REPLACEMENT-FACADE_世界树业务更新存在已有特征第三写透明门面详细设计_v0.1.md
流程图/20260805_L1-SIMPLIFY-P44-WORLD-BUSINESS-FEATURE-REPLACEMENT-FACADE_更新存在已有特征函数流程图_v0.1.md
计划/20260805_L1-SIMPLIFY-P44-WORLD-BUSINESS-FEATURE-REPLACEMENT-FACADE_世界树业务更新存在已有特征第三写透明门面代码实施切片_v0.1.md
```

未来代码白名单：

```text
海中鱼巣/领域/服务.世界树.ixx
海中鱼巣/领域/自检.世界树业务只读.ixx
海中鱼巣/领域/自检.世界树业务创建存在.ixx
海中鱼巣/领域/自检.世界树业务发布初始特征.ixx
海中鱼巣/领域/自检.世界树业务更新已有特征.ixx
海中鱼巣.vcxproj
海中鱼巣.vcxproj.filters
海中鱼巣/启动.应用程序.ixx
施工记录/20260805_L1-SIMPLIFY-P44-WORLD-BUSINESS-FEATURE-REPLACEMENT-FACADE_施工记录.md
验证记录/20260805_L1-SIMPLIFY-P44-WORLD-BUSINESS-FEATURE-REPLACEMENT-FACADE_验证记录.md
```

不得修改P29—P31、P34、P40、P42/P43 provider/DTO/账/参与者、P15、旧P1C服务、运行期装配或其它所有者WIP。白名单外出现正式构造调用点时停止并退回计划支撑，不得漏改或保留半装配构造。

## 3. 固定接口与所有权

精确实现：

```cpp
世界树业务服务(
    const 世界树快照读取服务& 快照读取,
    L1当前场景实际存在发布服务& 当前场景实际存在发布,
    实际存在I64初始特征批次发布服务& 初始特征批次发布,
    实际存在I64已有特征换代批次发布服务& 已有特征换代批次发布) noexcept;

实际存在I64已有特征换代批次发布结果 更新存在已有特征(
    const 实际存在I64已有特征换代批次发布请求& 请求) const;
```

第三写唯一合法函数体：

```cpp
return 已有特征换代批次发布_.发布实际存在I64已有特征换代批次(请求);
```

不允许额外入口、DTO、校验、映射、兼容构造、指针、optional、setter、函数对象或服务定位器。

## 4. 机械施工顺序

### S0：正式接口、构造和所有权重绑定

1. 核对正式HEAD、plan blob、可执行状态、唯一活动代码段、十文件白名单和空index。
2. 读取P32/P35/P41实际世界树类、五根、三引用构造和全部构造调用点，逐签名核对。
3. 读取P43实际模块、类、请求、结果和公开函数，确认非const provider可由门面const函数经非const引用直接调用。
4. 确认顺序460/470/480/500/510/520/530链实际匹配且530未占用。
5. 确认P15/P41/P43及届时活动计划释放全部目标文件和构建、自检、模块扫描资源。

任一接口、构造调用点、顺序、物理文件、plan blob或所有权漂移，停止并退回计划支撑。

### S1：构造与第三写门面

1. 在`服务.世界树.ixx`复用实例特征服务模块import，不引入P43下层内部模块。
2. 用详细设计四引用构造替代P41三引用构造，不保留overload或空provider。
3. 保持P32三读、P35第一写和P41第二写源码语义不变，新增一行直接委托第三写。
4. 静态断言五个合同版本为1、六个公开函数签名和特殊成员状态。

### S2：专项、工程与记录

1. 三个既有世界树专项只补入真实P43 fixture引用，原矩阵不变。
2. 新建顺序530专项，覆盖详细设计第7节；无P43测试seam的成功分支按P43专项与源码闭包分账，不伪造provider。
3. 工程/filters只登记新专项；启动只增加import、薄包装和顺序530项，不接生产消费者。
4. 形成专属施工/验证记录，登记构造调用点、签名、调用计数、结果全等、既有五根回归和静态扫描。

## 5. 禁止实现

```text
禁止重复实现、拆分或包装P42/P43批次算法
禁止门面解析批次、宿主、定义、槽、写前/新值、来源、摘要、4170记录或发布存在标志
禁止门面调用P15/P38/P39/P42/L1、仓库、SQL、材料或旧栈
禁止保留三引用构造、空P43、未实现第三写、setter或兼容overload
禁止第二provider调用、快照预读、结果重组、缓存、日志、retry、fallback或try/catch改判
禁止顺带设计第四写、状态/动作动态、移动/移除、治理路由、装配、旧入口退出或STEP-5
```

## 6. 激活门禁

保持`待激活`，直至：

1. P32/P34/P35/P40/P41真实结果进入正式main并经交互接受，五根、三引用构造和顺序460/470/480/500实际匹配；
2. P42/P43及正式依赖链真实结果进入main，P43经交互接受，其类/请求/结果/函数/异常和完整字段机械匹配；
3. P15替代版对P43的间接影响已由P43真实结果门禁吸收；
4. P32/P35/P41/P43释放全部白名单、工程/filters/启动、构建、自检和模块扫描资源；
5. 新HEAD扫描确认世界树服务全部构造调用点在白名单内，530未占用，旧P1C-F/FW未施工且无等价第三写；
6. 执行侧从新HEAD重做完整S0并确认本计划为唯一可执行代码段。

冻结合同只支持设计推演，不证明本计划当前可执行。

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

成功条件：P32三读、P35第一写、P41第二写不退化；P44专项证明四引用构造、第三写签名、P43一次委托、结果全等和零旁路；完整自检两配置退出0；strict、双配置模块扫描、工程/filters一致性、目标与cached diff check通过。

## 8. 停工与完成声明

若P41/P43实际接口不匹配、出现白名单外构造调用点、530已占用、必须保留空provider/兼容构造、需要修改provider/装配/旧入口，或验证持续无法归因，停止并退回计划支撑。

完成后只可声明：P44白名单内源码候选使唯一世界树业务服务增加更新存在已有特征的第三写透明门面，并保持P32三读、P35第一写和P41第二写不变。

不得声明：P32/P35/P41/P43已实现、第四写及后续状态/动作动态、移动/移除、治理白名单、生产装配、运行期接线、持久恢复、旧入口退出、集成验收、STEP-4、STEP-5或治理闭环完成。
