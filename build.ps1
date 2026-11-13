param([string]$Compiler = "", [string]$RaylibRoot = "C:\raylib")
$ErrorActionPreference = 'Stop'
Set-Location -LiteralPath $PSScriptRoot
if (!$Compiler) {
    $candidates = @('C:\mingw64ucrt\mingw64\bin\gcc.exe', 'C:\Program Files\CodeBlocks\MinGW\bin\gcc.exe', 'C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\gcc.exe')
    $Compiler = $candidates | Where-Object { Test-Path -LiteralPath $_ } | Select-Object -First 1
    if (!$Compiler) { $Compiler = (Get-Command gcc.exe -ErrorAction Stop).Source }
}
$env:PATH = (Split-Path -Parent $Compiler) + ';' + $env:PATH
New-Item -ItemType Directory -Path 'bin' -Force | Out-Null
& $Compiler -std=c99 -Wall -Wextra -Wpedantic main.c -o bin/organizer.exe -lm
if ($LASTEXITCODE -ne 0) { throw "Compilation failed (exit $LASTEXITCODE)." }
Write-Host 'Build complete: bin/organizer.exe'
