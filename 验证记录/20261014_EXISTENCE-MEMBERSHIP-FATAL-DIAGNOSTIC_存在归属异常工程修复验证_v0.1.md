# 存在归属读取异常工程修复

日期：2026-10-14。身份：`EXISTENCE-MEMBERSHIP-FATAL-DIAGNOSTIC`。本记录是工程实施与编译证据，不是运行验收。

## 1. 授权、S0与范围

用户明确要求代码层面的逻辑错误经日志、弹窗和调试定位修复，不传为项目逻辑问题；进一步选择“记录日志、弹窗后终止进程，修复代码再启动”。工程规则已在`88527f24e4e5926eebe6b529dd9990f8ff950089`发布。

冻结计划v0.1：提交`c263d3c9d8fd0d2924a5a891d626201219612f7b`，blob`1d525dc57ab12f43e873299233cd4a2556b3f046`。索引可执行登记及代码S0：`36ea723f7c01b95884a48639a85b04c6165997b3`，`D:/海中鱼巣`、main、与origin/main一致。index空，无异主WIP；顶层持有四生产文件、证据记录、构建资源和Git发布。计划子智能体只写计划/索引；独立代码复核只读，不构建或测试。

允许生产范围：`核心/容错检查.h`、`核心/基础数据集.cpp`、`领域/新_存在类.cpp`、`领域/概念_存在类.cpp`。实际仅这些文件及本记录发生修改，没有公开读取签名、DTO、方法/任务、自我装配、工程登记、持久化或日志格式变更。

## 2. 实际变化与静态复核

`工程异常终止()`不受可选容错宏控制，固定容量同步复制文件、入口、原因及what()，不保存异常对象或业务引用。首故障标志/副本为header内ODR共享的inline工程诊断材料；不是业务节点或错误事实仓。

诊断线程经`_beginthreadex`创建，只读取固定副本，尝试逻辑错误日志、调试器输出和MessageBox；弹窗关闭后通过TerminateProcess终止，API失败则__fastfail。日志异常不会形成业务值，也不阻止后续终止。诊断线程不调用领域owner，故障线程不返回、不展开业务析构，只等待进程终止；创建诊断线程失败时直接终止，不在故障线程同步输出。没有新业务等待状态、结算或补造结果。

四文件内九个读取入口的catch已退出原false/nullopt/空集合默认返回，改为分别捕获std::exception携what及未知异常，进入同一致命工程出口。正常未找到、不符合及合法结构拒绝分支保持原代码。

独立复核发现初版在诊断线程创建失败时由故障线程同步OutputDebugString；这可能处于外层持锁调用中。顶层已在最终代码删除该输出，创建失败直接强制终止，未扩大计划范围。

## 3. 验证命令与结果

输入：S0提交加本叶四生产文件WIP；不是纯提交树验收。输出归本叶独占，编译前没有其它MSBuild/cl/link进程。

```powershell
git diff --check
git diff --cached --check
& 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe' 'D:/海中鱼巣/海中鱼巣.vcxproj' /t:Build /p:Configuration=Debug /p:Platform=x64 /p:BuildProjectReferences=false /p:OutDir=D:/TEMP/海中鱼巣/EXISTENCE-MEMBERSHIP-FATAL-DIAGNOSTIC/20261014-A/bin/ /p:IntDir=D:/TEMP/海中鱼巣/EXISTENCE-MEMBERSHIP-FATAL-DIAGNOSTIC/20261014-A/obj/ /m:1 /nologo /verbosity:minimal
```

构建结果：首轮实际工程完整编译链接退出码0；移除线程创建失败分支的同步调试器输出后，同命令增量编译链接退出码0。两次输出均未出现警告或错误，产物为本轮bin/海中鱼巣.exe。差异检查通过，仅既有LF/CRLF提示。未运行程序、自检、故障注入、弹窗/日志观察、并发故障、Release/Win32或重启恢复；均NOT_RUN。本轮没有测试授权。

## 4. 函数级代码知识变更清单

以下九入口的完整签名未变；正常只读/未命中结果不变。唯一新增调用边为catch→工程异常终止，退出异常→业务默认值边：

| 完整入口 | 文件与原异常结果 |
| --- | --- |
| `std::optional<基础节点> 基础数据集::查询节点(稳定编码) const noexcept` | 核心/基础数据集.cpp；nullopt |
| `bool 新_存在类::是存在节点(稳定编码) const noexcept` | 领域/新_存在类.cpp；false |
| `bool 新_存在类::存在节点属于概念(稳定编码, 稳定编码) const noexcept` | 同上；false |
| `bool 新_存在类::存在概念包含(稳定编码, 稳定编码) const noexcept` | 同上；false |
| `std::optional<稳定编码> 新_存在类::查询专属存在概念(稳定编码) const noexcept` | 同上；nullopt |
| `bool 概念_存在类::是存在概念节点(稳定编码) const noexcept` | 领域/概念_存在类.cpp；false |
| `bool 概念_存在类::是专属存在概念节点(稳定编码) const noexcept` | 同上；false |
| `bool 概念_存在类::是抽象存在概念节点(稳定编码) const noexcept` | 同上；false |
| `std::vector<稳定编码> 概念_存在类::查询上位存在概念(稳定编码) const noexcept` | 同上；空集合 |

新增header-only入口：`[[noreturn]] inline void 工程异常终止(const wchar_t*, int, const wchar_t*, const wchar_t*, const char* = nullptr) noexcept`，物理位置核心/容错检查.h，诊断责任本地记录一次；数据来源静态调用标识及捕获异常，副作用仅人读诊断和进程终止。

内部辅助同置`工程诊断内部`命名空间：`复制文本`（有界字符数组复制/截断）、`转换异常说明`（UTF-8或有界十六进制摘要）、`输出致命诊断`（CRT线程入口，独立诊断/终止）、`强制终止`（TerminateProcess及__fastfail，无返回）。它们不消费业务服务或发布世界信息。原容错宏/追根因检查的正常开关行为保留，字符串化/宽化宏移至公共位置供默认构建使用。

相关依据为用户裁决、CODE-FILE-01 v4.3第4.4节及LOG-01 v0.6第2.1节。上述入口当前无对应用途卡，已直接回读现行源码及生产调用点；用途知识待发布后由维护角色更新，本轮不写共享知识索引。没有业务函数退出。

## 5. 完成与风险边界

九入口C++异常不再返回可被消费为归属否定的值，不等于全仓异常策略已迁移。未抛异常的既有结构拒绝分支未重分类；其它不在本链的读取入口仍可能吞异常，本叶未修改。

基础数据集::查询节点为全项目共享入口，本次影响其全部调用者的异常路径；正常不存在节点仍返回nullopt。诊断输出为尽力操作，资源不足时可能来不及写日志或弹窗，但异常路径仍终止，不能宣称每次日志必定落盘。致命终止不会执行业务收尾，尚未持久化的数据可能丢失，本叶不新增恢复保证。

本叶不建立服务对象判断本能方法，不证明人类识别、方法运行、实际异常诊断、重启恢复或服务治理闭环。结果提交以本记录Git历史追溯，计划及索引退出由计划所有者办理。
