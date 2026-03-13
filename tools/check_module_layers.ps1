[CmdletBinding()]
param(
    [string]$RepoRoot = (Split-Path -Parent $PSScriptRoot),
    [string]$BaselinePath = '',
    [string]$OutputDir = '',
    [switch]$UpdateBaseline
)

$ErrorActionPreference = 'Stop'

if (-not $BaselinePath) {
    $BaselinePath = Join-Path $RepoRoot '规范\模块层级基线.json'
}
if (-not $OutputDir) {
    $OutputDir = Join-Path $RepoRoot 'temp'
}

function Remove-CommentsFromLine([string]$line, [ref]$inBlock) {
    $s = $line
    if ($inBlock.Value) {
        if ($s -match '\*/') {
            $s = $s -replace '^.*?\*/', ''
            $inBlock.Value = $false
        }
        else {
            return ''
        }
    }

    while ($true) {
        if ($s -match '/\*') {
            if ($s -match '/\*.*?\*/') {
                $s = $s -replace '/\*.*?\*/', ''
                continue
            }
            else {
                $s = $s -replace '/\*.*$', ''
                $inBlock.Value = $true
                break
            }
        }
        break
    }

    $s = $s -replace '//.*$', ''
    return $s
}

$files = Get-ChildItem -Path $RepoRoot -Filter *.ixx -File -Recurse
$moduleByFile = @{}
$fileByModule = @{}
$rawImports = @{}

foreach ($f in $files) {
    $lines = Get-Content -Path $f.FullName -Encoding UTF8
    $module = $null
    $inBlock = $false

    foreach ($line in $lines) {
        $scan = Remove-CommentsFromLine $line ([ref]$inBlock)
        if (-not $module -and $scan -match '^\s*export\s+module\s+([^;]+);') {
            $module = $matches[1].Trim()
            break
        }
    }

    if (-not $module) { continue }

    if ($fileByModule.ContainsKey($module)) {
        throw "Duplicate module name '$module' in '$($fileByModule[$module])' and '$($f.FullName)'."
    }

    $moduleByFile[$f.FullName] = $module
    $fileByModule[$module] = $f.FullName
    if (-not $rawImports.ContainsKey($module)) {
        $rawImports[$module] = New-Object System.Collections.Generic.List[string]
    }

    $inBlock = $false
    foreach ($line in $lines) {
        $scan = Remove-CommentsFromLine $line ([ref]$inBlock)
        if ($scan -match '^\s*import\s+([^;]+);') {
            $imp = $matches[1].Trim()
            if ($imp.StartsWith('<')) { continue }
            if ($imp.StartsWith('std')) { continue }
            $rawImports[$module].Add($imp)
        }
    }
}

$modules = @($fileByModule.Keys)
$moduleSet = New-Object 'System.Collections.Generic.HashSet[string]'
foreach ($m in $modules) { [void]$moduleSet.Add($m) }

$adj = @{}
foreach ($m in $modules) {
    $list = New-Object System.Collections.Generic.List[string]
    if ($rawImports.ContainsKey($m)) {
        foreach ($imp in $rawImports[$m]) {
            if ($moduleSet.Contains($imp)) {
                $list.Add($imp)
            }
        }
    }
    $adj[$m] = $list
}

# Tarjan SCC
$script:index = 0
$script:idx = @{}
$script:low = @{}
$script:onStack = @{}
$script:stack = New-Object System.Collections.Generic.List[string]
$script:sccs = New-Object System.Collections.Generic.List[object]
$script:adjRef = $adj

function StrongConnect([string]$v) {
    $script:idx[$v] = $script:index
    $script:low[$v] = $script:index
    $script:index = $script:index + 1
    $script:stack.Add($v)
    $script:onStack[$v] = $true

    foreach ($w in $script:adjRef[$v]) {
        if (-not $script:idx.ContainsKey($w)) {
            StrongConnect $w
            $lv = [int]$script:low[$v]
            $lw = [int]$script:low[$w]
            if ($lw -lt $lv) { $script:low[$v] = $lw }
        }
        elseif ($script:onStack.ContainsKey($w) -and $script:onStack[$w]) {
            $lv = [int]$script:low[$v]
            $iw = [int]$script:idx[$w]
            if ($iw -lt $lv) { $script:low[$v] = $iw }
        }
    }

    if ([int]$script:low[$v] -eq [int]$script:idx[$v]) {
        $comp = New-Object System.Collections.Generic.List[string]
        while ($true) {
            $lastIndex = $script:stack.Count - 1
            $w = $script:stack[$lastIndex]
            $script:stack.RemoveAt($lastIndex)
            $script:onStack[$w] = $false
            $comp.Add($w)
            if ($w -eq $v) { break }
        }
        $script:sccs.Add($comp)
    }
}

foreach ($m in $modules) {
    if (-not $script:idx.ContainsKey($m)) {
        StrongConnect $m
    }
}

$compId = @{}
for ($i = 0; $i -lt $script:sccs.Count; $i++) {
    foreach ($m in $script:sccs[$i]) { $compId[$m] = $i }
}

$cAdj = @{}
for ($i = 0; $i -lt $script:sccs.Count; $i++) {
    $cAdj[$i] = New-Object 'System.Collections.Generic.HashSet[int]'
}

foreach ($m in $modules) {
    $a = [int]$compId[$m]
    foreach ($n in $adj[$m]) {
        $b = [int]$compId[$n]
        if ($a -ne $b) { [void]$cAdj[$a].Add($b) }
    }
}

$script:compLevel = @{}
$script:cAdjRef = $cAdj
function GetCompLevel([int]$c) {
    if ($script:compLevel.ContainsKey($c)) { return [int]$script:compLevel[$c] }
    $outs = $script:cAdjRef[$c]
    if ($outs.Count -eq 0) {
        $script:compLevel[$c] = 1
        return 1
    }
    $max = 1
    foreach ($x in $outs) {
        $lv = (GetCompLevel $x) + 1
        if ($lv -gt $max) { $max = $lv }
    }
    $script:compLevel[$c] = $max
    return $max
}

$inferredLevel = @{}
foreach ($m in $modules) {
    $inferredLevel[$m] = GetCompLevel([int]$compId[$m])
}

if (-not (Test-Path $OutputDir)) {
    New-Item -ItemType Directory -Path $OutputDir | Out-Null
}

$cycleComps = @()
foreach ($comp in $script:sccs) {
    if ($comp.Count -gt 1) {
        $cycleComps += ,(@($comp | Sort-Object))
    }
}

if ($UpdateBaseline) {
    $levels = [ordered]@{}
    foreach ($k in ($inferredLevel.Keys | Sort-Object)) {
        $levels[$k] = [int]$inferredLevel[$k]
    }

    $payload = [ordered]@{
        version      = 1
        generated_at = (Get-Date).ToString('yyyy-MM-ddTHH:mm:ssK')
        rules        = [ordered]@{
            forbid_same_or_higher_import = $true
            forbid_custom_import_for_l1  = $true
        }
        levels       = $levels
    }

    $json = $payload | ConvertTo-Json -Depth 8
    Set-Content -Path $BaselinePath -Value $json -Encoding UTF8
    Write-Host "Baseline written: $BaselinePath"
    exit 0
}

if (-not (Test-Path $BaselinePath)) {
    throw "Baseline file not found: $BaselinePath"
}

$baselineRaw = Get-Content -Path $BaselinePath -Raw -Encoding UTF8 | ConvertFrom-Json
$baseline = @{}
foreach ($p in $baselineRaw.levels.PSObject.Properties) {
    $baseline[$p.Name] = [int]$p.Value
}

$violations = New-Object System.Collections.Generic.List[object]
$edges = New-Object System.Collections.Generic.List[object]

foreach ($m in $modules) {
    if (-not $baseline.ContainsKey($m)) {
        $violations.Add([PSCustomObject]@{ Type = 'MissingLevel'; Module = $m; Imports = ''; Detail = 'Module missing from baseline.' }) | Out-Null
    }
}
foreach ($m in $baseline.Keys) {
    if (-not $fileByModule.ContainsKey($m)) {
        $violations.Add([PSCustomObject]@{ Type = 'StaleLevel'; Module = $m; Imports = ''; Detail = 'Baseline module not found in source.' }) | Out-Null
    }
}

$edgeCount = 0
foreach ($src in $modules) {
    foreach ($dst in $adj[$src]) {
        $edgeCount++
        $srcLevel = if ($baseline.ContainsKey($src)) { [int]$baseline[$src] } else { $null }
        $dstLevel = if ($baseline.ContainsKey($dst)) { [int]$baseline[$dst] } else { $null }

        $edges.Add([PSCustomObject]@{ Module = $src; ModuleLevel = $srcLevel; Imports = $dst; ImportLevel = $dstLevel }) | Out-Null

        if ($null -eq $srcLevel -or $null -eq $dstLevel) { continue }
        if ($srcLevel -le $dstLevel) {
            $violations.Add([PSCustomObject]@{
                    Type = 'DirectionViolation'; Module = $src; Imports = $dst;
                    Detail = "Level($src)=$srcLevel must be > Level($dst)=$dstLevel"
                }) | Out-Null
        }
    }
}

foreach ($src in $modules) {
    if (-not $baseline.ContainsKey($src)) { continue }
    if ([int]$baseline[$src] -ne 1) { continue }
    foreach ($dst in $adj[$src]) {
        $violations.Add([PSCustomObject]@{ Type = 'L1ImportViolation'; Module = $src; Imports = $dst; Detail = 'L1 cannot import custom modules.' }) | Out-Null
    }
}

if ($cycleComps.Count -gt 0) {
    foreach ($comp in $cycleComps) {
        $violations.Add([PSCustomObject]@{ Type = 'CycleViolation'; Module = ($comp -join ','); Imports = ''; Detail = 'Strongly connected component detected.' }) | Out-Null
    }
}

$currentLevelLines = foreach ($m in ($modules | Sort-Object -Property @{ Expression = { [int]$inferredLevel[$_] } }, @{ Expression = { $_ } })) {
    "L$($inferredLevel[$m])`t$m`t$($fileByModule[$m])"
}
Set-Content -Path (Join-Path $OutputDir 'module_levels_current.tsv') -Value $currentLevelLines -Encoding UTF8

$edges | Export-Csv -Path (Join-Path $OutputDir 'module_import_edges.csv') -NoTypeInformation -Encoding UTF8

if ($cycleComps.Count -eq 0) {
    Set-Content -Path (Join-Path $OutputDir 'module_cycles.txt') -Value 'CycleComponents=0' -Encoding UTF8
}
else {
    $cycleLines = @("CycleComponents=$($cycleComps.Count)")
    $i = 1
    foreach ($comp in $cycleComps) {
        $cycleLines += "Cycle#$i"
        foreach ($m in $comp) { $cycleLines += "  $m" }
        $i++
    }
    Set-Content -Path (Join-Path $OutputDir 'module_cycles.txt') -Value $cycleLines -Encoding UTF8
}

if ($violations.Count -gt 0) {
    $violations | Export-Csv -Path (Join-Path $OutputDir 'module_layer_violations.csv') -NoTypeInformation -Encoding UTF8
}
else {
    "Type,Module,Imports,Detail" | Set-Content -Path (Join-Path $OutputDir 'module_layer_violations.csv') -Encoding UTF8
}

$maxLevel = 0
if ($inferredLevel.Count -gt 0) {
    $maxLevel = ($inferredLevel.Values | Measure-Object -Maximum).Maximum
}
$dist = $inferredLevel.GetEnumerator() | Group-Object Value | Sort-Object Name | ForEach-Object { "L$($_.Name):$($_.Count)" }

Write-Host "RepoRoot=$RepoRoot"
Write-Host "Modules=$($modules.Count)"
Write-Host "Edges=$edgeCount"
Write-Host "CycleComponents=$($cycleComps.Count)"
Write-Host "InferredMaxLevel=$maxLevel"
Write-Host ("InferredDistribution=" + ($dist -join ', '))
Write-Host "Violations=$($violations.Count)"

if ($violations.Count -gt 0) {
    Write-Host "See: $(Join-Path $OutputDir 'module_layer_violations.csv')"
    exit 1
}

exit 0
