[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [ValidateSet('Debug', 'Release')]
    [string]$配置
)

$ErrorActionPreference = 'Stop'
$项目路径 = Join-Path $PSScriptRoot '共享截止世界与自我快照消费参数验证.vcxproj'
$任务根 = 'D:\TEMP\海中鱼巣\WORLD-SELF-CURRENT-SNAPSHOT-CONSUME\20260817-v04'
$本轮目录 = Join-Path $任务根 ([System.Guid]::NewGuid().ToString('N'))
$输出目录 = Join-Path $本轮目录 '输出'
$中间目录 = Join-Path $本轮目录 '中间'
$材料目录 = Join-Path $本轮目录 '材料'
[System.IO.Directory]::CreateDirectory($输出目录) | Out-Null
[System.IO.Directory]::CreateDirectory($中间目录) | Out-Null
[System.IO.Directory]::CreateDirectory($材料目录) | Out-Null

$构建工具路径 = 'C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe'
if (-not (Test-Path -LiteralPath $构建工具路径 -PathType Leaf)) {
    $构建工具路径 = (Get-Command 'MSBuild.exe' -ErrorAction Stop).Source
}

& $构建工具路径 $项目路径 '/m:1' '/t:Rebuild' "/p:Configuration=$配置" '/p:Platform=x64' "/p:OutDir=$输出目录\" "/p:IntDir=$中间目录\" '/verbosity:minimal'
if ($LASTEXITCODE -ne 0) {
    throw "共享截止世界与自我快照消费参数验证项目构建失败，退出码 $LASTEXITCODE。"
}

$程序路径 = Join-Path $输出目录 '共享截止世界与自我快照消费参数验证.exe'
if (-not (Test-Path -LiteralPath $程序路径 -PathType Leaf)) {
    throw "验证程序不存在：$程序路径"
}

Write-Host "验证临时目录：$本轮目录"
& $程序路径 $材料目录
if ($LASTEXITCODE -ne 0) {
    throw "共享截止世界与自我快照消费参数验证失败，退出码 $LASTEXITCODE。"
}

Write-Host "共享截止世界与自我快照消费参数验证通过：$配置"
