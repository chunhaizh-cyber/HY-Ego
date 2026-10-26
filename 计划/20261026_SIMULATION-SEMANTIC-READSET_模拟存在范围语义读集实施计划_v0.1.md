# 模拟存在范围语义读集实施计划

标识：SIMULATION-SEMANTIC-READSET；版本：v0.1；日期：2026-10-26。当前状态及精确版本见[计划索引](计划索引.md)；root精确本地提交、独立复核完成并重新S0后方可施工。

起点main `3826f008b3f877e908ff35f15ded9557ae9e4fb7`加root保留WIP，唯一合同为[详细设计v0.1](../规范/详细设计/20261026_SIMULATION-SEMANTIC-READSET_模拟存在范围语义读集详细设计_v0.1.md)。用户明确本叶只保护真正语义依赖、无关新引用不应使共享存在范围C失效，目标引用在L1发布点仍合法存续。

## 1. 目标和边界

解决模拟服务1351循环外C full快照被每样例C/R新增入向引用撞失效。增加当前L1中性只读字段/外部关系集合预期，存在owner输出定义/上位语义读集，facade、共享模板和隔离world同事务消费。不将定义/实际分类变化忽略，不通过刷新完整快照迁就结果。

范围限九文件：`海中鱼巣/核心/基础数据集.h/.cpp`、`海中鱼巣/领域/概念_存在类.h/.cpp`、`海中鱼巣/领域/新_方法类.h/.cpp`、`海中鱼巣/领域/模拟需求.数据.h`、`海中鱼巣/业务/模拟学习.服务.cpp`、`海中鱼巣/领域/新_模拟世界类.cpp`。新DTO/函数及状态、封存/组合、字段来源和失败边界见详细设计§3—7；无owner/代次或旧4080服务迁移。

源E/F/U/C/R、语言/原型实际关系以及方法/分支/每组合fresh消息full保留；FT/二次等只有本C重叠需同次分账，其余未收敛明确记录，不扩全仓CAS、任务目标、方法规格或学习算法。

## 2. 机械实施

1. S0核九文件接口/旧保护用途及WIP，root唯一代码/Git/构建owner；不碰其它owner的活动切片。
2. L1实现冻结只读selector及共用校验，追加在同锁交换前验证；封存/组合全链保留新guard，只读不取得写/删/替换或外边资格，旧ABI默认空不变。
3. 存在owner直接语义解码及字段/role/root登记读集，不先用旧full保护reader准入再裁。未知字段/非法端点工程错误，真实前提变化零载荷；facade只转发。
4. 模拟服务分开源事实full与共享定义，集中替换本C循环外full、同语义核同并合并guard；覆盖重复加入与最终模板重建，不按编码全局删源事实保护。
5. 输入图选择传递guard，world合入同一原子请求；私有写和真实引用存续保持，必要full不降级，不自动给纯只读目标补旧full。
6. 源码静态矩阵及九文件实际编译链接，记录输入blob、实际变化、未运行范围与残余保护债；不声称模拟学习已实测完成。

## 3. 验证及发布

文档`git diff --check`、`git diff --cached --check`和路径读回；实际项目统一Debug/x64编译。当前仅实施，无程序启动、测试、性能测量、专用脚本/工程授权。

```powershell
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe' '海中鱼巣.vcxproj' /m /p:Configuration=Debug /p:Platform=x64 /p:IntDir=D:/TEMP/海中鱼巣/SIMULATION-SEMANTIC-READSET/20261026/obj/ /p:OutDir=D:/TEMP/海中鱼巣/SIMULATION-SEMANTIC-READSET/20261026/bin/ /v:minimal
```

工作目录`D:/海中鱼巣`，root独占实际项目输出。记录`验证记录/20261026_SIMULATION-SEMANTIC-READSET_施工与编译记录.md`及知识变更清单。源码/工程/记录不提交推送；本设计/计划/后续本索引项仅root精确本地提交、不推送。无关WIP保留，不新建远期计划。设计漂移向root具名返回，不能为了编译删掉真实依赖或扩大泛读集能力。
