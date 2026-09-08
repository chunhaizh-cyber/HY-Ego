# FEATURE-CONCEPT-MODEL-CUTOVER 计划支撑退回修订记录 v0.6

日期：2026-09-08
角色：计划支撑智能体 `/root/support_feature_scene_v04`
正式起点：`ea2c3f950d8e349d1738d73dc1a63eed491e6603`，`main == origin/main`，ahead/behind `0/0`，index 空。
Git 发布租约：交互智能体已授予仅限本记录第 3 节四项的 `GIT-LEASE-GRANTED`；发布前 fetch 复核 `HEAD == origin/main == ea2c3f950d8e349d1738d73dc1a63eed491e6603`、ahead/behind `0/0`、index 空。

## 1. 来源 DRIFT

v0.5 代码结果发布后的索引退出复核发现，场景 `存在采用` 和 `最终读回` 阶段不能再次调用可能写入的 `收敛类型观察`。执行者改为直接读取持久观察、名称和 C 后，又暴露出更小的接口缺口：持久观察没有原准确值，场景只持 Fapp，无法调用特征数据服务按解引用内容证明继续材料的准确值仍指向观察 F。

外部值身份与 F 自有复制值事实身份可以不同；raw variant 等式会误拒绝，不比较则允许同键请求改义。这是原计划恢复链的公开只读 ABI 缺口，不是普通代码错误。

## 2. v0.6 裁决

1. Fapp 新增 `读取已发布类型观察(const 特征概念观察请求&, std::uint64_t Gread) const`，复用现有处理结果，不新增 DTO。
2. 入口内部调用 `查询准确特征(FT, 准确值)`完成解引用内容查询，再以持久观察锁定唯一 F，读取名称、C 并做域判定。
3. 入口零写、不生成原请求、不接触 pending/confirmed；同键准确值改义返回幂等冲突。
4. 场景初次/特征应用继续原处理/收敛入口；存在采用和最终读回只调用新读取入口。
5. v0.5 全部其它合同和原 18 项白名单不变；本轮预期只修改两个源码和两份原记录。

## 3. 候选产物与门禁

- `规范/详细设计/20260907_FEATURE-CONCEPT-MODEL-CUTOVER_准确特征与特征概念模型切换详细设计_v0.6.md`
- `计划/20260907_FEATURE-CONCEPT-MODEL-CUTOVER_准确特征与特征概念模型切换代码实施计划_v0.6.md`
- `计划/计划索引.md` 中唯一 `FEATURE-CONCEPT-MODEL-CUTOVER` 项
- 本支撑记录

本轮只运行接口/副作用/调用矩阵静态复核、`git diff --check` 和 `python .\tools\check_specs.py --strict`，不构建、不运行、不改代码、工程、正式规范或执行记录。候选发布后执行者须从正式 v0.6 和新 HEAD 重新 S0。

## 4. 候选检查结果

- `git diff --check -- 计划/计划索引.md`：exit 0；仅提示工作区 LF 将在 Git 后续处理时转换为 CRLF，不是 whitespace error。
- 三个新增文件分别运行 `git diff --no-index --check -- NUL <file>`：均 exit 1，表示存在正常新增差异且没有 whitespace error；同样只有 LF/CRLF 提示。
- `python .\tools\check_specs.py --strict`：exit 0，输出“101 份目录项全部通过”。
- 当前有效计划表中 `FEATURE-CONCEPT-MODEL-CUTOVER` 精确命中一项；索引候选状态为`待激活`，plan blob 为 `060e01341288c033734d0c43c68b01ce8224aa88`，design blob 为 `fa5dd374a3de7ddf8b1110aff25feb2054c7a38e`。
- 设计已把泛称“既有正式域判定”收紧为 `判定应用特征模板` 的精确请求与返回互证；执行者不再需要选择域判定接口。

本检查只证明 v0.6 文档候选的文本、目录和精确合同门禁通过；没有运行构建、程序、数据库、恢复或业务验收。发布时只允许精确暂存本记录第 3 节四项，禁止带入场景源码、施工记录、编译记录或其它 WIP。
