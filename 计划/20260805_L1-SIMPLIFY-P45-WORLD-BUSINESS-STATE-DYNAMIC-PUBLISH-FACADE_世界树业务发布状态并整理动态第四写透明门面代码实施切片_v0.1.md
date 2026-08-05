# L1-SIMPLIFY-P45 世界树业务发布状态并整理动态第四写透明门面代码实施切片 v0.1

日期：2026-08-05

状态：待激活

计划身份：`L1-SIMPLIFY-P45-WORLD-BUSINESS-STATE-DYNAMIC-PUBLISH-FACADE`

创建侧基线：`1e1cf221a54b56ee7d508a5f687386dd6c681df0`

## 1. 施工目标与完成条件

原位扩展 P32/P35/P41/P44 唯一`世界树业务服务`，把构造从四个既有 provider 引用扩展为再加 P16 唯一公开函数所需的 L1事实基座、实例特征、状态、比较、动态五个必需引用，新增第四写`发布状态并整理动态`。函数体只调用 P16 `发布实际存在I64后继状态与迁移动能`恰1次并逐字段透明返回。

完成必须满足：既有六根完全不变；P44四引用构造完整退出；第四写直接复用P16 DTO且为const、非noexcept；门面不解释或映射P16任何字段；P13/P14/P15及其它provider直接调用0；P32/P35/P41/P44回归、P45顺序540专项及完整验证通过。

## 2. 设计包与未来代码白名单

设计包：

```text
规范/详细设计/20260805_L1-SIMPLIFY-P45-WORLD-BUSINESS-STATE-DYNAMIC-PUBLISH-FACADE_世界树业务发布状态并整理动态第四写透明门面详细设计_v0.1.md
流程图/20260805_L1-SIMPLIFY-P45-WORLD-BUSINESS-STATE-DYNAMIC-PUBLISH-FACADE_发布状态并整理动态函数流程图_v0.1.md
计划/20260805_L1-SIMPLIFY-P45-WORLD-BUSINESS-STATE-DYNAMIC-PUBLISH-FACADE_世界树业务发布状态并整理动态第四写透明门面代码实施切片_v0.1.md
```

未来代码白名单：

```text
海中鱼巣/领域/服务.世界树.ixx
海中鱼巣/领域/自检.世界树业务只读.ixx
海中鱼巣/领域/自检.世界树业务创建存在.ixx
海中鱼巣/领域/自检.世界树业务发布初始特征.ixx
海中鱼巣/领域/自检.世界树业务更新已有特征.ixx
海中鱼巣/领域/自检.世界树业务发布状态动态.ixx
海中鱼巣.vcxproj
海中鱼巣.vcxproj.filters
海中鱼巣/启动.应用程序.ixx
施工记录/20260805_L1-SIMPLIFY-P45-WORLD-BUSINESS-STATE-DYNAMIC-PUBLISH-FACADE_施工记录.md
验证记录/20260805_L1-SIMPLIFY-P45-WORLD-BUSINESS-STATE-DYNAMIC-PUBLISH-FACADE_验证记录.md
```

不得修改 P13—P16 provider/DTO/规格/摘要/账、P29—P31、P34、P40、P43、P15、旧P1C服务、运行期装配或其它所有者WIP。白名单外出现正式构造调用点时停止并退回计划支撑，不得漏改或保留半装配构造。

## 3. 固定接口与所有权

精确新增接口：

```cpp
状态动态原子发布结果 发布状态并整理动态(
    const 状态动态原子发布请求& 请求) const;
```

第四写唯一合法函数体：

```cpp
return 发布实际存在I64后继状态与迁移动能(
    L1_, 实例特征_, 状态_, 比较_, 动态_, 请求);
```

构造在P44四引用尾部追加 P16 所需五个必需引用，形成唯一九引用构造。不允许额外入口、DTO、校验、映射、兼容构造、指针、optional、setter、函数对象或服务定位器。

## 4. 机械施工顺序

### S0：正式接口、构造和所有权重绑定

1. 核对正式HEAD、plan blob、可执行状态、唯一活动代码段、十一文件白名单和空index。
2. 读取P32/P35/P41/P44实际世界树类、六根、四引用构造和全部构造调用点，逐签名核对。
3. 读取经计划支撑修订并被交互接受的P16实际模块、请求、结果和唯一公开函数，确认五个服务实参与异常边界。
4. 核对P13 v0.2、P14 v0.2、P15当前正式结果已由P16修订版吸收；门面不得自行适配其下层版本。
5. 确认顺序460/470/480/500/510/520/530/540链实际匹配且540未占用。
6. 确认P16、P44及届时活动计划释放全部目标文件和构建、自检、模块扫描资源。

任一接口、构造调用点、顺序、物理文件、plan blob或所有权漂移，停止并退回计划支撑。

### S1：构造与第四写门面

1. 在`服务.世界树.ixx`只引入P16公开服务模块，不引入其私有摘要、规格或参与者模块。
2. 用详细设计九引用构造替代P44四引用构造，不保留overload或空provider。
3. 保持既有六根源码语义不变，新增一行直接委托第四写。
4. 静态断言世界树与P16合同版本及七个公开函数签名和特殊成员状态。

### S2：专项、工程与记录

1. 四个既有世界树专项只补入真实P16必需 fixture 引用，原矩阵不变。
2. 新建顺序540专项，覆盖详细设计第7节；无合规测试入口的P16成功分支按P16专项与源码闭包分账，不伪造provider。
3. 工程/filters只登记新专项；启动只增加import、薄包装和顺序540项，不接生产消费者。
4. 形成专属施工/验证记录，登记构造调用点、签名、调用计数、结果全等、既有六根回归和静态扫描。

## 5. 禁止实现

```text
禁止重复实现、拆分或包装P13/P14/P16状态动态算法
禁止门面解析前后状态、比较、迁移动能角色、幂等、摘要、写集、读回或失败见证
禁止门面调用P13/P14/P15/P9/P11、L1提交、仓库、SQL、材料或旧栈
禁止保留四引用构造、空P16依赖、未实现第四写、setter或兼容overload
禁止第二provider调用、快照预读、结果重组、缓存、日志、retry、fallback或try/catch改判
禁止顺带设计移动、移除、治理路由、动作动态、因果、装配、旧入口退出或STEP-5
```

## 6. 激活门禁

保持`待激活`，直至：

1. P32/P34/P35/P40/P41/P42/P43/P44真实结果进入正式main并经交互接受，六根、四引用构造和既有专项实际匹配；
2. P13 v0.2、P14 v0.2、P15当前正式结果进入main并经交互接受；
3. P16由唯一计划支撑所有者完成当前依赖版本修订，修订版真实结果进入main并经交互接受，实际类/DTO/函数/异常与本叶机械匹配；
4. P16/P44释放全部白名单、工程/filters/启动、构建、自检和模块扫描资源；
5. 新HEAD扫描确认世界树服务全部构造调用点在白名单内，540未占用，旧P1C-F/SDW未施工且无等价第四写；
6. 执行侧从新HEAD重做完整S0并确认本计划为唯一可执行代码段。

冻结合同只支持设计推演，不证明本计划当前可执行。若P16修订改变公开接口，本计划先退回计划支撑同步修订，不允许门面临场适配。

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

成功条件：P32三读和前三写不退化；P45专项证明九引用构造、第四写签名、P16一次委托、结果全等和零旁路；完整自检两配置退出0；strict、双配置模块扫描、工程/filters一致性、目标与cached diff check通过。

## 8. 停工与完成声明

若P16修订版实际接口不匹配、出现白名单外构造调用点、540已占用、必须保留空依赖/兼容构造、需要修改provider/装配/旧入口，或验证持续无法归因，停止并退回计划支撑。

完成后只可声明：P45白名单内源码候选使唯一世界树业务服务增加发布状态并整理动态的第四写透明门面，并保持P32三读和前三写不变。

不得声明：P13—P16或P32—P44已实现、动作动态、移动/移除、治理白名单、生产装配、运行期接线、持久恢复、旧入口退出、集成验收、STEP-4、STEP-5或治理闭环完成。
