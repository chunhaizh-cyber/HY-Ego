# 控制采集协作停止实施计划

标识：`CONTROL-COLLECTION-COOPERATIVE-STOP`；版本：v0.1；日期：2026-11-01。

创建侧审查已通过：root全文读审并核对当前源码批准；执行状态及精确blob由[计划索引](计划索引.md)登记，精确本地发布后重新S0方可施工。上位[服务主线目标](../目标/活动/GOAL-SERVICE-MAINLINE-END-TO-END-REPAIR.md)保持active及完整范围。本叶只闭合阶段1的私有控制采集协作取消，不宣称解决单次owner或整体硬实时停止。

## 1. 合同、基线及所有权

唯一施工合同为[详细设计v0.1](../规范/详细设计/20261101_CONTROL-COLLECTION-COOPERATIVE-STOP_控制采集协作停止详细设计_v0.1.md)，精确私有类型、函数、边界、缓存许可点及残余范围均以该文为准。main `5bee0908a677b3de5f864e56c851c447d269e268`加root的264项联合WIP（含本两份新文档）；origin/main `c3b12234181fac0da3eb0224c36a14c239df4355`；登记前index为空，目标cpp起点blob `b562c4ec895ad123d450cfcd9190d6c88e61562a`未变。

root唯一代码/Git/构建/运行/浏览器/固定日志owner；root确认第十二轮PID46108于2026-11-01 00:49:13自行退出0且无残留，运行资源已释放，但采集join64秒，不能称有界停止已解决。精确本地发布后再S0，不覆盖联合WIP。计划角色仅维护本两文档及索引新增本项，不动既有任一行，不写目标、日志、代码、记录或Git。root允许本两文档及本项索引精确本地发布，禁止推送或带入生产/诊断/记录/目标WIP，由root持发布租约串行办理。

正式依据与复用裁决见设计第2节：0050、8110、8100/8120、CODE-FILE-01、原三份控制面板设计及STOP-BROADCAST/STOP-AWARE现合同。token取自原jthread，不借用业务取消；当前末尾已有停止后禁发表，新增的是采集内部早退。

## 2. 精确施工范围及不变项

唯一允许生产文件`海中鱼巣/装配.普通应用.cpp`：私有取消tag/variant及检查函数、token版字符串转义、原投影存token/私有正常体/捕获取消入口、全部具名helper边界和唯一采集线程消费者分流。显式补`<stop_token>/<variant>`，不新增工程文件或公开合同。

禁止修改领域h/cpp及重载、装配公开h、工厂/宿主/页面/缓存getter、普通停止/析构/最终join、5000ms/2000ms、业务治理/查询范围、日志设施、规范/规则/共享知识/目标/其它计划。专属施工记录只由root新建`验证记录/20261101_CONTROL-COLLECTION-COOPERATIVE-STOP_施工与编译记录.md`，含知识变更清单，不反写共享用途卡。

## 3. 机械实施顺序

1. S0复核审查批准、索引精确blob、当前HEAD/cpp/WIP/唯一owner及已释放运行资源；新漂移只停受影响范围，不清理其它WIP。
2. 原采集结果邻近增加私有取消tag/variant和检查函数，原一参转义共享token版算法；不复制JSON算法，取消不输出半字符串。
3. 私有投影构造必收原token，增加检查/字符串成员、原正常体`采集完整材料()`及仅catch精确tag的`采集()`。按设计第4节覆盖每个owner调用前后、所有显式嵌套循环、接收认知及JSON边界，保留原结构核验和正常语义。
4. 唯一线程调用传同token，variant分流；取消/已停止跳过make_shared并在原缓存短锁置已停止，正文/序号/完成时间不变。bad_alloc仍原资源诊断，其它异常原最终工程链；缓存最后许可点之后的窄竞态仍允许一次已完整交换，不新增发布门。
5. 从第4节矩阵逐项回读全部新调用边与catch；确认域读取仍不可抢断、无成功截断、无业务写、全join/依赖寿命不变。成组改动后统一实际工程编译，形成专属记录/知识变更及未验证范围。

## 4. 验证与退出

静态：`git diff --check`、`git diff --cached --check`，本两新文档`git diff --no-index --check -- NUL <具名文件>`及相对链接存在性读回；不使用退役检查脚本。代码具体检查项、失败/取消分账及缓存竞态以设计第8节为完成门槛。

默认实际工程编译：VS18 Professional MSBuild，对`D:/海中鱼巣/海中鱼巣.vcxproj`执行`/t:Build /m:2 /p:Configuration=Debug /p:Platform=x64 /p:OutDir=D:/TEMP/海中鱼巣/CONTROL-COLLECTION-COOPERATIVE-STOP/20261101-code/debug-bin/ /p:IntDir=D:/TEMP/海中鱼巣/ACTION-REQUEST-SELF-ADOPTION/20261031-code/obj/`，日志本叶根`debug-build.log`。root明确允许复用其独占且无构建占用的既有obj，MSBuild按真实依赖增量重编；不手工复制旧obj或修改时间戳绕过编译。

允许原诊断Release/x64配置执行同一实际工程`/t:Build /m:2 /p:Configuration=Release /p:Platform=x64 /p:OutDir=D:/TEMP/海中鱼巣/CONTROL-COLLECTION-COOPERATIVE-STOP/20261101-code/diagnostic-bin/ /p:IntDir=D:/TEMP/海中鱼巣/ACTION-REQUEST-SELF-ADOPTION/20261031-parse-diagnostic/obj/`，日志本叶根`diagnostic-build.log`；只复用root原九个诊断宏，不新增宏、不改源码或工程，实际宏值及命令记入专属记录。两个配置obj不得混用，均须0错误且实际完成链接。联合WIP构建不是纯提交基线验收；诊断Release只用于获授权正常入口定位，不据此证明性能，不自动授权运行。

创建侧不运行构建或测试；代码默认仅编译，运行必须由root核对用户当前明确测试指令后进行。获指令也仅真实正常入口/正式网页/原结束，负载及检查范围按设计第8节；不新建脚本、注入、调试器替代、测试工程/模式，不强杀/detach，结束主动确认自然退出。未命中窗口记NOT_RUN，旧停止失败证据保留。

工程完成条件：单cpp完整合同与静态检查闭合、实际工程编译链接通过、施工记录形成；不包含单getter/完整采集的耗时上限。第7节的需求记录/完整调度/目录残余不是本叶可悄增的施工范围。真实停止、硬实时、服务主线贯通仍分别需要后续真实证据；本叶完成不使上位目标退出。

root作工程完工裁决后，计划角色才按具名授权删除本计划及索引仅本项，保留详细设计/记录及Git历史，不处理其它队列。设计/计划及本项索引由root持Git租约按当前精确本地发布授权办理，本创建侧不暂存、提交或推送。
