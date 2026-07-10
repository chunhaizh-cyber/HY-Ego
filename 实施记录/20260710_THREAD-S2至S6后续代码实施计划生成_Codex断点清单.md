# THREAD-S2 至 THREAD-S6 后续代码实施计划生成 Codex 断点清单

日期：2026-07-10

## 1. 输入入口

用户追问：

```text
那为什么不能继续生成后续计划?
THREAD-S1 正在确认过程中,不用管
```

处理口径：

```text
当前窗口仍按设计确认窗口处理。
THREAD-S1 的确认 / 移动状态作为当前工作区事实读取，不回退、不修正、不消费执行队列。
基于已确认运行宿主与多线程消息队列详细设计，继续生成 THREAD-S2 至 THREAD-S6 后置待确认计划。
```

## 2. 读取依据

已读取：

```text
AGENTS.md
计划/计划索引.md
规范/000_项目规则总纲.md
规范/001_规则迁移清单.md
项目记忆/Codex任务队列.md
项目记忆/待确认问题.md
规范/详细设计/运行宿主与多线程消息队列详细设计.md
计划/20260709_运行宿主与多线程消息队列专项计划_v0.1.md
项目记忆/当前状态.md
项目记忆/决策记录.md
项目记忆/验收记录.md
```

## 3. 生成产物

已生成：

```text
计划/待确认计划/20260710_THREAD-S2_运行宿主线程生命周期壳代码实施切片_v0.1.md
计划/待确认计划/20260710_THREAD-S3_任务管理线程与任务工作线程第一轮代码实施切片_v0.1.md
计划/待确认计划/20260710_THREAD-S4_缓存统计线程与事件日志线程第一轮代码实施切片_v0.1.md
计划/待确认计划/20260710_THREAD-S5_外设采样材料线程壳代码实施切片_v0.1.md
计划/待确认计划/20260710_THREAD-S6_并发验收停止收口与死锁排除验证切片_v0.1.md
```

已同步登记：

```text
计划/计划索引.md
项目记忆/当前状态.md
项目记忆/决策记录.md
项目记忆/待确认问题.md
项目记忆/Codex任务队列.md
```

## 4. 计划边界

本次只生成后置待确认计划：

```text
未确认。
未入队。
未改 C++。
未运行构建。
未迁移旧函数。
```

THREAD-S2 至 THREAD-S6 均不得绕过 THREAD-S1：

```text
THREAD-S2 依赖 THREAD-S1 完成并验证。
THREAD-S3 依赖 THREAD-S1 和 THREAD-S2 完成并验证。
THREAD-S4 依赖 THREAD-S1 和 THREAD-S2 完成并验证。
THREAD-S5 依赖 THREAD-S1 和 THREAD-S2 完成并验证。
THREAD-S6 依赖 THREAD-S1 至 THREAD-S5 对应范围完成并验证。
```

## 5. 禁止声明

不得声明：

```text
THREAD-S2 至 THREAD-S6 已确认。
THREAD-S2 至 THREAD-S6 已入队。
C++ 已修改。
构建已运行。
运行宿主线程已实现。
任务管理线程或任务工作线程已实现。
缓存统计线程或事件日志线程已实现。
外设采样线程已实现或外设已接入。
自我线程、自我循环或自我苏醒完成。
```

## 6. 验证

本次文档治理应运行：

```powershell
git diff --check
python .\tools\check_specs.py
```

本次不运行 C++ 构建。

已运行：

```powershell
git diff --check
python .\tools\check_specs.py
git status --short
```

结果：

```text
git diff --check：通过；仅提示若干工作区文件 LF/CRLF 换行转换 warning，未发现空白错误。
python .\tools\check_specs.py：通过，输出“海中鱼巣规范治理检查: 未发现问题”。
git status --short：显示本轮新增 THREAD-S2 至 THREAD-S6 待确认计划、生成断点清单和项目记忆 / 计划索引登记；同时显示 `海中鱼巣/入口.cpp`、`海中鱼巣/核心/运行消息队列.h` 等 THREAD-S1 相关代码变更仍在工作区，本轮未处理、未验证、未纳入完成声明。
```
