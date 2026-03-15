[CmdletBinding()]
param(
    [string]$Solution = '海鱼.sln',
    [string]$Configuration = 'Debug',
    [string]$Platform = 'x64',
    [string[]]$ExtraArgs = @()
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function Get-MSBuildPath {
    $vswhere = 'C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe'
    if (Test-Path $vswhere) {
        $resolved = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -find 'MSBuild\**\Bin\MSBuild.exe' 2>$null |
            Select-Object -First 1
        if ($resolved -and (Test-Path $resolved)) { return $resolved }
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

    $cmd = Get-Command MSBuild.exe -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Source }

    throw 'MSBuild.exe not found. Please install Visual Studio 2022 or Build Tools.'
}

$repoRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$solutionPath = Join-Path $repoRoot $Solution
if (-not (Test-Path $solutionPath)) {
    throw "Solution not found: $solutionPath"
}

# Build convention: always clean temp workspace before checks/build.
$tempDir = Join-Path $repoRoot 'temp'
if (Test-Path $tempDir) {
    Get-ChildItem -Path $tempDir -Force -ErrorAction SilentlyContinue |
        Remove-Item -Recurse -Force -ErrorAction SilentlyContinue
} else {
    $null = New-Item -Path $tempDir -ItemType Directory -Force
}

# Enforce module-layer rules before build to avoid hidden dependency regressions.
$layerCheckScript = Join-Path $repoRoot 'tools\check_module_layers.ps1'
if (Test-Path $layerCheckScript) {
    Write-Host 'Checking module layers...'
    & powershell -NoProfile -ExecutionPolicy Bypass -File $layerCheckScript -RepoRoot $repoRoot
    if ($LASTEXITCODE -ne 0) {
        throw 'Module layer check failed. See temp\module_layer_violations.csv'
    }
}

$msbuild = Get-MSBuildPath

$psi = New-Object System.Diagnostics.ProcessStartInfo
$psi.UseShellExecute = $false
$psi.FileName = $msbuild
$psi.WorkingDirectory = $repoRoot
$psi.RedirectStandardOutput = $true
$psi.RedirectStandardError = $true
$psi.StandardOutputEncoding = [System.Text.Encoding]::UTF8
$psi.StandardErrorEncoding = [System.Text.Encoding]::UTF8

# Codex/PowerShell can inject both Path and PATH into the parent process.
# MSBuild then forwards both to tool tasks, which makes MIDL/CL fail before compilation.
try { $psi.EnvironmentVariables.Remove('PATH') } catch {}

$arguments = @(
    ('"{0}"' -f $solutionPath),
    '/t:Build',
    "/p:Configuration=$Configuration",
    "/p:Platform=$Platform"
) + $ExtraArgs
$psi.Arguments = [string]::Join(' ', $arguments)

Write-Host "Using MSBuild: $msbuild"
Write-Host "Building: $Solution ($Configuration|$Platform)"

$process = New-Object System.Diagnostics.Process
$process.StartInfo = $psi
$null = $process.Start()

$stdout = $process.StandardOutput.ReadToEnd()
$stderr = $process.StandardError.ReadToEnd()
$process.WaitForExit()

if ($stdout) { Write-Host $stdout }
if ($stderr) { Write-Host $stderr }

exit $process.ExitCode
