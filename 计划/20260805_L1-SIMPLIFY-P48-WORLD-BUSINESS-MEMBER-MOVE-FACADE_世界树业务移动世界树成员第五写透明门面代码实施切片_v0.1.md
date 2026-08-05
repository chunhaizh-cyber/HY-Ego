# L1-SIMPLIFY-P48 世界树业务移动世界树成员第五写透明门面代码实施切片 v0.1

日期：2026-08-05

状态：待激活

计划身份：`L1-SIMPLIFY-P48-WORLD-BUSINESS-MEMBER-MOVE-FACADE`

创建侧基线：`ca774d73391de0561cedcaeaa3fbe172fb628fd7`

## 1. 施工目标与完成条件

原位扩展P45唯一`世界树业务服务`，把构造从九个既有引用扩展为追加`const L1世界树成员移动发布服务&`的唯一十引用构造，新增第五写`移动世界树成员`。函数体只调用P47同名方法恰1次并逐字段透明返回。

完成必须满足：既有七根完全不变；P45九引用构造完整退出；第五写直接复用P47 DTO且为const、非noexcept；门面不解释或映射任何P47字段；P46/P15/L1及其它provider直接调用0；P32/P35/P41/P44/P45回归、P48顺序547专项和完整验证通过。

## 2. 设计包与未来代码白名单

设计包：

```text
规范/详细设计/20260805_L1-SIMPLIFY-P48-WORLD-BUSINESS-MEMBER-MOVE-FACADE_世界树业务移动世界树成员第五写透明门面详细设计_v0.1.md
流程图/20260805_L1-SIMPLIFY-P48-WORLD-BUSINESS-MEMBER-MOVE-FACADE_移动世界树成员第五写透明门面函数流程图_v0.1.md
计划/20260805_L1-SIMPLIFY-P48-WORLD-BUSINESS-MEMBER-MOVE-FACADE_世界树业务移动世界树成员第五写透明门面代码实施切片_v0.1.md
```

未来代码白名单：

```text
海中鱼巣/领域/服务.世界树.ixx
海中鱼巣/领域/自检.世界树业务只读.ixx
海中鱼巣/领域/自检.世界树业务创建存在.ixx
海中鱼巣/领域/自检.世界树业务发布初始特征.ixx
海中鱼巣/领域/自检.世界树业务更新已有特征.ixx
海中鱼巣/领域/自检.世界树业务发布状态动态.ixx
海中鱼巣/领域/自检.世界树业务移动成员.ixx
海中鱼巣.vcxproj
海中鱼巣.vcxproj.filters
海中鱼巣/启动.应用程序.ixx
施工记录/20260805_L1-SIMPLIFY-P48-WORLD-BUSINESS-MEMBER-MOVE-FACADE_施工记录.md
验证记录/20260805_L1-SIMPLIFY-P48-WORLD-BUSINESS-MEMBER-MOVE-FACADE_验证记录.md
```

不得修改P15/P46/P47、P32—P45 provider或DTO、旧P1C服务、旧节点直接场景、运行期装配或其它所有者WIP。白名单外出现正式构造调用点时停止并退回计划支撑，不得漏改或保留半装配构造。

## 3. 固定接口与所有权

精确新增接口：

```cpp
世界树成员移动发布结果 移动世界树成员(
    const 世界树成员移动发布请求& 请求) const;
```

唯一合法函数体：

```cpp
return 世界树成员移动发布_.移动世界树成员(请求);
```

构造在P45九引用尾部追加`const L1世界树成员移动发布服务& 世界树成员移动发布`，形成唯一十引用构造。不允许额外入口、DTO、校验、映射、兼容构造、指针、optional、setter、函数对象或服务定位器。

## 4. 机械施工顺序

### S0：正式接口、构造和所有权重绑定

1. 核对正式HEAD、plan blob、可执行状态、唯一活动代码段、十二文件白名单和空index。
2. 读取P32/P35/P41/P44/P45实际世界树类、七根、九引用构造和全部构造调用点，逐签名核对。
3. 读取已由交互接受的P47实际DTO、类和const方法；确认P47真实结果已吸收P15替代版本，门面不得读取P15恢复载体临场适配。
4. 确认顺序460/480/500/530/540/545/546/547链匹配且547未占用，550保持既有占用。
5. 确认P45/P47及届时活动计划释放全部目标文件和构建、自检、模块扫描资源。

任一接口、构造调用点、顺序、物理文件、plan blob或所有权漂移，停止并退回计划支撑。

### S1：构造与第五写门面

1. 在`服务.世界树.ixx`只引入P47公开服务模块，不引入P46/P15/L1私有实现。
2. 用详细设计十引用构造替代P45九引用构造，不保留overload或空provider。
3. 保持既有七根源码语义不变，新增一行直接委托第五写。
4. 静态断言世界树与P47合同版本、八个公开函数签名和特殊成员状态。

### S2：专项、工程与记录

1. 五个既有世界树专项只补入P47必需fixture引用，原矩阵不变。
2. 新建顺序547专项，覆盖详细设计第7节；P47成功分支使用正式夹具，不伪造P46/P15/L1语义。
3. 工程/filters只登记新专项；启动只增加import、薄包装和顺序547项，不接生产消费者。
4. 形成专属施工/验证记录，登记构造调用点、签名、调用计数、结果全等、既有七根回归和静态扫描。

## 5. 禁止实现

```text
禁止重复实现、拆分或包装P46/P47移动算法
禁止门面解析根、成员、父链、关系、幂等、摘要、写集、读回或失败见证
禁止门面调用P46/P15/L1、仓库、SQL、恢复材料或旧节点直接场景
禁止把P15 v0.9或替代版恢复载体字段复制到P48合同
禁止保留九引用构造、空P47依赖、未实现第五写、setter或兼容overload
禁止第二provider调用、快照预读、结果重组、缓存、日志、retry、fallback或try/catch改判
禁止顺带设计移除存在、第六写、治理路由、动作动态、因果、装配、旧入口退出或STEP-5
```

## 6. 激活门禁

保持`待激活`，直至：

1. P32/P35/P41/P44/P45真实结果进入正式main并经交互接受，七根、九引用构造和既有专项实际匹配；
2. P47真实结果进入正式main并经交互接受，实际DTO、类、const函数、九状态与字段匹配；P47结果须已吸收P15替代结果；
3. P45/P47释放全部白名单、工程/filters/启动、顺序547、构建、自检和模块扫描资源；
4. 新HEAD扫描确认世界树服务全部构造调用点在白名单内，547未占用，旧P1C-F/SW未施工且无等价第五写；
5. 执行侧从新HEAD重做完整S0并确认本计划为唯一可执行代码段。

冻结合同只支持设计推演，不证明本计划当前可执行。若P47因P15替代结果改变公开接口，本计划先退回计划支撑同步修订，不允许门面临场适配。

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

成功条件：P32三读和前四写不退化；P48专项证明十引用构造、第五写签名、P47一次委托、结果全等和零旁路；完整自检两配置退出0；strict、双配置模块扫描、工程/filters一致性、目标与cached diff check通过。

## 8. 停工与完成声明

若P47实际接口不匹配、出现白名单外构造调用点、547已占用、必须保留空依赖/兼容构造、需要修改provider/装配/旧入口，或验证持续无法归因，停止并退回计划支撑。

完成后只可声明：P48白名单内源码候选使唯一世界树业务服务增加移动世界树成员的第五写透明门面，并保持P32三读和前四写不变。

不得声明：P15/P46/P47或P32—P45已实现、移除存在、第六写、动作动态、因果、任务/结算、持久恢复、生产装配、运行期接线、旧入口退出、集成验收、STEP-4、STEP-5或治理闭环完成。
