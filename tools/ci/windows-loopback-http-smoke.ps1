#Requires -Version 5.1
<#
.SYNOPSIS
  Compile+run a minimal Winsock2 loopback HTTP server on Windows CI (environment probe).

.NOTES
  Same underlying stack as HY-Ego formal web host: Winsock2 TCP + bind(127.0.0.1) + raw HTTP.
  Not HttpListener / HTTP.sys (no URL ACL). Loopback only. No firewall changes.
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $false)]
    [string]$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot '..\..')).Path,

    [Parameter(Mandatory = $false)]
    [string]$ArtifactRoot,

    [Parameter(Mandatory = $false)]
    [int]$ClientTimeoutSec = 30
)

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest

function New-Directory([string]$Path) {
    if (-not (Test-Path -LiteralPath $Path)) {
        New-Item -ItemType Directory -Path $Path -Force | Out-Null
    }
    return (Resolve-Path -LiteralPath $Path).Path
}

$RepoRoot = (Resolve-Path -LiteralPath $RepoRoot).Path
$baseTemp = if ($env:RUNNER_TEMP) { $env:RUNNER_TEMP } else { $env:TEMP }
if (-not $ArtifactRoot) {
    $ArtifactRoot = Join-Path $baseTemp 'hy-ego-loopback-http\artifacts'
}
$ArtifactRoot = New-Directory $ArtifactRoot
$work = New-Directory (Join-Path $baseTemp 'hy-ego-loopback-http\work')
$summary = Join-Path $ArtifactRoot 'loopback-http-summary.txt'

$src = Join-Path $RepoRoot 'tools\ci\windows-loopback-http-smoke.cpp'
if (-not (Test-Path -LiteralPath $src)) {
    throw "missing source: $src"
}
Copy-Item -LiteralPath $src -Destination (Join-Path $work 'windows-loopback-http-smoke.cpp') -Force
$cpp = Join-Path $work 'windows-loopback-http-smoke.cpp'
$exe = Join-Path $work 'windows-loopback-http-smoke.exe'

Write-Host '==== Locate VS / cl.exe ===='
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
if (-not (Test-Path -LiteralPath $vswhere)) { throw "vswhere missing: $vswhere" }
$vsPath = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if (-not $vsPath) { throw 'vswhere found no VC tools install' }
$vsPath = $vsPath.Trim()
Write-Host "VS=$vsPath"

# Import MSVC environment (same toolset family as formal v145 / VS18 image)
$vcvars = Join-Path $vsPath 'VC\Auxiliary\Build\vcvars64.bat'
if (-not (Test-Path -LiteralPath $vcvars)) { throw "vcvars64.bat missing: $vcvars" }

Write-Host '==== Compile Winsock loopback smoke (cl) ===='
$buildCmd = "`"$vcvars`" && cl /nologo /EHsc /W3 /O2 /Fe:`"$exe`" `"$cpp`" Ws2_32.lib"
cmd.exe /c $buildCmd
if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $exe)) {
    throw "cl failed; exe missing ($LASTEXITCODE)"
}
Get-Item $exe | Format-List FullName, Length, LastWriteTime

$server = $null
$listenUrl = $null
$passed = $false
try {
    Write-Host '==== Start loopback HTTP server ===='
    $stdout = Join-Path $work 'server-stdout.txt'
    $stderr = Join-Path $work 'server-stderr.txt'
    if (Test-Path $stdout) { Remove-Item $stdout -Force }
    if (Test-Path $stderr) { Remove-Item $stderr -Force }

    $server = Start-Process -FilePath $exe -WorkingDirectory $work `
        -RedirectStandardOutput $stdout -RedirectStandardError $stderr `
        -PassThru -WindowStyle Hidden
    Write-Host ("server ProcId={0}" -f $server.Id)

    $deadline = (Get-Date).AddSeconds(15)
    while ((Get-Date) -lt $deadline) {
        if ($server.HasExited) {
            $err = if (Test-Path $stderr) { Get-Content $stderr -Raw } else { '' }
            throw ("server exited before LISTEN line; ExitCode={0}; stderr={1}" -f $server.ExitCode, $err)
        }
        if (Test-Path -LiteralPath $stdout) {
            foreach ($line in Get-Content -LiteralPath $stdout -ErrorAction SilentlyContinue) {
                if ($line -match '^LISTEN=(https?://127\.0\.0\.1:\d+/)$') {
                    $listenUrl = $Matches[1]
                    break
                }
            }
        }
        if ($listenUrl) { break }
        Start-Sleep -Milliseconds 100
    }
    if (-not $listenUrl) {
        throw 'timeout waiting for LISTEN=http://127.0.0.1:<port>/ from server stdout'
    }
    Write-Host ("listenUrl={0} (Winsock bind INADDR_LOOPBACK, ephemeral port)" -f $listenUrl)

    Write-Host '==== Client GET (loopback) ===='
    $response = Invoke-WebRequest -Uri $listenUrl -Method GET -UseBasicParsing -TimeoutSec $ClientTimeoutSec
    $code = [int]$response.StatusCode
    $body = [string]$response.Content
    Write-Host ("HTTP status={0}" -f $code)
    Write-Host ("HTTP body=<<{0}>>" -f $body)
    if ($code -ne 200) { throw "unexpected status $code" }
    if ($body -ne 'hy-ego-loopback-http-ok') {
        throw ("unexpected body: {0}" -f $body)
    }

    # Server handles one request then exits
    $exitDeadline = (Get-Date).AddSeconds(15)
    while (-not $server.HasExited -and (Get-Date) -lt $exitDeadline) {
        Start-Sleep -Milliseconds 100
    }
    if (-not $server.HasExited) {
        throw ("server did not exit after single response; ProcId={0}" -f $server.Id)
    }
    if ($server.ExitCode -ne 0) {
        throw ("server ExitCode={0}" -f $server.ExitCode)
    }
    Write-Host ("server exited cleanly ExitCode={0}" -f $server.ExitCode)

    # Port should be free after close (best-effort observation)
    $uri = [Uri]$listenUrl
    $portInUse = Get-NetTCPConnection -LocalPort $uri.Port -State Listen -ErrorAction SilentlyContinue
    if ($portInUse) {
        throw ("port {0} still in Listen after server exit" -f $uri.Port)
    }
    Write-Host ("port {0} no longer Listening (cleanup ok)" -f $uri.Port)

    $passed = $true
    $text = @"
结论: Windows Winsock 回环 HTTP 环境探针通过（非 HY-Ego 业务冒烟）
- 机制: Winsock2 socket/bind(INADDR_LOOPBACK)/listen/accept/send（同正式宿主；非 HttpListener/HTTP.sys）
- listen: $listenUrl
- GET status: $code
- GET body: $body
- server ExitCode: $($server.ExitCode)
- port cleanup: ok
- 边界: 只证明 runner 可做 loopback TCP+原始 HTTP；不证明正式入口会话启动/锁/弹窗路径可用
"@
}
catch {
    $text = @"
结论: Windows Winsock 回环 HTTP 环境探针失败
错误: $($_.Exception.Message)
"@
    throw
}
finally {
    if ($null -ne $server -and -not $server.HasExited) {
        try { & taskkill.exe /PID $server.Id /T /F 2>$null | Out-Null } catch { }
    }
    foreach ($f in @((Join-Path $work 'server-stdout.txt'), (Join-Path $work 'server-stderr.txt'))) {
        if (Test-Path -LiteralPath $f) {
            Copy-Item -LiteralPath $f -Destination (Join-Path $ArtifactRoot ([IO.Path]::GetFileName($f))) -Force
        }
    }
    if (Test-Path -LiteralPath $exe) {
        Copy-Item -LiteralPath $exe -Destination (Join-Path $ArtifactRoot 'windows-loopback-http-smoke.exe') -Force -ErrorAction SilentlyContinue
    }
    if ($env:GITHUB_WORKSPACE) {
        $stage = Join-Path $env:GITHUB_WORKSPACE '_ci_upload_loopback_http'
        New-Directory $stage | Out-Null
        Copy-Item -Path (Join-Path $ArtifactRoot '*') -Destination $stage -Recurse -Force -ErrorAction SilentlyContinue
    }
    $text | Set-Content -LiteralPath $summary -Encoding UTF8
    Write-Host $text
}

if (-not $passed) { exit 1 }
exit 0
