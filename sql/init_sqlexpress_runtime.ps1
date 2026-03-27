param(
    [string]$ServerInstance = ".\\SQLEXPRESS",
    [string]$SchemaFile = "$PSScriptRoot\\001_runtime_exchange_sqlexpress.sql"
)

$sqlcmd = Get-Command SQLCMD.EXE -ErrorAction Stop

if (-not (Test-Path -LiteralPath $SchemaFile)) {
    throw "未找到建库脚本: $SchemaFile"
}

Write-Host "Applying schema to $ServerInstance using Windows authentication..."
& $sqlcmd.Source -S $ServerInstance -E -b -i $SchemaFile

if ($LASTEXITCODE -ne 0) {
    throw "sqlcmd 执行失败，退出码: $LASTEXITCODE"
}

Write-Host "Schema applied successfully."
