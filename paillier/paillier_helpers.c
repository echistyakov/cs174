#include <gmp.h>
#include <paillier.h>
#include "paillier_helpers.h"

void paillier_get_rand(void* buf, int len) {
    unsigned char *c = (unsigned char *) buf;
    int i;
    srand(time(NULL));  // Seed rand
    for (i = 0; i < len; i++) {
        c[i] = rand() % 256;
    }
}

void paillier_pubkey_free(paillier_pubkey_t *pub) {
    if (pub) {
        mpz_clear(pub->n);
        mpz_clear(pub->n_squared);
        mpz_clear(pub->n_plusone);
        free(pub);
    }
}

void paillier_prvkey_free(paillier_prvkey_t *prv) {
    if (prv) {
        mpz_clear(prv->lambda);
        mpz_clear(prv->x);
        free(prv);
    }
}

void paillier_plaintext_free(paillier_plaintext_t *pt) {
    if (pt) {
        mpz_clear(pt->m);
        free(pt);
    }
}

void paillier_ciphertext_free(paillier_ciphertext_t *ct) {
    if (ct) {
        mpz_clear(ct->c);
        free(ct);
    }
}
