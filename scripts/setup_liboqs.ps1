param()
$ErrorActionPreference = 'Stop'
$Root = Split-Path -Parent $PSScriptRoot
$Third = Join-Path $Root 'third_party'
New-Item -ItemType Directory -Path $Third -Force | Out-Null
Set-Location $Third
if (-Not (Test-Path liboqs)) {
  Write-Host "Cloning liboqs..."
  git clone https://github.com/open-quantum-safe/liboqs.git
} else {
  Write-Host "liboqs already present, skipping clone."
}
Set-Location liboqs
if (-Not (Test-Path build)) { New-Item -ItemType Directory build | Out-Null }
Set-Location build
cmake -DCMAKE_BUILD_TYPE=Release -DOQS_USE_OPENSSL=OFF -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX="$Third/liboqs/install" ..
cmake --build . --config Release
cmake --install . --config Release --prefix "$Third/liboqs/install"
Write-Host "liboqs built and installed to $Third/liboqs/install"
Write-Host "To install Python bindings: pip install git+https://github.com/open-quantum-safe/liboqs-python"
