# 动作参数类别引用实施计划

标识：ACTION-PARAMETER-CATEGORY-REFERENCE；版本：v0.1；日期：2026-10-26。状态见计划索引；root已完整复核并批准本地激活，精确本地提交及S0由root完成，不推送。

基线main `c447ac0f3f2bdd3f816490270882b9b9113933c9`加root保留WIP；现动态概念cpp blob `ba3f6300c87155091a54ccb540eca87697be1928`。唯一合同为[详细设计v0.1](../规范/详细设计/20261026_ACTION-PARAMETER-CATEGORY-REFERENCE_动作参数类别引用详细设计_v0.1.md)。

## 1. 目标和范围

只将新建有序动作概念参数改写为v3类别根引用：`[-1,3]`数字流保留序号，引用流为类别根＋范围。原无前缀角色/v2读取、同义DTO复用和旧C身份保持。无公共ABI、新schema、D/T/M参数、模拟或动作业务扩展。

仅允许`海中鱼巣/领域/概念_动态类.cpp`，记录`验证记录/20261026_ACTION-PARAMETER-CATEGORY-REFERENCE_施工与编译记录.md`；root唯一代码/Git/构建owner。其它源码、规范、工程、索引旧项、历史数据及共享知识禁止写。源码/记录不提交推送；新设计/计划仅root精确本地提交、不推送。

## 2. 机械施工

1. S0核基线/WIP、类别根实际提供者和投影调用点，复用现DTO/owner，不新建底座接口或单方法专属分支。
2. 依设计§3实现v3新写及三种严格读；新增私有类别proof映射/解析，范围准入原样。
3. 提取可传播`动作类别前提变化`的共享动作读取核心，getter不伪空，保护入口直接走核心后完整重核，映射原前提已变化空载荷。
4. 原完整DTO查重兼容旧格式；新C改现原子追加组＋完整联合根proof，根边role0及字段顺序不变。前提变化/预期不符映射现状态不一致(8)＋空C/空增量，不新增状态或业务失败。
5. 发布后重采根proof/C保护，不拿旧入向快照自证；全部字段/边回执验证后才交完整新增增量。后置变化保留已发表C但零载荷，后续同输入查重，不自动重做动作。
6. 核现删除/复用/保真复制边界及投影消费者，记录静态矩阵；统一实际工程编译，不能以编译证明真实动作或学习成功。

## 3. 验证与完成

文档`git diff --check`、`git diff --cached --check`和计划/设计路径读回。静态矩阵及所有新旧字段/控制分支见设计§6。当前只有实施指令，不启动程序、不运行测试、脚本或专用工程。

真实项目工作目录`D:/海中鱼巣`；经root确认此前构建目录无占用后复用已完成叶中间产物，只作实际项目增量编译：

```powershell
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe' '海中鱼巣.vcxproj' /m /p:Configuration=Debug /p:Platform=x64 /p:IntDir=D:/TEMP/海中鱼巣/FEATURE-CATEGORY-PARAMETER-REFERENCE/20261026/obj/ /p:OutDir=D:/TEMP/海中鱼巣/ACTION-PARAMETER-CATEGORY-REFERENCE/20261026/bin/ /v:minimal /fl '/flp:logfile=D:/TEMP/海中鱼巣/ACTION-PARAMETER-CATEGORY-REFERENCE/20261026/build.log;verbosity=normal'
```

记录复用IntDir实际输入、源码blob、命令/exit及产物。输出/日志独占，不把旧产物当新编译结果；不拷项目或改工程。报告“已编译、未运行测试”，生命周期/并发CAS/正常动作和恢复未实测。设计漂移具名回报root，不扩大到公共DTO或其它类别/业务链。
