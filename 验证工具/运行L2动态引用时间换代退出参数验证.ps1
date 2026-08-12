[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)][ValidateSet('Debug', 'Release')][string]$配置,
    [uint64]$幂等基值 = 81000,
    [int64]$起始初始I64 = 101,
    [uint64]$起始换代U64一 = 202,
    [uint64]$起始换代U64二 = 303,
    [int64]$结束初始I64 = 404,
    [int64]$结束换代I64 = 505
)
$ErrorActionPreference = 'Stop'
$项目路径 = Join-Path $PSScriptRoot 'L2动态引用时间换代退出参数验证.vcxproj'
$本轮目录 = Join-Path ([System.IO.Path]::GetTempPath()) ('海中鱼巣_DYN2_' + [System.Guid]::NewGuid().ToString('N'))
$输出目录 = Join-Path $本轮目录 '输出'
$中间目录 = Join-Path $本轮目录 '中间'
[System.IO.Directory]::CreateDirectory($输出目录) | Out-Null
[System.IO.Directory]::CreateDirectory($中间目录) | Out-Null
$构建工具 = Get-Command 'MSBuild.exe' -ErrorAction Stop
& $构建工具.Source $项目路径 '/m:1' '/t:Rebuild' "/p:Configuration=$配置" '/p:Platform=x64' "/p:OutDir=$输出目录\" "/p:IntDir=$中间目录\" '/verbosity:minimal'
if ($LASTEXITCODE -ne 0) { throw "DYN2 专项工程构建失败：$LASTEXITCODE" }
$程序路径 = Join-Path $输出目录 'L2动态引用时间换代退出参数验证.exe'
& $程序路径 --幂等基值 $幂等基值 --起始初始I64 $起始初始I64 `
    --起始换代U64一 $起始换代U64一 --起始换代U64二 $起始换代U64二 `
    --结束初始I64 $结束初始I64 --结束换代I64 $结束换代I64
if ($LASTEXITCODE -ne 0) { throw "DYN2 专项验证失败：$LASTEXITCODE" }
Write-Host "验证临时目录：$本轮目录"
