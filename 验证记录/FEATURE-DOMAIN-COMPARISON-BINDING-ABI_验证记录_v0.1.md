# FEATURE-DOMAIN-COMPARISON-BINDING-ABI 验证记录

计划：`FEATURE-DOMAIN-COMPARISON-BINDING-ABI` v0.1
验证对象：特征值域比较服务只读 `绑定于(L1)` ABI

## 专项运行验证

隔离夹具分别构造两个实际、独立的 L1 上下文 A/B，并构造四个比较服务组合：

| 组合 | 对 A | 对 B |
| --- | ---: | ---: |
| `{概念A, 特征A, 特征值A}` | true | false |
| `{概念A, 特征B, 特征值A}` | false | false |
| `{概念A, 特征A, 特征值B}` | false | false |
| `{概念A, 特征B, 特征值B}` | false | false |

Debug 匹配编译参数为 `/MDd /D_DEBUG /D_ITERATOR_DEBUG_LEVEL=2`，链接退出 `0`，运行输出：

```text
same(A,B)=1,0 mixed-feature(A,B)=0,0 mixed-value(A,B)=0,0 mixed-two(A,B)=0,0 generations(A,B)=23/23,23/23
debug_run_exit=0
```

Release 匹配编译参数为 `/MD /DNDEBUG /D_ITERATOR_DEBUG_LEVEL=0`，链接退出 `0`，运行输出：

```text
construct-a
construct-b
construct-services
invoke
same(A,B)=1,0 mixed-feature(A,B)=0,0 mixed-value(A,B)=0,0 mixed-two(A,B)=0,0 generations(A,B)=23/23,23/23
release_run_exit=0
```

两套输出中的 `23/23,23/23` 表示调用前后 A、B 当前事实代次未改变；因此覆盖同 L1 成功、单项/双项混用拒绝和调用零事实写入。

## 静态与工程验证

| 命令 / 检查 | 结果 |
| --- | --- |
| `git diff --check -- 海中鱼巣/领域/数据服务.特征值域比较类.h 海中鱼巣/领域/数据服务.特征值域比较类.cpp` | 退出 0 |
| `python .\tools\check_specs.py --strict` | 126/126 通过，退出 0 |
| 声明精确计数 | 1 |
| 定义精确计数 | 1 |
| Debug x64 `msbuild .\海中鱼巣.vcxproj /t:Build /p:Configuration=Debug /p:Platform=x64 /m:1 /v:minimal` | 退出 0 |
| Release x64 `msbuild .\海中鱼巣.vcxproj /t:Build /p:Configuration=Release /p:Platform=x64 /m:1 /v:minimal` | 退出 0 |

函数体的逐行静态巡检仅含三项 `绑定于(l1)` 调用；没有 L1 读取、端口、写集、提交或既有比较入口调用。

## 未覆盖边界

本记录不验证比较规则、非 I64 值域、两组定义提供者、概念创建、持久恢复、普通应用运行或业务验收。全工作区 `git diff --check` 未作为本计划结论：它受异主 `AGENTS.md` 的既有尾随空白影响。
