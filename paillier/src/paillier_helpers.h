#ifndef PAILLIER_HELPERS_H
#define PAILLIER_HELPERS_H

void paillier_get_rand(void* buf, int len);
void paillier_pubkey_free(paillier_pubkey_t *pub);
void paillier_prvkey_free(paillier_prvkey_t *prv);
void paillier_plaintext_free(paillier_plaintext_t *pt);
void paillier_ciphertext_free(paillier_ciphertext_t *ct);
paillier_ciphertext_t *paillier_ciphertext_zero(paillier_pubkey_t *pub);

#endif