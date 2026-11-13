param(
    [Parameter(Mandatory=$true)][string]$SourceDirectory,
    [Parameter(Mandatory=$true)][string]$DestinationDirectory
)

# 工程产物分发，不是运行期维护入口。只处理固定文件，最后切换 current。
$ErrorActionPreference = 'Stop'
$源 = [IO.Path]::GetFullPath($SourceDirectory).TrimEnd('\', '/')
$目标 = [IO.Path]::GetFullPath($DestinationDirectory).TrimEnd('\', '/')
if ($源 -eq $目标 -or -not $目标.EndsWith('\基础数据\词典', [StringComparison]::OrdinalIgnoreCase)) {
    throw '词典发布目标必须是独立产物目录下的基础数据\词典'
}
$编码 = New-Object Text.UTF8Encoding($false, $true)
$表名 = @('词典.简体.tsv', '繁简表.tsv', '词头表.tsv', '词性表.tsv')
$说明名 = @('来源清单.json', 'LICENSE.md', 'ATTRIBUTION.md', 'OpenCC-LICENSE.txt')
$文件名 = $表名 + $说明名
$暂存 = $null
$临时指向 = $null
$已切换 = $false

function 核对目标目录链([string]$路径) {
    $当前目录 = [IO.Path]::GetFullPath($路径)
    while ($当前目录) {
        if (Test-Path -LiteralPath $当前目录) {
            $项 = Get-Item -LiteralPath $当前目录 -Force
            if (-not $项.PSIsContainer -or ($项.Attributes -band [IO.FileAttributes]::ReparsePoint)) {
                throw "词典发布目录链不能经过文件或目录重解析点：$当前目录"
            }
        }
        $父目录 = [IO.Directory]::GetParent($当前目录)
        $当前目录 = if ($父目录) { $父目录.FullName } else { $null }
    }
}

function 读取普通文件([string]$路径) {
    if (-not (Test-Path -LiteralPath $路径)) { return $false }
    $项 = Get-Item -LiteralPath $路径 -Force
    if ($项.PSIsContainer -or ($项.Attributes -band [IO.FileAttributes]::ReparsePoint)) {
        throw "词典文件不是普通文件：$路径"
    }
    return $true
}

function 原子替换([string]$临时, [string]$正式) {
    if (读取普通文件 $正式) { [IO.File]::Replace($临时, $正式, $null) }
    else { [IO.File]::Move($临时, $正式) }
}

function 读取文件摘要([string]$路径) {
    # MSBuild 可继承 pwsh 的模块路径；直接用 .NET，不依赖 Get-FileHash 模块自动加载。
    $算法 = [Security.Cryptography.SHA256]::Create()
    $流 = $null
    try {
        $流 = [IO.File]::OpenRead($路径)
        return [BitConverter]::ToString($算法.ComputeHash($流)).Replace('-', '').ToLowerInvariant()
    } finally {
        if ($流) { $流.Dispose() }
        $算法.Dispose()
    }
}

function 核对批次([string]$目录, [string]$标识) {
    $清单 = Join-Path $目录 '批次清单.tsv'
    if (-not (读取普通文件 $清单)) { throw '词典批次清单缺失' }
    if ((Get-Item -LiteralPath $清单).Length -gt 16384 -or
        (读取文件摘要 $清单) -cne $标识) {
        throw '词典批次清单摘要不符'
    }
    $文本 = [IO.File]::ReadAllText($清单, $编码)
    $行 = $文本.Split("`n")
    if ($行.Count -ne 11 -or $行[10] -cne '' -or $文本.Contains("`r")) { throw '词典批次清单行格式不符' }
    $头 = $行[0].Split("`t")
    if ($头.Count -ne 4 -or $头[0] -cne 'HY-EGO-DICTIONARY-BATCH' -or $头[1] -cne '1' -or
        [string]::IsNullOrEmpty($头[2]) -or $头[3] -cnotmatch '^[0-9a-f]{64}$' -or
        $行[1] -cne "OpenCC`tver.1.4.2") { throw '词典批次清单包头不符' }
    $已见 = @{}
    foreach ($序 in 2..9) {
        $列 = $行[$序].Split("`t")
        if ($列.Count -ne 4 -or $文件名 -cnotcontains $列[0] -or $已见.ContainsKey($列[0]) -or
            $列[1] -cnotmatch '^[0-9]+$' -or $列[2] -cnotmatch '^[0-9a-f]{64}$' -or
            $列[3] -cnotmatch '^[0-9]+$') { throw '词典批次文件项不符' }
        $已见[$列[0]] = $true
        $路径 = Join-Path $目录 $列[0]
        if (-not (读取普通文件 $路径)) { throw "词典批次缺文件：$($列[0])" }
        if ([UInt64]$列[1] -eq 0 -or (Get-Item -LiteralPath $路径).Length -ne [UInt64]$列[1] -or
            (读取文件摘要 $路径) -cne $列[2]) {
            throw "词典批次长度或摘要不符：$($列[0])"
        }
        if ($表名 -ccontains $列[0]) {
            $读 = New-Object IO.StreamReader($路径, $编码, $false)
            try {
                if ($null -eq $读.ReadLine()) { throw '词典表缺包头' }
                $数 = [UInt64]0
                while ($null -ne $读.ReadLine()) { $数++ }
            } finally { $读.Dispose() }
            if ($数 -eq 0 -or $数 -ne [UInt64]$列[3]) { throw '词典批次数据行数不符' }
        } elseif ([UInt64]$列[3] -ne 0) { throw '词典说明文件行数应为0' }
    }
}

try {
    核对目标目录链 $目标
    [IO.Directory]::CreateDirectory($目标) | Out-Null
    $暂存 = Join-Path $目标 ('.发布-' + [Guid]::NewGuid().ToString('N'))
    [IO.Directory]::CreateDirectory($暂存) | Out-Null
    $当前 = Join-Path $源 '当前批次.txt'
    $目标当前 = Join-Path $目标 '当前批次.txt'
    if (读取普通文件 $当前) {
        if ((Get-Item -LiteralPath $当前).Length -ne 65) { throw '当前词典批次指向长度不符' }
        $文本 = [IO.File]::ReadAllText($当前, $编码)
        if ($文本 -cnotmatch '\A[0-9a-f]{64}\n\z') { throw '当前词典批次指向不合法' }
        $标识 = $文本.Substring(0, 64)
        $源批次 = Join-Path (Join-Path $源 '批次') $标识
        # 固定清单先验证；复制后的文件再次核验，未通过不发布。
        核对批次 $源批次 $标识
        foreach ($名 in ($文件名 + '批次清单.tsv')) {
            [IO.File]::Copy((Join-Path $源批次 $名), (Join-Path $暂存 $名), $false)
        }
        核对批次 $暂存 $标识
        $批次根 = Join-Path $目标 '批次'
        核对目标目录链 $批次根
        [IO.Directory]::CreateDirectory($批次根) | Out-Null
        $正式 = Join-Path $批次根 $标识
        核对目标目录链 $正式
        if (Test-Path -LiteralPath $正式) { 核对批次 $正式 $标识 }
        else { [IO.Directory]::Move($暂存, $正式); $暂存 = $null }
        $临时指向 = Join-Path $目标 ('.当前-' + [Guid]::NewGuid().ToString('N') + '.tmp')
        [IO.File]::WriteAllText($临时指向, $文本, $编码)
        原子替换 $临时指向 $目标当前
        $临时指向 = $null
        $已切换 = $true
    } else {
        # 保留旧原包只读查询；先准备，最后删除 current 才切换出批次模式。
        $原包 = Join-Path $源 '词典.tsv'
        $有原包 = 读取普通文件 $原包
        $旧名 = @('README.md', '来源清单.json', 'LICENSE.md', 'ATTRIBUTION.md')
        if ($有原包) { $旧名 += '词典.tsv' }
        $已复制 = @()
        foreach ($名 in $旧名) {
            $路径 = Join-Path $源 $名
            if (读取普通文件 $路径) {
                $副本 = Join-Path $暂存 $名
                [IO.File]::Copy($路径, $副本, $false)
                if ((读取文件摘要 $路径) -cne (读取文件摘要 $副本)) {
                    throw "旧词典复制不完整：$名"
                }
                $已复制 += $名
            }
        }
        foreach ($名 in $已复制) { 原子替换 (Join-Path $暂存 $名) (Join-Path $目标 $名) }
        $退出名 = $表名 + @('OpenCC-LICENSE.txt')
        if (-not $有原包) { $退出名 += '词典.tsv' }
        foreach ($名 in $退出名) {
            $路径 = Join-Path $目标 $名
            if (读取普通文件 $路径) { Remove-Item -LiteralPath $路径 }
        }
        if (读取普通文件 $目标当前) { Remove-Item -LiteralPath $目标当前 }
        $已切换 = $true
    }
} catch {
    Write-Error "词典分发失败（当前指向已切换=$已切换）：$_"
    exit 1
} finally {
    if ($临时指向 -and (Test-Path -LiteralPath $临时指向)) { Remove-Item -LiteralPath $临时指向 }
    if ($暂存 -and (Test-Path -LiteralPath $暂存)) {
        # 仅本次新建暂存目录内的固定文件；不递归删除、不触碰完整批次。
        foreach ($名 in ($文件名 + @('批次清单.tsv', 'README.md', '词典.tsv'))) {
            $路径 = Join-Path $暂存 $名
            if (Test-Path -LiteralPath $路径 -PathType Leaf) { Remove-Item -LiteralPath $路径 }
        }
        Remove-Item -LiteralPath $暂存
    }
}
