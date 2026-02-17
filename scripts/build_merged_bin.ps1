param(
  [Parameter(Mandatory=$true)][string]$SketchPath,
  [Parameter(Mandatory=$true)][string]$FQBN,
  [Parameter(Mandatory=$true)][string]$OutDir
)

$ErrorActionPreference = "Stop"
New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

# Limpia bins anteriores para evitar que el "app" agarre el merged viejo
Get-ChildItem $OutDir -Filter "*.bin" -ErrorAction SilentlyContinue | Remove-Item -Force -ErrorAction SilentlyContinue

# 1) Compila y exporta binarios (bootloader/partitions/app)
Write-Host "Compilando..."
arduino-cli compile `
  --fqbn $FQBN `
  --export-binaries `
  --output-dir $OutDir `
  $SketchPath

Write-Host "`nArchivos generados en OutDir:"
Get-ChildItem $OutDir | Format-Table Name, Length

# 2) Ubica binarios generados
# Detectar bootloader
$boot = Get-ChildItem $OutDir -Filter "*boot*.bin" | Select-Object -First 1
# Detectar partitions
$part = Get-ChildItem $OutDir -Filter "*.partitions*.bin" | Select-Object -First 1

# Detectar app, el .bin del sketch (excluye boot/partitions/merged)
$app = Get-ChildItem $OutDir -Filter "*.bin" |
  Where-Object { $_.Name -notmatch "boot|partit|with_bootloader|merged" } |
  Sort-Object Length -Descending |
  Select-Object -First 1

if (-not $boot -or -not $part -or -not $app) {
  Write-Host "`nNo se encontraron los binarios esperados. Archivos en OutDir:"
  Get-ChildItem $OutDir | Format-Table Name, Length
  throw "Faltan bootloader/partitions/app bin."
}

Write-Host "`nBootloader: $($boot.Name)"
Write-Host "Partitions: $($part.Name)"
Write-Host "App:        $($app.Name)"

# 3) Offsets típicos Arduino-ESP32 (ESP32-S3)
#    (si tu board usa otros, aquí se ajusta)
$BOOT_OFFSET = "0x0000"
$PART_OFFSET = "0x8000"
$APP_OFFSET  = "0x10000"

# =========================
# Extraer versión desde el .ino
# Busca: #define firmware "X.X"
# =========================
$inoText = Get-Content $SketchPath -Raw
if ($inoText -match '#define\s+firmware\s+"([^"]+)"') {
    $autoVersion = $matches[1]
} else {
    throw "No se encontró #define firmware `"X.X`" en el sketch."
}
Write-Host "Versión detectada desde firmware: $autoVersion"

# Obtener nombre base del proyecto para el bin final
$Project_BaseName = "MOVIL_VAP"

$dest = Join-Path $OutDir ("{0}_v{1}_with_bootloader.bin" -f $Project_BaseName, $autoVersion)

Write-Host "`nGenerando merged bin..."
# 4) Merge en un solo bin
esptool --chip esp32s3 merge-bin `
  -o $dest `
  $BOOT_OFFSET $boot.FullName `
  $PART_OFFSET $part.FullName `
  $APP_OFFSET  $app.FullName

Write-Host "`nOK DONE -> $dest"
