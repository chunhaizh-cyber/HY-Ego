# DATA-L2-SCENE-FEATURE-INSTANCE-ORGANIZATION 验证记录 v0.1

日期：2026-09-11

验证对象：施工起点 `70e235e41a758e1dc6c1f8e43b3dfa203696847b` 上的本切片最终工作区候选。

## 1. 专项构建与运行

命令：`验证工具/运行场景特征实例组织专项验证.ps1 -Configuration Debug|Release`。每次运行使用独立临时输出/中间目录，并顺序构建运行两个互斥探针。

最终候选结果：

| 配置/轮次 | ExistenceProvider | SceneOrganization | C1001 | 退出码 |
| --- | ---: | ---: | ---: | ---: |
| Debug 1 | 37 PASS | 58 PASS | 0 | 0 |
| Debug 2 | 37 PASS | 58 PASS | 0 | 0 |
| Debug 3 | 37 PASS | 58 PASS | 0 | 0 |
| Release 1 | 37 PASS | 58 PASS | 0 | 0 |

原始日志位于仓库外：`D:/TEMP/海中鱼巣/DATA-L2-SCENE-FEATURE-INSTANCE-ORGANIZATION/final-matrix-v3/`。实际临时候选目录依次为`c080e27839114dd3932e0ee8558df49b`、`e75036de0242413d8669fd01ffe839a4`、`d737f1b4236a473d9a4c169c30511a28`、`a721927a448a4d969444bf547737b853`。

覆盖包括：扩展首次/精确重放、根父/父F组织、路径读回和预算、全局单父、未知F、退出保护、空场景退出、保留键拒绝；真实provider覆盖当前/历史成功、未找到、坏形状、存在退出、当前采用存在但不参与证明和零写。F退出、provider并发漂移与4096固定内部预算耗尽未做确定性故障注入；其状态映射和失败零载荷仅经静态复核，不能把本记录解释为这些故障分支的运行证明。

## 2. 静态与结构复核

- 三个未发布旧候选文件物理不存在，代码及工程引用为零。
- 场景类没有`features_`、`knownFeatures_`或独立已知特征provider，只消费既有`existence_`。
- 生产代码中`存在结构身份只读提供者`唯一派生者为`存在类数据服务`；专项另有允许的严格桩。
- 专项两个import由互斥定义控制，单次实际TU不会同时导入具体存在类和场景类。
- 没有新增生产场景构造调用点。
- 目标路径`git diff --check`通过；只有Git的LF/CRLF提示，无空白错误。

## 3. 根工程分账

最终候选使用独立输出目录对根`海中鱼巣.vcxproj`执行Debug/Release x64 Rebuild，二者均退出1。归一后两配置各113个唯一`文件|行|错误码`，集合完全一致：

- `海中鱼巣/业务/应用服务.世界树类.ixx`：51项；
- `海中鱼巣/领域/数据服务.概念树类.ixx`：62项；
- 本切片合同、存在类、场景类和专项：0项；
- Debug独有：0；Release独有：0；C1001：0。

原始日志和归一集合位于仓库外：`D:/TEMP/海中鱼巣/DATA-L2-SCENE-FEATURE-INSTANCE-ORGANIZATION/root-final-v3/`。这些错误属于范围外异主旧消费者WIP，本切片未修改它们；根工程未通过，不能记为PASS。

## 4. 规范与治理检查

`python .\tools\check_specs.py --strict`：未通过。输出为124份目录项、1个错误：`tools/l1_frozen_surface_manifest.json`的FROZEN项必须登记存在的`acceptance_record`。这是范围外验收清单问题，本切片未修改，不能记为PASS。

## 5. 结论边界

本记录证明最终候选的互斥专项Debug/Release编译、链接和进程内运行矩阵通过，并证明目标模块未出现在本轮根工程诊断中；不证明根工程可编译、普通应用生产装配、数据库跨进程恢复、正式集成验收或业务闭环。
