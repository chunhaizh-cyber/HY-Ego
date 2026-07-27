[CmdletBinding()]
param(
    [Parameter(Mandatory)]
    [string]$候选JSON,
    [string]$输出目录,
    [int]$最多函数 = 0
)

$ErrorActionPreference = 'Stop'
$候选JSON = [System.IO.Path]::GetFullPath($候选JSON)
if (-not (Test-Path -LiteralPath $候选JSON -PathType Leaf)) {
    throw "找不到候选JSON: $候选JSON"
}
$材料 = Get-Content -LiteralPath $候选JSON -Raw | ConvertFrom-Json
if ($材料.schema -notin @(
    'CLANG-AST-FLOW-CANDIDATE/v0.3',
    'CLANG-MODULE-FLOW-CANDIDATE/v0.1')) {
    throw "不支持的候选schema: $($材料.schema)"
}
if (-not $输出目录) {
    $输出目录 = Join-Path ([System.IO.Path]::GetTempPath()) (
        '海中鱼巣-候选流程图草稿-' + $材料.observed_commit)
} elseif (-not [System.IO.Path]::IsPathRooted($输出目录)) {
    $输出目录 = Join-Path ([System.IO.Path]::GetTempPath()) $输出目录
}
$输出目录 = [System.IO.Path]::GetFullPath($输出目录)
$仓库根 = [System.IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\..'))
$正式目录 = [System.IO.Path]::GetFullPath(
    (Join-Path $仓库根 '流程图\代码流程图')).TrimEnd('\') + '\'
if ($输出目录.StartsWith(
    $正式目录,
    [System.StringComparison]::OrdinalIgnoreCase)) {
    throw "候选草稿禁止写入正式流程图目录: $输出目录"
}
$临时根 = [System.IO.Path]::GetFullPath([System.IO.Path]::GetTempPath()).
    TrimEnd('\') + '\'
if (-not $输出目录.StartsWith(
    $临时根,
    [System.StringComparison]::OrdinalIgnoreCase)) {
    throw "候选草稿只允许写入系统临时目录: $输出目录"
}
New-Item -ItemType Directory -Force -Path $输出目录 | Out-Null

function 转义-Mermaid([string]$文本) {
    if ($null -eq $文本) {
        return ''
    }
    return $文本.Replace('\', '\\').Replace('"', "'").Replace("`r", ' ').Replace("`n", ' ')
}

function 安全文件名([string]$文本) {
    $结果 = $文本
    foreach ($字符 in [System.IO.Path]::GetInvalidFileNameChars()) {
        $结果 = $结果.Replace([string]$字符, '_')
    }
    if ($结果.Length -gt 80) {
        $结果 = $结果.Substring(0, 80)
    }
    return $结果
}

$函数序号 = 0
$已写数量 = 0
foreach ($文件 in @($材料.files)) {
    foreach ($函数 in @($文件.functions)) {
        ++$函数序号
        if ($最多函数 -gt 0 -and $已写数量 -ge $最多函数) {
            break
        }
        ++$已写数量
        $名称 = if ($函数.name) { $函数.name } else { "匿名函数$函数序号" }
        $文件名 = '{0:D4}_{1}_候选流程图草稿.md' -f `
            $函数序号, (安全文件名 $名称)
        $输出文件 = Join-Path $输出目录 $文件名
        $行组 = [System.Collections.Generic.List[string]]::new()
        $行组.Add("# $名称 候选流程图草稿")
        $行组.Add('')
        $行组.Add('> 本文件由 Clang AST 机械生成，只是候选证据；不得直接复制到正式流程图，不证明节点分类、规范符合或能力完成。')
        $行组.Add('')
        $行组.Add("- 候选基线：$($材料.observed_commit)")
        $行组.Add("- 源文件：$($函数.location.file)")
        $行组.Add("- 完整类型：$($函数.signature)")
        $行组.Add("- 返回类型：$($函数.result_type)")
        $参数文本 = @($函数.parameters | ForEach-Object {
            "$($_.type) $($_.name)".Trim()
        }) -join '；'
        $行组.Add("- 参数候选：$(if ($参数文本) { $参数文本 } else { '无' })")
        $行组.Add('')
        $行组.Add('```mermaid')
        $行组.Add('flowchart TD')
        $行组.Add("    S[`"入口候选：$(转义-Mermaid $名称)`"]")
        $根节点组 = @($函数.flow_nodes | Where-Object { -not $_.parent_id })
        $上一根 = 'S'
        foreach ($节点 in @($函数.flow_nodes)) {
            $标签 = "$($节点.node_type)｜$($节点.cursor_kind)｜L$($节点.extent.start.line)-$($节点.extent.end.line)"
            if ($节点.name) {
                $标签 += "｜$($节点.name)"
            }
            $行组.Add("    $($节点.id)[`"$(转义-Mermaid $标签)`"]")
        }
        foreach ($节点 in @($函数.flow_nodes)) {
            if ($节点.parent_id) {
                $行组.Add("    $($节点.parent_id) --> $($节点.id)")
            } else {
                $行组.Add("    $上一根 --> $($节点.id)")
                $上一根 = $节点.id
            }
        }
        $行组.Add('    E["返回或离开函数候选"]')
        if (@($函数.flow_nodes).Count -gt 0) {
            $行组.Add("    $上一根 --> E")
        } else {
            $行组.Add('    S --> E')
        }
        $行组.Add('```')
        $行组.Add('')
        $行组.Add('## AST节点映射候选')
        $行组.Add('')
        $行组.Add('| 节点 | 父节点 | 类型候选 | AST种类 | 行号 | 名称 |')
        $行组.Add('| --- | --- | --- | --- | --- | --- |')
        foreach ($节点 in @($函数.flow_nodes)) {
            $节点名称 = "$($节点.name)".Replace('|', '\|')
            $行组.Add(
                "| $($节点.id) | $($节点.parent_id) | $($节点.node_type) | " +
                "$($节点.cursor_kind) | $($节点.extent.start.line)-$($节点.extent.end.line) | " +
                "$节点名称 |")
        }
        $行组.Add('')
        $行组.Add('正式化前仍须逐行读取源码，拆分判断分支，确认调用目标，并按0600规范补齐所有权、生命周期、事务、返回和验证合同。')
        [System.IO.File]::WriteAllLines($输出文件, $行组, [System.Text.UTF8Encoding]::new($false))
    }
    if ($最多函数 -gt 0 -and $已写数量 -ge $最多函数) {
        break
    }
}

Write-Host "候选流程图草稿: $已写数量 份"
Write-Host "输出目录: $输出目录"
