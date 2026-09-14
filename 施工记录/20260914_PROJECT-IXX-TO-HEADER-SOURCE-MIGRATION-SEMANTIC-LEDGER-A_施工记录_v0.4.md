# 全消费者闭包迁移证据清单施工记录 v0.4

计划：`PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A` v0.4。输入基线：`ff0a535ba9e8c8a7d81c0194beedc6f2f177d80c`。

## S0

- 当前分支为 `main`，开始时 `main == origin/main`，index 为空。
- 本计划只写本记录、验证记录与其专属附件；生产源码、工程 XML、正式规范、计划及三项交接删除 WIP 均未修改。
- Debug|x64 与 Release|x64 的 `-getItem:ClCompile` 结果均为 60 个项目项、59 个未排除 TU；唯一排除项是关闭 D455 后的 `海中鱼巣/适配/采集器.D455相机.ixx`。原始工程求值见附件。

## 实施与结果

MSVC 18.8.2 的隔离输入副本位于 `D:\TEMP\海中鱼巣\PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A\20260914T140927Z`。已建立逐 TU 的 UTF-8/LF、仓库相对路径 SHA-256 映射，并试验临时 `ForceImportBeforeCppTargets` 注入。

该计划要求的成功闭包未形成。原因与证据完整记录在[失败分账](附件/20260914_PROJECT-IXX-TO-HEADER-SOURCE-MIGRATION-SEMANTIC-LEDGER-A/v0.4/失败分账.json)：`/scanDependencies` 使编译器不生成对象文件，`Rebuild` 随后在链接阶段因缺少对象文件失败；而 MSBuild 的 `BeforeTargets=ClCompile` 在 `SelectClCompile` 之前执行，无法保留逐项元数据。将注入后移后，`ClCompile` 仍只产生 1 对 JSON，不能满足 59/59 的独立输出合同。

因此仅发布工程求值、输入范围、执行元数据与失败分账；未生成候选实体清单、人工物理落点复核或不完整的依赖索引，避免将不满足合同的资料冒充完整闭包。

## 代码知识变更清单

无。本切片未修改任何函数、公开合同、生产调用边或工程登记；仅产生迁移前证据记录。

## 完成边界

本记录证明 v0.4 的依赖扫描施工前提在当前 MSVC/MSBuild 调度下未闭合，不证明任何 `.ixx` 已迁移、代码能编译或运行。后续必须先修订计划，分离扫描式依赖采集与产生对象的依赖采集，并冻结逐实际编译调用的唯一输出策略。
