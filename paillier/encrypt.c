#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <paillier.h>
#include "paillier_helpers.h"


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
    paillier_pubkey_free(pub);
    paillier_plaintext_free(pt);
    paillier_ciphertext_free(ct);

    return 0;
}
