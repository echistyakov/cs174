#ifdef STANDARD
/* STANDARD is defined, don't use any mysql functions */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef __WIN__
typedef unsigned __int64 ulonglong;/* Microsofts 64 bit types */
typedef __int64 longlong;
#else
typedef unsigned long long ulonglong;
typedef long long longlong;
#endif /*__WIN__*/
#else
#include <my_global.h>
#include <my_sys.h>
#if defined(MYSQL_SERVER)
#include <m_string.h>/* To get strmov() */
#else
/* when compiled as standalone */
#include <string.h>
#define strmov(a,b) stpcpy(a,b)
#define bzero(a,b) memset(a,0,b)
#define memcpy_fixed(a,b,c) memcpy(a,b,c)
#endif
#endif
#include <mysql.h>
#include <ctype.h>

#ifdef HAVE_DLOPEN

#if !defined(HAVE_GETHOSTBYADDR_R) || !defined(HAVE_SOLARIS_STYLE_GETHOST)
static pthread_mutex_t LOCK_hostname;
#endif

//////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <paillier.h>


// Main function
char* sum_he(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);

// Init/deinit
my_bool sum_he_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
void sum_he_deinit(UDF_INIT *initid);

// Reset/clear/add
void sum_he_reset(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);
void sum_he_clear(UDF_INIT *initid, char *is_null, char *error);
void sum_he_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);

const char *PUB_HEX = "ffffffffffffffff";

void paillier_get_rand(void* buf, int len) {
    unsigned char *c = (unsigned char *) buf;
    int i;
    srand(time(NULL));  // Seed rand
    for (i = 0; i < len; i++) {
        c[i] = rand() % 256;
    }
}

typedef struct {
	paillier_ciphertext_t sum;
    paillier_pubkey_t pub;
} sum_he_t;

void sum_he_t_init(sum_he_t *sh) {
    // Set pub
    sh->pub = *paillier_pubkey_from_hex(PUB_HEX);

    // Set sum to 0
    paillier_plaintext_t pt = *paillier_plaintext_from_ui(0);
    sh->sum = *paillier_enc(NULL, sh->pub, pt, paillier_get_rand);
}

void sum_he_t_deinit(sum_he_t *sh) {
    // Clear pub
    mpz_clear(sh->pub.n);
    mpz_clear(sh->pub.n_squared);
    mpz_clear(sh->pub.n_plusone);
    
    // Clear sum
    mpz_clear(sh->sum.c);
}


// Implementation

my_bool sum_he_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    if (args->arg_count != 1) {
        strcpy(message, "SUM_HE() requires one argument");
        return 1;
    }
    
    if (args->arg_type[0] != STRING_RESULT) {
        strcpy(message, "SUM_HE() requires a TEXT");
        return 1;
    }
    
    initid->ptr = malloc(sizeof(sum_he_t));
    if (!initid->ptr) {
        strcpy(message, "Couldn't allocate memory");
        return 1;
    }

    return 0;
}

void sum_he_deinit(UDF_INIT *initid) {
    sum_he_t *sh = (sum_he_t *) initid->ptr;
    sum_he_t_deinit(sh);
    free(sh);
}

void sum_he_clear(UDF_INIT *initid, char *is_null, char *error) {
    sum_he_t_init((sum_he_t *) initid->ptr);
}

void sum_he_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error) {
    sum_he_t *sh = (sum_he_t *) initid->ptr;
    
    // Convert arg to ciphertext
    paillier_ciphertext_t ct;
    mpz_init_set_str(ct.c, (char *) args->args[0], 16);
    
    // Multiply
    paillier_ciphertext_t mul;
    mpz_init(mul.c);
    paillier_mul(&sh->pub, &mul, &ct, &sh->sum);
    sh->sum = mul;
}

void sum_he_reset(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error) {
    sum_he_clear(initid, is_null, error);
    sum_he_add(initid, args, is_null, error);
}

char* sum_he(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error) {
    sum_he_t *sh = (sum_he_t *) initid->ptr;
    return mpz_get_str(NULL, 16, sh->sum.c);
}

//////////////////////////////////////////////////////////////////
#endif /* HAVE_DLOPEN */
