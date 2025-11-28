# tests/test_oqs_integration.py
import importlib
def test_oqs_wrapper_importable():
    m = importlib.import_module('pqc.oqs_wrapper')
    # at least should expose generate_keypair function
    assert hasattr(m, 'generate_keypair')
