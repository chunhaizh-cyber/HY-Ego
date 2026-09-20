# ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION 代码实施计划 v0.3

日期：2026-08-31

状态：执行期具名漂移修订候选；发布并由计划索引登记精确 v0.3 blob 后才可重新 S0。

恢复基线：`main == origin/main == c4d8f87645db8c5153ee0e3dbd616c507bed31ba`。v0.2 未暂存安全 WIP保留，不撤销、不覆盖、不夹带。

恢复现场：特征源码 WIP blob `df6c81868fc55dc8cf9f542596386d16bac3af05`、状态源码 WIP blob `9ec9a496d0ec9d036c76f8495c87dd93fb18a6de`、动态发布源码 HEAD blob `96c77acfe3bbf1a550a7e4208abcbac1585535c1`，以及 v0.1 两份专属记录。任一 blob 出现新来源不明重叠变化时只停止本计划。

## 1. 目标与完成条件

闭合 v145 Debug fresh Rebuild 的 `LNK1120: 8`：删除状态模块对动态发布类的残留前置/friend；把动态发布 DTO 改为生产者/消费者共享单一无附着身份；以状态 owner 的最小公开 wrapper 替代动态发布对四个私有成员的直接访问。函数体业务逻辑、请求/结果字段、事务、owner、持久格式、错误与读回语义零变化。

详细设计唯一依据：`规范/详细设计/20260831_ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION_L2特征状态模块实体所有权ABI纠正详细设计_v0.3.md`。

## 2. 写白名单与禁止项

```text
海中鱼巣/领域/服务.L2状态结构.ixx
海中鱼巣/领域/服务.L2状态动态原子发布.ixx
施工记录/20260831_ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION_施工记录_v0.1.md
验证记录/20260831_ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION_验证记录_v0.1.md
```

`海中鱼巣/领域/L2状态动态原子发布.数据.h` 和全部消费者只读，不授权 guard、类型、字段、namespace、默认值或 include 变化。DTO 修复只在动态发布模块把现行 `export { #include ... }` 改成详细设计冻结的 language-linkage export。禁止修改装配、特征、Gseed、工程/filters、索引、规范、旧设计计划、其它 WIP；禁止 stage/commit 他人现场。

## 3. 机械施工

1. 重新 S0：核对 main、HEAD/origin、index、v0.2 plan/design blob、三份生产源码当前 blob及两记录现场。
2. 状态模块删除 `class L2状态动态原子发布服务;` 和同名 friend。
3. 只公开既有 `当前值记录状态参与者结果_v1`；新增详细设计冻结的 `组合发布当前值状态材料就绪_v1()`、`形成组合发布当前值记录状态参与者_v1(...)` 和 const `取得组合发布写入端口()`。动态发布必须先以就绪 helper 把 `false` 映射为 v1 `材料不足`；形成 wrapper 自身也要防御复核，缺定位时返回空载荷 `L2结构状态::未找到`，严禁解引用或返回 `入口拒绝`。现有嵌套 helper 正文原地不动；动态发布持有的仍是 `const L2状态结构服务&`。
4. 动态发布模块保留 global module fragment、`L2_STATE_DYNAMIC_ATOMIC_PUBLISH_NO_INCLUDES`、module declaration 与 imports，只把 `export { #include "L2状态动态原子发布.数据.h" }` 机械替换为 `export extern "C++" { #include "L2状态动态原子发布.数据.h" }`。禁止前移头、`export using`、alias、typedef、第二声明或消费者文本 include；其它结果 helper、类、方法顺序不变。
5. 将动态发布中两个定位检查、参与者形成、owner 身份和参与者首次完整的直接私有访问机械改接 3 个窄入口；不得顺手重构。
6. 追加 v0.3 记录：计划/design blob、S0、精确 diff、4 配置构建、dumpbin 身份、旧 8 UNDEF 归零和第三后继 LNK 分账。

若必须改变 DTO、函数签名、方法正文、字段、事务顺序、许可或失败映射，立即按设计漂移停止。

## 4. 验证与发布门禁

构建根使用：

```text
D:\TEMP\海中鱼巣\ARCH-L2-FEATURE-STATE-MODULE-ENTITY-OWNER-ABI-CORRECTION\<new-run>\<toolset-config>
```

枚举真实工程后，以 `海中鱼巣.vcxproj` 串行执行 v145/v143 × Debug/Release x64 `/t:Rebuild /m:1`，四项都须最终链接 0。另做：

```text
状态文件中动态发布 class/friend 零命中
动态发布对四个状态 private 成员零直接访问
普通应用两个 getter 与六个动态发布方法统一类/DTO身份
dumpbin：动态发布类仅附着动态发布模块，动态 DTO 无模块附着
git diff --check -- <本计划白名单>
git diff --check
git diff --cached --check
python .\tools\check_specs.py --strict
```

8 个旧未解析归零但出现新 LNK 时：若仍是本次类/DTO/窄合同复链，执行者在白名单内修复；否则固定首符号、对象、模块身份与日志，记为第三后继阻断并停止，不扩大范围。

四配置前先以 dumpbin 对 8 个唯一符号逐项验收：mutable/const 两 getter 的类返回身份一致；六方法的类身份、请求 DTO 和结果 DTO 身份逐字一致。链接日志中同一符号的多消费对象报错只按一个唯一符号计数，不能用报错行减少代替闭合。

通过后只精确暂存本计划文件，普通提交并推送，复核 `HEAD == origin/main`、ahead/behind `0/0`、index 空并报告剩余异主 WIP。本计划最多证明模块实体/DTO ABI 链接闭合；不得据此声明 Gseed、跨进程恢复、阶段三或本能路线完成。
