#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <oqs/oqs.h>

int main() {
    // Choose the Kyber algorithm
    const char *alg = "Kyber512";
    if (!OQS_KEM_alg_is_enabled(alg)) {
        printf("%s is not enabled in this build.\n", alg);
        return EXIT_FAILURE;
    }

    OQS_KEM *kem = OQS_KEM_new(alg);
    if (kem == NULL) {
        printf("Failed to initialize KEM.\n");
        return EXIT_FAILURE;
    }

    // Allocate memory for keys and ciphertext
    uint8_t *pk = malloc(kem->length_public_key);
    uint8_t *sk = malloc(kem->length_secret_key);
    uint8_t *ct = malloc(kem->length_ciphertext);
    uint8_t *ss_enc = malloc(kem->length_shared_secret);
    uint8_t *ss_dec = malloc(kem->length_shared_secret);

    // Generate key pair
    if (OQS_KEM_keypair(kem, pk, sk) != OQS_SUCCESS) {
        printf("Key generation failed.\n");
        return EXIT_FAILURE;
    }
    printf("Kyber key pair generated successfully.\n");

    // Encapsulate shared secret
    if (OQS_KEM_encaps(kem, ct, ss_enc, pk) != OQS_SUCCESS) {
        printf("Encapsulation failed.\n");
        return EXIT_FAILURE;
    }
    printf("Message encapsulated successfully.\n");

    // Decapsulate shared secret
    if (OQS_KEM_decaps(kem, ss_dec, ct, sk) != OQS_SUCCESS) {
        printf("Decapsulation failed.\n");
        return EXIT_FAILURE;
    }
    printf("Message decapsulated successfully.\n");

    // Compare shared secrets
    if (memcmp(ss_enc, ss_dec, kem->length_shared_secret) == 0) {
        printf("Success: Shared secrets match!\n");
    } else {
        printf("Failure: Shared secrets do not match.\n");
    }

    // Free memory
    free(pk);
    free(sk);
    free(ct);
    free(ss_enc);
    free(ss_dec);
    OQS_KEM_free(kem);

    return EXIT_SUCCESS;
}
