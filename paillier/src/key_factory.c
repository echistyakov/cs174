#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <paillier.h>
#include "paillier_helpers.h"


int main(int argc, char* argv[]) {
    // Arguments:
    // [1] modulus bits
    if (argc != 2) {
        printf("Usage: %s <modulus bits>\n", argv[0]);
        return 1;
    }
    int modulus_bits = atoi(argv[1]);
    
    // Check if modulus_bits is a power of two
    if (modulus_bits <= 0 || (modulus_bits & (modulus_bits - 1))) {
        printf("Error: modulus bits must be a power of 2\n");
        return 1;
    }
    
    // Generate key set
    paillier_pubkey_t* pub;
    paillier_prvkey_t* prv;
    paillier_keygen(modulus_bits, &pub, &prv, paillier_get_rand);
    
    // Obtain hex strings from key set
    char *pub_hex = paillier_pubkey_to_hex(pub);
    char *prv_hex = paillier_prvkey_to_hex(prv);
    
    FILE *fp;

    // Write pub key to file
    fp = fopen("pub.key", "w+");
    fprintf(fp, pub_hex);
    fclose(fp);
    
    // Write prv key to file
    fp = fopen("prv.key", "w+");
    fprintf(fp, prv_hex);
    fclose(fp);
    
    // Print out key set    
    printf("%s\n", pub_hex);
    printf("%s\n", prv_hex);
    
    // Clean up
    free(pub_hex);
    free(prv_hex);
    
    paillier_freepubkey(pub);
    paillier_freeprvkey(prv);

    return 0;
}
