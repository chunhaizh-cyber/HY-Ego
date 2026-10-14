# INIT-PRONOUN-CONCEPT-NAME-SEEDS 指代标准声明与初始化编译验证 v0.1

日期：2026-10-14。性质：具名代码叶施工及实际工程编译记录；不证明运行、恢复、并发、集成验收或自然语言双向转换。

## S0 与施工边界

- 计划：`计划/INIT-PRONOUN-CONCEPT-NAME-SEEDS_v0.1.md`，精确 blob `81b0bb6ba05de32a61747cb6997f972ab16a3975`；开工索引为唯一`可执行`。定义代码 `499c27c6`、名称提供者主线集成 `26303af3` 已在起点祖先，公开 DTO、完整概念名称持有及词服务同实例语言 getter 与计划相符。
- 代码工作树：`D:/worktree/init-pronoun-seeds-code/海中鱼巣`，分支 `codex/init-pronoun-seeds-code`，起点 `fe43738df6a140116c1979167c92f93078a1ea8c`；开工时 main/origin/main 均为该提交，独立 index 和工作树干净。当前主线后续前进由唯一集成者复核；本代码叶不自行集成或推送。
- 唯一代码所有者仅修改计划第7节的解释器头源、新业务头源、六行标准文本及 README、普通装配头源、实际项目/filters 和本记录。09fc 代码叶的存在/特征、服务对象文件与本叶无文件或语义重叠；本叶构建输出使用独立 `D:/TEMP/海中鱼巣/INIT-PRONOUN-CONCEPT-NAME-SEEDS/20261014-code-build1/`。
- 现行知识库无覆盖此 HEAD 的完整用途卡或调用图。复用结论直接依据现行头源、公开调用点与工程登记：扩展唯一 `解释初始化标准文本`；组合现有指代定义、名称、词条、语言及先天来源公开入口，不新建事实 owner、第二解析器、L1 写入或场景实例。

## 已发生变更

- 解释器同入口追加指代声明模板、强类型载荷、源角色、诊断与内部逆向核验。`R/T/P/K` 均由空格分词的文本词元解码；仅 `C` 建立一个逐出现指代概念符号。未知枚举、乱序/重复参数、不相容规则及非空约束分别拒绝。新枚举尾部追加，旧值、默认额度及六个旧模板保留。
- 六行 `指代定义种子.txt` 与计划第3节正文逐字节相同。业务先整批解释、六词范围、正式中文及同实例服务、定义和词条前置读取，再显式初始化指代结构、取得同一先天来源、按六槽建立或复用定义/词/首选名称。每个写调用记录入口、返回、身份、状态和阶段；源/词旧提供者失败后只以原请求作一次公开确认，保留原结果和工程异常，不把查询身份称为本次发布。每槽及全批经定义、词条、名称关系、概念反向持有和词条候选四路完整复核。
- 普通应用把第四文本放在上下文锁前读取；首次在语言知识之后调用写入口，相同配置的重复构造只调用 const 服务只读入口。结果先移入上下文再复制给装配返回，工程异常及未闭合状态进入现有终止诊断。工程登记、filters 和原复制 Target 增加第四资源，部署字节与源一致。
- 此叶没有创建实际说话者/受话者、场景、指代实例、目标或语言组合知识；没有修改定义、名称、词、语言、L1 提供者或其它任务文件。

## 函数级代码知识变更清单

下列函数为新增或修改，供代码正式发布后知识维护者按结果提交复核；本叶没有退出函数，也未写共享用途卡。

| 完整身份 | 用途、直接调用边和事实边界 |
| --- | --- |
| `std::optional<指代声明规则> 解析指代规则词元(std::string_view) noexcept`、`std::optional<指代目标范围> 解析指代目标范围词元(std::string_view) noexcept`、`std::optional<指代参数角色> 解析指代参数角色词元(std::string_view) noexcept`、`std::optional<指代参数类型> 解析指代参数类型词元(std::string_view) noexcept` | 解释器私有枚举解码；由同一文本入口的行解析及材料核验调用，零领域读写。 |
| `bool 指代声明参数相容(const 指代概念定义&) noexcept`、`bool 指代行结构匹配(const 行视图&) noexcept` | 解释器私有模板形状和规则—参数闭集核验；前者同时被行解析和逆向核验调用，纯值式。 |
| `void 核验完整材料(const 初始化定义材料&, std::size_t)` | 既有私有材料核验增加指代载荷、角色范围、固定词间隔和逆向枚举互证；由原解释入口内部调用。 |
| `初始化最小解释结果 解释标准文本内部(const std::string&, const 初始化最小资源额度&)` | 既有共同词法/六模板分派中增加指代行分派，首个具体错误词元可定位时定位该词元；最终仍只返回整批材料或拒绝。唯一公开调用者仍是`解释初始化标准文本`。 |
| `初始化最小解释结果 解释初始化标准文本(const std::string&, const 初始化最小资源额度&)` | 公开签名及原委托保持，所委托内部解释器现可返回指代定义材料；其新增生产调用方为指代种子业务，其它旧消费者仍拒绝不属于自己范围的目标。 |
| `bool 拒绝(初始化指代种子结果&, 初始化指代种子状态, std::string_view)`、`bool 工程拒绝(初始化指代种子结果&, std::string_view)` | 业务私有语义未完成与工程控制分账；由下述共同前置/发布/读回辅助调用，不写事实。 |
| `bool 定义查询可用(const 指代概念查询结果&) noexcept`、`bool 词条查询可用(const 新词条查询结果&) noexcept`、`bool 同一来源(const 新语言知识来源信息&, const 新语言知识来源请求&) noexcept`、`bool 同一来源(const 新语言知识来源信息&, const 新语言知识来源信息&) noexcept`、`bool 同一名称(const 指代名称关系信息&, const 指代名称关系信息&) noexcept`、`std::string_view 词面(const 初始化指代种子结果&, std::size_t)` | 私有完整结果、来源、名称及源范围比较；用于前置读取、发布确认和四路读回，零写入。 |
| `bool 准备首批(const 初始化指代种子请求&, 初始化指代种子结果&)`、`bool 核验语言(const 初始化指代种子请求&, const 新_语言类&, const 新_词类&, 初始化指代种子结果&)`、`bool 预查(const 初始化指代种子请求&, const 新_词类&, const 概念_存在类&, 初始化指代种子结果&, bool)` | 同一解释入口、六槽已确认范围、中文/合同/同实例和定义/词条完整前置；由两公开业务入口的共同行为调用。词面—规则表只作首批门禁，不构造定义。 |
| `bool 核验来源(const 初始化指代种子请求&, const 新_语言类&, 初始化指代种子结果&, const 新语言知识来源请求&)`、`bool 完整核验项(const 初始化指代种子请求&, const 新_词类&, const 概念_存在类&, const 概念_名称关系类&, const 新语言知识来源信息&, 初始化指代种子结果&, std::size_t, std::optional<稳定编码>)` | 按原文键/身份读取来源，按定义、词条、关系及两份完整列表读回同一名称并互证反向持有；由共同行为逐槽及终轮调用，准确未找到与技术失败分账。 |
| `void 确认未返回的写入(const 新语言知识来源请求&, const std::array<std::optional<新词条建立请求>, 6>&, 语言服务&, 词服务&, 初始化指代种子结果&) noexcept` | 源/词写入口异常后各用原请求一次公开只读确认，保留原状态或读取异常；由共同行为最终 catch 调用，不重试写入。 |
| `初始化指代种子结果 处理指代种子<只读>(...)`、`初始化指代种子结果 初始化指代概念名称(const 初始化指代种子请求&, 新_语言类&, 新_词类&, 概念_存在类&, 概念_名称关系类&)`、`初始化指代种子结果 读取初始化指代概念名称(const 初始化指代种子请求&, const 新_语言类&, const 新_词类&, const 概念_存在类&, const 概念_名称关系类&)` | 唯一业务协调：首次分阶段公开写/读及重复 const 零 CRUD 读回；公开入口由普通装配 helper 调用，结果值式记录调用事实和工程异常，不取得概念/名称/词/语言事实所有权。 |
| `void 初始化普通应用指代种子(普通应用上下文&, 普通应用装配结果&, const std::string&, bool) noexcept` | 普通装配私有第四批首次/重复入口；由`完成装配`和`构造普通应用上下文`调用，保存真实结果后诊断中止非成功。 |
| `普通应用装配结果 完成装配(普通应用上下文&, const std::string&, const std::string&, const std::string&, const std::string&) noexcept`、`bool 普通应用装配结果::成功() const noexcept`、`普通应用装配结果 构造普通应用上下文(const 普通应用配置&) noexcept` | 修改既有生产调用边：语言知识后首次指代初始化、成功条件增加六槽完整复核、锁前第四文件读取及重复 const 读回；原三批及其它装配合同仍保留。 |

## 实际验证与未验证范围

- `git diff --check` 无输出；精确 staged 差异检查在提交前读回。六行文本 UTF-8 无 BOM、LF、尾部 LF，字节数 1183；与计划第3节代码块文本逐字节相等。实际项目和 filters XML 可解析，新增头/源及资源各登记一次，原三资源及四配置构建选项未变。部署 `指代定义种子.txt` 与源码 SHA-256 均为 `98D22F29847FE2F416E35FD0FB52760E7865DDB77D3804B3EAA46F299BC3243D`。
- 实际工程：本工作树的 `海中鱼巣.vcxproj`，MSBuild 18 Professional，`Debug|x64`，`/t:Build /p:BuildProjectReferences=false /m:2`，独立 OutDir/IntDir。初稿构建及按只读复核修正后的最终构建均退出 0；最终 `build-final2.log` 记录链接 `bin/海中鱼巣.exe`、0 警告、0 错误。最终命令：

```powershell
$buildDir='D:/TEMP/海中鱼巣/INIT-PRONOUN-CONCEPT-NAME-SEEDS/20261014-code-build1'
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe' '海中鱼巣.vcxproj' '/t:Build' '/p:Configuration=Debug' '/p:Platform=x64' '/p:BuildProjectReferences=false' ('/p:OutDir=' + $buildDir + '/bin/') ('/p:IntDir=' + $buildDir + '/obj/') '/m:2' '/v:minimal' '/nologo' '/fl' ('/flp:LogFile=' + $buildDir + '/build-final2.log;Verbosity=normal;Encoding=UTF-8')
```

- 独立只读源码复核者依据计划逐项指出旧枚举值、来源/词条调用时点、准确未找到、确认状态保存、诊断和错误词元定位问题；本切片已修正并获其最终静态读回，无剩余已确认问题。该复核者未构建或运行程序。
- 用户未发出测试、自检、回归或验收指令：**已编译，未运行测试**。未验证真实六项创建/复用、重复只读、并发竞争、来源/名称冲突、异常恢复、进程重启或双向自然语言转换。进程内事实保留不等于持久恢复；提交及之后的 main 集成也不自动证明业务闭环。

## 发布边界

本记录随上述白名单产物由代码工作树所有者精确本地提交。工作树提交身份由交接消息给出；main 集成与远端普通推送由顶层唯一发布者执行。知识维护者应在 main 发布后按上表复核用途卡及当前性，本代码切片不修改共享知识索引。

## v0.2 设计退回恢复与最终字节证据

- 因 `core.autocrlf=true` 下新资源缺少固定 LF 检出属性，v0.1 WIP 暂停提交；计划所有者已将同一计划更新为 v0.2 并在 main/origin/main `b8a8adef7af4ec3dedb59b7a72c2d477a194be05` 发布，索引登记精确 blob `cebe4bca80dcdd5f2204562c15e88034ee6de406`、状态 `可执行`。本工作树保留原起点 `fe43738df6a140116c1979167c92f93078a1ea8c` 和原 11 文件 WIP；重新 S0 读回 v0.2 第7—9节。main 此间新增的代码改动在 `新_存在类`、`新_特征类`和服务对象材料等其它文件；本叶目标生产文件、公开指代合同及 `.gitattributes` 旧行未被主线修改。本切片依新白名单只补一条精确属性，不吸收其它所有者 WIP。
- `.gitattributes` 仅增加 `初始化材料/指代定义种子.txt text eol=lf`，原有 `最小语言知识种子.txt` 行保持；未改通配规则或 `core.autocrlf`。`git check-attr text eol` 显示 `text: set`、`eol: lf`；`git ls-files --eol` 显示 `i/lf w/lf attr/text eol=lf`。同时移除新 CPP 末尾多余空行，未改业务内容。
- 按主线 v0.2 计划第3节六行正文逐字节比较：计划正文、本工作树文件、暂存文本 blob、下述实际 Build 的 `bin/初始化材料/指代定义种子.txt` 四方全等；均 1183 字节、UTF-8 无 BOM、CR 0、LF 6、末尾恰一个 LF，SHA-256 均为 `98D22F29847FE2F416E35FD0FB52760E7865DDB77D3804B3EAA46F299BC3243D`。本证据不冒称 main 集成后的实际检出核验；该项由集成发布者在 main 检出后执行。
- 属性及末尾格式修复后的本工作树实际项目 `海中鱼巣.vcxproj` 再次执行 `Debug|x64` 的 `/t:Build /p:BuildProjectReferences=false /m:2`，沿用专属 OutDir/IntDir；`D:/TEMP/海中鱼巣/INIT-PRONOUN-CONCEPT-NAME-SEEDS/20261014-code-build1/build-v02-final.log` 记录成功链接 `bin/海中鱼巣.exe`，0 警告、0 错误。用户未授权运行测试：**已编译，未运行测试**；没有取得六项初始化真实发布、重复只读或恢复的运行证据。