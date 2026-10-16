# LANGUAGE-BASIC-SIX-BIDIRECTIONAL 施工与编译记录

日期：2026-10-16。身份：`LANGUAGE-BASIC-SIX-BIDIRECTIONAL`。本记录为实际代码施工与工程证据，不定义新的机器合同或计划状态。

## S0 与所有权

唯一代码执行所有者为本计划代码执行切片，任务工作树为 `D:\worktree\language-basic-six-code\海中鱼巣`，分支 `codex/language-basic-six-code`；精确起点 `9393218c3d0d1069a55bc9a37f49741b61c8e5c5`，开工时 main/origin/main 同该提交，工作树及 index 干净。已发布计划 [基础句型统一双向施工计划](../计划/LANGUAGE-BASIC-SIX-BIDIRECTIONAL_基础句型统一双向施工计划_v0.1.md) 的 blob 为 `4624eb48ba2f708c20165b227feccec9279608e2`；实现只按其 [详细设计](../规范/详细设计/20261016_LANGUAGE-BASIC-SIX-BIDIRECTIONAL_基础句型统一双向详细设计_v0.1.md) 冻结合同施工。

已读 execute 技能、AGENTS、单工作区与多工作树规则、代码知识使用规则、规范目录、0050/0600/CODE-FILE-01 和直接语言/名称/概念规范；知识库当前状态须按现行头源重建，本次直接核对现行语言、词、名称、存在/特征概念、语言世界树公开接口、既有 S01/S02/S09 与实际调用点和工程登记。没有把现实对象解释器复用于临时语言根。父线程只读审查，无第二代码写入者；其它工作树历史计划 WIP 未触碰。本切片只占自身实际项目 Debug|x64 构建资源，未占运行、日志、外设或数据库资源。

## 实际变更

新增业务内部 `转换.语言场景共同.h/.cpp`，将 UTF-8 标量映射、词条普通名称完整读取、概念当前名称完整读取、完整知识枚举及逐项读回、命名来源对齐与 S01 零写识别提取到同一位置。旧 S01 两公开接口及头文件不变，改为消费共同读取；其既有场景编排、正常状态和输出流程保留。

新增 `转换.语言场景基础.h/.cpp`，实现 S03 特征概念条件、S03 整数和 S05 组成的三构式双向入口，及 S01/S02/S03整数/S03概念/S05/S09概念归类确认六构式统一双向入口。先按完整正式知识和词条边界筛适用结构，再用只读分支取得全部候选；S02 继续调用已发布上下位图检索解释与可逆生成。多义候选完整返回，唯一前不建立场景。需要 E 的分支在复核后调用现有语言场景 owner；S03 一 E，S05 按两个出现分别实例化，同概念也保留两个不同身份。部分写入结构化非成功保留实际已取得的场景和 E；旧候选在技术读取失败或写后失配时清空。

整数接受 `[+-]?[0-9]+`，保留原词面，按字符串去正号及前导零、负零归零；未将数字转 `int64` 或作 FT 值域裁决。FT 仍通过正式根比较以及 I64材料/闭区间 variant 类型核验。S03 概念分支用 FT 根约束 C 候选。S09 仅形成内容，受询者保持空；输出复用 S02 子句后读取外层正式“吗”，不伪造受询者或建立现实事实。

反向核验旧原文、所有命名/固定词/数字范围与角色、仍存在旧名称的端点、当前场景实例/来源概念、知识和根约束。名称退出不要求重新分词原文。生成按知识有序项组织当前名称，实际生成文字必须经六构式共同零写入口唯一回读；统一入口按实际文本标量同时核验最大输出及最大回读原文额度。非成功不带文本/输出对齐/知识输出；读取故障不能被空对齐或其它可用文字掩盖。工程异常不捕获伪装正常业务结果，向上送出。

两工程文件只登记四个新增头源，未改构建配置、初始化、生产收发入口或共享 owner。改动共八个文件（四新增头源、既有 S01 cpp、两工程文件及本记录）。

## 实际验证

在本任务工作树实际 `海中鱼巣.slnx` 运行：

```text
C:\Program Files\Microsoft Visual Studio\18\Professional\MSBuild\Current\Bin\MSBuild.exe 海中鱼巣.slnx /t:Build /p:Configuration=Debug /p:Platform=x64 /m:4 /v:minimal /flp:logfile=x64\Debug\language-basic-six-build.log;verbosity=normal
```

首次完整构建发现新 cpp 紧邻中文标识符的三元问号触发 MSVC 词法错误，已在本文件修正；随后对一组实现及独立审查修正统一增量构建，实际编译三个转换源并链接 `D:\worktree\language-basic-six-code\海中鱼巣\x64\Debug\海中鱼巣.exe`，最终退出 0、0 警告、0 错误。构建日志保留在本工作树忽略输出目录 `x64/Debug/language-basic-six-build.log`，不作为新仓库文件提交。差异/暂存差异按 `git diff --check` 与 `git diff --cached --check` 核对。

**已编译，未运行测试、自检、冒烟、程序启动或验收。** 用户没有下达本切片测试指令。静态复核覆盖只读先行、名称/知识完整读取、来源篡改校验、同概念双 E、数字字符串、实例身份回传、输出实际回读和两项额度。编译不证明实际节点创建、功能运行、恢复、生产交互接线或现实信息采信；未发布表项、代词、复合句和完整自然语言能力不属于本次交付。

## 代码知识变更清单

结果提交：本记录随代码同一提交，精确提交身份由本任务交付回传和 Git 记录定位，不在提交内容中自引用其自身哈希。待共享知识维护者基于发布后代码新建/复核以下用途卡；本切片没有修改共享知识库。

四个新增公开入口均在 `海中鱼巣/业务/转换.语言场景基础.h/.cpp` 声明/定义：`转换单句基础结构到空白语言场景`、`从语言场景基础含义生成可回读短句`、`转换单句基础语言到信息`、`从基础信息生成可回读单句`。完整参数/结果 DTO 与签名见该头文件，均为 DATA 外业务自由函数；输入参数为调用期值式请求及具名正式服务引用。两个正向函数仅编排语言世界树写入，两个反向函数全程只读，异常向上送出。主张、候选、来源与对齐生命周期均为返回值，调用者保存 DTO 才保有关系主张，不持久化新事实关系。

共同内部函数位于 `海中鱼巣::语言场景转换内部`，头只由两个业务 cpp 包含；主三构式和六分支私有函数位于基础 cpp 匿名命名空间。以下按实际定义列出完整签名，内部类型别名以各 cpp 为准；这些不是新的 DATA 服务或公开知识执行入口。

### 转换.语言场景共同.cpp

新增/移动用途：正式材料读取、UTF-8范围、来源核验、S01零写识别及其结构/状态/候选辅助；调用现行语言、词、名称、概念公开读接口，不写场景。S01的原只读解释和同职责读取helper从归类cpp退出，由共同头源唯一承载；构式谓词也改为共同内部唯一实现。

```cpp
读取状态 名称状态(概念名称关系查询状态 状态);
读取状态 指代状态(指代名称读取状态 状态);
读取状态 知识状态(新语言知识状态 状态);
名称读取结果 读取名称组(const std::vector<稳定编码>& 节点组,
    std::optional<稳定编码> 词条限定, std::optional<稳定编码> 概念限定,
    稳定编码 语言节点, const 新_词类& 词服务, const 概念_名称关系类& 名称服务,
    const 概念_存在类& 存在服务, const 概念_特征类& 特征服务);
std::optional<原文映射> 解码语言场景原文(const std::string& 原文);
std::string 读取语言场景原文范围(const 原文映射& 原文, 解释原文范围 范围);
名称读取结果 读取词条普通名称完整(稳定编码 词条节点, 稳定编码 语言节点,
    const 新_词类& 词服务, const 概念_名称关系类& 名称服务,
    const 概念_存在类& 存在服务, const 概念_特征类& 特征服务);
名称读取结果 读取概念当前名称完整(稳定编码 概念节点, 稳定编码 语言节点,
    const 新_词类& 词服务, const 概念_名称关系类& 名称服务,
    const 概念_存在类& 存在服务, const 概念_特征类& 特征服务);
知识读取结果 读取语言场景基础知识完整(稳定编码 语言节点, std::size_t 最大条目数,
    const 新_语言类& 语言服务, const 新_词类& 词服务, const 概念_存在类& 存在服务);
读取状态 核验语言场景命名来源(const 原文映射& 原文, const std::vector<命名来源项>& 来源,
    稳定编码 语言节点, const 新_词类& 词服务, const 概念_名称关系类& 名称服务);
bool 是S01知识(const 新语言知识信息& 知识, 稳定编码 语言节点);
输入状态 映射读取失败(读取状态 状态);
输入状态 映射语言失败(新语言查询状态 状态);
输入状态 读取类名候选(稳定编码 词条节点, 稳定编码 语言节点,
    const 新_词类& 词服务, const 概念_名称关系类& 名称服务,
    const 概念_存在类& 存在概念服务, const 概念_特征类& 特征概念服务,
    std::vector<名称候选>& 结果);
std::optional<std::array<新语言词条匹配片段, 3>> 读取三词布局(
    const std::vector<新语言词条匹配片段>& 片段组,
    const std::string& 原文);
bool 候选顺序(const 语言场景归类候选& 左,
    const 语言场景归类候选& 右);
S01只读结果 只读识别语言场景S01(
    const 语言场景归类输入请求& 请求,
    const 新_语言类& 语言服务, const 新_词类& 词服务,
    const 概念_名称关系类& 名称服务,
    const 概念_存在类& 存在概念服务,
    const 概念_特征类& 特征概念服务);
```

### 转换.语言场景基础.cpp

新增用途与直接调用边：统一正向→六类零写识别→共同S01/现有S02/S09/三构式解释；唯一后→既有S01写编排或新三构式写编排→语言场景建立/逐E实例化/正式读回。统一反向→既有S01输出或新三构式输出或纯概念S02/S09内容组织→统一输出核验→六类零写识别。三构式解释→正式知识/完整名称→FT根比较；各生成函数→按已读知识排列文字→零写解析回读。快照/键/状态映射/DTO/来源/实例核验helper只服务这些入口，不取得数据所有权或跨请求缓存。

```cpp
I 输入映射(读取状态 s);
O 输出映射(I s);
bool 技术未完成(I s);
bool 范围相同(解释原文范围 a, 解释原文范围 b);
bool 知识相同(const 新语言知识信息& a, const 新语言知识信息& b);
std::optional<std::string> 规范数字(const std::string& 原文);
bool 角色定义相同(const 新语言知识目标描述& d, std::int64_t n,
    新语言知识角色种类 kind, const std::vector<K>& cats);
std::optional<U> 知识构式(const 新语言知识信息& k, 稳定编码 lang);
const 新语言知识信息* 找知识(const std::vector<新语言知识信息>& ks, 稳定编码 id);
std::vector<叶项> 展开知识(const 新语言知识信息& k, const std::vector<新语言知识信息>& ks, bool 外层=false);
布局结果 匹配叶布局(const std::string& text, 稳定编码 lang, const std::vector<叶项>& leaves,
    const 只读上下文& c);
auto 候选键(const 语言场景基础候选& x, bool 证据=true);
bool 候选相同(const 语言场景基础候选& a,const 语言场景基础候选& b);
bool 快照相同(const 读取快照& a,const 读取快照& b);
I 根状态(特征概念规则状态 s);
只读结果 解释一个构式(const 语言场景基础输入请求& q,const 只读上下文& c,U form);
只读结果 只读解释语言场景S03概念(const 语言场景基础输入请求& q,const 只读上下文& c);
只读结果 只读解释语言场景S03整数(const 语言场景基础输入请求& q,const 只读上下文& c);
只读结果 只读解释语言场景S05(const 语言场景基础输入请求& q,const 只读上下文& c);
只读结果 只读分派语言场景基础结构(const 语言场景基础输入请求& q,const 只读上下文& c);
const 语言场景基础命名出现& 命名(const 语言场景基础候选& x,std::size_t i);
U 统一构式(F f);
bool DTO形状有效(const 语言场景基础候选& x);
I 核验叶覆盖(const 原文映射& map,const std::vector<语言基础统一对齐>& align,const std::vector<叶项>& leaves,
    稳定编码 lang,const 新_词类& words,const 概念_名称关系类& names);
std::vector<语言基础统一对齐> 基础对齐(const 语言场景基础候选& x,const 新语言知识信息& k);
I 核验基础来源(const 语言场景基础含义& meaning,稳定编码 lang,const 只读上下文& c,
    const std::vector<新语言知识信息>& ks);
I 核验实例绑定(const 语言场景基础含义& m,const 新_自然语言世界树类& world);
生成文字候选 按基础知识生成文字(const 语言场景基础候选& x,const 新语言知识信息& k,
    const std::vector<生成项选择>& choices,bool spaced);
生成文字候选 生成语言场景S03概念文字(const 语言场景基础候选& x,const 新语言知识信息& k,const std::vector<生成项选择>& s,bool space);
生成文字候选 生成语言场景S03整数文字(const 语言场景基础候选& x,const 新语言知识信息& k,const std::vector<生成项选择>& s,bool space);
生成文字候选 生成语言场景S05文字(const 语言场景基础候选& x,const 新语言知识信息& k,const std::vector<生成项选择>& s,bool space);
语言场景基础输入结果 转换单句基础结构到空白语言场景(const 语言场景基础输入请求& q,
    新_自然语言世界树类& world,const 新_语言类& lang,const 新_词类& words,
    const 概念_名称关系类& names,const 概念_存在类& es,const 概念_特征类& fs);
语言场景基础输出结果 从语言场景基础含义生成可回读短句(const 语言场景基础输出请求& q,
    const 新_自然语言世界树类& world,const 新_语言类& lang,const 新_词类& words,
    const 概念_名称关系类& names,const 概念_存在类& es,const 概念_特征类& fs);
I S01状态(语言场景归类输入状态 s);
O S01输出状态(语言场景归类输出状态 s);
I S02状态(基础概念上下位输入状态 s);
I S09状态(基础概念归类确认输入状态 s);
U 候选构式(const 语言基础统一候选& v);
统一键 统一候选键(const 语言基础统一候选& v,bool names=false);
bool S02完全相同(const 基础概念上下位候选&a,const 基础概念上下位候选&b);
bool 统一候选相同(const 语言基础统一候选&a,const 语言基础统一候选&b);
语言基础统一输入结果 只读识别六类基础句型(const 语言基础统一输入请求& q,
    const 新_语言类& lang,const 新_词类& words,const 概念_名称关系类& names,
    const 概念_存在类& es,const 概念_特征类& fs,const 新_存在类& instances);
const 基础概念上下位候选& 纯概念主体(const 语言基础纯概念含义& p);
稳定编码 纯概念知识(const 语言基础纯概念含义& p);
对齐读取结果 构造纯概念对齐(const std::string& text,稳定编码 lang,
    const std::variant<基础概念上下位候选,基础概念归类确认候选>& v,const 新语言知识信息& k,
    const std::vector<新语言知识信息>& ks,const 只读上下文& c);
I 核验纯概念来源(const 语言基础纯概念含义& p,稳定编码 lang,
    const std::vector<新语言知识信息>& ks,const 只读上下文& c);
语言基础统一候选 含义候选(const 语言基础统一含义& m);
语言基础统一输入结果 转换单句基础语言到信息(const 语言基础统一输入请求& q,
    新_自然语言世界树类& world,const 新_语言类& lang,const 新_词类& words,const 概念_名称关系类& names,
    const 概念_存在类& es,const 概念_特征类& fs,const 新_存在类& instances);
纯概念生成结果 生成纯概念子句(const 语言基础统一输出请求& q,const 基础概念上下位候选& b,
    const 新_语言类& lang,const 新_词类& words,const 概念_名称关系类& names,
    const 概念_存在类& es,const 概念_特征类& fs,const 新_存在类& instances);
语言基础统一输入请求 回读请求(const 语言基础统一输出请求&q,const std::string&text);
O 统一输出核验(const 语言基础统一输出请求&q,const std::string&text,const 语言基础统一候选& expected,
    const 新_语言类& lang,const 新_词类& words,const 概念_名称关系类& names,
    const 概念_存在类& es,const 概念_特征类& fs,const 新_存在类& instances,
    std::optional<语言基础统一候选>& actual);
语言基础统一输出结果 生成无受询者概念归类确认文字(const 语言基础统一输出请求&q,
    const 新_语言类& lang,const 新_词类& words,const 概念_名称关系类& names,
    const 概念_存在类& es,const 概念_特征类& fs,const 新_存在类& instances);
语言基础统一输出结果 从基础信息生成可回读单句(const 语言基础统一输出请求&q,
    const 新_自然语言世界树类& world,const 新_语言类& lang,const 新_词类& words,const 概念_名称关系类& names,
    const 概念_存在类& es,const 概念_特征类& fs,const 新_存在类& instances);
```

### 转换.语言场景归类.cpp

修改用途：两公开入口改调共同零写识别，来源/当前名称读取改为共同能力；公开签名和已有业务语义保持。退出本文件旧 `只读解释语言场景S01`、`读取类名候选`、`读取三词布局`、UTF-8映射及重复知识/名称读取职责，S01自身候选等值与结果流程保留。

```cpp
bool 合法范围(解释原文范围 范围, std::size_t 总数);
输出状态 核验来源对齐(const 语言场景归类含义& 含义, 稳定编码 语言节点,
    const 新_词类& 词服务, const 概念_名称关系类& 名称服务);
bool 候选相同(const 语言场景归类候选& 左,
    const 语言场景归类候选& 右);
输出状态 映射输出失败(输入状态 状态);
输出状态 读取当前名称(稳定编码 概念节点, 稳定编码 语言节点,
    const 概念_名称关系类& 名称服务, const 新_词类& 词服务,
    const 概念_存在类& 存在服务, const 概念_特征类& 特征服务,
    std::vector<std::pair<稳定编码, 新词条信息>>& 结果);
语言场景归类输入结果 转换单句归类到空白语言场景(
    const 语言场景归类输入请求& 请求, 新_自然语言世界树类& 语言世界,
    const 新_语言类& 语言服务, const 新_词类& 词服务,
    const 概念_名称关系类& 名称服务,
    const 概念_存在类& 存在概念服务,
    const 概念_特征类& 特征概念服务);
语言场景归类输出结果 从语言场景归类含义生成可回读短句(
    const 语言场景归类输出请求& 请求,
    const 新_自然语言世界树类& 语言世界,
    const 新_语言类& 语言服务, const 新_词类& 词服务,
    const 概念_名称关系类& 名称服务,
    const 概念_存在类& 存在概念服务,
    const 概念_特征类& 特征概念服务);
```

## 交付边界

本地仅提交本切片八文件，不推送、不修改 main、不改计划/设计/规范/规则或共享索引。main 集成和普通推送交唯一发布者完成，工作树编译不自动升级为 main 集成编译或验收通过。
