# DATA-L2 六入口并发守卫漂移检查记录

日期：2026-08-16

计划：`DATA-L2-CURRENT-READ-GUARD-DRIFT v0.1`

验证根：`D:\TEMP\海中鱼巣\DATA-L2-CURRENT-READ-GUARD-DRIFT\20260816-001\`

## 1. 结论

两个生产文件完成计划内修复。根工程 `海中鱼巣.slnx` x64 Debug / Release fresh Rebuild 均成功，0 警告、0 错误；六入口 Debug / Release 最终并发矩阵全部退出 `0`，每个入口和配置均至少一次实际观察到读中 `事实代次漂移`，合法并发导致的 `内部不一致`均为 0。场景树两处具名漂移返回点由两份互补原生覆盖率结果分别动态命中。

这些证据只覆盖本计划六入口及具名并发合同，不升级为 WORLD-SELF、BIZ、普通应用或完整 DATA-L2 闭环。

## 2. 根工程双配置

根工程真实入口为 `海中鱼巣.slnx`。Debug / Release 均使用本计划验证根下独立 `OutDir / IntDir` 执行 fresh Rebuild：

| 配置 | 日志 | 结果 |
| --- | --- | --- |
| x64 Debug | `build/Debug/msbuild.log` | 退出 `0`；0 警告；0 错误；用时 `00:00:22.66` |
| x64 Release | `build/Release/msbuild.log` | 退出 `0`；0 警告；0 错误；用时 `00:00:23.68` |

构建输出没有回落仓库默认目录，也没有读取、覆盖或清理异主 `.codex-build`。

## 3. 六入口 Debug / Release 并发矩阵

专项源码为 `专项.cpp`，构建覆盖为 `专项覆盖.targets`。它只经正式公开读取入口和正式 `新增场景成员关系` writer 制造事实推进；没有读取私有状态，没有修改生产工程或增加生产验证入口。

每个最终批次同时检查：

- 稳定控制保持 `已读取`或入口现行合法状态；
- 读前失效精确返回 `事实代次漂移`；
- 零 `G0` 请求保持入口拒绝；
- 读中漂移结果截止非零且不等于 `G0`；
- 变更事实代次为空；
- 对应业务载荷全空；
- 许可拒绝、资源失败只按各自状态计数，不冒充漂移；
- `failures=0`、`internal=0`。

最终结果：

| 入口 | Debug 日志 | Debug 读中漂移 | Release 日志 | Release 读中漂移 | 内部不一致 |
| ---: | --- | ---: | --- | ---: | ---: |
| 1 `读取场景树` | `logs/Debug/TreeOnly-TreeWriterFirst-Debug-Final2.log` | 6 | `logs/Release/TreeOnly-TreeWriterFirst-Release-Final.log` | 4 | 0 / 0 |
| 2 `读取存在单例角色` | `logs/Debug/Entry2-Debug-Final2.log` | 1 | `logs/Release/Entry2-Release-Final.log` | 1 | 0 / 0 |
| 3 `读取完整存在` | `logs/Debug/Entry3-Debug-Final2.log` | 1 | `logs/Release/Entry3-Release-Final.log` | 1 | 0 / 0 |
| 4 `读取存在当前场景成员引用` | `logs/Debug/Entry4-Debug-Final2.log` | 1 | `logs/Release/Entry4-Release-Final.log` | 1 | 0 / 0 |
| 5 `读取场景宿主存在关系` | `logs/Debug/Entry5-Debug-Final2.log` | 1 | `logs/Release/Entry5-Release-Final.log` | 1 | 0 / 0 |
| 6 `读取存在当前宿主场景` | `logs/Debug/Entry6-Debug-Final2.log` | 1 | `logs/Release/Entry6-Release-Final.log` | 1 | 0 / 0 |

十二个最终日志均记录 `total_failures=0`。Debug / Release 中出现的许可拒绝或资源失败均是正式 writer 竞争下的合法非漂移状态；它们没有被计入读中漂移证据，也没有产生部分载荷。

## 4. 场景树两处具名返回点动态覆盖

覆盖率使用 Visual Studio 原生工具采集，不修改生产源码：

| 证据 | 源码行 | 结果 |
| --- | ---: | --- |
| `logs/Debug/TreeOnly-Debug-Coverage1.xml` | `服务.L2场景结构.ixx:3755` | `covered="partial"`；早段树归属目标关系组异截止返回点命中 |
| 同一证据 | `服务.L2场景结构.ixx:3864` | `covered="no"` |
| `logs/Debug/TreeOnly-Debug-LateCoverage2.xml` | `服务.L2场景结构.ixx:3755` | `covered="no"` |
| 同一证据 | `服务.L2场景结构.ixx:3864` | `covered="partial"`；后段目标父关系组异截止返回点命中 |

两份结果互补，证明两个不同具名返回点分别动态执行，而不是同一路径重复计数。覆盖 XML 绑定的生产源码 SHA-256 为 `6E9BF8DF59FCBF873A67239E8C72823CCF73D77F44732C8CCF96C290B570B1EF`。

## 5. 验证波动与处理

1. 入口 2 初始读者先行 160 轮全部成功、漂移 0；证据表明 writer 完成晚于 reader。该批次没有作为通过证据。
2. 改为 writer 先行并按同配置稳定 writer 耗时动态扫描后，入口 2 在两配置均命中读中漂移。
3. 一次 Release 入口 4 复跑出现 `NOT_OBSERVED`；同一时间点双采样后最终批次闭合。
4. 场景树一次因双采样缩小范围未命中；将树与非树调度分开，早段 writer 先行、后段 reader 先行并按稳定整树读取耗时扫描后闭合。

这些是仓库外调度窗口问题，不是生产合同降级。专项没有使用固定 sleep、生产 seam、测试宏、日志 PASS 或第二事实入口。

## 6. 静态门禁与基线

发布前核对目标值：

```text
plan blob = f09ff9321de5bad908594715ea2c3a81d2e169ce
design blob = c273b0688e696d77216daa19bd49ae950939c862
4230 blob = 0f418385811b738b5c01a456ab727254198dde6a
L2场景结构 DTO blob = 0b23c16a240a76751a23ce9dcf42f4e400829041
L2存在结构 DTO blob = 62b4d32b1014ba69ee633c8c026d856c4b9764c2
L2结构公共 DTO blob = bc8176562aea0404ff0cb142bf3586dc42944744
场景服务正式基线 blob = 9d17590eb8cb4ecd6098d923accf29bdeb3ed3fb
存在服务正式基线 blob = ae15e6c05f3a3c38e0038886d073b7202c91ff18
```

发布前实际结果：

- 四目标限定 `git diff --check`：通过；
- `python .\tools\check_specs.py --strict`：`113/113 PASS`；
- 四目标严格 UTF-8 无 BOM：`4/4 PASS`；
- 六个公开签名机械匹配：`6/6 PASS`；
- 三个 DTO、4230、plan、design 和两个生产源码正式基线 blob：全部匹配；
- 两生产文件新增行中的专项身份、`D:\TEMP`、测试宏、sleep、断言计数和 PASS 日志痕迹：`0`；
- 十二份最终日志结构化复核：`12/12 PASS`；
- 两份覆盖 XML 的两个具名源码返回点互补命中：`2/2 PASS`。

只暂存四目标后的 `git diff --cached --check`、cached 白名单、最新 HEAD / 远端关系及远端读回在同批 Git 发布时继续固定。

## 7. NOT_RUN

真实资源耗尽、长时公平、崩溃 / 重启、跨进程恢复、生产普通应用消费、WORLD-SELF v0.2 消费者矩阵、BIZ 快照、自我形成、其它 DATA 当前读取、具名集成验收和完整数据服务最终需求清单保持 `NOT_RUN`。

本检查记录不修改计划状态；结果发布后由计划索引唯一所有者另行登记退出并恢复 WORLD-SELF v0.2 的 S0。
