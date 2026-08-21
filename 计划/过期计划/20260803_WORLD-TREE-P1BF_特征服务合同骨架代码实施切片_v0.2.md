# WORLD-TREE-P1BF 特征查询服务合同骨架代码实施切片 v0.2

计划身份：WORLD-TREE-P1BF

版本：v0.2

创建侧仓库基线：`e35526db7719dfa50297ab0f1046e8acb0222ed6`

## 1. 目标、完成条件和依赖

在 P1A3F 唯一共享事实头上追加完整特征事实，建立节点直接特征查询的最终公开 DTO、构造和安全 `未实现` 骨架，并以顺序200接入正式完整自检。

完成条件：查询函数对任意输入稳定返回未实现空结果；零依赖调用、零机器事实变化；共享类型唯一；构造、模块、工程和顺序200自检真实编译接入。

具名依赖：

1. WORLD-TREE-P1A2 最终成功代码提供 `节点直接结构查询服务` 和本计划引用的 L1 稳定值；P1A2 v0.3 及其执行结果以开工时正式索引和代码为准。
2. WORLD-TREE-P1A3F v0.2 成功代码已经唯一建立 `海中鱼巣/领域/世界树事实.数据.h` 及顺序180自检；若 P1A3 已原位替换 P1A3F，则核对同一共享头和顺序180身份，本计划只追加自己的一个共享结构。
3. WORLD-TREE-P1A2Q v0.2 成功代码已占用顺序190并使正式自检总数为20。

任一依赖仅有设计/WIP/消息或实际签名不同，本计划保持待激活，S0零代码修改返回 `DRIFT-WORLD-TREE-P1BF-PROVIDER`。

唯一施工依据：

- `规范/4170_子规范_特征批次发布记录与幂等账.md` v0.8；
- `规范/4201_子规范_世界树合同骨架与未实现结果.md` v0.2；
- `规范/详细设计/20260803_WORLD-TREE-P1BF_特征服务合同骨架详细设计_v0.2.md` 全文；
- `流程图/20260803_节点直接特征查询服务读取宿主组完整当前特征_函数流程图_v0.2.md`。

## 2. 允许文件

```text
海中鱼巣/领域/世界树事实.数据.h
海中鱼巣/领域/服务.节点直接特征查询.ixx
海中鱼巣/领域/自检.节点直接特征查询合同骨架.ixx
海中鱼巣/启动.应用程序.ixx
海中鱼巣.vcxproj
海中鱼巣.vcxproj.filters
施工记录/20260803_WORLD-TREE-P1BF_施工记录.md
验证记录/20260803_WORLD-TREE-P1BF_验证记录.md
```

共享头只允许追加详细设计 §3 的 `世界树特征事实`；P1A3F/P1A3 已有字段零修改。其它路径均禁止。

## 3. 禁止范围

- 禁止实现真实特征查询、特征写入、批次账、幂等、事务、恢复、排序、字段校验或日志。
- 禁止修改 P1A2/P1A3F/P1A3/P1A2Q 服务、仓库、事务、查询或自检。
- 禁止 import/调用旧 `服务.特征`、`数据操作.特征体系`、旧特征参与者、SQL、材料或旧句柄接口。
- 禁止新增重载、兼容壳、默认成功、空成功、未找到回退、仓库/许可/会话/锁/句柄公开字段。
- 禁止重新加入不存在的 `来源关系`；来源只能由后继真实查询从类型化值读回的节点来源形成。
- 禁止让任何消费者因骨架存在解除 WORLD-TREE-P1A2G / WORLD-TREE-FEATURE-Q 真实成功依赖。

## 4. 机械施工步骤

1. S0 核对三个依赖结果、共享头逐字段、结构查询服务签名、顺序180/190与20项自检；不满足零修改收口。
2. 只在共享头末尾追加 `世界树特征事实`，字段逐字等于详细设计 §3。
3. 建立查询模块和最终构造；按查询图实现单节点未实现返回，函数体对保存依赖零调用。
4. 对详细设计固定的 prvalue 返回表达式加入 `noexcept(...)` 静态断言；函数固定 `noexcept`，不得增加命名临时值或 catch-all。
5. 建立唯一骨架自检，覆盖详细设计 F01—F07；以顺序200从20项扩为21项。
6. 更新 vcxproj/filters，保持共享头先于查询服务、自检最后的模块拓扑。
7. 形成专属施工/验证记录，逐项登记构建、自检、静态扫描和未覆盖范围。

## 5. 失败收口与验证

接口/模块/共享头/顺序漂移时零代码修改返回具名漂移；需要真实行为、旧栈适配、额外共享字段或特征写入服务时停止，不建立临时壳。任一验证失败保留WIP，不声明完成。

```powershell
git diff --check
python .\tools\check_specs.py --strict
msbuild .\海中鱼巣.vcxproj /m /p:Configuration=Debug /p:Platform=x64
msbuild .\海中鱼巣.vcxproj /m /p:Configuration=Release /p:Platform=x64
.\x64\Debug\海中鱼巣.exe --self-test-exit
```

模块依赖与目标函数零调用边使用现行 Clang AST 工具机械验证；输出只能写系统临时目录：

```powershell
$依赖 = Join-Path $env:TEMP 'WORLD-TREE-P1BF-模块依赖.json'
$候选 = Join-Path $env:TEMP 'WORLD-TREE-P1BF-调用候选.json'
.\tools\clang_ast\扫描.MSVC模块依赖.ps1 `
  -配置 Debug -平台 x64 `
  -输出路径 $依赖
$依赖材料 = Get-Content -Raw -LiteralPath $依赖 | ConvertFrom-Json
if ($依赖材料.failed_count -ne 0 `
    -or @($依赖材料.cyclic_modules).Count -ne 0 `
    -or @($依赖材料.unresolved_requirements).Count -ne 0) {
    throw 'P1BF MSVC模块依赖未闭合'
}
.\tools\clang_ast\扫描.Clang模块候选流程图.ps1 `
  -模块依赖路径 $依赖 `
  -目标模块 '海中鱼巣.领域.服务.节点直接特征查询' `
  -输出路径 $候选
$材料 = Get-Content -Raw -LiteralPath $候选 | ConvertFrom-Json
$文件 = @($材料.files | Where-Object { $_.module -eq '海中鱼巣.领域.服务.节点直接特征查询' })
if ($材料.status -ne '成功' -or $文件.Count -ne 1 -or $文件[0].status -ne '成功') {
    throw 'P1BF AST目标模块未完整成功'
}
$函数 = @($文件[0].functions | Where-Object {
    $_.name -eq '海中鱼巣::节点直接特征查询服务::读取宿主组完整当前特征'
})
if ($函数.Count -ne 1) { throw 'P1BF目标根函数身份不唯一' }
$直接调用 = @($文件[0].calls | Where-Object { $_.caller -eq $函数[0].candidate_id })
$未解析调用 = @($文件[0].unresolved_calls | Where-Object { $_.caller -eq $函数[0].candidate_id })
if ($直接调用.Count -ne 0 -or $未解析调用.Count -ne 0) {
    throw 'P1BF骨架根函数存在调用边'
}
```

另以 `rg` 扫描服务文件不得出现旧特征模块、句柄、SQL、仓库、数据操作、许可或锁；`结构_` 只允许出现在构造初始化和成员声明。AST 兼容失败不得伪报代码漂移，但本计划的零调用门禁在 AST 未完整成功时视为未通过，不得改用人工忽略命中后宣称 PASS。

## 6. 完成声明边界

完成只声明 P1BF 查询合同骨架通过编译接入、入口巡检和安全未实现矩阵。不得声明真实特征查询/写入、4170新域批次、恢复、世界快照、统一门面或生产服务验收完成。




本 v0.2 只替换已删除目标业务图依据并改用详细设计/函数图 v0.2；旧 v0.1 plan blob `be808e2f2e76988e662abdc601deb828efbcb84f` 仅作历史基础，不复用其执行许可。其余依赖、白名单、顺序200、零调用和验证合同不变。
