[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [ValidateSet('Debug', 'Release')]
    [string]$配置,

    [UInt32]$合同版本 = 1,

    [UInt32]$规则版本 = 1,

    [UInt64]$首次期望事实代次 = 0
)

$ErrorActionPreference = 'Stop'
$项目路径 = Join-Path $PSScriptRoot 'L2场景关系类型登记参数验证.vcxproj'
$临时根 = Join-Path ([System.IO.Path]::GetTempPath()) '海中鱼巣_L2场景关系类型登记参数验证'
$本轮目录 = Join-Path $临时根 ([System.Guid]::NewGuid().ToString('N'))
$输出目录 = Join-Path $本轮目录 '输出'
$中间目录 = Join-Path $本轮目录 '中间'
[System.IO.Directory]::CreateDirectory($输出目录) | Out-Null
[System.IO.Directory]::CreateDirectory($中间目录) | Out-Null

$构建工具 = Get-Command 'MSBuild.exe' -ErrorAction Stop
& $构建工具.Source $项目路径 '/m:1' '/t:Rebuild' "/p:Configuration=$配置" '/p:Platform=x64' "/p:OutDir=$输出目录\" "/p:IntDir=$中间目录\" '/verbosity:minimal'
if ($LASTEXITCODE -ne 0) {
    throw "L2 场景关系类型登记参数验证项目构建失败，退出码 $LASTEXITCODE。"
}

$程序路径 = Join-Path $输出目录 'L2场景关系类型登记参数验证.exe'
if (-not (Test-Path -LiteralPath $程序路径 -PathType Leaf)) {
    throw "验证程序不存在：$程序路径"
}

Write-Host "验证临时目录：$本轮目录"
& $程序路径 '--合同版本' $合同版本 '--规则版本' $规则版本 '--首次期望事实代次' $首次期望事实代次
if ($LASTEXITCODE -ne 0) {
    throw "L2 场景关系类型登记参数验证失败，退出码 $LASTEXITCODE。"
}
