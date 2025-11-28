README — liboqs & NIST integration (auto-generated)

Add post-quantum cryptography (PQC) support using liboqs (Open Quantum Safe) and the
NIST-selected/round-3 PQC algorithms (Kyber, if applicable) to this project, while
keeping the original fallback code intact.



What I added
- scripts/setup_liboqs.sh         : Bash script to clone & build liboqs (Linux/macOS)
- scripts/setup_liboqs.ps1        : PowerShell script for Windows
- pqc/oqs_wrapper.py              : Python wrapper that prefers `oqs` (liboqs python bindings) and falls back to existing kyber_fallback.py
- scripts/integrate_liboqs.py     : Helper Python script to wire existing code to use oqs wrapper
- tests/test_oqs_integration.py   : Small test to confirm the oqs wrapper is available
- PROJECT_SNAPSHOT.txt            : List of files in original project
- README_INTEGRATION.md           : This file (same content)
- Updated top-level ZIP (project_with_liboqs_integration.zip)


Quick start (Windows PowerShell)
1. Open PowerShell as a normal user in the project root.
2. Run:
     .\scripts\setup_liboqs.ps1
3. Create Python virtualenv:
     python -m venv venv
     .\venv\Scripts\Activate.ps1
4. pip install -r requirements-pqc.txt
5. python .\scripts\integrate_liboqs.py
6. pytest tests/test_oqs_integration.py

How integration works:
The wrapper `pqc/oqs_wrapper.py` tries to import `oqs` (the official python bindings for liboqs).
If import works, it exposes `generate_keypair()`, `encapsulate()`, `decapsulate()` wrapper functions
    mapping to liboqs KEM APIs.
If import fails, it imports `crypto/kyber_fallback.py` and exposes the same API, so the rest of the project
    can use the same calls.
You can modify existing code (e.g., `quantum/gen_keys.py`) to call `from pqc.oqs_wrapper import generate_keypair`
  instead of the internal Kyber implementation to prefer liboqs.


