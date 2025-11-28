#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <oqs/oqs.h>

// Simple XOR "AES-like" encryption for demonstration
void simple_encrypt(unsigned char *data, size_t len, unsigned char key) {
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key;
    }
}

void simple_decrypt(unsigned char *data, size_t len, unsigned char key) {
    // XOR is symmetric
    simple_encrypt(data, len, key);
}

int main() {
    // ---------------------------
    // 1. Generate Kyber keypair
    // ---------------------------
    OQS_KEM *kem = OQS_KEM_new(OQS_KEM_alg_kyber_512);
    if (kem == NULL) {
        printf("Kyber algorithm not available.\n");
        return 1;
    }

    uint8_t *public_key = malloc(kem->length_public_key);
    uint8_t *secret_key = malloc(kem->length_secret_key);
    if (OQS_KEM_keypair(kem, public_key, secret_key) != OQS_SUCCESS) {
        printf("Keypair generation failed.\n");
        return 1;
    }
    printf("Kyber keypair generated successfully.\n");

    // ---------------------------
    // 2. Encapsulate shared secret
    // ---------------------------
    uint8_t *ciphertext = malloc(kem->length_ciphertext);
    uint8_t *shared_secret_enc = malloc(kem->length_shared_secret);

    if (OQS_KEM_encaps(kem, ciphertext, shared_secret_enc, public_key) != OQS_SUCCESS) {
        printf("Encapsulation failed.\n");
        return 1;
    }
    printf("Shared secret encapsulated.\n");

    // ---------------------------
    // 3. Decapsulate shared secret
    // ---------------------------
    uint8_t *shared_secret_dec = malloc(kem->length_shared_secret);
    if (OQS_KEM_decaps(kem, shared_secret_dec, ciphertext, secret_key) != OQS_SUCCESS) {
        printf("Decapsulation failed.\n");
        return 1;
    }

    // Verify shared secret
    if (memcmp(shared_secret_enc, shared_secret_dec, kem->length_shared_secret) == 0) {
        printf("Shared secret verified successfully.\n");
    } else {
        printf("Shared secret mismatch!\n");
    }

    // ---------------------------
    // 4. "Encrypt" a message using simple XOR
    // ---------------------------
    unsigned char message[] = "Hello, Kyber world!";
    size_t msg_len = strlen((char*)message);
    unsigned char key_byte = shared_secret_enc[0]; // just take first byte of shared secret
    simple_encrypt(message, msg_len, key_byte);
    printf("Encrypted message: ");
    for (size_t i = 0; i < msg_len; i++) printf("%02X ", message[i]);
    printf("\n");

    // ---------------------------
    // 5. Decrypt
    // ---------------------------
    simple_decrypt(message, msg_len, key_byte);
    printf("Decrypted message: %s\n", message);

    // ---------------------------
    // Cleanup
    // ---------------------------
    free(public_key);
    free(secret_key);
    free(ciphertext);
    free(shared_secret_enc);
    free(shared_secret_dec);
    OQS_KEM_free(kem);

    return 0;
}
