#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include "paillier.h"


int main(int argc, char* argv[]) {
    // Generate key set
    const int modulobits = 128;
    paillier_pubkey_t* pub;
	paillier_prvkey_t* prv;
    paillier_keygen(128, &pub, &prv, paillier_get_rand_devrandom);
    
    // Create plaintexts
    paillier_plaintext_t *pt1 = paillier_plaintext_from_ui(4973);
    paillier_plaintext_t *pt2 = paillier_plaintext_from_ui(2879);
    printf("pt1 = %d\n", mpz_get_ui(pt1->m));
    printf("pt2 = %d\n", mpz_get_ui(pt2->m));
    // Encrypt plaintexts
    paillier_ciphertext_t *ct1 = paillier_enc(NULL, pub, pt1, paillier_get_rand_devrandom);
    paillier_ciphertext_t *ct2 = paillier_enc(NULL, pub, pt1, paillier_get_rand_devrandom);
    printf("test4\n");
    
    // Multiply
    paillier_ciphertext_t *mul = (paillier_ciphertext_t *) malloc(sizeof(paillier_ciphertext_t));
    paillier_mul(pub, mul, ct1, ct2);
    printf("test5\n");
    
    // Decrypt
    paillier_plaintext_t *added = paillier_dec(NULL, pub, prv, mul);
    printf("test6\n");
    printf("test7\n");
    
    printf("4973 + 2879 = %d\n", mpz_get_ui(added->m));
    
    return 0;
}