# INSTINCT-STAGE3-GSEED-ACTIVATION-CURSOR-COMPAT-MIGRATION 验证记录 v0.1

日期：2026-08-31

计划：`INSTINCT-STAGE3-GSEED-ACTIVATION-CURSOR-COMPAT-MIGRATION v0.2`

验证基线：`4b6320fcf5c5222a0f3d1705fea0d25407fb06f2` 加本计划六文件安全 WIP。

隔离根：

```text
D:\TEMP\HY-Ego\INSTINCT-STAGE3-GSEED-ACTIVATION-CURSOR-COMPAT-MIGRATION
```

## 1. 结果

| 验证项 | 结果 | 实际证据 |
| --- | --- | --- |
| Debug x64 全项目源码编译 | PASS | 根 `海中鱼巣.vcxproj`，`/t:ClCompile`，退出码 0；启动、游标服务、游标专项和全部依赖完成编译 |
| Release x64 全项目源码编译 | PASS | 根 `海中鱼巣.vcxproj`，`/t:ClCompile`，退出码 0 |
| Debug x64 根工程完整链接 | BLOCKED | 编译完成后 LNK2019 / LNK1120；第一根因是异主治理基线的 `装配.普通应用.ixx.obj` 无法解析 `L2任务子目标承接记录结构服务`构造函数，随后共 30 个未解析符号 |
| HEAD 隔离副本 Debug 完整链接 | BLOCKED | `git archive HEAD` 的 D:\TEMP 干净副本，只叠加本轮四文件并移除两份异主 WIP；仍从同一既有构造函数开始出现 32 个未解析符号，证明不是本轮混合文件污染 |
| Debug / Release 专项运行 | NOT_RUN | 没有形成可执行文件，禁止把“测试源码编译通过”升级为运行通过 |
| A、v1/v2/v3 U、幂等、恢复、旧投影和持久重启 | NOT_RUN | G01—G05 测试源码已编译；运行被基线链接缺口阻断 |
| B 成功序号 1/N、已可能发布、坏材料、双当前运行注入 | NOT_RUN | 当前没有合法生产夹具可形成未来成功维护 v4 或部分发布；未新增未冻结的第二写入口伪造证明 |
| 普通启动生产接线静态审计 | PASS | 只命中 v2 边界完整快照、显式激活 v1 和 v4 独立读回；旧 v1 覆盖起点和 v3 自动迁移调用零命中 |
| owner / 副作用静态审计 | PASS | 激活函数体唯一提交调用为游标 owner 的`提交所有者范围中性写集`；无 A/V、状态、动态、服务历史或秒序写调用 |
| v1/v2/v3 对 v4 索引审计 | PASS | 11 项分支分别使用完整秒 `[4]`、主动安全 `[5]`、服务历史 `[6]`、版本 `[9]/[10]` |
| 分离 ABI consumer 编译 | PASS | `启动.应用程序`与`端到端测试.本能被动维护游标`是游标服务之外的独立 module consumer，两配置均编译通过新 DTO 和两个公开入口 |
| `python .\tools\check_specs.py --strict` | PASS | 116 / 116 |
| `git diff --check` | PASS | 全工作区无空白错误；只有既存 LF / CRLF 提示 |
| `git diff --cached --check` | PASS | index 为空，无暂存空白错误 |
| 被承接 WIP 保护 | PASS | 启动两个线程装配 hunk和专项“删除等待合同登记”hunk仍在；本轮未恢复、覆盖或暂存它们 |

## 2. 实际命令

Debug / Release 编译采用仓库外独占根：

```powershell
msbuild .\海中鱼巣.vcxproj /m /t:ClCompile /p:Configuration=Debug /p:Platform=x64 /p:IntDir=D:\TEMP\...\compile-check-debug\obj\ /p:OutDir=D:\TEMP\...\compile-check-debug\bin\
msbuild .\海中鱼巣.vcxproj /m /t:ClCompile /p:Configuration=Release /p:Platform=x64 /p:IntDir=D:\TEMP\...\compile-check-release\obj\ /p:OutDir=D:\TEMP\...\compile-check-release\bin\
python .\tools\check_specs.py --strict
git diff --check
git diff --cached --check
```

隔离复核：

```text
git archive HEAD
-> 展开到 D:\TEMP
-> 只叠加四份目标源码 diff
-> 反向移除启动 / 游标专项两份异主基线 WIP
-> Debug 完整 Build
```

隔离复核仍在链接阶段以既有模块符号缺失失败；本轮目标源码均已先完成编译。

## 3. 新增测试源码覆盖

- G01：全新 A，维护纪元等于首次幂等身份；完整秒、主动安全水位、服务历史水位、Gseed、零成功序号及放弃边界逐字段闭合。
- G02：同键精确重复、同键异义幂等冲突、异键同义恢复并保持首次维护纪元。
- G03：11 项 v4 对 v1/v2/v3 旧读取提供字段受限投影。
- G04：v4 维护纪元、Gseed、成功序号和形成代次持久重启逐字段恢复。
- G05：v1/v2/v3 分别进入 U，结果零载荷且前后事实代次不变。

以上均为“测试源码编译覆盖”，不是运行通过证据。

## 4. 静态合同审计

1. v4 事实公开字段 12 项，持久材料 11 项；`形成事实代次`只来自值事实创建代次。
2. v4 独立读取把 6/7/8 项归为旧格式待裁决；其它坏长度不降级为未建立。
3. A 在当前槽未建立后继续读取专属游标节点的历史属性值组，只有历史为空才允许首次写入；不是只凭“当前值缺失”推断全新实例。
4. B 在完整 v4 且成功维护提交序号大于零时先返回需完整迁移，三水位零写。
5. U 不消费 v1/v2/v3 兼容投影证明 A/B；旧格式在激活入口固定 fail-closed。
6. 普通启动不调用秒序 composer，不发布 A/V，不保留 v3 自动回落。

## 5. 未验证与完成判断

- 未运行 Debug / Release 专项、普通启动 A、持久重启或故障注入。
- 未取得 B 成功序号 1/N、已可能发布收束、坏材料 / 双当前及 owner 零写的运行证据。
- 未完成根工程 Debug / Release 链接，也未形成可执行外部 ABI consumer；分离 module consumer 只证明编译期 ABI 可消费。
- 链接失败是当前 HEAD / 异主治理基线的既有模块符号缺口，不授权本计划修改工程或其它 owner 代码规避。

结论：当前产物是可编译、结构完整、fail-closed 的安全 WIP；因计划第 9 节的最低运行验证未满足，不能标记为代码结果完成，也不能声明 A/B/U 或本能路线业务闭环通过。
