#ifndef PAILLIER_HELPERS_H
#define PAILLIER_HELPERS_H

void paillier_get_rand(void* buf, int len);
void paillier_pubkey_clear(paillier_pubkey_t *pub);
void paillier_prvkey_clear(paillier_prvkey_t *prv);
void paillier_plaintext_clear(paillier_plaintext_t *pt);
void paillier_ciphertext_clear(paillier_ciphertext_t *ct);

#endif