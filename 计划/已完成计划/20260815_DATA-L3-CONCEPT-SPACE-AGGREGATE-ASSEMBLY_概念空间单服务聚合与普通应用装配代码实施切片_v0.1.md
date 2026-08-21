# DATA-L3 概念空间单服务聚合与普通应用装配代码实施切片

身份：`DATA-L3-CONCEPT-SPACE-AGGREGATE-ASSEMBLY`

版本：v0.1

日期：2026-08-15

状态：设计就绪；以计划索引正式登记状态为准

## 1. 目标与完成条件

依据[详细设计](../规范/详细设计/20260815_DATA-L3-CONCEPT-SPACE-AGGREGATE-ASSEMBLY_概念空间单服务聚合与普通应用装配详细设计_v0.1.md)，实现唯一 `L2概念结构聚合服务`，并让普通应用从同一 L1 运行包构造、持有唯一 C1 和聚合，只公开聚合 getter。

完成必须同时满足：

1. C1 最终结果与详细设计 v0.2 ABI 机械一致；
2. 普通应用只建立一个概念 owner、一个 C1 和一个概念聚合；
3. 聚合只持一份 non-owning C1 引用，const / non-const getter 返回同一实例；
4. 普通应用没有直接 C1 getter、optional getter或第二写入口；
5. 装配状态 24—27 精确分账，失败不返回半上下文；
6. 根工程 Debug / Release 构建及静态合同通过；
7. 永久记录如实列出实际检查和 `NOT_RUN`，不形成独立验收结论。

## 2. 前置与状态

- 设计基线：`51882fa4a9b64daeef082a01422947aa672eb4b9`。
- C1 v0.2 设计提交：`fd7aa21c5b83a77723412cf6a472d03f483fc4ab`；plan blob `2161afdbd6ff8fddd71cb495ea783c22a37bdaf6`。
- 正式 C1 尚未实现，EVENT 仍占用 `海中鱼巣.vcxproj/.filters`，因此本计划只能登记 `待激活`。
- 激活前必须等 C0 / C1 结果与退出、EVENT 共享文件释放，并从最新 HEAD 重做 S0；若普通应用或 C1 ABI 漂移，退回计划支撑。

## 3. 允许范围

最终生产与永久记录白名单：

```text
海中鱼巣/领域/服务.L2概念结构聚合.ixx
海中鱼巣/装配.普通应用.ixx
海中鱼巣.vcxproj
海中鱼巣.vcxproj.filters
施工记录/20260815_DATA-L3-CONCEPT-SPACE-AGGREGATE-ASSEMBLY_概念空间单服务聚合与普通应用装配施工记录_v0.1.md
验证记录/20260815_DATA-L3-CONCEPT-SPACE-AGGREGATE-ASSEMBLY_概念空间单服务聚合与普通应用装配检查记录_v0.1.md
```

本计划不创建临时验证源码、工程、脚本或输出目录。

## 4. 禁止范围

- 不修改 `L2结构公共.数据.h`、`L2概念结构.数据.h` 或 `服务.L2概念结构.ixx`；
- 不修改 `服务.L2结构聚合.ixx` 或六类世界结构服务；
- 不建立直接 C1 getter、万能聚合、按枚举选服务、第二 owner / 写端口或空指针成功路径；
- 不接线 BIZ、阶段 18、SELF、启动、线程或控制面板；
- 不增加测试 / 自检 / 验收 seam、日志判定、缓存、重试或恢复逻辑；
- 不带入 EVENT、流程图、旧材料删除、路线、SELF 或 `.codex-build` 等异主 WIP。

## 5. 实施顺序

1. 从激活后的最新正式 HEAD 做 S0，固定 C1 ABI、普通应用、工程 / filters、index、所有权和构建资源。
2. 新建唯一 `服务.L2概念结构聚合.ixx`，逐字实现不可复制 / 移动、单引用与两 getter。
3. 在普通应用加入 C1 / 聚合 import、两个唯一成员、两个聚合 getter和私有构造参数。
4. 追加装配状态 24—27；在现有世界聚合成功后依次建立概念 owner、形成交付、构造 C1、构造聚合并完整移动进入上下文。
5. 在 EVENT 最新正式工程内容上唯一登记聚合模块，复核 C0 / C1 前置登记已经存在且不重复。
6. 运行根工程 Debug / Release fresh Rebuild 和详细设计第 8 节静态检查。
7. 更新永久施工 / 检查记录，只提交白名单六文件并普通推送。

## 6. 验证与成功条件

至少运行：

```powershell
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Debug /p:Platform=x64
msbuild .\海中鱼巣.vcxproj /t:Rebuild /p:Configuration=Release /p:Platform=x64
python .\tools\check_specs.py --strict
git diff --check
git diff --cached --check
```

机械检查必须证明：

- 聚合模块工程唯一登记，四配置继续 `/utf-8`；
- 聚合类只有一份 `L2概念结构服务&`，零 owner / 写 / 锁 / 缓存 / DTO 映射；
- 普通应用恰一 C1 和恰一聚合所有权，声明顺序保证聚合先析构；
- 两级 const / non-const getter 均为 `noexcept` 引用，零直接 C1 getter；
- owner 建立、交付、C1 构造、聚合构造分别映射 24—27，0—23 不变；
- 任一失败返回空上下文，成功才移动全部所有权；
- 生产代码零测试 / 自检 / 验收 / 注入实体；最终提交只含允许文件。

真实普通应用运行、BIZ 阶段 18、四根建立、分配耗尽、恢复、跨进程和独立集成验收为 `NOT_RUN`。不得为补动态数量增加临时专项或生产 seam。

## 7. 失败收口

- 计划内编译或代码错误由执行者在白名单内修复并重跑；
- C1 / 普通应用 / 状态槽 / 工程所有权漂移固定 `DRIFT-CONCEPT-SPACE-AGGREGATE-ASSEMBLY-ABI` 并退回计划支撑；
- EVENT 或其它所有者仍占共享文件时保持待激活，不覆盖其 WIP；
- 构建资源冲突只等待资源释放，不改设计。

## 8. 完成声明边界

只允许声明 DATA-EXT-09 的单服务概念空间聚合与普通应用装配进入生产代码并通过必要构建检查。不得声明 C1 根事实、完整 DATA-EXT-08、普通概念、BIZ 阶段 18、启动或全部 DATA 服务已经完成。
