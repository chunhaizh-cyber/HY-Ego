# D455 可选 SDK 条件编译提供者验证记录

计划身份：`D455-CONDITIONAL-COMPILE-PROVIDER`

验证基线：`052a4dafca5dd0b249b2b59b1ae0dbc1a67e89e1`

日期：2026-09-18

## 验证结果

| 检查 | 命令或证据 | 结果 |
| --- | --- | --- |
| 关闭宏翻译单元 | VS 18 x64 开发环境：`cl /nologo /std:c++20 /EHsc /utf-8 /c /FoD:\TEMP\海中鱼巣\D455-CONDITIONAL-COMPILE-PROVIDER\20260918-052a4daf\off\采集器.D455相机.off.obj 海中鱼巣\适配\采集器.D455相机.cpp`，未定义 `HY_EGO_ENABLE_D455_REALSENSE` | 退出码 0；生成 13307 字节对象文件；证明默认关闭翻译单元不需要 RealSense 头 |
| 分支静态检查 | 读取源文本，核对唯一 `#if defined(HY_EGO_ENABLE_D455_REALSENSE)`、`#else`、`#endif`；SDK include 位于前半分支；fallback 分支无 `librealsense2` 或 `rs2_`；公开工厂合计一声明、两个互斥定义 | 通过 |
| 关闭分支结果形状 | 源码复核：先调用既有 `D455采集配置有效`；无效配置赋 `{false,false,配置无效}`，有效配置赋 `{false,false,无设备}`，两者 `return {}` | 通过；这是本计划冻结的零材料非成功合同 |
| 目标 diff 格式 | `git diff --check -- 海中鱼巣/适配/采集器.D455相机.cpp 施工记录/D455-CONDITIONAL-COMPILE-PROVIDER_施工记录_v0.1.md 验证记录/D455-CONDITIONAL-COMPILE-PROVIDER_验证记录_v0.1.md` | 退出码 0 |
| 正式规范目录 | `python .\tools\check_specs.py --strict` | 退出码 0；126/126 |

## 未覆盖

- 未定义宏的对象编译不链接，因本计划只证明翻译单元边界；未验证协议实现的链接路径。
- 未启用 SDK 宏，也未验证 SDK include/lib/DLL、真实硬件打开、采样、断流或 DLL 复制。
- 未执行世界树根专项；其 v4 消费者漂移与 `/bigobj` 属于后继 `WORLD-TREE-ROOT-V4-CONSUMER-MIGRATION`。
