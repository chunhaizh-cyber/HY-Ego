# 存在族身份生产与窄提供者验证记录

日期：2026-09-10

计划：`DATA-L2-EXISTENCE-FAMILY-IDENTITY-PROVIDER` v0.2

正式施工基线：`c872dc088f3efe83414f07a48040bf05b4cfcc06`

## 1. 专项隔离构建与运行

隔离工程：`D:/TEMP/海中鱼巣/DATA-L2-EXISTENCE-FAMILY-IDENTITY-PROVIDER/implementation-v01/existence_probe.vcxproj`

使用Visual Studio 18 Professional MSBuild、`Platform=x64`分别执行`Rebuild`并运行探针：

| 配置 | Rebuild | 探针运行 | 结果 |
| --- | --- | --- | --- |
| Debug | exit 0 | exit 0，`existence family identity provider probe passed` | 通过 |
| Release | exit 0 | exit 0，`existence family identity provider probe passed` | 通过 |

两配置均实际扫描、编译并链接L1服务、新窄合同、绑定合同、特征值类、特征类、状态类、存在类和最小消费者，形成存在模块IFC/OBJ及探针exe；未出现C4430、LNK2019或不同named-module类型签名。

探针实际覆盖：

- 4203 v2及绑定合同v1；首次族初始化、初始化异义拒绝、恢复重签发后的初始化重放；
- E与族归属同写同代；当前无载荷确认；历史完整见证；元节点、legacy裸节点和wrong-owner拒绝；
- 创建晚到精确重放、同键异义；成员新增/删除晚到精确重放；
- 外部引用阻断删除；E与族归属同退；退出前历史见证；当前已退出；删除晚到精确重放；
- wrong anchor/type/role、零来源、重复来源、节点/关系混代；
- 物理清理后历史材料已清理；不同L1绑定拒绝。

最终只读复核发现并修正成员变更晚到重放`Gread`缺陷后，上述Debug/Release矩阵重新完整执行并通过。

## 2. 静态检查

- `合同.存在结构身份只读.ixx`精确一处`export import 海中鱼巣.核心.服务.L1事实基座`；零`class L1事实基座服务;`，零直接`核心.合同.L1事实基座` import。
- `数据服务.存在类.ixx`对旧共用存在结果、状态/动态import、状态/动态服务成员、状态/动态关系类型、`状态组/动态组`及状态/动态读取失败名称均为零命中。
- 工程登记顺序为L1服务先于新窄合同，新窄合同先于绑定合同和存在类；工程diff只增加新窄合同项及过滤器项，没有修改L1源码或L1工程项。
- 正式HEAD的存在旧字段外部消费者仍精确为`应用服务.动态概念树类.ixx`第274、580、591行三处；当前异主WIP已另行清空相关内容，但不属于本提交。

## 3. 根工程分账

对正式根工程`海中鱼巣.vcxproj`分别执行Debug x64和Release x64独立`Rebuild`，两者均exit 1。

两次构建都已经扫描并编译到本计划的L1服务、新窄合同、绑定合同和存在类；没有在目标模块出现编译或链接错误。失败发生在异主清空/重构中的消费者，例如：

- `海中鱼巣/业务/应用服务.世界树类.ixx(126)`仍使用已经不存在的`场景类结点`，并继续调用当前清空场景类中不存在的旧入口；
- `海中鱼巣/领域/数据服务.概念树类.ixx(956)`起仍引用当前异主重构中已经不存在或未闭合的场景/动态类型和入口。

因此根工程当前不能宣称编译通过；该失败与计划预先登记的异主场景、动态、概念消费者WIP一致。本切片没有修改这些文件，也没有用空函数掩盖其未闭合合同。

## 4. 仓库检查

发布前实际执行：

- 全仓`git diff --check` exit 1，唯一错误为异主`AGENTS.md:12`行尾空白；本切片目标文件未报告空白错误。多个LF/CRLF提示属于工作区行尾提示，不是diff错误。
- `python .\tools\check_specs.py --strict` exit 1：检查115份目录项，唯一错误为`tools/l1_frozen_surface_manifest.json: FROZEN 必须登记存在的 acceptance_record`。该记录已被异主WIP删除，属于计划预先登记的外部阻断，本切片未恢复、修改或暂存。
- 精确暂存后`git diff --cached --check` exit 0；staged路径已核对，严格等于计划允许的五份生产文件和两份专属记录，共七项。

## 5. 验证结论与边界

专项Debug/Release编译、链接及公开接口探针证明：合法新E可以由存在owner原子生产族身份，4203 v2窄provider能够执行当前确认、历史完整见证、退出及幂等重放，存在类旧状态/动态成员已退出。

本记录不证明根工程整体编译成功，不证明场景C、真实B绑定、动态、普通应用装配、真实数据库跨进程恢复、正式集成验收或业务闭环完成。
