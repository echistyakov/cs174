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
