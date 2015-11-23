
char* sum_he(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);

my_bool sum_he_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
void sum_he_deinit(UDF_INIT *initid);

void sum_he_reset(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);
void sum_he_clear(UDF_INIT *initid, char *is_null, char *error);
void sum_he_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);

// Implementation

my_bool sum_he_init(UDF_INIT *initid, UDF_ARGS *args, char *message) {
    if (args->arg_count != 1) {
        strcpy(message, "SUM_HE() requires one argument");
        return 1;
    }
    
    if (args->arg_type[0] != STRING_RESULT) {
        strcpy(message, "SUM_HE() requires a BLOB");
        return 1;
    }
    
    initid->ptr = (char*) malloc(sizeof(mpz_t));
    if (!initid->ptr) {
        strcpy(message, "Couldn't allocate memory");
        return 1;
    }
    
    bzero(initid->ptr, sizeof(mpz_t));
    
    return 0;
}

void sum_he_deinit(UDF_INIT *initid) {
    if (initid->ptr) {
        free(initid->ptr);
    }
}

void sum_he_reset(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error) {
    sum_he_clear(initid, is_null, error);
    sum_he_add(initid, args, is_null, error);
}

void sum_he_clear(UDF_INIT *initid, char *is_null, char *error) {
    bzero(initid->ptr, sizeof(mpz_t));
}

void sum_he_add(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error) {
    // TODO: save into initid pointer
}

char* sum_he(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error) {
    mpz_t sum = (mpz_t*) initid->ptr;
    // TODO extract mpz_t as string an return
}