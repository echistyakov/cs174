#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <paillier.h>


void paillier_get_rand(void* buf, int len) {
    unsigned char *c = (unsigned char *) buf;
    int i;
    for (i = 0; i < len; i++) {
        c[i] = rand() % 256;
    }
}


int main(int argc, char* argv[]) {
    // Generate key set
    const int modulobits = 128;
    const unsigned long num1 = 4973;
    const unsigned long num2 = 2879;
    
    paillier_pubkey_t* pub;
    paillier_prvkey_t* prv;
    paillier_keygen(modulobits, &pub, &prv, paillier_get_rand);
    
    // Create plaintexts
    paillier_plaintext_t *pt1 = paillier_plaintext_from_ui(num1);
    paillier_plaintext_t *pt2 = paillier_plaintext_from_ui(num2);
    printf("pt1 = %lu\n", mpz_get_ui(pt1->m));
    printf("pt2 = %lu\n", mpz_get_ui(pt2->m));

    // Encrypt plaintexts
    paillier_ciphertext_t *ct1 = (paillier_ciphertext_t *) malloc(sizeof(paillier_ciphertext_t));
    mpz_init(ct1->c);
    paillier_enc(ct1, pub, pt1, paillier_get_rand);
    paillier_ciphertext_t *ct2 = (paillier_ciphertext_t *) malloc(sizeof(paillier_ciphertext_t));
    mpz_init(ct2->c);
    paillier_enc(ct2, pub, pt2, paillier_get_rand);
    printf("ct1: %s\n", mpz_get_str(NULL, 16, ct1->c));
    printf("ct2: %s\n", mpz_get_str(NULL, 16, ct2->c));
    
    // Multiply
    paillier_ciphertext_t *mul = (paillier_ciphertext_t *) malloc(sizeof(paillier_ciphertext_t));
    mpz_init(mul->c);
    paillier_mul(pub, mul, ct1, ct2);

    printf("mul: %s\n", mpz_get_str(NULL, 16, mul->c));
    
    // Decrypt
    paillier_plaintext_t *added = paillier_dec(NULL, pub, prv, mul);
    printf("mul: %s\n", mpz_get_str(NULL, 16, added->m));
    
    printf("%d + %d = %d (should be: %d)\n", num1, num2, mpz_get_ui(added->m), num1 + num2);

    return 0;
}
