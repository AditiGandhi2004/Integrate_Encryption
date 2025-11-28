#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
THIRD="$ROOT/third_party"
mkdir -p "$THIRD"
cd "$THIRD"
echo "Cloning liboqs (Open Quantum Safe) into $THIRD/liboqs ..."
if [ -d liboqs ]; then
  echo "liboqs directory already exists, skipping clone."
else
  git clone --depth 1 https://github.com/open-quantum-safe/liboqs.git
fi
cd liboqs
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DOQS_USE_OPENSSL=OFF -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX="$THIRD/liboqs/install" ..
cmake --build . -- -j$(nproc || echo 2)
cmake --install . --prefix "$THIRD/liboqs/install"
echo "liboqs built and installed to $THIRD/liboqs/install"
echo "To build Python bindings (oqs), run: pip install git+https://github.com/open-quantum-safe/liboqs-python"
