#Requires -Version 5.1
<#
.SYNOPSIS
  Windows MSVC Release 编译（可选）+ 正式入口启动/收发接口/关闭冒烟。

.DESCRIPTION
  结论范围严格限定为：进程能启动、HTTP 收发接口按现行合同返回期望状态码、关闭接口可调用。
  HTTP 202 仅表示消息入队 Accepted，不表示消息已理解或业务闭环。

.NOTES
  数据路径：读取普通应用程序目录() = GetModuleFileName 的父目录（exe 旁），不是 cwd。
  PostBuild 已把 初始化材料 / 基础数据\词典 复制到 OutDir；本脚本不另 checkout 数据树。
  WorkingDirectory 仍设为 ExeDir，与正式入口习惯一致，并保证相对路径旁路可定位。
  OutDir / ArtifactRoot 落在 RUNNER_TEMP（或调用方指定目录），与仓库工作树隔离。
  会话 URL 旁路文件硬编码为 D:/TEMP/海中鱼巣/last-session-url.txt（现行 main），
  故 CI 必须先准备 D: 合同路径（workflow 的 subst 步骤）。
  任何启动/HTTP/超时/异常路径均在 finally 中仅清理本次 Start-Process 得到的进程树，并尽量收集证据。
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $false)]
    [string]$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot '..\..')).Path,

    [Parameter(Mandatory = $false)]
    [string]$Configuration = 'Release',

    [Parameter(Mandatory = $false)]
    [string]$Platform = 'x64',

    # 若已由 workflow 编好，传入 exe 所在目录并跳过编译
    [Parameter(Mandatory = $false)]
    [string]$ExeDir,

    [Parameter(Mandatory = $false)]
    [string]$OutDir,

    [Parameter(Mandatory = $false)]
    [string]$ArtifactRoot,

    [Parameter(Mandatory = $false)]
    [int]$StartupTimeoutSec = 180,

    [Parameter(Mandatory = $false)]
    [int]$HttpTimeoutSec = 30,

    [Parameter(Mandatory = $false)]
    [string]$ProbeText = 'ci-smoke-probe',

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

function Test-ExactSessionUrl([string]$Candidate) {
    # 只接受完整可解析的 http(s) 绝对 URL；拒绝空串、空白、截断半行、缺 scheme。
    if ([string]::IsNullOrWhiteSpace($Candidate)) { return $false }
    $trimmed = $Candidate.Trim()
    # 现行写出形如 http://127.0.0.1:<port>/session/<token>，不允许空白字符夹杂
    if ($trimmed -notmatch '^https?://\S+$') { return $false }
    try {
        $uri = [Uri]$trimmed
        if (-not $uri.IsAbsoluteUri) { return $false }
        if ($uri.Scheme -ne 'http' -and $uri.Scheme -ne 'https') { return $false }
        if ([string]::IsNullOrWhiteSpace($uri.Host)) { return $false }
        if ($uri.Port -le 0) { return $false }
        # 路径应含 /session/（现行合同）；过宽接受会拿非法 URL 继续
        if ($uri.AbsolutePath -notmatch '^/session/') { return $false }
        return $true
    } catch {
        return $false
    }
}

function New-UuidV4DialogueId {
    # 对齐 是合法对话框标识：版本 nibble=4，variant ∈ {8,9,a,b}，全小写
    $g = [guid]::NewGuid().ToString('D').ToLowerInvariant().ToCharArray()
    $g[14] = '4'
    $variants = @('8', '9', 'a', 'b')
    $g[19] = $variants[(Get-Random -Maximum 4)]
    return -join $g
}

function Invoke-StatusRequest {
    param(
        [Parameter(Mandatory = $true)][string]$Method,
        [Parameter(Mandatory = $true)][string]$Uri,
        [Parameter(Mandatory = $false)][string]$Body,
        [Parameter(Mandatory = $false)][hashtable]$Headers,
        [Parameter(Mandatory = $false)][string]$ContentType,
        [Parameter(Mandatory = $true)][int]$TimeoutSec,
        [Parameter(Mandatory = $true)][int[]]$ExpectedStatus
    )
    $params = @{
        Method          = $Method
        Uri             = $Uri
        TimeoutSec      = $TimeoutSec
        UseBasicParsing = $true
    }
    if ($PSBoundParameters.ContainsKey('Headers') -and $Headers) {
        $params['Headers'] = $Headers
    }
    if ($PSBoundParameters.ContainsKey('Body') -and $null -ne $Body) {
        $params['Body'] = [System.Text.Encoding]::UTF8.GetBytes($Body)
    }
    if ($PSBoundParameters.ContainsKey('ContentType') -and $ContentType) {
        $params['ContentType'] = $ContentType
    }

    $code = $null
    $content = ''
    $respHeaders = $null
    try {
        $response = Invoke-WebRequest @params
        $code = [int]$response.StatusCode
        $content = [string]$response.Content
        $respHeaders = $response.Headers
    } catch {
        $resp = $_.Exception.Response
        if ($null -eq $resp) { throw }
        $code = [int]$resp.StatusCode
        $respHeaders = $resp.Headers
        try {
            $stream = $resp.GetResponseStream()
            if ($stream) {
                $reader = New-Object System.IO.StreamReader($stream)
                $content = $reader.ReadToEnd()
                $reader.Dispose()
            }
        } catch { }
    }

    if ($ExpectedStatus -notcontains $code) {
        throw ("HTTP {0} {1} 得到 {2}，期望之一: {3}; body={4}" -f `
            $Method, $Uri, $code, ($ExpectedStatus -join ','), $content)
    }
    return [pscustomobject]@{
        StatusCode = $code
        Headers    = $respHeaders
        Content    = $content
    }
}

function Stop-OwnedProcessTree {
    param(
        [Parameter(Mandatory = $false)][System.Diagnostics.Process]$Process,
        [Parameter(Mandatory = $false)][int]$GraceMs = 5000
    )
    if ($null -eq $Process) { return }
    $procId = $Process.Id
    try {
        if ($Process.HasExited) {
            Write-Host ("进程已退出 ProcId={0} ExitCode={1}" -f $procId, $Process.ExitCode)
            return
        }
    } catch {
        Write-Host ("进程句柄不可用 ProcId={0}: {1}" -f $procId, $_.Exception.Message)
        return
    }
    Write-Host ("清理本次启动进程树 ProcId={0} ..." -f $procId) -ForegroundColor Yellow
    try {
        # /T = 仅该 PID 及其子树；不按映像名全局杀
        & taskkill.exe /PID $procId /T /F 2>$null | Out-Null
    } catch {
        Write-Host ("taskkill 异常（继续）: {0}" -f $_.Exception.Message) -ForegroundColor Yellow
    }
    $sw = [Diagnostics.Stopwatch]::StartNew()
    while (-not $Process.HasExited -and $sw.ElapsedMilliseconds -lt $GraceMs) {
        Start-Sleep -Milliseconds 100
    }
    if (-not $Process.HasExited) {
        try { $Process.Kill() } catch { }
    }
}

function Copy-Evidence {
    param(
        [Parameter(Mandatory = $true)][string]$Source,
        [Parameter(Mandatory = $true)][string]$DestinationDir,
        [Parameter(Mandatory = $true)][string]$Name
    )
    if (-not (Test-Path -LiteralPath $Source)) { return }
    New-Directory $DestinationDir | Out-Null
    Copy-Item -LiteralPath $Source -Destination (Join-Path $DestinationDir $Name) -Force -ErrorAction SilentlyContinue
}

# ---- 路径与隔离目录 ----
$RepoRoot = (Resolve-Path -LiteralPath $RepoRoot).Path
$baseTemp = if ($env:RUNNER_TEMP) { $env:RUNNER_TEMP } else { $env:TEMP }
if (-not $ArtifactRoot) {
    $ArtifactRoot = Join-Path $baseTemp 'hy-ego-smoke\artifacts'
}
$ArtifactRoot = New-Directory $ArtifactRoot
$EvidenceDir = New-Directory (Join-Path $ArtifactRoot 'evidence')
$BuildLogPath = Join-Path $ArtifactRoot 'msbuild.log'
$SummaryPath = Join-Path $ArtifactRoot 'smoke-summary.txt'

$doBuild = -not $SkipBuild -and [string]::IsNullOrWhiteSpace($ExeDir)
if ($doBuild) {
    if (-not $OutDir) {
        $OutDir = Join-Path $baseTemp 'hy-ego-msvc-smoke\out'
    }
    $OutDir = New-Directory $OutDir
} elseif (-not [string]::IsNullOrWhiteSpace($ExeDir)) {
    $OutDir = (Resolve-Path -LiteralPath $ExeDir).Path
} else {
    throw '必须提供 -ExeDir，或允许编译（不要同时 SkipBuild 且无 ExeDir）'
}

$exeName = '海中鱼巣'
# 现行 main 硬编码旁路路径（宿主.网页对话.cpp）
$sessionUrlFile = 'D:\TEMP\海中鱼巣\last-session-url.txt'
if (-not (Test-Path -LiteralPath 'D:\TEMP\海中鱼巣')) {
    throw '缺少 D:\TEMP\海中鱼巣。workflow 必须先 subst/准备 D: 合同路径后再跑本脚本。'
}
if (Test-Path -LiteralPath $sessionUrlFile) {
    Remove-Item -LiteralPath $sessionUrlFile -Force
}

$ownedProcess = $null
$smokePassed = $false
$sessionUrl = $null
$pageStatus = $null
$postStatus = $null
$closeStatus = $null
$conclusion = '未完成'

try {
    if ($doBuild) {
        Write-Step '定位 MSBuild'
        $msbuild = Get-MsbuildPath
        Write-Host "MSBuild: $msbuild"

        Write-Step 'MSVC Release 编译（OutDir 隔离；/fl 持久化日志）'
        $project = Join-Path $RepoRoot '海中鱼巣.vcxproj'
        if (-not (Test-Path -LiteralPath $project)) {
            throw "找不到工程文件: $project"
        }
        $intDir = Join-Path $OutDir 'obj'
        New-Directory $intDir | Out-Null
        # EnableD455RealSense 默认 false，无需 vcpkg
        $msbuildArgs = @(
            $project
            '/t:Build'
            '/m'
            "/p:Configuration=$Configuration"
            "/p:Platform=$Platform"
            "/p:OutDir=$OutDir\"
            "/p:IntDir=$intDir\"
            '/nologo'
            '/v:minimal'
            '/fl'
            "/flp:LogFile=$BuildLogPath;Verbosity=normal"
        )
        Write-Host ("msbuild {0}" -f ($msbuildArgs -join ' '))
        & $msbuild @msbuildArgs
        if ($LASTEXITCODE -ne 0) {
            throw ("MSBuild 失败，ExitCode={0}；日志: {1}" -f $LASTEXITCODE, $BuildLogPath)
        }
    } else {
        Write-Step '跳过编译（使用已有 ExeDir）'
        Write-Host "ExeDir=$OutDir"
    }

    $exePath = Join-Path $OutDir ($exeName + '.exe')
    if (-not (Test-Path -LiteralPath $exePath)) {
        throw "未找到正式入口: $exePath"
    }

    Write-Step '核对 exe 旁数据/词典/种子（PostBuild → OutDir；相对 ExeDir 非 cwd）'
    $requiredRelative = @(
        '初始化材料',
        '基础数据\词典'
    )
    foreach ($rel in $requiredRelative) {
        $p = Join-Path $OutDir $rel
        if (-not (Test-Path -LiteralPath $p)) {
            throw ("正式入口依赖路径缺失（应在 exe 旁，由 PostBuild 复制；无需另 checkout 数据）: {0}" -f $p)
        }
        Write-Host "OK: $p"
    }

    Write-Step '启动正式入口（WorkingDirectory=ExeDir）'
    $ownedProcess = Start-Process -FilePath $exePath -WorkingDirectory $OutDir -PassThru -WindowStyle Hidden
    Write-Host ("已启动 ProcId={0}" -f $ownedProcess.Id)

    Write-Step '等待合法 session URL（D:\TEMP\海中鱼巣\last-session-url.txt）'
    $deadline = (Get-Date).AddSeconds($StartupTimeoutSec)
    while ((Get-Date) -lt $deadline) {
        if ($ownedProcess.HasExited) {
            throw ("正式入口在写出 URL 前退出，ExitCode={0}" -f $ownedProcess.ExitCode)
        }
        if (Test-Path -LiteralPath $sessionUrlFile) {
            $raw = Get-Content -LiteralPath $sessionUrlFile -Raw -ErrorAction SilentlyContinue
            if ($null -ne $raw) {
                foreach ($line in ($raw -split "`r?`n")) {
                    $candidate = $line.Trim()
                    if ([string]::IsNullOrWhiteSpace($candidate)) { continue }
                    if (Test-ExactSessionUrl $candidate) {
                        $sessionUrl = $candidate.TrimEnd('/')
                        Copy-Evidence -Source $sessionUrlFile -DestinationDir $EvidenceDir -Name 'last-session-url.txt'
                        Write-Host ("session URL: {0}" -f $sessionUrl)
                        break
                    }
                    # 非空但非法：继续等待，绝不拿半截 URL 往下跑
                    Write-Host ("忽略非合法 URL 行，继续等待: {0}" -f $candidate) -ForegroundColor Yellow
                }
            }
        }
        if ($sessionUrl) { break }
        Start-Sleep -Milliseconds 500
    }
    if (-not $sessionUrl) {
        throw ("等待合法 session URL 超时（{0}s）: {1}" -f $StartupTimeoutSec, $sessionUrlFile)
    }

    # 现行路由：基础路径 = /session/<token>
    # GET 基础路径 → 200 text/html
    # POST 基础路径/api/messages + X-HY-Dialogue-Id + 非空 UTF-8 正文 → 202
    # POST 基础路径/api/close 空正文 → 200
    Write-Step 'GET 会话页（启动可达冒烟）'
    $page = Invoke-StatusRequest -Method 'GET' -Uri $sessionUrl -TimeoutSec $HttpTimeoutSec -ExpectedStatus @(200)
    $pageStatus = $page.StatusCode
    Write-Host ("GET session → {0}" -f $pageStatus)

    Write-Step 'POST /api/messages（仅验证入队接口 202；不声称理解/业务闭环）'
    $dialogueId = New-UuidV4DialogueId
    $msgHeaders = @{ 'X-HY-Dialogue-Id' = $dialogueId }
    $post = Invoke-StatusRequest -Method 'POST' -Uri ($sessionUrl + '/api/messages') `
        -Headers $msgHeaders -Body $ProbeText -ContentType 'text/plain;charset=utf-8' `
        -TimeoutSec $HttpTimeoutSec -ExpectedStatus @(202)
    $postStatus = $post.StatusCode
    Write-Host ("POST /api/messages → {0}（Accepted=入队，非业务完成）dialogueId={1}" -f $postStatus, $dialogueId)

    Write-Step 'POST /api/close（关闭冒烟）'
    $close = Invoke-StatusRequest -Method 'POST' -Uri ($sessionUrl + '/api/close') `
        -TimeoutSec $HttpTimeoutSec -ExpectedStatus @(200)
    $closeStatus = $close.StatusCode
    Write-Host ("POST /api/close → {0}" -f $closeStatus)

    # 给进程自行退出一点时间；超时则 finally 强杀本次进程树
    $exitDeadline = (Get-Date).AddSeconds(20)
    while (-not $ownedProcess.HasExited -and (Get-Date) -lt $exitDeadline) {
        Start-Sleep -Milliseconds 200
    }

    $smokePassed = $true
    $conclusion = @"
结论: 启动/收发接口/关闭冒烟通过（限定范围）
- 编译: $(if ($doBuild) { "MSVC $Configuration|$Platform → $exePath" } else { "复用 ExeDir=$OutDir" })
- 启动: WorkingDirectory=ExeDir=$OutDir ProcId=$($ownedProcess.Id)
- GET session → $pageStatus
- POST /api/messages → $postStatus（仅表示入队 Accepted，不表示消息已理解或业务闭环）
- POST /api/close → $closeStatus
- 未验证: 消息理解、任务执行、业务闭环、持久化语义、控制面板业务
"@
}
catch {
    $conclusion = @"
结论: 冒烟失败（范围仍仅为启动/收发接口/关闭；本结果不涉及业务闭环判定）
错误: $($_.Exception.Message)
"@
    throw
}
finally {
    Write-Step '收集证据并清理本次进程'
    Copy-Evidence -Source $sessionUrlFile -DestinationDir $EvidenceDir -Name 'last-session-url.txt'
    if (Test-Path -LiteralPath $BuildLogPath) {
        Copy-Evidence -Source $BuildLogPath -DestinationDir $EvidenceDir -Name 'msbuild.log'
    }
    # 日志目录（若已写出）
    if (Test-Path -LiteralPath 'D:\海中鱼巣\日志') {
        try {
            $logCopy = Join-Path $EvidenceDir 'logs'
            New-Directory $logCopy | Out-Null
            Copy-Item -Path 'D:\海中鱼巣\日志\*' -Destination $logCopy -Recurse -Force -ErrorAction SilentlyContinue
        } catch { }
    }

    Stop-OwnedProcessTree -Process $ownedProcess

    $conclusion | Set-Content -LiteralPath $SummaryPath -Encoding UTF8
    Write-Host $conclusion
    Write-Host ("摘要: {0}" -f $SummaryPath)
    Write-Host ("构建日志(若有): {0}" -f $BuildLogPath)
}

if (-not $smokePassed) {
    exit 1
}
exit 0
