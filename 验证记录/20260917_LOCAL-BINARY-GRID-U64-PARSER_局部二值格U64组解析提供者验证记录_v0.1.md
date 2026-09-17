# LOCAL-BINARY-GRID-U64-PARSER 局部二值格 U64 组解析提供者验证记录

日期：2026-09-17

验证输入：`0309cfa3382d3fbcaf4f970086a16b870c8ba3e6` 加本切片四个未提交文件。

## 专项驱动

仓外专项目录为 `D:\TEMP\海中鱼巣\LOCAL-BINARY-GRID-U64-PARSER\20260917-v03`。以 MSVC C++20 直接编译 `parser_probe.cpp` 与本切片 `.cpp`，Debug 和 Release 均编译退出码 0、运行退出码 0。

断言覆盖：

| 类别 | 覆盖结果 |
| --- | --- |
| 轮廓普通完整 | 最低层非零、多个压缩层、正确 OR、错误 OR 均按合同通过或拒绝 |
| 体素普通完整 | 最低层非零、多个压缩层、正确 OR、错误 OR、颜色项数量与颜色高 40 位均按合同通过或拒绝 |
| 全零 | 普通完整全零轮廓和体素均拒绝；`[0,1,N]`、`[0,2,N]` 合法描述均成功；描述类型错误拒绝 |
| 入口与边界 | 空组、版本错误、空指针、错误分辨率和受检数量边界均返回非成功且格式为空 |

## 静态边界核验

对两份生产源的检索确认：没有 L1、写端口、节点、关系、材料、概念、特征值或特征服务依赖；仓内除本模块自身声明与定义外，零模块外生产调用点。`git diff --check --` 两份生产文件通过。

## 主工程构建

在共享主工作区分别执行：

```powershell
msbuild "D:\海中鱼巣\海中鱼巣.vcxproj" /t:Rebuild "/p:Configuration=Debug;Platform=x64" /m:1 /nologo
msbuild "D:\海中鱼巣\海中鱼巣.vcxproj" /t:Rebuild "/p:Configuration=Release;Platform=x64" /m:1 /nologo
```

两配置均正常结束：构建进程已退出，`x64/Debug/海中鱼巣.exe` 与 `x64/Release/海中鱼巣.exe` 已更新，且各自 `tlog/unsuccessfulbuild` 不存在。该结论仅为编译证据，没有启动普通应用。

另执行 `python .\tools\check_specs.py --strict`，结果为 126/126 通过；它验证正式规范结构，不证明本服务业务行为。

## 验证边界

本记录证明 v2 纯解析器的格式合同和主工程编译，不证明全零描述由格式生产方产生、DATA-L2 规范化、U64 持久化、特征值唯一性、特征比较、概念形成或运行期业务闭环。
