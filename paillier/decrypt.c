#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <paillier.h>


int main(int argc, char* argv[]) {
    // Arguments:
    // [1] pub
    // [2] prv
    // [3] data (hex)
    if (argc != 4) {
        printf("Usage: %s <pub> <prv> <data(hex)>", argv[0]);
    }

    paillier_prvkey_t* prv;
    paillier_pubkey_t* pub;
    paillier_plaintext_t* pt;
    paillier_ciphertext_t* ct;
    
    // Read in keys and data
    pub = paillier_pubkey_from_hex(argv[1]);
    prv = paillier_prvkey_from_hex(argv[2], pub);
    ct = (paillier_ciphertext_t *) malloc(sizeof(paillier_ciphertext_t));
    mpz_init_set_str(ct->c, argv[3], 16);

    // Decrypt
    pt = paillier_dec(NULL, pub, prv, ct);
    
    // Print out plain text
    printf("%d", mpz_get_ui(pt->m));
    
    // Clean up
    mpz_clear(pub->n);
    mpz_clear(pub->n_squared);
    mpz_clear(pub->n_plusone);
    free(pub);
    
    mpz_clear(prv->lambda);
    mpz_clear(prv->x);
    free(prv);
    
    mpz_clear(pt->m);
    free(pt);
    
    mpz_clear(ct->c);
    free(ct);

    return 0;
}
