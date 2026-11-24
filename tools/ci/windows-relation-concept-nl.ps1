#Requires -Version 5.1
<#
.SYNOPSIS
  Windows MSVC Release 编译 + --verify-relation-concept-nl 自检。

.DESCRIPTION
  结论分层：
  1) 编译：正式 海中鱼巣.vcxproj Release|x64
  2) 功能自检：正式入口 --headless --verify-relation-concept-nl
  3) 不声称用户机器正式持久库端到端；本脚本不读写生产库、不启治理外发。
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $false)]
    [string]$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot '..\..')).Path,

    [Parameter(Mandatory = $false)]
    [string]$Configuration = 'Release',

    [Parameter(Mandatory = $false)]
    [string]$Platform = 'x64',

    [Parameter(Mandatory = $false)]
    [string]$ExeDir,

    [Parameter(Mandatory = $false)]
    [string]$OutDir,

    [Parameter(Mandatory = $false)]
    [string]$ArtifactRoot,

    [Parameter(Mandatory = $false)]
    [int]$VerifyTimeoutSec = 600,

    [Parameter(Mandatory = $false)]
    [switch]$SkipBuild
)

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest

function Write-Step([string]$Message) {
    Write-Host ""
    Write-Host ("==== {0} ====" -f $Message) -ForegroundColor Cyan
}

function New-Directory([string]$Path) {
    if (-not (Test-Path -LiteralPath $Path)) {
        New-Item -ItemType Directory -Path $Path -Force | Out-Null
    }
    return (Resolve-Path -LiteralPath $Path).Path
}

function Get-MsbuildPath {
    $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
    if (-not (Test-Path -LiteralPath $vswhere)) {
        throw "vswhere.exe 不存在: $vswhere"
    }
    $installPath = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -property installationPath
    if (-not $installPath) {
        throw 'vswhere 未找到带 MSBuild 的 Visual Studio 安装'
    }
    $msbuild = Join-Path $installPath.Trim() 'MSBuild\Current\Bin\MSBuild.exe'
    if (-not (Test-Path -LiteralPath $msbuild)) {
        throw "MSBuild.exe 不存在: $msbuild"
    }
    return $msbuild
}

Write-Step 'Resolve paths'
$RepoRoot = (Resolve-Path -LiteralPath $RepoRoot).Path
if (-not $ArtifactRoot) {
    $ArtifactRoot = Join-Path $env:RUNNER_TEMP 'hy-ego-relation-concept-nl\artifacts'
}
$ArtifactRoot = New-Directory $ArtifactRoot
$summaryPath = Join-Path $ArtifactRoot 'verify-summary.txt'
$stdoutPath = Join-Path $ArtifactRoot 'verify-stdout.txt'
$stderrPath = Join-Path $ArtifactRoot 'verify-stderr.txt'
$msbuildLog = Join-Path $ArtifactRoot 'msbuild.log'

if (-not $ExeDir) {
    if (-not $OutDir) {
        $buildRoot = Join-Path $env:RUNNER_TEMP 'HY-Ego-build-Release-relation-nl'
        $OutDir = Join-Path $buildRoot 'bin\'
    }
    $ExeDir = $OutDir
}

$exe = Join-Path $ExeDir '海中鱼巣.exe'

if (-not $SkipBuild) {
    Write-Step 'MSBuild Release|x64'
    $obj = Join-Path (Split-Path -Parent $OutDir.TrimEnd('\', '/')) 'obj\'
    New-Directory $OutDir | Out-Null
    New-Directory $obj | Out-Null
    $msbuild = Get-MsbuildPath
    $proj = Join-Path $RepoRoot '海中鱼巣.vcxproj'
    & $msbuild $proj `
        /t:Build /m `
        /p:Configuration=$Configuration `
        /p:Platform=$Platform `
        "/p:OutDir=$OutDir" `
        "/p:IntDir=$obj" `
        /nologo /v:minimal `
        /fl "/flp:LogFile=$msbuildLog;Verbosity=normal"
    if ($LASTEXITCODE -ne 0) {
        throw "msbuild failed: $LASTEXITCODE (log=$msbuildLog)"
    }
}

if (-not (Test-Path -LiteralPath $exe)) {
    throw "missing exe: $exe"
}
foreach ($rel in @('初始化材料', '基础数据\词典')) {
    $p = Join-Path $ExeDir $rel
    if (-not (Test-Path -LiteralPath $p)) {
        throw "PostBuild data missing next to exe: $p"
    }
    if ($rel -eq '初始化材料') {
        $seed = Join-Path $p '世界关系概念种子.txt'
        if (-not (Test-Path -LiteralPath $seed)) {
            throw "missing relation concept seed next to exe: $seed"
        }
    }
}

Write-Step 'Run --verify-relation-concept-nl'
$proc = Start-Process -FilePath $exe `
    -ArgumentList @('--headless', '--verify-relation-concept-nl') `
    -WorkingDirectory $ExeDir `
    -PassThru `
    -NoNewWindow `
    -RedirectStandardOutput $stdoutPath `
    -RedirectStandardError $stderrPath

$sw = [Diagnostics.Stopwatch]::StartNew()
while (-not $proc.HasExited) {
    if ($sw.Elapsed.TotalSeconds -gt $VerifyTimeoutSec) {
        try { Stop-Process -Id $proc.Id -Force -ErrorAction SilentlyContinue } catch {}
        throw "verify timed out after ${VerifyTimeoutSec}s"
    }
    Start-Sleep -Milliseconds 500
}

$exit = $proc.ExitCode
$lines = @(
    "exitCode=$exit"
    "exe=$exe"
    "artifactRoot=$ArtifactRoot"
    "elapsedSec=$([int]$sw.Elapsed.TotalSeconds)"
    "layer1_compile=passed_if_reached_here"
    "layer2_verify_flag=--verify-relation-concept-nl"
    "layer3_formal_user_db=not_run"
)
$lines | Set-Content -LiteralPath $summaryPath -Encoding UTF8
Write-Host ($lines -join "`n")

if ($exit -ne 0) {
    throw "verify failed with exit code $exit (see $stdoutPath / $stderrPath / app logs under D:\海中鱼巣\日志)"
}

Write-Host 'Relation-concept NL outbound verify passed.' -ForegroundColor Green
