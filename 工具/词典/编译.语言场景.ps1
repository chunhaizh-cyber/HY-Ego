param([ValidateSet('Build','ClCompile','_Link')][string]$Target = 'Build', [string]$SelectedFile = '', [string]$MSBuild = 'C:/Program Files/Microsoft Visual Studio/18/Community/MSBuild/Current/Bin/MSBuild.exe')
$ErrorActionPreference = 'Stop'
$taskRoot = (Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
Push-Location -LiteralPath $taskRoot
try {
    $extraArgs = @("/t:$Target")
    if ($SelectedFile) { $SelectedFile = [IO.Path]::GetFullPath($SelectedFile).Replace('/', '\'); $extraArgs += "/p:SelectedFiles=$SelectedFile" }
    & $MSBuild '海中鱼巣.vcxproj' /m:4 /p:Configuration=Release /p:Platform=x64 /p:WindowsTargetPlatformVersion=10.0.26100.0 /p:_LatestWindowsTargetPlatformVersion=10.0.26100.0 '/p:TargetPlatformSdkPath=C:/Program Files (x86)/Windows Kits/10/' /p:TargetPlatformDisplayName=Windows10 /p:TrackFileAccess=false /p:ForceRebuild=true /p:EnableD455RealSense=false /p:OutDir=x64/DICT-LANG-SCENE/bin/ /p:IntDir=x64/DICT-LANG-SCENE/obj/ "/p:CustomAfterMicrosoftCommonTargets=$taskRoot/工具/词典/验证.语言场景.targets" /fl '/flp:logfile=x64/DICT-LANG-SCENE/build.log;verbosity=minimal' /v:minimal @extraArgs
    exit $LASTEXITCODE
} finally { Pop-Location }
