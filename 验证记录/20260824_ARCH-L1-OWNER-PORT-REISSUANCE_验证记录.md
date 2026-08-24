# ARCH-L1-OWNER-PORT-REISSUANCE v0.2 验证记录

## 范围与验证根

- 计划 blob：`c82ba65ceb90a034045800de78e23a2c1539147a`
- 施工起点：`fabe03c28eb0ac48bc4abe81ce0659bfaaa31bf9`
- 目标路径：两份生产源码与本施工/验证记录，共四路径。
- 仓库外隔离根：`D:\TEMP\海中鱼巣\ARCH-L1-OWNER-PORT-REISSUANCE\20260824-v0.2-执行-01`；未使用仓库内 `.codex-build`。

## 构建与专项结果

| 项目 | 结果 |
|---|---|
| 根工程 x64 Debug fresh Rebuild（首次） | FAIL：C2672/C2248，`optional::emplace` 无法访问端口私有构造；已在计划范围内修复，未隐去失败。日志 SHA-256 `DE2CAD6D6992858CBAEE8C0F9F8AC2B8CA99124F8EC7E972D01139BEF00CF52D`。 |
| 根工程 x64 Debug fresh Rebuild（修复后） | PASS，exit 0；日志 SHA-256 `AAE7026F74EB64BE80309C915503C6D47B24C4397073FD08EB683527BF1FC044`。 |
| 根工程 x64 Release fresh Rebuild | PASS，exit 0；日志 SHA-256 `4532A1477A24F1D32716E7EEE86051B8BC84CE80A3C808C8FBDF0FB177029DF4`。 |
| 仓库外专项 Debug 构建 | PASS，C++20、`/W4 /WX /utf-8 /permissive-`；日志 SHA-256 `A4445D1F675BD0F799FF70725DE5C9A919DA5A43A69CC4D7F9135E4058AA5BDF`。 |
| 仓库外专项 Debug 运行 | PASS，17/17，exit 0；日志 SHA-256 `0C345777306C394E1D63090F3B53F4122D64D85E7FA60496F0DB55446155A2A8`。 |
| 仓库外专项 Release 构建 | PASS，C++20、`/W4 /WX /utf-8 /permissive-`；日志 SHA-256 `5D2BBAA260BCAC26391C97CD6C4A171A8220A3549F48BDC562D9EB2B65BD7301`。 |
| 仓库外专项 Release 运行 | PASS，17/17，exit 0；日志 SHA-256 `0C345777306C394E1D63090F3B53F4122D64D85E7FA60496F0DB55446155A2A8`。 |

专项源码 SHA-256 为 `C090F9880B18F08F1F005DCAEC40BF0BBFBD6FA4661E927F82C3F52EE0F5EF61`，工程 SHA-256 为 `B52A92AFC9B65920C3060E7001463C132B7C14AB07AD50637BB4A0753C7527A5`。

## 17 项动态矩阵

Debug 和 Release 均逐项通过：

1. 零事实代次返回内部不一致且无端口。
2. 错合同、零 owner、零建立幂等身份返回入口拒绝。
3. 建立函数只填建立结果，既有端口绑定合同不变。
4. 同 owner 有活动端口时重签返回许可拒绝。
5. 既有建立精确重复仍只填建立结果并交付同实例端口。
6. 建立账缺失返回未找到，owner/账异义返回幂等冲突。
7. 重签成功只填重入结果、交付同实例唯一端口且不推进代次。
8. 重签端口移动后来源失效、目标保持租约。
9. 重签端口完成受控首次 owner 写入并从同 owner 幂等账正式读回。
10. 移动目标仍活动时拒绝第二端口。
11. 活动集合分配失败返回资源失败且事实零变化。
12. 资源失败回滚活动登记后，原请求可重试成功。
13. 同 owner 并发重签恰一成功，另一请求许可拒绝。
14. 不同 owner 并发重签均成功并保持各自端口。
15. 同 owner 退出与重签按端口锁串行，不形成退出后端口。
16. 已退出 owner 返回安全历史投影且不交付端口。
17. 运行包销毁后重签能力弱引用失效，端口提交许可拒绝。

## 机械门禁

| 门禁 | 结果 |
|---|---|
| `git diff --check` | PASS；全工作区无 whitespace 错误，仅 Git LF→CRLF 提示。 |
| 目标白名单 diff-check | PASS。 |
| `python .\tools\check_specs.py --strict` | PASS：116/116。异主 5210 等规范 WIP 未导致失败，也未由本切片修改。 |
| 目标源码 UTF-8 无 BOM | PASS：2/2。 |
| 根工程 XML | PASS。 |
| 四配置 `/utf-8` | PASS：4/4。 |
| 生产专项痕迹 | PASS：0。 |
| 目标生产源码 blob（发布前工作树） | 仓库 `fc6a16e97a365e4e77857177c566f6dcd83fdcca`；服务 `93adac75d764cce1a128c29ba900ae5f502f1d34`。 |

发布前还须运行 `git fetch`、确认 HEAD/remote/index、精确四路径 staged 集合及 `git diff --cached --check`；这些事实由最终完成回传绑定结果提交。

## 动态未覆盖与完成边界

- 隔离状态、仓库不变量损坏、非独占范围和建立账互证损坏无法通过合法公开 API 制造；相关生产防御分支已静态核对，但未新增生产测试钩子，因此不声明动态注入通过。
- 崩溃、重启、跨进程恢复、快照恢复、L2 G1、L4 消费者、真实 BIZ 消费和业务闭环均 `NOT_RUN`。
- 清理前确认隔离根共有 677 个文件、29 个子目录、1,699,029,912 字节，且没有从该目录运行的进程。精确递归删除命令被工具安全策略拒绝，未换壳绕过；该目录登记为 `NOT_CLEANED` 普通残留，不占 Git、index、代码段、运行进程或共享验证资源，不阻断本计划功能结果发布。
- 本记录证明计划内实现、根工程构建和专项动态矩阵，不把编译或专项 PASS 升级为五层目标完成。
