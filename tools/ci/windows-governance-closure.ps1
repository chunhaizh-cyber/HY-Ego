#Requires -Version 5.1
<#
.SYNOPSIS
  GB-3 R2：Windows 正式入口 + 真实网页 DOM 驱动的需求获取/理解/满足闭环观测。

.DESCRIPTION
  R2：干净种子启动 → 有界等待自我当前交互者正式证据 → 只投正句 → 按 messageId 长观察。
  不声称 202 即闭环；不注入需求节点；不改业务语义。页面 service-object 不算交互者正式读回。

.NOTES
  启动参数：--web-service-object-login=zch2005（与装配默认第一权限人登录名一致）。
  用例计划见 evidence/case-plan.json。
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
    [int]$ObserveMs = 900000,

    [Parameter(Mandatory = $false)]
    [int]$InteractantWaitMs = 600000,

    [Parameter(Mandatory = $false)]
    [string]$ProbeNl = '请把登录名为zch2005的人的姓名保持为周春海',

    [Parameter(Mandatory = $false)]
    [string]$ServiceObjectLogin = 'zch2005',

    [Parameter(Mandatory = $false)]
    [string]$RoundId = 'gb3-r2',

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
    caseId = 'GB3-E2E-01-R2'
    roundId = $RoundId
    businessBaselineHead = 'b167bd0deb5333a4bd1496a8b9dea0d9b68b1756'
    noteOnHead = '业务源码基线保持 b167bd0d；本轮仅允许 CI/观测脚本差异，产品断言不得因脚本绿而升格。'
    seedBaseline = '默认 PostBuild 初始化材料 + 基础数据\词典；不另改种子'
    serviceObjectLogin = $ServiceObjectLogin
    probeNl = $ProbeNl
    negativeNl = $null
    protocol = @(
        '干净正式种子启动并打开真实网页',
        '有界等待自我当前交互者正式证据（cognition.speaker 或自我出站等）；页面 service-object 不算正式读回',
        '前置未达成则明确记录，不假装已满足',
        '再单独 DOM 提交正句；本轮不投乱码',
        '按 receivedDemandCognition.message 贯穿追踪；文件名 phase 仅采集阶段'
    )
    selectionRationale = @(
        '完整目标 H 需要：需求主体、目标信息节点、目标特征类型、以及值目标的值域/比较或方向（3100）。',
        '种子下 zch2005 姓名已是周春海：正例验收识别人/理解目标/正式读回当前值/避免无意义动作；已满足不要求改名，不算本次服务贡献（5260/6190）。',
        '区分三类：交互者取得耗时；句子未解释成受支持角色结构；前置治理后无人接续。',
        '方法 24038 模拟学习异常继续记录是否重现；无证据链不归因。'
    )
    expectedChain = @(
        '交互者正式证据（或有界前置失败证据）',
        '真实页面 DOM 提交正句',
        '外设接收 / 接收认知推进（按 messageId）',
        '对象绑定/构式/角色/正式含义或前置治理分支',
        '若已满足：读回当前值与零动作；不强制改名结算',
        '页面反馈或有界超时稳定结论'
    )
    observableAssertions = [ordered]@{
        A1_pageSubmit = '页面 #input 提交后输入清空；#messages 出现用户侧文本'
        A2_interactant = '提交前 interactant-gate.json 标明正式证据是否达成；不得仅用页面 service-object 充数'
        A3_receive = '控制快照 receivedDemandCognition.message 与本次正句关联'
        A4_demand = '同 message 下 demand/meaning 出现与本句目标相关材料；否则记未证实'
        A5_task = '同 message 关联任务承接；内部自我域 D/T 不得冒充本句姓名任务'
        A6_method = '方法/执行证据；否则未证实'
        A7_satisfy = '已满足时允许零动作；改名结算非本用例默认目标'
        A8_not202 = '不得将 HTTP 入队升格为全闭环'
        A9_classes = '报告须区分：交互者耗时 / 未支持角色结构 / 前置治理无人接续'
    }
    observeMs = $ObserveMs
    interactantWaitMs = $InteractantWaitMs
    driver = 'playwright-chromium DOM (#input form submit) R2'
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

    Write-Step '真实页面 DOM 驱动（R2：交互者前置 + 只投正句）'
    $env:SESSION_URL = $sessionUrl
    $env:EVIDENCE_DIR = $PageEvidenceDir
    $env:PROBE_NL = $ProbeNl
    $env:OBSERVE_MS = [string]$ObserveMs
    $env:INTERACTANT_WAIT_MS = [string]$InteractantWaitMs
    $env:CASE_ID = 'GB3-E2E-01-R2'
    $env:ROUND_ID = $RoundId
    if ($env:NEGATIVE_NL) { Remove-Item Env:\NEGATIVE_NL -ErrorAction SilentlyContinue }
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
RoundId: $RoundId
sessionUrl: $sessionUrl
ProbeNl: $ProbeNl
NegativeNl: (本轮禁用)
ServiceObjectLogin: $ServiceObjectLogin
ObserveMs: $ObserveMs
InteractantWaitMs: $InteractantWaitMs
PageDriverExit: $pageExit
ProcExited: $(if ($ownedProcess) { $ownedProcess.HasExited } else { 'n/a' })
ExitCode: $(if ($ownedProcess -and $ownedProcess.HasExited) { $ownedProcess.ExitCode } else { 'n/a' })
证据目录: $EvidenceDir
说明: CI 脚本绿只说明取证完成；产品断言见中文验证记录。不把 202/页面回显升格为治理全闭环。
"@
    Set-Content -LiteralPath $SummaryPath -Value $summary -Encoding utf8
    Write-Host $summary
    Stop-ProcessTree -Process $ownedProcess
}
