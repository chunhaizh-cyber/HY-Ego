# INSTINCT-STAGE3-ACTIVE-SAFETY-WINDOW-ADJUDICATION 验证记录 v0.1

日期：2026-08-30

## 1. 基线与环境

- 起点：`901fdb68ad91d64a99e08e5d45261ad62792157c`
- 计划 blob：`c5c39d8794dac48c8b94076f29746a3ea9c6c321`
- 隔离根：`D:/TEMP/海中鱼巣/INSTINCT-STAGE3-ACTIVE-SAFETY-WINDOW-ADJUDICATION/20260830-02`
- 构建输入：由 `HEAD` 导出后只覆盖本计划三个生产 / 专项文件；仓库其它未提交 WIP 未进入构建。
- 精确 staged tree 首轮固定为 `3ae8738b1f340a0a13eaf7da37ce9a0c8b2d1b64`；随后只回写本验证记录，三个已验证代码 blob 保持 `d341ac16 / 614f793f / 00b0e8e3` 不变。
- 工具链：VS2022 `v143`，MSVC `14.44.35207`，x64；Release 代码生成启用 LTCG。
- 专项临时启动参数、ABI consumer、IFC 映射、静态库和目录联接全部位于 `D:/TEMP`，不进入仓库候选。

## 2. 构建、专项与 ABI

| 门禁 | Debug | Release |
| --- | --- | --- |
| 根工程精确候选 Rebuild | PASS | PASS，完整代码生成 |
| 扩展专项实际运行 | PASS，进程 exit 0 | PASS，进程 exit 0 |
| 独立 ABI consumer | PASS，`/GL` 编译、`/LTCG` 链接、运行 exit 0 | PASS，`/GL` 编译、`/LTCG /OPT:REF /OPT:ICF` 链接、运行 exit 0 |

ABI consumer 只 import `海中鱼巣.领域.组合.本能被动维护基础快照`，静态核对新公开成员函数的完整 `const noexcept` 签名，消费合同版本、请求、结果和成功谓词。Debug 手工 consumer 有模块环境 C5050 提示以及临时库的 LNK4075 提示，但编译、LTCG 链接和运行均成功；Release 无失败。

## 3. 专项矩阵

| 编号 | 结果 | 证据 |
| --- | --- | --- |
| A00 | PASS | 坏合同与非正半开窗口均入口拒绝、空载荷、截止 0 |
| A01 | PASS | 来源规则未发布与永久账未登记分别返回 `规则未发布` / `账未登记` |
| A02 | PASS | 规则 1 显式空组 + 永久账登记后的合法空组返回 `已读取 / 未改变A`；旧 G 返回当前性漂移 |
| A03 | PASS | 两条永久变化下预算 1 返回数量预算不足；预算 2 完整读回后返回来源未裁定，不降级为空集 |
| A04 | PASS | 下层当前性、集合不闭合、引用、资源和内部状态逐项映射到目标结构化状态 |
| P00—P08、Q00 | PASS | 既有基础快照、阈值版本投影和合法空服务裁决无回归 |
| Q01—Q06 | NOT_RUN | 本轮普通构建未开启服务合同验证宏；这些既有服务合同 fixture 不属于本计划新增矩阵 |

## 4. 静态与文档门禁

| 门禁 | 结果 |
| --- | --- |
| `python .\tools\check_specs.py --strict` | PASS，116 / 116 |
| 三个候选文件 `git diff --check` | PASS；仅有 Git 行尾提示，无空白错误 |
| 值域、L/H、定义版本和生产初始化差异 | PASS，相关生产文件零 diff；当前常量仍为 `INT64_MAX / 30% / 80% / version 1` |
| 工程、普通应用、启动、线程、owner 与 SELF-GOVERNANCE 文件 | 本计划零修改 |

## 5. 诊断过程

1. 首次 MSBuild 调用把 `OutDir/IntDir` 拆成额外参数，MSB1008 在编译前停止；改为单参数属性后通过。
2. 首次 ABI consumer 只给顶层 IFC，因导入链缺少依赖 IFC 以 C1199 停止；按 `ProvidedModule` 建立完整仓库外 IFC 映射后编译通过。
3. 首次临时库响应文件被 `lib.exe` 以旧代码页解释中文路径；改用指向同一隔离根的 ASCII 目录联接及编号 OBJ 后 Debug / Release LTCG 链接和运行通过。

这些均为验证载体调用 / 路径问题，没有修改生产合同或降低门禁。

## 6. 未证明边界

- 没有生产 UTC / 单调完整秒窗口绑定；窗口仍由调用方显式给出。
- 没有证明低位回升允许、主动安全方法存在或被动维护事务闭合。
- 双基础快照的中途并发漂移由正式稳定字段比较和旧 G 专项覆盖；本轮未增加生产故障注入接口。
- 精确 staged tree 双配置、专项与 ABI 已复验；结果提交和推送状态以正式 Git 发布与完成回传为准。
