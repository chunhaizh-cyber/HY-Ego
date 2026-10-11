# 实际项目模拟算法自检

日期：2026-10-11；身份：SIMULATION-ALGORITHM-SELFTEST v0.1。

计划发布`cd90bd99`，索引发布及代码起点`84455930`。结果代码为包含本记录的Git提交。

## 实际变更

新增`业务/自检.模拟学习.h/.cpp`；实际工程与filters登记；启动模式追加数值模拟自检3，解析`--self-test-simulation`；main在生产装配前分派一次自检调用。

测试数据仅局部纯值对象，无世界/任务/方法写入，无外设或治理线程启动。数值模拟算法公开接口和实现均未修改。

## 输入与所有权

main起点与origin/main一致，index为空；构建前无其它MSBuild/cl/link/海中鱼巣进程。代码、工程增量、构建及发布由`/root`独占。

实际工程消费了主线已有结算WIP：自我/管理线程、装配、任务/动态/方法/参数场景/本能函数/管理头源、需求实现及方法公共类型；包括未跟踪服务值结算本能运行服务/合同。异主目标流程图及数据目录不改、不提交。本结果是联合WIP构建与实际进程运行，不是纯提交基线集成验收。

## 构建与运行

```powershell
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe' `
 'D:/海中鱼巣/海中鱼巣.vcxproj' /t:Build /p:Configuration=Debug /p:Platform=x64 `
 /p:OutDir=D:/TEMP/海中鱼巣/SIMULATION-ALGORITHM-SELFTEST/20261011-v0.1/Debug/bin/ `
 /p:IntDir=D:/TEMP/海中鱼巣/SIMULATION-ALGORITHM-SELFTEST/20261011-v0.1/Debug/obj/ `
 /m:2 /nologo /verbosity:minimal /fl `
 '/flp:logfile=D:/TEMP/海中鱼巣/SIMULATION-ALGORITHM-SELFTEST/20261011-v0.1/Debug/build.log;verbosity=normal'
& 'D:/TEMP/海中鱼巣/SIMULATION-ALGORITHM-SELFTEST/20261011-v0.1/Debug/bin/海中鱼巣.exe' --self-test-simulation
```

构建退出0，0警告0错误，耗时00:01:01.33。运行退出0，汇总`SIMULATION_SELFTEST PASS=21 FAIL=0`。自检cpp blob `70c9c60dce25efebd3057f50a7a1593b52cb531c`。

通过的组：argument-parser、initialization-pause-resume、single-point、adjacent-points、identity-five-points、affine、odd-width、fractional-deviation、quadratic、midpoint-hidden-deviation、cartesian-bilinear、multiple-outputs、wide-identity、wide-constant、wide-descending、failed-results、output-dimension、infinite-output、callback-exception、callback-bad-alloc、reentrancy。

另实际启动重复自检参数退出2；`--headless --self-test-simulation`冲突退出2。未无参数启动生产应用。

每个成功用例核验真实callback次数、输入唯一性、返回增量唯一、独立公式输出、样例与区域计数、进度和终态不重放。异常用例是callback主动抛bad_alloc，不是实际容器内存分配故障注入。

## 边界及残留

Release/Win32、真实内存故障、持久恢复、模拟世界隔离、方法配对发布和学习线程均未验证。通过的21组不是穷尽所有输入，也不证明完整模拟学习闭环。

本轮具名输出目录保留构建和运行日志及实际工程产物；未复制源码或另建项目。清理所有者为`/root`，前轮同类清理遭工具策略拒绝，本轮未绕过该限制。旧工作区WIP全部保留。

## 函数级代码知识变更

- 修改`解析并验证启动选项(int,char*const[]) noexcept`：只增加新模式解析，复用重复/冲突拒绝；不触发运行。
- 修改`main(int,char*[])`：解析成功后调用具名自检入口，其它生产路径不变。
- 新增`int 运行模拟学习算法自检() noexcept`：唯一自检汇总/异常最终边界，返回0仅全部通过。
- 私有`要求`、`数值`、`推到终态`、`检验完成`、`检验失败`、`检验解析`、`检验初始化`、`检验重入`仅服务自检；调用公开细分器或启动解析，不取得生产私有状态。

共享知识用途卡待维护，不修改知识索引。发布范围仅本轮五个头源/入口文件、启动选项及工程精确新增项与本记录；不带入旧结算登记。
