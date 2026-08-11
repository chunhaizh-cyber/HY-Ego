# L1 中性历史关系组提供者验证记录

日期：2026-08-11

计划：`L1-NEUTRAL-HISTORICAL-RELATION-GROUP-PROVIDER v0.1`

## 验证基线

- 执行起点：`c83da9d8226ea65380c48d2f4045ba333fea6176`
- 配置：Visual Studio / MSBuild 18.5、PlatformToolset `v145`、x64、C++20、Unicode、`/utf-8`
- 外验输出 / 中间目录：系统临时目录，不在工作区生成外验构建物。

## 构建与运行

### 主工程

```powershell
MSBuild.exe .\海中鱼巣.vcxproj /m:1 /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /verbosity:minimal
MSBuild.exe .\海中鱼巣.vcxproj /m:1 /t:Rebuild /p:Configuration=Release /p:Platform=x64 /verbosity:minimal
```

结果：两项均退出码 `0`，分别生成 Debug / Release x64 可执行文件。未运行主程序，因此不把构建结果升级为生产业务闭环证据。

### 生产外公开接口矩阵

```powershell
powershell -ExecutionPolicy Bypass -File .\验证工具\运行L1历史关系组参数验证.ps1 -配置 Debug -合同版本 1 -起始幂等键 3101
powershell -ExecutionPolicy Bypass -File .\验证工具\运行L1历史关系组参数验证.ps1 -配置 Release -合同版本 1 -起始幂等键 4101
```

结果：两项均退出码 `0`；两种配置各 14 项检查全部 `PASS`，失败数 `0`。覆盖：

1. 建立关系类型、两个源端点和两个目标端点，并原子创建三条同类型关系；
2. 创建前截止成功空组，创建截止返回完整组；
3. 源 / 目标方向分组、端点 / 类型隔离及稳定编码严格升序；
4. 同一写集创建替代关系并退出旧关系，替换前 / 替换截止半开边界无空洞；
5. 后续退出替代关系，退出前最后截止仍适用、退出截止排除；
6. 单事实历史读取保留替代关系完整退出生命周期；
7. 当前源 / 目标关系组不混入已退出关系；
8. 未知端点 / 类型组合成功空组；
9. 错误合同版本、非法方向底值、零端点、零关系类型、零历史截止均入口拒绝、逐字段回显且代次不变；
10. 未来截止返回事实代次漂移，准确回显实际读取代次且关系组为空。

两次外验构建均出现 MSBuild `MSB8029` 警告：输出 / 中间目录位于系统临时目录，可能影响增量生成。本包装每轮使用新的 GUID 目录并执行 `Rebuild`，本次完整构建、链接和运行均成功；该警告未被当作性能或长期增量构建通过证据。

## 静态与治理检查

```powershell
python .\tools\check_specs.py --strict
```

结果：退出码 `0`，111 份正式规范目录项全部通过。

严格 UTF-8 解码并使用 XML DOM 解析主工程和外验工程，结果：

- 主工程生产 `.ixx`：26；
- 主工程 `ClInclude`：12；
- 主工程包含 `/utf-8` 的四配置项：4；
- 外验入口登记：1；
- 外验入口 import 总数：1，唯一 import 为公开 `海中鱼巣.核心.服务.L1事实基座`；
- 仓库 / 服务 `读取中性历史关系组` 定义：各 1；
- 主工程、filters、合同导出模块、入口和 L2：零差异。

首次 XML 审计命令直接把带 BOM 文本转换为 PowerShell `[xml]` 时失败；修正为严格解码后移除 BOM 字符并调用 `XmlDocument.LoadXml`，上述计数与解析全部通过。该工具调用波动没有改动仓库文件。

两份记录写入后完成最终检查：八文件精确 `git diff --check` 退出码 `0`；八文件严格 UTF-8 解码全部通过；Git index 为空；本计划范围精确呈现三个已修改生产文件和五个未跟踪生产外文件。Git 同时提示三个既有生产文件下次由 Git 接触时会按仓库设置从 LF 转为 CRLF，该提示未造成空白错误或文件解码失败。

## 未验证范围

- 未注入真实 `bad_alloc` / `length_error` 或其它资源耗尽；异常映射只由源码闭包和两配置编译覆盖。
- 未故意破坏私有索引或仓状态；索引矛盾只由状态完整性源码闭包和公开写入矩阵覆盖。
- 未做长时间并发、公平性、性能、崩溃 / 重启 / 跨进程恢复或生产消费者测试。
- 未执行 S2 场景关系 CRUD、完整历史场景、其它 L2 结构族或具名集成验收。
