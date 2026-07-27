[CmdletBinding()]
param(
    [string]$项目根目录,
    [ValidateSet('Debug', 'Release')]
    [string]$配置 = 'Debug',
    [ValidateSet('Win32', 'x64')]
    [string]$平台 = 'x64',
    [switch]$启用D455RealSense,
    [string[]]$源文件 = @(),
    [string]$输出路径,
    [switch]$保留中间文件
)

$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot '环境.公共.ps1')

$项目根目录 = 解析-项目根目录 $项目根目录
$文档 = 读取-工程文档 $项目根目录
$编译器 = 导入-VisualStudio开发环境 $平台
$工程文件组 = @(读取-工程源文件 `
    $文档 $项目根目录 @('.ixx') $配置 $平台 ([bool]$启用D455RealSense))

if ($源文件.Count -gt 0) {
    $工程文件集合 = [System.Collections.Generic.HashSet[string]]::new(
        [System.StringComparer]::OrdinalIgnoreCase)
    foreach ($工程文件 in $工程文件组) {
        [void]$工程文件集合.Add($工程文件)
    }
    $筛选文件组 = [System.Collections.Generic.List[string]]::new()
    foreach ($路径 in $源文件) {
        $绝对路径 = if ([System.IO.Path]::IsPathRooted($路径)) {
            [System.IO.Path]::GetFullPath($路径)
        } else {
            [System.IO.Path]::GetFullPath((Join-Path $项目根目录 $路径))
        }
        if (-not $工程文件集合.Contains($绝对路径)) {
            throw "具名 .ixx 未登记到当前工程或本地不存在: $绝对路径"
        }
        if (-not $筛选文件组.Contains($绝对路径)) {
            $筛选文件组.Add($绝对路径)
        }
    }
    $工程文件组 = $筛选文件组.ToArray()
}
if ($工程文件组.Count -eq 0) {
    throw '没有可扫描的 .ixx 文件。'
}

$提交 = (& git -C $项目根目录 rev-parse HEAD).Trim()
$输出路径 = 解析-候选输出路径 `
    $输出路径 `
    $项目根目录 `
    "海中鱼巣-msvc模块依赖-$提交.json"
断言-不是正式图谱输出 $输出路径 $项目根目录
$输出目录 = Split-Path -Parent $输出路径
if ($输出目录) {
    New-Item -ItemType Directory -Force -Path $输出目录 | Out-Null
}

$中间目录 = Join-Path ([System.IO.Path]::GetTempPath()) (
    '海中鱼巣-msvc模块扫描-' + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $中间目录 | Out-Null
$依赖目录 = Join-Path $中间目录 '依赖'
$对象目录 = Join-Path $中间目录 '对象'
$ifc目录 = Join-Path $中间目录 'ifc'
New-Item -ItemType Directory -Path $依赖目录, $对象目录, $ifc目录 | Out-Null

$公共参数 = @(形成-MSVC公共参数 `
    $文档 $项目根目录 $配置 $平台 ([bool]$启用D455RealSense))
$文件结果组 = [System.Collections.Generic.List[object]]::new()

try {
    for ($索引 = 0; $索引 -lt $工程文件组.Count; ++$索引) {
        $当前文件 = $工程文件组[$索引]
        $序号 = '{0:D4}' -f ($索引 + 1)
        $依赖文件 = Join-Path $依赖目录 "$序号.json"
        $对象文件 = Join-Path $对象目录 "$序号.obj"
        $参数组 = [System.Collections.Generic.List[string]]::new()
        foreach ($参数 in $公共参数) {
            $参数组.Add([string]$参数)
        }
        $参数组.Add('/interface')
        $参数组.Add('/scanDependencies')
        $参数组.Add($依赖文件)
        $参数组.Add('/c')
        $参数组.Add($当前文件)
        $参数组.Add("/Fo$对象文件")
        $参数组.Add("/ifcOutput$($ifc目录.TrimEnd('\'))\")

        $诊断 = @(& $编译器 @参数组 2>&1 | ForEach-Object { "$_" })
        $退出码 = $LASTEXITCODE
        $依赖材料 = $null
        if (Test-Path -LiteralPath $依赖文件 -PathType Leaf) {
            $依赖材料 = Get-Content -LiteralPath $依赖文件 -Raw | ConvertFrom-Json
        }
        $规则 = if ($依赖材料) { @($依赖材料.rules) } else { @() }
        $提供组 = @()
        $需求组 = @()
        foreach ($规则项 in $规则) {
            foreach ($提供 in @($规则项.provides)) {
                $提供组 += [ordered]@{
                    logical_name = $提供.'logical-name'
                    source_path = $提供.'source-path'
                    is_interface = [bool]$提供.'is-interface'
                }
            }
            foreach ($需求 in @($规则项.requires)) {
                $需求组 += [ordered]@{
                    logical_name = $需求.'logical-name'
                }
            }
        }
        $相对路径 = [System.IO.Path]::GetRelativePath($项目根目录, $当前文件).
            Replace('\', '/')
        $文件结果组.Add([ordered]@{
            source = $相对路径
            status = if ($退出码 -eq 0 -and $依赖材料) { '成功' } else { '扫描失败' }
            exit_code = $退出码
            diagnostics = $诊断
            provides = $提供组
            requires = $需求组
        })
    }

    $提供者 = @{}
    foreach ($文件 in $文件结果组) {
        foreach ($提供 in @($文件.provides)) {
            $提供者[$提供.logical_name] = $文件.source
        }
    }
    $模块组 = [System.Collections.Generic.List[object]]::new()
    foreach ($文件 in $文件结果组) {
        foreach ($提供 in @($文件.provides)) {
            $模块组.Add([ordered]@{
                name = $提供.logical_name
                source = $文件.source
                requires = @($文件.requires | ForEach-Object { $_.logical_name })
            })
        }
    }

    $入度 = @{}
    $后继 = @{}
    foreach ($模块 in $模块组) {
        $入度[$模块.name] = 0
        $后继[$模块.name] = [System.Collections.Generic.List[string]]::new()
    }
    foreach ($模块 in $模块组) {
        foreach ($需求 in @($模块.requires)) {
            if ($入度.ContainsKey($需求)) {
                $入度[$模块.name] += 1
                $后继[$需求].Add($模块.name)
            }
        }
    }
    $就绪 = [System.Collections.Generic.List[string]]::new()
    foreach ($名称 in ($入度.Keys | Sort-Object)) {
        if ($入度[$名称] -eq 0) {
            $就绪.Add($名称)
        }
    }
    $拓扑顺序 = [System.Collections.Generic.List[string]]::new()
    while ($就绪.Count -gt 0) {
        $名称 = $就绪[0]
        $就绪.RemoveAt(0)
        $拓扑顺序.Add($名称)
        foreach ($后继名称 in ($后继[$名称] | Sort-Object)) {
            $入度[$后继名称] -= 1
            if ($入度[$后继名称] -eq 0) {
                $就绪.Add($后继名称)
                $就绪.Sort()
            }
        }
    }
    $循环模块 = @($入度.Keys | Where-Object { $入度[$_] -gt 0 } | Sort-Object)
    $未找到提供者 = @(
        $模块组 |
            ForEach-Object { $_.requires } |
            Where-Object { -not $提供者.ContainsKey($_) } |
            Sort-Object -Unique
    )
    $扫描失败数 = @($文件结果组 | Where-Object { $_.status -ne '成功' }).Count
    $结果 = [ordered]@{
        schema = 'MSVC-MODULE-DEPENDENCIES/v0.1'
        observed_commit = $提交
        project_root = $项目根目录
        configuration = $配置
        platform = $平台
        compiler = $编译器
        source_count = $工程文件组.Count
        module_count = $模块组.Count
        failed_count = $扫描失败数
        topological_order = $拓扑顺序.ToArray()
        cyclic_modules = $循环模块
        unresolved_requirements = $未找到提供者
        modules = $模块组.ToArray()
        files = $文件结果组.ToArray()
    }
    $结果 | ConvertTo-Json -Depth 12 | Set-Content -LiteralPath $输出路径 -Encoding utf8

    Write-Host "MSVC模块扫描: sources=$($工程文件组.Count), modules=$($模块组.Count), failed=$扫描失败数"
    Write-Host "拓扑顺序: $($拓扑顺序.Count)，循环模块: $($循环模块.Count)，外部需求: $($未找到提供者.Count)"
    Write-Host "候选输出: $输出路径"
    if ($扫描失败数 -gt 0) {
        throw "存在 $扫描失败数 个 .ixx 扫描失败；候选JSON已保留。"
    }
} finally {
    if (-not $保留中间文件) {
        $临时根 = [System.IO.Path]::GetFullPath([System.IO.Path]::GetTempPath()).
            TrimEnd('\') + '\'
        $待删除 = [System.IO.Path]::GetFullPath($中间目录)
        if ($待删除.StartsWith($临时根, [System.StringComparison]::OrdinalIgnoreCase)) {
            Remove-Item -LiteralPath $待删除 -Recurse -Force -ErrorAction SilentlyContinue
        }
    } else {
        Write-Host "中间文件: $中间目录"
    }
}
