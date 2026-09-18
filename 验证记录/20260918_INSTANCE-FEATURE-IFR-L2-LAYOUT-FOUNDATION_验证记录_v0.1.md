# INSTANCE-FEATURE-IFR-L2-LAYOUT-FOUNDATION 验证记录 v0.1

结果提交：`8d79cf4fb128a3e48b3254c71f3a6f4dea15087d`。

- `git diff --check`：通过。
- `python .\tools\check_specs.py --strict`：126/126 通过。
- 隔离 Debug：MSVC 编译 `layout_probe.cpp`（包含存在服务头）与 `装配.普通应用.cpp`，退出0。
- 隔离 Release：同两翻译单元以 `/O2 /DNDEBUG` 编译，退出0。
- 静态扫描确认新键、v2、R项材料属性类型、属性类型/U64组与七节点写集；`读取IFR结构` 和 `提交IFR目标结构` 仍为零命中，符合本前置计划禁止范围。

全项目 `ClCompile` 未通过：项目自身并行编译共享隔离 PDB，输出为 C1041；该错误未归因于本切片。未运行完整应用、持久恢复或业务驱动。