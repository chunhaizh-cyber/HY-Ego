# SECONDARY-RELATION-CONCEPT-STRUCTURE-P1 验证记录

日期：2026-09-23

状态：计划v0.4要求的可执行验证通过；代码结果与证据已提交并普通推送

验证基线：`dbce8ca85c86758bb60fad1eb62d844e761cde55`加RC-P1当前未提交WIP

计划：v0.4 / `144ff9643621478004d4b113cbb748d6c4b997fb`

## 1. RC专项Debug与Release

隔离根：

```text
D:\TEMP\海中鱼巣\SECONDARY-RELATION-CONCEPT-STRUCTURE-P1\20260923-final
```

执行：

```powershell
MSBuild.exe 验证工具\二次关系概念结构专项验证.vcxproj /m /t:Rebuild /p:Configuration=Debug;Platform=x64 /p:OutDir=...\out-debug\ /p:IntDir=...\obj-debug\
MSBuild.exe 验证工具\二次关系概念结构专项验证.vcxproj /m /t:Rebuild /p:Configuration=Release;Platform=x64 /p:OutDir=...\out-release\ /p:IntDir=...\obj-release\
```

结果：

- Debug：Rebuild成功，0警告、0错误；运行退出码0，`PASS total=187`。
- Release：Rebuild成功，0警告、0错误；运行退出码0，`PASS total=187`。
- 覆盖有界历史组、53项结构、固定/当前K真实用量、FC/EC及嵌套值域真实用量、各物理轴少一项、规范化、建立/重放/同义复用、来源、用途、图、生命周期、外部引用和退出。

## 2. 真实双进程持久恢复

Debug受控根：

```text
D:\TEMP\海中鱼巣\SECONDARY-RELATION-CONCEPT-STRUCTURE-P1\20260923-cross-b
```

Release受控根：

```text
D:\TEMP\海中鱼巣\SECONDARY-RELATION-CONCEPT-STRUCTURE-P1\20260923-cross-release
```

每个配置均分两次独立启动：

```powershell
二次关系概念结构专项验证.exe --cross-process-create <受控根\store> <受控根\state.txt>
二次关系概念结构专项验证.exe --cross-process-read <受控根\store> <受控根\state.txt>
```

结果：

- 建立进程退出码0，`PASS cross-process-create total=8`。
- 恢复进程退出码0，`PASS cross-process-read total=32`。
- 恢复进程要求L1返回`已恢复`，重新签发同一owner，精确重放pure及RC结构，并核对25个RC结构稳定编码全部等于第一次进程记录。
- 该证据为两个真实进程先后访问同一持久根，不是单进程重新构造服务。

## 3. 绑定存在创建回归

隔离根：

```text
D:\TEMP\海中鱼巣\SECONDARY-RELATION-CONCEPT-STRUCTURE-P1\20260923-binding-final
```

结果：

- 首次Debug回归暴露验证工程未链接新增RC合同实现，产生两个未解析符号；该机械范围缺口已通过计划v0.4和提交`dbce8ca85`正式补齐。
- Debug：Rebuild成功，0警告、0错误；运行退出码0，`BOUND_EXISTENCE_OK 118`。
- Release：Rebuild成功，0警告、0错误；运行退出码0，`BOUND_EXISTENCE_OK 118`。
- 本回归只证明新增值域预算没有破坏既有存在绑定与两组概念路径。

## 4. 根工程Debug与Release

隔离根：

```text
D:\TEMP\海中鱼巣\SECONDARY-RELATION-CONCEPT-STRUCTURE-P1\20260923-root
```

结果：

- `海中鱼巣.vcxproj` Debug x64隔离Rebuild成功，0警告、0错误。
- `海中鱼巣.vcxproj` Release x64隔离Rebuild成功，0警告、0错误。
- 根工程构建包含RC合同、RC结构初始化、唯一概念服务及普通应用装配，不包含专项验证源。
- 根工程没有运行；构建通过不证明生产业务入口已执行RC形成或动态概念。

## 5. 静态与范围审计

- `git diff --check`和`git diff --cached --check`通过；只有行尾转换提示。
- 当前RC-P1允许范围包含26项dirty（含v0.4新增验证工程）；三份范围外dirty均为自我线程目标业务流程图，发布时明确排除。
- `.ixx`、`export module`、`module;`及C++模块导入零恢复。
- 新增RC合同与初始化的四个`.h/.cpp`在根工程和filters中各登记一次；专项源未进入根工程。
- 未发现第二概念owner、独立RC仓、持久二次实例、RC求值服务、RC形成应用或DC实现误入。
- `概念树规则身份/读取头/写入头`与`有界事实读取预算_B1/用量_B1`各只有一份完整定义。
- 旧概念根、旧CRUD、旧定义联合、旧构造链和兼容转发零恢复。

## 6. NOT_RUN与证据边界

- 故障注入、L1提交结果未知和资源失败动态注入：`NOT_RUN`。当前计划范围内没有合法注入入口；未新增生产故障seam。
- 当前验证证明RC结构owner、真实物理读取用量、53项交付、恢复、普通应用结构装配及计划列出的静态边界。
- 当前验证不证明RC求值、RC形成、动态实例、动态概念、现实支持、自我线程治理或完整业务闭环。

## 7. Git状态

- 计划v0.4已提交并推送：`dbce8ca85`。
- RC代码结果提交：`873637e2552c3b59caf54b539962efb5b49b0250`。
- 记录回填提交：`b3004e01ee0931e892e7100de81c8b1e3454808f`；代码结果与记录回填均已进入`origin/main`。
- 三份非RC流程图WIP未修改、未暂存、未纳入上述证据。
