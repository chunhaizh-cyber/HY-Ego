# L1-SIMPLIFY-P42 实际存在 I64 已有特征换代写入规格代码实施切片 v0.1

日期：2026-08-05

状态：待激活

计划身份：`L1-SIMPLIFY-P42-EXISTENCE-I64-FEATURE-REPLACEMENT-SPEC`

创建侧基线：`d521e32efedbf6d8e8bd08278ee9d21be8c1bf29`

## 1. 施工目标与完成条件

原位扩展唯一 `L1实例特征服务`，新增 `形成实际存在I64已有特征换代写入规格`。函数只读取并互证一个实际存在 I64 实例槽的写前当前事实、新值、新来源和四登记节点，返回同代次不可变规格，事实写入恒为0。

完成必须同时满足：

- DTO、状态、字段来源、物理位置和调用顺序与详细设计完全一致；
- 成功规格携带宿主、定义、写前读取见证/事实、两关系、写前来源资格、新值、新来源和四登记见证；
- 写前结构在同一非零截止内恰一完整，新值位于定义闭区间，写前来源资格编码匹配旧值，新来源与宿主同世界；
- P10/P15/P38/P39/P40 提交、账、探测和事务调用全部为0；
- 调用前后 L1 完整快照相等，P3/P4/P8/P10 及届时 P36/P40 回归不退化；
- P42 顺序510专项和完整验证通过。

## 2. 正式设计包与代码白名单

设计包：

```text
规范/详细设计/20260805_L1-SIMPLIFY-P42-EXISTENCE-I64-FEATURE-REPLACEMENT-SPEC_实际存在I64已有特征换代写入规格详细设计_v0.1.md
流程图/20260805_L1-SIMPLIFY-P42-EXISTENCE-I64-FEATURE-REPLACEMENT-SPEC_形成实际存在I64已有特征换代写入规格函数流程图_v0.1.md
计划/20260805_L1-SIMPLIFY-P42-EXISTENCE-I64-FEATURE-REPLACEMENT-SPEC_实际存在I64已有特征换代写入规格代码实施切片_v0.1.md
```

未来代码白名单：

```text
海中鱼巣/领域/L1实例特征.数据.h
海中鱼巣/领域/服务.L1实例特征.ixx
海中鱼巣/领域/自检.L1实例特征.ixx
施工记录/20260805_L1-SIMPLIFY-P42-EXISTENCE-I64-FEATURE-REPLACEMENT-SPEC_施工记录.md
验证记录/20260805_L1-SIMPLIFY-P42-EXISTENCE-I64-FEATURE-REPLACEMENT-SPEC_验证记录.md
```

不得修改 P7/P2/P10/P15/P38/P39/P40/P41、工程/filters/启动、旧特征栈、运行期装配或其它所有者 WIP。

## 3. 固定接口与所有权

精确新增详细设计第4节的两个版本常量、请求/规格/结果三个 DTO、相等比较和最小有效性函数，并在既有类追加：

```cpp
实际存在I64已有特征换代写入规格结果
形成实际存在I64已有特征换代写入规格(
    const 实际存在I64已有特征换代写入规格请求& 请求) const;
```

函数使用既有 `recursive_mutex`、登记、P7/P2/L1 引用和 `检查宿主定义唯一性`。不得新建服务、公开 helper、缓存、索引、幂等账、第二快照或写集函数。

## 4. 机械施工顺序

### S0：正式接口和所有权重绑定

1. 核对 `main==origin/main`、0/0、index空、目标 plan blob、索引状态为可执行且唯一活动代码段。
2. 读取 P15 v0.6、P36、P40 真实结果，逐项核对 L1 v2、实例特征 DTO/类、构造和三个共享文件。
3. 读取 P7/P8/P10 实际接口及 `检查宿主定义唯一性`，确认状态可无损映射。
4. 确认无等价换代规格 provider，顺序510未占用，五文件白名单和验证资源已释放。

任一接口、状态、helper、顺序、plan blob、文件所有权或构造漂移即停止并退回计划支撑。

### S1：DTO 与零写规格根

1. 在数据头按详细设计第4节追加版本、DTO、相等比较和有效性；不改既有枚举数值或 DTO。
2. 在类公开区追加唯一 const 根；不新增构造依赖。
3. 按详细设计第5节固定顺序实现：请求/登记门禁 -> P7宿主 -> P8当前 -> P2定义 -> P7写前来源资格 -> P7新来源 -> 四登记节点 -> 一次完整快照 -> 恰一结构 -> 构造规格。
4. 统一保证所有非已读取结果规格为空；许可拒绝立即返回；`bad_alloc` 映射资源失败，其它未知异常映射内部不一致。
5. 不调用任何提交、探测、批次账、参与者、P10、SQL、日志、时钟或旧栈入口。

### S2：专项和记录

1. 在既有实例特征自检原位追加顺序510专项，覆盖详细设计第8节全部矩阵。
2. 以调用前后完整快照相等和提交函数调用0证明零写，不以日志或 bool 冒充。
3. 保持 P3/P4/P8/P10 及届时 P36/P40 全部旧专项不退化。
4. 形成两份专属记录，登记实际接口、调用计数、状态矩阵、并发、静态扫描和完整验证。

## 5. 禁止实现

```text
禁止调用P10逐项换代或P15/P37/P39提交
禁止读取、写入或补造4170批次账
禁止形成批次身份、幂等键、摘要、本地键或稳定编码候选
禁止在单项provider排序、去重、判断连续或跨项聚合
禁止把新值等于旧值改判为无变化、幂等或拒绝
禁止保存规格、provider结果、许可、锁、引用或快照缓存
禁止新增第二实例特征服务、工程项、启动入口、日志或测试seam
禁止顺带形成完整换代批次、世界树第三写、混合批次或STEP-5
```

## 6. 激活门禁

保持 `待激活`，直至：

1. P15 v0.6、P36、P40 真实结果进入正式 main 并经交互接受；
2. P7/P8/P10 与三个实例特征共享文件的实际接口机械匹配；
3. P15/P36/P40 及届时活动代码段释放目标文件、顺序510、构建、自检和模块扫描资源；
4. 新 HEAD 重做 S0，确认无等价 provider、510未占用且本计划为唯一可执行代码段。

冻结合同只支持连续设计，不证明本计划当前可执行。

## 7. 验证命令与成功条件

至少执行：

```powershell
git diff --check -- <本计划白名单>
python .\tools\check_specs.py --strict
cmake --build --preset windows-msvc-debug --target 海中鱼巣 --clean-first
cmake --build --preset windows-msvc-release --target 海中鱼巣 --clean-first
& .\out\build\windows-msvc-debug\Debug\海中鱼巣.exe --self-test-exit
& .\out\build\windows-msvc-release\Release\海中鱼巣.exe --self-test-exit
pwsh -NoProfile -Command '$PSNativeCommandUseErrorActionPreference=$false; & ".\tools\clang_ast\扫描.MSVC模块依赖.ps1" -配置 Debug -平台 x64'
pwsh -NoProfile -Command '$PSNativeCommandUseErrorActionPreference=$false; & ".\tools\clang_ast\扫描.MSVC模块依赖.ps1" -配置 Release -平台 x64'
```

成功条件：P42专项证明完整同代次规格、状态分账、零写、并发和零旁路；既有回归全部通过；Debug/Release完整自检退出0；strict、双配置模块扫描、目标/cached diff check和白名单核对通过。

## 8. 停工与完成声明

以下情况停工：实际接口无法无损映射；P36/P40 已形成等价换代规格；P8 helper 无法证明恰一结构；必须修改白名单外 provider、批次账、工程或装配；510已占用；验证持续失败且无法在本计划内归因；Git或所有权冲突。

完成后只可声明：P42 白名单内源码候选可零写形成一个实际存在 I64 已有特征的完整同截止单项换代规格。

禁止声明：完整换代批次、4170换代发布、世界树第三写、状态/动态、生产装配、运行期接线、旧入口退出、集成验收、STEP-4、STEP-5或治理闭环完成。
