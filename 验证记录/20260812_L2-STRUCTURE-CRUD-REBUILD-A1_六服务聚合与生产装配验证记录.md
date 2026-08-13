# L2-STRUCTURE-CRUD-REBUILD-A1 六服务聚合与生产装配验证记录

日期：2026-08-13
基线：`34ac1ac588aa86237853204ce10b92f5398f2349`

## 验证矩阵

| 项目 | Debug | Release | 结论 |
| --- | --- | --- | --- |
| 专项项目 Rebuild | PASS | PASS | 正式模块、普通应用与专项 CPP 均完成编译链接 |
| 普通应用装配 | PASS | PASS | 状态为 `已装配` 且上下文非空 |
| 聚合六个非 const getter 与上下文直接 getter 同址 | PASS | PASS | 六类逐一相同 |
| 非 const getter 重复取得同址 | PASS | PASS | 六类逐一相同 |
| 聚合六个 const getter 与上下文直接 getter 同址 | PASS | PASS | 六类逐一相同 |
| const / 非 const 观察同实例 | PASS | PASS | 六类逐一相同 |
| 六服务合法公开只读与直接 getter 逐字段相同 | PASS | PASS | 场景、存在、特征、状态、动态、因果分别通过 |
| 聚合分配失败 | NOT_RUN | NOT_RUN | 未添加生产故障注入或测试钩子 |
| 根工程 x64 Rebuild | PASS | PASS | `海中鱼巣.exe` 生成成功 |

类型性质由专项 `static_assert` 验证：`L2结构聚合服务` 不可默认构造、不可复制构造 / 赋值、不可移动构造 / 赋值。

## 实际命令与静态结果

```powershell
& '.\验证工具\运行L2六服务聚合与生产装配参数验证.ps1' -配置 Debug
& '.\验证工具\运行L2六服务聚合与生产装配参数验证.ps1' -配置 Release

$msbuild = (Get-Command MSBuild.exe).Source
& $msbuild '.\海中鱼巣.vcxproj' '/m:1' '/t:Rebuild' '/p:Configuration=Debug' '/p:Platform=x64' '/verbosity:minimal'
& $msbuild '.\海中鱼巣.vcxproj' '/m:1' '/t:Rebuild' '/p:Configuration=Release' '/p:Platform=x64' '/verbosity:minimal'

python .\tools\check_specs.py --strict
git diff --check -- <A1 九条允许路径>
```

- strict：113 份目录项全部通过。
- 根工程：32 个生产 `.ixx`、17 个 `ClInclude`；相对激活基线只增加一个 `.ixx`、零头。
- 根工程四配置 `/utf-8`：4 / 4。
- 聚合源码静态形状：`private` 区恰六个服务引用成员；六组 const / 非 const getter，共十二个。
- 聚合禁止项扫描：零 `L1`、指针成员、`unique_ptr`、`shared_ptr`、写函数、仓库、锁、缓存、事务、SQL、日志、旧世界树命名。
- 普通应用：只增加一个聚合 owning 成员、一个最终构造阶段、状态 23 和一组 getter；六服务既有构造顺序与直接 getter保留。
- 启动入口：零 diff。
- 正式生产改动扫描：零“验证 / 测试 / 故障注入 / 计数器 / sleep / PASS / FAIL / NOT_RUN”痕迹。
- 严格 UTF-8：七个源码 / 工程 / 脚本文件均无 BOM、无无效 UTF-8、无 U+FFFD；根工程、filters 与专项工程 XML 可解析。

## 未验证范围

未运行普通控制面板、无窗口宿主或其它启动消费者；未验证聚合分配失败；未进行独立集成验收；未进入 I1 / I2、自我初始化；未建立或读取世界树根；未证明六类结构机器语义、恢复、跨进程或完整业务闭环。临时专项三件套保留到交互接受和计划索引退出，之后独立清理。
