[CmdletBinding()]
param(
    [string]$工具路径,
    [string]$项目根目录,
    [ValidateSet('Debug', 'Release')]
    [string]$配置 = 'Debug',
    [ValidateSet('Win32', 'x64')]
    [string]$平台 = 'x64',
    [switch]$启用D455RealSense,
    [string[]]$源文件 = @(),
    [string]$输出路径,
    [string[]]$额外编译参数 = @()
)

$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot '环境.公共.ps1')

$项目根目录 = 解析-项目根目录 $项目根目录
$文档 = 读取-工程文档 $项目根目录
[void](导入-VisualStudio开发环境 $平台)
$llvmBin = 'C:\Program Files\LLVM\bin'
if (Test-Path -LiteralPath $llvmBin -PathType Container) {
    $env:Path = $llvmBin + ';' + $env:Path
}
if (-not $工具路径) {
    $工具路径 = Join-Path ([System.IO.Path]::GetTempPath()) `
        '海中鱼巣-clang-ast-build\提取_候选流程图.exe'
}
$工具路径 = [System.IO.Path]::GetFullPath($工具路径)
if (-not (Test-Path -LiteralPath $工具路径 -PathType Leaf)) {
    throw "找不到候选流程图工具: $工具路径"
}

$源文件组 = @(读取-工程源文件 `
    $文档 $项目根目录 @('.cpp') $配置 $平台 ([bool]$启用D455RealSense))
if ($源文件.Count -gt 0) {
    $工程文件集合 = [System.Collections.Generic.HashSet[string]]::new(
        [System.StringComparer]::OrdinalIgnoreCase)
    foreach ($工程文件 in $源文件组) {
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
            throw "具名 .cpp 未登记到当前工程或本地不存在: $绝对路径"
        }
        if (-not $筛选文件组.Contains($绝对路径)) {
            $筛选文件组.Add($绝对路径)
        }
    }
    $源文件组 = $筛选文件组.ToArray()
}
if ($源文件组.Count -eq 0) {
    throw '工程中没有可扫描的 .cpp 文件。'
}
$提交 = (& git -C $项目根目录 rev-parse HEAD).Trim()
$输出路径 = 解析-候选输出路径 `
    $输出路径 `
    $项目根目录 `
    "海中鱼巣-cpp候选流程图-$提交.json"
断言-不是正式图谱输出 $输出路径 $项目根目录
$输出目录 = Split-Path -Parent $输出路径
if ($输出目录) {
    New-Item -ItemType Directory -Force -Path $输出目录 | Out-Null
}

$参数组 = [System.Collections.Generic.List[string]]::new()
foreach ($参数 in @(
    '--compiler', 'C:\Program Files\LLVM\bin\clang.exe',
    '--working-directory', $项目根目录,
    '--project-root', $项目根目录,
    '--observed-commit', $提交,
    '--output', $输出路径)) {
    $参数组.Add($参数)
}
foreach ($文件 in $源文件组) {
    $参数组.Add('--source')
    $参数组.Add($文件)
}
$参数组.Add('--')
$公共参数 = @(形成-Clang公共参数 `
    $文档 `
    $项目根目录 `
    $配置 `
    $平台 `
    ([bool]$启用D455RealSense) `
    $额外编译参数)
foreach ($参数 in $公共参数) {
    $参数组.Add([string]$参数)
}

& $工具路径 @参数组
if ($LASTEXITCODE -ne 0) {
    throw "CPP候选流程图扫描失败，退出码: $LASTEXITCODE"
}
$结果 = Get-Content -LiteralPath $输出路径 -Raw | ConvertFrom-Json
$函数数 = @($结果.files | ForEach-Object { $_.functions }).Count
$调用数 = @($结果.files | ForEach-Object { $_.calls }).Count
$未解析数 = @($结果.files | ForEach-Object { $_.unresolved_calls }).Count
$诊断数 = @($结果.files | ForEach-Object { $_.diagnostics }).Count
$成功数 = @($结果.files | Where-Object { $_.status -eq '成功' }).Count
$带诊断数 = @($结果.files | Where-Object { $_.status -eq '存在诊断' }).Count
$失败数 = @($结果.files | Where-Object { $_.status -eq '解析失败' }).Count
Write-Host "CPP候选: files=$($源文件组.Count), success=$成功数, diagnostics=$带诊断数, failed=$失败数"
Write-Host "functions=$函数数, calls=$调用数"
Write-Host "unresolved_calls=$未解析数, diagnostics=$诊断数"
Write-Host "候选输出: $输出路径"
