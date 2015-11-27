#ifndef PAILLIER_HELPERS_H
#define PAILLIER_HELPERS_H

void paillier_get_rand(void* buf, int len);
paillier_ciphertext_t *paillier_ciphertext_zero(paillier_pubkey_t *pub);

#endif