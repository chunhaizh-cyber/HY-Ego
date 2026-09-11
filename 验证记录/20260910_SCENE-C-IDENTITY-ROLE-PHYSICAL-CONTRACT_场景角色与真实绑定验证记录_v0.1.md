# SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT 场景角色与真实绑定验证记录

日期：2026-09-11

计划：`SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT` v0.8

输入HEAD：`6d5e35055ef11164a1df9757becb3cd5ab78ff32`

验证根：`D:\TEMP\海中鱼巣\SCENE-C-IDENTITY-ROLE-PHYSICAL-CONTRACT\run-20260911-v2`

工具：MSBuild `18.5.4.18101`

## 1. 精确输入

隔离副本由正式HEAD归档后仅覆盖本计划四个生产文件；4203及其它依赖取正式基线。最终覆盖SHA256：

| 文件 | SHA256 |
| --- | --- |
| `海中鱼巣/领域/合同.场景角色组织.ixx` | `359BD858C8CF9165362667A7786B8BCF10C8942C1ED733264001C90ECFBA9456` |
| `海中鱼巣/领域/数据服务.场景类.ixx` | `21614CDF7B380410678C3DC2E12E1512B42D173521ED3EFFF2148460C0AF7C13` |
| `海中鱼巣.vcxproj` | `6B735F3E59AF43518C658C89F7E9072C12BED6FCF6CC3DBF96A265EC2A55789B` |
| `海中鱼巣.vcxproj.filters` | `C506A5E9D70DEB66E553AAECAE2ECDD5B339CB0D6F599443FFC3A9D6A1493098` |

目标工程传递闭包实际包含L1合同/仓库/服务、特征值类、特征类、I64比较算法、存在只读合同/存在类、状态类、4203 B合同、4213动态合同、动态类、新场景合同和场景类。临时探针、工程、中间件、EXE和持久材料均只在上述验证根或同计划`D:\TEMP`目录，未进入仓库。

## 2. 构建与运行结果

统一构建形式：

```powershell
MSBuild.exe <probe.vcxproj> /t:Rebuild /p:Configuration=<Debug|Release> /p:Platform=x64 /m /nologo /v:minimal
```

| 项目/运行 | Debug x64 | Release x64 | 结论 |
| --- | --- | --- | --- |
| `compile-check/scene_compile.vcxproj` | Rebuild exit 0 | Rebuild exit 0 | 目标模块编译链接通过 |
| `probe/scene_probe.vcxproj` | Rebuild exit 0；Run exit 0；`SCENE_PROBE_OK` | 同左 | C/B/S组织、读取、退出和幂等公开路径通过 |
| `probe/dynamic_split_probe.vcxproj` | Rebuild exit 0；Run exit 0；`P01_RESTORE_AND_DYNAMIC_PROBE_OK` | 同左 | 真实原子/组合D、两层组织和同进程持久恢复通过 |
| `probe-cstage/cstage_probe.vcxproj` | Rebuild exit 0；Run exit 0；498 PASS/0 FAIL | 同左 | 合法失败面、映射、零写和结构破坏矩阵通过 |

动态探针在独立持久根分别执行。它实际证明：原子D组织成功；来源B.C属于E1的真实组合D可以直接组织到E2的动态根；两层路径可读；B仍受D引用保护；服务销毁后由公开L1持久恢复入口重建并读回C、B、动态路径及原C/B幂等键。该跨场景成功只证明4204 v1.1的场景服务不解释D来源，不证明E2是D的实际发生场景。

C阶段探针SHA256为`EBE08D8C9BBC9E0502F2262AAB2F2B3FB5DF28C108A64E72CDFF7726480009E8`。其真实动态provider转发计数证明`组织动态实例`成功路径调用`读取当前动态`恰好1次、调用`展开动态来源`0次；旧`G01-R-D-expand-map`断言已删除。

## 3. A/B/C矩阵分账

### A阶段

- `G01-M`：机械检查生产映射switch覆盖现行E窄、S、D当前读取和L1写结果枚举，均保留终端`default -> 内部不一致`；未知底层值不做非法运行注入。
- 工程XML可解析；新合同在vcxproj和filters中均恰好一项；无include伪装。
- 静态禁止项扫描：场景类无三个`未实现...结果`、无运行时未实现回退、无`展开动态来源`及逐原子B.C/请求C比较；七B签名与`绑定于`仍在。
- 锁序扫描：`write_`只在`串行提交`中取得，临界区内仅调用L1原子提交；外部provider、首次材料、前置读取和发布后读回均无服务锁。

### B阶段

`RUN_PASS`：C01、B01、O01-S、O02、R01、I01、X01、P01。真实E/S/D/L1均来自正式生产服务；原子D和组合D均经真实当前读取进入场景纯结构组织。C/B历史、路径、退出、引用保护、同键重放和同进程持久恢复均实际命中。

### C阶段

`RUN_PASS`：C02、B02、O01-F、F01、G01-R；Debug/Release均498 PASS/0 FAIL。可由公开provider替身表达的E/D失败状态、公开合法L1形成的代次漂移、预算、引用、幂等、恢复及结构损坏路径均实际运行并检查零写。

`UNREACHABLE_BY_PUBLIC_CONTRACT + G01-M`：需要非法未知枚举注入、具体L1/写端口私有状态篡改或测试后门才能形成的来源状态。对应证据为A阶段生产switch机械穷尽及C阶段真实公开可达状态清单；未修改L1或增加注入缝。

## 4. 静态与仓库检查

- `git diff --check -- <四项生产白名单>`：exit 0。
- `python .\tools\check_specs.py --strict`：exit 1；唯一既有范围外错误为`tools/l1_frozen_surface_manifest.json: FROZEN 必须登记存在的 acceptance_record`。本片未修改manifest或异主验收记录，不能将其记为通过。
- 工程登记计数：vcxproj=1，filters=1。
- 根工程全量Rebuild：`NOT_RUN`。计划已确认世界树/概念树存在范围外悬空旧消费者；本片不通过恢复旧API、删除消费者或移除工程项来掩盖它们。

## 5. 结论与未证明范围

本计划A/B/C要求的场景生产切片验证通过；strict存在具名范围外阻断，不改变目标模块及公开探针结果。验证只证明C角色、四根、C/E/S绑定、S/D组织路径、退出/重放/同进程L1持久恢复和4204 v1.1动态当前读取边界。

未证明：普通应用或根工程闭合、数据库跨进程恢复、完整七字段CRUD、直接成员/子场景、UTC发生期包含、动态实际发生场景/来源一致性、完整观察、全场景TTL、正式集成验收、生产业务运行或业务闭环。
