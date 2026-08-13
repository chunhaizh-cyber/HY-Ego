[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [ValidateSet('Debug', 'Release')]
    [string]$配置,
    [Parameter(Mandatory = $true)]
    [UInt64]$起始幂等身份
)
$ErrorActionPreference = 'Stop'
$项目路径 = Join-Path $PSScriptRoot 'L2场景身份来源提供者迁移参数验证.vcxproj'
$本轮目录 = Join-Path ([System.IO.Path]::GetTempPath()) ('海中鱼巣_SID_' + [System.Guid]::NewGuid().ToString('N'))
$输出目录 = Join-Path $本轮目录 '输出'
$中间目录 = Join-Path $本轮目录 '中间'
$严格UTF8 = [System.Text.UTF8Encoding]::new($false, $true)
$校验路径 = @(
    (Join-Path $PSScriptRoot '..\海中鱼巣\领域\L2场景结构.数据.h'),
    (Join-Path $PSScriptRoot '..\海中鱼巣\领域\服务.L2场景结构.ixx'),
    (Join-Path $PSScriptRoot '..\海中鱼巣\装配.普通应用.ixx'),
    (Join-Path $PSScriptRoot 'L2场景身份来源提供者迁移参数验证.cpp'),
    $项目路径,
    $PSCommandPath,
    (Join-Path $PSScriptRoot '..\施工记录\20260812_L2-STRUCTURE-CRUD-REBUILD-S-ID_场景身份来源提供者迁移施工记录.md'),
    (Join-Path $PSScriptRoot '..\验证记录\20260812_L2-STRUCTURE-CRUD-REBUILD-S-ID_场景身份来源提供者迁移验证记录.md')
)
try {
    foreach ($路径 in $校验路径) {
        if (-not (Test-Path -LiteralPath $路径 -PathType Leaf)) {
            throw "S-ID 允许路径缺失：$路径"
        }
        [void]$严格UTF8.GetString([System.IO.File]::ReadAllBytes($路径))
    }
    [xml](Get-Content -LiteralPath $项目路径 -Raw -Encoding UTF8) | Out-Null
    $系统临时根 = [System.IO.Path]::GetFullPath([System.IO.Path]::GetTempPath())
    $本轮完整路径 = [System.IO.Path]::GetFullPath($本轮目录)
    if (-not $本轮完整路径.StartsWith($系统临时根, [System.StringComparison]::OrdinalIgnoreCase)) {
        throw "S-ID 本轮输出没有隔离在系统临时目录：$本轮完整路径"
    }
    [System.IO.Directory]::CreateDirectory($输出目录) | Out-Null
    [System.IO.Directory]::CreateDirectory($中间目录) | Out-Null
    $构建工具 = Get-Command 'MSBuild.exe' -ErrorAction Stop
    & $构建工具.Source $项目路径 '/m:1' '/t:Rebuild' "/p:Configuration=$配置" '/p:Platform=x64' "/p:OutDir=$输出目录\" "/p:IntDir=$中间目录\" '/verbosity:minimal'
    if ($LASTEXITCODE -ne 0) { throw "S-ID 专项工程构建失败：$LASTEXITCODE" }
    $程序路径 = Join-Path $输出目录 'L2场景身份来源提供者迁移参数验证.exe'
    & $程序路径 '--起始幂等身份' $起始幂等身份
    if ($LASTEXITCODE -ne 0) { throw "S-ID 专项验证失败：$LASTEXITCODE" }
}
finally {
    if (Test-Path -LiteralPath $本轮目录) {
        Remove-Item -LiteralPath $本轮目录 -Recurse -Force
    }
}
