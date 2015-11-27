#include <stdlib.h>
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


paillier_ciphertext_t *paillier_ciphertext_zero(paillier_pubkey_t *pub) {
    paillier_plaintext_t *pt = paillier_plaintext_from_ui(0);
    paillier_ciphertext_t *ct = paillier_enc(NULL, pub, pt, paillier_get_rand);
    paillier_freeplaintext(pt);
    return ct;
}
