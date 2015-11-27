#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <paillier.h>
#include "paillier_helpers.h"

const int MODULO_BITS = 128;


int main(int argc, char* argv[]) {
    // Generate key set
    paillier_pubkey_t* pub;
    paillier_prvkey_t* prv;
    paillier_keygen(MODULO_BITS, &pub, &prv, paillier_get_rand);
    
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
