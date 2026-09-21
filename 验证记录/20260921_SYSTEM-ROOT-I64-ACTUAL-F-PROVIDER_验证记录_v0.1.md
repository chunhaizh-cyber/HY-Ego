# SYSTEM-ROOT-I64-ACTUAL-F-PROVIDER 验证记录 v0.1

日期：2026-09-21

计划：`SYSTEM-ROOT-I64-ACTUAL-F-PROVIDER` v1.0

计划 blob：`c967a515b70a0302ce1380677a8c7cde32b4b8fd`

验证基线：`7054d003c5138d8837103c0e1f8f1e2af45ac274` 加本计划未提交白名单 WIP

结果提交：待填；本记录与代码结果同一提交发布。

仓外隔离根：

- `D:\TEMP\海中鱼巣\SYSTEM-ROOT-I64-ACTUAL-F-PROVIDER\20260921-7054d003c-run7`
- `D:\TEMP\海中鱼巣\SYSTEM-ROOT-I64-ACTUAL-F-PROVIDER\20260921-7054d003c-run8`

## 1. 构建与头文件独立翻译

### 1.1 Debug x64 隔离全量 Rebuild

使用 `MSBuild.exe 海中鱼巣.vcxproj /t:Rebuild /m`，配置 `Debug|x64`，通过仓外 targets 仅以 `instinct_probe.cpp`替换普通应用实现单元和入口实现单元，`OutDir/IntDir`均指向 run8 隔离目录。

结果：退出码 0；未输出编译警告或错误；生成 `run8/debug-out/海中鱼巣.exe`。

### 1.2 Release x64 隔离全量 Rebuild

同样使用当前 WIP 和仓外探针，配置 `Release|x64`，`OutDir/IntDir`指向 run7 隔离目录。

结果：退出码 0；未输出编译警告或错误；生成 `run7/release-out/海中鱼巣.exe`。工具输出的“无可用旧 IPDB/IOBJ，回退完整编译”是全量代码生成信息，不是警告或错误。

### 1.3 目标头独立 C++20 翻译

使用 MSVC 14.51 x64，以 `/std:c++20 /permissive- /EHsc /utf-8`独立编译仓外 `header_probe.cpp`；该文件只包含 `业务/应用服务.特征概念类.h`。

结果：退出码 0，生成 `run8/header_probe.obj`。

## 2. 全新数据库首次形成与第二进程恢复

### 2.1 Debug 首次进程

数据库：`run8/db`，运行前不存在权威快照。

结果：退出码 0。

```text
PASS status=1 safety_ft=132 service_ft=156 safety_concept=147 service_concept=171 safety_f=227 service_f=256 generation=43
```

证明范围：双 FT、双完整域概念、双实际 F 首次形成；安全值/服务值身份互异；同一 E 的双 IF/R、双当前采用和单值概念上位检查均通过；外设来源缺 provider、先天来源携带 provider、异义 FT/概念固定键、旧 G 和无效初始化交付均被拒绝且探针核对零写。

### 2.2 Debug 第二进程恢复

关闭首次进程后，以同一可执行文件和同一 `run8/db`启动第二进程。

结果：退出码 0。

```text
PASS status=2 safety_ft=132 service_ft=156 safety_concept=147 service_concept=171 safety_f=227 service_f=256 generation=43
```

六个核心身份和事实代次与首次进程完全一致。该结果同时覆盖 U64 组元结构、特征概念出生使用结构、存在概念两组结构及含进程域事实的 L1 快照跨进程恢复；没有恢复进程域幂等账，也没有新增身份或事实代次。

### 2.3 Release 恢复运行

以当前 WIP 的 Release Rebuild 结果读取既有 `run7/db`。

结果：退出码 0。

```text
PASS status=2 safety_ft=132 service_ft=156 safety_concept=147 service_concept=171 safety_f=227 service_f=256 generation=43
```

## 3. 静态检查

- 生产和验证 C++ 源码中下列旧符号均为零引用：
  - `创建先天I64特征类型`
  - `读取先天I64特征类型`
  - `读取先天I64特征类型事实`
- `fprintf(`及仓外探针诊断字符串在生产源码中均为零引用。
- `git diff --check`：通过。
- 变更文件全部位于计划第3节白名单；没有工程清单、需求类、阶段21、规范、详细设计、计划或共享知识索引变更。

## 4. 两个既有验证工程的分账

### 4.1 `绑定存在创建专项验证.vcxproj`

Rebuild 失败，首个错误位于该工具既有代码对 `存在类数据服务`六参数旧构造器的调用，随后还有已退役 `创建绑定存在`、旧世界树请求和旧 DTO 字段等共 48 个错误。

裁决：该工程在本轮 I64 调用迁移之外已经整体落后于当前生产 ABI。本计划只授权迁移该文件中的旧 I64 入口，禁止重建整个旧工具；因此记录为“范围外既有整体漂移”，不把失败归因于本轮生产实现，也不扩大修改工程清单或旧 ABI。

### 4.2 `相关概念窄参与专项验证.vcxproj`

Rebuild 终态：1 个警告、2 个错误。首个 ABI 错误同样是该工具调用 `存在类数据服务`六参数旧构造器；另有工程重复编译 `装配.普通应用.cpp`的 `MSB8027`警告，以及概念树实现单元缺 `/bigobj`导致的 `C1128`。

裁决：三项均为该旧验证工程自身的既有工程/ABI 漂移，不是本轮迁移的新通用 I64 入口造成。按计划禁止范围不修改旧构造器、工程清单或 `/bigobj`配置。

## 5. 未执行与声明边界

- 资源耗尽与发布未知没有合法故障注入器，记录为 `NOT_RUN`。
- 未执行阶段21、根需求、线程、持续运行、后续值换代或完整本能系统集成验收。
- 两个旧专项工程的整体修复与运行不属于本计划；其失败已按既有漂移分账，不能被表述为 PASS。
- 本记录证明当前计划代码、普通应用初始化接线、Debug/Release 构建、全新数据库首次形成和跨进程恢复；不升级为项目整体业务闭环或服务验收。
