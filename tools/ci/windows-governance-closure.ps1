#Requires -Version 5.1
<#
.SYNOPSIS
  GB-3：Windows 正式入口 + 真实网页 DOM 驱动的需求获取/理解/满足闭环观测。

.DESCRIPTION
  结论范围：在默认种子与绑定服务对象条件下，通过 Playwright 操作真实页面 #input/#form，
  观察接收→需求/任务/方法/结算相关可观测证据。不声称 202 即闭环；不注入需求节点；不改业务语义。

.NOTES
  启动参数：--web-service-object-login=zch2005（与装配默认第一权限人登录名一致）。
  正向 NL 选择理由见 evidence/case-plan.json。
#>
[CmdletBinding()]
param(
    [Parameter(Mandatory = $false)]
    [string]$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot '..\..')).Path,

    [Parameter(Mandatory = $false)]
    [string]$ExeDir,

    [Parameter(Mandatory = $false)]
    [string]$ArtifactRoot,

    [Parameter(Mandatory = $false)]
    [int]$StartupTimeoutSec = 180,

    [Parameter(Mandatory = $false)]
    [int]$ObserveMs = 360000,

    [Parameter(Mandatory = $false)]
    [string]$ProbeNl = '请把登录名为zch2005的人的姓名保持为周春海',

    [Parameter(Mandatory = $false)]
    [string]$NegativeNl = 'asdfqwer没有结构的乱码输入',

    [Parameter(Mandatory = $false)]
    [string]$ServiceObjectLogin = 'zch2005',

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

function Test-ExactSessionUrl([string]$Url) {
    return $Url -match '^http://127\.0\.0\.1:\d+/session/[0-9a-f]{32}$'
}

function Stop-ProcessTree([System.Diagnostics.Process]$Process) {
    if ($null -eq $Process) { return }
    try {
        if (-not $Process.HasExited) {
            Write-Host ("清理本次启动进程树 ProcId={0} ..." -f $Process.Id)
            & taskkill.exe /PID $Process.Id /T /F 2>$null | Out-Null
        }
    } catch {
        Write-Host ("清理进程失败: {0}" -f $_.Exception.Message)
    }
}

function Copy-Evidence([string]$Source, [string]$DestinationDir, [string]$Name) {
    if (-not (Test-Path -LiteralPath $Source)) { return }
    $dest = Join-Path $DestinationDir $Name
    Copy-Item -LiteralPath $Source -Destination $dest -Force -ErrorAction SilentlyContinue
}

$RepoRoot = (Resolve-Path -LiteralPath $RepoRoot).Path
$baseTemp = if ($env:RUNNER_TEMP) { $env:RUNNER_TEMP } else { $env:TEMP }
if (-not $ArtifactRoot) {
    $ArtifactRoot = Join-Path $baseTemp 'hy-ego-governance-closure\artifacts'
}
$ArtifactRoot = New-Directory $ArtifactRoot
$EvidenceDir = New-Directory (Join-Path $ArtifactRoot 'evidence')
$PageEvidenceDir = New-Directory (Join-Path $EvidenceDir 'page')
$SummaryPath = Join-Path $ArtifactRoot 'governance-closure-summary.txt'
$BuildLogPath = Join-Path $ArtifactRoot 'msbuild.log'

if (-not $ExeDir) {
    throw '必须提供 -ExeDir（workflow 编译后传入）'
}
$OutDir = (Resolve-Path -LiteralPath $ExeDir).Path
$exePath = Join-Path $OutDir '海中鱼巣.exe'
if (-not (Test-Path -LiteralPath $exePath)) {
    throw "未找到正式入口: $exePath"
}

$sessionUrlFile = 'D:\TEMP\海中鱼巣\last-session-url.txt'
if (-not (Test-Path -LiteralPath 'D:\TEMP\海中鱼巣')) {
    throw '缺少 D:\TEMP\海中鱼巣。workflow 必须先准备 D: 合同路径。'
}
if (Test-Path -LiteralPath $sessionUrlFile) {
    Remove-Item -LiteralPath $sessionUrlFile -Force
}

# 用例计划（固定进证据，供中文报告引用）
$casePlan = [ordered]@{
    caseId = 'GB3-E2E-01'
    headHint = '基于 PR17/GB-2 已绿正式入口'
    seedBaseline = '默认 PostBuild 初始化材料 + 基础数据\词典；不另改种子'
    serviceObjectLogin = $ServiceObjectLogin
    positiveNl = $ProbeNl
    negativeNl = $NegativeNl
    selectionRationale = @(
        '完整目标 H 需要：需求主体、目标信息节点、目标特征类型、以及值目标的值域/比较或方向目标的方向（3100）。',
        '需求主体 ≠ 目标存在：本句主体为会话绑定服务对象（登录名 zch2005 / 第一权限人），目标信息节点为其人，目标特征为姓名，目标值为「周春海」。',
        '选择该句是因为装配默认第一权限人登录名/姓名即 zch2005/周春海，属种子世界已有可指对象，不注入新节点；是否被 NL 理解成正式被认知需求仍属观测对象。',
        '负例用于区分「无法理解/拒绝」与缺陷：期望正常追问或明确未理解，而不是伪装成已满足。'
    )
    expectedChain = @(
        '真实页面 DOM 提交 NL',
        '外设接收消息 / 接收认知推进',
        '解析或澄清完整目标 H（存在/特征类型/值域或方向）',
        '形成被认知需求或待完善需求',
        '服务对象需求→自我执行需求/任务筹办（若规范路径允许）',
        '查找方法→执行→满足判定→结算/反馈'
    )
    observableAssertions = [ordered]@{
        A1_pageSubmit = '页面 #input 提交后输入清空；#messages 出现用户侧文本'
        A2_binding = '页面 service-object 或控制快照可观测到会话绑定服务对象（非空登录绑定）'
        A3_receive = '运行.log 或控制快照 receivedDemandCognition / 消息承接证据与本次输入关联'
        A4_demand = '控制快照 demands.items 出现与本次目标相关的需求（完整或待完善）；否则记未证实'
        A5_task = '控制快照 tasks.items 出现承接任务；否则记未证实'
        A6_method = '方法目录/执行证据/学习工作快照显示查找或执行；否则记未证实'
        A7_satisfy = '需求活动/满足相关字段显示已满足且有反馈；否则记未证实/失败'
        A8_not202 = '不得将 HTTP 入队或单条完成日志升级为全闭环'
    }
    observeMs = $ObserveMs
    driver = 'playwright-chromium DOM (#input form submit)'
}
$casePlan | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath (Join-Path $EvidenceDir 'case-plan.json') -Encoding utf8

$ownedProcess = $null
$sessionUrl = $null
$pageExit = $null
$conclusion = '未完成'
$closureJudgement = '未判定'

try {
    Write-Step '核对 exe 旁种子'
    foreach ($rel in @('初始化材料', '基础数据\词典')) {
        $p = Join-Path $OutDir $rel
        if (-not (Test-Path -LiteralPath $p)) { throw "缺失: $p" }
        Write-Host "OK: $p"
    }

    Write-Step ("启动正式入口（绑定服务对象 login={0}）" -f $ServiceObjectLogin)
    $arg = "--web-service-object-login=$ServiceObjectLogin"
    $ownedProcess = Start-Process -FilePath $exePath -WorkingDirectory $OutDir `
        -ArgumentList $arg -PassThru -WindowStyle Hidden
    Write-Host ("已启动 ProcId={0} args={1}" -f $ownedProcess.Id, $arg)

    Write-Step '等待 session URL'
    $deadline = (Get-Date).AddSeconds($StartupTimeoutSec)
    while ((Get-Date) -lt $deadline) {
        if ($ownedProcess.HasExited) {
            throw ("正式入口过早退出 ExitCode={0}" -f $ownedProcess.ExitCode)
        }
        if (Test-Path -LiteralPath $sessionUrlFile) {
            $raw = Get-Content -LiteralPath $sessionUrlFile -Raw -ErrorAction SilentlyContinue
            if ($null -ne $raw) {
                foreach ($line in ($raw -split "`r?`n")) {
                    $candidate = $line.Trim()
                    if (Test-ExactSessionUrl $candidate) {
                        $sessionUrl = $candidate.TrimEnd('/')
                        Copy-Evidence -Source $sessionUrlFile -DestinationDir $EvidenceDir -Name 'last-session-url.txt'
                        break
                    }
                }
            }
        }
        if ($sessionUrl) { break }
        Start-Sleep -Milliseconds 500
    }
    if (-not $sessionUrl) {
        throw ("等待 session URL 超时: {0}" -f $sessionUrlFile)
    }
    Write-Host ("session URL: {0}" -f $sessionUrl)

    Write-Step '安装 Playwright Chromium（临时目录，不污染仓库）'
    $pwRoot = New-Directory (Join-Path $baseTemp 'hy-ego-playwright')
    Push-Location $pwRoot
    try {
        if (-not (Test-Path -LiteralPath (Join-Path $pwRoot 'package.json'))) {
            npm init -y | Out-Null
        }
        npm install --no-fund --no-audit playwright@1.49.1
        if ($LASTEXITCODE -ne 0) { throw "npm install playwright 失败: $LASTEXITCODE" }
        npx playwright install chromium
        if ($LASTEXITCODE -ne 0) { throw "playwright install chromium 失败: $LASTEXITCODE" }
    } finally {
        Pop-Location
    }

    Write-Step '真实页面 DOM 驱动（正向+负例观察）'
    $env:SESSION_URL = $sessionUrl
    $env:EVIDENCE_DIR = $PageEvidenceDir
    $env:PROBE_NL = $ProbeNl
    $env:NEGATIVE_NL = $NegativeNl
    $env:OBSERVE_MS = [string]$ObserveMs
    $env:CASE_ID = 'GB3-E2E-01'
    $driverSrc = Join-Path $RepoRoot 'tools\ci\governance-closure-page.mjs'
    $driverLocal = Join-Path $pwRoot 'governance-closure-page.mjs'
    Copy-Item -LiteralPath $driverSrc -Destination $driverLocal -Force
    Push-Location $pwRoot
    try {
        # 在已安装 playwright 的临时包目录运行，保证 ESM 能解析 'playwright'
        node .\governance-closure-page.mjs
        $pageExit = $LASTEXITCODE
    } finally {
        Pop-Location
    }
    if ($pageExit -ne 0) {
        throw ("页面驱动失败 exit={0}" -f $pageExit)
    }

    Write-Step '等待进程自行退出（close 后）'
    $exitDeadline = (Get-Date).AddSeconds(30)
    while (-not $ownedProcess.HasExited -and (Get-Date) -lt $exitDeadline) {
        Start-Sleep -Milliseconds 200
    }
    if ($ownedProcess.HasExited) {
        Write-Host ("进程已退出 ProcId={0} ExitCode={1}" -f $ownedProcess.Id, $ownedProcess.ExitCode)
    } else {
        Write-Host '进程未在时限内自行退出；将在 finally 清理（记入问题，不伪造成功退出）'
    }

    $closureJudgement = '需人工/报告侧依据 page 证据与运行日志判定（脚本不自动升格为全闭环）'
    $conclusion = '页面驱动与取证完成（闭环判定见中文测试记录）'
} catch {
    $conclusion = '失败'
    $closureJudgement = '失败/中断'
    Write-Host ("错误: {0}" -f $_.Exception.Message) -ForegroundColor Red
    throw
} finally {
    Write-Step '收集证据并清理'
    Copy-Evidence -Source $sessionUrlFile -DestinationDir $EvidenceDir -Name 'last-session-url.txt'
    if (Test-Path -LiteralPath 'D:\海中鱼巣\日志') {
        $logCopy = New-Directory (Join-Path $EvidenceDir 'logs')
        Copy-Item -Path 'D:\海中鱼巣\日志\*' -Destination $logCopy -Recurse -Force -ErrorAction SilentlyContinue
    }
    $summary = @"
结论: $conclusion
闭环脚本判定占位: $closureJudgement
HEAD_HINT: 由 workflow 注入
sessionUrl: $sessionUrl
ProbeNl: $ProbeNl
NegativeNl: $NegativeNl
ServiceObjectLogin: $ServiceObjectLogin
ObserveMs: $ObserveMs
PageDriverExit: $pageExit
ProcExited: $(if ($ownedProcess) { $ownedProcess.HasExited } else { 'n/a' })
ExitCode: $(if ($ownedProcess -and $ownedProcess.HasExited) { $ownedProcess.ExitCode } else { 'n/a' })
证据目录: $EvidenceDir
说明: 本摘要不把 202/页面回显升格为治理全闭环；以 case-plan 断言与日志/快照交叉核验为准。
"@
    Set-Content -LiteralPath $SummaryPath -Value $summary -Encoding utf8
    Write-Host $summary
    Stop-ProcessTree -Process $ownedProcess
}
