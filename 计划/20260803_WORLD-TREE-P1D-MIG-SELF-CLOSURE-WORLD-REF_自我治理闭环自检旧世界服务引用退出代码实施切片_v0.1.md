# WORLD-TREE-P1D-MIG-SELF-CLOSURE-WORLD-REF 自我治理闭环自检旧世界服务引用退出代码实施切片 v0.1

计划身份：WORLD-TREE-P1D-MIG-SELF-CLOSURE-WORLD-REF

版本：v0.1

创建侧代码基线：`b8dd29cdcda86fb88c0bee4a91467f5d637f817c`

直接依据：`规范/详细设计/20260803_WORLD-TREE-P1D-MIG-SELF-CLOSURE-WORLD-REF_自我治理闭环自检旧世界服务引用退出详细设计_v0.1.md`。

## 1. 目标和完成条件

在单一自检模块内删除旧 `世界服务` 的直接头文件、上下文字段和未使用局部别名。精确差异只有三处删除；运行时自检逻辑、旧世界初始化读数、报告和聚合不变。

本计划不删除 `世界服务.h`，不建立兼容壳，不迁移 `世界树初始化结果`，不修改统一门面、生产装配或其它消费者。

## 2. 前置和状态

本叶没有计划或提供者依赖。其它旧消费者迁移叶既不提供本叶所需接口，也不占用本叶唯一代码文件；迁移主题、文档顺序和创建顺序不构成依赖。统一门面真实行为、P1A3、P1B、P1C同样不构成本叶编译或运行门禁。

创建侧验证通过后登记为 `可执行`。执行侧仍须在 S0 中核对目标三点链、唯一活动代码段、Git index 和验证资源。

## 3. 施工依据

```text
流程图/20260803_WORLD-TREE-P1D-MIG-SELF-CLOSURE-WORLD-REF_自我治理闭环自检旧世界服务引用退出业务流程图_v0.1.md
流程图/20260803_运行自我治理闭环自检退出旧世界服务引用_函数流程图_v0.1.md
规范/详细设计/20260803_WORLD-TREE-P1D-MIG-SELF-CLOSURE-WORLD-REF_自我治理闭环自检旧世界服务引用退出详细设计_v0.1.md
规范/详细设计/函数结构知识图谱/20260803_WORLD-TREE-P1D-MIG-SELF-CLOSURE-WORLD-REF_自我治理闭环自检旧世界服务引用退出函数结构知识图谱_v0.1.md
```

## 4. 精确允许文件

```text
海中鱼巣/线程/自检.自我治理闭环.ixx
施工记录/20260803_WORLD-TREE-P1D-MIG-SELF-CLOSURE-WORLD-REF_施工记录.md
验证记录/20260803_WORLD-TREE-P1D-MIG-SELF-CLOSURE-WORLD-REF_验证记录.md
```

不修改工程、启动聚合、规范、设计、流程图、知识图谱、其它计划、计划索引或当前代码图谱。

## 5. S0

1. 核对正式计划版本/blob、索引状态和HEAD/main/origin；不读取其它迁移叶结果作为开工门禁。
2. 复算目标代码blob；创建基线为 `6a62610950517d0baf14246635ef46a073f5d775`。漂移时重新核对三点链。
3. 确认 `世界服务& 世界` 只有字段声明，局部 `世界` 只有一次绑定且无用途。
4. 确认全仓 `自我治理闭环自检上下文` 与 `运行自我治理闭环自检` 没有构造/调用点。
5. 确认目标文件无活动所有者，index和验证资源可用；记录当前顶层自检数量。

失败时返回 `DRIFT-P1D-MIG-SELF-CLOSURE-WORLD-REF-USAGE-OR-CALLER`，零修改收口。

## 6. 机械施工

1. 删除 `#include "../领域/世界服务.h"`。
2. 删除 `自我治理闭环自检上下文` 中 `世界服务& 世界;`。
3. 删除根函数中的 `auto& 世界 = 上下文.世界;`。
4. 不移动、不重排、不改写其它代码；形成专属记录。

## 7. 禁止

- 禁止删除 `世界服务.h` 本体、世界树初始化模块、服务引用或初始化读数。
- 禁止新增默认字段、指针、空服务、类型别名、兼容构造、包装结果或转发函数。
- 禁止因根函数当前无调用点而删除模块、根函数、自检报告或工程登记。
- 禁止改变任何验收分支、事实写入、报告字段、日志或输出。

## 8. 验证

```powershell
git diff --check -- 海中鱼巣/线程/自检.自我治理闭环.ixx
python .\tools\check_specs.py --strict
msbuild .\海中鱼巣.slnx /t:Rebuild /m:1 /p:Configuration=Debug /p:Platform=x64
.\x64\Debug\海中鱼巣.exe --self-test-exit
msbuild .\海中鱼巣.slnx /t:Rebuild /m:1 /p:Configuration=Release /p:Platform=x64
.\x64\Release\海中鱼巣.exe --self-test-exit
powershell -ExecutionPolicy Bypass -File .\tools\clang_ast\扫描.MSVC模块依赖.ps1 -配置 Debug -平台 x64
rg -n "世界服务\.h|\b世界服务\b|上下文\.世界\b|auto& 世界\b" 海中鱼巣/线程/自检.自我治理闭环.ixx
rg -n "世界树初始化结果|世界树初始化读数|世界树初始化服务" 海中鱼巣/线程/自检.自我治理闭环.ixx
rg -n "自我治理闭环自检上下文\s*[{(]|运行自我治理闭环自检\s*\(" 海中鱼巣
```

成功条件：第一组零输出；第二组仍命中全部保留链；第三组只命中类型和根函数定义；scoped diff仅三处删除；模块依赖不再含目标文件对世界服务头的直接文本包含；Debug/Release和两配置完整自检通过；strict通过；顶层数量不变。

## 9. 完成声明

完成只允许声明“自我治理闭环专项自检退出旧世界服务对象直接引用”。不得声明旧结果迁移、生产消费者迁移、旧头删除、P1D原子切换或服务验收完成。
