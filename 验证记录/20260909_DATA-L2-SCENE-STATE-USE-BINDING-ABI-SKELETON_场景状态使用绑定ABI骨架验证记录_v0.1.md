# DATA-L2-SCENE-STATE-USE-BINDING-ABI-SKELETON 场景状态使用绑定ABI骨架验证记录

日期：2026-09-09

版本：v0.1

计划：`DATA-L2-SCENE-STATE-USE-BINDING-ABI-SKELETON` v0.3

基线：`0fb2d13fb1ef0f2ada5ae2ab145f760b3cd9c462`

## 1. 验证对象

只验证合同模块、场景模块及两处工程登记。存在类、状态类、动态类、动态概念类、L1和普通应用不属于本轮对象。隔离材料位于 `D:\TEMP\海中鱼巣\DATA-L2-SCENE-STATE-USE-BINDING\abi-skeleton-v03`，未进入仓库。

## 2. 构建与运行

| 项目 | Debug x64 | Release x64 |
| --- | --- | --- |
| 隔离 Rebuild | PASS，exit 0 | PASS，exit 0 |
| 合同 IFC / OBJ | 3,435,320 / 93,895 bytes | 3,220,926 / 299,679 bytes |
| 场景 IFC / OBJ | 15,270 / 124,525 bytes | 15,272 / 86,347 bytes |
| `abi_probe.exe` | PASS，exit 0 | PASS，exit 0 |

探针编译引用场景全部公开骨架入口；运行检查全部未实现结果DTO保持合同版本1、事实截止0、变更为空、optional/vector为空且 `成功()==false`。

## 3. 静态检查

1. 场景模块唯一import为状态使用绑定合同，未import完整存在类或动态类。
2. 创建、当前读、历史读、按C/E/S三类当前反查和退出共七个业务入口均直接返回 `未实现`。
3. 七入口不调用L1、写端口、provider、状态服务、幂等账、索引、时钟或日志；构造只保存依赖，`绑定于`只比较L1地址。
4. 合同模块在 `.vcxproj` 和 `.filters` 中各登记一次，无重复。
5. 存在类工作树与HEAD blob同为 `1510813c6bc43513503171692cf57d3d7d5e627b`，内容diff为0；先前17行已撤出。
6. 4203 v0.3的类型、字段、枚举数值、函数形状和成功谓词逐项一致。
7. 全仓 `git diff --check` 仅被异主 `AGENTS.md` 第12行行尾空白阻断；本切片六文件将在暂存后以 `git diff --cached --check` 独立核验。
8. `check_specs.py --strict` 检查113份目录项，因冻结清单登记的验收记录被其它所有者工作区删除而报1个错误；该文件不在本计划白名单。

## 4. 根工程分账与未覆盖范围

根工程Debug/Release的合同和场景目标模块形成产物后，被白名单外的空动态模块、存在类旧状态消费者及旧世界树场景消费者阻断；这不构成根工程成功，也不是本计划门禁。

`NOT_IMPLEMENTED/NOT_RUN`：合法首次/重复、真实B写读退、三类真实反查、引用保护、恢复、具体存在provider、合法C/E身份生产、动态消费、生产装配、数据库重启、故障注入和正式集成验收。

## 5. 结论

ABI骨架验证通过：合同与场景目标模块在Debug/Release形成IFC/OBJ，最小消费者两配置运行通过，所有未实现路径安全失败。结论不证明真实状态使用绑定、生产运行或业务闭环。
