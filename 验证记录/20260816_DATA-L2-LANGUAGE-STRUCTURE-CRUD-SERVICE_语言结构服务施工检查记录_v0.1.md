# DATA-L2 语言结构 CRUD 服务施工检查记录

日期：2026-08-16

计划身份：`DATA-L2-LANGUAGE-STRUCTURE-CRUD-SERVICE v0.1`

计划 blob：`279fa2b3a38cdb0f01047a1449de4f7da507f784`

设计提交：`7e2dfc28049bd0f86c171c0a76e727f7c105b6b6`

验证基线：`cc1db7c5dbc01010481ebc32a95359cb10b30db7`

## 1. 结论

根工程 x64 Debug / Release fresh Rebuild、仓库外双配置专项、工程 XML、严格 UTF-8、固定键、唯一模块登记、正式规范、diff 和精确范围门禁通过。

这些结果只证明语言结构 CRUD 服务代码候选和施工门禁；没有普通应用装配或合法生产消费者，不得升级为真实消费者闭环、完整数据服务或 `DATA-EXT-10` 通过。

## 2. 根工程双配置

使用两个互不共享的系统临时 `OutDir / IntDir` 执行根 `海中鱼巣.vcxproj` fresh Rebuild：

```text
MSBuild.exe .\海中鱼巣.vcxproj /t:Rebuild /m:1 /p:Configuration=Debug /p:Platform=x64 /p:OutDir=<Debug临时目录> /p:IntDir=<Debug临时目录>
MSBuild.exe .\海中鱼巣.vcxproj /t:Rebuild /m:1 /p:Configuration=Release /p:Platform=x64 /p:OutDir=<Release临时目录> /p:IntDir=<Release临时目录>
```

- Debug：退出 `0`，生成同一生产程序 `海中鱼巣.exe`；
- Release：退出 `0`，生成同一生产程序 `海中鱼巣.exe`；
- 两配置只出现系统临时输出目录的 `MSB8029` 增量构建提示，无编译或链接错误；
- 未读取、写入或清理异主 `.codex-build`。

## 3. 仓库外双配置专项

仓库外专项直接导入正式 L1、不可变材料和语言服务模块，并链接同次根工程模块对象。Debug 与 Release 均编译、链接、运行退出 `0`，输出：

```text
DATA-L2-LANGUAGE-SERVICE 31/31 PASS
```

覆盖：

- 材料服务同 L1 接受、异 L1 构造拒绝；
- owner 交付与固定四项登记；
- 新增首次、立即精确重复、后继退出后的原新增重放；
- 同一登记键的异幂等请求返回引用冲突；
- 替换产生不同新身份、同键交接、立即精确重复和后继退出后的原替换重放；
- 退出首次和精确重复；
- 跨 owner 当前入边存在时退出返回引用冲突，关系退出后同请求的新幂等写可完成；
- 按身份当前 / 历史读取，旧身份当前为已退出；
- 按登记键当前 / 历史读取，退出后当前未找到、原截止历史命中；
- 严格 UTF-8 登记键、通用字节来源材料和首次原截止不被重复推进。
- 请求前事实代次漂移和坏请求入口拒绝均保持空载荷。

专项未伪造生产 seam；资源耗尽、发布未知、私有事实损坏和并发调度只保留静态合同或 `NOT_RUN`。专项源码、对象、程序、运行材料和日志在发布前清理，空目录骨架不占 Git / 工程 / 构建资源。

## 4. 静态门禁

- `python .\tools\check_specs.py --strict`：`113/113`；
- 新模块、不可变材料模块、vcxproj、filters 均可严格 UTF-8 解码；两个工程 XML 均可解析；
- 新模块在 vcxproj / filters 唯一登记 `1/1`；
- owner / 登记身份及八个 `0x0208` 固定键逐项唯一命中 `10/10`；
- 新模块正式函数均有相邻诊断责任；不可变材料新增谓词有唯一诊断责任；
- 新模块零自检、测试模式、PASS / FAIL、默认汉语、词条、语言记录或概念名称痕迹；
- 目标 `git diff --check` 通过，仅有既有工程文件 LF→CRLF 提示；
- 实际 Git 目标限定为四个代码 / 工程文件与两份专属记录。

## 5. NOT_RUN 与未证明范围

以下按计划保持 `NOT_RUN`：普通应用装配、合法生产消费者、词条、语言记录、概念名称、默认汉语、语素、词性、完整 `DATA-EXT-10`、真实分配 / I/O 耗尽、发布未知、私有事实损坏、跨进程恢复以及完整并发调度。

根工程构建、仓库外专项和代码发布都不等于真实消费者闭环或后续数据服务充分性通过。
