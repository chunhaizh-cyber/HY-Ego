[CmdletBinding()]
param(
    [string]$构建目录,
    [string]$LLVM根目录 = 'C:\Program Files\LLVM',
    [switch]$重新配置
)

$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot '环境.公共.ps1')
[void](导入-VisualStudio开发环境 'x64')

if (-not $构建目录) {
    $构建目录 = Join-Path ([System.IO.Path]::GetTempPath()) '海中鱼巣-clang-ast-build'
}
$构建目录 = [System.IO.Path]::GetFullPath($构建目录)
$源码目录 = [System.IO.Path]::GetFullPath($PSScriptRoot)
$clangCl = Join-Path $LLVM根目录 'bin\clang-cl.exe'
if (-not (Test-Path -LiteralPath $clangCl -PathType Leaf)) {
    throw "找不到 clang-cl: $clangCl"
}
$env:Path = (Join-Path $LLVM根目录 'bin') + ';' + $env:Path

if ($重新配置 -and (Test-Path -LiteralPath $构建目录 -PathType Container)) {
    $临时根 = [System.IO.Path]::GetFullPath([System.IO.Path]::GetTempPath()).
        TrimEnd('\') + '\'
    if (-not $构建目录.StartsWith(
        $临时根,
        [System.StringComparison]::OrdinalIgnoreCase)) {
        throw "只允许自动清理临时目录下的工具构建目录: $构建目录"
    }
    Remove-Item -LiteralPath $构建目录 -Recurse -Force
}

& cmake.exe `
    -S $源码目录 `
    -B $构建目录 `
    -G Ninja `
    "-DCMAKE_CXX_COMPILER=$clangCl" `
    "-DLLVM_ROOT=$LLVM根目录"
if ($LASTEXITCODE -ne 0) {
    throw "CMake配置失败，退出码: $LASTEXITCODE"
}
& cmake.exe --build $构建目录
if ($LASTEXITCODE -ne 0) {
    throw "工具构建失败，退出码: $LASTEXITCODE"
}

$工具路径 = Join-Path $构建目录 '提取_候选流程图.exe'
if (-not (Test-Path -LiteralPath $工具路径 -PathType Leaf)) {
    throw "构建结束但找不到工具: $工具路径"
}
Write-Host "候选流程图工具: $工具路径"
