# L1-SIMPLIFY-P0-CLOSE v0.1 验证记录

计划 blob：`0f70b5765353edad82625890d969fc86d857dc2b`。

## 已执行验证

- Debug x64 Rebuild：通过。
- Debug `x64/Debug/海中鱼巣.exe --self-test-exit`：退出码 0。
- Release x64 Rebuild：通过。
- Release `x64/Release/海中鱼巣.exe --self-test-exit`：退出码 0；Release 无 Debug 故障入口编译定义。
- `python .\tools\check_specs.py --strict`：110/110 通过。
- `git diff --check`：通过。
- MSVC 模块依赖扫描（Debug x64，`$PSNativeCommandUseErrorActionPreference=$false`）：187/187，失败 0，循环 0，外部需求 0。
- 公共数据头 blob 与 HEAD 相同；服务模块无 diff；状态枚举未改。
- 全仓生产代码 Debug 故障入口调用为 0；目标切片之外没有本轮暂存文件。

## 结果与未覆盖范围

顺序 270 自检覆盖当前/历史/永久占用互证、双属性/双关系、属性与关系替换、两类精确重复/同键异义、旧编码永久占用、恢复材料十类破坏矩阵、候选状态机、发布后权威快照逐项校准和三类 Debug 故障注入。以上结果不证明生产恢复来源、运行期恢复接线、旧基座退出、全部消费者迁移或集成验收。
