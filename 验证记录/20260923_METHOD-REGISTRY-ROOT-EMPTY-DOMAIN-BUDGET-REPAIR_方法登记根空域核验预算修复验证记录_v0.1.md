# METHOD-REGISTRY-ROOT-EMPTY-DOMAIN-BUDGET-REPAIR 验证记录 v0.1

日期：2026-09-23

计划：`METHOD-REGISTRY-ROOT-EMPTY-DOMAIN-BUDGET-REPAIR` v0.1 / blob `3587326598e2f3c8912ed65158763116b864b80e`

验证代码起点：`8d89eeb6bd236c935a88a18674406f777b6e3ba6`

结果提交：待本切片提交形成；以本记录所在提交为准。

## 1. 验证输入与隔离

- 隔离轮次：`D:\TEMP\海中鱼巣\METHOD-REGISTRY-ROOT-EMPTY-DOMAIN-BUDGET-REPAIR\R1-20260923-151728`
- 源码根：上述目录下`source`。
- 基线：由`git archive HEAD`导出的正式HEAD，只覆盖本计划四个生产文件和两个专项文件。
- 构建工具：`C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe`。
- 输出：上述轮次下`out\Debug`、`out\Release`；中间文件在`int\Debug`、`int\Release`。
- 持久恢复最终用例：`persistent-final3\Debug`与`persistent-final3\Release`。
- 未使用共享工作区根工程、阶段21专项、普通应用、启动源码或异主DATA-L1 WIP作为验证输入。

## 2. 构建命令

两配置分别执行：

```text
MSBuild.exe 验证工具\方法登记根空域核验预算专项验证.vcxproj /t:Rebuild /m:1 /p:Configuration=<Debug|Release> /p:Platform=x64 /p:OutDir=<隔离输出> /p:IntDir=<隔离中间> /nologo /verbosity:minimal
```

结果：Debug x64、Release x64均零错误完成。项目把两个公开头各自作为独立C++20翻译单元编译，并同时编译专项源及两个生产`.cpp`；启用`/utf-8 /bigobj`并链接`bcrypt.lib`。

## 3. 运行矩阵

| 编号 | 结果 | 实际证据 |
| --- | --- | --- |
| V01 错版本/预算0 | PASS | L2与provider均返回`入口拒绝`，根为空且调用前后G不变；编译期同时证明旧三字段L2聚合请求的末尾预算为0。无公开调用计数器，未调用空域读由入口早退源码静态复核，未增加生产seam。 |
| V02 多事实仓库预算少1 | PASS | 夹具实际成功发布4个当前节点，关系/值/历史/墓碑均为0；溢出保护求和得到N=4。预算3时L2与provider均返回`数量预算不足`，`H=0`、根为空、G不变。 |
| V03 精确足够预算 | PASS | 同一夹具预算4时provider返回`已初始化`，形成唯一根，只推进一次事实代次。 |
| V04 不同预算重复 | PASS | 根建立后以预算1调用，返回`精确重复`，根身份和截止不变，零新增事实；证明重复路径不重新要求全仓空域预算，预算不进入持久幂等材料。 |
| V05 owner未知结构 | PASS | 方法owner先写入另一幂等身份的普通节点；直接L2返回`所有者非空冲突`，`H=0`、根为空、G不变。 |
| V06 事实代次漂移 | PASS | 读取旧G0后由其它owner发布事实，再以旧G0调用L2；返回`事实代次漂移`并回显新截止，零写。 |
| V07 两进程seed/recover | PASS | Debug与Release各自独立执行seed和recover；seed根编码均为7，recover读回同一编码7并证明入口G等于结束G，恢复零写。 |
| V08 独立头翻译 | PASS | 两个公开头在Debug/Release x64工程中分别作为独立C++20翻译单元编译通过；普通应用式`方法登记根生产初始化请求{1}`由编译期断言取得唯一生产默认。 |
| V09 静态扫描 | PASS | 四个生产文件中具名默认定义1处、默认数字字面量1处、最大整数用法0处、L1空域调用1处；未发现自动倍增、第二默认、空域绕过或预建根。 |
| V10 Debug/Release x64 | PASS | 两配置均Rebuild通过；每配置内存专项9项、seed 2项、recover 4项全部PASS。 |
| 资源耗尽/私有损坏 | NOT_RUN | 无合法公开注入；按计划不新增生产测试开关或私有破坏入口。 |

## 4. 最终运行输出摘要

Debug与Release输出一致：

```text
memory: PASS total=9
seed:   ROOT=7, PASS total=2
recover: ROOT=7, PASS total=4
```

覆盖名称包括：入口拒绝、夹具写集预算推导、L2预算差一、provider预算差一、精确预算建立、不同预算精确重复、未知结构冲突、旧G0漂移、持久seed、持久recover同根及恢复零写。

## 5. 差异与静态检查

- `git diff --check -- <本计划六个代码/专项文件>`：PASS；生产文件仅有Git换行符提示，无空白错误。
- 生产扫描：`DEFAULT_DEFINITIONS=1`、`DEFAULT_LITERAL_OCCURRENCES=1`、`MAX_INTEGER_OCCURRENCES=0`、`EMPTY_DOMAIN_CALLS=1`。
- 专项工程未加入根工程；`海中鱼巣.vcxproj`与filters的现有dirty属于异主WIP，本切片未修改。
- DATA-L1、普通应用、启动、阶段21和RC-P1文件未纳入本切片暂存或隔离覆盖。

## 6. 验证过程中的一次失败

第一次Debug恢复试跑在`fixture node publication failed`停止。根因是恢复进程用新的G0重新提交seed阶段的填充事实，导致完整幂等请求不一致；这是专项夹具错误，不是生产预算链失败。专项改为恢复进程只重签既有owner、直接经provider读回既有根后，重新执行Debug/Release完整矩阵并全部通过。

## 7. 结论边界

验证证明本计划隔离输入上的显式预算、预算不足零写、精确预算建立、预算不进入重复幂等、未知结构冲突、旧G0漂移和两进程恢复合同成立。未构建根工程，未运行普通应用正式启动，未验证阶段21、自我线程或内部治理闭环，也不证明生产默认对未来任意规模永不耗尽。
