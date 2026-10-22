# SERVICE-REQUEST-INDEPENDENT-TARGET 施工与编译记录

日期：2026-10-21；状态：六文件代码已实施，实际项目Debug|x64编译链接通过；未运行程序或测试。

## 1. 授权、计划及 S0

用户已明确接管并保留 main 七份 WIP，允许本子项计划和登记文件提交推送；连接提交暂不合入 main，生产代码不推送。代码由 root 任务树的 implement_target 单一所有者整体实施，未拆分六文件公开合同。

- v0.2计划及登记发布：`c3c6394e70a6f7b254dd09076d55dc1f0ecc0fc1`，计划blob `91158a2c9cdc2eab45dcae99dc6d1a373dd77932`。
- 实施中识别不带R的同来源重投可能复用旧目标；暂停该分支，由计划支撑闭合最小纯读查询口。
- v0.3正式发布：`7cc46faf93fc34f0b41695acf4a4fd6d66cb3871`，计划blob `a2a68ffda2709c16959397aabc6427b60e04e9d0`，索引可执行；重读第8节后恢复。
- 实际工程：`D:/海中鱼巣/海中鱼巣.vcxproj`，main；构建输入HEAD为上述v0.3提交，不含连接提交 `c3b8c318`。
- 既有知识库不覆盖当前代码，无目标函数用途卡；直接核对当前头源、生产调用点、正式规范、计划和详细设计。复用R/U/C原子发布、既有身份保护、map联合快照去重和场景完整范围读取，没有第二owner或材料账。
- 初始index为空；原会话及相关子任务已停止。构建前无MSBuild/cl/link占用，本输出由root独占。没有恢复已暂停实测任务。

## 2. 保留现场与实际变更

原始七文件、三个未跟踪文档和二进制差异保存于 `C:/Users/zhchh/AppData/Local/Temp/HY-Ego-INDEPENDENT-TARGET-20261021/`，包括 `original-main.patch`、`original-status.txt`；没有使用reset/clean/stash/rebase/cherry-pick或覆盖WIP。除装配cpp叠加本叶增量外，其余六份既有WIP保持原始字节。

本叶六文件实现：完整请求显式目标E；U去重和带R复用包含E；部分目标不回填来源；来源／目标分别验证和保护；网页字符串target贯通表单、白名单、DTO、JSON及重述。来源主体仍由活跃会话绑定。

装配写入口完整查询现实场景范围；部分提交从来源键纯读定位旧R，交叉核验显式R，再去重核验新旧目标。读取变化正常拒绝本次写入；读取／结构／资源故障沿工程路径中止。身份保护不冒充现实归属锁。

部分结果helper改为纯映射；只读查询零投递。写入口分别返回内容确认和材料投递状态；异目标等待后继消费，旧同对象投递保留；资源失败不混同邮箱暂拒。网页确认POST的投递回执不被随后只读GET覆盖，GET失败也保留已确认事实和本次回执。

不创建D、不写对方需求集合、不建立自我需求／任务、不实现语言理解或连接自动调用。

## 3. 实际验证与未验证范围

- `git diff --check`、`git diff --cached --check`通过；index保持空，没有暂存／提交／推送生产代码。
- root独立静态审查最终六文件未确认新增P1/P2；其结论仅限源码与合同，不证明运行。
- root提取内置script并运行 `node --check`，退出0，未执行JS。临时证据 `C:/Users/zhchh/AppData/Local/Temp/HY-Ego-INDEPENDENT-TARGET-20261021/embedded-final.js`，SHA256 `420FEDC618992D5341352D8A180A18F3EB4C80E987AB82F34A230734CEAB1C48`。
- 首轮整组Build退出1，0警告、2错误、用时00:02:49.54：新增来源键纯读查询的两处UTF8校验将string_view传给既有const std::string&接口；在本叶调用处显式构造std::string修复，未改既有校验函数ABI。失败日志保留为`build-first-failed.log`，退出码为`build-first-failed-exit.txt`，输入为`source-first-failed-sha256.txt`。
- 随后同实际工程、同配置及同隔离输出完成必要增量Build与链接，退出0、0警告、0错误、用时00:00:23.26，生成`bin/海中鱼巣.exe`。最终日志为`build.log`、退出码`build-exit.txt=0`；最终源码清单`source-sha256.txt`已在修复后更新。已编译，未运行测试。
- 独立最终六文件审核后，仅结构化cpp的上述两行调用类型适配发生变化；JS、装配和其它文件未变，不将必要增量构建描述为第二次全量重编。

构建是“已发布 main＋七份接管WIP＋本叶六文件增量”的联合现场，不是纯提交基线验证，也不证明七份WIP功能验收、集成验收或整体获取服务需求业务闭环。

构建命令：

```powershell
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe' 'D:/海中鱼巣/海中鱼巣.vcxproj' /t:Build /m:1 /p:Configuration=Debug /p:Platform=x64 '/p:OutDir=D:/TEMP/海中鱼巣/SERVICE-REQUEST-INDEPENDENT-TARGET/20261021-main-v02/bin/' '/p:IntDir=D:/TEMP/海中鱼巣/SERVICE-REQUEST-INDEPENDENT-TARGET/20261021-main-v02/obj/' /fl '/flp:LogFile=D:/TEMP/海中鱼巣/SERVICE-REQUEST-INDEPENDENT-TARGET/20261021-main-v02/build.log;Verbosity=normal;Encoding=UTF-8' /nologo /verbosity:minimal
```

输出目录内的 `build-head.txt`、`build-status.txt`、`source-sha256.txt` 固定实际输入；`build.log`和`build-exit.txt`保存结果。没有复制工程或建立临时测试项目。

NOT_RUN：同对象回归、异目标完整预览／确认、R部分补齐及冲突、固定语义漂移、会话失效、目标移出范围、消息暂拒、同来源不带R重投、显式R不匹配、网页ID精度的实际运行／并发／恢复；无新测试授权，未启动产品、自检、网页或网络收发。

## 4. 函数级代码知识变更清单

| owner及位置 | 新增／修改的函数或合同 | 用途、调用边及边界 |
| --- | --- | --- |
| `业务/服务要求.结构化.h/.cpp` | `服务要求理解结果 结构化服务要求服务::建立或取得理解(const 服务要求理解请求&) noexcept` | 新必填目标E；调用既有存在身份保护；带R精确复用核验目标，不改变R/U/C事务。 |
| 同owner私有实现 | `bool 同一理解(const 服务要求理解信息&, const 服务要求理解信息&)`、`服务要求理解信息 实现::解码器::理解(稳定编码) const` | 完整等价键包含E，解码不再要求来源等于目标；存量编码不回填。 |
| 同owner私有实现 | `std::optional<服务要求确认信息> 实现::读取确认(稳定编码) const`、`void 实现::检查部分目标(const 新部分服务要求目标&) const` | 来源和目标分别读回；目标身份缺失与读取故障分流，不把目标要求为人类。 |
| 同owner | `部分服务要求结果 结构化服务要求服务::提交或补充要求(const 部分服务要求提交&) noexcept` | 解除来源等于目标的旧限制，保留来源键、冲突、原子补齐与读回。 |
| 同owner | 新增 `部分服务要求结果 结构化服务要求服务::查询部分要求(std::string_view, 稳定编码, std::string_view) const noexcept` | 同源三项精确键完整扫描；纯读，唯一新增调用者是装配部分提交；不查第一个、不创建材料或投递。 |
| 同owner | `服务要求确认结果 结构化服务要求服务::确认理解(std::string_view, 稳定编码, 稳定编码) noexcept`、`新服务要求确认保护结果 结构化服务要求服务::准备服务要求确认语义保护(稳定编码) const noexcept` | 目标身份独立验证；既有保护map合并同E快照；不保护目标全部实时特征值。 |
| `装配.普通应用.cpp`内部 | 新增 `网页服务输入状态 核验服务要求现实目标(普通应用上下文&, 稳定编码)` | 复用场景owner完整范围读取；仅预览、部分提交和确认写入口调用。 |
| 同文件内部 | `std::optional<网页服务要求显示信息> 形成服务要求显示(普通应用上下文&, const 服务要求理解信息&, std::optional<稳定编码>)` | 主体与目标分别重述，固定语义不变。 |
| 同文件内部 | `网页服务材料投递状态 投递完整服务需求材料(普通应用上下文&, 稳定编码)`、`网页部分服务要求结果 形成部分要求网页结果(普通应用上下文&, const 部分服务要求结果&)` | 投递返回强类型分账；helper不再调用投递；异目标不进入旧self。 |
| 普通应用公开入口 | `网页服务要求结果 预览普通应用服务要求(std::string_view, const 网页服务目标提交&) noexcept`、`读取普通应用服务要求预览(std::string_view, std::uint64_t) noexcept`、`确认普通应用服务要求(std::string_view, std::uint64_t) noexcept` | 来源授权保留；写前核现实范围；纯读不重新要求历史目标仍在现实；确认保留C并返回独立投递结果。 |
| 普通应用公开入口 | `网页部分服务要求结果 提交普通应用部分服务要求(std::string_view, const 网页部分服务要求提交&) noexcept` | 持上下文锁完成来源查询、显式R授权及新旧目标核验，原owner仍复核；只写入口显式投递。读取部分要求函数沿原调用只消费纯映射。 |
| `界面/宿主.网页对话.h/.cpp` | 网页完整／部分请求、结果DTO；`网页服务输入状态 解析服务目标(const 表单字段组&, 网页服务目标提交&) noexcept`、`解析部分服务目标(const 表单字段组&, 网页部分服务要求提交&)`、`bool 服务目标形状完整(const 网页服务目标提交&) noexcept` | 显式target完整必填、部分可未知；无后台来源回填，ID保持十进制字符串。 |
| 同文件内部 | 新增 `std::string_view 服务材料投递状态名称(网页服务材料投递状态) noexcept`；修改服务状态名称／HTTP映射、完整／部分结果序列化及服务路由、内置JS显示 | 独立delivery回执；纯读刷新不重试投递、不覆盖确认POST结果。JS未执行。 |

退出函数：无；共享知识索引未修改。以上函数用途卡待正式代码发布后由维护者新建／复核，本次结果尚未提交，不能登记为已发布能力。

## 5. 最终输入与保护证据

### 六文件及工程blob

| 文件 | Git blob（未写入对象） |
| --- | --- |
| `海中鱼巣/业务/服务要求.结构化.h` | `312a9a5768bb97c0c4ea2d34eeec8dd3151d3a06` |
| `海中鱼巣/业务/服务要求.结构化.cpp` | `e1c7ea275d37179203393d9f6d9e23d09b52827d` |
| `海中鱼巣/界面/宿主.网页对话.h` | `02c08f85d73ec2eab67467eaeec1d2b70c7c136f` |
| `海中鱼巣/界面/宿主.网页对话.cpp` | `a242c677a0c8342dd329e5c3f096de97997f3e7d` |
| `海中鱼巣/装配.普通应用.h` | `d2fe86e9f5224dbccc058f1e1b1824478e7e417a` |
| `海中鱼巣/装配.普通应用.cpp` | `edf79fae713473112fd13b948352257a13843914` |
| `海中鱼巣.vcxproj` | `18ca5b9f741eda2459b4719c5d637adb5862b245` |

### 七份接管WIP实际SHA256

| 文件 | 构建现场SHA256 | 原始保留 |
| --- | --- | --- |
| `海中鱼巣/业务/新_外设世界树类.cpp` | `5BE26139CA4D0149E559374E777365F60B6A722706C671BE0897E0B75A801919` | 逐字节一致 |
| `海中鱼巣/业务/服务值结算.本能运行服务.cpp` | `4EC17C6927AA241C6D133A09539D39F380E6B1BD77BCCB040004940A5445F2DF` | 逐字节一致 |
| `海中鱼巣/装配.普通应用.cpp` | `18A10C779550ABDFF80E3969FE1902547048EE0B2053C5BC582EFC23AC0737EC` | 原始装配诊断保留，其上叠加本叶增量 |
| `海中鱼巣/领域/新_任务类.cpp` | `59F39D7285A21C9CC253716DF191E5BA0982B02C678F5E8FEA90234537AD5B8C` | 逐字节一致 |
| `海中鱼巣/领域/新_存在类.cpp` | `255CEC1837F7D979B6638FBD12EDD84A585B3930A53B5CD3F9D410B67D8E828C` | 逐字节一致 |
| `海中鱼巣/领域/新_特征类.cpp` | `A96B536463ABEC318AB3F45003EB6634F8C5DBF8E1E2FB5DFAA7ADFE1991AC0B` | 逐字节一致 |
| `海中鱼巣/领域/概念_特征类.cpp` | `10ABC26F6B744356E19B5807C2716351CCA497BAE8771A87D6B4593A5EFFBB2A` | 逐字节一致 |

已知Git异常仍保留：此前读取报告commit-graph引用不可读对象，fetch自动维护报告无效snapshot引用和坏tree；本轮不清理引用／对象、不修复、不把其归为业务失败。计划发布事实由root核对；连接未合入，生产未提交／推送。
