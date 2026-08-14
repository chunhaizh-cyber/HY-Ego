# 所有者范围首次写入材料读取施工检查记录 v0.1

日期：2026-08-15

计划身份：`DATA-L1-OWNER-SCOPED-FIRST-WRITE-RESULT-READ v0.1`

检查基线：正式 `4abd3b9d9f71fd6dd7f9d7df5782dd6edeea6c3e` 加本计划三份生产改动及两份永久记录。

## 1. 根工程构建

生产实现完成后，根工程 Debug / Release x64 Rebuild 均通过。临时检查三件套删除后再次执行：

```powershell
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /m
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64 /m
```

最终结果：

- Debug：`PASS`，0 警告、0 错误，用时 17.29 秒；
- Release：`PASS`，0 警告、0 错误，用时 16.11 秒。

## 2. 生产外施工检查

临时检查工程分别执行 Debug / Release x64 Rebuild 和运行，结果均为：

```text
16/16 PASS
```

实际覆盖：

| 检查项 | 结果 |
| --- | --- |
| 空 owner 对非零身份读取 | PASS；返回未找到，双载荷为空 |
| 五类写项首次提交 | PASS；节点、关系、值、属性槽变更和退出事实均进入首次材料 |
| 首次规范化完整写集逐字段读回 | PASS |
| 原始首次成功结果和新编码映射逐字段读回 | PASS |
| 读取前后事实代次 | PASS；读取不推进代次 |
| 精确重复后的再次读取 | PASS；仍返回原始首次成功结果，不改写为精确重复 |
| 两个 owner 使用相同数值幂等身份 | PASS；严格隔离 |
| 后续其它提交后的首次材料 | PASS；首次代次和映射保持不变 |
| 错合同版本、零幂等身份 | PASS；入口拒绝 |
| 移动源端口 | PASS；许可拒绝 |
| 坏请求与失效 capability 同时出现 | PASS；请求入口拒绝优先 |
| 现行 owner 当前 / 历史代表性读取 | PASS；未发现回归 |

专项只证明同一施工源码和公开合同下可触达的风险分支，不构成独立验收。专项三件套已删除，三路径均不存在且不受 Git 跟踪。

## 3. 静态与清理门禁

- `python .\tools\check_specs.py --strict`：`113/113 PASS`。
- `git diff --check`：PASS。
- 三份生产文件严格 UTF-8 解码：`3/3 PASS`。
- 公开入口计数：`L1所有者范围写端口::读取首次写入材料` 恰一处；仓库实现恰一处。
- 普通 `L1事实基座服务` 同义入口：0。
- 生产工程 / filters 对临时检查路径的引用：0。
- 三份生产新增行中的验证、测试、故障注入、mock、seam 或参数检查痕迹：0。
- 临时检查 `.cpp`、`.vcxproj`、`.vcxproj.filters`：全部不存在且不受 Git 跟踪。
- 现有 `L1所有者范围读取状态` 数值保持 1—9，不改序、不改值。

## 4. NOT_RUN 与声明边界

以下项目没有合法稳定的生产公开注入方式，保持 `NOT_RUN`：

- 正式内存分配或容器长度资源耗尽；
- 私有 owner 幂等账损坏；
- 跨进程恢复；
- 实例或端口的不合法并发销毁。

没有为上述项目增加生产 seam、故障选择器、测试 friend 或公开比较运算符。本记录不宣称 DATA-L2、C1、普通应用、启动、跨进程恢复或业务消费者已经验收；实际消费者接入时继续依据正式状态、载荷和权威读回判断适用性。
