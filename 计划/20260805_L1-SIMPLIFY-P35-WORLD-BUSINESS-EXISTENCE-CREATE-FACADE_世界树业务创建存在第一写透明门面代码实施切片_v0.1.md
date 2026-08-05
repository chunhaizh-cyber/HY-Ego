# L1-SIMPLIFY-P35-WORLD-BUSINESS-EXISTENCE-CREATE-FACADE 世界树业务创建存在第一写透明门面代码实施切片 v0.1

日期：2026-08-05

状态：待激活

创建侧正式复核基线：`ba9dd59797db82f52bc5bf49c4de6a260fedd13d`

`06012409..ba9dd597` 只发布 P12 v0.3 三文档及其索引登记，与 P35 未来代码白名单和机器合同零重叠。

## 1. 目标与完成条件

原位扩展 P32 唯一 `世界树业务服务`，把构造从一个快照读取引用扩展为“快照读取 + P34当前场景实际存在发布”两个必需引用，新增第一写 `创建存在并接纳到场景`，函数体只调用 P34 同一请求恰1次并逐字段透明返回。

完成必须同时满足：

- P32 三读签名、const、异常和一对一委托完全不变；
- 单参数构造完整退出，双引用构造成为唯一合法构造；
- 第一写为const、非noexcept；请求/结果直接复用 P34，不新增 DTO 或状态映射；
- 门面第一写只调用 P34 一次，快照读取调用0；
- 门面不读取 L1/P7/P15/P17—P19/P33、不保存状态、不加锁、不捕获改判；
- P32三读专项、P35顺序480专项和完整验证全部通过。

## 2. 正式设计包与未来代码白名单

设计包：

```text
规范/详细设计/20260805_L1-SIMPLIFY-P35-WORLD-BUSINESS-EXISTENCE-CREATE-FACADE_世界树业务创建存在第一写透明门面详细设计_v0.1.md
流程图/20260805_L1-SIMPLIFY-P35-WORLD-BUSINESS-EXISTENCE-CREATE-FACADE_世界树业务创建存在第一写透明门面函数流程图_v0.1.md
计划/20260805_L1-SIMPLIFY-P35-WORLD-BUSINESS-EXISTENCE-CREATE-FACADE_世界树业务创建存在第一写透明门面代码实施切片_v0.1.md
```

未来代码白名单：

```text
海中鱼巣/领域/服务.世界树.ixx
海中鱼巣/领域/自检.世界树业务只读.ixx
海中鱼巣/领域/自检.世界树业务创建存在.ixx
海中鱼巣.vcxproj
海中鱼巣.vcxproj.filters
海中鱼巣/启动.应用程序.ixx
施工记录/20260805_L1-SIMPLIFY-P35-WORLD-BUSINESS-EXISTENCE-CREATE-FACADE_施工记录.md
验证记录/20260805_L1-SIMPLIFY-P35-WORLD-BUSINESS-EXISTENCE-CREATE-FACADE_验证记录.md
```

不得修改 P29—P31/P34 provider、P34数据头、旧P7组合器、旧WORLD-TREE服务、运行期装配、其它计划或所有者 WIP。若正式构造调用点超出上述白名单，停止并由计划支撑修订，不得漏改或保留半装配构造。

## 3. 施工步骤

### S0：正式接口与调用点重绑定

1. 核对 `main==origin/main`、0/0、index空、目标 plan blob、状态为 `可执行` 和唯一活动代码段。
2. 读取 P32 实际 `服务.世界树.ixx`、只读专项和全部构造调用点；确认三读与 v0.1 逐签名匹配。
3. 读取 P34 实际模块、类、请求、结果和公开函数；确认 provider 非const/非noexcept、门面可经非const引用在const函数中直接委托，且结果全等能力匹配。
4. 确认 P34 专项实际顺序470、P32顺序460，本叶顺序480无占用；读取当前完整自检数和 MSVC 模块数。
5. 核对八文件白名单与 P32/P34、工程、filters、启动、自检和模块扫描资源已释放。

任一接口、构造调用点、顺序、物理文件或所有权漂移，立即停止并退回计划支撑。

### S1：构造与第一写门面

1. 在 `服务.世界树.ixx` import P34 发布模块，不 import P34 的下层 provider。
2. 用详细设计第4节双引用构造替代 P32 单引用构造；删除单参数 overload，不增加指针、optional、setter、默认 provider 或兼容构造。
3. 保持三个读根源码语义不变，新增详细设计第5节一行直接委托函数。
4. 静态断言三个合同版本为1、四个公开函数完整签名和特殊成员状态。

### S2：专项、工程与记录

1. 只为 P32 只读专项补入真实 P34 fixture 引用，使原三读矩阵不变；不得把写测试塞入只读专项。
2. 新建 P35 顺序480专项，覆盖详细设计第7节；无 P34 测试 seam 的分支不得伪造运行覆盖，以真实入口拒绝运行和源码闭包分账。
3. 工程/filters只登记新增专项；启动只增加 import、薄包装和顺序480项，不接生产消费者。
4. 形成专属施工/验证记录，记录实际构造调用点、签名、调用计数、三读回归、静态扫描、构建和模块证据。

## 4. 激活门禁

本计划保持 `待激活`，直到：

1. P32 v0.1 真实结果进入正式 main 并经交互接受，三读、类、构造和顺序460实际匹配；
2. P34 v0.1 真实结果进入正式 main 并经交互接受，请求/结果/发布类/函数和完整结果合同实际匹配；
3. P32/P34 释放全部白名单和共享验证资源，P34专项正式使用顺序470；
4. 新 HEAD 全量扫描确认世界树服务构造调用点只在白名单内，且没有生产装配调用点需要本叶迁移；
5. 执行侧从新 HEAD 重做完整 S0，确认本计划是唯一可执行代码段。

P32/P34 的待实现冻结合同只支持本设计推演，不证明本计划现在可执行。

## 5. 验证命令与成功条件

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

成功条件：P32三读专项全通过；P35专项证明双引用构造、第一写签名、P34一次委托、结果全等和零旁路；完整自检两配置退出0；strict、双配置模块扫描、目标 diff check 和 staged 白名单全部通过。

## 6. 停工与完成声明

以下情况停工：P32/P34 实际接口不匹配；出现白名单外构造调用点；P34顺序不是470或480已占用；必须保留空 provider/兼容构造才能编译；需修改 provider/装配/旧入口；验证持续失败且无法在本计划内归因；Git或所有权冲突。

完成后只可声明：P35 白名单内源码候选使唯一世界树业务服务增加创建存在并接纳到场景的第一写透明门面，并保持 P32 三读不变。

禁止声明：P32/P34 本身由本叶实现、发布存在初始特征、其它五写、治理白名单、生产装配、运行期接线、旧入口退出、集成验收、STEP-4、STEP-5 或治理闭环完成。
