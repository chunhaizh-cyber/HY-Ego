# 全消费者闭包语义清单施工记录

计划身份：`PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A`
计划版本：v0.2 / blob `5e9d38ec71b82ecaccfcefd4334cfb0aba9fccfc`
结果：失败收口；未形成可供实际 `.h/.cpp` 迁移消费的逐实体清单。

## S0

- `analysis_source_commit`：`89697ac7220d17068f70da63cfb4a945824335f1`。
- `plan_release_head`：`a7b0d3448a061623ba5d0c6f6f760fac17dacf8a`，且 `main == origin/main`、index 为空。
- `plan_blob`：`5e9d38ec71b82ecaccfcefd4334cfb0aba9fccfc`，与计划索引登记相同。
- 可比性命令 `git diff --quiet 89697ac7..HEAD -- 海中鱼巣 海中鱼巣.vcxproj 海中鱼巣.vcxproj.filters 验证工具` 退出码为 `0`。
- 三项交接 WIP 仅以 `89697ac7` Git 对象作为未来消费者输入候选；未恢复、修改、删除或暂存工作区文件。

## 已执行分析

隔离根为 `D:/TEMP/海中鱼巣/PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A/20260914T050000Z/`。输入均由 `git show 89697ac7:<路径>` 导出。

先用 `clang++ 22.1.8` 直接运行失败，原因为缺少 MSVC 标准头 `cstddef`。随后按计划调用 `C:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat -arch=x64 -host_arch=x64` 导入 x64 MSVC 环境，并按工程条件加入 `-I D:\vcpkg\installed\x64-windows\include`；D455 外部头缺失已消除。

BMI 构建使用的固定形态为：

```text
clang++ -std=c++20 -fms-extensions -I D:\vcpkg\installed\x64-windows\include --precompile -x c++-module <Git对象导出ixx> -o <隔离pcm> -fmodule-file=<已生成provider>=<pcm>
```

为满足再导出模块，后续模块显式传入所有已经生成的 provider PCM。结果为 `22/57` BMI 成功，随后停止；AST 未启动，逐实体数为 `0`。

首个不可消除失败模块是 `海中鱼巣/核心/服务.L1事实基座.ixx`。固定命令退出 `1`，LLVM 首错为：

```text
line 24: error: imports must immediately follow the module declaration
line 25: error: imports must immediately follow the module declaration
line 26: error: imports must immediately follow the module declaration
line 27: error: imports must immediately follow the module declaration
```

该源模块在 module declaration 与 import 之间存在内容；之后的未解析类型错误是该 module import 未生效的继发错误。本叶合同禁止改源码、文本投影或伪造头文件，因此不能继续 BMI/AST。

## 隔离产物

以下不完整诊断产物保留在隔离根，不纳入仓库，也不得当作迁移冻结输入：

| 产物 | SHA-256 | 事实 |
| --- | --- | --- |
| `模块清单.json` | `27FE191A175255AD3371BD03F163BD27CE60A10ED9C812DD3D316AC50437137B` | 57 个模块的 Git blob / import / `.h` 映射候选 |
| `消费者闭包.json` | `AACFC911EE8BB8FDF35C84C32C2ECEEE6B055DA133D89FBD4A2B65D2C625FE65` | 59 个扫描命中消费者候选 |
| `工程闭包.json` | `14F115CC3F40255229707CC46724257D6263BFB268C525740A67AA09F4A3AF64` | 7 个工程的 XML 项扫描 |
| `语义实体迁移清单.json` | `4F53CDA18C2BAA0C0354BB5F9A3ECBE5ED12AB4D8E11BA873C2F11161202B945` | 空数组；不是有效清单 |
| `执行元数据.json` | `E6B05BFA8DC8F8CC1996C6AE0CB01361772D9AE796C203B3322EFC4ADD3415A9` | 命令、版本、22 次 BMI 与失败 stderr |

## 代码知识变更清单

无。本切片只写施工 / 验证记录，未新增、修改或退出生产函数，未改变调用边、DTO、事实读写、事务、并发或生命周期；因此没有可回写的函数用途卡。

## 边界

本记录不证明模块转换、编译、链接、运行、行为等价或业务闭环。计划 §4.4 规定 BMI 失败必须停止；后续只能以新的可执行计划裁决 LLVM 可接受的语义解析路径，或修订该分析合同，不能由本切片绕过。
