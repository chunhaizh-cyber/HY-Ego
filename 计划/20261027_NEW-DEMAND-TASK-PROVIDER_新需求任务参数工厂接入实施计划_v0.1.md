# 新需求任务参数工厂接入实施计划

标识：NEW-DEMAND-TASK-PROVIDER；版本：v0.1；日期：2026-10-27。

这是用户“初始化完善因果链”并已确认运行接续扩展的最早提供者叶，不是完整被动链完成声明。状态由[计划索引](计划索引.md)唯一维护；完整合同见[详细设计](../规范/详细设计/20261027_NEW-DEMAND-PASSIVE-CHAIN_任务参数提供者与后续被动链详细设计_v0.1.md)。

## 1. 目标、起点、所有权

起点`D:/海中鱼巣` main `a7fbb667cd701f88e7615d5a7509a6851caf4680`加root联合WIP，index为空；root唯一代码/构建/Gitowner。计划角色仅本新设计、计划及唯一索引项，不改其他材料。设计/计划/索引仅root本地精确提交不推送，生产代码/施工记录不提交不推送。

目标：新需求本能由真实任务X调用时，通过外层正式工厂建立scene需求能力并注入W3，保留参数唯一入口、正确A来源、fresh复核和join后生命周期。当前directK工厂不可用于X，不造K冒充X。实际操作H.F及完整D的原core不改；不做D通知/采用/因果初始化直到直接后继。

依据和当前事实：5360/5350/5260参数及外层执行职责；现W3场景无provider，runtime仅freshK，属于调用工程提供者缺口。完整签名、错误/租约及运行顺序由设计§2—4冻结。

## 2. 精确允许和禁止

允许修改仅6个生产文件（均有保留WIP）：

- `海中鱼巣/领域/本能需求.运行接口.h`：任务工厂请求、前提变化异常和三个纯虚方法，前置声明工厂scene结果类型。
- `海中鱼巣/领域/新_任务类.h/.cpp`：非拥有端口配置/卸载、W3在X成立后锁外场景工厂调用及fresh复核。
- `海中鱼巣/业务/服务需求.本能运行服务.h/.cpp`：task owner构造注入，支持实际M来源/实例的X工厂，来源类别化provider动作记录及freshK原路径保持。
- `海中鱼巣/装配.普通应用.cpp`：唯一runtime注入task及任务工厂绑定、join后RAII精确卸载/close。
- `验证记录/20261027_NEW-DEMAND-TASK-PROVIDER_施工与编译记录.md`：实施范围/hash/知识变更清单和实际编译证据。

禁止修改规范/知识/其它计划、工程登记、方法固定参数和头、共同核心、D字段/归档/反馈/采用、因果类/初始化/反查、模拟world/学习算法或已删除旧函数族。不扩作用范围、不绕过授权/安全/已冻结采用，不把来源X写入业务条件或伪造执行效果。

## 3. 步骤

1. S0核本计划精确blob、设计、当前HEAD/WIP/接口与资源，唯一代码owner认领允许块；原WIP不覆盖、不stash/reset/clean。
2. 按设计§2加入最小外层工厂DTO/三个纯虚方法及前提漂移异常，task新增配置/精确卸载，非拥有指针在task锁下管理，绝不持该锁调用provider。
3. runtime实现freshX工厂：核正式T/X/实际M与来源/冻结绑定和分支，X无A且当前位成立；同X唯一租约，失败释放计数。实际M可为合法实例，不机械等于抽象M。原freshK工厂保留。
4. provider按外层固定来源类别记录A：K走方法owner；X走task`记录本能动作身份(T,X,A,动态)`，后续W3同A幂等核验。A后core重入保护与两阶段publisherCAS原样保留。
5. W3在X成立后调用工厂`建立任务参数场景`，由runtime提供合法材料reader并生成sharedprovider；保留M/绑定/分支/治理owner/任务成员，替换局部scene。重新fresh核验后才真实调用。正式前提变化须task读回证明零动作，工程故障不得伪装条件不足。
6. app新需求登记完成后绑定工厂；RAII声明在runtime后、threads前，join后卸载/close，初始化失败只回收本app已成功绑定，不误销他人。移除有界等待后立即close新runtime的一行，其余生命周期不动。
7. 静态检查及统一实际工程编译链接，写专属记录；默认不运行测试。编译后沿同目标立即设计D结果交付直接后继，不提前登记因果/初始化多层库存。

## 4. 错误与完成

前提变化异常只表示X已不在当前执行位；W3须真实重读，确定零A才走现恢复路径。结构/资源/ABI错误走工程诊断，不生成业务负结果；动作建立/绑定失败保留实际A，不能自动重发。工厂支持M后不得空provider伪装成功。原函数不读X/T/TLS/用途，只用scene能力。生命周期严格按设计，运行场景sharedlease不延长task到其owner之外。

完成门：外层X工厂、W3真实调用点、同绑定scene注入、动作正确关联和app回收均已实现并实际项目编译链接；明确“未运行测试、未验证实际任务到达、未完成D通知/自我采用/通用关系因果/初始化/模拟writer”。不能将该叶退出当顶层目标完成。

## 5. 验证命令

```powershell
git diff --check
git diff --cached --check
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe' '海中鱼巣.vcxproj' /m /p:Configuration=Debug /p:Platform=x64 /p:OutDir=D:/TEMP/海中鱼巣/NEW-DEMAND-TASK-PROVIDER/20261027/bin/ /p:IntDir=D:/TEMP/海中鱼巣/NEW-DEMAND-TASK-PROVIDER/20261027/obj/ /fl /flp:logfile=D:/TEMP/海中鱼巣/NEW-DEMAND-TASK-PROVIDER/20261027/build.log
```

先核工具路径和专属输出无冲突；只使用实际根工程，不复制源码、不建临时工程/测试脚本、不启动程序。编译链接零错误，警告逐项记录。静态读回精确调用、正确来源、锁释放、生命周期和未改固定规格。文档门只做差异/链接检查，本叶代码结果尚未生成时不能提前写已编译。
