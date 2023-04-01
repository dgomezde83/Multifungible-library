#include <openssl/ec.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include <memory.h>
#include <assert.h>
#include <pthread.h>
#include "util.h"
#include "AddressGenerator.h"

#if OPENSSL_VERSION_NUMBER >= 0x30000000L
#include <openssl/core_names.h>

static pthread_t TID[addressgenerator_max_threads];

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; // protect vc_simplevanitygen->vc_found_num

// return thread index in array TID, return -1 if not found (you need check again).
int get_thread_index1(int max_index) {
    pthread_t t = pthread_self();
    int i;
    for (i = 0; i < max_index; i++) {
        if (pthread_equal(TID[i], t) != 0) {
            return i;
        }
    }
    return -1;
}

void *
get_private_key(EVP_PKEY *pkey, unsigned char *pub_buf, size_t buf_len, size_t *output_len) {
    EC_KEY *ec_key = EVP_PKEY_get1_EC_KEY(pkey);
    BIGNUM *pkbn = EC_KEY_get0_private_key(ec_key);
    *output_len = BN_bn2bin(pkbn, pub_buf);
}

void *
thread_loop_simplevanitygen(void *arg) {
    unsigned char priv_buf[32];
    unsigned char pub_buf[128];  // 65 bytes enough
    size_t pub_buf_len = 128;

    char address[1024] = {'\0'};

    int find_it = 0;
    size_t pattern_len;

    int thread_index;

    EVP_PKEY *pkey = NULL;
    EVP_PKEY_CTX *pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);

    if (EVP_PKEY_keygen_init(pctx) != 1) {
        return NULL;
    }

    OSSL_PARAM params[2];
    params[0] = OSSL_PARAM_construct_utf8_string(OSSL_PKEY_PARAM_GROUP_NAME, SN_secp256k1, 0);
    params[1] = OSSL_PARAM_construct_end();
    EVP_PKEY_CTX_set_params(pctx, params);

    ALC * t_alc = (ALC *) arg;

    pattern_len = strlen(t_alc->pattern);

    check_thread_index:
    thread_index = get_thread_index1(t_alc->vc_thread_num);
    if (thread_index == -1) {
        // check again
        goto check_thread_index;
    }

    int output_timeout = 0;
    char *coin = "EGLD";

    while (!t_alc->vc_halt) {
        // Generate a key-pair
        // EVP_PKEY_keygen is slow!
        if (EVP_PKEY_keygen(pctx, &pkey) <= 0) {
            return NULL;
        }

        t_alc->vc_check_count[thread_index]++;
        output_timeout++;

        if (t_alc->vc_found_num >= t_alc->vc_numpairs) {
            t_alc->vc_halt = 1;
            pthread_mutex_unlock(&mtx);
            goto out;
        }

        t_alc->vc_found_num++;
        // get private key from EVP_PKEY
        size_t output_len = 0;
        get_private_key(pkey, (unsigned char *) &priv_buf, pub_buf_len, &output_len);
        cdumphex(t_alc->cpp_priv_address,priv_buf, output_len);

        pthread_mutex_unlock(&mtx);

        if (output_timeout > 1500) {
            output_timeout = 0;
        }
    }

    out:
    EVP_PKEY_CTX_free(pctx);
    EVP_PKEY_free(pkey);

    return NULL;
}

int start_threads_address_generation(ALC *p_alc) {
    int i;

    for (i = 0; i < p_alc->vc_thread_num; i++) {
        if (pthread_create(&TID[i], NULL, thread_loop_simplevanitygen, p_alc))
            return 0;
    }

    for (i = 0; i < p_alc->vc_thread_num; i++) {
        pthread_join(TID[i], NULL);
    }
    return 1;
}
#endif
