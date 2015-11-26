#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <paillier.h>


void paillier_get_rand(void* buf, int len) {
    for (int i = 0; i < len; i++) {
        buf[i] = rand() % 256;
    }
}


int main(int argc, char* argv[]) {
    // Generate key set
    const int modulobits = 128;
    paillier_pubkey_t* pub;
    paillier_prvkey_t* prv;
    paillier_keygen(128, &pub, &prv, paillier_get_rand);
    
    // Create plaintexts
    paillier_plaintext_t *pt1 = paillier_plaintext_from_ui(4973);
    paillier_plaintext_t *pt2 = paillier_plaintext_from_ui(2879);
    printf("pt1 = %d\n", mpz_get_ui(pt1->m));
    printf("pt2 = %d\n", mpz_get_ui(pt2->m));

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
    
    printf("4973 + 2879 = %d\n", mpz_get_ui(added->m));
    
    return 0;
}
