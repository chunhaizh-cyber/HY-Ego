# L1-SIMPLIFY-P2-FEATURE-DEFINITION 抽象特征定义登记代码实施切片 v0.1

计划身份：`L1-SIMPLIFY-P2-FEATURE-DEFINITION`

版本：`v0.1`

## 1. 目标和完成条件

按 [详细设计](../规范/详细设计/20260804_L1-SIMPLIFY-P2-FEATURE-DEFINITION_抽象特征定义登记详细设计_v0.1.md) 建立运行期长期持有的 I64 区间抽象特征定义服务。完成必须同时证明：公开 DTO 不泄漏 L1 写机制；登记和定义均经单一 L1 写集；成功事实由独立同代次读取校准；完整验证通过并精确发布。

直接依赖：L1-SIMPLIFY-P0-FIX、P1A-RUNTIME-HOLDER、CONTRACT-F0 的正式结果均已进入 main。WORLD-STRUCTURE 不是业务依赖，只提供已占用私有幂等域 `0x01/0x02` 的事实。

## 2. 允许文件

```text
海中鱼巣/领域/L1特征定义.数据.h
海中鱼巣/领域/服务.L1特征定义.ixx
海中鱼巣/领域/自检.L1特征定义.ixx
海中鱼巣/启动.运行期上下文.ixx
海中鱼巣/自检.运行期上下文.ixx
海中鱼巣/启动.应用程序.ixx
海中鱼巣.vcxproj
施工记录/20260804_L1-SIMPLIFY-P2-FEATURE-DEFINITION_施工记录.md
验证记录/20260804_L1-SIMPLIFY-P2-FEATURE-DEFINITION_验证记录.md
```

禁止修改：任何规范、详细设计、流程图、计划、索引；旧 `服务.特征`、`数据操作.特征体系`、旧仓/节点直接仓；世界登记/场景结构；SQL、材料、恢复、入口.cpp、vcxproj.filters 及其它自检。

## 3. 实施合同

1. 新数据头唯一声明详细设计第4节全部 DTO/状态和不变式；只包含公开稳定编码，不出现 L1 写请求/键/仓库。
2. 新生产模块唯一拥有 `L1特征定义服务`；`export import 合同.L1公共事实`，普通 `import 服务.L1事实基座`，不得 `export import` 完整 L1 合同或服务。
3. 私有域固定 `0x03/0x04`。登记写集精确四节点；定义写集精确一节点、三值、三槽。不得建立关系或索引。
4. 四个公开函数、构造、辅助函数、状态映射、读回矩阵、锁和诊断责任逐项按详细设计实现；所有成功必须经公开 L1 读取路径重新形成事实。
5. 运行期上下文在 L1 服务之后持有特征定义服务，提供 const/非const 两个访问器；不改变 `完整()` 判定。
6. 新专项自检覆盖详细设计第8节；运行期上下文现有专项只增加同一服务身份/登记连续性验证。
7. 工程登记新头和两个模块；启动模块 import 自检，以顺序310登记唯一专项，总数25→26。

## 4. S0 与失败收口

执行前重算本计划 blob，确认索引版本/状态、HEAD/origin/index、九文件所有权和当前接口。若服务构造、L1 DTO、运行期成员顺序、顺序310或总数25已漂移且无法按详细设计机械形成，具名退回设计漂移；普通编译/自检错误在九文件内修复。

任一需要实例槽、关系22、旧特征服务修改、额外文件、公开 L1 写类型或新机器语义时停止，不扩大范围。

## 5. 验证

```powershell
git diff --check -- <九个允许文件>
msbuild .\海中鱼巣.vcxproj /m:1 /t:Rebuild /p:Configuration=Debug /p:Platform=x64
.\x64\Debug\海中鱼巣.exe --self-test-exit
msbuild .\海中鱼巣.vcxproj /m:1 /t:Rebuild /p:Configuration=Release /p:Platform=x64
.\x64\Release\海中鱼巣.exe --self-test-exit
python .\tools\check_specs.py --strict
powershell -ExecutionPolicy Bypass -File .\tools\clang_ast\扫描.MSVC模块依赖.ps1 -配置 Debug -平台 x64
git diff --cached --check
```

静态门禁：

- L2 数据头对 `写集幂等键|写集本地键|L1写集请求|L1事实基座仓库|候选|事务令牌` 零命中；
- 新服务 `export import` 只有公共事实合同，L1 服务为非导出 import；
- 私有域 `0x03/0x04` 各唯一，世界域文件零修改；
- 定义建立函数内节点1、值3、槽3、关系0、索引0；
- 运行期成员顺序和构造依赖正确；顺序310唯一、登记/执行/摘要均为26；
- 新 `.ixx` 只有唯一匹配 `export module`，工程为 `ClCompile`，没有 `#include` 调用。

## 6. 记录和发布

记录只写实际 S0、变更、验证、提交和边界。全部门禁通过后只暂存九个允许文件，精确提交并普通推送；其它所有者 WIP 原样保留。

## 7. 完成声明边界

只允许声明 I64 区间抽象特征定义的新 L1 领域写读候选和运行期所有权完成。不声明实例槽、当前值、关系22、特征批次、比较、生产初始化、恢复重入、旧特征体系迁移/删除或服务验收完成。
