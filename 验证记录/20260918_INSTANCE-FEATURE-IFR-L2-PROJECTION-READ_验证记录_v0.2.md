# INSTANCE-FEATURE-IFR-L2-PROJECTION-READ 验证记录 v0.2

结果提交：`12e678f744d13d3540f222eb8e6a17e1775dbc95`、`664e1fe7`。

- `git diff --check`、`git diff --cached --check`：通过。
- 隔离 MSVC Debug/Release `/Zs`：`ifr_projection_probe.cpp`（包含存在服务头）均退出 0。
- 隔离 MSVC Debug/Release `/Zs`：实际 `装配.普通应用.cpp` 均退出 0。
- 隔离 Debug/Release 合同驱动：`ifr_projection_contract_driver.cpp` 均输出 `PASS ifr-projection-read-contract`。
- 隔离 Debug/Release 进程内 L1 夹具：建立 v2 布局、一个 E/IF/R集合/版本/R项、正确 role 和 R项自来源 U64 材料；两配置均通过 `ifr-read-success`、形状、角色、材料反序列化和 `ifr-read-zero-write`。
- 静态检查：`读取IFR结构` 范围内没有 `写入端口_`、`提交所有者范围中性写集`、写集建立或退出调用；只有对历史生命周期字段的只读核验。

全工程 Debug `msbuild 海中鱼巣.vcxproj /t:ClCompile /p:Configuration=Debug /p:Platform=x64 /p:MultiProcessorCompilation=false /m:1` 未通过。首个阻断是 `海中鱼巣/领域/数据服务.概念树类.cpp:4489` 的 C2511：定义的 `规范化两组定义内部` 不匹配头文件声明，随后产生同一模块的级联错误；`装配.普通应用.cpp` 已在该轮被编译且本切片未出现诊断。未运行完整应用、持久恢复、跨进程恢复、并发漂移和故障注入。
