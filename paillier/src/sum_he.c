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
//////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <paillier.h>
#include "paillier_helpers.h"

////////////////
// PROTOTYPES //
////////////////

// Main function
char* sum_he(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error);
// Init/deinit
my_bool sum_he_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
void sum_he_deinit(UDF_INIT *initid);
// Reset/clear/add
void sum_he_reset(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);
void sum_he_clear(UDF_INIT *initid, char *is_null, char *error);
void sum_he_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);

////////////////
// PUBLIC KEY //
////////////////

const char *PUB_HEX = "cc0b753172e2f753226965e9bb85d4f3";

///////////////////
// SUM_HE STRUCT //
///////////////////

typedef struct {
    paillier_pubkey_t *pub;
    paillier_ciphertext_t *sum;
    int row_count;
} sum_he_t;

void sum_he_t_init(sum_he_t *sh) {
    // Set pub
    sh->pub = paillier_pubkey_from_hex(PUB_HEX);
    // Set sum to 0
    sh->sum = paillier_create_enc_zero();
    // Set row count to 0
    sh->row_count = 0;
}

void sum_he_t_free(sum_he_t *sh) {
    paillier_freepubkey(sh->pub);
    paillier_freeciphertext(sh->sum);
    free(sh);
}

////////////////////
// IMPLEMENTATION //
////////////////////

my_bool sum_he_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    // Must be a single argument
    if (args->arg_count != 1) {
        strcpy(message, "SUM_HE() requires one argument");
        return 1;
    }

    // Must be an argument of type STRING
    if (args->arg_type[0] != STRING_RESULT) {
        strcpy(message, "SUM_HE() requires a STRING");
        return 1;
    }

    // Allocate memory, initialize struct
    initid->ptr = malloc(sizeof(sum_he_t));
    sum_he_t *sh = (sum_he_t *) initid->ptr;
    sum_he_t_init(sh);
    if (!sh) {
        strcpy(message, "Couldn't allocate memory");
        return 1;
    }

    // Result may be NULL
    initid->maybe_null = 1;

    // Initialization successful
    return 0;
}

void sum_he_deinit(UDF_INIT *initid) {
    // Free allocated resources
    sum_he_t *sh = (sum_he_t *) initid->ptr;
    sum_he_t_free(sh);
}

void sum_he_clear(UDF_INIT *initid, char *is_null, char *error) {
    sum_he_t *sh = (sum_he_t *) initid->ptr;
    // Reset sum to 0
    paillier_freeciphertext(sh->sum);
    sh->sum = paillier_create_enc_zero();
    // Reset row count to 0
    sh->row_count = 0;
}

void sum_he_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error) {
    sum_he_t *sh = (sum_he_t *) initid->ptr;
    
    // NULL values and empty strings are invalid
    if (args->args[0] == NULL || args->lengths[0] == 0) {
        *error = 1;
        return;
    }
    
    // Extract hex value from args
    char *hex_value = (char *) malloc(sizeof(char) * (args->lengths[0] + 1));
    memcpy(hex_value, args->args[0], args->lengths[0]);
    hex_value[args->lengths[0]] = 0;  // Insert terminating null character
    
    // Check hex_value for validity
    if (!is_hex_string(hex_value)) {
        *error = 1;
        free(hex_value);
        return;
    }

    // Convert arg to ciphertext
    paillier_ciphertext_t *ct = (paillier_ciphertext_t *) malloc(sizeof(paillier_ciphertext_t));
    mpz_init_set_str(ct->c, hex_value, 16);
    free(hex_value);

    // Multiply
    paillier_ciphertext_t *mul = (paillier_ciphertext_t *) malloc(sizeof(paillier_ciphertext_t));
    mpz_init(mul->c);
    paillier_mul(sh->pub, mul, ct, sh->sum);
    paillier_freeciphertext(sh->sum);
    paillier_freeciphertext(ct);
    sh->sum = mul;
    
    // Increment row count
    sh->row_count += 1;
}

void sum_he_reset(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error) {
    // Function only required for older versions of MySQL
    sum_he_clear(initid, is_null, error);
    sum_he_add(initid, args, is_null, error);
}

char* sum_he(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error) {
    sum_he_t *sh = (sum_he_t *) initid->ptr;
    // Return result only if no errors occurred and row_count is greater than zero
    if (*error != 1 && *is_null != 1 && sh->row_count > 0) {
        char *hex_result = mpz_get_str(NULL, 16, sh->sum->c);
        strcpy(result, hex_result);
        *length = strlen(hex_result);
        free(hex_result);
        return result;
    }
    // Return NULL otherwise
    else {
        *is_null = 1;
        return result;
    }
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

#endif /* HAVE_DLOPEN */
