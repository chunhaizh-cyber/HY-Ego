[CmdletBinding()]
param(
    [Parameter(Mandatory)]
    [string]$模块依赖路径,
    [string]$项目根目录,
    [ValidateSet('Debug', 'Release')]
    [string]$配置 = 'Debug',
    [ValidateSet('Win32', 'x64')]
    [string]$平台 = 'x64',
    [switch]$启用D455RealSense,
    [string[]]$目标模块 = @(),
    [string]$输出路径,
    [string[]]$额外编译参数 = @(),
    [switch]$保留中间文件
)

$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot '环境.公共.ps1')

$项目根目录 = 解析-项目根目录 $项目根目录
$模块依赖路径 = [System.IO.Path]::GetFullPath($模块依赖路径)
if (-not (Test-Path -LiteralPath $模块依赖路径 -PathType Leaf)) {
    throw "找不到模块依赖JSON: $模块依赖路径"
}
$依赖材料 = Get-Content -LiteralPath $模块依赖路径 -Raw | ConvertFrom-Json
if ($依赖材料.schema -ne 'MSVC-MODULE-DEPENDENCIES/v0.1') {
    throw "不支持的模块依赖schema: $($依赖材料.schema)"
}

[void](导入-VisualStudio开发环境 $平台)
$clang = 'C:\Program Files\LLVM\bin\clang.exe'
if (-not (Test-Path -LiteralPath $clang -PathType Leaf)) {
    $clang命令 = Get-Command clang.exe -ErrorAction Stop
    $clang = $clang命令.Source
}
$文档 = 读取-工程文档 $项目根目录
$公共参数 = @(形成-Clang公共参数 `
    $文档 `
    $项目根目录 `
    $配置 `
    $平台 `
    ([bool]$启用D455RealSense) `
    $额外编译参数)

$模块表 = @{}
foreach ($模块 in @($依赖材料.modules)) {
    $模块表[$模块.name] = $模块
}
if ($目标模块.Count -eq 0) {
    $目标模块 = @($依赖材料.topological_order)
}
foreach ($名称 in $目标模块) {
    if (-not $模块表.ContainsKey($名称)) {
        throw "模块依赖JSON中不存在目标模块: $名称"
    }
}

$闭包集合 = [System.Collections.Generic.HashSet[string]]::new(
    [System.StringComparer]::Ordinal)
function 加入-依赖闭包([string]$名称) {
    if (-not $闭包集合.Add($名称)) {
        return
    }
    foreach ($需求 in @($模块表[$名称].requires)) {
        if ($模块表.ContainsKey($需求)) {
            加入-依赖闭包 $需求
        }
    }
}
foreach ($名称 in $目标模块) {
    加入-依赖闭包 $名称
}
$构建顺序 = @($依赖材料.topological_order | Where-Object {
    $闭包集合.Contains($_)
})
if ($构建顺序.Count -ne $闭包集合.Count) {
    throw '模块闭包包含循环或缺少拓扑顺序，不能建立Clang PCM。'
}
$需要PCM集合 = [System.Collections.Generic.HashSet[string]]::new(
    [System.StringComparer]::Ordinal)
foreach ($名称 in $构建顺序) {
    foreach ($需求 in @($模块表[$名称].requires)) {
        if ($闭包集合.Contains($需求)) {
            [void]$需要PCM集合.Add($需求)
        }
    }
}

$提交 = (& git -C $项目根目录 rev-parse HEAD).Trim()
if ($依赖材料.observed_commit -ne $提交) {
    throw "模块依赖基线 $($依赖材料.observed_commit) 与当前HEAD $提交 不一致。"
}
$输出路径 = 解析-候选输出路径 `
    $输出路径 `
    $项目根目录 `
    "海中鱼巣-clang模块候选流程图-$提交.json"
断言-不是正式图谱输出 $输出路径 $项目根目录
$输出目录 = Split-Path -Parent $输出路径
if ($输出目录) {
    New-Item -ItemType Directory -Force -Path $输出目录 | Out-Null
}

$中间目录 = Join-Path ([System.IO.Path]::GetTempPath()) (
    '海中鱼巣-clang模块-' + [guid]::NewGuid().ToString('N'))
$pcm目录 = Join-Path $中间目录 'pcm'
$单文件目录 = Join-Path $中间目录 'json'
$ast目录 = Join-Path $中间目录 'ast'
New-Item -ItemType Directory -Path $pcm目录, $单文件目录, $ast目录 | Out-Null
$ast提取脚本 = Join-Path $PSScriptRoot '提取.ClangAST候选流程图.py'
if (-not (Test-Path -LiteralPath $ast提取脚本 -PathType Leaf)) {
    throw "找不到 Clang AST JSON 提取脚本: $ast提取脚本"
}

$pcm表 = @{}
$不可用PCM原因表 = @{}
$构建记录组 = [System.Collections.Generic.List[object]]::new()
$候选文件组 = [System.Collections.Generic.List[object]]::new()
$候选调用组 = [System.Collections.Generic.List[object]]::new()
$候选未解析组 = [System.Collections.Generic.List[object]]::new()

try {
    foreach ($名称 in $构建顺序) {
        if (-not $需要PCM集合.Contains($名称)) {
            continue
        }
        $模块 = $模块表[$名称]
        $缺失需求组 = @($模块.requires | Where-Object {
            $闭包集合.Contains($_) -and -not $pcm表.ContainsKey($_)
        })
        if ($缺失需求组.Count -gt 0) {
            $原因 = "依赖PCM不可用: $($缺失需求组 -join ', ')"
            $不可用PCM原因表[$名称] = $原因
            $构建记录组.Add([ordered]@{
                module = $名称
                source = $模块.source
                status = '依赖PCM不可用'
                exit_code = $null
                blocked_by = $缺失需求组
                diagnostics = @($原因)
            })
            continue
        }
        $源路径 = [System.IO.Path]::GetFullPath((Join-Path $项目根目录 $模块.source))
        $pcm路径 = Join-Path $pcm目录 "$名称.pcm"
        $参数组 = [System.Collections.Generic.List[string]]::new()
        foreach ($参数 in $公共参数) {
            $参数组.Add([string]$参数)
        }
        $参数组.Add('-x')
        $参数组.Add('c++-module')
        $参数组.Add('--precompile')
        $参数组.Add($源路径)
        $参数组.Add('-o')
        $参数组.Add($pcm路径)
        foreach ($需求 in @($模块.requires)) {
            if ($pcm表.ContainsKey($需求)) {
                $参数组.Add("-fmodule-file=$需求=$($pcm表[$需求])")
            }
        }
        $诊断 = @(& $clang @参数组 2>&1 | ForEach-Object { "$_" })
        $退出码 = $LASTEXITCODE
        $成功 = $退出码 -eq 0 -and (Test-Path -LiteralPath $pcm路径 -PathType Leaf)
        $构建记录组.Add([ordered]@{
            module = $名称
            source = $模块.source
            status = if ($成功) { '成功' } else { 'PCM生成失败' }
            exit_code = $退出码
            blocked_by = @()
            diagnostics = $诊断
        })
        if (-not $成功) {
            $不可用PCM原因表[$名称] = (
                "PCM生成失败，退出码 ${退出码}: " + ($诊断 -join ' | '))
            continue
        }
        $pcm表[$名称] = $pcm路径
    }

    $目标序号 = 0
    foreach ($名称 in $目标模块) {
        ++$目标序号
        $模块 = $模块表[$名称]
        $源路径 = [System.IO.Path]::GetFullPath((Join-Path $项目根目录 $模块.source))
        $缺失需求组 = @($模块.requires | Where-Object {
            $闭包集合.Contains($_) -and -not $pcm表.ContainsKey($_)
        })
        if ($缺失需求组.Count -gt 0) {
            $候选文件组.Add([ordered]@{
                module = $名称
                source = $源路径
                status = '依赖PCM不可用'
                parse_error_code = $null
                diagnostics = @(
                    "依赖PCM不可用: $($缺失需求组 -join ', ')"
                )
                functions = @()
                calls = @()
                unresolved_calls = @()
            })
            continue
        }
        $单文件输出 = Join-Path $单文件目录 ('{0:D4}.json' -f $目标序号)
        $ast输出 = Join-Path $ast目录 ('{0:D4}.json' -f $目标序号)
        $ast诊断 = Join-Path $ast目录 ('{0:D4}.diagnostics.txt' -f $目标序号)
        $命令参数 = [System.Collections.Generic.List[string]]::new()
        foreach ($参数 in $公共参数) {
            $命令参数.Add([string]$参数)
        }
        $命令参数.Add('-x')
        $命令参数.Add('c++-module')
        $命令参数.Add('-fsyntax-only')
        foreach ($需求 in @($模块.requires)) {
            if ($pcm表.ContainsKey($需求)) {
                $命令参数.Add("-fmodule-file=$需求=$($pcm表[$需求])")
            }
        }
        $命令参数.Add('-Xclang')
        $命令参数.Add('-ast-dump=json')
        $命令参数.Add('-Xclang')
        $命令参数.Add('-ast-dump-filter')
        $命令参数.Add('-Xclang')
        $命令参数.Add('海中鱼巣')
        $命令参数.Add($源路径)
        & $clang @命令参数 1> $ast输出 2> $ast诊断
        if ($LASTEXITCODE -ne 0) {
            $ast退出码 = $LASTEXITCODE
            $ast失败诊断组 = @(
                if (Test-Path -LiteralPath $ast诊断 -PathType Leaf) {
                    Get-Content -LiteralPath $ast诊断 | Where-Object { $_.Trim() }
                }
            )
            $候选文件组.Add([ordered]@{
                module = $名称
                source = $源路径
                status = 'AST导出失败'
                parse_error_code = $ast退出码
                diagnostics = $ast失败诊断组
                functions = @()
                calls = @()
                unresolved_calls = @()
            })
            continue
        }
        $ast诊断组 = @(
            if (Test-Path -LiteralPath $ast诊断 -PathType Leaf) {
                Get-Content -LiteralPath $ast诊断 | Where-Object { $_.Trim() }
            }
        )
        & python.exe $ast提取脚本 `
            --ast-json $ast输出 `
            --source $源路径 `
            --project-root $项目根目录 `
            --observed-commit $提交 `
            --output $单文件输出
        if ($LASTEXITCODE -ne 0) {
            $候选文件组.Add([ordered]@{
                module = $名称
                source = $源路径
                status = 'AST候选转换失败'
                parse_error_code = $LASTEXITCODE
                diagnostics = @('Python AST JSON转换器退出非零。')
                functions = @()
                calls = @()
                unresolved_calls = @()
            })
            continue
        }
        $单文件材料 = Get-Content -LiteralPath $单文件输出 -Raw | ConvertFrom-Json
        foreach ($文件 in @($单文件材料.files)) {
            $文件 | Add-Member -NotePropertyName module -NotePropertyValue $名称
            $文件.diagnostics = $ast诊断组
            if ($ast诊断组.Count -gt 0) {
                $文件.status = '存在诊断'
            }
            $候选文件组.Add($文件)
            foreach ($调用 in @($文件.calls)) {
                $候选调用组.Add($调用)
            }
            foreach ($未解析 in @($文件.unresolved_calls)) {
                $候选未解析组.Add($未解析)
            }
        }
    }

    $失败文件组 = @($候选文件组 | Where-Object {
        $_.status -notin @('成功', '存在诊断')
    })
    $失败PCM组 = @($构建记录组 | Where-Object { $_.status -ne '成功' })
    $结果状态 = if ($失败文件组.Count -eq 0) {
        '成功'
    } elseif ($失败文件组.Count -lt $候选文件组.Count) {
        '部分成功'
    } else {
        '失败'
    }
    $结果 = [ordered]@{
        schema = 'CLANG-MODULE-FLOW-CANDIDATE/v0.1'
        status = $结果状态
        observed_commit = $提交
        module_dependency_source = $模块依赖路径
        project_root = $项目根目录
        compiler = $clang
        configuration = $配置
        platform = $平台
        target_modules = $目标模块
        dependency_closure = $构建顺序
        pcm_modules = @($构建顺序 | Where-Object {
            $需要PCM集合.Contains($_)
        })
        pcm_builds = $构建记录组.ToArray()
        files = $候选文件组.ToArray()
    }
    $结果 | ConvertTo-Json -Depth 24 | Set-Content -LiteralPath $输出路径 -Encoding utf8
    $函数数 = @($候选文件组 | ForEach-Object { $_.functions }).Count
    Write-Host "Clang模块候选: targets=$($目标模块.Count), closure=$($构建顺序.Count), functions=$函数数"
    Write-Host "calls=$($候选调用组.Count), unresolved_calls=$($候选未解析组.Count)"
    Write-Host "pcm_unavailable=$($失败PCM组.Count), target_failures=$($失败文件组.Count), status=$结果状态"
    Write-Host "候选输出: $输出路径"
    if ($失败文件组.Count -gt 0) {
        throw "存在 $($失败文件组.Count) 个目标模块未形成AST候选；失败JSON已保留: $输出路径"
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
