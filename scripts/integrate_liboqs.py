# scripts/integrate_liboqs.py
# Simple helper: attempts to import oqs and reports status. Creates pqc/USE_OQS when available.
import importlib, os, sys
try:
    oqs = importlib.import_module('oqs')
    print('oqs import successful. available KEMs:', oqs.KEM.available_kems())
    # create marker file
    with open(os.path.join(os.path.dirname(os.path.dirname(__file__)), 'pqc', 'USE_OQS'), 'w') as f:
        f.write('\n'.join(oqs.KEM.available_kems()))
    print('Created pqc/USE_OQS marker.')
except Exception as e:
    print('oqs import failed:', e)
    print('Project will continue using existing fallback (kyber_fallback).')
