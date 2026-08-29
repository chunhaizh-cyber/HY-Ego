# INSTINCT-STAGE3-PRODUCTION-SAFETY-DEFINITION 安全根生产定义启动发布代码实施计划 v0.1

日期：2026-08-29

计划身份：`INSTINCT-STAGE3-PRODUCTION-SAFETY-DEFINITION`

版本：v0.1

状态：待创建侧发布与计划索引登记

## 1. 目标与完成条件

实现 `安全根生产定义初始化提供者`，把 6120 / 6170 的生产定义 v1 接入普通启动，在完整秒和维护游标之前完成首次发布或恢复互证及独立读回。

完成必须同时满足：P00—P07、两配置构建、专项两配置运行、重启恢复、ABI consumer、严格规范检查、精确提交与普通推送。只完成 provider 自测或编译不能退出本计划。

## 2. 正式依据与起点

- 6120 v0.10、6170 v0.6。
- `BIZ-L3-001-03-13 v0.1`。
- `20260829_INSTINCT-STAGE3-PRODUCTION-SAFETY-DEFINITION_安全根生产定义启动发布详细设计_v0.1.md`。
- 直接前置结果提交：`2fdcc4bf598922d273108bd455ef59a26d7c63b1`。
- 创建侧观察基线：`53f8fe40b74ea8d7579142113ae7ed72f82a0506`；执行 S0 必须重读实际 HEAD。

## 3. 允许文件

生产与专项：

```text
海中鱼巣/业务/安全根生产定义初始化.数据.h
海中鱼巣/业务/提供者.安全根生产定义初始化.ixx
海中鱼巣/端到端测试.安全根生产定义初始化.ixx
海中鱼巣/启动.应用程序.ixx
海中鱼巣.vcxproj
海中鱼巣.vcxproj.filters
```

专属记录：

```text
施工记录/20260829_INSTINCT-STAGE3-PRODUCTION-SAFETY-DEFINITION_施工记录_v0.1.md
验证记录/20260829_INSTINCT-STAGE3-PRODUCTION-SAFETY-DEFINITION_验证记录_v0.1.md
```

禁止修改安全根定义 owner/provider、L1、L2特征 / 状态、普通应用装配、启动参数、线程、服务维护、正式规范、流程图、详细设计、计划和计划索引。执行只读消费它们。

## 4. 实施步骤

1. S0 核对计划 blob、HEAD、index、混合文件 WIP 和已有 provider ABI。
2. 新增生产常量、DTO、状态和严格成功谓词。
3. 新增初始化 provider，复用当前代次读取、定义发布与定义读取；实现一次同键收束。
4. 新增 P00—P05 专项；以同一持久根销毁 / 重建普通应用证明 P03。
5. 在本能根锚点成功后、时间和游标之前接入生产 provider；失败映射现有本能根初始化阶段。
6. 登记三个新文件；构建并修复计划范围内纯代码错误。
7. 形成施工 / 验证记录和函数级知识变更清单。
8. 构造精确提交树，运行 P06/P07、两配置与全部门禁。
9. 只暂存本计划增量，提交并普通推送；随后退出计划索引并归档计划。

## 5. 非成功与失败收口

- 已有异义定义：`定义冲突`，零写，启动失败。
- 当前性漂移 / 已可能发布：同一幂等键和同义定义最多重试一次；仍未闭合返回结构化非成功。
- 许可、资源、内部错误：空载荷、截止 0，启动不继续。
- 任何失败不得建立维护游标、创建线程、覆盖已有定义或切换 A 当前值。

## 6. 验证命令与成功条件

执行者按当前 Visual Studio 安装解析 MSBuild；已知有效工具链为 VS 2022 Professional、MSVC 14.44.35207、v143。仓库外验证根固定在 `D:\TEMP\海中鱼巣\INSTINCT-STAGE3-PRODUCTION-SAFETY-DEFINITION\<本轮>`。

至少执行：

```text
普通 Debug / Release Rebuild
专项 Debug / Release Rebuild + 运行
Debug / Release ABI consumer（Release含LTCG）
持久恢复后二次运行
python .\tools\check_specs.py --strict
git diff --check
git diff --cached --check
```

成功条件：全部命令退出 0，专项 P00—P07 全 PASS，提交树只含允许文件和专属记录，不依赖工作树其它 WIP。

## 7. 知识变更与完成边界

施工记录必须列出以下稳定函数身份及直接调用边：

```text
安全根生产定义初始化提供者::初始化并读取安全根生产定义_v1
安全根生产定义初始化结果_v1::成功
运行安全根生产定义初始化端到端测试
运行普通程序 -> 初始化并读取安全根生产定义_v1
```

计划完成只证明生产定义 v1 启动发布 / 恢复闭环；被动维护、主动安全结算、双根自检和整个阶段三仍为后继。
