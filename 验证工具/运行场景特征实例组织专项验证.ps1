[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [ValidateSet('Debug', 'Release')]
    [string]$Configuration
)

$ErrorActionPreference = 'Stop'
$项目路径 = Join-Path $PSScriptRoot '场景特征实例组织专项验证.vcxproj'
$本轮目录 = Join-Path 'D:\TEMP\海中鱼巣\DATA-L2-SCENE-FEATURE-INSTANCE-ORGANIZATION' ([System.Guid]::NewGuid().ToString('N'))
$构建工具 = Get-Command 'MSBuild.exe' -ErrorAction Stop
foreach ($探针 in @(
    @{ 模式 = 'ExistenceProvider'; 程序 = '场景特征存在组合提供者专项验证.exe' },
    @{ 模式 = 'SceneOrganization'; 程序 = '场景特征组织专项验证.exe' }
)) {
    $探针目录 = Join-Path $本轮目录 $探针.模式
    $输出目录 = Join-Path $探针目录 '输出'
    $中间目录 = Join-Path $探针目录 '中间'
    [System.IO.Directory]::CreateDirectory($输出目录) | Out-Null
    [System.IO.Directory]::CreateDirectory($中间目录) | Out-Null
    & $构建工具.Source $项目路径 '/m:1' '/t:Rebuild' "/p:Configuration=$Configuration" `
        '/p:Platform=x64' "/p:ProbeMode=$($探针.模式)" "/p:OutDir=$输出目录\" `
        "/p:IntDir=$中间目录\" '/verbosity:minimal'
    if ($LASTEXITCODE -ne 0) { throw "$($探针.模式) 探针构建失败，退出码 $LASTEXITCODE。" }
    $程序路径 = Join-Path $输出目录 $探针.程序
    if (-not (Test-Path -LiteralPath $程序路径 -PathType Leaf)) { throw "验证程序不存在：$程序路径" }
    & $程序路径
    if ($LASTEXITCODE -ne 0) { throw "$($探针.模式) 探针运行失败，退出码 $LASTEXITCODE。" }
}
Write-Host "验证临时目录：$本轮目录"
