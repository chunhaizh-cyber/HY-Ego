# SELFTEST-MIGRATION-B1B 代码实施断点清单

治理编号：JY-270

结论：#243 已将 CONCEPT-S7 8 项与 CONCEPT-S8-S1 10 项迁入两个真模块；S7 导出唯一支持读取辅助，入口 S6D 六处保持原时序改接，最终回归按 300 / 310 / 400 登记。

验证：Debug x64 构建 0 警告 / 0 错误；`--self-test-exit` 退出 0；`check_specs.py --strict` 与 `git diff --check` 通过；连续 20 / 20 轮退出 0。

边界：删除候选仍不可提交；未实现结构事务、物理删除或旧能力迁移。
