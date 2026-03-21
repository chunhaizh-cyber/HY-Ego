# HY-Ego（鱼巢）

`HY-Ego` 是一个以“自我线程 + 任务/需求/方法/学习/因果闭环”为核心的 C++20 实验工程。  
当前仓库按“仅源码”策略维护，便于代码同步与二次开发。

## 仓库内容

- 主要语言：`C++20`（含大量 `.ixx` 模块）
- 解决方案：[`鱼巢.sln`](./鱼巢.sln)
- 工程：
  - [`鱼巢.vcxproj`](./鱼巢.vcxproj)（控制面板主工程）
  - [`鱼巢Headless.vcxproj`](./鱼巢Headless.vcxproj)（Headless 宿主）
  - [`鱼巢_外设_相机.vcxproj`](./鱼巢_外设_相机.vcxproj)（相机外设宿主）

## 快速构建

推荐环境：

- Windows 10/11
- Visual Studio 2022
- MSVC v143
- Windows SDK 10.0
- x64

### 方式一：Visual Studio

1. 打开 [`鱼巢.sln`](./鱼巢.sln)。
2. 选择 `Debug|x64` 或 `Release|x64`。
3. 构建对应工程并运行。

### 方式二：MSBuild

```powershell
msbuild .\鱼巢.sln /p:Configuration=Debug /p:Platform=x64
```

## 核心入口文件

- [`海鱼Dlg.cpp`](./海鱼Dlg.cpp) / [`海鱼Dlg.h`](./海鱼Dlg.h)：控制面板主对话框
- [`自我线程模块.ixx`](./自我线程模块.ixx)：自我线程主循环
- [`任务控制总控模块_v0.ixx`](./任务控制总控模块_v0.ixx)：任务控制总线
- [`任务执行类.ixx`](./任务执行类.ixx)：任务执行与调度骨架
- [`学习写回模块_v0.ixx`](./学习写回模块_v0.ixx)：学习写回与经验更新
- [`因果信息类.ixx`](./因果信息类.ixx)：因果信息组织与输出

## 仓库策略说明

本仓库当前仅推送源码与工程配置文件，不包含以下内容：

- 构建产物（`x64/Debug/Release`、`.pdb/.ilk/.dll/.exe` 等）
- 本地测试产物与临时文件
- 规范文档与运行日志

如果你需要我继续补：

- 英文版 README
- 架构图（模块关系图）
- 开发/提交流程约定（CONTRIBUTING）

可以直接说“下一步”。我会接着补齐。

