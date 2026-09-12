param(
    [string]$OutputRoot = 'D:/TEMP/海中鱼巣/WORLD-TREE-ROOT-BOOTSTRAP/01a06f6b-20260912/special',
    [ValidateSet('Debug','Release')][string]$Configuration = 'Debug'
)
$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path $PSScriptRoot -Parent
$project = Join-Path $PSScriptRoot '世界树根启动专项验证.vcxproj'
$msbuild = 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe'
$resolvedOutput = [IO.Path]::GetFullPath($OutputRoot).TrimEnd('\','/')
$allowedRoot = [IO.Path]::GetFullPath('D:/TEMP/海中鱼巣/WORLD-TREE-ROOT-BOOTSTRAP/01a06f6b-20260912/special').TrimEnd('\','/')
if (-not $resolvedOutput.Equals($allowedRoot,[StringComparison]::OrdinalIgnoreCase)) {
    throw 'Unexpected output root'
}
$interface = Join-Path $repoRoot '海中鱼巣/装配.普通应用.ixx'
$implementation = Join-Path $repoRoot '海中鱼巣/装配.普通应用.cppcpp'
$rootContract = Join-Path $repoRoot '海中鱼巣/领域/合同.世界树根.ixx'
$rootService = Join-Path $repoRoot '海中鱼巣/领域/数据服务.世界树根.ixx'
$interfaceText = [IO.File]::ReadAllText($interface)
$implementationText = [IO.File]::ReadAllText($implementation)
if ($interfaceText -match '(?m)^\s*(export\s+)?import\s+海中鱼巣\.(领域|业务)\.' -or
    $interfaceText -match '普通应用装配结果\s+构造普通应用上下文\s*\([^;]*\)\s*\{') {
    throw 'ordinary assembly interface contains forbidden imports or implementation'
}
if ($implementationText -notmatch '(?m)^module 海中鱼巣\.装配\.普通应用;' -or
    $implementationText -notmatch '普通应用装配结果\s+构造普通应用上下文\s*\(') {
    throw 'ordinary assembly implementation unit is incomplete'
}
$publicText = [IO.File]::ReadAllText($rootContract) + [IO.File]::ReadAllText($rootService)
if ($publicText -match '发布世界树根节点') {
    throw 'public root publication symbol exists'
}
[void](New-Item -ItemType Directory -Path $resolvedOutput -Force)
$configRoot = Join-Path $resolvedOutput $Configuration
[void](New-Item -ItemType Directory -Path $configRoot -Force)
$buildLog = Join-Path $configRoot 'build.log'
$coreLog = Join-Path $configRoot 'core-run.log'
$ordinaryLog = Join-Path $configRoot 'ordinary-run.log'
& $msbuild $project /t:Rebuild /m:1 "/p:Configuration=$Configuration" /p:Platform=x64 `
    /p:PlatformToolset=v145 /p:VCToolsVersion=14.50.35717 `
    "/p:OutDir=$configRoot/out/" "/p:IntDir=$configRoot/obj/" `
    /p:TargetName=world-tree-root-special /v:minimal *> $buildLog
if ($LASTEXITCODE) { throw "$Configuration specialty Rebuild failed: $LASTEXITCODE" }
$exe = Join-Path $configRoot 'out/world-tree-root-special.exe'
& $exe *> $coreLog
if ($LASTEXITCODE) { throw "$Configuration core probe failed: $LASTEXITCODE" }
& $exe --ordinary *> $ordinaryLog
if ($LASTEXITCODE) { throw "$Configuration ordinary entry probe failed: $LASTEXITCODE" }
[pscustomobject]@{
    Configuration = $Configuration
    RebuildExit = 0
    CoreExit = 0
    OrdinaryExit = 0
    CoreLast = [string](Get-Content -LiteralPath $coreLog -Tail 1)
    OrdinaryLast = [string](Get-Content -LiteralPath $ordinaryLog -Tail 1)
    InterfaceSplit = 'PASS'
    PublicRootPublisher = 'ABSENT'
} | ConvertTo-Json -Depth 4 | Set-Content -LiteralPath (Join-Path $configRoot 'result.json') -Encoding utf8
Get-Content -LiteralPath (Join-Path $configRoot 'result.json')
