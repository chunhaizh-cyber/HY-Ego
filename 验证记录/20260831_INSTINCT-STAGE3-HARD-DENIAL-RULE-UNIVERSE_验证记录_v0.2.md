# INSTINCT-STAGE3-HARD-DENIAL-RULE-UNIVERSE 验证记录 v0.2

日期：2026-08-31

## 1. 验证对象

- 计划：`INSTINCT-STAGE3-HARD-DENIAL-RULE-UNIVERSE v0.2`
- S0 起点：`d46c43a14c3a1220f1a847d0f4030d6e2322d072`
- 计划 blob：`1b941be8013a3f36154f150c1ffdbc88aeef258b`
- 隔离树：`D:/TEMP/海中鱼巣/INSTINCT-STAGE3-HARD-DENIAL-RULE-UNIVERSE/20260831-072418`
- 隔离输入：正式 HEAD + 两个共享数据头精确 ABI hunk + 四个安全生产文件 + 专项 + 两个工程文件的精确登记。

共享 dirty 工作区没有作为编译或运行输入。

## 2. 结果矩阵

| 门禁 | 配置 | 结果 | 证据摘要 |
| --- | --- | --- | --- |
| 根工程 `/t:ClCompile` | Debug x64 | PASS | 全部模块完成扫描与 C++ 编译；P08 新增持久恢复代码已编译 |
| 根工程 `/t:ClCompile` | Release x64 | PASS | 全部模块完成扫描与 C++ 编译 |
| 外部 ABI consumer | Debug / Release x64 | PASS | 三类身份互异、任务 / 方法枚举同型、数值 1/2/3、冻结动作逐字段相等，运行 exit 0 |
| P00 | Debug / Release | PASS | 未发布宇宙与显式空宇宙分账，现实改变 fail-closed |
| P01 | Debug / Release | PASS | 首版空宇宙、精确重复、同键异义、跳版及空全集未命中 |
| P02 | Debug / Release | PASS | 同语义多来源同义归一，完整覆盖并形成强类型命中证据 |
| P03 | Debug / Release | PASS | 同语义约束异义返回引用冲突，零发布 |
| P04 | Debug / Release | PASS | 连续后继版本退出旧宇宙 / 旧规则，历史边界与当前闭包一致 |
| P05 | Debug / Release | PASS | 现实改变缺场景、混合场景、作用主体非成员均场景范围不闭合 |
| P06 | Debug / Release | PASS | 零现实改变不适用，不要求或补造规则宇宙 |
| P07 | Debug / Release | PASS | 作用范围不包含被完整证明；损坏见证不能降级为空适用组 |
| P08 | Debug / Release | PASS | 同一持久根第二会话恢复；旧版本历史重放、当前版本 / 全集逐字段一致，hard-veto 仍命中 |
| 安全目录第二枚举 / DTO | 静态 | PASS | `L2方法动作作用范围`和`任务冻结动作项`定义计数均为 0；唯一实体分别位于方法 / 任务 owner |
| 保留键 | 静态 | PASS | `0x0307'200F..201F` 使用计数 0 |
| 工程登记 | 静态 | PASS | 五个安全文件在 `.vcxproj` 和 `.filters` 中各恰好一次 |
| 自动发布空宇宙 | 静态 | PASS | 生产调用方计数 0；只有 owner 入口定义和专项调用 |
| L/H、值域和两个版本 1 | blob 互证 | PASS | 五个生产定义 / 服务 / 算法文件的候选 blob 与正式 HEAD 完全相同 |
| `check_specs.py --strict` | 当前工作区 | PASS | 116/116 |
| `git diff --check` | 当前工作区 | PASS | exit 0；只有异主文件的 CRLF 提示，无 whitespace error |

## 3. 运行命令形状

```text
MSBuild.exe 海中鱼巣.vcxproj /t:ClCompile
  /p:Configuration=<Debug|Release> /p:Platform=x64 /m /v:minimal /nologo

cl.exe /std:c++latest /utf-8 /EHsc /MD[d] ABI消费.cpp
ABI消费.<debug|release>.exe

link.exe /SUBSYSTEM:CONSOLE /FORCE:UNRESOLVED ...
硬否决专项.<debug|release>.exe

python .\tools\check_specs.py --strict
git diff --check
```

专项链接的 `/FORCE:UNRESOLVED` 只绕过正式 HEAD 中与本切片无关的旧装配未解析符号。专项实际调用所需的 L1、存在、场景、规则 owner 和 hard-veto 路径均已链接并在 Debug / Release 运行通过；这不等于全工程链接通过。

## 4. 尚未证明

- 未验证真实生产规则清单或生产宇宙，因为当前没有获授权的真实规则发布者；专项 fixture 不取得生产权威。
- 未验证方法动作 v2、任务冻结 producer、FRESH、最终当前性复核、普通应用和线程消费。
- 未执行全工程普通链接；正式 HEAD 的既有未解析装配符号继续独立分账。
- 未以故障注入制造提交后读回失败；生产代码保留 `已可能发布 + 四字段见证` 分支，但本次运行矩阵未覆盖该故障。
- 未做具名集成验收；本记录是施工期独立模块、ABI、持久恢复和静态门禁证据。

因此本次只能声明：规则 owner 与 hard-veto v2 的独立服务闭环在上述两配置和持久恢复矩阵内通过。
