# DATA-L2-FINAL-CLOSURE-P04.2 世界七族当前确认与历史见证迁移代码实施计划 v0.5

日期：2026-09-03

计划身份：`DATA-L2-FINAL-CLOSURE-P04.2-WORLD-SEVEN-CURRENT-CONFIRMATION-AND-HISTORICAL-WITNESS`

计划基线：`main == origin/main == 78c775a9f6d2dde32e1774915a4ec943aaf13a8e`。

状态：执行侧在 v0.4 S0 中具名退回的最小验证程序修订。经本文件与计划索引同次发布后，v0.5 是本身份唯一当前`可执行`候选；它要求执行智能体从头重新 S0，不证明代码、构建、恢复或集成验收已通过。

## 1. 绑定、范围与不变项

本版绑定已发布的 v0.2（plan blob `560b1031e91ac5602cf01e5df6152b2ca1fddbac`）、v0.3（plan blob `07da8114f7d21cd6c1676bb2640b6a3498517f17`）和 v0.4（plan blob `2383f4b1888385af956739a02e6e6668d5e3331d`）。除本版替代 v0.4 第 2、3、5 节中“将原物理补丁直接应用到 LF archive”的步骤外，v0.2/v0.3/v0.4 的目标、正式依据、七族 ABI、H 回显裁决、生产白名单、禁止范围、物理删除、调用分流、旧 ABI 精确扫描、隔离构建、记录路径和完成边界均原样继承。

本版不改变机器语义、规范、详细设计、公开 ABI、DTO、代码、所有权、WIP 白名单或验证资源。它只修复下列已复现的程序矛盾：S0 保存补丁的物理字节全为 CRLF，而 v0.4 要求在 LF archive 对该物理字节直接作无忽略应用；old-side raw blob 已正确但两份直接应用均失败。不得以 `--ignore-space-change`、`--ignore-whitespace`、文本重定向、3-way 合并或重记 WIP 指纹掩盖该矛盾。

## 2. 已冻结交接 WIP 与补丁规范化

v0.2 的两项 SHA-256 仍是 S0 二进制补丁 new-side Git canonical UTF-8/LF blob 字节的 SHA-256；不是 Windows 工作树 CRLF 文件的 `Get-FileHash`。原补丁的物理 SHA-256是交接证据，规范化副本只是一项经验证的应用材料，二者不得混同。

| 交接文件 | old-side blob | 原补丁物理 SHA-256 | 原补丁行尾形状 | 规范化 LF patch SHA-256 | new-side blob | canonical LF SHA-256 |
| --- | --- | --- | --- | --- | --- | --- |
| `海中鱼巣/领域/服务.L2需求结构.ixx` | `6d9dc9315ba59d45ee015f4451508a05bcbc68cc` | `9F8B65B5615E6ABFD362957BB6B003F515047F2162AA3D27DAE99E07D5E0E644` | 恰 12 个 LF，全部由前导 CR 配对；无孤立 CR/LF | `C08A7097B463BB41AC603324B4EE0C6C01E6E153FC7AE1ED84D00E9C3A89629B` | `487b667924d628b26d34babce1fc59405d7ea125` | `2F968A7ADF791137945879E2F87C517DA5C9A218EC7FB4530F2E648EC659EDC1` |
| `海中鱼巣/端到端测试.自我治理一致事实.ixx` | `b71dda6bb27fcefc165e2f13e5ba8455243ce840` | `FDA18D2AAA70FDF1CD75DEEC2CF75AE63754BF2C21D70068F2E9CA054765BFA3` | 恰 8393 个 LF，全部由前导 CR 配对；无孤立 CR/LF | `B4BB12525C4729BCA010AA7A55136F3AAB286B63ED4CE763DAF05CA08E60695F` | `4c95cf9c17a4496ef8dbd87a2757cd00dbf62d1a` | `D611182CA46D6481E4C650C83448C899BFF6C41B4EF60CD931FEF65062646920` |

补丁根仍固定为 `D:\TEMP\海中鱼巣\DATA-L2-FINAL-CLOSURE-P04.2-WORLD-SEVEN-CURRENT-CONFIRMATION-AND-HISTORICAL-WITNESS\20260903-101500-p042-executor\`。S0 对每份文件按下列不可替代的顺序执行：

1. 验证届时 `HEAD:<path>` 等于表中 old-side blob，原补丁物理 SHA-256 等于表值，且 `git cat-file -e <new-side>^{blob}` 成功。
2. 用二进制读取原补丁，先验证表中的完整 CRLF 形状和精确配对数；任一孤立 CR、孤立 LF、计数不符或物理 SHA 不符都停止，不得规范化或重做补丁。
3. 仅在本任务独占的 `D:\TEMP` 目录，逐字节把每一对 `0D 0A` 替为单个 `0A`，其它每一个字节保持不变；禁止字符解码、`Get-Content`、文本重定向或写回原补丁。生成副本的 SHA-256 必须等于表中的规范化 LF patch SHA-256。
4. 从届时 `HEAD` 建立 fresh LF archive；切换到本任务 `D:\TEMP` 根后，只以该根相对的 `--directory=archive` 对规范化副本运行 `git -c core.autocrlf=false apply --check --binary --recount --whitespace=nowarn`，通过后以同一选项应用。不得向 Windows Git 传入绝对 `--directory` 路径。禁止 `--ignore-space-change`、`--ignore-whitespace`、`--3way` 或其它放宽匹配的选项。
5. 应用后以 `git hash-object --no-filters` 验证目标 raw blob 恰等于表中 new-side blob，并以二进制 SHA-256 验证 canonical LF SHA-256。任何一步失败都停止两份文件的施工、暂存和验证，保留现场并退回计划支撑；不得以 CRLF 变体、语义可套用或整文件 dirty 放行。

### 2.1 固定二进制规范化命令

对表中每一行，把该行的原补丁路径、任务独占 `D:\TEMP` 中尚不存在的规范化副本路径、物理 SHA、配对数和规范化 SHA 分别填入下列五个变量；除这五项外不得改写算法或以文本工具替代。该命令只写 `$NormalizedPatch`，绝不写原补丁或仓库文件：

```powershell
$PhysicalPatch = '<表中原补丁绝对路径>'
$NormalizedPatch = '<任务独占 D:\TEMP 下的新 .lf.patch 路径>'
$ExpectedPhysicalSha = '<表中原补丁物理 SHA-256>'
$ExpectedPairs = <表中 CRLF 配对数>
$ExpectedNormalizedSha = '<表中规范化 LF patch SHA-256>'

function Get-Sha256([byte[]] $Bytes) {
    ([System.BitConverter]::ToString(
        [System.Security.Cryptography.SHA256]::Create().ComputeHash($Bytes)
    )).Replace('-', '')
}

if (Test-Path -LiteralPath $NormalizedPatch) {
    throw '规范化副本已存在，停止，禁止覆盖。'
}
$Bytes = [System.IO.File]::ReadAllBytes($PhysicalPatch)
if ((Get-Sha256 $Bytes) -cne $ExpectedPhysicalSha) {
    throw '原补丁物理 SHA 不匹配。'
}
$Normalized = [System.Collections.Generic.List[byte]]::new()
$CrCount = 0
$LfCount = 0
for ($i = 0; $i -lt $Bytes.Length; $i++) {
    if ($Bytes[$i] -eq 13) {
        $CrCount++
        if ($i + 1 -ge $Bytes.Length -or $Bytes[$i + 1] -ne 10) {
            throw '发现孤立 CR。'
        }
        continue
    }
    if ($Bytes[$i] -eq 10) {
        $LfCount++
        if ($i -eq 0 -or $Bytes[$i - 1] -ne 13) {
            throw '发现孤立 LF。'
        }
    }
    $Normalized.Add($Bytes[$i])
}
if ($CrCount -ne $ExpectedPairs -or $LfCount -ne $ExpectedPairs) {
    throw 'CRLF 配对数不匹配。'
}
[System.IO.File]::WriteAllBytes($NormalizedPatch, $Normalized.ToArray())
if ((Get-Sha256 $Normalized.ToArray()) -cne $ExpectedNormalizedSha) {
    throw '规范化副本 SHA 不匹配。'
}
```

随后在任务临时根执行，不得改成绝对 `--directory` 或省略任一检查：

```powershell
Set-Location '<任务独占 D:\TEMP 根>'
git -c core.autocrlf=false apply --check --binary --recount --whitespace=nowarn --directory=archive $NormalizedPatch
git -c core.autocrlf=false apply --binary --recount --whitespace=nowarn --directory=archive $NormalizedPatch
```

## 3. 允许叠加的精确 P04.2 增量

第 2 节的 raw new-side 通过后，才在 `D:\TEMP` 中的 LF archive 上叠加下列唯一 P04.2 增量，并逐文件与工作树以 `git diff --no-index --unified=0` 比较：

| 文件 | 相对原交接 WIP 的唯一允许 diff | 附加约束 |
| --- | --- | --- |
| `服务.L2需求结构.ixx` | 恰 `+16/-17`、一个 `需求目标身份验证` hunk：两处 `读取存在/特征定义身份来源(... L2读取类别::当前 ...)` 分别替为 `确认当前存在/特征定义结构身份(头, 身份)`，并仅以合同版本、`已读取`、截止=`期望代次`、空变更结果头判定成功。 | 不得触及原 import WIP、其它函数、生命周期 payload 或 C031 内容。 |
| `端到端测试.自我治理一致事实.ixx` | 恰 `+6/-4`、一个 `建立并验证真实状态迁移测试夹具` 的首态前 hunk：`读取特征实例身份来源(... 当前 ...)` 替为 `确认当前特征实例结构身份({L2结构合同版本, G}, 特征实例)`，并仅增加合同版本、状态、截止=G、空变更断言。 | 不得触及其余 C031 WIP；本文件永不进入 P04.2 index、构建或验收输入。 |

每个 diff 还必须通过 `git diff --check`，且除表中单一 hunk 外无其它文件内容差异。将经审定的 P04 hunk 在 `D:\TEMP` 副本逆转后，副本必须重新得到第 2 节的 new-side raw blob 与 canonical LF SHA-256；反转失败、hunk 数/numstat 不符、上下文漂移或任何额外行均为失败。当前工作树总差异可为需求 `+17/-17`、端到端测试 `+1612/-6659`，但这绝不表示异主 WIP 被本叶取得、可暂存或可提交。

## 4. index-only 与最终验收门禁

需求文件只能由 clean `HEAD` 副本加第 3 节需求 P04 hunk 生成 index-only patch，并以 `git apply --cached` 暂存；禁止 `git add 海中鱼巣/领域/服务.L2需求结构.ixx`。暂存后必须同时成立：

1. `git diff --cached -- 海中鱼巣/领域/服务.L2需求结构.ixx` 只含第 3 节两处当前确认替换，且不含 `任务结果消费身份` import；
2. `git diff -- 海中鱼巣/领域/服务.L2需求结构.ixx` 恰为原 `+1/-0` import WIP，工作树与 index 不再出现额外 P04 行；
3. `git diff --cached --name-only` 不含 `海中鱼巣/端到端测试.自我治理一致事实.ixx`，该文件也不得作为 P04.2 隔离构建、专项或验收输入；
4. v0.2 §7.1 的 staged 与工作树旧函数/旧 DTO 精确扫描仍均为零命中，宽模式仍只作新历史见证说明性核查；14 份生产文件和两份记录之外不得进入 index。

执行记录必须分别保存：clean HEAD archive、两份原物理补丁及其物理 SHA、行尾形状输出、两份规范化副本及其 SHA、exact apply 输出、canonical blob/sha 输出、原 WIP 到工作树的两个精确 diff、需求 index-only patch、staged/worktree diff、旧 ABI 精确扫描和排除 E2E 的证明。原 CRLF 补丁只能作为交接证据，规范化 LF 副本只能作为已认证的应用材料；两者都不能替代最终 canonical blob 判定。

## 5. 重新 S0、完成边界与修订记录

v0.5 发布后，执行智能体必须同时读取 v0.2、v0.3、v0.4 与本文件的精确 blob，以届时 HEAD、index、再用文件登记、交接补丁、原物理与规范化 SHA、两份文件的 canonical 基线、P04 增量、H 回显专项及验证资源重新 S0。任何后续 HEAD 变化都必须先重验第 2 节 old-side blob；本版不授予对异主 WIP 的覆盖、删除、整文件暂存或发布权。

本版只消除“保存补丁的 CRLF 物理指纹”与“LF canonical archive 精确复原”之间的计划内验证矛盾，不改变 DATA-L2 `FROZEN`、P08、4070、DATA-L3、ARCH-L4 或普通应用验收边界。

| 日期 | 版本 | 修订内容 |
| --- | --- | --- |
| 2026-09-03 | v0.5 | 对已认证的全 CRLF S0 保存补丁增加逐字节 CRLF→LF 规范化、副本 SHA 与 no-ignore exact apply 门禁；保留原物理补丁 SHA、new-side canonical blob、P04 薄迁移、需求 index-only 和 E2E 排除。 |
