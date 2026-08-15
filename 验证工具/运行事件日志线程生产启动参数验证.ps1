[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [ValidateSet('Debug', 'Release')]
    [string]$配置,

    [UInt64]$运行代次 = 815001
)

$ErrorActionPreference = 'Stop'
$项目路径 = Join-Path $PSScriptRoot '事件日志线程生产启动参数验证.vcxproj'
$临时根 = Join-Path ([System.IO.Path]::GetTempPath()) '海中鱼巣_EVENT_LOG_THREAD_PRODUCTION_START'
$本轮目录 = Join-Path $临时根 ([System.Guid]::NewGuid().ToString('N'))
$输出目录 = Join-Path $本轮目录 '输出'
$中间目录 = Join-Path $本轮目录 '中间'
[System.IO.Directory]::CreateDirectory($输出目录) | Out-Null
[System.IO.Directory]::CreateDirectory($中间目录) | Out-Null

$构建工具 = Get-Command 'MSBuild.exe' -ErrorAction SilentlyContinue
if (-not $构建工具) {
    $查找器 = 'C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe'
    $安装目录 = & $查找器 -latest -products * -requires Microsoft.Component.MSBuild -property installationPath
    $构建工具路径 = Join-Path $安装目录 'MSBuild\Current\Bin\MSBuild.exe'
} else {
    $构建工具路径 = $构建工具.Source
}

& $构建工具路径 $项目路径 '/m:1' '/t:Rebuild' "/p:Configuration=$配置" '/p:Platform=x64' "/p:OutDir=$输出目录\" "/p:IntDir=$中间目录\" '/verbosity:minimal'
if ($LASTEXITCODE -ne 0) {
    throw "事件日志线程生产启动参数验证项目构建失败，退出码 $LASTEXITCODE。"
}

$程序路径 = Join-Path $输出目录 '事件日志线程生产启动参数验证.exe'
if (-not (Test-Path -LiteralPath $程序路径 -PathType Leaf)) {
    throw "验证程序不存在：$程序路径"
}

Write-Host "验证临时目录：$本轮目录"
& $程序路径 '--运行代次' $运行代次
if ($LASTEXITCODE -ne 0) {
    throw "事件日志线程生产启动参数验证失败，退出码 $LASTEXITCODE。"
}
