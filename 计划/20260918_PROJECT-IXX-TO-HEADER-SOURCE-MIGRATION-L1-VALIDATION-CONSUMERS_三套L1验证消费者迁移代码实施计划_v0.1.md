# 三套 L1 验证消费者迁移代码实施计划

计划身份：`PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-L1-VALIDATION-CONSUMERS`

日期：2026-09-18
版本：v0.1
状态：可执行

## 1. 目标、依据与完成条件

按同名详细设计 v0.1，将迁移后消费者闭包审计所确认的三套活动 L1 验证消费者，从已退役的 C++ Modules 格式改为现行公开头源入口。直接依据为审计计划、施工/验证记录和同名详细设计；当前生产公开入口是 `海中鱼巣/核心/服务.L1事实基座.h`。

完成条件：三个验证源改用该头，三个验证项目移除各自五项旧 `.ixx` 编译项，六组 Debug/Release x64 构建与原运行检查成功，专项旧格式搜索零命中，并形成施工/验证记录。

## 2. 允许范围与禁止事项

允许：同名详细设计 §2 的六个验证文件，以及：

```text
施工记录/20260918_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-L1-VALIDATION-CONSUMERS_施工记录_v0.1.md
验证记录/20260918_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-L1-VALIDATION-CONSUMERS_验证记录_v0.1.md
```

禁止生产代码、L1 ABI、验证逻辑、PowerShell 运行器、solution/filters、其它验证资产、正式规范及异主 WIP 的修改、删除、暂存或覆盖。

## 3. 实施与异常处理

严格执行详细设计 §3：先 S0 对实际行数和公开头可达性复核；每源把唯一 `import` 换为冻结的相对 `#include`；每项目只删除五项已具名 `.ixx` `ClCompile`；再运行静态检查、隔离构建和原验证程序。

若头入口缺声明、现有项目项已漂移、构建失败不是上述机械变更可安全定位的问题，保留现场和输出，停止该项目并具名退回计划支撑；不得借机修改 L1 或改变验证断言。

## 4. 验证与发布

按详细设计 §4：每个项目 Debug/Release x64 隔离 Rebuild 和运行均退出 0；三源和三项目旧格式活动依赖均为零；运行目标路径 `git diff --check` 与 `python .\tools\check_specs.py --strict`。仅在获取发布租约后精确暂存允许文件，普通提交并推送。

完成不证明其它验证消费者、生产运行、恢复或业务闭环。
