# pqc/oqs_wrapper.py
# Thin compatibility wrapper: prefer liboqs python bindings (oqs), otherwise fall back to project's kyber_fallback.
try:
    import oqs
    _HAS_OQS = True
except Exception:
    _HAS_OQS = False

if _HAS_OQS:
    # Use oqs KEM API
    def algorithm_list():
        return oqs.KEM.available_kems()

    def generate_keypair(alg_name='kyber_512_cca_kem'):
        kem = oqs.KEM(alg_name)
        pk = kem.generate_keypair()
        sk = kem.export_secret_key()
        kem.free()
        return pk, sk

    def encapsulate(pk, alg_name='kyber_512_cca_kem'):
        kem = oqs.KEM(alg_name)
        ct, ss = kem.encapsulate(pk)
        kem.free()
        return ct, ss

    def decapsulate(ct, sk, alg_name='kyber_512_cca_kem'):
        kem = oqs.KEM(alg_name)
        ss = kem.decapsulate(ct, sk)
        kem.free()
        return ss

else:
    # Fall back to project's internal kyber_fallback implementation
    from crypto.kyber_fallback import generate_keypair as _gk, encapsulate as _enc, decapsulate as _dec

    def algorithm_list():
        return ['kyber_fallback']

    def generate_keypair(*a, **k):
        return _gk(*a, **k)

    def encapsulate(*a, **k):
        return _enc(*a, **k)

    def decapsulate(*a, **k):
        return _dec(*a, **k)
