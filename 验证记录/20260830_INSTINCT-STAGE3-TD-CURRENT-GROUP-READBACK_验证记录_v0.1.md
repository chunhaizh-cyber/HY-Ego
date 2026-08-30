# INSTINCT-STAGE3-TD-CURRENT-GROUP-READBACK 验证记录 v0.1

日期：2026-08-30

## 1. 验证对象与隔离边界

- 正式候选白名单：两份 task owner 生产文件和本计划两份记录。
- 隔离候选根：`D:/TEMP/海中鱼巣/INSTINCT-STAGE3-TD-CURRENT-GROUP-READBACK/20260830-implementation-v1`。
- 临时修改 `端到端测试.自我治理一致事实.ixx` 及仓库外 `abi.cpp`、runner、IFC 映射、静态库、持久根只用于验证，不进入正式差异或 Git。
- 提交树复验时的精确 staged tree 为 `e63a98b97933f9f0d60fb1766fb7ec37d79d80cf`，展开根为 `D:/TEMP/海中鱼巣/INSTINCT-STAGE3-TD-CURRENT-GROUP-READBACK/20260830-commit-tree-v1`；复验后仅补写两份记录，生产文件 blob 未改变。
- 两配置根工程与 ABI consumer 已从该 staged tree 重新验证；专项运行沿用隔离候选已完成证据，未为重复运行而清理用户生产持久根。

## 2. 构建、专项和 ABI

| 门禁 | 结果 | 证据边界 |
| --- | --- | --- |
| v143 x64 Debug 根工程 Rebuild | PASS | 隔离候选及精确 staged tree 均完整编译、链接成功；提交树输出 `x64/Debug/海中鱼巣.exe` |
| v143 x64 Release 根工程 Rebuild | PASS | 隔离候选及精确 staged tree 均完整代码生成、链接成功；提交树输出 `x64/Release/海中鱼巣.exe` |
| Debug 治理专项 | PASS | 临时入口运行 exit 0；包含 T→D V01—V07 |
| Release 治理专项 | PASS | 临时入口运行 exit 0；包含同一 T→D 矩阵 |
| task 登记持久恢复补充 | PASS | 临时测试投影以独立 L1 持久根完成普通应用首次装配、释放、同根恢复装配；新增两项登记首次与恢复重放均未使 task service 构造失败 |
| Debug ABI consumer | PASS | 隔离候选及精确 staged tree 均独立 import `海中鱼巣.领域.服务.L2任务结构`，消费事实/请求/结果、版本、两个谓词和公开成员函数；运行 exit 0 |
| Release ABI consumer | PASS | 隔离候选及精确 staged tree 均以 `/GL` 编译、`/LTCG` 链接并运行 exit 0；隔离候选另含 `/OPT:REF /OPT:ICF` |

ABI consumer 手工编译出现与项目模块环境宏差异的 C5050 警告，但编译、链接和运行均成功；它只传递纯值对象和成员函数指针，不据此宣称运行期业务消费。

治理大全专项依赖既有共享生产配置，不具备可重复清空的独立持久根。此前两配置完整运行已经 exit 0；后续为补登记恢复再次运行时，在本叶测试之前的既有“系统根初始化”处失败。未删除或清理用户生产配置来换取重复绿灯；该重复运行失败不归因于两份 task owner 候选，也不替代精确提交树复验。

## 3. T→D 专项矩阵

| 编号 | 结果 | 覆盖 |
| --- | --- | --- |
| V00 | PASS | 新增两项类型登记首次形成、普通应用释放后从同一 L1 持久根恢复重放 |
| V01 | PASS | 首次融合形成 1 项显式 `T→D`，公开入口逐字段正式读回 |
| V02 | PASS | 原键原请求精确重复，读回组与首次完全相同，无第二关系 |
| V03 | PASS | 任务退出后入口返回 `已退出`，无效任务、空组、截止 0 |
| V04 | PASS | 兼容 `新增任务` 返回合法 0 项，不从列表项补造 |
| V05 | PASS | 坏专属版本、零任务为入口拒绝；旧 G0 为事实代次漂移；全部非成功空载荷、截止 0 |
| V06 | PASS | 结果自身成功谓词拒绝非规范排序 |
| V07 | PASS | 结果自身成功谓词拒绝重复关系身份 |
| 多项 provider 逆序输入 | NOT_RUN | 当前正式生产仅有首次 1 项写入且本叶禁止第二业务写入口；服务代码执行规范排序，动态证据只覆盖 0/1，未以私有补造冒充 N 项运行证明 |
| 同键异义登记 / 登记材料损坏 | NOT_RUN | 公开 API 不能合法制造持久账异义；需要专属 L1 恢复故障 fixture |
| 读中并发漂移 / 许可 / 资源故障注入 | NOT_RUN | 当前测试缝不能稳定注入这些下层结果；映射与空载荷由代码事实和非成功谓词检查，不升级为运行证据 |

## 4. 静态检查

- 两生产候选相对 HEAD：数据头 `+78/-0`，服务模块 `+355/-15`；15 行删除均为形状、数量、索引和调用参数的等价替换，没有退出既有业务入口。
- 新键 `0x0301'2114/2115`、写入键 `0x0301'2613` 和独立登记幂等身份 `0x4C32'5452'434F'5245` 只落在 task owner 目标服务模块。
- 原十项 `任务结构类型登记幂等身份` 请求未增加成员；新增两项由独立请求登记。
- 正式差异不含临时专项、普通应用、工程文件、L/H、服务事实 owner、FRESH 或安全门禁。
- `python .\tools\check_specs.py --strict`：`116/116` PASS。
- `git diff --check`：PASS。
- `git diff --cached --check`：PASS。
- 精确 staged tree 的 v143 x64 Debug / Release Rebuild、Debug ABI consumer、Release `/GL + /LTCG` ABI consumer：PASS。

## 5. 结论边界

当前候选已证明生产首次任务融合的首个显式 `T→D`、兼容 0 项、任务退出收口及同 G0 当前组读取。多项 provider 排序与下层故障注入保持 `NOT_RUN`，不由静态搜索或单个状态码冒充。该结果不证明服务活动、安全门禁、被动维护事务、阶段三或完整路线完成。
