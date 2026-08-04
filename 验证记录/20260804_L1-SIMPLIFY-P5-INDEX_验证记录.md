# L1-SIMPLIFY-P5-INDEX 验证记录

## 1. 验证身份

- 计划：`L1-SIMPLIFY-P5-INDEX v0.1`
- plan blob：`92cb9cb2dd84443eedce7afdf0be11db394b7bf9`
- 执行基线：`997121dee037e35530949dcbbe3bfaf26e989205`

## 2. 动态验证

| 门禁 | 结果 |
| --- | --- |
| Debug x64 Rebuild | PASS，0 warning / 0 error |
| Release x64 Rebuild | PASS，0 warning / 0 error |
| Debug `--self-test-exit` | PASS，exit 0，完整自检 28/28 |
| Release `--self-test-exit` | PASS，exit 0，完整自检 28/28 |
| P5 顺序 330 专项 | PASS，20 组验收，覆盖详细设计 §11 动态矩阵 |
| `python .\tools\check_specs.py --strict` | PASS，110/110 |
| MSVC 模块依赖扫描 Debug x64 | PASS，sources 190 / modules 190 / failed 0 / cycle 0 / external 0 |

模块扫描按 PowerShell 7 宿主要求先设置 `$PSNativeCommandUseErrorActionPreference = $false`。

## 3. 静态门禁

- 新 3 个 `.ixx` 各有唯一 `export module`，只按 `ClCompile` 登记；新头只按 `ClInclude` 登记。
- 新生产模块依赖只到新索引合同和现行 L1 事实基座服务；旧索引、旧句柄、节点直接仓、SQL 和任一 L2 模块依赖命中 0。
- 执行基线与验证时工作区 blob 一致：
  - `L1公共事实.数据.h`：`d8e128e1ebf6a1d46c38051427b7e912a676cef2`
  - `L1事实基座.数据.h`：`cbc7e0097c2fbdd959f926cec42cc7139119b5a3`
  - `服务.L1事实基座.ixx`：`6b2c688af8174ddcf8bc5e5e3add48c8677ada29`
  - `服务.世界登记.ixx`：`1f13dc32c95a24da6245cc082bdffbd53c19871d`
  - `服务.L1场景结构.ixx`：`96f06a2f4345a81f8f6bd6f7ada5d6d61f40b560`
  - `服务.L1特征定义.ixx`：`2fe7d0a5f8cff87886dfefa7c079f29766b90411`
  - `服务.L1实例特征.ixx`：`e728ca08a8734b04470e08b8b206787e3564dc53`
- Release `索引.L1事实基座.ixx.obj` 中 `自检_损坏下一次重建候选` 符号命中 0；生产调用命中 0，唯一调用位于专项自检的 `_DEBUG` 分支。
- 索引操作前后隔离 L1 权威快照一致；并发结果只接受命中、确定未命中、索引不可用及允许的维护状态。
- `git diff --check` PASS；提交前另执行 cached diff check 和精确九文件范围核对。

## 4. 完成声明边界

本结果只证明新 L1 具备业务中性、非权威、可清空、可条件失效、可从指定权威快照确定重建的稳定编码候选索引，且命中需经现行 L1 同代次逐项回读。未证明领域结果索引已接线、类型兼容、生产恢复、旧索引删除、旧消费者迁移、方案 1B、集成验收或整个自我治理闭环完成。
