# INSTINCT-STAGE3-SERVICE-CONTRACT-FACT-OWNER 验证记录 v0.1

## 1. 基线与验证环境

- 计划 blob：`8d92a30ccd5ebdb107bcbbf2671cafe1bd5e5a4b`
- 起点：`6b55a9ef500c9b6b9159185d721412240cd2cff6`
- 工程入口：`海中鱼巣.vcxproj`
- 工具链：Visual Studio 18 Professional，MSVC `14.50.35717`（工程构建）；MSBuild `18`。
- 专项与 ABI runner：`D:/TEMP/海中鱼巣/INSTINCT-STAGE3-SERVICE-CONTRACT-FACT-OWNER/runner-01`。
- 干净索引候选：`D:/TEMP/海中鱼巣/INSTINCT-STAGE3-SERVICE-CONTRACT-FACT-OWNER/candidate-index-01`。

## 2. 门禁结果

| 项目 | 结果 | 证据摘要 |
| --- | --- | --- |
| `git diff --check` | PASS | 退出码 0；只有既有 LF/CRLF 提示 |
| `python .\tools\check_specs.py --strict` | PASS | `116` 份目录项全部通过 |
| cached diff-check | PASS | 精确索引候选六文件无 whitespace error；记录加入后须再次执行 |
| live Debug，验证宏开 | PARTIAL | 本计划服务与专项编译通过；根构建被三个异主 SELF 测试缺失 `等待合同登记` 接口阻断 |
| live Debug，验证宏关 | PARTIAL | 本计划生产模块与专项无宏形状编译通过；同一异主阻断 |
| live Release，验证宏关 | PARTIAL | 本计划生产模块编译通过；同一异主阻断 |
| 精确索引候选 Debug Rebuild | FAIL（基线链接） | 全部源码编译完成；最终链接为 26 个 L2 跨模块 LNK2001/LNK2019，非本计划符号 |
| 精确索引候选 Release Rebuild | FAIL（基线链接） | 与 Debug 相同的 26 个跨模块未解析符号 |
| Debug ABI consumer | PASS | 仓库外编译、链接、运行，退出码 0 |
| Release `/GL + /LTCG` ABI consumer | PASS | 生成代码完成，运行退出码 0 |
| Release fixture 符号扫描 | PASS | 生产 `服务.服务合同事实权威.ixx.obj` 中无三个 `ARCH_` fixture 符号 |
| 运行期只读查询旧接线扫描 | PASS | 两个运行期只读查询文件无新 owner / 两个完整集合入口命中 |

## 3. 专项矩阵

命令形状：工程以 `/p:ServiceContractFactAuthorityValidation=true` 编译服务和专项；仓库外 runner 链接专项、服务、L1 与直接 L2 依赖对象并运行。

| 编号 | 结果 | 证明内容 |
| --- | --- | --- |
| P00 | PASS | 非法请求精确映射为空失败 |
| P01 | PASS | 0 成员合法空合同 / 事件集合有登记 owner 完整见证 |
| P02 | PASS | 单合同、单事件在同 G0 完整读回 |
| P03 | PASS | N 合同、N 事件完整且规范排序 |
| P04 | PASS | 两阶段 fixture 精确重放，零第二次权威副作用 |
| P05 | PASS | 终态合同混入当前索引时 fail-closed 为集合不闭合 |
| P06 | PASS | 损坏载荷 fail-closed 为引用冲突，无部分成功 |
| P07 | PASS | 读中 G0 漂移返回当前性漂移和空载荷 |
| P08 | PASS | 资源失败返回空载荷，不复用旧结果 |
| P09 | PASS | 同进程服务重构恢复登记并保持逐字段读回 |
| P10 | PASS | 持久运行包关闭再恢复后，两类完整集合逐字段一致 |

## 4. 静态边界

- 生产 owner 写端口只用于结构登记；合同、状态和事件写入只存在于验证宏区间。
- 普通应用只建立 owner / 服务，不自动发布空合同、示例合同或到期事件。
- 没有新增公开生产合同写入入口。
- 当前没有生产合同发布者；合法空集合只证明已发布结构 owner 下成员数为 0，不证明 SERVICE-C1 闭环完成。

## 5. 未通过项与结论

根工程 Debug / Release Rebuild 的最终链接门禁未通过。当前证据足以证明本计划 leaf 的 ABI、完整集合、失败矩阵与持久恢复实现可用，但不足以满足计划第 6 节的完整根工程门禁。因此本验证结论为 `PARTIAL / SAFE WIP`，禁止升级为计划完成或上层业务完成。
