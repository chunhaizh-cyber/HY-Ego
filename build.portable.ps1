[CmdletBinding()]
param(
    [string]$Solution = '',
    [string]$Configuration = 'Debug',
    [string]$Platform = 'x64',
    [string]$MSBuildPath = '',
    [string]$VsInstallPath = '',
    [switch]$SkipLayerCheck,
    [switch]$KeepTemp,
    [string[]]$ExtraArgs = @()
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function Set-ChildEnvironmentValue {
    param(
        [System.Diagnostics.ProcessStartInfo]$ProcessStartInfo,
        [string]$Name,
        [AllowEmptyString()]
        [string]$Value
    )

    try { $ProcessStartInfo.EnvironmentVariables.Remove($Name) } catch {}
    if (-not [string]::IsNullOrWhiteSpace($Value)) {
        $ProcessStartInfo.EnvironmentVariables[$Name] = $Value
    }
}

function Get-VsWherePath {
    $candidates = @(
        'C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe',
        'D:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe'
    )

    foreach ($candidate in $candidates) {
        if (Test-Path $candidate) { return $candidate }
    }

    return $null
}

function Resolve-VsInstallPath {
    param([string]$PreferredPath)

    if ($PreferredPath) {
        $resolved = Resolve-Path -Path $PreferredPath -ErrorAction Stop
        return $resolved.Path
    }

    $vswhere = Get-VsWherePath
    if ($vswhere) {
        $nativeDesktop = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Workload.NativeDesktop -property installationPath 2>$null |
            Select-Object -First 1
        if ($nativeDesktop) { return $nativeDesktop }

        $msbuildOnly = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -property installationPath 2>$null |
            Select-Object -First 1
        if ($msbuildOnly) { return $msbuildOnly }
    }

    $candidates = @(
        'C:\Program Files\Microsoft Visual Studio\2022\Community',
        'C:\Program Files\Microsoft Visual Studio\2022\Professional',
        'C:\Program Files\Microsoft Visual Studio\2022\Enterprise',
        'C:\Program Files\Microsoft Visual Studio\2022\BuildTools',
        'D:\Program Files\Microsoft Visual Studio\2022\Community',
        'D:\Program Files\Microsoft Visual Studio\2022\Professional',
        'D:\Program Files\Microsoft Visual Studio\2022\Enterprise',
        'D:\Program Files\Microsoft Visual Studio\2022\BuildTools'
    )

    foreach ($candidate in $candidates) {
        if (Test-Path $candidate) { return $candidate }
    }

    return $null
}

function Get-MSBuildPath {
    param(
        [string]$PreferredPath,
        [string]$PreferredVsInstallPath
    )

    if ($PreferredPath) {
        $resolved = Resolve-Path -Path $PreferredPath -ErrorAction Stop
        return $resolved.Path
    }

    $vswhere = Get-VsWherePath
    if ($vswhere) {
        $resolved = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -find 'MSBuild\**\Bin\MSBuild.exe' 2>$null |
            Select-Object -First 1
        if ($resolved -and (Test-Path $resolved)) { return $resolved }
    }

    if ($PreferredVsInstallPath) {
        $candidate = Join-Path $PreferredVsInstallPath 'MSBuild\Current\Bin\MSBuild.exe'
        if (Test-Path $candidate) { return $candidate }
    }

    $candidates = @(
        'C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe',
        'C:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe',
        'C:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe',
        'C:\Program Files\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe',
        'D:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe',
        'D:\Program Files\Microsoft Visual Studio\2022\Professional\MSBuild\Current\Bin\MSBuild.exe',
        'D:\Program Files\Microsoft Visual Studio\2022\Enterprise\MSBuild\Current\Bin\MSBuild.exe',
        'D:\Program Files\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe'
    )

    foreach ($candidate in $candidates) {
        if (Test-Path $candidate) { return $candidate }
    }

    throw 'MSBuild.exe not found. Pass -MSBuildPath or install Visual Studio 2022 / Build Tools with Desktop C++.'
}

function Initialize-ChildBuildEnvironment {
    param([System.Diagnostics.ProcessStartInfo]$ProcessStartInfo)

    # Keep a single Path entry to avoid tool task failures under PowerShell/Codex.
    try { $ProcessStartInfo.EnvironmentVariables.Remove('PATH') } catch {}
    try { $ProcessStartInfo.EnvironmentVariables.Remove('Path') } catch {}

    $normalizedPath = if ($env:Path) { $env:Path } elseif ($env:PATH) { $env:PATH } else { '' }
    Set-ChildEnvironmentValue -ProcessStartInfo $ProcessStartInfo -Name 'Path' -Value $normalizedPath

    foreach ($name in @('TEMP', 'TMP', 'USERPROFILE', 'APPDATA', 'LOCALAPPDATA', 'ProgramData', 'ALLUSERSPROFILE', 'HOMEDRIVE', 'HOMEPATH')) {
        Set-ChildEnvironmentValue -ProcessStartInfo $ProcessStartInfo -Name $name -Value ([Environment]::GetEnvironmentVariable($name, 'Process'))
    }

    Set-ChildEnvironmentValue -ProcessStartInfo $ProcessStartInfo -Name 'VSCMD_SKIP_SENDTELEMETRY' -Value '1'
}

$repoRoot = Split-Path -Parent $MyInvocation.MyCommand.Path

if ($Solution) {
    $solutionPath = Join-Path $repoRoot $Solution
}
else {
    $solutionCandidates = @(Get-ChildItem -Path $repoRoot -Filter *.sln -File | Sort-Object Name)
    if ($solutionCandidates.Count -eq 1) {
        $solutionPath = $solutionCandidates[0].FullName
    }
    elseif ($solutionCandidates.Count -eq 0) {
        throw "No solution file found under: $repoRoot"
    }
    else {
        $candidateNames = ($solutionCandidates | ForEach-Object { $_.Name }) -join ', '
        throw "Multiple solution files found. Pass -Solution explicitly. Candidates: $candidateNames"
    }
}

if (-not (Test-Path $solutionPath)) {
    throw "Solution not found: $solutionPath"
}

$solutionDisplayName = Split-Path -Leaf $solutionPath

$tempDir = Join-Path $repoRoot 'temp'
if (-not $KeepTemp) {
    if (Test-Path $tempDir) {
        Get-ChildItem -Path $tempDir -Force -ErrorAction SilentlyContinue |
            Remove-Item -Recurse -Force -ErrorAction SilentlyContinue
    } else {
        $null = New-Item -Path $tempDir -ItemType Directory -Force
    }
}

if (-not $SkipLayerCheck) {
    $layerCheckScript = Join-Path $repoRoot 'tools\check_module_layers.ps1'
    if (Test-Path $layerCheckScript) {
        Write-Host 'Checking module layers...'
        & powershell -NoProfile -ExecutionPolicy Bypass -File $layerCheckScript -RepoRoot $repoRoot
        if ($LASTEXITCODE -ne 0) {
            throw 'Module layer check failed. See temp\module_layer_violations.csv'
        }
    }
}

$resolvedVsInstallPath = Resolve-VsInstallPath -PreferredPath $VsInstallPath
$msbuild = Get-MSBuildPath -PreferredPath $MSBuildPath -PreferredVsInstallPath $resolvedVsInstallPath

$psi = New-Object System.Diagnostics.ProcessStartInfo
$psi.UseShellExecute = $false
$psi.FileName = $msbuild
$psi.WorkingDirectory = $repoRoot
$psi.RedirectStandardOutput = $true
$psi.RedirectStandardError = $true
$psi.StandardOutputEncoding = [System.Text.Encoding]::UTF8
$psi.StandardErrorEncoding = [System.Text.Encoding]::UTF8

Initialize-ChildBuildEnvironment -ProcessStartInfo $psi

$arguments = @(
    ('"{0}"' -f $solutionPath),
    '/t:Build',
    "/p:Configuration=$Configuration",
    "/p:Platform=$Platform"
) + $ExtraArgs
$psi.Arguments = [string]::Join(' ', $arguments)

Write-Host "Using Visual Studio: $resolvedVsInstallPath"
Write-Host "Using MSBuild: $msbuild"
Write-Host "Building: $solutionDisplayName ($Configuration|$Platform)"

$process = New-Object System.Diagnostics.Process
$process.StartInfo = $psi
$null = $process.Start()

$stdout = $process.StandardOutput.ReadToEnd()
$stderr = $process.StandardError.ReadToEnd()
$process.WaitForExit()

if ($stdout) { Write-Host $stdout }
if ($stderr) { Write-Host $stderr }

exit $process.ExitCode
