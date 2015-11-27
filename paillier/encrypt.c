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
    // Arguments:
    // [1] pub
    // [2] data (int)
    if (argc != 3) {
        printf("Usage: %s <pub> <data(int)>", argv[0]);
    }

    paillier_pubkey_t* pub;
    paillier_plaintext_t* pt;
    paillier_ciphertext_t* ct;
    
    // Read in key and data
    pub = paillier_pubkey_from_hex(argv[1]);
    pt = paillier_plaintext_from_ui(atoi(argv[3]));

    // Encrypt
    ct = paillier_enc(NULL, pub, pt, paillier_get_rand);

    // Print out ciphertext
    printf("%s", mpz_get_str(NULL, 16, ct->c));
    
    // Clean up
    mpz_clear(pub->n);
    mpz_clear(pub->n_squared);
    mpz_clear(pub->n_plus_one);
    free(pub);
    
    mpz_clear(prv->lambda);
    mpz_clear(prv->x);
    free(prv);
    
    mpz_clear(pt->m);
    free(pt);

    return 0;
}
