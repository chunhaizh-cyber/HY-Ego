# 概念树旧动态承载退役与编译闭合验证记录

日期：2026-09-12。DATA-L2-CONCEPT-TREE-LEGACY-DC-RETIREMENT v0.2。验证所有者 `/root/feature_concept_execute`。候选源码已验证、尚待root精确发布；本记录不宣告正式验收或根工程总体通过。

## 1. 精确输入与隔离

计划 blob `01688232c757403071646a3f9ad3154a288e1a1d`，设计 blob `b37d2e2b9a17e92665393dbaac9a268705d45ed8`，v0.2 HEAD/origin `73e0a0f84beca1e6c261d0870634de4abe83ed83`。独占根 `R=D:/TEMP/海中鱼巣/DATA-L2-CONCEPT-TREE-LEGACY-DC-RETIREMENT/01a07099-20260912-code`。所有下表日志路径均相对 R。

源快照最初由 `git archive` 的5d420883形成；5d至73仅三文档差异。最终冻结前，未修改依赖全部与正式当前HEAD的原始字节比较；archive检出CRLF差异经仅换行互证后恢复HEAD原始字节，60文件清单见 `head-byte-normalization.json`。概念/EC/两工程与工作区本片候选逐字节一致；动态应用明确不存在。没有把暂停feature的12文件改动带入专项或根候选；具体排除路径/SHA见施工记录第6节及 `excluded-feature-wip.json`。

最终生产目标身份详见施工记录；概念 blob `bb15b6c3cce62df102489bb5e034a1e5460f3322`。退役差异 SHA256 `1076ad8ff24fd792ab973dc0d43c944301c48a75d10751a7158ebc5312cb65af`。消费者/完整根源快照/工程/脚本共75个冻结输入，`final-inputs.json` SHA256 `04973c5b73f64c05b1eb06e5a446884fa918e082294ebb7f8ac6a23ba095e82a`。最终6阶段共12次前后重算75项全部一致，检查记录 `final-checks.json` SHA256 `cc55f2920b8bd950388035157c4615abe5d80c161454d07f8453fd608c297eb3`；没有用仅记hash代替相等断言。

专项实际17模块：6个L1核心、特征值/特征/存在/状态/场景/概念6数据模块、存在身份/场景状态使用/动态结构/场景角色4合同和1算法。5个L1相邻数据头均在源快照和冻结清单。动态结构是当前场景服务的合同依赖，并非被退役的动态概念应用/具体动态数据服务。

## 2. 命令与结果

工具：VS18 Professional，MSBuild `C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe`，MSVC14.50.35717/v145，Windows SDK自动10.0，x64，C++20，utf-8。专项Debug显式 `_DEBUG`、`/MDd`、`/Od`，Release `/MD`、优化MaxSpeed。专项附加 `/bigobj`，根工程使用原正式项目参数；未关闭Debug以避错。

实际脚本 `run-final.ps1` 的每次构建均 `/t:Rebuild /p:Configuration=<Debug或Release> /p:Platform=x64 /m:1 /nologo /verbosity:minimal`，指定本轮独立 `OutDir/IntDir`。先专项双配置构建/运行，再根候选双配置。具体参数、退出码和计时原文见 `final-results.json`，SHA256 `f07654578c7790a60102b242255757c4077c26bf7c8ad60bce94d070027af591`。

| 命令阶段 | 退出码 | 秒 | 实际结论 |
| --- | --- | --- | --- |
| `final-Debug-build` | 0 | 45.812 | 17模块编译链接通过 |
| `final-Debug-run` | 0 | 25.708 | 541累计断言；G=46 |
| `final-Release-build` | 0 | 61.908 | 17模块编译链接通过 |
| `final-Release-run` | 0 | 1.089 | 541累计断言；G=46 |
| `root-Debug-build` | 1 | 49.753 | 概念树诊断0；范围外FC/EC错误 |
| `root-Release-build` | 1 | 42.383 | 概念树诊断0；范围外FC/EC错误 |

两次运行末行均 `RETIREMENT_OK 541 G=46`，每构造末行依次 `MODE_OK 0 G=25`、`MODE_OK 1 G=32`、`MODE_OK 2 G=39`、`MODE_OK 3 G=46`。541是包含真实owner/元节点准备及代次读取的累计断言数，绝不是541项业务能力。stderr空，无进程遗留。

根工程两配置实际首错均 `应用服务.特征概念类.ixx:368 C1001`，随后同文件旧scene API诊断与 `应用服务.存在概念树类.ixx:248/250` 旧 `使用存在服务/读取场景历史事实`。归一计数各配置FC6、EC25，概念树文件0，动态概念应用无编译项。来源是正式HEAD依赖快照，FC没有带入feature WIP，EC仅本片一行机械删除。这里只分账未改文件错误，未越权修复，也不把root失败当PASS。`root-diagnostics.json`保存具体首错和分类；probe03最小18模块在EC处先错，不等于完整根首错也是EC。

## 3. 实际矩阵

| 冻结要求 | 实际消费者/断言 | 结论 |
| --- | --- | --- |
| 四保留构造 | mode0基础、mode1特征、mode2特征+存在、mode3共享，各真实owner/正式类型布局 | 双配置通过；构造G不增 |
| 四根/动态根身份 | 各构造逐根完整只读；动态根getter精确身份 | 双配置通过 |
| 旧kind3 | 同一L1写集真实创建节点及kind/lifecycle属性，根归属与直接边；未使用已退DTO | 定义和动态完整图35、无载荷/G不增 |
| 无关根隔离 | 动态根下已有kind3时读FC/EC根图 | 双配置成功，没有吞旧节点后伪全图 |
| scene真实编码 | 独立scene三登记后真实启用C场景角色 | 仅作合法编码材料，不给concept新增provider |
| scene读取/释放 | 每构造普通及应用入口；空引用/空预期释放 | 30、无成功载荷/首次H、零G，不记无须变更 |
| 非法scene头/编码 | 零编码、未来G、显式非零H、零编码释放/未来G释放 | 输入或漂移原门禁；失败空载荷 |
| 非scene控制 | 每构造E空引用普通及统一查询；FT空引用/真实FC定义引用 | 真实读取成功；H0统一查询内部解析为G |
| 非scene创建/原key | 每构造普通FC由真实E/FT形成；同key完整请求重放 | 创建G+1、原key重复保持身份/首次H/G、独立读回成功 |
| 场景first-book前门禁 | 源码 `执行写入/执行应用概念写入` 强类型前置在首次账之前 | 静态闭合；本次零G与无首次H有动态证据 |
| 旧DTO/旧消费者物理退出 | 目标旧符号及全正式生产旧公开面扫描 | 零命中；源码与工程项均删除 |
| 根工程 | 正式HEAD依赖加仅本片差异；两配置Rebuild | 目标诊断0；总体失败按范围外错误分账 |

## 4. 探针历史与未覆盖

probe01在删除旧尾variant时出现C2275/2146，本片补闭合；probe02删除公共预算字段导致EC225/235错误，因当前消费者仍用已恢复。probe03概念已通过，EC旧scene接口失败，改为不依赖应用的17模块专项，没有修改EC额外逻辑。

probe04消费者关系role的uint64→int64收缩编译失败，probe05概念布局值来源写成公共anchor而非自身导致构造拒绝，probe06旧kind3节点与属性非同次创建导致拒绝；均按真实L1合同修夹具，生产校验未放宽。probe07真实发现统一查询H0/具体内部H冲突，在白名单concept修复，probe08成功501/G42。probe09新增FC夹具的显式身份构造遗漏编译失败，修正后probe10成功541/G46。以上局部Build/运行只作诊断，不代替最终同冻结Rebuild两配置。

NOT_RUN：没有重造“旧场景释放首次账已绑定”的生产旁路，旧key场景门禁由代码顺序核对；没有准确F位置创建提供者的本片夹具未新增无位置F，非scene控制实测E/FT/普通FC，F专属完整业务重放未单独运行；本片不做暂停feature应用验证、RC/DC新模型、完整FC/EC应用闭环、进程间持久恢复、坏分配/并发故障注入、正式集成验收。根工程总体仍失败。旧事实物理保留的长期清理/迁移不在本片。

## 5. 静态验证与知识交接

目标 `rg` 冻结旧符号模式退出1且空输出；正式生产 `.ixx/.cpp/.h` 旧动态公开面扫描退出1且空输出，日志 `static-target-old-symbols.log`、`static-production-consumers.log`。公共scene类型/import/同底座谓词及动态根getter保留，无 `读取场景角色历史` 假静态调用。两工程只删除动态应用登记，概念仍一项。

`git diff --check -- <本片五生产目标>` 退出0；完整七文件记录封尾的diff-check同样退出0，见 `diff-check-final.log`；cached diff-check退出0且index为空。`py -3.14 tools/check_specs.py --strict` 退出0，实际124份目录项全部通过，不能沿用预估的acceptance_record失败。未修改规范/索引/共享知识。函数级变更及保留签名见施工记录第3/4节，知识维护仍待发布后处理。

## 6. 输入与输出指纹

完整75项在manifest；下表列实际17模块、相邻头和专项直接输入。根额外源也在同一75清单，并已经与正式HEAD原始字节逐项核对。

| 输入 | bytes | SHA256 |
| --- | --- | --- |
| `consumer.cpp` | 14615 | `260c45830ffe85565eae3755f427c1d84dfc93ca991f32d09c25c75c589adabe` |
| `freeze-final.py` | 1564 | `88e43dcee1f04e3767a9e8ff229740cc55ac5cb0f60391993a15156e9660d9f3` |
| `module-list.txt` | 2458 | `d46ece3ded07d525449dcff196b3477c3d0f5136e58a8590a9c51f6cbf139ad6` |
| `Retirement.vcxproj` | 4852 | `1318651934ecf340e2b8dae0cfabceedfe7062ce4cc972299ef04470d684772d` |
| `run-final.ps1` | 2528 | `2ca80d8cb2b631b57f2e63374bd35184e7c87901348a8e071e01afb9f854f80f` |
| `snapshot/海中鱼巣/启动选项.数据.h` | 920 | `86566be0a09e4142ff9ba3f834b7c3b382b532bdc036b11e700d380162c03c22` |
| `snapshot/海中鱼巣/核心/L1中性CRUD.数据.h` | 20891 | `f6ebc66170656e73f4f1eb266ab50b975173af4ac00609374c786523d7c231b8` |
| `snapshot/海中鱼巣/核心/L1事实基座.数据.h` | 8329 | `9a700aed169e165699fc7f06664178d0734858375e9e12c8f10f70a0b4ada709` |
| `snapshot/海中鱼巣/核心/L1事实基座持久恢复.数据.h` | 2238 | `5cdde1a654b42aff8a7c6ddfdf3f94eeb74c158fc2866fca6a30d9dff1f4aea6` |
| `snapshot/海中鱼巣/核心/L1公共事实.数据.h` | 536 | `88c3831ca518d451b56c08e1fa7eb9f422c58f1d0efedb6c3c8e1aecf7a936de` |
| `snapshot/海中鱼巣/核心/L1所有者范围CRUD.数据.h` | 46746 | `510195d8d34192461a5f6d602d49df01925c08396034487d7e235e9ac0b9eaa9` |
| `snapshot/海中鱼巣/核心/仓库.L1事实基座.ixx` | 417556 | `dca0a8db8edeae943361eb24a289dc509dee08bbb29cbb33ac4924e4932e9fc2` |
| `snapshot/海中鱼巣/核心/合同.L1中性CRUD.ixx` | 357 | `30125f34e7dca0fd55428d50b953334527abe2fba27ddc3cbf33c5f520aa5372` |
| `snapshot/海中鱼巣/核心/合同.L1事实基座.ixx` | 440 | `209ba8fb3f272fc5c7e2e031550a44a79e6e5dbf858f661fce0f7aa1b469400c` |
| `snapshot/海中鱼巣/核心/合同.L1公共事实.ixx` | 132 | `4c6913fcfa0ec5b78eaf920b26b48f3e4e2bcd67e5ca1a73904d292ca44c970e` |
| `snapshot/海中鱼巣/核心/合同.L1所有者范围CRUD.ixx` | 345 | `9a5f13d93213fdee03e0986a705f6c86c816afd8824e95c27edd5bcc35c579c3` |
| `snapshot/海中鱼巣/核心/容错检查.h` | 1732 | `05e660e2f810ef6b9a24f48aed15ab3e2d0e2e36cc51e52b2750e34e5ad15be5` |
| `snapshot/海中鱼巣/核心/日志系统.h` | 9359 | `f1883be16f7d9c8d524728fcdcc92008b1ae35b46bd97aa37576a8b991b37db0` |
| `snapshot/海中鱼巣/核心/服务.L1事实基座.ixx` | 55025 | `a172cf0b404262dc562f3b0823e5d83d70ae7c28d2ee2f4564e333c32a3075ad` |
| `snapshot/海中鱼巣/程序运行结果.数据.h` | 1388 | `89c13ca8f0387c625200fd45cbe2887253070214b05a7d074a6453658f8b2a1e` |
| `snapshot/海中鱼巣/线程/事件日志线程.数据.h` | 4733 | `9c2c2b93678b4b2d34686d57243406e20c76ca43e2b2ada3a5321d135cbf5bc1` |
| `snapshot/海中鱼巣/线程/支持线程候选.数据.h` | 2629 | `b62e8decf0672494824003f225408eebb4e08f743fcc32cb04409ed0ef2fb36b` |
| `snapshot/海中鱼巣/线程/线程生命周期.数据.h` | 5808 | `3e3f4376ef0979acf22821a56843792acc7defbba468c2e2324ba30380cb81db` |
| `snapshot/海中鱼巣/领域/合同.动态结构.ixx` | 16871 | `a75c389ce3feb0c66d16a6f61885cfc5181f530c717b965878c0b96b8dc8ed3e` |
| `snapshot/海中鱼巣/领域/合同.场景状态使用绑定.ixx` | 8920 | `6e20377ace094db95ba1bcdcd6631abc2c36665274aeb15f5684c169a3b163b0` |
| `snapshot/海中鱼巣/领域/合同.场景角色组织.ixx` | 57406 | `9d68d142e9e39a10ab351c57438ca6f5a5c34c5e3a5952fba3b1afecf2adb39e` |
| `snapshot/海中鱼巣/领域/合同.存在结构身份只读.ixx` | 15785 | `b1323b8d280cc8493b264e2a6507c657d42ddbaa11b7705787a53d85f3267e69` |
| `snapshot/海中鱼巣/领域/数据服务.场景类.ixx` | 238613 | `ea23da15ef7d2685686d95052d9052a6c289a0b66b1fa75ef11c5b1b264ceee4` |
| `snapshot/海中鱼巣/领域/数据服务.存在类.ixx` | 237384 | `84368133e3843365c318e6e96da753a69d99b2ca1467d537303d52057779173b` |
| `snapshot/海中鱼巣/领域/数据服务.概念树类.ixx` | 340354 | `e3c958db1f54dfefe768023a92ae43890ec755f996f205a6bdcacdf77068aa8b` |
| `snapshot/海中鱼巣/领域/数据服务.特征值类.ixx` | 7271 | `c2a287c3534f62caed991e00e08c522f39b4e5ac0cdcd23f3b3b84a408beb00b` |
| `snapshot/海中鱼巣/领域/数据服务.特征类.ixx` | 200457 | `7ac355e164a36e821cf5cec6cc2d4075539d0f22bc252de7589703e587dbf1d8` |
| `snapshot/海中鱼巣/领域/数据服务.状态类.ixx` | 73657 | `7877bc9938eb10a63d98adf233159dc8a90f6fb8709ac2857ede7963ddf757ca` |
| `snapshot/海中鱼巣/领域/算法.有序I64特征比较.ixx` | 9200 | `acebb285b5ab4f26ecd99e578436e4d4dee6eb56beaaf8b21eee5ae31e83a9e4` |

最终日志和可执行文件如下。含全部探针、最终日志和控制材料的49产物清单在 `artifacts.json`，SHA256 `8075d813981fe9bc49e487d366ec79e08c522e1948ae00144d15c3f0fe56a1df`。

| 输出 | bytes | SHA256 |
| --- | --- | --- |
| `final-Debug-build.stderr.log` | 0 | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` |
| `final-Debug-build.stdout.log` | 1303 | `11107cb246b9d373b1e9e7863a7e42d37a945d3ea100d5f5d08f59705f899c44` |
| `final-Debug-out/Retirement.exe` | 9179136 | `00e9c87def21c91c06a7ab548514937ab7b8dfeea3ca1a4745dc9702b2ea1fa1` |
| `final-Debug-run.stderr.log` | 0 | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` |
| `final-Debug-run.stdout.log` | 12174 | `b4ec772b78e02204519d243c5a498762fd4a98150bbf56ce09dad983a9e9b35e` |
| `final-Release-build.stderr.log` | 0 | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` |
| `final-Release-build.stdout.log` | 1305 | `271da9a02dacc54086bea90a58623cd5e4a8a5605ce9262ced59c0b9198ff62d` |
| `final-Release-out/Retirement.exe` | 2929152 | `d6e4cf6be0d0522bd768e43ab9bd7599d635575279064d2e807fae4653dfebcb` |
| `final-Release-run.stderr.log` | 0 | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` |
| `final-Release-run.stdout.log` | 12174 | `b4ec772b78e02204519d243c5a498762fd4a98150bbf56ce09dad983a9e9b35e` |
| `root-Debug-build.stderr.log` | 0 | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` |
| `root-Debug-build.stdout.log` | 17809 | `0182f55f67d49413e1c63c07788ff3b46f8f0d6dcd39c67d11a743594a8c45a8` |
| `root-Release-build.stderr.log` | 0 | `e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855` |
| `root-Release-build.stdout.log` | 17809 | `78043b81c91e26b2a0de7cce33ce7ceec48624a070cb06403542d252317ca1ac` |

## 7. 收口与资源

所有6阶段已结束；按本轮绝对路径核对无MSBuild/cl/Retirement进程。独占根暂留供root只读实际核验，未清理、不触feature/其它旧验证目录。两份20260906受保护未跟踪记录未写。源码/工程冻结，记录封尾后申请root唯一Git发布租约；尚未暂存、提交或推送。待发布事实由root回执记录，不以本次源码/编译替代发布。
