param(
    [Parameter(Mandatory = $true)]
    [string]$ProjectDir,

    [Parameter(Mandatory = $true)]
    [string]$OutDir,

    [Parameter(Mandatory = $true)]
    [string]$Configuration
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function Copy-FileWithRetry {
    param(
        [Parameter(Mandatory = $true)]
        [string]$SourcePath,

        [Parameter(Mandatory = $true)]
        [string]$DestinationDir
    )

    $targetPath = Join-Path $DestinationDir (Split-Path -Leaf $SourcePath)
    $sourceItem = Get-Item -LiteralPath $SourcePath
    if (Test-Path -LiteralPath $targetPath) {
        $targetItem = Get-Item -LiteralPath $targetPath
        if ($targetItem.Length -eq $sourceItem.Length -and
            $targetItem.LastWriteTimeUtc -eq $sourceItem.LastWriteTimeUtc) {
            return
        }
    }

    for ($attempt = 0; $attempt -lt 5; $attempt++) {
        try {
            Copy-Item -LiteralPath $SourcePath -Destination $DestinationDir -Force
            return
        }
        catch {
            if ($attempt -ge 4) {
                Write-Warning "Skip copy '$targetPath': $($_.Exception.Message)"
                return
            }
            Start-Sleep -Milliseconds 200
        }
    }
}

function Copy-IfExists {
    param(
        [Parameter(Mandatory = $true)]
        [string]$SourcePath,

        [Parameter(Mandatory = $true)]
        [string]$DestinationDir
    )

    if (-not (Test-Path -LiteralPath $SourcePath)) {
        return
    }

    Copy-FileWithRetry -SourcePath $SourcePath -DestinationDir $DestinationDir
}

function Copy-PatternIfExists {
    param(
        [Parameter(Mandatory = $true)]
        [string]$SourceDir,

        [Parameter(Mandatory = $true)]
        [string]$Pattern,

        [Parameter(Mandatory = $true)]
        [string]$DestinationDir
    )

    if (-not (Test-Path -LiteralPath $SourceDir)) {
        return
    }

    Get-ChildItem -Path $SourceDir -Filter $Pattern -File -ErrorAction SilentlyContinue |
        ForEach-Object {
            Copy-FileWithRetry -SourcePath $_.FullName -DestinationDir $DestinationDir
        }
}

$projectRoot = (Resolve-Path -LiteralPath $ProjectDir).Path
if (-not (Test-Path -LiteralPath $OutDir)) {
    New-Item -ItemType Directory -Path $OutDir -Force | Out-Null
}
$outputRoot = (Resolve-Path -LiteralPath $OutDir).Path

$commonProjectDlls = @(
    'gl2ps.dll',
    'glew32.dll',
    'glfw3.dll'
)

if ($Configuration -eq 'Debug') {
    Copy-PatternIfExists -SourceDir 'D:\vcpkg\installed\x64-windows\debug\bin' -Pattern 'opencv_*4d.dll' -DestinationDir $outputRoot
    # librealsense needs lz4d.dll when the pipeline is first created.
    foreach ($name in @('jsoncpp.dll', 'libcurl-d.dll', 'zlibd1.dll', 'realsense2d.dll', 'lz4d.dll')) {
        Copy-IfExists -SourcePath (Join-Path 'D:\vcpkg\installed\x64-windows\debug\bin' $name) -DestinationDir $outputRoot
    }

    foreach ($name in $commonProjectDlls + @(
        'concrt140d.dll',
        'mfc140ud.dll',
        'msvcp140d.dll',
        'msvcp140d_atomic_wait.dll',
        'ucrtbased.dll',
        'vcruntime140d.dll',
        'vcruntime140_1d.dll'
    )) {
        Copy-IfExists -SourcePath (Join-Path $projectRoot $name) -DestinationDir $outputRoot
    }
}
else {
    Copy-PatternIfExists -SourceDir 'D:\vcpkg\installed\x64-windows\bin' -Pattern 'opencv_*4.dll' -DestinationDir $outputRoot
    # librealsense needs lz4.dll when the pipeline is first created.
    foreach ($name in @('jsoncpp.dll', 'libcurl.dll', 'zlib1.dll', 'realsense2.dll', 'lz4.dll')) {
        Copy-IfExists -SourcePath (Join-Path 'D:\vcpkg\installed\x64-windows\bin' $name) -DestinationDir $outputRoot
    }

    foreach ($name in $commonProjectDlls) {
        Copy-IfExists -SourcePath (Join-Path $projectRoot $name) -DestinationDir $outputRoot
    }
}
