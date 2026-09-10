# SERVICE-LAYER-BOUNDARY-CLEANUP-B 领域一致性与应用数据边界迁移验证记录 v0.1

## 1. 验证环境

- 基线：`bcfd76c1335e44dce3aa373b3f0844210fccd5e1`。
- 隔离根：`D:/TEMP/海中鱼巣/SERVICE-LAYER-BOUNDARY-CLEANUP-B/20260910-run1`。
- MSBuild：`C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe`，`/t:Rebuild /p:Platform=x64 /m:1 /v:minimal`。
- before 为基线归档；after 为同一归档仅覆盖七个候选生产文件。探针源码、工程、日志和产物均在仓库外，未进入 Git。

## 2. B01—B14 结果

| 编号 | 结果 | 实际证据 |
| --- | --- | --- |
| B01 | PASS | ABI 探针以标准布局、`offsetof`、成员函数指针核对三结果字段顺序和类型、三个入口及七谓词签名。 |
| B02 | PASS | Debug/Release 行为探针中三入口对同一非零 G 均返回成功；0 G 和三种漂移分别按专属状态收口。 |
| B03 | PASS | 仓外独立 L1 上，写线程最多80轮、每轮4000个合法普通节点写集；读侧最多30万轮、总时限15秒。真实独占写锁竞争命中三入口资源失败：Debug `1/1/1`，Release `1/1/2`，均不抛出；非并发正常/0/漂移调用前后 G 相同。 |
| B04 | PASS | 两个独立 L1 具有各自非零 G；特征、需求、任务窄谓词及需求/任务应用错绑构造均拒绝。 |
| B05 | PASS | 同一 L1 上不同特征服务对象的窄谓词接受；应用合法组合接受，未比较服务对象地址。 |
| B06 | PASS | 行为探针对存在的特征错绑，以及任务的需求、存在、方法、特征四项错绑逐项观察到`invalid_argument`。 |
| B07 | PASS | Debug/Release ABI 探针均编译、链接、运行退出0；新构造可用、旧 L1 构造不可用，三个应用无`绑定于(L1)`。 |
| B08 | PASS | 静态读回确认三个应用各保留两处`守卫(r.G)`；守卫分别调用特征、需求、任务核验，未换 G。 |
| B09 | PASS | 生产成功谓词实际覆盖保存定义+准确 F、本次节点+共享子图；缺节点、重复节点均返回失败；源码读回确认深度上限为`计算项组.size()`。 |
| B10 | PASS | 需求方向对`二次计算内部`零引用；三个应用对`L1事实基座服务|读取中性当前事实代次|绑定于(const L1事实基座服务`零命中。 |
| B11 | PASS | 枚举、业务请求/结果、预算及 G/H 字段无差异；本切片只替换构造、守卫和来源递归。 |
| B12 | PASS | 新入口/谓词无写端口、owner、缓存或锁外泄；行为探针确认非并发 G 核验前后 G 不变。 |
| B13 | PASS（范围外分账） | 独立专项 Debug/Release 对七模块和 ABI/行为探针均完整编译链接运行退出0。根 before/after 双配置均退出1，归一错误集合各67项且完全同构；错误只在既有世界树/概念树消费者。after Release生成七目标 IFC/OBJ，独立双配置专项均生成七目标 IFC/OBJ。 |
| B14 | PASS | 生产差异仅七个允许文件；加两份专属记录后共九项。未改工程、规范、计划、索引、L1及异主 WIP。 |

## 3. 精确命令与结果

```powershell
rg -n "L1事实基座服务|读取中性当前事实代次|绑定于\(const L1事实基座服务" <三个应用文件>
rg -n "二次计算内部" 海中鱼巣/业务/应用服务.需求目标方向.ixx
```

两条负向扫描均零命中。正向扫描命中三 DTO/入口、七谓词、三个领域守卫，以及存在/任务构造绑定条件；三个应用各有首尾两次`守卫(r.G)`。

```powershell
MSBuild.exe boundary_probe.vcxproj /t:Rebuild /p:Configuration={Debug|Release} /p:Platform=x64 /m:1 /v:minimal
boundary_probe.exe
MSBuild.exe behavior_probe.vcxproj /t:Rebuild /p:Configuration={Debug|Release} /p:Platform=x64 /m:1 /v:minimal
behavior_probe.exe
```

- ABI 探针 Debug/Release：构建、链接、运行均退出0。
- 行为探针 Debug/Release：构建、链接、运行均退出0，并输出`BEHAVIOR_PROBE_OK`。
- B03资源命中：Debug `RESOURCE_HITS 1 1 1`；Release `RESOURCE_HITS 1 1 2`。

```powershell
MSBuild.exe <before|after>/海中鱼巣.vcxproj /t:Rebuild /p:Configuration={Debug|Release} /p:Platform=x64 /m:1 /v:minimal
```

四次根 Rebuild 均退出1。将隔离根、行列和项目尾部路径归一后：Debug before/after 各67项、差异0；Release before/after 各67项、差异0。四份归一文本 SHA256 均为`2162903A038FB8588E521F5B0535340641F76AC96E46473EC94942CB2B38C142`。没有目标范围新增、消失或移动错误；失败不能写成根工程编译通过。

## 4. 未验证与完成边界

`git diff --check -- <九个允许文件>`退出0，仅输出既有行尾转换提示。`python .\tools\check_specs.py --strict`退出1：119份目录项中唯一错误为`tools/l1_frozen_surface_manifest.json: FROZEN 必须登记存在的 acceptance_record`，属于本计划禁止范围且 before 当前事实已有，未修复，不能记为 strict 通过。

未验证生产装配、普通应用可达、长期运行、数据库重启/恢复、生产并发、其它故障注入、跨进程和正式集成验收。根工程范围外旧消费者仍不兼容当前场景/动态接口；本计划未获权修复。仓外临时根保留在上述路径供发布者复核，不进入 Git。
