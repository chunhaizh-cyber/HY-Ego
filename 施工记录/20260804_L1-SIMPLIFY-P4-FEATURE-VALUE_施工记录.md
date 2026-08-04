# L1-SIMPLIFY-P4-FEATURE-VALUE 施工记录

计划：`L1-SIMPLIFY-P4-FEATURE-VALUE v0.1`
plan blob：`c4e3dd2a30564a009e1374c8d75c9252aeaf027`
施工基线：`a201bcb21108bf99f763f5654fd472bdc7572c37`

## S0

- 正式 `main`、`origin/main` 同为 `a201bcb2`，index 为空。
- P3 结果 `e610246d7380eb45d3f2843e226af0a72f5fb367` 已进入正式 main。
- 仅取得 P4 五文件白名单；其它 dirty/WIP 为不重叠的已知所有者现场。

## 施工内容

- 在 `L1实例特征.数据.h` 增加当前 I64 值换代请求及入口有效性。
- 在现有实例特征服务增加 0x07 幂等域的当前值换代入口。
- 固定一新值、一属性槽变更、一旧值退出，节点/关系为零；服务在发布后独立读取新当前、旧值当前/历史和当前属性槽后返回。
- 扩展 P3 专项自检覆盖成功换代、历史退出、幂等、冲突、旧基线、非法当前/来源/值域及未登记入口。

## 验证

- Debug x64 Rebuild：PASS。
- Release x64 Rebuild：PASS。
- Debug/Release `--self-test-exit`：退出码 0。
- `python .\tools\check_specs.py --strict`：110/110 PASS。
- MSVC 模块依赖扫描（Debug x64）：sources=187、modules=187、failed=0、循环模块=0、外部需求=0。
- 目标 `git diff --check`：PASS；禁止项扫描无 SQL、材料、旧特征体系、iostream/cout/cerr 或 P2-EVIDENCE。

## 完成边界

仅证明一个已建立 I64 实例槽的当前值换代、幂等及独立读回校准源码候选；不证明其它宿主/值类型、批次、比较、状态动态、恢复、生产初始化、旧体系删除或集成验收。
