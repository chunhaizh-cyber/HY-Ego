# L1-SIMPLIFY-METHOD-ROOT-R1-CURRENT-READ v0.1 验证记录

日期：2026-08-07

## 验证基线与范围

```text
施工起点：9f01335bac6de210921cd5b5d7f051e006f514ab
计划：L1-SIMPLIFY-METHOD-ROOT-R1-CURRENT-READ v0.1
验证目标：本计划精确八路径
构建输出：系统临时目录 hzys-r1-debug2 / hzys-r1-release
```

未使用或改写仓库内 `.codex-build/`。日志和控制台输出只作人读观察，结构化自检结果与进程退出码承担本次组件验证结论。

## 实际结果

| 验证项 | 结果 |
| --- | --- |
| Debug x64 串行 Rebuild | PASS，退出 0，生成同一 `海中鱼巣.exe` |
| Debug `海中鱼巣.exe --self-test-exit` | PASS，退出 0；R1 专项 14/14 |
| Release x64 串行 Rebuild | PASS，退出 0，生成同一 `海中鱼巣.exe` |
| Release `海中鱼巣.exe --self-test-exit` | PASS，退出 0；R1 专项 14/14 |
| Debug MSVC 模块扫描 | PASS，204/204，失败 0，循环 0，外部需求 0 |
| Release MSVC 模块扫描 | PASS，204/204，失败 0，循环 0，外部需求 0 |
| `python tools/check_specs.py --strict` | PASS，110/110 |
| `git diff --check` | PASS，八路径无空白错误 |

构建使用系统临时输出和中间目录，MSBuild 给出“临时目录不适合作为增量生成目录”的警告；本次执行的是完整 Rebuild，双配置均退出 0，该警告未改变构建结论。

## 覆盖说明

R1 专项真实覆盖：入口版本和定位资格、稳定编码互异、截止漂移、完整快照中的三节点全无、半结构、完整结构、角色槽缺失、角色值 1、旧角色值 6、错来源、错属性表示、创建见证和重复读取确定性。P17 自身既有顺序 270 专项继续覆盖非阻塞读取的许可拒绝、资源失败和内部异常；R1 对这些 P17 状态的映射由同一生产入口中的穷尽分支编译覆盖，未新增测试替身或第二公开入口。

## 未证明边界

本记录不证明方法登记根已经发布、调用方已经获得定位凭证、生产消费者已经接线、最小本能方法已经登记或可执行，也不证明服务集成、跨进程恢复、STEP-5 或治理闭环完成。

## 独立验收失败后的补修重验

重验使用系统临时根：

```text
C:\Users\zhchh\AppData\Local\Temp\hzys-r1-fix-f1cfe9006e4f40dc8fc87c8d75ee7fe3
```

未读取或改写仓库 `.codex-build/`。首次 Debug 重建暴露专项直接使用完整快照默认比较时的编译器实例化错误；改为专项内对值材料和快照逐字段比较后重新完整构建，该问题属于计划范围内自检代码错误，不改变生产合同。

| 验证项 | 补修后结果 |
| --- | --- |
| Debug x64 串行 Rebuild | PASS，退出0，同一生产程序 |
| Debug `海中鱼巣.exe --self-test-exit` | PASS，退出0；R1专项27/27；stderr 0字节 |
| Release x64 串行 Rebuild | PASS，退出0，同一生产程序 |
| Release `海中鱼巣.exe --self-test-exit` | PASS，退出0；R1专项27/27；stderr 0字节 |
| P17状态映射运行 | PASS，未找到/许可拒绝/资源失败/内部不一致/未知状态/坏成功共6类 |
| 异常和调用次数 | PASS，`bad_alloc`与其它异常已运行；合法请求最大调用1，非法请求调用0，无重试 |
| 重复结构与零写 | PASS，重复节点/值/槽均拒绝；真实L1前后快照逐字段相等 |
| R1具名运行日志 | PASS，实际命中；包含27项、失败0、映射6、最大调用1、零写1、截止1、正式接线否 |
| Debug MSVC模块扫描 | PASS，204/204，失败0，循环0，外部需求0 |
| Release MSVC模块扫描 | PASS，204/204，失败0，循环0，外部需求0 |
| `python tools/check_specs.py --strict` | PASS，110/110 |
| `git diff --check` | PASS |

正式运行日志只证明人读观察字段已经形成；结构化自检报告、27项逐项结果和进程退出码承担组件验证结论。补修仍没有生产消费者或生产定位凭证来源，不得据此声明正式接线、方法登记根已发布或治理闭环完成。
