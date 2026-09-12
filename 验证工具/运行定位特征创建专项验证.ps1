param(
    [string]$OutputRoot = 'D:/TEMP/海中鱼巣/FEATURE-PARENT-BOUND-CREATION/01a07099-20260912-code',
    [ValidateSet('Debug','Release','Both')][string]$Configuration = 'Both'
)
$ErrorActionPreference = 'Stop'
$repoRoot = Split-Path $PSScriptRoot -Parent
$project = Join-Path $PSScriptRoot '定位特征创建专项验证.vcxproj'
$msbuild = 'C:/Program Files/Microsoft Visual Studio/18/Professional/MSBuild/Current/Bin/MSBuild.exe'
$resolvedOutput = [IO.Path]::GetFullPath($OutputRoot).TrimEnd('\','/')
$allowedRoot = [IO.Path]::GetFullPath('D:/TEMP/海中鱼巣/FEATURE-PARENT-BOUND-CREATION').TrimEnd('\','/')
if (-not $resolvedOutput.StartsWith($allowedRoot + [IO.Path]::DirectorySeparatorChar,[StringComparison]::OrdinalIgnoreCase)) { throw 'Unexpected output root' }
[void](New-Item -ItemType Directory -Path $resolvedOutput -Force)
[xml]$xml = Get-Content -LiteralPath $project -Raw
$inputPaths = @($project,$PSCommandPath)
foreach ($node in $xml.Project.ItemGroup.ClCompile) {
    if ($node.Include) { $inputPaths += [IO.Path]::GetFullPath((Join-Path $PSScriptRoot $node.Include)) }
}
# Freeze the actual local header closure as well as modules and consumer.
$queue = [Collections.Generic.Queue[string]]::new()
$seen = [Collections.Generic.HashSet[string]]::new([StringComparer]::OrdinalIgnoreCase)
foreach ($path in $inputPaths) { $queue.Enqueue($path) }
while ($queue.Count) {
    $path = $queue.Dequeue()
    if (-not $seen.Add($path)) { continue }
    foreach ($match in [regex]::Matches([IO.File]::ReadAllText($path),'(?m)^\s*#\s*include\s*"([^"]+)"')) {
        $queue.Enqueue([IO.Path]::GetFullPath((Join-Path (Split-Path $path -Parent) $match.Groups[1].Value)))
    }
}
$inputs = @($seen | Sort-Object | ForEach-Object { [pscustomobject]@{Path=$_;SHA256=(Get-FileHash -LiteralPath $_ -Algorithm SHA256).Hash} })
$inputs | ConvertTo-Json -Depth 6 | Set-Content -LiteralPath (Join-Path $resolvedOutput 'core-inputs.json') -Encoding utf8
function Confirm-Inputs {
    foreach ($item in $inputs) { if ((Get-FileHash -LiteralPath $item.Path -Algorithm SHA256).Hash -ne $item.SHA256) { throw "Input changed: $($item.Path)" } }
}
$results = @()
$configs = if ($Configuration -eq 'Both') { @('Debug','Release') } else { @($Configuration) }
foreach ($config in $configs) {
    Confirm-Inputs
    $dir = Join-Path $resolvedOutput "core-$config"
    [void](New-Item -ItemType Directory -Path $dir -Force)
    $buildLog = Join-Path $resolvedOutput "core-$config-build.log"
    $runLog = Join-Path $resolvedOutput "core-$config-run.log"
    $clock = [Diagnostics.Stopwatch]::StartNew()
    & $msbuild $project /t:Rebuild /m:1 "/p:Configuration=$config" /p:Platform=x64 "/p:OutDir=$dir/out/" "/p:IntDir=$dir/obj/" /p:TargetName=parent-bound-core /v:minimal *> $buildLog
    $buildExit = $LASTEXITCODE
    Confirm-Inputs
    $results += [pscustomobject]@{Configuration=$config;Stage='Rebuild';Exit=$buildExit;Seconds=$clock.Elapsed.TotalSeconds;Log=$buildLog}
    $results | ConvertTo-Json -Depth 6 | Set-Content -LiteralPath (Join-Path $resolvedOutput 'core-results.json') -Encoding utf8
    if ($buildExit) { throw "$config Rebuild failed: $buildExit" }
    $clock.Restart()
    & (Join-Path $dir 'out/parent-bound-core.exe') *> $runLog
    $runExit = $LASTEXITCODE
    Confirm-Inputs
    $results += [pscustomobject]@{Configuration=$config;Stage='Run';Exit=$runExit;Seconds=$clock.Elapsed.TotalSeconds;Log=$runLog;Last=(Get-Content -LiteralPath $runLog -Tail 1)}
    $results | ConvertTo-Json -Depth 6 | Set-Content -LiteralPath (Join-Path $resolvedOutput 'core-results.json') -Encoding utf8
    if ($runExit) { throw "$config run failed: $runExit" }
}
$results | Format-Table -AutoSize
