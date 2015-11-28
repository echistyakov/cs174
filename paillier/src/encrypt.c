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
        printf("Usage: %s <pub> <data(int)>\n", argv[0]);
        return 1;
    }

    paillier_pubkey_t* pub;
    paillier_plaintext_t* pt;
    paillier_ciphertext_t* ct;
    
    // Read in key and data
    pub = paillier_pubkey_from_hex(argv[1]);
    pt = paillier_plaintext_from_ui(atoi(argv[2]));

    // Encrypt
    ct = paillier_enc(NULL, pub, pt, paillier_get_rand);

    // Print out ciphertext
    printf("%s\n", mpz_get_str(NULL, 16, ct->c));
    
    // Clean up
    paillier_freepubkey(pub);
    paillier_freeplaintext(pt);
    paillier_freeciphertext(ct);

    return 0;
}
