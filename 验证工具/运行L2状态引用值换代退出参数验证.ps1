[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)][ValidateSet('Debug', 'Release')][string]$配置,
    [Parameter(Mandatory = $true)][UInt64]$幂等基值,
    [Parameter(Mandatory = $true)][Int64]$精确初始I64,
    [Parameter(Mandatory = $true)][UInt64]$精确换代U64一,
    [Parameter(Mandatory = $true)][UInt64]$精确换代U64二,
    [Parameter(Mandatory = $true)][Int64]$时间初始I64,
    [Parameter(Mandatory = $true)][Int64]$时间换代I64
)
$ErrorActionPreference = 'Stop'
$项目路径 = Join-Path $PSScriptRoot 'L2状态引用值换代退出参数验证.vcxproj'
$本轮目录 = Join-Path ([System.IO.Path]::GetTempPath()) ('海中鱼巣_T2_' + [System.Guid]::NewGuid().ToString('N'))
$输出目录 = Join-Path $本轮目录 '输出'
$中间目录 = Join-Path $本轮目录 '中间'
[System.IO.Directory]::CreateDirectory($输出目录) | Out-Null
[System.IO.Directory]::CreateDirectory($中间目录) | Out-Null
$构建工具 = Get-Command 'MSBuild.exe' -ErrorAction Stop
& $构建工具.Source $项目路径 '/m:1' '/t:Rebuild' "/p:Configuration=$配置" '/p:Platform=x64' "/p:OutDir=$输出目录\" "/p:IntDir=$中间目录\" '/verbosity:minimal'
if ($LASTEXITCODE -ne 0) { throw "T2 专项工程构建失败：$LASTEXITCODE" }
$程序路径 = Join-Path $输出目录 'L2状态引用值换代退出参数验证.exe'
& $程序路径 '--幂等基值' $幂等基值 '--精确初始I64' $精确初始I64 '--精确换代U64一' $精确换代U64一 '--精确换代U64二' $精确换代U64二 '--时间初始I64' $时间初始I64 '--时间换代I64' $时间换代I64
if ($LASTEXITCODE -ne 0) { throw "T2 专项验证失败：$LASTEXITCODE" }
Write-Host "验证临时目录：$本轮目录"
