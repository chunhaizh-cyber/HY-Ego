# INSTINCT-STAGE3-GSEED-ACTIVATION-CURSOR-COMPAT-MIGRATION 验证记录 v0.1

日期：2026-08-31

计划：`INSTINCT-STAGE3-GSEED-ACTIVATION-CURSOR-COMPAT-MIGRATION v0.3`

计划支撑修订提交：`cfcd885e9d300965c02e0cdf9a73f6324477967b`；plan blob `37286d1deff8a6ae6fbd4bac4abd0bf4349e8026`；design blob `2a1f2045b81385c4944e85869b6812f8bde83004`。

历史安全 WIP 基线：`4b6320fcf5c5222a0f3d1705fea0d25407fb06f2`；安全代码 WIP 已由 `b38059b6b501c49705249f5d5587f6d1f9dba2e9` 发布。

恢复验证基线：`main == origin/main == 6bf847089bbf4e9af4261ed06dbd790e369decb1` 加本轮专项夹具修复；index 在验证开始时为空。

隔离根：

```text
D:\TEMP\海中鱼巣\INSTINCT-STAGE3-GSEED-ACTIVATION-CURSOR-COMPAT-MIGRATION\20260901-gseed-resume-01
```

## 1. 结果

| 验证项 | 结果 | 实际证据 |
| --- | --- | --- |
| 当前工作树 Debug x64 fresh Rebuild | PASS | 根 `海中鱼巣.vcxproj`，完整编译链接，退出码 0；1 个异主既有 C4715，0 error；日志 `debug-rebuild.log` |
| 当前工作树 Release x64 fresh Rebuild | PASS | 根 `海中鱼巣.vcxproj`，完整编译链接，退出码 0；0 warning / 0 error；日志 `release-rebuild.log` |
| exact-HEAD 隔离 Debug / Release 链接 | PASS | 从 `6bf84708` 导出隔离源码，只有仓外入口 / 测试投影；两配置均形成可执行文件，旧 LNK2019 / LNK1120 阻断已消失 |
| G01—G05 Debug 专项运行 | PASS | 修复测试夹具后 G01—G05 全 PASS，exit 0；`gseed-debug-run.log`，SHA256 `456A37C3D8F62711ACC5CEB81191887C45D8B42861E769950AEDAEB1E483C23B` |
| G01—G05 Release 专项运行 | PASS | G01—G05 全 PASS，exit 0；`gseed-release-run.log`，SHA256 同上 |
| A、v1/v2/v3 U、幂等、恢复、旧投影和持久重启 | PASS | G01—G05 运行证明 A 激活、同键重放 / 冲突、异键同义恢复、v4 到旧 ABI 投影、重启逐字段恢复和三种旧格式 U 前后代次不变 |
| 外部 ABI consumer Debug / Release | PASS | 仓外全局模块 TU 只 import 游标服务，静态锁定激活 v1 / 读取 v4 的完整签名与版本 1 / 4；双配置编译、链接、运行 exit 0 |
| B 成功序号 1/N、已可能发布、坏材料、双当前运行注入 | NOT_RUN | 当前没有合法公开夹具可形成未来成功维护 v4、部分发布或腐败 owner 当前槽；未新增第二写入口伪造证明 |
| 真实普通启动 A/B/U | NOT_RUN | 生产入口固定使用真实用户 AppData 且进入等待宿主；当前没有获授权的隔离配置 / 停止夹具，未污染真实运行根 |
| 普通启动生产接线静态审计 | PASS | 只命中 v2 边界完整快照、显式激活 v1 和 v4 独立读回；旧 v1 覆盖起点和 v3 自动迁移调用零命中 |
| owner / 副作用静态审计 | PASS | 激活函数体唯一提交调用为游标 owner 的`提交所有者范围中性写集`；无 A/V、状态、动态、服务历史或秒序写调用 |
| v1/v2/v3 对 v4 索引审计 | PASS | 11 项分支分别使用完整秒 `[4]`、主动安全 `[5]`、服务历史 `[6]`、版本 `[9]/[10]` |
| 分离 ABI consumer 编译 | PASS | 既有启动 / 专项 module consumer 与新增仓外全局模块 consumer 均消费新 DTO 和两个公开入口；后者还完成双配置链接与运行 |
| `python .\tools\check_specs.py --strict` | PASS | 116 / 116 |
| `git diff --check` | PASS | 全工作区无空白错误；只有既存 LF / CRLF 提示 |
| `git diff --cached --check` | PASS | index 为空，无暂存空白错误 |
| 被承接 WIP 保护 | PASS | 启动两个线程装配 hunk和专项“删除等待合同登记”hunk仍在；本轮未恢复、覆盖或暂存它们 |

## 2. 实际命令

当前工作树 Debug / Release fresh Rebuild 采用仓库外独占根：

```powershell
msbuild .\海中鱼巣.vcxproj /m:1 /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /p:VCToolsVersion=14.50.35717 /p:IntDir=D:\TEMP\...\debug\obj\ /p:OutDir=D:\TEMP\...\debug\bin\
msbuild .\海中鱼巣.vcxproj /m:1 /t:Rebuild /p:Configuration=Release /p:Platform=x64 /p:VCToolsVersion=14.50.35717 /p:IntDir=D:\TEMP\...\release\obj\ /p:OutDir=D:\TEMP\...\release\bin\
python .\tools\check_specs.py --strict
git diff --check
git diff --cached --check
```

隔离复核：

```text
git archive 6bf847089bbf4e9af4261ed06dbd790e369decb1
-> 展开到本轮 D:\TEMP 专属 source
-> 入口.cpp 只调用游标专项；专项仓外投影在 G05 后退出
-> 叠加本轮测试夹具修复：等待合同 + v4 / legacy 子根目录
-> Debug / Release 编译、链接并运行 G01—G05
-> 入口.cpp 再改为只 import 游标服务的全局模块 ABI consumer
-> Debug / Release 编译、链接并运行 ABI consumer
```

两份仓外入口投影只用于验证，不进入仓库或结果提交；所有运行持久数据均位于进程号与 tick 形成的 `D:\TEMP` 子根，并由专项 RAII 清理。

## 3. 新增测试源码覆盖

- G01：全新 A，维护纪元等于首次幂等身份；完整秒、主动安全水位、服务历史水位、Gseed、零成功序号及放弃边界逐字段闭合。
- G02：同键精确重复、同键异义幂等冲突、异键同义恢复并保持首次维护纪元。
- G03：11 项 v4 对 v1/v2/v3 旧读取提供字段受限投影。
- G04：v4 维护纪元、Gseed、成功序号和形成代次持久重启逐字段恢复。
- G05：v1/v2/v3 分别进入 U，结果零载荷且前后事实代次不变。

以上 G01—G05 已在 Debug / Release 实际运行并 exit 0，不再只是源码覆盖。它们不覆盖 B 序号 1/N、已可能发布、坏材料 / 双当前或真实普通启动。

## 4. 静态合同审计

1. v4 事实公开字段 12 项，持久材料 11 项；`形成事实代次`只来自值事实创建代次。
2. v4 独立读取把 6/7/8 项归为旧格式待裁决；其它坏长度不降级为未建立。
3. A 在当前槽未建立后继续读取专属游标节点的历史属性值组，只有历史为空才允许首次写入；不是只凭“当前值缺失”推断全新实例。
4. B 在完整 v4 且成功维护提交序号大于零时先返回需完整迁移，三水位零写。
5. U 不消费 v1/v2/v3 兼容投影证明 A/B；旧格式在激活入口固定 fail-closed。
6. 普通启动不调用秒序 composer，不发布 A/V，不保留 v3 自动回落。

## 5. 未验证与完成判断

- `B` 成功维护提交序号 1/N 只有未来成功维护原子提交入口有权形成；本计划明确不实现该入口，因此当前没有合法运行夹具。现有源码只完成并静态复核 `已有成功维护需完整迁移` 零写分支。
- `已可能发布`、坏材料 / 部分 v4 / 双当前、资源异常和并发故障没有获授权的公开注入夹具，保持 `NOT_RUN`。
- 真实普通启动固定写用户 AppData 并进入等待宿主；没有隔离配置和停止资源时不得为取证直接运行，保持 `NOT_RUN`。生产接线已完成静态审计。
- owner 零写已有静态证据；G05 还运行证明 U 分支前后事实代次不变，但没有覆盖全部其它 owner 的故障注入矩阵。

结论：A、幂等 / 冲突、旧投影、持久重启和 v1/v2/v3 U 已取得双配置运行证据，根工程链接与外部 ABI consumer 已闭合。按正式计划 v0.3 的 V01—V10，本叶可以在精确发布专项夹具修复和两份记录后退出；B、真实普通启动及故障注入继续按合法夹具边界分账。任何收口都不证明 B 完整迁移、阶段三、本能路线或业务集成验收完成。
