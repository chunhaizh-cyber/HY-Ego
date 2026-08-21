# DATA-L2 动态端点当前组读取数量预算代码实施切片

身份：`DATA-L2-DYNAMIC-ENDPOINT-CURRENT-GROUP-READ-BUDGET`

版本：v0.1

日期：2026-08-15

状态：设计就绪；以计划索引正式登记状态为准

## 1. 目标与完成条件

依据[详细设计](../规范/详细设计/20260815_DATA-L2-DYNAMIC-ENDPOINT-CURRENT-GROUP-READ-BUDGET_动态端点当前组读取数量预算详细设计_v0.1.md)，为按主体、按前状态、按后状态三个既有当前动态组查询增加非零动态对象数量预算。

完成条件：

1. 三请求各追加 `std::uint64_t 最大数量 = 0`，helper 拒绝零预算；
2. 三公开入口把预算传入同一私有端点读取；
3. 预算按关系组中唯一动态身份数量计数，比较发生在排序后、完整动态读回前；
4. 超限返回 `数量预算不足`、空动态组和空变更代次，不截断；
5. 预算内保持原端点互证、同守卫完整读回、严格升序、同截止与并发漂移重分类；
6. owner、写入口、来源查询、工程和普通应用零变化；
7. 双配置构建及静态合同通过，记录如实列出 `NOT_RUN`。

## 2. 前置与 S0

- 设计基线：`bc44caecc2355899c98e4ad0ffd98324e7c24445`。
- 共同 `数量预算不足` 状态、组结果和按来源有界查询已经发布。
- 三请求 / 入口无生产消费者，不需要兼容重载。
- 本叶不修改工程 / filters 或普通应用，不受异主 EVENT 共享文件 WIP 阻断。

执行前机械核对索引、plan blob、最终设计 blob、HEAD、index、两目标文件和消费者集合。若出现消费者、ABI 或所有权漂移，固定 `DRIFT-DYNAMIC-ENDPOINT-GROUP-BUDGET-ABI` 并退回计划支撑。

## 3. 允许范围

```text
海中鱼巣/领域/L2动态结构.数据.h
海中鱼巣/领域/服务.L2动态结构.ixx
施工记录/20260815_DATA-L2-DYNAMIC-ENDPOINT-CURRENT-GROUP-READ-BUDGET_动态端点当前组读取数量预算施工记录_v0.1.md
验证记录/20260815_DATA-L2-DYNAMIC-ENDPOINT-CURRENT-GROUP-READ-BUDGET_动态端点当前组读取数量预算检查记录_v0.1.md
```

禁止修改 L1、公共状态、其它结构服务、聚合、普通应用、工程 / filters、BIZ、线程和计划外文件；禁止第二服务 / owner / 写端口 / 缓存 / 索引 / 私有仓访问、分页、截断、deadline、sleep、日志裁决、生产 seam 或独立专项。

## 4. 实施

1. 三请求追加预算，三 helper 增加非零校验。
2. 私有 `按端点读取动态组` 追加预算参数，三入口逐项透传；现有 `收敛并发条件读取` 保持原样。
3. 维持原关系组形状和重复身份检查；排序后比较动态对象数。
4. 超限调用共同失败 helper 返回 `数量预算不足`；预算内继续原完整读回。
5. 新建两份永久记录，完成必要构建和静态检查。
6. 只提交四条允许路径，普通推送后更新索引为已退出。

## 5. 验证

```powershell
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64
python .\tools\check_specs.py --strict
git diff --check
git diff --cached --check
```

静态证明三字段、三 helper、三透传、唯一私有预算参数、排序后预算比较、零截断和生产零测试痕迹。不运行独立专项；运行态预算边界与正式消费者全部 `NOT_RUN`。

## 6. 完成声明

只证明 DATA-EXT-05 的主体 / 前状态 / 后状态当前组预算叶；不证明时间范围查询、底层关系枚举资源上界、DATA-EXT-05 整项或最终需求清单完成。
